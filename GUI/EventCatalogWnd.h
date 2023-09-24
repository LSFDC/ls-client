#pragma once


#include "../io3DEngine/ioMovingWnd.h"

class ioXMLElement;
typedef struct tagEventInfo
{
	bool         m_bWebEvent;
	ioHashString m_szEventURL;
	DWORD        m_dwStartDate;
	DWORD        m_dwEndDate;
	DWORD        m_dwEventWndID;
	int          m_iEventType;
	ioHashString m_szEventImgFirst;
	ioHashString m_szEventImgSecond;
	ChannelingType m_eChannelingType;

	tagEventInfo()
	{
		m_bWebEvent   = false;
		m_dwStartDate = 0;
		m_dwEndDate   = 0;
		m_dwEventWndID= 0;
		m_iEventType  = 0;
		m_eChannelingType = CNT_NONE;
	}

}EventInfo;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class EventCatalogWnd : public ioMovingWnd
{
public:
	enum
	{
		ID_EVENT_1_BTN = 1,
		ID_EVENT_2_BTN = 2,
		ID_EVENT_3_BTN = 3,
		ID_EVENT_4_BTN = 4,
		ID_EVENT_5_BTN = 5,
		ID_EVENT_6_BTN = 6,
		ID_EVENT_7_BTN = 7,
		ID_EVENT_8_BTN = 8,
		ID_EVENT_9_BTN = 9,
		ID_EVENT_10_BTN = 10,

		MAX_BTN        = 10,

		ID_EXIT_BTN    = 101,
		ID_DETAIL_BTN  = 1001,
	};

protected:
	typedef std::vector<EventInfo*> vEventInfoVector;

protected:
	static HWND m_IEHwnd;
	static BOOL CALLBACK EnumProc(HWND hWnd,LPARAM lParam);

protected:
	vEventInfoVector m_vEventInfoVector;
	int              m_iSelectId;
	ioUIRenderImage *m_pImgFirst;
	ioUIRenderImage *m_pImgSecond;

	DWORD m_dwIERefreshTimer;
	bool  m_bIERefreshCheck;

public:
	void SetShowHideChild( int iShowEventType = -1 );

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

	void ProcessIERefresh();
	void SetShowEventPage( int iVectorArray );
	bool IsAlive( const EventInfo &rkEventInfo );
	void SetImg( int iVectorArray );

	int GetSelectIdByEventType( int iEventType );

	void SendEventGiftRequest();

public:
	virtual void iwm_show();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );
	virtual bool iwm_spacebar();
	virtual void iwm_create();
	virtual void ParseExtraInfo( ioXMLElement &xElement );

public:
	EventCatalogWnd(void);
	virtual ~EventCatalogWnd(void);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class EvnetChildrenDay : public ioWnd
{
public:
	enum
	{
		ID_EXIT   = 1,
		ID_OK     = 2,
		ID_ACTION = 3,
	};

protected:
	static bool m_bAutoShow;
	ioPlayStage *m_pPlayStage;  

public:
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );
	virtual bool iwm_spacebar();

protected:
	virtual void OnRender();
	virtual void OnProcess( float fTimePerSec );

public:
	void SetPlayStage( ioPlayStage *pPlayStage );
	static void SetAutoShow(bool bAutoShow) { m_bAutoShow = bAutoShow; }

public:
	EvnetChildrenDay();
	virtual ~EvnetChildrenDay();
};

//////////////////////////////////////////////////////////////////////////
class EventMortMainWnd : public ioWnd
{
public:
	enum
	{
		ID_EXIT = 1,
		ID_OK   = 2,
	};

protected:
	static bool sg_bBonusPeso;

public:
	static void ShowEventPopup(){ sg_bBonusPeso = true; }

public:
	virtual void iwm_hide();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

public:
	EventMortMainWnd();
	virtual ~EventMortMainWnd();

};

//////////////////////////////////////////////////////////////////////////
class EventChanceMortmainCharWnd : public ioMovingWnd
{
public:
	enum
	{
		ID_EXIT = 1,
		ID_NORMAL_CHANCE = 2,
		ID_PCROOM_CHANCE = 3,
		ID_PCROOM_ALARM  = 4,
	};

protected:
	ioUIRenderImage *m_pGaugeBack;
	ioUIRenderImage *m_pGaugeLine;
	ioUIRenderImage *m_pTimeText;
	ioUIRenderImage *m_pTimeColon;

public:
	virtual void iwm_show();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

protected:
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

public:
	EventChanceMortmainCharWnd();
	virtual ~EventChanceMortmainCharWnd();
};

