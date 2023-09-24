

#include "stdafx.h"

#include "ioBaseChar.h"

#include "ioRunningAttackSkill10.h"

#include "ioGrowthUpInfo.h"

#include "WeaponDefine.h"

#include "ioPlayStage.h"


ioRunningAttackSkill10::ioRunningAttackSkill10()
{
	ClearData();

	m_bSetHandMesh = false;
}

ioRunningAttackSkill10::ioRunningAttackSkill10( const ioRunningAttackSkill10 &rhs )
	: ioAttackSkill( rhs ),
	m_dwLoopTime( rhs.m_dwLoopTime ),
	m_bUseFireEnd( rhs.m_bUseFireEnd ),
	m_bDirection( rhs.m_bDirection ),
	m_dwSkillProtectedTime( rhs.m_dwSkillProtectedTime ),
	m_ActionBuffList( rhs.m_ActionBuffList ),
	m_fRunSpeed( rhs.m_fRunSpeed ),
	m_dwRotateTime( rhs.m_dwRotateTime ),
	m_dwRunEndEnableTime( rhs.m_dwRunEndEnableTime ),
	m_bDisableRunColSkip( rhs.m_bDisableRunColSkip ),
	m_EndAttackAttribute( rhs.m_EndAttackAttribute ),
	m_StunEndAttackAttribute( rhs.m_StunEndAttackAttribute ),
	m_RunEndType( rhs.m_RunEndType ),
	m_RunKeyEndType( rhs. m_RunKeyEndType ),
	m_WeaponDeadEndType( rhs.m_WeaponDeadEndType ),
	m_SkillEndKey( rhs.m_SkillEndKey ),
	m_bSkillEndJump( rhs.m_bSkillEndJump ),
	m_fSkillMoveDecrease( rhs.m_fSkillMoveDecrease ),
	m_dwCharRotateTime( rhs.m_dwCharRotateTime ),
	m_ThisSkillName( rhs.m_ThisSkillName ),
	m_fUpDownSpeed( rhs.m_fUpDownSpeed ),
	//m_RunExtraAttackType( rhs.m_RunExtraAttackType ),
	m_RunExtraAttack( rhs.m_RunExtraAttack ),
	m_dwExtraAttackDuration( rhs.m_dwExtraAttackDuration ),
	m_fRunExtraAttackOffsetFr( rhs.m_fRunExtraAttackOffsetFr ),
	m_fRunExtraAttackOffsetHe( rhs.m_fRunExtraAttackOffsetHe ),
	m_fRunExtraAttackOffsetSi( rhs.m_fRunExtraAttackOffsetSi ),
	m_fRunExtraAttackAngle( rhs.m_fRunExtraAttackAngle ),
	m_fRunExtranWeaponOffsetZ( rhs.m_fRunExtranWeaponOffsetZ ),
	m_fRunExtranWeaponOffsetY( rhs.m_fRunExtranWeaponOffsetY ),
	m_OwnerBuffList( rhs.m_OwnerBuffList ),
	m_bSetStartPos( rhs.m_bSetStartPos )
{
	ClearData();

	m_bSetHandMesh = false;
	m_bTitanWeaponChecked = false;
}

ioRunningAttackSkill10::~ioRunningAttackSkill10()
{
}

void ioRunningAttackSkill10::ClearData()
{
	m_RunningState = RST_NONE;

	m_dwRunningStartTime = 0;
	m_dwRunningEndTime = 0;
	m_dwRunningEndEndTime = 0;

	m_dwFireDuration = 0;

	m_dwFireStartTime = 0;

	m_JumpEndType = SEJT_DEFAULT;
	m_bTargetRot = false;

	m_CurDirKey = ioUserKeyInput::DKI_NONE;

	m_iCurBlazeCnt = 1;

	m_dwCurSkillProtectTime = 0;
}

