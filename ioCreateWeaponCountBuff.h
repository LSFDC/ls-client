#pragma once

#include "ioBuff.h"

class ioBaseChar;
class ioINILoader;

class ioCreateWeaponCountBuff : public ioBuff
{
public:
	enum BuffState
	{
		BS_DELAY,
		BS_FIRE,
	};

	//�߰�//////////////////////////
	enum CreateWeaponByType
	{
		CWBT_NORMAL,
		CWBT_COUNT
	};
	///////////////////////////////

protected:
	BuffState m_BuffState;

	bool m_bDisableDownState;

	CEncrypt<DWORD> m_dwFireGapTime;
	DWORD m_dwCheckFireTime;

	ioHashString m_szDelayEffect;
	DWORD m_dwDelayEffectID;
	ioHashString m_szFireEffect;

	DWORD m_dwCheckGavityTime;
	CEncrypt<DWORD> m_dwGravityTime; 
	CEncrypt<float> m_fGravityRate;

	WeaponInfo m_WeaponInfo;

	ioHashString m_szFireSound;

	//�߰� ����////////////////////////////////////////////////////////
protected:
	//���� ���� Ÿ����?
	CreateWeaponByType m_CreateWeaponType;
	//�� ���� ���� Ƚ��
	int m_nTotalWeaponCnt;
	//���� ���� ��� Ƚ��
	int m_nCurWeaponCnt;
	///////////////////////////////////////////////////////////////////

public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ioBuff* Clone();

public:
	virtual void StartBuff( ioBaseChar *pOwner );
	virtual bool ApplyStartBuff( ioBaseChar *pOwner, DWORD dwGlobalTime, DWORD dwGapTime );
	virtual void ProcessBuff( float fTimePerSec );
	virtual void EndBuff();
	virtual void GetBuffInfo( SP2Packet &rkPacket );
	virtual void ApplyBuffInfo( SP2Packet &rkPacket );

protected:
	bool CheckOwnerStateCheck();
	bool CheckRestoreStateCheck();
	void ProcessKeyInput();
	void ProcessState();
	void CreateWeapon();

public:
	void ApplyExtraBuffInfo( SP2Packet &rkPacket );

	//�߰��Լ�////////////////////////////////////
public:
	//�߰� �б�
	void LoadWeaponCreateProperty( ioINILoader &rkLoader );
	//��ų������ ����
	void SetSkillGauge( float fSkillGauge );
	/////////////////////////////////////////////////////////////

public:
	ioCreateWeaponCountBuff();
	ioCreateWeaponCountBuff( const ioCreateWeaponCountBuff &rhs );
	virtual ~ioCreateWeaponCountBuff();
};


inline ioCreateWeaponCountBuff* ToNewCreateWeaponBuff( ioBuff *pBuff )
{
	if( !pBuff || pBuff->GetTypeID() != BT_CREATE_WEAPON_COUNT )
		return NULL;

	return dynamic_cast< ioCreateWeaponCountBuff* >( pBuff );
}