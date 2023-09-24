#pragma once


class ioBaseChar;
class ioINILoader;

class ioCallBuffOnAttackBuff : public ioBuff
{
protected:
	bool m_bDisableDownState;
	bool m_bUseWeaponEquipMesh;

	ioHashString	m_CallBuffName;

public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ioBuff* Clone();

public:
	virtual void StartBuff( ioBaseChar *pOwner );
	virtual bool ApplyStartBuff( ioBaseChar *pOwner, DWORD dwGlobalTime, DWORD dwGapTime );
	virtual void ProcessBuff( float fTimePerSec );
	virtual void EndBuff();

protected:
	bool CheckOwnerStateCheck();
	bool CheckRestoreStateCheck();

public:
	inline const ioHashString& GetCallBuffName() const { return m_CallBuffName; }

public:
	ioCallBuffOnAttackBuff();
	ioCallBuffOnAttackBuff( const ioCallBuffOnAttackBuff &rhs );
	virtual ~ioCallBuffOnAttackBuff();
};


inline ioCallBuffOnAttackBuff* ToCallBuffOnAttackBuff( ioBuff *pBuff )
{
	if( !pBuff || pBuff->GetTypeID() != BT_CALL_BUFF_ON_ATTACK )
		return NULL;

	return dynamic_cast< ioCallBuffOnAttackBuff* >( pBuff );
}


