
#include "stdafx.h"

#include "ioCircleMove6Skill.h"

#include "ioBaseChar.h"
#include "ioPlayStage.h"

#include "ioGrowthUpInfo.h"

ioCircleMove6Skill::ioCircleMove6Skill()
{
	m_bSetHandMesh = false;
	m_bCreateWeapon = false;
	m_dwFireStartTime = 0;
	m_dwCheckLoopTime = 0;
	m_dwCurSkillProtectTime = 0;
	m_SkillState = SS_NONE;
	m_dwGuideEffectID = -1;
}

ioCircleMove6Skill::ioCircleMove6Skill( const ioCircleMove6Skill &rhs )
	: ioNormalSkill( rhs ),
	  m_fCircleRange( rhs.m_fCircleRange ),
	  m_fCircleMoveSpeed( rhs.m_fCircleMoveSpeed ),
	  m_fCircleHeightGap( rhs.m_fCircleHeightGap ),
	  m_fCircleOffSet( rhs.m_fCircleOffSet ),
	  m_ReadyCircle( rhs.m_ReadyCircle ),
	  m_RedEffect( rhs.m_RedEffect ),
	  m_BlueEffect( rhs.m_BlueEffect ),
	  m_RedGuidCircle( rhs.m_RedGuidCircle ),
	  m_BlueGuidCircle( rhs.m_BlueGuidCircle ),
	  m_vEffectOffset( rhs.m_vEffectOffset ),
	  m_dwSkillProtectTime( rhs.m_dwSkillProtectTime ),
	  m_nFireWeaponNum( rhs.m_nFireWeaponNum ),
	  m_nFireWeaponResist( rhs.m_nFireWeaponResist ),
	  m_dwLoopDuration( rhs.m_dwLoopDuration ),
	  m_szLoopAni( rhs.m_szLoopAni ),
	  m_GuideEffect( rhs.m_GuideEffect ),
	  m_bEnableTeamOnlyVisible( rhs.m_bEnableTeamOnlyVisible )
{	
	m_bSetHandMesh = false;
	m_bCreateWeapon = false;
	m_dwFireStartTime = 0;
	m_dwCheckLoopTime = 0;
	m_dwCurSkillProtectTime = 0;
	m_SkillState = SS_NONE;
	m_dwGuideEffectID = -1;
}

ioCircleMove6Skill::~ioCircleMove6Skill()
{
}

void ioCircleMove6Skill::LoadProperty( ioINILoader &rkLoader )
{
	ioNormalSkill::LoadProperty( rkLoader );

	char szBuf[MAX_PATH];
	//무적시간 설정
	m_dwSkillProtectTime    = (DWORD)rkLoader.LoadInt_e( "skill_protect_time", 0 );

	//루프 설정
	rkLoader.LoadString_e( "loop_ani", "", szBuf, MAX_PATH );
	m_szLoopAni = szBuf;
	//루프 시간 설정
	m_dwLoopDuration = rkLoader.LoadInt_e( "loop_duration", 0 );

	//무기 설정
	m_nFireWeaponNum = rkLoader.LoadInt_e( "fire_attack_type", NO_WEAPON_ATTRIBUTE );
	m_nFireWeaponResist = rkLoader.LoadInt_e( "fire_attack_type_resist", NO_WEAPON_ATTRIBUTE );

	//마법진 설정
	m_fCircleRange = rkLoader.LoadFloat_e( "magic_circle_range", 0.0f );
	m_fCircleMoveSpeed = rkLoader.LoadFloat_e( "magic_circle_move_speed", 0.0f );
	m_fCircleHeightGap = rkLoader.LoadFloat_e( "magic_circle_height_gap", 0.0f );
	m_fCircleOffSet = rkLoader.LoadFloat_e( "magic_circle_offset", 0.0f );
	
	rkLoader.LoadString_e( "ready_circle", "", szBuf, MAX_PATH );
	m_ReadyCircle = szBuf;
	
	rkLoader.LoadString_e( "red_guid_circle", "", szBuf, MAX_PATH );
	m_RedGuidCircle = szBuf;
	rkLoader.LoadString_e( "blue_guid_circle", "", szBuf, MAX_PATH );
	m_BlueGuidCircle = szBuf;

	rkLoader.LoadString_e( "red_team_effect", "", szBuf, MAX_PATH );
	m_RedEffect = szBuf;
	rkLoader.LoadString_e( "blue_team_effect", "", szBuf, MAX_PATH );
	m_BlueEffect = szBuf;

	m_vEffectOffset.x = rkLoader.LoadFloat_e( "effect_offset_x", 0.0f );
	m_vEffectOffset.y = rkLoader.LoadFloat_e( "effect_offset_y", 0.0f );
	m_vEffectOffset.z = rkLoader.LoadFloat_e( "effect_offset_z", 0.0f );

	rkLoader.LoadString_e( "extra_guid_effect", "", szBuf, MAX_PATH );
	m_GuideEffect = szBuf;

	m_bEnableTeamOnlyVisible = rkLoader.LoadBool_e( "enable_effect_only_team_visible", false );
}

