

#include "StdAfx.h"

#include "../io3DEngine/ioWnd.h"
#include "../io3DEngine/ioFontManager.h"
#include "../io3DEngine/ioINILoader.h"
#include "../io3DEngine/ioUIRenderImage.h"
#include "../io3DEngine/ioUIRenderFrame.h"
#include "../io3DEngine/ioUIImageSetManager.h"
#include "../io3DEngine/ioXMLElement.h"
#include "../io3DEngine/ioSoundManager.h"
#include "../io3DEngine/HelpFunc.h"
#include "../io3DEngine/iostringmanager.h"
#include "../io3dengine/safesprintf.h"

#include "../WndID.h"
#include "../ioMyInfo.h"
#include "../Setting.h"
#include "../NetworkWrappingFunc.h"
#include "../TextColorTable.h"
#include "../ioApplication.h"
#include "../EtcHelpFunc.h"
#include "../ioPlayStage.h"
#include "../ioBaseChar.h"
#include "../ioClassPrice.h"
#include "../ioItemMaker.h"
#include "../ioSetItemInfo.h"
#include "../ioSetItemInfoManager.h"
#include "../ioQuestManager.h"
#include "../ioBowItem.h"
#include "../ioLaserItem.h"
#include "../ioFireWeaponItem.h"
#include "../ioAutoShotItem.h"
#include "../ioHeadquartersMode.h"

#include "ioTutorialManager.h"
#include "ioSP2GUIManager.h"
#include "SoldierSelectWnd.h"
#include "SoldierPracticeWnd.h"

#include <strsafe.h>
#include "../Local/ioLocalManager.h"

PracticeHelpWnd::PracticeHelpWnd()
{
	m_iStep = 0;
	m_iClassType = 0;
	m_dwTextTickTime	= 0;
	m_dwCurTextTickTime = 0;
	m_iCurTextTickCount	= 0;
	m_iCurTextLineCount	= 0;
	m_pPlayStage = NULL;

	m_pDevK = NULL;
	m_pDevKBack = NULL;
}

PracticeHelpWnd::~PracticeHelpWnd()
{
	SAFEDELETE( m_pDevK );
	SAFEDELETE( m_pDevKBack );
	m_vPracticeHelp.clear();
	m_vStringTickList.clear();
}

void PracticeHelpWnd::SetPlayStage( ioPlayStage *pPlayStage )
{
	m_pPlayStage = pPlayStage;
}

bool PracticeHelpWnd::SetPracticeHelp( int iClassType )
{
	if( m_pPlayStage == NULL ) return false;

	if( g_TutorialMgr.IsTutorial() )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return false;
	}

	if( m_pPlayStage->GetModeType() != MT_HEADQUARTERS )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return false;
	}

	if( iClassType == -1 ) 
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		return false;
	}

	m_iClassType = iClassType;
	m_vPracticeHelp.clear();
	m_vStringTickList.clear();

	char szBuf[MAX_PATH], szKey[MAX_PATH];
	ioINILoader_e kLoader( "config/sp2_practice_help.ini" );
	sprintf_e( szBuf, "Set%d", m_iClassType );
	kLoader.SetTitle( szBuf );

	int iMaxStep = kLoader.LoadInt_e( "max_step", 0 );
	if( iMaxStep == 0 )
		return false;
	for(int i = 0;i < iMaxStep;i++)
	{
		PracticeHelp kHelp;
		sprintf_e( szKey, "step%d_title", i );
		kLoader.LoadString( szKey, "", szBuf, MAX_PATH );
		kHelp.m_szTitle = szBuf;

		sprintf_e( szKey, "step%d_help", i );
		kLoader.LoadString( szKey, "", szBuf, MAX_PATH );
		kHelp.m_szHelp = szBuf;

		m_vPracticeHelp.push_back( kHelp );
	}
	ShowWnd();
	SetStep( 0 );

	return true;
}

