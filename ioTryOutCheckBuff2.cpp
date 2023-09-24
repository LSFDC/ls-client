

#include "stdafx.h"

#include "ioBaseChar.h"

#include "ioTryOutCheckBuff2.h"
#include "BuffDefine.h"


ioTryOutCheckBuff2::ioTryOutCheckBuff2()
{
	m_iCurHitCount = 0;
}

ioTryOutCheckBuff2::ioTryOutCheckBuff2( const ioTryOutCheckBuff2 &rhs )
	: ioBuff( rhs ),
	m_bSetFloatState( rhs.m_bSetFloatState ),
	m_FloatBuffInfo( rhs.m_FloatBuffInfo ),
	m_HoldAnimation( rhs.m_HoldAnimation ),
	m_HoldAirAnimation( rhs.m_HoldAirAnimation ),
	m_iMaxHitCount( rhs.m_iMaxHitCount ),
	m_bEnableTeamAttack( rhs.m_bEnableTeamAttack ),
	m_dwKeyInputStartTime( rhs.m_dwKeyInputStartTime ),
	m_dwKeyInputEndTime( rhs.m_dwKeyInputEndTime ),
	m_nMaxClickCnt( rhs.m_nMaxClickCnt ),
	m_nRandomMax( rhs.m_nRandomMax ),
	m_nRandomMin( rhs.m_nRandomMin )

{
	m_iCurHitCount = 0;
	m_bKeyInputed = false;
	m_bLeftKeyTime = true;
	m_nClickCnt = 0;
	m_bSafe = false;
	m_bSafeCheck = true;
}

ioTryOutCheckBuff2::~ioTryOutCheckBuff2()
{
}

void ioTryOutCheckBuff2::LoadProperty( ioINILoader &rkLoader )
{
	ioBuff::LoadProperty( rkLoader );
	char szBuf[MAX_PATH] = "";

	m_iMaxHitCount = rkLoader.LoadInt_e( "hit_count", 0 );
	m_bEnableTeamAttack = rkLoader.LoadBool_e( "enable_team_attack", false );

	// FloatInfo
	m_bSetFloatState = rkLoader.LoadBool_e( "set_float_state", false );
	m_FloatBuffInfo.m_bStopAction = rkLoader.LoadBool_e( "float_action_stop", false );

	m_FloatBuffInfo.m_fFloatJumpPower = rkLoader.LoadFloat_e( "float_jump_power", 0.0f );
	m_FloatBuffInfo.m_fFloatGravityAmt = rkLoader.LoadFloat_e( "float_gravity_amt", 0.0f );
	m_FloatBuffInfo.m_fFloatFrictionAmt = rkLoader.LoadFloat_e( "float_friction_amt", 0.0f );
	m_FloatBuffInfo.m_fFloatBlowRate = rkLoader.LoadFloat_e( "float_blow_rate", 1.0f );
	m_FloatBuffInfo.m_fFloatSpeedRate = rkLoader.LoadFloat_e( "float_speed_rate", 1.0f );
	m_FloatBuffInfo.m_fFloatEndJumpPower = rkLoader.LoadFloat_e( "float_end_jump_power", 0.1f );

	m_FloatBuffInfo.m_bEndJumpState = rkLoader.LoadBool_e( "enable_end_jump_state", false );

	// ���� Ư���� �̺κ��� �ϵ� �ڵ�����..
	m_FloatBuffInfo.m_bEnableDamage = true;
	m_FloatBuffInfo.m_bEnableTeamAttack = m_bEnableTeamAttack;

	m_FloatBuffInfo.m_iCancelType = rkLoader.LoadInt_e( "cancel_type", 0 );
	m_FloatBuffInfo.m_iMoveType = rkLoader.LoadInt_e( "move_type", 0 );
	m_FloatBuffInfo.m_iLandType = rkLoader.LoadInt_e( "land_type", 0 );

	m_FloatBuffInfo.m_dwFloatMinTime = rkLoader.LoadInt_e( "float_min_time", 0 );
	m_FloatBuffInfo.m_dwFloatMaxTime = rkLoader.LoadInt_e( "float_max_time", 0 );

	rkLoader.LoadString_e( "hold_animation", "", szBuf, MAX_PATH );
	m_HoldAnimation = szBuf;
	rkLoader.LoadString_e( "hold_air_animation", "", szBuf, MAX_PATH );
	m_HoldAirAnimation = szBuf;

	// �ƴ���
	m_dwKeyInputStartTime = (DWORD)rkLoader.LoadInt_e( "key_input_start_time", 0 );
	m_dwKeyInputEndTime = (DWORD)rkLoader.LoadInt_e( "key_input_end_time", 0 );
	m_nMaxClickCnt = rkLoader.LoadInt_e( "max_click_cnt", 0 );

	m_nRandomMax = (DWORD)rkLoader.LoadInt( "max_random_size", 100 );
	m_nRandomMin = (DWORD)rkLoader.LoadInt( "min_random_size", 0 );
}

