#include "StdAfx.h"
#include "ioAccGetUpProtectionDelayBuff.h"
#include "ioBaseChar.h"
#include "ioPlayStage.h"


ioAccGetUpProtectionDelayBuff::ioAccGetUpProtectionDelayBuff(void)
{
	m_dwGetUpDelayDuration = 0;
}

ioAccGetUpProtectionDelayBuff::ioAccGetUpProtectionDelayBuff( const ioAccGetUpProtectionDelayBuff &rhs ):
ioBuff( rhs ),
m_bDisableDownState( rhs.m_bDisableDownState ),
m_vEnableModeTypeList( rhs.m_vEnableModeTypeList )
{
	m_dwGetUpDelayDuration = 0;
}


ioAccGetUpProtectionDelayBuff::~ioAccGetUpProtectionDelayBuff(void)
{
}

ioBuff* ioAccGetUpProtectionDelayBuff::Clone()
{
	return new ioAccGetUpProtectionDelayBuff( *this );
}

void ioAccGetUpProtectionDelayBuff::LoadProperty( ioINILoader &rkLoader )
{
	ioBuff::LoadProperty( rkLoader );
	m_bDisableDownState  = rkLoader.LoadBool_e( "disable_down_state", false );

	char szBuff[MAX_PATH];
	int iCnt = rkLoader.LoadInt_e( "max_enable_mode_count", 0 );
	for ( int i=0 ; i<iCnt ; ++i )
	{
		wsprintf_e( szBuff, "enable_mode%d", i+1 );
		int iMode = rkLoader.LoadInt( szBuff, 0 );
		if ( iMode == 0 )
			continue;
		m_vEnableModeTypeList.push_back( iMode );
	}
}

void ioAccGetUpProtectionDelayBuff::StartBuff( ioBaseChar *pOwner )
{
	ioBuff::StartBuff( pOwner );
	ioItem *pItem= GetOwnerItem();
	if ( !pItem )
	{
		SetReserveEndBuff();
		return;
	}

	float fNum = pItem->GetAccessoryRandomNum() / 1000.0f;
	if ( fNum <= 0.0f )
	{
		SetReserveEndBuff();
		return;
	}

	m_dwGetUpDelayDuration = (DWORD)(fNum * 1000.0f);
	//m_fAddCriticalRate = fNum;
	//pOwner->AddCriticalRate( m_fAddCriticalRate );
}

bool ioAccGetUpProtectionDelayBuff::ApplyStartBuff( ioBaseChar *pOwner, DWORD dwGlobalTime, DWORD dwGapTime )
{
	if( !ioBuff::ApplyStartBuff( pOwner, dwGlobalTime, dwGapTime ) )
		return false;

	ioItem *pItem= GetOwnerItem();
	if ( !pItem )
	{
		SetReserveEndBuff();
		return false;
	}

	float fNum = pItem->GetAccessoryRandomNum() / 1000.0f;
	if ( fNum <= 0.0f )
	{
		SetReserveEndBuff();
		return false;
	}

	m_dwGetUpDelayDuration = (DWORD)(fNum * 1000.0f);
	//pOwner->AddCriticalRate( m_fAddCriticalRate );
	return true;
}

void ioAccGetUpProtectionDelayBuff::ProcessBuff( float fTimePerSec )
{
	CheckOwnerStateCheck();

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
}

void ioAccGetUpProtectionDelayBuff::CheckOwnerStateCheck()
{
	if( !m_pOwner ) return;

	CharState eState = m_pOwner->GetState();

	if( m_bDisableDownState )
	{
		if( eState == CS_FROZEN && m_pOwner->GetBlowFrozenState() )
			SetReserveEndBuff();
		else if( eState == CS_BLOW_WOUNDED && !m_pOwner->IsFloatingState() )
			SetReserveEndBuff();
		else if( eState == CS_BOUND_BLOW_WOUNDED && !m_pOwner->IsFloatingState() )
			SetReserveEndBuff();
		else if( eState == CS_ETC_STATE && m_pOwner->IsApplyDownState(false) )
			SetReserveEndBuff();
		else if( eState == CS_STOP_MOTION && m_pOwner->GetBlowStopMotionState() )
			SetReserveEndBuff();
		else if( eState == CS_DROP_ZONE_DOWN )
			SetReserveEndBuff();
		else if( eState == CS_ICE_STATE && m_pOwner->GetBlowStopMotionState() )
			SetReserveEndBuff();
	}
}

void ioAccGetUpProtectionDelayBuff::EndBuff()
{
	ioBuff::EndBuff();
}

void ioAccGetUpProtectionDelayBuff::CheckGetUpDelayDuration( DWORD& dwDuration, int iMode )
{
	if ( m_dwGetUpDelayDuration == 0 )
		return;

	if ( m_vEnableModeTypeList.empty() )
		return;

	int iSize = m_vEnableModeTypeList.size();
	bool bIsMode = false;
	for( int i=0; i<iSize; ++i )
	{
		if ( m_vEnableModeTypeList[i] == iMode )
		{
			bIsMode = true;
			break;
		}
	}

	if ( bIsMode )
		dwDuration += m_dwGetUpDelayDuration;
}