void PracticeHelpWnd::SetStep( int iStep )
{
	if( !COMPARE( iStep, 0, (int)m_vPracticeHelp.size() ) ) return;

	m_iStep = iStep;
	PracticeHelp &kHelp = m_vPracticeHelp[m_iStep];
	SplitDescription( kHelp.m_szHelp );
	SetTitleText( kHelp.m_szTitle.c_str() );

	if( (int)m_vPracticeHelp.size() == m_iStep + 1 )
	{
		HideChildWnd( ID_NEXT );
		ShowChildWnd( ID_CLOSE );
		SetChildInActive( ID_CLOSE );
	}
	else
	{
		HideChildWnd( ID_CLOSE );
		ShowChildWnd( ID_NEXT );
		SetChildInActive( ID_NEXT );
	}

	if( iStep == 0 )
		SetChildInActive( ID_PREV );
	else
		SetChildActive( ID_PREV );

	//
	if( m_DescList.empty() ) return;

	m_vStringTickList.clear();

	m_dwCurTextTickTime = FRAMEGETTIME();
	m_iCurTextTickCount = 0;
	m_iCurTextLineCount = 0;
	int iSize = m_DescList.size();
	for(int i = 0;i < iSize;i++)
	{
		StringTick kTick;
		m_vStringTickList.push_back( kTick );
	}
}

void PracticeHelpWnd::SplitDescription( const ioHashString &szDesc )
{
	m_DescList.clear();
	m_DescList = ioStringConverter::Split( szDesc.c_str(), "#" );
}

void PracticeHelpWnd::iwm_show()
{
	if( g_GUIMgr.IsShow( EXERCISE_SOLDIER_EVENT_WND ) )
		g_GUIMgr.HideWnd( EXERCISE_SOLDIER_EVENT_WND );
}


void PracticeHelpWnd::iwm_hide()
{
	if( !m_pPlayStage ) return;

	ioHeadquartersMode *pHeadquartersMode = ToHeadquartersMode( m_pPlayStage->GetPlayMode() );
	if( pHeadquartersMode && pHeadquartersMode->GetCharState() == ioHeadquartersMode::STATE_UNITE_CHAR )
	{
		if( pHeadquartersMode->GetRecordPlayUserCnt() == 1 )
		{
			SP2Packet kPacket( CTPK_HEADQUARTERS_STATE_CHANGE );
			kPacket << (int)ioHeadquartersMode::STATE_OPEN_CHAR;
			TCPNetwork::SendToServer( kPacket );
			TCPNetwork::MouseBusy( true );
		}
	}

	// 해외 이벤트
	if( !g_GUIMgr.IsShow( EXERCISE_SOLDIER_EVENT_WND ) )
		g_GUIMgr.ShowWnd( EXERCISE_SOLDIER_EVENT_WND );
}

bool PracticeHelpWnd::iwm_esc()
{
	return SetNextAction();
}

void PracticeHelpWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
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
	case ID_CLOSE:
		if( cmd == IOBN_BTNUP )
		{
			HideWnd();
			// 완료
			g_QuestMgr.QuestCompleteTerm( QC_SOLDIER_PRACTICE_SUCCESS, m_iClassType );
		}
		break;
	case ID_NEXT:
		if( cmd == IOBN_BTNUP )
		{
			// 다음 단계
			SetStep( m_iStep + 1 );			
		}
		break;
	case ID_PREV:
		if( cmd == IOBN_BTNUP )
		{
			// 이전 단계
			SetStep( max( 0, m_iStep - 1 ) );			
		}
		break;
	}
}

void PracticeHelpWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "DevK" )
	{
		SAFEDELETE( m_pDevK );
		m_pDevK = pImage;
	}
	else if( szType == "DevKBack" )
	{
		SAFEDELETE( m_pDevKBack );
		m_pDevKBack = pImage;
	}
	else
		ioWnd::AddRenderImage( szType, pImage );
}

void PracticeHelpWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	m_dwTextTickTime = xElement.GetIntAttribute_e( "TextTickTime" );
	m_szTextTickSnd  = xElement.GetStringAttribute_e( "TextTickSnd" );	
}

