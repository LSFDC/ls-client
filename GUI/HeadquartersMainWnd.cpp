

#include "StdAfx.h"

#include "../io3DEngine/InputBox.h"
#include "../io3DEngine/ioINILoader.h"
#include "../io3DEngine/ioWnd.h"
#include "../io3DEngine/ioEdit.h"
#include "../io3DEngine/ioScroll.h"
#include "../io3DEngine/ioFontManager.h"
#include "../io3DEngine/ioUIRenderer.h"
#include "../io3DEngine/ioMath.h"
#include "../io3DEngine/ioUIImageSetManager.h"
#include "../io3DEngine/ioUIRenderImage.h"
#include "../io3DEngine/ioSoundManager.h"
#include "../io3DEngine/ioXMLElement.h"
#include "../io3DEngine/HelpFunc.h"
#include "../io3DEngine/iostringmanager.h"
#include "../io3dengine/safesprintf.h"

#include "../NetworkWrappingFunc.h"
#include "../TextColorTable.h"
#include "../WndID.h"
#include "../ioMyInfo.h"
#include "../ioGuildInfo.h"
#include "../ioGuildMarkMgr.h"
#include "../ioApplication.h"
#include "../ioPlayStage.h"
#include "../ioMyLevelMgr.h"
#include "../Setting.h"
#include "../SlangUtil.h"
#include "../ioModeSelectInfo.h"
#include "../EtcHelpFunc.h"
#include "../ioLadderTeamMgr.h"
#include "../ioBattleRoomMgr.h"
#include "../ioHeadquartersMode.h"

#include "GlobalWnd.h"
#include "UserListWnd.h"
#include "HeadquartersMainWnd.h"
#include "ioUserInfoMgr.h"
#include "ioFriendManager.h"
#include "ioMessageBox.h"
#include "GameChatInput.h"
#include "TimeGateSearch.h"
#include "ModeMapSelectList.h"
#include "ioSP2GUIManager.h"
#include <strsafe.h>
#include "UserInfoToolTip.h"

HeadquartersInfoWnd::HeadquartersInfoWnd()
{
	m_pBarNormal		= NULL;
	m_pBarNormalLight	= NULL;
	m_pBarOver			= NULL;
	m_pPeopleOrange		= NULL;
	m_pPeopleGray		= NULL;
	m_pModeIconBack		= NULL;
	m_pModeIcon			= NULL;

	m_bMouseLBtnDown    = false;
	m_bMyCurrentRoom    = false;
}

HeadquartersInfoWnd::~HeadquartersInfoWnd()
{
	m_vUserList.clear();

	SAFEDELETE( m_pBarNormal );
	SAFEDELETE( m_pBarNormalLight );
	SAFEDELETE( m_pBarOver );
	SAFEDELETE( m_pPeopleOrange );
	SAFEDELETE( m_pPeopleGray );
	SAFEDELETE( m_pModeIconBack );
	SAFEDELETE( m_pModeIcon );
}

void HeadquartersInfoWnd::SetInfo( SP2Packet &rkPacket )
{
	m_vUserList.clear();
	m_bMyCurrentRoom = false;

	rkPacket >> m_HeadquartersInfo.m_szMasterName;
	rkPacket >> m_HeadquartersInfo.m_iMaxUserCount;
	rkPacket >> m_HeadquartersInfo.m_bLock;

	int iSize;
	rkPacket >> iSize;
	if( iSize > 0 )
	{
		for(int i = 0;i < iSize;i++)
		{
			HeadquartersUserInfo kUserInfo;
			rkPacket >> kUserInfo.m_iGradeLevel >> kUserInfo.m_szUserID >> kUserInfo.m_dwGuildIndex >> kUserInfo.m_dwGuildMark >> kUserInfo.m_iServerPingStep;
			m_vUserList.push_back( kUserInfo );
			if( kUserInfo.m_szUserID == g_MyInfo.GetPublicID() )
				m_bMyCurrentRoom = true;

			g_GuildInfo.SetGuildMark( kUserInfo.m_szUserID, kUserInfo.m_dwGuildIndex, kUserInfo.m_dwGuildMark );
		}
	}	

	// UI TITLE	
	char szTitle[MAX_PATH] = "";
	if( m_HeadquartersInfo.m_bLock )
	{
		SafeSprintf( szTitle, sizeof( szTitle ), STR(1), (int)m_vUserList.size(), m_HeadquartersInfo.m_iMaxUserCount );
	}
	else
	{
		SafeSprintf( szTitle, sizeof( szTitle ), STR(2), (int)m_vUserList.size(), m_HeadquartersInfo.m_iMaxUserCount );
	}
	SetTitleText( szTitle );

	if( !IsShow() )
		ShowWnd();
	else
		iwm_show();
}

void HeadquartersInfoWnd::ProcessSiteClick()
{
	bool bPrevMouseDown = m_bMouseLBtnDown;
	ioHashString szPrevOverName = m_MouseOverName;

	m_bMouseLBtnDown = false;
	m_MouseOverName.Clear();
	if( !g_App.IsVisibleMouse() || g_GUIMgr.GetPreOverWnd() != this )
		return;

	int iXPos, iYPos;
	iXPos = GetDerivedPosX();
	iYPos = GetDerivedPosY();

	int iRealXPos = iXPos + 18;
	int iRealYPos = iYPos + FLOAT100;
	ioLSMouse *pMouse = g_App.GetMouse();
	int iUserSize = m_vUserList.size();
	for( int i=0 ; i < iUserSize ;i++)
	{
		HeadquartersUserInfo &kUserInfo = m_vUserList[i];

		RECT rcNameRect;
		rcNameRect.left   = iRealXPos;
		rcNameRect.top    = iRealYPos;
		rcNameRect.right  = rcNameRect.left + 85;
		rcNameRect.bottom = rcNameRect.top + 21;

		if( ::PtInRect( &rcNameRect, pMouse->GetMousePos() ) )
		{
			if( szPrevOverName != kUserInfo.m_szUserID )
			{
				g_SoundMgr.PlaySound( "interface/public_mouseover_00.wav" );
			}

			m_MouseOverName = kUserInfo.m_szUserID;
			if( pMouse->IsLBtnDown() )
			{
				if( !bPrevMouseDown )
					g_SoundMgr.PlaySound( "interface/public_mouseclick_00.wav" );
				m_bMouseLBtnDown = true;
			}
			else if( pMouse->IsLBtnUp() )
			{
				g_App.OpenMannerWnd( m_MouseOverName.c_str(), true );
			}
			else if( pMouse->IsMouseShowing() )
				pMouse->SetToolTipID( m_MouseOverName, UserInfoToolTip::TT_GLOBAL );
			break;
		}

		if( 0 == ( i + 1 ) % MAX_VERT )
		{
			iRealXPos += 87;
			iRealYPos = iYPos + FLOAT100;
		}
		else
		{
			iRealYPos += 21;
		}
	}
}

void HeadquartersInfoWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "BarNormal" )
	{
		SAFEDELETE( m_pBarNormal );
		m_pBarNormal = pImage;
	}
	else if( szType == "BarNormalLight" )
	{
		SAFEDELETE( m_pBarNormalLight );
		m_pBarNormalLight = pImage;
	}
	else if( szType == "BarOver" )
	{
		SAFEDELETE( m_pBarOver );
		m_pBarOver = pImage;
	}
	else if( szType == "PeopleOrange" )
	{
		SAFEDELETE( m_pPeopleOrange );
		m_pPeopleOrange = pImage;
	}
	else if( szType == "PeopleGray" )
	{
		SAFEDELETE( m_pPeopleGray );
		m_pPeopleGray = pImage;
	}
	else if( szType == "ModeIconBack" )
	{
		SAFEDELETE( m_pModeIconBack );
		m_pModeIconBack = pImage;
	}
	else if( szType == "ModeIcon" )
	{
		SAFEDELETE( m_pModeIcon );
		m_pModeIcon = pImage;
	}
	else
	{
		ioWnd::AddRenderImage( szType, pImage );
	}
}

void HeadquartersInfoWnd::iwm_show()
{
	if( g_MyInfo.IsTutorialUser() ||
		g_MyInfo.GetCharCount() == 0 || 
		(g_MyInfo.GetActiveCharCount() == 1 && g_MyInfo.GetActiveExerciseCharCount()) ||
		g_BattleRoomMgr.IsBattleRoom() ||
		g_LadderTeamMgr.IsLadderTeam() ||
		m_HeadquartersInfo.m_bLock ||
		m_bMyCurrentRoom )
	{	
		SetChildWndStyleAdd( ID_ENTER_HQ, IWS_EXACTIVE );
	}
	else
	{
		SetChildWndStyleRemove( ID_ENTER_HQ, IWS_EXACTIVE );
	}
}

void HeadquartersInfoWnd::iwm_hide()
{
}

void HeadquartersInfoWnd::JoinHeadquartersRoom()
{
	if( g_App.IsMouseBusy() ) return;

	if( g_MyInfo.IsTutorialUser() )
	{	
		HideWnd();
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return;
	}

	if( g_MyInfo.GetCharCount() == 0 )
	{
		HideWnd();
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return;
	}

	if( g_MyInfo.GetActiveCharCount() == 1 && g_MyInfo.GetActiveExerciseCharCount() == 1 )
	{
		HideWnd();
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		return;
	}

	if( g_BattleRoomMgr.IsBattleRoom() )
	{
		HideWnd();
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		return;
	}

	if( g_LadderTeamMgr.IsLadderTeam() )
	{
		HideWnd();
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
		return;
	}

	if( m_bMyCurrentRoom )
	{
		HideWnd();
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6) );
		return;
	}

	if( m_HeadquartersInfo.m_bLock )
	{
		HideWnd();
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(7) );
		return;
	}	

	g_MyInfo.SetJoinHeadquartersRequestName( m_HeadquartersInfo.m_szMasterName );
	SP2Packet kPacket( CTPK_JOIN_HEADQUARTERS );
	kPacket << m_HeadquartersInfo.m_szMasterName << g_ModeSelectInfo.CheckHeadquartersMapIndex( g_ModeSelectInfo.GetHeadquartersModeIndex(), 1 ) << false;
	TCPNetwork::SendToServer( kPacket );
	TCPNetwork::MouseBusy( true );
	HideWnd();
}

void HeadquartersInfoWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_EXIT:
	case ID_CANCEL:
		if( cmd == IOBN_BTNUP )
		{			
			HideWnd();
		}
		break;
	case ID_ENTER_HQ:
		if( cmd == IOBN_BTNUP || cmd == IOEX_BTNUP )
		{
			JoinHeadquartersRoom();
		}
		break;
	}
}

void HeadquartersInfoWnd::OnRenderBar( int iXPos, int iYPos, int iMaxSlot, int iCurSlot, const ioHashString szName, int iGradeLevel )
{
	// ����
	if( iCurSlot % 2 == 0 )
	{
		if( m_pBarNormal )
			m_pBarNormal->Render( iXPos, iYPos );
	}
	else if( m_pBarNormalLight )
	{
		m_pBarNormalLight->Render( iXPos, iYPos );
	}
	
	g_FontMgr.SetTextStyle( TS_NORMAL );	
	g_FontMgr.SetAlignType( TAT_LEFT );
	g_FontMgr.SetBkColor( 0, 0, 0 );

	// ��� ������
	if( szName.IsEmpty() )
	{
		if( iCurSlot < iMaxSlot )
		{
			if( m_pPeopleOrange )
				m_pPeopleOrange->Render( iXPos + 4, iYPos + 2 );
		}
		else
		{
			if( m_pPeopleGray )
				m_pPeopleGray->Render( iXPos + 4, iYPos + 2 );			
		}
	}
	else if( m_MouseOverName == szName ) // ���� ����
	{
		if( m_pBarOver )
			m_pBarOver->Render( iXPos, iYPos );

		g_LevelMgr.RenderGrade( iXPos + 4, iYPos + 2, iGradeLevel, TEAM_PRIVATE_1, UI_RENDER_NORMAL_ADD_COLOR, RGB( 255, 255, 255 ) );
		g_FontMgr.SetTextColor( 255, 255, 255 );
	}
	else
	{
		g_LevelMgr.RenderGrade( iXPos + 4, iYPos + 2, iGradeLevel, TEAM_PRIVATE_1 );
		g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
	}
	g_FontMgr.PrintTextWidthCut( iXPos + 23, iYPos + 3, FONT_SIZE_12, 58.0f, szName.c_str() );
}

void HeadquartersInfoWnd::OnRender()
{
	ioMovingWnd::OnRender();

	int iXPos, iYPos;
	iXPos = GetDerivedPosX();
	iYPos = GetDerivedPosY();

	if( m_pModeIconBack )
	{
		m_pModeIconBack->SetScale( 0.60f, 0.60f );
		m_pModeIconBack->Render( iXPos + 39, iYPos + 70, UI_RENDER_MULTIPLY, TFO_BILINEAR );

		if( m_pModeIcon )
		{
			m_pModeIcon->SetScale( 0.60f, 0.60f );
			m_pModeIcon->Render( iXPos + 39, iYPos + 70, UI_RENDER_NORMAL, TFO_BILINEAR );
		}
	}

	g_FontMgr.SetTextStyle( TS_NORMAL );	
	g_FontMgr.SetAlignType( TAT_LEFT );
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
	g_FontMgr.PrintTextWidthCut( iXPos + 65, iYPos + 55, FONT_SIZE_13, 300.0f, m_HeadquartersInfo.m_szMasterName.c_str() );

	g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
	g_FontMgr.PrintTextWidthCut( iXPos + 65, iYPos + 73, FONT_SIZE_11, 300.0f, STR(1) );

	int iRealXPos = iXPos + 18;
	int iRealYPos = iYPos + FLOAT100;
	int iMaxLoop  = MAX_HORZ * MAX_VERT;
	for(int i = 0;i < iMaxLoop;i++)
	{
		if( i < (int)m_vUserList.size() )
		{
			OnRenderBar( iRealXPos, iRealYPos, m_HeadquartersInfo.m_iMaxUserCount, i, m_vUserList[i].m_szUserID, m_vUserList[i].m_iGradeLevel );
		}
		else
		{
			OnRenderBar( iRealXPos, iRealYPos, m_HeadquartersInfo.m_iMaxUserCount, i, "", 0 );
		}

		if( 0 == ( i + 1 ) % MAX_VERT )
		{
			iRealXPos += 87;
			iRealYPos = iYPos + FLOAT100;
		}
		else
		{
			iRealYPos += 21;
		}
	}
}

void HeadquartersInfoWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() )	return;

	ioMovingWnd::OnProcess( fTimePerSec );
	
	ProcessSiteClick();
}
//////////////////////////////////////////////////////////////////////////
HeadquartersJoinRequestWnd::HeadquartersJoinRequestWnd()
{
	m_pIconBack	= NULL;
	m_pIcon		= NULL;

	m_dwCurrentTime = 0;
}

HeadquartersJoinRequestWnd::~HeadquartersJoinRequestWnd()
{
	SAFEDELETE( m_pIconBack );
	SAFEDELETE( m_pIcon );
}

void HeadquartersJoinRequestWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "IconBack")
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
		ioWnd::AddRenderImage( szType, pImage );
}

void HeadquartersJoinRequestWnd::iwm_show()
{	
}

void HeadquartersJoinRequestWnd::iwm_hide()
{
}

bool HeadquartersJoinRequestWnd::iwm_spacebar()
{
	if( !IsShow() )
		return false;

	ioWnd *pBtn = FindChildWnd( ID_AGREE );
	if( !pBtn ) return false;
	if( !pBtn->IsShow() ) return false;

	iwm_command( pBtn, IOBN_BTNUP, 0 );
	return true;
}

void HeadquartersJoinRequestWnd::OnAgree()
{
	SP2Packet kPacket( CTPK_HEADQUARTERS_JOIN_AGREE );
	kPacket << m_szRequestName;
	TCPNetwork::SendToServer( kPacket );

	HideWnd();
}

void HeadquartersJoinRequestWnd::OnRefuse()
{	
	HideWnd();
}

void HeadquartersJoinRequestWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_AGREE:
		if( cmd == IOBN_BTNUP )
		{
			OnAgree();
		}
		break;
	case ID_EXIT:
	case ID_REFUSE:
		if( cmd == IOBN_BTNUP )
		{
			OnRefuse();
		}
		break;
	}
}

void HeadquartersJoinRequestWnd::SetRequestName( const ioHashString &rkName )
{
	m_szRequestName = rkName;
	m_dwCurrentTime = FRAMEGETTIME();

	char szTitle[MAX_PATH] = "";
	SafeSprintf( szTitle, sizeof( szTitle ), STR(1), REQUEST_DELAY_SEC );
	SetTitleText( szTitle );
}

void HeadquartersJoinRequestWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() )
		return;

	ioMovingWnd::OnProcess( fTimePerSec );

	if( m_dwCurrentTime == 0 )
		return;

	if( FRAMEGETTIME() - m_dwCurrentTime >= REQUEST_DELAY_SEC * 1000 )
	{
		HideWnd();	
		m_dwCurrentTime = 0;
	}
	else
	{
		DWORD dwSec = ((float)(FRAMEGETTIME() - m_dwCurrentTime) / FLOAT1000);
		int   iSec  = max( 0, REQUEST_DELAY_SEC - dwSec );	
		char szTitle[MAX_PATH] = "";
		SafeSprintf( szTitle, sizeof( szTitle ), STR(1), iSec );
		SetTitleText( szTitle );
	}
}

void HeadquartersJoinRequestWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	if( m_pIconBack )
	{
		m_pIconBack->SetScale( 0.55f, 0.55f );
		m_pIconBack->Render( iXPos + 33, iYPos + 70, UI_RENDER_MULTIPLY, TFO_BILINEAR );

		if( m_pIcon )
		{
			m_pIcon->SetScale( 0.55f, 0.55f );
			m_pIcon->Render( iXPos + 33, iYPos + 70, UI_RENDER_NORMAL, TFO_BILINEAR );
		}
	}

	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetAlignType( TAT_LEFT );
	g_FontMgr.SetBkColor( 0, 0, 0 );

	g_FontMgr.SetTextColor( TCT_DEFAULT_GREEN );
	g_FontMgr.PrintTextWidthCut( iXPos + 57, iYPos + 53, FONT_SIZE_13, 133.0f, m_szRequestName.c_str() );

	g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
	g_FontMgr.PrintText( iXPos + 57, iYPos + 71, FONT_SIZE_13,  STR(1) );

	g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
	g_FontMgr.PrintText( iXPos + 16, iYPos + 99, FONT_SIZE_12,  STR(2) );
	g_FontMgr.PrintText( iXPos + 30, iYPos + 117, FONT_SIZE_12, STR(3) );
	g_FontMgr.PrintText( iXPos + 16, iYPos + 135, FONT_SIZE_12, STR(4) );
}
//////////////////////////////////////////////////////////////////////////
HeadquartersMainOptionWnd::HeadquartersMainOptionWnd()
{
	m_pDarkLine	= NULL;
	m_pLightLine= NULL;
	m_pOverLine	= NULL;

	m_iAddWidth	= 0;
	m_iAddHeight= 0;
	m_iListXPos = 0;
	m_iListYPos = 0;
	m_iStartXPos= 0;
	m_iStartYPos= 0;
	m_iHeightGap= 0;
	m_iOverListIndex = -1;
	m_iMaxSlot  = 0;
	m_iSlotArray= 0;
}

HeadquartersMainOptionWnd::~HeadquartersMainOptionWnd()
{
	SAFEDELETE( m_pDarkLine	);
	SAFEDELETE( m_pLightLine );
	SAFEDELETE( m_pOverLine	);
}

void HeadquartersMainOptionWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	m_iAddWidth	= xElement.GetIntAttribute_e( "AddWidth" );
	m_iAddHeight= xElement.GetIntAttribute_e( "AddHeight" );
	m_iListXPos = xElement.GetIntAttribute_e( "ListX" );
	m_iListYPos = xElement.GetIntAttribute_e( "ListY" );
	m_iHeightGap= xElement.GetIntAttribute_e( "HeightGap" );
}

void HeadquartersMainOptionWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "DarkLine" )
	{
		SAFEDELETE( m_pDarkLine );
		m_pDarkLine = pImage;
	}
	else if( szType == "LightLine" )
	{
		SAFEDELETE( m_pLightLine );
		m_pLightLine = pImage;
	}
	else if( szType == "OverLine" )
	{
		SAFEDELETE( m_pOverLine );
		m_pOverLine = pImage;
	}
	else
	{
		ioWnd::AddRenderImage( szType, pImage );
	}
}

void HeadquartersMainOptionWnd::CommandOption( DWORD dwCmd )
{	
	switch( dwCmd )
	{
	case OPTION_INFO:
		{
			g_App.OpenMannerWnd( m_szName.c_str(), true );
		}
		break;
	case OPTION_KICK:
		{	
			if( m_szMasterName != g_MyInfo.GetPublicID() )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );			
				return;
			}

			if( g_MyInfo.GetPublicID() == m_szName )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );			
				return;
			}			

			if( g_App.IsAdminID( m_szName.c_str() ) )
			{
				g_GUIMgr.SetMsgBox( MB_OK,  NULL, STR(3) );			
				return;
			}

			g_GUIMgr.SetMsgBox( MB_YESNO, this, STR(4), m_szName.c_str() );	
		}
		break;
	case OPTION_CLOSE:
		{
			if( m_szMasterName != g_MyInfo.GetPublicID() )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );			
				return;
			}

			TCPNetwork::MouseBusy( true );
			SP2Packet kPacket( CTPK_HEADQUARTERS_COMMAND );
			kPacket << HEADQUARTERS_CMD_MAXPLAYER_CHANGE << m_iSlotArray;
			TCPNetwork::SendToServer( kPacket );			
		}
		break;
	case OPTION_OPEN:
		{
			if( m_szMasterName != g_MyInfo.GetPublicID() )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6) );			
				return;
			}

			TCPNetwork::MouseBusy( true );
			SP2Packet kPacket( CTPK_HEADQUARTERS_COMMAND );
			kPacket << HEADQUARTERS_CMD_MAXPLAYER_CHANGE << m_iSlotArray + 1;
			TCPNetwork::SendToServer( kPacket );			
		}
		break;
	}
}

void HeadquartersMainOptionWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case MESSAGE_BOX:
		if( cmd == IOBN_BTNUP )
		{
			if( param == IDYES )
			{
				//����
				if( g_MyInfo.GetPublicID() == m_szName )
					return;
				if( g_App.IsAdminID( m_szName.c_str() ) )
					return;
				if( m_szMasterName == g_MyInfo.GetPublicID() )
				{
					SP2Packet kPacket( CTPK_HEADQUARTERS_COMMAND );
					kPacket << HEADQUARTERS_CMD_KICK_OUT << m_szName;
					TCPNetwork::SendToServer( kPacket );
				}
			}
		}
		break;
	}
}

void HeadquartersMainOptionWnd::SetOptionInfo( const ioHashString &rkName, const ioHashString &rkMasterName, int iSlotArray, int iMaxSlot )
{
	HideWnd();
	/*
	-���� ������ ���
	�� �ڱ�Ŭ�� - �ڱ�����
	�� ���Ŭ�� - ������� / ����
	�� ���Ŭ�� - �ݱ�
	�� ����Ŭ�� - ����

	-���� �Ϲ��� ���
	�� �ڱ�Ŭ�� - �ڱ�����
	�� ���Ŭ�� - �������

	-��Ī
	[�������� ����]
	[�������� ��Ű��]
	[���� �ݱ�]
	[���� ����]

	-�ɼ� ����� �ϳ��ۿ� ������ ��� �����Ѵ�.
	*/
	m_OptionMap.clear();

	m_szName		= rkName;
	m_szMasterName  = rkMasterName;
	m_iMaxSlot		= iMaxSlot;
	m_iSlotArray	= iSlotArray;

	// ����
	if( m_szMasterName == g_MyInfo.GetPublicID() )
	{
		if( rkName.IsEmpty() )
		{
			if( m_iSlotArray >= m_iMaxSlot )
				m_OptionMap.insert( OptionMap::value_type( OPTION_OPEN, STR(1) ) );
			else
				m_OptionMap.insert( OptionMap::value_type( OPTION_CLOSE, STR(2) ) );
		}
		else if( rkName == g_MyInfo.GetPublicID() )
		{
			m_OptionMap.insert( OptionMap::value_type( OPTION_INFO, STR(3) ) );
		}
		else
		{
			char szTitle[MAX_PATH] = "";
			SafeSprintf( szTitle, sizeof( szTitle ), STR(4), m_szName.c_str() );
			m_OptionMap.insert( OptionMap::value_type( OPTION_INFO, szTitle ) );

			//����
			if( !g_App.IsAdminID( m_szName.c_str() ) )
			{
				m_OptionMap.insert( OptionMap::value_type( OPTION_KICK, STR(5) ) );
			}
		}
	}
	else    //�Ϲ� - ��� ����ǹǷ� Ÿ��Ʋ�� �Ű澲�� ����.
	{
		if( rkName.IsEmpty() )
		{	
			m_OptionMap.insert( OptionMap::value_type( OPTION_OPEN, STR(6) ) );
		}
		else
		{
			m_OptionMap.insert( OptionMap::value_type( OPTION_INFO, STR(7) ) );
		}
	}

	if( m_OptionMap.empty() )
		return;

	if( m_OptionMap.size() == 1 )
	{
		//��� ����
		OptionMap::iterator iCreate = m_OptionMap.begin();
		if( GetParent() )
		{
			CommandOption( iCreate->first );
		}
	}
	else
	{
		// ������ ������ ����
		int iListWidth = 0;
		OptionMap::iterator iter;
		for( iter = m_OptionMap.begin() ; iter != m_OptionMap.end() ; ++iter )
		{
			int iTextWidth = g_FontMgr.GetTextWidth( iter->second.c_str(), TS_NORMAL, FONT_SIZE_12 );
			if( iListWidth < iTextWidth )
				iListWidth = iTextWidth;
		}
		SetSize( iListWidth + m_iAddWidth + 10, (m_OptionMap.size() * m_iHeightGap) + m_iAddHeight );

		ioLSMouse *pMouse = g_App.GetMouse();
		if( pMouse && GetParent() )
		{
			int iOffsetX = GetParent()->GetXPos();
			int iOffsetY = GetParent()->GetYPos();
			if( pMouse->GetMousePos().x + GetWidth() > Setting::Width() )
				SetWndPos( ( pMouse->GetMousePos().x - GetWidth() ) - iOffsetX, pMouse->GetMousePos().y - iOffsetY );
			else
				SetWndPos( pMouse->GetMousePos().x - iOffsetX, pMouse->GetMousePos().y - iOffsetY );
		}
		ShowWnd();
	}
}

void HeadquartersMainOptionWnd::iwm_show()
{
	SetCheckParentOutSide( true );
}

void HeadquartersMainOptionWnd::iwm_hide()
{
	SetCheckParentOutSide( false );
}

void HeadquartersMainOptionWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() )	return;

	m_iOverListIndex = -1;

	// �ٸ� ������ Ŭ���� �Ͼ���� Ȯ��
	ioWnd *pPreClick = g_GUIMgr.GetPreClickWnd();
	if( pPreClick )
	{
		if( pPreClick != this )
		{
			HideWnd();
			return;
		}
	}

	int iXPos, iYPos;
	iXPos = GetDerivedPosX();
	iYPos = GetDerivedPosY();

	ioLSMouse *pMouse = g_App.GetMouse();

	OptionMap::iterator iter = m_OptionMap.begin();
	for(int iIndex = 0 ; iter != m_OptionMap.end() ; ++iter, ++iIndex )
	{
		RECT rcNameRect;
		rcNameRect.left = iXPos + m_iListXPos;
		rcNameRect.top  = iYPos + m_iListYPos + iIndex * m_iHeightGap;
		rcNameRect.right  = rcNameRect.left + ( GetWidth() - m_iAddWidth );
		rcNameRect.bottom = rcNameRect.top + m_iHeightGap;

		if( ::PtInRect( &rcNameRect, pMouse->GetMousePos() ) )
		{
			m_iOverListIndex  = iIndex;
			if( pMouse->IsLBtnUp() )
			{
				CommandOption( iter->first );
				HideWnd();
			}
			else if( pMouse->IsMouseShowing() )
				pMouse->SetToolTipID( m_szName, UserInfoToolTip::TT_GLOBAL );
			break;
		}
	}
}

