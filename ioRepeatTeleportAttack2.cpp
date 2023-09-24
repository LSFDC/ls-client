
#include "stdafx.h"

#include "ioRepeatTeleportAttack2.h"

#include "ioPlayStage.h"
#include "ioBaseChar.h"

#include "WeaponDefine.h"

ioRepeatTeleportAttack2::ioRepeatTeleportAttack2()
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

ioRepeatTeleportAttack2::ioRepeatTeleportAttack2( const ioRepeatTeleportAttack2 &rhs )
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
	m_fSkillRange( rhs.m_fSkillRange ),
	m_fSkillAngle( rhs.m_fSkillAngle ),
	m_fOffSet( rhs.m_fOffSet ),
	m_fBackRange( rhs.m_fBackRange ),
	m_dwReservKeyTime( rhs.m_dwReservKeyTime ),
	m_fNoneTargetMoveRange( rhs.m_fNoneTargetMoveRange ),
	m_stNoneTargetMarkBuff( rhs.m_stNoneTargetMarkBuff )
{
	m_SkillState = SS_NONE;

	m_bSetHandMesh = false;
	m_dwCurSkillProtectTime = 0;
	m_dwWeaponRepeatTime = 0;
	m_dwTeleportTime = 0;
	m_dwWeaponFireTime = 0;
	m_dwUseGaugeTime = 0;
	m_CurDir = ioUserKeyInput::DKI_NONE;
	m_dwNextReservKeyTime = 0;
}

ioRepeatTeleportAttack2::~ioRepeatTeleportAttack2()
{
}

void ioRepeatTeleportAttack2::LoadProperty( ioINILoader &rkLoader )
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

	//////////////////////////////////////////////////////////////////////////
	m_dwReservKeyTime = rkLoader.LoadInt_e( "wait_key_input_time", 0 );

	m_fSkillRange = rkLoader.LoadFloat_e( "apply_range", 0.0f );
	m_fSkillAngle = rkLoader.LoadFloat_e( "apply_angle", 0.0f );
	m_fOffSet = rkLoader.LoadFloat_e( "apply_position_offset", 0.0f );
	m_fBackRange = rkLoader.LoadFloat_e( "back_move_range", 0.0f );

	m_fNoneTargetMoveRange = rkLoader.LoadFloat_e( "none_target_move_range", 0.f );

	rkLoader.LoadString_e( "none_target_mark_buff", "", szBuf, MAX_PATH );
	m_stNoneTargetMarkBuff = szBuf;
}

ioSkill* ioRepeatTeleportAttack2::Clone()
{
	return new ioRepeatTeleportAttack2( *this );
}

bool ioRepeatTeleportAttack2::OnSkillStart( ioBaseChar *pChar )
{	
	m_SkillState = SS_NONE;

	m_bSetHandMesh = false;
	m_dwCurSkillProtectTime = 0;
	m_dwWeaponRepeatTime = 0;
	m_dwTeleportTime = 0;
	m_dwWeaponFireTime = 0;
	m_dwUseGaugeTime = 0;
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

void ioRepeatTeleportAttack2::OnProcessState( ioBaseChar *pChar, ioPlayStage *pStage )
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
				TeleportFindEnemy( pChar, pStage, false );
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

			if( pChar->IsNeedProcess() && m_dwNextReservKeyTime < dwCurTime )
			{
				SetEndState( pChar );
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

void ioRepeatTeleportAttack2::OnSkillEnd( ioBaseChar *pChar )
{
	ioAttackSkill::OnSkillEnd( pChar );

	EndCameraBuff( pChar );

	// �⺻���� ����
	if( m_bUseItemMesh )
		pChar->ShowWeaponItemMesh( true );

	if( !m_HandMesh.IsEmpty() )
		pChar->RestoreSkillWeaponMesh( this, m_HandMesh, m_bVisibleEquipMesh );

	m_bSetHandMesh = false;
}

void ioRepeatTeleportAttack2::ApplyAttributeToWeapon( ioWeapon *pWeapon, ioPlayStage *pStage )
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

void ioRepeatTeleportAttack2::ApplyExtraSkillInfo( ioBaseChar *pChar, ioPlayStage *pStage, SP2Packet &rkPacket )
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
	case SS_TELEPORT_FIND_ENEMY:
		{
			pStage->CreateMapEffect( m_szTeleportStartEffect, pChar->GetWorldPosition(), D3DXVECTOR3(1.0f,1.0f,1.0f) );
			D3DXVECTOR3 vNewPos;
			D3DXQUATERNION	qtRot;

			rkPacket >> vNewPos >> qtRot;
			pChar->SetWorldPosition( vNewPos );
			pChar->SetTargetRotToRotate( qtRot, true, false );

			SetAttackState( pChar, false );
		}
		break;
	case SS_ATTACK:
		SetAttackState( pChar, false );
		break;
	}
}

