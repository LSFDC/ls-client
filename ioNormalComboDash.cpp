

#include "stdafx.h"

#include "ioNormalComboDash.h"

#include "ioBaseChar.h"
#include "ioPlayStage.h"


#include "ItemDefine.h"
#include "WeaponDefine.h"

ioNormalComboDash::ioNormalComboDash()
{
	m_dwActionEndTime = 0;
	m_dwPreReserveEnableTime = 0;

	m_dwReserveEnableTime = 0;

	m_iSkillInput = -1;
	m_bEnableDefenseState = false;
	m_bEnableAttackState = false;
	m_bEnableJumpState = false;

	m_iCurComboCnt = 0;
	m_dwChargingStartTime = 0;
	m_DashState = DS_NONE;

	m_dwCancelCheckTime = 0;
	m_dwInputCancelCheckTime = 0;
}

ioNormalComboDash::ioNormalComboDash( const ioNormalComboDash &rhs )
: ioExtendDash( rhs ),
m_iMaxComboCnt( rhs.m_iMaxComboCnt ),
m_dwChargingTime( rhs.m_dwChargingTime ),
m_ChargingAniList( rhs.m_ChargingAniList ),
m_NormalAttackList( rhs.m_NormalAttackList ),
m_ExtendAttackList( rhs.m_ExtendAttackList )
{
	m_dwActionEndTime = 0;
	m_dwPreReserveEnableTime = 0;

	m_dwReserveEnableTime = 0;

	m_iSkillInput = -1;
	m_bEnableDefenseState = false;
	m_bEnableAttackState = false;
	m_bEnableJumpState = false;
	
	m_iCurComboCnt = 0;
	m_dwChargingStartTime = 0;
	m_DashState = DS_NONE;

	m_dwCancelCheckTime = 0;
	m_dwInputCancelCheckTime = 0;
}

ioExtendDash* ioNormalComboDash::Clone()
{
	return new ioNormalComboDash( *this );
}

ioNormalComboDash::~ioNormalComboDash()
{
}

DashType ioNormalComboDash::GetType() const
{
	return DT_NORMAL_COMBO;
}

void ioNormalComboDash::LoadProperty( ioINILoader &rkLoader )
{
	ioExtendDash::LoadProperty( rkLoader );

	char szKey[MAX_PATH];
	char szBuf[MAX_PATH];

	m_dwChargingTime = rkLoader.LoadInt_e( "dash_charge_time", 0 );

	m_iMaxComboCnt = rkLoader.LoadInt_e( "dash_max_combo_cnt", 0 );

	for( int i=0; i < m_iMaxComboCnt; ++i )
	{
		wsprintf_e( szKey, "charging_dash_ani%d", i+1 );
		rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );
		m_ChargingAniList.push_back( szBuf );
	}

	// normal combo
	for( int i=0; i < m_iMaxComboCnt; ++i )
	{
		AttackAttribute eAttribute;

		wsprintf_e( szKey, "normal_dash_attack%d", i+1);
		LoadAttackAttribute( szKey, eAttribute, rkLoader );

		m_NormalAttackList.push_back( eAttribute );
	}

	// Extend combo
	for( int i=0; i < m_iMaxComboCnt; ++i )
	{
		AttackAttribute eAttribute;

		wsprintf_e( szKey, "extend_dash_attack%d", i+1);
		LoadAttackAttribute( szKey, eAttribute, rkLoader );

		m_ExtendAttackList.push_back( eAttribute );
	}
}	

bool ioNormalComboDash::StartDash( ioBaseChar *pOwner )
{
	if( !pOwner )
		return false;

	ioExtendDash::StartDash( pOwner );

	m_iCurComboCnt = 0;
	m_dwChargingStartTime = 0;

	m_dwActionEndTime = 0;
	m_dwPreReserveEnableTime = 0;

	m_DashState = DS_NONE;

	m_dwInputCancelCheckTime = 0;
	m_dwCancelCheckTime = 0;

	pOwner->InitExtendAttackTagInfo();
	pOwner->SetNoWoundState( false );
	pOwner->SetState( CS_EXTEND_DASH );

	ChangeToCharging( pOwner );

	if( pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_EXTEND_DASH );
		kPacket << pOwner->GetCharName();
		kPacket << (int)GetType();
		kPacket << pOwner->GetHP().m_fCurValue;
		kPacket << DS_NONE;
		kPacket << m_iCurComboCnt;
		kPacket << pOwner->GetTargetRot();
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}

	return true;
}

