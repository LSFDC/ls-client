#pragma once

#include "ioBuff.h"

class ioBaseChar;
class ioINILoader;

class ioPositionBuff : public ioBuff
{
protected:
	bool m_bDisableDownState;
	bool m_bUseWeaponEquipMesh;
	CEncrypt<bool> m_bEscapeCatchState;

	ioHashString m_DummyCharName;

	//��� ������ �� ������ ���� �׿� �ּ���~
	//���� ���� ioBuff������ �ű���.
	ioHashString m_stEraseBuff;

	int m_nFirstDummyID;

	bool m_bEraseGravity;

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
	ioPositionBuff();
	ioPositionBuff( const ioPositionBuff &rhs );
	virtual ~ioPositionBuff();
};


inline ioPositionBuff* ToPositionBuff( ioBuff *pBuff )
{
	if( !pBuff || pBuff->GetTypeID() != BT_POSITION )
		return NULL;

	return dynamic_cast< ioPositionBuff* >( pBuff );
}


