#pragma once

#include "CharacterInfo.h"
/************************************************************************/
/* ����Ʈ�� ������ ó�� �߰��Ѵ�.                                       */
/************************************************************************/
class ioPlayStage;
class ioUIRenderImage;
class QuestData;
class SP2Packet;
class QuestParent 
{
public:
	enum
	{
		BATTLE_STYLE				= 1,	//�Ϲ�����������
		LADDER_STYLE				= 2,	//��������������
		ALL_BATTLE_STYLE			= 3,	//�������(�Ϲ�/������/����/��ȸ/�����Ǹ��)
		SHUFFLE_STYLE				= 4,	//�����Ǹ��
	};

protected:
	bool  m_bDirectSync;                 // ������ �Ͼ�� �ٷ� ������Ʈ.
	DWORD m_dwDirectSyncSafeTime;        // ��� ������ ���ص� �ּ� 10�п� �ѹ��� �ϵ����ؾ��� ��Ʈ�� ������ ���� �ս��� �߻������ʴ´�.

protected:
	DWORD m_dwMainIndex;                 // ����Ʈ ����
	
protected:
	DWORD m_dwSubIndex;                  // ����Ʈ ������ Ÿ��
	DWORD m_dwPerformType;               // ����Ʈ ���� Ÿ��
	
	bool  m_bOneDayStyle;                // ���� ����Ʈ ��� - �Ϸ� ��Ͽ� ���ԵǸ� ���� �Ϸ� ��Ͽ��� ������.
	bool  m_bRepeatStyle;                // �ݺ������� ���� ������ ����Ʈ - �Ϸ� ��Ͽ� �������� �ʴ´�.
	bool  m_bGuildStyle;                 // ��� ���� ����Ʈ - ��� �����ؾ� ���� �� �ְ� ��尡 ������ �������� ����Ʈ�� ������
	int   m_iGuildTermsLevel;            // ��� ���� ����Ʈ���� ��ũ ����
	bool  m_bPCRoomStyle;                // �Ǿ��� ���� ����Ʈ

	ChannelingType m_eChannelingType;    //ä�θ� : �Ϲ�, mgame, ��

	DWORD m_dwOccurValue;                // �߻� ����
	int   m_iOccurValueEx;               // �߻� ���ǿ� ���� �ٸ����� ���� - �Լ� ���ڿ��� value1�� ����.
	int   m_iOccurModeType;              // �߻��� �ش� ��� üũ
	int   m_iOccurRoomStyle;             // �߻��� �� ��Ÿ��(����or����) üũ
	int   m_iOccurExceptingMode;         // �߻��� ���� ���
	int   m_iOccurLimitGradeMin;		 // �߻��� �ּ� ��� 
	int   m_iOccurLimitGradeMax;         // �߻��� �ִ� ���

	DWORD m_dwCompleteValue;             // �Ϸ� ����
	int   m_iCompleteModeType;           // �Ϸ�� �ش� ��� üũ
	int   m_iCompleteRoomStyle;          // �Ϸ�� �� ��Ÿ��(����or����) üũ
	int   m_iCompleteExceptingMode;      // �Ϸ�� ���� ���

	DWORD m_dwPeriodHour;                // �ð� ���� ����Ʈ

	DWORDVec m_vRewardPresent;           // �Ϸ� ����

	bool  m_bRewardSelectStyle;			 // �Ϸ� ���� ���� ����Ʈ ����
	int	  m_iRewardSelectNum;			 // �Ϸ� ���� ���� ����	

	DWORD m_dwPrevMainIndex;             // �߻��� �Ϸ�Ǿ��־�� �� ����Ʈ MainIndex
	DWORD m_dwPrevSubIndex;              // �߻��� �Ϸ�Ǿ��־�� �� SubIndex

	DWORD m_dwNextMainIndex;             // �Ϸ�� ���� ����Ʈ MainIndex
	DWORD m_dwNextSubIndex;              // �Ϸ�� ���� ����Ʈ SubIndex

	bool  m_bInvitedPass;                // ����Ʈ �������̸� ��� �ʴ� �н�
	bool  m_bUserModePass;               // �Ϸ� ���� üũ�� �������� �н�
	bool  m_bDeathTimePass;              // �Ϸ� ���� üũ�� ����Ÿ���̸� �н�

	bool  m_bPlayOccurShow;              // �÷����߿� ���� UI ǥ��
	bool  m_bPlayCompleteShow;           // �÷����߿� �޼� UI ǥ��

	bool  m_bRoundEndOccurShow;          // ���� ������� ����ġ ��±��� ����Ʈ �߻� UIǥ��
	bool  m_bRoundEndCompleteShow;       // ���� ������� ����ġ ��±��� ����Ʈ �Ϸ� UIǥ��

	bool  m_bCompleteGameAlarm;          // �Ϸ�(����ȹ��)�� ���ӳ� �����鿡�� �˸�
	bool  m_bCompleteWebAlarm;           // �Ϸ�(����ȹ��)�� ���� �˸�

protected:
	bool  m_bAlive;                      // �̺�Ʈ ����Ʈ ���࿩�δ� ������ �Ǵ�

protected:    // UI��
	DWORD m_dwHelpIndex;
	ioUIRenderImage *m_pIcon;
	ioHashString m_szTitle;
	ioHashString m_szProgressText;
	ioHashString m_szProgressResult;
	ioHashString m_szOutProgressResult;	
	ioHashString m_szBackUpTooltipResult;

protected:
	bool m_bIsCostume;

protected:
	enum
	{
		MAX_CUSTOM_VALUE = 50,
	};
	IntVec m_CustomValue;                // ����Ʈ���� ����� ���.

protected:
	void ShowTooltip();

public:
	inline DWORD GetMainIndex(){ return m_dwMainIndex; }
	inline DWORD GetSubIndex(){ return m_dwSubIndex; }
	inline DWORD GetPerformType(){ return m_dwPerformType; }
	inline bool  IsOneDayStyle(){ return m_bOneDayStyle; }
	inline bool  IsRepeatStyle(){ return m_bRepeatStyle; }
	inline bool  IsGuildStyle(){ return m_bGuildStyle; }
	inline int   GetGuildTermsLevel(){ return m_iGuildTermsLevel; }
	inline bool  IsPCRoomStyle(){ return m_bPCRoomStyle; }
	inline ChannelingType GetChannelingType(){ return m_eChannelingType; }

