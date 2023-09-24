#include "stdafx.h"

#include "ioUserEtcItem.h"

ioUserEtcItem::ioUserEtcItem()
{
	Initialize();
}

ioUserEtcItem::~ioUserEtcItem()
{
	m_vEtcItemList.clear();
}

void ioUserEtcItem::Initialize()
{
	m_vEtcItemList.clear();
}

void ioUserEtcItem::ApplyData( SP2Packet &rkPacket )
{
	Initialize();

	int iSize;
	rkPacket >> iSize;
	for(int i = 0;i < iSize;i++)
	{
		ETCITEMDB kEtcItemDB;
		for (int i = 0; i < MAX_SLOT ; i++)
		{
			rkPacket >> kEtcItemDB.m_EtcItem[i].m_iType >> kEtcItemDB.m_EtcItem[i].m_iValue1 >> kEtcItemDB.m_EtcItem[i].m_iValue2;
			InsertStartTimeMap( kEtcItemDB.m_EtcItem[i].m_iType );
		}
		m_vEtcItemList.push_back( kEtcItemDB );
	}
}

int ioUserEtcItem::GetUse( int iType )
{
	if( m_vEtcItemList.empty() )
		return 0;

	for(vETCITEMDB::iterator iter = m_vEtcItemList.begin(); iter != m_vEtcItemList.end(); ++iter)
	{
	    ETCITEMDB &kEtcItemDB = *iter;
		for (int i = 0; i < MAX_SLOT ; i++)
		{
			if( kEtcItemDB.m_EtcItem[i].m_iType == iType )
			{
				return kEtcItemDB.m_EtcItem[i].m_iValue1;
			}
		}
	}

	return 0;
}

bool ioUserEtcItem::AddEtcItem( const ETCITEMSLOT &rkNewSlot )
{
	if( rkNewSlot.m_iType == 0 )
	{
		LOG.PrintTimeAndLog( 0, "ioUserEtcItem::AddEtcItem() Type is Zero" );
		return false;
	}

	vETCITEMDB::iterator iter, iEnd;
	iEnd = m_vEtcItemList.end();

	// update
	for(iter = m_vEtcItemList.begin();iter != iEnd;iter++)
	{
		ETCITEMDB &kEtcItemDB = *iter;
		for(int i = 0;i < MAX_SLOT;i++)
		{
			if( kEtcItemDB.m_EtcItem[i].m_iType == rkNewSlot.m_iType )
			{
				kEtcItemDB.m_EtcItem[i] = rkNewSlot;
				InsertStartTimeMap( rkNewSlot.m_iType );
				return true;
			}
		}		
	}

	// put blank
	for(iter = m_vEtcItemList.begin();iter != iEnd;iter++)
	{
		ETCITEMDB &kEtcItemDB = *iter;
		for(int i = 0;i < MAX_SLOT;i++)
		{
			if( kEtcItemDB.m_EtcItem[i].m_iType   == 0 &&
				kEtcItemDB.m_EtcItem[i].m_iValue1 == 0 &&
				kEtcItemDB.m_EtcItem[i].m_iValue2 == 0   )
			{
				kEtcItemDB.m_EtcItem[i] = rkNewSlot;
				InsertStartTimeMap( rkNewSlot.m_iType );
				return true;
			}
		}		
	}

	// New
	ETCITEMDB kEtcItemDB;
	kEtcItemDB.m_EtcItem[0] = rkNewSlot;
	m_vEtcItemList.push_back( kEtcItemDB );
	InsertStartTimeMap( rkNewSlot.m_iType );
	return true;
}


bool ioUserEtcItem::GetEtcItem( IN int iType, OUT ETCITEMSLOT &rkEtcItem )
{
	// �ʱ�ȭ
	rkEtcItem.m_iType   = 0;
	rkEtcItem.m_iValue1 = 0;
	rkEtcItem.m_iValue2 = 0;

	for(vETCITEMDB::iterator iter = m_vEtcItemList.begin(); iter != m_vEtcItemList.end(); ++iter)
	{
		ETCITEMDB &kEtcItemDB = *iter;
		for (int i = 0; i < MAX_SLOT ; i++)
		{
			if( kEtcItemDB.m_EtcItem[i].m_iType == iType )
			{
				rkEtcItem = kEtcItemDB.m_EtcItem[i];
				return true;
			}
		}
	}
	return false;
}

