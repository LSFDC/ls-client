
#include "StdAfx.h"

#include "../io3DEngine/ioWnd.h"
#include "../io3DEngine/ioScroll.h"
#include "../io3DEngine/ioXMLElement.h"
#include "../io3DEngine/ioFontManager.h"
#include "../io3DEngine/ioUITitle.h"
#include "../io3DEngine/ioINILoader.h"
#include "../io3DEngine/HelpFunc.h"
#include "../io3DEngine/iostringmanager.h"
#include "../io3dengine/safesprintf.h"
#include "../io3DEngine/ioUIRenderImage.h"
#include "../io3DEngine/ioUIRenderFrame.h"
#include "../io3DEngine/ioUIImageSetManager.h"
#include "../io3DEngine/ioSoundManager.h"

#include "../NetworkWrappingFunc.h"
#include "../TextColorTable.h"
#include "../EtcHelpFunc.h"
#include "../ioItemMaker.h"

#include "../ioMyInfo.h"

#include "../WndID.h"
#include "ioMessageBox.h"
#include "ioSP2GUIManager.h"
#include "ioSpecificDragItem.h"

#include "GlobalWnd.h"
#include "ManualWnd.h"
#include "MyInventoryInfoWnd.h"

#include "AlchemicFuncWnd.h"
#include <strsafe.h>

//////////////////////////////////////////////////////////////////////////////
SelectAlchemicFuncBtn::SelectAlchemicFuncBtn()
{
	m_iCurSelectFunc = 0;
	m_szSelectText.Clear();
}

SelectAlchemicFuncBtn::~SelectAlchemicFuncBtn()
{
}

void SelectAlchemicFuncBtn::iwm_show()
{
	m_iCurSelectFunc = 0;
	m_dwCurTitleColor = TCT_DEFAULT_DARKGRAY;
	m_szSelectText.Clear();
}

void SelectAlchemicFuncBtn::InitData()
{
	m_iCurSelectFunc = 0;
	m_dwCurTitleColor = TCT_DEFAULT_DARKGRAY;
	m_szSelectText.Clear();
}

void SelectAlchemicFuncBtn::SetSelectAlchemicFunc( int iSelectFunc )
{
	m_iCurSelectFunc = 0;
	m_dwCurTitleColor = TCT_DEFAULT_DARKGRAY;

	AlchemicFuncInfo kFuncInfo;

	if( g_AlchemicMgr.FindAlchemicFunc( iSelectFunc, kFuncInfo ) )
	{
		m_iCurSelectFunc = iSelectFunc;
		m_szSelectText = kFuncInfo.m_Desc;
	}
}

void SelectAlchemicFuncBtn::SetSelectNewAlchemicFunc( int iSelectFunc, int iValue )
{
	m_iCurSelectFunc = 0;
	m_dwCurTitleColor = TCT_DEFAULT_DARKGRAY;

	char szBuf[MAX_PATH] = "";
	AlchemicFuncInfo kFuncInfo;

	if( g_AlchemicMgr.FindAlchemicFunc( iSelectFunc, kFuncInfo ) )
	{
		m_iCurSelectFunc = iSelectFunc;
		m_szSelectText.Clear();

		switch( kFuncInfo.m_AlchemicType )
		{
		case ALT_NEW_SOLDIER:
			{
				ioHashString szSoldierName = g_MyInfo.GetClassName( iValue );
				wsprintf_e( szBuf, "%s : %s", kFuncInfo.m_Desc2.c_str(), szSoldierName.c_str() );
				m_szSelectText = szBuf;
			}
			break;
		case ALT_NEW_ITEM:
			{
				ioHashString szItemName;
				const ioItem *pItem = g_ItemMaker.GetItemConst( iValue, __FUNCTION__ );
				if( pItem )
					szItemName = pItem->GetName();

				if( szItemName.IsEmpty() )
					wsprintf_e( szBuf, "%s : None", kFuncInfo.m_Desc2.c_str() );
				else
					wsprintf_e( szBuf, "%s : %s", kFuncInfo.m_Desc2.c_str(), szItemName.c_str() );

				m_szSelectText = szBuf;
			}
			break;
		default:
			m_szSelectText = kFuncInfo.m_Desc;
			break;
		}
	}
}

void SelectAlchemicFuncBtn::SetCurTitleColor( DWORD dwTitleColor )
{
	m_dwCurTitleColor = dwTitleColor;
}

void SelectAlchemicFuncBtn::OnRender()
{
	ioButton::OnRender();
	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	ioUITitle *pTitle =  GetTitle();
	if( pTitle )
	{
		pTitle->SetColor( "Normal", m_dwCurTitleColor, 0xFF000000 );
		pTitle->SetColor( "Over", m_dwCurTitleColor, 0xFF000000 );
		pTitle->SetColor( "Push", m_dwCurTitleColor, 0xFF000000 );
	}

	if( m_iCurSelectFunc > 0 )
		SetTitleText( m_szSelectText.c_str() );
	else
		SetTitleText( m_szDefaultText.c_str() );
}

void SelectAlchemicFuncBtn::ParseExtraInfo( ioXMLElement &xElement )
{
	ioButton::ParseExtraInfo( xElement );

	m_szDefaultText = xElement.GetStringAttribute_e( "DescText" );
}

/////////////////////////////////////////////////////////////////////////////////

AlchemicItemRegisterWnd::AlchemicItemRegisterWnd()
{
	m_iCode = 0;
	m_iCount = 0;

	m_iMaxCount = 0;
	m_iCurCount = 0;

	m_pEditFrm = NULL;

	m_pIconImg = NULL;
	m_pIconBackImg = NULL;
	m_pPieceFrm = NULL;
	m_pAdditiveFrm = NULL;

	m_pPreWnd = NULL;
}

AlchemicItemRegisterWnd::~AlchemicItemRegisterWnd()
{
	SAFEDELETE(m_pEditFrm);

	SAFEDELETE(m_pIconImg);
	SAFEDELETE(m_pIconBackImg);
	SAFEDELETE(m_pPieceFrm);
	SAFEDELETE(m_pAdditiveFrm);
}

void AlchemicItemRegisterWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "icon_back_image" )
	{
		SAFEDELETE( m_pIconBackImg );
		m_pIconBackImg = pImage;
	}
	else
	{
		ioWnd::AddRenderImage( szType, pImage );
	}
}

void AlchemicItemRegisterWnd::AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame )
{
	if( szType == "EditFrm" )
	{
		SAFEDELETE( m_pEditFrm );
		m_pEditFrm = pFrame;
	}
	else if( szType == "piece_frm" )
	{
		SAFEDELETE( m_pPieceFrm );
		m_pPieceFrm = pFrame;
	}
	else if( szType == "additive_frm" )
	{
		SAFEDELETE( m_pAdditiveFrm );
		m_pAdditiveFrm = pFrame;
	}
	else
	{
		ioMovingWnd::AddRenderFrame( szType, pFrame );
	}
}

void AlchemicItemRegisterWnd::SetRegisterItemInfo( int iType, int iCode, int iMaxCnt, ioWnd *pWnd )
{
	m_iCode = 0;
	m_iCount = 0;

	m_iMaxCount = 0;
	m_iCurCount = 0;

	SAFEDELETE(m_pIconImg);

	ioAlchemicInventory *pInven = g_MyInfo.GetAlchemicInventory();
	if( !pInven )
		return;

	m_iCode = iCode;
	m_iMaxCount = iMaxCnt;
	m_iCurCount = pInven->GetAlchemicItemCnt( m_iCode );

	if( m_iMaxCount > 0 )
		m_iCount = min( m_iCurCount, m_iMaxCount );
	else
		m_iCount = m_iCurCount;

	ioHashString szIcon = g_AlchemicMgr.GetAlchemicItemIcon( m_iCode );

	m_pIconImg = g_UIImageSetMgr.CreateImageByFullName( szIcon );

	m_pPreWnd = pWnd;

	ioHashString szTitle;

	switch( iType )
	{
	case ALT_SOLDIER:
	case ALT_ITEM:
	case ALT_NEW_SOLDIER:
	case ALT_NEW_ITEM:
		if( m_iCode > ALCHEMIC_ADDITIVE_DIV )
			szTitle = STR(6);
		else
			szTitle = STR(7);
		break;
	case ALT_CHANGE:
	case ALT_EXCHANGE:
		if( m_iCode > ALCHEMIC_ADDITIVE_DIV )
			szTitle = STR(8);
		else
			szTitle = STR(9);
		break;
	case ALT_SELL:
		szTitle = STR(10);
		break;
	}

	ioUITitle *pTitle = GetTitle();
	if( pTitle )
	{
		SetTitleText( szTitle.c_str() );
	}

	SetCountEditFocus();

	if( !IsShow() )
		ShowWnd();
}

bool AlchemicItemRegisterWnd::iwm_spacebar()
{
	ioButton *pBtn = (ioButton*)FindChildWnd( ID_REGISTER_BTN );
	if( !pBtn ) return false;
	if( !pBtn->IsShow() ) return false;

	iwm_command( pBtn, IOBN_BTNUP, 0 );
	return true;
}

void AlchemicItemRegisterWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_REGISTER_BTN:
		if( cmd == IOBN_BTNUP )
		{
			SetCountRegister();
		}
		break;
	case ID_CANCEL_BTN:
	case ID_EXIT_BTN:
		if( cmd == IOBN_BTNUP )
		{
			HideWnd();
		}
		break;
	case ID_COUNT_EDIT:
		if( cmd == IOED_INPUT && param == 1 )
		{
			SetCountRegister();
		}
		break;
	}
}

void AlchemicItemRegisterWnd::SetCountRegister()
{
	ioEdit *pEdit = (ioEdit*)FindChildWnd( ID_COUNT_EDIT );
	if( pEdit )
	{
		if( strlen( pEdit->GetText() ) > 0 )
		{
			m_iCount = atoi( pEdit->GetText() );
		}

		// 미입력 체크
		if( m_iCount == 0 )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
			return;
		}

		// 최대 제한 갯수 체크
		if( m_iMaxCount > 0 && m_iCount > m_iMaxCount )
		{
			char szConvertNum[MAX_PATH] = "";
			Help::ConvertNumToStrComma( m_iMaxCount, szConvertNum, sizeof( szConvertNum ) );
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5), szConvertNum );
			return;
		}

		// 보유 갯수 체크
		if( m_iCount > m_iCurCount )
		{
			char szConvertNum[MAX_PATH] = "";
			Help::ConvertNumToStrComma( m_iCurCount, szConvertNum, sizeof( szConvertNum ) );
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6), szConvertNum );
			return;
		}

		pEdit->KillKeyFocus();

		SelectAlchemicItemBtn *pItemBtn = dynamic_cast<SelectAlchemicItemBtn*>(m_pPreWnd);
		if( pItemBtn )
		{
			pItemBtn->SetData( m_iCode, m_iCount );
		}

		HideWnd();
	}
}

void AlchemicItemRegisterWnd::OnRender()
{
	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	ioMovingWnd::OnRender();

	enum
	{
		ITEM_X				= 17,
		ITEM_Y				= 54,

		ICON_X				= 32,
		ICON_Y				= 32,

		COUNT_TEXT_X		= 94,
		COUNT_TEXT_Y		= 79,

		MAX_COUNT_TEXT_Y	= 99,

		COUNT_X				= 278,
		COUNT_Y				= 79,
	};

	// item
	bool bAdditive = false;
	if( m_iCode > ALCHEMIC_ADDITIVE_DIV )
		bAdditive = true;

	if( bAdditive )
	{
		if( m_pAdditiveFrm )
		{
			m_pAdditiveFrm->SetSize( 64, 64 );
			m_pAdditiveFrm->Render( iXPos+ITEM_X, iYPos+ITEM_Y );
		}
	}
	else
	{
		if( m_pPieceFrm )
		{
			m_pPieceFrm->SetSize( 64, 64 );
			m_pPieceFrm->Render( iXPos+ITEM_X, iYPos+ITEM_Y );
		}
	}

	if( m_pIconBackImg )
	{
		m_pIconBackImg->SetScale( 0.75f );
		m_pIconBackImg->Render( iXPos+ITEM_X+32, iYPos+ITEM_Y+32, UI_RENDER_MULTIPLY, TFO_BILINEAR );
	}

	if( m_pIconImg )
	{
		m_pIconImg->SetScale( 0.75f );
		m_pIconImg->Render( iXPos+ITEM_X+32, iYPos+ITEM_Y+32, UI_RENDER_NORMAL, TFO_BILINEAR );
	}

	if( m_pEditFrm )
		m_pEditFrm->Render( iXPos+88, iYPos+54 );

	ioEdit *pEdit = (ioEdit*)FindChildWnd( ID_COUNT_EDIT );
	if( pEdit )
	{
		if( strlen( pEdit->GetText() ) > 0 )
		{
			m_iCount = atoi( pEdit->GetText() );
		}
	}

	char szCount[MAX_PATH]="";

	ioComplexStringPrinter kPrinter;
	kPrinter.SetTextStyle( TS_NORMAL );
	kPrinter.SetBkColor( 0, 0, 0 );
	kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter.AddTextPiece( FONT_SIZE_13, STR(7) );
	kPrinter.PrintFullText( iXPos+COUNT_TEXT_X, iYPos+COUNT_TEXT_Y, TAT_LEFT );
	kPrinter.ClearList();

	kPrinter.SetTextStyle( TS_NORMAL );
	kPrinter.SetBkColor( 0, 0, 0 );
	kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter.AddTextPiece( FONT_SIZE_13, STR(8) );
	kPrinter.PrintFullText( iXPos+COUNT_TEXT_X, iYPos+MAX_COUNT_TEXT_Y, TAT_LEFT );
	kPrinter.ClearList();

	// 입력 갯수
	Help::ConvertNumToStrComma( m_iCount, szCount, sizeof( szCount ) );
	kPrinter.SetTextStyle( TS_NORMAL );
	kPrinter.SetBkColor( 0, 0, 0 );
	kPrinter.SetTextColor( TCT_DEFAULT_ORANGE );
	kPrinter.AddTextPiece( FONT_SIZE_13, STR(9), szCount );
	kPrinter.PrintFullText( iXPos+COUNT_X, iYPos+57, TAT_RIGHT );
	kPrinter.ClearList();

	// 보유 갯수
	Help::ConvertNumToStrComma( m_iCurCount, szCount, sizeof( szCount ) );
	kPrinter.SetTextStyle( TS_NORMAL );
	kPrinter.SetBkColor( 0, 0, 0 );
	kPrinter.SetTextColor( TCT_DEFAULT_GREEN );
	kPrinter.AddTextPiece( FONT_SIZE_13, STR(10), szCount );
	kPrinter.PrintFullText( iXPos+COUNT_X, iYPos+COUNT_Y, TAT_RIGHT );
	kPrinter.ClearList();

	// 최대 갯수
	kPrinter.SetTextStyle( TS_NORMAL );
	kPrinter.SetBkColor( 0, 0, 0 );
	kPrinter.SetTextColor( TCT_DEFAULT_GREEN );

	if( m_iMaxCount == 0 )
		kPrinter.AddTextPiece( FONT_SIZE_13, STR(11), szCount );
	else
	{
		Help::ConvertNumToStrComma( m_iMaxCount, szCount, sizeof( szCount ) );
		kPrinter.AddTextPiece( FONT_SIZE_13, STR(12), szCount );
	}
	kPrinter.PrintFullText( iXPos+COUNT_X, iYPos+COUNT_Y+20, TAT_RIGHT );
	kPrinter.ClearList();
}

void AlchemicItemRegisterWnd::SetCountEditFocus()
{
	ioEdit *pEdit = (ioEdit*)FindChildWnd( ID_COUNT_EDIT );
	if( pEdit )
	{
		pEdit->ClearString();
		pEdit->SetKeyFocus();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SelectAlchemicItemBtn::SelectAlchemicItemBtn(void)
{
	m_iCode = 0;
	m_iPreCode = 0;
	m_iCurCnt = 0;
	m_iEmptyCnt = 0;
	m_iDecreaseCnt = 0;

	m_pEmptyFrm = NULL;
	m_pPieceFrm = NULL;
	m_pAdditiveFrm = NULL;

	m_pPieceImg1 = NULL;
	m_pPieceImg2 = NULL;
	m_pPieceImg3 = NULL;
	m_pAdditiveImg = NULL;
	m_pAllImg = NULL;

	m_pMaxNum = NULL;

	m_pIconImg = NULL;
	m_pIconBackImg = NULL;
}

SelectAlchemicItemBtn::~SelectAlchemicItemBtn(void)
{
	SAFEDELETE( m_pEmptyFrm );
	SAFEDELETE( m_pPieceFrm );
	SAFEDELETE( m_pAdditiveFrm );

	SAFEDELETE( m_pPieceImg1 );
	SAFEDELETE( m_pPieceImg2 );
	SAFEDELETE( m_pPieceImg3 );
	SAFEDELETE( m_pAdditiveImg );
	SAFEDELETE( m_pAllImg );

	SAFEDELETE( m_pMaxNum );

	SAFEDELETE( m_pIconImg );
	SAFEDELETE( m_pIconBackImg );
}

void SelectAlchemicItemBtn::AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame )
{
	if( szType == "empty_frm" )
	{
		SAFEDELETE( m_pEmptyFrm );
		m_pEmptyFrm = pFrame;
	}
	else if( szType == "piece_frm" )
	{
		SAFEDELETE( m_pPieceFrm );
		m_pPieceFrm = pFrame;
	}
	else if( szType == "additive_frm" )
	{
		SAFEDELETE( m_pAdditiveFrm );
		m_pAdditiveFrm = pFrame;
	}
	else
	{
		ioButton::AddRenderFrame( szType, pFrame );
	}
}

void SelectAlchemicItemBtn::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "peice_image1" )
	{
		SAFEDELETE( m_pPieceImg1 );
		m_pPieceImg1 = pImage;
	}
	else if( szType == "peice_image2" )
	{
		SAFEDELETE( m_pPieceImg2 );
		m_pPieceImg2 = pImage;
	}
	else if( szType == "peice_image3" )
	{
		SAFEDELETE( m_pPieceImg3 );
		m_pPieceImg3 = pImage;
	}
	else if( szType == "additive_image" )
	{
		SAFEDELETE( m_pAdditiveImg );
		m_pAdditiveImg = pImage;
	}
	else if( szType == "peice_all_image" )
	{
		SAFEDELETE( m_pAllImg );
		m_pAllImg = pImage;
	}
	else if( szType == "max_num_image" )
	{
		SAFEDELETE( m_pMaxNum );
		m_pMaxNum = pImage;
	}
	else if( szType == "icon_back_image" )
	{
		SAFEDELETE( m_pIconBackImg );
		m_pIconBackImg = pImage;
	}
	else
	{
		ioButton::AddRenderImage( szType, pImage );	
	}	
}

void SelectAlchemicItemBtn::InitData()
{
	// 	m_iType, m_iEmptyCnt 값은 초기화 하지 않는다

	if( (m_iType != BTT_NEW_PIECE) && (m_iType != BTT_NEW_ADDITIVE) )
		m_iPreCode = 0;

	m_iCode = 0;
	m_iCurCnt = 0;
	m_iDecreaseCnt = 0;

	// count text
	m_CountText.ClearList();
	m_CountText2.ClearList();

	if( m_iPreCode == 0 )
	{
		char szCount[MAX_PATH]="";
		Help::ConvertNumToStrComma( m_iCurCnt, szCount, sizeof( szCount ) );

		m_CountText.SetTextStyle( TS_NORMAL );
		m_CountText.SetTextColor( 153, 153, 153 );

		switch( m_iType )
		{
		case BTT_PIECE1:
			m_CountText.AddTextPiece( FONT_SIZE_12, STR(13) );
			break;
		case BTT_PIECE2:
			m_CountText.AddTextPiece( FONT_SIZE_12, STR(14) );
			break;
		case BTT_PIECE3:
			m_CountText.AddTextPiece( FONT_SIZE_12, STR(15) );
			break;
		case BTT_PIECE4:
			m_CountText.AddTextPiece( FONT_SIZE_12, STR(16) );
			break;
		case BTT_ADDITIVE:
			m_CountText.AddTextPiece( FONT_SIZE_12, STR(17) );
			break;
		case BTT_ALL:
			m_CountText.AddTextPiece( FONT_SIZE_12, STR(18) );
			break;
		}
	}
}

void SelectAlchemicItemBtn::InitData( int iType, int iEmptyCnt )
{
	m_iType = iType;
	m_iEmptyCnt = iEmptyCnt;

	m_iCode = 0;
	m_iPreCode = 0;
	m_iCurCnt = 0;
	m_iDecreaseCnt = 0;

	// count text
	m_CountText.ClearList();
	m_CountText2.ClearList();

	char szCount[MAX_PATH]="";
	Help::ConvertNumToStrComma( m_iCurCnt, szCount, sizeof( szCount ) );

	m_CountText.SetTextStyle( TS_NORMAL );
	m_CountText.SetTextColor( 153, 153, 153 );

	switch( m_iType )
	{
	case BTT_PIECE1:
		m_CountText.AddTextPiece( FONT_SIZE_12, STR(19) );
		break;
	case BTT_PIECE2:
		m_CountText.AddTextPiece( FONT_SIZE_12, STR(20) );
		break;
	case BTT_PIECE3:
		m_CountText.AddTextPiece( FONT_SIZE_12, STR(21) );
		break;
	case BTT_PIECE4:
		m_CountText.AddTextPiece( FONT_SIZE_12, STR(22) );
		break;
	case BTT_ADDITIVE:
		m_CountText.AddTextPiece( FONT_SIZE_12, STR(23) );
		break;
	case BTT_ALL:
		m_CountText.AddTextPiece( FONT_SIZE_12, STR(24) );
		break;
	}
}

