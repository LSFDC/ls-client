#include "StdAfx.h"

#include "../io3DEngine/ioWnd.h"
#include "../io3DEngine/ioFontManager.h"
#include "../io3DEngine/ioUIRenderer.h"
#include "../io3DEngine/ioMath.h"
#include "../io3DEngine/ioUIImageSetManager.h"
#include "../io3DEngine/ioUIRenderImage.h"
#include "../io3DEngine/ioUIRenderFrame.h"
#include "../io3DEngine/ioOrientBox.h"
#include "../io3DEngine/ioSoundManager.h"
#include "../io3DEngine/HelpFunc.h"
#include "../io3DEngine/ioXMLElement.h"
#include "../io3DEngine/iostringmanager.h"
#include "../io3dengine/safesprintf.h"

#include "../Setting.h"
#include "../ioMyInfo.h"
#include "../ioApplication.h"
#include "../ioClassPrice.h"
#include "../ioMyLevelMgr.h"
#include "../TextColorTable.h"
#include "../NetworkWrappingFunc.h"

#include "../ioPlayStage.h"
#include "../ioPresentMgr.h"
#include "../ioItemMaker.h"
#include "../ioEtcItem.h"
#include "../ioEtcItemManager.h"
#include "../ioSetItemInfo.h"
#include "../ioSetItemInfoManager.h"
#include "../ioDecorationPrice.h"
#include "../ioUserExtraItem.h"
#include "../ioExtraItemInfoManager.h"
#include "../EtcHelpFunc.h"
#include "../ioQuestManager.h"
#include "../ioAlchemicMgr.h"

#include "../WndID.h"
#include "../ioMedalItemInfoManager.h"
#include "../ioExtendSoundManager.h"

#include "ioSP2GUIManager.h"

#include "HelpWnd.h"
#include "QuestWnd.h"
#include <strsafe.h>
#include "ioUITitle.h"

QuestListBtn::QuestListBtn() : m_pQuestParent( NULL )
{
	m_pRewardFrm	 = NULL;
	m_pEmptyQuest    = NULL;
	m_pQuestIcon	 = NULL;
	m_pIconBack		 = NULL;
	m_pEventMark	 = NULL;
	m_pCostumeMark = NULL;

	m_bIsCostume = false;
}

QuestListBtn::~QuestListBtn()
{
	SAFEDELETE( m_pRewardFrm );
	SAFEDELETE( m_pIconBack );
	SAFEDELETE( m_pEmptyQuest );
	SAFEDELETE( m_pEventMark );
	SAFEDELETE( m_pCostumeMark );
}

void QuestListBtn::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "IconBack" )
	{
		SAFEDELETE( m_pIconBack );
		m_pIconBack = pImage;
	}
	else if( szType == "EmptyQuest" )
	{
		SAFEDELETE( m_pEmptyQuest );
		m_pEmptyQuest = pImage;
	}
	else if( szType == "EventMark" )
	{
		SAFEDELETE( m_pEventMark );
		m_pEventMark = pImage;
	}
	else if( szType == "costume_image" )
	{
		SAFEDELETE( m_pCostumeMark );
		m_pCostumeMark = pImage;
	}
	else
	{
		ioButton::AddRenderImage( szType, pImage );	
	}	
}

void QuestListBtn::AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame )
{
	if( pFrame )
	{
		if( szType == "RewardFrm" )
		{
			SAFEDELETE( m_pRewardFrm );
			m_pRewardFrm = pFrame;
		}
		else
		{	
			ioButton::AddRenderFrame( szType, pFrame );
		}
	}
}

void QuestListBtn::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_QUEST_CONTENT_BTN:
	case ID_QUEST_RESULT_BTN:
		if( cmd == IOBN_BTNUP )
		{
			if( GetParent() )
				GetParent()->iwm_command( this, cmd, dwID );
		}
		break;
	case ID_QUEST_INSERT_ALARM:
		if( cmd == IOBN_BTNUP )
		{
			ioQuest *pQuest = g_MyInfo.GetQuest();
			if( pQuest && m_pQuestParent )
			{
				QuestData &rkData = pQuest->GetQuestData( m_pQuestParent->GetMainIndex(), m_pQuestParent->GetSubIndex() );
				if( rkData.GetMainIndex() > 0 )
				{
					if( rkData.GetState() == QS_PROGRESS && !rkData.IsAlarm() )
					{
						rkData.SetAlarm( true );
						SP2Packet kPacket( CTPK_QUEST_ALARM );
						kPacket << rkData.GetMainIndex() << rkData.GetSubIndex() << true;
						TCPNetwork::SendToServer( kPacket );
						g_QuestMgr.QuestInfoChange();
					}

					if( rkData.IsAlarm() )
					{
						HideChildWnd( ID_QUEST_INSERT_ALARM );
						ShowChildWnd( ID_QUEST_REMOVE_ALARM );
					}
				}
			}
		}
		break;
	case ID_QUEST_REMOVE_ALARM:
		if( cmd == IOBN_BTNUP )
		{
			ioQuest *pQuest = g_MyInfo.GetQuest();
			if( pQuest && m_pQuestParent )
			{
				QuestData &rkData = pQuest->GetQuestData( m_pQuestParent->GetMainIndex(), m_pQuestParent->GetSubIndex() );
				if( rkData.GetMainIndex() > 0 )
				{
					if( rkData.GetState() == QS_PROGRESS && rkData.IsAlarm() )
					{
						rkData.SetAlarm( false );
						SP2Packet kPacket( CTPK_QUEST_ALARM );
						kPacket << rkData.GetMainIndex() << rkData.GetSubIndex() << false;
						TCPNetwork::SendToServer( kPacket );
						g_QuestMgr.QuestInfoChange();
					}

					if( rkData.IsAlarm() )
					{
						ShowChildWnd( ID_QUEST_INSERT_ALARM );
						HideChildWnd( ID_QUEST_REMOVE_ALARM );
					}
				}
			}
		}
		break;
	}
}


void QuestListBtn::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) 
		return;
}

void QuestListBtn::OnDrawPushed( int iXPos, int iYPos )
{
	ioButton::OnDrawOvered( iXPos, iYPos );
}

void QuestListBtn::OnRender()
{
	ioButton::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();
	if( HasWndStyle( IWS_INACTIVE ) || !m_pQuestParent )
	{
		if( m_pEmptyQuest )
		{
			m_pEmptyQuest->SetScale( FLOAT1, FLOAT1 );
			m_pEmptyQuest->Render( iXPos + 52, iYPos + 49, UI_RENDER_MULTIPLY );
		}

		g_FontMgr.SetTextStyle( TS_NORMAL );
		g_FontMgr.SetAlignType( TAT_CENTER );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextColor( 136, 122, 84 );
		g_FontMgr.PrintText( iXPos + 52, iYPos + 104, FONT_SIZE_13, STR(1) );
		g_FontMgr.PrintText( iXPos + 52, iYPos + 124, FONT_SIZE_13, "-" );
	}
	else
	{
		if( m_pIconBack )
		{	
			m_pIconBack->SetScale( FLOAT1, FLOAT1 );
			m_pIconBack->Render( iXPos + 52, iYPos + 49, UI_RENDER_MULTIPLY );
		}

		g_FontMgr.SetTextStyle( TS_NORMAL );
		g_FontMgr.SetAlignType( TAT_CENTER );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		if( m_pQuestParent->GetPerformType() == QP_EVENT )
			g_FontMgr.SetTextColor( TCT_DEFAULT_RED );
		else
			g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
		g_FontMgr.PrintTextWidthCut( iXPos + 52, iYPos + 98, FONT_SIZE_13, FLOAT90, m_pQuestParent->GetTitle().c_str() );
		
		switch( m_QuestData.GetState() )
		{
		case QS_PROGRESS:
			{
				m_pQuestParent->RenderQuestIcon( iXPos + 52, iYPos + 49, FLOAT1 );

				// UI코스튬 관련 (코스튬 마크)
				if ( m_bIsCostume && m_pCostumeMark )
				{
					m_pCostumeMark->SetScale( 1.00f );
					m_pCostumeMark->Render( iXPos + 52, iYPos + 49 );
				}

				if( m_pQuestParent->GetPerformType() == QP_EVENT )
				{
					if( m_pEventMark )
						m_pEventMark->Render( iXPos, iYPos );
				}

				if( m_pRewardFrm )
					m_pRewardFrm->Render( iXPos, iYPos, UI_RENDER_MULTIPLY );

				OnRenderProgress( iXPos + 52, iYPos + 124 );

				g_FontMgr.SetTextColor( TCT_DEFAULT_ORANGE );
				if( m_pQuestParent->GetProgressResult().IsEmpty() )
					g_FontMgr.PrintText( iXPos + 52, iYPos + 157, FONT_SIZE_13, STR(3) );
				else
					g_FontMgr.PrintText( iXPos + 52, iYPos + 157, FONT_SIZE_13, m_pQuestParent->GetProgressResult().c_str() );
			}
			break;
		case QS_ATTAIN:
			{
				if( m_pQuestIcon )
				{
					m_pQuestIcon->SetScale( FLOAT1, FLOAT1 );
					m_pQuestIcon->Render( iXPos + 52, iYPos + 49 );
				}

				// UI코스튬 관련 (코스튬 마크)
				if ( m_bIsCostume && m_pCostumeMark )
				{
					m_pCostumeMark->SetScale( 1.00f, 1.00f );
					m_pCostumeMark->Render( iXPos + 52, iYPos + 49 );
				}

				if( m_pQuestParent->GetPerformType() == QP_EVENT )
				{
					if( m_pEventMark )
						m_pEventMark->Render( iXPos, iYPos );
				}				

				OnRenderProgress( iXPos + 52, iYPos + 124 );
			}
			break;
		case QS_COMPLETE:
			{
				if( m_pQuestIcon )
				{
					m_pQuestIcon->SetScale( FLOAT1, FLOAT1 );
					m_pQuestIcon->Render( iXPos + 52, iYPos + 49 );
				}

				// UI코스튬 관련 (코스튬 마크)
				if ( m_bIsCostume && m_pCostumeMark )
				{
					m_pCostumeMark->SetScale( 1.00f, 1.00f );
					m_pCostumeMark->Render( iXPos + 52, iYPos + 49 );
				}

				if( m_pQuestParent->GetPerformType() == QP_EVENT )
				{
					if( m_pEventMark )
						m_pEventMark->Render( iXPos, iYPos );
				}

				OnRenderProgress( iXPos + 52, iYPos + 124 );
			}
			break;
		}
	}
}

void QuestListBtn::OnRenderProgress( int iXPos, int iYPos )
{
	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetBkColor( 0, 0, 0 );

	if( m_pQuestParent->GetPerformType() == QP_EVENT )
		g_FontMgr.SetTextColor( 186, 79, 65 );
	else
		g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );

	float fWidth = FLOAT90;
	char szTitle[MAX_PATH] = "";
	sprintf( szTitle, m_pQuestParent->GetProgressText().c_str() );
	if( g_FontMgr.GetTextWidth( szTitle, TS_NORMAL, FONT_SIZE_11 ) <= fWidth )
	{
		g_FontMgr.PrintText( iXPos, iYPos, FONT_SIZE_11, szTitle );			
	}
	else // 2줄까지만 내려쓰기
	{
		char szDst[MAX_PATH] = "";
		memset( szDst, 0, sizeof( szDst ) );
		int iLen = Help::StringCutFun( FONT_SIZE_11, fWidth, TS_NORMAL, szDst, sizeof(szDst), szTitle );
		g_FontMgr.PrintText( iXPos, iYPos - 7, FONT_SIZE_11, szDst );
		if( iLen < (int)strlen( szTitle) )
			g_FontMgr.PrintTextWidthCut( iXPos, iYPos + 7, FONT_SIZE_11, fWidth, &szTitle[iLen] );
	}
}

void QuestListBtn::OnRenderReward(int iXPos, int iYPos )
{
	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );

	char szPresentName[MAX_PATH] = "";
	DWORD dwPresentID = m_pQuestParent->GetRewardPresentIndex( 0 );
	sprintf( szPresentName, g_QuestMgr.GetPresentName( dwPresentID ).c_str() );
	if( m_pQuestParent->GetMaxRewardPresent() <= 1 )
	{
		g_FontMgr.SetAlignType( TAT_CENTER );
		g_FontMgr.PrintTextWidthCut( iXPos, iYPos, FONT_SIZE_12, FLOAT90, szPresentName );
	}
	else
	{
		char szRestName[MAX_PATH] = "";
		SafeSprintf( szRestName, sizeof( szRestName ), STR(1), m_pQuestParent->GetMaxRewardPresent() - 1 );
		int iPresentSize = min( 33.0f, g_FontMgr.GetTextWidth( szPresentName, TS_NORMAL, FONT_SIZE_12 ) );
		int iRestSize = g_FontMgr.GetTextWidth( szRestName, TS_NORMAL, FONT_SIZE_12 );

		iXPos -= ( iPresentSize + iRestSize ) / 2;
		g_FontMgr.SetAlignType( TAT_LEFT );
		g_FontMgr.PrintTextWidthCut( iXPos, iYPos, FONT_SIZE_12, 33.0f, szPresentName );
		g_FontMgr.PrintText( iXPos + iPresentSize, iYPos, FONT_SIZE_12, szRestName );
	}		
}

void QuestListBtn::SetQuest( QuestData &rkQuestData )
{
	m_QuestData = rkQuestData;
	if( m_QuestData.GetMainIndex() == 0 )
	{
		SetInActive();
		HideChildWnd( ID_QUEST_CONTENT_BTN );
		HideChildWnd( ID_QUEST_RESULT_BTN );
		HideChildWnd( ID_QUEST_INSERT_ALARM );
		HideChildWnd( ID_QUEST_REMOVE_ALARM );
		m_pQuestParent = NULL;
	}
	else
	{
		SetActive();
		m_pQuestParent = g_QuestMgr.GetQuest( m_QuestData.GetMainIndex(), m_QuestData.GetSubIndex() );
		if( m_pQuestParent )
		{
			m_pQuestIcon = m_pQuestParent->GetQuestIcon();
			m_bIsCostume = m_pQuestParent->IsCostume();
		}

		if( m_QuestData.GetState() == QS_PROGRESS )
		{
			HideChildWnd( ID_QUEST_CONTENT_BTN );
			HideChildWnd( ID_QUEST_RESULT_BTN );
			if( m_QuestData.IsAlarm() )
			{
				HideChildWnd( ID_QUEST_INSERT_ALARM );
				ShowChildWnd( ID_QUEST_REMOVE_ALARM );
			}
			else
			{
				ShowChildWnd( ID_QUEST_INSERT_ALARM );
				HideChildWnd( ID_QUEST_REMOVE_ALARM );
			}
		}
		else
		{
			ShowChildWnd( ID_QUEST_CONTENT_BTN );
			ShowChildWnd( ID_QUEST_RESULT_BTN );	
			HideChildWnd( ID_QUEST_INSERT_ALARM );
			HideChildWnd( ID_QUEST_REMOVE_ALARM );
		}	
	}
}
//////////////////////////////////////////////////////////////////////////
QuestCompleteListWnd::QuestCompleteListWnd()
{
	m_iCurPage = 0;
	m_iMaxPage = 0;
}

QuestCompleteListWnd::~QuestCompleteListWnd()
{

}

void QuestCompleteListWnd::iwm_show()
{
	UpdatePage( 0 );
}

void QuestCompleteListWnd::iwm_hide()
{

}

void QuestCompleteListWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();	
	switch( dwID )
	{
	case ID_LEFT_BTN:
		if( cmd == IOBN_BTNUP )
		{
			UpdatePage( m_iCurPage - 1 );
		}
		break;
	case ID_RIGHT_BTN:
		if( cmd == IOBN_BTNUP )
		{
			UpdatePage( m_iCurPage + 1 );
		}
		break;
	case ID_COMPLETE_LIST1:
	case ID_COMPLETE_LIST2:
	case ID_COMPLETE_LIST3:
	case ID_COMPLETE_LIST4:
	case ID_COMPLETE_LIST5:
	case ID_COMPLETE_LIST6:
	case ID_COMPLETE_LIST7:
	case ID_COMPLETE_LIST8:
		if( cmd == IOBN_BTNUP )
		{			
			QuestListBtn *pQuestListBtn = dynamic_cast<QuestListBtn*>( pWnd );
			if( pQuestListBtn && pQuestListBtn->GetQuestParent() )
			{
				if( param == QuestListBtn::ID_QUEST_RESULT_BTN )
				{
					QuestCompleteInfoWnd *pQuestInfo = dynamic_cast<QuestCompleteInfoWnd*>(g_GUIMgr.FindWnd( QUEST_COMPLETE_INFO_WND ));
					if( pQuestInfo )
						pQuestInfo->SetQuest( pQuestListBtn->GetQuestParent() );
				}
				else
				{
					QuestProgressInfoMainWnd *pQuestInfo = dynamic_cast<QuestProgressInfoMainWnd*>(g_GUIMgr.FindWnd( QUEST_PROGRESS_INFO_WND ));
					if( pQuestInfo )
						pQuestInfo->SetQuest( pQuestListBtn->GetQuestParent() );
				}
			}
		}
		break;
	}
}

void QuestCompleteListWnd::OnRender()
{
	ioWnd::OnRender();
	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	ioComplexStringPrinter kPrinter;
	kPrinter.SetTextStyle( TS_NORMAL );
	kPrinter.SetBkColor( 0, 0, 0 );
	kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter.AddTextPiece( FONT_SIZE_12, STR(1), m_iCurPage + 1 );

	kPrinter.SetTextStyle( TS_NORMAL );
	kPrinter.SetBkColor( 0, 0, 0 );
	kPrinter.SetTextColor( TCT_DEFAULT_GRAY );
	kPrinter.AddTextPiece( FONT_SIZE_12, STR(2), m_iMaxPage + 1 );
	kPrinter.PrintFullText( iXPos + 212, iYPos + 377, TAT_CENTER );
}

void QuestCompleteListWnd::ChangeData()
{
	UpdatePage( m_iCurPage );
}

void QuestCompleteListWnd::UpdatePage( int iCurPage )
{
	ioQuest *pQuest = g_MyInfo.GetQuest();
	if( !pQuest ) return;

	int iCompleteCount = pQuest->GetMaxQuest( QS_ATTAIN ) + pQuest->GetMaxQuest( QS_COMPLETE );
	m_iMaxPage = max( 0, iCompleteCount - 1 ) / PAGE_QUEST_COUNT;
	m_iCurPage = max( 0, iCurPage );
	m_iCurPage = min( m_iCurPage, m_iMaxPage );

	int iStartArray = m_iCurPage * PAGE_QUEST_COUNT;
	for(int i = 0;i < PAGE_QUEST_COUNT;i++,iStartArray++)
	{
		QuestListBtn *pQuestListBtn = dynamic_cast<QuestListBtn*>( FindChildWnd( ID_COMPLETE_LIST1 + i ) );
		if( pQuestListBtn )
		{
			pQuestListBtn->SetQuest( pQuest->GetAttainNCompleteToArray( iStartArray ) );
		}
	}
}
//////////////////////////////////////////////////////////////////////////
QuestProgressListWnd::QuestProgressListWnd()
{
	m_iCurPage = 0;
	m_iMaxPage = 0;
}

QuestProgressListWnd::~QuestProgressListWnd()
{

}

void QuestProgressListWnd::iwm_show()
{
	UpdatePage( 0 );
}

void QuestProgressListWnd::iwm_hide()
{

}

void QuestProgressListWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();	
	switch( dwID )
	{
	case ID_LEFT_BTN:
		if( cmd == IOBN_BTNUP )
		{
			UpdatePage( m_iCurPage - 1 );
		}
		break;
	case ID_RIGHT_BTN:
		if( cmd == IOBN_BTNUP )
		{
			UpdatePage( m_iCurPage + 1 );
		}
		break;
	case ID_PROGRESS_LIST1:
	case ID_PROGRESS_LIST2:
	case ID_PROGRESS_LIST3:
	case ID_PROGRESS_LIST4:
	case ID_PROGRESS_LIST5:
	case ID_PROGRESS_LIST6:
	case ID_PROGRESS_LIST7:
	case ID_PROGRESS_LIST8:
		if( cmd == IOBN_BTNUP )
		{
			QuestListBtn *pQuestListBtn = dynamic_cast<QuestListBtn*>( pWnd );
			if( pQuestListBtn && pQuestListBtn->GetQuestParent() )
			{
				QuestProgressInfoMainWnd *pQuestInfo = dynamic_cast<QuestProgressInfoMainWnd*>(g_GUIMgr.FindWnd( QUEST_PROGRESS_INFO_WND ));
				if( pQuestInfo )
					pQuestInfo->SetQuest( pQuestListBtn->GetQuestParent() );
			}
		}
		break;
	}
}

void QuestProgressListWnd::OnRender()
{
	ioWnd::OnRender();
	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	ioComplexStringPrinter kPrinter;
	kPrinter.SetTextStyle( TS_NORMAL );
	kPrinter.SetBkColor( 0, 0, 0 );
	kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter.AddTextPiece( FONT_SIZE_12, STR(1), m_iCurPage + 1 );

	kPrinter.SetTextStyle( TS_NORMAL );
	kPrinter.SetBkColor( 0, 0, 0 );
	kPrinter.SetTextColor( TCT_DEFAULT_GRAY );
	kPrinter.AddTextPiece( FONT_SIZE_12, STR(2), m_iMaxPage + 1 );
	kPrinter.PrintFullText( iXPos + 212, iYPos + 377, TAT_CENTER );
}

void QuestProgressListWnd::ChangeData()
{
	UpdatePage( m_iCurPage );
}

void QuestProgressListWnd::UpdatePage( int iCurPage )
{
	ioQuest *pQuest = g_MyInfo.GetQuest();
	if( !pQuest ) return;

	m_iMaxPage = max( 0, pQuest->GetMaxQuest( QS_PROGRESS ) - 1 ) / PAGE_QUEST_COUNT;
	m_iCurPage = max( 0, iCurPage );
	m_iCurPage = min( m_iCurPage, m_iMaxPage );

	int iStartArray = m_iCurPage * PAGE_QUEST_COUNT;
	for(int i = 0;i < PAGE_QUEST_COUNT;i++,iStartArray++)
	{
		QuestListBtn *pQuestListBtn = dynamic_cast<QuestListBtn*>( FindChildWnd( ID_PROGRESS_LIST1 + i ) );
		if( pQuestListBtn )
		{
			pQuestListBtn->SetQuest( pQuest->GetProcessToArray( iStartArray ) );
		}
	}
}
//////////////////////////////////////////////////////////////////////////
QuestMainWnd::QuestMainWnd()
{
	m_dwTabID = ID_PROGRESS_BTN;
}

QuestMainWnd::~QuestMainWnd()
{
}

void QuestMainWnd::iwm_create()
{
	g_GUIMgr.AddWnd( "XML/questprogresslistwnd.xml", this );
	g_GUIMgr.AddWnd( "XML/questcompletelistwnd.xml", this );
}

void QuestMainWnd::iwm_show()
{	
	ChangeRadioWnd( ID_PROGRESS_BTN );
	ChangeTitle();

	g_QuestMgr.IsQuestInfoChange();

#if defined( USE_GA )
	// WND_QUEST
	g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FQUEST" );
#endif

}

void QuestMainWnd::iwm_hide()
{	
}

void QuestMainWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
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
	case ID_PROGRESS_BTN:
	case ID_COMPLETE_BTN:
		if( cmd == IOBN_BTNDOWN )
		{			
			ChangeRadioWnd( dwID );
		}
		break;
	}
}

void QuestMainWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() )
		return;

	ioMovingWnd::OnProcess( fTimePerSec );

	if( g_QuestMgr.IsQuestInfoChange() )
	{
		ChangeData();
		ChangeTitle();
	}
}

void QuestMainWnd::OnRender()
{
	ioWnd::OnRender();
}

void QuestMainWnd::ChangeRadioWnd( DWORD dwID )
{
	m_dwTabID = dwID;
	CheckRadioButton( ID_PROGRESS_BTN, ID_COMPLETE_BTN, dwID );

	HideChildWnd( ID_PROGRESS_WND );
	HideChildWnd( ID_COMPLETE_WND );

	switch( m_dwTabID )
	{
	case ID_PROGRESS_BTN:
		ShowChildWnd( ID_PROGRESS_WND );
		break;
	case ID_COMPLETE_BTN:
		ShowChildWnd( ID_COMPLETE_WND );
		break;
	}
}

void QuestMainWnd::ChangeData()
{
	switch( m_dwTabID )
	{
	case ID_PROGRESS_BTN:
		{
			QuestProgressListWnd *pListWnd = dynamic_cast<QuestProgressListWnd*>(FindChildWnd( ID_PROGRESS_WND ));
			if( pListWnd )
				pListWnd->ChangeData();
		}
		break;
	case ID_COMPLETE_BTN:
		{
			QuestCompleteListWnd *pListWnd = dynamic_cast<QuestCompleteListWnd*>(FindChildWnd( ID_COMPLETE_WND ));
			if( pListWnd )
				pListWnd->ChangeData();
		}
		break;
	}
}

void QuestMainWnd::ChangeTitle()
{
	ioQuest *pQuest = g_MyInfo.GetQuest();
	if( !pQuest ) return;

	// 윈도우 타이틀
	char szTitle[MAX_PATH] = "";
	SafeSprintf( szTitle, sizeof( szTitle ), STR(1), pQuest->GetMaxQuest( QS_PROGRESS ) );
	SetTitleText( szTitle );

	// 진행중 탭 버튼 타이틀
	ioWnd *pProgressBtn = FindChildWnd( ID_PROGRESS_BTN );
	if( pProgressBtn )
	{
		SafeSprintf( szTitle, sizeof( szTitle ), STR(2), pQuest->GetMaxQuest( QS_PROGRESS ) );
		pProgressBtn->SetTitleText( szTitle );
	}

	// 완료 탭 버튼 타이틀
	ioWnd *pCompleteBtn = FindChildWnd( ID_COMPLETE_BTN );
	if( pCompleteBtn )
	{
		SafeSprintf( szTitle, sizeof( szTitle ), STR(3), pQuest->GetMaxQuest( QS_ATTAIN ) + pQuest->GetMaxQuest( QS_COMPLETE ) );
		pCompleteBtn->SetTitleText( szTitle );
	}
}
//////////////////////////////////////////////////////////////////////////
LobbyQuestAlarmBtn::LobbyQuestAlarmBtn() : m_pQuestParent( NULL )
{
	m_pQuestIcon   = NULL;
	m_pIconBack    = NULL;
	m_pIconEmpty   = NULL;
	m_pEventMark   = NULL;
	m_pCostumeMark = NULL;

	m_bIsCostume = false;
}

LobbyQuestAlarmBtn::~LobbyQuestAlarmBtn()
{
	SAFEDELETE( m_pIconBack );
	SAFEDELETE( m_pIconEmpty );
	SAFEDELETE( m_pEventMark );
	SAFEDELETE( m_pCostumeMark );
}

void LobbyQuestAlarmBtn::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "IconBack" )
	{
		SAFEDELETE( m_pIconBack );
		m_pIconBack = pImage;
	}
	else if( szType == "IconEmpty" )
	{
		SAFEDELETE( m_pIconEmpty );
		m_pIconEmpty = pImage;
	}
	else if( szType == "EventMark" )
	{
		SAFEDELETE( m_pEventMark );
		m_pEventMark = pImage;
	}
	else if( szType == "costume_image" )
	{
		SAFEDELETE( m_pCostumeMark );
		m_pCostumeMark = pImage;
	}
	else
	{
		ioButton::AddRenderImage( szType, pImage );	
	}	
}

void LobbyQuestAlarmBtn::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_DELETE_BTN:
		if( cmd == IOBN_BTNUP )
		{
			// 알림에서 삭제
			ioQuest *pQuest = g_MyInfo.GetQuest();
			if( pQuest )
			{
				QuestData &rkData = pQuest->GetQuestData( m_QuestData.GetMainIndex(), m_QuestData.GetSubIndex() );
				if( rkData.GetMainIndex() > 0 )
				{
					rkData.SetAlarm( false );
					SP2Packet kPacket( CTPK_QUEST_ALARM );
					kPacket << rkData.GetMainIndex() << rkData.GetSubIndex() << false;
					TCPNetwork::SendToServer( kPacket );
					g_QuestMgr.QuestInfoChange();
				}
			}
		}
		break;
	}
}

void LobbyQuestAlarmBtn::OnRender()
{
	ioButton::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	if( !m_pQuestParent )
	{
		if( m_pIconEmpty )
		{
			m_pIconEmpty->SetScale( FLOAT05, FLOAT05 );
			m_pIconEmpty->Render( iXPos + 25, iYPos + 27, UI_RENDER_MULTIPLY, TFO_BILINEAR );
		}

		g_FontMgr.SetTextStyle( TS_NORMAL );
		g_FontMgr.SetAlignType( TAT_LEFT );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY2 );
		g_FontMgr.PrintText( iXPos + 48, iYPos + 14, FONT_SIZE_11,STR(1) );
		g_FontMgr.PrintText( iXPos + 48, iYPos + 28, FONT_SIZE_11,STR(2) );
		return;
	}

	if( m_pIconBack )
	{
		m_pIconBack->SetScale( FLOAT05, FLOAT05 );
		m_pIconBack->Render( iXPos + 25, iYPos + 27, UI_RENDER_MULTIPLY, TFO_BILINEAR );
	}

	switch( m_QuestData.GetState() )
	{
	case QS_PROGRESS:
		{
			m_pQuestParent->RenderQuestIcon( iXPos + 25, iYPos + 27, FLOAT05 );

			// UI코스튬 관련 (코스튬 마크)
			if( m_bIsCostume && m_pCostumeMark )
			{
				m_pCostumeMark->SetScale( 0.50f );
				m_pCostumeMark->Render( iXPos + 25, iYPos + 27, UI_RENDER_NORMAL, TFO_BILINEAR );
			}

			OnRenderQuestTitle( iXPos + 48, iYPos + 8, m_pQuestParent->GetProgressResultOnlyNumber().c_str() );
		}
		break;
	case QS_ATTAIN:
		{
			if( m_pQuestIcon )
			{
				m_pQuestIcon->SetScale( FLOAT05, FLOAT05 );
				m_pQuestIcon->Render( iXPos + 25, iYPos + 27, UI_RENDER_NORMAL, TFO_BILINEAR );
			}

			// UI코스튬 관련 (코스튬 마크)
			if( m_bIsCostume && m_pCostumeMark )
			{
				m_pCostumeMark->SetScale( 0.50f, 0.50f );
				m_pCostumeMark->Render( iXPos + 25, iYPos + 27, UI_RENDER_NORMAL, TFO_BILINEAR );
			}
			OnRenderQuestTitle( iXPos + 48, iYPos + 8, STR(3) );
		}
		break;
	case QS_COMPLETE:
		{
			if( m_pQuestIcon )
			{
				m_pQuestIcon->SetScale( FLOAT05, FLOAT05 );
				m_pQuestIcon->Render( iXPos + 25, iYPos + 27, UI_RENDER_NORMAL, TFO_BILINEAR );
			}

			// UI코스튬 관련 (코스튬 마크)
			if( m_bIsCostume && m_pCostumeMark )
			{
				m_pCostumeMark->SetScale( 0.50f, 0.50f );
				m_pCostumeMark->Render( iXPos + 25, iYPos + 27, UI_RENDER_NORMAL, TFO_BILINEAR );
			}
			OnRenderQuestTitle( iXPos + 48, iYPos + 8, STR(4) );
		}
		break;
	}

	if( m_pQuestParent->GetPerformType() == QP_EVENT )
	{
		if( m_pEventMark )
		{
			m_pEventMark->SetScale( 0.60f);
			m_pEventMark->Render( iXPos - 11, iYPos - 11, UI_RENDER_NORMAL, TFO_BILINEAR );
		}
	}
}

