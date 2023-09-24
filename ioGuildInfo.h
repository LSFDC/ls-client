#pragma once

#include "EncryptValue.h"
#include "ioGashaponMgr.h"

class SP2Packet;
class ioGuildInfo;
class ioGuildData
{
	friend class ioGuildInfo;
protected:
	DWORD			m_dwGuildIndex;
	DWORD			m_dwGuildMark;
	DWORD			m_dwGuildRank;
	DWORD			m_dwGuildLevel;
	DWORD			m_dwGuildJoinUser;
	DWORD			m_dwGuildMaxUser;
	DWORD			m_dwGuildPoint;
	DWORD			m_dwCurGuildPoint;
	DWORD			m_dwGuildRegDate;
	float			m_fGuildBonus;

	ioHashString	m_szGuildName;
	ioHashString	m_szGuildTitle;

	int				m_iGuildYesterdayAttendCount;	//���� �⼮�� ����
	int				m_iGuildTodayAttendCount;		//���� �⼮�� ����

	BOOL			m_bGuildRoomOpen;

	struct GuildUser
	{
		DWORD			m_dwTableIndex;
		DWORD			m_dwUserIndex;

		int				m_iUserLevel;
		int				m_iUserPos;
		int				m_iSortPoint;

		ioHashString	m_szUserID;
		ioHashString	m_szUserPos;
		
		bool			m_bOnline;				//�ڽ��� ������ ģ���� �����ϰ� ����ȭ ��Ų��.		
		bool			m_bAttendedYesterday;	//���� �⼮����
		bool			m_bAttendedToday;		//���� �⼮����

		GuildUser()
		{
			m_dwTableIndex			= 0;
			m_dwUserIndex			= 0;
			m_iUserLevel			= 0;
			m_iSortPoint			= 0;
			m_iUserPos				= 1;

			m_bOnline				= false;

			m_bAttendedYesterday	= false;
			m_bAttendedToday		= false;			
		}
	};
	class GuildUserSort : public std::binary_function< const GuildUser&, const GuildUser&, bool >
	{
	public:
		bool operator()( const GuildUser &lhs , const GuildUser &rhs ) const
		{
			if( lhs.m_iSortPoint > rhs.m_iSortPoint )
			{
				return true;
			}
			else if( lhs.m_iSortPoint == rhs.m_iSortPoint )
			{
				if( lhs.m_dwTableIndex < rhs.m_dwTableIndex )
				{
						return true;
				}
			}
			return false;
		}
	};
	typedef std::vector<GuildUser> vGuildUser;
	vGuildUser m_vUserList;

	DWORD      m_dwSyncTime;
	DWORD      m_dwSyncTitleTime;
	DWORD      m_dwSyncUserListTime;
	bool       m_bUserListUpdate;
	bool       m_bSync;          // �ѹ��̶� ����ȭ�� ��� �����ΰ�?

public:
	inline DWORD GetGuildIndex(){ return m_dwGuildIndex; }
	inline DWORD GetGuildMark(){ return m_dwGuildMark; }
	inline DWORD GetGuildRank(){ return m_dwGuildRank; }
	inline DWORD GetGuildLevel(){ return m_dwGuildLevel; }
	inline DWORD GetGuildJoinUser(){ return m_dwGuildJoinUser; }
	inline DWORD GetGuildMaxUser(){ return m_dwGuildMaxUser; }
	inline DWORD GetGuildPoint(){ return m_dwGuildPoint; }
	inline DWORD GetGuildCurPoint(){ return m_dwCurGuildPoint; }
	inline DWORD GetGuildRegYear(){ return m_dwGuildRegDate / 10000; }
	inline DWORD GetGuildRegMonth(){ return (m_dwGuildRegDate % 10000) / 100; }
	inline DWORD GetGuildRegDay(){ return m_dwGuildRegDate % 100; }
	inline float GetGuildBonus(){ return m_fGuildBonus; }
	inline ioHashString GetGuildName(){ return m_szGuildName; }
	inline ioHashString GetGuildTitle(){ return m_szGuildTitle; }
	inline int GetYesterdayAttendedCount() { return m_iGuildYesterdayAttendCount; }
	inline int GetTodayAttendedCount() { return m_iGuildTodayAttendCount; }
	inline BOOL GetGuildRoomOpen(){ return m_bGuildRoomOpen; }

	DWORD GetGuildMaster();
	const ioHashString &GetGuildMasterName();
	DWORD GetGuildSecondMaster();
	const ioHashString &GetGuildSecondMasterName();

