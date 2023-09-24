#pragma once


class SP2Packet;
enum
{
	BMT_ALL_MODE                = 0,  //��ȣ������
	BMT_RANDOM					= 1,  //���� ���
	BMT_CATCH					= 2,  //����Ż��
	BMT_CATCH_BOSS				= 3,  //����Ż�� - ����
	BMT_STONE					= 4,  //��¡��
	BMT_KING					= 5,  //���� ũ���
	BMT_SURVIVAL				= 6,  //������ġ
	BMT_TEAM_SURVIVAL			= 7,  //�� ������ġ
	BMT_TEAM_SURVIVAL_FIRST		= 8,  //�� ������ġ - �ʺ�
	BMT_STONE_BOSS				= 9,  //��¡�� - ����
	BMT_KING_BOSS				= 10, //���� ũ��� - ����
	BMT_TEAM_SURVIVAL_BOSS		= 11, //�� ������ġ - ����
	BMT_TEAM_SURVIVAL_FIRST_BOSS= 12, //�� ������ġ - �ʺ� - ����
	BMT_RANDOM_BOSS             = 13, //���� ��� - ����
	BMT_BOSS                    = 14, //�������
	BMT_MONSTER_SURVIVAL_EASY   = 15, //PvE��� - ����
	BMT_MONSTER_SURVIVAL_NORMAL = 16, //PvE��� - ����
	BMT_MONSTER_SURVIVAL_HARD   = 17, //PvE��� - �����
	BMT_USER_CUSTOM             = 18, //������� - ��������ġ & ����Ż��
	BMT_FOOTBALL				= 19, //�౸���
	BMT_FOOTBALL_BOSS			= 20, //�౸��� - ����
	BMT_GANGSI                  = 21, //���ø��
	BMT_DUNGEON_A_EASY			= 22, //������� - ���
	BMT_CATCH_RUNNINGMAN		= 23, //����Ż�� ���׸�
	BMT_CATCH_RUNNINGMAN_BOSS	= 24, //����Ż�� ���׸� - ����
	BMT_FIGHT_CLUB				= 25, //����ƮŬ��
	BMT_TOWER_DEFENSE_EASY		= 26, //tower defense
	BMT_TOWER_DEFENSE_NORMAL	= 27, 
	BMT_TOWER_DEFENSE_HARD		= 28,  //UNDONE : tower easy normal hard
	BMT_TOWER_DEFENSE_CHALLENGE	= 29,
	BMT_DARK_XMAS_EASY			= 30,
	BMT_DARK_XMAS_NORMAL		= 31,
	BMT_DARK_XMAS_HARD			= 32,
	BMT_DARK_XMAS_CHALLENGE		= 33,
	BMT_FIRE_TEMPLE_EASY		= 34,
	BMT_FIRE_TEMPLE_NORMAL		= 35,
	BMT_FIRE_TEMPLE_HARD		= 36,
	BMT_FIRE_TEMPLE_CHALLENGE	= 37,
	BMT_DOUBLE_CROWN			= 38,
	BMT_DOUBLE_CROWN_BOSS		= 39,
	BMT_FACTORY_EASY			= 40,
	BMT_FACTORY_NORMAL			= 41,
	BMT_FACTORY_HARD			= 42,
	BMT_FACTORY_CHALLENGE		= 43,
	BMT_TEAM_SURVIVAL_AI_EASY	= 44,
	BMT_TEAM_SURVIVAL_AI_HARD	= 45,
	BMT_UNDERWEAR				= 46,
	BMT_CBT						= 47,
	BMT_RAID					= 48,
	BMT_MAX,
};

// ������ �̺�Ʈ Ÿ��
enum
{
	BET_NORMAL = 0,             // �Ϲ� ������
	BET_BROADCAST_AFRICA = 1,   // AFRICA ��� ��
	BET_BROADCAST_MBC = 2,      // MBC ��� ��
	BET_TOURNAMENT_BATTLE = 3,  // ��ȸ ��� ������
};

