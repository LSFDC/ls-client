
#include "stdafx.h"

#include "ioAlchemicInventory.h"

#include "ioAlchemicMgr.h"
#include "ioBaseChar.h"

#include "GUI/MyInventoryWnd.h"


ioAlchemicInventory::ioAlchemicInventory()
{
	Initialize();
}

ioAlchemicInventory::~ioAlchemicInventory()
{
	m_vAlchemicList.clear();
}

void ioAlchemicInventory::Initialize()
{
	m_vAlchemicList.clear();
}

void ioAlchemicInventory::ApplyAlchemicData( SP2Packet &rkPacket )
{
	m_vAlchemicList.clear();

	int iItemSize;
	rkPacket >> iItemSize;

	for( int i=0; i < iItemSize; ++i )
	{
		AlchemicItem kItem;
		rkPacket >> kItem.m_iCode;
		rkPacket >> kItem.m_iCount;

		m_vAlchemicList.push_back( kItem );
	}
}

void ioAlchemicInventory::ApplyAlchemicSync( SP2Packet &rkPacket )
{
	int iSyncType;
	rkPacket >> iSyncType;

	switch( iSyncType )
	{
	case ALCHEMIC_SYNC_GAIN:
		{
			int iCode, iCount;
			rkPacket >> iCode >> iCount;

			GainAlchemicItem( iCode, iCount );
		}
		break;
	case ALCHEMIC_SYNC_USE:
		break;
	}

	// 조합창 열려있으면 정보갱신 필요
	MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(g_GUIMgr.FindWnd(MY_INVENTORY_WND));
	if( pInvenWnd && pInvenWnd->IsShow() )
	{
		pInvenWnd->UpdateTab( false, true );
	}
}

bool ioAlchemicInventory::FindAlchemicItem( int iCode, AlchemicItem &rkAlchemicItem )
{
	rkAlchemicItem.Init();

	if( m_vAlchemicList.empty() )
		return false;

	vAlchemicItem::iterator iter, iEnd;
	iEnd = m_vAlchemicList.end();
	for(iter = m_vAlchemicList.begin();iter != iEnd;iter++)
	{
		AlchemicItem &kItem = *iter;

		if( kItem.m_iCode == iCode )
		{
			rkAlchemicItem = kItem;
			return true;
		}
	}

	return false;
}

bool ioAlchemicInventory::HasAlchemicItem( int iCode )
{
	if( m_vAlchemicList.empty() )
		return false;

	vAlchemicItem::iterator iter, iEnd;
	iEnd = m_vAlchemicList.end();
	for(iter = m_vAlchemicList.begin();iter != iEnd;iter++)
	{
		AlchemicItem &kItem = *iter;

		if( kItem.m_iCode == iCode )
			return true;
	}

	return false;
}

bool ioAlchemicInventory::AddAlchemicItem( int iCode, int iCount )
{
	vAlchemicItem::iterator iter, iEnd;
	iEnd = m_vAlchemicList.end();
	for(iter = m_vAlchemicList.begin();iter != iEnd;iter++)
	{
		AlchemicItem &kItem = *iter;

		if( kItem.m_iCode == 0 )
		{
			kItem.m_iCode = iCode;
			kItem.m_iCount = iCount;
			kItem.m_iValue1 = 0;
			kItem.m_iValue2 = 0;
			return true;
		}
	}

	AlchemicItem kInfo;
	kInfo.m_iCode = iCode;
	kInfo.m_iCount = iCount;
	kInfo.m_iValue1 = 0;
	kInfo.m_iValue2 = 0;

	m_vAlchemicList.push_back( kInfo );

	return true;
}

bool ioAlchemicInventory::DeleteAlchemicItem( int iCode )
{
	if( m_vAlchemicList.empty() )
		return false;

	vAlchemicItem::iterator iter, iEnd;
	iEnd = m_vAlchemicList.end();
	for(iter = m_vAlchemicList.begin();iter != iEnd;iter++)
	{
		AlchemicItem &kItem = *iter;

		if( kItem.m_iCode == iCode )
		{
			kItem.m_iCode = 0;
			kItem.m_iCount = 0;
			kItem.m_iValue1 = 0;
			kItem.m_iValue2 = 0;
			return true;
		}
	}

	return false;
}

