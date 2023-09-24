//----------------------------------------------------------------------------------
/*
Filename	: ShopWnd
Author		: sumin
Date		: 113.11.5_14:41:3
*/
//----------------------------------------------------------------------------------

#include "StdAfx.h"
#include "ShopWnd.h"
#include "CharacterControlWnd.h"
#include "ioSP2GUIManager.h"
#include "ioTabWndEX.h"
#include "ioINILoader.h"
#include "ioScrollBarEX.h"
#include "../ioEventUserNode.h"
#include "../Channeling/ioChannelingNodeManager.h"
#include "../Channeling/ioChannelingNodeParent.h"
#include "../ioClassPrice.h"
#include "../ioSetItemInfoManager.h"
#include "../ioSetItemInfo.h"
#include "../ioExtraItemInfoManager.h"
#include "../ioEtcItemManager.h"
#include "../ioSaleManager.h"
#include "../ioPresentMgr.h"
#include "../ioUserEtcItem.h"
#include "../ioDecorationPrice.h"
#include "../ioEventGoodsMgr.h"
#include "DecoInfoTabWnd.h"


//-------------------------------------------------------------------------------
class ShopItemSorter : public std::binary_function< const ShopItemInfoEX& , const ShopItemInfoEX&, bool >
{
public:
	bool	operator() ( const ShopItemInfoEX& lhs, const ShopItemInfoEX& rhs ) const
	{		
		if ( lhs.m_iSortPoint < rhs.m_iSortPoint ) {
			return true;
		}
		return false;
	}
};

//----------------------------------------------------------------------------------
/*
생성, 파괴
*/
ShopWnd::ShopWnd()
	: ioShopWndBase()
	, m_charinfownd( 0 )
	, m_curtab( TT_WEEKLYITEM )
{
	for ( int i = 0; i < TT_MAX; ++i ) {
		m_tabwnds[ i ] = 0;
	}
}

ShopWnd::~ShopWnd()
{
}

//----------------------------------------------------------------------------------
/*
ioWnd Event Override
*/
void	ShopWnd::iwm_create()
{
	ioShopWndBase::iwm_create();
		
	// 캐릭터 정보창 추가
	ioWnd* base = FindChildWnd( ID_FRM_BACKGROUND );
	if ( base )
	{
		m_charinfownd = (CharacterControlWnd*)( g_GUIMgr.AddWnd( "xml/charactercontrolwnd.xml", base ) );
		if ( m_charinfownd )
		{
			m_charinfownd->ExceptFromID( true );
			m_charinfownd->SetPosX( 10 );
			m_charinfownd->SetPosY( 50 );
			m_charinfownd->ShowWnd();
		}
	}

	// tab 윈도우들 추가
	for ( int i = 0; i < TT_MAX; ++i ) {
		m_tabwnds[ i ] = 0;
	}
	static const int tabwndx = -7;
	static const int tabwndy = 60;
	AddTabWnd( ID_TABBTN_WEEKLYITEM, TT_WEEKLYITEM , "xml/shopwnd_weeklytab.xml", tabwndx, tabwndy );
	AddTabWnd( ID_TABBTN_MERCENARY, TT_MERCENARY, "xml/shopwnd_etctab.xml", tabwndx, tabwndy );
	AddTabWnd( ID_TABBTN_EQUIP, TT_EQUIP, "xml/shopwnd_etctab.xml", tabwndx, tabwndy );
	AddTabWnd( ID_TABBTN_ACCESSORY, TT_ACCESSORY, "xml/shopwnd_etctab.xml", tabwndx, tabwndy );
	AddTabWnd( ID_TABBTN_UNIQUE, TT_UNIQUE, "xml/shopwnd_etctab.xml", tabwndx, tabwndy );
	AddTabWnd( ID_TABBTN_EVENTSHOP, TT_EVENTSHOP, "xml/shopwnd_etctab.xml", tabwndx, tabwndy );
	
	// 처음거 선택
	if ( m_itemtabwnd ) {
		m_itemtabwnd->SetCheck( 0, true );
	}	
}

void	ShopWnd::iwm_show()
{
	ioShopWndBase::iwm_show();
}

