
#include "stdafx.h"

#include "ioPsychoMetry_Item.h"

#include "ioBaseChar.h"
#include "ioPlayStage.h"

#include "WeaponDefine.h"

ioPsychoMetry_Item::ioPsychoMetry_Item()
{	
	m_fCurGauge = 0.0f;
	Init();
}

ioPsychoMetry_Item::ioPsychoMetry_Item( const ioPsychoMetry_Item &rhs )
	: ioWeaponItem( rhs ),
	  m_AttackReadyAniList( rhs.m_AttackReadyAniList ),
	  m_fMaxGauge( rhs.m_fMaxGauge ),
	  m_fNeedGauge( rhs.m_fNeedGauge ),  
	  m_fIncreaseGauge( rhs.m_fIncreaseGauge ),
	  m_vChargeComboAttListUp( rhs.m_vChargeComboAttListUp ),
	  m_vChargeComboAttListCenter( rhs.m_vChargeComboAttListCenter ),
	  m_vChargeComboAttListDown( rhs.m_vChargeComboAttListDown ),
	  m_fChargeComboAttAngle( rhs.m_fChargeComboAttAngle ),
	  m_fColAngle( rhs. m_fColAngle ),
	  m_fColRange( rhs.m_fColRange ),
	  m_dwSPWaitDuration( rhs.m_dwSPWaitDuration ),
	  m_dwInputWaitDuration( rhs.m_dwInputWaitDuration ),
	  m_nMaxComboCnt( rhs.m_nMaxComboCnt ),
	  m_SuperPowerAttack( rhs.m_SuperPowerAttack ),
	  m_TeleportAttack( rhs.m_TeleportAttack ),
	  m_fTeleportOffset( rhs.m_fTeleportOffset ),
	  m_fEndJumpPower( rhs.m_fEndJumpPower ),
	  m_bSetDownAim( rhs.m_bSetDownAim ),
	  m_fJumpHeightGap( rhs.m_fJumpHeightGap )
{
	m_fCurGauge = 0.0f;
	Init();
}

ioPsychoMetry_Item::~ioPsychoMetry_Item()
{
}

void ioPsychoMetry_Item::LoadProperty( ioINILoader &rkLoader )
{
	ioWeaponItem::LoadProperty( rkLoader );

	LoadAttackProperty( rkLoader );
	
	//������ ����
	m_fMaxGauge = rkLoader.LoadFloat_e( "max_gauge", FLOAT100 );
	m_fNeedGauge = rkLoader.LoadFloat_e( "need_gauge", FLOAT100 );
	m_fIncreaseGauge = rkLoader.LoadFloat_e( "increase_gauge", FLOAT1 );

	//Ÿ�� ����
	m_fColAngle = rkLoader.LoadFloat_e( "target_angle", 0.0f );
	m_fColRange = rkLoader.LoadFloat_e( "target_range", 0.0f );

	m_bSetDownAim = rkLoader.LoadBool_e( "set_down_target", false );

	//�ʴɷ� ��� �ð�
	m_dwSPWaitDuration = rkLoader.LoadInt_e( "super_attack_duration", 0 );
	m_dwInputWaitDuration = rkLoader.LoadInt_e( "input_wait_duration", 0);

	//�ڷ���Ʈ ������
	m_fTeleportOffset = rkLoader.LoadFloat_e( "teleport_offset", 0.0f );
	//�����Ŀ�
	m_fEndJumpPower = rkLoader.LoadFloat_e( "teleport_end_jump_power", 0.0f );

	m_fJumpHeightGap = rkLoader.LoadFloat_e( "teleport_jump_height_gap", 0.0f );
}

ioItem* ioPsychoMetry_Item::Clone()
{
	return new ioPsychoMetry_Item( *this );
}

ioWeaponItem::WeaponSubType ioPsychoMetry_Item::GetSubType() const
{
	return WST_PSYCO_ITEM;
}

/////////////////////�⺻ ����
//���� ����
void ioPsychoMetry_Item::SetNormalAttack( ioBaseChar *pOwner, int iCurCombo )
{
	ClearCancelInfo();

	Init();

	DWORD dwCurTime = FRAMEGETTIME();
	int iComboCnt = m_vChargeComboAttListCenter.size();
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

		pGrp->ClearAllActionAni( FLOAT100, true );
		pGrp->ClearAllLoopAni( FLOAT100, true );

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

void ioPsychoMetry_Item::CheckNormalAttack( ioBaseChar *pOwner )
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

//���� ����
void ioPsychoMetry_Item::SetExtraInfoToWeapon( ioWeapon *pWeapon, bool bJumpAttack )
{
	ioAttackableItem::SetExtraInfoToWeapon( pWeapon, bJumpAttack );	
		
	ioTraceWeapon5 *pTrace = ToTraceWeapon5( pWeapon );
	if( pTrace )
	{
		//�ʴɷ�ź�̸�
		if ( !m_bSetTarget )
		{			
			pTrace->SetTraceTarget( m_szFirstTarget );
			pTrace->SetTraceState( NULL );	
		}
		else if ( m_bSetTarget )
		{
			pTrace->SetTraceTarget( m_szTargetName );
			pTrace->SetTraceState( NULL );
		}
	}
}

//����ȭ
void ioPsychoMetry_Item::OnItemGaugeOK( ioBaseChar *pOwner, SP2Packet &rkPacket )
{
	if( !pOwner ) 
		return;

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
			pOwner->SetState( CS_PSYCHO_SPECIAL );
			m_PsychoSpecialState = PSS_None;
		}
		break;
	case WS_SPECIAL_STATE_PROCESS:
		{
			RecvSpecialState( pOwner, rkPacket );
		}
		break;
	}
}

