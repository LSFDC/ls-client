#pragma once

#include "../io3DEngine/ioMapIterator.h"

#define HOLE_PACKET_SEND_TIME            1000
#define RECV_DELAY_CHECK_TIME            10000

class ioPlayStage;

struct NetworkAddr
{
	char m_IP[LENGTH_IP];
	int  m_Port;
	NetworkAddr()
	{
		strncpy( m_IP, "", LENGTH_IP );
		m_Port = -1;
	}
};

struct UserNetworkInfo
{
	DWORD m_dwUserIndex; //���� �ε���.
	ioHashString m_Name; //���� �г���
	DWORD m_dwStyle;     //UDP ��Ÿ��(����ä��/�÷���)

	NetworkAddr m_addrPublic;   // �ܺ� �ּ�
	NetworkAddr m_addrTransfer; // ���� �߰� �ּ�
	NetworkAddr m_addrP2pHole;  // P2P Ȯ�� ���� �ּ�
	NetworkAddr m_addrSend;     // ��Ŷ ���� �ּ�.

	bool  m_bHoleSendComplete;   // �������� P2P�� ���� ��Ŷ�� �����ϸ� TCP ��Ŷ�� �ް� TRUE����
	DWORD m_dwHoleSendTime;      // Ȧ ��Ŷ ���� Ÿ�̸�
	bool  m_bHoleRecvComplete;   // �� ������ ������ P2P�� ���� ��Ŷ�� ������ TCP ��Ŷ�� ������ TRUE����
	int   m_iHoleRecvCompleteCallCount; // HoleRecvComplete == TRUE�� �� ī��Ʈ
	DWORD m_dwOnlyP2PRecvTime;   // P2P�θ� ���۵� ��Ŷ �ð�
	bool  m_bNatUser;	         // ������ ����
	DWORD m_dwRecvDelayLimitTime;  // ���� ��Ŷ ������ ���� �ð�
	
	int  m_iPingTime;
	DWORD m_dwLastRecvTime;

	inline bool HasStyle( DWORD dwStyle ) const
	{
		if( m_dwStyle & dwStyle )
			return true;
		return false;
	}
	inline bool IsNatUser() const { return m_bNatUser; }
	inline bool IsServerRelay() const
	{
		if( strcmp( m_addrSend.m_IP, m_addrTransfer.m_IP ) == 0 )
			return true;
		return false;
	}

	// ��� UDP�޼����� ���� �ֱ� 100�� �ε���
	typedef std::deque<int> RecvIndexQueue;
	RecvIndexQueue m_RecvIndexQueue;

	int   m_iLastControlIndex;			// Control �޼��� ������ �ε���
	//rudp
	DWORD m_dwUserSeed;
	DWORD m_dwNPCSeed;
	DWORD m_dwLastHostTime;
};

class ioUserNetworkTable
{
public:
	typedef std::map< ioHashString, UserNetworkInfo* > NetworkTable;
	typedef ioMapIterator< NetworkTable > NetworkInfoIter;

protected:
	NetworkTable m_NetworkTable;
	DWORD m_dwProcessTime;

public:
#ifndef ANTIHACK
	UserNetworkInfo* AddNewUser( const DWORD dwUserIndex,
		const ioHashString &rkName,
		const char *szPublicIP,
		const char *szPrivateIP,
		int iPort,
		const char *szTransferIP,
		int iTransferPort,
		DWORD dwStyle,
		int nDebugValue );
#else
	UserNetworkInfo* AddNewUser( const DWORD dwUserIndex,
		const ioHashString &rkName,
		const char *szPublicIP,
		const char *szPrivateIP,
		int iPort,
		const char *szTransferIP,
		int iTransferPort,
		DWORD dwStyle, DWORD dwUserSeed, DWORD dwNPCSeed );
#endif
	

	void RemoveUser( const ioHashString &rkName , DWORD dwStyle );
	void RemoveAllUser();
	void RemoveAllUser( DWORD dwStyle );

	void CheckValidateNetTable( ioPlayStage *pStage );

public:
	void ServerToClientPortChange( const ioHashString &rkName, 
		                           const ioHashString &szPublicIP,					
								   const ioHashString &szPrivateIP,						
								   const ioHashString &szTransferIP,
								   int iPort,
								   int iTransferPort );

	void ClientToClientPortChange( const ioHashString &rkName, const char *szRcvIP, int iRcvPort );
	
public:
	void SetLastControlIndex( const ioHashString &rkName, int iIndex );
	int  GetLastControlIndex( const ioHashString &rkName );

public:
	const char *GetUserGlobalIP( const ioHashString &rkName );

public:
	//void  SetLastRecvTime( const ioHashString &rkName, DWORD dwHostTime, DWORD dwGlobalTime );
	void  SetLastRecvTime( const ioHashString &rkName, DWORD dwPingTime );
	int   GetP2pPingTime( const ioHashString &rkName );
	bool  IsServerRelay( const ioHashString &rkName );

public:
	bool CheckDuplicateIndex( UserNetworkInfo *pInfo, int iCurIndex );
	
public:
	bool  IsNetworkTimeOut( const ioHashString &rkName );
	bool  IsAllNetworkTimeOut();
	bool  IsNetworkTableEmpty(){ return m_NetworkTable.empty(); }

public:
	NetworkInfoIter GetNetworkInfoIter();
	UserNetworkInfo* GetNetworkInfo( const ioHashString &rkName );
	UserNetworkInfo* GetNetworkInfo( const char *szIP , const int iPort);
	UserNetworkInfo* GetNetworkInfo( DWORD dwUserIndex );

public:
	void HoleSendProcess();
	void HoleRecvProcess();

public:
	void HoleSendComplete( const ioHashString &rkName, const ioHashString &szIP, int iPort );
	void SetUDPRecvTimeOut( const ioHashString &rkName );
	void ReCheckUDPRecvTimeOut( const ioHashString &rkName );
	void AllNetworkTableServerRelay();

public:
	// for penguin ping log
	int GetNetworkCnt() { return m_NetworkTable.size(); }

#ifdef ANTIHACK
	DWORD GetControlSeed( const ioHashString &rkName, bool bUser );
	bool CheckControlSeed( const ioHashString &rkName, DWORD dwSeed, DWORD dwTime, bool bUser );
#endif

public:
	ioUserNetworkTable();
	~ioUserNetworkTable();
};

