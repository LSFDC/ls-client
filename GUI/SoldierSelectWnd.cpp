

#include "StdAfx.h"

#include "../io3DEngine/ioWnd.h"
#include "../io3DEngine/ioScroll.h"
#include "../io3DEngine/ioOrientBox.h"
#include "../io3DEngine/ioFontManager.h"
#include "../io3DEngine/ioUIRenderImage.h"
#include "../io3DEngine/ioUIRenderFrame.h"
#include "../io3DEngine/ioUIImageSetManager.h"
#include "../io3DEngine/HelpFunc.h"
#include "../io3DEngine/iostringmanager.h"
#include "../io3dengine/safesprintf.h"

#include "../WndID.h"
#include "../TextColorTable.h"
#include "../NetworkWrappingFunc.h"
#include "../CharacterInfo.h"
#include "../ioCreateChar.h"
#include "../ioApplication.h"
#include "../ioItemMaker.h"
#include "../ioComplexStringPrinter.h"
#include "../ioMyInfo.h"
#include "../ioClassPrice.h"
#include "../ioPlayStage.h"
#include "../DirectInput.h"
#include "../ChatMessageFunc.h"
#include "../ioSetItemInfo.h"
#include "../ioSetItemInfoManager.h"
#include "../ioBaseChar.h"
#include "../Setting.h"
#include "../ioMyLevelMgr.h"
#include "../ioKeyManager.h"
#include "../ioHeadquartersMode.h"

#include "../ioBowItem.h"
#include "../ioChargeComboBowItem.h"
#include "../ioLaserItem.h"
#include "../ioFireWeaponItem.h"
#include "../ioAutoShotItem.h"
#include "../ioUroborusItem.h"
#include "../ioWereWolfItem.h"
#include "../ioWitchItem.h"
#include "../ioExcaliburItem.h"
#include "../ioChargeCommandItem.h"
#include "../ioNakoruruItem.h"
#include "../ioPeterpanItem.h"
#include "../ioWindLordItem.h"
#include "../ioLeeItem.h"
#include "../ioProject_K_Item.h"
#include "../ioDevilItem.h"
#include "../ioMaidItem.h"
#include "../ioFlash_Item.h"
#include "../ioPsychoMetry_Item.h"
#include "../ioSlasher_Item.h"
#include "../ioDummyCharItem2.h"
#include "../ioLuciferItem.h"
#include "../ioTitanItem.h"
#include "../ioSuperDimensionItem.h"
#include "../ioTraceItem.h"
#include "../ioKaelthasItem.h"
#include "../ioStrikerItem.h"
#include "../ioParkilpyoItem.h"
#include "../ioJoroExpansionItem.h"
#include "../ioEarthQuakeItem.h"
#include "../ioShadowRangerItem.h"
#include "../ioNataItem.h"
#include "../ioResearcher.h"
#include "../ioBullFightItem.h"
#include "../ioChainMagicianItem.h"
#include "../ioStriderItem.h"
#include "../ioCuchulainItem.h"
#include "../ioOniMushaItem.h"
#include "../ioWestGunExtendItem.h"
#include "../ioSasinmuItem.h"
#include "../ioPredatorItem.h"

#include "../EtcHelpFunc.h"

#include "ioSP2GUIManager.h"
#include "SoldierLimitWnd.h"
#include "ioTutorialManager.h"
#include "ioMessageBox.h"
#include "TutorialWnd.h"
#include "MyInventoryWnd.h"
#include "NewShopWnd.h"
#include "ioUI3DRender.h"
#include "ioSpecificDragItem.h"
#include "ManualWnd.h"
#include "FightClubWnd.h"
#include "SoldierSelectWnd.h"
#include "SoldierExerciseWnd.h"
#include "ExPCRoomWnd.h"

#include <strsafe.h>
#include "../Local/ioLocalParent.h"

SoldierChargeWnd::SoldierChargeWnd()
{
	m_iSoldierNumber = 0;
}

SoldierChargeWnd::~SoldierChargeWnd()
{

}

void SoldierChargeWnd::SetCharacterCount( int iNumber )
{
	m_iSoldierNumber = iNumber;
	int iClassType = g_MyInfo.GetClassType( m_iSoldierNumber );
	if( g_MyInfo.GetClassArrayExceptExercise( iClassType ) != -1 )
	{
		ShowWnd();

		const ioSetItemInfo *pInfo = g_SetItemInfoMgr.GetSetInfoByIdx( iClassType - 1 );
		if( pInfo )
		{
			char szTitle[MAX_PATH] = "";
			SafeSprintf( szTitle, sizeof( szTitle ), STR(1), pInfo->GetName().c_str() );
			SetTitleText( szTitle );
		}
	}
	else
	{
		HideWnd();
	}
}

__int64 SoldierChargeWnd::GetNeedPeso()
{
	int iClassType  = g_MyInfo.GetClassType( m_iSoldierNumber );
	int iNeedSecond = max( 0, (int)g_ClassPrice.GetDefaultLimit() - g_MyInfo.GetCharTotalSecond( m_iSoldierNumber ) );
	float fSecondPeso = (float)g_ClassPrice.GetClassBuyPeso( iClassType, g_ClassPrice.GetDefaultLimit() ) / g_ClassPrice.GetDefaultLimit();

	__int64 iNeedPeso = (__int64)((float)iNeedSecond * fSecondPeso);	
	if( g_MyInfo.GetCharTotalSecond( m_iSoldierNumber ) == 0 )
		iNeedPeso = (__int64)g_ClassPrice.GetClassBuyPeso( iClassType, g_ClassPrice.GetDefaultLimit() );
	return iNeedPeso;
}

void SoldierChargeWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_EXIT:
	case ID_CANCEL:
		if( cmd == IOBN_BTNUP )
		{
			HideWnd();
		}
		break;
	case ID_CHARGE:
		if( cmd == IOBN_BTNUP )
		{
			HideWnd();

			DWORD dwSetItemCode = g_MyInfo.GetClassType( m_iSoldierNumber ) + SET_ITEM_CODE;
			const ioSetItemInfo *pSetItemInfo = g_SetItemInfoMgr.GetSetInfoByCode( dwSetItemCode );

			if( g_ClassPrice.GetDefaultLimit() <= (DWORD)g_MyInfo.GetCharTotalSecond( m_iSoldierNumber ) ||
				GetNeedPeso() == 0 )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
			}
			else if( g_MyInfo.GetMoney() - GetNeedPeso() < 0 ) 
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
			}
			else if( g_MyInfo.IsCharExercise( m_iSoldierNumber ) )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
			}
			else if( g_MyInfo.IsCharMortmain( g_MyInfo.GetClassType( m_iSoldierNumber ) ) )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
			}
			else if( !g_MyInfo.IsCanBuyItem( pSetItemInfo ))
			{
				if( pSetItemInfo )
				{
					char szGradeText[MAX_PATH]="";
					int iMax = pSetItemInfo->GetNeedLevelInfoListCnt();
					for (int j = 0; j < iMax ; j++)
					{
						if( pSetItemInfo->GetNeedLevelType(j) == ioSetItemInfo::NLT_GRADE )
						{
							g_LevelMgr.GetGradeName( pSetItemInfo->GetNeedLevel(j), szGradeText, sizeof( szGradeText ), false );
							break;
						}
					}
					ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
					kPrinter[0].SetTextStyle( TS_NORMAL );
					kPrinter[0].SetBkColor( 0, 0, 0 );	
					kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
					kPrinter[0].AddTextPiece( FONT_SIZE_13, szGradeText);
					kPrinter[0].SetTextColor( TCT_DEFAULT_GRAY );
					kPrinter[0].AddTextPiece( FONT_SIZE_13,STR(5) );

					g_GUIMgr.SetPrevMsgListBox( NULL, MB_OK, NULL, kPrinter );
				}
			}
			else
			{
				//충전
				SP2Packet kPacket( CTPK_CHAR_CHARGE );
				kPacket << g_MyInfo.GetCharIndex( m_iSoldierNumber );
				TCPNetwork::SendToServer( kPacket );
				g_App.SetMouseBusy( true );
			}
		}
		break;
	case ID_SHOP:
		if( cmd == IOBN_BTNUP )
		{
			HideWnd();

			int iClassType = g_MyInfo.GetClassType( m_iSoldierNumber );
			if( iClassType == -1 )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6) );
				return;
			}
		}
		break;
	}
}

void SoldierChargeWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() )
		return;
}