//----------------------------------------------------------------------------------
/*
Event Virtual Functions
*/
void	ShopWnd::OnMouseWheel( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
	if ( m_vscrollbar )
	{
		float pos = m_vscrollbar->GetScrollPos() + ( event.m_wheeldelta / -120.0f );
		if ( m_vscrollbar->GetScrollPos() != pos )
		{
			m_vscrollbar->SetScrollPos( pos );
			UpdateController( m_curtab );
		}
	}
}

void	ShopWnd::OnClickSBDecBtn( ioWndEX* wnd, ioWndEXScrollBarEvent& event )
{
	UpdateController( m_curtab );
}

void	ShopWnd::OnClickSBIncBtn( ioWndEX* wnd, ioWndEXScrollBarEvent& event )
{
	UpdateController( m_curtab );
}

void	ShopWnd::OnScrollSB( ioWndEX* wnd, ioWndEXScrollBarEvent& event )
{
	UpdateController( m_curtab );
}

void	ShopWnd::OnTabItem( ioWndEX* wnd, ioWndEXRadioEvent& event )
{
	TAB_TYPE type = static_cast<TAB_TYPE>( event.m_index );

	if ( m_vscrollbar ) {
		m_vscrollbar->SetScrollPos( 0 );
	}
	
	switch ( type )
	{
	case TT_WEEKLYITEM:
		OnTabWeeklyItem();
		break;

	case TT_MERCENARY:
		OnTabMercenary();
		break;

	case TT_EQUIP:
		OnTabEquip();
		break;

	case TT_ACCESSORY:
		OnTabAccessory();
		break;

	case TT_UNIQUE:
		OnTabUnique();
		break;

	case TT_EVENTSHOP:
		OnTabEventShop();
		break;
	}
	
	m_curtab = type;
}

// filter tab
void	ShopWnd::OnTabWeeklyFilter( ioWndEX* wnd, ioWndEXRadioEvent& event )
{	
	ShopItemInfoVec iteminfos;

	GetShopMecenaryInfo( iteminfos, true );
	GetShopEquipInfo( iteminfos, true );
	GetShopAccessoryInfo( iteminfos, GetCurClassType(), GetCurSexType(), true ); //! 임시
	GetShopUniqueInfo( iteminfos, true, SHOP_TAB_TYPE_NONE );
	
	// filtering
	switch ( event.m_index )
	{
	case TFW_WEEKLY:
		break;

	case TFW_NEW:
		break;

	case TFW_POP:
		break;

	case TFW_UNLOCK_GRADE:
		break;
	}

	std::sort( iteminfos.begin(), iteminfos.end(), ShopItemSorter() );

	SetupController( TT_WEEKLYITEM, 5, iteminfos, 5, 2 );
}

void	ShopWnd::OnTabMecenaryFilter( ioWndEX* wnd, ioWndEXRadioEvent& event )
{
	ShopItemInfoVec iteminfos;

	GetShopMecenaryInfo( iteminfos, false );
	GetShopUniqueInfo( iteminfos, false, SHOP_TAB_TYPE_SOLDIER );

	// filtering
	//switch ( event.m_index )
	//{
	//case TFM_MECENARY:		
	//	{
	//		ShopItemInfoVec::iterator iter = iteminfos.begin();
	//		for ( ; iter != iteminfos.end(); )
	//		{
	//			RandomMachineInfo* pInfo = g_ExtraItemInfoMgr.GetRandomMachineInfo( (*iter).m_iValue1 );
	//			if ( !pInfo || ( pInfo && !pInfo->IsPackage() ) ) {
	//				++iter;
	//			} else {
	//				iter = iteminfos.erase( iter );
	//			}
	//		}
	//	}
	//	break;

	//case TFM_PACKAGE:
	//	{
	//		ShopItemInfoVec::iterator iter = iteminfos.begin();
	//		for ( ; iter != iteminfos.end(); )
	//		{
	//			RandomMachineInfo* pInfo = g_ExtraItemInfoMgr.GetRandomMachineInfo( (*iter).m_iValue1 );
	//			if ( !pInfo || !pInfo->IsPackage() ) {
	//				iter = iteminfos.erase( iter );
	//			} else {
	//				++iter;
	//			}
	//		}
	//	}
	//	break;
	//}

	std::sort( iteminfos.begin(), iteminfos.end(), ShopItemSorter() );

	SetupController( TT_MERCENARY, 5, iteminfos, 5, 3 );
}

