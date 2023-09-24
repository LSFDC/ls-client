#pragma once

#include "../io3DEngine/ioMovingWnd.h"

#include "FirstHireWnd.h"
#include "ExtendedButton.h"
#include "SoldierExerciseWnd.h"

// EVT_PROPOSAL
class EventWnd : public FirstHireWnd
{
public:
	enum 
	{
		// FirstHireWnd 1 부터 사용
		// BankruptcyCharWnd 2000 부터 사용

		ID_GET_CHAR_BTN = 1000,
	};
protected:
	virtual void UpdateFirstClass( OUT IntVec &rvDeleteArrayList );
	virtual void LoadINI( IN const IntVec &rvDeleteArrayList );
	virtual void SetBtnTitle();

public:
	virtual void iwm_show();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

public:
	EventWnd(void);
	virtual ~EventWnd(void);
};

//---------------------------------------------------------------------------------------------------------------------------------
// EVT_PLAYTIME

class PlayTimeEventBtn : public TwinkleButton
{
public:
	enum ColorType 
	{
		CT_ORANGE = 1,
		CT_YELLOW = 2,
		CT_GRAY   = 3,
	};
protected:
	ColorType m_eColorType;

public:
	ColorType GetColorType() const { return m_eColorType; }
	void SetColorType( ColorType eColorType) { m_eColorType = eColorType; }

public:
	PlayTimeEventBtn();
	virtual ~PlayTimeEventBtn();
};
//---------------------------------------------
class PlayTimeEventWnd : public ioMovingWnd
{
public:
	enum
	{
		ID_GIFT_1_BTN    = 1,
		ID_GIFT_2_BTN    = 2,
		ID_GIFT_3_BTN    = 3,
		ID_GIFT_4_BTN    = 4,
		ID_GIFT_5_BTN    = 5,
		ID_EXIT_BTN      = 6,
		ID_OK_BTN        = 7,

		MAX_GIFT_BTN     = 5,
	};

protected:
	typedef struct tagTextInfo
	{
		ioHashString m_szText;
		DWORD        m_dwColor;
		bool         m_bChangeLine;
		int          m_iXOffset;

		tagTextInfo()
		{
			m_szText  = "";
			m_dwColor = 0;
			m_bChangeLine = false;
			m_iXOffset    = 0;
		}
	}TextInfo;

	typedef std::vector<TextInfo> TextInfoVector;

	bool m_bAutoShow;
	ioPlayStage *m_pPlayStage;
	TextInfoVector m_kTextInfoVector;

	ioUIRenderImage *m_pGauge;
	ioUIRenderImage *m_pGaugePartitionMark;
	ioUIRenderImage *m_pNumber;
	ioUIRenderImage *m_pDisableNumber;

public:
	virtual void iwm_show();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );
	virtual void ParseExtraInfo( ioXMLElement &xElement );
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );

public:
	void SetPlayStage(ioPlayStage *pPlayStage) { m_pPlayStage = pPlayStage; }
	void UpdateBtns();

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();
	
protected:
	void CheckAndSend( DWORD dwID );
	void SetAutoShow();

	void OnRenderText( int iXPos, int iYPos );
	void OnRenderGauge( int iXPos, int iYPos );
	void OnRenderTime( int iXPos, int iYPos );
	void OnRenderGiftBtnText( int iXPos, int iYPos );

public:
	PlayTimeEventWnd();
	virtual ~PlayTimeEventWnd();
};

//------------------------------------------------------------------------------------------------------------------------------
class MgameOpenEventWnd : public ioWnd
{
public:
	enum
	{
		ID_EXIT = 1,
		ID_OK   = 2,
	};
protected:
	bool m_bOneShow; 
	int  m_iBonusPeso;

public:
	virtual void iwm_show();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );
	virtual bool iwm_esc();

protected:
	virtual void OnRender();

