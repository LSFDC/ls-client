

#include "stdafx.h"

#include "ioMissileWeapon5.h"
#include "ioPlayStage.h"

#include "ioBaseChar.h"

ActionStopType ioMissileWeapon5::m_ActionStopType = AST_NONE;
ActionStopType ioMissileWeapon5::m_DieActionStopType = AST_NONE;

ioMissileWeapon5::ioMissileWeapon5( ioBaseChar *pOwner, const WeaponAttribute *pAttr )
: ioWeapon( pOwner, pAttr )
{
	SetPeculiarity( WP_ONE_TIME_USE | WP_MISSILE_WEAPON | WP_DIVIDE_ACTION );

	m_pFireBall = NULL;

	m_fCurMoveSpeed = 0.0f;
	m_fCurMoveRange = 0.0f;
	m_fCurMoveRate = 0.0f;
	
	m_bFirst = true;
	m_bSetGraceClose = false;

	m_pSecondAttr = NULL;

	m_bArleadyCallWeapon = false;

	m_dwNextCallShadowWeaponTime = 0;

	//m_vWeaponPosList.clear();
	m_vMoveInfoList.clear();

	m_MoveState = NONE_MOVE;
}

ioMissileWeapon5::~ioMissileWeapon5()
{
	if( m_pFireBall )
	{
		LOG.PrintTimeAndLog( 0, "ioMissileWeapon5::~ioMissileWeapon5 - UnReleased Resouce" );
	}

	m_vSecondAttrList.clear();
}

bool ioMissileWeapon5::InitResource()
{
	m_dwNextCallShadowWeaponTime = FRAMEGETTIME() + m_dwCallShadowWeaponDuration;

	if( ioWeapon::InitResource() )
	{
		DestroyResource();
		return CreateFireBall( GetAttribute()->m_WeaponEffect );
	}

	return false;
}

void ioMissileWeapon5::LoadProperty( ioINILoader &rkLoader )
{
	ioWeapon::LoadProperty( rkLoader );

	char szBuf[MAX_PATH] = "";
	
	m_bDisableOwnerScale = rkLoader.LoadBool_e( "disable_owner_scale", false );

	m_vStartOffSet.x = rkLoader.LoadFloat_e( "start_offset_x", 0.0f );
	m_vStartOffSet.y = rkLoader.LoadFloat_e( "start_offset_y", 0.0f );
	m_vStartOffSet.z = rkLoader.LoadFloat_e( "start_offset_z", 0.0f );

	LoadCallAttribute( rkLoader );
	LoadCallAttributeFireDirectionList( rkLoader );

	int iCnt = rkLoader.LoadInt_e( "change_dir_cnt", 0 );
	if( iCnt <= 0 ) return;

	m_ChangeDirInfoList.clear();
	m_ChangeDirInfoList.reserve( iCnt );

	for( int i=0; i < iCnt; ++i )
	{
		ChangeDirInfo kInfo;

		wsprintf_e( szBuf, "change_range%d", i+1 );
		kInfo.m_fChangeRange = rkLoader.LoadFloat( szBuf, 0.0f );

		wsprintf_e( szBuf, "change_speed%d", i+1 );
		kInfo.m_fChangeSpeed = rkLoader.LoadFloat( szBuf, 0.0f );

		wsprintf_e( szBuf, "change_angle_h%d", i+1 );
		kInfo.m_fChangeAngleH = rkLoader.LoadFloat( szBuf, 0.0f );

		wsprintf_e( szBuf, "change_angle_v%d", i+1 );
		kInfo.m_fChangeAngleV = rkLoader.LoadFloat( szBuf, 0.0f );

		m_ChangeDirInfoList.push_back( kInfo );
	}

	std::sort( m_ChangeDirInfoList.begin(), m_ChangeDirInfoList.end(), ChangeDirSort() );

	m_CallShadowWeapon.m_iWeaponIdx = rkLoader.LoadInt_e( "call_shadow_weapon", NO_WEAPON_ATTRIBUTE );
	m_dwCallShadowWeaponDuration    = (DWORD)rkLoader.LoadInt_e( "call_shadow_weapon_delay_time", 2000 );
	m_fAddShadowWeaponHeight = rkLoader.LoadFloat_e( "call_shadow_weapon_add_height", 0.f );

	//////////////////////////////////////////////////////////////////////////
	m_fMoveSpeed = rkLoader.LoadFloat_e( "weapon_move_speed", 0.f );
}