void	ShopWnd::OnTabEquipFilter( ioWndEX* wnd, ioWndEXRadioEvent& event )
{
	ShopItemInfoVec iteminfos;

	GetShopEquipInfo( iteminfos, false );
	GetShopUniqueInfo( iteminfos, false, SHOP_TAB_TYPE_EXTRA );

	std::sort( iteminfos.begin(), iteminfos.end(), ShopItemSorter() );

	SetupController( TT_EQUIP, 5, iteminfos, 5, 3 );
}

void	ShopWnd::OnTabAccessoryFilter( ioWndEX* wnd, ioWndEXRadioEvent& event )
{
	ShopItemInfoVec iteminfos;

	GetShopAccessoryInfo( iteminfos, GetCurClassType(), GetCurSexType(), false ); //! 임시
	GetShopUniqueInfo( iteminfos, false, SHOP_TAB_TYPE_DECO );

	std::sort( iteminfos.begin(), iteminfos.end(), ShopItemSorter() );

	SetupController( TT_ACCESSORY, 5, iteminfos, 5, 3 );
}

void	ShopWnd::OnTabUniqueFilter( ioWndEX* wnd, ioWndEXRadioEvent& event )
{
	ShopItemInfoVec iteminfos;

	GetShopUniqueInfo( iteminfos, false, SHOP_TAB_TYPE_NONE );

	std::sort( iteminfos.begin(), iteminfos.end(), ShopItemSorter() );

	SetupController( TT_UNIQUE, 5, iteminfos, 5, 3 );
}

void	ShopWnd::OnTabEventShopFilter( ioWndEX* wnd, ioWndEXRadioEvent& event )
{
	ShopItemInfoVec iteminfos;

	switch ( event.m_index )
	{
	case TFE_CLOVER:
		GetShopEventShopInfo( iteminfos, ioEtcItem::EIT_ETC_CLOVER );
		break;

	case TFE_EVENT:
		GetShopEventShopInfo( iteminfos, ioEtcItem::EIT_ETC_SILVER_COIN );
		break;

	case TFE_MILEAGE:
		GetShopEventShopInfo( iteminfos, ioEtcItem::EIT_ETC_MILEAGE_COIN );
		break;
	}

	std::sort( iteminfos.begin(), iteminfos.end(), ShopItemSorter() );

	SetupController( TT_EVENTSHOP, 5, iteminfos, 5, 3 );
}

//----------------------------------------------------------------------------------
/*
Get
*/
int		ShopWnd::GetCurClassType() const
{
	if ( !m_charinfownd ) return 0;
	return m_charinfownd->GetCurClassType();
}

int		ShopWnd::GetCurSexType() const
{
	if ( !m_charinfownd ) return 0;
	return m_charinfownd->GetCurSexType();
}

//----------------------------------------------------------------------------------
/*
ETC
*/
void	ShopWnd::SetupController( TAB_TYPE type, int elementcol, ShopItemInfoVec& infos, int uicol, int uirow )
{
	ShopWndTabController* controller = GetShopWndTabController( type );
	if ( !controller ) {
		return;
	}

	int totalsize = infos.size();
	int row = ( totalsize / elementcol ) + ( ( totalsize % elementcol ) ? 1 : 0 );
	controller->SetElementSize( elementcol, row );
	controller->SetUISize( uicol, uirow );

	if ( m_vscrollbar ) 
	{
		m_vscrollbar->SetScrollRange( row - uirow );
		m_vscrollbar->SetScrollPos( 0.0f );
	}

	for( int i = 0; i < row; ++i )
	{
		for ( int loop = 0; loop < elementcol; ++loop )
		{
			int idx = i * elementcol + loop;
			if ( totalsize > idx ) {
				controller->SetElement( loop, i, &(infos[ idx ]) );
			}
		}
	}	

	controller->UpdateElement();
}

