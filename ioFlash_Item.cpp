
#include "stdafx.h"

#include "ioFlash_Item.h"

#include "ioBaseChar.h"
#include "ioPlayStage.h"

#include "WeaponDefine.h"

ioFlash_Item::ioFlash_Item()
{	
	m_iCurAirJump = 0;
	m_fAirJumpPowerRate = 0.0f;
	m_fCurGauge = 0.0f;

	Init();
}

ioFlash_Item::ioFlash_Item( const ioFlash_Item &rhs )
	: ioWeaponItem( rhs ),
	  m_AttackReadyAniList( rhs.m_AttackReadyAniList ),
	  m_fMaxGauge( rhs.m_fMaxGauge ),
	  m_fNeedGauge( rhs.m_fNeedGauge ),  
	  m_fIncreaseGauge( rhs.m_fIncreaseGauge ),
	  m_szRunDelayAni( rhs.m_szRunDelayAni ),
	  m_fRunDelayAniRate( rhs.m_fRunDelayAniRate ),
	  m_dwRunDelay_PreDelay( rhs.m_dwRunDelay_PreDelay ),
	  m_szRunLoopAniName( rhs.m_szRunLoopAniName ),
	  m_szRunEndAni( rhs.m_szRunEndAni ),
	  m_fRunEndAniRate( rhs.m_fRunEndAniRate ),
	  m_dwRunEnd_PreDelay( rhs.m_dwRunEnd_PreDelay ),
	  m_nRunAttachWeaponNum( rhs.m_nRunAttachWeaponNum ),
	  m_nRunEndAttackWeaponNum( rhs.m_nRunEndAttackWeaponNum ),
	  m_nRunEndAttackResistNum( rhs.m_nRunEndAttackResistNum ),
	  m_szRunEndAttackWoundedAni( rhs.m_szRunEndAttackWoundedAni ),
	  m_dwRunEndAttackWoundedDuration( rhs.m_dwRunEndAttackWoundedDuration ),
	  m_bRunEndAttackWoundedLoop( rhs.m_bRunEndAttackWoundedLoop ),
	  m_vRunEndAttackOffset( rhs.m_vRunEndAttackOffset ),
	  m_fRunSpeed( rhs.m_fRunSpeed ),
	  m_dwRunTime( rhs.m_dwRunTime ),
	  m_bEnableDownAim( rhs.m_bEnableDownAim ),
	  m_fAimAngle( rhs.m_fAimAngle ),
	  m_fAimRange( rhs.m_fAimRange ),
	  m_fMinRange( rhs.m_fMinRange ),
	  m_fMarkerRange( rhs.m_fMarkerRange ),
	  m_fMaxMarkerRangeRate( rhs.m_fMaxMarkerRangeRate ),
	  m_fMaxAimRangeRate( rhs.m_fMaxAimRangeRate ),
	  m_fMaxAimAngleRate( rhs.m_fMaxAimAngleRate ),
	  m_nMaxTeleportCnt( rhs.m_nMaxTeleportCnt ),
	  m_TeleportList( rhs.m_TeleportList ),
	  m_fTeleportUseGauge( rhs.m_fTeleportUseGauge ),
	  m_fTeleportRange( rhs.m_fTeleportRange ),
	  m_fMaxMoveDist( rhs.m_fMaxMoveDist ),
	  m_fEndJumpPower( rhs.m_fEndJumpPower ),
	  m_fTeleportHeightGap( rhs.m_fTeleportHeightGap )
{
	m_iCurAirJump = 0;
	m_fAirJumpPowerRate = 0.0f;
	m_fCurGauge = 0.0f;

	Init();
}

ioFlash_Item::~ioFlash_Item()
{
}

void ioFlash_Item::LoadProperty( ioINILoader &rkLoader )
{
	//�⺻ ���� �б�
	ioWeaponItem::LoadProperty( rkLoader );

	m_AttackReadyAniList.clear();

	char szBuf[MAX_PATH];

	//���� �غ� �ִϸ��̼�
	int iMaxCombo = GetMaxCombo();
	for( int i=0; i < iMaxCombo; ++i )
	{
		wsprintf_e( szBuf, "attack_ready_animation%d", i+1 );
		rkLoader.LoadString( szBuf, "", szBuf, MAX_PATH );
		m_AttackReadyAniList.push_back( ioHashString(szBuf) );
	}
		
	//������ ����
	m_fMaxGauge = rkLoader.LoadFloat_e( "max_gauge", FLOAT100 );
	m_fNeedGauge = rkLoader.LoadFloat_e( "need_gauge", FLOAT100 );
	m_fIncreaseGauge = rkLoader.LoadFloat_e( "increase_gauge", FLOAT1 );

	//Run�ִϸ��̼�
	rkLoader.LoadString_e( "run_delay_ani_name", "", szBuf, MAX_PATH );
	m_szRunDelayAni = szBuf;
	m_fRunDelayAniRate = rkLoader.LoadFloat_e( "run_delay_ani_rate", FLOAT1 );
	m_dwRunDelay_PreDelay = rkLoader.LoadInt_e( "run_delay_ani_predelay", 0 );

	rkLoader.LoadString_e( "run_loop_ani_name", "", szBuf, MAX_PATH );
	m_szRunLoopAniName = szBuf;

	rkLoader.LoadString_e( "run_end_ani_name", "", szBuf, MAX_PATH );
	m_szRunEndAni = szBuf;
	m_fRunEndAniRate = rkLoader.LoadFloat_e( "run_end_ani_rate", FLOAT1 );
	m_dwRunEnd_PreDelay = rkLoader.LoadInt_e( "run_end_ani_predelay", 0 );

	//Run�ӵ�
	m_fMaxMoveDist = rkLoader.LoadFloat_e( "run_dist", 0.0f );
	m_fRunSpeed = rkLoader.LoadFloat_e( "run_speed", 0.0f );
	m_dwRunTime = rkLoader.LoadInt_e( "run_duration", 0 );

	//Run ����
	m_nRunAttachWeaponNum = rkLoader.LoadInt_e( "run_attach_type", NO_WEAPON_ATTRIBUTE );
	m_nRunEndAttackWeaponNum = rkLoader.LoadInt_e( "run_end_attack_type", NO_WEAPON_ATTRIBUTE);
	m_nRunEndAttackResistNum = rkLoader.LoadInt_e( "run_end_attack_resist", NO_WEAPON_ATTRIBUTE );

	rkLoader.LoadString_e( "run_end_attack_wounded", "", szBuf, MAX_PATH );
	m_szRunEndAttackWoundedAni = szBuf;
	
	m_dwRunEndAttackWoundedDuration = rkLoader.LoadInt_e( "run_end_attack_wounded_time", 0);
	m_bRunEndAttackWoundedLoop = rkLoader.LoadBool_e( "run_end_attack_wounded_loop_ani", false );

	m_vRunEndAttackOffset.x = rkLoader.LoadFloat_e( "run_end_attack_offset_x", 0.0f );
	m_vRunEndAttackOffset.y = rkLoader.LoadFloat_e( "run_end_attack_offset_y", 0.0f );
	m_vRunEndAttackOffset.z = rkLoader.LoadFloat_e( "run_end_attack_offset_z", 0.0f );

	//����
	m_bEnableDownAim = rkLoader.LoadBool_e( "enable_down_aim", false );
		
	m_fAimAngle = rkLoader.LoadFloat_e( "aim_angle", 45.0f );
	m_fAimRange = rkLoader.LoadFloat_e( "aim_range", FLOAT1000 );
	m_fMinRange = rkLoader.LoadFloat_e( "aim_min_range", 0.0f );
	m_fMarkerRange = rkLoader.LoadFloat_e( "marker_range", 0.0f );

	m_fMaxMarkerRangeRate = rkLoader.LoadFloat_e( "max_mark_range_rate", FLOAT1 );
	m_fMaxAimRangeRate = rkLoader.LoadFloat_e( "max_aim_range_rate", FLOAT1 );
	m_fMaxAimAngleRate = rkLoader.LoadFloat_e( "max_aim_angle_rate", FLOAT1 );

	m_nMaxTeleportCnt = rkLoader.LoadInt_e( "max_teleport_cnt", 0);

	m_fEndJumpPower = rkLoader.LoadFloat_e( "teleport_end_jump_power", 0.0f );

	//�ڷ���Ʈ ���� �ɼ� �б�
	LoadTeleportAttribute( rkLoader );
}

