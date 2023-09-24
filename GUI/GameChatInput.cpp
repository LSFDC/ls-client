	

#include "StdAfx.h"
#include "../io3DEngine/InputBox.h"
#include "../io3DEngine/ioWnd.h"
#include "../io3DEngine/ioEdit.h"
#include "../io3DEngine/ioUIRenderFrame.h"
#include "../io3DEngine/ioFontManager.h"
#include "../io3DEngine/ioXMLDocument.h"
#include "../io3DEngine/ioXMLElement.h"
#include "../io3DEngine/HelpFunc.h"
#include "../io3DEngine/iostringmanager.h"
#include "../io3dengine/safesprintf.h"

#include "../DirectInput.h"
#include "../TextColorTable.h"
#include "../ioPlayStage.h"
#include "../ioBaseChar.h"
#include "../NetworkWrappingFunc.h"
#include "../SlangUtil.h"
#include "../ioMyInfo.h"
#include "../ioChatMacro.h"
#include "../ioApplication.h"
#include "../ioBattleRoomMgr.h"
#include "../WndID.h"
#include "../TextColorTable.h"
#include "../Setting.h"
#include "../ioP2PChecker.h"
#include "../ioGuildInfo.h"
#include "../ioLadderTeamMgr.h"
#include "../ioCustomSoundMgr.h"
#include "../ioGuildMarkMgr.h"
#include "../ioEtcItem.h"
#include "../ioEtcItemManager.h"
#include "../ioCampManager.h"
#include "../EtcHelpFunc.h"
#include "../ioMapEventSceneMgr.h"
#include "../ioMyRoomMode.h"
#include "../ChatMacroMgr.h"
#include "../ioPresentMgr.h"


#include "DebugInfoWnd.h"
#include "ioAlarmManager.h"
#include "ChatMacro.h"
#include "GameChatInput.h"
#include "SoldierLimitWnd.h"
#include "ioSP2GUIManager.h"
#include "TimeGateSearch.h"
#include "ioTutorialManager.h"
#include "ioUserInfoMgr.h"
#include "GuildWnd.h"
#include "AnnouncementWnd.h"
#include "FishingInventoryWnd.h"
#include "QuestWnd.h"

#include <strsafe.h>
#include "../Local/ioLocalManager.h"

#include "../ioShuffleRoomMgr.h"

ChatType GameChatInput::sg_InputLinkChat = CT_ALL;
ioHashString GameChatInput::sg_InputLinkString;
GameChatInput::GameChatInput()
{
	m_pFocusBoxFrm = NULL;

	m_eChatType = CT_ALL;

	m_pStage   = NULL;
	m_pPreEdit = NULL;

	m_ChatEnableState = CES_ENABLE;

	m_iCurChatLimitCount = 0;
	m_iChatLimitCount    = 3;
	m_dwCurChatLimitTime = 0;
	m_dwChatLimitTime    = 3000;

	m_iCurWholeChatLimitCount = 0;
	m_iWholeChatLimitCount    = 3;
	m_dwCurWholeChatLimitTime = 0;
	m_dwWholeChatLimitTime    = 3000;
	
	m_bShowEditFocus     = false;

	m_szChatWarningText = STR(1);
	m_szChatBlockText   = STR(2);
}

GameChatInput::~GameChatInput()
{
	SAFEDELETE( m_pFocusBoxFrm );
}

void GameChatInput::AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame )
{
	if( pFrame )
	{
		if( szType == "FocusBox" )
		{
			SAFEDELETE( m_pFocusBoxFrm );
			m_pFocusBoxFrm = pFrame;
		}
		else
		{	
			ioWnd::AddRenderFrame( szType, pFrame );
		}
	}
}

void GameChatInput::SetPlayStage( ioPlayStage *pPlayStage )
{
	m_pStage = pPlayStage;
	CheckBtnChatType();
}

bool GameChatInput::IsBeforeStageChatTypeLink()
{
	if( m_pStage == NULL ) return false;
	if( sg_InputLinkString.IsEmpty() ) return false;

	if( sg_InputLinkChat == CT_WHOLE_SERVER && IsWholeChatItem() )
		return true;

	switch( m_pStage->GetModeType() )
	{
	case MT_MYROOM:
		{
			switch( ioMyRoomMode::GetMyRoomType() )
			{
			case MRT_LOBBY:
				{
					if( sg_InputLinkChat == CT_SERVER_LOBBY )
						return true;
				}
				break;
			case MRT_BATTLE:
			case MRT_SHUFFLE:
				{
					if( sg_InputLinkChat == CT_PARTY )
						return true;
				}
				break;
			case MRT_LADDER:
				{
					if( sg_InputLinkChat == CT_LADDER )
						return true;
				}
				break;
			default:
				{
					if( sg_InputLinkChat == CT_ALL )
						return true;
				}
				break;
			}
		}
		break;
	case MT_TRAINING:
	case MT_HEADQUARTERS:
	case MT_HOUSE:
	case MT_SURVIVAL:
	case MT_FIGHT_CLUB:
	case MT_MONSTER_SURVIVAL:
	case MT_DUNGEON_A:
	case MT_BOSS:
	case MT_GANGSI:
	case MT_SHUFFLE_BONUS:
		{
			if( sg_InputLinkChat == CT_PARTY || sg_InputLinkChat == CT_LADDER || sg_InputLinkChat == CT_ALL )
			{
				sg_InputLinkChat = CT_ALL;
				return true;
			}
		}
		break;
	default:
		{			
			if( sg_InputLinkChat == CT_PARTY || sg_InputLinkChat == CT_LADDER || sg_InputLinkChat == CT_ALL )
			{	
				sg_InputLinkChat = CT_ALL;
				return true;
			}
			else if( sg_InputLinkChat == CT_TEAM )
			{
				return true;
			}
		}
		break;
	}	
	return false;
}

