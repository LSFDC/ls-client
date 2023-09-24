#pragma once

#include "PetEnumType.h"

class ioUserPet
{
public:
	enum
	{
		MAX_SLOT_CNT = 20,
	};

protected:
	PetSlotList m_PetSlotList;

public:
	//����ȭ
	void ApplyPetSlotData( SP2Packet &rkPacket );

public:
	//���� �߰�
	void AddPetSlotData( const PetSlot &sNewSlot );
	//���� ����
	bool DeletePetSlotData( int nSlotIndex );
	//���� ����
	void SetPetSlot( const PetSlot &sPetSlot );

public:
	//���� ����
	void SetPetEquip( int nSlotIndex, bool bEquip );
	//���� ����
	void ClearPetEquip();

public:
	//���� ���(���� �ε���)
	bool GetPetSlot( int nSlotIndex, PetSlot &sPetSlot );
	//���� ���(���� �迭)
	bool GetPetSlotArray( int nArray, PetSlot &sPetSlot );
	//���� ���� ���� ���
	bool GetPetSlotEquip( PetSlot &sPetSlot );
	//���� ���� ���
	int GetPetSlotCnt();
	//���� ���� Ȯ��
	bool IsSlotFull();
	//����
	bool SlotSort();

public:
	ioUserPet();
	virtual ~ioUserPet();
};

