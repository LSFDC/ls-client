#include "StdAfx.h"
#include "TournamentPacketParser.h"

#include "ioApplication.h"
#include "ioTournamentManager.h"
#include "ioTournamentTeamManager.h"
#include "TournamentNode.h"
#include "ioReservedTask.h"

#include "GUI/LobbyMainWnd.h"
#include "GUI/QuickLobbyMenu.h"
#include "GUI/QuickLobbyWnd.h"

#include "GUI/TournamentMainWnd.h"
#include "GUI/TournamentDetailMainWnd.h"
#include "GUI/CustomTournamentWnd.h"
#include "GUI/CustomTournamentCreateWnd.h"

namespace TournamentPacketParser
{

	bool ProcessTCPPacket( SP2Packet &rkPacket )
	{
		switch( rkPacket.GetPacketID() )
		{
		case STPK_TOURNAMENT_REGULAR_INFO:
			ErrorReport::SetPosition( 3000, 1 );
			OnTournamentRegularInfo( rkPacket );
			ErrorReport::SetPosition( 3000, 2 );
			return true;
		case STPK_TOURNAMENT_MAIN_INFO:
			ErrorReport::SetPosition( 3000, 3 );
			OnTournamentMainInfo( rkPacket );
			ErrorReport::SetPosition( 3000, 4 );
			return true;
		case STPK_TOURNAMENT_TEAM_LIST:
			ErrorReport::SetPosition( 3000, 5 );
			OnTournamentTeamList( rkPacket );
			ErrorReport::SetPosition( 3000, 6 );
			return true;
		case STPK_TOURNAMENT_TEAM_INFO:
			ErrorReport::SetPosition( 3000, 7 );
			OnTournamentTeamInfo( rkPacket );
			ErrorReport::SetPosition( 3000, 8 );
			return true;
		case STPK_TOURNAMENT_TEAM_USER_LIST:
			ErrorReport::SetPosition( 3000, 9 );
			OnTournamentTeamUserList( rkPacket );
			ErrorReport::SetPosition( 3000, 10 );
			return true;
		case STPK_TOURNAMENT_SCHEDULE_INFO:
			ErrorReport::SetPosition( 3000, 11 );
			OnTournamentScheduleInfo( rkPacket );
			ErrorReport::SetPosition( 3000, 12 );
			return true;
		case STPK_TOURNAMENT_ROUND_TEAM_DATA:
			ErrorReport::SetPosition( 3000, 13 );
			OnTournamentRoundTeamData( rkPacket );
			ErrorReport::SetPosition( 3000, 14 );
			return true;
			//
			case STPK_TOURNAMENT_LIST_REQUEST:
			ErrorReport::SetPosition( 3000, 15 );
			OnTournamentListRequest( rkPacket );
			ErrorReport::SetPosition( 3000, 16 );
			return true;
		case STPK_TOURNAMENT_TEAM_CREATE:
			ErrorReport::SetPosition( 3000, 17 );
			OnTournamentCreateTeam( rkPacket );
			ErrorReport::SetPosition( 3000, 18 );
			return true;
		case STPK_TOURNAMENT_TEAM_INVITED:
			ErrorReport::SetPosition( 3000, 19 );
			OnTournamentTeamInvited( rkPacket );
			ErrorReport::SetPosition( 3000, 20 );
			return true;
		case STPK_TOURNAMENT_TEAM_ENTRY_MEMBER:
			ErrorReport::SetPosition( 3000, 21 );
			OnTournamentTeamEntryDelayMember( rkPacket );
			ErrorReport::SetPosition( 3000, 22 );
			return true;
		case STPK_TOURNAMENT_TEAM_ENTRY_APP:
			ErrorReport::SetPosition( 3000, 23 );
			OnTournamentTeamEntryApp( rkPacket );
			ErrorReport::SetPosition( 3000, 24 );
			return true;
		case STPK_TOURNAMENT_TEAM_ENTRY_AGREE:
			ErrorReport::SetPosition( 3000, 25 );
			OnTournamentTeamEntryAgree( rkPacket );
			ErrorReport::SetPosition( 3000, 26 );
			return true;
		case STPK_TOURNAMENT_TEAM_JOIN:
			ErrorReport::SetPosition( 3000, 27 );
			OnTournamentTeamJoin( rkPacket );
			ErrorReport::SetPosition( 3000, 28 );
			return true;
		case STPK_TOURNAMENT_TEAM_LEAVE:
			ErrorReport::SetPosition( 3000, 29 );
			OnTournamentTeamLeave( rkPacket );
			ErrorReport::SetPosition( 3000, 30 );
			return true;
		case STPK_TOURNAMENT_END_PROCESS:
			ErrorReport::SetPosition( 3000, 31 );
			OnTournamentEndProcess( rkPacket );
			ErrorReport::SetPosition( 3000, 32 );
			return true;
		case STPK_TOURNAMENT_BATTLE_INVITE:
			ErrorReport::SetPosition( 3000, 33 );
			OnTournamentBattleInvite( rkPacket );
			ErrorReport::SetPosition( 3000, 34 );
			return true;
		case STPK_TOURNAMENT_TEAM_POSITION:
			ErrorReport::SetPosition( 3000, 35 );
			OnTournamentTeamPosition( rkPacket );
			ErrorReport::SetPosition( 3000, 36 );
			return true;
		case STPK_TOURNAMENT_REWARD_DATA:
			ErrorReport::SetPosition( 3000, 37 );
			OnTournamentRewardData( rkPacket );
			ErrorReport::SetPosition( 3000, 38 );
			return true;
		case STPK_TOURNAMENT_ROOM_LIST:
			ErrorReport::SetPosition( 3000, 39 );
			OnTournamentRoomList( rkPacket );
			ErrorReport::SetPosition( 3000, 40 );
			return true;
		case STPK_TOURNAMENT_CUSTOM_CREATE:
			ErrorReport::SetPosition( 3000, 41 );
			OnTournamentCustomCreate( rkPacket );
			ErrorReport::SetPosition( 3000, 42 );
			return true;
		case STPK_TOURNAMENT_TEAM_ALLOCATE_LIST:
			ErrorReport::SetPosition( 3000, 43 );
			OnTournamentTeamAllocateList( rkPacket );
			ErrorReport::SetPosition( 3000, 44 );
			return true;
		case STPK_TOURNAMENT_TEAM_ALLOCATE_DATA:
			ErrorReport::SetPosition( 3000, 45 );
			OnTournamentTeamAllocateData( rkPacket );
			ErrorReport::SetPosition( 3000, 46 );
			return true;
		case STPK_TOURNAMENT_JOIN_CONFIRM_CHECK:
			ErrorReport::SetPosition( 3000, 47 );
			OnTournamentTeamJoinConfirmCheck( rkPacket );
			ErrorReport::SetPosition( 3000, 48 );
			return true;
		case STPK_TOURNAMENT_JOIN_CONFIRM_REQUEST:
			ErrorReport::SetPosition( 3000, 49 );
			OnTournamentTeamJoinConfirmRequest( rkPacket );
			ErrorReport::SetPosition( 3000, 50 );
			return true;
		case STPK_TOURNAMENT_JOIN_CONFIRM_LIST:
			ErrorReport::SetPosition( 3000, 51 );
			OnTournamentTeamJoinConfirmList( rkPacket );
			ErrorReport::SetPosition( 3000, 52 );
			return true;
		case STPK_TOURNAMENT_ANNOUNCE_CHANGE:
			ErrorReport::SetPosition( 3000, 53 );
			OnTournamentAnnounceChange( rkPacket );
			ErrorReport::SetPosition( 3000, 54 );
			return true;
		case STPK_TOURNAMENT_TOTAL_TEAM_LIST:
			ErrorReport::SetPosition( 3000, 55 );
			OnTournamentTotalTeamList( rkPacket );
			ErrorReport::SetPosition( 3000, 56 );
			return true;
		case STPK_TOURNAMENT_JOIN_CONFIRM_COMMAND:
			ErrorReport::SetPosition( 3000, 57 );
			OnTournamentTeamJoinConfirmCommand( rkPacket );
			ErrorReport::SetPosition( 3000, 58 );
			return true;
		case STPK_TOURNAMENT_CUSTOM_STATE_START:
			ErrorReport::SetPosition( 3000, 59 );
			OnTournamentCustomStateStart( rkPacket );
			ErrorReport::SetPosition( 3000, 60 );
			return true;
		case STPK_TOURNAMENT_CUSTOM_REWARD_LIST:
			ErrorReport::SetPosition( 3000, 61 );
			OnTournamentCustomRewardList( rkPacket );
			ErrorReport::SetPosition( 3000, 62 );
			return true;
		case STPK_TOURNAMENT_CUSTOM_REWARD_BUY:
			ErrorReport::SetPosition( 3000, 63 );
			OnTournamentCustomRewardBuy( rkPacket );
			ErrorReport::SetPosition( 3000, 64 );
			return true;
		case STPK_TOURNAMENT_CUSTOM_REWARD_DATA:
			ErrorReport::SetPosition( 3000, 65 );
			OnTournamentCustomRewardData( rkPacket );
			ErrorReport::SetPosition( 3000, 66 );
			return true;
		case STPK_TOURNAMENT_CHEER_DECISION:
			ErrorReport::SetPosition( 3000, 67 );
			OnTournamentCheerDecision( rkPacket );
			ErrorReport::SetPosition( 3000, 68 );
			return true;
		case STPK_TOURNAMENT_CHEER_LIST:
			ErrorReport::SetPosition( 3000, 69 );
			OnTournamentCheerList( rkPacket );
			ErrorReport::SetPosition( 3000, 70 );
			return true;
		}
		return false;
	}

