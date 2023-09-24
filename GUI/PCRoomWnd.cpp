

#include "StdAfx.h"

#include "../io3DEngine/ioWnd.h"
#include "../io3DEngine/ioFontManager.h"
#include "../io3DEngine/ioUIRenderer.h"
#include "../io3DEngine/ioUIImageSetManager.h"
#include "../io3DEngine/ioUIRenderImage.h"
#include "../io3DEngine/ioUIRenderFrame.h"
#include "../io3DEngine\ioXMLElement.h"
#include "../io3DEngine/ioXMLDocument.h"

#include "../TextColorTable.h"
#include "../NetworkWrappingFunc.h"
#include "../ioApplication.h"
#include "../ioComplexStringPrinter.h"
#include "../ioEventManager.h"
#include "../ioMyInfo.h"
#include "../ioClassPrice.h"
#include "../ioItemMaker.h"
#include "../ioSetItemInfo.h"
#include "../ioSetItemInfoManager.h"
#include "../ioPlayStage.h"
#include "../ioBaseChar.h"
#include "../WndID.h"

#include "ioSP2GUIManager.h"
#include "SoldierSelectWnd.h"
#include "PCRoomWnd.h"
#include "../io3DEngine/ioStringManager.h"
#include "../Local/ioLocalParent.h"

bool PCRoomAuthorityWnd::sg_bAutoShowPossible = true;
PCRoomAuthorityWnd::PCRoomAuthorityWnd()
{
	m_pBackGround = NULL;
	m_pIconBack	  = NULL;
	m_pPCRoomIcon = NULL;
	m_pPCRoomPeso = NULL;
	m_pPCRoomExp  = NULL;
	m_pSoldierIcon= NULL;

	m_iPesoBonus = m_iExpBonus = 0;
}

PCRoomAuthorityWnd::~PCRoomAuthorityWnd()
{
	SAFEDELETE( m_pBackGround );
	SAFEDELETE( m_pIconBack );
	SAFEDELETE( m_pPCRoomIcon );
	SAFEDELETE( m_pPCRoomPeso );
	SAFEDELETE( m_pPCRoomExp  );
	SAFEDELETE( m_pSoldierIcon );
}

void PCRoomAuthorityWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "BackGround" )
	{
		SAFEDELETE( m_pBackGround );
		m_pBackGround = pImage;
	}
	else if( szType == "IconBack" )
	{
		SAFEDELETE( m_pIconBack );
		m_pIconBack = pImage;
	}
	else if( szType == "PCRoomIcon" )
	{
		SAFEDELETE( m_pPCRoomIcon );
		m_pPCRoomIcon = pImage;
	}
	else if( szType == "PCRoomPeso" )
	{
		SAFEDELETE( m_pPCRoomPeso );
		m_pPCRoomPeso = pImage;
	}
	else if( szType == "PCRoomExp" )
	{
		SAFEDELETE( m_pPCRoomExp );
		m_pPCRoomExp = pImage;
	}
	else
		ioWnd::AddRenderImage( szType, pImage );
}

void PCRoomAuthorityWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	m_iPesoBonus = xElement.GetIntAttribute_e( "PesoBonus" );
	m_iExpBonus = xElement.GetIntAttribute_e( "ExpBonus" );
}

void PCRoomAuthorityWnd::iwm_show()
{
	// �ֽ� �뺴 ������
	int iClassType = 1;
	int iSize = g_ClassPrice.MaxClassPrice();
	for (int i = 0; i < iSize ; i++)
	{
		if( !g_ClassPrice.IsActiveClass( i ) ) continue;
		if( g_ClassPrice.IsCashOnlyByArray( i ) )
			continue;

		int iSetIdx = g_ClassPrice.ArrayToSetItemIdx( i );
		const ioSetItemInfo *pInfo = g_SetItemInfoMgr.GetSetInfoByIdx( iSetIdx );
		if( !pInfo ) continue;

		int iType = pInfo->GetSetCode() - SET_ITEM_CODE;
		if( iType > iClassType )
			iClassType = iType;
	}

	SAFEDELETE( m_pSoldierIcon );
	m_pSoldierIcon = g_MyInfo.GetSoldierIcon( iClassType, true );

	sg_bAutoShowPossible = false;
}

bool PCRoomAuthorityWnd::iwm_spacebar()
{
	iwm_command( FindChildWnd( ID_SOLDIER_BTN ), IOBN_BTNUP, 0 );
	return true;
}

void PCRoomAuthorityWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();

	switch( dwID )
	{
	case ID_SOLDIER_BTN:
		if( cmd == IOBN_BTNUP )
		{
			ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
			if( pLocal && pLocal->IsPCRoomBonusSoldier() )
			{
				g_GUIMgr.ShowWnd( PCROOM_SOLDIER_BONUS_WND );
			}
			HideWnd();
		}
		break;
	}
}

void PCRoomAuthorityWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;
}

void PCRoomAuthorityWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	if( m_pBackGround )
		m_pBackGround->Render( iXPos + 175, iYPos + 91, UI_RENDER_MULTIPLY );
	if( m_pPCRoomIcon )
		m_pPCRoomIcon->Render( iXPos + 175, iYPos + 91 );

	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
	g_FontMgr.PrintText( iXPos + 175, iYPos + 144, FONT_SIZE_13, STR(1) );
	g_FontMgr.PrintText( iXPos + 175, iYPos + 162, FONT_SIZE_13, STR(2) );

	OnBonusSoldier( iXPos + 17, iYPos + 191 );
	OnBonusPeso( iXPos + 123, iYPos + 191 );
	OnBonusExp( iXPos + 229, iYPos + 191 );
}

void PCRoomAuthorityWnd::OnBonusSoldier( int iXPos, int iYPos )
{
	if( m_pIconBack )
	{
		m_pIconBack->SetScale( FLOAT1 );
		m_pIconBack->Render( iXPos + 52, iYPos + 49, UI_RENDER_MULTIPLY );
		if( m_pSoldierIcon )
		{
			m_pSoldierIcon->SetScale( FLOAT1 );
			m_pSoldierIcon->Render( iXPos + 52, iYPos + 49 );
		}
	}

	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
	g_FontMgr.PrintText( iXPos + 52, iYPos + FLOAT100, FONT_SIZE_13, STR(1) );
	g_FontMgr.PrintText( iXPos + 52, iYPos + 116, FONT_SIZE_13, STR(2) );
	g_FontMgr.SetTextColor( TCT_DEFAULT_RED );
	g_FontMgr.PrintText( iXPos + 52, iYPos + 132, FONT_SIZE_17, STR(3) );
}

void PCRoomAuthorityWnd::OnBonusPeso( int iXPos, int iYPos )
{
	if( m_pIconBack )
	{
		m_pIconBack->SetScale( FLOAT1 );
		m_pIconBack->Render( iXPos + 52, iYPos + 49, UI_RENDER_MULTIPLY );
		if( m_pPCRoomPeso )
		{
			m_pPCRoomPeso->SetScale( FLOAT1 );
			m_pPCRoomPeso->Render( iXPos + 52, iYPos + 49 );
		}
	}

	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
	g_FontMgr.PrintText( iXPos + 52, iYPos + FLOAT100, FONT_SIZE_13, STR(1) );
	g_FontMgr.PrintText( iXPos + 52, iYPos + 116, FONT_SIZE_13, STR(2) );
	g_FontMgr.SetTextColor( TCT_DEFAULT_RED );
	g_FontMgr.PrintText( iXPos + 52, iYPos + 132, FONT_SIZE_17, STR(3), m_iPesoBonus );
}

void PCRoomAuthorityWnd::OnBonusExp( int iXPos, int iYPos )
{
	if( m_pIconBack )
	{
		m_pIconBack->SetScale( FLOAT1 );
		m_pIconBack->Render( iXPos + 52, iYPos + 49, UI_RENDER_MULTIPLY );
		if( m_pPCRoomExp )
		{
			m_pPCRoomExp->SetScale( FLOAT1 );
			m_pPCRoomExp->Render( iXPos + 52, iYPos + 49 );
		}
	}

	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
	g_FontMgr.PrintText( iXPos + 52, iYPos + FLOAT100, FONT_SIZE_13, STR(1) );
	g_FontMgr.PrintText( iXPos + 52, iYPos + 116, FONT_SIZE_13, STR(2) );
	g_FontMgr.SetTextColor( TCT_DEFAULT_RED );
	g_FontMgr.PrintText( iXPos + 52, iYPos + 132, FONT_SIZE_17, STR(3), m_iExpBonus );
}

//////////////////////////////////////////////////////////////////////////
PCRoomSoldierBonusBtn::PCRoomSoldierBonusBtn()
{
	m_pIconBack = NULL;
	m_pCharIcon = NULL;

	m_iClassType= -1;
}

PCRoomSoldierBonusBtn::~PCRoomSoldierBonusBtn()
{
	SAFEDELETE( m_pIconBack );
	SAFEDELETE( m_pCharIcon );
}

