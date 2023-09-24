#include "StdAfx.h"
#include "ioSamuraiExpansionItem.h"

#include "ioBaseChar.h"
#include "ioPlayStage.h"
#include "ioZoneEffectWeapon.h"
#include "ioDrinkItem.h"

ioSamuraiExpansionItem::ioSamuraiExpansionItem()
{
	m_ChargeState = CS_NONE;
	m_dwAttackStartTime = 0;
	m_iCurCombo = 0;

	m_GatheringStartEffectID = -1;
	m_GatheringMaxEffectID = -1;

	m_bSetChargingMoveAni = false;
	m_fCurChargeRate = 0.0f;

	m_iCurMaxCount = 0;
	m_iCurCount = 0;

	m_dwFireMotionEndTime = 0;
	m_dwFireEndMotionEndTime = 0;

	m_dwRetreatEndTime = 0;
	m_dwRetreatForceTime = 0;
	m_bFullGathering = false;

	m_iCurBullet = 0;

	m_bFireChargeAttack = false;
}

ioSamuraiExpansionItem::ioSamuraiExpansionItem( const ioSamuraiExpansionItem &rhs )
	: ioWeaponItem( rhs ),
	m_AttackReadyAniList( rhs.m_AttackReadyAniList ),
	m_vExtendAttributeList( rhs.m_vExtendAttributeList ),
	m_vExtendEndAttributeList( rhs.m_vExtendEndAttributeList ),
	m_vExtendMaxAttributeList( rhs.m_vExtendMaxAttributeList ),
	m_vExtendMaxEndAttributeList( rhs.m_vExtendMaxEndAttributeList ),
	m_GatherAnimation( rhs.m_GatherAnimation ),
	m_GatheringMaxEffect( rhs.m_GatheringMaxEffect ),
	m_ChargingMoveAnimation( rhs.m_ChargingMoveAnimation ),
	m_fGatherAniRate( rhs.m_fGatherAniRate ),
	m_GatheringMaxSound( rhs.m_GatheringMaxSound ),
	m_GatheringStartEffect( rhs.m_GatheringStartEffect ),
	m_GatheringStartSound( rhs.m_GatheringStartSound ),
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
	m_ChargeExtraAttack( rhs.m_ChargeExtraAttack ),
	m_iMaxBullet( rhs.m_iMaxBullet )
{
	m_ChargeState = CS_NONE;
	m_dwAttackStartTime = 0;
	m_iCurCombo = 0;

	m_GatheringMaxEffectID = -1;
	m_GatheringStartEffectID = -1;

	m_bSetChargingMoveAni = false;
	m_fCurChargeRate = 0.0f;

	m_iCurMaxCount = 0;
	m_iCurCount = 0;

	m_dwFireMotionEndTime = 0;
	m_dwFireEndMotionEndTime = 0;

	m_dwRetreatEndTime = 0;
	m_dwRetreatForceTime = 0;
	m_bFullGathering =false;

	m_iCurBullet = 0;

	m_bFireChargeAttack = false;
}

ioSamuraiExpansionItem::~ioSamuraiExpansionItem()
{
}