void LobbyQuestAlarmBtn::OnRenderQuestTitle( int iXPos, int iYPos, const char *szResult )
{	
	DWORD dwTitleColor = TCT_DEFAULT_DARKGRAY;
	DWORD dwProgressColor = TCT_DEFAULT_GRAY;
	if( m_pQuestParent->GetPerformType() == QP_EVENT )
	{
		dwTitleColor = TCT_DEFAULT_RED;
		dwProgressColor = 0xBA4F41;
	}

	// 한줄 & 두줄 체크
	float fLimitWidth = 114.0f;
	char szBuffer[MAX_PATH] = "";
	if( strlen( szResult ) == 0 )
		sprintf_e( szBuffer, "%s", m_pQuestParent->GetProgressText().c_str() );
	else
		sprintf_e( szBuffer, "%s %s", m_pQuestParent->GetProgressText().c_str(), szResult );

	if( g_FontMgr.GetTextWidth( szBuffer, TS_NORMAL, FONT_SIZE_11 ) <= fLimitWidth )
	{
		// 2줄짜리
		// 퀘스트 명
		g_FontMgr.SetTextStyle( TS_NORMAL );
		g_FontMgr.SetAlignType( TAT_LEFT );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextColor( dwTitleColor );
		g_FontMgr.PrintTextWidthCut( iXPos, iYPos + 6, FONT_SIZE_11, fLimitWidth, m_pQuestParent->GetTitle().c_str() );

		// 퀘스트 목적
		ioComplexStringPrinter kPrinter;
		kPrinter.SetTextStyle( TS_NORMAL );
		kPrinter.SetTextColor( dwProgressColor );
		kPrinter.AddTextPiece( FONT_SIZE_11, m_pQuestParent->GetProgressText().c_str() );
		if( strlen( szResult ) > 0 )
		{
			kPrinter.SetTextColor( TCT_DEFAULT_RED );
			kPrinter.AddTextPiece( FONT_SIZE_11, STR(1), szResult );
		}
		kPrinter.PrintFullText( iXPos, iYPos + 20, TAT_LEFT );
	}
	else
	{
		// 3줄짜리
		// 퀘스트 명
		g_FontMgr.SetTextStyle( TS_NORMAL );
		g_FontMgr.SetAlignType( TAT_LEFT );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextColor( dwTitleColor );
		g_FontMgr.PrintTextWidthCut( iXPos, iYPos, FONT_SIZE_11, fLimitWidth, m_pQuestParent->GetTitle().c_str() );

		// 퀘스트 목적
		int iResultXPos = 0;
		sprintf_e( szBuffer, "%s", m_pQuestParent->GetProgressText().c_str() );	
		g_FontMgr.SetTextColor( dwProgressColor );		
		char szDst[MAX_PATH] = "";
		memset( szDst, 0, sizeof( szDst ) );
		int iLen = Help::StringCutFun( FONT_SIZE_11, fLimitWidth, TS_NORMAL, szDst, sizeof(szDst), szBuffer );
		g_FontMgr.PrintText( iXPos, iYPos + 14, FONT_SIZE_11, szDst );
		if( iLen < (int)strlen( szBuffer ) )
		{
			float fWidthCut = fLimitWidth - g_FontMgr.GetTextWidth( szResult, TS_NORMAL, FONT_SIZE_11 );
			g_FontMgr.PrintTextWidthCut( iXPos, iYPos + 28, FONT_SIZE_11, fWidthCut, &szBuffer[iLen] );
			iResultXPos = 4 + min( fWidthCut, g_FontMgr.GetTextWidth( &szBuffer[iLen], TS_NORMAL, FONT_SIZE_11 ) );
		}

		// 
		g_FontMgr.SetTextColor( TCT_DEFAULT_RED );
		g_FontMgr.PrintText( iXPos + iResultXPos, iYPos + 28, FONT_SIZE_11, szResult );
	}
}

void LobbyQuestAlarmBtn::SetQuest( QuestData &rkQuestData )
{
	m_QuestData = rkQuestData;
	if( m_QuestData.GetMainIndex() == 0 )
	{
		SetInActive();
		HideChildWnd( ID_DELETE_BTN );
		m_pQuestParent = NULL;
	}
	else
	{
		SetActive();
		m_pQuestParent = g_QuestMgr.GetQuest( m_QuestData.GetMainIndex(), m_QuestData.GetSubIndex() );
		if( m_pQuestParent )
		{
			m_pQuestIcon = m_pQuestParent->GetQuestIcon();
			m_bIsCostume = m_pQuestParent->IsCostume();
		}
		ShowChildWnd( ID_DELETE_BTN );
	}
}
//////////////////////////////////////////////////////////////////////////
LobbyQuestAlarmWnd::LobbyQuestAlarmWnd()
{
}

LobbyQuestAlarmWnd::~LobbyQuestAlarmWnd()
{
}

void LobbyQuestAlarmWnd::iwm_show()
{
	UpdateAlarm();
}

void LobbyQuestAlarmWnd::iwm_hide()
{

}

void LobbyQuestAlarmWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_QUEST_BTN:
		if( cmd == IOBN_BTNUP )
		{
			//g_GUIMgr.ShowWnd( QUEST_MAIN_WND );
		}
		break;
	case ID_ALARM_BTN1:
	case ID_ALARM_BTN2:
	case ID_ALARM_BTN3:
	case ID_ALARM_BTN4:
		if( cmd == IOBN_BTNUP )
		{
			LobbyQuestAlarmBtn *pQuestListBtn = dynamic_cast<LobbyQuestAlarmBtn*>( pWnd );
			if( pQuestListBtn && pQuestListBtn->GetQuestParent() )
			{
				QuestProgressInfoMainWnd *pQuestInfo = dynamic_cast<QuestProgressInfoMainWnd*>(g_GUIMgr.FindWnd( QUEST_PROGRESS_INFO_WND ));
				if( pQuestInfo )
					pQuestInfo->SetQuest( pQuestListBtn->GetQuestParent() );
			}
		}
		break;
	}
}

void LobbyQuestAlarmWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;

	if( g_QuestMgr.IsQuestAlarmChange() )
	{
		UpdateAlarm();
	}
}

void LobbyQuestAlarmWnd::UpdateAlarm()
{
	ioQuest *pQuest = g_MyInfo.GetQuest();
	if( !pQuest ) return;

	int iProgressCount = pQuest->GetMaxQuest( QS_PROGRESS );
	ioWnd *pQuestBtn = FindChildWnd( ID_QUEST_BTN );
	if( pQuestBtn )
	{
		char szTitle[MAX_PATH] = "";
		SafeSprintf( szTitle, sizeof( szTitle ), STR(1), iProgressCount );
		pQuestBtn->SetTitleText( szTitle );
	}

	int iStartArray = 0;
	for(int i = 0;i < MAX_ALARM;i++,iStartArray++)
	{
		LobbyQuestAlarmBtn *pQuestAlarmBtn = dynamic_cast<LobbyQuestAlarmBtn*>( FindChildWnd( ID_ALARM_BTN1 + i ) );
		if( pQuestAlarmBtn )
		{
			pQuestAlarmBtn->SetQuest( pQuest->GetAlarmQuestToArray( iStartArray ) );
		}
	}
}
//////////////////////////////////////////////////////////////////////////
QuestInfoParentWnd::QuestInfoParentWnd()
{
	m_pRewardOnFrm	 = NULL;
	m_pRewardOffFrm	 = NULL;
	m_pRewardOffX	 = NULL;
	m_pStateGreenNew = NULL;
	m_pStateEventMark= NULL;
	m_pQuestIconBack = NULL;
	m_pRewardIconBack= NULL;
	m_pQuestParent   = NULL;
	m_pCostumeMark = NULL;
	m_iCurPos = 0;
	m_fCurWidth = 0.0f;
	m_fMaxWidth = 0.0f;
	m_dwQuestState = 0xFFFFFFFF;
}

QuestInfoParentWnd::~QuestInfoParentWnd()
{
	SAFEDELETE( m_pRewardOnFrm );
	SAFEDELETE( m_pRewardOffFrm );
	SAFEDELETE( m_pRewardOffX );
	SAFEDELETE( m_pStateGreenNew );
	SAFEDELETE( m_pStateEventMark );
	SAFEDELETE( m_pQuestIconBack );
	SAFEDELETE( m_pRewardIconBack );
	SAFEDELETE( m_pCostumeMark );
	ClearRewardIconMap();	
}

void QuestInfoParentWnd::ClearRewardIconMap()
{
	QuestRewardIconMap::iterator iCreate = m_QuestRewardIconMap.begin();
	for(;iCreate != m_QuestRewardIconMap.end();++iCreate)
	{
		SAFEDELETE( iCreate->second );
	}
	m_QuestRewardIconMap.clear();
}

void QuestInfoParentWnd::AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame )
{
	if( pFrame )
	{
		if( szType == "RewardOnFrm" )
		{
			SAFEDELETE( m_pRewardOnFrm );
			m_pRewardOnFrm = pFrame;
		}
		else if( szType == "RewardOffFrm" )
		{
			SAFEDELETE( m_pRewardOffFrm );
			m_pRewardOffFrm = pFrame;
		}
		else
		{	
			ioWnd::AddRenderFrame( szType, pFrame );
		}
	}
}

void QuestInfoParentWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "QuestIconBack" )
	{
		SAFEDELETE( m_pQuestIconBack );
		m_pQuestIconBack = pImage;
	}
	else if( szType == "RewardIconBack" )
	{
		SAFEDELETE( m_pRewardIconBack );
		m_pRewardIconBack = pImage;
	}
	else if( szType == "GreenNewState" )
	{
		SAFEDELETE( m_pStateGreenNew );
		m_pStateGreenNew = pImage;
	}
	else if( szType == "StateEventMark" )
	{
		SAFEDELETE( m_pStateEventMark );
		m_pStateEventMark = pImage;
	}
	else if( szType == "RewardOffX" )
	{
		SAFEDELETE( m_pRewardOffX );
		m_pRewardOffX = pImage;
	}
	else if( szType == "costume_image" )
	{
		SAFEDELETE( m_pCostumeMark );
		m_pCostumeMark = pImage;
	}
	else
	{
		ioWnd::AddRenderImage( szType, pImage );
	}
}

ioUIRenderImage *QuestInfoParentWnd::GetRewardIcon( int iPresentType, int iPresentValue1, int iPresentValue2 )
{
	ioHashString szIconName;
	switch( iPresentType )
	{
	case PRESENT_SOLDIER:
		{
			szIconName = g_MyInfo.GetMySoldierIconName( iPresentValue1 );
		}
		break;
	case PRESENT_DECORATION:
		{
			int iClassType = iPresentValue1 / 100000;
			int iSexType   = (iPresentValue1 % 100000) / FLOAT1000;
			int iDecoType  = iPresentValue1 % 1000;
			const DecoData &kData = g_DecorationPrice.GetDecoData( iSexType, iDecoType, iPresentValue2 );
			szIconName = kData.m_szIcon;
		}
		break;
	case PRESENT_ETC_ITEM:
		{
			ioEtcItem *pEtcItem = g_EtcItemMgr.FindEtcItem( iPresentValue1 );
			if( pEtcItem )
				szIconName = pEtcItem->GetIconName();
		}
		break;
	case PRESENT_PESO:
		{
			szIconName = "UIIconPack13#quest_002";
		}
		break;
	case PRESENT_EXTRAITEM:
	case PRESENT_ACCESSORY:
		{
			const ioItem *pItem = g_ItemMaker.GetItemConst( iPresentValue1, __FUNCTION__ );
			if( pItem )
			{
				szIconName = pItem->GetItemLargeIconName();
			}
		}
		break;
	case PRESENT_EXTRAITEM_BOX:
		{
			RandomMachineInfo *pInfo = g_ExtraItemInfoMgr.GetRandomMachineInfo( iPresentValue1 );
			if( pInfo )
			{
				szIconName = pInfo->GetIconName();
			}
		}
		break;
	case PRESENT_RANDOM_DECO:
		{
			int iClassType = iPresentValue1 / 100000;
			int iSexType   = (iPresentValue1 % 100000) / FLOAT1000;
			int iDecoType  = iPresentValue1 % 1000;
			const DecoData &kData = g_DecorationPrice.GetDecoData( iSexType, iDecoType, iPresentValue2 );
			szIconName = kData.m_szIcon;
		}
		break;
	case PRESENT_GRADE_EXP:
		{
			szIconName = "UIIconPack13#quest_001";
		}
		break;
	case PRESENT_MEDALITEM:
		{
			const ioMedalItemInfoManager::ItemInfo *pItem = g_MedalItemMgr.GetItemInfo( iPresentValue1 );
			if( pItem )	
				szIconName = pItem->m_sIcon;
		}
		break;
	case PRESENT_ALCHEMIC_ITEM:
		{
			szIconName = g_AlchemicMgr.GetAlchemicItemIcon( iPresentValue1 );
		}
		break;
	case PRESENT_PET:
		{
			int nPetRank = iPresentValue2%10000;
			szIconName = g_PetInfoMgr.GetPetIcon( iPresentValue1, (PetRankType)nPetRank );
		}
		break;
	case PRESENT_COSTUME:
		{
			// UI코스튬 관련 (퀘스트 정보 아이콘)
			szIconName = g_CostumeInfoMgr.GetCostumeIcon( iPresentValue1 );
		}
		break;
	case PRESENT_COSTUME_BOX:
		{
			CostumeShopInfo *pInfo = g_CostumeShopInfoMgr.GetCostumeMachineInfo( iPresentValue1 );
			if( pInfo )
				szIconName = pInfo->GetIconName();
		}
	case PRESENT_BONUS_CASH:
		{
			szIconName = "UIIconPack101#Cash";
		}
		break;
	}

	if( szIconName.IsEmpty() )
		return NULL;

	QuestRewardIconMap::iterator iter = m_QuestRewardIconMap.find( szIconName );
	if( iter != m_QuestRewardIconMap.end() )
		return iter->second;

	ioUIRenderImage *pIcon = g_UIImageSetMgr.CreateImageByFullName( szIconName );
	if( pIcon )
	{
		m_QuestRewardIconMap.insert( QuestRewardIconMap::value_type( szIconName, pIcon ) );
		return pIcon;
	}
	return NULL;
}

void QuestInfoParentWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;
}

void QuestInfoParentWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();
	if( m_pQuestParent )
	{
		OnRenderIcon( iXPos, iYPos );		

		// 퀘스트 설명
		g_QuestMgr.RenderQuestProgressHelp( m_pQuestParent, iXPos + 24, iYPos + 53, m_iCurPos, MAX_HELP_LINE, m_fCurWidth );

		// 퀘스트 보상
		OnRenderReward( iXPos + 21, iYPos + 288 );
	}
}

void QuestInfoParentWnd::OnRenderIcon( int iXPos, int iYPos )
{	
	enum { ICON_CUT = 4, YGAP = 21, };
	if( m_iCurPos <= ICON_CUT )
	{
		// 퀘스트 아이콘
		if( m_pQuestIconBack )
		{
			int iScrollYGap = m_iCurPos * YGAP;
		
			{
				int iIConCutHeight[ICON_CUT+1] = { 0, max( 0, m_pQuestIconBack->GetOffsetY() - YGAP ), m_pQuestIconBack->GetOffsetY(), m_pQuestIconBack->GetOffsetY() + YGAP, m_pQuestIconBack->GetHeight(), };
				m_pQuestIconBack->SetScale( FLOAT1, FLOAT1 );
				m_pQuestIconBack->RenderHeightCut( iXPos + 57, ( iYPos + 89 ) - iScrollYGap, iIConCutHeight[m_iCurPos], m_pQuestIconBack->GetHeight(), UI_RENDER_MULTIPLY );
			}

			ioUIRenderImage *pQuestIcon = m_pQuestParent->GetQuestIcon();
			if( pQuestIcon )
			{		
				int iIConCutHeight[ICON_CUT+1] = { 0, max( 0, pQuestIcon->GetOffsetY() - YGAP ), pQuestIcon->GetOffsetY(), pQuestIcon->GetOffsetY() + YGAP, pQuestIcon->GetHeight(), };
				pQuestIcon->SetScale( FLOAT1, FLOAT1 );
				pQuestIcon->RenderHeightCut( iXPos + 57, ( iYPos + 89 ) - iScrollYGap, iIConCutHeight[m_iCurPos], pQuestIcon->GetHeight() );
			}

			// UI코스튬 관련 (코스튬 마크)
			if ( m_pQuestParent->IsCostume() )
			{
				if( m_pCostumeMark )
				{		
					int iIConCutHeight[ICON_CUT+1] = { 0, max( 0, m_pCostumeMark->GetOffsetY() - YGAP ), m_pCostumeMark->GetOffsetY(), m_pCostumeMark->GetOffsetY() + YGAP, m_pCostumeMark->GetHeight(), };
					m_pCostumeMark->SetScale( 1.0f, 1.0f );
					m_pCostumeMark->RenderHeightCut( iXPos + 57, ( iYPos + 89 ) - iScrollYGap, iIConCutHeight[m_iCurPos], m_pCostumeMark->GetHeight() );
				}
			}
		}
	}

	if( m_pQuestParent->GetPerformType() == QP_EVENT )
	{
		if( m_iCurPos <= ICON_CUT - 1 && m_pStateEventMark )
		{
			int iScrollH = min( m_pStateEventMark->GetHeight(), YGAP * m_iCurPos );
			m_pStateEventMark->RenderHeightCut( iXPos, iYPos - iScrollH, iScrollH, m_pStateEventMark->GetHeight() );
		}
	}
	else if( m_iCurPos == 0 )
	{
		g_FontMgr.SetTextStyle( TS_NORMAL );
		g_FontMgr.SetAlignType( TAT_CENTER );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextColor( 255, 255, 255 );
		// 달성 퀘스트 인지 확인
		switch( m_dwQuestState )
		{
		case QS_PROGRESS:
		case QS_ATTAIN:
		case QS_COMPLETE:
			break;
		default:
			{
				if( m_pStateGreenNew )
					m_pStateGreenNew->Render( iXPos, iYPos );
			}
			break;
		}
	}
}

void QuestInfoParentWnd::OnRenderReward( int iXPos, int iYPos, int iWhiteAlphaRate /* = 0  */ )
{
	if( !m_pQuestParent ) return;
	if( m_pQuestParent->GetMaxRewardPresent() == 0 ) return;

	enum { XGAP=79, };
	for(int i = 0;i < MAX_REWARD;i++)
	{
		int iRealXPos = iXPos + i * XGAP;
		DWORD dwRewardIndex = m_pQuestParent->GetRewardPresentIndex( i );
		if( dwRewardIndex == 0 )
		{
			if( m_pRewardOffFrm )
				m_pRewardOffFrm->Render( iRealXPos, iYPos, UI_RENDER_MULTIPLY );

			if( m_pRewardOffX )
			{
				m_pRewardOffX->SetScale( FLOAT09 );
				m_pRewardOffX->Render( iRealXPos + 39, iYPos + 39, UI_RENDER_MULTIPLY );
			}
			g_FontMgr.SetTextStyle( TS_NORMAL );
			g_FontMgr.SetAlignType( TAT_CENTER );
			g_FontMgr.SetBkColor( 0, 0, 0 );
			g_FontMgr.SetTextColor( 136, 122, 84 );
			g_FontMgr.PrintText( iRealXPos + 39, iYPos + 98, FONT_SIZE_12, STR(1), i + 1 );	
		}
		else
		{
			if( m_pRewardOnFrm )
				m_pRewardOnFrm->Render( iRealXPos, iYPos );

			bool bPresentDirect = false;
			int iPresentType, iPresentValue1, iPresentValue2;
			g_QuestMgr.GetPresentData( dwRewardIndex, iPresentType, iPresentValue1, iPresentValue2, bPresentDirect );

			ioUIRenderImage *pRewardIcon = GetRewardIcon( iPresentType, iPresentValue1, iPresentValue2 );
			if( m_pRewardIconBack && pRewardIcon )
			{
				m_pRewardIconBack->SetScale( FLOAT09, FLOAT09 );
				m_pRewardIconBack->Render( iRealXPos + 39, iYPos + 39, UI_RENDER_MULTIPLY, TFO_BILINEAR );
				
				pRewardIcon->SetAlpha( MAX_ALPHA_RATE );
				pRewardIcon->SetScale( FLOAT09, FLOAT09 );
				pRewardIcon->Render( iRealXPos + 39, iYPos + 39, UI_RENDER_NORMAL, TFO_BILINEAR );

				if( iWhiteAlphaRate > 0 )
				{
					pRewardIcon->SetAlpha( iWhiteAlphaRate );
					pRewardIcon->SetColor( 255, 255, 255 );
					pRewardIcon->Render( iRealXPos + 39, iYPos + 39, UI_RENDER_NORMAL_ADD_COLOR, TFO_BILINEAR );
				}

				// UI코스튬 관련 (코스튬 마크)
				if ( ( iPresentType == PRESENT_COSTUME || iPresentType == PRESENT_COSTUME_BOX ) && m_pCostumeMark )
					m_pCostumeMark->Render( iRealXPos + 39, iYPos + 39, UI_RENDER_NORMAL, TFO_BILINEAR );
			}

			char szPresentName1[MAX_PATH] = "";
			char szPresentName2[MAX_PATH] = "";
			sprintf_e( szPresentName1, "%s", g_PresentMgr.GetPresentValue1Text( iPresentType, iPresentValue1, iPresentValue2 ).c_str() );
			if( iPresentType == PRESENT_MEDALITEM )
				SafeSprintf( szPresentName2, sizeof( szPresentName2 ), STR(2), g_PresentMgr.GetPresentValue2Text( iPresentType, iPresentValue1, iPresentValue2 ).c_str() );
			else
				sprintf_e( szPresentName2, "%s", g_PresentMgr.GetPresentValue2Text( iPresentType, iPresentValue1, iPresentValue2 ).c_str() );

			float fTextWidth = 71.0f;
			g_FontMgr.SetTextStyle( TS_NORMAL );
			g_FontMgr.SetAlignType( TAT_CENTER );
			g_FontMgr.SetBkColor( 0, 0, 0 );
			g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
			if( g_FontMgr.GetTextWidth( szPresentName1, TS_NORMAL, FONT_SIZE_12 ) <= fTextWidth )
			{
				g_FontMgr.PrintTextWidthCut( iRealXPos + 39, iYPos + 91, FONT_SIZE_12, fTextWidth, szPresentName1 );	
				g_FontMgr.PrintTextWidthCut( iRealXPos + 39, iYPos + 105, FONT_SIZE_12, fTextWidth, szPresentName2 );
			}
			else // 2줄까지만 내려쓰기
			{
				char szDst[MAX_PATH] = "";
				memset( szDst, 0, sizeof( szDst ) );
				int iLen = Help::StringCutFun( FONT_SIZE_12, fTextWidth, TS_NORMAL, szDst, sizeof(szDst), szPresentName1 );
				g_FontMgr.PrintText( iRealXPos + 39, iYPos + 84, FONT_SIZE_12, szDst );
				if( iLen < (int)strlen( szPresentName1 ) )
				{
					g_FontMgr.PrintTextWidthCut( iRealXPos + 39, iYPos + 98, FONT_SIZE_12, fTextWidth, &szPresentName1[iLen] );	
					g_FontMgr.PrintTextWidthCut( iRealXPos + 39, iYPos + 112, FONT_SIZE_12, fTextWidth, szPresentName2 );
				}
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
QuestOccurStateWnd::QuestOccurStateWnd()
{
	m_bMaximumWnd     = false;
	m_dwTextTickTime  = 0;
	m_dwTextTickCheckTime = 0;
	m_pBottomFrm	  = NULL;
	m_pTextBox        = NULL;
	m_pStateAimFrm    = NULL;
	m_iAddMaxHeight   = 0;
	m_iMinusRewardPassH = 0;
	m_dwCurrentScrollTime = 0;
	m_dwScrollTickCheckTime = 0;
	m_dwMaximumAniTime = 0;

	m_iWhiteAlphaRate  = 0;
	m_dwWhiteAlphaTime = 0;

	m_pRewardSelectDescWnd = 0;

	m_fTextPrintSpeed = 0.0f;
	m_fTextPrintSpeed_Scroll = 0.0f;
}

QuestOccurStateWnd::~QuestOccurStateWnd()
{
	SAFEDELETE( m_pBottomFrm );
	SAFEDELETE( m_pTextBox );
	SAFEDELETE( m_pStateAimFrm );
}

void QuestOccurStateWnd::AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame )
{
	if( pFrame )
	{
		if( szType == "BottomFrm" )
		{
			SAFEDELETE( m_pBottomFrm );
			m_pBottomFrm = pFrame;
		}
		else if( szType == "TextBox" )
		{
			SAFEDELETE( m_pTextBox );
			m_pTextBox = pFrame;
		}
		else if( szType == "StateAimFrm" )
		{
			SAFEDELETE( m_pStateAimFrm );
			m_pStateAimFrm = pFrame;
		}
		else
		{	
			QuestInfoParentWnd::AddRenderFrame( szType, pFrame );
		}
	}
}

void QuestOccurStateWnd::ReSizeWnd( int iWidth, int iHeight )
{
	SetSize( iWidth, iHeight );

	if( GetParent() )
	{
		GetParent()->SetSize( iWidth, iHeight );
	
		RECT  rcDrag = { 0, 0, GetWidth(), GetHeight() };
		GetParent()->SetDragRect( rcDrag );
	}
}

void QuestOccurStateWnd::ReSize( bool bMax )
{
	if( !bMax )
	{
		ReSizeWnd( MINIMUM_W, MINIMUM_H );

		ShowChildWnd( ID_AGREE_BTN );
		HideChildWnd( ID_CLOSE_BTN );
		m_dwMaximumAniTime = 0;
	}
	else
	{
		HideChildWnd( ID_AGREE_BTN );
		// 펼쳐질 때 사운드 1회만 발생
		if( m_bMaximumWnd != bMax && !m_szAgreeSnd.IsEmpty() )
			g_SoundMgr.PlaySound( m_szAgreeSnd );
		
		m_dwMaximumAniTime = FRAMEGETTIME();
		m_iWhiteAlphaRate  = 0;
		m_dwWhiteAlphaTime = 0;
	}
	m_bMaximumWnd = bMax;
}

bool QuestOccurStateWnd::IsRewardSelectStyle()
{
	if ( !m_pQuestParent ) {
		return false;
	}
	return m_pQuestParent->IsRewardSelectStyle();
}

void QuestOccurStateWnd::iwm_show()
{
	m_pQuestParent = g_QuestMgr.FirstQuestOccurList();
	m_dwQuestState = 0xFFFFFFFF;
	if( m_pQuestParent )
	{
		// 퀘스트 명
		char szTitle[MAX_PATH] = "";
		sprintf_s_e( szTitle, "%s", m_pQuestParent->GetTitle().c_str() );
		SetTitleText( szTitle );

		ioQuest *pQuest = g_MyInfo.GetQuest();
		if( pQuest )
		{
			m_dwQuestState = pQuest->GetQuestState( m_pQuestParent->GetMainIndex(), m_pQuestParent->GetSubIndex() );
		}		
		
		m_iMinusRewardPassH = 0;
		if( m_pQuestParent->GetMaxRewardPresent() == 0 )
			m_iMinusRewardPassH = REWARD_PASS_H;         // 보상이 없으면 윈도우 사이즈 -140

		m_pRewardSelectDescWnd = FindChildWnd( REWARD_SELECT_DESC_WND );
		if ( m_pRewardSelectDescWnd ) 
		{
			if ( m_pRewardSelectDescWnd->GetTitle() )
			{
				char buff[ MAX_PATH ] = "";
				SafeSprintf( buff, sizeof(buff), STR(1), m_pQuestParent->GetRewardSelectNum() );
				m_pRewardSelectDescWnd->GetTitle()->SetText( buff, "" );
			}
			m_pRewardSelectDescWnd->HideWnd();
		}

		m_dwCurrentScrollTime = 0;
		m_iAddMaxHeight = g_QuestMgr.GetMaxQuestPurposeHelp( m_pQuestParent ) * PURPOSE_GAP;
		if ( m_pRewardSelectDescWnd && IsRewardSelectStyle() ) {
			m_iAddMaxHeight += ( m_pRewardSelectDescWnd->GetHeight() + 2 );
		}
		if( m_dwQuestState == 0xFFFFFFFF )
		{
			ReSize( false );
		}
		else
		{
			ReSize( true );
		}

		if( !m_szShowSnd.IsEmpty() )
			g_SoundMgr.PlaySound( m_szShowSnd );

		m_iCurPos = 0;
		ioScroll *pScroll = (ioScroll*)FindChildWnd( ID_VERT_SCROLL );
		if( pScroll )
		{
			pScroll->SetScrollRange( m_iCurPos, g_QuestMgr.GetMaxQuestHelp( m_pQuestParent, QS_PROGRESS ) );
			pScroll->SetScrollPos( m_iCurPos );
		}

		m_fCurWidth      = 0.0f;
		m_fMaxWidth      = g_QuestMgr.GetMaxQuestHelpWidth( m_pQuestParent, QS_PROGRESS );
		m_dwTextTickTime = FRAMEGETTIME();
	}
	else
	{
		HideWnd();
	}
}

void QuestOccurStateWnd::iwm_hide()
{
	m_pQuestParent = NULL;
	if( !m_szHideSnd.IsEmpty() )
		g_SoundMgr.PlaySound( m_szHideSnd );

	if( GetParent() && GetParent()->IsShow() )
		GetParent()->HideWnd();
}

void QuestOccurStateWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_AGREE_BTN:
		if( cmd == IOBN_BTNUP )
		{
			if( m_pQuestParent )
			{
				SP2Packet kPacket( CTPK_QUEST_OCCUR );
				kPacket << m_pQuestParent->GetMainIndex() << m_pQuestParent->GetSubIndex();
				TCPNetwork::SendToServer( kPacket );
				TCPNetwork::MouseBusy( true );

				HideChildWnd( ID_AGREE_BTN );
				m_dwCurrentScrollTime = FRAMEGETTIME();

				// 스크롤할 내용이 없으면 최대화
				ioScroll *pScroll = (ioScroll*)FindChildWnd( ID_VERT_SCROLL );
				if( pScroll )
				{
					if( m_iCurPos >= pScroll->GetMaxPos() )
					{
						ReSize( true );
					}
				}
			}
		}
		break;
	case ID_CLOSE_BTN:
		if( cmd == IOBN_BTNUP )
		{
			g_QuestMgr.EraseQuestOccurList( m_pQuestParent->GetMainIndex(), m_pQuestParent->GetSubIndex() );
			HideWnd();			
		}
		break;
	}
}

void QuestOccurStateWnd::SetPositionToDefault( int iParentWidth, int iParentHeight, bool bChildMove, bool bCreate /* = false  */ )
{
	QuestInfoParentWnd::SetPositionToDefault( iParentWidth, iParentHeight, bChildMove, bCreate );
	if( !bCreate )
	{
		ReSize( m_bMaximumWnd );
	}
}

void QuestOccurStateWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	m_szShowSnd  = xElement.GetStringAttribute_e( "ShowSnd" );
	m_szTickSnd  = xElement.GetStringAttribute_e( "TickSnd" );
	m_szHideSnd  = xElement.GetStringAttribute_e( "HideSnd" );
	m_szAgreeSnd = xElement.GetStringAttribute_e( "AgreeSnd" );
	m_dwTextTickCheckTime = xElement.GetIntAttribute_e( "TickCheckTime" );
	m_dwScrollTickCheckTime = xElement.GetIntAttribute_e( "ScrollTickCheckTime" );
	m_fTextPrintSpeed_Scroll = xElement.GetFloatAttribute( "TextPrintSpeed_Scroll" );
	m_fTextPrintSpeed = xElement.GetFloatAttribute( "TextPrintSpeed" );
}

void QuestOccurStateWnd::iwm_wheel( int zDelta )
{
	if( !IsShow() )
		ioWnd::iwm_wheel( zDelta );
	else
	{
		if( g_App.IsMouseBusy() ) return;

		ioScroll *pScroll = (ioScroll*)FindChildWnd( ID_VERT_SCROLL );
		if( pScroll == NULL ) return;
		if( pScroll->HasWndStyle( IWS_INACTIVE ) ) return;

		if( zDelta == WHEEL_DELTA )
			pScroll->SetScrollPos( m_iCurPos - 1 );
		else if( zDelta == -WHEEL_DELTA )
			pScroll->SetScrollPos( m_iCurPos + 1 );
	}
}

