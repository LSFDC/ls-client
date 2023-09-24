//----------------------------------------------------------------------------------
/*
Filename	: ioGUIRegister.cpp
Author		: megamegy
Date		: 2013.10.16
Desc		: 
*/
//----------------------------------------------------------------------------------

#include "stdafx.h"
#include "ioGUIRegister.h"
#include "ioSP2GUIManager.h"
#include "CharacterControlWnd.h"
#include "ShopWnd.h"
#include "ShopWndWeeklyTab.h"
#include "ShopWndETCTab.h"
#include "ShopBuyWndFrame.h"
#include "MercenaryEquipInfoTabWnd.h"
#include "DecoInfoTabWnd.h"
#include "ETCInfoTabWnd.h"
#include "ShopBuyWnd.h"
#include "PaperMercenaryControlWnd.h"
#include "QuickLobbyMenu.h"
#include "QuickLobbyWnd.h"
#include "QuickLobbyExitWnd.h"

//#$%HEADERADDMARK%$#

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
ioGUIRegister
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
/*
Register
*/
void	ioGUIRegister::Register( ioSP2GUIManager* guimgr )
{
	if ( !guimgr ) {
		return;
	}
	/*
	guimgr->RegisterWndCreator<CharacterControlWnd>( "CharacterControlWnd" );
	guimgr->RegisterWndCreator<ShopWnd>( "ShopWnd" );
	guimgr->RegisterWndCreator<ShopWndWeeklyTab>( "ShopWndWeeklyTab" );
	guimgr->RegisterWndCreator<ShopWndETCTab>( "ShopWndETCTab" );
	guimgr->RegisterWndCreator<ShopBuyWndFrame>( "ShopBuyWndFrame" );
	guimgr->RegisterWndCreator<MercenaryEquipInfoTabWnd>( "MercenaryEquipInfoTabWnd" );
	guimgr->RegisterWndCreator<DecoInfoTabWnd>( "DecoInfoTabWnd" );
	guimgr->RegisterWndCreator<ETCInfoTabWnd>( "ETCInfoTabWnd" );
	guimgr->RegisterWndCreator<ShopBuyWnd>( "ShopBuyWnd" );
	guimgr->RegisterWndCreator<PaperMercenaryControlWnd>( "PaperMercenaryControlWnd" );
	*/

	guimgr->RegisterWndCreator<QuickLobbyMenu>( "QuickLobbyMenu" );
	guimgr->RegisterWndCreator<QuickLobbyWnd>( "QuickLobbyWnd" );
	guimgr->RegisterWndCreator<QuickLobbyTournament>( "QuickLobbyTournament" );
	guimgr->RegisterWndCreator<QuickLobbyTradeInfoWnd>( "QuickLobbyTradeInfoWnd" );
	guimgr->RegisterWndCreator<ioShuffleButtonEx>( "ioShuffleButtonEx" );
	guimgr->RegisterWndCreator<QuickLobbyExitWnd>( "QuickLobbyExitWnd" );

	
//#$%REGADDMARK%$#
}