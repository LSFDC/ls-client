
#include "stdafx.h"

#include "ioRepeatTeleportAttack.h"

#include "ioPlayStage.h"
#include "ioBaseChar.h"

ioRepeatTeleportAttack::ioRepeatTeleportAttack()
{
	m_SkillState = SS_NONE;
	
	m_bSetHandMesh = false;
	m_dwCurSkillProtectTime = 0;
	m_dwWeaponRepeatTime = 0;
	m_dwTeleportTime = 0;
	m_dwWeaponFireTime = 0;
	m_dwUseGaugeTime = 0;
	m_CurDir = ioUserKeyInput::DKI_NONE;
}

ioRepeatTeleportAttack::ioRepeatTeleportAttack( const ioRepeatTeleportAttack &rhs )
	: ioAttackSkill( rhs ),
      m_dwSkillProtectTime( rhs.m_dwSkillProtectTime ),
	  m_fGaugePerTic( rhs.m_fGaugePerTic ),
	  m_dwTicTime( rhs.m_dwTicTime ),
	  m_AttackInfo( rhs.m_AttackInfo ),
	  m_szLoopAni( rhs.m_szLoopAni ),
	  m_dwCheckWeaponRepeatTime( rhs.m_dwCheckWeaponRepeatTime ),
	  m_dwCheckTeleportTime( rhs.m_dwCheckTeleportTime ),
	  m_dwCheckWeaponFireTime( rhs.m_dwCheckWeaponFireTime ),
	  m_vTeleportOffset( rhs.m_vTeleportOffset ),
	  m_bNoDropState( rhs.m_bNoDropState ),
	  m_szTeleportStartEffect( rhs.m_szTeleportStartEffect ),
	  m_szTeleportEndEffect( rhs.m_szTeleportEndEffect ),
	  m_dwDropDelay( rhs.m_dwDropDelay ),
	  m_nMaxAttackCnt( rhs.m_nMaxAttackCnt ),
	  m_bUseMaxAttackCnt( rhs.m_bUseMaxAttackCnt )
{
	m_SkillState = SS_NONE;
	
	m_bSetHandMesh = false;
	m_dwCurSkillProtectTime = 0;
	m_dwWeaponRepeatTime = 0;
	m_dwTeleportTime = 0;
	m_dwWeaponFireTime = 0;
	m_dwUseGaugeTime = 0;
	m_nCurAttackCnt = 0;
	m_CurDir = ioUserKeyInput::DKI_NONE;
}

ioRepeatTeleportAttack::~ioRepeatTeleportAttack()
{
}

void ioRepeatTeleportAttack::LoadProperty( ioINILoader &rkLoader )
{
	ioAttackSkill::LoadProperty( rkLoader );
		
	char szBuf[MAX_PATH];

	//��ų ���� �ð�
	m_dwSkillProtectTime = (DWORD)rkLoader.LoadInt_e( "skill_protect_time", 0 );

	m_fGaugePerTic = rkLoader.LoadFloat_e( "gauge_per_tick", 0.0f );
	m_dwTicTime	= (DWORD)rkLoader.LoadInt_e( "tick_time", 0 );

	LoadAttackAttribute( "Attack_Info", m_AttackInfo, rkLoader );
	
	rkLoader.LoadString_e( "loop_ani", "", szBuf, MAX_PATH );
	m_szLoopAni = szBuf;

	m_dwCheckWeaponRepeatTime = rkLoader.LoadInt_e( "repeat_create_weapon_time", 0 );
	m_dwCheckTeleportTime = rkLoader.LoadInt_e( "repeat_teleport_time", 0 );

	m_vTeleportOffset.x = rkLoader.LoadFloat_e( "teleport_offset_x", 0.0f );
	m_vTeleportOffset.y = rkLoader.LoadFloat_e( "teleport_offset_y", 0.0f );
	m_vTeleportOffset.z = rkLoader.LoadFloat_e( "teleport_offset_z", 0.0f );

	m_dwCheckWeaponFireTime = rkLoader.LoadInt_e( "reload_weapon_time", 0 );

	m_bNoDropState = rkLoader.LoadBool_e( "no_drop_state", false );

	rkLoader.LoadString_e( "teleport_start_effect", "", szBuf, MAX_PATH );
	m_szTeleportStartEffect = szBuf;
	rkLoader.LoadString_e( "teleport_end_effect", "", szBuf, MAX_PATH );
	m_szTeleportEndEffect = szBuf;

	m_dwDropDelay = rkLoader.LoadInt_e( "drop_delay", 0 );

	m_bUseMaxAttackCnt = rkLoader.LoadBool_e( "use_max_attack_cnt", false );
	m_nMaxAttackCnt = rkLoader.LoadInt_e( "max_attack_cnt", 0 );
}

