#include "StdAfx.h"

#include "../io3DEngine/ioWnd.h"
#include "../io3DEngine/ioScroll.h"
#include "../io3DEngine/ioFontManager.h"
#include "../io3DEngine/ioUIRenderImage.h"
#include "../io3DEngine/ioUIRenderFrame.h"

#include "../WndID.h"
#include "../TextColorTable.h"
#include "../NetworkWrappingFunc.h"
#include "../ioApplication.h"
#include "../ioMyInfo.h"
#include "../DirectInput.h"
#include "../ioKeyManager.h"

#include "ioSP2GUIManager.h"
#include "ioSpecificDragItem.h"
#include ".\uisoldierselectwnd.h"
#include <strsafe.h>
#include "..\io3DEngine\ioStringManager.h"
#include "..\Local\ioLocalParent.h"

DWORD UISoldierBtn::sg_dwDragFocusID = 0;
DWORD UISoldierBtn::sg_dwFocusID     = 0;

UISoldierBtn::UISoldierBtn()
{
	m_pBlueKeyBack = NULL;
	m_pPinkKeyBack = NULL;
	m_pGrayKeyBack = NULL;
	m_pMortmainMark= NULL;
	m_pIconBack    = NULL;
	m_pIcon        = NULL;

	m_bPCRoomOpen  = false;

	m_iSoldierArray = -1;
	m_iSoldierSlot  = -1;
	m_pDragItem     = new ioShopSoldierDragItem( SDI_SHOP_SOLDIER_ITEM );

	m_bBtnOver    = false;
	m_bBtnDown    = false;
	m_bPCRoomOpen = false;

	m_nGradeType = 0;
}

UISoldierBtn::~UISoldierBtn()
{
	SAFEDELETE( m_pBlueKeyBack );
	SAFEDELETE( m_pPinkKeyBack );
	SAFEDELETE( m_pGrayKeyBack );
	SAFEDELETE( m_pMortmainMark );
	SAFEDELETE( m_pIconBack );
	SAFEDELETE( m_pIcon );
	SAFEDELETE( m_pDragItem );
}

void UISoldierBtn::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{		
	if( szType == "BlueKeyBack" )
	{
		SAFEDELETE( m_pBlueKeyBack );
		m_pBlueKeyBack = pImage;
	}
	else if( szType == "PinkKeyBack" )
	{
		SAFEDELETE( m_pPinkKeyBack );
		m_pPinkKeyBack = pImage;
	}
	else if( szType == "GrayKeyBack" )
	{
		SAFEDELETE( m_pGrayKeyBack );
		m_pGrayKeyBack = pImage;
	}
	else if( szType == "MortmainMark" )
	{
		SAFEDELETE( m_pMortmainMark );
		m_pMortmainMark = pImage;
		if( m_pMortmainMark )
			m_pMortmainMark->SetScale( UI_SOLDIERSELECT_MORTMAIN_MARK_SACLE );
	}
	else if( szType == "IconBack" )
	{
		SAFEDELETE( m_pIconBack );
		m_pIconBack = pImage;
		if( m_pIconBack )
			m_pIconBack->SetScale( SHOP_MY_SOLDIER_BTN_SCALE );
	}
	else
	{
		ioButton::AddRenderImage( szType, pImage );
	}
}

void UISoldierBtn::AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame )
{
	ioButton::AddRenderFrame( szType, pFrame );

	if( m_pNormalFrm )
		m_pNormalFrm->SetAlpha( (float)MAX_ALPHA_RATE * 0.60f );
}

void UISoldierBtn::RenderIcon( float fXPos, float fYPos )
{
	if( !m_pIcon )
		return;
	if( !m_pIconBack )
		return;

	m_pIconBack->Render( fXPos + GetWidth()/2, fYPos + GetHeight()/2, UI_RENDER_MULTIPLY, TFO_BILINEAR );	

	if( g_MyInfo.GetCharIndex( m_iSoldierArray ) == 0 || !g_MyInfo.IsCharActive( m_iSoldierArray ) )
	{
		g_UIImageRenderMgr.RenderGradeIconBack( m_nGradeType, fXPos + GetWidth()/2, fYPos + GetHeight()/2, SHOP_MY_SOLDIER_BTN_SCALE, UI_RENDER_GRAY );
		m_pIcon->Render( fXPos + GetWidth()/2, fYPos + GetHeight()/2, UI_RENDER_GRAY, TFO_BILINEAR );
	}
	else
	{
		g_UIImageRenderMgr.RenderGradeIconBack( m_nGradeType, fXPos + GetWidth()/2, fYPos + GetHeight()/2, SHOP_MY_SOLDIER_BTN_SCALE );
		m_pIcon->Render( fXPos + GetWidth()/2, fYPos + GetHeight()/2, UI_RENDER_NORMAL, TFO_BILINEAR );
	}
}

