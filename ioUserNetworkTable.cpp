

#include "stdafx.h"

#include "ioSP2TCPSocket.h"

#include "ioUserNetworkTable.h"

#include "ioPlayStage.h"

ioUserNetworkTable::ioUserNetworkTable()
{
	m_dwProcessTime = 0;
}

ioUserNetworkTable::~ioUserNetworkTable()
{
	RemoveAllUser();
}

UserNetworkInfo* ioUserNetworkTable::AddNewUser( const DWORD dwUserIndex,
												 const ioHashString &rkName,
												 const char *szPublicIP,
												 const char *szPrivateIP,
												 int iPort,
												 const char *szTransferIP,
												 int  iTransferPort,
#ifndef ANTIHACK
												 DWORD dwStyle,
												 int nDebugValue )
#else
												 DWORD dwStyle, DWORD dwUserSeed, DWORD dwNPCSeed )
#endif
												 
{
	/*if( dwStyle == P2P_STYLE_VOICE )
	return NULL;*/
	/*
	if( dwUserIndex == 0 )
	{
		LOG.PrintTimeAndLog( 0, "ioUserNetworkTable::AddNewUser UserIndex Zero!" );
	}
	*/
	//LOG.PrintTimeAndLog( 0, "ioUserNetworkTable::AddNewUser(%s) : %d - %d - %d - %d - %d", rkName.c_str(), iPort, (int)dwStyle, (int)dwUserIndex, (int)FRAMEGETTIME(), nDebugValue );
	NetworkTable::iterator iter = m_NetworkTable.find( rkName );
	if( iter != m_NetworkTable.end() )
	{
		/*UserNetworkInfo *pInfo = iter->second;
		if( pInfo )
		{
			if( dwStyle == P2P_STYLE_VOICE ) 
			{
				pInfo->m_dwStyle |= dwStyle;
				return pInfo;
			}
			else
				RemoveUser( rkName, dwStyle );
		}
		else
			RemoveUser( rkName, dwStyle );*/

		


		// �̹� �ִ� ����
		/*UserNetworkInfo *pInfo = iter->second;
		if( pInfo )
		{
			pInfo->m_dwStyle |= dwStyle;
			pInfo->m_dwOnlyP2PRecvTime = REALGETTIME();
			if( pInfo->IsServerRelay() )
			{
				LOG.PrintTimeAndLog( 0, "ioUserNetworkTable::AddNewUser Already User IsServerRelay(%s) : %d - %d - %d", pInfo->m_Name.c_str(), pInfo->m_addrPublic.m_Port, iPort, iTransferPort );
				strncpy( pInfo->m_addrSend.m_IP, szTransferIP, LENGTH_IP ); 	
				pInfo->m_addrSend.m_Port  = iTransferPort;			
				pInfo->m_addrPublic.m_Port= iPort;
			}
			else
			{					
				LOG.PrintTimeAndLog( 0, "ioUserNetworkTable::AddNewUser  Already User P2P(%s) : %d - %d", pInfo->m_Name.c_str(), pInfo->m_addrPublic.m_Port, iPort );
				if( pInfo->m_addrPublic.m_Port != iPort )
					ServerToClientPortChange( rkName, szPublicIP, szPrivateIP, szTransferIP, iPort, iTransferPort );
			}
			strncpy( pInfo->m_addrTransfer.m_IP, szTransferIP, LENGTH_IP ); 			
			pInfo->m_addrTransfer.m_Port = iTransferPort;			
		}
		return pInfo;*/
		
		
		//RemoveUser( rkName, dwStyle );

		UserNetworkInfo *pInfo = iter->second;
		if( pInfo )
		{
			pInfo->m_dwUserIndex	= dwUserIndex;
			pInfo->m_dwStyle		|= dwStyle;
			// ������ ���ÿ� �����ϴ� �ּ�
			strncpy( pInfo->m_addrPublic.m_IP, szPublicIP, LENGTH_IP );
			pInfo->m_addrPublic.m_Port  = iPort;
			strncpy( pInfo->m_addrTransfer.m_IP, szTransferIP, LENGTH_IP );
			pInfo->m_addrTransfer.m_Port= iTransferPort;

			// ������ ���ÿ� �����ϴ� �ּ�
			strncpy( pInfo->m_addrPublic.m_IP, szPublicIP, LENGTH_IP );
			pInfo->m_addrPublic.m_Port  = iPort;
			strncpy( pInfo->m_addrTransfer.m_IP, szTransferIP, LENGTH_IP );
			pInfo->m_addrTransfer.m_Port= iTransferPort;

			// ������ ���� Ȯ��
			if( strcmp( szPublicIP, szPrivateIP ) != 0 )
				pInfo->m_bNatUser = true;
			else
				pInfo->m_bNatUser = false;

			// P2P Hole & Send Address Init
			if( pInfo->IsNatUser() && g_MyInfo.IsSameMyIP( pInfo->m_addrPublic.m_IP ) )       // ���� �����⳻ ����
			{		
				strncpy( pInfo->m_addrP2pHole.m_IP, szPrivateIP, LENGTH_IP );
				pInfo->m_addrP2pHole.m_Port = g_App.CreateP2PPort( pInfo->m_addrP2pHole.m_IP );

				strncpy( pInfo->m_addrSend.m_IP, pInfo->m_addrP2pHole.m_IP, LENGTH_IP );
				pInfo->m_addrSend.m_Port = pInfo->m_addrP2pHole.m_Port;
			}
			else
			{
				strncpy( pInfo->m_addrP2pHole.m_IP, pInfo->m_addrPublic.m_IP, LENGTH_IP );
				pInfo->m_addrP2pHole.m_Port = pInfo->m_addrPublic.m_Port;

				strncpy( pInfo->m_addrSend.m_IP, pInfo->m_addrPublic.m_IP, LENGTH_IP );
				pInfo->m_addrSend.m_Port = pInfo->m_addrPublic.m_Port;
			}	

			pInfo->m_bHoleSendComplete = true;
			pInfo->m_bHoleRecvComplete = true;
			pInfo->m_iHoleRecvCompleteCallCount = 0;

			pInfo->m_dwHoleSendTime       = REALGETTIME();
			pInfo->m_dwLastRecvTime       = REALGETTIME();
			pInfo->m_dwOnlyP2PRecvTime    = REALGETTIME();
			pInfo->m_dwRecvDelayLimitTime = RECV_DELAY_CHECK_TIME;



		//	pInfo->m_dwOnlyP2PRecvTime = REALGETTIME();
			if( pInfo->IsServerRelay() )
			{
				//LOG.PrintTimeAndLog( 0, "ioUserNetworkTable::AddNewUser Already User IsServerRelay(%s) : %d - %d - %d", pInfo->m_Name.c_str(), pInfo->m_addrPublic.m_Port, iPort, iTransferPort );
				strncpy( pInfo->m_addrSend.m_IP, szTransferIP, LENGTH_IP ); 	
				pInfo->m_addrSend.m_Port  = iTransferPort;			
				pInfo->m_addrPublic.m_Port= iPort;
			}
			else
			{					
				//LOG.PrintTimeAndLog( 0, "ioUserNetworkTable::AddNewUser  Already User P2P(%s) : %d - %d", pInfo->m_Name.c_str(), pInfo->m_addrPublic.m_Port, iPort );
				if( pInfo->m_addrPublic.m_Port != iPort )
					ServerToClientPortChange( rkName, szPublicIP, szPrivateIP, szTransferIP, iPort, iTransferPort );
			}
			strncpy( pInfo->m_addrTransfer.m_IP, szTransferIP, LENGTH_IP ); 			
			pInfo->m_addrTransfer.m_Port = iTransferPort;

#ifdef ANTIHACK
			pInfo->m_dwUserSeed = dwUserSeed;
			pInfo->m_dwNPCSeed = dwNPCSeed;
			pInfo->m_dwLastHostTime = 0;
#endif	
		}
		else 
		{
			LOG.PrintTimeAndLog( 0, "ioUserNetworkTable::AddNewUser -- Error" );
		}

		return pInfo;
	}

	// ù ����
	UserNetworkInfo *pInfo = new UserNetworkInfo;
	pInfo->m_dwUserIndex   = dwUserIndex;
	pInfo->m_Name		   = rkName;
	pInfo->m_dwStyle	   = dwStyle;
    
	// ������ ���ÿ� �����ϴ� �ּ�
	strncpy( pInfo->m_addrPublic.m_IP, szPublicIP, LENGTH_IP );
	pInfo->m_addrPublic.m_Port  = iPort;
	strncpy( pInfo->m_addrTransfer.m_IP, szTransferIP, LENGTH_IP );
	pInfo->m_addrTransfer.m_Port= iTransferPort;

	// ������ ���� Ȯ��
	if( strcmp( szPublicIP, szPrivateIP ) != 0 )
		pInfo->m_bNatUser = true;
	else
		pInfo->m_bNatUser = false;
	
	// P2P Hole & Send Address Init
	if( pInfo->IsNatUser() && g_MyInfo.IsSameMyIP( pInfo->m_addrPublic.m_IP ) )       // ���� �����⳻ ����
	{		
		strncpy( pInfo->m_addrP2pHole.m_IP, szPrivateIP, LENGTH_IP );
		pInfo->m_addrP2pHole.m_Port = g_App.CreateP2PPort( pInfo->m_addrP2pHole.m_IP );

		strncpy( pInfo->m_addrSend.m_IP, pInfo->m_addrP2pHole.m_IP, LENGTH_IP );
		pInfo->m_addrSend.m_Port = pInfo->m_addrP2pHole.m_Port;
	}
	else
	{
		strncpy( pInfo->m_addrP2pHole.m_IP, pInfo->m_addrPublic.m_IP, LENGTH_IP );
		pInfo->m_addrP2pHole.m_Port = pInfo->m_addrPublic.m_Port;

		strncpy( pInfo->m_addrSend.m_IP, pInfo->m_addrPublic.m_IP, LENGTH_IP );
		pInfo->m_addrSend.m_Port = pInfo->m_addrPublic.m_Port;
	}	

	pInfo->m_bHoleSendComplete = true;
	pInfo->m_bHoleRecvComplete = true;
	pInfo->m_iHoleRecvCompleteCallCount = 0;
	
	pInfo->m_dwHoleSendTime       = REALGETTIME();
	pInfo->m_dwLastRecvTime       = REALGETTIME();
	pInfo->m_dwOnlyP2PRecvTime    = REALGETTIME();
	pInfo->m_dwRecvDelayLimitTime = RECV_DELAY_CHECK_TIME;
	pInfo->m_iPingTime = 0;

	pInfo->m_iLastControlIndex = 0;
	pInfo->m_RecvIndexQueue.clear();
#ifdef ANTIHACK
	pInfo->m_dwUserSeed = dwUserSeed;
	pInfo->m_dwNPCSeed = dwNPCSeed;
	pInfo->m_dwLastHostTime = 0;
#endif

	m_NetworkTable.insert( NetworkTable::value_type( rkName, pInfo ) );
	
	if( Help::IsOnlyServerRelay() )
		SetUDPRecvTimeOut( rkName );

	/*if( dwUserIndex == 0 )
	{
		LOG.PrintTimeAndLog( 0, "ioUserNetworkTable::AddNewUser UserIndex Zero!" );
	}*/

	return pInfo;
}