void GameChatInput::SetFirstChatTypeToBtn( ChatType eChatType )
{
	if( !IsBeforeStageChatTypeLink() )
		SetChatTypeToBtn( eChatType );
	else
	{
		SetChatTypeToBtn( sg_InputLinkChat );
		ShowWnd();
		
		if( m_ChatEnableState == CES_ENABLE )
		{
			ioEdit *pEdit = dynamic_cast<ioEdit*>(FindChildWnd( ID_CHAT_EDIT ));
			if( pEdit )
			{
				pEdit->SetKeyFocus();
				pEdit->SetText( sg_InputLinkString.c_str() );
				g_InputBox.SetString( sg_InputLinkString.c_str() );
			}
		}
	}
	sg_InputLinkString.Clear();
}

void GameChatInput::CheckBtnChatType()
{
	if( m_pStage == NULL ) return;

	// ��Ȳ�� �´� ��ư ��ġ	
	switch( m_pStage->GetModeType() )
	{
	case MT_MYROOM:
		{
			m_eChatType = CT_SERVER_LOBBY;
			switch( ioMyRoomMode::GetMyRoomType() )
			{
			case MRT_LOBBY:
				{
					m_eChatType = CT_SERVER_LOBBY;
				}
				break;
			case MRT_BATTLE:
			case MRT_SHUFFLE:
				{
					m_eChatType = CT_PARTY;
				}
				break;
			case MRT_LADDER:
				{
					m_eChatType = CT_LADDER;
				}
				break;
			default:
				{
					m_eChatType = CT_ALL;
				}
				break;
			}
		}
		break;
	case MT_TRAINING:
	case MT_HEADQUARTERS:
	case MT_HOUSE:
	case MT_SURVIVAL:
	case MT_FIGHT_CLUB:
	case MT_SHUFFLE_BONUS:
		{
			m_eChatType = CT_ALL;
		}
		break;
	default:
		{
			m_eChatType = CT_ALL;
			if( g_LadderTeamMgr.IsLadderTeam() && !g_LadderTeamMgr.IsHeroMatchMode() )
			{
				m_eChatType = CT_TEAM;
			}
		}
		break;
	}	

	SetChatTypeToBtn( m_eChatType );
}