void UISoldierBtn::RenderInfo( float fXPos, float fYPos )
{
	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_3X );
	g_FontMgr.SetBkColor( TCT_DEFAULT_DARKGRAY );
	g_FontMgr.SetTextColor( TCT_DEFAULT_WHITE );

	if( m_bPCRoomOpen || g_MyInfo.IsCharExerciseStyle( m_iSoldierArray, EXERCISE_PCROOM ) ) 
	{
		if( g_MyInfo.GetPCRoomAuthority() == FREEDAY_EVENT_CODE )
			g_FontMgr.PrintText( fXPos + 29, fYPos + 42, FONT_SIZE_10, STR(1) );
		else
			g_FontMgr.PrintText( fXPos + 29, fYPos + 42, FONT_SIZE_10, STR(1) );
	}
	else if( g_MyInfo.IsCharExerciseStyle( m_iSoldierArray, EXERCISE_RENTAL ) ) 
	{
		g_FontMgr.PrintText( fXPos + 29, fYPos + 42, FONT_SIZE_10, STR(3) );
	}
	else if( g_MyInfo.IsCharExerciseStyle( m_iSoldierArray, EXERCISE_EVENT ) )
	{
		g_FontMgr.PrintText( fXPos + 29, fYPos + 42, FONT_SIZE_10, STR(3) );
	}
	else if( g_MyInfo.GetCharIndex( m_iSoldierArray ) != 0 && !g_MyInfo.IsCharActive( m_iSoldierArray ) )
	{
		g_FontMgr.PrintText( fXPos + 29, fYPos + 42, FONT_SIZE_10, STR(2) );
	}
	else if( g_MyInfo.GetCharIndex( m_iSoldierArray ) != 0 && g_MyInfo.IsCharMortmain(  g_MyInfo.GetClassType( m_iSoldierArray ) ) )
	{
		if( m_pMortmainMark )
		{
			m_pMortmainMark->Render( fXPos + 30, fYPos + 48 , UI_RENDER_NORMAL, TFO_BILINEAR );
		}
	}
	else if( g_MyInfo.GetCharIndex( m_iSoldierArray ) != 0 )
	{
		int iTotalLimitDate = g_MyInfo.GetCharTotalSecond( m_iSoldierArray ) / 60;
		int iHour     = iTotalLimitDate / 60;      
		int iMinute   = iTotalLimitDate % 60;
		char szTitle[MAX_PATH] = "";
		StringCbPrintf_e( szTitle, sizeof( szTitle ), "%.2d:%.2d", iHour, iMinute );  //Except Extracting Hangeul
		g_FontMgr.PrintText( fXPos + 29, fYPos + 42, FONT_SIZE_10, szTitle );
	}
}

void UISoldierBtn::RenderKeyState( int iXPos, int iYPos )
{
	g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetTextColor( 255, 255, 255 );
	if( m_bPCRoomOpen || g_MyInfo.IsCharExerciseStyle( m_iSoldierArray, EXERCISE_PCROOM ) || g_MyInfo.IsCharExerciseStyle( m_iSoldierArray, EXERCISE_EVENT ) || g_MyInfo.IsCharExerciseStyle( m_iSoldierArray, EXERCISE_RENTAL ) ) 
	{
		if( m_pPinkKeyBack )
			m_pPinkKeyBack->Render( iXPos, iYPos );
		g_FontMgr.SetBkColor( 154, 39, 59 );
	}
	else if( g_MyInfo.GetCharIndex( m_iSoldierArray ) == 0 || !g_MyInfo.IsCharActive( m_iSoldierArray ) )
	{
		if( m_pGrayKeyBack )
			m_pGrayKeyBack->Render( iXPos, iYPos );
		g_FontMgr.SetBkColor( TCT_DEFAULT_GRAY );
	}
	else 
	{
		if( m_pBlueKeyBack )
			m_pBlueKeyBack->Render( iXPos, iYPos );
		g_FontMgr.SetBkColor( 33, 90, 162 );
	}

	if( !COMPARE( m_iSoldierSlot, 0, MAX_SLOT_KEY ) )
	{
		g_FontMgr.PrintText( iXPos + 11, iYPos + 2, FONT_SIZE_11, "-" );
	}
	else
	{
		char szKeyText[MAX_PATH] = "";
		StringCbCopy( szKeyText, sizeof( szKeyText ), g_KeyManager.GetKeyText( g_KeyManager.GetCurrentSoldierKey( m_iSoldierSlot ) ) );
		if( strcmp( szKeyText, STR(1) ) == 0 )
		{			
			StringCbPrintf_e( szKeyText, sizeof( szKeyText ), "-" );				
		}
		szKeyText[2] = NULL;
		g_FontMgr.PrintText( iXPos + 11, iYPos + 2, FONT_SIZE_11, szKeyText );
	}
}