void	ShopWnd::AddTabWnd( DWORD parentid, TAB_TYPE type, const char* filename, int posx, int posy )
{
	if ( type < 0 || type >= TT_MAX || !filename ) {
		return;
	}

	ioWnd* wtabbtn = FindChildWndEX( ID_TABBTN_WEEKLYITEM );
	if ( !wtabbtn ) {
		return;
	}

	ioWnd* tabbtn = FindChildWndEX( parentid );
	if ( tabbtn )
	{
		m_tabwnds[ type ] = g_GUIMgr.AddWnd( filename, tabbtn );
		if ( m_tabwnds[ type ] )
		{
			m_tabwnds[ type ]->ExceptFromID( true );
			
			int offsetx = wtabbtn->GetDerivedPosX() - tabbtn->GetDerivedPosX();
			int offsety = wtabbtn->GetDerivedPosY() - tabbtn->GetDerivedPosY();

			m_tabwnds[ type ]->SetPosX( posx + offsetx );
			m_tabwnds[ type ]->SetPosY( posy + offsety );

			ShopWndTabController* controller = (ShopWndTabController*)(m_tabwnds[ type ]);
			if ( controller )
			{
				controller->SetShopWnd( this );
				controller->AttachVertSB( (ioScrollBarEX*)( FindChildWndEX( ID_SCR_VERTSB ) ) );
			}
		}
	}
}

void	ShopWnd::UpdateController( TAB_TYPE type )
{
	ShopWndTabController* controller = GetShopWndTabController( type );
	if ( controller ) {
		controller->UpdateElement();
	}
}

void	ShopWnd::OnTabWeeklyItem()
{
	ShowSortComboBox( false );

	if ( m_tabweeklyfilter ) {
		m_tabweeklyfilter->SetCheck( 0, true );
	}
}

void	ShopWnd::OnTabMercenary()
{
	ShowSortComboBox( true );

	if ( m_tabmecenaryfilter ) {
		m_tabmecenaryfilter->SetCheck( 0, true );
	}
}

void	ShopWnd::OnTabEquip()
{
	ShowSortComboBox( true );

	if ( m_tabequipfilter ) {
		m_tabequipfilter->SetCheck( 0, true );
	}
}

void	ShopWnd::OnTabAccessory()
{
	ShowSortComboBox( true );

	if ( m_tabaccessoryfilter ) {
		m_tabaccessoryfilter->SetCheck( 0, true );
	}
}

void	ShopWnd::OnTabUnique()
{
	ShowSortComboBox( false );

	if ( m_tabuniquefilter ) {
		m_tabuniquefilter->SetCheck( 0, true );
	}
}

void	ShopWnd::OnTabEventShop()
{
	ShowSortComboBox( false );

	if ( m_tabeventshopfilter ) {
		m_tabeventshopfilter->SetCheck( 0, true );
	}
}

void	ShopWnd::ShowSortComboBox( bool show )
{
}

// 상점 정보 얻어오는 함수들
bool	ShopWnd::IsMecenaryEvent() const
{
	BuyCharNoLevelLimitEventUserNode* eventnode = (BuyCharNoLevelLimitEventUserNode*)( g_MyInfo.GetEventUserMgr().GetEventUserNode( EVT_BUY_CHAR_NO_LEVEL_LIMIT ) );
	return ( eventnode && eventnode->IsAliveUserNode() );
}

bool	ShopWnd::IsEnableSubscription() const
{
	ioChannelingNodeParent* channeclnode = g_ChannelingMgr.GetNode( g_MyInfo.GetChannelingType() );
	return ( channeclnode && channeclnode->IsSubscriptionRetract() );
}