void PracticeHelpWnd::ProcessTextTick()
{
	if( m_dwCurTextTickTime == 0 ) return;
	if( !COMPARE( m_iCurTextLineCount, 0, (int)m_DescList.size() ) ) return;
	if( FRAMEGETTIME() - m_dwCurTextTickTime < m_dwTextTickTime ) return;

	m_dwCurTextTickTime = FRAMEGETTIME();

	if( !m_szTextTickSnd.IsEmpty() )
		g_SoundMgr.PlaySound( m_szTextTickSnd );

	char szHelp[MAX_PATH] = "";
	memset( szHelp, 0, sizeof( szHelp ) );
	sprintf( szHelp, "%s", m_DescList[m_iCurTextLineCount].c_str() );

	int iMaxLen = strlen( szHelp );

#if defined( SRC_OVERSEAS ) && !defined( SRC_ID )

#if defined( MULTI_BYTE_CHECK )
	if( IsDBCSLeadByteEx( COUNTRY_CODE_PAGE, (BYTE) szHelp[m_iCurTextTickCount] ) )
#else
	if( false && IsDBCSLeadByteEx( COUNTRY_CODE_PAGE, (BYTE) szHelp[m_iCurTextTickCount] ) )
#endif

#else
	if( IsDBCSLeadByte( (BYTE) szHelp[m_iCurTextTickCount] ) )
#endif

		m_iCurTextTickCount+=2;
	else 
		m_iCurTextTickCount+=1;

	memcpy( m_vStringTickList[m_iCurTextLineCount].m_szCopyDesc, szHelp, m_iCurTextTickCount );

	if( m_iCurTextTickCount >= iMaxLen )
	{
		m_iCurTextTickCount = 0;
		m_iCurTextLineCount++;
	}

	if( m_iCurTextLineCount >= (int)m_DescList.size() )
	{
		m_dwCurTextTickTime = 0;
		m_iCurTextTickCount = 0;
		m_iCurTextLineCount = 0;

		SetChildActive( ID_NEXT );
		SetChildActive( ID_CLOSE );
	}
}

bool PracticeHelpWnd::iwm_spacebar()
{
	return SetNextAction();
}

void PracticeHelpWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() )	
		return;  

	if( m_pPlayStage )
	{
		ioBaseChar *pOwner = m_pPlayStage->GetOwnerChar();
		if( pOwner && pOwner->GetState() != CS_CHANGE_WAIT )
		{
			if( m_iClassType != g_MyInfo.GetClassType() )
			{
				HideWnd();
			}		
		}
	}	

	ProcessTextTick();
}

void PracticeHelpWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos, iYPos;
	iXPos = GetDerivedPosX();
	iYPos = GetDerivedPosY();
	if( m_pDevKBack && m_pDevK )
	{
		m_pDevKBack->SetScale( 0.85f );
		m_pDevKBack->Render( iXPos + 40, iYPos + 70, UI_RENDER_MULTIPLY, TFO_BILINEAR );

		m_pDevK->SetScale( 0.85f );
		m_pDevK->Render( iXPos + 40, iYPos + 70, UI_RENDER_NORMAL, TFO_BILINEAR );
	}

	//////////////////////////////////////////////////////////////////////////	
	g_FontMgr.SetAlignType( TAT_LEFT );
	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );	
	int i = 0;
	int iInfoSize = m_vStringTickList.size();      
	for(;i < iInfoSize;i++)
		g_FontMgr.PrintText( iXPos + 77, iYPos + 47 + ( 17 * i ), FONT_SIZE_12, m_vStringTickList[i].m_szCopyDesc );
}

bool PracticeHelpWnd::SetNextAction()
{
	if( !IsShow() )
		return false;

	ioButton *pButton = dynamic_cast<ioButton*>( FindChildWnd( ID_NEXT ) );
	if( pButton && pButton->IsShow() && !pButton->IsDisabled() )
	{
		iwm_command( pButton, IOBN_BTNUP, 0 );
		return true;
	}

	pButton = dynamic_cast<ioButton*>( FindChildWnd( ID_CLOSE ) );
	if( pButton && pButton->IsShow() && !pButton->IsDisabled() )
	{
		iwm_command( pButton, IOBN_BTNUP, 0 );
		return true;
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
SoldierPracticeSelectBtn::SoldierPracticeSelectBtn()
{
	m_pCustomNormal = NULL;
	m_pCustomOver	= NULL;
	m_pNone			= NULL;
	m_pIconBack		= NULL;
	m_pSoldierIcon  = NULL;

	m_iClassType = -1;
	m_bHaveClass = false;

	m_nGradeType = 0;
}

SoldierPracticeSelectBtn::~SoldierPracticeSelectBtn()
{
	SAFEDELETE( m_pCustomNormal );
	SAFEDELETE( m_pCustomOver );
	SAFEDELETE( m_pNone );
	SAFEDELETE( m_pIconBack );
	SAFEDELETE( m_pSoldierIcon );
}

void SoldierPracticeSelectBtn::AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame )
{
	if( szType == "CustomNormal" )
	{
		SAFEDELETE( m_pCustomNormal );
		m_pCustomNormal = pFrame;
	}
	else if( szType == "CustomOver" )
	{
		SAFEDELETE( m_pCustomOver );
		m_pCustomOver = pFrame;
	}
	else
	{
		ioButton::AddRenderFrame( szType, pFrame );
	}
}

void SoldierPracticeSelectBtn::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "None" )
	{
		SAFEDELETE( m_pNone );
		m_pNone = pImage;
	}
	else if( szType == "IconBack" )
	{
		SAFEDELETE( m_pIconBack );
		m_pIconBack = pImage;
	}
	else 
	{
		ioButton::AddRenderImage( szType, pImage );
	}
}

