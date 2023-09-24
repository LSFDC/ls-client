#pragma once

#include "ioDummyChar.h"

class ioPlayStage;
class ioPlayMode;
class ioINILoader;
class SP2Packet;

class ioDummyTurret : public ioDummyChar
{	
public:
	enum DummyTurretState
	{
		DMS_START,
		DMS_SENSING,
		DMS_END,
	};

protected:		
	DummyTurretState m_TurretState;
	//���� �ʱ�ȭ
	bool m_bStartInit;

	//�浹 ����
	//�ش� �Ÿ��� �ִ� Ÿ���� ã��
	float m_fMaxSearchingRange;
	//�������� ��Ÿ �ش� �Ÿ� �̻��ΰ� �ɷ� ����.
	float m_fFireLandMinRange;

	DWORD m_nCreateWeaponIndex;

	ioHashString m_szExplosionSound;

	ioHashStringVec m_FiredTargetList;

	//�̻��� ���� ������
	float m_fFireHeightOffset;

public:
	virtual void LoadProperty( ioINILoader &rkLoader );

protected:
	virtual void ProcessState( float fTimerPerSec, DWORD dwPreTime );

	void ProcessStartState( float fTimePerSec );
	void ProcessSensingState( float fTimePerSec );

	void SetStart();

public:
	virtual void ApplyDummyCharSync( ioPlayStage *pStage, SP2Packet &rkPacket );
	virtual void SetDieState( bool bEffect );
	virtual bool IsActivateWeapon();
	virtual ApplyWeaponExp ApplyWeapon( ioWeapon *pWeapon );
	virtual void TranslateByConveyer( const D3DXVECTOR3 &vMove );

protected:
	//������ �б�
	void LoadInitProperty( ioINILoader &rkLoader );
	//���� ����
	void CreateWeapon( int nAttackIndex, const D3DXVECTOR3 &vPos, const D3DXVECTOR3 &vDir, ioBaseChar *pTarget );
	//Ÿ�� Ȯ��
	void CheckTarget( ioBaseChar *pOwner );


public:
	ioDummyTurret( ioEntityGroup *pGrp, ioPlayMode *pMode );
	virtual ~ioDummyTurret();
};

inline ioDummyTurret* ToDummyTurret( ioDummyChar *pDummy )
{
	if( !pDummy || pDummy->GetType() != DCT_TURRET )
		return NULL;

	return dynamic_cast< ioDummyTurret* >( pDummy );
}