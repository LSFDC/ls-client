

#include "stdafx.h"

#include "ioAlchemicMgr.h"
#include "ioAlchemicInventory.h"

//////////////////////////////////////////////////////////////////////////////////////////
template<> ioAlchemicMgr* Singleton< ioAlchemicMgr >::ms_Singleton = 0;

ioAlchemicMgr::ioAlchemicMgr()
{
}

ioAlchemicMgr::~ioAlchemicMgr()
{
	ClearAllInfo();
}

void ioAlchemicMgr::ClearAllInfo()
{
	m_AlchemicFuncInfoList.clear();

	m_AlchemicItemIconMap.clear();
	m_RandomMap.clear();

	m_SoldierDisassembleMap.clear();
	m_ExtraItemDisassembleMap.clear();

	//
	m_NewAlchemicListMap.clear();
	m_NewAlchemicSoldierInfoMap.clear();
	m_NewAlchemicItemInfoMap.clear();
}

void ioAlchemicMgr::LoadMgrInfo()
{
	char szBuf[MAX_PATH] = "";
	char szKey[MAX_PATH] = "";
	
	ClearAllInfo();

	ioINILoader_e kLoader( "config/sp2_alchemic_info.ini" );

	kLoader.SetTitle_e( "common_info" );

	// 실패시 재료 소모 비율
	m_fMinFailRate = kLoader.LoadFloat_e( "alchemic_fail_min_rate", 0.0f );
	m_fMaxFailRate = kLoader.LoadFloat_e( "alchemic_fail_max_rate", 0.0f );

	// 첨가물 갯수에 대한 상황별 곱하는 상수값
	m_fAdditiveConstSoldier = kLoader.LoadFloat_e( "additive_const_soldier", 0.0f );
	m_fAdditiveConstItem = kLoader.LoadFloat_e( "additive_const_item", 0.0f );
	m_fAdditiveConstExchange = kLoader.LoadFloat_e( "additive_const_exchange", 0.0f );
	
	// 아이템 change 상황에서 변화 비율
	m_fPieceChangeConst = kLoader.LoadFloat_e( "piece_change_const", 0.0f );

	// alchemicitem 판매시 곱하는 상수값
	m_fItemSellConst = kLoader.LoadFloat_e( "item_sell_const", 0.0f );

	// disassemble
	m_dwSoldierDisassembleConst = kLoader.LoadInt_e( "soldier_disassemble_const", FLOAT100 );
	m_dwExtraItemDisassembleConst = kLoader.LoadInt_e( "extraitem_disassemble_const", 50 );

	// 최소갯수
	m_iMinTotalCnt = kLoader.LoadInt_e( "change_min_cnt", 10 );

	//
	m_fMinExchangeRate = kLoader.LoadFloat_e( "exchange_min_rate", 25.0f );
	m_fMaxExchangeRate = kLoader.LoadFloat_e( "exchange_max_rate", FLOAT100 );

	// alchemic info
	int iInfoCnt = kLoader.LoadInt_e( "alchemic_info_cnt", 0 );
	for( int i=0; i < iInfoCnt; ++i )
	{
		wsprintf_e( szKey, "alchemic_info%d", i+1 );
		kLoader.SetTitle( szKey );
		LoadFuncInfo( kLoader );
	}

	if( !m_AlchemicFuncInfoList.empty() )
		std::sort( m_AlchemicFuncInfoList.begin(), m_AlchemicFuncInfoList.end(), AlchemicFuncSort() );

	LoadRandomInfo( kLoader );

	// item info
	kLoader.SetTitle_e( "alchemic_item_info" );

	int iCode;

	// 조각들
	int iItemCnt = kLoader.LoadInt_e( "alchemic_item_cnt", 0 );
	for( int i=0; i < iItemCnt; ++i )
	{
		AlchemicItemData kInfo;

		wsprintf_e( szKey, "alchemic_item%d", i+1 );
		iCode = kLoader.LoadInt( szKey, 0 );

		wsprintf_e( szKey, "alchemic_item_name%d", i+1 );
		kLoader.LoadString( szKey, "", szBuf, MAX_PATH );
		kInfo.m_DescName = szBuf;

		wsprintf_e( szKey, "alchemic_item_icon%d", i+1 );
		kLoader.LoadString( szKey, "", szBuf, MAX_PATH );
		kInfo.m_IconName = szBuf;

		m_AlchemicItemIconMap.insert( AlchemicItemIconMap::value_type( iCode, kInfo ) );
	}

	// 첨가물
	iItemCnt = kLoader.LoadInt_e( "alchemic_additive_cnt", 0 );
	for( int i=0; i < iItemCnt; ++i )
	{
		AlchemicItemData kInfo;

		wsprintf_e( szKey, "alchemic_additive%d", i+1 );
		iCode = kLoader.LoadInt( szKey, 0 );

		wsprintf_e( szKey, "alchemic_additive_name%d", i+1 );
		kLoader.LoadString( szKey, "", szBuf, MAX_PATH );
		kInfo.m_DescName = szBuf;

		wsprintf_e( szKey, "alchemic_additive_icon%d", i+1 );
		kLoader.LoadString( szKey, "", szBuf, MAX_PATH );
		kInfo.m_IconName = szBuf;

		m_AlchemicItemIconMap.insert( AlchemicItemIconMap::value_type( iCode, kInfo ) );
	}

	// disassemble
	LoadSoldierDisassembleInfo( kLoader );
	LoadExtraItemDisassembleInfo( kLoader );

	// new alchemic
	ioINILoader_e kLoader2( "config/sp2_new_alchemic_info.ini" );

	LoadNewAlchemicListTable( kLoader2 );
	LoadNewAlchemicInfo( kLoader2 );
}