void PCRoomSoldierBonusBtn::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "IconBack" )
	{
		SAFEDELETE( m_pIconBack );
		m_pIconBack = pImage;
	}
	else
	{
		ioButton::AddRenderImage( szType, pImage );
	}
}

void PCRoomSoldierBonusBtn::SetClassType( int iClassType )
{
	m_iClassType = iClassType;
	if( m_iClassType != -1 )
	{
		SAFEDELETE( m_pCharIcon );
		m_pCharIcon = g_MyInfo.GetMySoldierIcon( iClassType );

		SetActive();
	}
	else
	{
		SetInActive();
	}
}

void PCRoomSoldierBonusBtn::OnRender()
{
	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	ioButton::OnDrawNormal( iXPos, iYPos );

	if( IsDisabled() ) 
		return;

	PCRoomSoldierBonusWnd *pParentWnd = dynamic_cast<PCRoomSoldierBonusWnd*>( GetParent() );
	if( pParentWnd )
	{
		if( pParentWnd->GetSelectBtn() == GetID() )
		{
			ioButton::OnDrawOvered( iXPos, iYPos );
		}
	}

	if( m_pIconBack )
	{
		m_pIconBack->SetScale( 0.60f );
		m_pIconBack->Render( iXPos + 29, iYPos + 29, UI_RENDER_MULTIPLY, TFO_BILINEAR );
	}

	if( m_pCharIcon )
	{
		m_pCharIcon->SetScale( 0.60f );
		m_pCharIcon->Render( iXPos + 29, iYPos + 29, UI_RENDER_NORMAL, TFO_BILINEAR );
	}
}

//////////////////////////////////////////////////////////////////////////
PCRoomSoldierBonusWnd::PCRoomSoldierBonusWnd()
{
	m_iCurPage = m_iMaxPage = 0;
	m_iSelectBtn = 0;
	m_pPlayStage  = NULL;
}

PCRoomSoldierBonusWnd::~PCRoomSoldierBonusWnd()
{
	
}

void PCRoomSoldierBonusWnd::SetPlayStage( ioPlayStage *pPlayStage )
{
	m_pPlayStage = pPlayStage;
}

void PCRoomSoldierBonusWnd::UpdateSetPage( int iCurPage )
{
	int iPrevPage = m_iCurPage;
	m_iCurPage = iCurPage;

	int iListSIze = m_ClassTypeList.size();
	int iStartPos = iCurPage * MAX_PAGE_LIST;
	for(int i = ID_BONUS_CHAR_BTN1;i < ID_BONUS_CHAR_BTN6 + 1;i++,iStartPos++)
	{
		PCRoomSoldierBonusBtn *pBtn = dynamic_cast<PCRoomSoldierBonusBtn*>( FindChildWnd( i ) );
		if( pBtn )
		{
			if( iStartPos < iListSIze )
			{
				pBtn->SetClassType( m_ClassTypeList[iStartPos] );
			}
			else
			{
				pBtn->SetClassType( -1 );
			}
		}
	}	
}

int PCRoomSoldierBonusWnd::FindClassTypeBtn( bool bFirstBtn )
{
	if( bFirstBtn )
	{
		for(int i = ID_BONUS_CHAR_BTN1;i < ID_BONUS_CHAR_BTN6 + 1;i++)
		{
			PCRoomSoldierBonusBtn *pSelectBtn = dynamic_cast<PCRoomSoldierBonusBtn*>( FindChildWnd( i ) );
			if( pSelectBtn && !pSelectBtn->IsDisabled() )
			{
				return i;
			}
		}	
	}
	else
	{
		for(int i = ID_BONUS_CHAR_BTN6;i >= ID_BONUS_CHAR_BTN1;i--)
		{
			PCRoomSoldierBonusBtn *pSelectBtn = dynamic_cast<PCRoomSoldierBonusBtn*>( FindChildWnd( i ) );
			if( pSelectBtn && !pSelectBtn->IsDisabled() )
			{
				return i;
			}
		}	
	}

	return 0;
}

int PCRoomSoldierBonusWnd::LastClassTypeBtn()
{
	int iResult = ID_BONUS_CHAR_BTN1;
	for(int i = ID_BONUS_CHAR_BTN1;i < ID_BONUS_CHAR_BTN6 + 1;i++)
	{
		PCRoomSoldierBonusBtn *pSelectBtn = dynamic_cast<PCRoomSoldierBonusBtn*>( FindChildWnd( i ) );
		if( pSelectBtn && !pSelectBtn->IsDisabled() )
		{
			iResult = i;
		}
	}	
	return iResult;
}

