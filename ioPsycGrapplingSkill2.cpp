

#include "stdafx.h"

#include "ioNpcChar.h"
#include "ioPlayStage.h"

#include "ioPsycGrapplingSkill2.h"

ioPsycGrapplingSkill2::ioPsycGrapplingSkill2()
{
	m_SkillState = SS_NONE;
	m_SkillEndActionKey = SEK_NONE;

	m_dwSkillLoopStartTime = 0;
	m_dwSkillEndActionFireTime = 0;

	m_bSetHandMesh = false;

	m_dwWeaponIndex = 0;
}

ioPsycGrapplingSkill2::ioPsycGrapplingSkill2( const ioPsycGrapplingSkill2 &rhs )
	: ioAttackSkill( rhs ),
	m_SkillLoopMotion( rhs.m_SkillLoopMotion ),
	m_fSkillLoopMotionRate( rhs.m_fSkillLoopMotionRate ),
	m_SkillPullMotion( rhs.m_SkillPullMotion ),
	m_fSkillPullMotionRate( rhs.m_fSkillPullMotionRate ),
	m_SkillBoomMotion( rhs.m_SkillBoomMotion ),
	m_fSkillBoomMotionRate( rhs.m_fSkillBoomMotionRate ),
	m_SkillPushMotion( rhs.m_SkillPushMotion ),
	m_fSkillPushMotionRate( rhs.m_fSkillPushMotionRate ),
	m_SkillEndMotion( rhs.m_SkillEndMotion ),
	m_fSkillEndMotionRate( rhs.m_fSkillEndMotionRate ),
	m_dwSkillLoopDuration( rhs.m_dwSkillLoopDuration ),
	m_dwSkillLoopProtectTime( rhs.m_dwSkillLoopProtectTime ),
	m_dwEnableActionTime( rhs.m_dwEnableActionTime ),
	m_PsycWoundInfo( rhs.m_PsycWoundInfo ),
	m_vPullForceInfoList( rhs.m_vPullForceInfoList ),
	m_vBoomForceInfoList( rhs.m_vBoomForceInfoList ),
	m_vPushForceInfoList( rhs.m_vPushForceInfoList ),
	m_stCheckBuff( rhs.m_stCheckBuff )
{
	m_SkillState = SS_NONE;
	m_SkillEndActionKey = SEK_NONE;

	m_dwSkillLoopStartTime = 0;
	m_dwSkillEndActionFireTime = 0;

	m_bSetHandMesh = false;

	m_dwWeaponIndex = 0;
}

ioPsycGrapplingSkill2::~ioPsycGrapplingSkill2()
{
}