void ioNormalComboDash::SetExtraInfoToWeapon( ioBaseChar *pOwner, ioPlayStage *pStage, ioWeapon *pWeapon, const ioHashString &szName )
{
	if( !pWeapon ) return;

	ioZoneEffectWeapon *pZone = ToZoneEffectWeapon( pWeapon );
	ioMissileWeapon *pMissleWeapon = ToMissileWeapon( pWeapon );
	if( pZone )
	{
		ioBaseChar *pChar = pZone->GetOwner();
		if( pChar )
		{
			D3DXVECTOR3 vDir = pChar->GetTargetRot() * D3DXVECTOR3( 0.0f, 0.0f, -FLOAT1 );
			pZone->SetCollisionDir( vDir );
			pZone->SetZoneValue( false, false );
		}
	}

	if( pMissleWeapon )
	{
		ioBaseChar *pChar = pMissleWeapon->GetOwner();
		if( pChar )
		{
			ioItem *pItem = pOwner->GetEquipedItem( ES_WEAPON );
			ioChargeComboBowItem *pChargeComboBowItem = ToChargeComboBowItem( pItem );
			if( pChargeComboBowItem )
				pChargeComboBowItem->WasteBullet( pChar );
		}
	}
}

void ioNormalComboDash::CheckKeyReserve( ioBaseChar *pOwner )
{
	if( !pOwner || !pOwner->IsNeedProcess() )
		return;

	bool bReserveSkill = false;
	int iSkillNum = pOwner->GetSkillKeyInput();
	if( iSkillNum >= 0 )
	{
		m_iSkillInput = iSkillNum;
		bReserveSkill = true;
	}

	if( pOwner->IsDefenseKeyDown() && !m_bEnableDefenseState && !bReserveSkill )
	{
		m_bEnableDefenseState = true;
		m_bEnableAttackState = false;
		m_bEnableJumpState = false;

		m_iSkillInput = -1;
	}
	else if( pOwner->IsJumpKey() && !m_bEnableJumpState && !bReserveSkill )
	{
		m_bEnableJumpState = true;
		m_bEnableAttackState = false;
		m_bEnableDefenseState = false;
		m_iSkillInput = -1;
	}
	else if( pOwner->IsAttackKey() && !m_bEnableAttackState && !bReserveSkill )
	{
		ioUserKeyInput::DirKeyInput eCurKey = pOwner->CheckCurDirKey();
		
		if( pOwner->IsSettedDirection() )
		{
			float fYawD = 180.0f + 45.0f * ( eCurKey - ioUserKeyInput::DKI_UP );
			fYawD = ioMath::ArrangeHead( fYawD );

			float fYawR = DEGtoRAD( fYawD );
			D3DXQUATERNION qtRotate;
			D3DXQuaternionRotationYawPitchRoll( &qtRotate, fYawR, 0.0f, 0.0f );

			pOwner->SetTargetRotToRotate( qtRotate, true, false );
		}

		ChangeToCharging( pOwner );

		if( pOwner->IsNeedSendNetwork() )
		{
			SP2Packet kPacket( CUPK_EXTEND_DASH );
			kPacket << pOwner->GetCharName();
			kPacket << (int)GetType();
			kPacket << pOwner->GetHP().m_fCurValue;
			kPacket << DS_CHARGE;
			kPacket << m_iCurComboCnt;
			kPacket << pOwner->GetTargetRot();
			P2PNetwork::SendToAllPlayingUser( kPacket );
		}
		return;
	}
}

void ioNormalComboDash::CheckExtraReserve( ioBaseChar *pOwner )
{
	if( !pOwner || !pOwner->IsNeedProcess() )
		return;

	bool bReserveSkill = false;
	int iSkillNum = pOwner->GetSkillKeyInput();
	if( iSkillNum >= 0 )
	{
		m_iSkillInput = iSkillNum;
		bReserveSkill = true;
	}

	if( pOwner->IsDefenseKeyDown() && !m_bEnableDefenseState && !bReserveSkill )
	{
		m_bEnableDefenseState = true;
		m_bEnableAttackState = false;
		m_bEnableJumpState = false;

		m_iSkillInput = -1;
	}
	else if( pOwner->IsJumpKey() && !m_bEnableJumpState && !bReserveSkill )
	{
		m_bEnableJumpState = true;
		m_bEnableAttackState = false;
		m_bEnableDefenseState = false;
		m_iSkillInput = -1;
	}
	else if( pOwner->IsAttackKey() && !m_bEnableAttackState && !bReserveSkill )
	{
		m_bEnableAttackState = true;
		m_bEnableDefenseState = false;
		m_bEnableJumpState = false;

		m_iSkillInput = -1;
	}
}

