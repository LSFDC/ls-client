#pragma once

#include "../ioQuest.h"
#include "../io3DEngine/ioStringConverter.h"
#include "../io3DEngine/ioScroll.h"
#include "../io3DEngine/ioMovingWnd.h"

#include "../ioComplexStringPrinter.h"
#include "FSMStateMgr.h"


class QuestParent;
class ioUIRenderFrame;
class ioUIRenderImage;
class QuestListBtn : public ioButton
{
public:
	enum
	{
		ID_QUEST_CONTENT_BTN  = 1,
		ID_QUEST_RESULT_BTN   = 2,
		ID_QUEST_INSERT_ALARM = 3,
		ID_QUEST_REMOVE_ALARM = 4,
	};

protected:
	ioUIRenderFrame *m_pRewardFrm;

	ioUIRenderImage *m_pEmptyQuest;
	ioUIRenderImage *m_pIconBack;
	ioUIRenderImage *m_pQuestIcon;
	ioUIRenderImage *m_pEventMark;
	ioUIRenderImage *m_pCostumeMark;

protected:
	QuestData m_QuestData;
	QuestParent *m_pQuestParent;

protected:
	bool m_bIsCostume;

protected:
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	
	virtual void AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame );

public:
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

protected:
	virtual void OnDrawPushed( int iXPos, int iYPos );

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();
	void OnRenderProgress( int iXPos, int iYPos );
	void OnRenderReward(int iXPos, int iYPos );

public:
	void SetQuest( QuestData &rkQuestData );
	QuestParent *GetQuestParent(){ return m_pQuestParent; }

public:
	QuestListBtn();
	virtual ~QuestListBtn();
};
//////////////////////////////////////////////////////////////////////////
class QuestCompleteListWnd : public ioWnd
{
public:
	enum
	{
		ID_LEFT_BTN			= 1,
		ID_RIGHT_BTN		= 2,

		ID_COMPLETE_LIST1	= 100,
		ID_COMPLETE_LIST2	= 101,
		ID_COMPLETE_LIST3	= 102,
		ID_COMPLETE_LIST4	= 103,
		ID_COMPLETE_LIST5	= 104,
		ID_COMPLETE_LIST6	= 105,
		ID_COMPLETE_LIST7	= 106,
		ID_COMPLETE_LIST8	= 107,
	};

	enum
	{
		PAGE_QUEST_COUNT = 8,
	};

protected:
	int m_iCurPage;
	int m_iMaxPage;

public:
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

protected:
	virtual void OnRender();

public:
	void ChangeData();
	void UpdatePage( int iCurPage );

public:
	QuestCompleteListWnd();
	virtual ~QuestCompleteListWnd();
};
//////////////////////////////////////////////////////////////////////////
class QuestProgressListWnd : public ioWnd
{
public:
	enum
	{
		ID_LEFT_BTN			= 1,
		ID_RIGHT_BTN		= 2,

		ID_PROGRESS_LIST1	= 100,
		ID_PROGRESS_LIST2	= 101,
		ID_PROGRESS_LIST3	= 102,
		ID_PROGRESS_LIST4	= 103,
		ID_PROGRESS_LIST5	= 104,
		ID_PROGRESS_LIST6	= 105,
		ID_PROGRESS_LIST7	= 106,
		ID_PROGRESS_LIST8	= 107,
	};

	enum
	{
		PAGE_QUEST_COUNT = 8,
	};

protected:
	int m_iCurPage;
	int m_iMaxPage;

public:
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

protected:
	virtual void OnRender();

public:
	void ChangeData();
	void UpdatePage( int iCurPage );

public:
	QuestProgressListWnd();
	virtual ~QuestProgressListWnd();
};
//////////////////////////////////////////////////////////////////////////
class QuestMainWnd : public ioMovingWnd
{
public:
	enum
	{
		ID_EXIT = 1,		
		ID_PROGRESS_BTN = 2,
		ID_COMPLETE_BTN = 3,