void ioSamuraiExpansionItem::LoadProperty( ioINILoader &rkLoader )
{
	ioWeaponItem::LoadProperty( rkLoader );

	int i;
	char szKey[MAX_PATH], szBuf[MAX_PATH];

	m_vExtendAttributeList.clear();
	m_vExtendEndAttributeList.clear();
	m_vExtendMaxAttributeList.clear();
	m_vExtendMaxEndAttributeList.clear();

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

	int iExtendMaxCnt = rkLoader.LoadInt_e( "extend_max_attack_cnt", 0 );
	m_vExtendMaxAttributeList.reserve( iExtendMaxCnt );
	for( i=0 ; i<iExtendMaxCnt ; i++ )
	{
		AttackAttribute kAttribute;
		wsprintf_e( szKey, "extend_max_attack0%d", i+1 );
		LoadAttackAttribute( szKey, kAttribute, rkLoader );
		m_vExtendMaxAttributeList.push_back( kAttribute );
	}

	m_vExtendMaxEndAttributeList.reserve( iExtendMaxCnt );
	for( i=0 ; i<iExtendMaxCnt ; i++ )
	{
		AttackAttribute kAttribute;
		wsprintf_e( szKey, "extend_max_end_attack0%d", i+1 );
		LoadAttackAttribute( szKey, kAttribute, rkLoader );
		m_vExtendMaxEndAttributeList.push_back( kAttribute );
	}

	rkLoader.LoadString_e( "gathering_animation", "", szBuf, MAX_PATH );
	m_GatherAnimation = szBuf;

	m_fGatherAniRate = rkLoader.LoadFloat_e( "gathering_ani_rate", 1.0f );

	rkLoader.LoadString_e( "gathering_start_effect", "", szBuf, MAX_PATH );
	m_GatheringStartEffect = szBuf;

	rkLoader.LoadString_e( "gathering_start_sound", "", szBuf, MAX_PATH );
	m_GatheringStartSound = szBuf;


	rkLoader.LoadString_e( "gathering_max_effect", "", szBuf, MAX_PATH );
	m_GatheringMaxEffect = szBuf;

	rkLoader.LoadString_e( "gathering_max_sound", "", szBuf, MAX_PATH );
	m_GatheringMaxSound = szBuf;

	rkLoader.LoadString_e( "attack_move_ani", "", szBuf, MAX_PATH );
	m_ChargingMoveAnimation = szBuf;

	//
	rkLoader.LoadString_e( "retreat_animation_front", "", szBuf, MAX_PATH );
	m_szRetreatFr = szBuf;
	m_fRetreatFrRate = rkLoader.LoadFloat_e( "retreat_animation_front_rate", 1.0f );
	rkLoader.LoadString_e( "retreat_animation_back", "", szBuf, MAX_PATH );
	m_szRetreatBack = szBuf;
	m_fRetreatBackRate = rkLoader.LoadFloat_e( "retreat_animation_back_rate", 1.0f );
	rkLoader.LoadString_e( "retreat_animation_right", "", szBuf, MAX_PATH );
	m_szRetreatRt = szBuf;
	m_fRetreatRtRate = rkLoader.LoadFloat_e( "retreat_animation_right_rate", 1.0f );
	rkLoader.LoadString_e( "retreat_animation_left", "", szBuf, MAX_PATH );
	m_szRetreatLt = szBuf;
	m_fRetreatLtRate = rkLoader.LoadFloat_e( "retreat_animation_left_rate", 1.0f );

	m_fRetreatForceAmt = rkLoader.LoadFloat_e( "retreat_force_amt", 0.0f );
	m_fRetreatForceFric = rkLoader.LoadFloat_e( "retreat_force_friction", 1.0f );

	//
	LoadAttackAttribute_e( "charge_extra_attack", m_ChargeExtraAttack, rkLoader );

	m_iMaxBullet = rkLoader.LoadInt_e( "max_gauge", 0 );
}

ioItem* ioSamuraiExpansionItem::Clone()
{
	return new ioSamuraiExpansionItem( *this );
}

ioWeaponItem::WeaponSubType ioSamuraiExpansionItem::GetSubType() const
{
	return WST_SAMURAI_EXPANSION_ITEM;
}

void ioSamuraiExpansionItem::ClearState()
{
	m_ChargeState = CS_NONE;
	m_bFireChargeAttack = false;

	m_fCurChargeRate = 0.0f;

	m_iCurMaxCount = 0;
	m_iCurCount = 0;

	m_iSkillInput = -1;
	m_bEnableDefenseState = false;
	m_bEnableAttackState = false;
	m_bEnableJumpState = false;
	m_bFullGathering = false;
}

