#pragma once

#include "ioDummyChar.h"

class ioPlayStage;
class ioPlayMode;
class ioINILoader;
class SP2Packet;

class ioDummyMine : public ioDummyChar
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
	//���� �ʱ�ȭ
	bool m_bStartInit;
	//���̴� ����
	float m_fVisibleRange;
	bool m_bMineVisible;

	//�浹 ����
	float m_fColRange_Front;
	float m_fColRange_Back;
	//�浹 ����
	float m_fColAngle_Front;
	float m_fColAngle_Back;
	//�浹 ����
	float m_fColUpHeight_Front;
	float m_fColUpHeight_Back;
	//�浹 �Ʒ� ����
	float m_fColUnderHeight_Front;
	float m_fColUnderHeight_Back;
	//����
	D3DXVECTOR3 m_vFrontDir;
	D3DXVECTOR3 m_vBackDir;
	D3DXVECTOR3 m_vCreateWeaponOffset;
	//���� ��ȣ
	int m_nCreateWeaponIndex_Front;
	int m_nCreateWeaponIndex_Back;
	//�����̾� ��Ʈ Ȯ��
	int m_nConveyerMoveFrameIdx;

	DWORD m_dwSensingTime;
	DWORD m_dwCheckSensingTime;

	DWORD m_dwCheckEndDelayTime;
	DWORD m_dwEndDelayTime;

	ioHashString m_szExplosionSound;

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
	ioDummyMine( ioEntityGroup *pGrp, ioPlayMode *pMode );
	virtual ~ioDummyMine();
};

inline ioDummyMine* ToDummyMine( ioDummyChar *pDummy )
{
	if( !pDummy || pDummy->GetType() != DCT_MINE )
		return NULL;

	return dynamic_cast< ioDummyMine* >( pDummy );
}