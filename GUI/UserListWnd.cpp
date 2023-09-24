

#include "StdAfx.h"

#include "../ioGuildMarkMgr.h"
#include "../ioPlayStage.h"
#include "../ioLadderTeamMgr.h"

#include "UserListWnd.h"
#include "MessengerWnd.h"
#include "CloverToolTip.h"

#include "GuildWnd.h"
#include "UserInfoToolTip.h"

CloverButton::CloverButton()
{
	m_pInActiveNormal = NULL;
	m_pInActiveOvered = NULL;
	m_pInActiveOveredAdd = NULL;
	m_pInActivePushed = NULL;
	m_bActive = true;
}

CloverButton::~CloverButton()
{
	SAFEDELETE( m_pInActiveNormal );
	SAFEDELETE( m_pInActiveOvered );
	SAFEDELETE( m_pInActiveOveredAdd );
	SAFEDELETE( m_pInActivePushed );
}

void CloverButton::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "InActiveNormal" )
	{
		SAFEDELETE( m_pInActiveNormal );
		m_pInActiveNormal = pImage;
	}
	else if( szType == "InActiveOver" )
	{
		SAFEDELETE( m_pInActiveOvered );
		m_pInActiveOvered = pImage;
	}
	else if( szType == "InActiveOverAdd" )
	{
		SAFEDELETE( m_pInActiveOveredAdd );
		m_pInActiveOveredAdd = pImage;
	}
	else if( szType == "InActivePush" )
	{
		SAFEDELETE( m_pInActivePushed );
		m_pInActivePushed = pImage;
	}
	else
	{
		ioButton::AddRenderImage( szType, pImage );
	}
}

void CloverButton::iwm_create()
{
}

void CloverButton::iwm_show()
{
}

void CloverButton::iwm_mouseover( const ioMouse& mouse )
{
	ioButton::iwm_mouseover( mouse );

	UserInfoListWnd *pUserInfoList = dynamic_cast<UserInfoListWnd*>( GetParent() );
	if( pUserInfoList )
	{
		UserListWnd *pUserList = dynamic_cast<UserListWnd*>( pUserInfoList->GetParent() );
		if( pUserList )
		{
			CloverToolTip *pToolTip = dynamic_cast<CloverToolTip*>(pUserList->FindChildWnd( UserListWnd::ID_TOOLTIP ));
			if( pToolTip )
				pToolTip->SetInfo( pUserInfoList->GetUserName(), GetID() );
		}
	}
}

void CloverButton::iwm_mouseleave( const ioMouse& mouse )
{
	ioButton::iwm_mouseleave( mouse );

	UserInfoListWnd *pUserInfoList = dynamic_cast<UserInfoListWnd*>( GetParent() );
	if( pUserInfoList )
	{
		UserListWnd *pUserList = dynamic_cast<UserListWnd*>( pUserInfoList->GetParent() );
		if( pUserList )
		{
			pUserList->HideChildWnd( UserListWnd::ID_TOOLTIP );
		}
	}
}

void CloverButton::OnDrawNormal( int iXPos, int iYPos )
{
	if( m_bActive )
	{
		ioButton::OnDrawNormal( iXPos, iYPos );
	}
	else
	{
		if( m_pInActiveNormal )
			m_pInActiveNormal->Render( iXPos, iYPos );
	}
}

void CloverButton::OnDrawOvered( int iXPos, int iYPos )
{
	if( m_bActive )
	{
		ioButton::OnDrawOvered( iXPos, iYPos );
	}
	else
	{
		if( HasExWndStyle( IWS_EX_OVER_SCREEN ) )
		{
			if( m_pInActiveOvered )
			{
				m_pInActiveOvered->Render( iXPos, iYPos );
				m_pInActiveOvered->Render( iXPos, iYPos, UI_RENDER_SCREEN );
			}
		}
		else
		{
			if( m_pInActiveOvered )
			{
				m_pInActiveOvered->Render( iXPos, iYPos );
			}
		}

		OnDrawOveredAdd( iXPos, iYPos );
	}
}

void CloverButton::OnDrawOveredAdd( int iXPos, int iYPos )
{
	if( m_bActive )
	{
		ioButton::OnDrawOveredAdd( iXPos, iYPos );
	}
	else
	{
		if( m_pInActiveOveredAdd )
			m_pInActiveOveredAdd->Render( iXPos, iYPos,  UI_RENDER_ADD );
	}
}

void CloverButton::OnDrawPushed( int iXPos, int iYPos )
{
	if( m_bActive )
	{
		ioButton::OnDrawPushed( iXPos, iYPos );
	}
	else
	{
		if( m_pInActivePushed )
			m_pInActivePushed->Render( iXPos, iYPos );
	}
}

void CloverButton::SetActive()
{
	m_bActive = true;
}

void CloverButton::SetInActive()
{
	m_bActive = false;
}

//////////////////////////////////////////////////////////////////////////
UserInfoListWnd::UserInfoListWnd()
{
	m_bOnline      = false;
	m_pPlayStage   = NULL;
	m_iListType    = TYPE_FRIENDLIST;
	m_iUserLevel   = 0;
	m_bMyBestFriend = false;
	m_bOtherBestFriend = false;

	m_pBlackMark   = NULL;
	m_pMyBestFriendBlack    = NULL;
	m_pMyBestFriendGray	    = NULL;
	m_pOtherBestFriendBlack = NULL;
	m_pOtherBestFriendGray	= NULL;
	m_pCoupleBestFriendBlack= NULL;
	m_pCoupleBestFriendGray = NULL;
}

UserInfoListWnd::~UserInfoListWnd()
{
	SAFEDELETE( m_pBlackMark );
	SAFEDELETE( m_pMyBestFriendBlack );
	SAFEDELETE( m_pMyBestFriendGray );
	SAFEDELETE( m_pOtherBestFriendBlack );
	SAFEDELETE( m_pOtherBestFriendGray );
	SAFEDELETE( m_pCoupleBestFriendBlack );
	SAFEDELETE( m_pCoupleBestFriendGray );
}

void UserInfoListWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "BlackMark" )
	{
		SAFEDELETE( m_pBlackMark );
		m_pBlackMark = pImage;
	}
	else if( szType == "MyBestFriendBlack" )
	{
		SAFEDELETE( m_pMyBestFriendBlack );
		m_pMyBestFriendBlack = pImage;
	}
	else if( szType == "MyBestFriendGray" )
	{
		SAFEDELETE( m_pMyBestFriendGray );
		m_pMyBestFriendGray = pImage;
	}
	else if( szType == "OtherBestFriendBlack" )
	{
		SAFEDELETE( m_pOtherBestFriendBlack );
		m_pOtherBestFriendBlack = pImage;
	}
	else if( szType == "OtherBestFriendGray" )
	{
		SAFEDELETE( m_pOtherBestFriendGray );
		m_pOtherBestFriendGray = pImage;
	}
	else if( szType == "CoupleBestFriendBlack" )
	{
		SAFEDELETE( m_pCoupleBestFriendBlack );
		m_pCoupleBestFriendBlack = pImage;
	}
	else if( szType == "CoupleBestFriendGray" )
	{
		SAFEDELETE( m_pCoupleBestFriendGray );
		m_pCoupleBestFriendGray = pImage;
	}
	else
	{
		ioButton::AddRenderImage( szType, pImage );
	}
}

void UserInfoListWnd::SetFriendUserInfo( ioHashString szUserName, bool bOnline, int iUserPos, int iUserLevel, bool bMyBestFriend, bool bOtherBestFriend, 
										 ioPlayStage *pPlayStage )
{
	m_szUserName	= szUserName;
	m_bOnline		= bOnline;		
	m_iUserPos		= iUserPos;
	m_iUserLevel	= iUserLevel;
	m_bMyBestFriend = bMyBestFriend;
	m_bOtherBestFriend = bOtherBestFriend;
	m_pPlayStage	= pPlayStage;
	m_iListType     = TYPE_FRIENDLIST;

	if( m_szUserName.IsEmpty() )
	{
		SetInActive();
		HideChildWnd( ID_RECV_CLOVER );
		HideChildWnd( ID_SEND_CLOVER );
	}
	else
	{
		SetActive();
		HideChildWnd( ID_RECV_CLOVER );
		ShowChildWnd( ID_SEND_CLOVER );
	}
}

void UserInfoListWnd::SetGuildUserInfo( ioHashString szUserName, bool bOnline, int iUserPos, int iUserLevel, ioPlayStage *pPlayStage )
{
	m_szUserName	= szUserName;
	m_bOnline		= bOnline;		
	m_iUserPos		= iUserPos;
	m_iUserLevel	= iUserLevel;
	m_pPlayStage	= pPlayStage;
	m_iListType     = TYPE_GUILDUSERLIST;

	if( m_szUserName.IsEmpty() )
		SetInActive();
	else
		SetActive();
}

void UserInfoListWnd::SetBlackListUserInfo( ioHashString szUserName, bool bOnline )
{
	m_szUserName = szUserName;
	m_bOnline    = bOnline;	
	m_pPlayStage = NULL;
	m_iListType  = TYPE_BLACKLIST;

	if( m_szUserName.IsEmpty() )
	{
		SetInActive();
	}
	else
	{
		SetActive();
	}
}

void UserInfoListWnd::iwm_create()
{
}

void UserInfoListWnd::iwm_show()
{
	if( m_iListType != TYPE_FRIENDLIST )
	{
		HideChildWnd( ID_RECV_CLOVER );
		HideChildWnd( ID_SEND_CLOVER );
	}
	else
	{
		HideChildWnd( ID_RECV_CLOVER );
		ShowChildWnd( ID_SEND_CLOVER );
	}
}

void UserInfoListWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_RECV_CLOVER:
		if( cmd == IOBN_BTNUP )
		{
			MessengerWnd *pWnd = dynamic_cast<MessengerWnd*>( g_GUIMgr.FindWnd(MESSENGER_WND) );
			if( pWnd )
			{
				ioButton *pBtn = dynamic_cast<ioButton*>( FindChildWnd(ID_RECV_CLOVER) );
				if( pBtn )
				{
					int iIndex = UserListWnd::ID_USER_LIST_1 - GetID();
					pWnd->SetMyCloverAni( iIndex, pBtn->GetDerivedPosX(), pBtn->GetDerivedPosY() );
					RecvClover();
				}
			}
		}
		break;
	case ID_SEND_CLOVER:
		if( cmd == IOBN_BTNUP )
		{
			MessengerWnd *pWnd = dynamic_cast<MessengerWnd*>( g_GUIMgr.FindWnd(MESSENGER_WND) );
			if( pWnd )
			{
				CloverButton *pBtn = dynamic_cast<CloverButton*>( FindChildWnd(ID_SEND_CLOVER) );
				if( pBtn )
				{
					if( SendClover() )
					{
						int iIndex = UserListWnd::ID_USER_LIST_1 - GetID();
						pWnd->SetGiftCloverAni( iIndex, pBtn->GetDerivedPosX(), pBtn->GetDerivedPosY() );
					}
				}
			}
		}
		break;
	}
}

float UserInfoListWnd::GetNameCut()
{
	return 140.0f;
}

void UserInfoListWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;
	if( m_szUserName.IsEmpty() ) return;

	if( m_iListType != TYPE_FRIENDLIST )
	{
		HideChildWnd( ID_RECV_CLOVER );
		HideChildWnd( ID_SEND_CLOVER );
		return;
	}

	ioClover *pClover = g_MyInfo.GetClover();
	if( pClover )
	{
		// Ŭ�ι� �ޱ�
		if( pClover->EnableRecvClover( m_szUserName ) )
			ShowChildWnd( ID_RECV_CLOVER );
		else
			HideChildWnd( ID_RECV_CLOVER );

		// Ŭ�ι� ������
		ioButton *pBtn = dynamic_cast<ioButton*>( FindChildWnd(ID_SEND_CLOVER) );
		if( pBtn )
		{
			if( pClover->EnableSendClover( m_szUserName ) )
				pBtn->SetActive();
			else
				pBtn->SetInActive();
		}
	}
}

