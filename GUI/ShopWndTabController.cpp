//----------------------------------------------------------------------------------
/*
Filename	: ShopWndTabController
Author		: sumin
Date		: 113.11.5_14:41:3
*/
//----------------------------------------------------------------------------------

#include "StdAfx.h"
#include "ShopWndTabController.h"
#include "ioScrollBarEX.h"
#include "../ioPresentMgr.h"
#include "../ioEtcItem.h"
#include "../EtcHelpFunc.h"
#include "../ioEtcItemManager.h"
#include "ShopBuyWndFrame.h"
#include "../WndID.h"

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
ShopItemInfoEX
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
const char*		ShopItemInfoEX::GetPrice()
{
	static const char* def = "0";
	static char buffer[ 512 ] = { 0, };

	switch ( m_eventshoptype )
	{
	case EVENTSHOPITEM_TYPE_NONE:
		{
			if ( IsShowPeso() )
			{
				Help::ConvertNumToStrComma( GetPeso( 0 ), buffer, sizeof( buffer ) );
				return buffer;
			}
			else
			{
				Help::ConvertNumToStrComma( GetCash( 0 ), buffer, sizeof( buffer ) );
				return buffer;
			}
		}
		break;

	case EVENTSHOPITEM_TYPE_CLOVER:
	case EVENTSHOPITEM_TYPE_SILVER_COIN:
	case EVENTSHOPITEM_TYPE_MILEAGE_COIN:
		{
			Help::ConvertNumToStrComma( GetStamp( 0 ), buffer, sizeof( buffer ) );
			return buffer;
		}
		break;
	}

	return def;
}

const char*		ShopItemInfoEX::GetPriceUnit()
{
	static const char* def = "???";
	static const char* peso = STR(1);
	static const char* gold = STR(2);
	static const char* clover = STR(3);
	static const char* silver = STR(4);
	static const char* mileage = "mileage";

	switch ( m_eventshoptype )
	{
	case EVENTSHOPITEM_TYPE_NONE:
		{
			if ( IsShowPeso() ) {
				return peso;
			} else {
				return gold;
			}
		}
		break;

	case EVENTSHOPITEM_TYPE_CLOVER:
		{
			ioEtcItem* pEtcItem = g_EtcItemMgr.FindEtcItem( m_dwStampEtcItem );
			if ( pEtcItem ) {
				return pEtcItem->GetName().c_str();
			} else {
				return clover;
			}
		}
		break;

	case EVENTSHOPITEM_TYPE_SILVER_COIN:
		{
			ioEtcItem* pEtcItem = g_EtcItemMgr.FindEtcItem( m_dwStampEtcItem );
			if ( pEtcItem ) {
				return pEtcItem->GetName().c_str();
			} else {
				return silver;
			}
		}
		break;

	case EVENTSHOPITEM_TYPE_MILEAGE_COIN:
		{
			ioEtcItem* pEtcItem = g_EtcItemMgr.FindEtcItem( m_dwStampEtcItem );
			if ( pEtcItem ) {
				return pEtcItem->GetName().c_str();
			} else {
				return mileage;
			}
		}
		break;
	}

	return def;
}

bool	ShopItemInfoEX::IsShowPeso()
{
	if ( m_bShowPeso && GetPeso( 0 ) > 0 ) {
		return true;
	}
	if ( GetPeso( 0 ) > 0 && GetCash( 0 ) <= 0 ) {
		return true;
	}
	if ( ( m_iNeedLevel == 0 ) && ( GetPeso( 0 ) > 0 ) && ( GetCash( 0 ) > 0 ) ) {
		return true;
	}

	// 경험치+200 예외
	if ( m_iItemType == PRESENT_ETC_ITEM && m_iValue1 == ioEtcItem::EIT_ETC_EXP_BONUS && GetPeso( 2 ) > 0 ) {
		return true;
	}

	// 페소+100 예외
	if ( m_iItemType == PRESENT_ETC_ITEM && m_iValue1 == ioEtcItem::EIT_ETC_PESO_BONUS && GetPeso( 2 ) > 0 ) {
		return true;
	}

	return false;
}

bool	ShopItemInfoEX::IsEventItem()
{
	return ( GetStamp( 0 ) > 0 );
}




//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
ShopWndTabController
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
/*
생성, 파괴
*/
ShopWndTabController::ShopWndTabController()
	: m_shopwnd( 0 )
	, m_vscrollbar( 0 )
	, m_hscrollbar( 0 )
	, m_uicol( 0 )
	, m_uirow( 0 )
{
}

ShopWndTabController::~ShopWndTabController()
{
	Clear();
}

//----------------------------------------------------------------------------------
/*
virtual SubmitElement
*/
void	ShopWndTabController::SubmitElement( int uicol, int uirow, ShopItemInfoEX* elem )
{
}

//----------------------------------------------------------------------------------
/*
UpdateElement
*/
void	ShopWndTabController::UpdateElement()
{
	int row = 0;
	if ( m_vscrollbar ) {
		row = static_cast<int>( m_vscrollbar->GetScrollPos() );
	}

	int col = 0;
	if ( m_hscrollbar ) {
		col = static_cast<int>( m_hscrollbar->GetScrollPos() );
	}

	for ( int i = 0; i < m_uicol; ++i ) {
		for ( int loop = 0; loop < m_uirow; ++loop ) {
			SubmitElement( i, loop, Get( col + i, row + loop ) );
		}
	}
}