void UISoldierBtn::OnRender()
{
	ioButton::OnRender();

	float fXPos, fYPos;
	fXPos = GetDerivedPosX();
	fYPos = GetDerivedPosY();

	RenderIcon( fXPos, fYPos );
	RenderKeyState( fXPos + 4, fYPos + 4 );
	RenderInfo( fXPos, fYPos );
}

void UISoldierBtn::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;

	if( m_bBtnOver && !m_bBtnDown )
	{
		ioLSMouse *pMouse = g_App.GetMouse();
		if( pMouse && pMouse->IsMouseShowing() &&
			g_MyInfo.IsCharActive(m_iSoldierArray) && !g_MyInfo.IsCharExercise(m_iSoldierArray) )
		{
			int iClassType = g_MyInfo.GetClassType( m_iSoldierArray );
			pMouse->SetToolTipID( g_MyInfo.GetPublicID(), iClassType );
		}
	}
}

void UISoldierBtn::SetCharacter( int iArray, int iSlot, bool bPCRoomOpen )
{
	m_bPCRoomOpen    = bPCRoomOpen;
	m_iSoldierArray  = iArray;
	m_iSoldierSlot   = iSlot;

	SAFEDELETE( m_pIcon );

	m_nGradeType = 0;

	if( m_iSoldierArray != -1 )
	{
		int iClassType = g_MyInfo.GetClassType( m_iSoldierArray );
		m_pIcon = g_MyInfo.GetMySoldierIcon( iClassType );
		if( m_pIcon )
			m_pIcon->SetScale( SHOP_MY_SOLDIER_BTN_SCALE );

		m_nGradeType = g_UIImageRenderMgr.GetPowerUpGradeType( iClassType );
	}
}

int UISoldierBtn::GetCharacterArray()
{
	return m_iSoldierArray;
}

bool UISoldierBtn::IsSoldierSlot()
{
	if( g_MyInfo.GetCharIndex( m_iSoldierArray ) != 0 )
		return true;

	return false;
}

void UISoldierBtn::iwm_mouseleave( const ioMouse& mouse )
{
	if( m_bClicked && g_MyInfo.GetCharIndex( m_iSoldierArray ) != 0 )
	{
		POINT ptOffset = { 0, 0};
		if( m_pDragItem )
		{
			m_pDragItem->SetSoldierArray( m_iSoldierArray );
			m_pDragItem->SetDragImageName( g_MyInfo.GetMySoldierIconName( g_MyInfo.GetClassType( m_iSoldierArray ) ) );
			m_pDragItem->DoDragDrop( GetParent(), m_rcPos, ptOffset );
		}
		sg_dwDragFocusID = GetID();
	}

	ioButton::iwm_mouseleave( mouse );

	m_bBtnOver = false;
	m_bBtnDown = false;
}

void UISoldierBtn::iwm_mouseover( const ioMouse& mouse )
{	
	ioButton::iwm_mouseover( mouse );

	if(!HasWndStyle( IWS_INACTIVE ) )
	{
		ioButton::GoToTop();
	}

	ioLSMouse *pMouse = g_App.GetMouse();
	if( pMouse && !pMouse->IsLBtnDown() )
	{
		m_bBtnOver = true;
	}
}

void UISoldierBtn::iwm_lbuttondown( const ioMouse& mouse )
{
	m_bBtnDown = true;

	ioButton::iwm_lbuttondown( mouse );
}

void UISoldierBtn::iwm_lbuttonup( const ioMouse& mouse )
{
	m_bBtnDown = false;

	ioButton::iwm_lbuttonup( mouse );
}

void UISoldierBtn::OnDrawOvered( int iXPos, int iYPos )
{
	bool bFocus = false;
	if( sg_dwFocusID == m_dwID || sg_dwDragFocusID == m_dwID )
		bFocus = true;
	else
	{
		UISoldierSelectWnd *pSoldierSelect = dynamic_cast<UISoldierSelectWnd*> ( GetParent() );
		if( pSoldierSelect && pSoldierSelect->GetSelectCharArray() == GetCharacterArray() )
			bFocus = true;
	}

	if( GetCharacterArray() == -1 )
	{
		ioButton::OnDrawNormal( iXPos, iYPos );
	}
	else
	{
		if( bFocus )
			ioButton::OnDrawOvered( iXPos, iYPos );
		else
			ioButton::OnDrawNormal( iXPos, iYPos );
	}
}