void GameChatInput::ChangeBtnChatType()
{
	if( m_pStage == NULL ) return;

	// ���� ä�� Ÿ��	
	switch( m_pStage->GetModeType() )
	{
	case MT_MYROOM:
		{
			switch( ioMyRoomMode::GetMyRoomType() )
			{
			case MRT_LOBBY:
				{
					if( m_eChatType == CT_SERVER_LOBBY && IsWholeChatItem() )
					{
						m_eChatType = CT_WHOLE_SERVER;
					}
					//�������� Ȯ���Ⱑ ���� Ȯ����2�� �ִٸ� ��ħ2��
					else if( m_eChatType == CT_SERVER_LOBBY && IsWholeChatRainbowItem() )
					{
						m_eChatType = CT_WHOLE_SERVER_RAINBOW;
					}
					//��ħ�̰� Ȯ����2�� �ִٸ� ��ħ2�� 
					else if ( m_eChatType == CT_WHOLE_SERVER && IsWholeChatRainbowItem() )
					{
						m_eChatType = CT_WHOLE_SERVER_RAINBOW;
					}
					//��ħ2 ������ ��ü��
					else
					{
						m_eChatType = CT_SERVER_LOBBY;
					}
				}
				break;
			case MRT_BATTLE:
			case MRT_SHUFFLE:
				{
					//�������� Ȯ���Ⱑ �ִٸ� ��ħ����
					if( m_eChatType == CT_PARTY && IsWholeChatItem() )
					{
						m_eChatType = CT_WHOLE_SERVER;
					}
					//�������� Ȯ���Ⱑ ���� Ȯ����2�� �ִٸ� ��ħ2��
					else if( m_eChatType == CT_PARTY && IsWholeChatRainbowItem() )
					{
						m_eChatType = CT_WHOLE_SERVER_RAINBOW;
					}
					//��ħ�̰� Ȯ����2�� �ִٸ� ��ħ2�� 
					else if ( m_eChatType == CT_WHOLE_SERVER && IsWholeChatRainbowItem() )
					{
						m_eChatType = CT_WHOLE_SERVER_RAINBOW;
					}
					//��ħ2 ������ ��ü��
					else
					{
						m_eChatType = CT_PARTY;
					}
				}
				break;
			case MRT_LADDER:
				{
					//�������� Ȯ���Ⱑ �ִٸ� ��ħ����
					if( m_eChatType == CT_LADDER && IsWholeChatItem() )
					{
						m_eChatType = CT_WHOLE_SERVER;
					}
					//�������� Ȯ���Ⱑ ���� Ȯ����2�� �ִٸ� ��ħ2��
					else if( m_eChatType == CT_LADDER && IsWholeChatRainbowItem() )
					{
						m_eChatType = CT_WHOLE_SERVER_RAINBOW;
					}
					//��ħ�̰� Ȯ����2�� �ִٸ� ��ħ2�� 
					else if ( m_eChatType == CT_WHOLE_SERVER && IsWholeChatRainbowItem() )
					{
						m_eChatType = CT_WHOLE_SERVER_RAINBOW;
					}
					//��ħ2 ������ ����
					else
					{
						m_eChatType = CT_LADDER;
					}
				}
				break;
			default:
				{
					//�������� Ȯ���Ⱑ �ִٸ� ��ħ����
					if( m_eChatType == CT_ALL && IsWholeChatItem() )
					{
						m_eChatType = CT_WHOLE_SERVER;
					}
					//�������� Ȯ���Ⱑ ���� Ȯ����2�� �ִٸ� ��ħ2��
					else if( m_eChatType == CT_ALL && IsWholeChatRainbowItem() )
					{
						m_eChatType = CT_WHOLE_SERVER_RAINBOW;
					}
					//��ħ�̰� Ȯ����2�� �ִٸ� ��ħ2�� 
					else if ( m_eChatType == CT_WHOLE_SERVER && IsWholeChatRainbowItem() )
					{
						m_eChatType = CT_WHOLE_SERVER_RAINBOW;
					}
					//��ħ2 ������ ��ü��
					else
					{
						m_eChatType = CT_PARTY;
					}
				}
				break;
			}
		}
		break;
	case MT_TRAINING:
	case MT_HEADQUARTERS:
	case MT_HOUSE:
	case MT_SURVIVAL:
	case MT_FIGHT_CLUB:
	case MT_MONSTER_SURVIVAL:
	case MT_DUNGEON_A:
	case MT_BOSS:
	case MT_GANGSI:
	case MT_SHUFFLE_BONUS:
		{
			//ä�� Ÿ���� �ٲ��� �� ���� ALL�̸� Ȯ����1��
			//Ȯ���� 1�� ������ 2 üũ 2�� �ִٸ� 2��
			
			//��ü���� Ȯ���Ⱑ �ִٸ� ��ħ����
			if( m_eChatType == CT_ALL && IsWholeChatItem() )
			{
				m_eChatType = CT_WHOLE_SERVER;
			}
			//��ü���� Ȯ���Ⱑ ���� Ȯ����2�� �ִٸ� ��ħ2��
			else if( m_eChatType == CT_ALL && IsWholeChatRainbowItem() )
			{
				m_eChatType = CT_WHOLE_SERVER_RAINBOW;
			}
			//��ħ�̰� Ȯ����2�� �ִٸ� ��ħ2�� 
			else if ( m_eChatType == CT_WHOLE_SERVER && IsWholeChatRainbowItem() )
			{
				m_eChatType = CT_WHOLE_SERVER_RAINBOW;
			}
			//��ħ2 ������ ��ü��
			else
			{
				m_eChatType = CT_ALL;
			}
		}
		break;
	default:
		{
			if( g_BattleRoomMgr.IsBattleRoom() )
			{
				//��ü���� 
				if( m_eChatType == CT_ALL )
				{
					//���� �����ڰ� �ƴ϶�� TEAMê����
					if( !g_BattleRoomMgr.IsObserverUser( g_MyInfo.GetPublicID() ) )
						m_eChatType = CT_TEAM;
					//���� �������̰� Ȯ���� �������� �ִٸ� Ȯ�����
					else if( IsWholeChatItem() )
						m_eChatType = CT_WHOLE_SERVER;
					else if ( IsWholeChatRainbowItem() )
						m_eChatType = CT_WHOLE_SERVER_RAINBOW;
				}
				//��ê���� Ȯ���Ⱑ �ִٸ�
				else if( m_eChatType == CT_TEAM && IsWholeChatItem() )
				{
					m_eChatType = CT_WHOLE_SERVER;
				}
				//��ê���� Ȯ����2�� �ִٸ�
				else if( m_eChatType == CT_TEAM && IsWholeChatRainbowItem() )
				{
					m_eChatType = CT_WHOLE_SERVER_RAINBOW;
				}
				//��ħ�̰� Ȯ����2�� �ִٸ� ��ħ2�� 
				else if ( m_eChatType == CT_WHOLE_SERVER && IsWholeChatRainbowItem() )
				{
					m_eChatType = CT_WHOLE_SERVER_RAINBOW;
				}
				else
				{
					m_eChatType = CT_ALL;
				}
			}
			else if( g_ShuffleRoomMgr.IsShuffleRoom() )
			{
				if( m_eChatType == CT_ALL )
				{
					m_eChatType = CT_TEAM;
				}
				//��ê���� Ȯ���Ⱑ �ִٸ�
				else if( m_eChatType == CT_TEAM && IsWholeChatItem() )
				{
					m_eChatType = CT_WHOLE_SERVER;
				}
				//��ê���� Ȯ����2�� �ִٸ�
				else if( m_eChatType == CT_TEAM && IsWholeChatRainbowItem() )
				{
					m_eChatType = CT_WHOLE_SERVER_RAINBOW;
				}
				//��ħ�̰� Ȯ����2�� �ִٸ� ��ħ2�� 
				else if ( m_eChatType == CT_WHOLE_SERVER && IsWholeChatRainbowItem() )
				{
					m_eChatType = CT_WHOLE_SERVER_RAINBOW;
				}
				else
				{
					m_eChatType = CT_ALL;
				}
			}
			else if( g_LadderTeamMgr.IsLadderTeam() )
			{
				if( m_eChatType == CT_ALL )
				{
					//���� �����ڰ� �ƴ϶�� TEAMê����
					if( !g_LadderTeamMgr.IsHeroMatchMode() )
						m_eChatType = CT_TEAM;
					//���� �������̰� Ȯ���� �������� �ִٸ� Ȯ�����
					else if( IsWholeChatItem() )
						m_eChatType = CT_WHOLE_SERVER;
					else if ( IsWholeChatRainbowItem() )
						m_eChatType = CT_WHOLE_SERVER_RAINBOW;
				}
				//��ê���� Ȯ���Ⱑ �ִٸ�
				else if( m_eChatType == CT_TEAM && IsWholeChatItem() )
				{
					m_eChatType = CT_WHOLE_SERVER;
				}
				//��ê���� Ȯ����2�� �ִٸ�
				else if( m_eChatType == CT_TEAM && IsWholeChatRainbowItem() )
				{
					m_eChatType = CT_WHOLE_SERVER_RAINBOW;
				}
				//��ħ�̰� Ȯ����2�� �ִٸ� ��ħ2�� 
				else if ( m_eChatType == CT_WHOLE_SERVER && IsWholeChatRainbowItem() )
				{
					m_eChatType = CT_WHOLE_SERVER_RAINBOW;
				}
				else
				{
					m_eChatType = CT_ALL;
				}
			}
		}
		break;
	}	

	SetChatTypeToBtn( m_eChatType );
}