ioBuff* ioTryOutCheckBuff2::Clone()
{
	return new ioTryOutCheckBuff2( *this );
}

void ioTryOutCheckBuff2::StartBuff( ioBaseChar *pOwner )
{
	ioBuff::StartBuff( pOwner );

	if( !pOwner )
		return;

	m_iCurHitCount = 0;

	m_bSafeCheck = true;

	/*ioEntityGroup *pGrp = pOwner->GetGroup();
	if( m_bSetFloatState )
	{
	if( !m_HoldAirAnimation.IsEmpty() )
	{
	pGrp->ClearAllLoopAni( 0.0f, true );
	pGrp->ClearAllActionAni( 0.0f, true );
	pGrp->SetLoopAni( m_HoldAirAnimation, 100.0f );
	}

	ioAffectWoundedValueBuff *pBuff = ToAffectWoundedValue( pOwner->GetAffectWoundedValueBuff() );
	if( pBuff )
	{
	if( pBuff->CheckTeamAttack() )
	{
	if( GetOwner() && GetCreator() && GetOwner()->GetTeam() != GetCreator()->GetTeam() )
	{
	m_FloatBuffInfo.m_fFloatJumpPower *= pBuff->GetFloatJumpPowerRate();
	m_FloatBuffInfo.m_fFloatGravityAmt *= pBuff->GetFloatGravityRate();
	m_FloatBuffInfo.m_fFloatFrictionAmt *= pBuff->GetFloatFrictionRate();
	}
	}
	else
	{
	m_FloatBuffInfo.m_fFloatJumpPower *= pBuff->GetFloatJumpPowerRate();
	m_FloatBuffInfo.m_fFloatGravityAmt *= pBuff->GetFloatGravityRate();
	m_FloatBuffInfo.m_fFloatFrictionAmt *= pBuff->GetFloatFrictionRate();
	}
	}

	pOwner->SetFloatStateValue( m_FloatBuffInfo );
	pOwner->SetState( CS_FLOAT_STATE );
	}
	else
	{
	if( !m_bPassHoldState )
	{
	if( !m_HoldAnimation.IsEmpty() )
	{
	pGrp->ClearAllLoopAni( 0.0f, true );
	pGrp->ClearAllActionAni( 0.0f, true );
	pGrp->SetLoopAni( m_HoldAnimation, 100.0f );
	}
	pOwner->SetState( CS_ETC_STATE );
	}
	}*/
}