void SoldierChargeWnd::OnRender()
{
	ioWnd::OnRender();
	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
	g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
	
	g_FontMgr.SetAlignType( TAT_LEFT );	
	g_FontMgr.PrintText( iXPos + 11, iYPos + 40, FONT_SIZE_13, STR(1) );
	g_FontMgr.PrintText( iXPos + 11, iYPos + 60, FONT_SIZE_13, STR(2) );
	g_FontMgr.PrintText( iXPos + 11, iYPos + 84, FONT_SIZE_13, STR(3) );

	// 보유 페소
	ioComplexStringPrinter kPrinter;
	kPrinter.SetTextStyle( TS_OUTLINE_FULL_2X );
	kPrinter.SetBkColor( 0, 0, 0 );
	kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter.AddTextPiece( FONT_SIZE_13, STR(7), g_MyInfo.GetMoney() );
	kPrinter.SetTextColor( TCT_DEFAULT_GRAY );
	kPrinter.AddTextPiece( FONT_SIZE_13, STR(4) );
	kPrinter.PrintFullText( iXPos + 220, iYPos + 40, TAT_RIGHT );
	kPrinter.ClearList();

	// 필요 페소
	__int64 iNeedPeso = GetNeedPeso();	
	kPrinter.SetTextStyle( TS_OUTLINE_FULL_2X );
	kPrinter.SetBkColor( 0, 0, 0 );
	kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter.AddTextPiece( FONT_SIZE_13, STR(8), iNeedPeso );
	kPrinter.SetTextColor( TCT_DEFAULT_GRAY );
	kPrinter.AddTextPiece( FONT_SIZE_13, STR(5) );
	kPrinter.PrintFullText( iXPos + 220, iYPos + 60, TAT_RIGHT );
	kPrinter.ClearList();

	// 충전후 페소
	kPrinter.SetTextStyle( TS_OUTLINE_FULL_2X );
	kPrinter.SetBkColor( 0, 0, 0 );
	kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter.AddTextPiece( FONT_SIZE_13, STR(9), g_MyInfo.GetMoney() - iNeedPeso );
	kPrinter.SetTextColor( TCT_DEFAULT_GRAY );
	kPrinter.AddTextPiece( FONT_SIZE_13, STR(6) );
	kPrinter.PrintFullText( iXPos + 220, iYPos + 84, TAT_RIGHT );
	kPrinter.ClearList();
}

//////////////////////////////////////////////////////////////////////////
SoldierChar::SoldierChar()
{
	m_fCharZPos = 800.0f;
	m_fBackRot  = FLOAT10;
	m_pShadowBack = NULL;
}

SoldierChar::~SoldierChar()
{
	SAFEDELETE( m_pShadowBack );
}

void SoldierChar::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "ShadowBack" )
	{
		SAFEDELETE( m_pShadowBack );
		m_pShadowBack = pImage;
	}
	else
	{
		ioWnd::AddRenderImage( szType, pImage );
	}
}

void SoldierChar::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;
	
	ProcessRotate( fTimePerSec );

	UICharLoadingBuffer *pUIChar = g_MyInfo.GetSoldierSelectUIChar( g_MyInfo.GetCharIndex( m_iCharIndex ) );
	if( !pUIChar ) return;
	if( !pUIChar->m_aMyChar ) return;

	pUIChar->m_aMyChar->Update( fTimePerSec );
}

void SoldierChar::OnRender()
{
	ioWnd::OnRender();
	
	UICharLoadingBuffer *pUIChar = g_MyInfo.GetSoldierSelectUIChar( g_MyInfo.GetCharIndex( m_iCharIndex ) );
	if( !pUIChar ) return;
	if( !pUIChar->m_aMyChar ) return;

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();
	if( m_pShadowBack )
	{
		m_pShadowBack->SetScale( FLOAT05 );
		m_pShadowBack->Render( iXPos - 41, iYPos + 161, UI_RENDER_MULTIPLY, TFO_BILINEAR );
	}
	RenderChar( iXPos - 355, iYPos - 135 );
}
//////////////////////////////////////////////////////////////////////////
SoldierChargeBtn::SoldierChargeBtn()
{
	m_iSoldierNumber = 0;
	
	m_pBlueNorFrm	= NULL;
	m_pBlueDownFrm	= NULL;
	m_pPinkNorFrm	= NULL;
	m_pPinkDownFrm	= NULL;

	m_pCurrentNorFrm = NULL;
	m_pCurrentDownFrm= NULL;

	m_bPCRoomOpen = false;
}

SoldierChargeBtn::~SoldierChargeBtn()
{
	SAFEDELETE( m_pBlueNorFrm );
	SAFEDELETE( m_pBlueDownFrm );
	SAFEDELETE( m_pPinkNorFrm );
	SAFEDELETE( m_pPinkDownFrm );
}

void SoldierChargeBtn::AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame )
{
	if( szType == "BlueNorFrm" )
	{
		SAFEDELETE( m_pBlueNorFrm );
		m_pBlueNorFrm = pFrame;
	}
	else if( szType == "BlueDownFrm" )
	{
		SAFEDELETE( m_pBlueDownFrm );
		m_pBlueDownFrm = pFrame;
	}
	else if( szType == "PinkNorFrm" )
	{
		SAFEDELETE( m_pPinkNorFrm );
		m_pPinkNorFrm = pFrame;
	}
	else if( szType == "PinkDownFrm" )
	{
		SAFEDELETE( m_pPinkDownFrm );
		m_pPinkDownFrm = pFrame;
	}
	else
	{
		ioButton::AddRenderFrame( szType, pFrame );
	}
}

void SoldierChargeBtn::SetCharacterCount(int iNumber, bool bPCRoomOpen )
{
	m_iSoldierNumber = iNumber;
	m_bPCRoomOpen    = bPCRoomOpen;

	if( !m_bPCRoomOpen && ( g_MyInfo.GetCharIndex( m_iSoldierNumber ) == 0 || 
		g_MyInfo.IsCharMortmain( g_MyInfo.GetClassType( m_iSoldierNumber ) ) || 
		( !g_MyInfo.IsCharActive( m_iSoldierNumber ) && !g_MyInfo.IsCharExerciseStyle( m_iSoldierNumber, EXERCISE_PCROOM ) ) ) )
	{
		HideWnd();
	}
	else
	{
		ShowWnd();
		
		if( m_bPCRoomOpen || g_MyInfo.IsCharExerciseStyle( m_iSoldierNumber, EXERCISE_PCROOM ) || g_MyInfo.IsCharExerciseStyle( m_iSoldierNumber, EXERCISE_EVENT ) ) 
		{
			m_pCurrentNorFrm = m_pPinkNorFrm;
			m_pCurrentDownFrm= m_pPinkDownFrm;
		}
		else
		{
			m_pCurrentNorFrm = m_pBlueNorFrm;
			m_pCurrentDownFrm= m_pBlueDownFrm;
		}
	}
}

void SoldierChargeBtn::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;
}

void SoldierChargeBtn::OnDrawNormal( int iXPos, int iYPos )
{
	if( m_pCurrentNorFrm )
		m_pCurrentNorFrm->Render( iXPos, iYPos );
	ioButton::OnDrawNormal( iXPos, iYPos );
}

void SoldierChargeBtn::OnDrawOvered( int iXPos, int iYPos )
{
	if( m_pCurrentNorFrm )
		m_pCurrentNorFrm->Render( iXPos, iYPos );
	ioButton::OnDrawOvered( iXPos, iYPos );
}

void SoldierChargeBtn::OnDrawPushed( int iXPos, int iYPos )
{
	if( m_pCurrentDownFrm )
		m_pCurrentDownFrm->Render( iXPos, iYPos );
	ioButton::OnDrawPushed( iXPos, iYPos );
}

void SoldierChargeBtn::OnRender()
{
	ioButton::OnRender();
	
	float fXPos, fYPos;
	fXPos = GetDerivedPosX();
	fYPos = GetDerivedPosY();	

	if( m_bPCRoomOpen || g_MyInfo.IsCharExerciseStyle( m_iSoldierNumber, EXERCISE_PCROOM ) ) 
	{
		g_FontMgr.SetAlignType( TAT_CENTER );
		g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_3X );
		g_FontMgr.SetBkColor( 154, 39, 59 );
		g_FontMgr.SetTextColor( 255, 255, 255 );

		if( g_MyInfo.GetPCRoomAuthority() == FREEDAY_EVENT_CODE )
			g_FontMgr.PrintText( fXPos + 40, fYPos + 2, FONT_SIZE_11, STR(4) );
		else if( g_MyInfo.GetPCRoomAuthority() == 0 && !g_MyInfo.IsUserEvent() )
			g_FontMgr.PrintText( fXPos + 40, fYPos + 2, FONT_SIZE_11, STR(5) );
		else
			g_FontMgr.PrintText( fXPos + 40, fYPos + 2, FONT_SIZE_11, STR(1) );
	}
	else if( g_MyInfo.IsCharExerciseStyle( m_iSoldierNumber, EXERCISE_EVENT ) )
	{
		g_FontMgr.SetAlignType( TAT_CENTER );
		g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_3X );
		g_FontMgr.SetBkColor( 154, 39, 59 );
		g_FontMgr.SetTextColor( 255, 255, 255 );
		g_FontMgr.PrintText( fXPos + 40, fYPos + 2, FONT_SIZE_11, STR(3) );
	}
	else
	{
		g_FontMgr.SetAlignType( TAT_CENTER );
		g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_3X );
		g_FontMgr.SetBkColor( 33, 90, 162 );
		g_FontMgr.SetTextColor( 255, 255, 255 );

		int iTotalLimitDate = g_MyInfo.GetCharTotalSecond( m_iSoldierNumber ) / 60;
		int iHour     = iTotalLimitDate / 60;      
		int iMinute   = iTotalLimitDate % 60;
		char szTitle[MAX_PATH] = "";
		SafeSprintf( szTitle, sizeof( szTitle ), STR(2), iHour, iMinute );
		g_FontMgr.PrintText( fXPos + 40, fYPos + 2, FONT_SIZE_11, szTitle );
	}
}

SoldierRentalBtn::SoldierRentalBtn()
{
	m_iSoldierNumber = 0;
}

SoldierRentalBtn::~SoldierRentalBtn()
{

}

