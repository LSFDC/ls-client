#include "StdAfx.h"

#include "..\io3DEngine\ioXMLElement.h"
#include "../io3DEngine/ioXMLDocument.h"
#include "../io3DEngine/ioUIImageSetManager.h"
#include "../io3DEngine/ioUIRenderImage.h"
#include "../io3DEngine/ioFontManager.h"
#include "../io3DEngine/ioSoundManager.h"
#include "../io3DEngine/iostringmanager.h"

#include "..\Setting.h"
#include "..\ioEventManager.h"
#include "../ioEventUserNode.h"
#include "..\ioApplication.h"
#include "..\EtcHelpFunc.h"
#include "..\ioMyInfo.h"
#include "..\WndID.h"
#include "..\ioPlayStage.h"
#include "../ioClassPrice.h"
#include "../NetworkWrappingFunc.h"
#include "../ioPresentMgr.h"
#include "../ioDecorationPrice.h"
#include "../ioEtcItemManager.h"
#include "../ioItemMaker.h"
#include "../TextColorTable.h"

#include "ioSP2GUIManager.h"
#include "SoldierSelectWnd.h"
#include "MyInventoryWnd.h"

#include ".\eventcatalogwnd.h"
#include <strsafe.h>
#include "..\TextColorTable.h"


//-----------------------------------------------------------------------------------------------------------------------------------
HWND EventCatalogWnd::m_IEHwnd    = 0;

EventCatalogWnd::EventCatalogWnd(void)
{
	m_bIERefreshCheck  = false;
	m_dwIERefreshTimer = 0;
	m_IEHwnd           = 0;

	m_iSelectId        = ID_EVENT_1_BTN;
	m_pImgFirst        = NULL;
	m_pImgSecond       = NULL;

	m_vEventInfoVector.reserve( MAX_BTN );
}

EventCatalogWnd::~EventCatalogWnd(void)
{
	for(vEventInfoVector::iterator iter = m_vEventInfoVector.begin(); iter != m_vEventInfoVector.end(); ++iter)
	{
	    delete *iter;
	}
	m_vEventInfoVector.clear();

	SAFEDELETE( m_pImgFirst );
	SAFEDELETE( m_pImgSecond );
}

void EventCatalogWnd::iwm_show()
{
	if( g_MyInfo.IsTutorialUser() )
	{
		HideWnd();
		return;
	}
	SetShowHideChild();

	EvnetChildrenDay::SetAutoShow( true );

	SendEventGiftRequest();
}

void EventCatalogWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();

	switch ( dwID )
	{
	case ID_EVENT_1_BTN:
	case ID_EVENT_2_BTN:
	case ID_EVENT_3_BTN:
	case ID_EVENT_4_BTN:
	case ID_EVENT_5_BTN:
	case ID_EVENT_6_BTN:
	case ID_EVENT_7_BTN:
	case ID_EVENT_8_BTN:
	case ID_EVENT_9_BTN:
	case ID_EVENT_10_BTN:
		if( cmd == IOBN_BTNDOWN )
		{
			m_iSelectId = dwID;
			CheckRadioButton( ID_EVENT_1_BTN, ID_EVENT_1_BTN + m_vEventInfoVector.size(), m_iSelectId );
			SetImg( m_iSelectId - ID_EVENT_1_BTN );
		}
		break;
	case ID_EXIT_BTN:
		if( cmd == IOBN_BTNUP )
		{
			HideWnd();
		}
		break;
	case ID_DETAIL_BTN:
		if( cmd == IOBN_BTNUP )
		{
			SetShowEventPage( m_iSelectId - ID_EVENT_1_BTN );
		}
		break;
	}
}

void EventCatalogWnd::SetShowEventPage( int iVectorArray )
{
	if( !COMPARE( iVectorArray, 0, (int) m_vEventInfoVector.size() ) )
	{
		LOG.PrintTimeAndLog( 0, "%s VectorArray is Wrong(%d).", __FUNCTION__, iVectorArray );
		return;
	}

	EventInfo *pInfo = m_vEventInfoVector[iVectorArray];
	if( !pInfo )
	{
		LOG.PrintTimeAndLog( 0, "%s pInfo is NULL", __FUNCTION__ );
		return;
	}

	if( pInfo->m_bWebEvent )
	{
		// show webpage
		if( Setting::FullScreen() )
			ShowWindow( g_App.GetHWnd(), SW_MINIMIZE );
		Help::ExcuteWeb( pInfo->m_szEventURL.c_str() );
		m_bIERefreshCheck = true;
		m_dwIERefreshTimer= REALGETTIME();
		m_IEHwnd          = 0; // 초기화
	}
	else
	{
		g_GUIMgr.ShowWnd( pInfo->m_dwEventWndID );
	}
}

void EventCatalogWnd::OnProcess( float fTimePerSec )
{
	ioMovingWnd::OnProcess( fTimePerSec );
	ProcessIERefresh();
}

void EventCatalogWnd::SetShowHideChild( int iShowEventType /*= -1 */ )
{
	// erase
	vEventInfoVector::iterator iter = m_vEventInfoVector.begin();
	while ( iter != m_vEventInfoVector.end() )
	{
		EventInfo *pInfo = (*iter);
		if( !pInfo )
		{
			LOG.PrintTimeAndLog( 0, "%s pInfo == NULL.", __FUNCTION__ );
			return;
		}

		if( IsAlive( *pInfo ) )
		{
			++iter;
		}
		else
		{
			delete *iter;
			iter = m_vEventInfoVector.erase( iter );
		}
	}

	// show hide
	int iSize = m_vEventInfoVector.size();
	for (int i = 0; i < MAX_BTN ; i++)
	{
		ioWnd *pBtn = FindChildWnd( ID_EVENT_1_BTN + i );
		if( !pBtn )
		{
			LOG.PrintTimeAndLog( 0, "%s pBtn is NULL(%d).", __FUNCTION__, ID_EVENT_1_BTN + i );
			continue;
		}

		if( COMPARE( i , 0 , iSize ) )
			pBtn->ShowWnd();
		else
			pBtn->HideWnd();
	}

	// radom 셋팅
	if( iShowEventType == -1 )
	{
		m_iSelectId = ID_EVENT_1_BTN;
		if( iSize > 0 ) // block divide by zero
		{
			m_iSelectId =  ID_EVENT_1_BTN + ( rand()%iSize );
		}
	}
	else
		m_iSelectId = GetSelectIdByEventType( iShowEventType );

	CheckRadioButton( ID_EVENT_1_BTN, ID_EVENT_1_BTN+iSize, m_iSelectId );
	SetImg( m_iSelectId - ID_EVENT_1_BTN );
}

BOOL CALLBACK EventCatalogWnd::EnumProc(HWND hWnd,LPARAM lParam)
{
	if( m_IEHwnd != 0 )
		return TRUE;

	char szText[MAX_PATH] = "";
	GetWindowText(hWnd,szText,MAX_PATH);
	if( strcmp(szText,STR(1)) == 0 )
	{
		m_IEHwnd = hWnd;
	}
	
	return TRUE;
}

void EventCatalogWnd::ProcessIERefresh()
{
	if( !IsShow() )
		return;

	if( !m_bIERefreshCheck )
		return;

	enum { MAX_CHECK_TIME = 10000, };
	if( REALGETTIME() - m_dwIERefreshTimer > MAX_CHECK_TIME )
	{
		m_bIERefreshCheck = false;
		return;
	}

	EnumWindows( EnumProc, 0 );
	if( m_IEHwnd == 0 )
		return;

	PostMessage( m_IEHwnd, WM_KEYDOWN, VK_F5, 0L ); // ie8에서 ie를 바로 실행하면 일부 페이지가 보이지 않아 리프레쉬 메세지를 한번 보낸다.
	m_bIERefreshCheck = false;
}

void EventCatalogWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	enum { MAX_XML_VALUE = 30, };
	for (int i = 0; i < MAX_XML_VALUE ; i++)
	{
		EventInfo *pInfo = new EventInfo;
		if( !pInfo )
			continue;

		char szName[MAX_PATH]="";
		StringCbPrintf_e( szName, sizeof( szName ), "IsWebEvent%d", i+1 );
		pInfo->m_bWebEvent  = xElement.GetBoolAttribute( szName );

		ZeroMemory( szName, sizeof( szName ) );
		StringCbPrintf_e( szName, sizeof( szName ), "ImgFirst%d", i+1 );
		pInfo->m_szEventImgFirst  = xElement.GetStringAttribute( szName );

		ZeroMemory( szName, sizeof( szName ) );
		StringCbPrintf_e( szName, sizeof( szName ), "ImgSecond%d", i+1 );
		pInfo->m_szEventImgSecond = xElement.GetStringAttribute( szName );

		if( pInfo->m_bWebEvent )
		{
			ZeroMemory( szName, sizeof( szName ) );
			StringCbPrintf_e( szName, sizeof( szName ), "EventUrl%d", i+1 );
			pInfo->m_szEventURL   = xElement.GetStringAttribute( szName );
			ZeroMemory( szName, sizeof( szName ) );
			StringCbPrintf_e( szName, sizeof( szName ), "StartDate%d", i+1 );
			pInfo->m_dwStartDate  = xElement.GetIntAttribute( szName);
			ZeroMemory( szName, sizeof( szName ) );
			StringCbPrintf_e( szName, sizeof( szName ), "EndDate%d", i+1 );
			pInfo->m_dwEndDate    = xElement.GetIntAttribute( szName );
			ZeroMemory( szName, sizeof( szName ) );
			StringCbPrintf_e( szName, sizeof( szName ), "ChannelingType%d", i+1 );
			pInfo->m_eChannelingType = (ChannelingType) xElement.GetIntAttribute( szName );
			ZeroMemory( szName, sizeof( szName ) );
			StringCbPrintf_e( szName, sizeof( szName ), "EventType%d", i+1 );
			pInfo->m_iEventType   = xElement.GetIntAttribute( szName);
		}
		else
		{
			ZeroMemory( szName, sizeof( szName ) );
			StringCbPrintf_e( szName, sizeof( szName ), "EventWndID%d", i+1 );
			pInfo->m_dwEventWndID = xElement.GetIntAttribute( szName);
			ZeroMemory( szName, sizeof( szName ) );
			StringCbPrintf_e( szName, sizeof( szName ), "EventType%d", i+1 );
			pInfo->m_iEventType   = xElement.GetIntAttribute( szName);
		}	

		m_vEventInfoVector.push_back( pInfo );
	}
}

bool EventCatalogWnd::IsAlive( const EventInfo &kEventInfo )
{
	if( kEventInfo.m_bWebEvent )
	{
		if( kEventInfo.m_eChannelingType != CNT_NONE )
		{
			if( g_MyInfo.GetChannelingType() != kEventInfo.m_eChannelingType )
				return false;
		}


		time_t kCurServerTime = g_MyInfo.GetServerDate();
		kCurServerTime += ( REALGETTIME()/FLOAT1000 );// 현재까지의 클라이언트초를 넣는다.

		struct tm *pDate = localtime(&kCurServerTime);
		if( !pDate )
			return false;

		DWORD dwCurTime =( (pDate->tm_year+1900) * 1000000) + ( (pDate->tm_mon+1) * 10000) + (pDate->tm_mday * FLOAT100) + pDate->tm_hour;

		if( COMPARE( dwCurTime, kEventInfo.m_dwStartDate, kEventInfo.m_dwEndDate + 1 ) )
			return true;
	}
	else
	{
		if( g_EventMgr.IsAlive( (EventType)kEventInfo.m_iEventType, g_MyInfo.GetChannelingType() ) )
			return true;
	}

	return false;
}

bool EventCatalogWnd::iwm_spacebar()
{
	ioWnd *pBtn = FindChildWnd( ID_DETAIL_BTN );
	if( !pBtn ) return false;
	if( !pBtn->IsShow() ) return false;

	iwm_command( pBtn, IOBN_BTNUP, 0 );
	return true;
}

void EventCatalogWnd::iwm_create()
{
	m_dwStyle |= IWS_PROCESS_ALWAYS;
}

void EventCatalogWnd::SetImg( int iVectorArray )
{
	SAFEDELETE( m_pImgFirst );
	SAFEDELETE( m_pImgSecond );

	if( !COMPARE( iVectorArray, 0, (int) m_vEventInfoVector.size() ) )
	{
		LOG.PrintTimeAndLog( 0, "%s VectorArray is Wrong(%d).", __FUNCTION__, iVectorArray );
		return;
	}

	EventInfo *pInfo = m_vEventInfoVector[iVectorArray];
	if( !pInfo )
	{
		LOG.PrintTimeAndLog( 0, "%s pInfo is NULL", __FUNCTION__ );
		return;
	}

	m_pImgFirst  = g_UIImageSetMgr.CreateImageByFullName( pInfo->m_szEventImgFirst );
	m_pImgSecond = g_UIImageSetMgr.CreateImageByFullName( pInfo->m_szEventImgSecond );
}

void EventCatalogWnd::OnRender()
{
	ioMovingWnd::OnRender();

	int iXPos, iYPos;
	iXPos = GetDerivedPosX();
	iYPos = GetDerivedPosY();

	enum { X_OFFSET_FIRST = 17, Y_OFFSET_FIRST = 48, X_OFFSET_SECOND = 529, Y_OFFSET_SECOND = 48, };
	if( m_pImgFirst )
		m_pImgFirst->Render( iXPos + X_OFFSET_FIRST, iYPos + Y_OFFSET_FIRST );
	if( m_pImgSecond )
		m_pImgSecond->Render( iXPos + X_OFFSET_SECOND, iYPos + Y_OFFSET_SECOND );
		
}

int EventCatalogWnd::GetSelectIdByEventType( int iEventType )
{
	int iSize = m_vEventInfoVector.size();
	for (int i = 0; i < iSize ; i++)
	{
		EventInfo *pInfo = m_vEventInfoVector[i];
		if( !pInfo )
			continue;
		if( pInfo->m_iEventType == iEventType )
			return ( ID_EVENT_1_BTN + i );
	}

	return ID_EVENT_1_BTN;
}

void EventCatalogWnd::SendEventGiftRequest()
{
	IntVec vEventTypeVec;
	vEventTypeVec.reserve(10);
	g_MyInfo.GetEventUserMgr().GetSameClassEventTypeVec( EVT_ONE_DAY_GIFT, vEventTypeVec );
	int iSize = vEventTypeVec.size();
	for (int i = 0; i < iSize ; i++)
	{
		OneDayGiftEventUserNode *pEventNode = dynamic_cast<OneDayGiftEventUserNode*>( g_MyInfo.GetEventUserMgr().GetEventUserNode( (EventType)vEventTypeVec[i] ) );
		if( !pEventNode )
			return;

		if( pEventNode->GetValue( VA_OG_GET_GIFT ) == 1 ) // 선물을 받았다면
			return;

		SP2Packet kPacket( CTPK_EVENT_DATA_UPDATE );
		kPacket << (int) pEventNode->GetType();
		TCPNetwork::SendToServer( kPacket );
	}
}

