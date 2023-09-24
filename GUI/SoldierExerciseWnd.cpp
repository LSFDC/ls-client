#include "StdAfx.h"

#include "../io3DEngine/ioWnd.h"
#include "../io3DEngine/ioUIRenderImage.h"
#include "../io3DEngine/ioUIImageSetManager.h"
#include "../io3DEngine/ioXMLElement.h"
#include "../io3DEngine/ioFontManager.h"
#include "../ioVoiceChat/Fourier.h"
#include "../io3DEngine/HelpFunc.h"
#include "../io3DEngine/iostringmanager.h"
#include "../io3dengine/safesprintf.h"

#include "../ioItemMaker.h"
#include "../ioSetItemInfo.h"
#include "../ioSetItemInfoManager.h"
#include "../NetworkWrappingFunc.h"
#include "../ioMyInfo.h"
#include "../ioApplication.h"
#include "../ioPlayStage.h"
#include "../ioPlayMode.h"
#include "../ioBaseChar.h"
#include "../ioClassPrice.h"
#include "../WndID.h"
#include "../Setting.h"
#include "../ioMyLevelMgr.h"
#include "../TextColorTable.h"
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
#include "../ioLuciferItem.h"
#include "../ioNakoruruItem.h"
#include "../ioPeterpanItem.h"
#include "../ioWindLordItem.h"
#include "../ioLeeItem.h"
#include "../ioProject_K_Item.h"
#include "../ioHakumenItem.h"
#include "../ioDevilItem.h"
#include "../ioMaidItem.h"
#include "../ioFlash_Item.h"
#include "../ioPsychoMetry_Item.h"
#include "../ioSuddenAttackItem.h"
#include "../ioSlasher_Item.h"
#include "../ioDummyCharItem2.h"
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
#include "SoldierManualWnd.h"
#include "SoldierSelectWnd.h"
#include "SoldierLimitWnd.h"

#include "SoldierExerciseWnd.h"
#include "ioMessageBox.h"
#include "ioTutorialManager.h"
#include "NewShopWnd.h"

#include <strsafe.h>
#include "../TextColorTable.h"

SoldierExerciseBtn::SoldierExerciseBtn()
{
	m_bForceOver   = false;
	m_eBoughtType  = SBT_NONE;
	m_iClassType   = -1;
	m_iNeedLevel = -1;
	m_pLock		   = NULL;

	m_pArrow	  = NULL;
	m_bArrow      = false;
	m_iDownHeight = 0;

	m_bScreenAction = false;
	m_bScreenSoldier= false;
	m_dwScreenTime  = 500;
	m_dwScreenStartTime = 0;

	m_eNeedLevelType    = ioSetItemInfo::NLT_NONE;
	m_pSoldierIcon      = NULL;
	m_pLockNum          = NULL;
	m_pLockBattleMark   = NULL;
	m_pLockMedalMark	= NULL;
	m_pLockAwardMark    = NULL;
	m_pBoughtType		= NULL;
	m_pExerciseGeneral  = NULL;
	m_pExercisePcroom   = NULL;
	m_pExerciseEvent    = NULL;
	m_pNewMark          = NULL;
	m_pNewEffect        = NULL;
	m_pHitMark          = NULL;
	m_pHitEffect        = NULL;
	m_pSaleMark          = NULL;
	m_pSaleEffect        = NULL;
	m_pEventMark         = NULL;
	m_pEventEffect       = NULL;
	m_pIconBack          = NULL;

	m_dwNewEffectWaitTime = 0;
	m_dwNewEffectCurTime  = 0;
	m_iMarkType           = 0; // ORDER_DEFAULT
	m_bMarkEffect         = false;
}

SoldierExerciseBtn::~SoldierExerciseBtn()
{
	SAFEDELETE( m_pSoldierIcon );
	SAFEDELETE( m_pLock );
	SAFEDELETE( m_pArrow );

	SAFEDELETE( m_pLockNum );
	SAFEDELETE( m_pLockBattleMark );
	SAFEDELETE( m_pLockMedalMark );
	SAFEDELETE( m_pLockAwardMark );
	SAFEDELETE( m_pBoughtType );
	SAFEDELETE( m_pExerciseGeneral );
	SAFEDELETE( m_pExercisePcroom );
	SAFEDELETE( m_pExerciseEvent );
	SAFEDELETE( m_pNewMark );
	SAFEDELETE( m_pNewEffect );
	SAFEDELETE( m_pHitMark );
	SAFEDELETE( m_pHitEffect );
	SAFEDELETE( m_pSaleMark );
	SAFEDELETE( m_pSaleEffect );
	SAFEDELETE( m_pEventMark );
	SAFEDELETE( m_pEventEffect );
	SAFEDELETE( m_pIconBack );
}

void SoldierExerciseBtn::SetSoldierIcon( ioUIRenderImage *pSoldierIcon, float fIconScale, bool bScreenAction )
{
	SAFEDELETE( m_pSoldierIcon );
	m_pSoldierIcon = pSoldierIcon;

	if( m_pSoldierIcon )
		m_pSoldierIcon->SetScale( fIconScale );

	m_bScreenAction = bScreenAction;
	m_bScreenSoldier= false;
	m_dwScreenTime  = 500;
	m_dwScreenStartTime = 0;
}

void SoldierExerciseBtn::SetBoughtType( SoldierBoughtType eBoughtType )
{
	m_eBoughtType = eBoughtType;
}

void SoldierExerciseBtn::OnDrawNormal( int iXPos, int iYPos )
{
	if( m_bForceOver )
		ioButton::OnDrawOvered( iXPos, iYPos );
	else
		ioButton::OnDrawNormal( iXPos, iYPos );
}

void SoldierExerciseBtn::RenderImageList( int iXPos, int iYPos, UIRenderType eRenderType )
{
	UIElementList::iterator iter;
	for( iter=m_ElementList.begin() ; iter!=m_ElementList.end() ; ++iter )
	{
		(*iter)->Render( iXPos, iYPos, eRenderType , TFO_BILINEAR );
	}
}