bool ioRepeatTeleportAttack2::IsProtected( int iDefenseBreakType ) const
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

bool ioRepeatTeleportAttack2::IsSkillMotionEnd( DWORD dwActionStop, ioBaseChar* pOwner ) const
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

bool ioRepeatTeleportAttack2::IsAttackEndState() const
{
	if( m_SkillState == SS_END )
		return true;

	return false;
}

bool ioRepeatTeleportAttack2::IsNoDropState() const
{
	return true;
	/*switch ( m_SkillState )
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
	return m_bNoDropState;*/
}

bool ioRepeatTeleportAttack2::IsCanSendControl() const
{
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ioRepeatTeleportAttack2::CheckSkillGauge( ioBaseChar *pChar )
{
	if ( !pChar )
		return true;

	DWORD dwCurTime = FRAMEGETTIME();

	if ( m_dwUseGaugeTime > 0 && m_dwUseGaugeTime + m_dwTicTime < dwCurTime )
	{
		m_dwUseGaugeTime = dwCurTime;

		if( pChar->ReduceCurSkillGaugeBySkill( GetOwnerItem(), m_fGaugePerTic ) <= 0.0f )
		{
			if ( m_dwMotionEndTime < dwCurTime )
				return true;
		}
	}

	return false;
}

bool ioRepeatTeleportAttack2::CheckKeyInput( ioBaseChar *pChar, ioPlayStage *pStage )
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
		TeleportFindEnemy( pChar, pStage );
		SetAttackState( pChar, true );
		return true;
	}

	return false;
}

bool ioRepeatTeleportAttack2::CheckTeleport( ioBaseChar *pChar )
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

bool ioRepeatTeleportAttack2::CheckAttack( ioBaseChar *pChar )
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

void ioRepeatTeleportAttack2::SetLoopState( ioBaseChar *pChar )
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

	m_dwNextReservKeyTime = FRAMEGETTIME() + m_dwReservKeyTime;
}

void ioRepeatTeleportAttack2::SetTelePortState( ioBaseChar *pChar, ioPlayStage *pStage )
{
	if ( !pChar || !pChar->IsNeedProcess() || !pStage )
		return;

	TeleportMove( pChar, pStage );
	//SetAttackState( pChar, true );
}

void ioRepeatTeleportAttack2::SetAttackState( ioBaseChar *pChar, bool bSendPacket )
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

	if ( bSendPacket && pChar->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_SKILL_EXTRA_INFO );
		kPacket << pChar->GetCharName();
		kPacket << GetName();
		kPacket << SS_ATTACK;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

