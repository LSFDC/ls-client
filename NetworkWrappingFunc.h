#pragma once


#include "SP2Packet.h"
#include "P2PProtocol.h"
#include "TCPProtocol.h"

class ioBaseChar;

namespace P2PNetwork
{
	bool IsNetworkPlaying();
	bool IsHost();

	void SendPacket( const char *szIP, int iPort, SP2Packet &rkPacket );
	void SendToAllPlayingUser( SP2Packet &rkPacket, ioBaseChar* pBase = NULL );
	void SendToAllUser( SP2Packet &rkPacket, ioBaseChar* pBase = NULL );
	void SendToTeam( SP2Packet &rkPacket, TeamType eSendTeam);
	void SendToUser( const ioHashString &rkName, SP2Packet &rkPacket );
	void SendToUserByIndex( const DWORD dwUserIndex, SP2Packet &rkPacket );
	void SendToHost( SP2Packet &rkPacket );
	void SendToServer( SP2Packet &rkPacket );
	void SendLog( SP2Packet &rkPacket );
	void SendP2PUserInfoSync( const ioHashString &rkName, SP2Packet &rkPacket );
	void SendToRelayPacket( const ioHashString &rkName, SP2Packet &rkPacket );
	void SendToRelaySelf( SP2Packet &rkPacket );

	void RecvPacket( SP2Packet &rkPacket, char *RcvIP, int iRcvPort );

	int  GetNextSendIndex( bool bNpc );
	void SetLastControlIndex( const ioHashString &rkName, int iIndex );
	void SetLastControlIndex( const ioHashString &rkName, SP2Packet &rkPacket );
	int  GetLastControlIndex( const ioHashString &rkName );

	void SetLastRecvTime( const ioHashString &rkName, DWORD dwPingTime );
	int  GetP2pPingTime( const ioHashString &rkName );
	bool IsServerRelay( const ioHashString &rkName );
	bool IsNetworkTimeOut( const ioHashString &rkName );
	bool IsNetworkTableUser( const ioHashString &rkName );

	bool ControlSyncExpand();
	void SetControlSyncExpand( bool bControlSyncExpand );

	//rudp�� ( �򰥸��� ������ ������ ���� )
	//bool CheckControlSeed( const ioHashString &rkName, DWORD dwSeed, DWORD dwTime );

}

namespace TCPNetwork
{
	void MouseBusy( bool bBusy );
	void SendToServer( SP2Packet &rkPacket );
	void RecvPacket( SP2Packet &rkPacket );

	void SetNagleAlgorithm( bool bOn );
}

namespace TCPChatNetwork
{
	void SendToServer( SP2Packet &rkPacket );
	void RecvPacket( SP2Packet &rkPacket );
}

