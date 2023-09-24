
#include "StdAfx.h"

#include "../io3DEngine/ioWnd.h"
#include "../io3DEngine/ioFontManager.h"
#include "../io3DEngine/ioUIRenderImage.h"
#include "../io3DEngine/ioUIRenderFrame.h"

#include "../WndID.h"
#include "../NetworkWrappingFunc.h"
#include "../ioMyInfo.h"
#include "../ioPlayStage.h"
#include "../ioBaseChar.h"
#include "../Setting.h"
#include "../ioKeyManager.h"

#include "../ioNakoruruItem.h"
#include "../ioPeterpanItem.h"
#include "../ioWindLordItem.h"
#include "../ioLeeItem.h"
#include "../ioProject_K_Item.h"
#include "../ioPsychoMetry_Item.h"
#include "../ioSuddenAttackItem.h"
#include "../ioBowItem.h"
#include "../ioLaserItem.h"
#include "../ioAutoShotItem.h"
#include "../ioUroborusItem.h"
#include "../ioWereWolfItem.h"
#include "../ioWitchItem.h"
#include "../ioExcaliburItem.h"
#include "../ioChargeComboBowItem.h"
#include "../ioTitanItem.h"
#include "../ioTraceItem.h"
#include "../ioSuperDimensionItem.h"
#include "../ioKaelthasItem.h"
#include "../ioStrikerItem.h"
#include "../ioParkilpyoItem.h"
#include "../ioJoroExpansionItem.h"
#include "../ioEarthQuakeItem.h"
#include "../ioBullFightItem.h"
#include "../ioStriderItem.h"
#include "../ioOniMushaItem.h"
#include "../ioWestGunExtendItem.h"
#include "../ioSasinmuItem.h"
#include "../ioPredatorItem.h"

#include "../ioVoiceChat/Fourier.h"

#include "ioSP2GUIManager.h"
#include "MiniSoldierSelectWnd.h"

#include <strsafe.h>

//////////////////////////////////////////////////////////////////////////
MiniSoldierBtn::MiniSoldierBtn()
{
	m_pEmptySlot = NULL;
	m_pCharIcon = NULL;
	m_pCharIconBack = NULL;
	m_pKeyBack = NULL;

	m_iSoldierNumber = -1;
	m_iSoldierSlot = -1;
	m_nGradeType = 0;
}

MiniSoldierBtn::~MiniSoldierBtn()
{
	SAFEDELETE( m_pEmptySlot );
	SAFEDELETE( m_pCharIcon );
	SAFEDELETE( m_pCharIconBack );
	SAFEDELETE( m_pKeyBack );
}

void MiniSoldierBtn::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "SlotIconBack" )
	{
		SAFEDELETE( m_pCharIconBack );
		m_pCharIconBack = pImage;
	}
	else if( szType == "EmptySlot" )
	{
		SAFEDELETE( m_pEmptySlot );
		m_pEmptySlot = pImage;
	}
	else
	{
		ioWnd::AddRenderImage( szType, pImage );
	}
}

void MiniSoldierBtn::AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame )
{
	if( szType == "KeyBack" )
	{
		SAFEDELETE( m_pKeyBack );
		m_pKeyBack = pFrame;
	}
	else
	{
		ioWnd::AddRenderFrame( szType, pFrame );
	}
}