	DWORD GetUserIndex( const ioHashString &szUserName );
	DWORD GetUserTableIndex( const ioHashString &szUserName );
	const ioHashString &GetUserName( DWORD dwUserIndex );

	int GetOnlineUserSize();
	void GetUserInfo( int iUserArray, DWORD &dwTableIndex, DWORD &dwUserIndex, int &iUserLevel, ioHashString &szUserID, ioHashString &szUserPos );
	void GetUserInfo( int iUserArray, ioHashString &szUserID, int &iUserLevel, int &iUserPos, bool &bOnline );
	void GetUserInfo( int iUserArray, ioHashString &szUserID, bool &bOnline );
	void GetUserInfo( int iUserArray, ioHashString &szUserID, int &iUserLevel, bool &bAttendYesterday, bool &bAttendToday );
	void GetUserPos( DWORD dwUserIndex, ioHashString& szUserPos );
	void FillUserList( SP2Packet &rkPacket );

	void ChangeGuildMaster( DWORD dwGuildMasterIndex );
	void ChangeGuildPosition( DWORD dwTargetIndex, const ioHashString &szChangePosition );
	void ChangeUserID( const ioHashString &rszUserID, const ioHashString &rszNewUserID );

public:
	 void SetGuildIndex( DWORD dwGuildIndex );
	 void SetGuildCampPos( DWORD dwGuildCampPos );
	 void SetGuildMark( DWORD dwGuildMark );
	 void SetGuildRank( DWORD dwGuildRank );
	 void SetGuildLevel( DWORD dwGuildLevel );
	 void SetGuildJoinUser( DWORD dwGuildJoinUser );
	 void SetGuildMaxUser( DWORD dwGuildMaxUser );
	 void SetGuildPoint( DWORD dwGuildPoint );
	 void SetGuildCurPoint( DWORD dwCurGuildPoint );
	 void SetGuildRegDate( DWORD dwGuildRegDate );	
	 void SetGuildBonus( float fGuildBonus );
	 void SetGuildName( const ioHashString &szGuildName );
	 void SetGuildTitle( const ioHashString &szGuildTitle );
	 void SetGuildUserLogIn( const ioHashString &szUserID );
	 void SetGuildUserLogOut( const ioHashString &szUserID );
	 void SetGuildUserPos( const ioHashString &szUserID, int iUserPos, int iUserLevel );     // ��� ��å�� �ƴ϶� �Ʒ�/����/������ ��ġ��.

	 void SetGuildInfo( SP2Packet &rkPacket );
	 void SetGuildUserList( SP2Packet &rkPacket );
	 void SetGuildUserListEx( SP2Packet &rkPacket );
	 void SetGuildRoomOpen( BOOL bGuildRoomOpen );

	 void LeaveGuildUser( const ioHashString &szUserName );


	 //��� �⼮ ����
public:
	 void SetMyGuildMemberTodayAttendInfoByPacket( SP2Packet &rkPacket );
	 void SetMyGuildMemberYesterdayAttendInfoByPacket( SP2Packet &rkPacket );
	 void SetGuildTodayAttendedUser( DWORD dwUserIndex );
	 void SetGuildYesterdayAttendedUser( DWORD dwUserIndex );
protected:
	 void InitGuildTodayAttendedMember();
	 void InitGuildYesterdayAttendedMember();
	 

public:
	void InitSyncTimer(){ m_dwSyncTime = 0; }
	void ProcessSync( bool bMyGuildOnlyInfo = false );
	void ProcessTitleSync();
	void ProcessUserListSync();
	bool IsUserListUpdate();
	bool IsSync(){ return m_bSync; }
	void Sync(){ m_bSync = true; }

protected:
	ioGuildData();
	virtual ~ioGuildData();
};

struct GuildRewardItemInfo
{
	int					m_iPresentType;	//PresentType
	int					m_iValue1;		//PresentValue1
	int					m_iValue2;		//PresentValue2
	
	bool IsEmpty()
	{
		if ( m_iPresentType == 0 && m_iValue1 == 0 && m_iValue2 == 0 )
			return true;
		return false;
	}
	GuildRewardItemInfo()
	{
		m_iPresentType = m_iValue1 = m_iValue2 = 0;
	}
};

struct GuildAttendRwardInfo
{
public:
	ioHashString			 m_szName;
	int						 m_iMaxAttendedCount;
	GuildRewardItemInfo		 m_RewardItem;			  //ȹ�� ������(��í)
	vGashaponPresent		 m_RewardPossibleItemVec; //ȹ�� ���� ���(��í �ȿ� ���빰)
};
typedef std::vector< GuildAttendRwardInfo* > GuildAttendRewardVector;