void HeadquartersMainOptionWnd::OnRender()
{
	ioWnd::OnRender();

	if( !m_pDarkLine || !m_pLightLine || !m_pOverLine ) 
		return;

	g_FontMgr.SetAlignType( TAT_LEFT );
	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetBkColor( 0, 0, 0 );

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	bool bDarkRender = false;
	OptionMap::iterator iter = m_OptionMap.begin();
	for( int iIndex = 0; iter != m_OptionMap.end() ; ++iter, ++iIndex )
	{
		bDarkRender = !bDarkRender;

		if( m_iOverListIndex == iIndex )
		{
			m_pOverLine->SetSize( GetWidth() - m_iAddWidth, 21 );
			m_pOverLine->SetHorzFormatting( ioUIRenderImage::HF_STRETCH );
			m_pOverLine->SetVertFormatting( ioUIRenderImage::VF_STRETCH );
			m_pOverLine->Render( iXPos + m_iListXPos, iYPos + m_iListYPos + ( iIndex * m_iHeightGap ) );
			g_FontMgr.SetTextColor( 255, 255, 255 );
		}
		else if( bDarkRender )
		{
			m_pDarkLine->SetSize( GetWidth() - m_iAddWidth, 21 );
			m_pDarkLine->SetHorzFormatting( ioUIRenderImage::HF_STRETCH );
			m_pDarkLine->SetVertFormatting( ioUIRenderImage::VF_STRETCH );
			m_pDarkLine->Render( iXPos + m_iListXPos, iYPos + m_iListYPos + ( iIndex * m_iHeightGap ) );
			g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
		}
		else
		{
			m_pLightLine->SetSize( GetWidth() - m_iAddWidth, 21 );
			m_pLightLine->SetHorzFormatting( ioUIRenderImage::HF_STRETCH );
			m_pLightLine->SetVertFormatting( ioUIRenderImage::VF_STRETCH );
			m_pLightLine->Render( iXPos + m_iListXPos, iYPos + m_iListYPos + ( iIndex * m_iHeightGap ) );
			g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
		}

		g_FontMgr.PrintText( iXPos + m_iListXPos + 5, iYPos + m_iListYPos + ( iIndex * m_iHeightGap ) + 3, FONT_SIZE_12, iter->second.c_str() );
	}
}
//////////////////////////////////////////////////////////////////////////
HeadquartersMainWnd::HeadquartersMainWnd() : m_pPlayStage( NULL )
{
	m_bMouseLBtnDown  = false;
	m_MouseOverIndex  = -1;

	m_pBarNormal	 = NULL;
	m_pBarNormalLight= NULL;
	m_pBarOver		 = NULL;
	m_pPeopleOrange  = NULL;
	m_pPeopleGray	 = NULL;
	m_pModeIconBack  = NULL;
	m_pModeIcon		 = NULL;

	m_bMyCurrentRoom = false;
}

HeadquartersMainWnd::~HeadquartersMainWnd()
{
	SAFEDELETE( m_pBarNormal );
	SAFEDELETE( m_pBarNormalLight );
	SAFEDELETE( m_pBarOver );
	SAFEDELETE( m_pPeopleOrange );
	SAFEDELETE( m_pPeopleGray );
	SAFEDELETE( m_pModeIconBack );
	SAFEDELETE( m_pModeIcon );
}

void HeadquartersMainWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "BarNormal" )
	{
		SAFEDELETE( m_pBarNormal );
		m_pBarNormal = pImage;
	}
	else if( szType == "BarNormalLight" )
	{
		SAFEDELETE( m_pBarNormalLight );
		m_pBarNormalLight = pImage;
	}
	else if( szType == "BarOver" )
	{
		SAFEDELETE( m_pBarOver );
		m_pBarOver = pImage;
	}
	else if( szType == "PeopleOrange" )
	{
		SAFEDELETE( m_pPeopleOrange );
		m_pPeopleOrange = pImage;
	}
	else if( szType == "PeopleGray" )
	{
		SAFEDELETE( m_pPeopleGray );
		m_pPeopleGray = pImage;
	}
	else if( szType == "ModeIconBack" )
	{
		SAFEDELETE( m_pModeIconBack );
		m_pModeIconBack = pImage;
	}
	else if( szType == "ModeIcon" )
	{
		SAFEDELETE( m_pModeIcon );
		m_pModeIcon = pImage;
	}
	else
	{
		ioWnd::AddRenderImage( szType, pImage );
	}
}

void HeadquartersMainWnd::SetPositionToDefault( int iParentWidth, int iParentHeight, bool bChildMove, bool bCreate /* = false  */ )
{
	if( bCreate )
	{
		ioWnd::SetPositionToDefault( iParentWidth, iParentHeight, bChildMove, bCreate );
		return;
	}

	RECT rcRect;
	::SetRect( &rcRect, 0, 0, 0, 0 );

	if( !GetParent() || m_bWidthParent )
		rcRect.right = iParentWidth;
	else
		rcRect.right = m_iDefWidth;

	if( !GetParent() || m_bHeightParent )
		rcRect.bottom = iParentHeight;
	else
		rcRect.bottom = m_iDefHeight;

	rcRect.left = m_iDefXPos;
	switch( m_XPosAlign )
	{
	case UI_CENTER_ALIGN:
		rcRect.left += ( iParentWidth - rcRect.right ) / 2;
		break;
	case UI_RIGHT_ALIGN:
		rcRect.left += iParentWidth - rcRect.right;
		break;
	}

	rcRect.top = m_iDefYPos;
	switch( m_YPosAlign )
	{
	case UI_CENTER_ALIGN:
		rcRect.top += ( iParentHeight - rcRect.bottom ) / 2;
		break;
	case UI_BOTTOM_ALIGN:
		rcRect.top += iParentHeight - rcRect.bottom;
		break;
	}

	SetWndPos( rcRect.left, rcRect.top );

	POINT NullPoint = { 0 , 0 };
	m_MoveWnd.SetState( WndMove::WMH_NONE, NullPoint );
}

void HeadquartersMainWnd::iwm_show()
{
	UpdateInfo();
}

void HeadquartersMainWnd::iwm_hide()
{
}

bool HeadquartersMainWnd::iwm_esc()
{
	return ioWnd::iwm_esc();
}

void HeadquartersMainWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_EXIT:
		if( cmd == IOBN_BTNUP )
		{
			HideWnd();
		}
		break;
	case ID_INVITE_BTN:
		if( cmd == IOBN_BTNUP || cmd == IOEX_BTNUP )
		{
			if( m_HeadquartersInfo.m_szMasterName != g_MyInfo.GetPublicID() )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
				return;
			}

			if( g_GUIMgr.IsShow( HEADQUARTERS_INVITE_LIST_WND ) )
				g_GUIMgr.HideWnd( HEADQUARTERS_INVITE_LIST_WND );
			else
				g_GUIMgr.ShowWnd( HEADQUARTERS_INVITE_LIST_WND );
		}
		break;
	case ID_CHANGE_INFO:
		if( cmd == IOBN_BTNUP || cmd == IOEX_BTNUP )
		{
			if( m_HeadquartersInfo.m_szMasterName != g_MyInfo.GetPublicID() )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
				return;
			}

			if( m_HeadquartersInfo.m_szMasterName == g_MyInfo.GetPublicID() )
			{
				g_GUIMgr.ShowWnd( HEADQUARTERS_OWNER_WND );
			}
		}
		break;
	}
}

void HeadquartersMainWnd::UpdateInfo()
{
	if( m_pPlayStage == NULL ) 
	{
		HideWnd();
		return;
	}

	ioHeadquartersMode *pHeadquartersMode = ToHeadquartersMode( m_pPlayStage->GetPlayMode() );
	if( pHeadquartersMode == NULL )
	{
		HideWnd();
		return;
	}

	m_HeadquartersInfo.m_szMasterName = pHeadquartersMode->GetMasterName();
	m_HeadquartersInfo.m_iMaxUserCount= pHeadquartersMode->GetMaxPlayer();
	m_HeadquartersInfo.m_bLock = pHeadquartersMode->IsJoinLock();

	m_UserInfoList.clear();
	m_bMyCurrentRoom = false;
	for( int i = 0 ; i < pHeadquartersMode->GetRecordCharCnt(); i++ )
	{
		RoundInfo kInfo;
		if( pHeadquartersMode->GetRoundInfo( i, kInfo ) )
		{
			m_UserInfoList.push_back( kInfo );
			if( kInfo.m_szName == g_MyInfo.GetPublicID() )
				m_bMyCurrentRoom = true;
		}
	}

	char szTitle[MAX_PATH] = "";
	if( m_HeadquartersInfo.m_bLock )
	{
		SafeSprintf( szTitle, sizeof( szTitle ), STR(1), (int)m_UserInfoList.size(), m_HeadquartersInfo.m_iMaxUserCount );
	}
	else
	{
		SafeSprintf( szTitle, sizeof( szTitle ), STR(2), (int)m_UserInfoList.size(), m_HeadquartersInfo.m_iMaxUserCount );
	}
	SetTitleText( szTitle );

	if( m_HeadquartersInfo.m_szMasterName == g_MyInfo.GetPublicID() )
	{
		SetChildWndStyleRemove( ID_CHANGE_INFO, IWS_EXACTIVE );
		SetChildWndStyleRemove( ID_INVITE_BTN, IWS_EXACTIVE );
	}
	else
	{
		SetChildWndStyleAdd( ID_CHANGE_INFO, IWS_EXACTIVE );
		SetChildWndStyleAdd( ID_INVITE_BTN, IWS_EXACTIVE );
	}
}

void HeadquartersMainWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;
	ioMovingWnd::OnProcess( fTimePerSec );

	ProcessSiteClick();
}