void QuestOccurStateWnd::iwm_vscroll( DWORD dwID, int curPos )
{
	if( m_iCurPos > curPos )
		m_dwCurrentScrollTime = 0;       // 스크롤을 위로 올리면 자동 스크롤 중단.

	m_iCurPos = curPos;

	if( !m_bMaximumWnd )
	{
		ioScroll *pScroll = (ioScroll*)FindChildWnd( ID_VERT_SCROLL );
		if( pScroll )
		{
			if( m_iCurPos >= pScroll->GetMaxPos() )
			{
				ioWnd *pAgreeBtn = FindChildWnd( ID_AGREE_BTN );
				if( pAgreeBtn && !pAgreeBtn->IsShow() )
				{
					ReSize( true );
				}
			}
		}
	}
}

bool QuestOccurStateWnd::iwm_spacebar()
{
	ioWnd *pAgreeBtn = FindChildWnd( ID_AGREE_BTN );
	if( pAgreeBtn && pAgreeBtn->IsShow() )
	{
		iwm_command( pAgreeBtn, IOBN_BTNUP, 0 );
		return true;
	}
	else
	{
		ioWnd *pCloseBtn = FindChildWnd( ID_CLOSE_BTN );
		if( pCloseBtn && pCloseBtn->IsShow() )
		{
			iwm_command( pCloseBtn, IOBN_BTNUP, 0 );
			return true;
		}
	}
	return false;
}

void QuestOccurStateWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;

	if( m_fCurWidth < m_fMaxWidth )
	{
		if( FRAMEGETTIME() - m_dwTextTickTime > m_dwTextTickCheckTime )
		{
			m_dwTextTickTime = FRAMEGETTIME();
			if( m_dwCurrentScrollTime > 0 )
				m_fCurWidth = min( m_fMaxWidth , m_fCurWidth + m_fTextPrintSpeed_Scroll );
			else
				m_fCurWidth = min( m_fMaxWidth , m_fCurWidth + m_fTextPrintSpeed );
			if( !m_szTickSnd.IsEmpty() )
				g_SoundMgr.PlaySound( m_szTickSnd );
		}
	}

	if( m_dwCurrentScrollTime > 0 )
	{
		if( FRAMEGETTIME() - m_dwCurrentScrollTime > m_dwScrollTickCheckTime )
		{
			m_dwCurrentScrollTime = FRAMEGETTIME();
			ioScroll *pScroll = (ioScroll*)FindChildWnd( ID_VERT_SCROLL );
			if( pScroll && !pScroll->HasWndStyle( IWS_INACTIVE ) )
			{
				pScroll->SetScrollPos( m_iCurPos + 3 );
				if( pScroll->GetCurPos() >= pScroll->GetMaxPos() )
					m_dwCurrentScrollTime = 0;
			}
		}
	}

	if( m_dwMaximumAniTime > 0 )
	{
		if( FRAMEGETTIME() - m_dwMaximumAniTime >= 200 )
		{
			ReSizeWnd( MAXIMUM_W, (MAXIMUM_H - m_iMinusRewardPassH) + m_iAddMaxHeight );

			ioWnd *pCloseBtn = FindChildWnd( ID_CLOSE_BTN );
			if( pCloseBtn )
			{
				pCloseBtn->SetWndPos( pCloseBtn->GetXPos(), GetHeight() - BTN_BOTTOM );
				pCloseBtn->ShowWnd();
			}

			m_dwMaximumAniTime = 0;
			m_iWhiteAlphaRate  = MAX_ALPHA_RATE;
			m_dwWhiteAlphaTime = FRAMEGETTIME();

			if ( m_pRewardSelectDescWnd ) {
				if ( m_bMaximumWnd && IsRewardSelectStyle() ) {
					m_pRewardSelectDescWnd->ShowWnd();
				} else {
					m_pRewardSelectDescWnd->HideWnd();
				}
			}
		}
		else
		{
			float fRate = (float)(FRAMEGETTIME() - m_dwMaximumAniTime) / 200.0f;
			int iHeightRate = (float)( ((MAXIMUM_H - m_iMinusRewardPassH) + m_iAddMaxHeight) - MINIMUM_H ) * fRate;

			ReSizeWnd( MAXIMUM_W, MINIMUM_H + iHeightRate );
		}
	}

	if( m_dwWhiteAlphaTime > 0 )
	{
		if( FRAMEGETTIME() - m_dwWhiteAlphaTime >= 200 )
		{
			m_iWhiteAlphaRate  = 0;
			m_dwWhiteAlphaTime = 0;
		}
		else
		{
			float fRate = max( 0.0f, FLOAT1 - ( (float)(FRAMEGETTIME() - m_dwWhiteAlphaTime) / 200.0f ) );
			m_iWhiteAlphaRate = (float)MAX_ALPHA_RATE * fRate;
		}
	}
}

void QuestOccurStateWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();
	if( m_pQuestParent )
	{
		if( m_pBottomFrm )
		{
			m_pBottomFrm->SetSize( GetWidth(), GetHeight() - BOTTOM_Y );
			m_pBottomFrm->Render( iXPos, iYPos, UI_RENDER_MULTIPLY );
		}

		OnRenderIcon( iXPos, iYPos );		

		// 퀘스트 설명
		g_QuestMgr.RenderQuestProgressHelp( m_pQuestParent, iXPos + 24, iYPos + 53, m_iCurPos, MAX_HELP_LINE, m_fCurWidth );

		// 퀘스트 보상
		if( !m_bMaximumWnd )
		{
			ioWnd *pAgreeBtn = FindChildWnd( ID_AGREE_BTN );
			if( pAgreeBtn && !pAgreeBtn->IsShow() )
			{
				if( m_pTextBox )
					m_pTextBox->Render( iXPos, iYPos, UI_RENDER_MULTIPLY );

				g_FontMgr.SetAlignType( TAT_CENTER );
				g_FontMgr.SetTextStyle( TS_NORMAL );
				g_FontMgr.SetTextColor( 89, 77, 50 );
				g_FontMgr.PrintText( iXPos + 200, iYPos + 295, FONT_SIZE_13, STR(1) );
			}
		}
		else if( m_bMaximumWnd && m_dwMaximumAniTime == 0 )
		{
			// 퀘스트 목표
			if( m_pStateAimFrm )
				m_pStateAimFrm->Render( iXPos + 17, iYPos + 288 );
			
			g_FontMgr.SetAlignType( TAT_CENTER );
			g_FontMgr.SetTextStyle( TS_NORMAL );
			g_FontMgr.SetTextColor( TCT_DEFAULT_WHITE );
			g_FontMgr.PrintText( iXPos + 44, iYPos + 289, FONT_SIZE_11, STR(2) );

			ioComplexStringPrinter kPrinter;
			kPrinter.SetTextStyle( TS_NORMAL );
			kPrinter.SetTextColor( 0, 0, 0 );
			kPrinter.AddTextPiece( FONT_SIZE_13, m_pQuestParent->GetProgressText().c_str() );
			kPrinter.SetTextColor( TCT_DEFAULT_RED );

			char szBuf[MAX_PATH] = {0, };
			strcpy(szBuf, m_pQuestParent->GetProgressResult().c_str());
			int len = strlen(szBuf);
			if(len > 0)
				kPrinter.AddTextPiece( FONT_SIZE_13, STR(3), m_pQuestParent->GetProgressResult().c_str() );

			kPrinter.PrintFullText( iXPos + 72, iYPos + 289, TAT_LEFT );

			g_QuestMgr.RenderQuestPurposeHelp( m_pQuestParent, iXPos + 17, iYPos + 312 );

			// 퀘스트 보상
			OnRenderReward( iXPos + 21, iYPos + 316 + m_iAddMaxHeight, m_iWhiteAlphaRate );

			// 보상 선택 설명
			if ( m_pRewardSelectDescWnd && m_pRewardSelectDescWnd->IsShow() ) {
				m_pRewardSelectDescWnd->SetWndPos( m_pRewardSelectDescWnd->GetXPos(), 316 + m_iAddMaxHeight - 2 - m_pRewardSelectDescWnd->GetHeight() );
			}
		}
	}
}

bool QuestOccurStateWnd::OnRenderShowAndHide()
{
	return ioWnd::OnRenderShowAndHide();
}
//////////////////////////////////////////////////////////////////////////
QuestOccurStateMainWnd::QuestOccurStateMainWnd()
{
	m_pQuestParent    = NULL;
	m_dwHideDelayTime = 0;
}

QuestOccurStateMainWnd::~QuestOccurStateMainWnd()
{

}

void QuestOccurStateMainWnd::iwm_create()
{
	g_GUIMgr.AddWnd( "XML/questoccurstatebrownwnd.xml", this );
	g_GUIMgr.AddWnd( "XML/questoccurstatepinkwnd.xml", this );
}

void QuestOccurStateMainWnd::iwm_show()
{
	m_pQuestParent = g_QuestMgr.FirstQuestOccurList();
	if( m_pQuestParent )
	{
		QuestOccurStateWnd *pQuestWnd = NULL;
		if( m_pQuestParent->GetPerformType() == QP_EVENT )
			pQuestWnd = dynamic_cast< QuestOccurStateWnd* >( FindChildWnd( ID_PINK_WND ) );
		else
			pQuestWnd = dynamic_cast< QuestOccurStateWnd* >( FindChildWnd( ID_BROWN_WND ) );
		if( pQuestWnd )
			pQuestWnd->ShowWnd();
	}
}

void QuestOccurStateMainWnd::iwm_hide()
{
	if( m_pQuestParent )
	{
		QuestOccurStateWnd *pQuestWnd = NULL;
		if( m_pQuestParent->GetPerformType() == QP_EVENT )
			pQuestWnd = dynamic_cast< QuestOccurStateWnd* >( FindChildWnd( ID_PINK_WND ) );
		else
			pQuestWnd = dynamic_cast< QuestOccurStateWnd* >( FindChildWnd( ID_BROWN_WND ) );
		if( pQuestWnd && pQuestWnd->IsShow() )
			pQuestWnd->HideWnd();
	}
	m_dwHideDelayTime = FRAMEGETTIME();
}

void QuestOccurStateMainWnd::iwm_wheel( int zDelta )
{
	if( !IsShow() ) return;

	if( m_pQuestParent )
	{
		QuestOccurStateWnd *pQuestWnd = NULL;
		if( m_pQuestParent->GetPerformType() == QP_EVENT )
			pQuestWnd = dynamic_cast< QuestOccurStateWnd* >( FindChildWnd( ID_PINK_WND ) );
		else
			pQuestWnd = dynamic_cast< QuestOccurStateWnd* >( FindChildWnd( ID_BROWN_WND ) );
		if( pQuestWnd && pQuestWnd->IsShow() )
			return pQuestWnd->iwm_wheel( zDelta );
	}
}

bool QuestOccurStateMainWnd::iwm_spacebar()
{
	if( m_pQuestParent )
	{
		QuestOccurStateWnd *pQuestWnd = NULL;
		if( m_pQuestParent->GetPerformType() == QP_EVENT )
			pQuestWnd = dynamic_cast< QuestOccurStateWnd* >( FindChildWnd( ID_PINK_WND ) );
		else
			pQuestWnd = dynamic_cast< QuestOccurStateWnd* >( FindChildWnd( ID_BROWN_WND ) );
		if( pQuestWnd && pQuestWnd->IsShow() )
			return pQuestWnd->iwm_spacebar();
	}
	return false;
}

bool QuestOccurStateMainWnd::IsShowCheck()
{
	if( g_QuestMgr.FirstQuestAttainList() )
		return false;
	if( g_GUIMgr.IsShow( QUEST_ATTAIN_STATE_WND ) )
		return false;
	if( g_GUIMgr.IsShow( QUEST_ATTAIN_SELECT_STATE_BROWN_WND ) )
		return false;
	if( g_GUIMgr.IsShow( QUEST_ATTAIN_SELECT_STATE_PINK_WND ) )
		return false;
	return true;
}

void QuestOccurStateMainWnd::OnProcess( float fTimePerSec )
{
	if( IsShow() )
	{
		if( IsShowCheck() )
		{
			ioMovingWnd::OnProcess( fTimePerSec );
			if( !g_QuestMgr.FirstQuestOccurList() )
				HideWnd();
		}
		else
		{
			HideWnd();
		}
	}
	else if( IsShowCheck() )
	{
		if( FRAMEGETTIME() - m_dwHideDelayTime < FLOAT100 )
			return;

		if( g_QuestMgr.FirstQuestOccurList() )
			ShowWnd();
	}
}

bool QuestOccurStateMainWnd::OnRenderShowAndHide()
{
	if( m_pQuestParent )
	{
		QuestOccurStateWnd *pQuestWnd = NULL;
		if( m_pQuestParent->GetPerformType() == QP_EVENT )
			pQuestWnd = dynamic_cast< QuestOccurStateWnd* >( FindChildWnd( ID_PINK_WND ) );
		else
			pQuestWnd = dynamic_cast< QuestOccurStateWnd* >( FindChildWnd( ID_BROWN_WND ) );
		if( pQuestWnd )
			return pQuestWnd->OnRenderShowAndHide();
	}
	return false;
}

void QuestOccurStateMainWnd::SetPositionToDefault( int iParentWidth, int iParentHeight, bool bChildMove, bool bCreate /* = false  */ )
{
	if( !bCreate )
	{
		if( m_pQuestParent )
		{
			QuestOccurStateWnd *pQuestWnd = NULL;
			if( m_pQuestParent->GetPerformType() == QP_EVENT )
				pQuestWnd = dynamic_cast< QuestOccurStateWnd* >( FindChildWnd( ID_PINK_WND ) );
			else
				pQuestWnd = dynamic_cast< QuestOccurStateWnd* >( FindChildWnd( ID_BROWN_WND ) );
			if( pQuestWnd && pQuestWnd->IsShow() )
				pQuestWnd->SetPositionToDefault( iParentWidth, iParentHeight, bChildMove, bCreate );
		}
	}
	ioMovingWnd::SetPositionToDefault( iParentWidth, iParentHeight, bChildMove, bCreate );
}
//////////////////////////////////////////////////////////////////////////
QuestAttainStateWnd::QuestAttainStateWnd()
{
	m_pBrownBackFrm = NULL;
	m_pBrownTitleFrm = NULL;
	m_pBrownBottomFrm = NULL;
	m_pBrownRewardOnFrm = NULL;

	m_pPinkBackFrm = NULL;
	m_pPinkTitleFrm = NULL;
	m_pPinkBottomFrm = NULL;
	m_pPinkRewardOnFrm = NULL;

	m_pQuestIconBack1 = NULL;
	m_pQuestIconBack2 = NULL;
	m_pCompleteText = NULL;

	m_pCurrentBackFrm = NULL;
	m_pCurrentTitleFrm = NULL;
	m_pCurrentBottomFrm = NULL;
	m_pCurrentRewardOnFrm = NULL;
	m_pCostumeMark = NULL;

	m_pQuestParent = NULL;
	m_dwHideDelayTime   = 0;
	m_iHelpMentSize     = 0;
	m_iRewardSize       = 0;

	m_dwState = STATE_NONE;
	m_dwCompleteTime = 0;
	m_iQuestIconWhiteAlpha = 0;
}

QuestAttainStateWnd::~QuestAttainStateWnd()
{
	SAFEDELETE( m_pBrownBackFrm );
	SAFEDELETE( m_pBrownTitleFrm );
	SAFEDELETE( m_pBrownBottomFrm );
	SAFEDELETE( m_pBrownRewardOnFrm );

	SAFEDELETE( m_pPinkBackFrm );
	SAFEDELETE( m_pPinkTitleFrm );
	SAFEDELETE( m_pPinkBottomFrm );
	SAFEDELETE( m_pPinkRewardOnFrm );

	SAFEDELETE( m_pQuestIconBack1 );
	SAFEDELETE( m_pQuestIconBack2 );
	SAFEDELETE( m_pCompleteText );
	SAFEDELETE( m_pCostumeMark );

	ClearRewardIconMap();	
}

void QuestAttainStateWnd::ClearRewardIconMap()
{
	QuestRewardIconMap::iterator iCreate = m_QuestRewardIconMap.begin();
	for(;iCreate != m_QuestRewardIconMap.end();++iCreate)
	{
		SAFEDELETE( iCreate->second );
	}
	m_QuestRewardIconMap.clear();
}

ioUIRenderImage *QuestAttainStateWnd::GetRewardIcon( int iPresentType, int iPresentValue1, int iPresentValue2 )
{
	ioHashString szIconName;
	switch( iPresentType )
	{
	case PRESENT_SOLDIER:
		{
			szIconName = g_MyInfo.GetMySoldierIconName( iPresentValue1 );
		}
		break;
	case PRESENT_DECORATION:
		{
			int iClassType = iPresentValue1 / 100000;
			int iSexType   = (iPresentValue1 % 100000) / FLOAT1000;
			int iDecoType  = iPresentValue1 % 1000;
			const DecoData &kData = g_DecorationPrice.GetDecoData( iSexType, iDecoType, iPresentValue2 );
			szIconName = kData.m_szIcon;
		}
		break;
	case PRESENT_ETC_ITEM:
		{
			ioEtcItem *pEtcItem = g_EtcItemMgr.FindEtcItem( iPresentValue1 );
			if( pEtcItem )
				szIconName = pEtcItem->GetIconName();
		}
		break;
	case PRESENT_PESO:
		{
			szIconName = "UIIconPack13#quest_002";
		}
		break;
	case PRESENT_EXTRAITEM:
	case PRESENT_ACCESSORY:
		{
			const ioItem *pItem = g_ItemMaker.GetItemConst( iPresentValue1, __FUNCTION__ );
			if( pItem )
			{
				szIconName = pItem->GetItemLargeIconName();
			}
		}
		break;
	case PRESENT_EXTRAITEM_BOX:
		{
			RandomMachineInfo *pInfo = g_ExtraItemInfoMgr.GetRandomMachineInfo( iPresentValue1 );
			if( pInfo )
			{
				szIconName = pInfo->GetIconName();
			}
		}
		break;
	case PRESENT_RANDOM_DECO:
		{
			int iClassType = iPresentValue1 / 100000;
			int iSexType   = (iPresentValue1 % 100000) / FLOAT1000;
			int iDecoType  = iPresentValue1 % 1000;
			const DecoData &kData = g_DecorationPrice.GetDecoData( iSexType, iDecoType, iPresentValue2 );
			szIconName = kData.m_szIcon;
		}
		break;
	case PRESENT_GRADE_EXP:
		{
			szIconName = "UIIconPack13#quest_001";
		}
		break;
	case PRESENT_MEDALITEM:
		{
			const ioMedalItemInfoManager::ItemInfo *pItem = g_MedalItemMgr.GetItemInfo( iPresentValue1 );
			if( pItem )	
				szIconName = pItem->m_sIcon;
		}
		break;
	case PRESENT_ALCHEMIC_ITEM:
		{
			szIconName = g_AlchemicMgr.GetAlchemicItemIcon( iPresentValue1 );
		}
		break;
	case PRESENT_PET:
		{
			int nPetRank = iPresentValue2%10000;
			szIconName = g_PetInfoMgr.GetPetIcon( iPresentValue1, (PetRankType)nPetRank );
		}
		break;
	case PRESENT_COSTUME:
		{
			// UI코스튬 관련 (퀘스트 아이콘)
			szIconName = g_CostumeInfoMgr.GetCostumeIcon( iPresentValue1 );
		}
		break;
	case PRESENT_COSTUME_BOX:
		{
			CostumeShopInfo *pInfo = g_CostumeShopInfoMgr.GetCostumeMachineInfo( iPresentValue1 );
			if( pInfo )
				szIconName = pInfo->GetIconName();
		}
		break;
	case PRESENT_BONUS_CASH:
		{
			szIconName = "UIIconPack101#Cash";
		}
		break;
	}

	if( szIconName.IsEmpty() )
		return NULL;

	QuestRewardIconMap::iterator iter = m_QuestRewardIconMap.find( szIconName );
	if( iter != m_QuestRewardIconMap.end() )
		return iter->second;

	ioUIRenderImage *pIcon = g_UIImageSetMgr.CreateImageByFullName( szIconName );
	if( pIcon )
	{
		m_QuestRewardIconMap.insert( QuestRewardIconMap::value_type( szIconName, pIcon ) );
		return pIcon;
	}
	return NULL;
}

void QuestAttainStateWnd::AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame )
{
	if( pFrame )
	{
		if( szType == "BrownBack" )
		{
			SAFEDELETE( m_pBrownBackFrm );
			m_pBrownBackFrm = pFrame;
		}
		else if( szType == "BrownTitle" )
		{
			SAFEDELETE( m_pBrownTitleFrm );
			m_pBrownTitleFrm = pFrame;
		}
		else if( szType == "BrownBottomFrm" )
		{
			SAFEDELETE( m_pBrownBottomFrm );
			m_pBrownBottomFrm = pFrame;
		}
		else if( szType == "BrownRewardOnFrm" )
		{
			SAFEDELETE( m_pBrownRewardOnFrm );
			m_pBrownRewardOnFrm = pFrame;
		}
		else if( szType == "PinkBack" )
		{
			SAFEDELETE( m_pPinkBackFrm );
			m_pPinkBackFrm = pFrame;
		}
		else if( szType == "PinkTitle" )
		{
			SAFEDELETE( m_pPinkTitleFrm );
			m_pPinkTitleFrm = pFrame;
		}
		else if( szType == "PinkBottomFrm" )
		{
			SAFEDELETE( m_pPinkBottomFrm );
			m_pPinkBottomFrm = pFrame;
		}
		else if( szType == "PinkRewardOnFrm" )
		{
			SAFEDELETE( m_pPinkRewardOnFrm );
			m_pPinkRewardOnFrm = pFrame;
		}
		else
		{	
			ioWnd::AddRenderFrame( szType, pFrame );
		}
	}
}

void QuestAttainStateWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "QuestIconBack1" )
	{
		SAFEDELETE( m_pQuestIconBack1 );
		m_pQuestIconBack1 = pImage;
	}
	else if( szType == "QuestIconBack2" )
	{
		SAFEDELETE( m_pQuestIconBack2 );
		m_pQuestIconBack2 = pImage;
	}
	else if( szType == "CompleteText" )
	{
		SAFEDELETE( m_pCompleteText );
		m_pCompleteText = pImage;
	}
	else if( szType == "costume_image" )
	{
		SAFEDELETE( m_pCostumeMark );
		m_pCostumeMark = pImage;
	}
	else
	{
		ioWnd::AddRenderImage( szType, pImage );
	}
}

void QuestAttainStateWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	m_szShowSnd   = xElement.GetStringAttribute_e( "ShowSnd" );
	m_szRewardSnd = xElement.GetStringAttribute_e( "Reward" );
}

void QuestAttainStateWnd::SetPositionToDefault( int iParentWidth, int iParentHeight, bool bChildMove, bool bCreate )
{
	ioWnd::SetPositionToDefault( iParentWidth, iParentHeight, bChildMove, bCreate );
	if( !bCreate )
	{
		ReSizeWnd();
	}
}

void QuestAttainStateWnd::ReSizeWnd()
{
	if( m_pQuestParent )
	{
		int iHeight = WND_HEIGHT_A + WND_HEIGHT_B + WND_HEIGHT_C;
		m_iHelpMentSize   = g_QuestMgr.GetMaxQuestHelp( m_pQuestParent, QS_ATTAIN ) * HELP_MENT_GAP;   // 멘트 사이즈
		m_iRewardSize     = m_pQuestParent->GetMaxRewardPresent() * REWARD_GAP;             // 선물 사이즈
		SetSize( GetWidth(), iHeight + m_iHelpMentSize + m_iRewardSize );
		SetWndPos( (Setting::Width() / 2) - (GetWidth() / 2), (Setting::Height() / 2) - (GetHeight() / 2) - 20 );

		ioWnd *pRewardBtn = FindChildWnd( ID_REWARD_BTN );
		if( pRewardBtn )
		{
			pRewardBtn->SetWndPos( pRewardBtn->GetXPos(), WND_HEIGHT_A + WND_HEIGHT_B + m_iHelpMentSize + m_iRewardSize + 10 );
		}
	}
}

void QuestAttainStateWnd::iwm_show()
{
	m_pQuestParent = g_QuestMgr.FirstQuestAttainList();
	if( m_pQuestParent )
	{
		if( m_pQuestParent->GetPerformType() == QP_EVENT )
		{
			m_pCurrentBackFrm     = m_pPinkBackFrm;
			m_pCurrentTitleFrm	  = m_pPinkTitleFrm;
			m_pCurrentBottomFrm	  = m_pPinkBottomFrm;
			m_pCurrentRewardOnFrm = m_pPinkRewardOnFrm;
		}
		else
		{
			m_pCurrentBackFrm     = m_pBrownBackFrm;
			m_pCurrentTitleFrm	  = m_pBrownTitleFrm;
			m_pCurrentBottomFrm	  = m_pBrownBottomFrm;
			m_pCurrentRewardOnFrm = m_pBrownRewardOnFrm;
		}

		// 수락 UI에 있는 리스트 삭제
		g_QuestMgr.EraseQuestOccurList( m_pQuestParent->GetMainIndex(), m_pQuestParent->GetSubIndex() );

		ReSizeWnd();
		ShowChildWnd( ID_ALPHA_WND );

		m_dwState = STATE_NONE;
		m_dwCompleteTime = 0;
		m_iQuestIconWhiteAlpha = 0;

		if( g_ExSoundMgr.IsSound( ExSound::EST_QUEST_COMPLETE ) )
		{
			g_ExSoundMgr.PlaySound( ExSound::EST_QUEST_COMPLETE );
		}
		else
		{
			if( !m_szShowSnd.IsEmpty() )
				g_SoundMgr.PlaySound( m_szShowSnd );
		}
	}
	else
	{
		HideWnd();
	}
}

void QuestAttainStateWnd::iwm_hide()
{
	m_pQuestParent = NULL;
	m_dwHideDelayTime = FRAMEGETTIME();
	HideChildWnd( ID_ALPHA_WND );

	AddWndStyle( IWS_MODAL );
}

bool QuestAttainStateWnd::iwm_spacebar()
{
	ioWnd *pAlphaWnd = FindChildWnd( ID_ALPHA_WND );
	if( pAlphaWnd && pAlphaWnd->IsShow() )
		return false;	

	iwm_command( FindChildWnd( ID_REWARD_BTN ), IOBN_BTNUP, 0 );
	return true;
}

void QuestAttainStateWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_REWARD_BTN:
		if( cmd == IOBN_BTNUP )
		{
			if( m_pQuestParent )
			{
				// 선물 확인
				if( !HasWndStyle( IWS_MODAL ) && g_PresentMgr.GetMaxPresent() > (int)g_PresentMgr.GetLimitPresentSize() )
				{
					if( !g_QuestMgr.IsAllDirectPresent( m_pQuestParent ) )
					{
						g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
						return;
					}
				}

				SP2Packet kPacket( CTPK_QUEST_REWARD );
				kPacket << m_pQuestParent->GetMainIndex() << m_pQuestParent->GetSubIndex() << m_pQuestParent->IsRewardSelectStyle();
				TCPNetwork::SendToServer( kPacket );
				TCPNetwork::MouseBusy( true );

				if( !m_szRewardSnd.IsEmpty() )
					g_SoundMgr.PlaySound( m_szRewardSnd );

				g_QuestMgr.EraseQuestAttainList( m_pQuestParent->GetMainIndex(), m_pQuestParent->GetSubIndex() );
				HideWnd();
			}
		}
		break;
	case ID_ALPHA_WND:
		if( cmd == IOWN_HIDE )
		{
			// 알파 끝!! Complete 애니 시작
			m_dwState = STATE_SCALE;
			m_dwCompleteTime = FRAMEGETTIME();
			m_iQuestIconWhiteAlpha = 0;
		}
		break;
	}
}

bool QuestAttainStateWnd::IsShowCheck()
{	
	if( g_GUIMgr.IsShow( QUEST_OCCUR_STATE_WND ) )
		return false;
	if( g_GUIMgr.IsShow( QUEST_ATTAIN_SELECT_STATE_BROWN_WND ) )
		return false;
	if( g_GUIMgr.IsShow( QUEST_ATTAIN_SELECT_STATE_PINK_WND ) )
		return false;
	QuestParent* quest = g_QuestMgr.FirstQuestAttainList();
	if ( !quest || quest->IsRewardSelectStyle() ) {
		return false;
	}
	return true;
}

void QuestAttainStateWnd::ProcessComplete()
{
	if( m_dwCompleteTime == 0 ) return;

	DWORD dwGapTime = FRAMEGETTIME() - m_dwCompleteTime;
	switch( m_dwState )
	{
	case STATE_SCALE:
		{
			if( dwGapTime <= FLOAT100 )
			{
				if( m_pCompleteText )
				{
					float fScalePer = FLOAT1 - (float)dwGapTime / FLOAT100;
					m_pCompleteText->SetScale( FLOAT1 + fScalePer );
				}
			}
			else
			{
				m_dwState = STATE_ALPHA;
				m_dwCompleteTime = FRAMEGETTIME();
				m_iQuestIconWhiteAlpha = 0;

				if( m_pCompleteText )
				{
					m_pCompleteText->SetScale( FLOAT1 );
				}
			}
		}
		break;
	case STATE_ALPHA:
		{
			if( dwGapTime <= 1000 )
			{
				float fAlphaPer = FLOAT1 - (float)dwGapTime / FLOAT1000;
				m_iQuestIconWhiteAlpha = max( 0, MAX_ALPHA_RATE * fAlphaPer );
			}
		}
		break;
	}
}

void QuestAttainStateWnd::OnProcess( float fTimePerSec )
{
	if( IsShow() )
	{
		if( IsShowCheck() )
		{
			if( !g_QuestMgr.FirstQuestAttainList()  )
			{
				HideWnd();      
			}
			ProcessComplete();
		}
		else
		{
			HideWnd();
		}		
	}
	else if( IsShowCheck() )
	{
		if( FRAMEGETTIME() - m_dwHideDelayTime < FLOAT100 )
			return;

		m_pQuestParent = g_QuestMgr.FirstQuestAttainList();
		if( m_pQuestParent )
		{
			// 선물 확인
			if( g_PresentMgr.GetMaxPresent() > (int)g_PresentMgr.GetLimitPresentSize() )
			{
				if( !g_QuestMgr.IsAllDirectPresent( g_QuestMgr.FirstQuestAttainList() ) )
					RemoveWndStyle( IWS_MODAL );
			}

			if( m_pQuestParent->GetPerformType() == QP_EVENT )
			{
				m_pCurrentBackFrm     = m_pPinkBackFrm;
				m_pCurrentTitleFrm	  = m_pPinkTitleFrm;
				m_pCurrentBottomFrm	  = m_pPinkBottomFrm;
				m_pCurrentRewardOnFrm = m_pPinkRewardOnFrm;
			}
			else
			{
				m_pCurrentBackFrm     = m_pBrownBackFrm;
				m_pCurrentTitleFrm	  = m_pBrownTitleFrm;
				m_pCurrentBottomFrm	  = m_pBrownBottomFrm;
				m_pCurrentRewardOnFrm = m_pBrownRewardOnFrm;
			}
			ShowWnd();          
		}
	}
}

bool QuestAttainStateWnd::OnRenderShowAndHide()
{
	if( !HasExWndStyle( IWS_EX_SHOW_HIDE_ACTION ) ) 
		return false;

	const DWORD dwCheckTime = 50;
	if( m_dwWndAniTime == 0 ) return false;
	if( FRAMEGETTIME() - m_dwWndAniTime > dwCheckTime )
	{
		m_dwWndAniTime = 0;
		if( m_pCurrentBackFrm )
		{
			m_pCurrentBackFrm->SetSize( GetWidth(), GetHeight() );
		}
		return false;
	}

	// 확대 & 축소를 20% ~ 100% 사이만 진행 시킨다.
	DWORD dwGapTime = FRAMEGETTIME() - m_dwWndAniTime;
	float fTimeRate = ( (float)dwGapTime / dwCheckTime ) * FLOAT08;
	if( IsShow() )
	{
		fTimeRate = min( FLOAT08, fTimeRate ) + 0.2f;
	}
	else
	{
		fTimeRate = max( 0.0f, FLOAT08 - fTimeRate ) + 0.2f;
	}

	if( m_pCurrentBackFrm )
	{
		int iWidth = GetWidth() * fTimeRate;
		int iHeight= GetHeight() * fTimeRate;
		int iXPos  = GetDerivedPosX() + ( ( GetWidth() - iWidth ) / 2 );
		int iYPos  = GetDerivedPosY() + ( ( GetHeight() - iHeight ) / 2 );
		m_pCurrentBackFrm->SetSize( iWidth, iHeight );
		m_pCurrentBackFrm->Render( iXPos, iYPos );
	}		

	return true;
}

void QuestAttainStateWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();
	if( m_pQuestParent )
	{
		if( m_pCurrentBackFrm )
		{
			m_pCurrentBackFrm->SetSize( GetWidth(), GetHeight() );
			m_pCurrentBackFrm->Render( iXPos, iYPos );
		}

		if( m_pCurrentTitleFrm )
		{
			m_pCurrentTitleFrm->SetSize( GetWidth() - 10, 36 );
			m_pCurrentTitleFrm->Render( iXPos, iYPos );
		}

		// 타이틀
		g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
		g_FontMgr.SetAlignType( TAT_CENTER );
		if( m_pQuestParent->GetPerformType() == QP_EVENT )
			g_FontMgr.SetBkColor( 186, 79, 65 );
		else
			g_FontMgr.SetBkColor( 75, 57, 35 );
		g_FontMgr.SetTextColor( 255, 255, 255 );
		g_FontMgr.PrintTextWidthCut( iXPos + GetWidth() / 2, iYPos + 11, FONT_SIZE_17, 330.0f, STR(1), m_pQuestParent->GetTitle().c_str() );	

		if( m_pCurrentBottomFrm )
		{
			m_pCurrentBottomFrm->SetSize( GetWidth(), WND_HEIGHT_C + m_iRewardSize + 13 );
			m_pCurrentBottomFrm->Render( iXPos, iYPos + WND_HEIGHT_A + m_iHelpMentSize + 10, UI_RENDER_MULTIPLY );
		}

		OnRenderIcon( iXPos + 171, iYPos + 90 );
		g_QuestMgr.RenderQuestCompleteHelp( m_pQuestParent, iXPos + 171, iYPos + 144 );
		OnRenderReward( iXPos + 17, iYPos + WND_HEIGHT_A + m_iHelpMentSize + WND_HEIGHT_B + 2 );		
	}
}

void QuestAttainStateWnd::OnRenderIcon( int iXPos, int iYPos )
{
	if( m_pQuestIconBack1 )
		m_pQuestIconBack1->Render( iXPos, iYPos, UI_RENDER_MULTIPLY );
	if( m_pQuestIconBack2 )
	{
		m_pQuestIconBack2->SetScale( FLOAT1 );
		m_pQuestIconBack2->Render( iXPos, iYPos, UI_RENDER_MULTIPLY );
	}

	ioUIRenderImage *pQuestIcon = m_pQuestParent->GetQuestIcon();
	if( pQuestIcon )
	{		
		pQuestIcon->Render( iXPos, iYPos );
	}

	// UI코스튬 관련 (코스튬 마크)
	if ( m_pQuestParent->IsCostume() )
	{
		if( m_pCostumeMark )
			m_pCostumeMark->Render( iXPos, iYPos );
	}

	if( m_dwState != STATE_NONE )
	{
		if( pQuestIcon )
		{
			pQuestIcon->SetColor( TCT_DEFAULT_WHITE );
			pQuestIcon->SetAlpha( (BYTE)m_iQuestIconWhiteAlpha );
			pQuestIcon->Render( iXPos, iYPos, UI_RENDER_NORMAL_ADD_COLOR );
		}

		if( m_pCompleteText )
			m_pCompleteText->Render( iXPos, iYPos );
	}
}

void QuestAttainStateWnd::OnRenderReward( int iXPos, int iYPos )
{
	for(int i = 0;i < m_pQuestParent->GetMaxRewardPresent();i++)
	{
		DWORD dwRewardIndex = m_pQuestParent->GetRewardPresentIndex( i );			
		bool bPresentDirect = false;
		int iPresentType, iPresentValue1, iPresentValue2;
		g_QuestMgr.GetPresentData( dwRewardIndex, iPresentType, iPresentValue1, iPresentValue2, bPresentDirect );
		
		if( iPresentType == 0 ) continue;

		if( m_pCurrentRewardOnFrm )
			m_pCurrentRewardOnFrm->Render( iXPos, iYPos );
		ioUIRenderImage *pRewardIcon = GetRewardIcon( iPresentType, iPresentValue1, iPresentValue2 );
		if( m_pQuestIconBack2 && pRewardIcon )
		{
			m_pQuestIconBack2->SetScale( FLOAT05 );
			m_pQuestIconBack2->Render( iXPos + 23, iYPos + 21, UI_RENDER_MULTIPLY, TFO_BILINEAR );
			pRewardIcon->SetScale( FLOAT05 );
			pRewardIcon->Render( iXPos + 23, iYPos + 21, UI_RENDER_NORMAL, TFO_BILINEAR );

			// UI코스튬 관련 (코스튬 마크)
			if ( ( iPresentType == PRESENT_COSTUME || iPresentType == PRESENT_COSTUME_BOX ) && m_pCostumeMark )
			{
				m_pCostumeMark->SetScale( 0.50f );
				m_pCostumeMark->Render( iXPos + 23, iYPos + 21, UI_RENDER_NORMAL, TFO_BILINEAR );
			}
		}

		char szPresentName[MAX_PATH] = "";
		if( iPresentType == PRESENT_PESO )
			sprintf_e( szPresentName, "%s", g_PresentMgr.GetPresentValue2Text( iPresentType, iPresentValue1, iPresentValue2 ).c_str() );
		else
		{
			if( iPresentType == PRESENT_MEDALITEM )
				SafeSprintf( szPresentName, sizeof( szPresentName ), STR(1), g_PresentMgr.GetPresentValue1Text( iPresentType, iPresentValue1, iPresentValue2 ).c_str(), g_PresentMgr.GetPresentValue2Text( iPresentType, iPresentValue1, iPresentValue2 ).c_str() );
			else
				sprintf_e( szPresentName, "%s %s", g_PresentMgr.GetPresentValue1Text( iPresentType, iPresentValue1, iPresentValue2 ).c_str(), g_PresentMgr.GetPresentValue2Text( iPresentType, iPresentValue1, iPresentValue2 ).c_str() );
		}

		g_FontMgr.SetTextStyle( TS_NORMAL );
		g_FontMgr.SetAlignType( TAT_LEFT );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
		g_FontMgr.PrintText( iXPos + 46, iYPos + 15, FONT_SIZE_13, szPresentName );	

		iYPos += REWARD_GAP;
	}
}

//////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------
/*
생성, 파괴
*/
QuestAttainSelectStateWnd::QuestAttainSelectStateWnd()
	: m_pQuestParent( NULL )
	, m_sunwndposx( 0 )
	, m_sunwndposy( 0 )
	, m_hidedelaytime ( 0 )
	, m_rewardselectnum( 0 )
{
	m_pCostumeMark = NULL;
	m_RewardIDList.clear();
}

QuestAttainSelectStateWnd::~QuestAttainSelectStateWnd()
{
	SAFEDELETE( m_pCostumeMark );
	DeleteRewardInfos();
}

//----------------------------------------------------------------
/*
Reward
*/
void	QuestAttainSelectStateWnd::DeleteRewardInfos()
{
	QuestRewardVec::iterator iter = m_rewardinfos.begin();
	QuestRewardVec::iterator eiter = m_rewardinfos.end();
	for ( ; iter != eiter; ++iter ) {
		SAFEDELETE( *iter );
	}
	m_rewardinfos.clear();

	m_RewardIDList.clear();
}

void	QuestAttainSelectStateWnd::FillRewardInfos()
{
	DeleteRewardInfos();

	if ( !m_pQuestParent ) {
		return;
	}

	for ( int i = 0; i < m_pQuestParent->GetMaxRewardPresent(); ++i )
	{
		DWORD index = m_pQuestParent->GetRewardPresentIndex( i );
		bool presentdirect = false;
		int presenttype, presentvalue1, presentvalue2;
		bool bIsCostume = false;
		g_QuestMgr.GetPresentData( index, presenttype, presentvalue1, presentvalue2, presentdirect );

		ioWnd* togglewnd = FindChildWnd( REWARD_WND_0 + i );
		ioUIRenderImage* icon = 0;
		if ( presenttype != 0 ) {
			icon = GetRewardIcon( presenttype, presentvalue1, presentvalue2 );
		}		

		char szPresentName[MAX_PATH] = "";
		if ( presenttype == PRESENT_PESO )
		{
			sprintf_e( szPresentName, "%s", g_PresentMgr.GetPresentValue2Text( presenttype, presentvalue1, presentvalue2 ).c_str() );
		}
		else
		{
			if ( presenttype == PRESENT_MEDALITEM ) 
			{
				SafeSprintf( szPresentName, sizeof( szPresentName ), STR(1),
					g_PresentMgr.GetPresentValue1Text( presenttype, presentvalue1, presentvalue2 ).c_str(),
					g_PresentMgr.GetPresentValue2Text( presenttype, presentvalue1, presentvalue2 ).c_str() 
				);
			}
			else
			{
				sprintf_e( szPresentName, "%s %s",
					g_PresentMgr.GetPresentValue1Text( presenttype, presentvalue1, presentvalue2 ).c_str(),
					g_PresentMgr.GetPresentValue2Text( presenttype, presentvalue1, presentvalue2 ).c_str()
				);
			}
		}	

		if ( presenttype == PRESENT_COSTUME || presenttype == PRESENT_COSTUME_BOX )
			bIsCostume = true;

		m_rewardinfos.push_back( new QuestRewardInfo( icon, togglewnd, szPresentName, bIsCostume ) );
	}
}

ioUIRenderImage*	QuestAttainSelectStateWnd::GetRewardIcon( int iPresentType, int iPresentValue1, int iPresentValue2 )
{
	ioHashString szIconName;
	switch( iPresentType )
	{
	case PRESENT_SOLDIER:
		{
			szIconName = g_MyInfo.GetMySoldierIconName( iPresentValue1 );
		}
		break;

	case PRESENT_DECORATION:
		{
			int iClassType = iPresentValue1 / 100000;
			int iSexType   = (iPresentValue1 % 100000) / FLOAT1000;
			int iDecoType  = iPresentValue1 % 1000;
			const DecoData &kData = g_DecorationPrice.GetDecoData( iSexType, iDecoType, iPresentValue2 );
			szIconName = kData.m_szIcon;
		}
		break;

	case PRESENT_ETC_ITEM:
		{
			ioEtcItem *pEtcItem = g_EtcItemMgr.FindEtcItem( iPresentValue1 );
			if ( pEtcItem ) {
				szIconName = pEtcItem->GetIconName();
			}
		}
		break;

	case PRESENT_PESO:
		{
			szIconName = "UIIconPack13#quest_002";
		}
		break;

	case PRESENT_EXTRAITEM:
	case PRESENT_ACCESSORY:
		{
			const ioItem *pItem = g_ItemMaker.GetItemConst( iPresentValue1, __FUNCTION__ );
			if ( pItem ) {
				szIconName = pItem->GetItemLargeIconName();
			}
		}
		break;

	case PRESENT_EXTRAITEM_BOX:
		{
			RandomMachineInfo *pInfo = g_ExtraItemInfoMgr.GetRandomMachineInfo( iPresentValue1 );
			if ( pInfo ) {
				szIconName = pInfo->GetIconName();
			}
		}
		break;

	case PRESENT_RANDOM_DECO:
		{
			int iClassType = iPresentValue1 / 100000;
			int iSexType   = (iPresentValue1 % 100000) / FLOAT1000;
			int iDecoType  = iPresentValue1 % 1000;
			const DecoData &kData = g_DecorationPrice.GetDecoData( iSexType, iDecoType, iPresentValue2 );
			szIconName = kData.m_szIcon;
		}
		break;

	case PRESENT_GRADE_EXP:
		{
			szIconName = "UIIconPack13#quest_001";
		}
		break;

	case PRESENT_MEDALITEM:
		{
			const ioMedalItemInfoManager::ItemInfo *pItem = g_MedalItemMgr.GetItemInfo( iPresentValue1 );
			if ( pItem ) {
				szIconName = pItem->m_sIcon;
			}
		}
		break;

	case PRESENT_ALCHEMIC_ITEM:
		{
			szIconName = g_AlchemicMgr.GetAlchemicItemIcon( iPresentValue1 );
		}
		break;

	case PRESENT_PET:
		{
			int nPetRank = iPresentValue2%10000;
			szIconName = g_PetInfoMgr.GetPetIcon( iPresentValue1, (PetRankType)nPetRank );
		}
		break;

	case PRESENT_COSTUME:
		{
			// UI코스튬 관련 (퀘스트 아이콘)
			szIconName = g_CostumeInfoMgr.GetCostumeIcon( iPresentValue1 );
		}
		break;

	case PRESENT_COSTUME_BOX:
		{
			CostumeShopInfo *pInfo = g_CostumeShopInfoMgr.GetCostumeMachineInfo( iPresentValue1 );
			if ( pInfo )
				szIconName = pInfo->GetIconName();
		}
		break;

	case PRESENT_BONUS_CASH:
		{
			szIconName = "UIIconPack101#Cash";
		}
		break;
	}

	if ( szIconName.IsEmpty() ) {
		return NULL;
	}

	return g_UIImageSetMgr.CreateImageByFullName( szIconName );
}

//----------------------------------------------------------------
/*
ioWnd override
*/
void	QuestAttainSelectStateWnd::Resize()
{
	if ( !m_pQuestParent ) {
		return;
	}

	static const int HEIGHT_GAP_MENT_TEXT = 19;	// 맨트 한줄한줄 사이의 공간 크기
	static const int HEIGHT_GAP_REWARD = 45;	// 보상 한개의 높이

	// 멘트높이를 구한후에 영향받는 윈도우들을 전부 멘트높이만큼 아래로 내린다.
	int mentheight = g_QuestMgr.GetMaxQuestHelp( m_pQuestParent, QS_ATTAIN ) * HEIGHT_GAP_MENT_TEXT;

	// 보상 높이를 구한다.
	int rewardheight = m_pQuestParent->GetMaxRewardPresent() * HEIGHT_GAP_REWARD;
			
	// 우선 본체의 높이를 조절한다.
	SetSize( 340, 226 + mentheight + rewardheight + 25 );
	SetWndPos( ( Setting::Width() / 2 ) - ( GetWidth() / 2 ), ( Setting::Height() / 2 ) - ( GetHeight() / 2 ) - 20 );
		
	// 멘트에 영향받는 윈도우 위치를 조절한다.
	int initposy[ CHILD_MAX - BOTTOM_MULTY ] =
	{
		155,	170,	195,	240,	285,
		330,	375,	204,	249,	294,
		339,	384,	258
	};
	for ( int i = BOTTOM_MULTY; i < CHILD_MAX; ++i )
	{
		ioWnd* wnd = FindChildWnd( i );
		if ( wnd ) {
			wnd->SetWndPos( wnd->GetXPos(), initposy[ i - BOTTOM_MULTY ] + mentheight );
		}
	}

	// 배경윈도우의 Size를 길게~
	ioWnd* backwnd = FindChildWnd( BACKGROUND_IMAGE );
	if ( backwnd ) {
		backwnd->SetSize( GetWidth(), GetHeight() );
	}
	ioWnd* backbottomwnd = FindChildWnd( BOTTOM_MULTY );
	if ( backbottomwnd && backwnd ) {
		backbottomwnd->SetSize( backwnd->GetWidth(), backwnd->GetHeight() - backbottomwnd->GetYPos() );
	}
	//보상 받기 버튼 위치를 조정한다.
	ioWnd* acceptmwnd = FindChildWnd( SPACE_BUTTON );
	if ( acceptmwnd && backwnd ) {
		acceptmwnd->SetWndPos( acceptmwnd->GetXPos(), backwnd->GetHeight() - acceptmwnd->GetHeight() - 17 );
	}
}

//----------------------------------------------------------------
/*
ioWnd override
*/
void QuestAttainSelectStateWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "costume_image" )
	{
		SAFEDELETE( m_pCostumeMark );
		m_pCostumeMark = pImage;
	}
	else
		ioWnd::AddRenderImage( szType, pImage );
}

void	QuestAttainSelectStateWnd::iwm_show()
{
	m_pQuestParent = g_QuestMgr.FirstQuestAttainList();
	//m_pQuestParent = g_QuestMgr.GetQuest( rand() % g_QuestMgr.GetNumQuest() );
	if ( !m_pQuestParent )
	{
		HideWnd();
		return;
	}

	// 수락 UI에 있는 리스트 삭제
	g_QuestMgr.EraseQuestOccurList( m_pQuestParent->GetMainIndex(), m_pQuestParent->GetSubIndex() );
	
	// 멘트에 따른 Size조절
	Resize();

	// 보상 정보를 채운다.
	FillRewardInfos();

	// 퀘스트 Title Text를 구해놓는다. ( STR의 속도 문제로 여기서 미리 구해놓아야함 )
	char szPresentName[MAX_PATH] = "";
	SafeSprintf( szPresentName, sizeof( szPresentName ), STR(1), m_pQuestParent->GetTitle().c_str() );
	m_questtitle = szPresentName;

	// 보상 선택 설명 text를 셋팅한다. ( FindChildWnd의 속도 문제로 여기서 미리 구해놓아야함 )
	ioWnd* rewarddescwnd = FindChildWnd( REWARD_DESC_BACK );
	if ( rewarddescwnd && rewarddescwnd->GetTitle() )
	{
		char buff[ MAX_PATH ] = "";
		SafeSprintf( buff, sizeof(buff), STR(2), m_pQuestParent->GetRewardSelectNum() );
		rewarddescwnd->GetTitle()->SetText( buff, "" );
	} 

	// 보상 토글버튼들을 보상갯수에 따라 show / hide 시킨다.
	m_rewardselectnum = 0;
	int nummaxreward = m_pQuestParent->GetMaxRewardPresent();
	for ( int i = REWARD_WND_0; i <= REWARD_WND_4; ++i, --nummaxreward )
	{
		ioWnd* togglewnd = FindChildWnd( i );
		ioCheckButton* checkwnd = (ioCheckButton*)( FindChildWnd( i + 5 ) );
		if ( nummaxreward > 0 ) 
		{
			if ( togglewnd ) {
				togglewnd->ShowWnd();
			}
			if ( checkwnd ) 
			{
				checkwnd->SetActive();
				checkwnd->RemoveWndStyle( IWS_NO_MOUSE_RESPONSE );
				checkwnd->SetCheck( false );
				checkwnd->ShowWnd();
			}
		}
		else 
		{
			if ( togglewnd ) {
				togglewnd->HideWnd();
			}
			if ( checkwnd ) {
				checkwnd->HideWnd();
			}
		}
	}

	// Alpha Effect 효과를 줄 윈도우를 그린다.
	ShowChildWnd( ALPHA_EFFECT );

	// find child가 겁나 느려서 여기서 저장해놓는다.
	ioWnd* sunwnd = FindChildWnd( SUN_IMAGE );
	if ( sunwnd )
	{
		m_sunwndposx = sunwnd->GetDerivedPosX();
		m_sunwndposy = sunwnd->GetDerivedPosY();
	}
	
	// 퀘스트 아이콘
	ioWnd* questiconwnd = FindChildWnd( QUEST_ICON );
	ioUIRenderImage* questiconeffect = NULL;
	if ( questiconwnd )
	{
		questiconwnd->ClearRenderImageList();
		ioUIRenderImage* image = m_pQuestParent->GetQuestIcon();
		if ( image )
		{
			questiconeffect = (ioUIRenderImage*)( image->Clone() );
			questiconwnd->AddRenderImage( "None", (ioUIRenderImage*)( image->Clone() ) );
			questiconwnd->AddRenderImage( "None", questiconeffect );
		}

		// UI코스튬 관련 (코스튬 마크)
		if ( m_pQuestParent->IsCostume() )
			questiconwnd->AddRenderImage( "None", dynamic_cast<ioUIRenderImage*>( m_pCostumeMark->Clone() ) );
	}

	// complete 이미지 효과
	{
		m_animatorscale.ClearFrames();

		RenderElementVec elements;
		ioWnd* wnd = FindChildWnd( COMPLETE_TEXT );
		if ( wnd )
		{
			int num = wnd->GetNumRenderElement();
			for ( int i = 0; i < num; ++i ) {
				elements.push_back( wnd->GetRenderElement( i ) );
			}

			m_animatorscale.AppendFrame( new ioRenderElementAnimationFrameScale( elements, D3DXVECTOR2( 1.7f, 1.7f ), 0 ) );
			m_animatorscale.AppendFrame( new ioRenderElementAnimationFrameScale( elements, D3DXVECTOR2( FLOAT1, FLOAT1 ), FLOAT100 ) );
			if ( questiconeffect )
			{
				questiconeffect->SetAlpha( 0x00 );
				questiconeffect->SetColor( TCT_DEFAULT_WHITE );
				questiconeffect->SetRenderType( UI_RENDER_NORMAL_ADD_COLOR );
				m_animatorscale.AppendFrame( new ioRenderElementAnimationFrameAlpha( questiconeffect, 255, 0 ) );
				m_animatorscale.AppendFrame( new ioRenderElementAnimationFrameAlpha( questiconeffect, 0, 1000 ) );
			}
		}
	}

	// show / hide 시 줄효과
	{
		m_animate_showscaler.ClearFrames();
		m_animate_hidescaler.ClearFrames();

		RenderElementVec elements;
		ioWnd* wnd = FindChildWnd( BACKGROUND_IMAGE );
		if ( wnd )
		{
			int num = wnd->GetNumRenderElement();
			for ( int i = 0; i < num; ++i ) {
				elements.push_back( wnd->GetRenderElement( i ) );
			}

			m_animate_showscaler.AppendFrame( new ioRenderElementAnimationFrameSize( elements, D3DXVECTOR2( GetWidth() * 0.2f, GetHeight() * 0.2f ), 0 ) );
			m_animate_showscaler.AppendFrame( new ioRenderElementAnimationFrameSize( elements, D3DXVECTOR2( GetWidth(), GetHeight() ), FLOAT100 ) );

			m_animate_hidescaler.AppendFrame( new ioRenderElementAnimationFrameSize( elements, D3DXVECTOR2( GetWidth(), GetHeight() ), 0 ) );
			m_animate_hidescaler.AppendFrame( new ioRenderElementAnimationFrameSize( elements, D3DXVECTOR2( GetWidth() * 0.2f, GetHeight() * 0.2f ), FLOAT100 ) );
		}
		m_animate_hidescaler.Stop();
		m_animate_showscaler.Play( false );
	}
	
	if( g_ExSoundMgr.IsSound( ExSound::EST_QUEST_COMPLETE ) )
	{
		g_ExSoundMgr.PlaySound( ExSound::EST_QUEST_COMPLETE );
	}
	else
	{
		// 사운드 출력	
		if( !m_showsoundname.IsEmpty() ) {
			g_SoundMgr.PlaySound( m_showsoundname );
		}
	}

	m_RewardIDList.clear();
}

void	QuestAttainSelectStateWnd::iwm_hide()
{
	m_pQuestParent = NULL;

	m_rewardselectnum = 0;

	m_hidedelaytime = FRAMEGETTIME();
	
	m_RewardIDList.clear();

	HideChildWnd( ALPHA_EFFECT );
	m_animate_showscaler.Stop();
	m_animate_hidescaler.Play( false );

	AddWndStyle( IWS_MODAL );
}

bool	QuestAttainSelectStateWnd::iwm_spacebar()
{
	// 이런게 왜 있을까...
	ioWnd* effectwnd = FindChildWnd( ALPHA_EFFECT );
	if ( effectwnd && effectwnd->IsShow() ) {
		return false;
	}

	iwm_command( FindChildWnd( SPACE_BUTTON ), IOBN_BTNUP, 0 );
	return true;
}

void	QuestAttainSelectStateWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	switch ( pWnd->GetID() )
	{
	case REWARD_CHECK_0:
	case REWARD_CHECK_1:
	case REWARD_CHECK_2:
	case REWARD_CHECK_3:
	case REWARD_CHECK_4:
		CheckButton( pWnd, cmd );
		break;

	case SPACE_BUTTON:
		if ( cmd == IOBN_BTNUP )
		{
			if ( m_pQuestParent )
			{
				// 선물함 다찼는지 확인
				if ( !HasWndStyle( IWS_MODAL ) && g_PresentMgr.GetMaxPresent() > (int)g_PresentMgr.GetLimitPresentSize() )
				{
					if ( !g_QuestMgr.IsAllDirectPresent( m_pQuestParent ) )
					{
						g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
						return;
					}
				}

				// 선택한 갯수 체크
				std::vector< BYTE > selvec;
				int rewardnum = m_pQuestParent->GetRewardSelectNum();
				int numsel = 0;
				for ( int i = REWARD_CHECK_0; i <= REWARD_CHECK_4; ++i )
				{
					ioCheckButton* checkwnd = (ioCheckButton*)( FindChildWnd( i ) );
					if ( !checkwnd || !checkwnd->IsShow() ) {
						continue;
					}
					if ( checkwnd->IsChecked() )
					{
						++numsel;
						selvec.push_back( static_cast<BYTE>( i - REWARD_CHECK_0 ) );
					}
				}
				if ( numsel < rewardnum )
				{
					char msg[ MAX_PATH ] = "";
					SafeSprintf( msg, sizeof(msg), STR(2), numsel, rewardnum );
					g_GUIMgr.SetMsgBox( MB_OK, NULL, msg );
					return;
				}
				else if ( numsel > rewardnum )
				{
					char msg[ MAX_PATH ] = "";
					SafeSprintf( msg, sizeof(msg), STR(3), numsel, rewardnum );
					g_GUIMgr.SetMsgBox( MB_OK, NULL, msg );
					return;
				}
				
				// 완료 패킷 보냄 ( 선택한 보상의 인덱스들을 보내준다. )
				SP2Packet kPacket( CTPK_QUEST_REWARD );
				kPacket << m_pQuestParent->GetMainIndex() << m_pQuestParent->GetSubIndex() << true << numsel;
				std::vector< BYTE >::iterator iter = selvec.begin();
				std::vector< BYTE >::iterator eiter = selvec.end();
				for ( ; iter != eiter; ++iter ) {
					kPacket << *iter;
				}
				TCPNetwork::SendToServer( kPacket );
				TCPNetwork::MouseBusy( true );
				
				// 달성 목록에서 지움
				g_QuestMgr.EraseQuestAttainList( m_pQuestParent->GetMainIndex(), m_pQuestParent->GetSubIndex() );
				m_pQuestParent = NULL;

				// 사운드 출력
				if ( !m_rewardsoundname.IsEmpty() ) {
					g_SoundMgr.PlaySound( m_rewardsoundname );
				}

				// 윈도우 숨김
				HideWnd();
			}
		}
		break;
	case ALPHA_EFFECT:
		if ( cmd == IOWN_HIDE )
		{
			// 알파 효과가 끝났으니 이제 Complete 글씨 이미지 효과를 줄 차례다.
			ShowChildWnd( COMPLETE_TEXT );
			m_animatorscale.Play( false );
		}
		break;
	}
}

void QuestAttainSelectStateWnd::CheckButton( ioWnd *pWnd, int cmd  )
{
	if ( m_pQuestParent && cmd == IOBN_BTNUP )
	{
		DWORD dwCheckWndID = pWnd->GetID();
		int iRewardMaxNum  = m_pQuestParent->GetRewardSelectNum();
		ioCheckButton* checkwnd = dynamic_cast<ioCheckButton*>( pWnd );
		if ( checkwnd )
		{
			if ( checkwnd->IsChecked() ) 
			{
				++m_rewardselectnum;
				m_RewardIDList.push_back( checkwnd->GetID() );
			} 
			else 
			{
				--m_rewardselectnum;
				if ( !m_RewardIDList.empty() )
				{
					DWORDVec::const_iterator iter = m_RewardIDList.begin();
					DWORDVec::const_iterator eiter = m_RewardIDList.end();
					for ( ; iter != eiter; ++iter ) 
					{
						if( (*iter) == checkwnd->GetID() )
						{
							checkwnd->SetCheck( false );
							m_RewardIDList.erase( iter );
						}
					}
				}
			}
		} 

		if ( m_rewardselectnum > iRewardMaxNum )
		{
			DWORDVec::const_iterator iter = m_RewardIDList.begin();
			if ( iter != m_RewardIDList.end() )
			{
				ioCheckButton* checkwnd = dynamic_cast<ioCheckButton*>( FindChildWnd( (*iter) ) );
				if ( checkwnd && checkwnd->IsShow() ) 
				{
					if ( checkwnd->IsChecked() )
					{
						checkwnd->SetCheck( false );
						m_RewardIDList.erase( iter );
						--m_rewardselectnum;
					}
				}
			}
			
		}
	}
	/*
	//기존 소스
	if ( m_pQuestParent && cmd == IOBN_BTNUP )
	{
		ioCheckButton* checkwnd = dynamic_cast<ioCheckButton*>( pWnd );
		if ( checkwnd )
		{
			if ( checkwnd->IsChecked() ) {
				++m_rewardselectnum;
			} else {
				--m_rewardselectnum;
			}
		}
		if ( m_rewardselectnum == m_pQuestParent->GetRewardSelectNum() )
		{
			for ( int i = REWARD_CHECK_0; i < REWARD_CHECK_4 + 1; ++i )
			{
				ioCheckButton* check = dynamic_cast<ioCheckButton*>( FindChildWnd( i ) );
				if ( !check || !check->IsShow() ) {
					continue;
				}
				if ( !check->IsChecked() ) 
				{
					check->SetInActive();
					check->AddWndStyle( IWS_NO_MOUSE_RESPONSE );
				}
			}
		}
		else
		{
			for ( int i = REWARD_CHECK_0; i < REWARD_CHECK_4 + 1; ++i )
			{
				ioCheckButton* check = dynamic_cast<ioCheckButton*>( FindChildWnd( i ) );
				if ( !check || !check->IsShow() ) {
					continue;
				}
				check->SetActive();
				check->RemoveWndStyle( IWS_NO_MOUSE_RESPONSE );
			}
		}
	}
	*/
}

bool	QuestAttainSelectStateWnd::IsShowCheck()
{	
	if ( g_GUIMgr.IsShow( QUEST_OCCUR_STATE_WND ) ) {
		return false;
	}
	QuestParent* quest = g_QuestMgr.FirstQuestAttainList();
	if ( !quest || !quest->IsRewardSelectStyle() ) {
		return false;
	}
	if ( GetID() == QUEST_ATTAIN_SELECT_STATE_PINK_WND ) {
		if ( quest->GetPerformType() != QP_EVENT || g_GUIMgr.IsShow( QUEST_ATTAIN_SELECT_STATE_BROWN_WND ) ) {
			return false;
		}
	} else if ( GetID() == QUEST_ATTAIN_SELECT_STATE_BROWN_WND ) {
		if ( quest->GetPerformType() != QP_NORMAL || g_GUIMgr.IsShow( QUEST_ATTAIN_SELECT_STATE_PINK_WND ) ) {
			return false;
		}
	}
	return true;
}

void	QuestAttainSelectStateWnd::OnProcess( float fTimePerSec )
{
	ioWnd::OnProcess( fTimePerSec );

	m_animatorscale.Update();
	m_animate_showscaler.Update();
	m_animate_hidescaler.Update();

	ProcessAutoShowHide( fTimePerSec );
}

void	QuestAttainSelectStateWnd::ProcessAutoShowHide( float fTimePerSec )
{
	if ( IsShow() )
	{
		if ( !IsShowCheck() ) {
			HideWnd();
		}
	}
	else if ( IsShowCheck() )
	{
		if ( FRAMEGETTIME() - m_hidedelaytime < FLOAT100 ) {
			return;
		}

		m_pQuestParent = g_QuestMgr.FirstQuestAttainList();
		if ( m_pQuestParent )
		{
			// 선물 확인
			if ( g_PresentMgr.GetMaxPresent() > (int)g_PresentMgr.GetLimitPresentSize() ) {
				if ( !g_QuestMgr.IsAllDirectPresent( g_QuestMgr.FirstQuestAttainList() ) ) {
					RemoveWndStyle( IWS_MODAL );
				}
			}
			ShowWnd();          
		}
	}
}