bool ioAlchemicInventory::ChangeAlchemicItem( int iCode, const AlchemicItem &rkAlchemicItem )
{
	vAlchemicItem::iterator iter, iEnd;
	iEnd = m_vAlchemicList.end();
	for(iter = m_vAlchemicList.begin();iter != iEnd;iter++)
	{
		AlchemicItem &kItem = *iter;

		if( kItem.m_iCode == iCode )
		{
			if( rkAlchemicItem.m_iCount == 0 )
			{
				kItem.m_iCode = 0;
				kItem.m_iCount = 0;
				kItem.m_iValue1 = 0;
				kItem.m_iValue2 = 0;
			}
			else
			{
				kItem.m_iCode = rkAlchemicItem.m_iCode;
				kItem.m_iCount = rkAlchemicItem.m_iCount;
				kItem.m_iValue1 = 0;
				kItem.m_iValue2 = 0;
			}
			return true;
		}
	}

	return false;
}

bool ioAlchemicInventory::GainAlchemicItem( int iCode, int iCount )
{
	AlchemicItem kTmpItem;
	if( FindAlchemicItem( iCode, kTmpItem ) )
	{
		kTmpItem.m_iCount += iCount;

		if( kTmpItem.m_iCount <= MAX_SLOT_CNT )
		{
			if( ChangeAlchemicItem( iCode, kTmpItem ) )
			{
				AlchemicItemAddNew( iCode );
				return true;
			}
		}
	}
	else
	{
		if( AddAlchemicItem( iCode, iCount ) )
		{
			AlchemicItemAddNew( iCode );
			return true;
		}
	}

	return false;
}

bool ioAlchemicInventory::UseAlchemicItem( int iCode, int iCount )
{
	if( m_vAlchemicList.empty() )
		return false;

	if( iCount <= 0 )
		return false;

	AlchemicItem kTmpItem;
	if( !FindAlchemicItem( iCode, kTmpItem ) )
		return false;

	kTmpItem.m_iCount -= iCount;

	if( kTmpItem.m_iCount < 0 )
		return false;

	if( ChangeAlchemicItem( iCode, kTmpItem ) )
		return true;

	return false;
}

bool ioAlchemicInventory::CheckEmptySlot()
{
	vAlchemicItem::iterator iter, iEnd;
	iEnd = m_vAlchemicList.end();

	int iCurSlotCnt = 0;
	for(iter = m_vAlchemicList.begin();iter != iEnd;iter++)
	{
		AlchemicItem &kAlchemic = *iter;

		iCurSlotCnt++;

		if( kAlchemic.m_iCode == 0 )
			return true;
	}

	if( iCurSlotCnt < MAX_INVEN )
		return true;

	return false;
}

int ioAlchemicInventory::GetAlchemicItemCnt( int iCode )
{
	AlchemicItem kTmpItem;
	if( FindAlchemicItem( iCode, kTmpItem ) )
		return kTmpItem.m_iCount;

	return 0;
}

bool ioAlchemicInventory::GetAlchemicItemInfo( vAlchemicItemInfoList &rkInfoList )
{
	rkInfoList.clear();

	if( m_vAlchemicList.empty() )
		return false;

	vAlchemicItem::iterator iter, iEnd;
	iEnd = m_vAlchemicList.end();
	for(iter = m_vAlchemicList.begin();iter != iEnd;iter++)
	{
		if( (*iter).m_iCode > 0 && (*iter).m_iCount > 0 )
		{
			AlchemicItemInfo kInfo;
			kInfo.m_iCode = (*iter).m_iCode;
			kInfo.m_iCount = (*iter).m_iCount;
			kInfo.m_bNewAdd = (*iter).m_bNewAdd;

			kInfo.m_szIcon = g_AlchemicMgr.GetAlchemicItemIcon( kInfo.m_iCode );

			rkInfoList.push_back( kInfo );

			(*iter).m_bNewAdd = false;
		}
	}

	if( rkInfoList.empty() )
		return false;

	return true;
}

void ioAlchemicInventory::AlchemicItemAddNew( int iCode )
{
	if( m_vAlchemicList.empty() )
		return;

	vAlchemicItem::iterator iter, iEnd;
	iEnd = m_vAlchemicList.end();
	for(iter = m_vAlchemicList.begin();iter != iEnd;iter++)
	{
		if( (*iter).m_iCode == iCode )
		{
			(*iter).m_bNewAdd = true;
			break;
		}
	}
}

