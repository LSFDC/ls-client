#include "stdafx.h"
#include "ioSoldierAwakeManager.h"
#include "ioModeSelectInfo.h"
#include "ioTime.h"
#include "ioBasicTooltipManager.h"
#include "ioPlayStage.h"
#include "ioBaseChar.h"
#include "GUI/MyInventoryInfoWnd.h"
#include "GUI/ioPullDownList.h"
#include "GUI/MannerValuationWnd.h"
#include "GUI/UserInfoToolTip.h"

template<> ioSoldierAwakeManager* Singleton< ioSoldierAwakeManager >::ms_Singleton = 0;
ioSoldierAwakeManager& ioSoldierAwakeManager::GetSingleton()
{
	return Singleton< ioSoldierAwakeManager >::GetSingleton();
}

ioSoldierAwakeManager::ioSoldierAwakeManager()
{
	m_nMaxAwakeLimit = 0;
}

ioSoldierAwakeManager::~ioSoldierAwakeManager()
{
}

void ioSoldierAwakeManager::Initialize()
{
	ioINILoader_e kLoader( "config/sp2_char_awake_info.ini" );

	kLoader.SetTitle_e( "common" );
	int nMaxAwake	 = kLoader.LoadInt_e( "max_awake", 0 );
	m_nMaxAwakeLimit = kLoader.LoadInt_e( "max_awake_period_limit", 0 );

	char szBuff[MAX_PATH];
	for( int i = 1; i <= nMaxAwake; ++i )
	{
		AwakeInfo Info;
		sprintf_s_e( szBuff, "awake%d", i );
		kLoader.SetTitle( szBuff );
		LoadGrowth( kLoader, Info );
		LoadUI( kLoader, Info );
		LoadEffect( kLoader, Info );

		Info.eAwakeType = static_cast<AwakeType>( i );
		m_AwakeInfoMap.insert( AwakeInfoMap::value_type( i, Info ) );
	}

	LoadMtrl( kLoader );
	LoadAwakeProduct( kLoader );
}

void ioSoldierAwakeManager::LoadGrowth( ioINILoader& rkLoader, AwakeInfo& rkInfo )
{
	char szBuff[MAX_PATH];

	for( int i = 0; i < MAX_CHAR_GROWTH; ++i )
	{
		sprintf_s_e( szBuff, "char_growth_%d", i+1 );
		rkInfo.nCharGrowth[i] = rkLoader.LoadInt( szBuff, 0 );
	}

	for( int i = 0; i < MAX_ITEM_GROWTH; ++i )
	{
		sprintf_s_e( szBuff, "item_growth_%d", i+1 );
		rkInfo.nItemGrowth[i] = rkLoader.LoadInt( szBuff, 0 );
	}
}

void ioSoldierAwakeManager::LoadUI( ioINILoader& rkLoader, AwakeInfo& rkInfo )
{
	//UI ������������ �����ڵ� ��� X
	char szBuff[MAX_PATH];
	rkLoader.LoadString( "name", "", szBuff, MAX_PATH );
	rkInfo.szName = szBuff;

	rkInfo.dwNameColor = rkLoader.LoadInt( "name_color", 0 );

	rkLoader.LoadString( "tooltip", "", szBuff, MAX_PATH );
	rkInfo.szToopTipFile = szBuff;
	g_TooltipMgr.NewReserveToolTip( szBuff );
	
}

void ioSoldierAwakeManager::LoadEffect( ioINILoader& rkLoader, AwakeInfo& rkInfo )
{
	//U������ �����ڵ� ��� X
	char szBuff[MAX_PATH];
	rkLoader.LoadString( "effect", "", szBuff, MAX_PATH );
	rkInfo.szEffect = szBuff;
}

void ioSoldierAwakeManager::LoadMtrl( ioINILoader& rkLoader )
{
	rkLoader.SetTitle_e( "common" );
	int nMaxMtrl = rkLoader.LoadInt_e( "max_material", 0 );
	
	char szBuff[MAX_PATH];
	for( int i = 1; i <= nMaxMtrl; ++i )
	{
		sprintf_s_e( szBuff, "material%d", i );
		rkLoader.SetTitle( szBuff );

		NeedMaterial Mtrl;
		Mtrl.eType		= static_cast<NeedMaterialType>( rkLoader.LoadInt_e( "material_type", static_cast<int>( ALT_NONE ) ) );		
		Mtrl.dwNeedCode	= rkLoader.LoadInt_e( "material_code", 0 );
		Mtrl.nNeedCount	= rkLoader.LoadInt_e( "material_need_cnt", 0 );

		m_NeedMaterialMap.insert( NeedMaterialMap::value_type( i, Mtrl ) );
	}
}

void ioSoldierAwakeManager::LoadAwakeProduct( ioINILoader& rkLoader )
{
	char szBuff[MAX_PATH];
	rkLoader.SetTitle_e( "common" );
	int nMaxMtrl = rkLoader.LoadInt_e( "max_awake_product", 0 );

	for( int i = 1; i <= nMaxMtrl; ++i )
	{
		sprintf_s_e( szBuff, "awake_product%d", i );
		rkLoader.SetTitle( szBuff );

		AwakeProduct sProduct;
		sProduct.nNeedDay				= rkLoader.LoadInt_e( "need_day", 0 );
		sProduct.nNeedMtrlIdx			= rkLoader.LoadInt_e( "need_mtrl", 0 );
		sProduct.nNeedAwakeIdx			= rkLoader.LoadInt_e( "need_awake", 0 );
		
		//UI������
		sProduct.sMainTitle.dwTitleColor	= rkLoader.LoadInt( "menu_color", 0 );
		sProduct.sMainTitle.iTitleSize		= rkLoader.LoadInt( "menu_size", 0 );				
		sProduct.sSubTitle.dwTitleColor		= rkLoader.LoadInt( "submenu_color", 0 );
		sProduct.sSubTitle.iTitleSize		= rkLoader.LoadInt( "submenu_size", 0 );

		GetAwakeTtitle( sProduct.sMainTitle.szTitle, sProduct.nNeedAwakeIdx, sProduct.nNeedDay );

		m_AwakeProductMap.insert( AwakeProductMap::value_type( i, sProduct ) );
	}
}

