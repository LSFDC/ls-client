#include "StdAfx.h"

#include "../io3DEngine/ioUI3DEffectRender.h"

#include "../ioCampManager.h"
#include "../ioTournamentManager.h"
#include "../ioShuffleRoomMgr.h"

#include "ioInterfaceBackground.h"
#include "ChatMsgWnd.h"
#include "TournamentMainWnd.h"
#include "TournamentDetailMainWnd.h"
#include "QuickModeToolTip.h"
#include "CampMainWnd.h"
#include "PlazaMainWnd.h"
#include "TradeMainWnd.h"
#include "LobbyMainWnd.h"

#include "../ioDateHelp.h"
#include "../Version.h"
#include "../Local/ioLocalUS.h"

#include "LadderTeamWnd.h"
#include "PartyVersusWnd.h"
#include "GuildHQEtcItemNeedPopup.h"
#include "HomeEtcItemNeedPopup.h"
#include "LobbyNoviceBtn.h"

#include "../ioBonusCashManager.h"

LobbyBtn::LobbyBtn()
{
	m_pIconBack = NULL;
	m_pIcon = NULL;
	m_pStateGreenFrm = NULL;
	m_pStateRedFrm = NULL;
	m_pStateOrangeFrm = NULL;
	m_pStatePinkFrm = NULL;
}

LobbyBtn::~LobbyBtn()
{
	SAFEDELETE( m_pIconBack );
	SAFEDELETE( m_pIcon );
	SAFEDELETE( m_pStateGreenFrm );
	SAFEDELETE( m_pStateRedFrm );
	SAFEDELETE( m_pStateOrangeFrm );
	SAFEDELETE( m_pStatePinkFrm );
}

void LobbyBtn::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "IconBack" )
	{
		SAFEDELETE( m_pIconBack );		
		m_pIconBack = pImage;
	}
	else if( szType == "Icon" )
	{
		SAFEDELETE( m_pIcon );
		m_pIcon = pImage;
	}
	else 
	{
		ioRadioButton::AddRenderImage( szType, pImage );
	}
}

void LobbyBtn::AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame )
{
	if( pFrame )
	{
		if( szType == "GreenFrm" )
		{
			SAFEDELETE( m_pStateGreenFrm );
			m_pStateGreenFrm = pFrame;
		}
		else if( szType == "RedFrm" )
		{
			SAFEDELETE( m_pStateRedFrm );
			m_pStateRedFrm = pFrame;
		}
		else if( szType == "OrangeFrm" )
		{
			SAFEDELETE( m_pStateOrangeFrm );
			m_pStateOrangeFrm = pFrame;
		}
		else if( szType == "PinkFrm" )
		{
			SAFEDELETE( m_pStatePinkFrm );
			m_pStatePinkFrm = pFrame;
		}
		else
		{	
			ioRadioButton::AddRenderFrame( szType, pFrame );
		}
	}
}

void LobbyBtn::iwm_show()
{
}

void LobbyBtn::iwm_hide()
{
}

//void LobbyBtn::OnProcess( float fTimePerSec )
//{
//	if( !IsShow() ) return;
//}

void LobbyBtn::OnRender()
{
	ioRadioButton::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	if( m_bClicked )
	{
		if( m_pIconBack )
		{
			m_pIconBack->SetScale( FLOAT05, FLOAT05 );
			m_pIconBack->Render( iXPos + 35, iYPos + 35, UI_RENDER_MULTIPLY, TFO_BILINEAR );
		}

		if( m_pIcon )
		{
			m_pIcon->SetScale( FLOAT05, FLOAT05 );
			m_pIcon->Render( iXPos + 35, iYPos + 35, UI_RENDER_NORMAL, TFO_BILINEAR );
		}
	}
	else
	{
		if( m_pIconBack )
		{
			m_pIconBack->SetScale( 0.55f, 0.55f );
			m_pIconBack->Render( iXPos + 35, iYPos + 35, UI_RENDER_MULTIPLY, TFO_BILINEAR );
		}

		if( m_pIcon )
		{
			m_pIcon->SetScale( 0.55f, 0.55f );
			if( g_TournamentMgr.IsRegularDisableTournament() && GetID() == LobbyWnd::ID_TOURNAMENT_BTN )
				m_pIcon->Render( iXPos + 35, iYPos + 35, UI_RENDER_GRAY_LIGHT, TFO_BILINEAR );
			else
				m_pIcon->Render( iXPos + 35, iYPos + 35, UI_RENDER_NORMAL, TFO_BILINEAR );
		}
	}

	switch( GetID() )
	{
	case LobbyWnd::ID_BATTLE_BTN:
		{
			g_FontMgr.SetTextStyle( TS_NORMAL );
			g_FontMgr.SetAlignType( TAT_LEFT );
			g_FontMgr.SetBkColor( TCT_DEFAULT_DARKGRAY );
			g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
			g_FontMgr.PrintText( iXPos + 59, iYPos + 18, FONT_SIZE_17, STR(1) );

			g_FontMgr.SetBkColor( TCT_DEFAULT_GRAY );
			g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
			g_FontMgr.PrintText( iXPos + 59, iYPos + 39, FONT_SIZE_11, STR(2) );
		}
		break;
	case LobbyWnd::ID_CAMP_BTN:
		{
			if( g_CampMgr.GetNextTodayBattleSec() == -1 )
			{
				// �޽���
				if( m_pStateRedFrm )
					m_pStateRedFrm->Render( iXPos, iYPos );
				g_FontMgr.SetTextStyle( TS_NORMAL );
				g_FontMgr.SetAlignType( TAT_CENTER );
				g_FontMgr.SetBkColor( 255, 255, 255 );
				g_FontMgr.SetTextColor( 255, 255, 255 );
				g_FontMgr.PrintText( iXPos + 27, iYPos + 9, FONT_SIZE_11, STR(3) );
			}
			else
			{
				// ������
				if( m_pStateGreenFrm )
					m_pStateGreenFrm->Render( iXPos, iYPos );
				g_FontMgr.SetTextStyle( TS_NORMAL );
				g_FontMgr.SetAlignType( TAT_CENTER );
				g_FontMgr.SetBkColor( 255, 255, 255 );
				g_FontMgr.SetTextColor( 255, 255, 255 );
				g_FontMgr.PrintText( iXPos + 27, iYPos + 9, FONT_SIZE_11, STR(4) );
			}

			g_FontMgr.SetTextStyle( TS_NORMAL );
			g_FontMgr.SetAlignType( TAT_LEFT );
			g_FontMgr.SetBkColor( TCT_DEFAULT_DARKGRAY );
			g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
			g_FontMgr.PrintText( iXPos + 59, iYPos + 18, FONT_SIZE_17, STR(5) );

			g_FontMgr.SetBkColor( TCT_DEFAULT_GRAY );
			g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
			g_FontMgr.PrintText( iXPos + 59, iYPos + 39, FONT_SIZE_11, STR(6) );
		}
		break;
	case LobbyWnd::ID_HERO_MAIN_BTN:
		{
			if( g_CampMgr.GetNextTodayBattleSec() == -1 )
			{
				// �޽���
				if( m_pStateRedFrm )
					m_pStateRedFrm->Render( iXPos, iYPos );
				g_FontMgr.SetTextStyle( TS_NORMAL );
				g_FontMgr.SetAlignType( TAT_CENTER );
				g_FontMgr.SetBkColor( 255, 255, 255 );
				g_FontMgr.SetTextColor( 255, 255, 255 );
				g_FontMgr.PrintText( iXPos + 27, iYPos + 9, FONT_SIZE_11, STR(7) );
			}
			else
			{
				// ������
				if( m_pStateGreenFrm )
					m_pStateGreenFrm->Render( iXPos, iYPos );
				g_FontMgr.SetTextStyle( TS_NORMAL );
				g_FontMgr.SetAlignType( TAT_CENTER );
				g_FontMgr.SetBkColor( 255, 255, 255 );
				g_FontMgr.SetTextColor( 255, 255, 255 );
				g_FontMgr.PrintText( iXPos + 27, iYPos + 9, FONT_SIZE_11, STR(8) );
			}

			g_FontMgr.SetTextStyle( TS_NORMAL );
			g_FontMgr.SetAlignType( TAT_LEFT );
			g_FontMgr.SetBkColor( TCT_DEFAULT_DARKGRAY );
			g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
			g_FontMgr.PrintText( iXPos + 59, iYPos + 18, FONT_SIZE_17, STR(9) );

			g_FontMgr.SetBkColor( TCT_DEFAULT_GRAY );
			g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
			g_FontMgr.PrintText( iXPos + 59, iYPos + 39, FONT_SIZE_11, STR(10) );
		}
		break;
	case LobbyWnd::ID_PLAZA_BTN:
		{
			g_FontMgr.SetTextStyle( TS_NORMAL );
			g_FontMgr.SetAlignType( TAT_LEFT );
			g_FontMgr.SetBkColor( TCT_DEFAULT_DARKGRAY );
			g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
			g_FontMgr.PrintText( iXPos + 59, iYPos + 18, FONT_SIZE_17, STR(11) );

			g_FontMgr.SetBkColor( TCT_DEFAULT_GRAY );
			g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
			g_FontMgr.PrintText( iXPos + 59, iYPos + 39, FONT_SIZE_11, STR(12) );
		}
		break;
	case LobbyWnd::ID_TRADE_BTN:
		{
			g_FontMgr.SetTextStyle( TS_NORMAL );
			g_FontMgr.SetAlignType( TAT_LEFT );
			g_FontMgr.SetBkColor( TCT_DEFAULT_DARKGRAY );
			g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
			g_FontMgr.PrintText( iXPos + 59, iYPos + 18, FONT_SIZE_17, STR(13) );

			g_FontMgr.SetBkColor( TCT_DEFAULT_GRAY );
			g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
			g_FontMgr.PrintText( iXPos + 59, iYPos + 39, FONT_SIZE_11, STR(14) );
		}
		break;
	case LobbyWnd::ID_TOURNAMENT_BTN:
		{
			int iState = g_TournamentMgr.GetRegularState();
			if( iState != -1 )       // ���� ������ �������� �ʾ���.
			{
				g_FontMgr.SetTextStyle( TS_NORMAL );
				g_FontMgr.SetAlignType( TAT_CENTER );
				g_FontMgr.SetBkColor( 255, 255, 255 );
				g_FontMgr.SetTextColor( 255, 255, 255 );

				if( g_TournamentMgr.IsRegularDisableTournament() )
				{
					if( m_pStateGreenFrm )
						m_pStateGreenFrm->Render( iXPos, iYPos, UI_RENDER_GRAY_LIGHT );

					g_FontMgr.SetTextColor( TCT_DEFAULT_LIGHTGRAY );
					g_FontMgr.PrintText( iXPos + 27, iYPos + 9, FONT_SIZE_11, STR(20) );
				}
				else
				{
					if( iState == TournamentNode::STATE_TEAM_APP )
					{
						if( m_pStateOrangeFrm )
							m_pStateOrangeFrm->Render( iXPos, iYPos );

						g_FontMgr.PrintText( iXPos + 27, iYPos + 9, FONT_SIZE_11, STR(21) );
					}
					else if( iState == TournamentNode::STATE_TEAM_DELAY )
					{
						if( m_pStatePinkFrm )
							m_pStatePinkFrm->Render( iXPos, iYPos );

						g_FontMgr.PrintText( iXPos + 27, iYPos + 9, FONT_SIZE_11, STR(22) );

					}
					else
					{
						if( m_pStateGreenFrm )
							m_pStateGreenFrm->Render( iXPos, iYPos );

						g_FontMgr.PrintText( iXPos + 27, iYPos + 9, FONT_SIZE_11, STR(23) );
					}
				}
			}

			g_FontMgr.SetTextStyle( TS_NORMAL );
			g_FontMgr.SetAlignType( TAT_LEFT );

			if( g_TournamentMgr.IsRegularDisableTournament() )
			{
				g_FontMgr.SetBkColor( TCT_DEFAULT_LIGHTGRAY );
				g_FontMgr.SetTextColor( TCT_DEFAULT_LIGHTGRAY );
			}
			else
			{
				g_FontMgr.SetBkColor( TCT_DEFAULT_DARKGRAY );
				g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
			}

			g_FontMgr.PrintText( iXPos + 59, iYPos + 18, FONT_SIZE_17, STR(24) );

			if( g_TournamentMgr.IsRegularDisableTournament() )
			{
				g_FontMgr.SetBkColor( TCT_DEFAULT_LIGHTGRAY );
				g_FontMgr.SetTextColor( TCT_DEFAULT_LIGHTGRAY );
			}
			else
			{
				g_FontMgr.SetBkColor( TCT_DEFAULT_GRAY );
				g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
			}
			g_FontMgr.PrintText( iXPos + 59, iYPos + 39, FONT_SIZE_11, STR(25) );
		}
		break;
	}

}
//////////////////////////////////////////////////////////////////////////
IconBtn::IconBtn()
{
	m_pIconBack   = NULL;
	m_pIcon       = NULL;
}