ioSkill* ioCircleMove6Skill::Clone()
{
	return new ioCircleMove6Skill( *this );
}

bool ioCircleMove6Skill::OnSkillStart( ioBaseChar *pChar )
{	
	if ( !ioNormalSkill::OnSkillStart( pChar ) )
		return false;

	m_bSetHandMesh = false;
	m_bCreateWeapon = false;
	m_dwFireStartTime = 0;
	m_dwCheckLoopTime = 0;
	m_dwCurSkillProtectTime = 0;
	m_SkillState = SS_NONE;

	if( m_dwSkillProtectTime > 0 )
		m_dwCurSkillProtectTime = FRAMEGETTIME() + m_dwSkillProtectTime;

	SetCameraBuff( pChar );

	//기본 게이지 감소 (마법진 게이지)
	pChar->ReduceNeedGaugeBySkill( this, GetNeedGauge(pChar->GetCharName()) );
	m_bReduceGauge = true;	
	
	// 기본 무기 손에서 제거
	if( m_bUseItemMesh )
	{
		pChar->ShowWeaponItemMesh( false );
	}
	//무기 변경
	if( m_bUseItemMesh && !m_bSetHandMesh && !m_HandMesh.IsEmpty() )
	{
		pChar->ChangeSkillWeaponMesh( this, m_HandMesh, m_bVisibleEquipMesh );
		m_bSetHandMesh = true;
	}

	return true;
}

void ioCircleMove6Skill::OnProcessState( ioBaseChar *pChar, ioPlayStage *pStage )
{
	DWORD dwCurTime = FRAMEGETTIME();
	
	switch( m_SkillState )
	{
	case SS_NONE:
		{
			DWORD dwPreDelay = GetPreDelayTime();
			if( m_dwPreDelayStartTime + dwPreDelay < dwCurTime )
			{
				m_dwPreDelayStartTime = 0;
				EndPreDelayEffect( pChar );

				//루프 상태로 전환
				SetLoopState( pChar );
			}
		}
		break;
	case SS_LOOP:
		{
			//마법진 갱신
			UpdateMagicCircle( pChar );
			//마법진 방향으로 회전
			CheckRotate( pChar );
			//키입력
			CheckInputKey( pChar );

			//루프 종료
			if ( m_dwCheckLoopTime + m_dwLoopDuration < dwCurTime )
			{
				SetEndState( pChar );
			}
		}
		break;
	case SS_ACTION:
		{
			//무기 생성
			CreateWeapon( pChar );

			//모션 종료
			if ( m_dwMotionEndTime < dwCurTime )
			{
				SetEndState( pChar );
			}
		}
		break;
	case SS_WAIT:
		break;
	}
}

void ioCircleMove6Skill::OnSkillEnd( ioBaseChar *pChar )
{
	ioNormalSkill::OnSkillEnd( pChar );

	EndCameraBuff( pChar );

	pChar->SetMagicCircleTarget( "" );
	pChar->DestroyMagicCircle();

	if( m_dwGuideEffectID != -1 )
	{
		pChar->EndEffect( m_GuideEffect, false );
		m_dwGuideEffectID = -1;
	}

	// 기본무기 장착
	if( m_bUseItemMesh )
	{
		pChar->ShowWeaponItemMesh( true );
	}

	// 스킬 중간 동작에 사용된 메쉬들 제거
	if( m_bSetHandMesh && !m_HandMesh.IsEmpty() )
	{
		pChar->RestoreSkillWeaponMesh( this, m_HandMesh, m_bVisibleEquipMesh );
		m_bSetHandMesh = false;
	}
}