void SelectAlchemicItemBtn::InitNewData( int iType, int iValue, int iMaxCnt )
{
	m_iType = iType;
	m_iEmptyCnt = iMaxCnt;

	m_iCode = 0;
	m_iPreCode = iValue;
	m_iCurCnt = 0;
	m_iDecreaseCnt = 0;

	m_CountText.ClearList();
	m_CountText2.ClearList();

	ioHashString szIcon = g_AlchemicMgr.GetAlchemicItemIcon( m_iPreCode );

	SAFEDELETE( m_pIconImg );
	m_pIconImg = g_UIImageSetMgr.CreateImageByFullName( szIcon );
}

void SelectAlchemicItemBtn::SetDragData( const AlchemicFuncInfo& rkInfo, int iCode )
{
	if( m_iType == BTT_PIECE3 )
	{
		ioAlchemicInventory *pInven = g_MyInfo.GetAlchemicInventory();
		if( !pInven )
			return;

		int iCurCount = pInven->GetAlchemicItemCnt( iCode );
		
		SetData( iCode, 0 );
		return;
	}

	AlchemicItemRegisterWnd *pRegisterWnd = dynamic_cast<AlchemicItemRegisterWnd*>(g_GUIMgr.FindWnd(ALCHEMIC_REGISTER_WND));
	if( pRegisterWnd )
	{
		pRegisterWnd->SetRegisterItemInfo( rkInfo.m_AlchemicType, iCode, m_iEmptyCnt, this );
	}
}

void SelectAlchemicItemBtn::SetData( int iCode, int iCnt )
{
	int iPreCode = m_iCode;
	m_iCode = iCode;
	m_iCurCnt = iCnt;

	ioHashString szIcon = g_AlchemicMgr.GetAlchemicItemIcon( iCode );

	SAFEDELETE( m_pIconImg );
	m_pIconImg = g_UIImageSetMgr.CreateImageByFullName( szIcon );

	// count text
	m_CountText.ClearList();
	m_CountText2.ClearList();

	char szCount[MAX_PATH]="";
	Help::ConvertNumToStrComma( m_iCurCnt, szCount, sizeof( szCount ) );

	m_CountText.SetTextStyle( TS_OUTLINE_FULL_3X );
	m_CountText.SetBkColor( TCT_DEFAULT_DARKGRAY );
	m_CountText.SetTextColor( TCT_DEFAULT_WHITE );
	m_CountText.AddTextPiece( FONT_SIZE_12, szCount );

	MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(g_GUIMgr.FindWnd(MY_INVENTORY_WND));
	if( pInvenWnd )
	{
		pInvenWnd->CheckAlchemicFuncEnable();
		pInvenWnd->SetAlchemicItemLockStat( m_iCode, true );

		if( iPreCode > 0 )
			pInvenWnd->SetAlchemicItemLockStat( iPreCode, false );
	}
}

void SelectAlchemicItemBtn::SetDecrease( int iCnt )
{
	m_iDecreaseCnt = iCnt;

	m_iCurCnt -= iCnt;

	// count text
	m_CountText.ClearList();
	m_CountText2.ClearList();

	char szCount[MAX_PATH]="";
	Help::ConvertNumToStrComma( m_iCurCnt, szCount, sizeof( szCount ) );

	m_CountText.SetTextStyle( TS_OUTLINE_FULL_3X );
	m_CountText.SetBkColor( TCT_DEFAULT_DARKGRAY );
	m_CountText.SetTextColor( TCT_DEFAULT_WHITE );
	m_CountText.AddTextPiece( FONT_SIZE_12, szCount );

	//
	Help::ConvertNumToStrComma( m_iDecreaseCnt, szCount, sizeof( szCount ) );

	m_CountText2.SetTextStyle( TS_OUTLINE_FULL_3X );
	m_CountText2.SetBkColor( TCT_DEFAULT_DARKGRAY );
	m_CountText2.SetTextColor( TCT_DEFAULT_RED );
	m_CountText2.AddTextPiece( FONT_SIZE_12, STR(2), szCount );
}

void SelectAlchemicItemBtn::iwm_hide()
{
	m_iCode = 0;
	m_iCurCnt = 0;
	m_iEmptyCnt = 0;
	m_iDecreaseCnt = 0;
}

void SelectAlchemicItemBtn::OnRender()
{
	switch( m_iType )
	{
		case BTT_PIECE1:
		case BTT_PIECE2:
		case BTT_PIECE3:
		case BTT_PIECE4:
		case BTT_ADDITIVE:
		case BTT_ALL:
			AlchemicRender();
			break;
		case BTT_NEW_PIECE:
		case BTT_NEW_ADDITIVE:
			NewAlchemicRender();
			break;
	}
}

void SelectAlchemicItemBtn::AlchemicRender()
{
	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	if( m_iCode <= 0 )		// empty
	{
		if( m_pEmptyFrm )
		{
			m_pEmptyFrm->SetSize( 64, 64 );
			m_pEmptyFrm->Render( iXPos, iYPos );
		}

		switch( m_iType )
		{
		case BTT_PIECE1:
			if( m_pPieceImg1 )
				m_pPieceImg1->Render( iXPos, iYPos );
			break;
		case BTT_PIECE2:
			if( m_pPieceImg2 )
				m_pPieceImg2->Render( iXPos, iYPos );
			break;
		case BTT_PIECE3:
			if( m_pPieceImg3 )
				m_pPieceImg3->Render( iXPos, iYPos );
			break;
		case BTT_PIECE4:
			if( m_pPieceImg1 )
				m_pPieceImg1->Render( iXPos, iYPos );
			break;
		case BTT_ADDITIVE:
			if( m_pAdditiveImg )
				m_pAdditiveImg->Render( iXPos, iYPos );
			break;
		case BTT_ALL:
			if( m_pAllImg )
				m_pAllImg->Render( iXPos, iYPos );
			break;
		}

		if( m_iEmptyCnt > 0 )
		{
			m_pMaxNum->SetHorzFormatting( ioUIRenderImage::HF_CENTER );
			m_pMaxNum->RenderNum( iXPos+32, iYPos+17, m_iEmptyCnt, -FLOAT1 );
		}

		if( m_CountText.IsEmpty() )
			return;

		m_CountText.PrintFullText( iXPos+32, iYPos+44, TAT_CENTER );
	}
	else
	{
		switch( m_iType )
		{
		case BTT_PIECE1:
		case BTT_PIECE2:
		case BTT_PIECE3:
		case BTT_PIECE4:
		case BTT_NEW_PIECE:
			if( m_pPieceFrm )
				m_pPieceFrm->Render( iXPos, iYPos );
			break;
		case BTT_ADDITIVE:
		case BTT_NEW_ADDITIVE:
			if( m_pAdditiveFrm )
				m_pAdditiveFrm->Render( iXPos, iYPos );
			break;
		case BTT_ALL:
			if( m_iCode > ALCHEMIC_ADDITIVE_DIV )
			{
				if( m_pAdditiveFrm )
					m_pAdditiveFrm->Render( iXPos, iYPos );
			}
			else
			{
				if( m_pPieceFrm )
					m_pPieceFrm->Render( iXPos, iYPos );
			}
			break;
		}

		if( m_pIconBackImg )
		{
			m_pIconBackImg->SetScale( 0.75f );
			m_pIconBackImg->Render( iXPos+32, iYPos+32, UI_RENDER_MULTIPLY, TFO_BILINEAR );
		}

		if( m_pIconImg )
		{
			m_pIconImg->SetScale( 0.75f );
			m_pIconImg->Render( iXPos+32, iYPos+32, UI_RENDER_NORMAL, TFO_BILINEAR );
		}

		if( IsOver() )
		{
			OnDrawOvered( iXPos, iYPos );
		}

		if( !m_CountText.IsEmpty() )
		{
			if( m_iType != BTT_PIECE3 || m_iCurCnt > 0 )
				m_CountText.PrintFullText( iXPos+61, iYPos+45, TAT_RIGHT );
		}

		if( !m_CountText2.IsEmpty() )
		{
			if( m_iType != BTT_PIECE3 || m_iCurCnt > 0 )
				m_CountText2.PrintFullText( iXPos+61, iYPos+31, TAT_RIGHT );
		}
	}
}

void SelectAlchemicItemBtn::NewAlchemicRender()
{
	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	if( m_iCode > 0 )
	{
		AlchemicRender();
	}
	else if( m_iPreCode > 0 )
	{
		if( m_pEmptyFrm )
		{
			m_pEmptyFrm->SetSize( 64, 64 );
			m_pEmptyFrm->Render( iXPos, iYPos );
		}

		if( m_pIconImg )
		{
			m_pIconImg->SetScale( 0.75f );
			m_pIconImg->SetAlpha( (float)MAX_ALPHA_RATE * 0.4f );
			m_pIconImg->Render( iXPos+32, iYPos+32, UI_RENDER_GRAY, TFO_BILINEAR );
		}
	}
	else
	{
		if( m_pEmptyFrm )
		{
			m_pEmptyFrm->SetSize( 64, 64 );
			m_pEmptyFrm->Render( iXPos, iYPos );
		}
	}
}

void SelectAlchemicItemBtn::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;

	if( m_bOver && !m_bClicked )
	{
		ioLSMouse *pMouse = g_App.GetMouse();
		if( pMouse )
		{
			pMouse->SetToolTipWndID( GetID() );
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PreSelectItemBtn::PreSelectItemBtn(void)
{
	m_iType = ALT_NONE;
	m_iCode = 0;
	m_bAlchemicEnable = false;

	m_pEmptyFrm = NULL;
	m_pItemFrm = NULL;

	m_pEmptyImg = NULL;
	m_pIconImg = NULL;
	m_pIconBackImg = NULL;
	m_nGradeType = 0;
}

PreSelectItemBtn::~PreSelectItemBtn(void)
{
	SAFEDELETE( m_pEmptyFrm );
	SAFEDELETE( m_pItemFrm );

	SAFEDELETE( m_pEmptyImg );
	SAFEDELETE( m_pIconImg );
	SAFEDELETE( m_pIconBackImg );
}

void PreSelectItemBtn::AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame )
{
	if( szType == "empty_frm" )
	{
		SAFEDELETE( m_pEmptyFrm );
		m_pEmptyFrm = pFrame;
	}
	else if( szType == "item_frm" )
	{
		SAFEDELETE( m_pItemFrm );
		m_pItemFrm = pFrame;
	}
	else
	{
		ioButton::AddRenderFrame( szType, pFrame );
	}
}

void PreSelectItemBtn::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "icon_back_image" )
	{
		SAFEDELETE( m_pIconBackImg );
		m_pIconBackImg = pImage;
	}
	else if( szType == "empty_image" )
	{
		SAFEDELETE( m_pEmptyImg );
		m_pEmptyImg = pImage;
	}
	else
	{
		ioButton::AddRenderImage( szType, pImage );	
	}	
}

void PreSelectItemBtn::InitData()
{
	m_iType = ALT_NONE;
	m_iCode = 0;
	m_bAlchemicEnable = false;
	m_nGradeType = 0;
}

void PreSelectItemBtn::SetItemValue( int iType, int iValue )
{
	m_iType = iType;
	m_iCode = iValue;
	m_nGradeType = 0;

	if( m_iCode == 0 )
	{
		m_bAlchemicEnable = false;

		SAFEDELETE( m_pIconImg );

		SetInActive();
	}
	else
	{
		ioHashString szIcon;

		switch( m_iType )
		{
		case ALT_NEW_SOLDIER:
			{
				szIcon = g_MyInfo.GetMySoldierIconName( m_iCode );

				m_bAlchemicEnable = g_AlchemicMgr.CheckEnableNewAlchemicSoldier( m_iCode );
				m_nGradeType = g_UIImageRenderMgr.GetPowerUpGradeType( m_iCode );
			}
			break;
		case ALT_NEW_ITEM:
			{
				const ioItem *pItem = g_ItemMaker.GetItemConst( m_iCode, __FUNCTION__ );
				if( pItem )
				{
					szIcon = pItem->GetItemLargeIconName();
					m_nGradeType = pItem->GetGradeType();
				}

				m_bAlchemicEnable = g_AlchemicMgr.CheckEnableNewAlchemicItem( m_iCode );
			}
			break;
		}

		SAFEDELETE( m_pIconImg );

		if( !szIcon.IsEmpty() )
			m_pIconImg = g_UIImageSetMgr.CreateImageByFullName( szIcon );

		SetActive();
	}
}

void PreSelectItemBtn::iwm_hide()
{
	InitData();
}

void PreSelectItemBtn::OnRender()
{
	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	enum
	{
		X_OFFSET	= 29,
		Y_OFFSET	= 29,
	};

	if( m_iCode <= 0 )		// empty
	{
		if( m_pEmptyFrm )
		{
			m_pEmptyFrm->Render( iXPos, iYPos );
		}

		if( m_pEmptyImg )
		{
			m_pEmptyImg->SetScale( 0.6f );
			m_pEmptyImg->Render( iXPos+X_OFFSET, iYPos+Y_OFFSET, UI_RENDER_MULTIPLY, TFO_BILINEAR );
		}
	}
	else
	{
		if( m_pItemFrm )
			m_pItemFrm->Render( iXPos, iYPos );

		if( m_pIconBackImg )
		{
			m_pIconBackImg->SetScale( 0.6f );
			m_pIconBackImg->Render( iXPos+X_OFFSET, iYPos+Y_OFFSET, UI_RENDER_MULTIPLY, TFO_BILINEAR );
		}
		
		if( m_pIconImg )
		{
			m_pIconImg->SetScale( 0.6f );

			if( m_bAlchemicEnable )
			{
				g_UIImageRenderMgr.RenderGradeIconBack( m_nGradeType, iXPos + X_OFFSET, iYPos + Y_OFFSET, 0.6f, UI_RENDER_NORMAL, TFO_BILINEAR );
				m_pIconImg->Render( iXPos+X_OFFSET, iYPos+Y_OFFSET, UI_RENDER_NORMAL, TFO_BILINEAR );
			}
			else
			{
				g_UIImageRenderMgr.RenderGradeIconBack( m_nGradeType, iXPos + X_OFFSET, iYPos + Y_OFFSET, 0.6f, UI_RENDER_GRAY, TFO_BILINEAR );
				m_pIconImg->Render( iXPos+X_OFFSET, iYPos+Y_OFFSET, UI_RENDER_GRAY, TFO_BILINEAR );
			}
		}

		if( IsOver() )
		{
			OnDrawOvered( iXPos, iYPos );
		}
	}
}

void PreSelectItemBtn::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;

	if( m_bOver && !m_bClicked )
	{
		ioLSMouse *pMouse = g_App.GetMouse();
		if( pMouse )
		{
			pMouse->SetToolTipWndID( GetID() );
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AlchemicFuncWnd::AlchemicFuncWnd(void)
{
	m_CurFuncInfo.Init();
	m_bActionEnable = false;
	m_iCurSelectFunc = 0;

	m_FuncState = AFS_SELECT;

	m_pManual = NULL;

	m_pResultIcon = NULL;
	m_pPesoIcon = NULL;
	m_pIconBack = NULL;

	m_pStripDark = NULL;
	m_pStripLight = NULL;

	m_pLightImage = NULL;

	m_pMagicCircle = NULL;
	m_pMagicCircleArrow = NULL;
	m_pSmallCircle = NULL;

	m_pNewAlchemicIcon = NULL;
	m_pNewAlchemicIconBack = NULL;

	m_pEffect = NULL;
	m_pEffect2 = NULL;

	m_dwResultStartTime = 0;
	m_dwResultEndTime = 0;

	m_dwFinalResultStartTime = 0;

	m_dwRollingEffectCheckTime = 0;

	ClearProcessDesc();
	ClearPreSelectDesc();

	m_vMagicEffectInfoList.clear();
	m_dwMagicEffectCreateTime = 0;
	m_nGradeType = 0;
}

AlchemicFuncWnd::~AlchemicFuncWnd(void)
{
	ClearProcessDesc();
	ClearPreSelectDesc();

	SAFEDELETE(m_pResultIcon);
	SAFEDELETE(m_pPesoIcon);
	SAFEDELETE(m_pIconBack);

	SAFEDELETE(m_pStripDark);
	SAFEDELETE(m_pStripLight);

	SAFEDELETE(m_pLightImage);

	SAFEDELETE(m_pMagicCircle);
	SAFEDELETE(m_pMagicCircleArrow);
	SAFEDELETE(m_pSmallCircle);

	SAFEDELETE(m_pNewAlchemicIcon);
	SAFEDELETE(m_pNewAlchemicIconBack);

	SAFEDELETE(m_pEffect);
	SAFEDELETE(m_pEffect2);

	m_vMagicEffectInfoList.clear();
}

void AlchemicFuncWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "PesoIcon" )
	{
		SAFEDELETE( m_pPesoIcon );
		m_pPesoIcon = pImage;
	}
	else if( szType == "Icon_back" )
	{
		SAFEDELETE( m_pIconBack );
		m_pIconBack = pImage;
	}
	else if( szType == "strip_gray_dark" )
	{
		SAFEDELETE( m_pStripDark );
		m_pStripDark = pImage;
	}
	else if( szType == "strip_gray_light" )
	{
		SAFEDELETE( m_pStripLight );
		m_pStripLight = pImage;
	}
	else if( szType == "light_image" )
	{
		SAFEDELETE( m_pLightImage );
		m_pLightImage = pImage;
	}
	else if( szType == "magic_circle" )
	{
		SAFEDELETE( m_pMagicCircle );
		m_pMagicCircle = pImage;
	}
	else if( szType == "small_circle" )
	{
		SAFEDELETE( m_pSmallCircle );
		m_pSmallCircle = pImage;
	}
	else if( szType == "new_alchemic_icon_back" )
	{
		SAFEDELETE( m_pNewAlchemicIconBack );
		m_pNewAlchemicIconBack = pImage;
	}
	else if( szType == "magic_circle_arrow" )
	{
		SAFEDELETE( m_pMagicCircleArrow );
		m_pMagicCircleArrow = pImage;
	}
	else
	{
		ioWnd::AddRenderImage( szType, pImage );	
	}	
}

void AlchemicFuncWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	char szKeyName[MAX_PATH]="";
	ioHashString szTemp;

	ZeroMemory( szKeyName, sizeof( szKeyName ) );
	StringCbPrintf( szKeyName, sizeof( szKeyName ), "rolling_sound" );
	m_szRollingSound = xElement.GetStringAttribute( szKeyName );

	ZeroMemory( szKeyName, sizeof( szKeyName ) );
	StringCbPrintf( szKeyName, sizeof( szKeyName ), "success_sound" );
	m_szSuccessSound = xElement.GetStringAttribute( szKeyName );

	ZeroMemory( szKeyName, sizeof( szKeyName ) );
	StringCbPrintf( szKeyName, sizeof( szKeyName ), "fail_sound" );
	m_szFailSound = xElement.GetStringAttribute( szKeyName );

	m_fScaleRate = xElement.GetFloatAttribute_e( "scale_rate" );
	m_fAlphaRate = xElement.GetFloatAttribute_e( "alpha_rate" );
	m_fSlopeRate = xElement.GetFloatAttribute_e( "slope_rate" );

	m_dwRollingEffectTime = xElement.GetIntAttribute_e( "Effect_Gap_Time" );
	m_vRollingEffect.clear();

	int iMax = xElement.GetIntAttribute_e( "Max_Rolling_Effect" );
	m_vRollingEffect.reserve( iMax );
	for (int i = 0; i < iMax ; i++)
	{
		char szName[MAX_PATH]="";
		StringCbPrintf( szName, sizeof( szName ), "Rolling_Effect_%d", i+1 );
		ioHashString szEffect = xElement.GetStringAttribute( szName );
		if( szEffect == "" )
			break;
		m_vRollingEffect.push_back( szEffect );
	}

	m_vEndEfffect_S.clear();
	iMax = xElement.GetIntAttribute_e( "Max_Success_Effect" );
	m_vEndEfffect_S.reserve( iMax );
	for (int i = 0; i < iMax ; i++)
	{
		char szName[MAX_PATH]="";
		StringCbPrintf( szName, sizeof( szName ), "Success_Effect_%d", i+1 );
		ioHashString szEffect = xElement.GetStringAttribute( szName );
		if( szEffect == "" )
			break;
		m_vEndEfffect_S.push_back( szEffect );
	}

	m_vEndEfffect_F.clear();
	iMax = xElement.GetIntAttribute_e( "Max_Fail_Effect" );
	m_vEndEfffect_F.reserve( iMax );
	for (int i = 0; i < iMax ; i++)
	{
		char szName[MAX_PATH]="";
		StringCbPrintf( szName, sizeof( szName ), "Fail_Effect_%d", i+1 );
		ioHashString szEffect = xElement.GetStringAttribute( szName );
		if( szEffect == "" )
			break;
		m_vEndEfffect_F.push_back( szEffect );
	}

	// result effect
	m_dwResultEffectTime = xElement.GetIntAttribute_e( "result_effect_Time" );

	// final effect
	m_dwSuccessEffectTime = xElement.GetIntAttribute_e( "success_effect_Time" );
	m_dwDestroyEffectTime = xElement.GetIntAttribute_e( "destroy_effect_Time" );
}

