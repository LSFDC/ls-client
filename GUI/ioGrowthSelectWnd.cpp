
#include "StdAfx.h"

#include "../io3DEngine/ioWnd.h"
#include "../io3DEngine/ioFontManager.h"
#include "../io3DEngine/ioStringConverter.h"
#include "../io3DEngine/ioUIRenderImage.h"
#include "../io3DEngine/ioUIRenderFrame.h"
#include "../io3DEngine/ioUIImageSetManager.h"
#include "../io3DEngine/ioUITitle.h"
#include "../io3DEngine/ioSoundManager.h"

#include "../io3DEngine/ioXMLDocument.h"
#include "../io3DEngine/ioXMLElement.h"
#include "../io3DEngine/iostringmanager.h"

#include "../TextColorTable.h"
#include "../NetworkWrappingFunc.h"

#include "../Setting.h"
#include "../WndID.h"
#include "../ioApplication.h"

#include "../ioMyInfo.h"
#include "../EtcHelpFunc.h"
#include "../ioGrowthLevel.h"
#include "../ioGrowthUpInfo.h"
#include "../ioGrowthInfoManager.h"
#include "../ioMedalItemInfoManager.h"
#include "../ioSoldierAwakeManager.h"
#include "../ioRSoldierInfoManager.h"

#include "ioSP2GUIManager.h"
#include "SkillListWnd.h"

#include "MyInventoryWnd.h"
#include "ioGrowthSelectWnd.h"

#include <strsafe.h>
#include "../io3DEngine/Safesprintf.h"
#include "../Local/ioLocalManager.h"

//////////////////////////////////////////////////////////////////////////

ioGrowthSubWnd::ioGrowthSubWnd()
{
	m_pClockGreenImg = NULL;
	m_pClockRedImg = NULL;
	m_pPointImg = NULL;
	m_pArrowImg = NULL;

	m_bTimeGrowth = false;

	m_iClassType = 0;

	m_iTimeSlot = 0;
	m_iGrowthInfoNum = 0;

	m_iCurLevel = 0;
	m_iClassLevel = 0;
	m_iReinforce = 0;

	m_iNeedPoint = 0;
	m_iNeedPeso = 0;

	m_SubType = TIME_GROWTH_TYPE;

	ClearList();
}

ioGrowthSubWnd::~ioGrowthSubWnd()
{
	SAFEDELETE(m_pClockGreenImg);
	SAFEDELETE(m_pClockRedImg);
	SAFEDELETE(m_pPointImg);
	SAFEDELETE(m_pArrowImg);

	ClearList();
}

void ioGrowthSubWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "Clock_Green_Img" )
	{
		SAFEDELETE( m_pClockGreenImg );
		m_pClockGreenImg = pImage;
	}
	else if( szType == "Clock_Red_Img" )
	{
		SAFEDELETE( m_pClockRedImg );
		m_pClockRedImg = pImage;
	}
	else if( szType == "P_Img" )
	{
		SAFEDELETE( m_pPointImg );
		m_pPointImg = pImage;
	}
	else if( szType == "Arrow_Img" )
	{
		SAFEDELETE( m_pArrowImg );
		m_pArrowImg = pImage;
	}
	else
		ioButton::AddRenderImage( szType, pImage );
}

void ioGrowthSubWnd::ClearList()
{
	for( int i=0; i < SUB_DESC_LINE; ++i )
	{
		m_GrowthDesc[i].ClearList();
	}
}

void ioGrowthSubWnd::SetInfo( int iClassType, int iSlot, int iCurLevel, int iGrowthInfoNum, GrowthSubType eType )
{
	if( !IsShow() )
		ShowWnd();

	m_SubType = eType;

	m_bTimeGrowth = false;
	m_iClassType = iClassType;
	m_iTimeSlot = iSlot;
	m_iCurLevel = iCurLevel;
	m_iGrowthInfoNum = iGrowthInfoNum;

	bool bChar = false;
	if( COMPARE( iSlot, TIG_ATTACK, TIG_DROP+1 ) )
		bChar = true;

	HideChildWnd(ID_TIME_START);
	HideChildWnd(ID_TIME_CANCEL);
	HideChildWnd(ID_PESO);

	int iNextLevel = m_iCurLevel + 1;
	m_iNeedPoint = g_GrowthInfoMgr.GetGrowthUpNeedPoint( bChar );
	m_iNeedPeso = g_GrowthInfoMgr.GetGrowthUpNeedPeso( bChar, iNextLevel );

	// Desc
	m_iClassLevel = g_MyInfo.GetClassLevel( iClassType, true );
	m_iClassLevel = min( m_iClassLevel, g_GrowthInfoMgr.GetMaxLevel() );

	ClearList();
	if( m_SubType == TIME_GROWTH_TYPE )
	{
		// Time Growth
		if( !HasWndStyle(IWS_INACTIVE) )
		{
			UpdateTimeGrowthInfo();
		}
	}
	else
	{
		// Peso Growth
		m_GrowthDesc[0].SetTextStyle( TS_NORMAL );
		m_GrowthDesc[0].SetBkColor( 0, 0, 0 );
		m_GrowthDesc[0].SetTextColor( TCT_DEFAULT_GRAY );
		m_GrowthDesc[0].AddTextPiece( FONT_SIZE_11, STR(1) );

		m_GrowthDesc[1].SetTextStyle( TS_NORMAL );
		m_GrowthDesc[1].SetBkColor( 0, 0, 0 );
		m_GrowthDesc[1].SetTextColor( TCT_DEFAULT_GRAY );
		m_GrowthDesc[1].AddTextPiece( FONT_SIZE_11, STR(2) );

		m_GrowthDesc[2].SetTextStyle( TS_NORMAL );
		m_GrowthDesc[2].SetBkColor( 0, 0, 0 );
		m_GrowthDesc[2].SetTextColor( TCT_DEFAULT_GRAY );
		m_GrowthDesc[2].AddTextPiece( FONT_SIZE_11, STR(3) );

		m_szRemainTimeText.Clear();

		ShowChildWnd(ID_PESO);
	}
}