IconBtn::~IconBtn()
{
	SAFEDELETE( m_pIconBack );
	SAFEDELETE( m_pIcon );
}

void IconBtn::OnRender()
{
	ioButton::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	// x, y offset�� xml���� ���� 
	if( m_pIconBack )
	{
		m_pIconBack->SetScale( m_fIconBackScale );		
		m_pIconBack->Render( iXPos, iYPos, UI_RENDER_MULTIPLY, TFO_BILINEAR );
	}

	if( m_pIcon )
	{
		m_pIcon->SetScale( m_fIconScale );

		if( HasWndStyle( IWS_EXACTIVE ) )
		{
			m_pIcon->Render( iXPos, iYPos, UI_RENDER_GRAY, TFO_BILINEAR );
		}
		else
		{
			m_pIcon->Render( iXPos, iYPos, UI_RENDER_NORMAL, TFO_BILINEAR );
		}
	}
}

void IconBtn::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "IconBack" )
	{
		SAFEDELETE( m_pIconBack );
		m_pIconBack = pImage;
	}
	else if( szType == "Icon" )
	{
		SAFEDELETE( m_pIcon );
		m_pIcon = pImage;
	}
	else
	{
		ioButton::AddRenderImage( szType, pImage );
	}
}

void IconBtn::ParseExtraInfo( ioXMLElement &xElement )
{
	m_fIconScale     = xElement.GetFloatAttribute_e( "IconScale" );
	m_fIconBackScale = xElement.GetFloatAttribute_e( "IconBackScale" );
}
//////////////////////////////////////////////////////////////////////////
LobbyWnd::LobbyWnd()
{
	m_pCustomTitle = NULL;
	m_pTabBack1	= NULL;
	m_pTabBack2	= NULL;
	m_pTabBack3	= NULL;
	m_pTabBack4 = NULL;

	m_dwTabID = ID_BATTLE_BTN;
	m_iMyCampPos = 0;

	m_pMajorMark  = NULL;
	m_pMinorGrade = NULL;
	m_pGaugeBack  = NULL;
	m_pGauge      = NULL;
	m_iPrevGradeLevel = -1;

	m_bMileage = false;
}

LobbyWnd::~LobbyWnd()
{
	SAFEDELETE( m_pCustomTitle );
	SAFEDELETE( m_pTabBack1 );
	SAFEDELETE( m_pTabBack2 );
	SAFEDELETE( m_pTabBack3 );
	SAFEDELETE( m_pTabBack4 );
	SAFEDELETE( m_pMajorMark );
	SAFEDELETE( m_pMinorGrade );
	SAFEDELETE( m_pGaugeBack );
	SAFEDELETE( m_pGauge );
}

void LobbyWnd::AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame )
{
	if( szType == "CustomTitle" )
	{
		SAFEDELETE( m_pCustomTitle );
		m_pCustomTitle = pFrame;
	}
	else
	{
		ioWnd::AddRenderFrame( szType, pFrame );
	}
}

void LobbyWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "TabBack1" )
	{
		SAFEDELETE( m_pTabBack1 );
		m_pTabBack1 = pImage;
	}
	else if( szType == "TabBack2" )
	{
		SAFEDELETE( m_pTabBack2 );
		m_pTabBack2 = pImage;
	}
	else if( szType == "TabBack3" )
	{
		SAFEDELETE( m_pTabBack3 );
		m_pTabBack3 = pImage;
	}
	else if( szType == "TabBack4" )
	{
		SAFEDELETE( m_pTabBack4 );
		m_pTabBack4 = pImage;
	}
	else if( szType == "MinorGrade" )
	{
		SAFEDELETE( m_pMinorGrade );
		m_pMinorGrade = pImage;
	}
	else if( szType == "GaugeBack" )
	{
		SAFEDELETE( m_pGaugeBack );
		m_pGaugeBack = pImage;
	}
	else if( szType == "Gauge" )
	{
		SAFEDELETE( m_pGauge );
		m_pGauge = pImage;
	}
	else
	{
		ioWnd::AddRenderImage( szType, pImage );
	}
}

void LobbyWnd::iwm_create()
{
	g_GUIMgr.AddWnd( "XML/partycataloguewnd.xml", this );
	g_GUIMgr.AddWnd( "XML/campmainwnd.xml", this );
	g_GUIMgr.AddWnd( "XML/plazalistwnd.xml", this );

	g_GUIMgr.AddWnd( "XML/heromainwnd.xml", this );
	g_GUIMgr.AddWnd( "XML/trademainwnd.xml", this );
	g_GUIMgr.AddWnd( "XML/tournamentmainwnd.xml", this );

	ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
	if( !pLocal )
		return;
	m_bMileage = pLocal->IsMileage();
}

void LobbyWnd::iwm_show()
{
	m_iMyCampPos = g_MyInfo.GetUserCampPos();
	ChangeRadioWnd( ID_BATTLE_BTN );

	UpdateGuildHQButton();
	UpdateHomeButton();
}

