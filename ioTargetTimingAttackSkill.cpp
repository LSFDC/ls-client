
#include "stdafx.h"

#include "ioTargetTimingAttackSkill.h"

#include "ioPlayStage.h"
#include "ioBaseChar.h"

#include "WeaponDefine.h"

ioTargetTimingAttackSkill::ioTargetTimingAttackSkill()
{
	InitData();
}

ioTargetTimingAttackSkill::ioTargetTimingAttackSkill( const ioTargetTimingAttackSkill &rhs )
	: ioAttackSkill( rhs ),
	  m_dwSkillProtectTime( rhs.m_dwSkillProtectTime ),
	  m_szAimedMarker( rhs.m_szAimedMarker ),
	  m_dwAimedTime( rhs.m_dwAimedTime ),
	  m_szZeroAimedMarker( rhs.m_szZeroAimedMarker ),
	  m_dwZeroAimedTime( rhs.m_dwZeroAimedTime ),
	  m_fAimRange( rhs.m_fAimRange ),
	  m_szAimLoopAni( rhs.m_szAimLoopAni ),
	  m_fAimLoopAniRate( rhs.m_fAimLoopAniRate ),
	  m_NormalAimedAttack( rhs.m_NormalAimedAttack ),
	  m_NormalZeroAimedAttack( rhs.m_NormalZeroAimedAttack ),
	  m_NormalZeroAimedExtraAttack( rhs.m_NormalZeroAimedExtraAttack ),
	  m_JumpAimedAttack( rhs.m_JumpAimedAttack ),
	  m_JumpZeroAimedAttack( rhs.m_JumpZeroAimedAttack ),
	  m_JumpZeroAimedExtraAttack( rhs.m_JumpZeroAimedExtraAttack ),
	  m_bEnableDownAim( rhs.m_bEnableDownAim ),
	  m_dwLoopTime( rhs.m_dwLoopTime ),
	  m_vNoTargetTeleportOffset( rhs.m_vNoTargetTeleportOffset ),
	  m_vTargetTeleportOffset( rhs.m_vTargetTeleportOffset ),
	  m_dwAttackTime( rhs.m_dwAttackTime )
{
	InitData();
}

ioTargetTimingAttackSkill::~ioTargetTimingAttackSkill()
{
}

void ioTargetTimingAttackSkill::LoadProperty( ioINILoader &rkLoader )
{
	ioAttackSkill::LoadProperty( rkLoader );
		
	char szBuf[MAX_PATH];

	//��ų ���� �ð�
	m_dwSkillProtectTime = (DWORD)rkLoader.LoadInt_e( "skill_protect_time", 0 );
	
	rkLoader.LoadString_e( "aimed_marker", "", szBuf, MAX_PATH );
	m_szAimedMarker = szBuf;
	m_dwAimedTime = rkLoader.LoadInt_e( "aimed_time", 0 );

	rkLoader.LoadString_e( "zero_aimed_marker", "", szBuf, MAX_PATH );
	m_szZeroAimedMarker = szBuf;
	m_dwZeroAimedTime = rkLoader.LoadInt_e( "zero_aimed_time", 0 );

	m_fAimRange = rkLoader.LoadFloat_e( "aim_range", 0.0f );

	rkLoader.LoadString_e( "aim_loop_ani", "", szBuf, MAX_PATH );
	m_szAimLoopAni = szBuf;

	m_fAimLoopAniRate = rkLoader.LoadFloat_e( "aim_loop_ani_rate", 1.0f );

	LoadAttackAttribute( "normal_aimed_attack", m_NormalAimedAttack, rkLoader );
	LoadAttackAttribute( "normal_zero_aimed_attack", m_NormalZeroAimedAttack, rkLoader );
	LoadAttackAttribute( "normal_zero_aimed_extra_attack", m_NormalZeroAimedExtraAttack, rkLoader );

	LoadAttackAttribute( "jump_aimed_attack", m_JumpAimedAttack, rkLoader );
	LoadAttackAttribute( "jump_zero_aimed_attack", m_JumpZeroAimedAttack, rkLoader );
	LoadAttackAttribute( "jump_zero_aimed_extra_attack", m_JumpZeroAimedExtraAttack, rkLoader );
	
	m_bEnableDownAim = rkLoader.LoadBool_e( "enable_down_aim", false );

	m_dwLoopTime = rkLoader.LoadInt_e( "loop_time", 0 );

	m_vNoTargetTeleportOffset.x = rkLoader.LoadFloat_e( "no_target_teleport_offset_x", 0.0f );
	m_vNoTargetTeleportOffset.y = rkLoader.LoadFloat_e( "no_target_teleport_offset_y", 0.0f );
	m_vNoTargetTeleportOffset.z = rkLoader.LoadFloat_e( "no_target_teleport_offset_z", 0.0f );

	m_vTargetTeleportOffset.x = rkLoader.LoadFloat_e( "target_teleport_offset_x", 0.0f );
	m_vTargetTeleportOffset.y = rkLoader.LoadFloat_e( "target_teleport_offset_y", 0.0f );
	m_vTargetTeleportOffset.z = rkLoader.LoadFloat_e( "target_teleport_offset_z", 0.0f );

	m_dwAttackTime = rkLoader.LoadInt_e( "attack_time", 0 );
}

