

#include "stdafx.h"

#include "ioCountChargeItem.h"

#include "ioBaseChar.h"

#include "WeaponDefine.h"
#include "ItemDefine.h"

ioCountChargeItem::ioCountChargeItem()
{
	m_ChargeState = CS_NONE;
	m_dwAttackStartTime = 0;
	m_iCurCombo = 0;

	m_GatheringMaxEffectID = -1;

	m_bSetChargingMoveAni = false;
	m_fCurChargeRate = 0.0f;
	m_fCurChargeRateForSkill = 0.0f;

	m_iCurMaxCount = 0;
	m_iCurCount = 0;

	m_dwFireMotionEndTime = 0;
	m_dwFireEndMotionEndTime = 0;

	m_dwRetreatEndTime = 0;
	m_dwRetreatForceTime = 0;
}

ioCountChargeItem::ioCountChargeItem( const ioCountChargeItem &rhs )
: ioWeaponItem( rhs ),
  m_AttackReadyAniList( rhs.m_AttackReadyAniList ),
  m_vExtendAttributeList( rhs.m_vExtendAttributeList ),
  m_vExtendEndAttributeList( rhs.m_vExtendEndAttributeList ),
  m_GatherAnimation( rhs.m_GatherAnimation ),
  m_GatheringMaxEffect( rhs.m_GatheringMaxEffect ),
  m_ChargingMoveAnimation( rhs.m_ChargingMoveAnimation ),
  m_fGatherAniRate( rhs.m_fGatherAniRate ),
  m_GatheringMaxSound( rhs.m_GatheringMaxSound ),
  m_szRetreatFr( rhs.m_szRetreatFr ),
  m_szRetreatBack( rhs.m_szRetreatBack ),
  m_szRetreatLt( rhs.m_szRetreatLt ),
  m_szRetreatRt( rhs.m_szRetreatRt ),
  m_fRetreatFrRate( rhs.m_fRetreatFrRate ),
  m_fRetreatBackRate( rhs.m_fRetreatBackRate ),
  m_fRetreatLtRate( rhs.m_fRetreatLtRate ),
  m_fRetreatRtRate( rhs.m_fRetreatRtRate ),
  m_fRetreatForceAmt( rhs.m_fRetreatForceAmt ),
  m_fRetreatForceFric( rhs.m_fRetreatForceFric ),
  m_ChargeExtraAttack( rhs.m_ChargeExtraAttack )
{
	m_ChargeState = CS_NONE;
	m_dwAttackStartTime = 0;
	m_iCurCombo = 0;

	m_GatheringMaxEffectID = -1;

	m_bSetChargingMoveAni = false;
	m_fCurChargeRate = 0.0f;
	m_fCurChargeRateForSkill = 0.0f;

	m_iCurMaxCount = 0;
	m_iCurCount = 0;

	m_dwFireMotionEndTime = 0;
	m_dwFireEndMotionEndTime = 0;

	m_dwRetreatEndTime = 0;
	m_dwRetreatForceTime = 0;
}

ioCountChargeItem::~ioCountChargeItem()
{
}

void ioCountChargeItem::LoadProperty( ioINILoader &rkLoader )
{
	ioWeaponItem::LoadProperty( rkLoader );

	int i;
	char szKey[MAX_PATH], szBuf[MAX_PATH];

	m_vExtendAttributeList.clear();
	m_vExtendEndAttributeList.clear();

	int iExtendCnt = rkLoader.LoadInt_e( "extend_attack_cnt", 0 );
	m_vExtendAttributeList.reserve( iExtendCnt );
	m_vExtendEndAttributeList.reserve( iExtendCnt );

	for( i=0 ; i<iExtendCnt ; i++ )
	{
		AttackAttribute kAttribute;
		wsprintf_e( szKey, "extend_attack0%d", i+1 );
		LoadAttackAttribute( szKey, kAttribute, rkLoader );
		m_vExtendAttributeList.push_back( kAttribute );
	}

	for( i=0 ; i<iExtendCnt ; i++ )
	{
		AttackAttribute kAttribute;
		wsprintf_e( szKey, "extend_end_attack0%d", i+1 );
		LoadAttackAttribute( szKey, kAttribute, rkLoader );
		m_vExtendEndAttributeList.push_back( kAttribute );
	}

	m_AttackReadyAniList.clear();
	for( i=0 ; i<iExtendCnt ; i++ )
	{
		wsprintf_e( szKey, "attack_ready_animation%d", i+1 );
		rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );
		m_AttackReadyAniList.push_back( ioHashString(szBuf) );
	}

	rkLoader.LoadString_e( "gathering_animation", "", szBuf, MAX_PATH );
	m_GatherAnimation = szBuf;

	m_fGatherAniRate = rkLoader.LoadFloat_e( "gathering_ani_rate", FLOAT1 );

	rkLoader.LoadString_e( "gathering_max_effect", "", szBuf, MAX_PATH );
	m_GatheringMaxEffect = szBuf;

	rkLoader.LoadString_e( "gathering_max_sound", "", szBuf, MAX_PATH );
	m_GatheringMaxSound = szBuf;

	rkLoader.LoadString_e( "attack_move_ani", "", szBuf, MAX_PATH );
	m_ChargingMoveAnimation = szBuf;

	//
	rkLoader.LoadString_e( "retreat_animation_front", "", szBuf, MAX_PATH );
	m_szRetreatFr = szBuf;
	m_fRetreatFrRate = rkLoader.LoadFloat_e( "retreat_animation_front_rate", FLOAT1 );
	rkLoader.LoadString_e( "retreat_animation_back", "", szBuf, MAX_PATH );
	m_szRetreatBack = szBuf;
	m_fRetreatBackRate = rkLoader.LoadFloat_e( "retreat_animation_back_rate", FLOAT1 );
	rkLoader.LoadString_e( "retreat_animation_right", "", szBuf, MAX_PATH );
	m_szRetreatRt = szBuf;
	m_fRetreatRtRate = rkLoader.LoadFloat_e( "retreat_animation_right_rate", FLOAT1 );
	rkLoader.LoadString_e( "retreat_animation_left", "", szBuf, MAX_PATH );
	m_szRetreatLt = szBuf;
	m_fRetreatLtRate = rkLoader.LoadFloat_e( "retreat_animation_left_rate", FLOAT1 );

	m_fRetreatForceAmt = rkLoader.LoadFloat_e( "retreat_force_amt", 0.0f );
	m_fRetreatForceFric = rkLoader.LoadFloat_e( "retreat_force_friction", FLOAT1 );

	//
	LoadAttackAttribute_e( "charge_extra_attack", m_ChargeExtraAttack, rkLoader );
}

