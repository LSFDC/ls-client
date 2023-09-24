
#include "stdafx.h"

#include "ioHide4Buff.h"

#include "ioBaseChar.h"
#include "ioPlayStage.h"

ioHide4Buff::ioHide4Buff()
{
}

ioHide4Buff::ioHide4Buff( const ioHide4Buff &rhs )
	: ioBuff( rhs ),
	  m_fAddSpeed( rhs.m_fAddSpeed ),
	  m_bDisableDownState( rhs.m_bDisableDownState ),
	  m_nAlphaRateSpeed( rhs.m_nAlphaRateSpeed ),
	  m_dwAlphaDelayTime( rhs.m_dwAlphaDelayTime ),
	  m_dwAlphaChangeTime( rhs.m_dwAlphaChangeTime ),
	  m_fFixedAlphaRate( rhs.m_fFixedAlphaRate ),
	  m_bReverseTestTeam( rhs.m_bReverseTestTeam )
{
}

ioHide4Buff::~ioHide4Buff()
{
}

void ioHide4Buff::LoadProperty( ioINILoader &rkLoader )
{
	ioBuff::LoadProperty( rkLoader );

	m_fAddSpeed = rkLoader.LoadFloat_e( "speed_add_value", 0.0f );
	m_bDisableDownState = rkLoader.LoadBool_e( "disable_down_state", false );
	m_nAlphaRateSpeed = rkLoader.LoadInt_e( "alpha_rate_speed", 0 );
	m_dwAlphaDelayTime = rkLoader.LoadInt_e( "alpha_delay_time", 0 );
	m_dwAlphaChangeTime = rkLoader.LoadInt_e( "alpha_change_time", 0 );
	m_fFixedAlphaRate = rkLoader.LoadFloat_e( "fixed_alpha_rate", 0.0f );
	m_bReverseTestTeam = rkLoader.LoadBool_e( "set_reverse", false );
}

ioBuff* ioHide4Buff::Clone()
{
	return new ioHide4Buff( *this );
}

void ioHide4Buff::StartBuff( ioBaseChar *pOwner )
{
	ioBuff::StartBuff( pOwner );
	
	Init();
}

bool ioHide4Buff::ApplyStartBuff( ioBaseChar *pOwner, DWORD dwGlobalTime, DWORD dwGapTime )
{
	if( !ioBuff::ApplyStartBuff( pOwner, dwGlobalTime, dwGapTime ) )
		return false;

	Init();

	return true;
}

void ioHide4Buff::ProcessBuff( float fTimePerSec )
{	
	if( CheckEnableDelayState() )
		return;

	if ( CheckOwnerStateCheck() )
	{
		SetReserveEndBuff();
		return;
	}

	switch( m_iOperationType )
	{
	case OT_SWITCH:
		if( !CheckSwitchType( fTimePerSec ) )
		{
			SetReserveEndBuff();
			return;
		}

		CheckCallingProcess();
		break;
	case OT_DELAY:
	case OT_ACTIVE:
		ioBuff::ProcessBuff( fTimePerSec );
		break;
	case OT_PASSIVE:
		CheckCallingAreaWeapon();
		break;
	}

	switch ( m_BuffState )
	{
	case BS_NONE:
		SetFuncState( m_pOwner );
		break;
	case BS_LOOP:
		break;
	case BS_ENEMY_LOOP:
		{
			ProcessAlphaRate( m_pOwner );

			if ( m_pOwner )
				m_pOwner->SetBottomCircleVisible( false );
		}
		break;
	}
}

void ioHide4Buff::EndBuff()
{
	ProcessEnd( m_pOwner );
	ioBuff::EndBuff();
}

bool ioHide4Buff::CheckOwnerStateCheck()
{
	if( !m_pOwner ) 
		return true;

	CharState eState = m_pOwner->GetState();

	bool bDown = false;
	if( m_bDisableDownState )
	{	
		if( eState == CS_FROZEN && m_pOwner->GetBlowFrozenState() )
			bDown = true;
		else if( eState == CS_BLOW_WOUNDED && !m_pOwner->IsFloatingState() )
			bDown = true;
		else if( eState == CS_BOUND_BLOW_WOUNDED && !m_pOwner->IsFloatingState() )
			bDown = true;
		else if( eState == CS_ETC_STATE && m_pOwner->IsApplyDownState(false) )
			bDown = true;
		else if( eState == CS_STOP_MOTION && m_pOwner->GetBlowStopMotionState() )
			bDown = true;
		else if( eState == CS_DROP_ZONE_DOWN )
			bDown = true;
		else if( eState == CS_ICE_STATE && m_pOwner->GetBlowStopMotionState() )
			bDown = true;
	}

	return bDown;
}

