

#include "stdafx.h"

#include "ioContactWeapon.h"
#include "ioBaseChar.h"

#include "SkillDefine.h"

#include "ioAniCollisionBoxGrp.h"
#include "ioCollisionBoxGrpManager.h"

ActionStopType ioContactWeapon::m_ActionStopType = AST_NONE;
ActionStopType ioContactWeapon::m_DieActionStopType = AST_NONE;

ioContactWeapon::ioContactWeapon( ioBaseChar *pOwner, const WeaponAttribute *pAttr )
: ioWeapon( pOwner, pAttr )
{
	SetPeculiarity( WP_DIVIDE_ACTION );
	m_UpdateState = CONTACT_CREATED;
	m_bFirstCheck = true;
	m_bDeadWait = false;
	m_bSkillWeapon = false;
	m_bArleadyCallWeapon = false;
	m_pSpecialBox = NULL;
}

ioContactWeapon::~ioContactWeapon()
{
	SAFEDELETE( m_pSpecialBox );
}

void ioContactWeapon::LoadProperty( ioINILoader &rkLoader )
{
	ioWeapon::LoadProperty( rkLoader );

	m_bDisableOwnerScale = rkLoader.LoadBool_e( "disable_owner_scale", false );

	m_dwLiveTime = rkLoader.LoadInt_e( "live_time", 0 );

	char szBuf[MAX_PATH];

	rkLoader.LoadString_e( "special_colision", "", szBuf, MAX_PATH );
	ioHashString szSpecialCol = szBuf;
	if ( !szSpecialCol.IsEmpty() )
	{
		SAFEDELETE( m_pSpecialBox );
		m_pSpecialBox = g_CollisionBoxGrpMgr.CreateAniCollisionGrp( szSpecialCol.c_str(), true );
	}

	m_bMoveWeapon = rkLoader.LoadBool_e( "move_weapon", false );

	LoadCallAttribute(  rkLoader );
	LoadCallAttributeFireDirectionList( rkLoader );
}

void ioContactWeapon::LoadCallAttribute( ioINILoader &rkLoader )
{
	char szBuf[MAX_PATH];

	m_CallWeaponInfo.m_iWeaponIdx = rkLoader.LoadInt_e( "call_attribute_index", 0 );		
	m_CallWeaponInfo.m_iResistanceIdx = rkLoader.LoadInt_e( "call_attribute_resistance", 0 );		

	rkLoader.LoadString_e( "call_attribute_wound_ani", "", szBuf, MAX_PATH );
	m_CallWeaponInfo.m_WoundedAnimation = szBuf;		

	m_CallWeaponInfo.m_dwWoundedDuration = rkLoader.LoadInt_e( "call_attribute_wound_duration", 0 );
	m_CallWeaponInfo.m_bWoundedLoopAni = rkLoader.LoadBool_e( "call_attribute_wound_loop_ani", false );
}

void ioContactWeapon::LoadCallAttributeFireDirectionList( ioINILoader &rkLoader )
{
	char szBuf[MAX_PATH] = "";
	char szKey[MAX_PATH] = "";

	int iCnt = rkLoader.LoadInt_e( "call_attribute_fire_dir_cnt", 0 );
	if( iCnt <= 0 ) return;

	m_CallAttFireList.clear();
	m_CallAttFireList.reserve( iCnt );

	for( int i= 0; i < iCnt; ++i )
	{
		ChangeDirInfo kInfo;

		wsprintf_e( szBuf, "call_attribute_fire%d_range", i+1 );
		kInfo.m_fChangeRange = rkLoader.LoadFloat( szBuf, 0.0f );

		wsprintf_e( szBuf, "call_attribute_fire%d_speed", i+1 );
		kInfo.m_fChangeSpeed = rkLoader.LoadFloat( szBuf, 0.0f );

		wsprintf_e( szBuf, "call_attribute_fire%d_angle_h", i+1 );
		kInfo.m_fChangeAngleH = rkLoader.LoadFloat( szBuf, 0.0f );

		wsprintf_e( szBuf, "call_attribute_fire%d_angle_v", i+1 );
		kInfo.m_fChangeAngleV = rkLoader.LoadFloat( szBuf, 0.0f );

		wsprintf_e( szBuf, "call_attribute_fire%d_offset_x", i+1 );
		kInfo.m_vFireOffset.x = rkLoader.LoadFloat( szBuf, 0.0f );

		wsprintf_e( szBuf, "call_attribute_fire%d_offset_y", i+1 );
		kInfo.m_vFireOffset.y = rkLoader.LoadFloat( szBuf, 0.0f );

		wsprintf_e( szBuf, "call_attribute_fire%d_offset_z", i+1 );
		kInfo.m_vFireOffset.z = rkLoader.LoadFloat( szBuf, 0.0f );

		m_CallAttFireList.push_back( kInfo );
	}

	std::sort( m_CallAttFireList.begin(), m_CallAttFireList.end(), ChangeDirSort() );
}

