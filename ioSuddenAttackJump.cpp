

#include "stdafx.h"

#include "ioPlayStage.h"
#include "ioBaseChar.h"

#include "ioSuddenAttackJump.h"

#include "ItemDefine.h"

ioSuddenAttackJump::ioSuddenAttackJump()
{
	m_ChargeComboState = CCS_NONE;

	m_bUseExtendJump = false;
	m_bUsedComboAttack = false;
	m_bUsedDoubleJump = false;
	m_bUsedFlash = false;

	// combo
	m_iCurComboCnt = 0;
	m_dwEnableComboTime_S = 0;
	m_dwEnableComboTime_E = 0;

	m_bReserveJumpAttack = false;

	m_dwChargingStartTime = 0;
	m_dwFireStartTime = 0;

	// double
	m_iCurJumpCnt = 1;

	// flash
	m_dwFlashDelayEndTime = 0;
	m_dwFlashEndTime = 0;

	// dash
	m_dwJumpDashStartTime = 0;
	m_dwJumpDashEndTime = 0;

	m_GuidEffectID = -1;

	m_bUsedChargeAttack = false;
}

ioSuddenAttackJump::ioSuddenAttackJump( const ioSuddenAttackJump &rhs )
: ioExtendJump( rhs ),
m_fJumpSpeedRateFR( rhs.m_fJumpSpeedRateFR ),
m_fJumpSpeedRateSI( rhs.m_fJumpSpeedRateSI ),
m_fJumpSpeedRateBG( rhs.m_fJumpSpeedRateBG ),
m_fDoubleJumpSpeedRateFR( rhs.m_fDoubleJumpSpeedRateFR ),
m_fDoubleJumpSpeedRateSI( rhs.m_fDoubleJumpSpeedRateSI ),
m_fDoubleJumpSpeedRateBG( rhs.m_fDoubleJumpSpeedRateBG ),
m_ComboAttackList( rhs.m_ComboAttackList ),
m_ChargeComboAttackList( rhs.m_ChargeComboAttackList ),
m_FlashDelayAni( rhs.m_FlashDelayAni ),
m_fFlashDelayRate( rhs.m_fFlashDelayRate ),
m_fFlashForce( rhs.m_fFlashForce ),
m_fFlashGravityAmt( rhs.m_fFlashGravityAmt ),
m_iMaxJumpCnt( rhs.m_iMaxJumpCnt ),
m_fDoubleJumpPower( rhs.m_fDoubleJumpPower ),
m_fDoubleJumpForce( rhs.m_fDoubleJumpForce ),
m_fDoubleJumpEnableHeight( rhs.m_fDoubleJumpEnableHeight ),
m_dwDoubleJumpEnableTime( rhs.m_dwDoubleJumpEnableTime ),
m_vDoubleJumpAniList( rhs.m_vDoubleJumpAniList ),
m_fDoubleJumpAniRate( rhs.m_fDoubleJumpAniRate ),
m_FlashJumpAttack( rhs.m_FlashJumpAttack ),
m_bEnableFlashJump( rhs.m_bEnableFlashJump ),
m_bEnableDoubleJump( rhs.m_bEnableDoubleJump ),
m_bEnableJumpDash( rhs.m_bEnableJumpDash ),
m_DashJumpAttack( rhs.m_DashJumpAttack ),
m_JumpDashLandAni( rhs.m_JumpDashLandAni ),
m_fJumpDashLandAniRate( rhs.m_fJumpDashLandAniRate ),
m_fJumpDashGravityAmt( rhs.m_fJumpDashGravityAmt ),
m_ComboDashType( rhs.m_ComboDashType ),
m_bUseNewJumpTypeDash( rhs.m_bUseNewJumpTypeDash ),
m_fExtraJumpPowerRate( rhs.m_fExtraJumpPowerRate ),
m_dwJumpChargeTime( rhs.m_dwJumpChargeTime ),
m_dwChargingTime( rhs.m_dwChargingTime ),
m_ChargingAniList( rhs.m_ChargingAniList ),
m_fChargeJumpPower( rhs.m_fChargeJumpPower ),
m_fChargeGravityRate( rhs.m_fChargeGravityRate ),
m_fChargeBaseRange( rhs.m_fChargeBaseRange ),
m_ChargeAttackLandAni( rhs.m_ChargeAttackLandAni ),
m_fChargeAttackLandAniRate( rhs.m_fChargeAttackLandAniRate ),
m_stExtend1_Aim_Up( rhs.m_stExtend1_Aim_Up ),
m_stExtend1_Aim_Center( rhs.m_stExtend1_Aim_Center ),
m_stExtend1_Aim_Down( rhs.m_stExtend1_Aim_Down ),
m_stExtend2_Aim_Up( rhs.m_stExtend2_Aim_Up ),
m_stExtend2_Aim_Center( rhs.m_stExtend2_Aim_Center ),
m_stExtend2_Aim_Down( rhs.m_stExtend2_Aim_Down ),
m_stExtend1_Aim_Up_Fire( rhs.m_stExtend1_Aim_Up_Fire ),
m_stExtend1_Aim_Center_Fire( rhs.m_stExtend1_Aim_Center_Fire ),
m_stExtend1_Aim_Down_Fire( rhs.m_stExtend1_Aim_Down_Fire ),
m_stExtend2_Aim_Up_Fire( rhs.m_stExtend2_Aim_Up_Fire ),
m_stExtend2_Aim_Center_Fire( rhs.m_stExtend2_Aim_Center_Fire ),
m_stExtend2_Aim_Down_Fire( rhs.m_stExtend2_Aim_Down_Fire ),
m_Extend1ModeAttList( rhs.m_Extend1ModeAttList ),
m_Extend2ModeAttList( rhs.m_Extend2ModeAttList ),
m_Extend1ChargingAni( rhs.m_Extend1ChargingAni ),
m_Extend2ChargingAni( rhs.m_Extend2ChargingAni )
{
	m_ChargeComboState = CCS_NONE;

	m_bUseExtendJump = false;
	m_bUsedComboAttack = false;
	m_bUsedDoubleJump = false;
	m_bUsedFlash = false;

	// combo
	m_iCurComboCnt = 0;
	m_dwEnableComboTime_S = 0;
	m_dwEnableComboTime_E = 0;

	m_bReserveJumpAttack = false;

	m_dwChargingStartTime = 0;
	m_dwFireStartTime = 0;

	// double
	m_iCurJumpCnt = 1;

	// flash
	m_dwFlashDelayEndTime = 0;
	m_dwFlashEndTime = 0;

	// dash
	m_dwJumpDashStartTime = 0;
	m_dwJumpDashEndTime = 0;

	m_GuidEffectID = -1;

	m_bUsedChargeAttack = false;
}

ioExtendJump* ioSuddenAttackJump::Clone()
{
	return new ioSuddenAttackJump( *this );
}

ioSuddenAttackJump::~ioSuddenAttackJump()
{
}

JumpType ioSuddenAttackJump::GetType() const
{
	return JT_SUDDEN_ATTACK_JUMP;
}