		ID_PROGRESS_WND = 10,
		ID_COMPLETE_WND = 11,
	};

protected:
	DWORD m_dwTabID;

protected:
	void ChangeRadioWnd( DWORD dwID );

protected:
	void ChangeData();
	void ChangeTitle();

public:
	virtual void iwm_create();
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

public:
	void QuestUpdateData();

public:
	QuestMainWnd();
	virtual ~QuestMainWnd();
};
//////////////////////////////////////////////////////////////////////////
class LobbyQuestAlarmBtn : public ioButton
{
public:
	enum
	{
		ID_DELETE_BTN = 1,
	};

protected:
	ioUIRenderImage *m_pIconBack;
	ioUIRenderImage *m_pQuestIcon;
	ioUIRenderImage *m_pIconEmpty;
	ioUIRenderImage *m_pEventMark;
	ioUIRenderImage *m_pCostumeMark;

protected:
	QuestData m_QuestData;
	QuestParent *m_pQuestParent;

protected:
	bool m_bIsCostume;

protected:
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	

public:
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

protected:
	virtual void OnRender();
	void OnRenderQuestTitle( int iXPos, int iYPos, const char *szResult );

public:
	void SetQuest( QuestData &rkQuestData );
	QuestParent *GetQuestParent(){ return m_pQuestParent; }

public:
	LobbyQuestAlarmBtn();
	virtual ~LobbyQuestAlarmBtn();
};
//////////////////////////////////////////////////////////////////////////
class LobbyQuestAlarmWnd : public ioWnd
{
public:
	enum
	{
		ID_QUEST_BTN = 1,

		ID_ALARM_BTN1 = 10,
		ID_ALARM_BTN2 = 11,
		ID_ALARM_BTN3 = 12,
		ID_ALARM_BTN4 = 13,
	};

	enum
	{
		MAX_ALARM = 4,
	};

public:
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

protected:
	virtual void OnProcess( float fTimePerSec );

public:
	void UpdateAlarm();

public:
	LobbyQuestAlarmWnd();
	virtual ~LobbyQuestAlarmWnd();
};
//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/* 퀘스트 수락 / 보상받기 / 정보창에서 중복적으로 사용할 내용을 한군데	*/
/* 모아 두었다. 여기서는 어떤 명령도 내리지 않고 그리기만 한다.			*/
/************************************************************************/
class QuestInfoParentWnd : public ioWnd
{
public:
	enum
	{
		MAX_HELP_LINE = 9,
		MAX_REWARD    = 5,
	};

protected:
	ioUIRenderFrame *m_pRewardOnFrm;
	ioUIRenderFrame *m_pRewardOffFrm;
	ioUIRenderImage *m_pRewardOffX;
	ioUIRenderImage *m_pStateGreenNew;
	ioUIRenderImage *m_pStateEventMark;
	ioUIRenderImage *m_pQuestIconBack;
	ioUIRenderImage *m_pRewardIconBack;
	ioUIRenderImage *m_pCostumeMark;

protected:
	int m_iCurPos;

protected:
	float m_fCurWidth;
	float m_fMaxWidth;

protected:
	typedef std::map< ioHashString, ioUIRenderImage * > QuestRewardIconMap; 
	QuestRewardIconMap m_QuestRewardIconMap;
	QuestParent *m_pQuestParent;
	DWORD        m_dwQuestState;

protected:
	void ClearRewardIconMap();
	ioUIRenderImage *GetRewardIcon( int iPresentType, int iPresentValue1, int iPresentValue2 );

protected:
	virtual void AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame );
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();
	virtual void OnRenderIcon( int iXPos, int iYPos );
	virtual void OnRenderReward( int iXPos, int iYPos, int iWhiteAlphaRate = 0 );	

public:
	QuestInfoParentWnd();
	virtual ~QuestInfoParentWnd();
};
//////////////////////////////////////////////////////////////////////////
class QuestOccurStateWnd : public QuestInfoParentWnd
{
public:
	enum
	{
		ID_AGREE_BTN = 1,
		ID_CLOSE_BTN = 2,

		ID_VERT_SCROLL  = 1000,
	};

	enum
	{
		MINIMUM_W = 435,
		MINIMUM_H = 336,
		MAXIMUM_W = 435,
		MAXIMUM_H = 513,

		BOTTOM_Y  = 271,
		BTN_BOTTOM= 51,
		PURPOSE_GAP = 18,
		REWARD_PASS_H = 140,

		REWARD_SELECT_DESC_WND = 7000,
	};

protected:
	ioUIRenderFrame *m_pBottomFrm;
	ioUIRenderFrame *m_pTextBox;
	ioUIRenderFrame *m_pStateAimFrm;

	ioWnd* m_pRewardSelectDescWnd;

	float			 m_fTextPrintSpeed;
	float			 m_fTextPrintSpeed_Scroll;

protected:
	bool  m_bMaximumWnd;
	DWORD m_dwMaximumAniTime;
	int   m_iMinusRewardPassH;
	int   m_iAddMaxHeight;
	DWORD m_dwTextTickTime;
	DWORD m_dwTextTickCheckTime;
	DWORD m_dwCurrentScrollTime;
	DWORD m_dwCurrentScrollStartTime;
	DWORD m_dwScrollTickCheckTime;