void ioContactWeapon::SetStartCollisionBox()
{
	ioPushStruct *pPush = g_PushStructListMgr.FindPushStruct( m_iStructIndex );
	if( pPush )
	{
		m_bStructWeapon = true;
	}

	ioDummyChar *pDummy = g_DummyCharMgr.FindDummyChar( GetOwner(), m_iDummyIndex );
	if( pDummy )
	{
		m_bDummyCharWeapon = true;
	}

	ioMachineStruct *pMachine = g_MachineStructMgr.FindMachineStruct( m_iMachineIndex );
	if( pMachine )
	{
		m_bMachineWeapon = true;
	}

	if( m_bStructWeapon )
		StartCollisionBoxByStruct();
	else if( m_bDummyCharWeapon )
		StartCollisionBoxByDummyChar();
	else if( m_bMachineWeapon )
		StartCollisionBoxByMachine();
	else
		StartCollisionBoxByOwner();
}

void ioContactWeapon::StartCollisionBoxByOwner()
{
	ioBaseChar *pOwner = GetOwner();
	if( pOwner )
	{
		if ( m_pSpecialBox )
			m_WorldAttackBox = pOwner->GetAttackCollisionBoxbyWeapon( m_pSpecialBox, m_CollisionType, m_bDisableOwnerScale );
		else
			m_WorldAttackBox = pOwner->GetAttackCollisionBox( m_CollisionType, m_bDisableOwnerScale );
	}
	else
	{
		m_WorldAttackBox.SetNull();
		LOG.PrintTimeAndLog( 0, "ioContactWeapon - %s Not Exist", m_OwnerName.c_str() );
	}

	m_UpdateState = CONTACT_CREATED;
}

void ioContactWeapon::StartCollisionBoxByStruct()
{
	ioPushStruct *pPush = g_PushStructListMgr.FindPushStruct( m_iStructIndex );
	if( pPush )
	{
		m_WorldAttackBox = pPush->GetAttackCollisionBox( m_CollisionType );
	}
	else
	{
		m_WorldAttackBox.SetNull();
		LOG.PrintTimeAndLog( 0, "ioContactWeapon - Struct Not Exist" );
	}

	m_UpdateState = CONTACT_CREATED;
}

void ioContactWeapon::StartCollisionBoxByDummyChar()
{
	ioDummyChar *pDummy = g_DummyCharMgr.FindDummyChar( GetOwner(), m_iDummyIndex );
	if( pDummy )
	{
		m_WorldAttackBox = pDummy->GetAttackCollisionBox( m_CollisionType );
	}
	else
	{
		m_WorldAttackBox.SetNull();
		LOG.PrintTimeAndLog( 0, "ioContactWeapon - DummyChar Not Exist" );
	}

	m_UpdateState = CONTACT_CREATED;
}

void ioContactWeapon::GetWorldAttackBoxList( std::vector< ioOrientBox > &vBoxList )
{
	vBoxList.reserve( 3 );
	vBoxList.push_back( m_PreAttackBox );
	vBoxList.push_back( m_MidAttackBox );
	vBoxList.push_back( m_WorldAttackBox );
}