const NeedMaterial* ioSoldierAwakeManager::GetMtrl( int nIdx ) const
{
	NeedMaterialMap::const_iterator iter = m_NeedMaterialMap.find( nIdx );
	if( iter != m_NeedMaterialMap.end() )
	{
		return &iter->second;
	}

	return NULL;
}

const AwakeInfo* ioSoldierAwakeManager::GetAwake( int nIdx ) const
{
	AwakeInfoMap::const_iterator iter = m_AwakeInfoMap.find( nIdx );
	if( iter != m_AwakeInfoMap.end() )
	{
		return &iter->second;
	}

	return NULL;
}

const AwakeInfo* ioSoldierAwakeManager::GetAwakeByType( AwakeType eType ) const
{
	AwakeInfoMap::const_iterator iter = m_AwakeInfoMap.begin();
	for( ; iter != m_AwakeInfoMap.end(); ++iter )
	{
		if( iter->second.eAwakeType == eType )
		{
			return &iter->second;
		}
	}

	return NULL;
}

const AwakeProduct* ioSoldierAwakeManager::GetAwakeProduct( int nIdx ) const
{
	AwakeProductMap::const_iterator iter = m_AwakeProductMap.find( nIdx );
	if( iter != m_AwakeProductMap.end() )
	{
		return &iter->second;
	}

	return NULL;
}

const std::string& ioSoldierAwakeManager::GetAwakeEffect( AwakeType eType ) const
{
	static std::string szNull;
	AwakeInfoMap::const_iterator iter = m_AwakeInfoMap.begin();
	for( ; iter != m_AwakeInfoMap.end(); ++iter )
	{
		if( iter->second.eAwakeType == eType )
		{
			return iter->second.szEffect;
		}
	}

	return szNull;
}

void ioSoldierAwakeManager::GetMtrlTitle( std::string& szTtitle, int nIdx ) const
{
	char szBuff[MAX_PATH];
	const NeedMaterial* pMtrl = GetMtrl( nIdx );
	if( pMtrl )
	{
		std::string szName = g_PresentMgr.GetPresentValue1Text( PRESENT_ALCHEMIC_ITEM, pMtrl->dwNeedCode, 0 ).c_str();
		SafeSprintf( szBuff, sizeof(szBuff), STR(1), szName.c_str(), pMtrl->nNeedCount );
		szTtitle = szBuff;
	}
	else
	{
		szTtitle.clear();
	}
}

void ioSoldierAwakeManager::GetClassPieceTitle( std::string& szTtitle, int iClassType, int iNeedCount ) const
{
	char szBuff[MAX_PATH];
	int iPiece		= 0;
	int iAdditive	= 0;
	g_AlchemicMgr.FindNewAlchemicSoldierInfo( iClassType, iPiece, iAdditive );
		
	SafeSprintf( szBuff, sizeof(szBuff), STR(1), g_PresentMgr.GetPresentValue1Text( PRESENT_ALCHEMIC_ITEM, iPiece, 0 ).c_str(), iNeedCount );
	szTtitle = szBuff;
}

void ioSoldierAwakeManager::GetAwakeTtitle( std::string& szTtitle, int nIdx, int nPeriod ) const
{
	char szBuff[MAX_PATH];
	const AwakeInfo* pAwake = GetAwake( nIdx );
	if( pAwake )
	{
		SafeSprintf( szBuff, sizeof(szBuff), STR(1), pAwake->szName.c_str(), nPeriod );
		szTtitle = szBuff;
	}
	else
	{
		szTtitle.clear();
	}
}

int ioSoldierAwakeManager::GetAwakeMtrlCode( int iType, DWORD dwCode ) const
{
	int iCode = 0;
	switch( iType )
	{
	case NMT_PIECE:
		iCode = g_AlchemicMgr.GetRareSoldierPiece( dwCode );
		break;
	case NMT_ADDITIVE:
		iCode = g_AlchemicMgr.GetRareSoldierAdditive( dwCode );
		break;
	}

	return iCode;
}

int ioSoldierAwakeManager::GetAwakeCharGrowth( AwakeType eType, int iArray )
{	
	if( eType == AWAKE_NONE )
		return 0;

	const AwakeInfo* pAwake = GetAwakeByType( static_cast<AwakeType>( eType ) );
	if( !pAwake )
		return 0;

	if( COMPARE( iArray, 0, MAX_CHAR_GROWTH ) )
		return pAwake->nCharGrowth[iArray];

	return 0;
}

int ioSoldierAwakeManager::GetAwakeItemGrowth( AwakeType eType, int iArray )
{	
	if( eType == AWAKE_NONE )
		return 0;

	const AwakeInfo* pAwake = GetAwakeByType( static_cast<AwakeType>( eType ) );
	if( !pAwake )
		return 0;

	if( COMPARE( iArray, 0, MAX_ITEM_GROWTH ) )
		return pAwake->nItemGrowth[iArray];

	return 0;
}

