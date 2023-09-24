
#include "stdafx.h"

#include "ioBaseChar.h"
#include "ioPlayStage.h"
#include "ioTargetControlBuff.h"


ioTargetControlBuff::ioTargetControlBuff()
{
}

ioTargetControlBuff::ioTargetControlBuff( const ioTargetControlBuff &rhs )
	: ioBuff( rhs ),
	m_szTransferOwnerList( rhs.m_szTransferOwnerList ),
	m_bDropZoneCheck( rhs.m_bDropZoneCheck ),
	m_fControlOutRange( rhs.m_fControlOutRange ),
	m_bDisableKeyInputCheck( rhs.m_bDisableKeyInputCheck )
{
	m_bNotTargetNpc = true;
	m_iCurUseCount  = 0;
}

ioTargetControlBuff::~ioTargetControlBuff()
{
}

void ioTargetControlBuff::LoadProperty( ioINILoader &rkLoader )
{
	ioBuff::LoadProperty( rkLoader );
	
	LoadOwnerTransferBuffExceptList( rkLoader );
	m_bNotTargetNpc = true;

	m_bDropZoneCheck		= rkLoader.LoadBool_e( "drop_zone_check", false );
	m_fControlOutRange		= rkLoader.LoadFloat_e( "control_out_range", 0.0f );
	m_bDisableKeyInputCheck = rkLoader.LoadBool_e( "disable_key_input_check", false );
}

void ioTargetControlBuff::LoadOwnerTransferBuffExceptList( ioINILoader &rkLoader )
{
	char szBuf[MAX_PATH], szKey[MAX_PATH];

	int iCnt = rkLoader.LoadInt_e( "transfer_owner_exception_cnt", 0 );
	m_szTransferOwnerList.clear();

	for( int i=0; i < iCnt; i++ )
	{
		wsprintf_e( szKey, "transfer_owner_buff%d", i+1 );
		rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );

		ioHashString szName = szBuf;
		if( !szName.IsEmpty() )
			m_szTransferOwnerList.push_back( szName );
	}
}

ioBuff* ioTargetControlBuff::Clone()
{
	return new ioTargetControlBuff( *this );
}

void ioTargetControlBuff::StartBuff( ioBaseChar *pOwner )
{
	ioBuff::StartBuff( pOwner );

	if( pOwner )
	{
		if ( m_bDisableKeyInputCheck )
		{
			pOwner->EnableKeyInputCheck( false );
		}		
		pOwner->EnableKeyInputMust( false );
	}

	m_iCurUseCount = 0;
}

bool ioTargetControlBuff::ApplyStartBuff( ioBaseChar *pOwner, DWORD dwGlobalTime, DWORD dwGapTime )
{
	if( !ioBuff::ApplyStartBuff( pOwner, dwGlobalTime, dwGapTime ) )
		return false;

	m_iCurUseCount = 0;

	return true;
}

void ioTargetControlBuff::RemoveOldTargetControlBuff( ioBaseChar* pOwner )
{
	if( !pOwner )
		return;

	pOwner->SetReserveEndBuff( BT_TARGET_CONTOL, this );
}


void ioTargetControlBuff::ProcessStartCreatorSync()
{
	ioBaseChar* pCreator = GetCreator();
	ioBaseChar* pOwner   = GetOwner();
	if( !pCreator || !pOwner )
	{
		SetReserveEndBuff();
		return;
	}

	if( pOwner->GetState() != CS_DELAY && pOwner->GetState() != CS_ATTACK && pOwner->GetState() != CS_RUN )
		return;

	if( 0 < m_iCurUseCount )
		return;

	switch( pCreator->GetState() )
	{
	case CS_RUN:
		{
			pCreator->SetTargetControlState( CS_RUN );
			m_iCurUseCount++;
		}
		break;
	case CS_ATTACK:
		{
			pCreator->SetTargetControlState( CS_ATTACK );
			m_iCurUseCount++;
		}
		break;
	case CS_DELAY:
		{
			pCreator->SetTargetControlState( CS_DELAY );
			m_iCurUseCount++;
		}
		break;
	}
}

void ioTargetControlBuff::ProcessBuff( float fTimePerSec )
{
	switch( m_iOperationType )
	{
	case OT_SWITCH:
		break;
	case OT_DELAY:
	case OT_ACTIVE:
		{
			ProcessStartCreatorSync();
			ProcessRun( fTimePerSec );
			ioBuff::ProcessBuff( fTimePerSec );
			CheckDropZone();
			CheckOutRange();
		}
		break;
	case OT_PASSIVE:
		{
			CheckCallingAreaWeapon();
		}
		break;
	}
}

void ioTargetControlBuff::ProcessRun( float fTimePerSec )
{
	ioBaseChar* pCreator = GetCreator();
	ioBaseChar* pOwner   = GetOwner();
	if( !pCreator || !pOwner )
	{
		SetReserveEndBuff();
		return;
	}

	switch( pOwner->GetState() )
	{
	case CS_ATTACK:
	case CS_RUN:
		if( IsLive() )
		{
			D3DXVECTOR3 vDir = pCreator->GetMoveDir();
			pOwner->SetTargetRotToDir( vDir, true, false );
		}
		break;
	}
}

void ioTargetControlBuff::EndBuff()
{
	ioBuff::EndBuff();

	ioBaseChar* pOwner = GetOwner();
	if( pOwner )
	{
		if ( m_bDisableKeyInputCheck )
		{
			pOwner->EnableKeyInputCheck( true );
		}	
		pOwner->EnableKeyInputMust( true );
		switch( pOwner->GetState() )
		{
		case CS_RUN:
		case CS_DELAY:
			pOwner->SetState( CS_DELAY, m_bSendDelayNetWork );
			break;
		}
	}
}

void ioTargetControlBuff::CheckDropZone()
{
	ioBaseChar* pOwner = GetOwner();
	if( !pOwner )
	{
		SetReserveEndBuff();
		return;
	}

	if ( pOwner->IsCharDropZonePos() && m_bDropZoneCheck )
	{
		SetReserveEndBuff();
		return;
	}
}

void ioTargetControlBuff::CheckOutRange()
{
	if ( m_fControlOutRange == 0.0f )
		return;

	ioBaseChar* pCreator = GetCreator();
	ioBaseChar* pOwner = GetOwner();
	if( !pCreator || !pOwner )
	{
		SetReserveEndBuff();
		return;
	}

	D3DXVECTOR3 vDiff = pOwner->GetWorldPosition() - pCreator->GetWorldPosition();
	float fDistanceSq = D3DXVec3LengthSq( &vDiff );

	if( m_fControlOutRange * m_fControlOutRange < fDistanceSq )
	{
		SetReserveEndBuff();
		return;
	}
}