ioSkill* ioTargetTimingAttackSkill::Clone()
{
	return new ioTargetTimingAttackSkill( *this );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

bool ioTargetTimingAttackSkill::OnSkillStart( ioBaseChar *pChar )
{	
	InitData();
	
	DWORD dwCurTime = FRAMEGETTIME();
	if( m_dwSkillProtectTime > 0 )
		m_dwCurSkillProtectTime = dwCurTime + m_dwSkillProtectTime;

	if ( m_dwLoopTime > 0)
		m_dwCheckLoopTime = dwCurTime + m_dwLoopTime;

	if( !ioAttackSkill::OnSkillStart( pChar ) )
		return false;
	
	SetCameraBuff( pChar );

	// �⺻ ���� �տ��� ����
	if( m_bUseItemMesh )
		pChar->ShowWeaponItemMesh( false );

	// ��� ������ ó��
	if( !m_bSetHandMesh && m_bUseItemMesh && !m_HandMesh.IsEmpty() )
	{
		pChar->ChangeSkillWeaponMesh( this, m_HandMesh, m_bVisibleEquipMesh );
		m_bSetHandMesh = true;
	}	
	
	if( !m_bReduceGauge )
	{
		pChar->ReduceNeedGaugeBySkill( this, GetNeedGauge( pChar->GetCharName() ) );
		m_bReduceGauge = true;
	}

	return true;
}

void ioTargetTimingAttackSkill::OnProcessState( ioBaseChar *pChar, ioPlayStage *pStage )
{
	DWORD dwCurTime = FRAMEGETTIME();

	if ( m_dwCheckLoopTime > 0 && m_dwCheckLoopTime < dwCurTime )
	{
		SetEndState( pChar );
		return;
	}

	switch( m_SkillState )
	{
	case SS_NONE:
		{
			DWORD dwPreDelay = GetPreDelayTime();
			if( m_dwPreDelayStartTime + dwPreDelay < dwCurTime )
			{
				m_dwPreDelayStartTime = 0;
				EndPreDelayEffect( pChar );
				SetAimState( pChar );
			}
		}
		break;
	case SS_AIM:
		{
			GetTargetList( pChar, pStage );
			SetTargetRotation( pChar );
			UpdateMaker( pChar, pStage );

			if ( CheckInputChangeTarget( pChar ) )
			{
				DestroyEffect( pChar, pStage, m_dwMakerID );
				return;
			}
			
			if ( CheckInputAttack( pChar ) )
			{
				SetTeleport( pChar, pStage );

				if ( m_bZeroAimed )
					SetAttackState( pChar, SS_ZERO_AIM_ATTACK );
				else
					SetAttackState( pChar, SS_AIM_ATTACK );
				return;
			}
		}
		break;
	case SS_AIM_ATTACK:
	case SS_ZERO_AIM_ATTACK:
	case SS_ZERO_AIM_EXTRA_ATTACK:
		{
			if ( m_dwMotionEndTime > 0 && m_dwMotionEndTime < dwCurTime )
			{
				if ( m_SkillState == SS_ZERO_AIM_ATTACK )
				{
					SetZeroAimedExtraAttackState( pChar );
					return;
				}
				else if ( m_SkillState == SS_AIM_ATTACK || m_SkillState == SS_ZERO_AIM_EXTRA_ATTACK )
				{
					SetEndState( pChar );
					return;
				}
			}
		}
		break;
	}
}

void ioTargetTimingAttackSkill::OnSkillEnd( ioBaseChar *pChar )
{
	pChar->ClearReservedSliding();

	ioPlayStage *pStage = pChar->GetCreator();
	if ( pStage )
	{
		DestroyEffect( pChar, pStage, m_dwMakerID );
	}

	ioAttackSkill::OnSkillEnd( pChar );

	EndCameraBuff( pChar );

	// �⺻���� ����
	if( m_bUseItemMesh )
		pChar->ShowWeaponItemMesh( true );

	if( !m_HandMesh.IsEmpty() )
		pChar->RestoreSkillWeaponMesh( this, m_HandMesh, m_bVisibleEquipMesh );

	m_bSetHandMesh = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

bool ioTargetTimingAttackSkill::IsProtected( int iDefenseBreakType ) const
{
	if( m_bDisableSkillProtect )
		return false;

	if( DBT_NONE != (DefenseBreakType)m_ProtectCancelType )
	{
		if( iDefenseBreakType == m_ProtectCancelType )   // ��ų ���� �ı�
			return false;
	}
	DWORD dwCurTime = FRAMEGETTIME();

	if( IsPreDelayProtect() )
	{
		if( m_dwPreDelayStartTime != 0 )
			return true;
	}

	if( m_dwPreDelayStartTime == 0 )
	{		
		if( m_dwCurSkillProtectTime > dwCurTime )
			return true;
	}

	if( m_dwFireMotionEndTime > 0 && m_dwFireMotionEndTime > dwCurTime )
		return true;

	return false;
}

bool ioTargetTimingAttackSkill::IsSkillMotionEnd( DWORD dwActionStop, ioBaseChar* pOwner ) const
{	
	if( m_dwPreDelayStartTime > 0 )
		return false;

	switch( m_SkillState )
	{
	case SS_AIM:
		return false;
	case SS_END:
		if( m_fSkillEndNextJumpAmt == 0.0f )
			return true;
		else
			return false;
	}

	if( m_dwMotionEndTime + dwActionStop >= FRAMEGETTIME() )
		return false;

	return true;
}

bool ioTargetTimingAttackSkill::IsAttackEndState() const
{	
	if( m_SkillState == SS_END )
		return true;

	return false;
}

bool ioTargetTimingAttackSkill::IsNoDropState() const
{	
	switch( m_SkillState )
	{
	case SS_AIM_ATTACK:
	case SS_ZERO_AIM_ATTACK:
	case SS_ZERO_AIM_EXTRA_ATTACK:
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void ioTargetTimingAttackSkill::ApplyAttributeToWeapon( ioWeapon *pWeapon, ioPlayStage *pStage )
{
	ioSkill::ApplyAttributeToWeapon( pWeapon, pStage );	

	ioZoneEffectWeapon *pZone = ToZoneEffectWeapon( pWeapon );
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
}

void ioTargetTimingAttackSkill::ApplyExtraSkillInfo( ioBaseChar *pChar, ioPlayStage *pStage, SP2Packet &rkPacket )
{
	if( !pChar || !pStage ) 
		return;

	int nState;
	rkPacket >> nState;

	switch ( nState )
	{		
	case SS_TARGET:
		{
			rkPacket >> m_szAimedTarget;
		}
		break;
	case SS_AIM_MARK:
		{
			rkPacket >> m_bZeroAimed;

			DestroyEffect( pChar, pStage, m_dwMakerID );

			D3DXVECTOR3 vPos = pChar->GetMidPositionByRate() + m_vAimDir * m_fAimRange;
			if ( !m_szAimedTarget.IsEmpty() )
			{
				ioBaseChar *pTarget = pStage->GetBaseChar( m_szAimedTarget );
				if ( pTarget )
					vPos = pTarget->GetMidPositionByRate();
			}

			if ( m_bZeroAimed )
				m_dwMakerID = CreateEffect( pChar, pStage, m_szZeroAimedMarker, vPos, D3DXVECTOR3(1.0f,1.0f,1.0f) );
			else
				m_dwMakerID = CreateEffect( pChar, pStage, m_szAimedMarker, vPos, D3DXVECTOR3(1.0f,1.0f,1.0f) );			
		}
		break;
	case SS_AIM_ATTACK:
	case SS_ZERO_AIM_ATTACK:
		{
			D3DXQUATERNION qtRot;
			rkPacket >> qtRot;

			pChar->SetTargetRotAndMoveDirChange( qtRot );

			D3DXVECTOR3 vPos;
			rkPacket >> vPos;

			pChar->SetWorldPosition( vPos );

			bool bFloating;
			rkPacket >> bFloating;

			if ( bFloating )
			{
				if ( nState == SS_AIM_ATTACK )
					m_CurAttack = m_JumpAimedAttack;
				else if ( nState == SS_ZERO_AIM_ATTACK )
					m_CurAttack = m_JumpZeroAimedAttack;
			}
			else
			{
				if ( nState == SS_AIM_ATTACK )
					m_CurAttack = m_NormalAimedAttack;
				else if ( nState == SS_ZERO_AIM_ATTACK )
					m_CurAttack = m_NormalZeroAimedAttack;
			}

			SetAttackState( pChar, (SkillState)nState );
		}
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void ioTargetTimingAttackSkill::InitData()
{
	m_SkillState = SS_NONE;	
	m_bSetHandMesh = false;
	m_dwCurSkillProtectTime = 0;
	m_dwCheckLoopTime = 0;

	m_szAimedTarget.Clear();
	m_dwMakerID = -1;
	m_dwCheckAimTime = 0;
	m_bZeroAimed = false;

	m_bChangeTargetPrev = false;
	m_bChangeTargetNext = false;

	m_vAimDir = ioMath::VEC3_ZERO;

	m_dwCheckAttackTime = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

DWORD ioTargetTimingAttackSkill::CreateEffect( ioBaseChar *pChar, ioPlayStage *pStage, ioHashString& szEffect, D3DXVECTOR3& vPos, D3DXVECTOR3& vScale )
{
	if ( !pChar || !pStage || szEffect.IsEmpty() )
		return -1;

	ioMapEffect *pMaker = pStage->CreateMapEffect( szEffect, vPos, vScale );
	if ( pMaker )
		return pMaker->GetUniqueID();

	return -1;
}

void ioTargetTimingAttackSkill::DestroyEffect( ioBaseChar *pChar, ioPlayStage *pStage, DWORD& dwEffectID )
{
	if ( !pChar || !pStage || dwEffectID == -1 )
		return;

	pStage->DestroyMapEffect( dwEffectID );
	dwEffectID = -1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void ioTargetTimingAttackSkill::GetTargetList( ioBaseChar *pChar, ioPlayStage *pStage )
{
	if ( !pChar || !pChar->IsNeedProcess() || !pStage )
		return;

	m_vTargetInfoList.clear();

	const BaseCharList &rkTargetList = pStage->GetBaseCharList();

	BaseCharList::const_iterator iter;
	for( iter = rkTargetList.begin(); iter != rkTargetList.end(); ++iter )
	{
		ioBaseChar *pTarget = (*iter);

		if( !pTarget ) 
			continue;

		if( pChar->GetCharName() == pTarget->GetCharName() )
			continue;

		if( pChar->GetTeam() == pTarget->GetTeam() )	
			continue;

		if( !m_bEnableDownAim && pTarget->IsApplyDownState() )
			continue;

		if( pTarget->IsSystemState() )
			continue;
			
		if( !pTarget->IsEnableTarget() )
			continue;

		D3DXVECTOR3 vDiff = pTarget->GetWorldPosition() - pChar->GetWorldPosition();
		if( D3DXVec3LengthSq( &vDiff ) < m_fAimRange * m_fAimRange )
		{
			BaseCharHP kInfo;
			kInfo.m_Name = pTarget->GetCharName();
			kInfo.m_fHP = pTarget->GetHP().m_fCurValue;
			kInfo.m_bPrisoner = pTarget->IsPrisonerMode();

			m_vTargetInfoList.push_back(kInfo);
		}
	}

	std::sort( m_vTargetInfoList.begin(), m_vTargetInfoList.end(), BaseCharHPSort() );	

	bool bSendPacket = false;
	if ( m_szAimedTarget.IsEmpty() )
	{
		if( !m_vTargetInfoList.empty() )
		{
			m_szAimedTarget = m_vTargetInfoList[0].m_Name;
			bSendPacket = true;
		}		
	}
	else if ( !m_vTargetInfoList.empty() )
	{
		int nCurTarget = -1;

		int nCnt = m_vTargetInfoList.size();
		for( int i=0; i<nCnt; ++i )
		{
			if( m_szAimedTarget == m_vTargetInfoList[i].m_Name )
				nCurTarget = i;
		}

		if ( nCurTarget != -1 )
		{
			if( m_bChangeTargetPrev )
			{
				nCurTarget--;
				m_bChangeTargetPrev = false;

				if( nCurTarget < 0 )
					nCurTarget = nCnt-1;
			}
			else if( m_bChangeTargetNext )
			{
				nCurTarget++;
				m_bChangeTargetNext = false;

				if( nCurTarget >= nCnt )
					nCurTarget = 0;
			}

			if( m_szAimedTarget != m_vTargetInfoList[nCurTarget].m_Name )
			{
				m_szAimedTarget = m_vTargetInfoList[nCurTarget].m_Name;
				bSendPacket = true;
			}
		}
		else if ( !m_szAimedTarget.IsEmpty() )
		{
			m_szAimedTarget.Clear();
			bSendPacket = true;
			m_bChangeTargetPrev = false;
			m_bChangeTargetNext = false;
		}

	}
	else
	{
		if ( !m_szAimedTarget.IsEmpty() )
		{
			m_szAimedTarget.Clear();
			bSendPacket = true;
			m_bChangeTargetPrev = false;
			m_bChangeTargetNext = false;
		}
	}

	if ( bSendPacket && pChar->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_SKILL_EXTRA_INFO );
		kPacket << pChar->GetCharName();
		kPacket << GetName();
		kPacket << SS_TARGET;
		kPacket << m_szAimedTarget;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

void ioTargetTimingAttackSkill::SetTargetRotation( ioBaseChar *pChar )
{
	if ( !pChar )
		return;
	
	ioEntityGroup *pGrp = pChar->GetGroup();
	if ( !pGrp )
		return;
	
	ioBaseChar *pTarget = pChar->GetBaseChar( m_szAimedTarget );
	D3DXVECTOR3 vAimedDir;
	if( pTarget )
	{
		vAimedDir = pTarget->GetMidPositionByRate() - pChar->GetMidPositionByRate();
		pChar->SetTargetRotToDir( vAimedDir, true );		
	}
	else
		vAimedDir = pChar->GetTargetRot() * D3DXVECTOR3( 0.0f, 0.0f, -1.0f );

	D3DXVec3Normalize( &vAimedDir, &vAimedDir );
	m_vAimDir = vAimedDir;
}

void ioTargetTimingAttackSkill::SetAimedMaker( ioBaseChar *pChar, ioPlayStage *pStage, D3DXVECTOR3 vPos )
{
	if ( !pChar || !pChar->IsNeedProcess() || !pStage )
		return;

	bool bSendNetwork = false;

	DWORD dwCurTime = FRAMEGETTIME();		
	if ( m_dwMakerID != -1 )
	{
		if ( m_dwCheckAimTime > 0 && m_dwCheckAimTime < dwCurTime )
		{
			bSendNetwork = true;
			DestroyEffect( pChar, pStage, m_dwMakerID );

			if ( !m_bZeroAimed )
			{
				m_dwMakerID = CreateEffect( pChar, pStage, m_szZeroAimedMarker, vPos, D3DXVECTOR3(1.0f,1.0f,1.0f) );
				m_bZeroAimed = true;
				m_dwCheckAimTime = m_dwZeroAimedTime + dwCurTime;
			}
			else
			{
				m_dwMakerID = CreateEffect( pChar, pStage, m_szAimedMarker, vPos, D3DXVECTOR3(1.0f,1.0f,1.0f) );
				m_bZeroAimed = false;
				m_dwCheckAimTime = m_dwAimedTime + dwCurTime;
			}
		}
	}
	else if ( m_dwMakerID == -1 )
	{
		bSendNetwork = true;
		m_dwMakerID = CreateEffect( pChar, pStage, m_szAimedMarker, vPos, D3DXVECTOR3(1.0f,1.0f,1.0f) );
		m_bZeroAimed = false;
		m_dwCheckAimTime = m_dwAimedTime + dwCurTime;
	}

	if ( bSendNetwork && pChar->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_SKILL_EXTRA_INFO );
		kPacket << pChar->GetCharName();
		kPacket << GetName();
		kPacket << SS_AIM_MARK;
		kPacket << m_bZeroAimed;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

void ioTargetTimingAttackSkill::UpdateMaker( ioBaseChar *pChar, ioPlayStage *pStage )
{
	if ( !pChar || !pStage )
		return;
	
	D3DXVECTOR3 vPos = pChar->GetMidPositionByRate() + m_vAimDir * m_fAimRange;

	if ( !m_szAimedTarget.IsEmpty() )
	{
		ioBaseChar *pTarget = pStage->GetBaseChar( m_szAimedTarget );
		if ( pTarget )
			vPos = pTarget->GetMidPositionByRate();
	}

	SetAimedMaker( pChar, pStage, vPos );

	ioMapEffect *pEffect = pStage->FindMapEffect( m_dwMakerID );
	if ( pEffect )		
		pEffect->SetWorldPosition( vPos );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void ioTargetTimingAttackSkill::SetAimState( ioBaseChar *pChar )
{
	if ( !pChar )
		return;

	ioEntityGroup *pGrp = pChar->GetGroup();

	int nAniID = pGrp->GetAnimationIdx( m_szAimLoopAni );
	if( nAniID == -1 )	
		return;

	pGrp->ClearAllLoopAni( 100.f, true );
	pGrp->ClearAllActionAni( 100.f, true );

	pGrp->SetLoopAni( nAniID, 100.0f, 1.0f, 1.0f/m_fAimLoopAniRate );

	m_SkillState = SS_AIM;

	m_dwCheckAttackTime = FRAMEGETTIME();
}

void ioTargetTimingAttackSkill::SetTeleport( ioBaseChar *pChar, ioPlayStage *pStage )
{
	if ( !pChar || !pStage )
		return;
	
	DestroyEffect( pChar, pStage, m_dwMakerID );

	D3DXVECTOR3 vNewPos = pChar->GetWorldPosition() + pChar->GetWorldOrientation() * m_vNoTargetTeleportOffset;
	ioBaseChar *pTarget = pChar->GetBaseChar( m_szAimedTarget );
	if( pTarget )
		vNewPos = pTarget->GetWorldPosition() + pChar->GetWorldOrientation() * m_vTargetTeleportOffset;

	float fMapHeight = pStage->GetMapHeightOnlyTerrain( vNewPos.x, vNewPos.z );
	if ( vNewPos.y <= fMapHeight )
		vNewPos.y = fMapHeight;

	pChar->SetWorldPosition( vNewPos );

	SetTargetRotation( pChar );
}

void ioTargetTimingAttackSkill::SetAttackState( ioBaseChar *pChar, SkillState eState )
{
	if ( !pChar )
		return;
	
	bool bFloating = false;
	if ( pChar->IsNeedProcess() )
	{
		if ( pChar->IsFloating() )
		{
			if ( eState == SS_AIM_ATTACK )
				m_CurAttack = m_JumpAimedAttack;
			else if ( eState == SS_ZERO_AIM_ATTACK )
				m_CurAttack = m_JumpZeroAimedAttack;
			bFloating = true;
		}
		else
		{
			if ( eState == SS_AIM_ATTACK )
				m_CurAttack = m_NormalAimedAttack;
			else if ( eState == SS_ZERO_AIM_ATTACK )
				m_CurAttack = m_NormalZeroAimedAttack;
		}
	}

	ioEntityGroup *pGrp = pChar->GetGroup();

	int nAniID = pGrp->GetAnimationIdx( m_CurAttack.m_AttackAnimation );
	if( nAniID == -1 )	
		return;

	float fTimeRate  = m_CurAttack.m_fAttackAniRate;
	DWORD dwPreDelay = m_CurAttack.m_dwPreDelay;

	pGrp->ClearAllLoopAni( 100.0f, true );
	pGrp->ClearAllActionAni( 100.0f, true );

	pGrp->SetActionAni( nAniID, 100.0f, 100.0f, 1.0f, 1.0f/fTimeRate, 0.0f, dwPreDelay );

	pChar->ClearReservedSliding();
	pChar->SetReservedSliding( m_CurAttack.m_vForceInfoList, nAniID, fTimeRate, dwPreDelay );
	pChar->CheckCharColSkipSkillTime( nAniID, fTimeRate, dwPreDelay );

	pChar->RefreshFireTimeList( nAniID, m_CurAttack.m_vWeaponInfoList, FTT_SKILL_ATTACK, fTimeRate, dwPreDelay );

	DWORD dwCurTime = FRAMEGETTIME();
	m_dwMotionEndTime = dwCurTime;
	m_dwMotionEndTime += pGrp->GetAnimationFullTime( nAniID ) * fTimeRate;
	m_dwMotionEndTime += dwPreDelay;	

	m_dwFireMotionEndTime = dwCurTime;
	m_dwFireMotionEndTime += pGrp->GetAnimationEventTime_e( nAniID, "firemotion_e" ) * fTimeRate;

	m_dwEnableKeyReserve = dwCurTime + pGrp->GetAnimationEventTime_e( nAniID, "key_reserve" ) * fTimeRate;

	m_SkillState = eState;

	if ( pChar->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_SKILL_EXTRA_INFO );
		kPacket << pChar->GetCharName();
		kPacket << GetName();
		kPacket << eState;
		kPacket << pChar->GetWorldOrientation();
		kPacket << pChar->GetWorldPosition();
		kPacket << bFloating;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

void ioTargetTimingAttackSkill::SetZeroAimedExtraAttackState( ioBaseChar *pChar )
{
	if ( !pChar )
		return;

	if ( pChar->IsFloating() )
		m_CurAttack = m_JumpZeroAimedExtraAttack;
	else
		m_CurAttack = m_NormalZeroAimedExtraAttack;

	ioEntityGroup *pGrp = pChar->GetGroup();

	int nAniID = pGrp->GetAnimationIdx( m_CurAttack.m_AttackAnimation );
	if( nAniID == -1 )	
		return;

	float fTimeRate  = m_CurAttack.m_fAttackAniRate;
	DWORD dwPreDelay = m_CurAttack.m_dwPreDelay;

	pGrp->ClearAllLoopAni( 100.0f, true );
	pGrp->ClearAllActionAni( 100.0f, true );

	pGrp->SetActionAni( nAniID, 100.0f, 100.0f, 1.0f, 1.0f/fTimeRate, 0.0f, dwPreDelay );

	pChar->ClearReservedSliding();
	pChar->SetReservedSliding( m_CurAttack.m_vForceInfoList, nAniID, fTimeRate, dwPreDelay );
	pChar->CheckCharColSkipSkillTime( nAniID, fTimeRate, dwPreDelay );

	pChar->RefreshFireTimeList( nAniID, m_CurAttack.m_vWeaponInfoList, FTT_SKILL_ATTACK, fTimeRate, dwPreDelay );

	DWORD dwCurTime = FRAMEGETTIME();
	m_dwMotionEndTime = dwCurTime;
	m_dwMotionEndTime += pGrp->GetAnimationFullTime( nAniID ) * fTimeRate;
	m_dwMotionEndTime += dwPreDelay;	

	m_dwFireMotionEndTime = dwCurTime;
	m_dwFireMotionEndTime += pGrp->GetAnimationEventTime_e( nAniID, "firemotion_e" ) * fTimeRate;

	m_SkillState = SS_ZERO_AIM_EXTRA_ATTACK;
}

void ioTargetTimingAttackSkill::SetEndState( ioBaseChar *pChar )
{
	if ( !pChar )
		return;

	m_SkillState = SS_END;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

bool ioTargetTimingAttackSkill::CheckInputChangeTarget( ioBaseChar *pChar )
{
	if ( !pChar || !pChar->IsNeedProcess() )
		return false;

	if ( pChar->IsDefenseKey() )
	{
		m_bChangeTargetNext = true;
		return true;
	}

	if ( pChar->IsJumpKey() )
	{
		m_bChangeTargetPrev = true;
		return true;
	}

	return false;
}

bool ioTargetTimingAttackSkill::CheckInputAttack( ioBaseChar *pChar )
{
	if ( !pChar || !pChar->IsNeedProcess() )
		return false;

	if ( m_dwCheckAttackTime + m_dwAttackTime > FRAMEGETTIME() )
		return false;

	if ( pChar->IsAttackKey() )
		return true;

	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

bool ioTargetTimingAttackSkill::CheckExceptionCamera()
{
	if( !m_szAimedTarget.IsEmpty() )
		return true;

	return false;
}

D3DXVECTOR3 ioTargetTimingAttackSkill::GetExceptionCameraPos( ioBaseChar *pChar )
{
	if( !pChar )
		return ioMath::VEC3_ZERO;

	ioBaseChar *pTarget = pChar->GetBaseChar( m_szAimedTarget );
	if( pTarget )
		return pTarget->GetWorldPosition();

	return pChar->GetWorldPosition();
}