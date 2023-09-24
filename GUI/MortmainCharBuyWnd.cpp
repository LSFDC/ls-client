#include "StdAfx.h"

#include "../io3DEngine/ioWnd.h"
#include "../io3DEngine/ioScroll.h"
#include "../io3DEngine/ioFontManager.h"
#include "../io3DEngine/ioUITitle.h"
#include "../io3DEngine/HelpFunc.h"
#include "../io3DEngine/iostringmanager.h"
#include "../io3dengine/safesprintf.h"

#include "../ioItemMaker.h"
#include "../ioSetItemInfo.h"
#include "../ioSetItemInfoManager.h"
#include "../ioClassPrice.h"
#include "../WndID.h"
#include "../EtcHelpFunc.h"
#include "../TextColorTable.h"

#include "ioSP2GUIManager.h"

#include ".\mortmaincharbuywnd.h"
#include <strsafe.h>
#include "..\TextColorTable.h"

CharBuyInfoBtn::CharBuyInfoBtn()
{
	m_iPeso = 0;
}

CharBuyInfoBtn::~CharBuyInfoBtn()
{

}

void CharBuyInfoBtn::SetInfo( int iSetIdx )
{
	const ioSetItemInfo *pInfo = g_SetItemInfoMgr.GetSetInfoByIdx( iSetIdx );
	if( !pInfo )
	{
		SetInActive();
		return;
	}

	SetActive();

	m_szCharName = pInfo->GetName();
	m_iPeso      = g_ClassPrice.GetMortmainCharPeso( pInfo->GetSetCode() - SET_ITEM_CODE );
}

void CharBuyInfoBtn::OnRender()
{
	ioButton::OnRender();
	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	if( m_dwStyle & IWS_INACTIVE )
		return;

	enum { NAME_X_OFFSET = 13, NAME_Y_OFFSET = -1, PESO_X_OFFSET = 176, PESO_Y_OFFSET = -1, };
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
	g_FontMgr.SetAlignType( TAT_LEFT );
	g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY ); // 노란
	g_FontMgr.PrintTextWidthCut( iXPos + NAME_X_OFFSET, iYPos + NAME_Y_OFFSET, FONT_SIZE_13, FLOAT100, m_szCharName.c_str() );
	g_FontMgr.SetAlignType( TAT_RIGHT );
	g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY ); // 주황
	g_FontMgr.PrintTextWidthCut( iXPos + PESO_X_OFFSET, iYPos + PESO_Y_OFFSET, FONT_SIZE_13, FLOAT100, STR(1), m_iPeso );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CharBuyInfoListWnd::CharBuyInfoListWnd()
{
	m_iCurPage = 0;
}

CharBuyInfoListWnd::~CharBuyInfoListWnd()
{

}

void CharBuyInfoListWnd::iwm_show()
{
	m_iCurPage = 0;
	UpdatePage( m_iCurPage );
	ioScroll *pScroll = (ioScroll*)FindChildWnd( ID_VERT_SCROLL );
	if( pScroll )
	{
		int iMaxPos = g_ClassPrice.MaxActiveClassPrice() / PAGE_PER_BTN_NUM;
		if( g_ClassPrice.MaxActiveClassPrice() % PAGE_PER_BTN_NUM )
			iMaxPos++;
		pScroll->SetScrollRange( 0, iMaxPos );
		pScroll->SetScrollPos( m_iCurPage );
	}
}

void CharBuyInfoListWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_LIST_1:
	case ID_LIST_2:
	case ID_LIST_3:
	case ID_LIST_4:
	case ID_LIST_5:
	case ID_LIST_6:
	case ID_LIST_7:
	case ID_LIST_8:
	case ID_LIST_9:
	case ID_LIST_10:
		if( cmd == IOBN_BTNUP )
		{
			SetParentInfo( dwID );
			HideWnd();
		}
		break;
	}
}

void CharBuyInfoListWnd::iwm_wheel( int zDelta )
{
	if( !IsShow() )
		ioWnd::iwm_wheel( zDelta );
	else
	{ 
		// 휠 이벤트 처리를 했다면 부모로 이벤트를 날리지 않는다.
		if( zDelta == WHEEL_DELTA )
		{
			ioScroll *pScroll = (ioScroll*)FindChildWnd( ID_VERT_SCROLL );
			if( pScroll )
				pScroll->SetScrollPos( m_iCurPage - 1 );
			if( g_GUIMgr.GetPreOverWnd() )
			{
				ioMouse NullMouse;
				g_GUIMgr.GetPreOverWnd()->iwm_mouseover( NullMouse );
			}
		}
		else if( zDelta == -WHEEL_DELTA )
		{
			ioScroll *pScroll = (ioScroll*)FindChildWnd( ID_VERT_SCROLL );
			if( pScroll )
				pScroll->SetScrollPos( m_iCurPage + 1 );
			if( g_GUIMgr.GetPreOverWnd() )
			{
				ioMouse NullMouse;
				g_GUIMgr.GetPreOverWnd()->iwm_mouseover( NullMouse );
			}
		}
	}
}