public:
	MgameOpenEventWnd(void);
	virtual ~MgameOpenEventWnd(void);
};
//----------------------------------------------------------------------------------------------------------------------------------
//EVT_PLAYTIME_PRESENT
class PlayTimePresentEventWnd : public ioMovingWnd
{
public:
	enum
	{
		ID_EXIT = 1,
		ID_NORMAL_CHANCE = 2,
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
	PlayTimePresentEventWnd();
	virtual ~PlayTimePresentEventWnd();
};
//----------------------------------------------------------------------------------------------------------------------------------
// EVT_EXERCISESOLDIER

#define EVENT_ICON_SCALE 0.80f
class ExerciseSoldierEventWnd : public ioWnd
{
public:
	enum
	{
		ID_EXERCISE_SOLDIER_1_BTN  = 101,
		ID_EXERCISE_SOLDIER_2_BTN  = 102,
		ID_EXERCISE_SOLDIER_3_BTN  = 103,
		ID_EXERCISE_SOLDIER_4_BTN  = 104,
		ID_EXERCISE_SOLDIER_5_BTN  = 105,
		ID_EXERCISE_SOLDIER_6_BTN  = 106,
		ID_EXERCISE_SOLDIER_7_BTN  = 107,
		ID_EXERCISE_SOLDIER_8_BTN  = 108,
		ID_EXERCISE_SOLDIER_9_BTN  = 109,
		ID_EXERCISE_SOLDIER_10_BTN  = 110,
		ID_EXERCISE_SOLDIER_11_BTN  = 111,
		ID_EXERCISE_SOLDIER_12_BTN  = 112,
		ID_EXERCISE_SOLDIER_13_BTN  = 113,
		ID_EXERCISE_SOLDIER_14_BTN  = 114,
		ID_EXERCISE_SOLDIER_15_BTN  = 115,
		ID_EXERCISE_SOLDIER_16_BTN  = 116,
		ID_EXERCISE_SOLDIER_17_BTN  = 117,
		ID_EXERCISE_SOLDIER_18_BTN  = 118,
		ID_EXERCISE_SOLDIER_19_BTN  = 119,
		ID_EXERCISE_SOLDIER_20_BTN  = 120,
		ID_EXERCISE_SOLDIER_21_BTN  = 121,
		ID_EXERCISE_SOLDIER_22_BTN  = 122,
		ID_EXERCISE_SOLDIER_23_BTN  = 123,
		ID_EXERCISE_SOLDIER_24_BTN  = 124,
		ID_EXERCISE_SOLDIER_25_BTN  = 125,

		ID_VERT_SCROLL              = 1001,
	};

	enum 
	{
		MAX_EXERCISE_SOLDIER_BTN = 25,
	};

protected:
	ioPlayStage *m_pPlayStage;
	int          m_iCurPos;
	int          m_iMaxPage;
	int          m_iCurBtnSize;
	vShopItemInfoList m_vItemInfoList;
	IntVec       m_vExtraClassVec;
	ioUIRenderFrame *m_pLeftFrm;

	void _UpdateChildPos(int a_iArray ,int iWndID);
	bool IsCanExerciseChar( bool bSelectWait );

	void SetOwnerChangeWaitState();
	void SendChangeWait( int iArray );
	void SetBlankIcon( int a_iWndID );

	void SetMaxPage();

	bool ChangeBoughtChar( int iArray, bool bWait );

	void SortItemInfo();
	int  GetSetIdx( int iArray );

	void SetWndBtnSize();

public:
	void SetPlayStage( ioPlayStage *pPlayStage );
	void SetBoughtType( SoldierExerciseBtn::SoldierBoughtType eBoughtType , int iCharArray );
	void UpdateChildPos();

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

public:	
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );
	virtual void iwm_create();
	virtual void iwm_show();	
	virtual void iwm_wheel( int zDelta );
	virtual void iwm_vscroll( DWORD dwID, int curPos );
	virtual void SetPositionToDefault( int iParentWidth, int iParentHeight, bool bChildMove, bool bCreate = false );
	virtual void ParseExtraInfo( ioXMLElement &xElement );
	virtual void AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame );

public:
	ExerciseSoldierEventWnd();
	virtual ~ExerciseSoldierEventWnd();
};

//-----------------------------------------------------------------------------------------------------------------------------------
// EVT_CONNECTION_TIME
class ConnectionTimeEventBtn : public ioButton
{
protected:
	ioUIRenderImage *m_pIcon;