void GameChatInput::ParseExtraInfo( ioXMLElement &xElement )
{
	m_iChatLimitCount   = xElement.GetIntAttribute_e( "ChatLimitCount" );
	m_dwChatLimitTime   = xElement.GetIntAttribute_e( "ChatLimitTime" );
	m_bShowEditFocus    = xElement.GetBoolAttribute_e( "ShowEditFocus" );
}

void GameChatInput::SetChatTypeToBtn( ChatType eChatType )
{
	m_eChatType = eChatType;
	ioWnd *pChatBtn = FindChildWnd( ID_CHAT_TYPE_BTN );
	if( pChatBtn )
	{
		pChatBtn->SetTitleNormalTextColor( TCT_DEFAULT_DARKGRAY );
		pChatBtn->SetTitleOverTextColor( TCT_DEFAULT_DARKGRAY );

		switch( m_eChatType )
		{
		case CT_TEAM:
			pChatBtn->SetTitleText( STR(1) );
			break;
		case CT_ALL:
			pChatBtn->SetTitleText( STR(2) );
			break;
		case CT_PARTY:
			pChatBtn->SetTitleText( STR(3) );
			break;
		case CT_LADDER:
			pChatBtn->SetTitleText( STR(4) );
			break;
		case CT_SERVER_LOBBY:
			pChatBtn->SetTitleText( STR(5) );
			break;
		case CT_WHOLE_SERVER:
			pChatBtn->SetTitleText( STR(6) );
			break;
		case CT_WHOLE_SERVER_RAINBOW:
			pChatBtn->SetTitleNormalTextColor( TCT_DEFAULT_RED );
			pChatBtn->SetTitleOverTextColor( TCT_DEFAULT_RED );
			pChatBtn->SetTitleText( STR(7) );
			break;
		}
	}
}

void GameChatInput::ShowChatInputBox()
{
	if( !IsShow() )
	{
		g_InputBox.ClearKeyState();
		ShowWnd();
	}
}

void GameChatInput::ShowChatConv( const ioHashString &rkName, TeamType eTeam, ioPlayStage *pStage )
{
	m_Name = rkName;
	m_OwnerTeam  = eTeam;
	m_pStage = pStage;

	ChangeBtnChatType();
	ShowChatInputBox();
}

void GameChatInput::ShowChat( const ioHashString &rkName, TeamType eTeam, ioPlayStage *pStage )
{
	m_Name = rkName;
	m_OwnerTeam = eTeam;
	m_pStage = pStage;

	if( IsShow() && m_ChatEnableState == CES_BLOCK )
	{
		// CES_BLOCK���¿����� ��Ŀ���� ���⶧���� �����Է��� ������
		// �ܺο��� ����Ű �Է½� ȣ��Ǵ� ���� �̰��̶�..;;
		HideWnd();
	}
	else
	{
		ShowChatInputBox();
	}
}

bool GameChatInput::IsEnableEnter()
{
	if( NULL == ioEdit::m_pKeyFocusEdit )
		return true;

	return false;
}

bool GameChatInput::IsParentWndCheck( ioWnd *pCheckWnd, ioWnd *pParent )
{
	if( pCheckWnd == pParent ) 
		return true;

	if( pCheckWnd )
	{
		return IsParentWndCheck( pCheckWnd->GetParent(), pParent );
	}
	return false;
}