void CharBuyInfoListWnd::iwm_vscroll( DWORD dwID, int curPos )
{
	UpdatePage( curPos );
}

void CharBuyInfoListWnd::UpdatePage( int iNewPage )
{
	m_iCurPage = iNewPage;

	/*ShopWnd *pShop = dynamic_cast<ShopWnd*> ( g_GUIMgr.FindWnd( SHOP_WND ) );
	if( !pShop )
	{
		LOG.PrintTimeAndLog( 0, "%s pShop == NULL.", __FUNCTION__ );
		return;
	}

	ShopSoldierTab *pSoldierTab = dynamic_cast< ShopSoldierTab* > ( pShop->FindChildWnd( ShopWnd::ID_SOLDIER_TAB ) );
	if( !pSoldierTab )
	{
		LOG.PrintTimeAndLog( 0, "%s pSoldierTab == NULL.", __FUNCTION__ );
		return;
	}

	int iCurStartArray = m_iCurPage * PAGE_PER_BTN_NUM;
	int iMaxArray      = pSoldierTab->GetItemInfoListSize();

	for (int i = 0; i < PAGE_PER_BTN_NUM ; i++)
	{
		int iCurArray = iCurStartArray + i;
		CharBuyInfoBtn *pBtn = dynamic_cast<CharBuyInfoBtn*> ( FindChildWnd( ID_LIST_1 + i ) );
		if( !pBtn )
			continue;
		pBtn->SetInfo( pSoldierTab->GetSetIdx( iCurArray ) );
	}*/
}

void CharBuyInfoListWnd::SetParentInfo( DWORD dwID )
{
	/*ShopWnd *pShop = dynamic_cast<ShopWnd*> ( g_GUIMgr.FindWnd( SHOP_WND ) );
	if( !pShop )
	{
		LOG.PrintTimeAndLog( 0, "%s pShop == NULL.", __FUNCTION__ );
		return;
	}

	ShopSoldierTab *pSoldierTab = dynamic_cast< ShopSoldierTab* > ( pShop->FindChildWnd( ShopWnd::ID_SOLDIER_TAB ) );
	if( !pSoldierTab )
	{
		LOG.PrintTimeAndLog( 0, "%s pSoldierTab == NULL.", __FUNCTION__ );
		return;
	}

	if( !GetParent() )
	{
		LOG.PrintTimeAndLog( 0, "%s Parent == NULL.", __FUNCTION__ );
		return;
	}
	
	MortmainCharBuyWnd *pWnd = dynamic_cast<MortmainCharBuyWnd*> ( GetParent() );
	if( !pWnd )
	{
		LOG.PrintTimeAndLog( 0, "%s Parent Wnd == NULL.", __FUNCTION__ );
		return;
	}
	
	int iCurStartArray = m_iCurPage * PAGE_PER_BTN_NUM;
	int iCurArray = iCurStartArray + ( dwID - ID_LIST_1 );
	pWnd->SetInfo( pSoldierTab->GetSetIdx( iCurArray ) );*/
}

void CharBuyInfoListWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() )
		return;

	// 다른 곳에서 클릭이 일어났는지 확인
	ioWnd *pPreClick = g_GUIMgr.GetPreClickWnd();
	if( !pPreClick )
		return;
		
	if( pPreClick == this )
		return;

	ioWnd *pParent = pPreClick->GetParent();
	if( !pParent )
	{
		HideWnd();
		return;
	}
	
	if( pParent == this )
		return;

	ioWnd *pGrandParent = pParent->GetParent();
	if( !pGrandParent )
	{
		HideWnd();
		return;
	}

	if( pGrandParent == this )
		return;
	
	HideWnd();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MortmainCharBuyWnd::MortmainCharBuyWnd(void)
{
	m_iSetIdx = 0;
}

MortmainCharBuyWnd::~MortmainCharBuyWnd(void)
{
}

void MortmainCharBuyWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_EXIT_BTN:
		if( cmd == IOBN_BTNUP )
		{
			HideWnd();
		}
		break;
	case ID_CHANGE_CHAR_BTN:
		if( cmd == IOBN_BTNUP )
		{
			ioWnd *pChild = FindChildWnd( ID_CHAR_LIST_WND );
			if( pChild )
			{
				if( pChild->IsShow() )
					pChild->HideWnd();
				else
					pChild->ShowWnd();
			}
		}
		break;
	case ID_BUY_BTN:
		if( cmd == IOBN_BTNUP )
		{
			OnMortMainCharBuy();
			HideWnd();
		}
		break;
	}
}