	// ToolTip
	ioUIRenderImage *m_pToolTipCen;
	ioUIRenderImage *m_pToolTipLMid;
	ioUIRenderImage *m_pToolTipRMid;
	ioUIRenderImage *m_pToolTipLeft;
	ioUIRenderImage *m_pToolTipRight;

	ioHashString m_szName;

protected:
		void RenderToolTip();
public:
	void SetIcon( ioUIRenderImage *pIcon, float fIconScale, const ioHashString &rszName );

public:
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );

protected:
	virtual void OnDrawNormal( int iXPos, int iYPos );
	virtual void OnDrawOvered( int iXPos, int iYPos );
	virtual void OnDrawPushed( int iXPos, int iYPos );

public:
	ConnectionTimeEventBtn();
	virtual ~ConnectionTimeEventBtn();
};
//-----------------------------------------------
class ConnectionTimeEventWnd : public ioMovingWnd
{
public:
	enum
	{
		ID_EXIT_BTN   = 1,
		ID_CHANCE_BTN = 2,
		ID_ICON_BTN   = 3,
	};

protected:
	ioUIRenderImage *m_pGaugeBack;
	ioUIRenderImage *m_pGaugeLine;
	ioUIRenderImage *m_pTimeText;
	ioUIRenderImage *m_pTimeColon;

	static bool m_bProcess;
	DWORD       m_dwCheckChanceTime;

protected:
	void ProcessChanceRequest(); 

public:
	virtual void iwm_show();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );
	virtual void iwm_create();

protected:
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

public:
	ConnectionTimeEventWnd();
	virtual ~ConnectionTimeEventWnd();
};

//-----------------------------------------------------------------------------------------------------------------------------------
// EVT_CONNECTION_TIME_SELECT_GIFT
class ConnectionTimeSelectGiftEventBtn : public ioButton
{
protected:
	enum 
	{
		CTSG_ICON_HEIGHT = 43,
	};

protected:
	ioUIRenderImage *m_pIcon;

	// ToolTip
	ioUIRenderImage *m_pToolTipCen;
	ioUIRenderImage *m_pToolTipLMid;
	ioUIRenderImage *m_pToolTipRMid;
	ioUIRenderImage *m_pToolTipLeft;
	ioUIRenderImage *m_pToolTipRight;

	ioHashString m_szName;
	
	int m_iMaxGauge;
	int m_iCurGauge;
	
	DWORD m_dwScaleStartTime;
	float m_fCurScaleRate;

protected:
	void RenderToolTip();
	void ProcessScale();

	int  GetRenderImageStartHeight( ioUIRenderImage *pImage, int iSrcHeight, int iCur, int iMax );
	void RenderGaugeIcon( int xPos, int yPos, ioUIRenderImage *pImage, int iSrcHeight, int iCur, int iMax , TextureFilterOption eOption = TFO_NEAREST );

public:
	void SetIcon( const ioHashString &rszName, int iMaxGauge );
	void SetCurGauge(int iCurGauge) { m_iCurGauge = iCurGauge; }

	const ioHashString &GetName() const { return m_szName; }

public:
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnDrawNormal( int iXPos, int iYPos );
	virtual void OnDrawOvered( int iXPos, int iYPos );
	virtual void OnDrawPushed( int iXPos, int iYPos );

public:
	ConnectionTimeSelectGiftEventBtn();
	virtual ~ConnectionTimeSelectGiftEventBtn();
};
//-----------------------------------------------
class ConnectionTimeSelectGiftEventWnd : public ioMovingWnd
{
public:
	enum
	{
		ID_EXIT_BTN     = 1,
		ID_ICON_1_BTN   = 2,
		ID_ICON_2_BTN   = 3,
	};

	enum 
	{
		MAX_ICON_BTN   = 2,
	};

protected:
	static bool m_bProcess;
	DWORD       m_dwCheckChanceTime;

	DWORD       m_dwSelectID;

protected:
	void ProcessChanceRequest(); 
	void ShowMsgBox( DWORD dwID );
	void SendRequestPresent();

public:
	virtual void iwm_show();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );
	virtual void iwm_create();

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

public:
	ConnectionTimeSelectGiftEventWnd();
	virtual ~ConnectionTimeSelectGiftEventWnd();
};



