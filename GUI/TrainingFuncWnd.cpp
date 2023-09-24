
#include "StdAfx.h"

#include "TrainingFuncWnd.h"

#include "PartyMenuWnd.h"
#include "PlazaMainWnd.h"
#include "TradeMainWnd.h"
#include "GlobalWnd.h"
#include "QuickModeToolTip.h"

#include "../ioLadderTeamMgr.h"
#include "../ioShuffleRoomMgr.h"

TrainingFuncWnd::TrainingFuncWnd()
{
	m_pModeHelpTooltip  = NULL;
	m_pQuickJoinTooltip = NULL;
	m_dwCurrMenu		= NONE;
	m_FuncMsgType		= FMT_NONE;
}

TrainingFuncWnd::~TrainingFuncWnd()
{
}

void TrainingFuncWnd::iwm_create()
{
	g_GUIMgr.AddWnd( "XML/partycataloguewnd.xml", this );
	g_GUIMgr.AddWnd( "XML/plazalistwnd.xml", this );
	g_GUIMgr.AddWnd( "XML/trademainwnd.xml", this );
	g_GUIMgr.AddWnd( "XML/shuffletooptip.xml", this );
}

void TrainingFuncWnd::iwm_show()
{
	PartyCatalogueWnd* pParty = dynamic_cast<PartyCatalogueWnd*>( FindChildWnd( ID_BATTLE ) );
	if( pParty )
	{
		pParty->SetMaxPartyList( PartyCatalogueWnd::PLAZA_MAX_PARTY_LIST );
		pParty->PlazaUIMove();
	}

	PlazaListWnd* pPlaza = dynamic_cast<PlazaListWnd*>( FindChildWnd( ID_PLAZA ) );
	if( pPlaza )
	{
		pPlaza->SetMaxPlazaList( PlazaListWnd::PLAZA_MAX_PLAZA_LIST );
		pPlaza->PlazaUIMove();
	}

	LobbyOwnerTradeListWnd* pLobbyOwnerTradeListWnd = dynamic_cast<LobbyOwnerTradeListWnd*>( FindChildWnd( ID_TRADE_INFO ) );
	if( pLobbyOwnerTradeListWnd )
	{
		pLobbyOwnerTradeListWnd->SetMaxTradeList( LobbyOwnerTradeListWnd::OWNER_MAX_TRADE );
		pLobbyOwnerTradeListWnd->PlazaUIMove( 54 );
	}

	m_FuncMsgType = FMT_NONE;
}

void TrainingFuncWnd::iwm_hide()
{
	ResetCurrMenu();

	ModeHelpTooltip *pModeHelpTooltip = dynamic_cast<ModeHelpTooltip*>( m_pModeHelpTooltip );	
	if( pModeHelpTooltip )
		pModeHelpTooltip->SetModeHelpTooltip( NULL, "", -1 );

	CQuickModeToolTip *pQuickTooltip = dynamic_cast<CQuickModeToolTip*>( m_pQuickJoinTooltip );
	if( pQuickTooltip )
		pQuickTooltip->SetModeHelpTooltip( NULL, -1 );

	HideChildWnd( ID_BATTLE );
	HideChildWnd( ID_PLAZA );
	HideChildWnd( ID_TRADE );
	HideChildWnd( ID_TRADE_INFO );
}

bool TrainingFuncWnd::iwm_esc()
{
	HideWnd();
	return true;
}

void TrainingFuncWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();	
	switch( dwID )
	{
	case ID_EXIT:
		if( cmd == IOBN_BTNUP )
			HideWnd();
		break;
	case MESSAGE_BOX3:
		if( cmd == IOBN_BTNUP )
		{
			if( param == IDYES )
			{
				switch( m_FuncMsgType )
				{
				case FMT_HQ:
					OnCreateHeadquarters();
					break;
				case FMT_HOME:
					OnCreateHouse();
					break;
				case FMT_GUILD_HQ:
					OnCreateGuildHeadquarters();
					break;
				}
			}
		}
		break;
	}
}