	inline DWORD GetOccurValue(){ return m_dwOccurValue; }
	inline int   GetOccurValueEx(){ return m_iOccurValueEx; }

	inline DWORD GetCompleteValue(){ return m_dwCompleteValue; }

	inline DWORD GetPeriodHour(){ return m_dwPeriodHour; }

	inline int GetMaxRewardPresent(){ return m_vRewardPresent.size(); }
	DWORD GetRewardPresentIndex( int iArray );

	inline bool	 IsRewardSelectStyle() const { return m_bRewardSelectStyle; }
	inline int   GetRewardSelectNum() const { return m_iRewardSelectNum; }

	inline DWORD GetPrevMainIndex(){ return m_dwPrevMainIndex; }
	inline DWORD GetPrevSubIndex(){ return m_dwPrevSubIndex; }
	inline DWORD GetNextMainIndex(){ return m_dwNextMainIndex; }
	inline DWORD GetNextSubIndex(){ return m_dwNextSubIndex; }

public:
	inline const DWORD &GetHelpIndex(){ return m_dwHelpIndex; }
	inline const ioHashString &GetTitle(){ return m_szTitle; }
	inline const ioHashString &GetProgressText(){ return m_szProgressText; }
	virtual ioHashString &GetProgressResult();
	virtual ioHashString &GetProgressResultOnlyNumber();
	virtual ioHashString &GetProgressResultOnlyText();
	virtual int GetProgressCount(){ return 1; }
	virtual int GetProgressPercentage( int iValue1, int iValue2 );
	virtual float GetProgressGaugeRate();
	
public:
	int GetCustomValue( int iIdx );
	ioUIRenderImage *GetQuestIcon();

public:
	int  GetRenderImageStartHeight( ioUIRenderImage *pImage, int iSrcHeight, float fGapRate );
	void RenderQuestIconGauge( int iXPos, int iYPos, float fScale );
	void RenderQuestIcon( int iXPos, int iYPos, float fScale );

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckAlreadyAttain(){ return false; }

public:
	virtual bool SetQuestData( DWORD dwMainIndex, DWORD dwSubIndex, ioINILoader &rkLoader );

public:
	virtual QuestData &GetMyInfoQuestData();
	virtual bool IsMeProgressQuest();
	virtual bool IsQuestAlarm();
	virtual bool IsQuestGuideShow();

public:
	virtual void SendAttainData( QuestData &rkData );
	virtual bool AddAttainProcess( int iAddValue = 0, int iCheckValue = 0 );
	virtual bool SetAttainProcess( int iSetValue = 0, int iCheckValue = 0 );
	virtual bool IsAbuseCheckQuest(){ return false; }

public:
	virtual void ChangeStageInit();
	virtual void Process( ioPlayStage *pPlayStage );
	virtual void ProcessRewardComplete();          // ���� ���� ���� ó�� ���� ������ ó��
	virtual void StartProcessTime(){};

public:
	void SendErrorLog( const ioHashString& szErrorLog );

public:
	void SetAlive( bool bAlive );
	bool IsAlive(){ return m_bAlive; }

public:
	inline bool IsInvitedPass(){ return m_bInvitedPass; }
	inline bool IsPlayOccurShow(){ return m_bPlayOccurShow; }
	inline bool IsPlayCompleteShow(){ return m_bPlayCompleteShow; }
	inline bool IsRoundEndOccurShow(){ return m_bRoundEndOccurShow; }
	inline bool IsRoundEndCompleteShow(){ return m_bRoundEndCompleteShow; }
	inline bool IsCompleteGameAlarm(){ return m_bCompleteGameAlarm; } 
	inline bool IsCompleteWebAlarm(){ return m_bCompleteWebAlarm; }	

public:  // �׽�Ʈ�� ���� ��ũ��
	void SendMacroAttain( QuestData &rkData );

public:
	bool IsCostume() { return m_bIsCostume; }

public:   
	QuestParent();
	virtual ~QuestParent();
};
//////////////////////////////////////////////////////////////////////////
class QuestBasic : public QuestParent
{
public:
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	QuestBasic(){}
	virtual ~QuestBasic(){}
};
//////////////////////////////////////////////////////////////////////////
class QuestMonsterModeClear : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		CLEAR_ROUND,
		CLEAR_COUNT,
		PLAYER_COUNT,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();