ioItem* ioCountChargeItem::Clone()
{
	return new ioCountChargeItem( *this );
}

ioWeaponItem::WeaponSubType ioCountChargeItem::GetSubType() const
{
	return WST_COUNT_CHARGE;
}

void ioCountChargeItem::ClearState()
{
	m_ChargeState = CS_NONE;

	m_fCurChargeRate = 0.0f;
	m_fCurChargeRateForSkill = 0.0f;

	m_iCurMaxCount = 0;
	m_iCurCount = 0;

	m_iSkillInput = -1;
	m_bEnableDefenseState = false;
	m_bEnableAttackState = false;
	m_bEnableJumpState = false;
}

void ioCountChargeItem::SetNormalAttack( ioBaseChar *pOwner, int iCurCombo )
{
	if( !pOwner ) return;

	ClearState();

	if( m_GatheringMaxEffectID != -1 )
	{
		pOwner->EndEffect( m_GatheringMaxEffectID, false );
		m_GatheringMaxEffectID = -1;
	}

	int iExtendCnt = m_vExtendAttributeList.size();
	if( !COMPARE( iCurCombo, 0, iExtendCnt ) )
	{
		ioWeaponItem::SetNormalAttack( pOwner, iCurCombo );
		
		m_ChargeState = CS_NONE;
		m_iCurCombo = iCurCombo;

		pOwner->SetAttackMoveEnable( false );
		pOwner->SetChargingState( false );

		m_bSetChargingMoveAni = false;
	}
	else
	{
		ioEntityGroup *pGrp = pOwner->GetGroup();
		pGrp->ClearAllLoopAni( FLOAT100, true );
		pGrp->ClearAllActionAni( FLOAT100, true );

		int iAniID = pGrp->GetAnimationIdx( m_AttackReadyAniList[iCurCombo] );
		float fTimeRate = (float)m_dwChargeTime / pGrp->GetAnimationFullTime( iAniID );
		pGrp->SetActionAni( iAniID, FLOAT100, FLOAT100, FLOAT1, FLOAT1/fTimeRate, 0.0f, 0.0f, true );

		m_ChargeState = CS_CHARGING;
		m_dwAttackStartTime = FRAMEGETTIME();
		m_iCurCombo = iCurCombo;
		pOwner->SetState( CS_ATTACK );
		pOwner->SetAttackMoveEnable( false );
		pOwner->SetChargingState( true );

		DWORD dwTrackingTime = FRAMEGETTIME();
		dwTrackingTime += pGrp->GetAnimationEventTime_e( iAniID, "tracking_e" ) * fTimeRate;
		pOwner->SetAutoTargetTrackingEndTime( dwTrackingTime );
	}
}

void ioCountChargeItem::CheckNormalAttack( ioBaseChar *pOwner )
{
	switch( m_ChargeState )
	{
	case CS_NORMAL_ATTACK:
		ioAttackableItem::CheckNormalAttack( pOwner );
		break;
	case CS_CHARGING:
		OnCharging( pOwner );
		break;
	case CS_GATHERING:
		OnGathering( pOwner );
		break;
	case CS_ATTACK_FIRE:
		OnChargeFire( pOwner );
		break;
	case CS_FIRE_END:
		OnFireEnd( pOwner );
		break;
	case CS_RETREAT:
		OnRetreatState( pOwner );
		break;
	case CS_CHARGE_EXTRA:
		OnChargeExtra( pOwner );
		break;
	}

	CheckChargingMoveState( pOwner );
}

void ioCountChargeItem::OnCharging( ioBaseChar *pOwner )
{
	if( !pOwner->IsNeedProcess() )	return;

	if( pOwner->IsAttackKeyDown() )
	{
		if( m_dwAttackStartTime + m_dwChargeTime <= FRAMEGETTIME() )
		{
			ChangeToGathering( pOwner );
		}
	}
	else
	{
		ChangeToNormalAttack( pOwner );
	}
}

void ioCountChargeItem::OnGathering( ioBaseChar *pOwner )
{
	if( !pOwner->IsNeedProcess() )	return;

	DWORD dwCurTime = FRAMEGETTIME();

	// ����Ű ����. �������� ��ȯ
	if( m_bChargeCharJump && pOwner->IsJumpKeyDown() && pOwner->IsCanJumpState() && !pOwner->IsBuffLimitJump() )
	{
		pOwner->SetJumpState();
		return;
	}
	
	if( pOwner->IsAttackKeyDown() )
	{
		if( pOwner->IsDefenseKeyDown() )
		{
			int iSkillNum = pOwner->GetSkillKeyInput();
			if(iSkillNum == 0 && !pOwner->IsBuffLimitSkill() && pOwner->SetUseSkill(iSkillNum, SUT_NORMAL))
			{
				m_fCurChargeRate = FLOAT1;
				m_fCurChargeRateForSkill = m_fCurChargeRate;

				pOwner->SetAttackMoveEnable( false );
				pOwner->SetChargingState( false );
				m_bSetChargingMoveAni = false;

				if( m_GatheringMaxEffectID != -1 )
				{
					pOwner->EndEffect( m_GatheringMaxEffectID, false );
					m_GatheringMaxEffectID = -1;
				}

				ClearState();

				if( pOwner->IsNeedSendNetwork() )
				{
					SP2Packet kPacket( CUPK_ITEM_GAUGE_OK );
					FillItemGaugePacketHeader( pOwner, kPacket );
					kPacket << (int)m_ChargeState;
					kPacket << m_iCurCombo;
					P2PNetwork::SendToAllPlayingUser( kPacket );
				}
				return;
			}
		}
		else if( pOwner->IsDirKeyDoubleClick() )
		{
			CheckRetreat( pOwner, true );
			return;
		}
	}
	else
	{
		ChangeToAttackFire( pOwner );
	}
}

