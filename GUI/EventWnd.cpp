#include "StdAfx.h"

#include "../io3DEngine/ioWnd.h"
#include "../io3DEngine/ioButton.h"
#include "../io3DEngine/ioINILoader.h"
#include "../io3DEngine/ioUIRenderImage.h"
#include "../io3DEngine/ioUIRenderFrame.h"
#include "../io3DEngine/ioXMLElement.h"
#include "../io3DEngine/ioFontManager.h"
#include "../io3dengine/ioUITitle.h"
#include "../io3DEngine/HelpFunc.h"
#include "../io3DEngine/iostringmanager.h"
#include "../io3dengine/safesprintf.h"
#include "../io3DEngine/ioUIImageSetManager.h"

#include "../ioDateHelp.h"
#include "../ioMyInfo.h"
#include "../ioEventUserNode.h"
#include "../NetworkWrappingFunc.h"
#include "../ioApplication.h"
#include "../ioSetItemInfoManager.h"
#include "../ioClassPrice.h"
#include "../ioPlayStage.h"
#include "../ioMyLevelMgr.h"
#include "../EtcHelpFunc.h"
#include "../WndID.h"
#include "../ioPlayMode.h"
#include "../ioItemMaker.h"
#include "../ioBaseChar.h"
#include "../ioBowItem.h"
#include "../ioChargeComboBowItem.h"
#include "../ioFireWeaponItem.h"
#include "../ioLaserItem.h"
#include "../ioAutoShotItem.h"
#include "../ioUroborusItem.h"
#include "../ioWereWolfItem.h"
#include "../ioWitchItem.h"
#include "../ioExcaliburItem.h"
#include "../ioMechanicsItem.h"
#include "../ioChargeCommandItem.h"
#include "../ioPhantomItem.h"
#include "../ioNakoruruItem.h"
#include "../ioDestroyerItem.h"
#include "../ioMichaelItem.h"
#include "../ioPeterpanItem.h"
#include "../ioCyvogueItem.h"
#include "../ioDarkSeasItem.h"
#include "../ioWindLordItem.h"
#include "../ioLeeItem.h"
#include "../ioProject_K_Item.h"
#include "../ioDevilItem.h"
#include "../ioFlash_Item.h"
#include "../ioPsychoMetry_Item.h"
#include "../ioSuddenAttackItem.h"
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
#include "../ioJeonWooChiItem.h"
#include "../ioChainMagicianItem.h"
#include "../ioStriderItem.h"
#include "../ioCuchulainItem.h"
#include "../ioOniMushaItem.h"
#include "../ioWestGunExtendItem.h"
#include "../ioSasinmuItem.h"
#include "../ioPredatorItem.h"

#include "../Setting.h"
#include "..\TextColorTable.h"
#include "..\ioEtcItem.h"
#include "..\ioExtraItemInfoManager.h"
#include "..\ioEtcItemManager.h"

#include "ioSP2GUIManager.h"
#include "SoldierSelectWnd.h"
#include "SoldierManualWnd.h"
#include "EventCatalogWnd.h"
#include ".\eventwnd.h"
#include <strsafe.h>
#include "ioMessageBox.h"
#include "..\io3DEngine\ioScroll.h"

EventWnd::EventWnd(void)
{
}

EventWnd::~EventWnd(void)
{
}

void EventWnd::UpdateFirstClass( OUT IntVec &rvDeleteArrayList )
{
	m_dwFirstClassList.clear();

	ioINILoader_e kLoader( "config/sp2_event.ini" );
	kLoader.SetTitle_e( "EVENT_CREATE_INFO" );
	int max_item = kLoader.LoadInt_e( "max_item", 0 );

	char szKeyName[MAX_PATH];
	for(int i = 0;i < max_item;i++)
	{
		sprintf_e( szKeyName, "item_code_%d", i + 1);
		int iClassType = kLoader.LoadInt( szKeyName, 0 );
		DWORD dwSetItemCode = iClassType + SET_ITEM_CODE;
		const ioSetItemInfo *pSetItemInfo = g_SetItemInfoMgr.GetSetInfoByCode( dwSetItemCode );
		if( g_MyInfo.IsCanBuyItem( pSetItemInfo ) )
			m_dwFirstClassList.push_back( iClassType );
		else
			rvDeleteArrayList.push_back( i );
	}		
}

void EventWnd::LoadINI( IN const IntVec &rvDeleteArrayList )
{
	m_szHelpTitleVec.clear();
	for(StringVectorList::iterator it = m_DescVecList.begin(); it != m_DescVecList.end(); ++it)
	{
		StringVector &szVec = *it;    
		szVec.clear();
	}
	m_DescVecList.clear();

	ioINILoader_e kLoader( "config/sp2_event.ini" );
	kLoader.SetTitle_e( "EVENT_UI_HELP" );
	int max_help = kLoader.LoadInt_e( "max_help", 0 );

	char szKeyName[MAX_PATH]="";
	for (int i = 0; i < max_help ; i++)
	{
		// class list에서 삭제되는 항목은 help에서도 삭제한다
		bool bContinue = false;
		for(IntVec::const_iterator it = rvDeleteArrayList.begin(); it != rvDeleteArrayList.end(); ++it)
		{
			const int &rArray = *it;
			if( i == rArray ) 
			{
				bContinue = true;
				break;
			}
		}
		if( bContinue )
			continue;

		ZeroMemory(szKeyName, sizeof( szKeyName ) );
		StringCbPrintf_e( szKeyName, sizeof(szKeyName), "title_%d", i+1);

		char szBuf[MAX_PATH]="";
		kLoader.LoadString( szKeyName, "", szBuf, MAX_PATH );
		m_szHelpTitleVec.push_back( ioHashString(szBuf) );

		ZeroMemory(szKeyName, sizeof( szKeyName ) );
		StringCbPrintf_e( szKeyName, sizeof(szKeyName), "desc_%d", i+1);

		ZeroMemory( szBuf, sizeof( szBuf) );
		kLoader.LoadString( szKeyName, "", szBuf, MAX_PATH );
		StringVector szDescVec;
		SplitDescription( ioHashString(szBuf) , szDescVec );	
		m_DescVecList.push_back( szDescVec );
		szDescVec.clear();
	}
}

void EventWnd::SetBtnTitle()
{
	ioButton *pFirsHireBtn = (ioButton*)FindChildWnd( ID_GET_CHAR_BTN );
	if( pFirsHireBtn )
	{
		char szTitle[MAX_PATH] = "";
		if( g_EventMgr.GetValue( EVT_PROPOSAL, EA_MAX_CHAR_USE_SEC ) >= 3600 )
			SafeSprintf( szTitle, sizeof( szTitle ), STR(1), g_EventMgr.GetValue( EVT_PROPOSAL, EA_MAX_CHAR_USE_SEC ) / 3600 );
		else
			SafeSprintf( szTitle, sizeof( szTitle ), STR(2), g_EventMgr.GetValue( EVT_PROPOSAL, EA_MAX_CHAR_USE_SEC ) / 60 );
		pFirsHireBtn->SetTitleText( szTitle );
	}
}

void EventWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	FirstHireWnd::iwm_command( pWnd, cmd, param );

	DWORD dwID = pWnd->GetID();

	switch( dwID )
	{
	case ID_GET_CHAR_BTN:
		if( cmd == IOBN_BTNUP)
		{	
			ProposalEventUserNode *pNode = dynamic_cast<ProposalEventUserNode*> ( g_MyInfo.GetEventUserMgr().GetEventUserNode( EVT_PROPOSAL ) );
			if( !pNode ||
			   ( pNode && !pNode->IsGiveChar() ) )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );	
				HideWnd();
			}
			else if( g_MyInfo.GetClassArrayExceptExercise( m_dwFirstClassList[m_iClassSelect] ) != -1 )
			{
				if( g_MyInfo.IsCharActive( g_MyInfo.GetClassArray( m_dwFirstClassList[m_iClassSelect] ) ) )
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2));
					return;
				}

				SP2Packet kPacket( CTPK_CHAR_EXTEND );
				kPacket << CHAR_EXTEND_EVENT;
				kPacket << g_MyInfo.GetCharIndex( g_MyInfo.GetClassArray( m_dwFirstClassList[m_iClassSelect] ) );
				kPacket << g_EventMgr.GetValue( EVT_PROPOSAL, EA_MAX_CHAR_USE_SEC );
				kPacket << false; // bCash
				kPacket << EVT_PROPOSAL;
				TCPNetwork::SendToServer( kPacket );
				g_App.SetMouseBusy( true );
				if( pNode )
				{
					pNode->SetValueGiveChar();
				if( pNode->IsGiveChar() )
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3));
				else
					HideWnd();
				}
			}
			else if( g_MyInfo.GetCharCount() >= g_MyInfo.GetCurMaxCharSlot() )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4));
				HideWnd();
			}
			else
			{
				SP2Packet kPacket( CTPK_CHAR_CREATE );
				kPacket << CHAR_CREATE_EVENT; 
				kPacket << m_dwFirstClassList[m_iClassSelect];
				kPacket << m_CharInfo.m_kindred;
				kPacket << m_CharInfo.m_sex;
				kPacket << g_EventMgr.GetValue( EVT_PROPOSAL, EA_MAX_CHAR_USE_SEC );
				kPacket << 0;     // iPrice
				kPacket << false; // bCash
				kPacket << CPT_TIME;

				kPacket << EVT_PROPOSAL;
				TCPNetwork::SendToServer( kPacket );
				g_App.SetMouseBusy( true );
				if( pNode )
				{
					pNode->SetValueGiveChar();
					if( !pNode->IsGiveChar() )
						HideWnd();
				}
			}
		}
		break;
	}
}

void EventWnd::iwm_show()
{
	if( g_MyInfo.GetCharCount() >= g_MyInfo.GetCurMaxCharSlot() )
		HideWnd();
	else
		FirstHireWnd::iwm_show();
}
//---------------------------------------------------------------------------------------------------------------------------------
// EVT_PLAYTIME

PlayTimeEventBtn::PlayTimeEventBtn()
{
	m_eColorType = CT_ORANGE;
}

PlayTimeEventBtn::~PlayTimeEventBtn()
{

}
//------------------------------------------------------------------------
PlayTimeEventWnd::PlayTimeEventWnd()
{
	m_bAutoShow  = false;
	m_pPlayStage = NULL;
	m_kTextInfoVector.reserve(3);

	m_pGauge               = NULL;
	m_pGaugePartitionMark  = NULL;
	m_pNumber              = NULL;
	m_pDisableNumber       = NULL;
}

PlayTimeEventWnd::~PlayTimeEventWnd()
{
	for(TextInfoVector::iterator iter = m_kTextInfoVector.begin(); iter != m_kTextInfoVector.end(); ++iter)
	{
	    TextInfo &Info = (*iter);
		Info.m_szText.Clear();
	}

	m_kTextInfoVector.clear();

	SAFEDELETE( m_pGauge );
	SAFEDELETE( m_pGaugePartitionMark );
	SAFEDELETE( m_pNumber );
	SAFEDELETE( m_pDisableNumber );
}

void PlayTimeEventWnd::OnRender()
{
	ioMovingWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	OnRenderText(iXPos, iYPos);
	OnRenderGauge(iXPos, iYPos);
	OnRenderTime(iXPos, iYPos);
	OnRenderGiftBtnText(iXPos, iYPos);
}


void PlayTimeEventWnd::iwm_show()
{
	if( g_MyInfo.IsTutorialUser() )
	{
		HideWnd();
		return;
	}

	if( !g_EventMgr.IsAlive( EVT_PLAYTIME, g_MyInfo.GetChannelingType() ) )
	{
		HideWnd();
		return;
	}

	UpdateBtns();
}

void PlayTimeEventWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_GIFT_1_BTN:
	case ID_GIFT_2_BTN:
	case ID_GIFT_3_BTN:
	case ID_GIFT_4_BTN:
	case ID_GIFT_5_BTN:
		if( cmd == IOBN_BTNUP )
		{
			CheckAndSend( dwID );
		}
		break;
	case ID_EXIT_BTN:
	case ID_OK_BTN:
		if( cmd == IOBN_BTNUP )
		{
			HideWnd();
		}
		break;
	}
}