void TrainingFuncWnd::OnProcess( float fTimePerSec )
{
	ioWnd* pWnd = FindChildWnd( ID_TRADE_INFO );
	if( pWnd && !pWnd->IsShow() )
	{
		TradeItemInfoList List;
		g_TradeInfoMgr.GetTradeItemListByUser( List, g_MyInfo.GetUserIndex() );
		if( !List.empty() )
		{
			ShowInfo( ID_TRADE, true );
		}
	}
}

void TrainingFuncWnd::SetModeHelpTooltip( ioWnd *pCallWnd, const ioHashString &szTitle, int iQuickstartModeHelpIndex )
{
	if( m_pModeHelpTooltip == NULL )
		m_pModeHelpTooltip = dynamic_cast< ModeHelpTooltip * >( FindChildWnd( ID_MODE_HELP_TOOLTIP ) );

	ModeHelpTooltip *pModeHelpTooltip = dynamic_cast< ModeHelpTooltip * >( m_pModeHelpTooltip );	
	if( pModeHelpTooltip )
		pModeHelpTooltip->SetModeHelpTooltip(  pCallWnd, szTitle, iQuickstartModeHelpIndex );
}

void TrainingFuncWnd::SetQuickJoinTooltip( ioWnd *pCallWnd, const int nMode )
{
	if( m_pQuickJoinTooltip == NULL )
		m_pQuickJoinTooltip = dynamic_cast<CQuickModeToolTip*>( FindChildWnd( ID_QUICKJOIN_TOOLTIP ) );

	CQuickModeToolTip *pQuickTooltip = dynamic_cast<CQuickModeToolTip*>( m_pQuickJoinTooltip );
	if( pQuickTooltip )
		pQuickTooltip->SetModeHelpTooltip( pCallWnd, nMode );
}

void TrainingFuncWnd::SetShuffleToolTip( bool bShow )
{
	if( bShow )
	{
		ioWnd *pWnd = FindChildWnd( ID_SHUFFLE_TOOLTIP );
		if( pWnd && !pWnd->IsShow() )
			pWnd->ShowWnd();
	}
	else
	{
		HideChildWnd( ID_SHUFFLE_TOOLTIP );
	}
}

void TrainingFuncWnd::SetPositionToDefault( int iParentWidth, int iParentHeight, bool bChildMove, bool bCreate )
{
	ioMovingWnd::SetPositionToDefault( iParentWidth, iParentHeight, bChildMove, bCreate );

	if( m_dwCurrMenu != NONE )
	{
		//Hide�ÿ� �޴� �ε��� �ʱ�ȭ������ ���
		DWORD dwPrevMenu = m_dwCurrMenu;
		HideWnd();
		ShowMenu( dwPrevMenu );
	}
}

void TrainingFuncWnd::ShowMenu( DWORD dwWndID )
{
	ShowWnd();
	
	HideChildWnd( ID_BATTLE );
	HideChildWnd( ID_PLAZA );
	HideChildWnd( ID_TRADE );
	HideChildWnd( ID_TRADE_INFO );

	ioWnd* pWnd = FindChildWnd( dwWndID );
	if( pWnd )
	{
		SetTitle( dwWndID );
		CalcSize( dwWndID );
		pWnd->SetWndPos( 17, 50 );
		pWnd->ShowWnd();
		ShowInfo( dwWndID );
		m_dwCurrMenu = dwWndID;
	}
}

void TrainingFuncWnd::ShowInfo( DWORD dwWndID, bool bReCalcPos )
{
	ioWnd* pWnd = NULL;
	switch( dwWndID )
	{
	case ID_TRADE:
		{
			LobbyOwnerTradeListWnd* pLobbyOwnerTradeListWnd = dynamic_cast<LobbyOwnerTradeListWnd*>( FindChildWnd( ID_TRADE_INFO ) );
			if( bReCalcPos )
				pWnd = pLobbyOwnerTradeListWnd;
			else
			{
				if( pLobbyOwnerTradeListWnd )
					pLobbyOwnerTradeListWnd->OnRefreshTradeItemList();
			}				
		}
		break;
	}

	if( pWnd )
	{
		pWnd->SetWndPos( 439, 0 );
		pWnd->ShowWnd();

		//������ ��ġ ����(Info �����츦 ������ ũ�⸦ ��������)
		Layout( 645, 526 );
	}
	else
	{
		//������ ��ġ ����(�⺻ ������ ũ�⸦ ��������)
		Layout( GetDefaultWidth(), GetDefaultHeight() );
	}
}