void ioRunningAttackSkill10::LoadProperty( ioINILoader &rkLoader )
{
	ioAttackSkill::LoadProperty( rkLoader );

	char szBuf[MAX_PATH] = "";
	char szKey[MAX_PATH] = "";

	m_dwLoopTime = rkLoader.LoadInt_e( "run_duration", 0 );
	m_bUseFireEnd = rkLoader.LoadBool_e( "use_fire_end", false );
	m_bDirection = rkLoader.LoadBool_e( "direction", false );
	m_dwSkillProtectedTime = rkLoader.LoadInt_e( "skill_protect_time", 0 );

	m_dwRunEndEnableTime = (DWORD)rkLoader.LoadInt_e( "run_end_enable_time", 0 );

	m_fRunSpeed = rkLoader.LoadFloat_e( "run_speed", 0.0f );
	m_dwRotateTime = (DWORD)rkLoader.LoadInt_e( "run_rotate_speed", 0 );

	m_dwCharRotateTime = rkLoader.LoadInt_e( "char_rotate_speed", 0 );

	m_bDisableRunColSkip = rkLoader.LoadBool_e( "disable_run_col_skip", false );

	m_RunEndType = (RunEndType)rkLoader.LoadInt_e( "run_end_type", RET_FORCE_END );
	m_RunKeyEndType = (RunEndType)rkLoader.LoadInt_e( "key_input_end_type", RET_FORCE_END );
	m_WeaponDeadEndType = (RunEndType)rkLoader.LoadInt_e( "weapon_dead_end_type", RET_NORMAL_END );

	m_SkillEndKey = (SkillEndKey)rkLoader.LoadInt_e( "skill_end_key_type", SEK_ALL );
	m_bSkillEndJump = rkLoader.LoadBool_e( "skill_end_key_jump", 0 );
	m_fSkillMoveDecrease = rkLoader.LoadFloat_e( "skill_end_move_decrease", 1.0f );

	// buff
	m_ActionBuffList.clear();
	int iCnt = rkLoader.LoadInt_e( "action_buff_list", 0 );
	for( int i=0; i < iCnt; ++i )
	{
		wsprintf_e( szKey, "action_buff%d", i+1 );
		rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );

		m_ActionBuffList.push_back( szBuf );
	}	

	//end
	m_EndAttackAttribute.Init();
	LoadAttackAttribute_e( "skill_end_attack", m_EndAttackAttribute, rkLoader );

	//stun end
	m_StunEndAttackAttribute.Init();
	LoadAttackAttribute_e( "skill_stun_end_attack", m_StunEndAttackAttribute, rkLoader );


	m_ThisSkillName = (SkillNameType)rkLoader.LoadInt( "this_skill_type", SKN_DEFAULT );


	m_fUpDownSpeed = rkLoader.LoadFloat_e( "up_down_speed", 0.f );

	//////////////////////////////////////////////////////////////////////////
	m_dwExtraAttackDuration = (DWORD)rkLoader.LoadInt_e( "run_extra_attack_duration", 0 );

	wsprintf_e( szKey, "run_extra_attack_type" );
	m_RunExtraAttack.m_iWeaponIdx = rkLoader.LoadInt( szKey, NO_WEAPON_ATTRIBUTE );

	wsprintf_e( szKey, "run_extra_attack_type_resist" );
	m_RunExtraAttack.m_iResistanceIdx = rkLoader.LoadInt( szKey, NO_WEAPON_ATTRIBUTE );

	wsprintf_e( szKey, "run_extra_attack_type_wounded" );
	rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );
	m_RunExtraAttack.m_WoundedAnimation = szBuf;

	wsprintf_e( szKey, "run_extra_attack_type_wounded_time" );
	m_RunExtraAttack.m_dwWoundedDuration = rkLoader.LoadInt( szKey, 0 );

	wsprintf_e( szKey, "run_extra_attack_type_wounded_loop_ani" );
	m_RunExtraAttack.m_bWoundedLoopAni = rkLoader.LoadBool( szKey, false );

	m_fRunExtraAttackOffsetFr = rkLoader.LoadFloat_e( "run_extra_attack_offset_fr", 0.0f );
	m_fRunExtraAttackOffsetHe = rkLoader.LoadFloat_e( "run_extra_attack_offset_he", 0.0f );
	m_fRunExtraAttackOffsetSi = rkLoader.LoadFloat_e( "run_extra_attack_offset_si", 0.0f );

	m_fRunExtraAttackAngle = rkLoader.LoadFloat_e( "run_extra_attack_angle", 0.0f );

	//////////////////////////////////////////////////////////////////////////
	m_fRunExtranWeaponOffsetZ = rkLoader.LoadFloat_e( "run_extra_attack_offset_z", 0.f );
	m_fRunExtranWeaponOffsetY = rkLoader.LoadFloat_e( "run_extra_attack_offset_y", 0.f );

	m_bSetStartPos = rkLoader.LoadBool_e( "set_start_pos", false );

	LoadOwnerBuffList( rkLoader );
}

ioSkill* ioRunningAttackSkill10::Clone()
{
	return new ioRunningAttackSkill10( *this );
}

bool ioRunningAttackSkill10::IsProtected( int iDefenseBreakType ) const
{
	if( m_bDisableSkillProtect )
		return false;

	if( DBT_NONE != (DefenseBreakType)m_ProtectCancelType )
	{
		if( iDefenseBreakType == m_ProtectCancelType )   // ��ų ���� �ı�
			return false;
	}

	if( IsPreDelayProtect() )
	{
		if( m_dwPreDelayStartTime != 0 )
			return true;
	}

	DWORD dwCurTime = FRAMEGETTIME();
	if( m_dwPreDelayStartTime == 0 )
	{		
		if( m_dwCurSkillProtectTime > dwCurTime )
			return true;
	}

	if( m_RunningState == RST_RUN && dwCurTime < m_dwFireMotionEndTime )
		return true;

	if( m_RunningState == RST_FORCE_END )
		return false;

	if( m_RunningState == RST_STUN_END )
		return false;

	if( m_RunningState == RST_END )
		return false;

	return false;
}

bool ioRunningAttackSkill10::IsEndState() const
{
	if( 0.0f < m_fSkillEndNextJumpAmt )
		return false;

	DWORD dwCurTime = FRAMEGETTIME();

	if( m_RunningState == RST_FORCE_END )
		return true;

	if( m_RunningState == RST_STUN_END )
		return true;

	if( m_RunningState == RST_END )
		return true;

	return false;
}

bool ioRunningAttackSkill10::IsAttackEndState() const
{	
	if( m_fSkillEndNextJumpAmt == 0.0f )
		return false;

	DWORD dwCurTime = FRAMEGETTIME();

	if( m_RunningState == RST_FORCE_END && m_dwRunningEndEndTime < dwCurTime )
		return true;

	if( m_RunningState == RST_STUN_END && m_dwRunningEndEndTime < dwCurTime )
		return true;

	if( m_RunningState == RST_END )
		return true;

	return false;
}

bool ioRunningAttackSkill10::IsSkillMotionEnd( DWORD dwActionStop, ioBaseChar* pOwner ) const
{
	if( m_dwPreDelayStartTime > 0 )
		return false;

	if( m_dwRunningEndEndTime == 0 )
		return false;

	if( m_dwRunningEndEndTime + dwActionStop < FRAMEGETTIME() )
	{
		if( m_fSkillEndNextJumpAmt == 0.0f )
			return true;
		else
			return false;
	}

	return false;
}