bool ioContactWeapon::IsCollision( ioPlayEntity *pEntity, bool bMapCollision )
{
	D3DXVECTOR3 vColPoint( 0.0f, 0.0f, 0.0f );

	if( m_UpdateState == CONTACT_FIRST )
	{
		if( pEntity->IsWoundedCollision( m_PreAttackBox, &vColPoint ) )
		{
			return CheckCollisionLine( pEntity, vColPoint );
		}
	}

	if( pEntity->IsWoundedCollision( m_MidAttackBox, &vColPoint ) )
	{
		return CheckCollisionLine( pEntity, vColPoint );
	}

	if( pEntity->IsWoundedCollision( m_WorldAttackBox, &vColPoint ) )
	{
		return CheckCollisionLine( pEntity, vColPoint );
	}

	return false;
}

bool ioContactWeapon::CheckCollisionLine( ioPlayEntity *pTarget, const D3DXVECTOR3 &vColPoint )
{
	if( GetPiercingType() == CPT_ALL || GetPiercingType() == CPT_ALL_LIMITE )
		return true;

	// Ÿ���� ĳ���Ͱ� �ƴ϶�� �浹�� ó���Ѵ�
	ioBaseChar *pTargetChar = ToBaseChar( pTarget );
	if( !pTargetChar )	return true;

	ioBaseChar *pOwner = GetOwner();
	if( pOwner )
		return pOwner->CheckCollisionLine( vColPoint );

	return false;	// ������ ���� ������ �浹���Ѱɷ� ����
}

ioWeapon::WeaponType ioContactWeapon::GetType() const
{
	return WT_CONTACT;
}

ActionStopType ioContactWeapon::GetActionStopType() const
{
	return m_ActionStopType;
}

ActionStopType ioContactWeapon::GetDieActionStopType() const
{
	return m_DieActionStopType;
}

void ioContactWeapon::UpdateWorldAttackBox( float fTimePerSec, bool bSetNull )
{
	if( m_bStructWeapon )
		UpdateAttackBoxByStruct( fTimePerSec, bSetNull );
	else if( m_bDummyCharWeapon )
		UpdateAttackBoxByDummyChar( fTimePerSec, bSetNull );
	else if( m_bMachineWeapon )
		UpdateAttackBoxByMachine( fTimePerSec, bSetNull );
	else
		UpdateAttackBoxByOwner( fTimePerSec, bSetNull );
}