bool ioCircleMove6Skill::IsProtected( int iDefenseBreakType ) const
{
	if( m_bDisableSkillProtect )
		return false;

	if( DBT_NONE != (DefenseBreakType)m_ProtectCancelType )
	{
		if( iDefenseBreakType == m_ProtectCancelType )   // 스킬 무적 파괴
			return false;
	}

	if( IsPreDelayProtect() )
	{
		if( m_dwPreDelayStartTime != 0 )
			return true;
	}

	DWORD dwCurTime = FRAMEGETTIME();
	if( m_dwCurSkillProtectTime > dwCurTime )
		return true;
	
	return false;
}

bool ioCircleMove6Skill::IsEndState() const
{
	if( m_SkillState == SS_END )
		return true;

	return false;
}

bool ioCircleMove6Skill::IsSkillMotionEnd( DWORD dwActionStop, ioBaseChar* pOwner ) const
{
	if( m_SkillState == SS_END )
		return true;

	return false;
}

bool ioCircleMove6Skill::CheckUseItemMesh()
{
	return m_bUseItemMesh;
}

void ioCircleMove6Skill::ApplyAttributeToWeapon( ioWeapon *pWeapon, ioPlayStage *pStage )
{
	ioSkill::ApplyAttributeToWeapon( pWeapon, pStage );
}

void ioCircleMove6Skill::ApplyExtraSkillInfo( ioBaseChar *pChar, ioPlayStage *pStage, SP2Packet &rkPacket )
{
	int nState;
	rkPacket >> nState;

	switch( (SkillState)nState )
	{
	case SS_ACTION:
		{
			rkPacket >> m_vCirclePos;

			CreateEffect( pChar );
			SetNormalSkill( pChar );
			
			m_SkillState = SS_WAIT;
		}
		break;
	case SS_CREATE_WEAPON:
		{
			DWORD dwWeaponBase;
			rkPacket >> dwWeaponBase;

			//무기 생성
			FireTime kFireTime;
			kFireTime.dwStart = FRAMEGETTIME();

			kFireTime.iAttributeIdx = m_nFireWeaponNum;
			kFireTime.iResistanceIdx = 0;
			kFireTime.szWoundedAni  = "";
			kFireTime.dwWoundedAniDuration = 0;
			kFireTime.bLoopWoundedAni = false;

			kFireTime.dwWeaponIdx = dwWeaponBase;
			kFireTime.eFireTimeType = FTT_SKILL_ATTACK;

			D3DXVECTOR3 vPos = m_vCirclePos;
			D3DXVECTOR3 vDir = ioMath::VEC3_ZERO;
			pChar->ExtendFireExplicit( kFireTime, vPos, vDir, "" );
		}
		break;
	case SS_END:
		{
			m_SkillState = SS_END;
		}
		break;
	}
}

void ioCircleMove6Skill::SetNormalSkill( ioBaseChar *pChar )
{
	if ( !pChar )
		return;

	ioEntityGroup *pGrp = pChar->GetGroup();
	ioHashString szSkillMotion = GetSkillMotionName();

	int iCurSkillAniID = pGrp->GetAnimationIdx( szSkillMotion );
	if( iCurSkillAniID == -1 )	
		return;

	float fAniRate = GetSkillMotionRate();
	float fTimeRate = fAniRate * m_fExtraAniRate;
	
	pGrp->ClearAllLoopAni( FLOAT100, true );
	pGrp->ClearAllActionAni( FLOAT100, true );
	pGrp->SetActionAni( iCurSkillAniID, FLOAT100, FLOAT100, FLOAT1, FLOAT1/fTimeRate );
	
	DWORD dwCurTime = FRAMEGETTIME();
	m_dwMotionEndTime = dwCurTime + pGrp->GetAnimationFullTime( iCurSkillAniID ) * fTimeRate;
	m_dwMotionEndTime += m_dwAnimationEndDelay;

	m_dwFireStartTime = dwCurTime;
	m_dwFireStartTime += pGrp->GetAnimationEventTime_e( iCurSkillAniID, "fire_s" ) * fTimeRate;

	//추가 게이지 감소
	ioItem *pItem = GetOwnerItem();
	if( pItem )
	{
		float fUseGauge = pItem->GetMaxSkillGauge( pChar->GetCharName() );
		pChar->ReduceNeedGaugeBySkill( this, fUseGauge );
		m_bReduceGauge = true;
	}
}

