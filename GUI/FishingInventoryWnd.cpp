#include "StdAfx.h"

#include "../io3DEngine/ioWnd.h"
#include "../io3DEngine/ioScroll.h"
#include "../io3DEngine/ioUITitle.h"
#include "../io3DEngine/ioUIRenderImage.h"
#include "../io3DEngine/ioUIImageSetManager.h"
#include "../io3DEngine/ioFontManager.h"
#include "../io3DEngine/ioXMLDocument.h"
#include "../io3DEngine/ioXMLElement.h"

#include "../Setting.h"
#include "../TextColorTable.h"
#include "../PlayEntityHelpStruct.h"
#include "../NetworkWrappingFunc.h"

#include "../ioMyInfo.h"
#include "../ioApplication.h"
#include "../ioPlayStage.h"
#include "../ioBaseChar.h"
#include "../ioFishingManager.h"
#include "../ioMyLevelMgr.h"

#include "../ioEtcItem.h"
#include "../ioEtcItemManager.h"

#include "../WndID.h"

#include "ioSP2GUIManager.h"
#include "NewShopWnd.h"
#include "GlobalWnd.h"
#include "ioFishingMessageBox.h"
#include "FishingInventoryWnd.h"

#include <strsafe.h>
#include "../io3DEngine/iostringmanager.h"
#include "../io3DEngine/Safesprintf.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FishingItemInfoWnd::FishingItemInfoWnd()
{
	m_iItemType = -1;
	m_iSlotNum = -1;
	m_pIcon = NULL;
	m_pBack = NULL;
}

FishingItemInfoWnd::~FishingItemInfoWnd()
{
	SAFEDELETE(m_pIcon);
	SAFEDELETE(m_pBack);
}

void FishingItemInfoWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "SlotBack" )
	{
		SAFEDELETE(m_pBack);
		m_pBack = pImage;
	}
	else
	{
		ioWnd::AddRenderImage( szType, pImage );
	}
}

void FishingItemInfoWnd::SetInfo( int iItemType, int iSlotNum )
{
	m_iItemType = iItemType;
	m_iSlotNum = iSlotNum;

	if( m_iItemType < 0 )
	{
		SAFEDELETE( m_pIcon );
		SetInActive();
	}
	else
	{
		SAFEDELETE( m_pIcon );
		m_pIcon = g_UIImageSetMgr.CreateImageByFullName(g_FishingMgr.GetFishingItemIconName(m_iItemType));
		
		SetActive();
	}
}