ioItem* ioFlash_Item::Clone()
{
	return new ioFlash_Item( *this );
}

ioWeaponItem::WeaponSubType ioFlash_Item::GetSubType() const
{
	return WST_FLASHITEM;
}

//���� �غ� �ִϸ��̼� ����
void ioFlash_Item::SetNormalAttack( ioBaseChar *pOwner, int iCurCombo )
{
	ClearCancelInfo();	
	
	Init();

	DWORD dwCurTime = FRAMEGETTIME();
	int iComboCnt = GetMaxCombo();
	if( !COMPARE( iCurCombo, 0, iComboCnt ) )
	{
		ioWeaponItem::SetNormalAttack( pOwner, iCurCombo );

		m_nNormalCombo = 0;
		pOwner->SetAttackMoveEnable( false );
		pOwner->SetChargingState( false );

		m_WeaponItemState = WS_NONE;
	}
	else
	{
		//���� �غ� �ִϸ��̼� ����
		ioEntityGroup *pGrp = pOwner->GetGroup();
		pGrp->ClearAllLoopAni( FLOAT100, true );
		pGrp->ClearAllActionAni( FLOAT100, true );

		int iAniID = pGrp->GetAnimationIdx( m_AttackReadyAniList[iCurCombo] );
		float fTimeRate = (float)m_dwChargeTime / pGrp->GetAnimationFullTime( iAniID );
		pGrp->SetActionAni( iAniID, FLOAT100, FLOAT100, FLOAT1, FLOAT1/fTimeRate, 0.0f, 0.0f, true );

		m_dwAttackStartTime = dwCurTime;
		m_nNormalCombo = iCurCombo;
		pOwner->SetState( CS_ATTACK );
		pOwner->SetAttackMoveEnable( false );
		pOwner->SetChargingState( true );

		DWORD dwTrackingTime = dwCurTime;
		dwTrackingTime += pGrp->GetAnimationEventTime( iAniID, "tracking_e" ) * fTimeRate;
		pOwner->SetAutoTargetTrackingEndTime( dwTrackingTime );

		m_WeaponItemState = WS_CHECK_CHARGING;
	}
}

//���� ���� Ȯ�� (���� ���μ���)
void ioFlash_Item::CheckNormalAttack( ioBaseChar *pOwner )
{
	switch( m_WeaponItemState )
	{
	case WS_CHECK_CHARGING:
		{
			CheckCharging( pOwner );
		}
		break;
	case WS_NORMAL_ATTACK:
		{
			if( pOwner->IsNeedProcess() )
			{
				CheckCancelReserve( pOwner );

				if( ProcessCancel(pOwner) )
					return;
			}

			ioWeaponItem::CheckNormalAttack( pOwner );
		}
		break;
	}
}

//������ ���⿡ �ɼ� ����
void ioFlash_Item::SetExtraInfoToWeapon( ioWeapon *pWeapon, bool bJumpAttack )
{
	ioAttackableItem::SetExtraInfoToWeapon( pWeapon, bJumpAttack );	

	ioZoneEffectWeapon *pZone = ToZoneEffectWeapon( pWeapon );
	if( pZone )
	{
		ioBaseChar *pChar = pZone->GetOwner();
		if( pChar )
		{
			D3DXVECTOR3 vDir = pChar->GetTargetRot() * D3DXVECTOR3( 0.0f, 0.0f, -FLOAT1 );
			pZone->SetCollisionDir( vDir );
			pZone->SetZoneValue( true, false );
		}
	}
}

//������ ���� (������ ȸ��)
void ioFlash_Item::UpdateExtraData( ioBaseChar *pOwner )
{
	if( !pOwner )
		return;

	// check owner state
	float fTimePerSec = g_FrameTimer.GetSecPerFrame();

	if( pOwner->IsSystemState() )
		return;

	switch( pOwner->GetState() )
	{
	case CS_DELAY:
	case CS_DASH:
	case CS_RUN:
		if( m_fCurGauge < m_fMaxGauge )
		{
			float fGauge = m_fIncreaseGauge * fTimePerSec;

			m_fCurGauge += fGauge;
			m_fCurGauge = min( m_fCurGauge, m_fMaxGauge );
		}
		return;
	}
}

//������ ����ȭ �κ�
void ioFlash_Item::OnItemGaugeOK( ioBaseChar *pOwner, SP2Packet &rkPacket )
{
	if( !pOwner ) return;

	int iState;
	rkPacket >> iState;

	switch( iState )
	{
	case WS_NORMAL_ATTACK:
		{
			rkPacket >> m_nNormalCombo;

			D3DXQUATERNION qtRot;
			rkPacket >> qtRot;

			pOwner->SetTargetRotToRotate( qtRot, true );
			SetNormalAttackState( pOwner );
		}
		break;
	case WS_SPECIAL_STATE_INIT:
		{
			pOwner->SetState( CS_FLASH_SPECIAL );
			m_FlashSpecialState = FMSS_FIND_TARGET;
		}
		break;
	case WS_SPECIAL_STATE_PROCESS:
		{
			RecvSpecialState( pOwner, rkPacket );
		}
		break;
	}
}

void ioFlash_Item::OnItemGaugeCancel( ioBaseChar *pOwner, SP2Packet &rkPacket )
{
	if( !pOwner ) return;
	
	bool bUsedExtra;
	rkPacket >> bUsedExtra;

	D3DXQUATERNION qtRot;
	rkPacket >> qtRot;

	pOwner->SetTargetRotToRotate( qtRot, true );

	pOwner->ApplyAutoTargetInfo( rkPacket );
}

//����ȭ
void ioFlash_Item::RecvSpecialState( ioBaseChar *pOwner, SP2Packet &rkPacket )
{
	if( !pOwner ) return;

	int nSpecialState;
	rkPacket >> nSpecialState;

	switch( nSpecialState )
	{
	case FMSS_FIND_TARGET:
		{
			rkPacket >> m_AimedTarget;
			rkPacket >> m_bAimedBall;
			m_FlashSpecialState = FMSS_CHECK_CHARGING;

			if ( !m_AimedTarget.IsEmpty() )
			{
				pOwner->SetTargetMarkerTarget( m_AimedTarget, m_bAimedBall );
				pOwner->SetTargetMarkerState( ioTargetMarker::MS_AIMED );
			}
		}
		break;
	case FMSS_CHAR_ROT:
		{
			int nInputKey;
			rkPacket >> nInputKey;

			m_InputDirKey = (ioUserKeyInput::DirKeyInput)nInputKey;
			pOwner->SetTargetRotToDirKey( m_InputDirKey );

			m_AimedTarget.Clear();
			pOwner->SetTargetMarkerState( ioTargetMarker::MS_NONE );
		}
		break;
	case FMSS_CREATE_ATTACH_WEAPON:
		{
			rkPacket >> m_dwRunWeaponBaseIndex;

			//������� (���� �޷��ִ� ����)
			FireTime kFireTime;
			kFireTime.dwStart = FRAMEGETTIME();

			kFireTime.iAttributeIdx = m_nRunAttachWeaponNum;
			kFireTime.iResistanceIdx = 0;
			kFireTime.szWoundedAni  = "";
			kFireTime.dwWoundedAniDuration = 0;
			kFireTime.bLoopWoundedAni = false;

			kFireTime.dwWeaponIdx = m_dwRunWeaponBaseIndex;
			kFireTime.eFireTimeType = FTT_NORMAL_ATTACK;

			D3DXVECTOR3 vPos = pOwner->GetMidPositionByRate();
			D3DXVECTOR3 vDir = pOwner->GetTargetRot() * D3DXVECTOR3( 0, 0, -1 );
			pOwner->ExtendFireExplicit( kFireTime, vPos, vDir, GetName() );
		}
		break;
	case FMSS_RUN_FIRE:
		{
			//������ ����
			DecreaseGauge( m_fNeedGauge );			
			pOwner->SetTargetMarkerState( ioTargetMarker::MS_NONE );
			m_bSetMoveAttack = true;
			SetRunDelayState( pOwner );
		}
		break;
	case FMSS_MOVE_END:
		{
			SetRunEndState( pOwner );
		}
		break;
	case FMSS_RUN_END_INPUT:
		{
			rkPacket >> m_dwTeleportWeaponBaseIndex;
			rkPacket >> m_bCheckMove;
			m_bEnableTeleport = false;
			DecreaseGauge( m_fTeleportUseGauge );
			if ( m_bCheckMove )
			{
				m_bCheckMove = SetTeleportMove( pOwner );
			}
			SetTeleportState( pOwner );
		}
		break;
	case FMSS_ENABLE_TELEPORT:
		{
			rkPacket >> m_bEnableTeleport;
		}
		break;
	case FMSS_SET_TARGET:
		{
			rkPacket >> m_szTeleportTargetName;
		}
		break;
	case FMSS_TELEPORT:
		{
			rkPacket >> m_bCheckMove;
			DecreaseGauge( m_fTeleportUseGauge );
			if ( m_bCheckMove )
			{
				m_bCheckMove = SetTeleportMove( pOwner );
			}
			SetTeleportState( pOwner );
		}
		break;
	case FMSS_TELEPORT_MOVE:
		{
			D3DXVECTOR3 vPos, vDir;
			rkPacket >> vPos;
			rkPacket >> vDir;

			//����
			pOwner->SetWorldPosition( vPos );

			//ȸ��
			pOwner->SetTargetRotToDir( vDir, true );
		}
		break;
	case FMSS_END:
		{
			m_FlashSpecialState = FMSS_END;			
		}
		break;
	case FMSS_END_JUMP:
		{
			pOwner->SetSKillEndJumpState( m_fEndJumpPower, false, false, false, true );
		}
		break;
	}
}

