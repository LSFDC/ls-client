#pragma once

enum MissionClasses
{
	MISSION_CLASS_NONE				= 0,
	MISSION_CLASS_LOGINTIME_CHECK,
	MISSION_CLASS_FISHING,
	MISSION_CLASS_PESO_COLLECT,
	MISSION_CLASS_MONSTER_KILL,
	MISSION_CLASS_ITEM_BUY,
	MISSION_CLASS_CLOVER,
	MISSION_CLASS_EXCAVATION,
	MISSION_CLASS_ITEM_USE,
	MISSION_CLASS_ITEMREINFORCE,
	MISSION_CLASS_MODEPLAY,
	MISSION_CLASS_DAILY_COMPLETE,		//�ְ��̼�. �ϰ��̼��� � �ߴ��� üũ
	MISSION_CLASS_DAILY_ALL_CLEAR,		//�ϰ��̼�. ���� �ϰ��̼��� ��Ŭ�����ϸ� �ش�.
	MISSION_CLASS_WEEKLY_COMPLETE,		//�����̼�. �ְ��̼��� ���� �ߴ��� üũ
	MISSION_CLASS_WEEKLY_ALL_CLEAR,		// �ְ� �̼� : �ְ��̼��� ��Ŭ�����ϸ� �ش�.
	MISSION_CLASS_MONTHLY_ALL_CLEAR,	// ���� �̼� : �����̼��� ��Ŭ�����ϸ� �ش�.
	MISSION_CLASS_ADDITIVE_USE,
};

enum MissionBackFrameType
{
	MBFT_NORMAL,
	MBFT_YELLOW,
};

enum MissionAttainShowStyle
{
	MASS_VALUE,	// CurValue / MaxValue Style
	MASS_GAUGE, // Gauege Style
};

//sort �켱����
//1���� : ����,�ְ�,���� �̼� ��Ŭ���� �̼�
//2���� : �޼������� ������ ���� ���� �̼�
//3���� : �޼����� �̼�
//4���� : �޼��� ������ ���� �̼�
enum MissionSortType
{
	MSORT_NONE,
	MSORT_ALL_CLEAR,
	MSORT_ATTAIN,
	MSORT_PROGRESS,
	MSORT_COMPLETE,
};

class ioMission
{
protected:
	struct ColorText
	{
		DWORD m_dwColor;
		ioHashString m_szText;

		ColorText()
		{
			m_dwColor = 0;
		}

		void Clear()
		{
			m_dwColor = 0;
			m_szText.Clear();
		}
	};
public:
	typedef std::vector< ColorText > vTitleLine;
	typedef std::vector< vTitleLine > vTitleText;
protected:
	DWORD					m_dwStartDay;
	// ������ �����Ǵ� ������ �̼� �ڵ�
	int						m_iMagicCode;

	int						m_iMissionType;
	// ������ ���� �̼��� Ÿ�Ը��� �ٸ���.
	int						m_iValue1;
	int						m_iValue2;
	int						m_iValue3;
	int						m_iValue4;
	ioHashString			m_IconName;
	BYTE					m_byClearMissionType;
	int						m_iCurAchieveValue;
	int						m_iPresentNum;

	//UI
	MissionBackFrameType	m_BackFrameType;
	MissionAttainShowStyle	m_AttainShowStyle;
	ioHashStringVec			m_szRewardTextVec;
	vTitleText				m_vTitle;
	MissionSortType			m_MissionSortType;
	bool					m_bShowToolTipReward;
	bool					m_bIsCostume;

protected:
	void								LoadTitleInfo( const char* szINIKeyName, ioINILoader& kLoader );

	//Get Func
public:
	const int&							GetMagicCode()			const { return m_iMagicCode; }
	const int&							GetMissionType()		const { return m_iMissionType; }
	const ioHashString&					GetIconName()			const { return m_IconName; }
	const BYTE&							GetClearMissionType()	const { return m_byClearMissionType; }
	const int&							GetPresentNumber()		const { return m_iPresentNum; }
	const MissionBackFrameType&			GetBackFrameType()		const { return m_BackFrameType; }
	const MissionAttainShowStyle&		GetAttinShowStyle()		const { return m_AttainShowStyle; }
	const ioHashStringVec&				GetRewardTextVec()		const { return m_szRewardTextVec; }
	const int&							GetCurAchieveValue()	const { return m_iCurAchieveValue; }
	const vTitleText&					GetTitlePrinter()		const { return m_vTitle; }
	const MissionSortType&				GetMissionSortType()	const { return m_MissionSortType; }
	const bool&							GetShowToolTipReward()	const { return m_bShowToolTipReward; }
	const bool							IsMissionCostume()		const { return m_bIsCostume; }
	