void ioContactWeapon::UpdateAttackBoxByOwner( float fTimePerSec, bool bSetNull )
{
	ioBaseChar *pOwner = GetOwner();

	if( bSetNull || !pOwner || !IsLive() )
	{
		m_PreAttackBox.SetNull();
		m_MidAttackBox.SetNull();
		m_WorldAttackBox.SetNull();
		m_WorldAttackBoxForMap.SetNull();
		return;
	}

	ioOrientBox eCharAttackBox;

	if ( m_pSpecialBox )
		eCharAttackBox = pOwner->GetAttackCollisionBoxbyWeapon( m_pSpecialBox, m_CollisionType, m_bDisableOwnerScale );
	else
		eCharAttackBox = pOwner->GetAttackCollisionBox( m_CollisionType, m_bDisableOwnerScale );

	m_PreAttackBox = m_WorldAttackBox;

	DWORD dwLooseTime = 0;
	if( pOwner )
	{
		dwLooseTime = pOwner->GetActionStopCurAdd();
	}

	float fFrameGap = fTimePerSec * FLOAT1000;

	DWORD dwCurTime = FRAMEGETTIME();
	DWORD dwLiveEndTime = m_dwCreatedTime + m_dwLiveTime + dwLooseTime;
	DWORD dwFrameEndTime = dwCurTime + (DWORD)fFrameGap;

	float fPreTime = (float)dwCurTime - fFrameGap;
	float fEndGap = (float)dwLiveEndTime - fPreTime;

	D3DXVECTOR3 vAxis;

	if( dwLiveEndTime >= dwFrameEndTime )
	{
		m_WorldAttackBox = eCharAttackBox;
	}
	else	// ���� �����ӳ����� ������ ������.
	{
		float fLerpRate = 0.0f;
		if( m_dwCreatedTime >= dwCurTime )	// ���� �����ӿ��� �����Ǿ���. 1������ �̳��� ����.
		{
			fLerpRate = (float)(m_dwLiveTime+dwLooseTime) / (float)( dwFrameEndTime - m_dwCreatedTime );
		}
		else	// ���������ӿ��� �����Ǿ���.
		{
			fLerpRate = fEndGap / (2 * fFrameGap);
		}

		fLerpRate = max( 0.0f, min( FLOAT1, fLerpRate ) );

		for( int i=0 ; i<3 ; i++ )
		{
			D3DXVec3Lerp( &vAxis, &m_PreAttackBox.GetAxis(i), &eCharAttackBox.GetAxis(i), fLerpRate );
			D3DXVec3Normalize( &vAxis, &vAxis );
			m_WorldAttackBox.SetAxis( i, vAxis );
		}

		D3DXVec3Lerp( &vAxis, &m_PreAttackBox.GetCenter(), &eCharAttackBox.GetCenter(), fLerpRate );
		m_WorldAttackBox.SetCenter( vAxis );
	}

	for( int i=0 ; i<3 ; i++ )
	{
		D3DXVec3Lerp( &vAxis, &m_PreAttackBox.GetAxis(i), &m_WorldAttackBox.GetAxis(i), FLOAT05 );
		D3DXVec3Normalize( &vAxis, &vAxis );
		m_MidAttackBox.SetAxis( i, vAxis );
		m_MidAttackBox.SetExtents( i, m_WorldAttackBox.GetExtents(i) );
	}

	D3DXVec3Lerp( &vAxis, &m_PreAttackBox.GetCenter(), &m_WorldAttackBox.GetCenter(), FLOAT05 );
	m_MidAttackBox.SetCenter( vAxis );


	if( m_UpdateState == CONTACT_CREATED )
		m_UpdateState = CONTACT_FIRST;
	else
		m_UpdateState = CONTACT_NORMAL;

	if( m_fOwnerChargeScaleRate > FLOAT1 )
	{
		for( int i=0 ; i<3 ; i++ )
		{
			m_PreAttackBox.SetExtents( i, eCharAttackBox.GetExtents(i) * m_fOwnerChargeScaleRate );
			m_MidAttackBox.SetExtents( i, eCharAttackBox.GetExtents(i) * m_fOwnerChargeScaleRate );
			m_WorldAttackBox.SetExtents( i, eCharAttackBox.GetExtents(i) * m_fOwnerChargeScaleRate );
		}
	}

	m_WorldAttackBoxForMap = m_WorldAttackBox;

	if( m_bFirstCheck )
	{
		m_bFirstCheck = false;
	}
}