bool ioPsychoMetry_Item::IsChargeAttackState( ioBaseChar *pOwner )
{
	if( pOwner->GetState() == CS_ATTACK )
	{
		switch( m_PsychoSpecialState )
		{
		case PSS_Find_Target:
		case PSS_Wait:
		case PSS_Combo:
		case PSS_Teleport:
			return true;
		}
	}

	return false;
}

///////////////////////������ ����///////////////////////////////////////////////
//������ ���� (������ ȸ��)
void ioPsychoMetry_Item::UpdateExtraData( ioBaseChar *pOwner )
{
	if( !pOwner ) return;

	float fTimePerSec = g_FrameTimer.GetSecPerFrame();

	if ( pOwner->GetState() == CS_JUMP && pOwner->GetJumpState() == JS_JUMP_ATTACK )
	{
		return;
	}

	if( pOwner->IsSystemState() )
		return;

	switch( pOwner->GetState() )
	{
	case CS_DELAY:
	case CS_DASH:
	case CS_RUN:
	case CS_JUMP:
		if( m_fCurGauge < m_fMaxGauge )
		{
			float fGauge = m_fIncreaseGauge * fTimePerSec;

			m_fCurGauge += fGauge;
			m_fCurGauge = min( m_fCurGauge, m_fMaxGauge );
		}
		return;
	}
}

//������ ���� �� ������ ���
int ioPsychoMetry_Item::GetMaxBullet()
{
	return (int)m_fMaxGauge;
}

int ioPsychoMetry_Item::GetCurBullet()
{
	float fCurCoolTime = m_fCurGauge;
	fCurCoolTime = max( 0, min( fCurCoolTime, m_fMaxGauge ) );

	return (int)fCurCoolTime;
}

void ioPsychoMetry_Item::SetCurBullet( int iCurBullet )
{
	if( iCurBullet < 0 )
		return;

	float fCurBullet = iCurBullet;
	fCurBullet = max( 0, min( fCurBullet, m_fMaxGauge ) );

	m_fCurGauge = fCurBullet;
}

void ioPsychoMetry_Item::InitExtraGauge()
{
	m_fCurGauge = 0.0f;
}

void ioPsychoMetry_Item::MaxExtraGauge()
{
	m_fCurGauge = m_fMaxGauge;
}

float ioPsychoMetry_Item::GetMaxExtraGauge()
{
	return m_fMaxGauge;
}

float ioPsychoMetry_Item::GetCurExtraGauge()
{
	return m_fCurGauge;
}

void ioPsychoMetry_Item::SetCurExtraGauge( float fGauge )
{
	m_fCurGauge = fGauge;
	m_fCurGauge = (float)max( 0, min( m_fCurGauge, m_fMaxGauge ) );
}

int ioPsychoMetry_Item::GetNeedBullet()
{
	return (int)m_fNeedGauge;
}

//������ ���
void ioPsychoMetry_Item::DecreaseGauge( float fNeedGauge )
{
	m_fCurGauge -= fNeedGauge;
	m_fCurGauge = (float)max( 0.f, min( m_fCurGauge, m_fMaxGauge ) );
}

/////////////////////////////////////////////////////////////////////////////////
//�߰� �б�
void ioPsychoMetry_Item::LoadAttackProperty( ioINILoader &rkLoader )
{
	char szBuf[MAX_PATH], szKey[MAX_PATH];

	//�غ� ����
	int iExtendCnt = rkLoader.LoadInt_e( "extend_attack_cnt", 0 );	
	m_AttackReadyAniList.clear();

	for( int i=0 ; i<iExtendCnt ; i++ )
	{
		wsprintf_e( szKey, "attack_ready_animation%d", i+1 );
		rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );
		m_AttackReadyAniList.push_back( ioHashString(szBuf) );
	}

	//�ʴɷ�ź ����
	LoadAttackAttribute_e( "super_power_attack", m_SuperPowerAttack, rkLoader );
	//�ڷ���Ʈ ����
	LoadAttackAttribute_e( "teleport_attack", m_TeleportAttack, rkLoader );

	//���� Ȯ��
	m_fChargeComboAttAngle = rkLoader.LoadFloat_e( "charge_combo_attack_Angle", 45.0f );

	//���� �޺�
	int iComboMax = rkLoader.LoadInt_e( "charge_combo_max_cnt", 1 );
	m_nMaxComboCnt = iComboMax;

	m_vChargeComboAttListUp.clear();
	m_vChargeComboAttListCenter.clear();
	m_vChargeComboAttListDown.clear();

	m_vChargeComboAttListUp.reserve( iComboMax );
	m_vChargeComboAttListCenter.reserve( iComboMax );
	m_vChargeComboAttListDown.reserve( iComboMax );

	for( int i=0; i < iComboMax; ++i )
	{
		char szBufTmp[MAX_PATH];

		AttackAttribute kAttribute;
		wsprintf_e( szBuf, "charge_combo_attack%d", i+1 );
		LoadAttackAttribute( szBuf, kAttribute, rkLoader );
		m_vChargeComboAttListCenter.push_back( kAttribute );

		wsprintf_e( szBuf, "charge_combo_attack%d_up", i+1 );
		rkLoader.LoadString( szBuf, "", szBufTmp, MAX_PATH );
		kAttribute.m_AttackAnimation = szBufTmp;
		m_vChargeComboAttListUp.push_back( kAttribute );

		wsprintf_e( szBuf, "charge_combo_attack%d_down", i+1 );
		rkLoader.LoadString( szBuf, "", szBufTmp, MAX_PATH );
		kAttribute.m_AttackAnimation = szBufTmp;
		m_vChargeComboAttListDown.push_back( kAttribute );
	}
}

