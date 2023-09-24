#pragma once

#include "AccessoryEnumType.h"

class ioUserAccessory
{
public:
	enum
	{
		MAX_SLOT_CNT = 500,
	};
protected:
	AccessorySlotList m_AccessorySlotList;
public:
	//����ȭ
	void ApplyAccessorySlotData( SP2Packet &rkPacket );

public:
	//���� �߰�
	void AddAccessorySlotData( const AccessorySlot &sNewSlot );
	//���� ����
	bool DeleteAccessorySlotData( int iSlotIndex );
	//���� ����
	void SetAccessorySlot( const AccessorySlot &sAccessorySlot );

public:
	//���� ����
	void SetAccessoryEquip( int iSlotIndex, bool bEquip );

public:
	//���� ���(���� �ε���)
	bool GetAccessorySlot( int iSlotIndex, AccessorySlot &sAccessorySlot );
	//���� ���(���� �迭)
	bool GetAccessorySlotArray( int iArray, AccessorySlot &sAccessorySlot );
	//���� ���� ���
	int GetAccessorySlotCnt();
	//���� ���� Ȯ��
	bool IsSlotFull();

	int GetAccessoryCode( int iSlotIndex );
	void GetUseInfo( int iSlotIndex, ioHashString &rsInfo );
public:
	ioUserAccessory(void);
	~ioUserAccessory(void);
};