	//Set Func
public:
	void SetMissionState( int iCurAchieveValue, BYTE byCurAchieveValue );
protected:
	void								SetCurAchieveValue( int iCurAchieveValue );
	void								SetClearMissionType( BYTE byCurAchieveValue );
	void								SetSortType();

//Virtual Func
public:
	//�� �ش� Ŭ������ Ÿ�Ը��� �޼�ġ Value�� �ٸ��� ������( ����� Value1�̰� ����� Value2�̰�)
	//���� �����Լ��� ��ӹ޴¾Ƽ� �����Ѵ�.
	virtual const int&					GetMaxAchieveValue() const = 0;
	
	virtual void						Init( const char* szINIKeyName, ioINILoader &kLoader );
	virtual bool						Process();
	virtual ioMission*					Clone() = 0;
public:
	ioMission(void);
	ioMission( const ioMission &rhs );
	~ioMission(void);
};

////////////////////////////////////////////////////////////////////////////////
class ioLoginTimeMission : public ioMission
{
protected:
	CTime				m_NowTime;
	int					m_iLoginwSecond;
	DWORD				m_dwProcessTime;
	int					m_iGapMinute;
public:
	virtual void		Init( const char* szINIKeyName, ioINILoader &kLoader );
	// �α��� ��ǥ ���� �ð�(�� ����)
	virtual const int&  GetMaxAchieveValue() const { return m_iValue1; }
	virtual bool		Process();
	virtual ioMission*	Clone();
public:
	ioLoginTimeMission(void);
	ioLoginTimeMission( const ioLoginTimeMission &rhs );
	virtual ~ioLoginTimeMission(void);
};

////////////////////////////////////////////////////////////////////////////////
class ioFishingMission : public ioMission
{
public:
	// ���� ��ü/����/���� Ƚ��
	virtual const int&  GetMaxAchieveValue() const { return m_iValue1; }
	virtual ioMission*	Clone();
public:
	ioFishingMission(void);
	ioFishingMission( const ioFishingMission &rhs );
	virtual ~ioFishingMission(void);
};

////////////////////////////////////////////////////////////////////////////////
class ioPesoCollectMission : public ioMission
{
public:
	// ��� ȹ��
	virtual const int&  GetMaxAchieveValue() const { return m_iValue1; }
	virtual ioMission*	Clone();
public:
	ioPesoCollectMission(void);
	ioPesoCollectMission( const ioPesoCollectMission &rhs );
	virtual ~ioPesoCollectMission(void);
};

////////////////////////////////////////////////////////////////////////////////
class ioMonsterKillMission : public ioMission
{
public:
	// NPC ��� ( ��� �� �ذ� ) ��ǥġ
	virtual const int&  GetMaxAchieveValue() const { return m_iValue1; }
	virtual ioMission*	Clone();
public:
	ioMonsterKillMission(void);
	ioMonsterKillMission( const ioMonsterKillMission &rhs );
	virtual ~ioMonsterKillMission(void);
};

////////////////////////////////////////////////////////////////////////////////
class ioItemBuyMission : public ioMission
{
public:
	// ������(��񺸱�/ETC) ����/ȹ�� ��ǥ �޼�ġ
	virtual const int&  GetMaxAchieveValue() const { return m_iValue3; }
	virtual ioMission*	Clone();
public:
	ioItemBuyMission(void);
	ioItemBuyMission( const ioItemBuyMission &rhs );
	virtual ~ioItemBuyMission(void);
};

////////////////////////////////////////////////////////////////////////////////
class ioCloverMission : public ioMission
{
public:
	//Ŭ�ι� ������/�ޱ� Ƚ��
	virtual const int&  GetMaxAchieveValue() const { return m_iValue1; }
	virtual ioMission*	Clone();
public:
	ioCloverMission(void);
	ioCloverMission( const ioCloverMission &rhs );
	virtual ~ioCloverMission(void);
};

////////////////////////////////////////////////////////////////////////////////
class ioExcavationMission : public ioMission
{
public:
	//Ž�� ��ü/����/���� Ƚ��
	virtual const int&  GetMaxAchieveValue() const { return m_iValue1; }
	virtual ioMission*	Clone();
public:
	ioExcavationMission(void);
	ioExcavationMission( const ioExcavationMission &rhs );
	virtual ~ioExcavationMission(void);
};