const std::string& ioSoldierAwakeManager::GetAwakeName( AwakeType eType ) const
{
	AwakeInfoMap::const_iterator iter = m_AwakeInfoMap.begin();
	for( ; iter != m_AwakeInfoMap.end(); ++iter )
	{
		if( iter->second.eAwakeType == eType )
		{
			return iter->second.szName;
		}
	}

	static std::string szNull;
	return szNull;
}

void ioSoldierAwakeManager::GetRemainTime( int iLimitTime, std::string& szText )
{
	char szBuff[MAX_PATH];

	//���� ���� ���� �ð�
	SYSTEMTIME rkServerTime;
	g_MyInfo.GetServerTime( rkServerTime );
	CTime NowTime( rkServerTime.wYear, rkServerTime.wMonth, rkServerTime.wDay, rkServerTime.wHour, rkServerTime.wMinute, 0 );

	//���� ���� �ð�
	CTime LimitTime( ioTime::GetYear( iLimitTime ), ioTime::GetMonth( iLimitTime ), ioTime::GetDay( iLimitTime ), ioTime::GetHour( iLimitTime ), ioTime::GetMinute( iLimitTime ), 0 );

	//���� ���� �ð�
	CTimeSpan GapTime = LimitTime - NowTime;

	//UI �ؽ�Ʈ
	int iRestHour = GapTime.GetTotalHours();		
	if( iRestHour < 1 )
	{
		int iRestMin = GapTime.GetTotalMinutes();
		if( iRestMin <= 1 )
			SafeSprintf( szBuff, sizeof(szBuff), STR(1), iRestMin );
		else
			SafeSprintf( szBuff, sizeof(szBuff), STR(2),iRestMin );
	}
	else if( iRestHour < 24 )
	{
		SafeSprintf( szBuff, sizeof(szBuff), STR(3), iRestHour );
	}
	else
	{
		SafeSprintf( szBuff, sizeof(szBuff), STR(4), iRestHour / 24, iRestHour % 24 );
	}

	szText = szBuff;
}

void ioSoldierAwakeManager::GetMenuListName( std::string& szName, int iAwakeType, int iLimitTime )
{
	if( iAwakeType == AWAKE_NONE )
	{
		if( m_AwakeInfoMap.empty() )
		{
			szName =STR(1);
		}
		else
		{
			AwakeInfoMap::iterator iter = m_AwakeInfoMap.begin();
			szName = iter->second.szName;
		}
	}
	else
	{
		const AwakeInfo* pAwake = GetAwakeByType( static_cast<AwakeType>( iAwakeType ) );
		if( pAwake )
		{
			szName = pAwake->szName;
		}
		
		std::string szRemainTime;
		GetRemainTime( iLimitTime, szRemainTime );

		char szBuff[MAX_PATH];
		sprintf_s( szBuff, "%s %s", szName.c_str(), szRemainTime.c_str() );
		szName = szBuff;
	}
}

void ioSoldierAwakeManager::GetMenuList( IN ioPullDownList* pList, int iClassType )
{
	if( pList == NULL )
		return;
	
	AwakeProductMap::iterator iter = m_AwakeProductMap.begin();
	for( int i = 1; iter != m_AwakeProductMap.end(); ++iter )
	{
		AwakeProduct& rkProduct = iter->second;	

		CharAwakePullDownItem* pPullDown = new CharAwakePullDownItem;		

		const AwakeInfo* pInfo		= GetAwake( rkProduct.nNeedAwakeIdx );
		const NeedMaterial* pMtrl	= GetMtrl( rkProduct.nNeedMtrlIdx );
		if( pMtrl && pInfo )
		{
			//��� Ÿ�Ժ� ����Ÿ��Ʋ ����
			switch( pMtrl->eType )
			{
			case NMT_PIECE:
				GetClassPieceTitle( rkProduct.sSubTitle.szTitle, iClassType, pMtrl->nNeedCount );
				break;
			case NMT_ADDITIVE:
				GetMtrlTitle( rkProduct.sSubTitle.szTitle, rkProduct.nNeedMtrlIdx );
				break;
			}

			//�޴� �̸� �� ��ǰ���� �Ҵ�
			pPullDown->m_iProductIdx		= iter->first;
			pPullDown->m_szTitle			= rkProduct.sMainTitle.szTitle;
			pPullDown->m_szSubTitle			= rkProduct.sSubTitle.szTitle;
			pPullDown->m_dwTitleSubColor	= TCT_DEFAULT_BLUE;

			//���� �Ұ��� ���� ���� ����
			const CHARACTER& rkSelectChar = g_MyInfo.GetCharacterToClassType( iClassType );		
			if( rkSelectChar.m_iAwakeType == AWAKE_RARE && pInfo->eAwakeType == AWAKE_NORMAL )
			{
				pPullDown->m_dwTitleColor		= TCT_DEFAULT_LIGHTGRAY;
				pPullDown->m_dwTitleSubColor	= TCT_DEFAULT_LIGHTGRAY;
			}

			//���� �Ҵ�	
			pPullDown->m_iTooltipIdx = g_TooltipMgr.GetToolTipID( pInfo->szToopTipFile );
		}
		pList->AddList( pPullDown );
	}
}