void SoldierRentalBtn::SetCharacterCount( int iNumber )
{
	m_iSoldierNumber = iNumber;
	if( g_MyInfo.IsCharExerciseStyle( m_iSoldierNumber, EXERCISE_RENTAL ) == false )
		HideWnd();
	else
	{
		ShowWnd();
	}
}

void SoldierRentalBtn::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;

	char szTitle[MAX_PATH] = "";
	DWORD dwRemainTime = g_MyInfo.GetCharRentalMinuteToArray( m_iSoldierNumber );	
	SafeSprintf( szTitle, sizeof(szTitle), STR(1), (int)(dwRemainTime / 60), (int)(dwRemainTime % 60) );
	SetTitleText( szTitle );
}
//////////////////////////////////////////////////////////////////////////
SoldierInfo::SoldierInfo()
{
	m_pBlueKeyBack = NULL;
	m_pPinkKeyBack = NULL;
	m_pGrayKeyBack = NULL;
	m_pMortmainMark= NULL;
	m_pSoldierShadow = NULL;
	m_bPCRoomOpen  = false;

	m_iSoldierNumber = 0;
	m_iSoldierSlot = -1;
	m_nGradeType = 0;
}

SoldierInfo::~SoldierInfo()
{
	SAFEDELETE( m_pBlueKeyBack );
	SAFEDELETE( m_pPinkKeyBack );
	SAFEDELETE( m_pGrayKeyBack );
	SAFEDELETE( m_pMortmainMark );
	SAFEDELETE( m_pSoldierShadow );
}

void SoldierInfo::SetCharacterCount( int iNumber, int iSlot, bool bPCRoomOpen )
{
	m_iSoldierNumber = iNumber;
	m_iSoldierSlot   = iSlot;
	m_bPCRoomOpen    = bPCRoomOpen;	

	m_nGradeType = 0;

	if ( m_iSoldierNumber > 0 )
	{
		int iClassType = g_MyInfo.GetClassType( m_iSoldierNumber );
		m_nGradeType = g_UIImageRenderMgr.GetPowerUpGradeType( iClassType );
	}
}

int SoldierInfo::GetCharacterCount() 
{
	return m_iSoldierNumber;
}

void SoldierInfo::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
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
	}
	else if( szType == "ShadowBack" )
	{
		SAFEDELETE( m_pSoldierShadow );
		m_pSoldierShadow = pImage;
	}
	else
	{
		ioWnd::AddRenderImage( szType, pImage );
	}
}

void SoldierInfo::RenderKeyState( int iXPos, int iYPos )
{
	g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetTextColor( 255, 255, 255 );
	if( m_bPCRoomOpen || g_MyInfo.IsCharExerciseStyle( m_iSoldierNumber, EXERCISE_PCROOM ) || g_MyInfo.IsCharExerciseStyle( m_iSoldierNumber, EXERCISE_RENTAL ) ) 
	{
		if( m_pPinkKeyBack )
			m_pPinkKeyBack->Render( iXPos, iYPos );
		g_FontMgr.SetBkColor( 154, 39, 59 );
	}
	else if( g_MyInfo.GetCharIndex( m_iSoldierNumber ) == 0 || !g_MyInfo.IsCharActive( m_iSoldierNumber ) || g_MyInfo.IsCharRental( m_iSoldierNumber ) || g_MyInfo.IsCharExerciseRentalInActive( m_iSoldierNumber ) )
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
		sprintf( szKeyText, "%s", g_KeyManager.GetKeyText( g_KeyManager.GetCurrentSoldierKey( m_iSoldierSlot ) ) );
		if( strcmp( szKeyText, STR(1) ) == 0 )
		{			
			sprintf( szKeyText, "-" );				
		}
		szKeyText[2] = NULL;
		g_FontMgr.PrintText( iXPos + 11, iYPos + 2, FONT_SIZE_11, szKeyText );
	}
}

void SoldierInfo::OnRender()
{
	ioWnd::OnRender();
	
	float fXPos, fYPos;
	fXPos = GetDerivedPosX();
	fYPos = GetDerivedPosY();

	RenderKeyState( fXPos + 5, fYPos + 5 );
	if( g_MyInfo.GetCharIndex( m_iSoldierNumber ) != 0 )
	{
		// 용병 경험치
		int iClassType = g_MyInfo.GetClassType( m_iSoldierNumber );
		int iSoldierLevel = g_MyInfo.GetClassLevel( iClassType, true );

		ioComplexStringPrinter kPrinter;
		kPrinter.SetTextStyle( TS_NORMAL );
		kPrinter.SetBkColor( 0, 0, 0 );
		kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
		kPrinter.AddTextPiece( FONT_SIZE_9, STR(1), g_MyInfo.GetClassLevel( iClassType, false ) );
		kPrinter.PrintFullText( fXPos + 84, fYPos + 5, TAT_RIGHT );
		kPrinter.ClearList();

		kPrinter.SetTextStyle( TS_NORMAL );
		kPrinter.SetBkColor( 0, 0, 0 );
		kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
		kPrinter.AddTextPiece( FONT_SIZE_9, STR(2), g_MyInfo.GetExpRate( iClassType ) );
		kPrinter.PrintFullText( fXPos + 84, fYPos + 15, TAT_RIGHT );


		// 영구 용병
		if( g_MyInfo.IsCharMortmain(  g_MyInfo.GetClassType( m_iSoldierNumber ) ) )
		{
			if( m_pMortmainMark )
			{
				m_pMortmainMark->Render( fXPos + 44, fYPos + 171 );
			}
		}
		else if( !g_MyInfo.IsCharActive( m_iSoldierNumber ) ) // 재고용
		{
			g_FontMgr.SetAlignType( TAT_CENTER );
			g_FontMgr.SetTextStyle( TS_NORMAL );
			g_FontMgr.SetBkColor( 0, 0, 0 );
			g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
			g_FontMgr.PrintText( fXPos + 44, fYPos + 161, FONT_SIZE_13, STR(3) );
		}
	}
	else if( m_bPCRoomOpen )
	{
		if( m_pSoldierShadow )
			m_pSoldierShadow->Render( fXPos + 8, fYPos + 22 );
	}
}

//////////////////////////////////////////////////////////////////////////
DWORD SoldierBtn::sg_dwDragFocusID = 0;
DWORD SoldierBtn::sg_dwFocusID     = 0;
SoldierBtn::SoldierBtn()
{
	m_pDragItem = new ioSoldierDragItem( SDI_SOLDIER_ITEM );
	m_pPlayStage = NULL;
	m_iSoldierNumber = -1;

	m_bBtnOver = false;
	m_bBtnDown = false;
	m_bPCRoomOpen = false;
}

SoldierBtn::~SoldierBtn()
{
	SAFEDELETE( m_pDragItem );
}

void SoldierBtn::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;

	if( m_bBtnOver && !m_bBtnDown )
	{
		ioLSMouse *pMouse = g_App.GetMouse();
		if( pMouse && pMouse->IsMouseShowing() &&
			g_MyInfo.IsCharActive(m_iSoldierNumber) && !g_MyInfo.IsCharExercise(m_iSoldierNumber) &&
			!g_MyInfo.IsCharRental(m_iSoldierNumber) && !g_MyInfo.IsCharExerciseRentalInActive( m_iSoldierNumber ) )
		{
			int iClassType = g_MyInfo.GetClassType( m_iSoldierNumber );
			pMouse->SetToolTipID( g_MyInfo.GetPublicID(), iClassType );
		}
	}
}

void SoldierBtn::SetCharacterCount( ioPlayStage *pPlayStage, int iNumber, int iSlot, bool bForceUpdateChar, bool bPCRoomOpen )
{
	m_pPlayStage = pPlayStage;
	m_bPCRoomOpen= bPCRoomOpen;
	int iOldSoldierNumber = m_iSoldierNumber;
	m_iSoldierNumber = iNumber;

	HideChildWnd( ID_EXTEND );
	HideChildWnd( ID_SHOP );
	HideChildWnd( ID_CHARGE );
	HideChildWnd( ID_RENTAL_STAND_BY );
	HideChildWnd( ID_RENTAL );
	HideChildWnd( ID_RENTAL_SOLDIER );

	SoldierInfo *pInfo = (SoldierInfo *)FindChildWnd( ID_INFO );
	if( pInfo )
	{
		pInfo->SetCharacterCount( m_iSoldierNumber, iSlot, m_bPCRoomOpen );
		pInfo->ShowWnd();
	}

	SoldierChargeBtn *pChargeBtn = (SoldierChargeBtn*)FindChildWnd( ID_CHARGE );
	if( pChargeBtn )
		pChargeBtn->SetCharacterCount( m_iSoldierNumber, m_bPCRoomOpen );

	SoldierRentalBtn *pRentalBtn = (SoldierRentalBtn*)FindChildWnd( ID_RENTAL_SOLDIER );
	if( pRentalBtn )
		pRentalBtn->SetCharacterCount( m_iSoldierNumber );

	if( g_MyInfo.GetCharIndex( m_iSoldierNumber ) == 0 )
	{
		HideChildWnd( ID_CHARACTER );		
		if( !m_bPCRoomOpen )
			ShowChildWnd( ID_SHOP );
	}
	else
	{
		SoldierChar *pChar = (SoldierChar *)FindChildWnd( ID_CHARACTER );
		if( pChar )
		{
			if( bForceUpdateChar ||
				iOldSoldierNumber == -1 ||
				iOldSoldierNumber != m_iSoldierNumber )
			{
				pChar->LoadMyCharacter( m_iSoldierNumber );
			}

			pChar->ShowWnd();
		}

		if( !g_MyInfo.IsCharActive( m_iSoldierNumber ) )
		{
			ShowChildWnd( ID_EXTEND );
		}
		else if( g_MyInfo.IsCharRental( m_iSoldierNumber ) )
		{
			if( g_MyInfo.GetCharRentalMinuteToArray( m_iSoldierNumber ) == 0 )
				ShowChildWnd( ID_RENTAL_STAND_BY );
			else
				ShowChildWnd( ID_RENTAL );
		}

		if( g_MyInfo.IsCharExerciseStyle( m_iSoldierNumber, EXERCISE_GENERAL ) )
		{
			HideChildWnd( ID_EXTEND );
		}
	}	

	if( g_MyInfo.IsTutorialUser() )
	{
		HideChildWnd( ID_EXTEND );
		HideChildWnd( ID_SHOP );
		HideChildWnd( ID_CHARGE );
		HideChildWnd( ID_RENTAL_STAND_BY );
		HideChildWnd( ID_RENTAL );
	}
}