void MiniSoldierBtn::OnRender()
{
	if( !IsShow() )
		return;

	MiniSoldierSelectWnd *pWnd = dynamic_cast<MiniSoldierSelectWnd*>( GetParent() );
	if( pWnd && pWnd->SkipRender() )
		return;

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	enum { SLOT_BACK_XOFFSET = 18, SLOT_BACK_YOFFSET = 18,
		   KEY_BACK_XOFFSET = 16, KEY_BACK_YOFFSET = 1 };

	if( m_pCharIcon && m_pCharIconBack )
	{
		m_pCharIconBack->SetScale( FLOAT05 );
		m_pCharIconBack->Render( iXPos + SLOT_BACK_XOFFSET, iYPos + SLOT_BACK_YOFFSET , UI_RENDER_MULTIPLY, TFO_BILINEAR );

		g_UIImageRenderMgr.RenderGradeIconBack( m_nGradeType, iXPos + SLOT_BACK_XOFFSET, iYPos + SLOT_BACK_YOFFSET, 0.5f );

		m_pCharIcon->SetScale( FLOAT05 );
		m_pCharIcon->Render( iXPos + SLOT_BACK_XOFFSET, iYPos + SLOT_BACK_YOFFSET , UI_RENDER_NORMAL, TFO_BILINEAR );
	}
	else if( m_pEmptySlot )
	{
		m_pEmptySlot->Render( iXPos + SLOT_BACK_XOFFSET, iYPos + SLOT_BACK_YOFFSET, UI_RENDER_MULTIPLY );
	}

	if( !m_pCharIcon )
		return;

	char szKeyText[MAX_PATH] = "";
	StringCbCopy( szKeyText, sizeof( szKeyText ), g_KeyManager.GetKeyText( g_KeyManager.GetCurrentSoldierKey( m_iSoldierSlot ) ) );

	if( strcmp( szKeyText, STR(1) ) == 0 )
	{
		StringCbPrintf( szKeyText, sizeof( szKeyText ), "-" );
	}

	if( m_pKeyBack )
	{
		m_pKeyBack->Render( iXPos, iYPos );
	}
	szKeyText[2] = NULL;

	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
	g_FontMgr.SetBkColor( 19, 53, 135 );
	g_FontMgr.SetTextColor( 220, 220, 220 );
	g_FontMgr.PrintText( iXPos + 18, iYPos + 29, FONT_SIZE_11, szKeyText );
}

void MiniSoldierBtn::SetCharacterCount( int iNumber, int iSlot )
{
	m_iSoldierNumber = iNumber;
	m_iSoldierSlot = iSlot;
	m_nGradeType = 0;

	SAFEDELETE( m_pCharIcon );
	if( m_iSoldierNumber != -1 )
	{
		int iClassType = g_MyInfo.GetClassType( m_iSoldierNumber );
		m_pCharIcon = g_MyInfo.GetMySoldierIcon( iClassType );
		m_nGradeType = g_UIImageRenderMgr.GetPowerUpGradeType( iClassType );
	}
}

//////////////////////////////////////////////////////////////////////////
MiniSoldierSelectWnd::MiniSoldierSelectWnd()
{
	m_iDefaultX = 0;
	m_iDefaultY = 0;

	m_eOwnerState		= CS_DELAY;
	m_pPlayStage		= NULL;
	m_iCurPage			= 0;
	m_bChangeResolution	= false;
	m_bSetMouseOver = false;

	m_eModeType = MT_NONE;

	InitShowAniState();
}

MiniSoldierSelectWnd::~MiniSoldierSelectWnd()
{
	if( !COMPARE( m_iDefaultX, 0, Setting::Width() ) || 
		!COMPARE( m_iDefaultY, 0, Setting::Height() ) )
		return;

	g_GUIMgr.GUIPosSaveInt( "MiniSoldierSelectWnd", "XPos", m_iDefaultX ,POS_SAVE_VERSION );
	g_GUIMgr.GUIPosSaveInt( "MiniSoldierSelectWnd", "YPos", m_iDefaultY ,POS_SAVE_VERSION );
}

void MiniSoldierSelectWnd::iwm_create()
{
	int iXPos = g_GUIMgr.GUIPosLoadInt( "MiniSoldierSelectWnd", "XPos", GetXPos() ,POS_SAVE_VERSION );
	int iYPos = g_GUIMgr.GUIPosLoadInt( "MiniSoldierSelectWnd", "YPos", GetYPos() ,POS_SAVE_VERSION );

	if( COMPARE( iXPos, 0, Setting::Width() ) && 
		COMPARE( iYPos, 0, Setting::Height() ) )
	{
		SetWndPos( iXPos, iYPos );
	}

	m_iDefaultX = GetXPos();
	m_iDefaultY = GetYPos();
}

