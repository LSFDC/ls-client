#include "stdafx.h"

#include "ioAimJump.h"
#include "ioBaseChar.h"

ioAimJump::ioAimJump()
{
	m_bDashEnd = false;
	m_bJumpAttackReserve = false;
	m_dwActionEndTime = 0;
	m_dwReserveEnableTime = 0;

	m_CurJumpState = AJS_NONE;
}

ioAimJump::ioAimJump( const ioAimJump &rhs )
: ioExtendJump( rhs ),
m_fExtraJumpPowerRate( rhs.m_fExtraJumpPowerRate ),
m_dwJumpChargeTime( rhs.m_dwJumpChargeTime ),
m_DashAction( rhs.m_DashAction ),
m_fActionTimeRate( rhs.m_fActionTimeRate ),
m_vForceInfoList( rhs.m_vForceInfoList ),
m_bEnableJumpDash( rhs.m_bEnableJumpDash )
{
	m_bDashEnd = false;
	m_bJumpAttackReserve = false;
	m_dwActionEndTime = 0;
	m_dwReserveEnableTime = 0;

	m_CurJumpState = AJS_NONE;
}

ioExtendJump* ioAimJump::Clone()
{
	return new ioAimJump( *this );
}

ioAimJump::~ioAimJump()
{
}

JumpType ioAimJump::GetType() const
{
	return JT_AIM_JUMP;
}

void ioAimJump::LoadProperty( ioINILoader &rkLoader )
{
	ioExtendJump::LoadProperty( rkLoader );
	m_dwJumpChargeTime = rkLoader.LoadInt_e( "jump_charge_time", 0 );
	m_fExtraJumpPowerRate = rkLoader.LoadFloat_e( "jump_extra_power_rate", FLOAT1 );

	char szBuf[MAX_PATH] = "";

	// Dash
	m_bEnableJumpDash = rkLoader.LoadBool_e( "enable_jump_dash", false );

	rkLoader.LoadString_e( "dash_jump_action", "", szBuf, MAX_PATH );
	m_DashAction = szBuf;

	m_fActionTimeRate = rkLoader.LoadFloat_e( "dash_jump_action_time_rate", FLOAT1 );

	m_vForceInfoList.clear();
	int iForceCnt = rkLoader.LoadInt_e( "dash_jump_force_cnt", 0 );
	if( iForceCnt > 0 )
		m_vForceInfoList.reserve( iForceCnt );

	for( int i=0; i < iForceCnt; ++i )
	{
		ForceInfo kInfo;

		wsprintf_e( szBuf, "dash_jump_force_move%d", i+1 );
		kInfo.m_fForceMovePower = rkLoader.LoadFloat( szBuf, 0.0f );

		wsprintf_e( szBuf, "dash_jump_force_friction%d", i+1 );
		kInfo.m_fForceMoveFriction = rkLoader.LoadFloat( szBuf, FLOAT1 );

		m_vForceInfoList.push_back( kInfo );
	}
}

void ioAimJump::SetJumpping( ioBaseChar *pOwner, bool bFullTime )
{
	m_bDashEnd = false;
	m_bJumpAttackReserve = false;
	m_dwActionEndTime = 0;
	m_dwReserveEnableTime = 0;

	m_CurJumpState = AJS_NONE;

	float fJumpPower = pOwner->GetJumpPower();
	if( bFullTime )
	{
		fJumpPower *= m_fExtraJumpPowerRate;
		m_bUseExtendJump = true;

		pOwner->SetJumpPower( fJumpPower );
		pOwner->SetEnableHide( false );
		pOwner->SetNoWoundState( false );
		pOwner->SetEnableChangeTeam( false );

		pOwner->SetChangeJumpMotion( false, "", "", "", 0.0f );
		g_TutorialMgr.ActionHighJump();
	}
}

DWORD ioAimJump::GetChargingTime( ioBaseChar *pChar )
{
	return m_dwJumpChargeTime;
}