byte ioSoldierAwakeManager::GetAwakeType( const CharAwakePullDownItem* pItem ) const
{
	const AwakeProduct* pProduct = GetAwakeProduct( pItem->m_iProductIdx );
	if( !pProduct )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return AWAKE_NONE;
	}

	const AwakeInfo* pAwake = GetAwake( pProduct->nNeedAwakeIdx );
	if( !pAwake )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return AWAKE_NONE;
	}

	return pAwake->eAwakeType;
}

AWAKERESULT ioSoldierAwakeManager::IsEnableAwakeMtrlUse( const CharAwakePullDownItem* pItem, int iClassType ) const
{
	const AwakeProduct* pProduct = GetAwakeProduct( pItem->m_iProductIdx );
	if( !pProduct )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return AR_ERROR;
	}

	const NeedMaterial* pMtrl = GetMtrl( pProduct->nNeedMtrlIdx );
	if( !pMtrl )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return AR_ERROR;
	}
	
	ioAlchemicInventory* pInven = g_MyInfo.GetAlchemicInventory();
	if( !pInven )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		return AR_ERROR;
	}
	
	//�뺴 ����
	if( pMtrl->eType == NMT_PIECE ) 
	{
		int iPiece		= 0;
		int iAdditive	= 0;
		g_AlchemicMgr.FindNewAlchemicSoldierInfo( iClassType, iPiece, iAdditive );

		if( !pInven->HasAlchemicItem( iPiece ) )
			return AR_FAIL;

		if( pInven->GetAlchemicItemCnt( iPiece ) < pMtrl->nNeedCount )
			return AR_FAIL;
	}
	//�뺴���� �� ÷���� ��
	else
	{
		if( !pInven->HasAlchemicItem( pMtrl->dwNeedCode ) )
			return AR_FAIL;

		if( pInven->GetAlchemicItemCnt( pMtrl->dwNeedCode ) < pMtrl->nNeedCount )
			return AR_FAIL;
	}

	return AR_OK;
}

void ioSoldierAwakeManager::OnAwakeMenuToolTipShow( const OnOverPullDownEvent& Event, int iCharArray ) const
{
	const ioPullDownList* pList = Event.GetEventCaller();
	if( !pList )
		return;

	const CharAwakePullDownItem* pItem = (const CharAwakePullDownItem*)( pList->GetPullDown( Event.iOverIdx ) );
	if( !pItem )
		return;

	g_TooltipMgr.ShowToolTip( pItem->m_iTooltipIdx );
}

void ioSoldierAwakeManager::OnAwakeMenuToolTipHide( const OnLeavePullDownEvent& Event, int iCharArray ) const
{
	const ioPullDownList* pList = Event.GetEventCaller();
	if( !pList )
		return;

	const CharAwakePullDownItem* pItem = (const CharAwakePullDownItem*)( pList->GetPullDown( Event.iLeaveIdx ) );
	if( !pItem )
		return;

	g_TooltipMgr.HideToolTip( pItem->m_iTooltipIdx );
}

