#pragma once
#include "stdafx.h"
#include "ioSpecialStateBase.h"
#include "ioBaseChar.h"
#include "ioOrientBox.h"

ioSpecialStateBase::ioSpecialStateBase() : m_pOwner( NULL )
{
}

ioSpecialStateBase::~ioSpecialStateBase()
{
}

void ioSpecialStateBase::Initialize( ioBaseChar* pOwner, CharState eCurrState )
{
	m_pOwner = pOwner;
}

void ioSpecialStateBase::Finalize( ioBaseChar* pOwner, CharState eCurrState, CharState eNewState )
{
	if( eNewState != CS_USING_SKILL )
	{
		pOwner->ClearAttackFireTimeAndSkill();
		pOwner->ClearReservedSliding();
	}
	pOwner->ClearCharColSkipTime();
}

bool ioSpecialStateBase::ProcessMove( ioBaseChar* pOwner, float fTimePerSec, DWORD dwPreTime, DWORD dwCurTime )
{
	// false�� ������ ��� ioBaseChar::ProcessMove()�� ����
	return false;
}

void ioSpecialStateBase::ProcessKeyInput( ioBaseChar* pOwner )
{
}

void ioSpecialStateBase::CheckFallStateChange( ioBaseChar* pOwner, float fMinFallHeightRate, float fBottomHeight, bool bBottomMaxCheck, float fMaxBottomHeight )
{
	// ( Char Half Height * 2.0 ) * fMinFallHeightRate -> 1.0 �̸� ĳ���� ����
	float fMinFallinRate = ( pOwner->GetWorldCollisionBox().GetExtents( 1 ) * 2.0f ) * fMinFallHeightRate;

	float fCharPosGap = 0.0f;
	if( bBottomMaxCheck )
	{
		float fCheckMapHeight = max( fBottomHeight, fMaxBottomHeight );
		fCharPosGap = pOwner->GetWorldPosition().y - fCheckMapHeight;
	}
	else
	{
		fCharPosGap = pOwner->GetWorldPosition().y - fBottomHeight;
	}

	if( fCharPosGap > fMinFallinRate )
	{
		pOwner->SetFallState( true );
	}
}

bool ioSpecialStateBase::IsCharCollisionSkipState( const ioBaseChar *const pOwner,
												DWORD dwStartTime,
												DWORD dwEndTime,
												DWORD dwSkipType,
												bool bTeamOnly,
												bool bDefense,
												TeamType eOwnerTeam,
												TeamType eOtherTeam ) const
{
	if( Help::CheckCharColState( dwStartTime, dwEndTime, dwSkipType, bTeamOnly, bDefense, eOwnerTeam, eOtherTeam ) )
	{
		return true;
	}

	return false;
}

void ioSpecialStateBase::GetGravity( ioBaseChar* pOwner, float &fCurGravity )
{
	if ( IsNoDropState( pOwner ) )
		fCurGravity = 0.0f;
}

void ioSpecialStateBase::FillJoinSync(  SP2Packet &rkPacket, ioBaseChar* pOwner  )
{
	//�߰� ���� ����ȭ Send
}

void ioSpecialStateBase::ApplyJoinSync( SP2Packet &rkPacket, ioBaseChar* pOwner )
{
	//�߰� ���� ����ȭ Recv
}

void ioSpecialStateBase::ApplyStateSync( SP2Packet &rkPacket, ioBaseChar* pOwner )
{
	//������Ʈ ���� ����ȭ Recv
}