void UserInfoListWnd::OnRender()
{
	ioButton::OnRender();	

	if( IsDisabled() )
		return;

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();
	
	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetAlignType( TAT_LEFT );
	g_FontMgr.SetBkColor( 0, 0, 0 );

	if( m_iListType == TYPE_FRIENDLIST || m_iListType == TYPE_GUILDUSERLIST )
	{
		if( m_bOver || m_bClicked )
		{
			if( m_bOnline || m_iListType == TYPE_GUILDUSERLIST )
				g_LevelMgr.RenderGrade( m_szUserName, iXPos + 4, iYPos + 2, m_iUserLevel, TEAM_PRIVATE_1, UI_RENDER_NORMAL_ADD_COLOR, RGB( 255, 255, 255 ) );			
			else
				g_LevelMgr.RenderMessengerOfflineMark( iXPos + 4, iYPos + 2, UI_RENDER_NORMAL_ADD_COLOR, RGB( 255, 255, 255 ) );
			g_FontMgr.SetTextColor( 255, 255, 255 );
		}
		else
		{
			if( m_bOnline ) 
			{
				g_LevelMgr.RenderGrade( m_szUserName, iXPos + 4, iYPos + 2, m_iUserLevel, TEAM_PRIVATE_1 );
				g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
			}
			else if( m_iListType == TYPE_GUILDUSERLIST )
			{
				g_LevelMgr.RenderGrade( m_szUserName, iXPos + 4, iYPos + 2, m_iUserLevel, TEAM_PRIVATE_1 );
				g_FontMgr.SetTextColor( TCT_DEFAULT_LIGHTGRAY );
			}
			else
			{
				g_LevelMgr.RenderMessengerOfflineMark( iXPos + 4, iYPos + 2 );
				g_FontMgr.SetTextColor( TCT_DEFAULT_LIGHTGRAY );
			}
		}	

		float iNameCut = GetNameCut();
		g_FontMgr.PrintTextWidthCut( iXPos + 23, iYPos + 3, FONT_SIZE_12, iNameCut, m_szUserName.c_str() );
		
		int iRealNameAfterPos = g_FontMgr.GetTextWidthCutSize( m_szUserName.c_str(), TS_NORMAL, FONT_SIZE_12, iNameCut );
		if( m_bOnline )
		{
			DWORD dwGuildIndex, dwGuildMark;
			g_UserInfoMgr.GetGuildInfo( m_szUserName, dwGuildIndex, dwGuildMark );			
			g_GuildMarkMgr.RenderSmallMark( dwGuildIndex, dwGuildMark, iXPos + 23 + iRealNameAfterPos + 3, iYPos + 4 );
			if( dwGuildIndex != 0 )
			{	
				iRealNameAfterPos += ( g_GuildMarkMgr.GetGuildSmallMarkSize( dwGuildIndex, dwGuildMark ) + 3 );
			}
		}

		int iFriendMakAfterPos = 0;
		if( m_iListType == TYPE_FRIENDLIST )
		{
			// ��ģ ��ũ
			if( m_bMyBestFriend && m_bOtherBestFriend )
			{
				if( m_bOnline && m_pCoupleBestFriendBlack )
				{
					m_pCoupleBestFriendBlack->Render( iXPos + 23 + iRealNameAfterPos + 3, iYPos + 3 );
					iFriendMakAfterPos += m_pCoupleBestFriendBlack->GetWidth() + iRealNameAfterPos + 26;
				}
				else if( m_pMyBestFriendGray )
				{
					m_pMyBestFriendGray->Render( iXPos + 23 + iRealNameAfterPos + 3, iYPos + 3 );
					iFriendMakAfterPos += m_pMyBestFriendGray->GetWidth() + iRealNameAfterPos + 26;
				}
			}
			else if( m_bMyBestFriend )
			{
				if( m_bOnline && m_pMyBestFriendBlack )
				{
					m_pMyBestFriendBlack->Render( iXPos + 23 + iRealNameAfterPos + 3, iYPos + 3 );
					iFriendMakAfterPos += m_pMyBestFriendBlack->GetWidth() + iRealNameAfterPos + 26;
				}
				else if( m_pMyBestFriendGray )
				{
					m_pMyBestFriendGray->Render( iXPos + 23 + iRealNameAfterPos + 3, iYPos + 3 );
					iFriendMakAfterPos += m_pMyBestFriendGray->GetWidth() + iRealNameAfterPos + 26;
				}
			}
			else if( m_bOtherBestFriend )
			{	
				if( m_bOnline && m_pOtherBestFriendBlack )
				{
					m_pOtherBestFriendBlack->Render( iXPos + 23 + iRealNameAfterPos + 3, iYPos + 3 );
					iFriendMakAfterPos += m_pOtherBestFriendBlack->GetWidth() + iRealNameAfterPos + 26;
				}
			}

			ioHashString szPosition;
			szPosition.Clear();
			switch( m_iUserPos )
			{
			case UP_TRAINING:
				szPosition = STR(7);
				break;
			case UP_BATTLE_ROOM:
				szPosition = STR(8);
				break;
			case UP_LADDER_TEAM:
				szPosition = STR(3);
				break;
			case UP_LOBBY:
				szPosition = STR(4);
				break;
			case UP_HEADQUARTERS:
				szPosition = STR(5);
				szPosition = STR(1);
				break;
			case UP_TOURNAMENT:
				szPosition = STR(1);
				break;
			case UP_SHUFFLE:
				szPosition = STR(2);
				break;
			case UP_GUILD_HQ:
				szPosition = STR(1);
				break;
			case UP_HOUSE:
				szPosition = STR(2);
				break;
			}

			if( !szPosition.IsEmpty() && m_bOnline )
			{
				int iXOffset = 0;
				if( iFriendMakAfterPos > 0 )
					iXOffset = iFriendMakAfterPos + 3;
				else
					iXOffset = iRealNameAfterPos + 26 + 3;

				if( g_BattleRoomMgr.IsBattleRoom() || g_LadderTeamMgr.IsLadderTeam() )
				{
					if( m_bOver || m_bClicked )
						g_FontMgr.SetTextColor( TCT_DEFAULT_WHITE );
					else if( CheckInvite( false ) )
						g_FontMgr.SetTextColor( 206, 78, 0 );
					else
						g_FontMgr.SetTextColor( TCT_DEFAULT_LIGHTGRAY );
				}
				else
				{
					if( m_bOver || m_bClicked )
						g_FontMgr.SetTextColor( TCT_DEFAULT_WHITE );
					else if( CheckMove( false ) )
						g_FontMgr.SetTextColor( 206, 78, 0 );
					else
						g_FontMgr.SetTextColor( TCT_DEFAULT_LIGHTGRAY );
				}

				g_FontMgr.PrintText( iXPos + iXOffset, iYPos + 3, FONT_SIZE_12, STR(6), szPosition.c_str() );
			}
		}
	}
	else if( m_iListType == TYPE_BLACKLIST )
	{
		if( m_bOver || m_bClicked )
		{
			g_FontMgr.SetTextColor( 255, 255, 255 );
		}
		else
		{
			g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
		}		

		if( m_pBlackMark )
			m_pBlackMark->Render( iXPos + 4, iYPos + 2 );
		g_FontMgr.PrintTextWidthCut( iXPos + 23, iYPos + 3, FONT_SIZE_12, 170.0f, m_szUserName.c_str() );
	}
}


bool UserInfoListWnd::CheckInvite( bool bAlarm )
{
	if( m_iUserPos == UP_BATTLE_ROOM || m_iUserPos == UP_LADDER_TEAM || m_iUserPos == UP_SHUFFLE || m_iUserPos == UP_TOURNAMENT )
	{
		if( bAlarm )
		{
			if( m_iUserPos == UP_BATTLE_ROOM )
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1), m_szUserName.c_str() );
			else if( m_iUserPos == UP_LADDER_TEAM )
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2), m_szUserName.c_str() );
			else if( m_iUserPos == UP_SHUFFLE )
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(14), m_szUserName.c_str() );
			else if( m_iUserPos == UP_TOURNAMENT )
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(15), m_szUserName.c_str() );
		}
		return false;
	}
	else if( g_BattleRoomMgr.IsBattleRoom() )
	{
		if( g_BattleRoomMgr.IsStartRoomEnterX() && g_BattleRoomMgr.IsBattleModePlaying() )
		{
			if( bAlarm )
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );

			return false;
		}
		else if( g_BattleRoomMgr.GetCurPlayer() >= g_BattleRoomMgr.GetMaxPlayer() )
		{
			if( bAlarm )
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );

			return false;
		}
		else if( g_BattleRoomMgr.IsHaveUser( m_szUserName ) )
		{
			if( bAlarm )
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );

			return false;
		}
		else if( g_BattleRoomMgr.IsSafetyLevelRoom() )
		{
			if( bAlarm )
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6) );

			return false;
		}
		else if( g_BattleRoomMgr.IsBroadcastModeType() && g_MyInfo.GetUserEventType() != USER_TYPE_BROADCAST_MBC )
		{
			if( bAlarm )
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(7) );

			return false;
		}
		else if( g_BattleRoomMgr.IsTournamentModeType() )
		{
			if( bAlarm )
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(8) );

			return false;
		}
	}
	else if( g_LadderTeamMgr.IsLadderTeam() )
	{
		if( g_LadderTeamMgr.IsLadderTeamFull() )
		{
			if( bAlarm )
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(9) );

			return false;
		}		
		else if( g_LadderTeamMgr.IsLadderBattleModePlaying() )
		{
			if( bAlarm )
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(10) );

			return false;
		}
		else if( g_LadderTeamMgr.IsSearching() && !g_LadderTeamMgr.IsLadderBattleModePlaying() )
		{
			if( bAlarm )
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(11) );

			return false;
		}
		else if( g_UserInfoMgr.GetCampPosition( m_szUserName ) != g_MyInfo.GetUserCampPos() )
		{
			if( bAlarm )
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(12) );

			return false;
		}
		else if( g_LadderTeamMgr.IsHaveUser( m_szUserName ) )
		{
			if( bAlarm )
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(13) );

			return false;
		}
	}	

	return true;
}

bool UserInfoListWnd::CheckMove( bool bAlarm )
{
	if( m_iUserPos == UP_LOBBY )
	{
		if( bAlarm )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		}
		return false;
	}
	else if( g_BattleRoomMgr.IsBattleRoom() )
	{
		if( bAlarm )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		}
		return false;
	}
	else if( g_LadderTeamMgr.IsLadderTeam() )
	{
		if( bAlarm )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		}
		return false;
	}
	return true;
}