//-----------------------------------------------------------------------------------------------------------------------------------

bool EvnetChildrenDay::m_bAutoShow = false;

EvnetChildrenDay::EvnetChildrenDay()
{
	m_pPlayStage = NULL;
}

EvnetChildrenDay::~EvnetChildrenDay()
{

}

void EvnetChildrenDay::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_EXIT:
	case ID_OK:
		if( cmd == IOBN_BTNUP )
		{
			HideWnd();
		}
		break;
	case ID_ACTION:
		if( cmd == IOBN_BTNUP )
		{
			HideWnd();
			if( m_pPlayStage )
			{
				SoldierSelectWnd *pSoldier = dynamic_cast<SoldierSelectWnd*>(g_GUIMgr.FindWnd( SOLDIER_SELECT_WND ));
				if( pSoldier && !pSoldier->IsShow() )
					pSoldier->SetSelectWaitShow( g_MyInfo.GetCharArrayToSlotIndex( g_MyInfo.GetSelectCharArray() ), m_pPlayStage->IsNetworkMode() ); 
			}
		}
		break;
	}
}

void EvnetChildrenDay::OnRender()
{
	ioWnd::OnRender();

	int iXPos, iYPos;
	iXPos = GetDerivedPosX();
	iYPos = GetDerivedPosY();

	g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
	g_FontMgr.SetAlignType( TAT_LEFT );
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );

	g_FontMgr.PrintText( iXPos + 18, iYPos + 266, FONT_SIZE_13, STR(1) );
	g_FontMgr.PrintText( iXPos + 18, iYPos + 286, FONT_SIZE_13, STR(2) );
	g_FontMgr.PrintText( iXPos + 18, iYPos + 306, FONT_SIZE_13, STR(3) );

	g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY ); // 주황
	g_FontMgr.PrintText( iXPos + 18, iYPos + 326, FONT_SIZE_13, STR(4) );
}

void EvnetChildrenDay::OnProcess( float fTimePerSec )
{
	if( m_bAutoShow && !IsShow() )
	{
		if( !g_EventMgr.IsAlive( EVT_CHILDRENDAY, g_MyInfo.GetChannelingType() ) )
		{
			m_bAutoShow = false;
			return;
		}

		ShowWnd();
		m_bAutoShow = false;
	}
}

bool EvnetChildrenDay::iwm_spacebar()
{
	ioWnd *pBtn = FindChildWnd( ID_ACTION );
	if( !pBtn ) return false;
	if( !pBtn->IsShow() ) return false;

	iwm_command( pBtn, IOBN_BTNUP, 0 );
	return true;
}

void EvnetChildrenDay::SetPlayStage( ioPlayStage *pPlayStage )
{
	m_pPlayStage = pPlayStage;
}
//////////////////////////////////////////////////////////////////////////
bool EventMortMainWnd::sg_bBonusPeso = false;
EventMortMainWnd::EventMortMainWnd()
{
	sg_bBonusPeso = false;
}

EventMortMainWnd::~EventMortMainWnd()
{

}

void EventMortMainWnd::iwm_hide()
{
	sg_bBonusPeso = false;
}

void EventMortMainWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_EXIT:
	case ID_OK:
		if( cmd == IOBN_BTNUP )
		{
			HideWnd();
		}
		break;
	}
}

void EventMortMainWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() )
	{
		if( sg_bBonusPeso )
		{
			if( g_GUIMgr.IsShow( MESSAGE_BOX ) )
				return;
			ShowWnd();
		}
		return;
	}	
}

void EventMortMainWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos, iYPos;
	iXPos = GetDerivedPosX();
	iYPos = GetDerivedPosY();

	ioComplexStringPrinter kPrinter;
	kPrinter.SetTextStyle( TS_OUTLINE_FULL_2X );
	kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );	
	kPrinter.SetBkColor( 0, 0, 0 );
	kPrinter.AddTextPiece( FONT_SIZE_13, STR(1));

	kPrinter.SetTextColor( TCT_DEFAULT_GRAY );	
	kPrinter.AddTextPiece( FONT_SIZE_13, STR(2));
	kPrinter.PrintFullText( iXPos + 105, iYPos + 55, TAT_LEFT );
	kPrinter.ClearList();

	kPrinter.SetTextColor( TCT_DEFAULT_GRAY );	
	kPrinter.AddTextPiece( FONT_SIZE_13, STR(3));
	kPrinter.PrintFullText( iXPos + 105, iYPos + 75, TAT_LEFT );
	kPrinter.ClearList();
}

//////////////////////////////////////////////////////////////////////////
EventChanceMortmainCharWnd::EventChanceMortmainCharWnd()
{
	m_pGaugeBack = NULL;
	m_pGaugeLine = NULL;
	m_pTimeText  = NULL;
	m_pTimeColon = NULL;
}

EventChanceMortmainCharWnd::~EventChanceMortmainCharWnd()
{
	SAFEDELETE( m_pGaugeBack );
	SAFEDELETE( m_pGaugeLine );
	SAFEDELETE( m_pTimeText );
	SAFEDELETE( m_pTimeColon );
}

void EventChanceMortmainCharWnd::iwm_show()
{
	if( !g_EventMgr.IsAlive( EVT_CHANCE_MORTMAIN_CHAR, g_MyInfo.GetChannelingType() ) || g_GUIMgr.IsShow( EVENT_CHANCE_ROLLING_WND ) )
	{
		// 이벤트 종료
		HideWnd();
	}
	else
	{
		ChanceMortmainCharEventUserNode *pEventNode = dynamic_cast<ChanceMortmainCharEventUserNode*>( g_MyInfo.GetEventUserMgr().GetEventUserNode( EVT_CHANCE_MORTMAIN_CHAR ) );
		if( pEventNode )
		{
			if( pEventNode->GetValue( VA_CMC_MAGIC_CODE ) >= 0 )
			{
				// 시간 게이지
				HideChildWnd( ID_NORMAL_CHANCE );
				HideChildWnd( ID_PCROOM_CHANCE );
				HideChildWnd( ID_PCROOM_ALARM );
			}
			else
			{
				// 찬스 사용
				ShowChildWnd( ID_NORMAL_CHANCE );
				ShowChildWnd( ID_PCROOM_CHANCE );
				ShowChildWnd( ID_PCROOM_ALARM );

				if( pEventNode->GetValue( VA_CMC_MAGIC_CODE ) == -1 )      //찬스
				{
					SetChildActive( ID_NORMAL_CHANCE );

					if( g_MyInfo.GetPCRoomAuthority()  || g_MyInfo.IsUserEvent() )
					{
						SetChildInActive( ID_PCROOM_CHANCE );
						HideChildWnd( ID_PCROOM_ALARM );
					}
					else
					{
						HideChildWnd( ID_PCROOM_CHANCE );
						ShowChildWnd( ID_PCROOM_ALARM );
					}
				}
				else if( pEventNode->GetValue( VA_CMC_MAGIC_CODE ) == -2 ) //PC방찬스
				{
					SetChildInActive( ID_NORMAL_CHANCE );
					SetChildActive( ID_PCROOM_CHANCE );
					HideChildWnd( ID_PCROOM_ALARM );
				}
				else if( pEventNode->GetValue( VA_CMC_MAGIC_CODE ) == -3 ) //찬스 + PC방찬스
				{
					SetChildActive( ID_NORMAL_CHANCE );
					SetChildActive( ID_PCROOM_CHANCE );
					HideChildWnd( ID_PCROOM_ALARM );
				}
			}
		}
	}
}

void EventChanceMortmainCharWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
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
	case ID_NORMAL_CHANCE:
		if( cmd == IOBN_BTNUP )
		{
			ChanceMortmainCharEventUserNode *pEventNode = dynamic_cast<ChanceMortmainCharEventUserNode*>( g_MyInfo.GetEventUserMgr().GetEventUserNode( EVT_CHANCE_MORTMAIN_CHAR ) );
			if( pEventNode )
			{
				if( pEventNode->GetValue( VA_CMC_MAGIC_CODE ) == -1 || pEventNode->GetValue( VA_CMC_MAGIC_CODE ) == -3 )
				{
					if( pEventNode->GetValue( VA_CMC_MAGIC_CODE ) == -3 )
						pEventNode->SetValue( VA_CMC_MAGIC_CODE, -2 );
					else
						pEventNode->SetValue( VA_CMC_MAGIC_CODE, 0 );

					// 찬스 사용
					EventChanceRollingWnd *pRolling = dynamic_cast<EventChanceRollingWnd *>( g_GUIMgr.FindWnd( EVENT_CHANCE_ROLLING_WND ) );
					if( pRolling )
					{
						pRolling->HideWnd();
						pRolling->StartRandAni();
						pRolling->ShowWnd();
					}

					SP2Packet kPacketPeso( CTPK_EVENT_DATA_UPDATE );
					kPacketPeso << EVT_CHANCE_MORTMAIN_CHAR;
					kPacketPeso << -1;
					TCPNetwork::SendToServer( kPacketPeso );
				}
			}
			HideWnd();
		}
		break;
	case ID_PCROOM_CHANCE:
		if( cmd == IOBN_BTNUP )
		{
			ChanceMortmainCharEventUserNode *pEventNode = dynamic_cast<ChanceMortmainCharEventUserNode*>( g_MyInfo.GetEventUserMgr().GetEventUserNode( EVT_CHANCE_MORTMAIN_CHAR ) );
			if( pEventNode )
			{
				if( pEventNode->GetValue( VA_CMC_MAGIC_CODE ) == -2 || pEventNode->GetValue( VA_CMC_MAGIC_CODE ) == -3 )
				{
					if( pEventNode->GetValue( VA_CMC_MAGIC_CODE ) == -3 )
						pEventNode->SetValue( VA_CMC_MAGIC_CODE, -1 );
					else
						pEventNode->SetValue( VA_CMC_MAGIC_CODE, 0 );

					// 찬스 사용
					EventChanceRollingWnd *pRolling = dynamic_cast<EventChanceRollingWnd *>( g_GUIMgr.FindWnd( EVENT_CHANCE_ROLLING_WND ) );
					if( pRolling )
					{
						pRolling->HideWnd();
						pRolling->StartRandAni();
						pRolling->ShowWnd();
					}

					SP2Packet kPacketPeso( CTPK_EVENT_DATA_UPDATE );
					kPacketPeso << EVT_CHANCE_MORTMAIN_CHAR;
					kPacketPeso << -2;
					TCPNetwork::SendToServer( kPacketPeso );
				}			
			}
			HideWnd();
		}
		break;
	case ID_PCROOM_ALARM:
		if( cmd == IOBN_BTNUP )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		}
		break;
	}
}

void EventChanceMortmainCharWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "GaugeBack" )
	{
		SAFEDELETE( m_pGaugeBack );
		m_pGaugeBack = pImage;
	}
	else if( szType == "GaugeLine" )
	{
		SAFEDELETE( m_pGaugeLine );
		m_pGaugeLine = pImage;
	}
	else if( szType == "TimeText" )
	{
		SAFEDELETE( m_pTimeText );
		m_pTimeText = pImage;
	}
	else if( szType == "TimeColon" )
	{
		SAFEDELETE( m_pTimeColon );
		m_pTimeColon = pImage;
	}
	else
		ioWnd::AddRenderImage( szType, pImage );
}

void EventChanceMortmainCharWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;

	if( !g_EventMgr.IsAlive( EVT_CHANCE_MORTMAIN_CHAR, g_MyInfo.GetChannelingType() ) )
	{
		// 이벤트 종료
		HideWnd();
	}
	else
	{
		ioMovingWnd::OnProcess( fTimePerSec );
	}
}

void EventChanceMortmainCharWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	ChanceMortmainCharEventUserNode *pEventNode = dynamic_cast<ChanceMortmainCharEventUserNode*>( g_MyInfo.GetEventUserMgr().GetEventUserNode( EVT_CHANCE_MORTMAIN_CHAR ) );
	if( pEventNode )
	{
		g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
		g_FontMgr.SetAlignType( TAT_CENTER );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );

		if( pEventNode->GetValue( VA_CMC_MAGIC_CODE ) >= 0 )
		{
			int iTotalMS = max( 1, g_EventMgr.GetValue( EVT_CHANCE_MORTMAIN_CHAR, EA_CHANCE_MORTMAIN_CHAR_TIME ) );
			int iCurMS = pEventNode->GetValue( VA_CMC_MAGIC_CODE );
			float fTempPer = (float)iCurMS / (float)iTotalMS;
			// 시간 게이지
			if( m_pGaugeBack && m_pGaugeLine && m_pTimeColon && m_pTimeText )
			{
				m_pGaugeBack->Render( iXPos + 17, iYPos + 68 );
				m_pGaugeLine->RenderWidthCut( iXPos + 17, iYPos + 68, 0, (float)m_pGaugeLine->GetWidth() * fTempPer );
				m_pTimeText->RenderNum( iXPos + 82, iYPos + 70, 0 );
				m_pTimeColon->Render( iXPos + 89, iYPos + 70 );

				int iNum = (float)( (float)iCurMS / 1000 ) / 60;
				if( iNum < 10 )
				{
					m_pTimeText->RenderNum( iXPos + 92, iYPos + 70, 0 );
					m_pTimeText->RenderNum( iXPos + 99, iYPos + 70, iNum );
				}
				else
				{
					m_pTimeText->RenderNum( iXPos + 92, iYPos + 70, iNum, -FLOAT1 );
				}
			}
			g_FontMgr.PrintText( iXPos + 94, iYPos + 38, FONT_SIZE_12, STR(1) );
		}
		else
		{
			// 찬스 사용
			g_FontMgr.PrintText( iXPos + 94, iYPos + 38, FONT_SIZE_12, STR(2) );
		}
	}
}
//////////////////////////////////////////////////////////////////////////
DWORD EventChanceRollingWnd::sg_dwCurRandTime = 0;
bool  EventChanceRollingWnd::sg_bShow = false;
int   EventChanceRollingWnd::sg_iOriginalBonus = 0;
EventChanceRollingWnd::EventChanceRollingWnd()
{
	m_pPesoIcon = NULL;
	m_pSoldierIcon = NULL;
	m_pLotteryText = NULL;
	m_pCongratulationText = NULL;
	
	m_dwCurRandTickTime = 0;
	m_dwRandTime = 0;
	m_dwRandTickTime = 0;
	m_iRandBonus = 0;
}