typedef struct tagBattleRoomInfo
{
	DWORD m_dwIndex;	
	ioHashString m_szName;
	ioHashString m_szOwnerName;
	ioHashString m_szPassword;
	
	int  m_iCurJoiner;
	int  m_iCurPlayer;
	int  m_iMaxPlayerBlue;
	int  m_iMaxPlayerRed;
	int  m_iMaxObserver;
	int  m_iAverageLevel;	
	
	// �� �� �� ����
	int  m_iBlueWin;
	int  m_iBlueLose;
	int  m_iBlueVictories;
	int  m_iRedWin;
	int  m_iRedLose;
	int  m_iRedVictories;

	// �ɼ�
	bool m_bRandomTeamMode;
	bool m_bStartRoomEnterX;
	bool m_bAutoModeStart;
	bool m_bBadPingKick;
	bool m_bNoChallenger;
	CEncrypt<bool> m_bUseExtraOption;

	CEncrypt<int> m_iPreSetMode;

	int m_iTeamAttackType;
	int m_iChangeCharType;
	int m_iCoolTimeType;
	CEncrypt<int> m_iRedHPType;
	CEncrypt<int> m_iBlueHPType;
	int m_iDropDamageType;
	int m_iGravityType;
	int m_iGetUpType;
	int m_iKOType;
	int m_iKOEffectType;
	int m_iRedBlowType;
	int m_iBlueBlowType;
	int m_iRedMoveSpeedType;
	int m_iBlueMoveSpeedType;
	int m_iRedEquipType;
	int m_iBlueEquipType;

	int m_iCatchModeRoundType;
	int m_iCatchModeRoundTimeType;

	int m_iGrowthUseType;
	int m_iExtraItemUseType;
	
	int  m_iBattleEventType;

	int m_iAiLevel;

	// ��ȸ ������
	DWORD m_dwTourIndex;
	DWORD m_dwBlueTeamIndex;
	DWORD m_dwRedTeamIndex;
	DWORD m_dwTourStartTimer;
	bool  m_bTourStartSender;
	
	 //�������̽� ����
	enum
	{
		BRS_ACTIVE				= 1,
		BRS_FULL_USER			= 2,
		BRS_TIME_CLOSE			= 3,
		BRS_ENTER_X				= 4,
		BRS_NOT_MIN_LEVEL_MATCH = 5,
		BRS_NOT_MAX_LEVEL_MATCH = 6,
		BRS_ALREADY_ROOM		= 7,
		BRS_FULL_OBSERVER		= 8,
		BRS_MAP_LIMIT_GRADE     = 9,
		BRS_OPTION_LIMIT_GRADE  = 10,
	};
	int   m_iNumber;
	int   m_iState;      
	bool  m_bPassword; 
	int   m_iModeType;
	int   m_iModeSelectIndex;
	int   m_iMaxPlayer;
	int   m_iPlayState;
	int   m_iBlueTeamWinCount;
	int   m_iRedTeamWinCount;
	DWORD m_dwRemainPlayTime;
	tagBattleRoomInfo()
	{
		m_dwIndex				= 0;
		m_iCurJoiner			= 0;
		m_iCurPlayer			= 0;
		m_iMaxPlayerBlue		= 0;
		m_iMaxPlayerRed			= 0;
		m_iMaxObserver			= 0;
		m_iAverageLevel			= 0;

		m_bRandomTeamMode = true;
		m_bStartRoomEnterX= false;
		m_bAutoModeStart  = false;
		m_bBadPingKick	  = true;		
		m_bUseExtraOption = false;
		m_bNoChallenger = true;

		m_iPreSetMode = 0;

		m_iTeamAttackType = 0;
		m_iChangeCharType = 0;
		m_iCoolTimeType = 0;
		m_iRedHPType = 0;
		m_iBlueHPType = 0;
		m_iDropDamageType = 0;
		m_iGravityType = 0;
		m_iGetUpType = 0;
		m_iKOType = 0;
		m_iKOEffectType = 0;
		m_iRedBlowType = 0;
		m_iBlueBlowType = 0;
		m_iRedMoveSpeedType = 0;
		m_iBlueMoveSpeedType = 0;
		m_iRedEquipType = 0;
		m_iBlueEquipType = 0;

		m_dwTourIndex	 = 0;
		m_dwBlueTeamIndex= 0;
		m_dwRedTeamIndex = 0;
		m_dwTourStartTimer = 0;
		m_bTourStartSender = false;

		m_iCatchModeRoundType = 0;
		m_iCatchModeRoundTimeType = 0;

		m_iGrowthUseType = 0;
		m_iExtraItemUseType = 0;

		m_iBattleEventType      = BET_NORMAL;

		m_iBlueWin				= 0;
		m_iBlueLose				= 0;
		m_iBlueVictories		= 0;
		m_iRedWin				= 0;
		m_iRedLose				= 0;
		m_iRedVictories			= 0;

		m_iNumber               = 0;
		m_iState                = BRS_ACTIVE;
		m_bPassword				= false;
		m_iModeType				= MT_NONE;
		m_iModeSelectIndex      = BMT_RANDOM;
		m_iMaxPlayer			= 0;

		m_iPlayState			= 0;
		m_iBlueTeamWinCount		= 0;
		m_iRedTeamWinCount		= 0;
		m_dwRemainPlayTime		= 0;
		m_iAiLevel				= 0;
	}
}BattleRoomInfo;