void ioNormalComboDash::ProcessDash( ioBaseChar *pOwner )
{
	DWORD dwCurTime = FRAMEGETTIME();

	switch( m_DashState )
	{
	case DS_CHARGE:
		ProcessCharging( pOwner );
		break;
	case DS_ATTACK_FIRE:
		ProcessAttackFireDash( pOwner );
		break;
	}
}

void ioNormalComboDash::ApplyDash( ioBaseChar *pOwner, SP2Packet &rkPacket )
{
	if( !pOwner ) return;

	int iState;
	rkPacket >> iState;
	rkPacket >> m_iCurComboCnt;

	D3DXQUATERNION qtRot;
	rkPacket >> qtRot;

	pOwner->SetTargetRotToRotate( qtRot, true, false );

	switch( iState )
	{
	case DS_NONE:
		StartDash( pOwner );
		break;
	case DS_CHARGE:
		ChangeToCharging( pOwner );
		break;
	case DS_ATTACK_FIRE:
		{
			bool bExtend;
			rkPacket >> bExtend;

			ChangeToAttackFire( pOwner, bExtend );
		}
		break;
	}
}

void ioNormalComboDash::ExtendDashEnd( ioBaseChar *pOwner )
{
	m_DashState = DS_NONE;

	m_iCurComboCnt = 0;
	m_dwChargingStartTime = 0;

	m_dwActionEndTime = 0;
	m_dwPreReserveEnableTime = 0;

	m_dwReserveEnableTime = 0;

	m_iSkillInput = -1;
	m_bEnableDefenseState = false;
	m_bEnableAttackState = false;
	m_bEnableJumpState = false;

	if( pOwner )
	{
		pOwner->InitExtendAttackTagInfo();
	}
}

void ioNormalComboDash::ChangeToCharging( ioBaseChar *pOwner )
{
	if( !pOwner )
		return;

	ioEntityGroup *pGrp = pOwner->GetGroup();
	if( !pGrp )
		return;

	if( m_ChargingAniList.empty() )
		return;

	ioHashString szCurChargingAni;
	if( COMPARE( m_iCurComboCnt, 0, m_iMaxComboCnt ) )
		szCurChargingAni = m_ChargingAniList[m_iCurComboCnt];
	else
		szCurChargingAni = m_ChargingAniList[0];

	int iAniID = pGrp->GetAnimationIdx( szCurChargingAni );

	pGrp->ClearAllLoopAni( FLOAT100, true );
	pGrp->ClearAllActionAni( FLOAT100, true );

	pGrp->SetLoopAni( szCurChargingAni, FLOAT100 );

	m_dwChargingStartTime = FRAMEGETTIME();

	m_dwActionEndTime = 0;
	m_dwPreReserveEnableTime = 0;

	m_dwReserveEnableTime = 0;

	m_iSkillInput = -1;
	m_bEnableDefenseState = false;
	m_bEnableAttackState = false;
	m_bEnableJumpState = false;

	m_DashState = DS_CHARGE;
}