// Server < - > Client Port Change
void ioUserNetworkTable::ServerToClientPortChange( const ioHashString &rkName,
												   const ioHashString &szPublicIP,
												   const ioHashString &szPrivateIP,
												   const ioHashString &szTransferIP,			
												   int iPort,
												   int iTransferPort )
{
	UserNetworkInfo *pInfo = GetNetworkInfo( rkName );
	if( pInfo )
	{
		char szLog[MAX_PATH*2] = "";
		sprintf( szLog, "ioUserNetworkTable::ServerToClientPortChange - %s (%d(%d)->%d)", rkName.c_str(), pInfo->m_addrPublic.m_Port, pInfo->m_addrP2pHole.m_Port, iPort );
		LOG.PrintTimeAndLog( 0, "%s", szLog );
		if( g_App.IsMeDeveloper() )
			g_ChatMgr.SetSystemMsg( szLog );

		strncpy( pInfo->m_addrPublic.m_IP, szPublicIP.c_str(), LENGTH_IP );
		pInfo->m_addrPublic.m_Port  = iPort;
		if( pInfo->IsServerRelay() )
		{
			strncpy( pInfo->m_addrSend.m_IP, szTransferIP.c_str(), LENGTH_IP );
			pInfo->m_addrSend.m_Port= iTransferPort;
		}
		strncpy( pInfo->m_addrTransfer.m_IP, szTransferIP.c_str(), LENGTH_IP );
		pInfo->m_addrTransfer.m_Port= iTransferPort;

		// Public - P2pHole ���ٸ� P2pHole��Ʈ�� ����
		if( strcmp( pInfo->m_addrPublic.m_IP, pInfo->m_addrP2pHole.m_IP ) == 0 )
		{
			pInfo->m_addrP2pHole.m_Port = pInfo->m_addrPublic.m_Port;
		}

		// Public - Send ���ٸ� Send��Ʈ�� ����
		if( strcmp( pInfo->m_addrPublic.m_IP, pInfo->m_addrSend.m_IP ) == 0 )
		{
			pInfo->m_addrSend.m_Port = pInfo->m_addrPublic.m_Port;
		}
	}
}