#define MAX_PARTY_PLAYER       16
#define MAX_OBSERVER			4
typedef struct tagBattleRoomUserInfo
{
	DWORD        m_dwUserIndex;
	ioHashString m_szUserID;
	int          m_iGradeLevel;
	int          m_iAbilityLevel;
	bool         m_bSafetyLevel;
	bool		 m_bObserver;
	int          m_iTeam;
	int          m_iMonsterCoin;
	int          m_iGoldMonsterCoin;
	int          m_iRaidCoin;
	int          m_iServerPingStep;
	DWORD        m_dwMonsterCoinFirstSyncTime;
	DWORD        m_dwCharIndex;
	DWORD        m_dwCharIndexFirstSyncTime;
	DWORD        m_dwCharMotionItem;
	DWORD        m_dwCharMotionFirstSyncTime;

	ioHashString m_szPublicIP;
	ioHashString m_szPrivateIP;
	ioHashString m_szTransferIP;
	int          m_iPort;
	int          m_iTransferPort;

	tagBattleRoomUserInfo()
	{
		m_dwUserIndex       = 0;
		m_iTeam				= 0;
		m_iMonsterCoin      = 0;
		m_iGoldMonsterCoin  = 0;
		m_iRaidCoin  = 0;
		m_iServerPingStep	= 0;
		m_iGradeLevel		= 0;
		m_iAbilityLevel     = 0;
		m_bSafetyLevel		= false;
		m_bObserver			= false;
		m_iPort				= 0;
		m_iTransferPort     = 0;
		m_dwMonsterCoinFirstSyncTime = 0;
		m_dwCharIndex		= 0;
		m_dwCharIndexFirstSyncTime = 0;
		m_dwCharMotionItem  = 0;
		m_dwCharMotionFirstSyncTime = 0;
	}
}BattleRoomUserInfo;

typedef struct tagQuickStartInfo
{
	int  m_iQuickStartType;
	int  m_iQuickStartMaxUser[BMT_MAX];
	int  m_iQuickStartMinUser[BMT_MAX];
	bool m_bQuickStartSameUser[BMT_MAX];
	tagQuickStartInfo()
	{
		m_iQuickStartType = -1;
		for(int i = 0;i < BMT_MAX;i++)
		{
			m_iQuickStartMaxUser[i] = 0;
			m_iQuickStartMinUser[i] = MAX_PLAYER;
			m_bQuickStartSameUser[i]= false;
		}
	}
}QuickStartInfo;

class ioBattleRoomMgr : public Singleton< ioBattleRoomMgr >
{
protected:
	BattleRoomInfo m_BattleRoom;
	typedef std::vector< BattleRoomUserInfo > vUserInfo;
	vUserInfo m_vUserList;
	
	int  m_iSelectModeIndex;
	int  m_iSelectMapIndex;
	bool m_bSafetyLevelRoom;

	//���� ����
	struct KickOutRoom
	{
		DWORD   m_dwIndex;
		DWORD   m_dwKickOutTimer;
		KickOutRoom()
		{
			m_dwIndex		 = 0;
			m_dwKickOutTimer = 0;
		}
	};
	typedef std::vector< KickOutRoom > vKickOutRoom;
	vKickOutRoom m_vKickOutRoom;	

	// �ʴ� ���� ����
	struct InvitedRefuse
	{
		DWORD m_dwIndex;
		DWORD m_dwRefuseTimer;
		InvitedRefuse()
		{
			m_dwIndex       = 0;
			m_dwRefuseTimer = 0;
		}
	};
	typedef std::vector< InvitedRefuse > vInvitedRefuse;
	vInvitedRefuse m_vInvitedRefuse;

	ioHashString m_szPrevMyOwnerID;
	ioHashString m_szKickOutUser;

	//
	ioHashString m_szBlueTeamEmoticon;
	ioHashString m_szRedTeamEmoticon;