	int   m_iWhiteAlphaRate;
	DWORD m_dwWhiteAlphaTime;

protected:
	ioHashString m_szShowSnd;
	ioHashString m_szTickSnd;
	ioHashString m_szHideSnd;
	ioHashString m_szAgreeSnd;
	
public:
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual void iwm_wheel( int zDelta );
	virtual void iwm_vscroll( DWORD dwID, int curPos );
	virtual bool iwm_spacebar();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

protected:
	virtual void OnProcess( float fTimePerSec );

protected:
	virtual void AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame );
	virtual void OnRender();

public:
	virtual bool OnRenderShowAndHide();

protected:
	void ReSize( bool bMax );
	void ReSizeWnd( int iWidth, int iHeight );

	bool IsRewardSelectStyle();

public:
	virtual void SetPositionToDefault( int iParentWidth, int iParentHeight, bool bChildMove, bool bCreate = false );
	virtual void ParseExtraInfo( ioXMLElement &xElement );

public:
	QuestOccurStateWnd();
	virtual ~QuestOccurStateWnd();
};
//////////////////////////////////////////////////////////////////////////
// QuestOccurState는 소스만 2개로 분리된다. Brown과 Pink 색으로 두가지 
class QuestOccurStateMainWnd : public ioMovingWnd
{
public:
	enum
	{
		ID_BROWN_WND = 1,
		ID_PINK_WND  = 2,
	};

protected:
	QuestParent *m_pQuestParent;

protected:
	DWORD m_dwHideDelayTime;

public:
	virtual void iwm_create();
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual void iwm_wheel( int zDelta );
	virtual bool iwm_spacebar();

protected:
	bool IsShowCheck();
	virtual void OnProcess( float fTimePerSec );
	virtual bool OnRenderShowAndHide();

public:
	virtual void SetPositionToDefault( int iParentWidth, int iParentHeight, bool bChildMove, bool bCreate = false );

public:
	QuestOccurStateMainWnd();
	virtual ~QuestOccurStateMainWnd();
};
//////////////////////////////////////////////////////////////////////////
class QuestAttainStateWnd : public ioWnd
{
public:
	enum
	{
		ID_REWARD_BTN	= 1,

		ID_ALPHA_WND    = 100,
	};

	enum
	{
		WND_HEIGHT_A = 145,
		WND_HEIGHT_B = 23,
		WND_HEIGHT_C = 58,
		HELP_MENT_GAP= 19,
		REWARD_GAP   = 45,
	};

	enum
	{
		STATE_NONE  = 0,
		STATE_SCALE = 1,
		STATE_ALPHA = 2,
	};
	
protected:
	ioUIRenderFrame *m_pBrownBackFrm;
	ioUIRenderFrame *m_pBrownTitleFrm;
	ioUIRenderFrame *m_pBrownBottomFrm;
	ioUIRenderFrame *m_pBrownRewardOnFrm;

	ioUIRenderFrame *m_pPinkBackFrm;
	ioUIRenderFrame *m_pPinkTitleFrm;
	ioUIRenderFrame *m_pPinkBottomFrm;
	ioUIRenderFrame *m_pPinkRewardOnFrm;
	
	ioUIRenderImage *m_pQuestIconBack1;
	ioUIRenderImage *m_pQuestIconBack2;
	ioUIRenderImage *m_pCompleteText;

	ioUIRenderFrame *m_pCurrentBackFrm;
	ioUIRenderFrame *m_pCurrentTitleFrm;
	ioUIRenderFrame *m_pCurrentBottomFrm;
	ioUIRenderFrame *m_pCurrentRewardOnFrm;
	ioUIRenderImage *m_pCostumeMark;

protected:
	typedef std::map< ioHashString, ioUIRenderImage * > QuestRewardIconMap; 
	QuestRewardIconMap m_QuestRewardIconMap;
	QuestParent *m_pQuestParent;

protected:
	DWORD m_dwHideDelayTime;

protected:
	DWORD m_dwState;
	DWORD m_dwCompleteTime;
	int   m_iQuestIconWhiteAlpha;

protected:
	ioHashString m_szShowSnd;
	ioHashString m_szRewardSnd;

protected:
	int m_iHelpMentSize;
	int m_iRewardSize;

public:
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual bool iwm_spacebar();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

protected:
	bool IsShowCheck();
	void ProcessComplete();
	virtual void OnProcess( float fTimePerSec );
	virtual bool OnRenderShowAndHide();

protected:
	void ClearRewardIconMap();
	ioUIRenderImage *GetRewardIcon( int iPresentType, int iPresentValue1, int iPresentValue2 );

protected:
	virtual void AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame );
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	
	virtual void OnRender();
	virtual void OnRenderIcon( int iXPos, int iYPos );
	virtual void OnRenderReward( int iXPos, int iYPos );	