void FishingItemInfoWnd::OnRender()
{
	ioButton::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	if( HasWndStyle( IWS_INACTIVE ) )
	{
		if( m_iSlotNum <= g_MyInfo.GetCurMaxFishingInventory() )
		{
			if( m_pBack )
			{
				m_pBack->Render( iXPos, iYPos, UI_RENDER_NORMAL, TFO_BILINEAR );
			}
			g_FontMgr.SetBkColor( 0, 0, 0 );
			g_FontMgr.SetTextStyle( TS_NORMAL );
			g_FontMgr.SetAlignType( TAT_CENTER );
			g_FontMgr.SetTextColor( 49, 70, 91 );
			g_FontMgr.PrintText( iXPos + 21, iYPos + 14, FONT_SIZE_12, STR(1), m_iSlotNum );
		}
	}
	else if( m_pIcon )
	{
		m_pIcon->SetScale( 0.65f );
		m_pIcon->Render( iXPos + 21, iYPos + 21, UI_RENDER_NORMAL, TFO_BILINEAR );

		if( IsOver() )
		{
			m_pIcon->Render( iXPos + 21, iYPos + 21, UI_RENDER_SCREEN, TFO_BILINEAR );
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
FishingItemPageBtn::FishingItemPageBtn()
{
	m_pActiveImage = NULL;
	m_pInActiveImage = NULL;
}

FishingItemPageBtn::~FishingItemPageBtn()
{
	SAFEDELETE(m_pActiveImage);
	SAFEDELETE(m_pInActiveImage);
}

void FishingItemPageBtn::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "ActiveImage" )
	{
		SAFEDELETE(m_pActiveImage);
		m_pActiveImage = pImage;
	}
	else if( szType == "InActiveImage" )
	{
		SAFEDELETE(m_pInActiveImage);
		m_pInActiveImage = pImage;
	}
	else
	{
		ioWnd::AddRenderImage( szType, pImage );
	}
}

void FishingItemPageBtn::OnRender()
{
	ioButton::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	if( HasWndStyle( IWS_INACTIVE ) )
	{
		if(m_pInActiveImage)
		{
			m_pInActiveImage->Render( iXPos+6, iYPos+9, UI_RENDER_NORMAL );
		}
	}
	else
	{
		if(m_pActiveImage)
		{
			m_pActiveImage->Render( iXPos+6, iYPos+9, UI_RENDER_NORMAL );

			if( IsOver() )
			{
				m_pActiveImage->Render( iXPos+6, iYPos+9, UI_RENDER_SCREEN );
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////
FishingExpWnd::FishingExpWnd()
{
	m_pExpGauge = NULL;

	m_iCurLevel = 0;
	m_iCurExp = 0;
}

FishingExpWnd::~FishingExpWnd()
{
	SAFEDELETE(m_pExpGauge);
}

void FishingExpWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "Exp_Gauge" )
	{
		SAFEDELETE(m_pExpGauge);
		m_pExpGauge = pImage;
	}
	else
	{
		ioWnd::AddRenderImage( szType, pImage );
	}
}

void FishingExpWnd::SetInfo( int iLevel, int iExp )
{
	m_iCurLevel = iLevel;
	m_iCurExp = iExp;
}

void FishingExpWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	float fExpRate = 0.0f;
	int iMaxExp = g_LevelMgr.GetNextFishingLevelUpExp( m_iCurLevel );
	if( iMaxExp != 0 )
		fExpRate = (float)m_iCurExp / (float)iMaxExp;

	if( m_pExpGauge )
	{
		int iWidth = m_pExpGauge->GetWidth();
		iWidth *= fExpRate;
		m_pExpGauge->RenderWidthCut( iXPos + 2, iYPos + 2, 0, iWidth );
	}

	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetAlignType( TAT_RIGHT );
	g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
	g_FontMgr.PrintText( iXPos - 6, iYPos - 3, FONT_SIZE_12, STR(1), m_iCurLevel );

	g_FontMgr.SetTextColor( TCT_DEFAULT_BLUE );
	g_FontMgr.PrintText( iXPos + 234, iYPos - 3, FONT_SIZE_12, STR(2), (int)( fExpRate * FLOAT100 ) );

}
////////////////////////////////////////////////////////////////////////////////////////////
FishingScreenBtn::FishingScreenBtn()
{
	m_bScreen = false;
	m_bFishing = false;
	m_dwScreenStartTime = 0;
}

FishingScreenBtn::~FishingScreenBtn()
{
}

void FishingScreenBtn::ParseExtraInfo( ioXMLElement &xElement )
{
	ioButton::ParseExtraInfo( xElement );
	m_dwScreenTimer = xElement.GetIntAttribute_e( "ScreenTime" );
}

void FishingScreenBtn::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;

	ProcessScreenEffect();
}

void FishingScreenBtn::OnDrawNormal( int iXPos, int iYPos )
{
	if( m_bScreen && !m_bFishing )
	{
		ioButton::OnDrawOvered( iXPos, iYPos );
	}
	else
	{
		ioButton::OnDrawNormal( iXPos, iYPos );
	}
}

void FishingScreenBtn::SetFishingState( bool bFishing )
{
	m_bFishing = bFishing;

	if( !m_bFishing )
		SetTitleText( STR(1) );
	else
		SetTitleText( STR(2) );
}

void FishingScreenBtn::ProcessScreenEffect()
{
	if( m_dwScreenStartTime == 0 )
		m_dwScreenStartTime = FRAMEGETTIME();

	if( m_bScreen )
	{
		if( FRAMEGETTIME() - m_dwScreenStartTime > m_dwScreenTimer )
		{
			m_dwScreenStartTime = FRAMEGETTIME();
			m_bScreen      = false;
		}
	}	
	else
	{
		if( FRAMEGETTIME() - m_dwScreenStartTime > m_dwScreenTimer )
		{
			m_dwScreenStartTime = FRAMEGETTIME();
			m_bScreen      = true;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
FishingBaitListWnd::FishingBaitListWnd()
{
	m_pDarkLine	= NULL;
	m_pLightLine= NULL;
	m_pOverLine	= NULL;

	m_pOpenBtn  = NULL;
	m_iAddWidth	= 0;
	m_iAddHeight= 0;
	m_iListXPos = 0;
	m_iListYPos = 0;
	m_iStartXPos= 0;
	m_iStartYPos= 0;
	m_iHeightGap= 0;
	m_iOverListIndex = -2;
}

FishingBaitListWnd::~FishingBaitListWnd()
{
	SAFEDELETE( m_pDarkLine	);
	SAFEDELETE( m_pLightLine );
	SAFEDELETE( m_pOverLine	);

	m_vBaitList.clear();
}

void FishingBaitListWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	m_iAddWidth	= xElement.GetIntAttribute_e( "AddWidth" );
	m_iAddHeight= xElement.GetIntAttribute_e( "AddHeight" );
	m_iListXPos = xElement.GetIntAttribute_e( "ListX" );
	m_iListYPos = xElement.GetIntAttribute_e( "ListY" );
	m_iHeightGap= xElement.GetIntAttribute_e( "HeightGap" );
}

void FishingBaitListWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "DarkLine" )
	{
		SAFEDELETE( m_pDarkLine );
		m_pDarkLine = pImage;
	}
	else if( szType == "LightLine" )
	{
		SAFEDELETE( m_pLightLine );
		m_pLightLine = pImage;
	}
	else if( szType == "OverLine" )
	{
		SAFEDELETE( m_pOverLine );
		m_pOverLine = pImage;
	}
	else
	{
		ioWnd::AddRenderImage( szType, pImage );
	}
}

bool FishingBaitListWnd::IsEmptyList()
{
	CheckBaitList();

	return m_vBaitList.empty();
}

void FishingBaitListWnd::CheckBaitList()
{
	m_vBaitList.clear();

	ioUserEtcItem *pUserEtcItem = g_MyInfo.GetUserEtcItem();
	if( !pUserEtcItem )
		return;

	ioUserEtcItem::ETCITEMSLOT kBaitSlot;

	if( pUserEtcItem->GetEtcItem( ioEtcItem::EIT_ETC_FISHING_BAIT, kBaitSlot ) )
	{
		ioEtcItem *pEtcItem = g_EtcItemMgr.FindEtcItem( ioEtcItem::EIT_ETC_FISHING_BAIT );
		if( pEtcItem )
		{
			BaitList kList;
			kList.m_iIndex = pEtcItem->GetType();
			if( !pEtcItem->GetExtraName().IsEmpty() )
				kList.m_szTitle= pEtcItem->GetExtraName();
			else
				kList.m_szTitle= pEtcItem->GetName();
			kList.m_iCount = kBaitSlot.GetUse();
			m_vBaitList.push_back( kList );
		}
	}

	if( pUserEtcItem->GetEtcItem( ioEtcItem::EIT_ETC_FISHING_MOON_BAIT, kBaitSlot ) )
	{
		ioEtcItem *pEtcItem = g_EtcItemMgr.FindEtcItem( ioEtcItem::EIT_ETC_FISHING_MOON_BAIT );
		if( pEtcItem )
		{
			BaitList kList;
			kList.m_iIndex = pEtcItem->GetType();
			if( !pEtcItem->GetExtraName().IsEmpty() )
				kList.m_szTitle= pEtcItem->GetExtraName();
			else
				kList.m_szTitle= pEtcItem->GetName();
			kList.m_iCount = kBaitSlot.GetUse();
			m_vBaitList.push_back( kList );
		}
	}

	if( pUserEtcItem->GetEtcItem( ioEtcItem::EIT_ETC_PCROOM_FISHING_BAIT, kBaitSlot ) )
	{
		ioEtcItem *pEtcItem = g_EtcItemMgr.FindEtcItem( ioEtcItem::EIT_ETC_PCROOM_FISHING_BAIT );
		if( pEtcItem )
		{
			BaitList kList;
			kList.m_iIndex = pEtcItem->GetType();
			if( !pEtcItem->GetExtraName().IsEmpty() )
				kList.m_szTitle= pEtcItem->GetExtraName();
			else
				kList.m_szTitle= pEtcItem->GetName();
			kList.m_iCount = kBaitSlot.GetUse();
			m_vBaitList.push_back( kList );
		}
	}
}

void FishingBaitListWnd::OpenList( ioWnd *pOpenBtn )
{
	if( IsShow() || !pOpenBtn )
	{
		HideWnd();
		return;
	}

	m_pOpenBtn			= pOpenBtn;
	m_iStartXPos		= m_pOpenBtn->GetXPos();
	m_iStartYPos		= m_pOpenBtn->GetYPos();	
	ShowWnd();
}

void FishingBaitListWnd::iwm_show()
{
	SetCheckParentOutSide( true );

	CheckBaitList();

	if( !m_pOpenBtn )
		return;

	// 사이즈 및 결정
	SetSize( GetWidth(), m_iAddHeight + ( (DWORD)m_vBaitList.size() * m_iHeightGap ) );
	SetWndPos( m_iStartXPos, m_iStartYPos - GetHeight() );
}

void FishingBaitListWnd::iwm_hide()
{
	SetCheckParentOutSide( false );
}

void FishingBaitListWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() )
		return;

	m_iOverListIndex  = -2;

	// 다른 곳에서 클릭이 일어났는지 확인
	ioWnd *pPreClick = g_GUIMgr.GetPreClickWnd();
	if( pPreClick )
	{
		if( pPreClick != m_pOpenBtn && pPreClick != this )
		{
			HideWnd();
			return;
		}
	}

	int iXPos, iYPos;
	iXPos = GetDerivedPosX();
	iYPos = GetDerivedPosY();

	ioLSMouse *pMouse = g_App.GetMouse();

	int iListSize = m_vBaitList.size();
	for(int i = 0;i < iListSize;i++)
	{
		RECT rcNameRect;
		rcNameRect.left = iXPos + m_iListXPos;
		rcNameRect.top  = iYPos + m_iListYPos + i * m_iHeightGap;
		rcNameRect.right  = rcNameRect.left + ( GetWidth() - ( m_iListXPos * 2 ) );
		rcNameRect.bottom = rcNameRect.top + m_iHeightGap;

		if( ::PtInRect( &rcNameRect, pMouse->GetMousePos() ) )
		{
			BaitList &kList = m_vBaitList[i];
			m_iOverListIndex  = kList.m_iIndex;
			if( pMouse->IsLBtnUp() )
			{
				SetParentInfo( m_iOverListIndex );
				HideWnd();
			}
			break;
		}
	}
}

