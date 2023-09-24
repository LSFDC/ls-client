
#include "stdafx.h"

#include "ioWarpFindDummyBuff.h"

#include "ioBaseChar.h"
#include "ioPlayStage.h"

#include "DummyCharDefine.h"

ioWarpFindDummyBuff::ioWarpFindDummyBuff()
{
}

ioWarpFindDummyBuff::ioWarpFindDummyBuff( const ioWarpFindDummyBuff &rhs )
	: ioBuff( rhs ),
	  m_szFindDummyName( rhs.m_szFindDummyName ),
	  m_szWarpAni( rhs.m_szWarpAni ),
	  m_fWarpAniRate( rhs.m_fWarpAniRate )
{
}

ioWarpFindDummyBuff::~ioWarpFindDummyBuff()
{
}

void ioWarpFindDummyBuff::LoadProperty( ioINILoader &rkLoader )
{
	ioBuff::LoadProperty( rkLoader );

	char szBuf[MAX_PATH] = "";
	rkLoader.LoadString_e( "find_dummy_name", "", szBuf, MAX_PATH );
	m_szFindDummyName = szBuf;

	rkLoader.LoadString_e( "warp_ani", "", szBuf, MAX_PATH );
	m_szWarpAni = szBuf;

	m_fWarpAniRate = rkLoader.LoadFloat_e( "warp_ani_rate", 1.0f );
}

ioBuff* ioWarpFindDummyBuff::Clone()
{
	return new ioWarpFindDummyBuff( *this );
}

void ioWarpFindDummyBuff::StartBuff( ioBaseChar *pOwner )
{
	ioBuff::StartBuff( pOwner );

	SetWarpState( pOwner );
}

bool ioWarpFindDummyBuff::ApplyStartBuff( ioBaseChar *pOwner, DWORD dwGlobalTime, DWORD dwGapTime )
{
	if( !ioBuff::ApplyStartBuff( pOwner, dwGlobalTime, dwGapTime ) )
		return false;

	SetWarpState( pOwner );

	return true;
}

void ioWarpFindDummyBuff::ProcessBuff( float fTimePerSec )
{
	if ( m_dwMotionEndTime > 0 && m_dwMotionEndTime < FRAMEGETTIME() && 
		 WarpChar( m_pOwner ) )
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
}

void ioWarpFindDummyBuff::EndBuff()
{
	ioBuff::EndBuff();

	if ( m_pOwner && m_pOwner->GetState() == CS_ETC_STATE )
		m_pOwner->SetState( CS_DELAY, m_bSendDelayNetWork );
}

void ioWarpFindDummyBuff::ApplyExtraBuffInfo( SP2Packet &rkPacket )
{
	D3DXVECTOR3 vNewPos;
	rkPacket >> vNewPos;

	if ( m_pOwner )
		m_pOwner->SetWorldPosition( vNewPos );

	SetReserveEndBuff();
}

bool ioWarpFindDummyBuff::WarpChar( ioBaseChar *pOwner )
{
	if ( !pOwner || !pOwner->IsNeedProcess() || 
		 m_CreateChar.IsEmpty() || m_szFindDummyName.IsEmpty() )
		return false;
	
	ioPlayStage *pPlayStage = pOwner->GetCreator();
	if ( !pPlayStage )
		return false;
	
	ioBaseChar *pCreateChar = pPlayStage->GetBaseChar( m_CreateChar );
	if ( pCreateChar )
	{
		ioDummyChar *pDummyChar = g_DummyCharMgr.FindDummyCharToName( pCreateChar, m_szFindDummyName );
		if ( pDummyChar )
		{
			D3DXVECTOR3 vDummyPos = pDummyChar->GetWorldPosition();
			pOwner->SetWorldPosition( vDummyPos );

			if ( pOwner->IsNeedSendNetwork() )
			{
				SP2Packet kPacket( CUPK_APPLY_EXTRA_BUFF_INFO );
				kPacket << pOwner->GetCharName();
				kPacket << GetName();
				kPacket << vDummyPos;
				P2PNetwork::SendToAllPlayingUser( kPacket );
			}
			return true;
		}
		return false;
	}
	return false;
}

void ioWarpFindDummyBuff::SetWarpState( ioBaseChar *pOwner )
{
	if ( !pOwner )
		return;

	ioEntityGroup *pGrp = pOwner->GetGroup();

	int nAniID = pGrp->GetAnimationIdx( m_szWarpAni );
	if( nAniID == -1 )	
		return;

	float fTimeRate  = m_fWarpAniRate;

	pGrp->ClearAllLoopAni( 100.0f, true );
	pGrp->ClearAllActionAni( 100.0f, true );

	DWORD dwCurTime = FRAMEGETTIME();
	m_dwMotionEndTime = dwCurTime;

	m_dwMotionEndTime += pGrp->GetAnimationFullTime( nAniID ) * fTimeRate;
	
	pGrp->SetActionAni( nAniID, 100.0f, 100.0f, 1.0f, 1.0f/fTimeRate );

	pOwner->CheckCharColSkipSkillTime( nAniID, fTimeRate );

	pOwner->SetState( CS_ETC_STATE );
}