void ioPsycGrapplingSkill2::LoadProperty( ioINILoader &rkLoader )
{
	ioAttackSkill::LoadProperty( rkLoader );

	char szBuf[MAX_PATH] = "";
	char szKey[MAX_PATH] = "";

	rkLoader.LoadString_e( "skill_loop_motion", "", szBuf, MAX_PATH );
	m_SkillLoopMotion = szBuf;

	m_fSkillLoopMotionRate = rkLoader.LoadFloat_e( "skill_loop_motion_rate", 1.0f );
	m_dwSkillLoopDuration = rkLoader.LoadInt_e( "skill_loop_duration", 0 );

	m_dwEnableActionTime = rkLoader.LoadInt_e( "enable_action_time", 0 );

	rkLoader.LoadString_e( "skill_pull_motion", "", szBuf, MAX_PATH );
	m_SkillPullMotion = szBuf;

	m_fSkillPullMotionRate = rkLoader.LoadFloat_e( "skill_pull_motion_rate", 1.0f );

	rkLoader.LoadString_e( "skill_boom_motion", "", szBuf, MAX_PATH );
	m_SkillBoomMotion = szBuf;

	m_fSkillBoomMotionRate = rkLoader.LoadFloat_e( "skill_boom_motion_rate", 1.0f );

	rkLoader.LoadString_e( "skill_push_motion", "", szBuf, MAX_PATH );
	m_SkillPushMotion = szBuf;

	m_fSkillPushMotionRate = rkLoader.LoadFloat_e( "skill_push_motion_rate", 1.0f );

	rkLoader.LoadString_e( "attack_skill_end_motion", "", szBuf, MAX_PATH );
	m_SkillEndMotion = szBuf;

	m_fSkillEndMotionRate = rkLoader.LoadFloat_e( "attack_skill_end_motion_rate", 1.0f );

	m_dwSkillLoopProtectTime = rkLoader.LoadInt_e( "skill_loop_protect_time", 0 );

	//
	m_vPullForceInfoList.clear();
	m_vBoomForceInfoList.clear();
	m_vPushForceInfoList.clear();

	int i;
	int iForceCnt;

	iForceCnt = rkLoader.LoadInt_e( "pull_motion_force_cnt", 0 );
	if( iForceCnt > 0 )
	{
		m_vPullForceInfoList.reserve( iForceCnt );
	}

	for( i=0; i < iForceCnt; ++i )
	{
		ForceInfo kInfo;

		wsprintf_e( szBuf, "pull_motion_force_move%d", i+1 );
		kInfo.m_fForceMovePower = rkLoader.LoadFloat( szBuf, 0.0f );

		wsprintf_e( szBuf, "pull_motion_force_friction%d", i+1 );
		kInfo.m_fForceMoveFriction = rkLoader.LoadFloat( szBuf, 1.0f );

		wsprintf_e( szBuf, "pull_motion_force_angle%d", i+1 );
		kInfo.m_fForceAngle = rkLoader.LoadFloat( szBuf, 0.0f );

		m_vPullForceInfoList.push_back( kInfo );
	}

	iForceCnt = rkLoader.LoadInt_e( "boom_motion_force_cnt", 0 );
	if( iForceCnt > 0 )
	{
		m_vBoomForceInfoList.reserve( iForceCnt );
	}

	for( i=0; i < iForceCnt; ++i )
	{
		ForceInfo kInfo;

		wsprintf_e( szBuf, "boom_motion_force_move%d", i+1 );
		kInfo.m_fForceMovePower = rkLoader.LoadFloat( szBuf, 0.0f );

		wsprintf_e( szBuf, "boom_motion_force_friction%d", i+1 );
		kInfo.m_fForceMoveFriction = rkLoader.LoadFloat( szBuf, 1.0f );

		wsprintf_e( szBuf, "boom_motion_force_angle%d", i+1 );
		kInfo.m_fForceAngle = rkLoader.LoadFloat( szBuf, 0.0f );

		m_vBoomForceInfoList.push_back( kInfo );
	}

	iForceCnt = rkLoader.LoadInt_e( "push_motion_force_cnt", 0 );
	if( iForceCnt > 0 )
	{
		m_vPushForceInfoList.reserve( iForceCnt );
	}

	for( i=0; i < iForceCnt; ++i )
	{
		ForceInfo kInfo;

		wsprintf_e( szBuf, "push_motion_force_move%d", i+1 );
		kInfo.m_fForceMovePower = rkLoader.LoadFloat( szBuf, 0.0f );

		wsprintf_e( szBuf, "push_motion_force_friction%d", i+1 );
		kInfo.m_fForceMoveFriction = rkLoader.LoadFloat( szBuf, 1.0f );

		wsprintf_e( szBuf, "push_motion_force_angle%d", i+1 );
		kInfo.m_fForceAngle = rkLoader.LoadFloat( szBuf, 0.0f );

		m_vPushForceInfoList.push_back( kInfo );
	}

	//
	m_PsycWoundInfo.Init();

	int iBuffCnt = rkLoader.LoadInt_e( "start_buff_cnt", 0 );
	for( i=0; i < iBuffCnt; ++i )
	{
		wsprintf_e( szKey, "start_buff%d", i+1 );
		rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );

		m_PsycWoundInfo.m_StartBuffList.push_back( szBuf );
	}

	iBuffCnt = rkLoader.LoadInt_e( "pull_buff_cnt", 0 );
	for( i=0; i < iBuffCnt; ++i )
	{
		wsprintf_e( szKey, "pull_buff%d", i+1 );
		rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );

		m_PsycWoundInfo.m_PullBuffList.push_back( szBuf );
	}

	iBuffCnt = rkLoader.LoadInt_e( "boom_buff_cnt", 0 );
	for( i=0; i < iBuffCnt; ++i )
	{
		wsprintf_e( szKey, "boom_buff%d", i+1 );
		rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );

		m_PsycWoundInfo.m_BoomBuffList.push_back( szBuf );
	}

	iBuffCnt = rkLoader.LoadInt_e( "push_buff_cnt", 0 );
	for( i=0; i < iBuffCnt; ++i )
	{
		wsprintf_e( szKey, "push_buff%d", i+1 );
		rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );

		m_PsycWoundInfo.m_PushBuffList.push_back( szBuf );
	}

	//////////////////////////////////////////////////////////////////////////
	rkLoader.LoadString( "check_buff", "", szBuf, MAX_PATH );
	m_stCheckBuff = szBuf;

}