// Client < - > Client Port Change
void ioUserNetworkTable::ClientToClientPortChange( const ioHashString &rkName, const char *szRcvIP, int iRcvPort )
{
	UserNetworkInfo *pInfo = GetNetworkInfo( rkName );
	if( pInfo )
	{
		// LogDB�� ���
		if( g_App.IsP2PDataDBLog() )
		{
			char szLOG[2048] = "";
			sprintf( szLOG, "Peer To Peer Port Change : %s ( %s:%d ) - ( %s:%d )", rkName.c_str(), pInfo->m_addrPublic.m_IP, pInfo->m_addrPublic.m_Port,
																				   szRcvIP, iRcvPort );
			SP2Packet kLogPacket( LUPK_LOG );
			kLogPacket << "CheckError";  // �α� ���� Ÿ��
			kLogPacket << szLOG;
			kLogPacket << 321;  // ������ȣ:321
			kLogPacket << true; // write db
			P2PNetwork::SendLog( kLogPacket );
		}

		char szLog[MAX_PATH*2] = "";
		sprintf( szLog, "ioUserNetworkTable::ClientToClientPortChange - %s (%d->%d)", rkName.c_str(), pInfo->m_addrP2pHole.m_Port, iRcvPort );
		LOG.PrintTimeAndLog( 0, "%s", szLog );
		if( g_App.IsMeDeveloper() )
			g_ChatMgr.SetSystemMsg( szLog );

		if( pInfo->m_bHoleSendComplete )
		{
			strncpy( pInfo->m_addrP2pHole.m_IP, szRcvIP, LENGTH_IP );
			pInfo->m_addrP2pHole.m_Port= iRcvPort;
			strncpy( pInfo->m_addrSend.m_IP, pInfo->m_addrP2pHole.m_IP, LENGTH_IP );
			pInfo->m_addrSend.m_Port = pInfo->m_addrP2pHole.m_Port;
		}
		else
		{
			strncpy( pInfo->m_addrP2pHole.m_IP, szRcvIP, LENGTH_IP );
			pInfo->m_addrP2pHole.m_Port = iRcvPort;
		}		
	}
}

