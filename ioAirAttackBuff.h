#pragma once

#include "ioBuff.h"


class ioBaseChar;
class ioINILoader;

class ioAirAttackBuff : public ioBuff
{
protected:
	CEncrypt<bool> m_bInitialGauge;
	CEncrypt<float> m_fEnableHeight;

	AttackAttribute m_AirAttackAttribute;

public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ioBuff* Clone();

public:
	virtual void StartBuff( ioBaseChar *pOwner );
	virtual void ProcessBuff( float fTimePerSec );	
	virtual void EndBuff();

	virtual bool ApplyStartBuff( ioBaseChar *pOwner, DWORD dwGlobalTime, DWORD dwGapTime );

protected:
	void CheckAirAttack();

public:
	ioAirAttackBuff();
	ioAirAttackBuff( const ioAirAttackBuff &rhs );
	virtual ~ioAirAttackBuff();
};