void ioMissileWeapon5::LoadCallAttribute( ioINILoader &rkLoader )
{
	char szBuf[MAX_PATH];

	m_CallWeaponInfo.m_iWeaponIdx = rkLoader.LoadInt_e( "call_attribute_index", 0 );		
	m_CallWeaponInfo.m_iResistanceIdx = rkLoader.LoadInt_e( "call_attribute_resistance", 0 );		

	rkLoader.LoadString_e( "call_attribute_wound_ani", "", szBuf, MAX_PATH );
	m_CallWeaponInfo.m_WoundedAnimation = szBuf;		

	m_CallWeaponInfo.m_dwWoundedDuration = rkLoader.LoadInt_e( "call_attribute_wound_duration", 0 );
	m_CallWeaponInfo.m_bWoundedLoopAni = rkLoader.LoadBool_e( "call_attribute_wound_loop_ani", false );	
}

void ioMissileWeapon5::LoadCallAttributeFireDirectionList( ioINILoader &rkLoader )
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

		m_CallAttFireList.push_back( kInfo );
	}

	std::sort( m_CallAttFireList.begin(), m_CallAttFireList.end(), ChangeDirSort() );
}


bool ioMissileWeapon5::CreateFireBall( const ioHashString &szFileName )
{
	ioBaseChar *pOwner = GetOwner();
	if( !pOwner )	return false;

	m_pFireBall = g_EffectFactory.CreateEffect( szFileName );
	if( !m_pFireBall )	return false;

	GetParentSceneNode()->AttachObject( m_pFireBall );

	if( m_bDisableOwnerScale )
		GetParentSceneNode()->SetScale( 1.0f, 1.0f, 1.0f );

	ioHashString szLinkBiped;
	D3DXVECTOR3 vLinkPosOffset;
	D3DXQUATERNION qtLinkRotOffset;

	if( m_pFireBall->GetSkeletonName().IsEmpty() && m_pFireBall->GetFirstLinkOffset(szLinkBiped, vLinkPosOffset, qtLinkRotOffset ) )
	{
		ioBiped *pLinked = pOwner->GetGroup()->GetBiped( szLinkBiped );
		if( pLinked )
		{
			vLinkPosOffset = pLinked->GetDerivedOrientation() * vLinkPosOffset + pLinked->GetDerivedPosition();
			vLinkPosOffset = vLinkPosOffset * pOwner->GetWorldScale();
			vLinkPosOffset = pOwner->GetWorldOrientation() * vLinkPosOffset + pOwner->GetWorldPosition();
			SetPosition( vLinkPosOffset );
		}
	}

	m_StartPos = GetPosition();

	AddEffectEvent( m_pFireBall );
	return true;
}

bool ioMissileWeapon5::ChangeWeaponEffect( const ioHashString& szEffectName )
{
	ioBaseChar *pOwner = GetOwner();
	if( !pOwner )	return false;

	if( szEffectName.IsEmpty() )
		return false;

	if( m_pFireBall )
	{
		g_EffectFactory.DestroyEffect( m_pFireBall );
		m_pFireBall = NULL;
	}

	m_pFireBall = g_EffectFactory.CreateEffect( szEffectName );
	if( !m_pFireBall )	return false;

	GetParentSceneNode()->AttachObject( m_pFireBall );

	AddEffectEvent( m_pFireBall );
	return true;
}

void ioMissileWeapon5::SetMoveDir( const D3DXVECTOR3 &vDir )
{
	m_vMoveDir = vDir;

	/*D3DXQUATERNION qtRot;
	ioMath::CalcDirectionQuaternion( qtRot, -vDir );
	SetOrientation( qtRot );*/

	D3DXQUATERNION qtRot;
	ioPlayEntityHelp::CheckOrientation( vDir, qtRot );
	SetOrientation( qtRot );
}

void ioMissileWeapon5::SetMoveSpeed( float fSpeed )
{
	m_fCurMoveSpeed = fSpeed;
}