void ioContactWeapon::UpdateAttackBoxByStruct( float fTimePerSec, bool bSetNull )
{
	ioPushStruct *pPush = g_PushStructListMgr.FindPushStruct( m_iStructIndex );

	if( bSetNull || !pPush || !IsLive() )
	{
		m_PreAttackBox.SetNull();
		m_MidAttackBox.SetNull();
		m_WorldAttackBox.SetNull();
		m_WorldAttackBoxForMap.SetNull();
		return;
	}

	m_PreAttackBox = m_WorldAttackBox;

	DWORD dwLooseTime = 0;

	float fFrameGap = fTimePerSec * FLOAT1000;
	DWORD dwCurTime = FRAMEGETTIME();
	DWORD dwLiveEndTime = m_dwCreatedTime + m_dwLiveTime + dwLooseTime;
	DWORD dwFrameEndTime = dwCurTime + (DWORD)fFrameGap;

	float fPreTime = (float)dwCurTime - fFrameGap;
	float fEndGap = (float)dwLiveEndTime - fPreTime;

	D3DXVECTOR3 vAxis;

	if( dwLiveEndTime >= dwFrameEndTime )
	{
		m_WorldAttackBox = pPush->GetAttackCollisionBox( m_CollisionType );
	}
	else	// ���� �����ӳ����� ������ ������.
	{
		float fLerpRate = 0.0f;
		if( m_dwCreatedTime >= dwCurTime )	// ���� �����ӿ��� �����Ǿ���. 1������ �̳��� ����.
		{
			fLerpRate = (float)(m_dwLiveTime+dwLooseTime) / (float)( dwFrameEndTime - m_dwCreatedTime );
		}
		else	// ���������ӿ��� �����Ǿ���.
		{
			fLerpRate = fEndGap / (2 * fFrameGap);
		}

		fLerpRate = max( 0.0f, min( FLOAT1, fLerpRate ) );

		ioOrientBox kCurAttackBox = pPush->GetAttackCollisionBox( m_CollisionType );

		for( int i=0 ; i<3 ; i++ )
		{
			D3DXVec3Lerp( &vAxis, &m_PreAttackBox.GetAxis(i), &kCurAttackBox.GetAxis(i), fLerpRate );
			D3DXVec3Normalize( &vAxis, &vAxis );
			m_WorldAttackBox.SetAxis( i, vAxis );
		}

		D3DXVec3Lerp( &vAxis, &m_PreAttackBox.GetCenter(), &kCurAttackBox.GetCenter(), fLerpRate );
		m_WorldAttackBox.SetCenter( vAxis );
	}

	for( int i=0 ; i<3 ; i++ )
	{
		D3DXVec3Lerp( &vAxis, &m_PreAttackBox.GetAxis(i), &m_WorldAttackBox.GetAxis(i), FLOAT05 );
		D3DXVec3Normalize( &vAxis, &vAxis );
		m_MidAttackBox.SetAxis( i, vAxis );
		m_MidAttackBox.SetExtents( i, m_WorldAttackBox.GetExtents(i) );
	}

	D3DXVec3Lerp( &vAxis, &m_PreAttackBox.GetCenter(), &m_WorldAttackBox.GetCenter(), FLOAT05 );
	m_MidAttackBox.SetCenter( vAxis );

	if( m_UpdateState == CONTACT_CREATED )
		m_UpdateState = CONTACT_FIRST;
	else
		m_UpdateState = CONTACT_NORMAL;

	m_WorldAttackBoxForMap = m_WorldAttackBox;

	if( m_bFirstCheck )
	{
		m_bFirstCheck = false;
	}
}