bool	QuestAttainSelectStateWnd::OnRenderShowAndHide()
{
	if ( !HasExWndStyle( IWS_EX_SHOW_HIDE_ACTION ) ) {
		return false;
	}	

	if ( ( IsShow() && m_animate_showscaler.IsPlaying() )
		||
		( !IsShow() && m_animate_hidescaler.IsPlaying() ) )
	{
		ioWnd* wnd = FindChildWnd( BACKGROUND_IMAGE );
		if ( wnd )
		{
			int num = wnd->GetNumRenderElement();
			for ( int i = 0; i < num; ++i ) 
			{
				ioUIRenderElement* elem = wnd->GetRenderElement( i );

				D3DXVECTOR2 size = elem->GetSize();
				float x = GetDerivedPosX() + ( GetWidth() * FLOAT05 ) - ( size.x * FLOAT05 );
				float y = GetDerivedPosY() + ( GetHeight() * FLOAT05 ) - ( size.y * FLOAT05 );

				elem->Render( x, y );
			}
		}
		return true;
	}

	return false;
}

void	QuestAttainSelectStateWnd::OnRenderAfterChild()
{
	ioWnd::OnRenderAfterChild();
		
	if ( !m_pQuestParent ) {
		return;
	}

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	// 타이틀
	g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
	g_FontMgr.SetAlignType( TAT_CENTER );
	if ( m_pQuestParent->GetPerformType() == QP_EVENT ) {
		g_FontMgr.SetBkColor( 186, 79, 65 );
	} else {
		g_FontMgr.SetBkColor( 75, 57, 35 );
	}
	g_FontMgr.SetTextColor( 255, 255, 255 );
	g_FontMgr.PrintTextWidthCut( iXPos + GetWidth() / 2, iYPos + 11, FONT_SIZE_17, 330.0f, m_questtitle.c_str() );
	
	// 퀘스트 설명
	g_QuestMgr.RenderQuestCompleteHelp( m_pQuestParent, iXPos + 171, iYPos + 144 );

	// 보상 icon 및 text들 출력
	RenderReward();	
}

void	QuestAttainSelectStateWnd::RenderReward()
{
	QuestRewardVec::iterator iter = m_rewardinfos.begin();
	QuestRewardVec::iterator eiter = m_rewardinfos.end();
	for ( ; iter != eiter; ++iter )
	{
		if ( (*iter)->m_rewardwnd && (*iter)->m_icon )
		{
			int x = (*iter)->m_rewardwnd->GetDerivedPosX();
			int y = (*iter)->m_rewardwnd->GetDerivedPosY();
			int width = (*iter)->m_rewardwnd->GetWidth();
			int height = (*iter)->m_rewardwnd->GetHeight();

			int w = width * FLOAT05;
			
			(*iter)->m_icon->SetScale( FLOAT05 );
			(*iter)->m_icon->Render( x + 23, y + 21, UI_RENDER_NORMAL, TFO_BILINEAR );

			// UI코스튬 관련 (코스튬 마크)
			if ( (*iter)->m_bIsCostume && m_pCostumeMark )
			{
				m_pCostumeMark->SetScale( 0.5f );
				m_pCostumeMark->Render( x + 23, y + 21, UI_RENDER_NORMAL, TFO_BILINEAR );
			}

			g_FontMgr.SetTextStyle( TS_NORMAL );
			g_FontMgr.SetAlignType( TAT_LEFT );
			g_FontMgr.SetBkColor( 0, 0, 0 );
			g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
			g_FontMgr.PrintTextWidthCut( x + 46, y + 15, FONT_SIZE_13, 220.0f, (*iter)->m_text.c_str() );
		}
	}
}

void	QuestAttainSelectStateWnd::SetPositionToDefault( int iParentWidth, int iParentHeight, bool bChildMove, bool bCreate )
{
	ioWnd::SetPositionToDefault( iParentWidth, iParentHeight, bChildMove, bCreate );

	if ( !bCreate ) {
		Resize();
	}
}

void	QuestAttainSelectStateWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	m_showsoundname = xElement.GetStringAttribute_e( "ShowSound" );
	m_rewardsoundname = xElement.GetStringAttribute_e( "RewardSound" );
}

//////////////////////////////////////////////////////////////////////////
QuestProgressInfoWnd::QuestProgressInfoWnd()
{
	m_iMinusRewardPassH = 0;
	m_iAddMaxHeight = 0;
	m_pBottomFrm    = NULL;
	m_pStatePurposeFrm = NULL;
}

QuestProgressInfoWnd::~QuestProgressInfoWnd()
{
	SAFEDELETE( m_pBottomFrm );
	SAFEDELETE( m_pStatePurposeFrm );
}


void QuestProgressInfoWnd::AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame )
{
	if( pFrame )
	{
		if( szType == "BottomFrm" )
		{
			SAFEDELETE( m_pBottomFrm );
			m_pBottomFrm = pFrame;
		}
		else if( szType == "StatePurposeFrm" )
		{
			SAFEDELETE( m_pStatePurposeFrm );
			m_pStatePurposeFrm = pFrame;
		}
		else
		{	
			QuestInfoParentWnd::AddRenderFrame( szType, pFrame );
		}
	}
}

void QuestProgressInfoWnd::ReSizeWnd( int iWidth, int iHeight )
{
	SetSize( GetWidth(), (MAXIMUM_H-m_iMinusRewardPassH) + m_iAddMaxHeight );

	if( GetParent() )
	{
		GetParent()->SetSize( GetWidth(), GetHeight() );

		RECT  rcDrag = { 0, 0, GetWidth(), GetHeight() };
		GetParent()->SetDragRect( rcDrag );
	}
}

void QuestProgressInfoWnd::SetPositionToDefault( int iParentWidth, int iParentHeight, bool bChildMove, bool bCreate /* = false  */ )
{
	QuestInfoParentWnd::SetPositionToDefault( iParentWidth, iParentHeight, bChildMove, bCreate );
	if( !bCreate )
	{
		ReSizeWnd( GetWidth(), (MAXIMUM_H-m_iMinusRewardPassH) + m_iAddMaxHeight );
	}
}

void QuestProgressInfoWnd::iwm_show()
{
	m_dwQuestState = 0xFFFFFFFF;
	if( m_pQuestParent )
	{
		// 퀘스트 명
		SetTitleText( m_pQuestParent->GetTitle().c_str() );

		m_iCurPos = 0;
		ioScroll *pScroll = (ioScroll*)FindChildWnd( ID_VERT_SCROLL );
		if( pScroll )
		{
			pScroll->SetScrollRange( m_iCurPos, g_QuestMgr.GetMaxQuestHelp( m_pQuestParent, QS_PROGRESS ) );
			pScroll->SetScrollPos( m_iCurPos );
		}
		
		m_iMinusRewardPassH = 0;
		if( m_pQuestParent->GetMaxRewardPresent() == 0 )
			m_iMinusRewardPassH = REWARD_PASS_H;

		m_iAddMaxHeight = g_QuestMgr.GetMaxQuestPurposeHelp( m_pQuestParent ) * PURPOSE_GAP;
		ReSizeWnd( GetWidth(), (MAXIMUM_H - m_iMinusRewardPassH) + m_iAddMaxHeight );

		ioQuest *pQuest = g_MyInfo.GetQuest();
		if( pQuest )
		{
			if( pQuest->IsQuestIndexCheck( m_pQuestParent->GetMainIndex(), m_pQuestParent->GetSubIndex() ) )
			{
				m_dwQuestState = pQuest->GetQuestState( m_pQuestParent->GetMainIndex(), m_pQuestParent->GetSubIndex() );
				if( m_dwQuestState != 0xFFFFFFFF )
				{
					QuestData &rkQuestData = pQuest->GetQuestData( m_pQuestParent->GetMainIndex(), m_pQuestParent->GetSubIndex() );
					if( m_dwQuestState != QS_PROGRESS )
					{
						HideChildWnd( ID_INSERT_ALARM_BTN );
						HideChildWnd( ID_DELETE_ALARM_BTN );
					}
					else if( rkQuestData.IsAlarm() )
					{
						HideChildWnd( ID_INSERT_ALARM_BTN );
						ShowChildWnd( ID_DELETE_ALARM_BTN );
					}
					else
					{
						ShowChildWnd( ID_INSERT_ALARM_BTN );
						HideChildWnd( ID_DELETE_ALARM_BTN );
					}
				}
			}			
			else
			{
				HideChildWnd( ID_INSERT_ALARM_BTN );
				HideChildWnd( ID_DELETE_ALARM_BTN );
			}
		}

		m_fCurWidth = m_fMaxWidth = g_QuestMgr.GetMaxQuestHelpWidth( m_pQuestParent, QS_PROGRESS );
	}
	else
	{
		HideWnd();
	}
}

void QuestProgressInfoWnd::iwm_hide()
{
	m_pQuestParent = NULL;

	if( GetParent() && GetParent()->IsShow() )
		GetParent()->HideWnd();
}

void QuestProgressInfoWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
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
	case ID_INSERT_ALARM_BTN:
		if( cmd == IOBN_BTNUP )
		{
			ioQuest *pQuest = g_MyInfo.GetQuest();
			if( pQuest && m_pQuestParent )
			{
				QuestData &rkData = pQuest->GetQuestData( m_pQuestParent->GetMainIndex(), m_pQuestParent->GetSubIndex() );
				if( rkData.GetMainIndex() > 0 )
				{
					if( rkData.GetState() == QS_PROGRESS && !rkData.IsAlarm() )
					{
						rkData.SetAlarm( true );
						SP2Packet kPacket( CTPK_QUEST_ALARM );
						kPacket << rkData.GetMainIndex() << rkData.GetSubIndex() << true;
						TCPNetwork::SendToServer( kPacket );
						g_QuestMgr.QuestInfoChange();
						HideChildWnd( ID_INSERT_ALARM_BTN );
						ShowChildWnd( ID_DELETE_ALARM_BTN );
					}
				}
			}
		}
		break;
	case ID_DELETE_ALARM_BTN:
		if( cmd == IOBN_BTNUP )
		{
			ioQuest *pQuest = g_MyInfo.GetQuest();
			if( pQuest && m_pQuestParent )
			{
				QuestData &rkData = pQuest->GetQuestData( m_pQuestParent->GetMainIndex(), m_pQuestParent->GetSubIndex() );
				if( rkData.GetMainIndex() > 0 )
				{
					if( rkData.GetState() == QS_PROGRESS && rkData.IsAlarm() )
					{
						rkData.SetAlarm( false );
						SP2Packet kPacket( CTPK_QUEST_ALARM );
						kPacket << rkData.GetMainIndex() << rkData.GetSubIndex() << false;
						TCPNetwork::SendToServer( kPacket );
						g_QuestMgr.QuestInfoChange();
						ShowChildWnd( ID_INSERT_ALARM_BTN );
						HideChildWnd( ID_DELETE_ALARM_BTN );
					}
				}
			}
		}
		break;
	}
}

void QuestProgressInfoWnd::iwm_wheel( int zDelta )
{
	if( !IsShow() )
		ioWnd::iwm_wheel( zDelta );
	else
	{
		if( g_App.IsMouseBusy() ) return;

		ioScroll *pScroll = (ioScroll*)FindChildWnd( ID_VERT_SCROLL );
		if( pScroll == NULL ) return;
		if( pScroll->HasWndStyle( IWS_INACTIVE ) ) return;

		if( zDelta == WHEEL_DELTA )
			pScroll->SetScrollPos( m_iCurPos - 1 );
		else if( zDelta == -WHEEL_DELTA )
			pScroll->SetScrollPos( m_iCurPos + 1 );
	}
}

void QuestProgressInfoWnd::iwm_vscroll( DWORD dwID, int curPos )
{
	m_iCurPos = curPos;
}

//void QuestProgressInfoWnd::OnProcess( float fTimePerSec )
//{
//	if( !IsShow() )
//		return;
//}

void QuestProgressInfoWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();
	if( m_pQuestParent )
	{
		if( m_pBottomFrm )
		{
			m_pBottomFrm->SetSize( GetWidth(), GetHeight() - BOTTOM_Y );
			m_pBottomFrm->Render( iXPos, iYPos, UI_RENDER_MULTIPLY );
		}

		OnRenderIcon( iXPos, iYPos );		

		// 퀘스트 설명
		g_QuestMgr.RenderQuestProgressHelp( m_pQuestParent, iXPos + 24, iYPos + 53, m_iCurPos, MAX_HELP_LINE, m_fCurWidth );

		// 퀘스트 목표
		if( m_pStatePurposeFrm )
			m_pStatePurposeFrm->Render( iXPos + 17, iYPos + 288 );

		g_FontMgr.SetAlignType( TAT_CENTER );
		g_FontMgr.SetTextStyle( TS_NORMAL );
		g_FontMgr.SetTextColor( TCT_DEFAULT_WHITE );
		g_FontMgr.PrintText( iXPos + 44, iYPos + 289, FONT_SIZE_11, STR(1) );

		ioComplexStringPrinter kPrinter;
		kPrinter.SetTextStyle( TS_NORMAL );
		kPrinter.SetTextColor( 0, 0, 0 );
		kPrinter.AddTextPiece( FONT_SIZE_13, m_pQuestParent->GetProgressText().c_str() );
		kPrinter.SetTextColor( TCT_DEFAULT_RED );
		
		char szBuf[MAX_PATH] = {0, };
		strcpy(szBuf, m_pQuestParent->GetProgressResult().c_str());
		int len = strlen(szBuf);
		if(len > 0)
			kPrinter.AddTextPiece( FONT_SIZE_13, STR(2), m_pQuestParent->GetProgressResult().c_str() );
		
		ioQuest *pQuest = g_MyInfo.GetQuest();
		if( pQuest )
		{
			if( pQuest->IsQuestComplete( m_pQuestParent->GetMainIndex(), m_pQuestParent->GetSubIndex() ) ||
				pQuest->IsQuestAttain( m_pQuestParent->GetMainIndex(), m_pQuestParent->GetSubIndex() ) )
			{
				kPrinter.AddTextPiece( FONT_SIZE_13, STR(3) );
			}
		}
		kPrinter.PrintFullText( iXPos + 72, iYPos + 289, TAT_LEFT );		

		g_QuestMgr.RenderQuestPurposeHelp( m_pQuestParent, iXPos + 17, iYPos + 312 );

		// 퀘스트 보상
		OnRenderReward( iXPos + 21, iYPos + 316 + m_iAddMaxHeight );
	}
}

bool QuestProgressInfoWnd::OnRenderShowAndHide()
{
	return ioWnd::OnRenderShowAndHide();
}

void QuestProgressInfoWnd::SetQuest( DWORD dwMainIndex, DWORD dwSubIndex )
{
	m_pQuestParent = g_QuestMgr.GetQuest( dwMainIndex, dwSubIndex );
	if( !m_pQuestParent )
	{
		g_ChatMgr.SetSystemMsg( STR(1), dwMainIndex, dwSubIndex );
		return;
	}
	ShowWnd();
}

void QuestProgressInfoWnd::SetQuest( QuestParent *pQuestParent )
{
	m_pQuestParent = pQuestParent;
	if( !m_pQuestParent )
	{
		g_ChatMgr.SetSystemMsg( STR(2) );
		return;
	}
	ShowWnd();
}
//////////////////////////////////////////////////////////////////////////
QuestProgressInfoMainWnd::QuestProgressInfoMainWnd()
{
	m_pQuestParent = NULL;
}

QuestProgressInfoMainWnd::~QuestProgressInfoMainWnd()
{

}

void QuestProgressInfoMainWnd::SetPositionToDefault(int iParentWidth, int iParentHeight, bool bChildMove, bool bCreate /* = false  */)
{
	if( !bCreate )
	{
		if( m_pQuestParent )
		{
			QuestProgressInfoWnd *pQuestWnd = NULL;
			if( m_pQuestParent->GetPerformType() == QP_EVENT )
				pQuestWnd = dynamic_cast< QuestProgressInfoWnd* >( FindChildWnd( ID_PINK_WND ) );
			else
				pQuestWnd = dynamic_cast< QuestProgressInfoWnd* >( FindChildWnd( ID_BROWN_WND ) );
			if( pQuestWnd )
				pQuestWnd->SetPositionToDefault( iParentWidth, iParentHeight, bChildMove, bCreate );
		}
	}
	ioMovingWnd::SetPositionToDefault( iParentWidth, iParentHeight, bChildMove, bCreate );
}

void QuestProgressInfoMainWnd::iwm_create()
{
	g_GUIMgr.AddWnd( "XML/QuestProgressInfoBrownWnd.xml", this );
	g_GUIMgr.AddWnd( "XML/QuestProgressInfoPinkWnd.xml", this );
}

void QuestProgressInfoMainWnd::iwm_show()
{
	if( m_pQuestParent )
	{
		QuestProgressInfoWnd *pQuestWnd = NULL;
		if( m_pQuestParent->GetPerformType() == QP_EVENT )
			pQuestWnd = dynamic_cast< QuestProgressInfoWnd* >( FindChildWnd( ID_PINK_WND ) );
		else
			pQuestWnd = dynamic_cast< QuestProgressInfoWnd* >( FindChildWnd( ID_BROWN_WND ) );
		if( pQuestWnd )
			pQuestWnd->SetQuest( m_pQuestParent );
	}
}

void QuestProgressInfoMainWnd::iwm_hide()
{
	if( m_pQuestParent )
	{
		QuestProgressInfoWnd *pQuestWnd = NULL;
		if( m_pQuestParent->GetPerformType() == QP_EVENT )
			pQuestWnd = dynamic_cast< QuestProgressInfoWnd* >( FindChildWnd( ID_PINK_WND ) );
		else
			pQuestWnd = dynamic_cast< QuestProgressInfoWnd* >( FindChildWnd( ID_BROWN_WND ) );
		if( pQuestWnd && pQuestWnd->IsShow() )
			pQuestWnd->HideWnd();
	}
}

bool QuestProgressInfoMainWnd::iwm_esc()
{
	if( m_pQuestParent )
	{
		QuestProgressInfoWnd *pQuestWnd = NULL;
		if( m_pQuestParent->GetPerformType() == QP_EVENT )
			pQuestWnd = dynamic_cast< QuestProgressInfoWnd* >( FindChildWnd( ID_PINK_WND ) );
		else
			pQuestWnd = dynamic_cast< QuestProgressInfoWnd* >( FindChildWnd( ID_BROWN_WND ) );
		if( pQuestWnd && pQuestWnd->IsShow() )
		{
			pQuestWnd->HideWnd();
			return true;
		}
	}
	return false;
}

void QuestProgressInfoMainWnd::iwm_wheel( int zDelta )
{
	if( !IsShow() ) return;

	if( m_pQuestParent )
	{
		QuestProgressInfoWnd *pQuestWnd = NULL;
		if( m_pQuestParent->GetPerformType() == QP_EVENT )
			pQuestWnd = dynamic_cast< QuestProgressInfoWnd* >( FindChildWnd( ID_PINK_WND ) );
		else
			pQuestWnd = dynamic_cast< QuestProgressInfoWnd* >( FindChildWnd( ID_BROWN_WND ) );
		if( pQuestWnd && pQuestWnd->IsShow() )
		{
			pQuestWnd->iwm_wheel( zDelta );
		}
	}
}

void QuestProgressInfoMainWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;

	ioMovingWnd::OnProcess( fTimePerSec );
}

bool QuestProgressInfoMainWnd::OnRenderShowAndHide()
{
	if( m_pQuestParent )
	{
		QuestProgressInfoWnd *pQuestWnd = NULL;
		if( m_pQuestParent->GetPerformType() == QP_EVENT )
			pQuestWnd = dynamic_cast< QuestProgressInfoWnd* >( FindChildWnd( ID_PINK_WND ) );
		else
			pQuestWnd = dynamic_cast< QuestProgressInfoWnd* >( FindChildWnd( ID_BROWN_WND ) );
		
		if( pQuestWnd )
			return pQuestWnd->OnRenderShowAndHide();
	}
	return false;
}

void QuestProgressInfoMainWnd::SetQuest( DWORD dwMainIndex, DWORD dwSubIndex )
{
	HideWnd();

	m_pQuestParent = g_QuestMgr.GetQuest( dwMainIndex, dwSubIndex );
	if( !m_pQuestParent )
	{
		g_ChatMgr.SetSystemMsg( STR(1), dwMainIndex, dwSubIndex );
		return;
	}
	ShowWnd();
}

void QuestProgressInfoMainWnd::SetQuest( QuestParent *pQuestParent )
{
	HideWnd();

	m_pQuestParent = pQuestParent;
	if( !m_pQuestParent )
	{
		g_ChatMgr.SetSystemMsg( STR(2) );
		return;
	}
	ShowWnd();
}
//////////////////////////////////////////////////////////////////////////
QuestCompleteInfoWnd::QuestCompleteInfoWnd()
{
	m_pBrownBackFrm = NULL;
	m_pBrownTitleFrm = NULL;
	m_pBrownBottomFrm = NULL;
	m_pBrownRewardOnFrm = NULL;

	m_pPinkBackFrm = NULL;
	m_pPinkTitleFrm = NULL;
	m_pPinkBottomFrm = NULL;
	m_pPinkRewardOnFrm = NULL;

	m_pQuestIconBack1 = NULL;
	m_pQuestIconBack2 = NULL;
	m_pAcquireText    = NULL;

	m_pCurrentBackFrm = NULL;
	m_pCurrentTitleFrm = NULL;
	m_pCurrentBottomFrm = NULL;
	m_pCurrentRewardOnFrm = NULL;
	m_pCostumeMark = NULL;

	m_pQuestParent    = NULL;
	m_dwHideDelayTime = 0;
	m_iHelpMentSize   = 0;
	m_iRewardSize     = 0;
	m_dwQuestState    = 0;
}

QuestCompleteInfoWnd::~QuestCompleteInfoWnd()
{
	SAFEDELETE( m_pBrownBackFrm );
	SAFEDELETE( m_pBrownTitleFrm );
	SAFEDELETE( m_pBrownBottomFrm );
	SAFEDELETE( m_pBrownRewardOnFrm );

	SAFEDELETE( m_pPinkBackFrm );
	SAFEDELETE( m_pPinkTitleFrm );
	SAFEDELETE( m_pPinkBottomFrm );
	SAFEDELETE( m_pPinkRewardOnFrm );

	SAFEDELETE( m_pQuestIconBack1 );
	SAFEDELETE( m_pQuestIconBack2 );
	SAFEDELETE( m_pAcquireText );
	SAFEDELETE( m_pCostumeMark );

	ClearRewardIconMap();	
}

void QuestCompleteInfoWnd::ClearRewardIconMap()
{
	QuestRewardIconMap::iterator iCreate = m_QuestRewardIconMap.begin();
	for(;iCreate != m_QuestRewardIconMap.end();++iCreate)
	{
		SAFEDELETE( iCreate->second );
	}
	m_QuestRewardIconMap.clear();
}

ioUIRenderImage *QuestCompleteInfoWnd::GetRewardIcon( int iPresentType, int iPresentValue1, int iPresentValue2 )
{
	ioHashString szIconName;
	switch( iPresentType )
	{
	case PRESENT_SOLDIER:
		{
			szIconName = g_MyInfo.GetMySoldierIconName( iPresentValue1 );
		}
		break;
	case PRESENT_DECORATION:
		{
			int iClassType = iPresentValue1 / 100000;
			int iSexType   = (iPresentValue1 % 100000) / FLOAT1000;
			int iDecoType  = iPresentValue1 % 1000;
			const DecoData &kData = g_DecorationPrice.GetDecoData( iSexType, iDecoType, iPresentValue2 );
			szIconName = kData.m_szIcon;
		}
		break;
	case PRESENT_ETC_ITEM:
		{
			ioEtcItem *pEtcItem = g_EtcItemMgr.FindEtcItem( iPresentValue1 );
			if( pEtcItem )
				szIconName = pEtcItem->GetIconName();
		}
		break;
	case PRESENT_PESO:
		{
			szIconName = "UIIconPack13#quest_002";
		}
		break;
	case PRESENT_EXTRAITEM:
	case PRESENT_ACCESSORY:
		{
			const ioItem *pItem = g_ItemMaker.GetItemConst( iPresentValue1, __FUNCTION__ );
			if( pItem )
			{
				szIconName = pItem->GetItemLargeIconName();
			}
		}
		break;
	case PRESENT_EXTRAITEM_BOX:
		{
			RandomMachineInfo *pInfo = g_ExtraItemInfoMgr.GetRandomMachineInfo( iPresentValue1 );
			if( pInfo )
			{
				szIconName = pInfo->GetIconName();
			}
		}
		break;
	case PRESENT_RANDOM_DECO:
		{
			int iClassType = iPresentValue1 / 100000;
			int iSexType   = (iPresentValue1 % 100000) / FLOAT1000;
			int iDecoType  = iPresentValue1 % 1000;
			const DecoData &kData = g_DecorationPrice.GetDecoData( iSexType, iDecoType, iPresentValue2 );
			szIconName = kData.m_szIcon;
		}
		break;
	case PRESENT_GRADE_EXP:
		{
			szIconName = "UIIconPack13#quest_001";
		}
		break;
	case PRESENT_MEDALITEM:
		{
			const ioMedalItemInfoManager::ItemInfo *pItem = g_MedalItemMgr.GetItemInfo( iPresentValue1 );
			if( pItem )	
				szIconName = pItem->m_sIcon;
		}
		break;
	case PRESENT_ALCHEMIC_ITEM:
		{
			szIconName = g_AlchemicMgr.GetAlchemicItemIcon( iPresentValue1 );
		}
		break;
	case PRESENT_PET:
		{
			int nPetRank = iPresentValue2%10000;
			szIconName = g_PetInfoMgr.GetPetIcon( iPresentValue1, (PetRankType)nPetRank );
		}
		break;
	case PRESENT_COSTUME:
		{
			// UI코스튬 관련 (퀘스트 아이콘)
			szIconName = g_CostumeInfoMgr.GetCostumeIcon( iPresentValue1 );
		}
		break;
	case PRESENT_COSTUME_BOX:
		{
			CostumeShopInfo *pInfo = g_CostumeShopInfoMgr.GetCostumeMachineInfo( iPresentValue1 );
			if( pInfo )
				szIconName = pInfo->GetIconName();
		}
		break;
	case PRESENT_BONUS_CASH:
		{
			szIconName = "UIIconPack101#Cash";
		}
		break;
	}

	if( szIconName.IsEmpty() )
		return NULL;

	QuestRewardIconMap::iterator iter = m_QuestRewardIconMap.find( szIconName );
	if( iter != m_QuestRewardIconMap.end() )
		return iter->second;

	ioUIRenderImage *pIcon = g_UIImageSetMgr.CreateImageByFullName( szIconName );
	if( pIcon )
	{
		m_QuestRewardIconMap.insert( QuestRewardIconMap::value_type( szIconName, pIcon ) );
		return pIcon;
	}
	return NULL;
}

void QuestCompleteInfoWnd::AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame )
{
	if( pFrame )
	{
		if( szType == "BrownBack" )
		{
			SAFEDELETE( m_pBrownBackFrm );
			m_pBrownBackFrm = pFrame;
		}
		else if( szType == "BrownTitle" )
		{
			SAFEDELETE( m_pBrownTitleFrm );
			m_pBrownTitleFrm = pFrame;
		}
		else if( szType == "BrownBottomFrm" )
		{
			SAFEDELETE( m_pBrownBottomFrm );
			m_pBrownBottomFrm = pFrame;
		}
		else if( szType == "BrownRewardOnFrm" )
		{
			SAFEDELETE( m_pBrownRewardOnFrm );
			m_pBrownRewardOnFrm = pFrame;
		}
		else if( szType == "PinkBack" )
		{
			SAFEDELETE( m_pPinkBackFrm );
			m_pPinkBackFrm = pFrame;
		}
		else if( szType == "PinkTitle" )
		{
			SAFEDELETE( m_pPinkTitleFrm );
			m_pPinkTitleFrm = pFrame;
		}
		else if( szType == "PinkBottomFrm" )
		{
			SAFEDELETE( m_pPinkBottomFrm );
			m_pPinkBottomFrm = pFrame;
		}
		else if( szType == "PinkRewardOnFrm" )
		{
			SAFEDELETE( m_pPinkRewardOnFrm );
			m_pPinkRewardOnFrm = pFrame;
		}
		else
		{	
			ioWnd::AddRenderFrame( szType, pFrame );
		}
	}
}

void QuestCompleteInfoWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "QuestIconBack1" )
	{
		SAFEDELETE( m_pQuestIconBack1 );
		m_pQuestIconBack1 = pImage;
	}
	else if( szType == "QuestIconBack2" )
	{
		SAFEDELETE( m_pQuestIconBack2 );
		m_pQuestIconBack2 = pImage;
	}
	else if( szType == "AcquireText" )
	{
		SAFEDELETE( m_pAcquireText );
		m_pAcquireText = pImage;
	}
	else if( szType == "costume_image" )
	{
		SAFEDELETE( m_pCostumeMark );
		m_pCostumeMark = pImage;
	}
	else
	{
		ioWnd::AddRenderImage( szType, pImage );
	}
}

void QuestCompleteInfoWnd::SetPositionToDefault( int iParentWidth, int iParentHeight, bool bChildMove, bool bCreate )
{
	ioWnd::SetPositionToDefault( iParentWidth, iParentHeight, bChildMove, bCreate );
	if( !bCreate )
	{
		ReSizeWnd();
	}
}

void QuestCompleteInfoWnd::SetQuest( QuestParent *pQuestParent )
{
	m_pQuestParent = pQuestParent;
	if( m_pQuestParent )
	{
		if( m_pQuestParent->GetPerformType() == QP_EVENT )
		{
			m_pCurrentBackFrm     = m_pPinkBackFrm;
			m_pCurrentTitleFrm	  = m_pPinkTitleFrm;
			m_pCurrentBottomFrm	  = m_pPinkBottomFrm;
			m_pCurrentRewardOnFrm = m_pPinkRewardOnFrm;

			ShowChildWnd( ID_PINK_EXIT );
			HideChildWnd( ID_BROWN_EXIT );
		}
		else
		{
			m_pCurrentBackFrm     = m_pBrownBackFrm;
			m_pCurrentTitleFrm	  = m_pBrownTitleFrm;
			m_pCurrentBottomFrm	  = m_pBrownBottomFrm;
			m_pCurrentRewardOnFrm = m_pBrownRewardOnFrm;

			HideChildWnd( ID_PINK_EXIT );
			ShowChildWnd( ID_BROWN_EXIT );
		}
	}

	if( IsShow() )
	{
		iwm_show();
	}
	else
	{
		ShowWnd();
		SetWndPos( (Setting::Width() / 2) - (GetWidth() / 2), (Setting::Height() / 2) - (GetHeight() / 2) - 20 );
	}
}

void QuestCompleteInfoWnd::ReSizeWnd()
{
	if( m_pQuestParent )
	{
		int iHeight = WND_HEIGHT_A + WND_HEIGHT_B + WND_HEIGHT_C;
		m_iHelpMentSize   = g_QuestMgr.GetMaxQuestHelp( m_pQuestParent, QS_COMPLETE ) * HELP_MENT_GAP;   // 멘트 사이즈
		m_iRewardSize     = m_pQuestParent->GetMaxRewardPresent() * REWARD_GAP;             // 선물 사이즈
		SetSize( GetWidth(), iHeight + m_iHelpMentSize + m_iRewardSize );

		RECT  rcDrag = { 0, 0, GetWidth(), GetHeight() };
		SetDragRect( rcDrag );
	}
}

void QuestCompleteInfoWnd::iwm_show()
{
	if( m_pQuestParent )
	{
		ReSizeWnd();

		ioQuest *pQuest = g_MyInfo.GetQuest();
		if( pQuest )
		{
			m_dwQuestState = pQuest->GetQuestState( m_pQuestParent->GetMainIndex(), m_pQuestParent->GetSubIndex() );			
		}
	}
	else
	{
		HideWnd();
	}
}

void QuestCompleteInfoWnd::iwm_hide()
{
	m_pQuestParent = NULL;
	m_dwHideDelayTime = FRAMEGETTIME();
}

void QuestCompleteInfoWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_BROWN_EXIT:
	case ID_PINK_EXIT:
		if( cmd == IOBN_BTNUP )
		{
			HideWnd();
		}
		break;
	}
}

void QuestCompleteInfoWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() )
		return;
	
	ioMovingWnd::OnProcess( fTimePerSec );
}