public:
	void ReSizeWnd();
	virtual void SetPositionToDefault( int iParentWidth, int iParentHeight, bool bChildMove, bool bCreate = false );
	virtual void ParseExtraInfo( ioXMLElement &xElement );

public:
	QuestAttainStateWnd();
	virtual ~QuestAttainStateWnd();
};

//////////////////////////////////////////////////////////////////////////
#include "ioRenderElementAnimation.h"
#include "ioRenderElementAnimationFrame.h"
class QuestAttainSelectStateWnd : public ioWnd
{
	enum // 자식들 ID 맵핑
	{
		BACKGROUND_IMAGE = 0,
		TITLE_BAR,
		SUN_IMAGE,
		QUEST_ICON,
		COMPLETE_TEXT,
		ALPHA_EFFECT,

		// 높이 효과 받는 애들
		BOTTOM_MULTY,
		REWARD_DESC_BACK,

		REWARD_WND_0,
		REWARD_WND_1,
		REWARD_WND_2,
		REWARD_WND_3,
		REWARD_WND_4,

		REWARD_CHECK_0,
		REWARD_CHECK_1,
		REWARD_CHECK_2,
		REWARD_CHECK_3,
		REWARD_CHECK_4,

		SPACE_BUTTON,		

		CHILD_MAX,
	};

	struct QuestRewardInfo
	{
		QuestRewardInfo( ioUIRenderImage* icon, ioWnd* rwnd, const std::string& text, bool bCostume )
			: m_icon( icon )
			, m_rewardwnd( rwnd )
			, m_text( text )
			, m_bIsCostume ( bCostume )
		{}

		~QuestRewardInfo()
		{
			SAFEDELETE( m_icon );
		}

		ioUIRenderImage*		m_icon;
		ioWnd*					m_rewardwnd;
		std::string				m_text;
		bool					m_bIsCostume;
	};
	typedef std::vector< QuestRewardInfo* > QuestRewardVec; 	

public:

	//----------------------------------------------------------------
	/*
	생성, 파괴
	*/
	QuestAttainSelectStateWnd();
	virtual ~QuestAttainSelectStateWnd();

	//----------------------------------------------------------------
	/*
	ioWnd override
	*/
	// iwm
	virtual void				AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );
	virtual void				iwm_show();
	virtual void				iwm_hide();
	virtual bool				iwm_spacebar();
	virtual void				iwm_command( ioWnd *pWnd, int cmd, DWORD param );	
	// process
	virtual void				OnProcess( float fTimePerSec );
	// render
	virtual bool				OnRenderShowAndHide();
	virtual void				OnRenderAfterChild();
	// etc
	virtual void				SetPositionToDefault( int iParentWidth, int iParentHeight, bool bChildMove, bool bCreate );
	virtual bool				IsShowCheck();
	virtual void				ParseExtraInfo( ioXMLElement &xElement );

	//----------------------------------------------------------------
	/*
	ProcessCompleteTextAnimation : Complete이미지 글씨 효과를 처리하는 부분 ( 후후...;;; )
	*/
	void						ProcessCompleteTextAnimation();

protected:

	//----------------------------------------------------------------
	/*
	ETC
	*/
	void						Resize();
	void						ProcessAutoShowHide( float fTimePerSec );
	void						RenderReward();

	void						CheckButton( ioWnd *pWnd, int cmd );
	//----------------------------------------------------------------
	/*
	Reward
	*/
	void						DeleteRewardInfos();
	void						FillRewardInfos();
	static ioUIRenderImage*		GetRewardIcon( int iPresentType, int iPresentValue1, int iPresentValue2 );	

protected:

	QuestParent*				m_pQuestParent;

	std::string					m_questtitle;

	int							m_sunwndposx;
	int							m_sunwndposy;

	DWORD						m_hidedelaytime;

	ioRenderElementAnimation	m_animatorscale;
	ioRenderElementAnimation	m_animate_showscaler; // show 할때 배경이미지 scaler
	ioRenderElementAnimation	m_animate_hidescaler; // hide 할때 배경이미지 scaler

	ioHashString				m_showsoundname;
	ioHashString				m_rewardsoundname;

	QuestRewardVec				m_rewardinfos;

	int							m_rewardselectnum;
	ioUIRenderImage*			m_pCostumeMark;

	DWORDVec					m_RewardIDList;
};