void PlayTimeEventWnd::CheckAndSend( DWORD dwID )
{
	PlayTimeEventUserNode::GiftType eGiftType;

	if( dwID == ID_GIFT_1_BTN )
	{
		eGiftType = PlayTimeEventUserNode::GT_1_PESO;
	}
	else if( dwID == ID_GIFT_2_BTN )
	{
		eGiftType = PlayTimeEventUserNode::GT_2_PESO;
	}
	else if( dwID == ID_GIFT_3_BTN )
	{
		eGiftType = PlayTimeEventUserNode::GT_3_SOLDIER;
	}
	else if( dwID == ID_GIFT_4_BTN )
	{
		eGiftType = PlayTimeEventUserNode::GT_4_SOLDIER;
	}
	else if( dwID == ID_GIFT_5_BTN )
	{
		eGiftType = PlayTimeEventUserNode::GT_5_SOLDIER;
	}
	else
	{
		LOG.PrintTimeAndLog( 0, "%s Type Error %d" __FUNCTION__, dwID );
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return;
	}

	PlayTimeEventUserNode *pEvent = dynamic_cast<PlayTimeEventUserNode*> ( g_MyInfo.GetEventUserMgr().GetEventUserNode( EVT_PLAYTIME ) );
	if( !pEvent )
	{
		LOG.PrintTimeAndLog( 0, "%s pEvent is NULL %d" __FUNCTION__, dwID );
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return;
	}

	if( !pEvent->IsGift( eGiftType ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		return;
	}

	// Peso
	if( eGiftType == PlayTimeEventUserNode::GT_1_PESO || 
		eGiftType == PlayTimeEventUserNode::GT_2_PESO )
	{
		pEvent->SetGift( eGiftType );

		SP2Packet kPacketPeso( CTPK_EVENT_DATA_UPDATE );
		kPacketPeso << EVT_PLAYTIME;
		kPacketPeso << (int) eGiftType;
		TCPNetwork::SendToServer( kPacketPeso );
		g_App.SetMouseBusy( true );
		return; //////
	}


	// Soldier
	enum { SMILE_JOKER_CLASS_TYPE = 21, };
	if( g_MyInfo.GetClassArrayExceptExercise( SMILE_JOKER_CLASS_TYPE ) != -1 ) // 보유
	{
		if( g_MyInfo.IsCharMortmain( SMILE_JOKER_CLASS_TYPE ) )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
			return;
		}

		//충전불가 체크
		if( g_MyInfo.GetCharTotalSecond( g_MyInfo.GetClassArray( SMILE_JOKER_CLASS_TYPE ) ) + (int)pEvent->GetLimitSecond( eGiftType ) >= g_ClassPrice.GetPeriodCharGoldChargeLimit() )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
			return;
		}

		pEvent->SetGift( eGiftType );

		SP2Packet kPacketExtend( CTPK_CHAR_EXTEND );
		kPacketExtend << CHAR_EXTEND_EVENT;
		kPacketExtend << g_MyInfo.GetCharIndex( g_MyInfo.GetClassArray( SMILE_JOKER_CLASS_TYPE ) );
		kPacketExtend << (int)pEvent->GetLimitSecond( eGiftType );
		kPacketExtend << false; // bCash
		kPacketExtend << EVT_PLAYTIME;
		kPacketExtend << (int) eGiftType;
		TCPNetwork::SendToServer( kPacketExtend );
		g_App.SetMouseBusy( true );
	}
	else if( g_MyInfo.GetCharCount() >= g_MyInfo.GetCurMaxCharSlot() )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6) );
		return;
	}
	else
	{
		pEvent->SetGift( eGiftType );

		SP2Packet kPacketCreate( CTPK_CHAR_CREATE );
		kPacketCreate << CHAR_CREATE_EVENT;
		kPacketCreate << SMILE_JOKER_CLASS_TYPE;
		kPacketCreate << 0;     // kindred : 인간
		kPacketCreate << 0;     // sex : 남자
		kPacketCreate << (int)pEvent->GetLimitSecond( eGiftType );
		kPacketCreate << 0;     // iPrice
		kPacketCreate << false; // bCash
		kPacketCreate << CPT_TIME;

		kPacketCreate << EVT_PLAYTIME;
		kPacketCreate << (int) eGiftType;
		TCPNetwork::SendToServer( kPacketCreate );
		g_App.SetMouseBusy( true );
	}
}

void PlayTimeEventWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() )
	{
		SetAutoShow();
	}
	else
	{
		ioMovingWnd::OnProcess( fTimePerSec );
	}
}

void PlayTimeEventWnd::SetAutoShow()
{
	if( m_bAutoShow )
		return;
	m_bAutoShow = true;
	
	if( !m_pPlayStage )
	{
		LOG.PrintTimeAndLog( 0, "%s PlayStage is NULL." , __FUNCTION__ );
		return;
	}

	if( m_pPlayStage->GetModeType() != MT_MYROOM )
		return;

	ShowWnd();
}

void PlayTimeEventWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	enum { MAX_CHECK = 30, };

	for (int i = 0; i < MAX_CHECK ; i++)
	{
		char szName[MAX_PATH]="";
		StringCbPrintf_e( szName, sizeof( szName ), "EventText%d", i+1);
		TextInfo kInfo;
		kInfo.m_szText = xElement.GetStringAttribute( szName );
		if( kInfo.m_szText.IsEmpty() )
			return;

		StringCbPrintf_e( szName, sizeof( szName ), "EventColor%d", i+1);
		ioHashString szColor = xElement.GetStringAttribute( szName );
		kInfo.m_dwColor = ioStringConverter::ParseColor( szColor );

		StringCbPrintf_e( szName, sizeof( szName ), "ChangeLine%d", i+1 );
		kInfo.m_bChangeLine = xElement.GetBoolAttribute( szName );

		StringCbPrintf_e( szName, sizeof( szName ), "XOffset%d", i+1 );
		kInfo.m_iXOffset = xElement.GetIntAttribute( szName );

		m_kTextInfoVector.push_back( kInfo );
	}
}

void PlayTimeEventWnd::OnRenderText( int iXPos, int iYPos )
{
	enum
	{
		TEXT_X_OFFSET = 17,
		TEXT_Y_OFFSET = 181,
		TEXT_Y_GAP    = 20,
	};
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetAlignType( TAT_LEFT );
	int iLineCnt = 0;
	for(TextInfoVector::iterator iter = m_kTextInfoVector.begin(); iter != m_kTextInfoVector.end(); ++iter)
	{
		TextInfo &rkInfo = (*iter);

		g_FontMgr.SetTextColor( rkInfo.m_dwColor );
		g_FontMgr.PrintText( iXPos + TEXT_X_OFFSET + rkInfo.m_iXOffset, iYPos + TEXT_Y_OFFSET + ( TEXT_Y_GAP * iLineCnt ), FONT_SIZE_15, rkInfo.m_szText.c_str() );	
		if( rkInfo.m_bChangeLine )
			iLineCnt++;
	}
}

void PlayTimeEventWnd::OnRenderTime( int iXPos, int iYPos )
{
	enum
	{
		TIME_X_OFFSET = 298,
		TIME_Y_OFFSET = 271,
	};

	PlayTimeEventUserNode *pEvent = dynamic_cast<PlayTimeEventUserNode*> ( g_MyInfo.GetEventUserMgr().GetEventUserNode( EVT_PLAYTIME ) );
	if( !pEvent)
		return; 

	int iHour   = pEvent->GetValue( VA_PLAYTIME_PLAY_SEC ) / 3600;
	int iMinute = ( pEvent->GetValue( VA_PLAYTIME_PLAY_SEC ) % 3600 ) / 60;

	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetTextColor( 0, 150, 255 ); // 파랑
	g_FontMgr.PrintText( iXPos + TIME_X_OFFSET, iYPos + TIME_Y_OFFSET, FONT_SIZE_15, STR(1), iHour, iMinute );
}

void PlayTimeEventWnd::OnRenderGiftBtnText( int iXPos, int iYPos )
{
	enum 
	{
		GIFT_TEXT_X_OFFSET = 174,
		GIFT_TEXT_Y_OFFSET = 306,
		GIFT_TEXT_Y_GAP    = 29,
	};
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetAlignType( TAT_LEFT );
	for (int i = 0; i < MAX_GIFT_BTN ; i++)
	{
		PlayTimeEventBtn *pBtn = dynamic_cast<PlayTimeEventBtn*> ( FindChildWnd( ID_GIFT_1_BTN + i ) );
		if( !pBtn )
		{
			continue;
		}

		DWORD dwColor = 0;
		if( pBtn->GetColorType() == PlayTimeEventBtn::CT_ORANGE )
		{
			dwColor = 0xFFFF8200;
		}
		else if( pBtn->GetColorType() == PlayTimeEventBtn::CT_YELLOW )
		{
			dwColor = 0xFFFFB400;
		}
		else if( pBtn->GetColorType() == PlayTimeEventBtn::CT_GRAY )
		{
			dwColor = 0xFF323232;
		}

		char szText[MAX_PATH]="";
		if( ID_GIFT_1_BTN == ( ID_GIFT_1_BTN + i ) )
		{
			SafeSprintf( szText, sizeof(szText), STR(1) );
		}
		else if( ID_GIFT_2_BTN == ( ID_GIFT_1_BTN + i ) )
		{
			SafeSprintf( szText, sizeof(szText), STR(2) );
		}
		else if( ID_GIFT_3_BTN == ( ID_GIFT_1_BTN + i ) )
		{
			SafeSprintf( szText, sizeof(szText), STR(3) );
		}
		else if( ID_GIFT_4_BTN == ( ID_GIFT_1_BTN + i ) )
		{
			SafeSprintf( szText, sizeof(szText), STR(4) );
		}
		else if( ID_GIFT_5_BTN == ( ID_GIFT_1_BTN + i ) )
		{
			SafeSprintf( szText, sizeof(szText), STR(5) );
		}

		g_FontMgr.SetTextColor( dwColor );
		g_FontMgr.PrintText( iXPos + GIFT_TEXT_X_OFFSET, iYPos + GIFT_TEXT_Y_OFFSET + ( GIFT_TEXT_Y_GAP * i ), FONT_SIZE_15, szText  );
	}
}

void PlayTimeEventWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "Gauge" )
	{
		SAFEDELETE( m_pGauge );
		m_pGauge = pImage;
	}
	else if( szType == "GaugePartitionMark" )
	{
		SAFEDELETE( m_pGaugePartitionMark );
		m_pGaugePartitionMark = pImage;
	}
	else if( szType == "Number" )
	{
		SAFEDELETE( m_pNumber );
		m_pNumber = pImage;
	}
	else if( szType == "DisableNumber" )
	{
		SAFEDELETE( m_pDisableNumber );
		m_pDisableNumber = pImage;
	}
	else
		ioMovingWnd::AddRenderImage( szType, pImage );
}

