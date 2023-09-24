#include "StdAfx.h"
#include "ioAccLandingRateBuff.h"
#include "ioBaseChar.h"

ioAccLandingRateBuff::ioAccLandingRateBuff(void)
{
	m_fLandingRate = 0.0f;
	m_fExtendLandingRate = 0.0f;
}

ioAccLandingRateBuff::ioAccLandingRateBuff( const ioAccLandingRateBuff& rhs ):
ioBuff( rhs )
{
	m_fLandingRate = 0.0f;
	m_fExtendLandingRate = 0.0f;
}


ioAccLandingRateBuff::~ioAccLandingRateBuff(void)
{
}

ioBuff* ioAccLandingRateBuff::Clone()
{
	return new ioAccLandingRateBuff( *this );
}

void ioAccLandingRateBuff::StartBuff( ioBaseChar *pOwner )
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
	m_fLandingRate = m_fExtendLandingRate = fNum;
	pOwner->SetLandingRateByBuff( m_fLandingRate, m_fExtendLandingRate );
}

bool ioAccLandingRateBuff::ApplyStartBuff( ioBaseChar *pOwner, DWORD dwGlobalTime, DWORD dwGapTime )
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

	m_fLandingRate = m_fExtendLandingRate = fNum;
	pOwner->SetLandingRateByBuff( m_fLandingRate, m_fExtendLandingRate );
	return true;
}

void ioAccLandingRateBuff::ProcessBuff( float fTimePerSec )
{
	switch( m_iOperationType )
	{
	case OT_SWITCH:
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

void ioAccLandingRateBuff::EndBuff()
{
	m_pOwner->SetLandingRateByBuff( 1.0f, 1.0f );

	ioBuff::EndBuff();
}