EventChanceRollingWnd::~EventChanceRollingWnd()
{
	SAFEDELETE( m_pPesoIcon );
	SAFEDELETE( m_pSoldierIcon );
	SAFEDELETE( m_pLotteryText );
	SAFEDELETE( m_pCongratulationText );
}

void EventChanceRollingWnd::SetBonusType( int iBonusType )
{
	sg_iOriginalBonus = iBonusType;
}

void EventChanceRollingWnd::iwm_show()
{
	sg_bShow = true;
	ChanceMortmainCharEventUserNode *pEventNode = dynamic_cast<ChanceMortmainCharEventUserNode*>( g_MyInfo.GetEventUserMgr().GetEventUserNode( EVT_CHANCE_MORTMAIN_CHAR ) );
	if( pEventNode )
	{
		if( pEventNode->GetValue( VA_CMC_MAGIC_CODE ) == -1 )
		{
			ioWnd *pChance = FindChildWnd( ID_CHANCE );
			if( pChance )
			{
				pChance->SetTitleText( STR(1) );
			}
		}
		else
		{
			ioWnd *pChance = FindChildWnd( ID_CHANCE );
			if( pChance )
			{
				pChance->SetTitleText( STR(2) );
			}
		}

		if( sg_dwCurRandTime != 0 || sg_iOriginalBonus == 0 )
		{
			SetChildInActive( ID_EXIT );
			SetChildInActive( ID_CHANCE );
			SetChildInActive( ID_OK );
			if( !m_szRollingSound.IsEmpty() )
				g_SoundMgr.PlaySound( m_szRollingSound, DSBVOLUME_MAX, PSM_LOOP | PSM_THREAD );
		}
		else
		{
			SetChildActive( ID_EXIT );
			SetChildActive( ID_OK );
			if( pEventNode->GetValue( VA_CMC_MAGIC_CODE ) < 0 )
				SetChildActive( ID_CHANCE );			

			if( !m_szRollingSound.IsEmpty() )
				g_SoundMgr.StopSound( m_szRollingSound, 0 );

			int iBonusType = sg_iOriginalBonus % 1000;
			if( iBonusType != 0 )
			{
				SAFEDELETE( m_pSoldierIcon );
				m_pSoldierIcon = g_MyInfo.GetSoldierIcon( iBonusType, true );

				if( !m_szCharSound.IsEmpty() )
					g_SoundMgr.PlaySound( m_szCharSound );
			}		
			else
			{
				if( !m_szPesoSound.IsEmpty() )
					g_SoundMgr.PlaySound( m_szPesoSound );
			}
		}
	}
	g_GUIMgr.HideWnd( EVENT_CHANCE_MORTMAIN_CHAR_WND );
}

void EventChanceRollingWnd::iwm_hide()
{
	if( !m_szRollingSound.IsEmpty() )
		g_SoundMgr.StopSound( m_szRollingSound, 0 );
	if( !m_szCharSound.IsEmpty() )
		g_SoundMgr.StopSound( m_szCharSound, 0 );
	if( !m_szPesoSound.IsEmpty() )
		g_SoundMgr.StopSound( m_szPesoSound, 0 );

	g_GUIMgr.ShowWnd( EVENT_CHANCE_MORTMAIN_CHAR_WND, true );
}

bool EventChanceRollingWnd::iwm_esc()
{
	if( sg_dwCurRandTime != 0 ) 
		return false;
	
	if( ioWnd::iwm_esc() )
	{
		sg_bShow = false;
		return true;
	}

	return false;
}

void EventChanceRollingWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_EXIT:
	case ID_OK:
		if( cmd == IOBN_BTNUP )
		{
			sg_bShow = false;
			HideWnd();
		}
		break;
	case ID_CHANCE:
		if( cmd == IOBN_BTNUP )
		{
			ChanceMortmainCharEventUserNode *pEventNode = dynamic_cast<ChanceMortmainCharEventUserNode*>( g_MyInfo.GetEventUserMgr().GetEventUserNode( EVT_CHANCE_MORTMAIN_CHAR ) );
			if( pEventNode )
			{
				if( pEventNode->GetValue( VA_CMC_MAGIC_CODE ) == -1 || pEventNode->GetValue( VA_CMC_MAGIC_CODE ) == -2 )
				{
					HideWnd();
					SP2Packet kPacket( CTPK_EVENT_DATA_UPDATE );
					kPacket << EVT_CHANCE_MORTMAIN_CHAR;
					kPacket << pEventNode->GetValue( VA_CMC_MAGIC_CODE );
					TCPNetwork::SendToServer( kPacket );
					pEventNode->SetValue( VA_CMC_MAGIC_CODE, 0 );
					
					StartRandAni();
					ShowWnd();
				}			
			}
		}
		break;
	}
}

void EventChanceRollingWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	m_dwRandTime		= xElement.GetIntAttribute_e( "RandTime" );
	m_dwRandTickTime	= xElement.GetIntAttribute_e( "RandTickTime" );
	m_szRollingSound    = xElement.GetStringAttribute_e( "RollingSound" );
	m_szPesoSound       = xElement.GetStringAttribute_e( "PesoSound" );  
	m_szCharSound       = xElement.GetStringAttribute_e( "CharSound" );
}

void EventChanceRollingWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "PesoIcon" )
	{
		SAFEDELETE( m_pPesoIcon );
		m_pPesoIcon = pImage;
	}
	else if( szType == "LotteryText" )
	{
		SAFEDELETE( m_pLotteryText );
		m_pLotteryText = pImage;
	}
	else if( szType == "CongratulationText" )
	{
		SAFEDELETE( m_pCongratulationText );
		m_pCongratulationText = pImage;
	}
	else
		ioWnd::AddRenderImage( szType, pImage );
}

void EventChanceRollingWnd::StartRandAni()
{
	sg_dwCurRandTime = FRAMEGETTIME();
	sg_iOriginalBonus= 0;
}