void ioNormalComboDash::ChangeToAttackFire( ioBaseChar *pOwner, bool bExtend )
{
	if( !pOwner || m_iMaxComboCnt <= 0 )
		return;

	int iCurIndex = m_iCurComboCnt;
	if( !COMPARE( iCurIndex, 0, m_iMaxComboCnt ) )
		iCurIndex = 0;

	AttackAttribute kAttackInfo = m_NormalAttackList[iCurIndex];
	if( bExtend )
		kAttackInfo = m_ExtendAttackList[iCurIndex];

	ioEntityGroup *pGrp = pOwner->GetGroup();
	int iAniID = pGrp->GetAnimationIdx( kAttackInfo.m_AttackAnimation );
	if( iAniID == -1 )
	{
		LOG.PrintTimeAndLog( 0, "ioNormalComboDash::ChangeToAttackFire - %s Animation is Not Exist",
								kAttackInfo.m_AttackAnimation.c_str() );
		return;
	}

	float fTimeRate = kAttackInfo.m_fAttackAniRate;
	if( fTimeRate <= 0.0f )
		fTimeRate = FLOAT1;

	m_dwActionEndTime = 0;
	m_dwPreReserveEnableTime = 0;

	m_dwReserveEnableTime = 0;

	m_iSkillInput = -1;
	m_bEnableDefenseState = false;
	m_bEnableAttackState = false;
	m_bEnableJumpState = false;

	pGrp->ClearAllLoopAni( FLOAT100, true );
	pGrp->ClearAllActionAni( FLOAT100, true );

	pOwner->RefreshFireTimeList( iAniID,
								 kAttackInfo.m_vWeaponInfoList,
								 FTT_EXTEND_DASH_ATTACK,
								 fTimeRate,
								 kAttackInfo.m_dwPreDelay );

	pOwner->IncreaseWeaponIndexBase();
	pOwner->SetNoWoundState( false );

	pOwner->SetState( CS_EXTEND_DASH );
	pOwner->SetAutoTarget( ATT_DASH );
	pGrp->SetActionAni( iAniID,	FLOAT100, FLOAT100, FLOAT1, FLOAT1/fTimeRate, 0.0f, (float)kAttackInfo.m_dwPreDelay, true );

	DWORD dwCurTime = FRAMEGETTIME();
	DWORD dwDuration = kAttackInfo.m_dwPreDelay + pGrp->GetAnimationFullTime( iAniID ) * fTimeRate + kAttackInfo.m_dwEndDelay;

	m_dwActionEndTime = dwCurTime + dwDuration;

	CheckKeyReserveTime( pOwner, iAniID, fTimeRate );

	pOwner->SetReservedSliding( kAttackInfo.m_vForceInfoList, iAniID, fTimeRate, kAttackInfo.m_dwPreDelay );
	pOwner->CheckCharColSkipTime( iAniID, fTimeRate, kAttackInfo.m_dwPreDelay );

	DWORD dwTrackingTime = FRAMEGETTIME() + kAttackInfo.m_dwPreDelay;
	dwTrackingTime += pGrp->GetAnimationEventTime_e( iAniID, "tracking_e" ) * fTimeRate;
	pOwner->SetAutoTargetTrackingEndTime( dwTrackingTime );

	m_DashState = DS_ATTACK_FIRE;

	if( pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_EXTEND_DASH );
		kPacket << pOwner->GetCharName();
		kPacket << (int)GetType();
		kPacket << pOwner->GetHP().m_fCurValue;
		kPacket << DS_ATTACK_FIRE;
		kPacket << m_iCurComboCnt;
		kPacket << pOwner->GetTargetRot();
		kPacket << bExtend;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}

	// after send
	m_iCurComboCnt++;
}

void ioNormalComboDash::ProcessCharging( ioBaseChar *pOwner )
{
	if( !pOwner ) return;
	if( !pOwner->IsNeedProcess() )
		return;

	ioEntityGroup *pGrp = pOwner->GetGroup();
	if( !pGrp ) return;

	DWORD dwCurTime = FRAMEGETTIME();
	if( pOwner->IsAttackKeyDown() )
	{
		if( m_dwChargingStartTime+m_dwChargingTime < dwCurTime )
		{
			ChangeToAttackFire( pOwner, true );
			return;
		}
	}
	else
	{
		ChangeToAttackFire( pOwner, false );
		return;
	}
}

void ioNormalComboDash::ProcessAttackFireDash( ioBaseChar *pOwner )
{
	DWORD dwCurTime = FRAMEGETTIME();
	DWORD dwEndTime = m_dwActionEndTime + pOwner->GetActionStopTotalDelay();

	if( dwEndTime > dwCurTime )
	{
		pOwner->CheckExtraAniJump();

		if( m_dwReserveEnableTime > 0 && m_dwReserveEnableTime + pOwner->GetActionStopTotalDelay() < dwCurTime )
		{
			// 예약체크
			if( m_iCurComboCnt >= m_iMaxComboCnt )
			{
				CheckExtraReserve( pOwner );
			}
			else
			{
				CheckKeyReserve( pOwner );
			}

			return;
		}
	}
	else
	{
		ProcessKeyReserve( pOwner );
		return;
	}
}

bool ioNormalComboDash::IsEnableDefense()
{
	return false;
}

bool ioNormalComboDash::IsPowerChargingState()
{
	return false;
}