void ioSuddenAttackJump::LoadProperty( ioINILoader &rkLoader )
{
	ioExtendJump::LoadProperty( rkLoader );

	char szBuf[MAX_PATH] = "";
	char szKey[MAX_PATH] = "";

	//
	m_fExtraJumpPowerRate = rkLoader.LoadFloat_e( "jump_extra_power_rate", FLOAT1 );
	m_dwJumpChargeTime = rkLoader.LoadInt_e( "jump_charge_time", 0 );

	m_bEnableFlashJump = rkLoader.LoadBool_e( "enable_flash_jump", false );
	m_bEnableDoubleJump = rkLoader.LoadBool_e( "enable_double_jump", false );
	m_bEnableJumpDash = rkLoader.LoadBool_e( "enable_jump_dash", false );

	// combo
	m_fJumpSpeedRateFR = rkLoader.LoadFloat_e( "extend_jump_speed_rate_front", FLOAT1 );
	m_fJumpSpeedRateSI = rkLoader.LoadFloat_e( "extend_jump_speed_rate_side", FLOAT1 );
	m_fJumpSpeedRateBG = rkLoader.LoadFloat_e( "extend_jump_speed_rate_back", FLOAT1 );

	LoadComboList( rkLoader );
	LoadChargeComboList( rkLoader );

	m_dwChargingTime = (DWORD)rkLoader.LoadInt_e( "charging_time", 0 );

	int iMaxChargeAni = rkLoader.LoadInt_e( "charging_jump_ani_cnt", 0 );
	for( int i=0; i < iMaxChargeAni; ++i )
	{
		wsprintf_e( szKey, "charging_jump_ani%d", i+1 );
		rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );
		m_ChargingAniList.push_back( szBuf );
	}

	// double
	m_iMaxJumpCnt = rkLoader.LoadInt_e( "max_jump_cnt", 0 );
	m_iMaxJumpCnt = max( 1, m_iMaxJumpCnt );

	m_fDoubleJumpPower = rkLoader.LoadFloat_e( "double_jump_power", 0.0f );
	m_fDoubleJumpForce = rkLoader.LoadFloat_e( "double_jump_force", 0.0f );
	m_dwDoubleJumpEnableTime = rkLoader.LoadInt_e( "double_jump_enable_time", 0 );
	m_fDoubleJumpEnableHeight = rkLoader.LoadFloat_e( "double_jump_enable_height", 0.0f );

	m_fDoubleJumpAniRate = rkLoader.LoadFloat_e( "double_jump_ani_rate", FLOAT1 );

	m_vDoubleJumpAniList.clear();
	m_vDoubleJumpAniList.reserve( 4 );

	rkLoader.LoadString_e( "double_jump_animation_front", "", szBuf, MAX_PATH );
	m_vDoubleJumpAniList.push_back( ioHashString(szBuf) );
	rkLoader.LoadString_e( "double_jump_animation_right", "", szBuf, MAX_PATH );
	m_vDoubleJumpAniList.push_back( ioHashString(szBuf) );
	rkLoader.LoadString_e( "double_jump_animation_back", "", szBuf, MAX_PATH );
	m_vDoubleJumpAniList.push_back( ioHashString(szBuf) );
	rkLoader.LoadString_e( "double_jump_animation_left", "", szBuf, MAX_PATH );
	m_vDoubleJumpAniList.push_back( ioHashString(szBuf) );

	m_fDoubleJumpSpeedRateFR = rkLoader.LoadFloat_e( "double_jump_speed_rate_front", FLOAT1 );
	m_fDoubleJumpSpeedRateSI = rkLoader.LoadFloat_e( "double_jump_speed_rate_side", FLOAT1 );
	m_fDoubleJumpSpeedRateBG = rkLoader.LoadFloat_e( "double_jump_speed_rate_back", FLOAT1 );

	// flash
	rkLoader.LoadString_e( "flash_jump_delay_animation", "", szBuf, MAX_PATH );
	m_FlashDelayAni = szBuf;

	m_fFlashDelayRate = rkLoader.LoadFloat_e( "flash_jump_delay_ani_rate", FLOAT1 );

	m_fFlashForce = rkLoader.LoadFloat_e( "flash_jump_force", 0.0f );
	m_fFlashGravityAmt = rkLoader.LoadFloat_e( "flash_jump_gravity_amt", FLOAT1 );

	m_FlashJumpAttack.Init();
	LoadAttackAttribute_e( "flash_jump_attack", m_FlashJumpAttack, rkLoader );

	// dash
	m_bEnableJumpDash = rkLoader.LoadBool_e( "enable_jump_dash", false );
	m_ComboDashType = (ComboDashType)rkLoader.LoadInt_e( "jump_dash_type", CDT_GRAVITY );

	m_DashJumpAttack.Init();
	LoadAttackAttribute_e( "dash_jump_attack", m_DashJumpAttack, rkLoader );

	rkLoader.LoadString_e( "jump_dash_land_ani", "", szBuf, MAX_PATH );
	m_JumpDashLandAni = szBuf;
	m_fJumpDashLandAniRate = rkLoader.LoadFloat_e( "jump_dash_land_ani_rate", FLOAT1 );

	m_fJumpDashGravityAmt = rkLoader.LoadFloat_e( "jump_dash_gravity_amt", 0.0f );

	m_bUseNewJumpTypeDash = rkLoader.LoadBool_e( "use_new_jump_type_dash", false );

	//
	m_fChargeJumpPower = rkLoader.LoadFloat_e( "jump_charge_jump_power", 0.0f );
	m_fChargeGravityRate = rkLoader.LoadFloat_e( "jump_charge_gravity_rate", FLOAT1 );
	m_fChargeBaseRange = rkLoader.LoadFloat_e( "jump_charge_base_range", 0.0f );

	rkLoader.LoadString_e( "jump_charge_land", "", szBuf, MAX_PATH );
	m_ChargeAttackLandAni = szBuf;
	m_fChargeAttackLandAniRate = rkLoader.LoadFloat_e( "jump_charge_land_time_rate", FLOAT1 );


	//////////////////////////////////////////////////////////////////////////
	//���� ����
	rkLoader.LoadString_e( "extend1_jump_aim_up_animation", "", szBuf, MAX_PATH );
	m_stExtend1_Aim_Up = szBuf;
	rkLoader.LoadString_e( "extend1_jump_aim_center_animation", "", szBuf, MAX_PATH );
	m_stExtend1_Aim_Center = szBuf;
	rkLoader.LoadString_e( "extend1_jump_aim_down_animation", "", szBuf, MAX_PATH );
	m_stExtend1_Aim_Down = szBuf;

	rkLoader.LoadString_e( "extend1_jump_aim_up_animation_fire", "", szBuf, MAX_PATH );
	m_stExtend1_Aim_Up_Fire = szBuf;
	rkLoader.LoadString_e( "extend1_jump_aim_center_animation_fire", "", szBuf, MAX_PATH );
	m_stExtend1_Aim_Center_Fire = szBuf;
	rkLoader.LoadString_e( "extend1_jump_aim_down_animation_fire", "", szBuf, MAX_PATH );
	m_stExtend1_Aim_Down_Fire = szBuf;

	rkLoader.LoadString_e( "extend2_jump_aim_up_animation", "", szBuf, MAX_PATH );
	m_stExtend2_Aim_Up = szBuf;
	rkLoader.LoadString_e( "extend2_jump_aim_center_animation", "", szBuf, MAX_PATH );
	m_stExtend2_Aim_Center = szBuf;
	rkLoader.LoadString_e( "extend2_jump_aim_down_animation", "", szBuf, MAX_PATH );
	m_stExtend2_Aim_Down = szBuf;

	rkLoader.LoadString_e( "extend2_jump_aim_up_animation_fire", "", szBuf, MAX_PATH );
	m_stExtend2_Aim_Up_Fire = szBuf;
	rkLoader.LoadString_e( "extend2_jump_aim_center_animation_fire", "", szBuf, MAX_PATH );
	m_stExtend2_Aim_Center_Fire = szBuf;
	rkLoader.LoadString_e( "extend2_jump_aim_down_animation_fire", "", szBuf, MAX_PATH );
	m_stExtend2_Aim_Down_Fire = szBuf;

	//////////////////////////////////////////////////////////////////////////
	m_Extend1ModeAttList.clear();
	int iMaxCombo = rkLoader.LoadInt_e( "jump_extend1_max_combo", 0 );
	for( int i=0 ; i<iMaxCombo ; i++ )
	{
		AttackAttribute kAttribute;

		wsprintf_e( szBuf, "jump_extend1_attack%d", i+1 );
		LoadAttackAttribute( szBuf, kAttribute, rkLoader );
		m_Extend1ModeAttList.push_back( kAttribute );
	}

	m_Extend2ModeAttList.clear();
	iMaxCombo = rkLoader.LoadInt_e( "jump_extend2_max_combo", 0 );
	for( int i=0 ; i<iMaxCombo ; i++ )
	{
		AttackAttribute kAttribute;

		wsprintf_e( szBuf, "jump_extend2_attack%d", i+1 );
		LoadAttackAttribute( szBuf, kAttribute, rkLoader );
		m_Extend2ModeAttList.push_back( kAttribute );
	}

	rkLoader.LoadString_e( "extend1_charging_jump_ani", "", szBuf, MAX_PATH );
	m_Extend1ChargingAni = szBuf;

	rkLoader.LoadString_e( "extend2_charging_jump_ani", "", szBuf, MAX_PATH );
	m_Extend2ChargingAni = szBuf;
}


void ioSuddenAttackJump::LoadComboList( ioINILoader &rkLoader )
{
	char szBuf[MAX_PATH];
	int iMaxCombo = rkLoader.LoadInt_e( "jump_max_combo", 0 );

	m_ComboAttackList.clear();
	for( int i=0 ; i<iMaxCombo ; i++ )
	{
		AttackAttribute kAttribute;

		wsprintf_e( szBuf, "jump_attack%d", i+1 );
		LoadAttackAttribute( szBuf, kAttribute, rkLoader );
		m_ComboAttackList.push_back( kAttribute );
	}
}


void ioSuddenAttackJump::LoadChargeComboList( ioINILoader &rkLoader )
{
	char szBuf[MAX_PATH];

	m_ChargeComboAttackList.clear();
	AttackAttribute kAttribute;
	wsprintf_e( szBuf, "jump_charge_attack%d", 1 );
	LoadAttackAttribute( szBuf, kAttribute, rkLoader );
	m_ChargeComboAttackList.push_back( kAttribute );
}

void ioSuddenAttackJump::SetJumpping( ioBaseChar *pOwner, bool bFullTime )
{
	ioAttackableItem *pItem = pOwner->GetPriorityItem( SPT_ATTACK );
	ioSuddenAttackItem *pSuddenItem = ToSuddenAttackItem( pItem );
	if( !pSuddenItem )
		return;

	ioEntityGroup *pGrp = pOwner->GetGroup();
	if( !pGrp )
		return;

	m_ChargeComboState = CCS_NONE;

	m_nWeaponItemMode = (int)pSuddenItem->GetMeshType();
	if( m_nWeaponItemMode < 0 || m_nWeaponItemMode > ioSuddenAttackItem::IMT_EXTRA2 )
		m_nWeaponItemMode = 0;

	if( m_nWeaponItemMode == ioSuddenAttackItem::IMT_EXTRA1 )
	{
		//pGrp->ClearAllActionAni( FLOAT100, true );
		//pGrp->ClearAllLoopAni( FLOAT100, true );

		pGrp->SetLoopAni( m_stExtend1_Aim_Up, 0.0f, 0.0f );
		pGrp->SetLoopAni( m_stExtend1_Aim_Down, 0.f, 0.0f );
		pGrp->SetLoopAni( m_stExtend1_Aim_Center, 0.0f, FLOAT1 );

		if( pSuddenItem->GetAimedTarget().IsEmpty() )
		{
			pOwner->SetTargetMarkerState( ioTargetMarker::MS_AIMING );
		}
		else
		{
			pOwner->SetTargetMarkerState( ioTargetMarker::MS_AIMED );
			pOwner->SetTargetMarkerTarget( pSuddenItem->GetAimedTarget(), false );
		}
	}
	else if( m_nWeaponItemMode == ioSuddenAttackItem::IMT_EXTRA2 )
	{
		if( !pSuddenItem->GetAimedTarget().IsEmpty() )
		{
			/*pGrp->ClearAllActionAni( FLOAT100, true );
			pGrp->ClearAllLoopAni( FLOAT100, true );

			pGrp->SetLoopAni( m_stExtend2_Aim_Up, 0.0f, 0.0f );
			pGrp->SetLoopAni( m_stExtend2_Aim_Down, 0.f, 0.0f );
			pGrp->SetLoopAni( m_stExtend2_Aim_Center, FLOAT100, FLOAT1 );*/

			if( pOwner->IsAttackKeyDown() )
				m_bExtend2FireReady = true;
			else
				m_bExtend2FireReady = false;

			SetAimingStateExtend2( pOwner );

			pOwner->SetTargetMarkerState( ioTargetMarker::MS_AIMED );
			pOwner->SetTargetMarkerTarget( pSuddenItem->GetAimedTarget(), false );
		}
		else if( pOwner->GetMarkerState() != ioTargetMarker::MS_NONE )
		{
			/*pGrp->ClearAllActionAni( FLOAT100, true );
			pGrp->ClearAllLoopAni( FLOAT100, true );

			pGrp->SetLoopAni( m_stExtend2_Aim_Up, 0.0f, 0.0f );
			pGrp->SetLoopAni( m_stExtend2_Aim_Down, 0.f, 0.0f );
			pGrp->SetLoopAni( m_stExtend2_Aim_Center, FLOAT100, FLOAT1 );

			pOwner->SetTargetMarkerState( ioTargetMarker::MS_AIMING );*/

			if( pOwner->IsAttackKeyDown() )
				m_bExtend2FireReady = true;
			else
				m_bExtend2FireReady = false;

			SetAimingStateExtend2( pOwner );
		}
	}

	pOwner->SetUsedFlashJump( false );

	m_bUseExtendJump = false;
	m_bUsedComboAttack = false;
	m_bUsedDoubleJump = false;
	m_bUsedFlash = false;

	// combo
	m_iCurComboCnt = 0;
	m_dwEnableComboTime_S = 0;
	m_dwEnableComboTime_E = 0;

	m_bReserveJumpAttack = false;

	m_dwChargingStartTime = 0;
	m_dwFireStartTime = 0;

	// double
	m_iCurJumpCnt = 1;

	// flash
	m_dwFlashDelayEndTime = 0;
	m_dwFlashEndTime = 0;

	// dash
	m_dwJumpDashStartTime = 0;
	m_dwJumpDashEndTime = 0;

	if( m_GuidEffectID != -1 )
	{
		pOwner->EndMapEffect( m_GuidEffectID, false );
		m_GuidEffectID = -1;
	}

	if( bFullTime && m_dwJumpChargeTime > 0 )
	{
		m_bUseExtendJump = true;
		float fJumpPower = pOwner->GetJumpPower();
		fJumpPower *= m_fExtraJumpPowerRate;
		pOwner->SetJumpPower( fJumpPower );

		pOwner->SetEnableHide( false );
		pOwner->SetEnableChangeTeam( false );
		pOwner->SetChangeJumpMotion( false, "", "", "", 0.0f );
	}

	//
	m_bUsedChargeAttack = false;
}


void ioSuddenAttackJump::ProcessLanding( ioBaseChar *pOwner )
{
}

void ioSuddenAttackJump::ProcessJumpping( ioBaseChar *pOwner, float fHeightGap, float fNeedGauge )
{
	if( !pOwner )
		return;

	// üũ ���� �߿�!!
	DWORD dwCurTime = FRAMEGETTIME();

	// combo
	if( m_bReserveJumpAttack )
	{
		SetChargingState( pOwner );
		return;
	}

	// DashCheck
	if( IsCanJumpDash( pOwner, fHeightGap ) )
	{
		SetJumpDash( pOwner, fHeightGap );
	}

	// Double
	if( IsCanDoubleJump( pOwner, fHeightGap ) )
	{
		if( m_fDoubleJumpPower >= 0.0f )
		{
			SetDoubleJump( pOwner );
			return;
		}
	}

	ProcessJumpDash( pOwner );

	/*if( m_nWeaponItemMode == ioSuddenAttackItem::IMT_EXTRA1 )
	{
		TrackingExtend1( pOwner );
	}*/
	/*else if( m_nWeaponItemMode == ioSuddenAttackItem::IMT_EXTRA2 )
	{
		TrackingExtend2( pOwner );
	}*/
}