void ioRepeatTeleportAttack2::SetEndState( ioBaseChar *pChar )
{	
	m_SkillState = SS_END;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void ioRepeatTeleportAttack2::TeleportMove( ioBaseChar *pChar, ioPlayStage *pStage )
{
	if ( !pChar || !pChar->IsNeedProcess() || !pStage )
		return;

	//pStage->CreateMapEffect( m_szTeleportStartEffect, pChar->GetWorldPosition(), D3DXVECTOR3(1.0f,1.0f,1.0f) );
	pChar->SetTargetRotToDirKey( m_CurDir );

	TeleportFindEnemy( pChar, pStage, true );

	/*D3DXVECTOR3 vNewPos = pChar->GetWorldPosition() + pChar->GetTargetRot() * m_vTeleportOffset;
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
	}*/
}


void ioRepeatTeleportAttack2::TeleportFindEnemy( ioBaseChar *pOwner, ioPlayStage *pStage, bool bSendNet )
{
	if ( !pOwner || !pOwner->IsNeedProcess() || !pStage )
		return;

	if( !pStage )
		return;

	if( !pOwner->IsNeedProcess() )
		return;

	pStage->CreateMapEffect( m_szTeleportStartEffect, pOwner->GetWorldPosition(), D3DXVECTOR3(1.0f,1.0f,1.0f) );

	float fPrevDistSq = m_fSkillRange;
	float fHalfCosine = cosf( DEGtoRAD( m_fSkillAngle ) * 0.5f );

	ioBaseChar *pTargetChar = pOwner;
	const BaseCharList &rkTargetList = pStage->GetBaseCharList();
	BaseCharList::const_iterator iter;
	for( iter = rkTargetList.begin(); iter != rkTargetList.end(); ++iter )
	{
		ioBaseChar *pChar = (*iter);
		if( pChar == pOwner )
			continue;

		if( !pStage->IsCollisionCheckByMode() )
			continue;

		if( pOwner->GetCharName() == pChar->GetCharName() )
			continue;

		if( pOwner->GetTeam() == pChar->GetTeam() )	
			continue;

		if( pChar->IsSystemState() )
			continue;
			
		if( !pChar->IsEnableTarget() )
			continue;

		if( pChar->HasBuff( pOwner->GetCharName(), m_stNoneTargetMarkBuff) )
			continue;	

		D3DXVECTOR3 vSightDir = pOwner->GetTargetRot() * D3DXVECTOR3( 0.0f, 0.0f, -1.0f );
		D3DXVec3Normalize( &vSightDir, &vSightDir );

		D3DXVECTOR3 vDiff = pChar->GetWorldPosition() - pOwner->GetWorldPosition();
		D3DXVECTOR3 vTargetDir = vDiff;
		D3DXVec3Normalize( &vTargetDir, &vTargetDir );

		if( D3DXVec3Dot( &vSightDir, &vTargetDir ) < fHalfCosine )
			continue;

		float fDistSq = D3DXVec3LengthSq( &vDiff );
		if( fDistSq < fPrevDistSq )	// Enemy
		{
			pTargetChar = pChar;
			fPrevDistSq = fDistSq;
		}
	}

	if( pOwner && pTargetChar )
	{
		MoveTeleport( pOwner, pTargetChar, pStage );

		if( pOwner->IsNeedSendNetwork() )
		{
			SP2Packet kPacket( CUPK_SKILL_EXTRA_INFO );
			kPacket << pOwner->GetCharName();
			kPacket << GetName();
			kPacket << SS_TELEPORT_FIND_ENEMY;
			kPacket << pOwner->GetWorldPosition();
			kPacket << pOwner->GetWorldOrientation();
			P2PNetwork::SendToAllPlayingUser( kPacket );
		}
	}

	SetAttackState( pOwner, false );
}


void ioRepeatTeleportAttack2::MoveTeleport( ioBaseChar *pOwner, ioBaseChar *pTarget, ioPlayStage *pStage )
{
	D3DXVECTOR3 vTargetPos = pTarget->GetWorldPosition();// + vOffSet;
	D3DXQUATERNION qtTargetOrientation = pTarget->GetWorldOrientation();	
	D3DXVECTOR3 vNewPos;


	if( pOwner->GetCharName() == pTarget->GetCharName() )
	{
		D3DXVECTOR3 vDir = pOwner->GetTargetRot() * D3DXVECTOR3( 0.f, 0.f, -1.f ); 
		vNewPos = vTargetPos + ( vDir * m_fNoneTargetMoveRange );
		float fMapHeight = pStage->GetMapHeight( vNewPos.x, vNewPos.z );
		if( fMapHeight > 0.0f )
			vNewPos.y = fMapHeight;

		pOwner->SetWorldPosition( vNewPos );
	}
	else
	{
		D3DXVECTOR3 vOwnerPos = pOwner->GetWorldPosition();
		D3DXVECTOR3 vDir = vTargetPos - vOwnerPos;
		D3DXVec3Normalize( &vDir, &vDir );
		vNewPos = vTargetPos + ( vDir * m_fBackRange );
		pOwner->SetWorldPosition( vNewPos );
		pOwner->SetTargetRotToTargetPos( vTargetPos, true );
	}

}