void AlchemicFuncWnd::iwm_create()
{
	m_InfoList.clear();
	g_AlchemicMgr.GetAlchemicFuncInfo( m_InfoList );
}

void AlchemicFuncWnd::iwm_show()
{
	m_CurFuncInfo.Init();
	m_bActionEnable = false;
	m_iCurSelectFunc = 0;

	ClearProcessDesc();
	ClearPreSelectDesc();

	m_vMagicEffectInfoList.clear();
	m_dwMagicEffectCreateTime = 0;

	m_FuncState = AFS_SELECT;

	m_dwResultStartTime = 0;
	m_dwResultEndTime = 0;

	m_dwFinalResultStartTime = 0;

	m_iNewAlchemicPiece = 0;
	m_iNewAlchemicAdditive = 0;
	m_iCurNewAlchemicSelectValue = 0;
	m_nGradeType = 0;

	m_pManual = ioAlchemicFuncManualMgr::GetSingletonPtr();

	SelectAlchemicFuncBtn *pTarget = dynamic_cast<SelectAlchemicFuncBtn*>(FindChildWnd(ID_ALCHEMIC_FUNC_BTN));
	if( pTarget )
	{
		pTarget->InitData();
	}

	AlchemicFuncInfo kFuncInfo;
	g_AlchemicMgr.FindAlchemicFuncFirst( kFuncInfo );
	ChangeFunc( kFuncInfo, false );
}

void AlchemicFuncWnd::ChangeFunc( AlchemicFuncInfo kFuncInfo, bool bCheckCode )
{
	bool bCurCheckCode = bCheckCode;

	if( m_FuncState != AFS_PRE_SELECT )
	{
		if( m_CurFuncInfo.m_AlchemicType == ALT_NEW_SOLDIER || m_CurFuncInfo.m_AlchemicType == ALT_NEW_ITEM )
			bCurCheckCode = false;
	}

	if( bCurCheckCode )
	{
		if(m_CurFuncInfo.m_iCode == kFuncInfo.m_iCode)
			return;
	}

	ClearResultData();

	m_FuncState = AFS_SELECT;

	MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(g_GUIMgr.FindWnd(MY_INVENTORY_WND));
	if( pInvenWnd )
	{
		pInvenWnd->SetAlchemicItemLockClear();
	}

	m_SuccessRateDesc.ClearList();

	HideChildWnd( ID_ACTION_BTN );
	HideChildWnd( ID_ACTION_BTN2 );
	HideChildWnd( ID_ACTION_BTN3 );

	HideChildWnd( ID_ALCHEMIC_FUNC_BTN );
	HideChildWnd( ID_ALCHEMIC_FUNC_WND );

	HideChildWnd( ID_SELECT_ITEM_BTN1 );
	HideChildWnd( ID_SELECT_ITEM_BTN2 );
	HideChildWnd( ID_SELECT_ITEM_BTN3 );
	HideChildWnd( ID_SELECT_ITEM_BTN4 );

	HideChildWnd( ID_ADDITIVE_ITEM_BTN );

	HideChildWnd( ID_RESULT_BTN );

	// pre new select
	for( int i=0; i < MAX_PRE_SELECT_ITEM; ++ i )
	{
		HideChildWnd( ID_PRE_SELECT_ITEM_BTN1+i );
	}

	HideChildWnd( ID_PAGE_PRE_BTN );
	HideChildWnd( ID_PAGE_NEXT_BTN );

	m_CurFuncInfo = kFuncInfo;

	switch( m_CurFuncInfo.m_AlchemicType )
	{
	case ALT_SOLDIER:
		SetAlchemicSoldier();
		break;
	case ALT_ITEM:
		SetAlchemicItem();
		break;
	case ALT_CHANGE:
		SetAlchemicChange();
		break;
	case ALT_EXCHANGE:
		SetAlchemicExchange();
		break;
	case ALT_SELL:
		SetAlchemicSell();
		break;
	case ALT_NEW_SOLDIER:
		SetPreNewAlchemicSoldier();
		break;
	case ALT_NEW_ITEM:
		SetPreNewAlchemicItem();
		break;
	}
}

void AlchemicFuncWnd::SetAlchemicSoldier()
{
	ReBatchButton( RBB_SELECT_ONE );

	SelectAlchemicFuncBtn *pFuncBtn = dynamic_cast<SelectAlchemicFuncBtn*>(FindChildWnd(ID_ALCHEMIC_FUNC_BTN));
	if( pFuncBtn )
	{
		// Show 부터 할 것
		pFuncBtn->ShowWnd();
		pFuncBtn->SetSelectAlchemicFunc( m_CurFuncInfo.m_iCode );
	}

	SelectAlchemicItemBtn *pItemBtn1 = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN1));
	if( pItemBtn1 )
	{
		pItemBtn1->SetWndPos( 27, 134 );
		pItemBtn1->InitData( SelectAlchemicItemBtn::BTT_PIECE1, m_CurFuncInfo.m_iMaxCnt1 );
		pItemBtn1->ShowWnd();
	}

	SelectAlchemicItemBtn *pItemBtn2 = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN2));
	if( pItemBtn2 )
	{
		pItemBtn2->SetWndPos( 167, 134 );
		pItemBtn2->InitData( SelectAlchemicItemBtn::BTT_PIECE1, m_CurFuncInfo.m_iMaxCnt2 );
		pItemBtn2->ShowWnd();
	}

	SelectAlchemicItemBtn *pItemBtn3 = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN3));
	if( pItemBtn3 )
	{
		pItemBtn3->SetWndPos( 27, 204 );
		pItemBtn3->InitData( SelectAlchemicItemBtn::BTT_PIECE1, m_CurFuncInfo.m_iMaxCnt3 );
		pItemBtn3->ShowWnd();
	}

	SelectAlchemicItemBtn *pItemBtn4 = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN4));
	if( pItemBtn4 )
	{
		pItemBtn4->SetWndPos( 167, 204 );
		pItemBtn4->InitData( SelectAlchemicItemBtn::BTT_PIECE1, m_CurFuncInfo.m_iMaxCnt4 );
		pItemBtn4->ShowWnd();
	}

	SelectAlchemicItemBtn *pAdditiveBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_ADDITIVE_ITEM_BTN));
	if( pAdditiveBtn )
	{
		pAdditiveBtn->SetWndPos( 97, 169 );
		pAdditiveBtn->InitData( SelectAlchemicItemBtn::BTT_ADDITIVE, m_CurFuncInfo.m_iMaxAdditive );
		pAdditiveBtn->ShowWnd();
	}

	CheckActionBtnText();
	CheckSuccessRateText();
	CheckProcessText();
}

void AlchemicFuncWnd::SetAlchemicItem()
{
	ReBatchButton( RBB_SELECT_ONE );

	SelectAlchemicFuncBtn *pFuncBtn = dynamic_cast<SelectAlchemicFuncBtn*>(FindChildWnd(ID_ALCHEMIC_FUNC_BTN));
	if( pFuncBtn )
	{
		// Show 부터 할 것
		pFuncBtn->ShowWnd();
		pFuncBtn->SetSelectAlchemicFunc( m_CurFuncInfo.m_iCode );
	}

	SelectAlchemicItemBtn *pItemBtn1 = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN1));
	if( pItemBtn1 )
	{
		pItemBtn1->SetWndPos( 27, 134 );
		pItemBtn1->InitData( SelectAlchemicItemBtn::BTT_PIECE1, m_CurFuncInfo.m_iMaxCnt1 );
		pItemBtn1->ShowWnd();
	}

	SelectAlchemicItemBtn *pItemBtn2 = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN2));
	if( pItemBtn2 )
	{
		pItemBtn2->SetWndPos( 167, 134 );
		pItemBtn2->InitData( SelectAlchemicItemBtn::BTT_PIECE1, m_CurFuncInfo.m_iMaxCnt2 );
		pItemBtn2->ShowWnd();
	}

	SelectAlchemicItemBtn *pItemBtn3 = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN3));
	if( pItemBtn3 )
	{
		pItemBtn3->SetWndPos( 27, 204 );
		pItemBtn3->InitData( SelectAlchemicItemBtn::BTT_PIECE1, m_CurFuncInfo.m_iMaxCnt3 );
		pItemBtn3->ShowWnd();
	}

	SelectAlchemicItemBtn *pItemBtn4 = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN4));
	if( pItemBtn4 )
	{
		pItemBtn4->SetWndPos( 167, 204 );
		pItemBtn4->InitData( SelectAlchemicItemBtn::BTT_PIECE1, m_CurFuncInfo.m_iMaxCnt4 );
		pItemBtn4->ShowWnd();
	}

	SelectAlchemicItemBtn *pAdditiveBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_ADDITIVE_ITEM_BTN));
	if( pAdditiveBtn )
	{
		pAdditiveBtn->SetWndPos( 97, 169 );
		pAdditiveBtn->InitData( SelectAlchemicItemBtn::BTT_ADDITIVE, m_CurFuncInfo.m_iMaxAdditive );
		pAdditiveBtn->ShowWnd();
	}

	CheckActionBtnText();
	CheckSuccessRateText();
	CheckProcessText();
}

void AlchemicFuncWnd::SetAlchemicChange()
{
	ReBatchButton( RBB_SELECT_ONE );

	SelectAlchemicFuncBtn *pFuncBtn = dynamic_cast<SelectAlchemicFuncBtn*>(FindChildWnd(ID_ALCHEMIC_FUNC_BTN));
	if( pFuncBtn )
	{
		// Show 부터 할 것
		pFuncBtn->ShowWnd();
		pFuncBtn->SetSelectAlchemicFunc( m_CurFuncInfo.m_iCode );
	}

	SelectAlchemicItemBtn *pItemBtn1 = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN1));
	if( pItemBtn1 )
	{
		pItemBtn1->SetWndPos( 62, 134 );
		pItemBtn1->InitData( SelectAlchemicItemBtn::BTT_PIECE2, m_CurFuncInfo.m_iMaxCnt1 );
		pItemBtn1->ShowWnd();
	}

	SelectAlchemicItemBtn *pItemBtn2 = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN2));
	if( pItemBtn2 )
	{
		pItemBtn2->SetWndPos( 132, 134 );
		pItemBtn2->InitData( SelectAlchemicItemBtn::BTT_PIECE2, m_CurFuncInfo.m_iMaxCnt2 );
		pItemBtn2->ShowWnd();
	}

	SelectAlchemicItemBtn *pItemBtn3 = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN3));
	if( pItemBtn3 )
	{
		pItemBtn3->SetWndPos( 62, 204 );
		pItemBtn3->InitData( SelectAlchemicItemBtn::BTT_PIECE2, m_CurFuncInfo.m_iMaxCnt3 );
		pItemBtn3->ShowWnd();
	}

	SelectAlchemicItemBtn *pItemBtn4 = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN4));
	if( pItemBtn4 )
	{
		pItemBtn4->SetWndPos( 132, 204 );
		pItemBtn4->InitData( SelectAlchemicItemBtn::BTT_PIECE2, m_CurFuncInfo.m_iMaxCnt4 );
		pItemBtn4->ShowWnd();
	}

	CheckActionBtnText();
	CheckSuccessRateText();
	CheckProcessText();
}

void AlchemicFuncWnd::SetAlchemicExchange()
{
	ReBatchButton( RBB_SELECT_ONE );

	SelectAlchemicFuncBtn *pFuncBtn = dynamic_cast<SelectAlchemicFuncBtn*>(FindChildWnd(ID_ALCHEMIC_FUNC_BTN));
	if( pFuncBtn )
	{
		// Show 부터 할 것
		pFuncBtn->ShowWnd();
		pFuncBtn->SetSelectAlchemicFunc( m_CurFuncInfo.m_iCode );
	}

	SelectAlchemicItemBtn *pItemBtn1 = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN1));
	if( pItemBtn1 )
	{
		pItemBtn1->SetWndPos( 27, 134 );
		pItemBtn1->InitData( SelectAlchemicItemBtn::BTT_PIECE4, m_CurFuncInfo.m_iMaxCnt1 );
		pItemBtn1->ShowWnd();
	}

	SelectAlchemicItemBtn *pItemBtn2 = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN2));
	if( pItemBtn2 )
	{
		pItemBtn2->SetWndPos( 167, 169 );
		pItemBtn2->InitData( SelectAlchemicItemBtn::BTT_PIECE3, m_CurFuncInfo.m_iMaxCnt2 );
		pItemBtn2->ShowWnd();
	}

	/*
	SelectAlchemicItemBtn *pAdditiveBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_ADDITIVE_ITEM_BTN));
	if( pAdditiveBtn )
	{
		pAdditiveBtn->SetWndPos( 27, 204 );
		pAdditiveBtn->InitData( SelectAlchemicItemBtn::BTT_ADDITIVE, m_CurFuncInfo.m_iMaxAdditive );
		pAdditiveBtn->ShowWnd();
	}
	*/

	SelectAlchemicItemBtn *pAdditiveBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_ADDITIVE_ITEM_BTN));
	if( pAdditiveBtn )
	{
		m_iNewAlchemicAdditive = g_AlchemicMgr.GetExchangeAdditive();
		pAdditiveBtn->SetWndPos( 27, 204 );
		pAdditiveBtn->InitNewData( SelectAlchemicItemBtn::BTT_NEW_ADDITIVE, m_iNewAlchemicAdditive, m_CurFuncInfo.m_iMaxAdditive );
		pAdditiveBtn->ShowWnd();
	}

	//
	MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(g_GUIMgr.FindWnd(MY_INVENTORY_WND));
	if( pInvenWnd )
	{
		pInvenWnd->SetAlchemicItemLockAllAdditive();
		pInvenWnd->SetAlchemicItemLockStat( m_iNewAlchemicAdditive, false );
	}

	CheckActionBtnText();
	CheckSuccessRateText();
	CheckProcessText();
}

void AlchemicFuncWnd::SetAlchemicSell()
{
	ReBatchButton( RBB_SELECT_ONE );

	SelectAlchemicFuncBtn *pFuncBtn = dynamic_cast<SelectAlchemicFuncBtn*>(FindChildWnd(ID_ALCHEMIC_FUNC_BTN));
	if( pFuncBtn )
	{
		// Show 부터 할 것
		pFuncBtn->ShowWnd();
		pFuncBtn->SetSelectAlchemicFunc( m_CurFuncInfo.m_iCode );
	}

	SelectAlchemicItemBtn *pItemBtn1 = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN1));
	if( pItemBtn1 )
	{
		pItemBtn1->SetWndPos( 62, 134 );
		pItemBtn1->InitData( SelectAlchemicItemBtn::BTT_ALL, m_CurFuncInfo.m_iMaxCnt1 );
		pItemBtn1->ShowWnd();
	}

	SelectAlchemicItemBtn *pItemBtn2 = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN2));
	if( pItemBtn2 )
	{
		pItemBtn2->SetWndPos( 132, 134 );
		pItemBtn2->InitData( SelectAlchemicItemBtn::BTT_ALL, m_CurFuncInfo.m_iMaxCnt2 );
		pItemBtn2->ShowWnd();
	}

	SelectAlchemicItemBtn *pItemBtn3 = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN3));
	if( pItemBtn3 )
	{
		pItemBtn3->SetWndPos( 62, 204 );
		pItemBtn3->InitData( SelectAlchemicItemBtn::BTT_ALL, m_CurFuncInfo.m_iMaxCnt3 );
		pItemBtn3->ShowWnd();
	}

	SelectAlchemicItemBtn *pItemBtn4 = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN4));
	if( pItemBtn4 )
	{
		pItemBtn4->SetWndPos( 132, 204 );
		pItemBtn4->InitData( SelectAlchemicItemBtn::BTT_ALL, m_CurFuncInfo.m_iMaxCnt4 );
		pItemBtn4->ShowWnd();
	}

	CheckActionBtnText();
	CheckSuccessRateText();
	CheckProcessText();
}

void AlchemicFuncWnd::CheckActionBtnText()
{
	ioButton *pAction = dynamic_cast<ioButton*>(FindChildWnd(ID_ACTION_BTN));
	if( !pAction )
		return;

	char szBuf[MAX_PATH] = "";

	switch( m_CurFuncInfo.m_AlchemicType )
	{
	case ALT_SOLDIER:
	case ALT_NEW_SOLDIER:
		if( m_FuncState == AFS_RESULT )
			SafeSprintf( szBuf, sizeof(szBuf), STR(11) );
		else
			SafeSprintf( szBuf, sizeof(szBuf), STR(12) );
		break;
	case ALT_ITEM:
	case ALT_NEW_ITEM:
		if( m_FuncState == AFS_RESULT )
			SafeSprintf( szBuf, sizeof(szBuf), STR(13) );
		else
			SafeSprintf( szBuf, sizeof(szBuf), STR(14) );
		break;
	case ALT_CHANGE:
		if( m_FuncState == AFS_RESULT )
			SafeSprintf( szBuf, sizeof(szBuf), STR(15) );
		else
			SafeSprintf( szBuf, sizeof(szBuf), STR(16) );
		break;
	case ALT_EXCHANGE:
		if( m_FuncState == AFS_RESULT )
			SafeSprintf( szBuf, sizeof(szBuf), STR(17) );
		else
			SafeSprintf( szBuf, sizeof(szBuf), STR(18) );
		break;
	case ALT_SELL:
		if( m_FuncState == AFS_RESULT )
			SafeSprintf( szBuf, sizeof(szBuf), STR(19) );
		else
			SafeSprintf( szBuf, sizeof(szBuf), STR(20) );
		break;
	}

	if( pAction )
	{
		pAction->SetTitleText( szBuf );

		if( m_FuncState == AFS_RESULT )
			pAction->SetInActive();
	}
}

void AlchemicFuncWnd::CheckSuccessRateText()
{
	char szFirst[MAX_PATH] = "";
	char szSecond[MAX_PATH] = {0,};
	char szBuf[MAX_PATH] = {0,};

	int iBtn1, iBtn2, iBtn3, iBtn4;
	int iAdd;

	iBtn1 = iBtn2 = iBtn3 = iBtn4 = 0;
	iAdd = 0;

	SelectAlchemicItemBtn *pItemBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN1));
	if( pItemBtn )
		iBtn1 = pItemBtn->GetCount();

	pItemBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN2));
	if( pItemBtn )
		iBtn2 = pItemBtn->GetCount();

	pItemBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN3));
	if( pItemBtn )
		iBtn3 = pItemBtn->GetCount();

	pItemBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN4));
	if( pItemBtn )
		iBtn4 = pItemBtn->GetCount();

	pItemBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_ADDITIVE_ITEM_BTN));
	if( pItemBtn )
		iAdd = pItemBtn->GetCount();

	switch( m_CurFuncInfo.m_AlchemicType )
	{
	case ALT_SOLDIER:
	case ALT_ITEM:
		{
			SafeSprintf( szFirst, sizeof(szFirst), STR(20) );

			float fMaxCnt = m_CurFuncInfo.m_iMaxCnt1 + m_CurFuncInfo.m_iMaxCnt2 + m_CurFuncInfo.m_iMaxCnt3 + m_CurFuncInfo.m_iMaxCnt4;
			float fCurCnt = (iBtn1+iBtn2+iBtn3+iBtn4) + (g_AlchemicMgr.GetSoldierConst()*iAdd);
			float fSuccessRate = fCurCnt / fMaxCnt;

			if( fCurCnt <= 0.0f )
			{
				SafeSprintf( szSecond, sizeof(szSecond), STR(21) );
			}
			else
			{
				if( fSuccessRate < 0.25f )
					SafeSprintf( szSecond, sizeof(szSecond), STR(22) );
				else if( fSuccessRate < FLOAT05 )
					SafeSprintf( szSecond, sizeof(szSecond), STR(23) );
				else if( fSuccessRate < 0.75f )
					SafeSprintf( szSecond, sizeof(szSecond), STR(24) );
				else
					SafeSprintf( szSecond, sizeof(szSecond), STR(25) );
			}
		}
		break;
	case ALT_CHANGE:
		SafeSprintf( szFirst, sizeof(szFirst), STR(26) );
		SafeSprintf( szSecond, sizeof(szSecond), STR(27) );
		break;
	case ALT_EXCHANGE:
		{
			SafeSprintf( szFirst, sizeof(szFirst), STR(28) );

			if( m_CurFuncInfo.m_iMaxAdditive <= 0 )
				SafeSprintf( szSecond, sizeof(szSecond), STR(29) );
			else
			{
				// 조각 갯수에 곱할 비율 계산
				float fMinRate = g_AlchemicMgr.GetMinExchangeRate();
				float fMaxRate = g_AlchemicMgr.GetMaxExchangeRate();
				float fGapRate = fMaxRate - fMinRate;
	
				float fChangeRate = (float)iAdd / m_CurFuncInfo.m_iMaxAdditive;
				float fResultRate = fMinRate + (fChangeRate * fGapRate);

				if( fResultRate <= 0.25f )
					SafeSprintf( szSecond, sizeof(szSecond), STR(30) );
				else if( fResultRate <= FLOAT05 )
					SafeSprintf( szSecond, sizeof(szSecond), STR(31) );
				else if( fResultRate <= 0.75f )
					SafeSprintf( szSecond, sizeof(szSecond), STR(32) );
				else
					SafeSprintf( szSecond, sizeof(szSecond), STR(33) );
			}
		}
		break;
	case ALT_SELL:
		{
			SafeSprintf( szFirst, sizeof(szFirst), STR(34) );

			int iTotalCnt = iBtn1 + iBtn2 + iBtn3 + iBtn4;
			if( iTotalCnt <= 0 )
				SafeSprintf( szSecond, sizeof(szSecond), STR(35) );
			else
			{
				int iPeso = iTotalCnt * g_AlchemicMgr.GetSellConst();
				Help::ConvertNumToStrComma( iPeso, szBuf, sizeof(szBuf) );

				SafeSprintf( szSecond, sizeof(szSecond), STR(36), szBuf );
			}
		}
		break;
	case ALT_NEW_SOLDIER:
	case ALT_NEW_ITEM:
		{
			SafeSprintf( szSecond, sizeof(szSecond), STR(37) );

			ioAlchemicInventory *pInven = g_MyInfo.GetAlchemicInventory();
			if( pInven )
			{
				if( !pInven->HasAlchemicItem( m_iNewAlchemicPiece ) || !pInven->HasAlchemicItem( m_iNewAlchemicAdditive ) )
					SafeSprintf( szSecond, sizeof(szSecond), STR(38) );
			}
		}
		break;
	}

	m_SuccessRateDesc.ClearList();

	m_SuccessRateDesc.SetTextStyle( TS_NORMAL );
	m_SuccessRateDesc.SetBkColor( 0, 0, 0 );

	if( strcmp(szFirst, "") != 0 )
	{
		m_SuccessRateDesc.SetTextColor( TCT_DEFAULT_DARKGRAY );
		m_SuccessRateDesc.AddTextPiece( FONT_SIZE_13, STR(39), szFirst );
	}
	
	if( strcmp(szSecond, "") != 0 )
	{
		m_SuccessRateDesc.SetTextColor( TCT_DEFAULT_RED );
		m_SuccessRateDesc.AddTextPiece( FONT_SIZE_13, STR(40), szSecond );
	}
	
}