bool ioUserEtcItem::GetEtcItemByArray( IN int iArray, OUT ETCITEMSLOT &rkEtcItem )
{
	// �ʱ�ȭ
	rkEtcItem.m_iType   = 0;
	rkEtcItem.m_iValue1 = 0;
	rkEtcItem.m_iValue2 = 0;

	for(vETCITEMDB::iterator iter = m_vEtcItemList.begin(); iter != m_vEtcItemList.end(); ++iter)
	{
		ETCITEMDB &kEtcItemDB = *iter;
		for (int i = 0; i < MAX_SLOT ; i++, iArray--)
		{
			if( iArray == 0 )
			{
				rkEtcItem = kEtcItemDB.m_EtcItem[i];
				return true;
			}
		}
	}
	return false;
}

bool ioUserEtcItem::GetEtcItemRange( IN int iStarType, IN int iEndType, OUT ETCITEMSLOT &rkEtcItem )
{
	// �ʱ�ȭ
	rkEtcItem.m_iType   = 0;
	rkEtcItem.m_iValue1 = 0;
	rkEtcItem.m_iValue2 = 0;

	for(vETCITEMDB::iterator iter = m_vEtcItemList.begin(); iter != m_vEtcItemList.end(); ++iter)
	{
		ETCITEMDB &kEtcItemDB = *iter;
		for (int i = 0; i < MAX_SLOT ; i++)
		{
			if( COMPARE( kEtcItemDB.m_EtcItem[i].m_iType, iStarType, iEndType + 1 ) )
			{
				rkEtcItem = kEtcItemDB.m_EtcItem[i];
				return true;
			}
		}
	}
	return false;
}

int  ioUserEtcItem::GetEtcItemCurrentSlot()
{
	// ���� �������� ���� ����
	return (int)m_vEtcItemList.size() * MAX_SLOT;
}

void ioUserEtcItem::SetEtcItem( const ETCITEMSLOT &rkEtcItem )
{
	for(vETCITEMDB::iterator iter = m_vEtcItemList.begin(); iter != m_vEtcItemList.end(); ++iter)
	{
		ETCITEMDB &kEtcItemDB = *iter;
		for (int i = 0; i < MAX_SLOT ; i++)
		{
			if( kEtcItemDB.m_EtcItem[i].m_iType == rkEtcItem.m_iType )
			{
				kEtcItemDB.m_EtcItem[i] = rkEtcItem;
			}
		}
	}
}

void ioUserEtcItem::DeleteEtcItem( int iType )
{
	for(vETCITEMDB::iterator iter = m_vEtcItemList.begin(); iter != m_vEtcItemList.end(); ++iter)
	{
		ETCITEMDB &kEtcItemDB = *iter;
		for (int i = 0; i < MAX_SLOT ; i++)
		{
			if( kEtcItemDB.m_EtcItem[i].m_iType == iType )
			{
#ifndef SHIPPING
				LOG.PrintTimeAndLog( 0, "DeleteEtcItem : Type:%d / Value1:%d / Value2:%d",kEtcItemDB.m_EtcItem[i].m_iType, kEtcItemDB.m_EtcItem[i].m_iValue1, kEtcItemDB.m_EtcItem[i].m_iValue2 );
#endif			
				kEtcItemDB.m_EtcItem[i].m_iType   = 0;
				kEtcItemDB.m_EtcItem[i].m_iValue1 = 0;
				kEtcItemDB.m_EtcItem[i].m_iValue2 = 0;
			}
		}
	}

	DeleteStartTimeMap( iType );
}

void ioUserEtcItem::InsertStartTimeMap( int iType )
{
	if( ( iType/ioEtcItem::USE_TYPE_CUT_VALUE ) != ioEtcItem::UT_TIME )
		return;

	StartTimeMap::iterator iter = m_StartTimeMap.find( iType );
	if( iter != m_StartTimeMap.end() )
		return;

	m_StartTimeMap.insert( StartTimeMap::value_type( iType, 0 ) );		
}


void ioUserEtcItem::DeleteStartTimeMap( int iType )
{
	StartTimeMap::iterator iter = m_StartTimeMap.find( iType );
	if( iter == m_StartTimeMap.end() )
		return;

	m_StartTimeMap.erase( iter );
}

void ioUserEtcItem::SetStartTimeMap( int iType, DWORD dwStartTime )
{
	StartTimeMap::iterator iter = m_StartTimeMap.find( iType );
	if( iter == m_StartTimeMap.end() )
		return;
	DWORD &rdwStartTime = iter->second;
	rdwStartTime = dwStartTime;
}