void GameChatInput::OnProcessFocus( ioEdit *pEdit )
{
	if( !pEdit )	return;

	if( ioEdit::m_pKeyFocusEdit == pEdit )      // ��Ŀ�� ����
	{
		if( g_GUIMgr.GetPreClickWnd() )
		{
			if( g_GUIMgr.GetPreClickWnd() == this ) return;
			if( g_GUIMgr.GetPreClickWnd() == pEdit ) return;
			if( IsParentWndCheck( g_GUIMgr.GetPreClickWnd()->GetParent(), GetParent() ) ) return;

			iwm_esc();
		}		
	}
	else if( NULL == ioEdit::m_pKeyFocusEdit && NULL == ioWnd::m_pTopModalWnd && g_InputBox.IsEnter() ) // ��Ŀ�� ����
	{
		m_pPreEdit = ioEdit::m_pKeyFocusEdit;
		switch( m_ChatEnableState )
		{
		case CES_ENABLE:
			pEdit->SetKeyFocus();
			break;
		case CES_WARNING:
			pEdit->SetKeyFocus();
			break;
		case CES_BLOCK:
			pEdit->SetKeyFocus();
			break;
		}
	}
}

void GameChatInput::OnProcess( float fTimePerSec )
{
	if( !IsShow() )	return;

	ProcessChangeChatBtn();

	ioEdit *pEdit = dynamic_cast< ioEdit* >( FindChildWnd( ID_CHAT_EDIT ) );
	if( !pEdit )	return;
    
	OnProcessFocus( pEdit );

	if( m_ChatEnableState != CES_BLOCK && ioEdit::m_pKeyFocusEdit == pEdit )
	{
		sg_InputLinkChat   = m_eChatType;
		sg_InputLinkString = g_InputBox.GetString();
	}	

	if( m_ChatEnableState != CES_WARNING && m_ChatEnableState != CES_BLOCK )
		return;	

	// ioEdit�� �ؽ�Ʈ������ �� OnProcess������ �̷�����⶧���� �������� ���̰� ����.
	// �ؼ� ���� g_InputBox.GetString()�� üũ��
	char szChatMsg[MAX_PATH];
	strcpy( szChatMsg, g_InputBox.GetString() );

	enum { MAX_LOOP = 2, };
	for (int i = 0; i < MAX_LOOP ; i++)
	{
		ioHashString szText;
		if( i == 0 )
			szText = m_szChatWarningText;
		else if( i == 1 )
			szText = m_szChatBlockText;

		if( strcmp(szText.c_str(), szChatMsg) == 0 )
		{
			if( i == 0 )
				m_ChatEnableState = CES_WARNING_EDITING;
			else if( i == 1 )
				m_ChatEnableState = CES_BLOCK_EDITING;

			int iWarningLen = strlen( szText.c_str() );
			if( (int)strlen( szChatMsg ) > iWarningLen )
			{
				pEdit->SetText( szChatMsg + iWarningLen );
				if( pEdit->IsKeyFocus() )
				{
					g_InputBox.SetStringNotDeleteComposing( szChatMsg + iWarningLen );
				}
			}
			else
			{
				pEdit->ClearString();
				if( pEdit->IsKeyFocus() )
				{
					g_InputBox.Clear();
				}
			}
		}	
	}
}

bool GameChatInput::iwm_esc()
{
	if( !IsShow() )
		return false;

	ioEdit *pEdit = dynamic_cast< ioEdit* >( FindChildWnd( ID_CHAT_EDIT ) );
	if( pEdit && pEdit == ioEdit::m_pKeyFocusEdit )
	{
		pEdit->KillKeyFocus();
		if( m_pPreEdit )	
		{
			ioWnd *pParent = m_pPreEdit->GetParent();
			if( pParent && pParent->IsShow() )
			{
				m_pPreEdit->SetKeyFocus();
			}

			m_pPreEdit = NULL;
		}
		return true;
	}
	return false;
}

void GameChatInput::iwm_show()
{
	if( g_MyInfo.GetBlockType() == BKT_LIMITATION )
		m_ChatEnableState = CES_BLOCK;
	else if( g_MyInfo.GetBlockType() == BKT_WARNNING )
		m_ChatEnableState = CES_WARNING;
	else
		m_ChatEnableState = CES_ENABLE;

	m_pPreEdit = ioEdit::m_pKeyFocusEdit;

	ioEdit *pEdit = (ioEdit*)FindChildWnd( ID_CHAT_EDIT );
	if( pEdit )
	{
		pEdit->ClearString();
		g_InputBox.SetString( "" );

		switch( m_ChatEnableState )
		{
		case CES_ENABLE:
			if( GetParent() == g_GUIMgr.FindWnd( GLOBAL_CHAT_WND ) )
				pEdit->SetTextColor( 0xFFFFFFFF, 0xFF000000 );
			else
				pEdit->SetTextColor( 0xFF3A3A3A, 0xFF000000 );
			pEdit->SetWriteCan();
			if( m_bShowEditFocus )
				pEdit->SetKeyFocus();
			break;
		case CES_WARNING:
			pEdit->SetTextColor( 0xFFFF8200, 0xFF000000 );
			pEdit->SetWriteCan();
			pEdit->SetText( m_szChatWarningText.c_str() );
			if( m_bShowEditFocus )
				pEdit->SetKeyFocus();
			break;
		case CES_BLOCK:
			pEdit->SetTextColor( 0xFFFF0000, 0xFF000000 );
			pEdit->SetWriteCan();
			pEdit->SetText( m_szChatBlockText.c_str() );
			if( m_bShowEditFocus )
				pEdit->SetKeyFocus();
			break;
		}
	}

	SetChatTypeToBtn( m_eChatType );
}