void MortmainCharBuyWnd::OnRender()
{
	ioWnd::OnRender();
	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	enum { X_OFFSET = 215, Y_OFFSET = 80, LINE_GAP = 20, };
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY ); // 노란
	iXPos += X_OFFSET;
	iYPos += Y_OFFSET;
	g_FontMgr.PrintText( iXPos, iYPos, FONT_SIZE_15, STR(1) );
	g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY ); // 주황
	iYPos += LINE_GAP;
	g_FontMgr.PrintText( iXPos, iYPos, FONT_SIZE_15, STR(2) );
	iYPos += LINE_GAP;
	g_FontMgr.PrintText( iXPos, iYPos, FONT_SIZE_15, STR(3) );
}

void MortmainCharBuyWnd::SetInfo( int iSetIdx )
{
	m_iSetIdx = iSetIdx;
	SetCharBtnTitle( m_iSetIdx );
}

void MortmainCharBuyWnd::iwm_show()
{
// 	ShopWnd *pShop = dynamic_cast<ShopWnd*> ( g_GUIMgr.FindWnd( SHOP_WND ) );
// 	if( !pShop )
// 	{
// 		LOG.PrintTimeAndLog( 0, "%s pShop == NULL.", __FUNCTION__ );
// 		return;
// 	}
// 
// 	ShopSoldierTab *pSoldierTab = dynamic_cast< ShopSoldierTab* > ( pShop->FindChildWnd( ShopWnd::ID_SOLDIER_TAB ) );
// 	if( !pSoldierTab )
// 	{
// 		LOG.PrintTimeAndLog( 0, "%s pSoldierTab == NULL.", __FUNCTION__ );
// 		return;
// 	}
// 
// 	pSoldierTab->SortItemInfo();
// 	m_iSetIdx = pSoldierTab->GetSetIdx( 0 );
// 
// 	SetCharBtnTitle( m_iSetIdx );
}

void MortmainCharBuyWnd::OnMortMainCharBuy()
{
	/*ShopWnd *pShop = dynamic_cast<ShopWnd*> ( g_GUIMgr.FindWnd( SHOP_WND ) );
	if( !pShop )
	{
		LOG.PrintTimeAndLog( 0, "%s pShop == NULL.", __FUNCTION__ );
		return;
	}

	ShopSoldierBuyTab *pSoldierBuyTab = dynamic_cast< ShopSoldierBuyTab* > ( pShop->FindChildWnd( ShopWnd::ID_SOLDIER_BUY_TAB ) );
	if( !pSoldierBuyTab )
	{
		LOG.PrintTimeAndLog( 0, "%s pSoldierBuyTab == NULL.", __FUNCTION__ );
		return;
	}

	pSoldierBuyTab->OnMortmainCharBuy( m_iSetIdx );*/
}

void MortmainCharBuyWnd::SetCharBtnTitle( int iSetIdx )
{
	ioWnd *pChild = FindChildWnd( ID_CHANGE_CHAR_BTN );
	if( !pChild )
	{
		LOG.PrintTimeAndLog( 0, "%s pChild == NULL.", __FUNCTION__ );
		return;
	}

	ioUITitle *pTitle = pChild->GetTitle();
	if( !pTitle )
	{
		LOG.PrintTimeAndLog( 0, "%s pTitle == NULL.", __FUNCTION__ );
		return;
	}

	const ioSetItemInfo *pInfo = g_SetItemInfoMgr.GetSetInfoByIdx( iSetIdx );
	if( !pInfo )
	{
		LOG.PrintTimeAndLog( 0, "%s pInfo == NULL.", __FUNCTION__ );
		return;
	}

	char szText[MAX_PATH]="";
	int iPeso = g_ClassPrice.GetMortmainCharPeso( pInfo->GetSetCode() - SET_ITEM_CODE );
	SafeSprintf( szText, sizeof( szText ), STR(1), pInfo->GetName().c_str(), iPeso );

	pTitle->SetWidthCutSize( 181.0f );
	pTitle->SetPrintTextStyle( ioUITitle::PTS_WIDTHCUT );
	pTitle->SetText( szText, "" );
}

void MortmainCharBuyWnd::SetPositionToDefault( int iParentWidth, int iParentHeight, bool bChildMove, bool bCreate /*= false */ )
{
	ioWnd::SetPositionToDefault( iParentWidth, iParentHeight, bChildMove, bCreate );
	enum { IMG_HEIGHT = 184, };
	CheckFrameReSize( GetWidth() , IMG_HEIGHT, GetWidth() , IMG_HEIGHT );
}