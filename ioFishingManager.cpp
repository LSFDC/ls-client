

#include "stdafx.h"

#include "ioFishingManager.h"

#include "ioDecorationPrice.h"
#include "ioEtcItem.h"

template<> ioFishingManager* Singleton< ioFishingManager >::ms_Singleton = 0;

ioFishingManager::ioFishingManager()
{
	ClearAllInfo();
}

ioFishingManager::~ioFishingManager()
{
	ClearAllInfo();
}

void ioFishingManager::LoadFishingInfo()
{
	ClearAllInfo();

	ioINILoader_e kLoader( "config/sp2_fishing_info.ini" );

	kLoader.SetTitle_e( "common" );

	LoadFishItemList( kLoader );
	LoadFishGradeList( kLoader );
	LoadFishTypeList( kLoader );
}

void ioFishingManager::LoadFishItemList( ioINILoader& rkLoader )
{
	// ItemList
	int iItemCnt = rkLoader.LoadInt_e( "max_item_cnt", 0 );
	
	char szKey[MAX_PATH] = "";
	char szBuf[MAX_PATH] = "";
	for( int i=0; i < iItemCnt; ++i )
	{
		FishingItem kItem;

		wsprintf_e( szKey, "item%d_name", i+1 );
		rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );
		kItem.m_Name = szBuf;

		wsprintf_e( szKey, "item%d_icon_name", i+1 );
		rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );
		kItem.m_IconName = szBuf;

		wsprintf_e( szKey, "item%d_desc", i+1 );
		rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );
		kItem.m_Desc = szBuf;

		wsprintf_e( szKey, "item%d_num", i+1 );
		kItem.m_iItemNum = rkLoader.LoadInt( szKey, 0 );

		wsprintf_e( szKey, "item%d_peso", i+1 );
		kItem.m_iPeso = (__int64)rkLoader.LoadInt( szKey, 0 );

		m_vFishingItemList.push_back( kItem );
	}
}

void ioFishingManager::LoadFishGradeList( ioINILoader& rkLoader )
{
	// ItemGradeList
	int iGradeCnt = rkLoader.LoadInt_e( "max_grade_cnt", 0 );

	char szKey[MAX_PATH] = "";
	char szBuf[MAX_PATH] = "";
	for( int j=0; j < iGradeCnt; ++j )
	{
		FishingItemGrade kGrade;

		wsprintf_e( szKey, "grade%d_name", j+1 );
		rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );
		kGrade.m_Name = szBuf;

		sprintf_e( szKey, "grade%d_desc", j+1 );
		rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );
		kGrade.m_Desc  = szBuf;

		wsprintf_e( szKey, "grade%d_num", j+1 );
		kGrade.m_iGradeNum = rkLoader.LoadInt( szKey, 0 );

		wsprintf_e( szKey, "grade%d_value", j+1 );
		kGrade.m_fValue = rkLoader.LoadFloat( szKey, FLOAT1 );

		m_vFishingItemGradeList.push_back( kGrade );
	}
}

void ioFishingManager::LoadFishTypeList( ioINILoader& rkLoader )
{
	// type list
	int iTypeCnt = rkLoader.LoadInt_e( "type_cnt", 0 );
	
	char szKey[MAX_PATH] = "";
	char szBuf[MAX_PATH] = "";
	for( int k=0; k < iTypeCnt; ++k )
	{
		FishingTypeInfo kInfo;

		wsprintf_e( szKey, "fishing_rod%d", k+1 );
		rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );
		kInfo.m_FishingRod = szBuf;

		wsprintf_e( szKey, "fishing_ready%d", k+1 );
		rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );
		kInfo.m_FishingReadyEffect = szBuf;

		wsprintf_e( szKey, "fishing_wait%d", k+1 );
		rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );
		kInfo.m_FishingWaitEffect = szBuf;

		wsprintf_e( szKey, "fishing_pull%d", k+1 );
		rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );
		kInfo.m_FishingPullEffect = szBuf;

		m_vFishingTypeInfoList.push_back( kInfo );
	}
}

void ioFishingManager::LoadGuildFishingInfo()
{
	ioINILoader_e kLoader( "config/sp2_fishing_info.ini" );
	kLoader.SetTitle_e( "GuildFisheryItem" );
	LoadFishItemList( kLoader );
	LoadFishGradeList( kLoader );
}

void ioFishingManager::ClearAllInfo()
{
	m_vFishingItemList.clear();
	m_vFishingItemGradeList.clear();
	m_vFishingTypeInfoList.clear();
}

ioFishingManager& ioFishingManager::GetSingleton()
{
	return Singleton< ioFishingManager >::GetSingleton();
}

ioHashString ioFishingManager::GetFishingItemName( int iItemType )
{
	FishingItemList::iterator iter = m_vFishingItemList.begin();
	while( iter != m_vFishingItemList.end() )
	{
		if( (*iter).m_iItemNum == iItemType )
			return (*iter).m_Name;

		++iter;
	}

	return ioHashString();
}

