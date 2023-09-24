
#include "stdafx.h"

#include "ioUserFishItem.h"

#include "ioBaseChar.h"

ioUserFishItem::ioUserFishItem()
{
	Initialize();
}

ioUserFishItem::~ioUserFishItem()
{
}

void ioUserFishItem::Initialize()
{
	m_vFishItemList.clear();
}

void ioUserFishItem::ApplyFishItemData( SP2Packet &rkPacket )
{
	m_vFishItemList.clear();

	int iSize;
	rkPacket >> iSize;

	for( int i=0; i < iSize; ++i )
	{
		FishItem kItem;

		rkPacket >> kItem.m_iType;
		rkPacket >> kItem.m_iArray;

		m_vFishItemList.push_back( kItem );
	}

	std::sort( m_vFishItemList.begin(), m_vFishItemList.end(), FishItemSort() );
}

void ioUserFishItem::AddFishItemData( BYTE iType, int iIndexArray )
{
	FishItem kItem;
	kItem.m_iType = iType;
	kItem.m_iArray = iIndexArray;

	m_vFishItemList.push_back( kItem );
}

void ioUserFishItem::DeleteFishItemData( int iIndexArray )
{
	int iSize = m_vFishItemList.size();

	for( int i=0; i < iSize; ++i )
	{
		if( m_vFishItemList[i].m_iArray == iIndexArray )
		{
			m_vFishItemList.erase( m_vFishItemList.begin() + i );
			return;
		}
	}
}

int ioUserFishItem::GetFishItemType( int iListArray )
{
	int iSize = m_vFishItemList.size();
	int iType = -1;

	if( COMPARE( iListArray, 0, iSize ) )
	{
		iType = m_vFishItemList[iListArray].m_iType;
	}

	return iType;
}

int ioUserFishItem::GetFishItemArray( int iListArray )
{
	int iSize = m_vFishItemList.size();
	int iIndexArray = 0;

	if( COMPARE( iListArray, 0, iSize ) )
	{
		iIndexArray = m_vFishItemList[iListArray].m_iArray;
	}

	return iIndexArray;
}

int ioUserFishItem::GetCurFishItemCnt()
{
	int iSize = m_vFishItemList.size();
	
	return iSize;
}


void ioUserFishItem::CheckCurMaxInventory()
{
	m_iCurMaxInventory = DEFAULT_FISH_INVEN;

	// 특별아이템 체크
	int iExtendItem = g_MyInfo.GetFishingSlotExtendItem();
	m_iCurMaxInventory += iExtendItem;

	// 낚시 레벨 체크
	int iLevelAdd = g_MyInfo.GetFishingLevel() / 10;
	m_iCurMaxInventory += iLevelAdd;
}

int ioUserFishItem::GetCurMaxInventory()
{
	CheckCurMaxInventory();

	return m_iCurMaxInventory;
}