void AlchemicFuncWnd::CheckProcessText()
{
	ClearProcessDesc();

	ioHashString szDesc[4];
	DWORD dwColor[4];

	for( int i=0; i < 4; ++i )
	{
		dwColor[i] = TCT_DEFAULT_DARKGRAY;
	}

	char szBuf[MAX_PATH] = "";

	switch( m_CurFuncInfo.m_AlchemicType )
	{
	case ALT_SOLDIER:
		if( m_FuncState == AFS_RESULT )
		{
			szDesc[2] = STR(39);
			dwColor[2] = TCT_DEFAULT_BLUE;
			szDesc[3] = STR(40);
			dwColor[3] = TCT_DEFAULT_GREEN;
		}
		else if( m_FuncState == AFS_FINAL_RESULT )
		{
			if( m_iResultType == ALCHEMIC_SOLDIER_SUCC )
			{
				szDesc[0] = g_MyInfo.GetClassName( m_iResultValue );
				dwColor[0] = TCT_DEFAULT_DARKGRAY;

				if( m_CurFuncInfo.m_iPeriodValue == 0 )
				{
					szDesc[1] = STR(41);
					dwColor[1] = TCT_DEFAULT_BLUE;
				}
				else
				{
					int iTotalLimitDate = m_CurFuncInfo.m_iPeriodValue / 60;
					int iHour   = iTotalLimitDate / 60;      
					int iMinute = iTotalLimitDate % 60;
					if( iMinute == 0 )
						SafeSprintf( szBuf, sizeof(szBuf), STR(42), iHour );
					else
						SafeSprintf( szBuf, sizeof(szBuf), STR(43), iHour, iMinute );

					szDesc[1] = szBuf;
					dwColor[1] = TCT_DEFAULT_BLUE;
				}

				szDesc[2] = STR(44);
				dwColor[2] = TCT_DEFAULT_BLUE;
				szDesc[3] = STR(45);
				dwColor[3] = TCT_DEFAULT_GREEN;
			}
			else if( m_iResultType == ALCHEMIC_SOLDIER_FAIL )
			{
				szDesc[0] = STR(46);
				dwColor[0] = TCT_DEFAULT_DARKGRAY;

				int iTotalCnt = m_iResultValue1+m_iResultValue2+m_iResultValue3+m_iResultValue4;
				SafeSprintf( szBuf, sizeof(szBuf), STR(47), iTotalCnt );
				szDesc[1] = szBuf;
				dwColor[1] = TCT_DEFAULT_RED;

				szDesc[3] = STR(48);
				dwColor[3] = TCT_DEFAULT_RED;
			}
		}
		break;
	case ALT_ITEM:
		if( m_FuncState == AFS_RESULT )
		{
			szDesc[2] = STR(49);
			dwColor[2] = TCT_DEFAULT_BLUE;
			szDesc[3] = STR(50);
			dwColor[3] = TCT_DEFAULT_GREEN;
		}
		else if( m_FuncState == AFS_FINAL_RESULT )
		{
			if( m_iResultType == ALCHEMIC_ITEM_SUCC )
			{
				const ioItem* pItem = g_ItemMaker.GetItemConst( m_iResultValue, __FUNCTION__ );
				if( pItem )
					szDesc[0] = pItem->GetName();

				dwColor[0] = TCT_DEFAULT_DARKGRAY;

				if( m_CurFuncInfo.m_iPeriodValue == 0 )
				{
					szDesc[1] = STR(51);
					dwColor[1] = TCT_DEFAULT_BLUE;
				}
				else
				{
					if( (m_CurFuncInfo.m_iPeriodValue/24) > 0 )
						SafeSprintf( szBuf, sizeof(szBuf), STR(52), m_CurFuncInfo.m_iPeriodValue/24 );
					else
						SafeSprintf( szBuf, sizeof(szBuf), STR(53), m_CurFuncInfo.m_iPeriodValue );

					szDesc[1] = szBuf;
					dwColor[1] = TCT_DEFAULT_BLUE;
				}

				szDesc[2] = STR(54);
				dwColor[2] = TCT_DEFAULT_BLUE;
				szDesc[3] = STR(55);
				dwColor[3] = TCT_DEFAULT_GREEN;
			}
			else if( m_iResultType == ALCHEMIC_ITEM_FAIL )
			{
				szDesc[0] = STR(56);
				dwColor[0] = TCT_DEFAULT_DARKGRAY;

				int iTotalCnt = m_iResultValue1+m_iResultValue2+m_iResultValue3+m_iResultValue4;
				SafeSprintf( szBuf, sizeof(szBuf), STR(57), iTotalCnt );
				szDesc[1] = szBuf;
				dwColor[1] = TCT_DEFAULT_RED;

				szDesc[3] = STR(58);
				dwColor[3] = TCT_DEFAULT_RED;
			}
		}
		break;
	case ALT_CHANGE:
		if( m_FuncState == AFS_RESULT )
		{
			szDesc[2] = STR(59);
			dwColor[2] = TCT_DEFAULT_BLUE;
			szDesc[3] = STR(60);
			dwColor[3] = TCT_DEFAULT_GREEN;
		}
		else if( m_FuncState == AFS_FINAL_RESULT )
		{
			SafeSprintf( szBuf, sizeof(szBuf), STR(61), g_MyInfo.GetClassName(m_iChangeCode) );
			szDesc[0] = szBuf;
			dwColor[0] = TCT_DEFAULT_DARKGRAY;

			SafeSprintf( szBuf, sizeof(szBuf), STR(62), m_iChangeCnt );
			szDesc[1] = szBuf;
			dwColor[1] = TCT_DEFAULT_BLUE;

			szDesc[3] = STR(63);
			dwColor[3] = TCT_DEFAULT_BLUE;
		}
		break;
	case ALT_EXCHANGE:
		if( m_FuncState == AFS_RESULT )
		{
			szDesc[2] = STR(64);
			dwColor[2] = TCT_DEFAULT_BLUE;
			szDesc[3] = STR(65);
			dwColor[3] = TCT_DEFAULT_GREEN;
		}
		else if( m_FuncState == AFS_FINAL_RESULT )
		{
			SafeSprintf( szBuf, sizeof(szBuf), STR(66), g_MyInfo.GetClassName(m_iChangeCode) );
			szDesc[0] = szBuf;
			dwColor[0] = TCT_DEFAULT_DARKGRAY;

			SafeSprintf( szBuf, sizeof(szBuf), STR(67), m_iChangeCnt );
			szDesc[1] = szBuf;
			dwColor[1] = TCT_DEFAULT_BLUE;

			szDesc[3] = STR(68);
			dwColor[3] = TCT_DEFAULT_BLUE;
		}
		break;
	case ALT_SELL:
		if( m_FuncState == AFS_RESULT )
		{
			szDesc[2] = STR(69);
			dwColor[2] = TCT_DEFAULT_BLUE;
			szDesc[3] = STR(70);
			dwColor[3] = TCT_DEFAULT_GREEN;
		}
		else if( m_FuncState == AFS_FINAL_RESULT )
		{
			int iTotalCnt = m_iResultValue1+m_iResultValue2+m_iResultValue3+m_iResultValue4;

			SafeSprintf( szBuf, sizeof(szBuf), STR(71), iTotalCnt );
			szDesc[0] = szBuf;
			dwColor[0] = TCT_DEFAULT_DARKGRAY;

			Help::ConvertNumToStrComma( m_iChangePeso, szBuf, sizeof(szBuf) );

			SafeSprintf( szBuf, sizeof(szBuf), STR(72), szBuf );
			szDesc[1] = szBuf;
			dwColor[1] = TCT_DEFAULT_DARKGRAY;
		}
		break;
	case ALT_NEW_SOLDIER:
		if( m_FuncState == AFS_RESULT )
		{
			szDesc[2] = STR(73);
			dwColor[2] = TCT_DEFAULT_BLUE;
			szDesc[3] = STR(74);
			dwColor[3] = TCT_DEFAULT_GREEN;
		}
		else if( m_FuncState == AFS_FINAL_RESULT )
		{
			if( m_iResultType == ALCHEMIC_NEW_SOLDIER_SUCC )
			{
				szDesc[0] = g_MyInfo.GetClassName( m_iResultValue );
				dwColor[0] = TCT_DEFAULT_DARKGRAY;

				if( m_iResultPeriod == 0 )
				{
					szDesc[1] = STR(75);
					dwColor[1] = TCT_DEFAULT_BLUE;
				}
				else
				{
					int iTotalLimitDate = m_iResultPeriod / 60;
					int iHour   = iTotalLimitDate / 60;
					int iMinute = iTotalLimitDate % 60;
					if( iMinute == 0 )
						SafeSprintf( szBuf, sizeof(szBuf), STR(76), iHour );
					else
						SafeSprintf( szBuf, sizeof(szBuf), STR(77), iHour, iMinute );

					szDesc[1] = szBuf;
					dwColor[1] = TCT_DEFAULT_BLUE;
				}

				szDesc[2] = STR(78);
				dwColor[2] = TCT_DEFAULT_BLUE;
				szDesc[3] = STR(79);
				dwColor[3] = TCT_DEFAULT_GREEN;
			}
		}
		break;
	case ALT_NEW_ITEM:
		if( m_FuncState == AFS_RESULT )
		{
			szDesc[2] = STR(80);
			dwColor[2] = TCT_DEFAULT_BLUE;
			szDesc[3] = STR(81);
			dwColor[3] = TCT_DEFAULT_GREEN;
		}
		else if( m_FuncState == AFS_FINAL_RESULT )
		{
			if( m_iResultType == ALCHEMIC_NEW_ITEM_SUCC )
			{
				const ioItem* pItem = g_ItemMaker.GetItemConst( m_iResultValue, __FUNCTION__ );
				if( pItem )
					szDesc[0] = pItem->GetName();

				dwColor[0] = TCT_DEFAULT_DARKGRAY;

				if( m_iResultPeriod == 0 )
				{
					szDesc[1] = STR(82);
					dwColor[1] = TCT_DEFAULT_BLUE;
				}
				else
				{
					if( (m_iResultPeriod/24) > 0 )
						SafeSprintf( szBuf, sizeof(szBuf), STR(83), m_iResultPeriod/24 );
					else
						SafeSprintf( szBuf, sizeof(szBuf), STR(84), m_iResultPeriod );

					szDesc[1] = szBuf;
					dwColor[1] = TCT_DEFAULT_BLUE;
				}

				szDesc[2] = STR(85);
				dwColor[2] = TCT_DEFAULT_BLUE;
				szDesc[3] = STR(86);
				dwColor[3] = TCT_DEFAULT_GREEN;
			}
		}
		break;
	}

	if( !szDesc[0].IsEmpty() )
	{
		m_ProcessDesc[0].SetTextStyle( TS_NORMAL );
		m_ProcessDesc[0].SetBkColor( 0, 0, 0 );
		m_ProcessDesc[0].SetTextColor( dwColor[0] );
		m_ProcessDesc[0].AddTextPiece( FONT_SIZE_17, STR(87), szDesc[0].c_str() );
	}

	if( !szDesc[1].IsEmpty() )
	{
		m_ProcessDesc[1].SetTextStyle( TS_NORMAL );
		m_ProcessDesc[1].SetBkColor( 0, 0, 0 );
		m_ProcessDesc[1].SetTextColor( dwColor[1] );
		m_ProcessDesc[1].AddTextPiece( FONT_SIZE_17, STR(88), szDesc[1].c_str() );
	}

	if( !szDesc[2].IsEmpty() )
	{
		m_ProcessDesc[2].SetTextStyle( TS_NORMAL );
		m_ProcessDesc[2].SetBkColor( 0, 0, 0 );
		m_ProcessDesc[2].SetTextColor( dwColor[2] );
		m_ProcessDesc[2].AddTextPiece( FONT_SIZE_17, STR(89), szDesc[2].c_str() );
	}

	if( !szDesc[3].IsEmpty() )
	{
		m_ProcessDesc[3].SetTextStyle( TS_NORMAL );
		m_ProcessDesc[3].SetBkColor( 0, 0, 0 );
		m_ProcessDesc[3].SetTextColor( dwColor[3] );
		m_ProcessDesc[3].AddTextPiece( FONT_SIZE_17, STR(90), szDesc[3].c_str() );
	}
}

void AlchemicFuncWnd::CheckSelectItemBtn()
{
	SelectAlchemicItemBtn *pItemBtn1 = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN1));
	SelectAlchemicItemBtn *pItemBtn2 = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN2));
	SelectAlchemicItemBtn *pItemBtn3 = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN3));
	SelectAlchemicItemBtn *pItemBtn4 = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN4));
	SelectAlchemicItemBtn *pAdditiveBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_ADDITIVE_ITEM_BTN));

	SelectAlchemicItemBtn *pResultBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_RESULT_BTN));

	switch( m_CurFuncInfo.m_AlchemicType )
	{
	case ALT_SOLDIER:
		if( m_FuncState == AFS_FINAL_RESULT )
		{
			if( m_iResultType == ALCHEMIC_SOLDIER_SUCC )
			{
				HideChildWnd(ID_SELECT_ITEM_BTN1);
				HideChildWnd(ID_SELECT_ITEM_BTN2);
				HideChildWnd(ID_SELECT_ITEM_BTN3);
				HideChildWnd(ID_SELECT_ITEM_BTN4);
				HideChildWnd(ID_ADDITIVE_ITEM_BTN);

				SAFEDELETE(m_pResultIcon);
				m_pResultIcon = g_MyInfo.GetSoldierIcon( m_iResultValue, true );
				m_nGradeType = g_UIImageRenderMgr.GetPowerUpGradeType( m_iResultValue );
			}
			else if( m_iResultType == ALCHEMIC_SOLDIER_FAIL )
			{
			}
		}
		break;
	case ALT_ITEM:
		if( m_FuncState == AFS_FINAL_RESULT )
		{
			if( m_iResultType == ALCHEMIC_ITEM_SUCC )
			{
				HideChildWnd(ID_SELECT_ITEM_BTN1);
				HideChildWnd(ID_SELECT_ITEM_BTN2);
				HideChildWnd(ID_SELECT_ITEM_BTN3);
				HideChildWnd(ID_SELECT_ITEM_BTN4);
				HideChildWnd(ID_ADDITIVE_ITEM_BTN);

				ioHashString szIcon;
				const ioItem *pItem = g_ItemMaker.GetItemConst( m_iResultValue, __FUNCTION__ );
				if( pItem )
				{
					szIcon = pItem->GetItemLargeIconName();
					m_nGradeType = pItem->GetGradeType();
				}

				SAFEDELETE(m_pResultIcon);
				m_pResultIcon = g_UIImageSetMgr.CreateImageByFullName( szIcon );
			}
			else if( m_iResultType == ALCHEMIC_ITEM_FAIL )
			{
			}
		}
		break;
	case ALT_CHANGE:
		if( m_FuncState == AFS_FINAL_RESULT )
		{
			HideChildWnd(ID_SELECT_ITEM_BTN1);
			HideChildWnd(ID_SELECT_ITEM_BTN2);
			HideChildWnd(ID_SELECT_ITEM_BTN3);
			HideChildWnd(ID_SELECT_ITEM_BTN4);

			if( pResultBtn )
			{
				pResultBtn->SetWndPos( 97, 169 );
				pResultBtn->InitData( SelectAlchemicItemBtn::BTT_PIECE1, 0 );
				pResultBtn->SetData( m_iChangeCode, m_iChangeCnt );
				pResultBtn->ShowWnd();
			}
		}
		break;
	case ALT_EXCHANGE:
		if( m_FuncState == AFS_FINAL_RESULT )
		{
			if( pItemBtn1 )
				pItemBtn1->InitData();

			if( pAdditiveBtn )
				pAdditiveBtn->InitData();

			if( pItemBtn2 )
				pItemBtn2->SetData( m_iChangeCode, m_iChangeCnt );
		}
		break;
	case ALT_SELL:
		if( m_FuncState == AFS_FINAL_RESULT )
		{
			HideChildWnd(ID_SELECT_ITEM_BTN1);
			HideChildWnd(ID_SELECT_ITEM_BTN2);
			HideChildWnd(ID_SELECT_ITEM_BTN3);
			HideChildWnd(ID_SELECT_ITEM_BTN4);
		}
		break;
	case ALT_NEW_SOLDIER:
		if( m_FuncState == AFS_FINAL_RESULT )
		{
			if( m_iResultType == ALCHEMIC_NEW_SOLDIER_SUCC )
			{
				HideChildWnd(ID_SELECT_ITEM_BTN1);
				HideChildWnd(ID_ADDITIVE_ITEM_BTN);

				SAFEDELETE(m_pResultIcon);
				m_pResultIcon = g_MyInfo.GetSoldierIcon( m_iResultValue, true );
				m_nGradeType = g_UIImageRenderMgr.GetPowerUpGradeType( m_iResultValue );
			}
		}
		break;
	case ALT_NEW_ITEM:
		if( m_FuncState == AFS_FINAL_RESULT )
		{
			if( m_iResultType == ALCHEMIC_NEW_ITEM_SUCC )
			{
				HideChildWnd(ID_SELECT_ITEM_BTN1);
				HideChildWnd(ID_ADDITIVE_ITEM_BTN);

				ioHashString szIcon;
				const ioItem *pItem = g_ItemMaker.GetItemConst( m_iResultValue, __FUNCTION__ );
				if( pItem )
				{
					szIcon = pItem->GetItemLargeIconName();
					m_nGradeType = pItem->GetGradeType();
				}

				SAFEDELETE(m_pResultIcon);
				m_pResultIcon = g_UIImageSetMgr.CreateImageByFullName( szIcon );
			}
		}
		break;
	}
}

void AlchemicFuncWnd::GetAlchemicFuncList( vNewShopPullDownItem &rkList )
{
	rkList.clear();

	int iSize = m_InfoList.size();
	rkList.reserve( iSize );

	for( int i=0; i < iSize; ++i )
	{
		NewShopPullDownItem kItem;
		kItem.m_iMagicCode = m_InfoList[i].m_iCode;

		char szName[MAX_PATH]="";
		StringCbPrintf( szName, sizeof( szName ), "%s", m_InfoList[i].m_Desc.c_str() );

		kItem.m_dwTextColor = TCT_DEFAULT_DARKGRAY;
		kItem.m_szName = szName;

		rkList.push_back( kItem );
	}
}