void PlayTimeEventWnd::OnRenderGauge( int iXPos, int iYPos )
{
	if( !m_pGauge              ||
		!m_pGaugePartitionMark ||
		!m_pNumber             ||
		!m_pDisableNumber )
	{
		return;
	}


	// gauge
	enum 
	{
		GAUGE_20PER_MINS  = 60,
		GAUGE_40PER_MINS  = 300,
		GAUGE_60PER_MINS  = 600,
		GAUGE_80PER_MINS  = 1800,
		GAUGE_100PER_MINS = 6000,

		MAX_CHECK = 5,

		GAUGE_X_OFFSET    = 19,
		GAUGE_Y_OFFSET    = 262,
		GAUGE_FULL_WIDTH  = 555,
	};

	PlayTimeEventUserNode *pEvent = dynamic_cast<PlayTimeEventUserNode*> ( g_MyInfo.GetEventUserMgr().GetEventUserNode( EVT_PLAYTIME ) );
	if( !pEvent)
		return; 

	int iTotalPlayMins = pEvent->GetValue( VA_PLAYTIME_PLAY_SEC ) / 60;

	int   iMinsArray[MAX_CHECK] = { GAUGE_20PER_MINS, GAUGE_40PER_MINS, GAUGE_60PER_MINS, GAUGE_80PER_MINS, GAUGE_100PER_MINS };
	float fDefaultPerArray[MAX_CHECK] = { 0.0f, 0.2f, 0.4f, 0.6f, FLOAT08 };

	double smallPer = 0.0f;
	double Per = 0.0f;

	for (int i = 0; i < MAX_CHECK ; i++)
	{
		if( iTotalPlayMins <= iMinsArray[i] )
		{
			Per = 0.2f; // 한구역당 최고값은 20%
			if( i == 0 )
			{
				smallPer = ( (double) iTotalPlayMins/ (double) iMinsArray[i] );
				Per = Per * smallPer;
			}
			else
			{
				smallPer =  ( (double) ( iTotalPlayMins-iMinsArray[i-1] )/ (double)( iMinsArray[i]-iMinsArray[i-1] ) );
				Per = Per * smallPer;
				Per += fDefaultPerArray[i]; // 기본적으로 가지고 있는 % 더한다. 현재 40%라면 이미 20% 채워진 상태이므로 20% 더한다.
			}
			break;
		}
	}

	if(Per > FLOAT1) 
		Per = FLOAT1;
	else if( Per < 0.0f)
		Per = 0.0f;

	m_pGauge->SetSize( ((double)GAUGE_FULL_WIDTH*Per), m_pGauge->GetHeight() );
	m_pGauge->SetHorzFormatting( ioUIRenderImage::HF_STRETCH );
	m_pGauge->Render( iXPos + GAUGE_X_OFFSET, iYPos + GAUGE_Y_OFFSET );


	// number , mark
	enum
	{
		NUMBER_1_X_OFFSET   = 18,
		NUMBER_ALL_Y_OFFSET = 254,

		MARK_X_OFFSET       = 128,
		MARK_Y_OFFSET       = 263,
		MARK_X_GAP          = 111,
	};

	// 0
	m_pNumber->RenderNum( iXPos + NUMBER_1_X_OFFSET, iYPos + NUMBER_ALL_Y_OFFSET, 0 );

	// 1, 5, 10, 30, 100
	int    iXOffsetArray[MAX_GIFT_BTN]={ 126, 237, 344, 455, 553 };
	float  fGapArray[MAX_GIFT_BTN]={ 0.0f, 0.0f, -FLOAT1, -FLOAT1, -FLOAT1 };
	int    iNumberArray[MAX_GIFT_BTN]={ 1, 5, 10, 30, 100 };

	for (int i = 0; i < MAX_GIFT_BTN ; i++)
	{
		PlayTimeEventBtn *pBtn = dynamic_cast<PlayTimeEventBtn*> ( FindChildWnd( ID_GIFT_1_BTN + i ) );
		if( !pBtn )
		{
			m_pDisableNumber->RenderNum( iXPos + iXOffsetArray[i], iYPos + NUMBER_ALL_Y_OFFSET, iNumberArray[i], fGapArray[i] );
			continue;
		}

		if( pBtn->GetColorType() == PlayTimeEventBtn::CT_ORANGE )
		{
			m_pDisableNumber->RenderNum( iXPos + iXOffsetArray[i], iYPos + NUMBER_ALL_Y_OFFSET, iNumberArray[i], fGapArray[i] );
		}
		else
		{
			m_pNumber->RenderNum( iXPos + iXOffsetArray[i], iYPos + NUMBER_ALL_Y_OFFSET, iNumberArray[i], fGapArray[i] );
		}

		if( i != ( MAX_GIFT_BTN-1 )  )
			m_pGaugePartitionMark->Render( iXPos + MARK_X_OFFSET + ( MARK_X_GAP * i ), iYPos + MARK_Y_OFFSET  );
	}
}

void PlayTimeEventWnd::UpdateBtns()
{
	PlayTimeEventUserNode *pEvent = dynamic_cast<PlayTimeEventUserNode*> ( g_MyInfo.GetEventUserMgr().GetEventUserNode( EVT_PLAYTIME ) );
	if( !pEvent )
	{
		LOG.PrintTimeAndLog( 0, "%s pEvent is NULL.", __FUNCTION__ );
		return;
	}

	for (int i = 0; i < MAX_GIFT_BTN ; i++)
	{
		PlayTimeEventBtn *pBtn = dynamic_cast<PlayTimeEventBtn*> ( FindChildWnd( ID_GIFT_1_BTN + i ) );
		if( !pBtn )
		{
			LOG.PrintTimeAndLog( 0, "%s pBtn is NULL.", __FUNCTION__ );
			continue;
		}

		if( pEvent->IsGiftData( (PlayTimeEventUserNode::GiftType) ( PlayTimeEventUserNode::GT_1_PESO + i ), PlayTimeEventUserNode::GS_NONE ) )
		{
			pBtn->SetActive();
			pBtn->SetTitleColor( 0xFFFF8200, 0xFF000000 ); // 주황
			pBtn->SetColorType( PlayTimeEventBtn::CT_ORANGE );
			pBtn->SetTwinkle( false );
		}
		else if( pEvent->IsGiftData( (PlayTimeEventUserNode::GiftType) ( PlayTimeEventUserNode::GT_1_PESO + i ), PlayTimeEventUserNode::GS_NOTICE ) )
		{
			pBtn->SetActive();
			pBtn->SetTitleColor( 0xFFFFB400, 0xFF000000 ); // 노랑
			pBtn->SetColorType( PlayTimeEventBtn::CT_YELLOW );
			pBtn->SetTwinkle( true );
		}
		else if( pEvent->IsGiftData( (PlayTimeEventUserNode::GiftType) ( PlayTimeEventUserNode::GT_1_PESO + i ), PlayTimeEventUserNode::GS_USED ) )
		{
			pBtn->SetInActive();
			pBtn->SetTitleColor( 0xFF323232, 0xFF000000 ); // 회색
			pBtn->SetColorType( PlayTimeEventBtn::CT_GRAY );
			pBtn->SetTwinkle( false );
		}
	}
}

//------------------------------------------------------------------------------------------------------------------------------

MgameOpenEventWnd::MgameOpenEventWnd( void )
{
	m_bOneShow   = false;
	m_iBonusPeso = 0;
}

MgameOpenEventWnd::~MgameOpenEventWnd( void )
{

}

void MgameOpenEventWnd::iwm_show()
{
	if( m_bOneShow )
	{
		HideWnd();
		return;
	}
	m_bOneShow = true;

	if( !g_MyInfo.IsGradeLevelUP() )	
	{
		HideWnd();
		return;
	}

	GradeUpEventUserNode *pEventUserNode = dynamic_cast<GradeUpEventUserNode*> ( g_MyInfo.GetEventUserMgr().GetEventUserNode( EVT_GRADEUP ) );
	if( !pEventUserNode )
	{
		HideWnd();
		return;
	}

	if( !pEventUserNode->IsGift() )
	{
		HideWnd();
		return;
	}

 	m_iBonusPeso = pEventUserNode->GetBounsPeso();


	char szGradeName[MAX_PATH]="";
	g_LevelMgr.GetGradeName( g_MyInfo.GetGradeLevel(), szGradeName, sizeof( szGradeName ), false, false );

	// [상사1] 에서 1을 삭제 해서 [상사]를 만든다.
	size_t iLength = 0;
	StringCbLength( szGradeName, sizeof( szGradeName ), &iLength );
	if( COMPARE( iLength-1, 0, MAX_PATH ) )
	{
#if defined( SRC_OVERSEAS ) && !defined( SRC_ID )

#if defined( MULTI_BYTE_CHECK )
		if( !IsDBCSLeadByteEx( COUNTRY_CODE_PAGE, szGradeName[iLength-1] ) )
#else
		if( true || !IsDBCSLeadByteEx( COUNTRY_CODE_PAGE, szGradeName[iLength-1] ) ) // 한글이 아니고 숫자라면
#endif

#else
		if( !IsDBCSLeadByte( szGradeName[iLength-1] ) ) // 한글이 아니고 숫자라면
#endif
			szGradeName[iLength-1] = NULL; 
	}

	char szTitleText[MAX_PATH]="";
	SafeSprintf( szTitleText, sizeof( szTitleText ), STR(1), szGradeName );

	ioUITitle *pTitle = GetTitle();
	if( pTitle )
		pTitle->SetText( szTitleText, "" );
}

void MgameOpenEventWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();

	switch( dwID )
	{
	case ID_EXIT:
	case ID_OK:
		if( cmd == IOBN_BTNUP )
			HideWnd();
		break;
	}
}

void MgameOpenEventWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	enum 
	{
		X_OFFSET = 105,
		Y_OFFSET = 45,

		LINE_Y_GAP      = 20,
		BIG_LINE_Y_GAP  = 30,

		MAX_TEXT        = 2,
	};

	char szTextArray[MAX_TEXT][MAX_PATH]={ "", "" };
	
	if( g_MyInfo.GetGradeLevel() == g_EventMgr.GetValue( EVT_GRADEUP, EA_GRADEUP_1_GRADE ) )
	{
		SafeSprintf( szTextArray[0], sizeof( szTextArray[0] ), STR(1) );
		SafeSprintf( szTextArray[1], sizeof( szTextArray[1] ), STR(2) );
	}
	else if( g_MyInfo.GetGradeLevel() == g_EventMgr.GetValue( EVT_GRADEUP, EA_GRADEUP_2_GRADE ))
	{
		SafeSprintf( szTextArray[0], sizeof( szTextArray[0] ), STR(3) );
		SafeSprintf( szTextArray[1], sizeof( szTextArray[1] ), STR(4) );
	}
	else if( g_MyInfo.GetGradeLevel() == g_EventMgr.GetValue( EVT_GRADEUP, EA_GRADEUP_3_GRADE ))
	{
		SafeSprintf( szTextArray[0], sizeof( szTextArray[0] ), STR(5)  );
		SafeSprintf( szTextArray[1], sizeof( szTextArray[1] ), STR(6) );
	}
	else if( g_MyInfo.GetGradeLevel() == g_EventMgr.GetValue( EVT_GRADEUP, EA_GRADEUP_4_GRADE ))
	{
		SafeSprintf( szTextArray[0], sizeof( szTextArray[0] ), STR(7) );
		SafeSprintf( szTextArray[1], sizeof( szTextArray[1] ), STR(8) );
	}

	char szNumComma[MAX_PATH]="";
	Help::ConvertNumToStrComma( m_iBonusPeso, szNumComma, sizeof( szNumComma ) );

	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetAlignType( TAT_LEFT );

	iYPos = iYPos + Y_OFFSET;
	g_FontMgr.SetTextColor( 225, 180, 0 ); // 노란
	g_FontMgr.PrintText( iXPos + X_OFFSET, iYPos, FONT_SIZE_15, STR(9) );
	iYPos = iYPos + LINE_Y_GAP;
	g_FontMgr.PrintText( iXPos + X_OFFSET, iYPos, FONT_SIZE_15, szTextArray[0] );
	iYPos = iYPos + BIG_LINE_Y_GAP;

	g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY ); // 주황
	g_FontMgr.PrintText( iXPos + X_OFFSET, iYPos, FONT_SIZE_15, STR(10), szNumComma );
	iYPos += LINE_Y_GAP;
	g_FontMgr.PrintText( iXPos + X_OFFSET, iYPos, FONT_SIZE_15, szTextArray[1] );
}

bool MgameOpenEventWnd::iwm_esc()
{
	ioWnd *pBtn = FindChildWnd( ID_OK );
	if( !pBtn ) return false;
	if( !pBtn->IsShow() ) return false;

	iwm_command( pBtn, IOBN_BTNUP, 0 );
	return true;
}
//-------------------------------------------------------------------------------------------------------------------------------
//EVT_PLAYTIME_PRESENT
PlayTimePresentEventWnd::PlayTimePresentEventWnd()
{
	m_pGaugeBack = NULL;
	m_pGaugeLine = NULL;
	m_pTimeText  = NULL;
	m_pTimeColon = NULL;
}

PlayTimePresentEventWnd::~PlayTimePresentEventWnd()
{
	SAFEDELETE( m_pGaugeBack );
	SAFEDELETE( m_pGaugeLine );
	SAFEDELETE( m_pTimeText );
	SAFEDELETE( m_pTimeColon );
}

void PlayTimePresentEventWnd::iwm_show()
{
	if( !g_EventMgr.IsAlive( EVT_PLAYTIME_PRESENT, g_MyInfo.GetChannelingType() ) )
	{
		// 이벤트 종료
		HideWnd();
	}
	else
	{
		PlayTimePresentEventUserNode *pEventNode = dynamic_cast<PlayTimePresentEventUserNode*>( g_MyInfo.GetEventUserMgr().GetEventUserNode( EVT_PLAYTIME_PRESENT ) );
		if( pEventNode )
		{
			if( pEventNode->GetValue( VA_PLAYTIME_PRESENT_TIME_CNT ) >= 0 )
			{
				// 시간 게이지
				HideChildWnd( ID_NORMAL_CHANCE );
			}
			else
			{
				// 찬스 사용
				ShowChildWnd( ID_NORMAL_CHANCE );
				SetChildActive( ID_NORMAL_CHANCE );
			}
		}
	}
}

void PlayTimePresentEventWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
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
			PlayTimePresentEventUserNode *pEventNode = dynamic_cast<PlayTimePresentEventUserNode*>( g_MyInfo.GetEventUserMgr().GetEventUserNode( EVT_PLAYTIME_PRESENT ) );
			if( pEventNode )
			{
				if( pEventNode->GetValue( VA_CMC_MAGIC_CODE ) == PlayTimePresentEventUserNode::CHANCE_GET_PRESENT )
				{
					pEventNode->SetValue( VA_CMC_MAGIC_CODE, 0 );

					SP2Packet kPacketPeso( CTPK_EVENT_DATA_UPDATE );
					kPacketPeso << EVT_PLAYTIME_PRESENT;
					TCPNetwork::SendToServer( kPacketPeso );
				}
			}
			HideWnd();
		}
		break;
	}
}

void PlayTimePresentEventWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
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

void PlayTimePresentEventWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;

	if( !g_EventMgr.IsAlive( EVT_PLAYTIME_PRESENT, g_MyInfo.GetChannelingType() ) )
	{
		// 이벤트 종료
		HideWnd();
	}
	else
	{
		ioMovingWnd::OnProcess( fTimePerSec );
	}
}

void PlayTimePresentEventWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	PlayTimePresentEventUserNode *pEventNode = dynamic_cast<PlayTimePresentEventUserNode*>( g_MyInfo.GetEventUserMgr().GetEventUserNode( EVT_PLAYTIME_PRESENT ) );
	if( pEventNode )
	{
		g_FontMgr.SetTextStyle( TS_NORMAL );
		g_FontMgr.SetAlignType( TAT_CENTER );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );

		if( pEventNode->GetValue( VA_PLAYTIME_PRESENT_TIME_CNT ) >= 0 )
		{
			int iTotalMS = max( 1, g_EventMgr.GetValue( EVT_PLAYTIME_PRESENT, EA_PLAYTIME_PRESENT_TIME ) );
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

//----------------------------------------------------------------------------------------------------------------------------------
// EVT_EXERCISESOLDIER
ExerciseSoldierEventWnd::ExerciseSoldierEventWnd()
{
	m_iCurPos			 = 0;
	m_iMaxPage			 = 0;
	m_iCurBtnSize        = 0;
	m_pPlayStage         = NULL;
	m_vItemInfoList.reserve(10);
	m_vExtraClassVec.clear();
	m_pLeftFrm           = NULL;
}

ExerciseSoldierEventWnd::~ExerciseSoldierEventWnd()
{
	m_vItemInfoList.clear();
	m_vExtraClassVec.clear();
	SAFEDELETE( m_pLeftFrm );
}

void ExerciseSoldierEventWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_EXERCISE_SOLDIER_1_BTN:
	case ID_EXERCISE_SOLDIER_2_BTN:
	case ID_EXERCISE_SOLDIER_3_BTN:
	case ID_EXERCISE_SOLDIER_4_BTN:
	case ID_EXERCISE_SOLDIER_5_BTN:
	case ID_EXERCISE_SOLDIER_6_BTN:
	case ID_EXERCISE_SOLDIER_7_BTN:
	case ID_EXERCISE_SOLDIER_8_BTN:
	case ID_EXERCISE_SOLDIER_9_BTN:
	case ID_EXERCISE_SOLDIER_10_BTN:
	case ID_EXERCISE_SOLDIER_11_BTN:
	case ID_EXERCISE_SOLDIER_12_BTN:
	case ID_EXERCISE_SOLDIER_13_BTN:
	case ID_EXERCISE_SOLDIER_14_BTN:
	case ID_EXERCISE_SOLDIER_15_BTN:
	case ID_EXERCISE_SOLDIER_16_BTN:
	case ID_EXERCISE_SOLDIER_17_BTN:
	case ID_EXERCISE_SOLDIER_18_BTN:
	case ID_EXERCISE_SOLDIER_19_BTN:
	case ID_EXERCISE_SOLDIER_20_BTN:
	case ID_EXERCISE_SOLDIER_21_BTN:
	case ID_EXERCISE_SOLDIER_22_BTN:
	case ID_EXERCISE_SOLDIER_23_BTN:
	case ID_EXERCISE_SOLDIER_24_BTN:
	case ID_EXERCISE_SOLDIER_25_BTN:
		if( cmd == IOBN_BTNUP )
		{
			bool bWait = true;
			if( !IsCanExerciseChar( bWait ) )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
				if( bWait )
					g_GUIMgr.HideWnd( SOLDIER_SELECT_WND );
				return;
			}

			SoldierExerciseBtn *pItemBtn = dynamic_cast<SoldierExerciseBtn *>( FindChildWnd( dwID ) );
			if( !pItemBtn ) 
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
				return;
			}

			// 용병 교체했으므로 리턴
			if( ChangeBoughtChar( g_MyInfo.GetClassArray( pItemBtn->GetClassType() ), bWait ) )
			{
				return;
			}

			int iCurMaxSlot = g_MyInfo.GetCurMaxCharSlot();
			bool bCharDelete = g_MyInfo.IsCharInActive( pItemBtn->GetClassType() );
			if( g_MyInfo.GetCharCount() >= iCurMaxSlot && g_MyInfo.GetExerciseCharCount() == 0 )
			{
				if( !bCharDelete )
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
					return;
				}
			}

			bool bOneChar = true;
			if( m_pPlayStage && !m_pPlayStage->IsOneMyChar() )
				bOneChar = false;

			SP2Packet kPacket( CTPK_EXERCISE_EVENT_CHAR_CREATE );
			kPacket << EXERCISE_EVENT;
			kPacket << pItemBtn->GetClassType();
			kPacket << bWait;
			kPacket << bOneChar;
			kPacket << bCharDelete;
			TCPNetwork::SendToServer( kPacket );
			g_App.SetMouseBusy( true );

			if( bWait )
			{
				SetOwnerChangeWaitState();
				g_GUIMgr.HideWnd( SOLDIER_SELECT_WND );
			}
			else
			{   
				// 모드 시작시 선택
				if( m_pPlayStage)
				{
					ioPlayMode *pPlayMode = m_pPlayStage->GetPlayMode();
					if( pPlayMode )
						pPlayMode->EndChangeCharacter();
				}
			}
		}
		break;
	}
}

void ExerciseSoldierEventWnd::iwm_create()
{
}

void ExerciseSoldierEventWnd::iwm_show()
{
	if( !g_EventMgr.IsAlive( EVT_EXERCISESOLDIER, g_MyInfo.GetChannelingType() ) )
	{
		HideWnd();
		return;
	}


	if( g_MyInfo.IsTutorialUser() )
	{
		HideWnd();
		return;
	}

	if( g_GUIMgr.IsShow( PRACTICE_HELP_WND ) )
	{
		HideWnd();
		return;
	}

	SetWndBtnSize();
}

void ExerciseSoldierEventWnd::UpdateChildPos()
{
	if( !IsShow() )
		return;

	for (int i = ID_EXERCISE_SOLDIER_1_BTN; i < ID_EXERCISE_SOLDIER_25_BTN + 1; i++)
	{
		ioWnd *pBtn = FindChildWnd( i );
		if( !pBtn ) continue;
		pBtn->HideWnd();
	}

	int iStart = m_iCurPos * m_iCurBtnSize;
	int iEnd = iStart + m_iCurBtnSize;
	int iWndID = ID_EXERCISE_SOLDIER_1_BTN;
	SortItemInfo();
	for (int i = iStart; i < iEnd ; i++)
	{
		_UpdateChildPos( i , iWndID);
		iWndID++;
	}
}

void ExerciseSoldierEventWnd::_UpdateChildPos( int a_iArray ,int iWndID )
{
	if( !COMPARE( a_iArray, 0, (int) m_vItemInfoList.size()) )
	{
		SetBlankIcon( iWndID );
		return;
	}
	int iSetIdx = GetSetIdx( a_iArray );
	const ioSetItemInfo *pInfo = g_SetItemInfoMgr.GetSetInfoByIdx( iSetIdx );
	if( !pInfo ) return;

	SoldierExerciseBtn *pItemBtn = dynamic_cast<SoldierExerciseBtn *>( FindChildWnd( iWndID ) );
	if( !pItemBtn ) return;

	const ioItem *pItem = g_ItemMaker.GetItemConst( pInfo->GetSetItemCode(0), __FUNCTION__ );
	if( !pItem ) return;

	char szBuf[MAX_PATH] = "";
	DWORD dwSoldierType = pItem->GetItemCode() % DEFAULT_BASIC_ITEM_CODE;
	ioUIRenderImage *pImg = g_MyInfo.GetMySoldierIcon( dwSoldierType );
	if( !pImg )
		return;

	pItemBtn->SetMarkType( pInfo->GetShopMarkType() );
	pItemBtn->SetSoldierIcon( pImg, EVENT_ICON_SCALE, false );
	int iClassType = pInfo->GetSetCode() - SET_ITEM_CODE;
	SoldierExerciseBtn::SoldierBoughtType eBoughtType = SoldierExerciseBtn::SBT_NONE;
	int iArray = g_MyInfo.GetClassArray( iClassType );
	if( iArray != -1 )
	{
		if( g_MyInfo.IsCharExerciseStyle( iArray, EXERCISE_GENERAL ) )
			eBoughtType = SoldierExerciseBtn::SBT_EXERCISE_GENERAL;
		else if( g_MyInfo.IsCharExerciseStyle( iArray, EXERCISE_PCROOM ) )
			eBoughtType = SoldierExerciseBtn::SBT_EXERCISE_PCROOM;
		else if( g_MyInfo.IsCharExerciseStyle( iArray, EXERCISE_EVENT ) )
			eBoughtType = SoldierExerciseBtn::SBT_EXERCISE_EVENT;
		else if( g_MyInfo.IsCharActive( iArray ) )
			eBoughtType = SoldierExerciseBtn::SBT_BOUGHT;
	}
	pItemBtn->SetBoughtType( eBoughtType );
	pItemBtn->SetClassType( iClassType );
	pItemBtn->SetActive();
	pItemBtn->ShowWnd();	

	int iType  = ioSetItemInfo::NLT_NONE;
	int iLevel = -1;

	if( iType == ioSetItemInfo::NLT_NONE )
		pItemBtn->SetNeedTypeLv( (ioSetItemInfo::NeedLevelType)iType, -1 ); 
	else
		pItemBtn->SetNeedTypeLv( (ioSetItemInfo::NeedLevelType)iType, iLevel );
}

void ExerciseSoldierEventWnd::SetPlayStage( ioPlayStage *pPlayStage )
{
	m_pPlayStage = pPlayStage;
}

bool ExerciseSoldierEventWnd::IsCanExerciseChar( bool bSelectWait )
{
	if( !m_pPlayStage ) return true;
	ioBaseChar *pOwner = m_pPlayStage->GetOwnerChar();
	if( !pOwner ) return true;
	if( g_MyInfo.GetCharCount() == 0 ) return true;
	if( bSelectWait && !pOwner->IsCanChangeCharState( true, false, false ) ) return false;

	if( pOwner->GetExperienceMode() == EMS_CHANGE_EXPERIENCE )
		return false;

	return true;
}

void ExerciseSoldierEventWnd::SetBoughtType( SoldierExerciseBtn::SoldierBoughtType eBoughtType, int iCharArray )
{
	int iClassType = g_MyInfo.GetClassType( iCharArray );

	for (int i = ID_EXERCISE_SOLDIER_1_BTN; i <  ID_EXERCISE_SOLDIER_25_BTN +1; i++)
	{
		SoldierExerciseBtn *pItemBtn = dynamic_cast<SoldierExerciseBtn *>( FindChildWnd( i ) );
		if( !pItemBtn )	 continue;

		if( iClassType == pItemBtn->GetClassType() )
		{
			pItemBtn->SetBoughtType( eBoughtType );
			break; ///////
		}		
	}
}

void ExerciseSoldierEventWnd::SetOwnerChangeWaitState()
{
	if( !m_pPlayStage ) return;

	if( m_pPlayStage->GetModeType() == MT_MYROOM )
		return;

	ioBaseChar *pOwner = m_pPlayStage->GetOwnerChar();
	if( !pOwner ) return;

	if( m_pPlayStage->IsOneMyChar() )
		pOwner->SetChangeWaitState( pOwner->GetSelectCharArray(), false );
}