void SoldierExerciseBtn::OnRender()
{
	ioButton::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	RenderSoldierIcon( iXPos, iYPos );
	RenderNewMark( iXPos, iYPos );
	RenderType( iXPos, iYPos );
	RenderArrow( iXPos, iYPos );
	RenderNeedLv( iXPos, iYPos );
}

void SoldierExerciseBtn::RenderNewMark( int iXPos, int iYPos )
{
	enum 
	{ 
		X_NEW_OFFSET   = 23,
		Y_NEW_OFFSET   = 2,
	};

	// new mark
	if( m_iMarkType == SHOP_MARK_TYPE_NEW )
	{
		if( m_pNewMark )
			m_pNewMark->Render( iXPos + X_NEW_OFFSET, iYPos + Y_NEW_OFFSET, UI_RENDER_NORMAL, TFO_BILINEAR );

		if( m_bMarkEffect )
		{
			if( m_pNewEffect )
				m_pNewEffect->Render( iXPos + X_NEW_OFFSET, iYPos + Y_NEW_OFFSET, UI_RENDER_SCREEN, TFO_BILINEAR );
		}
	}
	else if( m_iMarkType == SHOP_MARK_TYPE_HIT )
	{
		if( m_pHitMark )
			m_pHitMark->Render( iXPos + X_NEW_OFFSET, iYPos + Y_NEW_OFFSET, UI_RENDER_NORMAL, TFO_BILINEAR );

		if( m_bMarkEffect )
		{
			if( m_pHitEffect )
				m_pHitEffect->Render( iXPos + X_NEW_OFFSET, iYPos + Y_NEW_OFFSET, UI_RENDER_SCREEN, TFO_BILINEAR );
		}
	}
	else if( m_iMarkType == SHOP_MARK_TYPE_SALE )
	{
		if( m_pSaleMark )
			m_pSaleMark->Render( iXPos + X_NEW_OFFSET, iYPos + Y_NEW_OFFSET, UI_RENDER_NORMAL, TFO_BILINEAR );

		if( m_bMarkEffect )
		{
			if( m_pSaleEffect )
				m_pSaleEffect->Render( iXPos + X_NEW_OFFSET, iYPos + Y_NEW_OFFSET, UI_RENDER_SCREEN, TFO_BILINEAR );
		}
	}
	else if( m_iMarkType == SHOP_MARK_TYPE_EVENT )
	{
		if( m_pEventMark )
			m_pEventMark->Render( iXPos + X_NEW_OFFSET, iYPos + Y_NEW_OFFSET, UI_RENDER_NORMAL, TFO_BILINEAR );

		if( m_bMarkEffect )
		{
			if( m_pEventEffect )
				m_pEventEffect->Render( iXPos + X_NEW_OFFSET, iYPos + Y_NEW_OFFSET, UI_RENDER_SCREEN, TFO_BILINEAR );
		}
	}
}

void SoldierExerciseBtn::RenderNeedLv( int iXPos, int iYPos )
{
	if( m_eBoughtType != SBT_NONE ) return;
	if( m_eNeedLevelType == ioSetItemInfo::NLT_NONE ) return;

	enum { LOCK_OFFSET_X = 18, 
		LOCK_OFFSET_Y = 12, 

		GRADE_OFFSET_X = 21, 
		GRADE_OFFSET_Y = 22, 

		X_MARK_OFFSET  = 24,
		Y_MARK_OFFSET  = 20,

		X_LOCK_NUM_OFFSET = 31,
		Y_LOCK_NUM_OFFSET = 34,

		X_LOCK_ONE_NUM_OFFSET = 32,
	};

	if( m_pLock )
		m_pLock->Render( iXPos + LOCK_OFFSET_X, iYPos + LOCK_OFFSET_Y, UI_RENDER_NORMAL );

	if( m_eNeedLevelType != ioSetItemInfo::NLT_GRADE )
	{
		if( m_pLockNum )
		{
			m_pLockNum->SetHorzFormatting( ioUIRenderImage::HF_CENTER );
			int iXOffset = X_LOCK_NUM_OFFSET;
			if( ( m_iNeedLevel / 10 ) == 0 ) // 1자리
				iXOffset = X_LOCK_ONE_NUM_OFFSET;
			m_pLockNum->RenderNum( iXPos + iXOffset, iYPos + Y_LOCK_NUM_OFFSET , m_iNeedLevel, -FLOAT1, FLOAT1, UI_RENDER_NORMAL );
		}
	}

	if( m_eNeedLevelType == ioSetItemInfo::NLT_GRADE )
	{
		g_LevelMgr.RenderGrade( iXPos + GRADE_OFFSET_X, iYPos + GRADE_OFFSET_Y, m_iNeedLevel, TEAM_PRIVATE_1 , UI_RENDER_NORMAL);
	}
	else if( m_eNeedLevelType == ioSetItemInfo::NLT_BATTLE )
	{
		if( m_pLockBattleMark )
			m_pLockBattleMark->Render( iXPos + X_MARK_OFFSET, iYPos + Y_MARK_OFFSET, UI_RENDER_NORMAL );
	}
	else if( m_eNeedLevelType == ioSetItemInfo::NLT_AWARD )
	{
		if( m_pLockAwardMark )
			m_pLockAwardMark->Render( iXPos + X_MARK_OFFSET, iYPos + Y_MARK_OFFSET, UI_RENDER_NORMAL );
	}
}