struct GuildRankRwardInfo
{
public:
	ioHashString			 m_szName;
	int						 m_iRank;
	GuildRewardItemInfo		 m_RewardItem;			  //ȹ�� ������(��í)
	vGashaponPresent		 m_RewardPossibleItemVec; //ȹ�� ���� ���(��í �ȿ� ���빰)
};
typedef std::vector< GuildRankRwardInfo* > GuildRankRewardVector;

class ioGuildInfo : public Singleton< ioGuildInfo >
{
public:
	enum
	{
		GUILD_EVENT_NONE			= 0,	//�̺�Ʈ����
		GUILD_JOIN					= 1,	//��� ����
		GUILD_LEAVE					= 2,	//Ż������
		GUILD_SECOND_MASTER			= 3,	//�α渶 �Ӹ�
		GUILD_MASTER				= 4,	//�渶 ����
		GUILD_SECOND_MASTER_DISMISS = 5,	//�α渶 ����
		GUILD_DELETE				= 6,	//��� ����
		GUILD_CREATE				= 7,	//��� ����
		GUILD_MARK_BLOCK			= 8,	//��� ��ũ ��
		GUILD_HOME_ADMIN_SET		= 9,	//��� ���� �������� �ο�
		GUILD_HOME_ADMIN_RELEASE	= 10	//��� ���� �������� ����
	};

protected:
	//��� INI ����
	int m_iGuildCreatePeso;
	int m_iGuildMarkChangePeso;
	int m_iGuildCreateLimitLevel;
	CEncrypt<int> m_iGuildKickOutPeso;
	CEncrypt<int> m_iGuildLeavePeso;
	DWORDVec m_dwLevelToEntryCount;
	
	//���� ����� ��� ����Ʈ
	DWORDVec m_dwEntryDelayGuildList;       

	//�� ��� ����
	DWORD m_dwGuildIndex;
	ioHashString m_szGuildName;
	ioHashString m_szGuildPosition;

	//��ü ��� ������
	typedef std::vector< ioGuildData * > vGuildData;
	typedef vGuildData::iterator vGuildData_iter;
	vGuildData m_vGuildList;

	//��� ������ ���� ����Ʈ
	struct InvitationUser
	{
		ioHashString m_szUserName;
		DWORD m_dwInvitationTimer;
		InvitationUser()
		{
			m_dwInvitationTimer = 0;
		}
	};
	typedef std::vector< InvitationUser > vInvitationUser;
	vInvitationUser m_vInvitationUser;
	
	//��� ��ũ ������. . . .
	bool m_bGuildMarkChanging;

	// ��� �⼮, ��ũ ���� INI ����
	int	m_iAttendRenewalHour;	//��� �⼮ ���� �� ���� ���� �ð�.
	GuildAttendRewardVector	m_AttendRewardVec;
	GuildRankRewardVector	m_RankRewardVec;

	//��� �⼮ ����
	CTime		 m_GuildCreatedTime;		//��� ���� �ð�.
	CTime		 m_EnableAttendTime;		//�⼮ ������ �ð�.
	CTime		 m_EnableRewardTime;		//���� �ޱ� ������ �ð�.
	CTime		 m_LoginTime;				//�α��νð�
	CTime		 m_MemberAttendInfoRequestTime;

public:
	void Initialize();
	//��� �⼮����
	void InitializeGuildReward();
public:
	inline int GetGuildCreatePeso(){ return m_iGuildCreatePeso; }
	inline int GetGuildMarkChangePeso(){ return m_iGuildMarkChangePeso; }
	inline int GetGuildCreateLimitLevel(){ return m_iGuildCreateLimitLevel; }
	inline int GetGuildKickOutPeso(){ return m_iGuildKickOutPeso; }
	inline int GetGuildLeavePeso(){ return m_iGuildLeavePeso; }
	DWORD GetGuildLevelToMaxEntry( DWORD dwGuildLevel );

public:     //�� ��� ����
	void InitMyGuildData();
	void SetMyGuildData( DWORD dwIndex, const ioHashString &szGuildName, const ioHashString &szGuildPosition, DWORD dwGuildMark, BOOL bGuildRoomOpen );

	inline DWORD GetGuildIndex(){ return m_dwGuildIndex; }
	inline ioHashString GetGuildPosition(){ return m_szGuildPosition; }
	bool IsGuildMaster();
	bool IsGuildSecondMaster();
	DWORD GetMyGuildLevel();
	void SetMyGuildPosition( const ioHashString &szGuildPosition ){ m_szGuildPosition = szGuildPosition; }
	void SetMyGuildUserList( SP2Packet &rkPacket );

