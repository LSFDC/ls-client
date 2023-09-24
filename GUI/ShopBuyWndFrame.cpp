//----------------------------------------------------------------------------------
/*
Filename	: ShopBuyWndFrame
Author		: sumin
Date		: 113.11.5_14:41:3
*/
//----------------------------------------------------------------------------------

#include "StdAfx.h"
#include "ShopBuyWndFrame.h"
#include "ioButtonWndEX.h"
#include "ioLabelWndEX.h"
#include "ioFrameWndEX.h"
#include "MercenaryEquipInfoTabWnd.h"
#include "../ioPresentMgr.h"
#include "DecoInfoTabWnd.h"
#include "ETCInfoTabWnd.h"
#include "ShopBuyWnd.h"
#include "../WndID.h"

//----------------------------------------------------------------------------------
/*
생성, 파괴
*/
ShopBuyWndFrame::ShopBuyWndFrame()
	: ShopBuyWndFrameBase()
	, m_shopwnd( 0 )
	, m_meinfotabwnd( 0 )
	, m_decoinfotabwnd( 0 )
	, m_etcinfotabwnd( 0 )
{
}

ShopBuyWndFrame::~ShopBuyWndFrame()
{
}

//----------------------------------------------------------------------------------
/*
control
*/
void	ShopBuyWndFrame::SetItemInfo( const ShopItemInfoEX& info )
{
	m_iteminfo = info;

	if ( m_backframe )
	{
		switch ( m_iteminfo.m_iItemType )
		{
		case PRESENT_SOLDIER: //용병/장비일 경우
		case PRESENT_EXTRAITEM: // 장비
		case PRESENT_EXTRAITEM_BOX:
			{
				m_meinfotabwnd = (MercenaryEquipInfoTabWnd*)( g_GUIMgr.AddWnd( "xml/MercenaryEquipInfoTabWnd.xml", m_backframe ) );
				if ( m_meinfotabwnd )
				{
					m_meinfotabwnd->ExceptFromID( true );
					m_meinfotabwnd->SetPosX( 18 );
					m_meinfotabwnd->SetPosY( 65 );
					m_meinfotabwnd->SetShopWnd( m_shopwnd );
					m_meinfotabwnd->SetItemInfo( m_iteminfo );
				}
			}
			break;

		case PRESENT_DECORATION: // 치장일경우
			{
				m_decoinfotabwnd = (DecoInfoTabWnd*)( g_GUIMgr.AddWnd( "xml/DecoInfoTabWnd.xml", m_backframe ) );
				if ( m_decoinfotabwnd )
				{
					m_decoinfotabwnd->ExceptFromID( true );
					m_decoinfotabwnd->SetPosX( 18 );
					m_decoinfotabwnd->SetPosY( 65 );
					m_decoinfotabwnd->SetShopWnd( m_shopwnd );
					m_decoinfotabwnd->SetShopMode( true );
					m_decoinfotabwnd->SetItemInfo( m_iteminfo );
				}
			}
			break;

		default: // 그밖에
			{
				m_etcinfotabwnd = (ETCInfoTabWnd*)( g_GUIMgr.AddWnd( "xml/ETCInfoTabWnd.xml", m_backframe ) );
				if ( m_etcinfotabwnd )
				{
					m_etcinfotabwnd->ExceptFromID( true );
					m_etcinfotabwnd->SetPosX( 18 );
					m_etcinfotabwnd->SetPosY( 65 );
					m_etcinfotabwnd->SetItemInfo( m_iteminfo );
				}
			}
			break;
		}
	}
}

//----------------------------------------------------------------------------------
/*
Event Virtual Functions
*/
void	ShopBuyWndFrame::OnClickBuy( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
	ShopBuyWnd* buywnd = (ShopBuyWnd*)( g_GUIMgr.GetDeskTop()->FindChildWnd( SHOP_BUY_WND ) );
	if ( !buywnd ) {
		buywnd = (ShopBuyWnd*)( g_GUIMgr.AddWnd( "xml/ShopBuyWnd.xml" ) );
	}

	if ( buywnd )
	{
		buywnd->SetItemInfo( m_iteminfo );
		g_GUIMgr.SetModal( buywnd );
		buywnd->ShowWnd();
	}
}

void	ShopBuyWndFrame::OnClickClose( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
	HideWnd();
}