void ioUserNetworkTable::RemoveUser( const ioHashString &rkName , DWORD dwStyle )
{
	/*if( Help::CheckEnableTimeGapUser2( 1 ) )
		LOG.PrintTimeAndLog( 0, "ioUserNetworkTable::RemoveUser(%s) : %d", rkName.c_str(), dwStyle );*/

	NetworkTable::iterator iter = m_NetworkTable.find( rkName );
	if( iter != m_NetworkTable.end() )
	{
		UserNetworkInfo *pInfo = iter->second;
		if( pInfo ) 
		{
			if( pInfo->HasStyle( dwStyle ) )
				pInfo->m_dwStyle ^= dwStyle;
			if( pInfo->m_dwStyle == 0 )
			{
				/*if( Help::CheckEnableTimeGapUser2( 1 ) )
					LOG.PrintTimeAndLog( 0, "ioUserNetworkTable::RemoveUser(%s) : %d - %d", pInfo->m_Name.c_str(), pInfo->m_addrPublic.m_Port, pInfo->m_dwStyle );*/

				delete iter->second;
				m_NetworkTable.erase( iter );
			}
		}		
	}
}

void ioUserNetworkTable::RemoveAllUser() 
{
	/*if( Help::CheckEnableTimeGapUser2( 1 ) )
		LOG.PrintTimeAndLog( 0, "ioUserNetworkTable::RemoveAllUser - In" );*/

	NetworkTable::iterator iter;
	for( iter=m_NetworkTable.begin() ; iter!=m_NetworkTable.end() ; ++iter )
	{
		UserNetworkInfo *pInfo = iter->second;
		if( pInfo )
		{
			if( Help::CheckEnableTimeGapUser2( 1 ) )
				LOG.PrintTimeAndLog( 0, "ioUserNetworkTable::RemoveAllUser(%s) : %d - %d", pInfo->m_Name.c_str(), pInfo->m_addrPublic.m_Port, pInfo->m_dwStyle );
		
		}
		
		delete iter->second;
	}

	m_NetworkTable.clear();
}

void ioUserNetworkTable::RemoveAllUser( DWORD dwStyle )
{
	NetworkTable::iterator iter = m_NetworkTable.begin();
	while( iter!=m_NetworkTable.end() )
	{
		UserNetworkInfo *pInfo = iter->second;
		if( !pInfo ) 
		{
			if( Help::CheckEnableTimeGapUser2( 1 ) )
				LOG.PrintTimeAndLog( 0, "ioUserNetworkTable::RemoveAllUser - Style - Error1" );
			continue;
		}

		if( pInfo->HasStyle( dwStyle ) )
			pInfo->m_dwStyle ^= dwStyle;
		if( pInfo->m_dwStyle == 0 )
		{
			if( Help::CheckEnableTimeGapUser2( 1 ) )
				LOG.PrintTimeAndLog( 0, "ioUserNetworkTable::RemoveAllUser - Style - (%s) : %d - %d", pInfo->m_Name.c_str(), pInfo->m_addrPublic.m_Port, pInfo->m_dwStyle );
			delete iter->second;
			iter = m_NetworkTable.erase( iter );
		}
		else
		{
			++iter;
		}
	}
}

void ioUserNetworkTable::SetLastControlIndex( const ioHashString &rkName, int iIndex )
{
	UserNetworkInfo *pInfo = GetNetworkInfo( rkName );
	if( pInfo )
	{
		pInfo->m_iLastControlIndex = iIndex;
		pInfo->m_dwLastRecvTime = REALGETTIME();   //���������� UDP ��ȣ ���� �ð�.
	}
}

int ioUserNetworkTable::GetLastControlIndex( const ioHashString &rkName )
{
	UserNetworkInfo *pInfo = GetNetworkInfo( rkName );
	if( pInfo )
		return pInfo->m_iLastControlIndex;

	return -1;
}