void ioMissileWeapon5::CheckChangeDir()
{
	/*if( m_ChangeDirInfoList.empty() ) return;

	float fChangeRange = m_ChangeDirInfoList[0].m_fChangeRange;

	if( m_fCurMoveRange >= fChangeRange )
	{
		if( m_ChangeDirInfoList[0].m_fChangeSpeed > 0.0f )
			m_fCurMoveSpeed = m_ChangeDirInfoList[0].m_fChangeSpeed;

		RotateMoveDir( m_ChangeDirInfoList[0].m_fChangeAngleH, m_ChangeDirInfoList[0].m_fChangeAngleV );
		m_ChangeDirInfoList.erase( m_ChangeDirInfoList.begin() );
	}*/
	if( m_vMoveInfoList.empty() )
		return;

	SetMoveSpeed( m_fMoveSpeed );

	float fChangeRange = m_vMoveInfoList[0].fRange;
	if( m_fCurMoveRange >= fChangeRange )
	{
		m_fCurMoveRange = 0.f;
		m_MoveState = POSITION_MOVE;

		if( m_vMoveInfoList.size() >= 2 )
		{
			D3DXVECTOR3 vNowPos = m_vMoveInfoList[0].vPos;
			SetPosition( vNowPos );
			D3DXVECTOR3 vNextPos = m_vMoveInfoList[1].vPos;

			D3DXVECTOR3 vMoveDir = vNextPos - vNowPos;
			vMoveDir.y = 0.f;
			D3DXVec3Normalize( &vMoveDir, &vMoveDir );
			SetMoveDir( vMoveDir );
		}

		m_vMoveInfoList.erase( m_vMoveInfoList.begin() );
	}
}

void ioMissileWeapon5::RotateMoveDir( float fAngleH, float fAngleV )
{
	fAngleH = ioMath::ArrangeHead( fAngleH );
	fAngleV = ioMath::ArrangeHead( fAngleV );

	D3DXVECTOR3 vAxis;
	D3DXVec3Cross( &vAxis, &m_vMoveDir, &D3DXVECTOR3( 0.0f, -1.0f, 0.0f ) );

	// vertical
	D3DXQUATERNION qtRot;
	D3DXQuaternionRotationAxis( &qtRot, &vAxis, DEGtoRAD(fAngleV) );
	m_vMoveDir = qtRot * m_vMoveDir;

	// horizon
	D3DXQuaternionRotationAxis( &qtRot, &ioMath::UNIT_Y, DEGtoRAD(fAngleH) );
	m_vMoveDir = qtRot * m_vMoveDir;

	D3DXQUATERNION qtRotDir;
	ioPlayEntityHelp::CheckOrientation( m_vMoveDir, qtRotDir );
	SetOrientation( qtRotDir );
}

void ioMissileWeapon5::SetStartPosition( const D3DXVECTOR3 &vPos, ioPlayStage *pStage )
{
	if( m_pFireBall && m_pFireBall->GetSkeletonName().IsEmpty() && m_pFireBall->HasLinkedSystem() )
		return;

	ioBaseChar *pOwner = GetOwner();
	D3DXVECTOR3 vNewPos = vPos;
	if( pOwner && m_vStartOffSet != ioMath::VEC3_ZERO )
	{
		D3DXQUATERNION qtRot = pOwner->GetTargetRot();
		D3DXVECTOR3 vOffSet = qtRot * m_vStartOffSet;

		vNewPos += vOffSet;
	}

	SetPosition( vNewPos );
	m_StartPos = GetPosition();
}

void ioMissileWeapon5::SetGraceClose()
{
	if( m_bSetGraceClose )	return;

	if( m_pFireBall )
	{
		if( m_bEndEffectGrace )
			m_pFireBall->EndEffectGrace();
		else
			m_pFireBall->EndEffectForce();
	}

	StopLoopSound();
	m_bSetGraceClose = true;
}