void TrainingFuncWnd::SetTitle( DWORD dwWndID )
{
	switch( dwWndID )
	{
	case ID_BATTLE: 
		SetTitleText( STR(1) );
		break;
	case ID_PLAZA: 
		SetTitleText( STR(2) );
		break;
	case ID_TRADE:
		SetTitleText( STR(3) );
		break;
	default:
		SetTitleText( "" );
	}
}

void TrainingFuncWnd::CalcSize( DWORD dwWndID )
{
	switch( dwWndID )
	{
	case ID_BATTLE:
	case ID_PLAZA:
		SetSize( 437, 416 );
		break;
	default:
		SetSize( m_iDefWidth, m_iDefHeight );
		break;
	}
}

void TrainingFuncWnd::Layout( int iWidth, int iHeight )
{
	if( !m_pParent )
		return;

	int parentx      = m_pParent->GetXPos();
	int parenty      = m_pParent->GetYPos();
	int parentwidth  = m_pParent->GetWidth();
	int parentheight = m_pParent->GetHeight();

	RECT rcRect = { 0, 0, 0, 0 };
	rcRect.left = m_iDefXPos;
		
	switch( m_XPosAlign )
	{
	case UI_LEFT_ALIGN:
		rcRect.left = parentx;
		break;
	case UI_CENTER_ALIGN:
		rcRect.left = parentwidth / 2 - iWidth / 2;
		break;
	case UI_RATE_ALIGN:
		rcRect.left = parentwidth * m_fScreenRateX;
		break;
	case UI_RIGHT_ALIGN:
		rcRect.left = parentx + parentwidth - iWidth;
		break;
	}

	rcRect.top = m_iDefYPos;
	switch( m_YPosAlign )
	{
	case UI_TOP_ALIGN:
		rcRect.top = parenty;
		break;
	case UI_CENTER_ALIGN:
		rcRect.top = parentheight / 2 - iHeight / 2;
		break;
	case UI_RATE_ALIGN:
		rcRect.top = parenty * m_fScreenRateY;
		break;
	case UI_BOTTOM_ALIGN:
		rcRect.top = parenty + parentheight - iHeight;
		break;
	}

	rcRect.right  = rcRect.left + GetWidth();
	rcRect.bottom = rcRect.top + GetHeight();

	m_rcPos	= rcRect;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TrainingFuncWnd::ApplyNormalTeamList( SP2Packet &rkPacket )
{
	PartyCatalogueWnd *pCatalogueWnd = dynamic_cast<PartyCatalogueWnd*>( FindChildWnd( ID_BATTLE ) );
	if( pCatalogueWnd && pCatalogueWnd->IsShow() )
		pCatalogueWnd->SetPartyList( rkPacket );
}

void TrainingFuncWnd::RefreshBattleList( DWORD dwIndex )
{
	PartyCatalogueWnd *pCatalogueWnd = dynamic_cast<PartyCatalogueWnd*>( FindChildWnd( ID_BATTLE ) );
	if( pCatalogueWnd && pCatalogueWnd->IsShow() )
	{
		if( dwIndex == 0 || pCatalogueWnd->IsIndexToList( dwIndex ) )
			pCatalogueWnd->StartSend( false );
	}
}

void TrainingFuncWnd::ApplyPlazaList( SP2Packet &rkPacket )
{
	PlazaListWnd *pPlazaListWnd = dynamic_cast<PlazaListWnd*>( FindChildWnd( ID_PLAZA ) );
	if( pPlazaListWnd && pPlazaListWnd->IsShow() )
		pPlazaListWnd->SetPlazaList( rkPacket );
}

void TrainingFuncWnd::RefreshTradeItemList()
{
	TradeMainWnd *pTradeWnd = dynamic_cast<TradeMainWnd*>( FindChildWnd( ID_TRADE ) );
	if( pTradeWnd )
		pTradeWnd->OnRefreshTradeItemList();

	LobbyOwnerTradeListWnd *pLobbyOwnerTradeListWnd = dynamic_cast<LobbyOwnerTradeListWnd*>( FindChildWnd( ID_TRADE_INFO ) );
	if( pLobbyOwnerTradeListWnd )
		pLobbyOwnerTradeListWnd->OnRefreshTradeItemList();
}

void TrainingFuncWnd::ShowHeadquartersMsg()
{
	HideWnd();

	ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
	kPrinter[0].SetTextStyle( TS_NORMAL );
	kPrinter[0].SetBkColor( 0, 0, 0 );
	kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(1) );
	kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(1) );

	g_GUIMgr.SetPrevMsgListBoxWithTitle( NULL, MB_YESNO, this, kPrinter, STR(2), "", STR(3), STR(4) );
	m_FuncMsgType = FMT_HQ;
}