bool ioRunningAttackSkill10::OnSkillStart( ioBaseChar *pChar )
{
	m_bSetUpKey = false;
	m_bSetDownKey = false;

	if( m_dwSkillProtectedTime > 0 )
		m_dwCurSkillProtectTime = FRAMEGETTIME() + m_dwSkillProtectedTime;

	if( !HasSkillMotion() )
		return false;

	pChar->SetSkillProtection();

	DWORD dwCurTime = FRAMEGETTIME();

	m_dwAttackEndTime = 0;

	InitExtraAniJump();

	m_dwAniRotateTime = 0;
	m_fAniRotateAngle = 0.0f;

	m_iCurAniRotate = 0;

	m_dwEnableSwitchTime = 0;
	m_dwEnableSemiSwitchTime = 0;

	if( m_dwSwitchSkill > 0 )
		m_dwEnableSwitchTime = dwCurTime + m_dwSwitchSkill;
	if( m_dwSemiSwitchSkill > 0 )
		m_dwEnableSemiSwitchTime = dwCurTime + m_dwSemiSwitchSkill;

	m_dwSkillStartTime = FRAMEGETTIME();

	ClearData();

	ioPlayStage *pStage = g_SkillMaker.GetPlayStage();

	// ExtraAttackForce
	ioGrowthAttackForceUpInfo *pForceInfo = NULL;
	pForceInfo = ToAttackForceUpInfo(GetGrowthUpInfoByType(GT_SKILL_ATTACK_FORCE, pChar->GetCharName()));
	m_fExtraAttackMoveForce = 0.0f;
	m_fExtraAttackMoveFriction = 0.0f;

	if( pForceInfo )
	{
		m_fExtraAttackMoveForce = pForceInfo->GetForceValue(pStage);
		m_fExtraAttackMoveFriction = pForceInfo->GetFrictionValue(pStage);
	}

	SetExtraAttackForce();

	m_bSetHandMesh = false;

	// �⺻ ���� �տ��� ����
	if( m_bUseItemMesh )
	{
		pChar->ShowWeaponItemMesh( false );
	}

	if( m_bInitJumpPower )
	{
		pChar->SetCurJumpPowerAmt( 0.0f );
		pChar->SetGravityAmt( 0.0f );
	}

	if( 0 < m_dwCharRotateTime )
		pChar->SetSkillMoveEnable( true, false );

	m_dwWeaponBaseIndex = pChar->IncreaseWeaponIndexBase();
	if( !StartPreDelayAnimation( pChar ) )	// Delay Animation�� ����
	{
		SetRunState( pChar );
	}
	else
	{
		DWORD dwTrackingTime = 0;		
		ioEntityGroup *pGrp = pChar->GetGroup();
		if( pGrp )
		{
			ioHashString szPreDelay = GetPreDelayAniName();
			int iAniID = pGrp->GetAnimationIdx( szPreDelay );
			if( iAniID != -1 )
			{
				dwTrackingTime = pGrp->GetAnimationEventTime_e( iAniID, "tracking_e" );			
			}
		}

		DWORD dwPreDelay = GetPreDelayTime();

		if( dwTrackingTime > 0 )
		{
			dwTrackingTime += dwCurTime;	
		}
		else
		{
			dwTrackingTime = dwCurTime + dwPreDelay;
		}

		if( IsUseAutoTargetValue() )
		{
			pChar->SetAutoTarget( ATT_SKILL );
			pChar->SetAutoTargetTrackingEndTime( dwTrackingTime );		
		}
	}

	CheckSkillDesc( pChar );
	CheckChangeDirRot( pChar );

	SetSkillNameChatBubble( pChar );


	if( m_ThisSkillName == SKN_TITAN )
	{
		m_bTitanWeaponChecked = false;

		ioWeapon* pWeapon = pChar->SkillFireExplicit( GetAttribute().m_vWeaponInfoList.front(),
			GetName(),
			m_dwWeaponBaseIndex,
			pChar->GetWorldPosition() );

		ioAdhesiveMissileWeapon4* pWeapon4 = ToAdhesiveMissileWeapon4( pWeapon );
		if( pWeapon4 )
			pWeapon4->SetUseColCheck( false );
	}

	if ( m_bSetStartPos )
	{
		m_vStartPos = pChar->GetWorldPosition();
		m_qtStartRot = pChar->GetWorldOrientation();
	}

	return true;
}

void ioRunningAttackSkill10::SetRunState( ioBaseChar *pChar )
{
	if( pChar->IsNeedProcess() && !m_ActionBuffList.empty() )
	{
		pChar->SetRandomSeed( 0 );

		bool bResult = false;
		ioHashStringVec vBuffList;

		ioHashString szItemName;
		ioItem *pItem = GetOwnerItem();
		if( pItem )
		{
			szItemName = pItem->GetName();
		}

		float fValue = 0.0f;
		ioGrowthNormalUpInfo *pUpInfo = ToNormalUpInfo( GetGrowthUpInfoByType(GT_BUFF_DURATION, pChar->GetCharName()) );
		if( pUpInfo )
		{
			ioPlayStage *pStage = g_SkillMaker.GetPlayStage();
			fValue = pUpInfo->GetValue(pStage);
		}


		int iCnt = m_ActionBuffList.size();
		for( int i=0; i < iCnt; ++i )
		{
			ioHashString szBuffName = m_ActionBuffList[i];

			if( pChar->IsHasCrown() && m_bDisableKingSkill )
				continue;
			else if( pChar->HasBuff( BT_ABSOLUTE_PROTECTION ) )
				continue;

			if( !szBuffName.IsEmpty() )
			{
				ioBuff *pBuff = pChar->AddNewBuffWithAnimateTime( szBuffName, pChar->GetCharName(), szItemName, this, 0 );
				if( pBuff )
				{
					pBuff->SetExtraDurationByGrowth( fValue );

					bResult = true;
					vBuffList.push_back( szBuffName );
				}
			}
		}

		if( bResult )
		{
			int iCurBuffCnt = vBuffList.size();
			if( pChar->IsNeedSendNetwork() )
			{
				if( Help::CheckEnableTimeGapUser() )
				{
					LOG.PrintTimeAndLog( 0, "PC -SkillAddBuff5 : %s, %d", GetName().c_str(), FRAMEGETTIME() );
				}
				SP2Packet kPacket( CUPK_ADD_NEW_BUFF );
				kPacket << pChar->GetCharName();
				kPacket << pChar->GetCharName();
				kPacket << szItemName;
				kPacket << pChar->GetRandomSeed();
				kPacket << false;					// Ÿ��Ƚ�� üũ
				kPacket << 0.0f;
				kPacket << fValue;
				kPacket << 0.0f;
				kPacket << COWT_NONE;				// Check CloseOrderBuff(12.07.13 ����)
				kPacket << iCurBuffCnt;

				for( i=0; i < iCurBuffCnt; i++ )
				{
					kPacket << vBuffList[i];
				}

				kPacket << false;					// Use Force

				P2PNetwork::SendToAllPlayingUser( kPacket );
			}
		}
	}

	AttackSkillFire( pChar );

	m_dwRunningStartTime = FRAMEGETTIME();
}

