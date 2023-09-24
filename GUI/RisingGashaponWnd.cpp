#include "StdAfx.h"

#include "RisingGashaponWnd.h"

#include "../io3DEngine/ioWnd.h"
#include "../io3DEngine/ioScroll.h"
#include "..\io3DEngine\ioGUIManager.h"

#include "..\WndID.h"

#include "NewShopBuyWnd.h"
#include "..\ioEtcItem.h"
#include "..\Channeling\ioChannelingNodeParent.h"
#include "..\ioVoiceChat\Fourier.h"

#include "../ioEtcItem.h"
#include "../ioPlayStage.h"

#include "ioExtraItemListWnd.h"


const int C_INVALID_INDEX = -1;

////////////////////////////////////////////////////////////////////////////////


RisingGashaponItemInfoBtn::RisingGashaponItemInfoBtn()
{

	m_pSelect = NULL;
	m_pPresentIcon = NULL;
	m_pEffect = NULL;
	m_pPlus = NULL;
	m_pNum = NULL;
	m_pEmpty = NULL;
	m_pGain = NULL;

	m_bGetCard = false;
	m_bSelect  = false;

	m_iCurEffectArray     = -1;
	m_dwRollingEffectTime = 0;
	m_dwRollingEffectCheckTime = 0;
	m_vEndEfffect_S.clear();


}

RisingGashaponItemInfoBtn::~RisingGashaponItemInfoBtn()
{
	SAFEDELETE( m_pSelect );

	SAFEDELETE( m_pPresentIcon );
	SAFEDELETE( m_pEffect );

	SAFEDELETE( m_pNum );
	SAFEDELETE( m_pPlus );
	SAFEDELETE( m_pEmpty );
	SAFEDELETE( m_pGain );



}

void RisingGashaponItemInfoBtn::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{

	if( szType == "PlusText" )
	{
		SAFEDELETE( m_pPlus );
		m_pPlus = pImage;
	}
	else if( szType == "NumText" )
	{
		SAFEDELETE( m_pNum );
		m_pNum = pImage;
	}
	else if( szType == "stamp_gain" )
	{
		SAFEDELETE( m_pGain );
		m_pGain = pImage;
	}
	else
	{
		ioButton::AddRenderImage( szType, pImage );
	}
}

void RisingGashaponItemInfoBtn::AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame )
{
	if( szType == "PresentSelect" )
	{
		SAFEDELETE( m_pSelect );
		m_pSelect = pFrame;
	}
	else if( szType == "empty_frm" )
	{
		SAFEDELETE( m_pEmpty );
		m_pEmpty = pFrame;
	}
	else
	{
		ioButton::AddRenderFrame( szType, pFrame );
	}
}


void RisingGashaponItemInfoBtn::SetPresent( const GashaponPresent& rkPresent, ioHashString strName )
{
	m_GashaponPresent = rkPresent;

	// icon
	SAFEDELETE( m_pPresentIcon );
	const ioHashString &rkPresentIcon = g_PresentMgr.GetPresentIconText( rkPresent.m_iType, rkPresent.m_iValue1, rkPresent.m_iValue2 );
	m_pPresentIcon = g_UIImageSetMgr.CreateImageByFullName( rkPresentIcon );

	StringVector vItemName;
	Help::SplitTextWidth( strName.c_str(), vItemName, 62, TS_NORMAL, FONT_SIZE_11 );

	int nLineSize = vItemName.size();
	for (int i = 0; i < MAX_SPLIT_LINE ; i++)
	{

		if(COMPARE(i, 0, nLineSize))
		{
			m_szTitles[i] = vItemName[i].c_str();
		}
		else
		{
			m_szTitles[i] = "";
		}
	}

}

void RisingGashaponItemInfoBtn::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;

	RollingEffect( fTimePerSec );
}

void RisingGashaponItemInfoBtn::OnRender()
{
	ioButton::OnRender();
}

void RisingGashaponItemInfoBtn::ParseExtraInfo( ioXMLElement &xElement )
{
	m_dwRollingEffectTime = xElement.GetIntAttribute_e( "Effect_Gap_Time" );

	m_vEndEfffect_S.clear();
	int iMax = xElement.GetIntAttribute_e( "Max_Success_Effect" );
	m_vEndEfffect_S.reserve( iMax );
	for (int i = 0; i < iMax ; i++)
	{
		char szName[MAX_PATH]="";
		StringCbPrintf_e( szName, sizeof( szName ), "Success_Effect_%d", i+1 );
		ioHashString szEffect = xElement.GetStringAttribute( szName );
		if( szEffect == "" )
			break;
		m_vEndEfffect_S.push_back( szEffect );
	}

	char szKeyName[MAX_PATH]="";
	StringCbPrintf_e( szKeyName, sizeof( szKeyName ), "success_sound" );
	m_szSuccessSound = xElement.GetStringAttribute( szKeyName );
}

void RisingGashaponItemInfoBtn::iwm_create()
{
}

void RisingGashaponItemInfoBtn::iwm_show()
{
	SetEmptyCard();
}

void RisingGashaponItemInfoBtn::iwm_hide()
{
	if( !m_szSuccessSound.IsEmpty() )
		g_SoundMgr.StopSound( m_szSuccessSound, 0 );
}


