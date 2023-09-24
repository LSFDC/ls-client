#include "StdAfx.h"

#include "QuickLobbyWnd.h"
#include "QuickLobbyMenu.h"
#include "CampMainWnd.h"
#include "PlazaMainWnd.h"
#include "TradeMainWnd.h"
#include "TournamentMainWnd.h"
#include "GlobalWnd.h"
#include "QuickModeToolTip.h"

#include "TimeGateSearchShuffle.h"

#include "PartyMenuWnd.h"
#include "LadderTeamWnd.h"

QuickLobbyWnd::QuickLobbyWnd()
{
	m_pModeHelpTooltip  = NULL;
	m_pQuickJoinTooltip = NULL;
	m_dwCurrMenu		= NONE;
}

QuickLobbyWnd::~QuickLobbyWnd()
{
}

void QuickLobbyWnd::iwm_create()
{
	QuickLobbyWndBase::iwm_create();

	//����
	PartyCatalogueWnd* pParty = (PartyCatalogueWnd*)( g_GUIMgr.AddWnd( "XML/partycataloguewnd.xml", this ) );
	if( pParty )
	{
		pParty->SetMaxPartyList( PartyCatalogueWnd::PLAZA_MAX_PARTY_LIST );
		pParty->PlazaUIMove();
	}

	//������
	CampMainWnd* pCamp = (CampMainWnd*)( g_GUIMgr.AddWnd( "XML/campmainwnd.xml", this ) );
	if( pCamp )
	{
		LadderTeamListWnd* pTeamWnd = (LadderTeamListWnd*)( pCamp->FindChildWnd( CampMainWnd::ID_CAMP_TEAM_WND ) );
		if( pTeamWnd )
		{
			pTeamWnd->SetMaxTeamList( LadderTeamListWnd::PLAZA_MAX_TEAM_LIST );
			pTeamWnd->PlazaUIMove();
		}
	}

	g_GUIMgr.AddWnd( "XML/lobbycampinfownd.xml", this );
	PlazaListWnd* pPlaza = (PlazaListWnd*)( g_GUIMgr.AddWnd( "XML/plazalistwnd.xml", this ) );
	if( pPlaza )
	{
		pPlaza->SetMaxPlazaList( PlazaListWnd::PLAZA_MAX_PLAZA_LIST );
		pPlaza->PlazaUIMove();
	}

	//�ŷ���
	g_GUIMgr.AddWnd( "XML/trademainwnd.xml", this );
	g_GUIMgr.AddWnd( "XML/quickLobbytradeinfownd.xml", this );

	//��ȸ���
	g_GUIMgr.AddWnd( "XML/tournamentmainwnd.xml", this );
	g_GUIMgr.AddWnd( "XML/tournamentinfownd.xml", this );

	//������ ��� ����
	g_GUIMgr.AddWnd( "XML/shuffletooptip.xml", this );

}

void QuickLobbyWnd::iwm_show()
{
	PartyCatalogueWnd* pParty = (PartyCatalogueWnd*)( FindChildWnd( BATTLE ) );
	if( pParty )
	{
		pParty->SetMaxPartyList( PartyCatalogueWnd::PLAZA_MAX_PARTY_LIST );
		pParty->PlazaUIMove();
	}

	PlazaListWnd* pPlaza = (PlazaListWnd*)( FindChildWnd( PLAZA ) );
	if( pPlaza )
	{
		pPlaza->SetMaxPlazaList( PlazaListWnd::PLAZA_MAX_PLAZA_LIST );
		pPlaza->PlazaUIMove();
	}

	CampMainWnd* pCamp = (CampMainWnd*)( FindChildWnd( CAMP ) );
	if( pCamp )
	{
		LadderTeamListWnd* pTeamWnd = (LadderTeamListWnd*)( pCamp->FindChildWnd( CampMainWnd::ID_CAMP_TEAM_WND ) );
		if( pTeamWnd )
		{
			pTeamWnd->SetMaxTeamList( LadderTeamListWnd::PLAZA_MAX_TEAM_LIST );
			pTeamWnd->PlazaUIMove();
		}
	}
}

void QuickLobbyWnd::SetPositionToDefault( int iParentWidth, int iParentHeight, bool bChildMove, bool bCreate )
{
	QuickLobbyWndBase::SetPositionToDefault( iParentWidth, iParentHeight, bChildMove, bCreate );

	if( m_dwCurrMenu != NONE )
	{
		//Hide�ÿ� �޴� �ε��� �ʱ�ȭ������ ���
		DWORD dwPrevMenu = m_dwCurrMenu;
		HideWnd();
		ShowMenu( dwPrevMenu );
	}
}