void	ShopWnd::GetShopMecenaryInfo( ShopItemInfoVec& out, bool filteringweekly )
{
	bool isevent = IsMecenaryEvent();
	bool issubscription = IsEnableSubscription();

	int num = g_ClassPrice.MaxClassPrice();
	for ( int i = 0; i < num; ++i )
	{
		if ( !g_ClassPrice.IsActiveClass( i ) ) {
			continue;
		}

		int idxset = g_ClassPrice.ArrayToSetItemIdx( i );
		const ioSetItemInfo* setitemInfo = g_SetItemInfoMgr.GetSetInfoByIdx( idxset );
		if ( !setitemInfo ) {
			continue;
		}

		int classtype = setitemInfo->GetSetCode() - SET_ITEM_CODE;
		if ( filteringweekly )
		{
			if ( ORDER_DEFAULT == setitemInfo->GetRecommendedShopOrder() ) {
				continue;
			}
		
			if ( !g_SaleMgr.IsSaleByRecommended( ioSaleManager::IT_CLASS, classtype ) ) {
				continue;
			}
		}
		else
		{
			//if( !IsRightSmallTab( dwTabID, dwSmallTabID, pInfo->GetCharAttackType() , 0 ) ) {
			//	continue;
			//}
		}

		ShopItemInfoEX info;

		info.m_iItemType = PRESENT_SOLDIER;

		if ( filteringweekly ) {
			info.m_iSortPoint = setitemInfo->GetRecommendedShopOrder();
		} else {
			info.m_iSortPoint = setitemInfo->GetShopOrder();
		}

		info.m_iShopMarkType  = setitemInfo->GetShopMarkType();
		if ( info.m_iShopMarkType == SHOP_MARK_TYPE_NONE && ( g_MyInfo.GetClassArray( classtype ) != -1 ) ) {
			info.m_iShopMarkType = SHOP_MARK_TYPE_BOUGHT;
		}

		info.m_iNeedLevel = setitemInfo->GetNeedLevel( 0 ); // 0번째 array 정보 
		int iPeso = g_ClassPrice.GetClassBuyPeso( classtype, g_ClassPrice.GetLimitDatePeso( classtype , 0 ) );
		info.m_vPeso.push_back( iPeso ); // 기간제 1
		iPeso = g_ClassPrice.GetClassBuyPeso( classtype, g_ClassPrice.GetLimitDatePeso( classtype , 1 ) ); 
		info.m_vPeso.push_back( iPeso ); // 기간제 2

		int iLimitCashDate = g_ClassPrice.GetLimitDateCash( classtype, 0 ); 
		int iCash = g_ClassPrice.GetClassBuyCash( classtype, iLimitCashDate );
		if ( iCash > 0 ) {
			info.m_vCash.push_back( iCash ); // 캐쉬 기간제 1
		}
		iLimitCashDate = g_ClassPrice.GetLimitDateCash( classtype, 1 ); 
		iCash = g_ClassPrice.GetClassBuyCash( classtype, iLimitCashDate );
		if ( iCash > 0 ) {
			info.m_vCash.push_back( iCash ); // 캐쉬 기간제 2
		}
		int iMortmainCash = g_ClassPrice.GetMortmainCharCash( classtype );
		if ( iMortmainCash > 0 ) {
			info.m_vCash.push_back( iMortmainCash );
		}

		int iManualIndex = setitemInfo->GetItemManual() - 1; // -1: number -> idx
		info.m_vManualIndex.push_back( iManualIndex );
		info.m_iValue1 = classtype;
		info.m_sName = setitemInfo->GetName();
		info.m_sIcon = g_MyInfo.GetMySoldierIconName( classtype );

		if ( issubscription ) {
			info.m_iSubscriptionType = g_ClassPrice.GetSubscriptionType( classtype );
		}

		if ( isevent || g_MyInfo.IsCanBuyItem( setitemInfo ) ) {
			info.m_iNeedLevel = 0;
		}

		if ( filteringweekly )
		{
			if ( info.m_iShopMarkType == SHOP_MARK_TYPE_NONE || info.m_iShopMarkType == SHOP_MARK_TYPE_BOUGHT )
			{
				if ( info.m_iNeedLevel == 0 )
				{
					info.m_iSortPoint = info.GetPeso( 0 );
					info.m_iSortPoint += ( NewShopWnd::SORT_WEIGHT_SMALL + NewShopWnd::SORT_WEIGHT_SMALL );
				}
				else
				{
					info.m_iSortPoint += NewShopWnd::SORT_WEIGHT_BIG;
				}
			}
		}

		out.push_back( info );
	}
}