//////////////////////////////////////////////////////////////////////////
class QuestProgressInfoWnd : public QuestInfoParentWnd
{
public:
	enum
	{
		ID_EXIT = 1,
		ID_INSERT_ALARM_BTN = 2,
		ID_DELETE_ALARM_BTN = 3,

		ID_VERT_SCROLL = 1000,
	};

	enum
	{
		MAXIMUM_H   = 471,
		BOTTOM_Y    = 271,
		PURPOSE_GAP = 18,
		MAX_ALARM   = 4,
		REWARD_PASS_H = 140,
	};

protected:
	ioUIRenderFrame *m_pBottomFrm;
	ioUIRenderFrame *m_pStatePurposeFrm;

protected:
	int   m_iMinusRewardPassH;
	int   m_iAddMaxHeight;

protected:
	void ReSizeWnd( int iWidth, int iHeight );

protected:
	virtual void AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame );

public:
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );
	virtual void iwm_wheel( int zDelta );
	virtual void iwm_vscroll( DWORD dwID, int curPos );

protected:
	//virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

public:
	virtual void SetPositionToDefault( int iParentWidth, int iParentHeight, bool bChildMove, bool bCreate = false );
	virtual bool OnRenderShowAndHide();

public:
	void SetQuest( DWORD dwMainIndex, DWORD dwSubIndex );
	void SetQuest( QuestParent *pQuestParent );

public:
	QuestProgressInfoWnd();
	virtual ~QuestProgressInfoWnd();
};
//////////////////////////////////////////////////////////////////////////
// QuestProgressInfoWnd는 소스만 2개로 분리된다. Brown과 Pink 색으로 두가지 
class QuestProgressInfoMainWnd : public ioMovingWnd
{
public:
	enum
	{
		ID_BROWN_WND = 1,
		ID_PINK_WND  = 2,
	};

protected:
	QuestParent		*m_pQuestParent;

protected:
	virtual void SetPositionToDefault( int iParentWidth, int iParentHeight, bool bChildMove, bool bCreate = false );

public:
	virtual void iwm_create();
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual bool iwm_esc();
	virtual void iwm_wheel( int zDelta );

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual bool OnRenderShowAndHide();

public:
	void SetQuest( DWORD dwMainIndex, DWORD dwSubIndex );
	void SetQuest( QuestParent *pQuestParent );

public:
	QuestProgressInfoMainWnd();
	virtual ~QuestProgressInfoMainWnd();
};
//////////////////////////////////////////////////////////////////////////
class QuestCompleteInfoWnd : public ioMovingWnd
{
public:
	enum
	{
		ID_BROWN_EXIT	= 1,
		ID_PINK_EXIT	= 2,
	};

	enum
	{
		WND_HEIGHT_A = 145,
		WND_HEIGHT_B = 23,
		WND_HEIGHT_C = 58,
		HELP_MENT_GAP= 19,
		REWARD_GAP   = 45,
	};

protected:
	ioUIRenderFrame *m_pBrownBackFrm;
	ioUIRenderFrame *m_pBrownTitleFrm;
	ioUIRenderFrame *m_pBrownBottomFrm;
	ioUIRenderFrame *m_pBrownRewardOnFrm;

	ioUIRenderFrame *m_pPinkBackFrm;
	ioUIRenderFrame *m_pPinkTitleFrm;
	ioUIRenderFrame *m_pPinkBottomFrm;
	ioUIRenderFrame *m_pPinkRewardOnFrm;

	ioUIRenderImage *m_pQuestIconBack1;
	ioUIRenderImage *m_pQuestIconBack2;
	ioUIRenderImage *m_pAcquireText;

	ioUIRenderFrame *m_pCurrentBackFrm;
	ioUIRenderFrame *m_pCurrentTitleFrm;
	ioUIRenderFrame *m_pCurrentBottomFrm;
	ioUIRenderFrame *m_pCurrentRewardOnFrm;
	ioUIRenderImage *m_pCostumeMark;

protected:
	typedef std::map< ioHashString, ioUIRenderImage * > QuestRewardIconMap; 
	QuestRewardIconMap m_QuestRewardIconMap;
	QuestParent *m_pQuestParent;

protected:
	DWORD m_dwHideDelayTime;
	DWORD m_dwQuestState;

protected:
	int m_iHelpMentSize;
	int m_iRewardSize;

public:
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

protected:
	virtual void OnProcess( float fTimePerSec );

protected:
	void ClearRewardIconMap();
	ioUIRenderImage *GetRewardIcon( int iPresentType, int iPresentValue1, int iPresentValue2 );

protected:
	virtual void AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame );
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	
	virtual void OnRender();
	virtual void OnRenderIcon( int iXPos, int iYPos );
	virtual void OnRenderReward( int iXPos, int iYPos );	
	virtual bool OnRenderShowAndHide();