ioSkill* ioPsycGrapplingSkill2::Clone()
{
	return new ioPsycGrapplingSkill2( *this );
}

bool ioPsycGrapplingSkill2::OnSkillStart( ioBaseChar *pChar )
{
	m_SkillState = SS_NONE;
	m_SkillEndActionKey = SEK_NONE;
	m_dwSkillEndActionFireTime = 0;

	m_dwWeaponIndex = 0;

	if( !ioAttackSkill::OnSkillStart( pChar ) )
		return false;

	m_bSetHandMesh = false;
	m_vGrapplingTargets.clear();

	// 기본 무기 손에서 제거
	if( m_bUseItemMesh )
	{
		pChar->ShowWeaponItemMesh( false );
	}

	return true;
}

void ioPsycGrapplingSkill2::OnProcessState( ioBaseChar *pChar, ioPlayStage *pStage )
{
	DWORD dwCurTime = FRAMEGETTIME();
	int iSlot = GetOwnerItem()->GetType() - 1;

	switch( m_SkillState )
	{
	case SS_NONE:
		{
			DWORD dwPreDelay = GetPreDelayTime();
			if( m_dwPreDelayStartTime + dwPreDelay < dwCurTime )
			{
				m_dwPreDelayStartTime = 0;

				AttackSkillFire( pChar );
				EndPreDelayEffect( pChar );
			}
		}
		break;
	case SS_ACTION:
		{
			if( m_dwMotionEndTime < dwCurTime )
				SetSkillLoopState( pChar );
		}

		break;
	case SS_LOOP:
		{
			if( pChar->IsNeedProcess() && m_dwSkillLoopStartTime+g_App.GetGrapplingCheckDelay() < dwCurTime && m_vGrapplingTargets.size() == 0 )
			{
				SetSkillEndState( pChar, SEK_NONE, true );
				return;
			}

			// 시간만료 체크
			if( pChar->IsNeedProcess() && m_dwSkillLoopStartTime+m_dwSkillLoopDuration < dwCurTime )
			{
				SetSkillEndState( pChar, SEK_NONE, true );
				return;
			}

			// 액션키 체크
			if( pChar->IsNeedProcess() && m_dwSkillLoopStartTime+m_dwEnableActionTime < dwCurTime )
			{
				SkillEndKey eKey = SEK_NONE;

				if( pChar->IsAttackKey() )
					eKey = SEK_ATTACK;
				else if( pChar->IsDefenseKey() )
					eKey = SEK_DEFENSE;
				else if( pChar->IsJumpKey() )
					eKey = SEK_JUMP;

				if( eKey != SEK_NONE )
				{
					SetSkillEndState( pChar, eKey, true );
				}
				return;
			}
		}
		break;
	case SS_END:
		if( m_dwSkillEndActionFireTime > 0 && m_dwSkillEndActionFireTime < dwCurTime )
		{
			m_dwSkillEndActionFireTime = 0;

			for( int i = 0; i < (int)m_vGrapplingTargets.size(); i++ )
			{
				ioHashString stTarget = m_vGrapplingTargets[i];
				ioBaseChar *pTarget = pStage->GetBaseChar( stTarget );
				if( pTarget )
				{
					bool bAction = false;

					bool bNPC = false;
					if( ToNpcChar( pTarget ) )
						bNPC = true;

					if( !P2PNetwork::IsNetworkPlaying() || !pChar->GetCreator()->IsNetworkMode() || bNPC )
						bAction = true;
					else if( pTarget->IsNeedProcess() )
						bAction = true;

					if( bAction )//&& pTarget->GetPsycGrapplingAttacker() == pChar->GetCharName() )
					{
						pTarget->SetPsycGrapplingAction( pChar->GetCharName(),
							GetName(),
							GetPsycGrapplingInfo(),
							(int)m_SkillEndActionKey );
					}
				}
			}
		}
		break;
	}
}

