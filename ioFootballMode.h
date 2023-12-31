#pragma once


#include "../io3DEngine/ioOrientBox.h"
#include "ioPlayMode.h"

class SP2Packet;

class ioPlayEntity;
class ioBaseChar;
class ioPushStruct;

struct GoalGate
{
	TeamType m_Team;
	ioOrientBox m_GateBox;
	D3DXVECTOR3 m_vGateDir;

	GoalGate()
	{
		m_Team = TEAM_NONE;
	}
};

class ioFootballMode : public ioPlayMode
{
protected:
	typedef std::vector< FootballRecord > RecordList;
	RecordList m_vCharRecordList;

	typedef std::vector< int > RevivalList;
	RevivalList m_RedRevivalPosList;
	RevivalList m_BlueRevivalPosList;

	GoalGate m_GoalGateRed;
	GoalGate m_GoalGateBlue;

	ioHashString m_GoalPlayer;

	CEncrypt<bool> m_bNoDamage;
	CEncrypt<bool> m_bDisableAttack;

	DWORD m_dwBallSurroundTime;

public:
	virtual void InitMode( int iMapIndex );
	virtual void ProcessMode();
	virtual void RenderPreGUI();
	virtual void RenderAfterGUI();

	virtual void ReStartMode();
	virtual void ClearMode();

public:
	virtual bool ProcessTCPPacket( SP2Packet &rkPacket );
	virtual bool ProcessUDPPacket( SP2Packet &rkPacket );

	virtual void ApplyModeInfo( SP2Packet &rkPacket );
	virtual void ApplyCharModeInfo( const ioHashString &rkName,
									SP2Packet &rkPacket, bool bDieCheck = false );

	virtual void GetModeSpecialInfo( SP2Packet &rkPacket );

public:
	virtual void NotifyEntityDie( ioPlayEntity *pEntity, ioPlayEntity *pKiller, ioBaseChar *pBestAttacker,
								  float fLastRate, float fBestRate );
	virtual void NotifyCharDropDie( ioBaseChar *pDieChar, ioBaseChar *pKillChar, ioBaseChar *pBestAttacker,
									float fLastRate, float fBestRate );

public:
	virtual D3DXVECTOR3 GetRevivalPos( ioBaseChar *pChar );
	virtual int GetRoundWinCnt( TeamType eTeamType );

	virtual void SetZeroHP( bool bZero );

protected:
	virtual void LoadMap();
	virtual void LoadModeResource( ioINILoader &rkLoader );

protected:
	void ProcessReadyState();
	void ProcessPlayState();
	void ProcessResultState();

protected:
	void ProcessGUI();

	void ProcessRevival();
	void CheckRoundEnd();

	bool IsInBallRange( ioBaseChar *pChar, float fRange );
	void CheckBallSurround();

protected:
	virtual void UpdateMiniMapSpotPoint();
	virtual void UpdateRoundStateWnd();
	virtual void SetResultScoreWnd();
	virtual void SetTotalScoreWnd( IntVec &rvClassTypeList,
								   IntVec &rvClassPointList,
								   float fBlueGuildTeamBonus,
								   float fRedGuildTeamBonus,
								   float fBonusArray[BA_MAX] );

	void UpdateTimeScoreWnd();

protected:
	void OnNotifyCharDie( ioBaseChar *pDieChar, ioBaseChar *pKillChar, ioBaseChar *pBestAttacker,
						  float fLastRate, float fBestRate );

public:
	virtual void AddNewRecord( ioBaseChar *pChar );
	virtual void RemoveRecord( const ioHashString &rkName );
	virtual void RemoveRecord( int nIndex );

	virtual ioBaseChar* GetRecordChar( int i );
	virtual ioBaseChar* GetRecordChar( const ioHashString &rkName );
	virtual int GetRecordCharCnt() const;

protected:
	virtual ModeRecord* FindModeRecord( int iIdx );
	virtual ModeRecord* FindModeRecordCharName( const ioHashString &rkName );

	FootballRecord* FindRecordCharName( const ioHashString &rkName );

	virtual bool GetRoundInfo( int iIndex, RoundInfo &rkInfo );
	virtual bool GetResultInfo( int iIndex, ResultInfo &rkInfo );
	virtual bool GetFinalInfo( int iIndex, FinalInfo &rkInfo );

// Guide
protected:
	virtual ioGameEntity* GetGuideTarget( const D3DXVECTOR3& vArrowPos );
	//EndSceneCamera
	virtual void CheckEndSceneLookAt();
	virtual void ClearModeData();

public:    //기여도.
	void ContributeConvertTeam( TeamType eTeam );
	virtual void ContributeConvert();
	virtual int ContributeMyRank( int &rRankValue1, int &rRankValue2, int &rRankValue3, int &rRankValue4 );
	virtual void ContributeRoundPercent( DWORD dwCurTime, DWORD dwTotalTime );         //팀별 비율
	virtual void ContributeRoundAllPercent( DWORD dwCurTime = 0, DWORD dwTotalTime = 0 );      //전체 유저 비율
	virtual void ContributeLOG();

	virtual void DamageContribute( const ioHashString szAttacker, const ioHashString szTarget, float fDamage, DWORD dwItemCode, int iWeaponType = -1 );
	virtual void KillContribute( const ioHashString szKiller, const ioHashString szKilled, DWORD dwItemCode );
	virtual void AttackCntContribute( const ioHashString szAttacker, const ioHashString szTarget, DWORD dwItemCode, int iAttackValue = FLOAT100 );

	void BallTouchContribute( const ioHashString szAttacker );
	void GoalContribute( TeamType eGoalTeam, const ioHashString szAttacker );
	void AssistContribute( TeamType eGoalTeam, const ioHashString szAttacker );
	void SaveTouchContribute( const ioHashString szAttacker );

	virtual int GetContributePerChar( const ioHashString& szName );
	virtual void ContributeResultSync( const ioHashString &szName, int iContribute );

	virtual bool FillAllDamagedList( SP2Packet &rkPacket );

	virtual void FillLastPlayRecordInfo( SP2Packet &rkPacket );
	virtual void FillPlayRecordInfo( SP2Packet &rkPacket );
	virtual void ApplyPlayRecordInfo( SP2Packet &rkPacket );
	// For Rank
	virtual int GetCurRank( const ioHashString& szCharName );

protected:
	virtual void OnRoundJoin( SP2Packet &rkPacket );
	virtual void OnRoundJoinView( SP2Packet &rkPacket );
	virtual void OnRoundJoinObserver( SP2Packet &rkPacket );
	virtual void OnRoundReady( SP2Packet &rkPacket );
	virtual void OnZeroHP( SP2Packet &rkPacket );

public:
	virtual void OnModeSpecialInfo( SP2Packet &rkPacket );

protected:
	void OnGoal( SP2Packet &rkPacket );

public:
	void GetGoalGate( IN TeamType eTeam, OUT GoalGate &rkGate );
	inline bool IsNoDamageMode() const { return m_bNoDamage; }
	inline bool IsDisableAttackMode() const { return m_bDisableAttack; }

public:
	ioFootballMode( ioPlayStage *pCreator );
	virtual ~ioFootballMode();
};

inline ioFootballMode* ToFootballMode( ioPlayMode *pMode )
{
	if( !pMode || pMode->GetModeType() != MT_FOOTBALL )
		return NULL;

	return dynamic_cast< ioFootballMode* >( pMode );
}