void ioContactWeapon::UpdateAttackBoxByDummyChar( float fTimePerSec, bool bSetNull )
{
	ioDummyChar *pDummy = g_DummyCharMgr.FindDummyChar( GetOwner(), m_iDummyIndex );

	if( bSetNull || !pDummy || !IsLive() )
	{
		m_PreAttackBox.SetNull();
		m_MidAttackBox.SetNull();
		m_WorldAttackBox.SetNull();
		m_WorldAttackBoxForMap.SetNull();
		return;
	}

	m_PreAttackBox = m_WorldAttackBox;

	DWORD dwLooseTime = 0;
	DWORD dwCurTime = FRAMEGETTIME();
	DWORD dwLiveEndTime = m_dwCreatedTime + m_dwLiveTime + dwLooseTime;
	DWORD dwFrameEndTime = dwCurTime + (DWORD)( fTimePerSec * FLOAT1000 );
	D3DXVECTOR3 vAxis;

	if( dwLiveEndTime >= dwFrameEndTime )
	{
		m_WorldAttackBox = pDummy->GetAttackCollisionBox( m_CollisionType );
	}
	else	// ���� �����ӳ����� ������ ������.
	{
		float fLerpRate = 0.0f;
		if( m_dwCreatedTime >= dwCurTime )	// ���� �����ӿ��� �����Ǿ���. 1������ �̳��� ����.
		{
			fLerpRate = (float)(m_dwLiveTime+dwLooseTime) / (float)( dwFrameEndTime - m_dwCreatedTime );
		}
		else	// ���������ӿ��� �����Ǿ���.
		{
			fLerpRate = (float)(dwLiveEndTime - dwCurTime)/(float)( dwFrameEndTime - dwCurTime );
		}

		fLerpRate = max( 0.0f, min( FLOAT1, fLerpRate ) );

		ioOrientBox kCurAttackBox = pDummy->GetAttackCollisionBox( m_CollisionType );

		for( int i=0 ; i<3 ; i++ )
		{
			D3DXVec3Lerp( &vAxis, &m_PreAttackBox.GetAxis(i), &kCurAttackBox.GetAxis(i), fLerpRate );
			D3DXVec3Normalize( &vAxis, &vAxis );
			m_WorldAttackBox.SetAxis( i, vAxis );
		}

		D3DXVec3Lerp( &vAxis, &m_PreAttackBox.GetCenter(), &kCurAttackBox.GetCenter(), fLerpRate );
		m_WorldAttackBox.SetCenter( vAxis );
	}

	for( int i=0 ; i<3 ; i++ )
	{
		D3DXVec3Lerp( &vAxis, &m_PreAttackBox.GetAxis(i), &m_WorldAttackBox.GetAxis(i), FLOAT05 );
		D3DXVec3Normalize( &vAxis, &vAxis );
		m_MidAttackBox.SetAxis( i, vAxis );
		m_MidAttackBox.SetExtents( i, m_WorldAttackBox.GetExtents(i) );
	}

	D3DXVec3Lerp( &vAxis, &m_PreAttackBox.GetCenter(), &m_WorldAttackBox.GetCenter(), FLOAT05 );
	m_MidAttackBox.SetCenter( vAxis );

	if( m_UpdateState == CONTACT_CREATED )
		m_UpdateState = CONTACT_FIRST;
	else
		m_UpdateState = CONTACT_NORMAL;

	m_WorldAttackBoxForMap = m_WorldAttackBox;

	if( m_bFirstCheck )
	{
		m_bFirstCheck = false;
	}
}

void ioContactWeapon::Process( float fTimePerSec, ioPlayStage *pStage )
{
	DWORD dwCurTime = FRAMEGETTIME();
	if( m_dwCreatedTime > dwCurTime )
	{
		LOG.PrintTimeAndLog( 0, "ioContactWeapon::Process - Create Time Error(%d/%d)", m_dwCreatedTime, dwCurTime );
		SetWeaponDead();
		return;
	}

	if( m_bDeadWait )
	{
		SetWeaponDead();
		return;
	}

	if( !CheckPiercing() )
		return;

	DWORD dwLooseTime = 0;
	if( GetOwner() )
	{
		dwLooseTime = GetOwner()->GetActionStopCurAdd();
	}

	if( m_dwCreatedTime + m_dwLiveTime + dwLooseTime < dwCurTime )
	{
		m_bDeadWait = true;
	}

	UpdateWorldAttackBox( fTimePerSec, false );
}

void ioContactWeapon::SetLiveTime( DWORD dwTime )
{
	if( m_dwLiveTime == 0 )
	{
		m_dwLiveTime = dwTime;
	}
}

void ioContactWeapon::StartCollisionBoxByMachine()
{
	ioMachineStruct *pMachine = g_MachineStructMgr.FindMachineStruct( m_iMachineIndex );
	if( pMachine )
	{
		m_WorldAttackBox = pMachine->GetAttackCollisionBox( m_CollisionType );
	}
	else
	{
		m_WorldAttackBox.SetNull();
		LOG.PrintTimeAndLog( 0, "ioContactWeapon - Machine Not Exist" );
	}

	m_UpdateState = CONTACT_CREATED;
}