	void SetMyGuildUserLogIn( const ioHashString &szUserID );
	void SetMyGuildUserLogOut( const ioHashString &szUserID );
	void SetMyGuildUserPos( const ioHashString &szUserID, int iUserPos, int iUserLevel );     // ��� ��å�� �ƴ϶� �Ʒ�/����/������ ��ġ��.

	void SetGuildMarkChanging( bool bChange ){ m_bGuildMarkChanging = bChange; }
	
	void ChangeMyGuildUserID( const ioHashString &szUserID, const ioHashString &szNewUserID );

public:
	bool IsGuildEntryDelay( DWORD dwGuildIndex );
	bool IsGuildEntryDelayListDelete();
	bool IsMyGuild( DWORD dwGuildIndex );
	bool IsMyGuildUser( const ioHashString &szUserID );
	bool IsGuild();
	bool IsGuildMarkChanging(){ return m_bGuildMarkChanging; }

public:
	void ApplyEntryDelayGuildList( SP2Packet &rkPacket );
	void AddEntryDelayGuildList( DWORD dwGuildIndex );
	void DeleteEntryDelayGuildList();
	void DeleteEntryDelayGuildList( DWORD dwGuildIndex );

	// ��� ��� ������ �����Ѵ�.
public:
	ioGuildData *GetGuildData( DWORD dwGuildIndex, bool bNetwork = false );
	ioGuildData *GetGuildDataExist( const ioHashString &rszGuildName );
	void SetGuildUserList( SP2Packet &rkPacket );
	void SetGuildListInfo( DWORD dwGuildIndex, DWORD dwGuildRank, DWORD dwGuildLevel, DWORD dwGuildMark, ioHashString szGuildName,
						   DWORD dwGuildJoinUser, DWORD dwGuildMaxUser, float fGuildBonus, int iGuildPoint );
	void SetGuildListInfo( SP2Packet &rkPacket );
	void SetGuildSimpleInfo( DWORD dwGuildIndex, const ioHashString &szGuildName, DWORD dwGuildMark );
	void SetGuildMark( DWORD dwGuildIndex, DWORD dwGuildMark );
	void SetGuildMark( const ioHashString &szUserID, DWORD dwGuildIndex, DWORD dwGuildMark );
	void SetGuildTitle( DWORD dwGuildIndex, const ioHashString &szGuildTitle );
	void SetGuildCampInfo( DWORD dwGuildIndex, DWORD dwGuildMark, DWORD dwGuildLevel, DWORD dwGuildRank, ioHashString szGuildName, DWORD dwCurGuildPoint );

public:
	void SetInvitationUser( const ioHashString &szInvitationUser );
	bool IsInvitationUser( const ioHashString &szInvitationUser );

	//��� �⼮ ����
protected:
	void LoadRankReward( ioINILoader& rkLoader );
	void LoadAttendReward( ioINILoader& rkLoader );
	void SetAttendRewardTime( CTime RecievedTime );
	void SetAttendTime( CTime AttendedTime );
public:
	int GetAttendRewardSize();
	int GetRankRewardSize();
	const GuildAttendRwardInfo* const GetAttendRewardInfo( int iIndex ) const ;
	const GuildRankRwardInfo* const GetRankRewardInfo( int iIndex ) const ;


	INT64 GetEnableAttendTime() { return m_EnableAttendTime.GetTime(); }
	INT64 GetEnableAttendRewardTime() { return m_EnableRewardTime.GetTime(); }
	INT64 GetAttendInfoRequestTime() { return m_MemberAttendInfoRequestTime.GetTime(); }

	void SetMyGuildTimeSetting( DWORD dwRecievedTime, DWORD dwCreatedTime, DWORD dwLoginTime );
	void SetMyAttendRewardTimeByPacket( SP2Packet &rkPacket );
	void SetMyGuildMemberTodayAttendInfoByPacket( SP2Packet &rkPacket );
	void SetMyGuildMemberYesterdayAttendInfoByPacket( SP2Packet &rkPacket );
	void SetMyGuildAttendedUserInfoByPacket( SP2Packet &rkPacket, bool& bIsMe );
	bool CheckGuildCreatedTime();
	bool CheckEnableAttendRewardTime();
	bool CheckEnableAttendTime();
	bool CheckMemberAttendInfoRequestTime();
	//

public:
	static ioGuildInfo& GetSingleton();

public:
	ioGuildInfo();
	virtual ~ioGuildInfo();
};

#define g_GuildInfo ioGuildInfo::GetSingleton()