public:
	void ReSizeWnd();
	virtual void SetPositionToDefault( int iParentWidth, int iParentHeight, bool bChildMove, bool bCreate = false );

public:
	void SetQuest( QuestParent *pQuestParent );

public:
	QuestCompleteInfoWnd();
	virtual ~QuestCompleteInfoWnd();
};
//////////////////////////////////////////////////////////////////////////
class QuestDeleteAndFailWnd : public ioWnd
{
public:
	enum
	{
		ID_BROWN_CLOSE = 1,
		ID_PINK_CLOSE  = 2,
	};

protected:
	ioUIRenderFrame *m_pBrownBackFrm;
	ioUIRenderFrame *m_pBrownTitleFrm;
	ioUIRenderFrame *m_pBrownBottomFrm;
	ioUIRenderFrame *m_pBrownRewardOnFrm;

	ioUIRenderFrame *m_pPinkBackFrm;
	ioUIRenderFrame *m_pPinkTitleFrm;
	ioUIRenderFrame *m_pPinkBottomFrm;
	ioUIRenderFrame *m_pPinkRewardOnFrm;

	ioUIRenderImage *m_pQuestIconBack1;
	ioUIRenderImage *m_pQuestIconBack2;
	ioUIRenderImage *m_pFinishText;
	ioUIRenderImage *m_pFailText;

	ioUIRenderFrame *m_pCurrentBackFrm;
	ioUIRenderFrame *m_pCurrentTitleFrm;
	ioUIRenderFrame *m_pCurrentBottomFrm;
	ioUIRenderFrame *m_pCurrentRewardOnFrm;
	ioUIRenderImage *m_pCostumeMark;

protected:
	DWORD m_dwHideDelayTime;

protected:
	bool m_bDeleteQuest;
	QuestParent *m_pQuestParent;

protected:
	bool IsShowCheck();

public:
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

protected:
	virtual void AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame );
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();
	virtual bool OnRenderShowAndHide();

public:
	QuestDeleteAndFailWnd();
	virtual ~QuestDeleteAndFailWnd();
};
//////////////////////////////////////////////////////////////////////////
class GameQuestAlarmBtn : public ioButton
{
protected:
	virtual void OnRender();
	
public:
	GameQuestAlarmBtn();
	virtual ~GameQuestAlarmBtn();
};
//////////////////////////////////////////////////////////////////////////
class GameQuestAlarmScroll : public ioScroll
{
protected:
	ioUIRenderFrame *m_pSlideBack;

public:
	void ReSizeScroll( int iWidth, int iHeight );
	
protected:
	virtual void AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame );

public:
	virtual void SetPositionToDefault( int iParentWidth, int iParentHeight, bool bChildMove, bool bCreate = false );

protected:
	virtual void OnRender();

public:
	GameQuestAlarmScroll();
	virtual ~GameQuestAlarmScroll();
};
//////////////////////////////////////////////////////////////////////////
class GameQuestAlarmWnd : public ioMovingWnd
{
public:
	enum
	{
		ID_VERT_SCROLL  = 1000,
	};

	enum
	{
		POS_SAVE_VERSION = 5,

		ICON_X = 25,
		ICON_Y = 23,
		ICON_SIZE = 50,

		WND_GAP = 62,
		TEXT_LIMIT_SIZE = 121,

		SCROLL_GAP = 60,
		SCROLL_WIDTH = 13,

		TEXT_X = 162,
		TEXT_W = 163,
		TEXT_Y = 23,
		TEXT_H = 47,
	};

	enum ShowAniType 
	{
		SAT_NONE         = 0,
		SAT_START_ROUND  = 1,
		SAT_DELAY        = 2,
		SAT_UP           = 3,  // 가속
		SAT_DOWN         = 4,  // 감속
		SAT_UP_STOP      = 5,  // 가속
		SAT_STOP         = 6,
		SAT_END          = 7,
	};

protected:
	ioUIRenderFrame *m_pOverBack;
	ioUIRenderImage *m_pIconBack;
	ioUIRenderImage *m_pEventMark;
	ioUIRenderImage *m_pCostumeMark;

protected:
	struct SlideData
	{
		QuestParent *m_pQuestParent;
		DWORD        m_dwTextSlideTime;
		float        m_fTextSlideRate;
		SlideData()
		{
			m_pQuestParent = NULL;
			m_dwTextSlideTime= 0;
			m_fTextSlideRate = 0.0f;
		}
	};
	typedef std::vector< SlideData > vSlideData;
	vSlideData m_vOpenSlide;
	vSlideData m_vCloseSlide;

