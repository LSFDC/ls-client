#pragma once



class ioUIRenderImage;

enum CenterKillType
{
	CKT_NONE,
	CKT_OWNER_KILL,			//�ڽ��� ų�� �Ѱ��
	CKT_OWNER_DIE,			//�ڽ��� ���� ���
	CKT_TEAM_CHAR_KILL,		//���� ������ ���� ���
	CKT_TEAM_CHAR_DIE,		//���� �������� �׾��� ���
	CKT_ACCUMULATE_KILL,	//����ų ( ����,Ʈ���� ... )
};

class ioModeUIController  : public Singleton< ioModeUIController >
{
protected:
	ioWnd *m_pCenterInfoWnd;
	ioWnd *m_pKOMsgWnd;
	ioWnd *m_pCenterKillInfoWnd;

public:
	void LoadINI();

public:
	static ioModeUIController& GetSingleton();

public:
	void ProcessController();

public:
	//
	void SetDeathTime();
	void SetTeamDeathKO( TeamType eKill, const ioHashString &szKiller, TeamType eDie, const ioHashString &szDier,
						 float fBlueRate, float fRedRate );
	void SetTeamDeathWarning( float fBlueRate, float fRedRate );
	void SetDeathMatch( int iCurrentRank );
	void SetDeathMatchWarning( int iCurrentRank );
	void SetGangsiLiveUser( int iLiveUser, const ioHashString &szGangsi, const ioHashString &szSurvivor );
	void SetGangsiAlarm( bool bGangsiAlarmText, DWORD dwCurrentTime, DWORD dwWarningTime );
	void SetKingGauge( bool bKing, TeamType eKingTeam, TeamType eOwnerTeam, DWORD dwGivePointTime, DWORD dwCrownHoldTime, float fScoreWarningRate );

	void SetBossRaidMode( bool bShow, int iBossRaidOrder, int iBossRaidMaxOrder, int iMyBossLevel );

	void SetSymbolGaugeRate( bool bShow, float fBlueRate, float fRedRate );
	void SetSymbolAttack( TeamType eTeam );

	void SetMonsterDiceResult( const ioHashString &kPrizeWinner, int iPresentType, int iPresentValue1, int iPresentValue2 );

	void SetDoubleCrownGaugeRate( bool bShow, float fBlueRate, float fRedRate, bool bBlueKing, bool bRedKIng );
	void SetDoubleCrownAttack( TeamType eTeam );
	void SetDoubleCrownWinningTeam( TeamType eTeam );

	// 
	void SetKOMsg( TeamType eKillerTeam, const ioHashString &szKillerName, const ioHashString &szKillerSkillName, DWORD dwKillerItem,
				   TeamType eDierTeam, const ioHashString &szDierName );
	void SetPrisonerMsg( TeamType eKillerTeam, const ioHashString &szKillerName, const ioHashString &szKillerSkillName, DWORD dwKillerItem,
				         TeamType eDierTeam, const ioHashString &szDierName );
	void SetEscapeMsg( TeamType eKillerTeam, const ioHashString &szKillerName, const ioHashString &szKillerSkillName, DWORD dwKillerItem,
				       TeamType eDierTeam, const ioHashString &szDierName );
	void SetInfectionMsg( TeamType eKillerTeam, const ioHashString &szKillerName, const ioHashString &szKillerSkillName, DWORD dwKillerItem,
						  TeamType eDierTeam, const ioHashString &szDierName );

	void SetCenterKillUI( const ModeType& CurModeType, const int& iKillCount, const ioBaseChar* const pOwner, TeamType eKillerTeam, const ioHashString &szKillerName, 
						  DWORD dwKillerItem, TeamType eDierTeam, const ioHashString &szDierName, bool bKillerMale );

public:
	void SetCenterInfoWnd( ioWnd *pCenterInfoWnd );
	void SetKOMsgWnd( ioWnd *pKOMsgWnd );
	void SetCenterKillInfoWnd( ioWnd *pCenterKillInfoWnd );

public:
	bool IsHiddenKingWarning();
	bool IsCenterKillInfoWnd() const;

public:
	ioModeUIController();
	virtual ~ioModeUIController();
};

#define g_ModeUIController ioModeUIController::GetSingleton()