void ioGrowthSubWnd::UpdateTimeGrowthInfo()
{
	char szSelect[MAX_PATH] = "";
	ZeroMemory( szSelect, sizeof( szSelect ) );

	bool bChar = false;
	if( COMPARE( m_iTimeSlot, TIG_ATTACK, TIG_DROP+1 ) )
		bChar = true;

	int iCurMax = g_MyInfo.GetClassLevel( m_iClassType, true );
	iCurMax = min( iCurMax, g_GrowthInfoMgr.GetMaxLevel() );
	int iNextLevel = m_iCurLevel + 1;

	// Time Growth
	int iGapSec = 0;

	m_bTimeGrowth = false;

	DWORD dwNeedTime = g_GrowthInfoMgr.GetTimeGrowthNeedTime( bChar, iNextLevel ) * 60;
	DWORD dwConfirmTime = g_GrowthInfoMgr.GetTimeGrowthConfirmTime( bChar, iNextLevel ) * 60;

	iGapSec = dwNeedTime;

	ioGrowthLevel *pLevel = g_MyInfo.GetGrowthLevel();
	if( pLevel )
	{
		TimeGrowthInfo *pInfo = pLevel->FindTimeGrowthInfo( m_iClassType );
		if( pInfo && m_iTimeSlot > 0 && pInfo->m_iTimeSlot == m_iTimeSlot )
		{
			m_bTimeGrowth = true;
			iGapSec = pLevel->GetTimeGrowthRemainTime( m_iClassType );
		}
	}

	m_szRemainTimeText.Clear();
	enum { DAY_SEC = 86400, HOUR_SEC = 3600, MINUTE_SEC = 60, };

	// Need Time
	int iNeedHour = dwNeedTime / HOUR_SEC;
	int iNeedMin = (dwNeedTime-(iNeedHour*HOUR_SEC)) / MINUTE_SEC;
	int iNeedSec = dwNeedTime - (iNeedHour*HOUR_SEC) - (iNeedMin*MINUTE_SEC);
	int iNeedTotalMin = dwNeedTime / MINUTE_SEC;

	// Confirm Time
	int iConfirmHour = dwConfirmTime / HOUR_SEC;
	int iConfirmMin = (dwConfirmTime-(iConfirmHour*HOUR_SEC)) / MINUTE_SEC;
	int iConfirmTotalMin = dwConfirmTime / MINUTE_SEC;

	// Gap Time
	int iHour   = iGapSec / HOUR_SEC;
	int iMinute = ( iGapSec-(iHour*HOUR_SEC) ) / MINUTE_SEC;
	int iSec = iGapSec - (iHour*HOUR_SEC) - (iMinute*MINUTE_SEC);
	int iTotalMin = iGapSec / MINUTE_SEC;

	iHour = max( 0, iHour );
	iMinute = max( 0, iMinute );
	iSec = max( 0, iSec );
	iTotalMin = max( 0, iTotalMin );

	if( m_bTimeGrowth )
	{
		HideChildWnd(ID_TIME_START);
		ShowChildWnd(ID_TIME_CANCEL);

		if( iGapSec > MINUTE_SEC )
			SafeSprintf( szSelect, sizeof(szSelect), STR(1), iHour, iMinute, iSec );
		else
			SafeSprintf( szSelect, sizeof(szSelect), STR(2) );

		m_szRemainTimeText = szSelect;
	}
	else
	{
		ShowChildWnd(ID_TIME_START);
		HideChildWnd(ID_TIME_CANCEL);

		SafeSprintf( szSelect, sizeof(szSelect), STR(3), iNeedHour, iNeedMin, iNeedSec );
		m_szRemainTimeText = szSelect;
	}

	ClearList();

	m_GrowthDesc[0].SetTextStyle( TS_NORMAL );
	m_GrowthDesc[0].SetBkColor( 0, 0, 0 );
	m_GrowthDesc[0].SetTextColor( TCT_DEFAULT_GRAY );
	m_GrowthDesc[0].AddTextPiece( FONT_SIZE_11, STR(4), iNeedTotalMin );

	m_GrowthDesc[1].SetTextStyle( TS_NORMAL );
	m_GrowthDesc[1].SetBkColor( 0, 0, 0 );
	m_GrowthDesc[1].SetTextColor( TCT_DEFAULT_GRAY );
	m_GrowthDesc[1].AddTextPiece( FONT_SIZE_11, STR(5), iConfirmTotalMin );

	m_GrowthDesc[2].SetTextStyle( TS_NORMAL );
	m_GrowthDesc[2].SetBkColor( 0, 0, 0 );
	m_GrowthDesc[2].SetTextColor( TCT_DEFAULT_GRAY );
	m_GrowthDesc[2].AddTextPiece( FONT_SIZE_11, STR(6) );
}

void ioGrowthSubWnd::OnProcess( float fTimePerSec )
{
	ioButton::OnProcess( fTimePerSec );

	if( !IsShow() ) return;

	if( m_SubType == TIME_GROWTH_TYPE )
	{
		if( !HasWndStyle(IWS_INACTIVE) )
		{
			UpdateTimeGrowthInfo();
		}
	}
}

