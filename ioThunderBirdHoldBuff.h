#pragma once

#include "ioBuff.h"
#include "ioSpecialStateBase.h"

class ioBaseChar;
class ioINILoader;

class ioThunderBirdHoldBuff : public ioBuff
{
protected:
	ioHashString m_FloatAnimation;
	ioHashStringVec m_OwnerBuffList;

	bool m_bDisableDownState;
	bool m_bNoCancel;
	float m_fEndJumpPower;

	bool m_bSetTeamAttack;
	bool m_bFloating;

public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ioBuff* Clone();

public:
	virtual void StartBuff( ioBaseChar *pOwner );
	virtual bool ApplyStartBuff( ioBaseChar *pOwner, DWORD dwGlobalTime, DWORD dwGapTime );
	virtual void ProcessBuff( float fTimePerSec );
	virtual void EndBuff();

protected:
	void LoadOwnerBuffList( ioINILoader &rkLoader );
	void SetOwnerBuffList( ioBaseChar *pOwner );

protected:
	void SetFloatState( ioBaseChar *pOwner );
	bool CheckOwnerStateCheck( ioBaseChar *pOwner );
	void ProcessEnd( ioBaseChar *pOwner );

public:
	bool IsNoCancel();
	void SetTeamAttack( bool bTeamAttack, bool bFloating );

public:
	ioThunderBirdHoldBuff();
	ioThunderBirdHoldBuff( const ioThunderBirdHoldBuff &rhs );
	virtual ~ioThunderBirdHoldBuff();
};

inline ioThunderBirdHoldBuff* ToThunderBirdHoldBuff( ioBuff *pBuff )
{
	if( !pBuff || pBuff->GetTypeID() != BT_THUNDER_BIRD_HOLD )
		return NULL;

	return dynamic_cast< ioThunderBirdHoldBuff* >( pBuff );
}

//-----------------------------------------------------------------------------------------------------------

class ioThunderBirdHoldSpecialState : public ioSpecialStateBase
{
public:
	virtual void Initialize( ioBaseChar* pOwner, CharState eCurrState );
	virtual void Finalize( ioBaseChar* pOwner, CharState eCurrState, CharState eNewState );

	virtual const char* GetStateName(){ return "CS_THUNDER_BIRD_HOLD"; }
	virtual const CharState GetState(){ return CS_THUNDER_BIRD_HOLD; }	

public:
	virtual void CheckSpecialState( ioBaseChar* pOwner );
	virtual void CheckFallStateChange( ioBaseChar* pOwner, float fMinFallHeightRate, float fBottomHeight, bool bBottomMaxCheck, float fMaxBottomHeight );

public:
	ioThunderBirdHoldSpecialState();
	virtual ~ioThunderBirdHoldSpecialState();
};

inline ioThunderBirdHoldSpecialState* ToThunderBirdHoldSpecialState( ioSpecialStateBase *pState )
{
	if( !pState || pState->GetState() != CS_THUNDER_BIRD_HOLD )
		return NULL;

	return dynamic_cast< ioThunderBirdHoldSpecialState* >( pState );
}