void ioPsycGrapplingSkill2::OnSkillEnd( ioBaseChar *pChar )
{
	ioAttackSkill::OnSkillEnd( pChar );

	// 기본무기 장착
	if( m_bUseItemMesh )
	{
		pChar->ShowWeaponItemMesh( true );
	}

	ioWeapon* pWeapon = g_WeaponMgr.FindWeapon( pChar, m_dwWeaponIndex );
	if( pWeapon )
		pWeapon->SetWeaponDead();

	// 스킬 중간 동작에 사용된 메쉬들 제거
	m_bSetHandMesh = false;
	if( !m_HandMesh.IsEmpty() )
		pChar->RestoreSkillWeaponMesh( this, m_HandMesh, m_bVisibleEquipMesh );


	m_SkillState = SS_NONE;
	m_SkillEndActionKey = SEK_NONE;

	m_dwSkillEndActionFireTime = 0;
}

void ioPsycGrapplingSkill2::AttackSkillFire( ioBaseChar *pChar )
{
	ioAttackSkill::AttackSkillFire( pChar );

	m_SkillState = SS_ACTION;

	if( !pChar ) return;
	ioPlayStage *pStage = pChar->GetCreator();
	if( !pStage ) return;

	m_vGrapplingTargets.clear();

	// 사용 아이템 처리
	if( !m_bSetHandMesh && m_bUseItemMesh && !m_HandMesh.IsEmpty() )
	{
		pChar->ChangeSkillWeaponMesh( this, m_HandMesh, m_bVisibleEquipMesh );
		m_bSetHandMesh = true;
	}
}

void ioPsycGrapplingSkill2::SetSkillLoopState( ioBaseChar *pChar )
{
	if( !pChar ) return;
	if( m_SkillState != SS_ACTION )
		return;

	ioEntityGroup *pGrp = pChar->GetGroup();
	if( !pGrp ) return;

	pGrp->ClearAllActionAni( 100.0f, true );
	pGrp->ClearAllLoopAni( 100.0f, true );

	float fAniRate = 1.0f;
	if( m_fSkillLoopMotionRate > 0.0f )
		fAniRate = m_fSkillLoopMotionRate;

	int iAniID = pGrp->GetAnimationIdx( m_SkillLoopMotion );
	pGrp->SetLoopAni( iAniID, 100.0f, 1.0f, 1.0f/fAniRate );

	m_SkillState = SS_LOOP;

	m_dwSkillLoopStartTime = FRAMEGETTIME();
}