void SoldierPracticeSelectBtn::SetClassType( int iClassType )
{
	m_iClassType = iClassType;
	m_nGradeType = 0;

	if( g_MyInfo.GetCharIndexByClassType( iClassType ) == 0 )
	{		
		m_bHaveClass = false;
	}
	else
	{
		m_bHaveClass = true;
	}

	SAFEDELETE( m_pSoldierIcon );

	if( m_iClassType != -1 )
	{
		SetActive();
		m_pSoldierIcon = g_MyInfo.GetMySoldierIcon( m_iClassType );
		m_nGradeType = g_UIImageRenderMgr.GetPowerUpGradeType( m_iClassType );
	}
	else
	{
		SetInActive();
	}
}

void SoldierPracticeSelectBtn::OnRender()
{
	ioButton::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();
	if( IsDisabled() )
	{
		if( m_pCustomNormal )
		{
			m_pCustomNormal->SetAlpha( (float)MAX_ALPHA_RATE * 0.60f );
			m_pCustomNormal->Render( iXPos, iYPos );
		}

		if( m_pNone )
		{
			m_pNone->SetScale( 0.60f );
			m_pNone->Render( iXPos + 29, iYPos + 29, UI_RENDER_MULTIPLY, TFO_BILINEAR );
		}
	}
	else
	{
		if( m_bOver || m_bClicked )
		{
			if( m_pCustomOver )
			{
				m_pCustomOver->Render( iXPos, iYPos );
			}
		}
		else if( m_pCustomNormal )
		{
			m_pCustomNormal->SetAlpha( (float)MAX_ALPHA_RATE * 0.60f );
			m_pCustomNormal->Render( iXPos, iYPos );
		}

		if( m_pIconBack )
		{
			m_pIconBack->SetScale( 0.60f );
			m_pIconBack->Render( iXPos + 29, iYPos + 29, UI_RENDER_MULTIPLY, TFO_BILINEAR );

			if( m_bHaveClass )
				g_UIImageRenderMgr.RenderGradeIconBack( m_nGradeType, iXPos + 29, iYPos + 29, 0.60f );
			else
				g_UIImageRenderMgr.RenderGradeIconBack( m_nGradeType, iXPos + 29, iYPos + 29, 0.60f, UI_RENDER_GRAY );
		}

		if( m_pSoldierIcon )
		{
			m_pSoldierIcon->SetScale( 0.60f );
			if( m_bHaveClass )
				m_pSoldierIcon->Render( iXPos + 29, iYPos + 29, UI_RENDER_NORMAL, TFO_BILINEAR );
			else
				m_pSoldierIcon->Render( iXPos + 29, iYPos + 29, UI_RENDER_GRAY, TFO_BILINEAR );
		}
	}
}

//////////////////////////////////////////////////////////////////////////
SoldierPracticeSelectWnd::SoldierPracticeSelectWnd()
{
	m_pPlayStage = NULL;
	m_iCurPage = 0;
	m_iMaxPage = 0;
}

SoldierPracticeSelectWnd::~SoldierPracticeSelectWnd()
{
	
}

void SoldierPracticeSelectWnd::SetPlayStage( ioPlayStage *pPlayStage )
{
	m_pPlayStage = pPlayStage;
}

void SoldierPracticeSelectWnd::SetBtnPos()
{
	int iRealXPos = 11;
	int iRealYPos = 72;
	int iMaxLoop  = MAX_HORZ * MAX_VERT;
	for(int i = 0;i < iMaxLoop;i++)
	{
		SoldierPracticeSelectBtn *pSoldier = dynamic_cast<SoldierPracticeSelectBtn*>(FindChildWnd( ID_SOLDIER_BTN1 + i ));
		if( pSoldier )
		{
			pSoldier->SetWndPos( iRealXPos, iRealYPos );
		}
		if( 0 == ( i + 1 ) % MAX_HORZ )
		{
			iRealXPos  = 11;
			iRealYPos  += 61;
		}
		else
		{
			iRealXPos += 61;
		}
	}
}