void	ShopWnd::GetShopEquipInfo( ShopItemInfoVec& out, bool filteringweekly )
{
	bool issubscription = IsEnableSubscription();

	int num = g_ExtraItemInfoMgr.GetRadomMachineCnt();
	for ( int i = 0; i < num ; ++i )
	{
		RandomMachineInfo* pInfo = g_ExtraItemInfoMgr.GetRandomMachineInfoByArray( i );
		if ( !pInfo || !pInfo->IsActive() ) {
			continue;
		}

		if ( filteringweekly )
		{
			if ( ORDER_DEFAULT == pInfo->GetRecommandedShopOrder() ) {
				continue;
			}
			
			if ( !g_SaleMgr.IsSaleByRecommended( ioSaleManager::IT_EXTRA_BOX, pInfo->GetMachineCode() ) ) {
				continue;
			}
		}
		else
		{
			//if( !IsRightSmallTab( dwTabID, dwSmallTabID, pInfo->GetGroup(), 0 ) ) // 0: 필요없는 값으로 0셋팅
			//	continue;
		}
		
		ShopItemInfoEX info;

		info.m_iItemType = PRESENT_EXTRAITEM_BOX;

		if ( filteringweekly ) {
			info.m_iSortPoint = pInfo->GetRecommandedShopOrder();
		} else {
			info.m_iSortPoint = pInfo->GetPriority();
		}

		info.m_iShopMarkType = pInfo->GetShopMarkType();
		info.m_iNeedLevel = pInfo->GetNeedLevel();
		info.m_iValue1 = pInfo->GetMachineCode();
		info.m_iValue2 = pInfo->GetGroup();
		info.m_itemcode = pInfo->GetItemCode( 0 );
		info.m_sName = pInfo->GetName();
		info.m_sIcon = pInfo->GetIconName();

		if ( issubscription ) {
			info.m_iSubscriptionType = pInfo->GetSubscriptionType();
		}

		info.m_vBonusPeso.push_back( pInfo->GetBonusPeso() );
		info.m_vManualIndex.push_back( pInfo->GetMainManual() - 1 );
		info.m_vCash.push_back( pInfo->GetNeedCash() );

		int pesosize = pInfo->GetNeedPesoSize();
		for ( int i = 0; i < pesosize ; ++i )
		{
			info.m_vPeso.push_back( pInfo->GetNeedPeso( i ) );
			info.m_vValue3.push_back( pInfo->GetPeriod( i ) );
		}

		if ( info.GetValue3( 0 ) > 0  && pInfo->GetNeedCash() > 0 ) { // 멀티페소사용이라면
			info.m_vValue3.push_back( pInfo->GetPeriod( pInfo->GetNeedPesoSize() ) );
		}
		
		if ( g_MyInfo.GetGradeLevel() >= info.m_iNeedLevel ) {
			info.m_iNeedLevel = 0;
		}

		if ( pInfo->GetNeedPeso( 0 ) > 0 && info.m_iNeedLevel == 0 ) {
			info.m_bShowPeso = true;
		}

		out.push_back( info );
	}
}

void	ShopWnd::GetShopAccessoryInfo( ShopItemInfoVec& out, int decoclasstype, int sextype, bool filteringweekly )
{
	bool issubscription = IsEnableSubscription();

	int numsex = g_DecorationPrice.GetSexSize();
	for ( int i = 0; i < numsex ; ++i )
	{		
		int decosextype = g_DecorationPrice.GetSexType( i );
		if ( decosextype != sextype ) {
			continue;
		}

		int numtype = g_DecorationPrice.GetDecoTypeSize( i );
		for ( int loop = 0; loop < numtype; ++loop )
		{
			int type = g_DecorationPrice.GetDecoType( i, loop );
			int size = g_DecorationPrice.GetDecoSize( i, loop );
			for ( int j = 0; j < size; ++j )
			{
				if ( !g_DecorationPrice.GetActive( i, loop, j ) ) {
					continue;
				}

				int code = g_DecorationPrice.GetDecoCode( i, loop, j );

				const DecoData* data =  g_DecorationPrice.GetDecoDataPtr( decosextype, type, code );
				if ( !data ) {
					continue;
				}

				if ( filteringweekly )
				{
					if ( ORDER_DEFAULT == data->m_iRecommendedShopOrder ) {
						continue;
					}

					if ( !g_SaleMgr.IsSaleByRecommended( ioSaleManager::IT_DECO, type ) ) {
						continue;
					}
				}
				else
				{
					//if( !IsRightSmallTab( dwTabID, dwSmallTabID, iDecoType, iDecoCode ) )
					//	continue;
				}

				ShopItemInfoEX info;

				info.m_iItemType = PRESENT_DECORATION;

				if ( filteringweekly ) {
					info.m_iSortPoint = data->m_iRecommendedShopOrder;
				} else {					
					info.m_iSortPoint = data->m_iShopOrder;
				}

				info.m_iShopMarkType  = data->m_iShopMarkType;
				if ( info.m_iShopMarkType == SHOP_MARK_TYPE_NONE ) 
				{
					int iInvenDecoType = ( decoclasstype * 100000 ) + ( decosextype * 1000 ) + type;
					if ( g_MyInfo.IsSlotItem( iInvenDecoType, code ) ) {
						info.m_iShopMarkType = SHOP_MARK_TYPE_BOUGHT;
					}
				}

				info.m_vBonusPeso.push_back( data->m_iBonusPeso );
				info.m_iNeedLevel = data->m_iLimitLevel;
				info.m_vManualIndex.push_back( data->m_iManual - 1 );
				info.m_bShowPeso = data->m_bShowPeso;
				info.m_vPeso.push_back( data->m_iPeso );
				info.m_vCash.push_back( data->m_iCash );
				info.m_iValue1 = type;
				info.m_iValue2 = code;
				info.m_itemcode = code;
				info.m_vValue3.push_back( decosextype );
				info.m_sName = data->m_szName;
				info.m_sIcon = data->m_szIcon;

				if ( issubscription ) {
					info.m_vSubscriptionType.push_back( data->m_iSubscriptionType );
				} else {
					info.m_vSubscriptionType.push_back( SUBSCRIPTION_NONE );
				}

				if ( g_MyInfo.GetClassLevel( decoclasstype, true ) >= info.m_iNeedLevel ) {
					info.m_iNeedLevel = 0;
				}

				out.push_back( info );
			}
		}
	}
}

