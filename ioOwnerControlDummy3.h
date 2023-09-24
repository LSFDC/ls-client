#pragma once

#include "ioDummyChar.h"

class ioPlayStage;
class ioPlayMode;
class ioINILoader;
class SP2Packet;

class ioOwnerControlDummy3 : public ioDummyChar
{	
public:
	enum DummyState
	{
		DS_Rotate,
		DS_Height,
	};
	
protected:		
	typedef std::vector<int> CreateWeaponList;

	//���� �ʱ�ȭ
	bool m_bStartInit;
	//ȸ��
	bool m_bTargetRot;
	bool m_bLeftRot;
	ioUserKeyInput::DirKeyInput m_CurDirKey;
	//ȸ�� �ð�
	DWORD m_dwRotateTime;
	//���� ������
	D3DXVECTOR3 m_vCreateWeaponOffset;
	//���� �ε��� ��ȣ
	int m_nCreateWeaponIndex;
	//������ ���� �ε��� ��ȣ
	CreateWeaponList m_CreateWeaponList;
	//����� �ð�
	DWORD m_dwCreateWeaponTime;
	DWORD m_dwCheckWeaponTime;
	//���� ����
	float m_fCreateWeaponDegree;
	//������ ���� �ε���
	int m_nLastAttackIndex;
	//���� ���� ����
	bool m_bSetHeightAdjust;

public:
	virtual void LoadProperty( ioINILoader &rkLoader );

protected:
	virtual void ProcessState( float fTimerPerSec, DWORD dwPreTime );
	virtual void ProcessMoveState( float fTimePerSec );

	void ProcessStartState( float fTimePerSec );
	void SetMoveState();	

public:
	virtual void ApplyDummyCharSync( ioPlayStage *pStage, SP2Packet &rkPacket );
	virtual void SetDieState( bool bEffect );
	virtual bool IsActivateWeapon();
	virtual ApplyWeaponExp ApplyWeapon( ioWeapon *pWeapon );

public:
	virtual void CheckCollisionWeapon( ioBaseChar *pChar );

protected:
	//������ �б�
	void LoadInitProperty( ioINILoader &rkLoader );
	//���� ����
	void CreateWeapon( int nAttackIndex, const D3DXVECTOR3 &vPos, const D3DXVECTOR3 &vDir );
	//���� �߻�
	void FireWeapon();
	//������ ����
	void LastFireWeapon();
	//���� ����
	void RemoveWeapon();
	//�ڵ� �̵�
	void AutoMove( float fTimePerSec );
	//Ű�Է�
	void CheckInputKey( float fTimePerSec );
	//ȸ��
	void ProcessRotate();

public:
	ioOwnerControlDummy3( ioEntityGroup *pGrp, ioPlayMode *pMode );
	virtual ~ioOwnerControlDummy3();
};

inline ioOwnerControlDummy3* ToOwnerControlDummy3( ioDummyChar *pDummy )
{
	if( !pDummy || pDummy->GetType() != DCT_OWNER_CONTROL3 )
		return NULL;

	return dynamic_cast< ioOwnerControlDummy3* >( pDummy );
}