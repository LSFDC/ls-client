#pragma once

#include "ioDummyChar.h"

class ioPlayStage;
class ioPlayMode;
class ioINILoader;
class SP2Packet;

class ioDummyTurret2 : public ioDummyChar
{	
public:
	enum DummyTurretState
	{
		DMS_START,
		DMS_SENSING,
		DMS_FIRE,
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

	int m_nMaxBullet;
	int m_nCurBullet;

	//ioHashStringVec m_FiredTargetList;

	//�̻��� ���� ������
	float m_fFireHeightOffset;

	DWORD m_dwFireTicTime;
	DWORD m_dwNextFireTime;

	bool m_bUseAirSetting;
	float m_fAirHeightOffSet;

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
	virtual void InitDummyCharInfo(	int iIndex, DWORD dwCreateGapTime, const D3DXVECTOR3 &vPos, const D3DXVECTOR3 &vScale,float fStartAngle=0.0f, bool bNetWork = false );

protected:
	//������ �б�
	void LoadInitProperty( ioINILoader &rkLoader );
	//���� ����
	void CreateWeapon( const D3DXVECTOR3 &vPos, const D3DXVECTOR3 &vDir, ioBaseChar *pTarget );
	//Ÿ�� Ȯ��
	void CheckTarget( ioBaseChar *pOwner );


public:
	ioDummyTurret2( ioEntityGroup *pGrp, ioPlayMode *pMode );
	virtual ~ioDummyTurret2();
};

inline ioDummyTurret2* ToDummyTurret2( ioDummyChar *pDummy )
{
	if( !pDummy || pDummy->GetType() != DCT_TURRET2 )
		return NULL;

	return dynamic_cast< ioDummyTurret2* >( pDummy );
}