void	ShopWnd::GetShopUniqueInfo( ShopItemInfoVec& out, bool filteringweekly, int tabtype )
{
	bool issubscription = IsEnableSubscription();

	int num = g_EtcItemMgr.GetEtcItemCount();
	for ( int i = 0; i < num ; ++i )
	{
		ioEtcItem* pinfo = g_EtcItemMgr.GetEtcItemByIdx( i );
		if ( !pinfo ) {
			continue;
		}

		ShopItemInfoEX info;
		int isize = pinfo->GetValueSize();
		for ( int loop = 0; loop < isize ; ++loop )
		{
			if ( !pinfo->IsActive( loop ) ) {
				continue;
			}

			if ( tabtype == SHOP_TAB_TYPE_NONE )
			{
				if ( filteringweekly )
				{
					if ( ORDER_DEFAULT == pinfo->GetRecommandedShopOrder( loop ) ) {
						continue;
					}

					if ( !g_SaleMgr.IsSaleByRecommended( ioSaleManager::IT_ETC, pinfo->GetType(), loop ) ) {
						continue;
					}
				}
				else
				{
					//if ( !IsRightSmallTab( dwTabID, dwSmallTabID, pinfo->GetGroup(), pinfo->GetShopTabType( j ) ) ) 
					//	continue;
				}
			}
			else
			{
				if ( pinfo->GetShopTabType( loop ) != tabtype ) {
					continue;
				}
				//if ( !IsRightSmallTab( dwTabID, dwSmallTabID, ETC_ITEM_OTHER_VALUE, 0 ) ) 
				//	continue;
			}

			info.m_iItemType = PRESENT_ETC_ITEM;

			if ( filteringweekly ) {
				info.m_iSortPoint = pinfo->GetRecommandedShopOrder( loop );
			} else {
				info.m_iSortPoint = pinfo->GetShopOrder( loop );
			}

			info.m_iShopMarkType = pinfo->GetShopMarkType( loop );
			if ( info.m_iShopMarkType == SHOP_MARK_TYPE_NONE )
			{				
				ioUserEtcItem* pUserEtcItem = g_MyInfo.GetUserEtcItem();
				if ( pUserEtcItem )
				{
					ioUserEtcItem::ETCITEMSLOT kSlot;
					if ( pUserEtcItem->GetEtcItem( pinfo->GetType(), kSlot ) ) {
						info.m_iShopMarkType = SHOP_MARK_TYPE_BOUGHT;
					}
				}
			}

			info.m_vBonusPeso.push_back( pinfo->GetBonusPeso( loop ) );
			info.m_vManualIndex.push_back( pinfo->GetMainManual( loop ) - 1 );
			info.m_bShowPeso = pinfo->IsShowPeso( loop );
			info.m_vPeso.push_back( pinfo->GetPeso( loop ) );
			info.m_vCash.push_back( pinfo->GetCash( loop ) );
			info.m_iValue1 = pinfo->GetType();
			info.m_iValue2 = pinfo->GetShopTabType( loop );
			info.m_vValue3.push_back( loop );
			info.m_sName = pinfo->GetShopName( loop );
			info.m_sIcon = pinfo->GetIconName();

			if ( issubscription ) {
				info.m_vSubscriptionType.push_back( pinfo->GetSubscriptionType( loop ) );
			} else {
				info.m_vSubscriptionType.push_back( SUBSCRIPTION_NONE );
			}

			if ( tabtype == SHOP_TAB_TYPE_SOLDIER ) {
				if ( info.m_iShopMarkType == SHOP_MARK_TYPE_NONE || info.m_iShopMarkType == SHOP_MARK_TYPE_BOUGHT ) {
					info.m_iSortPoint += NewShopWnd::SORT_WEIGHT_SMALL;
				}
			}

		}

		if ( !info.m_sName.IsEmpty() ) {
			out.push_back( info );
		}
	}
}