	// 
	ioHashString m_szStartSoundDef;
	ioHashString m_szStartSoundGo;
	ioHashStringVec m_StartSoundSec;
	//
	int  m_iDefaultMaxPlayer;
	int  m_iDefaultMaxObserver;
	int  m_iLimitJoinLevel;
	int  m_iBattleDefaultCatchModeLevel;
	DWORD m_dwAutoStartAlarmCheckTime;

	int  m_iBattleCancelCount;

	bool m_bBattleModePlaying;           // ��� �÷����� or ��� ������.

	int  m_iModeStartPlayerCount;        // ��� ���۽� �÷��̾� ��.

	// ��ȣ ���
	QuickStartInfo m_QuickStartInfo;

	// ���� �����
	bool m_bChangeMeHostAlarm;

	//
	DWORD m_dwProcessTimer;

public:
	void LoadBattleRoomInfo();
	void SaveQuickStartInfo( QuickStartInfo &rkQuickStartInfo );

	void InitRecord();

    void UserListClear();

public:
	bool IsBattleRoom();

public:
	QuickStartInfo GetQuickStartInfo();
	int  GetQuickStartModeType();
	int  GetQuickStartMaxPlayer( int iModeIndex );
	int  GetQuickStartMinPlayer( int iModeIndex );
	bool IsQuickStartSameUser( int iModeIndex );

public:
	int GetDefMaxPlayer(){ return m_iDefaultMaxPlayer; }
	int GetDefMaxObserver() { return m_iDefaultMaxObserver; }
	int GetLimitJoinLevel(){ return m_iLimitJoinLevel; }

	bool IsBattleModePlaying(){ return m_bBattleModePlaying; }
	void SetBattleModePlaying( bool bPlaying ){ m_bBattleModePlaying = bPlaying; }

public:
	void SetKickOutRoom( DWORD dwIndex );
	bool IsKickOutRoom( DWORD dwIndex );

public:
	void SetInvitedRefuse( DWORD dwIndex );
	bool IsInvitedRefuseRoom( DWORD dwIndex );

public:
	void InitBattleCancelCount(){ m_iBattleCancelCount = 0; }
	void SetBattleCancelCount();
	int  GetBattleCancelCount(){ return m_iBattleCancelCount; }
	
public:
	void ChangeMyOwner();
	bool ChangeMeHostAlarm();

public:
	void SetKickOutUser( ioHashString szName );
	ioHashString &GetKickOutUser(){ return m_szKickOutUser; }

public:
	void LeaveRoom( bool bMove = false );	

public:
	void UpdateUserLevel( const ioHashString &szUserID, int iGradeLevel );
	void UpdateUserAbilityLevel( const ioHashString &szUserID, int iAbilityLevel );
	void UpdateUserSafetyLevel( const ioHashString &szUserID, bool bSafetyLevel );
	void UpdateUserMonsterCoin( const ioHashString &szUserID, int iMonsterCoin, int iGoldMonsterCoin, int iRaidCoin );
	void UpdateUserCharIndex( const ioHashString &szUserID, DWORD dwCharIndex, bool bSync );
	void UpdateUserCharMotion( const ioHashString &szUserID, DWORD dwCharMotion );
	void JoinUser( BattleRoomUserInfo &kUser );
	void LeaveUser( ioHashString szUserID, ModeType eModeType );
	bool IsHaveUser( ioHashString &rszUserID );
	const BattleRoomUserInfo &GetUserInfo( int iArray );