void ioSuddenAttackJump::TrackingExtend1( ioBaseChar *pChar )
{
	ioAttackableItem *pItem = pChar->GetPriorityItem( SPT_ATTACK );
	ioSuddenAttackItem *pSuddenItem = ToSuddenAttackItem( pItem );
	if( !pSuddenItem )
		return;

	pSuddenItem->CheckAimTarget( pChar );

	//���� ������
	pSuddenItem->ProcessTargetTracking_Extend_Delay( pChar, m_stExtend1_Aim_Up, m_stExtend1_Aim_Center, m_stExtend1_Aim_Down );

	//��ü
	pSuddenItem->UpdateAimDir( pChar );
}


void ioSuddenAttackJump::TrackingExtend2( ioBaseChar *pChar )
{
	ioAttackableItem *pItem = pChar->GetPriorityItem( SPT_ATTACK );
	ioSuddenAttackItem *pSuddenItem = ToSuddenAttackItem( pItem );
	if( !pSuddenItem )
		return;

	if( pChar->IsNeedProcess() && !pChar->IsAttackKeyDown() )
	{
		
	}

	/*pSuddenItem->CheckAimTarget( pChar );

	//���� ������
	pSuddenItem->ProcessTargetTracking_Extend_Delay( pChar, m_stExtend2_Aim_Up, m_stExtend2_Aim_Center, m_stExtend2_Aim_Down );

	//��ü
	pSuddenItem->UpdateAimDir( pChar );*/
}



// combo
void ioSuddenAttackJump::CheckComboJumpAttack( ioBaseChar *pOwner )
{
	switch( m_ChargeComboState )
	{
	case CCS_NONE:
	case CCS_NORMAL:
	case CCS_DASH:
	case CCS_DOUBLE:
		break;
	default:
		return;
	}

	ioAttackableItem *pItem = pOwner->GetPriorityItem( SPT_ATTACK );
	ioSuddenAttackItem *pSuddenItem = ToSuddenAttackItem( pItem );
	if( !pSuddenItem )
		return;

	if( pOwner->IsNeedProcess() && m_nWeaponItemMode == ioSuddenAttackItem::IMT_EXTRA1 && pSuddenItem->GetCurBullet() <= 0 )
		return;

	int iMaxCnt = 0;
	if( m_nWeaponItemMode == ioSuddenAttackItem::IMT_NORMAL )
		iMaxCnt = m_ComboAttackList.size();
	else if( m_nWeaponItemMode == ioSuddenAttackItem::IMT_EXTRA1 )
		iMaxCnt = m_Extend1ModeAttList.size();
	else if( m_nWeaponItemMode == ioSuddenAttackItem::IMT_EXTRA2 )
		iMaxCnt = m_Extend2ModeAttList.size();

	if( !COMPARE( m_iCurComboCnt, 0, iMaxCnt ) )
		return;

	if( m_bReserveJumpAttack )
		return;

	DWORD dwCurTime = FRAMEGETTIME();
	DWORD dwLooseTime = 0;

	if( pOwner )
		dwLooseTime = pOwner->GetActionStopTotalDelay();

	if( m_iCurComboCnt > 0 && !COMPARE( dwCurTime, m_dwEnableComboTime_S+dwLooseTime, m_dwEnableComboTime_E+dwLooseTime ) )
		return;

	if( m_iCurComboCnt > 0 && pOwner->IsHasCrown() )
		return;

	m_bReserveJumpAttack = true;
}

void ioSuddenAttackJump::SetComboJumpAttack( ioBaseChar *pOwner )
{
	if( m_nWeaponItemMode == ioSuddenAttackItem::IMT_NORMAL )
		SetNormalModeAttack( pOwner );
	else if( m_nWeaponItemMode == ioSuddenAttackItem::IMT_EXTRA1 )
		SetExtend1ModeAttack( pOwner );
	else if( m_nWeaponItemMode == ioSuddenAttackItem::IMT_EXTRA2 )
		SetExtend2ModeAttack( pOwner );
}


void ioSuddenAttackJump::SetExtend1ModeAttack( ioBaseChar *pOwner )
{
	//LOG.PrintTimeAndLog( 0, "SetExtend1ModeAttack-In" );
	ioAttackableItem *pItem = pOwner->GetPriorityItem( SPT_ATTACK );
	ioSuddenAttackItem *pSuddenItem = ToSuddenAttackItem( pItem );
	if( !pSuddenItem )
		return;

	if( pOwner->IsNeedProcess() && pSuddenItem->GetCurBullet() <= 0 )
		return;

	DWORD dwCurTime = FRAMEGETTIME();
	m_ChargeComboState = CCS_NORMAL;

	pOwner->ClearAttackFireTimeAndSkill();

	ioEntityGroup *pGrp = pOwner->GetGroup();
	pGrp->ClearAllActionAni( FLOAT100, true );
	pGrp->ClearAllLoopAni( FLOAT100, true );

	if( pOwner && !pOwner->IsActionStop() )
		pOwner->InitActionStopDelay( true );
	else
		pOwner->InitActionStopDelay( false );

	const AttackAttribute *pAttr = 0;
	if( (int)m_Extend1ModeAttList.size() <= m_iCurComboCnt )
	{
		pAttr = &m_Extend1ModeAttList[ 0 ];
		//LOG.PrintTimeAndLog( 0, "SetExtend1ModeAttack-In-FIND %d", m_iCurComboCnt );
	}
	else
		pAttr = &m_Extend1ModeAttList[ m_iCurComboCnt ];

	if( pAttr )
	{
		int iAniID = pGrp->GetAnimationIdx( pAttr->m_AttackAnimation );
		float fTimeRate = pAttr->m_fAttackAniRate;
		DWORD dwPreDelay = pAttr->m_dwPreDelay;
		DWORD dwStartTime = dwCurTime + dwPreDelay;

		ioHashString stTargetName = pSuddenItem->GetAimedTarget();
		ioBaseChar *pTarget = pOwner->GetBaseChar( stTargetName );

		//pGrp->SetActionAni( pAttr->m_AttackAnimation, 0.0f, 0.0f, FLOAT1, FLOAT1/fTimeRate, 0, 0, true );
		pOwner->RefreshFireTimeList( iAniID, pAttr->m_vWeaponInfoList,
			FTT_JUMP_ATTACK,
			fTimeRate,
			dwPreDelay );

		pOwner->IncreaseWeaponIndexBase();
		SetAirJump( pOwner, iAniID, fTimeRate );

		m_bReserveJumpAttack = false;
		m_bUseExtendJump = true;
		pOwner->SetEnableHide( false );
		pOwner->SetNoWoundState( false );
		pOwner->SetEnableChangeTeam( false );

		m_dwEnableComboTime_E = dwStartTime + pGrp->GetAnimationFullTime( iAniID ) * fTimeRate;
		m_dwEnableComboTime_S = dwStartTime + CheckKeyReserveTime( pOwner, iAniID, fTimeRate );		

		pGrp->ClearAllLoopAni( 0.0f, true );
		pGrp->ClearAllActionAni( 0.0f, true );

		if( pTarget )
		{
			D3DXVECTOR3 vAimedDir = pTarget->GetMidPositionByRate() - pOwner->GetMidPositionByRate();
			D3DXVec3Normalize( &vAimedDir, &vAimedDir );
			float fWeight = pSuddenItem->GetAimedUpDownWeight( vAimedDir, 85.0f );
			if( vAimedDir.y >= 0.0f )
			{
				AimedFireToUp( pGrp, fWeight, fTimeRate, 0, true, true, m_stExtend1_Aim_Up_Fire, m_stExtend1_Aim_Center_Fire );
			}
			else
			{
				AimedFireToDown( pGrp, fWeight, fTimeRate, 0, true, true, m_stExtend1_Aim_Down_Fire, m_stExtend1_Aim_Center_Fire );
			}
		}
		else
			pGrp->SetActionAni( pAttr->m_AttackAnimation, 0.0f, 0.0f, FLOAT1, FLOAT1/fTimeRate, 0, 0, true );
	}

	pOwner->SetJumpAttackState();

	if( pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_EXTEND_JUMP );
		kPacket << pOwner->GetCharName();
		kPacket << (int)GetType();
		kPacket << pOwner->GetHP().m_fCurValue;
		kPacket << m_ChargeComboState;
		kPacket << m_iCurComboCnt;
		kPacket << pOwner->GetTargetRot();
		kPacket << pOwner->GetMoveDir();
		kPacket << pOwner->GetInputDir();
		pOwner->FillAutoTargetInfo( kPacket );
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}

	m_iCurComboCnt++;

	//LOG.PrintTimeAndLog( 0, "SetExtend1ModeAttack-Out" );
}