bool UserInfoListWnd::SendClover()
{
	FriendNode *pNode = g_FriendManager.GetNode( m_szUserName );
	if( pNode )
	{
		if( !g_TCPSocketMgr.IsTestZone() )
		{
			if( g_App.IsMeDeveloper() || g_App.IsMeGameMaster() )
				return false;
		}

		char szBuf[MAX_PATH] = "";
		ioClover *pClover = g_MyInfo.GetClover();
		if( !pClover )
			return false;

		if( pClover->GetCurGiftCloverCount() <= 0 )
		{
			ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
			kPrinter[0].SetTextStyle( TS_NORMAL );
			kPrinter[0].SetBkColor( 0, 0, 0 );
			kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
			kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(1) );
			kPrinter[1].SetTextStyle( TS_NORMAL );
			kPrinter[1].SetBkColor( 0, 0, 0 );
			kPrinter[1].SetTextColor( TCT_DEFAULT_DARKGRAY );
			kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(2) );
			kPrinter[1].SetTextColor( TCT_DEFAULT_RED );
			kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(3), pClover->GetChargingTime() );
			kPrinter[1].SetTextColor( TCT_DEFAULT_DARKGRAY );
			kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(4), pClover->GetChargingCount() );

			g_GUIMgr.SetPrevMsgListBox( NULL, MB_OK, NULL, kPrinter );
		}
		else if( pClover->CheckFriendRegTime( m_szUserName ) )
		{
			ioHashString szRemainTime;
			pClover->GetFriendRegRemainTime( m_szUserName, szRemainTime );

			ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
			kPrinter[0].SetTextStyle( TS_NORMAL );
			kPrinter[0].SetBkColor( 0, 0, 0 );
			kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
			kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(5) );
			kPrinter[0].SetTextColor( TCT_DEFAULT_RED );
			if( pClover->GetAbusingTime() <= 60 )
				kPrinter[0].AddTextPiece( FONT_SIZE_12, STR(6), pClover->GetAbusingTime() );
			else
				kPrinter[0].AddTextPiece( FONT_SIZE_12, STR(7), pClover->GetAbusingTime()/60 );
			kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
			kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(8) );
			
			kPrinter[1].SetTextStyle( TS_NORMAL );
			kPrinter[1].SetBkColor( 0, 0, 0 );
			kPrinter[1].SetTextColor( TCT_DEFAULT_DARKGRAY );
			kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(9) );
			
			kPrinter[2].SetTextStyle( TS_NORMAL );
			kPrinter[2].SetBkColor( 0, 0, 0 );
			kPrinter[2].SetTextColor( TCT_DEFAULT_RED );
			kPrinter[2].AddTextPiece( FONT_SIZE_13, STR(10), szRemainTime.c_str() );
			kPrinter[2].SetTextColor( TCT_DEFAULT_DARKGRAY );
			kPrinter[2].AddTextPiece( FONT_SIZE_13, STR(11) );

			g_GUIMgr.SetPrevMsgListBox( NULL, MB_OK, NULL, kPrinter );
		}
		else if( pClover->CheckSendRemainTime( m_szUserName ) )
		{
			ioHashString szRemainTime;
			pClover->GetSendRemainTime( m_szUserName, szRemainTime );

			ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
			kPrinter[0].SetTextStyle( TS_NORMAL );
			kPrinter[0].SetBkColor( 0, 0, 0 );
			kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
			kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(12) );

			kPrinter[1].SetTextStyle( TS_NORMAL );
			kPrinter[1].SetBkColor( 0, 0, 0 );
			kPrinter[1].SetTextColor( TCT_DEFAULT_RED );
			kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(13), szRemainTime.c_str() );
			kPrinter[1].SetTextColor( TCT_DEFAULT_DARKGRAY );
			kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(14) );

			g_GUIMgr.SetPrevMsgListBox( NULL, MB_OK, NULL, kPrinter );
		}
		else
		{
			// �ӽ÷� Ŭ���̾�Ʈ�� ���� �������ش�. ( �����κ��� ������ ������ ���� ���� ������ ����ȴ�. )
			int iGiftClover = pClover->GetCurGiftCloverCount();
			iGiftClover--;
			iGiftClover = max( iGiftClover, 0 );
			pClover->SetCurGiftCloverCount( iGiftClover );

			pNode->SetCloverSendSec( pClover->GetSendTimeGap() * 60 );

			// Send Packet..
			SP2Packet kPacket( CTPK_GIFTCLOVER_SEND );
			kPacket << pNode->GetUserIndex();
			TCPNetwork::SendToServer( kPacket );

			TCPNetwork::MouseBusy( true );
			return true;
		}
	}

	return false;
}

void UserInfoListWnd::RecvClover()
{
	FriendNode *pNode = g_FriendManager.GetNode( m_szUserName );
	if( pNode )
	{
		if( !g_TCPSocketMgr.IsTestZone() )
		{
			if( g_App.IsMeDeveloper() || g_App.IsMeGameMaster() )
				return;
		}

		ioClover *pClover = g_MyInfo.GetClover();
		// �ӽ÷� Ŭ���̾�Ʈ�� ���� �������ش�. ( �����κ��� ������ ������ ���� ���� ������ ����ȴ�. )
		if( pClover )
		{
			pNode->SetCloverRecvSec( pClover->GetRecvTimeGap() * 60 );
		}

		// receive Packet..
		SP2Packet kPacket( CTPK_CLOVER_RECEIVE );
		kPacket << pNode->GetUserIndex();
		TCPNetwork::SendToServer( kPacket );

		TCPNetwork::MouseBusy( true );
	}
}
//////////////////////////////////////////////////////////////////////////
bool UserListWnd::sg_bUserListUpdate = false;
UserListWnd::UserListWnd()
{
	m_iTabID   = ID_FRIEND_LIST;
	m_iCurPage = m_iMaxPage = 0;	
	m_dwChangePageTimer = 0;
	m_dwUserPosRefreshTimer = 0;
	m_pPlayStage = NULL;
}

UserListWnd::~UserListWnd()
{
}

void UserListWnd::iwm_create()
{
	g_GUIMgr.AddWnd( "XML/clovertooltip.xml", this );
}

void UserListWnd::iwm_show()
{	
	m_dwChangePageTimer     = FRAMEGETTIME();
	m_dwUserPosRefreshTimer = 0;
	CheckButton( ID_FRIEND_LIST );	

	ioWnd *pFriendBtn = FindChildWnd( ID_FRIEND_LIST );
	if( pFriendBtn )
	{
		char szTitle[MAX_PATH] = "";
		SafeSprintf( szTitle, sizeof( szTitle ), STR(1), g_FriendManager.GetOnlineNodeSize(), g_FriendManager.GetNodeSize() );
		pFriendBtn->SetTitleText( szTitle );
	}

	ioWnd *pGuildBtn = FindChildWnd( ID_GUILD_LIST );
	if( pGuildBtn )
	{
		int iOnlineUserCnt = 0;
		int iMaxGuildUserCnt= 0;
		ioGuildData *pGuildData = g_GuildInfo.GetGuildData( g_GuildInfo.GetGuildIndex() );
		if( pGuildData )
		{
			pGuildBtn->SetActive();
			iMaxGuildUserCnt = (int)pGuildData->GetGuildJoinUser();
			for(int i = 0;i < iMaxGuildUserCnt;i++)
			{
				UserListInfo kMUI;
				pGuildData->GetUserInfo( i, kMUI.m_szUserID, kMUI.m_iGradeLevel, kMUI.m_iUserPos, kMUI.m_bOnline );
				if( !kMUI.m_szUserID.IsEmpty() && g_MyInfo.GetPublicID() != kMUI.m_szUserID )
				{
					if( kMUI.m_bOnline )
						iOnlineUserCnt++;
				}						
			}
		}
		else
		{
			pGuildBtn->SetInActive();
		}
		char szTitle[MAX_PATH] = "";
		SafeSprintf( szTitle, sizeof( szTitle ), STR(2), iOnlineUserCnt, max( 0, iMaxGuildUserCnt - 1 ) );
		pGuildBtn->SetTitleText( szTitle );
	}
}

void UserListWnd::iwm_hide()
{	
}

void UserListWnd::UpdateUserList()
{
	m_UserList.clear();
	switch( m_iTabID )
	{
	case ID_FRIEND_LIST:
		{
			// ģ�� ���
			int iOnlineUserCnt = 0;
			for(int i = 0;i < g_FriendManager.GetNodeSize();i++)
			{
				FriendNode *pNode = g_FriendManager.GetNode( i );
				if( pNode )
				{
					UserListInfo kMUI;
					kMUI.m_szUserID	   = pNode->GetName();
					kMUI.m_iGradeLevel = pNode->GetGradeLevel();
					kMUI.m_iUserPos    = pNode->GetUserPos();
					kMUI.m_bOnline     = pNode->IsOnline();
					kMUI.m_bOtherBestFriend = pNode->IsOtherBF();
					kMUI.m_iRecvWaitClover = pNode->GetCloverRecvWaitCount();
					kMUI.m_iRecvClover     = pNode->GetCloverRecvCount();
					kMUI.m_iSendClover     = pNode->GetCloverSendCount();
					if( g_FriendManager.GetBestFriendState( kMUI.m_szUserID ) == BFT_SET )
					{   
						kMUI.m_bMyBestFriend = true;
					}
					m_UserList.push_back( kMUI );

					if( kMUI.m_bOnline )
						iOnlineUserCnt++;
				}
			}			
			std::sort( m_UserList.begin(), m_UserList.end(), UserListInfoSort() );	
			m_iMaxPage = max( 0, (int)m_UserList.size() - 1 ) / MAX_LIST_SIZE;
			UpdateSetPage( m_iCurPage );

			ioWnd *pFriendBtn = FindChildWnd( ID_FRIEND_LIST );
			if( pFriendBtn )
			{
				char szTitle[MAX_PATH] = "";
				SafeSprintf( szTitle, sizeof( szTitle ), STR(1), iOnlineUserCnt, (int)m_UserList.size() );
				pFriendBtn->SetTitleText( szTitle );
			}
		}
		break;
	case ID_GUILD_LIST:
		{
			// ���� ���
			int iOnlineUserCnt = 0;
			if( g_GuildInfo.IsGuild() )
			{
				ioGuildData *pGuildData = g_GuildInfo.GetGuildData( g_GuildInfo.GetGuildIndex() );
				if( pGuildData )
				{
					for(int i = 0;i < (int)pGuildData->GetGuildJoinUser();i++)
					{
						UserListInfo kMUI;
						pGuildData->GetUserInfo( i, kMUI.m_szUserID, kMUI.m_iGradeLevel, kMUI.m_iUserPos, kMUI.m_bOnline );
						if( !kMUI.m_szUserID.IsEmpty() && g_MyInfo.GetPublicID() != kMUI.m_szUserID )
						{
							m_UserList.push_back( kMUI );
							if( kMUI.m_bOnline )
								iOnlineUserCnt++;
						}						
					}
				}
				std::sort( m_UserList.begin(), m_UserList.end(), UserListInfoSort() );	
			}
			m_iMaxPage = max( 0, (int)m_UserList.size() - 1 ) / MAX_LIST_SIZE;
			UpdateSetPage( m_iCurPage );

			ioWnd *pGuildBtn = FindChildWnd( ID_GUILD_LIST );
			if( pGuildBtn )
			{
				char szTitle[MAX_PATH] = "";
				SafeSprintf( szTitle, sizeof( szTitle ), STR(2), iOnlineUserCnt, (int)m_UserList.size() );
				pGuildBtn->SetTitleText( szTitle );
			}
		}
		break;
	case ID_BLACK_LIST:
		{
			for(int i = 0;i < g_BlackListManager.GetNodeSize();i++)
			{		
				ioHashString *pString = g_BlackListManager.GetNode( i );
				if( pString )
				{
					UserListInfo kMUI;
					kMUI.m_szUserID = *pString;
					m_UserList.push_back( kMUI );
				}
			}
			m_iMaxPage = max( 0, (int)m_UserList.size() - 1 ) / MAX_LIST_SIZE;
			UpdateSetPage( m_iCurPage );
		}
		break;
	}
}