void EventChanceRollingWnd::OnProcessRand()
{
	if( sg_dwCurRandTime == 0 ) return;
	if( FRAMEGETTIME() - m_dwCurRandTickTime < m_dwRandTickTime ) return;

	m_dwCurRandTickTime = FRAMEGETTIME();
	DWORD dwGap = FRAMEGETTIME() - sg_dwCurRandTime;
	if( dwGap > m_dwRandTime && sg_iOriginalBonus != 0 )
	{
		sg_dwCurRandTime = 0;
		SetChildActive( ID_EXIT );		
		SetChildActive( ID_OK );
		ChanceMortmainCharEventUserNode *pEventNode = dynamic_cast<ChanceMortmainCharEventUserNode*>( g_MyInfo.GetEventUserMgr().GetEventUserNode( EVT_CHANCE_MORTMAIN_CHAR ) );
		if( pEventNode )
		{
			if( pEventNode->GetValue( VA_CMC_MAGIC_CODE ) < 0 )
				SetChildActive( ID_CHANCE );
		}

		if( !m_szRollingSound.IsEmpty() )
			g_SoundMgr.StopSound( m_szRollingSound, 0 );			

		int iBonusType = sg_iOriginalBonus % 1000;
		if( iBonusType != 0 )
		{
			SAFEDELETE( m_pSoldierIcon );
			m_pSoldierIcon = g_MyInfo.GetSoldierIcon( iBonusType, true );

			ioComplexStringPrinter kPrinter;
			g_ChatMgr.PresetChatComplexString( MSG_ANNOUNCE_CHAT, kPrinter );
			kPrinter.SetTextColor( TCT_DEFAULT_VIOLET );
			kPrinter.AddTextPiece( g_ChatMgr.GetChatFontScale(), STR(1), g_MyInfo.GetPublicID().c_str(), g_MyInfo.GetClassName( iBonusType ) );
			g_ChatMgr.SetChatComplexString( STR(2), kPrinter );

			if( !m_szCharSound.IsEmpty() )
				g_SoundMgr.PlaySound( m_szCharSound );
		}		
		else
		{
			if( !m_szPesoSound.IsEmpty() )
				g_SoundMgr.PlaySound( m_szPesoSound );
		}
	}
	else
	{
		// 랜덤 보상 변경
		int iBonusType = m_iRandBonus % 1000;
		if( iBonusType == 0 )    // 페소 보상이었으면 용병 보상으로 
		{
			IntVec vClassType;
			vClassType.reserve(10);
			int iSize = g_ClassPrice.MaxClassPrice();
			for (int i = 0; i < iSize ; i++)
			{
				if( !g_ClassPrice.IsActiveClass( i ) )
					continue;
				if( g_ClassPrice.IsCashOnlyByArray( i ) )
					continue;
				vClassType.push_back( i + 1 );
			}
			int iRand = rand()%vClassType.size();
			m_iRandBonus = 1;
			if( COMPARE( iRand, 0, (int) vClassType.size() ) )
				m_iRandBonus = vClassType[iRand];
			vClassType.clear();

			SAFEDELETE( m_pSoldierIcon );
			m_pSoldierIcon = g_MyInfo.GetSoldierIcon( m_iRandBonus, true );
		}
		else                     // 페소 보상으로
		{
			int r = rand()%6;
			switch( r )
			{
			case 0:
				m_iRandBonus = 1000000;         //FLOAT1000페소
				break;
			case 1:
				m_iRandBonus = 700000;
				break;
			case 2:
				m_iRandBonus = 500000;
				break;
			case 3:
				m_iRandBonus = 300000;
				break;
			case 4:
				m_iRandBonus = 200000;
				break;
			case 5:
				m_iRandBonus = 100000;
				break;
			}
		}
	}
}

void EventChanceRollingWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() )
	{
		if( sg_bShow )
			ShowWnd();
		return;
	}

	OnProcessRand();
	ioMovingWnd::OnProcess( fTimePerSec );
}

void EventChanceRollingWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	ChanceMortmainCharEventUserNode *pEventNode = dynamic_cast<ChanceMortmainCharEventUserNode*>( g_MyInfo.GetEventUserMgr().GetEventUserNode( EVT_CHANCE_MORTMAIN_CHAR ) );
	if( pEventNode )
	{
		if( sg_dwCurRandTime == 0 )
		{
			// 오리지날 보상
			RenderOriginalBonus( iXPos, iYPos );
		}
		else 
		{
			// 랜덤 보상
			RenderRandBonus( iXPos, iYPos );
		}
	}
}

void EventChanceRollingWnd::RenderOriginalBonus( int iXPos, int iYPos )
{
	int iBonusType = sg_iOriginalBonus % 1000;
	if( iBonusType == 0 )
	{
		// 페소 보상
		if( m_pPesoIcon )
			m_pPesoIcon->Render( iXPos + 161, iYPos + 92 );

		char szConvertNum[MAX_PATH] = "";
		Help::ConvertNumToStrComma( sg_iOriginalBonus / 1000, szConvertNum, sizeof( szConvertNum ) );
		g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
		g_FontMgr.SetAlignType( TAT_CENTER );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
		g_FontMgr.PrintText( iXPos + 161, iYPos + 168, FONT_SIZE_13, STR(1), szConvertNum );
		g_FontMgr.SetTextColor( 255, 255, 255 );
		g_FontMgr.PrintText( iXPos + 161, iYPos + 188, FONT_SIZE_13, STR(2) );
	}
	else
	{
		// 용병 보상
		if( m_pSoldierIcon )
			m_pSoldierIcon->Render( iXPos + 161, iYPos + 92 );

		g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
		g_FontMgr.SetAlignType( TAT_CENTER );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
		g_FontMgr.PrintText( iXPos + 161, iYPos + 168, FONT_SIZE_13, STR(3), g_MyInfo.GetClassName( iBonusType ) );
		g_FontMgr.SetTextColor( 255, 255, 255 );
		g_FontMgr.PrintText( iXPos + 161, iYPos + 188, FONT_SIZE_13, STR(4) );		
	}

	if( m_pCongratulationText )
		m_pCongratulationText->Render( iXPos + 161, iYPos + 132 );
}

void EventChanceRollingWnd::RenderRandBonus( int iXPos, int iYPos )
{
	int iBonusType = m_iRandBonus % 1000;
	if( iBonusType == 0 )
	{
		// 페소 보상
		if( m_pPesoIcon )
			m_pPesoIcon->Render( iXPos + 161, iYPos + 92 );

		char szConvertNum[MAX_PATH] = "";
		Help::ConvertNumToStrComma( m_iRandBonus / 1000, szConvertNum, sizeof( szConvertNum ) );
		g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
		g_FontMgr.SetAlignType( TAT_CENTER );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextColor( 255, 255, 255 );
		g_FontMgr.PrintText( iXPos + 161, iYPos + 178, FONT_SIZE_13, STR(1), szConvertNum );
	}
	else
	{
		// 용병 보상
		if( m_pSoldierIcon )
			m_pSoldierIcon->Render( iXPos + 161, iYPos + 92 );

		g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
		g_FontMgr.SetAlignType( TAT_CENTER );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextColor( 255, 255, 255 );
		g_FontMgr.PrintText( iXPos + 161, iYPos + 178, FONT_SIZE_13, STR(2), g_MyInfo.GetClassName( iBonusType ) );
	}

	if( m_pLotteryText )
		m_pLotteryText->Render( iXPos + 161, iYPos + 132 );
}
//////////////////////////////////////////////////////////////////////////
OneDayGoldItemEventWnd::OneDayGoldItemEventWnd()
{
	m_dwState			= 0;
	m_iRollingIconArray = 0;
	m_dwCurrentFullTime = 0;
	m_dwRollingFullTime = 0;
	m_dwCurrentTickTime = 0;
	m_dwRollingTickTime = 0;
	m_iPresentType		= 0;
	m_iPresentValue1	= 0;
	m_iPresentValue2	= 0;
	m_iPresentValue3	= 0;
	m_iPresentValue4	= 0;
	m_bAlarm			= false;
	m_fCompleteIconScale = 1.50f;
	m_dwCompleteScaleTimer = 0;

	m_pBackRedLight = NULL;
	m_pBackBlueLight = NULL;
	m_pRecvIcon = NULL;
	m_pTextStateRecv = NULL;
	m_pTextStateRolling = NULL;
	m_pTextStateComplete = NULL;
	m_pPresentIcon = NULL;
	m_pRollingIcon = NULL;           // 메모리 해제 안함.
}

OneDayGoldItemEventWnd::~OneDayGoldItemEventWnd()
{
	SAFEDELETE( m_pBackRedLight );
	SAFEDELETE( m_pBackBlueLight );
	SAFEDELETE( m_pRecvIcon );
	SAFEDELETE( m_pTextStateRecv );
	SAFEDELETE( m_pTextStateRolling );
	SAFEDELETE( m_pTextStateComplete );
	SAFEDELETE( m_pPresentIcon );

	ioUIRenderImageVec::iterator iter = m_vRollingIconList.begin(); 
	for(;iter != m_vRollingIconList.end();iter++)
	{
		ioUIRenderImage *pImage = *iter;
		SAFEDELETE( pImage );
	}
	m_vRollingIconList.clear();
}