int SoldierBtn::GetCharacterCount() 
{
	return m_iSoldierNumber;
}

bool SoldierBtn::IsSoldierSlot()
{
	ioWnd *pCharWnd = FindChildWnd( ID_CHARACTER );
	if( pCharWnd && pCharWnd->IsShow() )
		return true;

	return false;
}

void SoldierBtn::iwm_mouseleave( const ioMouse& mouse )
{
	if( m_bClicked && g_MyInfo.GetCharIndex( m_iSoldierNumber ) != 0 )
	{
		POINT ptOffset = { 0, 0};
		m_pDragItem->SetSoldierInfo( g_MyInfo.GetCharIndex( m_iSoldierNumber ) );
		m_pDragItem->DoDragDrop( GetParent(), m_rcPos, ptOffset );
		sg_dwDragFocusID = GetID();
	}

	ioButton::iwm_mouseleave( mouse );

	m_bBtnOver = false;
	m_bBtnDown = false;
}

void SoldierBtn::iwm_mouseover( const ioMouse& mouse )
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

void SoldierBtn::iwm_lbuttondown( const ioMouse& mouse )
{
	m_bBtnDown = true;

	ioButton::iwm_lbuttondown( mouse );
}

void SoldierBtn::iwm_lbuttonup( const ioMouse& mouse )
{
	m_bBtnDown = false;

	ioButton::iwm_lbuttonup( mouse );
}

void SoldierBtn::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_EXTEND:
		if( cmd == IOBN_BTNUP )
		{
			ShowInventorySoldierTab();
		}
		break;
	case ID_SHOP:
		if( cmd == IOBN_BTNUP )
		{
			NewShopWnd *pShopWnd = dynamic_cast<NewShopWnd*>(g_GUIMgr.FindWnd(NEW_SHOP_WND));
			if( pShopWnd )			
			{
				pShopWnd->ShowWnd();
				pShopWnd->SetDirectTab( NewShopWnd::ID_SOLDIER_TAB_BTN, 0, 0 );
			}
		}
		break;
	case ID_CHARGE:
		if( cmd == IOBN_BTNUP )
		{
			if( m_bPCRoomOpen || g_MyInfo.IsCharExerciseStyle( m_iSoldierNumber, EXERCISE_PCROOM ) ) 
			{
				if( g_MyInfo.GetPCRoomAuthority() )
				{
					ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
					if( pLocal && pLocal->IsPCRoomBonusSoldier() )
					{
						if( g_MyInfo.GetPCRoomAuthority() ==  FREEDAY_EVENT_CODE )
							g_GUIMgr.ShowWnd( USER_SELECT_HERO_BONUS_WND );
						else if( g_MyInfo.GetPCRoomAuthority() != 0 )
							g_GUIMgr.ShowWnd( EX_PCROOM_SOLDIER_BONUS_WND );
					}
				}
				else if( g_MyInfo.IsUserEvent() )
					g_GUIMgr.ShowWnd( USER_SELECT_HERO_BONUS_WND );
			}
			else
			{
				ShowInventorySoldierTab();
			}
		}
		break;
	case ID_RENTAL_STAND_BY:
		if( cmd == IOBN_BTNUP )
		{
			g_App.OpenMannerWnd( g_MyInfo.GetPublicID().c_str(), true );
		}
		break;
	case ID_RENTAL:
		if( cmd == IOBN_BTNUP )
		{
			g_MyInfo.CharRentalTimeMsgBox( g_MyInfo.GetCharRentalMinuteToArray( m_iSoldierNumber ) );
		}
		break;
	case ID_RENTAL_SOLDIER:
		if( cmd == IOBN_BTNUP )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		}
		break;
	}	
}

