
#include "stdafx.h"

#include "ioInventory.h"

#include "ioDecorationPrice.h"

ioInventory::ioInventory()
{
	Initialize();
}

ioInventory::~ioInventory()
{
	
}

void ioInventory::Initialize()
{
	m_vItemList.clear();
}

void ioInventory::ApplyInventorySlotData( SP2Packet &rkPacket )
{
	int iSize;
	rkPacket >> iSize;
	for(int i = 0;i < iSize;i++)
	{
		ITEMDB kItemDB;
		for(int j = 0;j < MAX_SLOT;j++)
		{
			rkPacket >> kItemDB.m_Slot[j].m_item_type >> kItemDB.m_Slot[j].m_item_code >> kItemDB.m_Slot[j].m_item_equip;
		}
		m_vItemList.push_back( kItemDB );
	}
}

bool ioInventory::IsSlotItem( int iType, int iCode )
{
	if( m_vItemList.empty() ) 
		return false;

	
	if( iType%1000 == UID_KINDRED )
	{
		// ���� ġ��
		// ������ �������� �ʴ´�.
		if( iCode == RDT_HUMAN_MAN ) return true;        //�ΰ� ���ڴ� �⺻

		vITEMDB::iterator iter;
		for(iter = m_vItemList.begin();iter != m_vItemList.end();iter++)
		{
			ITEMDB &kItemDB = *iter;
			for(int i = 0;i < MAX_SLOT;i++)
			{
				if( ( kItemDB.m_Slot[i].m_item_type / 100000 ) != ( iType / 100000 ) ) continue;
				if( kItemDB.m_Slot[i].m_item_type % 1000 != UID_KINDRED ) continue;

				if( kItemDB.m_Slot[i].m_item_code == iCode )
					return true;
			}		
		}
	}
	else
	{
		// �Ϲ� ġ���� Ŭ���� + ���� + ġ��Ÿ���� �����ؾ���
		vITEMDB::iterator iter;
		for(iter = m_vItemList.begin();iter != m_vItemList.end();iter++)
		{
			ITEMDB &kItemDB = *iter;
			for(int i = 0;i < MAX_SLOT;i++)
			{
				if( kItemDB.m_Slot[i].m_item_type == iType &&
					kItemDB.m_Slot[i].m_item_code == iCode )
				{
					return true;
				}
			}		
		}
	}
	return false;
}

void ioInventory::AddSlotItem( int iType, int iCode )
{
	vITEMDB::iterator iter;
	for(iter = m_vItemList.begin();iter != m_vItemList.end();iter++)
	{
		ITEMDB &kItemDB = *iter;
		for(int i = 0;i < MAX_SLOT;i++)
		{
			if( kItemDB.m_Slot[i].m_item_type == 0 &&
				kItemDB.m_Slot[i].m_item_code == 0 )
			{
				kItemDB.m_Slot[i].m_item_type = iType;
				kItemDB.m_Slot[i].m_item_code = iCode;
				return;
			}
		}		
	}

	// New
	ITEMDB kItemDB;
	kItemDB.m_Slot[0].m_item_type = iType;
	kItemDB.m_Slot[0].m_item_code = iCode;
	m_vItemList.push_back( kItemDB );
}

void ioInventory::SetEquipItem( const int iItemType, const int iItemCode )
{
	vITEMDB::iterator iter, iEnd;
	iEnd = m_vItemList.end();
	for(iter = m_vItemList.begin();iter != iEnd;iter++)
	{
		ITEMDB &kItemDB = *iter;
		for(int i = 0;i < MAX_SLOT;i++)
		{
			// ���� ġ�� 
			if( iItemType % 1000 == UID_KINDRED )
			{
				if( iItemType / 100000 != kItemDB.m_Slot[i].m_item_type / 100000 ) continue;
				if( UID_KINDRED != kItemDB.m_Slot[i].m_item_type % 1000 ) continue;

				if( kItemDB.m_Slot[i].m_item_code == iItemCode)
				{
					kItemDB.m_Slot[i].m_item_equip = 1;
				}
				else
				{
					kItemDB.m_Slot[i].m_item_equip = 0;
				}
			}
			else      //�Ϲ� ġ��
			{
				if( kItemDB.m_Slot[i].m_item_type == iItemType && kItemDB.m_Slot[i].m_item_code == iItemCode)
				{
					kItemDB.m_Slot[i].m_item_equip = 1;
				}
				else if( kItemDB.m_Slot[i].m_item_type == iItemType )
				{
					kItemDB.m_Slot[i].m_item_equip = 0;
				}
			}
		}		
	}
}

RaceDetailType ioInventory::GetEquipRaceType( const int iClassType )
{
	vITEMDB::iterator iter, iEnd;
	iEnd = m_vItemList.end();
	for(iter = m_vItemList.begin();iter != iEnd;iter++)
	{
		ITEMDB &kItemDB = *iter;
		for(int i = 0;i < MAX_SLOT;i++)
		{
			if( kItemDB.m_Slot[i].m_item_equip == 0 ) continue;
			if( kItemDB.m_Slot[i].m_item_type / 100000 != iClassType ) continue;
			if( kItemDB.m_Slot[i].m_item_type % 1000 != UID_KINDRED ) continue;

			return (RaceDetailType)kItemDB.m_Slot[i].m_item_code;
		}		
	}
	return RDT_HUMAN_MAN;        // �ΰ� ���ڴ� �κ��丮�� �����Ƿ� �⺻���̴�.
}

