#pragma once

#include "ioDummyChar.h"

class ioPlayStage;
class ioPlayMode;
class ioINILoader;
class SP2Packet;

class ioDummyWarpMine : public ioDummyChar
{	
public:
	enum DummyMineState
	{
		DMS_START,
		DMS_SENSING,
		DMS_EXPLOSION,
		DMS_END,
	};
	
protected:		
	DummyMineState m_MineState;
	//���̴� ����
	float m_fVisibleRange;
	bool m_bMineVisible;

	//�浹 ����
	float m_fColRange;
	//�浹 ����
	float m_fColAngle;
	//�浹 ����
	float m_fColUpHeight;
	//�浹 �Ʒ� ����
	float m_fColUnderHeight;
	//����
	D3DXVECTOR3 m_vExplosionWeaponOffset;
	//���� ��ȣ
	int m_nExplosionWeaponIndex;

	DWORD m_dwSensingTime;
	DWORD m_dwCheckSensingTime;
	DWORD m_dwAutoExplosionTime;

	DWORD m_dwCheckEndDelayTime;
	DWORD m_dwEndDelayTime;

	ioHashString m_szExplosionSound;

	bool m_bSetVisible;
	bool m_bSetCheckTarget;
	bool m_bSetAutoExplosion;

	ioHashString m_szAreaWeaponName;

	ioHashString m_szAttachEffect;

	DWORD m_dwAreaWeaponIdx;
	
public:
	virtual void LoadProperty( ioINILoader &rkLoader );

protected:
	virtual void ProcessState( float fTimerPerSec, DWORD dwPreTime );

	void ProcessStartState( float fTimePerSec );
	void ProcessSensingState( float fTimePerSec );
	void ProcessExplosionState( float fTimePerSec );

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
	void CreateWeapon( int nAttackIndex, const D3DXVECTOR3 &vPos, const D3DXVECTOR3 &vDir );
	//�����
	void VisibleCheck( ioBaseChar *pOwner );
	//Ÿ�� Ȯ��
	void CheckTarget( ioBaseChar *pOwner );
	void GetTargetList( ioPlayStage* pStage, const D3DXVECTOR3 &vPos, const D3DXVECTOR3 &vColDir, BaseCharList &vCharList, float fColUpHeight, float fColUnderHeight, float fColRange, float fColAngle );
	//�����, �����̾� ��Ʈ Ȯ��
	void CheckDropZone( ioBaseChar *pOwner );

public:
	void SetExplosionState();

public:
	ioDummyWarpMine( ioEntityGroup *pGrp, ioPlayMode *pMode );
	virtual ~ioDummyWarpMine();
};

inline ioDummyWarpMine* ToDummyWarpMine( ioDummyChar *pDummy )
{
	if( !pDummy || pDummy->GetType() != DCT_WARP_MINE )
		return NULL;

	return dynamic_cast< ioDummyWarpMine* >( pDummy );
}