void SoldierExerciseBtn::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "Lock" )
	{
		SAFEDELETE( m_pLock );
		m_pLock = pImage;
	}
	else if( szType == "Arrow" )
	{
		SAFEDELETE( m_pArrow );
		m_pArrow = pImage;
		if( m_pArrow )
			m_pArrow->SetScale( ARROW_SCALE );
	}
	else if( szType == "LockNum" )
	{
		SAFEDELETE( m_pLockNum );
		m_pLockNum = pImage;
	}
	else if( szType == "LockBattleMark" )
	{
		SAFEDELETE( m_pLockBattleMark );
		m_pLockBattleMark = pImage;
	}
	else if( szType == "MedalMark" )
	{
		SAFEDELETE( m_pLockMedalMark );
		m_pLockMedalMark = pImage;
	}
	else if( szType == "LockAwardMark" )
	{
		SAFEDELETE( m_pLockAwardMark );
		m_pLockAwardMark = pImage;
	}
	else if( szType == "BoughtType" )
	{
		SAFEDELETE( m_pBoughtType );
		m_pBoughtType = pImage;
	}
	else if( szType == "ExerciseGeneral" )
	{
		SAFEDELETE( m_pExerciseGeneral );
		m_pExerciseGeneral = pImage;
	}
	else if( szType == "ExercisePcroom" )
	{
		SAFEDELETE( m_pExercisePcroom );
		m_pExercisePcroom = pImage;
	}
	else if( szType == "NewMark" )
	{
		SAFEDELETE( m_pNewMark );
		m_pNewMark = pImage;
		if( m_pNewMark )
			m_pNewMark->SetScale( NEW_SCALE );
	}
	else if( szType == "NewEffect" )
	{
		SAFEDELETE( m_pNewEffect );
		m_pNewEffect = pImage;
		if( m_pNewEffect )
			m_pNewEffect->SetScale( NEW_SCALE );
	}
	else if( szType == "HitMark" )
	{
		SAFEDELETE( m_pHitMark );
		m_pHitMark = pImage;
		if( m_pHitMark )
			m_pHitMark->SetScale( NEW_SCALE );
	}
	else if( szType == "HitEffect" )
	{
		SAFEDELETE( m_pHitEffect );
		m_pHitEffect = pImage;
		if( m_pHitEffect )
			m_pHitEffect->SetScale( NEW_SCALE );
	}
	else if( szType == "SaleMark" )
	{
		SAFEDELETE( m_pSaleMark );
		m_pSaleMark = pImage;
		if( m_pSaleMark )
			m_pSaleMark->SetScale( NEW_SCALE );
	}
	else if( szType == "SaleEffect" )
	{
		SAFEDELETE( m_pSaleEffect );
		m_pSaleEffect = pImage;
		if( m_pSaleEffect )
			m_pSaleEffect->SetScale( NEW_SCALE );
	}
	else if( szType == "EventMark" )
	{
		SAFEDELETE( m_pEventMark );
		m_pEventMark = pImage;
		if( m_pEventMark )
			m_pEventMark->SetScale( NEW_SCALE );
	}
	else if( szType == "EventEffect" )
	{
		SAFEDELETE( m_pEventEffect );
		m_pEventEffect = pImage;
		if( m_pEventEffect )
			m_pEventEffect->SetScale( NEW_SCALE );
	}
	else if( szType == "ExerciseEvent" )
	{
		SAFEDELETE( m_pExerciseEvent );
		m_pExerciseEvent = pImage;
	}
	else if( szType == "IconBack" )
	{
		SAFEDELETE( m_pIconBack );
		m_pIconBack = pImage;
		if( m_pIconBack )
			m_pIconBack->SetScale( NEW_SCALE );
	}
	else
		ioButton::AddRenderImage( szType, pImage );
}

void SoldierExerciseBtn::ParseExtraInfo( ioXMLElement &xElement )
{
	ioButton::ParseExtraInfo( xElement );
	m_bArrow                = xElement.GetBoolAttribute_e( "UseArrow" );
	m_iDownHeight           = xElement.GetIntAttribute_e( "DownHeight" );
	m_dwNewEffectWaitTime   = xElement.GetIntAttribute_e( "NewEffectWaitTime" );
}

void SoldierExerciseBtn::RenderSoldierIcon( int iXPos, int iYPos )
{
	if( !m_pSoldierIcon ) return;

	if( HasWndStyle( IWS_INACTIVE ) )
		m_pSoldierIcon->Render( iXPos + GetWidth() / 2, iYPos + GetHeight() / 2, UI_RENDER_GRAY, TFO_BILINEAR );
	else
	{
		if( m_pIconBack )
			m_pIconBack->Render( iXPos + GetWidth() / 2, iYPos + GetHeight() / 2, UI_RENDER_MULTIPLY, TFO_BILINEAR );

		m_pSoldierIcon->Render( iXPos + GetWidth() / 2, iYPos + GetHeight() / 2, UI_RENDER_NORMAL, TFO_BILINEAR );
		if( m_bClicked || m_bOver || m_bScreenSoldier )
			m_pSoldierIcon->Render( iXPos + GetWidth() / 2, iYPos + GetHeight() / 2, UI_RENDER_SCREEN, TFO_BILINEAR );
	}
}

void SoldierExerciseBtn::RenderType( int iXPos, int iYPos )
{
	if( m_eBoughtType == SBT_NONE ) return;

	iXPos += GetWidth() / 2;
	iYPos += GetHeight() / 2;
	if( m_eBoughtType == SBT_BOUGHT )
	{
		if( m_pBoughtType )
			m_pBoughtType->Render( iXPos, iYPos );
	}
	else if( m_eBoughtType == SBT_EXERCISE_GENERAL )
	{
		if( m_pExerciseGeneral )
			m_pExerciseGeneral->Render( iXPos, iYPos );
	}
	else if( m_eBoughtType == SBT_EXERCISE_PCROOM )
	{
		if( m_pExercisePcroom )
			m_pExercisePcroom->Render( iXPos, iYPos );
	}
	else if( m_eBoughtType == SBT_EXERCISE_EVENT )
	{
		if( m_pExerciseEvent )
			m_pExerciseEvent->Render( iXPos, iYPos );
	}
}

void SoldierExerciseBtn::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;

	UpdateAniState();

	if( m_pSoldierIcon )
	{
		if( m_bScreenAction && 
			m_eBoughtType != SBT_BOUGHT && 
			m_eBoughtType != SBT_EXERCISE_PCROOM && 
			m_eBoughtType != SBT_EXERCISE_EVENT  && 
			!HasWndStyle( IWS_INACTIVE ) && 
			m_eNeedLevelType == ioSetItemInfo::NLT_NONE )
		{
			if( m_dwScreenStartTime == 0 )
				m_dwScreenStartTime = FRAMEGETTIME();

			DWORD dwGapTime = FRAMEGETTIME() - m_dwScreenStartTime;
			if( dwGapTime > m_dwScreenTime )
			{
				m_bScreenSoldier = !m_bScreenSoldier;
				m_dwScreenStartTime = FRAMEGETTIME();
			}
		}
	}

	ProcessNewEffect();
}

