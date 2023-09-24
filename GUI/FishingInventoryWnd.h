#pragma once

#include "../io3DEngine/ioMovingWnd.h"

class FishingItemInfoWnd : public ioButton
{
protected:
	int m_iItemType;
	int m_iSlotNum;

	ioUIRenderImage *m_pBack;
	ioUIRenderImage *m_pIcon;

public:
	void SetInfo( int iItemType, int iSlotNum );

protected:
	virtual void OnRender();

public:
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );

public:
	FishingItemInfoWnd();
	virtual ~FishingItemInfoWnd();
};
/////////////////////////////////////////////////////////////////////////////////////////////////////
class FishingItemPageBtn : public ioButton
{
protected:
	ioUIRenderImage *m_pActiveImage;
	ioUIRenderImage *m_pInActiveImage;

protected:
	virtual void OnRender();

public:
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );

public:
	FishingItemPageBtn();
	virtual ~FishingItemPageBtn();
};
/////////////////////////////////////////////////////////////////////////////////////////////////////
class FishingScreenBtn : public ioButton
{
protected:
	bool m_bScreen;
	bool m_bFishing;
	DWORD m_dwScreenStartTime;
	DWORD m_dwScreenTimer;

protected:
	void ProcessScreenEffect();

public:
	void SetFishingState( bool bFishing );

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnDrawNormal( int iXPos, int iYPos );

public:
	virtual void ParseExtraInfo( ioXMLElement &xElement );

public:
	FishingScreenBtn();
	virtual ~FishingScreenBtn();
};
/////////////////////////////////////////////////////////////////////////////////////////////////////
class FishingExpWnd : public ioWnd
{
protected:
	ioUIRenderImage *m_pExpGauge;

	int m_iCurLevel;
	int m_iCurExp;

public:
	void SetInfo( int iLevel, int iExp );

protected:
	virtual void OnRender();

public:
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );

public:
	FishingExpWnd();
	virtual ~FishingExpWnd();
};
////////////////////////////////////////////////////////////////////////////////////////////////////
class FishingBaitListWnd : public ioWnd
{
protected:
	ioUIRenderImage *m_pDarkLine;
	ioUIRenderImage *m_pLightLine;
	ioUIRenderImage *m_pOverLine;

	ioWnd *m_pOpenBtn;
	int   m_iAddWidth;
	int   m_iAddHeight;
	int   m_iListXPos;
	int   m_iListYPos;
	int   m_iHeightGap;
	int   m_iStartXPos;
	int   m_iStartYPos;

	int   m_iOverListIndex;

protected:
	struct BaitList
	{
		int			 m_iIndex;
		ioHashString m_szTitle;
		int          m_iCount;
		BaitList()
		{
			m_iIndex = -1;
			m_iCount = 0;
		}
	};
	typedef std::vector< BaitList > vBaitList;
	vBaitList m_vBaitList;

protected:
	void CheckBaitList();
	void SetParentInfo( int iType );

public:
	void OpenList( ioWnd *pOpenBtn );
	bool IsEmptyList();

public:
	virtual void iwm_show();
	virtual void iwm_hide();

public:
	virtual void ParseExtraInfo( ioXMLElement &xElement );
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

public:
	FishingBaitListWnd();
	virtual ~FishingBaitListWnd();
};
//////////////////////////////////////////////////////////////////////////////////////////////////
class FishingInventoryWnd : public ioMovingWnd
{
public:
	enum
	{
		ID_ITEM1_BTN	= 1,
		ID_ITEM2_BTN	= 2,
		ID_ITEM3_BTN	= 3,
		ID_ITEM4_BTN	= 4,
		ID_ITEM5_BTN	= 5,
		ID_ITEM6_BTN	= 6,
		ID_ITEM7_BTN	= 7,
		ID_ITEM8_BTN	= 8,
		ID_ITEM9_BTN	= 9,
		ID_ITEM10_BTN	= 10,

		ID_ITEM_SELL_BTN	= 101,
		ID_ITEM_FISHING_BTN	= 102,
		ID_EXIT				= 103,

		ID_PAGE_UP			= 201,
		ID_PAGE_DOWN		= 202,

		ID_FISHING_EXP_WND	= 301,
		
		ID_BAIT_SELECT_BTN	= 401,
		ID_BAIT_LIST		= 402,

		ID_TOOL_TIP			= 501,

		MAX_BTN = 10,
	};

	enum
	{
		NONE_TYPE	= 1,
		ADD_TYPE	= 2,
		SELL_TYPE	= 3,
	};

protected:
	enum 
	{
		POS_SAVE_VERSION = 4,
	};

	ioHashString m_BaitCntText;
	int m_iBaitCnt;

	ioPlayStage* m_pPlayStage;

	int m_iDefaultX;
	int m_iDefaultY;

	int m_iCurPageCnt;
	int m_iCurPage;

protected:
	void UpdateBtns( int iArray , DWORD dwBtnID );
	void UpdateExp();
	void OnSetIconPush( DWORD dwID, bool bNextSel, int iStartArray );

	void CheckNextItem( int iSellItemArray );
	void CheckFishingBtn();
	void CheckMessageBox( int iStartArray );

	void SendItemSell( int iSellItemArray );

	void UpdateDefaultPos();

	void ProcessToolTip();

public:
	void SetPlayStage( ioPlayStage *pPlayStage );

	void CheckPageData( bool bFirst );

	void UpdateItemData( int iType, ioHashString szIcon = "", ioHashString szGrade = "" );
	void UpdateBait();
	void UpdatePage();

	bool IsMaxSlot();

	void SetBaitType( int iType );

public:
	virtual void iwm_create();
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );
	virtual void SetPositionToDefault( int iParentWidth, int iParentHeight, bool bChildMove, bool bCreate = false );

protected:
	virtual void OnRender();
	virtual void OnProcess( float fTimePerSec );

public:
	FishingInventoryWnd(void);
	virtual ~FishingInventoryWnd(void);
};