void ioRunningAttackSkill10::AttackSkillFire( ioBaseChar *pChar )
{
	ioEntityGroup *pGrp = pChar->GetGroup();
	int iAniID = pGrp->GetAnimationIdx( GetAttribute().m_AttackAnimation );
	if( iAniID == -1 )	return;

	float fTimeRate  = GetAttackTimeRate();

	pGrp->ClearAllLoopAni( 100.0f, true );
	pGrp->ClearAllActionAni( 100.0f, true );
	pGrp->SetLoopAni( iAniID, 100.0f, 1.0f, 1.0f/fTimeRate );

	pChar->AttachEffect( m_SkillEffectName );

	if( m_ThisSkillName != SKN_TITAN)
	{
		pChar->RefreshFireTimeListWithBase( m_dwWeaponBaseIndex,
			iAniID,
			GetAttribute().m_vWeaponInfoList,
			FTT_SKILL_ATTACK,
			fTimeRate,
			0 );
	}

	DWORD dwCurTime = FRAMEGETTIME();
	m_dwMotionStartTime = m_dwMotionEndTime = dwCurTime;
	m_dwMotionEndTime += m_dwLoopTime;
	m_dwRunningEndTime = m_dwMotionEndTime;

	if( !m_bDisableRunColSkip )
		pChar->SetCharColSkipSkillTime( dwCurTime, m_dwMotionEndTime );

	if( m_dwSkillProtectedTime <= 0 )
	{
		m_dwFireMotionEndTime = dwCurTime;
		m_dwFireMotionEndTime += pGrp->GetAnimationEventTime_e( iAniID, "firemotion_e" ) * fTimeRate;
	}
	else
	{
		m_dwFireMotionEndTime = dwCurTime;
		m_dwFireMotionEndTime += m_dwSkillProtectedTime;
	}

	if( !m_bUseFireEnd )
		m_dwFireMotionEndTime = m_dwMotionEndTime;

	DWORD dwFireTime = pGrp->GetAnimationEventTime_e( iAniID, "fire_s" ) * fTimeRate;
	m_dwFireDuration = m_dwLoopTime - dwFireTime;

	m_dwFireStartTime = dwCurTime;
	m_dwFireStartTime += dwFireTime;

	pChar->SetReservedSliding( GetAttribute().m_vForceInfoList, iAniID, fTimeRate, 0 );

	m_RunningState = RST_RUN;

	// ��� ������ ó��
	if( !m_bSetHandMesh && m_bUseItemMesh && !m_HandMesh.IsEmpty() )
	{
		pChar->ChangeSkillWeaponMesh( this, m_HandMesh, m_bVisibleEquipMesh );
		m_bSetHandMesh = true;
	}

	if( !m_bReduceGauge )
	{
		pChar->ReduceNeedGaugeBySkill( this, GetNeedGauge(pChar->GetCharName()) );
		m_bReduceGauge = true;
	}

	// run extra attack
	if( m_RunExtraAttack.m_iWeaponIdx > 0 )
		m_dwExtraAttackCheckTime = dwCurTime;
	else
		m_dwExtraAttackCheckTime = 0;
}

void ioRunningAttackSkill10::SetForceEndState( ioBaseChar *pChar )
{
	if( !pChar ) return;

	// clear buff
	int iBuffCnt = m_ActionBuffList.size();
	for( int i=0; i < iBuffCnt; ++i )
	{
		pChar->RemoveBuff( m_ActionBuffList[i] );
	}

	g_WeaponMgr.DestroyReserveContactWeapon( pChar->GetCharName() );
	g_WeaponMgr.DestroyReserveZoneWeapon( pChar->GetCharName() );
	g_WeaponMgr.CheckAdhesiveWeapon3( pChar->GetCharName() );
	g_WeaponMgr.CheckAdhesiveWeapon4( pChar->GetCharName() );

	float fTimeRate = m_EndAttackAttribute.m_fAttackAniRate;
	if( fTimeRate <= 0.0f )
		fTimeRate = 1.0f;

	ioEntityGroup *pGrp = pChar->GetGroup();
	int iAniID = pGrp->GetAnimationIdx( m_EndAttackAttribute.m_AttackAnimation );
	if( iAniID != -1 )
	{
		pChar->RefreshFireTimeListWithBase( m_dwWeaponBaseIndex,
			iAniID,
			m_EndAttackAttribute.m_vWeaponInfoList,
			FTT_SKILL_ATTACK,
			fTimeRate,
			m_EndAttackAttribute.m_dwPreDelay );

		DWORD dwCurTime = FRAMEGETTIME();
		m_dwRunningEndEndTime = dwCurTime;
		m_dwRunningEndEndTime += (pGrp->GetAnimationFullTime(iAniID) * fTimeRate);

		m_dwEnableReserveTime = dwCurTime;
		m_dwEnableReserveTime += pGrp->GetAnimationEventTime_e( iAniID, "key_reserve" ) * fTimeRate;

		pGrp->ClearAllActionAni( 100.0f, true );
		pGrp->ClearAllLoopAni( 100.0f, true );
		pGrp->SetActionAni( iAniID, 100.0f, 100.0f, 1.0f, 1.0f/fTimeRate );
	}

	pChar->SetSkillMoveEnable( false, false );
	pChar->SetMoveDirByRotate( pChar->GetTargetRot() );
	pChar->SetCharColSkipSkillTime( 0, 0 );

	m_RunningState = RST_FORCE_END;
}