void SoldierExerciseBtn::InitAniState()
{
	enum { DELAY_START_DOWN = 100, DELAY_START_UP_STOP = 100, DELAY_DOWN_UP = 150, DELAY_OUT = 100,};

	FSMState *pState = new FSMState(AT_NONE);
	if( pState )
		pState->Add( AT_START_DOWN,  AT_START_DOWN );
	m_AniState.Add( pState );

	pState = new FSMState(AT_START_DOWN);
	if( pState )
	{
		pState->Add( AT_START_DOWN, AT_START_UP, DELAY_START_DOWN );
		pState->Add( AT_WHITE_OUT,  AT_WHITE_OUT );
	}
	m_AniState.Add( pState );	

	pState = new FSMState(AT_START_UP);
	if( pState )
	{
		pState->Add( AT_START_UP, AT_START_STOP, DELAY_START_UP_STOP );
		pState->Add( AT_WHITE_OUT,  AT_WHITE_OUT );
	}
	m_AniState.Add( pState );	

	pState = new FSMState(AT_START_STOP);
	if( pState )
	{
		pState->Add( AT_START_STOP, AT_DOWN, DELAY_START_UP_STOP );
		pState->Add( AT_WHITE_OUT,  AT_WHITE_OUT );
	}
	m_AniState.Add( pState );	

	pState = new FSMState(AT_DOWN);
	if( pState )
	{
		pState->Add( AT_DOWN, AT_UP, DELAY_DOWN_UP );
		pState->Add( AT_WHITE_OUT,  AT_WHITE_OUT );
		pState->Add( AT_NONE,  AT_NONE );

	}
	m_AniState.Add( pState );	

	pState = new FSMState(AT_UP);
	if( pState )
	{
		pState->Add( AT_UP, AT_DOWN, DELAY_DOWN_UP);
		pState->Add( AT_WHITE_OUT,  AT_WHITE_OUT );
		pState->Add( AT_NONE,  AT_NONE );
	}
	m_AniState.Add( pState );	

	pState = new FSMState(AT_WHITE_OUT);
	if( pState )
	{
		pState->Add( AT_WHITE_OUT, AT_MOUSE_OVER, DELAY_OUT);
		pState->Add( AT_MOUSE_LEAVE, AT_START_DOWN );
	}
	m_AniState.Add( pState );	

	pState = new FSMState(AT_MOUSE_OVER);
	if( pState )
		pState->Add( AT_MOUSE_LEAVE,  AT_START_DOWN );
	m_AniState.Add( pState );	

	m_AniState.SetCurState( AT_NONE );
}

void SoldierExerciseBtn::UpdateAniState()
{
	if( !m_bArrow ) return;
	if( m_eBoughtType != SBT_NONE ) return;
	if( !g_TutorialMgr.IsTutorial() ) 
	{
		if( m_AniState.GetCurState() != AT_NONE )
			m_AniState.SetCurState( AT_NONE );
		return;
	}

	m_AniState.UpdateTime();

	if( g_TutorialMgr.IsShowExerciseStep() && !HasWndStyle( IWS_INACTIVE ) )
		m_AniState.ChangeState( AT_START_DOWN );
	else
		m_AniState.SetCurState( AT_NONE );
}

void SoldierExerciseBtn::RenderArrow( int iXPos, int iYPos )
{
	if( !m_pArrow ) return;
	if( m_eBoughtType != SBT_NONE ) return;
	if( m_AniState.GetCurState() == AT_NONE || m_AniState.GetCurState() == AT_MOUSE_OVER ) return;

	enum { X_OFFSET = -5, Y_OFFSET_DOWN = -48, Y_OFFSET_UP = -54,};

	if( m_AniState.GetCurState() == AT_START_DOWN )
	{
		enum { HEIGHT_DOWN = 114, START_Y_OFFSET = -176, };

		float fRate = ( (float) m_AniState.GetCurTime() / (float) m_AniState.GetDelay() );
		int iYOffSet = START_Y_OFFSET + ( fRate * HEIGHT_DOWN );

		m_pArrow->SetAlpha((BYTE) 255.0f * fRate );
		m_pArrow->SetColor( 255, 255, 255 );
		m_pArrow->Render( iXPos + X_OFFSET, iYPos + iYOffSet, UI_RENDER_NORMAL, TFO_BILINEAR );
	}
	else if( m_AniState.GetCurState() == AT_START_UP )
	{
		m_pArrow->SetAlpha( 255 );
		m_pArrow->SetColor( 255, 255, 255 );
		m_pArrow->Render( iXPos + X_OFFSET, iYPos + Y_OFFSET_UP, UI_RENDER_NORMAL, TFO_BILINEAR );
	}
	else if( m_AniState.GetCurState() == AT_START_STOP )
	{
		m_pArrow->SetAlpha( 255 );
		m_pArrow->SetColor( 255, 255, 255 );
		m_pArrow->Render( iXPos + X_OFFSET, iYPos + Y_OFFSET_DOWN, UI_RENDER_NORMAL, TFO_BILINEAR );
	}
	else if( m_AniState.GetCurState() == AT_WHITE_OUT )
	{
		float fRate = ( (float)m_AniState.GetCurTime() / (float)m_AniState.GetDelay() );
		int iColor = 255.0f * fRate;
		m_pArrow->SetColor(iColor, iColor, iColor );
		int iAlpha = 255.0f * ( FLOAT1 - fRate );
		m_pArrow->SetAlpha( iAlpha );
		m_pArrow->Render( iXPos + X_OFFSET, iYPos + Y_OFFSET_DOWN, UI_RENDER_NORMAL_ADD_COLOR, TFO_BILINEAR );

	}
	else if( m_AniState.GetCurState() == AT_DOWN )
	{
		// 가속 2->1->0
		int iYOffSet = cos( (FLOAT_PI * FLOAT05 * m_AniState.GetCurTime()  ) / m_AniState.GetDelay() ) * m_iDownHeight;	
		m_pArrow->SetAlpha( 255 );
		m_pArrow->SetColor( 255, 255, 255 );
		m_pArrow->Render( iXPos + X_OFFSET, iYPos + ( Y_OFFSET_DOWN - iYOffSet ), UI_RENDER_NORMAL, TFO_BILINEAR);
	}
	else if( m_AniState.GetCurState() == AT_UP )
	{
		// 감속 0->1->2
		int iYOffSet = sin( (FLOAT_PI * FLOAT05 * m_AniState.GetCurTime()  ) / m_AniState.GetDelay() ) * m_iDownHeight;	
		m_pArrow->SetAlpha( 255 );
		m_pArrow->SetColor( 255, 255, 255 );
		m_pArrow->Render( iXPos + X_OFFSET, iYPos + ( Y_OFFSET_DOWN - iYOffSet ), UI_RENDER_NORMAL, TFO_BILINEAR );
	}
}