void ioAlchemicMgr::LoadFuncInfo( ioINILoader &rkLoader )
{
	char szBuf[MAX_PATH] = "";
	AlchemicFuncInfo kFuncInfo;

	kFuncInfo.m_bActive = rkLoader.LoadBool_e( "alchemic_active", false );
	if( !kFuncInfo.m_bActive )
		return;

	kFuncInfo.m_iCode = rkLoader.LoadInt_e( "alchemic_code", 0 );
	kFuncInfo.m_iManaual = rkLoader.LoadInt_e( "alchemic_manaual", 0 );
	kFuncInfo.m_AlchemicType = (AlchemicType)rkLoader.LoadInt_e( "alchemic_type", ALT_NONE );

	kFuncInfo.m_iPriorityOrder = rkLoader.LoadInt_e( "alchemic_order", 1 );

	kFuncInfo.m_iMaxCnt1 = rkLoader.LoadInt_e( "alchemic_max_cnt1", 0 );
	kFuncInfo.m_iMaxCnt2 = rkLoader.LoadInt_e( "alchemic_max_cnt2", 0 );
	kFuncInfo.m_iMaxCnt3 = rkLoader.LoadInt_e( "alchemic_max_cnt3", 0 );
	kFuncInfo.m_iMaxCnt4 = rkLoader.LoadInt_e( "alchemic_max_cnt4", 0 );
	kFuncInfo.m_iMaxAdditive = rkLoader.LoadInt_e( "alchemic_max_additive", 0 );

	kFuncInfo.m_iPeriodValue = rkLoader.LoadInt_e( "alchemic_period_value", 0 );

	rkLoader.LoadString_e( "alchemic_desc", "", szBuf, MAX_PATH );
	kFuncInfo.m_Desc = szBuf;

	rkLoader.LoadString_e( "alchemic_desc2", "", szBuf, MAX_PATH );
	kFuncInfo.m_Desc2 = szBuf;

	kFuncInfo.m_iRandomListNum = rkLoader.LoadInt_e( "alchemic_random_num", 0 );

	kFuncInfo.m_iNewAlchemicListTable = rkLoader.LoadInt_e( "new_alchemic_table_num", 0 );

	m_AlchemicFuncInfoList.push_back( kFuncInfo );
}

void ioAlchemicMgr::LoadRandomInfo( ioINILoader &rkLoader )
{
	enum 
	{
		MAX_LOOP = 100,
	};

	char szKey[MAX_PATH]="";

	for (int i = 0; i < MAX_LOOP ; i++)
	{
		wsprintf_e( szKey, "random_table%d", i+1 );
		rkLoader.SetTitle( szKey );

		int iRandomCnt = rkLoader.LoadInt_e( "random_cnt", 0 );
		if( iRandomCnt <= 0 )
			break;

		RandomResultInfo kRandomInfo;

		int iCode;
		for( int j=0; j < iRandomCnt; ++j )
		{
			wsprintf_e( szKey, "random_result%d", j+1 );
			iCode = rkLoader.LoadInt( szKey, 0 );

			kRandomInfo.m_vList.push_back( iCode );
		}

		m_RandomMap.insert( RandomResultInfoMap::value_type( i+1, kRandomInfo ) );
	}
}