void ioSuddenAttackJump::SetExtend2ModeAttack( ioBaseChar *pOwner )
{
	ioAttackableItem *pItem = pOwner->GetPriorityItem( SPT_ATTACK );
	ioSuddenAttackItem *pSuddenItem = ToSuddenAttackItem( pItem );
	if( !pSuddenItem )
		return;

	if( pSuddenItem->GetCurBullet() <= 0 )
		return;

	DWORD dwCurTime = FRAMEGETTIME();
	m_ChargeComboState = CCS_NORMAL;

	pOwner->ClearAttackFireTimeAndSkill();

	ioEntityGroup *pGrp = pOwner->GetGroup();
	pGrp->ClearAllActionAni( FLOAT100, true );
	pGrp->ClearAllLoopAni( FLOAT100, true );

	if( pOwner && !pOwner->IsActionStop() )
		pOwner->InitActionStopDelay( true );
	else
		pOwner->InitActionStopDelay( false );

	const AttackAttribute *pAttr = &m_Extend2ModeAttList[ 0 ];
	if( pAttr )
	{
		int iAniID = pGrp->GetAnimationIdx( pAttr->m_AttackAnimation );
		float fTimeRate = pAttr->m_fAttackAniRate;
		DWORD dwPreDelay = pAttr->m_dwPreDelay;
		DWORD dwStartTime = dwCurTime + dwPreDelay;

		ioHashString stTargetName = pSuddenItem->GetAimedTarget();
		ioBaseChar *pTarget = pOwner->GetBaseChar( stTargetName );

		//pGrp->SetActionAni( pAttr->m_AttackAnimation, 0.0f, 0.0f, FLOAT1, FLOAT1/fTimeRate, 0, 0, true );
		pOwner->RefreshFireTimeList( iAniID, pAttr->m_vWeaponInfoList,
			FTT_JUMP_ATTACK,
			fTimeRate,
			dwPreDelay );

		pOwner->IncreaseWeaponIndexBase();
		SetAirJump( pOwner, iAniID, fTimeRate );

		m_bReserveJumpAttack = false;
		m_bUseExtendJump = true;
		pOwner->SetEnableHide( false );
		pOwner->SetNoWoundState( false );
		pOwner->SetEnableChangeTeam( false );

		m_dwEnableComboTime_E = dwStartTime + pGrp->GetAnimationFullTime( iAniID ) * fTimeRate;
		m_dwEnableComboTime_S = dwStartTime + CheckKeyReserveTime( pOwner, iAniID, fTimeRate );		

		if( pTarget )
		{
			D3DXVECTOR3 vAimedDir = pTarget->GetMidPositionByRate() - pOwner->GetMidPositionByRate();
			D3DXVec3Normalize( &vAimedDir, &vAimedDir );
			float fWeight = pSuddenItem->GetAimedUpDownWeight( vAimedDir, 85.0f );
			if( vAimedDir.y >= 0.0f )
			{
				AimedFireToUp( pGrp, fWeight, fTimeRate, 0, true, true, m_stExtend2_Aim_Up_Fire, m_stExtend2_Aim_Center_Fire );
			}
			else
			{
				AimedFireToDown( pGrp, fWeight, fTimeRate, 0, true, true, m_stExtend2_Aim_Down_Fire, m_stExtend2_Aim_Center_Fire );
			}
		}
		else
			pGrp->SetActionAni( pAttr->m_AttackAnimation, 0.0f, 0.0f, FLOAT1, FLOAT1/fTimeRate, 0, 0, true );
	}

	pOwner->SetJumpAttackState();
	m_iCurComboCnt++;

	pOwner->SetTargetMarkerState( ioTargetMarker::MS_NONE );

	if( pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_EXTEND_JUMP );
		kPacket << pOwner->GetCharName();
		kPacket << (int)GetType();
		kPacket << pOwner->GetHP().m_fCurValue;
		kPacket << m_ChargeComboState;
		kPacket << m_iCurComboCnt;
		kPacket << pOwner->GetTargetRot();
		kPacket << pOwner->GetMoveDir();
		kPacket << pOwner->GetInputDir();
		pOwner->FillAutoTargetInfo( kPacket );
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

void ioSuddenAttackJump::SetNormalModeAttack( ioBaseChar *pOwner )
{
	int iMaxCnt = m_ComboAttackList.size();
	if( !COMPARE( m_iCurComboCnt, 0, iMaxCnt ) )
		return;

	DWORD dwCurTime = FRAMEGETTIME();
	m_ChargeComboState = CCS_NORMAL;

	pOwner->ClearAttackFireTimeAndSkill();

	if( pOwner->IsNeedProcess() && pOwner->IsCanRotateJumpAttack() )
	{
		ioUserKeyInput::DirKeyInput eDir = pOwner->CheckCurDirKey();
		if( eDir != ioUserKeyInput::DKI_NONE )
		{
			float fYawD = 180.0f + 45.0f * ( eDir - ioUserKeyInput::DKI_UP );
			fYawD = ioMath::ArrangeHead( fYawD );

			float fYawR = DEGtoRAD( fYawD );
			D3DXQUATERNION qtRotate;
			D3DXQuaternionRotationYawPitchRoll( &qtRotate, fYawR, 0.0f, 0.0f );

			pOwner->SetTargetRotToRotate( qtRotate, false, false );
			pOwner->SetInputDirection( pOwner->GetMoveDir() );
		}
	}

	ioEntityGroup *pGrp = pOwner->GetGroup();
	pGrp->ClearAllActionAni( FLOAT100, true );
	pGrp->ClearAllLoopAni( FLOAT100, true );

	if( pOwner && !pOwner->IsActionStop() )
		pOwner->InitActionStopDelay( true );
	else
		pOwner->InitActionStopDelay( false );

	int iAniID = pGrp->GetAnimationIdx( m_ComboAttackList[m_iCurComboCnt].m_AttackAnimation );
	float fTimeRate = m_ComboAttackList[m_iCurComboCnt].m_fAttackAniRate;
	DWORD dwPreDelay = m_ComboAttackList[m_iCurComboCnt].m_dwPreDelay;

	DWORD dwStartTime = dwCurTime + dwPreDelay;
	m_dwEnableComboTime_E = dwStartTime + pGrp->GetAnimationFullTime( iAniID ) * fTimeRate;

	pOwner->SetAutoTarget( ATT_JUMP );

	pGrp->SetActionAni( iAniID, FLOAT100, FLOAT100, FLOAT1, FLOAT1/fTimeRate, 0.0f, dwPreDelay, true );		
	pOwner->RefreshFireTimeList( iAniID, m_ComboAttackList[m_iCurComboCnt].m_vWeaponInfoList,
		FTT_JUMP_ATTACK,
		fTimeRate,
		dwPreDelay );

	pOwner->IncreaseWeaponIndexBase();

	DWORD dwTrackingTime = dwStartTime;
	dwTrackingTime += pGrp->GetAnimationEventTime_e( iAniID, "tracking_e" ) * fTimeRate;

	m_dwEnableComboTime_S = dwStartTime + CheckKeyReserveTime( pOwner, iAniID, fTimeRate );

	pOwner->SetAutoTargetTrackingEndTime( dwTrackingTime );

	// For AirJumpPower
	SetAirJump( pOwner, iAniID, fTimeRate );

	if( pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_EXTEND_JUMP );
		kPacket << pOwner->GetCharName();
		kPacket << (int)GetType();
		kPacket << pOwner->GetHP().m_fCurValue;
		kPacket << m_ChargeComboState;
		kPacket << m_iCurComboCnt;
		kPacket << pOwner->GetTargetRot();
		kPacket << pOwner->GetMoveDir();
		kPacket << pOwner->GetInputDir();
		pOwner->FillAutoTargetInfo( kPacket );
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}

	m_bReserveJumpAttack = false;
	m_bUseExtendJump = true;
	pOwner->SetEnableHide( false );
	pOwner->SetNoWoundState( false );
	pOwner->SetEnableChangeTeam( false );

	pOwner->SetChangeJumpMotion( false, "", "", "", 0.0f );

	m_iCurComboCnt++;
	pOwner->SetJumpAttackState();
}


// double
void ioSuddenAttackJump::SetDoubleJump( ioBaseChar *pOwner )
{
	if( !pOwner ) return;

	m_ChargeComboState = CCS_DOUBLE;

	m_bUsedDoubleJump = true;
	m_bUseExtendJump = true;
	m_iCurJumpCnt++;

	pOwner->SetCurJumpPowerAmt( 0.0f );
	pOwner->SetForcePowerAmt( 0.0f );
	pOwner->SetGravityAmt( 0.0f );
	pOwner->SetJumpPower( m_fDoubleJumpPower );

	D3DXVECTOR3 vJumpDir = pOwner->GetMoveDir();
	if( m_fDoubleJumpForce > 0.0f )
		pOwner->SetForcePower( vJumpDir, fabs(m_fDoubleJumpForce), FLOAT1, true );
	else
		pOwner->SetForcePower( -vJumpDir, fabs(m_fDoubleJumpForce), FLOAT1, true );

	pOwner->SetEnableHide( false );
	pOwner->SetEnableChangeTeam( false );
	pOwner->SetChangeJumpMotion( false, "", "", "", 0.0f );

	g_TutorialMgr.ActionDoubleJump();

	SetDoubleJumpAni( pOwner );

	if( pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_DOUBLE_JUMP );
		kPacket << pOwner->GetCharName();
		kPacket << (int)GetType();
		kPacket << vJumpDir;
		kPacket << m_ChargeComboState;
		kPacket << m_fDoubleJumpPower;
		kPacket << m_fDoubleJumpForce;
		kPacket << pOwner->GetTargetRot();
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

bool ioSuddenAttackJump::IsCanDoubleJump( ioBaseChar *pOwner, float fHeightGap )
{
	if( !pOwner ) return false;
	if( !pOwner->IsNeedProcess() ) return false;
	if( !m_bEnableDoubleJump ) return false;

	if( !pOwner->IsJumpKeyPress() ||
		pOwner->IsHasCrown() ||
		pOwner->IsCatchMode() ||
		pOwner->IsPrisonerMode() ||
		(pOwner->GetUsedBuffJump() != BJUT_NONE) ||
		(pOwner->GetUsedBuffMultiFlyJump() != BJUT_NONE) ||
		(pOwner->GetUsedBuffFlyJump() != BJUT_NONE) )
		return false;

	if( pOwner->HasBuff( BT_BUFF_FLY_JUMP ) || pOwner->HasBuff( BT_BUFF_FLY_JUMP2 ) )
		return false;

	if( pOwner->IsBuffLimitExtendJump() )
		return false;

	if( m_iCurJumpCnt >= m_iMaxJumpCnt )
		return false;

	switch( m_ChargeComboState )
	{
	case CCS_NONE:
	case CCS_DOUBLE:
		break;
	default:
		return false;
	}

	DWORD dwMoveStartTime = pOwner->GetJumpMoveStartTime();
	if( dwMoveStartTime == 0 ) return false;

	if( FRAMEGETTIME() - dwMoveStartTime < m_dwDoubleJumpEnableTime )
		return false;

	if( fHeightGap < m_fDoubleJumpEnableHeight )
		return false;

	return true;
}

void ioSuddenAttackJump::SetDoubleJumpAni( ioBaseChar *pOwner )
{
	ioEntityGroup *pGrp = pOwner->GetGroup();
	D3DXVECTOR3 vSightDir = pOwner->GetMoveDir();

	D3DXVECTOR3 vXZDir( vSightDir.x, 0.0f, vSightDir.z );
	D3DXVec3Normalize( &vXZDir, &vXZDir );

	D3DXQUATERNION qtTargetRot;
	ioMath::CalcDirectionQuaternion( qtTargetRot, -vXZDir );

	float fCurYaw, fTargetYaw;
	fTargetYaw = RADtoDEG( ioMath::QuaterToYaw( qtTargetRot ) );
	fTargetYaw = ioMath::ArrangeHead( fTargetYaw );
	fCurYaw = RADtoDEG( ioMath::QuaterToYaw( pOwner->GetTargetRot() ) );
	fCurYaw = ioMath::ArrangeHead( fCurYaw );

	float fAngleGap = ioMath::AngleGap( fCurYaw, fTargetYaw );
	fAngleGap += FLOAT360;
	int iTest = (int)fAngleGap % 360;

	pGrp->ClearAllActionAni( FLOAT100, true );
	pGrp->ClearAllLoopAni( FLOAT100, true );

	float fRate = 0.0f;
	int iIndex = -1;
	int iIndex2 = -1;
	iIndex = iTest / 90;
	iTest = iTest % 90;

	int iCnt = m_vDoubleJumpAniList.size();
	if( COMPARE( iIndex, 0, iCnt ) )
	{
		fRate = (float)iTest / FLOAT90;
		iIndex2 = (iIndex+1) % iCnt;

		if( !m_vDoubleJumpAniList[iIndex].IsEmpty() && !m_vDoubleJumpAniList[iIndex2].IsEmpty() )
		{
			if( fRate <= FLOAT05 )
				pGrp->SetActionAni( m_vDoubleJumpAniList[iIndex], FLOAT100, FLOAT100, (FLOAT1 - fRate), FLOAT1/m_fDoubleJumpAniRate, 0.0f, 0, true );
			else
				pGrp->SetActionAni( m_vDoubleJumpAniList[iIndex2], FLOAT100, FLOAT100, fRate, FLOAT1/m_fDoubleJumpAniRate, 0.0f, 0, true );
		}
		else if( !m_vDoubleJumpAniList[iIndex].IsEmpty() && m_vDoubleJumpAniList[iIndex2].IsEmpty() )
		{
			pGrp->SetActionAni( m_vDoubleJumpAniList[iIndex], FLOAT100, FLOAT100, FLOAT1, FLOAT1/m_fDoubleJumpAniRate, 0.0f, 0, true );
		}
		else if( !m_vDoubleJumpAniList[iIndex2].IsEmpty() && m_vDoubleJumpAniList[iIndex].IsEmpty() )
		{
			pGrp->SetActionAni( m_vDoubleJumpAniList[iIndex2], FLOAT100, FLOAT100, FLOAT1, FLOAT1/m_fDoubleJumpAniRate, 0.0f, 0, true );
		}
		else
		{
			int iAniID = pOwner->GetJumppingAnimationIdx();
			pGrp->SetActionAni( iAniID, FLOAT100, FLOAT100, FLOAT1, FLOAT1/m_fDoubleJumpAniRate, 0.0f, 0, true );
		}
	}
	else
	{
		int iAniID = pOwner->GetJumppingAnimationIdx();
		pGrp->SetActionAni( iAniID, FLOAT100, FLOAT100, FLOAT1, FLOAT1/m_fDoubleJumpAniRate, 0.0f, 0, true );
	}
}

// flash
bool ioSuddenAttackJump::IsCanFlashJumpAttack( ioBaseChar *pOwner )
{
	if( !pOwner ) return false;
	if( !m_bEnableFlashJump ) return false;

	if( m_ChargeComboState == CCS_DOUBLE )
		return true;

	return false;
}

bool ioSuddenAttackJump::SetFlashJumpAttack( ioBaseChar *pOwner )
{
	if( !pOwner ) return false;

	ioEntityGroup *pGrp = pOwner->GetGroup();
	if( !pGrp ) return false;

	DWORD dwDelayTime = 0;
	DWORD dwCurTime = FRAMEGETTIME();

	if( pOwner->IsNeedProcess() && pOwner->IsCanRotateJumpAttack() )
	{
		ioUserKeyInput::DirKeyInput eDir = pOwner->CheckCurDirKey();
		if( eDir != ioUserKeyInput::DKI_NONE )
		{
			float fYawD = 180.0f + 45.0f * ( eDir - ioUserKeyInput::DKI_UP );
			fYawD = ioMath::ArrangeHead( fYawD );

			float fYawR = DEGtoRAD( fYawD );
			D3DXQUATERNION qtRotate;
			D3DXQuaternionRotationYawPitchRoll( &qtRotate, fYawR, 0.0f, 0.0f );

			pOwner->SetTargetRotToRotate( qtRotate, false, false );
			pOwner->SetInputDirection( pOwner->GetMoveDir() );
		}
	}

	if( !m_FlashDelayAni.IsEmpty() )
	{
		pGrp->ClearAllActionAni( FLOAT100, true );
		pGrp->ClearAllLoopAni( FLOAT100, true );

		pOwner->SetAutoTarget( ATT_JUMP );

		int iAniID = pGrp->GetAnimationIdx( m_FlashDelayAni );
		pGrp->SetActionAni( iAniID, FLOAT100, FLOAT100, FLOAT1, FLOAT1/m_fFlashDelayRate );

		dwDelayTime = pGrp->GetAnimationFullTime( iAniID ) * m_fFlashDelayRate;

		DWORD dwTrackingTime = dwCurTime;
		dwTrackingTime += pGrp->GetAnimationEventTime_e( iAniID, "tracking_e" ) * m_fFlashDelayRate;

		pOwner->SetAutoTargetTrackingEndTime( dwTrackingTime );
	}

	m_bUseExtendJump = true;
	pOwner->SetUsedFlashJump( true );

	pOwner->SetEnableHide( false );
	pOwner->SetNoWoundState( false );
	pOwner->SetEnableChangeTeam( false );

	pOwner->SetChangeJumpMotion( false, "", "", "", 0.0f );
	pOwner->SetJumpAttackState();

	pOwner->SetGravityAmt( 0.0f );
	pOwner->SetForcePowerAmt( 0.0f );
	pOwner->SetCurJumpPowerAmt( 0.0f );

	m_ChargeComboState = CCS_DELAY;
	m_dwFlashDelayEndTime = dwCurTime + dwDelayTime;

	if( pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_EXTEND_JUMP );
		kPacket << pOwner->GetCharName();
		kPacket << (int)GetType();
		kPacket << pOwner->GetHP().m_fCurValue;
		kPacket << m_ChargeComboState;
		kPacket << m_iCurJumpCnt;
		kPacket << pOwner->GetTargetRot();
		kPacket << pOwner->GetMoveDir();
		kPacket << pOwner->GetInputDir();
		pOwner->FillAutoTargetInfo( kPacket );
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}

	return true;
}

