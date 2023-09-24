#pragma once

#include "ioDummyChar.h"

class ioEntityGroup;
class ioPlayStage;
class ioPlayMode;
class ioGaugeHelper;
class ioINILoader;
class ioWeapon;
class SP2Packet;
class ioBaseChar;
class ioAniCollisionBoxGrp;

class ioRaptorDummy : public ioDummyChar
{
public:
	virtual void LoadProperty( ioINILoader &rkLoader );

protected:
	virtual void ProcessState( float fTimerPerSec, DWORD dwPreTime );
	virtual void ProcessPos( float fTimePerSec );

	virtual void ProcessDelayState( float fTimePerSec );
	virtual void ProcessMoveState( float fTimePerSec );
	virtual void ProcessDashState( float fTimePerSec );

	virtual void ProcessAttackState( float fTimePerSec, DWORD dwPreTime );

public:
	virtual void SetAttackState();

	virtual void SetMoveState();
	virtual void SetDashState();

	void SetMoveStateMid();
	void SetDashStateMid();

public:
	ioRaptorDummy( ioEntityGroup *pGrp, ioPlayMode *pMode );
	virtual ~ioRaptorDummy();
};

inline ioRaptorDummy* ToRaptorDummy( ioDummyChar *pDummy )
{
	if( !pDummy || pDummy->GetType() != DCT_RAPTOR )
		return NULL;

	return dynamic_cast< ioRaptorDummy* >( pDummy );
}