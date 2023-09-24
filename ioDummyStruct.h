#pragma once

#include "ioDummyChar.h"

class ioPlayStage;
class ioPlayMode;
class ioINILoader;
class SP2Packet;

class ioDummyStruct : public ioDummyChar
{			
protected:
	enum DummyStructState
	{
		DSS_START,
		DSS_LOOP,
	};

protected:
	ioHashString m_szCheckBuff;
	DummyStructState m_DummyStructState;
	bool m_bSetAllWeaponDefence;
	ioHashStringVec m_OwnerBuffList;
	ioHashStringVec m_RemoveBuffList;
	bool m_bCheckRange;
	float m_fRange;
	bool m_bOnlyOwnerAddBuff;

public:
	virtual void LoadProperty( ioINILoader &rkLoader );

protected:
	virtual void ProcessState( float fTimerPerSec, DWORD dwPreTime );

public:
	virtual void ApplyDummyCharSync( ioPlayStage *pStage, SP2Packet &rkPacket );
	virtual void SetDieState( bool bEffect );
	virtual bool IsActivateWeapon();
	virtual ApplyWeaponExp ApplyWeapon( ioWeapon *pWeapon );
	virtual void TranslateByConveyer( const D3DXVECTOR3 &vMove );
	virtual bool IsAllWeaponDefence() const;
	virtual int DontMoveEntityLevel() const;
	virtual bool IsDummyVsDummyCollisionSkipState( const ioDummyChar *pDummy );

protected:
	void LoadOwnerBuffList( ioINILoader &rkLoader );
	void LoadRemoveBuffList( ioINILoader &rkLoader );

protected:
	void CheckBuff();
	void ProcessEnd();
	void SetOwenrBuff();
	void CheckRange();

public:
	ioDummyStruct( ioEntityGroup *pGrp, ioPlayMode *pMode );
	virtual ~ioDummyStruct();
};

inline ioDummyStruct* ToDummyStruct( ioDummyChar *pDummy )
{
	if( !pDummy || pDummy->GetType() != DCT_STRUCT )
		return NULL;

	return dynamic_cast< ioDummyStruct* >( pDummy );
}