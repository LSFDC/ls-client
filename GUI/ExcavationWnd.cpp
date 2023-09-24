
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
#include "..\io3DEngine\ioStringManager.h"
#include "..\io3DEngine\Safesprintf.h"

#include "../NetworkWrappingFunc.h"
#include "../WndID.h"
#include "..\ioExcavationManager.h"
#include "..\ioItemMaker.h"
#include "..\ioMyInfo.h"
#include "..\EtcHelpFunc.h"
#include "..\ioPlayStage.h"
#include "..\ioBaseChar.h"
#include "..\ioEtcItemManager.h"
#include "..\Setting.h"
#include "..\TextColorTable.h"
#include "../ioPresentMgr.h"

#include "../io3DEngine/ioMovingWnd.h"

#include "ioMessageBox.h"
#include "ioSP2GUIManager.h"
#include "NewShopWnd.h"
#include "MyInventoryWnd.h"

#include ".\excavationwnd.h"

#include <strsafe.h>
#include "..\ioExcavationItem.h"
#include "..\ioKeyManager.h"
#include "..\ioMyLevelMgr.h"
#include "GlobalWnd.h"

ExcavationResultWnd::ExcavationResultWnd(void)
{
	m_pRedBackEffect  = NULL;
	m_pBlueBackEffect = NULL;
	m_pNumText  = NULL;
	m_pPlusText = NULL;

	m_pIcon	= NULL;

	m_bFinalResult       = false;

	m_fIconScale         = 0.0f;
	m_dwIconScaleTime    = 0;
	m_dwRollingTime      = 0;
	m_dwRollingTotalTime = 0;

	m_dwRollingCheckTime = 0;
	m_dwRollingEndTime   = 0;
	m_dwScaleStartTime   = 0;

	m_iCurIconArray      = 0;
	m_iCurGradeArray     = 0;

	m_iArtifactType      = 0;
	m_iArtifactValue1    = 0;
	m_iArtifactValue2    = 0;
	m_bAlarm             = false;
	m_fMapRate           = 0.0f;

	m_pPlayStage         = NULL;
}

ExcavationResultWnd::~ExcavationResultWnd(void)
{
	SAFEDELETE(m_pRedBackEffect);
	SAFEDELETE(m_pBlueBackEffect);

	SAFEDELETE(m_pNumText);
	SAFEDELETE(m_pPlusText);

	SAFEDELETE(m_pIcon);
}

void ExcavationResultWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "red_back_effect" )
	{
		SAFEDELETE( m_pRedBackEffect );
		m_pRedBackEffect = pImage;
	}
	else if( szType == "blue_back_effect" )
	{
		SAFEDELETE( m_pBlueBackEffect );
		m_pBlueBackEffect = pImage;
	}
	else if( szType == "num_text" )
	{
		SAFEDELETE( m_pNumText );
		m_pNumText = pImage;
	}
	else if( szType == "plus_image" )
	{
		SAFEDELETE( m_pPlusText );
		m_pPlusText = pImage;
	}
	else
		ioWnd::AddRenderImage( szType, pImage );
}

void ExcavationResultWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	m_dwRollingTime      = xElement.GetIntAttribute_e( "Rolling_Time" );
	m_dwRollingTotalTime = xElement.GetIntAttribute_e( "Rolling_Total_Time" );

	m_fIconScale      = xElement.GetFloatAttribute_e( "Icon_Scale" );
	m_dwIconScaleTime = xElement.GetIntAttribute_e( "Icon_Scale_Time" );

	m_szRollingSound = xElement.GetStringAttribute_e( "Rolling_Sound" );
	m_szResultSound  = xElement.GetStringAttribute_e( "Result_Sound" );
	m_szResultFailSound = xElement.GetStringAttribute_e( "Result_Fail_Sound" );
}

void ExcavationResultWnd::iwm_show()
{
	DWORD dwCurTime = FRAMEGETTIME();
	m_dwRollingEndTime = dwCurTime + m_dwRollingTotalTime;
	m_dwRollingCheckTime = 0;
	m_dwScaleStartTime   = 0;

	m_iCurIconArray = -1;
	m_iCurGradeArray= -1;
	m_bFinalResult  = false;

	char szHack1[] = {'?','?','?','?','?','\0'};
	m_szResultArtifact = szHack1;

	ReBatchButton( ROLLING_BTN );
	if( !m_szRollingSound.IsEmpty() )
		g_SoundMgr.PlaySound( m_szRollingSound, DSBVOLUME_MAX, PSM_LOOP | PSM_THREAD );
}

void ExcavationResultWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;

	ioWnd::OnProcess( fTimePerSec );

	DWORD dwCurTime = FRAMEGETTIME();

	if( m_dwRollingEndTime < dwCurTime  )			// RollingEnd
	{
		// Icon 설정
		if( !m_bFinalResult )		// 한 번만 하기 위해
		{
			SetArtifactInfo();
			m_dwScaleStartTime = dwCurTime;
		}

		// IconScale 설정
		float fScale = FLOAT1;
		DWORD dwGapTime = dwCurTime - m_dwScaleStartTime;
		if( dwGapTime < m_dwIconScaleTime )
		{
			float fScaleRate = (float)dwGapTime / (float)m_dwIconScaleTime;
			fScale = m_fIconScale * ( FLOAT1 - fScaleRate ) + FLOAT1 * fScaleRate;
		}

		if( m_pIcon )
			m_pIcon->SetScale( fScale );

		if( !m_bFinalResult )
		{
			m_bFinalResult = true;
			ReBatchButton( RESULT_BTN );

			if( !m_szRollingSound.IsEmpty() )
				g_SoundMgr.StopSound( m_szRollingSound, 0 );

			if( !COMPARE( m_iArtifactType, ioExcavationManager::FAIL_ARTIFACT_START_TYPE, ioExcavationManager::EXTRAITEM_START_TYPE ) )
			{
				if( !m_szResultSound.IsEmpty() )
					g_SoundMgr.PlaySound( m_szResultSound );
			}
			else
			{
				if( !m_szResultFailSound.IsEmpty() )
					g_SoundMgr.PlaySound( m_szResultFailSound );
			}

			if( m_bAlarm )
			{
				SP2Packet kPacket( CTPK_SERVER_ALARM_MSG );
				kPacket << UDP_SERVER_ALARM_EXCAVATION << g_MyInfo.GetPublicID() << m_iArtifactType << m_iArtifactValue1 << m_iArtifactValue2 << m_fMapRate;
				TCPNetwork::SendToServer( kPacket );
			}
			else 
			{
				ioHashString szPublicID = g_MyInfo.GetPublicID();
				g_ExcavationMgr.SetExcavatingSuccessAlarm( szPublicID, m_iArtifactType, m_iArtifactValue1, m_iArtifactValue2, m_fMapRate, false );
			}

			ExcavationSearchingWnd *pSearchingWnd = dynamic_cast<ExcavationSearchingWnd*> ( g_GUIMgr.FindWnd( EXCAVATION_SEARCHING_WND ) );
			if( pSearchingWnd && pSearchingWnd->IsShow() )
			{
				pSearchingWnd->SetProcessRadarPos( true );
			}
		}
	}
	else // Rolling
	{
		if( m_dwRollingCheckTime == 0 || dwCurTime - m_dwRollingCheckTime > m_dwRollingTime )		// 교체
		{
			// item icon
			SAFEDELETE( m_pIcon );
			m_iCurIconArray++;
			if( !COMPARE( m_iCurIconArray, 0, g_ExcavationMgr.GetItemSize() ) )
				m_iCurIconArray = 0;

			ioHashString szCurIcon;
			g_ExcavationMgr.GetItemIcon( m_iCurIconArray, szCurIcon );
			if( !szCurIcon.IsEmpty() )
				m_pIcon = g_UIImageSetMgr.CreateImageByFullName( szCurIcon );

			// name
			m_szName.Clear();
			g_ExcavationMgr.GetItemName( m_iCurIconArray, m_szName );
			m_szDesc.Clear();
			g_ExcavationMgr.GetItemDesc( m_iCurIconArray, m_szDesc );

			// grade
			m_iCurGradeArray++;
			if( !COMPARE( m_iCurGradeArray, 0, g_ExcavationMgr.GetGradeSize() ) )
				m_iCurGradeArray = 0;
			g_ExcavationMgr.GetGradeRank( m_iCurGradeArray, m_szGrade );
			m_dwRollingCheckTime = dwCurTime;
		}
	}
}

void ExcavationResultWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	enum
	{
		ICON_X		= 170,
		ICON_Y		= 98,

		GRADE_X     = 170,
		GRADE_Y     = 144,

		NAME_X		= 170,
		NAME_Y		= 166,

		DESC_X		= 170,
		DESC_Y		= 198,

		ARTIFACT_X  = 170,
		ARTIFACT_Y  = 218,

		REINFORCE_X	= 170,
		REINFORCE_Y	= 112,

		TEXT_SIZE	= 15,
		TEXT_GAP	= 5,
		PLUS_SIZE   = 14,
	};

	// Back
	if( m_bFinalResult && m_pBlueBackEffect )
		m_pBlueBackEffect->Render( iXPos+ICON_X, iYPos+ICON_Y, UI_RENDER_MULTIPLY );
	else if( !m_bFinalResult && m_pRedBackEffect )
		m_pRedBackEffect->Render( iXPos+ICON_X, iYPos+ICON_Y, UI_RENDER_MULTIPLY );
	
	// Icon
	if( m_pIcon )
		m_pIcon->Render( iXPos+ICON_X, iYPos+ICON_Y, UI_RENDER_NORMAL, TFO_BILINEAR );

	// Reinforce
	if( m_bFinalResult && m_pNumText && m_pPlusText && m_iArtifactType > ioExcavationManager::EXTRAITEM_START_TYPE )
	{
		int iReinforce = m_iArtifactValue2 / PRESENT_EXTRAITEM_DIVISION_2;   // /10000은 선물 value2의 장비 성장값과 동일
		int iCurPos = 0;
		int iDefaultPos = iXPos + REINFORCE_X;

		int iTotalSize = PLUS_SIZE;      
		if( iReinforce < 10 )
			iTotalSize += TEXT_SIZE;
		else if( iReinforce < FLOAT100 )
			iTotalSize += TEXT_SIZE * 2;
		else
			iTotalSize += TEXT_SIZE * 3;

		iCurPos = iDefaultPos - (iTotalSize / 2);
		m_pNumText->RenderNum( iCurPos + ( PLUS_SIZE - ( TEXT_GAP - 1 ) ), iYPos + REINFORCE_Y, iReinforce, -TEXT_GAP );
		m_pPlusText->Render( iCurPos, iYPos + REINFORCE_Y );
	}

	// Text
	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetAlignType( TAT_CENTER );

	g_FontMgr.SetTextColor( TCT_DEFAULT_BLUE );
	g_FontMgr.PrintText( iXPos + GRADE_X, iYPos + GRADE_Y, FONT_SIZE_17, m_szGrade.c_str() );

	g_FontMgr.SetTextColor( TCT_DEFAULT_BLUE );
	g_FontMgr.PrintText( iXPos + NAME_X, iYPos + NAME_Y, FONT_SIZE_17, m_szName.c_str() );

	g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
	g_FontMgr.PrintText( iXPos + DESC_X, iYPos + DESC_Y, FONT_SIZE_13, m_szDesc.c_str() );

	g_FontMgr.SetTextColor( TCT_DEFAULT_GREEN );
	g_FontMgr.PrintText( iXPos + ARTIFACT_X, iYPos + ARTIFACT_Y, FONT_SIZE_13, m_szResultArtifact.c_str() );
}

void ExcavationResultWnd::iwm_hide()
{
	if( !m_szRollingSound.IsEmpty() )
	{
		g_SoundMgr.StopSound( m_szRollingSound, 0 );
	}

	g_ExcavationMgr.NoticeLevelAndExp();

	ExcavationSearchingWnd *pSearchingWnd = dynamic_cast< ExcavationSearchingWnd* > ( g_GUIMgr.FindWnd( EXCAVATION_SEARCHING_WND ) );
	if( pSearchingWnd )
		pSearchingWnd->UpdateExp();
}


void ExcavationResultWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();

	switch( dwID )
	{
	case ID_GO:
		if( cmd == IOBN_BTNUP )
		{
			HideWnd();

			MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(g_GUIMgr.FindWnd(MY_INVENTORY_WND));
			if( pInvenWnd )
			{
				pInvenWnd->ShowExtraItemTabDirect( g_MyInfo.GetClassType() );
			}
		}
		break;
	case ID_X:
	case ID_OK:
		if( cmd == IOBN_BTNUP )
		{
			HideWnd();
		}
		break;
	}
}

void ExcavationResultWnd::ReBatchButton( int iType )
{
	switch( iType )
	{
	case ROLLING_BTN:
		ShowChildWnd( ID_GO );
		ShowChildWnd( ID_OK );
		ShowChildWnd( ID_X );

		SetChildInActive( ID_GO );
		SetChildInActive( ID_OK );
		SetChildInActive( ID_X );
		break;
	case RESULT_BTN:
		ShowChildWnd( ID_GO );
		ShowChildWnd( ID_OK );
		ShowChildWnd( ID_X );

		if( m_iArtifactType < ioExcavationManager::EXTRAITEM_START_TYPE )
			SetChildInActive( ID_GO );
		else
			SetChildActive( ID_GO );
		SetChildActive( ID_OK );
		SetChildActive( ID_X );
		break;
	}
}

