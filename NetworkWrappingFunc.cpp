

#include "stdafx.h"

#include "ioClientUDPSocket.h"

//종료 될 경우 모든 패킷을 무시하게끔
extern bool g_bExit;

extern bool m_bNetworkPlay;

namespace P2PNetwork
{
	bool IsNetworkPlaying()
	{
		return m_bNetworkPlay;
	}

	bool IsHost()
	{
		if( !IsNetworkPlaying() || g_MyInfo.IsHost() )
			return true;

		return false;
	}

	void SendToAllPlayingUser( SP2Packet &rkPacket, ioBaseChar* pBase )
	{
		g_App.SendToAllPlayingUser( rkPacket, pBase );
	}

	void SendToAllUser( SP2Packet &rkPacket, ioBaseChar* pBase )
	{
		g_App.SendToAllUser( rkPacket, pBase );
	}

	void SendToTeam( SP2Packet &rkPacket, TeamType eSendTeam)
	{
		g_App.SendToTeam( rkPacket , eSendTeam);
	}

	void SendToUser( const ioHashString &rkName, SP2Packet &rkPacket )
	{
		g_App.SendToUser( rkName, rkPacket );
	}

	void SendToUserByIndex( const DWORD dwUserIndex, SP2Packet &rkPacket )
	{
		g_App.SendToUserByIndex( dwUserIndex, rkPacket );
	}

	void SendToHost( SP2Packet &rkPacket )
	{
		if( !IsHost() )
		{
			g_App.SendToHost( rkPacket );
		}
	}

	void SendToServer( SP2Packet &rkPacket )
	{
		g_App.SendUDPToServer( rkPacket );
	}
	
	void SendLog( SP2Packet &rkPacket )
	{
		g_App.SendToLogServer( rkPacket );
	}

	void SendP2PUserInfoSync( const ioHashString &rkName, SP2Packet &rkPacket )
	{
		g_App.SendP2PUserInfoSync( rkName, rkPacket );
	}

	void SendToRelayPacket( const ioHashString &rkName, SP2Packet &rkPacket )
	{
		g_App.SendP2PRelayPacket( rkName, rkPacket );
	}

	void SendToRelaySelf( SP2Packet &rkPacket )
	{
		g_App.SendToRelaySelf( rkPacket );
	}

	void RecvPacket( SP2Packet &rkPacket, char *RcvIP, int iRcvPort )
	{
		if( g_bExit )
			return;
		g_App.RecvUDPPacket( rkPacket, RcvIP, iRcvPort );
	}
	
	int GetNextSendIndex( bool bNpc )
	{
		if( bNpc )
			return -1;
		return g_App.GetNextSendIndex();
	}

	void SetLastControlIndex( const ioHashString &rkName, int iIndex )
	{
		g_App.SetLastControlIndex( rkName, iIndex );
	}

	void SetLastControlIndex( const ioHashString &rkName, SP2Packet &rkPacket )
	{
		int iSendIndex;
		rkPacket >> iSendIndex;
		if( ControlSyncExpand() )
		{
			int iPreIndex = GetLastControlIndex( rkName );
			if( iPreIndex >= 0 && iPreIndex <= iSendIndex )
			{
				SetLastControlIndex( rkName, iSendIndex );
			}
		}
	}

	int GetLastControlIndex( const ioHashString &rkName )
	{
		return g_App.GetLastControlIndex( rkName );
	}

	void  SetLastRecvTime( const ioHashString &rkName, DWORD dwPingTime )
	{
		g_App.SetLastRecvTime( rkName, dwPingTime );
	}

	int   GetP2pPingTime( const ioHashString &rkName )
	{
		return g_App.GetP2pPingTime( rkName );
	}

	bool  IsServerRelay( const ioHashString &rkName )
	{
		return g_App.IsServerRelay( rkName );
	}

	bool  IsNetworkTimeOut( const ioHashString &rkName )
	{
		return g_App.IsNetworkTimeOut( rkName );
	}

	bool IsNetworkTableUser( const ioHashString &rkName )
	{
		return g_App.IsNetworkTableUser( rkName );
	}

	bool ControlSyncExpand()
	{
		return g_App.IsControlSyncExpand();
	}

	void SetControlSyncExpand( bool bControlSyncExpand )
	{
		g_App.SetControlSyncExpand( bControlSyncExpand );
	}
}

namespace TCPNetwork
{
	void MouseBusy( bool bBusy )
	{
		g_App.SetMouseBusy( bBusy );
	}

	void SendToServer( SP2Packet &rkPacket )
	{
		g_App.SendTCPPacket( rkPacket );
	}

	void RecvPacket( SP2Packet &rkPacket )
	{
		if( g_bExit )
			return;
		g_App.RecvTCPPacket( rkPacket );
	}

	void SetNagleAlgorithm( bool bOn )
	{
		g_App.SetNagleAlgorithm( bOn );
	}
}