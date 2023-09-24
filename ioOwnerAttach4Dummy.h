#pragma once

#include "ioDummyChar.h"

class ioPlayStage;
class ioPlayMode;
class ioINILoader;
class SP2Packet;

class ioOwnerAttach4Dummy : public ioDummyChar
{	
public:
	//������ ������ ����
	struct CreateWeaponInfo
	{
		//���� �ε��� ��ȣ
		int nAttackIndex;
		//������
		D3DXVECTOR3 vOffsetPos;
		//����
		float fDegree;
		//��߻� �ð�
		DWORD dwReloadTime;
		DWORD dwCheckTime;
		//�ִ� �߻� ����
		int nMaxFireCnt;
		int nCurFireCnt;
	};

	typedef std::vector<CreateWeaponInfo> CreateWeaponInfoList;

protected:	
	//���� ���� ����Ʈ
	CreateWeaponInfoList m_CreateWeaponList;

	//��ǥ�� ��ġ
	D3DXVECTOR3 m_vTargetPos;
	//�̵� ����
	D3DXVECTOR3 m_vMoveDir;
	//�� ��ĭ�ΰ�
	bool m_bIsHead;
	//���� �ʱ�ȭ
	bool m_bStartInit;
	//Ȯ�� �� ����
	ioHashString m_szCheckBuff;
	//�̵� �� ���ΰ� ����
	bool m_bSetDontMove;
	//�߻� ����
	bool m_bSetFire;
	//�ִ� ���� ����
	int m_nMaxSplitCnt;
	//���� ���� ����
	int m_nWeaponSplitCnt;
	//���� ���� �ε���
	int m_nCurSplitIndex;
	//�߻� �Ϸ� Ȯ�� ��
	int m_nFinshCnt;

public:
	virtual void LoadProperty( ioINILoader &rkLoader );

protected:
	virtual void ProcessState( float fTimerPerSec, DWORD dwPreTime );
	virtual void ProcessDelayState( float fTimePerSec );
	virtual void ProcessMoveState( float fTimePerSec );

	void ProcessStartState( float fTimePerSec );
	void SetDelayState();	
	void SetFollowState();	
	bool CheckOwnerBuff();

public:
	virtual void ApplyDummyCharSync( ioPlayStage *pStage, SP2Packet &rkPacket );
	virtual void SetDieState( bool bEffect );
	virtual bool IsActivateWeapon();
	virtual ApplyWeaponExp ApplyWeapon( ioWeapon *pWeapon );
	
protected:
	//������ �б�
	void LoadInitProperty( ioINILoader &rkLoader );
	//���� �̵�
	void DummyMove( float fTimePerSec );
	//���� ����
	void CreateWeapon( int nAttackIndex, const D3DXVECTOR3 &vPos, const D3DXVECTOR3 &vDir );
	//���� �߻�
	void FireWeapon();

public:
	//Ÿ�� ��ġ ����
	void SetTargetPos( D3DXVECTOR3 vPos);
	//��ĭ ����, Ȯ��
	void SetHead( bool bHead );
	bool GetHead();
	//�̵� ����
	void SetDontMove( bool bDontMove );
	//�߻� ����
	void SetFire( bool bFire );
	
public:
	ioOwnerAttach4Dummy( ioEntityGroup *pGrp, ioPlayMode *pMode );
	virtual ~ioOwnerAttach4Dummy();
};

inline ioOwnerAttach4Dummy* ToOwnerAttach4Dummy( ioDummyChar *pDummy )
{
	if( !pDummy || pDummy->GetType() != DCT_OWNER_ATTACH4 )
		return NULL;

	return dynamic_cast< ioOwnerAttach4Dummy* >( pDummy );
}