void QuickLobbyWnd::iwm_hide()
{
	ResetCurrMenu();

	ModeHelpTooltip *pModeHelpTooltip = (ModeHelpTooltip*)( m_pModeHelpTooltip );	
	if( pModeHelpTooltip )
	{
		pModeHelpTooltip->SetModeHelpTooltip( NULL, "", -1 );
	}	

	CQuickModeToolTip *pQuickTooltip = (CQuickModeToolTip*)( m_pQuickJoinTooltip );
	if( pQuickTooltip )
		pQuickTooltip->SetModeHelpTooltip( NULL, -1 );

	for( DWORD dwID = BATTLE; dwID <= TOURNAMENT; ++dwID )
	{
		ioWnd* pWnd = FindChildWnd( dwID );
		if( pWnd )
			pWnd->HideWnd();
	}
}

void QuickLobbyWnd::SetModeHelpTooltip( ioWnd *pCallWnd, const ioHashString &szTitle, int iQuickstartModeHelpIndex )
{
	if( m_pModeHelpTooltip == NULL )
		m_pModeHelpTooltip = dynamic_cast< ModeHelpTooltip * >( FindChildWnd( ID_MODE_HELP_TOOLTIP ) );

	ModeHelpTooltip *pModeHelpTooltip = dynamic_cast< ModeHelpTooltip * >( m_pModeHelpTooltip );	
	if( pModeHelpTooltip )
		pModeHelpTooltip->SetModeHelpTooltip(  pCallWnd, szTitle, iQuickstartModeHelpIndex );
}

void QuickLobbyWnd::SetQuickJoinTooltip( ioWnd *pCallWnd, const int nMode )
{
	if( m_pQuickJoinTooltip == NULL )
		m_pQuickJoinTooltip = (CQuickModeToolTip*)( FindChildWnd( ID_QUICKJOIN_TOOLTIP ) );

	CQuickModeToolTip *pQuickTooltip = (CQuickModeToolTip*)( m_pQuickJoinTooltip );
	if( pQuickTooltip )
		pQuickTooltip->SetModeHelpTooltip( pCallWnd, nMode );
}

void QuickLobbyWnd::SetShuffleToolTip( bool bShow )
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

bool QuickLobbyWnd::iwm_esc()
{
	HideWnd();
	return true;
}

void QuickLobbyWnd::ShowMenu( DWORD dwWndID )
{
	ShowWnd();

	//iwm_create�� ��ϵ� ������ �ܿ� �ٸ� �����찡 ���Ե��������� for������ show/hide
	for( DWORD dwID = BATTLE; dwID <= TOURNAMENT; ++dwID )
	{
		ioWnd* pWnd = FindChildWnd( dwID );
		if( pWnd )
		{
			if( dwID == dwWndID )
			{
				SeTitle( dwID );
				CalcSize( dwID );
				pWnd->SetWndPos( 17, 50 );
				pWnd->ShowWnd();
				m_dwCurrMenu = dwWndID;
				ShowInfo( dwID );
			}
			else
			{
				pWnd->HideWnd();
				HideInfo( dwID );
			}
		}
	}
}