public:
	QuestMonsterModeClear();
	virtual ~QuestMonsterModeClear();
};
//////////////////////////////////////////////////////////////////////////
class QuestEnterBattlePvPMode : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	QuestEnterBattlePvPMode();
	virtual ~QuestEnterBattlePvPMode();
};
//////////////////////////////////////////////////////////////////////////
class QuestEnterBattlePvPModeKO : public QuestParent
{
public:
	enum
	{
		KILL_COUNT = 0,
		BEST_ATTACKER_PASS,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();
	virtual ioHashString &GetProgressResult();
	virtual ioHashString &GetProgressResultOnlyNumber();
	virtual ioHashString &GetProgressResultOnlyText();
	virtual float GetProgressGaugeRate();

public:
	QuestEnterBattlePvPModeKO();
	virtual ~QuestEnterBattlePvPModeKO();
};
//////////////////////////////////////////////////////////////////////////
class QuestEnterBattlePvPModeWin : public QuestParent
{
public:
	enum
	{
		WIN_COUNT = 0,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();
	virtual ioHashString &GetProgressResult();
	virtual ioHashString &GetProgressResultOnlyNumber();
	virtual ioHashString &GetProgressResultOnlyText();
	virtual float GetProgressGaugeRate();

public:
	QuestEnterBattlePvPModeWin();
	virtual ~QuestEnterBattlePvPModeWin();
};
//////////////////////////////////////////////////////////////////////////
class QuestPvEMonsterKill : public QuestParent
{
public:
	enum
	{
		KILL_COUNT = 0,
		LIMIT_MIN_FLOOR,
		LIMIT_MAX_FLOOR,
		KILL_SOLDIER_CODE,
		LIMIT_GRADE,
		MAX_MONSTER_CODE,
		MONSTER_CODE,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();

public:
	QuestPvEMonsterKill();
	virtual ~QuestPvEMonsterKill();
};
//////////////////////////////////////////////////////////////////////////
class QuestGradeUP : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		COMPLETE_GRADE,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckAlreadyAttain();

public:
	virtual int GetProgressCount();
	virtual ioHashString &GetProgressResult();
	virtual ioHashString &GetProgressResultOnlyNumber();
	virtual ioHashString &GetProgressResultOnlyText();
	virtual float GetProgressGaugeRate();

public:
	QuestGradeUP();
	virtual ~QuestGradeUP();
};
//////////////////////////////////////////////////////////////////////////
class QuestTimeGrowthTry : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	QuestTimeGrowthTry();
	virtual ~QuestTimeGrowthTry();
};
//////////////////////////////////////////////////////////////////////////
class QuestTimeGrowthSuccess : public QuestParent
{
public:
	enum
	{
		GROWTH_UP_LEVEL = 0,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	QuestTimeGrowthSuccess();
	virtual ~QuestTimeGrowthSuccess();
};
//////////////////////////////////////////////////////////////////////////
class QuestPesoGrowthTry : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	QuestPesoGrowthTry();
	virtual ~QuestPesoGrowthTry();
};
//////////////////////////////////////////////////////////////////////////
class QuestFishingTry : public QuestParent
{
public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	QuestFishingTry();
	virtual ~QuestFishingTry();
};
//////////////////////////////////////////////////////////////////////////
class QuestFishingSuccess : public QuestParent
{
public:
	enum
	{
		FISHING_SUCCESS_COUNT = 0,
		LIMIT_FISHING_LEVEL,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();

public:
	QuestFishingSuccess();
	virtual ~QuestFishingSuccess();
};
//////////////////////////////////////////////////////////////////////////
class QuestFishingFailed : public QuestParent
{
public:
	enum
	{
		FISHING_FAIELD_COUNT = 0,
		LIMIT_FISHING_LEVEL,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();

public:
	QuestFishingFailed();
	virtual ~QuestFishingFailed();
};
//////////////////////////////////////////////////////////////////////////
class QuestFishingLevelUP : public QuestParent
{
public:
	enum
	{
		LIMIT_FISHING_LEVEL = 0,
		CHECK_FISHING_LEVEL,
		MINUS_FISHING_LEVEL,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckAlreadyAttain();

public:
	virtual int GetProgressCount();
	virtual ioHashString &GetProgressResult();
	virtual ioHashString &GetProgressResultOnlyNumber();
	virtual ioHashString &GetProgressResultOnlyText();
	virtual float GetProgressGaugeRate();

public:
	QuestFishingLevelUP();
	virtual ~QuestFishingLevelUP();
};
//////////////////////////////////////////////////////////////////////////
class QuestFishingSellPeso : public QuestParent
{
public:
	enum
	{
		LIMIT_FISHING_LEVEL = 0,
		SELL_FISH_LIMIT_PESO,
		COMPLETE_COUNT,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();

public:
	QuestFishingSellPeso();
	virtual ~QuestFishingSellPeso();
};
//////////////////////////////////////////////////////////////////////////
class QuestFishingSuccessItem : public QuestParent
{
public:
	enum
	{
		LIMIT_FISHING_LEVEL = 0,
		FISHING_ITEM_TYPE,
		COMPLETE_COUNT,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();

public:
	QuestFishingSuccessItem();
	virtual ~QuestFishingSuccessItem();
};
//////////////////////////////////////////////////////////////////////////
class QuestBattlePvPModeAwardAcquire : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		AWARD_COUNT,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	QuestBattlePvPModeAwardAcquire();
	virtual ~QuestBattlePvPModeAwardAcquire();
};
//////////////////////////////////////////////////////////////////////////
class QuestSoldierPractice : public QuestParent
{
public:
	enum
	{
		SOLDIER_CODE = 0,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	QuestSoldierPractice();
	virtual ~QuestSoldierPractice();
};
//////////////////////////////////////////////////////////////////////////
class QuestExtraItemReinforceSuccess : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		EXTRAITEM_REINFORCE_SUCCESS,
		MAX_EXTRA_ITEM_CODE,
		EXTRA_ITEM_CODE,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();

public:
	QuestExtraItemReinforceSuccess();
	virtual ~QuestExtraItemReinforceSuccess();
};
//////////////////////////////////////////////////////////////////////////
class QuestSoldierLevelUP : public QuestParent
{
public:
	enum
	{
		SOLDIER_CODE = 0,
		SOLDIER_LEVEL,
		SOLDIER_LIMIT_LEVEL,
		SOLDIER_MINUS_LEVEL,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckAlreadyAttain();

public:
	virtual int GetProgressCount();
	virtual ioHashString &GetProgressResult();
	virtual ioHashString &GetProgressResultOnlyNumber();
	virtual ioHashString &GetProgressResultOnlyText();
	virtual float GetProgressGaugeRate();

public:
	QuestSoldierLevelUP();
	virtual ~QuestSoldierLevelUP();
};
//////////////////////////////////////////////////////////////////////////
class QuestOpenManual : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		CHECK_MANUAL_ID,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	QuestOpenManual();
	virtual ~QuestOpenManual();
};
//////////////////////////////////////////////////////////////////////////
class QuestLoginCount : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		LOGIN_COUNT,     // �α��� Ƚ��
		LIMIT_LOW_DAY,   // �α��� ī��Ʈ ���� �ּ� ����
		LIMIT_HIGH_DAY,  // �α��� ī��Ʈ �ʱ�ȭ �ִ밪.
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();

protected:
	int  GetCountLaterDay();

public:
	virtual bool AddAttainProcess( int iAddValue = 0, int iCheckValue = 0 );
	virtual bool SetAttainProcess( int iSetValue = 0, int iCheckValue = 0 );

public:
	QuestLoginCount();
	virtual ~QuestLoginCount();
};
//////////////////////////////////////////////////////////////////////////
class QuestEnterPlaza : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	QuestEnterPlaza();
	virtual ~QuestEnterPlaza();
};
//////////////////////////////////////////////////////////////////////////
class QuestGetPotion : public QuestParent
{
public:
	enum
	{
		CHECK_ROUND = 0,
		CHECK_COUNT,
		LIMIT_MIN_FLOOR,
		LIMIT_MAX_FLOOR,
	};