//////////////////////////////////////////////////////////////////////////
class EventChanceRollingWnd : public ioMovingWnd
{
public:
	enum
	{
		ID_EXIT   = 1,
		ID_CHANCE = 2,
		ID_OK     = 3,
	};

protected:
	ioUIRenderImage *m_pPesoIcon;
	ioUIRenderImage *m_pSoldierIcon;
	ioUIRenderImage *m_pLotteryText;
	ioUIRenderImage *m_pCongratulationText;

	static DWORD sg_dwCurRandTime;
	static bool  sg_bShow;

	DWORD m_dwCurRandTickTime;

	DWORD m_dwRandTime;
	DWORD m_dwRandTickTime;
	ioHashString m_szRollingSound;
	ioHashString m_szPesoSound;
	ioHashString m_szCharSound;

	// 오리지날 보상
	static int sg_iOriginalBonus;           // 11110000     페소 | 용병타입

	// 랜덤 보상
	int m_iRandBonus;

protected:
	void OnProcessRand();

public:
	void StartRandAni();

public:
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual bool iwm_esc();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

protected:
	virtual void ParseExtraInfo( ioXMLElement &xElement );
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();
	void RenderOriginalBonus( int iXPos, int iYPos );
	void RenderRandBonus( int iXPos, int iYPos );

public:
	static void SetBonusType( int iBonusType );

public:
	EventChanceRollingWnd();
	virtual ~EventChanceRollingWnd();
};
//////////////////////////////////////////////////////////////////////////
class OneDayGoldItemEventWnd : public ioWnd
{
public:
	enum
	{
		ID_EXIT		 = 1,
		ID_RECV_ITEM = 2,
		ID_LINK      = 3,
	};

protected:
	enum
	{
		STATE_NONE    = 0,
		STATE_RECV    = 1,
		STATE_ROLLING = 2,
		STATE_COMPLETE= 3,
	};
	DWORD m_dwState;

	// STATE_ROLLING 정보
	int		m_iRollingIconArray;
	DWORD	m_dwCurrentFullTime;
	DWORD   m_dwRollingFullTime;
	DWORD	m_dwCurrentTickTime;
	DWORD   m_dwRollingTickTime;
	ioHashString m_szRollingSound;
	ioHashString m_szCompleteSound;

	// STATE_COMPLETE 정보
	short m_iPresentType;
	int   m_iPresentValue1;
	int   m_iPresentValue2;
	int   m_iPresentValue3;
	int   m_iPresentValue4;
	bool  m_bAlarm;
	float m_fCompleteIconScale;
	DWORD m_dwCompleteScaleTimer;

protected:
	ioUIRenderImage *m_pBackRedLight;
	ioUIRenderImage *m_pBackBlueLight;
	ioUIRenderImage *m_pRecvIcon;
	ioUIRenderImage *m_pTextStateRecv;
	ioUIRenderImage *m_pTextStateRolling;
	ioUIRenderImage *m_pTextStateComplete;
	ioUIRenderImage *m_pPresentIcon;
	ioUIRenderImage *m_pRollingIcon;
	
protected:	 
	typedef std::vector< ioUIRenderImage * > ioUIRenderImageVec;
	ioUIRenderImageVec m_vRollingIconList;

public:
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual bool iwm_esc();
	virtual bool iwm_spacebar();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

protected:
	virtual void ParseExtraInfo( ioXMLElement &xElement );
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

protected:
	void ProcessRolling();
	void ProcessComplete();

protected:
	void OnStateRecvRender( int iXPos, int iYPos );
	void OnStateRollingRender( int iXPos, int iYPos );
	void OnStateCompleteRender( int iXPos, int iYPos );

public:
	void SetRecvState();
	void SetRollingState();
	void SetCompleteState( short iPresentType, int iPresentValue1, int iPresentValue2, int iPresentValue3, int iPresentValue4, bool bAlarm );

public:
	OneDayGoldItemEventWnd();
	virtual ~OneDayGoldItemEventWnd();
};
