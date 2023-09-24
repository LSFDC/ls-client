

#include "stdafx.h"



#include "ioBaseChar.h"
#include "ioAirAttackBuff.h"

ioAirAttackBuff::ioAirAttackBuff()
{
}

ioAirAttackBuff::ioAirAttackBuff( const ioAirAttackBuff &rhs )
: ioBuff( rhs ),
m_AirAttackAttribute( rhs.m_AirAttackAttribute ),
m_fEnableHeight( rhs.m_fEnableHeight ),
m_bInitialGauge( rhs.m_bInitialGauge )
{
}

ioAirAttackBuff::~ioAirAttackBuff()
{
}

void ioAirAttackBuff::LoadProperty( ioINILoader &rkLoader )
{
	ioBuff::LoadProperty( rkLoader );

	char szBuf[MAX_PATH] = "";

	m_bInitialGauge = rkLoader.LoadBool_e( "initial_gauge", false );
	m_fEnableHeight = rkLoader.LoadFloat_e( "enable_height", 0.0f );

	wsprintf_e( szBuf, "air_attack" );
	LoadAttackAttribute( szBuf, m_AirAttackAttribute, rkLoader );
}

ioBuff* ioAirAttackBuff::Clone()
{
	return new ioAirAttackBuff( *this );
}

void ioAirAttackBuff::StartBuff( ioBaseChar *pOwner )
{
	ioBuff::StartBuff( pOwner );

	m_pOwner->SetBuffAirAttackInfo( m_AirAttackAttribute );
}

bool ioAirAttackBuff::ApplyStartBuff( ioBaseChar *pOwner, DWORD dwGlobalTime, DWORD dwGapTime )
{
	if( !ioBuff::ApplyStartBuff( pOwner, dwGlobalTime, dwGlobalTime ) )
		return false;

	m_pOwner->SetBuffAirAttackInfo( m_AirAttackAttribute );

	return true;
}

void ioAirAttackBuff::ProcessBuff( float fTimePerSec )
{
	if( m_pOwner->GetState() == CS_JUMP )
	{
		CheckAirAttack();
	}

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

void ioAirAttackBuff::CheckAirAttack()
{
	if( !m_pOwner ) return;
	if( !m_pOwner->IsNeedProcess() ) return;

	// ���°� üũ
	if( m_pOwner->IsHasCrown() ||
		(m_pOwner->GetUsedBuffJump() != BJUT_NONE) ||
		(m_pOwner->GetUsedBuffFlyJump() != BJUT_NONE) ||
		(m_pOwner->GetUsedBuffMultiFlyJump() != BJUT_NONE) ||
		!m_pOwner->CheckBuffAirAttackEnable() )
		return;

	if( m_pOwner->IsBuffLimitExtendJump() )
		return;

	// ���̰� üũ
	float fGapHeight = fabs(m_pOwner->GetBottomHeight() - m_pOwner->GetWorldPosition().y);
	if( fGapHeight < m_fEnableHeight )
	{
		return;
	}

	bool bSetAni = false;
	if( m_pOwner->IsDirKeyDoubleClick() )
	{
		m_pOwner->SetBuffAirAttackState();
		bSetAni = true;
	}

	//if( m_pOwnerItem && m_bInitialGauge && bSetAni )
	if( !m_ItemName.IsEmpty() && m_bInitialGauge && bSetAni )
	{
		ioItem *pItem = m_pOwner->GetEquipedItem( m_ItemName );
		if( pItem )
			pItem->SetCurSkillGauge( 0.0f );
		//m_pOwnerItem->SetCurSkillGauge( 0.0f );
	}
}

void ioAirAttackBuff::EndBuff()
{
	ioBuff::EndBuff();
}