void TrainingFuncWnd::ShowHomeMsg()
{
	HideWnd();
	ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
	kPrinter[0].SetTextStyle( TS_NORMAL );
	kPrinter[0].SetBkColor( 0, 0, 0 );
	kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(1) );

	g_GUIMgr.SetPrevMsgListBoxWithTitle( NULL, MB_YESNO, this, kPrinter, STR(2), "", STR(3), STR(4) );
	m_FuncMsgType = FMT_HOME;
}

void TrainingFuncWnd::ShowGuildHQMsg()
{
	HideWnd();
	ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
	kPrinter[0].SetTextStyle( TS_NORMAL );
	kPrinter[0].SetBkColor( 0, 0, 0 );
	kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(1) );

	g_GUIMgr.SetPrevMsgListBoxWithTitle( NULL, MB_YESNO, this, kPrinter, STR(2), "", STR(3), STR(4) );
	m_FuncMsgType = FMT_GUILD_HQ;
}

void TrainingFuncWnd::OnCreateHeadquarters()
{
	if( g_App.IsMouseBusy() )
		return;

	if( g_BattleRoomMgr.IsBattleRoom() )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return;
	}

	g_MyInfo.SetJoinHeadquartersRequestName( "" );
	SP2Packet kPacket( CTPK_JOIN_HEADQUARTERS );
	kPacket << g_MyInfo.GetPublicID();
	kPacket << g_ModeSelectInfo.CheckHeadquartersMapIndex( g_ModeSelectInfo.GetHeadquartersModeIndex(), 1 );
	kPacket << false;
	TCPNetwork::SendToServer( kPacket );
	TCPNetwork::MouseBusy( true );
}

void TrainingFuncWnd::OnCreateHouse()
{
	if( g_App.IsMouseBusy() )
		return;

	if( g_BattleRoomMgr.IsBattleRoom() )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return;
	}

	g_MyInfo.SetJoinHeadquartersRequestName( "" );
	SP2Packet kPacket( CTPK_JOIN_PERSONAL_HQ );
	kPacket << g_MyInfo.GetPublicID();
	kPacket << g_ModeSelectInfo.CheckHouseMapIndex( g_ModeSelectInfo.GetHouseModeIndex(), 1 );
	kPacket << false;
	TCPNetwork::SendToServer( kPacket );
	TCPNetwork::MouseBusy( true );
}

void TrainingFuncWnd::OnCreateGuildHeadquarters()
{
	if( g_App.IsMouseBusy() )
		return;	

	if( g_BattleRoomMgr.IsBattleRoom() )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return;
	}

	if( g_LadderTeamMgr.IsLadderTeam() )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return;
	}

	if( g_ShuffleRoomMgr.IsShuffleRoom() )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		return;
	}

	SP2Packet kPacket( CTPK_ENTER_GUILD_ROOM );

#if defined( SRC_ID ) || defined( SRC_SEA )
	kPacket << 17;
#else
	kPacket << 16;
#endif

	TCPNetwork::SendToServer( kPacket );
	TCPNetwork::MouseBusy( true );
}

bool TrainingFuncWnd::QuestGuide( int iModeSelectIndex, float &rfXPos, float &rfYPos )
{
	PartyCatalogueWnd *pCatalogueWnd = dynamic_cast<PartyCatalogueWnd*>( FindChildWnd( ID_BATTLE ) );
	if( pCatalogueWnd && pCatalogueWnd->IsShow() )
		return pCatalogueWnd->QuestGuide( iModeSelectIndex, rfXPos, rfYPos );

	return false;
}