ioHashString ioFishingManager::GetFishingItemDesc( int iItemType )
{
	FishingItemList::iterator iter = m_vFishingItemList.begin();
	while( iter != m_vFishingItemList.end() )
	{
		if( (*iter).m_iItemNum == iItemType )
			return (*iter).m_Desc;

		++iter;
	}

	return ioHashString();
}

ioHashString ioFishingManager::GetFishingItemIconName( int iItemType )
{
	FishingItemList::iterator iter = m_vFishingItemList.begin();
	while( iter != m_vFishingItemList.end() )
	{
		if( (*iter).m_iItemNum == iItemType )
			return (*iter).m_IconName;

		++iter;
	}

	return ioHashString();
}

ioHashString ioFishingManager::GetFishingItemGradeName( int iItemGrade )
{
	int iGradeCnt = m_vFishingItemGradeList.size();
	for( int i=0; i < iGradeCnt; ++i )
	{
		if( m_vFishingItemGradeList[i].m_iGradeNum == iItemGrade )
		{
			return m_vFishingItemGradeList[i].m_Name;
		}
	}

	return ioHashString();
}

ioHashString ioFishingManager::GetFishingItemGradeDesc( int iItemGrade )
{
	int iGradeCnt = m_vFishingItemGradeList.size();
	for( int i=0; i < iGradeCnt; ++i )
	{
		if( m_vFishingItemGradeList[i].m_iGradeNum == iItemGrade )
		{
			return m_vFishingItemGradeList[i].m_Desc;
		}
	}

	return ioHashString();
}
/*
__int64 ioFishingManager::GetFishingItemSellPeso( int iItemType, int iItemGrade )
{
	__int64 iResultPeso = 0;
	float fGradeRate = FLOAT1;

	int iGradeCnt = m_vFishingItemGradeList.size();
	for( int i=0; i < iGradeCnt; ++i )
	{
		if( m_vFishingItemGradeList[i].m_iGradeNum == iItemGrade )
		{
			fGradeRate = m_vFishingItemGradeList[i].m_fValue;
			break;
		}
	}

	FishingItemList::iterator iter = m_vFishingItemList.begin();
	while( iter != m_vFishingItemList.end() )
	{
		if( (*iter).m_iItemNum == iItemType )
		{
			iResultPeso = (*iter).m_iPeso;
			iResultPeso *= fGradeRate;
			break;
		}

		++iter;
	}

	return iResultPeso;
}
*/
ioHashString ioFishingManager::GetFishingRod( int iType )
{
	int iExtraType = -1;
	ioEtcItem *pItem = g_EtcItemMgr.FindEtcItem( iType );
	if( pItem )
	{
		iExtraType = pItem->GetExtraType() - 1;
	}

	if( COMPARE( iExtraType, 0, (int)m_vFishingTypeInfoList.size() ) )
	{
		return m_vFishingTypeInfoList[iExtraType].m_FishingRod;
	}

	return ioHashString();
}

ioHashString ioFishingManager::GetFishingReadyEffect( int iType )
{
	int iExtraType = -1;
	ioEtcItem *pItem = g_EtcItemMgr.FindEtcItem( iType );
	if( pItem )
	{
		iExtraType = pItem->GetExtraType() - 1;
	}

	if( COMPARE( iExtraType, 0, (int)m_vFishingTypeInfoList.size() ) )
	{
		return m_vFishingTypeInfoList[iExtraType].m_FishingReadyEffect;
	}

	return ioHashString();
}

ioHashString ioFishingManager::GetFishingWaitEffect( int iType )
{
	int iExtraType = -1;
	ioEtcItem *pItem = g_EtcItemMgr.FindEtcItem( iType );
	if( pItem )
	{
		iExtraType = pItem->GetExtraType() - 1;
	}

	if( COMPARE( iExtraType, 0, (int)m_vFishingTypeInfoList.size() ) )
	{
		return m_vFishingTypeInfoList[iExtraType].m_FishingWaitEffect;
	}

	return ioHashString();
}

ioHashString ioFishingManager::GetFishingPullEffect( int iType )
{
	int iExtraType = -1;
	ioEtcItem *pItem = g_EtcItemMgr.FindEtcItem( iType );
	if( pItem )
	{
		iExtraType = pItem->GetExtraType() - 1;
	}

	if( COMPARE( iExtraType, 0, (int)m_vFishingTypeInfoList.size() ) )
	{
		return m_vFishingTypeInfoList[iExtraType].m_FishingPullEffect;
	}

	return ioHashString();
}

