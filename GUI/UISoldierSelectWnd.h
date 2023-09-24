#pragma once


#include "../io3DEngine/ioButton.h"

//-------------------------------------------------------------------------
#define UI_SOLDIERSELECT_MORTMAIN_MARK_SACLE 0.80f

class ioShopSoldierDragItem;
class UISoldierBtn : public ioButton
{
public:
	enum
	{
		MAX_SLOT_KEY = 36,
	};

protected:
	ioUIRenderImage *m_pBlueKeyBack;
	ioUIRenderImage *m_pPinkKeyBack;
	ioUIRenderImage *m_pGrayKeyBack;
	ioUIRenderImage *m_pMortmainMark;
	ioUIRenderImage *m_pIconBack;
	ioUIRenderImage *m_pIcon;

	ioShopSoldierDragItem *m_pDragItem;

	int  m_iSoldierArray;
	int  m_iSoldierSlot;
	bool m_bPCRoomOpen;

	bool m_bBtnOver;
	bool m_bBtnDown;

	int m_nGradeType;

public:
	static DWORD    sg_dwDragFocusID;
	static DWORD    sg_dwFocusID;

protected:
	void RenderKeyState( int iXPos, int iYPos );
	void RenderIcon( float fXPos, float fYPos );
	void RenderInfo( float fXPos, float fYPos );

public:
	void SetCharacter( int iArray, int iSlot, bool bPCRoomOpen );
	int GetCharacterArray();
	bool IsSoldierSlot();

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	
	virtual void AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame );

public:
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual void iwm_mouseover( const ioMouse& mouse );
	virtual void iwm_mouseleave( const ioMouse& mouse );
	virtual void iwm_lbuttondown( const ioMouse& mouse );
	virtual void iwm_lbuttonup( const ioMouse& mouse );

protected:
	virtual void OnDrawOvered( int iXPos, int iYPos );
	virtual void OnDrawPushed( int iXPos, int iYPos );
	virtual void OnDrawNormal( int iXPos, int iYPos );

public:
	UISoldierBtn(void);
	virtual ~UISoldierBtn(void);
};
//-------------------------------------------------------------------------
class UISoldierSelectWnd : public ioWnd
{
public:
	enum
	{
		ID_EXIT        = 1,
		ID_VERT_SCROLL = 2,

		// first line
		ID_SOLDIER_BTN1= 3,
		ID_SOLDIER_BTN2= 4,
		ID_SOLDIER_BTN3= 5,
		ID_SOLDIER_BTN4= 6,
		ID_SOLDIER_BTN5= 7,
		ID_SOLDIER_BTN6= 8,

		// second line
		ID_SOLDIER_BTN7 = 9,
		ID_SOLDIER_BTN8 = 10,
		ID_SOLDIER_BTN9 = 11,
		ID_SOLDIER_BTN10= 12,
		ID_SOLDIER_BTN11= 13,
		ID_SOLDIER_BTN12= 14,
	};

	enum 
	{
		PAGE_PER_ITEM_NUM= 6,
		MAX_SOLDIER_BTN  = 12,
	};

protected:
	int       m_iCurPage;
	int       m_iSelectCharArray;
	IntVec    m_vCharSlot;
	ioWnd    *m_pReturnWnd;
	
	ioUserKeyInput::UseKeyValue m_kKeys;
	ioUserKeyInput::UseKeyValue m_kSecondKeys;
	ioUserKeyInput::UseKeyValue m_kJoyKeys;

protected:
	void SetCharSlotVec();
	int  GetCharSlotIndexToVecArray( int iSelIdx );
	int  GetCharVecArrayToSlotIndex( int iSelArray, bool bPlus );

protected:
	void  OnKeyProcess();
	DWORD CheckDropEnable( const ioMouse& mouse, ioDragItem *pItem );
	void  SetKeySelect();
	bool CheckCurPage( int iIndex, int iCurPage );

public:
	void SetFunctionKeySelect( int iArray );
	void ChangeSoldierSlot( int iLeftArray, int iRightArray );
	void AddMySoldier( DWORD dwIndex );
	void SettingSoldierBtn();
	void ReSetScroll();
	void SetReturnWnd(ioWnd * pReturnWnd) { m_pReturnWnd = pReturnWnd; }

	inline int  GetSelectCharArray() const { return m_iSelectCharArray; }
	inline void SetSelectCharArray( int iSelectCharArray ){ m_iSelectCharArray = iSelectCharArray; }

	void ShowUISoldierSelectWnd( ioWnd *pReturnWnd );

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

public:
	virtual void iwm_show();
	virtual void iwm_vscroll(DWORD dwID, int curPos );
	virtual void iwm_wheel( int zDelta );
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );
	virtual bool iwm_spacebar();
	virtual IWDropEffect iwm_dragenter( ioDragItem *pItem, const ioMouse& mouse );
	virtual IWDropEffect iwm_dragover( ioDragItem *pItem, const ioMouse& mouse );
	virtual bool iwm_dropitem( ioDragItem *pItem, const ioMouse& mouse, ioWnd *pStartWnd );
	virtual void iwm_dropdone( ioDragItem *pItem, ioWnd *pDroppedWnd, bool bDropped );

public:
	UISoldierSelectWnd(void);
	virtual ~UISoldierSelectWnd(void);
};