void ioAlchemicMgr::LoadSoldierDisassembleInfo( ioINILoader &rkLoader )
{
	m_SoldierDisassembleMap.clear();

	rkLoader.SetTitle_e( "soldier_disassemble_info" );

	char szKey[MAX_PATH]="";

	int iCnt = rkLoader.LoadInt_e( "disassemble_info_cnt", 0 );
	int iCode = 0;
	int iResultCode = 0;

	for( int i=0; i < iCnt; ++i )
	{
		wsprintf_e( szKey, "disassemble%d_code", i+1 );
		iCode = rkLoader.LoadInt( szKey, 0 );

		wsprintf_e( szKey, "disassemble%d_result", i+1 );
		iResultCode = rkLoader.LoadInt( szKey, 0 );

		m_SoldierDisassembleMap.insert( DisassembleInfoMap::value_type( iCode, iResultCode ) );
	}
}

void ioAlchemicMgr::LoadExtraItemDisassembleInfo( ioINILoader &rkLoader )
{
	m_ExtraItemDisassembleMap.clear();

	rkLoader.SetTitle_e( "extraitem_disassemble_info" );

	char szKey[MAX_PATH]="";

	int iCnt = rkLoader.LoadInt_e( "disassemble_info_cnt", 0 );
	int iCode = 0;
	int iResultCode = 0;

	for( int i=0; i < iCnt; ++i )
	{
		wsprintf_e( szKey, "disassemble%d_code", i+1 );
		iCode = rkLoader.LoadInt( szKey, 0 );

		wsprintf_e( szKey, "disassemble%d_result", i+1 );
		iResultCode = rkLoader.LoadInt( szKey, 0 );

		m_ExtraItemDisassembleMap.insert( DisassembleInfoMap::value_type( iCode, iResultCode ) );
	}
}

void ioAlchemicMgr::LoadNewAlchemicListTable( ioINILoader &rkLoader )
{
	char szBuf[MAX_PATH] = "";
	char szKey[MAX_PATH] = "";

	m_NewAlchemicListMap.clear();

	rkLoader.SetTitle_e( "new_alchemic_table" );

	int iTableCnt = rkLoader.LoadInt_e( "new_alchemic_table_cnt", 0 );
	for( int i=0; i < iTableCnt; ++i )
	{
		wsprintf_e( szKey, "alchemic_table%d_list_cnt", i+1 );
		int iListCnt = rkLoader.LoadInt( szKey, 0 );

		IntVec vValueList;
		for( int j=0; j < iListCnt; ++j )
		{
			wsprintf_e( szKey, "alchemic_table%d_value%d", i+1, j+1 );
			int iAlchemicValue = rkLoader.LoadInt( szKey, 0 );

			vValueList.push_back( iAlchemicValue );
		}

		m_NewAlchemicListMap.insert( AlcemicListMap::value_type( i+1, vValueList ) );
	}
}

bool ioAlchemicMgr::GetNewAlchemicList( IN int iCode, OUT IntVec &rkvList )
{
	int iTableNum = 0;
	rkvList.clear();

	vAlchemicFuncInfoList::iterator iter = m_AlchemicFuncInfoList.begin();
	while( iter != m_AlchemicFuncInfoList.end() )
	{
		if( (*iter).m_iCode == iCode )
		{
			iTableNum = (*iter).m_iNewAlchemicListTable;
			break;
		}

		++iter;
	}

	if( iTableNum > 0 )
	{
		AlcemicListMap::iterator iter = m_NewAlchemicListMap.find( iTableNum );
		if( iter != m_NewAlchemicListMap.end() )
		{
			int iListSize = iter->second.size();
			int iIndex = iListSize-1;

			while( COMPARE( iIndex, 0, iListSize ) )
			{
				rkvList.push_back( iter->second[iIndex] );
				
				--iIndex;
			}

			return true;
		}
	}

	return false;
}