void ExerciseSoldierEventWnd::SendChangeWait( int iArray )
{
	if( !m_pPlayStage ) return;

	ioBaseChar *pOwner = m_pPlayStage->GetOwnerChar();
	if( !pOwner ) return;

	if( m_pPlayStage->GetModeType() != MT_MYROOM )
	{
		pOwner->SetChangeWaitState( iArray );
	}
	else
	{
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
			ToMechanicsItem(pItem)			||
			ToMichaelItem(pItem)			||
			ToPhantomItem(pItem)			||
			ToLuciferItem(pItem)			||
			ToChargeComboBowItem(pItem)		||
			ToWestGunExtendItem(pItem)		||
			ToDestroyerItem(pItem)			||
			ToPeterpanItem(pItem)			||
			ToCyvogueItem(pItem)			||
			ToDarkSeasItem(pItem)			||
			ToWindLordItem(pItem)			||
			ToProject_K_Item(pItem)			||
			ToDevilItem(pItem)				||
			ToFlashItem(pItem)				||
			ToPsychoMetryItem(pItem)		||
			ToSuddenAttackItem(pItem)		||
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
			ToJeonWooChiWeaponItem(pItem)	||
			ToChainMagicianItem( pItem )	||
			ToStriderItem( pItem )			||
			ToCuchulainWeaponItem( pItem )  ||
			ToOniMushaItem( pItem ) 		||
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
	}
}

void ExerciseSoldierEventWnd::SetBlankIcon( int a_iWndID )
{
	SoldierExerciseBtn *pItemBtn = dynamic_cast<SoldierExerciseBtn *>( FindChildWnd( a_iWndID ) );
	if( !pItemBtn ) return;

	ioUIRenderImage *pImg = g_UIImageSetMgr.CreateImageByFullName( "UIIconPack3#empty_large" );
	if( !pImg )	return;

	pItemBtn->SetSoldierIcon( pImg, EVENT_ICON_SCALE, false );
	pItemBtn->SetBoughtType( SoldierExerciseBtn::SBT_NONE );
	pItemBtn->ShowWnd();		
	pItemBtn->SetInActive();
	pItemBtn->SetNeedTypeLv( ioSetItemInfo::NLT_NONE, -1 );
}


void ExerciseSoldierEventWnd::SetMaxPage()
{
	enum { MINUS_VALUE = 138, DIVISION_VALUE = 60 };
	m_iCurBtnSize = ( Setting::Width() - MINUS_VALUE ) / DIVISION_VALUE;
	if( m_iCurBtnSize > MAX_EXERCISE_SOLDIER_BTN )
		m_iCurBtnSize = MAX_EXERCISE_SOLDIER_BTN;

	int iMaxPage = 0;
	int iUseCharCnt = m_vItemInfoList.size();
	if( iUseCharCnt <= 0 )
	{
		return;
	}
	iMaxPage = (  iUseCharCnt / m_iCurBtnSize );
	if( ( iUseCharCnt % m_iCurBtnSize ) != 0 )
		iMaxPage++;

	m_iCurPos = 0;
	ioScroll *pScroll = (ioScroll*)FindChildWnd( ID_VERT_SCROLL );
	if( pScroll )
	{
		pScroll->SetScrollRange( 0, iMaxPage );
		pScroll->SetScrollPos( m_iCurPos );
		pScroll->ShowWnd();
	}
}

bool ExerciseSoldierEventWnd::ChangeBoughtChar( int iArray, bool bWait )
{
	if( iArray == -1 ) return false;

	if( g_MyInfo.IsCharActive( iArray ) )
	{	
		if( bWait )
		{
			SendChangeWait( iArray );
			g_GUIMgr.HideWnd( SOLDIER_SELECT_WND );
		}
		else
		{
			// 모드 시작시 선택
			SP2Packet kPacket( CTPK_CHANGE_CHAR );
			kPacket << iArray;
			kPacket << false;
			kPacket << MAX_INT_VALUE;
			TCPNetwork::SendToServer( kPacket );

			if( m_pPlayStage)
			{
				ioPlayMode *pPlayMode = m_pPlayStage->GetPlayMode();
				if( pPlayMode )
					pPlayMode->EndChangeCharacter();
			}
		}
		return true;
	}
	return false;
}

void ExerciseSoldierEventWnd::SortItemInfo()
{
	m_vItemInfoList.clear();
	int iMax = g_ClassPrice.MaxClassPrice();
	for (int i = 0; i <  iMax; i++)
	{
		if( !g_ClassPrice.IsActiveClass( i ) )
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

		if( g_ClassPrice.IsCashOnlyByArray( i ) )
			continue;

		int iSetIdx = g_ClassPrice.ArrayToSetItemIdx( i );
		const ioSetItemInfo *pInfo = g_SetItemInfoMgr.GetSetInfoByIdx( iSetIdx );
		if( !pInfo ) continue;

		ShopItemInfo kInfo;
		kInfo.m_iSetItemIdx = iSetIdx;
		kInfo.m_bCanBuy     = g_MyInfo.IsCanBuyItem( pInfo );
		kInfo.m_iPrice      = g_ClassPrice.GetClassBuyPeso( pInfo->GetSetCode() - SET_ITEM_CODE, g_ClassPrice.GetDefaultLimit() );
		kInfo.m_iPriorityOrder = pInfo->GetShopOrder();
		kInfo.m_iShopMarkType  = pInfo->GetShopMarkType();

		int iClassType = pInfo->GetSetCode() - SET_ITEM_CODE;
		int iArray = g_MyInfo.GetClassArray( iClassType );

		if( g_MyInfo.IsCharExerciseStyle( iArray, EXERCISE_NONE ) )
			continue;

		if( g_MyInfo.IsCharExerciseStyle( iArray, EXERCISE_PCROOM ) )
			continue;

		m_vItemInfoList.push_back( kInfo );
	}

	if( m_vItemInfoList.empty() )
		return;
	std::sort( m_vItemInfoList.begin() , m_vItemInfoList.end(), ShopItemInfoSort() );
}

int ExerciseSoldierEventWnd::GetSetIdx( int iArray )
{
	if( COMPARE( iArray, 0, (int) m_vItemInfoList.size() ) )
		return m_vItemInfoList[iArray].m_iSetItemIdx;

	return 0;
}

void ExerciseSoldierEventWnd::SetPositionToDefault( int iParentWidth, int iParentHeight, bool bChildMove, bool bCreate /*= false */ )
{
	ioWnd::SetPositionToDefault( iParentWidth, iParentHeight, bChildMove, bCreate );

	if( !bCreate )
	{
		SetWndBtnSize();
	}
}

void ExerciseSoldierEventWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() )
		return;
	
	if( !g_EventMgr.IsAlive( EVT_EXERCISESOLDIER, g_MyInfo.GetChannelingType() ) )
		HideWnd();
}

void ExerciseSoldierEventWnd::SetWndBtnSize()
{
	ioWnd::SetSize( Setting::Width(), GetHeight() );

	enum { X_OFFSET = 32, };
	ioWnd *pChild = FindChildWnd( ID_VERT_SCROLL );
	if( pChild )
		pChild->SetWndPos( Setting::Width() - X_OFFSET, pChild->GetYPos() );
	
	SortItemInfo();
	SetMaxPage();
}

void ExerciseSoldierEventWnd::ParseExtraInfo( ioXMLElement &xElement )
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

void ExerciseSoldierEventWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	if( m_pLeftFrm )
		m_pLeftFrm->Render( iXPos, iYPos, UI_RENDER_MULTIPLY );

	g_FontMgr.SetAlignType( TAT_LEFT );
	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );	

	enum 
	{
		X_OFFSET = 12,
		Y_OFFSET = 18,
		Y_LINFGAP= 18,
	};
	g_FontMgr.PrintText( iXPos + X_OFFSET, iYPos + Y_OFFSET , FONT_SIZE_13, STR(1) );
	g_FontMgr.PrintText( iXPos + X_OFFSET, iYPos + Y_OFFSET + Y_LINFGAP, FONT_SIZE_13, STR(2) );
}

void ExerciseSoldierEventWnd::iwm_wheel( int zDelta )
{
	if( !IsShow() )
		ioWnd::iwm_wheel( zDelta );
	else
	{
		if( g_App.IsMouseBusy() ) return;

		ioScroll *pScroll = (ioScroll*)FindChildWnd( ID_VERT_SCROLL );
		if( pScroll == NULL ) return;
		if( !pScroll->IsShow() ) return;

		if( zDelta == WHEEL_DELTA )
			pScroll->SetScrollPos( m_iCurPos - 1 );
		else if( zDelta == -WHEEL_DELTA )
			pScroll->SetScrollPos( m_iCurPos + 1 );
	}
}

void ExerciseSoldierEventWnd::iwm_vscroll( DWORD dwID, int curPos )
{
	ioScroll *pScroll = (ioScroll*)FindChildWnd( ID_VERT_SCROLL );
	if( pScroll == NULL ) return;
	if( !pScroll->IsShow() ) return;

	m_iCurPos = curPos;

	UpdateChildPos();
}

void ExerciseSoldierEventWnd::AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame )
{
	if( szType == "LeftFrm" )
	{
		SAFEDELETE( m_pLeftFrm );
		m_pLeftFrm = pFrame;
	}
	else
	{
		ioWnd::AddRenderFrame( szType, pFrame );
	}
}
//-------------------------------------------------------------------------------------------------------------------------------
// EVT_CONNECTION_TIME

ConnectionTimeEventBtn::ConnectionTimeEventBtn()
{
	m_pIcon      = NULL;

	m_pToolTipCen   = NULL;
	m_pToolTipLMid  = NULL;
	m_pToolTipRMid  = NULL;
	m_pToolTipLeft  = NULL;
	m_pToolTipRight = NULL;
}

ConnectionTimeEventBtn::~ConnectionTimeEventBtn()
{
	SAFEDELETE( m_pIcon );

	SAFEDELETE( m_pToolTipCen );
	SAFEDELETE( m_pToolTipLMid );
	SAFEDELETE( m_pToolTipRMid );
	SAFEDELETE( m_pToolTipLeft );
	SAFEDELETE( m_pToolTipRight );
}

void ConnectionTimeEventBtn::SetIcon( ioUIRenderImage *pIcon, float fIconScale, const ioHashString &rszName )
{
	if( !pIcon )
		return;

	SAFEDELETE( m_pIcon );
	m_pIcon = pIcon;

	if( m_pIcon )
		m_pIcon->SetScale( fIconScale );

	m_szName = rszName;
}

void ConnectionTimeEventBtn::OnDrawNormal( int iXPos, int iYPos )
{
	ioButton::OnDrawNormal( iXPos, iYPos );
	if( !m_pIcon ) return;
	m_pIcon->Render( iXPos + GetWidth() / 2, iYPos + GetHeight() / 2, UI_RENDER_NORMAL, TFO_BILINEAR );
}

void ConnectionTimeEventBtn::OnDrawOvered( int iXPos, int iYPos )
{
	ioButton::OnDrawOvered( iXPos, iYPos );
	if( !m_pIcon ) return;
	m_pIcon->Render( iXPos + GetWidth() / 2, iYPos + GetHeight() / 2, UI_RENDER_NORMAL, TFO_BILINEAR );
	m_pIcon->Render( iXPos + GetWidth() / 2, iYPos + GetHeight() / 2, UI_RENDER_SCREEN, TFO_BILINEAR );

	RenderToolTip();
}

void ConnectionTimeEventBtn::OnDrawPushed( int iXPos, int iYPos )
{
	ioButton::OnDrawPushed( iXPos, iYPos );
	if( !m_pIcon ) return;
	m_pIcon->Render( iXPos + GetWidth() / 2, iYPos + GetHeight() / 2, UI_RENDER_NORMAL, TFO_BILINEAR );
	m_pIcon->Render( iXPos + GetWidth() / 2, iYPos + GetHeight() / 2, UI_RENDER_SCREEN, TFO_BILINEAR );

	RenderToolTip();
}