void AlchemicFuncWnd::iwm_hide()
{
	m_bActionEnable = false;
	m_iCurSelectFunc = 0;

	HideChildWnd( ID_ACTION_BTN );
	HideChildWnd( ID_ACTION_BTN2 );
	HideChildWnd( ID_ACTION_BTN3 );

	HideChildWnd( ID_ALCHEMIC_FUNC_BTN );

	HideChildWnd( ID_ALCHEMIC_FUNC_WND );

	HideChildWnd( ID_SELECT_ITEM_BTN1 );
	HideChildWnd( ID_SELECT_ITEM_BTN2 );
	HideChildWnd( ID_SELECT_ITEM_BTN3 );
	HideChildWnd( ID_SELECT_ITEM_BTN4 );

	HideChildWnd( ID_ADDITIVE_ITEM_BTN );

	HideChildWnd( ID_RESULT_BTN );

	for( int i=0; i < MAX_PRE_SELECT_ITEM; ++ i )
	{
		HideChildWnd( ID_PRE_SELECT_ITEM_BTN1+i );
	}

	if( !m_szRollingSound.IsEmpty() )
		g_SoundMgr.StopSound( m_szRollingSound, 0 );

	if( !m_szSuccessSound.IsEmpty() )
		g_SoundMgr.StopSound( m_szSuccessSound, 0 );

	if( !m_szFailSound.IsEmpty() )
		g_SoundMgr.StopSound( m_szFailSound, 0 );
}

void AlchemicFuncWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_ACTION_BTN:
		if( cmd == IOBN_BTNUP )
		{
			SendAlchemicAction();
		}
		break;
	case ID_ACTION_BTN2:
		if( cmd == IOBN_BTNUP )
		{
			ChangeFunc( m_CurFuncInfo, false );
		}
		break;
	case ID_ACTION_BTN3:
		if( cmd == IOBN_BTNUP )
		{
			HideWnd();

			MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(g_GUIMgr.FindWnd(MY_INVENTORY_WND));
			if( pInvenWnd )
			{
				pInvenWnd->ShowPresentTabDirect();
			}
		}
		break;
	case ID_ALCHEMIC_FUNC_BTN:
		if( cmd == IOBN_BTNUP &&
			(m_FuncState == AFS_PRE_SELECT ||
			 m_FuncState == AFS_SELECT ||
			 m_FuncState == AFS_FINAL_RESULT) )
		{
			NewShopPullDownWnd *pListWnd = dynamic_cast<NewShopPullDownWnd*>(FindChildWnd(ID_ALCHEMIC_FUNC_WND));
			if( pListWnd && pListWnd->IsShow() )
			{
				pListWnd->HideWnd();
			}
			else if( pListWnd && g_AlchemicMgr.HasAlchemicFunc() )
			{
				pListWnd->CreatePullDown( pWnd, pWnd->GetXPos(), pWnd->GetYPos() + 33 );
				// 조건이 달라지면 리스트 따로 구하고...
				vNewShopPullDownItem kItemList;
				GetAlchemicFuncList( kItemList );

				if( !kItemList.empty() )
				{
					int iSize = kItemList.size();
					for(int i = 0;i < iSize; i++)
					{
						pListWnd->InsertItem( kItemList[i] );
					}
					pListWnd->ShowWnd();
				}
				else
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
				}
			}
		}
		break;
	case ID_ALCHEMIC_FUNC_WND:
		if( cmd == IOBN_BTNUP )
		{
			NewShopPullDownWnd *pPullDownWnd = dynamic_cast<NewShopPullDownWnd *>(pWnd);
			if( pPullDownWnd )
			{
				AlchemicFuncListBtnUp( pPullDownWnd->GetOpenBtn(), param );
				pPullDownWnd->HideWnd();
			}
		}
		break;
	case ID_SELECT_ITEM_BTN1:
	case ID_SELECT_ITEM_BTN2:
	case ID_SELECT_ITEM_BTN3:
	case ID_SELECT_ITEM_BTN4:
	case ID_ADDITIVE_ITEM_BTN:
		if( cmd == IOBN_BTNUP && m_FuncState == AFS_SELECT )
		{
			InitAlchemicItem( dwID );
		}
		break;
	default:
		if( cmd == IOBN_BTNDOWN && m_FuncState == AFS_PRE_SELECT )
		{
			if( COMPARE( dwID, ID_PRE_SELECT_ITEM_BTN1, ID_PRE_SELECT_ITEM_BTN15+1) )
			{
				if( m_CurFuncInfo.m_AlchemicType == ALT_NEW_SOLDIER )
					SetNewAlchemicSoldier( dwID );
				else if( m_CurFuncInfo.m_AlchemicType == ALT_NEW_ITEM )
					SetNewAlchemicItem( dwID );
			}
			else if( dwID == ID_PAGE_PRE_BTN )
			{
				if( m_iCurPreItemPage >= 1 )
				{
					m_iCurPreItemPage--;
					UpdatePreSelectPage();
				}
			}
			else if( dwID == ID_PAGE_NEXT_BTN )
			{
				if( m_iMaxPreItemPage > 0 && m_iCurPreItemPage < (m_iMaxPreItemPage-1) )
				{
					m_iCurPreItemPage++;
					UpdatePreSelectPage();
				}
			}
		}
		break;
	}
}

void AlchemicFuncWnd::InitAlchemicItem( DWORD dwID )
{
	SelectAlchemicItemBtn *pAlchemicItemBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(dwID));
	if( pAlchemicItemBtn )
	{
		int iCode = pAlchemicItemBtn->GetCode();

		switch( dwID )
		{
		case ID_SELECT_ITEM_BTN1:
		case ID_SELECT_ITEM_BTN2:
		case ID_SELECT_ITEM_BTN3:
		case ID_SELECT_ITEM_BTN4:
		case ID_ADDITIVE_ITEM_BTN:
			pAlchemicItemBtn->InitData();
			break;
		}

		MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(g_GUIMgr.FindWnd(MY_INVENTORY_WND));
		if( pInvenWnd )
			pInvenWnd->SetAlchemicItemLockStat( iCode, false );
	}

	CheckActionEnable();
}

void AlchemicFuncWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() )
		return;

	DWORD dwCurTime = FRAMEGETTIME();
	if( m_FuncState == AFS_RESULT )
	{
		if( m_dwResultEndTime < dwCurTime )
			SetFinalResultState();
	}

	switch( m_FuncState )
	{
	case AFS_SELECT:
		break;
	case AFS_RESULT:
		ProcessResultEffect();
		break;
	case AFS_FINAL_RESULT:
		ProcessFinalResultEffect();
		break;
	}

	ProcessMagicCircle();

	ioWnd::OnProcess( fTimePerSec );
}

void AlchemicFuncWnd::OnRender()
{
	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	OnRenderMagicCircle();

	if( m_FuncState == AFS_SELECT )
	{
		SuccessRateDescRender();
		OnRenderStrip();
		PrintManual( iXPos+33, iYPos+339, FONT_SIZE_12 );
	}
	else if( m_FuncState == AFS_FINAL_RESULT )
	{
		if( m_pResultIcon && m_pIconBack )
		{
			m_pIconBack->Render( iXPos+129, iYPos+201, UI_RENDER_MULTIPLY );
			g_UIImageRenderMgr.RenderGradeIconBack( m_nGradeType, iXPos + 129, iYPos + 201 );
			m_pResultIcon->Render( iXPos+129, iYPos+201 );
		}

		if( m_CurFuncInfo.m_AlchemicType == ALT_SELL )
		{
			if( m_pPesoIcon && m_pIconBack )
			{
				m_pIconBack->Render( iXPos+129, iYPos+201, UI_RENDER_MULTIPLY );
				m_pPesoIcon->Render( iXPos+129, iYPos+201 );
			}
		}
	}

	OnRenderProcessDesc();

	ioWnd::OnRender();
}

void AlchemicFuncWnd::SuccessRateDescRender()
{
	enum 
	{
		X_OFFSET = 129,
		Y_OFFSET = 312,
	};

	int iXPos = GetDerivedPosX() + X_OFFSET;
	int iYPos = GetDerivedPosY() + Y_OFFSET;

	if( m_SuccessRateDesc.IsEmpty() )
		return;
	
	m_SuccessRateDesc.PrintFullText( iXPos, iYPos, TAT_CENTER );
}

void AlchemicFuncWnd::OnRenderProcessDesc()
{
	enum 
	{
		X_OFFSET	= 129,
		Y_OFFSET	= 311,
		Y_OFFSET2	= 390,
		Y_OFFSET3	= 101,

		LINE_GAP	= 24,
		LINE_GAP2	= 16,
	};

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	if( m_FuncState == AFS_PRE_SELECT )
	{
		if( !m_PreSelectDesc[0].IsEmpty() )
			m_PreSelectDesc[0].PrintFullText( iXPos+X_OFFSET, iYPos+Y_OFFSET3, TAT_CENTER );

		if( !m_PreSelectDesc[1].IsEmpty() )
			m_PreSelectDesc[1].PrintFullText( iXPos+X_OFFSET, iYPos+Y_OFFSET3+LINE_GAP2, TAT_CENTER );

		m_PreSelectPage.PrintFullText( iXPos+X_OFFSET, iYPos+463, TAT_CENTER );
	}
	else
	{
		if( !m_ProcessDesc[0].IsEmpty() )
			m_ProcessDesc[0].PrintFullText( iXPos+X_OFFSET, iYPos+Y_OFFSET, TAT_CENTER );

		if( !m_ProcessDesc[1].IsEmpty() )
			m_ProcessDesc[1].PrintFullText( iXPos+X_OFFSET, iYPos+Y_OFFSET+LINE_GAP, TAT_CENTER );

		if( !m_ProcessDesc[2].IsEmpty() )
			m_ProcessDesc[2].PrintFullText( iXPos+X_OFFSET, iYPos+Y_OFFSET2, TAT_CENTER );

		if( !m_ProcessDesc[3].IsEmpty() )
			m_ProcessDesc[3].PrintFullText( iXPos+X_OFFSET, iYPos+Y_OFFSET2+LINE_GAP, TAT_CENTER );
	}
}

void AlchemicFuncWnd::OnRenderStrip()
{
	if( !m_pStripDark || !m_pStripLight )
		return;

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	enum 
	{
		X_OFFSET	= 27,
		Y_OFFSET	= 336,

		LINE_GAP	= 21,
	};

	iXPos += X_OFFSET;
	iYPos += Y_OFFSET;

	m_pStripDark->SetSize( 205, 21 );
	m_pStripDark->SetHorzFormatting( ioUIRenderImage::HF_STRETCH );
	m_pStripDark->SetVertFormatting( ioUIRenderImage::VF_STRETCH );

	m_pStripLight->SetSize( 205, 21 );
	m_pStripLight->SetHorzFormatting( ioUIRenderImage::HF_STRETCH );
	m_pStripLight->SetVertFormatting( ioUIRenderImage::VF_STRETCH );

	m_pStripDark->Render( iXPos, iYPos, UI_RENDER_MULTIPLY );
	m_pStripLight->Render( iXPos, iYPos+LINE_GAP, UI_RENDER_MULTIPLY );
	m_pStripDark->Render( iXPos, iYPos+(LINE_GAP*2), UI_RENDER_MULTIPLY );
	m_pStripLight->Render( iXPos, iYPos+(LINE_GAP*3), UI_RENDER_MULTIPLY );
	m_pStripDark->Render( iXPos, iYPos+(LINE_GAP*4), UI_RENDER_MULTIPLY );
}

void AlchemicFuncWnd::PrintManual( int a_iXPos, int a_iYPos , float fScale )
{
	if( !m_pManual )
		return;

	int iManaual = m_CurFuncInfo.m_iManaual - 1;
	ioComplexStringPrinter kPrinter;
	int iMaxLine = m_pManual->GetMaxLine( iManaual );
	for(int iLine = 1; iLine < iMaxLine; iLine++)
	{
		m_pManual->SetPrinter( iManaual, iLine, fScale, kPrinter);
		a_iYPos += m_pManual->GetSpaceLine( iManaual, iLine);
		kPrinter.PrintFullTextWidthCut( a_iXPos, a_iYPos, TAT_LEFT, DESC_WIDTH );
		kPrinter.ClearList();
	}
}

void AlchemicFuncWnd::SendAlchemicAction()
{
	if( !m_bActionEnable )
		return;

	switch( m_CurFuncInfo.m_AlchemicType )
	{
	case ALT_NONE:
	//case ALT_SOLDIER:
	//case ALT_ITEM:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return;
	}

	SP2Packet kPacket( CTPK_ALCHEMIC_FUNC );

	if( !FillSendInfo( kPacket ) )
		return;

	TCPNetwork::SendToServer( kPacket );
	TCPNetwork::MouseBusy( true );
}

bool AlchemicFuncWnd::FillSendInfo( SP2Packet &rkPacket )
{
	DWORD dwID;
	SelectAlchemicItemBtn *pItemBtn = NULL;
	SelectAlchemicItemBtn *pAdditiveBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_ADDITIVE_ITEM_BTN));

	int iTotalCnt = 0;
	int iMinTotalCnt = g_AlchemicMgr.GetMinTotalCnt();
	int iNewMinCnt = g_AlchemicMgr.GetNewAlchemicMinTotalCnt();

	switch( m_CurFuncInfo.m_AlchemicType )
	{
		case ALT_SOLDIER:
			{
				rkPacket << ALCHEMIC_FUNC_SOLDIER;
				rkPacket << m_CurFuncInfo.m_iCode;

				for( int i=0; i < 4; ++i )
				{
					dwID = ID_SELECT_ITEM_BTN1+i;
					pItemBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(dwID));

					if( pItemBtn )
					{
						rkPacket << pItemBtn->GetCode();
						rkPacket << pItemBtn->GetCount();
					}
					else
					{
						rkPacket << 0;
						rkPacket << 0;
					}
				}

				if( pAdditiveBtn )
				{
					rkPacket << pAdditiveBtn->GetCode();
					rkPacket << pAdditiveBtn->GetCount();
				}
				else
				{
					rkPacket << 0;
					rkPacket << 0;
				}

				return true;
			}
			break;
		case ALT_ITEM:
			{
				rkPacket << ALCHEMIC_FUNC_ITEM;
				rkPacket << m_CurFuncInfo.m_iCode;

				for( int i=0; i < 4; ++i )
				{
					dwID = ID_SELECT_ITEM_BTN1+i;
					pItemBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(dwID));

					if( pItemBtn )
					{
						rkPacket << pItemBtn->GetCode();
						rkPacket << pItemBtn->GetCount();
					}
					else
					{
						rkPacket << 0;
						rkPacket << 0;
					}
				}

				if( pAdditiveBtn )
				{
					rkPacket << pAdditiveBtn->GetCode();
					rkPacket << pAdditiveBtn->GetCount();
				}
				else
				{
					rkPacket << 0;
					rkPacket << 0;
				}

				return true;
			}
			break;
		case ALT_CHANGE:
			{
				rkPacket << ALCHEMIC_FUNC_CHANGE;
				rkPacket << m_CurFuncInfo.m_iCode;

				for( int i=0; i < 4; ++i )
				{
					dwID = ID_SELECT_ITEM_BTN1+i;
					pItemBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(dwID));

					if( pItemBtn )
					{
						rkPacket << pItemBtn->GetCode();
						rkPacket << pItemBtn->GetCount();

						iTotalCnt += pItemBtn->GetCount();
					}
					else
					{
						rkPacket << 0;
						rkPacket << 0;
					}
				}

				if( iTotalCnt > 0 && iTotalCnt < iMinTotalCnt )
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3), iMinTotalCnt );
					return false;
				}

				return true;
			}
			break;
		case ALT_EXCHANGE:
			{
				rkPacket << ALCHEMIC_FUNC_EXCHANGE;
				rkPacket << m_CurFuncInfo.m_iCode;

				for( int i=0; i < 2; ++i )
				{
					dwID = ID_SELECT_ITEM_BTN1+i;
					pItemBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(dwID));

					if( pItemBtn )
					{
						rkPacket << pItemBtn->GetCode();
						rkPacket << pItemBtn->GetCount();

						if( dwID == ID_SELECT_ITEM_BTN1 )
							iTotalCnt += pItemBtn->GetCount();
					}
					else
					{
						rkPacket << 0;
						rkPacket << 0;
					}
				}

				if( pAdditiveBtn )
				{
					rkPacket << pAdditiveBtn->GetCode();
					rkPacket << pAdditiveBtn->GetCount();
				}
				else
				{
					rkPacket << 0;
					rkPacket << 0;
				}

				if( iTotalCnt > 0 && iTotalCnt < iMinTotalCnt )
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4), iMinTotalCnt );
					return false;
				}

				return true;
			}
			break;
		case ALT_SELL:
			{
				rkPacket << ALCHEMIC_FUNC_SELL;
				rkPacket << m_CurFuncInfo.m_iCode;

				for( int i=0; i < 4; ++i )
				{
					dwID = ID_SELECT_ITEM_BTN1+i;
					pItemBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(dwID));

					if( pItemBtn )
					{
						rkPacket << pItemBtn->GetCode();
						rkPacket << pItemBtn->GetCount();
					}
					else
					{
						rkPacket << 0;
						rkPacket << 0;
					}
				}

				return true;
			}
			break;
		case ALT_NEW_SOLDIER:
			{
				rkPacket << ALCHEMIC_FUNC_NEW_SOLDIER;
				rkPacket << m_CurFuncInfo.m_iCode;

				rkPacket << m_iCurNewAlchemicSelectValue;

				pItemBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN1));
				if( pItemBtn )
				{
					rkPacket << pItemBtn->GetCode();
					rkPacket << pItemBtn->GetCount();

					iTotalCnt = pItemBtn->GetCount();
				}
				else
				{
					rkPacket << 0;
					rkPacket << 0;
				}

				if( pAdditiveBtn )
				{
					rkPacket << pAdditiveBtn->GetCode();
					rkPacket << pAdditiveBtn->GetCount();
				}
				else
				{
					rkPacket << 0;
					rkPacket << 0;
				}

				if( iTotalCnt > 0 && iTotalCnt < iNewMinCnt )
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5), iNewMinCnt );
					return false;
				}

				return true;
			}
			break;
		case ALT_NEW_ITEM:
			{
				rkPacket << ALCHEMIC_FUNC_NEW_ITEM;
				rkPacket << m_CurFuncInfo.m_iCode;

				rkPacket << m_iCurNewAlchemicSelectValue;

				pItemBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN1));
				if( pItemBtn )
				{
					rkPacket << pItemBtn->GetCode();
					rkPacket << pItemBtn->GetCount();

					iTotalCnt = pItemBtn->GetCount();
				}
				else
				{
					rkPacket << 0;
					rkPacket << 0;
				}

				if( pAdditiveBtn )
				{
					rkPacket << pAdditiveBtn->GetCode();
					rkPacket << pAdditiveBtn->GetCount();
				}
				else
				{
					rkPacket << 0;
					rkPacket << 0;
				}

				if( iTotalCnt > 0 && iTotalCnt < iNewMinCnt )
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6), iNewMinCnt );
					return false;
				}

				return true;
			}
			break;
	}

	return false;
}

void AlchemicFuncWnd::CheckActionEnable()
{
	m_bActionEnable = false;

	DWORD dwID;
	SelectAlchemicItemBtn *pItemBtn = NULL;
	SelectAlchemicItemBtn *pAdditiveBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_ADDITIVE_ITEM_BTN));

	switch( m_CurFuncInfo.m_AlchemicType )
	{
	case ALT_SOLDIER:
	case ALT_ITEM:
	case ALT_CHANGE:
	case ALT_SELL:
		{
			for( int i=0; i < 4; ++i )
			{
				dwID = ID_SELECT_ITEM_BTN1+i;
				pItemBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(dwID));

				if( pItemBtn && pItemBtn->GetCode() > 0 && pItemBtn->GetCount() > 0 )
				{
					m_bActionEnable = true;
					break;
				}
			}
		}
		break;
	case ALT_EXCHANGE:
		{
			pItemBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN1));
			if( !pItemBtn || pItemBtn->GetCode() <= 0 || pItemBtn->GetCount() <= 0 )
				break;

			pItemBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN2));
			if( !pItemBtn || pItemBtn->GetCode() <= 0 )
				break;

			if( !pAdditiveBtn || pAdditiveBtn->GetCode() <= 0 || pAdditiveBtn->GetCount() <= 0 )
				break;

			m_bActionEnable = true;
		}
		break;
	case ALT_NEW_SOLDIER:
	case ALT_NEW_ITEM:
		{
			pItemBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN1));
			if( !pItemBtn || pItemBtn->GetCode() <= 0 || pItemBtn->GetCount() <= 0 )
				break;

			if( !pAdditiveBtn || pAdditiveBtn->GetCode() <= 0 || pAdditiveBtn->GetCount() <= 0 )
				break;

			m_bActionEnable = true;
		}
		break;
	}

	CheckSuccessRateText();

	ioButton *pAction = dynamic_cast<ioButton*>(FindChildWnd(ID_ACTION_BTN));
	if( !pAction || !pAction->IsShow() )
		return;
	
	if( m_bActionEnable )
		pAction->SetActive();
	else
		pAction->SetInActive();
}

void AlchemicFuncWnd::RefreshAlchemicFunc()
{
	ChangeFunc( m_CurFuncInfo, false );
}