//성장 범위 얻기
float ioCircleMove6Skill::GetGrowthCircleRange( ioBaseChar *pChar )
{
	float fValue = 0.0f;

	if ( !pChar )
		return fValue;

	ioPlayStage *pStage = pChar->GetCreator();
	if ( !pStage )
		return fValue;

	ioGrowthUpInfo* pGrowthInfo = GetGrowthUpInfoByType( GT_MAGIC_CIRCLE_RANGE, pChar->GetCharName() );
	if ( pGrowthInfo )
	{
		ioGrowthNormalUpInfo *pUpInfo = ToNormalUpInfo( pGrowthInfo );
		if( pUpInfo )
			fValue = pUpInfo->GetValue(pStage);
	}

	return fValue;
}

//루프 상태 전환
void ioCircleMove6Skill::SetLoopState( ioBaseChar *pChar )
{
	if ( !pChar )
		return;

	ioEntityGroup *pGrp = pChar->GetGroup();
	int iAniID = pGrp->GetAnimationIdx( m_szLoopAni );
	if( iAniID == -1 )	
		return;
	
	pGrp->ClearAllLoopAni( FLOAT100, true );
	pGrp->ClearAllActionAni( FLOAT100, true );
	pGrp->SetLoopAni( iAniID, FLOAT100 );	

	//마법진 생성
	CreateMagicCircle( pChar );
	//가이드 이펙트 생성
	CreateGuideEffect( pChar );

	DWORD dwCurTime = FRAMEGETTIME();
	m_dwCheckLoopTime = dwCurTime;
	m_SkillState = SS_LOOP;	
}

//마법진 생성
void ioCircleMove6Skill::CreateMagicCircle( ioBaseChar *pChar )
{
	if ( !pChar )
		return;

	if( !pChar->CheckMagicCircle() )
	{
		// 팀 체크
		if( pChar->GetTeam() == TEAM_RED )
			pChar->CreateMagicCircle( m_ReadyCircle, m_ReadyCircle, m_RedGuidCircle, m_fCircleOffSet );
		else if( pChar->GetTeam() == TEAM_BLUE )
			pChar->CreateMagicCircle( m_ReadyCircle, m_ReadyCircle, m_BlueGuidCircle, m_fCircleOffSet );
		else
		{
			if( pChar->GetSingleConvertTeam() == TEAM_BLUE )			
				pChar->CreateMagicCircle( m_ReadyCircle, m_ReadyCircle, m_BlueGuidCircle, m_fCircleOffSet );
			else
				pChar->CreateMagicCircle( m_ReadyCircle, m_ReadyCircle, m_RedGuidCircle, m_fCircleOffSet );
		}
	}
}

//마법진 업데이트
void ioCircleMove6Skill::UpdateMagicCircle( ioBaseChar *pChar )
{
	if ( !pChar )
		return;
		
	if( pChar->CheckMagicCircle() )
	{
		float fCurRange = m_fCircleRange + GetGrowthCircleRange( pChar );

		//뱡향 체크
		pChar->CheckChangeDirectionForMagicCircle();
		//위치 갱신
		pChar->UpdateMagicCirclePosAll( m_fCircleMoveSpeed, fCurRange, m_fCircleHeightGap, false );
	}
}

//마법진 방향으로 회전
void ioCircleMove6Skill::CheckRotate( ioBaseChar *pChar )
{
	if ( !pChar )
		return;

	//마법진 방향으로 캐릭터 회전
	D3DXVECTOR3 vTargetPos = pChar->GetMagicCirclePos();
	pChar->SetTargetRotToTargetPos( vTargetPos, true, false );
}

//키입력 확인
void ioCircleMove6Skill::CheckInputKey( ioBaseChar *pChar )
{
	if ( !pChar || !pChar->IsNeedProcess() )
		return;	

	if( pChar->IsAttackKey() )
	{
		SetActionState( pChar );
	}
}