bool ioSoldierAwakeManager::AwakeTryMessageBox( const ClickPullDownEvent& Event, int iCharArray, ioWnd* pCalledWnd )
{
	const ioPullDownList* pList = Event.GetEventCaller();
	if( !pList )
		return false;

	const CharAwakePullDownItem* pItem = (const CharAwakePullDownItem*)( pList->GetPullDown( Event.iClickIdx ) );
	if( !pItem )
		return false;

	const CHARACTERDATA* pChar = g_MyInfo.GetCharacterData( iCharArray );
	if( !pChar )
		return false;
	
	const AwakeProduct* pProduct = GetAwakeProduct( pItem->m_iProductIdx );
	if( !pProduct )	
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return false;
	}
	
	const AwakeInfo* pInfo = GetAwake( pProduct->nNeedAwakeIdx );
	if( !pInfo )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return false;
	}

	const NeedMaterial* pMtrl = GetMtrl( pProduct->nNeedMtrlIdx );
	if( !pMtrl )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		return false;
	}

	ioAlchemicInventory* pInven = g_MyInfo.GetAlchemicInventory();
	if( !pInven )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		return false;
	}

	if( pInfo->eAwakeType == AWAKE_NORMAL && pChar->m_data.m_iAwakeType == AWAKE_RARE )
	{
		AwakeChangeFailMessageBox( GetAwakeName( static_cast<AwakeType>( static_cast<AwakeType>( pChar->m_data.m_iAwakeType ) ) ), GetAwakeName( pInfo->eAwakeType ) );
		return false;
	}

	int iCode = 0;

	//��� Ÿ��Ʋ
	std::string szMtrlTtitle;	
	switch( pMtrl->eType )
	{
	case NMT_PIECE:
		{
			GetClassPieceTitle( szMtrlTtitle, pChar->m_data.m_class_type, pMtrl->nNeedCount );			
			
			int iPiece		= 0;
			int iAdditive	= 0;	
			g_AlchemicMgr.FindNewAlchemicSoldierInfo( pChar->m_data.m_class_type, iPiece, iAdditive );
			iCode = iPiece;
		}
		break;
	case NMT_ADDITIVE:
		{
			GetMtrlTitle( szMtrlTtitle, pProduct->nNeedMtrlIdx );
			iCode = pMtrl->dwNeedCode;
		}
		break;
	}

	int iArray = 0;
	ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
	kPrinter[iArray].SetTextStyle( TS_NORMAL );
	kPrinter[iArray].SetBkColor( 0, 0, 0 );
	kPrinter[iArray].SetTextColor( TCT_DEFAULT_GREEN );
	kPrinter[iArray].AddTextPiece( FONT_SIZE_13, STR(5), pProduct->nNeedDay );
	kPrinter[iArray].SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter[iArray].AddTextPiece( FONT_SIZE_13, STR(6), pInfo->szName.c_str() );
	iArray++;
	

	if( pInfo->eAwakeType == AWAKE_RARE && pChar->m_data.m_iAwakeType == AWAKE_NORMAL )
	{
		std::string szAwakeTitle = GetAwakeName( static_cast<AwakeType>( pChar->m_data.m_iAwakeType ) );
		kPrinter[iArray].SetTextStyle( TS_NORMAL );
		kPrinter[iArray].SetBkColor( 0, 0, 0 );
		kPrinter[iArray].SetTextColor( TCT_DEFAULT_RED );
		kPrinter[iArray].AddTextPiece( FONT_SIZE_13, STR(7), szAwakeTitle.c_str() );
		iArray++;
		kPrinter[iArray].AddTextPiece( FONT_SIZE_13, "            " );
		iArray++;
	}
	else if( pChar->m_data.m_iAwakeType != AWAKE_NONE )
	{
		kPrinter[iArray].SetTextStyle( TS_NORMAL );
		kPrinter[iArray].SetBkColor( 0, 0, 0 );
		kPrinter[iArray].SetTextColor( TCT_DEFAULT_BLUE );
		kPrinter[iArray].AddTextPiece( FONT_SIZE_13, STR(8), pProduct->nNeedDay );
		iArray++;
		kPrinter[iArray].AddTextPiece( FONT_SIZE_13, "            " );
		iArray++;
	}

	kPrinter[iArray].SetTextStyle( TS_NORMAL );
	kPrinter[iArray].SetBkColor( 0, 0, 0 );
	kPrinter[iArray].SetTextColor( TCT_DEFAULT_BLUE );
	kPrinter[iArray].AddTextPiece( FONT_SIZE_13, STR(9), szMtrlTtitle.c_str() );
	iArray++;

	char szCount[MAX_PATH];
	Help::ConvertNumToStrComma( pInven->GetAlchemicItemCnt( iCode ), szCount, sizeof( szCount ) );
	kPrinter[iArray].SetTextStyle( TS_NORMAL );
	kPrinter[iArray].SetBkColor( 0, 0, 0 );
	kPrinter[iArray].SetTextColor( TCT_DEFAULT_GRAY );	
	kPrinter[iArray].AddTextPiece( FONT_SIZE_13, STR(10), szCount );
	iArray++;

	g_GUIMgr.SetPrevMsgListBoxWithTitle( NULL, MB_YESNO, pCalledWnd, kPrinter, pInfo->szName.c_str(), STR(11), STR(12), STR(13) );

	return true;
}

void ioSoldierAwakeManager::AwakeChangeFailMessageBox( const std::string& szOrgName, const std::string& szChangeName )
{
	ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
	kPrinter[0].SetTextStyle( TS_NORMAL );
	kPrinter[0].SetBkColor( 0, 0, 0 );
	kPrinter[0].SetTextColor( TCT_DEFAULT_RED );	
	kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(1), szOrgName.c_str() );

	kPrinter[1].SetTextStyle( TS_NORMAL );
	kPrinter[1].SetBkColor( 0, 0, 0 );
	kPrinter[1].SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(2), szChangeName.c_str() );

	g_GUIMgr.SetPrevMsgListBoxWithTitle( NULL, MB_OK, NULL, kPrinter, szChangeName.c_str(), STR(3), STR(4), STR(5) );
}

void ioSoldierAwakeManager::AwakeResultMessageBox( const std::string& szName, int iLimitTime )
{
	ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
	kPrinter[0].SetTextStyle( TS_NORMAL );
	kPrinter[0].SetBkColor( 0, 0, 0 );
	kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );	
	kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(1) );

	std::string szRemainTime;
	GetRemainTime( iLimitTime, szRemainTime );
	kPrinter[1].SetTextStyle( TS_NORMAL );
	kPrinter[1].SetBkColor( 0, 0, 0 );
	kPrinter[1].SetTextColor( TCT_DEFAULT_GREEN );
	kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(2), szRemainTime.c_str() );

	g_GUIMgr.SetPrevMsgListBoxWithTitle( NULL, MB_OK, NULL, kPrinter, szName.c_str(), STR(3), STR(4), STR(5) );
}

void ioSoldierAwakeManager::AwakeCharPeriodTypeError( const std::string& szName ) const
{
	ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
	kPrinter[0].SetTextStyle( TS_NORMAL );
	kPrinter[0].SetBkColor( 0, 0, 0 );
	kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );	
	kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(1) );
	
	kPrinter[1].SetTextStyle( TS_NORMAL );
	kPrinter[1].SetBkColor( 0, 0, 0 );
	kPrinter[1].SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(2), szName.c_str() );

	g_GUIMgr.SetPrevMsgListBoxWithTitle( NULL, MB_OK, NULL, kPrinter, szName.c_str(), STR(3), STR(4), STR(5) );
}

void ioSoldierAwakeManager::AwakeCharExerciseTypeError( const std::string& szName ) const
{
	ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
	kPrinter[0].SetTextStyle( TS_NORMAL );
	kPrinter[0].SetBkColor( 0, 0, 0 );
	kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );	
	kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(1) );

	kPrinter[1].SetTextStyle( TS_NORMAL );
	kPrinter[1].SetBkColor( 0, 0, 0 );
	kPrinter[1].SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(2), szName.c_str() );

	g_GUIMgr.SetPrevMsgListBoxWithTitle( NULL, MB_OK, NULL, kPrinter, szName.c_str(), STR(3), STR(4), STR(5) );
}