void ioHide4Buff::Init()
{
	m_dwCheckAlphaDelayTime = 0;
	m_dwCheckAlphaChangeTime = 0;
	m_bSetAlphaRateDel = true;
	m_nCurAlphaRateSpeed = -m_nAlphaRateSpeed;
	m_BuffState = BS_NONE;
}

void ioHide4Buff::SetFuncState( ioBaseChar *pOwner )
{
	if ( !pOwner )
		return;
	
	ioPlayStage *pStage = pOwner->GetCreator();
	if ( !pStage )
		return;

	ioBaseChar *pPlayer = pStage->GetOwnerChar();
	if ( !pPlayer )
		return;

	//아군이면 
	if ( IsTeamTest( pOwner->GetTeam(), pPlayer->GetTeam() ) )
	{
		pOwner->SetDisableGauge( false );
		pOwner->SetBottomCircleVisible( true );
		pOwner->SetAlphaRateDirect( MAX_ALPHA_RATE * m_fFixedAlphaRate );
		pOwner->SetAttachEffectAlphaRateDirect( MAX_ALPHA_RATE * m_fFixedAlphaRate, true );
		m_BuffState = BS_LOOP;
	}
	else
	{
		pOwner->SetDisableGauge( true );
		pOwner->SetBottomCircleVisible( false );
		pOwner->SetAlphaRateDirect( MAX_ALPHA_RATE );
		pOwner->SetAttachEffectAlphaRateDirect( MAX_ALPHA_RATE, true );
		m_BuffState = BS_ENEMY_LOOP;
	}

	pOwner->SetExtraMoveSpeed( m_fAddSpeed );
}

void ioHide4Buff::ProcessAlphaRate( ioBaseChar *pOwner )
{
	if ( !pOwner )
		return;

	DWORD dwCurTime = FRAMEGETTIME();
	if ( m_dwAlphaDelayTime + m_dwCheckAlphaDelayTime > dwCurTime )
		return;
	
	int nCurAlphaRate = pOwner->GetAlphaRate();

	if ( m_bSetAlphaRateDel && nCurAlphaRate <= 0 )
	{
		m_bSetAlphaRateDel = false;
		pOwner->SetAlphaRateDirect( 0 );
		pOwner->SetAttachEffectAlphaRateDirect( 0, true );
		m_dwCheckAlphaDelayTime = dwCurTime;
		m_nCurAlphaRateSpeed = m_nAlphaRateSpeed;
		return;
	}
	else if ( !m_bSetAlphaRateDel && nCurAlphaRate >= MAX_ALPHA_RATE )
	{
		m_bSetAlphaRateDel = true;
		pOwner->SetAlphaRateDirect( MAX_ALPHA_RATE );
		pOwner->SetAttachEffectAlphaRateDirect( MAX_ALPHA_RATE, true );
		m_dwCheckAlphaDelayTime = dwCurTime;
		m_nCurAlphaRateSpeed = -m_nAlphaRateSpeed;
		return;
	}	

	if ( m_dwAlphaChangeTime + m_dwCheckAlphaChangeTime > dwCurTime )
		return;

	pOwner->SetAlphaRateDirect( nCurAlphaRate + m_nCurAlphaRateSpeed );
	pOwner->SetAttachEffectAlphaRateDirect( nCurAlphaRate + m_nCurAlphaRateSpeed, true );
	m_dwCheckAlphaChangeTime = dwCurTime;
}

void ioHide4Buff::ProcessEnd( ioBaseChar *pOwner )
{
	if ( !pOwner )
		return;

	pOwner->SetDisableGauge( false );
	pOwner->SetBottomCircleVisible( true );
	pOwner->SetAlphaRateDirect( MAX_ALPHA_RATE );
	pOwner->SetAttachEffectAlphaRateDirect( MAX_ALPHA_RATE, true );
	pOwner->RemoveExtraMoveSpeed( m_fAddSpeed );
}

bool ioHide4Buff::IsTeamTest( TeamType eOwnerType, TeamType ePlayerType )
{
	if ( !m_bReverseTestTeam )
	{
		if ( eOwnerType == ePlayerType )
			return true;
	}
	else
	{
		if ( eOwnerType != ePlayerType )
			return true;
	}

	return false;
}