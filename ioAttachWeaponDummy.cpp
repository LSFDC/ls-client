
#include "stdafx.h"

#include "ioAttachWeaponDummy.h"

#include "ioPlayStage.h"
#include "ioBaseChar.h"

ioAttachWeaponDummy::ioAttachWeaponDummy( ioEntityGroup *pGrp, ioPlayMode *pMode )
: ioDummyChar( pGrp, pMode )
{
	m_CreateWeaponList.clear();
	m_AttachState = AS_START;
	m_szAttachEffect.Clear();
	m_bNotUseTranslateByConveyer = false;
}

ioAttachWeaponDummy::~ioAttachWeaponDummy()
{
	m_CreateWeaponList.clear();
}

void ioAttachWeaponDummy::LoadProperty( ioINILoader &rkLoader )
{
	ioDummyChar::LoadProperty( rkLoader );
	
	LoadInitProperty( rkLoader );
}

void ioAttachWeaponDummy::LoadInitProperty( ioINILoader &rkLoader )
{
	char szBuf[MAX_PATH];

	rkLoader.LoadString_e( "area_weapon_name", "", szBuf, MAX_PATH );
	m_szAreaWeaponName = szBuf;

	//���� �ε���
	m_nCreateWeaponIndex = rkLoader.LoadInt_e( "attack_type", NO_WEAPON_ATTRIBUTE );

	//������
	m_vCreateWeaponOffset.x = rkLoader.LoadFloat_e( "attack_offset_x", 0.0f );
	m_vCreateWeaponOffset.y = rkLoader.LoadFloat_e( "attack_offset_y", 0.0f );
	m_vCreateWeaponOffset.z = rkLoader.LoadFloat_e( "attack_offset_z", 0.0f );

	m_fVisibleRange = rkLoader.LoadFloat_e( "visible_range", 0.0f );

	m_bAlwaysVisible = rkLoader.LoadBool_e( "always_visible", false );

	//�߰� yong jun
	rkLoader.LoadString_e( "attach_effect", "", szBuf, MAX_PATH );
	m_szAttachEffect = szBuf;
	m_bNotUseTranslateByConveyer = rkLoader.LoadBool_e( "not_use_translate_by_conveyer", false );
	//
}

void ioAttachWeaponDummy::ProcessState( float fTimePerSec, DWORD dwPreTime )
{
	VisibleCheck();

	switch( m_AttachState )
	{
	case AS_START:
		SetFireState();
		break;
	case AS_FIRE:
		SetWaitState();
		break;
	case AS_WAIT:
		{
			if( m_dwStartTime + m_dwLifeTime < FRAMEGETTIME() )
				SendDieState( true );

			ProcessPos( fTimePerSec );
			CheckMove();
		}
		break;
	}
}

//���� �״� ���� ����
void ioAttachWeaponDummy::SetDieState( bool bEffect )
{
	ioDummyChar::SetDieState( bEffect );
	
	//������ ���� ����
	RemoveWeapon();

	//�߰� yong jun
	//������ effect ����
	RemoveEffect();
	//
}

/////�߰��� �Լ�////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ioAttachWeaponDummy::SetFireState()
{
	//��ġ ������ ����
	D3DXVECTOR3 vPos = GetWorldPosition();
	vPos.y += m_fHeightOffset;
	SetWorldPosition( vPos );

	m_AttachState = AS_FIRE;
	m_State = DCS_DELAY;
}

void ioAttachWeaponDummy::SetWaitState()
{	
	CreateWeapon();
	CreateAreaWeapon();
	CreateEffect();

	m_AttachState = AS_WAIT;
}

//���� ����
void ioAttachWeaponDummy::CreateWeapon()
{
	if ( m_nCreateWeaponIndex == NO_WEAPON_ATTRIBUTE )
		return;

	ioBaseChar *pOwner = GetOwner();

	if ( !pOwner )
		return;

	D3DXVECTOR3 vPos = GetWorldPosition() + m_vCreateWeaponOffset;
	D3DXVECTOR3 vDir = GetWorldOrientation() * D3DXVECTOR3( 0.0f, 0.0f, -FLOAT1 );

	m_dwBaseWeaponIndex = pOwner->GetWeaponIndexBase() + 1;

	FireTime kFireTime;
	kFireTime.dwStart = FRAMEGETTIME();

	kFireTime.iAttributeIdx = m_nCreateWeaponIndex;
	kFireTime.iResistanceIdx = 0;
	kFireTime.szWoundedAni  = "";
	kFireTime.dwWoundedAniDuration = 0;
	kFireTime.bLoopWoundedAni = false;
	kFireTime.dwWeaponIdx = m_dwBaseWeaponIndex;
	kFireTime.eFireTimeType = FTT_DUMMY_CHAR_ATTACK;

	pOwner->SetWeaponIndexBase( m_dwBaseWeaponIndex );

	ioWeapon *pWeapon = pOwner->ExtendFireExplicit( kFireTime, vPos, vDir, "" );
	if ( pWeapon )
	{
		pWeapon->SetPosition( vPos );

		CreateInfo sCreateInfo;
		sCreateInfo.wType = Normal_Weapon;
		sCreateInfo.dwIndex = pWeapon->GetWeaponIndex();
		m_CreateWeaponList.push_back( sCreateInfo );
	}	
}