void ExcavationResultWnd::SetResultInfo( int iArtifactType, int iArtifactValue1, int iArtifactValue2, bool bAlram, float fMapRate )
{
	m_iArtifactType   = iArtifactType;
	m_iArtifactValue1 = iArtifactValue1;
	m_iArtifactValue2 = iArtifactValue2;
	m_bAlarm          = bAlram;
	m_fMapRate        = fMapRate;

}

bool ExcavationResultWnd::iwm_spacebar()
{
	ioMessageBoxBtn *pYesBtn = (ioMessageBoxBtn*)FindChildWnd( ID_GO );
	if( !pYesBtn ) return false;
	if( !pYesBtn->IsShow() ) return false;

	if( pYesBtn->HasWndStyle( IWS_INACTIVE ) ) return false;

	iwm_command( pYesBtn, IOBN_BTNUP, 0 );
	return true;
}

bool ExcavationResultWnd::iwm_esc()
{
	ioMessageBoxBtn *pXBtn = (ioMessageBoxBtn*)FindChildWnd( ID_X );
	if( !pXBtn ) return false;
	if( !pXBtn->IsShow() ) return false;

	if( pXBtn->HasWndStyle( IWS_INACTIVE ) ) return false;

	iwm_command( pXBtn, IOBN_BTNUP, 0 );
	return true;
}

void ExcavationResultWnd::SetArtifactInfo()
{
	SAFEDELETE( m_pIcon );

	m_szName.Clear();
	m_szDesc.Clear();
	m_szGrade.Clear();
	m_szResultArtifact.Clear();

	// peso
	if( m_iArtifactType < ioExcavationManager::EXTRAITEM_START_TYPE ) 
	{
		int iItemArray = g_ExcavationMgr.GetItemArray( m_iArtifactType );
		
		ioHashString szIcon;
		g_ExcavationMgr.GetItemIcon( iItemArray, szIcon );
		if( !szIcon.IsEmpty() )
			m_pIcon = g_UIImageSetMgr.CreateImageByFullName( szIcon );

		if( m_pPlayStage )
		{
			ioBaseChar *pOwner = m_pPlayStage->GetOwnerChar();
			if( pOwner )
				pOwner->SetEmoticon( szIcon );
		}

		g_ExcavationMgr.GetItemName( iItemArray, m_szName );
		g_ExcavationMgr.GetItemDesc( iItemArray, m_szDesc );

		int iGradeArray = g_ExcavationMgr.GetGradeArray( m_iArtifactValue2 );
		g_ExcavationMgr.GetGradeRank( iGradeArray, m_szGrade );

		int iPeso = m_iArtifactValue1;
		iPeso = ( iPeso * g_ExcavationMgr.GetGradeRate( iGradeArray ) ) * m_fMapRate;

		char szComma[MAX_PATH]="";
		Help::ConvertNumToStrComma( iPeso, szComma, sizeof( szComma ) );
		char szTemp[MAX_PATH]="";
		SafeSprintf( szTemp, sizeof( szTemp ), STR(1), szComma );
		m_szResultArtifact = szTemp;

		return;
	}
	
	// extraitem
	ioHashString szIcon;
	ioHashString szName;
	const ioItem *pItem = g_ItemMaker.GetItemConst( m_iArtifactValue1, __FUNCTION__ );
	if( pItem )
	{
		szIcon = pItem->GetItemLargeIconName();
		szName = pItem->GetName();
	}
	if( !szIcon.IsEmpty() )
		m_pIcon = g_UIImageSetMgr.CreateImageByFullName( szIcon );

	if( m_pPlayStage )
	{
		ioBaseChar *pOwner = m_pPlayStage->GetOwnerChar();
		if( pOwner )
			pOwner->SetEmoticon( szIcon );
	}

	int iItemArray = g_ExcavationMgr.GetItemArray( m_iArtifactType );
	g_ExcavationMgr.GetItemName( iItemArray, m_szName );
	g_ExcavationMgr.GetItemDesc( iItemArray, m_szDesc );
	char szTemp[MAX_PATH]="";
	 // /10000은 선물 value2의 장비 성장값과 동일
	int iReinforce = m_iArtifactValue2 / 10000;  
	SafeSprintf( szTemp , sizeof( szTemp ), STR(2), iReinforce, szName.c_str() );
	m_szName = szTemp;

	// %10000은 선물하기 value2의 장비 기간값과 동일함.
	int iHours  = m_iArtifactValue2 % 10000;
	if( iHours == 0 )
		StringCbCopy( szTemp, sizeof( szTemp ), STR(3) );
	else if( ( iHours/24 ) > 0  )
		SafeSprintf( szTemp , sizeof( szTemp ), STR(4), iHours/24 );
	else
		SafeSprintf( szTemp , sizeof( szTemp ), STR(5), iHours ); 
	m_szGrade = szTemp;		
	m_szResultArtifact = STR(6);
}

void ExcavationResultWnd::SetPlayStage( ioPlayStage *pPlayStage )
{
	m_pPlayStage = pPlayStage;
}
//-------------------------------------------------------------------------------------------------------------------------------
ExcavationExpWnd::ExcavationExpWnd()
{
	m_pExpGauge = NULL;

	m_iCurLevel = 0;
	m_iCurExp = 0;
}

ExcavationExpWnd::~ExcavationExpWnd()
{
	SAFEDELETE(m_pExpGauge);
}

void ExcavationExpWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
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

void ExcavationExpWnd::SetInfo( int iLevel, int iExp )
{
	m_iCurLevel = iLevel;
	m_iCurExp = iExp;
}

void ExcavationExpWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	float fExpRate = 0.0f;
	int iMaxExp = g_LevelMgr.GetNextExcavationLevelUpExp( m_iCurLevel );
	if( iMaxExp != 0 )
		fExpRate = (float)m_iCurExp / (float)iMaxExp;

	if( m_pExpGauge )
	{
		int iWidth = m_pExpGauge->GetWidth();
		iWidth *= fExpRate;
		m_pExpGauge->RenderWidthCut( iXPos , iYPos + 1, 0, iWidth );
	}

	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetAlignType( TAT_RIGHT );
	g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
	g_FontMgr.PrintText( iXPos - 9, iYPos - 3, FONT_SIZE_12, STR(1), m_iCurLevel );

	g_FontMgr.SetTextColor( TCT_DEFAULT_BLUE );
	g_FontMgr.PrintText( iXPos + 191, iYPos - 3, FONT_SIZE_12, STR(2), (int)( fExpRate * FLOAT100 ) );

}
//-------------------------------------------------------------------------------------------------------------------------------
ExcavatingBtn::ExcavatingBtn()
{
	m_pIcon = NULL;

	m_iMaxCoolTime  = 0;
	m_iCurCoolTime  = 0;
	m_dwCoolStartTime = 0;

	m_fCurScaleRate    = 0.0f;
	m_dwScaleStartTime = 0;

	m_pKeyBackNor = NULL;
	m_pKeyBackOn  = NULL;
	m_pKeyBack    = NULL;

	m_pAttackKeyNor  = NULL;
	m_pAttackKeyOn   = NULL;

	m_pKeyDashNor = NULL;
	m_pKeyDashOn  = NULL;

	m_wAttackValue = 0;
}