void ioSoldierAwakeManager::OnAwakeMenuEvent( const ClickPullDownEvent& Event, int iCharArray ) const
{
	const ioPullDownList* pList = Event.GetEventCaller();
	if( !pList )
		return;

	const CharAwakePullDownItem* pItem = (const CharAwakePullDownItem*)( pList->GetPullDown( Event.iClickIdx ) );
	if( !pItem )
		return;

	const CHARACTERDATA* pChar = g_MyInfo.GetCharacterData( iCharArray );
	if( !pChar )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return;
	}

	//���� ��Ḧ ������ �ִ��� �˻�
	AWAKERESULT eReuslt = g_AwakeMgr.IsEnableAwakeMtrlUse( pItem, pChar->m_data.m_class_type );
	if( eReuslt == AR_ERROR )
	{
		return;
	}
	else if( eReuslt == AR_FAIL )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return;
	}

	byte eType = GetAwakeType( pItem );
	if( eType != AWAKE_NONE && pChar->m_data.m_iAwakeType == AWAKE_NONE )
	{
		//���� �߰�
		SetAwake( pItem, pChar );
	}
	else if( eType != AWAKE_NONE && pChar->m_data.m_iAwakeType == eType )
	{
		//�Ⱓ ����
		ExtendAwake( pItem, pChar );
	}
	else if( eType == AWAKE_RARE && pChar->m_data.m_iAwakeType == AWAKE_NORMAL )
	{
		//�븻 ���� -> �ʿ�
		SetAwake( pItem, pChar );
	}
	else
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
	}
}

void ioSoldierAwakeManager::SetAwake( const CharAwakePullDownItem* pItem, const CHARACTERDATA* pChar ) const
{
	if( !pChar )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
	}

	const AwakeProduct* pProduct = GetAwakeProduct( pItem->m_iProductIdx );
	if( !pProduct )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return;
	}

	const AwakeInfo* pAwake = GetAwake( pProduct->nNeedAwakeIdx );
	if( !pAwake )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		return;
	}
	
	if( pChar->m_data.m_chExerciseStyle != EXERCISE_NONE )
	{
		AwakeCharExerciseTypeError( pAwake->szName );
		return;
	}

	if( pChar->m_data.m_ePeriodType != CPT_MORTMAIN )
	{
		AwakeCharPeriodTypeError( pAwake->szName );
		return;
	}

	SP2Packet kPacket( CTPK_CHAR_AWAKE );
	kPacket << pChar->m_dwIndex;
	kPacket << static_cast<byte>( pAwake->eAwakeType );
	kPacket << static_cast<byte>( pProduct->nNeedDay );
	TCPNetwork::SendToServer( kPacket );
	TCPNetwork::MouseBusy( true );
}

void ioSoldierAwakeManager::ExtendAwake( const CharAwakePullDownItem* pItem, const CHARACTERDATA* pChar ) const
{
	if( !pChar )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
	}

	const AwakeProduct* pProduct = GetAwakeProduct( pItem->m_iProductIdx );
	if( !pProduct )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return;
	}

	const AwakeInfo* pAwake = GetAwake( pProduct->nNeedAwakeIdx );
	if( !pAwake )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		return;
	}
	
	if( pChar->m_data.m_chExerciseStyle != EXERCISE_NONE )
	{
		AwakeCharExerciseTypeError( pAwake->szName );
		return;
	}

	if( pChar->m_data.m_ePeriodType != CPT_MORTMAIN )
	{
		AwakeCharPeriodTypeError( pAwake->szName );
		return;
	}
	
	SP2Packet kPacket( CTPK_CHAR_AWAKE_EXTEND );
	kPacket << pChar->m_dwIndex;
	kPacket << static_cast<byte>( pProduct->nNeedDay );	
	TCPNetwork::SendToServer( kPacket );
	TCPNetwork::MouseBusy( true );
}