void FishingBaitListWnd::SetParentInfo( int iType )
{
	if( !GetParent() )
	{
		LOG.PrintTimeAndLog( 0, "%s Parent == NULL.", __FUNCTION__ );
		return;
	}

	FishingInventoryWnd *pWnd = dynamic_cast<FishingInventoryWnd*> ( GetParent() );
	if( !pWnd )
	{
		LOG.PrintTimeAndLog( 0, "%s Parent Wnd == NULL.", __FUNCTION__ );
		return;
	}

	pWnd->SetBaitType( iType );
	HideWnd();
}

void FishingBaitListWnd::OnRender()
{
	ioWnd::OnRender();

	if( !m_pDarkLine || !m_pLightLine || !m_pOverLine ) 
		return;

	g_FontMgr.SetAlignType( TAT_LEFT );
	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetBkColor( 0, 0, 0 );

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	bool bDarkRender = false;
	int iListSize = m_vBaitList.size();
	for(int i = 0;i < iListSize;i++)
	{
		const BaitList &rkSelectList = m_vBaitList[i];
		bDarkRender = !bDarkRender;

		if( m_iOverListIndex == rkSelectList.m_iIndex )
		{
			m_pOverLine->Render( iXPos, iYPos + ( i * m_iHeightGap ) );
			g_FontMgr.SetTextColor( 255, 255, 255 );
		}
		else if( bDarkRender )
		{
			m_pDarkLine->Render( iXPos, iYPos + ( i * m_iHeightGap ) );
			g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
		}
		else
		{
			m_pLightLine->Render( iXPos, iYPos + ( i * m_iHeightGap ) );
			g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
		}

		if (rkSelectList.m_iIndex == ioEtcItem::EIT_ETC_PCROOM_FISHING_BAIT)
			g_FontMgr.PrintTextWidthCut( iXPos + m_iListXPos + 5, iYPos + m_iListYPos + ( i * m_iHeightGap ) + 3, FONT_SIZE_12, 96.0f, STR(1), rkSelectList.m_szTitle.c_str() );
		else
			g_FontMgr.PrintTextWidthCut( iXPos + m_iListXPos + 5, iYPos + m_iListYPos + ( i * m_iHeightGap ) + 3, FONT_SIZE_12, 96.0f, STR(1), rkSelectList.m_szTitle.c_str(), rkSelectList.m_iCount );
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////
FishingInventoryWnd::FishingInventoryWnd()
{
	m_pPlayStage = NULL;

	m_iCurPage = 0;
	m_iCurPageCnt = 1;

	m_iBaitCnt = 0;
}

FishingInventoryWnd::~FishingInventoryWnd()
{
	if( !COMPARE( m_iDefaultX, 0, Setting::Width() ) || 
		!COMPARE( m_iDefaultY, 0, Setting::Height() ) )
		return;

	g_GUIMgr.GUIPosSaveInt( "FishingInventoryWnd", "XPos", m_iDefaultX ,POS_SAVE_VERSION );
	g_GUIMgr.GUIPosSaveInt( "FishingInventoryWnd", "YPos", m_iDefaultY ,POS_SAVE_VERSION );
}

void FishingInventoryWnd::iwm_create()
{
	int iXPos = g_GUIMgr.GUIPosLoadInt( "FishingInventoryWnd", "XPos", GetXPos() ,POS_SAVE_VERSION );
	int iYPos = g_GUIMgr.GUIPosLoadInt( "FishingInventoryWnd", "YPos", GetYPos() ,POS_SAVE_VERSION );

	if( COMPARE( iXPos, 0, Setting::Width() ) && 
		COMPARE( iYPos, 0, Setting::Height() ) )
	{
		SetWndPos( iXPos, iYPos );
	}

	m_iDefaultX = GetDerivedPosX();
	m_iDefaultY = GetDerivedPosY();
}

void FishingInventoryWnd::iwm_show()
{
	CheckPageData( true );
	UpdatePage();
	UpdateBait();
	UpdateExp();

#if defined( USE_GA )
	// PLAY_PLAZA_FISHING_START
	g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
		, "Play"
		, "Start"
		, ""
		, 1
		, "%2FPLAY%2FPLAZA%2FFISHING%2FSTART" );
#endif
}

void FishingInventoryWnd::iwm_hide()
{
	if( m_pPlayStage )
	{
		ioBaseChar *pOwner = m_pPlayStage->GetOwnerChar();
		if( pOwner )
		{
			pOwner->SetFishingEndState();
		}
	}
}

void FishingInventoryWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
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
	case ID_ITEM1_BTN:
	case ID_ITEM2_BTN:
	case ID_ITEM3_BTN: 
	case ID_ITEM4_BTN:
	case ID_ITEM5_BTN:
	case ID_ITEM6_BTN:
	case ID_ITEM7_BTN:
	case ID_ITEM8_BTN:
	case ID_ITEM9_BTN:
	case ID_ITEM10_BTN:
		if( cmd == IOBN_BTNUP )
		{
			if( !pWnd->HasWndStyle( IWS_INACTIVE ) )
			{
				UpdatePage();
				OnSetIconPush( dwID, false, 0 );
			}
		}
		break;
	case ID_ITEM_SELL_BTN:
		if( cmd == IOBN_BTNUP )
		{
			// 첫번째 것 판매
			if( g_MyInfo.GetCurFishingItemCnt() > 0 )
			{
				m_iCurPage = 0;
				UpdatePage();

				OnSetIconPush( ID_ITEM1_BTN, false, 0 );
			}
			else
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
			}
		}
		break;
	case ID_ITEM_FISHING_BTN:
		if( cmd == IOBN_BTNUP )
		{
			CheckFishingBtn();
		}
		break;
	case FISHING_MESSAGE_BOX:
		if( cmd == IOBN_BTNUP )
		{
			WORD wResult = LOWORD( param );
			if( wResult == IDYES )
			{
				SendItemSell( HIWORD( param ) );
			}
			else if( wResult == IDRETRY )
			{
				CheckNextItem( HIWORD( param ) );
			}
			else if( wResult == IDCANCEL )
			{
				HideWnd();
			}
		}
		break;
	case ID_PAGE_UP:
		if( cmd == IOBN_BTNUP )
		{
			if( !pWnd->HasWndStyle( IWS_INACTIVE ) )
			{
				m_iCurPage--;
				UpdatePage();
			}
		}
		break;
	case ID_PAGE_DOWN:
		if( cmd == IOBN_BTNUP )
		{
			if( !pWnd->HasWndStyle( IWS_INACTIVE ) )
			{
				m_iCurPage++;
				UpdatePage();
			}
		}
		break;
	case ID_BAIT_SELECT_BTN:
		if( cmd == IOBN_BTNUP )
		{
			FishingBaitListWnd *pListWnd = dynamic_cast<FishingBaitListWnd*>( FindChildWnd(ID_BAIT_LIST) );
			if( pListWnd )
			{
				if( pListWnd->IsEmptyList() )
				{
					NewShopWnd *pNewShopWnd = dynamic_cast<NewShopWnd*>( g_GUIMgr.FindWnd( NEW_SHOP_WND ) );
					if( pNewShopWnd )
					{
						pNewShopWnd->ShowWnd();
						pNewShopWnd->SetDirectTab( NewShopWnd::ID_ETC_TAB_BTN, ioEtcItem::EIT_ETC_FISHING_BAIT, 0 );
					}
				}
				else
				{
					pListWnd->OpenList( pWnd );
				}
			}
		}
		break;
	}
}

void FishingInventoryWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;
	if( !m_pPlayStage ) return;

	ioBaseChar *pOwner = m_pPlayStage->GetOwnerChar();
	if( pOwner )
	{
		FishingScreenBtn *pBtn = dynamic_cast<FishingScreenBtn*> ( FindChildWnd( ID_ITEM_FISHING_BTN ) );
		if( !pBtn ) return;

		if( pOwner->GetState() != CS_FISHING )
			pBtn->SetFishingState( false );
		else
			pBtn->SetFishingState( true );
	}

	UpdateDefaultPos();
	ProcessToolTip();
	ioMovingWnd::OnProcess( fTimePerSec );
}

void FishingInventoryWnd::SetPlayStage( ioPlayStage *pPlayStage )
{
	m_pPlayStage = pPlayStage;
}

void FishingInventoryWnd::CheckPageData( bool bFirst )
{
	int iCurMaxArray = g_MyInfo.GetCurMaxFishingInventory();
	int iCurItemCnt = g_MyInfo.GetCurFishingItemCnt();

	if( iCurMaxArray <= MAX_BTN )
	{
		m_iCurPageCnt = 1;
	}
	else
	{
		m_iCurPageCnt = iCurMaxArray / MAX_BTN;

		if( iCurMaxArray%MAX_BTN )
			++m_iCurPageCnt;
	};

	if( bFirst )
	{
		// 첫 페이지 보여줄 경우
		m_iCurPage = 0;
	}
	else
	{
		// 현재 페이지 고수
	}


	/*
	// 마지막 아이템 있는 페이지 보여줄 경우
	if( iCurItemCnt <= MAX_BTN )
	{
		m_iCurPage = 0;
	}
	else
	{
		m_iCurPage = iCurItemCnt / MAX_BTN;

		if( iCurItemCnt%MAX_BTN == 0 )
			--m_iCurPage;
	}

	if( m_iCurPage > 0 && m_iCurPage >= m_iCurPageCnt )
		m_iCurPage = m_iCurPageCnt - 1;
	*/
}

