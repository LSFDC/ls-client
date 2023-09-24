#pragma once

#include "ioBuff.h"

class ioBaseChar;
class ioINILoader;
class ioEffect;

class ioCheckChainBuff : public ioBuff
{
public:
	enum BuffState
	{
		BS_CreateChain,
		BS_CreateWeapon,
		BS_AutoAttack,
		BS_START,
		BS_WAIT,
		BS_EXPLOSION
	};

	struct Chain
	{
		int nWeaponIndex;
		bool bSetVisible;
		ioHashString szTargetName;
	};

	typedef std::vector<Chain> ChainList;

	struct DistInfo
	{
		ioHashString szName;
		float fDistSq;
		D3DXVECTOR3 vMidPos;

		bool operator<( const DistInfo &rhs ) const
		{
			return fDistSq < rhs.fDistSq;
		}
	};

	typedef std::vector< DistInfo > SortList;

protected:
	ChainList m_ChainList;
	SortList m_SortList;

	BuffState m_BuffState;
	//�浹 ����
	float m_fColRange;
	//�浹 ����
	float m_fColAngle;
	//Ȯ�� ����
	ioHashString m_szCheckBuffName;
	//������ ����
	int m_nChainWeapon;
	int m_nAttackWeapon;
	//�� ���� ����
	int m_nTotalChainCnt;
	//���� �ε���
	DWORD m_dwWeaponBaseIndex;
	//Ű�Է� ����
	bool m_bSetEnableInputKey;
	//���� �ִϸ��̼�
	ioHashString m_szAttackAnimation;
	float m_fAttackTimeRate;
	//���� ���� �ð�
	DWORD m_dwFireTime;
	//���� Ȯ��
	bool m_bEnableAttack;
	//�ڵ� ���� Ȯ��
	DWORD m_dwCheckAutoTime;
	DWORD m_dwAutoTime;
	
public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ioBuff* Clone();

public:
	virtual void StartBuff( ioBaseChar *pOwner );
	virtual bool ApplyStartBuff( ioBaseChar *pOwner, DWORD dwGlobalTime, DWORD dwGapTime );
	virtual void ProcessBuff( float fTimePerSec );
	virtual void EndBuff();
	virtual void ApplyExtraBuffInfo( SP2Packet &rkPacket );

protected:
	//ü�� ����
	void CreateChain( ioBaseChar *pOwner );
	//Ÿ�� Ȯ��
	void CheckTarget( ioBaseChar *pOwner );
	//Ÿ�� ����Ʈ ����
	void SortTargetList( ioBaseChar *pOwner, const BaseCharList &TargetList );
	//ü�� ����
	void SetChain( ioBaseChar *pOwner );
	//Ÿ�� �������� Ȯ��
	bool IsEnableTarget( ioBaseChar *pTarget );
	//�ڵ� ���� Ȯ��
	void CheckAutoAttack( ioBaseChar *pOwner );
	//ĳ���� ���� Ȯ��
	void CheckCharState( ioBaseChar *pOwner );
	//���� ������ �������� Ȯ��
	bool IsEnableAttack( ioBaseChar *pOwner );
	//���� �ִϸ��̼� ����
	void SetAttackTime( ioBaseChar *pOwner, ioHashString szName, float fRate );
	//���� ���� Ȯ��
	void CheckCreateWeapon( ioBaseChar *pOwner );
	//ü�� ����� Ÿ�ٸ� ���� ����
	void RemoveCheckBuff( ioBaseChar *pOwner );
	//����Ʈ ����
	void ClearList( ioBaseChar *pOwner );

public:
	ioCheckChainBuff();
	ioCheckChainBuff( const ioCheckChainBuff &rhs );
	virtual ~ioCheckChainBuff();
};

inline ioCheckChainBuff* ToCheckChainBuff( ioBuff *pBuff )
{
	if( !pBuff || pBuff->GetTypeID() != BT_CHECK_CHAIN )
		return NULL;

	return dynamic_cast< ioCheckChainBuff* >( pBuff );
}