	enum
	{
		POTION_CODE1 = 401000,        //HP ����
		POTION_CODE2 = 401001,		  //��� HP ����
		POTION_CODE3 = 401002,		  //��ų ����
		POTION_CODE4 = 401003,		  //��� ��ų ����
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();

public:
	QuestGetPotion();
	virtual ~QuestGetPotion();
};
//////////////////////////////////////////////////////////////////////////
class QuestTutorial : public QuestParent
{
public:
	enum
	{
#if defined (SRC_EU)
		TUTORIAL_STATE = 100,
#elif defined (SRC_SA)
		TUTORIAL_STATE = 100,
#else 
		TUTORIAL_STATE = 1200,
#endif
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual void Process( ioPlayStage *pPlayStage );

public:
	QuestTutorial();
	virtual ~QuestTutorial();
};
//////////////////////////////////////////////////////////////////////////
class QuestPresentReceive : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		RECV_COUNT,  
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();

public:
	QuestPresentReceive();
	virtual ~QuestPresentReceive();
};
//////////////////////////////////////////////////////////////////////////
class QuestCampJoin : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckAlreadyAttain();

public:
	QuestCampJoin();
	virtual ~QuestCampJoin();
};
//////////////////////////////////////////////////////////////////////////
class QuestEnterCampBattle : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	QuestEnterCampBattle();
	virtual ~QuestEnterCampBattle();
};
//////////////////////////////////////////////////////////////////////////
class QuestCampBattleKO : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		KILL_COUNT,
		BEST_ATTACKER_PASS,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();
	virtual ioHashString &GetProgressResult();
	virtual ioHashString &GetProgressResultOnlyNumber();
	virtual ioHashString &GetProgressResultOnlyText();
	virtual float GetProgressGaugeRate();

public:
	QuestCampBattleKO();
	virtual ~QuestCampBattleKO();
};
//////////////////////////////////////////////////////////////////////////
class QuestCampBattleWin : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		WIN_COUNT,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();
	virtual ioHashString &GetProgressResult();
	virtual ioHashString &GetProgressResultOnlyNumber();
	virtual ioHashString &GetProgressResultOnlyText();
	virtual float GetProgressGaugeRate();

public:
	QuestCampBattleWin();
	virtual ~QuestCampBattleWin();
};
//////////////////////////////////////////////////////////////////////////
class QuestCampSeasonReward : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		CHECK_WIN_LOSE,
		CHECK_CAMP_POINT,
		CHECK_CAMP_RANK,
		CHECK_CAMP_BONUS,
	};

	enum
	{
		WIN_CHECK = 1,
		LOSE_CHECK= 2,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	QuestCampSeasonReward();
	virtual ~QuestCampSeasonReward();
};
//////////////////////////////////////////////////////////////////////////
class QuestAwardAcquire : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		CHECK_AWARD_TYPE,
		CHECK_COUNT,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();
	virtual ioHashString &GetProgressResult();
	virtual ioHashString &GetProgressResultOnlyNumber();
	virtual ioHashString &GetProgressResultOnlyText();
	virtual float GetProgressGaugeRate();

public:
	QuestAwardAcquire();
	virtual ~QuestAwardAcquire();
};
//////////////////////////////////////////////////////////////////////////
class QuestPrisonerDrop : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		CHECK_COUNT,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();

public:
	QuestPrisonerDrop();
	virtual ~QuestPrisonerDrop();
};
//////////////////////////////////////////////////////////////////////////
class QuestPrisonerSave : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		CHECK_COUNT,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();