//����ȭ
void ioPsychoMetry_Item::RecvSpecialState( ioBaseChar *pOwner, SP2Packet &rkPacket )
{
	if( !pOwner ) 
		return;

	int nSpecialState;
	rkPacket >> nSpecialState;

	switch( nSpecialState )
	{
	case PSS_Find_Target:
		{	
			rkPacket >> m_dwWeaponBaseIndex;
			D3DXQUATERNION qtRot;
			rkPacket >> qtRot;
			rkPacket >> m_szFirstTarget;

			pOwner->SetTargetRot( qtRot );
			SetSuperPowerState( pOwner );
		}
		break;
	case PSS_Set_Target:
		{
			rkPacket >> m_szTargetName;
			rkPacket >> m_bSetTarget;
		}
		break;
	case PSS_Combo:
		{
			D3DXQUATERNION qtRot;
			rkPacket >> m_nCurComboCnt;
			rkPacket >> qtRot;

			pOwner->SetTargetRot( qtRot );
			SetEnergyFireState( pOwner );	
		}
		break;
	case PSS_Teleport:
		{
			D3DXVECTOR3 vPos;
			D3DXQUATERNION qtRot;
			rkPacket >> m_dwWeaponBaseIndex;
			rkPacket >> vPos;
			rkPacket >> qtRot;

			pOwner->SetTargetRot( qtRot );
			pOwner->SetWorldPosition( vPos );

			SetTeleportAttackState( pOwner );
		}
		break;
	case PSS_End:
		{
			m_PsychoSpecialState = PSS_End;
		}
		break;
	case PSS_End_Jump:
		{
			pOwner->SetSKillEndJumpState( m_fEndJumpPower, false, false, false, true );
		}
		break;
	}
}

//�ʱ�ȭ
void ioPsychoMetry_Item::Init()
{
	m_SortList.erase( m_SortList.begin(), m_SortList.end() );
	m_SortList.clear();

	m_dwCheckWaitTime = 0;
	m_dwCheckInputWaitTime = 0;

	m_dwComboReserveTime = 0;
	m_dwComboEndTime = 0;

	m_dwTeleportAttackEndTime = 0;
	m_dwTeleportReserveTime = 0;

	m_nNormalCombo = 0;
	m_nCurComboCnt = 0;

	m_WeaponItemState = WS_NONE;
	m_PsychoSpecialState = PSS_None;

	m_bSetTarget = false;
	m_szTargetName.Clear();
	m_szFirstTarget.Clear();

	m_vTargetDir = ioMath::VEC3_ZERO;
	m_vTargetDist = ioMath::VEC3_ZERO;

	m_bAttackKeyReserved = false;
	m_bDefenseKeyReserved = false;
	m_bJumpKeyReserved = false;
}

//��¡ Ȯ��
void ioPsychoMetry_Item::CheckCharging( ioBaseChar *pOwner )
{
	if( !pOwner->IsNeedProcess()  )	
		return;

	//������ Ȯ��
	if( pOwner->IsAttackKeyDown() && !pOwner->IsHasCrown() && GetCurBullet() >= GetNeedBullet() )
	{
		if( m_dwAttackStartTime + m_dwChargeTime <= FRAMEGETTIME() )
		{
			//Ư�� ���·� ����
			SetPsychoSpecialState( pOwner );
		}
	}
	else
	{
		//�Ϲ� ����
		SetNormalAttackState( pOwner );
	}
}

//�⺻ ���� ���·� ����
void ioPsychoMetry_Item::SetNormalAttackState( ioBaseChar *pOwner )
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

/////////////////////////////////////////////////////////////////////////////////
void ioPsychoMetry_Item::ClearState( ioBaseChar *pOwner )
{
	Init();
}