void ioGrowthSubWnd::ActiveRender()
{
	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	enum
	{
		X_OFFSET	= 72,

		Y_OFFSET1	= 14,
		Y_OFFSET2	= 37,
		Y_OFFSET3	= 64,

		LINE_GAP = 16,
	};

	char szBuf[MAX_PATH], szNumComma[MAX_PATH];
	ioHashString szNeedPoint, szNeedPeso;

	wsprintf_e( szBuf, "%d", m_iNeedPoint );
	szNeedPoint = szBuf;

	Help::ConvertNumToStrComma( m_iNeedPeso, szNumComma, sizeof(szBuf) );
	SafeSprintf( szBuf, sizeof( szBuf ), STR(1), szNumComma );
	szNeedPeso = szBuf;

	if( m_SubType == TIME_GROWTH_TYPE )
	{
		g_FontMgr.SetAlignType( TAT_CENTER );
		g_FontMgr.SetTextStyle( TS_NORMAL );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
		g_FontMgr.PrintText( iXPos+X_OFFSET, iYPos+Y_OFFSET1, FONT_SIZE_17, STR(2) );

		float fNumSize = g_FontMgr.GetTextWidth( szNeedPoint.c_str(), TS_NORMAL, FONT_SIZE_13 );
		float fTextSize = g_FontMgr.GetTextWidth( STR(3), TS_NORMAL, FONT_SIZE_13 );

		float fTotalSize = fNumSize + fTextSize + 12;
		// /2.0f 를 * FLOAT05로 변경
		float fHalfSize = fTotalSize * FLOAT05;

		float fPoint = iXPos + X_OFFSET - fHalfSize;
		float fImg =  fPoint + fNumSize;
		float fText = fImg + 12;

		g_FontMgr.SetAlignType( TAT_LEFT );
		g_FontMgr.SetTextStyle( TS_NORMAL );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextColor( TCT_DEFAULT_RED );
		g_FontMgr.PrintText( fPoint, iYPos+Y_OFFSET2, FONT_SIZE_13, STR(4), szNeedPoint.c_str() );

		if( m_pPointImg )
		{
			m_pPointImg->Render( fImg, iYPos+Y_OFFSET2 );
		}

		g_FontMgr.SetAlignType( TAT_LEFT );
		g_FontMgr.SetTextStyle( TS_NORMAL );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
		g_FontMgr.PrintText( fText, iYPos+Y_OFFSET2, FONT_SIZE_13, STR(5) );
	}
	else
	{
		g_FontMgr.SetAlignType( TAT_CENTER );
		g_FontMgr.SetTextStyle( TS_NORMAL );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
		g_FontMgr.PrintText( iXPos+X_OFFSET, iYPos+Y_OFFSET1, FONT_SIZE_17, STR(6) );

		float fNumSize = g_FontMgr.GetTextWidth( szNeedPoint.c_str(), TS_NORMAL, FONT_SIZE_13 );
		float fTextSize = g_FontMgr.GetTextWidth( szNeedPeso.c_str(), TS_NORMAL, FONT_SIZE_13 );

		float fTotalSize = fNumSize + fTextSize + 12;
		// /2.0f 를 * FLOAT05로 변경
		float fHalfSize = fTotalSize * FLOAT05;

		float fPoint = iXPos + X_OFFSET - fHalfSize;
		float fImg =  fPoint + fNumSize;
		float fText = fImg + 12;

		g_FontMgr.SetAlignType( TAT_LEFT );
		g_FontMgr.SetTextStyle( TS_NORMAL );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextColor( TCT_DEFAULT_RED );
		g_FontMgr.PrintText( fPoint, iYPos+Y_OFFSET2, FONT_SIZE_13, STR(7), szNeedPoint.c_str() );

		if( m_pPointImg )
		{
			m_pPointImg->Render( fImg, iYPos+Y_OFFSET2 );
		}

		g_FontMgr.SetAlignType( TAT_LEFT );
		g_FontMgr.SetTextStyle( TS_NORMAL );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextColor( TCT_DEFAULT_BLUE );
		g_FontMgr.PrintText( fText, iYPos+Y_OFFSET2, FONT_SIZE_13, STR(8), szNeedPeso.c_str() );
	}

	for( int i=0; i < SUB_DESC_LINE; ++i )
	{
		if( m_GrowthDesc[i].IsEmpty() )
			break;

		m_GrowthDesc[i].PrintFullText( iXPos+X_OFFSET, iYPos+Y_OFFSET3+(LINE_GAP*i), TAT_CENTER );
	}
}

void ioGrowthSubWnd::InActiveRender()
{
	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();


	enum
	{
		X_OFFSET1	= 72,
		Y_OFFSET1	= 14,

		X_OFFSET2	= 71,
		Y_OFFSET2	= 51,

		X_OFFSET3	= 72,
		Y_OFFSET3	= 133,

		LINE_GAP = 16,
	};


	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
	g_FontMgr.PrintText( iXPos+X_OFFSET1, iYPos+Y_OFFSET1, FONT_SIZE_17, STR(1) );

	g_FontMgr.PrintText( iXPos+X_OFFSET2, iYPos+Y_OFFSET2, FONT_SIZE_12, STR(2) );


	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );

	g_FontMgr.PrintText( iXPos+X_OFFSET2, iYPos+Y_OFFSET2+LINE_GAP, FONT_SIZE_12, STR(3), (g_GrowthInfoMgr.GetTimeGrowthLimitLevel()-1) );
	g_FontMgr.PrintText( iXPos+X_OFFSET2, iYPos+Y_OFFSET2+LINE_GAP*2, FONT_SIZE_12, STR(4) );
	g_FontMgr.PrintText( iXPos+X_OFFSET2, iYPos+Y_OFFSET2+LINE_GAP*3, FONT_SIZE_12, STR(5) );


	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );

	g_FontMgr.PrintText( iXPos+X_OFFSET3, iYPos+Y_OFFSET3, FONT_SIZE_12, STR(6) );
	g_FontMgr.PrintText( iXPos+X_OFFSET3, iYPos+Y_OFFSET3+LINE_GAP, FONT_SIZE_12, STR(7) );
}

void ioGrowthSubWnd::OnRender()
{
	ioButton::OnRender();

	if( !HasWndStyle(IWS_INACTIVE) )
		ActiveRender();
	else
		InActiveRender();

}

void ioGrowthSubWnd::OnRenderAfterChild()
{
	if( HasWndStyle(IWS_INACTIVE) )
		return;

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	if( m_SubType == TIME_GROWTH_TYPE )
	{
		int iNewXPos = iXPos + 7 + 66;
		int iNewYPos = iYPos + 122 + 11;

		if( m_bTimeGrowth )
		{
			float fTextSize = g_FontMgr.GetTextWidth( STR(1), TS_OUTLINE_FULL_3X, FONT_SIZE_13 );
			float fTotalSize = fTextSize + 15 -1;
			// /2.0f 를 * FLOAT05로 변경
			float fHalfSize = fTotalSize * FLOAT05;

			float fImg = iNewXPos - fHalfSize;
			float fText = fImg + 15 - 1;

			if( m_pClockRedImg )
				m_pClockRedImg->Render( fImg, iNewYPos+1 );

			g_FontMgr.SetAlignType( TAT_LEFT );
			g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_3X );
			g_FontMgr.SetBkColor( TCT_DEFAULT_RED );
			g_FontMgr.SetTextColor( TCT_DEFAULT_WHITE );
			g_FontMgr.PrintText( fText, iNewYPos, FONT_SIZE_13, STR(2) );


			g_FontMgr.SetAlignType( TAT_CENTER );
			g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_3X );
			g_FontMgr.SetBkColor( TCT_DEFAULT_RED );
			g_FontMgr.SetTextColor( TCT_DEFAULT_WHITE );
			g_FontMgr.PrintText( iNewXPos, iNewYPos+19, FONT_SIZE_13, STR(3), m_szRemainTimeText.c_str() );
		}
		else
		{
			float fTextSize = g_FontMgr.GetTextWidth( STR(4), TS_OUTLINE_FULL_3X, FONT_SIZE_13 );
			float fTotalSize = fTextSize + 15 -1;
			// /2.0f 를 * FLOAT05로 변경
			float fHalfSize = fTotalSize * FLOAT05;

			float fImg = iNewXPos - fHalfSize;
			float fText = fImg + 15 - 1;

			if( m_pClockGreenImg )
				m_pClockGreenImg->Render( fImg, iNewYPos+1 );

			g_FontMgr.SetAlignType( TAT_LEFT );
			g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_3X );
			g_FontMgr.SetBkColor( TCT_DEFAULT_GREEN );
			g_FontMgr.SetTextColor( TCT_DEFAULT_WHITE );
			g_FontMgr.PrintText( fText, iNewYPos, FONT_SIZE_13, STR(5) );


			g_FontMgr.SetAlignType( TAT_CENTER );
			g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_3X );
			g_FontMgr.SetBkColor( TCT_DEFAULT_GREEN );
			g_FontMgr.SetTextColor( TCT_DEFAULT_WHITE );
			g_FontMgr.PrintText( iNewXPos, iNewYPos+19, FONT_SIZE_13, STR(6), m_szRemainTimeText.c_str() );
		}
	}
	else
	{
		int iNewXPos = iXPos + 7 + 66;
		int iNewYPos = iYPos + 122 + 21;

		float fTextSize = g_FontMgr.GetTextWidth( STR(7), TS_OUTLINE_FULL_3X, FONT_SIZE_13 );
		float fTotalSize = fTextSize + 13 - 1;
		// /2.0f 를 * FLOAT05로 변경
		float fHalfSize = fTotalSize * FLOAT05;

		float fImg = iNewXPos - fHalfSize;
		float fText = fImg + 13 - 1;

		if( m_pArrowImg )
			m_pArrowImg->Render( fImg, iNewYPos+2 );

		g_FontMgr.SetAlignType( TAT_LEFT );
		g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_3X );
		g_FontMgr.SetBkColor( TCT_DEFAULT_GREEN );
		g_FontMgr.SetTextColor( TCT_DEFAULT_WHITE );
		g_FontMgr.PrintText( fText, iNewYPos, FONT_SIZE_13, STR(8) );
	}
}

void ioGrowthSubWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();

	switch( dwID )
	{
	case ID_TIME_START:
	case ID_TIME_CANCEL:
	case ID_PESO:
		if( cmd == IOBN_BTNUP )
		{
			if( GetParent() )
				GetParent()->iwm_command( this, cmd, dwID );
		}
	}
}

//////////////////////////////////////////////////////////////////////////
ioGrowthSelectWnd::ioGrowthSelectWnd()
{
	m_pGaugeBigBack = NULL;
	m_pGaugeBack = NULL;
	m_pGaugeLevel = NULL;
	m_pGaugeReinforce = NULL;
	m_pTriangle = NULL;
	m_pPointImg = NULL;

	m_pPrevWnd = NULL;

	m_iClassType = 0;
	m_iTimeSlot = 0;
	m_iCurLevel = 0;
	m_iGrowthInfoNum = 0;

	m_dwCurSelectBtn = 0;
	m_bBaseGrowthType = false;
}

ioGrowthSelectWnd::~ioGrowthSelectWnd()
{
	SAFEDELETE(m_pGaugeBigBack);
	SAFEDELETE(m_pGaugeBack);
	SAFEDELETE(m_pGaugeLevel);
	SAFEDELETE(m_pGaugeReinforce);
	SAFEDELETE(m_pTriangle);
	SAFEDELETE(m_pPointImg);

	ClearList();
}

void ioGrowthSelectWnd::iwm_show()
{
	if( m_iCurLevel+1 >= g_GrowthInfoMgr.GetTimeGrowthLimitLevel() )
		SetChildInActive( ID_TIME_GROWTH_WND );
	else
		SetChildActive( ID_TIME_GROWTH_WND );

	ShowChildWnd( ID_TIME_GROWTH_WND );
	ShowChildWnd( ID_PESO_GROWTH_WND );

	m_dwCurSelectBtn = 0;
}

void ioGrowthSelectWnd::iwm_hide()
{
	HideChildWnd( ID_TIME_GROWTH_WND );
	HideChildWnd( ID_PESO_GROWTH_WND );
}

void ioGrowthSelectWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "Gauge_Back" )
	{
		SAFEDELETE( m_pGaugeBack );
		m_pGaugeBack = pImage;
	}
	else if( szType == "Gauge_BigBack" )
	{
		SAFEDELETE( m_pGaugeBigBack );
		m_pGaugeBigBack = pImage;
	}
	else if( szType == "Gauge_Level" )
	{
		SAFEDELETE( m_pGaugeLevel );
		m_pGaugeLevel = pImage;
	}
	else if( szType == "Gauge_Reinforce" )
	{
		SAFEDELETE( m_pGaugeReinforce );
		m_pGaugeReinforce = pImage;
	}
	else if( szType == "Gauge_Triangle" )
	{
		SAFEDELETE( m_pTriangle );
		m_pTriangle = pImage;
	}
	else if( szType == "P_Img" )
	{
		SAFEDELETE( m_pPointImg );
		m_pPointImg = pImage;
	}
	else
		ioWnd::AddRenderImage( szType, pImage );
}

void ioGrowthSelectWnd::ClearList()
{
	for( int i=0; i < MAIN_DESC_LINE; ++i )
	{
		m_DescList[i].ClearList();
	}
}

void ioGrowthSelectWnd::SetGrowthSelectInfo( const ioHashString &szIcon,
	const ioHashString &szSlotName,
	const ioHashString &szGrowthName,
	int iClassType,
	int iSlot,
	int iCurLevel,
	int iGrowthInfoNum,
	ioWnd *pPreWnd )
{
	m_pPrevWnd = pPreWnd;

	m_iClassType = iClassType;
	m_iTimeSlot = iSlot;
	m_iCurLevel = iCurLevel;
	m_iGrowthInfoNum = iGrowthInfoNum;

	m_szSlotName = szSlotName;
	m_szGrowthName = szGrowthName;

	m_bBaseGrowthType = false;

	if( !IsShow() )
		ShowWnd();

	UpdateInfo();

	// Title
	char szTitle[MAX_PATH] = "";
	ioHashString szClassName = g_MyInfo.GetClassName( m_iClassType, g_MyInfo.GetCharPowerUpGradeByClassType( m_iClassType ) );
	SafeSprintf( szTitle, sizeof( szTitle ), STR(1), szClassName.c_str(), szSlotName.c_str() );

	SetTitleText( szTitle );

	// icon
	InventoryIconBtn *pBtn = dynamic_cast< InventoryIconBtn* > ( FindChildWnd( ID_ICON_BTN ) );
	if( pBtn )
	{
		ioUIRenderImage *pImg = NULL;
		pImg = g_UIImageSetMgr.CreateImageByFullName( szIcon );
		pBtn->SetIcon( pImg, 0.65f, 0.65f, m_iReinforceMedal + m_iCurLevel );

		pBtn->SetMarkType( INVENTORY_MARK_TYPE_NONE );
		pBtn->SetActive();
	}

	SetWndPos( (Setting::Width()/2) - GetWidth()/2, ((Setting::Height()/2) - GetHeight()/2) - 20 );
}