void ioCountChargeItem::OnChargeFire( ioBaseChar *pOwner )
{
	DWORD dwCurTime = FRAMEGETTIME();
	DWORD dwLooseTime = 0;
	if( pOwner )
		dwLooseTime = pOwner->GetActionStopTotalDelay();

	if( m_dwFireMotionEndTime + dwLooseTime < dwCurTime )
	{
		m_iCurCount++;

		if( m_iCurCount >= m_iCurMaxCount )
		{
			SetFireEndMotion( pOwner );
			return;
		}
		else
		{
			SetFireMotion( pOwner );
			return;
		}
	}
	else if( pOwner->IsNeedProcess() )
	{
		CheckCancelReserve( pOwner );
		if( ProcessCancel(pOwner) )
			return;

		if( m_dwKeyReserveTime > 0 && m_dwKeyReserveTime + dwLooseTime < dwCurTime )
		{
			if( pOwner->IsAttackKeyDown() && !m_ChargeExtraAttack.m_AttackAnimation.IsEmpty() )
			{
				ChangeToExtraAttack( pOwner );
				return;
			}

			if( pOwner->IsSettedDirection() )
			{
				CheckRetreat( pOwner, false );
				return;
			}
		}
	}
}

void ioCountChargeItem::OnFireEnd( ioBaseChar *pOwner )
{
	DWORD dwCurTime = FRAMEGETTIME();

	if( m_dwFireEndMotionEndTime > 0 && m_dwFireEndMotionEndTime < dwCurTime )
	{
		if( pOwner->IsNeedProcess() )
		{
			ProcessReserveKeyInput( pOwner );
		}
		else
		{
			ClearState();
			pOwner->SetState( CS_DELAY );
		}

		return;
	}
	else
	{
		if( pOwner->IsNeedProcess() )
		{
			if( m_dwKeyReserveTime > 0 && m_dwKeyReserveTime < dwCurTime )
				CheckReserve( pOwner );
		}
	}
}

void ioCountChargeItem::OnChargeExtra( ioBaseChar *pOwner )
{
	DWORD dwCurTime = FRAMEGETTIME();

	if( m_dwFireEndMotionEndTime > 0 && m_dwFireEndMotionEndTime < dwCurTime )
	{
		if( pOwner->IsNeedProcess() )
		{
			ProcessReserveKeyInput( pOwner );
		}
		else
		{
			ClearState();
			pOwner->SetState( CS_DELAY );
		}

		return;
	}
	else
	{
		if( pOwner->IsNeedProcess() )
		{
			CheckCancelReserve( pOwner );
			if( ProcessCancel(pOwner) )
				return;

			if( m_dwKeyReserveTime > 0 && m_dwKeyReserveTime < dwCurTime )
				CheckReserve( pOwner );
		}
	}
}

void ioCountChargeItem::OnRetreatState( ioBaseChar *pOwner )
{
	DWORD dwCurTime = FRAMEGETTIME();

	if( m_dwRetreatEndTime > 0 && m_dwRetreatEndTime < dwCurTime )
	{
		if( pOwner->IsNeedProcess() )
		{
			ProcessReserveKeyInput( pOwner );
		}
		else
		{
			ClearState();
			pOwner->SetState( CS_DELAY );
		}

		return;
	}
	else
	{
		if( pOwner->IsNeedProcess() )
		{
			if( m_dwKeyReserveTime > 0 && m_dwKeyReserveTime < dwCurTime )
			{
				if( pOwner->IsAttackKeyDown() && !m_ChargeExtraAttack.m_AttackAnimation.IsEmpty() )
				{
					ChangeToExtraAttack( pOwner );
					return;
				}

				CheckReserve( pOwner );
			}
		}

		if( m_dwRetreatForceTime > 0 && m_dwRetreatForceTime < dwCurTime )
		{
			m_dwRetreatForceTime = 0;
			pOwner->SetForcePower( m_vForceDir, m_fRetreatForceAmt, m_fRetreatForceFric );
		}
	}
}