void UserListWnd::UpdateSetPage( int iNewPage )
{
	int iPrevPage = m_iCurPage;
	m_iCurPage = iNewPage;
	switch( m_iTabID )
	{
	case ID_FRIEND_LIST:
		{
			int iStartArray = m_iCurPage * MAX_LIST_SIZE;
			for(int i = ID_USER_LIST_1;i < ID_USER_LIST_9+1;i++,iStartArray++)
			{
				UserInfoListWnd *pWnd = dynamic_cast<UserInfoListWnd*>( FindChildWnd( i ) );
				if( pWnd )
				{
					if( iStartArray < (int)m_UserList.size() )
					{
						UserListInfo &kMUI = m_UserList[iStartArray];
						pWnd->ShowWnd();
						pWnd->SetFriendUserInfo( kMUI.m_szUserID, kMUI.m_bOnline, kMUI.m_iUserPos, kMUI.m_iGradeLevel,
												 kMUI.m_bMyBestFriend, kMUI.m_bOtherBestFriend, m_pPlayStage );
					}
					else
						pWnd->HideWnd();
				}
			}			
		}
		break;
	case ID_GUILD_LIST:
		{
			int iStartArray = m_iCurPage * MAX_LIST_SIZE;
			for(int i = ID_USER_LIST_1;i < ID_USER_LIST_9+1;i++,iStartArray++)
			{
				UserInfoListWnd *pWnd = dynamic_cast<UserInfoListWnd*>( FindChildWnd( i ) );
				if( pWnd )
				{
					if( iStartArray < (int)m_UserList.size() )
					{
						UserListInfo &kMUI = m_UserList[iStartArray];
						pWnd->ShowWnd();
						pWnd->SetGuildUserInfo( kMUI.m_szUserID, kMUI.m_bOnline, kMUI.m_iUserPos, kMUI.m_iGradeLevel, m_pPlayStage );
					}
					else
						pWnd->HideWnd();
				}
			}			
		}
		break;
	case ID_BLACK_LIST:
		{
			int iStartArray = m_iCurPage * MAX_LIST_SIZE;
			for(int i = ID_USER_LIST_1;i < ID_USER_LIST_9+1;i++,iStartArray++)
			{
				UserInfoListWnd *pWnd = dynamic_cast<UserInfoListWnd*>( FindChildWnd( i ) );
				if( pWnd )
				{
					if( iStartArray < (int)m_UserList.size() )
					{
						UserListInfo &kMUI = m_UserList[iStartArray];
						pWnd->ShowWnd();
						pWnd->SetBlackListUserInfo( kMUI.m_szUserID, true ); // black list ������ online
					}
					else
						pWnd->HideWnd();
				}
			}
		}
		break;
	}

	if( iPrevPage != m_iCurPage )
	{
		m_dwChangePageTimer = FRAMEGETTIME();
		m_dwUserPosRefreshTimer = 0;
	}
}

void UserListWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_FRIEND_LIST:
	case ID_GUILD_LIST:
	case ID_BLACK_LIST:	
		if( cmd == IOBN_BTNDOWN )
		{
			CheckButton( dwID );
		}
		break;
	case ID_FRIEND_INSERT:
		if( cmd == IOBN_BTNUP )
		{
			g_GUIMgr.ShowWnd( FRIEND_APPLICATION_WND );
		}
		break;
	case ID_GUILD_INFO:
		if( cmd == IOBN_BTNUP )
		{
			if( g_GuildInfo.GetGuildIndex() != 0 )
			{
				GuildMainWnd *pGuildMainWnd = dynamic_cast<GuildMainWnd *>(g_GUIMgr.FindWnd( GUILD_MAIN_WND ));
				if( pGuildMainWnd )
					pGuildMainWnd->SetGuildIndex( g_GuildInfo.GetGuildIndex() );
			}
		}
		break;
	case ID_GUILD_HOME_PAGE:
		if( cmd == IOBN_BTNUP )
		{
			if( g_GuildInfo.GetGuildIndex() != 0 )
			{
				g_App.OpenGuildHomePage( g_GuildInfo.GetGuildIndex() );
			}
		}
		break;
	case ID_BLACK_INSERT:
		if( cmd == IOBN_BTNUP )
		{
			g_GUIMgr.ShowWnd( BLACKLIST_INSERT_WND );
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
	case ID_USER_LIST_1:
	case ID_USER_LIST_2:
	case ID_USER_LIST_3:
	case ID_USER_LIST_4:
	case ID_USER_LIST_5:
	case ID_USER_LIST_6:
	case ID_USER_LIST_7:
	case ID_USER_LIST_8:
	case ID_USER_LIST_9:
		if( cmd == IOBN_BTNUP )
		{
			if( cmd == IOBN_BTNUP )
			{
				UserInfoListWnd *pUserInfoWnd = dynamic_cast<UserInfoListWnd*>(pWnd);
				if( pUserInfoWnd )
				{
					UserInfoListOptionWnd *pOptionWnd = dynamic_cast<UserInfoListOptionWnd*>( FindChildWnd( ID_OPTION_WND ) );
					if( pOptionWnd )
					{
						pOptionWnd->SetOptionInfo( m_pPlayStage, pUserInfoWnd->GetUserID(), pUserInfoWnd->GetUserLevel(), pUserInfoWnd->IsBestFriend(), pUserInfoWnd->GetUserPosition(), pUserInfoWnd->GetListType(), pUserInfoWnd->IsOnline(), pUserInfoWnd->CheckMove( false ), pUserInfoWnd->CheckInvite( false ) );
					}
				}
			}
		}
		break;
	}
}

bool UserListWnd::iwm_esc()
{
	if( !IsShow() )
		return false;

	UserInfoListOptionWnd *pOptionWnd = dynamic_cast<UserInfoListOptionWnd *>( FindChildWnd( ID_OPTION_WND ) );
	if( pOptionWnd && pOptionWnd->IsShow() )
	{
		return pOptionWnd->iwm_esc();
	}
	return false;
}

void UserListWnd::CheckButton( DWORD dwID )
{
	m_iTabID = dwID;
	CheckRadioButton( ID_FRIEND_LIST, ID_BLACK_LIST, m_iTabID );

	HideChildWnd( ID_FRIEND_INSERT );
	HideChildWnd( ID_BESTFRIEND_INSERT );
	HideChildWnd( ID_GUILD_INFO );
	HideChildWnd( ID_GUILD_HOME_PAGE );
	HideChildWnd( ID_BLACK_INSERT );

	m_iCurPage = 0;
	switch( m_iTabID )
	{
	case ID_FRIEND_LIST:
		UpdateUserList();
		ShowChildWnd( ID_FRIEND_INSERT );
		ShowChildWnd( ID_BESTFRIEND_INSERT );
		break;
	case ID_GUILD_LIST:
		UpdateUserList();
		ShowChildWnd( ID_GUILD_INFO );
		ShowChildWnd( ID_GUILD_HOME_PAGE );
		break;
	case ID_BLACK_LIST:
		UpdateUserList();
		ShowChildWnd( ID_BLACK_INSERT );
		break;
	}
}

void UserListWnd::InviteResult( int iResult, ioHashString szInvitedName )
{
	switch( iResult )
	{
	case CHANNEL_INVITE_NOT_USER:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		break;
	case CHANNEL_INVITE_USER_NOT_CHANNEL:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		break;
	case CHANNEL_INVITE_USER_ALREADY_CHANNEL:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		break;
	}
}

void UserListWnd::ChannelCreate( int iResult, ioHashString szManToManName )
{
	g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
}

void UserListWnd::SetPlayStage( ioPlayStage *pPlayStage )
{
	m_pPlayStage = pPlayStage;
}

bool UserListWnd::IsChatBlock()
{
	if( !IsShow() )
		return false;
	return false;
}

void UserListWnd::SendUserPosRefresh()
{
	if( m_iTabID != ID_FRIEND_LIST && m_iTabID != ID_GUILD_LIST ) return;
    if( !g_FriendManager.IsLoadingComplete() ) return;

	int i = 0;
	ioHashStringVec vString;
	for(i = ID_USER_LIST_1;i < ID_USER_LIST_9+1;i++)
	{
		UserInfoListWnd *pUserListWnd = dynamic_cast<UserInfoListWnd*>( FindChildWnd( i ) );
		if( !pUserListWnd ) continue;
		if( !pUserListWnd->IsShow() ) continue;
		if( !pUserListWnd->IsOnline() ) continue;
		if( pUserListWnd->GetUserName().IsEmpty() ) continue;

		vString.push_back( pUserListWnd->GetUserName() );
	}

	int iUserSize = vString.size();
	if( iUserSize != 0 )
	{
		SP2Packet kPacket( CTPK_USER_POS_REFRESH );
		kPacket << iUserSize;
		for(i = 0;i < iUserSize;i++)
			kPacket << vString[i];
		TCPNetwork::SendToServer( kPacket );
		vString.clear();
	}					
}

void UserListWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() )	return;

	switch( m_iTabID )
	{
	case ID_FRIEND_LIST:
	case ID_GUILD_LIST:
		if( sg_bUserListUpdate )
		{
			sg_bUserListUpdate = false;
			UpdateUserList();					
		}
		else if( m_dwChangePageTimer != 0 )
		{
			if( FRAMEGETTIME() - m_dwChangePageTimer > 1000 )
			{
				SendUserPosRefresh();
				m_dwChangePageTimer = 0;
				m_dwUserPosRefreshTimer = FRAMEGETTIME();
			}			
		}
		else if( m_dwUserPosRefreshTimer != 0 )
		{
			if( FRAMEGETTIME() - m_dwUserPosRefreshTimer > 15000 )
			{
				SendUserPosRefresh();
				m_dwUserPosRefreshTimer = FRAMEGETTIME();
			}
		}
		break;
	case ID_BLACK_LIST:
		if( sg_bUserListUpdate )
		{
			sg_bUserListUpdate = false;
			UpdateUserList();
		}
		break;
	}
}

void UserListWnd::OnRender()
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
	kPrinter.PrintFullText( iXPos + 161, iYPos + 226, TAT_CENTER );	
}

void UserListWnd::OnInsertBestFriend( const ioHashString & szUserName )
{
	if( g_App.IsMouseBusy() )
		return;

	if( !g_FriendManager.IsLoadingComplete() )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return;
	}

	if( szUserName.IsEmpty() )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return;
	}

	FriendNode *pFriend = g_FriendManager.GetNode( szUserName );
	if( pFriend == NULL )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		return;
	}

	if( g_FriendManager.GetBestFriendState( pFriend->GetUserIndex() ) == BFT_SET )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4), pFriend->GetName().c_str() );
		return;
	}

 	SP2Packet kPacket( CTPK_INSERT_BESTFRIEND );
 	kPacket << pFriend->GetUserIndex();
 	TCPNetwork::SendToServer( kPacket );
 	TCPNetwork::MouseBusy( true );
}

bool UserListWnd::CheckDeleteBestFriend( const ioHashString & szUserName )
{
	if( g_App.IsMouseBusy() )
		return false;

	if( !g_FriendManager.IsLoadingComplete() )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return false;
	}

	FriendNode *pFriendNode = g_FriendManager.GetNode( szUserName );
	if( pFriendNode == NULL )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2), szUserName.c_str() );
		return false;
	}

	if( g_FriendManager.GetBestFriendState( pFriendNode->GetUserIndex() ) != BFT_SET )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3), pFriendNode->GetName().c_str() );
		return false;
	}

	return true;
}

void UserListWnd::_OnDeleteBestFriend( const ioHashString & szUserName )
{
	if( g_App.IsMouseBusy() )
		return;

	if( !g_FriendManager.IsLoadingComplete() )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return;
	}

	FriendNode *pFriendNode = g_FriendManager.GetNode( szUserName );
	if( pFriendNode == NULL )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2), szUserName.c_str() );
		return;
	}

	if( g_FriendManager.GetBestFriendState( pFriendNode->GetUserIndex() ) != BFT_SET )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3), pFriendNode->GetName().c_str() );
		return;
	}

	SP2Packet kPacket( CTPK_DISMISS_BESTFRIEND );
	kPacket << pFriendNode->GetUserIndex() << pFriendNode->GetName();
	TCPNetwork::SendToServer( kPacket );
	TCPNetwork::MouseBusy( true );
}
//////////////////////////////////////////////////////////////////////////
LobbyUserListBtn::LobbyUserListBtn()
{
	m_pMyBestFriendBlack	= NULL;
	m_pMyBestFriendGray		= NULL;
	m_pOtherBestFriendBlack = NULL;
	m_pOtherBestFriendGray	= NULL;
	m_pCoupleBestFriendBlack= NULL;
	m_pCoupleBestFriendGray	= NULL;

	m_dwParentID = 0;
	m_bGuild = false;
}