void RisingGashaponItemInfoBtn::SetEmptyCard()
{
	m_GashaponPresent.Clear();

	m_bGetCard     = false;
	m_bSelect      = false;
	m_iCurEffectArray          = -1;
	m_dwRollingEffectCheckTime = 0;

	for (int i = 0; i < MAX_SPLIT_LINE ; i++)
	{
		m_szTitles[i].Clear();
	}
}

void RisingGashaponItemInfoBtn::SetGetCard( bool bGetCard )
{
	m_bGetCard = bGetCard;
}

void RisingGashaponItemInfoBtn::SetSelect( bool bSelect )
{
	m_bSelect = bSelect;
}

void RisingGashaponItemInfoBtn::RollingEffect( float fTimePerSec )
{
	if(  m_dwRollingEffectCheckTime == 0 )
		return;

	DWORD dwCurTime = FRAMEGETTIME();
	if( dwCurTime - m_dwRollingEffectCheckTime > m_dwRollingEffectTime )		// 교체
	{
		SAFEDELETE( m_pEffect );

		m_iCurEffectArray++;

		if( !COMPARE( m_iCurEffectArray, 0, (int)m_vEndEfffect_S.size() ) )
			m_iCurEffectArray = 0;

		ioHashString szEffect;
		szEffect = m_vEndEfffect_S[m_iCurEffectArray];

		if( !szEffect.IsEmpty() )
		{
			m_pEffect = g_UIImageSetMgr.CreateImageByFullName( szEffect );
		}

		m_dwRollingEffectCheckTime = dwCurTime;

		if( m_iCurEffectArray >= (int)m_vEndEfffect_S.size()-1 ) // end
		{
			SAFEDELETE( m_pEffect );
			m_dwRollingEffectCheckTime = 0;
			if( !m_szSuccessSound.IsEmpty() )
				g_SoundMgr.PlaySound( m_szSuccessSound, DSBVOLUME_MAX, PSM_THREAD );
		}
	}
}

void RisingGashaponItemInfoBtn::SetGetEffect()
{
	m_dwRollingEffectCheckTime = FRAMEGETTIME();
	m_iCurEffectArray = -1;
	SAFEDELETE( m_pEffect );
}

void RisingGashaponItemInfoBtn::OnDrawNormal( int iXPos, int iYPos )
{
	if( IsEmptyCard() )
	{
		RenderEmpty(iXPos, iYPos, UI_RENDER_NORMAL);
	}
	else
	{
		RenderInfo( iXPos, iYPos, UI_RENDER_NORMAL );
		RenderEffect( iXPos, iYPos, UI_RENDER_ADD );
		RenderSelect( iXPos, iYPos, UI_RENDER_ADD );
	}
}

void RisingGashaponItemInfoBtn::OnDrawOvered( int iXPos, int iYPos )
{
	OnDrawNormal( iXPos, iYPos );
}

void RisingGashaponItemInfoBtn::OnDrawPushed( int iXPos, int iYPos )
{
	OnDrawNormal( iXPos, iYPos );
}

void RisingGashaponItemInfoBtn::OnDrawDisabled( int iXPos, int iYPos )
{
	OnDrawNormal( iXPos, iYPos );
}

void RisingGashaponItemInfoBtn::RenderEffect( int iXPos, int iYPos, UIRenderType eType )
{
	enum 
	{
		X_OFFSET = 35,
		Y_OFFSET = 29,
	};

	if( !m_pEffect )
		return;

	m_pEffect->SetScale( 1.36f );
	m_pEffect->Render( iXPos + X_OFFSET, iYPos + Y_OFFSET, eType, TFO_BILINEAR );
}

void RisingGashaponItemInfoBtn::RenderSelect( int iXPos, int iYPos, UIRenderType eType )
{
	if( !m_bSelect )
		return;

	if( !m_pSelect )
		return;

	m_pSelect->Render( iXPos, iYPos, eType );
}


