#pragma once

#include "ioDummyChar.h"

class ioPlayStage;
class ioPlayMode;
class ioINILoader;
class SP2Packet;

class ioOwnerAttach5Dummy : public ioDummyChar
{	
public:
	enum AttachState
	{
		AS_None,
		AS_Start,
		AS_Delay,
		AS_Follow,
		AS_Attack,
	};

	struct DistInfo
	{
		ioBaseChar *pChar;
		float fDistSq;

		bool operator<( const DistInfo &rhs ) const
		{
			return fDistSq < rhs.fDistSq;
		}
	};

	typedef std::vector< DistInfo > SortList;

protected:
	D3DXVECTOR3 m_vTargetOffset;
	//Ȯ�� �� ����
	ioHashString m_szCheckBuff;
	
	ioHashString m_szAttackAnimation;

	float m_fAttackAniRate;
	bool m_bSetDownAim;
	//�浹 ����
	float m_fColRange;
	//�浹 ����
	float m_fColAngle;

	DWORD m_dwRefreshTime;
	int m_nAttackWeaponNum;
	D3DXVECTOR3 m_vWeaponOffset;

	ioHashStringVec m_vCheckBuffList;

	float m_fRunMoveSpeed;
	float m_fDashMoveSpeed;

	float m_fFollowRange;
	
protected:	
	AttachState m_AttachState;

	//�̵� ����
	D3DXVECTOR3 m_vMoveDir;
	//���� �̵��� ��ġ
	D3DXVECTOR3 m_vOffsetPos;
	DWORD m_dwFireStart;
	DWORD m_dwAttackEndTime;

	SortList m_SortList;
	ioHashString m_szTargetName;

	bool m_bAutoFollow;
	bool m_bCreateWeapon;

	DWORD m_dwCheckRefreshTime;

public:
	virtual void LoadProperty( ioINILoader &rkLoader );

protected:
	virtual void ProcessState( float fTimerPerSec, DWORD dwPreTime );

public:
	virtual ApplyWeaponExp ApplyWeapon( ioWeapon *pWeapon );
	virtual void ApplyDummyCharSync( ioPlayStage *pStage, SP2Packet &rkPacket );
	virtual void SetDieState( bool bEffect );
	virtual bool IsActivateWeapon();
	
protected:
	//������ �б�
	void LoadInitProperty( ioINILoader &rkLoader );

protected:
	void SetStartState( ioBaseChar *pOwner );
	void ProcessStartState( ioBaseChar *pOwner, float fTimePerSec );
	void SetDelayState( ioBaseChar *pOwner );	
	void ProcessDelayState( ioBaseChar *pOwner, float fTimePerSec );
	void SetFollowState( ioBaseChar *pOwner );	
	void ProcessFollowState( ioBaseChar *pOwner, float fTimePerSec );
	void SetAttackState( ioBaseChar *pOwner );	
	void ProcessAttackState( ioBaseChar *pOwner, float fTimePerSec );

protected:
	bool FindTarget( ioBaseChar *pOwner );
	//Ÿ�� ����Ʈ ����
	void SortTargetList( ioBaseChar *pOwner, const BaseCharList &TargetList );
	//���� ������ �������� Ȯ��
	bool IsEnableTarget( ioBaseChar *pTarget );

protected:
	void SetRotateToTarget( ioBaseChar *pOwner );
	void SetRotateToOwner( ioBaseChar *pOwner );
	void SetOffsetPosAndDir( ioBaseChar *pOwner );
	void FollowOwner( ioBaseChar *pOwner, float fTimePerSec );	
	void CheckInputKey( ioBaseChar *pOwner );
	bool CheckControllRange( ioBaseChar *pOwner );
	bool CheckFollowRange( ioBaseChar *pOwner );
	bool CheckDropZone( ioBaseChar *pOwner );
	void CreateWeapon( ioBaseChar *pOwner );
	D3DXVECTOR3 GetAttackDir( ioBaseChar *pOwner, const D3DXVECTOR3 &vPos );

public:
	ioOwnerAttach5Dummy( ioEntityGroup *pGrp, ioPlayMode *pMode );
	virtual ~ioOwnerAttach5Dummy();
};

inline ioOwnerAttach5Dummy* ToOwnerAttach5Dummy( ioDummyChar *pDummy )
{
	if( !pDummy || pDummy->GetType() != DCT_OWNER_ATTACH5 )
		return NULL;

	return dynamic_cast< ioOwnerAttach5Dummy* >( pDummy );
}