void OneDayGoldItemEventWnd::iwm_show()
{
	switch( m_dwState )
	{
	case STATE_NONE:
		HideWnd();
		break;
	case STATE_RECV:
		{
			ShowChildWnd( ID_RECV_ITEM );
			HideChildWnd( ID_LINK );

			SetChildInActive( ID_EXIT );
			SetChildActive( ID_RECV_ITEM );
		}
		break;
	case STATE_ROLLING:
		{
			ShowChildWnd( ID_RECV_ITEM );
			HideChildWnd( ID_LINK );

			SetChildInActive( ID_EXIT );
			SetChildInActive( ID_RECV_ITEM );
		}
		break;
	case STATE_COMPLETE:
		{
			HideChildWnd( ID_RECV_ITEM );
			ShowChildWnd( ID_LINK );

			SetChildActive( ID_EXIT );

			if( m_pPresentIcon )
			{
				m_fCompleteIconScale   = 1.50f;
				m_dwCompleteScaleTimer = FRAMEGETTIME();
				m_pPresentIcon->SetScale( m_fCompleteIconScale );
			}
		}
		break;
	}
}

void OneDayGoldItemEventWnd::iwm_hide()
{
	m_dwState = STATE_NONE;
	if( !m_szRollingSound.IsEmpty() )
		g_SoundMgr.StopSound( m_szRollingSound, 0 );
	if( !m_szCompleteSound.IsEmpty() )
		g_SoundMgr.StopSound( m_szCompleteSound, 0 );
}

bool OneDayGoldItemEventWnd::iwm_esc()
{
	if( !IsShow() ) return false;

	if( m_dwState == STATE_COMPLETE )
		return ioWnd::iwm_esc();
	return false;
}

bool OneDayGoldItemEventWnd::iwm_spacebar()
{
	if( !IsShow() ) return false;

	if( m_dwState == STATE_COMPLETE )
	{
		iwm_command( FindChildWnd( ID_LINK ), IOBN_BTNUP, 0 );
		return true;
	}
	return false;
}

void OneDayGoldItemEventWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
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
	case ID_RECV_ITEM:
		if( cmd == IOBN_BTNUP )
		{
			SP2Packet kPacket( CTPK_EVENT_DATA_UPDATE );
			kPacket << EVT_ONE_DAY_GOLD_ITEM;			
			TCPNetwork::SendToServer( kPacket );
			TCPNetwork::MouseBusy( true );

			SetRollingState();
		}
		break;
	case ID_LINK:
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
	}
}

void OneDayGoldItemEventWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	m_dwRollingFullTime	= xElement.GetIntAttribute_e( "RollingFullTime" );
	m_dwRollingTickTime	= xElement.GetIntAttribute_e( "RollingTickTime" );
	m_szRollingSound    = xElement.GetStringAttribute_e( "RollingSound" );
	m_szCompleteSound   = xElement.GetStringAttribute_e( "CompleteSound" );  
}

void OneDayGoldItemEventWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "BackRedLight" )
	{
		SAFEDELETE( m_pBackRedLight );
		m_pBackRedLight = pImage;
	}
	else if( szType == "BackBlueLight" )
	{
		SAFEDELETE( m_pBackBlueLight );
		m_pBackBlueLight = pImage;
	}
	else if( szType == "RecvIcon" )
	{
		SAFEDELETE( m_pRecvIcon );
		m_pRecvIcon = pImage;
	}
	else if( szType == "TextStateRecv" )
	{
		SAFEDELETE( m_pTextStateRecv );
		m_pTextStateRecv = pImage;
	}
	else if( szType == "TextStateRolling" )
	{
		SAFEDELETE( m_pTextStateRolling );
		m_pTextStateRolling = pImage;
	}
	else if( szType == "TextStateComplete" )
	{
		SAFEDELETE( m_pTextStateComplete );
		m_pTextStateComplete = pImage;
	}
	else
		ioWnd::AddRenderImage( szType, pImage );
}

void OneDayGoldItemEventWnd::SetRecvState()
{
	HideWnd();
	m_dwState = STATE_RECV;
	ShowWnd();
}

void OneDayGoldItemEventWnd::SetRollingState()
{
	HideWnd();
	m_dwState = STATE_ROLLING;

	// ICON Setting
	m_vRollingIconList.push_back( g_UIImageSetMgr.CreateImageByFullName( "UIIconPack6#goods_yell" ) );				// 확성기
	m_vRollingIconList.push_back( g_UIImageSetMgr.CreateImageByFullName( "UIIconPack9#goods_fishing_bait2" ) );     // 달빛시공미끼
	m_vRollingIconList.push_back( g_UIImageSetMgr.CreateImageByFullName( "UIIconPack9#goods_fishing_rod2" ) );      // 달빛시공낚싯대
	m_vRollingIconList.push_back( g_UIImageSetMgr.CreateImageByFullName( "UIIconPack8#goods_growrecovery" ) );      // 강화복구권
	m_vRollingIconList.push_back( g_UIImageSetMgr.CreateImageByFullName( "UIIconPack8#goods_exp100pro" ) );			// 경험치 100%
	m_vRollingIconList.push_back( g_UIImageSetMgr.CreateImageByFullName( "UIIconPack8#goods_peso40pro" ) );			// 페소+40%
	m_vRollingIconList.push_back( g_UIImageSetMgr.CreateImageByFullName( "UIIconPack6#goods_namechange" ) );		// 프리미엄 닉네임
	m_vRollingIconList.push_back( g_UIImageSetMgr.CreateImageByFullName( "UIIconPack6#goods_guildcreate" ) );		// 길드생성권
	m_vRollingIconList.push_back( g_UIImageSetMgr.CreateImageByFullName( "UIIconPack6#goods_guildmarkchange" ) );   // 길드마크변경권
	m_vRollingIconList.push_back( g_UIImageSetMgr.CreateImageByFullName( "UIIconPack9#goods_guildnamechange" ) );   // 프리미엄길드명
	m_vRollingIconList.push_back( g_UIImageSetMgr.CreateImageByFullName( "UIIconPack6#goods_friendlistplus" ) );    // 친구목록 확장
	m_vRollingIconList.push_back( g_UIImageSetMgr.CreateImageByFullName( "UIIconPack8#goods_slotplus" ) );			// 용병슬롯확장+6

	m_pRollingIcon = m_vRollingIconList[0];
	m_iRollingIconArray = 0;
	m_dwCurrentFullTime = FRAMEGETTIME();
	m_dwCurrentTickTime = FRAMEGETTIME();

	if( !m_szRollingSound.IsEmpty() )
		g_SoundMgr.PlaySound( m_szRollingSound, DSBVOLUME_MAX, PSM_LOOP | PSM_THREAD );
	ShowWnd();
}