void ConnectionTimeEventBtn::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "ToolTipCenter")
	{
		SAFEDELETE( m_pToolTipCen );
		m_pToolTipCen = pImage;
	}
	else if( szType == "ToolTipLMiddle" )
	{
		SAFEDELETE( m_pToolTipLMid );
		m_pToolTipLMid = pImage;
	}
	else if( szType == "ToolTipRMiddle" )
	{
		SAFEDELETE( m_pToolTipRMid );
		m_pToolTipRMid = pImage;
	}
	else if( szType == "ToolTipLeft" )
	{
		SAFEDELETE( m_pToolTipLeft );
		m_pToolTipLeft = pImage;
	}
	else if( szType == "ToolTipRight" )
	{
		SAFEDELETE( m_pToolTipRight );
		m_pToolTipRight = pImage;
	}
	else
	{
		ioButton::AddRenderImage( szType, pImage );
	}
}


void ConnectionTimeEventBtn::RenderToolTip()
{
	if( m_szName.IsEmpty() )
		return;

	int iXPos, iYPos;
	iXPos = GetDerivedPosX() + 21;
	iYPos = GetDerivedPosY() - 30;

	int iTextSize = g_FontMgr.GetTextWidth( m_szName.c_str(), TS_NORMAL, FLOAT05 );

	enum
	{
		BACK_HEIGHT = 18,
		SIDE_BLANK = 3,
	};

	if( iTextSize > 0 && m_pToolTipCen && m_pToolTipLMid && m_pToolTipRMid && m_pToolTipLeft && m_pToolTipRight )
	{
		int iLHalfPos = iTextSize / 2 + SIDE_BLANK;
		int iHalfSize = (iTextSize - m_pToolTipCen->GetWidth()) / 2 + SIDE_BLANK;

		//Left
		int iCurPos = iXPos - iLHalfPos - m_pToolTipLeft->GetWidth();
		m_pToolTipLeft->Render( iCurPos, iYPos );

		//Left Mid
		iCurPos = iXPos - iLHalfPos;

		m_pToolTipLMid->SetSize( iHalfSize, BACK_HEIGHT );
		m_pToolTipLMid->SetHorzFormatting( ioUIRenderImage::HF_STRETCH );
		m_pToolTipLMid->Render( iCurPos, iYPos );

		//Center
		iCurPos = iCurPos + iHalfSize;
		m_pToolTipCen->Render( iCurPos, iYPos );

		//Right Mid
		iCurPos = iCurPos + m_pToolTipCen->GetWidth();
		m_pToolTipRMid->SetSize( iHalfSize, BACK_HEIGHT );
		m_pToolTipRMid->SetHorzFormatting( ioUIRenderImage::HF_STRETCH );
		m_pToolTipRMid->Render( iCurPos, iYPos );

		//Right
		iCurPos = iCurPos + iHalfSize;
		m_pToolTipRight->Render( iCurPos, iYPos );
	}

	// Text
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetAlignType( TAT_CENTER );


	// 바깥쪽 글자의 1px을 반투명하게 찍기 위해서
	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );

	g_FontMgr.PrintTextAlpha(iXPos, iYPos + 3, FLOAT05, 127, m_szName.c_str() );

	g_FontMgr.SetTextStyle( TS_OUTLINE_FULL );
	g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
	g_FontMgr.PrintTextAlpha( iXPos, iYPos + 3, FLOAT05, 255, m_szName.c_str() );
}

//-----------------------------------------------
bool ConnectionTimeEventWnd::m_bProcess = false;

ConnectionTimeEventWnd::ConnectionTimeEventWnd()
{
	m_pGaugeBack = NULL;
	m_pGaugeLine = NULL;
	m_pTimeText  = NULL;
	m_pTimeColon = NULL;
	m_dwCheckChanceTime = 0;
}

ConnectionTimeEventWnd::~ConnectionTimeEventWnd()
{
	SAFEDELETE( m_pGaugeBack );
	SAFEDELETE( m_pGaugeLine );
	SAFEDELETE( m_pTimeText );
	SAFEDELETE( m_pTimeColon );
}

void ConnectionTimeEventWnd::iwm_show()
{
	if( !g_EventMgr.IsAlive( EVT_CONNECTION_TIME, g_MyInfo.GetChannelingType() ) )
	{
		// 이벤트 종료
		HideWnd();
		m_bProcess = false;
	}
	else
	{
		if( g_MyInfo.GetGradeLevel() <= g_EventMgr.GetValue( EVT_CONNECTION_TIME, EA_CT_PRESENT_LOW_USER_LEVEL ) )
		{
			ioHashString szName;
			ioEtcItem *pEtcItem = g_EtcItemMgr.FindEtcItem( ioEtcItem::EIT_ETC_GASHAPON );
			if( pEtcItem )
			{
				szName = pEtcItem->GetName();
			}

			ioUIRenderImage *pImg = g_UIImageSetMgr.CreateImageByFullName( "UIIconPack10#goods_soldiersummon" );
			ConnectionTimeEventBtn *pBtn = dynamic_cast< ConnectionTimeEventBtn* > ( FindChildWnd( ID_ICON_BTN ) );
			if( pBtn )
				pBtn->SetIcon( pImg, 0.65f, szName );

			SetTitleText( STR(1) );
		}
		else
		{
			ioHashString szName;
			RandomMachineInfo *pInfo = g_ExtraItemInfoMgr.GetRandomMachineInfo( 11 ); // 11 초급 장비 보급
			if( pInfo )
			{
				szName = pInfo->GetName();
			}

			ioUIRenderImage *pImg = g_UIImageSetMgr.CreateImageByFullName( "UIIconPack11#goods_extra0" );
			ConnectionTimeEventBtn *pBtn = dynamic_cast< ConnectionTimeEventBtn* > ( FindChildWnd( ID_ICON_BTN ) );
			if( pBtn )
				pBtn->SetIcon( pImg, 0.65f, szName );

			SetTitleText( STR(2) );
		}

		ConnectionTimeEventUserNode *pEventNode = dynamic_cast<ConnectionTimeEventUserNode*>( g_MyInfo.GetEventUserMgr().GetEventUserNode( EVT_CONNECTION_TIME ) );
		if( pEventNode )
		{
			if( pEventNode->GetValue( VA_CT_IS_CHANCE ) == 1 )
			{
				ShowChildWnd( ID_CHANCE_BTN );
			}
			else
			{
				HideChildWnd( ID_CHANCE_BTN );
			}
		}

		m_bProcess = true;
	}
}

void ConnectionTimeEventWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_EXIT_BTN:
		if( cmd == IOBN_BTNUP )
		{
			HideWnd();
		}
		break;
	case ID_CHANCE_BTN:
		if( cmd == IOBN_BTNUP )
		{
			ConnectionTimeEventUserNode *pEventNode = dynamic_cast<ConnectionTimeEventUserNode*>( g_MyInfo.GetEventUserMgr().GetEventUserNode( EVT_CONNECTION_TIME ) );
			if( pEventNode )
			{
				if( pEventNode->GetValue( VA_CT_IS_CHANCE ) == 1 )
				{
					pEventNode->SetValue( VA_CT_IS_CHANCE, 0 );

					SP2Packet kPacket( CTPK_EVENT_DATA_UPDATE );
					kPacket << (int) pEventNode->GetType();
					kPacket << true; // use chance
					TCPNetwork::SendToServer( kPacket );
				}
			}
		}
		break;
	}
}

void ConnectionTimeEventWnd::iwm_create()
{
	m_dwStyle |= IWS_PROCESS_ALWAYS;
}

void ConnectionTimeEventWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
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

void ConnectionTimeEventWnd::OnProcess( float fTimePerSec )
{
	if( !m_bProcess ) return;

	if( !g_EventMgr.IsAlive( EVT_CONNECTION_TIME, g_MyInfo.GetChannelingType() ) )
	{
		// 이벤트 종료
		HideWnd();
		m_bProcess = false;
	}
	else
	{
		if( IsShow() )
			ioMovingWnd::OnProcess( fTimePerSec );

		ProcessChanceRequest();
	}
}

void ConnectionTimeEventWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	ConnectionTimeEventUserNode *pEventNode = dynamic_cast<ConnectionTimeEventUserNode*>( g_MyInfo.GetEventUserMgr().GetEventUserNode( EVT_CONNECTION_TIME ) );
	if( !pEventNode )
		return;

	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );

	int iTotalMinutes = max( 1, g_EventMgr.GetValue( EVT_CONNECTION_TIME, EA_CT_CHANCE_TIME ) );

	bool bLowLevel = false;
	if( g_MyInfo.GetGradeLevel() <= g_EventMgr.GetValue( EVT_CONNECTION_TIME, EA_CT_PRESENT_LOW_USER_LEVEL ) )
		bLowLevel = true;

	if( pEventNode->GetValue( VA_CT_IS_CHANCE ) == 1 )
	{
		if( bLowLevel )
			g_FontMgr.PrintText( iXPos + 119, iYPos + 39, FONT_SIZE_12, STR(1) );
		else
			g_FontMgr.PrintText( iXPos + 119, iYPos + 39, FONT_SIZE_12, STR(2) );

		return;
	}

	// 남은 분 계산
	enum { YEAR = 2010, };
	time_t kChanceTime = DateHelp::ConvertSecondTime( YEAR,  pEventNode->GetValue( VA_CT_GET_CHANCE_DATE )/1000000, (pEventNode->GetValue( VA_CT_GET_CHANCE_DATE )/10000)%100, (pEventNode->GetValue( VA_CT_GET_CHANCE_DATE )/100)%100, pEventNode->GetValue( VA_CT_GET_CHANCE_DATE )%100, 0 );
	time_t kCurServerTime = g_MyInfo.GetServerDate();
	kCurServerTime += ( REALGETTIME()/FLOAT1000 );// 현재까지의 클라이언트초를 넣는다.
	int iGapSec = kChanceTime - kCurServerTime;
	enum { MINUTE_SEC = 60, };
	int iRemainMinutes = iGapSec / MINUTE_SEC;
	int iPassedMinutes = iTotalMinutes-iRemainMinutes;
	iPassedMinutes--; // MINUTE_SEC 나눌때 나머지값이 삭제되므로 1분을 줄여서 보정
	
	if( iPassedMinutes < 0 )
		iPassedMinutes = 0;
	else if( iPassedMinutes > iTotalMinutes )
		iPassedMinutes = iTotalMinutes;

	float fTempPer = (float)iPassedMinutes / (float)iTotalMinutes;
	
	// 시간 게이지
	if( !m_pGaugeBack || !m_pGaugeLine || !m_pTimeColon || !m_pTimeText )
		return;

	m_pGaugeBack->Render( iXPos + 68, iYPos + 68 );
	m_pGaugeLine->RenderWidthCut( iXPos + 68, iYPos + 68, 0, (float)m_pGaugeLine->GetWidth() * fTempPer );

	int iHours = iPassedMinutes / 60 ;
	m_pTimeText->RenderNum( iXPos + 107, iYPos + 70, iHours );
	m_pTimeColon->Render( iXPos + 114, iYPos + 70 );

	int iMinutes = iPassedMinutes % 60;
	if( iMinutes < 10 )
	{
		m_pTimeText->RenderNum( iXPos + 117, iYPos + 70, 0 );
		m_pTimeText->RenderNum( iXPos + 124, iYPos + 70, iMinutes );
	}
	else
	{
		m_pTimeText->RenderNum( iXPos + 117, iYPos + 70, iMinutes, -FLOAT1 );
	}

	enum { MINS_PER_HOUR = 60, };
	// 찬스 사용
	g_FontMgr.PrintText( iXPos + 119, iYPos + 43, FONT_SIZE_12, STR(3), iTotalMinutes/MINS_PER_HOUR );
}