void ioUserEtcItem::ApplyTimeData( SP2Packet &rkPacket, bool bClearStartTime /*= false */ )
{
	int iSize = 0;
	rkPacket >> iSize;
	for (int i = 0; i < iSize ; i++)
	{
		int iType         = 0;
		int iValue1       = 0;
		int iValue2       = 0;

		rkPacket >> iType;
		rkPacket >> iValue1;
		rkPacket >> iValue2;

		if( iType == 0 )
		{
			LOG.PrintTimeAndLog( 0, "ApplyTimeData : Type is Zero." );
			continue;
		}

		ETCITEMSLOT kSlot;
		if( !GetEtcItem( iType, kSlot ) )
		{
			LOG.PrintTimeAndLog( 0, "ApplyTimeData : Type is wrong. %d", iType );
			continue;
		}

		kSlot.m_iValue1 = iValue1;
		kSlot.m_iValue2 = iValue2;
		SetEtcItem( kSlot );
		if( bClearStartTime )
			SetStartTimeMap( iType, 0 );
		else
			SetStartTimeMap( iType, FRAMEGETTIME() );
	}
}

void ioUserEtcItem::DeleteEtcItemZeroTime()
{
	for(vETCITEMDB::iterator iter = m_vEtcItemList.begin(); iter != m_vEtcItemList.end(); ++iter)
	{
		ETCITEMDB &kEtcItemDB = *iter;
		for (int i = 0; i < MAX_SLOT ; i++)
		{
			if( (kEtcItemDB.m_EtcItem[i].m_iType / ioEtcItem::USE_TYPE_CUT_VALUE) != ioEtcItem::UT_TIME )
				continue;
			if( kEtcItemDB.m_EtcItem[i].m_iValue1 > 0 )
				continue;

			DeleteStartTimeMap( kEtcItemDB.m_EtcItem[i].m_iType );

			kEtcItemDB.m_EtcItem[i].m_iType   = 0;
			kEtcItemDB.m_EtcItem[i].m_iValue1 = 0;
			kEtcItemDB.m_EtcItem[i].m_iValue2 = 0;
		}
	}
}

int ioUserEtcItem::GetValueByCalcTime( int iType )
{
	if( ( iType / ioEtcItem::USE_TYPE_CUT_VALUE ) != ioEtcItem::UT_TIME )
		return 0;

	StartTimeMap::iterator iter = m_StartTimeMap.find( iType );
	if( iter == m_StartTimeMap.end() )
		return 0;

	int iTotalSecond = GetUse( iType );

	DWORD &rdwStartTime = iter->second;
	if( rdwStartTime == 0 )
		return iTotalSecond;

	enum { DIVIDE_VALUE = 1000, };
	DWORD dwGapSecond = ( FRAMEGETTIME() - rdwStartTime ) / DIVIDE_VALUE;
	iTotalSecond -= dwGapSecond;
	if( iTotalSecond < 0 )
		return 0;

	return iTotalSecond;
}

bool ioUserEtcItem::IsCanUse( int iType )
{
	for(vETCITEMDB::iterator iter = m_vEtcItemList.begin(); iter != m_vEtcItemList.end(); ++iter)
	{
		ETCITEMDB &kEtcItemDB = *iter;
		for (int i = 0; i < MAX_SLOT ; i++)
		{
			if( kEtcItemDB.m_EtcItem[i].m_iType == iType )
			{
				if( kEtcItemDB.m_EtcItem[i].m_iValue1 > 0 )
					return true;
			}
		}
	}
	return false;
}

void ioUserEtcItem::SendUse( int iType )
{
	if( ( iType / ioEtcItem::USE_TYPE_CUT_VALUE ) != ioEtcItem::UT_COUNT )
		return;

	for(vETCITEMDB::iterator iter = m_vEtcItemList.begin(); iter != m_vEtcItemList.end(); ++iter)
	{
		ETCITEMDB &kEtcItemDB = *iter;
		for (int i = 0; i < MAX_SLOT ; i++)
		{
			if( kEtcItemDB.m_EtcItem[i].m_iType != iType )
				continue;
			if( kEtcItemDB.m_EtcItem[i].m_iValue1 <= 0 )
				continue;
			kEtcItemDB.m_EtcItem[i].m_iValue1--;
			SP2Packet kPacket( CTPK_ETCITEM_USE );
			kPacket << iType;
			TCPNetwork::SendToServer( kPacket );
		}
	}
}

int ioUserEtcItem::GetEtcItemCnt()
{
	int iSize = m_vEtcItemList.size();

	return iSize;
}