void AlchemicFuncWnd::AlchemicFuncListBtnUp( ioWnd *pOpenBtn, int iListArray )
{
	if( !pOpenBtn ) 
		return;	

	vNewShopPullDownItem kItemList;
	GetAlchemicFuncList( kItemList );	

	if( !COMPARE( iListArray, 0, (int)kItemList.size() ) ) 
		return;			

	ioUITitle *pTitle =  pOpenBtn->GetTitle();
	if( !pTitle )
		return;

	ioHashString szTitle;
	pTitle->GetFirstText( szTitle );

	NewShopPullDownItem &rkItem = kItemList[iListArray];
	if( szTitle == rkItem.m_szName ) 
		return;

	int iCode = rkItem.m_iMagicCode;
	if( iCode <= 0 )
		return;

	SelectAlchemicFuncBtn *pFuncBtn = dynamic_cast<SelectAlchemicFuncBtn*>(FindChildWnd(ID_ALCHEMIC_FUNC_BTN));
	if( pFuncBtn )
		pFuncBtn->SetSelectAlchemicFunc( iCode );

	AlchemicFuncInfo kFuncInfo;
	g_AlchemicMgr.FindAlchemicFunc( iCode, kFuncInfo );
	ChangeFunc( kFuncInfo, true );
}

void AlchemicFuncWnd::OnAlchemicFuncSubResult( int iResult )
{
	switch( iResult )
	{
	case ART_NOT_FIND_FUNC:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(8) );
		break;
	case ART_NOT_MACH_FUNC:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(9) );
		break;
	case ART_NOT_MACH_VALUE:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(10) );
		break;
	case ART_NOT_EMPTY_SLOT:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(11) );
		break;
	case ART_OVER_MAX_CNT:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(12) );
		break;
	case ART_NOT_ENOUGH_CNT:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(13) );
		break;
	case ART_EXCEPTION:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(14) );
		break;
	case ART_NOT_MACH_CODE:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(15) );
		break;
	case ART_PERIOD_ERROR:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(16) );
		break;
	case ART_TABLE_ERROR:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(17) );
		break;
	}
}

void AlchemicFuncWnd::OnAlchemicFuncResult( int iResult, SP2Packet &rkPacket )
{
	switch( iResult )
	{
	case ALCHEMIC_EXCEPTION_FAIL:
		{
			int iSubType;
			rkPacket >> iSubType;

			RefreshAlchemicFunc();

			OnAlchemicFuncSubResult( iSubType );
		}
		break;
	case ALCHEMIC_SOLDIER_SUCC:
		{
			ClearResultData();

			rkPacket >> m_iResultCode1 >> m_iResultValue1;
			rkPacket >> m_iResultCode2 >> m_iResultValue2;
			rkPacket >> m_iResultCode3 >> m_iResultValue3;
			rkPacket >> m_iResultCode4 >> m_iResultValue4;
			rkPacket >> m_iResultAddCode >> m_iResultAddValue;
			rkPacket >> m_iResultValue;

			SetResultState( iResult );

#if defined( USE_GA )
			// ITEM_COMBINE_HERO_NORMAL_SUCC
			g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
				, "Item"
				, "NormalHeroSuccess"
				, ""
				, 1
				, "%2FITEM%2FCOMBINE%2FHERO%2FNORMAL%2FSUCC" );
#endif
		}
		break;
	case ALCHEMIC_SOLDIER_FAIL:
		{
			ClearResultData();

			rkPacket >> m_iResultCode1 >> m_iResultValue1;
			rkPacket >> m_iResultCode2 >> m_iResultValue2;
			rkPacket >> m_iResultCode3 >> m_iResultValue3;
			rkPacket >> m_iResultCode4 >> m_iResultValue4;
			rkPacket >> m_iResultAddCode >> m_iResultAddValue;

			SetResultState( iResult );

#if defined( USE_GA )
			// ITEM_COMBINE_HERO_NORMAL_FAIL
			g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
				, "Item"
				, "NormalHeroFail"
				, ""
				, 1
				, "%2FITEM%2FCOMBINE%2FHERO%2FNORMAL%2FFAIL" );
#endif
		}
		break;
	case ALCHEMIC_ITEM_SUCC:
		{
			ClearResultData();

			rkPacket >> m_iResultCode1 >> m_iResultValue1;
			rkPacket >> m_iResultCode2 >> m_iResultValue2;
			rkPacket >> m_iResultCode3 >> m_iResultValue3;
			rkPacket >> m_iResultCode4 >> m_iResultValue4;
			rkPacket >> m_iResultAddCode >> m_iResultAddValue;
			rkPacket >> m_iResultValue;

			SetResultState( iResult );

#if defined( USE_GA )
			// ITEM_COMBINE_ITEM_NORMAL_SUCC
			g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
				, "Item"
				, "NormalItemSuccess"
				, ""
				, 1
				, "%2FITEM%2FCOMBINE%2FITEM%2FNORMAL%2FSUCC" );
#endif
		}
		break;
	case ALCHEMIC_ITEM_FAIL:
		{
			ClearResultData();

			rkPacket >> m_iResultCode1 >> m_iResultValue1;
			rkPacket >> m_iResultCode2 >> m_iResultValue2;
			rkPacket >> m_iResultCode3 >> m_iResultValue3;
			rkPacket >> m_iResultCode4 >> m_iResultValue4;
			rkPacket >> m_iResultAddCode >> m_iResultAddValue;

			SetResultState( iResult );

#if defined( USE_GA )
			// ITEM_COMBINE_ITEM_NORMAL_FAIL
			g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
				, "Item"
				, "NormalItemFail"
				, ""
				, 1
				, "%2FITEM%2FCOMBINE%2FITEM%2FNORMAL%2FFAIL" );
#endif
		}
		break;
	case ALCHEMIC_CHANGE_SUCC:
		{
			ClearResultData();

			rkPacket >> m_iResultCode1 >> m_iResultValue1;
			rkPacket >> m_iResultCode2 >> m_iResultValue2;
			rkPacket >> m_iResultCode3 >> m_iResultValue3;
			rkPacket >> m_iResultCode4 >> m_iResultValue4;
			rkPacket >> m_iChangeCode >> m_iChangeCnt;

			SetResultState( iResult );

#if defined( USE_GA )
			// ITEM_COMBINE_PIECE_RANDOM
			g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
				, "Item"
				, "PieceRandom"
				, ""
				, 1
				, "%2FITEM%2FCOMBINE%2FPIECE%2FRANDOM" );
#endif
		}
		break;
	case ALCHEMIC_EXCHANGE_SUCC:
		{
			ClearResultData();

			rkPacket >> m_iResultCode1 >> m_iResultValue1;
			rkPacket >> m_iResultAddCode >> m_iResultAddValue;
			rkPacket >> m_iChangeCode >> m_iChangeCnt;

			SetResultState( iResult );

#if defined( USE_GA )
			// ITEM_COMBINE_PIECE_APPOINT
			g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
				, "Item"
				, "PieceAppoint"
				, ""
				, 1
				, "%2FITEM%2FCOMBINE%2FPIECE%2FAPPOINT" );
#endif
		}
		break;
	case ALCHEMIC_PESO_SUCC:
		{
			ClearResultData();

			rkPacket >> m_iResultCode1 >> m_iResultValue1;
			rkPacket >> m_iResultCode2 >> m_iResultValue2;
			rkPacket >> m_iResultCode3 >> m_iResultValue3;
			rkPacket >> m_iResultCode4 >> m_iResultValue4;
			rkPacket >> m_iChangePeso >> m_iCurPeso;

			SetResultState( iResult );

#if defined( USE_GA )
			char chGetPeso[32]		= {0,};
			char chSlash[16]		= {0,};
			char chPostData[256]	= {0,};

			if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
			{
				sprintf_e( chGetPeso, "%d", m_iChangePeso );
				sprintf_e( chSlash, "%%2F" );
				sprintf_e( chPostData, "%sPESO%sGET%sCOMBINE%s%s", chSlash, chSlash, chSlash, chSlash, chGetPeso );
			}
			else
			{
				SafeSprintf( chGetPeso, sizeof(chGetPeso), "%1", m_iChangePeso );
				SafeSprintf( chSlash, sizeof(chSlash), "%2F" );
				SafeSprintf( chPostData, sizeof(chPostData), "%1PESO%2GET%3COMBINE%4%5", chSlash, chSlash, chSlash, chSlash, chGetPeso );
			}			

			// PESO_GET_COMBINE
			g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
				, "Peso"
				, "Combine"
				, ""
				, 1
				, chPostData );
#endif
		}
		break;
	case ALCHEMIC_NEW_SOLDIER_SUCC:
		{
			ClearResultData();

			rkPacket >> m_iResultCode1 >> m_iResultValue1;
			rkPacket >> m_iResultAddCode >> m_iResultAddValue;

			rkPacket >> m_iResultValue >> m_iResultPeriod;

			SetResultState( iResult );

#if defined( USE_GA )
			// ITEM_COMBINE_HERO_RARE
			g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
				, "Item"
				, "RareHero"
				, ""
				, 1
				, "%2FITEM%2FCOMBINE%2FHERO%2FRARE" );
#endif
		}
		break;
	case ALCHEMIC_NEW_ITEM_SUCC:
		{
			ClearResultData();

			rkPacket >> m_iResultCode1 >> m_iResultValue1;
			rkPacket >> m_iResultAddCode >> m_iResultAddValue;

			rkPacket >> m_iResultValue >> m_iResultPeriod;

			SetResultState( iResult );

#if defined( USE_GA )
			// ITEM_COMBINE_ITEM_RARE
			g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
				, "Item"
				, "RareItem"
				, ""
				, 1
				, "%2FITEM%2FCOMBINE%2FITEM%2FRARE" );
#endif
		}
		break;
	}
}

void AlchemicFuncWnd::ReBatchButton( int iType )
{
	enum
	{
		ONE_BTN_SIZE	= 204,
		TWO_BTN_SIZE	= 102,

		ONE_BTN_X		= 27,
		TWO_BTN_X		= 131,

		ONE_TEXT_X		= 102,
		TWO_TEXT_X		= 51,
	};

	ioButton *pButton = NULL;

	switch( iType )
	{
	case RBB_SELECT_NONE:
		HideChildWnd(ID_ACTION_BTN );
		HideChildWnd(ID_ACTION_BTN2 );
		HideChildWnd(ID_ACTION_BTN3 );
		break;
	case RBB_SELECT_ONE:
		ShowChildWnd(ID_ACTION_BTN);
		SetChildInActive(ID_ACTION_BTN);

		HideChildWnd(ID_ACTION_BTN2);
		HideChildWnd(ID_ACTION_BTN3);
		break;
	case RBB_RESULT_ONE:
		{
			HideChildWnd(ID_ACTION_BTN);
			HideChildWnd(ID_ACTION_BTN3);

			pButton = dynamic_cast<ioButton*>(FindChildWnd(ID_ACTION_BTN2));
			if( pButton )
			{
				pButton->SetSize( ONE_BTN_SIZE, pButton->GetHeight() );
				pButton->SetWndPos( ONE_BTN_X, pButton->GetYPos() );

				ioUITitle *pTitle =  pButton->GetTitle();
				if( pTitle )
				{
					pTitle->SetOffset( ONE_TEXT_X, pTitle->GetTextYOffSet() );
				}

				pButton->ShowWnd();
			}
		}
		break;
	case RBB_TWO:
		{
			HideChildWnd(ID_ACTION_BTN);

			pButton = dynamic_cast<ioButton*>(FindChildWnd(ID_ACTION_BTN2));
			if( pButton )
			{
				pButton->SetSize( TWO_BTN_SIZE, pButton->GetHeight() );
				pButton->SetWndPos( ONE_BTN_X, pButton->GetYPos() );

				ioUITitle *pTitle =  pButton->GetTitle();
				if( pTitle )
				{
					pTitle->SetOffset( TWO_TEXT_X, pTitle->GetTextYOffSet() );
				}

				pButton->ShowWnd();
			}

			pButton = dynamic_cast<ioButton*>(FindChildWnd(ID_ACTION_BTN3));
			if( pButton )
			{
				pButton->SetSize( TWO_BTN_SIZE, pButton->GetHeight() );
				pButton->SetWndPos( TWO_BTN_X, pButton->GetYPos() );

				ioUITitle *pTitle =  pButton->GetTitle();
				if( pTitle )
				{
					pTitle->SetOffset( TWO_TEXT_X, pTitle->GetTextYOffSet() );
				}

				pButton->ShowWnd();
			}

		}
		break;
	}
}

IWDropEffect AlchemicFuncWnd::iwm_dragenter( ioDragItem *pItem, const ioMouse& mouse )
{
	DWORD dwBtnID = CheckDropEnable( mouse, pItem );
	if( dwBtnID == 0 )
		return IW_DROP_NONE;

	return IW_DROP_ENABLE;
}

IWDropEffect AlchemicFuncWnd::iwm_dragover( ioDragItem *pItem, const ioMouse& mouse )
{
	return iwm_dragenter( pItem, mouse );
}

bool AlchemicFuncWnd::iwm_dropitem( ioDragItem *pItem, const ioMouse& mouse, ioWnd *pStartWnd )
{
	DWORD dwBtnID = CheckDropEnable( mouse, pItem );
	if( dwBtnID == 0 )
		return false;

	ioAlchemicDragItem *pDragItem = ToAlchemicDragItem( pItem );
	if( !pDragItem )	return false;

	SelectAlchemicItemBtn *pBtn = dynamic_cast<SelectAlchemicItemBtn*>( FindChildWnd( dwBtnID ) );
	if( !pBtn || !pBtn->IsShow() )
		return false;

	int iCode = pDragItem->GetCode();
	pBtn->SetDragData( m_CurFuncInfo, iCode );

	return true;
}

void AlchemicFuncWnd::SelectClickAction( int iCode )
{
	if( m_FuncState == AFS_FINAL_RESULT )
		return;

	SelectAlchemicItemBtn *pItemBtn = NULL;
	SelectAlchemicItemBtn *pAdditiveBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_ADDITIVE_ITEM_BTN));
	
	DWORD dwID = 0;
	DWORD dwCurSelectBtn = 0;

	switch( m_CurFuncInfo.m_AlchemicType )
	{
	case ALT_SOLDIER:
	case ALT_ITEM:
		{
			// piece
			for( int i=0; i < 4; ++i )
			{
				dwID = ID_SELECT_ITEM_BTN1+i;
				pItemBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(dwID));

				if( pItemBtn && pItemBtn->GetCode() <= 0 && iCode < ALCHEMIC_ADDITIVE_DIV )
				{
					dwCurSelectBtn = dwID;
					break;
				}
			}

			// additive
			if( pAdditiveBtn && pAdditiveBtn->GetCode() <= 0 && iCode > ALCHEMIC_ADDITIVE_DIV )
			{
				dwCurSelectBtn = ID_ADDITIVE_ITEM_BTN;
				break;
			}
		}
		break;
	case ALT_CHANGE:
		{
			// piece
			for( int i=0; i < 4; ++i )
			{
				dwID = ID_SELECT_ITEM_BTN1+i;
				pItemBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(dwID));

				if( pItemBtn && pItemBtn->GetCode() <= 0 && iCode < ALCHEMIC_ADDITIVE_DIV )
				{
					dwCurSelectBtn = dwID;
					break;
				}
			}
		}
		break;
	case ALT_SELL:
		{
			// piece
			for( int i=0; i < 4; ++i )
			{
				dwID = ID_SELECT_ITEM_BTN1+i;
				pItemBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(dwID));

				if( pItemBtn && pItemBtn->GetCode() <= 0 )
				{
					dwCurSelectBtn = dwID;
					break;
				}
			}
		}
		break;
	case ALT_EXCHANGE:
		{
			pItemBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN1));
			if( pItemBtn && pItemBtn->GetCode() <= 0 && iCode < ALCHEMIC_ADDITIVE_DIV )
			{
				dwCurSelectBtn = ID_SELECT_ITEM_BTN1;
				break;
			}

			if( pAdditiveBtn && pAdditiveBtn->GetCode() <= 0 && iCode > ALCHEMIC_ADDITIVE_DIV )
			{
				dwCurSelectBtn = ID_ADDITIVE_ITEM_BTN;
				break;
			}

			pItemBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN2));
			if( pItemBtn && pItemBtn->GetCode() <= 0 && iCode < ALCHEMIC_ADDITIVE_DIV )
			{
				dwCurSelectBtn = ID_SELECT_ITEM_BTN2;
				break;
			}
		}
		break;
	case ALT_NEW_SOLDIER:
	case ALT_NEW_ITEM:
		{
			pItemBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN1));
			if( pItemBtn && pItemBtn->GetCode() <= 0 && iCode < ALCHEMIC_ADDITIVE_DIV )
			{
				dwCurSelectBtn = ID_SELECT_ITEM_BTN1;
				break;
			}

			if( pAdditiveBtn && pAdditiveBtn->GetCode() <= 0 && iCode > ALCHEMIC_ADDITIVE_DIV )
			{
				dwCurSelectBtn = ID_ADDITIVE_ITEM_BTN;
				break;
			}
		}
		break;
	}

	pItemBtn = dynamic_cast<SelectAlchemicItemBtn*>( FindChildWnd(dwCurSelectBtn) );
	if( !pItemBtn || !pItemBtn->IsShow() )
		return;

	if( dwCurSelectBtn > 0 )
	{
		pItemBtn->SetDragData( m_CurFuncInfo, iCode );
	}
}

DWORD AlchemicFuncWnd::CheckDropEnable( const ioMouse& mouse, ioDragItem *pItem )
{
	ioAlchemicDragItem *pDragItem = ToAlchemicDragItem( pItem );
	if( !pDragItem )
		return 0;

	DWORD dwBtnID = 0;

	bool bAdditive = false;
	int iCode = pDragItem->GetCode();
	if( m_CurFuncInfo.m_AlchemicType != ALT_SELL && iCode > ALCHEMIC_ADDITIVE_DIV )
		bAdditive = true;

	if( bAdditive )
	{
		SelectAlchemicItemBtn *pAdditiveBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_ADDITIVE_ITEM_BTN));
		if( pAdditiveBtn && pAdditiveBtn->IsShow() )
		{
			if( pAdditiveBtn->IsInWndRect( mouse.GetMousePos() ) )
				return ID_ADDITIVE_ITEM_BTN;
			else
				pAdditiveBtn->iwm_mouseleave( mouse );
		}

		return 0;
	}

	for( int i = ID_SELECT_ITEM_BTN1; i < ID_SELECT_ITEM_BTN4+1; ++i )
	{
		dwBtnID = i;
		SelectAlchemicItemBtn *pBtn = dynamic_cast<SelectAlchemicItemBtn*>( FindChildWnd( dwBtnID ) );
		if( pBtn && pBtn->IsShow() )
		{
			if( pBtn->IsInWndRect( mouse.GetMousePos() ) )
				return dwBtnID;
			else
				pBtn->iwm_mouseleave( mouse );
		}
	}

	return 0;
}

void AlchemicFuncWnd::ClearProcessDesc()
{
	for( int i=0; i < 4; ++i )
	{
		m_ProcessDesc[i].ClearList();
	}
}

void AlchemicFuncWnd::ClearResultData()
{
	m_iResultCode1 = 0;
	m_iResultCode2 = 0;
	m_iResultCode3 = 0;
	m_iResultCode4 = 0;
	m_iResultAddCode = 0;

	m_iResultValue1 = 0;
	m_iResultValue2 = 0;
	m_iResultValue3 = 0;
	m_iResultValue4 = 0;
	m_iResultAddValue = 0;

	m_iResultValue = 0;
	m_iResultPeriod = 0;

	m_iChangeCode = 0;
	m_iChangeCnt = 0;

	m_iChangePeso = 0;
	m_iCurPeso = 0;

	m_dwResultStartTime = 0;
	m_dwResultEndTime = 0;
	m_dwResultEffectEndTime = 0;

	m_dwFinalResultStartTime = 0;

	m_iAlpha1 = 0;
	m_iAlpha2 = 0;
	m_fCurEffectScale = 1.3f;

	m_dwRollingEffectCheckTime = 0;

	m_iCurEffectArray = -1;
	m_iCurSuccessEffectArray = -1;

	SAFEDELETE(m_pEffect);
	SAFEDELETE(m_pEffect2);
	SAFEDELETE(m_pResultIcon);
}

