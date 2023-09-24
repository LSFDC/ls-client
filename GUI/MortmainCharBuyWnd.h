#pragma once

#include "../io3DEngine/ioButton.h"

class CharBuyInfoBtn : public ioButton
{
protected:
	ioHashString m_szCharName;
	int          m_iPeso;

public:
	void SetInfo( int iSetIdx );

protected:
	virtual void OnRender();

public:
	CharBuyInfoBtn();
	virtual ~CharBuyInfoBtn();
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CharBuyInfoListWnd : public ioWnd
{
public:
	enum 
	{
		ID_LIST_1  = 101,
		ID_LIST_2  = 102,
		ID_LIST_3  = 103,
		ID_LIST_4  = 104,
		ID_LIST_5  = 105,
		ID_LIST_6  = 106,
		ID_LIST_7  = 107,
		ID_LIST_8  = 108,
		ID_LIST_9  = 109,
		ID_LIST_10 = 110,

		ID_VERT_SCROLL = 1001,
	};

	enum
	{
		PAGE_PER_BTN_NUM = 10,
	};

protected:
	int          m_iCurPage;
	
	void UpdatePage( int iNewPage );
	void SetParentInfo( DWORD dwID );

public:
	virtual void iwm_show();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );
	virtual void iwm_wheel( int zDelta );
	virtual void iwm_vscroll(DWORD dwID, int curPos );

protected:
	virtual void OnProcess( float fTimePerSec );

public:
	CharBuyInfoListWnd();
	virtual ~CharBuyInfoListWnd();
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MortmainCharBuyWnd : public ioWnd
{
public: 
	enum 
	{
		ID_EXIT_BTN        = 1,
		ID_CHANGE_CHAR_BTN = 2,
		ID_BUY_BTN         = 3,

		ID_CHAR_LIST_WND   = 101,
	};

protected:
	int  m_iSetIdx;

	void OnMortMainCharBuy();
	void SetCharBtnTitle( int iSetIdx );

public:
	void SetInfo( int iSetIdx );

public:
	virtual void iwm_show();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );
	virtual void SetPositionToDefault( int iParentWidth,
						        	   int iParentHeight,
		                               bool bChildMove,
		                               bool bCreate = false );

protected:
	virtual void OnRender();
	
public:
	MortmainCharBuyWnd(void);
	virtual ~MortmainCharBuyWnd(void);
};