public:
	QuestPrisonerSave();
	virtual ~QuestPrisonerSave();
};
//////////////////////////////////////////////////////////////////////////
class QuestBattleLevel : public QuestParent
{
public:
	enum
	{
		LIMIT_BATTLE_LEVEL = 0,
		CHECK_BATTLE_LEVEL,
		MINUS_BATTLE_LEVEL,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckAlreadyAttain();

public:
	virtual int GetProgressCount();
	virtual ioHashString &GetProgressResult();
	virtual ioHashString &GetProgressResultOnlyNumber();
	virtual ioHashString &GetProgressResultOnlyText();
	virtual float GetProgressGaugeRate();

public:
	QuestBattleLevel();
	virtual ~QuestBattleLevel();
};
//////////////////////////////////////////////////////////////////////////
class QuestAwardLevel : public QuestParent
{
public:
	enum
	{
		LIMIT_AWARD_LEVEL = 0,
		CHECK_AWARD_LEVEL,
		MINUS_AWARD_LEVEL,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckAlreadyAttain();

public:
	virtual int GetProgressCount();
	virtual ioHashString &GetProgressResult();
	virtual ioHashString &GetProgressResultOnlyNumber();
	virtual ioHashString &GetProgressResultOnlyText();
	virtual float GetProgressGaugeRate();

public:
	QuestAwardLevel();
	virtual ~QuestAwardLevel();
};
//////////////////////////////////////////////////////////////////////////
class QuestContribute : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		LIMIT_CONTRIBUTE,
		CHECK_COUNT,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();

public:
	QuestContribute();
	virtual ~QuestContribute();
};
//////////////////////////////////////////////////////////////////////////
class QuestDropKill : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		KILL_COUNT,
		BEST_ATTACKER_PASS,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();
	virtual ioHashString &GetProgressResult();
	virtual ioHashString &GetProgressResultOnlyNumber();
	virtual ioHashString &GetProgressResultOnlyText();
	virtual float GetProgressGaugeRate();

public:
	QuestDropKill();
	virtual ~QuestDropKill();
};
//////////////////////////////////////////////////////////////////////////
class QuestMultiKill : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		KILL_COUNT,
		BEST_ATTACKER_PASS,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();
	virtual ioHashString &GetProgressResult();
	virtual ioHashString &GetProgressResultOnlyNumber();
	virtual ioHashString &GetProgressResultOnlyText();
	virtual float GetProgressGaugeRate();

public:
	virtual void ChangeStageInit();

public:
	QuestMultiKill();
	virtual ~QuestMultiKill();
};
//////////////////////////////////////////////////////////////////////////
class QuestPvPConsecution : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		CONSECUTION_COUNT,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();

public:
	QuestPvPConsecution();
	virtual ~QuestPvPConsecution();
};
//////////////////////////////////////////////////////////////////////////
class QuestCampConsecution : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		CONSECUTION_COUNT,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();

public:
	QuestCampConsecution();
	virtual ~QuestCampConsecution();
};
//////////////////////////////////////////////////////////////////////////
class QuestEtcItemUse : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		USE_COUNT,
		MAX_ETC_ITEM,
		ETC_ITEM_CODE,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();

public:
	QuestEtcItemUse();
	virtual ~QuestEtcItemUse();
};
//////////////////////////////////////////////////////////////////////////
class QuestEtcItemCnt : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		CHECK_COUNT,
		ETC_ITEM_CODE,
		BOOLEAN_ETC_ITEM_DELETE,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckAlreadyAttain();

public:
	virtual int GetProgressCount();

public:
	void ProcessRewardComplete();

public:
	QuestEtcItemCnt();
	virtual ~QuestEtcItemCnt();
};
//////////////////////////////////////////////////////////////////////////
class QuestRequestFriend : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	QuestRequestFriend();
	virtual ~QuestRequestFriend();
};
//////////////////////////////////////////////////////////////////////////
class QuestMakeFriends : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		FRIEND_COUNT,
		LIMIT_FRIEND_COUNT,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckAlreadyAttain();

public:
	virtual int GetProgressCount();

public:
	QuestMakeFriends();
	virtual ~QuestMakeFriends();
};
//////////////////////////////////////////////////////////////////////////
class QuestModePlayTime : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		CHECK_PLAY_MIN,
		CHECK_SOLDIER,
	};

protected:
	CEncrypt<DWORD> m_dwProcessTime;
	CEncrypt<DWORD> m_dwAccrueSec;

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsAbuseCheckQuest(){ return true; }
	virtual void StartProcessTime();

public:
	virtual int GetProgressCount();

public:
	virtual void ChangeStageInit();
	virtual void Process( ioPlayStage *pPlayStage );

public:
	QuestModePlayTime();
	virtual ~QuestModePlayTime();
};
//////////////////////////////////////////////////////////////////////////
class QuestStoneAttack : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		CHECK_COUNT,
		TEAM_STYLE,
	};

	enum
	{
		SAME_TEAM = 0,
		OTHER_TEAM,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();
	virtual ioHashString &GetProgressResult();
	virtual ioHashString &GetProgressResultOnlyNumber();
	virtual ioHashString &GetProgressResultOnlyText();
	virtual float GetProgressGaugeRate();

public:
	QuestStoneAttack();
	virtual ~QuestStoneAttack();
};
//////////////////////////////////////////////////////////////////////////
class QuestKingAttack : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		CHECK_COUNT,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();

public:
	QuestKingAttack();
	virtual ~QuestKingAttack();
};
//////////////////////////////////////////////////////////////////////////
class QuestCrownHoldTime : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		CHECK_COUNT,
	};
	
protected:
	DWORD m_dwMilliSecond;

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();

public:
	QuestCrownHoldTime();
	virtual ~QuestCrownHoldTime();
};
//////////////////////////////////////////////////////////////////////////
class QuestBossModeBecomeBoss : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		CHECK_COUNT,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();