void FishingInventoryWnd::UpdatePage()
{
	int iCurMaxArray = g_MyInfo.GetCurMaxFishingInventory();
	int iStartArray = m_iCurPage * MAX_BTN;

	for( int i = 0 ; i < MAX_BTN ; i++ )
	{
		int iCurArray = iStartArray + i;
		UpdateBtns( iCurArray, ID_ITEM1_BTN + i );
	}

	// Page UP, DOWN 버튼
	if( m_iCurPage == 0 )
		SetChildInActive( ID_PAGE_UP );
	else
		SetChildActive( ID_PAGE_UP );

	if( m_iCurPage == (m_iCurPageCnt-1) )
		SetChildInActive( ID_PAGE_DOWN );
	else
		SetChildActive( ID_PAGE_DOWN );
}

void FishingInventoryWnd::UpdateBtns( int iArray , DWORD dwBtnID )
{
	FishingItemInfoWnd* pItem = dynamic_cast<FishingItemInfoWnd*> ( FindChildWnd( dwBtnID ) );
	if( !pItem )
		return;

	int iItemType = g_MyInfo.GetFishingItemType( iArray );
	pItem->SetInfo( iItemType, iArray+1 );
}

void FishingInventoryWnd::UpdateExp()
{
	FishingExpWnd* pExp = dynamic_cast<FishingExpWnd*> ( FindChildWnd( ID_FISHING_EXP_WND ) );
	if( !pExp ) return;

	pExp->SetInfo( g_MyInfo.GetFishingLevel(), g_MyInfo.GetFishingExpert() );
}