ioHashString ioFishingManager::GetFishingPresentIconName( short iPresentType, int iValue1, int iValue2 )
{
	switch( iPresentType )
	{
	case PRESENT_SOLDIER:
		return g_MyInfo.GetMySoldierIconName( iValue1 );
	case PRESENT_DECORATION:
		{
			int iClassType = iValue1 / 100000;
			int iSexType   = (iValue1 % 100000) / FLOAT1000;
			int iDecoType  = iValue1 % 1000;
			const DecoData &kData = g_DecorationPrice.GetDecoData( iSexType, iDecoType, iValue2 );
			return kData.m_szIcon;
		}
		break;
	case PRESENT_ETC_ITEM:
		{
			ioEtcItem *pEtcItem = g_EtcItemMgr.FindEtcItem( iValue1 );
			if( pEtcItem )
			{
				return pEtcItem->GetIconName();
			}			
		}
		break;
	case PRESENT_PESO:
		{
			return ioHashString( "UIIconPack13#quest_002" );
		}
		break;
	case PRESENT_EXTRAITEM:
	case PRESENT_ACCESSORY:
		{
			const ioItem *pItem = g_ItemMaker.GetItemConst( iValue1, __FUNCTION__ );
			if( pItem )
			{
				ioHashString szIcon = pItem->GetItemLargeIconName();
				if( !szIcon.IsEmpty() )
				{
					return szIcon;
				}
			}
		}
		break;
	case PRESENT_EXTRAITEM_BOX:
		{
			RandomMachineInfo *pInfo = g_ExtraItemInfoMgr.GetRandomMachineInfo( iValue1 );
			if( pInfo )
			{
				ioHashString szIcon = pInfo->GetIconName();
				if( !szIcon.IsEmpty() )
				{
					return szIcon;
				}
			}
		}
		break;
	case PRESENT_RANDOM_DECO:
		{
			int iClassType = iValue1 / 100000;
			int iSexType   = (iValue1 % 100000) / FLOAT1000;
			int iDecoType  = iValue1 % 1000;
			const DecoData &kData = g_DecorationPrice.GetDecoData( iSexType, iDecoType, iValue2 );
			return kData.m_szIcon;
		}
		break;
	case PRESENT_GRADE_EXP:
		{
			return ioHashString( "UIIconPack13#quest_001" );
		}
		break;
	case PRESENT_MEDALITEM:
		{
			const ioMedalItemInfoManager::ItemInfo *pItem = g_MedalItemMgr.GetItemInfo( iValue1 );
			ioHashString szIcon = pItem->m_sIcon;
			if( !szIcon.IsEmpty() )
			{
				return szIcon;
			}
		}
		break;
	case PRESENT_COSTUME:
		{
			ioHashString szIcon = g_CostumeInfoMgr.GetCostumeIcon( iValue1 );
			if( !szIcon.IsEmpty() )
				return szIcon;
		}
		break;
	case PRESENT_COSTUME_BOX:
		{
			CostumeShopInfo *pInfo = g_CostumeShopInfoMgr.GetCostumeMachineInfo( iValue1 );
			if( pInfo )
			{
				ioHashString szIcon = pInfo->GetIconName();
				if( !szIcon.IsEmpty() )
					return szIcon;
			}
		}
		break;
	case PRESENT_BONUS_CASH:
		{
			return ioHashString( "UIIconPack101#Cash" );
		}
		break;
	}

	return ioHashString();
}

ioHashString ioFishingManager::GetFishingPresentItemName( short iPresentType, int iValue1, int iValue2 )
{
	static ioHashString szReturn;

	ioHashString szText1 = g_PresentMgr.GetPresentValue1Text( iPresentType, iValue1, iValue2 );
	ioHashString szText2 = g_PresentMgr.GetPresentValue2Text( iPresentType, iValue1, iValue2 );

	char szBuf[MAX_PATH] = "";
	wsprintf_e( szBuf, "%s %s", szText1.c_str(), szText2.c_str() );
	szReturn = szBuf;

	switch( iPresentType )
	{
	case PRESENT_ETC_ITEM:
		{
			ioEtcItem *pEtcItem = g_EtcItemMgr.FindEtcItem( iValue1 );
			if( !pEtcItem )
				szReturn = "None";
		}
		break;
	case PRESENT_EXTRAITEM:
	case PRESENT_ACCESSORY:
		{
			const ioItem *pItem = g_ItemMaker.GetItemConst( iValue1, __FUNCTION__ );
			if( !pItem )
				szReturn = "None";
		}
		break;
	case PRESENT_EXTRAITEM_BOX:
		{
			RandomMachineInfo *pInfo = g_ExtraItemInfoMgr.GetRandomMachineInfo( iValue1 );
			if( !pInfo )
				szReturn = "None";
		}
		break;
	case PRESENT_MEDALITEM:
		{
			const ioMedalItemInfoManager::ItemInfo *pItem = g_MedalItemMgr.GetItemInfo( iValue1 );
			if( !pItem )	
				szReturn = "None";
		}
		break;
	case PRESENT_COSTUME:
		{
			CostumeInfo sCostumeInfo;
			if ( !g_CostumeInfoMgr.GetCostumeInfo( iValue1, sCostumeInfo ) )
				szReturn = "None";
		}
		break;
	case PRESENT_COSTUME_BOX:
		{
			CostumeShopInfo *pInfo = g_CostumeShopInfoMgr.GetCostumeMachineInfo( iValue1 );
			if( !pInfo )
				szReturn = "None";
		}
		break;
	}

	return szReturn;
}