void ioRunningAttackSkill10::SetStunEndState( ioBaseChar *pChar )
{
	if( !pChar ) return;

	// clear buff
	int iBuffCnt = m_ActionBuffList.size();
	for( int i=0; i < iBuffCnt; ++i )
	{
		pChar->RemoveBuff( m_ActionBuffList[i] );
	}

	g_WeaponMgr.DestroyReserveContactWeapon( pChar->GetCharName() );
	g_WeaponMgr.DestroyReserveZoneWeapon( pChar->GetCharName() );
	g_WeaponMgr.CheckAdhesiveWeapon3( pChar->GetCharName() );
	g_WeaponMgr.CheckAdhesiveWeapon4( pChar->GetCharName() );

	float fTimeRate = m_StunEndAttackAttribute.m_fAttackAniRate;
	if( fTimeRate <= 0.0f )
		fTimeRate = 1.0f;

	ioEntityGroup *pGrp = pChar->GetGroup();
	int iAniID = pGrp->GetAnimationIdx( m_StunEndAttackAttribute.m_AttackAnimation );	
	if( iAniID != -1 )	
	{
		pChar->RefreshFireTimeListWithBase( m_dwWeaponBaseIndex,
			iAniID,
			m_StunEndAttackAttribute.m_vWeaponInfoList,
			FTT_SKILL_ATTACK,
			fTimeRate,
			m_StunEndAttackAttribute.m_dwPreDelay );

		DWORD dwCurTime = FRAMEGETTIME();
		m_dwRunningEndEndTime = dwCurTime;
		m_dwRunningEndEndTime += ( pGrp->GetAnimationFullTime(iAniID) * fTimeRate );

		m_dwEnableReserveTime = dwCurTime;
		m_dwEnableReserveTime += pGrp->GetAnimationEventTime_e( iAniID, "key_reserve" ) * fTimeRate;

		pGrp->ClearAllActionAni( 100.0f, true );
		pGrp->ClearAllLoopAni( 100.0f, true );
		pGrp->SetActionAni( iAniID, 100.0f, 100.0f, 1.0f, 1.0f/fTimeRate );
	}

	pChar->SetSkillMoveEnable( false, false );
	pChar->SetMoveDirByRotate( pChar->GetTargetRot() );
	pChar->SetCharColSkipSkillTime( 0, 0 );

	m_RunningState = RST_STUN_END;
}

void ioRunningAttackSkill10::SetEndState( ioBaseChar *pChar )
{
	if( !pChar ) return;

	DWORD dwCurTime = FRAMEGETTIME();
	m_dwRunningEndEndTime = dwCurTime;
	m_dwEnableReserveTime = dwCurTime;

	pChar->SetSkillMoveEnable( false, false );
	pChar->SetMoveDirByRotate( pChar->GetTargetRot() );
	pChar->SetCharColSkipSkillTime( 0, 0 );

	m_RunningState = RST_END;
}

void ioRunningAttackSkill10::OnProcessState( ioBaseChar *pChar, ioPlayStage *pStage )
{
	DWORD dwCurTime = FRAMEGETTIME();

	switch( m_RunningState )
	{
	case RST_NONE:
		{
			DWORD dwPreDelay = GetPreDelayTime();
			if( m_dwPreDelayStartTime + dwPreDelay < dwCurTime )
			{
				m_dwPreDelayStartTime = 0;
				EndPreDelayEffect( pChar );
				SetRunState( pChar );
				SetOwnerBuffList( pChar );
			}
		}
		break;
	case RST_RUN:
		{		
			//��ų �ð��� �Ǽ� ���� ������
			if( m_dwRunningEndTime < dwCurTime )
			{
				m_dwRunningEndTime = 0;
				CheckSkillEnd( pChar, m_RunEndType );
				return;
			}
			else
			{
				//������ �ҽ� ������ 
				/*if( !CheckControlWeapon( pChar, dwCurTime ) && m_WeaponDeadEndType != RET_NORMAL_END )
				{
				m_dwRunningEndTime = 0;
				CheckSkillEnd( pChar, m_WeaponDeadEndType );					
				return;
				}*/

				//Ű�� ��������
				if( CheckSkillEndKey( pChar ) && pChar->IsNeedProcess() &&
					m_dwRunningStartTime + m_dwRunEndEnableTime < dwCurTime )
				{
					m_dwRunningEndTime = 0;
					CheckSkillEnd( pChar, m_RunKeyEndType );

					if( pChar->IsNeedSendNetwork() )
					{
						SP2Packet kPacket( CUPK_SKILL_EXTRA_INFO );
						kPacket << pChar->GetCharName();
						kPacket << GetName();
						kPacket << SST_RUN;
						kPacket << (int)m_RunKeyEndType;
						kPacket << (int)m_JumpEndType;
						P2PNetwork::SendToAllPlayingUser( kPacket );
						return;
					}
					return;
				}
				ProcessExtraAttack( pChar );

				CheckKeyInput( pChar );
				ProcessRotate( pChar );
				ProcessCharRotate( pChar );
				if( !ProcessUpDown( pChar ) )
				{
					SetEndState( pChar );
					return;
				}

				pChar->SetCurMoveSpeed( m_fRunSpeed );
			}
		}
		break;
	}
}