void	ShopWnd::GetShopEventShopInfo( ShopItemInfoVec& out, DWORD dwNeedEtcItemIdx )
{
	int num = g_EventGoodsMgr.GetMaxGoods();
	for ( int i = 0; i < num ; ++i )
	{
		DWORD dwGoodsIndex = 0;
		DWORD dwNeedEtcItem= 0;
		short iPresentType = 0;
		int iGoodsCount, iNeedEtcItemCount, iPresentValue1, iPresentValue2, iPresentValue3, iPresentValue4, iManualIndex, iGoodsOrder, iGoodsMark;
		iGoodsCount = iNeedEtcItemCount = iPresentValue1 = iPresentValue2 = iPresentValue3 = iPresentValue4 = iManualIndex = iGoodsOrder = iGoodsMark = 0;
		g_EventGoodsMgr.GetGoods( i, dwGoodsIndex, iGoodsCount, dwNeedEtcItem, iNeedEtcItemCount, iPresentType, iPresentValue1, iPresentValue2, iPresentValue3, iPresentValue4, iManualIndex, iGoodsOrder, iGoodsMark );
		if( dwGoodsIndex == 0 ) continue;
		if( dwNeedEtcItemIdx != dwNeedEtcItem ) continue;
				
		//if( !IsRightSmallTab( dwTabID, dwSmallTabID, dwNeedEtcItemIdx , 0 ) ) // 0은 미사용
		//	continue;

		ShopItemInfoEX info;
		info.m_iItemType = PRESENT_DEFAULT;
		info.m_iValue0 = iPresentType;
		info.m_iValue1 = iPresentValue1;
		info.m_iValue2 = iPresentValue2;
		info.m_iShopMarkType = iGoodsMark;
		info.m_iSortPoint = iGoodsOrder;
		info.m_dwStampEtcItem = dwNeedEtcItem;
		info.m_vStamp.push_back( iNeedEtcItemCount );
		info.m_vManualIndex.push_back( iManualIndex );
		info.m_vValue3.push_back( dwGoodsIndex );
		info.m_vValue3.push_back( iGoodsCount );
		info.m_sName = g_PresentMgr.GetPresentValue1Text( iPresentType, iPresentValue1, iPresentValue2 );
		info.m_sIcon = g_PresentMgr.GetPresentIconText( iPresentType, iPresentValue1, iPresentValue2 );
		if ( dwNeedEtcItemIdx == ioEtcItem::EIT_ETC_CLOVER ) {
			info.m_eventshoptype = ShopItemInfoEX::EVENTSHOPITEM_TYPE_CLOVER;
		} else if ( dwNeedEtcItemIdx == ioEtcItem::EIT_ETC_SILVER_COIN ) {
			info.m_eventshoptype = ShopItemInfoEX::EVENTSHOPITEM_TYPE_SILVER_COIN;
		} else if ( dwNeedEtcItemIdx == ioEtcItem::EIT_ETC_MILEAGE_COIN ) {
			info.m_eventshoptype = ShopItemInfoEX::EVENTSHOPITEM_TYPE_MILEAGE_COIN;
		}else {
			info.m_eventshoptype = ShopItemInfoEX::EVENTSHOPITEM_TYPE_NONE;
		}

        out.push_back( info );	
	}
}