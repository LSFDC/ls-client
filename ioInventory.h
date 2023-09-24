#pragma once

#include "CharacterInfo.h"

class SP2Packet;

class ioInventory
{
public:
	enum
	{
		MAX_SLOT         = 20,	
		//13.12.10 ���� ( ���� �̵� �� ���������� ���� )
		//MAX_SLOT_CNT	 = 4000,             // �ִ� 4,000�� ���� ����
		MAX_SLOT_CNT	 = 10000,             // �ִ� 3,000�� ���� ����
	};

protected:
	struct ITEMDB
	{
		ITEMSLOT m_Slot[MAX_SLOT];		
		ITEMDB()
		{
			memset( m_Slot, 0, sizeof( m_Slot ) );
		}
	};
	typedef std::vector< ITEMDB > vITEMDB;
	vITEMDB m_vItemList;

public:
	void Initialize();

public:
	void ApplyInventorySlotData( SP2Packet &rkPacket );

public:
	bool IsSlotItem( int iType, int iCode );
	void AddSlotItem( int iType, int iCode );

public:
	void SetEquipItem( const int iItemType, const int iItemCode  );

public:
	RaceDetailType GetEquipRaceType( const int iClassType );
	void GetEquipItemCode( CHARACTER &charInfo );
	int GetEquipItemCode( const int iItemType );

public:
	bool IsClassTypeInventory( int iClassType );
	bool IsClassTypeInventory( int iClassType, int iSexType, int iDecoType );

public:
	int  GetInventoryItemCnt();
	bool IsInventoryItemFull();

public:
	ioInventory();
	virtual ~ioInventory();
};