void SoldierBtn::OnDrawOvered( int iXPos, int iYPos )
{
	bool bFocus = false;
	if( sg_dwFocusID == m_dwID || sg_dwDragFocusID == m_dwID )
		bFocus = true;
	else
	{
		SoldierSelectWnd *pSoldierSelect = dynamic_cast<SoldierSelectWnd*>( GetParent() );
		if( pSoldierSelect && pSoldierSelect->GetSelectCharArray() == GetCharacterCount() )
			bFocus = true;
	}

	if( GetCharacterCount() == -1 )
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

void SoldierBtn::OnDrawPushed( int iXPos, int iYPos )
{
	if( GetCharacterCount() != -1 )	
		ioButton::OnDrawPushed( iXPos, iYPos );
	else
		ioButton::OnDrawNormal( iXPos, iYPos );
}

void SoldierBtn::OnDrawNormal( int iXPos, int iYPos )
{
	bool bFocus = false;
	if( sg_dwFocusID == m_dwID || sg_dwDragFocusID == m_dwID )
		bFocus = true;
	else
	{
		SoldierSelectWnd *pSoldierSelect = dynamic_cast<SoldierSelectWnd*>( GetParent() );
		if( pSoldierSelect && pSoldierSelect->GetSelectCharArray() == GetCharacterCount() )
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

void SoldierBtn::iwm_hide()
{
	SoldierChar *pChar = (SoldierChar *)FindChildWnd( ID_CHARACTER );
	if( pChar )
		pChar->HideWnd();

	m_bBtnDown = false;
	m_bBtnOver = false;
}

void SoldierBtn::iwm_show()
{
	SoldierChar *pChar = (SoldierChar *)FindChildWnd( ID_CHARACTER );
	if( pChar && !pChar->IsShow() )
		pChar->ShowWnd();

	m_bBtnDown = false;
	m_bBtnOver = false;
}

void SoldierBtn::ShowInventorySoldierTab()
{
	int iClassType = g_MyInfo.GetClassType( m_iSoldierNumber );

	MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(g_GUIMgr.FindWnd(MY_INVENTORY_WND));
	if( pInvenWnd )
	{
		pInvenWnd->ShowSoldierTabDirect( iClassType );
	}
}
//////////////////////////////////////////////////////////////////////////
SoldierSelectWnd::SoldierSelectWnd()
{
	m_eSoldierWndStyle   = FORCED_STYLE;

	m_iCurPage           = 0;
	m_dwRecoverStartTime = 0;
	m_dwRemainSecond     = 0;

	m_bSelectOk          = false;
	m_bNetworkMode       = true;
	m_pPlayStage         = NULL;
	m_iSelectCharArray   = 0;
	m_iAutoSelectCharArray = -1;

	m_iMaxSodierBtn      = MAX_MINIMUM_SOLDIER_BTN;
	m_dwScrollWndID      = ID_VERT_SCROLL;
	m_eWndShapeType      = WST_NORMAL_MIN;

	m_bChangeResolution  = false;
}

SoldierSelectWnd::~SoldierSelectWnd()
{
}

void SoldierSelectWnd::iwm_create()
{
	ReSetScroll();

	if( g_MyInfo.GetCharCount() > MAX_MINIMUM_SOLDIER_BTN )
	{
		m_eWndShapeType = WST_NORMAL_MAX;
	}
}

void SoldierSelectWnd::iwm_show()
{
	ReSetScroll();

	if( g_MyInfo.GetCharCount() > MAX_MINIMUM_SOLDIER_BTN )
	{
		m_eWndShapeType = WST_NORMAL_MAX;
	}

	ReSetWindow();

	// 예약한 용병이 있으면 자동 선택됨
	if( m_eSoldierWndStyle == FORCED_STYLE )
	{
		if( m_iAutoSelectCharArray != -1 )
		{
			SetSelectCharArray( m_iAutoSelectCharArray );
			SelectOk( true );				
			m_iAutoSelectCharArray = -1;
		}
		else
		{
			// 파이트클럽에서는 첫 용병 선택창 나올 때 사운드 출력
			if( m_pPlayStage && m_pPlayStage->GetModeType() == MT_FIGHT_CLUB )
			{
				if( !m_szFightClubShowSnd.IsEmpty() )
				{
					g_SoundMgr.PlaySound( m_szFightClubShowSnd );
				}
			}
		}
	}
}

void SoldierSelectWnd::ReSetScroll()
{
	ioScroll *pScroll = (ioScroll*)FindChildWnd( ID_VERT_SCROLL );
	if( pScroll )
		pScroll->SetScrollRange( 0, g_MyInfo.GetCurMaxCharSlot() / MAX_MINIMUM_SOLDIER_BTN );

	pScroll = (ioScroll*)FindChildWnd( ID_VERT_SCROLL_TALL );
	if( pScroll )
	{
		int iCurMax = g_MyInfo.GetCurMaxCharSlot() / MAX_MINIMUM_SOLDIER_BTN;
		int iRest	= g_MyInfo.GetCurMaxCharSlot() % MAX_MINIMUM_SOLDIER_BTN;
		if( iCurMax > 1 )
			iCurMax--;

		if( 0 < iRest )
			iCurMax++;

		pScroll->SetScrollRange( 0, iCurMax );
	}
}

void SoldierSelectWnd::iwm_hide()
{	
	m_eSoldierWndStyle = FORCED_STYLE;

	int iMax = ID_SOLDIER_BTN6+1;
	if( m_iMaxSodierBtn == MAX_MAXIMUM_SOLDIER_BTN )
		iMax = ID_SOLDIER_BTN12+1;

	for( int i=ID_SOLDIER_BTN1; i<iMax; i++ )
	{
		SoldierBtn *pBtn = (SoldierBtn*)FindChildWnd(i);
		if( !pBtn )	continue;

		pBtn->HideWnd();
	}
}

void SoldierSelectWnd::SettingSoldierBtn()
{
	int iMax = ID_SOLDIER_BTN6+1;
	if( m_iMaxSodierBtn == MAX_MAXIMUM_SOLDIER_BTN )
		iMax = ID_SOLDIER_BTN12+1;

	bool bForceUpdateChar = IsShow();
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


	for( int i = ID_SOLDIER_BTN1; i < iMax; ++i )
	{
		SoldierBtn *pBtn = (SoldierBtn*)FindChildWnd( i );
		if( !pBtn )	
			continue;

		int iSlotIdx = ( i - ID_SOLDIER_BTN1 ) + ( m_iCurPage * MAX_MINIMUM_SOLDIER_BTN );
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

		pBtn->SetCharacterCount( m_pPlayStage, iArray, iSlotIdx, bForceUpdateChar, bPCRoom );

		if( !pBtn->IsShow() )
			pBtn->ShowWnd();
	}

	SetCharSlotVec();
}

void SoldierSelectWnd::SetCharSlotVec()
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

int SoldierSelectWnd::GetCharSlotIndexToVecArray( int iSelIdx )
{
	int iSize = m_vCharSlot.size();
	for(int i = 0;i < iSize;i++)
	{
		if( m_vCharSlot[i] == iSelIdx )
			return i;
	}

	return -1;
}

int SoldierSelectWnd::GetCharVecArrayToSlotIndex( int iSelArray, bool bPlus  )
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

void SoldierSelectWnd::AddMySoldier( DWORD dwIndex )
{
	if( IsShow() )
	{
		g_GUIMgr.HideWnd( NEW_SHOP_WND );
	}

	SettingSoldierBtn();
}

void SoldierSelectWnd::iwm_wheel( int zDelta )
{
	if( !IsShow() )
		ioWnd::iwm_wheel( zDelta );
	else
	{ 
		// 휠 이벤트 처리를 했다면 부모로 이벤트를 날리지 않는다.
		if( zDelta == WHEEL_DELTA )
		{
			ioScroll *pScroll = (ioScroll*)FindChildWnd( m_dwScrollWndID );
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
			ioScroll *pScroll = (ioScroll*)FindChildWnd( m_dwScrollWndID );
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

void SoldierSelectWnd::iwm_vscroll(DWORD dwID, int curPos )
{
	m_iCurPage = curPos;
	SettingSoldierBtn();
}

bool SoldierSelectWnd::iwm_spacebar()
{
	if( !IsShow() ) return false;

	SetKeySelect();
	return true;
}

void SoldierSelectWnd::ShowCharExtend()
{
	int iClassType = g_MyInfo.GetClassType( GetSelectCharArray() );

	NewShopWnd *pShopWnd = dynamic_cast<NewShopWnd*>(g_GUIMgr.FindWnd(NEW_SHOP_WND));
	if( pShopWnd )
	{
		pShopWnd->ShowWnd();
		pShopWnd->SetDirectTab( NewShopWnd::ID_SOLDIER_TAB_BTN, iClassType, 0 );
	}
}

void SoldierSelectWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
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
			if( !IsSelectOk() )
			{
				SoldierBtn *pSoldierBtn = (SoldierBtn*)pWnd;
				if( COMPARE( pSoldierBtn->GetCharacterCount(), 0, g_MyInfo.GetCharCount() ) )
				{
					SetSelectCharArray( pSoldierBtn->GetCharacterCount() );
					SetKeySelect();
				}
			}
		}
		else if( cmd == IOWN_OVERED )
		{
			if( !IsSelectOk() )
			{
				SoldierBtn *pSoldierBtn = (SoldierBtn*)pWnd;
				if( COMPARE( pSoldierBtn->GetCharacterCount(), 0, g_MyInfo.GetCharCount() ) )
					SetSelectCharArray( pSoldierBtn->GetCharacterCount() );
			}
		}
		break;
	case ID_MAXIMUM_BTN:
		if( cmd == IOBN_BTNUP )
		{
			ChangeWnd( WST_NORMAL_MAX );
		}
		break;
	case ID_MINIMUM_BTN:
		if( cmd == IOBN_BTNUP )
		{
			ChangeWnd( WST_NORMAL_MIN );
		}
		break;
	case ID_ONE_SOLDIER_BTN:
		if( cmd == IOBN_BTNUP || cmd == IOEX_BTNUP )
		{
			if( IsCharStateInActive() )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
			}
			else
			{
				DWORD dwCharState = ioHeadquartersMode::STATE_OPEN_CHAR;
				if( m_pPlayStage )
				{
					ioHeadquartersMode *pHeadquartersMode = ToHeadquartersMode( m_pPlayStage->GetPlayMode() );
					if( pHeadquartersMode )
						dwCharState = pHeadquartersMode->GetCharState();
				}
				if( dwCharState == ioHeadquartersMode::STATE_UNITE_CHAR )
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
				}
				else
				{
					SP2Packet kPacket( CTPK_HEADQUARTERS_STATE_CHANGE );
					kPacket << (int)ioHeadquartersMode::STATE_UNITE_CHAR;
					TCPNetwork::SendToServer( kPacket );
					TCPNetwork::MouseBusy( true );
				}
			}			
			HideWnd();
		}
		break;
	case ID_ALL_SOLDIER_BTN:
		if( cmd == IOBN_BTNUP || cmd == IOEX_BTNUP )
		{
			if( IsCharStateInActive() )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
			}
			else
			{
				DWORD dwCharState = ioHeadquartersMode::STATE_OPEN_CHAR;
				if( m_pPlayStage )
				{
					ioHeadquartersMode *pHeadquartersMode = ToHeadquartersMode( m_pPlayStage->GetPlayMode() );
					if( pHeadquartersMode )
						dwCharState = pHeadquartersMode->GetCharState();
				}
				if( dwCharState == ioHeadquartersMode::STATE_OPEN_CHAR )
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
				}
				else
				{
					SP2Packet kPacket( CTPK_HEADQUARTERS_STATE_CHANGE );
					kPacket << (int)ioHeadquartersMode::STATE_OPEN_CHAR;
					TCPNetwork::SendToServer( kPacket );
					TCPNetwork::MouseBusy( true );
				}
			}

			HideWnd();
		}
		break;
	case ID_DISPLAY_SOLDIER_BTN:
		if( cmd == IOBN_BTNUP || cmd == IOEX_BTNUP )
		{
			if( IsCharStateInActive() )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
			}
			else
			{
				DWORD dwCharState = ioHeadquartersMode::STATE_OPEN_CHAR;
				if( m_pPlayStage )
				{
					ioHeadquartersMode *pHeadquartersMode = ToHeadquartersMode( m_pPlayStage->GetPlayMode() );
					if( pHeadquartersMode )
						dwCharState = pHeadquartersMode->GetCharState();
				}
				if( dwCharState == ioHeadquartersMode::STATE_DISPLAY_CHAR )
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6) );
				}
				else
				{
					SP2Packet kPacket( CTPK_HEADQUARTERS_STATE_CHANGE );
					kPacket << (int)ioHeadquartersMode::STATE_DISPLAY_CHAR;
					TCPNetwork::SendToServer( kPacket );
					TCPNetwork::MouseBusy( true );
				}
			}

			HideWnd();
		}
		break;
	}
}

void SoldierSelectWnd::SetFunctionKeySelect( int iArray, bool bNetMode )
{
	m_bNetworkMode = bNetMode;

	if( iArray == -1 || IsSelectOk() )
		return;

	int iArrayToSlot = g_MyInfo.GetCharArrayToSlotIndex( iArray );
	ioScroll *pScroll = dynamic_cast<ioScroll*>( FindChildWnd( m_dwScrollWndID ) );
	if( !pScroll )
		return;
	
	int iIndex = iArrayToSlot / PAGE_PER_BTN_NUM;
	pScroll->SetScrollPos( iIndex );

	int iBtnIdx = iArrayToSlot % PAGE_PER_BTN_NUM;
	if( m_iMaxSodierBtn == MAX_MAXIMUM_SOLDIER_BTN )
	{
		// 마지막 페이지만 스크롤이 안되므로 하단 캐릭터를 선택하기 위해서
		if( pScroll->GetMaxPos() < iIndex )
			iBtnIdx += PAGE_PER_BTN_NUM;
	}
	SoldierBtn *pSoldierBtn = (SoldierBtn*)FindChildWnd( iBtnIdx + ID_SOLDIER_BTN1 );
	if( pSoldierBtn )
	{
		int iCharCount = pSoldierBtn->GetCharacterCount();
		if( iCharCount != -1 && g_MyInfo.IsCharActive( iCharCount ) && !g_MyInfo.IsCharRental( iCharCount ) && !g_MyInfo.IsCharExerciseRentalInActive( iCharCount ) )
		{
			iwm_command( dynamic_cast<ioWnd*>( pSoldierBtn ), IOBN_BTNUP, 0 );
		}
	}
}