void ioAimJump::ProcessJumpping( ioBaseChar *pOwner, float fHeightGap, float fNeedGauge )
{
	if( !m_bEnableJumpDash )
	{
		return;
	}

	// DashCheck
	if( IsCanJumpDash( pOwner, fHeightGap ) )
	{
		ioEntityGroup *pGrp = pOwner->GetGroup();
		int iAniID = pGrp->GetAnimationIdx( m_DashAction );
		if( pOwner && iAniID != -1 )
		{
			pOwner->SetJumppingState();

			m_CurJumpState = AJS_DASH;
			m_bUseExtendJump = true;

			m_dwActionEndTime = 0;

			pOwner->SetCurJumpPowerAmt( 0.0f );
			pOwner->SetForcePowerAmt( 0.0f );
			pOwner->SetGravityAmt( 0.0f );

			pOwner->ChangeDirectionByInputDir( false );

			pGrp->ClearAllActionAni( FLOAT100, true );
			pGrp->ClearAllLoopAni( FLOAT100, true );
			pGrp->SetActionAni( iAniID, FLOAT100, FLOAT100, FLOAT1, FLOAT1/m_fActionTimeRate, 0.0f, 0, true );

			pOwner->SetEnableHide( false );
			pOwner->SetEnableChangeTeam( false );
			pOwner->SetChangeJumpMotion( false, "", "", "", 0.0f );

			pOwner->ClearJumpAimState();

			D3DXVECTOR3 vMoveDir = pOwner->GetMoveDir();

			DWORD dwCurTime = FRAMEGETTIME();
			DWORD dwDuration = pGrp->GetAnimationFullTime( iAniID ) * m_fActionTimeRate;

			m_dwActionEndTime = dwCurTime;
			m_dwActionEndTime += dwDuration;

			pOwner->SetReservedSliding( m_vForceInfoList, iAniID, m_fActionTimeRate, 0 );

			CheckKeyReserveTime( pOwner, iAniID, m_fActionTimeRate );

			if( pOwner->IsNeedSendNetwork() )
			{
				SP2Packet kPacket( CUPK_JUMP_DASH );
				kPacket << pOwner->GetCharName();
				kPacket << (int)GetType();
				kPacket << vMoveDir;
				kPacket << (int)m_CurJumpState;
				kPacket << false;					// dash end
				kPacket << pOwner->GetTargetRot();
				P2PNetwork::SendToAllPlayingUser( kPacket );
			}
		}
	}

	ProcessJumpDash( pOwner );
}

bool ioAimJump::IsCanJumpDash( ioBaseChar *pOwner, float fHeightGap )
{
	if( !pOwner ) return false;
	if( !pOwner->IsNeedProcess() ) return false;

	if( pOwner->IsHasCrown() ||
		pOwner->IsCatchMode() ||
		pOwner->IsPrisonerMode() ||
		(pOwner->GetUsedBuffJump() != BJUT_NONE) ||
		(pOwner->GetUsedBuffFlyJump() != BJUT_NONE) ||
		(pOwner->GetUsedBuffMultiFlyJump() != BJUT_NONE) ||
		pOwner->GetUsedJumpAttack() )
		return false;

	if( pOwner->IsBuffLimitExtendJump() )
		return false;

	if( pOwner->HasBuff(BT_AIR_ATTACK) )
		return false;

	if( m_CurJumpState != AJS_NONE )
		return false;

	DWORD dwMoveStartTime = pOwner->GetJumpMoveStartTime();
	if( dwMoveStartTime == 0 ) return false;

	if( !pOwner->IsDirKeyDoubleClick() )
		return false;

	return true;
}

void ioAimJump::ProcessJumpDash( ioBaseChar *pOwner )
{
	if( !pOwner ) return;
	if( m_CurJumpState != AJS_DASH ) return;
	if( m_bDashEnd ) return;
	if( m_dwActionEndTime == 0 ) return;

	DWORD dwCurTime = FRAMEGETTIME();
	if( m_dwActionEndTime < dwCurTime )
	{
		CheckKeyReserve( pOwner );

		int iAniID = pOwner->GetJumppingAnimationIdx();
		if( pOwner && iAniID != -1 )
		{
			ioEntityGroup *pGrp = pOwner->GetGroup();
			if( pGrp )
			{
				pGrp->ClearAllActionAni( FLOAT100, true );
				pGrp->ClearAllLoopAni( FLOAT100, true );
				pGrp->SetActionAni( iAniID,	0.0f, 0.0f, FLOAT1, FLOAT1,	0.0f, 0.0f,	true );

				m_bDashEnd = true;

				if( pOwner->IsNeedProcess() && pOwner->IsSettedDirection() )
				{
					pOwner->ChangeDirectionByInputDir( false );

					D3DXVECTOR3 vMoveDir = pOwner->GetMoveDir();

					if( pOwner->IsNeedSendNetwork() )
					{
						SP2Packet kPacket( CUPK_JUMP_DASH );
						kPacket << pOwner->GetCharName();
						kPacket << (int)GetType();
						kPacket << vMoveDir;
						kPacket << (int)m_CurJumpState;
						kPacket << true;					// dash end
						kPacket << pOwner->GetTargetRot();
						P2PNetwork::SendToAllPlayingUser( kPacket );
					}
				}
			}
		}
	}
}

void ioAimJump::CheckKeyReserve( ioBaseChar *pOwner )
{
	bool bReserveSkill = false;

	if( pOwner->IsAttackKeyDown() && !m_bJumpAttackReserve && !pOwner->IsChatModeState( true ) )
	{
		m_bJumpAttackReserve = true;
	}
}