// jump dash
bool ioSuddenAttackJump::IsCanJumpDash( ioBaseChar *pOwner, float fHeightGap )
{
	if( !pOwner ) return false;
	if( !m_bEnableJumpDash ) return false;

	if( !pOwner->IsNeedProcess() ) return false;

	if( pOwner->IsHasCrown() ||
		pOwner->IsCatchMode() ||
		pOwner->IsPrisonerMode() ||
		(pOwner->GetUsedBuffJump() != BJUT_NONE) ||
		(pOwner->GetUsedBuffFlyJump() != BJUT_NONE) ||
		(pOwner->GetUsedBuffMultiFlyJump() != BJUT_NONE) ||
		pOwner->GetUsedJumpAttack()	)
		return false;

	if( pOwner->IsBuffLimitExtendJump() )
		return false;

	if( pOwner->HasBuff(BT_AIR_ATTACK) )
		return false;

	if( m_ChargeComboState != CCS_NONE )
		return false;

	DWORD dwMoveStartTime = pOwner->GetJumpMoveStartTime();
	if( dwMoveStartTime == 0 ) return false;

	if( !pOwner->IsDirKeyDoubleClick() )
		return false;

	if( /*m_nWeaponItemMode == ioSuddenAttackItem::IMT_EXTRA1 ||*/ m_nWeaponItemMode == ioSuddenAttackItem::IMT_EXTRA2 )
		return false;

	return true;
}

bool ioSuddenAttackJump::IsJumpDashState()
{
	switch( m_ChargeComboState )
	{
	case CCS_DASH:
	case CCS_DASH_END:
		return true;
	}

	return false;
}

void ioSuddenAttackJump::ProcessJumpAttack( ioBaseChar *pOwner, float fHeightGap )
{
	if( !pOwner ) return;

	ioPlayStage *pStage = pOwner->GetCreator();
	if( !pStage )
		return;

	switch( m_ChargeComboState )
	{
	case CCS_NORMAL:
		if( IsCanDoubleJump( pOwner, fHeightGap ) )
		{
			if( m_fDoubleJumpPower >= 0.0f )
			{
				SetDoubleJump( pOwner );
				return;
			}
		}

		ProcessComboJumpAttack( pOwner );
		break;
	case CCS_CHARGE:
		ProcessCharging( pOwner );
		break;
	case CCS_EXTEND_ATTACK:
		PowerChargeMove( pOwner, pStage );
		break;
	case CCS_DELAY:
		ProcessDelay( pOwner );
		break;
	case CCS_FLASH:
		ProcessFlashMove( pOwner );
		break;
	case CCS_NONE:
		CheckAirJump( pOwner );
		break;
	case CCS_EXTEDN2_AIM:
		ProcessAimExtend2( pOwner );
		break;
	}
}


void ioSuddenAttackJump::ProcessAimExtend2( ioBaseChar *pOwner )
{
	//if( !pOwner->IsNeedProcess() )
		//return;

	ioAttackableItem *pItem = pOwner->GetPriorityItem( SPT_ATTACK );
	ioSuddenAttackItem *pSuddenItem = ToSuddenAttackItem( pItem );
	if( !pSuddenItem )
		return;

	if( pOwner->IsNeedProcess() && !pOwner->IsAttackKeyDown() )
	{
		//pSuddenItem->CheckAimTarget( pOwner );

		//���� ������
		//pSuddenItem->ProcessTargetTracking_Extend_Delay( pOwner, m_stExtend2_Aim_Up, m_stExtend2_Aim_Center, m_stExtend2_Aim_Down );

		//��ü
		//pSuddenItem->UpdateAimDir( pOwner );

		SetComboJumpAttack( pOwner );
		return;
	}

	pSuddenItem->CheckAimTarget( pOwner );

	//���� ������
	pSuddenItem->ProcessTargetTracking_Extend_Delay( pOwner, m_stExtend2_Aim_Up, m_stExtend2_Aim_Center, m_stExtend2_Aim_Down );

	//��ü
	pSuddenItem->UpdateAimDir( pOwner );
}


void ioSuddenAttackJump::ProcessComboJumpAttack( ioBaseChar *pOwner )
{
	DWORD dwCurTime = FRAMEGETTIME();
	DWORD dwLooseTime = 0;

	if( pOwner )
		dwLooseTime = pOwner->GetActionStopTotalDelay();

	CheckAirJump( pOwner );

	if( m_bReserveJumpAttack )
	{
		DWORD dwLooseTime = 0;

		if( pOwner )
			dwLooseTime = pOwner->GetActionStopTotalDelay();

		if( dwCurTime >= m_dwEnableComboTime_S+dwLooseTime )
		{
			SetChargingState( pOwner );
		}
	}
}

void ioSuddenAttackJump::ProcessDelay( ioBaseChar *pOwner )
{
	if( !pOwner ) return;

	ioEntityGroup *pGrp = pOwner->GetGroup();
	if( !pGrp ) return;

	DWORD dwCurTime = FRAMEGETTIME();

	if( m_dwFlashDelayEndTime > 0 && m_dwFlashDelayEndTime < dwCurTime )
	{
		m_ChargeComboState = CCS_FLASH;

		int iAniID = pGrp->GetAnimationIdx( m_FlashJumpAttack.m_AttackAnimation );
		float fTimeRate = m_FlashJumpAttack.m_fAttackAniRate;
		DWORD dwPreDelay = m_FlashJumpAttack.m_dwPreDelay;

		pGrp->ClearAllActionAni( FLOAT100, true );
		pGrp->ClearAllLoopAni( FLOAT100, true );

		pGrp->SetActionAni( iAniID, FLOAT100, FLOAT100, FLOAT1, FLOAT1/fTimeRate, 0.0f, dwPreDelay, true );		

		pOwner->RefreshFireTimeList( iAniID, m_FlashJumpAttack.m_vWeaponInfoList,
									 FTT_JUMP_ATTACK,
									 fTimeRate,
									 dwPreDelay );

		pOwner->IncreaseWeaponIndexBase();

		m_dwFlashEndTime = dwCurTime + dwPreDelay + pGrp->GetAnimationFullTime( iAniID ) * fTimeRate;

		pOwner->CheckCharColSkipTime( iAniID, fTimeRate, dwPreDelay );
	}
}

void ioSuddenAttackJump::ProcessFlashMove( ioBaseChar *pOwner )
{
	if( !pOwner ) return;

	ioEntityGroup *pGrp = pOwner->GetGroup();
	if( !pGrp ) return;

	DWORD dwCurTime = FRAMEGETTIME();

	if( m_dwFlashEndTime > 0 && m_dwFlashEndTime < dwCurTime )
	{
		m_ChargeComboState = CCS_FLASH_END;

		pOwner->SetUsedFlashJump( false );
		pOwner->SetGravityAmt( 0.0f );

		pOwner->SetCharCollisionSkipTime( 0, 0 );
		pOwner->SetMoveDirByRotate( pOwner->GetTargetRot() );
	}
}



void ioSuddenAttackJump::ProcessJumpDash( ioBaseChar *pOwner )
{
	if( !pOwner ) return;

	switch( m_ChargeComboState )
	{
	case CCS_DASH:
	case CCS_DASH_END:
		break;
	default:
		return;
	}

	if( m_ComboDashType != CDT_NO_GRAVITY )
		return;

	DWORD dwCurTime = FRAMEGETTIME();

	switch( m_ChargeComboState )
	{
	case CCS_DASH:
		if( m_dwJumpDashEndTime > 0 && m_dwJumpDashEndTime < dwCurTime )
		{
			m_dwJumpDashEndTime = 0;
			m_ChargeComboState = CCS_DASH_END;

			int iAniID = pOwner->GetJumppingAnimationIdx();
			ioEntityGroup *pGrp = pOwner->GetGroup();
			if( pGrp )
			{
				pGrp->ClearAllActionAni( FLOAT100, true );
				pGrp->ClearAllLoopAni( FLOAT100, true );
				pGrp->SetActionAni( iAniID,	0.0f, 0.0f, FLOAT1, FLOAT1,	0.0f, 0.0f,	true );
			}

			if( pOwner->IsNeedSendNetwork() )
			{
				SP2Packet kPacket( CUPK_JUMP_DASH );
				kPacket << pOwner->GetCharName();
				kPacket << (int)GetType();
				kPacket << pOwner->GetMoveDir();
				kPacket << m_ChargeComboState;
				kPacket << pOwner->GetTargetRot();
				P2PNetwork::SendToAllPlayingUser( kPacket );
			}
		}
		break;
	case CCS_DASH_END:
		break;
	}
}