void MiniSoldierSelectWnd::iwm_show()
{
	SettingMiniSoldierBtn();

	for( int i=ID_SOLDIER_BTN1; i<ID_SOLDIER_MAX; ++i )
	{
		ShowChildWnd(i);
	}
}

void MiniSoldierSelectWnd::iwm_hide()
{
	for( int i=ID_SOLDIER_BTN1; i<ID_SOLDIER_MAX; ++i )
	{
		HideChildWnd(i);
	}

	for( int i=ID_BUTTON_PAGE_UP; i<ID_BUTTON_MAX; ++i )
	{
		HideChildWnd(i);
	}
}

void MiniSoldierSelectWnd::iwm_mouseover( const ioMouse& mouse )
{
	ioMovingWnd::iwm_mouseover( mouse );

	if( SkipRender() )
		return;

	if(!HasWndStyle( IWS_INACTIVE ) )
	{
		ioMovingWnd::GoToTop();
	}

	for( int i=ID_BUTTON_PAGE_UP; i<ID_BUTTON_MAX; ++i )
	{
		ShowChildWnd(i);
	}

	m_bSetMouseOver = true;
}

void MiniSoldierSelectWnd::iwm_mouseleave( const ioMouse& mouse )
{
	ioMovingWnd::iwm_mouseleave( mouse );

	if( SkipRender() )
		return;

	for( int i=ID_BUTTON_PAGE_UP; i<ID_BUTTON_MAX; ++i )
	{
		HideChildWnd(i);
	}

	m_bSetMouseOver = false;
}

void MiniSoldierSelectWnd::iwm_lbuttonup( const ioMouse& mouse )
{
	if( SkipRender() )
		return;

	if( !m_MoveWnd.MouseLUp( mouse.GetMousePos() ) )
	{
		// 용병 아이콘 마우스 입력
		for( int i=ID_SOLDIER_BTN1; i<ID_SOLDIER_MAX; ++i )
		{
			MiniSoldierBtn *pBtn = dynamic_cast<MiniSoldierBtn*>( FindChildWnd( i ) );
			if( pBtn && pBtn->IsInWndRect( mouse.GetMousePos() ) )
			{
				iwm_command( pBtn, IOBN_BTNUP, ID_BTNUP_DONT_SETTING );
			}
		}

		// 페이지 마우스 입력
		for( int i=ID_BUTTON_PAGE_UP; i<ID_BUTTON_MAX; ++i )
		{
			ioButton *pBtn = dynamic_cast<ioButton*>( FindChildWnd( i ) );
			if( pBtn && pBtn->IsInWndRect( mouse.GetMousePos() ) )
			{
				iwm_command( pBtn, IOBN_BTNUP, ID_BTNUP_DONT_SETTING );
			}
		}
	}
	ioMovingWnd::iwm_lbuttonup( mouse );
}

void MiniSoldierSelectWnd::iwm_wheel( int zDelta )
{
	if( SkipRender() )
		return;

	if( !IsShow() )
		ioWnd::iwm_wheel( zDelta );
	else
	{ 
		if( zDelta == WHEEL_DELTA )
		{
			ioButton *pBtn = dynamic_cast<ioButton*>( FindChildWnd( ID_BUTTON_PAGE_UP ) );
			if( pBtn )
			{
				iwm_command( pBtn, IOBN_BTNUP, ID_BTNUP_DONT_SETTING );
			}
		}
		else if( zDelta == -WHEEL_DELTA )
		{
			ioButton *pBtn = dynamic_cast<ioButton*>( FindChildWnd( ID_BUTTON_PAGE_DN ) );
			if( pBtn )
			{
				iwm_command( pBtn, IOBN_BTNUP, ID_BTNUP_DONT_SETTING );
			}
		}
	}
}

void MiniSoldierSelectWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();

	switch( dwID )
	{
	case ID_SOLDIER_BTN1:
	case ID_SOLDIER_BTN2:
	case ID_SOLDIER_BTN3:
	case ID_SOLDIER_BTN4:
	case ID_SOLDIER_BTN5:
	case ID_SOLDIER_BTN6:
		if( cmd == IOBN_BTNUP )
		{
			int iSlotIdx = ( dwID - ID_SOLDIER_BTN1 ) + ( m_iCurPage * ID_SOLDIER_MAX );
			int iArray   = g_MyInfo.GetCharSlotIndexToArray( iSlotIdx );
			if( !g_MyInfo.IsCharActive( iArray ) )
				iArray = -1;

			if( iArray != -1 )
				SendCharChange( iArray );
		}
		break;
	case ID_BUTTON_PAGE_UP:
		if( cmd == IOBN_BTNUP )
		{
			m_iCurPage--;
			m_iCurPage = max( 0, m_iCurPage );
			SettingMiniSoldierBtn();
		}
		break;
	case ID_BUTTON_PAGE_DN:
		if( cmd == IOBN_BTNUP )
		{
			m_iCurPage++;
			int iCurMaxSlot = g_MyInfo.GetCurMaxCharSlot();
			if( iCurMaxSlot < ( m_iCurPage + 1 ) * MiniSoldierSelectWnd::ID_SOLDIER_MAX )
				m_iCurPage--;
			SettingMiniSoldierBtn();
		}
		break;
	}
}

void MiniSoldierSelectWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() )
		return;

	//////////////////////////////////////////////////////////////////////////
	if( !g_GUIMgr.IsSkipUIByBlind() && m_ShowAniState.GetCurState() == SAT_NONE )
		m_ShowAniState.ChangeState( SAT_END  );

	if( !g_GUIMgr.IsSkipUIByBlind() && m_ShowAniState.GetCurState() == SAT_START_ROUND )
	{
		if( g_TutorialMgr.IsShowSkillStep() )
		{
			m_ShowAniState.ChangeState( SAT_DELAY );
		}
	}

	if( g_GUIMgr.IsSkipUIByBlind() && m_ShowAniState.GetCurState() == SAT_END )
		m_ShowAniState.ChangeState( SAT_NONE );

	m_ShowAniState.UpdateTime();

	if( m_ShowAniState.GetCurState() == SAT_UP )
	{
		enum { PLUS_Y_POS = 75 };
		int iStartYPos = PLUS_Y_POS + ( Setting::Height() - ( m_iDefaultY + GetHeight() ) );
		int iCurYpos = m_iDefaultY + ( cos( (PI/2 * m_ShowAniState.GetCurTime()  ) / m_ShowAniState.GetDelay() ) * iStartYPos );	
		SetWndPos( m_iDefaultX, iCurYpos );
	}
	else if( m_ShowAniState.GetCurState() == SAT_DOWN )
	{
		enum { MAX_OFFSET_YPOS = 11 };
		int iCurYpos = m_iDefaultY + ( sin( (PI/2 * m_ShowAniState.GetCurTime()  ) / m_ShowAniState.GetDelay() ) * MAX_OFFSET_YPOS );	
		SetWndPos( m_iDefaultX, iCurYpos );
	}
	else if( m_ShowAniState.GetCurState() == SAT_UP_STOP )
	{
		enum { MAX_OFFSET_YPOS = 11 };
		int iCurYpos = m_iDefaultY + ( cos( (PI/2 * m_ShowAniState.GetCurTime()  ) / m_ShowAniState.GetDelay() ) * MAX_OFFSET_YPOS );	
		SetWndPos( m_iDefaultX, iCurYpos );
	}
	else if( m_ShowAniState.GetCurState() == SAT_STOP )
	{
		SetWndPos( m_iDefaultX, m_iDefaultY );
		m_ShowAniState.ChangeState( SAT_END );
	}
	//////////////////////////////////////////////////////////////////////////

	if( SkipRender() )
		return;

	ioMovingWnd::OnProcess( fTimePerSec );

	if( m_bChangeResolution )
	{
		m_bChangeResolution = false;
		ReSetWindow();
	}

	UpdateDefaultPos();
	CheckApplyToolTip();
}