void ioMissileWeapon5::Process( float fTimePerSec, ioPlayStage *pStage )
{
	if( !CheckPiercing() )
		return;
	
	if( !m_bFirst )
	{
		CheckChangeDir();

		float fMoveDist = GetMoveSpeed() * fTimePerSec;
		GetParentSceneNode()->Translate( m_vMoveDir * fMoveDist );
		m_fCurMoveRange += fMoveDist;

		float fMaxRange = GetMaxRange() + m_fGrowthMoveRange;
		if( fMaxRange > 0.0f )
			m_fCurMoveRate = m_fCurMoveRange / fMaxRange;

		if( m_fCurMoveRange < fMaxRange )
		{
			UpdateWorldAttackBox( fTimePerSec, false );
		}
		else
		{
			UpdateWorldAttackBox( fTimePerSec, true );

			SetGraceClose();

			if( !m_pFireBall || !m_pFireBall->IsLive() )
			{
				SetWeaponDead();
			}
		}

		if( m_MoveState == POSITION_MOVE )
		{
			D3DXVECTOR3 vPos = GetPosition();
			float fHeight = pStage->GetMapHeightOnlyTerrain( vPos.x, vPos.z );
			if( fHeight > 10.f )
			{
				vPos.y = fHeight;
				SetPosition( vPos );
			}
		}

		if( m_vMoveInfoList.size() == 0 )
			SetWeaponDead();
	}

	m_bFirst = false;
	
	if( m_pFireBall )
	{
		m_pFireBall->Update( fTimePerSec * 1000.0f );
	}

	DWORD dwCurTime = FRAMEGETTIME();
	if( m_CallShadowWeapon.m_iWeaponIdx && dwCurTime > m_dwNextCallShadowWeaponTime )
	{
		m_dwNextCallShadowWeaponTime += m_dwCallShadowWeaponDuration;

		D3DXVECTOR3 vAdd( 0.f, m_fAddShadowWeaponHeight, 0.f );
		g_WeaponMgr.SetReserveCallWeaponInfo( GetOwnerName(),
			m_CallShadowWeapon,
			GetPosition() +vAdd,
			m_vMoveDir,
			dwCurTime + m_CallShadowWeapon.m_dwCallTime,
			m_dwWeaponIdx );
	}
}

void ioMissileWeapon5::DestroyResource( ioPlayStage *pStage )
{
	if( m_pFireBall )
	{
		g_EffectFactory.DestroyEffect( m_pFireBall );
		m_pFireBall = NULL;
	}
}

D3DXVECTOR3 ioMissileWeapon5::GetAttackDir( const ioPlayEntity *pEntity ) const
{
	return m_vMoveDir;
}

const ioOrientBox& ioMissileWeapon5::GetLocalAttackBox() const
{
	if( m_pFireBall )
		return m_pFireBall->GetCollisionBox();

	return ioWeapon::GetLocalAttackBox();
}

ioWeapon::WeaponType ioMissileWeapon5::GetType() const
{
	return WT_MISSILE5;
}

ActionStopType ioMissileWeapon5::GetActionStopType() const
{
	return m_ActionStopType;
}

ActionStopType ioMissileWeapon5::GetDieActionStopType() const
{
	return m_DieActionStopType;
}

float ioMissileWeapon5::GetMoveSpeed()
{
	DWORD dwCurTime = FRAMEGETTIME();
	if( m_dwMoveDelayTime > 0 && m_dwCreatedTime+m_dwMoveDelayTime >= dwCurTime )
		return 0.0f;

	if( m_fCurMoveSpeed == 0.0f )
		m_fCurMoveSpeed = GetAttribute()->m_fMoveSpeed;

	return m_fCurMoveSpeed * m_fOwnerChargeSpeedRate;
}

float ioMissileWeapon5::GetMaxRange()
{
	return GetAttribute()->m_fMoveRange * m_fOwnerChargeRangeRate;
}