void LobbyWnd::iwm_hide()
{

}

void LobbyWnd::UpdateGuildHQButton()
{
	SetChildWndStyleAdd( ID_GUILD_HEADQUATER_BTN, IWS_EXACTIVE );

	ioGuildData *pGuildData = g_GuildInfo.GetGuildData( g_GuildInfo.GetGuildIndex() );
	if( pGuildData && pGuildData->GetGuildRoomOpen() )
	{
		SetChildWndStyleRemove( ID_GUILD_HEADQUATER_BTN, IWS_EXACTIVE );
	}
}

void LobbyWnd::UpdateHomeButton()
{
	SetChildWndStyleAdd( ID_MY_HOME_BTN, IWS_EXACTIVE );

	if( g_MyInfo.IsHouseOpen() )
		SetChildWndStyleRemove( ID_MY_HOME_BTN, IWS_EXACTIVE );
}

void LobbyWnd::ChangeRadioWnd( DWORD dwID )
{
	m_dwTabID = dwID;
	CheckRadioButton( ID_BATTLE_BTN, ID_TOURNAMENT_WND, dwID );

	HideChildWnd( ID_BATTLE_WND );
	HideChildWnd( ID_CAMP_MAIN_WND );
	HideChildWnd( ID_PLAZA_WND );
	HideChildWnd( ID_HERO_MAIN_WND );
	HideChildWnd( ID_TRADE_MAIN_WND );
	HideChildWnd( ID_TOURNAMENT_WND );
	
	int iPrevMyCampPos = m_iMyCampPos;
	m_iMyCampPos = g_MyInfo.GetUserCampPos();
	switch( m_dwTabID )
	{
	case ID_BATTLE_BTN:
		ShowChildWnd( ID_BATTLE_WND );
		break;
	case ID_CAMP_BTN:
		{
			switch( m_iMyCampPos )
			{
			case CAMP_NONE:
				ShowChildWnd( ID_CAMP_MAIN_WND );
				break;
			default:
				{
					CampMainWnd *pCampMainWnd = dynamic_cast<CampMainWnd*>(FindChildWnd( ID_CAMP_MAIN_WND ));
					if( pCampMainWnd )
					{
						pCampMainWnd->ShowWnd();
						// �������� ���� ���� ������ �����ش�.
						if( iPrevMyCampPos == CAMP_NONE )     
							pCampMainWnd->CheckCampButton( CampMainWnd::ID_CAMP_INFO_BTN );
					}
				}
				break;
			}
		}
		break;
	case ID_HERO_MAIN_BTN:
		ShowChildWnd( ID_HERO_MAIN_WND );
		break;
	case ID_PLAZA_BTN:
		ShowChildWnd( ID_PLAZA_WND );
		break;
	case ID_TRADE_BTN:
		{
			TradeMainWnd *pTradeWnd = dynamic_cast<TradeMainWnd*>(FindChildWnd( ID_TRADE_MAIN_WND ));
			if( pTradeWnd && !pTradeWnd->IsShow() )
				ShowChildWnd( ID_TRADE_MAIN_WND );
		}
		break;
	case ID_TOURNAMENT_BTN:
		ShowChildWnd( ID_TOURNAMENT_WND );
		break;
	}

	if( GetParent() )
		GetParent()->iwm_command( this, IOBN_BTNDOWN, m_dwTabID );
}

void LobbyWnd::RefreshTab()
{
	ChangeRadioWnd( m_dwTabID ); 
}

void LobbyWnd::OnCreateHeadquarters()
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
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return;
	}

	g_MyInfo.SetJoinHeadquartersRequestName( "" );
	SP2Packet kPacket( CTPK_JOIN_HEADQUARTERS );
	kPacket << g_MyInfo.GetPublicID() << g_ModeSelectInfo.CheckHeadquartersMapIndex( g_ModeSelectInfo.GetHeadquartersModeIndex(), 1 ) << false;
	TCPNetwork::SendToServer( kPacket );
	TCPNetwork::MouseBusy( true );
}

void LobbyWnd::OnCreateGuildHeadquarters()
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

void LobbyWnd::OnCreateHouse()
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

	g_MyInfo.SetJoinHouseRequestName( "" );
	SP2Packet kPacket( CTPK_JOIN_PERSONAL_HQ );
	kPacket << g_MyInfo.GetPublicID() << g_ModeSelectInfo.CheckHouseMapIndex( g_ModeSelectInfo.GetHouseModeIndex(), 1 ) << false;
	TCPNetwork::SendToServer( kPacket );
	TCPNetwork::MouseBusy( true );
}

void LobbyWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_BATTLE_BTN:
	case ID_CAMP_BTN:
	case ID_PLAZA_BTN:
	case ID_HERO_MAIN_BTN:
	case ID_TRADE_BTN:
	case ID_TOURNAMENT_BTN:
		if( cmd == IOBN_BTNDOWN )
		{
			ChangeRadioWnd( dwID );
		}
		else if( cmd == IOEX_BTNDOWN )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, g_TournamentMgr.GetRegularDisableTournamentMent().c_str() );
			ChangeRadioWnd( m_dwTabID );
		}
		break;
	case ID_HEADQUARTER_BTN:
		if( cmd == IOBN_BTNUP )
		{
			OnCreateHeadquarters();
		}
		break;
	case ID_GUILD_HEADQUATER_BTN:
		if( cmd == IOBN_BTNUP )
		{
			OnCreateGuildHeadquarters();
		}
		else if( cmd == IOEX_BTNUP )
		{
			ioGuildData *pGuildData = g_GuildInfo.GetGuildData( g_GuildInfo.GetGuildIndex() );
			if( pGuildData )
			{
				if( pGuildData->GetGuildMasterName() == g_MyInfo.GetPublicID() )
				{
					GuildHQEtcItemNeedPopup* pWnd = dynamic_cast<GuildHQEtcItemNeedPopup*>( g_GUIMgr.FindWnd( GUILD_HQ_NEED_POPUP ) );
					if( pWnd )
					{
						pWnd->SetType( GuildHQEtcItemNeedPopup::PUT_NEED_HQ );
						ioUserEtcItem* pInven = g_MyInfo.GetUserEtcItem();
						if( pInven )
						{
							ioUserEtcItem::ETCITEMSLOT Slot;
							if( pInven->GetEtcItem( ioEtcItem::EIT_ETC_CREATE_GUILD_HQ, Slot ) && Slot.m_iValue1 > 0 )
							{
								pWnd->SetType( GuildHQEtcItemNeedPopup::PUT_USE_HQ );
							}
						}
						pWnd->ShowWnd();
					}
				}
				else
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
			}
			else
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
			}
		}
		break;
	case ID_MY_HOME_BTN:
		if( cmd == IOBN_BTNUP )
		{
			OnCreateHouse();
		}
		else if( cmd == IOEX_BTNUP )
		{
			HomeEtcItemNeedPopup* pWnd = dynamic_cast<HomeEtcItemNeedPopup*>( g_GUIMgr.FindWnd( HOUSE_NEED_POPUP ) );
			if( pWnd )
				pWnd->ShowWnd();
		}
		break;
	case ID_BONUS_CASH_BTN:
		if( cmd == IOBN_BTNUP )
		{
			if ( !g_GUIMgr.IsShow( BONUS_CASH_WND ))
				g_GUIMgr.ShowWnd( BONUS_CASH_WND );
			else
				g_GUIMgr.HideWnd( BONUS_CASH_WND );
		}
		break;
	}
}

void LobbyWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() )	return;	

	switch( m_dwTabID )
	{
	case ID_CAMP_BTN:
		{
			if( m_iMyCampPos != g_MyInfo.GetUserCampPos() )
			{
				ChangeRadioWnd( ID_CAMP_BTN );
			}
		}
		break;
	case ID_HERO_MAIN_BTN:
		{
			if( m_iMyCampPos != g_MyInfo.GetUserCampPos() )
			{
				ChangeRadioWnd( ID_HERO_MAIN_BTN );
			}
		}
		break;
	}

	SetChildWndStyleRemove( ID_TOURNAMENT_BTN, IWS_EXACTIVE );	
	if( g_TournamentMgr.IsRegularDisableTournament() )
	{
		SetChildWndStyleAdd( ID_TOURNAMENT_BTN, IWS_EXACTIVE );
		ioWnd* pTournamentWnd = FindChildWnd( ID_TOURNAMENT_WND );
		if( pTournamentWnd && pTournamentWnd->IsShow() )
			ChangeRadioWnd( ID_BATTLE_BTN );
	}

	UpdateGuildHQButton();
	UpdateHomeButton();
	CheckBonusCash();

	// �ɽ� ��û ��Ŷ�� ���� ƽ
	ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
	if( pLocal )
		pLocal->ProcessGetCash();
}

void LobbyWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	if( m_pTabBack1 )
		m_pTabBack1->Render( iXPos + 2, iYPos + 2, UI_RENDER_MULTIPLY );
	if( m_pTabBack2 )
	{
		for(int i = 0;i < 5;i++)
			m_pTabBack2->Render( iXPos + 2, iYPos + 49 + i * 71, UI_RENDER_MULTIPLY );
	}
	if( m_pTabBack3 )
	{
		m_pTabBack3->SetHorzFormatting( ioUIRenderImage::HF_STRETCH );
		m_pTabBack3->SetVertFormatting( ioUIRenderImage::VF_STRETCH );
		m_pTabBack3->SetSize( 127, 113 );
		m_pTabBack3->Render( iXPos + 2, iYPos + 404, UI_RENDER_MULTIPLY );
	}
	if( m_pTabBack4 )
		m_pTabBack4->Render( iXPos + 2, iYPos + 517, UI_RENDER_MULTIPLY );

	if( m_pCustomTitle )
		m_pCustomTitle->Render( iXPos, iYPos );

	g_FontMgr.SetAlignType( TAT_LEFT );
	g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
	g_FontMgr.SetBkColor( 12, 66, 111 );
	g_FontMgr.SetTextColor( TCT_DEFAULT_WHITE );
	g_FontMgr.PrintText( iXPos + 13, iYPos + 11, FONT_SIZE_17, STR(1) );

	RenderGradeExp( iXPos, iYPos );

	// peso and gold
	enum 
	{
		X_OFFSET					= 548,
		X_OFFSET2					= 532,
		Y_OFFSET					= 15,
		X_BONUS_CASH_TOOLTIP_OFFSET	= 63,
		Y_BONUS_CASH_TOOLTIP_OFFSET	= 22,
	};

	char szPeso[MAX_PATH]="";
	char szCash[MAX_PATH]="";
	char szStamp[MAX_PATH]="";
	char szClover[MAX_PATH]="";
	char szMileage[MAX_PATH]="";
	int  iMaxStamp = g_MyInfo.GetEtcCoinMoney( ioEtcItem::EIT_ETC_SILVER_COIN );
	int  iMaxClover = g_MyInfo.GetEtcCoinMoney( ioEtcItem::EIT_ETC_CLOVER );
	int  iMaxMileage = g_MyInfo.GetEtcCoinMoney( ioEtcItem::EIT_ETC_MILEAGE_COIN );

	Help::ConvertNumToStrComma( g_MyInfo.GetMoney(), szPeso, sizeof( szPeso ) );
	Help::ConvertNumToStrComma( g_MyInfo.GetTotalCash(), szCash, sizeof( szCash ) );
	Help::ConvertNumToStrComma( iMaxStamp, szStamp, sizeof( szStamp ) );
	Help::ConvertNumToStrComma( iMaxClover, szClover, sizeof( szClover ) );

	int iAdd_OffsetY = 0;
	if( iMaxMileage > 0 )
	{
		Help::ConvertNumToStrComma( iMaxMileage, szMileage, sizeof( szMileage ) );
		iAdd_OffsetY = 7;
	}

	ioComplexStringPrinter kPrinter;
	kPrinter.SetTextStyle( TS_OUTLINE_FULL_2X );
	kPrinter.SetBkColor( 12, 66, 111 );
	if( iMaxStamp > 0 )
	{
		kPrinter.SetTextColor( 130, 198, 255 );	
		ioEtcItem *pEtcItem = g_EtcItemMgr.FindEtcItem( ioEtcItem::EIT_ETC_SILVER_COIN );
		if( pEtcItem )
		{
			kPrinter.AddTextPiece( FONT_SIZE_11, STR(5), pEtcItem->GetName().c_str(), szStamp );
		}
		else
		{
			kPrinter.AddTextPiece( FONT_SIZE_11, STR(4), szStamp );
		}
		kPrinter.SetTextColor( TCT_DEFAULT_LIGHTGRAY );		
		kPrinter.AddTextPiece( FONT_SIZE_11, " / " );
	}
	
	if( iMaxClover > 0 )
	{
		kPrinter.SetTextColor( 130, 198, 255 );	
		ioEtcItem *pEtcItem = g_EtcItemMgr.FindEtcItem( ioEtcItem::EIT_ETC_CLOVER );
		if( pEtcItem )
			kPrinter.AddTextPiece( FONT_SIZE_12, STR(6), pEtcItem->GetName().c_str(), szClover );
		else
			kPrinter.AddTextPiece( FONT_SIZE_12, STR(7), szClover );
		kPrinter.SetTextColor( TCT_DEFAULT_LIGHTGRAY );		
		kPrinter.AddTextPiece( FONT_SIZE_11, " / " );
	}
	kPrinter.SetTextColor( 130, 198, 255 );		
	kPrinter.AddTextPiece( FONT_SIZE_11, STR(2) );
	kPrinter.AddTextPiece( FONT_SIZE_11, szPeso );
	kPrinter.SetTextColor( TCT_DEFAULT_LIGHTGRAY );		
	kPrinter.AddTextPiece( FONT_SIZE_11, " / " );
	kPrinter.SetTextColor( 255, 188, 26 );		
	kPrinter.AddTextPiece( FONT_SIZE_12, STR(3) );
	kPrinter.AddTextPiece( FONT_SIZE_12, szCash );
	
	ioButton* pBonusCashBtn = dynamic_cast<ioButton*>(FindChildWnd( ID_BONUS_CASH_BTN ));
	if ( pBonusCashBtn && pBonusCashBtn->IsShow() )
	{
		//���ϸ����� �����Ѵٸ�
		if ( iMaxMileage <= 0 )
		{
			pBonusCashBtn->SetWndPos( pBonusCashBtn->GetXPos() - 2 , pBonusCashBtn->GetYPos() + 6 );
			pBonusCashBtn->RenderWnd();
			pBonusCashBtn->SetWndPos( pBonusCashBtn->GetXPos() + 2 , pBonusCashBtn->GetYPos() - 6 );
		}
		else
			pBonusCashBtn->RenderWnd();
		kPrinter.PrintFullText( iXPos + X_OFFSET2, iYPos + Y_OFFSET-iAdd_OffsetY, TAT_RIGHT );
	}
	else
		kPrinter.PrintFullText( iXPos + X_OFFSET, iYPos + Y_OFFSET-iAdd_OffsetY, TAT_RIGHT );
	
	kPrinter.ClearList();

	if( iMaxMileage > 0 )
	{
		iAdd_OffsetY = 6;
		kPrinter.SetTextColor( 115, 204, 0 );
		ioEtcItem *pEtcItem = g_EtcItemMgr.FindEtcItem( ioEtcItem::EIT_ETC_MILEAGE_COIN );
		if( pEtcItem )
		{
			kPrinter.AddTextPiece( FONT_SIZE_12, STR(5), pEtcItem->GetName().c_str(), szMileage );
		}
		else
		{
			kPrinter.AddTextPiece( FONT_SIZE_12, STR(4), szMileage );
		}
		kPrinter.SetTextColor( TCT_DEFAULT_LIGHTGRAY );		
	}

	kPrinter.PrintFullText( iXPos + X_OFFSET, iYPos + Y_OFFSET+iAdd_OffsetY, TAT_RIGHT );
	kPrinter.ClearList();


#ifdef SHIPPING
	if( g_App.IsMeDeveloper() || g_App.IsMeGameMaster() )
#endif 
	{

		int iLicentDate = 0;
		ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
		if( pLocal )
		{
			iLicentDate = pLocal->GetLicenseDate();
		}

		g_FontMgr.SetAlignType( TAT_RIGHT );
		g_FontMgr.SetTextStyle( TS_NORMAL );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextColor( 0, 0, 0 );
		char szText[MAX_PATH]="";
		StringCbPrintf( szText, sizeof( szText ), "Version : %d | %s | %d", FILEVER, STRINTERNALNAME, iLicentDate); //Except Extracting Hangeul
		g_FontMgr.PrintText( iXPos + GetWidth()-5, iYPos + GetHeight()-18, FONT_SIZE_12, szText  ); 
	}

	if ( m_pParent )
	{
		ioWnd* pBonusCashToolTip = m_pParent->FindChildWnd( LobbyMainWnd::ID_BONUS_CASH_TOOLTIP );
		if ( pBonusCashToolTip && pBonusCashToolTip->IsShow() )
			pBonusCashToolTip->SetWndPos( X_OFFSET2 - X_BONUS_CASH_TOOLTIP_OFFSET, Y_OFFSET-iAdd_OffsetY - Y_BONUS_CASH_TOOLTIP_OFFSET );
	}
}

void LobbyWnd::ApplyNormalTeamList( SP2Packet &rkPacket )
{
	if( m_dwTabID != ID_BATTLE_BTN ) return;

	PartyCatalogueWnd *pCatalogueWnd = dynamic_cast<PartyCatalogueWnd*>(FindChildWnd( ID_BATTLE_WND ));
	if( !pCatalogueWnd || !pCatalogueWnd->IsShow() )
		return;

	pCatalogueWnd->SetPartyList( rkPacket );
}