void FishingInventoryWnd::OnSetIconPush( DWORD dwID, bool bNextSel, int iStartArray )
{
	if( bNextSel )
	{
		if( g_MyInfo.GetFishingItemType( iStartArray ) == -1 )
		{
			iStartArray = 0;
		}
		// 판매 창으로 전달
		CheckMessageBox( iStartArray );
	}
	else
	{
		int iCurArray = dwID - ID_ITEM1_BTN;
		iStartArray = ( m_iCurPage * MAX_BTN ) + iCurArray;
		if( g_MyInfo.GetFishingItemType( iStartArray ) != -1 )
		{
			ioFishingMessageBox *pFishMessageBox = dynamic_cast<ioFishingMessageBox*>(g_GUIMgr.FindWnd(FISHING_MESSAGE_BOX));
			if( pFishMessageBox && pFishMessageBox->IsShow() )
			{
				pFishMessageBox->StopAutoSell();
			}
			// 판매 창으로 전달
			CheckMessageBox( iStartArray );
		}
	}
}

void FishingInventoryWnd::SendItemSell( int iSellItemArray )
{
	int iItemType = g_MyInfo.GetFishingItemType( iSellItemArray );
	int iIndexArray = g_MyInfo.GetFishingItemArray( iSellItemArray );

	if( iItemType != -1 )
	{
		SP2Packet kPacket( CTPK_FISHING );
		kPacket << FISHING_SELL;
		kPacket << iIndexArray;
		TCPNetwork::SendToServer( kPacket );

		g_App.SetMouseBusy( true );
	}
}