void SoldierExerciseBtn::iwm_create()
{
	if( m_bArrow )
		InitAniState();
}

void SoldierExerciseBtn::iwm_mouseleave( const ioMouse& mouse )
{
	ioWnd::iwm_mouseleave( mouse );
}

void SoldierExerciseBtn::iwm_mouseover( const ioMouse& mouse )
{
	ioButton::iwm_mouseover( mouse );
}

void SoldierExerciseBtn::SetNeedTypeLv( ioSetItemInfo::NeedLevelType eType, int iNeedLv )
{
	m_eNeedLevelType = eType;
	m_iNeedLevel     = iNeedLv;
}

void SoldierExerciseBtn::ProcessNewEffect()
{
	if( m_iMarkType == SHOP_MARK_TYPE_NONE )
		return;

	if( m_dwNewEffectCurTime == 0 )
	{
		m_dwNewEffectCurTime = FRAMEGETTIME();
		return;
	}

	if( FRAMEGETTIME() - m_dwNewEffectCurTime < m_dwNewEffectWaitTime )
		return;
	m_dwNewEffectCurTime = FRAMEGETTIME();

	m_bMarkEffect = !m_bMarkEffect;
}

//////////////////////////////////////////////////////////////////////////////////////////

SoldierExerciseWnd::SoldierExerciseWnd()
{
	m_iExerciseStyle     = EXERCISE_GENERAL;
	m_iCurPage			 = 0;
	m_iMaxPage			 = 0;
	m_pPlayStage         = NULL;
	m_pArrow             = NULL;
	m_pBigArrow          = NULL;
	m_bArrow             = false;
	m_iDownHeight        = 0;
	m_vItemInfoList.reserve(10);
	InitAniState();
	InitBigArrowAniState();
}

SoldierExerciseWnd::~SoldierExerciseWnd()
{
	SAFEDELETE( m_pArrow );
	SAFEDELETE( m_pBigArrow );
	m_vItemInfoList.clear();
}

void SoldierExerciseWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
	g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
	g_FontMgr.SetAlignType( TAT_LEFT );

	if( m_iExerciseStyle == EXERCISE_PCROOM )
	{		
		g_FontMgr.PrintText( iXPos + 17, iYPos +  8,  FONT_SIZE_13, STR(1));
		g_FontMgr.PrintText( iXPos + 17, iYPos + 28,  FONT_SIZE_13, STR(2));
	}
	else if( m_iExerciseStyle == EXERCISE_EVENT )
	{
		g_FontMgr.PrintText( iXPos + 17, iYPos +  8,  FONT_SIZE_13, STR(3));
		g_FontMgr.PrintText( iXPos + 17, iYPos + 28,  FONT_SIZE_13, STR(4));
	}
	else 
	{
		g_FontMgr.PrintText( iXPos + 17, iYPos +  8,  FONT_SIZE_13, STR(5));
		g_FontMgr.PrintText( iXPos + 17, iYPos + 28,  FONT_SIZE_13, STR(6));
	}

	// arrow animation
	if( m_pArrow )
	{
		enum { FRONT_X = 86, BACK_X = 78, Y_POS = 19, };
		m_pArrow->Render( iXPos + BACK_X, iYPos + Y_POS );
		m_pArrow->Render( iXPos + FRONT_X, iYPos + Y_POS );

		if( m_AniState.GetCurState() == AT_FRONT )
			m_pArrow->Render( iXPos + FRONT_X, iYPos + Y_POS , UI_RENDER_SCREEN );
		else if( m_AniState.GetCurState() == AT_BACK )
			m_pArrow->Render( iXPos + BACK_X, iYPos + Y_POS , UI_RENDER_SCREEN );
	}

	// big arrow animation
	RenderBigArrow( iXPos, iYPos );
}

void SoldierExerciseWnd::RenderBigArrow( int iXPos, int iYPos )
{
	if( !m_bArrow ) return;
	if( !m_pBigArrow ) return;

	enum { X_OFFSET_DOWN = -39, Y_OFFSET = 5, };

	if( m_BigArrowAniState.GetCurState() == BAT_DOWN )
	{
		// 가속 2->1->0
		int iYOffSet = cos( (FLOAT_PI * FLOAT05 * m_BigArrowAniState.GetCurTime()  ) / m_BigArrowAniState.GetDelay() ) * m_iDownHeight;	
		m_pBigArrow->SetAlpha( 255 );
		m_pBigArrow->SetColor( 255, 255, 255 );
		m_pBigArrow->Render( iXPos + ( X_OFFSET_DOWN - iYOffSet ), iYPos + Y_OFFSET, UI_RENDER_NORMAL, TFO_BILINEAR);
	}
	else if( m_BigArrowAniState.GetCurState() == BAT_UP )
	{
		// 감속 0->1->2
		int iYOffSet = sin( (FLOAT_PI * FLOAT05 * m_BigArrowAniState.GetCurTime()  ) / m_BigArrowAniState.GetDelay() ) * m_iDownHeight;	
		m_pBigArrow->SetAlpha( 255 );
		m_pBigArrow->SetColor( 255, 255, 255 );
		m_pBigArrow->Render( iXPos + ( X_OFFSET_DOWN - iYOffSet ), iYPos + Y_OFFSET, UI_RENDER_NORMAL, TFO_BILINEAR );
	}
}

void SoldierExerciseWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return ;

	m_AniState.UpdateTime();
	m_BigArrowAniState.UpdateTime();

	SetExerciseStyle();
}


void SoldierExerciseWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_EXERCISE_CHAR_BUY_BTN1:
	case ID_EXERCISE_CHAR_BUY_BTN2:
	case ID_EXERCISE_CHAR_BUY_BTN3:
	case ID_EXERCISE_CHAR_BUY_BTN4:
	case ID_EXERCISE_CHAR_BUY_BTN5:
	case ID_EXERCISE_CHAR_BUY_BTN6:
	case ID_EXERCISE_CHAR_BUY_BTN7:
	case ID_EXERCISE_CHAR_BUY_BTN8:
	case ID_EXERCISE_CHAR_BUY_BTN9:
	case ID_EXERCISE_CHAR_BUY_BTN10:
		if( cmd == IOBN_BTNUP )
		{
			bool bWait = false;
			SoldierSelectWnd *pSelectWnd = dynamic_cast<SoldierSelectWnd*>(g_GUIMgr.FindWnd( SOLDIER_SELECT_WND ));
			if( pSelectWnd && pSelectWnd->IsSelectWait() )
				bWait = true;

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

			if( !IsCanBuyLevel(pItemBtn) )
				return;

			// 유저가 실수 할 수 있으므로 고용된 용병을 클릭해도 튜토리얼 수행 한 것으로 판단.
			g_TutorialMgr.ActionBuyExercise();


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
			kPacket << m_iExerciseStyle;
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
	case ID_UP_BTN:
		if( cmd == IOBN_BTNUP )
		{
			UpdatePage( false );
			UpdateChildPos();
		}
		break;
	case ID_DOWN_BTN:
		if( cmd == IOBN_BTNUP )
		{
			UpdatePage( true );
			UpdateChildPos();
		}
		break;
	}
}

void SoldierExerciseWnd::iwm_create()
{
	SetMaxPage();
}

void SoldierExerciseWnd::SetExerciseStyle()
{
	if( !m_pPlayStage ) return;

	int iChangeStyle = m_iExerciseStyle;
	if( iChangeStyle == EXERCISE_GENERAL )
	{
		//EventType eEventType = EVT_CHILDRENDAY;
		EventType eEventType = EVT_EXERCISESOLDIER;

		// 피씨방 - > 이벤트 순 
		if( g_MyInfo.GetPCRoomAuthority() )
			iChangeStyle = EXERCISE_PCROOM;
		else if( g_MyInfo.IsUserEvent() )
			iChangeStyle = EXERCISE_PCROOM;
		else if( g_EventMgr.IsAlive( eEventType, g_MyInfo.GetChannelingType() ) ) 
			iChangeStyle = EXERCISE_EVENT;
	}
	else if( iChangeStyle == EXERCISE_EVENT )
	{
		if( g_MyInfo.GetPCRoomAuthority() )
			iChangeStyle = EXERCISE_PCROOM;
	}

	if( iChangeStyle != m_iExerciseStyle )
	{
		m_iExerciseStyle = iChangeStyle;
		UpdateChildPos();
	}	
}

void SoldierExerciseWnd::iwm_show()
{
	m_iExerciseStyle = EXERCISE_GENERAL;
	SetExerciseStyle();

	UpdateChildPos();
}

void SoldierExerciseWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	m_bArrow                = xElement.GetBoolAttribute_e( "UseArrow" );
	m_iDownHeight           = xElement.GetIntAttribute_e( "DownHeight" );
}

void SoldierExerciseWnd::UpdateChildPos()
{
	for (int i = ID_EXERCISE_CHAR_BUY_BTN1; i < ID_EXERCISE_CHAR_BUY_BTN10 + 1; i++)
	{
		ioWnd *pBtn = FindChildWnd( i );
		if( !pBtn ) continue;
		pBtn->HideWnd();
	}

	int iPreMaxPage = m_iMaxPage;
	SetMaxPage();

	if( iPreMaxPage != m_iMaxPage )
	{
		SetChildInActive( ID_UP_BTN );
		m_iCurPage = 0;
		if( m_iMaxPage == 1 )
			SetChildInActive( ID_DOWN_BTN );
		else
			SetChildActive( ID_DOWN_BTN );
	}

	int iStart = m_iCurPage * MAX_BUY_BTN;
	int iEnd = iStart + MAX_BUY_BTN;
	int iWndID = ID_EXERCISE_CHAR_BUY_BTN1;
	SortItemInfo();
	for (int i = iStart; i < iEnd ; i++)
	{
		_UpdateChildPos( i , iWndID);
		iWndID++;
	}
}

void SoldierExerciseWnd::_UpdateChildPos( int a_iArray ,int iWndID )
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

	if( m_iExerciseStyle == EXERCISE_GENERAL )
		pItemBtn->SetSoldierIcon( pImg, ICON_SCALE, false );
	else
		pItemBtn->SetSoldierIcon( pImg, ICON_SCALE, true );
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
	
	//EventType eEventType = EVT_CHILDRENDAY;
	EventType eEventType = EVT_EXERCISESOLDIER;
	if( eEventType == EVT_CHILDRENDAY)
	{
		// 이벤트 체험 용병만 선택에 제한이있음
		if( m_iExerciseStyle == EXERCISE_EVENT && g_EventMgr.GetValue( EVT_CHILDRENDAY, EA_CHILDRENDAY_IS_LIMIT ) == 1 )
			g_MyInfo.GetNeedLevelInfo( pInfo, iType, iLevel );
	}

	if( iType == ioSetItemInfo::NLT_NONE )
		pItemBtn->SetNeedTypeLv( (ioSetItemInfo::NeedLevelType)iType, -1 ); 
	else
		pItemBtn->SetNeedTypeLv( (ioSetItemInfo::NeedLevelType)iType, iLevel );
}

void SoldierExerciseWnd::SetPlayStage( ioPlayStage *pPlayStage )
{
	m_pPlayStage = pPlayStage;
}

bool SoldierExerciseWnd::IsCanExerciseChar( bool bSelectWait )
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