	// ��Ƽ ����
	void SetBattleTeam( ioHashString szName, TeamType eTeam );
	const BattleRoomUserInfo &GetBattleUserInfo( TeamType eTeam, int iArray );
	const BattleRoomUserInfo &GetBattleObserverInfo( int iArray );
	int GetBattleTeamUserCount( TeamType eTeam );
	int GetBattleUserTeam( ioHashString szName );
	int GetBattleTeamAverageLevel( TeamType eTeam );
	void UpdateRecord( TeamType eWinTeam );

public:
	void SetTournamentData( DWORD dwTourIndex, DWORD dwBlueTeamIndex, DWORD dwRedTeamIndex, DWORD dwStartTimer );
	void SetTournamentDeleteTeam( DWORD dwDeleteTeam, DWORD dwStartTimer );
	void SetTournamentNewTeam( DWORD dwNewTeam );

public:
	bool ApplyBattleRoomInfo( SP2Packet &rkPacket, bool &rBattleRoom );
	ioHashString ApplyJoinUserInfo( SP2Packet &rkPacket );
	ioHashString ApplyLeaveUserInfo( SP2Packet &rkPacket, ModeType eModeType );
	void ApplyBattleRoomRecord( SP2Packet &rkPacket );
	void ApplyBattleRoomOption( SP2Packet &rkPacket, bool bAlarm );
	void ApplyBattleRoomEventType( SP2Packet &rkPacket );
	void ApplyBattleRoomAILevel( int iLevel );

public:
	void GetBattleRoomInfo( BattleRoomInfo *pBattleRoomInfo );

public:
	DWORD GetIndex(){ return m_BattleRoom.m_dwIndex; }
	int GetCurJoiner(){ return m_BattleRoom.m_iCurJoiner; }
	int GetCurPlayer();
	int GetMaxPlayer(){ return m_BattleRoom.m_iMaxPlayerBlue + m_BattleRoom.m_iMaxPlayerRed; }
	int GetMaxPlayerBlue(){ return m_BattleRoom.m_iMaxPlayerBlue; }
	int GetMaxPlayerRed(){ return m_BattleRoom.m_iMaxPlayerRed; }
	int GetMaxObserver() { return m_BattleRoom.m_iMaxObserver; }

	int GetAverageLevel();
	bool IsRandomTeamMode(){ return m_BattleRoom.m_bRandomTeamMode; }
	bool IsStartRoomEnterX(){ return m_BattleRoom.m_bStartRoomEnterX; }
	bool IsAutoModeStart(){ return m_BattleRoom.m_bAutoModeStart; }
	bool IsBadPingKick(){ return m_BattleRoom.m_bBadPingKick; }	
	bool IsUseExtraOption() { return m_BattleRoom.m_bUseExtraOption; }
	bool IsNoChallenger() { return m_BattleRoom.m_bNoChallenger; }

	int GetTeamAttackType();
	int GetChangeCharType();
	int GetCoolTimeType();
	int GetRedHPType();
	int GetBlueHPType();
	int GetDropDamageType();
	int GetGravityType();
	int GetGetUpType();
	int GetKOType();
	int GetKOEffectType();
	int GetRedBlowType();
	int GetBlueBlowType();
	int GetRedMoveSpeedType();
	int GetBlueMoveSpeedType();
	int GetRedEquipType();
	int GetBlueEquipType();

	int GetRoundType();
	int GetRoundTimeType();

	int GetGrowthUseType();
	int GetExtraItemUseType();

	int GetPreSetModeType();

	int  GetBattleEventType(){ return m_BattleRoom.m_iBattleEventType; }
	bool IsBroadcastModeType();
	bool IsTournamentModeType();
	bool IsTeamEmoticon();
	bool IsAllUserSafetyLevel();
	bool IsAllUserHighGradeLevel( int iGradeLevel );

	ioHashString GetName(){ return m_BattleRoom.m_szName; }
	ioHashString GetOwnerName(){ return m_BattleRoom.m_szOwnerName; }
	ioHashString GetPW(){ return m_BattleRoom.m_szPassword; }

	void SetName( ioHashString szName ){ m_BattleRoom.m_szName = szName; }
	void SetPW( ioHashString szPW ){ m_BattleRoom.m_szPassword = szPW; }
	void SetCurJoiner( int iCount ){ m_BattleRoom.m_iCurJoiner = iCount; }
	void SetCurPlayer( int iCount ){ m_BattleRoom.m_iCurPlayer = iCount; }
	void SetMaxPlayer( int iBlueCount, int iRedCount, int iObserver );
	void SetPlayerCountChangeAlarm( bool bCountOver );
	void SetAverageLevel( int iAverageLevel ){ m_BattleRoom.m_iAverageLevel = iAverageLevel; }
	void SetBattleEventType( int iBattleEventType ){ m_BattleRoom.m_iBattleEventType = iBattleEventType; }

	void SetRandomTeamMode( bool bRandom );
	void SetStartRoomEnterX( bool bEnterX );
	void SetAutoModeStart( bool bAuto );
	void SetBadPingKick( bool bBadPingKick );
	
	void SetRoomOptionDefault( bool bAlarm );
	void SetFirstPreSetMode();
	void SetUseExtraOption( bool bUseOption, bool bAlarm );
	void SetNoChallenger( bool bNoChallenger );

