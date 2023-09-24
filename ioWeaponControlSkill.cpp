
#include "stdafx.h"

#include "ioWeaponControlSkill.h"


#include "ioBaseChar.h"
#include "ioPlayStage.h"

#include "WeaponDefine.h"

ioWeaponControlSkill::ioWeaponControlSkill()
{
	m_ControlState = RS_NONE;

	m_dwLoopStartTime = 0;
	m_bSetHandMesh = false;

	m_dwFireStartTime = 0;
	m_dwEndAniEndTime = 0;
}

ioWeaponControlSkill::ioWeaponControlSkill( const ioWeaponControlSkill &rhs )
: ioNormalSkill( rhs ),
 m_AttackAttribute( rhs.m_AttackAttribute ),
 m_LoopAnimation( rhs.m_LoopAnimation ),
 m_dwLoopTime( rhs.m_dwLoopTime ),
 m_StartHandMesh( rhs.m_StartHandMesh ),
 m_LoopHandMesh( rhs.m_LoopHandMesh ),
 m_EndHandMesh( rhs.m_EndHandMesh ),
 m_dwLoopProtectDuration( rhs.m_dwLoopProtectDuration ),
 m_EndAnimation( rhs.m_EndAnimation ),
 m_fEndAniRate( rhs.m_fEndAniRate )
{
	m_ControlState = RS_NONE;

	m_dwLoopStartTime = 0;
	m_bSetHandMesh = false;

	m_dwFireStartTime = 0;
	m_dwEndAniEndTime = 0;
}

ioWeaponControlSkill::~ioWeaponControlSkill()
{
}

void ioWeaponControlSkill::LoadProperty( ioINILoader &rkLoader )
{
	ioNormalSkill::LoadProperty( rkLoader );

	char szBuf[MAX_PATH];

	// Animation
	rkLoader.LoadString_e( "loop_animation", "", szBuf, MAX_PATH );
	m_LoopAnimation = szBuf;
	m_dwLoopTime = rkLoader.LoadInt_e( "loop_duration", 0 );
	m_dwLoopProtectDuration = rkLoader.LoadInt_e( "loop_protect_duration", 0 );

	rkLoader.LoadString_e( "end_animation", "", szBuf, MAX_PATH );
	m_EndAnimation = szBuf;
	m_fEndAniRate = rkLoader.LoadFloat_e( "end_ani_rate", FLOAT1 );

	LoadAttackAttribute_e( "control_attack", m_AttackAttribute, rkLoader );

	// HandMesh
	rkLoader.LoadString_e( "start_hand_mesh", "", szBuf, MAX_PATH );
	m_StartHandMesh = szBuf;
	rkLoader.LoadString_e( "loop_hand_mesh", "", szBuf, MAX_PATH );
	m_LoopHandMesh = szBuf;
	rkLoader.LoadString_e( "end_hand_mesh", "", szBuf, MAX_PATH );
	m_EndHandMesh = szBuf;
}

ioSkill* ioWeaponControlSkill::Clone()
{
	return new ioWeaponControlSkill( *this );
}

bool ioWeaponControlSkill::IsProtected( int iDefenseBreakType ) const
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

	if( m_ControlState == RS_ACTION )
		return true;

	if( m_ControlState == RS_LOOP )
	{
		if( m_dwLoopProtectDuration == 0 )
			return true;

		if( m_dwProtectTime > dwCurTime )
			return true;
	}
	
	return false;
}

bool ioWeaponControlSkill::IsCanUseSkill( ioBaseChar *pChar, bool bHelpMsg )
{
	ioAttackableItem *pAttackable = ToAttackableItem( GetOwnerItem() );
	if( pAttackable )
	{
		if( !pAttackable->IsCanFire( pChar ) )
			return false;
	}

	if( !CheckNeedGauge( pChar, bHelpMsg ) )
		return false;

	return true;
}

bool ioWeaponControlSkill::IsSkillMotionEnd( DWORD dwActionStop, ioBaseChar* pOwner ) const
{
	if( m_ControlState != RS_END )
		return false;

	if( m_dwEndAniEndTime > 0 && m_dwEndAniEndTime < FRAMEGETTIME() )
		return true;

	return false;
}