ioSkill* ioRepeatTeleportAttack::Clone()
{
	return new ioRepeatTeleportAttack( *this );
}

bool ioRepeatTeleportAttack::OnSkillStart( ioBaseChar *pChar )
{	
	m_SkillState = SS_NONE;
	
	m_bSetHandMesh = false;
	m_dwCurSkillProtectTime = 0;
	m_dwWeaponRepeatTime = 0;
	m_dwTeleportTime = 0;
	m_dwWeaponFireTime = 0;
	m_dwUseGaugeTime = 0;
	m_nCurAttackCnt = 0;
	m_CurDir = ioUserKeyInput::DKI_NONE;

	if( m_dwSkillProtectTime > 0 )
		m_dwCurSkillProtectTime = FRAMEGETTIME() + m_dwSkillProtectTime;
		
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
		pChar->ReduceNeedGaugeBySkill( this, GetNeedGauge(pChar->GetCharName()) );
		m_bReduceGauge = true;
	}

	return true;
}

void ioRepeatTeleportAttack::OnProcessState( ioBaseChar *pChar, ioPlayStage *pStage )
{
	DWORD dwCurTime = FRAMEGETTIME();

	if ( CheckSkillGauge(pChar) )
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

				m_dwUseGaugeTime = dwCurTime;

				SetAttackState( pChar, false );
			}
		}
		break;
	case SS_LOOP:
		{
			if ( CheckKeyInput( pChar, pStage ) )
				return;

			if ( m_dwWeaponRepeatTime + m_dwCheckWeaponRepeatTime < dwCurTime )
			{
				SetAttackState( pChar, false );
				return;
			}
		}
		break;
	case SS_ATTACK:
		{			
			if ( m_dwMotionEndTime < dwCurTime )
			{
				m_dwWeaponFireTime = dwCurTime;
				SetLoopState( pChar );
				return;
			}
		}
		break;
	case SS_END:
		break;
	}
}

void ioRepeatTeleportAttack::OnSkillEnd( ioBaseChar *pChar )
{
	ioAttackSkill::OnSkillEnd( pChar );

	EndCameraBuff( pChar );

	// �⺻���� ����
	if( m_bUseItemMesh )
		pChar->ShowWeaponItemMesh( true );

	if( !m_HandMesh.IsEmpty() )
		pChar->RestoreSkillWeaponMesh( this, m_HandMesh, m_bVisibleEquipMesh );

	m_bSetHandMesh = false;

	if( m_bUseMaxAttackCnt)
		pChar->ReduceCurSkillGaugeBySkill( GetOwnerItem(), 1000.f );
}

void ioRepeatTeleportAttack::ApplyAttributeToWeapon( ioWeapon *pWeapon, ioPlayStage *pStage )
{
	ioSkill::ApplyAttributeToWeapon( pWeapon, pStage );	
}

void ioRepeatTeleportAttack::ApplyExtraSkillInfo( ioBaseChar *pChar, ioPlayStage *pStage, SP2Packet &rkPacket )
{
	if( !pChar || !pStage ) 
		return;

	int nState;
	rkPacket >> nState;

	switch( nState )
	{
	case SS_TELEPORT:
		{
			pStage->CreateMapEffect( m_szTeleportStartEffect, pChar->GetWorldPosition(), D3DXVECTOR3(1.0f,1.0f,1.0f) );

			int nNewDir;
			rkPacket >> nNewDir;
			m_CurDir = (ioUserKeyInput::DirKeyInput)nNewDir;

			pChar->SetTargetRotToDirKey( m_CurDir );

			D3DXVECTOR3 vNewPos;
			rkPacket >> vNewPos;

			pChar->SetWorldPosition( vNewPos );
			pStage->CreateMapEffect( m_szTeleportEndEffect, vNewPos, D3DXVECTOR3(1.0f,1.0f,1.0f) );

			SetAttackState( pChar, false );
		}
		break;
	case SS_ATTACK:
		SetAttackState( pChar, false );
		break;
	}
}