void SoldierSelectWnd::SetSelectWaitShow( int iArray, bool bNetMode )
{
	if( g_MyInfo.GetCharCount() == 0 )
	{	
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return ;
	}

	m_bNetworkMode = bNetMode;

	ioBaseChar *pOwner = m_pPlayStage->GetOwnerChar();
	if( !pOwner ) return;
	if( pOwner->IsChangeWaitState() ) return;

	m_eSoldierWndStyle = SELECT_STYLE;

	int iSlotToArray = g_MyInfo.GetCharSlotIndexToArray( iArray );
	if(	iSlotToArray == pOwner->GetSelectCharArray() || 
		g_MyInfo.GetCharIndex( iSlotToArray ) == 0 || 
		!g_MyInfo.IsCharActive( iSlotToArray ) ||
		g_MyInfo.IsCharRental( iSlotToArray ) ||
		g_MyInfo.IsCharExerciseRentalInActive( iSlotToArray ) )
	{
		ShowWnd();
		SetSelectCharArray( pOwner->GetSelectCharArray() );

		ioScroll *pScroll = dynamic_cast<ioScroll*>( FindChildWnd( m_dwScrollWndID ) );
		if( pScroll )
		{			
			int iIndex = iArray / PAGE_PER_BTN_NUM;
			pScroll->SetScrollPos( iIndex );
		}
	}
	else
	{
		SelectOk( false );
		
		if( iSlotToArray == -1 )
		{
			SetSelectCharArray( pOwner->GetSelectCharArray() );
		}

		ioScroll *pScroll = dynamic_cast<ioScroll*>( FindChildWnd( m_dwScrollWndID ) );
		if( pScroll )
		{
			if( m_iMaxSodierBtn == MAX_MAXIMUM_SOLDIER_BTN )
			{
				// 첫 페이지 보여주기
				pScroll->SetScrollPos( 0 );
			}
			else
			{
				int iIndex = iArray / m_iMaxSodierBtn;
				pScroll->SetScrollPos( iIndex );
			}
		}

		SetFunctionKeySelect( iSlotToArray, m_bNetworkMode );
	}
}

void SoldierSelectWnd::SetSettingSoldierShow( int iArray, bool bNetMode )
{
	m_bNetworkMode = bNetMode;

	ioBaseChar *pOwner = m_pPlayStage->GetOwnerChar();
	if( !pOwner ) return;

	m_eSoldierWndStyle = SETTING_STYLE;

	ShowWnd();
	SetSelectCharArray( pOwner->GetSelectCharArray() );

	ioScroll *pScroll = dynamic_cast<ioScroll*>( FindChildWnd( m_dwScrollWndID ) );
	if( pScroll )
	{
		if( m_iMaxSodierBtn == MAX_MAXIMUM_SOLDIER_BTN )
		{
			// 첫 페이지 보여주기
			pScroll->SetScrollPos( 0 );
		}
		else
		{
			int iIndex = iArray / m_iMaxSodierBtn;
			pScroll->SetScrollPos( iIndex );
		}
	}	
}

void SoldierSelectWnd::ChangeSoldierSlot( int iLeftArray, int iRightArray )
{
	SettingSoldierBtn();
}

void SoldierSelectWnd::OnKeyProcess()
{
	int iCurChar = GetCharSlotIndexToVecArray( g_MyInfo.GetCharArrayToSlotIndex( GetSelectCharArray() ) );
	if( iCurChar == -1 )
		return;
	int iPrevChar= iCurChar;
	int iMaxChar = m_vCharSlot.size() - 1;
	bool bPlus   = false;

	if( !m_pPlayStage ) return;
	ioBaseChar *pOwner = m_pPlayStage->GetOwnerChar();
	if( !pOwner ) return;

	if( pOwner->IsLeftKeyRelease() )
	{			
		iCurChar -= 1;
		if( iCurChar < 0 )
			iCurChar = iMaxChar;
	}
	else if( pOwner->IsRightKeyRelease() )
	{
		iCurChar += 1;
		if( iCurChar > iMaxChar )
			iCurChar = 0;
		bPlus = true;
	}
	else if( pOwner->IsUpKeyRelease() )
	{
		iCurChar -= MAX_MINIMUM_SOLDIER_BTN;
		if( iCurChar < 0 )
		{
			iCurChar = min( iMaxChar, (((iMaxChar+1)/MAX_MINIMUM_SOLDIER_BTN)*MAX_MINIMUM_SOLDIER_BTN) + (MAX_MINIMUM_SOLDIER_BTN+iCurChar) );		
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
					int iGap = iCurChar % MAX_MINIMUM_SOLDIER_BTN;
					iGap = MAX_MINIMUM_SOLDIER_BTN - iGap - 1;

					iCurChar += iGap;
				}
			}
		}
	}
	else if( pOwner->IsDownKeyRelease() )
	{
		iCurChar += MAX_MINIMUM_SOLDIER_BTN;

		if( iCurChar > iMaxChar )
		{
			if( iCurChar / MAX_MINIMUM_SOLDIER_BTN == iMaxChar / MAX_MINIMUM_SOLDIER_BTN )
				iCurChar = min( iMaxChar, (((iMaxChar+1)/MAX_MINIMUM_SOLDIER_BTN)*MAX_MINIMUM_SOLDIER_BTN) + (iCurChar % MAX_MINIMUM_SOLDIER_BTN) );
			else
				iCurChar = iCurChar % MAX_MINIMUM_SOLDIER_BTN;

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
					int iGap = iCurChar % MAX_MINIMUM_SOLDIER_BTN;
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
		ioScroll *pScroll = (ioScroll*)FindChildWnd( m_dwScrollWndID );
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
					iCurPos = iCurIdx / MAX_MINIMUM_SOLDIER_BTN;

				// 바뀐 페이지에도 없으면 그냥 그 인덱스 페이지로
				if( !CheckCurPage( iCurIdx, iCurPos ) )
					iCurPos = iCurIdx / MAX_MINIMUM_SOLDIER_BTN;
			}

			if( iCurPos != pScroll->GetCurPos() )
				pScroll->SetScrollPos( iCurPos );
		}
	}
}

bool SoldierSelectWnd::CheckCurPage( int iIndex, int iCurPage )
{
	ioScroll *pScroll = (ioScroll*)FindChildWnd( m_dwScrollWndID );
	if( pScroll )
	{
		int iFirstIndex = iCurPage * MAX_MINIMUM_SOLDIER_BTN;
		
		return COMPARE( iIndex, iFirstIndex, iFirstIndex+m_iMaxSodierBtn );
	}

	return false;
}

void SoldierSelectWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;

	ioWnd::OnProcess( fTimePerSec );

	if( m_bChangeResolution )
	{
		m_bChangeResolution = false;
		ReSetWindow();
	}
		
	if( IsSelectOk() ) return;

	// Recover Penalty
	DWORD dwCurTime = FRAMEGETTIME();
	if( m_dwRecoverStartTime + 1000 <= dwCurTime )
	{
		g_MyInfo.RecoverRevivalPenalty();
		m_dwRecoverStartTime = dwCurTime;
	}

	OnKeyProcess();

	if( !m_pPlayStage ) return;
	ioBaseChar *pOwner = m_pPlayStage->GetOwnerChar();
	if( !pOwner ) return;

	if( g_Input.IsKeyUp( KEY_ENTER )||
		pOwner->IsAttackKeyRelease() )
	{
		SetKeySelect();		
	}
}

void SoldierSelectWnd::SettingClassModeProcess( int iClassType )
{
	if( m_pPlayStage == NULL ) return;

	ModeType eModeType = m_pPlayStage->GetModeType();
	
	switch( eModeType )
	{
	case MT_FIGHT_CLUB:
		{
			// UI 전달
			FightClubTimeGaugeWnd *pTimeGaugeWnd = dynamic_cast< FightClubTimeGaugeWnd * >( g_GUIMgr.FindWnd( FIGHTCLUB_TIME_GAUGE_WND ) );
			if( pTimeGaugeWnd )
			{
				pTimeGaugeWnd->SetOwnerReserveChar( iClassType );
			}
		}
		break;
	}	
}