ExcavatingBtn::~ExcavatingBtn()
{
	SAFEDELETE( m_pIcon );

	SAFEDELETE( m_pKeyBackNor );
	SAFEDELETE( m_pKeyBackOn );
	SAFEDELETE( m_pKeyBack );

	SAFEDELETE( m_pAttackKeyNor );
	SAFEDELETE( m_pAttackKeyOn );

	SAFEDELETE( m_pKeyDashNor );
	SAFEDELETE( m_pKeyDashOn );
}

void ExcavatingBtn::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "Icon" )
	{
		SAFEDELETE( m_pIcon );
		m_pIcon  = pImage;
	}
	else if( szType == "KeyBackNor" )
	{
		SAFEDELETE( m_pKeyBackNor );
		m_pKeyBackNor = pImage;
	}
	else if( szType == "KeyBackOn" )
	{
		SAFEDELETE( m_pKeyBackOn );
		m_pKeyBackOn = pImage;
	}
	else if( szType == "KeyBack" )
	{
		SAFEDELETE( m_pKeyBack );
		m_pKeyBack = pImage;
	}
	else if( szType == "KeyDashNor" )
	{
		SAFEDELETE( m_pKeyDashNor );
		m_pKeyDashNor = pImage;
	}
	else if( szType == "KeyDashOn" )
	{
		SAFEDELETE( m_pKeyDashOn );
		m_pKeyDashOn  = pImage;
	}
	else
	{
		ioWnd::AddRenderImage( szType, pImage );
	}
}

void ExcavatingBtn::OnRender()
{
	ioWnd::OnRender();

	int iXPos, iYPos;
	iXPos = GetDerivedPosX();
	iYPos = GetDerivedPosY();

	enum 
	{ 
		ICON_HEIGHT_EX = 54,  
		ICON_X     = 28,
		ICON_Y     = 27,

		DESC_X     = 30,
		DESC_Y     = -12,

		INPUTBACK_XOFFSET = 12,
		INPUTBACK_YOFFSET = 53,
		INPUT_XOFFSET = 13, 
		INPUT_YOFFSET = 54,
	};

	if( IsCanExcavating() )
	{
		if( m_pIcon )
			m_pIcon->Render( iXPos + ICON_X, iYPos + ICON_Y, UI_RENDER_NORMAL, TFO_BILINEAR );
	}
	else
		RenderGaugeIcon( iXPos + ICON_X, iYPos + ICON_Y, m_pIcon, ICON_HEIGHT_EX, m_iCurCoolTime, m_iMaxCoolTime );

	// text
	g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetAlignType( TAT_CENTER );

	g_FontMgr.SetTextColor( 255, 255, 255 );
	if( IsCanExcavating() )
		g_FontMgr.PrintText( iXPos + DESC_X, iYPos + DESC_Y, FONT_SIZE_13, STR(1) );
	else
		g_FontMgr.PrintText( iXPos + DESC_X, iYPos + DESC_Y, FONT_SIZE_13, STR(2) );

	RenderInputKeyBack( iXPos + INPUTBACK_XOFFSET, iYPos + INPUTBACK_YOFFSET );
	RenderInputKey( iXPos + INPUT_XOFFSET, iYPos + INPUT_YOFFSET );
}

void ExcavatingBtn::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;

	ProcessScale();
	ProcessGaugeIcon();
	UpdateAttackKeyImage();
}