void ioGrowthSelectWnd::UpdateInfo()
{
	ClearList();

	int iSlotIndex = -1;
	m_iReinforce = 0;

	if( COMPARE( m_iTimeSlot, TIG_WEAPON, TIG_ATTACK ) )
		iSlotIndex = m_iTimeSlot - TIG_WEAPON;
	else if( COMPARE( m_iTimeSlot, TIG_ATTACK, TIG_DROP+1 ) )
		iSlotIndex = m_iTimeSlot - TIG_ATTACK;

	if( iSlotIndex != -1 )
	{
		int iCharArray = g_MyInfo.GetClassArray( m_iClassType );
		const ITEM_DATA &kItem = g_MyInfo.GetCharItem( iCharArray, iSlotIndex );
		m_iReinforce = kItem.m_item_reinforce;
	}

	bool bChar = false;
	if( COMPARE( m_iTimeSlot, TIG_ATTACK, TIG_DROP+1 ) )
		bChar = true;

	// medal
	//강화레벨에 따른 추가 스탯
	if ( m_iReinforce <= g_CompoundMaterialMgr.GetFixedFailReinforce() )
		m_iReinforceMedal = m_iReinforce;
	else
		m_iReinforceMedal = g_GrowthInfoMgr.GetReinforceAddStat( m_iReinforce );

	IntVec vMedalItemTypeVec;
	g_MyInfo.GetMedalItemTypeVec( vMedalItemTypeVec, m_iClassType );

	int iMedalSize = vMedalItemTypeVec.size();
	for (int i = 0; i < iMedalSize ; i++)
		m_iReinforceMedal += g_MedalItemMgr.GetMedalItemGrowth( vMedalItemTypeVec[i], bChar, iSlotIndex );

	PetSlot sPetSlot;
	ioUserPet *pUserPet = g_MyInfo.GetUserPet();
	if ( pUserPet )
		pUserPet->GetPetSlotEquip( sPetSlot );

	AwakeType eAwakeType = static_cast<AwakeType>(g_MyInfo.GetAwakeType( m_iClassType ));
	if ( bChar )
	{
		m_iReinforceMedal += g_PetInfoMgr.GetTotalStat( sPetSlot.m_nPetCode, iSlotIndex, sPetSlot.m_nCurLevel );
		m_iReinforceMedal += g_AwakeMgr.GetAwakeCharGrowth( eAwakeType, iSlotIndex );

		const CHARACTER& rkCharInfo = g_MyInfo.GetCharacterToClassType( m_iClassType );
		if( g_RSoldierInfoManager.IsRSoldierType( rkCharInfo ) )
			m_iReinforceMedal += g_RSoldierInfoManager.GetSoldierAddStat( iSlotIndex, g_MyInfo.GetRSoldierPossessionCount() );

		if( g_RSoldierInfoManager.IsSoldierSet( rkCharInfo ) )
			m_iReinforceMedal += g_RSoldierInfoManager.GetSoldierSetAddStat( iSlotIndex, rkCharInfo.m_class_type, g_MyInfo.GetSoldierSetCnt( g_RSoldierInfoManager.GetSoldierSetNum( rkCharInfo.m_class_type ) ) );
	}
	else 
	{
		m_iReinforceMedal += g_PetInfoMgr.GetTotalStat( sPetSlot.m_nPetCode, iSlotIndex + MAX_ITEM_GROWTH, sPetSlot.m_nCurLevel );
		m_iReinforceMedal += g_AwakeMgr.GetAwakeItemGrowth( eAwakeType, iSlotIndex );

		const CHARACTER& rkCharInfo = g_MyInfo.GetCharacterToClassType( m_iClassType );
		if( g_RSoldierInfoManager.IsRSoldierType( rkCharInfo ) )
			m_iReinforceMedal += g_RSoldierInfoManager.GetSoldierAddStat( iSlotIndex + MAX_ITEM_GROWTH, g_MyInfo.GetRSoldierPossessionCount() );

		if( g_RSoldierInfoManager.IsSoldierSet( rkCharInfo ) )
			m_iReinforceMedal += g_RSoldierInfoManager.GetSoldierSetAddStat( iSlotIndex + MAX_ITEM_GROWTH, rkCharInfo.m_class_type, g_MyInfo.GetSoldierSetCnt( g_RSoldierInfoManager.GetSoldierSetNum( rkCharInfo.m_class_type ) ) );
	}

	m_iCurPoint = 0;
	m_bTimeGrowth = false;

	ioGrowthLevel *pLevel = g_MyInfo.GetGrowthLevel();
	if( pLevel )
	{
		TimeGrowthInfo *pInfo = pLevel->FindTimeGrowthInfo( m_iClassType );
		if( pInfo && m_iTimeSlot > 0 && pInfo->m_iTimeSlot == m_iTimeSlot )
		{
			m_bTimeGrowth = true;
		}

		m_iCurPoint = pLevel->GetCharGrowthPoint( m_iClassType );

		if( bChar )
			m_iCurLevel = pLevel->GetCharGrowthLevel( m_iClassType, iSlotIndex, true );
		else
			m_iCurLevel = pLevel->GetItemGrowthLevel( m_iClassType, iSlotIndex, true );
	}

	// Desc
	m_iClassLevel = g_MyInfo.GetClassLevel( m_iClassType, true );
	m_iClassLevel = min( m_iClassLevel, g_GrowthInfoMgr.GetMaxLevel() );

	int iNextLevel = m_iCurLevel + 1;

	m_bBaseGrowthType = false;
	ioGrowthInfo *pInfo = g_GrowthInfoMgr.GetGrowthInfo( m_iGrowthInfoNum );
	if( pInfo )
	{
		int iGrowthType = pInfo->GetGrowthType();
		ioHashString szDesc = g_GrowthInfoMgr.GetGrowthTypeDesc( iGrowthType );

		switch( iGrowthType )
		{
		case GT_CHAR_ATTACK:
		case GT_CHAR_DEFENSE:
		case GT_CHAR_MOVE_SPEED:
		case GT_CHAR_DROP_DAMAGE:
		case GT_SKILL_ATTACK:
		case GT_SKILL_MOVE_SPEED:
		case GT_SKILL_DROP_DAMAGE:
		case GT_SKILL_DEFENSE:
			m_bBaseGrowthType = true;
			break;
		}

		m_DescList[0].SetTextStyle( TS_NORMAL );
		m_DescList[0].SetBkColor( 0, 0, 0 );
		m_DescList[0].SetTextColor( TCT_DEFAULT_DARKGRAY );

		if( m_bBaseGrowthType )
			m_DescList[0].AddTextPiece( FONT_SIZE_17, STR(1), m_szGrowthName.c_str() );
		else
			m_DescList[0].AddTextPiece( FONT_SIZE_17, STR(2), m_szSlotName.c_str(), m_szGrowthName.c_str() );

		m_DescList[0].SetTextColor( TCT_DEFAULT_GREEN );
		m_DescList[0].AddTextPiece( FONT_SIZE_17, "+1" );
		//m_DescList[0].AddTextPiece( FONT_SIZE_17, "+%d → +%d", m_iCurLevel+m_iReinforceMedal, iNextLevel+m_iReinforceMedal );

		StringVector vDescList;
		vDescList.clear();
		vDescList = ioStringConverter::Split( szDesc.c_str(), "#" );

		int iDescLine = 0;
		StringVector::iterator iter;
		for( iter = vDescList.begin() ; iter!=vDescList.end() ; ++iter, ++iDescLine )
		{
			m_DescList[1+iDescLine].SetTextStyle( TS_NORMAL );
			m_DescList[1+iDescLine].SetBkColor( 0, 0, 0 );
			m_DescList[1+iDescLine].SetTextColor( TCT_DEFAULT_GRAY );
			m_DescList[1+iDescLine].AddTextPiece( FONT_SIZE_11, STR(3), iter->c_str() );
		}
	}

#if defined( _LSWC )
	CheckGrowthUp(); //LSWC
	return;
#endif

	ioGrowthSubWnd *pSubWnd = dynamic_cast<ioGrowthSubWnd*>(FindChildWnd(ID_TIME_GROWTH_WND));
	if( pSubWnd )
	{
		pSubWnd->SetInfo( m_iClassType, m_iTimeSlot, m_iCurLevel, m_iGrowthInfoNum, TIME_GROWTH_TYPE );
	}

	pSubWnd = dynamic_cast<ioGrowthSubWnd*>(FindChildWnd(ID_PESO_GROWTH_WND));
	if( pSubWnd )
	{
		pSubWnd->SetInfo( m_iClassType, m_iTimeSlot, m_iCurLevel, m_iGrowthInfoNum, PESO_GROWTH_TYPE );
	}
}

void ioGrowthSelectWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	RenderGauge( iXPos, iYPos );

	if( !m_DescList[0].IsEmpty() )
	{
		m_DescList[0].PrintFullText( iXPos+162, iYPos+108, TAT_CENTER );
	}

	for( int i=0; i < (MAIN_DESC_LINE-1); ++i )
	{
		if( m_DescList[i+1].IsEmpty() )
			break;

		m_DescList[i+1].PrintFullText( iXPos+162, iYPos+133+(17*i), TAT_CENTER );
	}

	//
	int iNewXPos = iXPos + 162;
	int iNewYPos = iYPos + 399;

	ioHashString szPointText, szNum, szMyPeso, szPesoNum;

	char szPeso[MAX_PATH], szBuf[MAX_PATH];

	szPointText = STR(1);
	szMyPeso = STR(2);

	wsprintf_e( szBuf, "%d", m_iCurPoint );
	szNum = szBuf;

	Help::ConvertNumToStrComma( g_MyInfo.GetMoney(), szPeso, sizeof( szPeso ) );
	SafeSprintf( szBuf, sizeof( szBuf ), STR(3), szPeso );
	szPesoNum = szBuf;

	float fPointSize = g_FontMgr.GetTextWidth( szPointText.c_str(), TS_NORMAL, FONT_SIZE_13 );
	float fNumSize = g_FontMgr.GetTextWidth( szNum.c_str(), TS_NORMAL, FONT_SIZE_13 );
	float fMyPesoSize = g_FontMgr.GetTextWidth( szMyPeso.c_str(), TS_NORMAL, FONT_SIZE_13 );
	float fPesoNumSize = g_FontMgr.GetTextWidth( szPesoNum.c_str(), TS_NORMAL, FONT_SIZE_13 );

	float fTotalSize = fPointSize+fNumSize+fMyPesoSize+fPesoNumSize+12;
	// /2.0f 를 * FLOAT05로 변경
	float fHalfSize = fTotalSize * FLOAT05;

	float fCurPos = iNewXPos - fHalfSize;
	g_FontMgr.SetAlignType( TAT_LEFT );
	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetBkColor( 0, 0, 0 );

	g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
	g_FontMgr.PrintText( fCurPos, iNewYPos, FONT_SIZE_13, STR(4), szPointText.c_str() );

	fCurPos += fPointSize;
	g_FontMgr.SetTextColor( TCT_DEFAULT_RED );
	g_FontMgr.PrintText( fCurPos, iNewYPos, FONT_SIZE_13, STR(5), szNum.c_str() );

	fCurPos += fNumSize;
	if( m_pPointImg )
		m_pPointImg->Render( fCurPos, iNewYPos );

	fCurPos += 12;
	g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
	g_FontMgr.PrintText( fCurPos, iNewYPos, FONT_SIZE_13, STR(6), szMyPeso.c_str() );

	fCurPos += fMyPesoSize;
	g_FontMgr.SetTextColor( TCT_DEFAULT_BLUE );
	g_FontMgr.PrintText( fCurPos, iNewYPos, FONT_SIZE_13, STR(7), szPesoNum.c_str() );
}