void GameChatInput::iwm_hide()
{
	ioEdit *pEdit = (ioEdit*)FindChildWnd( ID_CHAT_EDIT );
	if( pEdit )
	{
		pEdit->KillKeyFocus();
	}

	if( m_pPreEdit )	
	{
		ioWnd *pParent = m_pPreEdit->GetParent();
		if( pParent && pParent->IsShow() )
		{
			m_pPreEdit->SetKeyFocus();
		}

		m_pPreEdit = NULL;
	}
}

void GameChatInput::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	switch( pWnd->GetID() )
	{
	case ID_CHAT_EDIT:
		if( cmd == IOED_INPUT )
		{
			ioEdit *pEdit = dynamic_cast<ioEdit*>( pWnd );
			OnChatInput( pEdit, param );
			
			if( ioEdit::m_pKeyFocusEdit != pEdit )
			{
				pEdit->ClearString();
				g_InputBox.Clear();
				sg_InputLinkString.Clear();
			}
		}
		break;
	case ID_CHAT_TYPE_BTN:
		if( cmd == IOBN_BTNDOWN )
		{
			ChangeBtnChatType();
		}
		break;
	}
}

bool GameChatInput::CheckChatLimit()
{
	// �κ� ä�� ���� ���� 
	if( m_dwCurChatLimitTime == 0 )
		m_dwCurChatLimitTime = FRAMEGETTIME();

	m_iCurChatLimitCount++;
	if( FRAMEGETTIME() - m_dwCurChatLimitTime >= m_dwChatLimitTime )
	{
		m_iCurChatLimitCount = 0;
		m_dwCurChatLimitTime = FRAMEGETTIME();
	}

    if( m_iCurChatLimitCount >= m_iChatLimitCount )
	{
		m_dwCurChatLimitTime = FRAMEGETTIME();
		return true;
	}
	return false;
}

bool GameChatInput::CheckWholeChatLimit()
{
	// ��ħ ä�� ���� ���� 
	if( m_dwCurWholeChatLimitTime == 0 )
		m_dwCurWholeChatLimitTime = FRAMEGETTIME();

	m_iCurWholeChatLimitCount++;
	if( FRAMEGETTIME() - m_dwCurWholeChatLimitTime >= m_dwWholeChatLimitTime )
	{
		m_iCurWholeChatLimitCount = 0;
		m_dwCurWholeChatLimitTime = FRAMEGETTIME();
	}

	if( m_iCurWholeChatLimitCount >= m_iWholeChatLimitCount )
	{
		m_dwCurWholeChatLimitTime = FRAMEGETTIME();
		return true;
	}
	return false;
}

bool GameChatInput::IsCheckStealthMode()
{
	if( m_pStage )
	{
		if( m_pStage->GetModeType() == MT_NONE			|| 
			m_pStage->GetModeType() == MT_MYROOM		|| 
			m_pStage->GetModeType() == MT_TRAINING		||
			m_pStage->GetModeType() == MT_HEADQUARTERS	||
			m_pStage->GetModeType() == MT_HOUSE )
		{
			return false;
		}

		if( g_App.IsMeDeveloper() )
		{
			if( !g_BattleRoomMgr.IsBattleRoom() && !g_LadderTeamMgr.IsLadderTeam() && !g_ShuffleRoomMgr.IsShuffleRoom() )
			{
				return true;
			}
		}
	}
	return false;
}