void LobbyWnd::ApplyPlazaList( SP2Packet &rkPacket )
{
	if( m_dwTabID != ID_PLAZA_BTN ) return;

	PlazaListWnd *pPlazaListWnd = dynamic_cast<PlazaListWnd*>(FindChildWnd( ID_PLAZA_WND ));
	if( !pPlazaListWnd || !pPlazaListWnd->IsShow() )
		return;

	pPlazaListWnd->SetPlazaList( rkPacket );
}

void LobbyWnd::ApplyLadderTeamList( SP2Packet &rkPacket )
{
	if( m_dwTabID != ID_CAMP_BTN ) return;

	CampMainWnd *pCampMainWnd = dynamic_cast<CampMainWnd*>(FindChildWnd( ID_CAMP_MAIN_WND ));
	if( !pCampMainWnd || !pCampMainWnd->IsShow() )
		return;

	pCampMainWnd->SetLadderTeamList( rkPacket );
}

void LobbyWnd::RefreshBattleList( DWORD dwIndex )
{
	PartyCatalogueWnd *pCatalogueWnd = dynamic_cast<PartyCatalogueWnd*>(FindChildWnd( ID_BATTLE_WND ));
	if( pCatalogueWnd && pCatalogueWnd->IsShow() )
	{
		if( dwIndex == 0 || pCatalogueWnd->IsIndexToList( dwIndex ) )
			pCatalogueWnd->StartSend( false );
	}
}

void LobbyWnd::SetCampLink()
{
	ChangeRadioWnd( ID_CAMP_BTN );
}

void LobbyWnd::SetTournamentLink( DWORD dwTourIndex )
{
	TournamentMainWnd *pTournamentWnd = dynamic_cast< TournamentMainWnd * >( FindChildWnd( ID_TOURNAMENT_WND ) );
	if( pTournamentWnd )
	{
		pTournamentWnd->SetTournamentLink( dwTourIndex );
	}

	ChangeRadioWnd( ID_TOURNAMENT_BTN );
}

bool LobbyWnd::QuestGuide( int iModeSelectIndex, float &rfXPos, float &rfYPos )
{
	PartyCatalogueWnd *pCatalogueWnd = dynamic_cast<PartyCatalogueWnd*>( FindChildWnd( ID_BATTLE_WND ) );
	if( pCatalogueWnd && pCatalogueWnd->IsShow() )
	{
		return pCatalogueWnd->QuestGuide( iModeSelectIndex, rfXPos, rfYPos );
	}
	return false;
}

void LobbyWnd::RenderGradeExp( int iXPos, int iYPos )
{
	if( !m_pMinorGrade )
		return;
	if( !m_pGaugeBack )
		return;
	if( !m_pGauge )
		return;

	enum 
	{
		MAJOR_X_OFFSET = 66,
		MAJOR_Y_OFFSET = 365,
		MINOR_Y_OFFSET = 394,

		TEXT_X_OFFSET  = 65,
		TEXT_Y_OFFSET  = 410,

		GAUGE_X_OFFSET = 17,
		GAUGE_Y_OFFSET = 427,
		GAUGE_BAR_OFFSET = 2,
	};


	// mark
	int iGradeLevel = g_MyInfo.GetGradeLevel();
	/*
	if( iGradeLevel != m_iPrevGradeLevel )
	{
		m_iPrevGradeLevel = iGradeLevel;
		SAFEDELETE( m_pMajorMark );
		m_pMajorMark = g_LevelMgr.GetGradeBigIcon( iGradeLevel);
	}
	if( m_pMajorMark )
	{
		m_pMajorMark->SetScale( FLOAT08 );
		m_pMajorMark->Render( iXPos + MAJOR_X_OFFSET, iYPos + MAJOR_Y_OFFSET, UI_RENDER_NORMAL, TFO_BILINEAR );
	}

	m_pMinorGrade->SetScale( FLOAT08 );
	// �ϻ� ~ ���
	if( COMPARE( iGradeLevel, 5, 49 ) )
	{
		int iMinorGrade = iGradeLevel % 5;
		if( iMinorGrade == 0 )
			m_pMinorGrade->Render( iXPos + 63, iYPos + MINOR_Y_OFFSET, UI_RENDER_NORMAL, TFO_BILINEAR );
		else if( iMinorGrade == 1 )
		{
			m_pMinorGrade->Render( iXPos + 59, iYPos + MINOR_Y_OFFSET, UI_RENDER_NORMAL, TFO_BILINEAR );
			m_pMinorGrade->Render( iXPos + 67, iYPos + MINOR_Y_OFFSET, UI_RENDER_NORMAL, TFO_BILINEAR );
		}
		else if( iMinorGrade == 2 )
		{	
			m_pMinorGrade->Render( iXPos + 55, iYPos + MINOR_Y_OFFSET, UI_RENDER_NORMAL, TFO_BILINEAR );
			m_pMinorGrade->Render( iXPos + 63, iYPos + MINOR_Y_OFFSET, UI_RENDER_NORMAL, TFO_BILINEAR );
			m_pMinorGrade->Render( iXPos + 71, iYPos + MINOR_Y_OFFSET, UI_RENDER_NORMAL, TFO_BILINEAR );
		}
		else if( iMinorGrade == 3 )
		{
			m_pMinorGrade->Render( iXPos + 51, iYPos + MINOR_Y_OFFSET, UI_RENDER_NORMAL, TFO_BILINEAR );
			m_pMinorGrade->Render( iXPos + 59, iYPos + MINOR_Y_OFFSET, UI_RENDER_NORMAL, TFO_BILINEAR );
			m_pMinorGrade->Render( iXPos + 67, iYPos + MINOR_Y_OFFSET, UI_RENDER_NORMAL, TFO_BILINEAR );
			m_pMinorGrade->Render( iXPos + 75, iYPos + MINOR_Y_OFFSET, UI_RENDER_NORMAL, TFO_BILINEAR );
		}
		else if( iMinorGrade == 4 )
		{
			m_pMinorGrade->Render( iXPos + 47, iYPos + MINOR_Y_OFFSET, UI_RENDER_NORMAL, TFO_BILINEAR );
			m_pMinorGrade->Render( iXPos + 55, iYPos + MINOR_Y_OFFSET, UI_RENDER_NORMAL, TFO_BILINEAR );
			m_pMinorGrade->Render( iXPos + 63, iYPos + MINOR_Y_OFFSET, UI_RENDER_NORMAL, TFO_BILINEAR );
			m_pMinorGrade->Render( iXPos + 71, iYPos + MINOR_Y_OFFSET, UI_RENDER_NORMAL, TFO_BILINEAR );
			m_pMinorGrade->Render( iXPos + 79, iYPos + MINOR_Y_OFFSET, UI_RENDER_NORMAL, TFO_BILINEAR );
		}
	}
	*/

	// text
	char szGradeName[MAX_PATH] = "";
	g_LevelMgr.GetGradeName( iGradeLevel, szGradeName, sizeof( szGradeName ), false, true );
	int iGradeExpRate = g_MyInfo.GetGradeExpRate();

	ioComplexStringPrinter kPrinter;
	kPrinter.SetTextStyle( TS_NORMAL );
	kPrinter.SetBkColor( 0, 0, 0 );
	kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );		
	kPrinter.AddTextPiece( FONT_SIZE_12, szGradeName );
	kPrinter.SetTextColor( TCT_DEFAULT_GRAY );		

	int iGaugeWidth = 0;
	if( g_MyInfo.IsGeneralGrade() )
	{
		kPrinter.AddTextPiece( FONT_SIZE_12, STR(2), g_MyInfo.GetUserRanking() );
		iGaugeWidth = m_pGauge->GetWidth();
	}
	else
	{
		kPrinter.AddTextPiece( FONT_SIZE_12, STR(1), iGradeExpRate );
		iGaugeWidth = m_pGauge->GetWidth() * (float)min( FLOAT1, ( (float)iGradeExpRate / FLOAT100 ) );
	}
	kPrinter.PrintFullText( iXPos + TEXT_X_OFFSET, iYPos + TEXT_Y_OFFSET, TAT_CENTER );
	kPrinter.ClearList();

	// gauge
	m_pGaugeBack->Render( iXPos + GAUGE_X_OFFSET, iYPos + GAUGE_Y_OFFSET );
	m_pGauge->RenderWidthCut( iXPos + GAUGE_X_OFFSET + GAUGE_BAR_OFFSET, iYPos + GAUGE_Y_OFFSET + GAUGE_BAR_OFFSET, 0, iGaugeWidth );
}

void LobbyWnd::ApplyTournamentList( SP2Packet &rkPacket )
{
	if( m_dwTabID != ID_TOURNAMENT_BTN ) return;

	TournamentMainWnd *pTournamentWnd = dynamic_cast< TournamentMainWnd * >( FindChildWnd( ID_TOURNAMENT_WND ) );
	if( pTournamentWnd && pTournamentWnd->IsShow() )
	{
		pTournamentWnd->ApplyTournamentList( rkPacket );
	}
}