void ioAlchemicMgr::LoadNewAlchemicInfo( ioINILoader &rkLoader )
{
	char szBuf[MAX_PATH] = "";
	char szKey[MAX_PATH] = "";

	m_NewAlchemicSoldierInfoMap.clear();
	m_NewAlchemicItemInfoMap.clear();

	rkLoader.SetTitle_e( "new_alchemic_info" );

	m_iSoldierAdditive = rkLoader.LoadInt_e( "soldier_additive", 0 );
	m_iWeaponAdditive = rkLoader.LoadInt_e( "weapon_additive", 0 );
	m_iArmorAdditive = rkLoader.LoadInt_e( "armor_additive", 0 );
	m_iHelmetAdditive = rkLoader.LoadInt_e( "helmet_additive", 0 );
	m_iCloakAdditive = rkLoader.LoadInt_e( "cloak_additive", 0 );
	m_iExchangeAdditive = rkLoader.LoadInt_e( "exchange_additive", 0 );

	m_iNewAlchemicMinTotalCnt = rkLoader.LoadInt_e( "new_alchemic_min_cnt", 1 );

	// soldier
	int iSoldierInfoCnt = rkLoader.LoadInt_e( "rare_soldier_info_cnt", 0 );
	for( int i=0; i < iSoldierInfoCnt; ++i )
	{
		wsprintf_e( szKey, "rare_soldier%d_result", i+1 );
		int iResultValue = rkLoader.LoadInt( szKey, 0 );

		NewAlchemicInfo kNewInfo;

		wsprintf_e( szKey, "rare_soldier%d_piece", i+1 );
		kNewInfo.m_iPieceNum = rkLoader.LoadInt( szKey, 0 );

		wsprintf_e( szKey, "rare_soldier%d_additive", i+1 );
		kNewInfo.m_iAdditiveNum = rkLoader.LoadInt( szKey, 0 );

		m_NewAlchemicSoldierInfoMap.insert( NewAlchemicInfoMap::value_type( iResultValue, kNewInfo ) );
	}

	// item
	int iItemInfoCnt = rkLoader.LoadInt_e( "rare_item_info_cnt", 0 );	
	for( int j=0; j < iItemInfoCnt; ++j )
	{
		wsprintf_e( szKey, "rare_item%d_result", j+1 );
		int iResultValue = rkLoader.LoadInt( szKey, 0 );

		NewAlchemicInfo kNewInfo;

		wsprintf_e( szKey, "rare_item%d_piece", j+1 );
		kNewInfo.m_iPieceNum = rkLoader.LoadInt( szKey, 0 );

		wsprintf_e( szKey, "rare_item%d_additive", j+1 );
		kNewInfo.m_iAdditiveNum = rkLoader.LoadInt( szKey, 0 );

		m_NewAlchemicItemInfoMap.insert( NewAlchemicInfoMap::value_type( iResultValue, kNewInfo ) );
	}
}

bool ioAlchemicMgr::FindAlchemicFunc( int iCode, AlchemicFuncInfo &rkFuncInfo )
{
	rkFuncInfo.Init();

	if( m_AlchemicFuncInfoList.empty() )
		return false;

	vAlchemicFuncInfoList::iterator iter = m_AlchemicFuncInfoList.begin();
	while( iter != m_AlchemicFuncInfoList.end() )
	{
		if( (*iter).m_iCode == iCode )
		{
			rkFuncInfo = (*iter);
			return true;
		}

		++iter;
	}

	return false;
}

bool ioAlchemicMgr::FindAlchemicFuncFirst( AlchemicFuncInfo &rkFuncInfo )
{
	rkFuncInfo.Init();

	if( m_AlchemicFuncInfoList.empty() )
		return false;

	rkFuncInfo = m_AlchemicFuncInfoList[0];
	return true;
}

bool ioAlchemicMgr::FindRandomResultInfo( int iNum, RandomResultInfo &rkRandomInfo )
{
	rkRandomInfo.Init();

	RandomResultInfoMap::iterator iter = m_RandomMap.find( iNum );
	if( iter != m_RandomMap.end() )
	{
		int iSize = iter->second.m_vList.size();
		int iCode;
		for( int i=0; i < iSize; ++i )
		{
			iCode = iter->second.m_vList[i];

			rkRandomInfo.m_vList.push_back( iCode );
		}

		return true;
	}

	return false;
}