	void OnTournamentRegularInfo( SP2Packet &rkPacket )
	{
		g_TournamentMgr.ApplyTournamentRegularInfo( rkPacket );

		if( g_App.GetConnectedMoveMode() == ioApplication::CMM_LOBBY )
		{
			LobbyMainWnd *pLobbyMainWnd = dynamic_cast<LobbyMainWnd*>(g_GUIMgr.FindWnd( LOBBY_MAIN_WND ));
			if( pLobbyMainWnd )
				pLobbyMainWnd->RefreshTab();
		}
		else
		{
			QuickLobbyMenu* pMenu = (QuickLobbyMenu*)( g_GUIMgr.FindWnd( LOBBY_QUICK_MENU ) );
			if( pMenu )
			{
				pMenu->RefreshTournamentTab();
			}			
		}
	}

	void OnTournamentMainInfo( SP2Packet &rkPacket )
	{
		g_TournamentMgr.ApplyTournamentMainInfo( rkPacket );
	}

	void OnTournamentTeamList( SP2Packet &rkPacket )
	{
		g_MyInfo.ApplyTournamentTeamList( rkPacket );
	}

	void OnTournamentTeamInfo( SP2Packet &rkPacket )
	{
		g_TournamentTeamMgr.SetFullData( rkPacket );
	}

	void OnTournamentTeamUserList( SP2Packet &rkPacket )
	{
		DWORD dwTeamIndex;
		rkPacket >> dwTeamIndex;

		ioUserTournament *pUserTournament = g_MyInfo.GetTournament();
		if( pUserTournament )
		{
			pUserTournament->ApplyUserData( dwTeamIndex, rkPacket );
		}
	}

	void OnTournamentScheduleInfo( SP2Packet &rkPacket )
	{
		g_TournamentMgr.ApplyTournamentScheduleInfo( rkPacket );
	}