const char *ioUserNetworkTable::GetUserGlobalIP( const ioHashString &rkName )
{
	UserNetworkInfo *pInfo = GetNetworkInfo( rkName );
	if( pInfo )
		return pInfo->m_addrPublic.m_IP;
	return "";
}
/*
void  ioUserNetworkTable::SetLastRecvTime( const ioHashString &rkName, DWORD dwHostTime, DWORD dwGlobalTime )
{
	//1�ʿ� �ѹ��� �޴´�.
	UserNetworkInfo *pInfo = GetNetworkInfo( rkName );
	if( pInfo )
	{
		pInfo->m_iPingTime  = (int)dwGlobalTime - (int)dwHostTime ;	
	}
}
*/
void  ioUserNetworkTable::SetLastRecvTime( const ioHashString &rkName, DWORD dwPingTime )
{
	//1�ʿ� �ѹ��� �޴´�.
	UserNetworkInfo *pInfo = GetNetworkInfo( rkName );
	if( pInfo )
	{
		pInfo->m_iPingTime  = dwPingTime;
	}
}

int ioUserNetworkTable::GetP2pPingTime( const ioHashString &rkName )
{
	UserNetworkInfo *pInfo = GetNetworkInfo( rkName );
	if( pInfo )
		return pInfo->m_iPingTime;

	return 0;
}

bool  ioUserNetworkTable::IsServerRelay( const ioHashString &rkName )
{
	UserNetworkInfo *pInfo = GetNetworkInfo( rkName );
	if( pInfo )
		return pInfo->IsServerRelay();
	return false;	
}

#define MAX_RECV_INDEX 100

bool ioUserNetworkTable::CheckDuplicateIndex( UserNetworkInfo *pInfo, int iCurIndex )
{
	while( pInfo->m_RecvIndexQueue.size() >= MAX_RECV_INDEX )
	{
		pInfo->m_RecvIndexQueue.pop_front();
	}

	UserNetworkInfo::RecvIndexQueue::iterator iter;
	iter = std::find( pInfo->m_RecvIndexQueue.begin(), pInfo->m_RecvIndexQueue.end(), iCurIndex );
	if( iter != pInfo->m_RecvIndexQueue.end() )
		return true;

	pInfo->m_RecvIndexQueue.push_back( iCurIndex );

	return false;
}

bool ioUserNetworkTable::IsNetworkTimeOut( const ioHashString &rkName )
{
	UserNetworkInfo *pInfo = GetNetworkInfo( rkName );
	if( pInfo )
	{
		if( REALGETTIME() - pInfo->m_dwLastRecvTime > RECV_DELAY_CHECK_TIME )   //10�ʰ� �ѹ��� UDP ��ȣ�� ���� ���ߴ�.
			return true;
		else
			return false;
	}
	return true;
}

bool ioUserNetworkTable::IsAllNetworkTimeOut()
{
	if( m_NetworkTable.empty() ) return false;

	NetworkTable::iterator iter;
	for( iter=m_NetworkTable.begin() ; iter!=m_NetworkTable.end() ; ++iter )
	{
		UserNetworkInfo *pInfo = iter->second;
		if( !pInfo ) continue;

		if( REALGETTIME() - pInfo->m_dwLastRecvTime <= RECV_DELAY_CHECK_TIME )   //10�ʾȿ� UDP ��ȣ�� �޾Ҵ�.
			return false;
	}
	return true;
}

ioUserNetworkTable::NetworkInfoIter ioUserNetworkTable::GetNetworkInfoIter()
{
	return NetworkInfoIter( m_NetworkTable.begin(), m_NetworkTable.end() );
}

UserNetworkInfo* ioUserNetworkTable::GetNetworkInfo( const ioHashString &rkName )
{
	NetworkTable::iterator iter = m_NetworkTable.find( rkName );
	if( iter!=m_NetworkTable.end() )
		return iter->second;

	return NULL;
}

UserNetworkInfo* ioUserNetworkTable::GetNetworkInfo( DWORD dwUserIndex )
{
	NetworkTable::iterator iter;
	for( iter=m_NetworkTable.begin() ; iter!=m_NetworkTable.end() ; ++iter )
	{
		if( iter->second->m_dwUserIndex == dwUserIndex )
			return iter->second;
	}

	return NULL;
}

UserNetworkInfo* ioUserNetworkTable::GetNetworkInfo( const char *szIP, const int iPort )
{
	NetworkTable::iterator iter;
	for( iter=m_NetworkTable.begin() ; iter!=m_NetworkTable.end() ; ++iter )
	{
		if( strcmp( iter->second->m_addrPublic.m_IP, szIP ) == 0 && 
			iter->second->m_addrPublic.m_Port == iPort )
			return iter->second;
	}

	return NULL;
}

