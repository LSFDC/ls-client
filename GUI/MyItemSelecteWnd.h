#pragma once

class ioUIRenderImage;
class ioUIRenderFrame;

class MyItemSelectRadioButton : public ioRadioButton 
{
protected:
	int	m_iTabType;
public:
	virtual void ParseExtraInfo( ioXMLElement &xElement );
	const int& GetTabType(){ return m_iTabType; }
public:
	MyItemSelectRadioButton();
	virtual ~MyItemSelectRadioButton();
};

//////////////////////////////////////////////////////////////////////////

class ItemSelectInfoWnd : public ioButton
{
public:
	enum 
	{
		ID_ICON_BTN			= 1,
		ID_SELECT_BTN		= 2,
	};

	enum
	{
		MAX_DESC	= 3,
	};

protected:
	bool m_bWidthCutDescArray[MAX_DESC];
	ioComplexStringPrinter m_szDescArray[MAX_DESC];
	EXTRAITEMSLOT m_ItemInfo;
	ioUIRenderImage *m_pEmptyImg;

protected:	
	void ClearDesc();
	void RenderDesc( int iXPos, int iYPos );

	bool SetInfo();

public:
	void SetBlank();
	void SetItemInfo( const EXTRAITEMSLOT& rkItem );

protected:
	virtual void OnRender();

public:
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );
	
public:
	ItemSelectInfoWnd();
	virtual ~ItemSelectInfoWnd();
};

//////////////////////////////////////////////////////////////////////////

class MyItemSelecteWnd :public ioWnd
{
public:
	enum
	{
		ID_EXIT_BTN			= 1,
		ID_FIND_EDIT        = 2,
		ID_FIND_BTN         = 3,

		ID_ITEM_INFO_WND_1_BTN	= 201,
		ID_ITEM_INFO_WND_2_BTN	= 202,
		ID_ITEM_INFO_WND_3_BTN	= 203,
		ID_ITEM_INFO_WND_4_BTN	= 204,
		ID_ITEM_INFO_WND_5_BTN	= 205,
		ID_ITEM_INFO_WND_6_BTN	= 206,
		ID_ITEM_INFO_WND_7_BTN	= 207,
		ID_ITEM_INFO_WND_8_BTN	= 208,

		ID_VERT_SCROLL			= 1001,
	};

	enum
	{
		ID_TAB_1_BTN	 = 101, // all
		ID_TAB_2_BTN	 = 102, //
		ID_TAB_3_BTN	 = 103, //
		ID_TAB_4_BTN	 = 104, //
		ID_TAB_5_BTN	 = 105, //
		ID_TAB_COUNT_MAX = 5,
	};

	enum 
	{
		MAX_TAB_BTN				= 5,

		MAX_ITEM_INFO_WND_BTN	= 8,
		PAGE_PER_ITEM_NUM		= 4,
	};

protected:
	int m_iCurPage;

	DWORD m_dwCurTabID;

	ioUIRenderFrame *m_pEditFrm;
	ioHashString m_szCurrFindKeyword;

	ExtraItemList m_vAllItemList;
	ExtraItemList m_vCurItemList;

	ioWnd*			m_pPreSelectedWnd;
public:
	void ShowItemSeletWnd( ExtraItemList vExtraitemList, ioWnd* pPreSelectedWnd );
public:
	virtual void iwm_vscroll(DWORD dwID, int curPos );
	virtual void iwm_wheel( int zDelta );
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );
protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();
	virtual void AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame );
protected:
	void ClearAll();
	void ChangeTab( DWORD dwID, const char *szFind );
	void SetCurItemInfoVec( DWORD dwTabID, const char *szFind );
	void SetFind();
	bool IsRightTab( DWORD dwTabID, int iType );
	void UpdatePage( int iNewPage );
public:
	MyItemSelecteWnd(void);
	virtual ~MyItemSelecteWnd(void);
};