void ConnectionTimeEventWnd::ProcessChanceRequest()
{
	if( m_dwCheckChanceTime == 0 )
		m_dwCheckChanceTime = REALGETTIME();

	enum { CHECK_TIME = 60000, }; // 1분 
	if( REALGETTIME() - m_dwCheckChanceTime < CHECK_TIME )
		return;
	m_dwCheckChanceTime = REALGETTIME();

	ConnectionTimeEventUserNode *pEventNode = dynamic_cast<ConnectionTimeEventUserNode*>( g_MyInfo.GetEventUserMgr().GetEventUserNode( EVT_CONNECTION_TIME ) );
	if( !pEventNode )
		return;

	if( pEventNode->GetValue( VA_CT_IS_CHANCE ) == 1 )
		return;

	if( pEventNode->GetValue( VA_CT_GET_CHANCE_DATE ) <= 0 )
		return;

	enum { YEAR = 2010, };
	time_t kChanceTime = DateHelp::ConvertSecondTime( YEAR,  pEventNode->GetValue( VA_CT_GET_CHANCE_DATE )/1000000, (pEventNode->GetValue( VA_CT_GET_CHANCE_DATE )/10000)%100, (pEventNode->GetValue( VA_CT_GET_CHANCE_DATE )/100)%100, pEventNode->GetValue( VA_CT_GET_CHANCE_DATE )%100, 0 );
	kChanceTime += 60; // 로컬에서 계산을 하는 것이므로 오차가 있을 수 있어 1분 이후에 요청한다.
	time_t kCurServerTime = g_MyInfo.GetServerDate();
	kCurServerTime += ( REALGETTIME()/FLOAT1000 );// 현재까지의 클라이언트초를 넣는다.

	if( kChanceTime > kCurServerTime )
		return;

	SP2Packet kPacket( CTPK_EVENT_DATA_UPDATE );
	kPacket << (int) pEventNode->GetType();
	kPacket << false; // use chance
	TCPNetwork::SendToServer( kPacket );
}

//-------------------------------------------------------------------------------------------------------------------------------
// EVT_CONNECTION_TIME_SELECT_GIFT
ConnectionTimeSelectGiftEventBtn::ConnectionTimeSelectGiftEventBtn()
{
	m_pIcon         = NULL;

	m_pToolTipCen   = NULL;
	m_pToolTipLMid  = NULL;
	m_pToolTipRMid  = NULL;
	m_pToolTipLeft  = NULL;
	m_pToolTipRight = NULL;

	m_iMaxGauge     = 0;
	m_iCurGauge     = 0;

	m_fCurScaleRate    = 0.0f;
	m_dwScaleStartTime = 0;
}

ConnectionTimeSelectGiftEventBtn::~ConnectionTimeSelectGiftEventBtn()
{
	SAFEDELETE( m_pIcon );

	SAFEDELETE( m_pToolTipCen );
	SAFEDELETE( m_pToolTipLMid );
	SAFEDELETE( m_pToolTipRMid );
	SAFEDELETE( m_pToolTipLeft );
	SAFEDELETE( m_pToolTipRight );
}

void ConnectionTimeSelectGiftEventBtn::SetIcon( const ioHashString &rszName, int iMaxGauge )
{
	m_szName           = rszName;
	m_iMaxGauge        = iMaxGauge;
}

void ConnectionTimeSelectGiftEventBtn::OnDrawNormal( int iXPos, int iYPos )
{
	ioButton::OnDrawNormal( iXPos, iYPos );
	if( !m_pIcon ) return;

	ConnectionTimeSelectGiftEventUserNode *pEventNode = dynamic_cast<ConnectionTimeSelectGiftEventUserNode*>( g_MyInfo.GetEventUserMgr().GetEventUserNode( EVT_CONNECTION_TIME_SELECT_GIFT ) );
	if( !pEventNode )
		return;

	if( pEventNode->GetValue( VA_CTSG_IS_CHANCE ) == 1 )
		m_pIcon->Render( iXPos + GetWidth() / 2, iYPos + GetHeight() / 2, UI_RENDER_NORMAL, TFO_BILINEAR );
	else
		RenderGaugeIcon( iXPos + GetWidth() / 2, iYPos + GetHeight() / 2, m_pIcon, CTSG_ICON_HEIGHT, m_iCurGauge, m_iMaxGauge, TFO_BILINEAR );
}

void ConnectionTimeSelectGiftEventBtn::OnDrawOvered( int iXPos, int iYPos )
{
	ioButton::OnDrawOvered( iXPos, iYPos );
	if( !m_pIcon ) return;

	ConnectionTimeSelectGiftEventUserNode *pEventNode = dynamic_cast<ConnectionTimeSelectGiftEventUserNode*>( g_MyInfo.GetEventUserMgr().GetEventUserNode( EVT_CONNECTION_TIME_SELECT_GIFT ) );
	if( !pEventNode )
		return;

	if( pEventNode->GetValue( VA_CTSG_IS_CHANCE ) == 1 )
	{
		m_pIcon->Render( iXPos + GetWidth() / 2, iYPos + GetHeight() / 2, UI_RENDER_NORMAL, TFO_BILINEAR );
		m_pIcon->Render( iXPos + GetWidth() / 2, iYPos + GetHeight() / 2, UI_RENDER_SCREEN, TFO_BILINEAR );
	}
	else
		RenderGaugeIcon( iXPos + GetWidth() / 2, iYPos + GetHeight() / 2, m_pIcon, CTSG_ICON_HEIGHT, m_iCurGauge, m_iMaxGauge, TFO_BILINEAR );

	RenderToolTip();
}

void ConnectionTimeSelectGiftEventBtn::OnDrawPushed( int iXPos, int iYPos )
{
	ioButton::OnDrawPushed( iXPos, iYPos );
	if( !m_pIcon ) return;

	ConnectionTimeSelectGiftEventUserNode *pEventNode = dynamic_cast<ConnectionTimeSelectGiftEventUserNode*>( g_MyInfo.GetEventUserMgr().GetEventUserNode( EVT_CONNECTION_TIME_SELECT_GIFT ) );
	if( !pEventNode )
		return;

	if( pEventNode->GetValue( VA_CTSG_IS_CHANCE ) == 1 )
	{
		m_pIcon->Render( iXPos + GetWidth() / 2, iYPos + GetHeight() / 2, UI_RENDER_NORMAL, TFO_BILINEAR );
		m_pIcon->Render( iXPos + GetWidth() / 2, iYPos + GetHeight() / 2, UI_RENDER_SCREEN, TFO_BILINEAR );
	}
	else
		RenderGaugeIcon( iXPos + GetWidth() / 2, iYPos + GetHeight() / 2, m_pIcon, CTSG_ICON_HEIGHT, m_iCurGauge, m_iMaxGauge, TFO_BILINEAR );

	RenderToolTip();
}

void ConnectionTimeSelectGiftEventBtn::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "Icon")
	{
		SAFEDELETE( m_pIcon );
		m_pIcon = pImage;
	}
	else if( szType == "ToolTipCenter")
	{
		SAFEDELETE( m_pToolTipCen );
		m_pToolTipCen = pImage;
	}
	else if( szType == "ToolTipLMiddle" )
	{
		SAFEDELETE( m_pToolTipLMid );
		m_pToolTipLMid = pImage;
	}
	else if( szType == "ToolTipRMiddle" )
	{
		SAFEDELETE( m_pToolTipRMid );
		m_pToolTipRMid = pImage;
	}
	else if( szType == "ToolTipLeft" )
	{
		SAFEDELETE( m_pToolTipLeft );
		m_pToolTipLeft = pImage;
	}
	else if( szType == "ToolTipRight" )
	{
		SAFEDELETE( m_pToolTipRight );
		m_pToolTipRight = pImage;
	}
	else
	{
		ioButton::AddRenderImage( szType, pImage );
	}
}


void ConnectionTimeSelectGiftEventBtn::RenderToolTip()
{
	if( m_szName.IsEmpty() )
		return;

	int iXPos, iYPos;
	iXPos = GetDerivedPosX() + 21;
	iYPos = GetDerivedPosY() - 30;

	int iTextSize = g_FontMgr.GetTextWidth( m_szName.c_str(), TS_NORMAL, FLOAT05 );

	enum
	{
		BACK_HEIGHT = 18,
		SIDE_BLANK = 3,
	};

	if( iTextSize > 0 && m_pToolTipCen && m_pToolTipLMid && m_pToolTipRMid && m_pToolTipLeft && m_pToolTipRight )
	{
		int iLHalfPos = iTextSize / 2 + SIDE_BLANK;
		int iHalfSize = (iTextSize - m_pToolTipCen->GetWidth()) / 2 + SIDE_BLANK;

		//Left
		int iCurPos = iXPos - iLHalfPos - m_pToolTipLeft->GetWidth();
		m_pToolTipLeft->Render( iCurPos, iYPos );

		//Left Mid
		iCurPos = iXPos - iLHalfPos;

		m_pToolTipLMid->SetSize( iHalfSize, BACK_HEIGHT );
		m_pToolTipLMid->SetHorzFormatting( ioUIRenderImage::HF_STRETCH );
		m_pToolTipLMid->Render( iCurPos, iYPos );

		//Center
		iCurPos = iCurPos + iHalfSize;
		m_pToolTipCen->Render( iCurPos, iYPos );

		//Right Mid
		iCurPos = iCurPos + m_pToolTipCen->GetWidth();
		m_pToolTipRMid->SetSize( iHalfSize, BACK_HEIGHT );
		m_pToolTipRMid->SetHorzFormatting( ioUIRenderImage::HF_STRETCH );
		m_pToolTipRMid->Render( iCurPos, iYPos );

		//Right
		iCurPos = iCurPos + iHalfSize;
		m_pToolTipRight->Render( iCurPos, iYPos );
	}

	// Text
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetAlignType( TAT_CENTER );


	// 바깥쪽 글자의 1px을 반투명하게 찍기 위해서
	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );

	g_FontMgr.PrintTextAlpha(iXPos, iYPos + 3, FLOAT05, 127, m_szName.c_str() );

	g_FontMgr.SetTextStyle( TS_OUTLINE_FULL );
	g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
	g_FontMgr.PrintTextAlpha( iXPos, iYPos + 3, FLOAT05, 255, m_szName.c_str() );
}

void ConnectionTimeSelectGiftEventBtn::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;

	ProcessScale();
}

void ConnectionTimeSelectGiftEventBtn::ProcessScale()
{
	ConnectionTimeSelectGiftEventUserNode *pEventNode = dynamic_cast<ConnectionTimeSelectGiftEventUserNode*>( g_MyInfo.GetEventUserMgr().GetEventUserNode( EVT_CONNECTION_TIME_SELECT_GIFT ) );
	if( !pEventNode )
		return;

	if( pEventNode->GetValue( VA_CTSG_IS_CHANCE ) == 0 )
	{
		m_fCurScaleRate = FLOAT1;
		if( m_pIcon )
			m_pIcon->SetScale( m_fCurScaleRate );
		return;
	}

	enum { SCALE_TIME = 300, };
	if( m_dwScaleStartTime == 0 )
		m_dwScaleStartTime = FRAMEGETTIME();

	DWORD dwGapTime = FRAMEGETTIME() - m_dwScaleStartTime;
	if( dwGapTime > SCALE_TIME )
	{
		dwGapTime -= SCALE_TIME;
		m_dwScaleStartTime = FRAMEGETTIME() - ( dwGapTime % SCALE_TIME );
	}

	float fTimeRate = (float)dwGapTime / (float)SCALE_TIME;
	m_fCurScaleRate = FLOAT1 + 0.2f * sinf( D3DX_PI * fTimeRate );

	if( m_pIcon )
		m_pIcon->SetScale( m_fCurScaleRate );
}

int ConnectionTimeSelectGiftEventBtn::GetRenderImageStartHeight( ioUIRenderImage *pImage, int iSrcHeight, int iCur, int iMax )
{
	if( iMax == iCur ) return 0;
	if( !pImage )	return 0;

	float fGap = (float)iCur / (float)iMax;
	float fGapHeight = (float)iSrcHeight * fGap;
	int iOffYPos = pImage->GetOffsetY();
	int iHeight  = pImage->GetHeight();

	return iOffYPos + ( (iSrcHeight / 2) - fGapHeight );							
}

void ConnectionTimeSelectGiftEventBtn::RenderGaugeIcon( int xPos, int yPos, ioUIRenderImage *pImage, int iSrcHeight, int iCur, int iMax , TextureFilterOption eOption /*= TFO_NEAREST */ )
{
	if( !pImage )
		return;

	int iRealHeight = GetRenderImageStartHeight( pImage, iSrcHeight, iCur, iMax );

	pImage->Render( xPos, yPos, UI_RENDER_GRAY, eOption );
	pImage->RenderHeightCut( xPos, yPos, iRealHeight, pImage->GetHeight(), UI_RENDER_NORMAL, eOption );

	if( iRealHeight + 2 <= pImage->GetHeight() )
	{
		pImage->SetColor( 0, 0, 0 );
		pImage->RenderHeightCut( xPos, yPos, iRealHeight, iRealHeight + 2, UI_RENDER_NORMAL, eOption );
		pImage->SetColor( 255, 255, 255 );
	}
}

//-----------------------------------------------
bool ConnectionTimeSelectGiftEventWnd::m_bProcess = false;