	void SetTeamAttackType( int iType, bool bAlarm );
	void SetChangeCharType( int iType, bool bAlarm );
	void SetCoolTimeType( int iType, bool bAlarm );
	void SetRedHPType( int iType, bool bAlarm );
	void SetBlueHPType( int iType, bool bAlarm );
	void SetDropDamageType( int iType, bool bAlarm );
	void SetGravityType( int iType, bool bAlarm );
	void SetGetUpType( int iType, bool bAlarm );
	void SetKOType( int iType, bool bAlarm );
	void SetKOEffectType( int iType, bool bAlarm );
	void SetRedBlowType( int iType, bool bAlarm );
	void SetBlueBlowType( int iType, bool bAlarm );
	void SetRedMoveSpeedType( int iType, bool bAlarm );
	void SetBlueMoveSpeedType( int iType, bool bAlarm );
	void SetRedEquipType( int iType, bool bAlarm );
	void SetBlueEquipType( int iType, bool bAlarm );

	void SetRoundType( int iType, bool bAlarm );
	void SetRoundTimeType( int iType, bool bAlarm );

	void SetGrowthUseType( int iType, bool bAlarm );
	void SetExtraItemUseType( int iType, bool bAlarm );

	bool SetPreSetModeType( int iType, bool bUpdate );
	bool SetPreSetModeTypeManual( int iType );
	
public:
	int  GetSelectModeIndex(){ return m_iSelectModeIndex; }
	int  GetSelectMapIndex(){ return m_iSelectMapIndex; }	
	bool IsSafetyLevelRoom(){ return m_bSafetyLevelRoom; }
	void SetSelectModeNMap( int iSelectMode, int iSelectMap, bool bSafetyLevelRoom );

public:
	bool IsTeamNoneMonsterCoinCheck( int iUseCoinCnt );
	void GetNeedMonsterCoinUserIDVec( IN int iUseCoinCnt, OUT ioHashStringVec &rvUserID );
	bool IsTeamNoneRaidCoinCheck( int iUseCoinCnt );
	void GetNeedRaidCoinUserIDVec( IN int iUseCoinCnt, OUT ioHashStringVec &rvUserID );
	void SyncMonsterCoin();

public:
	int GetUserMonsterCoin( const ioHashString &rkUserID );
	int GetUserGoldMonsterCoin( const ioHashString &rkUserID );
	int GetUserRaidCoin( const ioHashString &rkUserID );
	DWORD GetUserCharIndex( const ioHashString &rkUserID );
	DWORD GetUserCharMotion( const ioHashString &rkUserID );

public:
	void PlayStartSound( int iCurSec );

public:
	const ioHashString &GetTeamEmoticon( const ioHashString &rkName );

public:
	void FillBattleRoomInfo( SP2Packet &rkPacket );

	bool IsObserverUser( const ioHashString& szUserID );

	DWORD GetAutoStartAlarmCheckTime(){ return m_dwAutoStartAlarmCheckTime; }

public:
	void ProcessBattleRoom();

public:
	void SendBattleRoomP2P( SP2Packet &rkPacket );
	void SendBattleRoomAnn( const char *szChat );

	void SendBattleRoomOption();

public:
	int  GetModeStartPlayerCount(){ return m_iModeStartPlayerCount; }
	void ModeStartPlayerCount();

public:
	void StartChatBubble( const char *szID, const char *szChat, bool bWholeChat );

public:
	DWORD GetTournamentIndex(){ return m_BattleRoom.m_dwTourIndex; }
	DWORD GetTournamentBlueIndex(){ return m_BattleRoom.m_dwBlueTeamIndex; }
	DWORD GetTournamentRedIndex(){ return m_BattleRoom.m_dwRedTeamIndex; }
	DWORD GetTournamentStartTime(){ return m_BattleRoom.m_dwTourStartTimer; }
	bool  IsTournamentStartSender(){ return m_BattleRoom.m_bTourStartSender; }
	void  SetTournamentStartSender( bool bSender ){ m_BattleRoom.m_bTourStartSender = bSender; }

public:
	bool  IsBattleRoomMovePenaltyPeso();
	//ai
	void SetAiLevel( int iLevel );
	int	 GetAiLevel();	
	std::vector<int> m_vecAIMaxSlot;
	int	GetAIMaxSlot( int iMapIndex );

	//������ ���� ����
	bool ApplyLadderRoomInfo( SP2Packet &rkPacket, bool &rBattleRoom );
public:
	static ioBattleRoomMgr& GetSingleton();

public:
	ioBattleRoomMgr();
	virtual ~ioBattleRoomMgr();
};

#define g_BattleRoomMgr ioBattleRoomMgr::GetSingleton()