ioHashString ioAlchemicMgr::GetAlchemicItemIcon( int iCode )
{
	AlchemicItemIconMap::iterator iter = m_AlchemicItemIconMap.find( iCode );
	if( iter != m_AlchemicItemIconMap.end() )
	{
		return iter->second.m_IconName;
	}

	return ioHashString();
}

ioHashString ioAlchemicMgr::GetAlchemicItemName( int iCode )
{
	AlchemicItemIconMap::iterator iter = m_AlchemicItemIconMap.find( iCode );
	if( iter != m_AlchemicItemIconMap.end() )
	{
		return iter->second.m_DescName;
	}

	return ioHashString();
}

bool ioAlchemicMgr::HasAlchemicFunc()
{
	if( m_AlchemicFuncInfoList.empty() )
		return false;

	return true;
}

bool ioAlchemicMgr::GetAlchemicFuncInfo( vAlchemicFuncInfoSList &rkInfoList )
{
	rkInfoList.clear();

	if( m_AlchemicFuncInfoList.empty() )
		return false;

	vAlchemicFuncInfoList::iterator iter = m_AlchemicFuncInfoList.begin();
	while( iter != m_AlchemicFuncInfoList.end() )
	{
		AlchemicFuncInfoS kInfo;
		kInfo.m_iCode = (*iter).m_iCode;
		kInfo.m_Desc = (*iter).m_Desc;

		rkInfoList.push_back( kInfo );

		++iter;
	}

	if( rkInfoList.empty() )
		return false;

	return true;
}

ioAlchemicMgr& ioAlchemicMgr::GetSingleton()
{
	return Singleton< ioAlchemicMgr >::GetSingleton();
}

int ioAlchemicMgr::GetDisassembleMinCnt( int iType, bool bMortmain, DWORD dwTime, DWORD dwMagicValue )
{
	int iCount = 0;
	DWORD dwCurTime = dwTime;

	DWORD dwMinCnt = 0;

	switch( iType )
	{
	case ADT_SOLDIER:
		{
			if( bMortmain )
				dwCurTime = m_dwSoldierDisassembleConst;

			dwMinCnt = dwCurTime * FLOAT1;
		}
		break;
	case ADT_EXTRAITEM:
		{
			if( bMortmain )
				dwCurTime = m_dwExtraItemDisassembleConst;

			int iCurReinforce = dwMagicValue;
			if( iCurReinforce < 10 )
				iCurReinforce = 10;

			float fReinforceRate = (float)iCurReinforce/FLOAT10;

			dwMinCnt = dwCurTime * FLOAT1;
		}
		break;
	}

	return dwMinCnt;
}

int ioAlchemicMgr::GetDisassembleMaxCnt( int iType, bool bMortmain, DWORD dwTime, DWORD dwMagicValue )
{
	int iCount = 0;
	DWORD dwCurTime = dwTime;

	DWORD dwMaxCnt = 0;

	switch( iType )
	{
	case ADT_SOLDIER:
		{
			if( bMortmain )
				dwCurTime = m_dwSoldierDisassembleConst;

			dwMaxCnt = dwCurTime * 2.0f;
		}
		break;
	case ADT_EXTRAITEM:
		{
			if( bMortmain )
				dwCurTime = m_dwExtraItemDisassembleConst;

			int iCurReinforce = dwMagicValue;
			if( iCurReinforce < 10 )
				iCurReinforce = 10;

			float fReinforceRate = (float)iCurReinforce/FLOAT10;

			dwMaxCnt = dwCurTime * fReinforceRate * 2.0f;
		}
		break;
	}

	return dwMaxCnt;
}

int ioAlchemicMgr::GetDisassembleCode( int iType, DWORD dwMagicCode )
{
	DisassembleInfoMap::iterator iter;
	int iCode = 0;

	switch( iType )
	{
	case ADT_SOLDIER:
		iter = m_SoldierDisassembleMap.find( dwMagicCode );

		if( iter != m_SoldierDisassembleMap.end() )
			iCode = iter->second;
		else
			iCode = dwMagicCode;
		break;
	case ADT_EXTRAITEM:
		iter = m_ExtraItemDisassembleMap.find( dwMagicCode );

		if( iter != m_ExtraItemDisassembleMap.end() )
			iCode = iter->second;
		else
			iCode = dwMagicCode%1000;

		break;
	}

	return iCode;
}