void ioUserNetworkTable::HoleSendProcess()
{
	if( Help::IsOnlyServerRelay() ) return;     //������ ���� �߰�

	NetworkTable::iterator iter;
	for( iter=m_NetworkTable.begin() ; iter!=m_NetworkTable.end() ; ++iter )
	{
		UserNetworkInfo *pInfo = iter->second;
		if( !pInfo ) continue;
		if( pInfo->m_bHoleSendComplete ) continue;
		if( REALGETTIME() - pInfo->m_dwHoleSendTime <= HOLE_PACKET_SEND_TIME ) continue;

		pInfo->m_dwHoleSendTime = REALGETTIME();

		// ������ ������ ���� ������ ������ �ƴ϶�� Public�� P2pHole�� �ּҰ� �����ϴ�.
		if( strcmp( pInfo->m_addrPublic.m_IP, pInfo->m_addrP2pHole.m_IP ) == 0 )
		{
			// P2pHole���� ����
			SP2Packet kPacket( CUPK_HOLE_CHECK );
			kPacket << g_MyInfo.GetPublicID() << pInfo->m_addrP2pHole.m_IP << pInfo->m_addrP2pHole.m_Port;
			g_App.SendP2PDirectPacket( pInfo->m_addrP2pHole.m_IP, pInfo->m_addrP2pHole.m_Port, kPacket );
			
			if( g_App.IsMeDeveloper() )
			{
				char szLog[MAX_PATH*2] = "";
				sprintf( szLog, "HoleSendProcess : %s - %s:%d", pInfo->m_Name.c_str(), pInfo->m_addrP2pHole.m_IP, pInfo->m_addrP2pHole.m_Port );
				LOG.PrintTimeAndLog( 0, "%s", szLog );
				//g_ChatMgr.SetSystemMsg( szLog );
			}
		}
		else
		{
			// P2pHole�� ������
			{
				SP2Packet kPacket( CUPK_HOLE_CHECK );
				kPacket << g_MyInfo.GetPublicID() << pInfo->m_addrP2pHole.m_IP << pInfo->m_addrP2pHole.m_Port;
				g_App.SendP2PDirectPacket( pInfo->m_addrP2pHole.m_IP, pInfo->m_addrP2pHole.m_Port, kPacket );
			}
			// Public���� ����
			{
				SP2Packet kPacket( CUPK_HOLE_CHECK );
				kPacket << g_MyInfo.GetPublicID() << pInfo->m_addrPublic.m_IP << pInfo->m_addrPublic.m_Port;
				g_App.SendP2PDirectPacket( pInfo->m_addrPublic.m_IP, pInfo->m_addrPublic.m_Port, kPacket );
			}

			if( g_App.IsMeDeveloper() )
			{
				char szLog[MAX_PATH*2] = "";
				sprintf( szLog, "HoleSendProcess NAT: %s - %s:%d - %s:%d", pInfo->m_Name.c_str(), 
							    pInfo->m_addrP2pHole.m_IP, pInfo->m_addrP2pHole.m_Port, 
								pInfo->m_addrPublic.m_IP, pInfo->m_addrPublic.m_Port );
				LOG.PrintTimeAndLog( 0, "%s", szLog );
				//g_ChatMgr.SetSystemMsg( szLog );
			}
		}
	}
}

void ioUserNetworkTable::HoleRecvProcess()
{
	if( Help::IsOnlyServerRelay() ) return;     //������ ���� �߰�

	NetworkTable::iterator iter;
	for( iter=m_NetworkTable.begin() ; iter!=m_NetworkTable.end() ; ++iter )
	{
		UserNetworkInfo *pInfo = iter->second;
		if( !pInfo ) continue;
		if( !pInfo->m_bHoleRecvComplete ) continue;
		if( REALGETTIME() - pInfo->m_dwOnlyP2PRecvTime <= pInfo->m_dwRecvDelayLimitTime ) continue;

		pInfo->m_dwOnlyP2PRecvTime = REALGETTIME();
		pInfo->m_bHoleRecvComplete = false;
		pInfo->m_iHoleRecvCompleteCallCount = 0;

		// ������ ��ȣ �ȿ´ٰ� �˸�
		SP2Packet kPacket( CTPK_UDP_RECV_TIMEOUT );
		kPacket << pInfo->m_Name;
		TCPNetwork::SendToServer( kPacket );
		if( g_App.IsMeDeveloper() )
		{
			char szLog[MAX_PATH*2] = "";
			sprintf( szLog,  "HoleRecvProcess %dSec Timeout : %s - %d - %s:%d", pInfo->m_dwRecvDelayLimitTime/1000, pInfo->m_Name.c_str(), 
							 (int)pInfo->m_bHoleSendComplete, pInfo->m_addrSend.m_IP, pInfo->m_addrSend.m_Port );
			LOG.PrintTimeAndLog( 0, "%s", szLog );
			g_ChatMgr.SetSystemMsg( szLog );
		}

		// LogDB�� ���
		if( g_App.IsP2PDataDBLog() )
		{
			char szLOG[2048] = "";
			sprintf( szLOG, "%d�ʰ� UDP ������ ���� ���� �߰� ��û ����: %s(%s) - > %s(%s:%d)",  //Except Extracting Hangeul
				            pInfo->m_dwRecvDelayLimitTime/1000, g_MyInfo.GetPublicID().c_str(), g_MyInfo.GetMyIP().c_str(),
				            pInfo->m_Name.c_str(), pInfo->m_addrPublic.m_IP, pInfo->m_addrPublic.m_Port );
			SP2Packet kLogPacket( LUPK_LOG );
			kLogPacket << "CheckError";  // �α� ���� Ÿ��
			kLogPacket << szLOG;
			kLogPacket << 3184;  // ������ȣ:3184
			kLogPacket << true; // write db
			P2PNetwork::SendLog( kLogPacket );
		}
	}
/*
#ifndef SHIPPING
	if( m_NetworkTable.empty() ) return;
	if( !g_App.IsMeDeveloper() ) return;
	if( strcmp( "������L", g_MyInfo.GetPublicID().c_str() ) == 0 ) return; //Except Extracting Hangeul
	
	// �׽�Ʈ�� �ڵ�
	if( m_dwProcessTime == 0 )
		m_dwProcessTime = REALGETTIME() + (DWORD)(rand()%RECV_DELAY_CHECK_TIME + RECV_DELAY_CHECK_TIME);
	if( REALGETTIME() > m_dwProcessTime )
	{
		m_dwProcessTime = REALGETTIME() + (DWORD)(rand()%RECV_DELAY_CHECK_TIME + RECV_DELAY_CHECK_TIME);
		g_App.ResetInitUDPSocket();
	}
#endif
*/
}