void ioUserEtcItem::GetUseInfo( IN int iType, OUT ioHashString &rsInfo1, OUT ioHashString &rsInfo2, OUT DWORD &rInfo1Color, OUT DWORD &rInfo2Color )
{
	/*
	|EXE_ioUserEtcItem::GetUseInfo_1|%d / %d ��|
	|EXE_ioUserEtcItem::GetUseInfo_2|�����Ϸ�|
	|EXE_ioUserEtcItem::GetUseInfo_3|������(%d�� ����)|
	|EXE_ioUserEtcItem::GetUseInfo_4|%d�� �߰�|
	|EXE_ioUserEtcItem::GetUseInfo_5|%d���� �߰�|
	|EXE_ioUserEtcItem::GetUseInfo_6|%d���� �߰�|
	|EXE_ioUserEtcItem::GetUseInfo_7|%d��|
	|EXE_ioUserEtcItem::GetUseInfo_8|�������|
	|EXE_ioUserEtcItem::GetUseInfo_9|%d��|
	|EXE_ioUserEtcItem::GetUseInfo_10|%d��|
	|EXE_ioUserEtcItem::GetUseInfo_11|%d�ð� %d��|
	|EXE_ioUserEtcItem::GetUseInfo_12|�������|
	|EXE_ioUserEtcItem::GetUseInfo_13|��밡��|
	|EXE_ioUserEtcItem::GetUseInfo_14|%d�� ����|
	|EXE_ioUserEtcItem::GetUseInfo_15|%d�ð� ����|
	|EXE_ioUserEtcItem::GetUseInfo_16|%d�ð� %d�� ����|
	*/
	ETCITEMSLOT kSlot;
	if( !GetEtcItem( iType, kSlot ) )
		return;

	rInfo1Color = TCT_DEFAULT_GRAY;    // ���� �ٲܰ����� �ִ´�.
	rInfo2Color = TCT_DEFAULT_GRAY;

	char szText1[MAX_PATH] = "";
	char szText2[MAX_PATH] = "";
	if( kSlot.m_iType == ioEtcItem::EIT_ETC_MONSTER_COIN )    // ���� ǥ�� - ���� ���� �ð�
	{
		SafeSprintf( szText1, sizeof( szText1 ), STR(1), kSlot.GetUse(), Help::GetRefillCoinMax() );		
		if( kSlot.GetUse() >= Help::GetRefillCoinMax() )
			SafeSprintf( szText2, sizeof( szText2 ), STR(2) );
		else 
			SafeSprintf( szText2, sizeof( szText2 ), STR(3), g_MyInfo.RemainRefillMinute() );
	}
	else if( COMPARE( kSlot.m_iType, ioEtcItem::EIT_ETC_ITEM_TIME_GASHAPON1, ioEtcItem::EIT_ETC_ITEM_TIME_GASHAPON100 + 1 )	  ||
		     COMPARE( kSlot.m_iType, ioEtcItem::EIT_ETC_ITEM_TIME_GASHAPON101, ioEtcItem::EIT_ETC_ITEM_TIME_GASHAPON300 + 1 ) ||
			 COMPARE( kSlot.m_iType, ioEtcItem::EIT_ETC_ITEM_TIME_GASHAPON301, ioEtcItem::EIT_ETC_ITEM_TIME_GASHAPON600 + 1 ) )
	{
		// �Ⱓ
		if( g_EtcItemMgr.IsBlockEtcItem( kSlot.m_iType ) && kSlot.m_iValue1 == 0 && kSlot.GetHour() == 0 && kSlot.GetMinute() == 0 )
		{
			SafeSprintf( szText1, sizeof( szText1 ), STR(12) );
		}
		else
		{
			Help::GetRemainTime( kSlot.GetYear(), kSlot.GetMonth(), kSlot.GetDay(), kSlot.GetHour(), kSlot.GetMinute(), szText1, sizeof( szText1 ) );
		}

		// ��� ����
		ioEtcItemTimeGashapon *pEtcItem = dynamic_cast< ioEtcItemTimeGashapon * >( g_EtcItemMgr.FindEtcItem( kSlot.m_iType ) );
		if( !pEtcItem || !pEtcItem->IsExtendData() )
		{
			if( kSlot.GetDateExcludeValue2() == 0 )
			{
				rInfo2Color = TCT_DEFAULT_BLUE;
				SafeSprintf( szText2, sizeof( szText2 ), STR(13) );
			}
			else
			{
				rInfo2Color = TCT_DEFAULT_RED;
				int iValue = max( 1, kSlot.GetDateExcludeValue2() );
				if( iValue < 60 )
				{
					SafeSprintf( szText2, sizeof( szText2 ), STR(17), iValue );
				}
				else
				{
					int iHour   = (float)iValue / 60;
					int iMinute = iValue % 60;
					if( iMinute == 0 )
					{
						SafeSprintf( szText2, sizeof( szText2 ), STR(18), iHour );
					}
					else
					{
						SafeSprintf( szText2, sizeof( szText2 ), STR(19), iHour, iMinute );
					}
				}
			}
		}
		else if( pEtcItem->IsSequenceOrder() )
		{
			int iState = kSlot.GetDateExcludeValue3State();
			int iRealTime = kSlot.GetDateExcludeValue3Time();

			if( iRealTime == 0 )
			{
				rInfo2Color = TCT_DEFAULT_BLUE;
				SafeSprintf( szText2, sizeof( szText2 ), STR(13) );
			}
			else
			{
				rInfo2Color = TCT_DEFAULT_RED;
				int iValue = max( 1, iRealTime );
				if( iValue < 60 )
				{
					SafeSprintf( szText2, sizeof( szText2 ), STR(17), iValue );
				}
				else
				{
					int iHour   = (float)iValue / 60;
					int iMinute = iValue % 60;
					if( iMinute == 0 )
					{
						SafeSprintf( szText2, sizeof( szText2 ), STR(18), iHour );
					}
					else
					{
						SafeSprintf( szText2, sizeof( szText2 ), STR(19), iHour, iMinute );
					}
				}
			}
		}
		else 
		{
			rInfo2Color = TCT_DEFAULT_RED;
			int iValue = max( 0, pEtcItem->GetRepeatDate() - kSlot.GetDateExcludeValue2() );
			if( iValue < 60 )
			{
				SafeSprintf( szText2, sizeof( szText2 ), STR(14), iValue );
			}
			else
			{
				int iHour   = (float)iValue / 60;
				int iMinute = iValue % 60;
				if( iMinute == 0 )
				{
					SafeSprintf( szText2, sizeof( szText2 ), STR(15), iHour );
				}
				else
				{
					SafeSprintf( szText2, sizeof( szText2 ), STR(16), iHour, iMinute );
				}
			}
		}
	}
	else if( kSlot.m_iType == ioEtcItem::EIT_ETC_FRIEND_SLOT_EXTEND ) // ���� ǥ�� ������ UT_ETERNITY
	{
		SafeSprintf( szText1, sizeof( szText1 ), STR(4), kSlot.GetUse() );
	}
	else if( kSlot.m_iType == ioEtcItem::EIT_ETC_CHAR_SLOT_EXTEND || 
			 kSlot.m_iType == ioEtcItem::EIT_ETC_EXTRAITEM_SLOT_EXTEND ) // ���� ǥ�� ������ UT_ETERNITY
	{
		SafeSprintf( szText1, sizeof( szText1 ), STR(5), kSlot.GetUse() );
	}
	else if( kSlot.m_iType == ioEtcItem::EIT_ETC_FISHING_SLOT_EXTEND ) // ���� ǥ�� ������ UT_ETERNITY
	{
		SafeSprintf( szText1, sizeof( szText1 ), STR(6), kSlot.GetUse() );
	}
	else if( ( kSlot.m_iType / ioEtcItemManager::USE_TYPE_POS ) == ioEtcItem::UT_COUNT )
	{
		SafeSprintf( szText1, sizeof( szText1 ), STR(7), kSlot.GetUse() );
	}
	else if( ( kSlot.m_iType / ioEtcItemManager::USE_TYPE_POS ) == ioEtcItem::UT_ETERNITY )
	{
		SafeSprintf( szText1, sizeof( szText1 ), STR(8) );
	}
	else if( ( kSlot.m_iType / ioEtcItemManager::USE_TYPE_POS ) == ioEtcItem::UT_ONCE )
	{
		SafeSprintf( szText1, sizeof( szText1 ), STR(9), kSlot.GetUse() );
	}
	else if( ( kSlot.m_iType / ioEtcItemManager::USE_TYPE_POS ) == ioEtcItem::UT_TIME )
	{
		int iHour     = kSlot.GetUse()/3600;      
		int iMinute   = (kSlot.GetUse()%3600)/60;

		if( iHour == 0 )
			SafeSprintf( szText1, sizeof( szText1 ), STR(10), iMinute );
		else
			SafeSprintf( szText1, sizeof( szText1 ), STR(11), iHour, iMinute );
	}
	else if( ( kSlot.m_iType / ioEtcItemManager::USE_TYPE_POS ) == ioEtcItem::UT_DATE )
	{
		if( kSlot.m_iType == ioEtcItem::EIT_ETC_SOLDIER_EXP_BONUS )
		{
			if( kSlot.GetDateExcludeValue2() == 0 )
			{
				Help::GetRemainTime( kSlot.GetYear(), kSlot.GetMonth(), kSlot.GetDay(), kSlot.GetHour(), kSlot.GetMinute(), szText1, sizeof( szText1 ) );
			}
			else
			{
				sprintf_e( szText1, "%s", g_MyInfo.GetClassName( kSlot.GetDateExcludeValue2() ) );
				Help::GetRemainTime( kSlot.GetYear(), kSlot.GetMonth(), kSlot.GetDay(), kSlot.GetHour(), kSlot.GetMinute(), szText2, sizeof( szText2 ) );
			}
		}
		else if( g_EtcItemMgr.IsBlockEtcItem( kSlot.m_iType ) && kSlot.m_iValue1 == 0 && kSlot.m_iValue2 == 0 )
		{
			SafeSprintf( szText1, sizeof( szText1 ), STR(12) );
		}
		else
		{
			Help::GetRemainTime( kSlot.GetYear(), kSlot.GetMonth(), kSlot.GetDay(), kSlot.GetHour(), kSlot.GetMinute(), szText1, sizeof( szText1 ) );
		}
	}
	else
	{
		StringCbPrintf_e( szText1, sizeof( szText1 ), "%d", kSlot.GetUse() );
	}

	rsInfo1 = szText1;
	rsInfo2 = szText2;
}