void ioAttachWeaponDummy::CreateAreaWeapon()
{
	if ( m_szAreaWeaponName.IsEmpty() )
		return;

	ioBaseChar *pOwner = GetOwner();
	if ( !pOwner )
		return;

	//����, ��ġ
	D3DXVECTOR3 vPos = GetWorldPosition() + m_vCreateWeaponOffset;
	D3DXQUATERNION qtRot = GetWorldOrientation();

	//Area ���� ����
	ioAreaWeapon *pArea = g_AreaWeaponMgr.CreateAreaWeapon( pOwner->GetCharName(),
		"",
		"",
		m_szAreaWeaponName,
		vPos,
		qtRot,
		ioAreaWeapon::CT_NORMAL );

	if ( pArea )
	{
		CreateInfo sCreateInfo;
		sCreateInfo.wType = Area_Weapon;
		sCreateInfo.dwIndex = pArea->GetAreaWeaponIndex();
		m_CreateWeaponList.push_back( sCreateInfo );
	}
}

//���� ����
void ioAttachWeaponDummy::RemoveWeapon()
{
	ioBaseChar *pOwner = GetOwner();
	if ( !pOwner )
		return;

	ioPlayStage *pStage = pOwner->GetCreator();
	if ( !pStage )
		return;

	//���� ����Ʈ ���
	int nWeaponCnt = (int)m_CreateWeaponList.size();

	for (int i=0; i<nWeaponCnt; ++i)
	{
		CreateInfo sCreateInfo = m_CreateWeaponList[i];
		if ( sCreateInfo.wType == Normal_Weapon )
		{
			ioWeapon *pWeapon = g_WeaponMgr.FindWeapon( pOwner, sCreateInfo.dwIndex );
			if ( pWeapon && pWeapon->IsLive() )
			{
				pWeapon->SetWeaponDead();
			}
		}
		else
		{
			ioAreaWeapon *pAreaWeapon = g_AreaWeaponMgr.FindAreaWeaponByIndex( sCreateInfo.dwIndex );
			if ( pAreaWeapon && pAreaWeapon->IsLive() )
			{
				pAreaWeapon->DestroyAreaWeapon( pStage );
			}
		}
	}

	//����Ʈ ���
	m_CreateWeaponList.clear();
}

void ioAttachWeaponDummy::CheckMove()
{
	ioBaseChar *pOwner = GetOwner();
	if ( !pOwner )
		return;

	ioPlayStage *pStage = pOwner->GetCreator();
	if ( !pStage )
		return;

	D3DXVECTOR3 vPos = GetWorldPosition() + m_vCreateWeaponOffset;

	//���� ����Ʈ ���
	int nWeaponCnt = (int)m_CreateWeaponList.size();

	for (int i=0; i<nWeaponCnt; ++i)
	{
		CreateInfo sCreateInfo = m_CreateWeaponList[i];
		if ( sCreateInfo.wType == Normal_Weapon )
		{
			ioWeapon *pWeapon = g_WeaponMgr.FindWeapon( pOwner, sCreateInfo.dwIndex );
			if ( pWeapon && pWeapon->IsLive() )
			{
				pWeapon->SetPosition( vPos );
			}
		}
		else
		{
			ioAreaWeapon *pAreaWeapon = g_AreaWeaponMgr.FindAreaWeaponByIndex( sCreateInfo.dwIndex );
			if ( pAreaWeapon && pAreaWeapon->IsLive() )
			{
				pAreaWeapon->SetAreaPosition( vPos, pStage );
			}
		}
	}
}

// �̺κ� ���̴°� üũ
void ioAttachWeaponDummy::VisibleCheck()
{
	ioBaseChar *pOwner = GetOwner();
	if( !m_pGroup || !pOwner || m_bAlwaysVisible )	
		return;

	ioPlayStage *pStage = pOwner->GetCreator();
	if ( !pStage )
		return;

	ioBaseChar *pTarget = pStage->GetCameraTargetChar();
	if( pTarget )
	{
		if( GetTeam() == pTarget->GetTeam() )
		{
			m_pGroup->SetVisible( true );
		}
		else
		{
			float fVisibleRangeSq = m_fVisibleRange * m_fVisibleRange;

			D3DXVECTOR3 vDiff = GetWorldPosition() - pTarget->GetWorldPosition();

			if( D3DXVec3LengthSq( &vDiff ) < fVisibleRangeSq )
				m_pGroup->SetVisible( true );	
			else
				m_pGroup->SetVisible( false );			
		}
	}
	else
	{
		m_pGroup->SetVisible( false );
	}
}

//�߰� yong jun
/////////////////////////////////////////////////////////////////////////////////////
void ioAttachWeaponDummy::CreateEffect()
{
	if ( !m_szAttachEffect.IsEmpty() )
		m_pGroup->AttachEffect( m_szAttachEffect, NULL );
}

void ioAttachWeaponDummy::RemoveEffect()
{
	if ( !m_szAttachEffect.IsEmpty() )
		m_pGroup->EndEffect(m_szAttachEffect, false );
}

void ioAttachWeaponDummy::TranslateByConveyer( const D3DXVECTOR3 &vMove )
{
	if ( m_bNotUseTranslateByConveyer )
		return;
	else
		ioDummyChar::TranslateByConveyer( vMove );
}

/////////////////////////////////////////////////////////////////////////////////////