public:
	QuestBossModeBecomeBoss();
	virtual ~QuestBossModeBecomeBoss();
};
//////////////////////////////////////////////////////////////////////////
class QuestBossModeBosswithKill : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		CHECK_COUNT,
		DEATH_INIT,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();

public:
	virtual void ChangeStageInit();
	virtual void Process( ioPlayStage *pPlayStage );

public:
	QuestBossModeBosswithKill();
	virtual ~QuestBossModeBosswithKill();
};
//////////////////////////////////////////////////////////////////////////
class QuestMortmainSoldierCount : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		CHECK_COUNT,
		LIMIT_MORTMAIN_SOLDIER,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckAlreadyAttain();

public:
	virtual int GetProgressCount();

public:
	QuestMortmainSoldierCount();
	virtual ~QuestMortmainSoldierCount();
};
////////////////////////////////////////////////////////////////////////// 20100624 
class QuestHitCount : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		CHECK_COUNT,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();

public:
	QuestHitCount();
	virtual ~QuestHitCount();
};
//////////////////////////////////////////////////////////////////////////
class QuestDefenceCount : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		CHECK_COUNT,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();

public:
	QuestDefenceCount();
	virtual ~QuestDefenceCount();
};
//////////////////////////////////////////////////////////////////////////
class QuestPickItem : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		CHECK_COUNT,
		MAX_ITEM_CODE,
		PICK_ITEM_CODE,		
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();

public:
	QuestPickItem();
	virtual ~QuestPickItem();
};
//////////////////////////////////////////////////////////////////////////
class QuestCampLevel : public QuestParent
{
public:
	enum
	{
		LIMIT_CAMP_LEVEL = 0,
		CHECK_CAMP_LEVEL,
		MINUS_CAMP_LEVEL,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckAlreadyAttain();

public:
	virtual int GetProgressCount();
	virtual ioHashString &GetProgressResult();
	virtual ioHashString &GetProgressResultOnlyNumber();
	virtual ioHashString &GetProgressResultOnlyText();
	virtual float GetProgressGaugeRate();

public:
	QuestCampLevel();
	virtual ~QuestCampLevel();
};
//////////////////////////////////////////////////////////////////////////
class QuestBuyItem : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		CHECK_COUNT,
		MAX_ITEM_CODE,
		ITEM_CODE,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();

public:
	QuestBuyItem();
	virtual ~QuestBuyItem();
};
//////////////////////////////////////////////////////////////////////////
class QuestMaxFriendSlot : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		CHECK_COUNT,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckAlreadyAttain();

public:
	virtual int GetProgressCount();

public:
	QuestMaxFriendSlot();
	virtual ~QuestMaxFriendSlot();
};
//////////////////////////////////////////////////////////////////////////
class QuestRealTimeCount : public QuestParent
{
public:
	enum
	{
		LIMIT_MIN_GRADE = 0,
		LIMIT_MAX_GRADE,
		CHECK_COUNT,
	};

protected:
	DWORD m_dwProcessTime;
	DWORD m_dwAccrueSec;

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsAbuseCheckQuest(){ return true; }
	virtual void StartProcessTime();

public:
	virtual int GetProgressCount();
	virtual ioHashString &GetProgressResult();
	virtual ioHashString &GetProgressResultOnlyNumber();
	virtual ioHashString &GetProgressResultOnlyText();
	virtual float GetProgressGaugeRate();

public:
	virtual void Process( ioPlayStage *pPlayStage );

public:
	QuestRealTimeCount();
	virtual ~QuestRealTimeCount();
};
//////////////////////////////////////////////////////////////////////////
class QuestPlayTimeCount : public QuestParent
{
public:
	enum
	{
		LIMIT_MIN_GRADE = 0,
		LIMIT_MAX_GRADE,
		CHECK_COUNT,
	};

protected:
	CEncrypt<DWORD> m_dwProcessTime;
	CEncrypt<DWORD> m_dwAccrueSec;

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsAbuseCheckQuest(){ return true; }
	virtual void StartProcessTime();

public:
	virtual int GetProgressCount();
	virtual ioHashString &GetProgressResult();
	virtual ioHashString &GetProgressResultOnlyNumber();
	virtual ioHashString &GetProgressResultOnlyText();
	virtual float GetProgressGaugeRate();

public:
	virtual void Process( ioPlayStage *pPlayStage );

public:
	QuestPlayTimeCount();
	virtual ~QuestPlayTimeCount();
};
//////////////////////////////////////////////////////////////////////////
class QuestFriendRecommendPlayTime : public QuestParent
{
public:
	enum
	{
		LIMIT_MIN_GRADE = 0,
		LIMIT_MAX_GRADE,
		CHECK_COUNT,
	};

protected:
	CEncrypt<DWORD> m_dwProcessTime;
	CEncrypt<DWORD> m_dwAccrueSec;

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsAbuseCheckQuest(){ return true; }
	virtual void StartProcessTime();

public:
	virtual int GetProgressCount();
	virtual ioHashString &GetProgressResult();
	virtual ioHashString &GetProgressResultOnlyNumber();
	virtual ioHashString &GetProgressResultOnlyText();
	virtual float GetProgressGaugeRate();

public:
	virtual void Process( ioPlayStage *pPlayStage );

public:
	QuestFriendRecommendPlayTime();
	virtual ~QuestFriendRecommendPlayTime();
};
//////////////////////////////////////////////////////////////////////////
class QuestQuickStartOption : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		CHECK_INDEX,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	QuestQuickStartOption();
	virtual ~QuestQuickStartOption();
};
//////////////////////////////////////////////////////////////////////////
class QuestBuySoldier : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		CHECK_COUNT,
		CHECK_TYPE,
		CHECK_STYLE,
		MAX_SOLDIER,
		SOLDIER_CODE,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckAlreadyAttain();