void ioUserEtcItem::UpdateGashaponTime( int iMinute )
{
	for(vETCITEMDB::iterator iter = m_vEtcItemList.begin(); iter != m_vEtcItemList.end(); ++iter)
	{
		ETCITEMDB &kEtcItemDB = *iter;
		for (int i = 0; i < MAX_SLOT ; i++)
		{
			if( !COMPARE( kEtcItemDB.m_EtcItem[i].m_iType, ioEtcItem::EIT_ETC_ITEM_TIME_GASHAPON1, ioEtcItem::EIT_ETC_ITEM_TIME_GASHAPON100 + 1 ) &&
				!COMPARE( kEtcItemDB.m_EtcItem[i].m_iType, ioEtcItem::EIT_ETC_ITEM_TIME_GASHAPON101, ioEtcItem::EIT_ETC_ITEM_TIME_GASHAPON300 + 1 ) &&
				!COMPARE( kEtcItemDB.m_EtcItem[i].m_iType, ioEtcItem::EIT_ETC_ITEM_TIME_GASHAPON301, ioEtcItem::EIT_ETC_ITEM_TIME_GASHAPON600 + 1 ) )
				continue;

			ioEtcItemTimeGashapon* pGashapon = dynamic_cast<ioEtcItemTimeGashapon*>( g_EtcItemMgr.FindEtcItem( kEtcItemDB.m_EtcItem[i].m_iType ) );
			if( !pGashapon )
				continue;

			if( pGashapon->IsSequenceOrder() )
			{
				int iState	  = kEtcItemDB.m_EtcItem[i].GetDateExcludeValue3State();
				int iRealTime = kEtcItemDB.m_EtcItem[i].GetDateExcludeValue3Time();
				if( iRealTime > 0 )
				{
					kEtcItemDB.m_EtcItem[i].SetDateExcludeValue3( max( 0 , kEtcItemDB.m_EtcItem[i].GetDateExcludeValue3Time() - iMinute), iState );
					if( kEtcItemDB.m_EtcItem[i].GetDateExcludeValue3Time() == 0 )
					{
						// UI ����
						MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(g_GUIMgr.FindWnd(MY_INVENTORY_WND));
						if( pInvenWnd && pInvenWnd->IsShow() )
							pInvenWnd->UpdateTab( false, true );
					}
				}
			}
			else
			{
				if( kEtcItemDB.m_EtcItem[i].GetDateExcludeValue2() > 0 )
					kEtcItemDB.m_EtcItem[i].SetDateExcludeValue2( max( 1, kEtcItemDB.m_EtcItem[i].GetDateExcludeValue2() - iMinute ) );
			}
		}
	}
}