void RisingGashaponItemInfoBtn::RenderInfo( int iXPos, int iYPos, UIRenderType eType )
{
	const float ICON_SIZE_SCALE = 0.68f;
	enum 
	{
		X_OFFSET_BACK = 35,
		Y_OFFSET_BACK = 44, 

		X_OFFSET_ICON = 35,
		Y_OFFSET_ICON = 29,

		X_OFFSET_ONE_TEXT = 35,
		Y_OFFSET_ONE_TEXT = 66,

		X_OFFSET_TWO_TEXT = 35,
		Y_OFFSET_TWO_TEXT = 60,
		TWO_TEXT_LINE_GAP = 12,

		PLUS_SIZE = 14,
		TEXT_SIZE = 20,
		REINFORCE_GAP = -4,
	};
	// back
	if( m_pNormal )
	{
		m_pNormal->Render( iXPos + X_OFFSET_BACK, iYPos + Y_OFFSET_BACK, eType, TFO_BILINEAR );
	}


	// icon
	if( m_pPresentIcon )
	{
		m_pPresentIcon->SetScale( ICON_SIZE_SCALE );
		m_pPresentIcon->Render( iXPos + X_OFFSET_ICON, iYPos + Y_OFFSET_ICON, eType, TFO_BILINEAR );
	}
	// 강화수치 표시
	if( m_pPlus && m_pNum && m_GashaponPresent.m_iValue2 > PRESENT_EXTRAITEM_DIVISION_2 )
	{
		int iReinforce = m_GashaponPresent.m_iValue2 / PRESENT_EXTRAITEM_DIVISION_2;   // /10000은 선물 value2의 장비 성장값과 동일
		int iCurPos = 0;
		int iDefaultPos = iXPos;

		int iNewXPos = iXPos + m_pPlus->GetWidth() / 2;

		int iTotalSize = PLUS_SIZE;      
		if( iReinforce < 10 )
			iTotalSize += TEXT_SIZE + REINFORCE_GAP;
		else if( iReinforce < 100 )
			iTotalSize += (TEXT_SIZE + REINFORCE_GAP) * 2;
		else
			iTotalSize += (TEXT_SIZE + REINFORCE_GAP) * 3;

		iCurPos = iNewXPos - (iTotalSize / 2);

		m_pPlus->Render( iCurPos, iYPos );
		m_pNum->RenderNum( iCurPos + ( PLUS_SIZE + REINFORCE_GAP ) , iYPos, iReinforce, REINFORCE_GAP);
	}


	// text
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetTextStyle( TS_NORMAL );

	if( eType == UI_RENDER_NORMAL )
	{
		g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
	}
	else if( eType == UI_RENDER_GRAY )
	{
		g_FontMgr.SetTextColor( TCT_DEFAULT_LIGHTGRAY );
	}

	bool bTwoLines = true;
	for (int i = 0; i < MAX_SPLIT_LINE ; i++)
	{
		if( m_szTitles[i].IsEmpty() )
		{
			bTwoLines = false;
			break;
		}
	}

	if( bTwoLines )
	{
		for (int i = 0; i < MAX_SPLIT_LINE ; i++)
		{
			g_FontMgr.PrintText( iXPos + X_OFFSET_TWO_TEXT, iYPos + Y_OFFSET_TWO_TEXT + ( TWO_TEXT_LINE_GAP*i), FONT_SIZE_11, m_szTitles[i].c_str() );
		}
	}
	else
	{
		for (int i = 0; i < MAX_SPLIT_LINE-1 ; i++)
		{
			g_FontMgr.PrintText( iXPos + X_OFFSET_ONE_TEXT, iYPos + Y_OFFSET_ONE_TEXT, FONT_SIZE_11, m_szTitles[i].c_str() );
		}
	}

	if(m_bGetCard)
	{
		if(m_pGain)
		{
			m_pGain->Render(iXPos + X_OFFSET_ICON, iYPos + Y_OFFSET_ICON, eType);
		}
	}


}

void RisingGashaponItemInfoBtn::RenderEmpty( int iXPos, int iYPos, UIRenderType eType )
{
	if( m_pEmpty )
		m_pEmpty->Render( iXPos, iYPos, eType, TFO_BILINEAR );
}



////////////////////////////////////////////////////////////////////////////////

RisingGashaponWnd::RisingGashaponWnd(void)
{

	m_sDescCard = STR(1);

	m_pLineLeft   = NULL;
	m_pLineCenter = NULL;
	m_pLineRight  = NULL;

	m_dwAniWaitDelay  = 0;
	m_dwAniRollDelay  = 0;
	m_dwAniRollCnt    = 0;
	m_dwCurAniRollCnt = 0;
	m_iMaxSelectItem  = 0;
	m_iEtcItemType    = 0;

	m_bBuyCash = true;
	m_iBuyCount = 0;
	m_eMsgType = eMSGBOX_TYPE_NONE;

}


RisingGashaponWnd::~RisingGashaponWnd(void)
{
	SAFEDELETE( m_pLineLeft );
	SAFEDELETE( m_pLineCenter );
	SAFEDELETE( m_pLineRight );
}


void RisingGashaponWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	m_dwAniWaitDelay = xElement.GetIntAttribute_e( "WaitDelay" );
	m_dwAniRollDelay = xElement.GetIntAttribute_e( "RollDelay" );
	m_dwAniRollCnt   = xElement.GetIntAttribute_e( "RollCnt" );


	char szKeyName[MAX_PATH] = {0,};
	StringCbPrintf_e( szKeyName, sizeof( szKeyName ), "rolling_sound" );
	m_szRollingSound = xElement.GetStringAttribute( szKeyName );
}

void RisingGashaponWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "LineLeft" )
	{
		SAFEDELETE( m_pLineLeft );
		m_pLineLeft = pImage;
	}
	else if( szType == "LineCenter" )
	{
		SAFEDELETE( m_pLineCenter );
		m_pLineCenter = pImage;
	}
	else if( szType == "LineRight" )
	{
		SAFEDELETE( m_pLineRight );
		m_pLineRight = pImage;

		if( m_pLineRight )
			m_pLineRight->SetReverseFormatting( ioUIRenderImage::RF_HORZ_REVERSE );
	}
	else
	{
		ioWnd::AddRenderImage( szType, pImage );	
	}
}