void OneDayGoldItemEventWnd::SetCompleteState( short iPresentType, int iPresentValue1, int iPresentValue2, int iPresentValue3, int iPresentValue4, bool bAlarm )
{
	//
	m_iPresentType   = iPresentType;
	m_iPresentValue1 = iPresentValue1;
	m_iPresentValue2 = iPresentValue2;
	m_iPresentValue3 = iPresentValue3;
	m_iPresentValue4 = iPresentValue4;
	m_bAlarm		 = bAlarm;

	SAFEDELETE( m_pPresentIcon );
	switch( m_iPresentType )
	{
	case PRESENT_SOLDIER:
		{
			m_pPresentIcon = g_UIImageSetMgr.CreateImageByFullName( g_MyInfo.GetMySoldierIconName( m_iPresentValue1 ) );
		}
		break;
	case PRESENT_DECORATION:
		{
			int iClassType = m_iPresentValue1 / 100000;
			int iSexType   = (m_iPresentValue1 % 100000) / 1000;
			int iDecoType  = m_iPresentValue1 % 1000;
			const DecoData &kData = g_DecorationPrice.GetDecoData( iSexType, iDecoType, m_iPresentValue2 );
			m_pPresentIcon = g_UIImageSetMgr.CreateImageByFullName( kData.m_szIcon );
		}
		break;
	case PRESENT_ETC_ITEM:
		{
			ioEtcItem *pEtcItem = g_EtcItemMgr.FindEtcItem( m_iPresentValue1 );
			if( pEtcItem )
			{
				m_pPresentIcon = g_UIImageSetMgr.CreateImageByFullName( pEtcItem->GetIconName() );
			}			
		}
		break;
	case PRESENT_PESO:
		{
			m_pPresentIcon = g_UIImageSetMgr.CreateImageByFullName( ioHashString( "UIIconPack13#quest_002" ) );
		}
		break;
	case PRESENT_RANDOM_DECO:
		{
			int iClassType = m_iPresentValue1 / 100000;
			int iSexType   = (m_iPresentValue1 % 100000) / 1000;
			int iDecoType  = m_iPresentValue1 % 1000;
			const DecoData &kData = g_DecorationPrice.GetDecoData( iSexType, iDecoType, m_iPresentValue2 );
			m_pPresentIcon = g_UIImageSetMgr.CreateImageByFullName( kData.m_szIcon );
		}
		break;
	case PRESENT_GRADE_EXP:
		{
			m_pPresentIcon = g_UIImageSetMgr.CreateImageByFullName( ioHashString( "UIIconPack13#quest_001" ) );
		}
		break;
	}	
}

void OneDayGoldItemEventWnd::ProcessRolling()
{
	if( m_dwState != STATE_ROLLING ) return;

	if( FRAMEGETTIME() - m_dwCurrentTickTime > m_dwRollingTickTime )
	{
		m_dwCurrentTickTime = FRAMEGETTIME();
		m_iRollingIconArray++;
		if( !COMPARE( m_iRollingIconArray, 0, (int)m_vRollingIconList.size() ) )
			m_iRollingIconArray = 0;
		m_pRollingIcon = m_vRollingIconList[m_iRollingIconArray];
	}

	if( FRAMEGETTIME() - m_dwCurrentFullTime > m_dwRollingFullTime && m_iPresentType != 0 )
	{
		// 완료
		HideWnd();
		m_dwState = STATE_COMPLETE;
		if( m_bAlarm )
		{
			SP2Packet kPacket( CTPK_SERVER_ALARM_MSG );
			kPacket << UDP_SERVER_ALARM_GOLD_ITEM_EVENT << g_MyInfo.GetPublicID() << m_iPresentType << m_iPresentValue1 << m_iPresentValue2;
			TCPNetwork::SendToServer( kPacket );
		}

		if( !m_szCompleteSound.IsEmpty() )
			g_SoundMgr.PlaySound( m_szCompleteSound );
		ShowWnd();
	}
}

void OneDayGoldItemEventWnd::ProcessComplete()
{
	if( m_dwState != STATE_COMPLETE ) return;
	if( m_dwCompleteScaleTimer == 0 ) return;

	if( m_pPresentIcon )
	{
		DWORD dwGapTime = FRAMEGETTIME() - m_dwCompleteScaleTimer;
		if( dwGapTime < FLOAT100 )
		{
			m_fCompleteIconScale = FLOAT1 + ( FLOAT05 * ( (float)( FLOAT100 - dwGapTime ) / FLOAT100 ) );
		}
		else
		{
			m_dwCompleteScaleTimer = 0;
			m_fCompleteIconScale   = FLOAT1;
		}
		m_pPresentIcon->SetScale( m_fCompleteIconScale );
	}
}

void OneDayGoldItemEventWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;

	ProcessRolling();
	ProcessComplete();
}

void OneDayGoldItemEventWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();
	switch( m_dwState )
	{
	case STATE_RECV:
		OnStateRecvRender( iXPos, iYPos );
		break;
	case STATE_ROLLING:
		OnStateRollingRender( iXPos, iYPos );
		break;
	case STATE_COMPLETE:
		OnStateCompleteRender( iXPos, iYPos );
		break;
	}
}

void OneDayGoldItemEventWnd::OnStateRecvRender( int iXPos, int iYPos )
{
	if( !m_pBackRedLight || !m_pRecvIcon || !m_pTextStateRecv ) return;

	m_pBackRedLight->Render( iXPos + 161, iYPos + 92 );
	m_pRecvIcon->Render( iXPos + 161, iYPos + 92 );
	m_pTextStateRecv->Render( iXPos + 161, iYPos + 132 );

	g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
	g_FontMgr.PrintText( iXPos + 161, iYPos + 168, FONT_SIZE_13, STR(1) );
	g_FontMgr.PrintText( iXPos + 161, iYPos + 190, FONT_SIZE_13, STR(2) );

	g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
	g_FontMgr.PrintText( iXPos + 161, iYPos + 212, FONT_SIZE_13, STR(3) );
}

void OneDayGoldItemEventWnd::OnStateRollingRender( int iXPos, int iYPos )
{
	if( !m_pBackRedLight || !m_pRollingIcon || !m_pTextStateRolling ) return;

	m_pBackRedLight->Render( iXPos + 161, iYPos + 92 );
	m_pRollingIcon->Render( iXPos + 161, iYPos + 92 );
	m_pTextStateRolling->Render( iXPos + 161, iYPos + 132 );

	g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
	g_FontMgr.PrintText( iXPos + 161, iYPos + 168, FONT_SIZE_13, STR(1) );
	g_FontMgr.PrintText( iXPos + 161, iYPos + 190, FONT_SIZE_13, STR(2) );

	g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
	g_FontMgr.PrintText( iXPos + 161, iYPos + 212, FONT_SIZE_13, STR(3) );
}

void OneDayGoldItemEventWnd::OnStateCompleteRender( int iXPos, int iYPos )
{
	if( !m_pBackBlueLight || !m_pPresentIcon || !m_pTextStateComplete ) return;

	m_pBackBlueLight->Render( iXPos + 161, iYPos + 92 );
	m_pPresentIcon->Render( iXPos + 161, iYPos + 92, UI_RENDER_NORMAL, TFO_BILINEAR );
	m_pTextStateComplete->Render( iXPos + 161, iYPos + 132 );

	g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
	g_FontMgr.PrintText( iXPos + 161, iYPos + 168, FONT_SIZE_13, STR(1) );
	g_FontMgr.PrintText( iXPos + 161, iYPos + 190, FONT_SIZE_13, STR(2), g_PresentMgr.GetPresentValue1Text( m_iPresentType, m_iPresentValue1, m_iPresentValue2 ).c_str(),
																		 g_PresentMgr.GetPresentValue2Text( m_iPresentType, m_iPresentValue1, m_iPresentValue2 ).c_str() );
	g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
	g_FontMgr.PrintText( iXPos + 161, iYPos + 212, FONT_SIZE_13, STR(3) );
}