void ioPsycGrapplingSkill2::SetSkillEndState( ioBaseChar *pChar, SkillEndKey eKey, bool bSendNet )
{
	if( m_SkillState != SS_LOOP )
		return;

	if( !pChar ) return;

	ioEntityGroup *pGrp = pChar->GetGroup();
	if( !pGrp ) return;

	DWORD dwCurTime = FRAMEGETTIME();

	pGrp->ClearAllActionAni( 100.0f, true );
	pGrp->ClearAllLoopAni( 100.0f, true );

	ioHashString szMotion = m_SkillEndMotion;
	float fMotionRate = m_fSkillEndMotionRate;

	ForceInfoList vForceInfoList;

	m_SkillEndActionKey = eKey;
	switch( m_SkillEndActionKey )
	{
	case SEK_ATTACK:
		szMotion = m_SkillPushMotion;
		fMotionRate = m_fSkillPushMotionRate;
		vForceInfoList = m_vPushForceInfoList;
		break;
	case SEK_DEFENSE:
		szMotion = m_SkillBoomMotion;
		fMotionRate = m_fSkillBoomMotionRate;
		vForceInfoList = m_vBoomForceInfoList;
		break;
	case SEK_JUMP:
		szMotion = m_SkillPullMotion;
		fMotionRate = m_fSkillPullMotionRate;
		vForceInfoList = m_vPullForceInfoList;
		break;
	}

	float fAniRate = 1.0f;
	if( fMotionRate > 0.0f )
		fAniRate = fMotionRate;

	int iAniID = pGrp->GetAnimationIdx( szMotion );
	pGrp->SetActionAni( iAniID, 100.0f, 100.0f, 1.0f, 1.0f/fAniRate );

	m_dwMotionStartTime = m_dwMotionEndTime = dwCurTime;
	m_dwMotionEndTime += pGrp->GetAnimationFullTime( iAniID ) * fAniRate;

	m_dwFireMotionEndTime = dwCurTime;
	m_dwFireMotionEndTime += pGrp->GetAnimationEventTime_e( iAniID, "firemotion_e" ) * fAniRate;

	// Key Reserve
	m_dwEnableReserveTime = dwCurTime;
	m_dwEnableReserveTime += pGrp->GetAnimationEventTime_e( iAniID, "key_reserve" );

	pChar->SetReservedSliding( vForceInfoList, iAniID, fAniRate, 0 );
	pChar->CheckCharColSkipSkillTime( iAniID, fAniRate );

	m_dwSkillEndActionFireTime = dwCurTime;
	float fValue = pGrp->GetAnimationEventTime_e( iAniID, "fire_s" );
	m_dwSkillEndActionFireTime += fValue;

	m_SkillState = SS_END;

	if( bSendNet && pChar->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_SKILL_EXTRA_INFO );
		kPacket << pChar->GetCharName();
		kPacket << GetName();
		kPacket << SSC_END;
		kPacket << pChar->GetTargetRot();
		kPacket << (int)m_SkillEndActionKey;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

void ioPsycGrapplingSkill2::ApplyAttributeToWeapon( ioWeapon *pWeapon, ioPlayStage *pStage )
{
	ioSkill::ApplyAttributeToWeapon( pWeapon, pStage );

	if( m_dwWeaponIndex == 0 )
		m_dwWeaponIndex = pWeapon->GetWeaponIndex();
}

bool ioPsycGrapplingSkill2::SetSkillGrapplingState( ioBaseChar *pChar, const ioHashString &szTargetName )
{
	if( !pChar ) return false;
	if( m_SkillState != SS_ACTION ) return false;

	m_vGrapplingTargets.push_back( szTargetName );

	//m_GrapplingTargetName = szTargetName;
	ioBaseChar *pTarget = pChar->GetBaseChar( szTargetName );
	if( !pTarget ) return false;

	//SetSkillLoopState( pChar );

	if( pChar->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_SKILL_EXTRA_INFO );
		kPacket << pChar->GetCharName();
		kPacket << GetName();
		kPacket << SSC_LOOP;
		kPacket << pChar->GetTargetRot();
		kPacket << szTargetName;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}

	// for NPC
	bool bNPC = false;
	if( ToNpcChar( pTarget ) )
	{
		bNPC = true;
	}

	// local or 훈련소 or NPC
	if( !P2PNetwork::IsNetworkPlaying() || !pChar->GetCreator()->IsNetworkMode() || bNPC )
	{
		pTarget->SetWoundPsycGrappling( pChar->GetCharName(), GetName(), GetPsycGrapplingInfo() );
	}

	return true;
}