void ioRunningAttackSkill10::OnSkillEnd( ioBaseChar *pChar )
{
	// �⺻���� ����
	if( m_bUseItemMesh )
	{
		pChar->ShowWeaponItemMesh( true );
	}

	if( !m_HandMesh.IsEmpty() )
	{
		pChar->RestoreSkillWeaponMesh( this, m_HandMesh, m_bVisibleEquipMesh );
	}
	m_bSetHandMesh = false;

	// clear buff
	int iBuffCnt = m_ActionBuffList.size();
	for( int i=0; i < iBuffCnt; ++i )
	{
		pChar->RemoveBuff( m_ActionBuffList[i] );
	}

	g_WeaponMgr.DestroyReserveContactWeapon( pChar->GetCharName() );
	g_WeaponMgr.DestroyReserveZoneWeapon( pChar->GetCharName() );
	g_WeaponMgr.DestroyReserveAdhesive3( pChar->GetCharName() );
	g_WeaponMgr.DestroyReserveAdhesive4( pChar->GetCharName() );

	RemoveExtraAttackForce();
	EndPreDelayEffect( pChar );

	pChar->EndEffect( m_SkillEffectName );

	m_bReduceGauge = false;
	pChar->SetSkillMoveEnable( false, false );
	pChar->SetMoveDirByRotate( pChar->GetTargetRot() );
	pChar->RestoreSkillProtection();
	pChar->SetCharColSkipSkillTime( 0, 0 );

	m_SkillUseType = SUT_NORMAL;

	if( !m_SkillEndEffectName.IsEmpty() )
		pChar->AttachEffect( m_SkillEndEffectName );

	if ( m_bSetStartPos && !pChar->HasBuff( BT_ESCAPE_DROP_ZONE ) )
	{
		pChar->SetWorldPosition( m_vStartPos );
		pChar->SetTargetRotToRotate( m_qtStartRot, true );
	}

	RemoveOwnerBuffList( pChar );
}

void ioRunningAttackSkill10::ApplyAttributeToWeapon( ioWeapon *pWeapon, ioPlayStage *pStage )
{
	ioSkill::ApplyAttributeToWeapon( pWeapon, pStage );

	ioZoneEffectWeapon *pZone = ToZoneEffectWeapon( pWeapon );
	ioAdhesiveMissileWeapon3 *pAdhesive3 = ToAdhesiveMissileWeapon3( pWeapon );
	ioAdhesiveMissileWeapon4 *pAdhesive4 = ToAdhesiveMissileWeapon4( pWeapon );
	if( pZone )
	{
		ioBaseChar *pChar = pZone->GetOwner();
		if( pChar )
		{				
			D3DXVECTOR3 vDir = pChar->GetTargetRot() * D3DXVECTOR3( 0.0f, 0.0f, -1.0f );
			pZone->SetCollisionDir( vDir );
			pZone->SetZoneValue( true, true );
		}		
	}
	else if( pAdhesive3 )
	{
		pAdhesive3->CheckOwnerAttach( true );
	}
	else if( pAdhesive4 )
	{
		pAdhesive4->SetState( ioAdhesiveMissileWeapon4::WS_ATTACH_MOVE );
	}
	else if( !m_bUseFireEnd )
	{
		pWeapon->SetLiveTime( m_dwFireDuration );
	}

}

bool ioRunningAttackSkill10::CheckSkillEndKey( ioBaseChar *pOwner )
{
	switch( m_SkillEndKey )
	{	
	case SEK_ATTACK:
		if( pOwner->IsAttackKey() )
		{	
			if( m_bSkillEndJump )
				m_JumpEndType = SEJT_JUMP;

			return true;
		}
		break;
	default:
		if( pOwner->IsAttackKey() )
		{
			if( m_bSkillEndJump )
				m_JumpEndType = SEJT_JUMP;

			return true;
		}
	}

	if( m_bSkillEndJump )
		m_JumpEndType = SEJT_NOT_JUMP;

	return false;
}

void ioRunningAttackSkill10::CheckSkillEnd( ioBaseChar *pOwner, RunEndType endType )
{
	pOwner->SetCurMoveSpeed( m_fRunSpeed * m_fSkillMoveDecrease );

	switch( endType )
	{
	case RET_FORCE_END:
		SetForceEndState( pOwner );
		break;
	case RET_STUN_END:
		SetStunEndState( pOwner );
		break;
	default:
		SetEndState( pOwner );
		break;
	}
}

void ioRunningAttackSkill10::CheckKeyInput( ioBaseChar *pOwner )
{
	if( !pOwner ) return;
	if( !pOwner->IsNeedProcess() ) return;
	if( 0 < m_dwCharRotateTime ) return;


	bool bChangeUpDown = false;
	// Check UpDown
	if( m_bSetUpKey )
	{
		if( !pOwner->IsJumpKeyDown() )
		{
			bChangeUpDown = true;
			m_bSetUpKey = false;

			if( pOwner->IsDefenseKeyDown() )
				m_bSetDownKey = true;
		}
	}
	else if( m_bSetDownKey )
	{
		if( !pOwner->IsDefenseKeyDown() )
		{
			bChangeUpDown = true;
			m_bSetDownKey = false;

			if( pOwner->IsJumpKeyDown() )
				m_bSetUpKey = true;
		}
	}
	else
	{
		if( pOwner->IsDefenseKeyDown() )
		{
			bChangeUpDown = true;
			m_bSetUpKey = false;
			m_bSetDownKey = true;
		}

		if( pOwner->IsJumpKeyDown() )
		{
			bChangeUpDown = true;
			m_bSetUpKey = true;
			m_bSetDownKey = false;
		}
	}


	ioUserKeyInput::DirKeyInput eNewDirKey = pOwner->CheckCurDirKey();
	if( eNewDirKey != m_CurDirKey || bChangeUpDown )
	{
		m_bTargetRot = false;
		m_CurDirKey = eNewDirKey;

		if( pOwner->IsNeedSendNetwork() )
		{
			SP2Packet kPacket( CUPK_SKILL_EXTRA_INFO );
			kPacket << pOwner->GetCharName();
			kPacket << GetName();
			kPacket << SST_ROTATE;
			kPacket << pOwner->GetTargetRot();
			kPacket << (int)m_CurDirKey;
			kPacket << m_bSetUpKey;
			kPacket << m_bSetDownKey;
			P2PNetwork::SendToAllPlayingUser( kPacket );
		}
	}
}