bool ioTryOutCheckBuff2::ApplyStartBuff(ioBaseChar *pOwner, DWORD dwGlobalTime, DWORD dwGapTime )
{
	if( !ioBuff::ApplyStartBuff( pOwner, dwGlobalTime, dwGapTime ) )
		return false;

	if( !pOwner )
		return false;

	ioEntityGroup *pGrp = pOwner->GetGroup();
	if( m_bSetFloatState )
	{
		if( !m_HoldAirAnimation.IsEmpty() )
		{
			pGrp->ClearAllLoopAni( 0.0f, true );
			pGrp->ClearAllActionAni( 0.0f, true );
			pGrp->SetLoopAni( m_HoldAirAnimation, 100.0f );
		}

		ioAffectWoundedValueBuff *pBuff = ToAffectWoundedValue( pOwner->GetAffectWoundedValueBuff() );
		if( pBuff )
		{
			if( pBuff->CheckTeamAttack() )
			{
				if( GetOwner() && GetCreator() && GetOwner()->GetTeam() != GetCreator()->GetTeam() )
				{
					m_FloatBuffInfo.m_fFloatJumpPower *= pBuff->GetFloatJumpPowerRate();
					m_FloatBuffInfo.m_fFloatGravityAmt *= pBuff->GetFloatGravityRate();
					m_FloatBuffInfo.m_fFloatFrictionAmt *= pBuff->GetFloatFrictionRate();
				}
			}
			else
			{
				m_FloatBuffInfo.m_fFloatJumpPower *= pBuff->GetFloatJumpPowerRate();
				m_FloatBuffInfo.m_fFloatGravityAmt *= pBuff->GetFloatGravityRate();
				m_FloatBuffInfo.m_fFloatFrictionAmt *= pBuff->GetFloatFrictionRate();
			}
		}

		pOwner->SetFloatStateValue( m_FloatBuffInfo );
		pOwner->SetState( CS_FLOAT_STATE );
	}
	else
	{
		if( !m_HoldAnimation.IsEmpty() )
		{
			pGrp->ClearAllLoopAni( 0.0f, true );
			pGrp->ClearAllActionAni( 0.0f, true );
			pGrp->SetLoopAni( m_HoldAnimation, 100.0f );
		}
		pOwner->SetState( CS_ETC_STATE );
	}

	return true;
}


bool ioTryOutCheckBuff2::IsShowCheckKey() const
{
	return m_bKeyInputed;
}

void ioTryOutCheckBuff2::ProcessBuff( float fTimePerSec )
{
	DWORD dwCurTime = FRAMEGETTIME();

	/*if( m_iMaxHitCount > 0 && m_iMaxHitCount - m_iCurHitCount <= 0 )
	{
	SetReserveEndBuff();
	return;
	}*/

	if( m_bSafe )
	{
		SetReserveEndBuff();
		return;
	}

	if( m_pOwner && m_pOwner->IsNeedProcess() && !m_bSafe )
	{
		if( !m_bKeyInputed && 
			(m_dwBuffStartTime + m_dwKeyInputStartTime) < dwCurTime )
		{
			m_bKeyInputed = true;
		}

		if( m_bKeyInputed && (m_dwBuffStartTime + m_dwKeyInputEndTime) > dwCurTime )
		{
			if( m_bLeftKeyTime )
			{
				ioUserKeyInput::DirKeyInput eNewDirKey = m_pOwner->CheckCurDirKey();
				if( eNewDirKey == ioUserKeyInput::DKI_LEFT )
				{
					m_nClickCnt++;
					m_bLeftKeyTime = false;
				}
			}
			else
			{
				ioUserKeyInput::DirKeyInput eNewDirKey = m_pOwner->CheckCurDirKey();
				if( eNewDirKey == ioUserKeyInput::DKI_RIGHT )
				{
					m_nClickCnt++;
					m_bLeftKeyTime = true;
				}
			}
		}

		if( m_nClickCnt >= m_nMaxClickCnt && m_bSafeCheck )
		{
			m_bSafeCheck = false;
			IORandom random;
			DWORD dwSeed = timeGetTime();
			int iRand = 0;
			random.SetRandomSeed( dwSeed );

			int nRand = random.Random( m_nRandomMax );
			if( nRand <= m_nRandomMin )
			{
				m_bSafe = true;
				SendSafeInfo();
				return;
			}
		}
	}

	CheckOwnerState();

	switch( m_iOperationType )
	{
	case OT_SWITCH:
	case OT_DELAY:
	case OT_ACTIVE:
		ioBuff::ProcessBuff( fTimePerSec );
		break;
	case OT_PASSIVE:
		CheckCallingAreaWeapon();
		break;
	}
}