void ioPsycGrapplingSkill2::ApplyExtraSkillInfo( ioBaseChar *pChar, ioPlayStage *pStage, SP2Packet &rkPacket )
{
	if( !pChar ) return;
	if( !pStage ) return;

	int iState;
	rkPacket >> iState;

	D3DXQUATERNION qtRot;
	rkPacket >> qtRot;

	pChar->SetTargetRotToRotate( qtRot, true, false );

	switch( iState )
	{
	case SSC_LOOP:
		{
			ioHashString szTargetName;
			rkPacket >> szTargetName;

			m_vGrapplingTargets.push_back( szTargetName );

			//SetSkillLoopState( pChar );

			ioBaseChar *pTarget = pStage->GetBaseChar( szTargetName );
			if( pTarget && pTarget->IsNeedProcess() )
			{
				pTarget->SetWoundPsycGrappling( pChar->GetCharName(), GetName(), GetPsycGrapplingInfo() );
			}
		}
		break;
	case SSC_END:
		{
			int iKey;
			rkPacket >> iKey;

			if( m_SkillState != SS_END )
			{
				SetSkillEndState( pChar, (SkillEndKey)iKey, false );
			}
		}
		break;
	}
}

bool ioPsycGrapplingSkill2::IsSkillMotionEnd( DWORD dwActionStop, ioBaseChar* pOwner ) const
{
	DWORD dwCurTime = FRAMEGETTIME();

	if( m_dwPreDelayStartTime > 0 )
		return false;

	switch( m_SkillState )
	{
	case SS_LOOP:
		return false;
	case SS_ACTION:
	case SS_END:
		if( m_dwMotionEndTime + dwActionStop < dwCurTime )
		{
			return true;
		}
		return false;
	}

	return false;
}

bool ioPsycGrapplingSkill2::IsProtected( int iDefenseBreakType ) const
{
	if( m_bDisableSkillProtect )
		return false;

	if( DBT_NONE != (DefenseBreakType)m_ProtectCancelType )
	{
		if( iDefenseBreakType == m_ProtectCancelType )   // 스킬 무적 파괴
			return false;
	}

	DWORD dwCurTime = FRAMEGETTIME();

	if( IsPreDelayProtect() )
	{
		if( m_dwPreDelayStartTime != 0 )
			return true;
	}

	switch( m_SkillState )
	{
	case SS_ACTION:
		return true;
	case SS_LOOP:
	case SS_END:
		if( m_dwSkillLoopStartTime+m_dwSkillLoopProtectTime < dwCurTime )
			return false;

		return true;
	}

	return false;
}

bool ioPsycGrapplingSkill2::IsEnableReserve() const
{
	if( m_dwPreDelayStartTime != 0 )
		return false;

	switch( m_SkillState )
	{
	case SS_END:
		if( m_dwEnableReserveTime == 0 || m_dwMotionStartTime >= m_dwEnableReserveTime )
			return false;

		if( m_dwEnableReserveTime < FRAMEGETTIME() )
			return true;
		break;
	}

	return false;
}

bool ioPsycGrapplingSkill2::CheckUseItemMesh()
{
	return m_bUseItemMesh;
}

PsycGrapplingWoundedInfo ioPsycGrapplingSkill2::GetPsycGrapplingInfo() const
{
	return m_PsycWoundInfo;
}