	typedef std::vector< QuestParent * > vAlarmQuest;
	vAlarmQuest m_vAlarmQuest;

	int m_iDefaultX;
	int m_iDefaultY;
	int m_iBlindOffSetY;
	int m_iPreBlindOffSetY;
	int m_iOverListIndex;
	FSMStateMgr m_ShowAniState;

	ioWnd *m_pScrollWnd;
	int    m_iMouseDownX;
	int    m_iMouseDownY;

	int    m_iLimitAlarmCnt;

protected:
	static int sg_iCurPage;	

public:
	virtual void iwm_create();
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual void iwm_wheel( int zDelta );
	virtual void iwm_vscroll(DWORD dwID, int curPos );

protected:
	virtual void AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame );
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

protected:
	bool IsOverCheck();
	void OnProcessMouse();
	void OnProcessSlide();
	void OnRenderQuestSlide( int iXPos, int iYPos, QuestParent *pQuestParent );
	void OnRenderQuestTitle( int iXPos, int iYPos, QuestParent *pQuestParent, float fSlideRate );

public:
	virtual void SetPositionToDefault( int iParentWidth, int iParentHeight, bool bChildMove, bool bCreate = false );
	virtual void ParseExtraInfo( ioXMLElement &xElement );

protected:
	bool IsShowCheck();
	bool IsShowScroll();
	void InitShowAniState();
	void UpdateAniState();
	bool UpdateShowAni();
	void UpdateDefaultPos();

protected:
	void InsertOpenSlide( QuestParent *pQuestParent );
	void InsertCloseSlide( QuestParent *pQuestParent, DWORD dwSlideTime, float fSlideRate );

public:
	void UpdateSetPage( int iCurPage );

public:
	void ChangeShowAniState( ShowAniType eShowAniType );
	int  GetCurShowAniState();
	void SetBlindOffSet( int iYOffSet ) { m_iBlindOffSetY = iYOffSet; }
	void SetDefaultY(int iDefaultY) { m_iDefaultY = iDefaultY; }

public:
	GameQuestAlarmWnd();
	virtual ~GameQuestAlarmWnd();
};
////////////////////////////////////////////////////////////////////////// 6002
class QuestToolTip : public ioWnd
{
public:
	enum
	{
		ID_COMMAND  = 1,
	};

	enum
	{
		STATE_WIDTH = 40,
	};

protected:
	ioUIRenderFrame *m_pStateRedFrm;
	ioUIRenderImage *m_pTooltipLeft;
	ioUIRenderImage *m_pTooltipLeftIn;
	ioUIRenderImage *m_pTooltipCenter;
	ioUIRenderImage *m_pTooltipRightIn;
	ioUIRenderImage *m_pTooltipRight;

protected:
	QuestParent  *m_pQuestParent;
	ioHashString  m_szFirstContents;
	ioHashString  m_szLastContents;
	DWORD         m_dwShowTime;

public:
	void ShowTooltip( DWORD dwMainIndex, DWORD dwSubIndex );

public:
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

protected:
	virtual void AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame );
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

protected:
	void OnRenderTooltip( int iXPos, int iYPos );

public:
	QuestToolTip();
	virtual ~QuestToolTip();
};
////////////////////////////////////////////////////////////////////////// 1 ~ 1000
class QuestGuideWnd : public ioWnd
{
public:
	enum
	{
		ID_INFO_BTN = 1,
	};

protected:
	QuestParent *m_pQuestParent;
	ioWnd *m_pInfoBtn;

public:
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

public:
	void SetQuestParent( QuestParent *pQuestParent );
	bool IsButtonScreen();

public:
	QuestGuideWnd();
	virtual ~QuestGuideWnd();
};

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
class LobbyQuestNewAlarmWnd : public ioWnd
{
public:
	enum
	{
		ID_PROCESS_BTN	= 1,
		ID_PRE_BTN		= 2,
		ID_NEXT_BTN		= 3,

		ID_ALARM_BTN1	= 10,
		ID_ALARM_BTN2	= 11,
		ID_ALARM_BTN3	= 12,
		ID_ALARM_BTN4	= 13,
		ID_ALARM_BTN5	= 14,
		ID_ALARM_BTN6	= 15,
		ID_ALARM_BTN7	= 16,
	};

	enum
	{
		MAX_ALARM = 6,
	};

protected:
	int						m_iCurPage;
	int						m_iMaxPage;
	ioComplexStringPrinter	m_PagePrinter;
public:
	virtual void iwm_show();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

public:
	void UpdateAlarm();

public:
	LobbyQuestNewAlarmWnd();
	virtual ~LobbyQuestNewAlarmWnd();
};