void ExcavatingBtn::ProcessScale()
{
	if( !IsCanExcavating() )
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

void ExcavatingBtn::ProcessGaugeIcon()
{
	m_iCurCoolTime = FRAMEGETTIME() - m_dwCoolStartTime;
	if( m_iCurCoolTime > m_iMaxCoolTime )
		m_iCurCoolTime = m_iMaxCoolTime;
}

int ExcavatingBtn::GetRenderImageStartHeight( ioUIRenderImage *pImage, int iSrcHeight, int iCur, int iMax )
{
	if( iMax == iCur ) return 0;
	if( !pImage )	return 0;

	float fGap = (float)iCur / (float)iMax;
	float fGapHeight = (float)iSrcHeight * fGap;
	int iOffYPos = pImage->GetOffsetY();
	int iHeight  = pImage->GetHeight();

	return iOffYPos + ( (iSrcHeight / 2) - fGapHeight );							
}

void ExcavatingBtn::RenderGaugeIcon( int xPos, int yPos, ioUIRenderImage *pImage, int iSrcHeight, int iCur, int iMax )
{
	if( !pImage )
		return;

	int iRealHeight = GetRenderImageStartHeight( pImage, iSrcHeight, iCur, iMax );

	pImage->Render( xPos, yPos, UI_RENDER_GRAY );
	pImage->RenderHeightCut( xPos, yPos, iRealHeight, pImage->GetHeight() );
	
	if( iRealHeight + 2 <= pImage->GetHeight() )
	{
		pImage->SetColor( 0, 0, 0 );
		pImage->RenderHeightCut( xPos, yPos, iRealHeight, iRealHeight + 2 );
		pImage->SetColor( 255, 255, 255 );
	}
}

bool ExcavatingBtn::IsCanExcavating()
{
	if( m_iMaxCoolTime == m_iCurCoolTime )
		return true;

	return false;
}

void ExcavatingBtn::InitCoolTime()
{
	m_iMaxCoolTime = g_ExcavationMgr.GetExcavatingCoolTime();
	m_iCurCoolTime = 0;
	m_dwCoolStartTime = FRAMEGETTIME();
}

void ExcavatingBtn::iwm_show()
{
	m_iMaxCoolTime    = g_ExcavationMgr.GetExcavatingFirstCoolTime();
	m_iCurCoolTime    = 0;
	m_dwCoolStartTime = FRAMEGETTIME();
}

void ExcavatingBtn::ClearCurCoolTime()
{
	m_iCurCoolTime = 0;
	m_dwCoolStartTime = FRAMEGETTIME();
}

void ExcavatingBtn::RenderInputKeyBack( int xPos, int yPos )
{
	if( !m_pKeyBack ) return;
	m_pKeyBack->Render( xPos, yPos);
}


void ExcavatingBtn::RenderInputKey( int xPos, int yPos )
{
	if( !m_pKeyBackNor || !m_pKeyBackOn ) return;

	if( IsCanExcavating() == false )
	{
		m_pKeyBackNor->Render(xPos, yPos);	

		if( m_pKeyDashNor )
			m_pKeyDashNor->Render( xPos + 4, yPos + 3 );

		if( m_pAttackKeyNor )
			m_pAttackKeyNor->Render( xPos + 12, yPos + 3 );

		if( m_pKeyDashNor )
			m_pKeyDashNor->Render( xPos + 20, yPos + 3 );
	}
	else
	{
		m_pKeyBackOn->Render(xPos, yPos);			

		if( m_pKeyDashOn )
			m_pKeyDashOn->Render( xPos + 4, yPos + 3 );

		if( m_pAttackKeyOn )
			m_pAttackKeyOn->Render( xPos + 12, yPos + 3 );

		if( m_pKeyDashOn )
			m_pKeyDashOn->Render( xPos + 20, yPos + 3 );
	}	
}

void ExcavatingBtn::UpdateAttackKeyImage()
{
	bool bChangeKey = false;
	ioUserKeyInput::UseKeyValue kKeys;
	kKeys.SetDefaultFirstKey();
	g_KeyManager.GetGameKey( kKeys );

	if( m_wAttackValue != kKeys.m_wAttackKey )
	{
		m_wAttackValue = kKeys.m_wAttackKey;
		bChangeKey = true;
	}

	if( bChangeKey )
	{ 
		SAFEDELETE( m_pAttackKeyNor );
		SAFEDELETE( m_pAttackKeyOn );
		m_pAttackKeyNor = GetNorKeyImage( m_wAttackValue );
		m_pAttackKeyOn  = GetOnKeyImage( m_wAttackValue );
	}
}

ioUIRenderImage *ExcavatingBtn::GetNorKeyImage( WORD wKeySetValue )
{
	// Off
	char szBuf[MAX_PATH] = "";
	StringCbPrintf( szBuf, sizeof( szBuf ), "BaseImage010#Skillkey_Nor_%d", wKeySetValue );
	ioUIRenderImage *pReturnImage = g_UIImageSetMgr.CreateImageByFullName( szBuf );
	if( pReturnImage == NULL )
		pReturnImage = g_UIImageSetMgr.CreateImageByFullName( "BaseImage010#Skillkey_Nor_Question" );
	return pReturnImage;
}

ioUIRenderImage *ExcavatingBtn::GetOnKeyImage( WORD wKeySetValue )
{
	// On
	char szBuf[MAX_PATH] = "";
	StringCbPrintf( szBuf, sizeof( szBuf ), "BaseImage010#Skillkey_Act_%d", wKeySetValue );
	ioUIRenderImage *pReturnImage = g_UIImageSetMgr.CreateImageByFullName( szBuf );
	if( pReturnImage == NULL )
		pReturnImage = g_UIImageSetMgr.CreateImageByFullName( "BaseImage010#Skillkey_Act_Question" );

	return pReturnImage;
}

//-------------------------------------------------------------------------
ExcavationSearchingWnd::ExcavationSearchingWnd()
{
	m_pPlayStage    = NULL;
	
	m_pBatteryGauge = NULL;
	for (int i = 0; i < MAX_SEARCHING_TYPE ; i++)
	{
		m_pWave[i] = NULL;
	}

	m_pDistNumBlue  = NULL;
	m_pDistNumYellow= NULL;
	m_pDistNumRed   = NULL;
	m_pRateNum      = NULL;
	m_pDistNone     = NULL;

	m_dwFullBatteryTime   = 0;
	m_dwMiddleBatteryTime = 0;
	m_dwLowBatteryTime    = 0;

	m_iRangeTypeArray         = 0;
	m_iCurGrowWaveTypeArray   = 0;
	m_iCurExtinctWaveTypeArray= 0;
	m_iWaveWidthcut           = 0;

	m_dwArtifactPosCheckTime    = 0;
	m_dwCurArtifactPosCheckTime = 0;

	m_dwRadarCheckTime          = 0;
	m_dwCurRadarCheckTime       = 0;

	m_iWaveAddSize              = 0;

	m_iArtifactDist             = 0;

	m_iDefaultX = 0;
	m_iDefaultY = 0;

	m_iCurSuccessPercent        = 0;

	m_dwSuccessPerCheckTime    = 0;
	m_dwCurSuccessPerCheckTime = 0;

	m_bProcessRadarPos = true;
}

ExcavationSearchingWnd::~ExcavationSearchingWnd()
{
	SAFEDELETE(m_pBatteryGauge);

	for (int i = 0; i < MAX_SEARCHING_TYPE ; i++)
	{
		SAFEDELETE(m_pWave[i]);
	}

	SAFEDELETE(m_pDistNumBlue);
	SAFEDELETE(m_pDistNumYellow);
	SAFEDELETE(m_pDistNumRed);
	SAFEDELETE(m_pRateNum);
	SAFEDELETE(m_pDistNone);

	m_vDistLevelVector.clear();

	if( !COMPARE( m_iDefaultX, 0, Setting::Width() ) || 
		!COMPARE( m_iDefaultY, 0, Setting::Height() ) )
		return;

	g_GUIMgr.GUIPosSaveInt( "ExcavationSearchingWnd", "XPos", m_iDefaultX ,POS_SAVE_VERSION );
	g_GUIMgr.GUIPosSaveInt( "ExcavationSearchingWnd", "YPos", m_iDefaultY ,POS_SAVE_VERSION );
}

void ExcavationSearchingWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();

	switch( dwID )
	{
	case ID_EXCAVATING_BTN:
		if( cmd == IOBN_BTNUP )
		{
			OnExcavatingBtnUp( dwID );
			g_ExcavationMgr.SetKeyInputTimer(0);
		}
		break;
	case ID_EXIT_BTN:
		if( cmd == IOBN_BTNUP )
		{
			// 서버에서 메시지를 받고 hide됨	
			if( m_pPlayStage )
			{
				ioBaseChar *pOwner = m_pPlayStage->GetOwnerChar();
				g_ExcavationMgr.OnExcavationBtnUp( pOwner );
			}
		}
	case MESSAGE_BOX3: // g_GUIMgr.SetPrevMsgListBox() OR g_GUIMgr.SetPrevMsgListBoxWithTitle()
		if( cmd == IOBN_BTNUP )
		{
			if( param == IDYES )
			{
				// 상점 특별아이템 열기
				NewShopWnd *pNewShopWnd = dynamic_cast<NewShopWnd*>( g_GUIMgr.FindWnd( NEW_SHOP_WND ) );
				if( pNewShopWnd )
				{
					pNewShopWnd->ShowWnd();
					pNewShopWnd->ShowEtcItemTab();
				}
			}
		}
		break;
	}
}

void ExcavationSearchingWnd::SetPlayStage( ioPlayStage *pPlayStage )
{
	m_pPlayStage = pPlayStage;
}

void ExcavationSearchingWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "BatteryGauge" )
	{
		SAFEDELETE( m_pBatteryGauge );
		m_pBatteryGauge = pImage;
	}
	else if( szType == "Wave1" )
	{
		SAFEDELETE( m_pWave[0] );
		m_pWave[0] = pImage;
	}
	else if( szType == "Wave2" )
	{
		SAFEDELETE( m_pWave[1] );
		m_pWave[1] = pImage;
	}
	else if( szType == "Wave3" )
	{
		SAFEDELETE( m_pWave[2] );
		m_pWave[2] = pImage;
	}
	else if( szType == "Wave4" )
	{
		SAFEDELETE( m_pWave[3] );
		m_pWave[3] = pImage;
	}
	else if( szType == "Wave5" )
	{
		SAFEDELETE( m_pWave[4] );
		m_pWave[4] = pImage;
	}
	else if( szType == "Wave6" )
	{
		SAFEDELETE( m_pWave[5] );
		m_pWave[5] = pImage;
	}
	else if( szType == "DistNumBlue" )
	{
		SAFEDELETE( m_pDistNumBlue );
		m_pDistNumBlue = pImage;
	}
	else if( szType == "DistNumYellow" )
	{
		SAFEDELETE( m_pDistNumYellow );
		m_pDistNumYellow = pImage;
	}
	else if( szType == "DistNumRed" )
	{
		SAFEDELETE( m_pDistNumRed );
		m_pDistNumRed = pImage;
	}
	else if( szType == "RateNum" )
	{
		SAFEDELETE( m_pRateNum );
		m_pRateNum = pImage;
	}
	else if( szType == "DistNone" )
	{
		SAFEDELETE( m_pDistNone );
		m_pDistNone = pImage;
	}
	else
	{
		ioMovingWnd::AddRenderImage( szType, pImage );
	}
}

void ExcavationSearchingWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	m_dwFullBatteryTime   = xElement.GetIntAttribute_e( "Full_Battery_Time" );
	m_dwMiddleBatteryTime = xElement.GetIntAttribute_e( "Middle_Battery_Time" );
	m_dwLowBatteryTime    = xElement.GetIntAttribute_e( "Low_Battery_Time" );

	m_dwArtifactPosCheckTime   = xElement.GetIntAttribute_e( "Pos_Check_Time" );
	m_dwRadarCheckTime         = xElement.GetIntAttribute_e( "Radar_Check_Time" );
	m_iWaveAddSize             = xElement.GetIntAttribute_e( "Wave_Add_Size" );
	m_dwSuccessPerCheckTime    = xElement.GetIntAttribute_e( "Success_Per_Check_Time" );

	for (int i = 0; i < MAX_SEARCHING_TYPE ; i++)
	{
		char szName[MAX_PATH]="";
		StringCbPrintf( szName, sizeof( szName ), "Wave%d_Sound", i+1 );
		m_sWaveSoundArray[i] = xElement.GetStringAttribute( szName );
	}

	//기본값
	if( m_dwArtifactPosCheckTime > 10000 )
		m_dwArtifactPosCheckTime = 10000;

	if( m_iWaveAddSize <= 0 )
		m_iWaveAddSize = 1;

	m_vDistLevelVector.clear();
	int iMax = xElement.GetIntAttribute_e( "Max_Info" );
	for (int i = 0; i < iMax ; i++)
	{
		char szName[MAX_PATH]="";
		StringCbPrintf_e( szName, sizeof( szName ), "Dist_Level_%d", i+1 );
		int iDist = xElement.GetIntAttribute( szName );
		m_vDistLevelVector.push_back( iDist );
	}

	std::sort( m_vDistLevelVector.begin(), m_vDistLevelVector.end(), DistLevelSort() );
}

void ExcavationSearchingWnd::OnRender()
{
	ioMovingWnd::OnRender();

	int iXPos, iYPos;
	iXPos = GetDerivedPosX();
	iYPos = GetDerivedPosY();

	ioUserEtcItem *pUserEtcItem = g_MyInfo.GetUserEtcItem();
	if( !pUserEtcItem )
		return;

	int iGaugeCnt  = 0;
	int iRemainSec = 0;

	ioUserEtcItem::ETCITEMSLOT kSlot;
	if( pUserEtcItem->GetEtcItem( ioEtcItem::EIT_ETC_EXCAVATING_KIT, kSlot ) )
	{
		if( kSlot.IsUse() )
			iRemainSec = kSlot.GetUse();
	}

	if( pUserEtcItem->GetEtcItem( ioEtcItem::EIT_ETC_GOLD_EXCAVATING_KIT, kSlot ) )
	{
		if( kSlot.IsUse() )
			iRemainSec = kSlot.GetUse();
	}

	if( iRemainSec > (int)m_dwFullBatteryTime )
		iGaugeCnt = 3;
	else if( iRemainSec > (int)m_dwMiddleBatteryTime )
		iGaugeCnt = 2;
	else if( iRemainSec > (int)m_dwLowBatteryTime )
		iGaugeCnt = 1;

	enum 
	{
		GAUGE_X = 17,
		GAUGE_Y = 28,

		GAUGE_GAP_X = 5,

		RADAR_X = 96,
		RADAR_Y = 63,

		MAX_RADAR_WIDTH  = 172,
		MAX_RADAR_HEIGHT = 83,

		RADAR_WIDTH      = 175,
		RADAR_HEIGHT     = 175,

		RANGE_X = 132,
		RANGE_Y = 25,

		WAVE_X  = 8,
		WAVE_Y  = 21,

		WAVE_WIDTH = 168,

		DIST_NUM_X = 116,
		DIST_NUM_Y = 46, 

		RATE_NUM_X = 149,
		RATE_NUM_Y = 23,

		BLUE_DIST  = 100,
		YELLOW_DIST= 50,

		DIST_NONE_X= 44,
		DIST_NONE_Y= 72,
	};

	if( COMPARE( m_iCurGrowWaveTypeArray, 0, MAX_SEARCHING_TYPE ) )
	{
		if( m_pWave[m_iCurGrowWaveTypeArray] ) // 성장 파형
			m_pWave[m_iCurGrowWaveTypeArray]->RenderWidthCut( iXPos + WAVE_X -m_iWaveWidthcut, iYPos + WAVE_Y , m_iWaveWidthcut, WAVE_WIDTH );
	}

 	if( COMPARE( m_iCurExtinctWaveTypeArray, 0, MAX_SEARCHING_TYPE ) )
 	{
 		if( m_pWave[m_iCurExtinctWaveTypeArray] ) // 소멸 파형
 			m_pWave[m_iCurExtinctWaveTypeArray]->RenderWidthCut( iXPos + WAVE_X + (WAVE_WIDTH-m_iWaveWidthcut), iYPos + WAVE_Y ,0, m_iWaveWidthcut );
 	}

	for (int i = 0; i < iGaugeCnt ; i++)
	{
		if( m_pBatteryGauge )
			m_pBatteryGauge->Render( iXPos + GAUGE_X + ( GAUGE_GAP_X * i ), iYPos + GAUGE_Y  );
	}

	// dist
	int iCnt   = 0;
	int iCheck = 10;
	while( iCheck <= m_iArtifactDist )
	{
		iCheck *= 10;
		iCnt++;
	}

	int iXOffset = -32*iCnt;
	if( m_iArtifactDist == -1 )
	{
		if( m_pDistNone )
			m_pDistNone->Render( iXPos + DIST_NONE_X + iXOffset, iYPos + DIST_NONE_Y );
	}
	else if( m_iArtifactDist > BLUE_DIST )
	{
		if( m_pDistNumBlue )
			m_pDistNumBlue->RenderNum( iXPos + DIST_NUM_X + iXOffset, iYPos + DIST_NUM_Y, m_iArtifactDist, FLOAT1 );
	}
	else if( m_iArtifactDist > YELLOW_DIST )
	{
		if( m_pDistNumYellow )
			m_pDistNumYellow->RenderNum( iXPos + DIST_NUM_X + iXOffset, iYPos + DIST_NUM_Y, m_iArtifactDist, FLOAT1 );
	}
	else 
	{
		if( m_pDistNumRed )
			m_pDistNumRed->RenderNum( iXPos + DIST_NUM_X + iXOffset, iYPos + DIST_NUM_Y, m_iArtifactDist, FLOAT1 );
	}
	

	// rate
	iCnt      = 0;
	iCheck    = 10;
	while( iCheck <= m_iCurSuccessPercent )
	{
		iCheck *= 10;
		iCnt++;
	}

	iXOffset = -10*iCnt;
	if( m_pRateNum )
		m_pRateNum->RenderNum( iXPos + RATE_NUM_X + iXOffset, iYPos + RATE_NUM_Y, m_iCurSuccessPercent, -FLOAT1 );
}

void ExcavationSearchingWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() )
		return ;
	ioMovingWnd::OnProcess( fTimePerSec );
	ProcessArtifactsPos();
	ProcessRadar();
	UpdateDefaultPos();
	ProcessSuccessPercent();
	ProcessToolTip();
}

void ExcavationSearchingWnd::ProcessArtifactsPos()
{
	if( REALGETTIME() - m_dwCurArtifactPosCheckTime < m_dwArtifactPosCheckTime )
		return;
	m_dwCurArtifactPosCheckTime = REALGETTIME();

	if( !m_pPlayStage )
		return;

	if( !m_bProcessRadarPos )
		return;

	ioBaseChar *pOwner = m_pPlayStage->GetOwnerChar();
    if( !pOwner )
		return;

	IntVec vIntVector;
	vIntVector.reserve(10);
	int iSize = g_ExcavationMgr.GetArtifactSize();
	for (int i = 0; i < iSize ; i++)
	{
		D3DXVECTOR3 vArtifact;
		int iX = 0;
		int iY = 0;
		int iZ = 0;
		g_ExcavationMgr.GetArtifactPos( i, iX, iY, iZ );
		vArtifact.x = (float)iX;
		vArtifact.y = (float)iY;
		vArtifact.z = (float)iZ;

		// 수평 거리 체크
		D3DXVECTOR3 vDiff1 = vArtifact - pOwner->GetWorldPosition();
		vDiff1.y = 0.0f;

		float fDist = D3DXVec3Length( &vDiff1 );
		vIntVector.push_back( (int) fDist );
	}
	
	// 낮은 순으로 정렬
	std::sort( vIntVector.begin(), vIntVector.end() );

	if( vIntVector.empty() )
	{
		m_iRangeTypeArray = 0;
		m_iArtifactDist   = -1;
		return;
	}

	int iSmallestDist = vIntVector[0];

	enum { MAX_DIST = 9999, };
	m_iArtifactDist = iSmallestDist/FLOAT100;
	if( m_iArtifactDist > MAX_DIST )
		m_iArtifactDist = MAX_DIST;

	iSize = m_vDistLevelVector.size();
	for (int i = 0; i < iSize ; i++)
	{
		if( iSmallestDist > m_vDistLevelVector[i] )
		{
			m_iRangeTypeArray = i;
			return;
		}
	}
}

void ExcavationSearchingWnd::ProcessSuccessPercent()
{
	if( REALGETTIME() - m_dwCurSuccessPerCheckTime < m_dwSuccessPerCheckTime )
		return;
	m_dwCurSuccessPerCheckTime = REALGETTIME();

	if( !m_pPlayStage )
		return;

	int iCnt = 0;
	const BaseCharList &rkTargetList = m_pPlayStage->GetBaseCharList();
	BaseCharList::const_iterator iter;
	for( iter = rkTargetList.begin(); iter != rkTargetList.end(); ++iter )
	{
		ioBaseChar *pChar = *iter;
		if( !pChar ) 
			continue;

		ioExcavationItem *pItem = ToExcavationItem( pChar->GetObject() );
		if( !pItem )
			continue;

		DWORD dwPassedTime = REALGETTIME() - pItem->GetEquipTime();
		if( dwPassedTime >= (DWORD)g_ExcavationMgr.GetRealExcavatingUserTime() )
			iCnt++;
	}

	m_iCurSuccessPercent = iCnt * g_ExcavationMgr.GetSuccessRatePerOneUser();
	if( m_iCurSuccessPercent > g_ExcavationMgr.GetMaxSuccessRate() )
		m_iCurSuccessPercent = g_ExcavationMgr.GetMaxSuccessRate();
}

void ExcavationSearchingWnd::ProcessRadar()
{
	if( FRAMEGETTIME() - m_dwCurRadarCheckTime < m_dwRadarCheckTime  )
		return;
	m_dwCurRadarCheckTime = FRAMEGETTIME();

	enum { WAVE_WIDTH = 168, };
	m_iWaveWidthcut-=m_iWaveAddSize;
	if( m_iWaveWidthcut < 0 )
		m_iWaveWidthcut = WAVE_WIDTH;

	if( !m_bProcessRadarPos )
		return;

	if( m_iWaveWidthcut == WAVE_WIDTH )
	{
		m_iCurGrowWaveTypeArray = m_iRangeTypeArray; 

		if( !m_sCurWaveSound.IsEmpty() )
			g_SoundMgr.StopSound( m_sCurWaveSound, 0 );

		if( COMPARE( m_iCurGrowWaveTypeArray, 0, MAX_SEARCHING_TYPE ) )
		{
			m_sCurWaveSound = m_sWaveSoundArray[m_iCurGrowWaveTypeArray];
			if( !m_sCurWaveSound.IsEmpty() )
				g_SoundMgr.PlaySound( m_sCurWaveSound, DSBVOLUME_MAX, PSM_LOOP | PSM_THREAD );
		}
	}

	if( COMPARE( m_iWaveWidthcut , 0, 10 ) )
		m_iCurExtinctWaveTypeArray = m_iCurGrowWaveTypeArray;
}

void ExcavationSearchingWnd::UpdateDefaultPos()
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