void GameChatInput::OnChatInput( ioEdit *pEdit, DWORD param )
{
	if( !pEdit ) return;

	if( IsCheckStealthMode() )
	{
		g_ChatMgr.SetSystemMsg( STR(2) );
		return;
	}

	if( param == 0 )	// tab key
	{
		pEdit->SetKeyFocus();
		return;
	}

	if( g_Input.IsKeyDown( KEY_LEFTSHIFT ) || g_Input.IsKeyDown( KEY_RIGHTSHIFT ) )
	{
		pEdit->SetKeyFocus();
		ShowChatConv( m_Name, m_OwnerTeam, m_pStage );
		return;
	}

	if( !g_ChatMacroInfo.CheckNormalChatEnable() )
	{
		pEdit->SetKeyFocus();
		if( GetParent() )
			GetParent()->iwm_command( this, IOBN_BTNUP, m_eChatType );
		return;
	}

	ioHashString szChat = pEdit->GetText();
	if( !szChat.IsEmpty() && m_ChatEnableState == CES_WARNING )
	{
		if( szChat == m_szChatWarningText )
		{
			if( GetParent() )
				GetParent()->iwm_command( this, IOBN_BTNUP, m_eChatType );
			return;
		}
	}

	if( !szChat.IsEmpty() && m_ChatEnableState == CES_BLOCK )
	{
		if( szChat == m_szChatBlockText )
		{
			if( GetParent() )
				GetParent()->iwm_command( this, IOBN_BTNUP, m_eChatType );
			return;
		}
	}

	if( szChat.IsEmpty() )
	{
		if( GetParent() )
			GetParent()->iwm_command( this, IOBN_BTNUP, m_eChatType );
	}

	if( ChatMacro::NONE_MACRO != g_ChatMacroMgr.MacroProcess( szChat.c_str() ) )
	{
		// ��ũ�� �Է� �� ä�� �Է�â �ݱ�
		if( GetParent() )
			GetParent()->iwm_command( this, IOBN_BTNUP, m_eChatType );
	}
	else if( !szChat.IsEmpty() )
	{
		if( m_ChatEnableState == CES_BLOCK || m_ChatEnableState == CES_BLOCK_EDITING )
		{
			if( GetParent() )
				GetParent()->iwm_command( this, IOBN_BTNUP, m_eChatType );
			return;
		}

		if( m_pStage )
		{
			m_Name = g_MyInfo.GetPublicID();
			if( g_BattleRoomMgr.IsBattleRoom() && !g_BattleRoomMgr.IsBattleModePlaying() )
				m_OwnerTeam = (TeamType)g_BattleRoomMgr.GetBattleUserTeam( m_Name );
			else if( g_ShuffleRoomMgr.IsShuffleRoom() )
				m_OwnerTeam = (TeamType)g_ShuffleRoomMgr.GetShuffleUserTeam( m_Name );
			else 
			{	
				m_OwnerTeam = m_pStage->GetOwnerTeam();
			}
		}

		ModeType eModeType = MT_NONE;
		int iRoomIndex = -1;

		DWORD dwCustomSoundIndex = 0;
		DWORD dwCustomSoundCRC   = 0;
		DWORD dwCustomSoundSize  = 0;
		g_CustomSoundMgr.GetCustomSoundInfo( szChat, dwCustomSoundIndex, dwCustomSoundCRC, dwCustomSoundSize );

		bool bCanUseEtcItem = false;
		ioUserEtcItem *pUserEtcItem = g_MyInfo.GetUserEtcItem();
		if( pUserEtcItem && pUserEtcItem->IsCanUse( ioEtcItem::EIT_ETC_CUSTOM_SOUND ) )
			bCanUseEtcItem = true;

		bool bCustomSound = false;
		if( dwCustomSoundIndex != 0 && g_CustomSoundMgr.IsCanTimePlayMy() && bCanUseEtcItem )
		{
			bCustomSound = true;
			g_ChatMgr.SetNoChatSound( true );
		}

		if( m_pStage )
		{
			m_pStage->SetChatIcon( m_Name, szChat, g_ChatMgr.IsNoChatSound() , bCustomSound );

			eModeType  = m_pStage->GetModeType();
			iRoomIndex = m_pStage->GetRoomIndex();
		}

		char szChatBuf[MAX_PATH] = "";
		ioHashString szOwnerChat = szChat;
		if( !Setting::IsSlangFilterOther() )
		{
			StringCbCopy( szChatBuf, MAX_PATH, g_SlangUtil.ConvertStringWithPreLine( szChat.c_str() ) );
			szOwnerChat = szChatBuf;
		}
		int iSendMsgLen = strlen( szOwnerChat.c_str() );
		bool bChannelLimit = false;
		switch( m_eChatType )
		{
		case CT_ALL:
			{
				bool bObserverMode = false;
				if( g_BattleRoomMgr.IsBattleRoom() ) 
					bObserverMode = g_BattleRoomMgr.IsObserverUser( g_MyInfo.GetPublicID() );

				if( bObserverMode )
					SetChatMsg( TEAM_NONE, m_Name.c_str(), szOwnerChat.c_str(), g_ChatMgr.IsNoChatSound(), bCustomSound );
				else
					SetChatMsg( m_OwnerTeam, m_Name.c_str(), szOwnerChat.c_str(), g_ChatMgr.IsNoChatSound(), bCustomSound );

				if( iSendMsgLen > 0 )
				{
					if( bObserverMode )
						SendChatMsg( TEAM_NONE, m_Name.c_str(), szChat.c_str(), eModeType, iRoomIndex, bCustomSound );
					else
						SendChatMsg( m_OwnerTeam, m_Name.c_str(), szChat.c_str(), eModeType, iRoomIndex, bCustomSound );
				}

				if( bCanUseEtcItem && g_CustomSoundMgr.IsCanTimePlayMy() )
				{
					g_CustomSoundMgr.SendCustomSoundInfo( dwCustomSoundIndex, dwCustomSoundCRC, dwCustomSoundSize, true, m_OwnerTeam );
					g_CustomSoundMgr.PlayMy( dwCustomSoundIndex, dwCustomSoundCRC );
					if( pUserEtcItem )
						pUserEtcItem->SendUse( ioEtcItem::EIT_ETC_CUSTOM_SOUND );
				}
			}
			break;
		case CT_SERVER_LOBBY:
			{
				if( iSendMsgLen > 0 )
				{
					if( !CheckChatLimit() )
					{
						SetServerLobbyChatMsg( TEAM_BLUE, m_Name.c_str(), szOwnerChat.c_str(), g_ChatMgr.IsNoChatSound() );
						SendServerLobbyChatMsg( m_Name.c_str(), szOwnerChat.c_str() );	
					}
					else
					{
						bChannelLimit = true;
						g_ChatMgr.SetSystemMsg( STR(1) );
					}
				}
			}
			break;
		case CT_TEAM:
			{
				SetTeamChatMsg( m_OwnerTeam, m_Name.c_str(), szOwnerChat.c_str(), g_ChatMgr.IsNoChatSound(), bCustomSound );

				if( iSendMsgLen > 0 )
					SendTeamChatMsg( m_OwnerTeam, m_Name.c_str(), szChat.c_str(), eModeType, iRoomIndex, bCustomSound );

				if( bCanUseEtcItem && g_CustomSoundMgr.IsCanTimePlayMy() )
				{
					g_CustomSoundMgr.SendCustomSoundInfo( dwCustomSoundIndex, dwCustomSoundCRC, dwCustomSoundSize, false, m_OwnerTeam );
					g_CustomSoundMgr.PlayMy( dwCustomSoundIndex, dwCustomSoundCRC );
					if( pUserEtcItem  )
						pUserEtcItem->SendUse( ioEtcItem::EIT_ETC_CUSTOM_SOUND );
				}
			}
			break;
		case CT_PARTY:
			{
				SetPartyChatMsg( m_OwnerTeam, m_Name.c_str(), szOwnerChat.c_str(), g_ChatMgr.IsNoChatSound() );

				if( iSendMsgLen > 0 )
					SendPartyChatMsg( m_OwnerTeam, m_Name.c_str(), szChat.c_str() );
			}
			break;
		case CT_LADDER:
			{
				SetLadderChatMsg( m_Name.c_str(), szOwnerChat.c_str(), g_ChatMgr.IsNoChatSound() );

				if( iSendMsgLen > 0 )
					SendLadderChatMsg( m_Name.c_str(), szChat.c_str() );
			}
			break;
		case CT_WHOLE_SERVER:
			{
				if( !CheckWholeChatLimit() )
				{
					if( Help::IsWholeChatOn() )
					{
						SetWholeServerChatMsg( m_Name.c_str(), szOwnerChat.c_str(), g_ChatMgr.IsNoChatSound() );
						if( iSendMsgLen > 0 )
							SendWholeServerChatMsg( m_Name.c_str(), szOwnerChat.c_str() );
					}
					else
					{
						g_ChatMgr.SetSystemMsg( STR(3) );
					}
				}
				else
				{
					bChannelLimit = true;
					g_ChatMgr.SetSystemMsg( STR(1) );
				}
			}
			break;
		case CT_WHOLE_SERVER_RAINBOW:
			{
				if( !CheckWholeChatLimit() )
				{
					if( Help::IsWholeRainbowChatOn() )
					{
						SetWholeServerRainbowChatMsg( m_Name.c_str(), szOwnerChat.c_str(), g_ChatMgr.IsNoChatSound() );
						if( iSendMsgLen > 0 )
							SendWholeServerRainbowChatMsg( m_Name.c_str(), szOwnerChat.c_str() );
					}
					else
					{
						g_ChatMgr.SetSystemMsg( STR(3) );
					}
				}
				else
				{
					bChannelLimit = true;
					g_ChatMgr.SetSystemMsg( STR(1) );
				}
			}
			break;
		}
		
		if( !bChannelLimit )
		{
			g_AlarmMgr.CheckChatSlang( szChat );
		}

		// ���� ä���� �Է��ߴ�.
		if( !szOwnerChat.IsEmpty() )
		{
			if( GetParent() )
				GetParent()->iwm_command( this, IOBN_BTNUP, m_eChatType );
		}		
	}
}

