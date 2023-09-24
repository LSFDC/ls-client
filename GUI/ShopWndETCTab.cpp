//----------------------------------------------------------------------------------
/*
Filename	: ShopWndETCTab
Author		: sumin
Date		: 113.11.5_14:41:3
*/
//----------------------------------------------------------------------------------

#include "StdAfx.h"
#include "ShopWndETCTab.h"
#include "ioButtonWndEX.h"
#include "ioLabelWndEX.h"
#include "ioFrameWndEX.h"

//----------------------------------------------------------------------------------
/*
»ý¼º, ÆÄ±«
*/
ShopWndETCTab::ShopWndETCTab()
	: ioShopWndEtcTabBase()
{
}

ShopWndETCTab::~ShopWndETCTab()
{
}

//----------------------------------------------------------------------------------
/*
ioWnd Event Override
*/
void	ShopWndETCTab::iwm_create()
{
	ioShopWndEtcTabBase::iwm_create();
}

//----------------------------------------------------------------------------------
/*
ShopWndTabController override
*/
void	ShopWndETCTab::SubmitElement( int uicol, int uirow, ShopItemInfoEX* elem )
{
	int idx = uirow * 5 + uicol;
	if ( idx < 0 || idx >= ELEMENT_NUM ) {
		return;
	}

	if ( !elem ) 
	{
		if ( m_backfrm[ idx ] ) {
			m_backfrm[ idx ]->HideWnd();
		}
		if ( m_pricebackfrm[ idx ] ) {
			m_pricebackfrm[ idx ]->HideWnd();
		}
		if ( m_iconbtn[ idx ] ) {
			m_iconbtn[ idx ]->HideWnd();
		}
		if ( m_namelabel[ idx ] ) {	
			m_namelabel[ idx ]->HideWnd();
		}
		if ( m_pricelabel[ idx ] ) {
			m_pricelabel[ idx ]->HideWnd();
		}
		return;
	}
	
	bool showpeso = elem->IsShowPeso();

	if ( m_backfrm[ idx ] ) 
	{
		m_backfrm[ idx ]->ShowWnd();
		if ( showpeso ) {
			m_backfrm[ idx ]->SetColor( 0xFFB0B0FF );
		} else {
			m_backfrm[ idx ]->SetColor( 0xFFFFB0B0 );
		}
	}

	if ( m_pricebackfrm[ idx ] ) 
	{
		m_pricebackfrm[ idx ]->ShowWnd();
		if ( showpeso ) {
			m_pricebackfrm[ idx ]->SetColor( 0xFFB0B0FF );
		} else {
			m_pricebackfrm[ idx ]->SetColor( 0xFFFFB0B0 );
		}
	}

	if ( m_iconbtn[ idx ] ) 
	{
		m_iconbtn[ idx ]->ShowWnd();
		m_iconbtn[ idx ]->SetImageSetNormal( elem->m_sIcon.c_str() );
		m_iconbtn[ idx ]->SetImageSetOver( elem->m_sIcon.c_str() );
		m_iconbtn[ idx ]->SetImageSetPush( elem->m_sIcon.c_str() );
		m_iconbtn[ idx ]->SetImageSetDisable( elem->m_sIcon.c_str() );
	}
	
	if ( m_namelabel[ idx ] )
	{
		m_namelabel[ idx ]->ShowWnd();
		m_namelabel[ idx ]->SetLabelText( elem->m_sName.c_str() );
	}

	if ( m_pricelabel[ idx ] ) 
	{
		m_pricelabel[ idx ]->ShowWnd();

		m_pricelabel[ idx ]->ClearText();
		m_pricelabel[ idx ]->AddText( 13, TS_BOLD, 0xFFFFFFFF, elem->GetPrice() );
		m_pricelabel[ idx ]->AddText( 13, TS_NORMAL, 0xFFFEF0A0, elem->GetPriceUnit() );
	}
}