void ioGrowthSelectWnd::RenderGauge( int iXPos, int iYPos )
{
	int iNewXPos = iXPos + 15;
	int iNewYPos = iYPos + 53;

	enum
	{
		BACK_X_OFFSET	= 44,
		BACK_Y_OFFSET	= 18,

		GAUGE_X_OFFSET	= 46,
		GAUGE_Y_OFFSET	= 20,

		TOP_X_OFFSET	= 283,
		TOP_Y_OFFSET	= 15,

		BOTTOM_X_OFFSET	= 41,
		BOTTOM_Y_OFFSET	= 29,

		BOTTOM_NUM_X	= 46,
		BOTTOM_NUM_Y	= 37,
	};

	int iTotal = m_iReinforceMedal + m_iClassLevel;
	int iCur = m_iReinforceMedal + m_iCurLevel;

	// Gauge
	if( m_pGaugeBigBack )
		m_pGaugeBigBack->Render( iNewXPos, iNewYPos );

	if( m_pGaugeBack )
		m_pGaugeBack->Render( iNewXPos+BACK_X_OFFSET, iNewYPos+BACK_Y_OFFSET );

	float fTotalRate = 0.0f;
	if( iTotal > 0 )
	{
		if( m_pGaugeLevel && iCur > 0 )
		{
			fTotalRate = (float)iCur / iTotal;

			int iCurPos = 243 * fTotalRate;
			m_pGaugeLevel->RenderWidthCut( iNewXPos+GAUGE_X_OFFSET, iNewYPos+GAUGE_Y_OFFSET, 0, iCurPos );
		}

		if( m_pGaugeReinforce && m_iReinforceMedal > 0 )
		{
			float fReinforceRate = (float)m_iReinforceMedal / iTotal;
			int iCurPos = 243 * fReinforceRate;
			m_pGaugeReinforce->RenderWidthCut( iNewXPos+GAUGE_X_OFFSET, iNewYPos+GAUGE_Y_OFFSET, 0, iCurPos );
		}
	}

	if( m_pTriangle && fTotalRate >= 0 )
	{
		// Top Triangle
		m_pTriangle->SetReverseFormatting( ioUIRenderImage::RF_VERT_REVERSE );
		m_pTriangle->Render( iNewXPos+TOP_X_OFFSET, iNewYPos+TOP_Y_OFFSET );

		// Bottom Triangle
		int iCurPos = 243 * fTotalRate;
		m_pTriangle->SetReverseFormatting( ioUIRenderImage::RF_NONE_REVERSE );
		m_pTriangle->Render( iNewXPos+BOTTOM_X_OFFSET+iCurPos, iNewYPos+BOTTOM_Y_OFFSET );

		ioComplexStringPrinter kPrinterNum;
		kPrinterNum.SetTextStyle( TS_NORMAL );
		kPrinterNum.SetBkColor( 0, 0, 0 );
		kPrinterNum.SetTextColor( TCT_DEFAULT_GREEN );

		iCur = max( 0, iCur );
		kPrinterNum.AddTextPiece( FONT_SIZE_11, STR(1), iCur );

		kPrinterNum.PrintFullText( iNewXPos+BOTTOM_NUM_X+iCurPos, iNewYPos+BOTTOM_NUM_Y, TAT_CENTER );
	}

	// Text
	ioComplexStringPrinter kPrinter;
	kPrinter.SetTextStyle( TS_NORMAL );
	kPrinter.SetBkColor( 0, 0, 0 );
	kPrinter.SetTextColor( TCT_DEFAULT_GRAY );
	kPrinter.AddTextPiece( FONT_SIZE_10, STR(2) );

	kPrinter.SetTextColor( TCT_DEFAULT_BLUE );
	kPrinter.AddTextPiece( FONT_SIZE_10, STR(3), m_iReinforceMedal );

	kPrinter.SetTextColor( TCT_DEFAULT_GRAY );
	kPrinter.AddTextPiece( FONT_SIZE_10, STR(4) );

	kPrinter.SetTextColor( TCT_DEFAULT_GREEN );
	kPrinter.AddTextPiece( FONT_SIZE_10, STR(5), m_iClassLevel );

	kPrinter.SetTextColor( TCT_DEFAULT_GRAY );
	kPrinter.AddTextPiece( FONT_SIZE_10, STR(6) );

	kPrinter.SetTextColor( TCT_DEFAULT_GREEN );
	if( iTotal >= 0 )
		kPrinter.AddTextPiece( FONT_SIZE_10, STR(7), iTotal );
	else
		kPrinter.AddTextPiece( FONT_SIZE_10, "0" );

	kPrinter.PrintFullText( iNewXPos+295, iNewYPos+1, TAT_RIGHT );
}

bool ioGrowthSelectWnd::EnableCheck()
{
	bool bChar = false;
	if( COMPARE( m_iTimeSlot, TIG_ATTACK, TIG_DROP+1 ) )
		bChar = true;

	int iNextLevel = m_iCurLevel+1;

	int iCurMax = g_MyInfo.GetClassLevel( m_iClassType, true );
	iCurMax = min( iCurMax, g_GrowthInfoMgr.GetMaxLevel() );

#ifndef BALANCE_RENEWAL
	if( iNextLevel > iCurMax )
	{
		ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
		kPrinter[0].SetTextStyle( TS_NORMAL );
		kPrinter[0].SetBkColor( 0, 0, 0 );	
		kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
		kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(1) );

		kPrinter[1].SetTextStyle( TS_NORMAL );
		kPrinter[1].SetBkColor( 0, 0, 0 );	
		kPrinter[1].SetTextColor( TCT_DEFAULT_GRAY );
		kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(2) );

		g_GUIMgr.SetPrevMsgListBox( NULL, MB_OK, NULL, kPrinter );
		return false;
	}