	void OnTournamentRoundTeamData( SP2Packet &rkPacket )
	{
		g_TournamentMgr.ApplyTournamentRoundTeamData( rkPacket );
	}

	void OnTournamentListRequest( SP2Packet &rkPacket )
	{
		if( g_App.GetConnectedMoveMode() == ioApplication::CMM_LOBBY )
		{
			LobbyMainWnd *pLobbyMainWnd = dynamic_cast< LobbyMainWnd* >( g_GUIMgr.FindWnd( LOBBY_MAIN_WND ) );
			if( pLobbyMainWnd && pLobbyMainWnd->IsShow() )
				pLobbyMainWnd->ApplyTournamentList( rkPacket );
		}
		else
		{
			QuickLobbyWnd* pWnd = (QuickLobbyWnd*)( g_GUIMgr.FindWnd( LOBBY_QUICK_WND ) );
			if( pWnd && pWnd->IsShow() )
				pWnd->ApplyTournamentList( rkPacket );
		}
	}

	void OnTournamentCreateTeam( SP2Packet &rkPacket )
	{
		TCPNetwork::MouseBusy( false );

		int iResult;
		rkPacket >> iResult;

		switch( iResult )
		{
		case TOURNAMENT_TEAM_CREATE_OK:
			{
				ioUserTournament *pTournament = g_MyInfo.GetTournament();
				if( pTournament )
				{
					pTournament->CreateTeamData( rkPacket );
				}
			}
			break;
		case TOURNAMENT_TEAM_CREATE_NAME_ALREADY:
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, "������ �� �̸��� �����մϴ�" );
			}
			break;
		case TOURNAMENT_TEAM_CREATE_ALREADY_TEAM1:
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, "�̹� ���� �����մϴ�" );
			}
			break;
		case TOURNAMENT_TEAM_CREATE_ALREADY_TEAM2:
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, "�̹� ���� �ҼӵǾ��ֽ��ϴ�" );
			}
			break;
		case TOURNAMENT_TEAM_CREATE_END_LEAGUE:
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, "��ȸ�� �� ���� �Ⱓ�� ����Ǿ����ϴ�" );
			}
			break;
		case TOURNAMENT_TEAM_CREATE_FAILED:
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, "�̹� LSNC�������� �����Ͽ��⿡# �ش��ȸ�� ������ �Ұ����մϴ�" );
			}
		}
	}

	void OnTournamentEndProcess( SP2Packet &rkPacket )
	{
		DWORD dwTourIndex;
		rkPacket >> dwTourIndex;

		ioUserTournament *pUserTournament = g_MyInfo.GetTournament();
		if( pUserTournament == NULL ) return;

		TournamentDetailMainWnd* pMainWnd = dynamic_cast<TournamentDetailMainWnd*>( g_GUIMgr.FindWnd( TOURNAMENT_INFO ) );
		if( pMainWnd && pMainWnd->IsShow() )
			pMainWnd->HideWnd();

		pUserTournament->TournamentEndDeleteTeam( dwTourIndex );

		TournamentNode* pNode = g_TournamentMgr.GetTournament( dwTourIndex, false );
		if( pNode && pNode->GetType() == TournamentNode::TYPE_REGULAR )
			pNode->ClearUnAllocateTeamData();
	}


	void OnTournamentTeamInvited( SP2Packet &rkPacket )
	{
		TournamentTeamInvitedWnd *pInvited = dynamic_cast< TournamentTeamInvitedWnd * >( g_GUIMgr.FindWnd( TOURNAMENT_TEAM_INVITED_WND ) );
		if( pInvited )
		{
			pInvited->SetInvitedInfo( rkPacket );
		}
	}

	void OnTournamentTeamEntryDelayMember( SP2Packet &rkPacket )
	{
		TournamentTeamEntryDelayUserWnd *pDelayUserWnd = dynamic_cast<TournamentTeamEntryDelayUserWnd *>(g_GUIMgr.FindWnd( TOURNAMENT_TEAM_ENTRY_DELAY_USER_WND ) );
		if( pDelayUserWnd )
		{
			pDelayUserWnd->SetEntryDelayUser( rkPacket );
		}
	}

	void OnTournamentTeamEntryApp( SP2Packet &rkPacket )
	{
		TCPNetwork::MouseBusy( false );

		int iResult;
		DWORD dwTourIndex, dwTeamIndex;
		rkPacket >> iResult >> dwTourIndex >> dwTeamIndex;

		switch( iResult )
		{
		case TOURNAMENT_TEAM_ENTRY_APP_OK:
			{
				TeamGlobalData *pTeamData = g_TournamentTeamMgr.GetTeamData( dwTeamIndex, false );
				if( pTeamData )
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, "[%s]���� ���� ��û�� �Ϸ�Ǿ����ϴ�", pTeamData->GetTeamName().c_str() );
				}
				else
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, "���� ��û�� �Ϸ�Ǿ����ϴ�" );
				}
			}
			break;
		case TOURNAMENT_TEAM_ENTRY_APP_FAILED:
			{
				// �ӽ� ȸ��
				g_App.OpenFormalityEntryPage();
			}
			break;
		case TOURNAMENT_TEAM_ENTRY_APP_ALREADY_TEAM1:
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, "��ȸ�� �̹� ���� �����մϴ�" );
			}
			break;
		case TOURNAMENT_TEAM_ENTRY_APP_ALREADY_TEAM2:
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, "�̹� ��û�� ���Դϴ�" );
			}
			break;
		case TOURNAMENT_TEAM_ENTRY_APP_FULL_WAITING:
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, "���� ��� �ο��� ���� á���ϴ�" );
			}
			break;
		case TOURNAMENT_TEAM_ENTRY_APP_BAN:
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, "�̹� LSNC�������� �����Ͽ��⿡#�ش��ȸ�� ������ �Ұ����մϴ�" );
			}
			break;
		}
	}

	void OnTournamentTeamEntryAgree( SP2Packet &rkPacket )
	{
		int iResult;
		rkPacket >> iResult;

		switch( iResult )
		{
		case TOURNAMENT_TEAM_ENTRY_AGREE_OK:
			{
				DWORD dwTeamIndex;
				rkPacket >> dwTeamIndex;
				ioUserTournament *pUserTournament = g_MyInfo.GetTournament();
				if( pUserTournament )
				{
					pUserTournament->SetTeamUserAdd( dwTeamIndex, rkPacket );
				}
			}
			break;
		case TOURNAMENT_TEAM_ENTRY_AGREE_FAILED1:
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, "���� ��û�� ��ҵ� �����Դϴ�" );
			}
			break;
		case TOURNAMENT_TEAM_ENTRY_AGREE_FAILED2:
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, "��ȸ���� ������ �����մϴ�" );
			}
			break;
		case TOURNAMENT_TEAM_ENTRY_AGREE_ALREAY_TEAM:
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, "�̹� ���� �ִ� �����Դϴ�" );
			}
			break;
		case TOURNAMENT_TEAM_ENTRY_AGREE_FULL_JOINER:
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, "���̻� ������ ���� �� �����ϴ�" );
			}
			break;
		case TOURNAMENT_TEAM_ENTRY_AGREE_CAMP_NONE:
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, "���� ������ ������ ���� �����մϴ�" );
			}
			break;
		case TOURNAMENT_TEAM_ENTRY_AGREE_END_APP:
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, "������ �Ⱓ�� ����Ǿ����ϴ�." );
			}
			break;
		}
	}

	void OnTournamentTeamJoin( SP2Packet &rkPacket )
	{
		ioUserTournament *pUserTournament = g_MyInfo.GetTournament();
		if( pUserTournament )
		{
			pUserTournament->JoinTeamData( rkPacket );
		}
	}

	void OnTournamentTeamLeave( SP2Packet &rkPacket )
	{
		int iResult;
		rkPacket >> iResult;

		switch( iResult )
		{
		case TOURNAMENT_TEAM_LEAVE_OK:
			{
				ioUserTournament *pUserTournament = g_MyInfo.GetTournament();
				if( pUserTournament == NULL ) return;

				bool bTeamDelete;
				DWORD dwSenderIndex, dwTeamIndex, dwLeaveUserIndex;
				rkPacket >> dwSenderIndex >> dwTeamIndex >> dwLeaveUserIndex >> bTeamDelete;

				if( dwSenderIndex == g_MyInfo.GetUserIndex() )
				{
					TCPNetwork::MouseBusy( false );
				}

				if( g_MyInfo.GetUserIndex() == dwLeaveUserIndex )
				{
					if( bTeamDelete )
					{					
						// �� ��ü UI ǥ��
						TournamentTeamEventWnd *pTeamEventWnd = dynamic_cast< TournamentTeamEventWnd * >( g_GUIMgr.FindWnd( TOURNAMENT_TEAM_EVENT_WND ) );
						if( pTeamEventWnd )
						{
							TeamData &rkTeamData = pUserTournament->GetTeamData( dwTeamIndex );
							pTeamEventWnd->SetTeamEventData( rkTeamData.m_dwTourIndex, rkTeamData.m_dwTeamIndex, TournamentTeamEventWnd::EVENT_TYPE_TEAM_DELETE );
						}
					}
					else 
					{
						// �� Ż�� UI ǥ��
						TournamentTeamEventWnd *pTeamEventWnd = dynamic_cast< TournamentTeamEventWnd * >( g_GUIMgr.FindWnd( TOURNAMENT_TEAM_EVENT_WND ) );
						if( pTeamEventWnd )
						{
							TeamData &rkTeamData = pUserTournament->GetTeamData( dwTeamIndex );
							pTeamEventWnd->SetTeamEventData( rkTeamData.m_dwTourIndex, rkTeamData.m_dwTeamIndex, TournamentTeamEventWnd::EVENT_TYPE_TEAM_LEAVE );
						}
					}
				}
				else if( dwSenderIndex != g_MyInfo.GetUserIndex() )
				{
					// ������ Ż�� �Ǿ��� �� ǥ��?
					TournamentTeamAlarmWnd *pTeamAlarmWnd = dynamic_cast< TournamentTeamAlarmWnd * >( g_GUIMgr.FindWnd( TOURNAMENT_TEAM_ALARM_WND ) );
					if( pTeamAlarmWnd )
					{
						pTeamAlarmWnd->SetAlarmData( dwTeamIndex, dwLeaveUserIndex, TournamentTeamAlarmWnd::ALARM_TYPE_LEAVE_USER );
					}
				}

				if( bTeamDelete || 
					dwLeaveUserIndex == g_MyInfo.GetUserIndex() )
				{
					// ���� ������
					pUserTournament->DeleteTeam( dwTeamIndex );
				}
				else
				{
					// ���� ����Ʈ ����
					pUserTournament->LeaveTeamUser( dwTeamIndex, dwLeaveUserIndex );
				}
			}
			break;
		case TOURNAMENT_TEAM_LEAVE_FAILED_STATE:
			{
				TCPNetwork::MouseBusy( false );
				g_GUIMgr.SetMsgBox( MB_OK, NULL, "��ȸ�� ���۵Ǿ� Ż�� �Ұ����մϴ�" );
			}
			break;
		case TOURNAMENT_TEAM_LEAVE_FAILED_NON_MEMBER:
			{
				TCPNetwork::MouseBusy( false );
				g_GUIMgr.SetMsgBox( MB_OK, NULL, "ȸ�� ������ ��ġ���� �ʽ��ϴ�" );
			}
			break;
		}
	}

	void OnTournamentBattleInvite( SP2Packet &rkPacket )
	{
		DWORD dwBattleIndex, dwBlueIndex, dwRedIndex, dwInviteTime;
		rkPacket >> dwBattleIndex >> dwBlueIndex >> dwRedIndex >> dwInviteTime;
		TournamentBattleInviteWnd::SetTournamentBattle( dwBattleIndex, dwBlueIndex, dwRedIndex, dwInviteTime );
	}

	void OnTournamentTeamPosition( SP2Packet &rkPacket )
	{
		BYTE TourPos;
		SHORT Position;
		DWORD dwTeamIndex;
		rkPacket >> dwTeamIndex >> Position >> TourPos;

		LOG.PrintTimeAndLog(0, "%s 1�� - %d, %d, %d", __FUNCTION__, dwTeamIndex, Position, TourPos );

		TeamGlobalData *pTeamGlobalData = g_TournamentTeamMgr.GetTeamData( dwTeamIndex, false );
		if( pTeamGlobalData )
		{
			BYTE PrevTourPos = pTeamGlobalData->GetTourPos();

			pTeamGlobalData->SetPosition( Position );
			pTeamGlobalData->SetTourPos( TourPos );

			ioUserTournament *pUserTournament = g_MyInfo.GetTournament();
			if( pUserTournament && pUserTournament->IsAlreadyTeam( dwTeamIndex ) )
			{
				// �¸� & ������ �˸�
				TournamentNode *pTournament = g_TournamentMgr.GetTournament( pTeamGlobalData->GetTourIndex() );
				if( pTournament )
				{
					// �� ���� ����ǥ�� ��� ��������!!
					pTournament->ChangeRoundTeamData( pTeamGlobalData->GetTeamIndex(), pTeamGlobalData->GetTeamName(), pTeamGlobalData->GetCampPos(), pTeamGlobalData->GetTourPos(), pTeamGlobalData->GetPosition() );

					if( g_BattleRoomMgr.IsTournamentModeType() )
					{
						if( g_BattleRoomMgr.GetTournamentBlueIndex() == dwTeamIndex || g_BattleRoomMgr.GetTournamentRedIndex()  == dwTeamIndex )
							return;
					}

					if( PrevTourPos <= 0 )
						return;
					
					BYTE NextTourPos = TourPos - static_cast<BYTE>( TournamentNode::STATE_TOURNAMENT ) + 1;
					int iRoundCount = Help::TournamentCurrentRoundWithTeam( pTournament->GetTournamentStartTeamCount(), NextTourPos );
					TournamentUnearnedWinAlarmWnd* pWnd = dynamic_cast<TournamentUnearnedWinAlarmWnd*>( g_GUIMgr.FindWnd( TOURNAMENT_UNEARNED_WIN_ALARM_WND ) );
					if( pWnd )
					{
						pWnd->SetTournamentData( pTournament->GetTournamentTitle(), iRoundCount,  pTournament->GetType() );
						pWnd->ShowWnd();
					}

					LOG.PrintTimeAndLog(0, "%s 2�� - %d, %d, %d", __FUNCTION__, pTournament->GetTournamentStartTeamCount(), iRoundCount, NextTourPos );
				}
			}
		}
	}

	void OnTournamentRewardData( SP2Packet &rkPacket )
	{
		BYTE AttainTourPos;
		DWORD dwStartDate, dwCheerPeso;
		int iMyCampPos, iWinCampPos, iLadderBonusPeso, iLadderRank, iLadderPoint;
		ioHashString szWinnerTeamName;

		rkPacket >> dwStartDate >> AttainTourPos >> iMyCampPos >> iWinCampPos >> iLadderBonusPeso >> iLadderRank >> iLadderPoint >> dwCheerPeso >> szWinnerTeamName;

		ioTaskTournamentReward* task = new ioTaskTournamentReward;
		task->eType				= RESERVEDCMD_TYPE_TOURNAMENT;
		task->eWhen				= RESERVEDCMD_WHEN_RECV_TOURNAEMNT_REWARD;
		task->AttainTourPos		= AttainTourPos;
		task->dwStartDate		= dwStartDate;
		task->dwCheerPeso		= dwCheerPeso;
		task->iMyCampPos		= iMyCampPos;
		task->iWinCampPos		= iWinCampPos;
		task->iLadderBonusPeso	= iLadderBonusPeso;
		task->iLadderRank		= iLadderRank;
		task->szWinnerTeamName	= szWinnerTeamName;
		task->iLadderPoint		= iLadderPoint;

		g_RserveTask.AddReservedTask( task );

		LOG.PrintTimeAndLog( 0 , "%s : Accept Tournament Data ", __FUNCTION__ );
	}

	void OnTournamentRoomList( SP2Packet &rkPacket )
	{
		TournamentRoomListWnd *pTournamentRoomList = dynamic_cast< TournamentRoomListWnd * >( g_GUIMgr.FindWnd( TOURNAMENT_ROOM_LIST_WND ) );
		if( pTournamentRoomList )
		{
			if( pTournamentRoomList->IsShow() )
			{
				pTournamentRoomList->SetRoomList( rkPacket );
			}
		}
	}

	void OnTournamentCustomCreate( SP2Packet &rkPacket )
	{
		int iResult;
		rkPacket >> iResult;

		TCPNetwork::MouseBusy( false );
		switch( iResult )
		{
		case TOURNAMENT_CUSTOM_CREATE_OK:
			{
				DWORD dwTourIndex;
				rkPacket >> dwTourIndex;

				CustomTournamentCreateResultWnd* pWnd = dynamic_cast<CustomTournamentCreateResultWnd*>( g_GUIMgr.FindWnd(CUSTOM_TOURNAMENT_CREATE_RESULT_WND) );
				if( pWnd )
					pWnd->SetCustomTournamentData( dwTourIndex );
			}
			break;
		case TOURNAMENT_CUSTOM_CREATE_NAME_FAILED:
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, "����� �� ���� ��ȸ���Դϴ�" );
			}
			break;
		case TOURNAMENT_CUSTOM_CREATE_EXCEPTION:
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, "��ȸ ������ �����Ͽ����ϴ�#����� �������� ������ ��ȯ�˴ϴ�" );
			}
			break;
		}
	}

	void OnTournamentTeamAllocateList( SP2Packet &rkPacket )
	{
		DWORD dwTourIndex;
		int iCurPage;
		DWORD dwUnAllocateTeamCount;
		int iCurTeamCnt;

		TCPNetwork::MouseBusy( false );

		rkPacket >> dwTourIndex >> iCurPage >> dwUnAllocateTeamCount >> iCurTeamCnt;
		TournamentNode* pNode = g_TournamentMgr.GetTournament( dwTourIndex, false );
		if( !pNode )
			return;

		pNode->SetUnAllocateTotalCount( dwUnAllocateTeamCount );
		if( pNode->GetType() == TournamentNode::TYPE_REGULAR )
		{
			DWORD dwTotalCheerCount;
			int iMyTeamPage;
			rkPacket >> dwTotalCheerCount;
			rkPacket >> iMyTeamPage;

			ioUserTournament* pUserTournament = g_MyInfo.GetTournament();
			if( pUserTournament )					
				pUserTournament->UpdateCheerTeamPage( dwTourIndex, iMyTeamPage );		

			pNode->SetReguralTotalCheerCount( dwTotalCheerCount );
		}

		for( int i = 0; i < iCurTeamCnt; ++i )
		{
			DWORD dwTeamIdx = 0;
			ioHashString szTeamName;
			BYTE iCampPos = CAMP_NONE;
			int iCheerPoint = 0;
			rkPacket >> dwTeamIdx;

			if( pNode->GetType() == TournamentNode::TYPE_REGULAR )
			{
				rkPacket >> iCampPos;
				rkPacket >> iCheerPoint;
				rkPacket >> szTeamName;
			}
			else
			{
				rkPacket >> szTeamName;
			}

			pNode->AddUnAllocateTeamData( dwTeamIdx, szTeamName, iCurPage, iCurPage * TournamentHelp::PAGE_TEAM_MAX + i, iCampPos, iCheerPoint );
		}
	}

	void OnTournamentTeamAllocateData( SP2Packet &rkPacket )
	{
		int iResult;
		DWORD dwTourIndex;
		rkPacket >> iResult >> dwTourIndex;

		TCPNetwork::MouseBusy( false );
		switch( iResult )
		{
		case TOURNAMENT_TEAM_ALLOCATE_OK:	// ���� ����
			{
				CustomTournamentAllocateResultWnd* pWnd = dynamic_cast<CustomTournamentAllocateResultWnd*>( g_GUIMgr.FindWnd( CUSTOM_TOURNAMENT_ALLOCATE_RESULT_WND ) );
				if( pWnd )			
					pWnd->SetCustomTournamentData( dwTourIndex );

				TournamentDetailMainWnd* pMainWnd = dynamic_cast<TournamentDetailMainWnd*>( g_GUIMgr.FindWnd( TOURNAMENT_INFO ) );
				if( pMainWnd )
					pMainWnd->SetAllocated( dwTourIndex );
			}
			break;
		case TOURNAMENT_TEAM_ALLOCATE_END:	// ��ȸ�� ����Ǿ���
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, " ����� ��ȸ�Դϴ�." );	
			}
			break;
		}
	}

	void OnTournamentTeamJoinConfirmCheck( SP2Packet &rkPacket )
	{
		if( g_App.GetConnectedMoveMode() == ioApplication::CMM_LOBBY )
		{
			LobbyMainWnd *pLobbyMainWnd = dynamic_cast< LobbyMainWnd* >( g_GUIMgr.FindWnd( LOBBY_MAIN_WND ) );
			if( pLobbyMainWnd )
				pLobbyMainWnd->ApplyTournamentConfirmCheck( rkPacket );	
		}
		else
		{
			QuickLobbyWnd* pWnd = (QuickLobbyWnd*)( g_GUIMgr.FindWnd( LOBBY_QUICK_WND ) );
			if( pWnd )
				pWnd->ApplyTournamentConfirmCheck( rkPacket );
		}
	}

	void OnTournamentTeamJoinConfirmRequest( SP2Packet &rkPacket )
	{
		if( g_App.GetConnectedMoveMode() == ioApplication::CMM_LOBBY )
		{
			LobbyMainWnd *pLobbyMainWnd = dynamic_cast< LobbyMainWnd* >( g_GUIMgr.FindWnd( LOBBY_MAIN_WND ) );
			if( pLobbyMainWnd )
				pLobbyMainWnd->ApplyTournamentConfirmRequest( rkPacket );
		}
		else
		{
			QuickLobbyWnd* pWnd = (QuickLobbyWnd*)( g_GUIMgr.FindWnd( LOBBY_QUICK_WND ) );
			if( pWnd )
				pWnd->ApplyTournamentConfirmRequest( rkPacket );		
		}
	}

	void OnTournamentTeamJoinConfirmList( SP2Packet &rkPacket )
	{
		DWORD dwTourIdx;
		rkPacket >> dwTourIdx;

		TournamentNode* pNode = g_TournamentMgr.GetTournament( dwTourIdx, false );	
		if( pNode )
		{
			pNode->ApplyTournamentJoinConfirmList( rkPacket );
		}
		else
		{
			g_TournamentMgr.CreateTournament( dwTourIdx );
			TournamentNode* pNode = g_TournamentMgr.GetTournament( dwTourIdx, false );
			pNode->ApplyTournamentJoinConfirmList( rkPacket );
		}

		CustomTournamentDelayCreateTeamListWnd* pWnd = dynamic_cast<CustomTournamentDelayCreateTeamListWnd*>( g_GUIMgr.FindWnd( TOURNAMENT_DELAY_CREATE_TEAM_LIST_WND ) );
		if( pWnd )
			pWnd->SetAutoShow( dwTourIdx, true );
	}

	void OnTournamentAnnounceChange( SP2Packet &rkPacket )
	{
		int iResult;
		DWORD dwTourIdx;
		rkPacket >> iResult >> dwTourIdx;

		switch( iResult )
		{
		case TOURNAMENT_ANNOUNCE_CHANGE_OK:
			{
				TournamentNode* pNode = g_TournamentMgr.GetTournament( dwTourIdx, false );
				if( pNode )
					pNode->ApplyCustomAnnounce( rkPacket );
			}
			break;
		case TOURNAMENT_ANNOUNCE_CHANGE_CLOSE :
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, " ����� ��ȸ�Դϴ�." );
			}
			break;
		case TOURNAMENT_ANNOUNCE_CHANGE_FAILED:
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, "��ȸ�� �����ڰ� �ƴմϴ�." );
			}
			break;
		}
	}

	void OnTournamentTotalTeamList( SP2Packet &rkPacket )
	{
		CustomTournamentEntryListWnd* pWnd = dynamic_cast<CustomTournamentEntryListWnd*>( g_GUIMgr.FindWnd( CUSTOM_TOURNAMENT_ENTRY_LIST_WND ) );
		if( pWnd && pWnd->IsShow() )
			pWnd->ApplyCustomTournamentEntryList( rkPacket );
	}

	void OnTournamentTeamJoinConfirmCommand( SP2Packet &rkPacket )
	{
		int iResult;
		DWORD dwTourIdx;
		rkPacket >> iResult >> dwTourIdx;

		switch( iResult )
		{
		case TOURNAMENT_JOIN_CONFIRM_COMMAND_INVITE_OK:
			{			
				ioHashString szUserID;
				rkPacket >> szUserID;			
				g_GUIMgr.SetMsgBox( MB_OK, NULL, "%s�Կ��� �ʴ븦 �߼� �Ͽ����ϴ�.", szUserID.c_str() );
			}
			break;
		case TOURNAMENT_JOIN_CONFIRM_COMMAND_INVITE_OFFLINE:
			{
				ioHashString szUserID;
				rkPacket >> szUserID;			
				g_GUIMgr.SetMsgBox( MB_OK, NULL, "%s ���� �������� �����Դϴ�.", szUserID.c_str() );

			}
			break;
		case TOURNAMENT_JOIN_CONFIRM_COMMAND_INVITED:
			{
				g_MyInfo.RemoveTournamentConfirmRequest( dwTourIdx );
				g_MyInfo.AddTournamentConfirm( dwTourIdx );
				TournamentJoinConfirmAgreeWnd* pWnd = dynamic_cast<TournamentJoinConfirmAgreeWnd*>( g_GUIMgr.FindWnd( TOURNAMENT_JOIN_CONFIRM_AGREE_WND ) );
				if( pWnd )
					pWnd->SetTournament( dwTourIdx, TournamentJoinConfirmAgreeWnd::TOUR_JOIN_INVITED );
			}
			break;
		}
	}

	void OnTournamentCustomStateStart( SP2Packet &rkPacket )
	{
		int iResult;
		DWORD dwTourIdx;
		rkPacket >> iResult >> dwTourIdx;

		TCPNetwork::MouseBusy( false );
		switch( iResult )
		{
		case TOURNAMENT_CUSTOM_STATE_START_OK:
			{
				BYTE State;
				DWORD dwStateStartDate;
				DWORD dwStateEndDate;
				rkPacket >> State >> dwStateStartDate >> dwStateEndDate;
				TournamentNode* pNode = g_TournamentMgr.GetTournament( dwTourIdx, false );
				if( pNode )
					pNode->SetCustomState( State, dwStateStartDate, dwStateEndDate );
			}
			break;
		case TOURNAMENT_CUSTOM_STATE_START_CLOSE:
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, "�̹� ����� ��ȸ�Դϴ�." );
			}
			break;
		case TOURNAMENT_CUSTOM_STATE_START_FAILED:
			{			
				g_GUIMgr.SetMsgBox( MB_OK, NULL, "��ȸ�� �����ڰ� �ƴϰų� # ��ȸ ���� ���ɽð��� �ƴմϴ�." );
			}
			break;
		}
	}

	void OnTournamentCustomRewardList( SP2Packet &rkPacket )
	{
		DWORD dwTourIndex;
		rkPacket >> dwTourIndex;

		TournamentNode *pTournament = g_TournamentMgr.GetTournament( dwTourIndex, false );
		if( pTournament )
		{
			pTournament->ApplyCustomRewardList( rkPacket );
		}
	}

	void OnTournamentCustomRewardBuy( SP2Packet &rkPacket )
	{
		int iResult;
		DWORD dwTourIdx;
		rkPacket >> iResult >> dwTourIdx;

		TCPNetwork::MouseBusy( false );
		switch( iResult )
		{
		case TOURNAMENT_CUSTOM_REWARD_BUY_OK:
			{
				int iTotalPrice;
				rkPacket >> iTotalPrice;

				TournamentNode* pNode = g_TournamentMgr.GetTournament( dwTourIdx, false );
				if( pNode )
					pNode->SendCustomRewardDataSync();

				g_MyInfo.UseEtcCoinMoney( ioEtcItem::EIT_ETC_TOURNAMENT_COIN, iTotalPrice );
				g_GUIMgr.SetMsgBox( MB_OK, NULL, "��ȸ���� �߰��� �Ϸ� �Ǿ����ϴ�." );
			}
			break;
		case TOURNAMENT_CUSTOM_REWARD_BUY_CLOSE:
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, "�̹� ����� ��ȸ�Դϴ�." );
			}
			break;
		case TOURNAMENT_CUSTOM_REWARD_BUY_EMPTY_SLOT:
			{
				CustomTournamentRewardSelectWnd* pWnd = dynamic_cast<CustomTournamentRewardSelectWnd*>( g_GUIMgr.FindWnd( CUSTOM_TOURNAMENT_REWARD_SELECT_WND ) );
				if( pWnd && pWnd->IsShow() )
					pWnd->HideWnd();

				g_GUIMgr.SetMsgBox( MB_OK, NULL, "���� ������ ���� �ִ� ������ �ʰ��Ͽ����ϴ�." );
			}
			break;
		case TOURNAMENT_CUSTOM_REWARD_BUY_NOT_OWNER:
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, "��ȸ�� �����ڰ� �ƴմϴ�." );
			}
			break;
		case TOURNAMENT_CUSTOM_REWARD_BUY_COIN_LACK:
			{
				TournamentCoinItemNeedPopup* pWnd = dynamic_cast<TournamentCoinItemNeedPopup*>( g_GUIMgr.FindWnd( TOURNAMENT_COIN_NEED_WND ) );
				if( pWnd )
					pWnd->ShowWnd();
			}
			break;
		}
	}

	void OnTournamentCustomRewardData( SP2Packet &rkPacket )
	{
		ioHashString szTitle;
		int iTourPos;
		short MaxRound;
		int iEtcRewradA, iEtcRewradB, iEtcRewradC, iEtcRewradD;
		rkPacket >> szTitle >> iTourPos >> MaxRound >> iEtcRewradA >> iEtcRewradB >> iEtcRewradC >> iEtcRewradD;
		DWORDVec RewardVec;
		RewardVec.push_back( iEtcRewradA );
		RewardVec.push_back( iEtcRewradB );
		RewardVec.push_back( iEtcRewradC );
		RewardVec.push_back( iEtcRewradD );

		TournamentAlarmCustomRewardWnd* pWnd = dynamic_cast<TournamentAlarmCustomRewardWnd*>( g_GUIMgr.FindWnd( CUSTOM_TOURNAMENT_REWARD_ALARM_WND ) );
		if( pWnd )	
			pWnd->AddCustomTournamentRewardData( RewardVec, szTitle, MaxRound, iTourPos );	
	}

	void OnTournamentCheerDecision( SP2Packet &rkPacket )
	{
		int iResult;
		rkPacket >> iResult;	

		TCPNetwork::MouseBusy( false );

		switch( iResult )
		{
		case TOURNAMENT_CHEER_DECISION_OK:
			{
				int iCheerCnt;
				DWORD dwTourIndex;
				DWORD dwTeamIndex;

				rkPacket >> iCheerCnt >> dwTourIndex >> dwTeamIndex;
				ioUserTournament *pUserTournament = g_MyInfo.GetTournament();

				if( pUserTournament )
					pUserTournament->AddCheerTeamList( dwTourIndex, dwTeamIndex );

				g_TournamentTeamMgr.UpdateCheerPoint( dwTeamIndex, iCheerCnt );

				TournamentNode* pTournament = g_TournamentMgr.GetTournament( dwTourIndex );
				if( pTournament->GetType() == TournamentNode::TYPE_REGULAR )
				{
					TournamentDetailMainWnd* pMainWnd = dynamic_cast<TournamentDetailMainWnd*>( g_GUIMgr.FindWnd( TOURNAMENT_INFO ) );
					if( pMainWnd && pMainWnd->IsShow() )
						pMainWnd->ApplyDecision();
				}
			}
			break;
		case TOURNAMENT_CHEER_DECISION_ALREADY:
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, "�̹� �����ϱ⿡ ���� �ϼ̽��ϴ�" );
			}
			break;
		case TOURNAMENT_CHEER_DECISION_NONE_TEAM:
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, "�������� �ʴ� ���Դϴ�." );
			}
			break;
		case TOURNAMENT_CHEER_DECISION_CLOSE:
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, "�̹� ����� ��ȸ�Դϴ�." );
			}
			break;
		case TOURNAMENT_CHEER_DECISION_NOT_REGULAR:
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, "���Դ�ȸ�� �ƴմϴ�." );
			}
			break;
		case TOURNAMENT_CHEER_DECISION_NOT_TEAM_DELAY:
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, "�����Ⱓ���� ���� �����մϴ�." );
			}
			break;
		case TOURNAMENT_CHEER_DECISION_FAIL:
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, "�����ϱ⿡ ���� �Ͽ����ϴ�." );
			}
			break;
		}
	}

	void OnTournamentCheerList( SP2Packet &rkPacket )
	{
		TCPNetwork::MouseBusy( false );

		ioUserTournament *pTournament = g_MyInfo.GetTournament();
		if( !pTournament )
		{
			LOG.PrintTimeAndLog(0 ,"%s pTournament == NULL ", __FUNCTION__ );
			return;
		}	

		int iCount;
		rkPacket >> iCount;

		for( int i = 0; i < iCount; ++i )
		{
			DWORD dwTourIdx, dwTeamIdx;
			rkPacket >> dwTourIdx >> dwTeamIdx;
			pTournament->AddCheerTeamList( dwTourIdx, dwTeamIdx );

			if( g_TournamentMgr.GetRegularIndex() == dwTourIdx && dwTourIdx != 0 )
			{
				SP2Packet kPacket( CTPK_TOURNAMENT_TEAM_INFO );
				kPacket << dwTourIdx << dwTeamIdx;
				TCPNetwork::SendToServer( kPacket );
			}
		}
	} 
}