void ioSamuraiExpansionItem::SetNormalAttack( ioBaseChar *pOwner, int iCurCombo )
{
	if( !pOwner ) return;

	ClearState();
	m_iCurBullet = 0;
	if( m_GatheringMaxEffectID != -1 )
	{
		pOwner->EndEffect( m_GatheringMaxEffectID, false );
		m_GatheringMaxEffectID = -1;
	}
	if( m_GatheringStartEffectID != -1 )
	{
		pOwner->EndEffect( m_GatheringStartEffectID, false );
		m_GatheringStartEffectID = -1;
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
		pGrp->ClearAllLoopAni( 100.0f, true );
		pGrp->ClearAllActionAni( 100.0f, true );

		int iAniID = pGrp->GetAnimationIdx( m_AttackReadyAniList[iCurCombo] );
		float fTimeRate = (float)m_dwChargeTime / pGrp->GetAnimationFullTime( iAniID );
		pGrp->SetActionAni( iAniID, 100.0f, 100.0f, 1.0f, 1.0f/fTimeRate, 0.0f, 0.0f, true );

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

void ioSamuraiExpansionItem::CheckNormalAttack( ioBaseChar *pOwner )
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

void ioSamuraiExpansionItem::OnCharging( ioBaseChar *pOwner )
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

void ioSamuraiExpansionItem::OnGathering( ioBaseChar *pOwner )
{
	if( !pOwner || !pOwner->IsNeedProcess() )
		return;

	// ����Ű ����. �������� ��ȯ
	if( m_bChargeCharJump && pOwner->IsJumpKeyDown() && pOwner->IsCanJumpState() && !pOwner->IsBuffLimitJump() )
	{
		m_iCurBullet = 0;
		m_fCurChargeRate = 1.0f;
		pOwner->SetJumpState();
		return;
	}

	if( pOwner->IsAttackKeyDown() )
	{
		DWORD dwCurTime = FRAMEGETTIME();
		DWORD dwGapTime = dwCurTime - m_dwGatheringStartTime;
		m_fCurChargeRate = 0.0f;
		if( m_dwGatheringStartTime > 0 && m_dwMaxChargeTime > 0 )
		{
			if( m_dwMaxChargeTime > dwGapTime )
				m_fCurChargeRate = (float)dwGapTime / m_dwMaxChargeTime;
			else
				m_fCurChargeRate = 1.0f;
		}

		if( pOwner->IsDefenseKeyDown() )
		{
			int iSkillNum = pOwner->GetSkillKeyInput();
			if(iSkillNum == 0 && !pOwner->IsBuffLimitSkill() && pOwner->SetUseSkill(iSkillNum, SUT_NORMAL))
			{
				pOwner->SetAttackMoveEnable( false );
				pOwner->SetChargingState( false );
				m_bSetChargingMoveAni = false;

				if( m_GatheringMaxEffectID != -1 )
				{
					pOwner->EndEffect( m_GatheringMaxEffectID, false );
					m_GatheringMaxEffectID = -1;
				}
				if( m_GatheringStartEffectID != -1 )
				{
					pOwner->EndEffect( m_GatheringStartEffectID, false );
					m_GatheringStartEffectID = -1;
				}

				m_iCurBullet = 0;
				ClearState();

				if( pOwner->IsNeedSendNetwork() )
				{
					SP2Packet kPacket( CUPK_ITEM_GAUGE_OK );
					FillItemGaugePacketHeader( pOwner, kPacket );
					kPacket << SST_NONE;
					kPacket << m_iCurCombo;
					P2PNetwork::SendToAllPlayingUser( kPacket );
				}
				return;
			}
		}

		m_iCurBullet = 100 * m_fCurChargeRate;

		if( (m_dwGatheringStartTime+m_dwMaxChargeTime < dwCurTime) &&
			m_GatheringMaxEffectID == -1 )
		{
			m_bFullGathering = true;
			if( m_GatheringStartEffectID != -1 )
			{
				pOwner->EndEffect( m_GatheringStartEffectID, false );
				m_GatheringStartEffectID = -1;
			}

			if ( m_GatheringMaxEffectID != - 1 )
			{
				pOwner->EndEffect( m_GatheringMaxEffectID, false );
				m_GatheringMaxEffectID = -1;
			}
			ioEffect *pEffect = pOwner->AttachEffect( m_GatheringMaxEffect );
			if( pEffect )
				m_GatheringMaxEffectID = pEffect->GetUniqueID();

			if( !m_GatheringMaxSound.IsEmpty() )
			{
				ioEntityGroup *pGrp = pOwner->GetGroup();
				if( pGrp )
					g_SoundMgr.PlaySound( m_GatheringMaxSound, pGrp->GetParentSceneNode() );
			}

			if( pOwner->IsNeedSendNetwork() )
			{
				SP2Packet kPacket( CUPK_ITEM_GAUGE_OK );
				FillItemGaugePacketHeader( pOwner, kPacket );
				kPacket << SST_GATHER_MAX;
				kPacket << m_iCurCombo;
				kPacket << m_bFullGathering;
				P2PNetwork::SendToAllPlayingUser( kPacket );
			}
		}

		if( pOwner->IsDirKeyDoubleClick() )
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

void ioSamuraiExpansionItem::OnChargeFire( ioBaseChar *pOwner )
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

void ioSamuraiExpansionItem::OnFireEnd( ioBaseChar *pOwner )
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

void ioSamuraiExpansionItem::OnChargeExtra( ioBaseChar *pOwner )
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

void ioSamuraiExpansionItem::OnRetreatState( ioBaseChar *pOwner )
{
	DWORD dwCurTime = FRAMEGETTIME();

	if( m_dwRetreatEndTime > 0 && m_dwRetreatEndTime < dwCurTime )
	{
		if( pOwner->IsNeedProcess() )
		{
			// ����Ű�� ������ ������ ���� ���� ó��
			if( pOwner->IsAttackKeyDown() && 
				!pOwner->IsJumpKeyDown() &&
				!pOwner->IsDefenseKeyDown() &&
				!pOwner->IsChatModeState(true) )
			{
				ChangeToGathering( pOwner, m_bFireChargeAttack );
				return;
			}
			else
			{
				m_iCurBullet = 0;
				ProcessReserveKeyInput( pOwner );
			}
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

void ioSamuraiExpansionItem::ChangeToGathering( ioBaseChar *pOwner, bool bRefresh /* = true */ )
{
	ioEntityGroup *pGrp = pOwner->GetGroup();
	pGrp->ClearAllActionAni( 100.0f, true );
	pGrp->ClearAllLoopAni( 100.0f, true );

	float fCurRate = 1.0f;
	if( m_fGatherAniRate > 0.0f )
		fCurRate = m_fGatherAniRate;

	pGrp->SetLoopAni( m_GatherAnimation, 100.0f, 1.0f, 1.0f/fCurRate );

	
	if ( !m_bFullGathering )
	{
		if( m_GatheringStartEffectID != -1 )
		{
			pOwner->EndEffect( m_GatheringStartEffectID, false );
			m_GatheringStartEffectID = -1;
		}

		ioEffect *pEffect = pOwner->AttachEffect( m_GatheringStartEffect );
		if( pEffect )
			m_GatheringStartEffectID = pEffect->GetUniqueID();

		if( !m_GatheringStartSound.IsEmpty() )
		{
			ioEntityGroup *pGrp = pOwner->GetGroup();
			if( pGrp )
				g_SoundMgr.PlaySound( m_GatheringStartSound, pGrp->GetParentSceneNode() );
		}
	}

	m_ChargeState = CS_GATHERING;

	if ( bRefresh )
		m_dwGatheringStartTime = FRAMEGETTIME();

	pOwner->SetAttackMoveEnable( true );

	if( pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_ITEM_GAUGE_OK );
		FillItemGaugePacketHeader( pOwner, kPacket );
		kPacket << SST_GATHERING;
		kPacket << m_iCurCombo;
		kPacket << bRefresh;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

void ioSamuraiExpansionItem::ChangeToNormalAttack( ioBaseChar *pOwner )
{
	pOwner->SetAttackMoveEnable( false );
	pOwner->SetChargingState( false );
	m_bSetChargingMoveAni = false;

	if( m_GatheringMaxEffectID != -1 )
	{
		pOwner->EndEffect( m_GatheringMaxEffectID, false );
		m_GatheringMaxEffectID = -1;
	}
	if( m_GatheringStartEffectID != -1 )
	{
		pOwner->EndEffect( m_GatheringStartEffectID, false );
		m_GatheringStartEffectID = -1;
	}

	m_iCurBullet=0;
	m_fCurChargeRate = 0.0f;

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
		kPacket << SST_NORMAL_ATTACK;
		kPacket << m_iCurCombo;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

void ioSamuraiExpansionItem::ChangeToAttackFire( ioBaseChar *pOwner )
{
	DWORD dwCurTime = FRAMEGETTIME();

	//
	pOwner->SetAttackMoveEnable( false );
	pOwner->SetChargingState( false );
	m_bSetChargingMoveAni = false;

	m_iCurBullet = 0;

	m_iCurCount = 0;
	m_dwKeyReserveTime = 0;
	m_dwFireMotionEndTime = 0;
	m_dwFireEndMotionEndTime = 0;
	m_bFireChargeAttack = true;

	if( m_GatheringMaxEffectID != -1 )
	{
		pOwner->EndEffect( m_GatheringMaxEffectID, false );
		m_GatheringMaxEffectID = -1;
	}
	if( m_GatheringStartEffectID != -1 )
	{
		pOwner->EndEffect( m_GatheringStartEffectID, false );
		m_GatheringStartEffectID = -1;
	}

	m_ChargeState = CS_ATTACK_FIRE;
	if( pOwner->IsNeedProcess() )
	{
		if ( m_bFullGathering )
		{
			int iExtendCnt = m_vExtendMaxAttributeList.size();
			m_iCurMaxCount = iExtendCnt;
			m_iCurMaxCount = max( m_iCurMaxCount, 1 );
		}
		else
		{
			int iExtendCnt = m_vExtendAttributeList.size();
			m_iCurMaxCount = iExtendCnt;
			m_iCurMaxCount = max( m_iCurMaxCount, 1 );
		}
		
	}

	SetFireMotion( pOwner );

	if( pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_ITEM_GAUGE_OK );
		FillItemGaugePacketHeader( pOwner, kPacket );
		kPacket << SST_ATTACK_FIRE;
		kPacket << m_iCurCombo;
		kPacket << m_iCurMaxCount;
		kPacket << m_iCurCount;
		kPacket << m_fCurChargeRate;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

void ioSamuraiExpansionItem::ChangeToExtraAttack( ioBaseChar *pOwner )
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

	m_iCurBullet = 0;
	m_fCurChargeRate = 0.0f;

	if( m_GatheringMaxEffectID != -1 )
	{
		pOwner->EndEffect( m_GatheringMaxEffectID, false );
		m_GatheringMaxEffectID = -1;
	}
	if( m_GatheringStartEffectID != -1 )
	{
		pOwner->EndEffect( m_GatheringStartEffectID, false );
		m_GatheringStartEffectID = -1;
	}

	if( pOwner->IsNeedProcess() && pOwner->IsSettedDirection() )
	{
		ioUserKeyInput::DirKeyInput eNewDirKey = pOwner->CheckCurDirKey();
		pOwner->SetTargetRotToDirKey( eNewDirKey );
	}

	m_ChargeState = CS_CHARGE_EXTRA;
	SetAttackByAttribute( pOwner, m_ChargeExtraAttack, true, 1.0f, 1.0f );

	if( pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_ITEM_GAUGE_OK );
		FillItemGaugePacketHeader( pOwner, kPacket );
		kPacket << SST_CHARGE_EXTRA;
		kPacket << m_iCurCombo;
		kPacket << pOwner->GetTargetRot();
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

void ioSamuraiExpansionItem::ChangeToRetreatState( ioBaseChar *pOwner, const ioHashString &szAni, float fAniRate, D3DXVECTOR3 vDir )
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
		fCurRate = 1.0f;

	m_ChargeState = CS_RETREAT;

	int iAniID = pGrp->GetAnimationIdx( szAni );
	pGrp->ClearAllActionAni( 0.0f, true );
	pGrp->ClearAllLoopAni( 0.0f, true );
	pGrp->SetActionAni( iAniID, 100.0f, 100.0f, 1.0f, 1.0f/fCurRate );

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
		kPacket << SST_RETREAT;
		kPacket << m_iCurCombo;
		kPacket << szAni;
		kPacket << fCurRate;
		kPacket << vDir;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

void ioSamuraiExpansionItem::OnItemGaugeOK( ioBaseChar *pOwner, SP2Packet &rkPacket )
{
	int iState;
	rkPacket >> iState;
	rkPacket >> m_iCurCombo;

	switch( iState )
	{
	case SST_NONE:
		pOwner->SetAttackMoveEnable( false );
		pOwner->SetChargingState( false );
		m_bSetChargingMoveAni = false;

		if( m_GatheringMaxEffectID != -1 )
		{
			pOwner->EndEffect( m_GatheringMaxEffectID, false );
			m_GatheringMaxEffectID = -1;
		}
		if( m_GatheringStartEffectID != -1 )
		{
			pOwner->EndEffect( m_GatheringStartEffectID, false );
			m_GatheringStartEffectID = -1;
		}

		m_ChargeState = CS_NONE;
		break;
	case SST_NORMAL_ATTACK:
		ChangeToNormalAttack( pOwner );
		break;
	case SST_GATHERING:
		{
			bool bRefresh;
			rkPacket >> m_iCurCombo;
			rkPacket >> bRefresh;
			ChangeToGathering( pOwner, bRefresh );
		}
		break;
	case SST_ATTACK_FIRE:
		{
			rkPacket >> m_iCurMaxCount;
			rkPacket >> m_iCurCount;
			rkPacket >> m_fCurChargeRate;

			ChangeToAttackFire( pOwner );
		}
		break;
	case SST_RETREAT:
		{
			ioHashString szAni;
			float fAniRate;
			D3DXVECTOR3 vDir;

			rkPacket >> szAni >> fAniRate >> vDir;

			ChangeToRetreatState( pOwner, szAni, fAniRate, vDir );
		}
		break;
	case SST_CHARGE_EXTRA:
		{
			D3DXQUATERNION qtRot;
			rkPacket >> qtRot;

			pOwner->SetTargetRotToRotate( qtRot, false );

			ChangeToExtraAttack( pOwner );
		}
		break;
	case SST_GATHER_MAX:
		{
			rkPacket >> m_bFullGathering;
			
			if( m_GatheringStartEffectID != -1 )
			{
				pOwner->EndEffect( m_GatheringStartEffectID, false );
				m_GatheringStartEffectID = -1;
			}

			if ( m_GatheringMaxEffectID != - 1 )
			{
				pOwner->EndEffect( m_GatheringMaxEffectID, false );
				m_GatheringMaxEffectID = -1;
			}
			ioEffect *pEffect = pOwner->AttachEffect( m_GatheringMaxEffect );
			if( pEffect )
				m_GatheringMaxEffectID = pEffect->GetUniqueID();

			if( !m_GatheringMaxSound.IsEmpty() )
			{
				ioEntityGroup *pGrp = pOwner->GetGroup();
				if( pGrp )
					g_SoundMgr.PlaySound( m_GatheringMaxSound, pGrp->GetParentSceneNode() );
			}
		}
		break;
	}
}

void ioSamuraiExpansionItem::SetExtraInfoToWeapon( ioWeapon *pWeapon, bool bJumpAttack )
{
	ioAttackableItem::SetExtraInfoToWeapon( pWeapon, bJumpAttack );

	float fCurDamageRate, fCurForceRate, fCurRangeRate, fCurSpeedRate, fCurBlowRate, fCurFloatRate;
	float fCurScaleRate, fCurExplosionRate, fCurLiveTimeRate;

	fCurDamageRate = fCurForceRate = fCurRangeRate = fCurSpeedRate = fCurBlowRate = fCurFloatRate = 1.0f;
	fCurScaleRate = fCurExplosionRate = fCurLiveTimeRate = 1.0f;

	if( m_fCurChargeRate > 0.0f && !bJumpAttack )
	{
		float fCurMaxDamage = max( 0.0f, (m_fMaxDamageRate - 1.0f) );
		float fCurMaxForce = max( 0.0f, (m_fMaxForceRate - 1.0f) );
		float fCurMaxBlow = max( 0.0f, (m_fMaxBlowRate - 1.0f) );

		float fCurMaxRange = max( 0.0f, (m_fMaxRangeRate - 1.0f) );
		float fCurMaxSpeed = max( 0.0f, (m_fMaxSpeedRate - 1.0f) );

		float fCurMaxFloat = max( 0.0f, (m_fMaxFloatRate - 1.0f) );

		float fCurMaxScale = max( 0.0f, (m_fMaxScaleRate - 1.0f) );
		float fCurMaxExplosion = max( 0.0f, (m_fMaxExplosionRate - 1.0f) );
		float fCurMaxLiveTime = max( 0.0f, (m_fMaxLiveTimeRate - 1.0f) );

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
	}

	//
	ioZoneEffectWeapon *pZone = ToZoneEffectWeapon( pWeapon );
	if( pZone )
	{
		ioBaseChar *pChar = pZone->GetOwner();
		if( pChar )
		{
			D3DXVECTOR3 vDir = pChar->GetTargetRot() * D3DXVECTOR3( 0.0f, 0.0f, -1.0f );
			pZone->SetCollisionDir( vDir );
			pZone->SetZoneValue( false, false );
		}
	}
}

bool ioSamuraiExpansionItem::IsChargeAttackState( ioBaseChar *pOwner )
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

bool ioSamuraiExpansionItem::IsEnableAttackDashCheck( ioBaseChar *pOwner, bool bCheck )
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

void ioSamuraiExpansionItem::CheckChargingMoveState( ioBaseChar *pOwner )
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

			pGrp->ClearAllActionAni( 100.0f, true );
			pGrp->ClearAllLoopAni( 100.0f, true );

			float fCurRate = 1.0f;
			if( m_fGatherAniRate > 0.0f )
				fCurRate = m_fGatherAniRate;

			pGrp->SetLoopAniInMid( iAniID, dwPlayTime, 100.0f, 1.0f, 1.0f/fCurRate );
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

			pGrp->ClearAllActionAni( 100.0f, true );
			pGrp->ClearAllLoopAni( 100.0f, true );

			float fCurRate = 1.0f;
			if( m_fGatherAniRate > 0.0f )
				fCurRate = m_fGatherAniRate;

			pGrp->SetLoopAniInMid( iAniID, dwPlayTime, 100.0f, 1.0f, 1.0f/fCurRate );
			m_bSetChargingMoveAni = false;
		}
	}
}

void ioSamuraiExpansionItem::OnEndExtendAttack( ioBaseChar *pOwner, bool bReleaseItem )
{
	pOwner->SetAttackMoveEnable( false );
	pOwner->SetChargingState( false );
	m_bSetChargingMoveAni = false;

	m_iCurBullet = 0;
	m_fCurChargeRate = 0.0f;

	if( m_GatheringMaxEffectID != -1 )
	{
		pOwner->EndEffect( m_GatheringMaxEffectID, false );
		m_GatheringMaxEffectID = -1;
	}
	if( m_GatheringStartEffectID != -1 )
	{
		pOwner->EndEffect( m_GatheringStartEffectID, false );
		m_GatheringStartEffectID = -1;
	}
}

bool ioSamuraiExpansionItem::IsChargeAutoTarget( ioBaseChar *pOwner )
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

void ioSamuraiExpansionItem::GetAutoTargetValue( ioBaseChar *pOwner,
	float &fRange, float &fMinAngle, float &fMaxAngle,
	AutoTargetType eType )
{
	if( IsChargeAutoTarget(pOwner) )
	{
		float fCurTargetRangeRate, fCurTargetAngleRate;
		fCurTargetRangeRate = fCurTargetAngleRate = 1.0f;

		float fCurMaxTargetRange = max( 0.0f, (m_fMaxTargetRangeRate - 1.0f) );
		float fCurMaxTargetAngle = max( 0.0f, (m_fMaxTargetAngleRate - 1.0f) );

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

void ioSamuraiExpansionItem::SetFireMotion( ioBaseChar *pOwner )
{
	DWORD dwCurTime = FRAMEGETTIME();

	m_fCurChargeRate = 0.0f;

	float fCurChargeAniRate = 1.0f;
	float fCurForceSlidingRate = 1.0f;

	ioEntityGroup *pGrp = pOwner->GetGroup();
	if( !pGrp ) return;

	if ( m_bFullGathering )
		SetAttackByAttribute( pOwner, m_vExtendMaxAttributeList[m_iCurCount], false, fCurChargeAniRate, fCurForceSlidingRate );
	else
		SetAttackByAttribute( pOwner, m_vExtendAttributeList[m_iCurCount], false, fCurChargeAniRate, fCurForceSlidingRate );

	
}

void ioSamuraiExpansionItem::SetFireEndMotion( ioBaseChar *pOwner )
{
	DWORD dwCurTime = FRAMEGETTIME();

	ioEntityGroup *pGrp = pOwner->GetGroup();
	if( !pGrp ) return;

	m_iCurCount--;
	if ( m_bFullGathering )
	{
		if( !COMPARE( m_iCurCount, 0, (int)m_vExtendMaxEndAttributeList.size() ) )
		{
			m_dwFireEndMotionEndTime = dwCurTime;
			m_ChargeState = CS_FIRE_END;
			return;
		}
	}
	else
	{
		if( !COMPARE( m_iCurCount, 0, (int)m_vExtendEndAttributeList.size() ) )
		{
			m_dwFireEndMotionEndTime = dwCurTime;
			m_ChargeState = CS_FIRE_END;
			return;
		}
	}
	

	m_ChargeState = CS_FIRE_END;
	if ( m_bFullGathering )
		SetAttackByAttribute( pOwner, m_vExtendMaxEndAttributeList[m_iCurCount], true );
	else
		SetAttackByAttribute( pOwner, m_vExtendEndAttributeList[m_iCurCount], true );
}

void ioSamuraiExpansionItem::SetAttackByAttribute( ioBaseChar *pOwner, const AttackAttribute &rkAttr, bool bEnd, float fExtraAniRate, float fExtraForceRate )
{
	ioEntityGroup *pGrp = pOwner->GetGroup();
	if( !pGrp ) return;

	DWORD dwCurTime = FRAMEGETTIME();

	pOwner->SetAutoTarget( ATT_NORMAL );

	int iAniID	     = pGrp->GetAnimationIdx( rkAttr.m_AttackAnimation );
	float fTimeRate  = rkAttr.m_fAttackAniRate * fExtraAniRate;
	DWORD dwPreDelay = rkAttr.m_dwPreDelay;
	DWORD dwEndDelay = rkAttr.m_dwEndDelay;

	pGrp->ClearAllLoopAni( 100.0f, true );
	pGrp->ClearAllActionAni( 100.0f, true );

	if( pOwner && !pOwner->IsActionStop() )
		pOwner->InitActionStopDelay( true );
	else
		pOwner->InitActionStopDelay( false );

	pGrp->SetActionAni( iAniID, 100.0f, 100.0f, 1.0f, 1.0f/fTimeRate, 0.0f, dwPreDelay, true );

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

bool ioSamuraiExpansionItem::CheckRetreat( ioBaseChar *pOwner, bool bFront )
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
	D3DXVECTOR3 vSightDir = qtRotate * D3DXVECTOR3( 0.0f, 0.0f, -1.0f );
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
	fAngleGap += 360.0f;
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
			LOG.PrintTimeAndLog( 0, "ioSamuraiExpansionItem::CheckRetreat - No Front Ani" );
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
			LOG.PrintTimeAndLog( 0, "ioSamuraiExpansionItem::CheckRetreat - No Front Ani" );
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
				LOG.PrintTimeAndLog( 0, "ioSamuraiExpansionItem::CheckRetreat - No Right Ani" );
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
				LOG.PrintTimeAndLog( 0, "ioSamuraiExpansionItem::CheckRetreat - No Right Ani" );
		}
		else
		{
			if( !m_szRetreatBack.IsEmpty() )
			{
				ChangeToRetreatState( pOwner, m_szRetreatBack, m_fRetreatBackRate, vMoveDir );
				return true;
			}
			else
				LOG.PrintTimeAndLog( 0, "ioSamuraiExpansionItem::CheckRetreat - No Back Ani" );
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
			LOG.PrintTimeAndLog( 0, "ioSamuraiExpansionItem::CheckRetreat - No Back Ani" );
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
				LOG.PrintTimeAndLog( 0, "ioSamuraiExpansionItem::CheckRetreat - No Left Ani" );
		}
		else
		{
			if( !m_szRetreatBack.IsEmpty() )
			{
				ChangeToRetreatState( pOwner, m_szRetreatBack, m_fRetreatBackRate, vMoveDir );
				return true;
			}
			else
				LOG.PrintTimeAndLog( 0, "ioSamuraiExpansionItem::CheckRetreat - No Back Ani" );
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
				LOG.PrintTimeAndLog( 0, "ioSamuraiExpansionItem::CheckRetreat - No Left Ani" );
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

void ioSamuraiExpansionItem::ProcessReserveKeyInput( ioBaseChar *pOwner )
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