void SoldierExerciseWnd::SetBoughtType( SoldierExerciseBtn::SoldierBoughtType eBoughtType, int iCharArray )
{
	int iClassType = g_MyInfo.GetClassType( iCharArray );

	for (int i = ID_EXERCISE_CHAR_BUY_BTN1; i <  ID_EXERCISE_CHAR_BUY_BTN10 +1; i++)
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

void SoldierExerciseWnd::SetOwnerChangeWaitState()
{
	if( !m_pPlayStage ) return;

	if( m_pPlayStage->GetModeType() == MT_MYROOM )
		return;

	ioBaseChar *pOwner = m_pPlayStage->GetOwnerChar();
	if( !pOwner ) return;

	if( m_pPlayStage->IsOneMyChar() )
		pOwner->SetChangeWaitState( pOwner->GetSelectCharArray(), false );
}

void SoldierExerciseWnd::SendChangeWait( int iArray )
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
			ToLuciferItem(pItem)			||
			ToChargeComboBowItem(pItem)		||
			ToWestGunExtendItem(pItem)		||
			ToPeterpanItem(pItem)			||
			ToWindLordItem(pItem)			||
			ToProject_K_Item(pItem)			||
			ToHakumenItem(pItem)			||
			ToDevilItem(pItem)				||
			ToMaidItem(pItem)				||
			ToFlashItem(pItem)				||
			ToPsychoMetryItem(pItem)		||
			ToSuddenAttackItem(pItem)		||
			ToSlasherItem(pItem)			||
			ToDummyCharItem2(pItem)			||
			ToTitanWeaponItem(pItem)		||
			ToSuperDimensionItem( pItem )	||
			ToTraceItem(pItem)				||
			ToKaelthasItem(pItem)			||
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
			ToChainMagicianItem( pItem )	||
			ToStriderItem( pItem )			||
			ToCuchulainWeaponItem( pItem )  ||
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
	}
}

void SoldierExerciseWnd::SetBlankIcon( int a_iWndID )
{
	SoldierExerciseBtn *pItemBtn = dynamic_cast<SoldierExerciseBtn *>( FindChildWnd( a_iWndID ) );
	if( !pItemBtn ) return;

	ioUIRenderImage *pImg = g_UIImageSetMgr.CreateImageByFullName( "UIIconPack3#empty_large" );
	if( !pImg )	return;

	if( m_iExerciseStyle == EXERCISE_GENERAL )
		pItemBtn->SetSoldierIcon( pImg, ICON_SCALE, false );
	else
		pItemBtn->SetSoldierIcon( pImg, ICON_SCALE, true );
	pItemBtn->SetBoughtType( SoldierExerciseBtn::SBT_NONE );
	pItemBtn->ShowWnd();		
	pItemBtn->SetInActive();
	pItemBtn->SetNeedTypeLv( ioSetItemInfo::NLT_NONE, -1 );
}

void SoldierExerciseWnd::UpdatePage( bool bPlus )
{
	if( bPlus )
	{
		m_iCurPage++;
		if( !COMPARE( m_iCurPage, 0, m_iMaxPage ) )
			m_iCurPage = m_iMaxPage-1;

		if( m_iCurPage == (m_iMaxPage-1) )
		{
			if( m_iMaxPage > 1 )
				SetChildActive( ID_UP_BTN );
			SetChildInActive( ID_DOWN_BTN );
		}
		else
			SetChildActive( ID_UP_BTN );
	}
	else
	{
		m_iCurPage--;
		if( !COMPARE( m_iCurPage, 0, m_iMaxPage ) )
			m_iCurPage = 0;

		if( m_iCurPage == 0 )
		{
			if( m_iMaxPage > 1 )
				SetChildActive( ID_DOWN_BTN );
			SetChildInActive( ID_UP_BTN );
		}
		else
			SetChildActive( ID_DOWN_BTN );
	}
}

void SoldierExerciseWnd::SetMaxPage()
{
	int iUseCharCnt = g_ClassPrice.MaxActiveClassPrice()-g_MyInfo.GetActiveCharCount();
	if( iUseCharCnt <= 0 )
	{	
		m_iMaxPage = 0;
		return;
	}

	m_iMaxPage = ( iUseCharCnt / MAX_BUY_BTN );
	if( ( iUseCharCnt % MAX_BUY_BTN ) != 0 )
		m_iMaxPage++;
}

void SoldierExerciseWnd::InitAniState()
{
	enum { DELAY_DEFAULT = 100, };

	FSMState *pState = new FSMState( AT_NONE );
	if( pState )
		pState->Add( AT_NONE, AT_FRONT, DELAY_DEFAULT );
	m_AniState.Add( pState );

	pState = new FSMState( AT_FRONT );
	if( pState )
		pState->Add( AT_FRONT, AT_BACK, DELAY_DEFAULT );
	m_AniState.Add( pState );

	pState = new FSMState( AT_BACK );
	if( pState )
		pState->Add( AT_BACK, AT_NONE, DELAY_DEFAULT );
	m_AniState.Add( pState );

	m_AniState.SetCurState( AT_NONE );
}

void SoldierExerciseWnd::InitBigArrowAniState()
{
	enum { DELAY_DOWN_UP = 150, };
	FSMState *pState = new FSMState( BAT_DOWN );
	if( pState )
	{
		pState->Add( BAT_DOWN, BAT_UP, DELAY_DOWN_UP );
	}
	m_BigArrowAniState.Add( pState );	

	pState = new FSMState( BAT_UP );
	if( pState )
	{
		pState->Add( BAT_UP, BAT_DOWN, DELAY_DOWN_UP);
	}
	m_BigArrowAniState.Add( pState );	

	m_BigArrowAniState.SetCurState( BAT_DOWN );
}

void SoldierExerciseWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "Arrow" )
	{
		SAFEDELETE( m_pArrow );
		m_pArrow = pImage;
	}
	else if( szType == "BigArrow" )
	{
		SAFEDELETE( m_pBigArrow );
		m_pBigArrow = pImage;
		if( m_pBigArrow )
			m_pBigArrow->SetScale( BIGARROW_SCALE );
	}
	else
		ioWnd::AddRenderImage( szType, pImage );
}