class LobbyQuestNewAlarmBtn : public ioButton
{
public:
	enum
	{
		ID_DELETE_BTN = 1,
	};

protected:
	ioUIRenderImage *m_pIconBack;
	ioUIRenderImage *m_pQuestIcon;
	ioUIRenderImage *m_pIconEmpty;
	ioUIRenderImage *m_pEventMark;
	ioUIRenderImage *m_pCostumeMark;

	ioUIRenderFrame	*m_pCurFrm;
	ioUIRenderFrame *m_pEventBackFrm;
	ioUIRenderFrame *m_pNormalBackFrm;

	ioUIRenderFrame	*m_pCurDownFrm;
	ioUIRenderFrame *m_pEventDownBackFrm;
	ioUIRenderFrame *m_pNormalDownBackFrm;

protected:
	QuestData m_QuestData;
	QuestParent *m_pQuestParent;

protected:
	bool m_bIsCostume;

protected:
	virtual void	AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	
	virtual void	AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame );
public:
	virtual void	iwm_command( ioWnd *pWnd, int cmd, DWORD param );

protected:
	virtual void	OnRender();
	void			OnRenderQuestTitle( int iXPos, int iYPos, const char *szResult );
	virtual void	OnDrawNormal( int iXPos, int iYPos );
	virtual void	OnDrawOvered( int iXPos, int iYPos );
	virtual void	OnDrawPushed( int iXPos, int iYPos );
public:
	void SetQuest( QuestData &rkQuestData );
	QuestParent *GetQuestParent(){ return m_pQuestParent; }

public:
	LobbyQuestNewAlarmBtn();
	virtual ~LobbyQuestNewAlarmBtn();
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//미션이 생긴이후 추가한 Quest관련 Wnd
//기본적으로 하는 기능은 QuestMainWnd와 같다.
class QuestListWnd : public ioWnd
{
public:
	enum
	{
		ID_PRE_BTN			= 1,
		ID_NEXT_BTN			= 2,
		ID_PROGRESS_BTN		= 3,
		ID_COMPLETE_BTN		= 4,

		ID_QUEST_LIST_BTN1	= 100,
		ID_QUEST_LIST_BTN2	= 101,
		ID_QUEST_LIST_BTN3	= 102,
		ID_QUEST_LIST_BTN4	= 103,
		ID_QUEST_LIST_BTN5	= 104,
	};

	enum
	{
		MAX_BTN				= 5,
	};

protected:
	DWORD m_dwTabID;
	int	  m_iMaxPage;
	int	  m_iCurPage;
	ioComplexStringPrinter	m_PagePrinter;

protected:
	void ChangeRadioWnd( DWORD dwID );
	void ClearData();
public:
	void ChangeData( int iCurPage );
	void ChangeTitle();

public:
	virtual void iwm_create();
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();
public:
	QuestListWnd();
	virtual ~QuestListWnd();
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class NewQuestListBtn : public ioButton
{
public:
	enum
	{
		ID_QUEST_RESULT_BTN   = 1,
		ID_QUEST_INSERT_ALARM = 2,
		ID_QUEST_REMOVE_ALARM = 3,
	};

protected:
	ioUIRenderFrame *m_pRewardFrm;
	ioUIRenderFrame *m_pCurBackFrm;
	ioUIRenderFrame *m_pEventBackFrm;
	ioUIRenderFrame *m_pNormalBackFrm;


	ioUIRenderImage *m_pEmptyQuest;
	ioUIRenderImage *m_pIconBack;
	ioUIRenderImage *m_pQuestIcon;
	ioUIRenderImage *m_pEventMark;
	ioUIRenderImage *m_pCostumeMark;

protected:
	QuestData m_QuestData;
	QuestParent *m_pQuestParent;

protected:
	bool m_bIsCostume;

protected:
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	
	virtual void AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame );

public:
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();
	virtual void OnDrawNormal( int iXPos, int iYPos );
	virtual void OnDrawOvered( int iXPos, int iYPos );
	virtual void OnDrawPushed( int iXPos, int iYPos );

protected:
	void		 OnRenderText( int iXPos, int iYPos );
	void		 OnRenderIcon( int iXPos, int iYPos );

public:
	void SetQuest( QuestData &rkQuestData );
	QuestParent *GetQuestParent(){ return m_pQuestParent; }

public:
	NewQuestListBtn();
	virtual ~NewQuestListBtn();
};
///////////////////////////////////////////////////////////////////////////////////////////////////