void ioSoldierAwakeManager::ApplyAwake( SP2Packet &rkPacket )
{	
	TCPNetwork::MouseBusy( false );

	byte eResult;
	rkPacket >> eResult;
	if( eResult == AWAKE_MY_OK )
	{
		byte iAwakeType;
		int iCharIndex;
		int iMtrlCode;
		int iLimitDate;
		short nMtrlCnt;
		rkPacket >> iCharIndex >> iAwakeType >> iMtrlCode >> nMtrlCnt >> iLimitDate;
		ioAlchemicInventory* pInven = g_MyInfo.GetAlchemicInventory();
		if( !pInven )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
			return;
		}

		const CHARACTERDATA* pCharData = g_MyInfo.GetCharacterDataByIndex( iCharIndex );
		if( !pCharData )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
			return;
		}

		g_MyInfo.ChangeAwakeInfo( iCharIndex, iAwakeType );
		g_MyInfo.ChangeAwakeDate( iCharIndex, iLimitDate );
		pInven->GainAlchemicItem( iMtrlCode, -nMtrlCnt );

		const AwakeInfo* pAwake = GetAwakeByType( static_cast<AwakeType>( iAwakeType ) );
		if( pAwake )
		{
			AwakeResultMessageBox( pAwake->szName, iLimitDate );
		}

		//�������� ����
		g_UserInfoMgr.SetCharAwake( g_MyInfo.GetPublicID(), pCharData->m_data.m_class_type, static_cast<int>( iAwakeType ) );

		//�����ִ� ĳ������ ���������� ����
		if( m_pPlayStage )
		{
			const CHARACTER& rkSelectChar = g_MyInfo.GetCharacter( g_MyInfo.GetCharIndexByClassType( pCharData->m_data.m_class_type ) );
			ioBaseChar* pChar = m_pPlayStage->GetBaseChar( g_MyInfo.GetPublicID() );

			if( pChar && pChar->GetCharacterInfo().m_class_type == pCharData->m_data.m_class_type )
			{
				pChar->ChangeAwake( iAwakeType );
				pChar->ChangeAwakeDate( iLimitDate );
			}
		}

		//�κ��丮 ����
		MyInventoryWnd *pMyInventoryWnd = dynamic_cast<MyInventoryWnd*> ( g_GUIMgr.FindWnd( MY_INVENTORY_WND ) );
		if( pMyInventoryWnd )
		{
			MyInventoryInfoWnd *pMyInventoryInfoWnd = (MyInventoryInfoWnd*)( pMyInventoryWnd->FindChildWnd( MyInventoryWnd::ID_INVENTORY_INFO_WND ) );
			if( pMyInventoryInfoWnd )
				pMyInventoryInfoWnd->CreateAwakeTitle();

			if( pMyInventoryWnd->IsShow() )
				pMyInventoryWnd->UpdateTab( false, true );
		}

#if defined( USE_GA )
		if( iAwakeType == 1 )
		{
			char chLabel[32] = {0,};

			if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
				sprintf_e( chLabel, "%d", iCharIndex );
			else
				SafeSprintf( chLabel, sizeof(chLabel), "%1", iCharIndex );

			// CHAR_WAKEUP_NORMAL
			g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
				, "Character"
				, "Normal"
				, chLabel
				, 1
				, "%2FCHAR%2FWAKEUP%2FNORMAL" );
		}
		if( iAwakeType == 2 )
		{
			char chLabel[32] = {0,};

			if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
				sprintf_e( chLabel, "%d", iCharIndex );
			else
				SafeSprintf( chLabel, sizeof(chLabel), "%1", iCharIndex );

			// CHAR_WAKEUP_SPECIAL
			g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
				, "Character"
				, "Special"
				, chLabel
				, 1
				, "%2FCHAR%2FWAKEUP%2FSPECIAL" );
		}		
#endif

	}
	else if( eResult == AWAKE_OTHER_OK )
	{
		ioHashString szName;
		byte eAwakeType;
		int iClassType;
		rkPacket >> szName >> eAwakeType >> iClassType;
		g_UserInfoMgr.SetCharAwake( szName, iClassType, static_cast<int>( eAwakeType ) );

		if( m_pPlayStage )
		{ 
			ioBaseChar* pChar = m_pPlayStage->GetBaseChar( szName );
			if( pChar && pChar->GetCharacterInfo().m_class_type == iClassType )
			{
				pChar->ChangeAwake( static_cast<int>( eAwakeType ) );
				pChar->SetAwakeChangeEffect();
			}
		}		
	}
	else
	{
		byte nError;
		rkPacket >> nError;
		switch( nError )
		{
		case AWAKE_NONE_MATERIAL:
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
			}
			break;
		case AWAKE_MATERIAL_SHORTAGE:
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
			}
			break;
		case AWAKE_EXCEPTION:
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
			}
			break;
		}
	}
}

void ioSoldierAwakeManager::ApplyAwakeExtend( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );

	byte eResult;
	rkPacket >> eResult;

	if( eResult == AWAKE_MY_OK )
	{
		int iCharIndex;
		int iMtrlCode;
		int iLimitDate;
		short nMtrlCnt;
		rkPacket >> iCharIndex >> iMtrlCode >> nMtrlCnt >> iLimitDate;	
		ioAlchemicInventory* pInven = g_MyInfo.GetAlchemicInventory();
		if( !pInven )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
			return;
		}

		const CHARACTERDATA* pCharData = g_MyInfo.GetCharacterDataByIndex( iCharIndex );
		if( !pCharData )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
			return;
		}
	
		g_MyInfo.ChangeAwakeDate( iCharIndex, iLimitDate );
		pInven->GainAlchemicItem( iMtrlCode, -nMtrlCnt );

		const AwakeInfo* pAwake = GetAwakeByType( static_cast<AwakeType>( pCharData->m_data.m_iAwakeType ) );
		if( pAwake )
		{
			AwakeResultMessageBox( pAwake->szName, iLimitDate );
		}

		//�������� ����
		g_UserInfoMgr.SetCharAwake( g_MyInfo.GetPublicID(), pCharData->m_data.m_class_type, static_cast<int>( pCharData->m_data.m_iAwakeType ) );

		//�����ִ� ĳ������ ���������� ����
		if( m_pPlayStage )
		{
			const CHARACTER& rkSelectChar = g_MyInfo.GetCharacter( g_MyInfo.GetCharIndexByClassType( pCharData->m_data.m_class_type ) );
			ioBaseChar* pChar = m_pPlayStage->GetBaseChar( g_MyInfo.GetPublicID() );
			if( pChar && pChar->GetCharacterInfo().m_class_type == pCharData->m_data.m_class_type )
			{
				pChar->ChangeAwakeDate( iLimitDate );
			}
		}

		//�κ��丮 ����
		MyInventoryWnd *pMyInventoryWnd = dynamic_cast<MyInventoryWnd*> (g_GUIMgr.FindWnd( MY_INVENTORY_WND ) );
		if( pMyInventoryWnd )
		{
			MyInventoryInfoWnd *pMyInventoryInfoWnd = (MyInventoryInfoWnd*)( pMyInventoryWnd->FindChildWnd( MyInventoryWnd::ID_INVENTORY_INFO_WND ) );
			if( pMyInventoryInfoWnd )
				pMyInventoryInfoWnd->CreateAwakeTitle();
		}