void FishingInventoryWnd::CheckNextItem( int iSellItemArray )
{
	int iCurTotalItem = g_MyInfo.GetCurFishingItemCnt();
	if( iCurTotalItem > 0 )
	{
		OnSetIconPush( ID_ITEM1_BTN + (iSellItemArray % MAX_BTN), true, iSellItemArray );
	}
	else
	{
		g_GUIMgr.HideWnd( FISHING_MESSAGE_BOX );
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
	}
}

void FishingInventoryWnd::UpdateItemData( int iType, ioHashString szIcon, ioHashString szGrade )
{
	// 갱신된 리스트로 페이지 갱신
	CheckPageData( false );
	UpdatePage();
	UpdateExp();
	UpdateBait();

	switch( iType )
	{
	case SELL_TYPE:
		{
			ioBaseChar *pOwner = m_pPlayStage->GetOwnerChar();
			if( !pOwner ) return;

			pOwner->SetFishingGradeEmoticon( szIcon, szGrade );
			if( pOwner->IsNeedSendNetwork() )
			{
				SP2Packet kPacket( CUPK_EMOTICON_SYNC );
				kPacket << pOwner->GetCharName();
				kPacket << EMOTICON_FISHING_GRADE;
				kPacket << szIcon;
				kPacket << szGrade;
				P2PNetwork::SendToAllPlayingUser( kPacket );
			}
		}
		break;
	}
}

void FishingInventoryWnd::CheckMessageBox( int iStartArray )
{
	g_GUIMgr.SetFishingMsgListBox( this, iStartArray );
}

void FishingInventoryWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	enum
	{
		ONE_NUM_X	= 329,
		ONE_NUM_Y	= 63,

		TWO_NUM_X	= 325,
		TWO_NUM_Y	= 63,
	};

	enum { NAME_X_OFFSET = 20, NAME_Y_OFFSET = 142, CNT_X_OFFSET = 104, CNT_Y_OFFSET = 142, };

	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
	g_FontMgr.SetAlignType( TAT_LEFT );
	g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
	g_FontMgr.PrintTextWidthCut( iXPos + NAME_X_OFFSET, iYPos + NAME_Y_OFFSET, FONT_SIZE_13, 48.0f, m_BaitCntText.c_str() );

	g_FontMgr.SetAlignType( TAT_RIGHT );
	g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
	g_FontMgr.PrintText( iXPos+CNT_X_OFFSET, iYPos+CNT_Y_OFFSET, FONT_SIZE_13, STR(1), m_iBaitCnt );


	// 슬롯 페이지
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetTextColor( 42, 60, 77 );
	g_FontMgr.PrintText( iXPos + 334, iYPos + 61, FONT_SIZE_14, STR(2), m_iCurPage + 1 );
}

