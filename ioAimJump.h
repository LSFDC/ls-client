#pragma once

#include "ioExtendJump.h"

class ioBaseChar;
class ioINILoader;
class SP2Packet;

class ioAimJump : public ioExtendJump
{
public:
	enum AimJumpState
	{
		AJS_NONE,
		AJS_DASH,
	};

protected:
	AimJumpState m_CurJumpState;
	DWORD m_dwJumpChargeTime;
	float m_fExtraJumpPowerRate;

// Dash
protected:
	bool m_bEnableJumpDash;
	ioHashString m_DashAction;
	float m_fActionTimeRate;

	ForceInfoList m_vForceInfoList;
	DWORD m_dwActionEndTime;

	bool m_bDashEnd;
	bool m_bJumpAttackReserve;

public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ioExtendJump* Clone();
	virtual JumpType GetType() const;

public:
	virtual void SetJumpping( ioBaseChar *pOwner, bool bFullTime );
	virtual void ProcessJumpping( ioBaseChar *pOwner, float fHeightGap, float fNeedGauge );
	virtual void ApplyJump( ioBaseChar *pOwner, SP2Packet &rkPacket );

	virtual bool ProcessAimJump( ioBaseChar *pOwner, float fHeightGap );

	virtual DWORD GetChargingTime( ioBaseChar *pChar );

	virtual bool IsCanJumpAttack( ioBaseChar *pOwner );

	virtual bool IsEnableJumpDash( ioBaseChar *pOwner );
	bool IsJumpDashState();
	bool IsJumpAttackReserve();

protected:
	bool IsCanJumpDash( ioBaseChar *pOwner, float fHeightGap );
	void ProcessJumpDash( ioBaseChar *pOwner );
	
	virtual void CheckKeyReserve( ioBaseChar *pOwner );

public:
	ioAimJump();
	ioAimJump( const ioAimJump &rhs );
	virtual ~ioAimJump();
};

inline ioAimJump* ToAimJump( ioExtendJump *pJump )
{
	if( !pJump || pJump->GetType() != JT_AIM_JUMP )
		return NULL;

	return dynamic_cast< ioAimJump* > (pJump);
}