void ioSuddenAttackJump::ApplyJump( ioBaseChar *pOwner, SP2Packet &rkPacket )
{
	if( !pOwner ) return;

	ioEntityGroup *pGrp = pOwner->GetGroup();
	if( !pGrp ) return;

	DWORD dwDelayTime = 0;
	DWORD dwCurTime = FRAMEGETTIME();

	int iType;
	rkPacket >> iType;

	//LOG.PrintTimeAndLog( 0, "ApplyJump - (%d)", iType );

	switch( iType )
	{
	case CCS_EXTEDN2_AIM:
		{
			SetAimingStateExtend2( pOwner );
		}
		break;
	case CCS_NORMAL:
		{
			rkPacket >> m_iCurComboCnt;

			D3DXQUATERNION qtRot;
			rkPacket >> qtRot;

			D3DXVECTOR3 vMoveDir, vInputDir;
			rkPacket >> vMoveDir >> vInputDir;

			pOwner->SetJumpAttckTargetRot( qtRot, vMoveDir, vInputDir );

			if( m_nWeaponItemMode == ioSuddenAttackItem::IMT_NORMAL )
			{
				if( !COMPARE( m_iCurComboCnt, 0, (int) m_ComboAttackList.size() ) )
					return;
			}
			else
			if( m_nWeaponItemMode == ioSuddenAttackItem::IMT_EXTRA1 )
			{
				SetExtend1ModeAttack( pOwner );
				return;
			}else if( m_nWeaponItemMode == ioSuddenAttackItem::IMT_EXTRA2 )
			{
				SetExtend2ModeAttack( pOwner );
				return;
			}
			else
				return;

			m_ChargeComboState = CCS_NORMAL;

			pGrp->ClearAllActionAni( FLOAT100, true );
			pGrp->ClearAllLoopAni( FLOAT100, true );

			if( pOwner && !pOwner->IsActionStop() )
				pOwner->InitActionStopDelay( true );
			else
				pOwner->InitActionStopDelay( false );

			int iAniID = pGrp->GetAnimationIdx( m_ComboAttackList[m_iCurComboCnt].m_AttackAnimation );
			float fTimeRate = m_ComboAttackList[m_iCurComboCnt].m_fAttackAniRate;
			DWORD dwPreDelay = m_ComboAttackList[m_iCurComboCnt].m_dwPreDelay;
			m_dwEnableComboTime_E = dwCurTime + pGrp->GetAnimationFullTime( iAniID ) * fTimeRate;

			pOwner->SetAutoTarget( ATT_JUMP );

			pGrp->SetActionAni( iAniID, FLOAT100, FLOAT100, FLOAT1, FLOAT1/fTimeRate, 0.0f, dwPreDelay, true );
			pOwner->RefreshFireTimeList( iAniID,
				m_ComboAttackList[m_iCurComboCnt].m_vWeaponInfoList,
				FTT_JUMP_ATTACK,
				fTimeRate,
				dwPreDelay );

			pOwner->IncreaseWeaponIndexBase();

			DWORD dwStartTime = dwCurTime + dwPreDelay;
			DWORD dwTrackingTime = dwStartTime;
			dwTrackingTime += pGrp->GetAnimationEventTime_e( iAniID, "tracking_e" ) * fTimeRate;

			m_dwEnableComboTime_S = dwStartTime + CheckKeyReserveTime( pOwner, iAniID, fTimeRate );

			pOwner->SetAutoTargetTrackingEndTime( dwTrackingTime );

			// For AirJumpPower
			SetAirJump( pOwner, iAniID, fTimeRate );

			pOwner->ApplyAutoTargetInfo( rkPacket );

			m_bReserveJumpAttack = false;
			m_bUseExtendJump = true;
			pOwner->SetEnableHide( false );
			pOwner->SetNoWoundState( false );
			pOwner->SetEnableChangeTeam( false );

			pOwner->SetChangeJumpMotion( false, "", "", "", 0.0f );

			m_iCurComboCnt++;
			pOwner->SetJumpAttackState();
		}
		break;
	case CCS_DOUBLE:
		{
			float fJumpPower, fJumpForce;
			D3DXQUATERNION qtRotate;

			rkPacket >> fJumpPower >> fJumpForce;
			rkPacket >> qtRotate;

			if( fJumpPower >= 0.0f )
			{
				m_ChargeComboState = CCS_DOUBLE;

				m_bUsedDoubleJump = true;
				m_bUseExtendJump = true;

				pOwner->SetCurJumpPowerAmt( 0.0f );
				pOwner->SetForcePowerAmt( 0.0f );
				pOwner->SetGravityAmt( 0.0f );

				pOwner->SetTargetRot( qtRotate );
				pOwner->SetJumpPower( fJumpPower );

				D3DXVECTOR3 vJumpDir = pOwner->GetMoveDir();
				if( m_fDoubleJumpForce > 0.0f )
					pOwner->SetForcePower( vJumpDir, fabs(m_fDoubleJumpForce), FLOAT1, true );
				else
					pOwner->SetForcePower( -vJumpDir, fabs(m_fDoubleJumpForce), FLOAT1, true );

				SetDoubleJumpAni( pOwner );
			}
		}
		break;
	case CCS_DELAY:
		{
			rkPacket >> m_iCurJumpCnt;

			D3DXQUATERNION qtRot;
			rkPacket >> qtRot;

			D3DXVECTOR3 vMoveDir, vInputDir;
			rkPacket >> vMoveDir >> vInputDir;

			pOwner->SetJumpAttckTargetRot( qtRot, vMoveDir, vInputDir );

			if( !m_FlashDelayAni.IsEmpty() )
			{
				pGrp->ClearAllActionAni( FLOAT100, true );
				pGrp->ClearAllLoopAni( FLOAT100, true );

				pOwner->SetAutoTarget( ATT_JUMP );

				int iAniID = pGrp->GetAnimationIdx( m_FlashDelayAni );
				pGrp->SetActionAni( iAniID, FLOAT100, FLOAT100, FLOAT1/m_fFlashDelayRate );

				dwDelayTime = pGrp->GetAnimationFullTime( iAniID ) * m_fFlashDelayRate;

				DWORD dwTrackingTime = dwCurTime;
				dwTrackingTime += pGrp->GetAnimationEventTime_e( iAniID, "tracking_e" ) * m_fFlashDelayRate;

				pOwner->SetAutoTargetTrackingEndTime( dwTrackingTime );
			}

			m_bUseExtendJump = true;
			pOwner->SetUsedFlashJump( true );

			pOwner->SetEnableHide( false );
			pOwner->SetNoWoundState( false );
			pOwner->SetEnableChangeTeam( false );

			pOwner->SetChangeJumpMotion( false, "", "", "", 0.0f );
			pOwner->SetJumpAttackState();

			pOwner->SetGravityAmt( 0.0f );
			pOwner->SetForcePowerAmt( 0.0f );
			pOwner->SetCurJumpPowerAmt( 0.0f );

			m_ChargeComboState = CCS_DELAY;
			m_dwFlashDelayEndTime = dwCurTime + dwDelayTime;

			pOwner->ApplyAutoTargetInfo( rkPacket );
		}
		break;
	case CCS_DASH:
		{
			D3DXQUATERNION qtRotate;
			rkPacket >> qtRotate;

			pOwner->SetTargetRotToRotate( qtRotate, false );

			ioEntityGroup *pGrp = pOwner->GetGroup();
			m_ChargeComboState = CCS_DASH;
			m_bUseExtendJump = true;

			pOwner->SetCurJumpPowerAmt( 0.0f );
			pOwner->SetForcePowerAmt( 0.0f );
			pOwner->SetGravityAmt( 0.0f );

			int iAniID = pGrp->GetAnimationIdx( m_DashJumpAttack.m_AttackAnimation );
			float fTimeRate = m_DashJumpAttack.m_fAttackAniRate;
			DWORD dwPreDelay = m_DashJumpAttack.m_dwPreDelay;
			DWORD dwEndDelay = m_DashJumpAttack.m_dwEndDelay;

			pOwner->SetEnableHide( false );
			pOwner->SetEnableChangeTeam( false );
			pOwner->SetChangeJumpMotion( false, "", "", "", 0.0f );

			pGrp->ClearAllActionAni( FLOAT100, true );
			pGrp->ClearAllLoopAni( FLOAT100, true );

			pOwner->SetAutoTarget( ATT_JUMP );
			pGrp->SetActionAni( iAniID, FLOAT100, FLOAT100, FLOAT1, FLOAT1/fTimeRate, 0.0f, dwPreDelay, true );

			pOwner->RefreshFireTimeList( iAniID,
										 m_DashJumpAttack.m_vWeaponInfoList,
										 FTT_JUMP_ATTACK,
										 fTimeRate,
										 dwPreDelay );

			pOwner->IncreaseWeaponIndexBase();
			//
			m_dwJumpDashStartTime = dwCurTime;
			m_dwJumpDashEndTime = dwCurTime + dwPreDelay + dwEndDelay + pGrp->GetAnimationFullTime( iAniID ) * fTimeRate;

			pOwner->CheckCharColSkipTime( iAniID, fTimeRate, dwPreDelay );

			pOwner->SetReservedSliding( m_DashJumpAttack.m_vForceInfoList, iAniID, fTimeRate, dwPreDelay );

			DWORD dwTrackingTime = dwCurTime;
			dwTrackingTime += pGrp->GetAnimationEventTime_e( iAniID, "tracking_e" ) * fTimeRate;
			pOwner->SetAutoTargetTrackingEndTime( dwTrackingTime );

			pOwner->ApplyAutoTargetInfo( rkPacket );

			if( m_bUseNewJumpTypeDash )
			{
				DWORD dwKeyReserveTime = dwCurTime + CheckKeyReserveTime( pOwner, iAniID, fTimeRate );
				SetNewJumpTypeDashAttack( pOwner, m_dwJumpDashEndTime, dwKeyReserveTime );
			}
		}
		break;
	case CCS_DASH_END:
		{
			D3DXQUATERNION qtRotate;
			rkPacket >> qtRotate;

			pOwner->SetTargetRotToRotate( qtRotate, false );

			m_dwJumpDashEndTime = 0;
			m_ChargeComboState = CCS_DASH_END;

			int iAniID = pOwner->GetJumppingAnimationIdx();
			ioEntityGroup *pGrp = pOwner->GetGroup();
			if( pGrp )
			{
				pGrp->ClearAllActionAni( FLOAT100, true );
				pGrp->ClearAllLoopAni( FLOAT100, true );
				pGrp->SetActionAni( iAniID,	0.0f, 0.0f, FLOAT1, FLOAT1,	0.0f, 0.0f,	true );
			}
		}
		break;
	case CCS_CHARGE:
		{
			D3DXQUATERNION qtRot;
			rkPacket >> qtRot;

			pOwner->SetTargetRotAndMoveDirChange( qtRot );

			SetChargingState( pOwner );
		}
		break;
	case CCS_EXTEND_ATTACK:
		{
			D3DXQUATERNION qtRot;
			rkPacket >> qtRot;

			pOwner->SetTargetRotAndMoveDirChange( qtRot );

			ChangeToPowerChargeJumpAttack( pOwner );

			pOwner->ApplyAutoTargetInfo( rkPacket );
		}
		break;
	}
}