void AlchemicFuncWnd::SetResultState( int iResultType )
{
	ioAlchemicInventory *pInven = g_MyInfo.GetAlchemicInventory();
	if( !pInven )
		return;

	m_iResultType = iResultType;
	m_FuncState = AFS_RESULT;
	
	m_dwResultStartTime = FRAMEGETTIME();
	m_dwResultEndTime = m_dwResultStartTime + m_dwResultEffectTime;
	m_dwResultEffectEndTime = m_dwResultStartTime + m_dwResultEffectTime;

	m_iAlpha1 = 0;
	m_iAlpha2 = 0;
	m_fCurEffectScale = 1.3f;

	m_SuccessRateDesc.ClearList();

	switch( m_iResultType )
	{
	case ALCHEMIC_SOLDIER_SUCC:
		pInven->UseAlchemicItem( m_iResultCode1, m_iResultValue1 );
		pInven->UseAlchemicItem( m_iResultCode2, m_iResultValue2 );
		pInven->UseAlchemicItem( m_iResultCode3, m_iResultValue3 );
		pInven->UseAlchemicItem( m_iResultCode4, m_iResultValue4 );
		pInven->UseAlchemicItem( m_iResultAddCode, m_iResultAddValue );
		break;
	case ALCHEMIC_SOLDIER_FAIL:
		pInven->UseAlchemicItem( m_iResultCode1, m_iResultValue1 );
		pInven->UseAlchemicItem( m_iResultCode2, m_iResultValue2 );
		pInven->UseAlchemicItem( m_iResultCode3, m_iResultValue3 );
		pInven->UseAlchemicItem( m_iResultCode4, m_iResultValue4 );
		pInven->UseAlchemicItem( m_iResultAddCode, m_iResultAddValue );
		break;
	case ALCHEMIC_ITEM_SUCC:
		pInven->UseAlchemicItem( m_iResultCode1, m_iResultValue1 );
		pInven->UseAlchemicItem( m_iResultCode2, m_iResultValue2 );
		pInven->UseAlchemicItem( m_iResultCode3, m_iResultValue3 );
		pInven->UseAlchemicItem( m_iResultCode4, m_iResultValue4 );
		pInven->UseAlchemicItem( m_iResultAddCode, m_iResultAddValue );
		break;
	case ALCHEMIC_ITEM_FAIL:
		pInven->UseAlchemicItem( m_iResultCode1, m_iResultValue1 );
		pInven->UseAlchemicItem( m_iResultCode2, m_iResultValue2 );
		pInven->UseAlchemicItem( m_iResultCode3, m_iResultValue3 );
		pInven->UseAlchemicItem( m_iResultCode4, m_iResultValue4 );
		pInven->UseAlchemicItem( m_iResultAddCode, m_iResultAddValue );
		break;
	case ALCHEMIC_CHANGE_SUCC:
		pInven->UseAlchemicItem( m_iResultCode1, m_iResultValue1 );
		pInven->UseAlchemicItem( m_iResultCode2, m_iResultValue2 );
		pInven->UseAlchemicItem( m_iResultCode3, m_iResultValue3 );
		pInven->UseAlchemicItem( m_iResultCode4, m_iResultValue4 );

		pInven->GainAlchemicItem( m_iChangeCode, m_iChangeCnt );
		break;
	case ALCHEMIC_EXCHANGE_SUCC:
		pInven->UseAlchemicItem( m_iResultCode1, m_iResultValue1 );
		pInven->UseAlchemicItem( m_iResultAddCode, m_iResultAddValue );

		pInven->GainAlchemicItem( m_iChangeCode, m_iChangeCnt );
		break;
	case ALCHEMIC_PESO_SUCC:
		g_MyInfo.SetMoney( m_iCurPeso );

		pInven->UseAlchemicItem( m_iResultCode1, m_iResultValue1 );
		pInven->UseAlchemicItem( m_iResultCode2, m_iResultValue2 );
		pInven->UseAlchemicItem( m_iResultCode3, m_iResultValue3 );
		pInven->UseAlchemicItem( m_iResultCode4, m_iResultValue4 );
		break;
	case ALCHEMIC_NEW_SOLDIER_SUCC:
		pInven->UseAlchemicItem( m_iResultCode1, m_iResultValue1 );
		pInven->UseAlchemicItem( m_iResultAddCode, m_iResultAddValue );
		break;
	case ALCHEMIC_NEW_ITEM_SUCC:
		pInven->UseAlchemicItem( m_iResultCode1, m_iResultValue1 );
		pInven->UseAlchemicItem( m_iResultAddCode, m_iResultAddValue );
		break;
	}

	CheckProcessText();
	CheckActionBtnText();

	if( !m_szRollingSound.IsEmpty() )
		g_SoundMgr.PlaySound( m_szRollingSound, DSBVOLUME_MAX, PSM_LOOP | PSM_THREAD );
}

void AlchemicFuncWnd::SetFinalResultState()
{
	MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(g_GUIMgr.FindWnd(MY_INVENTORY_WND));
	if( pInvenWnd )
	{
		pInvenWnd->SetAlchemicItemLockClear();
	}

	ioAlchemicInventory *pInven = g_MyInfo.GetAlchemicInventory();
	if( !pInven )
		return;

	DWORD dwCurTime = FRAMEGETTIME();
	SAFEDELETE(m_pEffect);
	SAFEDELETE(m_pEffect2);

	m_dwFinalResultStartTime = dwCurTime;
	m_dwRollingEffectCheckTime = 0;

	m_iCurEffectArray = -1;
	m_iCurSuccessEffectArray = -1;

	SelectAlchemicItemBtn *pItemBtn1 = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN1));
	SelectAlchemicItemBtn *pItemBtn2 = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN2));
	SelectAlchemicItemBtn *pItemBtn3 = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN3));
	SelectAlchemicItemBtn *pItemBtn4 = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN4));
	SelectAlchemicItemBtn *pAdditiveBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_ADDITIVE_ITEM_BTN));

	m_FuncState = AFS_FINAL_RESULT;
	CheckActionBtnText();
	CheckProcessText();
	CheckSelectItemBtn();

	if( !m_szRollingSound.IsEmpty() )
		g_SoundMgr.StopSound( m_szRollingSound, 0 );

	switch( m_iResultType )
	{
	case ALCHEMIC_SOLDIER_SUCC:
	case ALCHEMIC_NEW_SOLDIER_SUCC:
		if( !m_szSuccessSound.IsEmpty() )
			g_SoundMgr.PlaySound( m_szSuccessSound, DSBVOLUME_MAX, PSM_THREAD );
		ReBatchButton( RBB_TWO );
		break;
	case ALCHEMIC_SOLDIER_FAIL:
		if( pItemBtn1 && pItemBtn1->GetCode() > 0 )
			pItemBtn1->SetDecrease( m_iResultValue1 );

		if( pItemBtn2 && pItemBtn2->GetCode() > 0 )
			pItemBtn2->SetDecrease( m_iResultValue2 );

		if( pItemBtn3 && pItemBtn3->GetCode() > 0 )
			pItemBtn3->SetDecrease( m_iResultValue3 );

		if( pItemBtn4 && pItemBtn4->GetCode() > 0 )
			pItemBtn4->SetDecrease( m_iResultValue4 );

		if( pAdditiveBtn && pAdditiveBtn->GetCode() > 0 )
			pAdditiveBtn->SetDecrease( m_iResultAddValue );

		if( !m_szFailSound.IsEmpty() )
			g_SoundMgr.PlaySound( m_szFailSound, DSBVOLUME_MAX, PSM_THREAD );

		ReBatchButton( RBB_RESULT_ONE );
		break;
	case ALCHEMIC_ITEM_SUCC:
	case ALCHEMIC_NEW_ITEM_SUCC:
		if( !m_szSuccessSound.IsEmpty() )
			g_SoundMgr.PlaySound( m_szSuccessSound, DSBVOLUME_MAX, PSM_THREAD );
		ReBatchButton( RBB_TWO );
		break;
	case ALCHEMIC_ITEM_FAIL:
		if( pItemBtn1 && pItemBtn1->GetCode() > 0 )
			pItemBtn1->SetDecrease( m_iResultValue1 );

		if( pItemBtn2 && pItemBtn2->GetCode() > 0 )
			pItemBtn2->SetDecrease( m_iResultValue2 );

		if( pItemBtn3 && pItemBtn3->GetCode() > 0 )
			pItemBtn3->SetDecrease( m_iResultValue3 );

		if( pItemBtn4 && pItemBtn4->GetCode() > 0 )
			pItemBtn4->SetDecrease( m_iResultValue4 );

		if( pAdditiveBtn && pAdditiveBtn->GetCode() > 0 )
			pAdditiveBtn->SetDecrease( m_iResultAddValue );

		if( !m_szFailSound.IsEmpty() )
			g_SoundMgr.PlaySound( m_szFailSound, DSBVOLUME_MAX, PSM_THREAD );

		ReBatchButton( RBB_RESULT_ONE );
		break;
	case ALCHEMIC_CHANGE_SUCC:
		if( !m_szSuccessSound.IsEmpty() )
			g_SoundMgr.PlaySound( m_szSuccessSound, DSBVOLUME_MAX, PSM_THREAD );

		if( pItemBtn1 && pItemBtn1->GetCode() > 0 )
			pItemBtn1->SetDecrease( m_iResultCode1 );

		if( pItemBtn2 && pItemBtn2->GetCode() > 0 )
			pItemBtn2->SetDecrease( m_iResultCode2 );

		if( pItemBtn3 && pItemBtn3->GetCode() > 0 )
			pItemBtn3->SetDecrease( m_iResultCode3 );

		if( pItemBtn4 && pItemBtn4->GetCode() > 0 )
			pItemBtn4->SetDecrease( m_iResultCode4 );

		ReBatchButton( RBB_RESULT_ONE );
		break;
	case ALCHEMIC_EXCHANGE_SUCC:
		if( !m_szSuccessSound.IsEmpty() )
			g_SoundMgr.PlaySound( m_szSuccessSound, DSBVOLUME_MAX, PSM_THREAD );

		ReBatchButton( RBB_RESULT_ONE );
		break;
	case ALCHEMIC_PESO_SUCC:
		if( !m_szSuccessSound.IsEmpty() )
			g_SoundMgr.PlaySound( m_szSuccessSound, DSBVOLUME_MAX, PSM_THREAD );

		ReBatchButton( RBB_RESULT_ONE );
		break;
	}

	if( pInvenWnd && pInvenWnd->IsShow() )
		pInvenWnd->UpdateTab( false, false );
}

void AlchemicFuncWnd::ProcessResultEffect()
{
	DWORD dwCurTime = FRAMEGETTIME();

	// 네모 이펙트
	if( m_dwResultEffectEndTime > 0 && dwCurTime < m_dwResultEffectEndTime )
	{
		float fRate = FLOAT1;
		if( m_dwResultEffectTime > 0 )
		{
			DWORD dwGap = m_dwResultEffectEndTime - dwCurTime;
			fRate = FLOAT1 - ((float)dwGap / (float)m_dwResultEffectTime);
		}

		//float fCurEffectRate = (float)sin(FLOAT_PI * FLOAT05.0f * fRate);
		float fXValue = fRate - FLOAT1;
		float fPowValue = fabs( pow(fXValue, m_fSlopeRate) );
		float fCurEffectRate = -fPowValue + FLOAT1;

		// 0.0f -> FLOAT1
		m_iAlpha1 = (float)MAX_ALPHA_RATE * fCurEffectRate;

		// 0.0f -> alpharate
		m_iAlpha2 = (float)MAX_ALPHA_RATE * (m_fAlphaRate*fCurEffectRate);

		// scalerate -> FLOAT1
		float fScaleGap = m_fScaleRate - FLOAT1;
		m_fCurEffectScale = m_fScaleRate - (fScaleGap*fCurEffectRate);
	}
}

void AlchemicFuncWnd::ProcessFinalResultEffect()
{
	DWORD dwCurTime = FRAMEGETTIME();

	bool bSuccess = false;
	bool bDestroy = false;

	switch( m_iResultType )
	{
	case ALCHEMIC_SOLDIER_SUCC:
	case ALCHEMIC_ITEM_SUCC:
	case ALCHEMIC_CHANGE_SUCC:
	case ALCHEMIC_EXCHANGE_SUCC:
	case ALCHEMIC_PESO_SUCC:
	case ALCHEMIC_NEW_SOLDIER_SUCC:
	case ALCHEMIC_NEW_ITEM_SUCC:
		bSuccess = true;
		break;
	}

	if( bSuccess )
	{
		if( m_dwFinalResultStartTime > 0 &&  (dwCurTime - m_dwFinalResultStartTime) < m_dwSuccessEffectTime )
			bSuccess = true;
		else
		{
			SAFEDELETE( m_pEffect2 );
			bSuccess = false;
		}
	}

	if( m_dwFinalResultStartTime > 0 && (dwCurTime - m_dwFinalResultStartTime) < m_dwDestroyEffectTime )
		bDestroy = true;
	else
	{
		SAFEDELETE( m_pEffect );
		bDestroy = false;
	}

	bool bChange = false;
	if( m_dwRollingEffectCheckTime == 0 || dwCurTime - m_dwRollingEffectCheckTime > m_dwRollingEffectTime )		// 교체
	{
		bChange = true;
		m_dwRollingEffectCheckTime = dwCurTime;
	}

	// destroy effect
	if( bDestroy )
	{
		int iEffectListCnt = m_vEndEfffect_F.size();
		if( iEffectListCnt == 0 )
		{
			SAFEDELETE( m_pEffect );
		}
		else if( bChange )
		{
			SAFEDELETE( m_pEffect );

			m_iCurEffectArray++;
			if( COMPARE(m_iCurEffectArray, 0, iEffectListCnt) )
			{
				ioHashString szEffect = m_vEndEfffect_F[m_iCurEffectArray];

				if( !szEffect.IsEmpty() )
				{
					m_pEffect = g_UIImageSetMgr.CreateImageByFullName( szEffect );
				}
			}
		}
	}

	// success effect
	if( bSuccess )
	{
		int iEffectListCnt = m_vEndEfffect_S.size();
		if( iEffectListCnt == 0 )
		{
			SAFEDELETE( m_pEffect2 );
			return;
		}
		else if( bChange )
		{
			SAFEDELETE( m_pEffect2 );

			m_iCurSuccessEffectArray++;

			if( COMPARE( m_iCurSuccessEffectArray, 0, iEffectListCnt ) )
			{
				ioHashString szEffect = m_vEndEfffect_S[m_iCurSuccessEffectArray];

				if( !szEffect.IsEmpty() )
				{
					m_pEffect2 = g_UIImageSetMgr.CreateImageByFullName( szEffect );
				}
			}
		}
	}
}

void AlchemicFuncWnd::OnRenderResultEffect()
{
	int iXPos = 0;
	int iYPos = 0;

	SelectAlchemicItemBtn *pItemBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN1));
	if( pItemBtn && pItemBtn->IsShow() && pItemBtn->GetCode() > 0 )
	{
		iXPos = pItemBtn->GetDerivedPosX() + 32;
		iYPos = pItemBtn->GetDerivedPosY() + 32;

		if( m_pLightImage )
		{
			m_pLightImage->SetScale( FLOAT1 );
			m_pLightImage->SetAlpha( m_iAlpha1 );
			m_pLightImage->Render( iXPos, iYPos );

			m_pLightImage->SetScale( m_fCurEffectScale );
			m_pLightImage->SetAlpha( m_iAlpha2 );
			m_pLightImage->Render( iXPos, iYPos );
		}
	}

	pItemBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN2));
	if( pItemBtn && pItemBtn->IsShow() && pItemBtn->GetCode() > 0 )
	{
		iXPos = pItemBtn->GetDerivedPosX() + 32;
		iYPos = pItemBtn->GetDerivedPosY() + 32;

		if( m_pLightImage )
		{
			m_pLightImage->SetScale( FLOAT1 );
			m_pLightImage->SetAlpha( m_iAlpha1 );
			m_pLightImage->Render( iXPos, iYPos );

			m_pLightImage->SetScale( m_fCurEffectScale );
			m_pLightImage->SetAlpha( m_iAlpha2 );
			m_pLightImage->Render( iXPos, iYPos );
		}
	}

	pItemBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN3));
	if( pItemBtn && pItemBtn->IsShow() && pItemBtn->GetCode() > 0 )
	{
		iXPos = pItemBtn->GetDerivedPosX() + 32;
		iYPos = pItemBtn->GetDerivedPosY() + 32;

		if( m_pLightImage )
		{
			m_pLightImage->SetScale( FLOAT1 );
			m_pLightImage->SetAlpha( m_iAlpha1 );
			m_pLightImage->Render( iXPos, iYPos );

			m_pLightImage->SetScale( m_fCurEffectScale );
			m_pLightImage->SetAlpha( m_iAlpha2 );
			m_pLightImage->Render( iXPos, iYPos );
		}
	}

	pItemBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN4));
	if( pItemBtn && pItemBtn->IsShow() && pItemBtn->GetCode() > 0 )
	{
		iXPos = pItemBtn->GetDerivedPosX() + 32;
		iYPos = pItemBtn->GetDerivedPosY() + 32;

		if( m_pLightImage )
		{
			m_pLightImage->SetScale( FLOAT1 );
			m_pLightImage->SetAlpha( m_iAlpha1 );
			m_pLightImage->Render( iXPos, iYPos );

			m_pLightImage->SetScale( m_fCurEffectScale );
			m_pLightImage->SetAlpha( m_iAlpha2 );
			m_pLightImage->Render( iXPos, iYPos );
		}
	}

	pItemBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_ADDITIVE_ITEM_BTN));
	if( pItemBtn && pItemBtn->IsShow() && pItemBtn->GetCode() > 0 )
	{
		iXPos = pItemBtn->GetDerivedPosX() + 32;
		iYPos = pItemBtn->GetDerivedPosY() + 32;

		if( m_pLightImage )
		{
			m_pLightImage->SetScale( FLOAT1 );
			m_pLightImage->SetAlpha( m_iAlpha1 );
			m_pLightImage->Render( iXPos, iYPos );

			m_pLightImage->SetScale( m_fCurEffectScale );
			m_pLightImage->SetAlpha( m_iAlpha2 );
			m_pLightImage->Render( iXPos, iYPos );
		}
	}
}

void AlchemicFuncWnd::OnRenderFinalResultEffect()
{
	OnRenderDestroyEffect();
	OnRenderSuccessEffect();
}

void AlchemicFuncWnd::OnRenderAfterChild()
{
	switch( m_FuncState )
	{
	case AFS_SELECT:
		break;
	case AFS_RESULT:
		OnRenderResultEffect();
		break;
	case AFS_FINAL_RESULT:
		OnRenderFinalResultEffect();
		break;
	}
}

void AlchemicFuncWnd::OnRenderDestroyEffect()
{
	SelectAlchemicItemBtn *pItemBtn1 = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN1));
	SelectAlchemicItemBtn *pItemBtn2 = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN2));
	SelectAlchemicItemBtn *pItemBtn3 = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN3));
	SelectAlchemicItemBtn *pItemBtn4 = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN4));
	SelectAlchemicItemBtn *pAdditiveBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_ADDITIVE_ITEM_BTN));

	SelectAlchemicItemBtn *pResultBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_RESULT_BTN));

	int iXPos = 0;
	int iYPos = 0;

	switch( m_iResultType )
	{
	case ALCHEMIC_SOLDIER_FAIL:
	case ALCHEMIC_ITEM_FAIL:
		if( pItemBtn1 && pItemBtn1->IsShow() && pItemBtn1->GetCode() > 0 )
		{
			iXPos = pItemBtn1->GetDerivedPosX() + 32;
			iYPos = pItemBtn1->GetDerivedPosY() + 32;

			// destroy effect
			if( m_pEffect )
			{
				m_pEffect->SetScale( 1.5f );
				m_pEffect->Render( iXPos, iYPos, UI_RENDER_ADD );
			}
		}

		if( pItemBtn2 && pItemBtn2->IsShow() && pItemBtn2->GetCode() > 0 )
		{
			iXPos = pItemBtn2->GetDerivedPosX() + 32;
			iYPos = pItemBtn2->GetDerivedPosY() + 32;

			// destroy effect
			if( m_pEffect )
			{
				m_pEffect->SetScale( 1.5f );
				m_pEffect->Render( iXPos, iYPos, UI_RENDER_ADD );
			}
		}

		if( pItemBtn3 && pItemBtn3->IsShow() && pItemBtn3->GetCode() > 0 )
		{
			iXPos = pItemBtn3->GetDerivedPosX() + 32;
			iYPos = pItemBtn3->GetDerivedPosY() + 32;

			// destroy effect
			if( m_pEffect )
			{
				m_pEffect->SetScale( 1.5f );
				m_pEffect->Render( iXPos, iYPos, UI_RENDER_ADD );
			}
		}

		if( pItemBtn4 && pItemBtn4->IsShow() && pItemBtn4->GetCode() > 0 )
		{
			iXPos = pItemBtn4->GetDerivedPosX() + 32;
			iYPos = pItemBtn4->GetDerivedPosY() + 32;

			// destroy effect
			if( m_pEffect )
			{
				m_pEffect->SetScale( 1.5f );
				m_pEffect->Render( iXPos, iYPos, UI_RENDER_ADD );
			}
		}

		if( pAdditiveBtn && pAdditiveBtn->IsShow() && pAdditiveBtn->GetCode() > 0 )
		{
			iXPos = pAdditiveBtn->GetDerivedPosX() + 32;
			iYPos = pAdditiveBtn->GetDerivedPosY() + 32;

			// destroy effect
			if( m_pEffect )
			{
				m_pEffect->SetScale( 1.5f );
				m_pEffect->Render( iXPos, iYPos, UI_RENDER_ADD );
			}
		}
		break;
	case ALCHEMIC_EXCHANGE_SUCC:
		if( pItemBtn1 && pItemBtn1->IsShow() )
		{
			iXPos = pItemBtn1->GetDerivedPosX() + 32;
			iYPos = pItemBtn1->GetDerivedPosY() + 32;

			// destroy effect
			if( m_pEffect )
			{
				m_pEffect->SetScale( 1.5f );
				m_pEffect->Render( iXPos, iYPos, UI_RENDER_ADD );
			}
		}

		if( pAdditiveBtn && pAdditiveBtn->IsShow() )
		{
			iXPos = pAdditiveBtn->GetDerivedPosX() + 32;
			iYPos = pAdditiveBtn->GetDerivedPosY() + 32;

			// destroy effect
			if( m_pEffect )
			{
				m_pEffect->SetScale( 1.5f );
				m_pEffect->Render( iXPos, iYPos, UI_RENDER_ADD );
			}
		}
		break;
	}
}