void LobbyWnd::ApplyTournamentConfirmCheck( SP2Packet &rkPacket )
{
	if( m_dwTabID != ID_TOURNAMENT_BTN ) return;

	TournamentMainWnd *pTournamentWnd = dynamic_cast< TournamentMainWnd * >( FindChildWnd( ID_TOURNAMENT_WND ) );
	if( pTournamentWnd && pTournamentWnd->IsShow() )
	{
		pTournamentWnd->ApplyTournamentConfirmCheck( rkPacket );
	}
}

void LobbyWnd::ApplyTournamentConfirmRequest( SP2Packet &rkPacket )
{
	if( m_dwTabID != ID_TOURNAMENT_BTN ) return;

	TournamentMainWnd *pTournamentWnd = dynamic_cast< TournamentMainWnd * >( FindChildWnd( ID_TOURNAMENT_WND ) );
	if( pTournamentWnd && pTournamentWnd->IsShow() )
	{
		pTournamentWnd->ApplyTournamentConfirmRequest( rkPacket );
	}
}

void LobbyWnd::OnRefreshTradeItemList()
{
	TradeMainWnd *pTradeWnd = dynamic_cast<TradeMainWnd*>(FindChildWnd( ID_TRADE_MAIN_WND ));
	if( !pTradeWnd || !pTradeWnd->IsShow() )
		return;

	pTradeWnd->OnRefreshTradeItemList();
}

void LobbyWnd::CheckBonusCash()
{
	ioButton* pBonusCashBtn = dynamic_cast<ioButton*>(FindChildWnd( ID_BONUS_CASH_BTN ));
	if ( !pBonusCashBtn )
		return;

	int iBonusCash = g_MyInfo.GetBonusCash();
	if ( iBonusCash <=0 )
	{
		pBonusCashBtn->HideWnd();
		return;
	}
	else
	{
		pBonusCashBtn->ShowWnd();
	}

	if ( pBonusCashBtn->IsShow() && pBonusCashBtn->IsOver() )
	{
		ioLSMouse *pMouse = g_App.GetMouse();
		if ( pMouse )
			pMouse->SetToolTipCursor();
	}
}

void LobbyWnd::ApplyLadderList( SP2Packet &rkPacket )
{
	if( m_dwTabID != ID_BATTLE_BTN ) return;

	PartyCatalogueWnd *pCatalogueWnd = dynamic_cast<PartyCatalogueWnd*>(FindChildWnd( ID_BATTLE_WND ));
	if( !pCatalogueWnd || !pCatalogueWnd->IsShow() )
		return;

	pCatalogueWnd->SetLadderList(rkPacket);
}

//////////////////////////////////////////////////////////////////////////
LobbyChatWnd::LobbyChatWnd()
{
}

LobbyChatWnd::~LobbyChatWnd()
{
}

void LobbyChatWnd::SetPlayStage( ioPlayStage *pPlayStage )
{
	GameChatInput *pGameChatInput = dynamic_cast<GameChatInput*>(FindChildWnd( ID_CHAT_INPUT_WND ));
	if( pGameChatInput )
	{
		pGameChatInput->SetPlayStage( pPlayStage );
		pGameChatInput->SetChatTypeToBtn( CT_SERVER_LOBBY );
	}
}

void LobbyChatWnd::iwm_create()
{
}

void LobbyChatWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	m_szLoadInfo = xElement.GetStringAttribute_e( "LoadInfo" );		
}

void LobbyChatWnd::iwm_show()
{
	g_GUIMgr.HideWnd( GLOBAL_CHAT_WND );

	g_ChatMgr.LoadChatInfo( m_szLoadInfo );

	ShowChildWnd( ID_CHAT_MSG_WND );
	ShowChildWnd( ID_CHAT_INPUT_WND );

	GameChatInput *pGameChatInput = dynamic_cast<GameChatInput*>(FindChildWnd( ID_CHAT_INPUT_WND ));
	if( pGameChatInput )
	{
		pGameChatInput->SetFirstChatTypeToBtn( CT_SERVER_LOBBY );
	}

	g_ChatMgr.SetMsgWndFeature( 0 );
}

void LobbyChatWnd::iwm_hide()
{
	HideChildWnd( ID_CHAT_MSG_WND );
	HideChildWnd( ID_CHAT_INPUT_WND );
}

bool LobbyChatWnd::iwm_esc()
{
	GameChatInput *pGameChatInput = dynamic_cast<GameChatInput*>(FindChildWnd( ID_CHAT_INPUT_WND ));
	if( pGameChatInput )
		return pGameChatInput->iwm_esc();
	return false;
}

void LobbyChatWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_CHAT_INPUT_WND:
		if( cmd == IOBN_BTNUP )
		{
			ChatMsgWnd *pChatWnd = dynamic_cast<ChatMsgWnd*>( FindChildWnd( ID_CHAT_MSG_WND ) );
			if( pChatWnd )
			{
				pChatWnd->InitScrollPos();
			}
		}
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
LobbyMainWnd::LobbyMainWnd()
{
	m_pPlayStage  = NULL;
	m_iDefaultX   = 0;
	m_iDefaultY   = 0;
	m_pModeHelpTooltip  = NULL;
	m_pQuickJoinTooltip = NULL;
	m_pNoviceBtn = NULL;
	m_pEventEff = NULL;

	m_dwBonusCashTooltipCheckStartTime = 0;
}

LobbyMainWnd::~LobbyMainWnd()
{
	g_SoundMgr.StopStream();

	if( !COMPARE( m_iDefaultX, 0, Setting::Width() ) || 
		!COMPARE( m_iDefaultY, 0, Setting::Height() ) )
		return;

	g_GUIMgr.GUIPosSaveInt( "LobbyMainWnd", "XPos", m_iDefaultX ,POS_SAVE_VERSION );
	g_GUIMgr.GUIPosSaveInt( "LobbyMainWnd", "YPos", m_iDefaultY ,POS_SAVE_VERSION );
}

void LobbyMainWnd::SetPlayStage( ioPlayStage *pPlayStage )
{
	m_pPlayStage = pPlayStage;

	LobbyChatWnd *pChatWnd = dynamic_cast<LobbyChatWnd*>(FindChildWnd(ID_CHAT_WND));
	if( pChatWnd )
	{
		pChatWnd->SetPlayStage( pPlayStage );
	}
	ShowWnd();
}

void LobbyMainWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	m_szBackOgg = xElement.GetStringAttribute_e( "BackOGG" );
	m_dwBonusCashTooltipCheckDuration = (DWORD)xElement.GetIntAttribute_e( "BonusCashTooltipCheckDuration" );
}

void LobbyMainWnd::iwm_create()
{
	g_GUIMgr.AddWnd( "XML/lobbywnd.xml", this );
	g_GUIMgr.AddWnd( "XML/lobbychatwnd.xml", this );
	g_GUIMgr.AddWnd( "XML/lobbycampinfownd.xml", this );
	g_GUIMgr.AddWnd( "XML/lobbybattleinfownd.xml", this );
	g_GUIMgr.AddWnd( "XML/lobbyplazainfownd.xml", this );
	g_GUIMgr.AddWnd( "XML/lobbytradeinfownd.xml", this );
	g_GUIMgr.AddWnd( "XML/tournamentinfownd.xml", this );
	g_GUIMgr.AddWnd( "XML/campentermodalwnd.xml" );
	g_GUIMgr.AddWnd( "XML/lobbyplazainfownd.xml", this );
	g_GUIMgr.AddWnd( "XML/shuffletooptip.xml", this );
	
	int iXPos = g_GUIMgr.GUIPosLoadInt( "LobbyMainWnd", "XPos", GetXPos() ,POS_SAVE_VERSION );
	int iYPos = g_GUIMgr.GUIPosLoadInt( "LobbyMainWnd", "YPos", GetYPos() ,POS_SAVE_VERSION );

	if( COMPARE( iXPos, 0, Setting::Width() ) && COMPARE( iYPos, 0, Setting::Height() ) )
	{
		SetWndPos( iXPos, iYPos );
	}
	m_iDefaultX = GetXPos();
	m_iDefaultY = GetYPos();

	ioWnd * pEffWnd = FindChildWnd(ID_EFFECT_WND);
	if(pEffWnd)
	{
		m_pEventEff = pEffWnd->GetEffect("lobby event effect");
	}
}

void LobbyMainWnd::iwm_show()
{
	ShowChildWnd( ID_LOBBY_WND );	

	g_BackgroundMgr.SetBackground( BGT_LOBBY );

	// �κ� ����

	SP2Packet kPacket( CTPK_JOIN_SERVER_LOBBY_INFO );
	kPacket << true;
	TCPNetwork::SendToServer( kPacket );


	// special notice
	static bool bShow = false;
	if( !bShow )
	{
		bShow = true;
		if( g_KeyManager.IsAutoFix() )
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
	}

	if(m_pEventEff)
		m_pEventEff->Start();
}