void RisingGashaponWnd::SendBuy()
{
	if( g_App.IsMouseBusy() )
		return;

	g_App.SetMouseBusy( true );

	SP2Packet kPacket( CTPK_BUY_RISING_GASHAPON );
	kPacket << m_iEtcItemType;
	kPacket << m_bBuyCash;

	TCPNetwork::SendToServer( kPacket );


}


void RisingGashaponWnd::CheckBuy()
{

	// 모두 구매함.. 리셋 해야한다.
	if(m_iBuyCount == m_vGashaponItemList.size())
	{
		// 리셋 메시지박스 출력
		ShowInitWarning();
		return;
	}

	// 장비 제한 체크
	if( g_MyInfo.IsExtraItemFull( true ) )
		return;

	// cash
	if( g_MyInfo.GetEntryType() == ET_TERMINATION || g_MyInfo.IsExpertEntryTemporary() )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return;
	}


	int iPayAmt = 0;
	char szName[MAX_PATH]="";
	ioRisingGashapon *pEtcItem = static_cast< ioRisingGashapon* > ( g_EtcItemMgr.FindEtcItem( m_iEtcItemType ) );
	if( pEtcItem )
	{
		// 몇번째이냐에 따라 가격 달라짐 즉 순번으로 가격 가져오기
		if(m_bBuyCash)
		{
			iPayAmt = pEtcItem->GetCash( m_iBuyCount );
		}
		else
		{
			iPayAmt = pEtcItem->GetPeso( m_iBuyCount );
			
		}
		StringCbCopy( szName, sizeof( szName ), pEtcItem->GetTtitle().c_str() );
	}

	if( iPayAmt <= 0 )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return;
	}
	
	if(m_bBuyCash)
	{
		if( g_MyInfo.GetTotalCash() < iPayAmt )
		{
			if( ioChannelingNodeParent::IsBillingTestID( g_MyInfo.GetPublicID() ) )
			{
				LOG.PrintTimeAndLog( 0, "IsBillingTestID is true : %s", g_MyInfo.GetPublicID().c_str() );
			}
			else
			{
				g_GUIMgr.ShowWnd( BILLING_WANT_WND );
				return;
			}
		}
	}
	else
	{
		if( g_MyInfo.GetMoney() < iPayAmt )
		{
			ShowNeedPeso();
			return;
		}

	}

	// show confirm 
	char szPrice[MAX_PATH]="";
	Help::ConvertNumToStrComma( iPayAmt, szPrice, sizeof( szPrice ) );
	if(m_bBuyCash)
		StringCbCat( szPrice, sizeof( szPrice ), STR(3));
	else
		StringCbCat( szPrice, sizeof( szPrice ), STR(4));


	ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
	kPrinter[0].SetTextStyle( TS_NORMAL );
	kPrinter[0].SetBkColor( 0, 0, 0 );	
	kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(5), szName );

	kPrinter[1].SetTextStyle( TS_NORMAL );
	kPrinter[1].SetBkColor( 0, 0, 0 );	
	kPrinter[1].SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(6), szPrice );

	kPrinter[2].SetTextStyle( TS_NORMAL );
	kPrinter[2].SetBkColor( 0, 0, 0 );	
	kPrinter[2].SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter[2].AddTextPiece( FONT_SIZE_13, STR(7) );

	kPrinter[3].SetTextStyle( TS_NORMAL );
	kPrinter[3].SetBkColor( 0, 0, 0 );	
	kPrinter[3].SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter[3].AddTextPiece( FONT_SIZE_13, "                 " );

	kPrinter[4].SetTextStyle( TS_NORMAL );
	kPrinter[4].SetBkColor( 0, 0, 0 );	
	kPrinter[4].SetTextColor( TCT_DEFAULT_GRAY );
	kPrinter[4].AddTextPiece( FONT_SIZE_13, STR(8) );

	kPrinter[5].SetTextStyle( TS_NORMAL );
	kPrinter[5].SetBkColor( 0, 0, 0 );	
	kPrinter[5].SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter[5].AddTextPiece( FONT_SIZE_13, "                 " );

	NewShopBuyConfirmWnd *pBuyMsgWnd = ( NewShopBuyConfirmWnd* )( g_GUIMgr.FindWnd( NEWSHOP_BUY_CONFIRM_WND ) ); 
	if( pBuyMsgWnd )
		pBuyMsgWnd->SetPrintMessage( this, kPrinter, SUBSCRIPTION_NONE );
}


void RisingGashaponWnd::ShowRisingGashaponWnd( int iEtcItemType, DWORD dwID)
{
	if( !COMPARE( iEtcItemType, ioEtcItem::EIT_ETC_RISING_GASHAPON_001, ioEtcItem::EIT_ETC_RISING_GASHAPON_MAX + 1 ) )
		return;

	ioRisingGashapon *pEtcItem = static_cast< ioRisingGashapon* > ( g_EtcItemMgr.FindEtcItem( iEtcItemType ) );
	if( !pEtcItem )
		return;

	// 캐쉬, 페소구매 
	if(dwID == NewShopBuyWnd::ID_BUY_CASH_BTN)
	{
		m_bBuyCash = true;
	}
	else if(dwID == NewShopBuyWnd::ID_BUY_PESO_BTN)
	{
		m_bBuyCash = false;
	}

	InitAniState();

	m_iEtcItemType   = iEtcItemType;





	if( !pEtcItem->GetTtitle().IsEmpty() )
	{
		StringVector vItemName;
		int width = 80;
		Help::SplitTextWidth( pEtcItem->GetTtitle().c_str(), vItemName, width, TS_NORMAL, FONT_SIZE_11 );
		if(!vItemName.empty())
		{
			vItemName[0] += "..";
			SetTitleText( vItemName[0].c_str() );
		}
	}
	else
	{
		SetTitleText( "" );
	}


	if( !pEtcItem->GetMent1().IsEmpty() )
		m_szMent1 = pEtcItem->GetMent1();

	if( !pEtcItem->GetMent2().IsEmpty() )
		m_szMent2 = pEtcItem->GetMent2();	

	Init();
	ShowWnd();
}

