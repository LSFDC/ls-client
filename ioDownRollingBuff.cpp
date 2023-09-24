

#include "stdafx.h"



#include "ioBaseChar.h"
#include "ioDownRollingBuff.h"

ioDownRollingBuff::ioDownRollingBuff()
{
}

ioDownRollingBuff::ioDownRollingBuff( const ioDownRollingBuff &rhs )
: ioBuff( rhs ),
m_dwEnableTime( rhs.m_dwEnableTime ),
m_bInitialGauge( rhs.m_bInitialGauge ),
m_RollingAction( rhs.m_RollingAction ),
m_fRollingActionRate( rhs.m_fRollingActionRate ),
m_GetUpAction( rhs.m_GetUpAction ),
m_fGetUpActionRate( rhs.m_fGetUpActionRate ),
m_vForceInfoList( rhs.m_vForceInfoList )
{
}

ioDownRollingBuff::~ioDownRollingBuff()
{
}

void ioDownRollingBuff::LoadProperty( ioINILoader &rkLoader )
{
	ioBuff::LoadProperty( rkLoader );

	char szBuf[MAX_PATH] = "";

	m_dwEnableTime = rkLoader.LoadInt_e( "enable_check_time", 0 );
	m_bInitialGauge = rkLoader.LoadBool_e( "initial_gauge", false );

	rkLoader.LoadString_e( "rolling_action", "", szBuf, MAX_PATH );
	m_RollingAction = szBuf;
	rkLoader.LoadString_e( "getup_action", "", szBuf, MAX_PATH );
	m_GetUpAction = szBuf;

	m_fRollingActionRate = rkLoader.LoadFloat_e( "rolling_action_rate", FLOAT1 );
	m_fGetUpActionRate = rkLoader.LoadFloat_e( "getup_action_rate", FLOAT1 );

	m_vForceInfoList.clear();
	int iForceCnt = rkLoader.LoadInt_e( "rolling_force_cnt", 0 );
	if( iForceCnt > 0 )
	{
		m_vForceInfoList.reserve( iForceCnt );
	}

	for( int i=0; i < iForceCnt; ++i )
	{
		ForceInfo kInfo;

		wsprintf_e( szBuf, "rolling_force_move%d", i+1 );
		kInfo.m_fForceMovePower = rkLoader.LoadFloat( szBuf, 0.0f );

		wsprintf_e( szBuf, "rolling_force_friction%d", i+1 );
		kInfo.m_fForceMoveFriction = rkLoader.LoadFloat( szBuf, FLOAT1 );

		m_vForceInfoList.push_back( kInfo );
	}
}

ioBuff* ioDownRollingBuff::Clone()
{
	return new ioDownRollingBuff( *this );
}

void ioDownRollingBuff::StartBuff( ioBaseChar *pOwner )
{
	ioBuff::StartBuff( pOwner );
}

bool ioDownRollingBuff::ApplyStartBuff( ioBaseChar *pOwner, DWORD dwGlobalTime, DWORD dwGapTime )
{
	if( !ioBuff::ApplyStartBuff( pOwner, dwGlobalTime, dwGlobalTime ) )
		return false;

	return true;
}

void ioDownRollingBuff::ProcessBuff( float fTimePerSec )
{
	switch( m_iOperationType )
	{
	case OT_SWITCH:
		if( !CheckSwitchType( fTimePerSec ) )
		{
			SetReserveEndBuff();
			return;
		}

		if( m_pOwner->GetState() == CS_BLOW_WOUNDED ||
			m_pOwner->GetState() == CS_BOUND_BLOW_WOUNDED ||
			m_pOwner->GetState() == CS_BLOW_EXTEND_WOUND )
		{
			CheckDownRolling();
		}

		CheckCallingProcess();
		break;
	case OT_DELAY:
	case OT_ACTIVE:
		if( m_pOwner->GetState() == CS_BLOW_WOUNDED ||
			m_pOwner->GetState() == CS_BOUND_BLOW_WOUNDED ||
			m_pOwner->GetState() == CS_BLOW_EXTEND_WOUND )
		{
			CheckDownRolling();
		}

		ioBuff::ProcessBuff( fTimePerSec );
		break;
	case OT_PASSIVE:
		if( m_pOwner->GetState() == CS_BLOW_WOUNDED ||
			m_pOwner->GetState() == CS_BOUND_BLOW_WOUNDED ||
			m_pOwner->GetState() == CS_BLOW_EXTEND_WOUND )
		{
			CheckDownRolling();
		}

		CheckCallingAreaWeapon();
		break;
	}
}

void ioDownRollingBuff::CheckDownRolling()
{
	if( !m_pOwner ) return;
	if( !m_pOwner->IsOwnerChar() ) return;
	if( m_pOwner->IsPrisonerMode() ) return;
	if( m_pOwner->IsCatchMode() ) return;

	if( m_pOwner->HasDisableSkillFlameBuff() ) return;
	if( !m_pOwner->IsBlowWoundedDownState( m_dwEnableTime ) ) return;

	bool bSetAni = false;
	if( m_pOwner->IsSettedDirection() )
	{
		m_pOwner->SetDownRollingState( m_RollingAction, m_fRollingActionRate,
									   m_GetUpAction, m_fGetUpActionRate,
									   m_vForceInfoList );
		bSetAni = true;
	}

	ioItem *pOwnerItem = m_pOwner->GetEquipedItem( m_ItemName );

	/*if( m_pOwnerItem && m_bInitialGauge && bSetAni )
	{
		m_pOwnerItem->SetCurSkillGauge( 0.0f );
	}*/
	if( pOwnerItem && m_bInitialGauge && bSetAni )
	{
		pOwnerItem->SetCurSkillGauge( 0.0f );
	}
}

void ioDownRollingBuff::EndBuff()
{
	ioBuff::EndBuff();
}