void LobbyMainWnd::iwm_hide()
{
	HideChildWnd( ID_LOBBY_WND );
	HideChildWnd( ID_CHAT_WND );
	HideChildWnd( ID_CAMP_INFO_WND );
	HideChildWnd( ID_BATTLE_INFO_WND );
	HideChildWnd( ID_PLAZA_INFO_WND );
	HideChildWnd( ID_TRADE_INFO_WND );
	HideChildWnd( ID_TOURNAMENT_INFO_WND );

	ModeHelpTooltip *pModeHelpTooltip = (ModeHelpTooltip*)( m_pModeHelpTooltip );	
	if( pModeHelpTooltip )
	{
		pModeHelpTooltip->SetModeHelpTooltip( NULL, "", -1 );
	}	

	CQuickModeToolTip *pQuickTooltip = (CQuickModeToolTip*)( m_pQuickJoinTooltip );
	if(pQuickTooltip)
		pQuickTooltip->SetModeHelpTooltip(NULL, -1);


	// �κ� ����
	SP2Packet kPacket( CTPK_JOIN_SERVER_LOBBY_INFO );
	kPacket << false;
	TCPNetwork::SendToServer( kPacket );
	if(m_pEventEff)
		m_pEventEff->Stop();
}

void LobbyMainWnd::SetModeHelpTooltip( ioWnd *pCallWnd, const ioHashString &szTitle, int iQuickstartModeHelpIndex )
{
	if( m_pModeHelpTooltip == NULL )
		m_pModeHelpTooltip = dynamic_cast< ModeHelpTooltip * >( FindChildWnd( ID_MODE_HELP_TOOLTIP ) );

	ModeHelpTooltip *pModeHelpTooltip = dynamic_cast< ModeHelpTooltip * >( m_pModeHelpTooltip );	
	if( pModeHelpTooltip )
	{
		pModeHelpTooltip->SetModeHelpTooltip(  pCallWnd, szTitle, iQuickstartModeHelpIndex );
	}	
}

void LobbyMainWnd::SetQuickJoinTooltip(ioWnd *pCallWnd, const int nMode)
{
	if( m_pQuickJoinTooltip == NULL )
		m_pQuickJoinTooltip = (CQuickModeToolTip*)( FindChildWnd( ID_QUICKJOIN_TOOLTIP ) );

	CQuickModeToolTip *pQuickTooltip = (CQuickModeToolTip*)( m_pQuickJoinTooltip );
	if( pQuickTooltip )
		pQuickTooltip->SetModeHelpTooltip(pCallWnd, nMode);
}

void LobbyMainWnd::SetShuffleToolTip( bool bShow )
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

bool LobbyMainWnd::iwm_esc()
{
	LobbyChatWnd *pChatWnd = dynamic_cast<LobbyChatWnd*>( FindChildWnd(ID_CHAT_WND) );
	if( pChatWnd && pChatWnd->IsShow() )
		return pChatWnd->iwm_esc();
	return false;
}

void LobbyMainWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();

	switch( dwID )
	{
	case ID_LOBBY_WND:
		if( cmd == IOBN_BTNDOWN )
		{
			CmdMainTab( param );
		}
		break;	

	case ID_NOVICE_BUTTON:
		if(cmd == IOWN_OVERED)
		{
			/// ���� �߰��ϱ�.
			GlobalTextTooltip *pTooltip = static_cast<GlobalTextTooltip*>(g_GUIMgr.FindWnd( ITEM_CUSTOM_INFO_TOOLTIP_WND ));
			if( pTooltip )
			{
				pTooltip->SetTooltipText( m_spNovicePrinter );
			}
		}
		else if(cmd == IOWN_LEAVED)
		{
			//ioLSMouse *pMouse = g_App.GetMouse();
			//if( !pMouse )
			//	return;
			//// ��������
			//pMouse->SetToolTipWndID(0);
			//pMouse->SetToolTipCursor();
			GlobalTextTooltip *pTooltip = static_cast<GlobalTextTooltip*>(g_GUIMgr.FindWnd( ITEM_CUSTOM_INFO_TOOLTIP_WND ));
			if( pTooltip )
			{
				pTooltip->HideWnd();
			}
		}
		break;
	}
}

void LobbyMainWnd::SetPositionToDefault( int iParentWidth, int iParentHeight, bool bChildMove, bool bCreate )
{
	ioMovingWnd::SetPositionToDefault( iParentWidth, iParentHeight, bChildMove, bCreate );

	if( !bCreate )
	{
		m_iDefaultX = GetXPos();
		m_iDefaultY = GetYPos();
	}
}

void LobbyMainWnd::PlayBackgroundOGG()
{
	if( !IsShow() )
		return;

	if( !m_szBackOgg.IsEmpty() )
	{
		g_SoundMgr.StopStream();

		char szName[MAX_PATH];
		sprintf( szName, "Resource/wave/%s", m_szBackOgg.c_str() );

		__try
		{
			g_SoundMgr.OpenOggStream( szName );
			g_SoundMgr.PlayStream( true );
		}
		__except( EXCEPTION_EXECUTE_HANDLER )
		{
			MEMORYSTATUSEX statex;
			statex.dwLength = sizeof (statex);
			GlobalMemoryStatusEx (&statex);

			LOG.PrintTimeAndLog(0, ("There is  %d percent of memory in use."),statex.dwMemoryLoad);
			LOG.PrintTimeAndLog(0, ("There are %d total Mbytes of physical memory."),statex.ullTotalPhys/1048576);
			LOG.PrintTimeAndLog(0, ("There are %d free Mbytes of physical memory."), statex.ullAvailPhys/1048576);
			LOG.PrintTimeAndLog(0, ("There are %d total Mbytes of paging file."), statex.ullTotalPageFile/1048576);
			LOG.PrintTimeAndLog(0, ("There are %d free Mbytes of paging file."), statex.ullAvailPageFile/1048576);
			LOG.PrintTimeAndLog(0, ("There are %d total Mbytes of virtual memory."), statex.ullTotalVirtual/1048576);
			LOG.PrintTimeAndLog(0, ("There are %d free Mbytes of virtual memory."), statex.ullAvailVirtual/1048576);
			LOG.PrintTimeAndLog(0, ("There are %d free Mbytes of extended memory."), statex.ullAvailExtendedVirtual/1048576);

			LOG.PrintTimeAndLog( 0, "%s %s", __FUNCTION__, m_szBackOgg.c_str() );
		}
	}
}

void LobbyMainWnd::RefreshTab()
{
	LobbyWnd *pLobbyWnd = dynamic_cast<LobbyWnd*>( FindChildWnd(ID_LOBBY_WND) );
	if( pLobbyWnd )
	{
		pLobbyWnd->RefreshTab();
	}
}

void LobbyMainWnd::SetCampLink()
{
	LobbyWnd *pLobbyWnd = dynamic_cast<LobbyWnd*>( FindChildWnd(ID_LOBBY_WND) );
	if( pLobbyWnd )
	{
		pLobbyWnd->SetCampLink();
	}
}

void LobbyMainWnd::SetTournamentLink( DWORD dwTourIndex )
{
	LobbyWnd *pLobbyWnd = dynamic_cast<LobbyWnd*>( FindChildWnd(ID_LOBBY_WND) );
	if( pLobbyWnd )
	{
		pLobbyWnd->SetTournamentLink( dwTourIndex );
	}
}

void LobbyMainWnd::CmdMainTab( DWORD dwTabID )
{
	switch( dwTabID )
	{
	case LobbyWnd::ID_BATTLE_BTN:
		{
			ShowChatWnd( true );
			HideChildWnd( ID_CAMP_INFO_WND );
			ShowChildWnd( ID_BATTLE_INFO_WND, true );
			HideChildWnd( ID_PLAZA_INFO_WND );
			HideChildWnd( ID_TRADE_INFO_WND );
			HideChildWnd( ID_TOURNAMENT_INFO_WND );

#if defined( USE_GA )
			// WND_LOBBY_BATTLE
			g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FLOBBY%2FBATTLE" );
#endif
		}
		break;
	case LobbyWnd::ID_CAMP_BTN:
		{
			if( g_MyInfo.GetUserCampPos() == CAMP_NONE )
				ShowChatWnd( false );
			ShowChildWnd( ID_CAMP_INFO_WND, true );
			HideChildWnd( ID_BATTLE_INFO_WND );
			HideChildWnd( ID_PLAZA_INFO_WND );
			HideChildWnd( ID_TRADE_INFO_WND );
			HideChildWnd( ID_TOURNAMENT_INFO_WND );

#if defined( USE_GA )
			// WND_LOBBY_FACTION
			g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FLOBBY%2FFACTION" );
#endif
		}
		break;
	case LobbyWnd::ID_HERO_MAIN_BTN:
		{
			ShowChildWnd( ID_CAMP_INFO_WND, true );
			HideChildWnd( ID_BATTLE_INFO_WND );
			HideChildWnd( ID_PLAZA_INFO_WND );
			HideChildWnd( ID_TRADE_INFO_WND );
			HideChildWnd( ID_TOURNAMENT_INFO_WND );
		}
		break;
	case LobbyWnd::ID_PLAZA_BTN:
		{
			ShowChatWnd( true );
			HideChildWnd( ID_CAMP_INFO_WND );
			HideChildWnd( ID_BATTLE_INFO_WND );
			ShowChildWnd( ID_PLAZA_INFO_WND );
			HideChildWnd( ID_TRADE_INFO_WND );
			HideChildWnd( ID_TOURNAMENT_INFO_WND );

#if defined( USE_GA )
			// WND_LOBBY_PLAZA
			g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FLOBBY%2FPLAZA" );
#endif
		}
		break;
	case LobbyWnd::ID_TRADE_BTN:
		{
			ShowChatWnd( false );
			HideChildWnd( ID_CAMP_INFO_WND );
			HideChildWnd( ID_BATTLE_INFO_WND );
			HideChildWnd( ID_PLAZA_INFO_WND );
			ShowChildWnd( ID_TRADE_INFO_WND );
			HideChildWnd( ID_TOURNAMENT_INFO_WND );

#if defined( USE_GA )
			// WND_LOBBY_MARKET
			g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FLOBBY%2FMARKET" );
#endif
		}
		break;
	case LobbyWnd::ID_TOURNAMENT_BTN:
		{
			ShowChatWnd( false );
			HideChildWnd( ID_CAMP_INFO_WND );
			HideChildWnd( ID_BATTLE_INFO_WND );
			HideChildWnd( ID_PLAZA_INFO_WND );
			HideChildWnd( ID_TRADE_INFO_WND );
			ShowChildWnd( ID_TOURNAMENT_INFO_WND );

#if defined( USE_GA )
			// WND_LOBBY_TOURNAMENT
			g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FLOBBY%2FTOURNAMENT" );
#endif
		}
		break;
	}
}

void LobbyMainWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;

	ioMovingWnd::OnProcess( fTimePerSec );
	if( g_GUIMgr.IsDragMove( m_dwID ) || IsMoving() )
	{
		if( m_iDefaultX != GetXPos() )
		{
			if( COMPARE( GetXPos(), 0, (Setting::Width() - GetWidth() ) + 1 ) )
				m_iDefaultX = GetXPos();
		}

		if( m_iDefaultY != GetYPos() )
		{
			int iYPos = GetYPos();
			if( COMPARE( iYPos, 0, (Setting::Height() - GetHeight() ) + 1 ) )
				m_iDefaultY = iYPos;
		}
	}

	CheckBonusCashTooltip();
}
void LobbyMainWnd::OnRender()
{
	g_BackgroundMgr.RenderBackground();
}

void LobbyMainWnd::ApplyNormalTeamList( SP2Packet &rkPacket )
{
	LobbyWnd *pLobbyWnd = dynamic_cast<LobbyWnd*>(FindChildWnd(ID_LOBBY_WND));
	if( pLobbyWnd )
	{
		pLobbyWnd->ApplyNormalTeamList( rkPacket );
	}
}

void LobbyMainWnd::ApplyPlazaList( SP2Packet &rkPacket )
{
	LobbyWnd *pLobbyWnd = dynamic_cast<LobbyWnd*>(FindChildWnd(ID_LOBBY_WND));
	if( pLobbyWnd )
	{
		pLobbyWnd->ApplyPlazaList( rkPacket );
	}
}

void LobbyMainWnd::ApplyLadderTeamList( SP2Packet &rkPacket )
{
	LobbyWnd *pLobbyWnd = dynamic_cast<LobbyWnd*>(FindChildWnd(ID_LOBBY_WND));
	if( pLobbyWnd )
	{
		pLobbyWnd->ApplyLadderTeamList( rkPacket );
	}	
}

void LobbyMainWnd::ApplyLobbyUserList( SP2Packet &rkPacket )
{
	LobbyBattleInfoWnd *pBattleInfoWnd = dynamic_cast<LobbyBattleInfoWnd *>(FindChildWnd(ID_BATTLE_INFO_WND));
	LobbyPlazaInfoWnd *pPlazaInfoWnd = dynamic_cast<LobbyPlazaInfoWnd*>(FindChildWnd(ID_PLAZA_INFO_WND));
	LobbyTradeInfoWnd *pTradeInfoWnd = dynamic_cast<LobbyTradeInfoWnd*>(FindChildWnd(ID_TRADE_INFO_WND));

	if( pBattleInfoWnd && pBattleInfoWnd->IsShow() )
		pBattleInfoWnd->ApplyLobbyUserList( rkPacket );
	else if( pPlazaInfoWnd && pPlazaInfoWnd->IsShow() )
		pPlazaInfoWnd->ApplyLobbyUserList( rkPacket );
	else if( pTradeInfoWnd && pTradeInfoWnd->IsShow() )
		pTradeInfoWnd->ApplyLobbyUserList( rkPacket );
}

void LobbyMainWnd::RefreshBattleList( DWORD dwIndex )
{
	LobbyWnd *pLobbyWnd = dynamic_cast<LobbyWnd*>(FindChildWnd( ID_LOBBY_WND ));
	if( pLobbyWnd && pLobbyWnd->IsShow() )
		pLobbyWnd->RefreshBattleList( dwIndex );
}

void LobbyMainWnd::ShowChatWnd( bool bShow )
{
	if( bShow )
		ShowChildWnd( ID_CHAT_WND );
	else
		HideChildWnd( ID_CHAT_WND );
}

bool LobbyMainWnd::QuestGuide( int iModeSelectIndex, float &rfXPos, float &rfYPos )
{
	LobbyWnd *pLobbyWnd = dynamic_cast<LobbyWnd*>(FindChildWnd( ID_LOBBY_WND ) );
	if( pLobbyWnd )
	{
		return pLobbyWnd->QuestGuide( iModeSelectIndex, rfXPos, rfYPos );
	}
	return false;
}

void LobbyMainWnd::ApplyTournamentList( SP2Packet &rkPacket )
{
	LobbyWnd *pLobbyWnd = dynamic_cast<LobbyWnd*>(FindChildWnd(ID_LOBBY_WND));
	if( pLobbyWnd )
	{
		pLobbyWnd->ApplyTournamentList( rkPacket );
	}	
}

void LobbyMainWnd::ApplyTournamentConfirmCheck( SP2Packet &rkPacket )
{
	LobbyWnd *pLobbyWnd = dynamic_cast<LobbyWnd*>(FindChildWnd(ID_LOBBY_WND));
	if( pLobbyWnd )
	{
		pLobbyWnd->ApplyTournamentConfirmCheck( rkPacket );
	}	
}

void LobbyMainWnd::ApplyTournamentConfirmRequest( SP2Packet &rkPacket )
{
	LobbyWnd *pLobbyWnd = dynamic_cast<LobbyWnd*>(FindChildWnd(ID_LOBBY_WND));
	if( pLobbyWnd )
	{
		pLobbyWnd->ApplyTournamentConfirmRequest( rkPacket );
	}	
}

void LobbyMainWnd::OnRefreshTradeItemList()
{
	// Trade info wnd
	LobbyTradeInfoWnd *pTradeInfoWnd = dynamic_cast<LobbyTradeInfoWnd*>( FindChildWnd( ID_TRADE_INFO_WND ) );
	if( pTradeInfoWnd )
	{
		pTradeInfoWnd->OnRefreshTradeItemList();
	}

	// lobby wnd
	LobbyWnd *pLobbyWnd = dynamic_cast<LobbyWnd*>( FindChildWnd( ID_LOBBY_WND ) );
	if( pLobbyWnd )
	{
		pLobbyWnd->OnRefreshTradeItemList();
	}

}

void LobbyMainWnd::CheckBonusCashTooltip()
{
	g_BackgroundMgr.ProcessBackground();

	if ( m_dwBonusCashTooltipCheckDuration == 0 )
		return;
	if ( m_dwBonusCashTooltipCheckStartTime + m_dwBonusCashTooltipCheckDuration > FRAMEGETTIME() )
		return;

	m_dwBonusCashTooltipCheckStartTime = FRAMEGETTIME();

	if ( g_BonusCashMgr.CheckAlarmExpireBonusCash() )
	{
		ioWnd* pBonusTooltip = FindChildWnd( ID_BONUS_CASH_TOOLTIP );
		if ( pBonusTooltip && !pBonusTooltip->IsShow() )
			pBonusTooltip->ShowWnd();
	}
	else
	{
		ioWnd* pBonusTooltip = FindChildWnd( ID_BONUS_CASH_TOOLTIP );
		if ( pBonusTooltip && pBonusTooltip->IsShow() )
			pBonusTooltip->HideWnd();
	}
}

void LobbyMainWnd::ApplyLadderList( SP2Packet &rkPacket )
{
	LobbyWnd *pLobbyWnd = dynamic_cast<LobbyWnd*>(FindChildWnd(ID_LOBBY_WND));
	if( pLobbyWnd )
	{
		pLobbyWnd->ApplyLadderList( rkPacket );
		//pLobbyWnd->ApplyNormalTeamList( rkPacket );
	}
}