void AlchemicFuncWnd::OnRenderSuccessEffect()
{
	SelectAlchemicItemBtn *pItemBtn2 = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN2));
	SelectAlchemicItemBtn *pResultBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_RESULT_BTN));

	int iXPos = 0;
	int iYPos = 0;

	switch( m_iResultType )
	{
	case ALCHEMIC_SOLDIER_SUCC:
	case ALCHEMIC_ITEM_SUCC:
	case ALCHEMIC_PESO_SUCC:
	case ALCHEMIC_NEW_SOLDIER_SUCC:
	case ALCHEMIC_NEW_ITEM_SUCC:
		if( m_pEffect2 )
		{
			iXPos = GetDerivedPosX()+129;
			iYPos = GetDerivedPosY()+201;

			m_pEffect2->SetScale( 2.0f );
			m_pEffect2->Render( iXPos, iYPos, UI_RENDER_ADD );
		}
		break;
	case ALCHEMIC_CHANGE_SUCC:
		if( pResultBtn && pResultBtn->IsShow() && pResultBtn->GetCode() > 0 )
		{
			iXPos = pResultBtn->GetDerivedPosX() + 32;
			iYPos = pResultBtn->GetDerivedPosY() + 32;

			if( m_pEffect2 )
			{
				m_pEffect2->SetScale( 2.0f );
				m_pEffect2->Render( iXPos, iYPos, UI_RENDER_ADD );
			}
		}
		break;
	case ALCHEMIC_EXCHANGE_SUCC:
		if( pItemBtn2 && pItemBtn2->IsShow() && pItemBtn2->GetCode() > 0 )
		{
			iXPos = pItemBtn2->GetDerivedPosX() + 32;
			iYPos = pItemBtn2->GetDerivedPosY() + 32;

			if( m_pEffect2 )
			{
				m_pEffect2->SetScale( 2.0f );
				m_pEffect2->Render( iXPos, iYPos, UI_RENDER_ADD );
			}
		}
		break;
	}
}

void AlchemicFuncWnd::CreateMagicCircle()
{
	DWORD dwCurTime = FRAMEGETTIME();

	MagicEffectInfo kInfo;
	kInfo.m_dwCreateTime = dwCurTime;

	m_vMagicEffectInfoList.push_back( kInfo );
}

void AlchemicFuncWnd::ProcessMagicCircle()
{
	DWORD dwCurTime = FRAMEGETTIME();

	if( m_dwMagicEffectCreateTime == 0 ||
		m_dwMagicEffectCreateTime+1000 < dwCurTime )
	{
		CreateMagicCircle();
		m_dwMagicEffectCreateTime = dwCurTime;
	}

	// process
	vMagicEffectInfo::iterator iter = m_vMagicEffectInfoList.begin();
	while( iter != m_vMagicEffectInfoList.end() )
	{
		DWORD dwGap = dwCurTime - (*iter).m_dwCreateTime;
		if( dwGap >= 1500 )
		{
			iter = m_vMagicEffectInfoList.erase( iter );
			continue;
		}

		float fRate = FLOAT1 - ((float)dwGap / 1500.0f);

		// /2 를 FLOAT05로 변경
		float fCurEffectRate = (float)sin(FLOAT_PI * FLOAT05 * fRate);
		/*
		float fXValue = fRate - FLOAT1;
		float fPowValue = fabs( pow(fXValue, m_fSlopeRate) );
		float fCurEffectRate = -fPowValue + FLOAT1;
		*/

		// FLOAT05 -> 0.0f
		(*iter).m_iCurAlphaRate = (float)MAX_ALPHA_RATE * ( FLOAT05 * fCurEffectRate );

		// FLOAT1 -> 1.25f
		(*iter).m_fCurScaleRate = 1.25f - (0.25f*fCurEffectRate);

		++iter;
	}
}

void AlchemicFuncWnd::OnRenderMagicCircle()
{
	if( m_FuncState == AFS_PRE_SELECT )
		return;

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	if( m_pMagicCircle )
	{
		m_pMagicCircle->SetAlpha( MAX_ALPHA_RATE );
		m_pMagicCircle->SetScale( FLOAT1 );
		m_pMagicCircle->Render( iXPos+129, iYPos+201 );

		// 퍼지는 이펙트
		vMagicEffectInfo::iterator iter = m_vMagicEffectInfoList.begin();
		while( iter != m_vMagicEffectInfoList.end() )
		{
			m_pMagicCircle->SetAlpha( (*iter).m_iCurAlphaRate );
			m_pMagicCircle->SetScale( (*iter).m_fCurScaleRate );
			m_pMagicCircle->Render( iXPos+129, iYPos+201 );

			++iter;
		}
	}

	switch( m_CurFuncInfo.m_AlchemicType )
	{
	case ALT_EXCHANGE:
		{
			if( m_pMagicCircleArrow )
				m_pMagicCircleArrow->Render( iXPos+27, iYPos+164 );
		}
		break;
	case ALT_NEW_SOLDIER:
	case ALT_NEW_ITEM:
		if( m_FuncState != AFS_FINAL_RESULT )
		{
			if( m_pMagicCircleArrow )
				m_pMagicCircleArrow->Render( iXPos+27, iYPos+164 );

			if( m_pSmallCircle )
				m_pSmallCircle->Render( iXPos+199, iYPos+201 );

			if( m_pNewAlchemicIconBack )
			{
				m_pNewAlchemicIconBack->SetScale( 0.75f );
				m_pNewAlchemicIconBack->Render( iXPos+199, iYPos+201, UI_RENDER_MULTIPLY, TFO_BILINEAR );
			}

			g_UIImageRenderMgr.RenderGradeIconBack( m_nGradeType, iXPos + 199, iYPos + 201, 0.75f );

			if( m_pNewAlchemicIcon )
			{
				m_pNewAlchemicIcon->SetScale( 0.75f );
				m_pNewAlchemicIcon->Render( iXPos+199, iYPos+201, UI_RENDER_NORMAL, TFO_BILINEAR );
			}
		}
		break;
	}
}

void AlchemicFuncWnd::SetPreNewAlchemicSoldier()
{
	ReBatchButton( RBB_SELECT_NONE );

	m_FuncState = AFS_PRE_SELECT;

	SelectAlchemicFuncBtn *pFuncBtn = dynamic_cast<SelectAlchemicFuncBtn*>(FindChildWnd(ID_ALCHEMIC_FUNC_BTN));
	if( pFuncBtn )
	{
		// Show 부터 할 것
		pFuncBtn->ShowWnd();
		pFuncBtn->SetSelectAlchemicFunc( m_CurFuncInfo.m_iCode );
	}

	g_AlchemicMgr.GetNewAlchemicList( m_CurFuncInfo.m_iCode, m_vPreSelectItemList );

	// pre new select
	for( int i=0; i < MAX_PRE_SELECT_ITEM; ++ i )
	{
		ShowChildWnd( ID_PRE_SELECT_ITEM_BTN1+i );
	}

	ShowChildWnd( ID_PAGE_PRE_BTN );
	ShowChildWnd( ID_PAGE_NEXT_BTN );

	CheckPreSelectDesc();
	UpdatePreSelectItemList();
	UpdatePreSelectPage();
}

void AlchemicFuncWnd::SetNewAlchemicSoldier( DWORD dwID )
{
	m_FuncState = AFS_SELECT;

	ReBatchButton( RBB_SELECT_ONE );

	// get pre data
	int iItemValue = 0;
	PreSelectItemBtn* pItemBtn = dynamic_cast<PreSelectItemBtn*>(FindChildWnd(dwID));
	if( pItemBtn )
	{
		iItemValue = pItemBtn->GetCode();
	}

	SelectNewAlchemicSoldier( iItemValue );

	// pre new select hide
	for( int i=0; i < MAX_PRE_SELECT_ITEM; ++ i )
	{
		HideChildWnd( ID_PRE_SELECT_ITEM_BTN1+i );
	}

	HideChildWnd( ID_PAGE_PRE_BTN );
	HideChildWnd( ID_PAGE_NEXT_BTN );

	SelectAlchemicFuncBtn *pFuncBtn = dynamic_cast<SelectAlchemicFuncBtn*>(FindChildWnd(ID_ALCHEMIC_FUNC_BTN));
	if( pFuncBtn )
	{
		// Show 부터 할 것
		pFuncBtn->ShowWnd();
		pFuncBtn->SetSelectNewAlchemicFunc( m_CurFuncInfo.m_iCode, m_iCurNewAlchemicSelectValue );
	}

	SelectAlchemicItemBtn *pItemBtn1 = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN1));
	if( pItemBtn1 )
	{
		pItemBtn1->SetWndPos( 27, 134 );
		pItemBtn1->InitNewData( SelectAlchemicItemBtn::BTT_NEW_PIECE, m_iNewAlchemicPiece, m_CurFuncInfo.m_iMaxCnt1 );
		pItemBtn1->ShowWnd();
	}

	SelectAlchemicItemBtn *pAdditiveBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_ADDITIVE_ITEM_BTN));
	if( pAdditiveBtn )
	{
		pAdditiveBtn->SetWndPos( 27, 204 );
		pAdditiveBtn->InitNewData( SelectAlchemicItemBtn::BTT_NEW_ADDITIVE, m_iNewAlchemicAdditive, m_CurFuncInfo.m_iMaxAdditive );
		pAdditiveBtn->ShowWnd();
	}

	//
	ioHashString szIcon = g_MyInfo.GetMySoldierIconName( m_iCurNewAlchemicSelectValue );
	SAFEDELETE( m_pNewAlchemicIcon );

	m_nGradeType = g_UIImageRenderMgr.GetPowerUpGradeType( m_iCurNewAlchemicSelectValue );

	if( !szIcon.IsEmpty() )
		m_pNewAlchemicIcon = g_UIImageSetMgr.CreateImageByFullName( szIcon );

	//
	MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(g_GUIMgr.FindWnd(MY_INVENTORY_WND));
	if( pInvenWnd )
	{
		pInvenWnd->SetAlchemicItemLockAll();
		pInvenWnd->SetAlchemicItemLockStat( m_iNewAlchemicPiece, false );
		pInvenWnd->SetAlchemicItemLockStat( m_iNewAlchemicAdditive, false );
	}

	CheckActionBtnText();
	CheckSuccessRateText();
	CheckProcessText();
}

void AlchemicFuncWnd::SetPreNewAlchemicItem()
{
	ReBatchButton( RBB_SELECT_NONE );

	m_FuncState = AFS_PRE_SELECT;

	SelectAlchemicFuncBtn *pFuncBtn = dynamic_cast<SelectAlchemicFuncBtn*>(FindChildWnd(ID_ALCHEMIC_FUNC_BTN));
	if( pFuncBtn )
	{
		// Show 부터 할 것
		pFuncBtn->ShowWnd();
		pFuncBtn->SetSelectAlchemicFunc( m_CurFuncInfo.m_iCode );
	}

	g_AlchemicMgr.GetNewAlchemicList( m_CurFuncInfo.m_iCode, m_vPreSelectItemList );

	// pre new select
	for( int i=0; i < MAX_PRE_SELECT_ITEM; ++ i )
	{
		ShowChildWnd( ID_PRE_SELECT_ITEM_BTN1+i );
	}

	ShowChildWnd( ID_PAGE_PRE_BTN );
	ShowChildWnd( ID_PAGE_NEXT_BTN );

	CheckPreSelectDesc();
	UpdatePreSelectItemList();
	UpdatePreSelectPage();
}

void AlchemicFuncWnd::SetNewAlchemicItem( DWORD dwID )
{
	m_FuncState = AFS_SELECT;

	ReBatchButton( RBB_SELECT_ONE );

	// get pre data
	int iItemValue = 0;
	PreSelectItemBtn* pItemBtn = dynamic_cast<PreSelectItemBtn*>(FindChildWnd(dwID));
	if( pItemBtn )
	{
		iItemValue = pItemBtn->GetCode();
	}

	SelectNewalchemicItem( iItemValue );

	// pre new select hide
	for( int i=0; i < MAX_PRE_SELECT_ITEM; ++ i )
	{
		HideChildWnd( ID_PRE_SELECT_ITEM_BTN1+i );
	}

	HideChildWnd( ID_PAGE_PRE_BTN );
	HideChildWnd( ID_PAGE_NEXT_BTN );

	SelectAlchemicFuncBtn *pFuncBtn = dynamic_cast<SelectAlchemicFuncBtn*>(FindChildWnd(ID_ALCHEMIC_FUNC_BTN));
	if( pFuncBtn )
	{
		// Show 부터 할 것
		pFuncBtn->ShowWnd();
		pFuncBtn->SetSelectNewAlchemicFunc( m_CurFuncInfo.m_iCode, m_iCurNewAlchemicSelectValue );
	}

	SelectAlchemicItemBtn *pItemBtn1 = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_SELECT_ITEM_BTN1));
	if( pItemBtn1 )
	{
		pItemBtn1->SetWndPos( 27, 134 );
		pItemBtn1->InitNewData( SelectAlchemicItemBtn::BTT_NEW_PIECE, m_iNewAlchemicPiece, m_CurFuncInfo.m_iMaxCnt1 );
		pItemBtn1->ShowWnd();
	}

	SelectAlchemicItemBtn *pAdditiveBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(ID_ADDITIVE_ITEM_BTN));
	if( pAdditiveBtn )
	{
		pAdditiveBtn->SetWndPos( 27, 204 );
		pAdditiveBtn->InitNewData( SelectAlchemicItemBtn::BTT_NEW_ADDITIVE, m_iNewAlchemicAdditive, m_CurFuncInfo.m_iMaxAdditive );
		pAdditiveBtn->ShowWnd();
	}

	//
	ioHashString szIcon;
	const ioItem *pItem = g_ItemMaker.GetItemConst( m_iCurNewAlchemicSelectValue, __FUNCTION__ );
	if( pItem )
	{
		szIcon = pItem->GetItemLargeIconName();
		m_nGradeType = pItem->GetGradeType();
	}

	SAFEDELETE( m_pNewAlchemicIcon );

	if( !szIcon.IsEmpty() )
		m_pNewAlchemicIcon = g_UIImageSetMgr.CreateImageByFullName( szIcon );

	//
	MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(g_GUIMgr.FindWnd(MY_INVENTORY_WND));
	if( pInvenWnd )
	{
		pInvenWnd->SetAlchemicItemLockAll();
		pInvenWnd->SetAlchemicItemLockStat( m_iNewAlchemicPiece, false );
		pInvenWnd->SetAlchemicItemLockStat( m_iNewAlchemicAdditive, false );
	}

	CheckActionBtnText();
	CheckSuccessRateText();
	CheckProcessText();
}

void AlchemicFuncWnd::SelectNewAlchemicSoldier( int iClassType )
{
	m_iNewAlchemicPiece = 0;
	m_iNewAlchemicAdditive = 0;
	m_iCurNewAlchemicSelectValue = 0;

	if( g_AlchemicMgr.FindNewAlchemicSoldierInfo( iClassType, m_iNewAlchemicPiece, m_iNewAlchemicAdditive ) )
	{
		m_iCurNewAlchemicSelectValue = iClassType;
	}
}

void AlchemicFuncWnd::SelectNewalchemicItem( int iItemCode )
{
	m_iNewAlchemicPiece = 0;
	m_iNewAlchemicAdditive = 0;
	m_iCurNewAlchemicSelectValue = 0;

	if( g_AlchemicMgr.FindNewAlchemicItemInfo( iItemCode, m_iNewAlchemicPiece, m_iNewAlchemicAdditive ) )
	{
		m_iCurNewAlchemicSelectValue = iItemCode;
	}
}

void AlchemicFuncWnd::CheckPreSelectDesc()
{
	ClearPreSelectDesc();

	char szFirst[MAX_PATH] = "";
	char szSecond[MAX_PATH] = "";

	switch( m_CurFuncInfo.m_AlchemicType )
	{
	case ALT_NEW_SOLDIER:
		{
			SafeSprintf( szFirst, sizeof(szFirst), STR(1) );
			SafeSprintf( szSecond, sizeof(szSecond), STR(2) );
		}
		break;
	case ALT_NEW_ITEM:
		{
			SafeSprintf( szFirst, sizeof(szFirst), STR(3) );
			SafeSprintf( szSecond, sizeof(szSecond), STR(4) );
		}
		break;
	}

	m_PreSelectDesc[0].SetTextStyle( TS_NORMAL );
	m_PreSelectDesc[0].SetBkColor( 0, 0, 0 );
	m_PreSelectDesc[0].SetTextColor( TCT_DEFAULT_GRAY );
	m_PreSelectDesc[0].AddTextPiece( FONT_SIZE_12, STR(5), szFirst );

	m_PreSelectDesc[1].SetTextStyle( TS_NORMAL );
	m_PreSelectDesc[1].SetBkColor( 0, 0, 0 );
	m_PreSelectDesc[1].SetTextColor( TCT_DEFAULT_GRAY );
	m_PreSelectDesc[1].AddTextPiece( FONT_SIZE_12, STR(6), szSecond );
}

void AlchemicFuncWnd::UpdatePreSelectItemList()
{
	int iListSize = m_vPreSelectItemList.size();

	m_iMaxPreItemPage = iListSize / MAX_PRE_SELECT_ITEM;
	if( (iListSize%MAX_PRE_SELECT_ITEM) > 0 )
		m_iMaxPreItemPage += 1;
	else if( m_iMaxPreItemPage == 0 )
		m_iMaxPreItemPage = 1;

	m_iCurPreItemPage = 0;
}

void AlchemicFuncWnd::UpdatePreSelectPage()
{
	if( m_iMaxPreItemPage == 0 )
		return;

	int iListSize = m_vPreSelectItemList.size();
	int iCurPos = m_iCurPreItemPage * MAX_PRE_SELECT_ITEM;

	PreSelectItemBtn *pItemBtn = NULL;
	int iItemValue = 0;

	for( int i=0; i < MAX_PRE_SELECT_ITEM; ++i, ++iCurPos )
	{
		iItemValue = 0;
		if( iCurPos < iListSize )
			iItemValue = m_vPreSelectItemList[iCurPos];

		pItemBtn = dynamic_cast<PreSelectItemBtn*>(FindChildWnd(ID_PRE_SELECT_ITEM_BTN1+i));
		if( pItemBtn )
		{
			pItemBtn->SetItemValue( m_CurFuncInfo.m_AlchemicType, iItemValue );
		}
	}

	m_PreSelectPage.ClearList();

	m_PreSelectPage.SetTextStyle( TS_NORMAL );
	m_PreSelectPage.SetBkColor( 0, 0, 0 );
	m_PreSelectPage.SetTextColor( TCT_DEFAULT_DARKGRAY );
	m_PreSelectPage.AddTextPiece( FONT_SIZE_12, STR(1), m_iCurPreItemPage+1 );
	m_PreSelectPage.SetTextColor( TCT_DEFAULT_GRAY );
	m_PreSelectPage.AddTextPiece( FONT_SIZE_12, STR(2), m_iMaxPreItemPage );
}

void AlchemicFuncWnd::ClearPreSelectDesc()
{
	for( int i=0; i < 2; ++i )
	{
		m_PreSelectDesc[i].ClearList();
	}

	m_PreSelectPage.ClearList();
}

bool AlchemicFuncWnd::GetCurAlchemicItemInfo( OUT AlchemicToolTipInfo &rkInfo, IN DWORD dwItemInfoID )
{
	if( COMPARE(dwItemInfoID, ID_PRE_SELECT_ITEM_BTN1, ID_PRE_SELECT_ITEM_BTN15+1) )
	{
		PreSelectItemBtn *pItem = dynamic_cast<PreSelectItemBtn*>(FindChildWnd(dwItemInfoID));
		if( pItem )
		{
			int iCode = pItem->GetCode();
			if( iCode <= 0 )
				return false;

			rkInfo.m_iCode = iCode;

			if( pItem->GetType() == ALT_NEW_SOLDIER )
				rkInfo.m_iType = AlchemicItemToolTip::ATT_SOLDIER;
			else
				rkInfo.m_iType = AlchemicItemToolTip::ATT_ITEM;

			return true;
		}
	}
	else if( COMPARE(dwItemInfoID, ID_SELECT_ITEM_BTN1, ID_SELECT_ITEM_BTN4+1) ||
			 dwItemInfoID == ID_ADDITIVE_ITEM_BTN ||
			 dwItemInfoID == ID_RESULT_BTN )
	{
		SelectAlchemicItemBtn *pItemBtn = dynamic_cast<SelectAlchemicItemBtn*>(FindChildWnd(dwItemInfoID));
		if( pItemBtn )
		{
			int iCode = pItemBtn->GetCode();
			int iPreCode = pItemBtn->GetPreCode();

			if( pItemBtn->GetType() == SelectAlchemicItemBtn::BTT_NEW_PIECE ||
				pItemBtn->GetType() == SelectAlchemicItemBtn::BTT_NEW_ADDITIVE )
			{
				if( iPreCode <= 0 )
					return false;

				rkInfo.m_iCode = iPreCode;
				rkInfo.m_iType = AlchemicItemToolTip::ATT_ALCHEMIC_ITEM;
				return true;
			}
			else if( iCode > 0 )
			{
				rkInfo.m_iCode = iCode;
				rkInfo.m_iType = AlchemicItemToolTip::ATT_ALCHEMIC_ITEM;
				return true;
			}
		}
	}

	return false;
}