void SoldierSelectWnd::SetKeySelect()
{
	switch( m_eSoldierWndStyle )
	{
	case FORCED_STYLE:
		{
			if( g_MyInfo.IsCharActive( GetSelectCharArray() ) && !g_MyInfo.IsCharRental( GetSelectCharArray() ) && !g_MyInfo.IsCharExerciseRentalInActive( GetSelectCharArray() ) )
			{
				SelectOk( true );	
			}
			else if( g_MyInfo.IsCharExerciseRentalInActive( GetSelectCharArray() ) )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
			}
			else if( g_MyInfo.IsCharRental( GetSelectCharArray() ) )
			{
				g_MyInfo.CharRentalTimeMsgBox( g_MyInfo.GetCharRentalMinuteToArray( GetSelectCharArray() ) );
			}
			else
			{
				ShowCharExtend();
			}
		}
		break;
	case SELECT_STYLE:
		{
			if( g_MyInfo.IsCharActive( GetSelectCharArray() ) && !g_MyInfo.IsCharRental( GetSelectCharArray() ) && !g_MyInfo.IsCharExerciseRentalInActive( GetSelectCharArray() ) )
			{
				SendCharChange( GetSelectCharArray() );
			}
			else if( g_MyInfo.IsCharExerciseRentalInActive( GetSelectCharArray() ) )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
			}
			else if( g_MyInfo.IsCharRental( GetSelectCharArray() ) )
			{
				g_MyInfo.CharRentalTimeMsgBox( g_MyInfo.GetCharRentalMinuteToArray( GetSelectCharArray() ) );
			}
			else
			{
				ShowCharExtend();
			}
		}
		break;
	case SETTING_STYLE:
		{
			if( g_MyInfo.IsCharActive( GetSelectCharArray() ) && !g_MyInfo.IsCharRental( GetSelectCharArray() ) && !g_MyInfo.IsCharExerciseRentalInActive( GetSelectCharArray() ) )
			{
				m_iAutoSelectCharArray = GetSelectCharArray();
				// 다음 라운드 시작시 출전 예약 용병
				int iClassType = g_MyInfo.GetClassType( m_iAutoSelectCharArray ) ;
				g_ChatMgr.SetSystemMsg( STR(1), g_MyInfo.GetClassName( iClassType, g_MyInfo.GetCharPowerUpGradeByClassType( iClassType ) ) );
				
				SettingClassModeProcess( iClassType );

				HideWnd();
			}
			else if( g_MyInfo.IsCharExerciseRentalInActive( GetSelectCharArray() ) )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
			}
			else if( g_MyInfo.IsCharRental( GetSelectCharArray() ) )
			{
				g_MyInfo.CharRentalTimeMsgBox( g_MyInfo.GetCharRentalMinuteToArray( GetSelectCharArray() ) );					
			}
			else
			{
				ShowCharExtend();
			}
		}
		break;
	}
}

