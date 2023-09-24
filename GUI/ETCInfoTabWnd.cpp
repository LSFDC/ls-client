//----------------------------------------------------------------------------------
/*
Filename	: DecoInfoTabWnd
Author		: sumin
Date		: 113.11.5_14:41:3
*/
//----------------------------------------------------------------------------------

#include "StdAfx.h"
#include "ETCInfoTabWnd.h"
#include "ioButtonWndEX.h"
#include "ioLabelWndEX.h"
#include "ioFrameWndEX.h"
#include "../ioPresentMgr.h"
#include "../ioItemMaker.h"
#include "../ioSkillMaker.h"

//----------------------------------------------------------------------------------
/*
»ý¼º, ÆÄ±«
*/
ETCInfoTabWnd::ETCInfoTabWnd()
	: ETCInfoTabWndBase()
	, ShopBuyTabCommon()
{
}

ETCInfoTabWnd::~ETCInfoTabWnd()
{
}

//----------------------------------------------------------------------------------
/*
control
*/
void	ETCInfoTabWnd::SetItemInfo( const ShopItemInfoEX& info )
{
	ShopBuyTabCommon::SetItemInfo( info );
	
	if ( m_lefttoplabel ) {
		m_lefttoplabel->SetLabelText( info.m_sName.c_str() );
	}

	if ( m_iconimg ) {
		m_iconimg->SetImageSet( info.m_sIcon.c_str() );
	}

	if ( m_descrich )
	{
		m_descrich->ClearText();
		m_descrich->AddText( 13, TS_NORMAL, 0x00000000, STR(1) );
	}
}