//공격 상태로 전환
void ioCircleMove6Skill::SetActionState( ioBaseChar *pChar )
{
	if ( !pChar || !pChar->IsNeedProcess() )
		return;

	GetMagicCirclePos( pChar );
	CreateEffect( pChar );

	if( m_dwGuideEffectID != -1 )
	{
		pChar->EndEffect( m_GuideEffect, false );
		m_dwGuideEffectID = -1;
	}

	SetNormalSkill( pChar );

	m_SkillState = SS_ACTION;

	if( pChar->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_SKILL_EXTRA_INFO );
		kPacket << pChar->GetCharName();
		kPacket << GetName();
		kPacket << SS_ACTION;
		kPacket << m_vCirclePos;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

//마법진 위치 얻기
void ioCircleMove6Skill::GetMagicCirclePos( ioBaseChar *pChar )
{
	if ( !pChar )
		return;

	if( pChar->CheckMagicCircle() )
	{
		float fCurRange = m_fCircleRange + GetGrowthCircleRange( pChar );

		pChar->CheckMagicCircleVisible( false );
		pChar->UpdateMagicCirclePos( m_fCircleMoveSpeed, fCurRange, m_fCircleHeightGap, true, true );
	}

	m_vCirclePos = pChar->GetMagicCirclePos( m_fCircleOffSet, m_bCircleNoChangeY );
}

//이펙트 생성
void ioCircleMove6Skill::CreateEffect( ioBaseChar *pChar )
{
	if ( !pChar )
		return;

	ioPlayStage *pStage = pChar->GetCreator();
	if ( !pStage )
		return;

	D3DXVECTOR3 vPos = m_vCirclePos + m_vEffectOffset;
	D3DXVECTOR3 vScale = D3DXVECTOR3( FLOAT1, FLOAT1, FLOAT1 );

	if( pChar->GetTeam() == TEAM_RED )
		pStage->CreateMapEffect( m_RedEffect, vPos, vScale );
	else if( pChar->GetTeam() == TEAM_BLUE )
		pStage->CreateMapEffect( m_BlueEffect, vPos, vScale );
	else
	{
		if( pChar->GetSingleConvertTeam() == TEAM_BLUE )
			pStage->CreateMapEffect( m_BlueEffect, vPos, vScale );
		else
			pStage->CreateMapEffect( m_RedEffect, vPos, vScale );
	}
}

//무기 생성
void ioCircleMove6Skill::CreateWeapon( ioBaseChar *pChar )
{	
	if ( !pChar || !pChar->IsNeedProcess() || m_bCreateWeapon )
		return;

	DWORD dwCurTime = FRAMEGETTIME();
	if ( m_dwFireStartTime > dwCurTime )
		return;

	DWORD dwWeaponBase = pChar->IncreaseWeaponIndexBase();

	//무기 생성
	FireTime kFireTime;
	kFireTime.dwStart = dwCurTime;

	kFireTime.iAttributeIdx = m_nFireWeaponNum;
	kFireTime.iResistanceIdx = m_nFireWeaponResist;
	kFireTime.szWoundedAni  = "";
	kFireTime.dwWoundedAniDuration = 0;
	kFireTime.bLoopWoundedAni = false;

	kFireTime.dwWeaponIdx = dwWeaponBase;
	kFireTime.eFireTimeType = FTT_SKILL_ATTACK;

	D3DXVECTOR3 vPos = m_vCirclePos;
	D3DXVECTOR3 vDir = ioMath::VEC3_ZERO;
	pChar->ExtendFireExplicit( kFireTime, vPos, vDir, "" );

	m_bCreateWeapon = true;

	//패킷 전송
	if( pChar->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_SKILL_EXTRA_INFO );
		kPacket << pChar->GetCharName();
		kPacket << GetName();
		kPacket << SS_CREATE_WEAPON;
		kPacket << dwWeaponBase;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

//종료 상태 설정
void ioCircleMove6Skill::SetEndState( ioBaseChar *pChar )
{
	if ( !pChar || !pChar->IsNeedProcess() )
		return;

	m_SkillState = SS_END;
	
	//패킷 전송
	if( pChar->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_SKILL_EXTRA_INFO );
		kPacket << pChar->GetCharName();
		kPacket << GetName();
		kPacket << SS_END;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

void ioCircleMove6Skill::CreateGuideEffect( ioBaseChar *pChar )
{
	if ( !pChar )
		return;

	ioPlayStage *pStage = pChar->GetCreator();
	if ( !pStage )
		return;

	bool bVisible = false;
	if( m_bEnableTeamOnlyVisible )
	{
		ioBaseChar* pOwnerChar = pStage->GetOwnerChar();		
		if( pOwnerChar && pChar->GetTeam() == pOwnerChar->GetTeam() )
		{
			bVisible = true;
		}
	}
	else if( pChar->IsNeedProcess() )
	{
		bVisible = true;
	}
	
	ioEffect *pEffect = pChar->AttachEffect( m_GuideEffect );
	if( pEffect )
	{
		pEffect->SetVisible( bVisible );
		m_dwGuideEffectID = pEffect->GetUniqueID();		
	}
}