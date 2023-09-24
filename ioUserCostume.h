#pragma once

#include "CostumeEnumType.h"

class ioUserCostume
{
public:
	enum
	{
		MAX_SLOT_CNT = 500,
	};

protected:
	CostumeSlotList m_CostumeSlotList;

public:
	//����ȭ
	void ApplyCostumeSlotData( SP2Packet &rkPacket );

public:
	//���� �߰�
	void AddCostumeSlotData( const CostumeSlot &sNewSlot );
	//���� ����
	bool DeleteCostumeSlotData( int nSlotIndex );
	//���� ����
	void SetCostumeSlot( const CostumeSlot &sCostumeSlot );

public:
	//���� ����
	void SetCostumeEquip( int nSlotIndex, bool bEquip );

public:
	//���� ���(���� �ε���)
	bool GetCostumeSlot( int nSlotIndex, CostumeSlot &sCostumeSlot );
	//���� ���(���� �迭)
	bool GetCostumeSlotArray( int nArray, CostumeSlot &sCostumeSlot );
	//���� ���� ���
	int GetCostumeSlotCnt();
	//���� ���� Ȯ��
	bool IsSlotFull();
	//����
	bool SlotSort();

	int GetCostumeCode( int nSlotIndex );
	void GetUseInfo( int nSlotIndex, ioHashString &rsInfo );

public:
	ioUserCostume();
	virtual ~ioUserCostume();
};