//void ioMissileWeapon5::UpdateWorldAttackBox( float fTimePerSec, bool bSetNull )
//{
//	if( !IsLive() || bSetNull )
//	{
//		m_WorldAttackBox.SetNull();
//		m_WorldAttackBoxForMap.SetNull();
//		return;
//	}
//
//	m_WorldAttackBox = GetLocalAttackBox();
//
//	float fScaleRate = 1.0f + (GetAttribute()->m_fScaleRate * m_fCurMoveRate );
//	float fExtentsX = m_WorldAttackBox.GetExtents(0) * fScaleRate;
//	float fExtentsY = m_WorldAttackBox.GetExtents(1) * fScaleRate;
//
//	m_WorldAttackBox.SetExtents( 0, fExtentsX );
//	m_WorldAttackBox.SetExtents( 1, fExtentsY );
//
//	float fMapCollisionRate = GetAttribute()->m_fMapCollisionBoxRate;
//	m_WorldAttackBoxForMap = m_WorldAttackBox;
//	m_WorldAttackBoxForMap.SetExtents( 0, m_WorldAttackBox.GetExtents(0)*fMapCollisionRate );
//	m_WorldAttackBoxForMap.SetExtents( 1, m_WorldAttackBox.GetExtents(1)*fMapCollisionRate );
//	m_WorldAttackBoxForMap.SetExtents( 2, m_WorldAttackBox.GetExtents(2)*fMapCollisionRate );
//
//	if( !m_WorldAttackBox.IsNull() )
//	{
//		if( m_bDisableOwnerScale )
//		{
//			m_WorldAttackBox.Transform( GetPosition(), ioMath::UNIT_ALL, GetParentSceneNode()->GetDerivedOrientation() );
//
//			m_WorldAttackBoxForMap.Transform( GetPosition(), ioMath::UNIT_ALL, GetParentSceneNode()->GetDerivedOrientation() );
//		}
//		else
//		{
//			m_WorldAttackBox.Transform( GetPosition(),
//										GetScale(),
//										GetParentSceneNode()->GetDerivedOrientation() );
//
//			m_WorldAttackBoxForMap.Transform( GetPosition(),
//											  GetScale(),
//											  GetParentSceneNode()->GetDerivedOrientation() );
//		}
//	}
//}

void ioMissileWeapon5::SetSecondAttribute( SecondAttributeList vSecondList )
{
	int iSize = vSecondList.size();
	for( int i=0; i < iSize; ++i )
	{
		SecondAttribute kSecondAttr = vSecondList[i];
		m_vSecondAttrList.push_back( kSecondAttr );
	}
}

void ioMissileWeapon5::CheckCurAttribute( const ioPlayEntity *pTarget )
{
	ioWeapon::CheckCurAttribute( pTarget );

	if( m_bResistance ) return;

	int iSize = m_vSecondAttrList.size();
	if( iSize > 0 )
	{
		for( int i=0; i < iSize-1; ++i )
		{
			if( m_fCurMoveRange < m_vSecondAttrList[i].m_fLimiteValue )
			{
				m_pSecondAttr = g_WeaponMgr.FindAttribute( m_vSecondAttrList[i].m_iAttributeIndex );
				return;
			}
		}

		m_pSecondAttr = g_WeaponMgr.FindAttribute( m_vSecondAttrList[iSize-1].m_iAttributeIndex );
		return;
	}

	m_pSecondAttr = NULL;
	return;
}

const WeaponAttribute* ioMissileWeapon5::GetAttribute() const
{
	if( m_bResistance )
	{
		if( m_pResistanceAttr )
			return m_pResistanceAttr;
	}

	if( m_pSecondAttr )
		return m_pSecondAttr;

	return m_pConstAttr;
}

/*bool ioMissileWeapon5::IsCollision( ioPlayEntity *pEntity, bool bMapCollision )
{
	bool bCollision = false;

	D3DXVECTOR3 vColPoint( 0.0f, 0.0f, 0.0f );
	if( pEntity->IsWoundedCollision( GetWorldAttackBox(bMapCollision), &vColPoint ) )
		bCollision = CheckCollisionLine( pEntity, vColPoint );

	CheckPiercing( bCollision );

	if( bCollision )
		CallAttribute( pEntity );

	return bCollision;	
}*/

/*bool ioMissileWeapon5::CheckCollisionLine( ioPlayEntity *pTarget, const D3DXVECTOR3 &vColPoint )
{
	if( GetPiercingType() == CPT_ALL || GetPiercingType() == CPT_ALL_LIMITE )
		return true;

	// 타겟이 캐릭터가 아니라면 충돌로 처리한다
	ioBaseChar *pTargetChar = ToBaseChar( pTarget );
	if( !pTargetChar )	return true;

	ioBaseChar *pOwner = GetOwner();
	if( pOwner )
		return pOwner->CheckCollisionLine( m_StartPos, vColPoint );

	return false;	// 주인이 없는 무기라면 충돌안한걸로 하자
}*/