void SoldierPracticeSelectWnd::UpdateSetPage( int iCurPage )
{
	m_iCurPage = iCurPage;

	int iListSIze = m_CharList.size();
	int iStartPos = iCurPage * MAX_SOLDIER_BTN;
	for(int i = ID_SOLDIER_BTN1;i < ID_SOLDIER_BTN30 + 1;i++,iStartPos++)
	{
		SoldierPracticeSelectBtn *pBtn = dynamic_cast<SoldierPracticeSelectBtn*>( FindChildWnd( i ) );
		if( pBtn )
		{
			if( iStartPos < iListSIze )
			{
				pBtn->SetClassType( m_CharList[iStartPos] );
			}
			else
			{
				pBtn->SetClassType( -1 );
			}
		}
	}	
}

void SoldierPracticeSelectWnd::iwm_show()
{
	SetBtnPos();
	
	m_CharList.clear();    
	IntVec HaveList;
	int i = 0;
	int iMax = g_ClassPrice.MaxClassPrice();
	for (i = 0; i <  iMax; i++)
	{
		if( g_ClassPrice.IsSoldierTraingActive() && !g_ClassPrice.IsActiveClass( i ) )
		{
			// inactive 용병이지만 표시해야 하는 경우가 있다 ( 해외 가챠 캡슐용병등 )
			bool bContinue = true;
			int iClassType = g_ClassPrice.GetClassType( i );
			int iSize = m_vExtraClassVec.size();
			for (int j = 0; j < iSize ; j++)
			{
				int &rClassType = m_vExtraClassVec[j];
				if( iClassType == rClassType )
				{
					bContinue = false;
					break;
				}
			}

			if( bContinue )
				continue;
		}


		int iSetIdx = g_ClassPrice.ArrayToSetItemIdx( i );
		const ioSetItemInfo *pInfo = g_SetItemInfoMgr.GetSetInfoByIdx( iSetIdx );
		if( !pInfo ) continue;

		int iClassType = pInfo->GetSetCode() - SET_ITEM_CODE;
		if( g_MyInfo.GetCharIndexByClassType( iClassType ) != 0 )
		{
			HaveList.insert( HaveList.begin(), iClassType );
			continue;
		}

		if( pInfo->IsDummyInfo() )
			continue;

		/*std::string szFileFullName = pInfo->GetName().c_str();
		int iPos = szFileFullName.find( STR(1));
		if( iPos == -1  )*/
		m_CharList.push_back( iClassType );
	}

	for(i = 0;i < (int)HaveList.size();i++)
	{
		m_CharList.insert( m_CharList.begin(), HaveList[i] );
	}
	HaveList.clear();

	m_iMaxPage = max( 0, (int)m_CharList.size() - 1 ) / MAX_SOLDIER_BTN;
	UpdateSetPage( 0 );
}

void SoldierPracticeSelectWnd::iwm_hide()
{

}

void SoldierPracticeSelectWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_EXIT:
		if( cmd == IOBN_BTNUP )
			HideWnd();
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
	}

	if( COMPARE( dwID, ID_SOLDIER_BTN1, ID_SOLDIER_BTN30 + 1 ) )
	{
		if( cmd == IOBN_BTNUP )
		{
			ioHeadquartersMode *pHeadquartersMode = ToHeadquartersMode( m_pPlayStage->GetPlayMode() );
			if( pHeadquartersMode )
			{
				if( pHeadquartersMode->GetRecordPlayUserCnt() > 1 )
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
					return;
				}
			}

			SoldierPracticeSelectBtn *pSoldier = dynamic_cast<SoldierPracticeSelectBtn*>(pWnd);
			int iClassType = pSoldier->GetClassType();
			if( iClassType == -1 ) return;

			if( g_MyInfo.GetCharIndexByClassType( iClassType ) == 0 )
			{
				// 용병 구매 팝업
				SoldierPracticeAlarmWnd *pAlarmWnd = dynamic_cast<SoldierPracticeAlarmWnd*>( g_GUIMgr.FindWnd(SOLDIER_PRACTICE_ALARM_WND) );
				if( pAlarmWnd )
					pAlarmWnd->SetClassType( iClassType );
			}
			else
			{
				// 용병 선택 및 연습
				if( m_pPlayStage )
				{
					ioBaseChar *pOwner = m_pPlayStage->GetOwnerChar();
					if( !pOwner )	return;

					ioHeadquartersMode *pHeadquartersMode = ToHeadquartersMode( m_pPlayStage->GetPlayMode() );
					if( pHeadquartersMode->GetCharState() != ioHeadquartersMode::STATE_UNITE_CHAR )
					{
						SP2Packet kPacket( CTPK_HEADQUARTERS_STATE_CHANGE );
						kPacket << (int)ioHeadquartersMode::STATE_UNITE_CHAR;
						TCPNetwork::SendToServer( kPacket );
						TCPNetwork::MouseBusy( true );
					}

					int iCharArray = g_MyInfo.GetClassArray( iClassType );
					if( iCharArray != pOwner->GetSelectCharArray() )
					{
						pOwner->SetChangeWaitState( iCharArray, true );
					}

					PracticeHelpWnd *pPracticeHelp = dynamic_cast<PracticeHelpWnd *>( g_GUIMgr.FindWnd(PRACTICE_HELP_WND) );
					if( pPracticeHelp )
					{
						pPracticeHelp->SetPracticeHelp( iClassType );
					}

#if defined( USE_GA )
					char chLabel[32] = {0,};

					if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
						sprintf_e( chLabel, "%d", iClassType );
					else
						SafeSprintf( chLabel, sizeof(chLabel), "%1", iClassType );

					// PLAY_HQ_TRAINING
					g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
						, "Play"
						, "Trainig"
						, chLabel
						, 1
						, "%2FPLAY%2FHQ%2FTRAINING" );
#endif
				}
			}
			HideWnd();
		}
	}
}

void SoldierPracticeSelectWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;

	ioMovingWnd::OnProcess( fTimePerSec );
}

void SoldierPracticeSelectWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	ioComplexStringPrinter kPrinter;
	kPrinter.SetTextStyle( TS_NORMAL );
	kPrinter.SetBkColor( 0, 0, 0 );
	kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter.AddTextPiece( FONT_SIZE_13, STR(1) );
	kPrinter.PrintFullText( iXPos + 193, iYPos + 48, TAT_CENTER );
	kPrinter.ClearList();

	{
		kPrinter.SetTextStyle( TS_NORMAL );
		kPrinter.SetBkColor( 0, 0, 0 );
		kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
		kPrinter.AddTextPiece( FONT_SIZE_12, STR(2), m_iCurPage + 1 );

		kPrinter.SetTextStyle( TS_NORMAL );
		kPrinter.SetBkColor( 0, 0, 0 );
		kPrinter.SetTextColor( TCT_DEFAULT_GRAY );
		kPrinter.AddTextPiece( FONT_SIZE_12, STR(3), m_iMaxPage + 1 );
		kPrinter.PrintFullText( iXPos + 192, iYPos + 382, TAT_CENTER );
	}
}

bool SoldierPracticeSelectWnd::QuestGuide( int iClassType, float &rfXPos, float &rfYPos )
{
	int i = 0;
	for(i = ID_SOLDIER_BTN1;i < ID_SOLDIER_BTN30 + 1;i++)
	{
		SoldierPracticeSelectBtn *pSelectBtn = dynamic_cast<SoldierPracticeSelectBtn*>( FindChildWnd( i ) );
		if( pSelectBtn )
		{
			if( pSelectBtn->GetClassType() == iClassType )
			{
				rfXPos = pSelectBtn->GetDerivedPosX() + ( pSelectBtn->GetWidth() / 2 );
				rfYPos = pSelectBtn->GetDerivedPosY();
				return true;
			}
		}
	}	

	// 리스트에 없으면..
	int iClassPos = 0;
	for(iClassPos = 0;iClassPos < (int)m_CharList.size();iClassPos++)
	{
		if( m_CharList[iClassPos] == iClassType )
			break;
	}

	if( m_iCurPage < (iClassPos / MAX_SOLDIER_BTN) )
	{
		// Right Btn
		ioWnd *pRightBtn = FindChildWnd( ID_RIGHT_PAGE );
		if( pRightBtn )
		{
			rfXPos = pRightBtn->GetDerivedPosX() + ( pRightBtn->GetWidth() / 2 );
			rfYPos = pRightBtn->GetDerivedPosY();
			return true;
		}
	}
	else
	{
		// Left Btn
		ioWnd *pLeftBtn = FindChildWnd( ID_LEFT_PAGE );
		if( pLeftBtn )
		{
			rfXPos = pLeftBtn->GetDerivedPosX() + ( pLeftBtn->GetWidth() / 2 );
			rfYPos = pLeftBtn->GetDerivedPosY();
			return true;
		}
	}

	return false;
}

void SoldierPracticeSelectWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	m_vExtraClassVec.clear();
	m_vExtraClassVec.reserve(10);
	int iMax = xElement.GetIntAttribute_e( "MaxClass" );
	for (int i = 0; i < iMax ; i++)
	{
		char szName[MAX_PATH]="";
		StringCbPrintf_e( szName, sizeof( szName ), "ClassType%d", i+1 );
		int iClassType = xElement.GetIntAttribute( szName );
		if( iClassType  <= 0 )
			continue;
		m_vExtraClassVec.push_back( iClassType );
	}
}
//////////////////////////////////////////////////////////////////////////
SoldierPracticeAlarmWnd::SoldierPracticeAlarmWnd()
{
	m_pSoldierIcon = NULL;

	m_iClassType = 0;
}

SoldierPracticeAlarmWnd::~SoldierPracticeAlarmWnd()
{
	SAFEDELETE( m_pSoldierIcon );
}

void SoldierPracticeAlarmWnd::SetClassType( int iClassType )
{
	m_iClassType = iClassType;

	SAFEDELETE( m_pSoldierIcon );
	m_pSoldierIcon = g_MyInfo.GetMySoldierIcon( iClassType );

	char szTitle[MAX_PATH] = "";
	SafeSprintf( szTitle, sizeof( szTitle ), STR(1), g_MyInfo.GetClassName( m_iClassType, g_MyInfo.GetCharPowerUpGradeByClassType( m_iClassType ) ) );
	SetTitleText( szTitle );

	ShowWnd();
}

void SoldierPracticeAlarmWnd::iwm_hide()
{
	if( !g_GUIMgr.IsShow( NEW_SHOP_WND ) )
	{
		g_GUIMgr.ShowWnd( SOLDIER_PRACTICE_SELECT_WND );
	}
}

void SoldierPracticeAlarmWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_EXIT:
	case ID_CLOSE:
		if( cmd == IOBN_BTNUP )
			HideWnd();
		break;
	case ID_SHOP:
		if( cmd == IOBN_BTNUP )
		{	
			g_GUIMgr.ShowWnd( NEW_SHOP_WND );
			HideWnd();
		}
		break;
	}
}

void SoldierPracticeAlarmWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	if( m_pSoldierIcon )
		m_pSoldierIcon->Render( iXPos + 151, iYPos + 98 );

	ioComplexStringPrinter kPrinter;
	kPrinter.SetTextStyle( TS_NORMAL );
	kPrinter.SetBkColor( 0, 0, 0 );
	kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter.AddTextPiece( FONT_SIZE_13, STR(1), g_MyInfo.GetClassName( m_iClassType, g_MyInfo.GetCharPowerUpGradeByClassType( m_iClassType ) ) );
	kPrinter.PrintFullText( iXPos + 151, iYPos + 144, TAT_CENTER );
	kPrinter.ClearList();

	kPrinter.SetTextStyle( TS_NORMAL );
	kPrinter.SetBkColor( 0, 0, 0 );
	kPrinter.SetTextColor( TCT_DEFAULT_GRAY );
	kPrinter.AddTextPiece( FONT_SIZE_13, STR(2) );
	kPrinter.PrintFullText( iXPos + 151, iYPos + 164, TAT_CENTER );
	kPrinter.ClearList();

	kPrinter.SetTextStyle( TS_NORMAL );
	kPrinter.SetBkColor( 0, 0, 0 );
	kPrinter.SetTextColor( TCT_DEFAULT_GRAY );
	kPrinter.AddTextPiece( FONT_SIZE_13, STR(3) );
	kPrinter.PrintFullText( iXPos + 151, iYPos + 184, TAT_CENTER );
	kPrinter.ClearList();
}