//��¡ ���� �����ΰ�
bool ioFlash_Item::IsChargeAttackState( ioBaseChar *pOwner )
{
	if( pOwner->GetState() == CS_ATTACK )
	{
		if( m_WeaponItemState == WS_CHECK_CHARGING )
			return true;
	}

	return false;
}

//������ ���� �� ������ ���
int ioFlash_Item::GetMaxBullet()
{
	return (int)m_fMaxGauge;
}

int ioFlash_Item::GetCurBullet()
{
	float fCurCoolTime = m_fCurGauge;
	fCurCoolTime = max( 0, min( fCurCoolTime, m_fMaxGauge ) );

	return (int)fCurCoolTime;
}

void ioFlash_Item::SetCurBullet( int iCurBullet )
{
	if( iCurBullet < 0 )
		return;

	float fCurBullet = iCurBullet;
	fCurBullet = max( 0, min( fCurBullet, m_fMaxGauge ) );

	m_fCurGauge = fCurBullet;
}

void ioFlash_Item::InitExtraGauge()
{
	m_fCurGauge = 0.0f;
}

void ioFlash_Item::MaxExtraGauge()
{
	m_fCurGauge = m_fMaxGauge;
}

float ioFlash_Item::GetMaxExtraGauge()
{
	return m_fMaxGauge;
}

float ioFlash_Item::GetCurExtraGauge()
{
	return m_fCurGauge;
}

void ioFlash_Item::SetCurExtraGauge( float fGauge )
{
	m_fCurGauge = fGauge;
	m_fCurGauge = (float)max( 0, min( m_fCurGauge, m_fMaxGauge ) );
}

int ioFlash_Item::GetNeedBullet()
{
	return (int)m_fNeedGauge;
}

//������ ���
void ioFlash_Item::DecreaseGauge( float fNeedGauge )
{
	m_fCurGauge -= fNeedGauge;
	m_fCurGauge = (float)max( 0.f, min( m_fCurGauge, m_fMaxGauge ) );
}

//��� ���� ��
void ioFlash_Item::OnEquiped( ioBaseChar *pOwner )
{
	ioWeaponItem::OnEquiped( pOwner );
	pOwner->SetTargetMarker( ioTargetMarker::MT_BOW_ITEM, m_szReadyMarker, m_szAimingMarker, m_szAimedMarker, m_szZeroAimedMarker, "" );
	pOwner->SetTargetMarkerRange( m_fMarkerRange );
}

//��� ���� ��
void ioFlash_Item::OnReleased( ioBaseChar *pOwner )
{
	pOwner->SetTargetMarker( ioTargetMarker::MT_NONE, "", "", "", "", "" );
	ioWeaponItem::OnReleased( pOwner );
}

////////////////////////////////////////////////////////////////////////////
///////////�⺻ ���� ����///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

//�ʱ�ȭ
void ioFlash_Item::Init()
{	
	m_WeaponItemState = WS_NONE;
	m_dwAttackStartTime = 0;
	m_dwCheckRunTime = 0;

	m_FlashSpecialState = FMSS_NONE;
	
	m_AimedTarget.Clear();
	m_InputDirKey = ioUserKeyInput::DKI_NONE;
	m_bSetMoveAttack = false;
	m_bSetTarget = false;
	m_vTargetDir = ioMath::VEC3_ZERO;
	m_fCurMoveDist = 0.0f;

	m_nCurTeleportCnt = 0;
	m_dwRunWeaponBaseIndex = 0;
	m_szTeleportTargetName.Clear();
	m_dwTeleportWeaponBaseIndex = 0;
	m_dwTeleportAttackEndTime = 0;
	m_dwTeleportReserveTime = 0;
	m_bEnableTeleport = true;

	m_bAttackKeyReserved = false;
	m_bDefenseKeyReserved = false;
	m_bJumpKeyReserved = false;

	m_bCheckMove = true;
}

//��¡ Ȯ��
void ioFlash_Item::CheckCharging( ioBaseChar *pOwner )
{
	if( !pOwner->IsNeedProcess()  )	
		return;

	//������ Ȯ��
	if( pOwner->IsAttackKeyDown() && !pOwner->IsHasCrown() && m_fCurGauge >= m_fNeedGauge )
	{
		if( m_dwAttackStartTime + m_dwChargeTime <= FRAMEGETTIME() )
		{
			//Ư�� ���·� ����
			SetFlashSpecialState( pOwner );
		}
	}
	else
	{
		//�Ϲ� ����
		SetNormalAttackState( pOwner );
	}
}