LobbyUserListBtn::~LobbyUserListBtn()
{
	SAFEDELETE( m_pMyBestFriendBlack );
	SAFEDELETE( m_pMyBestFriendGray );
	SAFEDELETE( m_pOtherBestFriendBlack );
	SAFEDELETE( m_pOtherBestFriendGray );
	SAFEDELETE( m_pCoupleBestFriendBlack );
	SAFEDELETE( m_pCoupleBestFriendGray );
}

void LobbyUserListBtn::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "MyBestFriendBlack" )
	{
		SAFEDELETE( m_pMyBestFriendBlack );
		m_pMyBestFriendBlack = pImage;
	}
	else if( szType == "MyBestFriendGray" )
	{
		SAFEDELETE( m_pMyBestFriendGray );
		m_pMyBestFriendGray = pImage;
	}
	else if( szType == "OtherBestFriendBlack" )
	{
		SAFEDELETE( m_pOtherBestFriendBlack );
		m_pOtherBestFriendBlack = pImage;
	}
	else if( szType == "OtherBestFriendGray" )
	{
		SAFEDELETE( m_pOtherBestFriendGray );
		m_pOtherBestFriendGray = pImage;
	}
	else if( szType == "CoupleBestFriendBlack" )
	{
		SAFEDELETE( m_pCoupleBestFriendBlack );
		m_pCoupleBestFriendBlack = pImage;
	}
	else if( szType == "CoupleBestFriendGray" )
	{
		SAFEDELETE( m_pCoupleBestFriendGray );
		m_pCoupleBestFriendGray = pImage;
	}
	else
	{
		ioButton::AddRenderImage( szType, pImage );
	}
}

void LobbyUserListBtn::SetLobbyUserInfo( UserListInfo &rkUserInfo, bool bGuild )
{
	m_bGuild   = bGuild;
	m_UserInfo = rkUserInfo;
	if( GetParent() )
		m_dwParentID = GetParent()->GetID();

	if( m_UserInfo.m_szUserID.IsEmpty() )
		SetInActive();
	else
		SetActive();
}

float LobbyUserListBtn::GetNameCut()
{
	if( m_UserInfo.m_bMyBestFriend && m_UserInfo.m_bOtherBestFriend )
		return 51.0f;
	else if( m_UserInfo.m_bMyBestFriend || m_UserInfo.m_bOtherBestFriend )
		return 67.0f;
	return 95.0f;
}

void LobbyUserListBtn::OnRender()
{
	ioButton::OnRender();

	if( !m_UserInfo.m_szUserID.IsEmpty() )
	{
		int iXPos = GetDerivedPosX();
		int iYPos = GetDerivedPosY();

		g_FontMgr.SetTextStyle( TS_NORMAL );
		g_FontMgr.SetAlignType( TAT_LEFT );
		g_FontMgr.SetBkColor( 0, 0, 0 );

		DWORD dwColorType1 = 0xFF3A3A3A;       //���̵� ��
		DWORD dwColorType2 = 0xFF808080;       //�κ� ��
		DWORD dwColorType3 = 0xFF808080;       //���� ��
		DWORD dwColorType4 = 0xFFCE4E00;       //����&����&���� �� 
		if( m_bOver || m_bClicked )
		{
			if( m_UserInfo.m_bOnline || m_bGuild )
				g_LevelMgr.RenderGrade( m_UserInfo.m_szUserID, iXPos + 6, iYPos + 2, m_UserInfo.m_iGradeLevel, TEAM_PRIVATE_1, UI_RENDER_NORMAL_ADD_COLOR, RGB( 255, 255, 255 ) );
			else
				g_LevelMgr.RenderLobbyOfflineMark( iXPos + 6, iYPos + 2, UI_RENDER_NORMAL_ADD_COLOR, RGB( 255, 255, 255 ) );

			dwColorType1 = dwColorType2 = dwColorType3 = dwColorType4 = 0xFFFFFFFF;
		}
		else
		{
			if( m_UserInfo.m_bOnline )
			{
				g_LevelMgr.RenderGrade( m_UserInfo.m_szUserID, iXPos + 6, iYPos + 2, m_UserInfo.m_iGradeLevel, TEAM_PRIVATE_1 );
			}
			else if( m_bGuild )
			{	
				g_LevelMgr.RenderGrade( m_UserInfo.m_szUserID, iXPos + 6, iYPos + 2, m_UserInfo.m_iGradeLevel, TEAM_PRIVATE_1, UI_RENDER_GRAY );
			}
			else
			{
				g_LevelMgr.RenderLobbyOfflineMark( iXPos + 6, iYPos + 2 );
			}
		}

		if( m_UserInfo.m_bOnline )
		{
			g_FontMgr.SetTextColor( dwColorType1 );
			g_FontMgr.PrintTextWidthCut( iXPos + 25, iYPos + 3, FONT_SIZE_12, GetNameCut(), m_UserInfo.m_szUserID.c_str() );

			DWORD dwGuildIndex, dwGuildMark;
			g_UserInfoMgr.GetGuildInfo( m_UserInfo.m_szUserID, dwGuildIndex, dwGuildMark );
			int iRealNameSize = g_FontMgr.GetTextWidthCutSize( m_UserInfo.m_szUserID.c_str(), TS_NORMAL, FONT_SIZE_13, GetNameCut() );
			g_GuildMarkMgr.RenderSmallMark( dwGuildIndex, dwGuildMark, iXPos + 25 + iRealNameSize + 3, iYPos + 4 );

			if( dwGuildIndex != 0 )
			{	
				iRealNameSize += ( g_GuildMarkMgr.GetGuildSmallMarkSize( dwGuildIndex, dwGuildMark ) + 3 );
			}

			if( m_UserInfo.m_bMyBestFriend && m_UserInfo.m_bOtherBestFriend )
			{
				if( m_pCoupleBestFriendBlack )
				{
					m_pCoupleBestFriendBlack->Render( iXPos + 25 + iRealNameSize + 3, iYPos + 3 );
				}
			}
			else if( m_UserInfo.m_bMyBestFriend )
			{
				if( m_pMyBestFriendBlack )
				{
					m_pMyBestFriendBlack->Render( iXPos + 25 + iRealNameSize + 3, iYPos + 3 );
				}
			}
			else if( m_UserInfo.m_bOtherBestFriend )
			{	
				if( m_pOtherBestFriendBlack )
				{
					m_pOtherBestFriendBlack->Render( iXPos + 25 + iRealNameSize + 3, iYPos + 3 );
				}
			}

			if( m_dwParentID != LobbyUserListWnd::ID_LOBBY_WND )
			{
				g_FontMgr.SetAlignType( TAT_RIGHT );
				switch( m_UserInfo.m_iUserPos )
				{
				case UP_BATTLE_ROOM:
					g_FontMgr.SetTextColor( dwColorType4 );
					g_FontMgr.PrintText( iXPos + 165, iYPos + 3, FONT_SIZE_12, STR(1) );
					break;
				case UP_LADDER_TEAM:
					g_FontMgr.SetTextColor( dwColorType4 );
					g_FontMgr.PrintText( iXPos + 165, iYPos + 3, FONT_SIZE_12, STR(2) );
					break;
				case UP_TRAINING:
					g_FontMgr.SetTextColor( dwColorType4 );
					g_FontMgr.PrintText( iXPos + 165, iYPos + 3, FONT_SIZE_12, STR(3) );
					break;
				case UP_LOBBY:
					g_FontMgr.SetTextColor( dwColorType2 );
					g_FontMgr.PrintText( iXPos + 165, iYPos + 3, FONT_SIZE_12, STR(4) );
					break;
				case UP_HEADQUARTERS:
					g_FontMgr.SetTextColor( dwColorType4 );
					g_FontMgr.PrintText( iXPos + 165, iYPos + 3, FONT_SIZE_12, STR(1) );
					g_FontMgr.PrintText( iXPos + 165, iYPos + 3, FONT_SIZE_12, STR(1) );
					break;
				case UP_SHUFFLE:
					g_FontMgr.SetTextColor( dwColorType4 );
					g_FontMgr.PrintText( iXPos + 165, iYPos + 3, FONT_SIZE_12, STR(7) );
					break;
				case UP_TOURNAMENT:
					g_FontMgr.SetTextColor( dwColorType4 );
					g_FontMgr.PrintText( iXPos + 165, iYPos + 3, FONT_SIZE_12, STR(8) );
					break;
				case UP_GUILD_HQ:
					g_FontMgr.SetTextColor( dwColorType4 );
					g_FontMgr.PrintText( iXPos + 165, iYPos + 3, FONT_SIZE_12, STR(2) );
					break;
				case UP_HOUSE:
					g_FontMgr.SetTextColor( dwColorType4 );
					g_FontMgr.PrintText( iXPos + 165, iYPos + 3, FONT_SIZE_12, STR(3) );
					break;
				}
			}
		}		
		else
		{
			g_FontMgr.SetTextColor( dwColorType3 );
			g_FontMgr.PrintTextWidthCut( iXPos + 25, iYPos + 3, FONT_SIZE_12, GetNameCut(), m_UserInfo.m_szUserID.c_str() );

			int iRealNameSize = g_FontMgr.GetTextWidthCutSize( m_UserInfo.m_szUserID.c_str(), TS_NORMAL, FONT_SIZE_13, GetNameCut() );
			if( m_UserInfo.m_bMyBestFriend )
			{
				if( m_pMyBestFriendGray )
				{
					m_pMyBestFriendGray->Render( iXPos + 25 + iRealNameSize + 3, iYPos + 3 );
				}
			}

			g_FontMgr.SetAlignType( TAT_RIGHT );
			g_FontMgr.SetTextColor( dwColorType3 );
			g_FontMgr.PrintText( iXPos + 165, iYPos + 3, FONT_SIZE_12, STR(6) );
		}
	}
}

void LobbyUserListBtn::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;
	if( !m_UserInfo.m_bOnline ) return;
	if( m_UserInfo.m_szUserID.IsEmpty() ) return;
	if( !g_App.IsMouseRender() ) return;

	if( m_bOver && !m_bClicked )
	{
		ioLSMouse *pMouse = g_App.GetMouse();
		if( pMouse && pMouse->IsMouseShowing() )
			pMouse->SetToolTipID( m_UserInfo.m_szUserID, UserInfoToolTip::TT_GLOBAL );
	}	
}
//////////////////////////////////////////////////////////////////////////
bool LobbyFriendListWnd::sg_bFriendListUpdate = false;
LobbyFriendListWnd::LobbyFriendListWnd()
{
	m_iCurPage = m_iMaxPage = 0;
	m_dwChangePageTimer = 0;
	m_dwUserPosRefreshTimer = 0;
	m_dwFriendManual = 0;
}

LobbyFriendListWnd::~LobbyFriendListWnd()
{

}

void LobbyFriendListWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	m_dwFriendManual = xElement.GetIntAttribute( "FriendManual" );
}

void LobbyFriendListWnd::iwm_show()
{
	m_dwChangePageTimer     = FRAMEGETTIME();
	m_dwUserPosRefreshTimer = 0;
	UpdateFriendList( 0 );
}

void LobbyFriendListWnd::iwm_hide()
{

}

void LobbyFriendListWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_LEFT_BTN:
		if( cmd == IOBN_BTNUP )
		{
			UpdateFriendList( m_iCurPage - 1 );
		}
		break;
	case ID_RIGHT_BTN:
		if( cmd == IOBN_BTNUP )
		{
			UpdateFriendList( m_iCurPage + 1 );
		}
		break;
	case ID_HELP_BTN:
		if( cmd == IOBN_BTNUP )
		{
			ManualListWnd *pManualWnd = dynamic_cast<ManualListWnd*> (g_GUIMgr.FindWnd( MANUAL_LIST_WND ) );
			if( pManualWnd )
				pManualWnd->ShowWndByCurManual( m_dwFriendManual );
		}
		break;
	case ID_LIST_1:
	case ID_LIST_2:
	case ID_LIST_3:
	case ID_LIST_4:
	case ID_LIST_5:
		if( cmd == IOBN_BTNUP )
		{
			LobbyUserListBtn *pUserInfo = dynamic_cast<LobbyUserListBtn*>( pWnd );
			if( pUserInfo )
			{
				LobbyUserListOptionWnd *pOptionWnd = dynamic_cast<LobbyUserListOptionWnd*>( FindChildWnd( ID_OPTION_WND ) );
				if( pOptionWnd )
				{
					pOptionWnd->SetOptionInfo( pUserInfo->GetUserInfo() );
				}
			}
		}
		break;
	}
}

void LobbyFriendListWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;

	if( sg_bFriendListUpdate )
	{
		UpdateFriendList( m_iCurPage );
	} 
	else if( m_dwChangePageTimer != 0 )
	{
		if( FRAMEGETTIME() - m_dwChangePageTimer > 1000 )
		{
			if( g_App.IsMouseRender() )
			{
				SendUserPosRefresh();
				m_dwChangePageTimer = 0;
				m_dwUserPosRefreshTimer = FRAMEGETTIME();
			}
		}			
	}
	else if( m_dwUserPosRefreshTimer != 0 )
	{
		if( FRAMEGETTIME() - m_dwUserPosRefreshTimer > 15000 )
		{
			if( g_App.IsMouseRender() )
			{
				SendUserPosRefresh();
				m_dwUserPosRefreshTimer = FRAMEGETTIME();
			}
		}
	}
}

void LobbyFriendListWnd::SendUserPosRefresh()
{
	if( !g_FriendManager.IsLoadingComplete() ) return;

	int i = 0;
	ioHashStringVec vString;
	for(i = 0;i < MAX_PAGE_LIST;i++)
	{
		LobbyUserListBtn *pWnd = (LobbyUserListBtn*)FindChildWnd( ID_LIST_1 + i );
		if( !pWnd ) continue;
		if( !pWnd->IsShow() ) continue;

		UserListInfo &rkInfo = pWnd->GetUserInfo();
		if( !rkInfo.m_bOnline ) continue;
		if( rkInfo.m_szUserID.IsEmpty() ) continue;


		vString.push_back( rkInfo.m_szUserID );
	}

	int iUserSize = vString.size();
	if( iUserSize != 0 )
	{
		SP2Packet kPacket( CTPK_USER_POS_REFRESH );
		kPacket << iUserSize;
		for(i = 0;i < iUserSize;i++)
			kPacket << vString[i];
		TCPNetwork::SendToServer( kPacket );
		vString.clear();
	}					
}

void LobbyFriendListWnd::UpdateFriendList( int iCurPage )
{
	sg_bFriendListUpdate = false;

	// ģ�� ���
	int i = 0;
	m_UserList.clear();
	for(i = 0;i < g_FriendManager.GetNodeSize();i++)
	{
		FriendNode *pNode = g_FriendManager.GetNode( i );
		if( pNode )
		{
			UserListInfo kULI;
			kULI.m_szUserID	   = pNode->GetName();
			kULI.m_iGradeLevel = pNode->GetGradeLevel();
			kULI.m_iUserPos    = pNode->GetUserPos();
			kULI.m_bOnline     = pNode->IsOnline();
			kULI.m_bOtherBestFriend = pNode->IsOtherBF();
			if( g_FriendManager.GetBestFriendState( kULI.m_szUserID ) == BFT_SET )
			{   
				kULI.m_bMyBestFriend = true;
			}
			m_UserList.push_back( kULI );
		}
	}			
	std::sort( m_UserList.begin(), m_UserList.end(), UserListInfoSort() );	

	int iPrevPage = m_iCurPage;
	int iFirendCount = m_UserList.size();
	m_iMaxPage = max( 0, iFirendCount - 1 ) / MAX_PAGE_LIST;
	m_iCurPage = max( 0, iCurPage );
	m_iCurPage = min( m_iCurPage, m_iMaxPage );

	int iStartArray = m_iCurPage * MAX_PAGE_LIST;
	for(i = 0;i < MAX_PAGE_LIST;i++,iStartArray++)
	{
		LobbyUserListBtn *pWnd = (LobbyUserListBtn*)FindChildWnd( ID_LIST_1 + i );
		if( pWnd )
		{
			if( iStartArray < (int)m_UserList.size() )
			{
				UserListInfo &kULI = m_UserList[iStartArray];		
				pWnd->SetLobbyUserInfo( kULI, false );				
			}
			else
			{
				UserListInfo kULI;		
				pWnd->SetLobbyUserInfo( kULI, false );		
			}
		}
	}

	if( iFirendCount == 0 )
	{
		HideChildWnd( ID_LEFT_BTN );
		HideChildWnd( ID_RIGHT_BTN );
		ShowChildWnd( ID_HELP_BTN );
	}
	else
	{
		ShowChildWnd( ID_LEFT_BTN );
		ShowChildWnd( ID_RIGHT_BTN );
		HideChildWnd( ID_HELP_BTN );
	}

	if( m_iCurPage != iPrevPage )
	{
		m_dwChangePageTimer     = FRAMEGETTIME();
		m_dwUserPosRefreshTimer = 0;
	}
}

void LobbyFriendListWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();
	if( (int)m_UserList.size() == 0 )
	{
		g_FontMgr.SetTextStyle( TS_NORMAL );
		g_FontMgr.SetAlignType( TAT_CENTER );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextColor( 128, 128, 128 );
		g_FontMgr.PrintText( iXPos + 86, iYPos + 45, FONT_SIZE_12, STR(1) );
	}
	else
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
		kPrinter.PrintFullText( iXPos + 88, iYPos + 114, TAT_CENTER );
	}
}
//////////////////////////////////////////////////////////////////////////
bool LobbyGuildUserListWnd::sg_bGuildUserListUpdate = false;
LobbyGuildUserListWnd::LobbyGuildUserListWnd()
{
	m_iCurPage = m_iMaxPage = 0;
	m_dwChangePageTimer = 0;
	m_dwUserPosRefreshTimer = 0;
	m_dwGuildManual = 0;
}

LobbyGuildUserListWnd::~LobbyGuildUserListWnd()
{

}

void LobbyGuildUserListWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	m_dwGuildManual = xElement.GetIntAttribute( "GuildManual" );
}

void LobbyGuildUserListWnd::iwm_show()
{
	m_dwChangePageTimer     = FRAMEGETTIME();
	m_dwUserPosRefreshTimer = 0;
	UpdateGuildUserList( 0 );
}

void LobbyGuildUserListWnd::iwm_hide()
{

}

void LobbyGuildUserListWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_LEFT_BTN:
		if( cmd == IOBN_BTNUP )
		{
			UpdateGuildUserList( m_iCurPage - 1 );
		}
		break;
	case ID_RIGHT_BTN:
		if( cmd == IOBN_BTNUP )
		{
			UpdateGuildUserList( m_iCurPage + 1 );
		}
		break;
	case ID_HELP_BTN:
		if( cmd == IOBN_BTNUP )
		{
			ManualListWnd *pManualWnd = dynamic_cast<ManualListWnd*> (g_GUIMgr.FindWnd( MANUAL_LIST_WND ) );
			if( pManualWnd )
				pManualWnd->ShowWndByCurManual( m_dwGuildManual );
		}
		break;
	case ID_LIST_1:
	case ID_LIST_2:
	case ID_LIST_3:
	case ID_LIST_4:
	case ID_LIST_5:
		if( cmd == IOBN_BTNUP )
		{
			LobbyUserListBtn *pUserInfo = dynamic_cast<LobbyUserListBtn*>( pWnd );
			if( pUserInfo )
			{
				LobbyUserListOptionWnd *pOptionWnd = dynamic_cast<LobbyUserListOptionWnd*>( FindChildWnd( ID_OPTION_WND ) );
				if( pOptionWnd )
				{
					pOptionWnd->SetOptionInfo( pUserInfo->GetUserInfo() );
				}
			}
		}
		break;
	}
}

void LobbyGuildUserListWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;

	if( sg_bGuildUserListUpdate )
	{
		UpdateGuildUserList( m_iCurPage );
	} 
	else if( m_dwChangePageTimer != 0 )
	{
		if( FRAMEGETTIME() - m_dwChangePageTimer > 1000 )
		{
			if( g_App.IsMouseRender() )
			{
				SendUserPosRefresh();
				m_dwChangePageTimer = 0;
				m_dwUserPosRefreshTimer = FRAMEGETTIME();
			}
		}			
	}
	else if( m_dwUserPosRefreshTimer != 0 )
	{
		if( FRAMEGETTIME() - m_dwUserPosRefreshTimer > 15000 )
		{
			if( g_App.IsMouseRender() )
			{
				SendUserPosRefresh();
				m_dwUserPosRefreshTimer = FRAMEGETTIME();
			}
		}
	}
}

void LobbyGuildUserListWnd::SendUserPosRefresh()
{
	int i = 0;
	ioHashStringVec vString;
	for(i = 0;i < MAX_PAGE_LIST;i++)
	{
		LobbyUserListBtn *pWnd = (LobbyUserListBtn*)FindChildWnd( ID_LIST_1 + i );
		if( !pWnd ) continue;
		if( !pWnd->IsShow() ) continue;

		UserListInfo &rkInfo = pWnd->GetUserInfo();
		if( !rkInfo.m_bOnline ) continue;
		if( rkInfo.m_szUserID.IsEmpty() ) continue;


		vString.push_back( rkInfo.m_szUserID );
	}

	int iUserSize = vString.size();
	if( iUserSize != 0 )
	{
		SP2Packet kPacket( CTPK_USER_POS_REFRESH );
		kPacket << iUserSize;
		for(i = 0;i < iUserSize;i++)
			kPacket << vString[i];
		TCPNetwork::SendToServer( kPacket );
		vString.clear();
	}
}

void LobbyGuildUserListWnd::UpdateGuildUserList( int iCurPage )
{
	sg_bGuildUserListUpdate = false;

	// ���� ���
	int i = 0;
	m_UserList.clear();
	if( g_GuildInfo.IsGuild() )
	{
		ioGuildData *pGuildData = g_GuildInfo.GetGuildData( g_GuildInfo.GetGuildIndex() );
		if( pGuildData )
		{
			for(int i = 0;i < (int)pGuildData->GetGuildJoinUser();i++)
			{
				UserListInfo kULI;
				pGuildData->GetUserInfo( i, kULI.m_szUserID, kULI.m_iGradeLevel, kULI.m_iUserPos, kULI.m_bOnline );
				if( !kULI.m_szUserID.IsEmpty() && g_MyInfo.GetPublicID() != kULI.m_szUserID )
					m_UserList.push_back( kULI );
			}
		}
		std::sort( m_UserList.begin(), m_UserList.end(), UserListInfoSort() );	
	}

	int iPrevPage = m_iCurPage;
	int iUserCount = m_UserList.size();
	m_iMaxPage = max( 0, iUserCount - 1 ) / MAX_PAGE_LIST;
	m_iCurPage = max( 0, iCurPage );
	m_iCurPage = min( m_iCurPage, m_iMaxPage );

	int iStartArray = m_iCurPage * MAX_PAGE_LIST;
	for(i = 0;i < MAX_PAGE_LIST;i++,iStartArray++)
	{
		LobbyUserListBtn *pWnd = (LobbyUserListBtn*)FindChildWnd( ID_LIST_1 + i );
		if( pWnd )
		{
			if( iStartArray < (int)m_UserList.size() )
			{
				UserListInfo &kULI = m_UserList[iStartArray];		
				pWnd->SetLobbyUserInfo( kULI, true );				
			}
			else
			{
				UserListInfo kULI;		
				pWnd->SetLobbyUserInfo( kULI, true );		
			}
		}
	}

	if( g_GuildInfo.IsGuild() )
	{
		HideChildWnd( ID_HELP_BTN );
		ShowChildWnd( ID_LEFT_BTN );
		ShowChildWnd( ID_RIGHT_BTN );
	}
	else
	{
		ShowChildWnd( ID_HELP_BTN );
		HideChildWnd( ID_LEFT_BTN );
		HideChildWnd( ID_RIGHT_BTN );
	}

	if( m_iCurPage != iPrevPage )
	{
		m_dwChangePageTimer     = FRAMEGETTIME();
		m_dwUserPosRefreshTimer = 0;
	}
}

void LobbyGuildUserListWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();
	if( !g_GuildInfo.IsGuild() )
	{
		g_FontMgr.SetTextStyle( TS_NORMAL );
		g_FontMgr.SetAlignType( TAT_CENTER );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextColor( 128, 128, 128 );
		g_FontMgr.PrintText( iXPos + 86, iYPos + 45, FONT_SIZE_12, STR(1) );
	}
	else
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
		kPrinter.PrintFullText( iXPos + 88, iYPos + 114, TAT_CENTER );
	}
}
//////////////////////////////////////////////////////////////////////////
LobbyStageUserListWnd::LobbyStageUserListWnd()
{
	m_iCurPage = m_iMaxPage = 0;
	m_dwSendTimer = 0;
}

LobbyStageUserListWnd::~LobbyStageUserListWnd()
{

}

void LobbyStageUserListWnd::iwm_show()
{
	SendList( 0 );
	m_iCurPage = m_iMaxPage = 0;
	m_UserList.clear();
	UpdateLobbyStageUserList( 0 );

	ShowChildWnd( ID_LEFT_BTN );
	ShowChildWnd( ID_RIGHT_BTN );
}

void LobbyStageUserListWnd::iwm_hide()
{

}

void LobbyStageUserListWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_LEFT_BTN:
		if( cmd == IOBN_BTNUP )
		{
			UpdateLobbyStageUserList( m_iCurPage - 1 );
		}
		break;
	case ID_RIGHT_BTN:
		if( cmd == IOBN_BTNUP )
		{
			UpdateLobbyStageUserList( m_iCurPage + 1 );
		}
		break;
	case ID_LIST_1:
	case ID_LIST_2:
	case ID_LIST_3:
	case ID_LIST_4:
	case ID_LIST_5:
		if( cmd == IOBN_BTNUP )
		{
			LobbyUserListBtn *pUserInfo = dynamic_cast<LobbyUserListBtn*>( pWnd );
			if( pUserInfo )
			{
				LobbyUserListOptionWnd *pOptionWnd = dynamic_cast<LobbyUserListOptionWnd*>( FindChildWnd( ID_OPTION_WND ) );
				if( pOptionWnd )
				{
					pOptionWnd->SetOptionInfo( pUserInfo->GetUserInfo() );
				}
			}
		}
		break;
	}
}

void LobbyStageUserListWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;

    if( FRAMEGETTIME() - m_dwSendTimer > SEND_TIME )
	{
		SendList( m_iCurPage );
	}
}

void LobbyStageUserListWnd::SendList( int iCurPage )
{
	if( !g_App.IsMouseRender() )
		return;

	m_dwSendTimer = FRAMEGETTIME();

	/*
	SP2Packet kPacket( CTPK_SERVER_LOBBY_INFO );
	kPacket << iCurPage << MAX_PAGE_LIST;
	TCPNetwork::SendToServer( kPacket );
	*/
}

void LobbyStageUserListWnd::UpdateLobbyStageUserList( int iCurPage )
{
	// �κ� ���� ���
	int i = 0;
	int iPrevPage = m_iCurPage;
	m_iCurPage = max( 0, iCurPage );
	m_iCurPage = min( m_iCurPage, m_iMaxPage );

	if( iPrevPage != m_iCurPage )
	{
		SendList( m_iCurPage );
		m_UserList.clear();
	}

	int iUserCount = m_UserList.size();
	for(i = 0;i < MAX_PAGE_LIST;i++)
	{
		LobbyUserListBtn *pWnd = (LobbyUserListBtn*)FindChildWnd( ID_LIST_1 + i );
		if( pWnd )
		{
			if( i < (int)m_UserList.size() )
			{
				UserListInfo &kULI = m_UserList[i];		
				pWnd->SetLobbyUserInfo( kULI, false );				
			}
			else
			{
				UserListInfo kULI;		
				pWnd->SetLobbyUserInfo( kULI, false );		
			}
		}
	}
}

void LobbyStageUserListWnd::ApplyLobbyStageList( SP2Packet &rkPacket )
{
	m_UserList.clear();

	int iTotalList, iCurList;
	rkPacket >> iTotalList >> iCurList;			
	for(int i = 0;i < iCurList;i++)
	{
		UserListInfo kUserInfo;
		DWORD dwGuildIndex, dwGuildMark;
		rkPacket >> kUserInfo.m_szUserID >> kUserInfo.m_iGradeLevel >> dwGuildIndex >> dwGuildMark;
		kUserInfo.m_iUserPos = UP_LOBBY;
		kUserInfo.m_bOnline  = true;
		
		// ��� ����
		g_GuildInfo.SetGuildMark( kUserInfo.m_szUserID, dwGuildIndex, dwGuildMark );

		m_UserList.push_back( kUserInfo );
	}    
	m_iMaxPage = max( 0, iTotalList - 1 ) / MAX_PAGE_LIST;
	UpdateLobbyStageUserList( m_iCurPage );
}

void LobbyStageUserListWnd::OnRender()
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
	kPrinter.PrintFullText( iXPos + 88, iYPos + 114, TAT_CENTER );
}
//////////////////////////////////////////////////////////////////////////
LobbyUserListOptionWnd::LobbyUserListOptionWnd()
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
}

LobbyUserListOptionWnd::~LobbyUserListOptionWnd()
{
	SAFEDELETE( m_pDarkLine	);
	SAFEDELETE( m_pLightLine );
	SAFEDELETE( m_pOverLine	);
}

void LobbyUserListOptionWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	m_iAddWidth	= xElement.GetIntAttribute( "AddWidth" );
	m_iAddHeight= xElement.GetIntAttribute( "AddHeight" );
	m_iListXPos = xElement.GetIntAttribute( "ListX" );
	m_iListYPos = xElement.GetIntAttribute( "ListY" );
	m_iHeightGap= xElement.GetIntAttribute( "HeightGap" );
}

void LobbyUserListOptionWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
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

void LobbyUserListOptionWnd::CommandOption( DWORD dwCmd )
{
	switch( dwCmd )
	{
	case OPTION_INFO:
		{
			g_App.OpenMannerWnd( m_UserInfo.m_szUserID.c_str(), m_UserInfo.m_bOnline );
		}
		break;
	case OPTION_FOLLOW:
		{
			if( !m_UserInfo.m_szUserID.IsEmpty() )
			{
				switch( m_UserInfo.m_iUserPos )
				{
				case UP_BATTLE_ROOM:
				case UP_LADDER_TEAM:
				case UP_TRAINING:
				case UP_HEADQUARTERS:
				case UP_TOURNAMENT:
				case UP_SHUFFLE:
				case UP_GUILD_HQ:
				case UP_HOUSE:
					{
						SP2Packet kPacket( CTPK_USER_POS_INDEX );
						kPacket << m_UserInfo.m_szUserID << m_UserInfo.m_iUserPos;
						TCPNetwork::SendToServer( kPacket );
					}
					break;
				}
			}
		}
		break;	
	}
}

void LobbyUserListOptionWnd::SetOptionInfo( UserListInfo &rkInfo )
{
	HideWnd();
	/*	
	-��Ī
	[�������� ����]
	[���� ���󰡱�]
	[���� ���󰡱�]
	[���� ���󰡱�]
	[�Ʒü� ���󰡱�]
	-�ɼ� ����� �ϳ��ۿ� ������ ��� �����Ѵ�.
	*/
	m_OptionMap.clear();

	m_UserInfo = rkInfo;
	if( m_UserInfo.m_szUserID.IsEmpty() )
		return;

	char szTitle[MAX_PATH] = "";
	SafeSprintf( szTitle, sizeof( szTitle ), STR(1), m_UserInfo.m_szUserID.c_str() );
	m_OptionMap.insert( OptionMap::value_type( OPTION_INFO, szTitle ) );
	
	//
	switch( m_UserInfo.m_iUserPos )
	{
	case UP_BATTLE_ROOM:
		{
			m_OptionMap.insert( OptionMap::value_type( OPTION_FOLLOW, STR(2) ) );
		}
		break;
	case UP_LADDER_TEAM:
		{
			m_OptionMap.insert( OptionMap::value_type( OPTION_FOLLOW, STR(3) ) );
		}
		break;
	case UP_TRAINING:
		{
			m_OptionMap.insert( OptionMap::value_type( OPTION_FOLLOW, STR(4) ) );
		}
		break;
	case UP_HEADQUARTERS:
		{
			m_OptionMap.insert( OptionMap::value_type( OPTION_FOLLOW, STR(1) ) );
			m_OptionMap.insert( OptionMap::value_type( OPTION_FOLLOW, STR(1) ) );
		}
		break;
	case UP_HOUSE:
		{
			m_OptionMap.insert( OptionMap::value_type( OPTION_FOLLOW, STR(2) ) );
		}
		break;
	/*
	case UP_GUILD_HQ:
		{
			m_OptionMap.insert( OptionMap::value_type( OPTION_FOLLOW, STR(3) ) );
		}
		break;
	*/
	}

	//
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
			int iOffsetX = GetParent()->GetDerivedPosX();
			int iOffsetY = GetParent()->GetDerivedPosY();
			if( pMouse->GetMousePos().x + GetWidth() > Setting::Width() )
				SetWndPos( ( pMouse->GetMousePos().x - GetWidth() ) - iOffsetX, pMouse->GetMousePos().y - iOffsetY );
			else
				SetWndPos( pMouse->GetMousePos().x - iOffsetX, pMouse->GetMousePos().y - iOffsetY );
		}
		ShowWnd();
	}
}

void LobbyUserListOptionWnd::iwm_show()
{
	SetCheckParentOutSide( true );
}

void LobbyUserListOptionWnd::iwm_hide()
{
	SetCheckParentOutSide( false );
}

void LobbyUserListOptionWnd::OnProcess( float fTimePerSec )
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
			break;
		}
	}
}

void LobbyUserListOptionWnd::OnRender()
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
LobbyUserListWnd::LobbyUserListWnd()
{
	m_dwTabID = ID_LOBBY_BTN;
}

LobbyUserListWnd::~LobbyUserListWnd()
{

}

void LobbyUserListWnd::iwm_create()
{
	g_GUIMgr.AddWnd( "XML/lobbyfriendlistwnd.xml", this );
	g_GUIMgr.AddWnd( "XML/lobbyguilduserlistwnd.xml", this );
	g_GUIMgr.AddWnd( "XML/lobbystageuserlistwnd.xml", this );
}

void LobbyUserListWnd::iwm_show()
{
	ChangeTab( ID_LOBBY_BTN );
}

void LobbyUserListWnd::iwm_hide()
{
	HideChildWnd( ID_FRIEND_WND );
	HideChildWnd( ID_GUILD_WND );
	HideChildWnd( ID_LOBBY_WND );
}

void LobbyUserListWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_FRIEND_BTN:
	case ID_GUILD_BTN:
	case ID_LOBBY_BTN:
		if( cmd == IOBN_BTNDOWN )
		{
			ChangeTab( dwID );
		}
		break;
	}
}