void ioMissileWeapon5::SetWeaponDead()
{
	CreateCollisionAreaWeapon();

	ioWeapon::SetWeaponDead();
}

void ioMissileWeapon5::AddMovePosition( const D3DXVECTOR3& vPos )
{
	//m_vWeaponPosList.push_back( vPos );
	stMovePositionInfo Info;
	Info.vPos = vPos;
	D3DXVECTOR3 vPosCalc;
	int nSize = (int)m_vMoveInfoList.size();
	//처음은 케릭터 위에서 이동
	if( nSize <= 0 )
	{
		vPosCalc = vPos - m_StartPos;
	}
	else//이후부터는 지상에서 y값 무시하고 이동
	{
		D3DXVECTOR3 vTempPos = vPos;
		vTempPos.y = 0.f;

		vPosCalc = m_vMoveInfoList[ nSize - 1 ].vPos;
		vPosCalc.y = 0.f;

		vPosCalc = vPosCalc - vTempPos;
	}

	if( m_vMoveInfoList.size() == 0 )
	{
		D3DXVECTOR3 vMoveDir = vPos - m_StartPos;
		D3DXVec3Normalize( &vMoveDir, &vMoveDir );
		SetMoveDir( vMoveDir );
	}

	Info.fRange = D3DXVec3Length( &vPosCalc );
	m_vMoveInfoList.push_back( Info );

	m_MoveState = START_MOVE;
}

bool ioMissileWeapon5::isCallAttribute()
{
	if(!m_CallAttFireList.empty())
		return true;

	return false;
}

void ioMissileWeapon5::CallAttribute( ioPlayEntity *pEntity )
{
	if( m_bArleadyCallWeapon )
		return;	
	
	if( m_CallAttFireList.empty() )
		return;

	ioBaseChar *pOwner = GetOwner();
	if( !pOwner )
		return;	

	ioBaseChar* pTarget = ToBaseChar(pEntity);
	if( !pTarget )	
		return;

	DWORD dwCurTime = FRAMEGETTIME();
	DWORD dwCurGenerateTime = dwCurTime;
		
	ChangeDirInfoList::iterator iter;
	
	exReserveWeaponCollisionInfo pInfo;
	pInfo.m_szTarget = pTarget->GetCharName();

	for( iter = m_CallAttFireList.begin(); iter != m_CallAttFireList.end(); ++iter )
	{
		ChangeDirInfo Info = (*iter);

		D3DXVECTOR3 vDir = m_vMoveDir;
		D3DXVec3Normalize(&vDir, &vDir);
		float fAngleH = ioMath::ArrangeHead( Info.m_fChangeAngleH );
		float fAngleV = ioMath::ArrangeHead( Info.m_fChangeAngleV );

		D3DXQUATERNION qtRot;
		D3DXVECTOR3 vAxis;
		D3DXVec3Cross( &vAxis, &vDir, &D3DXVECTOR3( 0.0f, -1.0f, 0.0f ) );

		// vertical
		D3DXQuaternionRotationAxis( &qtRot, &vAxis, DEGtoRAD(fAngleV) );
		vDir = qtRot * vDir;

		// horizon
		D3DXQuaternionRotationAxis( &qtRot, &ioMath::UNIT_Y, DEGtoRAD(fAngleH) );
		vDir = qtRot * vDir;

		D3DXVECTOR3	vPos = GetPosition();

		g_WeaponMgr.SetReserveCallWeaponCollisionInfo( GetOwnerName(), 
			m_CallWeaponInfo,
			vPos,
			-vDir,
			dwCurGenerateTime,
			m_dwWeaponIdx,
			pInfo
		);
		
		dwCurGenerateTime += 10;		
	}

	m_bArleadyCallWeapon = true;	
}

bool ioMissileWeapon5::IsIgnoreTestMapCollision() const
{
	return true;
}