void RisingGashaponWnd::iwm_hide()
{
	if( !m_szRollingSound.IsEmpty() )
		g_SoundMgr.StopSound( m_szRollingSound, 0 );

	m_szMent1.Clear();
	m_szMent2.Clear();

	m_iEtcItemType = 0;

	SendInit();
}

void RisingGashaponWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_EXIT:
		if( cmd == IOBN_BTNUP )
		{
			ShowExitMsg();
		}
		break;
	case ID_BUY:
		if( cmd == IOBN_BTNUP )
		{
			CheckBuy();
		}
		break;
	case ID_GOLD_CHARGE:
		if( cmd == IOBN_BTNUP )
		{
			OnCashCharge();
		}
		break;			
	case ID_ITEM_1:
	case ID_ITEM_2:
	case ID_ITEM_3:
	case ID_ITEM_4:
	case ID_ITEM_5:
	case ID_ITEM_6:
	case ID_ITEM_7:
	case ID_ITEM_8:
	case ID_ITEM_9:
	case ID_ITEM_10:
		break;
	case MESSAGE_BOX:
		{
			if( cmd == IOBN_BTNUP )
			{
				if( param == IDYES )
				{
					if(m_eMsgType == eMSGBOX_TYPE_INITAGAIN)
					{
						SendInit();
						Init();
					}
					else if(m_eMsgType == eMSGBOX_TYPE_EXIT)
					{
						HideWnd();
					}
				}
			}
		}
		break;
	}
}


void RisingGashaponWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() )
		return;

	ProcessAni();
}

void RisingGashaponWnd::InitAniState()
{
	FSMState *pState = new FSMState(AT_NONE);
	if( pState )
		pState->Add( AT_WAIT,  AT_WAIT );
	if( pState )
		pState->Add( AT_END,  AT_END );
	m_AniState.Add( pState );

	pState = new FSMState(AT_WAIT);
	if( pState )
		pState->Add( AT_WAIT, AT_SOUND, m_dwAniWaitDelay );
	m_AniState.Add( pState );

	pState = new FSMState(AT_SOUND);
	if( pState )
		pState->Add( AT_ROLL, AT_ROLL );
	m_AniState.Add( pState );

	pState = new FSMState(AT_ROLL);
	if( pState )
		pState->Add( AT_ROLL, AT_CHANGE, m_dwAniRollDelay );
	m_AniState.Add( pState );	

	pState = new FSMState(AT_CHANGE);
	if( pState )
	{
		pState->Add( AT_ROLL, AT_ROLL );
		pState->Add( AT_END, AT_END );
	}
	m_AniState.Add( pState );	

	pState = new FSMState(AT_END);
	if( pState )
		pState->Add( AT_NONE, AT_NONE );
	m_AniState.Add( pState );	

	m_AniState.SetCurState( AT_NONE );
}

