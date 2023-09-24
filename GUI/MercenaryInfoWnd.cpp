//----------------------------------------------------------------------------------
/*
Filename	: MercenaryInfoWnd
Author		: sumin
Date		: 113.11.5_14:41:3
*/
//----------------------------------------------------------------------------------

#include "StdAfx.h"
#include "MercenaryInfoWnd.h"
#include "ioSP2GUIManager.h"
#include "../ioMyInfo.h"

//----------------------------------------------------------------------------------
/*
»ý¼º, ÆÄ±«
*/
MercenaryInfoWnd::MercenaryInfoWnd()
	: MercenaryInfoWndBase() 
{
}

MercenaryInfoWnd::~MercenaryInfoWnd()
{
}

//----------------------------------------------------------------------------------
/*
ioWnd Event Override
*/
void	MercenaryInfoWnd::iwm_show()
{
	AddWndStyle( IWS_MODAL );

	MercenaryInfoWndBase::iwm_show();

	//g_MyInfo.
}

//----------------------------------------------------------------------------------
/*
Event Virtual Functions
*/
void	MercenaryInfoWnd::OnClickBuy( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
}

void	MercenaryInfoWnd::OnClickClose( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
	//g_GUIMgr.
}