void LobbyUserListWnd::ChangeTab( DWORD dwTabID )
{
	m_dwTabID = dwTabID;
	CheckRadioButton( ID_FRIEND_BTN, ID_LOBBY_BTN, m_dwTabID );

	HideChildWnd( ID_FRIEND_WND );
	HideChildWnd( ID_GUILD_WND );
	HideChildWnd( ID_LOBBY_WND );

	switch( m_dwTabID )
	{
	case ID_FRIEND_BTN:
		ShowChildWnd( ID_FRIEND_WND );
		break;
	case ID_GUILD_BTN:
		ShowChildWnd( ID_GUILD_WND );
		break;
	case ID_LOBBY_BTN:
		ShowChildWnd( ID_LOBBY_WND );
		break;
	}
}

void LobbyUserListWnd::ApplyLobbyUserList( SP2Packet &rkPacket )
{
	LobbyStageUserListWnd *pLobbyStageWnd = dynamic_cast<LobbyStageUserListWnd*>(FindChildWnd( ID_LOBBY_WND ));
	if( pLobbyStageWnd && pLobbyStageWnd->IsShow() )
		pLobbyStageWnd->ApplyLobbyStageList( rkPacket );
}

//////////////////////////////////////////////////////////////////////////
UserInfoListOptionWnd::UserInfoListOptionWnd()
{
	m_pPlayStage = NULL;
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

	m_dwMsgCmd = 0;
}

UserInfoListOptionWnd::~UserInfoListOptionWnd()
{
	SAFEDELETE( m_pDarkLine	);
	SAFEDELETE( m_pLightLine );
	SAFEDELETE( m_pOverLine	);
}

void UserInfoListOptionWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	m_iAddWidth	= xElement.GetIntAttribute( "AddWidth" );
	m_iAddHeight= xElement.GetIntAttribute( "AddHeight" );
	m_iListXPos = xElement.GetIntAttribute( "ListX" );
	m_iListYPos = xElement.GetIntAttribute( "ListY" );
	m_iHeightGap= xElement.GetIntAttribute( "HeightGap" );
}

void UserInfoListOptionWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
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

void UserInfoListOptionWnd::CommandOption( DWORD dwCmd )
{
	switch( dwCmd )
	{
	case OPTION_INFO:
		{
			g_App.OpenMannerWnd( m_szUserID.c_str(), m_bOnLine );
		}
		break;
	case OPTION_ADD_BEST_FRIEND:
		{
			UserListWnd *pWnd = dynamic_cast<UserListWnd*>(GetParent());
			if( pWnd )
				pWnd->OnInsertBestFriend( m_szUserID );
		}
		break;
	case OPTION_RELEASE_BEST_FRIEND:
		{
			UserListWnd *pWnd = dynamic_cast<UserListWnd*>(GetParent());
			if( pWnd && pWnd->CheckDeleteBestFriend( m_szUserID ) )
			{
				g_GUIMgr.SetMsgBox( MB_YESNO, this, STR(1), m_szUserID.c_str() );
				m_dwMsgCmd = OPTION_RELEASE_BEST_FRIEND;
			}
		}
		break;
	case OPTION_BLACK_DELETE:
		{
			g_BlackListManager.RemoveBlackList( m_szUserID );
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		}
		break;
	case OPTION_FOLLOW:
		if( m_pPlayStage )
		{
			if( m_pPlayStage->GetModeType() != MT_MYROOM && m_pPlayStage->GetModeType() != MT_TRAINING && m_pPlayStage->GetModeType() != MT_HEADQUARTERS && m_pPlayStage->GetModeType() != MT_HOUSE )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
			}	
			else if( g_MyInfo.GetActiveCharCount() == 0 )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
			}
			else if( g_MyInfo.GetActiveCharCount() == 1 && g_MyInfo.GetActiveExerciseCharCount() == 1 )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
			}
			else if( g_MyInfo.IsTutorialUser() )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6) );
			}
			else if( !m_szUserID.IsEmpty() )
			{
				if( m_bEnableMove )
				{
					switch( m_iUserPos )
					{
					case UP_TRAINING:
					case UP_BATTLE_ROOM:  
					case UP_LADDER_TEAM:
					case UP_HEADQUARTERS:
					case UP_TOURNAMENT:
					case UP_SHUFFLE:
					case UP_GUILD_HQ:
					case UP_HOUSE:
						{
							SP2Packet kPacket( CTPK_USER_POS_INDEX );
							kPacket << m_szUserID << m_iUserPos;
							TCPNetwork::SendToServer( kPacket );
						}
						break;
					default:
						{
							TCPNetwork::MouseBusy( true );
							SP2Packet kPacket( CTPK_FOLLOW_USER );
							kPacket << m_szUserID << m_iUserPos;
							TCPNetwork::SendToServer( kPacket );
						}
						break;
					}							
				}
			}
		}
		break;
	case OPTION_INVITE:
		{
			if( m_bEnableInvite )
			{
				// �ʴ� �۷�
				if( g_LadderTeamMgr.IsLadderTeam() )
				{
					//�ʴ�
					if( m_iUserLevel < g_LadderTeamMgr.GetLadderTeamLimitGrade() )
					{
						char szGradeName[MAX_PATH] = "";
						g_LevelMgr.GetGradeName( g_LadderTeamMgr.GetLadderTeamLimitGrade(), szGradeName, sizeof( szGradeName ), false );
						g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(7), szGradeName );
					}
					else
					{
						SP2Packet kPacket( CTPK_LADDERTEAM_INVITE );
						kPacket << 1 << m_szUserID;
						TCPNetwork::SendToServer( kPacket );
					}
				}
				else if( g_BattleRoomMgr.IsBattleRoom() )
				{
					TCPNetwork::MouseBusy( true );
					SP2Packet kPacket( CTPK_BATTLEROOM_INVITE );
					kPacket << 1 << m_szUserID << -1 << 0 << 0;
					TCPNetwork::SendToServer( kPacket );	
				}
			}
		}
		break;
	}
}

void UserInfoListOptionWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case MESSAGE_BOX:
		if( cmd == IOBN_BTNUP )
		{
			if( param != IDYES )
				return;

			switch( m_dwMsgCmd )
			{
			case OPTION_RELEASE_BEST_FRIEND:
				{
					UserListWnd *pWnd = dynamic_cast<UserListWnd*>(GetParent());
					if( pWnd )
					{
						pWnd->_OnDeleteBestFriend( m_szUserID );
					}
				}
				break;
			}
		}
		break;
	}	
}

void UserInfoListOptionWnd::SetOptionInfo( ioPlayStage *pStage, const ioHashString &szUserID, int iUserLevel, bool bBestFriend, int iUserPos, int iListType, bool bOnLine, bool bEnableMove, bool bEnableInvite )
{
	HideWnd();
	/*	
	-ģ�� ��
	-��� ��
	[�������� ����]
	[��ġ ���󰡱�] / [�ʴ�]
	[��ģ���� ����] & [��ģ ����]
	
	-���� ��
	[�������� ����]
	[���� ����]

	-�ɼ� ����� �ϳ��ۿ� ������ ��� �����Ѵ�.
	-����� �� ���� �ɼ��� �������� �ʴ´�.
	*/
	m_OptionMap.clear();

	m_pPlayStage    = pStage;
	m_szUserID      = szUserID;
	m_iUserLevel    = iUserLevel;
	m_iUserPos      = iUserPos;
	m_bBestFriend   = bBestFriend;
	m_iListType     = iListType;
	m_bOnLine       = bOnLine;
	m_bEnableMove   = bEnableMove;
	m_bEnableInvite = bEnableInvite;

	// ����
	if( !m_szUserID.IsEmpty() )
	{
		char szTitle[MAX_PATH] = "";
		SafeSprintf( szTitle, sizeof( szTitle ), STR(1), m_szUserID.c_str() );
		m_OptionMap.insert( OptionMap::value_type( OPTION_INFO, szTitle ) );
	}

	if( m_iListType == UserInfoListWnd::TYPE_FRIENDLIST||
		m_iListType == UserInfoListWnd::TYPE_GUILDUSERLIST )
	{
		char szTitle[MAX_PATH] = "";
		if( m_bOnLine )
		{
			if( m_bEnableInvite )
			{
				if( g_BattleRoomMgr.IsBattleRoom() || g_LadderTeamMgr.IsLadderTeam() )
				{
					SafeSprintf( szTitle, sizeof( szTitle ), STR(2) );
					m_OptionMap.insert( OptionMap::value_type( OPTION_INVITE, szTitle ) );
				}
			}

			if( m_bEnableMove )
			{
				switch( m_iUserPos )
				{
				case UP_TRAINING:
					SafeSprintf( szTitle, sizeof( szTitle ), STR(3) );
					m_OptionMap.insert( OptionMap::value_type( OPTION_FOLLOW, szTitle ) );
					break;
				case UP_BATTLE_ROOM:
					SafeSprintf( szTitle, sizeof( szTitle ), STR(4) );
					m_OptionMap.insert( OptionMap::value_type( OPTION_FOLLOW, szTitle ) );
					break;
				case UP_LADDER_TEAM:
					SafeSprintf( szTitle, sizeof( szTitle ), STR(5) );
					m_OptionMap.insert( OptionMap::value_type( OPTION_FOLLOW, szTitle ) );
					break;
				case UP_LOBBY:
					SafeSprintf( szTitle, sizeof( szTitle ), STR(6) );
					m_OptionMap.insert( OptionMap::value_type( OPTION_FOLLOW, szTitle ) );
					break;
				case UP_HEADQUARTERS:
					SafeSprintf( szTitle, sizeof( szTitle ), STR(7) );
					SafeSprintf( szTitle, sizeof( szTitle ), STR(1) );
					m_OptionMap.insert( OptionMap::value_type( OPTION_FOLLOW, szTitle ) );
					break;
				case UP_HOUSE:
					SafeSprintf( szTitle, sizeof( szTitle ), STR(1) );
					m_OptionMap.insert( OptionMap::value_type( OPTION_FOLLOW, szTitle ) );
					break;
				/*
				case UP_GUILD_HQ:
					SafeSprintf( szTitle, sizeof( szTitle ), STR(2) );
					m_OptionMap.insert( OptionMap::value_type( OPTION_FOLLOW, szTitle ) );
					break;
				*/
				}
			}
		}

		if( m_bBestFriend )
		{
			SafeSprintf( szTitle, sizeof( szTitle ), STR(8) );
			m_OptionMap.insert( OptionMap::value_type( OPTION_RELEASE_BEST_FRIEND, szTitle ) );
		}
		else
		{
			SafeSprintf( szTitle, sizeof( szTitle ), STR(9) );
			m_OptionMap.insert( OptionMap::value_type( OPTION_ADD_BEST_FRIEND, szTitle ) );
		}
	}
	else if( m_iListType == UserInfoListWnd::TYPE_BLACKLIST )
	{
		char szTitle[MAX_PATH] = "";
		SafeSprintf( szTitle, sizeof( szTitle ), STR(10) );
		m_OptionMap.insert( OptionMap::value_type( OPTION_BLACK_DELETE, szTitle ) );
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
			int iOffsetX = GetParent()->GetDerivedPosX();
			int iOffsetY = GetParent()->GetDerivedPosY();
			if( pMouse->GetMousePos().x + GetWidth() > Setting::Width() )
				SetWndPos( ( pMouse->GetMousePos().x - GetWidth() ) - iOffsetX, pMouse->GetMousePos().y - iOffsetY );
			else
				SetWndPos( pMouse->GetMousePos().x - iOffsetX, pMouse->GetMousePos().y - iOffsetY );
		}
		ShowWnd();
	}
}

void UserInfoListOptionWnd::iwm_show()
{
	SetCheckParentOutSide( true );
}

void UserInfoListOptionWnd::iwm_hide()
{
	SetCheckParentOutSide( false );
}

void UserInfoListOptionWnd::OnProcess( float fTimePerSec )
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
			break;
		}
	}
}

void UserInfoListOptionWnd::OnRender()
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