void RisingGashaponWnd::ProcessAni()
{
	m_AniState.UpdateTime();

	if( m_AniState.GetCurState() == AT_SOUND )
	{
		if( !m_szRollingSound.IsEmpty() )
			g_SoundMgr.PlaySound( m_szRollingSound, DSBVOLUME_MAX, PSM_LOOP | PSM_THREAD );

		m_AniState.ChangeState( AT_ROLL );
	}
	else if( m_AniState.GetCurState() == AT_CHANGE )
	{

		static unsigned int iRandIndex = 0;

		// 증가
		++iRandIndex;
		if(iRandIndex >= m_vUnGettedIndexList.size())
		{
			iRandIndex = 0;
		}
		int randValue = m_vUnGettedIndexList[iRandIndex];

		for (int i = 0; i < MAX_ITEM; i++)
		{
			int UIID = i + ID_ITEM_1;
			RisingGashaponItemInfoBtn *pCardBtn = static_cast< RisingGashaponItemInfoBtn * >( FindChildWnd( UIID ) );
			if( !pCardBtn )
				continue;

			if( pCardBtn->IsEmptyCard() )
				continue;

			if( i == randValue ) 
				pCardBtn->SetSelect( true );
			else
				pCardBtn->SetSelect( false );
		}	


		m_dwCurAniRollCnt++;
		if( m_dwCurAniRollCnt >= m_dwAniRollCnt )
			m_AniState.ChangeState( AT_END );
		else
			m_AniState.ChangeState( AT_ROLL );
	}
	else if( m_AniState.GetCurState() == AT_END )
	{
		if( !m_szRollingSound.IsEmpty() )
			g_SoundMgr.StopSound( m_szRollingSound, 0 );


		for (int i = 0; i < MAX_ITEM; i++)
		{
			int UIID = i + ID_ITEM_1;
			RisingGashaponItemInfoBtn *pCardBtn = static_cast< RisingGashaponItemInfoBtn * >( FindChildWnd( UIID ) );
			if( !pCardBtn )
				continue;

			if( pCardBtn->IsEmptyCard() )
				continue;

			pCardBtn->SetSelect( false );
		}	

		auto itGet = m_vGettedIndexList.rbegin();
		int getIndex = C_INVALID_INDEX;
		if(itGet != m_vGettedIndexList.rend())
		{
			getIndex = *itGet;

			DWORD getUIID = getIndex + ID_ITEM_1;
			RisingGashaponItemInfoBtn *pCardBtn = static_cast< RisingGashaponItemInfoBtn * >( FindChildWnd( getUIID ) );
			if( pCardBtn )
			{
				pCardBtn->SetGetCard(true);
				pCardBtn->SetGetEffect();
			}
		}

		// 못얻은 리스트 에서 제거 나중에 랜덤 리스트 생성에 사용
		auto itList = std::find(m_vUnGettedIndexList.begin(), m_vUnGettedIndexList.end(), getIndex);
		if(itList != m_vUnGettedIndexList.end())
		{
			m_vUnGettedIndexList.erase(itList);
		}
		else
		{
			LOG.PrintTimeAndLog(0, "%s, Something wrong!!, %d %d", __FUNCTION__, m_iEtcItemType, getIndex);
		}


		// 가격 갱신
		SetTitleBuyBtn(m_iBuyCount);

		ShowChildWnd( ID_BUY );
		HideChildWnd( ID_WAITING );

		m_AniState.ChangeState( AT_NONE );
	}
}

void RisingGashaponWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	enum 
	{
		X_OFFSET_DESC = 197,
		Y_OFFSET_DESC = 53,
		LINE_GAP      = 18,

		X_OFFSET_SELECT = 197,
		Y_OFFSET_SELECT = 97,

		X_OFFSET_DESC_CARD = 198,
		Y_OFFSET_DESC_CARD = 128,

		X_OFFSET_LEFT = 20,
		Y_OFFSET_LEFT = 134,

		X_OFFSET_RIGHT = 371,
		Y_OFFSET_RIGHT = 134,

		ALL_CARD_WIDTH = 344,

		X_OFFSET_CENTER_LEFT = 27,
		Y_OFFSET_CENTER_LEFT = 134,

		X_OFFSET_CENTER_RIGHT = 371,
		Y_OFFSET_CENTER_RIGHT = 134,
	};

	// text
	ioComplexStringPrinter kPrinter;
	kPrinter.SetTextStyle( TS_NORMAL );
	kPrinter.SetTextColor( TCT_DEFAULT_GRAY );
	kPrinter.SetBkColor( 0, 0, 0 );
	if( m_szMent1.IsEmpty() )
	{
		kPrinter.AddTextPiece( FONT_SIZE_13, STR(1) );
	}
	else
	{
		kPrinter.AddTextPiece( FONT_SIZE_13, m_szMent1.c_str() );
	}

	kPrinter.PrintFullText( iXPos + X_OFFSET_DESC, iYPos + Y_OFFSET_DESC, TAT_CENTER );
	kPrinter.ClearList();

	if( m_szMent2.IsEmpty() )
	{
		kPrinter.SetTextColor( TCT_DEFAULT_GRAY );
		kPrinter.AddTextPiece( FONT_SIZE_13, STR(2));
	}
	else
	{
		kPrinter.SetTextColor( TCT_DEFAULT_GRAY );
		kPrinter.AddTextPiece( FONT_SIZE_13, m_szMent2.c_str() );
	}

	kPrinter.PrintFullText( iXPos + X_OFFSET_DESC, iYPos + Y_OFFSET_DESC + LINE_GAP, TAT_CENTER );
	kPrinter.ClearList();
	

	char szSelect[MAX_PATH]="";
	SafeSprintf( szSelect, sizeof( szSelect ), STR(3), (int)m_vUnGettedIndexList.size() );

	kPrinter.SetTextColor( TCT_DEFAULT_RED );
	kPrinter.AddTextPiece( FONT_SIZE_15, szSelect );
	kPrinter.PrintFullText( iXPos + X_OFFSET_SELECT, iYPos + Y_OFFSET_SELECT, TAT_CENTER );
	kPrinter.ClearList();

	kPrinter.SetTextColor( TCT_DEFAULT_LIGHTGRAY );	
	kPrinter.AddTextPiece( FONT_SIZE_12, m_sDescCard.c_str() );
	kPrinter.PrintFullText( iXPos + X_OFFSET_DESC_CARD, iYPos + Y_OFFSET_DESC_CARD, TAT_CENTER );
	kPrinter.ClearList();

	// 테두리
	if( m_pLineLeft )
		m_pLineLeft->Render( iXPos + X_OFFSET_LEFT, iYPos + Y_OFFSET_LEFT );

	if( m_pLineCenter )
	{
		// 자동 계산
		
		// left center // /2 를 * FLOAT05로 변경
		// ALL_CARD_WIDTH 를 FLOAT344 로 변경
		int iWidth = ( FLOAT344 - ( g_FontMgr.GetTextWidth( m_sDescCard.c_str(), TS_NORMAL, FONT_SIZE_12 ) + FLOAT10 ) ) * FLOAT05;
		m_pLineCenter->SetSize( iWidth, m_pLineCenter->GetHeight() );
		m_pLineCenter->SetHorzFormatting( ioUIRenderImage::HF_STRETCH );
		m_pLineCenter->Render( iXPos + X_OFFSET_CENTER_LEFT, iYPos + Y_OFFSET_CENTER_LEFT );

		// right center
		m_pLineCenter->Render( iXPos + X_OFFSET_CENTER_RIGHT - iWidth, iYPos + Y_OFFSET_CENTER_RIGHT );
	}

	if( m_pLineRight )
		m_pLineRight->Render( iXPos + X_OFFSET_RIGHT, iYPos + Y_OFFSET_RIGHT );
}