void QuickLobbyWnd::ShowInfo( DWORD dwWndID, bool bReCalcPos )
{
	HideChildWnd( TOUR_INFO );
	HideChildWnd( CAMP_INFO );
	HideChildWnd( TRADE_INFO );

	ioWnd* pWnd = NULL;
	switch( dwWndID )
	{
	case CAMP:
		{
			pWnd = FindChildWnd( CAMP_INFO );
		}
		break;
	case TOURNAMENT:
		{
			pWnd = FindChildWnd( TOUR_INFO );
		}
		break;
	case TRADE:
		{
			QuickLobbyTradeInfoWnd* pTradeWnd = (QuickLobbyTradeInfoWnd*)( FindChildWnd( TRADE_INFO ) );
			if( bReCalcPos )
			{
				pWnd = pTradeWnd;
			}
			else
			{
				//Ʈ���̵� ������� �ڽ��� ����� ��ǰ�� ���� ��쿡�� info �����츦 show
				if( pTradeWnd )
					pTradeWnd->OnRefreshTradeItemList();
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

void QuickLobbyWnd::HideInfo( DWORD dwWndID )
{
	switch( dwWndID )
	{
	case CAMP:
		{
			HideChildWnd( CAMP_INFO );
		}
		break;
	}
}

void QuickLobbyWnd::SeTitle( DWORD dwWndID )
{
	switch( dwWndID )
	{
	case BATTLE: 
		SetTitleText( STR(1) );
		break;
	case CAMP:
		SetTitleText( STR(2) );
		break;
	case PLAZA: 
		SetTitleText( STR(3) );
		break;
	case TRADE:
		SetTitleText( STR(4) );
		break;
	case TOURNAMENT:
		SetTitleText( STR(5) );
		break;
	default:
		SetTitleText( "" );
	}
}

void QuickLobbyWnd::CalcSize( DWORD dwWndID )
{
	switch( dwWndID )
	{
	case BATTLE:
	case PLAZA:
		SetSize( 437, 416 );
		break;
	default:
		SetSize( m_iDefWidth, m_iDefHeight );
		break;
	}
}

void QuickLobbyWnd::OnCreateHeadquarters()
{
	if( g_App.IsMouseBusy() )
		return;

	if( g_BattleRoomMgr.IsBattleRoom() )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return;
	}

	if( g_LadderTeamMgr.IsLadderTeam() )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return;
	}

	g_MyInfo.SetJoinHeadquartersRequestName( "" );
	SP2Packet kPacket( CTPK_JOIN_HEADQUARTERS );
	kPacket << g_MyInfo.GetPublicID() << g_ModeSelectInfo.CheckHeadquartersMapIndex( g_ModeSelectInfo.GetHeadquartersModeIndex(), 1 ) << false;
	TCPNetwork::SendToServer( kPacket );
	TCPNetwork::MouseBusy( true );
}

void QuickLobbyWnd::OnExitButtonDown( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
	HideWnd();
}

void QuickLobbyWnd::Layout( int iWidth, int iHeight )
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

void QuickLobbyWnd::ApplyPlazaList( SP2Packet &rkPacket )
{
	PlazaListWnd *pPlazaListWnd = (PlazaListWnd*)( FindChildWnd( PLAZA ) );
	if( pPlazaListWnd && pPlazaListWnd->IsShow() )
		pPlazaListWnd->SetPlazaList( rkPacket );
}

void QuickLobbyWnd::ApplyTournamentList( SP2Packet &rkPacket )
{
	TournamentMainWnd *pTournamentWnd = ( TournamentMainWnd * )( FindChildWnd( TOURNAMENT ) );
	if( pTournamentWnd && pTournamentWnd->IsShow() )
		pTournamentWnd->ApplyTournamentList( rkPacket );
}

void QuickLobbyWnd::ApplyTournamentConfirmCheck( SP2Packet &rkPacket )
{
	TournamentMainWnd *pTournamentWnd = ( TournamentMainWnd * )( FindChildWnd( TOURNAMENT ) );
	if( pTournamentWnd && pTournamentWnd->IsShow() )
		pTournamentWnd->ApplyTournamentConfirmCheck( rkPacket );
}

void QuickLobbyWnd::ApplyTournamentConfirmRequest( SP2Packet &rkPacket )
{
	TournamentMainWnd *pTournamentWnd = ( TournamentMainWnd * )( FindChildWnd( TOURNAMENT ) );
	if( pTournamentWnd && pTournamentWnd->IsShow() )
		pTournamentWnd->ApplyTournamentConfirmRequest( rkPacket );
}

void QuickLobbyWnd::ApplyNormalTeamList( SP2Packet &rkPacket )
{
	PartyCatalogueWnd *pCatalogueWnd = (PartyCatalogueWnd*)( FindChildWnd( BATTLE ) );
	if( pCatalogueWnd && pCatalogueWnd->IsShow() )
		pCatalogueWnd->SetPartyList( rkPacket );
}

void QuickLobbyWnd::RefreshBattleList( DWORD dwIndex )
{
	PartyCatalogueWnd *pCatalogueWnd = (PartyCatalogueWnd*)( FindChildWnd( BATTLE ) );
	if( pCatalogueWnd && pCatalogueWnd->IsShow() )
	{
		if( dwIndex == 0 || pCatalogueWnd->IsIndexToList( dwIndex ) )
			pCatalogueWnd->StartSend( false );
	}
}

void QuickLobbyWnd::ApplyLadderTeamList( SP2Packet &rkPacket )
{
	CampMainWnd *pCampMainWnd = dynamic_cast<CampMainWnd*>( FindChildWnd( CAMP ) );
	if( pCampMainWnd && pCampMainWnd->IsShow() )
		pCampMainWnd->SetLadderTeamList( rkPacket );
}

void QuickLobbyWnd::RefreshTradeItemList()
{
	TradeMainWnd *pTradeWnd = (TradeMainWnd*)( FindChildWnd( TRADE ) );
	if( pTradeWnd )
		pTradeWnd->OnRefreshTradeItemList();

	QuickLobbyTradeInfoWnd *pTradeInfoWnd = (QuickLobbyTradeInfoWnd*)( FindChildWnd( TRADE_INFO ) );
	if( pTradeInfoWnd )
		pTradeInfoWnd->OnRefreshTradeItemList();
}

void QuickLobbyWnd::ApplyTradeItemList( SP2Packet &rkPacket )
{
	bool bOwnerList = false;
	int iMaxSize = 0;
	rkPacket >> bOwnerList;
	rkPacket >> iMaxSize;

	TradeMainWnd *pTradeWnd = (TradeMainWnd*)( FindChildWnd( TRADE ) );
	QuickLobbyTradeInfoWnd *pTradeInfoWnd = (QuickLobbyTradeInfoWnd*)( FindChildWnd( TRADE_INFO ) );

	if( bOwnerList )
	{
		if( pTradeWnd && pTradeWnd->IsShow() && pTradeInfoWnd )
		{
			//��ϵ� �������� ������ ������ ��ġ ������
			if( 0 < iMaxSize )
				ShowInfo( TRADE, true );

			pTradeInfoWnd->ApplyTradeItemList( rkPacket, iMaxSize );
		}
	}
	else
	{
		TradeMainWnd *pTradeWnd = (TradeMainWnd*)( FindChildWnd( TRADE ) );
		if( pTradeWnd  && pTradeWnd->IsShow() )
		{
			pTradeWnd->SetTradeItemList( rkPacket, iMaxSize );
		}
	}
}

void QuickLobbyWnd::SetTournamentLink( DWORD dwTourIndex )
{
	TournamentMainWnd *pTournamentWnd = dynamic_cast< TournamentMainWnd * >( FindChildWnd( TOURNAMENT ) );
	if( pTournamentWnd )
	{
		pTournamentWnd->SetTournamentLink( dwTourIndex );
	}
}

bool QuickLobbyWnd::QuestGuide( int iModeSelectIndex, float &rfXPos, float &rfYPos )
{
	PartyCatalogueWnd *pCatalogueWnd = (PartyCatalogueWnd*)( FindChildWnd( BATTLE ) );
	if( pCatalogueWnd && pCatalogueWnd->IsShow() )
	{
		return pCatalogueWnd->QuestGuide( iModeSelectIndex, rfXPos, rfYPos );
	}

	return false;
}

void QuickLobbyWnd::RefreshCampInfo( int iPrevMyCampPos )
{
	CampMainWnd *pCampMainWnd = dynamic_cast<CampMainWnd*>( FindChildWnd( CAMP ) );
	if( pCampMainWnd )
	{
		ShowMenu( CAMP );

		// �������� ���� ���� ������ �����ش�.
		if( iPrevMyCampPos == CAMP_NONE )
			pCampMainWnd->CheckCampButton( CampMainWnd::ID_CAMP_INFO_BTN );
	}
}

//----------------------------------------------------------------------------------------------------------

QuickLobbyTradeInfoWnd::QuickLobbyTradeInfoWnd()
{
	m_pInfoWnd = NULL;
}

QuickLobbyTradeInfoWnd::~QuickLobbyTradeInfoWnd()
{
	m_pInfoWnd = NULL;
}

void QuickLobbyTradeInfoWnd::iwm_create()
{
	m_pInfoWnd = (LobbyOwnerTradeListWnd*)( g_GUIMgr.AddWnd( "XML/LobbyOwnerTradeListWnd.xml", this ) );
	if( m_pInfoWnd )
	{
		m_pInfoWnd->SetMaxTradeList( LobbyOwnerTradeListWnd::OWNER_MAX_TRADE );
		m_pInfoWnd->PlazaUIMove();
	}
}

void QuickLobbyTradeInfoWnd::iwm_hide()
{
	if( m_pInfoWnd )
		m_pInfoWnd->HideWnd();
}

void QuickLobbyTradeInfoWnd::iwm_show()
{
	if( m_pInfoWnd )
		m_pInfoWnd->ShowWnd();
}

void QuickLobbyTradeInfoWnd::OnRefreshTradeItemList()
{
	if( m_pInfoWnd )
		m_pInfoWnd->OnRefreshTradeItemList();
}

void QuickLobbyTradeInfoWnd::ApplyTradeItemList( SP2Packet &rkPacket, int iMaxSize )
{
	if( m_pInfoWnd )
		m_pInfoWnd->SetTradeItemList( rkPacket, iMaxSize );
}