//�⺻ ���� ���·� ����
void ioFlash_Item::SetNormalAttackState( ioBaseChar *pOwner )
{
	if( !pOwner ) 
		return;

	ClearCancelInfo();

	DWORD dwCurTime = FRAMEGETTIME();

	pOwner->InitExtendAttackTagInfo();
	pOwner->InitActionStopDelay( true );
	
	const AttackAttribute *pAttribute = m_FormalAttack.GetAttribute( AT_NORMAL, m_nNormalCombo );
	if( pAttribute )
	{
		pOwner->SetNormalAttackByAttribute( *pAttribute );

		m_szCurAni = pAttribute->m_AttackAnimation;
		m_fCurAniRate = pAttribute->m_fAttackAniRate;

		m_dwInputCancelCheckTime = dwCurTime;
		m_dwCancelCheckTime = dwCurTime;
	}

	pOwner->ApplyNormalAttackResult( m_nNormalCombo );
	m_WeaponItemState = WS_NORMAL_ATTACK;

	if( pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_ITEM_GAUGE_OK );
		FillItemGaugePacketHeader( pOwner, kPacket );
		kPacket << m_WeaponItemState;
		kPacket << m_nNormalCombo;
		kPacket << pOwner->GetTargetRot();
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

//���ִ� ���� �ΰ�
bool ioFlash_Item::IsFloatState( ioBaseChar *pOwner )
{	
	if ( !pOwner )
		return false;
	
	ioPlayStage *pStage = pOwner->GetCreator();
	if ( !pStage )
		return false;

	D3DXVECTOR3 vPos = pOwner->GetWorldPosition();
	float fMapHeight = pStage->GetMapHeightOnlyTerrain( vPos.x, vPos.z );

	if ( fabs( fMapHeight - vPos.y ) >= 20.0f )
	{
		return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////
///////////////////Ư�� ����////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//Ư�� ���� �ʱ�ȭ
void ioFlash_Item::ClearState( ioBaseChar *pOwner )
{
	m_FlashSpecialState = FMSS_NONE;
}

//������ ����� �����Ѱ�?
bool ioFlash_Item::IsEnableGauge()
{
	if( m_fCurGauge >= m_fNeedGauge )
		return true;

	return false;
}

//���� Ư�� ����
ioFlash_Item::FlashSpecialState ioFlash_Item::GetFlashSpecialState()
{
	return m_FlashSpecialState;
}

//ĳ���͸� Ư�� ���·� ���� (���� ���¿��� Ư�� ���·� ����)
void ioFlash_Item::SetFlashSpecialState( ioBaseChar *pOwner )
{
	if( !pOwner || !pOwner->IsNeedProcess() )
		return;
	
	pOwner->SetState( CS_FLASH_SPECIAL );
	m_FlashSpecialState = FMSS_FIND_TARGET;

	if( pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_ITEM_GAUGE_OK );
		FillItemGaugePacketHeader( pOwner, kPacket );
		kPacket << WS_SPECIAL_STATE_INIT;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

//�뽬 ������ ����
void ioFlash_Item::SetDashCharge( ioBaseChar *pOwner )
{
	Init();
	CheckFindTarget( pOwner );
	CheckRotation( pOwner );
	DecreaseGauge( m_fNeedGauge );			
	pOwner->SetTargetMarkerState( ioTargetMarker::MS_NONE );
	SetRunDelayState( pOwner );
	CreateAttachWeapon( pOwner );
}

//ĳ���� Ư�� ���� ���μ���
void ioFlash_Item::ProcessFlashSpecialState( ioBaseChar *pOwner )
{
	if( !pOwner )
		return;

	DWORD dwCurTime = FRAMEGETTIME();

	switch( m_FlashSpecialState )
	{
	case FMSS_FIND_TARGET:
		{
			CheckFindTarget( pOwner );
			m_FlashSpecialState = FMSS_CHECK_CHARGING;
		}
		break;
	case FMSS_CHECK_CHARGING:
		{
			//ĳ���� ȸ��
			SetCharacterRot( pOwner );
			
			//Ÿ���� �׻� �ٶ󺸵��� ����
			CheckRotation( pOwner );
			
			//Ÿ���� ���ٸ� ��˻�
			if ( (!m_bSetMoveAttack && m_AimedTarget.IsEmpty()) ||
				 IsTargetHasHideBuff( pOwner, m_AimedTarget ) ||
				 ( m_bSetTarget && D3DXVec3Length( &m_vTargetDir ) > GetChargeAimRange()) )
			{
				m_FlashSpecialState = FMSS_FIND_TARGET;
				m_bSetTarget = false;
			}

			CheckInputKey( pOwner );
		}
		break;
	case FMSS_MOVE_DELAY:
		{			
			if ( m_dwRunDelayMotionEndTime < dwCurTime )
			{				
				//�޸��� ���·� ����
				SetRunLoopState( pOwner );
			}
		}
		break;
	case FMSS_MOVE_RUN:
		{
			CheckMove( pOwner );

			//�Ÿ��� �Ǿ��ų�, �ð��� ������ �޸��� ����
			if ( m_fCurMoveDist >= m_fMaxMoveDist || 
				( m_dwCheckRunTime + m_dwRunTime < FRAMEGETTIME() ) )
			{				
				SetRunEndState( pOwner );			
			}
		}
		break;
	case FMSS_MOVE_END:
		{
			CheckRunEndInputKey( pOwner );

			//�ִϸ��̼� ������ ����
			if ( m_dwRunEndMotionEndTime < dwCurTime )
			{
				CheckReserveInput( pOwner );				
			}
		}
		break;
	case FMSS_TELEPORT:
		{
			CheckTeleportInputKey( pOwner );
						
			//�ڷ���Ʈ ���� ���� (Ű�� ���� �� �ִϸ��̼��� ����Ǹ� ����)
			if ( m_dwTeleportAttackEndTime > 0 && m_dwTeleportAttackEndTime < dwCurTime )
			{
				CheckReserveInput( pOwner );	
			}
		}
		break;
	case FMSS_END:
		{			
			if ( IsFloatState( pOwner ) )
			{	
				if ( pOwner->IsNeedProcess() )
				{
					pOwner->SetSKillEndJumpState( m_fEndJumpPower, false, false, true, true );

					if ( pOwner->IsNeedSendNetwork() )
					{
						SP2Packet kPacket( CUPK_ITEM_GAUGE_OK );
						FillItemGaugePacketHeader( pOwner, kPacket );
						kPacket << WS_SPECIAL_STATE_PROCESS;
						kPacket << FMSS_END_JUMP;
						P2PNetwork::SendToAllPlayingUser( kPacket );
					}
				}
			}
			else
			{
				pOwner->SetState( CS_DELAY );
			}
		}
		break;
	}
}

//ĳ���� ��� ����
bool ioFlash_Item::IsFlashSpecialCharCollisionSkipState()
{
	switch( m_FlashSpecialState )
	{
	case FMSS_MOVE_RUN:
		return true;
	}

	return false;
}

//Ű�Է� Ȯ��
void ioFlash_Item::CheckInputKey( ioBaseChar *pOwner )
{
	if ( !pOwner || !pOwner->IsNeedProcess() )
		return;
	
	if ( !m_bSetMoveAttack && !pOwner->IsAttackKeyDown() )
	{
		DecreaseGauge( m_fNeedGauge );			
		pOwner->SetTargetMarkerState( ioTargetMarker::MS_NONE );
		m_bSetMoveAttack = true;
		SetRunDelayState( pOwner );
		CreateAttachWeapon( pOwner );

		if( pOwner->IsNeedSendNetwork() )
		{
			SP2Packet kPacket( CUPK_ITEM_GAUGE_OK );
			FillItemGaugePacketHeader( pOwner, kPacket );
			kPacket << WS_SPECIAL_STATE_PROCESS;
			kPacket << FMSS_RUN_FIRE;
			P2PNetwork::SendToAllPlayingUser( kPacket );
		}
	}
}

//�̵� ��� ����
void ioFlash_Item::SetRunDelayState( ioBaseChar *pOwner )
{
	if ( !pOwner)
		return;

	//���� �غ� �ִϸ��̼� ����
	ioEntityGroup *pGrp = pOwner->GetGroup();

	int iAniID = pGrp->GetAnimationIdx( m_szRunDelayAni );
	if ( iAniID == -1)
		return;

	pGrp->ClearAllLoopAni( FLOAT100, true );
	pGrp->ClearAllActionAni( FLOAT100, true );

	float fTimeRate  = m_fRunDelayAniRate;
	DWORD dwPreDelay = m_dwRunDelay_PreDelay;

	pGrp->SetActionAni( iAniID, FLOAT100, FLOAT100, FLOAT1, FLOAT1/fTimeRate, 0.0f, dwPreDelay, true );	

	m_dwRunDelayMotionEndTime = FRAMEGETTIME();
	m_dwRunDelayMotionEndTime += pGrp->GetAnimationFullTime( iAniID ) * fTimeRate;
	m_dwRunDelayMotionEndTime += dwPreDelay;

	m_FlashSpecialState = FMSS_MOVE_DELAY;
}

void ioFlash_Item::SetRunLoopState( ioBaseChar *pOwner )
{
	if ( !pOwner)
		return;

	//���� �غ� �ִϸ��̼� ����
	ioEntityGroup *pGrp = pOwner->GetGroup();

	int iAniID = pGrp->GetAnimationIdx( m_szRunLoopAniName );
	if ( iAniID == -1)
		return;

	pGrp->ClearAllLoopAni( FLOAT100, true );
	pGrp->ClearAllActionAni( FLOAT100, true );

	pGrp->SetLoopAni( iAniID, FLOAT100 );	

	m_dwCheckRunTime = FRAMEGETTIME();
	m_FlashSpecialState = FMSS_MOVE_RUN;
}

void ioFlash_Item::SetRunEndState( ioBaseChar *pOwner )
{
	if ( !pOwner)
		return;

	//���� �غ� �ִϸ��̼� ����
	ioEntityGroup *pGrp = pOwner->GetGroup();

	int iAniID = pGrp->GetAnimationIdx( m_szRunEndAni );
	if ( iAniID == -1)
		return;

	pGrp->ClearAllLoopAni( FLOAT100, true );
	pGrp->ClearAllActionAni( FLOAT100, true );

	float fTimeRate  = m_fRunEndAniRate;
	DWORD dwPreDelay = m_dwRunEnd_PreDelay;

	pGrp->SetActionAni( iAniID, FLOAT100, FLOAT100, FLOAT1, FLOAT1/fTimeRate, 0.0f, dwPreDelay, true );	

	m_dwRunEndMotionEndTime = FRAMEGETTIME();
	m_dwRunEndMotionEndTime += pGrp->GetAnimationFullTime( iAniID ) * fTimeRate;
	m_dwRunEndMotionEndTime += dwPreDelay;

	m_dwTeleportReserveTime = FRAMEGETTIME();
	m_dwTeleportReserveTime += pGrp->GetAnimationEventTime_e( iAniID, "key_reserve" ) * fTimeRate;

	//RunEnd �� ����
	FireTime kFireTime;
	kFireTime.dwStart = FRAMEGETTIME();

	kFireTime.iAttributeIdx = m_nRunEndAttackWeaponNum;
	kFireTime.iResistanceIdx = m_nRunEndAttackResistNum;
	kFireTime.szWoundedAni  = m_szRunEndAttackWoundedAni;
	kFireTime.dwWoundedAniDuration = m_dwRunEndAttackWoundedDuration;
	kFireTime.bLoopWoundedAni = m_bRunEndAttackWoundedLoop;

	kFireTime.dwWeaponIdx = m_dwRunWeaponBaseIndex + 1;
	kFireTime.eFireTimeType = FTT_NORMAL_ATTACK;

	D3DXQUATERNION qtRot = pOwner->GetTargetRot();
	D3DXVECTOR3 vPos = pOwner->GetWorldPosition() + ( qtRot * m_vRunEndAttackOffset );
	D3DXVECTOR3 vDir = qtRot * D3DXVECTOR3( 0, 0, -1 );
	pOwner->ExtendFireExplicit( kFireTime, vPos, vDir, GetName() );

	m_FlashSpecialState = FMSS_MOVE_END;

	if ( pOwner->IsNeedProcess() && pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_ITEM_GAUGE_OK );
		FillItemGaugePacketHeader( pOwner, kPacket );
		kPacket << WS_SPECIAL_STATE_PROCESS;
		kPacket << FMSS_MOVE_END;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

//ȸ��
void ioFlash_Item::CheckRotation( ioBaseChar *pOwner )
{
	if ( m_AimedTarget.IsEmpty() )
		return;

	ioBaseChar *pTarget = pOwner->GetBaseChar( m_AimedTarget );
	if ( pTarget )
	{
		m_vTargetDir = pTarget->GetWorldPosition() - pOwner->GetWorldPosition();
		m_bSetTarget = true;

		D3DXVECTOR3 vDir = m_vTargetDir;
		D3DXVec3Normalize( &vDir, &vDir );
		pOwner->SetTargetRotToDir( vDir, true );
	}
}

//ĳ���� ȸ��
void ioFlash_Item::SetCharacterRot( ioBaseChar *pOwner )
{
	if ( !pOwner || !pOwner->IsNeedProcess() )
		return;

	//����Ű �Է��� �ִٸ�
	if( pOwner->IsSettedDirection() )
	{
		//���� �Էµ� ����Ű Ȯ���ϰ� ����
		ioUserKeyInput::DirKeyInput eNewDirKey = pOwner->CheckCurDirKey();
		m_InputDirKey = eNewDirKey;
	}

	//�Էµ� ����Ű�� ���� ĳ������ ������ �ٸ��ٸ�
	if ( m_InputDirKey != pOwner->GetCurDirKey() )
	{
		//�Էµ� �������� ȸ��
		pOwner->SetTargetRotToDirKey( m_InputDirKey );
		//Ÿ���̸� ����
		m_AimedTarget.Clear();

		if( pOwner->IsNeedSendNetwork() )
		{
			SP2Packet kPacket( CUPK_ITEM_GAUGE_OK );
			FillItemGaugePacketHeader( pOwner, kPacket );
			kPacket << WS_SPECIAL_STATE_PROCESS;
			kPacket << FMSS_CHAR_ROT;
			kPacket << (int)m_InputDirKey;
			P2PNetwork::SendToAllPlayingUser( kPacket );
		}

		m_InputDirKey = pOwner->GetCurDirKey();
	}
}

//�̵�
void ioFlash_Item::CheckMove( ioBaseChar *pOwner )
{	
	if( !pOwner ) return;

	ioPlayStage *pStage = pOwner->GetCreator();
	if( !pStage ) return;

	float fTimePerSec = g_FrameTimer.GetSecPerFrame();

	bool bCurMove = false;
	D3DXVECTOR3 vTotalMove( 0.0f, 0.0f, 0.0f );

	float fCurTimeGap = fTimePerSec * FLOAT1000;
	
	D3DXVECTOR3 vDir;
	if ( m_AimedTarget.IsEmpty() )
	{
		vDir = pOwner->GetWorldOrientation() * D3DXVECTOR3( 0.0f, 0.0f, -FLOAT1 );
	}
	else 
	{
		vDir = m_vTargetDir;
	}
	D3DXVec3Normalize( &vDir, &vDir );

	while( fCurTimeGap > 0.0f )
	{
		float fDivideTimePerSec = min( fCurTimeGap, MIN_FRAME_TIME ) * FLOAT0001;
		fCurTimeGap -= MIN_FRAME_TIME;

		D3DXVECTOR3 vMoveAmt;
		
		if( m_fRunSpeed > 0.0f && vDir != ioMath::VEC3_ZERO )
		{
			float fFinalMoveAmt = m_fRunSpeed * fDivideTimePerSec;
			if( fFinalMoveAmt > 0.0f )
			{
				vMoveAmt = vDir * fFinalMoveAmt;
				vTotalMove += vMoveAmt;
				bCurMove = true;
			}
		}

		if( pOwner->ProcessForceMove( fDivideTimePerSec, vMoveAmt ) )
		{
			vTotalMove += vMoveAmt;
			bCurMove = true;
		}
	}

	if( bCurMove )
	{
		bool bCol = false;
		if( pStage->ReCalculateMoveVectorByTerrain( pOwner, &vTotalMove, bCol ) )
		{
			float fPreHeight = pOwner->GetBottomHeight();

			pOwner->Translate( vTotalMove );

			D3DXVECTOR3 vDiff = ioMath::VEC3_ZERO;
			vDiff = vTotalMove - vDiff;
			float fMoveDist = D3DXVec3Length( &vDiff );
			m_fCurMoveDist += fMoveDist;

			if( pOwner->GetForceAmt() != 0.0f )	// ���� ForceMove�� ������ ����ϴ� ��쿡 ���� ����
			{
				float fMoveHeight = pOwner->GetBottomHeight();

				if( fPreHeight > 0.0f && fMoveHeight <= FLOAT1 )
				{
					D3DXVECTOR3 vCurPos = pOwner->GetWorldPosition();

					float fTopHeight = pStage->GetMapHeight( vCurPos.x,
															 vCurPos.z,
															 pOwner,
															 false,
															 pOwner->GetMidHeightByRate( FLOAT1 ) );

					if( fTopHeight > FLOAT1 )
					{
						vCurPos.y = max( fTopHeight, fPreHeight );
						pOwner->SetWorldPosition( vCurPos );
					}
				}
			}
		}
		if ( bCol )
		{			
			m_FlashSpecialState = FMSS_END;
			return;
		}
	}

	D3DXVECTOR3 vPos = pOwner->GetWorldPosition();
	float fPreHeight = pOwner->GetBottomHeight();

	if( vPos.y < fPreHeight )
		vPos.y = fPreHeight;

	if( pStage && pOwner->IsAxisLimit() )
	{
		pStage->CheckAxisLimit( &vPos );
		pOwner->SetWorldPosition( vPos );
	}	
}

//Ÿ�� ã��
void ioFlash_Item::CheckFindTarget( ioBaseChar *pOwner )
{
	if ( !pOwner || !pOwner->IsNeedProcess() )
		return;

	ioHashString szTarget;
	m_AimedTarget.Clear();
	m_bAimedBall = false;

	float fAimAngle = GetChargeAimAngle();
	float fAimRange = GetChargeAimRange();

	if( pOwner->CheckAimedTargetByBow( fAimAngle, m_fMinRange, fAimRange, szTarget, m_bEnableDownAim ) )
	{
		m_AimedTarget = szTarget;
		pOwner->SetTargetMarkerTarget( m_AimedTarget, m_bAimedBall );
		pOwner->SetTargetMarkerState( ioTargetMarker::MS_AIMED );		
	}
	else
	{
		m_AimedTarget.Clear();
		pOwner->SetTargetMarkerState( ioTargetMarker::MS_READY );
		return;
	}

	// Ÿ�� ��
	if( CompareAimedTarget( pOwner ) )
	{
		m_AimedTarget.Clear();
		m_bAimedBall = true;
	}		

	if( pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_ITEM_GAUGE_OK );
		FillItemGaugePacketHeader( pOwner, kPacket );
		kPacket << WS_SPECIAL_STATE_PROCESS;
		kPacket << FMSS_FIND_TARGET;
		kPacket << m_AimedTarget;
		kPacket << m_bAimedBall;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

float ioFlash_Item::GetChargeAimRange()
{
	if( m_fCurChargeRate > 0.0f )
	{
		float fCurAimRangeRate = FLOAT1;
		float fCurMaxAimRange = max( 0.0f, (m_fMaxAimRangeRate - FLOAT1) );

		fCurAimRangeRate += fCurMaxAimRange * m_fCurChargeRate;

		float fCurRange = m_fAimRange * fCurAimRangeRate;

		return fCurRange;
	}

	return m_fAimRange;
}

float ioFlash_Item::GetChargeAimAngle()
{
	if( m_fCurChargeRate > 0.0f )
	{
		float fCurAimAngleRate = FLOAT1;
		float fCurMaxAimAngle = max( 0.0f, (m_fMaxAimAngleRate - FLOAT1) );

		fCurAimAngleRate += fCurMaxAimAngle * m_fCurChargeRate;

		float fCurAngle = m_fAimAngle * fCurAimAngleRate;
		return fCurAngle;
	}

	return m_fAimAngle;
}

bool ioFlash_Item::CompareAimedTarget( ioBaseChar *pOwner )
{
	if( !pOwner ) return false;

	ioBall *pBall = pOwner->FindBall();
	if( !pBall ) 
		return false;

	float fAimRange = GetChargeAimRange();
	float fAimAngle = GetChargeAimAngle();
	
	D3DXVECTOR3 vDiff;
	D3DXVECTOR3 vOwnerPos = pOwner->GetWorldPosition();
	D3DXVECTOR3 vCharDir  = pOwner->GetMoveDir();

	float fHalfCosine = cosf( DEGtoRAD( fAimAngle ) * FLOAT05 );

	float fLimitDistSq = fAimRange * fAimRange;
	float fMinDistSq = m_fMinRange * m_fMinRange;
	
	// Ball
	if( pBall )
	{
		vDiff = pBall->GetWorldPosition() - vOwnerPos;
		float fCurDistSq = D3DXVec3LengthSq( &vDiff );

		if( (fLimitDistSq >= fCurDistSq) && (fMinDistSq <= fCurDistSq) )
		{
			vDiff.y = 0.0f;
			D3DXVec3Normalize( &vDiff, &vDiff );

			if( D3DXVec3Dot( &vCharDir, &vDiff ) >= fHalfCosine )
				return true;
		}
	}
	return false;
}

bool ioFlash_Item::IsChargeAutoTarget( ioBaseChar *pOwner )
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

void ioFlash_Item::GetAutoTargetValue( ioBaseChar *pOwner,
	float &fRange, float &fMinAngle, float &fMaxAngle,
	AutoTargetType eType )
{
	if( IsChargeAutoTarget(pOwner) )
	{
		float fCurTargetRangeRate, fCurTargetAngleRate;
		fCurTargetRangeRate = fCurTargetAngleRate = FLOAT1;

		float fCurMaxTargetRange = max( 0.0f, (m_fMaxTargetRangeRate - FLOAT1) );
		float fCurMaxTargetAngle = max( 0.0f, (m_fMaxTargetAngleRate - FLOAT1) );

		fCurTargetRangeRate += fCurMaxTargetRange * m_fCurChargeRate;
		fCurTargetAngleRate += fCurMaxTargetAngle * m_fCurChargeRate;

		fRange = m_fAutoTargetRange * fCurTargetRangeRate;
		fMinAngle = m_fAutoTargetMinAngle / fCurTargetAngleRate;
		fMaxAngle = m_fAutoTargetMaxAngle / fCurTargetAngleRate;
	}
	else
	{
		switch( eType )
		{
		case ATT_NORMAL:
			fRange = m_fAutoTargetRange;
			fMinAngle = m_fAutoTargetMinAngle;
			fMaxAngle = m_fAutoTargetMaxAngle;
			return;
		case ATT_DASH:
			if( m_fDashAutoTargetRange == 0.0f )
				break;

			fRange = m_fDashAutoTargetRange;
			fMinAngle = m_fDashAutoTargetMinAngle;
			fMaxAngle = m_fDashAutoTargetMaxAngle;
			return;
		case ATT_JUMP:
			if( m_fJumpAutoTargetRange == 0.0f )
				break;

			fRange = m_fJumpAutoTargetRange;
			fMinAngle = m_fJumpAutoTargetMinAngle;
			fMaxAngle = m_fJumpAutoTargetMaxAngle;
			return;
		case ATT_COUNTER:
			if( m_fCountAutoTargetRange == 0.0f )
				break;

			fRange = m_fCountAutoTargetRange;
			fMinAngle = m_fCountAutoTargetMinAngle;
			fMaxAngle = m_fCountAutoTargetMaxAngle;
			return;
		}


		fRange = m_fAutoTargetRange;
		fMinAngle = m_fAutoTargetMinAngle;
		fMaxAngle = m_fAutoTargetMaxAngle;

		return;
	}
}

//�޸��� ���� �� Ű�Է� Ȯ��
void ioFlash_Item::CheckRunEndInputKey( ioBaseChar *pOwner )
{
	if ( !pOwner || !pOwner->IsNeedProcess() )
		return;

	if ( m_dwTeleportReserveTime < FRAMEGETTIME() && !m_szTeleportTargetName.IsEmpty() && 
		 m_bEnableTeleport && m_nCurTeleportCnt < m_nMaxTeleportCnt && 
		 m_fCurGauge >= m_fTeleportUseGauge && pOwner->IsAttackKey() )
	{
		m_dwTeleportWeaponBaseIndex = pOwner->IncreaseWeaponIndexBase();
		m_bEnableTeleport = false;
		DecreaseGauge( m_fTeleportUseGauge );
		if ( m_bCheckMove )
		{
			m_bCheckMove = SetTeleportMove( pOwner );
		}
		SetTeleportState( pOwner );

		if( pOwner->IsNeedSendNetwork() )
		{
			SP2Packet kPacket( CUPK_ITEM_GAUGE_OK );
			FillItemGaugePacketHeader( pOwner, kPacket );
			kPacket << WS_SPECIAL_STATE_PROCESS;
			kPacket << FMSS_RUN_END_INPUT;
			kPacket << m_dwTeleportWeaponBaseIndex;
			kPacket << m_bCheckMove;
			P2PNetwork::SendToAllPlayingUser( kPacket );
		}
	}
	else if ( m_dwTeleportReserveTime < FRAMEGETTIME() )
	{
		if( pOwner->IsAttackKey() )
			m_bAttackKeyReserved = true;

		if( pOwner->IsDefenseKey() )
			m_bDefenseKeyReserved = true;

		if( pOwner->IsJumpKey() )
			m_bJumpKeyReserved = true;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
////�ڷ���Ʈ///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//������ �б�
void ioFlash_Item::LoadTeleportAttribute( ioINILoader &rkLoader )
{
	char szBuf[MAX_PATH];

	m_fTeleportUseGauge = rkLoader.LoadFloat_e( "teleport_need_gauge", 0.0f );
	m_fTeleportRange = rkLoader.LoadFloat_e( "teleport_range", 0.0f);
	m_fTeleportHeightGap = rkLoader.LoadFloat_e( "teleport_height_gap", 0.0f );
	m_TeleportList.clear();
	if( m_nMaxTeleportCnt > 0 )
	{
		m_TeleportList.reserve( m_nMaxTeleportCnt );

		for( int i=0; i < m_nMaxTeleportCnt; ++i )
		{
			AttackAttribute kAttack;

			wsprintf_e( szBuf, "teleport_attack%d", i+1 );
			LoadAttackAttribute( szBuf, kAttack, rkLoader );

			m_TeleportList.push_back( kAttack );
		}
	}
}

//�ڷ���Ʈ ���� ����
void ioFlash_Item::SetNextItemState( ioBaseChar *pWoundChar, ioBaseChar *pAttacker, ioWeapon *pWeapon, bool bDefense, float fDamage )
{
	pAttacker->ClearAttackFireTimeAndSkill();

	//�ڷ���Ʈ ���·� ����
	SetRunEndState( pAttacker );

	if( pWoundChar->IsNeedProcess() )
	{
		if ( pWoundChar && pAttacker )
		{
			m_szTeleportTargetName = pWoundChar->GetCharName();

			if( pWoundChar->IsNeedSendNetwork() )
			{
				SP2Packet kPacket( CUPK_ITEM_GAUGE_OK );
				FillItemGaugePacketHeader( pAttacker, kPacket );
				kPacket << WS_SPECIAL_STATE_PROCESS;
				kPacket << FMSS_SET_TARGET;
				kPacket << m_szTeleportTargetName;
				P2PNetwork::SendToAllPlayingUser( kPacket );
			}
		}
	}
}

/*
//�ڷ���Ʈ Ÿ�� �̸� ����(���� �ʿ��� �����ڿ� ���� ��Ŷ ����)
void ioFlash_Item::SetTeleportTarget( ioBaseChar *pTarget, ioBaseChar *pAttacker )
{	
	if ( pTarget && pAttacker )
	{
		m_szTeleportTargetName = pTarget->GetCharName();

		if( pTarget->IsNeedProcess() && pTarget->IsNeedSendNetwork() )
		{
			SP2Packet kPacket( CUPK_ITEM_GAUGE_OK );
			FillItemGaugePacketHeader( pAttacker, kPacket );
			kPacket << WS_SPECIAL_STATE_PROCESS;
			kPacket << FMSS_SET_TARGET;
			kPacket << m_szTeleportTargetName;
			P2PNetwork::SendToAllPlayingUser( kPacket );
		}
	}
}
*/

//�ڷ���Ʈ ���� ����
void ioFlash_Item::SetTeleportState( ioBaseChar *pOwner )
{
	if( !pOwner )
		return;

	pOwner->ClearSkillReserveKey();

	const AttributeList &rkTeleportList = m_TeleportList;
	
	if( (int)rkTeleportList.size() <= m_nCurTeleportCnt )
	{
		m_FlashSpecialState = FMSS_END;
		return;
	}

	AttackAttribute kAttack = rkTeleportList[m_nCurTeleportCnt];

	ioEntityGroup *pGrp = pOwner->GetGroup();
	int iAniID = pGrp->GetAnimationIdx( kAttack.m_AttackAnimation );
	if( iAniID == -1 )	
		return;

	float fTimeRate = kAttack.m_fAttackAniRate;
	DWORD dwPreDelay = kAttack.m_dwPreDelay;

	pGrp->ClearAllLoopAni( FLOAT100, true );
	pGrp->ClearAllActionAni( FLOAT100, true );
	
	pGrp->SetActionAni( iAniID, FLOAT100, FLOAT100, FLOAT1, FLOAT1/fTimeRate, 0.0f, dwPreDelay, true );	

	DWORD dwCurTime = FRAMEGETTIME();
	m_dwTeleportAttackEndTime = dwCurTime;
	m_dwTeleportAttackEndTime += pGrp->GetAnimationFullTime( iAniID ) * fTimeRate;
	m_dwTeleportAttackEndTime += dwPreDelay;

	m_dwTeleportReserveTime = dwCurTime;
	m_dwTeleportReserveTime += pGrp->GetAnimationEventTime_e( iAniID, "key_reserve" ) * fTimeRate;
	
	pOwner->RefreshFireTimeListWithBase( m_dwTeleportWeaponBaseIndex + m_nCurTeleportCnt,
		iAniID,
		kAttack.m_vWeaponInfoList,
		FTT_EXTEND_ATTACK,
		fTimeRate,
		dwPreDelay );	

	m_nCurTeleportCnt++;
	m_FlashSpecialState = FMSS_TELEPORT;
}

//�ڷ���Ʈ Ű �Է� Ȯ��
void ioFlash_Item::CheckTeleportInputKey( ioBaseChar *pOwner )
{
	if ( !pOwner || !pOwner->IsNeedProcess() )
		return;

	if ( m_bEnableTeleport && m_nCurTeleportCnt < m_nMaxTeleportCnt && m_fCurGauge >= m_fTeleportUseGauge && 
		 m_dwTeleportReserveTime < FRAMEGETTIME() && pOwner->IsAttackKey() )
	{
		m_bEnableTeleport = false;
		DecreaseGauge( m_fTeleportUseGauge );
		if ( m_bCheckMove )
		{
			m_bCheckMove = SetTeleportMove( pOwner );
		}
		SetTeleportState( pOwner );

		if( pOwner->IsNeedSendNetwork() )
		{
			SP2Packet kPacket( CUPK_ITEM_GAUGE_OK );
			FillItemGaugePacketHeader( pOwner, kPacket );
			kPacket << WS_SPECIAL_STATE_PROCESS;
			kPacket << FMSS_TELEPORT;
			kPacket << m_bCheckMove;
			P2PNetwork::SendToAllPlayingUser( kPacket );
		}
	}
	else if ( m_dwTeleportReserveTime < FRAMEGETTIME() )
	{
		if( pOwner->IsAttackKey() )
			m_bAttackKeyReserved = true;

		if( pOwner->IsDefenseKey() )
			m_bDefenseKeyReserved = true;

		if( pOwner->IsJumpKey() )
			m_bJumpKeyReserved = true;
	}
}

//�ڷ���Ʈ �̵�
bool ioFlash_Item::SetTeleportMove( ioBaseChar *pOwner )
{
	if ( !pOwner || !pOwner->IsNeedProcess() || m_szTeleportTargetName.IsEmpty() )
		return false;

	//Ÿ�� ���
	ioBaseChar *pTarget = pOwner->GetBaseChar( m_szTeleportTargetName );
	if ( !pTarget )
		return false;

	D3DXVECTOR3 vTargetPos = pTarget->GetWorldPosition();

	bool bAvailable[TD_MAX];
	for (int nDir=0; nDir<TD_MAX; nDir++)
	{
		bAvailable[nDir] = true;

		D3DXVECTOR3 vDir = ioMath::VEC3_ZERO;
		
		switch( (TeleportDir)nDir )
		{
		case TD_Forward:
			vDir = ioMath::UNIT_Z;
			break;
		case TD_Right:
			vDir = ioMath::UNIT_X;
			break;
		case TD_Backward:
			vDir = -ioMath::UNIT_Z;
			break;
		case TD_Left:	
			vDir = -ioMath::UNIT_X;
			break;
		}	

		D3DXVECTOR3 vTempPos = vTargetPos + ( vDir * m_fTeleportRange);
		float fMapHeight = pOwner->GetCreator()->GetMapHeightOnlyTerrain( vTempPos.x, vTempPos.z );
		if ( fMapHeight > vTempPos.y + m_fTeleportHeightGap || fMapHeight <= 0.0f )
		{
			bAvailable[nDir] = false;
		}
	}

	//��� �̵� �Ұ��� ����
	if ( !bAvailable[TD_Forward] && !bAvailable[TD_Right] && !bAvailable[TD_Backward] && !bAvailable[TD_Left] )
		return false;

	IORandom random;
	DWORD dwSeed = timeGetTime();
	random.SetRandomSeed( dwSeed );

	bool bTeleport = false;
	D3DXVECTOR3 vTeleportPos;

	while( !bTeleport )
	{		
		int nTeleportDir = random.Random( (unsigned int)TD_Forward, (unsigned int)TD_MAX );
		if ( bAvailable[nTeleportDir] )
		{
			switch( (TeleportDir)nTeleportDir )
			{
			case TD_Forward:
				vTeleportPos = vTargetPos + (ioMath::UNIT_Z * m_fTeleportRange);
				break;
			case TD_Right:
				vTeleportPos = vTargetPos + (ioMath::UNIT_X * m_fTeleportRange);
				break;
			case TD_Backward:
				vTeleportPos = vTargetPos - (ioMath::UNIT_Z * m_fTeleportRange);
				break;
			case TD_Left:
				vTeleportPos = vTargetPos - (ioMath::UNIT_X * m_fTeleportRange);	
				break;
			}	
			bTeleport = true;
		}
	}	
	
	if ( bTeleport )
	{
		//����
		pOwner->SetWorldPosition( vTeleportPos );
	
		//ȸ��
		D3DXVECTOR3 vDir = pTarget->GetWorldPosition() - vTeleportPos;
		D3DXVec3Normalize( &vDir, &vDir );
		pOwner->SetTargetRotToDir( vDir, true );

		if( pOwner->IsNeedSendNetwork() )
		{
			SP2Packet kPacket( CUPK_ITEM_GAUGE_OK );
			FillItemGaugePacketHeader( pOwner, kPacket );
			kPacket << WS_SPECIAL_STATE_PROCESS;
			kPacket << FMSS_TELEPORT_MOVE;
			kPacket << vTeleportPos;
			kPacket << vDir;
			P2PNetwork::SendToAllPlayingUser( kPacket );
		}
	}

	return true;
}

//�ڷ���Ʈ ����(���� �ʿ��� �����ڿ� ���� ��Ŷ ����)
void ioFlash_Item::SetEnableTeleport( ioBaseChar *pTarget, ioBaseChar *pAttacker )
{
	if ( pTarget && pAttacker )
	{
		ioHashString szTarget = pTarget->GetCharName();
		if ( szTarget.IsEmpty() || m_szTeleportTargetName.IsEmpty() || szTarget != m_szTeleportTargetName )
			return;

		m_bEnableTeleport = true;

		if( pTarget->IsNeedProcess() && pTarget->IsNeedSendNetwork() )
		{
			SP2Packet kPacket( CUPK_ITEM_GAUGE_OK );
			FillItemGaugePacketHeader( pAttacker, kPacket );
			kPacket << WS_SPECIAL_STATE_PROCESS;
			kPacket << FMSS_ENABLE_TELEPORT;
			kPacket << m_bEnableTeleport;
			P2PNetwork::SendToAllPlayingUser( kPacket );
		}
	}
}

//�������
void ioFlash_Item::CheckReserveInput( ioBaseChar *pOwner )
{
	if ( !pOwner || !pOwner->IsNeedProcess() )
		return;

	if( m_bAttackKeyReserved )
	{
		SetReserveAttack( pOwner );
		return;
	}
	else if( m_bDefenseKeyReserved && pOwner->IsDefenseKeyDown() )
	{
		SetReserveDefence( pOwner );
		return;
	}
	else if( m_bJumpKeyReserved )
	{
		SetReserveJump( pOwner );
		return;
	}
		
	m_FlashSpecialState = FMSS_END;
	if( pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_ITEM_GAUGE_OK );
		FillItemGaugePacketHeader( pOwner, kPacket );
		kPacket << WS_SPECIAL_STATE_PROCESS;
		kPacket << FMSS_END;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}	
}

void ioFlash_Item::SetReserveAttack( ioBaseChar *pOwner )
{
	if ( !pOwner || !pOwner->IsNeedProcess() )
		return;
	
	if ( !IsFloatState(pOwner) )
	{
		pOwner->SetNormalAttack(0);
	}
	m_bAttackKeyReserved = false;
}

void ioFlash_Item::SetReserveDefence( ioBaseChar *pOwner )
{
	if ( !pOwner || !pOwner->IsNeedProcess() )
		return;

	pOwner->SetDefenseState();
	m_bDefenseKeyReserved = false;
}

void ioFlash_Item::SetReserveJump( ioBaseChar *pOwner )
{
	if ( !pOwner || !pOwner->IsNeedProcess() )
		return;

	pOwner->SetJumpState();
	m_bJumpKeyReserved = false;	
}

//Ÿ���� ���̵� ������ ���� �ִ°�?
bool ioFlash_Item::IsTargetHasHideBuff( ioBaseChar *pOwner, ioHashString szTarget )
{
	if ( !pOwner || szTarget.IsEmpty() )
		return false;

	ioBaseChar *pTarget = pOwner->GetBaseChar( szTarget );
	if ( pTarget && pTarget->HasHideBuff() )
	{
		return true;
	}

	return false;
}

void ioFlash_Item::CreateAttachWeapon( ioBaseChar *pOwner )
{
	if ( !pOwner || !pOwner->IsNeedProcess())
		return;

	//������� (���� �޷��ִ� ����)
	FireTime kFireTime;
	kFireTime.dwStart = FRAMEGETTIME();

	kFireTime.iAttributeIdx = m_nRunAttachWeaponNum;
	kFireTime.iResistanceIdx = 0;
	kFireTime.szWoundedAni  = "";
	kFireTime.dwWoundedAniDuration = 0;
	kFireTime.bLoopWoundedAni = false;
	m_dwRunWeaponBaseIndex = pOwner->IncreaseWeaponIndexBase();
	kFireTime.dwWeaponIdx = m_dwRunWeaponBaseIndex;
	kFireTime.eFireTimeType = FTT_NORMAL_ATTACK;

	D3DXVECTOR3 vPos = pOwner->GetMidPositionByRate();
	D3DXVECTOR3 vDir = pOwner->GetTargetRot() * D3DXVECTOR3( 0, 0, -1 );
	pOwner->ExtendFireExplicit( kFireTime, vPos, vDir, GetName() );

	if ( pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_ITEM_GAUGE_OK );
		FillItemGaugePacketHeader( pOwner, kPacket );
		kPacket << WS_SPECIAL_STATE_PROCESS;
		kPacket << FMSS_CREATE_ATTACH_WEAPON;
		kPacket << m_dwRunWeaponBaseIndex;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

ioFlashSpecialState::ioFlashSpecialState()
{
}

ioFlashSpecialState::~ioFlashSpecialState()
{
}

void ioFlashSpecialState::Initialize( ioBaseChar* pOwner, CharState eCurrState )
{
	ioSpecialStateBase::Initialize( pOwner, eCurrState );
}

void ioFlashSpecialState::Finalize( ioBaseChar* pOwner, CharState eCurrState, CharState eNewState )
{
	ioSpecialStateBase::Finalize( pOwner, eCurrState, eNewState );

	ioFlash_Item *pFlashItem = ToFlashItem( pOwner->GetPriorityItem( SPT_ATTACK ) );
	if( pFlashItem )
		pFlashItem->ClearState( pOwner );
}

bool ioFlashSpecialState::IsEntityCollisionSkipState( const ioBaseChar *const pOwner, ioGameEntity* pColEntity ) const 
{ 
	ioFlash_Item *pFlashItem = ToFlashItem( pOwner->GetPriorityItem(SPT_ATTACK) );
	if( pFlashItem && pFlashItem->IsFlashSpecialCharCollisionSkipState() )
		return true;

	return false; 
}

bool ioFlashSpecialState::IsCharCollisionSkipState( const ioBaseChar *const pOwner, DWORD dwStartTime, DWORD dwEndTime, DWORD dwSkipType, bool bTeamOnly, bool bDefense, TeamType eOwnerTeam, TeamType eOtherTeam ) const
{
	ioFlash_Item *pFlashItem = ToFlashItem( pOwner->GetPriorityItem(SPT_ATTACK) );
	if( pFlashItem && pFlashItem->IsFlashSpecialCharCollisionSkipState() )
		return true;

	if( Help::CheckCharColState( dwStartTime, dwEndTime, dwSkipType, bTeamOnly, bDefense, eOwnerTeam, eOtherTeam ) )
		return true;

	return false;
}

void ioFlashSpecialState::CheckSpecialState( ioBaseChar* pOwner )
{
	ioFlash_Item *pFlash_Item = ToFlashItem( pOwner->GetPriorityItem( SPT_ATTACK ) );
	if( pFlash_Item )
		pFlash_Item->ProcessFlashSpecialState( pOwner );
	else
	{
		pOwner->SetState( CS_DELAY );
		pOwner->SetFallState( true );
		return;
	}
}

void ioFlashSpecialState::CheckFallStateChange( ioBaseChar* pOwner, float fMinFallHeightRate, float fBottomHeight, bool bBottomMaxCheck, float fMaxBottomHeight )
{
	bool FallCheck = false;
	if( GetFlashSpecialState( pOwner ) == ioFlash_Item::FMSS_CHECK_CHARGING )
		FallCheck = true;

	if( FallCheck )
	{				
		float fMinFallinRate = ( pOwner->GetWorldCollisionBox().GetExtents( 1 ) * 2.0f ) * pOwner->GetMinFallHeightRate();
		float fCharPosGap = pOwner->GetWorldPosition().y - fBottomHeight;
		if( fCharPosGap > fMinFallinRate )
			pOwner->SetFallState( true );
	}
}

bool ioFlashSpecialState::ProcessMove( ioBaseChar* pOwner, float fTimePerSec, DWORD dwPreTime, DWORD dwCurTime )
{
	return true;
}

int ioFlashSpecialState::GetFlashSpecialState( ioBaseChar* pOwner )
{
	ioFlash_Item *pFlash_Item = ToFlashItem( pOwner->GetPriorityItem( SPT_ATTACK ) );
	if( !pFlash_Item )
		return static_cast<int>( ioFlash_Item::FMSS_CHECK_CHARGING );

	return static_cast<int>( pFlash_Item->GetFlashSpecialState() );
}