//----------------------------------------------------------------------------------
/*
Control
*/
void	ShopWndTabController::Clear()
{
	int numcol = GetNumCol();
	int numrow = GetNumRow();
	for ( int i = 0; i < numcol; ++i ) 
	{
		for ( int loop = 0; loop < numrow; ++loop )
		{
			ShopItemInfoEX* elem = m_elements[ i ][ loop ];
			if ( elem ) {
				delete elem;
			}
		}
		m_elements[ i ].clear();
	}
	m_elements.clear();

	m_uicol = 0;
	m_uirow = 0;
}

void	ShopWndTabController::SetElementSize( int col, int row )
{
	int numcol = GetNumCol();
	int numrow = GetNumRow();

	if ( numcol < col )
	{
		m_elements.resize( col, std::vector< ShopItemInfoEX* >( 0, numrow ) );
	}
	else if ( numcol > col )
	{
		for ( int i = 0; i < numcol - col; ++i )
		{
			DeleteRow( m_elements[ m_elements.size() - 1 ] );
			m_elements.pop_back();
		}
	}

	if ( numrow < row )
	{
		for ( int i = 0; i < numcol; ++i ) {
			for ( int loop = 0; loop < row - numrow; ++loop ) {
				m_elements[ i ].push_back( 0 );//.resize( row, 0 );
			}
		}
	}
	else if ( numrow > row )
	{
		for ( int i = 0; i < numcol; ++i ) 
		{
			for ( int loop = 0; loop < numrow - row; ++loop )
			{
				ShopItemInfoEX* elem = m_elements[ i ][ m_elements[ i ].size() - 1 ];
				if ( elem ) {
					delete elem;
				}
				m_elements[ i ].pop_back();
			}
		}
	}
}

void	ShopWndTabController::SetElement( int col, int row, ShopItemInfoEX* elem )
{
	if ( ( col < 0 || GetNumCol() <= col ) || ( row < 0 || GetNumRow() <= row ) ) {
		return;
	}
	
	if ( m_elements[ col ][ row ] ) 
	{
		delete m_elements[ col ][ row ];
		m_elements[ col ][ row ] = 0;
	}

	if ( elem )
	{
		m_elements[ col ][ row ] = new ShopItemInfoEX();
		*m_elements[ col ][ row ] = *elem;
	}
}

int		ShopWndTabController::GetNumRow() const
{
	if ( m_elements.empty() ) {
		return 0;
	}
	return static_cast<int>( m_elements[ 0 ].size() );
}

ShopItemInfoEX*	ShopWndTabController::Get( int col, int row )
{
	if ( ( col < 0 || GetNumCol() <= col ) || ( row < 0 || GetNumRow() <= row ) ) {
		return 0;
	}
	return m_elements[ col ][ row ];
}

void	ShopWndTabController::DeleteRow( std::vector< ShopItemInfoEX* >& rows )
{
	std::vector< ShopItemInfoEX* >::iterator iter = rows.begin();
	std::vector< ShopItemInfoEX* >::iterator eiter = rows.end();
	for ( ; iter != eiter; ++iter ) {
		if ( *iter ) {
			delete *iter;
		}
	}
	rows.clear();
}

//----------------------------------------------------------------------------------
/*
Icon Click
*/
void	ShopWndTabController::OnClickShopIcon( ioWndEX* wnd, ioWndEXMouseEvent& event, int col, int row )
{
	if ( !g_GUIMgr.GetDeskTop() ) return;

	if ( m_vscrollbar ) {
		row += static_cast<int>( m_vscrollbar->GetScrollPos() );
	}
	if ( m_hscrollbar ) {
		col += static_cast<int>( m_hscrollbar->GetScrollPos() );
	}

	if ( col < 0 ) return;
	if ( row < 0 ) return;

	int numcol = GetNumCol();
	int numrow = GetNumRow();

	if ( col >= numcol ) return;
	if ( row >= numrow ) return;

	ShopBuyWndFrame* buywnd = (ShopBuyWndFrame*)( g_GUIMgr.GetDeskTop()->FindChildWnd( SHOP_BUY_WND_FRAME ) );
	if ( !buywnd )
	{
		buywnd = (ShopBuyWndFrame*)( g_GUIMgr.AddWnd( "xml/ShopBuyWndFrame.xml" ) );
		if ( buywnd && m_elements[ col ][ row ] )
		{		
			buywnd->SetShopWnd( m_shopwnd );
			buywnd->SetItemInfo( *( m_elements[ col ][ row ] ) );

			g_GUIMgr.SetModal( buywnd );
			buywnd->ShowWnd();
		}
	}
	else
	{
		buywnd->SetShopWnd( m_shopwnd );
		buywnd->ExceptFromID( true );

		g_GUIMgr.SetModal( buywnd );
		buywnd->ShowWnd();
	}
}