void UISoldierBtn::OnDrawPushed( int iXPos, int iYPos )
{
	if( GetCharacterArray() != -1 )	
		ioButton::OnDrawPushed( iXPos, iYPos );
	else
		ioButton::OnDrawNormal( iXPos, iYPos );
}

void UISoldierBtn::OnDrawNormal( int iXPos, int iYPos )
{
	bool bFocus = false;
	if( sg_dwFocusID == m_dwID || sg_dwDragFocusID == m_dwID )
		bFocus = true;
	else
	{
		UISoldierSelectWnd *pSoldierSelect = dynamic_cast<UISoldierSelectWnd*> ( GetParent() );
		if( pSoldierSelect && pSoldierSelect->GetSelectCharArray() == GetCharacterArray() )
		{
			if( sg_dwFocusID == 0 && sg_dwDragFocusID == 0 )
				bFocus = true;
		}
	}

	if( bFocus ) 
		ioButton::OnDrawOvered( iXPos, iYPos );
	else 
		ioButton::OnDrawNormal( iXPos, iYPos );
}

void UISoldierBtn::iwm_hide()
{
	m_bBtnDown = false;
	m_bBtnOver = false;
}

void UISoldierBtn::iwm_show()
{
	m_bBtnDown = false;
	m_bBtnOver = false;
}

//-------------------------------------------------------------------------
UISoldierSelectWnd::UISoldierSelectWnd()
{
	m_iCurPage           = 0;
	m_iSelectCharArray   = 0;
	m_pReturnWnd         = NULL;
}

UISoldierSelectWnd::~UISoldierSelectWnd()
{
	m_vCharSlot.clear();
}

void UISoldierSelectWnd::iwm_show()
{
	// key
	m_kKeys.SetDefaultFirstKey();
	m_kSecondKeys.SetDefaultSecondKey();
	m_kJoyKeys.SetDefaultJoyKey();

	g_KeyManager.GetGameKey(m_kKeys);
	g_KeyManager.GetSecondGameKey(m_kSecondKeys);
	g_KeyManager.GetJoyGameKey(m_kJoyKeys);

	m_pReturnWnd = NULL;

	SettingSoldierBtn();
	ReSetScroll();
}

void UISoldierSelectWnd::ReSetScroll()
{
	ioScroll *pScroll = (ioScroll*)FindChildWnd( ID_VERT_SCROLL );
	if( pScroll )
	{
		int iCurMax = g_MyInfo.GetCurMaxCharSlot() / PAGE_PER_ITEM_NUM;
		int iRest	= g_MyInfo.GetCurMaxCharSlot() % PAGE_PER_ITEM_NUM;
		if( iCurMax > 1 )
			iCurMax--;

		if( 0 < iRest )
			iCurMax++;

		pScroll->SetScrollRange( 0, iCurMax );
	}
}


void UISoldierSelectWnd::SettingSoldierBtn()
{
	// 피씨방 유저인데 피씨방 캐릭을 선택하지 않았으면..
	IntVec vPCRoomEmptySlot;
	if( g_MyInfo.GetPCRoomAuthority() )
	{
		ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
		if( pLocal && pLocal->IsPCRoomBonusSoldier() )
		{
			int iArrayCount = g_MyInfo.GetPcRoomCharMax() - g_MyInfo.GetExerciseCharCount( EXERCISE_PCROOM );
			g_MyInfo.GetCharEmptySlotArray( iArrayCount, vPCRoomEmptySlot ); 
		}
	}

	for( int i=ID_SOLDIER_BTN1; i<ID_SOLDIER_BTN12+1; i++ )
	{
		UISoldierBtn *pBtn = dynamic_cast<UISoldierBtn*> ( FindChildWnd(i) );
		if( !pBtn )	
			continue;

		int iSlotIdx = ( i - ID_SOLDIER_BTN1 ) + ( m_iCurPage * PAGE_PER_ITEM_NUM );
		int iArray   = g_MyInfo.GetCharSlotIndexToArray( iSlotIdx );

		bool bPCRoom = false;
		IntVec::iterator iter = vPCRoomEmptySlot.begin();
		for( ; iter != vPCRoomEmptySlot.end(); ++iter )
		{
			if( *iter == iSlotIdx )
			{
				bPCRoom = true;
				break;
			}
		}

		pBtn->SetCharacter( iArray, iSlotIdx, bPCRoom );

		if( !pBtn->IsShow() )
			pBtn->ShowWnd();
	}

	SetCharSlotVec();
}

