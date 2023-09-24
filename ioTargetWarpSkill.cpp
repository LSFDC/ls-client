

#include "stdafx.h"



#include "ioPlayStage.h"
#include "ioBaseChar.h"

#include "ioTargetWarpSkill.h"

#include "WeaponDefine.h"
#include "ItemDefine.h"

ioTargetWarpSkill::ioTargetWarpSkill()
{
	m_bSetHandMesh = false;
	m_SkillState = SS_NONE;
}

ioTargetWarpSkill::ioTargetWarpSkill( const ioTargetWarpSkill &rhs )
: ioAttackSkill( rhs ),
 m_WarpMotion( rhs.m_WarpMotion ),
 m_fWarpMotionRate( rhs.m_fWarpMotionRate ),
 m_fWarpOffsetHeight( rhs.m_fWarpOffsetHeight )
{
	m_bSetHandMesh = false;
	m_SkillState = SS_NONE;
}

ioTargetWarpSkill::~ioTargetWarpSkill()
{
}

void ioTargetWarpSkill::LoadProperty( ioINILoader &rkLoader )
{
	ioAttackSkill::LoadProperty( rkLoader );

	char szBuf[MAX_PATH];

	//
	rkLoader.LoadString_e( "warp_motion", "", szBuf, MAX_PATH );
	m_WarpMotion = szBuf;

	m_fWarpMotionRate = rkLoader.LoadFloat_e( "warp_motion_rate", FLOAT1 );

	m_fWarpOffsetHeight = rkLoader.LoadFloat_e( "warp_offset_height", 0.0f );
}

ioSkill* ioTargetWarpSkill::Clone()
{
	return new ioTargetWarpSkill( *this );
}

bool ioTargetWarpSkill::OnSkillStart( ioBaseChar *pChar )
{
	if( !HasSkillMotion() )
		return false;

	DWORD dwCurTime = FRAMEGETTIME();
	pChar->SetSkillProtection();

	m_dwAttackEndTime = 0;

	InitExtraAniJump();

	m_dwAniRotateTime = 0;
	m_fAniRotateAngle = 0.0f;

	m_iCurAniRotate = 0;

	m_dwEnableSwitchTime = 0;
	m_dwEnableSemiSwitchTime = 0;
	
	m_dwEnableReserveTime = 0;

	if( m_dwSwitchSkill > 0 )
		m_dwEnableSwitchTime = dwCurTime + m_dwSwitchSkill;
	if( m_dwSemiSwitchSkill > 0 )
		m_dwEnableSemiSwitchTime = dwCurTime + m_dwSemiSwitchSkill;

	m_dwSkillStartTime = FRAMEGETTIME();

	ioPlayStage *pStage = g_SkillMaker.GetPlayStage();

	m_bSetHandMesh = false;
	m_SkillState = SS_NONE;
	m_fCurChargeRate = 0.0f;

	// 기본 무기 손에서 제거
	if( m_bUseItemMesh )
	{
		pChar->ShowWeaponItemMesh( false );
	}

	m_dwWeaponBaseIndex = pChar->IncreaseWeaponIndexBase();
	if( !StartPreDelayAnimation( pChar ) )	// Delay Animation이 없다
	{
		AttackSkillFire( pChar );
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
				dwTrackingTime = pGrp->GetAnimationEventTime_e( iAniID, "tracking_e" );
		}

		DWORD dwPreDelay = GetPreDelayTime();

		if( dwTrackingTime > 0 )
			dwTrackingTime += dwCurTime;
		else
			dwTrackingTime = dwCurTime + dwPreDelay;

		if( IsUseAutoTargetValue() )
		{
			pChar->SetAutoTarget( ATT_SKILL );
			pChar->SetAutoTargetTrackingEndTime( dwTrackingTime );
		}
	}

	CheckSkillDesc( pChar );

	if( m_bInitJumpPower )
	{
		pChar->SetCurJumpPowerAmt( 0.0f );
		pChar->SetGravityAmt( 0.0f );
	}

	SetSkillNameChatBubble( pChar );

	return true;
}

void ioTargetWarpSkill::OnSkillEnd( ioBaseChar *pChar )
{
	ioAttackSkill::OnSkillEnd( pChar );
	
	if( GetOwnerItem() )
		GetOwnerItem()->ClearCurChargeRateForSkill();

	// 기본무기 장착
	if( m_bUseItemMesh )
	{
		pChar->ShowWeaponItemMesh( true );
	}

	if( !m_HandMesh.IsEmpty() )
	{
		pChar->RestoreSkillWeaponMesh( this, m_HandMesh, m_bVisibleEquipMesh );
	}

	m_bSetHandMesh = false;
	m_SkillState = SS_NONE;
	m_fCurChargeRate = 0.0f;
}