#if defined( USE_GA )
		if( pCharData->m_data.m_iAwakeType == 1 )
		{
			char chLabel[32] = {0,};

			if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
				sprintf_e( chLabel, "%d", iCharIndex );
			else
				SafeSprintf( chLabel, sizeof(chLabel), "%1", iCharIndex );

			// CHAR_WAKEUP_NORMAL_EXTEND
			g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
				, "Character"
				, "Normal_Extend"
				, chLabel
				, 1
				, "%2FCHAR%2FWAKEUP%2FNORMAL%2FEXTEND" );
		}
		if( pCharData->m_data.m_iAwakeType == 2 )
		{
			char chLabel[32] = {0,};

			if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
				sprintf_e( chLabel, "%d", iCharIndex );
			else
				SafeSprintf( chLabel, sizeof(chLabel), "%1", iCharIndex );

			// CHAR_WAKEUP_SPECIAL_EXTEND
			g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
				, "Character"
				, "Special_Extend"
				, chLabel
				, 1
				, "%2FCHAR%2FWAKEUP%2FSPECIAL%2FEXTEND" );
		}		
#endif

	}
	else
	{
		byte nError;
		rkPacket >> nError;
		switch( nError )
		{
		case AWAKE_EXTEND_OVER_DATE:
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
			}
			break;
		case AWAKE_EXTEND_NONE_MATERIAL:
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
			}
			break;
		case AWAKE_EXTEND_MATERIAL_SHORTAGE:
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
			}
			break;
		case AWAKE_EXTEND_EXCEPTION:
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6) );
			}
			break;
		}
	}
}

void ioSoldierAwakeManager::ApplyAwakeEnd( SP2Packet &rkPacket )
{
	int iMax = 0;
	rkPacket >> iMax;	
	for( int i = 0; i < iMax; ++i )
	{
		int iCharIndex;		
		byte eType;
		rkPacket >> iCharIndex >> eType;
		const CHARACTERDATA* pChar	= g_MyInfo.GetCharacterDataByIndex( iCharIndex );
		const AwakeInfo* pAwake		= GetAwakeByType( static_cast<AwakeType>( eType ) );
		if( pChar && pAwake )
		{
			int iClassType = pChar->m_data.m_class_type;

			AwakeEndInfo Info;
			Info.iClassType			= iClassType;
			Info.szClassName		= g_MyInfo.GetClassName( iClassType, g_MyInfo.GetCharPowerUpGradeByClassType( iClassType ) );
			Info.szClassIconName	= g_MyInfo.GetMySoldierIconName( iClassType ).c_str();
			Info.szAwakeName		= pAwake->szName;
			Info.dwAwakeColor		= pAwake->dwNameColor;

			m_AwakeEndVector.push_back( Info );
			g_MyInfo.ChangeAwakeClear( iCharIndex );

			//�������� ����
			g_UserInfoMgr.SetCharAwake( g_MyInfo.GetPublicID(), iClassType, static_cast<int>( AWAKE_NONE ) );

			//�κ��丮 ����
			MyInventoryWnd *pMyInventoryWnd = dynamic_cast<MyInventoryWnd*> (g_GUIMgr.FindWnd( MY_INVENTORY_WND ) );
			if( pMyInventoryWnd )
			{
				MyInventoryInfoWnd* pMyInventoryInfoWnd = (MyInventoryInfoWnd*)( pMyInventoryWnd->FindChildWnd( MyInventoryWnd::ID_INVENTORY_INFO_WND ) );
				if( pMyInventoryInfoWnd )
					pMyInventoryInfoWnd->CreateAwakeTitle();

				if( pMyInventoryWnd->IsShow() )
					pMyInventoryWnd->UpdateTab( false, true );
			}
		}
		else
		{
			LOG.PrintTimeAndLog(0, "%s - Awake End Error - max : %d, index : %d, type : %d", iMax, iCharIndex, static_cast<int>( eType ) );
		}
	}
}

bool ioSoldierAwakeManager::IsReserveDeleteAwake()
{
	if( m_AwakeEndVector.empty() )
		return false;

	return true;
}

void ioSoldierAwakeManager::GetFirstDeleteAwake( std::string& szClassName, std::string& szClassIconName, std::string& szAwakeName, DWORD& dwAwakeColor, int& nClassType )
{
	if( m_AwakeEndVector.empty() )
		return;

	const AwakeEndInfo& rkInfo = *m_AwakeEndVector.begin();

	szClassName		= rkInfo.szClassName;
	szClassIconName	= rkInfo.szClassIconName;
	szAwakeName		= rkInfo.szAwakeName;
	dwAwakeColor	= dwAwakeColor;
	nClassType		= rkInfo.iClassType;
}

void ioSoldierAwakeManager::EraseDeleteExtraAwake( bool bAllErase )
{
	if( m_AwakeEndVector.empty() )
		return;

	if( bAllErase )
		m_AwakeEndVector.clear();
	else
		m_AwakeEndVector.erase( m_AwakeEndVector.begin() );
}