void UISoldierSelectWnd::SetCharSlotVec()
{
	int iCurMaxSlot = g_MyInfo.GetCurMaxCharSlot();
	m_vCharSlot.erase( m_vCharSlot.begin(), m_vCharSlot.end() );
	m_vCharSlot.reserve( iCurMaxSlot );

	for( int i=0; i < iCurMaxSlot; i++ )
	{
		if( g_MyInfo.GetCharSlotIndexToArray( i ) == -1 )
			m_vCharSlot.push_back( -1 );
		else
			m_vCharSlot.push_back( i );
	}
}

int UISoldierSelectWnd::GetCharSlotIndexToVecArray( int iSelIdx )
{
	int iSize = m_vCharSlot.size();
	for(int i = 0;i < iSize;i++)
	{
		if( m_vCharSlot[i] == iSelIdx )
			return i;
	}

	return -1;
}

int UISoldierSelectWnd::GetCharVecArrayToSlotIndex( int iSelArray, bool bPlus  )
{
	int iSize = m_vCharSlot.size();
	if( !COMPARE( iSelArray, 0, iSize ) )
		return -1;

	int i = 0;
	if( bPlus )
	{
		for(i = iSelArray;i < iSize;i++)
		{
			if( m_vCharSlot[i] != -1 )
				return m_vCharSlot[i];
		}

		for(i = 0;i < iSize;i++)
		{
			if( m_vCharSlot[i] != -1 )
				return m_vCharSlot[i];
		}
	}
	else
	{
		for(i = iSelArray;i >= 0;i--)
		{
			if( m_vCharSlot[i] != -1 )
				return m_vCharSlot[i];
		}

		for(i = iSize - 1;i >= 0;i--)
		{
			if( m_vCharSlot[i] != -1 )
				return m_vCharSlot[i];
		}
	}

	return -1;
}

void UISoldierSelectWnd::AddMySoldier( DWORD dwIndex )
{
	SettingSoldierBtn();
}

void UISoldierSelectWnd::iwm_wheel( int zDelta )
{
	if( !IsShow() )
		ioWnd::iwm_wheel( zDelta );
	else
	{ 
		// 휠 이벤트 처리를 했다면 부모로 이벤트를 날리지 않는다.
		if( zDelta == WHEEL_DELTA )
		{
			ioScroll *pScroll = (ioScroll*)FindChildWnd( ID_VERT_SCROLL );
			if( pScroll )
				pScroll->SetScrollPos( m_iCurPage - 1 );
			if( g_GUIMgr.GetPreOverWnd() )
			{
				ioMouse NullMouse;
				g_GUIMgr.GetPreOverWnd()->iwm_mouseover( NullMouse );
			}
		}
		else if( zDelta == -WHEEL_DELTA )
		{
			ioScroll *pScroll = (ioScroll*)FindChildWnd( ID_VERT_SCROLL );
			if( pScroll )
				pScroll->SetScrollPos( m_iCurPage + 1 );
			if( g_GUIMgr.GetPreOverWnd() )
			{
				ioMouse NullMouse;
				g_GUIMgr.GetPreOverWnd()->iwm_mouseover( NullMouse );
			}
		}
	}
}

void UISoldierSelectWnd::iwm_vscroll(DWORD dwID, int curPos )
{
	m_iCurPage = curPos;
	SettingSoldierBtn();
}

bool UISoldierSelectWnd::iwm_spacebar()
{
	if( !IsShow() ) return false;

	SetKeySelect();
	return true;
}

void UISoldierSelectWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();

	switch( dwID )
	{
	case ID_EXIT:
		if( cmd == IOBN_BTNUP )
		{
			HideWnd();
		}
		break;
	case ID_SOLDIER_BTN1:
	case ID_SOLDIER_BTN2:
	case ID_SOLDIER_BTN3:
	case ID_SOLDIER_BTN4:
	case ID_SOLDIER_BTN5:
	case ID_SOLDIER_BTN6:
	case ID_SOLDIER_BTN7:
	case ID_SOLDIER_BTN8:
	case ID_SOLDIER_BTN9:
	case ID_SOLDIER_BTN10:
	case ID_SOLDIER_BTN11:
	case ID_SOLDIER_BTN12:
		if( cmd == IOBN_BTNUP )
		{
			UISoldierBtn *pSoldierBtn = dynamic_cast<UISoldierBtn*> ( pWnd );
			if( COMPARE( pSoldierBtn->GetCharacterArray(), 0, g_MyInfo.GetCharCount() ) )
			{
				SetSelectCharArray( pSoldierBtn->GetCharacterArray() );
				SetKeySelect();
			}
		}
		else if( cmd == IOWN_OVERED )
		{
			UISoldierBtn *pSoldierBtn = dynamic_cast<UISoldierBtn*> ( pWnd );
			if( COMPARE( pSoldierBtn->GetCharacterArray(), 0, g_MyInfo.GetCharCount() ) )
				SetSelectCharArray( pSoldierBtn->GetCharacterArray() );
		}
		break;
	}
}