void ioSuddenAttackJump::SetLandingState( ioBaseChar *pOwner )
{
	m_bUseExtendJump = false;
	m_bUsedChargeAttack = false;
	m_bReserveJumpAttack = false;

	m_ChargeComboState = CCS_NONE;
	pOwner->SetUsedFlashJump( false );

	if( m_GuidEffectID != -1 )
	{
		pOwner->EndMapEffect( m_GuidEffectID, false );
		m_GuidEffectID = -1;
	}
}

float ioSuddenAttackJump::GetCurFlashForceAmt()
{
	switch( m_ChargeComboState )
	{
	case CCS_FLASH:
		return m_fFlashForce;
	}

	return 0.0f;
}

float ioSuddenAttackJump::GetCurFlashGravityAmt()
{
	switch( m_ChargeComboState )
	{
	case CCS_FLASH:
		return m_fFlashGravityAmt;
	}

	return 0.0f;
}

float ioSuddenAttackJump::GetExtraJumpSpeedRate( const D3DXVECTOR3 &vDir, const D3DXQUATERNION &qtRot ) const
{
	D3DXVECTOR3 vCurDir = vDir;

	D3DXVECTOR3 vXZDir( vCurDir.x, 0.0f, vCurDir.z );
	D3DXVec3Normalize( &vXZDir, &vXZDir );

	D3DXQUATERNION qtTargetRot;
	ioMath::CalcDirectionQuaternion( qtTargetRot, -vXZDir );

	float fCurYaw, fTargetYaw;
	fTargetYaw = RADtoDEG( ioMath::QuaterToYaw( qtTargetRot ) );
	fTargetYaw = ioMath::ArrangeHead( fTargetYaw );
	fCurYaw = RADtoDEG( ioMath::QuaterToYaw( qtRot ) );
	fCurYaw = ioMath::ArrangeHead( fCurYaw );

	float fAngleGap = ioMath::AngleGap( fCurYaw, fTargetYaw );
	fAngleGap += FLOAT360;

	int iTest = (int)fAngleGap % 360;
	iTest /= 45;

	if( m_ChargeComboState == CCS_DOUBLE )
	{
		switch( iTest )
		{
		case 0:
		case 7:
			return m_fDoubleJumpSpeedRateFR;
		case 1:
		case 2:
		case 5:
		case 6:
			return m_fDoubleJumpSpeedRateSI;
		case 3:
		case 4:
			return m_fDoubleJumpSpeedRateBG;
		}
	}
	else if( m_bUseExtendJump )
	{
		switch( iTest )
		{
		case 0:
		case 7:
			return m_fJumpSpeedRateFR;
		case 1:
		case 2:
		case 5:
		case 6:
			return m_fJumpSpeedRateSI;
		case 3:
		case 4:
			return m_fJumpSpeedRateBG;
		}
	}

	return m_fJumpSpeedRate;
}

bool ioSuddenAttackJump::IsCanJumpingSkill( ioBaseChar *pOwner )
{
	switch( m_ChargeComboState )
	{
	case CCS_DELAY:
	case CCS_FLASH:
	case CCS_FLASH_END:
	case CCS_DASH:
	case CCS_DASH_END:
	case CCS_NEW_JUMP:
		return false;
	}

	return true;
}

DWORD ioSuddenAttackJump::GetChargingTime( ioBaseChar *pChar )
{
	if( pChar && pChar->IsBuffLimitExtendJump() )
		return 0;

	return m_dwJumpChargeTime;
}