void ExcavationSearchingWnd::iwm_create()
{
	int iXPos = g_GUIMgr.GUIPosLoadInt( "ExcavationSearchingWnd", "XPos", GetXPos() ,POS_SAVE_VERSION );
	int iYPos = g_GUIMgr.GUIPosLoadInt( "ExcavationSearchingWnd", "YPos", GetYPos() ,POS_SAVE_VERSION );

	if( COMPARE( iXPos, 0, Setting::Width() ) && 
		COMPARE( iYPos, 0, Setting::Height() ) )
	{
		SetWndPos( iXPos, iYPos );
	}

	m_iDefaultX = GetDerivedPosX();
	m_iDefaultY = GetDerivedPosY();
}

void ExcavationSearchingWnd::SetPositionToDefault( int iParentWidth, int iParentHeight, bool bChildMove, bool bCreate /*= false */ )
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

void ExcavationSearchingWnd::iwm_show()
{
	ShowChildWnd( ID_EXCAVATING_BTN );
	m_iWaveWidthcut = 0;
	m_sCurWaveSound.Clear();
	m_iRangeTypeArray       = 0;
	m_iCurGrowWaveTypeArray = 0;
	m_iCurExtinctWaveTypeArray = 0;
	m_iArtifactDist            = 0;
	m_iCurSuccessPercent       = 0;
	m_bProcessRadarPos         = true;

	UpdateExp();
}

void ExcavationSearchingWnd::iwm_hide()
{
	HideChildWnd( ID_EXCAVATING_BTN );

	if( !m_sCurWaveSound.IsEmpty() )
		g_SoundMgr.StopSound( m_sCurWaveSound, 0 );

	for (int i = 0; i < MAX_SEARCHING_TYPE ; i++)
	{
		if( !m_sWaveSoundArray[i].IsEmpty() )
		g_SoundMgr.StopSound( m_sWaveSoundArray[i], 0 );
	}
}

void ExcavationSearchingWnd::ClearExcavatingCurCoolTime()
{
	ExcavatingBtn *pBtn = dynamic_cast<ExcavatingBtn*> ( FindChildWnd( ID_EXCAVATING_BTN ) );
	if( pBtn )
		pBtn->ClearCurCoolTime();
}

void ExcavationSearchingWnd::OnExcavatingBtnUp( DWORD dwID )
{
	ExcavatingBtn *pBtn = dynamic_cast<ExcavatingBtn*> ( FindChildWnd( dwID ) );
	if( pBtn )
	{
		if( pBtn->IsCanExcavating() )
		{
			if( m_pPlayStage )
			{
				ioBaseChar *pOwner = m_pPlayStage->GetOwnerChar();
				if( g_ExcavationMgr.SetExcavating( pOwner ) )
					pBtn->InitCoolTime();
			}
		}
	}
}

void ExcavationSearchingWnd::UpdateExp()
{
	ExcavationExpWnd* pExp = dynamic_cast<ExcavationExpWnd*> ( FindChildWnd( ID_EXP_WND ) );
	if( !pExp ) return;

	pExp->SetInfo( g_MyInfo.GetExcavationLevel(), g_MyInfo.GetExcavationExpert() );
}

void ExcavationSearchingWnd::ProcessToolTip()
{
	DWORD dwCurOverID = 0;
	if( g_GUIMgr.GetPreOverWnd() )
	{
		if( g_GUIMgr.GetPreOverWnd()->GetParent() == this )
			dwCurOverID = g_GUIMgr.GetPreOverWnd()->GetID();
	}

	ioComplexStringPrinter kPrinter[GLOBAL_TEXT_TOOLTIP_LINE];

	if( ID_EXP_WND == dwCurOverID )
	{
		kPrinter[0].SetTextStyle( TS_NORMAL );
		kPrinter[0].SetBkColor( 0, 0, 0 );
		kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
		kPrinter[0].AddTextPiece( FONT_SIZE_12, STR(1), g_MyInfo.GetExcavationExpert(), g_LevelMgr.GetNextExcavationLevelUpExp( g_MyInfo.GetExcavationLevel() ) );
	}

	GlobalTextTooltip *pTooltip = dynamic_cast<GlobalTextTooltip*>(FindChildWnd( ID_TOOL_TIP ));
	if( pTooltip )
	{
		pTooltip->SetTooltipText( kPrinter );
	}
}
//////////////////////////////////////////////////////////////////////////
ExcavationBuyPopup::ExcavationBuyPopup()
{
	m_pIconBack = NULL;
	m_pItemIcon = NULL;
}

ExcavationBuyPopup::~ExcavationBuyPopup()
{
	SAFEDELETE( m_pIconBack );
	SAFEDELETE( m_pItemIcon );
}

void ExcavationBuyPopup::iwm_show()
{
	ioEtcItem *pEtcItem = g_EtcItemMgr.FindEtcItem( ioEtcItem::EIT_ETC_EXCAVATING_KIT );
	if( !pEtcItem )
	{
		HideWnd();
		return;
	}

	m_szItemName = pEtcItem->GetName();

	SAFEDELETE( m_pItemIcon );
	m_pItemIcon = g_UIImageSetMgr.CreateImageByFullName( pEtcItem->GetIconName() );
}

void ExcavationBuyPopup::iwm_hide()
{

}

void ExcavationBuyPopup::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_EXIT:
	case ID_CANCEL:
		if( cmd == IOBN_BTNUP )
			HideWnd();
		break;
	case ID_SHOP:
		if( cmd == IOBN_BTNUP )
		{
			// 상점 특별아이템 열기
			NewShopWnd *pNewShopWnd = dynamic_cast<NewShopWnd*>( g_GUIMgr.FindWnd( NEW_SHOP_WND ) );
			if( pNewShopWnd )
			{
				pNewShopWnd->ShowWnd();
				pNewShopWnd->SetDirectTab( NewShopWnd::ID_ETC_TAB_BTN, ioEtcItem::EIT_ETC_EXCAVATING_KIT, 0 );
				HideWnd();
			}
		}
		break;		
	}
}

void ExcavationBuyPopup::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "IconBack" )
	{
		SAFEDELETE( m_pIconBack );
		m_pIconBack = pImage;
	}
	else
	{
		ioWnd::AddRenderImage( szType, pImage );
	}
}

void ExcavationBuyPopup::OnRender()
{
	ioWnd::OnRender();
	
	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();
	if( m_pIconBack && m_pItemIcon )
	{
		m_pIconBack->Render( iXPos + 151, iYPos + 98, UI_RENDER_MULTIPLY );
		m_pItemIcon->Render( iXPos + 151, iYPos + 98 );
	}

	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
	g_FontMgr.PrintText( iXPos + 151, iYPos + 144, FONT_SIZE_13, STR(1), m_szItemName.c_str() );

	g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
	g_FontMgr.PrintText( iXPos + 151, iYPos + 164, FONT_SIZE_13, STR(2) );
	g_FontMgr.PrintText( iXPos + 151, iYPos + 184, FONT_SIZE_13, STR(3) );
}

