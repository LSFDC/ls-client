#pragma once

#include "ioBuff.h"


class ioBaseChar;
class ioINILoader;

class ioDownRollingBuff : public ioBuff
{
protected:
	CEncrypt<DWORD> m_dwEnableTime;
	CEncrypt<bool> m_bInitialGauge;

	ioHashString m_RollingAction;
	float m_fRollingActionRate;

	ioHashString m_GetUpAction;
	float m_fGetUpActionRate;

	ForceInfoList m_vForceInfoList;

public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ioBuff* Clone();

public:
	virtual void StartBuff( ioBaseChar *pOwner );
	virtual void ProcessBuff( float fTimePerSec );	
	virtual void EndBuff();

	virtual bool ApplyStartBuff( ioBaseChar *pOwner, DWORD dwGlobalTime, DWORD dwGapTime );

protected:
	void CheckDownRolling();

public:
	ioDownRollingBuff();
	ioDownRollingBuff( const ioDownRollingBuff &rhs );
	virtual ~ioDownRollingBuff();
};