void RisingGashaponWnd::SetTitleBuyBtn( int iCashArray )
{
	int iPayAmt = 0;

	ioRisingGashapon * pRising = static_cast<ioRisingGashapon *>(g_EtcItemMgr.FindEtcItem( m_iEtcItemType ));
	if( pRising )
	{
		if(m_bBuyCash)
		{
			iPayAmt = pRising->GetCash( iCashArray );
		}
		else
		{
			iPayAmt = pRising->GetPeso( iCashArray );
		}
	}

	char szPrice[MAX_PATH]="";
	Help::ConvertNumToStrComma( iPayAmt, szPrice, sizeof( szPrice ) );

	char szTitle[MAX_PATH]="";
	if(m_bBuyCash)
	{
		const ioHashString & strBuy = pRising->GetBuyCashText();
		if(strBuy.IsEmpty())
			SafeSprintf( szTitle, sizeof( szTitle ), STR(1), szPrice );
		else
			SafeSprintf( szTitle, sizeof( szTitle ), strBuy.c_str(), szPrice );

	}
	else
	{
		const ioHashString & strBuy = pRising->GetBuyPesoText();
		if(strBuy.IsEmpty())
			SafeSprintf( szTitle, sizeof( szTitle ), STR(2), szPrice );
		else
			SafeSprintf( szTitle, sizeof( szTitle ), strBuy.c_str(), szPrice );
	}
	
	ioWnd *pWnd = FindChildWnd( ID_BUY );
	if( pWnd )
	{
		pWnd->SetTitleText( szTitle );
	}
}

void RisingGashaponWnd::SetResult( const GashaponPresent& rkResultPresent )
{

	m_dwCurAniRollCnt = 0;
	m_iBuyCount++;
	// 받은아이템 확인
	bool bFind = false;
	unsigned int iGettedItemIndex = C_INVALID_INDEX;
	for( unsigned int i = 0; i < m_vGashaponItemList.size(); ++i )
	{
		const GashaponPresent& rkRandom = m_vGashaponItemList[i];
		if( rkRandom == rkResultPresent )
		{
			iGettedItemIndex = i;
			m_vGettedIndexList.push_back(iGettedItemIndex);
			bFind = true;
			break;
		}
	}

	if(!bFind)
	{
		ShowError(eERROR_NUM_TYPE_PRESENT_CANT_FIND);
	}

	// 랜덤 리스트 생성
	std::random_shuffle(m_vUnGettedIndexList.begin(), m_vUnGettedIndexList.end());


	if(m_vUnGettedIndexList.size() < 2 || !bFind)
	{
		m_AniState.ChangeState( AT_END );

	}
	else
	{
		HideChildWnd( ID_BUY );
		ShowChildWnd( ID_WAITING );

		m_AniState.ChangeState( AT_WAIT );
	}

}


void RisingGashaponWnd::Init()
{
	// 구매 횟수 초기화
	m_iBuyCount = 0;
	m_eMsgType = eMSGBOX_TYPE_NONE;

	m_dwCurAniRollCnt  = 0;

	m_vGashaponItemList = g_GashaponMgr.FindGashaponPresentList( m_iEtcItemType );

	m_vGettedIndexList.clear();
	m_vUnGettedIndexList.clear();
	for(unsigned int i = 0; i < m_vGashaponItemList.size(); ++i)
	{
		m_vUnGettedIndexList.push_back(i);
	}

	ioRisingGashapon * pRising = static_cast<ioRisingGashapon *>(g_EtcItemMgr.FindEtcItem( m_iEtcItemType ));
	if(!pRising)
	{
		LOG.PrintTimeAndLog(0, "%s, Can't Find EtcItem, %d", __FUNCTION__, m_iEtcItemType);
		return;
	}
	
	ShowChildWnd( ID_BUY );
	HideChildWnd( ID_WAITING );

	int gashaponSize = m_vGashaponItemList.size();
	ioHashString strName;
	for (int i = 0; i < MAX_ITEM ; i++)
	{
		int idNum = ID_ITEM_1 + i;
		RisingGashaponItemInfoBtn *pItemWnd = static_cast< RisingGashaponItemInfoBtn*>( FindChildWnd( idNum ) );
		if( pItemWnd )
		{
			pItemWnd->SetEmptyCard();

			if(COMPARE(i, 0, gashaponSize))
			{
				if(pRising)
					strName = pRising->GetItemName(i);
				else
					strName = STR(1);

				pItemWnd->SetPresent(m_vGashaponItemList[i], strName);
			}
		}
	}
	
	SetTitleBuyBtn( 0 );
	m_AniState.SetCurState( AT_NONE );
}