void SoldierSelectWnd::SendCharChange(int iArray )
{
	HideWnd();

	if( !m_pPlayStage )
		return;

	ioBaseChar *pOwner = m_pPlayStage->GetOwnerChar();
	if( !pOwner )	return;

	if( m_bNetworkMode )
	{
		if( !pOwner->IsChangeWaitState() )
		{
			if( pOwner->IsCanChangeCharState() )
			{
				if( pOwner->GetExperienceMode() == EMS_CHANGE_EXPERIENCE || pOwner->GetExperienceMode() == EMS_SEND_EXPERIENCE )
					return;

				pOwner->SetChangeWaitState( iArray );
			}
		}
	}
	else // 훈련소
	{
		if( pOwner->IsCanChangeCharState() )
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
			if( ToBowItem(pItem)				||
				ToFireWeaponItem(pItem)			||
				ToLaserItem(pItem)				||
				ToAutoShotItem(pItem)			||
				ToUroborusItem(pItem)			||
				ToWereWolfItem(pItem)			||
				ToWitchItem(pItem)				||
				ToNakoruruItem(pItem)			||
				ToExcaliburItem(pItem)			||
				ToLuciferItem(pItem)			||
				ToChargeComboBowItem(pItem)		||
				ToWestGunExtendItem(pItem)		||
				ToPeterpanItem(pItem)			||
				ToWindLordItem(pItem)			||
				ToProject_K_Item(pItem)			||
				ToDevilItem( pItem )			||
				ToMaidItem( pItem)				||
				ToFlashItem(pItem)				||
				ToPsychoMetryItem(pItem)		||
				ToSlasherItem(pItem)			||
				ToDummyCharItem2(pItem)			||
				ToTitanWeaponItem( pItem )		||
				ToSuperDimensionItem( pItem )	||
				ToTraceItem( pItem )			||
				ToKaelthasItem( pItem )			||
				ToStrikerItem(pItem)			||
				ToLeeItem(pItem)				||
				ToParkilpyoItem(pItem)			||
				ToJoroExpansionItem(pItem)		||
				ToEarthQuakeItem(pItem)			||
				ToShadowRangerItem(pItem)		||
				ToChargeCommandItem(pItem)		||
				ToNataItem(pItem)				||
				ToResearcher(pItem)				||
				ToBullFightItem(pItem)			||
				ToChainMagicianItem(pItem)		||
				ToStriderItem( pItem )			||
				ToCuchulainWeaponItem( pItem )	||
				ToOniMushaItem( pItem )			||
				ToSasinmuItem(pItem)			||
				ToPredatorItem(pItem) )
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

void SoldierSelectWnd::UpdateRecoverRevivalPenalty()
{
	// Recover Penalty
	DWORD dwCurTime = FRAMEGETTIME();
	if( m_dwRecoverStartTime + 1000 <= dwCurTime )
	{
		g_MyInfo.RecoverRevivalPenalty( true );
		m_dwRecoverStartTime = dwCurTime;
	}
}

void SoldierSelectWnd::SetRemainSecond( DWORD dwSecond )
{
	if( m_dwRemainSecond != 0 && m_dwRemainSecond != dwSecond )
	{
		//파이트클럽에서는 초가 변경될때마다 사운드 출력
		if( m_pPlayStage && m_pPlayStage->GetModeType() == MT_FIGHT_CLUB )
		{
			if( !m_szFightClubTickSnd.IsEmpty() )
			{
				g_SoundMgr.PlaySound( m_szFightClubTickSnd );
			}
		}
	}

	m_dwRemainSecond = dwSecond;
}

IWDropEffect SoldierSelectWnd::iwm_dragenter( ioDragItem *pItem, const ioMouse& mouse )
{
	DWORD dwBtnID = CheckDropEnable( mouse, pItem );
	if( dwBtnID == 0 )
		return IW_DROP_NONE;

	return IW_DROP_ENABLE;
}

IWDropEffect SoldierSelectWnd::iwm_dragover( ioDragItem *pItem, const ioMouse& mouse )
{
	return iwm_dragenter( pItem, mouse );
}

bool SoldierSelectWnd::iwm_dropitem( ioDragItem *pItem, const ioMouse& mouse, ioWnd *pStartWnd )
{
	DWORD dwBtnID = CheckDropEnable( mouse, pItem );
	if( dwBtnID == 0 )
		return false;
	
	ioSoldierDragItem *pDragItem = ToSoldierDragItem( pItem );
	if( !pDragItem )	return false;

	int iCurMaxSlot = g_MyInfo.GetCurMaxCharSlot();
	int iLeftSlotIndex = g_MyInfo.GetCharIndexToSlotIndex( pDragItem->GetIndex() );
	int iRightSlotIndex= ( SoldierBtn::sg_dwFocusID - ID_SOLDIER_BTN1 ) + ( m_iCurPage * MAX_MINIMUM_SOLDIER_BTN );
	if( !COMPARE( iLeftSlotIndex, 0, iCurMaxSlot) || !COMPARE( iRightSlotIndex, 0, iCurMaxSlot) ) return false;

	if( iLeftSlotIndex != iRightSlotIndex )
	{
		SP2Packet kPacket( CTPK_CHAR_SLOT_CHANGE );
		kPacket << iLeftSlotIndex << iRightSlotIndex;
		TCPNetwork::SendToServer( kPacket );
		TCPNetwork::MouseBusy( true );
	}

	SoldierBtn::sg_dwFocusID = 0;
	SoldierBtn::sg_dwDragFocusID = 0;
	return true;
}

void SoldierSelectWnd::iwm_dropdone( ioDragItem *pItem, ioWnd *pDroppedWnd, bool bDropped )
{
	SoldierBtn::sg_dwFocusID = 0;
	SoldierBtn::sg_dwDragFocusID = 0;
}

DWORD SoldierSelectWnd::CheckDropEnable( const ioMouse& mouse, ioDragItem *pItem )
{
	ioSoldierDragItem *pDragItem = ToSoldierDragItem( pItem );
	if( !pDragItem )	return 0;
	
	SoldierBtn::sg_dwFocusID = 0;
	DWORD dwBtnID = 0;
	int i = 0;

	for(i = ID_SOLDIER_BTN1;i < ID_SOLDIER_BTN12+1;i++)
	{
		dwBtnID = i;
		SoldierBtn *pBtn = dynamic_cast<SoldierBtn*>( FindChildWnd( dwBtnID ) );
		if( pBtn && !pBtn->HasWndStyle( IWS_INACTIVE ) && pBtn->IsShow())
		{
			if( pBtn->IsInWndRect( mouse.GetMousePos() ) )
			{
				SoldierBtn::sg_dwFocusID = dwBtnID;
				return dwBtnID;
			}
			else
			{
				ioMouse NullMouse;
				pBtn->iwm_mouseleave( NullMouse );
			}
		}
	}

	ioScroll *pScroll = dynamic_cast<ioScroll*>( FindChildWnd( m_dwScrollWndID ) );
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

void SoldierSelectWnd::OnRender()
{
	ioWnd::OnRender();
	
	int iXPos, iYPos;
	iXPos = GetDerivedPosX();
	iYPos = GetDerivedPosY();

	g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
	g_FontMgr.SetAlignType( TAT_LEFT );
	g_FontMgr.SetBkColor( 12, 66, 111 );
	g_FontMgr.SetTextColor( 255, 255, 255 );
	switch( m_eSoldierWndStyle  )
	{
	case FORCED_STYLE:
		g_FontMgr.PrintText( iXPos + 12, iYPos + 10, FONT_SIZE_13, STR(1), m_dwRemainSecond );	
		break;
	case SELECT_STYLE:
		g_FontMgr.PrintText( iXPos + 12, iYPos + 10, FONT_SIZE_13, STR(2) );	
		break;
	case SETTING_STYLE:
		g_FontMgr.PrintText( iXPos + 12, iYPos + 10, FONT_SIZE_13, STR(3) );	
		break;		
	}
}

bool SoldierSelectWnd::IsWndSizeEx()
{
	if( g_MyInfo.IsTutorialUser() ) return false;
	if( m_pPlayStage == NULL ) return false;
	if( m_eSoldierWndStyle == FORCED_STYLE ) return false;

	ioHeadquartersMode *pHeadquartersMode = ToHeadquartersMode( m_pPlayStage->GetPlayMode() );
	if( pHeadquartersMode == NULL ) return false;
	if( pHeadquartersMode->GetMasterName() != g_MyInfo.GetPublicID() ) return false;
    
	return true;
}

bool SoldierSelectWnd::IsCharStateInActive()
{
	if( g_MyInfo.IsTutorialUser() ) return true;
	if( m_pPlayStage == NULL ) return true;
	if( m_eSoldierWndStyle == FORCED_STYLE ) return true;

	ioHeadquartersMode *pHeadquartersMode = ToHeadquartersMode( m_pPlayStage->GetPlayMode() );
	if( pHeadquartersMode == NULL ) return true;
	if( pHeadquartersMode->GetMasterName() != g_MyInfo.GetPublicID() ) return true;
	if( pHeadquartersMode->GetRecordPlayUserCnt() > 1 ) return true;

	return false;
}

void SoldierSelectWnd::ChangeWnd( WndShapeType eWndType )
{
	m_eWndShapeType = eWndType;

	if( eWndType == WST_NORMAL_MIN )
	{
		ShowChildWnd( ID_MAXIMUM_BTN );
		HideChildWnd( ID_MINIMUM_BTN );
		
		ShowChildWnd( ID_VERT_SCROLL );
		HideChildWnd( ID_VERT_SCROLL_TALL );

		if( !IsWndSizeEx() )
			SetSize( NORMAL_MIN_WIDTH, NORMAL_MIN_HEIGHT );
		else
			SetSize( NORMAL_MIN_WIDTH, NORMAL_MIN_HEIGHT + MYROOM_ADD_HEIGHT );

		m_iMaxSodierBtn = MAX_MINIMUM_SOLDIER_BTN;
		m_dwScrollWndID = ID_VERT_SCROLL;

		for (int i = ID_SOLDIER_BTN7 ; i <  ID_SOLDIER_BTN12+1; i++)
			HideChildWnd( i );
	}
	else if( eWndType == WST_NORMAL_MAX )
	{
		HideChildWnd( ID_MAXIMUM_BTN );
		ShowChildWnd( ID_MINIMUM_BTN );

		HideChildWnd( ID_VERT_SCROLL );
		ShowChildWnd( ID_VERT_SCROLL_TALL );

		if( !IsWndSizeEx() )
			SetSize( NORMAL_MAX_WIDTH, NORMAL_MAX_HEIGHT );
		else
			SetSize( NORMAL_MAX_WIDTH, NORMAL_MAX_HEIGHT + MYROOM_ADD_HEIGHT );

		m_iMaxSodierBtn = MAX_MAXIMUM_SOLDIER_BTN;
		m_dwScrollWndID = ID_VERT_SCROLL_TALL;

		for (int i = ID_SOLDIER_BTN7 ; i <  ID_SOLDIER_BTN12+1; i++)
			ShowChildWnd( i );
	}

	ioWnd *pBtn = FindChildWnd( ID_ALL_SOLDIER_BTN );
	if( pBtn )
	{
		pBtn->SetWndPos( pBtn->GetXPos(), GetHeight() - BOTTOM_SOLDIER_BTN_GAP );
	}

	pBtn = FindChildWnd( ID_ONE_SOLDIER_BTN );
	if( pBtn )
	{
		pBtn->SetWndPos( pBtn->GetXPos(), GetHeight() - BOTTOM_SOLDIER_BTN_GAP );
	}

	pBtn = FindChildWnd( ID_DISPLAY_SOLDIER_BTN );
	if( pBtn )
	{
		pBtn->SetWndPos( pBtn->GetXPos(), GetHeight() - BOTTOM_SOLDIER_BTN_GAP );
	}

	m_iCurPage = 0;
	SettingSoldierBtn();
	SelectOk( false );

	ioScroll *pScroll = (ioScroll*)FindChildWnd( m_dwScrollWndID );
	if( pScroll )
		pScroll->SetScrollPos( m_iCurPage );

	int iXPos = ( Setting::Width() - ioWnd::GetWidth() ) / 2;
	int iYPos = ( Setting::Height() - ioWnd::GetHeight() ) / 2;
	ioWnd::SetWndPos( iXPos, iYPos - 20 );
}

void SoldierSelectWnd::SetPlayStage( ioPlayStage * pStage )
{
	m_pPlayStage = pStage;
}

void SoldierSelectWnd::ReSetWindow()
{
	if( m_eWndShapeType == WST_NORMAL_MIN )
		ChangeWnd( WST_NORMAL_MIN );
	else
		ChangeWnd( WST_NORMAL_MAX );

	SettingSoldierBtn();

	m_dwRecoverStartTime = FRAMEGETTIME();
	SelectOk( false );

	DWORD dwWndStyle = GetWndStyle();
	if( m_eSoldierWndStyle == FORCED_STYLE )
	{
		SetChildInActive( ID_EXIT );
		dwWndStyle &= ~IWS_EXIT_ESC;
	}
	else
	{
		SetChildActive( ID_EXIT );
		dwWndStyle |= IWS_EXIT_ESC;
	}

	if( !IsWndSizeEx() )
	{
		HideChildWnd( ID_ONE_SOLDIER_BTN );
		HideChildWnd( ID_ALL_SOLDIER_BTN );
		HideChildWnd( ID_DISPLAY_SOLDIER_BTN );
	}
	else 
	{
		ShowChildWnd( ID_ONE_SOLDIER_BTN );
		ShowChildWnd( ID_ALL_SOLDIER_BTN );
		ShowChildWnd( ID_DISPLAY_SOLDIER_BTN );
		if( IsCharStateInActive() )
		{
			SetChildWndStyleAdd( ID_ONE_SOLDIER_BTN, IWS_EXACTIVE );
			SetChildWndStyleAdd( ID_ALL_SOLDIER_BTN, IWS_EXACTIVE );
			SetChildWndStyleAdd( ID_DISPLAY_SOLDIER_BTN, IWS_EXACTIVE );
		}
		else
		{
			DWORD dwCharState = ioHeadquartersMode::STATE_OPEN_CHAR;
			if( m_pPlayStage )
			{
				ioHeadquartersMode *pHeadquartersMode = ToHeadquartersMode( m_pPlayStage->GetPlayMode() );
				if( pHeadquartersMode )
					dwCharState = pHeadquartersMode->GetCharState();
			}
			switch( dwCharState )
			{
			case ioHeadquartersMode::STATE_OPEN_CHAR:
				{
					SetChildWndStyleRemove( ID_ONE_SOLDIER_BTN, IWS_EXACTIVE );
					SetChildWndStyleAdd( ID_ALL_SOLDIER_BTN, IWS_EXACTIVE );
					SetChildWndStyleRemove( ID_DISPLAY_SOLDIER_BTN, IWS_EXACTIVE );
				}
				break;
			case ioHeadquartersMode::STATE_UNITE_CHAR:
				{
					SetChildWndStyleAdd( ID_ONE_SOLDIER_BTN, IWS_EXACTIVE );
					SetChildWndStyleRemove( ID_ALL_SOLDIER_BTN, IWS_EXACTIVE );
					SetChildWndStyleRemove( ID_DISPLAY_SOLDIER_BTN, IWS_EXACTIVE );
				}
				break;
			case ioHeadquartersMode::STATE_DISPLAY_CHAR:
				{
					SetChildWndStyleRemove( ID_ONE_SOLDIER_BTN, IWS_EXACTIVE );
					SetChildWndStyleRemove( ID_ALL_SOLDIER_BTN, IWS_EXACTIVE );
					SetChildWndStyleAdd( ID_DISPLAY_SOLDIER_BTN, IWS_EXACTIVE );
				}
				break;
			}
		}
	}


	SetWndStyle( dwWndStyle );
}

void SoldierSelectWnd::SetPositionToDefault( int iParentWidth, int iParentHeight, bool bChildMove, bool bCreate /*= false */ )
{
	ioWnd::SetPositionToDefault( iParentWidth, iParentHeight, bChildMove, bCreate );
	if( !bCreate )
	{
		// ResetWindow시 crash 발생하여 OnProcess에서 처리함.
		m_bChangeResolution = true;
	}
}

void SoldierSelectWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	m_szFightClubShowSnd = xElement.GetStringAttribute_e( "FightClubShowSnd" );
	m_szFightClubTickSnd = xElement.GetStringAttribute_e( "FightClubTickSnd" );
}