ConnectionTimeSelectGiftEventWnd::ConnectionTimeSelectGiftEventWnd()
{
	m_dwCheckChanceTime = 0;
	m_dwSelectID        = 0;
}

ConnectionTimeSelectGiftEventWnd::~ConnectionTimeSelectGiftEventWnd()
{
}

void ConnectionTimeSelectGiftEventWnd::iwm_show()
{
	if( !g_EventMgr.IsAlive( EVT_CONNECTION_TIME_SELECT_GIFT, g_MyInfo.GetChannelingType() ) )
	{
		// 이벤트 종료
		HideWnd();
		m_bProcess = false;
	}
	else
	{
		m_dwSelectID = 0;

		int iTotalMinutes = max( 1, g_EventMgr.GetValue( EVT_CONNECTION_TIME_SELECT_GIFT, EA_CTSG_CHANCE_TIME ) );

		for (int i = 0; i < MAX_ICON_BTN ; i++)
		{
			ConnectionTimeSelectGiftEventBtn *pBtn = dynamic_cast<ConnectionTimeSelectGiftEventBtn*> ( FindChildWnd( ID_ICON_1_BTN + i ) );
			if( !pBtn )
				continue;

			if( !pBtn->GetName().IsEmpty() )
				continue;

			ioHashString szName;
			if( i == 0 )
			{
				ioEtcItem *pEtcItem = g_EtcItemMgr.FindEtcItem( ioEtcItem::EIT_ETC_GASHAPON );
				if( pEtcItem )
					szName = pEtcItem->GetName();
			}
			else
			{
				RandomMachineInfo *pInfo = g_ExtraItemInfoMgr.GetRandomMachineInfo( 11 ); // 11 초급 장비 보급
				if( pInfo )
					szName = pInfo->GetName();
			}

			pBtn->SetIcon( szName, iTotalMinutes );
		}

		m_bProcess = true;
	}
}

void ConnectionTimeSelectGiftEventWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_EXIT_BTN:
		if( cmd == IOBN_BTNUP )
		{
			HideWnd();
		}
		break;
	case ID_ICON_1_BTN:
	case ID_ICON_2_BTN:
		if( cmd == IOBN_BTNUP )
		{
			ShowMsgBox( dwID );
		}
		break;
	case MESSAGE_BOX3: // g_GUIMgr.SetPrevMsgListBox() OR g_GUIMgr.SetPrevMsgListBoxWithTitle()
		if( cmd == IOBN_BTNUP )
		{
			if( param == IDYES )
			{
				SendRequestPresent();
			}
		}
		break;
	}
}

void ConnectionTimeSelectGiftEventWnd::OnProcess( float fTimePerSec )
{
	if( !m_bProcess ) return;

	if( !g_EventMgr.IsAlive( EVT_CONNECTION_TIME_SELECT_GIFT, g_MyInfo.GetChannelingType() ) )
	{
		// 이벤트 종료
		HideWnd();
		m_bProcess = false;
	}
	else
	{
		if( IsShow() )
			ioMovingWnd::OnProcess( fTimePerSec );

		ProcessChanceRequest();
	}
}

void ConnectionTimeSelectGiftEventWnd::OnRender()
{
	ioMovingWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	ConnectionTimeSelectGiftEventUserNode *pEventNode = dynamic_cast<ConnectionTimeSelectGiftEventUserNode*>( g_MyInfo.GetEventUserMgr().GetEventUserNode( EVT_CONNECTION_TIME_SELECT_GIFT ) );
	if( !pEventNode )
		return;

	
	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetBkColor( 0, 0, 0 );

	if( pEventNode->GetValue( VA_CTSG_IS_CHANCE ) == 1 )
	{
		enum { X_OFFSET = 146, Y_OFFSET = 49, Y_GAP = 18, };
		g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
		g_FontMgr.PrintText( iXPos + X_OFFSET, iYPos + Y_OFFSET, FONT_SIZE_13, STR(1) );
		g_FontMgr.PrintText( iXPos + X_OFFSET, iYPos + Y_OFFSET + Y_GAP, FONT_SIZE_13, STR(2) );
	}
	else
	{
		enum { X_OFFSET = 146, Y_OFFSET = 40, Y_GAP = 18, };
		g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
		g_FontMgr.PrintText( iXPos + X_OFFSET, iYPos + Y_OFFSET, FONT_SIZE_13, STR(3) );
		g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
		g_FontMgr.PrintText( iXPos + X_OFFSET, iYPos + Y_OFFSET + Y_GAP, FONT_SIZE_13, STR(4) , pEventNode->GetValue( VA_CTSG_GET_CHANCE_DATE )/1000000, (pEventNode->GetValue( VA_CTSG_GET_CHANCE_DATE )/10000)%100 );
		g_FontMgr.PrintText( iXPos + X_OFFSET, iYPos + Y_OFFSET + Y_GAP*2, FONT_SIZE_13, STR(5) , (pEventNode->GetValue( VA_CTSG_GET_CHANCE_DATE )/100)%100, pEventNode->GetValue( VA_CTSG_GET_CHANCE_DATE )%100 );
	}

	int iTotalMinutes = max( 1, g_EventMgr.GetValue( EVT_CONNECTION_TIME_SELECT_GIFT, EA_CTSG_CHANCE_TIME ) );

	// 남은 분 계산
	enum { YEAR = 2010, };
	time_t kChanceTime = DateHelp::ConvertSecondTime( YEAR,  pEventNode->GetValue( VA_CTSG_GET_CHANCE_DATE )/1000000, (pEventNode->GetValue( VA_CTSG_GET_CHANCE_DATE )/10000)%100, (pEventNode->GetValue( VA_CTSG_GET_CHANCE_DATE )/100)%100, pEventNode->GetValue( VA_CTSG_GET_CHANCE_DATE )%100, 0 );
	time_t kCurServerTime = g_MyInfo.GetServerDate();
	kCurServerTime += ( REALGETTIME()/FLOAT1000 );// 현재까지의 클라이언트초를 넣는다.
	int iGapSec = kChanceTime - kCurServerTime;
	enum { MINUTE_SEC = 60, };
	int iRemainMinutes = iGapSec / MINUTE_SEC;
	int iPassedMinutes = iTotalMinutes-iRemainMinutes;
	iPassedMinutes--; // MINUTE_SEC 나눌때 나머지값이 삭제되므로 1분을 줄여서 보정

	if( iPassedMinutes < 0 )
		iPassedMinutes = 0;
	else if( iPassedMinutes > iTotalMinutes )
		iPassedMinutes = iTotalMinutes;

	for (int i = 0; i < MAX_ICON_BTN ; i++)
	{
		ConnectionTimeSelectGiftEventBtn *pBtn = dynamic_cast<ConnectionTimeSelectGiftEventBtn*> ( FindChildWnd( ID_ICON_1_BTN + i ) );
		if( pBtn )
			pBtn->SetCurGauge( iPassedMinutes );
	}
}

void ConnectionTimeSelectGiftEventWnd::ProcessChanceRequest()
{
	if( m_dwCheckChanceTime == 0 )
		m_dwCheckChanceTime = REALGETTIME();

	enum { CHECK_TIME = 60000, }; // 1분 
	if( REALGETTIME() - m_dwCheckChanceTime < CHECK_TIME )
		return;
	m_dwCheckChanceTime = REALGETTIME();

	ConnectionTimeSelectGiftEventUserNode *pEventNode = dynamic_cast<ConnectionTimeSelectGiftEventUserNode*>( g_MyInfo.GetEventUserMgr().GetEventUserNode( EVT_CONNECTION_TIME_SELECT_GIFT ) );
	if( !pEventNode )
		return;

	if( pEventNode->GetValue( VA_CTSG_IS_CHANCE ) == 1 )
		return;

	if( pEventNode->GetValue( VA_CTSG_GET_CHANCE_DATE ) <= 0 )
		return;

	enum { YEAR = 2010, };
	time_t kChanceTime = DateHelp::ConvertSecondTime( YEAR,  pEventNode->GetValue( VA_CTSG_GET_CHANCE_DATE )/1000000, (pEventNode->GetValue( VA_CTSG_GET_CHANCE_DATE )/10000)%100, (pEventNode->GetValue( VA_CTSG_GET_CHANCE_DATE )/100)%100, pEventNode->GetValue( VA_CTSG_GET_CHANCE_DATE )%100, 0 );
	kChanceTime += 60; // 로컬에서 계산을 하는 것이므로 오차가 있을 수 있어 1분 이후에 요청한다.
	time_t kCurServerTime = g_MyInfo.GetServerDate();
	kCurServerTime += ( REALGETTIME()/FLOAT1000 );// 현재까지의 클라이언트초를 넣는다.

	if( kChanceTime > kCurServerTime )
		return;

	SP2Packet kPacket( CTPK_EVENT_DATA_UPDATE );
	kPacket << (int) pEventNode->GetType();
	kPacket << -1; // updadte time
	TCPNetwork::SendToServer( kPacket );
}

void ConnectionTimeSelectGiftEventWnd::ShowMsgBox( DWORD dwID )
{
	ConnectionTimeSelectGiftEventUserNode *pEventNode = dynamic_cast<ConnectionTimeSelectGiftEventUserNode*>( g_MyInfo.GetEventUserMgr().GetEventUserNode( EVT_CONNECTION_TIME_SELECT_GIFT ) );
	if( !pEventNode )
	{
		LOG.PrintTimeAndLog( 0, "%s pEventNode == NULL. ", __FUNCTION__ );
		return;
	}
	
	if( pEventNode->GetValue( VA_CTSG_IS_CHANCE ) == 0 )
		return;

	ConnectionTimeSelectGiftEventBtn *pBtn = dynamic_cast<ConnectionTimeSelectGiftEventBtn*> ( FindChildWnd( dwID ) );
	if( !pBtn )
	{
		LOG.PrintTimeAndLog( 0, "%s pBtn == NULL.", __FUNCTION__ );
		return;
	}

	ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];

	kPrinter[0].SetTextStyle( TS_NORMAL );
	kPrinter[0].SetBkColor( 0, 0, 0 );
	kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter[0].AddTextPiece( FONT_SIZE_15, STR(1), pBtn->GetName().c_str() );

	kPrinter[0].SetTextStyle( TS_NORMAL );
	kPrinter[0].SetBkColor( 0, 0, 0 );
	kPrinter[0].SetTextColor( TCT_DEFAULT_GRAY );
	kPrinter[0].AddTextPiece( FONT_SIZE_15, STR(2) );

	kPrinter[1].SetTextStyle( TS_NORMAL );
	kPrinter[1].SetBkColor( 0, 0, 0 );
	kPrinter[1].SetTextColor( TCT_DEFAULT_GRAY );
	kPrinter[1].AddTextPiece( FONT_SIZE_15, STR(3) );

	g_GUIMgr.SetPrevMsgListBoxWithTitle( NULL, MB_YESNO, this, kPrinter, STR(4), "", STR(5), STR(6) );

	m_dwSelectID = dwID;
}

void ConnectionTimeSelectGiftEventWnd::SendRequestPresent()
{
	ConnectionTimeSelectGiftEventUserNode *pEventNode = dynamic_cast<ConnectionTimeSelectGiftEventUserNode*>( g_MyInfo.GetEventUserMgr().GetEventUserNode( EVT_CONNECTION_TIME_SELECT_GIFT ) );
	if( !pEventNode )
	{
		LOG.PrintTimeAndLog( 0, "%s pEventNode == NULL. ", __FUNCTION__ );
		return;
	}
	
	if( pEventNode->GetValue( VA_CTSG_IS_CHANCE ) == 0 )
	{
		LOG.PrintTimeAndLog( 0, "%s fail state. ", __FUNCTION__ );
		return;
	}
	
	pEventNode->SetValue( VA_CTSG_IS_CHANCE, 0 );

	int iPresentReceiveType = -1;
	if( m_dwSelectID == ID_ICON_1_BTN )
		iPresentReceiveType = 1;
	else
		iPresentReceiveType = 2;

	SP2Packet kPacket( CTPK_EVENT_DATA_UPDATE );
	kPacket << (int) pEventNode->GetType();
	kPacket << iPresentReceiveType;
	TCPNetwork::SendToServer( kPacket );
}

void ConnectionTimeSelectGiftEventWnd::iwm_create()
{
	m_dwStyle |= IWS_PROCESS_ALWAYS;
}