void UISoldierSelectWnd::SetFunctionKeySelect( int iArray )
{
	if( iArray == -1  )
		return;

	int iArrayToSlot = g_MyInfo.GetCharArrayToSlotIndex( iArray );
	ioScroll *pScroll = dynamic_cast<ioScroll*>( FindChildWnd( ID_VERT_SCROLL ) );
	if( pScroll )
	{
		// 첫 페이지 보여주기
		pScroll->SetScrollPos( 0 );
	}

	int iBtnIdx = iArrayToSlot % MAX_SOLDIER_BTN;
	UISoldierBtn *pSoldierBtn = dynamic_cast<UISoldierBtn*> ( FindChildWnd( iBtnIdx + ID_SOLDIER_BTN1 ) );
	if( pSoldierBtn )
	{
		int iCharCount = pSoldierBtn->GetCharacterArray();
		if( iCharCount != -1 && g_MyInfo.IsCharActive( iCharCount ) )
		{
			iwm_command( dynamic_cast<ioWnd*>( pSoldierBtn ), IOBN_BTNUP, 0 );
		}
	}
}

void UISoldierSelectWnd::ChangeSoldierSlot( int iLeftArray, int iRightArray )
{
	SettingSoldierBtn();
}

void UISoldierSelectWnd::OnKeyProcess()
{
	int iCurChar = GetCharSlotIndexToVecArray( g_MyInfo.GetCharArrayToSlotIndex( GetSelectCharArray() ) );
	if( iCurChar == -1 )
		return;
	int iPrevChar= iCurChar;
	int iMaxChar = m_vCharSlot.size() - 1;
	bool bPlus   = false;

	if( g_Input.IsKeyUp( m_kKeys.m_wLeftKey )       ||
		g_Input.IsKeyUp( m_kSecondKeys.m_wLeftKey ) ||
		g_Input.IsKeyUp( m_kJoyKeys.m_wLeftKey ) )
	{			
		iCurChar -= 1;
		if( iCurChar < 0 )
			iCurChar = iMaxChar;
	}
	else if( g_Input.IsKeyUp( m_kKeys.m_wRightKey )       ||
		     g_Input.IsKeyUp( m_kSecondKeys.m_wRightKey ) ||
		     g_Input.IsKeyUp( m_kJoyKeys.m_wRightKey ) )
	{
		iCurChar += 1;
		if( iCurChar > iMaxChar )
			iCurChar = 0;
		bPlus = true;
	}
	else if( g_Input.IsKeyUp( m_kKeys.m_wUpKey )       ||
			 g_Input.IsKeyUp( m_kSecondKeys.m_wUpKey ) ||
			 g_Input.IsKeyUp( m_kJoyKeys.m_wUpKey ) )
	{
		iCurChar -= PAGE_PER_ITEM_NUM;
		if( iCurChar < 0 )
		{
			iCurChar = min( iMaxChar, (((iMaxChar+1)/PAGE_PER_ITEM_NUM)*PAGE_PER_ITEM_NUM) + (PAGE_PER_ITEM_NUM+iCurChar) );		
			if( COMPARE( iCurChar, 0, iMaxChar + 1 ) )
			{
				if( m_vCharSlot[iCurChar] == -1 )
					iCurChar = iMaxChar;
			}
		}		
		else
		{
			if( COMPARE( iCurChar, 0, iMaxChar + 1 ) )
			{
				if( m_vCharSlot[iCurChar] == -1 )
				{
					int iGap = iCurChar % PAGE_PER_ITEM_NUM;
					iGap = PAGE_PER_ITEM_NUM - iGap - 1;

					iCurChar += iGap;
				}
			}
		}
	}
	else if( g_Input.IsKeyUp( m_kKeys.m_wDownKey )       ||
			 g_Input.IsKeyUp( m_kSecondKeys.m_wDownKey ) ||
			 g_Input.IsKeyUp( m_kJoyKeys.m_wDownKey ) )
	{
		iCurChar += PAGE_PER_ITEM_NUM;

		if( iCurChar > iMaxChar )
		{
			if( iCurChar / PAGE_PER_ITEM_NUM == iMaxChar / PAGE_PER_ITEM_NUM )
				iCurChar = min( iMaxChar, (((iMaxChar+1)/PAGE_PER_ITEM_NUM)*PAGE_PER_ITEM_NUM) + (iCurChar % PAGE_PER_ITEM_NUM) );
			else
				iCurChar = iCurChar % PAGE_PER_ITEM_NUM;

			if( COMPARE( iCurChar, 0, iMaxChar + 1 ) )
			{
				if( m_vCharSlot[iCurChar] == -1 )
					iCurChar = 0;
			}
		}
		else
		{
			if( COMPARE( iCurChar, 0, iMaxChar + 1 ) )
			{
				if( m_vCharSlot[iCurChar] == -1 )
				{
					int iGap = iCurChar % PAGE_PER_ITEM_NUM;
					iCurChar -= iGap;
				}
			}
		}
		bPlus = true;
	}

	if( iCurChar != iPrevChar )
	{
		int iCurIdx  = GetCharVecArrayToSlotIndex( iCurChar, bPlus );
		iCurChar = g_MyInfo.GetCharSlotIndexToArray( iCurIdx );
		if( iCurChar == -1 )
			return;

		SetSelectCharArray( iCurChar );
		ioScroll *pScroll = (ioScroll*)FindChildWnd( ID_VERT_SCROLL );
		if( pScroll )
		{
			int iCurPos = pScroll->GetCurPos();
			if( !CheckCurPage( iCurIdx, iCurPos ) )
			{
				if( bPlus )
					iCurPos++;
				else
					iCurPos--;

				if( iCurPos < 0 || iCurPos > pScroll->GetMaxPos() )
					iCurPos = iCurIdx / PAGE_PER_ITEM_NUM;

				// 바뀐 페이지에도 없으면 그냥 그 인덱스 페이지로
				if( !CheckCurPage( iCurIdx, iCurPos ) )
					iCurPos = iCurIdx / PAGE_PER_ITEM_NUM;
			}

			if( iCurPos != pScroll->GetCurPos() )
				pScroll->SetScrollPos( iCurPos );
		}
	}
}

