#pragma once

#include "ioExtendDash.h"

class ioBaseChar;
class ioINILoader;

class ioRollingDash : public ioExtendDash
{
protected:
	ioHashString m_DashAction;
	DWORD m_dwActionPreDelay;
	DWORD m_dwActionDelay;
	float m_fActionTimeRate;

	ForceInfoList m_vForceInfoList;

	DWORD m_dwActionEndTime;

	bool m_bUseDashEndDir;
	
public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual DashType GetType() const;
	virtual ioExtendDash* Clone();

public:
	virtual bool StartDash( ioBaseChar *pOwner );
	virtual void ProcessDash( ioBaseChar *pOwner );
	virtual void ApplyDash( ioBaseChar *pOwner, SP2Packet &rkPacket );

protected:
	virtual void CheckKeyReserve( ioBaseChar *pOwner );

public:
	ioRollingDash();
	ioRollingDash( const ioRollingDash &rhs );
	virtual ~ioRollingDash();
};

inline ioRollingDash* ToRollingDash( ioExtendDash *pDash )
{
	if( !pDash || pDash->GetType() != DT_ROLLING_AIM )
		return NULL;

	return dynamic_cast< ioRollingDash* >( pDash );
}