void FishingInventoryWnd::ProcessToolTip()
{
	ioComplexStringPrinter kPrinter[GLOBAL_TEXT_TOOLTIP_LINE];

	DWORD dwCurOverID = 0;
	if( g_GUIMgr.GetPreOverWnd() )
	{
		if( g_GUIMgr.GetPreOverWnd()->GetParent() == this )
			dwCurOverID = g_GUIMgr.GetPreOverWnd()->GetID();
	}
	
	if( COMPARE( dwCurOverID, ID_ITEM1_BTN, ID_ITEM1_BTN+MAX_BTN ) )
	{
		int iStartArray = m_iCurPage * MAX_BTN;
		int iCurArray = dwCurOverID - ID_ITEM1_BTN;

		int iItemType = g_MyInfo.GetFishingItemType( iStartArray + iCurArray );
		if( iItemType == -1 )
			return;

		FishingItemInfoWnd *pInfoWnd = dynamic_cast<FishingItemInfoWnd*>( FindChildWnd( dwCurOverID ) );
		if( !pInfoWnd )
			return;

		ioHashString szName = g_FishingMgr.GetFishingItemName( iItemType );
		if( szName.IsEmpty() )
			return;

		kPrinter[0].SetTextStyle( TS_NORMAL );
		kPrinter[0].SetBkColor( 0, 0, 0 );
		kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
		kPrinter[0].AddTextPiece( FONT_SIZE_12, szName.c_str() );
	}
	else if( ID_FISHING_EXP_WND == dwCurOverID )
	{
		kPrinter[0].SetTextStyle( TS_NORMAL );
		kPrinter[0].SetBkColor( 0, 0, 0 );
		kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
		kPrinter[0].AddTextPiece( FONT_SIZE_12, STR(1), g_MyInfo.GetFishingExpert(), g_LevelMgr.GetNextFishingLevelUpExp( g_MyInfo.GetFishingLevel() ) );
	}

	GlobalTextTooltip *pTooltip = dynamic_cast<GlobalTextTooltip*>(FindChildWnd( ID_TOOL_TIP ));
	if( pTooltip )
	{
		pTooltip->SetTooltipText( kPrinter );
	}
}

bool FishingInventoryWnd::IsMaxSlot()
{
	return g_MyInfo.IsFishingItemMax();
}

void FishingInventoryWnd::SetBaitType( int iType )
{
	if( !m_pPlayStage ) return;

	ioBaseChar *pOwner = m_pPlayStage->GetOwnerChar();
	if( !pOwner ) return;

	if( g_MyInfo.SetCurFishingBait( iType ) )
	{
		if( pOwner->GetState() == CS_FISHING )
			pOwner->SetFishingEndState();

		UpdateBait();
	}
}

void FishingInventoryWnd::CheckFishingBtn()
{
	if( !m_pPlayStage ) return;
	
	ioBaseChar *pOwner = m_pPlayStage->GetOwnerChar();
	if( !pOwner ) return;

	if( pOwner->GetState() == CS_FISHING )
	{
		pOwner->SetFishingEndState();
	}
	else
	{
		if( pOwner->EnableFishing( true ) )
		{
			pOwner->SetFishingState( true );
		}
	}
}

void FishingInventoryWnd::UpdateBait()
{
	ioWnd *pBaitWnd = FindChildWnd(ID_BAIT_SELECT_BTN);
	if( !pBaitWnd )
		return;

	int iBaitType = g_MyInfo.GetCurFishingBait();
	int iBaitCnt = g_MyInfo.GetCurFishingBaitCnt();

	char szTitle[MAX_PATH] = "";
	if( iBaitType == 0 )
	{
		SafeSprintf( szTitle, sizeof( szTitle ), STR(1) );
	}
	else
	{
		ioEtcItem *pItem = g_EtcItemMgr.FindEtcItem( iBaitType );
		if( pItem )
		{
			if (iBaitType == ioEtcItem::EIT_ETC_PCROOM_FISHING_BAIT)
				SafeSprintf( szTitle, sizeof( szTitle ), STR(1), pItem->GetExtraName().c_str() );
			else if( !pItem->GetExtraName().IsEmpty() )
				SafeSprintf( szTitle, sizeof( szTitle ), STR(2), pItem->GetExtraName().c_str(), iBaitCnt );
			else
				SafeSprintf( szTitle, sizeof( szTitle ), STR(3), pItem->GetName().c_str(), iBaitCnt );
		}
		else
		{
			SafeSprintf( szTitle, sizeof( szTitle ), STR(4) );
		}
	}

	ioUITitle *pTitle = pBaitWnd->GetTitle();
	if( pTitle )
	{
		pTitle->SetWidthCutSize( 96.0f );
		pTitle->SetText( szTitle, "" );
	}
}

void FishingInventoryWnd::UpdateDefaultPos()
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

void FishingInventoryWnd::SetPositionToDefault( int iParentWidth, int iParentHeight, bool bChildMove, bool bCreate /*= false */ )
{
	int iTempDefYPos = m_iDefYPos;
	m_iDefYPos -= ( Setting::Height() - 600 ) * 0.22f;

	ioMovingWnd::SetPositionToDefault( iParentWidth, iParentHeight, bChildMove, bCreate );

	m_iDefYPos = iTempDefYPos;

	if( !bCreate )
	{
		m_iDefaultX = GetXPos();
		m_iDefaultY = GetYPos();
	}
}