bool ioAlchemicMgr::FindNewAlchemicSoldierInfo( IN int iClassType, OUT int &iPiece, OUT int &iAdditive )
{
	NewAlchemicInfoMap::iterator iter = m_NewAlchemicSoldierInfoMap.find( iClassType );

	if( iter != m_NewAlchemicSoldierInfoMap.end() )
	{
		iPiece = iter->second.m_iPieceNum;
		iAdditive = iter->second.m_iAdditiveNum;
	}
	else
	{
		iPiece = iClassType;
		iAdditive = m_iSoldierAdditive;
	}
	
	return true;
}

bool ioAlchemicMgr::FindNewAlchemicItemInfo( IN int iItemCode, OUT int &iPiece, OUT int &iAdditive )
{
	NewAlchemicInfoMap::iterator iter = m_NewAlchemicItemInfoMap.find( iItemCode );

	if( iter != m_NewAlchemicItemInfoMap.end() )
	{
		iPiece = iter->second.m_iPieceNum;
		iAdditive = iter->second.m_iAdditiveNum;
	}
	else
	{
		iPiece = iItemCode % 1000;
		int iSlot = iItemCode / DEFAULT_BASIC_ITEM_CODE;

		switch( iSlot )
		{
		case 0:
			iAdditive = m_iWeaponAdditive;
			break;
		case 1:
			iAdditive = m_iArmorAdditive;
			break;
		case 2:
			iAdditive = m_iHelmetAdditive;
			break;
		case 3:
			iAdditive = m_iCloakAdditive;
			break;
		}
	}
	
	return true;
}

bool ioAlchemicMgr::CheckEnableNewAlchemicSoldier( int iClassType )
{
	ioAlchemicInventory *pInven = g_MyInfo.GetAlchemicInventory();
	if( !pInven )
		return false;

	NewAlchemicInfoMap::iterator iter = m_NewAlchemicSoldierInfoMap.find( iClassType );

	int iPiece = 0;
	int iAdditive = 0;

	if( iter != m_NewAlchemicSoldierInfoMap.end() )
	{
		iPiece = iter->second.m_iPieceNum;
		iAdditive = iter->second.m_iAdditiveNum;
	}
	else
	{
		iPiece = iClassType;
		iAdditive = m_iSoldierAdditive;
	}

	if( pInven->HasAlchemicItem( iPiece ) && pInven->HasAlchemicItem( iAdditive ) )
		return true;
	
	return false;
}

bool ioAlchemicMgr::CheckEnableNewAlchemicItem( int iItemCode )
{
	ioAlchemicInventory *pInven = g_MyInfo.GetAlchemicInventory();
	if( !pInven )
		return false;

	NewAlchemicInfoMap::iterator iter = m_NewAlchemicItemInfoMap.find( iItemCode );

	int iPiece = 0;
	int iAdditive = 0;

	if( iter != m_NewAlchemicItemInfoMap.end() )
	{
		iPiece = iter->second.m_iPieceNum;
		iAdditive = iter->second.m_iAdditiveNum;
	}
	else
	{
		iPiece = iItemCode % 1000;
		int iSlot = iItemCode / DEFAULT_BASIC_ITEM_CODE;

		switch( iSlot )
		{
		case 0:
			iAdditive = m_iWeaponAdditive;
			break;
		case 1:
			iAdditive = m_iArmorAdditive;
			break;
		case 2:
			iAdditive = m_iHelmetAdditive;
			break;
		case 3:
			iAdditive = m_iCloakAdditive;
			break;
		}
	}

	if( pInven->HasAlchemicItem( iPiece ) && pInven->HasAlchemicItem( iAdditive ) )
		return true;

	return false;
}

int ioAlchemicMgr::GetRareSoldierPiece( DWORD dwCode )
{
	NewAlchemicInfoMap::iterator iter = m_NewAlchemicSoldierInfoMap.find( dwCode );
	if( iter != m_NewAlchemicSoldierInfoMap.end() )
	{			
		return iter->second.m_iPieceNum;			
	}

	return dwCode;
}

int ioAlchemicMgr::GetRareSoldierAdditive( DWORD dwCode )
{
	NewAlchemicInfoMap::iterator iter = m_NewAlchemicSoldierInfoMap.find( dwCode );
	if( iter != m_NewAlchemicSoldierInfoMap.end() )
	{			
		return iter->second.m_iAdditiveNum;
	}

	return dwCode;
}