void ioCountChargeItem::ChangeToGathering( ioBaseChar *pOwner )
{
	ioEntityGroup *pGrp = pOwner->GetGroup();
	pGrp->ClearAllActionAni( FLOAT100, true );
	pGrp->ClearAllLoopAni( FLOAT100, true );

	float fCurRate = FLOAT1;
	if( m_fGatherAniRate > 0.0f )
		fCurRate = m_fGatherAniRate;

	pGrp->SetLoopAni( m_GatherAnimation, FLOAT100, FLOAT1, FLOAT1/fCurRate );

	ioEffect *pEffect = pOwner->AttachEffect( m_GatheringMaxEffect );
	if( pEffect )
	{
		m_GatheringMaxEffectID = pEffect->GetUniqueID();
	}

	if( !m_GatheringMaxSound.IsEmpty() )
	{
		ioEntityGroup *pGrp = pOwner->GetGroup();
		if( pGrp )
		{
			g_SoundMgr.PlaySound( m_GatheringMaxSound, pGrp->GetParentSceneNode() );
		}
	}
	
	m_ChargeState = CS_GATHERING;
	m_dwGatheringStartTime = FRAMEGETTIME();

	pOwner->SetAttackMoveEnable( true );

	if( pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_ITEM_GAUGE_OK );
		FillItemGaugePacketHeader( pOwner, kPacket );
		kPacket << m_ChargeState;
		kPacket << m_iCurCombo;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

void ioCountChargeItem::ChangeToNormalAttack( ioBaseChar *pOwner )
{
	pOwner->SetAttackMoveEnable( false );
	pOwner->SetChargingState( false );
	m_bSetChargingMoveAni = false;

	if( m_GatheringMaxEffectID != -1 )
	{
		pOwner->EndEffect( m_GatheringMaxEffectID, false );
		m_GatheringMaxEffectID = -1;
	}

	m_fCurChargeRate = 0.0f;
	m_fCurChargeRateForSkill = 0.0f;

	m_ChargeState = CS_ATTACK_FIRE;

	const AttackAttribute *pAttribute = m_FormalAttack.GetAttribute( AT_NORMAL, m_iCurCombo );
	if ( pAttribute )
	{
		pOwner->SetNormalAttackByAttribute( *pAttribute );
	}

	pOwner->ApplyNormalAttackResult( m_iCurCombo );
	m_ChargeState = CS_NORMAL_ATTACK;

	if( pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_ITEM_GAUGE_OK );
		FillItemGaugePacketHeader( pOwner, kPacket );
		kPacket << m_ChargeState;
		kPacket << m_iCurCombo;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

void ioCountChargeItem::ChangeToAttackFire( ioBaseChar *pOwner )
{
	DWORD dwCurTime = FRAMEGETTIME();

	//
	pOwner->SetAttackMoveEnable( false );
	pOwner->SetChargingState( false );
	m_bSetChargingMoveAni = false;
	
	m_iCurCount = 0;
	m_dwKeyReserveTime = 0;
	m_dwFireMotionEndTime = 0;
	m_dwFireEndMotionEndTime = 0;

	if( m_GatheringMaxEffectID != -1 )
	{
		pOwner->EndEffect( m_GatheringMaxEffectID, false );
		m_GatheringMaxEffectID = -1;
	}

	m_ChargeState = CS_ATTACK_FIRE;
	if( pOwner->IsNeedProcess() )
	{
		m_fCurChargeRate = FLOAT1;
		m_fCurChargeRateForSkill = 0.0f;

		int iExtendCnt = m_vExtendAttributeList.size();
		m_iCurMaxCount = (int)(iExtendCnt * m_fCurChargeRate);
		m_iCurMaxCount = max( m_iCurMaxCount, 1 );
	}

	SetFireMotion( pOwner );

	if( pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_ITEM_GAUGE_OK );
		FillItemGaugePacketHeader( pOwner, kPacket );
		kPacket << m_ChargeState;
		kPacket << m_iCurCombo;
		kPacket << m_iCurMaxCount;
		kPacket << m_iCurCount;
		kPacket << m_fCurChargeRate;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

void ioCountChargeItem::ChangeToExtraAttack( ioBaseChar *pOwner )
{
	DWORD dwCurTime = FRAMEGETTIME();

	//
	pOwner->SetAttackMoveEnable( false );
	pOwner->SetChargingState( false );
	m_bSetChargingMoveAni = false;

	m_iCurCount = 0;
	m_dwKeyReserveTime = dwCurTime;
	m_dwFireMotionEndTime = 0;
	m_dwFireEndMotionEndTime = 0;

	m_fCurChargeRate = 0.0f;
	m_fCurChargeRateForSkill = 0.0f;

	if( m_GatheringMaxEffectID != -1 )
	{
		pOwner->EndEffect( m_GatheringMaxEffectID, false );
		m_GatheringMaxEffectID = -1;
	}

	if( pOwner->IsNeedProcess() && pOwner->IsSettedDirection() )
	{
		ioUserKeyInput::DirKeyInput eNewDirKey = pOwner->CheckCurDirKey();
		pOwner->SetTargetRotToDirKey( eNewDirKey );
	}

	m_ChargeState = CS_CHARGE_EXTRA;
	SetAttackByAttribute( pOwner, m_ChargeExtraAttack, true, FLOAT1, FLOAT1 );

	if( pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_ITEM_GAUGE_OK );
		FillItemGaugePacketHeader( pOwner, kPacket );
		kPacket << m_ChargeState;
		kPacket << m_iCurCombo;
		kPacket << pOwner->GetTargetRot();
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

void ioCountChargeItem::ChangeToRetreatState( ioBaseChar *pOwner, const ioHashString &szAni, float fAniRate, D3DXVECTOR3 vDir )
{
	if( !pOwner ) return;
	ioEntityGroup *pGrp = pOwner->GetGroup();
	if( !pGrp ) return;

	DWORD dwCurTime = FRAMEGETTIME();

	pOwner->ClearDirDoubleClick();
	pOwner->ClearReservedSliding();

	pOwner->SetForcePowerAmt( 0.0f );

	pOwner->SetAttackMoveEnable( false );
	pOwner->SetChargingState( false );
	m_bSetChargingMoveAni = false;

	m_iCurCount = 0;
	m_dwKeyReserveTime = 0;
	m_dwFireMotionEndTime = 0;
	m_dwFireEndMotionEndTime = 0;

	m_dwRetreatEndTime = 0;
	m_dwRetreatForceTime = 0;

	float fCurRate = fAniRate;
	if( fCurRate <= 0.0f )
		fCurRate = FLOAT1;

	m_ChargeState = CS_RETREAT;

	int iAniID = pGrp->GetAnimationIdx( szAni );
	pGrp->ClearAllActionAni( 0.0f, true );
	pGrp->ClearAllLoopAni( 0.0f, true );
	pGrp->SetActionAni( iAniID, FLOAT100, FLOAT100, FLOAT1, FLOAT1/fCurRate );

	m_dwRetreatEndTime = dwCurTime + pGrp->GetAnimationFullTime(iAniID) * fCurRate;

	DWORD dwTime = (DWORD)pGrp->GetAnimationEventTime_e( iAniID, "fire_sliding" );
	m_dwRetreatForceTime = dwCurTime + dwTime;

	m_vForceDir = vDir;

	DWORD dwKeyReserve = pGrp->GetAnimationEventTime_e( iAniID, "key_reserve" ) * fCurRate;
	if( dwKeyReserve > 0 )
		m_dwKeyReserveTime = dwCurTime + dwKeyReserve;

	pOwner->ApplyNormalAttackResult( m_iCurCombo );

	if( pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_ITEM_GAUGE_OK );
		FillItemGaugePacketHeader( pOwner, kPacket );
		kPacket << m_ChargeState;
		kPacket << m_iCurCombo;
		kPacket << szAni;
		kPacket << fCurRate;
		kPacket << vDir;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

void ioCountChargeItem::OnItemGaugeOK( ioBaseChar *pOwner, SP2Packet &rkPacket )
{
	int iState;
	rkPacket >> iState;
	rkPacket >> m_iCurCombo;

	switch( iState )
	{
	case CS_NONE:
		pOwner->SetAttackMoveEnable( false );
		pOwner->SetChargingState( false );
		m_bSetChargingMoveAni = false;

		if( m_GatheringMaxEffectID != -1 )
		{
			pOwner->EndEffect( m_GatheringMaxEffectID, false );
			m_GatheringMaxEffectID = -1;
		}

		m_ChargeState = CS_NONE;
		break;
	case CS_NORMAL_ATTACK:
		ChangeToNormalAttack( pOwner );
		break;
	case CS_GATHERING:
		{
			ChangeToGathering( pOwner );
		}
		break;
	case CS_ATTACK_FIRE:
		{
			rkPacket >> m_iCurMaxCount;
			rkPacket >> m_iCurCount;
			rkPacket >> m_fCurChargeRate;

			ChangeToAttackFire( pOwner );
		}
		break;
	case CS_RETREAT:
		{
			ioHashString szAni;
			float fAniRate;
			D3DXVECTOR3 vDir;

			rkPacket >> szAni >> fAniRate >> vDir;

			ChangeToRetreatState( pOwner, szAni, fAniRate, vDir );
		}
		break;
	case CS_CHARGE_EXTRA:
		{
			D3DXQUATERNION qtRot;
			rkPacket >> qtRot;

			pOwner->SetTargetRotToRotate( qtRot, false );

			ChangeToExtraAttack( pOwner );
		}
		break;
	}
}

void ioCountChargeItem::SetExtraInfoToWeapon( ioWeapon *pWeapon, bool bJumpAttack )
{
	ioAttackableItem::SetExtraInfoToWeapon( pWeapon, bJumpAttack );

	float fCurDamageRate, fCurForceRate, fCurRangeRate, fCurSpeedRate, fCurBlowRate, fCurFloatRate;
	float fCurScaleRate, fCurExplosionRate, fCurLiveTimeRate;
	
	fCurDamageRate = fCurForceRate = fCurRangeRate = fCurSpeedRate = fCurBlowRate = fCurFloatRate = FLOAT1;
	fCurScaleRate = fCurExplosionRate = fCurLiveTimeRate = FLOAT1;

	if( m_fCurChargeRate > 0.0f && !bJumpAttack )
	{
		float fCurMaxDamage = max( 0.0f, (m_fMaxDamageRate - FLOAT1) );
		float fCurMaxForce = max( 0.0f, (m_fMaxForceRate - FLOAT1) );
		float fCurMaxBlow = max( 0.0f, (m_fMaxBlowRate - FLOAT1) );

		float fCurMaxRange = max( 0.0f, (m_fMaxRangeRate - FLOAT1) );
		float fCurMaxSpeed = max( 0.0f, (m_fMaxSpeedRate - FLOAT1) );

		float fCurMaxFloat = max( 0.0f, (m_fMaxFloatRate - FLOAT1) );
		
		float fCurMaxScale = max( 0.0f, (m_fMaxScaleRate - FLOAT1) );
		float fCurMaxExplosion = max( 0.0f, (m_fMaxExplosionRate - FLOAT1) );
		float fCurMaxLiveTime = max( 0.0f, (m_fMaxLiveTimeRate - FLOAT1) );
	
		fCurDamageRate += fCurMaxDamage * m_fCurChargeRate;
        fCurForceRate += fCurMaxForce * m_fCurChargeRate;
		fCurBlowRate += fCurMaxBlow * m_fCurChargeRate;

		fCurRangeRate += fCurMaxRange * m_fCurChargeRate;
		fCurSpeedRate += fCurMaxSpeed * m_fCurChargeRate;

		fCurFloatRate += fCurMaxFloat * m_fCurChargeRate;

		fCurScaleRate += fCurMaxScale * m_fCurChargeRate;
		fCurExplosionRate += fCurMaxExplosion * m_fCurChargeRate;
		fCurLiveTimeRate += fCurLiveTimeRate * m_fCurChargeRate;

		pWeapon->SetOwnerChargeDamageRate( fCurDamageRate );
		pWeapon->SetOwnerChargeForceRate( fCurForceRate );
		pWeapon->SetOwnerChargeBlowRate( fCurBlowRate );

		pWeapon->SetOwnerChargeRangeRate( fCurRangeRate );
		pWeapon->SetOwnerChargeSpeedRate( fCurSpeedRate );

		pWeapon->SetOwnerChargeFloatRate( fCurFloatRate );

		pWeapon->SetOwnerChargeScaleRate( fCurScaleRate );
		pWeapon->SetOwnerChargeExplosionRate( fCurExplosionRate );
		pWeapon->SetOwnerChargeLiveTimeRate( fCurLiveTimeRate );

		m_fCurChargeRate = 0.0f;
		m_fCurChargeRateForSkill = 0.0f;
	}

	//
	ioZoneEffectWeapon *pZone = ToZoneEffectWeapon( pWeapon );
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
}

bool ioCountChargeItem::IsChargeAttackState( ioBaseChar *pOwner )
{
	if( pOwner->GetState() == CS_ATTACK )
	{
		switch( m_ChargeState )
		{
		case CS_CHARGING:
		case CS_GATHERING:
		case CS_ATTACK_FIRE:
			return true;
		}
	}

	return false;
}

bool ioCountChargeItem::IsEnableAttackDashCheck( ioBaseChar *pOwner, bool bCheck )
{
	if( !bCheck ) return false;

	if( pOwner->GetState() == CS_ATTACK )
	{
		switch( m_ChargeState )
		{
		case CS_GATHERING:
			return true;
		}
	}

	return false;
}

void ioCountChargeItem::CheckChargingMoveState( ioBaseChar *pOwner )
{
	if( !pOwner->IsCanAttackMove() ) return;

	DWORD dwCurTime = FRAMEGETTIME();
	ioEntityGroup *pGrp = pOwner->GetGroup();
	int iAniID = 0;

	if( pOwner->IsSettedDirection() )
	{
		if( !m_bSetChargingMoveAni )
		{
			iAniID = pGrp->GetAnimationIdx(m_ChargingMoveAnimation);
			if( iAniID == -1 )
				return;

			DWORD dwPlayTime = 0;
			ioAnimation *pAni = pGrp->GetLoopInPlay( m_GatherAnimation );
			if( pAni )
			{
				dwPlayTime = pAni->GetCurTime();
			}

			pGrp->ClearAllActionAni( FLOAT100, true );
			pGrp->ClearAllLoopAni( FLOAT100, true );

			float fCurRate = FLOAT1;
			if( m_fGatherAniRate > 0.0f )
				fCurRate = m_fGatherAniRate;

			pGrp->SetLoopAniInMid( iAniID, dwPlayTime, FLOAT100, FLOAT1, FLOAT1/fCurRate );
			m_bSetChargingMoveAni = true;
		}
	}
	else
	{
		if( m_bSetChargingMoveAni )
		{
			iAniID = pGrp->GetAnimationIdx( m_GatherAnimation );
			if( iAniID == -1 )
				return;

			DWORD dwPlayTime = 0;
			ioAnimation *pAni = pGrp->GetLoopInPlay( m_ChargingMoveAnimation );
			if( pAni )
				dwPlayTime = pAni->GetCurTime();

			pGrp->ClearAllActionAni( FLOAT100, true );
			pGrp->ClearAllLoopAni( FLOAT100, true );

			float fCurRate = FLOAT1;
			if( m_fGatherAniRate > 0.0f )
				fCurRate = m_fGatherAniRate;

			pGrp->SetLoopAniInMid( iAniID, dwPlayTime, FLOAT100, FLOAT1, FLOAT1/fCurRate );
			m_bSetChargingMoveAni = false;
		}
	}
}

void ioCountChargeItem::OnEndExtendAttack( ioBaseChar *pOwner, bool bReleaseItem )
{
	pOwner->SetAttackMoveEnable( false );
	pOwner->SetChargingState( false );
	m_bSetChargingMoveAni = false;

	m_fCurChargeRate = 0.0f;

	if( m_GatheringMaxEffectID != -1 )
	{
		pOwner->EndEffect( m_GatheringMaxEffectID, false );
		m_GatheringMaxEffectID = -1;
	}
}

bool ioCountChargeItem::IsChargeAutoTarget( ioBaseChar *pOwner )
{
	if( !pOwner )
		return false;

	if( pOwner->GetState() != CS_ATTACK )
		return false;

	if( m_fCurChargeRate == 0.0f )
		return false;

	if( m_fMaxTargetRangeRate == 0.0f || m_fMaxTargetAngleRate == 0.0f )
		return false;

	return true;
}

void ioCountChargeItem::GetAutoTargetValue( ioBaseChar *pOwner,
										    float &fRange, float &fMinAngle, float &fMaxAngle,
											AutoTargetType eType )
{
	if( IsChargeAutoTarget(pOwner) )
	{
		float fCurTargetRangeRate, fCurTargetAngleRate;
		fCurTargetRangeRate = fCurTargetAngleRate = FLOAT1;

		float fCurMaxTargetRange = max( 0.0f, (m_fMaxTargetRangeRate - FLOAT1) );
		float fCurMaxTargetAngle = max( 0.0f, (m_fMaxTargetAngleRate - FLOAT1) );

		fCurTargetRangeRate += fCurMaxTargetRange * m_fCurChargeRate;
		fCurTargetAngleRate += fCurMaxTargetAngle * m_fCurChargeRate;

		fRange = m_fAutoTargetRange * fCurTargetRangeRate;
		fMinAngle = m_fAutoTargetMinAngle / fCurTargetAngleRate;
		fMaxAngle = m_fAutoTargetMaxAngle / fCurTargetAngleRate;
	}
	else
	{
		fRange = m_fAutoTargetRange;
		fMinAngle = m_fAutoTargetMinAngle;
		fMaxAngle = m_fAutoTargetMaxAngle;
	}
}

float ioCountChargeItem::GetChargeDamageRateForSkill()
{
	float fCurDamageRate = FLOAT1;
	if( m_fCurChargeRateForSkill > 0.0f )
	{
		float fCurMaxDamage = max( 0.0f, (m_fMaxDamageRate - FLOAT1) );
		fCurDamageRate += fCurMaxDamage * m_fCurChargeRateForSkill;
	}

	return fCurDamageRate;
}

float ioCountChargeItem::GetChargeForceRateForSkill()
{
	float fCurForceRate = FLOAT1;
	if( m_fCurChargeRateForSkill > 0.0f )
	{
		float fCurMaxForce = max( 0.0f, (m_fMaxForceRate - FLOAT1) );
		fCurForceRate += fCurMaxForce * m_fCurChargeRateForSkill;
	}

	return fCurForceRate;
}

float ioCountChargeItem::GetChargeBlowRateForSkill()
{
	float fCurBlowRate = FLOAT1;
	if( m_fCurChargeRateForSkill > 0.0f )
	{
		float fCurMaxBlow = max( 0.0f, (m_fMaxBlowRate - FLOAT1) );
		fCurBlowRate += fCurMaxBlow * m_fCurChargeRateForSkill;
	}

	return fCurBlowRate;
}

float ioCountChargeItem::GetChargeRangeRateForSkill()
{
	float fCurRangeRate = FLOAT1;
	if( m_fCurChargeRateForSkill > 0.0f )
	{
		float fCurMaxRange = max( 0.0f, (m_fMaxRangeRate - FLOAT1) );
		fCurRangeRate += fCurMaxRange * m_fCurChargeRateForSkill;
	}

	return fCurRangeRate;
}

float ioCountChargeItem::GetChargeSpeedRateForSkill()
{
	float fCurSpeedRate  = FLOAT1;
	if( m_fCurChargeRateForSkill > 0.0f )
	{
		float fCurMaxSpeed = max( 0.0f, (m_fMaxSpeedRate - FLOAT1) );
		fCurSpeedRate += fCurMaxSpeed * m_fCurChargeRateForSkill;
	}

	return fCurSpeedRate;
}

float ioCountChargeItem::GetChargeFloatRateForSkill()
{
	float fCurFloatRate = FLOAT1;
	if( m_fCurChargeRateForSkill > 0.0f )
	{
		float fCurMaxFloat = max( 0.0f, (m_fMaxFloatRate - FLOAT1) );
		fCurFloatRate += fCurMaxFloat * m_fCurChargeRateForSkill;
	}

	return fCurFloatRate;
}

float ioCountChargeItem::GetChargeScaleRateForSkill()
{
	float fCurScaleRate  = FLOAT1;
	if( m_fCurChargeRateForSkill > 0.0f )
	{
		float fCurMaxScale = max( 0.0f, (m_fMaxScaleRate - FLOAT1) );
		fCurScaleRate += fCurMaxScale * m_fCurChargeRateForSkill;
	}

	return fCurScaleRate;
}

float ioCountChargeItem::GetChargeExplosionRateForSkill()
{
	float fCurExplosionRate = FLOAT1;
	if( m_fCurChargeRateForSkill > 0.0f )
	{
		float fCurMaxExplosion = max( 0.0f, (m_fMaxExplosionRate - FLOAT1) );
		fCurExplosionRate += fCurMaxExplosion * m_fCurChargeRateForSkill;
	}

	return fCurExplosionRate;
}

void ioCountChargeItem::ClearCurChargeRateForSkill()
{
	m_fCurChargeRate = 0.0f;
	m_fCurChargeRateForSkill = 0.0f;
}

void ioCountChargeItem::SetFireMotion( ioBaseChar *pOwner )
{
	DWORD dwCurTime = FRAMEGETTIME();

	m_fCurChargeRate = 0.0f;
	m_fCurChargeRateForSkill = 0.0f;

	float fCurChargeAniRate = FLOAT1;
	float fCurForceSlidingRate = FLOAT1;

	ioEntityGroup *pGrp = pOwner->GetGroup();
	if( !pGrp ) return;

	SetAttackByAttribute( pOwner, m_vExtendAttributeList[m_iCurCount], false, fCurChargeAniRate, fCurForceSlidingRate );
}

void ioCountChargeItem::SetFireEndMotion( ioBaseChar *pOwner )
{
	DWORD dwCurTime = FRAMEGETTIME();

	ioEntityGroup *pGrp = pOwner->GetGroup();
	if( !pGrp ) return;

	m_iCurCount--;
	if( !COMPARE( m_iCurCount, 0, (int)m_vExtendEndAttributeList.size() ) )
	{
		m_dwFireEndMotionEndTime = dwCurTime;
		m_ChargeState = CS_FIRE_END;
		return;
	}

	m_ChargeState = CS_FIRE_END;
	SetAttackByAttribute( pOwner, m_vExtendEndAttributeList[m_iCurCount], true );
}

void ioCountChargeItem::SetAttackByAttribute( ioBaseChar *pOwner, const AttackAttribute &rkAttr, bool bEnd, float fExtraAniRate, float fExtraForceRate )
{
	ioEntityGroup *pGrp = pOwner->GetGroup();
	if( !pGrp ) return;

	DWORD dwCurTime = FRAMEGETTIME();

	pOwner->SetAutoTarget( ATT_NORMAL );

	int iAniID	     = pGrp->GetAnimationIdx( rkAttr.m_AttackAnimation );
	float fTimeRate  = rkAttr.m_fAttackAniRate * fExtraAniRate;
	DWORD dwPreDelay = rkAttr.m_dwPreDelay;
	DWORD dwEndDelay = rkAttr.m_dwEndDelay;

	pGrp->ClearAllLoopAni( FLOAT100, true );
	pGrp->ClearAllActionAni( FLOAT100, true );

	if( pOwner && !pOwner->IsActionStop() )
		pOwner->InitActionStopDelay( true );
	else
		pOwner->InitActionStopDelay( false );

	pGrp->SetActionAni( iAniID, FLOAT100, FLOAT100, FLOAT1, FLOAT1/fTimeRate, 0.0f, dwPreDelay, true );
	
	pOwner->RefreshFireTimeList( iAniID, rkAttr.m_vWeaponInfoList, FTT_NORMAL_ATTACK, fTimeRate, dwPreDelay );
	pOwner->IncreaseWeaponIndexBase();

	if( !bEnd )
		m_dwFireMotionEndTime = dwCurTime + pGrp->GetAnimationFullTime( iAniID ) * fTimeRate + dwPreDelay + dwEndDelay;
	else
		m_dwFireEndMotionEndTime = dwCurTime + pGrp->GetAnimationFullTime( iAniID ) * fTimeRate + dwPreDelay + dwEndDelay;


	float fKeyReserve = pGrp->GetAnimationEventTime_e( iAniID, "key_reserve" ) * fTimeRate;
	if( fKeyReserve > 0.0f )
		m_dwKeyReserveTime = dwCurTime + fKeyReserve + dwPreDelay;

	pOwner->CheckCharColSkipTime( iAniID, fTimeRate, dwPreDelay );

	pOwner->SetReservedSliding( rkAttr.m_vForceInfoList, iAniID, fTimeRate, dwPreDelay, fExtraForceRate );

	DWORD dwTrackingTime = dwCurTime;
	dwTrackingTime += pGrp->GetAnimationEventTime_e( iAniID, "tracking_e" ) * fTimeRate + dwPreDelay;
	pOwner->SetAutoTargetTrackingEndTime( dwTrackingTime );

	m_szCurAni = rkAttr.m_AttackAnimation;
	m_fCurAniRate = rkAttr.m_fAttackAniRate;

	m_dwInputCancelCheckTime = dwCurTime;
	m_dwCancelCheckTime = dwCurTime;
}

bool ioCountChargeItem::CheckRetreat( ioBaseChar *pOwner, bool bFront )
{
	if( !pOwner ) return false;

	DWORD dwCurTime = FRAMEGETTIME();

	pOwner->SetAttackMoveEnable( false );
	pOwner->SetChargingState( false );
	m_bSetChargingMoveAni = false;

	ioEntityGroup *pGrp = pOwner->GetGroup();
	int iAniID = 0;

	// KeyDir
	ioUserKeyInput::DirKeyInput eDir = pOwner->CheckCurDirKey();
	float fYawD = 180.0f + 45.0f * ( eDir - ioUserKeyInput::DKI_UP );
	fYawD = ioMath::ArrangeHead( fYawD );

	D3DXQUATERNION qtRotate;
	D3DXQuaternionRotationYawPitchRoll( &qtRotate, DEGtoRAD( fYawD ), 0.0f, 0.0f );
	D3DXVECTOR3 vSightDir = qtRotate * D3DXVECTOR3( 0.0f, 0.0f, -FLOAT1 );
	D3DXVECTOR3 vMoveDir( vSightDir.x, 0.0f, vSightDir.z );
	D3DXVec3Normalize( &vMoveDir, &vMoveDir );

	D3DXQUATERNION qtTargetRot;
	ioMath::CalcDirectionQuaternion( qtTargetRot, -vMoveDir );

	float fCurYaw, fTargetYaw;
	fTargetYaw = RADtoDEG( ioMath::QuaterToYaw( qtTargetRot ) );
	fTargetYaw = ioMath::ArrangeHead( fTargetYaw );
	fCurYaw = RADtoDEG( ioMath::QuaterToYaw( pOwner->GetTargetRot() ) );
	fCurYaw = ioMath::ArrangeHead( fCurYaw );

	float fAngleGap = ioMath::AngleGap( fCurYaw, fTargetYaw );
	fAngleGap += FLOAT360;
	int iSide = (int)fAngleGap % 360;

	int iCnt = iSide / 45;
	int iRest = iSide % 45;

	switch( iCnt )
	{
	case 0:
		if( !m_szRetreatFr.IsEmpty() )
		{
			if( bFront )
			{
				ChangeToRetreatState( pOwner, m_szRetreatFr, m_fRetreatFrRate, vMoveDir );
				return true;			
			}
		}
		else
		{
			LOG.PrintTimeAndLog( 0, "ioCountChargeItem::CheckRetreat - No Front Ani" );
		}
		break;
	case 7:
		if( !m_szRetreatFr.IsEmpty() )
		{
			if( bFront )
			{
				ChangeToRetreatState( pOwner, m_szRetreatFr, m_fRetreatFrRate, vMoveDir );
				return true;
			}
		}
		else
		{
			LOG.PrintTimeAndLog( 0, "ioCountChargeItem::CheckRetreat - No Front Ani" );
		}
		break;
	case 1:
		if( iRest > 15 )
		{
			if( !m_szRetreatRt.IsEmpty() )
			{
				ChangeToRetreatState( pOwner, m_szRetreatRt, m_fRetreatRtRate, vMoveDir );
				return true;
			}
			else
				LOG.PrintTimeAndLog( 0, "ioCountChargeItem::CheckRetreat - No Right Ani" );
		}
		else
		{
			if( bFront && !m_szRetreatFr.IsEmpty() )
			{
				ChangeToRetreatState( pOwner, m_szRetreatFr, m_fRetreatFrRate, vMoveDir );
				return true;
			}
		}
		break;
	case 2:
		if( iRest <= 40 )
		{
			if( !m_szRetreatRt.IsEmpty() )
			{
				ChangeToRetreatState( pOwner, m_szRetreatRt, m_fRetreatRtRate, vMoveDir );
				return true;
			}
			else
				LOG.PrintTimeAndLog( 0, "ioCountChargeItem::CheckRetreat - No Right Ani" );
		}
		else
		{
			if( !m_szRetreatBack.IsEmpty() )
			{
				ChangeToRetreatState( pOwner, m_szRetreatBack, m_fRetreatBackRate, vMoveDir );
				return true;
			}
			else
				LOG.PrintTimeAndLog( 0, "ioCountChargeItem::CheckRetreat - No Back Ani" );
		}
		break;
	case 3:
	case 4:
		if( !m_szRetreatBack.IsEmpty() )
		{
			ChangeToRetreatState( pOwner, m_szRetreatBack, m_fRetreatBackRate, vMoveDir );
			return true;
		}
		else
		{
			LOG.PrintTimeAndLog( 0, "ioCountChargeItem::CheckRetreat - No Back Ani" );
		}
		break;
	case 5:
		if( iRest > 5 )
		{
			if( !m_szRetreatLt.IsEmpty() )
			{
				ChangeToRetreatState( pOwner, m_szRetreatLt, m_fRetreatLtRate, vMoveDir );
				return true;
			}
			else
				LOG.PrintTimeAndLog( 0, "ioCountChargeItem::CheckRetreat - No Left Ani" );
		}
		else
		{
			if( !m_szRetreatBack.IsEmpty() )
			{
				ChangeToRetreatState( pOwner, m_szRetreatBack, m_fRetreatBackRate, vMoveDir );
				return true;
			}
			else
				LOG.PrintTimeAndLog( 0, "ioCountChargeItem::CheckRetreat - No Back Ani" );
		}
		break;
	case 6:
		if( iRest <= 30 )
		{
			if( !m_szRetreatLt.IsEmpty() )
			{
				ChangeToRetreatState( pOwner, m_szRetreatLt, m_fRetreatLtRate, vMoveDir );
				return true;
			}
			else
				LOG.PrintTimeAndLog( 0, "ioCountChargeItem::CheckRetreat - No Left Ani" );
		}
		else
		{
			if( bFront && !m_szRetreatFr.IsEmpty() )
			{
				ChangeToRetreatState( pOwner, m_szRetreatFr, m_fRetreatFrRate, vMoveDir );
				return true;
			}
		}
		break;
	}

	return false;
}

void ioCountChargeItem::ProcessReserveKeyInput( ioBaseChar *pOwner )
{
	if( !pOwner )
		return;

	pOwner->NormalAttackOver();

	// ����Ű�� ������ ������ ���� ���� ó��
	if( pOwner->IsAttackKeyDown() && 
		!pOwner->IsJumpKeyDown() &&
		!pOwner->IsDefenseKeyDown() &&
		!pOwner->IsChatModeState(true) )
	{
		ClearState();

		ioObjectItem *pItem = pOwner->GetObject();
		if( ToDrinkItem( pItem ) )
		{
			pOwner->SetDrinkState();
		}
		else
		{
			if( pOwner->IsCanNormalAttack() && !pOwner->IsBuffLimitAttack() )
			{
				pOwner->SetNormalAttack( 0 );
			}
		}

		return;
	}

	// �ʻ�� ����Ǿ����� �ʻ�� �ߵ�
	if( m_iSkillInput >= 0 && !pOwner->IsBuffLimitSkill() && pOwner->SetUseSkill( m_iSkillInput, SUT_NORMAL ) )
	{
		ClearState();
		return;
	}

	// ���
	if( m_bEnableDefenseState && pOwner->IsEnableDefenseByObject() && !pOwner->IsBuffLimitDefense() )
	{
		ClearState();
		pOwner->SetDefenseState();
		return;
	}

	// ����
	if( m_bEnableJumpState && pOwner->IsCanJumpState() && !pOwner->IsBuffLimitJump() )
	{
		ClearState();
		pOwner->SetJumpState();
		return;
	}

	// ����
	if( m_bEnableAttackState )
	{
		ClearState();
		ioObjectItem *pItem = pOwner->GetObject();

		if( ToDrinkItem( pItem ) )
		{
			pOwner->SetDrinkState();
		}
		else
		{
			if( pOwner->IsCanNormalAttack() && !pOwner->IsBuffLimitAttack() )
			{
				pOwner->SetNormalAttack( 0 );
			}
		}
		return;
	}

	ClearState();
	pOwner->SetState( CS_DELAY );
	return;
}