void HeadquartersMainWnd::ProcessSiteClick()
{
	bool bPrevMouseDown = m_bMouseLBtnDown;
	ioHashString szPrevOverName = m_MouseOverName;

	m_bMouseLBtnDown = false;
	m_MouseOverName.Clear();
	m_MouseOverIndex = -1;
	if( !g_App.IsVisibleMouse() || g_GUIMgr.GetPreOverWnd() != this )
		return;

	int iXPos, iYPos;
	iXPos = GetDerivedPosX();
	iYPos = GetDerivedPosY();
	int iRealXPos = iXPos + 18;
	int iRealYPos = iYPos + FLOAT100;
	ioLSMouse *pMouse = g_App.GetMouse();
	int iMaxLoop  = MAX_HORZ * MAX_VERT;
	for(int i = 0;i < iMaxLoop;i++)
	{		
		RECT rcNameRect;
		rcNameRect.left   = iRealXPos;
		rcNameRect.top    = iRealYPos;
		rcNameRect.right  = rcNameRect.left + 85;
		rcNameRect.bottom = rcNameRect.top + 21;

		if( ::PtInRect( &rcNameRect, pMouse->GetMousePos() ) )
		{
			bool bEmptySlot = false;
			char szOverName[MAX_PATH] = "";
			if( i < (int)m_UserInfoList.size() )
				sprintf( szOverName, "%s", m_UserInfoList[i].m_szName.c_str() );
			else
			{
				bEmptySlot = true;
				sprintf_e( szOverName, "Empty.Slot%d", i );

				//
				if( m_HeadquartersInfo.m_szMasterName != g_MyInfo.GetPublicID() )
					break;
			}

			m_MouseOverIndex = i;
			if( szPrevOverName != szOverName )
			{
				g_SoundMgr.PlaySound( "interface/public_mouseover_00.wav" );
			}
			m_MouseOverName = szOverName;
			if( pMouse->IsLBtnDown() )
			{
				if( !bPrevMouseDown )
					g_SoundMgr.PlaySound( "interface/public_mouseclick_00.wav" );
				m_bMouseLBtnDown = true;
			}
			else if( pMouse->IsLBtnUp() )
			{
				HeadquartersMainOptionWnd *pOptionWnd = dynamic_cast<HeadquartersMainOptionWnd*>( FindChildWnd( ID_OPTION_WND ) );
				if( pOptionWnd )
				{
					if( bEmptySlot )
					{
						pOptionWnd->SetOptionInfo( "", m_HeadquartersInfo.m_szMasterName, i, m_HeadquartersInfo.m_iMaxUserCount );
					}
					else
					{
						pOptionWnd->SetOptionInfo( szOverName, m_HeadquartersInfo.m_szMasterName, i, m_HeadquartersInfo.m_iMaxUserCount );
					}
				}
			}
			break;
		}

		if( 0 == ( i + 1 ) % MAX_VERT )
		{
			iRealXPos += 87;
			iRealYPos = iYPos + FLOAT100;
		}
		else
		{
			iRealYPos += 21;
		}
	}
}

void HeadquartersMainWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos, iYPos;
	iXPos = GetDerivedPosX();
	iYPos = GetDerivedPosY();

	if( m_pModeIconBack )
	{
		m_pModeIconBack->SetScale( 0.60f, 0.60f );
		m_pModeIconBack->Render( iXPos + 39, iYPos + 70, UI_RENDER_MULTIPLY, TFO_BILINEAR );

		// ��� ������
		if( m_pModeIcon )
		{
			m_pModeIcon->SetScale( 0.60f, 0.60f );
			m_pModeIcon->Render( iXPos + 39, iYPos + 70, UI_RENDER_NORMAL, TFO_BILINEAR );
		}
	}

	g_FontMgr.SetTextStyle( TS_NORMAL );	
	g_FontMgr.SetAlignType( TAT_LEFT );
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
	g_FontMgr.PrintTextWidthCut( iXPos + 65, iYPos + 55, FONT_SIZE_13, 300.0f, m_HeadquartersInfo.m_szMasterName.c_str() );

	g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
	g_FontMgr.PrintTextWidthCut( iXPos + 65, iYPos + 73, FONT_SIZE_11, 300.0f, STR(1) );

	int iRealXPos = iXPos + 18;
	int iRealYPos = iYPos + FLOAT100;
	int iMaxLoop  = MAX_HORZ * MAX_VERT;
	for(int i = 0;i < iMaxLoop;i++)
	{
		if( i < (int)m_UserInfoList.size() )
		{
			OnRenderBar( iRealXPos, iRealYPos, m_HeadquartersInfo.m_iMaxUserCount, i, m_UserInfoList[i].m_szName, m_UserInfoList[i].m_iLevel );
		}
		else
		{
			OnRenderBar( iRealXPos, iRealYPos, m_HeadquartersInfo.m_iMaxUserCount, i, "", 0 );
		}

		if( 0 == ( i + 1 ) % MAX_VERT )
		{
			iRealXPos += 87;
			iRealYPos = iYPos + FLOAT100;
		}
		else
		{
			iRealYPos += 21;
		}
	}
}

void HeadquartersMainWnd::OnRenderBar( int iXPos, int iYPos, int iMaxSlot, int iCurSlot, const ioHashString szName, int iGradeLevel )
{
	if( !m_pPeopleOrange || !m_pPeopleGray || !m_pBarOver )
		return;

	// ����
	if( iCurSlot % 2 == 0 )
	{
		if( m_pBarNormal )
			m_pBarNormal->Render( iXPos, iYPos );
	}
	else if( m_pBarNormalLight )
	{
		m_pBarNormalLight->Render( iXPos, iYPos );
	}

	g_FontMgr.SetTextStyle( TS_NORMAL );	
	g_FontMgr.SetAlignType( TAT_LEFT );
	g_FontMgr.SetBkColor( 0, 0, 0 );

	// ��� ������
	if( szName.IsEmpty() )
	{
		if( iCurSlot < iMaxSlot )
		{
			if( m_MouseOverIndex == iCurSlot )
			{
				m_pBarOver->Render( iXPos, iYPos );

				m_pPeopleOrange->SetColor( 255, 255, 255 );
				m_pPeopleOrange->Render( iXPos + 4, iYPos + 2, UI_RENDER_NORMAL_ADD_COLOR );
				g_FontMgr.SetTextColor( 255, 255, 255 );
			}
			else
			{
				m_pPeopleOrange->Render( iXPos + 4, iYPos + 2 );
				g_FontMgr.SetTextColor( TCT_DEFAULT_ORANGE );
			}
			g_FontMgr.PrintText( iXPos + 23, iYPos + 3, FONT_SIZE_12, STR(1) );
		}
		else
		{
			if( m_MouseOverIndex == iCurSlot )
			{
				m_pBarOver->Render( iXPos, iYPos );

				m_pPeopleGray->SetColor( 255, 255, 255 );
				m_pPeopleGray->Render( iXPos + 4, iYPos + 2, UI_RENDER_NORMAL_ADD_COLOR );	
				g_FontMgr.SetTextColor( 255, 255, 255 );
			}
			else
			{
				m_pPeopleGray->Render( iXPos + 4, iYPos + 2 );	
				g_FontMgr.SetTextColor( TCT_DEFAULT_LIGHTGRAY );		
			}
			g_FontMgr.PrintText( iXPos + 23, iYPos + 3, FONT_SIZE_12, STR(2) );
		}
	}
	else if( m_MouseOverName == szName ) // ���� ����
	{
		m_pBarOver->Render( iXPos, iYPos );

		g_LevelMgr.RenderGrade( iXPos + 4, iYPos + 2, iGradeLevel, TEAM_PRIVATE_1, UI_RENDER_NORMAL_ADD_COLOR, RGB( 255, 255, 255 ) );
		g_FontMgr.SetTextColor( 255, 255, 255 );
	}
	else
	{
		g_LevelMgr.RenderGrade( iXPos + 4, iYPos + 2, iGradeLevel, TEAM_PRIVATE_1 );
		g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
	}
	g_FontMgr.PrintTextWidthCut( iXPos + 23, iYPos + 3, FONT_SIZE_12, 58.0f, szName.c_str() );
}
//////////////////////////////////////////////////////////////////////////
HeadquartersOwnerWnd::HeadquartersOwnerWnd()
{
	m_dwRadioBtnID = ID_OPEN_BTN;
}

HeadquartersOwnerWnd::~HeadquartersOwnerWnd()
{

}

void HeadquartersOwnerWnd::iwm_show()
{
	UserHeadquartersOption *pUserHeadquartersData = g_MyInfo.GetUserHeadquartersOption();
	if( pUserHeadquartersData )
	{
		if( pUserHeadquartersData->m_sLock == 0 )
		{
			m_dwRadioBtnID = ID_OPEN_BTN;
		}
		else
		{
			m_dwRadioBtnID = ID_CLOSE_BTN;
		}

		CheckRadioButton( ID_OPEN_BTN, ID_CLOSE_BTN, m_dwRadioBtnID );
	}
}

void HeadquartersOwnerWnd::iwm_hide()
{

}

void HeadquartersOwnerWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_EXIT:
	case ID_CANCEL_BTN:
		if( cmd == IOBN_BTNUP )
			HideWnd();
		break;
	case ID_OPEN_BTN:
	case ID_CLOSE_BTN:
		if( cmd == IOBN_BTNDOWN )
		{
			m_dwRadioBtnID = dwID;
			CheckRadioButton( ID_OPEN_BTN, ID_CLOSE_BTN, m_dwRadioBtnID );
		}
		break;
	case ID_CHANGE_BTN:
		if( cmd == IOBN_BTNUP )
		{
			UserHeadquartersOption *pUserHeadquartersData = g_MyInfo.GetUserHeadquartersOption();
			if( pUserHeadquartersData )
			{
				short sPreLock = pUserHeadquartersData->m_sLock;
				if( m_dwRadioBtnID == ID_OPEN_BTN )
				{
					pUserHeadquartersData->m_sLock = 0;
				}
				else
				{
					pUserHeadquartersData->m_sLock = 1;
				}
				
				if( sPreLock != pUserHeadquartersData->m_sLock )
				{
					SP2Packet kPacket( CTPK_HEADQUARTERS_OPTION_CMD );
					kPacket << HEADQUARTERS_OPTION_LOCK << pUserHeadquartersData->m_sLock;
					TCPNetwork::SendToServer( kPacket );
				}
			}
			HideWnd();
		}
		break;
	}
}

void HeadquartersOwnerWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	ioComplexStringPrinter kPrinter;
	kPrinter.SetTextStyle( TS_NORMAL );
	kPrinter.SetBkColor( 0, 0, 0 );
	kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter.AddTextPiece( FONT_SIZE_13, STR(1) );
	kPrinter.SetTextColor( TCT_DEFAULT_GRAY );
	kPrinter.AddTextPiece( FONT_SIZE_13, STR(2) );
	kPrinter.PrintFullText( iXPos + 37, iYPos + 57, TAT_LEFT );
	kPrinter.ClearList();

	kPrinter.SetTextStyle( TS_NORMAL );
	kPrinter.SetBkColor( 0, 0, 0 );
	kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter.AddTextPiece( FONT_SIZE_13, STR(3) );
	kPrinter.SetTextColor( TCT_DEFAULT_GRAY );
	kPrinter.AddTextPiece( FONT_SIZE_13, STR(4) );
	kPrinter.PrintFullText( iXPos + 37, iYPos + 79, TAT_LEFT );
	kPrinter.ClearList();
}
//////////////////////////////////////////////////////////////////////////
HeadquartersInviteList::HeadquartersInviteList()
{
	m_bInviteActive = true;
	m_iGradeLevel = 0;
	m_dwGuildIndex = 0;
	m_dwGuildMark  = 0;

	m_pGuildMark	= NULL;
	m_pFriendMark	= NULL;
}

HeadquartersInviteList::~HeadquartersInviteList()
{
	SAFEDELETE( m_pGuildMark );
	SAFEDELETE( m_pFriendMark );
}

void HeadquartersInviteList::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "GuildMark" )
	{
		SAFEDELETE( m_pGuildMark );
		m_pGuildMark = pImage;
	}
	else if( szType == "FriendMark" )
	{
		SAFEDELETE( m_pFriendMark );
		m_pFriendMark = pImage;
	}
	else
	{
		ioButton::AddRenderImage( szType, pImage );
	}
}

void HeadquartersInviteList::InviteInfo( int iGradeLevel, ioHashString szName, DWORD dwGuildIndex, DWORD dwGUildMark, bool bActive )
{
	m_bInviteActive = bActive;
	m_iGradeLevel   = iGradeLevel;
	m_dwGuildIndex  = dwGuildIndex;
	m_dwGuildMark   = dwGUildMark;
	m_szName        = szName;

	HideChildWnd( ID_INVITE_BTN );
	if( !szName.IsEmpty() )
	{
		ShowChildWnd( ID_INVITE_BTN );
		SetActive();
	}
	else
	{
		SetInActive();
	}

	if( !m_bInviteActive )
	{
		HideChildWnd( ID_INVITE_BTN );
	}
	else
	{
		ShowChildWnd( ID_INVITE_BTN );
	}
}

void HeadquartersInviteList::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_INVITE_BTN:
		if( cmd == IOBN_BTNUP )
		{
			if( g_App.IsAdminID( m_szName.c_str() ) )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
				return;
			}

			if( !m_bInviteActive )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
				return;
			}

			//�ʴ�
			SP2Packet kPacket( CTPK_HEADQUARTERS_USER_INVITE );
			kPacket << 1 << m_szName;
			TCPNetwork::SendToServer( kPacket );

			if( GetParent() )
			{
				HeadquartersInviteListWnd *pParentWnd = dynamic_cast<HeadquartersInviteListWnd*>(GetParent());
				pParentWnd->StartInivteWait( GetLevel(), GetName() );
			}
		}
		break;
	}
}

void HeadquartersInviteList::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;
	if( m_szName.IsEmpty() ) return;

	if( m_bOver && !m_bClicked )
	{
		ioLSMouse *pMouse = g_App.GetMouse();
		if( pMouse && pMouse->IsMouseShowing() )
			pMouse->SetToolTipID( m_szName, UserInfoToolTip::TT_GLOBAL );
	}	
}

void HeadquartersInviteList::OnRender()
{
	ioButton::OnRender();

	if( !m_szName.IsEmpty() )
	{
		int iXPos = GetDerivedPosX();
		int iYPos = GetDerivedPosY();

		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextStyle( TS_NORMAL );
		g_FontMgr.SetAlignType( TAT_LEFT );

		if( m_bOver || m_bClicked )
		{
			g_LevelMgr.RenderGrade( m_szName, iXPos + 4, iYPos + 2, m_iGradeLevel, TEAM_PRIVATE_1, UI_RENDER_NORMAL_ADD_COLOR, RGB( 255, 255, 255 ) );
			g_FontMgr.SetTextColor( 255, 255, 255 );
		}
		else
		{
			g_LevelMgr.RenderGrade( m_szName, iXPos + 4, iYPos + 2, m_iGradeLevel, TEAM_PRIVATE_1 );
			g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
		}
		g_FontMgr.PrintTextWidthCut( iXPos + 23, iYPos + 3, FONT_SIZE_12, 133.0f, m_szName.c_str() );
		int iRealNameSize = g_FontMgr.GetTextWidthCutSize( m_szName.c_str(), TS_NORMAL, FONT_SIZE_12, 133.0f );
		g_GuildMarkMgr.RenderSmallMark( m_dwGuildIndex, m_dwGuildMark, iXPos + 23 + iRealNameSize + 3, iYPos + 4 );
		if( m_dwGuildIndex != 0 )
			iRealNameSize += 16;
		// ��� - > ģ�� - > �Ϲ�
		if( g_GuildInfo.IsMyGuildUser( m_szName ) && m_pGuildMark )
			m_pGuildMark->Render( iXPos + 23 + iRealNameSize + 3, iYPos + 3 );
		else if( g_FriendManager.IsFriend( m_szName ) && m_pFriendMark )
			m_pFriendMark->Render( iXPos + 23 + iRealNameSize + 3, iYPos + 3 );
	}		
}
//////////////////////////////////////////////////////////////////////////
HeadquartersInviteListWnd::HeadquartersInviteListWnd() : m_pPreEdit( NULL )
{
	m_pPlayStage = NULL;
	m_iCurPage = m_iPrevCurPage = m_iMaxPage = 0;
	m_bInviteActive = true;
	m_dwSearchTime = 0;
}

HeadquartersInviteListWnd::~HeadquartersInviteListWnd()
{
	m_vUserList.clear();
}

void HeadquartersInviteListWnd::StartSend( bool bProcess )
{
	SP2Packet kPacket( CTPK_HEADQUARTERS_INVITE_LIST );
	kPacket << m_iCurPage << MAX_PAGE_LIST;
	TCPNetwork::SendToServer( kPacket );	
	m_dwSearchTime = FRAMEGETTIME();
}

void HeadquartersInviteListWnd::ProcessSearch()
{
	if( FRAMEGETTIME() - m_dwSearchTime > INVITE_SEARCH_TIME )
	{
		StartSend( true );
	}
}

void HeadquartersInviteListWnd::iwm_show()
{	
	g_GUIMgr.HideWnd( INVITE_PROGRESS_WND );

	if( m_pPlayStage == NULL )
	{
		HideWnd();
		return;
	}

	ioHeadquartersMode *pHeadquartersMode = ToHeadquartersMode( m_pPlayStage->GetPlayMode() );
	if( pHeadquartersMode == NULL )
	{
		HideWnd();
		return;
	}

	if( pHeadquartersMode->GetRecordPlayUserCnt() >= pHeadquartersMode->GetMaxPlayer() )
	{	
		m_bInviteActive = false;
	}
	else
	{	
		m_bInviteActive = true;
	}


	m_vUserList.clear();
	m_iCurPage = m_iPrevCurPage;
	m_iPrevCurPage = 0;
	StartSend();

	for(int i = ID_USER_LIST_1;i < ID_USER_LIST_10 + 1;i++)
	{
		HeadquartersInviteList *pItem = (HeadquartersInviteList*)FindChildWnd( i );
		if( pItem )
		{
			pItem->InviteInfo( 0, "", 0, 0, false );
		}
	}	

	// ���̵� �Է�
	m_pPreEdit = ioEdit::m_pKeyFocusEdit;

	ioEdit *pNameEdit = (ioEdit*)FindChildWnd( ID_NAME_EDIT );	
	if( pNameEdit )
	{
		pNameEdit->ClearString();
		pNameEdit->SetKeyFocus();
		g_InputBox.SetString( "" );
	}
}

void HeadquartersInviteListWnd::iwm_hide()
{	
	m_vUserList.clear();

	ioEdit *pNameEdit = (ioEdit*)FindChildWnd( ID_NAME_EDIT );
	if( pNameEdit )
	{
		pNameEdit->ClearString();
		pNameEdit->KillKeyFocus();		
	}

	if( m_pPreEdit )	
	{
		ioWnd *pParent = m_pPreEdit->GetParent();
		if( pParent )
		{
			if( pParent->IsShow() )
				((ioEdit*)m_pPreEdit)->SetKeyFocus();	
		}
	}	
	m_pPreEdit = NULL;
}

void HeadquartersInviteListWnd::UpdateSetPage( int iCurPage )
{
	int iPrevPage = m_iCurPage;
	m_iCurPage = min( m_iMaxPage, iCurPage );

	if( m_iCurPage != iPrevPage )
		StartSend();

	bool bActive = false;
	int iListSIze = m_vUserList.size();
	int iStartPos = 0;
	for(int i = ID_USER_LIST_1;i < ID_USER_LIST_10 + 1;i++,iStartPos++)
	{
		HeadquartersInviteList *pItem = (HeadquartersInviteList*)FindChildWnd( i );
		if( pItem )
		{
			if( iStartPos < iListSIze )
			{
				UserInviteInfo &kInfo = m_vUserList[iStartPos];
				pItem->InviteInfo( kInfo.m_iLevel, kInfo.m_szName, 
								   g_UserInfoMgr.GetGuildIndex( kInfo.m_szName ), g_UserInfoMgr.GetGuildMark( kInfo.m_szName ), m_bInviteActive );
			}
			else
			{
				pItem->InviteInfo( 0, "", 0, 0, false );
			}
		}
	}	
}

void HeadquartersInviteListWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_EXIT:
		if( cmd == IOBN_BTNUP )
		{
			HideWnd();
		}
		break;
	case ID_ALL_INVITE:
		if( cmd == IOBN_BTNUP )
		{
			AllInvite();
		}
		break;
	case ID_NAME_INVITE:
		if( cmd == IOBN_BTNUP )
		{
			NameInvite();
		}
		break;
	case ID_NAME_EDIT:
		if( cmd == IOED_INPUT )
		{
			if( param == 0 || param == 1 )
			{
				iwm_command( FindChildWnd( ID_NAME_INVITE ), IOBN_BTNUP, 0 );
			}			
		}
		break;
	case ID_USER_LIST_1:
	case ID_USER_LIST_2:
	case ID_USER_LIST_3:
	case ID_USER_LIST_4:
	case ID_USER_LIST_5:
	case ID_USER_LIST_6:
	case ID_USER_LIST_7:
	case ID_USER_LIST_8:
	case ID_USER_LIST_9:
	case ID_USER_LIST_10:
		if( cmd == IOBN_BTNUP )
		{
			HeadquartersInviteList *pBtn = dynamic_cast<HeadquartersInviteList*>(pWnd);
			if( !pBtn->GetName().IsEmpty() )
				g_App.OpenMannerWnd( pBtn->GetName().c_str(), true );				
		}
		break;
	case ID_LEFT_PAGE:
		if( cmd == IOBN_BTNUP )
		{
			UpdateSetPage( max( 0, m_iCurPage - 1 ) );
		}
		break;
	case ID_RIGHT_PAGE:
		if( cmd == IOBN_BTNUP )
		{
			UpdateSetPage( min( m_iMaxPage, m_iCurPage + 1 ) );
		}
		break;
	}
}

void HeadquartersInviteListWnd::NameInvite()
{
	if( !m_bInviteActive )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return;
	}

	if( g_App.IsMouseBusy() )
		return;

	ioEdit *pNameEdit = (ioEdit*)FindChildWnd( ID_NAME_EDIT );
	if( !pNameEdit )
		return;

	char szName[MAX_PATH] = "";
	sprintf( szName, "%s", pNameEdit->GetText() );

	pNameEdit->ClearString();
	pNameEdit->SetKeyFocus();
	g_InputBox.SetString( "" );

	if( !g_App.IsRightID( szName ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return;
	}

	ioHashString szIsID = szName;
	if( g_MyInfo.GetPublicID() == szIsID )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		return;
	}

	if( g_App.IsAdminID( szName ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		return;
	}		

	m_iPrevCurPage = m_iCurPage;
	SP2Packet kPacket( CTPK_HEADQUARTERS_USER_INVITE );
	kPacket << 1 << szName;
	TCPNetwork::SendToServer( kPacket );
	StartInivteWait( -1, szName );
}

void HeadquartersInviteListWnd::AllInvite()
{
	if( !m_bInviteActive )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return;
	}

	int i;
	vUserInviteInfo vSendList;
	for(i = 0;i < (int)m_vUserList.size();i++)
	{
		UserInviteInfo &kInfo = m_vUserList[i];
		if( kInfo.m_szName.IsEmpty() ) continue;
		if( g_App.IsAdminID( kInfo.m_szName.c_str() ) ) continue;

		vSendList.push_back( kInfo );
	}

	if( vSendList.empty() )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return;
	}

	m_iPrevCurPage = m_iCurPage;
	SP2Packet kPacket( CTPK_HEADQUARTERS_USER_INVITE );
	kPacket << (int)vSendList.size();
	for(i = 0;i < (int)vSendList.size();i++)
	{
		UserInviteInfo &kInfo = vSendList[i];
		kPacket << kInfo.m_szName;
	}	
	TCPNetwork::SendToServer( kPacket );

	StartInivteWait( (int)vSendList.size(), "" );
	vSendList.clear();
}

void HeadquartersInviteListWnd::StartInivteWait( int iValue, const ioHashString &szName )
{
	if( !IsShow() )
		return;

	HideWnd();
	// �ʴ� ��Ȳ UI SHOW
	InviteProgressWnd *pProgressWnd = dynamic_cast<InviteProgressWnd*>( g_GUIMgr.FindWnd( INVITE_PROGRESS_WND ) );
	if( pProgressWnd )
	{
		pProgressWnd->StartInivteWait( this, iValue, szName );
	}
}

void HeadquartersInviteListWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() )
		return;

	ioMovingWnd::OnProcess( fTimePerSec );
	ProcessSearch();		
}

void HeadquartersInviteListWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	ioComplexStringPrinter kPrinter;
	kPrinter.SetTextStyle( TS_NORMAL );
	kPrinter.SetBkColor( 0, 0, 0 );
	kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter.AddTextPiece( FONT_SIZE_12, STR(1), m_iCurPage + 1 );

	kPrinter.SetTextStyle( TS_NORMAL );
	kPrinter.SetBkColor( 0, 0, 0 );
	kPrinter.SetTextColor( TCT_DEFAULT_GRAY );
	kPrinter.AddTextPiece( FONT_SIZE_12, STR(2), m_iMaxPage + 1 );
	kPrinter.PrintFullText( iXPos + 151, iYPos + 273, TAT_CENTER );

	if( m_vUserList.empty() )
	{
		g_FontMgr.SetTextStyle( TS_NORMAL );
		g_FontMgr.SetAlignType( TAT_CENTER );
		g_FontMgr.SetTextColor( TCT_DEFAULT_LIGHTGRAY );
		g_FontMgr.PrintText( iXPos + 151, iYPos + 120, FONT_SIZE_12, STR(3) );
	}
}

void HeadquartersInviteListWnd::SetUserList( SP2Packet &rkPacket )
{	
	m_vUserList.clear();

	int i = 0;
	int iTotalList;
	int iCurList;
	rkPacket >> iTotalList >> iCurList;
	for(i = 0;i < iCurList;i++)
	{
		DWORD dwGuildIndex, dwGuildMark;
		UserInviteInfo kList;
		rkPacket >> kList.m_szName >> kList.m_iLevel >> dwGuildIndex >> dwGuildMark;
		m_vUserList.push_back( kList );
		g_GuildInfo.SetGuildMark( kList.m_szName, dwGuildIndex, dwGuildMark );
	}    	

	m_iMaxPage = max( 0, iTotalList - 1 ) / MAX_PAGE_LIST;
	UpdateSetPage( m_iCurPage );
}
//////////////////////////////////////////////////////////////////////////
HeadquartersInvitedBtn::HeadquartersInvitedBtn()
{
	m_pPeopleOrange = NULL;
	m_pPeopleGray   = NULL;

	m_iSlotArray = m_iMaxSlot = 0;
	m_iGradeLevel = m_iPingStep = 0;
}

HeadquartersInvitedBtn::~HeadquartersInvitedBtn()
{
	SAFEDELETE( m_pPeopleOrange );
	SAFEDELETE( m_pPeopleGray );
}

void HeadquartersInvitedBtn::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "PeopleOrange" )
	{
		SAFEDELETE( m_pPeopleOrange );
		m_pPeopleOrange = pImage;
	}
	else if( szType == "PeopleGray" )
	{
		SAFEDELETE( m_pPeopleGray );
		m_pPeopleGray = pImage;
	}
	else
	{
		ioButton::AddRenderImage( szType, pImage );
	}
}

void HeadquartersInvitedBtn::iwm_create()
{
	m_PingStep.Create();
}

void HeadquartersInvitedBtn::SetMemberInfo( int iMaxSlot, int iSlotArray, const ioHashString &rkName, int iGradeLevel, int iPingStep )
{
	m_iMaxSlot   = iMaxSlot;
	m_iSlotArray = iSlotArray;
	m_szName     = rkName;
	m_iGradeLevel= iGradeLevel;
	m_iPingStep  = iPingStep;

	if( m_szName.IsEmpty() )
	{
		SetInActive();
	}
	else
	{	
		SetActive();
	}
}

void HeadquartersInvitedBtn::OnRender()
{
	ioButton::OnRender();

	int iXPos, iYPos;
	iXPos = GetDerivedPosX();
	iYPos = GetDerivedPosY();

	if( m_szName.IsEmpty() )
	{
		if( m_iSlotArray < m_iMaxSlot )
		{
			if( m_pPeopleOrange )
				m_pPeopleOrange->Render( iXPos + 4, iYPos + 2 );
		}
		else
		{
			if( m_pPeopleGray )
				m_pPeopleGray->Render( iXPos + 4, iYPos + 2 );			
		}
	}
	else
	{
		g_FontMgr.SetTextStyle( TS_NORMAL );	
		g_FontMgr.SetAlignType( TAT_LEFT );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		if( m_bOver || m_bClicked )
		{
			g_LevelMgr.RenderGrade( iXPos + 4, iYPos + 2, m_iGradeLevel, TEAM_PRIVATE_1, UI_RENDER_NORMAL_ADD_COLOR, RGB( 255, 255, 255 ) );
			g_FontMgr.SetTextColor( 255, 255, 255 );
		}
		else
		{
			g_LevelMgr.RenderGrade( iXPos + 4, iYPos + 2, m_iGradeLevel, TEAM_PRIVATE_1 );
			g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
		}
		g_FontMgr.PrintTextWidthCut( iXPos + 23, iYPos + 3, FONT_SIZE_12, 59.0f, m_szName.c_str() );

		m_PingStep.RenderPing( iXPos + 85, iYPos + 2, m_iPingStep, false );
	}
}

void HeadquartersInvitedBtn::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;
	if( m_szName.IsEmpty() ) return;

	if( m_bOver && !m_bClicked )
	{
		ioLSMouse *pMouse = g_App.GetMouse();
		if( pMouse && pMouse->IsMouseShowing() )
			pMouse->SetToolTipID( m_szName, UserInfoToolTip::TT_GLOBAL );
	}	
}
//////////////////////////////////////////////////////////////////////////
bool HeadquartersInvitedWnd::sg_bAutoRefuse = false;
HeadquartersInvitedWnd::HeadquartersInvitedWnd()
{
	m_iMaxPlayer = 0;
	m_dwStartTime = 0;

	m_iCurPage = 0;
	m_iMaxPage = 3;

	m_pModeIconBack = NULL;
	m_pModeIcon     = NULL;
}