void ioRunningAttackSkill10::ProcessRotate( ioBaseChar *pOwner )
{
	if( !pOwner ) return;
	if( m_dwRotateTime <= 0 ) return;
	if( m_bTargetRot ) return;
	if( 0 < m_dwCharRotateTime ) return;

	if( m_CurDirKey == ioUserKeyInput::DKI_NONE )
		return;

	float fFrameGap = g_FrameTimer.GetCurFrameGap();
	float fRate = fFrameGap / m_dwRotateTime;
	float fRotateAngle = 360.0f * fRate;

	D3DXQUATERNION qtCurRot = pOwner->GetTargetRot();
	D3DXVECTOR3 vCurMoveDir = qtCurRot * D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	D3DXVec3Normalize( &vCurMoveDir, &vCurMoveDir );

	// ��ǥ�� ����üũ �� ó��
	float fYawD = 180.0f + 45.0f * ( m_CurDirKey - ioUserKeyInput::DKI_UP );
	fYawD = ioMath::ArrangeHead( fYawD );

	float fYawR = DEGtoRAD( fYawD );
	D3DXQUATERNION qtRotate;
	D3DXQuaternionRotationYawPitchRoll( &qtRotate, fYawR, 0.0f, 0.0f );

	D3DXVECTOR3 vTargetDir = qtRotate * D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	D3DXVec3Normalize( &vTargetDir, &vTargetDir );

	float fDotValue = D3DXVec3Dot( &vCurMoveDir, &vTargetDir );
	float fGapAngle = RADtoDEG( acosf(fDotValue) );

	if( fDotValue >= 0.98f )
	{
		pOwner->SetTargetRotToRotate( qtRotate, true );
		return;
	}

	D3DXVECTOR3 vAxis;
	if( fDotValue <= -0.98f )
	{
		vAxis = ioMath::UNIT_Y;
	}
	else
	{
		D3DXVec3Cross( &vAxis, &vTargetDir, &vCurMoveDir );
		D3DXVec3Normalize( &vAxis, &-vAxis );
	}

	D3DXQUATERNION qtNewRot;
	if( fGapAngle < fRotateAngle )
		D3DXQuaternionRotationAxis( &qtNewRot, &vAxis, DEGtoRAD(fGapAngle) );
	else
		D3DXQuaternionRotationAxis( &qtNewRot, &vAxis, DEGtoRAD(fRotateAngle) );

	D3DXVECTOR3 vNewDir = qtNewRot * vCurMoveDir;
	D3DXVec3Normalize( &vNewDir, &vNewDir );

	ioMath::CalcDirectionQuaternion( qtNewRot, -vNewDir );
	pOwner->SetTargetRotToRotate( qtNewRot, true );

}

void ioRunningAttackSkill10::ProcessCharRotate( ioBaseChar *pChar )
{
	if( m_dwCharRotateTime == 0 )
		return;

	// ĳ���� ȸ��
	float fTimePerSec = g_FrameTimer.GetSecPerFrame();

	float fRate = (fTimePerSec*1000) / m_dwCharRotateTime;
	float fRotateAngle = 360.0f * fRate;

	fRotateAngle = ioMath::ArrangeHead( fRotateAngle );

	D3DXQUATERNION	qtNewRot;
	D3DXVECTOR3 vAxis = ioMath::UNIT_Y;

	D3DXQuaternionRotationAxis( &qtNewRot, &vAxis, DEGtoRAD(fRotateAngle));

	D3DXQUATERNION qtCharRot = pChar->GetTargetRot();
	D3DXVECTOR3 vDir = qtCharRot * D3DXVECTOR3( 0.0, 0.0, -1.0f );
	vDir = qtNewRot * vDir;

	ioMath::CalcDirectionQuaternion( qtNewRot, -vDir );
	pChar->SetTargetRotToRotate( qtNewRot, true, true );
}


bool ioRunningAttackSkill10::ProcessUpDown( ioBaseChar *pChar )
{
	if( !pChar )
		return false;

	ioPlayStage *pStage = pChar->GetCreator();
	if( !pStage )
		return false;

	// ĳ���� ȸ��
	float fTimePerSec = g_FrameTimer.GetSecPerFrame();

	D3DXVECTOR3 vCurPos = pChar->GetWorldPosition();
	float fMapHeight = pStage->GetMapHeight( vCurPos.x, vCurPos.z, pChar, false, pChar->GetMidHeightByRate(1.0f) );

	// UpDown Move
	if( m_bSetUpKey || m_bSetDownKey )
	{
		float fCurHeightAmt = 0.0f;
		float fCurUpDownSpeed = m_fUpDownSpeed * fTimePerSec;

		if( m_bSetUpKey )
			fCurHeightAmt += fCurUpDownSpeed;
		else if( m_bSetDownKey )
			fCurHeightAmt -= fCurUpDownSpeed;

		vCurPos.y += fCurHeightAmt;

		vCurPos.y = max( vCurPos.y, fMapHeight );

		pStage->CheckFlyMapLimit( &vCurPos );
		pStage->CheckAxisLimit( &vCurPos );
		pChar->SetWorldPosition( vCurPos );
	}

	return true;
}