bool QuestCompleteInfoWnd::OnRenderShowAndHide()
{
	if( !HasExWndStyle( IWS_EX_SHOW_HIDE_ACTION ) ) 
		return false;

	const DWORD dwCheckTime = 50;
	if( m_dwWndAniTime == 0 ) return false;
	if( FRAMEGETTIME() - m_dwWndAniTime > dwCheckTime )
	{
		m_dwWndAniTime = 0;
		if( m_pCurrentBackFrm )
		{
			m_pCurrentBackFrm->SetSize( GetWidth(), GetHeight() );
		}
		return false;
	}

	// 확대 & 축소를 20% ~ 100% 사이만 진행 시킨다.
	DWORD dwGapTime = FRAMEGETTIME() - m_dwWndAniTime;
	float fTimeRate = ( (float)dwGapTime / dwCheckTime ) * FLOAT08;
	if( IsShow() )
	{
		fTimeRate = min( FLOAT08, fTimeRate ) + 0.2f;
	}
	else
	{
		fTimeRate = max( 0.0f, FLOAT08 - fTimeRate ) + 0.2f;
	}

	if( m_pCurrentBackFrm )
	{
		int iWidth = GetWidth() * fTimeRate;
		int iHeight= GetHeight() * fTimeRate;
		int iXPos  = GetDerivedPosX() + ( ( GetWidth() - iWidth ) / 2 );
		int iYPos  = GetDerivedPosY() + ( ( GetHeight() - iHeight ) / 2 );
		m_pCurrentBackFrm->SetSize( iWidth, iHeight );
		m_pCurrentBackFrm->Render( iXPos, iYPos );
	}		

	return true;
}

void QuestCompleteInfoWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();
	if( m_pQuestParent )
	{
		if( m_pCurrentBackFrm )
		{
			m_pCurrentBackFrm->SetSize( GetWidth(), GetHeight() );
			m_pCurrentBackFrm->Render( iXPos, iYPos );
		}

		if( m_pCurrentTitleFrm )
		{
			m_pCurrentTitleFrm->SetSize( GetWidth() - 10, 36 );
			m_pCurrentTitleFrm->Render( iXPos, iYPos );
		}

		// 타이틀
		g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
		g_FontMgr.SetAlignType( TAT_LEFT );
		if( m_pQuestParent->GetPerformType() == QP_EVENT )
			g_FontMgr.SetBkColor( 186, 79, 65 );
		else
			g_FontMgr.SetBkColor( 75, 57, 35 );
		g_FontMgr.SetTextColor( 255, 255, 255 );
		g_FontMgr.PrintTextWidthCut( iXPos + 13, iYPos + 11, FONT_SIZE_17, 330.0f, STR(1), m_pQuestParent->GetTitle().c_str() );	

		if( m_pCurrentBottomFrm )
		{
			m_pCurrentBottomFrm->SetSize( GetWidth(), WND_HEIGHT_C + m_iRewardSize + 13 );
			m_pCurrentBottomFrm->Render( iXPos, iYPos + WND_HEIGHT_A + m_iHelpMentSize + 10, UI_RENDER_MULTIPLY );
		}

		OnRenderIcon( iXPos, iYPos );
		g_QuestMgr.RenderQuestCompleteHelp( m_pQuestParent, iXPos + 171, iYPos + 144 );
		OnRenderReward( iXPos + 17, iYPos + WND_HEIGHT_A + m_iHelpMentSize + WND_HEIGHT_B + 2 );	

		ioComplexStringPrinter kPrinter;
		kPrinter.SetTextStyle( TS_NORMAL );
		kPrinter.SetBkColor( 0, 0, 0 );
		kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY ); 
		kPrinter.AddTextPiece( FONT_SIZE_13, STR(2), m_pQuestParent->GetProgressText().c_str() );
		switch( m_dwQuestState )
		{
		case QS_ATTAIN:
			kPrinter.SetTextColor( TCT_DEFAULT_GRAY ); 
			
			{
				char szBuf[MAX_PATH] = {0, };
				strcpy(szBuf, m_pQuestParent->GetProgressResult().c_str());
				int len = strlen(szBuf);
				if(len > 0)
					kPrinter.AddTextPiece( FONT_SIZE_13, STR(3), m_pQuestParent->GetProgressResult().c_str() );
			}

			break;
		case QS_COMPLETE:
			kPrinter.SetTextColor( TCT_DEFAULT_RED ); 
			
			{
				char szBuf[MAX_PATH] = {0, };
				strcpy(szBuf, m_pQuestParent->GetProgressResult().c_str());
				int len = strlen(szBuf);
				if(len > 0)
					kPrinter.AddTextPiece( FONT_SIZE_13, STR(3), m_pQuestParent->GetProgressResult().c_str() );
			}

			break;
		}
		kPrinter.PrintFullText( iXPos + 169, iYPos + WND_HEIGHT_A + WND_HEIGHT_B + m_iHelpMentSize + m_iRewardSize + 18, TAT_CENTER );
	}
}

void QuestCompleteInfoWnd::OnRenderIcon( int iXPos, int iYPos )
{
	if( m_pQuestIconBack1 )
		m_pQuestIconBack1->Render( iXPos + 171, iYPos + 90, UI_RENDER_MULTIPLY );
	if( m_pQuestIconBack2 )
	{
		m_pQuestIconBack2->SetScale( FLOAT1 );
		m_pQuestIconBack2->Render( iXPos + 171, iYPos + 90, UI_RENDER_MULTIPLY );
	}

	ioUIRenderImage *pQuestIcon = m_pQuestParent->GetQuestIcon();
	if( pQuestIcon )
	{		
		pQuestIcon->SetScale( FLOAT1 );
		pQuestIcon->Render( iXPos + 171, iYPos + 90 );
	}

	// UI코스튬 관련 (코스튬 마크)
	if ( m_pQuestParent->IsCostume() )
	{
		if ( m_pCostumeMark )
		{
			m_pCostumeMark->SetScale( 1.0f );
			m_pCostumeMark->Render( iXPos + 171, iYPos + 90 );
		}
	}
}

void QuestCompleteInfoWnd::OnRenderReward( int iXPos, int iYPos )
{
	for(int i = 0;i < m_pQuestParent->GetMaxRewardPresent();i++)
	{
		DWORD dwRewardIndex = m_pQuestParent->GetRewardPresentIndex( i );			
		bool bPresentDirect = false;
		int iPresentType, iPresentValue1, iPresentValue2;
		g_QuestMgr.GetPresentData( dwRewardIndex, iPresentType, iPresentValue1, iPresentValue2, bPresentDirect );

		if( iPresentType == 0 ) continue;

		if( m_pCurrentRewardOnFrm )
			m_pCurrentRewardOnFrm->Render( iXPos, iYPos );
		ioUIRenderImage *pRewardIcon = GetRewardIcon( iPresentType, iPresentValue1, iPresentValue2 );
		if( m_pQuestIconBack2 && pRewardIcon )
		{
			m_pQuestIconBack2->SetScale( FLOAT05 );
			m_pQuestIconBack2->Render( iXPos + 23, iYPos + 21, UI_RENDER_MULTIPLY, TFO_BILINEAR );
			pRewardIcon->SetScale( FLOAT05 );
			pRewardIcon->Render( iXPos + 23, iYPos + 21, UI_RENDER_NORMAL, TFO_BILINEAR );

			// UI코스튬 관련 (코스튬 마크)
			if ( ( iPresentType == PRESENT_COSTUME || iPresentType == PRESENT_COSTUME_BOX ) && m_pCostumeMark )
			{
				m_pCostumeMark->SetScale( 0.50f );
				m_pCostumeMark->Render( iXPos + 23, iYPos + 21, UI_RENDER_NORMAL, TFO_BILINEAR );
			}
		}

		char szPresentName[MAX_PATH] = "";
		if( iPresentType == PRESENT_PESO )
			sprintf_e( szPresentName, "%s", g_PresentMgr.GetPresentValue2Text( iPresentType, iPresentValue1, iPresentValue2 ).c_str() );
		else
		{
			if( iPresentType == PRESENT_MEDALITEM )
				SafeSprintf( szPresentName, sizeof( szPresentName ), STR(1), g_PresentMgr.GetPresentValue1Text( iPresentType, iPresentValue1, iPresentValue2 ).c_str(), g_PresentMgr.GetPresentValue2Text( iPresentType, iPresentValue1, iPresentValue2 ).c_str() );
			else
				sprintf_e( szPresentName, "%s %s", g_PresentMgr.GetPresentValue1Text( iPresentType, iPresentValue1, iPresentValue2 ).c_str(), g_PresentMgr.GetPresentValue2Text( iPresentType, iPresentValue1, iPresentValue2 ).c_str() );
		}

		g_FontMgr.SetTextStyle( TS_NORMAL );
		g_FontMgr.SetAlignType( TAT_LEFT );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
		g_FontMgr.PrintText( iXPos + 46, iYPos + 15, FONT_SIZE_13, szPresentName );	

		//if( m_pAcquireText )
			//m_pAcquireText->Render( iXPos + 277, iYPos + 20 );

		iYPos += REWARD_GAP;
	}
}
//////////////////////////////////////////////////////////////////////////
QuestDeleteAndFailWnd::QuestDeleteAndFailWnd()
{
	m_pBrownBackFrm = NULL;
	m_pBrownTitleFrm = NULL;
	m_pBrownBottomFrm = NULL;
	m_pBrownRewardOnFrm = NULL;

	m_pPinkBackFrm = NULL;
	m_pPinkTitleFrm = NULL;
	m_pPinkBottomFrm = NULL;
	m_pPinkRewardOnFrm = NULL;

	m_pCurrentBackFrm = NULL;
	m_pCurrentTitleFrm = NULL;
	m_pCurrentBottomFrm = NULL;
	m_pCurrentRewardOnFrm = NULL;

	m_pQuestIconBack1 = NULL;
	m_pQuestIconBack2 = NULL;
	m_pFinishText	  = NULL;
	m_pFailText		  = NULL;

	m_dwHideDelayTime = 0;
	m_bDeleteQuest    = false;
	m_pQuestParent	  = NULL;
	m_pCostumeMark = NULL;
}

QuestDeleteAndFailWnd::~QuestDeleteAndFailWnd()
{
	SAFEDELETE( m_pBrownBackFrm );
	SAFEDELETE( m_pBrownTitleFrm );
	SAFEDELETE( m_pBrownBottomFrm );
	SAFEDELETE( m_pBrownRewardOnFrm );

	SAFEDELETE( m_pPinkBackFrm );
	SAFEDELETE( m_pPinkTitleFrm );
	SAFEDELETE( m_pPinkBottomFrm );
	SAFEDELETE( m_pPinkRewardOnFrm );

	SAFEDELETE( m_pQuestIconBack1 );
	SAFEDELETE( m_pQuestIconBack2 );
	SAFEDELETE( m_pFinishText );
	SAFEDELETE( m_pFailText );
	SAFEDELETE( m_pCostumeMark );
}

void QuestDeleteAndFailWnd::AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame )
{
	if( pFrame )
	{
		if( szType == "BrownBack" )
		{
			SAFEDELETE( m_pBrownBackFrm );
			m_pBrownBackFrm = pFrame;
		}
		else if( szType == "BrownTitle" )
		{
			SAFEDELETE( m_pBrownTitleFrm );
			m_pBrownTitleFrm = pFrame;
		}
		else if( szType == "BrownBottomFrm" )
		{
			SAFEDELETE( m_pBrownBottomFrm );
			m_pBrownBottomFrm = pFrame;
		}
		else if( szType == "PinkBack" )
		{
			SAFEDELETE( m_pPinkBackFrm );
			m_pPinkBackFrm = pFrame;
		}
		else if( szType == "PinkTitle" )
		{
			SAFEDELETE( m_pPinkTitleFrm );
			m_pPinkTitleFrm = pFrame;
		}
		else if( szType == "PinkBottomFrm" )
		{
			SAFEDELETE( m_pPinkBottomFrm );
			m_pPinkBottomFrm = pFrame;
		}
		else
		{	
			ioWnd::AddRenderFrame( szType, pFrame );
		}
	}
}

void QuestDeleteAndFailWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "QuestIconBack1" )
	{
		SAFEDELETE( m_pQuestIconBack1 );
		m_pQuestIconBack1 = pImage;

	}
	else if( szType == "QuestIconBack2" )
	{
		SAFEDELETE( m_pQuestIconBack2 );
		m_pQuestIconBack2 = pImage;

	}
	else if( szType == "FinishText" )
	{
		SAFEDELETE( m_pFinishText );
		m_pFinishText = pImage;
	}
	else if( szType == "FailText" )
	{
		SAFEDELETE( m_pFailText );
		m_pFailText = pImage;
	}
	else if( szType == "costume_image" )
	{
		SAFEDELETE( m_pCostumeMark );
		m_pCostumeMark = pImage;
	}
	else
	{
		ioWnd::AddRenderImage( szType, pImage );
	}
}

void QuestDeleteAndFailWnd::iwm_show()
{

}

void QuestDeleteAndFailWnd::iwm_hide()
{
	m_dwHideDelayTime = FRAMEGETTIME();

	if( m_pQuestParent )
	{
		g_QuestMgr.EraseQuestDelAndFailList( m_pQuestParent->GetMainIndex(), m_pQuestParent->GetSubIndex() );
		m_bDeleteQuest    = false;
		m_pQuestParent	  = NULL;
	}
}

void QuestDeleteAndFailWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_BROWN_CLOSE:
	case ID_PINK_CLOSE:
		if( cmd == IOBN_BTNUP )
			HideWnd();
		break;
	}
}

bool QuestDeleteAndFailWnd::IsShowCheck()
{
	if( FRAMEGETTIME() - m_dwHideDelayTime < FLOAT100 ) 
		return false;

	m_bDeleteQuest    = false;
	m_pQuestParent	  = g_QuestMgr.FirstQuestDelAndFailList( m_bDeleteQuest );
	if( m_pQuestParent )
	{
		if( m_pQuestParent->GetPerformType() == QP_EVENT )
		{
			m_pCurrentBackFrm     = m_pPinkBackFrm;
			m_pCurrentTitleFrm	  = m_pPinkTitleFrm;
			m_pCurrentBottomFrm	  = m_pPinkBottomFrm;
			m_pCurrentRewardOnFrm = m_pPinkRewardOnFrm;

			ShowChildWnd( ID_PINK_CLOSE );
			HideChildWnd( ID_BROWN_CLOSE );
		}
		else
		{
			m_pCurrentBackFrm     = m_pBrownBackFrm;
			m_pCurrentTitleFrm	  = m_pBrownTitleFrm;
			m_pCurrentBottomFrm	  = m_pBrownBottomFrm;
			m_pCurrentRewardOnFrm = m_pBrownRewardOnFrm;

			HideChildWnd( ID_PINK_CLOSE );
			ShowChildWnd( ID_BROWN_CLOSE );
		}
		return true;
	}
	return false;
}

void QuestDeleteAndFailWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() )
	{
		if( IsShowCheck() )
		{
			ShowWnd();
		}
	}
	else
	{

	}
}

bool QuestDeleteAndFailWnd::OnRenderShowAndHide()
{
	if( !HasExWndStyle( IWS_EX_SHOW_HIDE_ACTION ) ) 
		return false;

	const DWORD dwCheckTime = 50;
	if( m_dwWndAniTime == 0 ) return false;
	if( FRAMEGETTIME() - m_dwWndAniTime > dwCheckTime )
	{
		m_dwWndAniTime = 0;
		if( m_pCurrentBackFrm )
		{
			m_pCurrentBackFrm->SetSize( GetWidth(), GetHeight() );
		}
		return false;
	}

	// 확대 & 축소를 20% ~ 100% 사이만 진행 시킨다.
	DWORD dwGapTime = FRAMEGETTIME() - m_dwWndAniTime;
	float fTimeRate = ( (float)dwGapTime / dwCheckTime ) * FLOAT08;
	if( IsShow() )
	{
		fTimeRate = min( FLOAT08, fTimeRate ) + 0.2f;
	}
	else
	{
		fTimeRate = max( 0.0f, FLOAT08 - fTimeRate ) + 0.2f;
	}

	if( m_pCurrentBackFrm )
	{
		int iWidth = GetWidth() * fTimeRate;
		int iHeight= GetHeight() * fTimeRate;
		int iXPos  = GetDerivedPosX() + ( ( GetWidth() - iWidth ) / 2 );
		int iYPos  = GetDerivedPosY() + ( ( GetHeight() - iHeight ) / 2 );
		m_pCurrentBackFrm->SetSize( iWidth, iHeight );
		m_pCurrentBackFrm->Render( iXPos, iYPos );
	}		

	return true;
}

void QuestDeleteAndFailWnd::OnRender()
{
	ioWnd::OnRender();

	if( !m_pQuestParent ) return;

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	if( m_pCurrentBackFrm )
	{
		m_pCurrentBackFrm->SetSize( GetWidth(), GetHeight() );
		m_pCurrentBackFrm->Render( iXPos, iYPos );
	}

	if( m_pCurrentTitleFrm )
	{
		m_pCurrentTitleFrm->SetSize( GetWidth() - 10, 36 );
		m_pCurrentTitleFrm->Render( iXPos, iYPos );
	}

	if( m_pCurrentBottomFrm )
		m_pCurrentBottomFrm->Render( iXPos, iYPos, UI_RENDER_MULTIPLY );

	iXPos += ( GetWidth() / 2 );
	if( m_bDeleteQuest )
	{	
		g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
		g_FontMgr.SetAlignType( TAT_CENTER );
		if( m_pQuestParent->GetPerformType() == QP_EVENT )
			g_FontMgr.SetBkColor( 186, 79, 65 );
		else
			g_FontMgr.SetBkColor( 75, 57, 35 );
		g_FontMgr.SetTextColor( 255, 255, 255 );
		g_FontMgr.PrintTextWidthCut( iXPos, iYPos + 11, FONT_SIZE_17, 330.0f, STR(1), m_pQuestParent->GetTitle().c_str() );

		//
		if( m_pQuestIconBack1 )
			m_pQuestIconBack1->Render( iXPos, iYPos + 90, UI_RENDER_MULTIPLY );
		if( m_pQuestIconBack2 )
		{
			m_pQuestIconBack2->SetScale( FLOAT1 );
			m_pQuestIconBack2->Render( iXPos, iYPos + 90, UI_RENDER_MULTIPLY );
		}

		ioUIRenderImage *pQuestIcon = m_pQuestParent->GetQuestIcon();
		if( pQuestIcon )
		{		
			pQuestIcon->SetScale( FLOAT1 );
			pQuestIcon->Render( iXPos, iYPos + 90, UI_RENDER_GRAY );
		}

		// UI코스튬 관련 (코스튬 마크)
		if( m_pQuestParent->IsCostume() && m_pCostumeMark )
		{		
			m_pCostumeMark->SetScale( 1.0f );
			m_pCostumeMark->Render( iXPos, iYPos + 90, UI_RENDER_GRAY );
		}

		if( m_pFinishText )
			m_pFinishText->Render( iXPos, iYPos + 90 );

		//
		g_FontMgr.SetTextStyle( TS_NORMAL );
		g_FontMgr.SetAlignType( TAT_CENTER );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
		g_FontMgr.PrintText( iXPos, iYPos + 144, FONT_SIZE_13, STR(2) );
		g_FontMgr.PrintText( iXPos, iYPos + 163, FONT_SIZE_13, STR(3) );
	}
	else
	{	
		g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
		g_FontMgr.SetAlignType( TAT_CENTER );
		if( m_pQuestParent->GetPerformType() == QP_EVENT )
			g_FontMgr.SetBkColor( 186, 79, 65 );
		else
			g_FontMgr.SetBkColor( 75, 57, 35 );
		g_FontMgr.SetTextColor( 255, 255, 255 );
		g_FontMgr.PrintTextWidthCut( iXPos, iYPos + 11, FONT_SIZE_17, 330.0f, STR(4), m_pQuestParent->GetTitle().c_str() );	
		
		//
		if( m_pQuestIconBack1 )
			m_pQuestIconBack1->Render( iXPos, iYPos + 90, UI_RENDER_MULTIPLY );
		if( m_pQuestIconBack2 )
		{
			m_pQuestIconBack2->SetScale( FLOAT1 );
			m_pQuestIconBack2->Render( iXPos, iYPos + 90, UI_RENDER_MULTIPLY );
		}

		ioUIRenderImage *pQuestIcon = m_pQuestParent->GetQuestIcon();
		if( pQuestIcon )
		{		
			pQuestIcon->SetScale( FLOAT1 );
			pQuestIcon->Render( iXPos, iYPos + 90, UI_RENDER_GRAY );
		}

		// UI코스튬 관련 (코스튬 마크)
		if( m_pQuestParent->IsCostume() && m_pCostumeMark )
		{		
			m_pCostumeMark->SetScale( 1.0f );
			m_pCostumeMark->Render( iXPos, iYPos + 90, UI_RENDER_GRAY );
		}

		if( m_pFailText )
			m_pFailText->Render( iXPos, iYPos + 90 );

		//
		g_FontMgr.SetTextStyle( TS_NORMAL );
		g_FontMgr.SetAlignType( TAT_CENTER );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
		g_FontMgr.PrintText( iXPos, iYPos + 144, FONT_SIZE_13, STR(5) );
		g_FontMgr.PrintText( iXPos, iYPos + 163, FONT_SIZE_13, STR(6) );
	}
}
//////////////////////////////////////////////////////////////////////////
GameQuestAlarmBtn::GameQuestAlarmBtn(){}
GameQuestAlarmBtn::~GameQuestAlarmBtn(){}
void GameQuestAlarmBtn::OnRender()
{
	GameQuestAlarmWnd *pParentWnd = dynamic_cast<GameQuestAlarmWnd*>(GetParent());
	if( pParentWnd )
	{
		if( pParentWnd->GetCurShowAniState() == GameQuestAlarmWnd::SAT_DELAY || 
			pParentWnd->GetCurShowAniState() == GameQuestAlarmWnd::SAT_START_ROUND )
			return;
	}
	ioButton::OnRender();
}
//////////////////////////////////////////////////////////////////////////
GameQuestAlarmScroll::GameQuestAlarmScroll()
{
	m_pSlideBack = NULL;
}

GameQuestAlarmScroll::~GameQuestAlarmScroll()
{
	SAFEDELETE( m_pSlideBack );
}

void GameQuestAlarmScroll::AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame )
{
	if( szType == "SlideBack" )
	{
		SAFEDELETE( m_pSlideBack );
		m_pSlideBack = pFrame;
	}
	else
	{
		ioScroll::AddRenderFrame( szType, pFrame );
	}
}

void GameQuestAlarmScroll::ReSizeScroll( int iWidth, int iHeight )
{
	SetSize( iWidth, iHeight );

	m_rcSlide.left	= 0;
	m_rcSlide.right = GetWidth();
	m_rcSlide.top	= 0;
	m_rcSlide.bottom= GetHeight();
}

void GameQuestAlarmScroll::SetPositionToDefault( int iParentWidth, int iParentHeight, bool bChildMove, bool bCreate )
{
	ioScroll::SetPositionToDefault( iParentWidth, iParentHeight, bChildMove, bCreate );;
}

void GameQuestAlarmScroll::OnRender()
{
	ioScroll::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();
	if( m_pSlideBack )
	{
		m_pSlideBack->SetSize( GetWidth() - 4, GetHeight() );
		m_pSlideBack->Render( iXPos + 2, iYPos );
	}
}
//////////////////////////////////////////////////////////////////////////
int GameQuestAlarmWnd::sg_iCurPage = 0;
GameQuestAlarmWnd::GameQuestAlarmWnd()
{
	m_pOverBack = NULL;
	m_pIconBack = NULL;
	m_pEventMark= NULL;
	m_pCostumeMark = NULL;

	m_iDefaultX  = 0;
	m_iDefaultY  = 0;
	m_iBlindOffSetY    = 0;
	m_iPreBlindOffSetY = 0;
	m_iOverListIndex   = -1;

	m_iMouseDownX = m_iMouseDownY = -1;
	m_iLimitAlarmCnt = 0;

	InitShowAniState();
	m_pScrollWnd = NULL;
}

GameQuestAlarmWnd::~GameQuestAlarmWnd()
{
	if( COMPARE( m_iDefaultX, 0, Setting::Width() ) && 
		COMPARE( m_iDefaultY, 0, Setting::Height() ) )
	{
		g_GUIMgr.GUIPosSaveInt( "GameQuestAlarmWnd", "XPos", m_iDefaultX ,POS_SAVE_VERSION );
		g_GUIMgr.GUIPosSaveInt( "GameQuestAlarmWnd", "YPos", m_iDefaultY ,POS_SAVE_VERSION );
	}

	SAFEDELETE( m_pOverBack );
	SAFEDELETE( m_pIconBack );
	SAFEDELETE( m_pEventMark );
	SAFEDELETE( m_pCostumeMark );
}

void GameQuestAlarmWnd::AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame )
{
	if( szType == "OverBack" )
	{
		SAFEDELETE( m_pOverBack );
		m_pOverBack = pFrame;
	}
	else
	{
		ioWnd::AddRenderFrame( szType, pFrame );
	}
}

void GameQuestAlarmWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "IconBack" )
	{
		SAFEDELETE( m_pIconBack );
		m_pIconBack = pImage;
	}
	else if( szType == "EventMark" )
	{
		SAFEDELETE( m_pEventMark );
		m_pEventMark = pImage;
	}
	else if( szType == "costume_image" )
	{
		SAFEDELETE( m_pCostumeMark );
		m_pCostumeMark = pImage;
	}
	else
	{
		ioWnd::AddRenderImage( szType, pImage );
	}
}

void GameQuestAlarmWnd::ParseExtraInfo( ioXMLElement &xElement )
{
}

void GameQuestAlarmWnd::iwm_create()
{
	int iXPos = g_GUIMgr.GUIPosLoadInt( "GameQuestAlarmWnd", "XPos", GetXPos() ,POS_SAVE_VERSION );
	int iYPos = g_GUIMgr.GUIPosLoadInt( "GameQuestAlarmWnd", "YPos", GetYPos() ,POS_SAVE_VERSION );

	if( COMPARE( iXPos, 0, Setting::Width() ) && 
		COMPARE( iYPos, 0, Setting::Height() ) )
	{
		SetWndPos( iXPos, iYPos );
	}

	m_iDefaultX = GetDerivedPosX();
	m_iDefaultY = GetDerivedPosY();
}

void GameQuestAlarmWnd::iwm_show()
{
	m_pScrollWnd = FindChildWnd( ID_VERT_SCROLL );

	UpdateSetPage( sg_iCurPage );
}

void GameQuestAlarmWnd::iwm_hide()
{
	m_vOpenSlide.clear();
	m_vCloseSlide.clear();
}

void GameQuestAlarmWnd::iwm_wheel( int zDelta )
{
	if( !IsShow() )
		ioWnd::iwm_wheel( zDelta );
	else
	{
		if( g_App.IsMouseBusy() ) return;

		ioScroll *pScroll = (ioScroll*)m_pScrollWnd;
		if( pScroll == NULL ) return;
		if( pScroll->HasWndStyle( IWS_INACTIVE ) ) return;

		if( zDelta == WHEEL_DELTA )
			pScroll->SetScrollPos( sg_iCurPage - 1 );
		else if( zDelta == -WHEEL_DELTA )
			pScroll->SetScrollPos( sg_iCurPage + 1 );
	}
}

void GameQuestAlarmWnd::iwm_vscroll(DWORD dwID, int curPos )
{
	UpdateSetPage( curPos );
}

bool GameQuestAlarmWnd::IsOverCheck()
{
	if( !g_App.IsMouseRender() ) 
		return false;

	if( !g_GUIMgr.GetPreOverWnd() ) 
		return true;

	if( g_GUIMgr.GetPreOverWnd() != this )
	{
		if( g_GUIMgr.GetPreOverWnd()->GetParent() )
		{
			if( g_GUIMgr.GetPreOverWnd()->GetParent() == m_pScrollWnd )
				return true;
			if( g_GUIMgr.GetPreOverWnd()->GetParent() != this )
				return false;
		}
	}

	return true;
}

bool GameQuestAlarmWnd::IsShowScroll()
{
	if( !g_App.IsMouseRender() ) 
		return false;

	if( g_GUIMgr.GetPreOverWnd() )
	{
		if( g_GUIMgr.GetPreOverWnd()->GetParent() )
		{
			if( g_GUIMgr.GetPreOverWnd()->GetParent() == m_pScrollWnd )
				return true;

			if( g_GUIMgr.GetPreOverWnd()->GetParent() == this )
				return true;
		}
	}

	if( g_GUIMgr.GetPreClickWnd() )
	{
		if( g_GUIMgr.GetPreClickWnd()->GetParent() )
		{
			if( g_GUIMgr.GetPreClickWnd()->GetParent() == m_pScrollWnd )
				return true;

			if( g_GUIMgr.GetPreClickWnd()->GetParent() == this )
				return true;
		}
	}

	if( g_GUIMgr.GetPreOverWnd() == this || g_GUIMgr.GetPreClickWnd() == this )
		return true;
	return false;
}

void GameQuestAlarmWnd::OnProcessMouse()
{
	if( !IsShow() ) return;

	if( m_iOverListIndex == -1 )
		m_iMouseDownX = m_iMouseDownY = -1;

	m_iOverListIndex  = -1;
	
	bool bScrollCheck = IsShowScroll();
	if( IsOverCheck() )
	{
		int iXPos, iYPos;
		iXPos = GetDerivedPosX();
		iYPos = GetDerivedPosY();

		if( m_ShowAniState.GetCurState() != SAT_DELAY && 
			m_ShowAniState.GetCurState() != SAT_START_ROUND )
		{
			ioLSMouse *pMouse = g_App.GetMouse();
			for(int i = 0;i < (int)m_vAlarmQuest.size();i++)
			{
				QuestParent *pQuestParent = m_vAlarmQuest[i];
				if( !pQuestParent ) continue;

				RECT rcNameRect;
				rcNameRect.left = iXPos;
				rcNameRect.top  = iYPos;
				rcNameRect.right  = rcNameRect.left + ICON_SIZE;
				rcNameRect.bottom = rcNameRect.top + ICON_SIZE;

				if( ::PtInRect( &rcNameRect, pMouse->GetMousePos() ) )
				{
					bScrollCheck = true;
					m_iOverListIndex  = i;								

					if( pMouse->IsLBtnDown() )
					{
						if( m_iMouseDownX == -1 )
							m_iMouseDownX = GetXPos();
						if( m_iMouseDownY == -1 )
							m_iMouseDownY = GetYPos();
					}
					else if( pMouse->IsLBtnUp() )
					{
						// 윈도우가 이동되었으면 체크하지 않는다. 
						if( abs( m_iMouseDownX - GetXPos() ) == 0 && abs( m_iMouseDownY - GetYPos() ) == 0 )
						{
							QuestProgressInfoMainWnd *pQuestInfo = dynamic_cast<QuestProgressInfoMainWnd*>(g_GUIMgr.FindWnd( QUEST_PROGRESS_INFO_WND ));
							if( pQuestInfo )
								pQuestInfo->SetQuest( pQuestParent );
						}
						m_iMouseDownX = m_iMouseDownY = -1;
					}
					InsertOpenSlide( pQuestParent );
					break;
				}
				iYPos += WND_GAP;
			}
		}			
	}

	if( m_iOverListIndex == -1 )
		InsertOpenSlide( NULL );

	if( m_pScrollWnd )
	{
		if( bScrollCheck )
			m_pScrollWnd->ShowWnd();
		else
			m_pScrollWnd->HideWnd();
	}
}