void MiniSoldierSelectWnd::OnRender()
{
	if( SkipRender() )
		return;

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	if( m_bOver )
	{
		g_FontMgr.SetAlignType( TAT_CENTER );
		g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextColor( 220, 220, 220 );
		char szPage[MAX_PATH] = "";
		wsprintf_e( szPage, "%d", m_iCurPage+1 );
		g_FontMgr.PrintText( iXPos+261, iYPos+11, FONT_SIZE_12, szPage );
	}

	ioMovingWnd::OnRender();
}

void MiniSoldierSelectWnd::CheckApplyToolTip()
{
	if( SkipRender() || !m_bSetMouseOver )
		return;

	// + Btn
	ioLSMouse *pMouse = g_App.GetMouse();

	POINT pt;
	pt.x = pMouse->GetMousePos().x;
	pt.y = pMouse->GetMousePos().y;

	ioWnd *pWnd = NULL;

	for( int i=ID_SOLDIER_BTN1; i<ID_SOLDIER_MAX; ++i )
	{
		int iSlotIdx = ( i - ID_SOLDIER_BTN1 ) + ( m_iCurPage * ID_SOLDIER_MAX );
		int iArray = g_MyInfo.GetCharSlotIndexToArray( iSlotIdx );
		if( !g_MyInfo.IsCharActive( iArray ) )
			iArray = -1;

		int iClassType = -1;

		if( iArray != -1 )
		{
			iClassType = g_MyInfo.GetClassType( iArray );

			pWnd = FindChildWnd( i );
			if( pWnd && pWnd->IsInWndRect( pt ) )
			{
				MiniSoldierBtn *pMiniSoldier = (MiniSoldierBtn*)FindChildWnd( i );
				if( pMiniSoldier )
				{
					if( pMouse && pMouse->IsMouseShowing() )
						pMouse->SetToolTipID( g_MyInfo.GetPublicID(), iClassType );
				}
			}
		}
	}
}

void MiniSoldierSelectWnd::SendCharChange(int iArray )
{
	if( !m_pPlayStage )
		return;

	ioBaseChar *pOwner = m_pPlayStage->GetOwnerChar();
	if( !pOwner )	return;

	if( m_pPlayStage->IsNetworkMode() )
	{
		if( !pOwner->IsChangeWaitState() )
		{
			if( pOwner->IsCanChangeCharState( true ) )
			{
				if( pOwner->GetExperienceMode() == EMS_CHANGE_EXPERIENCE )
					return;

				pOwner->SetChangeWaitState( iArray );
			}
		}
	}
	else // 훈련소
	{
		if( pOwner->IsCanChangeCharState( true ) )
		{
			if( pOwner->GetExperienceMode() == EMS_CHANGE_EXPERIENCE )
				return;

			SP2Packet kPacket( CTPK_CHANGE_SINGLE_CHAR );
			kPacket << pOwner->GetIndex();
			kPacket << g_MyInfo.GetCharIndex( iArray );
			TCPNetwork::SendToServer( kPacket );
			TCPNetwork::MouseBusy( true );

			ioItem *pItem = pOwner->GetEquipedItem(ES_WEAPON);
			ITEM_DATA &rkItem = g_MyInfo.GetCharWeaponItem();
			if( ToBowItem(pItem)			||
				ToFireWeaponItem(pItem)		||
				ToLaserItem(pItem)			||
				ToAutoShotItem(pItem)		||
				ToUroborusItem(pItem)		||
				ToWereWolfItem(pItem)		||
				ToWitchItem(pItem)			||
				ToNakoruruItem(pItem)		||
				ToExcaliburItem(pItem)		||
				ToChargeComboBowItem(pItem) ||
				ToWestGunExtendItem(pItem)	||
				ToPeterpanItem(pItem)		||
				ToWindLordItem(pItem)		||
				ToProject_K_Item(pItem)		||
				ToPsychoMetryItem(pItem)	||
				ToSuddenAttackItem(pItem)	||
				ToTitanWeaponItem(pItem)	||
				ToSuperDimensionItem(pItem) ||
				ToTraceItem(pItem)			||
				ToKaelthasItem(pItem)		||
				ToStrikerItem(pItem)		||
				ToLeeItem(pItem)			||
				ToParkilpyoItem(pItem)		||
				ToJoroExpansionItem(pItem)	||
				ToEarthQuakeItem(pItem)		||
				ToChargeCommandItem(pItem)	||
				ToBullFightItem(pItem)		||
				ToStriderItem( pItem )		||
				ToOniMushaItem( pItem )		||
				ToSasinmuItem(pItem)		||
				ToPredatorItem(pItem)  )
			{
				if( rkItem.m_item_code == pItem->GetItemCode() )
					rkItem.m_item_bullet = pItem->GetCurBullet();
				else
					rkItem.m_item_bullet = -1;
			}
			else
			{
				rkItem.m_item_bullet = -1;
			}

			pOwner->SetChangeWaitState( iArray, false );
		}
	}
}