void ioTryOutCheckBuff2::CheckOwnerState()
{
	ioBaseChar *pOwner = GetOwner();
	if( !pOwner )
		return;

	if( pOwner->GetState() == CS_DROP_ZONE_DOWN )
		return;

	//if( m_bPassHoldState )
	{
		ioBaseChar *pCreator =  GetCreator();
		if( !pCreator )
		{
			SetReserveEndBuff();
			return;
		}

		if( pCreator->GetState() != CS_GRAPPLING_EXPANSION )
		{
			SetReserveEndBuff();
			return;
		}

		if( pOwner->GetState() != CS_GRAPPLING_WOUNDED )
			SetReserveEndBuff();
		
		return;
	}

	/*if( pOwner->GetState() != CS_DIE &&
		pOwner->GetState() != CS_LOADING &&
		pOwner->GetState() != CS_OBSERVER &&
		pOwner->GetState() != CS_VIEW )
	{
		CharState eState = pOwner->GetState();
		if( m_bSetFloatState && eState != CS_FLOAT_STATE )
		{
			ioEntityGroup *pGrp = pOwner->GetGroup();
			if( !m_HoldAirAnimation.IsEmpty() )
			{
				pGrp->ClearAllLoopAni( 0.0f, true );
				pGrp->ClearAllActionAni( 0.0f, true );
				pGrp->SetLoopAni( m_HoldAirAnimation, 100.0f );
			}
			pOwner->SetState( CS_FLOAT_STATE );
		}
		else if( !m_bSetFloatState && eState != CS_ETC_STATE )
		{
			ioEntityGroup *pGrp = pOwner->GetGroup();
			if( !m_HoldAnimation.IsEmpty() )
			{
				pGrp->ClearAllLoopAni( 0.0f, true );
				pGrp->ClearAllActionAni( 0.0f, true );
				pGrp->SetLoopAni( m_HoldAnimation, 100.0f );
			}
			pOwner->SetState( CS_ETC_STATE );
		}
	}*/
}

void ioTryOutCheckBuff2::EndBuff()
{
	ioBaseChar *pOwner = GetOwner();
	if( !pOwner )
	{
		ioBuff::EndBuff();
		return;
	}

	if( pOwner->GetState() == CS_FLOAT_STATE )
	{
		if( m_FloatBuffInfo.m_bEndJumpState )
		{
			// HARDCODE : ����ȭ ���ؼ� 0.1f �ο�
			pOwner->SetSKillEndJumpState( m_FloatBuffInfo.m_fFloatEndJumpPower, false, true );
		}
		else
		{
			BlowWoundAniInfo kInfo;
			pOwner->SetBlowWoundedState( true, kInfo );
		}
	}
	else if( pOwner->GetState() == CS_ETC_STATE )
	{
		pOwner->SetState( CS_DELAY, m_bSendDelayNetWork );
		pOwner->SetDashFailStart();
	}

	if( m_bSafe )
		ioBuff::m_bPassEndBuffCall = true;

	ioBuff::EndBuff();
}

void ioTryOutCheckBuff2::GetBuffInfo( SP2Packet &rkPacket )
{
	ioBuff::GetBuffInfo( rkPacket );

	rkPacket << m_iCurHitCount;
}

void ioTryOutCheckBuff2::ApplyBuffInfo( SP2Packet &rkPacket )
{
	rkPacket >> m_iCurHitCount;
}

void ioTryOutCheckBuff2::IncreaseHitCount()
{
	m_iCurHitCount++;
	m_iCurHitCount = min( m_iCurHitCount, m_iMaxHitCount );
}


void ioTryOutCheckBuff2::ApplyExtraBuffInfo(  SP2Packet &rkPacket  )
{
	ioBaseChar *pOwner = GetOwner();
	if( !pOwner )
		return;

	ioPlayStage *pStage = pOwner->GetCreator();
	if( !pStage )
		return;

	bool bSafe;
	rkPacket >> bSafe;

	m_bSafe = bSafe;
}


void ioTryOutCheckBuff2::SendSafeInfo()
{
	SP2Packet kPacket( CUPK_APPLY_EXTRA_BUFF_INFO );
	kPacket << m_pOwner->GetCharName();
	kPacket << GetName();
	kPacket << m_bSafe;

	P2PNetwork::SendToAllPlayingUser( kPacket );
}


void ioTryOutCheckBuff2::ResetCount()
{
	m_nClickCnt = 0;
	m_bSafe = false;

	m_bSafeCheck = true;
}