HeadquartersInvitedWnd::~HeadquartersInvitedWnd()
{
	SAFEDELETE( m_pModeIconBack );
	SAFEDELETE( m_pModeIcon );
}

void HeadquartersInvitedWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "ModeIconBack" )
	{
		SAFEDELETE( m_pModeIconBack );
		m_pModeIconBack = pImage;
	}
	else if( szType == "ModeIcon" )
	{
		SAFEDELETE( m_pModeIcon );
		m_pModeIcon = pImage;
	}
	else
	{
		ioWnd::AddRenderImage( szType, pImage );
	}
}

void HeadquartersInvitedWnd::SetInfo( const ioHashString &rkMasterName, SP2Packet &rkPacket )
{
	m_vUserList.clear();

	m_szMasterName = rkMasterName;
	rkPacket >> m_iMaxPlayer;

	bool bFriendInParty = false;
	int iUserSize;
	rkPacket >> iUserSize;
	for(int i = 0;i < iUserSize;i++)
	{
		HeadquartersUserInfo kUserInfo;
		rkPacket >> kUserInfo.m_szUserID >> kUserInfo.m_iGradeLevel >> kUserInfo.m_iServerPingStep;
		if( kUserInfo.m_szUserID.IsEmpty() ) continue;

		m_vUserList.push_back( kUserInfo );

		if( !bFriendInParty )
		{
			if( g_FriendManager.IsFriend(kUserInfo.m_szUserID) || g_GuildInfo.IsMyGuildUser( kUserInfo.m_szUserID )  )
			{
				bFriendInParty = true;
				// ģ�� ���� ����
				g_FriendManager.ClientFriendInfoUpdate( kUserInfo.m_szUserID, kUserInfo.m_iGradeLevel, UP_HEADQUARTERS );
			}
		}
	}
	if( !bFriendInParty )
	{
		if( sg_bAutoRefuse )
		{
			return;
		}
	}

	g_SoundMgr.PlaySound( "interface/team_invite.wav" );
	m_dwStartTime = FRAMEGETTIME();

	char szTitle[MAX_PATH] = "";
	SafeSprintf( szTitle, sizeof( szTitle ), STR(1), (int)m_vUserList.size(), m_iMaxPlayer );
	SetTitleText( szTitle );

	if( IsShow() )
		HideWnd();
	ShowWnd();
}

void HeadquartersInvitedWnd::JoinHeadquarters()
{
	if( g_App.IsMouseBusy() ) return;

	if( g_MyInfo.IsTutorialUser() )
	{	
		HideWnd();
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return;
	}

	if( g_MyInfo.GetCharCount() == 0 )
	{
		HideWnd();
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return;
	}

	if( g_MyInfo.GetActiveCharCount() == 1 && g_MyInfo.GetActiveExerciseCharCount() == 1 )
	{
		HideWnd();
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		return;
	}

	if( g_BattleRoomMgr.IsBattleRoom() )
	{
		HideWnd();
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		return;
	}

	if( g_LadderTeamMgr.IsLadderTeam() )
	{
		HideWnd();
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
		return;
	}

	g_MyInfo.SetJoinHeadquartersRequestName( m_szMasterName );
	SP2Packet kPacket( CTPK_JOIN_HEADQUARTERS );
	kPacket << m_szMasterName << g_ModeSelectInfo.CheckHeadquartersMapIndex( g_ModeSelectInfo.GetHeadquartersModeIndex(), 1 ) << true;
	TCPNetwork::SendToServer( kPacket );
	TCPNetwork::MouseBusy( true );
	HideWnd();
}

void HeadquartersInvitedWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_EXIT:
	case ID_REFUSE:
		if( cmd == IOBN_BTNUP )
		{			
			HideWnd();
		}
		break;
	case ID_AUTO_REFUSE:
		if( cmd == IOBN_BTNUP )
		{	
			sg_bAutoRefuse = !sg_bAutoRefuse;
			if( sg_bAutoRefuse )
			{
				HideWnd();
			}			
		}
		break;
	case ID_LEFT_PAGE:
		if( cmd == IOBN_BTNUP )
		{
			UpdatePage( m_iCurPage - 1 );
		}
		break;
	case ID_RIGHT_PAGE:
		if( cmd == IOBN_BTNUP )
		{	
			UpdatePage( m_iCurPage + 1 );
		}
		break;
	case ID_AGREE:
		if( cmd == IOBN_BTNUP )
		{
			JoinHeadquarters();
		}
		break;
	case ID_LIST_1:
	case ID_LIST_2:
	case ID_LIST_3:
	case ID_LIST_4:
	case ID_LIST_5:
	case ID_LIST_6:
	case ID_LIST_7:
	case ID_LIST_8:
		if( cmd == IOBN_BTNUP )
		{
			HeadquartersInvitedBtn *pUserBtn = dynamic_cast<HeadquartersInvitedBtn*>(pWnd);
			if( pUserBtn )
				g_App.OpenMannerWnd( pUserBtn->GetMemberName().c_str(), true );
		}
		break;
	}
}

const HeadquartersUserInfo &HeadquartersInvitedWnd::GetUserInfo( int iArray )
{
	for(int i = 0;i < (int)m_vUserList.size();i++)
	{
		if( --iArray < 0 )
			return m_vUserList[i];
	}

	static HeadquartersUserInfo kReturnInfo;
	return kReturnInfo;
}

void HeadquartersInvitedWnd::UpdatePage( int iCurPage )
{
	m_iCurPage = max( 0, iCurPage );
	m_iCurPage = min( m_iCurPage, m_iMaxPage );
	const int iStartArray = m_iCurPage * MAX_PAGE_LIST;

	int i = 0;
	int iSlotArray = 0;		
	for(i = 0,iSlotArray = iStartArray;i < MAX_PAGE_LIST;i++,iSlotArray++)
	{
		HeadquartersInvitedBtn *pHeadquartersUserBtn = dynamic_cast<HeadquartersInvitedBtn*>(FindChildWnd( ID_LIST_1 + i ));
		if( pHeadquartersUserBtn )
		{
			const HeadquartersUserInfo &rkInfo = GetUserInfo( iSlotArray );
			pHeadquartersUserBtn->SetMemberInfo( m_iMaxPlayer, iSlotArray, rkInfo.m_szUserID, rkInfo.m_iGradeLevel, rkInfo.m_iServerPingStep );
		}
	}
}

void HeadquartersInvitedWnd::iwm_show()
{
	ioButton *pAgreeBtn = dynamic_cast<ioButton*>( FindChildWnd( ID_AGREE ) );
	if( pAgreeBtn )
	{
		char szTitle[MAX_PATH] = "";
		SafeSprintf( szTitle, sizeof( szTitle ), STR(1), INVITE_DELAY_SEC );
		pAgreeBtn->SetTitleText( szTitle );
	}
	UpdatePage( 0 );	

	ioCheckButton *pCheckBtn = dynamic_cast<ioCheckButton*>( FindChildWnd( ID_AUTO_REFUSE ) );
	if( pCheckBtn )
		pCheckBtn->SetCheck( sg_bAutoRefuse );
}

void HeadquartersInvitedWnd::iwm_hide()
{
}

void HeadquartersInvitedWnd::OnRender()
{
	ioMovingWnd::OnRender();

	int iXPos, iYPos;
	iXPos = GetDerivedPosX();
	iYPos = GetDerivedPosY();

	if( m_pModeIconBack )
	{
		m_pModeIconBack->SetScale( 0.60f, 0.60f );
		m_pModeIconBack->Render( iXPos + 39, iYPos + 70, UI_RENDER_MULTIPLY, TFO_BILINEAR );

		// ��� ������
		if( m_pModeIcon )
		{
			m_pModeIcon->SetScale( 0.60f, 0.60f );
			m_pModeIcon->Render( iXPos + 39, iYPos + 70, UI_RENDER_NORMAL, TFO_BILINEAR );
		}
	}

	g_FontMgr.SetTextStyle( TS_NORMAL );	
	g_FontMgr.SetAlignType( TAT_LEFT );
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
	g_FontMgr.PrintTextWidthCut( iXPos + 65, iYPos + 55, FONT_SIZE_13, 175.0f, m_szMasterName.c_str() );

	g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
	g_FontMgr.PrintTextWidthCut( iXPos + 65, iYPos + 73, FONT_SIZE_11, 175.0f, STR(1) );


	////
	{
		ioComplexStringPrinter kPrinter;
		kPrinter.SetTextStyle( TS_NORMAL );
		kPrinter.SetBkColor( 0, 0, 0 );
		kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
		kPrinter.AddTextPiece( FONT_SIZE_12, STR(2), m_iCurPage + 1 );

		kPrinter.SetTextStyle( TS_NORMAL );
		kPrinter.SetBkColor( 0, 0, 0 );
		kPrinter.SetTextColor( TCT_DEFAULT_GRAY );
		kPrinter.AddTextPiece( FONT_SIZE_12, STR(3), m_iMaxPage + 1 );
		kPrinter.PrintFullText( iXPos + 128, iYPos + 193, TAT_CENTER );
	}
}

void HeadquartersInvitedWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;	

	ioMovingWnd::OnProcess( fTimePerSec );

	if(m_dwStartTime == 0)
		return;

	if( FRAMEGETTIME() - m_dwStartTime >= INVITE_DELAY_SEC * 1000 )
	{
		HideWnd();	
		m_dwStartTime = 0;
	}
	else
	{
		ioButton *pBtn = (ioButton*)FindChildWnd( ID_AGREE );
		if( pBtn )
		{
			DWORD dwSec = ((float)(FRAMEGETTIME() - m_dwStartTime) / FLOAT1000);
			int   iSec  = max( 0, INVITE_DELAY_SEC - dwSec );	
			char szTitle[MAX_PATH] = "";
			SafeSprintf( szTitle, sizeof( szTitle ), STR(1), iSec );
			pBtn->SetTitleText( szTitle );
		}
	}
}