void MiniSoldierSelectWnd::SetPlayStage( ioPlayStage * pStage )
{
	m_pPlayStage = pStage;
}

void MiniSoldierSelectWnd::UpdateByOwner( ioBaseChar *pOwner )
{
	if( !pOwner )	return;
	
	m_eOwnerState = pOwner->GetState();
}

void MiniSoldierSelectWnd::ChangeSoldierSlot( int iLeftArray, int iRightArray )
{
	SettingMiniSoldierBtn();
}

void MiniSoldierSelectWnd::AddMySoldier( DWORD dwIndex )
{
	SettingMiniSoldierBtn();
}

void MiniSoldierSelectWnd::SettingMiniSoldierBtn()
{
	bool bForceUpdateChar = IsShow();

	for( int i=ID_SOLDIER_BTN1; i<ID_SOLDIER_MAX; i++ )
	{
		MiniSoldierBtn *pBtn = (MiniSoldierBtn*)FindChildWnd(i);
		if( !pBtn )	continue;

		int iSlotIdx = ( i - ID_SOLDIER_BTN1 ) + ( m_iCurPage * ID_SOLDIER_MAX );
		int iArray   = g_MyInfo.GetCharSlotIndexToArray( iSlotIdx );;
		
		if( !g_MyInfo.IsCharActive( iArray ) )
			iArray = -1;

		pBtn->SetCharacterCount( iArray, iSlotIdx );
	}
}

void MiniSoldierSelectWnd::ReSetWindow()
{
	ChangeWnd();
	SettingMiniSoldierBtn();

	DWORD dwWndStyle = GetWndStyle();

	SetWndStyle( dwWndStyle );
}

void MiniSoldierSelectWnd::SetPositionToDefault( int iParentWidth, int iParentHeight, bool bChildMove, bool bCreate /*= false */ )
{
	float fPosGap = ( Setting::Height() - 600.0f ) / FLOAT10;

	int iTempDefYPos = m_iDefYPos;

	m_iDefYPos = iTempDefYPos - fPosGap;

	ioMovingWnd::SetPositionToDefault( iParentWidth, iParentHeight, bChildMove, bCreate );

	m_iDefYPos = iTempDefYPos;

	if( !bCreate )
	{
		m_iDefaultX = GetXPos();
		m_iDefaultY = GetYPos();
		SetWndPos( m_iDefaultX, m_iDefaultY );
		m_bChangeResolution = true;
	}

	/*ioMovingWnd::SetPositionToDefault( iParentWidth, iParentHeight, bChildMove, bCreate );
	if( !bCreate )
	{
		// ResetWindow시 crash 발생하여 OnProcess에서 처리함.
		m_bChangeResolution = true;
	}*/
}

void MiniSoldierSelectWnd::UpdateDefaultPos()
{
	if(  g_GUIMgr.IsDragMove( m_dwID )  || IsMoving() )
	{
		if( m_iDefaultX != GetXPos() )
		{
			if( COMPARE( GetXPos(), 0, (Setting::Width() - GetWidth() ) + 1 ))
				m_iDefaultX = GetXPos();
		}

		if( m_iDefaultY != GetYPos() )
		{
			if( COMPARE( GetYPos(), 0, (Setting::Height() - GetHeight() ) + 1 ))
				m_iDefaultY = GetYPos();
		}
	}
}