void ioUserNetworkTable::HoleSendComplete( const ioHashString &rkName, const ioHashString &szIP, int iPort )
{
	UserNetworkInfo *pInfo = GetNetworkInfo( rkName );
	if( pInfo )
	{
		pInfo->m_bHoleSendComplete = true;
		strncpy( pInfo->m_addrSend.m_IP, szIP.c_str(), LENGTH_IP );
		pInfo->m_addrSend.m_Port = iPort;  
		if( g_App.IsMeDeveloper() )
		{
			char szLog[MAX_PATH*2] = "";
			sprintf( szLog,  "HoleSendComplete : %s - %s:%d", rkName.c_str(), szIP.c_str(), iPort );
			LOG.PrintTimeAndLog( 0, "%s", szLog );
			g_ChatMgr.SetSystemMsg( szLog );
		}

		// LogDB�� ���
		if( g_App.IsP2PDataDBLog() )
		{
			char szLOG[2048] = "";
			sprintf( szLOG, "%s(%s:%d) �����κ��� Hole ���� ��Ŷ ����(%s:%s)", //Except Extracting Hangeul
				            rkName.c_str(), szIP.c_str(), iPort, g_MyInfo.GetPublicID().c_str(), g_MyInfo.GetMyIP().c_str() );
			SP2Packet kLogPacket( LUPK_LOG );
			kLogPacket << "CheckError";  // �α� ���� Ÿ��
			kLogPacket << szLOG;
			kLogPacket << 3185;  // ������ȣ:3185
			kLogPacket << true; // write db
			P2PNetwork::SendLog( kLogPacket );
		}
	}
	else
	{
		LOG.PrintTimeAndLog( 0, "HoleSendComplete : Error User : %s", rkName.c_str() );
	}
}

void ioUserNetworkTable::SetUDPRecvTimeOut( const ioHashString &rkName )
{
	UserNetworkInfo *pInfo = GetNetworkInfo( rkName );
	if( pInfo )
	{
		pInfo->m_bHoleSendComplete = false;
		strncpy( pInfo->m_addrSend.m_IP, pInfo->m_addrTransfer.m_IP, LENGTH_IP );
		pInfo->m_addrSend.m_Port = pInfo->m_addrTransfer.m_Port;

		if( g_App.IsMeDeveloper() )
		{
			char szLog[MAX_PATH*2] = "";
			sprintf( szLog,  "SetUDPRecvTimeOut : %s - %s:%d", rkName.c_str(), pInfo->m_addrSend.m_IP, pInfo->m_addrSend.m_Port );
			LOG.PrintTimeAndLog( 0, "%s", szLog );
			g_ChatMgr.SetSystemMsg( szLog );
		}

		// LogDB�� ���
		if( g_App.IsP2PDataDBLog() )
		{
			char szLOG[2048] = "";
			sprintf( szLOG, "%d�ʰ� UDP ������ ���� ���� �߰� ��û ����: %s(%s:%d) - > %s(%s) ", //Except Extracting Hangeul
				            pInfo->m_dwRecvDelayLimitTime/1000, pInfo->m_Name.c_str(), pInfo->m_addrPublic.m_IP, pInfo->m_addrPublic.m_Port,
							g_MyInfo.GetPublicID().c_str(), g_MyInfo.GetMyIP().c_str() );
			SP2Packet kLogPacket( LUPK_LOG );
			kLogPacket << "CheckError";  // �α� ���� Ÿ��
			kLogPacket << szLOG;
			kLogPacket << 3186;  // ������ȣ:3186
			kLogPacket << true; // write db
			P2PNetwork::SendLog( kLogPacket );
		}
	}
	else
	{
		LOG.PrintTimeAndLog( 0, "SetUDPRecvTimeOut : Error User : %s", rkName.c_str() );
	}
}