void PCRoomSoldierBonusWnd::iwm_show()
{
	m_ClassTypeList.clear();

	int iMax = g_ClassPrice.MaxClassPrice();
	for (int i = 0; i <  iMax; i++)
	{
		if( !g_ClassPrice.IsActiveClass( i ) ) continue;
		if( g_ClassPrice.IsCashOnlyByArray( i ) )
			continue;

		int iSetIdx = g_ClassPrice.ArrayToSetItemIdx( i );
		const ioSetItemInfo *pInfo = g_SetItemInfoMgr.GetSetInfoByIdx( iSetIdx );
		if( !pInfo ) continue;

		int iClassType = pInfo->GetSetCode() - SET_ITEM_CODE;
		if( g_MyInfo.GetCharIndexByClassType( iClassType ) != 0 ) continue;
		
		m_ClassTypeList.insert( m_ClassTypeList.begin(), iClassType );
	}
	m_iMaxPage = max( 0, (int)m_ClassTypeList.size() - 1 ) / MAX_PAGE_LIST;
	UpdateSetPage( 0 );

	if( m_ClassTypeList.empty() )
	{
		HideWnd();
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
	}
}

bool PCRoomSoldierBonusWnd::iwm_spacebar()
{
	if( !COMPARE( m_iSelectBtn, ID_BONUS_CHAR_BTN1, ID_BONUS_CHAR_BTN6 + 1 ) )
		return false;

	iwm_command( FindChildWnd( m_iSelectBtn ), IOBN_BTNUP, 0 );
	return true;
}

bool PCRoomSoldierBonusWnd::IsCanExerciseChar( bool bSelectWait )
{
	if( !m_pPlayStage ) return true;
	if( g_MyInfo.GetCharCount() == 0 ) return true;

	ioBaseChar *pOwner = m_pPlayStage->GetOwnerChar();
	if( !pOwner ) return true;
	if( bSelectWait && !pOwner->IsCanChangeCharState( true, false, true ) ) return false;

	if( pOwner->GetExperienceMode() == EMS_CHANGE_EXPERIENCE )
		return false;

	return true;
}

void PCRoomSoldierBonusWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
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
	case ID_LEFT_PAGE:
		if( cmd == IOBN_BTNUP )
		{
			UpdateSetPage( max( 0, m_iCurPage - 1 ) );
		}
		break;
	case ID_RIGHT_PAGE:
		if( cmd == IOBN_BTNUP )
		{
			UpdateSetPage( min( m_iMaxPage, m_iCurPage + 1 ) );
		}
		break;
	case ID_BONUS_CHAR_BTN1:
	case ID_BONUS_CHAR_BTN2:
	case ID_BONUS_CHAR_BTN3:
	case ID_BONUS_CHAR_BTN4:
	case ID_BONUS_CHAR_BTN5:
	case ID_BONUS_CHAR_BTN6:
		if( cmd == IOBN_BTNUP )
		{
			PCRoomSoldierBonusBtn *pBtn = dynamic_cast<PCRoomSoldierBonusBtn*>( pWnd );
			if( pBtn->GetClassType() == -1 )
			{
				HideWnd();
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );				
			}
			else if( g_MyInfo.GetCharIndexByClassType( pBtn->GetClassType() ) != 0 )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
			}
			else
			{
				int iCurMaxSlot = g_MyInfo.GetCurMaxCharSlot();
				bool bCharDelete = g_MyInfo.IsCharInActive( pBtn->GetClassType() );
				if( g_MyInfo.GetCharCount() >= iCurMaxSlot && g_MyInfo.GetExerciseCharCount() == 0 )
				{
					if( !bCharDelete )
					{
						HideWnd();
						g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
						return;
					}
				}

				bool bWait = true;
				SoldierSelectWnd *pSelectWnd = dynamic_cast<SoldierSelectWnd*>(g_GUIMgr.FindWnd( SOLDIER_SELECT_WND ));
				if( pSelectWnd && pSelectWnd->IsShow() && pSelectWnd->IsForcedWait() ) // hide �ʱⰪ�� forcewait�� �ǹǷ� IsShow Ȯ��
					bWait = false;

				if( !IsCanExerciseChar( bWait ) )
				{
					HideWnd();				
					return;
				}

				bool bOneChar = true;
				if( m_pPlayStage && !m_pPlayStage->IsOneMyChar() )
					bOneChar = false;

				SP2Packet kPacket( CTPK_EXERCISE_PCROOM_CHAR_CREATE );
				kPacket << bWait;
				kPacket << bOneChar;
				kPacket << 1;
				kPacket << pBtn->GetClassType();
				kPacket << bCharDelete;				
				TCPNetwork::SendToServer( kPacket );
				g_App.SetMouseBusy( true );
				HideWnd();
			}
		}
		else if( cmd == IOWN_OVERED )
		{
			PCRoomSoldierBonusBtn *pBtn = dynamic_cast<PCRoomSoldierBonusBtn*>( pWnd );
			if( pBtn->GetClassType() != -1 )
			{
				m_iSelectBtn = dwID;
			}
		}
		break;
	}
}

void PCRoomSoldierBonusWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) 
		return;

	// ���õ� ��ư�� ����.
	if( !COMPARE( m_iSelectBtn, ID_BONUS_CHAR_BTN1, ID_BONUS_CHAR_BTN6 + 1 ) )
	{
		for(int i = ID_BONUS_CHAR_BTN1;i < ID_BONUS_CHAR_BTN6 + 1;i++)
		{
			PCRoomSoldierBonusBtn *pSelectBtn = dynamic_cast<PCRoomSoldierBonusBtn*>( FindChildWnd( i ) );
			if( pSelectBtn && !pSelectBtn->IsDisabled() )
			{
				m_iSelectBtn = i;
				break;
			}
		}	
	}
	else
	{
		// ���õǾ��ִ� ��ư üũ
		PCRoomSoldierBonusBtn *pSelectBtn = dynamic_cast<PCRoomSoldierBonusBtn*>( FindChildWnd( m_iSelectBtn ) );
		if( !pSelectBtn || pSelectBtn->IsDisabled() )
		{
			m_iSelectBtn = FindClassTypeBtn( true );
		}
		else if( m_pPlayStage )
		{
			// Ű���� �Է�		
			ioBaseChar *pOwner = m_pPlayStage->GetOwnerChar();
			if( !pOwner ) return;

			if( pOwner->IsLeftKeyRelease() )
			{
				m_iSelectBtn--;
				if( m_iSelectBtn < ID_BONUS_CHAR_BTN1 )
				{
					if( m_iCurPage - 1 < 0 )
						UpdateSetPage( m_iMaxPage );
					else
						UpdateSetPage( m_iCurPage - 1 );

					m_iSelectBtn = ID_BONUS_CHAR_BTN6;
				}
				PCRoomSoldierBonusBtn *pSelectBtn = dynamic_cast<PCRoomSoldierBonusBtn*>( FindChildWnd( m_iSelectBtn ) );
				if( !pSelectBtn || pSelectBtn->IsDisabled() )
				{
					m_iSelectBtn = FindClassTypeBtn( false );
				}
			}
			else if( pOwner->IsRightKeyRelease() )
			{
				m_iSelectBtn++;
				if( m_iSelectBtn > LastClassTypeBtn() )
				{
					if( m_iCurPage + 1 > m_iMaxPage )
						UpdateSetPage( 0 );
					else
						UpdateSetPage( m_iCurPage + 1 );

					m_iSelectBtn = ID_BONUS_CHAR_BTN1;
				}
				PCRoomSoldierBonusBtn *pSelectBtn = dynamic_cast<PCRoomSoldierBonusBtn*>( FindChildWnd( m_iSelectBtn ) );
				if( !pSelectBtn || pSelectBtn->IsDisabled() )
				{
					m_iSelectBtn = FindClassTypeBtn( true );
				}	
			}
		}		
	}
}

void PCRoomSoldierBonusWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextStyle( TS_NORMAL);
	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
	g_FontMgr.PrintText( iXPos + 192, iYPos + 41, FONT_SIZE_13, STR(1) );

	ioComplexStringPrinter kPrinter;
	kPrinter.SetTextStyle( TS_NORMAL );
	kPrinter.SetBkColor( 0, 0, 0 );
	kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter.AddTextPiece( FONT_SIZE_12, STR(2), m_iCurPage + 1 );

	kPrinter.SetTextStyle( TS_NORMAL );
	kPrinter.SetBkColor( 0, 0, 0 );
	kPrinter.SetTextColor( TCT_DEFAULT_GRAY );
	kPrinter.AddTextPiece( FONT_SIZE_12, STR(3), m_iMaxPage + 1 );
	kPrinter.PrintFullText( iXPos + 192, iYPos + 131, TAT_CENTER );
}