public:
	virtual int GetProgressCount();

public:
	QuestBuySoldier();
	virtual ~QuestBuySoldier();
};
//////////////////////////////////////////////////////////////////////////
class QuestPvETimeAttack : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		LIMIT_ROUND,
		LIMIT_SECOND,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	QuestPvETimeAttack();
	virtual ~QuestPvETimeAttack();
};
//////////////////////////////////////////////////////////////////////////
class QuestSoccerBallHit : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		CHECK_COUNT,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();
	virtual ioHashString &GetProgressResult();
	virtual ioHashString &GetProgressResultOnlyNumber();
	virtual ioHashString &GetProgressResultOnlyText();
	virtual float GetProgressGaugeRate();

public:
	QuestSoccerBallHit();
	virtual ~QuestSoccerBallHit();
};
//////////////////////////////////////////////////////////////////////////
class QuestSoccerGoal : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		CHECK_COUNT,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();
	virtual ioHashString &GetProgressResult();
	virtual ioHashString &GetProgressResultOnlyNumber();
	virtual ioHashString &GetProgressResultOnlyText();
	virtual float GetProgressGaugeRate();

public:
	QuestSoccerGoal();
	virtual ~QuestSoccerGoal();
};
//////////////////////////////////////////////////////////////////////////
class QuestSoccerAssist : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		CHECK_COUNT,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();
	virtual ioHashString &GetProgressResult();
	virtual ioHashString &GetProgressResultOnlyNumber();
	virtual ioHashString &GetProgressResultOnlyText();
	virtual float GetProgressGaugeRate();

public:
	QuestSoccerAssist();
	virtual ~QuestSoccerAssist();
};
//////////////////////////////////////////////////////////////////////////
class QuestSoccerSave : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		CHECK_COUNT,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();

public:
	QuestSoccerSave();
	virtual ~QuestSoccerSave();
};
//////////////////////////////////////////////////////////////////////////
class QuestExcavationTry : public QuestParent
{
public:
	enum
	{
		LIMIT_EXCAVATION_LEVEL = 0,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	QuestExcavationTry();
	virtual ~QuestExcavationTry();
};
//////////////////////////////////////////////////////////////////////////
class QuestExcavationLevelUP : public QuestParent
{
public:
	enum
	{
		LIMIT_EXCAVATION_LEVEL = 0,
		CHECK_EXCAVATION_LEVEL,
		MINUS_EXCAVATION_LEVEL,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckAlreadyAttain();

public:
	virtual int GetProgressCount();
	virtual ioHashString &GetProgressResult();
	virtual ioHashString &GetProgressResultOnlyNumber();
	virtual ioHashString &GetProgressResultOnlyText();
	virtual float GetProgressGaugeRate();

public:
	QuestExcavationLevelUP();
	virtual ~QuestExcavationLevelUP();
};
//////////////////////////////////////////////////////////////////////////
class QuestExcavationSuccess : public QuestParent
{
public:
	enum
	{
		LIMIT_EXCAVATION_LEVEL = 0,
		CHECK_COUNT,
		LIMIT_PESO_GRADE,
		LIMIT_PESO,
		LIMIT_REINFORCE,
		LIMIT_PERIOD_TIME,
		MAX_ARTIFACT,
		ARTIFACT_CODE,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();

public:
	QuestExcavationSuccess();
	virtual ~QuestExcavationSuccess();
};
//////////////////////////////////////////////////////////////////////////
class QuestExcavationFail : public QuestParent
{
public:
	enum
	{
		LIMIT_EXCAVATION_LEVEL = 0,
		CHECK_COUNT,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();

public:
	QuestExcavationFail();
	virtual ~QuestExcavationFail();
};
//////////////////////////////////////////////////////////////////////////
class QuestExcavationTime : public QuestParent
{
public:
	enum
	{
		LIMIT_EXCAVATION_LEVEL = 0,
		CHECK_COUNT,
	};

protected:
	DWORD m_dwProcessTime;
	DWORD m_dwAccrueSec;

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsAbuseCheckQuest(){ return true; }
	virtual void StartProcessTime();

public:
	virtual int GetProgressCount();
	virtual ioHashString &GetProgressResult();
	virtual ioHashString &GetProgressResultOnlyNumber();
	virtual ioHashString &GetProgressResultOnlyText();
	virtual float GetProgressGaugeRate();

public:
	virtual void Process( ioPlayStage *pPlayStage );

public:
	QuestExcavationTime();
	virtual ~QuestExcavationTime();
};
//////////////////////////////////////////////////////////////////////////
class QuestScreenShot : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		CHECK_COUNT,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();

public:
	QuestScreenShot();
	virtual ~QuestScreenShot();
};
//////////////////////////////////////////////////////////////////////////
class QuestMovieMode : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	QuestMovieMode();
	virtual ~QuestMovieMode();
};
//////////////////////////////////////////////////////////////////////////
class QuestMakeMovie : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	QuestMakeMovie();
	virtual ~QuestMakeMovie();
};
//////////////////////////////////////////////////////////////////////////
class QuestExtraItemAcquire : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		LIMIT_PERIOD_TIME,
		MAX_EXTRA_ITEM,
		EXTRA_ITEM_CODE,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	QuestExtraItemAcquire();
	virtual ~QuestExtraItemAcquire();
};
//////////////////////////////////////////////////////////////////////////
class QuestExtraItemEquip : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		LIMIT_PERIOD_TIME,
		EXTRA_ITEM_CODE,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	QuestExtraItemEquip();
	virtual ~QuestExtraItemEquip();
};
//////////////////////////////////////////////////////////////////////////
class QuestExtraItemEquipKo : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		CHECK_COUNT,
		BEST_ATTACKER_PASS,
		EXTRA_ITEM_CODE,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();
	virtual ioHashString &GetProgressResult();
	virtual ioHashString &GetProgressResultOnlyNumber();
	virtual ioHashString &GetProgressResultOnlyText();
	virtual float GetProgressGaugeRate();