void ioUserNetworkTable::ReCheckUDPRecvTimeOut( const ioHashString &rkName )
{
	UserNetworkInfo *pInfo = GetNetworkInfo( rkName );
	if( pInfo )
	{
		pInfo->m_dwOnlyP2PRecvTime = REALGETTIME();
		pInfo->m_bHoleRecvComplete = true;
		pInfo->m_iHoleRecvCompleteCallCount = 0;

		if( g_App.IsMeDeveloper() )
		{
			char szLog[MAX_PATH*2] = "";
			sprintf( szLog,  "TimeOut Room Loading Or Server Move Recheck : %s - %s:%d", rkName.c_str(), pInfo->m_addrSend.m_IP, pInfo->m_addrSend.m_Port );
			LOG.PrintTimeAndLog( 0, "%s", szLog );
			g_ChatMgr.SetSystemMsg( szLog );
		}
	}
	else
	{
		LOG.PrintTimeAndLog( 0, "ReCheckUDPRecvTimeOut : Error User : %s", rkName.c_str() );
	}
}

void ioUserNetworkTable::AllNetworkTableServerRelay()
{
	NetworkTable::iterator iter;
	for( iter=m_NetworkTable.begin() ; iter!=m_NetworkTable.end() ; ++iter )
	{
		UserNetworkInfo *pInfo = iter->second;
		if( !pInfo ) continue;

		pInfo->m_bHoleSendComplete = false;
		strncpy( pInfo->m_addrSend.m_IP, pInfo->m_addrTransfer.m_IP, LENGTH_IP );
		pInfo->m_addrSend.m_Port = pInfo->m_addrTransfer.m_Port;

		if( g_App.IsMeDeveloper() )
		{
			char szLog[MAX_PATH*2] = "";
			sprintf( szLog,  "AllNetworkTableServerRelay : %s - %s:%d", pInfo->m_Name.c_str(), pInfo->m_addrSend.m_IP, pInfo->m_addrSend.m_Port );
			LOG.PrintTimeAndLog( 0, "%s", szLog );
			g_ChatMgr.SetSystemMsg( szLog );
		}
	}
}

void ioUserNetworkTable::CheckValidateNetTable( ioPlayStage *pStage )
{
	if( !pStage )
		return;

	ioHashString szName;
	NetworkTable::iterator iter = m_NetworkTable.begin();
	while( iter != m_NetworkTable.end() )
	{
		szName = iter->first;
		if( !pStage->CheckValidateMember(szName) )
		{
			/*
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1), iter->first.c_str() );

			delete iter->second;
			iter = m_NetworkTable.erase( iter );
			continue;
			*/

			/*
			ErrorReport::SetPosition( 9000, 11 );
			ErrorReport::SetFormat( "Invalidate : %s", szName.c_str() );
			// �α� ���� ����
			char szErrorPos[MAX_PATH] = "";
			SafeSprintf( szErrorPos, sizeof( szErrorPos ), STR(2), g_MyInfo.GetMyIP().c_str(),
																										g_MyInfo.GetPrivateID().c_str(),
																										g_MyInfo.GetPublicID().c_str(),
																										ErrorReport::GetMajorPosition(),
																										ErrorReport::GetMinorPosition(),
																										ErrorReport::GetEngineMajorPos(),
																										ErrorReport::GetEngineMinorPos(),
																										ErrorReport::GetString() );


			SP2Packet kPacket( LUPK_LOG );
			kPacket << "ClientError";  // �α� ���� Ÿ��
			kPacket << szErrorPos;
			kPacket << 317;  // ������ȣ:317
			kPacket << false; // write db
			P2PNetwork::SendLog(kPacket);

			ErrorReport::SetFormat( "" );
			

			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3), iter->first.c_str() );
			*/
			LOG.PrintTimeAndLog( 0, "Test - Check NetTable: %s", szName.c_str() );
			continue;
		}

		++iter;
	}
}

#ifdef ANTIHACK
DWORD ioUserNetworkTable::GetControlSeed( const ioHashString &rkName, bool bUser )
{
	UserNetworkInfo* pInfo = GetNetworkInfo( rkName );
	if( pInfo )
		pInfo->m_dwUserSeed;

	return -1;
}

bool ioUserNetworkTable::CheckControlSeed( const ioHashString &rkName, DWORD dwSeed, DWORD dwTime, bool bUser )
{
	UserNetworkInfo* pInfo = GetNetworkInfo( rkName );
	if( pInfo )
	{
		if( bUser )
		{
			if( pInfo->m_dwUserSeed+1 == dwSeed )
			{
				pInfo->m_dwUserSeed++;
				pInfo->m_dwLastHostTime = dwTime;
				return true;
			}
			return false;
		}
		else
		{
			if( pInfo->m_dwNPCSeed+1 == dwSeed )
			{
				pInfo->m_dwNPCSeed++;
				pInfo->m_dwLastHostTime = dwTime;
				return true;
			}
			return false;
		}
	}

	return false;
}
#endif