void MiniSoldierSelectWnd::ChangeWnd()
{
	m_iCurPage = 0;
	SettingMiniSoldierBtn();

	/*int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();*/
	int iXPos = GetXPos();
	int iYPos = GetYPos();

	ioMovingWnd::SetWndPos( iXPos, iYPos );
}

bool MiniSoldierSelectWnd::SkipRender()
{
	bool bSkipRender = false;

	if( Setting::IsHideMiniSoldierSelectSlot() )
		bSkipRender = true;

	if( g_GUIMgr.IsSkipUIByBlind() )
		bSkipRender = true;

	if( m_ShowAniState.GetCurState() == SAT_DELAY || 
		m_ShowAniState.GetCurState() == SAT_START_ROUND )
		bSkipRender = true;

	if( m_eOwnerState == CS_DIE || 
		m_eOwnerState == CS_READY ||
		m_eOwnerState == CS_VIEW ||
		m_eOwnerState == CS_OBSERVER )
		bSkipRender = true;

	if( !g_TutorialMgr.IsShowSkillStep() )
		bSkipRender = true;

	if( bSkipRender )
	{
		HideChildWnd( ID_BUTTON_PAGE_UP );
		HideChildWnd( ID_BUTTON_PAGE_DN );
	}

	return bSkipRender;
}


void MiniSoldierSelectWnd::ChangeShowAniState( ShowAniType eShowAniType )
{
	m_ShowAniState.ChangeState( (int) eShowAniType );
}


void MiniSoldierSelectWnd::SetCurModeType( ModeType eModeType )
{
	m_eModeType = eModeType;
}


void MiniSoldierSelectWnd::InitShowAniState()
{
	enum { DELAY_DELAY = 165, DELAY_UP = 198, DELAY_DOWN = 66, DELAY_UP_STOP = 66, };

	m_ShowAniState.SetCurState( SAT_NONE );

	FSMState *pState = new FSMState(SAT_NONE);
	if( pState )
	{
		pState->Add( SAT_DELAY,  SAT_DELAY );
		pState->Add( SAT_START_ROUND, SAT_START_ROUND );
		pState->Add( SAT_END,  SAT_END );
	}
	m_ShowAniState.Add( pState );

	pState = new FSMState(SAT_START_ROUND);
	if( pState )
		pState->Add( SAT_DELAY,  SAT_DELAY);
	m_ShowAniState.Add( pState );	

	pState = new FSMState(SAT_DELAY);
	if( pState )
		pState->Add( SAT_DELAY,  SAT_UP , DELAY_DELAY);
	m_ShowAniState.Add( pState );	

	pState = new FSMState(SAT_UP);
	if( pState )
		pState->Add( SAT_UP,  SAT_DOWN , DELAY_UP);
	m_ShowAniState.Add( pState );

	pState = new FSMState(SAT_DOWN);
	if( pState )
		pState->Add( SAT_DOWN,  SAT_UP_STOP , DELAY_DOWN );
	m_ShowAniState.Add( pState );

	pState = new FSMState(SAT_UP_STOP);
	if( pState )
		pState->Add( SAT_UP_STOP,  SAT_STOP , DELAY_UP_STOP );
	m_ShowAniState.Add( pState );

	pState = new FSMState(SAT_STOP);
	if( pState )
		pState->Add( SAT_END,  SAT_END );
	m_ShowAniState.Add( pState );

	pState = new FSMState(SAT_END);
	if( pState )
	{
		pState->Add( SAT_NONE,  SAT_NONE );
		pState->Add( SAT_START_ROUND, SAT_START_ROUND );
	}
	m_ShowAniState.Add( pState );
}


int MiniSoldierSelectWnd::GetCurState() const
{
	return m_ShowAniState.GetCurState();
}