bool ioWeaponControlSkill::OnSkillStart( ioBaseChar *pChar )
{
	if( !HasSkillMotion() )
		return false;

	pChar->SetSkillProtection();

	m_dwEnableSwitchTime = 0;
	m_dwEnableSemiSwitchTime = 0;

	if( m_dwSwitchSkill > 0 )
		m_dwEnableSwitchTime = FRAMEGETTIME() + m_dwSwitchSkill;
	if( m_dwSemiSwitchSkill > 0 )
		m_dwEnableSemiSwitchTime = FRAMEGETTIME() + m_dwSemiSwitchSkill;

	m_dwSkillStartTime = FRAMEGETTIME();

	m_ControlState = RS_NONE;
	m_dwWeaponBaseIndex = pChar->IncreaseWeaponIndexBase();

	m_dwMotionStartTime = 0;
	m_dwLoopStartTime = 0;
	m_dwFireStartTime = 0;
	m_dwEndAniEndTime = 0;

	m_vCreatePos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	m_dwProtectTime = 0;
	m_bSetHandMesh = false;

	m_bReduceGauge = false;

	// �⺻ ���� �տ��� ����
	if( m_bUseItemMesh )
	{
		pChar->ShowWeaponItemMesh( false );
	}

	DWORD dwPreDelayTime = GetPreDelayTime();
	if( !StartPreDelayAnimation( pChar ) )	// Delay Animation�� ����
	{
		m_dwPreDelayStartTime = 0;
		EndPreDelayEffect( pChar );
		SetActionState( pChar );
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
			dwTrackingTime += FRAMEGETTIME();
		else
			dwTrackingTime = FRAMEGETTIME() + dwPreDelay;

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

void ioWeaponControlSkill::OnProcessState( ioBaseChar *pChar, ioPlayStage *pStage )
{
	if( !pChar ) return;

	DWORD dwCurTime = FRAMEGETTIME();
	DWORD dwPreDelayTime = GetPreDelayTime();

	switch( m_ControlState )
	{
	case RS_NONE:
		if( m_dwPreDelayStartTime > 0 && m_dwPreDelayStartTime + dwPreDelayTime < dwCurTime )
		{
			m_dwPreDelayStartTime = 0;
			EndPreDelayEffect( pChar );
			SetActionState( pChar );
		}
		break;
	case RS_ACTION:
		if( m_dwMotionStartTime > 0 && m_dwMotionEndTime < dwCurTime )
		{
			m_dwMotionStartTime = 0;
			SetLoopState( pChar, pStage );
		}
		
		// SetLoopState�� �߻��������� �Ѵ�
		if( m_dwFireStartTime > 0 && m_dwFireStartTime < dwCurTime )
		{
			m_dwFireStartTime = 0;

			m_vCreatePos = pChar->GetMidPositionByRate();
			pChar->SkillFireExplicit( m_AttackAttribute.m_vWeaponInfoList.front(),
									  GetName(),
									  m_dwWeaponBaseIndex,
									  m_vCreatePos );
		}
		break;
	case RS_LOOP:
		if( m_dwLoopStartTime > 0 && m_dwLoopStartTime+m_dwLoopTime < dwCurTime )
		{
			m_dwLoopStartTime = 0;
			SetEndState( pChar );
		}
		else if( !CheckControlWeapon( pChar ) )
		{
			m_dwLoopStartTime = 0;
			SetEndState( pChar );
		}
		break;
	case RS_END:
		break;
	}
}

void ioWeaponControlSkill::OnSkillEnd( ioBaseChar *pOwner )
{
	// �⺻���� ����
	if( m_bUseItemMesh )
	{
		pOwner->ShowWeaponItemMesh( true );
	}

	// ��ų �߰� ���ۿ� ���� �޽��� ����
	m_bSetHandMesh = false;

	if( !m_StartHandMesh.IsEmpty() )
		pOwner->RestoreSkillWeaponMesh( this, m_StartHandMesh, m_bVisibleEquipMesh );

	if( !m_LoopHandMesh.IsEmpty() )
		pOwner->RestoreSkillWeaponMesh( this, m_LoopHandMesh, m_bVisibleEquipMesh );

	if( !m_EndHandMesh.IsEmpty() )
		pOwner->RestoreSkillWeaponMesh( this, m_EndHandMesh, m_bVisibleEquipMesh );

	// Ani
	m_dwMotionStartTime = 0;
	m_dwLoopStartTime = 0;
	m_dwEndAniEndTime = 0;

	ioNormalSkill::OnSkillEnd( pOwner );

	g_WeaponMgr.CheckSkillDependencyWeaponDead( pOwner->GetCharName(), GetName() );
}

void ioWeaponControlSkill::StartPreDelayEffect( ioBaseChar *pChar )
{
	ioSkill::StartPreDelayEffect( pChar );
}

void ioWeaponControlSkill::EndPreDelayEffect( ioBaseChar *pChar )
{
	ioSkill::EndPreDelayEffect( pChar );
}

void ioWeaponControlSkill::SetActionState( ioBaseChar *pChar )
{
	m_dwMotionStartTime = FRAMEGETTIME();
	m_dwMotionEndTime = m_dwMotionStartTime;

	ioEntityGroup *pGrp = pChar->GetGroup();
	ioHashString szSkillMotion = GetSkillMotionName();

	int iCurSkillAniID = pGrp->GetAnimationIdx( szSkillMotion );
	if( iCurSkillAniID == -1 )	return;

	float fAniRate = GetSkillMotionRate();

	float fTimeRate = fAniRate * m_fExtraAniRate;
	
	pGrp->ClearAllLoopAni( FLOAT100, true );
	pGrp->ClearAllActionAni( FLOAT100, true );
	pGrp->SetActionAni( iCurSkillAniID, FLOAT100, FLOAT100, FLOAT1, FLOAT1/fTimeRate );
	

	m_dwMotionEndTime += pGrp->GetAnimationFullTime( iCurSkillAniID )*fTimeRate;

	DWORD dwCreateTime = pGrp->GetAnimationEventTime_e( iCurSkillAniID, "fire_s" ) * fTimeRate;
	m_dwFireStartTime = m_dwMotionStartTime + dwCreateTime;

	m_ControlState = RS_ACTION;

	pChar->AttachEffect( m_SkillEffectName );

	// ��� ������ ó��
	if( m_bUseItemMesh && !m_StartHandMesh.IsEmpty() )
	{
		pChar->ChangeSkillWeaponMesh( this, m_StartHandMesh, m_bVisibleEquipMesh );
		m_bSetHandMesh = true;
	}

	if( !m_bReduceGauge )
	{
		pChar->ReduceNeedGaugeBySkill( this, GetNeedGauge(pChar->GetCharName()) );
		m_bReduceGauge = true;
	}
}

void ioWeaponControlSkill::SetLoopState( ioBaseChar *pChar, ioPlayStage *pStage )
{
	if( m_LoopAnimation.IsEmpty() )
	{
		m_dwLoopStartTime = 0;
		return;
	}

	if( m_bSetHandMesh )
	{
		if( !m_StartHandMesh.IsEmpty() )
		{
			pChar->RestoreSkillWeaponMesh( this, m_StartHandMesh, m_bVisibleEquipMesh );
		}
		m_bSetHandMesh = false;
	}

	// ��� ������ ó��
	if( m_bUseItemMesh && !m_LoopHandMesh.IsEmpty() )
	{
		pChar->ChangeSkillWeaponMesh( this, m_LoopHandMesh, m_bVisibleEquipMesh );
		m_bSetHandMesh = true;
	}

	ioEntityGroup *pGrp = pChar->GetGroup();
	pGrp->ClearAllLoopAni( FLOAT100, true );
	pGrp->ClearAllActionAni( FLOAT100, true );
	pGrp->SetLoopAni( m_LoopAnimation, FLOAT100 );

	m_dwLoopStartTime = FRAMEGETTIME();
	m_dwProtectTime = m_dwLoopStartTime + m_dwLoopProtectDuration;

	m_ControlState = RS_LOOP;
}

void ioWeaponControlSkill::SetEndState( ioBaseChar *pChar )
{
	if( !pChar ) return;

	m_ControlState = RS_END;
	DWORD dwCurTime = FRAMEGETTIME();
	m_dwEndAniEndTime = dwCurTime;

	ioEntityGroup *pGrp = pChar->GetGroup();
	int iCurSkillAniID = pGrp->GetAnimationIdx( m_EndAnimation );
	if( iCurSkillAniID == -1 )	return;

	pGrp->ClearAllLoopAni( FLOAT100, true );
	pGrp->ClearAllActionAni( FLOAT100, true );
	pGrp->SetActionAni( iCurSkillAniID, FLOAT100, FLOAT100, FLOAT1, FLOAT1/m_fEndAniRate );

	m_dwEndAniEndTime += pGrp->GetAnimationFullTime( iCurSkillAniID ) * m_fEndAniRate;

	if( m_bSetHandMesh )
	{
		if( !m_LoopHandMesh.IsEmpty() )
		{
			pChar->RestoreSkillWeaponMesh( this, m_LoopHandMesh, m_bVisibleEquipMesh );
		}
		m_bSetHandMesh = false;
	}

	// ��� ������ ó��
	if( m_bUseItemMesh && !m_EndHandMesh.IsEmpty() )
	{
		pChar->ChangeSkillWeaponMesh( this, m_EndHandMesh, m_bVisibleEquipMesh );
		m_bSetHandMesh = true;
	}

	g_WeaponMgr.CheckSkillDependencyWeaponDead( pChar->GetCharName(), GetName() );
}

void ioWeaponControlSkill::ApplyExtraSkillInfo( ioBaseChar *pChar, ioPlayStage *pStage, SP2Packet &rkPacket )
{
}

const D3DXVECTOR3& ioWeaponControlSkill::GetWeaponPos( ioBaseChar *pChar )
{
	if( !pChar )
		return m_vCreatePos;

	if( m_dwWeaponBaseIndex < 0 )
		return pChar->GetWorldPosition();

	ioWeapon *pWeapon = g_WeaponMgr.FindWeapon( pChar, m_dwWeaponBaseIndex );

	if( ToBombMissileWeapon2( pWeapon ) || ToBombMissileWeapon4( pWeapon ) || ToBombMissileWeapon5( pWeapon ) )
	{
		return pWeapon->GetPosition();
	}

	return m_vCreatePos;
}

bool ioWeaponControlSkill::CheckControlWeapon( ioBaseChar *pChar )
{
	ioWeapon *pWeapon = g_WeaponMgr.FindWeapon( pChar, m_dwWeaponBaseIndex );
	if( ToBombMissileWeapon2( pWeapon ) || ToBombMissileWeapon4( pWeapon ) || ToBombMissileWeapon5( pWeapon ) )
		return true;

	return false;
}

bool ioWeaponControlSkill::CheckUseItemMesh()
{
	return m_bUseItemMesh;
}

bool ioWeaponControlSkill::IsWeaponControl( ioBaseChar *pChar )
{
	switch( m_ControlState )
	{
	case RS_LOOP:
	case RS_END:
		if( CheckControlWeapon( pChar ) )
			return true;
		break;
	}

	return false;
}

bool ioWeaponControlSkill::UseActiveCount( ioBaseChar *pOwner )
{
	if( m_ControlState != RS_LOOP )
		return false;

	if( !pOwner )
		return false;

	ioWeapon *pWeapon = g_WeaponMgr.FindWeapon( pOwner, m_dwWeaponBaseIndex );
	ioBombMissileWeapon4 *pBombM4 = ToBombMissileWeapon4( pWeapon );
	if( pBombM4 && pBombM4->GetMaxExplosionCnt() > 0 )
		return true;

	ioBombMissileWeapon5 *pBombM5 = ToBombMissileWeapon5( pWeapon );
	//if( pBombM5 && pBombM5->GetMaxExplosionCnt() > 0 )
		//return true;
	if( pBombM5 )
		return false;

	return false;
}

int ioWeaponControlSkill::GetMaxActiveCount( ioBaseChar *pOwner )
{
	if( m_ControlState != RS_LOOP )
		return 0;

	if( !pOwner )
		return 0;

	ioWeapon *pWeapon = g_WeaponMgr.FindWeapon( pOwner, m_dwWeaponBaseIndex );
	ioBombMissileWeapon4 *pBombM4 = ToBombMissileWeapon4( pWeapon );
	if( pBombM4 )
		return pBombM4->GetMaxExplosionCnt();

	ioBombMissileWeapon5 *pBombM5 = ToBombMissileWeapon5( pWeapon );
	if( pBombM5 )
		return pBombM5->GetMaxExplosionCnt();

	return 0;
}

int ioWeaponControlSkill::GetCurActiveCount( ioBaseChar *pOwner )
{
	if( m_ControlState != RS_LOOP )
		return 0;

	if( !pOwner )
		return 0;

	ioWeapon *pWeapon = g_WeaponMgr.FindWeapon( pOwner, m_dwWeaponBaseIndex );
	ioBombMissileWeapon4 *pBombM4 = ToBombMissileWeapon4( pWeapon );
	if( pBombM4 )
		return pBombM4->GetCurExplosionCnt();

	ioBombMissileWeapon5 *pBombM5 = ToBombMissileWeapon5( pWeapon );
	if( pBombM5 )
		return pBombM5->GetCurExplosionCnt();

	return 0;
}