void ioSuddenAttackJump::SetAimingStateExtend2( ioBaseChar *pOwner )
{
	if( !pOwner ) 
		return;

	ioEntityGroup *pGrp = pOwner->GetGroup();
	if( !pGrp ) 
		return;

	ioAttackableItem *pItem = pOwner->GetPriorityItem( SPT_ATTACK );
	ioSuddenAttackItem *pSuddenItem = ToSuddenAttackItem( pItem );
	if( !pSuddenItem )
		return;

	pGrp->ClearAllActionAni( FLOAT100, true );
	pGrp->ClearAllLoopAni( FLOAT100, true );

	pGrp->SetLoopAni( m_stExtend2_Aim_Up, 0.0f, 0.0f );
	pGrp->SetLoopAni( m_stExtend2_Aim_Down, 0.f, 0.0f );
	pGrp->SetLoopAni( m_stExtend2_Aim_Center, FLOAT100, FLOAT1 );

	m_ChargeComboState = CCS_EXTEDN2_AIM;
	pOwner->SetJumpAttackState();

	//if( pOwner->IsNeedProcess() )
	{
		const ioHashString& stTargetAiming = pSuddenItem->GetAimingMarker();
		const ioHashString& stTargetAimed = pSuddenItem->GetAimedMarker();
		const ioHashString& stTargetZeroAimed = pSuddenItem->GetZeroAimedMarker();
		const ioHashString& stTargetHeadAimed = pSuddenItem->GetHeadAimedMarker();
		float fTargetMaxRange = pSuddenItem->GetExtend2Range();
		pOwner->SetTargetMarker( ioTargetMarker::MT_OTHER_MARKER, 
			stTargetAiming, stTargetAiming, stTargetAimed, stTargetZeroAimed,stTargetHeadAimed );

		pOwner->SetTargetMarkerState( ioTargetMarker::MS_AIMING );
		pOwner->SetTargetMarkerRange( fTargetMaxRange );

		//pOwner->SetTargetMarkerTarget( m_AimedTarget, m_bAimedBall );
	}

	if( pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_EXTEND_JUMP );
		kPacket << pOwner->GetCharName();
		kPacket << (int)GetType();
		kPacket << pOwner->GetHP().m_fCurValue;
		kPacket << m_ChargeComboState;
		kPacket << pOwner->GetTargetRot();
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

void ioSuddenAttackJump::SetChargingState( ioBaseChar *pOwner )
{
	if( !pOwner ) 
		return;

	ioEntityGroup *pGrp = pOwner->GetGroup();
	if( !pGrp ) 
		return;

	if( m_ChargingAniList.empty() )
		return;

	ioAttackableItem *pItem = pOwner->GetPriorityItem( SPT_ATTACK );
	ioSuddenAttackItem *pSuddenItem = ToSuddenAttackItem( pItem );
	if( !pSuddenItem )
		return;

	pOwner->ClearAttackFireTimeAndSkill();

	DWORD dwCurTime = FRAMEGETTIME();
	ioHashString szCurChargingAni;

	int iChargingCnt = 0;

	if( pSuddenItem->GetMeshType() == ioSuddenAttackItem::IMT_NORMAL )
	{
		iChargingCnt = m_ChargingAniList.size();
		if( COMPARE( m_iCurComboCnt, 0, iChargingCnt ) )
			szCurChargingAni = m_ChargingAniList[m_iCurComboCnt];
		else
			szCurChargingAni = m_ChargingAniList[0];
	}
	else if( pSuddenItem->GetMeshType() == ioSuddenAttackItem::IMT_EXTRA1 )
	{
		szCurChargingAni = m_Extend1ChargingAni;
	}
	else if( pSuddenItem->GetMeshType() == ioSuddenAttackItem::IMT_EXTRA2 )
	{
		szCurChargingAni = m_Extend2ChargingAni;
	}

	pGrp->ClearAllLoopAni( FLOAT100, true );
	pGrp->ClearAllActionAni( FLOAT100, true );

	int iAniID = pGrp->GetAnimationIdx( szCurChargingAni );
	float fTimeRate = (float)m_dwChargingTime / pGrp->GetAnimationFullTime( iAniID );
	pGrp->SetActionAni( iAniID, FLOAT100, FLOAT100, FLOAT1, FLOAT1/fTimeRate, 0.0f, 0.0f, true );

	m_bUseExtendJump = true;

	pOwner->SetEnableHide( false );
	pOwner->SetNoWoundState( false );
	pOwner->SetEnableChangeTeam( false );

	pOwner->SetChangeJumpMotion( false, "", "", "", 0.0f );
	pOwner->SetJumpAttackState();

	m_ChargeComboState = CCS_CHARGE;
	m_dwChargingStartTime = dwCurTime;

	if( pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_EXTEND_JUMP );
		kPacket << pOwner->GetCharName();
		kPacket << (int)GetType();
		kPacket << pOwner->GetHP().m_fCurValue;
		kPacket << m_ChargeComboState;
		kPacket << pOwner->GetTargetRot();
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

void ioSuddenAttackJump::ProcessCharging( ioBaseChar *pOwner )
{
	if( !pOwner ) return;
	if( !pOwner->IsNeedProcess() )
		return;

	DWORD dwCurTime = FRAMEGETTIME();
	if( m_nWeaponItemMode == ioSuddenAttackItem::IMT_EXTRA1 )
	{
		SetComboJumpAttack( pOwner );
		return;
	}
	else if( m_nWeaponItemMode == ioSuddenAttackItem::IMT_EXTRA2 )
	{
		if( pOwner->IsAttackKeyDown() )
		{
			if( m_dwChargingStartTime+m_dwChargingTime < dwCurTime )
			{
				//ChangeToPowerChargeJumpAttack( pOwner );
				SetAimingStateExtend2( pOwner );
				return;
			}
		}
		else
		{
			SetComboJumpAttack( pOwner );
			return;
		}
	}
	else
	{
		/*if( pOwner->IsAttackKeyDown() && !pOwner->IsHasCrown() )
		{
			if( m_dwChargingStartTime+m_dwChargingTime < dwCurTime )
			{
				ChangeToPowerChargeJumpAttack( pOwner );
				return;
			}
		}
		else
		{*/
			SetComboJumpAttack( pOwner );
			return;
		//}
	}
}

int ioSuddenAttackJump::GetCurChargeComboState()
{
	return m_ChargeComboState;
}

void ioSuddenAttackJump::ChangeToPowerChargeJumpAttack( ioBaseChar *pOwner )
{
	ioEntityGroup *pGrp = pOwner->GetGroup();
	if( !pGrp ) return;

	const AttackAttribute *pAttr = pOwner->GetAttackAttribute( AT_EXTEND_JUMP_ATTACK );
	if( !pAttr ) return;

	DWORD dwCurTime = FRAMEGETTIME();

	if( pOwner->IsNeedProcess() && pOwner->IsCanRotateJumpAttack() )
	{
		ioUserKeyInput::DirKeyInput eDir = pOwner->CheckCurDirKey();
		if( eDir != ioUserKeyInput::DKI_NONE )
		{
			float fYawD = 180.0f + 45.0f * ( eDir - ioUserKeyInput::DKI_UP );
			fYawD = ioMath::ArrangeHead( fYawD );

			float fYawR = DEGtoRAD( fYawD );
			D3DXQUATERNION qtRotate;
			D3DXQuaternionRotationYawPitchRoll( &qtRotate, fYawR, 0.0f, 0.0f );

			pOwner->SetTargetRotToRotate( qtRotate, false, false );
			pOwner->SetInputDirection( pOwner->GetMoveDir() );
		}
	}

	pOwner->SetAutoTarget( ATT_JUMP );

	int iAniID = pGrp->GetAnimationIdx( pAttr->m_AttackAnimation );
	float fTimeRate = pAttr->m_fAttackAniRate;
	DWORD dwPreDelay = pAttr->m_dwPreDelay;

	if( fTimeRate <= 0.0f )
		fTimeRate = FLOAT1;

	pGrp->ClearAllActionAni( FLOAT100, true );
	pGrp->ClearAllLoopAni( FLOAT100, true );

	pGrp->SetActionAni( iAniID, FLOAT100, FLOAT100, FLOAT1, FLOAT1/fTimeRate, 0.0f, dwPreDelay, true );

	pOwner->RefreshFireTimeList( iAniID, pAttr->m_vWeaponInfoList, FTT_JUMP_ATTACK, fTimeRate, dwPreDelay );

	pOwner->IncreaseWeaponIndexBase();

	m_ChargeComboState = CCS_EXTEND_ATTACK;
	m_bUsedChargeAttack = true;

	// For AirJumpPower
	SetAirJump( pOwner, iAniID, fTimeRate );

	DWORD dwTrackingTime = dwCurTime;
	dwTrackingTime += pGrp->GetAnimationEventTime_e( iAniID, "tracking_e" ) * fTimeRate;

	pOwner->SetAutoTargetTrackingEndTime( dwTrackingTime );

	pOwner->CheckCharColSkipTime( iAniID, fTimeRate, dwPreDelay );

	//
	float fCurGravity = pOwner->GetCharGravity();
	float fFlightDuration = GetFlightDuration( fCurGravity );
	if( fFlightDuration > 0.0f )
	{
		m_fCurCharMoveSpeed = m_fChargeBaseRange / (fFlightDuration / FLOAT1000);
	}

	m_dwFlightTime = (DWORD)fFlightDuration;

	m_vCharMoveDir = ioMath::VEC3_ZERO;

	D3DXVECTOR3 vDir = pOwner->GetTargetRot() * D3DXVECTOR3( 0.0f, 0.0f, -FLOAT1 );
	vDir.y = 0.0f;
	D3DXVec3Normalize( &m_vCharMoveDir, &vDir );

	D3DXQUATERNION qtRot;
	ioMath::CalcDirectionQuaternion( qtRot, -m_vCharMoveDir );
	pOwner->SetTargetRotAndMoveDirChange( qtRot, false );
	pOwner->SetWorldOrientation( qtRot );
	pOwner->SetJumpPower( m_fChargeJumpPower );

	if( pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_EXTEND_JUMP );
		kPacket << pOwner->GetCharName();
		kPacket << (int)GetType();
		kPacket << pOwner->GetHP().m_fCurValue;
		kPacket << m_ChargeComboState;
		kPacket << pOwner->GetTargetRot();
		pOwner->FillAutoTargetInfo( kPacket );
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

float ioSuddenAttackJump::GetFlightDuration( float fCurGravity )
{
	float fTopTime, fTopHeight;
	fTopTime = fTopHeight = 0.0f;

	float fNewGravity = fCurGravity * m_fChargeGravityRate;
	if( fNewGravity > 0.0f )
	{
		fTopTime = m_fChargeJumpPower / fNewGravity;
		fTopHeight = (m_fChargeJumpPower*fTopTime) - (FLOAT05*fNewGravity*fTopTime*fTopTime);
	}

	float fResult = 2 * fTopTime * FLOAT1000;

	return fResult;
}

void ioSuddenAttackJump::PowerChargeMove( ioBaseChar *pChar, ioPlayStage *pStage )
{
	if( m_ChargeComboState != CCS_EXTEND_ATTACK )
		return;

	if( !pChar ) return;
	if( !pStage ) return;
	if( m_fCurCharMoveSpeed == 0.0f ) return;

	float fTimePerSec = g_FrameTimer.GetSecPerFrame();

	bool bCurMove = false;
	D3DXVECTOR3 vTotalMove( 0.0f, 0.0f, 0.0f );

	float fCurTimeGap = fTimePerSec * FLOAT1000;
	while( fCurTimeGap > 0.0f )
	{
		float fDivideTimePerSec = min( fCurTimeGap, MIN_FRAME_TIME ) * FLOAT0001;
		fCurTimeGap -= MIN_FRAME_TIME;

		D3DXVECTOR3 vMoveAmt = m_vCharMoveDir * m_fCurCharMoveSpeed * fDivideTimePerSec;
		if( vMoveAmt != ioMath::VEC3_ZERO )
		{
			vTotalMove += vMoveAmt;
			bCurMove = true;
		}
	}

	if( bCurMove )
	{
		bool bCol = false;
		if( pStage->ReCalculateMoveVectorByTerrain( pChar, &vTotalMove, bCol ) )
		{
			pChar->Translate( vTotalMove );
		}
	}

	D3DXVECTOR3 vPos = pChar->GetWorldPosition();
	if( pChar->CheckAxisLimit( &vPos ) )
		pChar->SetWorldPosition( vPos );
}

bool ioSuddenAttackJump::IsChargeAttackState()
{
	return m_bUsedChargeAttack;
}


void ioSuddenAttackJump::OnEndExtendJump( ioBaseChar *pOwner )
{
	if( !pOwner ) return;

	if( m_GuidEffectID != -1 )
	{
		pOwner->EndMapEffect( m_GuidEffectID, false );
		m_GuidEffectID = -1;
	}
}

void ioSuddenAttackJump::SetJumpDash( ioBaseChar *pOwner, float fHeightGap )
{
	if( !pOwner ) return;

	DWORD dwCurTime = FRAMEGETTIME();
	ioUserKeyInput::DirKeyInput eDir = pOwner->CheckCurDirKey();

	if( eDir != ioUserKeyInput::DKI_NONE )
	{
		float fYawD = 180.0f + 45.0f * ( eDir - ioUserKeyInput::DKI_UP );
		fYawD = ioMath::ArrangeHead( fYawD );

		float fYawR = DEGtoRAD( fYawD );
		D3DXQUATERNION qtRotate;
		D3DXQuaternionRotationYawPitchRoll( &qtRotate, fYawR, 0.0f, 0.0f );

		pOwner->SetTargetRotToRotate( qtRotate, false );
	}

	ioEntityGroup *pGrp = pOwner->GetGroup();
	m_ChargeComboState = CCS_DASH;
	m_bUseExtendJump = true;

	pOwner->SetCurJumpPowerAmt( 0.0f );
	pOwner->SetForcePowerAmt( 0.0f );
	pOwner->SetGravityAmt( 0.0f );

	int iAniID = pGrp->GetAnimationIdx( m_DashJumpAttack.m_AttackAnimation );
	float fTimeRate = m_DashJumpAttack.m_fAttackAniRate;
	DWORD dwPreDelay = m_DashJumpAttack.m_dwPreDelay;
	DWORD dwEndDelay = m_DashJumpAttack.m_dwEndDelay;

	pOwner->SetEnableHide( false );
	pOwner->SetNoWoundState( false );
	pOwner->SetEnableChangeTeam( false );
	pOwner->SetChangeJumpMotion( false, "", "", "", 0.0f );

	pGrp->ClearAllActionAni( FLOAT100, true );
	pGrp->ClearAllLoopAni( FLOAT100, true );

	pOwner->SetAutoTarget( ATT_JUMP );
	pGrp->SetActionAni( iAniID, FLOAT100, FLOAT100, FLOAT1, FLOAT1/fTimeRate, 0.0f, dwPreDelay, true );

	pOwner->RefreshFireTimeList( iAniID,
								 m_DashJumpAttack.m_vWeaponInfoList,
								 FTT_JUMP_ATTACK,
								 fTimeRate,
								 dwPreDelay );

	pOwner->IncreaseWeaponIndexBase();
	//
	m_dwJumpDashStartTime = dwCurTime;
	m_dwJumpDashEndTime = dwCurTime + dwPreDelay + dwEndDelay + pGrp->GetAnimationFullTime( iAniID ) * fTimeRate;

	pOwner->CheckCharColSkipTime( iAniID, fTimeRate, dwPreDelay );

	pOwner->SetReservedSliding( m_DashJumpAttack.m_vForceInfoList, iAniID, fTimeRate, dwPreDelay );

	DWORD dwTrackingTime = dwCurTime;
	dwTrackingTime += pGrp->GetAnimationEventTime_e( iAniID, "tracking_e" ) * fTimeRate;
	pOwner->SetAutoTargetTrackingEndTime( dwTrackingTime );

	if( pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_JUMP_DASH );
		kPacket << pOwner->GetCharName();
		kPacket << (int)GetType();
		kPacket << pOwner->GetMoveDir();
		kPacket << m_ChargeComboState;
		kPacket << pOwner->GetTargetRot();
		pOwner->FillAutoTargetInfo( kPacket );
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}

	if( m_bUseNewJumpTypeDash )
	{
		DWORD dwKeyReserveTime = dwCurTime + CheckKeyReserveTime( pOwner, iAniID, fTimeRate );
		SetNewJumpTypeDashAttack( pOwner, m_dwJumpDashEndTime, dwKeyReserveTime );
		return;
	}
}

void ioSuddenAttackJump::SetNewJumpTypeDashAttack( ioBaseChar *pOwner, DWORD dwEndTime, DWORD dwReserveTime )
{
	if( !pOwner ) return;

	m_bUseExtendJump = false;
	m_ChargeComboState = CCS_NEW_JUMP;

	pOwner->SetNewJumpAttackStateInfo( dwEndTime, dwReserveTime, m_fJumpDashGravityAmt );
}

float ioSuddenAttackJump::GetCurGravityAmt()
{
	if( m_ChargeComboState == CCS_DASH )
	{
		if( m_ComboDashType == CDT_GRAVITY )
			return m_fJumpDashGravityAmt;
		else if( m_ComboDashType == CDT_NO_GRAVITY )
			return -FLOAT1;
	}

	return 0.0f;
}

bool ioSuddenAttackJump::IsEnableJumpLandEndDash()
{
	if( m_ChargeComboState == CCS_DASH || m_ChargeComboState == CCS_DASH_END )
		return true;

	return false;
}

bool ioSuddenAttackJump::IsJumpAttackReserve()
{
	if( m_nWeaponItemMode != ioSuddenAttackItem::IMT_EXTRA2 )
		return false;

	return m_bReserveJumpAttack;
}


void ioSuddenAttackJump::AimedFireToUp
( ioEntityGroup *pGrp, float fWeight, float fTimeRate, DWORD dwPreDelay, bool bJump, bool bHold, const ioHashString& stUpAni, const ioHashString& stCenterAni )
{
	float fFadeOut = 0.0f;

	bool bAniFx1 = false;
	bool bAniFx2 = true;

	if( fWeight > FLOAT05 )
	{
		bAniFx1 = true;
		bAniFx2 = false;
	}

	if( fWeight > 0.0f )
	{
		pGrp->SetActionAni( stUpAni,
			0.0f, fFadeOut,
			fWeight, FLOAT1/fTimeRate,
			0.0f, dwPreDelay,
			bHold, bAniFx1 );
	}

	pGrp->SetActionAni( stCenterAni,
		0.0f, fFadeOut,
		FLOAT1 - fWeight, FLOAT1/fTimeRate,
		0.0f, dwPreDelay,
		bHold, bAniFx2 );
}


void ioSuddenAttackJump::AimedFireToDown
( ioEntityGroup *pGrp, float fWeight, float fTimeRate, DWORD dwPreDelay, bool bJump, bool bHold, const ioHashString& stDownAni, const ioHashString& stCenterAni )
{
	float fFadeOut = 0.0f;

	bool bAniFx1 = true;
	bool bAniFx2 = false;

	if( fWeight > FLOAT05 )
	{
		bAniFx1 = false;
		bAniFx2 = true;
	}

	if( fWeight > 0.0f )
	{
		pGrp->SetActionAni( stCenterAni,
			0.0f, fFadeOut,
			FLOAT1 - fWeight, FLOAT1/fTimeRate,
			0.0f, dwPreDelay,
			bHold, bAniFx1 );
	}

	pGrp->SetActionAni( stDownAni,
		0.0f, fFadeOut,
		fWeight, FLOAT1/fTimeRate,
		0.0f, dwPreDelay,
		bHold, bAniFx2 );
}