bool UISoldierSelectWnd::CheckCurPage( int iIndex, int iCurPage )
{
	ioScroll *pScroll = (ioScroll*)FindChildWnd( ID_VERT_SCROLL );
	if( pScroll )
	{
		int iFirstIndex = iCurPage * PAGE_PER_ITEM_NUM;

		return COMPARE( iIndex, iFirstIndex, iFirstIndex+MAX_SOLDIER_BTN );
	}

	return false;
}

void UISoldierSelectWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;

	ioWnd::OnProcess( fTimePerSec );
	
	OnKeyProcess();

	//if( g_Input.IsKeyUp( KEY_ENTER )                  ||
	//	g_Input.IsKeyUp( m_kKeys.m_wAttackKey )       ||
	//	g_Input.IsKeyUp( m_kSecondKeys.m_wAttackKey ) ||
	//	g_Input.IsKeyUp( m_kJoyKeys.m_wAttackKey ) )
	//{
	//	SetKeySelect();		
	//}
    //
	//int iArray = g_KeyManager.GetSoldierKeyPressArray();
	//if( COMPARE(iArray, 0, MAX_CHARACTER_KEY ) )
	//{
	//	int iSlotToArray = g_MyInfo.GetCharSlotIndexToArray( iArray );
	//	SetFunctionKeySelect( iSlotToArray );
	//}

	if( m_pReturnWnd && !m_pReturnWnd->IsShow() )
		HideWnd();
}

void UISoldierSelectWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos, iYPos;
	iXPos = GetDerivedPosX();
	iYPos = GetDerivedPosY();

	if( m_pReturnWnd && m_pReturnWnd->GetID() == MANNER_VALUATION_WND )
	{
		g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
		g_FontMgr.SetAlignType( TAT_LEFT );
		g_FontMgr.SetBkColor( 12, 66, 111 );	
		g_FontMgr.SetTextColor( TCT_DEFAULT_WHITE );
		g_FontMgr.PrintText( iXPos + 12, iYPos + 10, FONT_SIZE_13, STR(1) );
	}
	else
	{
		g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
		g_FontMgr.SetAlignType( TAT_LEFT );
		g_FontMgr.SetBkColor( 12, 66, 111 );	
		g_FontMgr.SetTextColor( TCT_DEFAULT_WHITE );
		g_FontMgr.PrintText( iXPos + 12, iYPos + 10, FONT_SIZE_13, STR(2) );
	}
}

void UISoldierSelectWnd::SetKeySelect()
{
	if( m_pReturnWnd && m_pReturnWnd->IsShow() )
		m_pReturnWnd->iwm_command( this, IOBN_BTNUP, GetSelectCharArray() );

	HideWnd();
}

