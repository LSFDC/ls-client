#pragma once

#include "ioBuff.h"

class ioINILoader;
class ioBaseChar;

class ioChangeTeamBuff : public ioBuff
{
private:
	enum EndType 
	{
		ET_DEFAULT,
		ET_NODASH,
		ET_NOCHANGE,
	};
protected:
	bool m_bEnableTargetAim;
	DWORD m_BuffEffectID;
	EndType m_iEndType;

public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ioBuff* Clone();

public:
	virtual void StartBuff( ioBaseChar *pOwner );
	virtual void ProcessBuff( float fTimePerSec );
	virtual void EndBuff();

	virtual bool ApplyStartBuff( ioBaseChar *pOwner, DWORD dwGlobalTime, DWORD dwGapTime );

public:
	inline bool IsEnableTargetAim() const { return m_bEnableTargetAim; }

protected:
	virtual void SetAction();
	virtual void ProcessDelay( float fTimePerSec );

protected:
	void CheckingOwnerChar( ioBaseChar *pOwner );

public:
	ioChangeTeamBuff();
	ioChangeTeamBuff( const ioChangeTeamBuff &rhs );
	virtual ~ioChangeTeamBuff();
};

inline ioChangeTeamBuff* ToChangeTeamBuff( ioBuff *pBuff )
{
	if( !pBuff || pBuff->GetTypeID() != BT_CHANGE_TEAM )
		return NULL;

	return dynamic_cast< ioChangeTeamBuff* >( pBuff );
}