bool ioTargetWarpSkill::CheckUseItemMesh()
{
	return m_bUseItemMesh;
}

void ioTargetWarpSkill::ApplyAttributeToWeapon( ioWeapon *pWeapon, ioPlayStage *pStage )
{
	ioSkill::ApplyAttributeToWeapon( pWeapon, pStage );

	ioBaseChar *pChar = NULL;
	if( pWeapon )
		pChar = pWeapon->GetOwner();

	ioZoneEffectWeapon *pZone = ToZoneEffectWeapon( pWeapon );
	if( pZone )
	{
		if( pChar )
		{
			D3DXVECTOR3 vDir = pChar->GetTargetRot() * D3DXVECTOR3( 0.0f, 0.0f, -FLOAT1 );
			pZone->SetCollisionDir( vDir );
			pZone->SetZoneValue( false, true );
		}
	}
}

void ioTargetWarpSkill::OnProcessState( ioBaseChar *pChar, ioPlayStage *pStage )
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

				AttackSkillFire( pChar );
			}
		}
		break;
	case SS_FIRE:
		CheckExtraAniJump( pChar );
		break;
	}
}

bool ioTargetWarpSkill::IsProtected( int iDefenseBreakType ) const
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

	switch( m_SkillState )
	{
	case SS_FIRE:
		return true;
	}

	return false;
}

void ioTargetWarpSkill::ApplyExtraSkillInfo( ioBaseChar *pChar, ioPlayStage *pStage, SP2Packet &rkPacket )
{
}

bool ioTargetWarpSkill::IsSkillMotionEnd( DWORD dwActionStop, ioBaseChar* pOwner ) const
{
	if( m_dwPreDelayStartTime > 0 )
		return false;

	switch( m_SkillState )
	{
	case SS_NONE:
		return false;
	}

	if( m_dwMotionEndTime + dwActionStop > FRAMEGETTIME() )
		return false;

	return true;
}

void ioTargetWarpSkill::AttackSkillFire( ioBaseChar *pChar )
{
	ioAttackSkill::AttackSkillFire( pChar );

	m_SkillState = SS_FIRE;
}

void ioTargetWarpSkill::SetWarpMotionState( ioBaseChar *pChar, ioWeapon *pWeapon )
{
	if( !pChar ) return;
	if( !pWeapon ) return;

	if( m_SkillState != SS_FIRE )
		return;

	ioEntityGroup *pGrp = pChar->GetGroup();

	int iAniID = pGrp->GetAnimationIdx( m_WarpMotion );
	float fTimeRate = m_fWarpMotionRate;
	if( fTimeRate <= 0.0f )
		fTimeRate = FLOAT1;

	pGrp->ClearAllActionAni( FLOAT100, true );
	pGrp->ClearAllLoopAni( FLOAT100, true );

	pGrp->SetActionAni( iAniID, FLOAT100, FLOAT100, FLOAT1, FLOAT1/fTimeRate, 0.0f, 0.0f, true );

	m_dwMotionStartTime = m_dwMotionEndTime = FRAMEGETTIME();
	m_dwMotionEndTime += pGrp->GetAnimationFullTime( iAniID ) * fTimeRate;

	m_dwFireMotionEndTime = m_dwMotionEndTime;

	//
	ioPlayStage *pStage = g_SkillMaker.GetPlayStage();
	if( pStage )
	{
		float fBoundHeight = pStage->GetBoundHeight();
		D3DXVECTOR3 vTargetPos = pWeapon->GetPosition();
		vTargetPos.y += m_fWarpOffsetHeight;

		D3DXVECTOR3 vBoundPos = vTargetPos;
		vBoundPos.y = fBoundHeight;
		
		float fMapHeight = pStage->GetMapHeight( vBoundPos.x, vBoundPos.y );
		if( vTargetPos.y <= fMapHeight )
			vTargetPos.y = fMapHeight;

		pChar->SetWorldPosition( vTargetPos );

		pStage->SetCameraToOwnerExplicit( pChar );
	}
	//

	m_SkillState = SS_WARP;
}

bool ioTargetWarpSkill::IsEndState() const
{
	if( m_SkillState == SS_WARP )
	{
		return true;
	}

	return false;
}