void ioContactWeapon::UpdateAttackBoxByMachine( float fTimePerSec, bool bSetNull )
{
	ioMachineStruct *pMachine = g_MachineStructMgr.FindMachineStruct( m_iMachineIndex );

	if( bSetNull || !pMachine || !IsLive() )
	{
		m_PreAttackBox.SetNull();
		m_MidAttackBox.SetNull();
		m_WorldAttackBox.SetNull();
		m_WorldAttackBoxForMap.SetNull();
		return;
	}

	m_PreAttackBox = m_WorldAttackBox;

	DWORD dwLooseTime = 0;

	float fFrameGap = fTimePerSec * FLOAT1000;
	DWORD dwCurTime = FRAMEGETTIME();
	DWORD dwLiveEndTime = m_dwCreatedTime + m_dwLiveTime + dwLooseTime;
	DWORD dwFrameEndTime = dwCurTime + (DWORD)fFrameGap;

	float fPreTime = (float)dwCurTime - fFrameGap;
	float fEndGap = (float)dwLiveEndTime - fPreTime;

	D3DXVECTOR3 vAxis;

	if( dwLiveEndTime >= dwFrameEndTime )
	{
		m_WorldAttackBox = pMachine->GetAttackCollisionBox( m_CollisionType );
	}
	else	// ���� �����ӳ����� ������ ������.
	{
		float fLerpRate = 0.0f;
		if( m_dwCreatedTime >= dwCurTime )	// ���� �����ӿ��� �����Ǿ���. 1������ �̳��� ����.
		{
			fLerpRate = (float)(m_dwLiveTime+dwLooseTime) / (float)( dwFrameEndTime - m_dwCreatedTime );
		}
		else	// ���������ӿ��� �����Ǿ���.
		{
			fLerpRate = fEndGap / (2 * fFrameGap);
		}

		fLerpRate = max( 0.0f, min( FLOAT1, fLerpRate ) );

		ioOrientBox kCurAttackBox = pMachine->GetAttackCollisionBox( m_CollisionType );

		for( int i=0 ; i<3 ; i++ )
		{
			D3DXVec3Lerp( &vAxis, &m_PreAttackBox.GetAxis(i), &kCurAttackBox.GetAxis(i), fLerpRate );
			D3DXVec3Normalize( &vAxis, &vAxis );
			m_WorldAttackBox.SetAxis( i, vAxis );
		}

		D3DXVec3Lerp( &vAxis, &m_PreAttackBox.GetCenter(), &kCurAttackBox.GetCenter(), fLerpRate );
		m_WorldAttackBox.SetCenter( vAxis );
	}

	for( int i=0 ; i<3 ; i++ )
	{
		D3DXVec3Lerp( &vAxis, &m_PreAttackBox.GetAxis(i), &m_WorldAttackBox.GetAxis(i), FLOAT05 );
		D3DXVec3Normalize( &vAxis, &vAxis );
		m_MidAttackBox.SetAxis( i, vAxis );
		m_MidAttackBox.SetExtents( i, m_WorldAttackBox.GetExtents(i) );
	}

	D3DXVec3Lerp( &vAxis, &m_PreAttackBox.GetCenter(), &m_WorldAttackBox.GetCenter(), FLOAT05 );
	m_MidAttackBox.SetCenter( vAxis );

	if( m_UpdateState == CONTACT_CREATED )
		m_UpdateState = CONTACT_FIRST;
	else
		m_UpdateState = CONTACT_NORMAL;

	m_WorldAttackBoxForMap = m_WorldAttackBox;

	if( m_bFirstCheck )
	{
		m_bFirstCheck = false;
	}
}