void GameQuestAlarmWnd::InsertOpenSlide( QuestParent *pQuestParent )
{
	{
		vSlideData::iterator iter = m_vOpenSlide.begin();
		for(;iter != m_vOpenSlide.end();)
		{
			SlideData &rkData = *iter;
			if( rkData.m_pQuestParent == pQuestParent )
			{
				iter++;
			}
			else
			{
				if( rkData.m_fTextSlideRate == FLOAT1 )
				{
					InsertCloseSlide( rkData.m_pQuestParent, FRAMEGETTIME(), FLOAT1 );
					iter = m_vOpenSlide.erase( iter );
				}
				else
				{
					iter++;
				}
			}
		}
		
		if( pQuestParent )
		{
			if( m_vOpenSlide.empty() )
			{
				SlideData kData;
				kData.m_pQuestParent	= pQuestParent;
				kData.m_dwTextSlideTime	= FRAMEGETTIME();
				kData.m_fTextSlideRate  = 0.0f;
				m_vOpenSlide.push_back( kData );
			}
		}
	}

	if( pQuestParent )
	{
		int i = 0;
		int iSize = m_vCloseSlide.size();
		for(i = 0;i < iSize;i++)
		{
			SlideData &rkData = m_vCloseSlide[i];
			if( rkData.m_pQuestParent == pQuestParent )
			{
				m_vCloseSlide.erase( m_vCloseSlide.begin() + i );			
				break;
			}
		}
	}
}

void GameQuestAlarmWnd::InsertCloseSlide( QuestParent *pQuestParent, DWORD dwSlideTime, float fSlideRate )
{
	int i = 0;
	int iSize = m_vCloseSlide.size();
	for(i = 0;i < iSize;i++)
	{
		SlideData &rkData = m_vCloseSlide[i];
		if( rkData.m_pQuestParent == pQuestParent )
			return;
	}

	SlideData kData;
	kData.m_pQuestParent	= pQuestParent;
	kData.m_dwTextSlideTime	= dwSlideTime;
	kData.m_fTextSlideRate  = fSlideRate;
	m_vCloseSlide.push_back( kData );
}

void GameQuestAlarmWnd::OnProcessSlide()
{
	enum { SLIDE_TIME = 300, };
	// Open
	{
		int i = 0;
		int iSize = m_vOpenSlide.size();
		for(i = 0;i < iSize;i++)
		{
			SlideData &rkData = m_vOpenSlide[i];
			DWORD dwGapTime = FRAMEGETTIME() - rkData.m_dwTextSlideTime ;
			if( dwGapTime < SLIDE_TIME )
				rkData.m_fTextSlideRate = sinf( ( FLOAT_PI * FLOAT05 ) * (float)( dwGapTime ) / SLIDE_TIME );
			else
				rkData.m_fTextSlideRate = FLOAT1;
		}
	}

	// Close
	{
		vSlideData::iterator iter = m_vCloseSlide.begin();
		for(;iter != m_vCloseSlide.end();)
		{
			SlideData &rkData = *iter;
			DWORD dwGapTime = FRAMEGETTIME() - rkData.m_dwTextSlideTime ;
			if( dwGapTime < SLIDE_TIME )
			{
				rkData.m_fTextSlideRate = cosf( ( FLOAT_PI * FLOAT05 ) * (float)( dwGapTime ) / SLIDE_TIME );
				iter++;
			}
			else
			{
				iter = m_vCloseSlide.erase( iter );
			}
		}
	}
}

bool GameQuestAlarmWnd::IsShowCheck()
{
	if( g_GUIMgr.IsShow( PARTY_SHAM_BATTLE_WND ) )
		return false;
	if( g_GUIMgr.IsShow( LADDER_BATTLE_WND ) )
		return false;
	if( g_GUIMgr.IsShow( TOTAL_RESULT_MAIN_WND ) )
		return false;
	if( g_GUIMgr.IsShow( BRIEFING_WND ) )
		return false;
	return true;
}

void GameQuestAlarmWnd::OnProcess( float fTimePerSec )
{
	ioQuest *pQuest = g_MyInfo.GetQuest();
	if( !pQuest )
		return;

	if( IsShow() )
	{
		if( IsShowCheck() )
		{
			ioMovingWnd::OnProcess( fTimePerSec );
			if( pQuest->GetMaxAlarmQuest() == 0 )
				HideWnd();

			if( g_QuestMgr.IsQuestAlarmChange() )
				UpdateSetPage( sg_iCurPage );

			OnProcessMouse();
			OnProcessSlide();
			UpdateAniState();	
		}
		else
		{
			HideWnd();
		}
	}
	else if( IsShowCheck() )
	{
		if( pQuest->GetMaxAlarmQuest() > 0 )
			ShowWnd();
	}
}

void GameQuestAlarmWnd::OnRender()
{
	if( m_ShowAniState.GetCurState() == SAT_DELAY || 
		m_ShowAniState.GetCurState() == SAT_START_ROUND )
		return;

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	//
	for(int i = 0;i < (int)m_vAlarmQuest.size();i++)
	{
		QuestParent *pQuestParent = m_vAlarmQuest[i];
		if( !pQuestParent ) continue;

		OnRenderQuestSlide( (iXPos + ICON_X), (iYPos + ICON_Y) - TEXT_Y, pQuestParent );

		if( m_pIconBack )
		{
			m_pIconBack->SetScale( FLOAT08 );
			m_pIconBack->Render( iXPos + ICON_X, iYPos + ICON_Y, UI_RENDER_MULTIPLY, TFO_BILINEAR );
			
			pQuestParent->RenderQuestIcon( iXPos + ICON_X, iYPos + ICON_Y, FLOAT08 );

			if ( pQuestParent->IsCostume() && m_pCostumeMark )
			{
				m_pCostumeMark->SetScale( 0.80f );
				m_pCostumeMark->Render( iXPos + ICON_X, iYPos + ICON_Y, UI_RENDER_NORMAL, TFO_BILINEAR );
			}
		}

		if( pQuestParent->GetPerformType() == QP_EVENT )
		{
			if( m_pEventMark )
			{
				m_pEventMark->Render( ( iXPos + ICON_X ) - (m_pEventMark->GetWidth() / 2) , iYPos - 5, UI_RENDER_NORMAL, TFO_BILINEAR );
			}
		}

		g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
		g_FontMgr.SetAlignType( TAT_CENTER );
		g_FontMgr.SetBkColor( 31, 31, 31 );
		g_FontMgr.SetTextColor( 255, 130, 0 );
		g_FontMgr.PrintText( iXPos + ICON_X, iYPos + ICON_Y + 10, FONT_SIZE_13, pQuestParent->GetProgressResultOnlyNumber().c_str() );

		iYPos += WND_GAP;
	}
}

void GameQuestAlarmWnd::OnRenderQuestSlide( int iXPos, int iYPos, QuestParent *pQuestParent )
{
	// Open
	{
		for(int i = 0;i < (int)m_vOpenSlide.size();i++)
		{
			SlideData &rkData = m_vOpenSlide[i];
			if( rkData.m_pQuestParent == pQuestParent )
			{
				if( rkData.m_fTextSlideRate == 0.0f ) 
					return;

				OnRenderQuestTitle( iXPos, iYPos, pQuestParent, rkData.m_fTextSlideRate );
				return;
			}
		}
	}

	// Close
	{
		for(int i = 0;i < (int)m_vCloseSlide.size();i++)
		{
			SlideData &rkData = m_vCloseSlide[i];
			if( rkData.m_pQuestParent == pQuestParent )
			{
				if( rkData.m_fTextSlideRate == 0.0f ) 
					return;

				OnRenderQuestTitle( iXPos, iYPos, pQuestParent, rkData.m_fTextSlideRate );
				return;
			}
		}
	}
}

void GameQuestAlarmWnd::OnRenderQuestTitle( int iXPos, int iYPos, QuestParent *pQuestParent, float fSlideRate )
{
	float fSlideWidth = (float)TEXT_W * fSlideRate;
	if( m_pOverBack )
	{
		m_pOverBack->SetSize( fSlideWidth, TEXT_H );
		m_pOverBack->Render( iXPos - ( (float)TEXT_X * fSlideRate ), iYPos );
	}

	float fTextLimitSize = min( max( 0.0f, fSlideWidth - 7 ), TEXT_LIMIT_SIZE );
	g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
	g_FontMgr.SetAlignType( TAT_LEFT );
	g_FontMgr.SetBkColor( 31, 31, 31 );
	g_FontMgr.SetTextColor( TCT_DEFAULT_WHITE );
	g_FontMgr.PrintTextWidthCut( ( iXPos + 7 )  - ( (float)TEXT_X * fSlideRate ), iYPos + 7, FONT_SIZE_12, fTextLimitSize, pQuestParent->GetTitle().c_str() );

	g_FontMgr.SetBkColor( 31, 31, 31 );
	g_FontMgr.SetTextColor( 189, 189, 189 );
	g_FontMgr.PrintTextWidthCut( ( iXPos + 7 )  - ( (float)TEXT_X * fSlideRate ), iYPos + 24, FONT_SIZE_11, fTextLimitSize, pQuestParent->GetProgressText().c_str() );

/*  텍스트를 통으로 자름
 	float fTextLimitSize = min( max( 0.0f, fSlideWidth - 7 ), TEXT_LIMIT_SIZE );
	char szWidthCut[MAX_PATH] = "";
	Help::StringCutFun( FONT_SIZE_13, fTextLimitSize, TS_OUTLINE_FULL_2X, szWidthCut, sizeof( szWidthCut ), pQuestParent->GetTitle().c_str() );
	g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
	g_FontMgr.SetAlignType( TAT_LEFT );
	g_FontMgr.SetBkColor( 31, 31, 31 );
	g_FontMgr.SetTextColor( TCT_DEFAULT_WHITE );
	g_FontMgr.PrintText( ( iXPos + 7 )  - ( (float)TEXT_X * fSlideRate ), iYPos + 7, FONT_SIZE_13, szWidthCut );

	Help::StringCutFun( FONT_SIZE_13, fTextLimitSize, TS_OUTLINE_FULL_2X, szWidthCut, sizeof( szWidthCut ), pQuestParent->GetProgressText().c_str() );
	g_FontMgr.SetBkColor( 31, 31, 31 );
	g_FontMgr.SetTextColor( 189, 189, 189 );
	g_FontMgr.PrintText( ( iXPos + 7 )  - ( (float)TEXT_X * fSlideRate ), iYPos + 24, FONT_SIZE_13, szWidthCut );
*/
}

void GameQuestAlarmWnd::InitShowAniState()
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
	{
		pState->Add( SAT_DELAY,  SAT_DELAY);
		pState->Add( SAT_NONE,  SAT_NONE);
	}
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

void GameQuestAlarmWnd::UpdateAniState()
{
	if( UpdateShowAni() )
		return;

	UpdateDefaultPos();

	// blind ani
	if( m_iBlindOffSetY != m_iPreBlindOffSetY )
	{
		int iCurYPos = m_iDefaultY + m_iBlindOffSetY;
		if( COMPARE( iCurYPos, 0, ( Setting::Height() - GetHeight() ) + 1) )
		{
			SetWndPos( m_iDefaultX,  iCurYPos );
			m_iPreBlindOffSetY = m_iBlindOffSetY;
		}
	}
}

void GameQuestAlarmWnd::ChangeShowAniState( ShowAniType eShowAniType )
{
	m_ShowAniState.ChangeState( (int) eShowAniType );
}

bool GameQuestAlarmWnd::UpdateShowAni()
{
	if( !g_GUIMgr.IsSkipUIByBlind() && m_ShowAniState.GetCurState() == SAT_NONE )
		m_ShowAniState.ChangeState( SAT_END );

	if( !g_GUIMgr.IsSkipUIByBlind() && m_ShowAniState.GetCurState() == SAT_START_ROUND )
		m_ShowAniState.ChangeState( SAT_DELAY );

	if( g_GUIMgr.IsSkipUIByBlind() && m_ShowAniState.GetCurState() == SAT_END )
		m_ShowAniState.ChangeState( SAT_NONE );

	m_ShowAniState.UpdateTime();

	if( m_ShowAniState.GetCurState() == SAT_UP )
	{
		enum { MINUS_Y_POS = 140 };
		int iStartYPos = MINUS_Y_POS + m_iDefaultY;
		int iCurYpos = m_iDefaultY - ( cos( (FLOAT_PI * FLOAT05 * m_ShowAniState.GetCurTime()  ) / m_ShowAniState.GetDelay() ) * iStartYPos );	
		SetWndPos( m_iDefaultX, iCurYpos );
		return true;
	}
	else if( m_ShowAniState.GetCurState() == SAT_DOWN )
	{
		enum { MAX_OFFSET_YPOS = 11 };
		int iCurYpos = m_iDefaultY - ( sin( (FLOAT_PI * FLOAT05 * m_ShowAniState.GetCurTime()  ) / m_ShowAniState.GetDelay() ) * MAX_OFFSET_YPOS );	
		SetWndPos( m_iDefaultX, iCurYpos );
		return true;
	}
	else if( m_ShowAniState.GetCurState() == SAT_UP_STOP )
	{
		enum { MAX_OFFSET_YPOS = 11 };
		int iCurYpos = m_iDefaultY - ( cos( (FLOAT_PI * FLOAT05 * m_ShowAniState.GetCurTime()  ) / m_ShowAniState.GetDelay() ) * MAX_OFFSET_YPOS );	
		SetWndPos( m_iDefaultX, iCurYpos );
		return true;
	}
	else if( m_ShowAniState.GetCurState() == SAT_STOP )
	{
		SetWndPos( m_iDefaultX, m_iDefaultY );
		m_ShowAniState.ChangeState( SAT_END );
		return true;
	}

	return false;
}

int GameQuestAlarmWnd::GetCurShowAniState()
{
	return m_ShowAniState.GetCurState();
}

void GameQuestAlarmWnd::UpdateDefaultPos()
{
	if( g_GUIMgr.IsDragMove( m_dwID ) || IsMoving() )
	{
		if( m_iDefaultX != GetXPos() )
		{
			if( COMPARE( GetXPos(), 0, (Setting::Width() - GetWidth() ) + 1 ))
				m_iDefaultX = GetXPos();
		}

		if( m_iDefaultY != GetYPos() )
		{
			int iYPos = GetYPos() - m_iBlindOffSetY;
			if( COMPARE( iYPos, 0, (Setting::Height() - GetHeight() ) + 1 ))
				m_iDefaultY = iYPos;
		}
	}
}

void GameQuestAlarmWnd::SetPositionToDefault( int iParentWidth, int iParentHeight, bool bChildMove, bool bCreate /* = false  */ )
{
	float fPosGap = ( Setting::Height() - 600.0f ) / FLOAT10;

	int iTempDefXPos = m_iDefXPos;
	int iTempDefYPos = m_iDefYPos;

	m_iDefXPos = iTempDefXPos - fPosGap;
	m_iDefYPos = iTempDefYPos + fPosGap;

	ioMovingWnd::SetPositionToDefault( iParentWidth, iParentHeight, bChildMove, bCreate );

	m_iDefXPos = iTempDefXPos;
	m_iDefYPos = iTempDefYPos;

	m_iLimitAlarmCnt = (float)( (float)(Setting::Height() - 222) - (fPosGap * 2) ) / WND_GAP;

	if( !bCreate )
	{
		m_iDefaultX = GetXPos();
		m_iDefaultY = GetYPos();

		SetWndPos( m_iDefaultX, m_iDefaultY + m_iBlindOffSetY );
		UpdateSetPage( 0 );
	}	
}

void GameQuestAlarmWnd::UpdateSetPage( int iCurPage )
{
	ioQuest *pQuest = g_MyInfo.GetQuest();
	if( !pQuest ) return;
	if( !m_pScrollWnd ) return;

	GameQuestAlarmScroll *pScrollWnd = dynamic_cast<GameQuestAlarmScroll*>(m_pScrollWnd);
	
	int iMaxQuest = pQuest->GetMaxAlarmQuest();
	int iMaxAlarm = min( m_iLimitAlarmCnt, iMaxQuest );
	
	if( m_iLimitAlarmCnt + iCurPage > iMaxQuest )
	{
		sg_iCurPage = 0;		
		if( iCurPage > 0 )
		{
			pScrollWnd->SetScrollPos( sg_iCurPage );
			return;
		}	
	}
	else
	{
		sg_iCurPage = iCurPage;
	}

	m_vAlarmQuest.clear();	
	for(int i = 0;i < iMaxAlarm;i++)
	{
		QuestData kData = pQuest->GetAlarmQuestToArray( sg_iCurPage + i );
		if( kData.GetMainIndex() > 0 )
		{
			QuestParent *pQuestParent = g_QuestMgr.GetQuest( kData.GetMainIndex(), kData.GetSubIndex() );
			if( pQuestParent )
			{
				m_vAlarmQuest.push_back( pQuestParent );
			}
		}
	}

	// 윈도우 사이즈
	SetSize( GetWidth(), ( m_vAlarmQuest.size() * WND_GAP ) );
	RECT rcDrag = { 0, 0, GetWidth(), GetHeight() };
	SetDragRect( rcDrag );	

	pScrollWnd->ReSizeScroll( m_pScrollWnd->GetWidth(), ( m_vAlarmQuest.size() * SCROLL_GAP ) );
	pScrollWnd->SetPageRange( m_iLimitAlarmCnt );
	pScrollWnd->SetScrollRange( 0, iMaxQuest );
}
//////////////////////////////////////////////////////////////////////////
QuestToolTip::QuestToolTip()
{
	m_pStateRedFrm   = NULL;
	m_pTooltipLeft	 = NULL;
	m_pTooltipLeftIn = NULL;
	m_pTooltipCenter = NULL;
	m_pTooltipRightIn= NULL;
	m_pTooltipRight  = NULL;

	m_pQuestParent   = NULL;
	m_dwShowTime	 = 0;
}

QuestToolTip::~QuestToolTip()
{
	SAFEDELETE( m_pStateRedFrm );
	SAFEDELETE( m_pTooltipLeft );
	SAFEDELETE( m_pTooltipLeftIn );
	SAFEDELETE( m_pTooltipCenter );
	SAFEDELETE( m_pTooltipRightIn );
	SAFEDELETE( m_pTooltipRight );
}

void QuestToolTip::AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame )
{
	if( szType == "StateRedFrm" )
	{
		SAFEDELETE( m_pStateRedFrm );
		m_pStateRedFrm = pFrame;
	}
	else
	{
		ioWnd::AddRenderFrame( szType, pFrame );
	}
}

void QuestToolTip::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "TooltipLeft" )
	{
		SAFEDELETE( m_pTooltipLeft );
		m_pTooltipLeft = pImage;
	}
	else if( szType == "TooltipLeftIn" )
	{
		SAFEDELETE( m_pTooltipLeftIn );
		m_pTooltipLeftIn = pImage;
	}
	else if( szType == "TooltipCenter" )
	{
		SAFEDELETE( m_pTooltipCenter );
		m_pTooltipCenter = pImage;
	}
	else if( szType == "TooltipRightIn" )
	{
		SAFEDELETE( m_pTooltipRightIn );
		m_pTooltipRightIn = pImage;
	}
	else if( szType == "TooltipRight" )
	{
		SAFEDELETE( m_pTooltipRight );
		m_pTooltipRight = pImage;
	}
	else
	{
		ioWnd::AddRenderImage( szType, pImage );
	}
}

void QuestToolTip::ShowTooltip( DWORD dwMainIndex, DWORD dwSubIndex )
{
	QuestParent *pQuestParent = g_QuestMgr.GetQuest( dwMainIndex, dwSubIndex );
	if( !pQuestParent )
		return;

	QuestData &kQuestData = pQuestParent->GetMyInfoQuestData();
	if( kQuestData.GetState() != QS_PROGRESS )
	{
		if( m_pQuestParent == pQuestParent )
			HideWnd();
		return;
	}

	m_pQuestParent    = pQuestParent;
	m_szFirstContents = m_pQuestParent->GetProgressText();	
	m_szLastContents  = m_pQuestParent->GetProgressResult();

	ShowWnd();
}

void QuestToolTip::iwm_show()
{
	m_dwShowTime = FRAMEGETTIME();
	char szContents[MAX_PATH * 2] = "";
	sprintf_e( szContents, "%s %s", m_szFirstContents.c_str(), m_szLastContents.c_str() );
	// 3은 갭이당.
	SetSize( STATE_WIDTH + 3 + g_FontMgr.GetTextWidth( szContents, TS_NORMAL, FONT_SIZE_13 ) + 20, GetHeight() );

	int iXPos = ( HelpWnd::GetQuestXPos() + 30 ) - ( GetWidth() / 2 );
	int iYPos = min( HelpWnd::GetQuestYPos() - 28, Setting::Height() - GetHeight() );
	SetWndPos( iXPos, iYPos );

	ioWnd *pCommandBtn = FindChildWnd( ID_COMMAND );
	if( pCommandBtn )
		pCommandBtn->SetSize( GetWidth(), GetHeight() );	
}

void QuestToolTip::iwm_hide()
{
	m_dwShowTime = 0;
	m_pQuestParent = NULL;	
}

void QuestToolTip::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_COMMAND:
		if( cmd == IOBN_BTNUP )
		{
			if( m_pQuestParent )
			{
				QuestProgressInfoMainWnd *pQuestInfoWnd = dynamic_cast<QuestProgressInfoMainWnd *>(g_GUIMgr.FindWnd( QUEST_PROGRESS_INFO_WND ));
				if( pQuestInfoWnd )
					pQuestInfoWnd->SetQuest( m_pQuestParent );
			}
		}
		break;
	}
}

void QuestToolTip::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;

	if( !m_pQuestParent )
		HideWnd();
	else if( FRAMEGETTIME() - m_dwShowTime > 6000 )
		HideWnd();
	else
	{
		int iXPos = ( HelpWnd::GetQuestXPos() + 30 ) - ( GetWidth() / 2 );
		int iYPos = min( HelpWnd::GetQuestYPos() - 28, Setting::Height() - GetHeight() );
		if( GetXPos() != iXPos || GetYPos() != iYPos )
			SetWndPos( iXPos, iYPos );
	}
}

void QuestToolTip::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	OnRenderTooltip( iXPos - 7, iYPos - 6 );

	int iRealXPos = iXPos + 9;
	int iRealYPos = iYPos + 10;
	if( m_pStateRedFrm )
	{
		m_pStateRedFrm->Render( iRealXPos, iRealYPos );
		g_FontMgr.SetAlignType( TAT_CENTER );
		g_FontMgr.SetTextStyle( TS_NORMAL );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextColor( 255, 255, 255 );
		g_FontMgr.PrintText( iRealXPos + 20, iRealYPos + 2, FONT_SIZE_11, STR(1) );
	}

	ioComplexStringPrinter kPrinter;
	kPrinter.SetTextStyle( TS_NORMAL );
	kPrinter.SetBkColor( 0, 0, 0 );	
	kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter.AddTextPiece( FONT_SIZE_13, STR(2), m_szFirstContents.c_str() );

	kPrinter.SetTextStyle( TS_NORMAL );
	kPrinter.SetBkColor( 0, 0, 0 );	
	kPrinter.SetTextColor( TCT_DEFAULT_ORANGE );
	kPrinter.AddTextPiece( FONT_SIZE_13, STR(3), m_szLastContents.c_str() );
	kPrinter.PrintFullText( iRealXPos + STATE_WIDTH + 3, iRealYPos, TAT_LEFT );
	kPrinter.ClearList();
}

void QuestToolTip::OnRenderTooltip( int iXPos, int iYPos )
{
	if( !m_pTooltipLeft || !m_pTooltipLeftIn || !m_pTooltipCenter || !m_pTooltipRightIn || !m_pTooltipRight ) return;

	// 15(L) / 19(C) / 15(R)
	int iSplitWidth = (GetWidth() - 35) / 2;

	m_pTooltipLeft->Render( iXPos, iYPos );
	iXPos += m_pTooltipLeft->GetWidth();	
	
	m_pTooltipLeftIn->SetSize( iSplitWidth, m_pTooltipLeftIn->GetHeight() );
	m_pTooltipLeftIn->SetHorzFormatting( ioUIRenderImage::HF_STRETCH );
	m_pTooltipLeftIn->Render( iXPos, iYPos );
	iXPos += iSplitWidth;

	m_pTooltipCenter->Render( iXPos, iYPos );
	iXPos += m_pTooltipCenter->GetWidth();

	m_pTooltipRightIn->SetSize( iSplitWidth, m_pTooltipRightIn->GetHeight() );
	m_pTooltipRightIn->SetHorzFormatting( ioUIRenderImage::HF_STRETCH );
	m_pTooltipRightIn->Render( iXPos, iYPos );
	iXPos += iSplitWidth;

	m_pTooltipRight->Render( iXPos, iYPos );
	iXPos += m_pTooltipRight->GetWidth();
}
//////////////////////////////////////////////////////////////////////////
QuestGuideWnd::QuestGuideWnd()
{
	m_pQuestParent = NULL;
	m_pInfoBtn     = NULL;
}

QuestGuideWnd::~QuestGuideWnd()
{

}

void QuestGuideWnd::SetQuestParent( QuestParent *pQuestParent )
{
	m_pQuestParent = pQuestParent;
}

bool QuestGuideWnd::IsButtonScreen()
{
	if( m_pInfoBtn == NULL )
		m_pInfoBtn = FindChildWnd( ID_INFO_BTN );
	
	if( m_pInfoBtn )
	{
		if( m_pInfoBtn->IsOver() || m_pInfoBtn->IsClicked() )
			return true;
	}
	return false;
}

void QuestGuideWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_INFO_BTN:
		if( cmd == IOBN_BTNUP )
		{
			if( m_pQuestParent && m_pQuestParent->IsQuestGuideShow() )
			{
				QuestProgressInfoMainWnd *pQuestInfo = dynamic_cast<QuestProgressInfoMainWnd*>(g_GUIMgr.FindWnd( QUEST_PROGRESS_INFO_WND ));
				if( pQuestInfo )
				{	
					pQuestInfo->SetQuest( m_pQuestParent );	
				}
			}
		}
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
LobbyQuestNewAlarmWnd::LobbyQuestNewAlarmWnd()
{
	m_iCurPage = m_iMaxPage = 0;
}

LobbyQuestNewAlarmWnd::~LobbyQuestNewAlarmWnd()
{
}

void LobbyQuestNewAlarmWnd::iwm_show()
{
	m_iCurPage = m_iMaxPage = 0;

	for( DWORD dwID = ID_ALARM_BTN1; dwID <= ID_ALARM_BTN7; ++dwID )
	{
		LobbyQuestNewAlarmBtn *pBtn = dynamic_cast<LobbyQuestNewAlarmBtn*>( FindChildWnd( dwID ) );
		if( pBtn )
		{
			if( dwID < static_cast<int>( ID_ALARM_BTN1 + MAX_ALARM ) )
				pBtn->ShowWnd();
			else
				pBtn->HideWnd();
		}
	}

	UpdateAlarm();
	CheckRadioButton( ID_PROCESS_BTN, ID_PROCESS_BTN, ID_PROCESS_BTN );
}

void LobbyQuestNewAlarmWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_PRE_BTN:
		{
			if( cmd == IOBN_BTNUP )
			{
				if ( m_iCurPage > 0 )
				{
					m_iCurPage--;
					UpdateAlarm();
				}
			}
		}
		break;
	case ID_NEXT_BTN:
		{
			if( cmd == IOBN_BTNUP )
			{
				if ( m_iCurPage < m_iMaxPage - 1 )
				{
					m_iCurPage++;
					UpdateAlarm();
				}
			}
		}
		break;
	case ID_ALARM_BTN1:
	case ID_ALARM_BTN2:
	case ID_ALARM_BTN3:
	case ID_ALARM_BTN4:
	case ID_ALARM_BTN5:
	case ID_ALARM_BTN6:
	case ID_ALARM_BTN7:
		if( cmd == IOBN_BTNUP )
		{
			LobbyQuestNewAlarmBtn *pQuestListBtn = dynamic_cast<LobbyQuestNewAlarmBtn*>( pWnd );
			if( pQuestListBtn && pQuestListBtn->GetQuestParent() )
			{
				QuestProgressInfoMainWnd *pQuestInfo = dynamic_cast<QuestProgressInfoMainWnd*>(g_GUIMgr.FindWnd( QUEST_PROGRESS_INFO_WND ));
				if( pQuestInfo )
					pQuestInfo->SetQuest( pQuestListBtn->GetQuestParent() );
			}
		}
		break;
	}
}

void LobbyQuestNewAlarmWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;

	if( g_QuestMgr.IsQuestAlarmChange() )
	{
		UpdateAlarm();
	}
}

void LobbyQuestNewAlarmWnd::UpdateAlarm()
{
	ioQuest *pQuest = g_MyInfo.GetQuest();
	if( !pQuest ) return;
	
	int iAlarmCount = pQuest->GetMaxAlarmQuest();
	m_iMaxPage = ( iAlarmCount / MAX_ALARM );
	if ( iAlarmCount % MAX_ALARM > 0 )
		m_iMaxPage++;

	if ( !COMPARE( m_iCurPage, 0, m_iMaxPage ) )
	{
		m_iCurPage = max( 0, m_iCurPage );
		m_iCurPage = min( m_iCurPage, max( 0, (m_iMaxPage - 1) ) );
	}

	int iStartArray =  m_iCurPage * MAX_ALARM;
	for(int i = 0;i < MAX_ALARM;i++,iStartArray++)
	{
		LobbyQuestNewAlarmBtn *pQuestAlarmBtn = dynamic_cast<LobbyQuestNewAlarmBtn*>( FindChildWnd( ID_ALARM_BTN1 + i ) );
		if( pQuestAlarmBtn )
			pQuestAlarmBtn->SetQuest( pQuest->GetAlarmQuestToArray( iStartArray ) );
	}

	m_PagePrinter.ClearList();

	m_PagePrinter.SetTextStyle( TS_NORMAL );
	m_PagePrinter.SetBkColor( 0, 0, 0 );
	m_PagePrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
	m_PagePrinter.AddTextPiece( FONT_SIZE_13, STR(1), m_iCurPage + 1 );
	m_PagePrinter.SetTextColor( TCT_DEFAULT_GRAY );
	if ( m_iMaxPage == 0 )
		m_PagePrinter.AddTextPiece( FONT_SIZE_13, STR(2), m_iMaxPage + 1 );
	else
		m_PagePrinter.AddTextPiece( FONT_SIZE_13, STR(3), m_iMaxPage );
}

void LobbyQuestNewAlarmWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	enum 
	{ 
		CUR_PAGE_X = 105,
		CUR_PAGE_Y = 392,
	};

	m_PagePrinter.PrintFullText( iXPos + CUR_PAGE_X, iYPos + CUR_PAGE_Y, TAT_CENTER );
}

//////////////////////////////////////////////////////////////////////////
LobbyQuestNewAlarmBtn::LobbyQuestNewAlarmBtn() : m_pQuestParent( NULL )
{
	m_pQuestIcon   = NULL;
	m_pIconBack    = NULL;
	m_pIconEmpty   = NULL;
	m_pEventMark   = NULL;
	m_pCostumeMark = NULL;
	
	m_pEventBackFrm = NULL;
	m_pNormalBackFrm = NULL;

	m_pEventDownBackFrm = NULL;
	m_pNormalDownBackFrm = NULL;

	m_bIsCostume = false;
}

LobbyQuestNewAlarmBtn::~LobbyQuestNewAlarmBtn()
{
	SAFEDELETE( m_pIconBack );
	SAFEDELETE( m_pIconEmpty );
	SAFEDELETE( m_pEventMark );
	SAFEDELETE( m_pCostumeMark );

	SAFEDELETE( m_pEventBackFrm );
	SAFEDELETE( m_pNormalBackFrm );

	SAFEDELETE( m_pEventDownBackFrm );
	SAFEDELETE( m_pNormalDownBackFrm );
}

void LobbyQuestNewAlarmBtn::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "IconBack" )
	{
		SAFEDELETE( m_pIconBack );
		m_pIconBack = pImage;
	}
	else if( szType == "IconEmpty" )
	{
		SAFEDELETE( m_pIconEmpty );
		m_pIconEmpty = pImage;
	}
	else if( szType == "EventMark" )
	{
		SAFEDELETE( m_pEventMark );
		m_pEventMark = pImage;
	}
	else if( szType == "costume_image" )
	{
		SAFEDELETE( m_pCostumeMark );
		m_pCostumeMark = pImage;
	}
	else
	{
		ioButton::AddRenderImage( szType, pImage );	
	}	
}

void LobbyQuestNewAlarmBtn::AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame )
{
	if( pFrame )
	{
		if( szType == "EventBack" )
		{
			SAFEDELETE( m_pEventBackFrm );
			m_pEventBackFrm = pFrame;
		}
		else if( szType == "NormalBack" )
		{
			SAFEDELETE( m_pNormalBackFrm );
			m_pNormalBackFrm = pFrame;
		}
		else if( szType == "EventDownBack" )
		{
			SAFEDELETE( m_pEventDownBackFrm );
			m_pEventDownBackFrm = pFrame;
		}
		else if( szType == "NormalDownBack" )
		{
			SAFEDELETE( m_pNormalDownBackFrm );
			m_pNormalDownBackFrm = pFrame;
		}
		else
			ioButton::AddRenderFrame( szType, pFrame );
	}
}