////////////////////////////////////////////////////////////////////////////////
class ioItemUseMission : public ioMission
{
public:
	//Ư�� ������ ���/�Ҹ� ( ��񺸱�/ETC(��í,���۰�í)/���Ƿ� )
	virtual const int&  GetMaxAchieveValue() const { return m_iValue3; }
	virtual ioMission*	Clone();
public:
	ioItemUseMission(void);
	ioItemUseMission( const ioItemUseMission &rhs );
	virtual ~ioItemUseMission(void);
};

////////////////////////////////////////////////////////////////////////////////
class ioItemReinforceMission : public ioMission
{
public:
	// ��ȭ ��ü/����/���� Ƚ��
	virtual const int&  GetMaxAchieveValue() const { return m_iValue1; }
	virtual ioMission*	Clone();
public:
	ioItemReinforceMission(void);
	ioItemReinforceMission( const ioItemReinforceMission &rhs );
	virtual ~ioItemReinforceMission(void);
};

////////////////////////////////////////////////////////////////////////////////
class ioModePlayMission : public ioMission
{
public:
	//Ư�� ��� �÷��� Ƚ��
	virtual const int&  GetMaxAchieveValue() const { return m_iValue1; }
	virtual ioMission*	Clone();
public:
	ioModePlayMission(void);
	ioModePlayMission( const ioModePlayMission &rhs );
	virtual ~ioModePlayMission(void);
};

////////////////////////////////////////////////////////////////////////////////
class ioDailyCompleteMisson : public ioMission
{
public:
	//���� �̼� �޼� üũ
	virtual const int&  GetMaxAchieveValue() const { return m_iValue1; }
	virtual ioMission*	Clone();
public:
	ioDailyCompleteMisson(void);
	ioDailyCompleteMisson( const ioDailyCompleteMisson &rhs );
	virtual ~ioDailyCompleteMisson(void);
};

////////////////////////////////////////////////////////////////////////////////
class ioDailyAllClearMisson : public ioMission
{
public:
	//���� ���Ϲ̼� ��� �Ϸ� ����
	virtual const int&  GetMaxAchieveValue() const { return m_iValue1; }
	virtual ioMission*	Clone();
public:
	ioDailyAllClearMisson(void);
	ioDailyAllClearMisson( const ioDailyAllClearMisson	 &rhs );
	virtual ~ioDailyAllClearMisson(void);
};

////////////////////////////////////////////////////////////////////////////////
class ioWeeklyCompleteMission : public ioMission
{
public:
	//�ְ� �̼� �޼� üũ
	virtual const int&  GetMaxAchieveValue() const { return m_iValue1; }
	virtual ioMission*	Clone();
public:
	ioWeeklyCompleteMission(void);
	ioWeeklyCompleteMission( const ioWeeklyCompleteMission &rhs );
	virtual ~ioWeeklyCompleteMission(void);
};

////////////////////////////////////////////////////////////////////////////////
class ioWeeklyAllClearMission : public ioMission
{
public:
	//�ش� �ְ� �̼� ��� �Ϸ� ����
	virtual const int&  GetMaxAchieveValue() const { return m_iValue1; }
	virtual ioMission*	Clone();
public:
	ioWeeklyAllClearMission(void);
	ioWeeklyAllClearMission( const ioWeeklyAllClearMission &rhs );
	virtual ~ioWeeklyAllClearMission(void);
};

////////////////////////////////////////////////////////////////////////////////
class ioMonthlyAllClearMission : public ioMission
{
public:
	//���� �̼� ��� �Ϸ� ����
	virtual const int&  GetMaxAchieveValue() const { return m_iValue1; }
	virtual ioMission*	Clone();
public:
	ioMonthlyAllClearMission(void);
	ioMonthlyAllClearMission( const ioMonthlyAllClearMission &rhs );
	virtual ~ioMonthlyAllClearMission(void);
};

////////////////////////////////////////////////////////////////////////////////
class ioAdditiveUseMission : public ioMission
{
public:
	//���� ���� ���
	virtual const int&  GetMaxAchieveValue() const { return m_iValue1; }
	virtual ioMission*	Clone();
public:
	ioAdditiveUseMission(void);
	ioAdditiveUseMission( const ioAdditiveUseMission &rhs );
	virtual ~ioAdditiveUseMission(void);
};

////////////////////////////////////////////////////////////////////////////////