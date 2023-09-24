#pragma once

class SP2Packet;
class ioPlayStage;

enum
{
	UP_HEADQUARTERS = 1,                //�Ʒü�
	UP_LOBBY,                           //�κ�
	UP_TRAINING,                        //����
	UP_BATTLE_ROOM,                     //������
	UP_LADDER_TEAM,                     //������
	UP_TOURNAMENT,						//��ȸ
	UP_SHUFFLE,							//�����Ǹ��
	UP_GUILD_HQ,						//��庻��
	UP_HOUSE,							//���κ���
};

class FriendNode
{
protected:
	int          m_iIndex;
	DWORD        m_dwUserIndex;
	ioHashString m_FriendID;
	bool         m_bOnline;
	int          m_iUserPos;
	int          m_iGradeLevel;
	bool		 m_bOtherBF;             //������ ��ģ ��������
	DWORD        m_dwRegTime;
	int          m_iFriendRegSec;

	//Clover
	int m_iCloverSendCount;
	int m_iCloverSendDate;
	int m_iCloverRecvCount;
	int m_iCloverRecvDate;
	int m_iCloverRecvWaitCount;
	int m_iCloverSendSec;
	int m_iCloverRecvSec;

public:
	int   GetIndex(){ return m_iIndex; }
	DWORD GetUserIndex(){ return m_dwUserIndex; }
	const ioHashString &GetName(){ return m_FriendID; }
	bool  IsOnline(){ return m_bOnline; }
	int   GetUserPos(){ return m_iUserPos; }
	int   GetGradeLevel(){ return m_iGradeLevel; }
	bool  IsOtherBF(){ return m_bOtherBF; }
	DWORD GetRegTime(){ return m_dwRegTime; }

public:
	void  SetOnline( bool bOnline ){ m_bOnline = bOnline; }
	void  SetUserPos( int iUserPos ){ m_iUserPos = iUserPos; }
	void  SetGradeLevel( int iGradeLevel );
	void  SetName( const ioHashString &rszName ) { m_FriendID = rszName; }
	void  SetOtherBF( bool bOtherBF ){ m_bOtherBF = bOtherBF; }

// Clover(12.11.23)
public:
	int   GetCloverSendCount() { return m_iCloverSendCount; }
	int   GetCloverRecvCount() { return m_iCloverRecvCount; }
	int   GetCloverRecvWaitCount() { return m_iCloverRecvWaitCount; }

	void  SetCloverSendCount( int iSendCount ) { m_iCloverSendCount = iSendCount; }
	void  SetCloverRecvCount( int iRecvCount ) { m_iCloverRecvCount = iRecvCount; }
	void  SetCloverRecvWaitCount( int RecvWaitCount ) { m_iCloverRecvWaitCount = RecvWaitCount; }

	int   GetCloverSendDate() { return m_iCloverSendDate; }
	int   GetCloverRecvDate() { return m_iCloverRecvDate; }

	void  SetCloverSendTime( int dwSendDate ) { m_iCloverSendSec = dwSendDate; }
	void  SetCloverRecvTime( int dwRecvDate ) { m_iCloverRecvSec = dwRecvDate; }

	int   GetCloverSendSec() { return m_iCloverSendSec; }
	int   GetCloverRecvSec() { return m_iCloverRecvSec; }

	void  SetCloverSendSec( int iSendSec ) { m_iCloverSendSec = iSendSec; }
	void  SetCloverRecvSec( int iRecvSec ) { m_iCloverRecvSec = iRecvSec; }

	void  DecreaseCloverSendSec();
	void  DecreaseCloverRecvSec();

	int   GetFriendRegSec() { return m_iFriendRegSec; }
	int   SetFriendRegSec( int iRegSec ) { return m_iFriendRegSec = iRegSec; }
	void  DecreaseFriendRegSec();

	void ApplyFriendClover( SP2Packet &rkPacket );

	void InitCloverSendSec();
	void InitCloverRecvSec();
	void InitCloverRegSec();

public:
	FriendNode( int iIndex, DWORD dwUserIndex, ioHashString FriendID, bool bOnline, int iUserPos, int iGradeLevel, bool bOtherBF, DWORD dwRedTime,
				int iCloverSendCount, int iCloverSendTime, int iCloverRecvCount, int iCloverRecvTime, int iCloverRecvWaitCount );
	virtual ~FriendNode();
};
typedef std::vector< FriendNode* > vFriendNode;
typedef vFriendNode::iterator vFriendNode_iter;

enum
{	// ���� ���� Ÿ��
	BFT_NONE		= 0,    // ���� �ƴ�
	BFT_SET			= 1,	// ���� ����
	BFT_DISMISS     = 2,    // ���� ������.
};
typedef struct tagBestFriend
{
	enum
	{
		DEFAULT_YEAR    = 2010,			// 2010���� DB�� �������� �ʴ´�. �� DateData�� �⵵�� 0�̸� 2010�̶� ���̴�. 1�̸� 2011��
		DATE_YEAR_VALUE = 100000000,    // ����� ������.
		DATE_MONTH_VALUE= 1000000,      // ������ ������.
		DATE_DAY_VALUE =  10000,        // �ϱ��� ������.
		DATE_HOUR_VALUE = 100,          // �ñ��� ������.
	};

	DWORD m_dwIndex;          // ���� �ε���
	DWORD m_dwUserIndex;      // ���� ���� �ε���
	DWORD m_dwState;		  // ���� ���� - ������.. / �뺴 �뿩 
	DWORD m_dwMagicDate;      // ���� ���� ���� �ð� - ������ ���� ��� �ð� / �뿩�� ���� �뿩 ���� �ð�

	// Date
	DWORD GetYear(){ return DEFAULT_YEAR + ( m_dwMagicDate / DATE_YEAR_VALUE ); }
	DWORD GetMonth(){ return ( m_dwMagicDate % DATE_YEAR_VALUE ) / DATE_MONTH_VALUE; }
	DWORD GetDay(){	return ( m_dwMagicDate % DATE_MONTH_VALUE ) / DATE_DAY_VALUE; }
	DWORD GetHour(){ return ( m_dwMagicDate % DATE_DAY_VALUE ) / DATE_HOUR_VALUE; }
	DWORD GetMinute(){ return ( m_dwMagicDate % DATE_HOUR_VALUE ); }
	tagBestFriend()
	{
		m_dwIndex = m_dwUserIndex = 0;
		m_dwState       = BFT_NONE;
		m_dwMagicDate   = 0;       
	}
}BestFriend;
typedef std::vector< BestFriend > vBestFriend;
typedef vBestFriend::iterator vBestFriend_iter;