void GameChatInput::ProcessChangeChatBtn()
{
	if( m_eChatType == CT_WHOLE_SERVER )
	{
		if( !IsWholeChatItem() )
			CheckBtnChatType();
	}
	else if( m_eChatType == CT_WHOLE_SERVER_RAINBOW )
	{
		if( !IsWholeChatRainbowItem() )
			CheckBtnChatType();
	}
}

bool GameChatInput::IsWholeChatItem()
{
	ioUserEtcItem *pUserEtcItem = g_MyInfo.GetUserEtcItem();
	if( pUserEtcItem )
	{
		ioUserEtcItem::ETCITEMSLOT kSlot;
		if( pUserEtcItem->GetEtcItem( ioEtcItem::EIT_ETC_WHOLE_CHAT, kSlot ) )
		{
			if( kSlot.m_iValue1 > 0 )
			{
				return true;
			}
		}
	}
	return false;
}

bool GameChatInput::IsWholeChatRainbowItem()
{
	ioUserEtcItem *pUserEtcItem = g_MyInfo.GetUserEtcItem();
	if( pUserEtcItem )
	{
		ioUserEtcItem::ETCITEMSLOT kSlot;
		if( pUserEtcItem->GetEtcItem( ioEtcItem::EIT_ETC_RAINBOW_WHOLE_CHAT, kSlot ) )
		{
			if( kSlot.m_iValue1 > 0 )
			{
				return true;
			}
		}
	}
	return false;
}

void GameChatInput::OnRender()
{
	ioWnd::OnRender();

	if( ioEdit::m_pKeyFocusEdit == (ioEdit*)FindChildWnd( ID_CHAT_EDIT ) )
	{
		if( m_pFocusBoxFrm )
		{
			m_pFocusBoxFrm->Render( GetDerivedPosX(), GetDerivedPosY() );
		}
	}
}