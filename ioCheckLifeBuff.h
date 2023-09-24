#pragma once

#include "ioBuff.h"

class ioBaseChar;
class ioINILoader;

class ioCheckLifeBuff : public ioBuff
{
protected:
	//bool m_bDisableDownState;
	//bool m_bUseWeaponEquipMesh;
	//CEncrypt<bool> m_bEscapeCatchState;

	//��� ������ �� ������ ���� �׿� �ּ���~
	//���� ���� ioBuff������ �ű���.
	//ioHashString m_stEraseBuff;

	//int m_nFirstDummyID;

	ioHashString m_stCheckBuff;

public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ioBuff* Clone();

public:
	virtual void StartBuff( ioBaseChar *pOwner );
	virtual bool ApplyStartBuff( ioBaseChar *pOwner, DWORD dwGlobalTime, DWORD dwGapTime );
	virtual void ProcessBuff( float fTimePerSec );
	virtual void EndBuff();

protected:
	virtual void SetAction();

public:
	ioCheckLifeBuff();
	ioCheckLifeBuff( const ioCheckLifeBuff &rhs );
	virtual ~ioCheckLifeBuff();
};


inline ioCheckLifeBuff* ToCheckLifeBuff( ioBuff *pBuff )
{
	if( !pBuff || pBuff->GetTypeID() != BT_CHECK_LIFE )
		return NULL;

	return dynamic_cast< ioCheckLifeBuff* >( pBuff );
}


