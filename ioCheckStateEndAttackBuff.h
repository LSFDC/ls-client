#pragma once

#include "ioBuff.h"

class ioBaseChar;
class ioINILoader;
class ioEffect;

class ioCheckStateEndAttackBuff : public ioBuff
{
public:
	enum BuffState
	{
		BS_WAIT,
		BS_EXPLOSION
	};

protected:
	BuffState m_BuffState;
	ioHashString m_szCheckBuffName;

	int m_nAttackWeapon;
	DWORD m_dwWeaponBaseIndex;

	ioHashString m_szAttackAnimation;
	float m_fAttackTimeRate;

	DWORD m_dwFireTime;
	int m_nCheckState;

public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ioBuff* Clone();

public:
	virtual void StartBuff( ioBaseChar *pOwner );
	virtual bool ApplyStartBuff( ioBaseChar *pOwner, DWORD dwGlobalTime, DWORD dwGapTime );
	virtual void ProcessBuff( float fTimePerSec );
	virtual void EndBuff();

protected:
	//ĳ���� ���� Ȯ��
	void CheckCharState( ioBaseChar *pOwner );
	//���� ������ �������� Ȯ��
	bool IsEnableAttack( ioBaseChar *pOwner );
	//���� �ִϸ��̼� ����
	void SetAttackTime( ioBaseChar *pOwner, ioHashString szName, float fRate );
	//���� ���� Ȯ��
	void CheckCreateWeapon( ioBaseChar *pOwner );

public:
	ioCheckStateEndAttackBuff();
	ioCheckStateEndAttackBuff( const ioCheckStateEndAttackBuff &rhs );
	virtual ~ioCheckStateEndAttackBuff();
};

inline ioCheckStateEndAttackBuff* ToCheckStateEndAttackBuff( ioBuff *pBuff )
{
	if( !pBuff || pBuff->GetTypeID() != BT_CHECK_STATE_END_ATTACK )
		return NULL;

	return dynamic_cast< ioCheckStateEndAttackBuff* >( pBuff );
}