IWDropEffect UISoldierSelectWnd::iwm_dragenter( ioDragItem *pItem, const ioMouse& mouse )
{
	DWORD dwBtnID = CheckDropEnable( mouse, pItem );
	if( dwBtnID == 0 )
		return IW_DROP_NONE;

	return IW_DROP_ENABLE;
}

IWDropEffect UISoldierSelectWnd::iwm_dragover( ioDragItem *pItem, const ioMouse& mouse )
{
	return iwm_dragenter( pItem, mouse );
}

bool UISoldierSelectWnd::iwm_dropitem( ioDragItem *pItem, const ioMouse& mouse, ioWnd *pStartWnd )
{
	DWORD dwBtnID = CheckDropEnable( mouse, pItem );
	if( dwBtnID == 0 )
		return false;

	ioShopSoldierDragItem *pDragItem = ToShopSoldierDragItem( pItem );
	if( !pDragItem )
		return false;

	int iCurMaxSlot     = g_MyInfo.GetCurMaxCharSlot();
	int iLeftSlotIndex  = g_MyInfo.GetCharArrayToSlotIndex( pDragItem->GetSoldierArray() );
	int iRightSlotIndex = ( UISoldierBtn::sg_dwFocusID - ID_SOLDIER_BTN1 ) + ( m_iCurPage * PAGE_PER_ITEM_NUM );
	if( !COMPARE( iLeftSlotIndex, 0, iCurMaxSlot) || !COMPARE( iRightSlotIndex, 0, iCurMaxSlot) ) return false;

	if( iLeftSlotIndex != iRightSlotIndex )
	{
		SP2Packet kPacket( CTPK_CHAR_SLOT_CHANGE );
		kPacket << iLeftSlotIndex << iRightSlotIndex;
		TCPNetwork::SendToServer( kPacket );
		TCPNetwork::MouseBusy( true );
	}

	UISoldierBtn::sg_dwFocusID = 0;
	UISoldierBtn::sg_dwDragFocusID = 0;
	return true;
}

void UISoldierSelectWnd::iwm_dropdone( ioDragItem *pItem, ioWnd *pDroppedWnd, bool bDropped )
{
	UISoldierBtn::sg_dwFocusID = 0;
	UISoldierBtn::sg_dwDragFocusID = 0;
}

DWORD UISoldierSelectWnd::CheckDropEnable( const ioMouse& mouse, ioDragItem *pItem )
{
	ioShopSoldierDragItem *pDragItem = ToShopSoldierDragItem( pItem );
	if( !pDragItem )	return 0;

	UISoldierBtn::sg_dwFocusID = 0;
	DWORD dwBtnID = 0;
	int i = 0;

	for( i = ID_SOLDIER_BTN1; i < ID_SOLDIER_BTN12+1; i++ )
	{
		dwBtnID = i;
		UISoldierBtn *pBtn = dynamic_cast<UISoldierBtn*>( FindChildWnd( dwBtnID ) );
		if( pBtn && !pBtn->HasWndStyle( IWS_INACTIVE ) && pBtn->IsShow())
		{
			if( pBtn->IsInWndRect( mouse.GetMousePos() ) )
			{
				UISoldierBtn::sg_dwFocusID = dwBtnID;
				return dwBtnID;
			}
			else
			{
				ioMouse NullMouse;
				pBtn->iwm_mouseleave( NullMouse );
			}
		}
	}

	ioScroll *pScroll = dynamic_cast<ioScroll*>( FindChildWnd( ID_VERT_SCROLL ) );
	if( pScroll && !pScroll->HasWndStyle( IWS_INACTIVE ) && pScroll->IsInWndRect( mouse.GetMousePos() ) )
	{
		ioButton *pUpBtn = dynamic_cast<ioButton*>( pScroll->FindChildWnd( ioScroll::ID_SCROLL_UP ) );
		if( pUpBtn && pUpBtn->IsInWndRect( mouse.GetMousePos() ) )
		{
			pScroll->SetScrollPos( m_iCurPage - 1 );
		}
		ioButton *pDownBtn = dynamic_cast<ioButton*>( pScroll->FindChildWnd( ioScroll::ID_SCROLL_DOWN ) );
		if( pDownBtn && pDownBtn->IsInWndRect( mouse.GetMousePos() ) )
		{
			pScroll->SetScrollPos( m_iCurPage + 1 );
		}
	}
	return 0 ;
}

void UISoldierSelectWnd::ShowUISoldierSelectWnd( ioWnd *pReturnWnd )
{
	ShowWnd();
	m_pReturnWnd = pReturnWnd; // show 이후 설정
}