void LobbyQuestNewAlarmBtn::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_DELETE_BTN:
		if( cmd == IOBN_BTNUP )
		{
			// 알림에서 삭제
			ioQuest *pQuest = g_MyInfo.GetQuest();
			if( pQuest )
			{
				QuestData &rkData = pQuest->GetQuestData( m_QuestData.GetMainIndex(), m_QuestData.GetSubIndex() );
				if( rkData.GetMainIndex() > 0 )
				{
					rkData.SetAlarm( false );
					SP2Packet kPacket( CTPK_QUEST_ALARM );
					kPacket << rkData.GetMainIndex() << rkData.GetSubIndex() << false;
					TCPNetwork::SendToServer( kPacket );
					g_QuestMgr.QuestInfoChange();
				}
			}
		}
		break;
	}
}

void LobbyQuestNewAlarmBtn::OnRender()
{
	ioButton::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	enum
	{
		//미션 없는 빈슬롯
		EMPTY_ICON_POS_X				= 31,
		EMPTY_ICON_POS_Y				= 29,

		EMPTY_TEXT_POS_X				= 63,
		EMPTY_TEXT_POS_Y				= 16,
		EMPTY_TEXT_OFFSET_Y_LING_GAP	= 14,

		//미션이 있는 경우
		ICON_POS_X						= 32,
		ICON_POS_Y						= 31,

		TEXT_POS_X						= 63,
		TEXT_POS_Y						= 8,
	};

	if( !m_pQuestParent )
	{
		if( m_pIconEmpty )
		{
			m_pIconEmpty->SetScale( 0.7f, 0.7f );
			m_pIconEmpty->Render( iXPos + EMPTY_ICON_POS_X, iYPos + EMPTY_ICON_POS_Y, UI_RENDER_MULTIPLY, TFO_BILINEAR );
		}

		g_FontMgr.SetTextStyle( TS_NORMAL );
		g_FontMgr.SetAlignType( TAT_LEFT );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY2 );
		g_FontMgr.PrintText( iXPos + EMPTY_TEXT_POS_X, iYPos + EMPTY_TEXT_POS_Y, FONT_SIZE_11, STR(1) );
		g_FontMgr.PrintText( iXPos + EMPTY_TEXT_POS_X, iYPos + EMPTY_TEXT_POS_Y + EMPTY_TEXT_OFFSET_Y_LING_GAP, FONT_SIZE_11, STR(2) );
		return;
	}

	if( m_pIconBack )
	{
		m_pIconBack->SetScale( 0.60f, 0.60f );
		m_pIconBack->Render( iXPos + ICON_POS_X, iYPos + ICON_POS_Y, UI_RENDER_MULTIPLY, TFO_BILINEAR );
	}

	switch( m_QuestData.GetState() )
	{
	case QS_PROGRESS:
		{
			if( m_pQuestIcon )
			{
				m_pQuestIcon->SetScale( 0.60f, 0.60f );
				m_pQuestIcon->Render( iXPos + ICON_POS_X, iYPos + ICON_POS_Y, UI_RENDER_NORMAL, TFO_BILINEAR );
			}

			// UI코스튬 관련 (코스튬 마크)
			if ( m_bIsCostume && m_pCostumeMark )
			{
				m_pCostumeMark->SetScale( 0.60f, 0.60f );
				m_pCostumeMark->Render( iXPos + ICON_POS_X, iYPos + ICON_POS_Y, UI_RENDER_NORMAL, TFO_BILINEAR );
			}

			OnRenderQuestTitle( iXPos + TEXT_POS_X, iYPos + TEXT_POS_Y, m_pQuestParent->GetProgressResultOnlyNumber().c_str() );
		}
		break;
	case QS_ATTAIN:
		{
			if( m_pQuestIcon )
			{
				m_pQuestIcon->SetScale( 0.60f, 0.60f );
				m_pQuestIcon->Render( iXPos + ICON_POS_X, iYPos + ICON_POS_Y, UI_RENDER_NORMAL, TFO_BILINEAR );
			}

			// UI코스튬 관련 (코스튬 마크)
			if ( m_bIsCostume && m_pCostumeMark )
			{
				m_pCostumeMark->SetScale( 0.60f, 0.60f );
				m_pCostumeMark->Render( iXPos + ICON_POS_X, iYPos + ICON_POS_Y, UI_RENDER_NORMAL, TFO_BILINEAR );
			}

			OnRenderQuestTitle( iXPos + TEXT_POS_X, iYPos + TEXT_POS_Y, STR(3) );
		}
		break;
	case QS_COMPLETE:
		{
			if( m_pQuestIcon )
			{
				m_pQuestIcon->SetScale( 0.60f, 0.60f );
				m_pQuestIcon->Render( iXPos + ICON_POS_X, iYPos + ICON_POS_Y, UI_RENDER_NORMAL, TFO_BILINEAR );
			}

			// UI코스튬 관련 (코스튬 마크)
			if ( m_bIsCostume && m_pCostumeMark )
			{
				m_pCostumeMark->SetScale( 0.60f, 0.60f );
				m_pCostumeMark->Render( iXPos + ICON_POS_X, iYPos + ICON_POS_Y, UI_RENDER_NORMAL, TFO_BILINEAR );
			}

			OnRenderQuestTitle( iXPos + TEXT_POS_X, iYPos + TEXT_POS_Y, STR(4) );
		}
		break;
	}

	if( m_pQuestParent->GetPerformType() == QP_EVENT )
	{
		if( m_pEventMark )
		{
			m_pEventMark->SetScale( 0.60f);
			m_pEventMark->Render( iXPos - 11, iYPos - 11, UI_RENDER_NORMAL, TFO_BILINEAR );
		}
	}
}

void LobbyQuestNewAlarmBtn::OnRenderQuestTitle( int iXPos, int iYPos, const char *szResult )
{	
	// 한줄 & 두줄 체크
	float fTitleLimitWidth = 93.0f;
	float fLimitWidth = 99.0f;
	char szBuffer[MAX_PATH] = "";
	if( strlen( szResult ) == 0 )
		sprintf_e( szBuffer, "%s", m_pQuestParent->GetProgressText().c_str() );
	else
		sprintf_e( szBuffer, "%s %s", m_pQuestParent->GetProgressText().c_str(), szResult );

	if( g_FontMgr.GetTextWidth( szBuffer, TS_NORMAL, FONT_SIZE_11 ) <= fLimitWidth )
	{
		// 2줄짜리
		// 퀘스트 명
		g_FontMgr.SetTextStyle( TS_NORMAL );
		g_FontMgr.SetAlignType( TAT_LEFT );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
		g_FontMgr.PrintTextWidthCut( iXPos, iYPos + 6, FONT_SIZE_11, fTitleLimitWidth, m_pQuestParent->GetTitle().c_str() );

		// 퀘스트 목적
		ioComplexStringPrinter kPrinter;
		kPrinter.SetTextStyle( TS_NORMAL );
		kPrinter.SetTextColor( TCT_DEFAULT_GRAY );
		kPrinter.AddTextPiece( FONT_SIZE_11, m_pQuestParent->GetProgressText().c_str() );
		if( strlen( szResult ) > 0 )
		{
			kPrinter.SetTextColor( TCT_DEFAULT_RED );
			kPrinter.AddTextPiece( FONT_SIZE_11, szResult );
		}
		kPrinter.PrintFullText( iXPos, iYPos + 20, TAT_LEFT );
	}
	else
	{
		// 3줄짜리
		// 퀘스트 명
		g_FontMgr.SetTextStyle( TS_NORMAL );
		g_FontMgr.SetAlignType( TAT_LEFT );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
		g_FontMgr.PrintTextWidthCut( iXPos, iYPos, FONT_SIZE_11, fTitleLimitWidth, m_pQuestParent->GetTitle().c_str() );

		// 퀘스트 목적
		int iResultXPos = 0;
		sprintf_e( szBuffer, "%s", m_pQuestParent->GetProgressText().c_str() );	
		g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );		
		char szDst[MAX_PATH] = "";
		memset( szDst, 0, sizeof( szDst ) );
		int iLen = Help::StringCutFun( FONT_SIZE_11, fLimitWidth, TS_NORMAL, szDst, sizeof(szDst), szBuffer );
		g_FontMgr.PrintText( iXPos, iYPos + 14, FONT_SIZE_11, szDst );
		if( iLen < (int)strlen( szBuffer ) )
		{
			float fWidthCut = fLimitWidth - g_FontMgr.GetTextWidth( szResult, TS_NORMAL, FONT_SIZE_11 );
			g_FontMgr.PrintTextWidthCut( iXPos, iYPos + 28, FONT_SIZE_11, fWidthCut, &szBuffer[iLen] );
			iResultXPos = 4 + min( fWidthCut, g_FontMgr.GetTextWidth( &szBuffer[iLen], TS_NORMAL, FONT_SIZE_11 ) );
		}

		// 
		g_FontMgr.SetTextColor( TCT_DEFAULT_RED );
		g_FontMgr.PrintText( iXPos + iResultXPos, iYPos + 28, FONT_SIZE_11, szResult );
	}
}

void LobbyQuestNewAlarmBtn::SetQuest( QuestData &rkQuestData )
{
	m_pCurFrm = NULL;
	m_pCurDownFrm = NULL;
	m_QuestData = rkQuestData;
	if( m_QuestData.GetMainIndex() == 0 )
	{
		SetInActive();
		HideChildWnd( ID_DELETE_BTN );
		m_pQuestParent = NULL;
	}
	else
	{
		SetActive();
		m_pQuestParent = g_QuestMgr.GetQuest( m_QuestData.GetMainIndex(), m_QuestData.GetSubIndex() );
		if( m_pQuestParent )
		{
			m_pQuestIcon = m_pQuestParent->GetQuestIcon();
			m_bIsCostume = m_pQuestParent->IsCostume();

			if( m_pQuestParent->GetPerformType() == QP_EVENT )
			{
				m_pCurFrm = m_pEventBackFrm;
				m_pCurDownFrm = m_pEventDownBackFrm;
			}
			else
			{
				m_pCurFrm = m_pNormalBackFrm;
				m_pCurDownFrm = m_pNormalDownBackFrm;
			}
		}
		ShowChildWnd( ID_DELETE_BTN );

		
	}		
}

void LobbyQuestNewAlarmBtn::OnDrawNormal( int iXPos, int iYPos )
{
	if( m_pCurFrm )
		m_pCurFrm->Render( iXPos, iYPos );

	RenderImageList( iXPos, iYPos, UI_RENDER_NORMAL );
}

void LobbyQuestNewAlarmBtn::OnDrawOvered( int iXPos, int iYPos )
{
	if( m_pCurFrm )
		m_pCurFrm->Render( iXPos, iYPos );
	OnDrawOveredAdd( iXPos, iYPos );
	RenderImageList( iXPos, iYPos, UI_RENDER_NORMAL );
}

void LobbyQuestNewAlarmBtn::OnDrawPushed( int iXPos, int iYPos )
{
	if( m_pCurDownFrm )
		m_pCurDownFrm->Render( iXPos, iYPos );
	RenderImageList( iXPos, iYPos, UI_RENDER_SCREEN );
}
//////////////////////////////////////////////////////////////////////////
QuestListWnd::QuestListWnd()
{
	ClearData();
	m_dwTabID = ID_PROGRESS_BTN;
}

QuestListWnd::~QuestListWnd()
{

}

void QuestListWnd::iwm_create()
{
}

void QuestListWnd::ClearData()
{
	m_iCurPage = m_iMaxPage = 0;
}

void QuestListWnd::iwm_show()
{
	ClearData();
	ChangeRadioWnd( ID_PROGRESS_BTN );
	ChangeTitle();

	g_QuestMgr.IsQuestInfoChange();
	for( DWORD dwID = ID_QUEST_LIST_BTN1; dwID <= ID_QUEST_LIST_BTN5; ++dwID )
	{
		NewQuestListBtn *pBtn = dynamic_cast<NewQuestListBtn*>( FindChildWnd( dwID ) );
		if( pBtn )
		{
			if( dwID < static_cast<int>( ID_QUEST_LIST_BTN1 + MAX_BTN ) )
				pBtn->ShowWnd();
			else
				pBtn->HideWnd();
		}
	}
}

void QuestListWnd::iwm_hide()
{

}

void QuestListWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();	
	switch( dwID )
	{
	case ID_PRE_BTN:
		if( cmd == IOBN_BTNUP )
		{
			ChangeData( m_iCurPage - 1 );
		}
		break;
	case ID_NEXT_BTN:
		if( cmd == IOBN_BTNUP )
		{
			ChangeData( m_iCurPage + 1 );
		}
		break;
	case ID_PROGRESS_BTN:
	case ID_COMPLETE_BTN:
		if( cmd == IOBN_BTNDOWN )
		{			
			ChangeRadioWnd( dwID );
		}
		break;
	case ID_QUEST_LIST_BTN1:
	case ID_QUEST_LIST_BTN2:
	case ID_QUEST_LIST_BTN3:
	case ID_QUEST_LIST_BTN4:
	case ID_QUEST_LIST_BTN5:
		if( cmd == IOBN_BTNUP )
		{
			NewQuestListBtn *pQuestListBtn = dynamic_cast<NewQuestListBtn*>( pWnd );
			if( pQuestListBtn && pQuestListBtn->GetQuestParent() )
			{
				if( param == NewQuestListBtn::ID_QUEST_RESULT_BTN )
				{
					QuestCompleteInfoWnd *pQuestInfo = dynamic_cast<QuestCompleteInfoWnd*>(g_GUIMgr.FindWnd( QUEST_COMPLETE_INFO_WND ));
					if( pQuestInfo )
						pQuestInfo->SetQuest( pQuestListBtn->GetQuestParent() );
				}
				else
				{
					QuestProgressInfoMainWnd *pQuestInfo = dynamic_cast<QuestProgressInfoMainWnd*>(g_GUIMgr.FindWnd( QUEST_PROGRESS_INFO_WND ));
					if( pQuestInfo )
						pQuestInfo->SetQuest( pQuestListBtn->GetQuestParent() );
				}
			}
		}
		break;
	}
}

void QuestListWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() )
		return;

	if( g_QuestMgr.IsQuestInfoChange() )
	{
		ChangeData( m_iCurPage );
		ChangeTitle();
	}
}

void QuestListWnd::ChangeTitle()
{
	ioQuest *pQuest = g_MyInfo.GetQuest();
	if( !pQuest ) return;
	char szTitle[MAX_PATH] = "";

	// 진행중 탭 버튼 타이틀
	ioWnd *pProgressBtn = FindChildWnd( ID_PROGRESS_BTN );
	if( pProgressBtn )
	{
		SafeSprintf( szTitle, sizeof( szTitle ), STR(1), pQuest->GetMaxQuest( QS_PROGRESS ) );
		pProgressBtn->SetTitleText( szTitle );
	}

	// 완료 탭 버튼 타이틀
	ioWnd *pCompleteBtn = FindChildWnd( ID_COMPLETE_BTN );
	if( pCompleteBtn )
	{
		SafeSprintf( szTitle, sizeof( szTitle ), STR(2), pQuest->GetMaxQuest( QS_ATTAIN ) + pQuest->GetMaxQuest( QS_COMPLETE ) );
		pCompleteBtn->SetTitleText( szTitle );
	}
}

void QuestListWnd::ChangeData( int iCurPage )
{
	switch( m_dwTabID )
	{
	case ID_PROGRESS_BTN:
		{
			ioQuest *pQuest = g_MyInfo.GetQuest();
			if( !pQuest ) return;

			m_iMaxPage = max( 0, pQuest->GetMaxQuest( QS_PROGRESS ) - 1 ) / MAX_BTN;
			m_iCurPage = max( 0, iCurPage );
			m_iCurPage = min( m_iCurPage, m_iMaxPage );

			int iStartArray = m_iCurPage * MAX_BTN;
			for(int i = 0;i < MAX_BTN;i++,iStartArray++)
			{
				NewQuestListBtn *pQuestListBtn = dynamic_cast<NewQuestListBtn*>( FindChildWnd( ID_QUEST_LIST_BTN1 + i ) );
				if( pQuestListBtn )
				{
					pQuestListBtn->SetQuest( pQuest->GetProcessToArray( iStartArray ) );
				}
			}
			
		}
		break;
	case ID_COMPLETE_BTN:
		{
			ioQuest *pQuest = g_MyInfo.GetQuest();
			if( !pQuest ) return;

			int iCompleteCount = pQuest->GetMaxQuest( QS_ATTAIN ) + pQuest->GetMaxQuest( QS_COMPLETE );
			m_iMaxPage = max( 0, iCompleteCount - 1 ) / MAX_BTN;
			m_iCurPage = max( 0, iCurPage );
			m_iCurPage = min( m_iCurPage, m_iMaxPage );

			int iStartArray = m_iCurPage * MAX_BTN;
			for(int i = 0;i < MAX_BTN;i++,iStartArray++)
			{
				NewQuestListBtn *pQuestListBtn = dynamic_cast<NewQuestListBtn*>( FindChildWnd( ID_QUEST_LIST_BTN1 + i ) );
				if( pQuestListBtn )
				{
					pQuestListBtn->SetQuest( pQuest->GetAttainNCompleteToArray( iStartArray ) );
				}
			}
			
		}
		break;
	}

	m_PagePrinter.ClearList();
	m_PagePrinter.SetTextStyle( TS_NORMAL );
	m_PagePrinter.SetBkColor( 0, 0, 0 );
	m_PagePrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
	m_PagePrinter.AddTextPiece( FONT_SIZE_13, STR(1), m_iCurPage + 1 );
	m_PagePrinter.SetTextColor( TCT_DEFAULT_GRAY );
	m_PagePrinter.AddTextPiece( FONT_SIZE_13, STR(2), m_iMaxPage + 1 );
}

void QuestListWnd::ChangeRadioWnd( DWORD dwID )
{
	m_dwTabID = dwID;
	CheckRadioButton( ID_PROGRESS_BTN, ID_COMPLETE_BTN, dwID );

	ChangeData( 0 );
}

void QuestListWnd::OnRender()
{
	ioWnd::OnRender();
	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	enum 
	{ 
		CUR_PAGE_X = 230,
		CUR_PAGE_Y = 393,
	};

	m_PagePrinter.PrintFullText( iXPos + CUR_PAGE_X, iYPos + CUR_PAGE_Y, TAT_CENTER );
}

//////////////////////////////////////////////////////////////////////////
NewQuestListBtn::NewQuestListBtn() : m_pQuestParent( NULL )
{
	m_pRewardFrm	 = NULL;
	m_pEmptyQuest    = NULL;
	m_pQuestIcon	 = NULL;
	m_pIconBack		 = NULL;
	m_pEventMark	 = NULL;
	m_pCostumeMark = NULL;

	m_pNormalBackFrm = NULL;
	m_pEventBackFrm	 = NULL;
	m_pCurBackFrm	 = NULL;

	m_bIsCostume = false;
}

NewQuestListBtn::~NewQuestListBtn()
{
	SAFEDELETE( m_pRewardFrm );
	SAFEDELETE( m_pIconBack );
	SAFEDELETE( m_pEmptyQuest );
	SAFEDELETE( m_pEventMark );
	SAFEDELETE( m_pCostumeMark );

	SAFEDELETE( m_pNormalBackFrm );
	SAFEDELETE( m_pEventBackFrm );
}

void NewQuestListBtn::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "IconBack" )
	{
		SAFEDELETE( m_pIconBack );
		m_pIconBack = pImage;
	}
	else if( szType == "EmptyQuest" )
	{
		SAFEDELETE( m_pEmptyQuest );
		m_pEmptyQuest = pImage;
	}
	else if( szType == "EventMark" )
	{
		SAFEDELETE( m_pEventMark );
		m_pEventMark = pImage;
	}
	else if( szType == "costume_image" )
	{
		SAFEDELETE( m_pCostumeMark );
		m_pCostumeMark = pImage;
	}
	else
	{
		ioButton::AddRenderImage( szType, pImage );	
	}	
}

void NewQuestListBtn::AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame )
{
	if( pFrame )
	{
		if( szType == "RewardFrm" )
		{
			SAFEDELETE( m_pRewardFrm );
			m_pRewardFrm = pFrame;
		}
		else if( szType == "EventBack" )
		{
			SAFEDELETE( m_pEventBackFrm );
			m_pEventBackFrm = pFrame;
		}
		else if( szType == "NormalBack" )
		{
			SAFEDELETE( m_pNormalBackFrm );
			m_pNormalBackFrm = pFrame;
		}
		else
		{	
			ioButton::AddRenderFrame( szType, pFrame );
		}
	}
}

void NewQuestListBtn::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_QUEST_RESULT_BTN:
		if( cmd == IOBN_BTNUP )
		{
			if( GetParent() )
				GetParent()->iwm_command( this, cmd, dwID );
		}
		break;
	case ID_QUEST_INSERT_ALARM:
		if( cmd == IOBN_BTNUP )
		{
			ioQuest *pQuest = g_MyInfo.GetQuest();
			if( pQuest && m_pQuestParent )
			{
				QuestData &rkData = pQuest->GetQuestData( m_pQuestParent->GetMainIndex(), m_pQuestParent->GetSubIndex() );
				if( rkData.GetMainIndex() > 0 )
				{
					if( rkData.GetState() == QS_PROGRESS && !rkData.IsAlarm() )
					{
						rkData.SetAlarm( true );
						SP2Packet kPacket( CTPK_QUEST_ALARM );
						kPacket << rkData.GetMainIndex() << rkData.GetSubIndex() << true;
						TCPNetwork::SendToServer( kPacket );
						g_QuestMgr.QuestInfoChange();
					}

					if( rkData.IsAlarm() )
					{
						HideChildWnd( ID_QUEST_INSERT_ALARM );
						ShowChildWnd( ID_QUEST_REMOVE_ALARM );
					}
				}
			}
		}
		break;
	case ID_QUEST_REMOVE_ALARM:
		if( cmd == IOBN_BTNUP )
		{
			ioQuest *pQuest = g_MyInfo.GetQuest();
			if( pQuest && m_pQuestParent )
			{
				QuestData &rkData = pQuest->GetQuestData( m_pQuestParent->GetMainIndex(), m_pQuestParent->GetSubIndex() );
				if( rkData.GetMainIndex() > 0 )
				{
					if( rkData.GetState() == QS_PROGRESS && rkData.IsAlarm() )
					{
						rkData.SetAlarm( false );
						SP2Packet kPacket( CTPK_QUEST_ALARM );
						kPacket << rkData.GetMainIndex() << rkData.GetSubIndex() << false;
						TCPNetwork::SendToServer( kPacket );
						g_QuestMgr.QuestInfoChange();
					}

					if( rkData.IsAlarm() )
					{
						ShowChildWnd( ID_QUEST_INSERT_ALARM );
						HideChildWnd( ID_QUEST_REMOVE_ALARM );
					}
				}
			}
		}
		break;
	}
}

void NewQuestListBtn::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) 
		return;
}

void NewQuestListBtn::OnRender()
{
	
	ioButton::OnRender();
	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	enum
	{
		//미션 없는 빈슬롯
		EMPTY_ICON_POS_X				= 36,
		EMPTY_ICON_POS_Y				= 35,

		EMPTY_TEXT_POS_X				= 74,
		EMPTY_TEXT_POS_Y				= 18,
		EMPTY_TEXT_OFFSET_Y_LING_GAP	= 16,
	};

	if( HasWndStyle( IWS_INACTIVE ) || !m_pQuestParent )
	{
		if( m_pEmptyQuest )
		{
			m_pEmptyQuest->SetScale( 0.75f, 0.75f );
			m_pEmptyQuest->Render( iXPos + EMPTY_ICON_POS_X, iYPos + EMPTY_ICON_POS_Y, UI_RENDER_MULTIPLY, TFO_BILINEAR );
		}

		g_FontMgr.SetTextStyle( TS_NORMAL );
		g_FontMgr.SetAlignType( TAT_LEFT );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextColor( 178, 168, 130 );
		g_FontMgr.PrintText( iXPos + EMPTY_TEXT_POS_X, iYPos + EMPTY_TEXT_POS_Y, FONT_SIZE_13, STR(1) );
		g_FontMgr.PrintText( iXPos + EMPTY_TEXT_POS_X, iYPos + EMPTY_TEXT_POS_Y + EMPTY_TEXT_OFFSET_Y_LING_GAP, FONT_SIZE_13, STR(2) );
	}
	else
	{
		//Render Icon 
		OnRenderIcon( iXPos, iYPos );
		//Render Text
		OnRenderText( iXPos , iYPos );
	}
}

void NewQuestListBtn::OnRenderIcon( int iXPos, int iYPos )
{
	enum
	{
		//미션이 있는 경우
		ICON_POS_X						= 36,
		ICON_POS_Y						= 33,
	};
	if( m_pIconBack )
	{
		m_pIconBack->SetScale( 0.65f, 0.65f );
		m_pIconBack->Render( iXPos + ICON_POS_X, iYPos + ICON_POS_Y, UI_RENDER_MULTIPLY, TFO_BILINEAR );
	}

	if( m_pQuestIcon )
	{
		m_pQuestIcon->SetScale( 0.65f, 0.65f );
		m_pQuestIcon->Render( iXPos + ICON_POS_X, iYPos + ICON_POS_Y, UI_RENDER_NORMAL, TFO_BILINEAR );
	}

	// UI코스튬 관련 (코스튬 마크)
	if( m_bIsCostume && m_pCostumeMark )
	{
		m_pCostumeMark->SetScale( 0.65f, 0.65f );
		m_pCostumeMark->Render( iXPos + ICON_POS_X, iYPos + ICON_POS_Y, UI_RENDER_NORMAL, TFO_BILINEAR );
	}

	if( m_pQuestParent->GetPerformType() == QP_EVENT )
	{
		if( m_pEventMark )
		{
			m_pEventMark->SetScale( 0.70f);
			m_pEventMark->Render( iXPos - 9.0f , iYPos - 9.0f, UI_RENDER_NORMAL, TFO_BILINEAR );
		}
	}
}

void NewQuestListBtn::OnRenderText( int iXPos, int iYPos )
{
	enum
	{
		TEXT_POS_X						= 74,
		ONE_SUBSTANCE_TITLE_POS_Y		= 18,
		ONE_SUBSTANCE_POS_Y				= 36,
		
		TWO_SUBSTANCE_TITLE_POS_Y		= 12,
		TWO_SUBSTANCE_POS_Y				= 28,

		SUBSTANCE_OFFSET_Y_LING_GAP		= 16,

		PROGRESS_RESULT_POS_X			= 405,
		PROGRESS_RESULT_POS_Y			= 26,
	};

	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetAlignType( TAT_LEFT );
	g_FontMgr.SetBkColor( 0, 0, 0 );

	float fWidth = 245.0f;
	char szTitle[MAX_PATH] = "";
	sprintf( szTitle, m_pQuestParent->GetProgressText().c_str() );
	if( g_FontMgr.GetTextWidth( szTitle, TS_NORMAL, FONT_SIZE_11 ) <= fWidth )
	{
		//타이틀
		g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
		g_FontMgr.PrintTextWidthCut( iXPos + TEXT_POS_X, iYPos + ONE_SUBSTANCE_TITLE_POS_Y, FONT_SIZE_13, fWidth, m_pQuestParent->GetTitle().c_str() );

		// 내용
		g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
		g_FontMgr.PrintText( iXPos + TEXT_POS_X, iYPos + ONE_SUBSTANCE_POS_Y, FONT_SIZE_11, szTitle );	
	}
	else // 2줄까지만 내려쓰기
	{
		//타이틀
		g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
		g_FontMgr.PrintTextWidthCut( iXPos + TEXT_POS_X, iYPos + TWO_SUBSTANCE_TITLE_POS_Y, FONT_SIZE_13, fWidth, m_pQuestParent->GetTitle().c_str() );

		// 내용
		g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
		char szDst[MAX_PATH] = "";
		memset( szDst, 0, sizeof( szDst ) );
		int iLen = Help::StringCutFun( FONT_SIZE_11, fWidth, TS_NORMAL, szDst, sizeof(szDst), szTitle );
		g_FontMgr.PrintText( iXPos + TEXT_POS_X, iYPos + TWO_SUBSTANCE_POS_Y, FONT_SIZE_11, szDst );
		if( iLen < (int)strlen( szTitle) )
		{
			g_FontMgr.PrintTextWidthCut( iXPos + TEXT_POS_X, iYPos + TWO_SUBSTANCE_POS_Y + SUBSTANCE_OFFSET_Y_LING_GAP,
				FONT_SIZE_11, fWidth, &szTitle[iLen] );
		}
			
	}

	if ( m_QuestData.GetState() == QS_PROGRESS )
	{
		g_FontMgr.SetAlignType( TAT_RIGHT );
		g_FontMgr.SetTextColor( TCT_DEFAULT_RED );
		if( m_pQuestParent->GetProgressResult().IsEmpty() )
			g_FontMgr.PrintText( iXPos + PROGRESS_RESULT_POS_X, iYPos + PROGRESS_RESULT_POS_Y, FONT_SIZE_13, STR(1) );
		else
			g_FontMgr.PrintText( iXPos + PROGRESS_RESULT_POS_X, iYPos + PROGRESS_RESULT_POS_Y, FONT_SIZE_13, m_pQuestParent->GetProgressResult().c_str() );
	}
}

void NewQuestListBtn::SetQuest( QuestData &rkQuestData )
{
	m_QuestData = rkQuestData;
	if( m_QuestData.GetMainIndex() == 0 )
	{
		SetInActive();

		HideChildWnd( ID_QUEST_RESULT_BTN );
		HideChildWnd( ID_QUEST_INSERT_ALARM );
		HideChildWnd( ID_QUEST_REMOVE_ALARM );
		m_pQuestParent = NULL;
	}
	else
	{
		SetActive();
		m_pQuestParent = g_QuestMgr.GetQuest( m_QuestData.GetMainIndex(), m_QuestData.GetSubIndex() );
		if( m_pQuestParent )
		{
			m_pQuestIcon = m_pQuestParent->GetQuestIcon();
			m_bIsCostume = m_pQuestParent->IsCostume();

			if( m_pQuestParent->GetPerformType() == QP_EVENT )
				m_pCurBackFrm = m_pEventBackFrm;
			else
				m_pCurBackFrm = m_pNormalBackFrm;
		}

		if( m_QuestData.GetState() == QS_PROGRESS )
		{
			HideChildWnd( ID_QUEST_RESULT_BTN );
			if( m_QuestData.IsAlarm() )
			{
				HideChildWnd( ID_QUEST_INSERT_ALARM );
				ShowChildWnd( ID_QUEST_REMOVE_ALARM );
			}
			else
			{
				ShowChildWnd( ID_QUEST_INSERT_ALARM );
				HideChildWnd( ID_QUEST_REMOVE_ALARM );
			}
		}
		else
		{
			ShowChildWnd( ID_QUEST_RESULT_BTN );	
			HideChildWnd( ID_QUEST_INSERT_ALARM );
			HideChildWnd( ID_QUEST_REMOVE_ALARM );
		}
	}
}

void NewQuestListBtn::OnDrawNormal( int iXPos, int iYPos )
{
	if( m_pCurBackFrm )
		m_pCurBackFrm->Render( iXPos, iYPos );

	RenderImageList( iXPos, iYPos, UI_RENDER_NORMAL );
}

void NewQuestListBtn::OnDrawOvered( int iXPos, int iYPos )
{

	if( m_pCurBackFrm )
		m_pCurBackFrm->Render( iXPos, iYPos );
	OnDrawOveredAdd( iXPos, iYPos );
	RenderImageList( iXPos, iYPos, UI_RENDER_NORMAL );
}

void NewQuestListBtn::OnDrawPushed( int iXPos, int iYPos )
{
	if( m_pCurBackFrm )
		m_pCurBackFrm->Render( iXPos, iYPos );
	RenderImageList( iXPos, iYPos, UI_RENDER_SCREEN );
}

//////////////////////////////////////////////////////////////////////////