void RisingGashaponWnd::ShowNeedPeso()
{
	NewShopNeedPesoWnd *pNeedPesoWnd = static_cast<NewShopNeedPesoWnd*> ( g_GUIMgr.FindWnd( NEW_SHOP_NEED_PESO_WND ) );
	if( !pNeedPesoWnd )
		return;

	ioEtcItem *pEtcItem = g_EtcItemMgr.FindEtcItem( m_iEtcItemType );
	if( pEtcItem )
	{
		NewShopNeedPesoWnd::NeedPesoInfo kInfo;
		kInfo.m_sName = pEtcItem->GetName();
		kInfo.m_sIcon = pEtcItem->GetIconName();

		int iPeso = pEtcItem->GetPeso( m_iBuyCount );
		char szPrice[MAX_PATH]="";
		Help::ConvertNumToStrComma( iPeso, szPrice, sizeof( szPrice ) );
		StringCbCat( szPrice, sizeof( szPrice ), STR(1) );
		kInfo.m_sPrice = szPrice;

		ZeroMemory( szPrice, sizeof( szPrice ) );
		char szTemp[MAX_PATH]="";
		int iNeedPeso = iPeso - g_MyInfo.GetMoney();
		Help::ConvertNumToStrComma( iNeedPeso, szTemp, sizeof( szTemp ) );
		SafeSprintf( szPrice, sizeof( szPrice ), STR(2), szTemp );
		kInfo.m_sMinusPrice = szPrice;
		if( pNeedPesoWnd->SetInfo( kInfo ) )
			pNeedPesoWnd->ShowWnd();
	}
}

void RisingGashaponWnd::ShowExitMsg()
{
	m_eMsgType = eMSGBOX_TYPE_EXIT;
	g_GUIMgr.SetMsgBox( MB_YESNO, this, STR(1) );

}

void RisingGashaponWnd::ShowInitWarning()
{
	m_eMsgType = eMSGBOX_TYPE_INITAGAIN;
	g_GUIMgr.SetMsgBox( MB_YESNO, this, STR(1));
}

void RisingGashaponWnd::OnCashCharge()
{
	ioPlayStage *pStage = (ioPlayStage *)g_App.GetGameStage();

	if( g_App.GetConnectedMoveMode() == ioApplication::CMM_LOBBY )
	{
		if( pStage && pStage->GetModeType() != MT_MYROOM )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
			return;
		}
	}
	else
	{
		if( pStage && pStage->GetModeType() != MT_TRAINING && pStage->GetModeType() != MT_HEADQUARTERS )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
			return;
		}
	}

	ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
	if( pLocal )
	{
		pLocal->SetGoldChargeBtn( true );
		pLocal->OpenFillCashPage();
	}
}

void RisingGashaponWnd::SendInit()
{
	SP2Packet kPacket( CTPK_INIT_RISING_GASHAPON );

	TCPNetwork::SendToServer( kPacket );
}

void RisingGashaponWnd::OnRenderAfterChild()
{
	// 타이틀 캐쉬출력

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	enum 
	{
		X_OFFSET = 256,
		Y_OFFSET = 14,
	};

	char szCash[MAX_PATH] = {0,};
	Help::ConvertNumToStrComma( g_MyInfo.GetTotalCash(), szCash, sizeof( szCash ) );


	ioComplexStringPrinter kPrinter;
	kPrinter.SetTextStyle( TS_OUTLINE_FULL_2X );
	kPrinter.SetBkColor( 12, 66, 111 );

	kPrinter.SetTextColor( 255, 188, 26 );		
	//kPrinter.SetTextColor( 141, 87, 4 );		
	kPrinter.AddTextPiece( FONT_SIZE_12, STR(1) );
	kPrinter.AddTextPiece( FONT_SIZE_12, szCash );

	kPrinter.PrintFullText( iXPos + X_OFFSET, iYPos + Y_OFFSET, TAT_RIGHT );
	kPrinter.ClearList();

}

bool RisingGashaponWnd::iwm_esc()
{
	ShowExitMsg();
	return true;
}

void RisingGashaponWnd::ShowError( ErrorNumType eErrorType )
{
	switch (eErrorType)
	{
	case eERROR_NUM_TYPE_PRESENT_CANT_FIND:
		g_GUIMgr.SetMsgBox( MB_OK, this, STR(2) );
		LOG.PrintTimeAndLog(0, "Can't find Present, %d", m_iEtcItemType);
		break;
	default:
		g_GUIMgr.SetMsgBox( MB_OK, this, STR(1) );
		LOG.PrintTimeAndLog(0, "%s Unknown Error, %d", __FUNCTION__, m_iEtcItemType);
		break;
	}
}