bool SoldierExerciseWnd::ChangeBoughtChar( int iArray, bool bWait )
{
	if( iArray == -1 ) return false;

	if( g_MyInfo.IsCharExerciseStyle( iArray, EXERCISE_GENERAL ) ||
		g_MyInfo.IsCharExerciseStyle( iArray, EXERCISE_EVENT ) )
	{
		if( m_iExerciseStyle == EXERCISE_PCROOM )
			return false;      
	}

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

/*	{
		SoldierLimitWnd *pLimitWnd = dynamic_cast<SoldierLimitWnd*>(g_GUIMgr.FindWnd( SOLDIER_LIMIT_WND ));
		if( pLimitWnd )
		{
			DWORD dwIndex = g_MyInfo.GetCharIndex( iArray );
			if( dwIndex != 0 && !pLimitWnd->IsShow() )
			{
				pLimitWnd->SetInActiveChar( dwIndex );
				pLimitWnd->SettingWnd( m_pPlayStage, true );
			}
		}
	}
*/
}

void SoldierExerciseWnd::SortItemInfo()
{
	m_vItemInfoList.clear();
	int iMax = g_ClassPrice.MaxClassPrice();
	for (int i = 0; i <  iMax; i++)
	{
		if( !g_ClassPrice.IsActiveClass( i ) )
			continue;
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

		if( m_iExerciseStyle == EXERCISE_PCROOM )
		{
			int iMaxType = pInfo->GetNeedLevelInfoListCnt();
			for (int j = 0; j <  iMaxType; j++)
			{
				if( pInfo->GetNeedLevelType(j) == ioSetItemInfo::NLT_GRADE )			
					kInfo.m_iGradeLv = pInfo->GetNeedLevel(j);
				else if( pInfo->GetNeedLevelType(j) == ioSetItemInfo::NLT_BATTLE )
					kInfo.m_iBattleLv = pInfo->GetNeedLevel(j);
				else if( pInfo->GetNeedLevelType(j) == ioSetItemInfo::NLT_TRAINING )
					kInfo.m_iTrainingLv = pInfo->GetNeedLevel(j);
				else if( pInfo->GetNeedLevelType(j) == ioSetItemInfo::NLT_AWARD )
					kInfo.m_iAwardLv = pInfo->GetNeedLevel(j);
			}	
		}		
		
		int iClassType = pInfo->GetSetCode() - SET_ITEM_CODE;
		int iArray = g_MyInfo.GetClassArray( iClassType );
		if( g_MyInfo.IsCharExerciseStyle( iArray, EXERCISE_NONE ) )
			continue;

		m_vItemInfoList.push_back( kInfo );
	}
	if( m_vItemInfoList.empty() )
		return;
	std::sort( m_vItemInfoList.begin() , m_vItemInfoList.end(), ShopItemInfoSort() );
}

int SoldierExerciseWnd::GetSetIdx( int iArray )
{
	if( COMPARE( iArray, 0, (int) m_vItemInfoList.size() ) )
		return m_vItemInfoList[iArray].m_iSetItemIdx;

	return 0;
}

bool SoldierExerciseWnd::IsCanBuyLevel( const SoldierExerciseBtn *pItemBtn )
{
	if( !pItemBtn )
		return false;

	DWORD dwSetCode = pItemBtn->GetClassType() + SET_ITEM_CODE;
	const ioSetItemInfo *pInfo = g_SetItemInfoMgr.GetSetInfoByCode( dwSetCode );
	if( !pInfo )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return false;
	}

	// EventType eEventType = EVT_CHILDRENDAY;
	EventType eEventType = EVT_EXERCISESOLDIER;
	if( eEventType == EVT_CHILDRENDAY)
	{
		if( m_iExerciseStyle == EXERCISE_EVENT && g_EventMgr.GetValue( EVT_CHILDRENDAY, EA_CHILDRENDAY_IS_LIMIT ) == 1 )
		{
			if( !g_MyInfo.IsCanBuyItem( pInfo ) )
			{
				NoticeNeedLevel( pInfo );
				return false;
			}
		}
	}
	return true;
}

void SoldierExerciseWnd::NoticeNeedLevel( const ioSetItemInfo *pInfo )
{
	if( !pInfo )
	{
		LOG.PrintTimeAndLog(0, "SoldierExerciseWnd::NoticeNeedLevel" );
		return;
	}

	int iNeedType  = 0;
	int iNeedLevel = 0;
	g_MyInfo.GetNeedLevelInfo( pInfo, iNeedType, iNeedLevel );

	char szHelp[MAX_PATH]="";
	if( iNeedType == ioSetItemInfo::NLT_GRADE )
		g_LevelMgr.GetGradeName( iNeedLevel, szHelp, sizeof(szHelp), false );
	else if( iNeedType == ioSetItemInfo::NLT_BATTLE )
		SafeSprintf( szHelp, sizeof( szHelp ), STR(1), iNeedLevel );
	else if( iNeedType == ioSetItemInfo::NLT_AWARD )
		SafeSprintf( szHelp, sizeof( szHelp ), STR(2), iNeedLevel );
	else
		SafeSprintf( szHelp, sizeof( szHelp ), STR(4) );

	ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];

	kPrinter[0].SetTextStyle( TS_NORMAL );
	kPrinter[0].SetBkColor( 0, 0, 0 );	
	kPrinter[0].SetTextColor( TCT_DEFAULT_GRAY );
	kPrinter[0].AddTextPiece( FONT_SIZE_13,STR(5));

	kPrinter[1].SetTextStyle( TS_NORMAL );
	kPrinter[1].SetBkColor( 0, 0, 0 );	
	kPrinter[1].SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter[1].AddTextPiece( FONT_SIZE_13,STR(6) );
	kPrinter[1].SetTextColor( 255, 60, 0 );
	kPrinter[1].AddTextPiece( FONT_SIZE_13, szHelp );

	kPrinter[2].SetTextStyle( TS_NORMAL );
	kPrinter[2].SetBkColor( 0, 0, 0 );	
	kPrinter[2].SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter[2].AddTextPiece( FONT_SIZE_13,STR(7) );
	kPrinter[2].SetTextColor( 255, 60, 0 );
	kPrinter[2].AddTextPiece( FONT_SIZE_13,STR(8) );
	kPrinter[2].SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter[2].AddTextPiece( FONT_SIZE_13,STR(9) );

	g_GUIMgr.SetPrevMsgListBox( NULL, MB_OK, NULL, kPrinter );
}