#endif

	if( g_MyInfo.IsCharRentalToClassType( m_iClassType ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		return false;
	}

	if( g_MyInfo.IsCharExerciseStyleToClassType( m_iClassType, EXERCISE_RENTAL ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		return false;
	}	

	return true;
}

void ioGrowthSelectWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_TIME_GROWTH_WND:
		if( cmd == IOBN_BTNUP )
		{
			if( !EnableCheck() )
				return;

			m_dwCurSelectBtn = ID_TIME_GROWTH_WND;

			if( param == ioGrowthSubWnd::ID_TIME_CANCEL )
			{
				g_GUIMgr.SetMsgBox( MB_YESNO, this, STR(1) );
			}
			else if( param == ioGrowthSubWnd::ID_TIME_START )
			{
				bool bChar = false;
				if( COMPARE( m_iTimeSlot, TIG_ATTACK, TIG_DROP+1 ) )
					bChar = true;

				int iNextLevel = m_iCurLevel+1;
				int iNeedPoint = g_GrowthInfoMgr.GetGrowthUpNeedPoint( bChar );
				int iNeedPeso = g_GrowthInfoMgr.GetGrowthUpNeedPeso( bChar, iNextLevel );

				ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
				kPrinter[0].SetTextStyle( TS_NORMAL );
				kPrinter[0].SetBkColor( 0, 0, 0 );
				kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
				kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(2) );

				kPrinter[1].SetTextStyle( TS_NORMAL );
				kPrinter[1].SetBkColor( 0, 0, 0 );
				kPrinter[1].SetTextColor( TCT_DEFAULT_DARKGRAY );

				if( m_bBaseGrowthType )
					kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(3), m_szGrowthName.c_str() );
				else
					kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(4), m_szSlotName.c_str(), m_szGrowthName.c_str() );

				kPrinter[1].SetTextColor( TCT_DEFAULT_GREEN );
				kPrinter[1].AddTextPiece( FONT_SIZE_13, "+1" );
				//kPrinter[1].AddTextPiece( FONT_SIZE_13, "+%d → +%d", m_iCurLevel+m_iReinforceMedal, iNextLevel+m_iReinforceMedal );

				kPrinter[2].SetTextStyle( TS_NORMAL );
				kPrinter[2].SetBkColor( 0, 0, 0 );
				kPrinter[2].SetTextColor( TCT_DEFAULT_RED );
				kPrinter[2].AddTextPiece( FONT_SIZE_13, STR(5), iNeedPoint );
				kPrinter[2].SetTextColor( TCT_DEFAULT_GRAY );
				kPrinter[2].AddTextPiece( FONT_SIZE_13, "+ " );

				kPrinter[2].SetTextColor( TCT_DEFAULT_BLUE );
				kPrinter[2].AddTextPiece( FONT_SIZE_13, STR(6) );

				g_GUIMgr.SetPrevMsgListBox( NULL, MB_YESNO, this, kPrinter );
			}
		}
		break;
	case ID_PESO_GROWTH_WND:
		if( cmd == IOBN_BTNUP )
		{
			if( !EnableCheck() )
				return;

			m_dwCurSelectBtn = ID_PESO_GROWTH_WND;

			if( m_bTimeGrowth )
			{
				g_GUIMgr.SetMsgBox( MB_YESNO, this, STR(1) );
			}
			else
			{
				bool bChar = false;
				if( COMPARE( m_iTimeSlot, TIG_ATTACK, TIG_DROP+1 ) )
					bChar = true;

				int iNextLevel = m_iCurLevel+1;
				int iNeedPoint = g_GrowthInfoMgr.GetGrowthUpNeedPoint( bChar );
				int iNeedPeso = g_GrowthInfoMgr.GetGrowthUpNeedPeso( bChar, iNextLevel );

				ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
				kPrinter[0].SetTextStyle( TS_NORMAL );
				kPrinter[0].SetBkColor( 0, 0, 0 );
				kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
				kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(7) );

				kPrinter[1].SetTextStyle( TS_NORMAL );
				kPrinter[1].SetBkColor( 0, 0, 0 );
				kPrinter[1].SetTextColor( TCT_DEFAULT_DARKGRAY );

				if( m_bBaseGrowthType )
					kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(8), m_szGrowthName.c_str() );
				else
					kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(9), m_szSlotName.c_str(), m_szGrowthName.c_str() );

				kPrinter[1].SetTextColor( TCT_DEFAULT_GREEN );
				kPrinter[1].AddTextPiece( FONT_SIZE_13, "+1" );
				//kPrinter[1].AddTextPiece( FONT_SIZE_13, "+%d → +%d", m_iCurLevel+m_iReinforceMedal, iNextLevel+m_iReinforceMedal );

				kPrinter[2].SetTextStyle( TS_NORMAL );
				kPrinter[2].SetBkColor( 0, 0, 0 );
				kPrinter[2].SetTextColor( TCT_DEFAULT_RED );
				kPrinter[2].AddTextPiece( FONT_SIZE_13, STR(10), iNeedPoint );
				kPrinter[2].SetTextColor( TCT_DEFAULT_GRAY );
				kPrinter[2].AddTextPiece( FONT_SIZE_13, "+ " );

				char szBuf[MAX_PATH], szNumComma[MAX_PATH];
				Help::ConvertNumToStrComma( iNeedPeso, szNumComma, sizeof(szNumComma) );
				SafeSprintf( szBuf, sizeof( szBuf ), STR(11), szNumComma );

				kPrinter[2].SetTextColor( TCT_DEFAULT_BLUE );
				kPrinter[2].AddTextPiece( FONT_SIZE_13, STR(12), szBuf );

				g_GUIMgr.SetPrevMsgListBox( NULL, MB_YESNO, this, kPrinter );
			}
		}
		break;
	case ID_X:
		if( cmd == IOBN_BTNUP )
		{
			HideWnd();
		}
		break;
	case MESSAGE_BOX:
	case MESSAGE_BOX3:
		if( cmd == IOBN_BTNUP )
		{
			if( param == IDYES )
			{
				if( m_dwCurSelectBtn == ID_TIME_GROWTH_WND )
					CheckTimeGrowthUp();
				else if( m_dwCurSelectBtn == ID_PESO_GROWTH_WND )
					CheckGrowthUp();
			}

			m_dwCurSelectBtn = 0;
		}
		break;
	}
}

bool ioGrowthSelectWnd::iwm_esc()
{
	if( HasWndStyle( IWS_EXIT_ESC ) )
	{
		iwm_command( FindChildWnd( ID_X ), IOBN_BTNUP, 0 );
		return true;
	}
	return false;
}

void ioGrowthSelectWnd::CheckGrowthUp()
{
	int iSlot = 0;
	bool bItem = false;

	if( COMPARE( m_iTimeSlot, TIG_WEAPON, TIG_ATTACK ) )
	{
		bItem = true;
		iSlot = m_iTimeSlot - TIG_WEAPON;
	}
	else if( COMPARE( m_iTimeSlot, TIG_ATTACK, TIG_DROP+1 ) )
	{
		bItem = false;
		iSlot = m_iTimeSlot - TIG_ATTACK;
	}
	else
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return;
	}

	// Send
	SP2Packet kPacket( CTPK_GROWTH_LEVEL_UP );
	kPacket << m_iClassType;
	kPacket << bItem;
	kPacket << iSlot;
	kPacket << m_iGrowthInfoNum;
	TCPNetwork::SendToServer( kPacket );

	g_App.SetMouseBusy( true );

	HideWnd();
}

void ioGrowthSelectWnd::CheckTimeGrowthUp()
{
	// Send
	if( !m_bTimeGrowth )
	{
		SP2Packet kPacket( CTPK_TIME_GROWTH_ADD );
		kPacket << m_iClassType;
		kPacket << m_iTimeSlot;
		TCPNetwork::SendToServer( kPacket );
	}
	else
	{
		SP2Packet kPacket( CTPK_TIME_GROWTH_REMOVE );
		kPacket << m_iClassType;
		kPacket << m_iTimeSlot;
		TCPNetwork::SendToServer( kPacket );
	}

	g_App.SetMouseBusy( true );

	HideWnd();
}