//ĳ���͸� Ư�� ���·� ���� (���� ���¿��� Ư�� ���·� ����)
void ioPsychoMetry_Item::SetPsychoSpecialState( ioBaseChar *pOwner )
{
	if( !pOwner || !pOwner->IsNeedProcess() )
		return;

	pOwner->SetState( CS_PSYCHO_SPECIAL );
	
	m_PsychoSpecialState = PSS_Find_Target;

	if( pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_ITEM_GAUGE_OK );
		FillItemGaugePacketHeader( pOwner, kPacket );
		kPacket << WS_SPECIAL_STATE_INIT;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

//ĳ���� Ư�� ���� ���μ���
void ioPsychoMetry_Item::ProcessPsychoSpecialState( ioBaseChar *pOwner )
{
	if( !pOwner )
		return;

	DWORD dwCurTime = FRAMEGETTIME();
	
	switch( m_PsychoSpecialState )
	{
	case PSS_Find_Target:
		{
			if ( !FindTarget( pOwner ) )
				m_szFirstTarget.Clear();

			m_dwWeaponBaseIndex = pOwner->IncreaseWeaponIndexBase();
			DecreaseGauge( m_fNeedGauge );
			SetSuperPowerState( pOwner );
		}
		break;
	case PSS_Wait:
		{			
			//���� �ð� �� ����
			if ( m_dwCheckWaitTime + m_dwSPWaitDuration < dwCurTime )
			{
				m_PsychoSpecialState = PSS_End;
				return;
			}			

			//�ʴɷ�ź Ȯ��
			if ( m_bSetTarget )
			{
				m_dwCheckInputWaitTime = dwCurTime;
				m_PsychoSpecialState = PSS_InputWait;
				return;
			}
		}
		break;
	case PSS_InputWait:
		{
			//���� �ð� �� ����
			if ( m_dwCheckInputWaitTime + m_dwInputWaitDuration < dwCurTime )
			{
				m_PsychoSpecialState = PSS_End;
				return;
			}			

			CheckInputKey( pOwner );
		}
		break;
	case PSS_Combo:
		{
			if ( m_nCurComboCnt < m_nMaxComboCnt )
				CheckInputKey( pOwner );
			else
				ReserveInputKey( pOwner );

			//Ư�� ���� ���� (Ű�� ���� �� �ִϸ��̼��� ����Ǹ� ����)
			if ( m_dwComboEndTime > 0 && m_dwComboEndTime < dwCurTime )
			{
				CheckReserveState( pOwner );
				return;
			}
		}
		break;
	case PSS_Teleport:
		{
			ReserveInputKey( pOwner );

			//Ư�� ���� ���� (Ű�� ���� �� �ִϸ��̼��� ����Ǹ� ����)
			if ( m_dwTeleportAttackEndTime > 0 && m_dwTeleportAttackEndTime < dwCurTime )
			{
				CheckReserveState( pOwner );
				return;
			}
		}
		break;
	case PSS_End:
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
						kPacket << PSS_End_Jump;
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

/*
//Ÿ�� ����(���� �ʿ��� ������ ������ ���� ��Ŷ ����)
void ioPsychoMetry_Item::SetTarget( ioBaseChar *pTarget, ioBaseChar *pAttacker )
{
	if ( pTarget && pAttacker )
	{
		m_szTargetName = pTarget->GetCharName();
		m_bSetTarget = true;

		if( pTarget->IsNeedProcess() && pTarget->IsNeedSendNetwork() )
		{
			SP2Packet kPacket( CUPK_ITEM_GAUGE_OK );
			FillItemGaugePacketHeader( pAttacker, kPacket );
			kPacket << WS_SPECIAL_STATE_PROCESS;
			kPacket << PSS_Set_Target;
			kPacket << m_szTargetName;
			kPacket << m_bSetTarget;
			P2PNetwork::SendToAllPlayingUser( kPacket );
		}
	}
}
*/

/////////////////////////////////////////////////////////////////////////////////
//���� ����� Ÿ�� �˻�
bool ioPsychoMetry_Item::FindTarget( ioBaseChar *pOwner )
{
	if( !pOwner || !pOwner->IsNeedProcess() ) 
		return false;

	ioPlayStage *pStage = pOwner->GetCreator();
	if ( !pStage )
		return false;

	//Ÿ�� ����Ʈ
	BaseCharList vTargetList;

	//��ġ ����
	D3DXVECTOR3 vPos = pOwner->GetWorldPosition();

	//Ÿ�� ����Ʈ ���
	pOwner->GetTargetCharListBySphere( vPos, m_fColRange, m_fColAngle, vTargetList );

	//Ÿ�� ����Ʈ�� ����ִٸ� Ư�� ����
	if ( vTargetList.empty() )
		return false;

	//Ÿ�� ����Ʈ ����
	SortTargetList( pOwner, vTargetList );
	
	//ȸ��
	if ( !m_SortList.empty() )
	{
		ioBaseChar *pTarget = m_SortList[0].pChar;
		if ( pTarget )
		{
			m_szFirstTarget = pTarget->GetCharName();

			D3DXVECTOR3 vDir = pTarget->GetWorldPosition() - pOwner->GetWorldPosition();
			D3DXVec3Normalize( &vDir, &vDir );
			pOwner->SetTargetRotToDir( vDir, true );
			return true;
		}
	}

	return false;
}

//Ÿ�� ����Ʈ ����� ������ ����
void ioPsychoMetry_Item::SortTargetList( ioBaseChar *pOwner, const BaseCharList &TargetList )
{
	if( !pOwner )	
		return;

	m_SortList.erase( m_SortList.begin(), m_SortList.end() );
	m_SortList.reserve( TargetList.size() );

	D3DXVECTOR3 vDiff;
	D3DXVECTOR3 vOwnerPos = pOwner->GetWorldPosition();

	BaseCharList::const_iterator iter;
	for( iter=TargetList.begin(); iter!=TargetList.end(); ++iter )
	{
		ioBaseChar *pCurChar = *iter;
		if( pCurChar != pOwner )
		{
			//���� ���ų� Ÿ���� �ɼ� ������ ����Ʈ�� ���� ����
			if ( pOwner->GetTeam() == pCurChar->GetTeam() )
				continue;

			if ( !IsEnableTarget( pCurChar ) )
				continue;

			vDiff = pCurChar->GetWorldPosition() - vOwnerPos;
			float fCurDistSq = D3DXVec3LengthSq( &vDiff );

			DistInfo sDist;
			sDist.pChar = pCurChar;
			sDist.fDistSq = fCurDistSq;
			m_SortList.push_back( sDist );
		}
	}

	std::sort( m_SortList.begin(), m_SortList.end() );
}

//Ÿ�� �������� Ȯ��
bool ioPsychoMetry_Item::IsEnableTarget( ioBaseChar *pTarget )
{
	if ( !pTarget || pTarget->IsPrisonerMode() )
		return false;
	
	if( pTarget->IsSystemState() )
		return false;
		
	if( !pTarget->IsEnableTarget() )
		return false;

	CharState eTargetState = pTarget->GetState();
	switch( eTargetState )
	{
	case CS_BLOW_WOUNDED:
		{
			if ( !pTarget->IsFloatingState() && !m_bSetDownAim )
				return false;
		}
	case CS_BOUND_BLOW_WOUNDED:
		{
			if ( !pTarget->IsFloatingState() && !m_bSetDownAim )
				return false;
		}
	case CS_ETC_STATE:
		{
			if ( pTarget->IsApplyDownState(false) && !m_bSetDownAim )
				return false;
		}
	case CS_USING_SKILL:
		{
			if ( pTarget->IsApplyDownState(false) && !m_bSetDownAim )
				return false;
		}
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////////
//�ʴɷ�ź �ִϸ��̼� ����
void ioPsychoMetry_Item::SetSuperPowerState( ioBaseChar *pOwner )
{
	if ( !pOwner )
		return;

	//�ʴɷ� ���� �ִϸ��̼� ����
	ioEntityGroup *pGrp = pOwner->GetGroup();
	int iAniID = pGrp->GetAnimationIdx( m_SuperPowerAttack.m_AttackAnimation );
	if( iAniID == -1 )	
		return;

	float fTimeRate = m_SuperPowerAttack.m_fAttackAniRate;
	DWORD dwPreDelay = m_SuperPowerAttack.m_dwPreDelay;

	pGrp->ClearAllLoopAni( FLOAT100, true );
	pGrp->ClearAllActionAni( FLOAT100, true );

	pGrp->SetActionAni( iAniID, FLOAT100, FLOAT100, FLOAT1, FLOAT1/fTimeRate, 0.0f, dwPreDelay, true );	
	
	pOwner->RefreshFireTimeListWithBase( m_dwWeaponBaseIndex,
		iAniID,
		m_SuperPowerAttack.m_vWeaponInfoList,
		FTT_NORMAL_ATTACK,
		fTimeRate,
		dwPreDelay );	

	//�����̵� ����
	pOwner->SetReservedSliding( m_SuperPowerAttack.m_vForceInfoList, iAniID, fTimeRate, 0 );

	DWORD dwCurTime = FRAMEGETTIME();
	m_dwCheckWaitTime = dwCurTime;

	m_PsychoSpecialState = PSS_Wait;

	if( pOwner->IsNeedProcess() && pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_ITEM_GAUGE_OK );
		FillItemGaugePacketHeader( pOwner, kPacket );
		kPacket << WS_SPECIAL_STATE_PROCESS;
		kPacket << PSS_Find_Target;
		kPacket << m_dwWeaponBaseIndex;
		kPacket << pOwner->GetTargetRot();
		kPacket << m_szFirstTarget;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

//�޺� ���� ����
void ioPsychoMetry_Item::SetEnergyFireState( ioBaseChar *pOwner )
{
	if( !pOwner )
		return;

	pOwner->ClearSkillReserveKey();
		
	const AttackAttribute *pAttr = GetCorrectDirAttibute( pOwner );
	if ( !pAttr )
		return;

	ioEntityGroup *pGrp = pOwner->GetGroup();
	int iAniID = pGrp->GetAnimationIdx( pAttr->m_AttackAnimation );
	if( iAniID == -1 )	
		return;

	float fTimeRate = pAttr->m_fAttackAniRate;
	DWORD dwPreDelay = pAttr->m_dwPreDelay;

	pGrp->ClearAllLoopAni( FLOAT100, true );
	pGrp->ClearAllActionAni( FLOAT100, true );

	pGrp->SetActionAni( iAniID, FLOAT100, FLOAT100, FLOAT1, FLOAT1/fTimeRate, 0.0f, dwPreDelay, true );	

	DWORD dwCurTime = FRAMEGETTIME();
	m_dwComboEndTime = dwCurTime;
	m_dwComboEndTime += pGrp->GetAnimationFullTime( iAniID ) * fTimeRate;
	m_dwComboEndTime += dwPreDelay;

	m_dwComboReserveTime = dwCurTime;
	m_dwComboReserveTime += pGrp->GetAnimationEventTime_e( iAniID, "key_reserve" ) * fTimeRate;

	pOwner->RefreshFireTimeListWithBase( m_dwWeaponBaseIndex + m_nCurComboCnt + 1,
		iAniID,
		pAttr->m_vWeaponInfoList,
		FTT_NORMAL_ATTACK,
		fTimeRate,
		dwPreDelay );	

	m_PsychoSpecialState = PSS_Combo;	

	if( pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_ITEM_GAUGE_OK );
		FillItemGaugePacketHeader( pOwner, kPacket );
		kPacket << WS_SPECIAL_STATE_PROCESS;
		kPacket << PSS_Combo;
		kPacket << m_nCurComboCnt;
		kPacket << pOwner->GetTargetRot();
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}

	m_nCurComboCnt++;
}

//�޺��� ���� Ȯ��
AttackAttribute* ioPsychoMetry_Item::GetCorrectDirAttibute( ioBaseChar *pOwner )
{
	if ( !pOwner || m_szTargetName.IsEmpty() )
		return NULL;

	bool bUp = false;

	D3DXVECTOR3 vPosSrc = pOwner->GetWorldPosition();

	ioBaseChar *pTarget = pOwner->GetBaseChar( m_szTargetName );
	if( !pTarget )
		return &m_vChargeComboAttListCenter[m_nCurComboCnt];

	D3DXVECTOR3 vPosDest = pTarget->GetWorldPosition();

	if( (vPosDest.y - vPosSrc.y) > 0 )
		bUp = true;

	D3DXVECTOR3 v1 = vPosDest - vPosSrc;
	D3DXVECTOR3 v2 = pOwner->GetTargetRot() * D3DXVECTOR3( 0, 0, -1 );

	D3DXVec3Normalize( &v1, &v1);
	D3DXVec3Normalize( &v2, &v2);

	float fDotValue = (float)D3DXVec3Dot( &v1, &v2 );
	float fAngle = RADtoDEG( acosf( fDotValue ) );

	if( fAngle > m_fChargeComboAttAngle )
	{
		if( bUp )
			return &m_vChargeComboAttListUp[m_nCurComboCnt];		
		else
			return &m_vChargeComboAttListDown[m_nCurComboCnt];		
	}	

	return &m_vChargeComboAttListCenter[m_nCurComboCnt];
}

//�����̵� �� ����
void ioPsychoMetry_Item::SetTeleportAttackState( ioBaseChar *pOwner )
{
	if( !pOwner )
		return;
	
	ioEntityGroup *pGrp = pOwner->GetGroup();
	int iAniID = pGrp->GetAnimationIdx( m_TeleportAttack.m_AttackAnimation );
	if( iAniID == -1 )	
		return;

	float fTimeRate = m_TeleportAttack.m_fAttackAniRate;
	DWORD dwPreDelay = m_TeleportAttack.m_dwPreDelay;

	pGrp->ClearAllLoopAni( FLOAT100, true );
	pGrp->ClearAllActionAni( FLOAT100, true );

	pGrp->SetActionAni( iAniID, FLOAT100, FLOAT100, FLOAT1, FLOAT1/fTimeRate, 0.0f, dwPreDelay, true );	

	DWORD dwCurTime = FRAMEGETTIME();
	m_dwTeleportAttackEndTime = dwCurTime;
	m_dwTeleportAttackEndTime += pGrp->GetAnimationFullTime( iAniID ) * fTimeRate;
	m_dwTeleportAttackEndTime += dwPreDelay;

	m_dwTeleportReserveTime = dwCurTime;
	m_dwTeleportReserveTime += pGrp->GetAnimationEventTime_e( iAniID, "key_reserve" ) * fTimeRate;

	pOwner->RefreshFireTimeListWithBase( m_dwWeaponBaseIndex,
		iAniID,
		m_TeleportAttack.m_vWeaponInfoList,
		FTT_NORMAL_ATTACK,
		fTimeRate,
		dwPreDelay );	

	m_PsychoSpecialState = PSS_Teleport;

	if( pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_ITEM_GAUGE_OK );
		FillItemGaugePacketHeader( pOwner, kPacket );
		kPacket << WS_SPECIAL_STATE_PROCESS;
		kPacket << PSS_Teleport;
		kPacket << m_dwWeaponBaseIndex;
		kPacket << pOwner->GetWorldPosition();
		kPacket << pOwner->GetTargetRot();
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

//�����̵�
void ioPsychoMetry_Item::SetTeleportMove( ioBaseChar *pOwner )
{
	if ( !pOwner || m_szTargetName.IsEmpty() )
		return;

	SetRotateToTarget( pOwner );

	//Ÿ�� ��� (����� �ӽ÷� ����)
	ioBaseChar *pTarget = pOwner->GetBaseChar( m_szTargetName );
	if ( pTarget )
	{		
		//�Ÿ� ���(X, Z)
		float fDist = D3DXVec3Length( &m_vTargetDist );

		//�����º��� Ŭ���� �̵�
		D3DXVECTOR3 vNewPos = ioMath::VEC3_ZERO;

		fDist -= m_fTeleportOffset;

		//���� ��ġ ���
		D3DXVECTOR3 vOnwerPos = pOwner->GetWorldPosition();
		//Ÿ�� ��ġ ���
		D3DXVECTOR3 vTargetPos = pTarget->GetWorldPosition();
		//���� ����		
		vOnwerPos.y = vTargetPos.y;

		vNewPos = vOnwerPos;
		vNewPos += ( m_vTargetDir * fDist );

		float fMapHeight = pOwner->GetCreator()->GetMapHeightNoneEntity( vNewPos.x, vNewPos.z );
		if ( fMapHeight > vNewPos.y )
		{
			//���� �� �̵�
			pOwner->SetWorldPosition( vTargetPos );
		}
		else
		{
			//���� �� �̵�
			pOwner->SetWorldPosition( vNewPos );
		}

		SetRotateToTarget( pOwner );
	}
}

//����� �ڷ���Ʈ �̵�
void ioPsychoMetry_Item::SetDropZoneTeleportMove( ioBaseChar *pOwner )
{
	if ( !pOwner || m_szTargetName.IsEmpty() )
		return;

	SetRotateToTarget( pOwner );

	//Ÿ�� ��� (����� �ӽ÷� ����)
	ioBaseChar *pTarget = pOwner->GetBaseChar( m_szTargetName );
	if ( pTarget )
	{		
		//�Ÿ� ���(X, Z)
		float fDist = D3DXVec3Length( &m_vTargetDist );

		//�����º��� Ŭ���� �̵�
		D3DXVECTOR3 vNewPos = ioMath::VEC3_ZERO;
							
		fDist -= m_fTeleportOffset;

		//���� ��ġ ���
		D3DXVECTOR3 vOnwerPos = pOwner->GetWorldPosition();
		//Ÿ�� ��ġ ���
		D3DXVECTOR3 vTargetPos = pTarget->GetWorldPosition();
			
		vNewPos = vOnwerPos;
		vNewPos += ( m_vTargetDir * fDist );

		float fMapHeight = pOwner->GetCreator()->GetMapHeightNoneEntity( vNewPos.x, vNewPos.z );
		if ( fMapHeight > vNewPos.y )
		{
			//�ش� ���̰� ���� ���� ���� ������ ����
			vNewPos.y = fMapHeight;
		}

		pOwner->SetWorldPosition( vNewPos );

		SetRotateToTarget( pOwner );
	}

}

/////////////////////////////////////////////////////////////////////////////////
//Ű�Է�
void ioPsychoMetry_Item::CheckInputKey( ioBaseChar *pOwner )
{
	if ( !pOwner || !pOwner->IsNeedProcess() || !m_bSetTarget )
		return;

	if ( IsEnableInputKey( pOwner ) )
	{
		if ( m_dwComboReserveTime < FRAMEGETTIME() )
		{
			if ( pOwner->IsAttackKey() )
			{
				SetRotateToTarget( pOwner );
				SetEnergyFireState( pOwner );		
			}
			else if ( pOwner->IsJumpKey() )
			{			
				m_dwWeaponBaseIndex = pOwner->IncreaseWeaponIndexBase();

				if ( CheckDropZone( pOwner ) )
					SetDropZoneTeleportMove( pOwner );
				else
					SetTeleportMove( pOwner );

				SetTeleportAttackState( pOwner );
			}
		}
	}
	else
	{
		m_PsychoSpecialState = PSS_End;

		if( pOwner->IsNeedSendNetwork() )
		{
			SP2Packet kPacket( CUPK_ITEM_GAUGE_OK );
			FillItemGaugePacketHeader( pOwner, kPacket );
			kPacket << WS_SPECIAL_STATE_PROCESS;
			kPacket << PSS_End;
			P2PNetwork::SendToAllPlayingUser( kPacket );
		}	
	}
}

//Ÿ������ ȸ��
void ioPsychoMetry_Item::SetRotateToTarget( ioBaseChar *pOwner )
{
	if ( !pOwner || m_szTargetName.IsEmpty() )
		return;

	//ȸ�� Ÿ�� ���
	ioBaseChar *pTarget = pOwner->GetBaseChar( m_szTargetName );
	if ( pTarget )
	{
		//���� ��ġ ���
		D3DXVECTOR3 vOnwerPos = pOwner->GetWorldPosition();
		//Ÿ�� ��ġ ���
		D3DXVECTOR3 vTargetPos = pTarget->GetWorldPosition();
		//���� ����		
		vOnwerPos.y = vTargetPos.y;

		//Ÿ�� ���� ����
		m_vTargetDir = vTargetPos - vOnwerPos;

		m_vTargetDist = m_vTargetDir;

		//ĳ���� ȸ�� �� ���� ����
		D3DXVec3Normalize( &m_vTargetDir, &m_vTargetDir );		
		pOwner->SetTargetRotToDir( m_vTargetDir, true );
	}
}

//Ű�Է��� �����Ѱ�
bool ioPsychoMetry_Item::IsEnableInputKey( ioBaseChar *pOwner )
{
	if ( !pOwner || m_szTargetName.IsEmpty() || !m_bSetTarget )
		return false;

	ioBaseChar *pTarget = pOwner->GetBaseChar( m_szTargetName );
	if ( pTarget )
	{
		return !pTarget->HasHideBuff();
	}
	
	return false;
}

//�ڷ���Ʈ �����Ѱ�
bool ioPsychoMetry_Item::CheckDropZone( ioBaseChar *pOwner )
{
	if ( !pOwner || m_szTargetName.IsEmpty() )
		return false;

	ioBaseChar *pTarget = pOwner->GetBaseChar( m_szTargetName );
	if ( !pTarget )
		return false;	

	ioPlayStage *pStage = pOwner->GetCreator();
	if ( !pStage )
		return false;

	D3DXVECTOR3 vTargetPos = pTarget->GetWorldPosition();
	float fMapHeight = pStage->GetMapHeightOnlyTerrain( vTargetPos.x, vTargetPos.z );
	if ( fMapHeight <= 0.0f )
	{
		return true;
	}

	return false;
}

//���ִ� ���� �ΰ�
bool ioPsychoMetry_Item::IsFloatState( ioBaseChar *pOwner )
{	
	if ( !pOwner )
		return false;

	ioPlayStage *pStage = pOwner->GetCreator();
	if ( !pStage )
		return false;

	D3DXVECTOR3 vPos = pOwner->GetWorldPosition();
	float fMapHeight = pStage->GetMapHeightOnlyTerrain( vPos.x, vPos.z );

	if ( fabs( fMapHeight - vPos.y ) >= m_fJumpHeightGap )
	{
		return true;
	}

	return false;
}

//�������
void ioPsychoMetry_Item::ReserveInputKey(ioBaseChar *pOwner )
{
	if ( !pOwner || !pOwner->IsNeedProcess() )
		return;

	if( pOwner->IsAttackKey() )
		m_bAttackKeyReserved = true;

	if( pOwner->IsDefenseKey() )
		m_bDefenseKeyReserved = true;

	if( pOwner->IsJumpKey() )
		m_bJumpKeyReserved = true;
}

void ioPsychoMetry_Item::CheckReserveState( ioBaseChar *pOwner )
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

	m_PsychoSpecialState = PSS_End;

	if( pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_ITEM_GAUGE_OK );
		FillItemGaugePacketHeader( pOwner, kPacket );
		kPacket << WS_SPECIAL_STATE_PROCESS;
		kPacket << PSS_End;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}	
}

void ioPsychoMetry_Item::SetReserveAttack( ioBaseChar *pOwner )
{
	if ( !pOwner || !pOwner->IsNeedProcess() )
		return;

	if ( !IsFloatState(pOwner) )
	{
		pOwner->SetNormalAttack(0);
	}
	m_bAttackKeyReserved = false;
}

void ioPsychoMetry_Item::SetReserveDefence( ioBaseChar *pOwner )
{
	if ( !pOwner || !pOwner->IsNeedProcess() )
		return;

	pOwner->SetDefenseState();
	m_bDefenseKeyReserved = false;
}

void ioPsychoMetry_Item::SetReserveJump( ioBaseChar *pOwner )
{
	if ( !pOwner || !pOwner->IsNeedProcess() )
		return;

	pOwner->SetJumpState();
	m_bJumpKeyReserved = false;	
}

void ioPsychoMetry_Item::SetNextItemState( ioBaseChar *pWoundChar, ioBaseChar *pAttacker, ioWeapon *pWeapon, bool bDefense, float fDamage )
{
	if( pWoundChar->IsNeedProcess() && fDamage > 0.0f )
	{
		if ( pWoundChar && pAttacker )
		{
			m_szTargetName = pWoundChar->GetCharName();
			m_bSetTarget = true;

			if( pWoundChar->IsNeedSendNetwork() )
			{
				SP2Packet kPacket( CUPK_ITEM_GAUGE_OK );
				FillItemGaugePacketHeader( pAttacker, kPacket );
				kPacket << WS_SPECIAL_STATE_PROCESS;
				kPacket << PSS_Set_Target;
				kPacket << m_szTargetName;
				kPacket << m_bSetTarget;
				P2PNetwork::SendToAllPlayingUser( kPacket );
			}
		}
	}
	
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

ioPsychoMetrySpecialState::ioPsychoMetrySpecialState()
{
}

ioPsychoMetrySpecialState::~ioPsychoMetrySpecialState()
{
}

void ioPsychoMetrySpecialState::Initialize( ioBaseChar* pOwner, CharState eCurrState )
{
	ioSpecialStateBase::Initialize( pOwner, eCurrState );
}

void ioPsychoMetrySpecialState::Finalize( ioBaseChar* pOwner, CharState eCurrState, CharState eNewState )
{
	ioSpecialStateBase::Finalize( pOwner, eCurrState, eNewState );

	ioPsychoMetry_Item *pPsychoItem = ToPsychoMetryItem( pOwner->GetPriorityItem( SPT_ATTACK ) );
	if( pPsychoItem )
		pPsychoItem->ClearState( pOwner );
}

void ioPsychoMetrySpecialState::CheckSpecialState( ioBaseChar* pOwner )
{
	ioPsychoMetry_Item *pPsychoItem = ToPsychoMetryItem( pOwner->GetPriorityItem( SPT_ATTACK ) );
	if( pPsychoItem )
	{
		pPsychoItem->ProcessPsychoSpecialState( pOwner );
	}
	else
	{
		pOwner->SetState( CS_DELAY );
		pOwner->SetFallState( true );
		return;
	}
}

void ioPsychoMetrySpecialState::CheckFallStateChange( ioBaseChar* pOwner, float fMinFallHeightRate, float fBottomHeight, bool bBottomMaxCheck, float fMaxBottomHeight )
{
	return;
}

void ioPsychoMetrySpecialState::GetGravity( ioBaseChar* pOwner, float &fCurGravity )
{
	ioPsychoMetry_Item *pPsyChoItem = ToPsychoMetryItem( pOwner->GetPriorityItem( SPT_ATTACK ) );
	if( pPsyChoItem )
		fCurGravity = 0.0f;
}