void ioAimJump::ApplyJump( ioBaseChar *pOwner, SP2Packet &rkPacket )
{
	if( !pOwner )
		return;

	int iType;
	rkPacket >> iType;

	switch( iType )
	{
	case AJS_DASH:
		{
			bool bEnd;
			rkPacket >> bEnd;

			D3DXQUATERNION qtRotate;
			rkPacket >> qtRotate;

			if( !bEnd )
			{
				ioEntityGroup *pGrp = pOwner->GetGroup();
				int iAniID = pGrp->GetAnimationIdx( m_DashAction );
				if( pOwner && iAniID != -1 )
				{
					pOwner->SetJumppingState();

					m_CurJumpState = AJS_DASH;
					m_bUseExtendJump = true;

					m_dwActionEndTime = 0;

					pOwner->SetCurJumpPowerAmt( 0.0f );
					pOwner->SetForcePowerAmt( 0.0f );
					pOwner->SetGravityAmt( 0.0f );

					pOwner->SetTargetRot( qtRotate );

					pGrp->ClearAllActionAni( FLOAT100, true );
					pGrp->ClearAllLoopAni( FLOAT100, true );
					pGrp->SetActionAni( iAniID, FLOAT100, FLOAT100, FLOAT1, FLOAT1/m_fActionTimeRate, 0.0f, 0, true );

					pOwner->SetEnableHide( false );
					pOwner->SetEnableChangeTeam( false );
					pOwner->SetChangeJumpMotion( false, "", "", "", 0.0f );

					pOwner->ClearJumpAimState();

					DWORD dwCurTime = FRAMEGETTIME();
					DWORD dwDuration = pGrp->GetAnimationFullTime( iAniID ) * m_fActionTimeRate;

					m_dwActionEndTime = dwCurTime;
					m_dwActionEndTime += dwDuration;

					pOwner->SetReservedSliding( m_vForceInfoList, iAniID, m_fActionTimeRate, 0 );
				}
			}
			else
			{
				pOwner->SetTargetRot( qtRotate );
			}
		}
		break;
	}
}

bool ioAimJump::IsCanJumpAttack( ioBaseChar *pOwner )
{
	if( m_CurJumpState == AJS_DASH )
	{
		if( m_dwActionEndTime > 0 && m_dwActionEndTime >= FRAMEGETTIME() )
			return false;
	}

	return true;
}

bool ioAimJump::IsJumpDashState()
{
	if( m_CurJumpState == AJS_DASH )
	{
		if( m_dwActionEndTime > 0 && m_dwActionEndTime >= FRAMEGETTIME() )
			return true;
	}

	return false;
}

bool ioAimJump::IsJumpAttackReserve()
{
	return m_bJumpAttackReserve;
}

bool ioAimJump::ProcessAimJump( ioBaseChar *pOwner, float fHeightGap )
{
	if( !m_bEnableJumpDash )
	{
		return false;
	}

	// DashCheck
	bool bDash = false;
	if( IsCanJumpDash( pOwner, fHeightGap ) )
	{
		ioEntityGroup *pGrp = pOwner->GetGroup();
		int iAniID = pGrp->GetAnimationIdx( m_DashAction );
		if( pOwner && iAniID != -1 )
		{
			pOwner->SetJumppingState();

			bDash = true;
			m_CurJumpState = AJS_DASH;
			m_bUseExtendJump = true;

			m_dwActionEndTime = 0;

			pOwner->SetCurJumpPowerAmt( 0.0f );
			pOwner->SetForcePowerAmt( 0.0f );
			pOwner->SetGravityAmt( 0.0f );

			pOwner->ChangeDirectionByInputDir( false );

			pGrp->ClearAllActionAni( FLOAT100, true );
			pGrp->ClearAllLoopAni( FLOAT100, true );
			pGrp->SetActionAni( iAniID, FLOAT100, FLOAT100, FLOAT1, FLOAT1/m_fActionTimeRate, 0.0f, 0, true );

			pOwner->SetEnableHide( false );
			pOwner->SetEnableChangeTeam( false );
			pOwner->SetChangeJumpMotion( false, "", "", "", 0.0f );

			pOwner->ClearJumpAimState();

			D3DXVECTOR3 vMoveDir = pOwner->GetMoveDir();

			DWORD dwCurTime = FRAMEGETTIME();
			DWORD dwDuration = pGrp->GetAnimationFullTime( iAniID ) * m_fActionTimeRate;

			m_dwActionEndTime = dwCurTime;
			m_dwActionEndTime += dwDuration;

			pOwner->SetReservedSliding( m_vForceInfoList, iAniID, m_fActionTimeRate, 0 );

			CheckKeyReserveTime( pOwner, iAniID, m_fActionTimeRate );

			if( pOwner->IsNeedSendNetwork() )
			{
				SP2Packet kPacket( CUPK_JUMP_DASH );
				kPacket << pOwner->GetCharName();
				kPacket << (int)GetType();
				kPacket << vMoveDir;
				kPacket << (int)m_CurJumpState;
				kPacket << false;					// dash end
				kPacket << pOwner->GetTargetRot();
				P2PNetwork::SendToAllPlayingUser( kPacket );
			}
		}
	}

	ProcessJumpDash( pOwner );

	return bDash;
}

bool ioAimJump::IsEnableJumpDash( ioBaseChar *pOwner )
{
	if( !pOwner )
		return false;

	if( !m_bEnableJumpDash )
		return false;

	if( pOwner->GetJumpState() == JS_JUMPPING )
		return true;
	else if( pOwner->GetJumpState() == JS_AIM_JUMP )
		return true;

	return false;
}