bool ioContactWeapon::TestMapCollision( ioOpcodeShape *pShape, ioPlayStage *pStage, const D3DXMATRIX* pShapeWorldMat )
{
	//pShape�� ���� �� �浹 �˻縦 ���� �ϰ��� �Ҷ� TRUE
	//�Ϲ� ����� ��� ���� �޽��� 1�������� �Ͽ�¡�ý����� ��� N�� �̱� ������ TestMapCollision() �Լ��� ������ ȣ���

	if( !GetOwner() )
		return true;
	
	if( GetOwner()->GetState() == CS_USING_SKILL )
	{
		ioSkill *pSkill = GetOwner()->GetCurActiveSkill();
		if( pSkill )
		{
			ioDashHandAttackSkill *pHand = ToDashHandAttackSkill( pSkill );
			if( pHand )
			{
				int iFaceIndex = 0;
				if( !pShape->FindIntersection( pShapeWorldMat, GetWorldAttackBox(true), &iFaceIndex ) )
					return false;

				pHand->SetCollisionByApplySkill();		
			}
		}
	}

	return true;
}

const D3DXVECTOR3& ioContactWeapon::GetAttchBoxPosition()
{
	return m_WorldAttackBox.GetCenter();
}

void ioContactWeapon::CheckCollision( ioBaseChar *pChar, bool bApplyCol )
{
	if( !pChar ) return;
	if ( m_bSkillWeapon )
	{
		if ( bApplyCol )
		{
			CharCollision();
		}
	}	
}

void ioContactWeapon::CharCollision()
{
	ioBaseChar *pOwner = GetOwner();
	if( !pOwner )
		return;

	ioComboJumpAttackSkill *pComboJump = ToComboJumpAttackSkill( pOwner->GetCurActiveSkill() );
	if ( m_bSkillWeapon && pComboJump )
	{
		pComboJump->SetWeaponAttack( true );
	}

}

const D3DXVECTOR3& ioContactWeapon::GetPosition() const
{
	if( m_bMoveWeapon )
		return m_WorldAttackBoxForMap.GetCenter();

	return __super::GetPosition();
}

void ioContactWeapon::CallAttribute()
{	
	if( m_bArleadyCallWeapon )
		return;

	if( m_CallAttFireList.empty() )
		return;

	ioBaseChar *pOwner = GetOwner();
	if( !pOwner )	return;	

	DWORD dwCurTime = FRAMEGETTIME();
	DWORD dwCurGenerateTime = dwCurTime;

	for( ChangeDirInfoList::size_type i = 0; i < m_CallAttFireList.size(); ++i )
	{
		D3DXVECTOR3 vDir = pOwner->GetTargetRot() * D3DXVECTOR3( 0.0f, 0.0f, -1.0f );
		D3DXVec3Normalize(&vDir, &vDir);
		float fAngleH = ioMath::ArrangeHead( m_CallAttFireList[i].m_fChangeAngleH );
		float fAngleV = ioMath::ArrangeHead( m_CallAttFireList[i].m_fChangeAngleV );

		//Offset
		D3DXVECTOR3	vPos = GetPosition();
		ioPlayEntityHelp::MoveOffset( m_CallAttFireList[i].m_vFireOffset, pOwner->GetTargetRot(), vPos );

		D3DXQUATERNION qtRot;
		D3DXVECTOR3 vAxis;
		D3DXVec3Cross( &vAxis, &vDir, &D3DXVECTOR3( 0.0f, -1.0f, 0.0f ) );

		// vertical
		D3DXQuaternionRotationAxis( &qtRot, &vAxis, DEGtoRAD(fAngleV) );
		vDir = qtRot * vDir;

		// horizon
		D3DXQuaternionRotationAxis( &qtRot, &ioMath::UNIT_Y, DEGtoRAD(fAngleH) );
		vDir = qtRot * vDir;
		
		g_WeaponMgr.SetReserveCallWeaponInfo( GetOwnerName(), 
			m_CallWeaponInfo,
			vPos,
			-vDir,											
			dwCurGenerateTime,
			m_dwWeaponIdx );

		dwCurGenerateTime += 10;
	}

	m_bArleadyCallWeapon = true;

}

void ioContactWeapon::SetWeaponDead()
{
	ioWeapon::SetWeaponDead();
	CallAttribute();
}