public:
	QuestExtraItemEquipKo();
	virtual ~QuestExtraItemEquipKo();
};
//////////////////////////////////////////////////////////////////////////
class QuestGangsiKill : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		CHECK_COUNT,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();

public:
	QuestGangsiKill();
	virtual ~QuestGangsiKill();
};
//////////////////////////////////////////////////////////////////////////
class QuestGangsiHumanKill : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		CHECK_COUNT,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();

public:
	QuestGangsiHumanKill();
	virtual ~QuestGangsiHumanKill();
};
//////////////////////////////////////////////////////////////////////////
class QuestGangsiAliveTime : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		CHECK_COUNT,
	};

protected:
	DWORD m_dwMilliSecond;

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();

public:
	QuestGangsiAliveTime();
	virtual ~QuestGangsiAliveTime();
};
//////////////////////////////////////////////////////////////////////////
class QuestGangsiHumanWin : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		CHECK_COUNT,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();

public:
	QuestGangsiHumanWin();
	virtual ~QuestGangsiHumanWin();
};
//////////////////////////////////////////////////////////////////////////
class QuestHitAttackAttribute : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		CHECK_COUNT,
		MAX_ATTRIBUTE_CODE,
		ATTRIBUTE_CODE,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();

public:
	QuestHitAttackAttribute();
	virtual ~QuestHitAttackAttribute();
};
//////////////////////////////////////////////////////////////////////////
class QuestGuildLevel : public QuestParent
{
public:
	enum
	{
		CHECK_COUNT = 0,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();

public:
	QuestGuildLevel();
	virtual ~QuestGuildLevel();
};
//////////////////////////////////////////////////////////////////////////
class QuestGuildLevelMaintenance : public QuestParent
{
public:
	enum
	{
		LIMIT_GUILD_LEVEL = 0,
		CHECK_COUNT,
		LIMIT_CAMP_REWARD,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();

public:
	QuestGuildLevelMaintenance();
	virtual ~QuestGuildLevelMaintenance();
};
//////////////////////////////////////////////////////////////////////////
class QuestGuildTeamWin : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		WIN_COUNT,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual int GetProgressCount();
	virtual ioHashString &GetProgressResult();
	virtual ioHashString &GetProgressResultOnlyNumber();
	virtual ioHashString &GetProgressResultOnlyText();
	virtual float GetProgressGaugeRate();

public:
	QuestGuildTeamWin();
	virtual ~QuestGuildTeamWin();
};
//////////////////////////////////////////////////////////////////////////
class QuestGuildTeamPlayTime : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		CHECK_PLAY_MIN,
	};

protected:
	DWORD m_dwProcessTime;
	DWORD m_dwAccrueSec;

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsAbuseCheckQuest(){ return true; }
	virtual void StartProcessTime();

public:
	virtual int GetProgressCount();

public:
	virtual void ChangeStageInit();
	virtual void Process( ioPlayStage *pPlayStage );

public:
	QuestGuildTeamPlayTime();
	virtual ~QuestGuildTeamPlayTime();
};
//////////////////////////////////////////////////////////////////////////
class QuestPlayTimeRepeat : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		REPEAT_TIME,          // �ݺ� �ð�
	};

protected:
	DWORD m_dwProcessTime;
	DWORD m_dwAccrueSec;

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsAbuseCheckQuest(){ return true; }
	virtual void StartProcessTime();

public:
	virtual int GetProgressCount();
	virtual ioHashString &GetProgressResult();
	virtual ioHashString &GetProgressResultOnlyNumber();
	virtual ioHashString &GetProgressResultOnlyText();
	virtual float GetProgressGaugeRate();

public:
	virtual bool AddAttainProcess( int iAddValue = 0, int iCheckValue = 0 );

public:
	virtual void Process( ioPlayStage *pPlayStage );

public:
	QuestPlayTimeRepeat();
	virtual ~QuestPlayTimeRepeat();
};
//////////////////////////////////////////////////////////////////////////
class QuestDormantUser : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		LIMIT_DAY,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckAlreadyAttain();

public:
	virtual void Process( ioPlayStage *pPlayStage );

public:
	QuestDormantUser();
	virtual ~QuestDormantUser();
};
//////////////////////////////////////////////////////////////////////////
class QuestDormantUserCustom : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		LIMIT_YEAR,
		LIMIT_MONTH,
		LIMIT_DAY,
	};

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckAlreadyAttain();
	
public:
	QuestDormantUserCustom();
	virtual ~QuestDormantUserCustom();
};
//////////////////////////////////////////////////////////////////////////
class QuestDevKMove : public QuestParent
{
public:
	enum
	{
		LIMIT_GRADE = 0,
		POSITION_X,
		POSITION_Y,
		POSITION_Z,
		POSITION_RANGE,
	};

protected:
	DWORD m_dwProcessTime;

public:
	virtual bool IsCheckOccurTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );
	virtual bool IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );

public:
	virtual void Process( ioPlayStage *pPlayStage );

public:
	QuestDevKMove();
	virtual ~QuestDevKMove();
};