#define SEND_FRIEND_LIST_COUNT               18               // ��Ŷ ������ ���̱� ���� 18���� ����
#define SEND_FRIEND_REQUEST_LIST             10

class ioFriendManager : public Singleton< ioFriendManager >
{
	enum
	{
		NEW_START = 0,
		MESSENGER_OPEN = 1,
		NOT_VALUE = 2,

		MAX_FRIEND_IDX_VALUE = 2147483647,
	};
protected:
	vFriendNode   m_vFriendNode;
	vBestFriend   m_vBestFriend;
	bool	m_bLoadingCompelte;
	DWORD	m_dwLastLoadingTimer;
	int     m_iMessengerState;
	int     m_iDefaultSlotSize;
	bool    m_bBestFriendCheck;

	// ģ���߰��� Ȯ���� ���� �ӽÿ뵵 ����
	ioHashString m_InsertedFriend;

	// ģ�� ��û�� ����
	struct FriendRequestUser
	{
		DWORD m_dwTableIndex;
		DWORD m_dwUserIndex;
		ioHashString m_szUserName;
		FriendRequestUser()
		{
			m_dwTableIndex = 0;
			m_dwUserIndex  = 0;
		}
	};
	typedef std::vector< FriendRequestUser > vFriendRequestUser;
	vFriendRequestUser m_vFriendRequestUser;
	
protected:
	ioPlayStage *m_pPlayStage;

public:
	void SetPlayStage( ioPlayStage *pPlayStage ){ m_pPlayStage = pPlayStage; }

public:
	void RemoveNode( FriendNode *node );
	void RemoveBestFriend( DWORD dwUserIndex );
	
public:
	int GetNodeSize(){ return m_vFriendNode.size(); }
	int GetBestFriendSize(){ return m_vBestFriend.size(); }
	int GetOnlineNodeSize();
	int GetFriendRequestSize(){ return m_vFriendRequestUser.size(); }
	DWORD GetFriendRequestLastIndex();
	bool  IsAlreadyRequestUser( DWORD dwTableIndex );
	const char *GetFriendRequestUser();
	const char *GetFriendRequestUser( DWORD &dwTableIndex, DWORD &dwUserIndex );

	bool IsFriend( ioHashString szName );
	FriendNode *GetNode( int iArray );
	FriendNode *GetNodeOnlineUser( int iArray );
	FriendNode *GetNode( const ioHashString &rkName );
	FriendNode *GetNodeToUserIndex( const DWORD &dwUserIndex );
	DWORD GetBestFriendState( const ioHashString &rkName );
	DWORD GetBestFriendState( const DWORD dwUserIndex );
	int GetBestFriendStateCount( DWORD dwState );

	BestFriend &GetBestFriendFirstDismiss();
	BestFriend &GetBestFriend( const ioHashString &rkName );
	BestFriend &GetBestFriend( const DWORD dwUserIndex );
	void CheckBestFriend();

	int GetSlotSize();
	inline int GetDefaultSlotSize() const { return m_iDefaultSlotSize; }
		
public:
	void SetUpdate();
	void ChangeName( const ioHashString &rszName, const ioHashString &rszNewName );

public:	
	bool IsLoadingComplete();

public:
	void OnLogInUser( ioHashString szName, int iGradeLevel );
	void OnLogOutUser( ioHashString szName );
	void OnOtherBestFriend( ioHashString szName, bool bOtherBestFriend );
	void OnUserPos( ioHashString szName, int iUserPos, int iUserLevel, DWORD dwGuildIndex, DWORD dwGuildMark );
	void OnUserPos(  ioHashString szName, int iUserPos, int iUserLevel );

public:
	void SendFriendList();
	void SendFriendRequestList();
	void DeleteFriend( ioHashString szName );
	void DeleteRequestFriend( const ioHashString szName );
	void ApplyFriendList( SP2Packet &rkPacket );
	void ApplyFriendRequestUser( SP2Packet &rkPacket );
	void ClientFriendInfoUpdate( ioHashString szName, int iGradeLevel, int iUserPos = UP_LOBBY );

public:
    bool AlreadyBestFriendCheck( DWORD dwTableIndex );
	void ApplyBestFriendList( SP2Packet &rkPacket );
	void InsertBestFriend( DWORD dwTableIndex, DWORD dwFriendIndex );
	void ChangeBestFriendData( DWORD dwUserIndex, DWORD dwState, DWORD dwMagicDate );

public:
	void LoadINIInfo( ioINILoader &rkLoader );

public:
	const ioHashString& GetInsertedFriend() const { return m_InsertedFriend; }

public:
	static ioFriendManager &GetSingleton();
		
public:
	ioFriendManager();
	virtual ~ioFriendManager();	
};
#define g_FriendManager ioFriendManager::GetSingleton()