bool ioRepeatTeleportAttack::IsProtected( int iDefenseBreakType ) const
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

	return false;
}

bool ioRepeatTeleportAttack::IsSkillMotionEnd( DWORD dwActionStop, ioBaseChar* pOwner ) const
{	
	if ( m_SkillState != SS_END )
		return false;

	float fHeightGap = pOwner->GetWorldPosition().y - pOwner->GetBottomHeight();

	if( m_fSkillEndNextJumpAmt > 0.0f )
	{
		if( fHeightGap > 10.f )
			return false;
	}

	if( m_dwPreDelayStartTime > 0 )
		return false;
	
	if( m_dwMotionEndTime + dwActionStop >= FRAMEGETTIME() )
		return false;

	return true;
}

bool ioRepeatTeleportAttack::IsAttackEndState() const
{
	if( m_SkillState == SS_END )
		return true;

	return false;
}

bool ioRepeatTeleportAttack::IsNoDropState() const
{
	switch ( m_SkillState )
	{
	case SS_LOOP:
		{
			if ( m_dwWeaponRepeatTime + m_dwDropDelay > FRAMEGETTIME() )
				return true;
		}
		break;
	case SS_ATTACK:
		return true;
	}
	return m_bNoDropState;
}

bool ioRepeatTeleportAttack::IsCanSendControl() const
{
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ioRepeatTeleportAttack::CheckSkillGauge( ioBaseChar *pChar )
{
	if ( !pChar )
		return true;

	DWORD dwCurTime = FRAMEGETTIME();

	if ( m_dwUseGaugeTime > 0 && m_dwUseGaugeTime + m_dwTicTime < dwCurTime )
	{
		m_dwUseGaugeTime = dwCurTime;

		if( m_bUseMaxAttackCnt && m_nCurAttackCnt >= m_nMaxAttackCnt )
		{
			if ( m_dwMotionEndTime < dwCurTime )
				return true;
		}

		if( pChar->ReduceCurSkillGaugeBySkill( GetOwnerItem(), m_fGaugePerTic ) <= 0.0f )
		{
			if ( m_dwMotionEndTime < dwCurTime )
				return true;
		}
	}

	return false;
}

bool ioRepeatTeleportAttack::CheckKeyInput( ioBaseChar *pChar, ioPlayStage *pStage )
{
	if( !pChar || !pChar->IsNeedProcess() || !pStage ) 
		return false;

	if ( CheckTeleport( pChar ) )
	{
		SetTelePortState( pChar, pStage );
		return true;
	}

	if ( CheckAttack( pChar ) )
	{
		SetAttackState( pChar, true );
		return true;
	}

	return false;
}

bool ioRepeatTeleportAttack::CheckTeleport( ioBaseChar *pChar )
{
	if( !pChar || !pChar->IsNeedProcess() ) 
		return false;

	DWORD dwCurTime = FRAMEGETTIME();
	if ( m_dwTeleportTime + m_dwCheckTeleportTime > dwCurTime )
		return false;

	m_CurDir = pChar->CheckCurDirKey();

	if ( m_CurDir != ioUserKeyInput::DKI_NONE && pChar->IsAttackKey() )
	{
		m_dwTeleportTime = dwCurTime;
		return true;
	}
	return false;
}

bool ioRepeatTeleportAttack::CheckAttack( ioBaseChar *pChar )
{
	if( !pChar || !pChar->IsNeedProcess() ) 
		return false;

	DWORD dwCurTime = FRAMEGETTIME();
	if ( m_dwWeaponFireTime + m_dwCheckWeaponFireTime > dwCurTime )
		return false;

	if ( pChar->IsAttackKey() )
		return true;

	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void ioRepeatTeleportAttack::SetLoopState( ioBaseChar *pChar )
{
	if ( !pChar )
		return;

	ioEntityGroup *pGrp = pChar->GetGroup();

	int nAniID = pGrp->GetAnimationIdx( m_szLoopAni );
	if( nAniID == -1 )	
		return;

	pGrp->ClearAllLoopAni( 100.0f, true );
	pGrp->ClearAllActionAni( 100.0f, true );

	pGrp->SetLoopAni( nAniID, 100.0f );

	DWORD dwCurTime = FRAMEGETTIME();
	m_dwWeaponRepeatTime = dwCurTime;

	m_SkillState = SS_LOOP;
}

void ioRepeatTeleportAttack::SetTelePortState( ioBaseChar *pChar, ioPlayStage *pStage )
{
	if ( !pChar || !pChar->IsNeedProcess() || !pStage )
		return;
	
	TeleportMove( pChar, pStage );
	SetAttackState( pChar, true );
}

void ioRepeatTeleportAttack::SetAttackState( ioBaseChar *pChar, bool bSendPacket )
{
	if ( !pChar )
		return;

	ioEntityGroup *pGrp = pChar->GetGroup();

	//�ܰ��� �ִϸ��̼� �� �б�
	int nAniID = pGrp->GetAnimationIdx( m_AttackInfo.m_AttackAnimation );
	if( nAniID == -1 )	
		return;

	//������ ����, ������ ����
	float fTimeRate  = m_AttackInfo.m_fAttackAniRate;
	DWORD dwPreDelay = m_AttackInfo.m_dwPreDelay;

	pGrp->ClearAllLoopAni( 100.0f, true );
	pGrp->ClearAllActionAni( 100.0f, true );

	//��� �ð� ����
	DWORD dwCurTime = FRAMEGETTIME();
	m_dwMotionEndTime = dwCurTime;

	//���� �ð� ����
	m_dwMotionEndTime += pGrp->GetAnimationFullTime( nAniID ) * fTimeRate;
	m_dwMotionEndTime += dwPreDelay;
		
	pGrp->SetActionAni( nAniID, 100.0f, 100.0f, 1.0f, 1.0f/fTimeRate, 0.0f, dwPreDelay );

	pChar->ClearReservedSliding();
	pChar->SetReservedSliding( m_AttackInfo.m_vForceInfoList, nAniID, fTimeRate, dwPreDelay );

	m_dwWeaponBaseIndex = pChar->IncreaseWeaponIndexBase();

	pChar->RefreshFireTimeListWithBase( m_dwWeaponBaseIndex, nAniID, m_AttackInfo.m_vWeaponInfoList, FTT_SKILL_ATTACK, fTimeRate, dwPreDelay );

	m_SkillState = SS_ATTACK;

	m_nCurAttackCnt++;

	if ( bSendPacket && pChar->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_SKILL_EXTRA_INFO );
		kPacket << pChar->GetCharName();
		kPacket << GetName();
		kPacket << SS_ATTACK;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

void ioRepeatTeleportAttack::SetEndState( ioBaseChar *pChar )
{	
	m_SkillState = SS_END;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void ioRepeatTeleportAttack::TeleportMove( ioBaseChar *pChar, ioPlayStage *pStage )
{
	if ( !pChar || !pChar->IsNeedProcess() || !pStage )
		return;

	pStage->CreateMapEffect( m_szTeleportStartEffect, pChar->GetWorldPosition(), D3DXVECTOR3(1.0f,1.0f,1.0f) );
	pChar->SetTargetRotToDirKey( m_CurDir );
	
	D3DXVECTOR3 vNewPos = pChar->GetWorldPosition() + pChar->GetTargetRot() * m_vTeleportOffset;
	float fMapHeight = pStage->GetMapHeightNoneEntity( vNewPos.x, vNewPos.z );
	if ( fMapHeight > 0.0f )
		vNewPos.y = fMapHeight;

	pChar->SetWorldPosition( vNewPos );
	pStage->CreateMapEffect( m_szTeleportEndEffect, vNewPos, D3DXVECTOR3(1.0f,1.0f,1.0f) );

	if ( pChar->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_SKILL_EXTRA_INFO );
		kPacket << pChar->GetCharName();
		kPacket << GetName();
		kPacket << SS_TELEPORT;
		kPacket << (int)m_CurDir;
		kPacket << vNewPos;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}