void ioRunningAttackSkill10::ProcessExtraAttack( ioBaseChar *pOwner )
{
	if( !pOwner ) return;
	if( m_dwExtraAttackCheckTime == 0 ) return;

	if( m_RunExtraAttack.m_iWeaponIdx <= 0 )
		return;

	if( m_RunningState != RST_RUN ) return;

	DWORD dwCurTime = FRAMEGETTIME();

	if( m_dwExtraAttackCheckTime < dwCurTime )
	{
		D3DXVECTOR3 vPos = pOwner->GetMidPositionByRate();
		D3DXVECTOR3 vAddZ = pOwner->GetWorldOrientation() * D3DXVECTOR3( 0.0f, 0.0f, -1.0f );

		D3DXVec3Normalize( &vAddZ, &vAddZ );
		vAddZ = vAddZ * m_fRunExtranWeaponOffsetZ;

		vPos = vPos + vAddZ + D3DXVECTOR3( 0.f, m_fRunExtranWeaponOffsetZ, 0.f );

		pOwner->SkillFireExplicit( m_RunExtraAttack,
			GetName(),
			m_dwWeaponBaseIndex+m_iCurBlazeCnt,
			vPos );

		m_iCurBlazeCnt++;
		m_dwExtraAttackCheckTime = dwCurTime + m_dwExtraAttackDuration;
	}
}


void ioRunningAttackSkill10::ApplyExtraSkillInfo( ioBaseChar *pChar, ioPlayStage *pStage, SP2Packet &rkPacket )
{
	int iType;
	rkPacket >> iType;

	if( iType == SST_ROTATE )
	{
		D3DXQUATERNION qtRot;
		rkPacket >> qtRot;

		pChar->SetTargetRotToRotate( qtRot, true, false );

		int iDir;
		rkPacket >> iDir;
		m_CurDirKey = (ioUserKeyInput::DirKeyInput)iDir;

		rkPacket >> m_bSetUpKey;
		rkPacket >> m_bSetDownKey;

		m_bTargetRot = false;
	}
	else if( iType == SST_RUN )
	{
		int iRunEndType, iSkillEndJumpType;

		rkPacket >> iRunEndType;
		rkPacket >> iSkillEndJumpType;

		m_JumpEndType = (SkillEndJumpType)iSkillEndJumpType;

		m_dwRunningEndTime = 0;
		CheckSkillEnd( pChar, (RunEndType)iRunEndType );
	}
}

void ioRunningAttackSkill10::SetCreateWeaponDependency( ioBaseChar *pOwner, ioPlayStage *pStage )
{
	if( !pOwner ) return;
	if( !pStage ) return;

	if( m_RunningState == RST_RUN )
	{
		m_dwRunningEndTime = 0;
		m_dwRunningEndEndTime = FRAMEGETTIME();

		switch( m_RunEndType )
		{
		case RET_NORMAL_END:
			m_RunningState = RST_END;
			break;
		case RET_FORCE_END:
			m_RunningState = RST_FORCE_END;
			break;
		case RET_STUN_END:
			m_RunningState = RST_STUN_END;
			break;
		}
	}
	return;
}

SkillEndJumpType ioRunningAttackSkill10::GetSkillEndNextJumpMustEnable()
{
	return m_JumpEndType;
}

bool ioRunningAttackSkill10::CheckControlWeapon( ioBaseChar *pChar, DWORD dwCurTime )
{
	if( dwCurTime < m_dwFireStartTime )
		return true;

	ioWeapon *pWeapon = g_WeaponMgr.FindWeapon( pChar, m_dwWeaponBaseIndex );
	if( ToZoneEffectWeapon( pWeapon ) )
		return true;
	if( ToAdhesiveMissileWeapon4( pWeapon ) )
		return true;

	return false;
}

bool ioRunningAttackSkill10::IsNoDropState() const
{
	if( 0 < m_fSkillEndNextJumpAmt )
		return true;
	else
		return false;
}

//���� ����Ʈ �б�
void ioRunningAttackSkill10::LoadOwnerBuffList( ioINILoader &rkLoader )
{
	int iCnt = rkLoader.LoadInt_e( "owner_buff_cnt", 0 );
	m_OwnerBuffList.clear();

	if( iCnt <= 0 ) 
		return;

	m_OwnerBuffList.reserve( iCnt );

	char szTitle[MAX_PATH], szBuf[MAX_PATH];
	for( int i=0; i < iCnt; ++i )
	{
		wsprintf_e( szTitle, "owner_buff%d", i+1 );
		rkLoader.LoadString( szTitle, "", szBuf, MAX_PATH );

		m_OwnerBuffList.push_back( szBuf );
	}
}

void ioRunningAttackSkill10::SetOwnerBuffList( ioBaseChar *pOwner )
{
	if ( !pOwner )
		return;

	if( m_OwnerBuffList.empty() ) 
		return;

	int nBuffCnt = m_OwnerBuffList.size();
	for( int i=0; i < nBuffCnt; ++i )
	{
		ioHashString szBuffName = m_OwnerBuffList[i];
		if( !szBuffName.IsEmpty() )
			pOwner->AddNewBuff( szBuffName, pOwner->GetCharName(), "", NULL );
	}
}

void ioRunningAttackSkill10::RemoveOwnerBuffList( ioBaseChar *pOwner )
{
	if ( !pOwner )
		return;

	if( m_OwnerBuffList.empty() ) 
		return;

	int nBuffCnt = m_OwnerBuffList.size();
	for( int i=0; i < nBuffCnt; ++i )
	{
		ioBuff *pBuff = pOwner->GetBuff( m_OwnerBuffList[i], pOwner->GetCharName() );
		if ( pBuff )
			pBuff->SetReserveEndBuff();
	}
}