void ioInventory::GetEquipItemCode( CHARACTER &charInfo )
{
	RaceDetailType eRaceType = GetEquipRaceType( charInfo.m_class_type );
	switch( eRaceType )
	{
	case RDT_HUMAN_MAN:
		charInfo.m_kindred = 1;
		charInfo.m_sex = 1;
		break;
	case RDT_HUMAN_WOMAN:
		charInfo.m_kindred = 1;
		charInfo.m_sex = 2;
		break;
	case RDT_ELF_MAN:
		charInfo.m_kindred = 2;
		charInfo.m_sex = 1;
		break;
	case RDT_ELF_WOMAN:
		charInfo.m_kindred = 2;
		charInfo.m_sex = 2;
		break;
	case RDT_DWARF_MAN:
		charInfo.m_kindred = 3;
		charInfo.m_sex = 1;
		break;
	case RDT_DWARF_WOMAN:
		charInfo.m_kindred = 3;
		charInfo.m_sex = 2;
		break;
	}

	// ��       UID_FACE
	charInfo.m_face = GetEquipItemCode( ( charInfo.m_class_type * 100000 ) + ( ( charInfo.m_sex - 1) * 1000 ) + UID_FACE );
	// �Ӹ�       UID_HAIR  
	charInfo.m_hair = GetEquipItemCode( ( charInfo.m_class_type * 100000 ) + ( ( charInfo.m_sex - 1) * 1000 ) + UID_HAIR );
	// �Ǻλ�     UID_SKIN_COLOR
	charInfo.m_skin_color = GetEquipItemCode( ( charInfo.m_class_type * 100000 ) + ( ( charInfo.m_sex - 1) * 1000 ) + UID_SKIN_COLOR );
	// �Ӹ���     UID_HAIR_COLOR
	charInfo.m_hair_color = GetEquipItemCode( ( charInfo.m_class_type * 100000 ) + ( ( charInfo.m_sex - 1) * 1000 ) + UID_HAIR_COLOR );
	// �ӿ�       UID_UNDERWEAR
	charInfo.m_underwear = GetEquipItemCode( ( charInfo.m_class_type * 100000 ) + ( ( charInfo.m_sex - 1) * 1000 ) + UID_UNDERWEAR );
}

int ioInventory::GetEquipItemCode( const int iItemType )
{
	vITEMDB::iterator iter, iEnd;
	iEnd = m_vItemList.end();
	for(iter = m_vItemList.begin();iter != iEnd;iter++)
	{
		ITEMDB &kItemDB = *iter;
		for(int i = 0;i < MAX_SLOT;i++)
		{
			if( kItemDB.m_Slot[i].m_item_type  == iItemType &&
				kItemDB.m_Slot[i].m_item_equip ==  1 ) // ���� 
			{
				return ( kItemDB.m_Slot[i].m_item_code );				
			}
		}		
	}

	return -1;
}

bool ioInventory::IsClassTypeInventory( int iClassType )
{
	// �ش� �뺴�� ġ�� �������� ������ true. : �̰��� ���� ġ���� ���Կ� �����Ƿ� ���� 1ȸ�� �������� ���� �뺴�� �˼��ִ�.
	vITEMDB::iterator iter, iEnd;
	iEnd = m_vItemList.end();
	for(iter = m_vItemList.begin();iter != iEnd;iter++)
	{
		ITEMDB &kItemDB = *iter;
		for(int i = 0;i < MAX_SLOT;i++)
		{
			int iItemClass = kItemDB.m_Slot[i].m_item_type / 100000;
			if( iItemClass == iClassType )
				return true;
		}		
	}
	return false;
}

bool ioInventory::IsClassTypeInventory( int iClassType, int iSexType, int iDecoType )
{
	// �ش� �뺴�� ġ�� �������� ������ true. : �̰��� ���� ġ���� ���Կ� �����Ƿ� ���� 1ȸ�� �������� ���� �뺴�� �˼��ִ�.
	int iItemType = ( iClassType * 100000 ) + ( iSexType * 1000 ) + iDecoType;

	vITEMDB::iterator iter, iEnd;
	iEnd = m_vItemList.end();
	for(iter = m_vItemList.begin();iter != iEnd;iter++)
	{
		ITEMDB &kItemDB = *iter;
		for(int i = 0;i < MAX_SLOT;i++)
		{
			if( kItemDB.m_Slot[i].m_item_type == iItemType )
				return true;
		}		
	}
	return false;
}

int  ioInventory::GetInventoryItemCnt()
{
	int iItemCnt = 0;
	vITEMDB::iterator iter, iEnd;
	iEnd = m_vItemList.end();
	for(iter = m_vItemList.begin();iter != iEnd;iter++)
	{
		ITEMDB &kItemDB = *iter;
		for(int i = 0;i < MAX_SLOT;i++)
		{
			if( kItemDB.m_Slot[i].m_item_type == 0 ) continue;

			iItemCnt++;
		}		
	}

	return iItemCnt;
}

bool ioInventory::IsInventoryItemFull()
{
	if( GetInventoryItemCnt() > MAX_SLOT_CNT )
		return true;
	return false;
}