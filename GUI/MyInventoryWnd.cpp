#include "StdAfx.h"

#include "../ioUserAccessory.h"
#include "../ioAccessoryInfoManager.h"

#include "../WndID.h"
#include "../Setting.h"
#include "../ioMyInfo.h"
#include "../ioApplication.h"
#include "../ioDateHelp.h"

#include "../ioMyLevelMgr.h"
#include "../ioClassPrice.h"

#include "../EtcHelpFunc.h"
#include "../TextColorTable.h"

#include "../ioPlayStage.h"

#include "../ioBaseChar.h"
#include "../ioCreateChar.h"
#include "../ioSkillMaker.h"

#include "../ioGrowthLevel.h"
#include "../ioGrowthUpInfo.h"
#include "../ioGrowthInfoManager.h"

#include "../ioItem.h"
#include "../ioItemMaker.h"
#include "../ioSetItemInfo.h"
#include "../ioSetItemInfoManager.h"

#include "../ioUserEtcItem.h"
#include "../ioEtcItemManager.h"
#include "../ioUserExtraItem.h"
#include "../ioExtraItemInfoManager.h"
#include "../ioSubscriptionMgr.h"

#include "../ioAlchemicInventory.h"
#include "../ioAlchemicMgr.h"

#include "../ioPresentMgr.h"
#include "../ioDecorationPrice.h"

#include "../ioMedalItemInfoManager.h"
#include "../ioExpandMedalSlotManager.h"
#include "../ioSoldierAwakeManager.h"

#include "../ioQuestManager.h"
#include "../ioMyRoomMode.h"

#include "../ioPowerUpManager.h"

#include "..\Local\ioLocalManager.h"
#include "..\Local\ioLocalParent.h"

#include "ioUI3DRender.h"
#include "MySoldierWnd.h"
#include "ioSP2GUIManager.h"
#include "GlobalWnd.h"

#include "NewShopWnd.h"
#include "MyInventoryWnd.h"
#include "MyInventoryInfoWnd.h"

#include "ioGrowthSelectWnd.h"
#include "RandomDecoSelectWnd.h"
#include "PresentWnd.h"
#include "ItemCompoundWnd.h"
#include "ItemCompoundResultWnd.h"
#include "ItemMultipleCompoundWnd.h"
#include "ItemMultipleCompoundResultWnd.h"
#include "GrowthDownSelectWnd.h"
#include "GashaponWnd.h"
#include "RandomDecoWnd.h"
#include "ioExtraItemRandomBox.h"
#include "GrowthDownDoneWnd.h"
#include "ItemResellWnd.h"
#include "ChangeNameWnd.h"
#include "RecordInitWnd.h"
#include "UISoldierSelectWnd.h"
#include "ioSpecificDragItem.h"
#include "ChangeTradeStateWnd.h"
#include "ItemCustomWnd.h"
#include "ExpandMedalSlotWnd.h"
#include "ExpandMedalSlotResultWnd.h"
#include "AlchemicFuncWnd.h"
#include "EtcItemSlotWnd.h"

#include "invensoldierselectwnd.h"
#include "InvenFourExtraCompoundWnd.h"
#include "SuperGashaponWnd.h"

#include "ItemMaterialCompoundWnd.h"
#include "ItemMaterialCompoundResultWnd.h"
#include "PetEggResultWnd.h"
#include "PetWnd.h"

#include "CostumeCustomWnd.h"
#include "ioCostumeRandomBox.h"

#include "ReinforceSelectWnd.h"
#include "HeroReinforceDownWnd.h"
#include "ItemReinforceDownWnd.h"
#include "ioPullDownList.h"
#include "../ioBasicTooltipManager.h"
#include "PopUpStoreWnd.h"

#include "ItemNewMultipleCompoundWnd.h"
#include "../ioRSoldierInfoManager.h"

#include "../Housing/BlockDefine.h"

#include "HomeEtcItemNeedPopup.h"
#include "ioHousingBlockInvenWnd.h"

#include "NamedTitleResultWnd.h"
#include "NamedTitlePremiumSelectWnd.h"
#include "SmallTabButton.h"
#include "../ioBonusCashManager.h"

InventoryIconBtn::InventoryIconBtn()
{
	m_pIcon       = NULL;
	m_pIconBack   = NULL;
	m_pIconSelect = NULL;
	m_pCustomIcon = NULL;
	m_pCostumeMark = NULL;

	m_pNumText = NULL;
	m_pPlusText = NULL;
	m_pSmallNumText = NULL;
	m_pSmallPlusText = NULL;

	m_pGaugeBack = NULL;
	m_pGaugeImg = NULL;

	m_pStateBlueFrm = NULL;
	m_pStateGreenFrm = NULL;
	m_pStateGreenFrm2 = NULL;
	m_pStateOrangeFrm = NULL;
	m_pStatePinkFrm = NULL;

	m_iMarkType = 0; // INVENTORY_MARK_TYPE_NONE

	m_iNum = 0;
	m_eNumberType = NT_NONE;
	m_fScale = FLOAT1;
	m_fBackScale = FLOAT1;

	m_iSexType  = 0;
	m_iDecoType = 0;
	m_iDecoCode = 0;

	m_iGapTime = 0;
	m_iNeedTime = 0;

	m_iClassType= 0;

	m_iEtcItemType = 0;

	m_bEffect = false;
	m_dwCheckTime = 0;

	m_bSelect   = false;

	m_bExtraItemCustom = false;
	m_bExtraItemGrowthCatalyst = false;

	m_pNewShopItemInfo = NULL;

	m_bIsCostume = false;

	m_nGradeType = 0;
}

InventoryIconBtn::~InventoryIconBtn()
{
	SAFEDELETE( m_pIcon );
	SAFEDELETE( m_pIconBack );
	SAFEDELETE( m_pIconSelect );
	SAFEDELETE( m_pCustomIcon );
	SAFEDELETE( m_pCostumeMark );

	SAFEDELETE( m_pNumText );
	SAFEDELETE( m_pPlusText );
	SAFEDELETE( m_pSmallNumText );
	SAFEDELETE( m_pSmallPlusText );

	SAFEDELETE( m_pGaugeBack );
	SAFEDELETE( m_pGaugeImg );

	SAFEDELETE( m_pStateBlueFrm );
	SAFEDELETE( m_pStateGreenFrm );
	SAFEDELETE( m_pStateGreenFrm2 );
	SAFEDELETE( m_pStateOrangeFrm );
	SAFEDELETE( m_pStatePinkFrm );

	SAFEDELETE( m_pNewShopItemInfo );
}

void InventoryIconBtn::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "IconBack" )
	{
		SAFEDELETE( m_pIconBack );
		m_pIconBack = pImage;
	}
	else if( szType == "IconSelect" )
	{
		SAFEDELETE( m_pIconSelect );
		m_pIconSelect = pImage;
	}
	else if( szType == "custom_image" )
	{
		SAFEDELETE( m_pCustomIcon );
		m_pCustomIcon = pImage;
	}
	else if( szType == "costume_image" )
	{
		SAFEDELETE( m_pCostumeMark );
		m_pCostumeMark = pImage;
	}
	else if( szType == "NumText" )
	{
		SAFEDELETE( m_pNumText );
		m_pNumText = pImage;
	}
	else if( szType == "PlusText" )
	{
		SAFEDELETE( m_pPlusText );
		m_pPlusText = pImage;
	}
	else if( szType == "SmallNumText" )
	{
		SAFEDELETE( m_pSmallNumText );
		m_pSmallNumText = pImage;
	}
	else if( szType == "SmallPlusText" )
	{
		SAFEDELETE( m_pSmallPlusText );
		m_pSmallPlusText = pImage;
	}
	else if( szType == "GaugeBack" )
	{
		SAFEDELETE( m_pGaugeBack );
		m_pGaugeBack = pImage;
	}
	else if( szType == "GaugeImg" )
	{
		SAFEDELETE( m_pGaugeImg );
		m_pGaugeImg = pImage;
	}
	else
	{
		ioButton::AddRenderImage( szType, pImage );	
	}	
}

void InventoryIconBtn::AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame )
{
	if( pFrame )
	{
		if( szType == "StateBlueFrm" )
		{
			SAFEDELETE( m_pStateBlueFrm );
			m_pStateBlueFrm = pFrame;
		}
		else if( szType == "StateGreenFrm" )
		{
			SAFEDELETE( m_pStateGreenFrm );
			m_pStateGreenFrm = pFrame;
		}
		else if( szType == "StateGreenFrm2" )
		{
			SAFEDELETE( m_pStateGreenFrm2 );
			m_pStateGreenFrm2 = pFrame;
		}
		else if( szType == "StateOrangeFrm" )
		{
			SAFEDELETE( m_pStateOrangeFrm );
			m_pStateOrangeFrm = pFrame;
		}
		else if( szType == "StatePinkFrm" )
		{
			SAFEDELETE( m_pStatePinkFrm );
			m_pStatePinkFrm = pFrame;
		}
		else
		{	
			ioButton::AddRenderFrame( szType, pFrame );
		}
	}
}

void InventoryIconBtn::ParseExtraInfo( ioXMLElement &xElement )
{
	ioButton::ParseExtraInfo( xElement );

	m_dwEffectTime = xElement.GetIntAttribute_e( "EffectShow" );
	m_dwEffectHide = xElement.GetIntAttribute_e( "EffectHide" );
}

int InventoryIconBtn::GetRenderImageStartHeight( ioUIRenderImage *pImage, int iSrcHeight, int iCur, int iMax )
{
	if( iMax == iCur ) return 0;
	if( !pImage )	return 0;

	float fGap = (float)iCur / (float)iMax;
	float fGapHeight = (float)iSrcHeight * fGap;
	return pImage->GetOffsetY() + ( (iSrcHeight / 2) - fGapHeight );							
}

void InventoryIconBtn::OnDrawNormal( int iXPos, int iYPos )
{
	ioButton::OnDrawNormal( iXPos, iYPos );

	RenderSelect( iXPos, iYPos );
	RenderBack( iXPos, iYPos );
	RenderIcon( iXPos, iYPos , UI_RENDER_NORMAL );
	RenderOtherInfo( iXPos, iYPos );
}

void InventoryIconBtn::OnDrawOvered( int iXPos, int iYPos )
{
	ioButton::OnDrawOvered( iXPos, iYPos );

	RenderSelect( iXPos, iYPos );
	RenderBack( iXPos, iYPos );
	RenderIcon( iXPos, iYPos , UI_RENDER_NORMAL );
	RenderIcon( iXPos, iYPos , UI_RENDER_SCREEN );
	RenderOtherInfo( iXPos, iYPos );
}

void InventoryIconBtn::OnDrawPushed( int iXPos, int iYPos )
{
	ioButton::OnDrawPushed( iXPos, iYPos );

	RenderSelect( iXPos, iYPos );
	RenderBack( iXPos, iYPos );
	RenderIcon( iXPos, iYPos , UI_RENDER_NORMAL );
	RenderIcon( iXPos, iYPos , UI_RENDER_SCREEN );
	RenderOtherInfo( iXPos, iYPos );
}

void InventoryIconBtn::OnDrawDisabled( int iXPos, int iYPos )
{
	ioButton::OnDrawDisabled( iXPos, iYPos );

	RenderSelect( iXPos, iYPos );
	RenderBack( iXPos, iYPos );
	RenderIcon( iXPos, iYPos , UI_RENDER_NORMAL );
	RenderOtherInfo( iXPos, iYPos );
}

void InventoryIconBtn::RenderBack( int iXPos, int iYPos )
{
	int iNewXPos = iXPos + GetWidth() / 2;
	int iNewYPos = iYPos + GetHeight() / 2;

	// Icon
	if( m_pIconBack )
	{
		m_pIconBack->SetScale( m_fBackScale );

		if( m_fBackScale != FLOAT1 )
			m_pIconBack->Render( iNewXPos, iNewYPos, UI_RENDER_MULTIPLY, TFO_BILINEAR );
		else
			m_pIconBack->Render( iNewXPos, iNewYPos, UI_RENDER_MULTIPLY );
	}
}

void InventoryIconBtn::RenderGaugeIcon( int iXPos, int iYPos, ioUIRenderImage *pImage, int iSrcHeight, int iCur, int iMax, UIRenderType eType )
{
	int iRealHeight = GetRenderImageStartHeight( pImage, iSrcHeight, iCur, iMax );

	pImage->Render( iXPos, iYPos, UI_RENDER_GRAY );
	if( eType == UI_RENDER_SCREEN )
	{
		pImage->Render( iXPos, iYPos, UI_RENDER_GRAY, TFO_NEAREST, false, UI_RENDER_SCREEN );
	}

	pImage->RenderHeightCut( iXPos, iYPos, iRealHeight, pImage->GetHeight(), UI_RENDER_NORMAL );
	if( eType == UI_RENDER_SCREEN )
	{
		pImage->RenderHeightCut( iXPos, iYPos, iRealHeight, pImage->GetHeight(), eType );
	}

	if( iRealHeight + 2 <= pImage->GetHeight() )
	{
		pImage->SetColor( 0, 0, 0 );
		pImage->RenderHeightCut( iXPos, iYPos, iRealHeight, iRealHeight + 2, UI_RENDER_NORMAL );
		pImage->SetColor( 255, 255, 255 );
	}	
}

void InventoryIconBtn::RenderIcon( int iXPos, int iYPos, UIRenderType eType )
{
	enum 
	{
		X_TEXT_OFFSET   = 20,
		Y_TEXT_OFFSET   = 2,
	};

	int iNewXPos = iXPos + GetWidth() / 2;
	int iNewYPos = iYPos + GetHeight() / 2;

	if( m_pIcon )
	{
		m_pIcon->SetScale( m_fScale );

		if( COMPARE( m_iEtcItemType, ioEtcItem::EIT_ETC_ITEM_TIME_GASHAPON1,	ioEtcItem::EIT_ETC_ITEM_TIME_GASHAPON100 + 1 ) ||
			COMPARE( m_iEtcItemType, ioEtcItem::EIT_ETC_ITEM_TIME_GASHAPON101,	ioEtcItem::EIT_ETC_ITEM_TIME_GASHAPON300 + 1 ) ||
			COMPARE( m_iEtcItemType, ioEtcItem::EIT_ETC_ITEM_TIME_GASHAPON301,	ioEtcItem::EIT_ETC_ITEM_TIME_GASHAPON600 + 1 ) )
		{
			ioUserEtcItem *pUserEtcItem = g_MyInfo.GetUserEtcItem();
			ioEtcItemTimeGashapon *pTimeGashapon = dynamic_cast< ioEtcItemTimeGashapon * >( g_EtcItemMgr.FindEtcItem( m_iEtcItemType ) );
			if( pUserEtcItem && pTimeGashapon )
			{
				ioUserEtcItem::ETCITEMSLOT kSlot;
				if( pUserEtcItem->GetEtcItem( m_iEtcItemType, kSlot ) )
				{
					if( pTimeGashapon->IsExtendData() && !pTimeGashapon->IsSequenceOrder() )
					{
						RenderGaugeIcon( iNewXPos, iNewYPos, m_pIcon, m_pIcon->GetHeight(), pTimeGashapon->GetRepeatDate(), pTimeGashapon->GetRepeatDate(), eType );
					}
					else
					{
						if( pTimeGashapon->IsSequenceOrder() )
						{
							int iState    = kSlot.GetDateExcludeValue3State();
							int iRealTime = kSlot.GetDateExcludeValue3Time();

							RenderGaugeIcon( iNewXPos, iNewYPos, m_pIcon, m_pIcon->GetHeight(), 
								pTimeGashapon->GetSequenceDate( iState ) - iRealTime, pTimeGashapon->GetSequenceDate( iState ), eType );
						}
						else
						{
							m_pIcon->Render( iNewXPos, iNewYPos, UI_RENDER_GRAY );
							RenderGaugeIcon( iNewXPos, iNewYPos, m_pIcon, m_pIcon->GetHeight(), 
								pTimeGashapon->GetRepeatDate() - kSlot.GetDateExcludeValue2(), pTimeGashapon->GetRepeatDate(), eType );
						}
					}
				}
			}
		}
		else
		{
			UIRenderType eNewType = eType;
			if( HasWndStyle( IWS_INACTIVE ) )
				eNewType = UI_RENDER_GRAY;

			if ( eNewType != UI_RENDER_SCREEN )
				g_UIImageRenderMgr.RenderGradeIconBack( m_nGradeType, iNewXPos, iNewYPos, m_fScale, eNewType );

			if( m_fScale != FLOAT1 )
				m_pIcon->Render( iNewXPos, iNewYPos, eNewType, TFO_BILINEAR );
			else
				m_pIcon->Render( iNewXPos, iNewYPos, eNewType );
		}
	}

	if( m_bExtraItemCustom && m_pCustomIcon )
	{
		iNewXPos = iNewXPos + ( 7.0f * m_fScale );
		iNewYPos = iNewYPos - ( 43.0f * m_fScale );
		m_pCustomIcon->SetScale( m_fScale );
		if( m_fScale != FLOAT1 )
			m_pCustomIcon->Render( iNewXPos, iNewYPos, UI_RENDER_NORMAL, TFO_BILINEAR );
		else
			m_pCustomIcon->Render( iNewXPos, iNewYPos );
	}

	// UI코스튬 관련 (코스튬 마크)
	if( m_bIsCostume && m_pCostumeMark )
	{
		m_pCostumeMark->SetScale( m_fScale );

		UIRenderType eNewType = eType;
		if( HasWndStyle( IWS_INACTIVE ) )
			eNewType = UI_RENDER_GRAY;

		if( m_fScale != 1.0f )
			m_pCostumeMark->Render( iXPos + GetWidth()/2, iYPos + GetHeight()/2, eNewType, TFO_BILINEAR );
		else
			m_pCostumeMark->Render( iXPos + GetWidth()/2, iYPos + GetHeight()/2, eNewType );
	}
}

void InventoryIconBtn::RenderOtherInfo( int iXPos, int iYPos )
{
	enum 
	{
		X_TEXT_OFFSET   = 20,
		Y_TEXT_OFFSET   = 2,
	};

	int iNewXPos = iXPos + GetWidth() / 2;
	int iNewYPos = iYPos + GetHeight() / 2;


	// Mark
	int iMarkX = iNewXPos;
	int iMarkY = iNewYPos;

	int iTextX = iMarkX;
	int iTextY = iMarkY;

	char szName[MAX_PATH]="";
	if( m_iMarkType == INVENTORY_MARK_TYPE_GROWTH_ENABLE )
	{
		iMarkX = iNewXPos-62;
		iMarkY = iNewYPos-37;

		if( m_pStateGreenFrm )
			m_pStateGreenFrm->Render( iMarkX, iMarkY, UI_RENDER_NORMAL );

		iTextX = iMarkX+27;
		iTextY = iMarkY+Y_TEXT_OFFSET;
		SafeSprintf( szName, sizeof( szName ), STR(1) );
	}
	else if( m_iMarkType == INVENTORY_MARK_TYPE_GROWTH_DISABLE )
	{
		iMarkX = iNewXPos-62;
		iMarkY = iNewYPos-37;

		if( m_pStateGreenFrm )
			m_pStateGreenFrm->Render( iMarkX, iMarkY, UI_RENDER_GRAY_LIGHT );

		iTextX = iMarkX+27;
		iTextY = iMarkY+Y_TEXT_OFFSET;
		SafeSprintf( szName, sizeof( szName ), STR(2) );
	}
	else if( m_iMarkType == INVENTORY_MARK_TYPE_EQUIP )
	{
		iMarkX = iNewXPos-45;
		iMarkY = iNewYPos-41;

		if( m_pStateBlueFrm )
			m_pStateBlueFrm->Render( iMarkX, iMarkY, UI_RENDER_NORMAL );

		iTextX = iMarkX+X_TEXT_OFFSET;
		iTextY = iMarkY+Y_TEXT_OFFSET;
		SafeSprintf( szName, sizeof( szName ), STR(3) );
	}
	else if( m_iMarkType == INVENTORY_MARK_TYPE_TIMEGROWTH )
	{
		iMarkX = iNewXPos-62;
		iMarkY = iNewYPos-37;

		iTextX = iMarkX + 62;
		iTextY = iMarkY + 2;

		enum { DAY_SEC = 86400, HOUR_SEC = 3600, MINUTE_SEC = 60, };

		// Gap Time
		int iHour   = m_iGapTime / HOUR_SEC;
		int iMinute = ( m_iGapTime-(iHour*HOUR_SEC) ) / MINUTE_SEC;
		int iSec = m_iGapTime - (iHour*HOUR_SEC) - (iMinute*MINUTE_SEC);

		iHour = max( 0, iHour );
		iMinute = max( 0, iMinute );
		iSec = max( 0, iSec );

		if( m_iGapTime > MINUTE_SEC )
			SafeSprintf( szName, sizeof(szName), STR(4), iHour, iMinute, iSec );
		else
			SafeSprintf( szName, sizeof(szName), STR(5) );

		//
		float fRate = 0.0f;
		if( m_iNeedTime > 0 )
			fRate = FLOAT1 - (float)m_iGapTime / m_iNeedTime;

		fRate = min( fRate, FLOAT1 );

		int iCurPos = 121 * fRate;

		if( m_pGaugeBack )
			m_pGaugeBack->Render( iMarkX, iMarkY );

		if( m_pGaugeImg )
		{
			m_pGaugeImg->RenderWidthCut( iMarkX+2, iMarkY+2, 0, iCurPos );

			if( m_bEffect )
				m_pGaugeImg->RenderWidthCut( iMarkX+2, iMarkY+2, 0, iCurPos, UI_RENDER_SCREEN );
		}
	}
	else if( m_iMarkType == INVENTORY_MARK_TYPE_TRADE_WAIT )
	{
		iMarkX = iNewXPos-45;
		iMarkY = iNewYPos-41;

		if( m_pStateOrangeFrm )
			m_pStateOrangeFrm->Render( iMarkX, iMarkY, UI_RENDER_NORMAL );

		iTextX = iMarkX+27;
		iTextY = iMarkY+Y_TEXT_OFFSET;
		SafeSprintf( szName, sizeof( szName ), STR(6) );
	}
	else if( m_iMarkType == INVENTORY_MARK_TYPE_TRADE_ENABLE )
	{
		iMarkX = iNewXPos-45;
		iMarkY = iNewYPos-41;

		if( m_pStateGreenFrm2 )
			m_pStateGreenFrm2->Render( iMarkX, iMarkY, UI_RENDER_NORMAL );

		iTextX = iMarkX+27;
		iTextY = iMarkY+Y_TEXT_OFFSET;
		SafeSprintf( szName, sizeof( szName ), STR(7) );
	}
	else if( m_bExtraItemGrowthCatalyst )
	{
		iMarkX = iNewXPos-45;
		iMarkY = iNewYPos-41;

		if( m_pStatePinkFrm )
			m_pStatePinkFrm->Render( iMarkX, iMarkY, UI_RENDER_NORMAL );

		iTextX = iMarkX+27;
		iTextY = iMarkY+Y_TEXT_OFFSET;
		SafeSprintf( szName, sizeof( szName ), STR(8), m_iNum );
	}

	g_FontMgr.SetTextStyle( TS_BOLD );
	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextColor( TCT_DEFAULT_WHITE );
	g_FontMgr.PrintText( iTextX, iTextY, FONT_SIZE_11, szName );

	// Num
	enum
	{
		REINFORCE_Y	= 14,

		NUM_SIZE	= 20,
		PLUS_SIZE	= 14,

		NUM_GAP		= 4,
	};

	switch( m_eNumberType )
	{
	case NT_SMALL:
		if( m_pSmallNumText && m_pSmallPlusText && m_iNum > 0 )
		{
			int iCurPos = 0;
			int iDefaultPos = iNewXPos;

			int iCurYPos = iNewYPos+(REINFORCE_Y * m_fScale);

		int iTotalSize = 0;
		if( m_iNum < 10 )
			iTotalSize = PLUS_SIZE + NUM_SIZE - NUM_GAP;
		else if( m_iNum < FLOAT100 )
			iTotalSize = PLUS_SIZE + NUM_SIZE*2 - NUM_GAP*2;
		else
			iTotalSize = PLUS_SIZE + NUM_SIZE*3 - NUM_GAP*3;

			iCurPos = iDefaultPos - (iTotalSize / 2);

			m_pSmallPlusText->Render( iCurPos, iCurYPos - 3 );

			iCurPos += (PLUS_SIZE-NUM_GAP);

			m_pSmallNumText->RenderNum( iCurPos, iCurYPos - 3, m_iNum, -NUM_GAP );
		}
		break;
	default:
		if( m_pNumText && m_pPlusText && m_iNum > 0 )
		{
			int iCurPos = 0;
			int iDefaultPos = iNewXPos;

			int iCurYPos = iNewYPos+(REINFORCE_Y * m_fScale);

			int iTotalSize = 0;
			if( m_iNum < 10 )
				iTotalSize = PLUS_SIZE + NUM_SIZE - NUM_GAP;
			else if( m_iNum < 100 )
				iTotalSize = PLUS_SIZE + NUM_SIZE*2 - NUM_GAP*2;
			else
				iTotalSize = PLUS_SIZE + NUM_SIZE*3 - NUM_GAP*3;

			iCurPos = iDefaultPos - (iTotalSize / 2);

			m_pPlusText->Render( iCurPos, iCurYPos );

			iCurPos += (PLUS_SIZE-NUM_GAP);

			m_pNumText->RenderNum( iCurPos, iCurYPos, m_iNum, -NUM_GAP );
		}
	}
}

void InventoryIconBtn::RenderSelect( int iXPos, int iYPos )
{
	if( !m_bSelect )
		return;

	int iNewXPos = iXPos;
	int iNewYPos = iYPos;

	if( m_pIconSelect )
		m_pIconSelect->Render( iNewXPos, iNewYPos );
}

bool InventoryIconBtn::SetIcon( ioUIRenderImage *pIcon, float fScale, float fBackScale, int iNum /* = 0 */, InventoryIconBtn::NumberType eType /*= NT_NONE*/ )
{
	m_bExtraItemCustom = false;
	m_bExtraItemGrowthCatalyst = false;
	m_bIsCostume = false;
	m_nGradeType = 0;
	
	m_iNum = iNum;
	m_fScale = fScale;
	m_fBackScale = fBackScale;
	m_eNumberType = eType;

	m_iMarkType = 0;
	m_iClassType= 0;

	m_iGapTime = 0;
	m_iNeedTime = 0;

	m_iEtcItemType = 0;

	m_bEffect = false;
	m_dwCheckTime = FRAMEGETTIME();

	SAFEDELETE( m_pIcon );
	if( !pIcon )
	{
		m_iNum = 0;
		return false;
	}

	m_pIcon = pIcon;

	return true;
}

void InventoryIconBtn::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;

	if( !m_pParent )
		return;

	if( m_iMarkType == INVENTORY_MARK_TYPE_TIMEGROWTH )
		ProcessTimeGrowth( fTimePerSec );

	if( m_bOver && !m_bClicked )
	{
		ioLSMouse *pMouse = g_App.GetMouse();
		if( pMouse )
		{
			DWORD dwID = 0;
			if( m_iClassType != 0 )
			{
				pMouse->SetToolTipID( g_MyInfo.GetPublicID(), m_iClassType );
			}
			else if( m_pParent->GetID() == MyInventoryWnd::ID_INVENTORY_INFO_WND )
			{
				dwID = GetID();
				pMouse->SetToolTipWndID( dwID );
			}
			else if( m_pParent->GetID() == NewShopWnd::ID_BUY_WND )
			{
				dwID = GetID();
				pMouse->SetToolTipWndID( dwID );
			}
			else
			{
				dwID = m_pParent->GetID();
				pMouse->SetToolTipWndID( dwID );
			}
			return;
		}
	}	
}

void InventoryIconBtn::ProcessTimeGrowth( float fTimePerSec )
{
	m_iGapTime = 0;
	m_iNeedTime = 0;
	m_bExtraItemCustom = false;
	m_bIsCostume = false;

	ioGrowthLevel *pLevel = g_MyInfo.GetGrowthLevel();
	if( pLevel )
	{
		// TimeGrowth
		TimeGrowthInfo *pInfo = pLevel->FindTimeGrowthInfo( m_iClassType );
		if( pInfo )
		{
			m_iGapTime = pLevel->GetTimeGrowthRemainTime( m_iClassType );

			int iTimeSlot = pInfo->m_iTimeSlot;
			int iLevel = pLevel->GetGrowthLevelByTimeSlot( m_iClassType, iTimeSlot );

			bool bChar = false;
			if( COMPARE( iTimeSlot, TIG_ATTACK, TIG_DROP+1 ) )
				bChar = true;

			m_iNeedTime = g_GrowthInfoMgr.GetTimeGrowthNeedTime( bChar, iLevel+1 ) * 60;
		}
	}

	//
	DWORD dwCurTime = FRAMEGETTIME();
	if( m_bEffect )
	{
		if( m_dwCheckTime+m_dwEffectTime < dwCurTime )
		{
			m_bEffect = false;
			m_dwCheckTime = dwCurTime;
		}
	}
	else
	{
		if( m_dwCheckTime+m_dwEffectHide < dwCurTime )
		{
			m_bEffect = true;
			m_dwCheckTime = dwCurTime;
		}
	}
}

void InventoryIconBtn::SetDecoInfo( int iSexType, int iDecoType, int iDecoCode )
{
	m_iSexType  = iSexType;
	m_iDecoType = iDecoType;
	m_iDecoCode = iDecoCode;
	m_bExtraItemCustom = false;
	m_bExtraItemGrowthCatalyst = false;
	m_bIsCostume = false;
}

void InventoryIconBtn::GetDecoInfo( OUT int &riSexType, OUT int &riDecoType, OUT int &riDecoCode )
{
	riSexType = m_iSexType;
	riDecoType= m_iDecoType;
	riDecoCode= m_iDecoCode;
	m_bExtraItemCustom = false;
	m_bExtraItemGrowthCatalyst = false;
	m_bIsCostume = false;
}

void InventoryIconBtn::SetEtcItemType( int iEtcItem )
{
	m_iEtcItemType = iEtcItem;
}

void InventoryIconBtn::SetNewShopItemInfo( const NewShopItemInfo &rkNewShopItemInfo )
{
	if( !m_pNewShopItemInfo )
		m_pNewShopItemInfo = new NewShopItemInfo;

	if( m_pNewShopItemInfo )
		*m_pNewShopItemInfo = rkNewShopItemInfo;
	m_bExtraItemCustom = false;
	m_bExtraItemGrowthCatalyst = false;
	m_bIsCostume = false;
}

void InventoryIconBtn::GetNewShopItemInfo( OUT NewShopItemInfo &rkNewShopItemInfo )
{
	if( !m_pNewShopItemInfo )
		return;
	rkNewShopItemInfo = *m_pNewShopItemInfo;
}
//////////////////////////////////////////////////////////////////////////
InventoryEtcMotionBtn::InventoryEtcMotionBtn()
{
	m_pOrangeNor	= NULL;
	m_pOrangePush	= NULL;
	m_pGrayNor		= NULL;
	m_pGrayPush		= NULL;
	m_pCurrentOver  = NULL;
	m_pCurrentNor	= NULL;
	m_pCurrentPush	= NULL;

	m_dwEtcItem	= 0;
	m_bSelected	= 0;
}

InventoryEtcMotionBtn::~InventoryEtcMotionBtn()
{
	SAFEDELETE( m_pOrangeNor );
	SAFEDELETE( m_pOrangePush );
	SAFEDELETE( m_pGrayNor );
	SAFEDELETE( m_pGrayPush );
	SAFEDELETE( m_pCurrentOver );
}

void InventoryEtcMotionBtn::AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame )
{
	if( szType == "OrangeNor" )
	{
		SAFEDELETE( m_pOrangeNor );
		m_pOrangeNor = pFrame;
	}
	else if( szType == "OrangePush" )
	{
		SAFEDELETE( m_pOrangePush );
		m_pOrangePush = pFrame;
	}
	else if( szType == "GrayNor" )
	{
		SAFEDELETE( m_pGrayNor );
		m_pGrayNor = pFrame;
	}
	else if( szType == "GrayPush" )
	{	
		SAFEDELETE( m_pGrayPush );
		m_pGrayPush = pFrame;
	}
	else if( szType == "CurrentOver" )
	{
		SAFEDELETE( m_pCurrentOver );
		m_pCurrentOver = pFrame;
	}
	else 
	{
		ioButton::AddRenderFrame( szType, pFrame );
	}
}

void InventoryEtcMotionBtn::SetEtcCode( DWORD dwCode )
{
	if( !COMPARE( dwCode, ioEtcItem::EIT_ETC_MOTION1, ioEtcItem::EIT_ETC_MOTION100 + 1 ) &&
		!COMPARE( dwCode, ioEtcItem::EIT_ETC_MOTION101, ioEtcItem::EIT_ETC_MOTION400 + 1 ) )
		return;

	m_dwEtcItem = dwCode;
	m_szTitle.Clear();
	
	ioUserEtcItem *pUserEtcItem = g_MyInfo.GetUserEtcItem();
	if( pUserEtcItem )
	{
		ioUserEtcItem::ETCITEMSLOT kSlot;
		if( pUserEtcItem->GetEtcItem( m_dwEtcItem, kSlot ) )
		{
			int iUseType = kSlot.GetUse();
			if( COMPARE( iUseType, ioEtcItemMotion::MOTION_OPTION_1, ioEtcItemMotion::MAX_OPTION ) )
			{
				ioEtcItemMotion *pEtcMotion = dynamic_cast< ioEtcItemMotion * >( g_EtcItemMgr.FindEtcItem( m_dwEtcItem ) );
				if( pEtcMotion )
				{					
					m_szTitle = pEtcMotion->GetMotionOptionTitle( iUseType );
				}
			}
		}
	}

	// 설정된 옵션이 없다.
	if( m_szTitle.IsEmpty() )
	{
		m_szTitle = STR(1);

		m_pCurrentNor = m_pGrayNor;
		m_pCurrentPush= m_pGrayPush;
		m_bSelected   = false;
	}
	else
	{
		// 설정된 옵션이 있음
		m_pCurrentNor = m_pOrangeNor;
		m_pCurrentPush= m_pOrangePush;
		m_bSelected   = true;
	}
}

void InventoryEtcMotionBtn::OnDrawNormal( int iXPos, int iYPos )
{
	if( m_pCurrentNor )
	{
		m_pCurrentNor->Render( iXPos, iYPos );
	}

	OnDrawTitle( iXPos, iYPos );
}

void InventoryEtcMotionBtn::OnDrawOvered( int iXPos, int iYPos )
{
	if( m_pCurrentNor )
	{
		m_pCurrentNor->Render( iXPos, iYPos );
	}	
	
	if( m_pCurrentOver )
	{
		m_pCurrentOver->Render( iXPos, iYPos, UI_RENDER_ADD );
	}

	OnDrawTitle( iXPos, iYPos );
}

void InventoryEtcMotionBtn::OnDrawPushed( int iXPos, int iYPos )
{
	if( m_pCurrentPush )
	{
		m_pCurrentPush->Render( iXPos, iYPos );
	}	

	OnDrawTitle( iXPos, iYPos );
}

void InventoryEtcMotionBtn::OnDrawTitle( int iXPos, int iYPos )
{
	if( !m_bSelected )
	{
		iXPos += (GetWidth() / 2);
		iYPos += 3;

		g_FontMgr.SetTextStyle( TS_NORMAL );
		g_FontMgr.SetAlignType( TAT_CENTER );
		g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
		g_FontMgr.SetBkColor( 0, 0, 0 );
	}
	else
	{
		iXPos += (GetWidth() / 2);
		iYPos += 1;

		g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_3X );
		g_FontMgr.SetAlignType( TAT_CENTER );
		g_FontMgr.SetTextColor( TCT_DEFAULT_WHITE );
		g_FontMgr.SetBkColor( 190, 101, 1 );
	}
	g_FontMgr.PrintText( iXPos, iYPos, FONT_SIZE_11, m_szTitle.c_str() );
}
/////////////////////////////////////////////////////////////////////////

InventoryPresentBtn::InventoryPresentBtn()
{
	m_iPresentBtnType = IPBT_RECV;
}

InventoryPresentBtn::~InventoryPresentBtn()
{
}

void InventoryPresentBtn::SetPresentBtnType( int iType )
{
	m_iPresentBtnType = iType;
}

/////////////////////////////////////////////////////////////////////////

InventoryItemWnd::InventoryItemWnd()
{
	m_bEquip = false;
	m_iMainType = 0;
	m_iMagicCode = 0;
	m_iMagicCodeEx = 0;

	m_iDecoType = 0;
	m_iDecoCode = 0;

	m_pEmptyImg = NULL;
	m_pSelectFrm = NULL;
	m_bSelect    = false;

	m_dwCurIconActionID = INVENTORY_ACTION_NONE;
	m_szCurActionText.Clear();
	m_iCurXGap = 0;
	m_iCurYGap = 0;
	m_pCurActionImg = NULL;

	m_pPlusImg = NULL;
	m_pMinusImg = NULL;
	m_pLeftImg = NULL;
	m_pPlayStage = NULL;
}

InventoryItemWnd::~InventoryItemWnd()
{
	ClearDesc();

	m_pCurActionImg = NULL;

	SAFEDELETE(m_pEmptyImg);
	SAFEDELETE(m_pSelectFrm);

	SAFEDELETE(m_pPlusImg);
	SAFEDELETE(m_pMinusImg);
	SAFEDELETE(m_pLeftImg);
}

void InventoryItemWnd::iwm_hide()
{
	for ( int i = ID_ICON_BTN; i < MAX_BTN ; i++ )
	{
		HideChildWnd( i );
	}

	ItemCustomThumbnailTooltip *pThumbnailTooltip = dynamic_cast<ItemCustomThumbnailTooltip*>(g_GUIMgr.FindWnd( MY_INVENTORY_CUSTOM_TOOLTIP ));
	if( pThumbnailTooltip && pThumbnailTooltip->IsShow() )
	{
		pThumbnailTooltip->HideWnd();
	}

	CostumeCustomThumbnailTooltip *pCostumeThumbnailTooltip = dynamic_cast<CostumeCustomThumbnailTooltip*>(g_GUIMgr.FindWnd( MY_INVENTORY_COSTUME_CUSTOM_TOOLTIP ));
	if( pCostumeThumbnailTooltip && pCostumeThumbnailTooltip->IsShow() )
		pCostumeThumbnailTooltip->HideWnd();
}

void InventoryItemWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();

	switch( dwID )
	{
	case ID_ICON_BTN:
	case ID_EQUIP_BTN:
	case ID_RELEASE_BTN:
		if( cmd == IOBN_BTNUP ||
			cmd == IOWN_OVERED||
			cmd == IOWN_LEAVED )
		{
			if( GetParent() )
				GetParent()->iwm_command( this, cmd, m_dwCurIconActionID );
		}
		break;
	case ID_RESELL_BTN:
	case ID_RESELL_MEDAL_BTN:
		if( cmd == IOBN_BTNUP ||
			cmd == IOWN_OVERED||
			cmd == IOWN_LEAVED )
		{
			if( GetParent() )
				GetParent()->iwm_command( this, cmd, INVENTORY_ACTION_RESELL );
		}
		break;
	case ID_DISASSEMBLE_BTN:
		if( cmd == IOBN_BTNUP ||
			cmd == IOWN_OVERED||
			cmd == IOWN_LEAVED )
		{
			if( GetParent() )
				GetParent()->iwm_command( this, cmd, INVENTORY_ACTION_DISASSEMBLE );
		}
		break;
	case ID_USE_ON_BTN:
	case ID_USE_OFF_BTN:
		if( cmd == IOBN_BTNDOWN )
		{
			SendEtcItemOnOff( dwID );
			if( GetParent() )
				GetParent()->iwm_command( this, IOBN_BTNUP, m_dwCurIconActionID );
		}
		break;
	case ID_CUSTOM_INFO_BTN:
		if( cmd == IOBN_BTNUP )
		{
			if( GetParent() )
				GetParent()->iwm_command( this, cmd, INVENTORY_ACTION_CUSTOM_INFO );
		}
		break;
	case ID_ETC_MOTION_BTN:
		if( cmd == IOBN_BTNUP )
		{
			EtcMotionSelectWnd *pSelectWnd = dynamic_cast< EtcMotionSelectWnd * >( g_GUIMgr.FindWnd( ETC_MOTION_SELECT_WND ) );
			if( pSelectWnd )
			{
				pSelectWnd->ShowSelectWnd( pWnd );
			}
		}
		break;

	case ID_ETC_SLOT_BTN:
		if( cmd == IOBN_BTNUP )
		{
			EtcItemSlotWnd *pSlotWnd = dynamic_cast< EtcItemSlotWnd * >( g_GUIMgr.FindWnd( ETC_ITEM_SLOT_WND ) );
			if(pSlotWnd->IsShow())
				pSlotWnd->GoToTop();
			else
				pSlotWnd->ShowSlotWnd();
		}
		break;
	case ID_PRESENT_BTN:
		if( cmd == IOBN_BTNUP )
		{
			InventoryPresentBtn *pPresentBtn = dynamic_cast< InventoryPresentBtn * >( FindChildWnd(ID_PRESENT_BTN) );
			if( pPresentBtn )
			{
				MyInventoryWnd *pInven = dynamic_cast<MyInventoryWnd*> ( g_GUIMgr.FindWnd( MY_INVENTORY_WND ) );

				if( pInven )
				{
					pInven->iwm_command( this, cmd, m_dwCurIconActionID );

					if( pPresentBtn->GetButtonType() == InventoryPresentBtn::IPBT_RECV )
					{
						pInven->CheckDefaultActionBtn();
					}
				}
			}
		}
		break;
	case ID_ADDED_BTN:
		{
			if( cmd == IOBN_BTNUP )
			{
				ioPullDownList *pList = (ioPullDownList *)( FindChildWnd( ID_POWERUP_LIST ) );
				if( pList )
				{
					pList->OpenList( pWnd );
					g_PowerUpManager.GetMenuList( pList );
				}
			}
		}
		break;
	}

}

void InventoryItemWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "empty_image" )
	{
		SAFEDELETE( m_pEmptyImg );
		m_pEmptyImg = pImage;
	}
	else if( szType == "plus_image" )
	{
		SAFEDELETE( m_pPlusImg );
		m_pPlusImg = pImage;
	}
	else if( szType == "minus_image" )
	{
		SAFEDELETE( m_pMinusImg );
		m_pMinusImg = pImage;
	}
	else if( szType == "left_image" )
	{
		SAFEDELETE( m_pLeftImg );
		m_pLeftImg = pImage;
	}
	else
	{
		ioButton::AddRenderImage( szType, pImage );
	}
}

void InventoryItemWnd::AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame )
{
	if( pFrame )
	{
		if( szType == "SelectFrm" )
		{
			SAFEDELETE( m_pSelectFrm );
			m_pSelectFrm = pFrame;
		}
		else
		{	
			ioButton::AddRenderFrame( szType, pFrame );
		}
	}
}

void InventoryItemWnd::SetBlank()
{
	m_ItemInfo.Init();

	m_bEquip = false;
	m_bActive = true;
	m_iMainType = 0;
	m_iMagicCode = 0;
	m_iMagicCodeEx = 0;

	m_iDecoType = 0;
	m_iDecoCode = 0;

	m_dwCurIconActionID = INVENTORY_ACTION_NONE;
	m_szCurActionText.Clear();
	m_iCurXGap = 0;
	m_iCurYGap = 0;
	m_pCurActionImg = NULL;

	SetInActive();

	for (int i = ID_ICON_BTN; i < MAX_BTN ; i++)
	{
		HideChildWnd( i );
	}

	ClearDesc();
}

void InventoryItemWnd::ClearDesc()
{
	for (int i = 0; i < MAX_DESC ; i++)
	{
		m_szDescArray[i].ClearList();
	}
}

bool InventoryItemWnd::IsCustomToolTip()
{
	if( g_GUIMgr.GetPreOverWnd() == FindChildWnd( ID_CUSTOM_INFO_BTN ) )
		return true;
	return false;
}

void InventoryItemWnd::OnRender()
{
	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	if( HasWndStyle( IWS_INACTIVE ) )
	{	
		if( m_pDisableFrm )
			m_pDisableFrm->Render( iXPos, iYPos );
		else if( m_pNormalFrm )
			m_pNormalFrm->Render( iXPos, iYPos );

		// 기타 이미지 추가.
		if( m_pEmptyImg )
		{
			m_pEmptyImg->Render( iXPos+52, iYPos+49, UI_RENDER_MULTIPLY );
		}
	}
	else
	{
		ioButton::OnRender();

		if( m_pMultiplyFrm )
			m_pMultiplyFrm->Render( iXPos, iYPos, UI_RENDER_MULTIPLY );

		/*
		if( m_iMainType != INVENTORY_ITEM_EXTRAITEM )
		{
			if( m_pMultiplyFrm )
				m_pMultiplyFrm->Render( iXPos, iYPos, UI_RENDER_MULTIPLY );
		}
		*/

		if( m_bSelect )
		{
			if( m_pSelectFrm )
				m_pSelectFrm->Render( iXPos, iYPos );
		}

		if( !m_szCurActionText.IsEmpty() && m_pCurActionImg )
		{
			int iImgSize = m_pCurActionImg->GetWidth();
			int iTextSize = g_FontMgr.GetTextWidth( m_szCurActionText.c_str(), TS_NORMAL, FONT_SIZE_12 );

			int iTotalSize = iTextSize + m_iCurXGap + iImgSize;
			int iHalfSize = iTotalSize / 2;

			int iNewXPos = iXPos + 52;
			int iNewYPos = iYPos + 157;

			int iCurXPos = iNewXPos;

			// 텍스트
			iCurXPos -= iHalfSize;
			g_FontMgr.SetTextStyle( TS_NORMAL );
			g_FontMgr.SetAlignType( TAT_LEFT );
			g_FontMgr.SetBkColor( 0, 0, 0 );
			g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
			g_FontMgr.PrintText( iCurXPos, iNewYPos, FONT_SIZE_12, STR(1), m_szCurActionText.c_str() );

			// 이미지
			iCurXPos += (iTextSize + m_iCurXGap);
			m_pCurActionImg->SetReverseFormatting( ioUIRenderImage::RF_HORZ_REVERSE );
			m_pCurActionImg->Render( iCurXPos, iNewYPos+m_iCurYGap );
		}
	}

	RenderDesc( iXPos, iYPos );
}

void InventoryItemWnd::RenderDesc( int iXPos, int iYPos )
{
	enum 
	{
		X_OFFSET           = 52,
		Y_OFFSET           = 105,
		Y_LINE_GAP         = 16,
		Y_OFFSET_SECOND    = -8,
	};

	iXPos += X_OFFSET;
	iYPos += Y_OFFSET;


	if( HasWndStyle( IWS_INACTIVE ) )
	{
		for( int i = 0 ; i < MAX_DESC ; i++ )
		{
			m_szDescArray[i].ClearList();
		}

		m_szDescArray[0].SetTextStyle( TS_NORMAL );
		m_szDescArray[0].SetBkColor( 0, 0, 0 );	
		m_szDescArray[0].SetTextColor( TCT_DEFAULT_GRAY );
		m_szDescArray[0].AddTextPiece( FONT_SIZE_13, STR(1) );
		m_szDescArray[0].PrintFullTextWidthCut( iXPos, iYPos, TAT_CENTER, INVENTORY_ITEM_INFO_WIDTHCUT_SIZE );

		m_szDescArray[1].SetTextStyle( TS_NORMAL );
		m_szDescArray[1].SetBkColor( 0, 0, 0 );	
		m_szDescArray[1].SetTextColor( TCT_DEFAULT_GRAY );
		m_szDescArray[1].AddTextPiece( FONT_SIZE_11, "-" );
		m_szDescArray[1].PrintFullTextWidthCut( iXPos, iYPos+Y_LINE_GAP, TAT_CENTER, INVENTORY_ITEM_INFO_WIDTHCUT_SIZE );
	}
	else
	{
		int iDescCnt = 0;
		for( int i = 0 ; i < MAX_DESC ; i++ )
		{
			if( m_szDescArray[i].IsEmpty() )
				continue;

			iDescCnt++;
		}

		if( iDescCnt == 3 )
			iYPos += Y_OFFSET_SECOND;

		int iCnt  = 0;
		for( int i = 0 ; i < MAX_DESC ; i++ )
		{
			if( m_szDescArray[i].IsEmpty() )
				continue;

			m_szDescArray[i].PrintFullTextWidthCut( iXPos, iYPos + ( Y_LINE_GAP * iCnt ), TAT_CENTER, INVENTORY_ITEM_INFO_WIDTHCUT_SIZE );
			iCnt++;
		}
	}
}

void InventoryItemWnd::SetInfo( ioPlayStage *pPlayStage, InventoryItemInfo &rkInfo )
{
	SetBlank();
	
	m_pPlayStage = pPlayStage;

	ioHashString szName, szIcon;

	m_bEquip = rkInfo.m_bEquip;
	m_bActive = rkInfo.m_bActive;

	m_iMainType = rkInfo.m_iMainType;
	m_iMagicCode = rkInfo.m_iMagicCode;
	m_iMagicCodeEx = rkInfo.m_iMagicCodeEx;

	SetActive();

	switch( m_iMainType )
	{
	case INVENTORY_ITEM_EXTRAITEM:
		{
			if( !SetExtraItemInfo() )
			{
				SetBlank();
				return;
			}
		}
		break;
	case INVENTORY_ITEM_DECO:
		{
			if( !SetDecoItemInfo( rkInfo ) )
			{
				SetBlank();
				return;
			}
		}
		break;
	case INVENTORY_ITEM_ETC:
		{
			if( !SetEtcItemInfo() )
			{
				SetBlank();
				return;
			}
		}
		break;
	case INVENTORY_ITEM_PRESENT:
		{
			if( !SetPresentItemInfo() )
			{
				SetBlank();
				return;
			}
		}
		break;
	case INVENTORY_ITEM_MEDALITEM:
		{
			if( !SetMedalItemInfo() )
			{
				SetBlank();
				return;
			}
		}
		break;
	case INVENTORY_ITEM_SUBSCRIPTION:
		{
			if( !SetSubscriptionItemInfo( rkInfo ) )
			{
				SetBlank();
				return;
			}
		}
		break;
	case INVENTORY_ITEM_COSTUME:
		{
			// UI코스튬 관련 (정보 확인)
			if( !SetCostumeInfo() )
			{
				SetBlank();
				return;
			}
		}
		break;
	case INVENTORY_ITEM_ACCESSORY:
		if( !SetAccessoryInfo() )
		{
			SetBlank();
			return;
		}
		break;
	default:
		SetBlank();
		return;
	}

	m_ItemInfo = rkInfo;
}

void InventoryItemWnd::SetSelect( bool bSelect )
{
	if( m_iMainType == INVENTORY_ITEM_EXTRAITEM ||
		m_iMainType == INVENTORY_ITEM_MEDALITEM ||
		m_iMainType == INVENTORY_ITEM_DECO ||
		m_iMainType == INVENTORY_ITEM_COSTUME ||
		m_iMainType == INVENTORY_ITEM_ACCESSORY )
	{
		m_bSelect = false;
	}
	else
	{
		m_bSelect = bSelect;
	}
}

InventoryItemInfo& InventoryItemWnd::GetInfo()
{
	return m_ItemInfo;
}

bool InventoryItemWnd::SetExtraItemInfo()
{
	if( m_iMagicCode <= 0 )
		return false;

	EXTRAITEMSLOT kSlot;
	ioUserExtraItem *pExtraItem = g_MyInfo.GetUserExtraItem();
	if( !pExtraItem )
		return false;

	if( !pExtraItem->GetExtraItem( m_iMagicCode, kSlot ) )
		return false;

	// icon btn
	ShowChildWnd( ID_ICON_BTN );

	if( m_bEquip )
	{
		ShowChildWnd( ID_RELEASE_BTN );

		m_dwCurIconActionID = INVENTORY_ACTION_RELEASE;
	}
	else
	{
		ioHashString szError;
		if ( g_PowerUpManager.CheckEnableItemPowerUp( kSlot.m_iIndex, szError ) )
		{
			ShowChildWnd( ID_ADDED_BTN );
		}
		else
		{
			ShowChildWnd( ID_RESELL_BTN );
			//ShowChildWnd( ID_EQUIP_BTN );
			ShowChildWnd( ID_DISASSEMBLE_BTN );
		}
		

		m_dwCurIconActionID = INVENTORY_ACTION_EQUIP;
	}

	if( kSlot.m_dwMaleCustom > 0 || kSlot.m_dwFemaleCustom > 0 )
	{
		ShowChildWnd( ID_CUSTOM_INFO_BTN );
	}

	InventoryIconBtn *pBtn = dynamic_cast< InventoryIconBtn* > ( FindChildWnd( ID_ICON_BTN ) );
	if( !pBtn )
	{
		LOG.PrintTimeAndLog( 0, "%s pBtn == NULL.", __FUNCTION__ );
	}
	else
	{
		ioUIRenderImage *pImg = NULL;
		pImg = g_UIImageSetMgr.CreateImageByFullName( kSlot.m_IconName );
		pBtn->SetIcon( pImg, FLOAT1, FLOAT1, kSlot.m_iReinforce );

		if( m_bEquip )
		{
			pBtn->SetMarkType( INVENTORY_MARK_TYPE_EQUIP );
			pBtn->SetInActive();
		}
		else if( kSlot.m_iTradeState == EET_NORMAL && kSlot.m_PeriodType == EPT_MORTMAIN )
		{
			pBtn->SetMarkType( INVENTORY_MARK_TYPE_TRADE_WAIT );
			pBtn->SetActive();
		}
		else if( kSlot.m_iTradeState == EET_ENABLE && kSlot.m_PeriodType == EPT_MORTMAIN )
		{
			pBtn->SetMarkType( INVENTORY_MARK_TYPE_TRADE_ENABLE );
			pBtn->SetActive();
		}
		else
		{
			pBtn->SetMarkType( INVENTORY_MARK_TYPE_NONE );
			pBtn->SetActive();
		}

		if( kSlot.m_PeriodType == EPT_GROW_TIME )
			pBtn->SetExtraItemGrowthCatalyst( true );
	}


	// desc
	ClearDesc();
	SetIsWidthCutDesc( 0, true );
	SetIsWidthCutDesc( 2, true );

	// Name
	m_szDescArray[0].SetTextStyle( TS_NORMAL );
	m_szDescArray[0].SetBkColor( 0, 0, 0 );	
	m_szDescArray[0].SetTextColor( TCT_DEFAULT_DARKGRAY ); // 진회
	m_szDescArray[0].AddTextPiece( FONT_SIZE_13, STR(1), kSlot.m_Name.c_str() );

	// Limit
	int iLimitLevel = kSlot.m_iReinforce * g_ExtraItemInfoMgr.GetLevelLimitConst();
	int iEnableClass = 0;
	int iOnlySexType = ioSetItemInfo::CST_ALL;
	int iPowerUpItemGrade = ConvertPowerUpItemToItemGrade( kSlot.m_iItemCode );

	const ioItem *pItem = g_ItemMaker.GetItemConst( kSlot.m_iItemCode, __FUNCTION__ );
	if( pItem )
	{
		iEnableClass = pItem->GetEnableClass();
		iOnlySexType = pItem->GetCharOnlySexType();

		if ( pBtn )
			pBtn->SetGradeType( pItem->GetGradeType() );
	}

	m_szDescArray[2].SetTextStyle( TS_NORMAL );
	m_szDescArray[2].SetBkColor( 0, 0, 0 );	
	m_szDescArray[2].SetTextColor( TCT_DEFAULT_GRAY );

	if( iEnableClass > 0 )
	{
		switch( iOnlySexType )
		{
		case ioSetItemInfo::CST_ALL:
			//m_szDescArray[2].AddTextPiece( FONT_SIZE_11, STR(2), iLimitLevel, g_MyInfo.GetClassName( iEnableClass, ConvertPowerUpItemToChar( iPowerUpItemGrade ) ) );
			m_szDescArray[2].AddTextPiece( FONT_SIZE_11, STR(8), g_MyInfo.GetClassName( iEnableClass, ConvertPowerUpItemToChar( iPowerUpItemGrade ) ) );
			break;
		case ioSetItemInfo::CST_MALE:
			m_szDescArray[2].AddTextPiece( FONT_SIZE_11, STR(4), iLimitLevel, g_MyInfo.GetClassName( iEnableClass, ConvertPowerUpItemToChar( iPowerUpItemGrade )  ) );
			break;
		case ioSetItemInfo::CST_FEMALE:
			m_szDescArray[2].AddTextPiece( FONT_SIZE_11, STR(5), iLimitLevel, g_MyInfo.GetClassName( iEnableClass, ConvertPowerUpItemToChar( iPowerUpItemGrade )  ) );
			break;
		}
	}
	else
	{
		switch( iOnlySexType )
		{
		case ioSetItemInfo::CST_ALL:
			m_szDescArray[2].AddTextPiece( FONT_SIZE_11, STR(3), iLimitLevel );
			break;
		case ioSetItemInfo::CST_MALE:
			m_szDescArray[2].AddTextPiece( FONT_SIZE_11, STR(6), iLimitLevel );
			break;
		case ioSetItemInfo::CST_FEMALE:
			m_szDescArray[2].AddTextPiece( FONT_SIZE_11, STR(7), iLimitLevel );
			break;
		}
	}

	// 1라인은(기간) 실시간 갱신

	return true;
}

bool InventoryItemWnd::SetDecoItemInfo( InventoryItemInfo &rkInfo )
{
	int iClassType = g_MyInfo.GetClassType();
	MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(GetParent());
	if( pInvenWnd )
	{
		iClassType = pInvenWnd->GetSelectClassType();
	}

	int iSexType = rkInfo.m_iSexType;
	m_iDecoType = rkInfo.m_iDecoType;
	m_iDecoCode = rkInfo.m_iDecoCode;

	const DecoData *pData =  g_DecorationPrice.GetDecoDataPtr( iSexType, m_iDecoType, m_iDecoCode );
	if( !pData ) return false;

	// icon btn
	ioUIRenderImage *pImg = NULL;
	// 종족 아이콘은 용병 아이콘을 사용한다.
	if( m_iDecoType == UID_KINDRED )
	{
		bool bMale = true;
		// 여성 캐릭터 타입
		switch( m_iDecoCode )
		{
		case RDT_HUMAN_WOMAN:
		case RDT_ELF_WOMAN:
		case RDT_DWARF_WOMAN:
			bMale = false;
			break;
		}

		pImg = g_MyInfo.GetSoldierIcon( iClassType, bMale );
	}
	else
	{
		pImg = g_UIImageSetMgr.CreateImageByFullName( pData->m_szIcon );
	}

	InventoryIconBtn *pBtn = dynamic_cast< InventoryIconBtn* > ( FindChildWnd( ID_ICON_BTN ) );
	if( pBtn )
	{
		pBtn->SetIcon( pImg, FLOAT1, FLOAT1 );

		if( m_bEquip )
		{
			pBtn->SetMarkType( INVENTORY_MARK_TYPE_EQUIP );
			pBtn->SetInActive();
		}
		else
		{
			pBtn->SetMarkType( INVENTORY_MARK_TYPE_NONE );
			pBtn->SetActive();
		}

		ShowChildWnd( ID_ICON_BTN );
	}
	else
	{
		LOG.PrintTimeAndLog( 0, "%s pBtn == NULL.", __FUNCTION__ );
	}

	if( m_bEquip )
	{
		m_szCurActionText = STR(1);
		m_pCurActionImg = m_pMinusImg;

		m_iCurXGap = 0;
		m_iCurYGap = -1;

		m_dwCurIconActionID = INVENTORY_ACTION_RELEASE;
	}
	else
	{
		m_szCurActionText = STR(2);
		m_pCurActionImg = m_pPlusImg;

		m_iCurXGap = 0;
		m_iCurYGap = -1;

		m_dwCurIconActionID = INVENTORY_ACTION_EQUIP;
	}

	// desc
	ClearDesc();
	SetIsWidthCutDesc( 0, true );

	m_szDescArray[0].SetTextStyle( TS_NORMAL );
	m_szDescArray[0].SetBkColor( 0, 0, 0 );	
	m_szDescArray[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
	m_szDescArray[0].AddTextPiece( FONT_SIZE_13, pData->m_szName.c_str() );

	m_szDescArray[1].SetTextStyle( TS_NORMAL );
	m_szDescArray[1].SetBkColor( 0, 0, 0 );	
	m_szDescArray[1].SetTextColor( TCT_DEFAULT_BLUE );
	m_szDescArray[1].AddTextPiece( FONT_SIZE_13, STR(3) );

	return true;
}

bool InventoryItemWnd::SetMedalItemInfo()
{
	int iMedalItemType = m_iMagicCode;

	if( iMedalItemType <= 0 )
		return false;

	ioUserMedalItem *pMedaltem = g_MyInfo.GetUserMedalItem();
	if( !pMedaltem )
		return false;

	ioUserMedalItem::MEDALITEMSLOT kSlot;
	if( !pMedaltem->GetMedalItem( iMedalItemType, kSlot ) )
		return false;

	const ioMedalItemInfoManager::ItemInfo *pItemInfo = g_MedalItemMgr.GetItemInfo( iMedalItemType );
	if( !pItemInfo )
		return false;

	// icon btn
	ShowChildWnd( ID_ICON_BTN );

	if( m_bEquip )
	{
		m_dwCurIconActionID = INVENTORY_ACTION_RELEASE;

		m_szCurActionText = STR(1);
		m_pCurActionImg = m_pMinusImg;

		m_iCurXGap = 0;
		m_iCurYGap = -1;
	}
	else
	{
		m_dwCurIconActionID = INVENTORY_ACTION_EQUIP;

		m_pCurActionImg = m_pPlusImg;

		m_iCurXGap = 0;
		m_iCurYGap = -1;

		ShowChildWnd( ID_RESELL_MEDAL_BTN );
	}

	InventoryIconBtn *pBtn = dynamic_cast< InventoryIconBtn* > ( FindChildWnd( ID_ICON_BTN ) );
	if( !pBtn )
	{
		LOG.PrintTimeAndLog( 0, "%s pBtn == NULL.", __FUNCTION__ );
	}
	else
	{
		ioUIRenderImage *pImg = NULL;
		pImg = g_UIImageSetMgr.CreateImageByFullName( pItemInfo->m_sIcon );
		pBtn->SetIcon( pImg, FLOAT1, FLOAT1, 0 );

		if( m_bEquip )
		{
			pBtn->SetMarkType( INVENTORY_MARK_TYPE_EQUIP );
			pBtn->SetInActive();
		}
		else
		{
			pBtn->SetMarkType( INVENTORY_MARK_TYPE_NONE );
			pBtn->SetActive();
		}
	}


	// desc
	ClearDesc();
	SetIsWidthCutDesc( 0, true );
	SetIsWidthCutDesc( 2, true );

	// Name
	m_szDescArray[0].SetTextStyle( TS_NORMAL );
	m_szDescArray[0].SetBkColor( 0, 0, 0 );	
	m_szDescArray[0].SetTextColor( TCT_DEFAULT_DARKGRAY ); // 진회
	m_szDescArray[0].AddTextPiece( FONT_SIZE_13, STR(3), pItemInfo->m_sName.c_str() );

	// Limit
	int iLimitLevel = g_MedalItemMgr.GetLevelLimit( iMedalItemType );

	m_szDescArray[2].SetTextStyle( TS_NORMAL );
	m_szDescArray[2].SetBkColor( 0, 0, 0 );	
	m_szDescArray[2].SetTextColor( TCT_DEFAULT_GRAY );
	m_szDescArray[2].AddTextPiece( FONT_SIZE_11, STR(4), iLimitLevel );

	// 1라인은(기간) 실시간 갱신

	return true;
}

int InventoryItemWnd::GetDecoType()
{
	if( m_iMainType == INVENTORY_ITEM_DECO )
		return m_iDecoType;

	return 0;
}

int InventoryItemWnd::GetDecoCode()
{
	if( m_iMainType == INVENTORY_ITEM_DECO )
		return m_iDecoCode;

	return 0;
}

bool InventoryItemWnd::SetEtcItemInfo()
{
	if( m_iMagicCode <= 0 )
		return false;

	ioEtcItem *pEtcItem = g_EtcItemMgr.FindEtcItem( m_iMagicCode );
	if( !pEtcItem )
	{
		LOG.PrintTimeAndLog( 0, "%s ioEtcItem == NULL , Type:%d", __FUNCTION__, m_iMagicCode );
		return false;
	}

	ioUserEtcItem *pUserEtcItem =  g_MyInfo.GetUserEtcItem();
	if( !pUserEtcItem )
	{
		LOG.PrintTimeAndLog( 0, "%s UserEtcItem == NULL , Type:%d", __FUNCTION__, m_iMagicCode );
		return false;
	}

	ioUserEtcItem::ETCITEMSLOT kItemSlot;
	if( !pUserEtcItem->GetEtcItem( m_iMagicCode, kItemSlot ) )
	{
		LOG.PrintTimeAndLog( 0, "%s don't have , Type:%d", __FUNCTION__, m_iMagicCode );
		return false;
	}

	// icon btn
	ioUIRenderImage *pImg = g_UIImageSetMgr.CreateImageByFullName( pEtcItem->GetIconName() );
	InventoryIconBtn *pBtn = dynamic_cast< InventoryIconBtn* > ( FindChildWnd( ID_ICON_BTN ) );
	if( !pBtn )
	{
		LOG.PrintTimeAndLog( 0, "%s pBtn == NULL , Type:%d", __FUNCTION__, m_iMagicCode );
	}
	else
	{
		pBtn->SetIcon( pImg, FLOAT1, FLOAT1 );
		pBtn->SetGradeType( pEtcItem->GetGradeType() );
		pBtn->SetMarkType( INVENTORY_MARK_TYPE_NONE );
		pBtn->SetActive();
		pBtn->SetEtcItemType( m_iMagicCode );
	}

	ShowChildWnd( ID_ICON_BTN );
	m_dwCurIconActionID = INVENTORY_ACTION_DETAIL;

	if( pEtcItem->IsCanOnOff() )
	{
		ShowChildWnd( ID_USE_ON_BTN );
		ShowChildWnd( ID_USE_OFF_BTN );
		if( kItemSlot.IsUse() )
			CheckRadioButton( ID_USE_ON_BTN, ID_USE_OFF_BTN, ID_USE_ON_BTN );
		else
			CheckRadioButton( ID_USE_ON_BTN, ID_USE_OFF_BTN, ID_USE_OFF_BTN );
	}
	else if( COMPARE( pEtcItem->GetType(), ioEtcItem::EIT_ETC_MOTION1, ioEtcItem::EIT_ETC_MOTION100 + 1 ) ||
		     COMPARE( pEtcItem->GetType(), ioEtcItem::EIT_ETC_MOTION101, ioEtcItem::EIT_ETC_MOTION400 + 1 ) )
	{
		ioEtcItemMotion *pEtcMotion = dynamic_cast< ioEtcItemMotion * > ( pEtcItem );
		if( pEtcMotion->GetMaxMotionOption() > 0 )
		{
			InventoryEtcMotionBtn *pMotionBtn = dynamic_cast< InventoryEtcMotionBtn * >( FindChildWnd( ID_ETC_MOTION_BTN ) );
			if( pMotionBtn )
			{
				pMotionBtn->SetEtcCode( pEtcItem->GetType() );
				pMotionBtn->ShowWnd();
			}
		}
		else
		{
			m_szCurActionText = STR(1);
			m_pCurActionImg = m_pLeftImg;		
		}
	}
	else if( COMPARE( pEtcItem->GetType(), ioEtcItem::EIT_ETC_CONSUMPTION_BUFF01, ioEtcItem::EIT_ETC_CONSUMPTION_REVIVE + 1 ) )
	{
		ioButton *pSlotBtn = dynamic_cast< ioButton * >( FindChildWnd( ID_ETC_SLOT_BTN ) );
		if( pSlotBtn )
			pSlotBtn->ShowWnd();
	}
	else
	{
		m_szCurActionText = STR(1);
		m_pCurActionImg = m_pLeftImg;		
	}

	m_iCurXGap = 2;
	m_iCurYGap = 1;

	// desc
	ClearDesc();
	SetIsWidthCutDesc( 0, true );

	m_szDescArray[0].SetTextStyle( TS_NORMAL );
	m_szDescArray[0].SetBkColor( 0, 0, 0 );
	m_szDescArray[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
	m_szDescArray[0].AddTextPiece( FONT_SIZE_13, pEtcItem->GetName().c_str() );

	// 1, 2 라인은(갯수, 기간등등) 실시간 갱신

	return true;
}

void InventoryItemWnd::SendEtcItemOnOff( DWORD dwID )
{
	if( m_iMagicCode <= 0 )
		return;

	CheckRadioButton( ID_USE_ON_BTN, ID_USE_OFF_BTN, dwID );

	ioEtcItem *pEtcItem = g_EtcItemMgr.FindEtcItem( m_iMagicCode );
	if( !pEtcItem )
	{
		LOG.PrintTimeAndLog( 0, "%s ioEtcItem == NULL , Type:%d", __FUNCTION__, m_iMagicCode );
		return;
	}

	if( pEtcItem->IsCanOnOff() )
	{
		if( dwID == ID_USE_ON_BTN )
		{
			if( !pEtcItem->OnUseSwitchBtnPush( g_MyInfo.GetUserEtcItem(), true, m_pPlayStage ) )
			{
				CheckRadioButton( ID_USE_ON_BTN, ID_USE_OFF_BTN, ID_USE_OFF_BTN );
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
			}
		}
		else
		{
			if( !pEtcItem->OnUseSwitchBtnPush( g_MyInfo.GetUserEtcItem(), false, m_pPlayStage ) )
			{
				CheckRadioButton( ID_USE_ON_BTN, ID_USE_OFF_BTN, ID_USE_ON_BTN );
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
			}
		}
	}
}

bool InventoryItemWnd::SetPresentItemInfo()
{
	if( m_iMagicCode <= 0 && m_iMagicCodeEx <= 0 )
		return false;

	ioHashString szSendID;
	short sPresentType, sPresentMent;
	int iPresentValue1, iPresentValue2, iPresentValue3, iPresentValue4;
	DWORD dwLimitDate;

	ShowChildWnd( ID_ICON_BTN );
	m_dwCurIconActionID = INVENTORY_ACTION_DETAIL;

	/*
	m_szCurActionText = STR(1);
	m_pCurActionImg = m_pLeftImg;
	*/

	// New
	ShowChildWnd( ID_PRESENT_BTN );

	m_szCurActionText.Clear();
	m_pCurActionImg = NULL;
	//

	m_iCurXGap = 2;
	m_iCurYGap = 1;

	g_PresentMgr.GetPresentDataToIndex( m_iMagicCode, m_iMagicCodeEx, szSendID, sPresentType, sPresentMent,
										iPresentValue1,	iPresentValue2,	iPresentValue3,	iPresentValue4,	dwLimitDate );

	switch( sPresentType )
	{
	case PRESENT_SOLDIER:
		SetPresentSoldier( iPresentValue1, iPresentValue2 );
		break;
	case PRESENT_DECORATION:
		SetPresentDeco( iPresentValue1, iPresentValue2 );
		break;
	case PRESENT_ETC_ITEM:
		SetPresentEtcItem( iPresentValue1, iPresentValue2 );
		break;
	case PRESENT_PESO:
		SetPresentPeso( iPresentValue1, iPresentValue2 );
		break;
	case PRESENT_EXTRAITEM:
		{
			SetPresentExtraItem( iPresentValue1, iPresentValue2, sPresentMent );
			if( iPresentValue3 > 0 || iPresentValue4 > 0 )
			{
				ShowChildWnd( ID_CUSTOM_INFO_BTN );
			}
		}
		break;
	case PRESENT_EXTRAITEM_BOX:
		SetPresentExtraItemBox( iPresentValue1, iPresentValue2 );
		break;
	case PRESENT_RANDOM_DECO:
		SetPresentRandomDeco( iPresentValue1, iPresentValue2 );
		break;
	case PRESENT_GRADE_EXP:
		SetPresentGradeExp( iPresentValue1, iPresentValue2 );
		break;
	case PRESENT_MEDALITEM:
		SetPresentMedalItem( iPresentValue1, iPresentValue2 );
		break;
	case PRESENT_ALCHEMIC_ITEM:
		SetPresentAlchemicItem( iPresentValue1, iPresentValue2 );
		break;
	case PRESENT_PET:
		SetPresentPet( iPresentValue1, iPresentValue2 );
		break;
	case PRESENT_COSTUME:
		{
			SetPresentCostume( iPresentValue1, iPresentValue2 );
			if( iPresentValue3 > 0 || iPresentValue4 > 0 )
			{
				ShowChildWnd( ID_CUSTOM_INFO_BTN );
			}
		}
		break;
	case PRESENT_COSTUME_BOX:
		SetPresentCostumeBox( iPresentValue1, iPresentValue2 );
		break;
	case PRESENT_BONUS_CASH:
		SetPresentBonusCash( iPresentValue1, iPresentValue2 );
		break;
	case PRESENT_ACCESSORY:
		SetPresentAccessory( iPresentValue1, iPresentValue2 );
		break;
	}

	// desc
	ClearDesc();
	SetIsWidthCutDesc( 0, true );
	SetIsWidthCutDesc( 1, true );

	// 첫 라인 : 이름
	m_szDescArray[0].SetTextStyle( TS_NORMAL );
	m_szDescArray[0].SetBkColor( 0, 0, 0 );	
	m_szDescArray[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
	m_szDescArray[0].AddTextPiece( FONT_SIZE_13, g_PresentMgr.GetPresentValue1Text( sPresentType, iPresentValue1, iPresentValue2 ).c_str() );

	// 두번째 라인 : 기간
	m_szDescArray[1].SetTextStyle( TS_NORMAL );
	m_szDescArray[1].SetBkColor( 0, 0, 0 );
	m_szDescArray[1].SetTextColor( TCT_DEFAULT_GRAY );

	char szUseInfo[MAX_PATH]="";
	if( sPresentType == PRESENT_MEDALITEM )
		SafeSprintf( szUseInfo, sizeof( szUseInfo ), STR(2),  g_PresentMgr.GetPresentValue2Text( sPresentType, iPresentValue1, iPresentValue2 ).c_str() );
	else if( sPresentType == PRESENT_PET )
		SafeSprintf( szUseInfo, sizeof( szUseInfo ), STR(1), iPresentValue2/10000, g_PresentMgr.GetPresentValue2Text( sPresentType, iPresentValue1, iPresentValue2 ).c_str() );
	else
		StringCbCopy( szUseInfo, sizeof( szUseInfo ), g_PresentMgr.GetPresentValue2Text( sPresentType, iPresentValue1, iPresentValue2 ).c_str() );
	m_szDescArray[1].AddTextPiece( FONT_SIZE_11,  szUseInfo );

	// 세번째 라인 : 선물 기간
	m_szDescArray[2].SetTextStyle( TS_NORMAL );
	m_szDescArray[2].SetBkColor( 0, 0, 0 );	
	m_szDescArray[2].SetTextColor( TCT_DEFAULT_GRAY );
	m_szDescArray[2].AddTextPiece( FONT_SIZE_11, g_PresentMgr.GetLmitDateText( dwLimitDate ).c_str() );

	return true;
}

bool InventoryItemWnd::SetSubscriptionItemInfo( InventoryItemInfo &rkInfo )
{
	if( m_iMagicCode <= 0 && m_iMagicCodeEx <= 0 )
		return false;

	ioHashString szSendID;
	short sPresentType, sPresentMent;
	int iPresentValue1, iPresentValue2, iPresentValue3, iPresentValue4;

	ShowChildWnd( ID_ICON_BTN );
	m_dwCurIconActionID = INVENTORY_ACTION_DETAIL;

	m_szCurActionText = STR(1);
	m_pCurActionImg = m_pLeftImg;

	m_iCurXGap = 2;
	m_iCurYGap = 1;

	ioSubscriptionMgr::SubscriptionData kData;
	g_SubscriptionMgr.GetSubscriptionDataToIndex( rkInfo.m_iMagicCode, rkInfo.m_szSubscriptionID, kData );

	sPresentType = kData.m_iPresentType;
	iPresentValue1 = kData.m_iPresentValue1;
	iPresentValue2 = kData.m_iPresentValue2;
	iPresentValue3 = iPresentValue4 = 0;
	sPresentMent = kData.m_iMentType;

	ioHashString szValueText1 = g_PresentMgr.GetPresentValue1Text( kData.m_iPresentType, kData.m_iPresentValue1, kData.m_iPresentValue2 );
	ioHashString szValueText2 = g_PresentMgr.GetPresentValue2Text( kData.m_iPresentType, kData.m_iPresentValue1, kData.m_iPresentValue2 );

	switch( sPresentType )
	{
	case PRESENT_SOLDIER:
		SetPresentSoldier( iPresentValue1, iPresentValue2 );
		break;
	case PRESENT_DECORATION:
		SetPresentDeco( iPresentValue1, iPresentValue2 );
		break;
	case PRESENT_ETC_ITEM:
		SetPresentEtcItem( iPresentValue1, iPresentValue2 );
		break;
	case PRESENT_PESO:
		SetPresentPeso( iPresentValue1, iPresentValue2 );
		break;
	case PRESENT_EXTRAITEM:
		{
			SetPresentExtraItem( iPresentValue1, iPresentValue2, sPresentMent );
			if( iPresentValue3 > 0 || iPresentValue4 > 0 )
			{
				ShowChildWnd( ID_CUSTOM_INFO_BTN );
			}
		}
		break;
	case PRESENT_EXTRAITEM_BOX:
		SetPresentExtraItemBox( iPresentValue1, iPresentValue2 );
		break;
	case PRESENT_RANDOM_DECO:
		SetPresentRandomDeco( iPresentValue1, iPresentValue2 );
		break;
	case PRESENT_GRADE_EXP:
		SetPresentGradeExp( iPresentValue1, iPresentValue2 );
		break;
	case PRESENT_MEDALITEM:
		SetPresentMedalItem( iPresentValue1, iPresentValue2 );
		break;
	case PRESENT_ALCHEMIC_ITEM:
		SetPresentAlchemicItem( iPresentValue1, iPresentValue2 );
		break;
	case PRESENT_PET:
		SetPresentPet( iPresentValue1, iPresentValue2 );
		break;
	case PRESENT_COSTUME:
		{
			SetPresentCostume( iPresentValue1, iPresentValue2 );
			if( iPresentValue3 > 0 || iPresentValue4 > 0 )
			{
				ShowChildWnd( ID_CUSTOM_INFO_BTN );
			}
		}
		break;
	case PRESENT_COSTUME_BOX:
		SetPresentCostumeBox( iPresentValue1, iPresentValue2 );
		break;
	case PRESENT_BONUS_CASH:
		SetPresentBonusCash( iPresentValue1, iPresentValue2 );
		break;
	}

	// desc
	ClearDesc();
	SetIsWidthCutDesc( 0, true );
	SetIsWidthCutDesc( 1, true );

	// 첫 라인 : 이름
	m_szDescArray[0].SetTextStyle( TS_NORMAL );
	m_szDescArray[0].SetBkColor( 0, 0, 0 );	
	m_szDescArray[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
	m_szDescArray[0].AddTextPiece( FONT_SIZE_13, szValueText1.c_str() );

	// 두번째 라인 : 기간
	m_szDescArray[1].SetTextStyle( TS_NORMAL );
	m_szDescArray[1].SetBkColor( 0, 0, 0 );
	m_szDescArray[1].SetTextColor( TCT_DEFAULT_GRAY );

	char szUseInfo[MAX_PATH]="";
	if( sPresentType == PRESENT_MEDALITEM )
		SafeSprintf( szUseInfo, sizeof( szUseInfo ), STR(2),  szValueText2.c_str() );
	else
		StringCbCopy( szUseInfo, sizeof( szUseInfo ), szValueText2.c_str() );
	m_szDescArray[1].AddTextPiece( FONT_SIZE_11, szUseInfo );

	// 세번째 라인 : 선물 기간
	m_szDescArray[2].SetTextStyle( TS_NORMAL );
	m_szDescArray[2].SetBkColor( 0, 0, 0 );	
	m_szDescArray[2].SetTextColor( TCT_DEFAULT_GRAY );
	m_szDescArray[2].AddTextPiece( FONT_SIZE_11, STR(3) );

	return true;
}

// UI코스튬 관련 (코스튬 인벤토리 해당 아이템 버튼 관련)
bool InventoryItemWnd::SetCostumeInfo()
{
	if( m_iMagicCode <= 0 )
		return false;

	CostumeSlot kSlot;
	ioUserCostume *pCostume = g_MyInfo.GetUserCostume();
	if( !pCostume )
		return false;

	if( !pCostume->GetCostumeSlot( m_iMagicCode, kSlot ) )
		return false;

	// icon btn
	ShowChildWnd( ID_ICON_BTN );

	if( m_bEquip )
	{
		ShowChildWnd( ID_RELEASE_BTN );

		m_dwCurIconActionID = INVENTORY_ACTION_RELEASE;
	}
	else
	{
		ShowChildWnd( ID_RESELL_MEDAL_BTN );

		m_dwCurIconActionID = INVENTORY_ACTION_EQUIP;
	}

	if( kSlot.m_dwMaleCustom > 0 || kSlot.m_dwFemaleCustom > 0 )
	{
		ShowChildWnd( ID_CUSTOM_INFO_BTN );
	}

	InventoryIconBtn *pBtn = dynamic_cast< InventoryIconBtn* > ( FindChildWnd( ID_ICON_BTN ) );
	if( !pBtn )
	{
		LOG.PrintTimeAndLog( 0, "%s pBtn == NULL.", __FUNCTION__ );
	}
	else
	{
		ioUIRenderImage *pImg = NULL;
		pImg = g_UIImageSetMgr.CreateImageByFullName( kSlot.m_IconName );
		pBtn->SetIcon( pImg, 1.0f, 1.0f, 0 );
		pBtn->SetGradeType( g_CostumeInfoMgr.GetGradeType( kSlot.m_nCostumeCode ) );
		pBtn->SetCostume( true );

		if( m_bEquip )
		{
			pBtn->SetMarkType( INVENTORY_MARK_TYPE_EQUIP );
			pBtn->SetInActive();
		}
		else
		{
			pBtn->SetMarkType( INVENTORY_MARK_TYPE_NONE );
			pBtn->SetActive();
		}
	}

	// desc
	ClearDesc();
	SetIsWidthCutDesc( 0, true );
	SetIsWidthCutDesc( 2, true );

	// Name
	m_szDescArray[0].SetTextStyle( TS_NORMAL );
	m_szDescArray[0].SetBkColor( 0, 0, 0 );	
	m_szDescArray[0].SetTextColor( TCT_DEFAULT_DARKGRAY ); // 진회
	m_szDescArray[0].AddTextPiece( FONT_SIZE_13, STR(1), kSlot.m_Name.c_str() );

	// Limit
	CostumeInfo sCostumeInfo;
	g_CostumeInfoMgr.GetCostumeInfo( kSlot.m_nCostumeCode, sCostumeInfo );

	m_szDescArray[2].SetTextStyle( TS_NORMAL );
	m_szDescArray[2].SetBkColor( 0, 0, 0 );	
	m_szDescArray[2].SetTextColor( TCT_DEFAULT_GRAY );

	if( sCostumeInfo.m_nEnableClass > 0 )
		m_szDescArray[2].AddTextPiece( FONT_SIZE_11, STR(2), g_MyInfo.GetClassName( sCostumeInfo.m_nEnableClass ) );
	else
		m_szDescArray[2].AddTextPiece( FONT_SIZE_11, STR(3) );

	return true;
}

void InventoryItemWnd::SetIsWidthCutDesc( int iArray , bool bWidthCut )
{
	if( COMPARE( iArray, 0, MAX_DESC ) )
		m_bWidthCutDescArray[iArray] = bWidthCut;
}

bool InventoryItemWnd::IsWidthCutDesc( int iArray )
{
	if( COMPARE( iArray, 0, MAX_DESC ) )
		return m_bWidthCutDescArray[iArray];

	return false;
}

void InventoryItemWnd::SetPresentSoldier( int iValue1, int iValue2 )
{
	// icon btn
	ioUIRenderImage *pImg = g_UIImageSetMgr.CreateImageByFullName( g_MyInfo.GetMySoldierIconName( iValue1 ) );
	InventoryIconBtn *pBtn = dynamic_cast< InventoryIconBtn* > ( FindChildWnd( ID_ICON_BTN ) );
	if( pBtn )
	{
		pBtn->SetIcon( pImg, FLOAT1, FLOAT1 );
		pBtn->SetGradeType( g_UIImageRenderMgr.GetPowerUpGradeType( iValue1 ) );
		pBtn->SetMarkType( INVENTORY_MARK_TYPE_NONE );
		pBtn->SetActive();
	}
}

void InventoryItemWnd::SetPresentDeco( int iValue1, int iValue2 )
{
	int iClassType = iValue1 / 100000;
	int iSexType   = (iValue1 % 100000) / 1000;
	int iDecoType  = iValue1 % 1000;
	const DecoData &kData = g_DecorationPrice.GetDecoData( iSexType, iDecoType, iValue2 );

	// icon btn
	ioUIRenderImage *pImg = g_UIImageSetMgr.CreateImageByFullName( kData.m_szIcon );
	InventoryIconBtn *pBtn = dynamic_cast< InventoryIconBtn* > ( FindChildWnd( ID_ICON_BTN ) );
	if( pBtn )
	{
		pBtn->SetIcon( pImg, FLOAT1, FLOAT1 );
		pBtn->SetMarkType( INVENTORY_MARK_TYPE_NONE );
		pBtn->SetActive();
	}
}

void InventoryItemWnd::SetPresentRandomDeco( int iValue1, int iValue2 )
{
	int iClassType = iValue1 / 100000;
	int iSexType   = (iValue1 % 100000) / 1000;
	int iDecoType  = iValue1 % 1000;

	const DecoData &kData = g_DecorationPrice.GetDecoData( iSexType, iDecoType, iValue2 );

	// icon btn
	ioUIRenderImage *pImg = g_UIImageSetMgr.CreateImageByFullName( kData.m_szIcon );
	InventoryIconBtn *pBtn = dynamic_cast< InventoryIconBtn* > ( FindChildWnd( ID_ICON_BTN ) );
	if( pBtn )
	{
		pBtn->SetIcon( pImg, FLOAT1, FLOAT1 );
		pBtn->SetMarkType( INVENTORY_MARK_TYPE_NONE );
		pBtn->SetActive();
	}
}

void InventoryItemWnd::SetPresentEtcItem( int iValue1, int iValue2 )
{
	ioEtcItem *pEtcItem = g_EtcItemMgr.FindEtcItem( iValue1 );
	if( !pEtcItem ) return;

	// icon btn
	ioUIRenderImage *pImg = g_UIImageSetMgr.CreateImageByFullName( pEtcItem->GetIconName() );
	InventoryIconBtn *pBtn = dynamic_cast< InventoryIconBtn* > ( FindChildWnd( ID_ICON_BTN ) );
	if( pBtn )
	{
		pBtn->SetIcon( pImg, FLOAT1, FLOAT1 );
		pBtn->SetGradeType( pEtcItem->GetGradeType() );
		pBtn->SetMarkType( INVENTORY_MARK_TYPE_NONE );
		pBtn->SetActive();
	}
}

void InventoryItemWnd::SetPresentPeso( int iValue1, int iValue2 )
{
	// icon btn
	ioUIRenderImage *pImg = g_UIImageSetMgr.CreateImageByFullName( ioHashString( "UIIconPack13#quest_002" ) );
	InventoryIconBtn *pBtn = dynamic_cast< InventoryIconBtn* > ( FindChildWnd( ID_ICON_BTN ) );
	if( pBtn )
	{
		pBtn->SetIcon( pImg, FLOAT1, FLOAT1 );
		pBtn->SetMarkType( INVENTORY_MARK_TYPE_NONE );
		pBtn->SetActive();
	}
}

void InventoryItemWnd::SetPresentExtraItem( int iValue1, int iValue2, short sPresentMent )
{
	// icon btn
	const ioItem *pItem = g_ItemMaker.GetItemConst( iValue1, __FUNCTION__ );
	if( pItem )
	{
		ioHashString szIcon = pItem->GetItemLargeIconName();
		if( !szIcon.IsEmpty() )
		{
			InventoryIconBtn *pBtn = dynamic_cast< InventoryIconBtn* > ( FindChildWnd( ID_ICON_BTN ) );
			if( pBtn )
			{
				int iReinforce = ( iValue2 % PRESENT_EXTRAITEM_DIVISION_1 ) / PRESENT_EXTRAITEM_DIVISION_2;
				pBtn->SetIcon( g_UIImageSetMgr.CreateImageByFullName( szIcon ), FLOAT1, FLOAT1, iReinforce );
				pBtn->SetGradeType( pItem->GetGradeType() );

				if( iValue2 / PRESENT_EXTRAITEM_DIVISION_1 != 0 )
					pBtn->SetExtraItemGrowthCatalyst( true );

				if( sPresentMent == PRESENT_TRADE_CANCEL_MENT || sPresentMent == PRESENT_TRADE_TIMEOUT_MENT )
				{
					pBtn->SetMarkType( INVENTORY_MARK_TYPE_TRADE_ENABLE );
					pBtn->SetActive();
				}
				else
				{
					pBtn->SetMarkType( INVENTORY_MARK_TYPE_NONE );
					pBtn->SetActive();
				}
			}
		}
	}
}

void InventoryItemWnd::SetPresentExtraItemBox( int iValue1, int iValue2 )
{
	// icon btn
	RandomMachineInfo *pInfo = g_ExtraItemInfoMgr.GetRandomMachineInfo( iValue1 );
	if( pInfo )
	{
		ioHashString szIcon = pInfo->GetIconName();
		if( !szIcon.IsEmpty() )
		{
			InventoryIconBtn *pBtn = dynamic_cast< InventoryIconBtn* > ( FindChildWnd( ID_ICON_BTN ) );
			if( pBtn )
			{
				pBtn->SetIcon( g_UIImageSetMgr.CreateImageByFullName( szIcon ), FLOAT1, FLOAT1 );
				pBtn->SetGradeType( pInfo->GetGradeType() );
				pBtn->SetMarkType( INVENTORY_MARK_TYPE_NONE );
				pBtn->SetActive();
			}
		}
	}
}

void InventoryItemWnd::SetPresentGradeExp( int iValue1, int iValue2 )
{
	// icon btn
	ioUIRenderImage *pImg = g_UIImageSetMgr.CreateImageByFullName( ioHashString( "UIIconPack13#quest_001" ) );
	InventoryIconBtn *pBtn = dynamic_cast< InventoryIconBtn* > ( FindChildWnd( ID_ICON_BTN ) );
	if( pBtn )
	{
		pBtn->SetIcon( pImg, FLOAT1, FLOAT1 );
		pBtn->SetMarkType( INVENTORY_MARK_TYPE_NONE );
		pBtn->SetActive();
	}
}

void InventoryItemWnd::SetPresentMedalItem( int iValue1, int iValue2 )
{
	// icon btn
	const ioMedalItemInfoManager::ItemInfo *pItemInfo = g_MedalItemMgr.GetItemInfo( iValue1 );
	if( pItemInfo )
	{
		ioHashString szIcon = pItemInfo->m_sIcon;
		if( !szIcon.IsEmpty() )
		{
			InventoryIconBtn *pBtn = dynamic_cast< InventoryIconBtn* > ( FindChildWnd( ID_ICON_BTN ) );
			if( pBtn )
			{
				pBtn->SetIcon( g_UIImageSetMgr.CreateImageByFullName( szIcon ), FLOAT1, FLOAT1 );
				pBtn->SetMarkType( INVENTORY_MARK_TYPE_NONE );
				pBtn->SetActive();
			}
		}
	}
}

void InventoryItemWnd::SetPresentAlchemicItem( int iValue1, int iValue2 )
{
	// icon btn
	ioHashString szIcon = g_AlchemicMgr.GetAlchemicItemIcon( iValue1 );
	if( !szIcon.IsEmpty() )
	{
		InventoryIconBtn *pBtn = dynamic_cast< InventoryIconBtn* > (FindChildWnd(ID_ICON_BTN));
		if( pBtn )
		{
			pBtn->SetIcon( g_UIImageSetMgr.CreateImageByFullName( szIcon ), FLOAT1, FLOAT1 );
			pBtn->SetMarkType( INVENTORY_MARK_TYPE_NONE );
			pBtn->SetActive();
		}
	}
}

void InventoryItemWnd::SetPresentPet( int iValue1, int iValue2 )
{
	// icon btn
	int nPetRank = iValue2%10000;
	ioHashString szIcon = g_PetInfoMgr.GetPetIcon( iValue1, (PetRankType)nPetRank );
	if( !szIcon.IsEmpty() )
	{
		InventoryIconBtn *pBtn = dynamic_cast< InventoryIconBtn* > (FindChildWnd(ID_ICON_BTN));
		if( pBtn )
		{
			pBtn->SetIcon( g_UIImageSetMgr.CreateImageByFullName( szIcon ), FLOAT1, FLOAT1 );
			pBtn->SetMarkType( INVENTORY_MARK_TYPE_NONE );
			pBtn->SetActive();
		}
	}
}

// UI코스튬 관련 (선물 설정)
void InventoryItemWnd::SetPresentCostume( int iValue1, int iValue2 )
{
	ioHashString szIcon = g_CostumeInfoMgr.GetCostumeIcon( iValue1 );
	if( !szIcon.IsEmpty() )
	{
		InventoryIconBtn *pBtn = dynamic_cast< InventoryIconBtn* > (FindChildWnd(ID_ICON_BTN));
		if( pBtn )
		{
			pBtn->SetIcon( g_UIImageSetMgr.CreateImageByFullName( szIcon ), 1.0f, 1.0f );
			pBtn->SetGradeType( g_CostumeInfoMgr.GetGradeType( iValue1 ) );
			pBtn->SetCostume( true );
			pBtn->SetMarkType( INVENTORY_MARK_TYPE_NONE );
			pBtn->SetActive();
		}
	}
}

void InventoryItemWnd::SetPresentCostumeBox( int iValue1, int iValue2 )
{
	// icon btn
	CostumeShopInfo *pInfo = g_CostumeShopInfoMgr.GetCostumeMachineInfo( iValue1 );
	if( pInfo )
	{
		ioHashString szIcon = pInfo->GetIconName();
		if( !szIcon.IsEmpty() )
		{
			InventoryIconBtn *pBtn = dynamic_cast< InventoryIconBtn* > ( FindChildWnd( ID_ICON_BTN ) );
			if( pBtn )
			{
				pBtn->SetIcon( g_UIImageSetMgr.CreateImageByFullName( szIcon ), 1.0f, 1.0f );
				pBtn->SetGradeType( pInfo->GetGradeType() );
				pBtn->SetMarkType( INVENTORY_MARK_TYPE_NONE );
				pBtn->SetActive();
			}
		}
	}
}

void InventoryItemWnd::SetPresentAccessory( int iValue1, int iValue2 )
{
	const ioItem *pItem = g_ItemMaker.GetItemConst( iValue1, __FUNCTION__ );
	if( pItem )
	{
		ioHashString szIcon = pItem->GetItemLargeIconName();
		if( !szIcon.IsEmpty() )
		{
			InventoryIconBtn *pBtn = dynamic_cast< InventoryIconBtn* > ( FindChildWnd( ID_ICON_BTN ) );
			if( pBtn )
			{
				pBtn->SetIcon( g_UIImageSetMgr.CreateImageByFullName( szIcon ), 1.0f, 1.0f );
				pBtn->SetGradeType( pItem->GetGradeType() );

				
				pBtn->SetMarkType( INVENTORY_MARK_TYPE_NONE );
				pBtn->SetActive();
			}
		}
	}
}

void InventoryItemWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() )
		return;

	ProcessDesc();

//	ProcessToolTip();
}


void InventoryItemWnd::ProcessDesc()
{
	switch( m_iMainType )
	{
	case INVENTORY_ITEM_EXTRAITEM:
		ProcessExtraItemDesc();
		break;
	case INVENTORY_ITEM_ETC:
		ProcessEtcItemDesc();
		break;
	case INVENTORY_ITEM_MEDALITEM:
		ProcessMedalItemDesc();
		break;
		// UI코스튬 관련 (설명 관련)
	case INVENTORY_ITEM_COSTUME:
		ProcessCostumeDesc();
		break;
	case INVENTORY_ITEM_ACCESSORY:
		ProcessAccessoryDesc();
		break;
	}
}

void InventoryItemWnd::ProcessExtraItemDesc()
{
	ioUserExtraItem *pExtraItem = g_MyInfo.GetUserExtraItem();
	if( !pExtraItem )
	{
		LOG.PrintTimeAndLog( 0, "%s UserExtraItem == NULL", __FUNCTION__ );
		return;
	}

	ioHashString sInfo;
	pExtraItem->GetUseInfo( m_iMagicCode, sInfo );

	EXTRAITEMSLOT kSlot;
	if( !pExtraItem->GetExtraItem( m_iMagicCode, kSlot ) )
	{
		LOG.PrintTimeAndLog( 0, "%s don't have , Use Info:%d", __FUNCTION__, m_iMagicCode );
		return;
	}

	if( sInfo.IsEmpty() )
	{
		LOG.PrintTimeAndLog( 0, "%s don't have , Use Info:%d", __FUNCTION__, m_iMagicCode );
		return;
	}

	m_szDescArray[1].ClearList();
	m_szDescArray[1].SetTextStyle( TS_NORMAL );
	m_szDescArray[1].SetBkColor( 0, 0, 0 );	
	if( kSlot.m_PeriodType == EPT_MORTMAIN )
		m_szDescArray[1].SetTextColor( TCT_DEFAULT_BLUE );
	else
		m_szDescArray[1].SetTextColor( TCT_DEFAULT_RED );
	m_szDescArray[1].AddTextPiece( FONT_SIZE_13, sInfo.c_str() );
}

void InventoryItemWnd::ProcessMedalItemDesc()
{
	ioUserMedalItem *pMedalItem = g_MyInfo.GetUserMedalItem();
	if( !pMedalItem )
	{
		LOG.PrintTimeAndLog( 0, "%s UserMedalItem == NULL", __FUNCTION__ );
		return;
	}

	ioUserMedalItem::MEDALITEMSLOT kSlot;
	if( !pMedalItem->GetMedalItem( m_iMagicCode, kSlot ) )
	{
		LOG.PrintTimeAndLog( 0, "InventoryItemWnd::ProcessMedalItemDesc() Not Exist Medal Item : %d", m_iMagicCode );
		return;
	}

	ioHashString sInfo;
	pMedalItem->GetUseInfo( m_iMagicCode, sInfo );
	
	if( sInfo.IsEmpty() )
	{
		LOG.PrintTimeAndLog( 0, "%s don't have , Use Info:%d", __FUNCTION__, m_iMagicCode );
		return;
	}

	if( kSlot.m_iPeriodType == ioUserMedalItem::PT_MORTMAIN )
	{
		m_szDescArray[1].ClearList();
		m_szDescArray[1].SetTextStyle( TS_NORMAL );
		m_szDescArray[1].SetBkColor( 0, 0, 0 );	
		m_szDescArray[1].SetTextColor( TCT_DEFAULT_BLUE );
		m_szDescArray[1].AddTextPiece( FONT_SIZE_13, STR(1), sInfo.c_str() );
	}
	else
	{
		m_szDescArray[1].ClearList();
		m_szDescArray[1].SetTextStyle( TS_NORMAL );
		m_szDescArray[1].SetBkColor( 0, 0, 0 );	
		m_szDescArray[1].SetTextColor( TCT_DEFAULT_RED );
		m_szDescArray[1].AddTextPiece( FONT_SIZE_13, STR(2), sInfo.c_str() );
	}
}

void InventoryItemWnd::ProcessEtcItemDesc()
{
	ioUserEtcItem *pUserEtcItem =  g_MyInfo.GetUserEtcItem();
	if( !pUserEtcItem )
	{
		LOG.PrintTimeAndLog( 0, "%s UserEtcItem == NULL , Type:%d", __FUNCTION__, m_iMagicCode );
		return;
	}

	DWORD sInfo1Color;
	DWORD sInfo2Color;
	ioHashString sInfo1;
	ioHashString sInfo2;
	pUserEtcItem->GetUseInfo( m_iMagicCode, sInfo1, sInfo2, sInfo1Color, sInfo2Color );

	if( !sInfo1.IsEmpty() )
	{
		m_szDescArray[1].ClearList();
		m_szDescArray[1].SetTextStyle( TS_NORMAL );
		m_szDescArray[1].SetBkColor( 0, 0, 0 );	
		m_szDescArray[1].SetTextColor( sInfo1Color );
		m_szDescArray[1].AddTextPiece( FONT_SIZE_11, sInfo1.c_str() );
	}

	if( !sInfo2.IsEmpty() )
	{
		m_szDescArray[2].ClearList();
		m_szDescArray[2].SetTextStyle( TS_NORMAL );
		m_szDescArray[2].SetBkColor( 0, 0, 0 );	
		m_szDescArray[2].SetTextColor( sInfo2Color );
		m_szDescArray[2].AddTextPiece( FONT_SIZE_11, sInfo2.c_str() );
	}
}

void InventoryItemWnd::ProcessCostumeDesc()
{
	ioUserCostume *pCostume = g_MyInfo.GetUserCostume();
	if( !pCostume )
	{
		LOG.PrintTimeAndLog( 0, "%s UserCostume == NULL", __FUNCTION__ );
		return;
	}

	ioHashString sInfo;
	pCostume->GetUseInfo( m_iMagicCode, sInfo );

	CostumeSlot kSlot;
	if( !pCostume->GetCostumeSlot( m_iMagicCode, kSlot ) )
	{
		LOG.PrintTimeAndLog( 0, "%s don't have , Use Info:%d", __FUNCTION__, m_iMagicCode );
		return;
	}

	if( sInfo.IsEmpty() )
	{
		LOG.PrintTimeAndLog( 0, "%s don't have , Use Info:%d", __FUNCTION__, m_iMagicCode );
		return;
	}

	m_szDescArray[1].ClearList();
	m_szDescArray[1].SetTextStyle( TS_NORMAL );
	m_szDescArray[1].SetBkColor( 0, 0, 0 );	

	if( kSlot.m_PeriodType == PCPT_MORTMAIN )
		m_szDescArray[1].SetTextColor( TCT_DEFAULT_BLUE );
	else
		m_szDescArray[1].SetTextColor( TCT_DEFAULT_RED );
	m_szDescArray[1].AddTextPiece( FONT_SIZE_13, sInfo.c_str() );
}

void InventoryItemWnd::ProcessToolTip()
{
	if( m_bOver && !m_bClicked )
	{
		ioLSMouse *pMouse = g_App.GetMouse();
		if( pMouse )
		{
			pMouse->SetToolTipWndID( m_dwID );
			return;
		}
	}	

	for(int i = ID_ICON_BTN; i < MAX_BTN ; i++)
	{
		ioButton *pButton = dynamic_cast<ioButton*> ( FindChildWnd( i ) );
		if( pButton && pButton->IsOvered() )
		{
			ioLSMouse *pMouse = g_App.GetMouse();
			if( pMouse )
			{
				pMouse->SetToolTipWndID( m_dwID );
				return;
			}
		}
	}
}

bool InventoryItemWnd::QuestGuide( const IntVec &rValue, float &rfXPos, float &rfYPos )
{
	if( rValue.size() < 8 ) return false;

	// MainTabID.SubTabID.Btn(Type.MainCode.SubCode1.SubCode2.SubCode3.SubCode4)
	int iMainCode = rValue[3];

	ioWnd *pGuideBtn = NULL;
	switch( m_iMainType )
	{
	case INVENTORY_ITEM_EXTRAITEM:
		{
			if( m_iMagicCode <= 0 )
				return false;

			EXTRAITEMSLOT kSlot;
			ioUserExtraItem *pExtraItem = g_MyInfo.GetUserExtraItem();
			if( !pExtraItem )
				return false;

			if( !pExtraItem->GetExtraItem( m_iMagicCode, kSlot ) )
				return false;
			
			if( kSlot.m_iItemCode != iMainCode )
				return false;
            
			pGuideBtn = FindChildWnd( ID_ICON_BTN );
		}
		break;
	case INVENTORY_ITEM_DECO:
		{
			if( iMainCode != g_MyInfo.GetClassType() )
				return false;
			
			if( m_ItemInfo.m_iSexType != rValue[4] )
				return false;

			if( m_ItemInfo.m_iDecoType != rValue[5] )
				return false;

			if( m_ItemInfo.m_iDecoCode != rValue[6] )
				return false;

			pGuideBtn = FindChildWnd( ID_ICON_BTN );
		}
		break;
	case INVENTORY_ITEM_ETC:
		{
			if( m_iMagicCode != iMainCode )
				return false;
            
			pGuideBtn = FindChildWnd( ID_ICON_BTN );
		}
		break;
	case INVENTORY_ITEM_PRESENT:
		{
			if( m_iMagicCode <= 0 )
				return false;

			ioHashString szSendID;
			short sPresentType, sPresentMent;
			int iPresentValue1, iPresentValue2, iPresentValue3, iPresentValue4;
			DWORD dwLimitDate;
			g_PresentMgr.GetPresentDataToIndex( m_iMagicCode, m_iMagicCodeEx, szSendID, sPresentType, sPresentMent, iPresentValue1, iPresentValue2, iPresentValue3, iPresentValue4, dwLimitDate );
			if( (int)sPresentType != iMainCode )
				return false;

			if( iPresentValue1 != rValue[4] )
				return false;

			if( iPresentValue2 != rValue[5] )
				return false;

			pGuideBtn = FindChildWnd( ID_ICON_BTN );
		}
		break;
	case INVENTORY_ITEM_MEDALITEM:
		{
			if( m_iMagicCode == iMainCode )
				pGuideBtn = FindChildWnd( ID_ICON_BTN );
		}
		break;
	case INVENTORY_ITEM_COSTUME:
		{
			// UI코스튬 관련 (퀘스트 가이드)
			if( m_iMagicCode <= 0 )
				return false;

			CostumeSlot kSlot;
			ioUserCostume *pCostume = g_MyInfo.GetUserCostume();
			if( !pCostume )
				return false;

			if( !pCostume->GetCostumeSlot( m_iMagicCode, kSlot ) )
				return false;

			if( kSlot.m_nIndex != iMainCode )
				return false;

			pGuideBtn = FindChildWnd( ID_ICON_BTN );
		}
		break;
	}

	if( pGuideBtn && pGuideBtn->IsShow() )
	{
		rfXPos = pGuideBtn->GetDerivedPosX() + (pGuideBtn->GetWidth() / 2);
		rfYPos = pGuideBtn->GetDerivedPosY();
		return true;
	}

	return false;
}

void InventoryItemWnd::iwm_pulldown_event( const PullDownEvent& Event )
{
	//마우스 오버
	if( CLASSTYPEQUALS( OnOverPullDownEvent ) == CLASSTYPEQUALS( Event ) )
	{
		const OnOverPullDownEvent& OverEvent = (const OnOverPullDownEvent&)( Event );
		if( OverEvent.IsImageMouseOver() )
			g_PowerUpManager.OnPowerUpMenuToolTipShow( OverEvent );
	}
	//마우스 클릭
	else if( CLASSTYPEQUALS( ClickPullDownEvent ) == CLASSTYPEQUALS( Event ) )
	{
		const ClickPullDownEvent& ClickEvent = (const ClickPullDownEvent&)( Event );
		if( ClickEvent.IsPulldownClick() )
		{
			g_PowerUpManager.OnPowerUpMenuClick( ClickEvent, m_iMagicCode, this );
		}
		else if( ClickEvent.IsImageClick() )
		{
			g_PowerUpManager.OnPowerUpImageClick( ClickEvent);
		}
	}
	//마우스 리브
	else if( CLASSTYPEQUALS( OnLeavePullDownEvent ) == CLASSTYPEQUALS( Event ) )
	{
		const OnLeavePullDownEvent& LeaveEvent = (const OnLeavePullDownEvent&)( Event );
		if( LeaveEvent.IsImageMouseLeave() )
			g_PowerUpManager.OnPowerUpMenuToolTipHide( LeaveEvent );
	}
}

void InventoryItemWnd::SetPresentBonusCash( int iValue1, int iValue2 )
{
	// icon btn
	ioUIRenderImage *pImg = g_UIImageSetMgr.CreateImageByFullName( ioHashString( "UIIconPack101#Cash" ) );
	InventoryIconBtn *pBtn = dynamic_cast< InventoryIconBtn* > ( FindChildWnd( ID_ICON_BTN ) );
	if( pBtn )
	{
		pBtn->SetIcon( pImg, 1.0f, 1.0f );
		pBtn->SetMarkType( INVENTORY_MARK_TYPE_NONE );
		pBtn->SetActive();
	}
}

bool InventoryItemWnd::SetAccessoryInfo()
{
	if( m_iMagicCode <= 0 )
		return false;

	AccessorySlot kSlot;
	ioUserAccessory *pAccessory = g_MyInfo.GetUserAccessory();
	if( !pAccessory )
		return false;

	if( !pAccessory->GetAccessorySlot( m_iMagicCode, kSlot ) )
		return false;

	// icon btn
	ShowChildWnd( ID_ICON_BTN );

	if( m_bEquip )
	{
		ShowChildWnd( ID_RELEASE_BTN );

		m_dwCurIconActionID = INVENTORY_ACTION_RELEASE;
	}
	else
	{
		ShowChildWnd( ID_RESELL_MEDAL_BTN );

		m_dwCurIconActionID = INVENTORY_ACTION_EQUIP;
	}

	if( kSlot.m_dwMaleCustom > 0 || kSlot.m_dwFemaleCustom > 0 )
	{
		ShowChildWnd( ID_CUSTOM_INFO_BTN );
	}

	InventoryIconBtn *pBtn = dynamic_cast< InventoryIconBtn* > ( FindChildWnd( ID_ICON_BTN ) );
	if( !pBtn )
	{
		LOG.PrintTimeAndLog( 0, "%s pBtn == NULL.", __FUNCTION__ );
	}
	else
	{
		ioUIRenderImage *pImg = NULL;
		pImg = g_UIImageSetMgr.CreateImageByFullName( kSlot.m_IconName );
		pBtn->SetIcon( pImg, 1.0f, 1.0f, 0 );

		if( m_bEquip )
		{
			pBtn->SetMarkType( INVENTORY_MARK_TYPE_EQUIP );
			pBtn->SetInActive();
		}
		else
		{
			pBtn->SetMarkType( INVENTORY_MARK_TYPE_NONE );
			pBtn->SetActive();
		}
	}

	// desc
	ClearDesc();
	SetIsWidthCutDesc( 0, true );
	SetIsWidthCutDesc( 2, true );

	// Name
	m_szDescArray[0].SetTextStyle( TS_NORMAL );
	m_szDescArray[0].SetBkColor( 0, 0, 0 );	
	m_szDescArray[0].SetTextColor( TCT_DEFAULT_DARKGRAY ); // 진회
	m_szDescArray[0].AddTextPiece( FONT_SIZE_13, STR(1), kSlot.m_Name.c_str() );

	// Limit
	/*
	AccessoryInfo sAccessoryInfo;
	g_AccessoryInfoMgr.GetAccessoryInfo( kSlot.m_iAccessoryCode, sAccessoryInfo );

	m_szDescArray[2].SetTextStyle( TS_NORMAL );
	m_szDescArray[2].SetBkColor( 0, 0, 0 );	
	m_szDescArray[2].SetTextColor( TCT_DEFAULT_GRAY );

	if( sAccessoryInfo.m_nEnableClass > 0 )
		m_szDescArray[2].AddTextPiece( FONT_SIZE_11, STR(2), g_MyInfo.GetClassName( sAccessoryInfo.m_nEnableClass ) );
	else
		m_szDescArray[2].AddTextPiece( FONT_SIZE_11, STR(3) );


	*/

	return true;
}

void InventoryItemWnd::ProcessAccessoryDesc()
{
	ioUserAccessory *pAccessory = g_MyInfo.GetUserAccessory();
	if( !pAccessory )
	{
		LOG.PrintTimeAndLog( 0, "%s UserAccessoy == NULL", __FUNCTION__ );
		return;
	}

	ioHashString sInfo;
	pAccessory->GetUseInfo( m_iMagicCode, sInfo );

	AccessorySlot kSlot;
	if( !pAccessory->GetAccessorySlot( m_iMagicCode, kSlot ) )
	{
		LOG.PrintTimeAndLog( 0, "%s don't have , Use Info:%d", __FUNCTION__, m_iMagicCode );
		return;
	}

	if( sInfo.IsEmpty() )
	{
		LOG.PrintTimeAndLog( 0, "%s don't have , Use Info:%d", __FUNCTION__, m_iMagicCode );
		return;
	}

	m_szDescArray[1].ClearList();
	m_szDescArray[1].SetTextStyle( TS_NORMAL );
	m_szDescArray[1].SetBkColor( 0, 0, 0 );	

	if( kSlot.m_PeriodType == ACCPT_MORTMAIN )
		m_szDescArray[1].SetTextColor( TCT_DEFAULT_BLUE );
	else
		m_szDescArray[1].SetTextColor( TCT_DEFAULT_RED );
	m_szDescArray[1].AddTextPiece( FONT_SIZE_13, sInfo.c_str() );
}

//////////////////////////////////////////////////////////////////////////

GrowthUpBtn::GrowthUpBtn()
{
	m_pArrowImg = NULL;
	m_pArrowRedImg = NULL;

	m_pRedFrm = NULL;
	m_pPushRedFrm = NULL;

	m_UpType = GUT_ENABLE;

	m_bEffect = false;
	m_dwCheckTime = 0;
}

GrowthUpBtn::~GrowthUpBtn()
{
	SAFEDELETE( m_pArrowImg );
	SAFEDELETE( m_pArrowRedImg );

	SAFEDELETE( m_pRedFrm );
	SAFEDELETE( m_pPushRedFrm );
}

void GrowthUpBtn::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "ArrowImg" )
	{
		SAFEDELETE( m_pArrowImg );
		m_pArrowImg = pImage;
	}
	else if( szType == "ArrowRedImg" )
	{
		SAFEDELETE( m_pArrowRedImg );
		m_pArrowRedImg = pImage;
	}
	else
	{
		ioButton::AddRenderImage( szType, pImage );
	}	
}

void GrowthUpBtn::AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame )
{
	if( pFrame )
	{
		if( szType == "RedFrm" )
		{
			SAFEDELETE( m_pRedFrm );
			m_pRedFrm = pFrame;
		}
		else if( szType == "PushRedFrm" )
		{
			SAFEDELETE( m_pPushRedFrm );
			m_pPushRedFrm = pFrame;
		}
		else
		{	
			ioButton::AddRenderFrame( szType, pFrame );
		}
	}
}

void GrowthUpBtn::ParseExtraInfo( ioXMLElement &xElement )
{
	ioButton::ParseExtraInfo( xElement );

	m_dwEffectTime = xElement.GetIntAttribute_e( "EffectShow" );
	m_dwEffectHide = xElement.GetIntAttribute_e( "EffectHide" );
}

void GrowthUpBtn::SetUpBtnType( UpType eType )
{
	m_UpType = eType;

	m_bEffect = false;
	m_dwCheckTime = FRAMEGETTIME();
}

void GrowthUpBtn::OnDrawNormal( int iXPos, int iYPos )
{
	DrawUpTypeFrame( iXPos, iYPos, false );
}

void GrowthUpBtn::OnDrawOvered( int iXPos, int iYPos )
{
	bool bScreen = false;
	if( HasExWndStyle( IWS_EX_OVER_SCREEN ) )
		bScreen = true;

	DrawUpTypeFrame( iXPos, iYPos, bScreen );

	OnDrawOveredAdd( iXPos, iYPos );
}

void GrowthUpBtn::OnDrawPushed( int iXPos, int iYPos )
{
	if( HasExWndStyle( IWS_EX_PUSH_SCREEN ) )
	{
		DrawUpTypePushFrame( iXPos, iYPos, true );
	}
	else if( HasExWndStyle( IWS_EX_PUSH_DOUBLE ) )
	{
		DrawUpTypePushFrame( iXPos, iYPos, false );
		DrawUpTypePushFrame( iXPos, iYPos, false );
	}
	else if( HasExWndStyle( IWS_EX_PUSHED_OVER_SCREEN ) )
	{
		DrawUpTypePushFrame( iXPos, iYPos, m_bOver );
	}
	else
	{
		DrawUpTypePushFrame( iXPos, iYPos, false );
	}
}

void GrowthUpBtn::OnDrawDisabled( int iXPos, int iYPos )
{
	DrawUpTypeFrame( iXPos, iYPos, false );
}

void GrowthUpBtn::RenderUpArrow( int iXPos, int iYPos, UIRenderType eType )
{
	switch( m_UpType )
	{
	case GUT_DISABLE:
		if( m_pArrowImg )
			m_pArrowImg->Render( iXPos, iYPos, eType );
		break;
	case GUT_ENABLE:
		if( m_pArrowImg )
			m_pArrowImg->Render( iXPos, iYPos, eType );
		break;
	case GUT_TIME:
		if( m_pArrowRedImg )
			m_pArrowRedImg->Render( iXPos, iYPos, eType );
		break;
	}
}

void GrowthUpBtn::DrawUpTypeFrame( int iXPos, int iYPos, bool bCheckScreen )
{
	switch( m_UpType )
	{
	case GUT_DISABLE:
		if( m_pNormalFrm )
		{
			m_pNormalFrm->Render( iXPos, iYPos, UI_RENDER_GRAY_LIGHT );
			RenderUpArrow( iXPos, iYPos, UI_RENDER_GRAY_LIGHT );
		}
		break;
	case GUT_ENABLE:
		if( m_pNormalFrm )
		{
			m_pNormalFrm->Render( iXPos, iYPos );

			if( bCheckScreen )
				m_pNormalFrm->Render( iXPos, iYPos, UI_RENDER_SCREEN );

			RenderUpArrow( iXPos, iYPos, UI_RENDER_NORMAL );

			if( bCheckScreen )
				RenderUpArrow( iXPos, iYPos, UI_RENDER_SCREEN );
		}
		break;
	case GUT_TIME:
		if( m_pRedFrm )
		{
			m_pRedFrm->Render( iXPos, iYPos );

			if( bCheckScreen )
				m_pRedFrm->Render( iXPos, iYPos, UI_RENDER_SCREEN );
			else if( m_bEffect )
				m_pRedFrm->Render( iXPos, iYPos, UI_RENDER_SCREEN );

			RenderUpArrow( iXPos, iYPos, UI_RENDER_NORMAL );

			if( bCheckScreen )
				RenderUpArrow( iXPos, iYPos, UI_RENDER_SCREEN );
			else if( m_bEffect )
				RenderUpArrow( iXPos, iYPos, UI_RENDER_SCREEN );
		}
		break;
	}
}

void GrowthUpBtn::DrawUpTypePushFrame( int iXPos, int iYPos, bool bCheckScreen )
{
	switch( m_UpType )
	{
	case GUT_DISABLE:
		if( m_pPushFrm )
		{
			m_pPushFrm->Render( iXPos, iYPos, UI_RENDER_GRAY_LIGHT );
			RenderUpArrow( iXPos, iYPos, UI_RENDER_GRAY_LIGHT );
		}
		break;
	case GUT_ENABLE:
		if( m_pPushFrm )
		{
			m_pPushFrm->Render( iXPos, iYPos );

			if( bCheckScreen )
				m_pPushFrm->Render( iXPos, iYPos, UI_RENDER_SCREEN );

			RenderUpArrow( iXPos, iYPos, UI_RENDER_NORMAL );

			if( bCheckScreen )
				RenderUpArrow( iXPos, iYPos, UI_RENDER_SCREEN );
		}
		break;
	case GUT_TIME:
		if( m_pPushRedFrm )
		{
			m_pPushRedFrm->Render( iXPos, iYPos );

			if( bCheckScreen )
				m_pPushRedFrm->Render( iXPos, iYPos, UI_RENDER_SCREEN );

			RenderUpArrow( iXPos, iYPos, UI_RENDER_NORMAL );

			if( bCheckScreen )
				RenderUpArrow( iXPos, iYPos, UI_RENDER_SCREEN );
		}
		break;
	}
}

void GrowthUpBtn::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;

	DWORD dwCurTime = FRAMEGETTIME();
	if( m_bEffect )
	{
		if( m_dwCheckTime+m_dwEffectTime < dwCurTime )
		{
			m_bEffect = false;
			m_dwCheckTime = dwCurTime;
		}
	}
	else
	{
		if( m_dwCheckTime+m_dwEffectHide < dwCurTime )
		{
			m_bEffect = true;
			m_dwCheckTime = dwCurTime;
		}
	}
}

//////////////////////////////////////////////////////////////////////////

GrowthSlotBtn::GrowthSlotBtn()
{
	m_pSmallIcon = NULL;

	ClearData();
}

GrowthSlotBtn::~GrowthSlotBtn()
{
	SAFEDELETE( m_pSmallIcon );
}

void GrowthSlotBtn::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	ioButton::AddRenderImage( szType, pImage );
}

void GrowthSlotBtn::ParseExtraInfo( ioXMLElement &xElement )
{
	ioButton::ParseExtraInfo( xElement );
}

void GrowthSlotBtn::ClearData()
{
	m_iClassType = 0;

	m_iItemSlot = 0;
	m_iGrowthLevel = 0;
	m_iItemReinforce = 0;

	m_iCurLevel = 0;

	m_bSkill = false;
	m_bTimeGrowth = false;

	m_Name.Clear();
	m_GrowthText.ClearList();

	SAFEDELETE( m_pSmallIcon );
}

void GrowthSlotBtn::SetInfo( int iClassType, const ioHashString &szName, bool bSkill, int iSlot )
{
	ClearData();

	if( iClassType <= 0 )
		return;

	bool bHired = false;
	int iCharArray = g_MyInfo.GetClassArray( iClassType );
	if( g_MyInfo.IsCharMortmain( iClassType ) )
		bHired = true;
	else if( g_MyInfo.IsCharExerciseStyle( iCharArray, EXERCISE_GENERAL ) )
		bHired = true;
	else if( g_MyInfo.IsCharExerciseStyle( iCharArray, EXERCISE_PCROOM ) )
		bHired = true;
	else if( g_MyInfo.IsCharExerciseStyle( iCharArray, EXERCISE_EVENT ) )
		bHired = true;
	else if( g_MyInfo.IsCharExerciseStyle( iCharArray, EXERCISE_RENTAL ) )
		bHired = true;
	else
	{
		if( g_MyInfo.GetCharTotalSecond( iCharArray ) > 0 )
			bHired = true;
	}

	GrowthUpBtn *pUpBtn = dynamic_cast<GrowthUpBtn*>(FindChildWnd(ID_GROWTH_BTN));
	if( pUpBtn )
	{
		if( !bHired )
			pUpBtn->SetUpBtnType( GrowthUpBtn::GUT_DISABLE );
		else
			pUpBtn->SetUpBtnType( GrowthUpBtn::GUT_ENABLE );
	}

	//
	CHARACTER kCharInfo;
	if( COMPARE( iCharArray, 0, g_MyInfo.GetCharCount() ) )
		kCharInfo = g_MyInfo.GetCharacter( iCharArray );
	else
		NewShopBuyWnd::GetDefaultCharInfo( kCharInfo, iClassType );


	// Small Icon
	ioUserExtraItem *pExtraItem = g_MyInfo.GetUserExtraItem();
	if( bSkill && pExtraItem )
	{
		SAFEDELETE( m_pSmallIcon );

		EXTRAITEMSLOT kSlot;
		int iExtraItemIndex = kCharInfo.m_extra_item[iSlot];
		if( pExtraItem->GetExtraItem( iExtraItemIndex, kSlot ) )
		{
			m_pSmallIcon = g_UIImageSetMgr.CreateImageByFullName( kSlot.m_IconName );
		}
	}

	ioGrowthLevel *pLevel = g_MyInfo.GetGrowthLevel();
	if( pLevel )
	{
		m_Name = szName;
		m_bSkill = bSkill;
		m_iItemSlot = iSlot;
		m_iClassType = iClassType;

		if( m_bSkill )
			m_iGrowthLevel = pLevel->GetItemGrowthLevel( m_iClassType, m_iItemSlot, false );
		else
			m_iGrowthLevel = pLevel->GetCharGrowthLevel( m_iClassType, m_iItemSlot, false );

		// TimeGrowth
		TimeGrowthInfo *pInfo = pLevel->FindTimeGrowthInfo( m_iClassType );
		if( pInfo )
		{
			int iSlotIndex = m_iItemSlot+1;
			if( !m_bSkill )
				iSlotIndex += CUR_GROWTH_CNT;

			int iTimeGrowthSlot = pInfo->m_iTimeSlot;
			if( iTimeGrowthSlot == iSlotIndex )
			{
				m_bTimeGrowth = true;
			}
		}

		int iNextLevel = m_iGrowthLevel+1;
		int iCurMax = g_MyInfo.GetClassLevel( m_iClassType, true );
		iCurMax = min( iCurMax, g_GrowthInfoMgr.GetMaxLevel() );

		int iCurPoint = pLevel->GetCharGrowthPoint( m_iClassType );

		if( g_MyInfo.IsCharExerciseStyle( iCharArray, EXERCISE_RENTAL ) )
			iCurPoint = 0;

		if( pUpBtn )
		{
			int iNeedPoint = g_GrowthInfoMgr.GetGrowthUpNeedPoint( !m_bSkill );

			if( m_bTimeGrowth )
			{
				pUpBtn->SetUpBtnType( GrowthUpBtn::GUT_TIME );
			}
			else if( iCurPoint <= 0 || iCurPoint < iNeedPoint )
			{
				pUpBtn->SetUpBtnType( GrowthUpBtn::GUT_DISABLE );
			}
//#ifdef  BALANCE_RENEWAL
//			else if( iNextLevel > iCurMax )
//			{
//				if( g_GrowthInfoMgr.GetUseNextLvInfo() )
//					pUpBtn->SetUpBtnType( GrowthUpBtn::GUT_DISABLE );
//				else
//					pUpBtn->SetUpBtnType( GrowthUpBtn::GUT_ENABLE );
//			}
//#else
			else if( iNextLevel > iCurMax )
			{
				pUpBtn->SetUpBtnType( GrowthUpBtn::GUT_DISABLE );
			}
//#endif

		}

		int iCharArray = g_MyInfo.GetClassArray( m_iClassType );
		const ITEM_DATA &kItem = g_MyInfo.GetCharItem( iCharArray, m_iItemSlot );
		m_iItemReinforce = kItem.m_item_reinforce;

		if ( m_iItemReinforce <= g_CompoundMaterialMgr.GetFixedFailReinforce() )
			m_iCurLevel = m_iGrowthLevel + m_iItemReinforce;
		else
			m_iCurLevel = m_iGrowthLevel + g_GrowthInfoMgr.GetReinforceAddStat( m_iItemReinforce );

		// medal
		IntVec vMedalItemTypeVec;
		g_MyInfo.GetMedalItemTypeVec( vMedalItemTypeVec, m_iClassType );

		int iMedalSize = vMedalItemTypeVec.size();
		for (int i = 0; i < iMedalSize ; i++)
			m_iCurLevel += g_MedalItemMgr.GetMedalItemGrowth( vMedalItemTypeVec[i], !m_bSkill, m_iItemSlot );	

		//펫 성장
		PetSlot sPetSlot;
		ioUserPet *pUserPet = g_MyInfo.GetUserPet();
		if( pUserPet )
			pUserPet->GetPetSlotEquip( sPetSlot );

		if( sPetSlot.m_nPetCode > 0 )
		{
			if( m_bSkill )
				m_iCurLevel += g_PetInfoMgr.GetTotalStat( sPetSlot.m_nPetCode, m_iItemSlot + MAX_ITEM_GROWTH, sPetSlot.m_nCurLevel );
			else
				m_iCurLevel += g_PetInfoMgr.GetTotalStat( sPetSlot.m_nPetCode, m_iItemSlot, sPetSlot.m_nCurLevel );
		}

		//각성
		if( m_bSkill )
			m_iCurLevel += g_AwakeMgr.GetAwakeCharGrowth( static_cast<AwakeType>( g_MyInfo.GetAwakeType( m_iClassType ) ), m_iItemSlot );
		else
			m_iCurLevel += g_AwakeMgr.GetAwakeItemGrowth( static_cast<AwakeType>( g_MyInfo.GetAwakeType( m_iClassType ) ), m_iItemSlot );

		const CHARACTER& rkCharInfo = g_MyInfo.GetCharacterToClassType( m_iClassType );
		if( g_RSoldierInfoManager.IsRSoldierType( rkCharInfo ) )
		{
			if( m_bSkill )
				m_iCurLevel += g_RSoldierInfoManager.GetSoldierAddStat( m_iItemSlot + MAX_ITEM_GROWTH, g_MyInfo.GetRSoldierPossessionCount() );
			else
				m_iCurLevel += g_RSoldierInfoManager.GetSoldierAddStat( m_iItemSlot, g_MyInfo.GetRSoldierPossessionCount() );
		}

		if( g_RSoldierInfoManager.IsSoldierSet( rkCharInfo ) )
		{
			if( m_bSkill )
				m_iCurLevel += g_RSoldierInfoManager.GetSoldierSetAddStat( m_iItemSlot + MAX_ITEM_GROWTH, rkCharInfo.m_class_type, g_MyInfo.GetSoldierSetCnt( g_RSoldierInfoManager.GetSoldierSetNum( rkCharInfo.m_class_type ) ) );
			else
				m_iCurLevel += g_RSoldierInfoManager.GetSoldierSetAddStat( m_iItemSlot, rkCharInfo.m_class_type, g_MyInfo.GetSoldierSetCnt( g_RSoldierInfoManager.GetSoldierSetNum( rkCharInfo.m_class_type ) ) );
		}
	}
}

//void GrowthSlotBtn::OnProcess( float fTimePerSec )
//{
//	if( !IsShow() )
//		return;
//}

void GrowthSlotBtn::OnRender()
{
	if( HasWndStyle( IWS_INACTIVE ) )
		return;

	ioButton::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	// Small Icon
	enum
	{
		ICON_X_OFFSET	= 17,
		ICON_Y_OFFSET	= 165,
	};

	if( m_pSmallIcon )
	{
		m_pSmallIcon->SetScale( 0.24f );
		m_pSmallIcon->Render( iXPos+FLOAT10, iYPos+FLOAT10, UI_RENDER_NORMAL, TFO_BILINEAR );
	}

	// Text
	m_GrowthText.ClearList();
	m_GrowthText.SetBkColor( 0, 0, 0 );
	m_GrowthText.SetTextStyle( TS_NORMAL );

	if( m_iCurLevel > 0 )
	{
		if( m_bOver )
			m_GrowthText.SetTextColor( TCT_DEFAULT_WHITE );
		else
			m_GrowthText.SetTextColor( TCT_DEFAULT_DARKGRAY );

		m_GrowthText.AddTextPiece( FONT_SIZE_12, STR(1), m_Name.c_str() );

		if( m_bOver )
			m_GrowthText.SetTextColor( TCT_DEFAULT_WHITE );
		else
			m_GrowthText.SetTextColor( TCT_DEFAULT_GREEN );

		m_GrowthText.AddTextPiece( FONT_SIZE_12, STR(2), m_iCurLevel );
	}
	else
	{
		if( m_bOver )
			m_GrowthText.SetTextColor( TCT_DEFAULT_WHITE );
		else
			m_GrowthText.SetTextColor( TCT_DEFAULT_LIGHTGRAY );

		m_GrowthText.AddTextPiece( FONT_SIZE_12, STR(3), m_Name.c_str() );
	}

	if( !m_GrowthText.IsEmpty() )
	{
		if( m_pSmallIcon )
			m_GrowthText.PrintFullTextWidthCut( iXPos+20, iYPos+3, TAT_LEFT, 95.0f );
		else
			m_GrowthText.PrintFullTextWidthCut( iXPos+3, iYPos+3, TAT_LEFT, 95.0f );
	}
}

void GrowthSlotBtn::OnRenderAfterChild()
{
	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();
}

void GrowthSlotBtn::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();

	switch( dwID )
	{
	case ID_GROWTH_BTN:
#ifdef _LSWC
	case ID_AGING_BTN:
#endif
		if( cmd == IOBN_BTNUP )
		{
			if( GetParent() )
				GetParent()->iwm_command( this, cmd, dwID );
		}
		else if( cmd == IOWN_OVERED )
		{
			m_bOver = true;

			if( GetParent() )
				GetParent()->iwm_command( this, cmd, dwID );
		}
		else if( cmd == IOWN_LEAVED )
		{
			m_bOver = false;

			if( GetParent() )
				GetParent()->iwm_command( this, cmd, dwID );
		}
		break;
	}
}

//////////////////////////////////////////////////////////////////////////

InventorySoldierItemWnd::InventorySoldierItemWnd()
{
	m_iClassType = 0;
	m_iCurPoint = 0;
	m_dwCurWndID = 0;

	m_pCurGrowthBack = NULL;
	m_pPointImg = NULL;

	m_pBottomFrm = NULL;
	m_pSelectFrm = NULL;
	m_bSelect    = false;

	m_pEmptyIcon = NULL;
	m_pBoxLeft = NULL;
	m_pBoxTop = NULL;
	m_pDarkStrip = NULL;
	m_pLightStrip = NULL;

	m_ClassInfo[0].ClearList();
	m_ClassInfo[1].ClearList();

	m_PointText.ClearList();

	m_CharGrowthText.clear();
	m_CharGrowthText.reserve(CUR_GROWTH_CNT);

	m_ItemGrowthText.clear();
	m_ItemGrowthText.reserve(CUR_GROWTH_CNT);

	ClearGrowthInfo();
	ClearSkillText();

	m_bHired = false;
}

InventorySoldierItemWnd::~InventorySoldierItemWnd()
{
	SAFEDELETE(m_pCurGrowthBack);
	SAFEDELETE(m_pPointImg);

	SAFEDELETE(m_pBottomFrm);
	SAFEDELETE( m_pSelectFrm );

	SAFEDELETE(m_pEmptyIcon);
	SAFEDELETE(m_pBoxLeft);
	SAFEDELETE(m_pBoxTop);
	SAFEDELETE(m_pDarkStrip);
	SAFEDELETE(m_pLightStrip);

	ClearGrowthInfo();
	ClearSkillText();
}

void InventorySoldierItemWnd::ClearGrowthInfo()
{
	int i, j;
	for( i=0; i < CUR_GROWTH_CNT; ++i )
	{
		m_CharLevel[i] = 0;
		m_CharGrowth[i] = 0;
	}

	for( j=0; j < CUR_GROWTH_CNT; ++j )
	{
		m_ItemLevel[j] = 0;
		m_ItemGrowth[j] = 0;
	}
}

void InventorySoldierItemWnd::ClearSkillText()
{
	for( int i=0; i < CUR_GROWTH_CNT; ++i )
	{
		m_ItemSkillText[i].Clear();
	}
}

void InventorySoldierItemWnd::SetInfo( int iClassType )
{
	m_iClassType = iClassType;
	m_bHired = false;

	ClearGrowthInfo();
	ClearSkillText();

	if( m_iClassType <= 0 )
	{
		m_iClassType = 0;
		m_iCurPoint = 0;

		m_ClassInfo[0].ClearList();
		m_ClassInfo[1].ClearList();

		InventoryIconBtn *pWnd = dynamic_cast< InventoryIconBtn* > ( FindChildWnd(ID_ICON_BTN) );
		if( pWnd )
		{
			pWnd->SetIcon( NULL, FLOAT1, FLOAT1 );
			pWnd->SetMarkType( INVENTORY_MARK_TYPE_NONE );
		}

		SetInActive();

		HideChildWnd( ID_ICON_BTN );

		HideChildWnd( ID_SKILL_SLOT1 );
		HideChildWnd( ID_SKILL_SLOT2 );
		HideChildWnd( ID_SKILL_SLOT3 );
		HideChildWnd( ID_SKILL_SLOT4 );

		HideChildWnd( ID_CHAR_SLOT1 );
		HideChildWnd( ID_CHAR_SLOT2 );
		HideChildWnd( ID_CHAR_SLOT3 );
		HideChildWnd( ID_CHAR_SLOT4 );
	}
	else
	{
		// UpdateItemData에서 Active 체크한다. 그래서 먼저...
		SetActive();

		UpdateItemData();

		bool bHired = false;
		bool bTimeGrowth = false;
		InventoryIconBtn *pWnd = dynamic_cast< InventoryIconBtn* > ( FindChildWnd(ID_ICON_BTN) );
		if( pWnd )
		{
			ioUIRenderImage *pImg = g_UIImageSetMgr.CreateImageByFullName(g_MyInfo.GetMySoldierIconName(m_iClassType));
			pWnd->SetIcon( pImg, FLOAT1, FLOAT1 );
			pWnd->SetGradeType( g_UIImageRenderMgr.GetPowerUpGradeType( m_iClassType ) );

			int iCharArray = g_MyInfo.GetClassArray( m_iClassType );
			if( g_MyInfo.IsCharMortmain( m_iClassType ) )
				bHired = true;
			else if( g_MyInfo.IsCharExerciseStyle( iCharArray, EXERCISE_GENERAL ) )
				bHired = true;
			else if( g_MyInfo.IsCharExerciseStyle( iCharArray, EXERCISE_PCROOM ) )
				bHired = true;
			else if( g_MyInfo.IsCharExerciseStyle( iCharArray, EXERCISE_EVENT ) )
				bHired = true;
			else if( g_MyInfo.IsCharExerciseStyle( iCharArray, EXERCISE_RENTAL ) )
				bHired = true;
			else
			{
				if( g_MyInfo.GetCharTotalSecond( iCharArray ) > 0 )
					bHired = true;
			}

			int iCurLevel = 0;
			ioGrowthLevel *pLevel = g_MyInfo.GetGrowthLevel();
			if( pLevel )
			{
				TimeGrowthInfo *pInfo = pLevel->FindTimeGrowthInfo( m_iClassType );
				if( pInfo )
					bTimeGrowth = true;
			}

			pWnd->SetMarkType( INVENTORY_MARK_TYPE_NONE );
			if( bTimeGrowth )
			{
				pWnd->SetMarkType( INVENTORY_MARK_TYPE_TIMEGROWTH );
			}
			else
			{
				if( bHired )
				{
					if( m_iCurPoint > 0 )
						pWnd->SetMarkType( INVENTORY_MARK_TYPE_GROWTH_ENABLE );
					else
						pWnd->SetMarkType( INVENTORY_MARK_TYPE_GROWTH_DISABLE );
				}
			}

			pWnd->SetClassType( m_iClassType );
		}

		ShowChildWnd( ID_ICON_BTN );

		if( bHired )
		{
			m_bHired = true;
			SetChildActive( ID_ICON_BTN );
		}
		else
			SetChildInActive( ID_ICON_BTN );

		ShowChildWnd( ID_SKILL_SLOT1 );
		ShowChildWnd( ID_SKILL_SLOT2 );
		ShowChildWnd( ID_SKILL_SLOT3 );
		ShowChildWnd( ID_SKILL_SLOT4 );

		ShowChildWnd( ID_CHAR_SLOT1 );
		ShowChildWnd( ID_CHAR_SLOT2 );
		ShowChildWnd( ID_CHAR_SLOT3 );
		ShowChildWnd( ID_CHAR_SLOT4 );
	}
}

void InventorySoldierItemWnd::SetSelect( bool bSelect )
{
	m_bSelect = bSelect;
}

void InventorySoldierItemWnd::UpdateItemData()
{
	if( HasWndStyle( IWS_INACTIVE ) )
		return;

	char szBuf[MAX_PATH] = "";

	m_ClassInfo[0].ClearList();
	m_ClassInfo[1].ClearList();

	ioGrowthLevel *pLevel = g_MyInfo.GetGrowthLevel();
	if( !pLevel )
	{
		return;
	}

	bool bHired = false;
	int iCharArray = g_MyInfo.GetClassArray( m_iClassType );
	if( g_MyInfo.IsCharMortmain( m_iClassType ) )
		bHired = true;
	else if( g_MyInfo.IsCharExerciseStyle( iCharArray, EXERCISE_GENERAL ) )
		bHired = true;
	else if( g_MyInfo.IsCharExerciseStyle( iCharArray, EXERCISE_PCROOM ) )
		bHired = true;
	else if( g_MyInfo.IsCharExerciseStyle( iCharArray, EXERCISE_EVENT ) )
		bHired = true;
	else if( g_MyInfo.IsCharExerciseStyle( iCharArray, EXERCISE_EVENT ) )
		bHired = true;
	else if( g_MyInfo.IsCharExerciseStyle( iCharArray, EXERCISE_RENTAL) )
		bHired = true;
	else
	{
		if( g_MyInfo.GetCharTotalSecond( iCharArray ) > 0 )
			bHired = true;
	}
	
	wsprintf_e( szBuf, "Lv%d %s", g_MyInfo.GetClassLevel( m_iClassType, false ), g_MyInfo.GetClassName( m_iClassType ) );
	if( g_FontMgr.GetTextWidth( szBuf, TS_NORMAL, FONT_SIZE_13 ) >= 117 )
	{
		char szTextWidthCut[MAX_PATH];
		Help::StringCutFun( FONT_SIZE_13, 112, TS_NORMAL, szTextWidthCut, MAX_PATH, szBuf );
		sprintf_s( szBuf, "%s...", szTextWidthCut );
	}

	m_ClassInfo[0].SetTextStyle( TS_NORMAL );
	m_ClassInfo[0].SetBkColor( 0, 0, 0 );
	if( bHired )
		m_ClassInfo[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
	else
		m_ClassInfo[0].SetTextColor( TCT_DEFAULT_GRAY );
	m_ClassInfo[0].AddTextPiece( FONT_SIZE_13, STR(1), szBuf );

	ClearSkillText();

	ioGrowthInfo *pInfo = NULL;
	// Skill
	int i;
	int iRealItemCode = 0;
	for( i=0; i < CUR_GROWTH_CNT; ++i )
	{
		// GrowthInfoNum
		int iCharArray = g_MyInfo.GetClassArray( m_iClassType );
		if( iCharArray != -1 )
		{
			const ITEM_DATA &kItem = g_MyInfo.GetCharItem( iCharArray, i );
			iRealItemCode = kItem.m_item_code;
		}
		else
		{
			iRealItemCode = i*DEFAULT_BASIC_ITEM_CODE + m_iClassType;
		}

		int iGrowthInfo = 0;
		const ioItem *pItem = g_ItemMaker.GetItemConst( iRealItemCode, __FUNCTION__ );
		if( pItem )
		{
			ioHashString szSkillName = pItem->GetSkillName();
			const ioSkill *pSkill = g_SkillMaker.GetSkillConst( szSkillName );
			if( pSkill )
			{
				ioHashString szSkillIcon = pSkill->GetIconName();
				m_SkillIconName[i] = szSkillIcon;

				m_ItemSkillText[i] = pSkill->GetDescName();
			}

			iGrowthInfo = pItem->GetGrowthInfoNum( 1 );
			m_ItemGrowth[i] = iGrowthInfo;
		}

		// GrowthLevel
		ioHashString szName;
		int iGrowthType = 0;

		m_ItemLevel[i] = 0;
		pInfo = g_GrowthInfoMgr.GetGrowthInfo( m_ItemGrowth[i] );
		if( pInfo )
		{
			szName = pInfo->GetGrowthInfoName();
			iGrowthType = pInfo->GetGrowthType();
			m_ItemLevel[i] = pLevel->GetItemGrowthLevel( m_iClassType, i, false );
		}

		bool bBaseGrowthType = false;
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
			bBaseGrowthType = true;
			break;
		}

		// Btn
		GrowthSlotBtn *pSlot = dynamic_cast<GrowthSlotBtn*>(FindChildWnd(ID_SKILL_SLOT1+i));
		if( pSlot )
		{
			if( bBaseGrowthType )
				pSlot->SetInfo( m_iClassType, szName, true, i );
			else
				pSlot->SetInfo( m_iClassType, m_ItemGrowthText[i], true, i );
		}
	}

	// Char
	for( i=0; i < CUR_GROWTH_CNT; ++i )
	{
		// GrowthInfoNum
		m_CharGrowth[i] = ioBaseChar::m_vGrowthInfoNum[i];


		// GrowthLevel
		m_CharLevel[i] = 0;
		pInfo = g_GrowthInfoMgr.GetGrowthInfo( m_CharGrowth[i] );
		if( pInfo )
		{
			m_CharLevel[i] = pLevel->GetCharGrowthLevel( m_iClassType, i, false );
		}

		// Btn
		GrowthSlotBtn *pSlot = dynamic_cast<GrowthSlotBtn*>(FindChildWnd(ID_CHAR_SLOT1+i));
		if( pSlot )
		{
			pSlot->SetInfo( m_iClassType, m_CharGrowthText[i], false, i );
		}
	}


	// CurPoint
	m_iCurPoint = pLevel->GetCharGrowthPoint( m_iClassType );
	if( g_MyInfo.IsCharExerciseStyle( iCharArray, EXERCISE_RENTAL ) )
		m_iCurPoint = 0;

	m_PointText.ClearList();
	m_PointText.SetTextStyle( TS_NORMAL );
	m_PointText.SetBkColor( 0, 0, 0 );
	m_PointText.SetTextColor( TCT_DEFAULT_DARKGRAY );
	m_PointText.AddTextPiece( FONT_SIZE_12, STR(2) );

	m_PointText.SetTextColor( TCT_DEFAULT_RED );
	m_PointText.AddTextPiece( FONT_SIZE_12, STR(3), m_iCurPoint );
}

void InventorySoldierItemWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();

	switch( dwID )
	{
	case ID_ICON_BTN:
		if( cmd == IOBN_BTNUP ||
			cmd == IOWN_OVERED||
			cmd == IOWN_LEAVED )
		{
			if( GetParent() )
			{
				if( !HasWndStyle( IWS_INACTIVE ) )
					GetParent()->iwm_command( this, cmd, dwID );
			}
		}
		break;
	case ID_SKILL_SLOT1:
	case ID_SKILL_SLOT2:
	case ID_SKILL_SLOT3:
	case ID_SKILL_SLOT4:
		if( cmd == IOBN_BTNUP )
		{
			GrowthSlotBtn *pSlot = dynamic_cast<GrowthSlotBtn*>(FindChildWnd(dwID));
			if( !pSlot ) return;

			// 상위로 정보 넘겨준다
			if( GetParent() )
			{
				if( !HasWndStyle( IWS_INACTIVE ) )
				{
					GetParent()->iwm_command( this, cmd, dwID );
				}
			}

			int iIndex = dwID - ID_SKILL_SLOT1;
			int iGrowthInfoNum = m_ItemGrowth[iIndex];

			ioHashString szName;
			ioGrowthInfo *pInfo = g_GrowthInfoMgr.GetGrowthInfo( iGrowthInfoNum );
			if( pInfo )
				szName = pInfo->GetGrowthInfoName();

			int iCurLevel = m_ItemLevel[iIndex];
			int iNextLevel = iCurLevel+1;
#ifdef _LSWC
			if( param == GrowthSlotBtn::ID_AGING_BTN && iCurLevel > 0 ) //육성 복구
			{
				SP2Packet kPacket( CTPK_ETCITEM_USE ); //park 다운시 여기걸림
				kPacket << (int)ioEtcItem::EIT_ETC_GROWTH_DOWN;
				kPacket << m_iClassType;
				kPacket << true;
				kPacket << iIndex;
				TCPNetwork::SendToServer( kPacket );
				TCPNetwork::MouseBusy( true );
				return;
			}
			if( param == GrowthSlotBtn::ID_AGING_BTN && iCurLevel <= 0 )
				return;
#endif
			int iCurMax = g_MyInfo.GetClassLevel( m_iClassType, true );
			iCurMax = min( iCurMax, g_GrowthInfoMgr.GetMaxLevel() );

			bool bHired = false;
			int iCharArray = g_MyInfo.GetClassArray( m_iClassType );
			if( g_MyInfo.IsCharMortmain( m_iClassType ) )
				bHired = true;
			else if( g_MyInfo.IsCharExerciseStyle( iCharArray, EXERCISE_GENERAL ) )
				bHired = true;
			else if( g_MyInfo.IsCharExerciseStyle( iCharArray, EXERCISE_PCROOM ) )
				bHired = true;
			else if( g_MyInfo.IsCharExerciseStyle( iCharArray, EXERCISE_EVENT ) )
				bHired = true;
			else if( g_MyInfo.IsCharExerciseStyle( iCharArray, EXERCISE_RENTAL ) )
				bHired = true;
			else
			{
				if( g_MyInfo.GetCharTotalSecond( iCharArray ) > 0 )
					bHired = true;
			}

			if( g_MyInfo.IsCharExerciseStyle( g_MyInfo.GetClassArray( m_iClassType ), EXERCISE_GENERAL ) ||
				g_MyInfo.IsCharExerciseStyle( g_MyInfo.GetClassArray( m_iClassType ), EXERCISE_EVENT )   ||
				g_MyInfo.IsCharExerciseStyle( g_MyInfo.GetClassArray( m_iClassType ), EXERCISE_RENTAL ))
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
				return;
			}

			if( !pSlot->IsTimeGrowth() && !bHired )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
				return;
			}

			if( Help::IsMonsterDungeonMode( ioPlayMode::GetModeType() ) )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(18) );
				return;
			}

			int iNeedPoint = g_GrowthInfoMgr.GetGrowthUpNeedPoint( false );
			if( !pSlot->IsTimeGrowth() && (m_iCurPoint <= 0 || m_iCurPoint < iNeedPoint) )
			{
				ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
				kPrinter[0].SetTextStyle( TS_NORMAL );
				kPrinter[0].SetBkColor( 0, 0, 0 );	
				kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
				kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(3) );

				kPrinter[1].SetTextStyle( TS_NORMAL );
				kPrinter[1].SetBkColor( 0, 0, 0 );	
				kPrinter[1].SetTextColor( TCT_DEFAULT_GRAY );
				kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(4) );

				g_GUIMgr.SetPrevMsgListBox( NULL, MB_OK, NULL, kPrinter );
				return;
			}
//#ifndef BALANCE_RENEWAL
			if( iNextLevel > iCurMax )
			{
				ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
				kPrinter[0].SetTextStyle( TS_NORMAL );
				kPrinter[0].SetBkColor( 0, 0, 0 );	
				kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
				kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(5) );

				kPrinter[1].SetTextStyle( TS_NORMAL );
				kPrinter[1].SetBkColor( 0, 0, 0 );	
				kPrinter[1].SetTextColor( TCT_DEFAULT_GRAY );
				kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(6) );

				g_GUIMgr.SetPrevMsgListBox( NULL, MB_OK, NULL, kPrinter );
				return;
			}
//#endif

			if( g_MyInfo.IsCharRentalToClassType( m_iClassType ) )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(13) );
				return;
			}

			if( g_MyInfo.IsCharExerciseStyleToClassType( m_iClassType, EXERCISE_RENTAL ) )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(14) );
				return;
			}	

			m_dwCurWndID = dwID;
			int iSlot = iIndex + 1;
			CheckSelectInfo(  m_SkillIconName[iIndex], m_ItemSkillText[iIndex], szName, iSlot, iCurLevel, iGrowthInfoNum );
		}
		break;
	case ID_CHAR_SLOT1:
	case ID_CHAR_SLOT2:
	case ID_CHAR_SLOT3:
	case ID_CHAR_SLOT4:
		if( cmd == IOBN_BTNUP )
		{
			GrowthSlotBtn *pSlot = dynamic_cast<GrowthSlotBtn*>(FindChildWnd(dwID));
			if( !pSlot ) return;

			if( GetParent() )
			{
				if( !HasWndStyle( IWS_INACTIVE ) )
					GetParent()->iwm_command( this, cmd, dwID );
			}

			int iIndex = dwID - ID_CHAR_SLOT1;
			int iGrowthInfoNum = m_CharGrowth[iIndex];

			ioHashString szName;
			ioGrowthInfo *pInfo = g_GrowthInfoMgr.GetGrowthInfo( iGrowthInfoNum );
			if( pInfo )
				szName = pInfo->GetGrowthInfoName();

			int iCurLevel = m_CharLevel[iIndex];
			int iNextLevel = iCurLevel+1;
#ifdef _LSWC
			if( param == GrowthSlotBtn::ID_AGING_BTN && iCurLevel > 0 ) //육성 복구
			{
				SP2Packet kPacket( CTPK_ETCITEM_USE ); 
				kPacket << (int)ioEtcItem::EIT_ETC_GROWTH_DOWN;
				kPacket << m_iClassType;
				kPacket << false;
				kPacket << iIndex;
				TCPNetwork::SendToServer( kPacket );
				TCPNetwork::MouseBusy( true );
				return;
			}
			if( param == GrowthSlotBtn::ID_AGING_BTN && iCurLevel <= 0 )
				return;
#endif
			int iCurMax = g_MyInfo.GetClassLevel( m_iClassType, true );
			iCurMax = min( iCurMax, g_GrowthInfoMgr.GetMaxLevel() );

			bool bHired = false;
			int iCharArray = g_MyInfo.GetClassArray( m_iClassType );
			if( g_MyInfo.IsCharMortmain( m_iClassType ) )
				bHired = true;
			else if( g_MyInfo.IsCharExerciseStyle( iCharArray, EXERCISE_GENERAL ) )
				bHired = true;
			else if( g_MyInfo.IsCharExerciseStyle( iCharArray, EXERCISE_PCROOM ) )
				bHired = true;
			else if( g_MyInfo.IsCharExerciseStyle( iCharArray, EXERCISE_EVENT ) )
				bHired = true;
			else if( g_MyInfo.IsCharExerciseStyle( iCharArray, EXERCISE_RENTAL ) )
				bHired = true;
			else
			{
				if( g_MyInfo.GetCharTotalSecond( iCharArray ) > 0 )
					bHired = true;
			}

			if( g_MyInfo.IsCharExerciseStyle( g_MyInfo.GetClassArray( m_iClassType ), EXERCISE_GENERAL ) ||
				g_MyInfo.IsCharExerciseStyle( g_MyInfo.GetClassArray( m_iClassType ), EXERCISE_EVENT )   ||
				g_MyInfo.IsCharExerciseStyle( g_MyInfo.GetClassArray( m_iClassType ), EXERCISE_RENTAL ))
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(7) );
				return;
			}

			if( !pSlot->IsTimeGrowth() && !bHired )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(8) );
				return;
			}
						
			if( Help::IsMonsterDungeonMode( ioPlayMode::GetModeType() ) )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
				return;
			}

			int iNeedPoint = g_GrowthInfoMgr.GetGrowthUpNeedPoint( true );
			if( !pSlot->IsTimeGrowth() && (m_iCurPoint <= 0 || m_iCurPoint < iNeedPoint) )
			{
				ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
				kPrinter[0].SetTextStyle( TS_NORMAL );
				kPrinter[0].SetBkColor( 0, 0, 0 );	
				kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
				kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(9) );

				kPrinter[1].SetTextStyle( TS_NORMAL );
				kPrinter[1].SetBkColor( 0, 0, 0 );	
				kPrinter[1].SetTextColor( TCT_DEFAULT_GRAY );
				kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(10) );

				g_GUIMgr.SetPrevMsgListBox( NULL, MB_OK, NULL, kPrinter );
				return;
			}

//#ifndef BALANCE_RENEWAL
			if( iNextLevel > iCurMax )
			{
				ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
				kPrinter[0].SetTextStyle( TS_NORMAL );
				kPrinter[0].SetBkColor( 0, 0, 0 );	
				kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
				kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(11) );

				kPrinter[1].SetTextStyle( TS_NORMAL );
				kPrinter[1].SetBkColor( 0, 0, 0 );	
				kPrinter[1].SetTextColor( TCT_DEFAULT_GRAY );
				kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(12) );

				g_GUIMgr.SetPrevMsgListBox( NULL, MB_OK, NULL, kPrinter );
				return;
			}
//#endif

			if( g_MyInfo.IsCharRentalToClassType( m_iClassType ) )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(15) );
				return;
			}

			if( g_MyInfo.IsCharExerciseStyleToClassType( m_iClassType, EXERCISE_RENTAL ) )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(16) );
				return;
			}	

			m_dwCurWndID = dwID;
			int iSlot = iIndex + 4 + 1;
			CheckSelectInfo( m_CharIconName[iIndex], m_CharGrowthText[iIndex], szName, iSlot, iCurLevel, iGrowthInfoNum );
		}
		break;
	}
}

void InventorySoldierItemWnd::CheckSelectInfo( const ioHashString& szIcon,
											  const ioHashString& szSlotName,
											  const ioHashString &szGrowthName,
											  int iSlot,
											  int iCurLevel,
											  int iInfoNum )
{
	ioGrowthSelectWnd *pWnd = dynamic_cast<ioGrowthSelectWnd*>(g_GUIMgr.FindWnd(GROWTH_SELECT_WND));
	if( pWnd )
	{
		pWnd->SetGrowthSelectInfo( szIcon, szSlotName, szGrowthName, m_iClassType, iSlot, iCurLevel, iInfoNum, this );
	}
}

//void InventorySoldierItemWnd::OnProcess( float fTimePerSec )
//{
//}

void InventorySoldierItemWnd::BeforeRenderDesc()
{
	char szText[MAX_PATH]="";

	DWORD dwColor = TCT_DEFAULT_RED;
	bool bHired = false;
	int iCharArray = g_MyInfo.GetClassArray( m_iClassType );

	//용병진화관련
	ioHashString szPowerupName;
	const ioClassExpert::CLASSSLOT* pClassSlot = g_MyInfo.GetClassExpertSlot( m_iClassType );
	if ( pClassSlot && pClassSlot->m_ePowerUpCharGrade != PUGT_NONE )
		szPowerupName = g_PowerUpManager.GetCharGradeTitle( m_iClassType, pClassSlot->m_ePowerUpCharGrade );

	if( g_MyInfo.IsCharExerciseStyle( iCharArray, EXERCISE_RENTAL ) )
	{
		SafeSprintf( szText, sizeof( szText ), STR(8) );
	}
	else if( g_MyInfo.IsCharMortmain( m_iClassType ) )
	{		
		if( szPowerupName.IsEmpty() )
		{			
			SafeSprintf( szText, sizeof( szText ), STR(1) );
		}
		else
			SafeSprintf( szText, sizeof( szText ), STR(9), szPowerupName.c_str() );

		dwColor = TCT_DEFAULT_BLUE;
	}
	else if( g_MyInfo.IsCharExerciseStyle( iCharArray, EXERCISE_GENERAL ) )
	{
		if( szPowerupName.IsEmpty() )		
			SafeSprintf( szText, sizeof( szText ), STR(2) );		
		else
			SafeSprintf( szText, sizeof( szText ), STR(10), szPowerupName.c_str() );
	}
	else if( g_MyInfo.IsCharExerciseStyle( iCharArray, EXERCISE_PCROOM ) && !g_MyInfo.IsUserEvent() )
	{
		if(g_MyInfo.BeforeHeroEvent() == true)
		{
			if( szPowerupName.IsEmpty() )
				SafeSprintf( szText, sizeof( szText ), STR(10) );
			else
				SafeSprintf( szText, sizeof( szText ), STR(1), szPowerupName.c_str() );
		}
		else if( szPowerupName.IsEmpty() )
			SafeSprintf( szText, sizeof( szText ), STR(3) );
		else
			SafeSprintf( szText, sizeof( szText ), STR(11), szPowerupName.c_str() );
	}
	else if( g_MyInfo.IsCharExerciseStyle( iCharArray, EXERCISE_PCROOM ) && g_MyInfo.IsUserEvent() )
	{
		if( szPowerupName.IsEmpty() )
			SafeSprintf( szText, sizeof( szText ), STR(10) );
		else
			SafeSprintf( szText, sizeof( szText ), STR(2), szPowerupName.c_str() );
	}
	else if( g_MyInfo.IsCharExerciseStyle( iCharArray, EXERCISE_EVENT ) )
	{
		if( szPowerupName.IsEmpty() )
			SafeSprintf( szText, sizeof( szText ), STR(4) );
		else
			SafeSprintf( szText, sizeof( szText ), STR(12), szPowerupName.c_str() );
	}
	else
	{
		if( g_MyInfo.GetCharTotalSecond( iCharArray ) <= 0 )
			dwColor = TCT_DEFAULT_GRAY;

		int iTotalLimitMinute = g_MyInfo.GetCharTotalSecond( iCharArray ) / 60;
		int iHour     = iTotalLimitMinute / 60;      
		int iMinute   = iTotalLimitMinute % 60;

		if( iHour == 0 )
		{
			if( szPowerupName.IsEmpty() )
				SafeSprintf( szText, sizeof( szText ), STR(5), iMinute );
			else			
				SafeSprintf( szText, sizeof( szText ), STR(13), szPowerupName.c_str(), iMinute );	
		}
		else if( iMinute == 0 )
		{
			if( szPowerupName.IsEmpty() )
				SafeSprintf( szText, sizeof( szText ), STR(6), iHour );
			else
				SafeSprintf( szText, sizeof( szText ), STR(14), szPowerupName.c_str(), iHour );
		}
		else
		{
			if( szPowerupName.IsEmpty() )
				SafeSprintf( szText, sizeof( szText ), STR(7), iHour, iMinute );
			else
				SafeSprintf( szText, sizeof( szText ), STR(15), szPowerupName.c_str(), iHour, iMinute );
		}
	}

	m_ClassInfo[1].ClearList();
	m_ClassInfo[1].SetTextStyle( TS_NORMAL );
	m_ClassInfo[1].SetBkColor( 0, 0, 0 );
	m_ClassInfo[1].SetTextColor( dwColor );
	m_ClassInfo[1].AddTextPiece( FONT_SIZE_13, szText );
}

void InventorySoldierItemWnd::OnRender()
{
	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	if( HasWndStyle( IWS_INACTIVE ) )
	{
		if( m_pDisableFrm )
			m_pDisableFrm->Render( iXPos, iYPos );
		else if( m_pNormalFrm )
			m_pNormalFrm->Render( iXPos, iYPos );

		RenderStrip();

		if( m_pEmptyIcon )
			m_pEmptyIcon->Render( iXPos+69, iYPos+48, UI_RENDER_MULTIPLY );

		g_FontMgr.SetTextStyle( TS_NORMAL );
		g_FontMgr.SetAlignType( TAT_CENTER );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
		g_FontMgr.PrintText( iXPos+69, iYPos+96, FONT_SIZE_13,  STR(1) );
	}
	else
	{
		ioButton::OnRender();

		if( m_pBottomFrm )
		{
			m_pBottomFrm->Render( iXPos, iYPos, UI_RENDER_MULTIPLY );
		}

		if( m_bSelect )
		{
			if( m_pSelectFrm )
				m_pSelectFrm->Render( iXPos, iYPos );
		}

		RenderStrip();
		BeforeRenderDesc();
		RenderDesc();

		if( m_pBoxLeft && m_pBoxTop )
		{
			m_pBoxLeft->SetReverseFormatting( ioUIRenderImage::RF_NONE_REVERSE );
			m_pBoxLeft->Render( iXPos+7, iYPos+143 );

			m_pBoxTop->SetHorzFormatting( ioUIRenderImage::HF_STRETCH );
			m_pBoxTop->SetVertFormatting( ioUIRenderImage::VF_STRETCH );
			m_pBoxTop->SetSize( 30, 10 );
			
			m_pBoxTop->Render( iXPos+14, iYPos+143 );
			m_pBoxTop->Render( iXPos+95, iYPos+143 );

			m_pBoxLeft->SetReverseFormatting( ioUIRenderImage::RF_HORZ_REVERSE );
			m_pBoxLeft->Render( iXPos+125, iYPos+143 );
		}

		if( m_pCurGrowthBack )
		{
			m_pCurGrowthBack->Render( iXPos+7, iYPos+143 );
		}

		g_FontMgr.SetTextStyle( TS_NORMAL );
		g_FontMgr.SetAlignType( TAT_CENTER );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextColor( 167, 167, 167 );
		g_FontMgr.PrintText( iXPos+69, iYPos+138, FONT_SIZE_11,  STR(2) );
	}
}

void InventorySoldierItemWnd::RenderStrip()
{
	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	enum
	{
		STRIP_X		= 7,
		STRIP_Y		= 155,
		STRIP_GAP	= 21,
	};

	if( m_pDarkStrip && m_pLightStrip )
	{
		m_pDarkStrip->SetHorzFormatting( ioUIRenderImage::HF_STRETCH );
		m_pDarkStrip->SetVertFormatting( ioUIRenderImage::VF_STRETCH );
		m_pDarkStrip->SetSize( 125, 21 );

		m_pLightStrip->SetHorzFormatting( ioUIRenderImage::HF_STRETCH );
		m_pLightStrip->SetVertFormatting( ioUIRenderImage::VF_STRETCH );
		m_pLightStrip->SetSize( 125, 21 );

		for( int i=0; i < 8; ++i )
		{
			if( (i%2) == 0 )
				m_pDarkStrip->Render( iXPos+STRIP_X, iYPos+STRIP_Y+(STRIP_GAP*i), UI_RENDER_MULTIPLY );
			else
				m_pLightStrip->Render( iXPos+STRIP_X, iYPos+STRIP_Y+(STRIP_GAP*i), UI_RENDER_MULTIPLY );
		}
	}
}

void InventorySoldierItemWnd::RenderDesc()
{
	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	m_ClassInfo[0].PrintFullText( iXPos+69, iYPos+96, TAT_CENTER );
	m_ClassInfo[1].PrintFullText( iXPos+69, iYPos+96+16, TAT_CENTER );

	m_PointText.PrintFullText( iXPos+69, iYPos+337, TAT_CENTER );

	if( m_pPointImg )
	{
		// /2.0f 를 * FLOAT05로 변경
		float fWidth = m_PointText.GetFullWidth() * FLOAT05;
		m_pPointImg->Render( iXPos+69+fWidth+FLOAT1, iYPos+337 );
	}
}

void InventorySoldierItemWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "Growth_Back" )
	{
		SAFEDELETE( m_pCurGrowthBack );
		m_pCurGrowthBack = pImage;
	}
	else if( szType == "P_Image" )
	{
		SAFEDELETE( m_pPointImg );
		m_pPointImg = pImage;
	}
	else if( szType == "empty_icon" )
	{
		SAFEDELETE( m_pEmptyIcon );
		m_pEmptyIcon = pImage;
	}
	else if( szType == "box_left" )
	{
		SAFEDELETE( m_pBoxLeft );
		m_pBoxLeft = pImage;
	}
	else if( szType == "box_top" )
	{
		SAFEDELETE( m_pBoxTop );
		m_pBoxTop = pImage;
	}
	else if( szType == "dark_strip" )
	{
		SAFEDELETE( m_pDarkStrip );
		m_pDarkStrip = pImage;
	}
	else if( szType == "light_strip" )
	{
		SAFEDELETE( m_pLightStrip );
		m_pLightStrip = pImage;
	}
	else
	{
		ioButton::AddRenderImage( szType, pImage );
	}
}

void InventorySoldierItemWnd::AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame )
{
	if( pFrame )
	{
		if( szType == "BottomFrm" )
		{
			SAFEDELETE( m_pBottomFrm );
			m_pBottomFrm = pFrame;
		}
		else if( szType == "SelectFrm" )
		{
			SAFEDELETE( m_pSelectFrm );
			m_pSelectFrm = pFrame;
		}
		else
		{	
			ioButton::AddRenderFrame( szType, pFrame );
		}
	}
}

void InventorySoldierItemWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	char szKeyName[MAX_PATH]="";
	ioHashString szTemp;

	for (int i = 0; i < CUR_GROWTH_CNT ; i++)
	{
		// CharGrowth IconName
		ZeroMemory( szKeyName, sizeof( szKeyName ) );
		StringCbPrintf_e( szKeyName, sizeof( szKeyName ), "CharIcon%d", i+1 );
		szTemp  = xElement.GetStringAttribute( szKeyName );
		m_CharIconName[i] = szTemp;

		// Char, Item Growth Text
		ZeroMemory( szKeyName, sizeof( szKeyName ) );
		StringCbPrintf_e( szKeyName, sizeof( szKeyName ), "CharGrowth%d", i+1 );
		szTemp  = xElement.GetStringAttribute( szKeyName );
		m_CharGrowthText.push_back( szTemp );

		ZeroMemory( szKeyName, sizeof( szKeyName ) );
		StringCbPrintf_e( szKeyName, sizeof( szKeyName ), "ItemGrowth%d", i+1 );
		szTemp  = xElement.GetStringAttribute( szKeyName );
		m_ItemGrowthText.push_back( szTemp );
	}
}

bool InventorySoldierItemWnd::QuestGuide( int iClassType, float &rfXPos, float &rfYPos )
{
	if( m_iClassType != iClassType )
		return false;

	ioWnd *pIconBtn = FindChildWnd( ID_ICON_BTN );
	if( !pIconBtn ) return false;
	if( !pIconBtn->IsShow() ) return false;

	rfXPos = pIconBtn->GetDerivedPosX() + (pIconBtn->GetWidth() / 2);
	rfYPos = pIconBtn->GetDerivedPosY();
	return true;
}

//////////////////////////////////////////////////////////////////////////

bool MyInventoryWnd::m_bGashaponListRequest = false;

MyInventoryWnd::MyInventoryWnd()
{
	m_pPlayStage = NULL;
	m_pTabDot = NULL;

	m_dwExtraItemChangeTime = 0;
	m_dwMedalItemChangeTime = 0;

	m_iDefaultTabType = ID_SOLDIER_TAB_BTN;

	m_dwCurTabID = ID_SOLDIER_TAB_BTN;
	m_dwSmallTabEndID = ID_SMALL_TAB_1_BTN;

	m_bRoomOutTimeCheck = false;
	m_bCharItem = false;

	m_iSelectClassType = -1;
	m_iSellSlotIndex = 0;

	m_bMileage = false;
	m_dwBonusCashTooltipCheckStartTime = 0;
}

MyInventoryWnd::~MyInventoryWnd()
{
	SAFEDELETE( m_pTabDot );

	for (int i = 0; i < MAX_TAB_BTN ; i++)
	{
		m_vSmallTabNameVecArray[i].clear();
		m_vSmallTabTypeVecArray[i].clear();
	}

	m_vCharInfoList.clear();
	m_vInventoryItemInfoList.clear();
	m_vAlchemicItemInfoList.clear();
}

void MyInventoryWnd::iwm_create()
{
	g_GUIMgr.AddWnd( "XML/MyInventoryInfoWnd.xml", this );
	g_GUIMgr.AddWnd( "XML/growthdownselectwnd.xml", this );
	g_GUIMgr.AddWnd( "XML/itemcompoundwnd.xml", this );
	g_GUIMgr.AddWnd( "XML/ItemMultipleCompoundWnd.xml", this );
	g_GUIMgr.AddWnd( "XML/ItemMultipleCompoundResultWnd.xml", this );
	g_GUIMgr.AddWnd( "XML/PackageItemBuyWnd.xml", this );
	g_GUIMgr.AddWnd( "XML/GashaponWnd.xml", this );
	g_GUIMgr.AddWnd( "XML/RandomDecoWnd.xml", this );
	g_GUIMgr.AddWnd( "XML/RandomDecoSelectWnd.xml", this );
	g_GUIMgr.AddWnd( "XML/ItemCompoundResultWnd.xml", this );
	g_GUIMgr.AddWnd( "XML/ExtraItemRandomBox.xml", this );
	g_GUIMgr.AddWnd( "XML/growthdowndonewnd.xml", this );
	g_GUIMgr.AddWnd( "XML/PackageItemBuyResultWnd.xml", this );
	g_GUIMgr.AddWnd( "XML/ChangeNameWnd.xml", this );
	g_GUIMgr.AddWnd( "XML/ChangeNameResultWnd.xml", this );
	g_GUIMgr.AddWnd( "XML/ChangeTradeStateResultWnd.xml", this );
	g_GUIMgr.AddWnd( "XML/ItemRecvSellInfoWnd.xml", this );
	g_GUIMgr.AddWnd( "XML/RecordInitWnd.xml", this );
	g_GUIMgr.AddWnd( "XML/RecordInitResultWnd.xml", this );
	g_GUIMgr.AddWnd( "XML/itemgrowthcatalystwnd.xml", this );
	g_GUIMgr.AddWnd( "XML/itemgrowthcatalystresultwnd.xml", this );
	g_GUIMgr.AddWnd( "XML/itemcompoundexwnd.xml", this );
	g_GUIMgr.AddWnd( "XML/itemcompoundexresultwnd.xml", this );
	g_GUIMgr.AddWnd( "XML/itemluckycoinwnd.xml", this );
	g_GUIMgr.AddWnd( "XML/itemluckycoinresultwnd.xml", this );
	g_GUIMgr.AddWnd( "XML/itemrainbowmixerwnd.xml", this );
	g_GUIMgr.AddWnd( "XML/itemrainbowmixerresultwnd.xml", this );
	g_GUIMgr.AddWnd( "XML/itemlostsagamixerwnd.xml", this );
	g_GUIMgr.AddWnd( "XML/itemlostsagamixerresultwnd.xml", this );
	g_GUIMgr.AddWnd( "XML/itemgoldboxwnd.xml", this );
	g_GUIMgr.AddWnd( "XML/invensoldierselectbuywnd.xml", this );
	g_GUIMgr.AddWnd( "XML/invensoldierselectbuyresultwnd.xml", this );
	g_GUIMgr.AddWnd( "XML/InvenFourExtraCompoundBuyWnd.xml", this );
	g_GUIMgr.AddWnd( "XML/InvenFourExtraCompoundBuyResultWnd.xml", this );
	g_GUIMgr.AddWnd( "XML/invensoldierexpbonuswnd.xml", this );
	g_GUIMgr.AddWnd( "XML/invensoldierexpbonusresultwnd.xml", this );
	g_GUIMgr.AddWnd( "XML/ExpandMedalSlotWnd.xml", this );
	g_GUIMgr.AddWnd( "XML/ExpandMedalSlotResultWnd.xml", this );
	g_GUIMgr.AddWnd( "XML/AlchemicFuncWnd.xml", this );
	g_GUIMgr.AddWnd( "XML/PreSetSoldierPackageWnd.xml", this );
	g_GUIMgr.AddWnd( "XML/PreSetSoldierPackageResultWnd.xml", this );
	g_GUIMgr.AddWnd( "XML/growthalldownselectwnd.xml", this );
	g_GUIMgr.AddWnd( "XML/growthalldowndonewnd.xml", this );
	g_GUIMgr.AddWnd( "XML/supergashponinvenwnd.xml", this );

	g_GUIMgr.AddWnd( "XML/itemmaterialcompoundwnd.xml", this );
	g_GUIMgr.AddWnd( "XML/itemmaterialcompoundresultwnd.xml", this );
	g_GUIMgr.AddWnd( "XML/peteggresultwnd.xml", this );

	//소울스톤 강화용병
	g_GUIMgr.AddWnd( "XML/heroreinforcedownwnd.xml",this );
	g_GUIMgr.AddWnd( "XML/heroreinforcedonewnd.xml",this );
	g_GUIMgr.AddWnd( "XML/itemreinforcedownwnd.xml",this );
	g_GUIMgr.AddWnd( "XML/itemreinforcedonewnd.xml",this );

	// UI코스튬 관련 (코스튬 박스)
	g_GUIMgr.AddWnd( "XML/CostumeRandomBox.xml", this );

	g_GUIMgr.AddWnd( "XML/Itemnewmultiplecompoundwnd.xml", this );
	g_GUIMgr.AddWnd( "XML/namedtitleresultwnd.xml", this );
	g_GUIMgr.AddWnd( "XML/namedtitlepremiumresultwnd.xml", this );
	g_GUIMgr.AddWnd( "XML/namedtitlepremiumselectwnd.xml", this );

	UppderAlphaWnd *pFlashWnd = dynamic_cast<UppderAlphaWnd*> ( FindChildWnd( ID_FLASH_WND ) );
	if( pFlashWnd )
		pFlashWnd->SetSizeFromParent( false );

	ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
	if( !pLocal )
		return;
	m_bMileage = pLocal->IsMileage();
}

void MyInventoryWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "TabDot" )
	{
		SAFEDELETE( m_pTabDot );
		m_pTabDot = pImage;
	}
	else
	{
		ioMovingWnd::AddRenderImage( szType, pImage );	
	}
}

void MyInventoryWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	ioHashString sSmallTabName;
	int          iType = 0;
	for (int i = 0; i < MAX_TAB_BTN ; i++)
	{
		char szName[MAX_PATH]="";
		StringCbPrintf_e( szName, sizeof( szName ), "Max_%d", ID_SOLDIER_TAB_BTN+i );
		int iMax = xElement.GetIntAttribute( szName );
		for (int j = 0; j < iMax ; j++)
		{
			StringCbPrintf_e( szName, sizeof( szName ), "Name_%d_%d", ID_SOLDIER_TAB_BTN+i, j+1 );
			sSmallTabName = xElement.GetStringAttribute( szName );
			m_vSmallTabNameVecArray[i].push_back( sSmallTabName );

			StringCbPrintf_e( szName, sizeof( szName ), "Type_%d_%d", ID_SOLDIER_TAB_BTN+i, j+1 );
			iType = xElement.GetIntAttribute( szName );
			m_vSmallTabTypeVecArray[i].push_back( iType );
		}

		StringCbPrintf_e( szName, sizeof( szName ), "Default_%d", ID_SOLDIER_TAB_BTN+i );
		bool bUse = xElement.GetBoolAttribute( szName );
		if( bUse )
		{
			StringCbPrintf_e( szName, sizeof( szName ), "Default_Type_%d", ID_SOLDIER_TAB_BTN+i );
			m_iDefaultTypeArray[i] = xElement.GetIntAttribute( szName );
		}
		else
			m_iDefaultTypeArray[i] = ETC_ITEM_OTHER_VALUE;
	}

	m_iDefaultTabType = xElement.GetIntAttribute_e( "Default_Tab_Type" );
	
	m_nCostumeHelpTipIdx = xElement.GetIntAttribute_e( "costume_help_tip_idx" );

	m_dwBonusCashTooltipCheckDuration = (DWORD)xElement.GetIntAttribute_e( "BonusCashTooltipCheckDuration" );
}

void MyInventoryWnd::SetPlayStage( ioPlayStage *pPlayStage )
{
	m_pPlayStage = pPlayStage;
}

bool MyInventoryWnd::IsNetworkMode()
{
	if( m_pPlayStage == NULL )
		return false;

	return m_pPlayStage->IsNetworkMode();
}

void MyInventoryWnd::iwm_show()
{
	// 현재 select classtype
	m_iSelectClassType = g_MyInfo.GetClassType();
	m_iSellSlotIndex = 0;

	SendRequestGashaponList();

	m_bRoomOutTimeCheck = false;

	enum { Y_OFFSET = -20 };
	int iXPos = ( Setting::Width() - GetWidth() ) / 2;
	int iYPos = ( Setting::Height() - GetHeight() ) / 2 + Y_OFFSET;

	PetWnd *pPetWnd = (PetWnd*)(g_GUIMgr.FindWnd(PET_WND));
	if ( pPetWnd && pPetWnd->IsShow() )
		pPetWnd->HideWnd();

	NewShopWnd *pShopWnd = dynamic_cast<NewShopWnd*>(g_GUIMgr.FindWnd(NEW_SHOP_WND));
	if( pShopWnd && pShopWnd->IsShow() )
	{
		iXPos = pShopWnd->GetDerivedPosX();
		iYPos = pShopWnd->GetDerivedPosY();
		pShopWnd->HideWnd();
	}
	ioMovingWnd::SetWndPos( iXPos, iYPos );

	if( !g_App.IsWemadeChannelingGetCash() )
	{
		if( pShopWnd )
			pShopWnd->SendGetCash();
	}

	DWORD dwSmallTab = GetDefaultSmallTab( m_iDefaultTabType );
	ChangeMainTab( m_iDefaultTabType );
	ChangeSmallTab( dwSmallTab );

	if( m_dwCurTabID == ID_ALCHEMIC_TAB_BTN )
	{
		ShowMainInfoWnd();
		ShowAlchemicFuncWnd();
	}
	else
	{
		SetFirstBtnInfo();

		if( m_vInventoryItemInfoList.empty() )
		{
			ChangeMainTab( m_iDefaultTabType );
			SetFirstBtnInfo();
		}
	}
}

void MyInventoryWnd::iwm_hide()
{
	SetAlchemicItemLockClear();

	for (int i = 0; i < MAX_ITEM_INFO_WND_BTN ; i++)
		HideChildWnd( ID_ITEM_INFO_WND_1_BTN + i );

	for (int i= 0; i < MAX_CHAR_INFO_WND_BTN ; i++)
		HideChildWnd( ID_CHAR_INFO_WND_1_BTN + i );

	for ( int i=0; i < MAX_ALCHEMIC_INFO_BTN; ++i )
		HideChildWnd( ID_ALCHEMIC_INFO_WND_1_BTN + i );


	HideChildWnd(ID_INVENTORY_INFO_WND);
	HideChildWnd(ID_GROWTH_DOWN_WND);
	HideChildWnd(ID_ITEM_COMPOUND_WND);
	HideChildWnd(ID_MULTIPLE_COMPOUND_WND);
	HideChildWnd(ID_PACKAGE_ITEM_WND);
	HideChildWnd(ID_GASHPON_WND);
	HideChildWnd(ID_SUPER_GASHAPON_WND);
	HideChildWnd(ID_RANDOM_DECO_WND);
	HideChildWnd(ID_DECO_SELECT_WND);
	HideChildWnd(ID_MULTIPLE_COMPOUND_RESULT);
	HideChildWnd(ID_ITEM_COMPOUND_RESULT);
	HideChildWnd(ID_EXTRAITEM_BOX_WND);
	HideChildWnd(ID_GROWTH_DOWN_RESULT);
	HideChildWnd(ID_PACKAGE_ITEM_RESULT);
	HideChildWnd(ID_CHANGE_NAME_WND);
	HideChildWnd(ID_TRADE_STATE_CHANGE_WND);
	HideChildWnd(ID_RECORD_INIT_WND);
	HideChildWnd(ID_RECORD_INIT_RESULT_WND);
	HideChildWnd(ID_ITEM_GROWTH_CATALYST_WND);
	HideChildWnd(ID_ITEM_GROWTH_CATALYST_RESULT_WND);
	HideChildWnd(ID_ITEM_LUCKY_COIN_USE_WND);
	HideChildWnd(ID_ITEM_LUCKY_COIN_USE_RESULT_WND);
	HideChildWnd(ID_ITEM_COMPOUNDEX_WND);
	HideChildWnd(ID_ITEM_COMPOUNDEX_RESULT_WND);
	HideChildWnd(ID_ITEM_RAINBOW_MIXER_WND );
	HideChildWnd(ID_ITEM_RAINBOW_MIXER_RESULT_WND);
	HideChildWnd(ID_ITEM_LOSTSAGA_MIXER_WND );
	HideChildWnd(ID_ITEM_LOSTSAGA_MIXER_RESULT_WND);
	HideChildWnd(ID_ITEM_GOLD_BOX_WND);
	HideChildWnd(ID_SOLDIER_SELECT_BUY_WND);
	HideChildWnd(ID_SOLDIER_SELECT_RESULT_WND);
	HideChildWnd(ID_FOUR_EXTRA_COMPOUND_BUY_WND);
	HideChildWnd(ID_FOUR_EXTRA_COMPOUND_RESULT_WND);
	HideChildWnd(ID_SOLDIER_EXP_BONUS_SELECT_WND);
	HideChildWnd(ID_SOLDIER_EXP_BONUS_SELECT_RESULT_WND);
	HideChildWnd(ID_EXPAND_MEDAL_SLOT_OPEN_WND);
	HideChildWnd(ID_EXPAND_MEDAL_SLOT_RESULT_WND);
	HideChildWnd(ID_ALCHEMICFUNC_WND);
	HideChildWnd(ID_PRESET_PACKAGE_WND);
	HideChildWnd(ID_PRESET_PACKAGE_RESULT_WND);
	HideChildWnd(ID_GROWTH_ALL_DOWN_WND);
	HideChildWnd(ID_GROWTH_ALL_DOWN_RESULT);

	HideChildWnd(ID_ITEM_MATERIAL_COMPOUND_WND);
	HideChildWnd(ID_ITEM_MATERIAL_COMPOUND_RESULT);

	HideChildWnd(ID_PET_EGG_RESULT_WND);
	HideChildWnd(ID_NAMED_TITLE_RESULT_WND);
	HideChildWnd(ID_NAMED_TITLE_PREMIUM_RESULT_WND);
	HideChildWnd(ID_NAMED_TITLE_PREMIUM_SELECT_WND);

	HideChildWnd(ID_HERO_REINFORCE_DWON_WND);
	HideChildWnd(ID_HERO_REINFORCE_DONE_WND);
	HideChildWnd(ID_ITEM_REINFORCE_DWON_WND);
	HideChildWnd(ID_ITEM_REINFORCE_DONE_WND);
	HideChildWnd(ID_COSTUME_BOX_WND);
	HideChildWnd(ID_NEW_MULTIPLE_COMPOUND_WND);

	m_bRoomOutTimeCheck = false;
}

void MyInventoryWnd::iwm_vscroll( DWORD dwID, int curPos )
{
	if( m_dwCurTabID == ID_ALCHEMIC_TAB_BTN )
		return;

	UpdatePage( curPos );
}

void MyInventoryWnd::iwm_wheel( int zDelta )
{
	if( !IsShow() )
	{
		ioMovingWnd::iwm_wheel( zDelta );
	}
	else
	{ 
		// 휠 이벤트 처리를 했다면 부모로 이벤트를 날리지 않는다.
		if( zDelta == WHEEL_DELTA )
		{
			ioScroll *pScroll = (ioScroll*)FindChildWnd( ID_VERT_SCROLL );
			if( pScroll )
				pScroll->SetScrollPos( m_iCurPage - 1 );

			if( g_GUIMgr.GetPreOverWnd() )
			{
				ioMouse NullMouse;
				g_GUIMgr.GetPreOverWnd()->iwm_mouseover( NullMouse );
			}
		}
		else if( zDelta == -WHEEL_DELTA )
		{
			ioScroll *pScroll = (ioScroll*)FindChildWnd( ID_VERT_SCROLL );
			if( pScroll )
				pScroll->SetScrollPos( m_iCurPage + 1 );

			if( g_GUIMgr.GetPreOverWnd() )
			{
				ioMouse NullMouse;
				g_GUIMgr.GetPreOverWnd()->iwm_mouseover( NullMouse );
			}
		}
	}
}

void MyInventoryWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_CHARGE_GOLD_BTN:
		if( cmd == IOBN_BTNUP )
		{
			if( g_App.GetConnectedMoveMode() == ioApplication::CMM_LOBBY )
			{
				if( m_pPlayStage && m_pPlayStage->GetModeType() != MT_MYROOM )
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
					return;
				}
			}
			else
			{
				if( m_pPlayStage && m_pPlayStage->GetModeType() != MT_TRAINING && m_pPlayStage->GetModeType() != MT_HEADQUARTERS && m_pPlayStage->GetModeType() != MT_HOUSE )
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
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
		break;
	case ID_SHOP_BTN:
		if( cmd == IOBN_BTNUP )
		{
			NewShopWnd *pShopWnd = dynamic_cast<NewShopWnd*>(g_GUIMgr.FindWnd(NEW_SHOP_WND));
			if( pShopWnd )
			{
#if defined (_LSWC)
				pShopWnd->ShowSoldierTab();
				break;
#else
				switch( m_dwCurTabID )
				{
				case ID_SOLDIER_TAB_BTN:
					pShopWnd->ShowSoldierTab();
					break;
				case ID_EXTRA_TAB_BTN:
				case ID_ACCESSORY_TAB_BTN:
					pShopWnd->ShowExtraItemTab();
					break;
				case ID_DECO_TAB_BTN:
					pShopWnd->ShowDecoItemTab();
					break;
				case ID_ETC_TAB_BTN:
				case ID_ALCHEMIC_TAB_BTN:
					pShopWnd->ShowEtcItemTab();
					break;
					// UI코스튬 관련 (해당 탭에서 상점가기 눌렀을때)
				case ID_COSTUME_TAB_BTN:
					pShopWnd->ShowCostumeItemTab();
					break;
				default:
					pShopWnd->ShowWnd();
					break;
				}
#endif
			}
		}
		break;
	case ID_EXIT_BTN:
		if( cmd == IOBN_BTNUP )
		{
			HideWnd();
		}
		break;
	case ID_SOLDIER_TAB_BTN:
		if( cmd == IOBN_BTNDOWN )
		{
			DWORD dwSmallTab = GetDefaultSmallTab( ID_SOLDIER_TAB_BTN );
			ChangeMainTab( ID_SOLDIER_TAB_BTN );
			ChangeSmallTab( dwSmallTab );
			SetFirstBtnInfo();

			if( m_vInventoryItemInfoList.empty() )
			{
				ChangeMainTab( dwID );

				m_iSelectArray = FindSelectCharArray( m_iSelectClassType );

				int iStartArray = m_iCurPage * PAGE_PER_CHAR_NUM;
				for( int i = 0 ; i < MAX_CHAR_INFO_WND_BTN ; i++ )
				{
					InventorySoldierItemWnd *pItem = dynamic_cast<InventorySoldierItemWnd*>(FindChildWnd(ID_CHAR_INFO_WND_1_BTN + i));
					if( !pItem ) continue;

					int iCurArray = iStartArray + i;
					if( m_iSelectArray == iCurArray )
						pItem->SetSelect( true );
					else
						pItem->SetSelect( false );
				}

				MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
				if( pInfoWnd )
				{
					ShowMainInfoWnd();
					pInfoWnd->SetSlodierInfo( m_iSelectClassType );
				}
			}
		}
		break;
	case ID_EXTRA_TAB_BTN:
	case ID_DECO_TAB_BTN:
	case ID_ETC_TAB_BTN:
	case ID_COSTUME_TAB_BTN:
	case ID_ACCESSORY_TAB_BTN:
		if( cmd == IOBN_BTNDOWN )
		{
			// UI코스튬 관련 (메인 탭 클릭시)
			UISoldierSelectWnd *pUISoliderWnd = dynamic_cast<UISoldierSelectWnd *>(g_GUIMgr.FindWnd( UI_SOLDIERSELECT_WND ));
			if( pUISoliderWnd && pUISoliderWnd->IsShow() )
				pUISoliderWnd->HideWnd();

			ChangeMainTab( dwID );
			SetFirstBtnInfo();
		}
		break;
	case ID_PRESENT_TAB_BTN:
		if( cmd == IOBN_BTNDOWN )
		{
			UISoldierSelectWnd *pUISoliderWnd = dynamic_cast<UISoldierSelectWnd *>(g_GUIMgr.FindWnd( UI_SOLDIERSELECT_WND ));
			if( pUISoliderWnd && pUISoliderWnd->IsShow() )
				pUISoliderWnd->HideWnd();

			ShowSubscriptionTabDirect();
			SetFirstBtnInfo();
		}
		break;
	case ID_ALCHEMIC_TAB_BTN:
		if( cmd == IOBN_BTNDOWN )
		{
			UISoldierSelectWnd *pUISoliderWnd = dynamic_cast<UISoldierSelectWnd *>(g_GUIMgr.FindWnd( UI_SOLDIERSELECT_WND ));
			if( pUISoliderWnd && pUISoliderWnd->IsShow() )
				pUISoliderWnd->HideWnd();

			ChangeMainTab( dwID );

			ShowMainInfoWnd();
			ShowAlchemicFuncWnd();
		}
		break;
	case ID_SMALL_TAB_1_BTN:
	case ID_SMALL_TAB_2_BTN:
	case ID_SMALL_TAB_3_BTN:
	case ID_SMALL_TAB_4_BTN:
	case ID_SMALL_TAB_5_BTN:
	case ID_SMALL_TAB_6_BTN:
	case ID_SMALL_TAB_7_BTN:
	case ID_SMALL_TAB_8_BTN:
	case ID_SMALL_TAB_9_BTN:
	case ID_SMALL_TAB_10_BTN:
		if( cmd == IOBN_BTNDOWN )
		{
			ChangeSmallTab( dwID );

			if( m_dwCurTabID == ID_ALCHEMIC_TAB_BTN )
			{
				ShowMainInfoWnd();
				ShowAlchemicFuncWnd();
			}
			else
				SetFirstBtnInfo();
		}
		break;
	case ID_HOUSE_ITEM_BTN:
		if( cmd == IOBN_BTNDOWN )
		{
			ioHousingBlockInvenWnd* pWnd = dynamic_cast<ioHousingBlockInvenWnd*>( g_GUIMgr.FindWnd( HOUSING_BLOCK_INVEN_WND ) );
			if( pWnd )
			{
				pWnd->SetBlockModeType( BMT_PERSONAL );
				pWnd->ShowWnd();
			}
		}
		else if( cmd == IOEX_BTNUP )
		{
			HomeEtcItemNeedPopup* pWnd = dynamic_cast<HomeEtcItemNeedPopup*>( g_GUIMgr.FindWnd( HOUSE_NEED_POPUP ) );
			if( pWnd )
				pWnd->ShowWnd();
		}
		break;
	case ID_ITEM_INFO_WND_1_BTN:
	case ID_ITEM_INFO_WND_2_BTN:
	case ID_ITEM_INFO_WND_3_BTN:
	case ID_ITEM_INFO_WND_4_BTN:
	case ID_ITEM_INFO_WND_5_BTN:
	case ID_ITEM_INFO_WND_6_BTN:
	case ID_ITEM_INFO_WND_7_BTN:
	case ID_ITEM_INFO_WND_8_BTN:
		if( cmd == IOBN_BTNUP )
		{
			SelectItemBtn( dwID, param );

			InventoryItemWnd *pItem = dynamic_cast<InventoryItemWnd*>(FindChildWnd(dwID));
			if( pItem && param == 0 )
			{
				param = pItem->GetCurActionID();
			}

			if( param == INVENTORY_ACTION_NONE )
				OnItemIconPush( dwID );
			else if( param == INVENTORY_ACTION_EQUIP )
				OnItemEquipPush( dwID );
			else if( param == INVENTORY_ACTION_RELEASE )
				OnItemReleasePush( dwID );
			else if( param == INVENTORY_ACTION_RESELL )
				OnItemReSellPush( dwID );
			else if( param == INVENTORY_ACTION_DETAIL )
				OnItemDetailPush( dwID );
			else if( param == INVENTORY_ACTION_CUSTOM_INFO )
				OnItemCustomInfoPush( dwID );
			else if( param == INVENTORY_ACTION_DISASSEMBLE )
				OnItemDisassemblePush( dwID );
		}
		break;
	case ID_CHAR_INFO_WND_1_BTN:
	case ID_CHAR_INFO_WND_2_BTN:
	case ID_CHAR_INFO_WND_3_BTN:
		if( cmd == IOBN_BTNUP )
		{
			SelectCharItemBtn( dwID, param );
		}
		break;
	case ITEM_RESELL_WND:
		if( cmd == IOBN_BTNUP )
		{
			if( param != ACST_SELL && param != ACST_DISASSEMBLE && param != ACST_SELL_MEDAL 
				&& param != ACST_SELL_COSTUME && param != ACST_SELL_ACCESSORY )
				break;

			switch( m_dwCurTabID )
			{
			case ID_EXTRA_TAB_BTN:
				if( m_iSellSlotIndex > 0 )
				{
					if( param == ACST_SELL )
					{
						if( !CheckTimeEndExtraItem( m_iSellSlotIndex ) )
							return;

						TCPNetwork::MouseBusy( true );

						SP2Packet kPacket( CTPK_EXTRAITEM_SELL );
						kPacket << m_iSellSlotIndex;
						TCPNetwork::SendToServer( kPacket );
					}
					else if( param == ACST_DISASSEMBLE )
					{
						if( !CheckTimeEndExtraItem( m_iSellSlotIndex ) )
							return;

						TCPNetwork::MouseBusy( true );

						SP2Packet kPacket( CTPK_EXTRAITEM_DISASSEMBLE );
						kPacket << m_iSellSlotIndex;
						TCPNetwork::SendToServer( kPacket );
					}
					else if( param == ACST_SELL_MEDAL )
					{
						TCPNetwork::MouseBusy( true );

						SP2Packet kPacket( CTPK_MEDALITEM_SELL );
						kPacket << m_iSellSlotIndex;
						TCPNetwork::SendToServer( kPacket );
					}
				}
				break;
			case ID_COSTUME_TAB_BTN:
				if( m_iSellSlotIndex > 0 )
				{
					// UI코스튬 (아이템 판매)
					if( param == ACST_SELL_COSTUME )
					{
						if( !CheckTimeEndCostume( m_iSellSlotIndex ) )
							return;
												
						TCPNetwork::MouseBusy( true );
						SP2Packet kPacket( CTPK_COSTUME_SELL );
						kPacket << m_iSellSlotIndex;
						TCPNetwork::SendToServer( kPacket );
					}
				}
				break;
			case ID_ACCESSORY_TAB_BTN:
				if( m_iSellSlotIndex > 0 )
				{
					if( param == ACST_SELL_ACCESSORY )
					{
						if( !CheckTimeEndAccessory( m_iSellSlotIndex ) )
							return;

						TCPNetwork::MouseBusy( true );
						SP2Packet kPacket( CTPK_ACCESSORY_SELL );
						kPacket << m_iSellSlotIndex;
						TCPNetwork::SendToServer( kPacket );
					}
				}
				break;
			}

			m_iSellSlotIndex = 0;
		}
		break;
	case ID_COSTUME_HELP:
		{
			if( cmd == IOBN_BTNUP )
			{
				HideChildWnd( ID_COSTUME_HELP_TIP );

				ManualListWnd *pManualWnd = dynamic_cast<ManualListWnd*> (g_GUIMgr.FindWnd( MANUAL_LIST_WND ) );
				if( pManualWnd )
					pManualWnd->ShowWndByCurManual( m_nCostumeHelpTipIdx );
			}
			else if( cmd == IOWN_OVERED )
			{
				ShowChildWnd( ID_COSTUME_HELP_TIP );
			}
			else if( cmd == IOWN_LEAVED )
			{
				HideChildWnd( ID_COSTUME_HELP_TIP );
			}
		}
		break;
	case ID_BONUS_CASH_BTN:
		if( cmd == IOBN_BTNUP )
		{
			if ( !g_GUIMgr.IsShow( BONUS_CASH_WND ))
				g_GUIMgr.ShowWnd( BONUS_CASH_WND );
			else
				g_GUIMgr.HideWnd( BONUS_CASH_WND );
		}
		break;
	default:
		{
			if( COMPARE( dwID, ID_ALCHEMIC_INFO_WND_1_BTN, ID_ALCHEMIC_INFO_WND_30_BTN+1) &&
				(cmd == IOBN_BTNUP) )
			{
				SetSelectAlchemicClickAction( dwID );
			}
		}
		break;
	}
}

void MyInventoryWnd::SetSelectAlchemicClickAction( DWORD dwID )
{
	AlchemicItemWnd *pItem = dynamic_cast<AlchemicItemWnd*>(FindChildWnd(dwID));
	if( !pItem )
		return;

	AlchemicFuncWnd *pAlchemicWnd = dynamic_cast<AlchemicFuncWnd*>(FindChildWnd(ID_ALCHEMICFUNC_WND));
	if( pAlchemicWnd && !pItem->IsLockState() )
	{
		pAlchemicWnd->SelectClickAction( pItem->GetItemCode() );
	}
}

void MyInventoryWnd::UpdatePage( int iNewPage )
{
	for ( int i = 0; i < MAX_ITEM_INFO_WND_BTN; ++i )
		HideChildWnd( ID_ITEM_INFO_WND_1_BTN + i );

	for ( int i= 0; i < MAX_CHAR_INFO_WND_BTN; ++i )
		HideChildWnd( ID_CHAR_INFO_WND_1_BTN + i );

	for ( int i=0; i < MAX_ALCHEMIC_INFO_BTN; ++i )
		HideChildWnd( ID_ALCHEMIC_INFO_WND_1_BTN + i );

	m_iCurPage = iNewPage;

	int iCharItemSize = m_vCharInfoList.size();
	int iItemSize = m_vInventoryItemInfoList.size();
	int iAlchemicSize = m_vAlchemicItemInfoList.size();

	if( m_dwCurTabID == ID_ALCHEMIC_TAB_BTN )
	{
		for( int i=0; i < MAX_ALCHEMIC_INFO_BTN; ++i )
		{
			AlchemicItemWnd *pItem = dynamic_cast<AlchemicItemWnd*>(FindChildWnd(ID_ALCHEMIC_INFO_WND_1_BTN + i));
			if( !pItem )
				continue;

			pItem->ShowWnd();

			if( COMPARE( i, 0, iAlchemicSize ) )
				pItem->SetInfo( m_vAlchemicItemInfoList[i].m_iCode,
								m_vAlchemicItemInfoList[i].m_iCount,
								m_vAlchemicItemInfoList[i].m_szIcon,
								m_vAlchemicItemInfoList[i].m_bNewAdd );
			else
				pItem->SetInfo( 0, 0, ioHashString(), false );
		}
	}
	else if( m_bCharItem )
	{
		int iStartArray = m_iCurPage * PAGE_PER_CHAR_NUM;
		for( int i = 0 ; i < MAX_CHAR_INFO_WND_BTN ; i++ )
		{
			InventorySoldierItemWnd *pItem = dynamic_cast<InventorySoldierItemWnd*>(FindChildWnd(ID_CHAR_INFO_WND_1_BTN + i));
			if( !pItem )
				continue;

			pItem->ShowWnd();

			int iCurArray = iStartArray +i;
			if( !COMPARE( iCurArray, 0, iCharItemSize ) )
			{
				// 초기화
				pItem->SetInfo( -1 );
				continue;
			}

			pItem->SetInfo( m_vCharInfoList[iCurArray].m_iClassType );

			// SetInfo 이후 적용
			if( m_iSelectArray == iCurArray )
				pItem->SetSelect( true );
			else
				pItem->SetSelect( false );
		}
	}
	else
	{
		int iStartArray = m_iCurPage * PAGE_PER_ITEM_NUM;
		for( int i = 0 ; i < MAX_ITEM_INFO_WND_BTN ; i++ )
		{
			InventoryItemWnd *pItem = dynamic_cast<InventoryItemWnd*>(FindChildWnd(ID_ITEM_INFO_WND_1_BTN + i));
			if( !pItem )
				continue;

			pItem->ShowWnd();

			int iCurArray = iStartArray +i;
			if( !COMPARE( iCurArray, 0, iItemSize ) )
			{
				// 초기화
				InventoryItemInfo kInfo;
				pItem->SetInfo( m_pPlayStage, kInfo );
				continue;
			}

			pItem->SetInfo( m_pPlayStage, m_vInventoryItemInfoList[iCurArray] );

			// SetInfo 이후 적용
			if( m_iSelectArray == iCurArray )
			{
				pItem->SetSelect( true );
			}
			else
			{
				pItem->SetSelect( false );
			}
		}
	}
}

void MyInventoryWnd::SetFirstBtnInfo()
{
	// 각 텝의 첫번째걸로 오른쪽 창에 정보 전달하는 함수.
	// 일반적인 형태로 텝 변경할 경우 호출.

	if( m_dwCurTabID == ID_ALCHEMIC_TAB_BTN )
		return;

	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( !pInfoWnd ) return;

	pInfoWnd->HideWnd();

	switch( m_dwCurTabID )
	{
	case ID_SOLDIER_TAB_BTN:
		{
			if( m_bCharItem )
			{
				int iStartArray = m_iCurPage * PAGE_PER_CHAR_NUM;

				if( !m_vCharInfoList.empty() )
				{
					if( (g_MyInfo.GetCharIndexByClassType( m_iSelectClassType ) <= 0) ||
						(FindSelectCharArray( m_iSelectClassType ) == -1) )
					{
						m_iSelectClassType = m_vCharInfoList[0].m_iClassType;
						m_iSelectArray = iStartArray;
					}
					else
					{
						m_iSelectArray = FindSelectCharArray( m_iSelectClassType );
					}

					ShowMainInfoWnd();
					pInfoWnd->SetSlodierInfo( m_iSelectClassType );
				}

				// 해당 용병이 있는 페이지로 갱신
				int iNewPage = m_iSelectArray / PAGE_PER_CHAR_NUM;
				ioScroll *pScroll = (ioScroll*)FindChildWnd( ID_VERT_SCROLL );
				if( pScroll )
				{
					pScroll->SetScrollPos( iNewPage );
				}

				for( int i = 0 ; i < MAX_CHAR_INFO_WND_BTN ; i++ )
				{
					InventorySoldierItemWnd *pItem = dynamic_cast<InventorySoldierItemWnd*>(FindChildWnd(ID_CHAR_INFO_WND_1_BTN + i));
					if( !pItem ) continue;

					int iCurArray = iStartArray + i;
					if( m_iSelectArray == iCurArray )
						pItem->SetSelect( true );
					else
						pItem->SetSelect( false );
				}
			}
			else
			{
				if( !m_vCharInfoList.empty() )
				{
					if( g_MyInfo.GetCharIndexByClassType( m_iSelectClassType ) <= 0 )
						m_iSelectClassType = m_vCharInfoList[0].m_iClassType;

					ShowMainInfoWnd();
					pInfoWnd->SetSlodierInfo( m_iSelectClassType );
				}

				int iStartArray = m_iCurPage * PAGE_PER_ITEM_NUM;
				m_iSelectArray = -1;

				for( int i = 0 ; i < MAX_ITEM_INFO_WND_BTN ; i++ )
				{
					InventoryItemWnd *pItem = dynamic_cast<InventoryItemWnd*>(FindChildWnd(ID_ITEM_INFO_WND_1_BTN + i));
					if( !pItem )
						continue;

					pItem->SetSelect( false );
				}
			}
		}
		break;
	case ID_EXTRA_TAB_BTN:
	case ID_DECO_TAB_BTN:
		// UI코스튬 관련 (첫 아이템 선택)
	case ID_COSTUME_TAB_BTN:
	case ID_ACCESSORY_TAB_BTN:
		{
			ShowMainInfoWnd();

			if( g_MyInfo.GetCharIndexByClassType( m_iSelectClassType ) > 0 )
				pInfoWnd->SetItemBlank( m_dwCurTabID, m_dwCurSmallTabID, m_iSelectClassType );
			else
			{
				if( !m_vCharInfoList.empty() )
				{
					m_iSelectClassType = m_vCharInfoList[0].m_iClassType;
					if( g_MyInfo.GetCharIndexByClassType( m_iSelectClassType ) <= 0 )
						m_iSelectClassType = g_MyInfo.GetClassType();
				}
				
				if( m_iSelectClassType == -1 )
					m_iSelectClassType = g_MyInfo.GetClassType();

				pInfoWnd->SetItemBlank( m_dwCurTabID, m_dwCurSmallTabID, m_iSelectClassType );
			}

			int iStartArray = m_iCurPage * PAGE_PER_ITEM_NUM;
			m_iSelectArray = -1;

			for( int i = 0 ; i < MAX_ITEM_INFO_WND_BTN ; i++ )
			{
				InventoryItemWnd *pItem = dynamic_cast<InventoryItemWnd*>(FindChildWnd(ID_ITEM_INFO_WND_1_BTN + i));
				if( !pItem )
					continue;

				pItem->SetSelect( false );
			}
		}
		break;
	case ID_ETC_TAB_BTN:
	case ID_PRESENT_TAB_BTN:
		{
			ShowMainInfoWnd();
			if( !m_vInventoryItemInfoList.empty() )
			{
				pInfoWnd->SetInvenItemInfo( m_iSelectClassType, m_vInventoryItemInfoList[0] );
			}
			else
			{
				pInfoWnd->SetItemBlank( m_dwCurTabID, m_dwCurSmallTabID, m_iSelectClassType );
			}

			int iStartArray = m_iCurPage * PAGE_PER_ITEM_NUM;
			m_iSelectArray = iStartArray;

			for( int i = 0 ; i < MAX_ITEM_INFO_WND_BTN ; i++ )
			{
				InventoryItemWnd *pItem = dynamic_cast<InventoryItemWnd*>(FindChildWnd(ID_ITEM_INFO_WND_1_BTN + i));
				if( !pItem )
					continue;

				int iCurArray = iStartArray + i;
				if( m_iSelectArray == iCurArray )
				{
					pItem->SetSelect( true );
				}
				else
				{
					pItem->SetSelect( false );
				}
			}
		}
		break;
	}
}

DWORD MyInventoryWnd::GetDefaultSmallTab( DWORD dwMainTab )
{
	int iSmallTab = 0;

	if( dwMainTab == ID_ALCHEMIC_TAB_BTN )
	{
		return ID_SMALL_TAB_1_BTN;
	}

	int iArray = ( dwMainTab - ID_SOLDIER_TAB_BTN );
	if( COMPARE( iArray, 0, MAX_TAB_BTN ) )
	{
		int iSmallSize = m_vSmallTabTypeVecArray[iArray].size();
		for( int i=0; i < iSmallSize; ++i )
		{
			if( m_vSmallTabTypeVecArray[iArray][i] == m_iDefaultTypeArray[iArray] )
				iSmallTab = ID_SMALL_TAB_1_BTN + i;
		}
	}

	return iSmallTab;
}

void MyInventoryWnd::ChangeMainTab( DWORD dwID )
{
	m_dwCurTabID = dwID;
	CheckRadioButton( ID_SOLDIER_TAB_BTN, ID_ACCESSORY_TAB_BTN, dwID );

	int iArray = dwID-ID_SOLDIER_TAB_BTN;
	if( !COMPARE( iArray, 0, MAX_TAB_BTN ) )
	{
		LOG.PrintTimeAndLog( 0, "%s Array is wrong.", __FUNCTION__ );
		return;
	}

	enum
	{
		NOR_WIDTH	= 55,
		NEW_WIDTH	= 94,

		NOR_GAP		= 56,
		NEW_GAP		= 95,

		NOR_TITLE_X	= 27,
		NOR_TITLE_Y	= 4,
		NEW_TITLE_X	= 47,
		NEW_TITLE_Y	= 4,
	};

	int iXPos = 27;
	int iYPos = 91;

	int iCurWidth = NOR_WIDTH;
	int iCurGap = NOR_GAP;
	int iCurTitleX = NOR_TITLE_X;
	int iCurTitleY = NOR_TITLE_Y;

	if( m_dwCurTabID == ID_PRESENT_TAB_BTN )
	{
		iCurWidth = NEW_WIDTH;
		iCurGap = NEW_GAP;

		iCurTitleX = NEW_TITLE_X;
		iCurTitleY = NEW_TITLE_Y;
	}
	int iXOffset = 0;
	int iSize = m_vSmallTabNameVecArray[iArray].size();
	for (int i = 0; i < MAX_SMALL_TAB_BTN; i++)
	{
		SmallTabButton* pButton = dynamic_cast<SmallTabButton*>( FindChildWnd( ID_SMALL_TAB_1_BTN + i ) );
		if( !pButton )
			continue;
		if( COMPARE( i, 0, iSize ) )
		{
			if( i == 0 )
				iXOffset = pButton->GetXPos();

			if( i == iSize-1 )			
				pButton->SetDisable( true );
			else
				pButton->SetDisable( false );

			ShowChildWnd( ID_SMALL_TAB_1_BTN + i );
			SetChildActive( ID_SMALL_TAB_1_BTN  + i );

			iXOffset += UpdateSmallTabName( pButton, iXOffset, m_vSmallTabNameVecArray[iArray][i] );
		}
		else
		{
			HideChildWnd( ID_SMALL_TAB_1_BTN + i );
		}

		/*
		if( COMPARE( i, 0, iSize ) )
		{
			//ShowChildWnd( ID_SMALL_TAB_1_BTN + i);

			pWnd->SetSize( iCurWidth, pWnd->GetHeight() );
			pWnd->SetWndPos( iXPos+(iCurGap*i), iYPos );
			pWnd->ShowWnd();

			ioUITitle *pTitle = pWnd->GetTitle();
			if( pTitle )
				pTitle->SetOffset( iCurTitleX, iCurTitleY );

			pWnd->SetTitleText( m_vSmallTabNameVecArray[iArray][i].c_str() );
		}
		else
		{
			HideChildWnd( ID_SMALL_TAB_1_BTN + i);
		}
		*/
	}

	int iAddID = (iSize-1);
	if( !COMPARE( iAddID, 0, MAX_SMALL_TAB_BTN ) )
		iAddID = 0;
	m_dwSmallTabEndID = ID_SMALL_TAB_1_BTN+iAddID;

	ChangeSmallTab( ID_SMALL_TAB_1_BTN );

	// UI코스튬 관련 (공략 물음표 버튼 설정)
	if ( dwID == ID_COSTUME_TAB_BTN )
	{
		ShowChildWnd( ID_COSTUME_HELP );
		HideChildWnd( ID_COSTUME_HELP_TIP );
	}
	else
	{
		HideChildWnd( ID_COSTUME_HELP );
		HideChildWnd( ID_COSTUME_HELP_TIP );
	}
}

void MyInventoryWnd::ChangeSmallTab( DWORD dwID )
{
	SetAlchemicItemLockClear();

	HideChildWnd( ID_HOUSE_ITEM_BTN ); 
	CheckRadioButton( ID_SMALL_TAB_1_BTN, m_dwSmallTabEndID, dwID );
	m_dwCurSmallTabID = dwID;

	m_bCharItem = false;
	if( m_dwCurTabID == ID_SOLDIER_TAB_BTN )
	{
		SetSoldierItemInfoVec( m_dwCurTabID, m_dwCurSmallTabID );
	}
	else if( m_dwCurTabID == ID_EXTRA_TAB_BTN )
	{
		SetExtraItemInfoVec( m_dwCurTabID, m_dwCurSmallTabID );
	}
	else if( m_dwCurTabID == ID_DECO_TAB_BTN )
	{
		SetDecoItemInfoVec( m_dwCurTabID, m_dwCurSmallTabID );
	}
	else if( m_dwCurTabID == ID_ETC_TAB_BTN )
	{
		SetEtcItemInfoVec( m_dwCurTabID, m_dwCurSmallTabID );
		ShowChildWnd( ID_HOUSE_ITEM_BTN );
	}
	else if( m_dwCurTabID == ID_PRESENT_TAB_BTN )
	{
		SetPresentItemInfoVec( m_dwCurTabID, m_dwCurSmallTabID );
	}
	else if( m_dwCurTabID == ID_ALCHEMIC_TAB_BTN )
	{
		SetAlchemicItemInfoVec( m_dwCurTabID, m_dwCurSmallTabID );
	}
	else if( m_dwCurTabID == ID_COSTUME_TAB_BTN )
	{
		// UI코스튬 관련 (아이템 데이터 추가)
		SetCostumeInfoVec( m_dwCurTabID, m_dwCurSmallTabID );
	}
	else if( m_dwCurTabID == ID_ACCESSORY_TAB_BTN )
	{
		SetAccessoryInfoVec( m_dwCurTabID, m_dwCurSmallTabID );
	}

	m_iCurPage = 0;

	if( m_dwCurTabID == ID_ALCHEMIC_TAB_BTN )
	{
		HideChildWnd( ID_VERT_SCROLL );
		UpdatePage( m_iCurPage );

#if defined( USE_GA )
		// WND_INVEN_CRAFT
		g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FINVEN%2FCRAFT" );
#endif

		return;
	}

	ioScroll *pScroll = (ioScroll*)FindChildWnd( ID_VERT_SCROLL );
	if( pScroll )
	{
		int iCharItemSize = m_vCharInfoList.size();
		int iItemSize = m_vInventoryItemInfoList.size();

		if( m_bCharItem )
		{
			int iMaxPos =  iCharItemSize / PAGE_PER_CHAR_NUM;
			if( iCharItemSize % PAGE_PER_CHAR_NUM )
				iMaxPos++;

			pScroll->SetPageRange(1);
			pScroll->SetScrollRange( 0, iMaxPos );
			pScroll->SetScrollPos( m_iCurPage );
		}
		else
		{
			int iMaxPos =  iItemSize / PAGE_PER_ITEM_NUM;
			if( iItemSize % PAGE_PER_ITEM_NUM )
				iMaxPos++;

			pScroll->SetPageRange(2);
			pScroll->SetScrollRange( 0, iMaxPos );
			pScroll->SetScrollPos( m_iCurPage );
		}

		pScroll->ShowWnd();
	}

#if defined( USE_GA )
	switch( m_dwCurTabID )
	{
	case ID_SOLDIER_TAB_BTN:
		{
			switch( m_dwCurSmallTabID )
			{
			case ID_SMALL_TAB_1_BTN:
				{
					// WND_INVEN_HERO_ALL
					g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FINVEN%2FHERO%2FALL" );
				}
				break;
			case ID_SMALL_TAB_2_BTN:
				{
					// WND_INVEN_HERO_PREMIUM
					g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FINVEN%2FHERO%2FPREMIUM" );
				}
				break;
			case ID_SMALL_TAB_3_BTN:
				{
					// WND_INVEN_HERO_MELEE
					g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FINVEN%2FHERO%2FMELEE" );
				}
				break;
			case ID_SMALL_TAB_4_BTN:
				{
					// WND_INVEN_HERO_RANGED
					g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FINVEN%2FHERO%2FRANGED" );
				}
				break;
			case ID_SMALL_TAB_5_BTN:
				{
					// WND_INVEN_HERO_MAGIC
					g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FINVEN%2FHERO%2FMAGIC" );
				}
				break;
			case ID_SMALL_TAB_6_BTN:
				{
					// WND_INVEN_HERO_SPECIAL
					g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FINVEN%2FHERO%2FSPECIAL" );
				}
				break;
			case ID_SMALL_TAB_7_BTN:
				{
					// WND_INVEN_HERO_PACKAGE
					g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FINVEN%2FHERO%2FPACKAGE" );
				}
				break;
			}
		}
		break;

	case ID_EXTRA_TAB_BTN:
		{
			switch( m_dwCurSmallTabID )
			{
			case ID_SMALL_TAB_1_BTN:
				{
					// WND_INVEN_GEAR_ALL
					g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FINVEN%2FGEAR%2FALL" );
				}
				break;
			case ID_SMALL_TAB_2_BTN:
				{
					// WND_INVEN_GEAR_WEAPON
					g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FINVEN%2FGEAR%2FWEAPON" );
				}
				break;
			case ID_SMALL_TAB_3_BTN:
				{
					// WND_INVEN_GEAR_ARMOR
					g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FINVEN%2FGEAR%2FARMOR" );
				}
				break;
			case ID_SMALL_TAB_4_BTN:
				{
					// WND_INVEN_GEAR_HELMET
					g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FINVEN%2FGEAR%2FHELMET" );
				}
				break;
			case ID_SMALL_TAB_5_BTN:
				{
					// WND_INVEN_GEAR_TRINKET
					g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FINVEN%2FGEAR%2FTRINKET" );
				}
				break;
			case ID_SMALL_TAB_6_BTN:
				{
					// WND_INVEN_GEAR_MEDAL
					g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FINVEN%2FGEAR%2FMEDAL" );
				}
				break;
			case ID_SMALL_TAB_7_BTN:
				{
					// WND_INVEN_GEAR_FEATURES
					g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FINVEN%2FGEAR%2FFEATURES" );
				}
				break;
			}
		}
		break;

	case ID_DECO_TAB_BTN:
		{
			switch( m_dwCurSmallTabID )
			{
			case ID_SMALL_TAB_1_BTN:
				{
					// WND_INVEN_DECO_ALL
					g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FINVEN%2FDECO%2FALL" );
				}
				break;
			case ID_SMALL_TAB_2_BTN:
				{
					// WND_INVEN_DECO_GENDER
					g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FINVEN%2FDECO%2FGENDER" );
				}
				break;
			case ID_SMALL_TAB_3_BTN:
				{
					// WND_INVEN_DECO_HAIR
					g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FINVEN%2FDECO%2FHAIR" );
				}
				break;
			case ID_SMALL_TAB_4_BTN:
				{
					// WND_INVEN_DECO_DYE
					g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FINVEN%2FDECO%2FDYE" );
				}
				break;
			case ID_SMALL_TAB_5_BTN:
				{
					// WND_INVEN_DECO_FACE
					g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FINVEN%2FDECO%2FFACE" );
				}
				break;
			case ID_SMALL_TAB_6_BTN:
				{
					// WND_INVEN_DECO_SKIN
					g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FINVEN%2FDECO%2FSKIN" );
				}
				break;
			case ID_SMALL_TAB_7_BTN:
				{
					// WND_INVEN_DECO_UNDIES
					g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FINVEN%2FDECO%2FUNDIES" );
				}
				break;
			case ID_SMALL_TAB_8_BTN:
				{
					// WND_INVEN_DECO_PACKAGE
					g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FINVEN%2FDECO%2FPACKAGE" );
				}
				break;
			}
		}
		break;	

	case ID_ETC_TAB_BTN:
		{
			switch( m_dwCurSmallTabID )
			{
			case ID_SMALL_TAB_1_BTN:
				{
					// WND_INVEN_SPECIAL_ALL
					g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FINVEN%2FSPECIAL%2FALL" );
				}
				break;
			case ID_SMALL_TAB_2_BTN:
				{
					// WND_INVEN_SPECIAL_BOOST
					g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FINVEN%2FSPECIAL%2FBOOST" );
				}
				break;
			case ID_SMALL_TAB_3_BTN:
				{
					// WND_INVEN_SPECIAL_PREMIUM
					g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FINVEN%2FSPECIAL%2FPREMIUM" );
				}
				break;
			case ID_SMALL_TAB_4_BTN:
				{
					// WND_INVEN_SPECIAL_ETC
					g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FINVEN%2FSPECIAL%2FETC" );
				}
				break;
			}
		}
		break;

	case ID_PRESENT_TAB_BTN:
		{
			switch( m_dwCurSmallTabID )
			{
			case ID_SMALL_TAB_1_BTN:
				{
					// WND_INVEN_STORAGE_GIFT
					g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FINVEN%2FSTORAGE%2FGIFT" );
				}
				break;
			case ID_SMALL_TAB_2_BTN:
				{
					// WND_INVEN_STORAGE_TEMP
					g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FINVEN%2FSTORAGE%2FTEMP" );
				}
				break;
			}
		}
		break;

	case ID_COSTUME_TAB_BTN:
		{
			switch( m_dwCurSmallTabID )
			{
			case ID_SMALL_TAB_1_BTN:
				{
					// WND_INVEN_COSTUME_ALL
					g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FINVEN%2FCOSTUME%2FALL" );
				}
				break;
			case ID_SMALL_TAB_2_BTN:
				{
					// WND_INVEN_COSTUME_ARMOR
					g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FINVEN%2FCOSTUME%2FARMOR" );
				}
				break;
			case ID_SMALL_TAB_3_BTN:
				{
					// WND_INVEN_COSTUME_HELMET
					g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FINVEN%2FCOSTUME%2FHELMET" );
				}
				break;
			case ID_SMALL_TAB_4_BTN:
				{
					// WND_INVEN_COSTUME_TRINKET
					g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FINVEN%2FCOSTUME%2FTRINKET" );
				}
				break;
			}
		}
		break;
	}
#endif

}

void MyInventoryWnd::SetSoldierItemInfoVec( DWORD dwTabID, DWORD dwSmallTabID )
{
	m_vCharInfoList.clear();
	m_vInventoryItemInfoList.clear();
	m_vAlchemicItemInfoList.clear();

	if( dwTabID != ID_SOLDIER_TAB_BTN )
	{
		return;
	}

	DWORD dwCheckSmallTabID = dwSmallTabID;

	m_bCharItem = true;
	if( IsRightSmallTab(dwTabID, dwSmallTabID, ETC_ITEM_OTHER_VALUE) )
	{
		m_bCharItem = false;

		// Etc Soldier 추가
		AddItemInfoVecFromEtcItem( dwTabID, dwSmallTabID, SHOP_TAB_TYPE_SOLDIER );

		if( !m_vInventoryItemInfoList.empty() )
		{
			std::sort( m_vInventoryItemInfoList.begin(), m_vInventoryItemInfoList.end(), InventoryItemInfoSort() );
		}

		dwCheckSmallTabID = ID_SMALL_TAB_1_BTN;
	}

	// Soldier
	int iMax = g_MyInfo.GetCharCount();
	for ( int i=0; i < iMax; i++ )
	{
		int iClassType = g_MyInfo.GetClassType( i );
		int iSetIdx = iClassType - 1;
		const ioSetItemInfo *pInfo = g_SetItemInfoMgr.GetSetInfoByIdx( iSetIdx );
		if( !pInfo )
			continue;

		if( !IsRightSmallTab( dwTabID, dwCheckSmallTabID, pInfo->GetCharAttackType() ) )
			continue;

		SoldierItemInfo kInfo;
		kInfo.m_iClassType = iClassType;
		kInfo.m_iPriorityOrder = g_MyInfo.GetCharArrayToSlotIndex( i );

		m_vCharInfoList.push_back( kInfo );
	}

	// 보유중이지 않은 용병...
	int iMaxClass = g_ClassPrice.MaxClassPrice();
	for ( int i=0; i < iMaxClass; i++ )
	{
		int iSetIdx = g_ClassPrice.ArrayToSetItemIdx( i );
		const ioSetItemInfo *pInfo = g_SetItemInfoMgr.GetSetInfoByIdx( iSetIdx );
		if( !pInfo ) continue;

		if( !IsRightSmallTab( dwTabID, dwCheckSmallTabID, pInfo->GetCharAttackType() ) )
			continue;

		int iClassType = iSetIdx + 1;
		if( g_MyInfo.GetCharIndexByClassType( iClassType ) > 0 )		// 보유중인 용병은 패스
			continue;

		if( !g_MyInfo.IsClassTypeInventory( iClassType ) )		// 한 번도 고용된적 없으면 패스
			continue;

		SoldierItemInfo kInfo;
		kInfo.m_iClassType = iClassType;
		kInfo.m_iPriorityOrder = 1000 + iClassType;

		m_vCharInfoList.push_back( kInfo );
	}

	if( m_vCharInfoList.empty() )
		return;

	std::sort( m_vCharInfoList.begin(), m_vCharInfoList.end(), SoldierItemInfoSort() );
}

void MyInventoryWnd::SetExtraItemInfoVec( DWORD dwTabID, DWORD dwSmallTabID )
{
	//m_vCharInfoList.clear();
	m_vInventoryItemInfoList.clear();
	m_vAlchemicItemInfoList.clear();

	ioUserExtraItem *pItem = g_MyInfo.GetUserExtraItem();
	if( !pItem ) return;

	CHARACTER rkCharInfo;
	int iCharArray = g_MyInfo.GetClassArray( m_iSelectClassType );
	if( COMPARE( iCharArray, 0, g_MyInfo.GetCharCount() ) )
		rkCharInfo = g_MyInfo.GetCharacter( iCharArray );
	else
		NewShopBuyWnd::GetDefaultCharInfo( rkCharInfo, m_iSelectClassType );

	int iSize = pItem->GetExtraItemCnt();
	for( int i=0; i < iSize; ++i )
	{
		EXTRAITEMSLOT kSlot;
		if( !pItem->GetExtraItemArray( i, kSlot ) )
			continue;

		int iGroup = kSlot.m_iItemCode/DEFAULT_BASIC_ITEM_CODE + 1;
		if( !IsRightSmallTab( dwTabID, dwSmallTabID, iGroup ) )
			continue;

		if( kSlot.m_bCharEquip )
		{
			continue;
			/*
			if( rkCharInfo.m_extra_item[0] != kSlot.m_iIndex && rkCharInfo.m_extra_item[1] != kSlot.m_iIndex &&
				rkCharInfo.m_extra_item[2] != kSlot.m_iIndex && rkCharInfo.m_extra_item[3] != kSlot.m_iIndex )
			{
				continue;
			}
			*/
		}

		InventoryItemInfo kInfo;
		kInfo.m_iMainType = INVENTORY_ITEM_EXTRAITEM;
		kInfo.m_iMagicCode = kSlot.m_iIndex;
		kInfo.m_bEquip = kSlot.m_bCharEquip;

		int iSlot = kSlot.m_iItemCode/DEFAULT_BASIC_ITEM_CODE;
		int iValue = kSlot.m_iReinforce;

		if( !kSlot.m_bCharEquip )
			iValue += FLOAT100;

		kInfo.m_iPriorityOrder = iSlot * 10000 + iValue;
		kInfo.m_iPriorityOrder2 = kSlot.m_iItemCode;

		m_vInventoryItemInfoList.push_back( kInfo );
	}

	// Etc ExtraItem 추가
	AddItemInfoVecFromEtcItem( dwTabID, dwSmallTabID, SHOP_TAB_TYPE_EXTRA );
	//

	// medalitem extraitem 추가
	AddItemInfoVecFromMedalItem( dwTabID, dwSmallTabID );
	//

	if( m_vInventoryItemInfoList.empty() )
		return;

	// 각 하위텝 마다 다르게 정렬한다.
	if( dwSmallTabID == ID_SMALL_TAB_1_BTN )	// 전체
	{
		// m_iMagicCode 역순으로 정렬
		std::sort( m_vInventoryItemInfoList.begin(), m_vInventoryItemInfoList.end(), InventoryItemInfoSort2() );
	}
	else
	{
		std::sort( m_vInventoryItemInfoList.begin(), m_vInventoryItemInfoList.end(), InventoryItemInfoSort3() );
	}
}

void MyInventoryWnd::SetDecoItemInfoVec( DWORD dwTabID, DWORD dwSmallTabID )
{
	m_vCharInfoList.clear();
	m_vInventoryItemInfoList.clear();
	m_vAlchemicItemInfoList.clear();

	CHARACTER kCurCharInfo;

	if( g_MyInfo.GetCharIndexByClassType( m_iSelectClassType ) <= 0 )
		m_iSelectClassType = g_MyInfo.GetClassType();

	int iCharArray = g_MyInfo.GetClassArray( m_iSelectClassType );
	if( iCharArray == - 1 )
		kCurCharInfo = g_MyInfo.GetCharacter();
	else
		kCurCharInfo = g_MyInfo.GetCharacter( iCharArray );

	int iCurSexType = kCurCharInfo.m_sex - 1;

	int i = 0;
	int iSexSize = g_DecorationPrice.GetSexSize();
	for (i = 0; i < iSexSize ; i++)
	{
		InventoryItemInfo kInfo;

		kInfo.m_iMainType = INVENTORY_ITEM_DECO;
		kInfo.m_iSexType = g_DecorationPrice.GetSexType( i );
		if( kInfo.m_iSexType != iCurSexType )
			continue;

		int iDecoTypeSize = g_DecorationPrice.GetDecoTypeSize( i );
		for (int j = 0; j < iDecoTypeSize; j++)
		{
			kInfo.m_iDecoType = g_DecorationPrice.GetDecoType( i , j );

			if( !IsRightSmallTab( dwTabID, dwSmallTabID, kInfo.m_iDecoType ) )
				continue;

			IntVec vBoughtList;
			int iDecoSize = g_DecorationPrice.GetDecoSize( i, j );
			for (int z = 0; z < iDecoSize; z++)
			{
				int iDecoCode = g_DecorationPrice.GetDecoCode( i, j, z );

				if( IsBoughtDeco( kCurCharInfo.m_class_type, kInfo.m_iSexType, kInfo.m_iDecoType, iDecoCode ) )
				{
					vBoughtList.push_back( iDecoCode );
				}
			}

			int iBoughtCnt = vBoughtList.size();
			for( int k=0; k < iBoughtCnt; ++k )
			{
				kInfo.m_iDecoCode = vBoughtList[k];
				kInfo.m_bEquip = IsEquipDeco( kCurCharInfo, kInfo.m_iDecoType, kInfo.m_iDecoCode );

				if( kInfo.m_bEquip )
					continue;

				kInfo.m_iPriorityOrder = j * 10000;

				kInfo.m_bActive = false;
				if( iBoughtCnt > 1 )
					kInfo.m_bActive = true;

				kInfo.m_iPriorityOrder += 1000;
				kInfo.m_iPriorityOrder += k;

				m_vInventoryItemInfoList.push_back( kInfo );
			}
		}
	}

	// Etc Deco 추가
	AddItemInfoVecFromEtcItem( dwTabID, dwSmallTabID, SHOP_TAB_TYPE_DECO );
	//

	if( m_vInventoryItemInfoList.empty() )
		return;

	std::sort( m_vInventoryItemInfoList.begin(), m_vInventoryItemInfoList.end(), EtcItemPrioritySort() );
}

void MyInventoryWnd::SetEtcItemInfoVec( DWORD dwTabID, DWORD dwSmallTabID )
{
	m_vCharInfoList.clear();
	m_vInventoryItemInfoList.clear();
	m_vAlchemicItemInfoList.clear();

	// etc
	ioUserEtcItem *pUserEtcItem = g_MyInfo.GetUserEtcItem();
	if( !pUserEtcItem )
	{
		LOG.PrintTimeAndLog( 0, "%s pUserEtcItem == NULL", __FUNCTION__ );
		return;
	}

	int iMax = g_EtcItemMgr.GetEtcItemCount();
	for (int i = 0; i < iMax ; i++)
	{
		ioEtcItem *pEtcItem = g_EtcItemMgr.GetEtcItemByIdx( i );
		if( !pEtcItem ) continue;

		ioUserEtcItem::ETCITEMSLOT kSlot;
		if( pUserEtcItem->GetEtcItem( pEtcItem->GetType(), kSlot ))
		{
			bool bAdd = false;
			int iSize = pEtcItem->GetValueSize();
			for (int j = 0; j < iSize ; j++)
			{
				int iShopTabType = pEtcItem->GetShopTabType(j);
				if( iShopTabType != SHOP_TAB_TYPE_NONE && iShopTabType != SHOP_TAB_TYPE_ETC && iShopTabType != SHOP_TAB_TYPE_LIMIT )
					continue;

				if( !IsRightSmallTab( dwTabID, dwSmallTabID, pEtcItem->GetGroup() ) )
					continue;

				bAdd = true;
			}

			if( bAdd )
			{
				InventoryItemInfo kInfo;
				kInfo.m_iMainType = INVENTORY_ITEM_ETC;
				kInfo.m_iMagicCode = pEtcItem->GetType();
				kInfo.m_iManualIndex = pEtcItem->GetInventoryManual()-1;
				kInfo.m_iPriorityOrder = pEtcItem->GetInventoryOrder();

				m_vInventoryItemInfoList.push_back( kInfo );
			}
		}
	}

	if( m_vInventoryItemInfoList.empty() )
		return;

	std::sort( m_vInventoryItemInfoList.begin(), m_vInventoryItemInfoList.end(), InventoryItemInfoSort() );
}

void MyInventoryWnd::SetPresentItemInfoVec( DWORD dwTabID, DWORD dwSmallTabID )
{
	m_vCharInfoList.clear();
	m_vInventoryItemInfoList.clear();
	m_vAlchemicItemInfoList.clear();

	// HARDCODE:: 타입값을 xml 파일에서 바꿀경우 함께 바꿔줘야함
	int iSmallTabType = GetSmallTabType( dwTabID, dwSmallTabID );
	if( iSmallTabType == 0 )			// 선물
	{
		int iSize = g_PresentMgr.GetMaxPresent();
		for( int i=0; i < iSize; ++i )
		{
			DWORD dwSlotIndex = 0;
			DWORD dwIndex = g_PresentMgr.GetPresentIndexToArray( i, dwSlotIndex );

			ioHashString szSendID;
			short sPresentType, sPresentMent;
			int iPresentValue1, iPresentValue2, iPresentValue3, iPresentValue4;
			DWORD dwLimitDate;

			g_PresentMgr.GetPresentDataToIndex( dwIndex, dwSlotIndex, szSendID, sPresentType, sPresentMent,	iPresentValue1,
												iPresentValue2,	iPresentValue3,	iPresentValue4,	dwLimitDate );

			InventoryItemInfo kInfo;
			kInfo.m_iMainType = INVENTORY_ITEM_PRESENT;
			kInfo.m_iMagicCode = dwIndex;
			kInfo.m_iMagicCodeEx = dwSlotIndex;
			kInfo.m_iManualIndex = sPresentMent;
			kInfo.m_iPriorityOrder = i;

			m_vInventoryItemInfoList.push_back( kInfo );
		}
	}
	else if( iSmallTabType == ETC_ITEM_OTHER_VALUE )		// 임시보관함
	{
		int iSize = g_SubscriptionMgr.GetMaxSubscription();
		for( int i=0; i < iSize; ++i )
		{
			ioSubscriptionMgr::SubscriptionData kData;

			if( g_SubscriptionMgr.GetSubscriptionDataToArray( i, kData ) )
			{
				InventoryItemInfo kInfo;
				kInfo.m_iMainType = INVENTORY_ITEM_SUBSCRIPTION;
				kInfo.m_iMagicCode = kData.m_dwIndex;
				kInfo.m_szSubscriptionID = kData.m_szSubscriptionID;
				kInfo.m_iSubscriptionGold = kData.m_iSubscriptionGold;
				kInfo.m_iSubscriptionBonusGold = kData.m_iSubscriptionBonusGold;
				kInfo.m_iManualIndex = kData.m_iMentType;
				kInfo.m_iPriorityOrder = i;

				m_vInventoryItemInfoList.push_back( kInfo );
			}
		}
	}

	if( m_vInventoryItemInfoList.empty() )
		return;

	std::sort( m_vInventoryItemInfoList.begin(), m_vInventoryItemInfoList.end(), InventoryItemInfoSort4() );
}

// UI코스튬 (아이템 데이터 얻기)
void MyInventoryWnd::SetCostumeInfoVec( DWORD dwTabID, DWORD dwSmallTabID )
{
	m_vInventoryItemInfoList.clear();
	m_vAlchemicItemInfoList.clear();

	ioUserCostume *pCostume = g_MyInfo.GetUserCostume();
	if( !pCostume ) 
		return;
	
	int nSize = pCostume->GetCostumeSlotCnt();
	for( int i=0; i<nSize; ++i )
	{
		CostumeSlot kSlot;
		if( !pCostume->GetCostumeSlotArray( i, kSlot ) )
			continue;

		// 스몰탭 인덱스랑 같아야함
		int nGroup = (kSlot.m_nCostumeCode/DEFAULT_BASIC_COSTUME_CODE) + 1;
		if( !IsRightSmallTab( dwTabID, dwSmallTabID, nGroup ) )
			continue;

		if( kSlot.m_bEquip )
			continue;

		InventoryItemInfo kInfo;
		kInfo.m_iMainType = INVENTORY_ITEM_COSTUME;
		kInfo.m_iMagicCode = kSlot.m_nIndex;
		kInfo.m_bEquip = kSlot.m_bEquip;

		int nSlot = kSlot.m_nCostumeCode/DEFAULT_BASIC_COSTUME_CODE;
		int nValue = kSlot.m_iValue1;

		if( !kSlot.m_bEquip )
			nValue += 100;

		kInfo.m_iPriorityOrder = nSlot * 10000 + nValue;
		kInfo.m_iPriorityOrder2 = kSlot.m_nCostumeCode;

		m_vInventoryItemInfoList.push_back( kInfo );
	}

	// Etc Costume 추가
	AddItemInfoVecFromEtcItem( dwTabID, dwSmallTabID, SHOP_TAB_TYPE_COSTUME );
	
	if( m_vInventoryItemInfoList.empty() )
		return;

	// 각 하위텝 마다 다르게 정렬한다.
	if( dwSmallTabID == ID_SMALL_TAB_1_BTN )	// 전체
	{
		// m_iMagicCode 역순으로 정렬
		std::sort( m_vInventoryItemInfoList.begin(), m_vInventoryItemInfoList.end(), InventoryItemInfoSort2() );
	}
	else
		std::sort( m_vInventoryItemInfoList.begin(), m_vInventoryItemInfoList.end(), InventoryItemInfoSort3() );
}

void MyInventoryWnd::SetAlchemicItemInfoVec( DWORD dwTabID, DWORD dwSmallTabID )
{
	m_vCharInfoList.clear();
	m_vInventoryItemInfoList.clear();
	m_vAlchemicItemInfoList.clear();

	ioAlchemicInventory *pAlchemicInven = g_MyInfo.GetAlchemicInventory();
	if( !pAlchemicInven )
		return;

	pAlchemicInven->GetAlchemicItemInfo( m_vAlchemicItemInfoList );

	std::sort( m_vAlchemicItemInfoList.begin(), m_vAlchemicItemInfoList.end(), AlchemicItemSort() );
}

void MyInventoryWnd::SetAlchemicItemLockStat( int iCode, bool bLock )
{
	if( !IsShow() )
		return;

	if( m_dwCurTabID != ID_ALCHEMIC_TAB_BTN )
		return;

	for( int i=0; i < MAX_ALCHEMIC_INFO_BTN; ++i )
	{
		AlchemicItemWnd *pItem = dynamic_cast<AlchemicItemWnd*>(FindChildWnd(ID_ALCHEMIC_INFO_WND_1_BTN + i));
		if( !pItem )
			continue;

		if( pItem->GetItemCode() == iCode )
		{
			pItem->SetLockState( bLock );
			return;
		}
	}
}

void MyInventoryWnd::SetAlchemicItemLockAll()
{
	if( !IsShow() )
		return;

	if( m_dwCurTabID != ID_ALCHEMIC_TAB_BTN )
		return;

	for( int i=0; i < MAX_ALCHEMIC_INFO_BTN; ++i )
	{
		AlchemicItemWnd *pItem = dynamic_cast<AlchemicItemWnd*>(FindChildWnd(ID_ALCHEMIC_INFO_WND_1_BTN + i));
		if( !pItem )
			continue;

		if( !pItem->IsLockState() )
			pItem->SetLockState( true );
	}
}

void MyInventoryWnd::SetAlchemicItemLockAllAdditive()
{
	if( !IsShow() )
		return;

	if( m_dwCurTabID != ID_ALCHEMIC_TAB_BTN )
		return;

	for( int i=0; i < MAX_ALCHEMIC_INFO_BTN; ++i )
	{
		AlchemicItemWnd *pItem = dynamic_cast<AlchemicItemWnd*>(FindChildWnd(ID_ALCHEMIC_INFO_WND_1_BTN + i));
		if( !pItem )
			continue;

		if( !pItem->IsLockState() && pItem->IsAdditive() )
			pItem->SetLockState( true );
	}
}

void MyInventoryWnd::SetAlchemicItemLockClear()
{
	if( !IsShow() )
		return;

	if( m_dwCurTabID != ID_ALCHEMIC_TAB_BTN )
		return;

	for( int i=0; i < MAX_ALCHEMIC_INFO_BTN; ++i )
	{
		AlchemicItemWnd *pItem = dynamic_cast<AlchemicItemWnd*>(FindChildWnd(ID_ALCHEMIC_INFO_WND_1_BTN + i));
		if( !pItem )
			continue;

		if( pItem->IsLockState() )
			pItem->SetLockState( false );
	}
}

void MyInventoryWnd::AddItemInfoVecFromPresent( DWORD dwTabID, int iValue1, int iValue2 )
{
	int iSize = g_PresentMgr.GetMaxPresent();
	for( int i=0; i < iSize; ++i )
	{
		DWORD dwSlotIndex = 0;
		DWORD dwIndex = g_PresentMgr.GetPresentIndexToArray( i, dwSlotIndex );

		ioHashString szSendID;
		short sPresentType, sPresentMent;
		int iPresentValue1, iPresentValue2, iPresentValue3, iPresentValue4;
		DWORD dwLimitDate;

		g_PresentMgr.GetPresentDataToIndex( dwIndex, dwSlotIndex, szSendID, sPresentType, sPresentMent,	iPresentValue1, iPresentValue2,	iPresentValue3,	iPresentValue4,	dwLimitDate );
		if( iValue1 == iPresentValue1 && iValue2 == iPresentValue2 )
		{
			InventoryItemInfo kInfo;
			kInfo.m_iMainType = INVENTORY_ITEM_PRESENT;
			kInfo.m_iMagicCode = dwIndex;
			kInfo.m_iMagicCodeEx = dwSlotIndex;
			kInfo.m_iManualIndex = sPresentMent;
			kInfo.m_iPriorityOrder = i;
			m_vInventoryItemInfoList.push_back( kInfo );
		}
	}

	if( m_vInventoryItemInfoList.empty() )
		return;

	if( !IsRightSmallTab( dwTabID, 0, 0 ) ) 
		return;

	std::sort( m_vInventoryItemInfoList.begin(), m_vInventoryItemInfoList.end(), InventoryItemInfoSort4() );
	UpdatePage( m_iCurPage );
}

void MyInventoryWnd::AddItemInfoVecFromEtcItem( DWORD dwTabID, DWORD dwSmallTabID, int iShopTabType )
{
	if( !IsRightSmallTab( dwTabID, dwSmallTabID, ETC_ITEM_OTHER_VALUE ) ) 
		return;

	ioUserEtcItem *pUserEtcItem = g_MyInfo.GetUserEtcItem();
	if( !pUserEtcItem )
	{
		LOG.PrintTimeAndLog( 0, "%s pUserEtcItem == NULL", __FUNCTION__ );
		return;
	}

	int iMax = g_EtcItemMgr.GetEtcItemCount();
	for (int i = 0; i < iMax ; i++)
	{
		ioEtcItem *pEtcItem = g_EtcItemMgr.GetEtcItemByIdx( i );
		if( !pEtcItem ) continue;


		ioUserEtcItem::ETCITEMSLOT kSlot;
		if( pUserEtcItem->GetEtcItem( pEtcItem->GetType(), kSlot ))
		{
			bool bAdd = false;
			int iSize = pEtcItem->GetValueSize();
			for (int j = 0; j < iSize ; j++)
			{
				if( pEtcItem->GetShopTabType( j ) != iShopTabType )
					continue;

				bAdd = true;
			}

			if( bAdd )
			{
				InventoryItemInfo kInfo;
				kInfo.m_iMainType = INVENTORY_ITEM_ETC;
				kInfo.m_iMagicCode = pEtcItem->GetType();
				kInfo.m_iManualIndex = pEtcItem->GetInventoryManual()-1;
				kInfo.m_iPriorityOrder = pEtcItem->GetInventoryOrder();

				m_vInventoryItemInfoList.push_back( kInfo );
			}
		}
	}
}

void MyInventoryWnd::AddItemInfoVecFromMedalItem( DWORD dwTabID, DWORD dwSmallTabID )
{
	if( !IsRightSmallTab( dwTabID, dwSmallTabID, MEDALITEM_SMALL_TAB_TYPE ) ) 
		return;

	ioUserMedalItem *pUserMedalItem = g_MyInfo.GetUserMedalItem();
	if( !pUserMedalItem )
	{
		LOG.PrintTimeAndLog( 0, "%s pUserMedalItem == NULL", __FUNCTION__ );
		return;
	}

	int iMax = pUserMedalItem->GetMedalItemCnt();
	for (int i = 0; i < iMax ; i++)
	{
		ioUserMedalItem::MEDALITEMSLOT kSlot;
		pUserMedalItem->GetMedalItemArray( i , kSlot );
		if( kSlot.m_iItemType == 0 )
			continue;

		InventoryItemInfo kInfo;
		kInfo.m_iMainType      = INVENTORY_ITEM_MEDALITEM;
		kInfo.m_iMagicCode     = kSlot.m_iItemType;
		kInfo.m_iPriorityOrder = kSlot.m_iItemType;
		if( kSlot.m_iEquipClass != 0 )
		{
			continue;
			/*
			if( kSlot.m_iEquipClass != m_iSelectClassType )
				continue;

			kInfo.m_bEquip     = true;
			*/
		}
		else
			kInfo.m_bEquip     = false;

		m_vInventoryItemInfoList.push_back( kInfo );
	}
}

bool MyInventoryWnd::IsBoughtDeco( int iClassType, int iSexType, int iDecoType, int iDecoCode )
{
	int iCharArray = g_MyInfo.GetClassArray( iClassType );

	// 체험,미고용용병이면 남자종족은 소유하지 않은 것
	if( g_MyInfo.IsCharExercise( iCharArray ) || !COMPARE( iCharArray, 0, g_MyInfo.GetCharCount() ) ) 
	{
		if( iDecoType == UID_KINDRED && iDecoCode == RDT_HUMAN_MAN )
			return false;
	}

	//고유 특성 1111 22 333 ( 세트, 종족(성별), 치장타입 )

	int iType = ( iClassType * 100000) + ( iSexType * 1000);
	int iCode = 0;
	switch( iDecoType )
	{
	case UID_FACE:
	case UID_HAIR:
	case UID_SKIN_COLOR:
	case UID_HAIR_COLOR:
	case UID_UNDERWEAR:
	case UID_KINDRED:
		{
			iType += iDecoType;
			iCode =  iDecoCode;
		}
		break;
	case UID_CLASS:
		{
			int iEquipType = iDecoCode / 1000;
			int iEquipCode = iDecoCode % 1000;
			iType += iEquipType;
			iCode  = iEquipCode;
		}
		break;			
	}
	return g_MyInfo.IsSlotItem( iType, iCode );
}

bool MyInventoryWnd::IsEquipDeco( const CHARACTER &rkCharInfo, int iDecoType, int iDecoCode )
{
	switch( iDecoType )
	{
	case UID_FACE:
		{
			if( rkCharInfo.m_face == iDecoCode )
				return true;
		}
		break;
	case UID_HAIR:
		{
			if( rkCharInfo.m_hair == iDecoCode )
				return true;
		}
		break;
	case UID_SKIN_COLOR:
		{
			if( rkCharInfo.m_skin_color == iDecoCode )
				return true;
		}
		break;
	case UID_HAIR_COLOR:
		{
			if( rkCharInfo.m_hair_color == iDecoCode )
				return true;
		}
		break;
	case UID_KINDRED:
		{
			if( (int)GetRaceDetailType( rkCharInfo ) == iDecoCode )
				return true;
		}
		break;
	case UID_UNDERWEAR:
		{
			if( rkCharInfo.m_underwear == iDecoCode )
				return true;
		}
		break;
	}

	return false;
}

void MyInventoryWnd::SelectItemBtn( DWORD dwID, DWORD dwParam )
{
	int iStartArray = m_iCurPage * PAGE_PER_ITEM_NUM;
	m_iSelectArray = iStartArray + (dwID-ID_ITEM_INFO_WND_1_BTN);

	for( int i=0; i < MAX_ITEM_INFO_WND_BTN; i++ )
	{
		InventoryItemWnd *pItem = dynamic_cast<InventoryItemWnd*>(FindChildWnd(ID_ITEM_INFO_WND_1_BTN + i));
		if( !pItem ) continue;

		int iCurArray = iStartArray + i;

		if( m_iSelectArray == iCurArray )
			pItem->SetSelect( true );
		else
			pItem->SetSelect( false );
	}
}

void MyInventoryWnd::SelectCharItemBtn( DWORD dwID, DWORD dwParam )
{
	int iStartArray = m_iCurPage * PAGE_PER_CHAR_NUM;
	m_iSelectArray = iStartArray + (dwID-ID_CHAR_INFO_WND_1_BTN);

	for( int i = 0 ; i < MAX_CHAR_INFO_WND_BTN ; i++ )
	{
		InventorySoldierItemWnd *pItem = dynamic_cast<InventorySoldierItemWnd*>(FindChildWnd(ID_CHAR_INFO_WND_1_BTN + i));
		if( !pItem ) continue;

		int iCurArray = iStartArray + i;
		if( m_iSelectArray == iCurArray )
			pItem->SetSelect( true );
		else
			pItem->SetSelect( false );
	}

	InventorySoldierItemWnd *pItemBtn = dynamic_cast<InventorySoldierItemWnd*>(FindChildWnd(dwID));
	if( pItemBtn )
	{
		m_iSelectClassType = pItemBtn->GetClassType();

		MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
		if( pInfoWnd )
		{
			ShowMainInfoWnd();
			pInfoWnd->SetSlodierInfo( m_iSelectClassType );
		}
	}
}

void MyInventoryWnd::UpdateTab( bool bPage, bool bInfo, bool bSelectChange )
{
	CheckRadioButton( ID_SMALL_TAB_1_BTN, m_dwSmallTabEndID, m_dwCurSmallTabID );
	
	// UI코스튬 관련 (탭 업데이트)
	m_bCharItem = false;
	if( m_dwCurTabID == ID_SOLDIER_TAB_BTN )
		SetSoldierItemInfoVec( m_dwCurTabID, m_dwCurSmallTabID );
	else if( m_dwCurTabID == ID_EXTRA_TAB_BTN )
		SetExtraItemInfoVec( m_dwCurTabID, m_dwCurSmallTabID );
	else if( m_dwCurTabID == ID_DECO_TAB_BTN )
		SetDecoItemInfoVec( m_dwCurTabID, m_dwCurSmallTabID );
	else if( m_dwCurTabID == ID_ETC_TAB_BTN )
		SetEtcItemInfoVec( m_dwCurTabID, m_dwCurSmallTabID );
	else if( m_dwCurTabID == ID_PRESENT_TAB_BTN )
		SetPresentItemInfoVec( m_dwCurTabID, m_dwCurSmallTabID );
	else if( m_dwCurTabID == ID_ALCHEMIC_TAB_BTN )
		SetAlchemicItemInfoVec( m_dwCurTabID, m_dwCurSmallTabID );
	else if( m_dwCurTabID == ID_COSTUME_TAB_BTN )
		SetCostumeInfoVec( m_dwCurTabID, m_dwCurSmallTabID );
	else if( m_dwCurTabID == ID_ACCESSORY_TAB_BTN )
		SetAccessoryInfoVec( m_dwCurTabID, m_dwCurSmallTabID );

	// 페이지 갱신 여부
	if( bPage )
	{
		m_iCurPage = 0;
	}

	ioScroll *pScroll = (ioScroll*)FindChildWnd( ID_VERT_SCROLL );
	if( pScroll && (m_dwCurTabID != ID_ALCHEMIC_TAB_BTN) )
	{
		int iMinPos = pScroll->GetMinPos();
		int iMaxPos = pScroll->GetMaxPos();
		m_iCurPage = max( iMinPos, min( m_iCurPage, iMaxPos ) );

		int iCharItemSize = m_vCharInfoList.size();
		int iItemSize = m_vInventoryItemInfoList.size();

		if( m_bCharItem )
		{
			int iMaxPos =  iCharItemSize / PAGE_PER_CHAR_NUM;
			if( iCharItemSize % PAGE_PER_CHAR_NUM )
				iMaxPos++;

			pScroll->SetPageRange(1);
			pScroll->SetScrollRange( 0, iMaxPos );
			pScroll->SetScrollPos( m_iCurPage );
		}
		else
		{
			int iMaxPos =  iItemSize / PAGE_PER_ITEM_NUM;
			if( iItemSize % PAGE_PER_ITEM_NUM )
				iMaxPos++;

			pScroll->SetPageRange(2);
			pScroll->SetScrollRange( 0, iMaxPos );
			pScroll->SetScrollPos( m_iCurPage );
		}
	}

	UpdatePage( m_iCurPage );

	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( !pInfoWnd ) return;

	if( m_dwCurTabID == ID_ALCHEMIC_TAB_BTN )
	{
		if( bInfo )
		{
			ShowMainInfoWnd();
			ShowAlchemicFuncWnd();
		}
	}
	else if( m_bCharItem )
	{
		int iStartArray = m_iCurPage * PAGE_PER_CHAR_NUM;
		if( bPage )
		{
			m_iSelectArray = iStartArray;
		}

		if( !COMPARE( m_iSelectArray, 0, (int)m_vCharInfoList.size() ) )
		{
			m_iSelectArray = iStartArray;
		}

		for( int i = 0 ; i < MAX_CHAR_INFO_WND_BTN ; i++ )
		{
			InventorySoldierItemWnd *pItem = dynamic_cast<InventorySoldierItemWnd*>(FindChildWnd(ID_CHAR_INFO_WND_1_BTN + i));
			if( !pItem ) continue;

			pItem->SetSelect( false );
		}

		if( bInfo )
		{
			ShowMainInfoWnd();
			pInfoWnd->SetSlodierInfo( m_iSelectClassType );
		}
	}
	else
	{
		// UI코스튬 관련 (아이템 선택)
		switch( m_dwCurTabID )
		{
		case ID_DECO_TAB_BTN:
		case ID_EXTRA_TAB_BTN:
		case ID_ETC_TAB_BTN:
		case ID_PRESENT_TAB_BTN:
		case ID_COSTUME_TAB_BTN:
		case ID_ACCESSORY_TAB_BTN:
			{
				int iStartArray = m_iCurPage * PAGE_PER_ITEM_NUM;
				DWORD dwSelectID = ID_ITEM_INFO_WND_1_BTN;

				if( !bSelectChange )
				{
					if( bPage )
					{
						m_iSelectArray = iStartArray;
					}

					if( !COMPARE( m_iSelectArray, 0, (int)m_vInventoryItemInfoList.size() ) )
					{
						m_iSelectArray = iStartArray;
					}

					for( int i = 0 ; i < MAX_ITEM_INFO_WND_BTN ; i++ )
					{
						InventoryItemWnd *pItem = dynamic_cast<InventoryItemWnd*>(FindChildWnd(ID_ITEM_INFO_WND_1_BTN + i));
						if( !pItem )
							continue;

						int iCurArray = iStartArray + i;

						if( m_iSelectArray == iCurArray )
						{
							pItem->SetSelect( true );
							dwSelectID = ID_ITEM_INFO_WND_1_BTN + i;
						}
						else
						{
							pItem->SetSelect( false );
						}
					}
				}
				else
				{
					m_iSelectArray = -1;

					for( int i = 0 ; i < MAX_ITEM_INFO_WND_BTN ; i++ )
					{
						InventoryItemWnd *pItem = dynamic_cast<InventoryItemWnd*>(FindChildWnd(ID_ITEM_INFO_WND_1_BTN + i));
						if( !pItem )
							continue;

						int iCurArray = iStartArray + i;
						pItem->SetSelect( false );
					}
				}

				if( bInfo )
				{
					ShowMainInfoWnd();
					if( m_iSelectArray != -1 && !m_vInventoryItemInfoList.empty() )
					{
						InventoryItemWnd *pSelectItem = dynamic_cast<InventoryItemWnd*>(FindChildWnd(dwSelectID));
						if( pSelectItem )
						{
							pInfoWnd->SetInvenItemInfo( m_iSelectClassType, pSelectItem->GetInfo() );							
						}
					}
					else
					{
						pInfoWnd->SetItemBlank( m_dwCurTabID, m_dwCurSmallTabID, m_iSelectClassType );
					}
				}
			}
			break;
		}
	}
}

void MyInventoryWnd::UpdateData()
{
	ChangeMainTab( m_dwCurTabID );
	SetFirstBtnInfo();
}

void MyInventoryWnd::ChangeClassType( int iClassType )
{
	if( !IsShow() )
		return;

	m_iSelectClassType = iClassType;

	switch( m_dwCurTabID )
	{
	case ID_SOLDIER_TAB_BTN:
		ShowSoldierTabDirect( iClassType );
		break;
	case ID_DECO_TAB_BTN:
	case ID_EXTRA_TAB_BTN:
		// UI코스튬 관련 (캐릭터 변경시)
	case ID_COSTUME_TAB_BTN:
	case ID_ACCESSORY_TAB_BTN:
		UpdateTab( true, true );
		break;
	}
}

int MyInventoryWnd::FindSelectCharArray( int iSelectClassType )
{
	// 해당 용병이 있는 페이지로 갱신
	int iSize = m_vCharInfoList.size();
	for( int i=0; i < iSize; ++i )
	{
		if( m_vCharInfoList[i].m_iClassType == iSelectClassType )
		{
			return i;
		}
	}

	return -1;
}

void MyInventoryWnd::ShowSoldierTabDirect()
{
	if( !IsShow() )
		ShowWnd();

	DWORD dwSmallTab = GetDefaultSmallTab( ID_SOLDIER_TAB_BTN );
	ChangeMainTab( ID_SOLDIER_TAB_BTN );
	ChangeSmallTab( dwSmallTab );
	SetFirstBtnInfo();

	if( m_vInventoryItemInfoList.empty() )
	{
		ChangeMainTab( ID_SOLDIER_TAB_BTN );

		m_iSelectArray = FindSelectCharArray( m_iSelectClassType );

		int iStartArray = m_iCurPage * PAGE_PER_CHAR_NUM;
		for( int i = 0 ; i < MAX_CHAR_INFO_WND_BTN ; i++ )
		{
			InventorySoldierItemWnd *pItem = dynamic_cast<InventorySoldierItemWnd*>(FindChildWnd(ID_CHAR_INFO_WND_1_BTN + i));
			if( !pItem ) continue;

			int iCurArray = iStartArray + i;
			if( m_iSelectArray == iCurArray )
				pItem->SetSelect( true );
			else
				pItem->SetSelect( false );
		}

		MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
		if( pInfoWnd )
		{
			ShowMainInfoWnd();
			pInfoWnd->SetSlodierInfo( m_iSelectClassType );
		}
	}
}

void MyInventoryWnd::ShowSoldierTabDirect( int iClassType )
{
	if( !IsShow() )
		ShowWnd();

	m_iSelectClassType = iClassType;

	ChangeMainTab( ID_SOLDIER_TAB_BTN );

	// 오른쪽 창쪽에 클레스 타입 전달 필요.
	if( m_iSelectClassType == -1 )
	{
		SetFirstBtnInfo();
		return;
	}

	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd )
	{
		ShowMainInfoWnd();
		pInfoWnd->SetSlodierInfo( m_iSelectClassType );
	}

	// 해당 용병이 있는 페이지로 갱신
	int iSize = m_vCharInfoList.size();
	for( int i=0; i < iSize; ++i )
	{
		if( m_vCharInfoList[i].m_iClassType == m_iSelectClassType )
		{
			m_iSelectArray = i;

			int iNewPage = i / PAGE_PER_CHAR_NUM;

			ioScroll *pScroll = (ioScroll*)FindChildWnd( ID_VERT_SCROLL );
			if( pScroll )
			{
				pScroll->SetScrollPos( iNewPage );
			}
		}
	}
}

void MyInventoryWnd::ShowExtraItemTabDirect( int iClassType, int iIndex )
{
	if( !IsShow() )
		ShowWnd();

	m_iSelectClassType = iClassType;

	ChangeMainTab( ID_EXTRA_TAB_BTN );
	SetFirstBtnInfo();

	ioUserExtraItem *pExtraItem = g_MyInfo.GetUserExtraItem();
	EXTRAITEMSLOT kSlot;
	if( !pExtraItem || !pExtraItem->GetExtraItem( iIndex, kSlot ) )
		return;

	int iArrayCnt = 0;
	for( iArrayCnt; iArrayCnt < (int)m_vInventoryItemInfoList.size(); ++iArrayCnt )
	{
		if( m_vInventoryItemInfoList[iArrayCnt].m_iMainType == INVENTORY_ITEM_EXTRAITEM &&
			m_vInventoryItemInfoList[iArrayCnt].m_iMagicCode == iIndex )
		{
			int iNewPage = iArrayCnt / PAGE_PER_ITEM_NUM;

			ioScroll *pScroll = (ioScroll*)FindChildWnd( ID_VERT_SCROLL );
			if( pScroll )
			{
				pScroll->SetScrollPos( iNewPage );
			}

			break;
		}
	}
}

void MyInventoryWnd::ShowDecoTabDirect( int iClassType )
{
	if( !IsShow() )
		ShowWnd();

	m_iSelectClassType = iClassType;

	ChangeMainTab( ID_DECO_TAB_BTN );
	SetFirstBtnInfo();
}

void MyInventoryWnd::ShowEtcTabDirect( DWORD dwItemCode )
{
	if( !IsShow() )
		ShowWnd();

	if( dwItemCode == 0 )
	{
		ChangeMainTab( ID_ETC_TAB_BTN );
		SetFirstBtnInfo();
		return;
	}

	ioEtcItem *pEtcItem = g_EtcItemMgr.FindEtcItem( dwItemCode );
	if( pEtcItem )
	{
		int iShopTabType = SHOP_TAB_TYPE_NONE;
		int iSize = pEtcItem->GetValueSize();
		for (int j = 0; j < iSize ; j++)
		{
			iShopTabType = pEtcItem->GetShopTabType( j );
			if( iShopTabType != SHOP_TAB_TYPE_NONE )
				break;
		}

		DWORD dwSmallTab = 0;
		switch( iShopTabType )
		{
		case SHOP_TAB_TYPE_SOLDIER:
			{
				dwSmallTab = GetDefaultSmallTab( ID_SOLDIER_TAB_BTN );
				ChangeMainTab( ID_SOLDIER_TAB_BTN );
				ChangeSmallTab( dwSmallTab );
				SetFirstBtnInfo();
			}
			break;
		case SHOP_TAB_TYPE_EXTRA:
			{
				dwSmallTab = GetDefaultSmallTab( ID_EXTRA_TAB_BTN );
				ChangeMainTab( ID_EXTRA_TAB_BTN );
				ChangeSmallTab( dwSmallTab );
				SetFirstBtnInfo();
			}
			break;
		case SHOP_TAB_TYPE_DECO:
			{
				dwSmallTab = GetDefaultSmallTab( ID_DECO_TAB_BTN );
				ChangeMainTab( ID_DECO_TAB_BTN );
				ChangeSmallTab( dwSmallTab );
				SetFirstBtnInfo();
			}
			break;
		case SHOP_TAB_TYPE_COSTUME:
			{
				// UI코스튬 관련 (특별아이템 재구매시)
				dwSmallTab = GetDefaultSmallTab( ID_COSTUME_TAB_BTN );
				ChangeMainTab( ID_COSTUME_TAB_BTN );
				ChangeSmallTab( dwSmallTab );
				SetFirstBtnInfo();
			}
			break;
		default:
			{
				ChangeMainTab( ID_ETC_TAB_BTN );
				SetFirstBtnInfo();
			}
			break;
		}
	}
	else
	{
		ChangeMainTab( ID_ETC_TAB_BTN );
		SetFirstBtnInfo();
	}
}

bool MyInventoryWnd::IsShowPresentTab()
{
	if( !IsShow() )
		return false;

	return (m_dwCurTabID == ID_PRESENT_TAB_BTN);
}

void MyInventoryWnd::ShowPresentTabDirect()
{
	if( !IsShow() )
		ShowWnd();

	ChangeMainTab( ID_PRESENT_TAB_BTN );
	SetFirstBtnInfo();
}

void MyInventoryWnd::ShowSubscriptionTabDirect()
{
	if( !IsShow() )
		ShowWnd();

	ChangeMainTab( ID_PRESENT_TAB_BTN );

	int iSize = g_SubscriptionMgr.GetMaxSubscription();
	if( iSize > 0 )
	{
		ChangeSmallTab( ID_SMALL_TAB_2_BTN );
	}

	SetFirstBtnInfo();
}

void MyInventoryWnd::ShowAlchemicTabDirect()
{
	if( !IsShow() )
		ShowWnd();

	ChangeMainTab( ID_ALCHEMIC_TAB_BTN );
	
	ShowMainInfoWnd();
	ShowAlchemicFuncWnd();
}

// UI코스튬 관련 (코스튬 탭으로 바로 이동)
void MyInventoryWnd::ShowCostumeTabDirect( int iClassType, int iIndex )
{
	if( !IsShow() )
		ShowWnd();

	m_iSelectClassType = iClassType;

	ChangeMainTab( ID_COSTUME_TAB_BTN );
	SetFirstBtnInfo();
	
	ioUserCostume *pCostume = g_MyInfo.GetUserCostume();
	CostumeSlot kSlot;
	if( !pCostume || !pCostume->GetCostumeSlot( iIndex, kSlot ) )
		return;

	int iArrayCnt = 0;
	for( iArrayCnt; iArrayCnt < (int)m_vInventoryItemInfoList.size(); ++iArrayCnt )
	{
		if( m_vInventoryItemInfoList[iArrayCnt].m_iMainType == INVENTORY_ITEM_COSTUME &&
			m_vInventoryItemInfoList[iArrayCnt].m_iMagicCode == iIndex )
		{
			int iNewPage = iArrayCnt / PAGE_PER_ITEM_NUM;

			ioScroll *pScroll = (ioScroll*)FindChildWnd( ID_VERT_SCROLL );
			if( pScroll )
			{
				pScroll->SetScrollPos( iNewPage );
			}

			break;
		}
	}
}

void MyInventoryWnd::ShowAccessoryTabDirect( int iClassType, int iIndex /*= -1 */ )
{
	if( !IsShow() )
		ShowWnd();

	m_iSelectClassType = iClassType;

	ChangeMainTab( ID_ACCESSORY_TAB_BTN );
	SetFirstBtnInfo();

	ioUserAccessory *pAccessory = g_MyInfo.GetUserAccessory();
	AccessorySlot kSlot;
	if( !pAccessory || !pAccessory->GetAccessorySlot( iIndex, kSlot ) )
		return;

	int iArrayCnt = 0;
	for( iArrayCnt; iArrayCnt < (int)m_vInventoryItemInfoList.size(); ++iArrayCnt )
	{
		if( m_vInventoryItemInfoList[iArrayCnt].m_iMainType == INVENTORY_ITEM_ACCESSORY &&
			m_vInventoryItemInfoList[iArrayCnt].m_iMagicCode == iIndex )
		{
			int iNewPage = iArrayCnt / PAGE_PER_ITEM_NUM;

			ioScroll *pScroll = (ioScroll*)FindChildWnd( ID_VERT_SCROLL );
			if( pScroll )
			{
				pScroll->SetScrollPos( iNewPage );
			}

			break;
		}
	}
}

void MyInventoryWnd::RenderWnd()
{
	if( OnRenderShowAndHide() ) return;
	if( !IsShow() )	return;

	OnRender();

	if( HasChild() )
	{
		m_bNowGoToTopReserving = true;

		ioWndList::iterator iter = m_ChildList.begin();
		for( ; iter!=m_ChildList.end() ; ++iter )
		{
			ioWnd *pWnd = *iter;
			if( pWnd )
			{
				// HARDCODE : ID_CHARGE_GOLD_BTN, ID_SHOP_BTN, ID_EXIT_BTN들은 RenderAfterChild 시점에 한다
				DWORD dwID = pWnd->GetID();
				switch( dwID )
				{
				case ID_CHARGE_GOLD_BTN:
				case ID_SHOP_BTN:
				case ID_EXIT_BTN:
					continue;
				}

				pWnd->RenderWnd();
			}
		}

		m_bNowGoToTopReserving = false;
		ExecuteReservedChildGoToTop();
	}

	OnRenderAfterChild();
}

void MyInventoryWnd::OnRender()
{
	// Title은 나중에 출력.
	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	if( m_pSelectedFrm )
		m_pSelectedFrm->Render( iXPos, iYPos );
	if( m_pMultiplyFrm )
		m_pMultiplyFrm->Render( iXPos, iYPos, UI_RENDER_MULTIPLY );

	RenderImageList( iXPos, iYPos, UI_RENDER_NORMAL );

	// tab space dot
	if( !m_pTabDot )
		return;

	enum { WIDTH = 1, HEIGHT = 2, TOTAL_WIDTH = 444, };

	m_pTabDot->SetSize( WIDTH, HEIGHT );
	m_pTabDot->SetHorzFormatting( ioUIRenderImage::HF_STRETCH );

	int iSpaceSize = m_dwSmallTabEndID-ID_SMALL_TAB_1_BTN;

	if( m_dwCurTabID == ID_PRESENT_TAB_BTN )
	{
		enum { X_TAB_DOT = 121, Y_TAB_DOT = 112, TAB_DOT_X_GET = 95, };

		for (int i = 0; i < iSpaceSize ; i++)
		{
			m_pTabDot->Render( iXPos + X_TAB_DOT + (TAB_DOT_X_GET * i), iYPos + Y_TAB_DOT );
		}

		// 하단탭 줄
		int iCurWidth = TOTAL_WIDTH - ( TAB_DOT_X_GET*(iSpaceSize+1) ); // 탭표시 공간 만큼 축소 , +1는 첫번째탭 크기를 더한다.
		if( iCurWidth > 0 )
		{
			m_pTabDot->SetSize( iCurWidth, HEIGHT );
			m_pTabDot->Render( iXPos + X_TAB_DOT + (TAB_DOT_X_GET * iSpaceSize), iYPos + Y_TAB_DOT );
		}
	}
	else
	{
		enum { X_OFFSET = 517, Y_OFFSET = 14, X_TAB_DOT = 82, Y_TAB_DOT = 112, TAB_DOT_X_GET = 56, };

		for (int i = 0; i < iSpaceSize ; i++)
		{
			m_pTabDot->Render( iXPos + X_TAB_DOT + (TAB_DOT_X_GET * i), iYPos + Y_TAB_DOT );
		}

		// 하단탭 줄
		int iCurWidth = TOTAL_WIDTH - ( TAB_DOT_X_GET*(iSpaceSize+1) ); // 탭표시 공간 만큼 축소 , +1는 첫번째탭 크기를 더한다.
		if( iCurWidth > 0 )
		{
			m_pTabDot->SetSize( iCurWidth, HEIGHT );
			m_pTabDot->Render( iXPos + X_TAB_DOT + (TAB_DOT_X_GET * iSpaceSize), iYPos + Y_TAB_DOT );
		}
	}
}

void MyInventoryWnd::OnRenderAfterChild()
{
	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	// Title 출력
	if( m_pSelectedTitleFrm)
		m_pSelectedTitleFrm->Render( iXPos, iYPos );

	if( m_pUITitle )
		m_pUITitle->PrintNormal( iXPos, iYPos );

	// Title 상에 존재하는 Child 출력
	if( HasChild() )
	{
		m_bNowGoToTopReserving = true;

		ioWnd *pWnd = NULL;
		pWnd = FindChildWnd( ID_CHARGE_GOLD_BTN );
		if( pWnd )
			pWnd->RenderWnd();


		pWnd = FindChildWnd( ID_SHOP_BTN );
		if( pWnd )
			pWnd->RenderWnd();

		pWnd = FindChildWnd( ID_EXIT_BTN );
		if( pWnd )
			pWnd->RenderWnd();

		m_bNowGoToTopReserving = false;
		ExecuteReservedChildGoToTop();
	}
#ifdef STEAM_ATTACH
	enum { X_OFFSET = 517 + 78, Y_OFFSET = 14, };
#else
	// Title 상에 존재하는 텍스트 출력
	enum { X_OFFSET = 594, X_OFFSET2	= 578, Y_OFFSET = 15, };
#endif
	char szPeso[MAX_PATH]="";
	char szCash[MAX_PATH]="";
	char szStamp[MAX_PATH]="";
	char szClover[MAX_PATH]="";
	char szMileage[MAX_PATH]="";
	int  iMaxStamp = g_MyInfo.GetEtcCoinMoney( ioEtcItem::EIT_ETC_SILVER_COIN );
	int  iMaxClover = g_MyInfo.GetEtcCoinMoney( ioEtcItem::EIT_ETC_CLOVER );
	int  iMaxMileage = g_MyInfo.GetEtcCoinMoney( ioEtcItem::EIT_ETC_MILEAGE_COIN );
	Help::ConvertNumToStrComma( g_MyInfo.GetMoney(), szPeso, sizeof( szPeso ) );
	Help::ConvertNumToStrComma( g_MyInfo.GetTotalCash(), szCash, sizeof( szCash ) );
	Help::ConvertNumToStrComma( iMaxStamp, szStamp, sizeof( szStamp ) );
	Help::ConvertNumToStrComma( iMaxClover, szClover, sizeof( szClover ) );
	
	int iAdd_OffsetY = 0;
	if( iMaxMileage > 0 )
	{
		Help::ConvertNumToStrComma( iMaxMileage, szMileage, sizeof( szMileage ) );
		iAdd_OffsetY = 7;
	}

	ioComplexStringPrinter kPrinter;
	kPrinter.SetTextStyle( TS_OUTLINE_FULL_2X );
	kPrinter.SetBkColor( 12, 66, 111 );
	if( iMaxStamp > 0 )
	{
		kPrinter.SetTextColor( 130, 198, 255 );	
		ioEtcItem *pEtcItem = g_EtcItemMgr.FindEtcItem( ioEtcItem::EIT_ETC_SILVER_COIN );
		if( pEtcItem )
		{
			kPrinter.AddTextPiece( FONT_SIZE_11, STR(4), pEtcItem->GetName().c_str(), szStamp );
		}
		else
		{
			kPrinter.AddTextPiece( FONT_SIZE_11, STR(3), szStamp );
		}
		kPrinter.SetTextColor( TCT_DEFAULT_LIGHTGRAY );
		kPrinter.AddTextPiece( FONT_SIZE_11, " / " );
	}

	if( iMaxClover > 0 )
	{
		kPrinter.SetTextColor( 130, 198, 255 );	
		ioEtcItem *pEtcItem = g_EtcItemMgr.FindEtcItem( ioEtcItem::EIT_ETC_CLOVER );
		if( pEtcItem )
			kPrinter.AddTextPiece( FONT_SIZE_11, STR(5), pEtcItem->GetName().c_str(), szClover );
		else
			kPrinter.AddTextPiece( FONT_SIZE_11, STR(6), szClover );
		kPrinter.SetTextColor( TCT_DEFAULT_LIGHTGRAY );		
		kPrinter.AddTextPiece( FONT_SIZE_11, " / " );
	}


	kPrinter.SetTextColor( 130, 198, 255 );
	kPrinter.AddTextPiece( FONT_SIZE_11, STR(1) );
	kPrinter.AddTextPiece( FONT_SIZE_11, szPeso );

	//if( !g_App.IsWemadeChannelingGetCash() )
	{
		kPrinter.SetTextColor( TCT_DEFAULT_LIGHTGRAY );
		kPrinter.AddTextPiece( FONT_SIZE_11, " / " );
		kPrinter.SetTextColor( 255, 188, 26 );
		kPrinter.AddTextPiece( FONT_SIZE_11, STR(2) );
		kPrinter.AddTextPiece( FONT_SIZE_11, szCash );
	}
	
	ioButton* pBonusCashBtn = dynamic_cast<ioButton*>(FindChildWnd( ID_BONUS_CASH_BTN ));
	if ( pBonusCashBtn && pBonusCashBtn->IsShow() )
	{
		//마일리지가 존재한다면
		if ( iMaxMileage <= 0 )
		{
			pBonusCashBtn->SetWndPos( pBonusCashBtn->GetXPos() - 2 , pBonusCashBtn->GetYPos() + 6 );
			pBonusCashBtn->RenderWnd();
			pBonusCashBtn->SetWndPos( pBonusCashBtn->GetXPos() + 2 , pBonusCashBtn->GetYPos() - 6 );
		}
		else
			pBonusCashBtn->RenderWnd();
		kPrinter.PrintFullText( iXPos + X_OFFSET2, iYPos + Y_OFFSET-iAdd_OffsetY, TAT_RIGHT );
	}
	else
		kPrinter.PrintFullText( iXPos + X_OFFSET, iYPos + Y_OFFSET-iAdd_OffsetY, TAT_RIGHT );
	kPrinter.ClearList();

	if( iMaxMileage > 0 )
	{
		iAdd_OffsetY = 6;
		kPrinter.SetTextColor( 115, 204, 0 );
		ioEtcItem *pEtcItem = g_EtcItemMgr.FindEtcItem( ioEtcItem::EIT_ETC_MILEAGE_COIN );
		if( pEtcItem )
		{
			kPrinter.AddTextPiece( FONT_SIZE_11, STR(4), pEtcItem->GetName().c_str(), szMileage );
		}
		else
		{
			kPrinter.AddTextPiece( FONT_SIZE_11, STR(4), szMileage );
		}
		kPrinter.SetTextColor( TCT_DEFAULT_LIGHTGRAY );	
		kPrinter.PrintFullText( iXPos + X_OFFSET, iYPos + Y_OFFSET+iAdd_OffsetY, TAT_RIGHT );
		kPrinter.ClearList();	
	}

	enum
	{
		X_BONUS_CASH_TOOLTIP_OFFSET	= 63,
		Y_BONUS_CASH_TOOLTIP_OFFSET	= 22,
	};
	ioWnd* pBonusCashToolTip = FindChildWnd( ID_BONUS_CASH_TOOLTIP );
	if ( pBonusCashToolTip && pBonusCashToolTip->IsShow() )
	{
		pBonusCashToolTip->SetWndPos( X_OFFSET2 - X_BONUS_CASH_TOOLTIP_OFFSET, Y_OFFSET-iAdd_OffsetY - Y_BONUS_CASH_TOOLTIP_OFFSET );
		pBonusCashToolTip->RenderWnd();
	}
}
 
void MyInventoryWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() )
		return;

	ioMovingWnd::OnProcess( fTimePerSec );
	
	ProcessCustomBtnTooltip();

	//본부에서는 g_MyInfo.GetRoomOutSec() 내부에서 시간 확인 한후 다른 룸으로 내보내지않음
	if( g_MyInfo.GetRoomOutSec() != -1 && ioPlayMode::GetModeType() != MT_HEADQUARTERS && ioPlayMode::GetModeType() != MT_HOUSE ) 
	{
		char szTitle[MAX_PATH] = "";
		SafeSprintf( szTitle, sizeof( szTitle ), STR(1), g_MyInfo.GetRoomOutSec() );
		SetTitleText( szTitle );
		m_bRoomOutTimeCheck = true;
	}
	else if( m_bRoomOutTimeCheck )
	{
		HideWnd();
	}

	UpdateHouseButton();
	CheckBonusCash();
#ifdef _LSWC
	SetChildInActive( ID_CHARGE_GOLD_BTN );
#endif

}

void MyInventoryWnd::UpdateHouseButton()
{
	SetChildWndStyleAdd( ID_HOUSE_ITEM_BTN, IWS_EXACTIVE );
	if( g_MyInfo.IsHouseOpen() )
	{
		SetChildWndStyleRemove( ID_HOUSE_ITEM_BTN, IWS_EXACTIVE );
	}
}

bool MyInventoryWnd::ProcessCustomBtnTooltipExtraItem()
{
	int i = 0;
	EXTRAITEMSLOT kSlot;
	ioComplexStringPrinter kPrinter[GLOBAL_TEXT_TOOLTIP_LINE];
	for(int i = 0;i < MAX_ITEM_INFO_WND_BTN;i++)
	{
		InventoryItemWnd *pItemWnd = dynamic_cast<InventoryItemWnd*>(FindChildWnd( ID_ITEM_INFO_WND_1_BTN + i ));
		if( !pItemWnd ) continue;
		if( !pItemWnd->IsShow() ) continue;
		if( pItemWnd->GetMainType() != INVENTORY_ITEM_EXTRAITEM ) continue;
		if( !pItemWnd->IsCustomToolTip() ) continue;

		ioUserExtraItem *pExtraItem = g_MyInfo.GetUserExtraItem();
		if( !pExtraItem ) continue;

		int iMagicCode = pItemWnd->GetMagicCode();
		if( !pExtraItem->GetExtraItem( iMagicCode, kSlot ) ) continue;
	}

	if( kSlot.m_dwMaleCustom > 0 || kSlot.m_dwFemaleCustom > 0 )
	{
		kPrinter[0].SetTextStyle( TS_NORMAL );
		kPrinter[0].SetBkColor( 0, 0, 0 );
		if( kSlot.m_dwMaleCustom > 0 && kSlot.m_dwFemaleCustom > 0 )
		{
			kPrinter[0].SetTextColor( TCT_DEFAULT_BLUE );
			kPrinter[0].AddTextPiece( FONT_SIZE_12, STR(1) );
			kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
			kPrinter[0].AddTextPiece( FONT_SIZE_12, ", " );
			kPrinter[0].SetTextColor( TCT_DEFAULT_RED );
			kPrinter[0].AddTextPiece( FONT_SIZE_12, STR(2) );
		}		
		else if( kSlot.m_dwMaleCustom > 0 )
		{
			kPrinter[0].SetTextColor( TCT_DEFAULT_BLUE );
			kPrinter[0].AddTextPiece( FONT_SIZE_12, STR(3) );
		}
		else if( kSlot.m_dwFemaleCustom > 0 )
		{
			kPrinter[0].SetTextColor( TCT_DEFAULT_RED );
			kPrinter[0].AddTextPiece( FONT_SIZE_12, STR(4) );
		}
		kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
		kPrinter[0].AddTextPiece( FONT_SIZE_12, STR(5) );

		kPrinter[1].SetTextStyle( TS_NORMAL );
		kPrinter[1].SetBkColor( 0, 0, 0 );
		kPrinter[1].SetTextColor( TCT_DEFAULT_GRAY );
		kPrinter[1].AddTextPiece( FONT_SIZE_12, STR(6) );
		kPrinter[1].SetTextColor( TCT_DEFAULT_RED );
		kPrinter[1].AddTextPiece( FONT_SIZE_12, STR(7) );
		kPrinter[1].SetTextColor( TCT_DEFAULT_GRAY );
		kPrinter[1].AddTextPiece( FONT_SIZE_12, STR(8) );
	}

	ItemCustomThumbnailTooltip *pThumbnailTooltip = dynamic_cast<ItemCustomThumbnailTooltip*>(g_GUIMgr.FindWnd( MY_INVENTORY_CUSTOM_TOOLTIP ));
	if( pThumbnailTooltip )
	{
		pThumbnailTooltip->SetTooltipText( kPrinter, kSlot );
		return pThumbnailTooltip->IsShow();
	}
	return false;
}

bool MyInventoryWnd::ProcessCustomBtnTooltipPresent()
{
	int i = 0;
	EXTRAITEMSLOT kSlot;
	ioComplexStringPrinter kPrinter[GLOBAL_TEXT_TOOLTIP_LINE];
	for(int i = 0;i < MAX_ITEM_INFO_WND_BTN;i++)
	{
		InventoryItemWnd *pItemWnd = dynamic_cast<InventoryItemWnd*>(FindChildWnd( ID_ITEM_INFO_WND_1_BTN + i ));
		if( !pItemWnd ) continue;
		if( !pItemWnd->IsShow() ) continue;
		if( pItemWnd->GetMainType() != INVENTORY_ITEM_PRESENT ) continue;
		if( !pItemWnd->IsCustomToolTip() ) continue;

		ioHashString szSendID;
		short sPresentType = 0;
		short sPresentMent = 0;
		int iPresentValue1 = 0;
		int iPresentValue2 = 0;
		int iPresentValue3 = 0;
		int iPresentValue4 = 0;
		DWORD dwLimitDate  = 0;
		g_PresentMgr.GetPresentDataToIndex( pItemWnd->GetMagicCode(), pItemWnd->GetMagicCodeEx(), szSendID,	sPresentType, sPresentMent, iPresentValue1, iPresentValue2, iPresentValue3, iPresentValue4, dwLimitDate );
        
		if( sPresentType == PRESENT_EXTRAITEM )
		{
			kSlot.m_iItemCode = iPresentValue1;
			kSlot.m_iReinforce= ( iPresentValue2 % PRESENT_EXTRAITEM_DIVISION_1 ) / PRESENT_EXTRAITEM_DIVISION_2;
			kSlot.m_dwMaleCustom = iPresentValue3;
			kSlot.m_dwFemaleCustom = iPresentValue4;
			break;
		}
	}

	if( kSlot.m_dwMaleCustom > 0 || kSlot.m_dwFemaleCustom > 0 )
	{
		kPrinter[0].SetTextStyle( TS_NORMAL );
		kPrinter[0].SetBkColor( 0, 0, 0 );
		kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
		kPrinter[0].AddTextPiece( FONT_SIZE_12, STR(1) );

		if( kSlot.m_dwMaleCustom > 0 && kSlot.m_dwFemaleCustom > 0 )
		{
			kPrinter[0].SetTextColor( TCT_DEFAULT_BLUE );
			kPrinter[0].AddTextPiece( FONT_SIZE_12, STR(2) );
			kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
			kPrinter[0].AddTextPiece( FONT_SIZE_12, ", " );
			kPrinter[0].SetTextColor( TCT_DEFAULT_RED );
			kPrinter[0].AddTextPiece( FONT_SIZE_12, STR(3) );
		}		
		else if( kSlot.m_dwMaleCustom > 0 )
		{
			kPrinter[0].SetTextColor( TCT_DEFAULT_BLUE );
			kPrinter[0].AddTextPiece( FONT_SIZE_12, STR(4) );
		}
		else if( kSlot.m_dwFemaleCustom > 0 )
		{
			kPrinter[0].SetTextColor( TCT_DEFAULT_RED );
			kPrinter[0].AddTextPiece( FONT_SIZE_12, STR(5) );
		}
		kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
		kPrinter[0].AddTextPiece( FONT_SIZE_12, STR(6) );

		kPrinter[1].SetTextStyle( TS_NORMAL );
		kPrinter[1].SetBkColor( 0, 0, 0 );
		kPrinter[1].SetTextColor( TCT_DEFAULT_GRAY );
		kPrinter[1].AddTextPiece( FONT_SIZE_12, STR(7) );
		kPrinter[1].SetTextColor( TCT_DEFAULT_GREEN );
		kPrinter[1].AddTextPiece( FONT_SIZE_12, STR(8) );
		kPrinter[1].SetTextColor( TCT_DEFAULT_GRAY );
		kPrinter[1].AddTextPiece( FONT_SIZE_12, STR(9) );
	}

	ItemCustomThumbnailTooltip *pTooltip = dynamic_cast<ItemCustomThumbnailTooltip*>(g_GUIMgr.FindWnd( PRESENT_ITEM_THUMBNAIL_TOOLTIP ));
	if( pTooltip )
	{
		pTooltip->SetTooltipText( kPrinter, kSlot );
		return pTooltip->IsShow();
	}
	return false;
}

// UI코스튬 관련 (코스튬 스킨 적용 미리보기 설정 - 코스튬)
bool MyInventoryWnd::ProcessCustomBtnTooltipCostume()
{
	CostumeSlot kSlot;
	ioComplexStringPrinter kPrinter[GLOBAL_TEXT_TOOLTIP_LINE];
	for( int i=0; i<MAX_ITEM_INFO_WND_BTN; i++ )
	{
		InventoryItemWnd *pItemWnd = dynamic_cast<InventoryItemWnd*>(FindChildWnd( ID_ITEM_INFO_WND_1_BTN + i ));
		if( !pItemWnd ) continue;
		if( !pItemWnd->IsShow() ) continue;
		if( pItemWnd->GetMainType() != INVENTORY_ITEM_COSTUME ) continue;
		if( !pItemWnd->IsCustomToolTip() ) continue;

		ioUserCostume *pCostume = g_MyInfo.GetUserCostume();
		if( !pCostume ) continue;

		int iMagicCode = pItemWnd->GetMagicCode();
		if( !pCostume->GetCostumeSlot( iMagicCode, kSlot ) ) continue;
	}

	if( kSlot.m_dwMaleCustom > 0 || kSlot.m_dwFemaleCustom > 0 )
	{
		kPrinter[0].SetTextStyle( TS_NORMAL );
		kPrinter[0].SetBkColor( 0, 0, 0 );
		if( kSlot.m_dwMaleCustom > 0 && kSlot.m_dwFemaleCustom > 0 )
		{
			kPrinter[0].SetTextColor( TCT_DEFAULT_BLUE );
			kPrinter[0].AddTextPiece( FONT_SIZE_12, STR(1) );
			kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
			kPrinter[0].AddTextPiece( FONT_SIZE_12, ", " );
			kPrinter[0].SetTextColor( TCT_DEFAULT_RED );
			kPrinter[0].AddTextPiece( FONT_SIZE_12, STR(2) );
		}		
		else if( kSlot.m_dwMaleCustom > 0 )
		{
			kPrinter[0].SetTextColor( TCT_DEFAULT_BLUE );
			kPrinter[0].AddTextPiece( FONT_SIZE_12, STR(3) );
		}
		else if( kSlot.m_dwFemaleCustom > 0 )
		{
			kPrinter[0].SetTextColor( TCT_DEFAULT_RED );
			kPrinter[0].AddTextPiece( FONT_SIZE_12, STR(4) );
		}
		kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
		kPrinter[0].AddTextPiece( FONT_SIZE_12, STR(5) );

		kPrinter[1].SetTextStyle( TS_NORMAL );
		kPrinter[1].SetBkColor( 0, 0, 0 );
		kPrinter[1].SetTextColor( TCT_DEFAULT_GRAY );
		kPrinter[1].AddTextPiece( FONT_SIZE_12, STR(6) );
		kPrinter[1].SetTextColor( TCT_DEFAULT_RED );
		kPrinter[1].AddTextPiece( FONT_SIZE_12, STR(7) );
		kPrinter[1].SetTextColor( TCT_DEFAULT_GRAY );
		kPrinter[1].AddTextPiece( FONT_SIZE_12, STR(8) );
	}

	CostumeCustomThumbnailTooltip *pThumbnailTooltip = dynamic_cast<CostumeCustomThumbnailTooltip*>(g_GUIMgr.FindWnd( MY_INVENTORY_COSTUME_CUSTOM_TOOLTIP ));
	if( pThumbnailTooltip )
	{
		CostumeEquipInfo sCostumeEquipInfo;
		sCostumeEquipInfo.m_CostumeCode = kSlot.m_nCostumeCode;
		sCostumeEquipInfo.m_Costume_Male_Custom = kSlot.m_dwMaleCustom;
		sCostumeEquipInfo.m_Costume_Female_Custom = kSlot.m_dwFemaleCustom;

		pThumbnailTooltip->SetTooltipText( kPrinter, sCostumeEquipInfo );
		return pThumbnailTooltip->IsShow();
	}
	return false;
}

// UI코스튬 관련 (코스튬 스킨 적용 미리보기 설정 - 선물)
bool MyInventoryWnd::ProcessCustomBtnTooltipCostumePresent()
{
	CostumeEquipInfo sCostumeEquipInfo;
	ioComplexStringPrinter kPrinter[GLOBAL_TEXT_TOOLTIP_LINE];
	for( int i=0; i<MAX_ITEM_INFO_WND_BTN; i++ )
	{
		InventoryItemWnd *pItemWnd = dynamic_cast<InventoryItemWnd*>(FindChildWnd( ID_ITEM_INFO_WND_1_BTN + i ));
		if( !pItemWnd ) continue;
		if( !pItemWnd->IsShow() ) continue;
		if( pItemWnd->GetMainType() != INVENTORY_ITEM_PRESENT ) continue;
		if( !pItemWnd->IsCustomToolTip() ) continue;

		ioHashString szSendID;
		short sPresentType = 0;
		short sPresentMent = 0;
		int iPresentValue1 = 0;
		int iPresentValue2 = 0;
		int iPresentValue3 = 0;
		int iPresentValue4 = 0;
		DWORD dwLimitDate  = 0;
		g_PresentMgr.GetPresentDataToIndex( pItemWnd->GetMagicCode(), pItemWnd->GetMagicCodeEx(), szSendID,	sPresentType, sPresentMent, iPresentValue1, iPresentValue2, iPresentValue3, iPresentValue4, dwLimitDate );

		if( sPresentType == PRESENT_COSTUME )
		{
			sCostumeEquipInfo.m_CostumeCode = iPresentValue1;
			sCostumeEquipInfo.m_Costume_Male_Custom = iPresentValue3;
			sCostumeEquipInfo.m_Costume_Female_Custom = iPresentValue4;
			break;
		}
	}

	if( sCostumeEquipInfo.m_Costume_Male_Custom > 0 || sCostumeEquipInfo.m_Costume_Female_Custom > 0 )
	{
		kPrinter[0].SetTextStyle( TS_NORMAL );
		kPrinter[0].SetBkColor( 0, 0, 0 );
		kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
		kPrinter[0].AddTextPiece( FONT_SIZE_12, STR(1) );

		if( sCostumeEquipInfo.m_Costume_Male_Custom > 0 && sCostumeEquipInfo.m_Costume_Female_Custom > 0 )
		{
			kPrinter[0].SetTextColor( TCT_DEFAULT_BLUE );
			kPrinter[0].AddTextPiece( FONT_SIZE_12, STR(2) );
			kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
			kPrinter[0].AddTextPiece( FONT_SIZE_12, ", " );
			kPrinter[0].SetTextColor( TCT_DEFAULT_RED );
			kPrinter[0].AddTextPiece( FONT_SIZE_12, STR(3) );
		}		
		else if( sCostumeEquipInfo.m_Costume_Male_Custom > 0 )
		{
			kPrinter[0].SetTextColor( TCT_DEFAULT_BLUE );
			kPrinter[0].AddTextPiece( FONT_SIZE_12, STR(4) );
		}
		else if( sCostumeEquipInfo.m_Costume_Female_Custom > 0 )
		{
			kPrinter[0].SetTextColor( TCT_DEFAULT_RED );
			kPrinter[0].AddTextPiece( FONT_SIZE_12, STR(5) );
		}
		kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
		kPrinter[0].AddTextPiece( FONT_SIZE_12, STR(6) );

		kPrinter[1].SetTextStyle( TS_NORMAL );
		kPrinter[1].SetBkColor( 0, 0, 0 );
		kPrinter[1].SetTextColor( TCT_DEFAULT_GRAY );
		kPrinter[1].AddTextPiece( FONT_SIZE_12, STR(7) );
		kPrinter[1].SetTextColor( TCT_DEFAULT_GREEN );
		kPrinter[1].AddTextPiece( FONT_SIZE_12, STR(8) );
		kPrinter[1].SetTextColor( TCT_DEFAULT_GRAY );
		kPrinter[1].AddTextPiece( FONT_SIZE_12, STR(9) );
	}

	CostumeCustomThumbnailTooltip *pTooltip = dynamic_cast<CostumeCustomThumbnailTooltip*>(g_GUIMgr.FindWnd( PRESENT_COSTUME_THUMBNAIL_TOOLTIP ));
	if( pTooltip )
	{
		pTooltip->SetTooltipText( kPrinter, sCostumeEquipInfo );
		return pTooltip->IsShow();
	}
	return false;
}

void MyInventoryWnd::ProcessCustomBtnTooltip()
{		
	if( !ProcessCustomBtnTooltipExtraItem() )
	{
		ProcessCustomBtnTooltipPresent();
	}
	
	// UI코스튬 관련 (코스튬 스킨 적용 미리보기 설정)
	if( !ProcessCustomBtnTooltipCostume() )
	{
		ProcessCustomBtnTooltipCostumePresent();
	}
}

void MyInventoryWnd::SendRequestGashaponList()
{
	ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
	if( pLocal && !pLocal->IsSendGashaponList() )
		return;

	if( m_bGashaponListRequest )
		return;

	m_bGashaponListRequest = true;

	NewShopWnd *pShop = dynamic_cast<NewShopWnd*> ( g_GUIMgr.FindWnd( NEW_SHOP_WND ) );
	if( pShop )
	{
		if( pShop->IsGashaponListRequest() )
			return;
	}

	SP2Packet kPacket( CTPK_GASHAPON_LIST );
	kPacket << ioEtcItem::EIT_ETC_GASHAPON14;
	TCPNetwork::SendToServer( kPacket );
}

int MyInventoryWnd::GetSmallTabType( DWORD dwTabID, DWORD dwSmallTabID )
{
	int iArray = ( dwTabID - ID_SOLDIER_TAB_BTN );
	if( !COMPARE( iArray, 0, MAX_TAB_BTN ) )
		return -1;

	int iSmallArray = ( dwSmallTabID - ID_SMALL_TAB_1_BTN );
	if( !COMPARE( iSmallArray, 0, (int) m_vSmallTabTypeVecArray[iArray].size() ) )
		return -1;

	return m_vSmallTabTypeVecArray[iArray][iSmallArray];
}

bool MyInventoryWnd::IsRightSmallTab( DWORD dwTabID, DWORD dwSmallTabID, int iValue1 )
{
	if( dwTabID == ID_SOLDIER_TAB_BTN )
	{
		if( iValue1 != ETC_ITEM_OTHER_VALUE && dwSmallTabID == ID_SMALL_TAB_1_BTN ) 
			return true;
		else if( iValue1 == GetSmallTabType( dwTabID, dwSmallTabID ) )
			return true;
	}
	else if( dwTabID == ID_EXTRA_TAB_BTN )
	{
		if( dwSmallTabID == ID_SMALL_TAB_1_BTN )
			return true;
		else if( iValue1 == GetSmallTabType( dwTabID, dwSmallTabID ) )
			return true;
	}
	else if( dwTabID == ID_DECO_TAB_BTN )
	{
		if( dwSmallTabID == ID_SMALL_TAB_1_BTN )
			return true;
		else if( iValue1 == GetSmallTabType( dwTabID, dwSmallTabID ) )
			return true;
	}
	else if( dwTabID == ID_ETC_TAB_BTN )
	{
		if( dwSmallTabID == ID_SMALL_TAB_1_BTN )
			return true;
		else if( iValue1 == GetSmallTabType( dwTabID, dwSmallTabID ) )
			return true;
	}
	else if( dwTabID == ID_PRESENT_TAB_BTN )
	{
		if( iValue1 == GetSmallTabType( dwTabID, dwSmallTabID ) )
			return true;
	}
	else if( dwTabID == ID_COSTUME_TAB_BTN )
	{
		// UI코스튬 관련 (스몰 탭 확인)
		if( dwSmallTabID == ID_SMALL_TAB_1_BTN )
			return true;
		else if( iValue1 == GetSmallTabType( dwTabID, dwSmallTabID ) )
			return true;
	}
	else if( dwTabID == ID_ACCESSORY_TAB_BTN )
	{
		if( dwSmallTabID == ID_SMALL_TAB_1_BTN )
			return true;
		else if( iValue1 == GetSmallTabType( dwTabID, dwSmallTabID ) )
			return true;
	}

	return false;
}

void MyInventoryWnd::OnItemIconOver( DWORD dwID )
{
	InventoryItemWnd *pItem = dynamic_cast<InventoryItemWnd*>(FindChildWnd(dwID));
	if( pItem )
	{
		int iMainType = pItem->GetMainType();

		switch( iMainType )
		{
		case INVENTORY_ITEM_EXTRAITEM:
			break;
		case INVENTORY_ITEM_DECO:
			break;
		case INVENTORY_ITEM_COSTUME:
			break;
		}
	}
}

void MyInventoryWnd::OnItemIconPush( DWORD dwID )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( !pInfoWnd ) return;

	ShowMainInfoWnd();

	InventoryItemWnd *pItem = dynamic_cast<InventoryItemWnd*>(FindChildWnd(dwID));
	if( pItem )
	{
		int iMainType = pItem->GetMainType();

		switch( iMainType )
		{
		case INVENTORY_ITEM_ETC:
		case INVENTORY_ITEM_PRESENT:
			pInfoWnd->SetInvenItemInfo( m_iSelectClassType, pItem->GetInfo() );
			break;
		}
	}
}

void MyInventoryWnd::OnItemEquipPush( DWORD dwID )
{
	InventoryItemWnd *pItem = dynamic_cast<InventoryItemWnd*>(FindChildWnd(dwID));
	if( pItem )
	{
		int iMainType = pItem->GetMainType();

		switch( iMainType )
		{
		case INVENTORY_ITEM_EXTRAITEM:
			{
				SetEquipExtraItem( dwID );
			}
			break;
		case INVENTORY_ITEM_MEDALITEM:
			{
				SetEquipMedalItem(dwID);
			}
			break;
		case INVENTORY_ITEM_DECO:
			{
				SetEquipDecoItem( dwID );
			}
			break;
		case INVENTORY_ITEM_COSTUME:
			{
				// UI코스튬 관련 (코스튬 장착)
				SetEquipCostume( dwID );
			}
			break;
		case INVENTORY_ITEM_ACCESSORY:
			{
				SetEquipAccessory( dwID );
			}
			break;
		}
	}
}

void MyInventoryWnd::OnItemReleasePush( DWORD dwID )
{
	InventoryItemWnd *pItem = dynamic_cast<InventoryItemWnd*>(FindChildWnd(dwID));
	if( pItem )
	{
		int iMainType = pItem->GetMainType();
		int iMagicCode = pItem->GetMagicCode();

		switch( iMainType )
		{
		case INVENTORY_ITEM_EXTRAITEM:
			{
				SetReleaseExtraItem(iMagicCode);
			}
			break;
		case INVENTORY_ITEM_DECO:
			{
				int iDecoType = pItem->GetDecoType();
				int iDecoCode = pItem->GetDecoCode();

				SetReleaseDecoItem( iDecoType, iDecoCode );
			}
			break;
		case INVENTORY_ITEM_MEDALITEM:
			{
				SetReleaseMedalItem( iMagicCode );
			}
			break;
		case INVENTORY_ITEM_COSTUME:
			{
				// UI코스튬 관련 (코스튬 해제)
				SetReleaseCostume(iMagicCode);
			}
			break;
		}
	}
}

void MyInventoryWnd::OnItemDetailPush( DWORD dwID )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( !pInfoWnd ) return;

	ShowMainInfoWnd();

	InventoryItemWnd *pItem = dynamic_cast<InventoryItemWnd*>(FindChildWnd(dwID));
	if( pItem )
	{
		int iMainType = pItem->GetMainType();

		switch( iMainType )
		{
		case INVENTORY_ITEM_ETC:
		case INVENTORY_ITEM_PRESENT:
		case INVENTORY_ITEM_SUBSCRIPTION:
			{
				pInfoWnd->SetInvenItemInfo( m_iSelectClassType, pItem->GetInfo() );
			}
			break;
		}
	}
}

void MyInventoryWnd::OnItemReSellPush( DWORD dwID )
{
	InventoryItemWnd *pItem = dynamic_cast<InventoryItemWnd*>(FindChildWnd(dwID));
	if( pItem )
	{
		int iMainType = pItem->GetMainType();

		switch( iMainType )
		{
		case INVENTORY_ITEM_EXTRAITEM:
			SetReSellExtraItem( dwID );
			break;
		case INVENTORY_ITEM_MEDALITEM:
			SetReSellMedalItem( dwID );
			break;
		case INVENTORY_ITEM_COSTUME:
			// UI코스튬 관련 (코스튬 판매)
			SetReSellCostume( dwID );
			break;
		case INVENTORY_ITEM_ACCESSORY:
			SetReSellAccessory( dwID );
			break;
		}
	}
}

void MyInventoryWnd::OnItemCustomInfoPush( DWORD dwID )
{
	InventoryItemWnd *pItem = dynamic_cast<InventoryItemWnd*>(FindChildWnd(dwID));
	if( pItem )
	{
		int iMainType  = pItem->GetMainType();
		int iMagicCode = pItem->GetMagicCode();
		int iMagicCodeEx = pItem->GetMagicCodeEx();

		switch( iMainType )
		{
		case INVENTORY_ITEM_EXTRAITEM:
			{
				// 장비 스킨 삭제 팝업
				ioUserExtraItem *pExtraItem = g_MyInfo.GetUserExtraItem();
				if( !pExtraItem ) return;

				EXTRAITEMSLOT kSlot;
				if( !pExtraItem->GetExtraItem( iMagicCode, kSlot ) )
				{
					if( g_App.GetConnectedMoveMode() == ioApplication::CMM_LOBBY )
					{
						g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
					}
					else
					{
						g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
					}
				}
				else
				{
					bool bError = false;
					if( !m_pPlayStage )
						bError = true;

					if( g_App.GetConnectedMoveMode() == ioApplication::CMM_LOBBY )
					{
						if( !bError && m_pPlayStage->GetModeType() != MT_MYROOM )
							bError = true;

						if( !bError && ioMyRoomMode::GetMyRoomType() != MRT_LOBBY )
							bError = true;
					}
					else
					{
						if( !bError && m_pPlayStage->GetModeType() != MT_TRAINING && m_pPlayStage->GetModeType() != MT_HEADQUARTERS&& m_pPlayStage->GetModeType() != MT_HOUSE )
							bError = true;
					}
					
					if( bError )
					{
						if( g_App.GetConnectedMoveMode() == ioApplication::CMM_LOBBY )
						{
							g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
						}
						else
						{
							g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
						}
					}
					else
					{
						ItemCustomDeleteWnd *pCustomDelWnd = dynamic_cast<ItemCustomDeleteWnd*>(g_GUIMgr.FindWnd( ITEM_CUSTOM_DELETE_WND ));
						if( pCustomDelWnd )
							pCustomDelWnd->SetSlotIndex( iMagicCode );
					}
				}
			}
			break;
		case INVENTORY_ITEM_PRESENT:
			{
				ioHashString szSendID;
				short sPresentType = 0;
				short sPresentMent = 0;
				int iPresentValue1 = 0;
				int iPresentValue2 = 0;
				int iPresentValue3 = 0;
				int iPresentValue4 = 0;
				DWORD dwLimitDate  = 0;
				g_PresentMgr.GetPresentDataToIndex( iMagicCode, iMagicCodeEx, szSendID,	sPresentType, sPresentMent, iPresentValue1, iPresentValue2, iPresentValue3, iPresentValue4, dwLimitDate );

				if( sPresentType == PRESENT_EXTRAITEM )
				{
					ItemCustomGlobalDetailViewWnd *pDetailViewWnd = dynamic_cast< ItemCustomGlobalDetailViewWnd * >( g_GUIMgr.FindWnd( ITEM_CUSTOM_GLOBAL_DETAIL_VIEW_WND ) );
					if( pDetailViewWnd )
					{
						pDetailViewWnd->CreateCharacter( iPresentValue1, iPresentValue3, iPresentValue4 );
					}
				}
				else if( sPresentType == PRESENT_COSTUME )
				{
					CostumeEquipInfo sCostumeEquipInfo;
					sCostumeEquipInfo.m_CostumeCode = iPresentValue1;
					sCostumeEquipInfo.m_Costume_Male_Custom = iPresentValue3;
					sCostumeEquipInfo.m_Costume_Female_Custom = iPresentValue4;

					CostumeCustomGlobalDetailViewWnd *pDetailViewWnd = dynamic_cast<CostumeCustomGlobalDetailViewWnd*>( g_GUIMgr.FindWnd( COSTUME_CUSTOM_GLOBAL_DETAIL_VIEW_WND ) );
					if( pDetailViewWnd )
						pDetailViewWnd->CreateCharacter( sCostumeEquipInfo );
				}
			}
			break;
		case INVENTORY_ITEM_COSTUME:
			{
				// 장비 스킨 삭제 팝업
				ioUserCostume *pCostume = g_MyInfo.GetUserCostume();
				if( !pCostume ) 
					return;

				CostumeSlot kSlot;
				if( !pCostume->GetCostumeSlot( iMagicCode, kSlot ) )
				{
					if( g_App.GetConnectedMoveMode() == ioApplication::CMM_LOBBY )
						g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
					else
						g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
				}
				else
				{
					bool bError = false;
					if( !m_pPlayStage )
						bError = true;

					if( g_App.GetConnectedMoveMode() == ioApplication::CMM_LOBBY )
					{
						if( !bError && m_pPlayStage->GetModeType() != MT_MYROOM )
							bError = true;

						if( !bError && ioMyRoomMode::GetMyRoomType() != MRT_LOBBY )
							bError = true;
					}
					else
					{
						if( !bError && m_pPlayStage->GetModeType() != MT_TRAINING && m_pPlayStage->GetModeType() != MT_HEADQUARTERS && m_pPlayStage->GetModeType() != MT_HOUSE )
							bError = true;
					}

					if( bError )
					{
						if( g_App.GetConnectedMoveMode() == ioApplication::CMM_LOBBY )
							g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
						else
							g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
					}
					else
					{
						CostumeCustomDeleteWnd *pCustomDelWnd = dynamic_cast<CostumeCustomDeleteWnd*>(g_GUIMgr.FindWnd( COSTUME_CUSTOM_DELETE_WND ));
						if( pCustomDelWnd )
							pCustomDelWnd->SetSlotIndex( iMagicCode );
					}
				}
			}
			break;
		}
	}
}

void MyInventoryWnd::OnItemDisassemblePush( DWORD dwID )
{
	InventoryItemWnd *pItem = dynamic_cast<InventoryItemWnd*>(FindChildWnd(dwID));
	if( pItem )
	{
		int iMainType = pItem->GetMainType();
		switch( iMainType )
		{
		case INVENTORY_ITEM_EXTRAITEM:
			SetDisassembleExtraItem( dwID );
			break;
		case INVENTORY_ITEM_COSTUME:
			// UI코스튬 관련 (코스튬 분해)
			SetDisassembleCostume( dwID );
			break;
		}
	}
}

bool MyInventoryWnd::SetEquipExtraItem( DWORD dwID )
{
	InventoryItemWnd *pInvenItem = dynamic_cast<InventoryItemWnd*>(FindChildWnd(dwID));
	if( !pInvenItem ) return false;

	// 오른쪽 창으로부터 캐릭터 정보 가져와야함.
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( !pInfoWnd ) return false;

	ShowMainInfoWnd();
	pInfoWnd->SetInvenItemInfo( m_iSelectClassType, pInvenItem->GetInfo() );

	CHARACTER rkCharInfo = pInfoWnd->GetCharInfo();

	int iNewSlotIndex = pInvenItem->GetMagicCode();
	if( iNewSlotIndex <= 0 )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		LOG.PrintTimeAndLog( 0, "MyInventoryWnd::SetEquipExtraItem Wrong Btn : %d", dwID );
		return false;
	}

	int iCharArray = g_MyInfo.GetClassArray( rkCharInfo.m_class_type );
	if( !COMPARE( iCharArray, 0, g_MyInfo.GetCharCount() ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		LOG.PrintTimeAndLog( 0, "MyInventoryWnd::SetEquipExtraItem Wrong Char Array : %d", iCharArray );
		return false;
	}

	if( g_MyInfo.IsCharRentalToClassType( rkCharInfo.m_class_type ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(11) );
		return false;
	}

	if( g_MyInfo.IsCharExerciseStyleToClassType( rkCharInfo.m_class_type, EXERCISE_RENTAL ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(12) );
		return false;
	}	

	ioUserExtraItem *pExtraItem = g_MyInfo.GetUserExtraItem();
	if( !pExtraItem )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		LOG.PrintTimeAndLog( 0, "MyInventoryWnd::SetEquipExtraItem Don't Have UserExtraItem" );
		return false;
	}

	EXTRAITEMSLOT kSlot;
	if( !pExtraItem->GetExtraItem( iNewSlotIndex, kSlot ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		LOG.PrintTimeAndLog( 0, "MyInventoryWnd::SetEquipExtraItem Not Exist Item : %d", iNewSlotIndex );
		return false;
	}

	if( kSlot.m_bCharEquip )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
		LOG.PrintTimeAndLog( 0, "MyInventoryWnd::SetEquipExtraItem Equiped Item : %d", iNewSlotIndex );
		return false;
	}

	const ioItem *pItem = g_ItemMaker.GetItemConst( kSlot.m_iItemCode, __FUNCTION__ );
	if( !pItem )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6) );
		LOG.PrintTimeAndLog( 0, "MyInventoryWnd::SetEquipExtraItem Not Exist Item : %d", kSlot.m_iItemCode );
		return false;
	}

	// 클레스 체크
	bool bDisableClass = false;
	int iEnableClass = pItem->GetEnableClass();
	if( iEnableClass > 0 && rkCharInfo.m_class_type != iEnableClass )
	{
		bDisableClass = true;
	}

	// 성별 체크
	bool bDisableSex = false;
	int iOnlySetType = pItem->GetCharOnlySexType();
	if( iOnlySetType != ioSetItemInfo::CST_ALL && rkCharInfo.m_sex != iOnlySetType )
	{
		bDisableSex = true;
	}

	if( bDisableClass || bDisableSex )
	{
		if( iEnableClass > 0 )
		{
			ioHashString szClassName = g_MyInfo.GetClassName( iEnableClass );

			ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
			kPrinter[0].SetTextStyle( TS_NORMAL );
			kPrinter[0].SetBkColor( 0, 0, 0 );	
			kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
			kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(13) );

			kPrinter[1].SetTextStyle( TS_NORMAL );
			kPrinter[1].SetBkColor( 0, 0, 0 );
			kPrinter[1].SetTextColor( TCT_DEFAULT_DARKGRAY );

			switch( iOnlySetType )
			{
			case ioSetItemInfo::CST_ALL:
				kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(14), szClassName.c_str() );
				break;
			case ioSetItemInfo::CST_MALE:
				kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(15), szClassName.c_str() );
				break;
			case ioSetItemInfo::CST_FEMALE:
				kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(16), szClassName.c_str() );
				break;
			}

			g_GUIMgr.SetPrevMsgListBox( NULL, MB_OK, NULL, kPrinter );
		}
		else
		{
			ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
			kPrinter[0].SetTextStyle( TS_NORMAL );
			kPrinter[0].SetBkColor( 0, 0, 0 );	
			kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
			kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(17) );

			kPrinter[1].SetTextStyle( TS_NORMAL );
			kPrinter[1].SetBkColor( 0, 0, 0 );	
			kPrinter[1].SetTextColor( TCT_DEFAULT_DARKGRAY );

			switch( iOnlySetType )
			{
			case ioSetItemInfo::CST_ALL:
				kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(18) );
				break;
			case ioSetItemInfo::CST_MALE:
				kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(19) );
				break;
			case ioSetItemInfo::CST_FEMALE:
				kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(20) );
				break;
			}

			g_GUIMgr.SetPrevMsgListBox( NULL, MB_OK, NULL, kPrinter );
		}

		return false;
	}
	
	// 가능 레벨 체크
	int iEnableLevel = kSlot.m_iReinforce * g_ExtraItemInfoMgr.GetLevelLimitConst();
	int iCurLevel = g_MyInfo.GetClassLevel(rkCharInfo.m_class_type,true);
	if( iCurLevel < iEnableLevel )
	{
		ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
		kPrinter[0].SetTextStyle( TS_NORMAL );
		kPrinter[0].SetBkColor( 0, 0, 0 );	
		kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
		kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(9) );

		kPrinter[1].SetTextStyle( TS_NORMAL );
		kPrinter[1].SetBkColor( 0, 0, 0 );	
		kPrinter[1].SetTextColor( TCT_DEFAULT_GRAY ); // 노란
		kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(10), iEnableLevel );

		g_GUIMgr.SetPrevMsgListBox( NULL, MB_OK, NULL, kPrinter );
		return false;
	}


	// 제한 시간 체크
	DWORD dwCurTime = FRAMEGETTIME();
	DWORD dwGap = dwCurTime - m_dwExtraItemChangeTime;
	if( dwGap < 500 )
	{
		return false;
	}

	//강화 등급별 장착여부 체크
	if( iEnableClass > 0 )
	{
		ioHashString szClassName;
		bool bChangeFail = false;
		int eTargetCharGrade = ConvertPowerUpItemToChar( ConvertPowerUpItemToItemGrade( kSlot.m_iItemCode ) );		
		EquipItemExtendType eExtendType = GetEquipItemExtendType( kSlot.m_iItemCode );

		if( eExtendType == EIET_EXTRA_POWERUP )
		{
			if( !g_PowerUpManager.CheckEnableEquipPowerUpItem( rkCharInfo.m_eCharPowerUpType, kSlot.m_iItemCode ) )
			{
				bChangeFail	= true;
				szClassName = g_MyInfo.GetClassName( iEnableClass, eTargetCharGrade );
			}
		}
		else if(  eExtendType == EIET_BASIC_EXTREA || eExtendType == EIET_SPECIAL_EXTREA )
		{		
			if( eTargetCharGrade != rkCharInfo.m_eCharPowerUpType )
			{
				bChangeFail = true;
				szClassName = g_MyInfo.GetClassName( iEnableClass );
			}
		}

		if( bChangeFail )
		{
			ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
			kPrinter[0].SetTextStyle( TS_NORMAL );
			kPrinter[0].SetBkColor( 0, 0, 0 );	
			kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
			kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(13) );

			kPrinter[1].SetTextStyle( TS_NORMAL );
			kPrinter[1].SetBkColor( 0, 0, 0 );
			kPrinter[1].SetTextColor( TCT_DEFAULT_DARKGRAY );
			kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(21), szClassName.c_str() );

			g_GUIMgr.SetPrevMsgListBox( NULL, MB_OK, NULL, kPrinter );
			return false;
		}
	}

	// 봉인, 봉인가능 체크
	int iSlot = kSlot.m_iItemCode / DEFAULT_BASIC_ITEM_CODE;
	if( kSlot.m_iTradeState != EET_DISABLE )
	{
		ExtraItemEquipConfirmWnd *pConfirmWnd = dynamic_cast<ExtraItemEquipConfirmWnd*>(g_GUIMgr.FindWnd(EXTRAITEM_EQUIPL_CONFIRM_WND));
		if( pConfirmWnd && pConfirmWnd->SetExtraItemEquipInfo( iCharArray, iSlot, iNewSlotIndex ) )
		{
			m_dwExtraItemChangeTime = dwCurTime;
			return true;
		}
		return false;
	}

	// 출전중인 용병만
	if( iCharArray == g_MyInfo.GetSelectCharArray() )
	{
		if( m_pPlayStage && !m_pPlayStage->IsModeStateResult() )
		{
			ioBaseChar *pOwner = m_pPlayStage->GetOwnerChar();
			if( pOwner )
			{
				// 장비 교체 가능상태 체크
				if( !pOwner->SetExtraItemChangeState( false ) )
					return false;
			}
		}
	}

	TCPNetwork::MouseBusy( true );

	SP2Packet kPacket( CTPK_EXTRAITEM_CHANGE );
	kPacket << iCharArray;
	kPacket << iSlot;
	kPacket << iNewSlotIndex;
	TCPNetwork::SendToServer( kPacket );

	g_MyInfo.SetExtraItemChange( 1, iCharArray, iSlot, iNewSlotIndex );
	g_QuestMgr.QuestCompleteTerm( QC_EXTRA_ITEM_EQUIP, kSlot.m_iItemCode, kSlot.m_PeriodType );

	return true;
}

bool MyInventoryWnd::SetEquipDecoItem( DWORD dwID )
{
	InventoryItemWnd *pInvenItem = dynamic_cast<InventoryItemWnd*>(FindChildWnd(dwID));
	if( !pInvenItem ) return false;

	// 오른쪽 창으로부터 캐릭터 정보 가져와야함.
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( !pInfoWnd ) return false;
	if( !pInfoWnd->IsShow() ) return false;

	ShowMainInfoWnd();
	pInfoWnd->SetInvenItemInfo( m_iSelectClassType, pInvenItem->GetInfo() );

	CHARACTER rkCharInfo = pInfoWnd->GetCharInfo();

	int iDecoType = pInvenItem->GetDecoType();
	int iDecoCode = pInvenItem->GetDecoCode();

	int iCharArray = g_MyInfo.GetClassArray( rkCharInfo.m_class_type );
	if( !COMPARE( iCharArray, 0, g_MyInfo.GetCharCount() ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		LOG.PrintTimeAndLog( 0, "MyInventoryWnd::SetEquipDecoItem Wrong Char Array : %d", iCharArray );
		return false;
	}

	if( g_MyInfo.IsCharRentalToClassType( rkCharInfo.m_class_type ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
		return false;
	}

	if( g_MyInfo.IsCharExerciseStyleToClassType( rkCharInfo.m_class_type, EXERCISE_RENTAL ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6) );
		return false;
	}

	if( iDecoType == UID_KINDRED && !EnableDecoChangeByEquipItem( rkCharInfo ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(7) );
		return false;
	}

	// 출전중인 용병만
	if( iCharArray == g_MyInfo.GetSelectCharArray() )
	{
		if( m_pPlayStage )
		{
			ioBaseChar *pOwner = m_pPlayStage->GetOwnerChar();
			if( pOwner )
			{
				if( pOwner->IsObejctEquipState() )
				{
					g_ChatMgr.SetSystemMsg( STR(2) );
					return false;
				}

				if( pOwner->IsInvisibleHead() )
				{
					g_ChatMgr.SetSystemMsg( STR(2) );
					return false;
				}

				if( pOwner->HasBuff( BT_CHANGE_SKIN_COLOR ) )
				{
					g_ChatMgr.SetSystemMsg( STR(2) );
					return false;
				}

				if( pOwner->HasBuff( BT_CHANGE_MESH ) )
				{
					g_ChatMgr.SetSystemMsg( STR(2) );
					return false;
				}

				int iCharState = (int)pOwner->GetState();
				if ( iCharState != CS_DELAY )
					return false;

				/*
				switch( iCharState )
				{
				case CS_USING_SKILL:
				case CS_NOT_MOVE_PROTECT:
					g_ChatMgr.SetSystemMsg( STR(3) );
					return false;
				case CS_STOP_MOTION:
					g_ChatMgr.SetSystemMsg( STR(4) );
					return false;
				}
				*/

				if( !pOwner->EnableDecoChangeByEquipItem( iDecoType ) )
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(8) );
					return false;
				}
			}
		}
	}

	if( iDecoType == 0 )
		return false;

	//고유 특성 1111 22 333 ( 세트, 종족(성별), 치장타입 )
	int iType = (rkCharInfo.m_class_type * 100000) + ((rkCharInfo.m_sex - 1) * 1000);
	int iCode = 0;
	switch( iDecoType )
	{
	case UID_FACE:
	case UID_HAIR:
	case UID_SKIN_COLOR:
	case UID_HAIR_COLOR:
	case UID_UNDERWEAR:
	case UID_KINDRED:
		{
			iType += iDecoType;
			iCode =  iDecoCode;
		}
		break;
	case UID_CLASS:
		{
			int iEquipType = iDecoCode / 1000;
			int iEquipCode = iDecoCode % 1000;
			iType += iEquipType;
			iCode  = iEquipCode;
		}
		break;
	}

	TCPNetwork::MouseBusy( true );
	SP2Packet kPacket( CTPK_EQUIP_SLOT_ITEM );
	kPacket << EQUIP_SLOT_ITEM_STATE;
	kPacket << iCharArray;
	kPacket << iType << iCode;
	TCPNetwork::SendToServer( kPacket );

	return true;
}

bool MyInventoryWnd::SetEquipMedalItem( DWORD dwID )
{
	InventoryItemWnd *pInvenItem = dynamic_cast<InventoryItemWnd*>(FindChildWnd(dwID));
	if( !pInvenItem ) return false;

	// 오른쪽 창으로부터 캐릭터 정보 가져와야함.
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( !pInfoWnd ) return false;

	ShowMainInfoWnd();
	pInfoWnd->SetInvenItemInfo( m_iSelectClassType, pInvenItem->GetInfo() );

	CHARACTER rkCharInfo = pInfoWnd->GetCharInfo();

	int iMedalItemType = pInvenItem->GetMagicCode();
	if( iMedalItemType <= 0 )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		LOG.PrintTimeAndLog( 0, "MyInventoryWnd::SetEquipMedalItem Wrong Btn : %d", dwID );
		return false;
	}

	int iCharArray = g_MyInfo.GetClassArray( rkCharInfo.m_class_type );
	if( !COMPARE( iCharArray, 0, g_MyInfo.GetCharCount() ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		LOG.PrintTimeAndLog( 0, "MyInventoryWnd::SetEquipMedalItem Wrong Char Array : %d", iCharArray );
		return false;
	}

	ioUserMedalItem *pMedalItem = g_MyInfo.GetUserMedalItem();
	if( !pMedalItem )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		LOG.PrintTimeAndLog( 0, "MyInventoryWnd::SetEquipMedalItem Don't Have UserMedalItem" );
		return false;
	}

	ioUserMedalItem::MEDALITEMSLOT kSlot;
	if( !pMedalItem->GetMedalItem( iMedalItemType, kSlot, true ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		LOG.PrintTimeAndLog( 0, "MyInventoryWnd::SetEquipMedalItem Not Exist Item : %d", iMedalItemType );
		return false;
	}

	if( kSlot.m_iEquipClass != 0 )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
		LOG.PrintTimeAndLog( 0, "MyInventoryWnd::SetEquipMedalItem Equiped Item : %d", iMedalItemType );
		return false;
	}

	const ioMedalItemInfoManager::ItemInfo *pItem = g_MedalItemMgr.GetItemInfo( iMedalItemType );
	if( !pItem )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6) );
		LOG.PrintTimeAndLog( 0, "MyInventoryWnd::SetEquipMedalItem Not Exist Item : %d", iMedalItemType );
		return false;
	}

	// 클레스 체크
	if( !g_MedalItemMgr.IsRight( iMedalItemType, rkCharInfo.m_class_type) )
	{
		ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
		kPrinter[0].SetTextStyle( TS_NORMAL );
		kPrinter[0].SetBkColor( 0, 0, 0 );	
		kPrinter[0].SetTextColor( TCT_DEFAULT_GRAY );
		kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(7) );

		g_GUIMgr.SetPrevMsgListBox( NULL, MB_OK, NULL, kPrinter );
		return false;
	}

	if( g_MyInfo.IsCharRentalToClassType( rkCharInfo.m_class_type ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(11) );
		return false;
	}

	if( g_MyInfo.IsCharExerciseStyleToClassType( rkCharInfo.m_class_type, EXERCISE_RENTAL ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(12) );
		return false;
	}	

	// 가능 레벨 체크
	int iEnableLevel = g_MedalItemMgr.GetLevelLimit( iMedalItemType );
	int iCurLevel = g_MyInfo.GetClassLevel( rkCharInfo.m_class_type, true );
	if( iCurLevel < iEnableLevel )
	{
		ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
		kPrinter[0].SetTextStyle( TS_NORMAL );
		kPrinter[0].SetBkColor( 0, 0, 0 );	
		kPrinter[0].SetTextColor( TCT_DEFAULT_GRAY );
		kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(8) );

		kPrinter[1].SetTextStyle( TS_NORMAL );
		kPrinter[1].SetBkColor( 0, 0, 0 );	
		kPrinter[1].SetTextColor( TCT_DEFAULT_GRAY ); // 노란
		kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(9), iEnableLevel );

		g_GUIMgr.SetPrevMsgListBox( NULL, MB_OK, NULL, kPrinter );
		return false;
	}

	IntVec vItemTypeVec;
	g_MyInfo.GetMedalItemTypeVec( vItemTypeVec, rkCharInfo.m_class_type );
	int iEquipMedalItemCnt = vItemTypeVec.size();
	if( iEquipMedalItemCnt >= ioMedalItemInfoManager::MAX_SLOT_NUM || 
		iEquipMedalItemCnt >= g_MedalItemMgr.GetSlotNum( iCurLevel ) + g_ExpandMedalSlotMgr.GetExpandMedalSlotNum( rkCharInfo.m_class_type ) )
	{
		ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
		kPrinter[0].SetTextStyle( TS_NORMAL );
		kPrinter[0].SetBkColor( 0, 0, 0 );	
		kPrinter[0].SetTextColor( TCT_DEFAULT_GRAY );
		kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(10) );

		g_GUIMgr.SetPrevMsgListBox( NULL, MB_OK, NULL, kPrinter );
		return false;
	}


	// 제한 시간 체크
	DWORD dwCurTime = FRAMEGETTIME();
	DWORD dwGap = dwCurTime - m_dwMedalItemChangeTime;
	if( dwGap < 500 )
	{
		return false;
	}

	m_dwMedalItemChangeTime = dwCurTime;

	TCPNetwork::MouseBusy( true );

	SP2Packet kPacket( CTPK_MEDALITEM_CHANGE );
	kPacket << iCharArray;
	kPacket << iMedalItemType;
	kPacket << true;
	TCPNetwork::SendToServer( kPacket );

	return true;
}

// UI코스튬 관련 (코스튬 장착)
bool MyInventoryWnd::SetEquipCostume( DWORD dwID )
{
	InventoryItemWnd *pInvenItem = dynamic_cast<InventoryItemWnd*>(FindChildWnd(dwID));
	if( !pInvenItem ) 
		return false;

	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( !pInfoWnd ) 
		return false;

	ShowMainInfoWnd();
	pInfoWnd->SetInvenItemInfo( m_iSelectClassType, pInvenItem->GetInfo() );

	CHARACTER rkCharInfo = pInfoWnd->GetCharInfo();

	int nNewSlotIndex = pInvenItem->GetMagicCode();
	if( nNewSlotIndex <= 0 )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return false;
	}

	int nCharArray = g_MyInfo.GetClassArray( rkCharInfo.m_class_type );
	if( !COMPARE( nCharArray, 0, g_MyInfo.GetCharCount() ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return false;
	}

	if( g_MyInfo.IsCharRentalToClassType( rkCharInfo.m_class_type ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		return false;
	}

	if( g_MyInfo.IsCharExerciseStyleToClassType( rkCharInfo.m_class_type, EXERCISE_RENTAL ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		return false;
	}	

	ioUserCostume *pCostume = g_MyInfo.GetUserCostume();
	if( !pCostume )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
		return false;
	}

	CostumeSlot kSlot;
	if( !pCostume->GetCostumeSlot( nNewSlotIndex, kSlot ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6) );
		return false;
	}

	if( kSlot.m_bEquip )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(7) );
		return false;
	}

	CostumeInfo sCostumeInfo;
	if( !g_CostumeInfoMgr.GetCostumeInfo( kSlot.m_nCostumeCode, sCostumeInfo ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(8) );
		return false;
	}

	// 클레스 체크
	if( sCostumeInfo.m_nEnableClass > 0 && 
		rkCharInfo.m_class_type != sCostumeInfo.m_nEnableClass )
	{
		if( sCostumeInfo.m_nEnableClass > 0 )
		{
			ioHashString szClassName = g_MyInfo.GetClassName( sCostumeInfo.m_nEnableClass, rkCharInfo.m_eCharPowerUpType );

			ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
			kPrinter[0].SetTextStyle( TS_NORMAL );
			kPrinter[0].SetBkColor( 0, 0, 0 );	
			kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
			kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(9) );

			kPrinter[1].SetTextStyle( TS_NORMAL );
			kPrinter[1].SetBkColor( 0, 0, 0 );
			kPrinter[1].SetTextColor( TCT_DEFAULT_DARKGRAY );
			kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(10), szClassName.c_str() );

			g_GUIMgr.SetPrevMsgListBox( NULL, MB_OK, NULL, kPrinter );
		}
		else
		{
			ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
			kPrinter[0].SetTextStyle( TS_NORMAL );
			kPrinter[0].SetBkColor( 0, 0, 0 );	
			kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
			kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(11) );

			kPrinter[1].SetTextStyle( TS_NORMAL );
			kPrinter[1].SetBkColor( 0, 0, 0 );	
			kPrinter[1].SetTextColor( TCT_DEFAULT_DARKGRAY );
			kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(12) );

			g_GUIMgr.SetPrevMsgListBox( NULL, MB_OK, NULL, kPrinter );
		}

		return false;
	}

	// 제한 시간 체크
	DWORD dwCurTime = FRAMEGETTIME();
	DWORD dwGap = dwCurTime - m_dwExtraItemChangeTime;
	if( dwGap < 500 )
	{
		return false;
	}
	
	// 출전중인 용병만
	if( nCharArray == g_MyInfo.GetSelectCharArray() )
	{
		if( m_pPlayStage && !m_pPlayStage->IsModeStateResult() )
		{
			ioBaseChar *pOwner = m_pPlayStage->GetOwnerChar();
			if( pOwner )
			{
				// 장비 교체 가능상태 체크
				if( !pOwner->SetExtraItemChangeState( false ) )
					return false;
			}
		}
	}
	
	m_dwExtraItemChangeTime = dwCurTime;

	// UI코스튬 관련 (장착 패킷 전송)
	TCPNetwork::MouseBusy( true );	
	SP2Packet kPacket( CTPK_COSTUME_CHANGE );
	kPacket << g_MyInfo.GetCharIndex(nCharArray);
	kPacket << (byte)(kSlot.m_nCostumeCode/DEFAULT_BASIC_COSTUME_CODE);
#ifdef CUSTOM_COSTUME
	kPacket << (DWORD)kSlot.m_dwMaleCustom;
	kPacket << (DWORD)kSlot.m_dwFemaleCustom;
#endif
	kPacket << nNewSlotIndex;
	kPacket << true;
	TCPNetwork::SendToServer( kPacket );
	return true;
}

bool MyInventoryWnd::SetEquipAccessory( DWORD dwID )
{
	InventoryItemWnd *pInvenItem = dynamic_cast<InventoryItemWnd*>(FindChildWnd(dwID));
	if( !pInvenItem )
		return false;

	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( !pInfoWnd )
		return false;

	ShowMainInfoWnd();
	pInfoWnd->SetInvenItemInfo( m_iSelectClassType, pInvenItem->GetInfo() );

	CHARACTER rkCharInfo = pInfoWnd->GetCharInfo();

	int iNewSlotIndex = pInvenItem->GetMagicCode();
	if( iNewSlotIndex <= 0 )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return false;
	}

	int iCharArray = g_MyInfo.GetClassArray( rkCharInfo.m_class_type );
	if( !COMPARE( iCharArray, 0, g_MyInfo.GetCharCount() ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return false;
	}

	if( g_MyInfo.IsCharRentalToClassType( rkCharInfo.m_class_type ) )
	{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		return false;
	}

	if( g_MyInfo.IsCharExerciseStyleToClassType( rkCharInfo.m_class_type, EXERCISE_RENTAL ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		return false;
	}	

	ioUserAccessory *pAccessory = g_MyInfo.GetUserAccessory();
	if( !pAccessory )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
		return false;
	}

	AccessorySlot kSlot;
	if( !pAccessory->GetAccessorySlot( iNewSlotIndex, kSlot ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6) );
		return false;
	}

	if( kSlot.m_bEquip )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(7) );
		return false;
	}

	const ioItem *pItem = g_ItemMaker.GetItemConst( kSlot.m_iAccessoryCode, __FUNCTION__ );
	if( !pItem )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(8) );
		return false;
	}

	// 클레스 체크
	bool bDisableClass = false;
	int iEnableClass = pItem->GetEnableClass();
	if( iEnableClass > 0 && rkCharInfo.m_class_type != iEnableClass )
	{
		bDisableClass = true;
	}

	// 성별 체크
	bool bDisableSex = false;
	int iOnlySetType = pItem->GetCharOnlySexType();
	if( iOnlySetType != ioSetItemInfo::CST_ALL && rkCharInfo.m_sex != iOnlySetType )
	{
		bDisableSex = true;
	}

	if( bDisableClass || bDisableSex )
	{
		if( iEnableClass > 0 )
		{
			ioHashString szClassName = g_MyInfo.GetClassName( iEnableClass );

			ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
			kPrinter[0].SetTextStyle( TS_NORMAL );
			kPrinter[0].SetBkColor( 0, 0, 0 );	
			kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
			kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(9) );

			kPrinter[1].SetTextStyle( TS_NORMAL );
			kPrinter[1].SetBkColor( 0, 0, 0 );
			kPrinter[1].SetTextColor( TCT_DEFAULT_DARKGRAY );

			switch( iOnlySetType )
			{
			case ioSetItemInfo::CST_ALL:
				kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(10), szClassName.c_str() );
				break;
			case ioSetItemInfo::CST_MALE:
				kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(11), szClassName.c_str() );
				break;
			case ioSetItemInfo::CST_FEMALE:
				kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(12), szClassName.c_str() );
				break;
			}

			g_GUIMgr.SetPrevMsgListBox( NULL, MB_OK, NULL, kPrinter );
		}
		else
		{
			ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
			kPrinter[0].SetTextStyle( TS_NORMAL );
			kPrinter[0].SetBkColor( 0, 0, 0 );	
			kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
			kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(13) );

			kPrinter[1].SetTextStyle( TS_NORMAL );
			kPrinter[1].SetBkColor( 0, 0, 0 );	
			kPrinter[1].SetTextColor( TCT_DEFAULT_DARKGRAY );

			switch( iOnlySetType )
			{
			case ioSetItemInfo::CST_ALL:
				kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(14) );
				break;
			case ioSetItemInfo::CST_MALE:
				kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(15) );
				break;
			case ioSetItemInfo::CST_FEMALE:
				kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(16) );
				break;
			}

			g_GUIMgr.SetPrevMsgListBox( NULL, MB_OK, NULL, kPrinter );
		}

		return false;
	}

	// 제한 시간 체크
	DWORD dwCurTime = FRAMEGETTIME();
	DWORD dwGap = dwCurTime - m_dwExtraItemChangeTime;
	if( dwGap < 500 )
	{
		return false;
	}

	// 출전중인 용병만
	if( iCharArray == g_MyInfo.GetSelectCharArray() )
	{
		if( m_pPlayStage && !m_pPlayStage->IsModeStateResult() )
		{
			ioBaseChar *pOwner = m_pPlayStage->GetOwnerChar();
			if( pOwner )
			{
				// 장비 교체 가능상태 체크
				if( !pOwner->SetExtraItemChangeState( false ) )
					return false;
			}
		}
	}

	int iSlot = kSlot.m_iAccessoryCode / DEFAULT_BASIC_ACCESSORY_CODE - 1;
	if ( !COMPARE( iSlot, 0, MAX_ACC_INVENTORY ) )
		return false;
	
	m_dwExtraItemChangeTime = dwCurTime;

	TCPNetwork::MouseBusy( true );
	SP2Packet kPacket( CTPK_ACCESSORY_CHANGE );
	kPacket << g_MyInfo.GetCharIndex(iCharArray);
	kPacket << (BYTE)iSlot;
	kPacket << iNewSlotIndex;
	kPacket << true;
	TCPNetwork::SendToServer( kPacket );

	return true;
}

bool MyInventoryWnd::SetReleaseDecoItem( int iDecoType, int iDecoCode )
{
	// 오른쪽 창으로부터 캐릭터 정보 가져와야함.
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( !pInfoWnd ) return false;

	CHARACTER rkCharInfo = pInfoWnd->GetCharInfo();

	int iCharArray = g_MyInfo.GetClassArray( rkCharInfo.m_class_type );
	if( !COMPARE( iCharArray, 0, g_MyInfo.GetCharCount() ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		LOG.PrintTimeAndLog( 0, "MyInventoryWnd::SetReleaseDecoItem Wrong Char Array : %d", iCharArray );
		return false;
	}

	if( g_MyInfo.IsCharRentalToClassType( rkCharInfo.m_class_type ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		return false;
	}

	if( g_MyInfo.IsCharExerciseStyleToClassType( rkCharInfo.m_class_type, EXERCISE_RENTAL ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		return false;
	}

	if( iDecoType == UID_KINDRED && !EnableDecoChangeByEquipItem( rkCharInfo ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
		return false;
	}

	// 출전중인 용병만
	if( iCharArray == g_MyInfo.GetSelectCharArray() )
	{
		if( m_pPlayStage )
		{
			ioBaseChar *pOwner = m_pPlayStage->GetOwnerChar();
			if( pOwner )
			{
				if( pOwner->IsObejctEquipState() )
				{
					g_ChatMgr.SetSystemMsg( STR(2) );
					return false;
				}

				if( pOwner->IsInvisibleHead() )
				{
					g_ChatMgr.SetSystemMsg( STR(2) );
					return false;
				}

				if( pOwner->HasBuff( BT_CHANGE_SKIN_COLOR ) )
				{
					g_ChatMgr.SetSystemMsg( STR(2) );
					return false;
				}

				if( pOwner->HasBuff( BT_CHANGE_MESH ) )
				{
					g_ChatMgr.SetSystemMsg( STR(2) );
					return false;
				}

				if( !pOwner->EnableDecoChangeByEquipItem( iDecoType ) )
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6) );
					return false;
				}
			}
		}
	}

	if( iDecoType == 0 )
		return false;

	//고유 특성 1111 22 333 ( 세트, 종족(성별), 치장타입 )
	int iType = (rkCharInfo.m_class_type * 100000) + ((rkCharInfo.m_sex - 1) * 1000);
	int iCode = 0;
	switch( iDecoType )
	{
	case UID_FACE:
	case UID_HAIR:
	case UID_SKIN_COLOR:
	case UID_HAIR_COLOR:
	case UID_UNDERWEAR:
	case UID_KINDRED:
		{
			iType += iDecoType;
			iCode =  iDecoCode;
		}
		break;
	case UID_CLASS:
		{
			int iEquipType = iDecoCode / 1000;
			int iEquipCode = iDecoCode % 1000;
			iType += iEquipType;
			iCode  = iEquipCode;
		}
		break;
	}

	TCPNetwork::MouseBusy( true );
	SP2Packet kPacket( CTPK_EQUIP_SLOT_ITEM );
	kPacket << RELEASE_SLOT_ITEM_STATE;
	kPacket << iCharArray;
	kPacket << iType << iCode;
	TCPNetwork::SendToServer( kPacket );

	return true;
}

bool MyInventoryWnd::SetReleaseExtraItem( int iSlotIndex )
{
	// 오른쪽 창으로부터 캐릭터 정보 가져와야함.
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( !pInfoWnd ) return false;

	CHARACTER rkCharInfo = pInfoWnd->GetCharInfo();

	int iCharArray = g_MyInfo.GetClassArray( rkCharInfo.m_class_type );
	if( !COMPARE( iCharArray, 0, g_MyInfo.GetCharCount() ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		LOG.PrintTimeAndLog( 0, "MyInventoryWnd::SetReleaseExtraItem Wrong Char Array : %d", iCharArray );
		return false;
	}

	if( g_MyInfo.IsCharRentalToClassType( rkCharInfo.m_class_type ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		return false;
	}

	if( g_MyInfo.IsCharExerciseStyleToClassType( rkCharInfo.m_class_type, EXERCISE_RENTAL ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
		return false;
	}	

	ioUserExtraItem *pExtraItem = g_MyInfo.GetUserExtraItem();
	if( !pExtraItem )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		LOG.PrintTimeAndLog( 0, "MyInventoryWnd::SetReleaseExtraItem Don't Have UserExtraItem" );
		return false;
	}

	EXTRAITEMSLOT kSlot;
	if( !pExtraItem->GetExtraItem( iSlotIndex, kSlot ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		LOG.PrintTimeAndLog( 0, "MyInventoryWnd::SetReleaseExtraItem Not Exist Item : %d", iSlotIndex );
		return false;
	}

	// 제한 시간 체크
	DWORD dwCurTime = FRAMEGETTIME();
	DWORD dwGap = dwCurTime - m_dwExtraItemChangeTime;
	if( dwGap < 500 )
	{
		return false;
	}

	// 출전중인 용병만
	if( iCharArray == g_MyInfo.GetSelectCharArray() )
	{
		if( m_pPlayStage && !m_pPlayStage->IsModeStateResult() )
		{
			ioBaseChar *pOwner = m_pPlayStage->GetOwnerChar();
			if( pOwner )
			{
				if( !pOwner->SetExtraItemChangeState( false ) )
				{
					return false;
				}
			}
		}
	}

	int iSlot = kSlot.m_iItemCode / DEFAULT_BASIC_ITEM_CODE;
	m_dwExtraItemChangeTime = dwCurTime;

	TCPNetwork::MouseBusy( true );

	SP2Packet kPacket( CTPK_EXTRAITEM_CHANGE );
	kPacket << iCharArray;
	kPacket << iSlot;
	kPacket << 0;
	TCPNetwork::SendToServer( kPacket );

	g_MyInfo.SetExtraItemChange( 2, iCharArray, iSlot, 0 );
	return true;
}

bool MyInventoryWnd::SetReleaseMedalItem( int iMedalItemType )
{
	// 오른쪽 창으로부터 캐릭터 정보 가져와야함.
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( !pInfoWnd ) return false;

	CHARACTER rkCharInfo = pInfoWnd->GetCharInfo();

	int iCharArray = g_MyInfo.GetClassArray( rkCharInfo.m_class_type );
	if( !COMPARE( iCharArray, 0, g_MyInfo.GetCharCount() ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		LOG.PrintTimeAndLog( 0, "MyInventoryWnd::SetReleaseMedalItem Wrong Char Array : %d", iCharArray );
		return false;
	}

	if( g_MyInfo.IsCharRentalToClassType( rkCharInfo.m_class_type ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		return false;
	}

	if( g_MyInfo.IsCharExerciseStyleToClassType( rkCharInfo.m_class_type, EXERCISE_RENTAL ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
		return false;
	}	

	ioUserMedalItem *pMedalItem = g_MyInfo.GetUserMedalItem();
	if( !pMedalItem )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		LOG.PrintTimeAndLog( 0, "MyInventoryWnd::SetReleaseMedalItem Don't Have UserMedalItem" );
		return false;
	}

	ioUserMedalItem::MEDALITEMSLOT kSlot;
	if( !pMedalItem->GetMedalItem( iMedalItemType, kSlot ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		LOG.PrintTimeAndLog( 0, "MyInventoryWnd::SetReleaseMedalItem Not Exist Item : %d", iMedalItemType );
		return false;
	}

	// 제한 시간 체크
	DWORD dwCurTime = FRAMEGETTIME();
	DWORD dwGap = dwCurTime - m_dwMedalItemChangeTime;
	if( dwGap < 500 )
	{
		return false;
	}

	m_dwMedalItemChangeTime = dwCurTime;

	TCPNetwork::MouseBusy( true );

	SP2Packet kPacket( CTPK_MEDALITEM_CHANGE );
	kPacket << iCharArray;
	kPacket << iMedalItemType;
	kPacket << false;
	TCPNetwork::SendToServer( kPacket );

	return true;
}

// UI코스튬 관련 (코스튬 해제)
bool MyInventoryWnd::SetReleaseCostume( int iSlotIndex )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( !pInfoWnd ) 
		return false;

	CHARACTER rkCharInfo = pInfoWnd->GetCharInfo();

	int nCharArray = g_MyInfo.GetClassArray( rkCharInfo.m_class_type );
	if( !COMPARE( nCharArray, 0, g_MyInfo.GetCharCount() ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return false;
	}

	if( g_MyInfo.IsCharRentalToClassType( rkCharInfo.m_class_type ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return false;
	}

	if( g_MyInfo.IsCharExerciseStyleToClassType( rkCharInfo.m_class_type, EXERCISE_RENTAL ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		return false;
	}	

	ioUserCostume *pCostume = g_MyInfo.GetUserCostume();
	if( !pCostume )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		return false;
	}

	CostumeSlot kSlot;
	if( !pCostume->GetCostumeSlot( iSlotIndex, kSlot ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
		return false;
	}

	// 제한 시간 체크
	DWORD dwCurTime = FRAMEGETTIME();
	DWORD dwGap = dwCurTime - m_dwExtraItemChangeTime;
	if( dwGap < 500 )
	{
		return false;
	}

	// 출전중인 용병만
	if( nCharArray == g_MyInfo.GetSelectCharArray() )
	{
		if( m_pPlayStage && !m_pPlayStage->IsModeStateResult() )
		{
			ioBaseChar *pOwner = m_pPlayStage->GetOwnerChar();
			if( pOwner )
			{
				if( !pOwner->SetExtraItemChangeState( false ) )
					return false;
			}
		}
	}

	m_dwExtraItemChangeTime = dwCurTime;

	// UI코스튬 관련 (장착 패킷 전송)
	TCPNetwork::MouseBusy( true );
	SP2Packet kPacket( CTPK_COSTUME_CHANGE );
	kPacket << g_MyInfo.GetCharIndex(nCharArray);
	kPacket << (byte)(kSlot.m_nCostumeCode/DEFAULT_BASIC_COSTUME_CODE);
#ifdef CUSTOM_COSTUME
	kPacket << (DWORD)kSlot.m_dwMaleCustom;
	kPacket << (DWORD)kSlot.m_dwFemaleCustom;
#endif
	kPacket << iSlotIndex;
	kPacket << false;
	TCPNetwork::SendToServer( kPacket );

	return true;
}

bool MyInventoryWnd::SetReleaseAccessory( int iSlotIndex )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( !pInfoWnd ) 
		return false;

	CHARACTER rkCharInfo = pInfoWnd->GetCharInfo();

	int nCharArray = g_MyInfo.GetClassArray( rkCharInfo.m_class_type );
	if( !COMPARE( nCharArray, 0, g_MyInfo.GetCharCount() ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return false;
	}

	if( g_MyInfo.IsCharRentalToClassType( rkCharInfo.m_class_type ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return false;
	}

	if( g_MyInfo.IsCharExerciseStyleToClassType( rkCharInfo.m_class_type, EXERCISE_RENTAL ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		return false;
	}	

	ioUserAccessory *pAccessory = g_MyInfo.GetUserAccessory();
	if( !pAccessory )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		return false;
	}

	AccessorySlot kSlot;
	if( !pAccessory->GetAccessorySlot( iSlotIndex, kSlot ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
		return false;
	}

	// 제한 시간 체크
	DWORD dwCurTime = FRAMEGETTIME();
	DWORD dwGap = dwCurTime - m_dwExtraItemChangeTime;
	if( dwGap < 500 )
	{
		return false;
	}

	// 출전중인 용병만
	if( nCharArray == g_MyInfo.GetSelectCharArray() )
	{
		if( m_pPlayStage && !m_pPlayStage->IsModeStateResult() )
		{
			ioBaseChar *pOwner = m_pPlayStage->GetOwnerChar();
			if( pOwner )
			{
				if( !pOwner->SetExtraItemChangeState( false ) )
					return false;
			}
		}
	}

	int iSlot = kSlot.m_iAccessoryCode / DEFAULT_BASIC_ACCESSORY_CODE - 1;
	if ( !COMPARE( iSlot, 0, MAX_ACC_INVENTORY ) )
		return false;

	m_dwExtraItemChangeTime = dwCurTime;

	// UI코스튬 관련 (장착 패킷 전송)
	TCPNetwork::MouseBusy( true );
	SP2Packet kPacket( CTPK_ACCESSORY_CHANGE );
	kPacket << g_MyInfo.GetCharIndex(nCharArray);
	kPacket << (BYTE)iSlot;
	kPacket << iSlotIndex;
	kPacket << false;
	TCPNetwork::SendToServer( kPacket );

	return true;
}

bool MyInventoryWnd::SetReSellExtraItem( DWORD dwID )
{
	InventoryItemWnd *pInvenItem = dynamic_cast<InventoryItemWnd*>(FindChildWnd(dwID));
	if( !pInvenItem ) return false;

	int iNewSlotIndex = pInvenItem->GetMagicCode();
	if( iNewSlotIndex <= 0 )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		LOG.PrintTimeAndLog( 0, "MyInventoryWnd::SetReSellExtraItem Wrong Btn : %d", dwID );
		return false;
	}

	ioUserExtraItem *pExtraItem = g_MyInfo.GetUserExtraItem();
	if( !pExtraItem )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		LOG.PrintTimeAndLog( 0, "MyInventoryWnd::SetReSellExtraItem Don't Have UserExtraItem" );
		return false;
	}

	EXTRAITEMSLOT kSlot;
	if( !pExtraItem->GetExtraItem( iNewSlotIndex, kSlot ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		LOG.PrintTimeAndLog( 0, "MyInventoryWnd::SetReSellExtraItem Not Exist Item : %d", iNewSlotIndex );
		return false;
	}

	if( kSlot.m_bCharEquip )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		LOG.PrintTimeAndLog( 0, "MyInventoryWnd::SetReSellExtraItem Item : %d", iNewSlotIndex );
		return false;
	}

	if( !CheckTimeEndExtraItem( iNewSlotIndex ) )
		return false;

	m_iSellSlotIndex = iNewSlotIndex;

	float fReturnPeso = 0.0f;
	DWORD dwEditType = SELL_EDIT_NONE;
	if( kSlot.m_PeriodType == EPT_MORTMAIN )
	{
		fReturnPeso = g_ExtraItemInfoMgr.GetMortmainItemSellPesoReinforce( kSlot.m_iReinforce );
		dwEditType = SELL_EDIT_EXTRAITEM;
	}
	else
	{
		time_t kLimitTime = DateHelp::ConvertSecondTime( kSlot.GetYear(), kSlot.GetMonth(), kSlot.GetDay(), kSlot.GetHour(), kSlot.GetMinute(), 0 );
		time_t kCurServerTime = g_MyInfo.GetServerDate();
		kCurServerTime += ( REALGETTIME()/FLOAT1000 );// 현재까지의 클라이언트초를 넣는다.

		int iGapSec = kLimitTime - kCurServerTime;
		int iTotal = iGapSec / 60;

		fReturnPeso = g_ExtraItemInfoMgr.GetTimeItemSellPesoReinforce( iTotal, kSlot.m_iReinforce );
		fReturnPeso = max( 0, fReturnPeso );
	}

	__int64 iPeso = (__int64)fReturnPeso;

	char szConvertNum[MAX_PATH] = "";
	Help::ConvertNumToStrComma( iPeso, szConvertNum, sizeof( szConvertNum ) );

	ioHashString sInfo;
	pExtraItem->GetUseInfo( iNewSlotIndex, sInfo );

	if( sInfo.IsEmpty() )
	{
		LOG.PrintTimeAndLog( 0, "%s don't have , Use Info:%d", __FUNCTION__, iNewSlotIndex );
	}

	ioComplexStringPrinter kPrinter[ItemResellWnd::MAX_INFO_LINE];

	// ItemInfo
	kPrinter[0].SetTextStyle( TS_NORMAL );
	kPrinter[0].SetBkColor( 0, 0, 0 );
	kPrinter[0].SetTextColor( TCT_DEFAULT_RED );
	kPrinter[0].AddTextPiece( FONT_SIZE_17, STR(5), kSlot.m_Name.c_str() );

	kPrinter[1].SetTextStyle( TS_NORMAL );
	kPrinter[1].SetBkColor( 0, 0, 0 );
	kPrinter[1].SetTextColor( TCT_DEFAULT_RED );
	kPrinter[1].AddTextPiece( FONT_SIZE_17, STR(6), sInfo.c_str() );

	// Desc
	kPrinter[2].SetTextStyle( TS_NORMAL );
	kPrinter[2].SetBkColor( 0, 0, 0 );
	kPrinter[2].SetTextColor( TCT_DEFAULT_GRAY );
	kPrinter[2].AddTextPiece( FONT_SIZE_13, STR(7) );

	kPrinter[3].SetTextStyle( TS_NORMAL );
	kPrinter[3].SetBkColor( 0, 0, 0 );
	kPrinter[3].SetTextColor( TCT_DEFAULT_GRAY );
	kPrinter[3].AddTextPiece( FONT_SIZE_13, STR(8) );
	kPrinter[3].SetTextColor( TCT_DEFAULT_BLUE );
	kPrinter[3].AddTextPiece( FONT_SIZE_13, STR(9), szConvertNum );

	kPrinter[4].SetTextStyle( TS_NORMAL );
	kPrinter[4].SetBkColor( 0, 0, 0 );
	kPrinter[4].SetTextColor( TCT_DEFAULT_GRAY );
	kPrinter[4].AddTextPiece( FONT_SIZE_13, STR(10) );

	ItemResellWnd *pResellWnd = dynamic_cast<ItemResellWnd*>(g_GUIMgr.FindWnd(ITEM_RESELL_WND));
	if( pResellWnd )
	{
		int nGradeType = 0;
		ioUIRenderImage *pImg = g_UIImageSetMgr.CreateImageByFullName( kSlot.m_IconName );		
		const ioItem *pItem = g_ItemMaker.GetItemConst( kSlot.m_iItemCode, __FUNCTION__ );
		if( pItem )
			nGradeType = pItem->GetGradeType();

		pResellWnd->SetInfoAndShow( STR(11), pImg, kSlot.m_iReinforce, kPrinter, this, dwEditType, ACST_SELL, false, nGradeType );
	}

	return true;
}

bool MyInventoryWnd::SetReSellMedalItem( DWORD dwID )
{
	InventoryItemWnd *pInvenItem = dynamic_cast<InventoryItemWnd*>(FindChildWnd(dwID));
	if( !pInvenItem ) return false;

	int iMedalIndex = pInvenItem->GetMagicCode();
	if( iMedalIndex <= 0 )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		LOG.PrintTimeAndLog( 0, "MyInventoryWnd::SetReSellMedalItem Wrong Btn : %d", dwID );
		return false;
	}

	ioUserMedalItem *pMedalItem = g_MyInfo.GetUserMedalItem();
	if( !pMedalItem )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		LOG.PrintTimeAndLog( 0, "MyInventoryWnd::SetReSellMedalItem Don't Have UserMedalItem" );
		return false;
	}
	
	ioUserMedalItem::MEDALITEMSLOT kSlot;
	if( !pMedalItem->GetMedalItem( iMedalIndex, kSlot ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		LOG.PrintTimeAndLog( 0, "MyInventoryWnd::SetReSellMedalItem Not Exist Item : %d", iMedalIndex );
		return false;
	}
	
	if( !CheckTimeEndMedalItem( kSlot ) )
		return false;

	m_iSellSlotIndex = iMedalIndex;

	float fReturnPeso = 0.0f;
	DWORD dwEditType = SELL_EDIT_NONE;
	if( kSlot.m_iPeriodType == ioUserMedalItem::PT_MORTMAIN )
	{
		fReturnPeso = g_MedalItemMgr.GetSellPeso( kSlot.m_iItemType );
		dwEditType = SELL_EDIT_EXTRAITEM;
	}
	else
	{
		fReturnPeso = g_MedalItemMgr.GetSellPeso( kSlot.m_iItemType );
		fReturnPeso = fReturnPeso / g_MedalItemMgr.GetSellPesoByMinute();

		time_t kLimitTime = DateHelp::ConvertSecondTime( kSlot.GetYear(), kSlot.GetMonth(), kSlot.GetDay(), kSlot.GetHour(), kSlot.GetMinute(), 0 );
		time_t kCurServerTime = g_MyInfo.GetServerDate();
		kCurServerTime += ( REALGETTIME()/FLOAT1000 );// 현재까지의 클라이언트초를 넣는다.

		int iGapSec = kLimitTime - kCurServerTime;
		int iTotal = iGapSec / 60;

		fReturnPeso = iTotal * fReturnPeso;
		fReturnPeso = min( fReturnPeso, g_MedalItemMgr.GetSellPeso( kSlot.m_iItemType ) );
	}

	int iPeso = static_cast<int>(fReturnPeso);

	char szConvertNum[MAX_PATH] = "";
	Help::ConvertNumToStrComma( iPeso, szConvertNum, sizeof( szConvertNum ) );

	ioHashString sInfo;
	pMedalItem->GetUseInfo( iMedalIndex, sInfo );

	if( sInfo.IsEmpty() )
	{
		LOG.PrintTimeAndLog( 0, "%s don't have , Use Info:%d", __FUNCTION__, iMedalIndex );
	}

	ioComplexStringPrinter kPrinter[ItemResellWnd::MAX_INFO_LINE];

	const ioMedalItemInfoManager::ItemInfo *kMedalInfo = g_MedalItemMgr.GetItemInfo( kSlot.m_iItemType );
	if( !kMedalInfo )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		LOG.PrintTimeAndLog( 0, "MyInventoryWnd::SetReSellMedalItem Not Exist Item : %d", kSlot.m_iItemType );
		return false;
	}

	// ItemInfo
	kPrinter[0].SetTextStyle( TS_NORMAL );
	kPrinter[0].SetBkColor( 0, 0, 0 );
	kPrinter[0].SetTextColor( TCT_DEFAULT_RED );
	kPrinter[0].AddTextPiece( FONT_SIZE_17, STR(5), kMedalInfo->m_sName.c_str() );

	kPrinter[1].SetTextStyle( TS_NORMAL );
	kPrinter[1].SetBkColor( 0, 0, 0 );
	kPrinter[1].SetTextColor( TCT_DEFAULT_RED );
	kPrinter[1].AddTextPiece( FONT_SIZE_17, STR(6), sInfo.c_str() );

	// Desc
	kPrinter[2].SetTextStyle( TS_NORMAL );
	kPrinter[2].SetBkColor( 0, 0, 0 );
	kPrinter[2].SetTextColor( TCT_DEFAULT_GRAY );
	kPrinter[2].AddTextPiece( FONT_SIZE_13, STR(7) );

	kPrinter[3].SetTextStyle( TS_NORMAL );
	kPrinter[3].SetBkColor( 0, 0, 0 );
	kPrinter[3].SetTextColor( TCT_DEFAULT_GRAY );
	kPrinter[3].AddTextPiece( FONT_SIZE_13, STR(8) );
	kPrinter[3].SetTextColor( TCT_DEFAULT_BLUE );
	kPrinter[3].AddTextPiece( FONT_SIZE_13, STR(9), szConvertNum );

	kPrinter[4].SetTextStyle( TS_NORMAL );
	kPrinter[4].SetBkColor( 0, 0, 0 );
	kPrinter[4].SetTextColor( TCT_DEFAULT_GRAY );
	kPrinter[4].AddTextPiece( FONT_SIZE_13, STR(10) );

	ItemResellWnd *pResellWnd = dynamic_cast<ItemResellWnd*>(g_GUIMgr.FindWnd(ITEM_RESELL_WND));
	if( pResellWnd )
	{
		ioUIRenderImage *pImg = g_UIImageSetMgr.CreateImageByFullName( kMedalInfo->m_sIcon );
		pResellWnd->SetInfoAndShow( STR(11), pImg, 0, kPrinter, this, dwEditType, ACST_SELL_MEDAL, false );
	}

	return true;
}

// UI코스튬 관련(되팔기)
bool MyInventoryWnd::SetReSellCostume( DWORD dwID )
{
	InventoryItemWnd *pInvenItem = dynamic_cast<InventoryItemWnd*>(FindChildWnd(dwID));
	if( !pInvenItem ) 
		return false;

	int iNewSlotIndex = pInvenItem->GetMagicCode();
	if( iNewSlotIndex <= 0 )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		LOG.PrintTimeAndLog( 0, "MyInventoryWnd::SetReSellCostume Wrong Btn : %d", dwID );
		return false;
	}

	ioUserCostume *pCostume = g_MyInfo.GetUserCostume();
	if( !pCostume )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		LOG.PrintTimeAndLog( 0, "MyInventoryWnd::SetReSellCostume Don't Have UserExtraItem" );
		return false;
	}

	CostumeSlot kSlot;
	if( !pCostume->GetCostumeSlot( iNewSlotIndex, kSlot ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		LOG.PrintTimeAndLog( 0, "MyInventoryWnd::SetReSellCostume Not Exist Item : %d", iNewSlotIndex );
		return false;
	}

	if( kSlot.m_bEquip )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		LOG.PrintTimeAndLog( 0, "MyInventoryWnd::SetReSellCostume Item : %d", iNewSlotIndex );
		return false;
	}

	if( !CheckTimeEndCostume( iNewSlotIndex ) )
		return false;

	m_iSellSlotIndex = iNewSlotIndex;

	float fReturnPeso = 0.0f;
	DWORD dwEditType = SELL_EDIT_NONE;
	if( kSlot.m_PeriodType == PCPT_MORTMAIN )
	{
		fReturnPeso = g_CostumeShopInfoMgr.GetResellMortmainCostumePeso();
		dwEditType = SELL_EDIT_EXTRAITEM;
	}
	else
		fReturnPeso = g_CostumeShopInfoMgr.GetResellTimeCostumeTimePeso();

	__int64 iPeso = (__int64)fReturnPeso;

	char szConvertNum[MAX_PATH] = "";
	Help::ConvertNumToStrComma( iPeso, szConvertNum, sizeof( szConvertNum ) );

	ioHashString sInfo;
	pCostume->GetUseInfo( iNewSlotIndex, sInfo );

	if( sInfo.IsEmpty() )
	{
		LOG.PrintTimeAndLog( 0, "%s don't have , Use Info:%d", __FUNCTION__, iNewSlotIndex );
	}

	ioComplexStringPrinter kPrinter[ItemResellWnd::MAX_INFO_LINE];

	// ItemInfo
	kPrinter[0].SetTextStyle( TS_NORMAL );
	kPrinter[0].SetBkColor( 0, 0, 0 );
	kPrinter[0].SetTextColor( TCT_DEFAULT_RED );
	kPrinter[0].AddTextPiece( FONT_SIZE_17, STR(5), kSlot.m_Name.c_str() );

	kPrinter[1].SetTextStyle( TS_NORMAL );
	kPrinter[1].SetBkColor( 0, 0, 0 );
	kPrinter[1].SetTextColor( TCT_DEFAULT_RED );
	kPrinter[1].AddTextPiece( FONT_SIZE_17, STR(6), sInfo.c_str() );

	// Desc
	kPrinter[2].SetTextStyle( TS_NORMAL );
	kPrinter[2].SetBkColor( 0, 0, 0 );
	kPrinter[2].SetTextColor( TCT_DEFAULT_GRAY );
	kPrinter[2].AddTextPiece( FONT_SIZE_13, STR(7) );

	kPrinter[3].SetTextStyle( TS_NORMAL );
	kPrinter[3].SetBkColor( 0, 0, 0 );
	kPrinter[3].SetTextColor( TCT_DEFAULT_GRAY );
	kPrinter[3].AddTextPiece( FONT_SIZE_13, STR(8) );
	kPrinter[3].SetTextColor( TCT_DEFAULT_BLUE );
	kPrinter[3].AddTextPiece( FONT_SIZE_13, STR(9), szConvertNum );

	kPrinter[4].SetTextStyle( TS_NORMAL );
	kPrinter[4].SetBkColor( 0, 0, 0 );
	kPrinter[4].SetTextColor( TCT_DEFAULT_GRAY );
	kPrinter[4].AddTextPiece( FONT_SIZE_13, STR(10) );

	ItemResellWnd *pResellWnd = dynamic_cast<ItemResellWnd*>(g_GUIMgr.FindWnd(ITEM_RESELL_WND));
	if( pResellWnd )
	{
		ioUIRenderImage *pImg = g_UIImageSetMgr.CreateImageByFullName( kSlot.m_IconName );
		pResellWnd->SetInfoAndShow( STR(11), pImg, 0, kPrinter, this, dwEditType, ACST_SELL_COSTUME, true );
	}

	return true;
}

bool MyInventoryWnd::SetEtcItemUse( int iType )
{
	ioEtcItem *pEtcItem = g_EtcItemMgr.FindEtcItem( iType );
	if( !pEtcItem )
	{
		LOG.PrintTimeAndLog( 0, "%s pEtcItem == NULL : %d", __FUNCTION__, iType );
		return false;
	}

	ioUserEtcItem *pUserEtcItem =  g_MyInfo.GetUserEtcItem();
	if( !pUserEtcItem )
	{
		LOG.PrintTimeAndLog( 0, "%s UserEtcItem == NULL : %d", __FUNCTION__, iType );
		return false;
	}

	pEtcItem->OnUseBtnPush( iType, pUserEtcItem );

	return true;
}

bool MyInventoryWnd::SetDisassembleExtraItem( DWORD dwID )
{
	InventoryItemWnd *pInvenItem = dynamic_cast<InventoryItemWnd*>(FindChildWnd(dwID));
	if( !pInvenItem ) return false;

	int iNewSlotIndex = pInvenItem->GetMagicCode();
	if( iNewSlotIndex <= 0 )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(13) );
		return false;
	}

	ioUserExtraItem *pExtraItem = g_MyInfo.GetUserExtraItem();
	if( !pExtraItem )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(14) );
		return false;
	}

	EXTRAITEMSLOT kSlot;
	if( !pExtraItem->GetExtraItem( iNewSlotIndex, kSlot ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(15) );
		return false;
	}

	if( kSlot.m_bCharEquip )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(16) );
		return false;
	}

	if( !CheckTimeEndExtraItem( iNewSlotIndex ) )
		return false;

	m_iSellSlotIndex = iNewSlotIndex;

	int iMinCnt = 0;
	int iMaxCnt = 0;
	DWORD dwEditType = SELL_EDIT_NONE;
	if( kSlot.m_PeriodType == EPT_MORTMAIN )
	{
		//영구장비 분해 텍스트 입력
		dwEditType = SELL_EDIT_EXTRAITEM;
		//14.04.09 텍스트 입력 제거
		//dwEditType = SELL_EDIT_NONE;
	}
	else
	{
		time_t kLimitTime = DateHelp::ConvertSecondTime( kSlot.GetYear(), kSlot.GetMonth(), kSlot.GetDay(), kSlot.GetHour(), kSlot.GetMinute(), 0 );
		time_t kCurServerTime = g_MyInfo.GetServerDate();
		kCurServerTime += ( REALGETTIME()/FLOAT1000 );// 현재까지의 클라이언트초를 넣는다.

		int iGapSec = kLimitTime - kCurServerTime;
		int iTotal = iGapSec / 60 / 60 / 24;

		iMinCnt = g_AlchemicMgr.GetDisassembleMinCnt( ADT_EXTRAITEM, false, iTotal, kSlot.m_iReinforce );
		iMaxCnt = g_AlchemicMgr.GetDisassembleMaxCnt( ADT_EXTRAITEM, false, iTotal, kSlot.m_iReinforce );

		if( iMinCnt <= 0 || iMaxCnt <= 0 )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(17) );
			return false;
		}
	}

	int iGainCode = g_AlchemicMgr.GetDisassembleCode( ADT_EXTRAITEM, kSlot.m_iItemCode );

	// ui 처리
	ioHashString sInfo;
	pExtraItem->GetUseInfo( iNewSlotIndex, sInfo );

	ioComplexStringPrinter kPrinter[ItemResellWnd::MAX_INFO_LINE];

	// ItemInfo
	kPrinter[0].SetTextStyle( TS_NORMAL );
	kPrinter[0].SetBkColor( 0, 0, 0 );
	kPrinter[0].SetTextColor( TCT_DEFAULT_RED );
	kPrinter[0].AddTextPiece( FONT_SIZE_17, STR(18), kSlot.m_Name.c_str() );

	kPrinter[1].SetTextStyle( TS_NORMAL );
	kPrinter[1].SetBkColor( 0, 0, 0 );
	kPrinter[1].SetTextColor( TCT_DEFAULT_RED );
	kPrinter[1].AddTextPiece( FONT_SIZE_17, STR(19), sInfo.c_str() );

	// Desc
	kPrinter[2].SetTextStyle( TS_NORMAL );
	kPrinter[2].SetBkColor( 0, 0, 0 );
	kPrinter[2].SetTextColor( TCT_DEFAULT_GRAY );
	kPrinter[2].AddTextPiece( FONT_SIZE_13, STR(20) );

	kPrinter[3].SetTextStyle( TS_NORMAL );
	kPrinter[3].SetBkColor( 0, 0, 0 );
	kPrinter[3].SetTextColor( TCT_DEFAULT_GRAY );
	kPrinter[3].AddTextPiece( FONT_SIZE_13, STR(21) );
	kPrinter[3].SetTextColor( TCT_DEFAULT_BLUE );

	if( iGainCode > ALCHEMIC_ADDITIVE_DIV )
		kPrinter[3].AddTextPiece( FONT_SIZE_13, STR(22), g_AlchemicMgr.GetAlchemicItemName(iGainCode).c_str() );
	else
		kPrinter[3].AddTextPiece( FONT_SIZE_13, STR(23), g_MyInfo.GetClassName(iGainCode) );

	ItemResellWnd *pResellWnd = dynamic_cast<ItemResellWnd*>(g_GUIMgr.FindWnd(ITEM_RESELL_WND));
	if( pResellWnd )
	{
		int nGradeType = 0;
		ioUIRenderImage *pImg = g_UIImageSetMgr.CreateImageByFullName( kSlot.m_IconName );		
		const ioItem *pItem = g_ItemMaker.GetItemConst( kSlot.m_iItemCode, __FUNCTION__ );
		if( pItem )
			nGradeType = pItem->GetGradeType();

		pResellWnd->SetInfoAndShow( STR(24), pImg, kSlot.m_iReinforce, kPrinter, this, dwEditType, ACST_DISASSEMBLE, false, nGradeType );
	}

	return true;
}

// UI코스튬 관련 (코스튬 분해)
bool MyInventoryWnd::SetDisassembleCostume( DWORD dwID )
{
	InventoryItemWnd *pInvenItem = dynamic_cast<InventoryItemWnd*>(FindChildWnd(dwID));
	if( !pInvenItem ) 
		return false;

	int iNewSlotIndex = pInvenItem->GetMagicCode();
	if( iNewSlotIndex <= 0 )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return false;
	}

	ioUserCostume *pCostume = g_MyInfo.GetUserCostume();
	if( !pCostume )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return false;
	}

	CostumeSlot kSlot;
	if( !pCostume->GetCostumeSlot( iNewSlotIndex, kSlot ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		return false;
	}

	if( kSlot.m_bEquip )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		return false;
	}

	if( !CheckTimeEndCostume( iNewSlotIndex ) )
		return false;

	m_iSellSlotIndex = iNewSlotIndex;

	int iMinCnt = 0;
	int iMaxCnt = 0;
	DWORD dwEditType = SELL_EDIT_NONE;
	if( kSlot.m_PeriodType == PCPT_MORTMAIN )
	{
		// 텍스트 입력 제거
		dwEditType = SELL_EDIT_NONE;
	}
	else
	{
		time_t kLimitTime = DateHelp::ConvertSecondTime( kSlot.GetYear(), kSlot.GetMonth(), kSlot.GetDay(), kSlot.GetHour(), kSlot.GetMinute(), 0 );
		time_t kCurServerTime = g_MyInfo.GetServerDate();
		kCurServerTime += ( REALGETTIME()/1000 );// 현재까지의 클라이언트초를 넣는다.

		int iGapSec = kLimitTime - kCurServerTime;
		int iTotal = iGapSec / 60 / 60 / 24;

		//분해시 최소, 최대 개수
		iMinCnt = g_CostumeInfoMgr.GetDisassembleMinCnt( kSlot.m_nCostumeCode, iTotal );
		iMaxCnt = g_CostumeInfoMgr.GetDisassembleMaxCnt( kSlot.m_nCostumeCode, iTotal );

		if( iMinCnt <= 0 || iMaxCnt <= 0 )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
			return false;
		}
	}

	int iGainCode = g_CostumeInfoMgr.GetDisassembleCode( kSlot.m_nCostumeCode );

	// ui 처리
	ioHashString sInfo;
	pCostume->GetUseInfo( iNewSlotIndex, sInfo );

	ioComplexStringPrinter kPrinter[ItemResellWnd::MAX_INFO_LINE];

	// ItemInfo
	kPrinter[0].SetTextStyle( TS_NORMAL );
	kPrinter[0].SetBkColor( 0, 0, 0 );
	kPrinter[0].SetTextColor( TCT_DEFAULT_RED );
	kPrinter[0].AddTextPiece( FONT_SIZE_17, STR(6), kSlot.m_Name.c_str() );

	kPrinter[1].SetTextStyle( TS_NORMAL );
	kPrinter[1].SetBkColor( 0, 0, 0 );
	kPrinter[1].SetTextColor( TCT_DEFAULT_RED );
	kPrinter[1].AddTextPiece( FONT_SIZE_17, STR(7), sInfo.c_str() );

	// Desc
	kPrinter[2].SetTextStyle( TS_NORMAL );
	kPrinter[2].SetBkColor( 0, 0, 0 );
	kPrinter[2].SetTextColor( TCT_DEFAULT_GRAY );
	kPrinter[2].AddTextPiece( FONT_SIZE_13, STR(8) );

	kPrinter[3].SetTextStyle( TS_NORMAL );
	kPrinter[3].SetBkColor( 0, 0, 0 );
	kPrinter[3].SetTextColor( TCT_DEFAULT_GRAY );
	kPrinter[3].AddTextPiece( FONT_SIZE_13, STR(9) );
	kPrinter[3].SetTextColor( TCT_DEFAULT_BLUE );

	if( iGainCode > ALCHEMIC_ADDITIVE_DIV )
		kPrinter[3].AddTextPiece( FONT_SIZE_13, STR(10), g_AlchemicMgr.GetAlchemicItemName(iGainCode).c_str() );
	else
		kPrinter[3].AddTextPiece( FONT_SIZE_13, STR(11), g_MyInfo.GetClassName(iGainCode) );

	ItemResellWnd *pResellWnd = dynamic_cast<ItemResellWnd*>(g_GUIMgr.FindWnd(ITEM_RESELL_WND));
	if( pResellWnd )
	{
		ioUIRenderImage *pImg = g_UIImageSetMgr.CreateImageByFullName( kSlot.m_IconName );
		pResellWnd->SetInfoAndShow( STR(12), pImg, 0, kPrinter, this, dwEditType, ACST_DISASSEMBLE, true );
	}

	return true;
}

bool MyInventoryWnd::CheckTimeEndExtraItem( int iSlotIndex )
{
	ioUserExtraItem *pExtraItem = g_MyInfo.GetUserExtraItem();
	if( pExtraItem )
	{
		EXTRAITEMSLOT kSlot;
		if( pExtraItem->GetExtraItem( iSlotIndex, kSlot ) )
		{
			if( kSlot.m_PeriodType != EPT_MORTMAIN )
			{
				time_t kLimitTime = DateHelp::ConvertSecondTime( kSlot.GetYear(), kSlot.GetMonth(), kSlot.GetDay(), kSlot.GetHour(), kSlot.GetMinute(), 0 );
				time_t kCurServerTime = g_MyInfo.GetServerDate();
				kCurServerTime += ( REALGETTIME()/FLOAT1000 );// 현재까지의 클라이언트초를 넣는다.

				int iGapSec = kLimitTime - kCurServerTime;
				enum { DAY_SEC = 86400, HOUR_SEC = 3600, MINUTE_SEC = 60, };
				int iDay    = iGapSec/DAY_SEC;
				int iHourSec= iGapSec-(iDay*DAY_SEC);
				int iHour   = iHourSec / HOUR_SEC;
				int iMinute = ( iHourSec-(iHour*HOUR_SEC) ) / MINUTE_SEC;

				if( iDay <= 0 && iHour <= 0 && iMinute <= 1 )
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
					return false;
				}
			}
		}
	}
	else
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return false;
	}

	return true;
}

bool MyInventoryWnd::CheckTimeEndMedalItem( ioUserMedalItem::MEDALITEMSLOT kSlot )
{
	if( kSlot.m_iPeriodType != EPT_MORTMAIN )
	{
		time_t kLimitTime = DateHelp::ConvertSecondTime( kSlot.GetYear(), kSlot.GetMonth(), kSlot.GetDay(), kSlot.GetHour(), kSlot.GetMinute(), 0 );
		time_t kCurServerTime = g_MyInfo.GetServerDate();
		kCurServerTime += ( REALGETTIME()/FLOAT1000 );// 현재까지의 클라이언트초를 넣는다.

		int iGapSec = kLimitTime - kCurServerTime;
		enum { DAY_SEC = 86400, HOUR_SEC = 3600, MINUTE_SEC = 60, };
		int iDay    = iGapSec/DAY_SEC;
		int iHourSec= iGapSec-(iDay*DAY_SEC);
		int iHour   = iHourSec / HOUR_SEC;
		int iMinute = ( iHourSec-(iHour*HOUR_SEC) ) / MINUTE_SEC;

		if( iDay <= 0 && iHour <= 0 && iMinute <= 1 )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
			return false;
		}
	}

	return true;
}

// UI코스튬 관련 (기간제 시간 체크)
bool MyInventoryWnd::CheckTimeEndCostume( int iSlotIndex )
{
	ioUserCostume *pCostume = g_MyInfo.GetUserCostume();
	if( pCostume )
	{
		CostumeSlot kSlot;
		if( pCostume->GetCostumeSlot( iSlotIndex, kSlot ) )
		{
			if( kSlot.m_PeriodType != PCPT_MORTMAIN )
			{
				time_t kLimitTime = DateHelp::ConvertSecondTime( kSlot.GetYear(), kSlot.GetMonth(), kSlot.GetDay(), kSlot.GetHour(), kSlot.GetMinute(), 0 );
				time_t kCurServerTime = g_MyInfo.GetServerDate();
				kCurServerTime += ( REALGETTIME()/1000 );// 현재까지의 클라이언트초를 넣는다.

				int iGapSec = kLimitTime - kCurServerTime;
				enum { DAY_SEC = 86400, HOUR_SEC = 3600, MINUTE_SEC = 60, };
				int iDay    = iGapSec/DAY_SEC;
				int iHourSec= iGapSec-(iDay*DAY_SEC);
				int iHour   = iHourSec / HOUR_SEC;
				int iMinute = ( iHourSec-(iHour*HOUR_SEC) ) / MINUTE_SEC;

				if( iDay <= 0 && iHour <= 0 && iMinute <= 1 )
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
					return false;
				}
			}
		}
	}
	else
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return false;
	}

	return true;
}

void MyInventoryWnd::CheckDefaultActionBtn()
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
	{
		pInfoWnd->CheckDefaultActionBtn();
	}
}

void MyInventoryWnd::SetEtcItemUseBtn( const InventoryItemInfo &rkInfo )
{
	int iType = rkInfo.m_iMagicCode;

	/*
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();
	*/

	SetEtcItemUse( iType );
	g_MyInfo.SetUseEtcItem( iType );
}

void MyInventoryWnd::ShowMainInfoWnd()
{
	// 예외 체크
	GashaponWnd *pGashaponWnd = dynamic_cast<GashaponWnd*>(FindChildWnd(ID_GASHPON_WND));
	if( pGashaponWnd && pGashaponWnd->IsShow() && pGashaponWnd->IsRolling() )
		return;

	SuperGashaponWnd *pSuperGashaponWnd = dynamic_cast<SuperGashaponWnd*>(FindChildWnd(ID_SUPER_GASHAPON_WND));
	if( pSuperGashaponWnd && pSuperGashaponWnd->IsShow() && pSuperGashaponWnd->IsRolling() )
		return;

	RandomDecoWnd *pRandomDecoWnd = dynamic_cast<RandomDecoWnd*>(FindChildWnd(ID_RANDOM_DECO_WND));
	if( pRandomDecoWnd && pRandomDecoWnd->IsShow() && pRandomDecoWnd->IsRolling() )
		return;

	PetEggResultWnd *pPetEggResultWnd = dynamic_cast<PetEggResultWnd*>(FindChildWnd(ID_PET_EGG_RESULT_WND));
	if( pPetEggResultWnd && pPetEggResultWnd->IsShow() && pPetEggResultWnd->IsRolling() )
		return;

	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd )
	{
		pInfoWnd->HideWnd();
		ShowChildWnd( ID_FLASH_WND );
		pInfoWnd->ShowWnd();
	}

	HideChildWnd(ID_GROWTH_DOWN_WND);
	HideChildWnd(ID_ITEM_COMPOUND_WND);
	HideChildWnd(ID_MULTIPLE_COMPOUND_WND);
	HideChildWnd(ID_PACKAGE_ITEM_WND);
	HideChildWnd(ID_GASHPON_WND);
	HideChildWnd(ID_SUPER_GASHAPON_WND);
	HideChildWnd(ID_RANDOM_DECO_WND);
	HideChildWnd(ID_DECO_SELECT_WND);
	HideChildWnd(ID_MULTIPLE_COMPOUND_RESULT);
	HideChildWnd(ID_ITEM_COMPOUND_RESULT);
	HideChildWnd(ID_EXTRAITEM_BOX_WND);
	HideChildWnd(ID_GROWTH_DOWN_RESULT);
	HideChildWnd(ID_PACKAGE_ITEM_RESULT);
	HideChildWnd(ID_CHANGE_NAME_WND);
	HideChildWnd(ID_CHANGE_NAME_RESULT_WND);
	HideChildWnd(ID_TRADE_STATE_CHANGE_WND);
	HideChildWnd(ID_ITEM_RECVSELL_INFO_WND);
	HideChildWnd(ID_RECORD_INIT_WND);
	HideChildWnd(ID_RECORD_INIT_RESULT_WND);
	HideChildWnd(ID_ITEM_GROWTH_CATALYST_WND);
	HideChildWnd(ID_ITEM_GROWTH_CATALYST_RESULT_WND);
	HideChildWnd(ID_ITEM_LUCKY_COIN_USE_WND);
	HideChildWnd(ID_ITEM_LUCKY_COIN_USE_RESULT_WND);
	HideChildWnd(ID_ITEM_COMPOUNDEX_WND);
	HideChildWnd(ID_ITEM_COMPOUNDEX_RESULT_WND);
	HideChildWnd(ID_ITEM_RAINBOW_MIXER_WND);
	HideChildWnd(ID_ITEM_RAINBOW_MIXER_RESULT_WND);
	HideChildWnd(ID_ITEM_LOSTSAGA_MIXER_WND);
	HideChildWnd(ID_ITEM_LOSTSAGA_MIXER_RESULT_WND);
	HideChildWnd(ID_ITEM_GOLD_BOX_WND);
	HideChildWnd(ID_SOLDIER_SELECT_BUY_WND);
	HideChildWnd(ID_SOLDIER_SELECT_RESULT_WND);
	HideChildWnd(ID_FOUR_EXTRA_COMPOUND_BUY_WND);
	HideChildWnd(ID_FOUR_EXTRA_COMPOUND_RESULT_WND);
	HideChildWnd(ID_SOLDIER_EXP_BONUS_SELECT_WND);
	HideChildWnd(ID_SOLDIER_EXP_BONUS_SELECT_RESULT_WND);
	HideChildWnd(ID_EXPAND_MEDAL_SLOT_OPEN_WND);
	HideChildWnd(ID_EXPAND_MEDAL_SLOT_RESULT_WND);
	HideChildWnd(ID_ALCHEMICFUNC_WND);
	HideChildWnd(ID_PRESET_PACKAGE_WND);
	HideChildWnd(ID_PRESET_PACKAGE_RESULT_WND);
	HideChildWnd(ID_GROWTH_ALL_DOWN_WND);
	HideChildWnd(ID_GROWTH_ALL_DOWN_RESULT);
	
	HideChildWnd(ID_ITEM_MATERIAL_COMPOUND_WND);
	HideChildWnd(ID_ITEM_MATERIAL_COMPOUND_RESULT);
	HideChildWnd(ID_PET_EGG_RESULT_WND);
	HideChildWnd(ID_NAMED_TITLE_RESULT_WND);
	HideChildWnd(ID_NAMED_TITLE_PREMIUM_RESULT_WND);
	HideChildWnd(ID_NAMED_TITLE_PREMIUM_SELECT_WND);

	HideChildWnd(ID_HERO_REINFORCE_DWON_WND);
	HideChildWnd(ID_HERO_REINFORCE_DONE_WND);
	HideChildWnd(ID_ITEM_REINFORCE_DWON_WND);
	HideChildWnd(ID_ITEM_REINFORCE_DONE_WND);
	HideChildWnd(ID_COSTUME_BOX_WND);
	HideChildWnd(ID_NEW_MULTIPLE_COMPOUND_WND);
}

void MyInventoryWnd::ShowGrowthDownWnd()
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	GrowthDownSelectWnd *pWnd = dynamic_cast<GrowthDownSelectWnd*>(FindChildWnd(ID_GROWTH_DOWN_WND));
	if( pWnd )
		pWnd->ShowWnd();
}

void MyInventoryWnd::OnGrowthDownResult( int iClassType, int iReturnPoint )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	GrowthDownDoneWnd *pWnd = dynamic_cast<GrowthDownDoneWnd*>(FindChildWnd(ID_GROWTH_DOWN_RESULT));
	if( pWnd )
		pWnd->ShowGrowthDownDone( iClassType, iReturnPoint );
}

void MyInventoryWnd::ShowGrowthAllDownWnd()
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	GrowthAllDownSelectWnd *pWnd = dynamic_cast<GrowthAllDownSelectWnd*>(FindChildWnd(ID_GROWTH_ALL_DOWN_WND));
	if( pWnd )
		pWnd->ShowWnd();
}

void MyInventoryWnd::OnGrowthAllDownResult( int iClassType, int iReturnPoint, __int64 iReturnPeso )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	GrowthAllDownDoneWnd *pWnd = dynamic_cast<GrowthAllDownDoneWnd*>(FindChildWnd(ID_GROWTH_ALL_DOWN_RESULT));
	if( pWnd )
		pWnd->ShowGrowthAllDownDone( iClassType, iReturnPoint, iReturnPeso );
}

void MyInventoryWnd::ShowItemCompoundWnd( DWORD dwCode )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	ItemCompoundWnd *pWnd = dynamic_cast<ItemCompoundWnd*>(FindChildWnd(ID_ITEM_COMPOUND_WND));
	if( pWnd )
		pWnd->ShowWndWithEtcItemCode( dwCode );
}

void MyInventoryWnd::OnItemCompoundResult( bool bSuccess, int iTargetSlot, int iReinforce, DWORD dwCode )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	HideChildWnd(ID_ITEM_COMPOUND_WND);

	ShowChildWnd( ID_FLASH_WND );
	ItemCompoundResultWnd *pWnd = dynamic_cast<ItemCompoundResultWnd*>(FindChildWnd(ID_ITEM_COMPOUND_RESULT));
	if( pWnd )
		pWnd->SetCompoundResultInfo( bSuccess, iTargetSlot, iReinforce, dwCode );
}

void MyInventoryWnd::ShowItemGrowthCatalystWnd( DWORD dwCode )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	ItemGrowthCatalystWnd *pWnd = dynamic_cast<ItemGrowthCatalystWnd*>(FindChildWnd(ID_ITEM_GROWTH_CATALYST_WND));
	if( pWnd )
	{
		pWnd->ShowWndWithEtcItemCode( dwCode );
	}
}

void MyInventoryWnd::OnItemGrowthCatalystDelay( int iTargetSlot )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	HideChildWnd(ID_ITEM_GROWTH_CATALYST_WND);

	ShowChildWnd( ID_FLASH_WND );
	
	ItemGrowthCatalystResultWnd *pWnd = dynamic_cast<ItemGrowthCatalystResultWnd*>(FindChildWnd(ID_ITEM_GROWTH_CATALYST_RESULT_WND));
	if( pWnd )
		pWnd->SetGrowthCatalystDelayInfo( iTargetSlot );
}

void MyInventoryWnd::OnItemGrowthCatalystResult( bool bSuccess, int iTargetSlot, int iReinforce, int iTargetPeriodType, DWORD dwCode )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	HideChildWnd(ID_ITEM_GROWTH_CATALYST_WND);

	ItemGrowthCatalystResultWnd *pWnd = dynamic_cast<ItemGrowthCatalystResultWnd*>(FindChildWnd(ID_ITEM_GROWTH_CATALYST_RESULT_WND));
	if( pWnd )
		pWnd->SetGrowthCatalystResultInfo( bSuccess, iTargetSlot, iReinforce, iTargetPeriodType, dwCode );
}

void MyInventoryWnd::ShowItemCompoundExWnd( DWORD dwCode )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	ItemCompoundExWnd *pWnd = dynamic_cast<ItemCompoundExWnd*>(FindChildWnd(ID_ITEM_COMPOUNDEX_WND));
	if( pWnd )
	{
		pWnd->ShowWndWithEtcItemCode( dwCode );
	}
}

void MyInventoryWnd::OnItemCompoundExResult( bool bSuccess, int iTargetSlot, int iReinforce, DWORD dwCode )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	HideChildWnd(ID_ITEM_COMPOUNDEX_WND);

	ItemCompoundExResultWnd *pWnd = dynamic_cast<ItemCompoundExResultWnd*>(FindChildWnd(ID_ITEM_COMPOUNDEX_RESULT_WND));
	if( pWnd )
		pWnd->SetItemCompoundExInfo( bSuccess, iTargetSlot, iReinforce, dwCode );
}

void MyInventoryWnd::ShowItemLuckyCoinWnd( DWORD dwCode )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	ItemLuckyCoinWnd *pWnd = dynamic_cast<ItemLuckyCoinWnd*>(FindChildWnd(ID_ITEM_LUCKY_COIN_USE_WND));
	if( pWnd )
	{
		pWnd->ShowWndWithEtcItemCode( dwCode );
	}
}

void MyInventoryWnd::OnItemLuckyCoinResult( DWORD dwCode, int iSelectIndex )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	HideChildWnd(ID_ITEM_LUCKY_COIN_USE_WND);
	ItemLuckyCoinResultWnd *pWnd = dynamic_cast<ItemLuckyCoinResultWnd*>(FindChildWnd(ID_ITEM_LUCKY_COIN_USE_RESULT_WND));
	if( pWnd )
		pWnd->SetItemLuckyCoinResultInfo( dwCode, iSelectIndex );
}

void MyInventoryWnd::ShowMultipleCompoundWnd( DWORD dwType )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	ItemMultipleCompoundWnd *pWnd = dynamic_cast<ItemMultipleCompoundWnd*>(FindChildWnd(ID_MULTIPLE_COMPOUND_WND));
	if( pWnd )
		pWnd->SetInfoAndShow( dwType );
}

void MyInventoryWnd::OnMultipleCompoundResult(int iEtcType, int iItemCode, int iReinforce )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	HideChildWnd(ID_MULTIPLE_COMPOUND_WND);

	ShowChildWnd( ID_FLASH_WND );
	ItemMultipleCompoundResultWnd *pWnd = dynamic_cast<ItemMultipleCompoundResultWnd*>(FindChildWnd(ID_MULTIPLE_COMPOUND_RESULT));
	if( pWnd )
		pWnd->SetInfoAndShow( iEtcType, iItemCode, iReinforce );
}

void MyInventoryWnd::ShowRainbowMixerWnd( DWORD dwType )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	ItemRainbowMixerWnd *pWnd = dynamic_cast<ItemRainbowMixerWnd*>(FindChildWnd(ID_ITEM_RAINBOW_MIXER_WND));
	if( pWnd )
		pWnd->ShowWndWithEtcItemCode( dwType );
}

void MyInventoryWnd::OnRainbowMixerResult( int iEtcType, int iSelectIndex )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	HideChildWnd(ID_ITEM_RAINBOW_MIXER_WND);
	HideChildWnd(ID_ITEM_GOLD_BOX_WND);

	ShowChildWnd( ID_FLASH_WND );
	ItemRainbowMixerResultWnd *pWnd = dynamic_cast<ItemRainbowMixerResultWnd*>(FindChildWnd(ID_ITEM_RAINBOW_MIXER_RESULT_WND));
	if( pWnd )
		pWnd->SetInfoAndShow( iEtcType, iSelectIndex );
}

void MyInventoryWnd::ShowLostSagaMixerWnd( DWORD dwType )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	ItemLostSagaMixerWnd *pWnd = dynamic_cast<ItemLostSagaMixerWnd*>(FindChildWnd(ID_ITEM_LOSTSAGA_MIXER_WND));
	if( pWnd )
		pWnd->ShowWndWithEtcItemCode( dwType );
}

void MyInventoryWnd::OnLostSagaMixerResult( int iEtcType, int iSelectIndex )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	HideChildWnd(ID_ITEM_LOSTSAGA_MIXER_WND);

	ShowChildWnd( ID_FLASH_WND );
	ItemLostSagaMixerResultWnd *pWnd = dynamic_cast<ItemLostSagaMixerResultWnd*>(FindChildWnd(ID_ITEM_LOSTSAGA_MIXER_RESULT_WND));
	if( pWnd )
		pWnd->SetInfoAndShow( iEtcType, iSelectIndex );
}

void MyInventoryWnd::ShowGoldBoxWnd( DWORD dwType )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	ItemGoldBoxWnd *pWnd = dynamic_cast<ItemGoldBoxWnd*>( FindChildWnd( ID_ITEM_GOLD_BOX_WND ) );
	if( pWnd )
		pWnd->ShowWndWithEtcItemCode( dwType );
}

void MyInventoryWnd::ShowPackageItemWnd( PackageItemBuyWnd::PackageStyle ePackageStyle, DWORD dwType, int eActiveFilter /* = 0 : none */ )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>( FindChildWnd( ID_INVENTORY_INFO_WND ) );
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	PackageItemBuyWnd *pWnd = dynamic_cast<PackageItemBuyWnd*>( FindChildWnd( ID_PACKAGE_ITEM_WND ) );
	if( pWnd )
		pWnd->ShowPackage( ePackageStyle, dwType, eActiveFilter );
}

void MyInventoryWnd::ShowSoldierSelectBuyWnd( DWORD dwEtcItemType )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	InvenSoldierSelectBuyWnd *pWnd = dynamic_cast<InvenSoldierSelectBuyWnd*>(FindChildWnd(ID_SOLDIER_SELECT_BUY_WND));
	if( pWnd )
	{
		pWnd->SetEtcItemType( dwEtcItemType );
		pWnd->ShowWnd();
	}
}

void MyInventoryWnd::ShowFourExtraCompoundBuyWnd( DWORD dwEtcItemType )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	InvenFourExtraCompoundBuyWnd *pWnd = dynamic_cast<InvenFourExtraCompoundBuyWnd*>(FindChildWnd(ID_FOUR_EXTRA_COMPOUND_BUY_WND));
	if( pWnd )
	{
		pWnd->SetEtcItemType( dwEtcItemType );
		pWnd->ShowWnd();
	}
}

void MyInventoryWnd::OnPackageItemSoldierResult( CHARACTER &rkCharInfo, int iKeepBonusPeso )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	PackageItemBuyWnd *pBuyWnd = dynamic_cast<PackageItemBuyWnd*>(FindChildWnd(ID_PACKAGE_ITEM_WND));
	if( pBuyWnd && pBuyWnd->IsShow() )
		pBuyWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	PackageItemBuyResultWnd *pResultWnd = dynamic_cast<PackageItemBuyResultWnd*>(FindChildWnd(ID_PACKAGE_ITEM_RESULT));
	if( pResultWnd )
		pResultWnd->SetResultSoldierPackage( rkCharInfo, iKeepBonusPeso );
}

void MyInventoryWnd::OnPackageItemDecoResult( CHARACTER &rkCharInfo, int iKeepBonusPeso )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	PackageItemBuyWnd *pBuyWnd = dynamic_cast<PackageItemBuyWnd*>(FindChildWnd(ID_PACKAGE_ITEM_WND));
	if( pBuyWnd && pBuyWnd->IsShow() )
		pBuyWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	PackageItemBuyResultWnd *pResultWnd = dynamic_cast<PackageItemBuyResultWnd*>(FindChildWnd(ID_PACKAGE_ITEM_RESULT));
	if( pResultWnd )
		pResultWnd->SetResultDecorationPackage( rkCharInfo, iKeepBonusPeso );
}

void MyInventoryWnd::OnSoldierSelectResult( int iClassType, int iClassTime, DWORD dwEtcItemType )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	InvenSoldierSelectBuyWnd *pBuyWnd = dynamic_cast<InvenSoldierSelectBuyWnd*>(FindChildWnd(ID_SOLDIER_SELECT_BUY_WND));
	if( pBuyWnd && pBuyWnd->IsShow() )
		pBuyWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	InvenSoldierSelectBuyResultWnd *pResultWnd = dynamic_cast<InvenSoldierSelectBuyResultWnd*>(FindChildWnd(ID_SOLDIER_SELECT_RESULT_WND));
	if( pResultWnd )
		pResultWnd->ShowResult( iClassType, iClassTime, dwEtcItemType );
}

void MyInventoryWnd::OnForExtraCompoundResult( int iClassType, int iClassTime, DWORD dwEtcItemType, bool bSuccess )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	InvenFourExtraCompoundBuyWnd *pBuyWnd = dynamic_cast<InvenFourExtraCompoundBuyWnd*>(FindChildWnd(ID_FOUR_EXTRA_COMPOUND_BUY_WND));
	if( pBuyWnd && pBuyWnd->IsShow() )
		pBuyWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	InvenFourExtraCompoundBuyResultWnd *pResultWnd = dynamic_cast<InvenFourExtraCompoundBuyResultWnd*>(FindChildWnd(ID_FOUR_EXTRA_COMPOUND_RESULT_WND));
	if( pResultWnd )
		pResultWnd->ShowResult( iClassType, iClassTime, dwEtcItemType, bSuccess );
}

void MyInventoryWnd::ShowGashponWnd( DWORD dwPossessEtcItemType, DWORD dwEtcItemType )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	GashaponWnd *pWnd = dynamic_cast<GashaponWnd*>(FindChildWnd(ID_GASHPON_WND));
	if( pWnd )
		pWnd->ShowRandomBox( dwPossessEtcItemType, dwEtcItemType );
}

void MyInventoryWnd::OnGashponResultInfo( short iPresentType, int iPresentValue1, int iPresentValue2, int iPresentValue3, int iPresentValue4, bool bAlram, int iPresentPeso )
{
	GashaponWnd *pWnd = dynamic_cast<GashaponWnd*>(FindChildWnd(ID_GASHPON_WND));
	if( pWnd && pWnd->IsShow() )
	{
		pWnd->OnResultInfo( iPresentType, iPresentValue1, iPresentValue2, iPresentValue3, iPresentValue4, bAlram, iPresentPeso );
	}
}

void MyInventoryWnd::ShowSuperGashponWnd( DWORD dwEtcItemType )
{
	ShowPresentTabDirect();
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();
	
	SuperGashaponWnd *pSuperGashaponWnd = dynamic_cast<SuperGashaponWnd*> ( FindChildWnd( ID_SUPER_GASHAPON_WND ) );
	if( pSuperGashaponWnd )
		pSuperGashaponWnd->ShowSuperGashaponWnd( PRESENT_ETC_ITEM, dwEtcItemType, 0, SUPERGASHPON_USE );
}

void MyInventoryWnd::ShowRandomDecoWnd( bool bMan )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	RandomDecoWnd *pWnd = dynamic_cast<RandomDecoWnd*>(FindChildWnd(ID_RANDOM_DECO_WND));
	if( pWnd )
		pWnd->ShowRandomBox( bMan );
}

void MyInventoryWnd::OnRandomDecoResultInfo( short iPresentType, int iPresentValue1, int iPresentValue2, int iPresentValue3, int iPresentValue4, bool bAlram, int iPresentPeso )
{
	RandomDecoWnd *pWnd = dynamic_cast<RandomDecoWnd*>(FindChildWnd(ID_RANDOM_DECO_WND));
	if( pWnd && pWnd->IsShow() )
	{
		pWnd->OnResultInfo( iPresentType, iPresentValue1, iPresentValue2, iPresentValue3, iPresentValue4, bAlram, iPresentPeso );
	}
}

void MyInventoryWnd::ShowDecoSelectWnd( int iIndex, const ioHashString& szSubscriptionID )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	RandomDecoSelectWnd *pWnd = dynamic_cast<RandomDecoSelectWnd*>(FindChildWnd(ID_DECO_SELECT_WND));
	if( pWnd )
		pWnd->SetSubscription( iIndex, szSubscriptionID );
}

void MyInventoryWnd::ShowDecoSelectWnd( int iPresentIndex, int iPresentSlot )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	RandomDecoSelectWnd *pWnd = dynamic_cast<RandomDecoSelectWnd*>(FindChildWnd(ID_DECO_SELECT_WND));
	if( pWnd )
		pWnd->SetPresent( iPresentIndex, iPresentSlot );
}

void MyInventoryWnd::OnDecoSelectResult()
{
	RandomDecoSelectWnd *pWnd = dynamic_cast<RandomDecoSelectWnd*>(FindChildWnd(ID_DECO_SELECT_WND));
	if( pWnd && pWnd->IsShow() )
		pWnd->OnResultState();
}

void MyInventoryWnd::ShowExtraItemBoxWnd( int iMachineCode )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	ioExtraItemRandomBox *pWnd = dynamic_cast<ioExtraItemRandomBox*>(FindChildWnd(ID_EXTRAITEM_BOX_WND));
	if( pWnd )
		pWnd->ShowRandomBox( iMachineCode );
}

void MyInventoryWnd::OnExtraItemBoxResultInfo( int iItemCode, int iPeriodTime, int iReinforce, int iTradeType )
{
	ioExtraItemRandomBox *pWnd = dynamic_cast<ioExtraItemRandomBox*>(FindChildWnd(ID_EXTRAITEM_BOX_WND));
	if( pWnd && pWnd->IsShow() )
	{
		pWnd->OnResultInfo( iItemCode, iPeriodTime, iReinforce, iTradeType );
	}
}

void MyInventoryWnd::ShowChangeNameWnd( DWORD dwEtcType )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	ChangeNameWnd *pWnd = dynamic_cast<ChangeNameWnd*>(FindChildWnd(ID_CHANGE_NAME_WND));
	if( pWnd )
		pWnd->ShowChangeIDWnd( dwEtcType );
}

void MyInventoryWnd::OnChangeNameResultWnd( const ioHashString &szNewName )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ChangeNameWnd *pWnd = dynamic_cast<ChangeNameWnd*>(FindChildWnd(ID_CHANGE_NAME_WND));
	if( pWnd && pWnd->IsShow() )
		pWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	ChangeNameResultWnd *pResultWnd = dynamic_cast<ChangeNameResultWnd*>(FindChildWnd(ID_CHANGE_NAME_RESULT_WND));
	if( pResultWnd )
		pResultWnd->SetInfoAndShow( szNewName );
}

void MyInventoryWnd::OnChangeGuildNameResultWnd( const ioHashString &szPreName, const ioHashString &szNewName )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ChangeNameWnd *pWnd = dynamic_cast<ChangeNameWnd*>(FindChildWnd(ID_CHANGE_NAME_WND));
	if( pWnd && pWnd->IsShow() )
		pWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	ChangeNameResultWnd *pResultWnd = dynamic_cast<ChangeNameResultWnd*>(FindChildWnd(ID_CHANGE_NAME_RESULT_WND));
	if( pResultWnd )
		pResultWnd->SetInfoAndShow( szPreName, szNewName );
}

void MyInventoryWnd::ShowSoldierExpSelectWnd( DWORD dwEtcItemType )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	InvenSoldierExpBonusWnd *pWnd = dynamic_cast<InvenSoldierExpBonusWnd*>(FindChildWnd(ID_SOLDIER_EXP_BONUS_SELECT_WND));
	if( pWnd )
	{
		pWnd->SetEtcItemType( dwEtcItemType );
		pWnd->ShowWnd();
	}
}

void MyInventoryWnd::OnSoldierExpSelectResult( int iClassType, DWORD dwEtcItemType )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	InvenSoldierExpBonusWnd *pBuyWnd = dynamic_cast<InvenSoldierExpBonusWnd*>(FindChildWnd(ID_SOLDIER_EXP_BONUS_SELECT_WND));
	if( pBuyWnd && pBuyWnd->IsShow() )
		pBuyWnd->HideWnd();
	
	InvenSoldierExpBonusResultWnd *pWnd = dynamic_cast<InvenSoldierExpBonusResultWnd*>(FindChildWnd(ID_SOLDIER_EXP_BONUS_SELECT_RESULT_WND));
	if( pWnd )
	{
		pWnd->ShowResult( iClassType, dwEtcItemType );
		pWnd->ShowWnd();
	}
}

void MyInventoryWnd::OnErrorEdit( const char *szError )
{
	ChangeNameWnd *pWnd = dynamic_cast<ChangeNameWnd*>(FindChildWnd(ID_CHANGE_NAME_WND));
	if( pWnd && pWnd->IsShow() )
	{
		pWnd->ErrorEdit( szError );
	}
}

void MyInventoryWnd::OnChangeTradeStateWnd( int iIndex )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	ChangeTradeStateResultWnd *pResultWnd = dynamic_cast<ChangeTradeStateResultWnd*>(FindChildWnd(ID_TRADE_STATE_CHANGE_WND));
	if( pResultWnd )
		pResultWnd->SetChangeResultInfo( iIndex );
}

void MyInventoryWnd::ShowRecordInitWnd( DWORD dwEtcType )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	RecordInitWnd *pRecordInit = dynamic_cast<RecordInitWnd*>(FindChildWnd(ID_RECORD_INIT_WND));
	if( pRecordInit )
	{
		pRecordInit->ShowRecordInitWnd( dwEtcType );
	}
}

void MyInventoryWnd::OnRecordInitResultWnd( DWORD dwEtcType )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	RecordInitWnd *pWnd = dynamic_cast<RecordInitWnd*>(FindChildWnd(ID_RECORD_INIT_WND));
	if( pWnd && pWnd->IsShow() )
		pWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	RecordInitResultWnd *pResultWnd = dynamic_cast<RecordInitResultWnd*>(FindChildWnd(ID_RECORD_INIT_RESULT_WND));
	if( pResultWnd )
		pResultWnd->SetInfoAndShow( dwEtcType );
}

void MyInventoryWnd::ShowItemRecvSellInfoWnd( int iCommandType,
											  int iItemInfoType,
											  const ioHashString szIconName,
											  const ComplexStringPrinterVec &pTitleVec,
											  const ComplexStringPrinterVec &pDescVec,
											  int iClassType,
											  int iReinforce,
											  bool bExtraItemCustom,
											  bool bIsCostume,
											  int nGradeType )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	ItemRecvSellInfoWnd *pWnd = dynamic_cast<ItemRecvSellInfoWnd*>(FindChildWnd(ID_ITEM_RECVSELL_INFO_WND));
	if( pWnd )
		pWnd->SetItemRecvSellInfo( iCommandType, iItemInfoType, szIconName, pTitleVec, pDescVec, iClassType, iReinforce, bExtraItemCustom, bIsCostume, nGradeType );
}

void MyInventoryWnd::ShowSubscriptionRetrCheckLimit()
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
	{
		pInfoWnd->ShowSubscriptionRetrCheckLimit();
	}
}

void MyInventoryWnd::ShowSubscriptionRetrCheck( int iIndex, const ioHashString& szSubscriptionID, int iSubscriptionGold, int iRetraGold )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
	{
		pInfoWnd->ShowSubscriptionRetrCheck( iIndex, szSubscriptionID, iSubscriptionGold, iRetraGold );
	}
}

void MyInventoryWnd::ShowExpandMedalSlotOpenWnd( DWORD dwClassType )
{
	ioEtcItemExpandMedalSlot *pItem = dynamic_cast<ioEtcItemExpandMedalSlot*> (g_EtcItemMgr.FindEtcItem(dwClassType) );
	if( !pItem )
	{
		LOG.PrintTimeAndLog( 0, "%s ioEtcItem == NULL , Type:%d", __FUNCTION__, dwClassType );
		return;
	}

	int iUseLevel = pItem->GetLimitLevel();
	BYTE iSlotNumber = pItem->GetUseSlotNumber();

	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	ExpandMedalSlotWnd *pWnd = dynamic_cast<ExpandMedalSlotWnd*>(FindChildWnd(ID_EXPAND_MEDAL_SLOT_OPEN_WND));
	if( pWnd )
	{
		pWnd->SetItemInfo( dwClassType );
		pWnd->ShowWnd();
	}
}

void MyInventoryWnd::OnExpandMedalSlotResultWnd( int iClassType, int iSlotNumber )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	ExpandMedalSlotResultWnd *pWnd = dynamic_cast<ExpandMedalSlotResultWnd*> ( FindChildWnd(ID_EXPAND_MEDAL_SLOT_RESULT_WND) );
	if( pWnd )
		pWnd->SetInfo( iClassType, iSlotNumber );
}

void MyInventoryWnd::ShowAlchemicFuncWnd()
{
	// 예외 체크
	GashaponWnd *pGashaponWnd = dynamic_cast<GashaponWnd*>(FindChildWnd(ID_GASHPON_WND));
	if( pGashaponWnd && pGashaponWnd->IsShow() && pGashaponWnd->IsRolling() )
		return;

	SuperGashaponWnd *pSuperGashaponWnd = dynamic_cast<SuperGashaponWnd*>(FindChildWnd(ID_SUPER_GASHAPON_WND));
	if( pSuperGashaponWnd && pSuperGashaponWnd->IsShow() && pSuperGashaponWnd->IsRolling() )
		return;

	RandomDecoWnd *pRandomDecoWnd = dynamic_cast<RandomDecoWnd*>(FindChildWnd(ID_RANDOM_DECO_WND));
	if( pRandomDecoWnd && pRandomDecoWnd->IsShow() && pRandomDecoWnd->IsRolling() )
		return;

	PetEggResultWnd *pPetEggResultWnd = dynamic_cast<PetEggResultWnd*>(FindChildWnd(ID_PET_EGG_RESULT_WND));
	if( pPetEggResultWnd && pPetEggResultWnd->IsShow() && pPetEggResultWnd->IsRolling() )
		return;

	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd )
		pInfoWnd->HideWnd();

	ItemRecvSellInfoWnd *pSellWnd = dynamic_cast<ItemRecvSellInfoWnd*>(FindChildWnd(ID_ITEM_RECVSELL_INFO_WND));
	if( pSellWnd )
		pSellWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	AlchemicFuncWnd *pWnd = dynamic_cast<AlchemicFuncWnd*>(FindChildWnd(ID_ALCHEMICFUNC_WND));
	if( pWnd )
	{
		pWnd->ShowWnd();
	}
}

void MyInventoryWnd::OnAlchemicFuncResult( int iResult, SP2Packet &rkPacket )
{
	AlchemicFuncWnd *pWnd = dynamic_cast<AlchemicFuncWnd*>(FindChildWnd(ID_ALCHEMICFUNC_WND));
	if( pWnd && pWnd->IsShow() )
	{
		pWnd->OnAlchemicFuncResult( iResult, rkPacket );
	}
}

void MyInventoryWnd::CheckAlchemicFuncEnable()
{
	AlchemicFuncWnd *pWnd = dynamic_cast<AlchemicFuncWnd*>(FindChildWnd(ID_ALCHEMICFUNC_WND));
	if( pWnd && pWnd->IsShow() )
	{
		pWnd->CheckActionEnable();
	}
}

bool MyInventoryWnd::CheckAlchemicEnableDrag()
{
	AlchemicFuncWnd *pWnd = dynamic_cast<AlchemicFuncWnd*>(FindChildWnd(ID_ALCHEMICFUNC_WND));
	if( pWnd && pWnd->IsShow() )
	{
		if( pWnd->GetFuncState() == AlchemicFuncWnd::AFS_SELECT )
			return true;
	}

	return false;
}

bool MyInventoryWnd::GetCurItemInfo( OUT InventoryItemInfo &rkInfo, IN DWORD dwItemInfoID )
{
	if( !COMPARE( dwItemInfoID, ID_ITEM_INFO_WND_1_BTN, ID_ITEM_INFO_WND_8_BTN+1) )
		return false;

	int iStartArray = m_iCurPage * PAGE_PER_ITEM_NUM;
	int iCurArray   = iStartArray + ( dwItemInfoID - ID_ITEM_INFO_WND_1_BTN );
	int iSize       = m_vInventoryItemInfoList.size();
	if( !COMPARE( iCurArray, 0, iSize ) )
		return false;

	rkInfo = m_vInventoryItemInfoList[iCurArray];
	return true;
}

bool MyInventoryWnd::GetCurAlchemicItemInfo( OUT AlchemicToolTipInfo &rkInfo, IN DWORD dwItemInfoID )
{
	if( COMPARE(dwItemInfoID, ID_ALCHEMIC_INFO_WND_1_BTN, ID_ALCHEMIC_INFO_WND_30_BTN+1) )
	{
		AlchemicItemWnd *pItem = dynamic_cast<AlchemicItemWnd*>(FindChildWnd(dwItemInfoID));
		if( pItem )
		{
			rkInfo.m_iCode = pItem->GetItemCode();
			rkInfo.m_iType = AlchemicItemToolTip::ATT_ALCHEMIC_ITEM;
			return true;
		}
	}
	else
	{
		AlchemicFuncWnd *pAlchemicWnd = dynamic_cast<AlchemicFuncWnd*>(FindChildWnd(ID_ALCHEMICFUNC_WND));
		if( pAlchemicWnd && pAlchemicWnd->IsShow() )
		{
			if( pAlchemicWnd->GetCurAlchemicItemInfo( rkInfo, dwItemInfoID ) )
				return true;
		}

		ItemMaterialCompoundWnd *pItemMaterialCompoundWnd = (ItemMaterialCompoundWnd*)(FindChildWnd( ID_ITEM_MATERIAL_COMPOUND_WND ));
		if ( pItemMaterialCompoundWnd && pItemMaterialCompoundWnd->IsShow() )
		{
			if( pItemMaterialCompoundWnd->GetCurAlchemicItemInfo( rkInfo, dwItemInfoID ) )
				return true;
		}		
	}

	return false;
}

void MyInventoryWnd::ShowPreSetPackageItemWnd( DWORD dwType )
{
	ShowMainInfoWnd();

	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	PreSetSoldierPackageWnd *pWnd = dynamic_cast<PreSetSoldierPackageWnd*>(FindChildWnd(ID_PRESET_PACKAGE_WND));
	if( pWnd )
		pWnd->ShowPackage( dwType );
}

void MyInventoryWnd::OnPreSetPackageItemResult( CHARACTER &rkCharInfo, int iKeepBonusPeso, int iLimitTime )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	PreSetSoldierPackageWnd *pBuyWnd = dynamic_cast<PreSetSoldierPackageWnd*>(FindChildWnd(ID_PRESET_PACKAGE_WND));
	if( pBuyWnd && pBuyWnd->IsShow() )
		pBuyWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	PreSetSoldierPackageResultWnd *pResultWnd = dynamic_cast<PreSetSoldierPackageResultWnd*>(FindChildWnd(ID_PRESET_PACKAGE_RESULT_WND));
	if( pResultWnd )
		pResultWnd->SetResultSoldierPackage( rkCharInfo, iKeepBonusPeso, iLimitTime );
}

bool MyInventoryWnd::QuestGuide( const IntVec &rValue, float &rfXPos, float &rfYPos )
{
	if( rValue.size() < 8 ) return false;

	int iMTabID = rValue[0];
	int iSTabID = rValue[1];

	switch( rValue[2] )
	{
	case INVENTORY_ITEM_EXTRAITEM:
	case INVENTORY_ITEM_DECO:
	case INVENTORY_ITEM_ETC:
	case INVENTORY_ITEM_PRESENT:
	case INVENTORY_ITEM_MEDALITEM:
	case INVENTORY_ITEM_COSTUME:
		{
			for(int i = 0;i < MAX_ITEM_INFO_WND_BTN;i++)
			{
				InventoryItemWnd *pItemWnd = dynamic_cast<InventoryItemWnd*>(FindChildWnd( ID_ITEM_INFO_WND_1_BTN + i ));
				if( !pItemWnd ) continue;
				if( !pItemWnd->IsShow() ) continue;
				if( pItemWnd->GetMainType() != rValue[2] ) continue;

                if( pItemWnd->QuestGuide( rValue, rfXPos, rfYPos ) )
					return true;
			}
		}
		break;
	default:
		{
			for(int i = 0;i < MAX_CHAR_INFO_WND_BTN;i++)
			{
				InventorySoldierItemWnd *pItemWnd = dynamic_cast<InventorySoldierItemWnd*>(FindChildWnd( ID_CHAR_INFO_WND_1_BTN + i ));
				if( !pItemWnd ) continue;
				if( !pItemWnd->IsShow() ) continue;

				if( pItemWnd->QuestGuide( rValue[3], rfXPos, rfYPos ) )
					return true;
			}
		}
		break;
	}

	if( m_dwCurTabID == (DWORD)iMTabID && m_dwCurSmallTabID == (DWORD)iSTabID ) 
	{
		// 탭 버튼이 정상적으로 선태되어있으면 스크롤 UI에 위치
		ioScroll *pScroll = dynamic_cast<ioScroll*>(FindChildWnd( ID_VERT_SCROLL ));
		if( pScroll )
		{
			rfXPos = pScroll->GetDerivedPosX() + (pScroll->GetWidth() / 2);
			rfYPos = pScroll->GetDerivedPosY();
			return true;
		}
	}
	else if( m_dwCurTabID == (DWORD)iMTabID )	
	{
		ioWnd *pSubTab = FindChildWnd( iSTabID );
		if( pSubTab )
		{
			rfXPos = pSubTab->GetDerivedPosX() + (pSubTab->GetWidth() / 2);
			rfYPos = pSubTab->GetDerivedPosY();
			return true;
		}
	}
	else
	{
		ioWnd *pMainTab = FindChildWnd( iMTabID );
		if( pMainTab )
		{
			rfXPos = pMainTab->GetDerivedPosX() + (pMainTab->GetWidth() / 2);
			rfYPos = pMainTab->GetDerivedPosY();
			return true;
		}
	}
	return false;
}

void MyInventoryWnd::iwm_gototop()
{
	// 용병 선택창이 항상 상위에 위치한다.
	UISoldierSelectWnd *pUISoliderWnd = dynamic_cast<UISoldierSelectWnd *>(g_GUIMgr.FindWnd( UI_SOLDIERSELECT_WND ));
	if( pUISoliderWnd && pUISoliderWnd->IsShow() )
		pUISoliderWnd->GoToTop();
}

bool MyInventoryWnd::EnableDecoChangeByEquipItem( CHARACTER &rkCharInfo )
{
	ioUserExtraItem *pExtraItem = g_MyInfo.GetUserExtraItem();
	if( !pExtraItem )
		return true;

	for( int i=0; i < 4; ++i )
	{
		EXTRAITEMSLOT kSlot;
		int iExtraItemIndex = rkCharInfo.m_extra_item[i];

		if( pExtraItem->GetExtraItem( iExtraItemIndex, kSlot ) )
		{
			const ioItem *pItem = g_ItemMaker.GetItemConst( kSlot.m_iItemCode, __FUNCTION__ );
			if( pItem && pItem->GetCharOnlySexType() != ioSetItemInfo::CST_ALL )
				return false;
		}
	}

	return true;
}

//-------------------------------------------------------------------------
MyInventoryToolTip::MyInventoryToolTip()
{
	m_iMainType    = 0;
	m_iMagicCode   = 0;
	m_iMagicCodeEx = 0;
	m_iMagicCodeEx2= 0;
	m_iManualIndex = 0;

	m_pGrowthLineLeft   = NULL;
	m_pGrowthLineCenter = NULL;
	m_pGrowthLineRight  = NULL;
	m_pGrowthBackGray   = NULL;
	m_pGrowthBackLight  = NULL;

	for (int i = 0; i < MAX_CHAR_GROWTH ; i++)
		m_iCharGrowth[i] = 0;
	for (int i = 0; i < MAX_ITEM_GROWTH ; i++)
		m_iItemGrowth[i] = 0;
}

MyInventoryToolTip::~MyInventoryToolTip()
{
	ClearDesc();

	SAFEDELETE( m_pGrowthLineLeft );
	SAFEDELETE( m_pGrowthLineCenter );
	SAFEDELETE( m_pGrowthLineRight );
	SAFEDELETE( m_pGrowthBackGray );
	SAFEDELETE( m_pGrowthBackLight );
}

void MyInventoryToolTip::OnProcess( float fTimePerSec )
{
	//팝업이 보이면 이쪽 먼저
	if( g_GUIMgr.IsShow( POPUP_STORE_WND ) )
	{
		OnProcessPopup();
		return;
	}
	if( !g_GUIMgr.IsShow( MY_INVENTORY_WND ) )
	{
		HideWnd();
		return;
	}

	ioLSMouse *pMouse = g_App.GetMouse();
	if( !pMouse ) return;

	InventoryItemInfo kInfo;
	MyInventoryWnd *pInven = dynamic_cast<MyInventoryWnd*> ( g_GUIMgr.FindWnd( MY_INVENTORY_WND ) );
	ioWnd *pInvenInfo = NULL;
	if( pInven )
		pInvenInfo = pInven->FindChildWnd( MyInventoryWnd::ID_INVENTORY_INFO_WND );

	if( COMPARE( pMouse->GetToolTipWndID(), MyInventoryWnd::ID_ITEM_INFO_WND_1_BTN, MyInventoryWnd::ID_ITEM_INFO_WND_8_BTN + 1 ) )
	{
		if( pInven )
			pInven->GetCurItemInfo( kInfo, pMouse->GetToolTipWndID() );
	}
	else if( COMPARE( pMouse->GetToolTipWndID(), MyInventoryInfoWnd::ID_KINDRED_BTN, MyInventoryInfoWnd::ID_UNDERWEAR_BTN + 1 ) )
	{
		if( pInvenInfo )
		{
			InventoryIconBtn *pBtn = dynamic_cast<InventoryIconBtn*> ( pInvenInfo->FindChildWnd( pMouse->GetToolTipWndID() ) );
			if( pBtn )
				pBtn->GetDecoInfo( kInfo.m_iSexType, kInfo.m_iDecoType, kInfo.m_iDecoCode );
			kInfo.m_iMainType = INVENTORY_ITEM_DECO;
		}
	}
	else if( COMPARE( pMouse->GetToolTipWndID(), MyInventoryInfoWnd::ID_WEAPON_ITEM_BTN, MyInventoryInfoWnd::ID_MEDAL_ITEM_6_BTN + 1 ) )
	{
		if( pInvenInfo )
		{
			SlotIconBtn *pBtn = dynamic_cast<SlotIconBtn*> ( pInvenInfo->FindChildWnd( pMouse->GetToolTipWndID() ) );
			if( pBtn )
			{
				kInfo.m_iMagicCode   = pBtn->GetMagicCode();
				kInfo.m_iMagicCodeEx = pBtn->GetMagicCodeEx();
				kInfo.m_iMagicCodeEx2= pBtn->GetMagicCodeEx2();
			}

			if( COMPARE( pMouse->GetToolTipWndID(), MyInventoryInfoWnd::ID_WEAPON_ITEM_BTN, MyInventoryInfoWnd::ID_CLOAK_ITEM_BTN + 1 ) )
				kInfo.m_iMainType = INVENTORY_ITEM_EXTRAITEM;
			else
				kInfo.m_iMainType = INVENTORY_ITEM_MEDALITEM;
		}
	}
	// UI코스튬 관련 (툴팁 설정)
	else if( COMPARE( pMouse->GetToolTipWndID(), MyInventoryInfoWnd::ID_WEAPON_COSTUME_BTN, MyInventoryInfoWnd::ID_CLOAK_COSTUME_BTN + 1 ) )
	{
		if( pInvenInfo )
		{
			SlotIconBtn *pBtn = dynamic_cast<SlotIconBtn*> ( pInvenInfo->FindChildWnd( pMouse->GetToolTipWndID() ) );
			if( pBtn )
			{
				kInfo.m_iMagicCode   = pBtn->GetMagicCode();
				kInfo.m_iMagicCodeEx = pBtn->GetMagicCodeEx();
				kInfo.m_iMagicCodeEx2= pBtn->GetMagicCodeEx2();
			}

			kInfo.m_iMainType = INVENTORY_ITEM_COSTUME;
		}
	}
	else if( COMPARE( pMouse->GetToolTipWndID(), MyInventoryInfoWnd::ID_ACCESSORY_RING_BTN, MyInventoryInfoWnd::ID_ACCESSORY_BRACELET_BTN + 1 ) )
	{
		if( pInvenInfo )
		{
			SlotIconBtn *pBtn = dynamic_cast<SlotIconBtn*> ( pInvenInfo->FindChildWnd( pMouse->GetToolTipWndID() ) );
			if( pBtn )
			{
				kInfo.m_iMagicCode   = pBtn->GetMagicCode();
				kInfo.m_iMagicCodeEx = pBtn->GetMagicCodeEx();
				kInfo.m_iMagicCodeEx2= pBtn->GetMagicCodeEx2();
			}

			kInfo.m_iMainType = INVENTORY_ITEM_ACCESSORY;
		}
	}

	if( IsShow() )
	{	
		if( !SetInfo( kInfo ) )                                          
			HideWnd();
		else
		{
			pMouse->SetToolTipWndID( 0 );
			pMouse->SetToolTipCursor();
			UpdateWndSize();
			int iXPos = 0;
			int iYPos = 0;
			Help::GetNotCutWndPos( pMouse->GetMousePos().x, pMouse->GetMousePos().y, GetWidth(), GetHeight(), iXPos, iYPos );
			SetWndPos( iXPos, iYPos );

		}

		ProcessDesc();
	}
	else
	{
		if( SetInfo( kInfo ) ) // setinfo 후에 showwnd
		{
			ShowWnd();
			UpdateWndSize();
			int iXPos = 0;
			int iYPos = 0;
			Help::GetNotCutWndPos( pMouse->GetMousePos().x, pMouse->GetMousePos().y, GetWidth(), GetHeight(), iXPos, iYPos );
			SetWndPos( iXPos, iYPos );
		}
	}
}

void MyInventoryToolTip::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	enum { PRINT_XOFFSET = 14, FIRST_YOFFSET = 99, SECOND_YOFFSET = 207,   };
	RenderDesc( iXPos, iYPos );
	RenderGrowth( iXPos, iYPos + FIRST_YOFFSET );
	RenderAccDesc( iXPos, iYPos );

	int iPrintYPos = iYPos + FIRST_YOFFSET;
	if( m_iMainType == INVENTORY_ITEM_EXTRAITEM ||
		m_iMainType == INVENTORY_ITEM_MEDALITEM )
		iPrintYPos = iYPos + SECOND_YOFFSET;


	int iAccOffset = m_iMaxAccDesc * ACC_LINE_GAP;
	int iPrintManualYPos = iPrintYPos + iAccOffset;
	PrintManual( iXPos+PRINT_XOFFSET, iPrintManualYPos, FONT_SIZE_12 );
}

void MyInventoryToolTip::ClearDesc()
{
	for (int i = 0; i < MAX_DESC ; i++)
	{
		m_szDescArray[i].ClearList();
	}

	m_iMaxAccDesc = 0;
	for (int i = 0; i < MAX_ACC_DESC ; i++)
	{
		m_szAccDescArray[i].ClearList();
	}
}

void MyInventoryToolTip::RenderDesc( int iXPos, int iYPos )
{
	enum 
	{
		X_OFFSET       = 90,
		MAX_TYPE       = 2,
	};

	iXPos += X_OFFSET;

	int iYPosArray[MAX_TYPE][MAX_DESC]={ 21, 41, 59, 
		32, 53, 0, };

	int iDescCnt = 0;
	for( int i = 0 ; i < MAX_DESC ; i++ )
	{
		if( m_szDescArray[i].IsEmpty() )
			continue;
		iDescCnt++;
	}

	int iType = 0;
	if( iDescCnt != MAX_DESC )
		iType = 1;

	int iCnt  = 0;
	for( int i = 0 ; i < MAX_DESC ; i++ )
	{
		if( m_szDescArray[i].IsEmpty() )
			continue;
		if( COMPARE( iType, 0, MAX_TYPE ) && COMPARE( iCnt, 0, MAX_DESC ) )
			m_szDescArray[i].PrintFullTextWidthCut( iXPos, iYPos + iYPosArray[iType][iCnt] ,TAT_LEFT, INVEN_TOOLTIP_WIDTHCUT_SIZE );
		iCnt++;
	}
}

void MyInventoryToolTip::RenderAccDesc( int iXPos, int iYPos )
{
	enum { PRINT_XOFFSET = 14, FIRST_YOFFSET = 99, SECOND_YOFFSET = 117,  INVEN_TOOLTIP_ACC_WIDTHCUT_SIZE = 210, };

	if( m_szAccDescArray[0].IsEmpty() )
		return;
	m_szAccDescArray[0].PrintFullTextWidthCut( iXPos + PRINT_XOFFSET, iYPos + FIRST_YOFFSET, TAT_LEFT, INVEN_TOOLTIP_ACC_WIDTHCUT_SIZE );

	if( m_szAccDescArray[1].IsEmpty() )
		return;
	m_szAccDescArray[1].PrintFullTextWidthCut( iXPos + PRINT_XOFFSET, iYPos + SECOND_YOFFSET, TAT_LEFT, INVEN_TOOLTIP_ACC_WIDTHCUT_SIZE );
}

void MyInventoryToolTip::PrintManual( int a_iXPos, int a_iYPos , float fScale )
{
	ManualMgr *pManual = NULL;
	if( m_iMainType == INVENTORY_ITEM_EXTRAITEM )	
		pManual = ioExtraInvenManualMgr::GetSingletonPtr();
	else if( m_iMainType == INVENTORY_ITEM_DECO )
		pManual = ioDecoInvenManualMgr::GetSingletonPtr();
	else if( m_iMainType == INVENTORY_ITEM_ETC )
		pManual = ioEtcInvenManualMgr::GetSingletonPtr();
	else if( m_iMainType == INVENTORY_ITEM_PRESENT )
		pManual = ioPresentInvenManualMgr::GetSingletonPtr();
	else if( m_iMainType == INVENTORY_ITEM_MEDALITEM )
		pManual = ioMedalInvenManualMgr::GetSingletonPtr();
	else if( m_iMainType == INVENTORY_ITEM_COSTUME )
		pManual = ioCostumeInvenManualMgr::GetSingletonPtr();
	else if( m_iMainType == POPUPSTORE_ITEM_PRESENT )
		pManual = ioPopupStorePresentItemManualMgr::GetSingletonPtr();
	else if( m_iMainType == INVENTORY_ITEM_ACCESSORY )
		pManual = ioAccessoryInvenManualMgr::GetSingletonPtr();

#ifdef POPUPSTORE
	else if( m_iMainType == POPUPSTORE_ITEM_PRESENT )
		pManual = ioPopupStorePresentItemManualMgr::GetSingletonPtr();
#endif
	
	if( !pManual )
		return;

	bool bAutoLine = false;
	ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
	if( pLocal && pLocal->IsTooltipAutoLine() )
		bAutoLine = true;


	ioComplexStringPrinter kPrinter;
	int iMaxLine = pManual->GetMaxLine( m_iManualIndex );
	for(int iLine = 1; iLine < iMaxLine; iLine++)
	{
		if( !bAutoLine )
		{
			pManual->SetPrinter( m_iManualIndex, iLine, fScale, kPrinter);
			a_iYPos += pManual->GetSpaceLine( m_iManualIndex, iLine);
			kPrinter.PrintFullTextWidthCut( a_iXPos, a_iYPos, TAT_LEFT, DESC_WIDTH );
			kPrinter.ClearList();
		}
		else
		{
			pManual->SetPrinter( m_iManualIndex, iLine, fScale, kPrinter);
			enum { MAX_LOOP = 20, Y_GAP = 18, };
			bool bFirstGap = true;
			for (int i = 0; i < MAX_LOOP ; i++)
			{
				if( kPrinter.GetFullWidth() < DESC_WIDTH )
				{
					if( bFirstGap )
					{
						a_iYPos += pManual->GetSpaceLine( m_iManualIndex, iLine);
						bFirstGap = false;
					}
					else
						a_iYPos += Y_GAP;
					kPrinter.PrintFullText( a_iXPos, a_iYPos, TAT_LEFT );
					kPrinter.ClearList();
					break;
				}

				ioComplexStringPrinter kPrinterA, kPrinterB;
				kPrinter.SplitFullText( kPrinterA, kPrinterB, DESC_WIDTH );
				if( bFirstGap )
				{
					a_iYPos += pManual->GetSpaceLine( m_iManualIndex, iLine);
					bFirstGap = false;
				}
				else
					a_iYPos += Y_GAP;
				kPrinterA.PrintFullText( a_iXPos, a_iYPos, TAT_LEFT );
				kPrinterB.CopyFullText( kPrinter );
				kPrinterA.ClearList();
				kPrinterB.ClearList();
			}
		}
	}	
}

bool MyInventoryToolTip::SetInfo( InventoryItemInfo &rkInfo )
{
	Clear();

	MToolTipItemInfo kToolTiopItemInfo;
	GetItemInfo( rkInfo, kToolTiopItemInfo );

	if( kToolTiopItemInfo.m_sName.IsEmpty() || kToolTiopItemInfo.m_sIcon.IsEmpty() )
	{
		Clear();
		return false;
	}

	// desc
	m_szDescArray[0].SetTextStyle( TS_NORMAL );
	m_szDescArray[0].SetBkColor( 0, 0, 0 );	
	m_szDescArray[0].SetTextColor( TCT_DEFAULT_DARKGRAY ); // 진회색
	m_szDescArray[0].AddTextPiece( FONT_SIZE_17, kToolTiopItemInfo.m_sName.c_str() );

	m_szDescArray[1].SetTextStyle( TS_NORMAL );
	m_szDescArray[1].SetBkColor( 0, 0, 0);	
	m_szDescArray[1].SetTextColor( TCT_DEFAULT_GRAY ); // 연회색
	if( kToolTiopItemInfo.m_iLimitLevel > 0 )
	{
		char szText[MAX_PATH]="";
		SafeSprintf( szText, sizeof( szText ),  STR(1), kToolTiopItemInfo.m_iLimitLevel );
		m_szDescArray[1].AddTextPiece( FONT_SIZE_13, szText );
	}

	// icon btn
	InventoryIconBtn *pBtn = dynamic_cast< InventoryIconBtn* > ( FindChildWnd( ID_ICON_BTN ) );
	if( !pBtn )
	{
		Clear();
		LOG.PrintTimeAndLog( 0, "%s pBtn == NULL", __FUNCTION__ );
		return false;
	}

	ioUIRenderImage *pIcon = g_UIImageSetMgr.CreateImageByFullName( kToolTiopItemInfo.m_sIcon );
	if( !pBtn->SetIcon( pIcon, FLOAT1, FLOAT1, kToolTiopItemInfo.m_iReinForce ) )
	{
		Clear();
		return false;
	}

	pBtn->SetGradeType( kToolTiopItemInfo.m_nGradeType );

	if ( rkInfo.m_iMainType == INVENTORY_ITEM_COSTUME )
		pBtn->SetCostume( true );

	m_iMainType    = rkInfo.m_iMainType;
	m_iMagicCode   = rkInfo.m_iMagicCode;
	m_iMagicCodeEx = rkInfo.m_iMagicCodeEx;
	m_iMagicCodeEx2= rkInfo.m_iMagicCodeEx2;
	m_iManualIndex = kToolTiopItemInfo.m_iManualIndex;
	for (int i = 0; i < MAX_CHAR_GROWTH ; i++)
		m_iCharGrowth[i] = kToolTiopItemInfo.m_iCharGrowth[i];
	for (int i = 0; i < MAX_ITEM_GROWTH ; i++)
		m_iItemGrowth[i] = kToolTiopItemInfo.m_iItemGrowth[i];

	return true;
}
bool MyInventoryToolTip::SetInfoPopup( MToolTipItemInfo &kToolTiopItemInfo )
{
	Clear();

	if( kToolTiopItemInfo.m_sName.IsEmpty() || kToolTiopItemInfo.m_sIcon.IsEmpty() )
	{
		Clear();
		return false;
	}

	m_iMainType = POPUPSTORE_ITEM_PRESENT;

	// desc
	m_szDescArray[0].SetTextStyle( TS_NORMAL );
	m_szDescArray[0].SetBkColor( 0, 0, 0 );	
	m_szDescArray[0].SetTextColor( TCT_DEFAULT_DARKGRAY ); // 진회색
	m_szDescArray[0].AddTextPiece( FONT_SIZE_17, kToolTiopItemInfo.m_sName.c_str() );

	m_szDescArray[1].SetTextStyle( TS_NORMAL );
	m_szDescArray[1].SetBkColor( 0, 0, 0);	
	m_szDescArray[1].SetTextColor( TCT_DEFAULT_GRAY ); // 연회색
	if( kToolTiopItemInfo.m_iLimitLevel > 0 )
	{
		char szText[MAX_PATH]="";
		SafeSprintf( szText, sizeof( szText ),  STR(1), kToolTiopItemInfo.m_iLimitLevel );
		m_szDescArray[1].AddTextPiece( FONT_SIZE_13, szText );
	}

	// icon btn
	InventoryIconBtn *pBtn = dynamic_cast< InventoryIconBtn* > ( FindChildWnd( ID_ICON_BTN ) );
	if( !pBtn )
	{
		Clear();
		LOG.PrintTimeAndLog( 0, "%s pBtn == NULL", __FUNCTION__ );
		return false;
	}

	ioUIRenderImage *pIcon = g_UIImageSetMgr.CreateImageByFullName( kToolTiopItemInfo.m_sIcon );
	if( !pBtn->SetIcon( pIcon, 1.f, 1.f, kToolTiopItemInfo.m_iReinForce ) )
	{
		Clear();
		return false;
	}

	m_iManualIndex = kToolTiopItemInfo.m_iManualIndex;
	for (int i = 0; i < MAX_CHAR_GROWTH ; i++)
		m_iCharGrowth[i] = kToolTiopItemInfo.m_iCharGrowth[i];
	for (int i = 0; i < MAX_ITEM_GROWTH ; i++)
		m_iItemGrowth[i] = kToolTiopItemInfo.m_iItemGrowth[i];

	return true;
}

void MyInventoryToolTip::iwm_show()
{
}

void MyInventoryToolTip::GetItemInfo( IN const InventoryItemInfo &rkInfo, OUT MToolTipItemInfo &rkReturnInfo )
{
	rkReturnInfo.Clear();

	if( rkInfo.m_iMainType == INVENTORY_ITEM_EXTRAITEM )
		GetExtraItemInfo( rkInfo, rkReturnInfo );
	else if( rkInfo.m_iMainType == INVENTORY_ITEM_DECO )
		GetDecoItemInfo( rkInfo, rkReturnInfo );
	else if( rkInfo.m_iMainType == INVENTORY_ITEM_ETC )
		GetEtcItemInfo( rkInfo, rkReturnInfo );
	else if( rkInfo.m_iMainType == INVENTORY_ITEM_PRESENT )
		GetPresentItemInfo( rkInfo, rkReturnInfo );
	else if( rkInfo.m_iMainType == INVENTORY_ITEM_MEDALITEM )
		GetMedalItemInfo( rkInfo, rkReturnInfo );
	// UI코스튬 관련 (코스튬 정보 얻기)
	if( rkInfo.m_iMainType == INVENTORY_ITEM_COSTUME )
		GetCostumeInfo( rkInfo, rkReturnInfo );
	if( rkInfo.m_iMainType == INVENTORY_ITEM_ACCESSORY )
		GetAccessoryInfo( rkInfo, rkReturnInfo );

}

void MyInventoryToolTip::GetExtraItemInfo( IN const InventoryItemInfo &rkInfo, OUT MToolTipItemInfo &rkReturnInfo )
{	
	int iSlotIndex = rkInfo.m_iMagicCode;
	if( iSlotIndex <= 0 )
	{
		// 대여 장비
		GetExtraItemInfoByItemCode( rkInfo.m_iMagicCodeEx, rkInfo.m_iMagicCodeEx2, rkReturnInfo );
	}
	else
	{
		EXTRAITEMSLOT kSlot;
		ioUserExtraItem *pExtraItem = g_MyInfo.GetUserExtraItem();
		if( !pExtraItem )
			return;

		if( !pExtraItem->GetExtraItem( iSlotIndex, kSlot ) )
			return;

		GetExtraItemInfoByItemCode( kSlot.m_iItemCode, kSlot.m_iReinforce, rkReturnInfo );
	}
}

void MyInventoryToolTip::GetExtraItemInfoByItemCode( IN int iItemCode , IN int iReinforce, OUT MToolTipItemInfo &rkReturnInfo )
{
	const ioItem *pItem = g_ItemMaker.GetItemConst( iItemCode, __FUNCTION__ );
	if( !pItem )
		return;

	rkReturnInfo.m_sName = pItem->GetName();
	rkReturnInfo.m_sIcon = pItem->GetItemLargeIconName();
	rkReturnInfo.m_iLimitLevel = iReinforce * g_ExtraItemInfoMgr.GetLevelLimitConst();
	rkReturnInfo.m_iReinForce  = iReinforce;
	rkReturnInfo.m_iManualIndex = pItem->GetManual() - 1; // index -1
	rkReturnInfo.m_nGradeType = pItem->GetGradeType();

	int iArray = pItem->GetType()-1;
	if( COMPARE( iArray, 0, MAX_CHAR_GROWTH ) )
	{
		if ( iReinforce <= g_CompoundMaterialMgr.GetFixedFailReinforce() )
			rkReturnInfo.m_iCharGrowth[iArray] = iReinforce;
		else
			rkReturnInfo.m_iCharGrowth[iArray] = g_GrowthInfoMgr.GetReinforceAddStat( iReinforce );
	}
	if( COMPARE( iArray, 0, MAX_ITEM_GROWTH ) )
	{
		if ( iReinforce <= g_CompoundMaterialMgr.GetFixedFailReinforce() )
			rkReturnInfo.m_iItemGrowth[iArray] = iReinforce;
		else
			rkReturnInfo.m_iItemGrowth[iArray] = g_GrowthInfoMgr.GetReinforceAddStat( iReinforce );
	}
}

void MyInventoryToolTip::GetDecoItemInfo( IN const InventoryItemInfo &rkInfo, OUT MToolTipItemInfo &rkReturnInfo )
{
	const DecoData *pData =  g_DecorationPrice.GetDecoDataPtr( rkInfo.m_iSexType, rkInfo.m_iDecoType, rkInfo.m_iDecoCode );
	if( !pData ) 
		return;

	rkReturnInfo.m_sName = pData->m_szName;
	rkReturnInfo.m_sIcon = pData->m_szIcon;
	rkReturnInfo.m_iLimitLevel  = pData->m_iLimitLevel;
	rkReturnInfo.m_iManualIndex = pData->m_iManual - 1; // index -1
}

void MyInventoryToolTip::GetEtcItemInfo( IN const InventoryItemInfo &rkInfo, OUT MToolTipItemInfo &rkReturnInfo )
{
	int iItemType = rkInfo.m_iMagicCode;
	if( iItemType <= 0 )
		return;

	ioEtcItem *pEtcItem = g_EtcItemMgr.FindEtcItem( iItemType );
	if( !pEtcItem ) 
		return;

	rkReturnInfo.m_sName = pEtcItem->GetName();
	rkReturnInfo.m_sIcon = pEtcItem->GetIconName();
	rkReturnInfo.m_iManualIndex = pEtcItem->GetInventoryManual() - 1; // index -1
	rkReturnInfo.m_nGradeType = pEtcItem->GetGradeType();
}

void MyInventoryToolTip::GetMedalItemInfo( IN const InventoryItemInfo &rkInfo, OUT MToolTipItemInfo &rkReturnInfo )
{
	int iMedalItemType = rkInfo.m_iMagicCode;

	if( iMedalItemType <= 0 )
		return;

	const ioMedalItemInfoManager::ItemInfo *pItemInfo = g_MedalItemMgr.GetItemInfo( iMedalItemType );
	if( !pItemInfo )
		return;

	rkReturnInfo.m_sName = pItemInfo->m_sName;
	rkReturnInfo.m_sIcon = pItemInfo->m_sIcon;
	rkReturnInfo.m_iLimitLevel  = g_MedalItemMgr.GetLevelLimit( iMedalItemType );
	rkReturnInfo.m_iManualIndex = pItemInfo->m_iManual - 1; // index -1

	for (int i = 0; i < MAX_CHAR_GROWTH ; i++)
		rkReturnInfo.m_iCharGrowth[i] = pItemInfo->m_iCharGrowth[i];

	for (int i = 0; i < MAX_ITEM_GROWTH ; i++)
		rkReturnInfo.m_iItemGrowth[i] = pItemInfo->m_iItemGrowth[i];
}

void MyInventoryToolTip::GetPresentItemInfo( IN const InventoryItemInfo &rkInfo, OUT MToolTipItemInfo &rkReturnInfo )
{
	ioHashString szSendID;
	short sPresentType = 0;
	short sPresentMent = 0;
	int iPresentValue1 = 0;
	int iPresentValue2 = 0;
	int iPresentValue3 = 0;
	int iPresentValue4 = 0;
	DWORD dwLimitDate  = 0;

	g_PresentMgr.GetPresentDataToIndex( rkInfo.m_iMagicCode, rkInfo.m_iMagicCodeEx, szSendID, sPresentType, sPresentMent, iPresentValue1, iPresentValue2, iPresentValue3, iPresentValue4, dwLimitDate );

	if( sPresentType ==  PRESENT_SOLDIER )
	{
		int iClassType      = iPresentValue1;
		DWORD dwSetItemCode = iClassType + SET_ITEM_CODE;
		const ioSetItemInfo *pSetItemInfo = g_SetItemInfoMgr.GetSetInfoByCode( dwSetItemCode );
		if( !pSetItemInfo )
			return;

		rkReturnInfo.m_sName = pSetItemInfo->GetName();
		rkReturnInfo.m_sIcon =  g_MyInfo.GetMySoldierIconName( iClassType );
		rkReturnInfo.m_iLimitLevel  = pSetItemInfo->GetNeedLevel( 0 );        // 0번째 array 정보 ;
		rkReturnInfo.m_iManualIndex = sPresentMent;
		rkReturnInfo.m_nGradeType = g_UIImageRenderMgr.GetPowerUpGradeType( iClassType );
	}
	else if( sPresentType == PRESENT_DECORATION ||
		     sPresentType == PRESENT_RANDOM_DECO )
	{
		InventoryItemInfo kInfo = rkInfo;
		kInfo.m_iMainType = INVENTORY_ITEM_DECO;
		kInfo.m_iSexType  = (iPresentValue1 % 100000) / 1000;
		kInfo.m_iDecoType = iPresentValue1 % 1000;
		kInfo.m_iDecoCode = iPresentValue2;
		GetDecoItemInfo( kInfo, rkReturnInfo );
		rkReturnInfo.m_iManualIndex = sPresentMent;
	}
	else if( sPresentType == PRESENT_ETC_ITEM )
	{
		InventoryItemInfo kInfo = rkInfo;
		kInfo.m_iMainType = INVENTORY_ITEM_ETC;
		kInfo.m_iMagicCode= iPresentValue1;
		GetEtcItemInfo( kInfo, rkReturnInfo );
		rkReturnInfo.m_iManualIndex = sPresentMent;
	}	
	else if( sPresentType == PRESENT_PESO )
	{
		rkReturnInfo.m_sName        = g_PresentMgr.GetPresentValue1Text( sPresentType, iPresentValue1, iPresentValue2 ).c_str();
		rkReturnInfo.m_sIcon        = "UIIconPack13#quest_002";
		rkReturnInfo.m_iManualIndex = sPresentMent;
	}
	else if( sPresentType == PRESENT_EXTRAITEM )
	{	
		GetExtraItemInfoByItemCode( iPresentValue1, (iPresentValue2%PRESENT_EXTRAITEM_DIVISION_1)/PRESENT_EXTRAITEM_DIVISION_2, rkReturnInfo );
		rkReturnInfo.m_iManualIndex = sPresentMent;
	}
	else if( sPresentType == PRESENT_EXTRAITEM_BOX )
	{
		RandomMachineInfo *pInfo = g_ExtraItemInfoMgr.GetRandomMachineInfo( iPresentValue1 );
		if( !pInfo )
			return;

		rkReturnInfo.m_sName = pInfo->GetName();
		rkReturnInfo.m_sIcon = pInfo->GetIconName();
		rkReturnInfo.m_iLimitLevel  = pInfo->GetNeedLevel();
		rkReturnInfo.m_iManualIndex = sPresentMent;
		rkReturnInfo.m_nGradeType = pInfo->GetGradeType();
	}
	else if( sPresentType == PRESENT_GRADE_EXP )
	{
		rkReturnInfo.m_sName = g_PresentMgr.GetPresentValue1Text( sPresentType, iPresentValue1, iPresentValue2 ).c_str();
		rkReturnInfo.m_sIcon = "UIIconPack13#quest_001";
		rkReturnInfo.m_iManualIndex = sPresentMent;
	}
	else if( sPresentType == PRESENT_MEDALITEM )
	{
		InventoryItemInfo kInfo = rkInfo;
		kInfo.m_iMainType = INVENTORY_ITEM_MEDALITEM;
		kInfo.m_iMagicCode= iPresentValue1;
		GetMedalItemInfo( kInfo, rkReturnInfo );
		rkReturnInfo.m_iManualIndex = sPresentMent;
	}
	else if( sPresentType == PRESENT_ALCHEMIC_ITEM )
	{
		rkReturnInfo.m_sName = g_PresentMgr.GetPresentValue1Text( sPresentType, iPresentValue1, iPresentValue2 ).c_str();
		rkReturnInfo.m_sIcon = g_AlchemicMgr.GetAlchemicItemIcon( iPresentValue1 );
		rkReturnInfo.m_iManualIndex = sPresentMent;
	}
	else if( sPresentType == PRESENT_PET )
	{
		int nPetRank = iPresentValue2%10000;
		rkReturnInfo.m_sName = g_PresentMgr.GetPresentValue1Text( sPresentType, iPresentValue1, iPresentValue2 ).c_str();
		rkReturnInfo.m_sIcon = g_PetInfoMgr.GetPetIcon( iPresentValue1, (PetRankType)nPetRank );
		rkReturnInfo.m_iManualIndex = sPresentMent;
	}
	else if( sPresentType == PRESENT_COSTUME )
	{
		// UI코스튬 관련 (선물 툴팁 설정)
		rkReturnInfo.m_sName = g_PresentMgr.GetPresentValue1Text( sPresentType, iPresentValue1, iPresentValue2 ).c_str();
		rkReturnInfo.m_sIcon = g_CostumeInfoMgr.GetCostumeIcon( iPresentValue1 );
		rkReturnInfo.m_iManualIndex = sPresentMent;
		rkReturnInfo.m_nGradeType = g_CostumeInfoMgr.GetGradeType( iPresentValue1 );
	}
	else if( sPresentType == PRESENT_COSTUME_BOX )
	{
		CostumeShopInfo *pInfo = g_CostumeShopInfoMgr.GetCostumeMachineInfo( iPresentValue1 );
		if( !pInfo )
			return;

		rkReturnInfo.m_sName = pInfo->GetName();
		rkReturnInfo.m_sIcon = pInfo->GetIconName();
		rkReturnInfo.m_iLimitLevel  = pInfo->GetNeedLevel();
		rkReturnInfo.m_iManualIndex = sPresentMent;
		rkReturnInfo.m_nGradeType = pInfo->GetGradeType();
	}
	else if ( sPresentType == PRESENT_ACCESSORY )
	{
		const ioItem *pItem = g_ItemMaker.GetItemConst( iPresentValue1, __FUNCTION__ );
		if( !pItem )
			return;

		rkReturnInfo.m_sName = pItem->GetName();
		rkReturnInfo.m_sIcon = pItem->GetItemLargeIconName();
		rkReturnInfo.m_iManualIndex = sPresentMent;
		rkReturnInfo.m_nGradeType = pItem->GetGradeType();
	}
}

void MyInventoryToolTip::GetCostumeInfo( IN const InventoryItemInfo &rkInfo, OUT MToolTipItemInfo &rkReturnInfo )
{	
	int iSlotIndex = rkInfo.m_iMagicCode;

	CostumeSlot kSlot;
	ioUserCostume *pCostume = g_MyInfo.GetUserCostume();
	if( !pCostume )
		return;

	if( !pCostume->GetCostumeSlot( iSlotIndex, kSlot ) )
		return;

	CostumeInfo sInfo;
	if( !g_CostumeInfoMgr.GetCostumeInfo( kSlot.m_nCostumeCode, sInfo ) )
		return;

	rkReturnInfo.m_sName = sInfo.m_szName;
	rkReturnInfo.m_sIcon = sInfo.m_szIconName;
	rkReturnInfo.m_iManualIndex = sInfo.m_nManual-1;
	rkReturnInfo.m_nGradeType = sInfo.m_nGradeType;
}

void MyInventoryToolTip::GetAccessoryInfo( IN const InventoryItemInfo &rkInfo, OUT MToolTipItemInfo &rkReturnInfo )
{
	int iSlotIndex = rkInfo.m_iMagicCode;

	AccessorySlot kSlot;
	ioUserAccessory *pAccessory = g_MyInfo.GetUserAccessory();
	if( !pAccessory )
		return;

	if( !pAccessory->GetAccessorySlot( iSlotIndex, kSlot ) )
		return;
	const ioItem* pItem = g_ItemMaker.GetItemConst( kSlot.m_iAccessoryCode, __FUNCTION__ );
	if ( !pItem )
		return;
	int iSlot = ES_RING + kSlot.m_iAccessoryCode/DEFAULT_BASIC_ACCESSORY_CODE - 1;

	ioHashStringVec vTextList;
	if ( iSlot == ES_RING )
	{
		const ioRingItem* pRing = ToRingItemConst( pItem );
		if ( !pRing )
			return;
		rkReturnInfo.m_iManualIndex = pRing->GetManualIndex();
		vTextList = pRing->GetToolTipText();
	}
	else if ( iSlot == ES_NECKLACE )
	{
		const ioNecklaceItem* pNeck = ToNecklaceItemConst( pItem );
		if ( !pNeck )
			return;
		rkReturnInfo.m_iManualIndex = pNeck->GetManualIndex();
		vTextList = pNeck->GetToolTipText();
	}
	else if ( iSlot == ES_BRACELET )
	{
		const ioBraceletItem* pBracelet = ToBraceletItemConst( pItem );
		if ( !pBracelet )
			return;
		rkReturnInfo.m_iManualIndex = pBracelet->GetManualIndex();
		vTextList = pBracelet->GetToolTipText();
	}

	m_iMaxAccDesc = vTextList.size();
	if ( !COMPARE( m_iMaxAccDesc, 0, MAX_ACC_DESC + 1 ) )
		return;

	float fRanNum = kSlot.m_iRandomNumber/1000.0f;
	for ( int i=0 ; i<m_iMaxAccDesc ; ++i )
	{
		m_szAccDescArray[i].SetTextStyle( TS_NORMAL );
		m_szAccDescArray[i].SetBkColor( 0, 0, 0 );	
		m_szAccDescArray[i].SetTextColor( TCT_DEFAULT_DARKGRAY );
		if ( i == 0 )
			m_szAccDescArray[i].AddTextPiece( FONT_SIZE_13, vTextList[i].c_str(), fRanNum );
		else if( i == 1 )
			m_szAccDescArray[i].AddTextPiece( FONT_SIZE_12, vTextList[i].c_str() );
	}
	rkReturnInfo.m_sName = kSlot.m_Name;
	rkReturnInfo.m_sIcon = kSlot.m_IconName;
	rkReturnInfo.m_nGradeType = pItem->GetGradeType();


}

DWORD MyInventoryToolTip::GetUseInfo( OUT ioHashString &rsInfo )
{
	if( m_iMainType == INVENTORY_ITEM_EXTRAITEM )
	{
		if( m_iMagicCode == 0 )
			return TCT_DEFAULT_RED;
		else if( m_iMagicCode < 0 )
		{
			rsInfo = STR(3);
			return TCT_DEFAULT_BLUE;
		}
		else
		{
			ioUserExtraItem *pExtraItem = g_MyInfo.GetUserExtraItem();
			if( !pExtraItem )
			{
				LOG.PrintTimeAndLog( 0, "%s UserExtraItem == NULL", __FUNCTION__ );
				return TCT_DEFAULT_RED;
			}

			EXTRAITEMSLOT kSlot;
			if( !pExtraItem->GetExtraItem( m_iMagicCode, kSlot ) )
			{
				LOG.PrintTimeAndLog( 0, "%s No ExtraItem : %d", __FUNCTION__, m_iMagicCode );
				return TCT_DEFAULT_RED;
			}

			pExtraItem->GetUseInfo( m_iMagicCode, rsInfo );

			if( kSlot.m_PeriodType == EPT_MORTMAIN )
				return TCT_DEFAULT_BLUE;
		}
		return TCT_DEFAULT_RED;
	}
	else if( m_iMainType == INVENTORY_ITEM_DECO )
	{
		rsInfo = STR(1);
		return TCT_DEFAULT_BLUE;
	}
	else if( m_iMainType == INVENTORY_ITEM_ETC )
	{
		ioUserEtcItem *pUserEtcItem =  g_MyInfo.GetUserEtcItem();
		if( !pUserEtcItem )
		{
			LOG.PrintTimeAndLog( 0, "%s UserEtcItem == NULL , Type:%d", __FUNCTION__, m_iMagicCode );
			return TCT_DEFAULT_RED;
		}

		// 1번만 사용
		DWORD sInfo1Color;
		DWORD sInfo2Color;
		ioHashString sInfo2;
		pUserEtcItem->GetUseInfo( m_iMagicCode, rsInfo, sInfo2, sInfo1Color, sInfo2Color );
	}
	else if( m_iMainType == INVENTORY_ITEM_PRESENT )
	{
		ioHashString szSendID;
		short sPresentType = 0;
		short sPresentMent = 0;
		int iPresentValue1 = 0;
		int iPresentValue2 = 0;
		int iPresentValue3 = 0;
		int iPresentValue4 = 0;
		DWORD dwLimitDate  = 0;

		g_PresentMgr.GetPresentDataToIndex( m_iMagicCode, m_iMagicCodeEx, szSendID,	sPresentType, sPresentMent, iPresentValue1, iPresentValue2, iPresentValue3, iPresentValue4, dwLimitDate );
		rsInfo = g_PresentMgr.GetLmitDateText( dwLimitDate );
	}
	else if( m_iMainType == INVENTORY_ITEM_MEDALITEM )
	{
		ioUserMedalItem::MEDALITEMSLOT kSlot;
		ioUserMedalItem *pMedalItem = g_MyInfo.GetUserMedalItem();
		if( !pMedalItem )
		{
			LOG.PrintTimeAndLog( 0, "%s UserMedalItem == NULL", __FUNCTION__ );
			return TCT_DEFAULT_RED;
		}

		pMedalItem->GetUseInfo( m_iMagicCode, rsInfo );
		char szTemp[MAX_PATH]="";
		SafeSprintf( szTemp, sizeof( szTemp ), STR(2), rsInfo.c_str() );
		rsInfo = szTemp;
		return TCT_DEFAULT_BLUE;
	}
	else if( m_iMainType == INVENTORY_ITEM_COSTUME )
	{
		// UI코스튬 (툴팁 정보 얻기)
		if( m_iMagicCode == 0 )
			return TCT_DEFAULT_RED;
		else if( m_iMagicCode < 0 )
		{
			rsInfo = STR(3);
			return TCT_DEFAULT_BLUE;
		}
		else
		{
			ioUserCostume *pCostume = g_MyInfo.GetUserCostume();
			if( !pCostume )
			{
				LOG.PrintTimeAndLog( 0, "%s UserCostume == NULL", __FUNCTION__ );
				return TCT_DEFAULT_RED;
			}

			CostumeSlot kSlot;
			if( !pCostume->GetCostumeSlot( m_iMagicCode, kSlot ) )
			{
				LOG.PrintTimeAndLog( 0, "%s No Costume : %d", __FUNCTION__, m_iMagicCode );
				return TCT_DEFAULT_RED;
			}

			pCostume->GetUseInfo( m_iMagicCode, rsInfo );

			if( kSlot.m_PeriodType == PCPT_MORTMAIN )
				return TCT_DEFAULT_BLUE;
		}
		return TCT_DEFAULT_RED;
	}
	else if( m_iMainType == INVENTORY_ITEM_ACCESSORY )
	{
		if( m_iMagicCode == 0 )
			return TCT_DEFAULT_RED;
		else if( m_iMagicCode < 0 )
		{
			rsInfo = STR(3);
			return TCT_DEFAULT_BLUE;
		}
		else
		{
			ioUserAccessory *pAccessory = g_MyInfo.GetUserAccessory();
			if( !pAccessory )
			{
				LOG.PrintTimeAndLog( 0, "%s UserAccessory == NULL", __FUNCTION__ );
				return TCT_DEFAULT_RED;
			}

			AccessorySlot kSlot;
			if( !pAccessory->GetAccessorySlot( m_iMagicCode, kSlot ) )
			{
				LOG.PrintTimeAndLog( 0, "%s No Accessory : %d", __FUNCTION__, m_iMagicCode );
				return TCT_DEFAULT_RED;
			}

			pAccessory->GetUseInfo( m_iMagicCode, rsInfo );

			if( kSlot.m_PeriodType == ACCPT_MORTMAIN )
				return TCT_DEFAULT_BLUE;
		}
		return TCT_DEFAULT_RED;
	}

	return TCT_DEFAULT_RED;
}

void MyInventoryToolTip::ProcessDesc()
{
	m_szDescArray[2].ClearList();

	ioHashString sInfo;
	DWORD dwColor = GetUseInfo( sInfo );
	if( sInfo.IsEmpty() )
		return;

	m_szDescArray[2].SetTextStyle( TS_NORMAL );
	m_szDescArray[2].SetBkColor( 0, 0, 0 );	
	m_szDescArray[2].SetTextColor( dwColor );
	m_szDescArray[2].AddTextPiece( FONT_SIZE_13, sInfo.c_str() );
}

void MyInventoryToolTip::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "GrowthLineLeft" )
	{
		SAFEDELETE( m_pGrowthLineLeft );
		m_pGrowthLineLeft = pImage;
	}
	else if( szType == "GrowthLineCenter" )
	{
		SAFEDELETE( m_pGrowthLineCenter );
		m_pGrowthLineCenter = pImage;
	}
	else if( szType == "GrowthLineRight" )
	{
		SAFEDELETE( m_pGrowthLineRight );
		m_pGrowthLineRight = pImage;
		if( m_pGrowthLineRight )
			m_pGrowthLineRight->SetReverseFormatting( ioUIRenderImage::RF_HORZ_REVERSE );
	}
	else if( szType == "GrowthBackGray" )
	{
		SAFEDELETE( m_pGrowthBackGray );
		m_pGrowthBackGray = pImage;
	}
	else if( szType == "GrowthBackLight" )
	{
		SAFEDELETE( m_pGrowthBackLight );
		m_pGrowthBackLight = pImage;
	}
	else
	{
		ioWnd::AddRenderImage( szType, pImage );	
	}
}

void MyInventoryToolTip::RenderGrowth( int iXPos, int iYPos )
{
	if( m_iMainType != INVENTORY_ITEM_EXTRAITEM && 
		m_iMainType != INVENTORY_ITEM_MEDALITEM )
		return;

	if( !m_pGrowthLineLeft  ||
		!m_pGrowthLineCenter||
		!m_pGrowthLineRight ||
		!m_pGrowthBackGray  ||
		!m_pGrowthBackLight )
		return;

	enum 
	{
		GROWTH_XOFFSET       = 116,
		GROWTH_YOFFSET       = -4,

		LEFT_XOFFSET         = 14, 
		CENTER_LEFT_XOFFSET  = 21,
		CENTER_RIGHT_XOFFSET = 136,
		RIGHT_XOFFSET        = 212,

		CENTER_WIDTH         = 76,
		CENTER_HEIGHT        = 10,

		LEFT_BACK_XOFFSET    = 14,
		RIGHT_BACK_XOFFSET   = 117,
		BACK_YOFFSET         = 12,
		YGAP                 = 21,

		BACK_WIDTH           = 102,
		BACK_HEIGHT          = 21,

		MAX_BACK             = 4,

		LEFT_TEXT_XOFFSET    = 19,
		RIGHT_TEXT_XOFFSET   = 122,
		TEXT_YOFFSET         = 16,
	};

	// |------- 능력치 --------|
	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextColor( TCT_DEFAULT_LIGHTGRAY );
	g_FontMgr.PrintText( iXPos + GROWTH_XOFFSET, iYPos + GROWTH_YOFFSET, FONT_SIZE_11,  STR(1) );

	m_pGrowthLineLeft->Render( iXPos + LEFT_XOFFSET, iYPos );
	// left center
	m_pGrowthLineCenter->SetSize( CENTER_WIDTH, CENTER_HEIGHT );
	m_pGrowthLineCenter->SetHorzFormatting( ioUIRenderImage::HF_STRETCH );
	m_pGrowthLineCenter->Render( iXPos + CENTER_LEFT_XOFFSET, iYPos );

	// right center
	m_pGrowthLineCenter->Render( iXPos + CENTER_RIGHT_XOFFSET, iYPos );
	m_pGrowthLineRight->Render( iXPos + RIGHT_XOFFSET, iYPos );

	// back
	POINT ptGrayOffset[MAX_BACK]={ LEFT_BACK_XOFFSET, BACK_YOFFSET, RIGHT_BACK_XOFFSET, BACK_YOFFSET, 
		LEFT_BACK_XOFFSET, BACK_YOFFSET+(YGAP*2), RIGHT_BACK_XOFFSET, BACK_YOFFSET+(YGAP*2) };
	POINT ptLightOffset[MAX_BACK]={ LEFT_BACK_XOFFSET, BACK_YOFFSET+YGAP, RIGHT_BACK_XOFFSET, BACK_YOFFSET+YGAP, 
		LEFT_BACK_XOFFSET, BACK_YOFFSET+(YGAP*3), RIGHT_BACK_XOFFSET, BACK_YOFFSET+(YGAP*3) };

	for (int i = 0; i < MAX_BACK ; i++)
	{
		m_pGrowthBackGray->SetHorzFormatting( ioUIRenderImage::HF_STRETCH );
		m_pGrowthBackGray->SetVertFormatting( ioUIRenderImage::VF_STRETCH );
		m_pGrowthBackGray->SetSize( BACK_WIDTH, BACK_HEIGHT );
		m_pGrowthBackGray->Render( iXPos + ptGrayOffset[i].x, iYPos + ptGrayOffset[i].y, UI_RENDER_MULTIPLY );

		m_pGrowthBackLight->SetHorzFormatting( ioUIRenderImage::HF_STRETCH );
		m_pGrowthBackLight->SetVertFormatting( ioUIRenderImage::VF_STRETCH );
		m_pGrowthBackLight->SetSize( BACK_WIDTH, BACK_HEIGHT );
		m_pGrowthBackLight->Render( iXPos + ptLightOffset[i].x, iYPos + ptLightOffset[i].y, UI_RENDER_MULTIPLY );
	}

	// text
	char szCharGrowthArray[MAX_CHAR_GROWTH][MAX_PATH]={ "", "", "", "" };
	for (int i = 0; i < MAX_CHAR_GROWTH ; i++) // 스트링 추출을 위해서 loop을 돌지 않고 처리
	{
		if( i == 0 )
			SafeSprintf( szCharGrowthArray[i] , sizeof( szCharGrowthArray[i]), STR(2) );
		else if( i == 1 )
			SafeSprintf( szCharGrowthArray[i] , sizeof( szCharGrowthArray[i]), STR(3) );
		else if( i == 2 )
			SafeSprintf( szCharGrowthArray[i] , sizeof( szCharGrowthArray[i]), STR(4) );
		else if( i == 3 )
			SafeSprintf( szCharGrowthArray[i] , sizeof( szCharGrowthArray[i]), STR(5) );
	}

	for (int i = 0; i < MAX_CHAR_GROWTH ; i++)
	{
		ioComplexStringPrinter kPrinter;
		kPrinter.SetTextStyle( TS_NORMAL);
		kPrinter.SetBkColor( 0, 0, 0 );	
		if( m_iCharGrowth[i] == 0 )
		{
			kPrinter.SetTextColor( TCT_DEFAULT_LIGHTGRAY );
			kPrinter.AddTextPiece( FONT_SIZE_12, szCharGrowthArray[i] );
		}
		else
		{
			kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
			kPrinter.AddTextPiece( FONT_SIZE_12, szCharGrowthArray[i] );
			kPrinter.SetTextColor( TCT_DEFAULT_GREEN );
			if( m_iCharGrowth[i] > 0 )
				kPrinter.AddTextPiece( FONT_SIZE_12, STR(6), m_iCharGrowth[i] );
			else
				kPrinter.AddTextPiece( FONT_SIZE_12, STR(7), m_iCharGrowth[i] );
		}
		kPrinter.PrintFullText( iXPos+RIGHT_TEXT_XOFFSET, iYPos+TEXT_YOFFSET+(YGAP*i), TAT_LEFT );	
	}

	char szItemGrowthArray[MAX_ITEM_GROWTH][MAX_PATH]={ "", "", "", "" };
	for (int i = 0; i < MAX_ITEM_GROWTH ; i++) // 스트링 추출을 위해서 loop을 돌지 않고 처리
	{
		if( i == 0 )
			SafeSprintf( szItemGrowthArray[i] , sizeof( szItemGrowthArray[i]), STR(8) );
		else if( i == 1 )
			SafeSprintf( szItemGrowthArray[i] , sizeof( szItemGrowthArray[i]), STR(9) );
		else if( i == 2 )
			SafeSprintf( szItemGrowthArray[i] , sizeof( szItemGrowthArray[i]), STR(10) );
		else if( i == 3 )
			SafeSprintf( szItemGrowthArray[i] , sizeof( szItemGrowthArray[i]), STR(11) );
	}
	for (int i = 0; i < MAX_ITEM_GROWTH ; i++)
	{
		ioComplexStringPrinter kPrinter;
		kPrinter.SetTextStyle( TS_NORMAL);
		kPrinter.SetBkColor( 0, 0, 0 );	
		if( m_iItemGrowth[i] == 0 )
		{
			kPrinter.SetTextColor( TCT_DEFAULT_LIGHTGRAY );
			kPrinter.AddTextPiece( FONT_SIZE_12, szItemGrowthArray[i] );
		}
		else
		{
			kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
			kPrinter.AddTextPiece( FONT_SIZE_12, szItemGrowthArray[i] );
			kPrinter.SetTextColor( TCT_DEFAULT_GREEN );
			if( m_iItemGrowth[i] > 0 )
				kPrinter.AddTextPiece( FONT_SIZE_12, STR(12), m_iItemGrowth[i] );
			else
				kPrinter.AddTextPiece( FONT_SIZE_12, STR(13), m_iItemGrowth[i] );
		}
		kPrinter.PrintFullText( iXPos+LEFT_TEXT_XOFFSET, iYPos+TEXT_YOFFSET+(YGAP*i), TAT_LEFT );	
	}
}

void MyInventoryToolTip::Clear()
{
	for (int i = 0; i < MAX_CHAR_GROWTH ; i++)
		m_iCharGrowth[i] = 0;
	for (int i = 0; i < MAX_ITEM_GROWTH ; i++)
		m_iItemGrowth[i] = 0;

	ClearDesc();

	m_iMainType    = 0;
	m_iMagicCode   = 0;
	m_iMagicCodeEx = 0;
	m_iManualIndex = 0;
}

void MyInventoryToolTip::UpdateWndSize()
{
	ManualMgr *pManual = NULL;
	if( m_iMainType == INVENTORY_ITEM_EXTRAITEM )	
		pManual = ioExtraInvenManualMgr::GetSingletonPtr();
	else if( m_iMainType == INVENTORY_ITEM_DECO )
		pManual = ioDecoInvenManualMgr::GetSingletonPtr();
	else if( m_iMainType == INVENTORY_ITEM_ETC )
		pManual = ioEtcInvenManualMgr::GetSingletonPtr();
	else if( m_iMainType == INVENTORY_ITEM_PRESENT )
		pManual = ioPresentInvenManualMgr::GetSingletonPtr();
	else if( m_iMainType == INVENTORY_ITEM_MEDALITEM )
		pManual = ioMedalInvenManualMgr::GetSingletonPtr();
	else if( m_iMainType == INVENTORY_ITEM_COSTUME )
		pManual = ioCostumeInvenManualMgr::GetSingletonPtr();
	else if( m_iMainType == POPUPSTORE_ITEM_PRESENT )
		pManual = ioPopupStorePresentItemManualMgr::GetSingletonPtr();
	else if( m_iMainType == INVENTORY_ITEM_ACCESSORY )
		pManual = ioAccessoryInvenManualMgr::GetSingletonPtr();

	if( !pManual )
		return;

	bool bAutoLine = false;
	ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
	if( pLocal && pLocal->IsTooltipAutoLine() )
		bAutoLine = true;

	if( !bAutoLine )
	{
		int iMaxLine = pManual->GetMaxLine( m_iManualIndex );
		iMaxLine--; // 1번째는 타이틀이므로 삭제
		if( iMaxLine <= 0 )
			iMaxLine = 0;

		enum { DEFAULT_HEIGHT = 123, LINE_GAP = 18, GROWTH_HEIGHT = 108, };
		int iGrwothHeight = 0;
		if( m_iMainType == INVENTORY_ITEM_EXTRAITEM ||
			m_iMainType == INVENTORY_ITEM_MEDALITEM )
			iGrwothHeight = GROWTH_HEIGHT;

		int iAccSize = m_iMaxAccDesc * ACC_LINE_GAP;
		SetSize( GetWidth(), DEFAULT_HEIGHT+( LINE_GAP * iMaxLine) + iGrwothHeight + iAccSize );
	}
	else
	{
		int iMaxLine = pManual->GetMaxLine( m_iManualIndex );
		int iNewLine = iMaxLine - 1; // 1번째는 타이틀이므로 삭제
		if( iNewLine <= 0 )
			iNewLine = 0;

		ioComplexStringPrinter kPrinter;
		int iTotalLineGap = 18;
		if( iNewLine > 0 )
		{
			for(int iLine = 1; iLine < iMaxLine; iLine++)
			{
				int iGap = pManual->GetSpaceLine( m_iManualIndex, iLine);
				enum { MAX_LOOP = 20, Y_GAP = 18, };
				bool bFirstGap = true;
				for (int i = 0; i < MAX_LOOP ; i++)
				{
					if( kPrinter.GetFullWidth() < DESC_WIDTH )
					{
						if( bFirstGap )
						{
							iTotalLineGap += pManual->GetSpaceLine( m_iManualIndex, iLine);
							bFirstGap = false;
						}
						else
							iTotalLineGap += Y_GAP;
						kPrinter.ClearList();
						break;
					}

					ioComplexStringPrinter kPrinterA, kPrinterB;
					kPrinter.SplitFullText( kPrinterA, kPrinterB, DESC_WIDTH );
					if( bFirstGap )
					{
						iTotalLineGap += pManual->GetSpaceLine( m_iManualIndex, iLine);
						bFirstGap = false;
					}
					else
						iTotalLineGap += Y_GAP;
					kPrinterB.CopyFullText( kPrinter );
					kPrinterA.ClearList();
					kPrinterB.ClearList();
				}
			}
		}

		enum { DEFAULT_HEIGHT = 123, GROWTH_HEIGHT = 108, };

		int iGrwothHeight = 0;
		if( m_iMainType == INVENTORY_ITEM_EXTRAITEM ||
			m_iMainType == INVENTORY_ITEM_MEDALITEM )
			iGrwothHeight = GROWTH_HEIGHT;

		int iAccSize = m_iMaxAccDesc * ACC_LINE_GAP;
		SetSize( GetWidth(), DEFAULT_HEIGHT+ iTotalLineGap + iGrwothHeight + iAccSize );
	}
}

void MyInventoryToolTip::OnProcessPopup()
{
	ioLSMouse *pMouse = g_App.GetMouse();
	if( !pMouse ) return;

	PopupStoreWnd* pWnd = dynamic_cast<PopupStoreWnd*> ( g_GUIMgr.FindWnd( POPUP_STORE_WND ) );

	MToolTipItemInfo kInfo;
	if( pWnd && pMouse->GetToolTipWndID() == PopupStoreWnd::eID_ITEM_IMG )
	{
		PopupStoreWnd::sPopupItemInfo pInfo = pWnd->GetPopupItemInfo();

		GetPopupItemInfo( pInfo.iPresentType, pInfo.iManualIdx, pInfo.iPresentValue1, pInfo.iPresentValue2, kInfo );
		//GetItemInfo( rkInfo, kToolTiopItemInfo );
	}

	if( IsShow() )
	{	
		if( !SetInfoPopup( kInfo ) )                                          
			HideWnd();
		else
		{
			//pMouse->SetToolTipWndID( 0 );
			pMouse->SetToolTipCursor();
			UpdateWndSize();
			int iXPos = 0;
			int iYPos = 0;
			Help::GetNotCutWndPos( pMouse->GetMousePos().x, pMouse->GetMousePos().y, GetWidth(), GetHeight(), iXPos, iYPos );
			SetWndPos( iXPos, iYPos );

		}

		ProcessDesc();
	}
	else
	{
		if( SetInfoPopup( kInfo ) ) // setinfo 후에 showwnd
		{
			ShowWnd();
			UpdateWndSize();
			int iXPos = 0;
			int iYPos = 0;
			Help::GetNotCutWndPos( pMouse->GetMousePos().x, pMouse->GetMousePos().y, GetWidth(), GetHeight(), iXPos, iYPos );
			SetWndPos( iXPos, iYPos );
		}
	}
}
void MyInventoryToolTip::GetPopupItemInfo( int iPresentType, int iMent, int iPresentValue1, int iPresentValue2, OUT MToolTipItemInfo &rkReturnInfo )
{
	short sPresentType = iPresentType;
	short sPresentMent = iMent;
	//int iPresentValue1 = iPresentValue1;
	//int iPresentValue2 = iPresentValue2;
	//int iPresentValue3 = 0;
	//int iPresentValue4 = 0;
	//DWORD dwLimitDate  = 0;

	InventoryItemInfo rkInfo;
	if( sPresentType == PRESENT_SOLDIER )
	{
		int iClassType      = iPresentValue1;
		DWORD dwSetItemCode = iClassType + SET_ITEM_CODE;
		const ioSetItemInfo *pSetItemInfo = g_SetItemInfoMgr.GetSetInfoByCode( dwSetItemCode );
		if( !pSetItemInfo )
			return;

		rkReturnInfo.m_sName = pSetItemInfo->GetName();
		rkReturnInfo.m_sIcon =  g_MyInfo.GetMySoldierIconName( iClassType );
		rkReturnInfo.m_iLimitLevel  = pSetItemInfo->GetNeedLevel( 0 );        // 0번째 array 정보 ;
		rkReturnInfo.m_iManualIndex = sPresentMent;
		rkReturnInfo.m_nGradeType = g_UIImageRenderMgr.GetPowerUpGradeType( iClassType );
	}
	else if( sPresentType == PRESENT_DECORATION ||
		sPresentType == PRESENT_RANDOM_DECO )
	{
		InventoryItemInfo kInfo = rkInfo;
		kInfo.m_iMainType = INVENTORY_ITEM_DECO;
		kInfo.m_iSexType  = (iPresentValue1 % 100000) / 1000;
		kInfo.m_iDecoType = iPresentValue1 % 1000;
		kInfo.m_iDecoCode = iPresentValue2;
		GetDecoItemInfo( kInfo, rkReturnInfo );
		rkReturnInfo.m_iManualIndex = sPresentMent;
	}
	else if( sPresentType == PRESENT_ETC_ITEM )
	{
		InventoryItemInfo kInfo = rkInfo;
		kInfo.m_iMainType = INVENTORY_ITEM_ETC;
		kInfo.m_iMagicCode= iPresentValue1;
		GetEtcItemInfo( kInfo, rkReturnInfo );
		rkReturnInfo.m_iManualIndex = sPresentMent;
	}	
	else if( sPresentType == PRESENT_PESO )
	{
		rkReturnInfo.m_sName        = g_PresentMgr.GetPresentValue1Text( sPresentType, iPresentValue1, iPresentValue2 ).c_str();
		rkReturnInfo.m_sIcon        = "UIIconPack13#quest_002";
		rkReturnInfo.m_iManualIndex = sPresentMent;
	}
	else if( sPresentType == PRESENT_EXTRAITEM )
	{	
		GetExtraItemInfoByItemCode( iPresentValue1, (iPresentValue2%PRESENT_EXTRAITEM_DIVISION_1)/PRESENT_EXTRAITEM_DIVISION_2, rkReturnInfo );
		rkReturnInfo.m_iManualIndex = sPresentMent;
	}
	else if( sPresentType == PRESENT_EXTRAITEM_BOX )
	{
		RandomMachineInfo *pInfo = g_ExtraItemInfoMgr.GetRandomMachineInfo( iPresentValue1 );
		if( !pInfo )
			return;

		rkReturnInfo.m_sName = pInfo->GetName();
		rkReturnInfo.m_sIcon = pInfo->GetIconName();
		rkReturnInfo.m_iLimitLevel  = pInfo->GetNeedLevel();
		rkReturnInfo.m_iManualIndex = sPresentMent;
		rkReturnInfo.m_nGradeType = pInfo->GetGradeType();
	}
	else if( sPresentType == PRESENT_GRADE_EXP )
	{
		rkReturnInfo.m_sName = g_PresentMgr.GetPresentValue1Text( sPresentType, iPresentValue1, iPresentValue2 ).c_str();
		rkReturnInfo.m_sIcon = "UIIconPack13#quest_001";
		rkReturnInfo.m_iManualIndex = sPresentMent;
	}
	else if( sPresentType ==PRESENT_MEDALITEM )
	{
		InventoryItemInfo kInfo = rkInfo;
		kInfo.m_iMainType = INVENTORY_ITEM_MEDALITEM;
		kInfo.m_iMagicCode= iPresentValue1;
		GetMedalItemInfo( kInfo, rkReturnInfo );
		rkReturnInfo.m_iManualIndex = sPresentMent;
	}
	else if( sPresentType ==PRESENT_ALCHEMIC_ITEM )
	{
		rkReturnInfo.m_sName = g_PresentMgr.GetPresentValue1Text( sPresentType, iPresentValue1, iPresentValue2 ).c_str();
		rkReturnInfo.m_sIcon = g_AlchemicMgr.GetAlchemicItemIcon( iPresentValue1 );
		rkReturnInfo.m_iManualIndex = sPresentMent;
	}
	else if( sPresentType ==PRESENT_PET )
	{
		int nPetRank = iPresentValue2%10000;
		rkReturnInfo.m_sName = g_PresentMgr.GetPresentValue1Text( sPresentType, iPresentValue1, iPresentValue2 ).c_str();
		rkReturnInfo.m_sIcon = g_PetInfoMgr.GetPetIcon( iPresentValue1, (PetRankType)nPetRank );
		rkReturnInfo.m_iManualIndex = sPresentMent;
	}
}

//////////////////////////////////////////////////////////////////////////

ExtraItemEquipConfirmWnd::ExtraItemEquipConfirmWnd()
{
	m_pExclamationMark = NULL;
	m_pPlayStage = NULL;
}

ExtraItemEquipConfirmWnd::~ExtraItemEquipConfirmWnd()
{
	SAFEDELETE( m_pExclamationMark );
}

void ExtraItemEquipConfirmWnd::AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame )
{
	ioMovingWnd::AddRenderFrame( szType, pFrame );
}

void ExtraItemEquipConfirmWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "ExclamationMark" )
	{
		SAFEDELETE( m_pExclamationMark );
		m_pExclamationMark = pImage;
	}
	else
	{
		ioMovingWnd::AddRenderImage( szType, pImage );	
	}
}

bool ExtraItemEquipConfirmWnd::SetExtraItemEquipInfo( int iCharArray, int iSlot, int iSlotIndex )
{
	EXTRAITEMSLOT kSlot;
	ioUserExtraItem *pExtraItem = g_MyInfo.GetUserExtraItem();
	if( pExtraItem && pExtraItem->GetExtraItem( iSlotIndex, kSlot ) )
	{
		m_szName = kSlot.m_Name;

		m_iCharArray = iCharArray;
		m_iEquipSlot = iSlot;
		m_iExtraItemIndex = iSlotIndex;

		m_iItemCode = kSlot.m_iItemCode;
		m_iPeriodType = kSlot.m_PeriodType;

		ioUIRenderImage *pIcon = g_UIImageSetMgr.CreateImageByFullName( kSlot.m_IconName );
		TradeIconBtn *pIconBtn = dynamic_cast<TradeIconBtn*>(FindChildWnd(ID_ICON_BTN));
		if( pIconBtn )
		{
			pIconBtn->SetIcon( pIcon, FLOAT1, FLOAT1, kSlot.m_iReinforce );

			const ioItem *pItem = g_ItemMaker.GetItemConst( kSlot.m_iItemCode, __FUNCTION__ );
			if ( pItem )
				pIconBtn->SetGradeType( pItem->GetGradeType() );

			pIconBtn->SetMarkType( TRADE_MARK_TYPE_DISABLE );

			if( kSlot.m_dwMaleCustom > 0 || kSlot.m_dwFemaleCustom > 0 )
				pIconBtn->SetCustomItem( true );
			else
				pIconBtn->SetCustomItem( false );
		}
	}
	else
	{
		HideWnd();
		return false;
	}

	if( !IsShow() )
		ShowWnd();

	return true;
}

void ExtraItemEquipConfirmWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_EQUIP_BTN:
		if( cmd == IOBN_BTNUP )
		{
			// 출전중인 용병만
			if( m_iCharArray == g_MyInfo.GetSelectCharArray() )
			{
				if( m_pPlayStage && !m_pPlayStage->IsModeStateResult() )
				{
					ioBaseChar *pOwner = m_pPlayStage->GetOwnerChar();
					// 장비 교체 가능상태 체크
					if( pOwner && !pOwner->SetExtraItemChangeState( false ) )
					{
						HideWnd();
						return;
					}
				}
			}

			TCPNetwork::MouseBusy( true );

			SP2Packet kPacket( CTPK_EXTRAITEM_CHANGE );
			kPacket << m_iCharArray;
			kPacket << m_iEquipSlot;
			kPacket << m_iExtraItemIndex;
			TCPNetwork::SendToServer( kPacket );

			g_MyInfo.SetExtraItemChange( 3, m_iCharArray, m_iEquipSlot, m_iExtraItemIndex );
			g_QuestMgr.QuestCompleteTerm( QC_EXTRA_ITEM_EQUIP, m_iItemCode, m_iPeriodType  );

			HideWnd();
		}
		break;
	case ID_CANCEL_BTN:
	case ID_EXIT_BTN:
		if( cmd == IOBN_BTNUP )
		{
			HideWnd();
		}
		break;
	}
}

void ExtraItemEquipConfirmWnd::OnRender()
{
	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	ioMovingWnd::OnRender();

	enum
	{
		NAME_X		= 103,
		NAME_Y		= 149,

		TEXT_X		= 103,
		TEXT_Y		= 179,
	};

	ioComplexStringPrinter kPrinter;
	kPrinter.SetTextStyle( TS_NORMAL );
	kPrinter.SetBkColor( 0, 0, 0 );
	kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter.AddTextPiece( FONT_SIZE_17, STR(1), m_szName.c_str() );
	kPrinter.PrintFullText( iXPos+NAME_X, iYPos+NAME_Y, TAT_CENTER );
	kPrinter.ClearList();

	kPrinter.SetTextStyle( TS_NORMAL );
	kPrinter.SetBkColor( 0, 0, 0 );
	kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter.AddTextPiece( FONT_SIZE_13, STR(2) );
	kPrinter.PrintFullText( iXPos+TEXT_X, iYPos+TEXT_Y, TAT_CENTER );
	kPrinter.ClearList();

	kPrinter.SetTextStyle( TS_NORMAL );
	kPrinter.SetBkColor( 0, 0, 0 );
	kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter.AddTextPiece( FONT_SIZE_13, STR(3) );
	kPrinter.PrintFullText( iXPos+TEXT_X, iYPos+TEXT_Y+20, TAT_CENTER );

	kPrinter.ClearList();

	kPrinter.SetTextStyle( TS_NORMAL );
	kPrinter.SetBkColor( 0, 0, 0 );
	kPrinter.SetTextColor( TCT_DEFAULT_RED );
	kPrinter.AddTextPiece( FONT_SIZE_13, STR(4) );
	kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter.AddTextPiece( FONT_SIZE_13, STR(5) );
	kPrinter.PrintFullText( iXPos+TEXT_X, iYPos+TEXT_Y+40, TAT_CENTER );

	kPrinter.ClearList();
}

void ExtraItemEquipConfirmWnd::OnRenderAfterChild()
{
	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	enum
	{
		MARK_X		= 103,
		MARK_Y		= 98,
	};

	if( m_pExclamationMark )
		m_pExclamationMark->Render( iXPos+MARK_X, iYPos+MARK_Y );
}

void ExtraItemEquipConfirmWnd::SetPlayStage( ioPlayStage *pPlayStage )
{
	m_pPlayStage = pPlayStage;
}

//////////////////////////////////////////////////////////////////////////

AlchemicItemWnd::AlchemicItemWnd()
{
	m_pEmptyImg = NULL;
	m_pIconBackImg = NULL;

	m_pEmptyFrm = NULL;
	m_pPieceFrm = NULL;
	m_pAdditiveFrm = NULL;
	m_pNewAddFrm = NULL;

	m_pIconImg = NULL;

	m_iCode = 0;
	m_iCount = 0;
	m_bLockState = false;
	m_bNewAdd = false;
	m_iNewAddAlpha = 0;
	m_dwNewAddEffectCheckTime = 0;

	m_iCurXGap = 0;
	m_iCurYGap = 0;

	m_pDragItem = new ioAlchemicDragItem( SDI_ALCHEMIC_ITEM );
}

AlchemicItemWnd::~AlchemicItemWnd()
{
	ClearDesc();

	SAFEDELETE(m_pEmptyImg);
	SAFEDELETE(m_pIconBackImg);

	SAFEDELETE(m_pEmptyFrm);
	SAFEDELETE(m_pPieceFrm);
	SAFEDELETE(m_pAdditiveFrm);
	SAFEDELETE(m_pNewAddFrm);

	SAFEDELETE(m_pIconImg);

	SAFEDELETE(m_pDragItem);
}

void AlchemicItemWnd::iwm_mouseleave( const ioMouse& mouse )
{
	if( IsLockState() )
	{
		ioButton::iwm_mouseleave( mouse );
		return;
	}

	bool bEnableDrag = false;
	MyInventoryWnd *pMyInvenWnd = dynamic_cast<MyInventoryWnd*>(GetParent());
	if( pMyInvenWnd && pMyInvenWnd->IsShow() )
	{
		bEnableDrag = pMyInvenWnd->CheckAlchemicEnableDrag();
	}

	if( m_bClicked && bEnableDrag && m_iCode > 0 )
	{
		POINT ptOffset = { 0, 0};
		if( m_pDragItem )
		{
			m_pDragItem->SetAlchemicCode( m_iCode );
			m_pDragItem->SetDragImageName( m_IconName );
			m_pDragItem->DoDragDrop( GetParent(), m_rcPos, ptOffset );
		}
	}

	ioButton::iwm_mouseleave( mouse );
}

void AlchemicItemWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "empty_image" )
	{
		SAFEDELETE( m_pEmptyImg );
		m_pEmptyImg = pImage;
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

void AlchemicItemWnd::AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame )
{
	if( pFrame )
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
		else if( szType == "new_add_frm" )
		{
			SAFEDELETE( m_pNewAddFrm );
			m_pNewAddFrm = pFrame;
		}
		else
		{	
			ioButton::AddRenderFrame( szType, pFrame );
		}
	}
}

void AlchemicItemWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	ioButton::ParseExtraInfo( xElement );

	m_dwNewAddEffectTime = xElement.GetIntAttribute_e( "new_add_effect_time" );
}

void AlchemicItemWnd::SetBlank()
{
	m_iCurXGap = 0;
	m_iCurYGap = 0;

	m_iCode = 0;
	m_iCount = 0;
	m_bLockState = false;
	m_bNewAdd = false;
	m_iNewAddAlpha = 0;
	m_dwNewAddEffectCheckTime = 0;

	SetInActive();

	ClearDesc();
}

void AlchemicItemWnd::ClearDesc()
{
	m_CountText.ClearList();
}

void AlchemicItemWnd::OnRender()
{
	enum 
	{
		CENTER_X	= 36,
		CENTER_Y	= 35,
	};

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	if( m_iCode <= 0 )
	{
		if( m_pEmptyFrm )
			m_pEmptyFrm->Render( iXPos, iYPos );

		if( m_pEmptyImg )
		{
			m_pEmptyImg->SetScale( 0.75f );
			m_pEmptyImg->Render( iXPos+CENTER_X, iYPos+CENTER_Y, UI_RENDER_MULTIPLY, TFO_BILINEAR );
		}
	}
	else
	{
		if( IsAdditive() )
		{
			if( m_pAdditiveFrm )
			{
				if( m_bLockState )
					m_pAdditiveFrm->Render( iXPos, iYPos, UI_RENDER_GRAY );
				else
					m_pAdditiveFrm->Render( iXPos, iYPos );
			}
		}
		else
		{
			if( m_pPieceFrm )
			{
				if( m_bLockState )
					m_pPieceFrm->Render( iXPos, iYPos, UI_RENDER_GRAY );
				else
					m_pPieceFrm->Render( iXPos, iYPos );
			}
		}

		if( m_bNewAdd && m_pNewAddFrm )
		{
			m_pNewAddFrm->SetAlpha( m_iNewAddAlpha );
			m_pNewAddFrm->Render( iXPos, iYPos );
		}

		if( m_pIconBackImg )
		{
			m_pIconBackImg->SetScale( 0.75f );
			m_pIconBackImg->Render( iXPos+CENTER_X, iYPos+CENTER_Y, UI_RENDER_MULTIPLY, TFO_BILINEAR );
		}

		if( m_pIconImg )
		{
			m_pIconImg->SetScale( 0.75f );

			if( m_bLockState )
				m_pIconImg->Render( iXPos+CENTER_X, iYPos+CENTER_Y, UI_RENDER_GRAY, TFO_BILINEAR );
			else
				m_pIconImg->Render( iXPos+CENTER_X, iYPos+CENTER_Y, UI_RENDER_NORMAL, TFO_BILINEAR );
		}

		if( IsOver() )
		{
			OnDrawOvered( iXPos, iYPos );
		}

		RenderDesc( iXPos, iYPos );
	}
}

void AlchemicItemWnd::RenderDesc( int iXPos, int iYPos )
{
	if( m_CountText.IsEmpty() )
		return;

	enum 
	{
		X_OFFSET	= 65,
		Y_OFFSET	= 48,
	};

	iXPos += X_OFFSET;
	iYPos += Y_OFFSET;

	m_CountText.PrintFullText( iXPos, iYPos, TAT_RIGHT );
}

bool AlchemicItemWnd::IsAdditive()
{
	if( m_iCode > ALCHEMIC_ADDITIVE_DIV )
		return true;

	return false;
}

void AlchemicItemWnd::SetLockState( bool bLock )
{
	m_bLockState = bLock;
}

void AlchemicItemWnd::SetInfo( int iCode, int iCount, ioHashString szIconName, bool bAddNew )
{
	SetBlank();
	
	m_iCode = iCode;
	m_iCount = iCount;
	m_IconName = szIconName;
	m_bLockState = false;

	if( bAddNew )
		SetNewAdd( true );

	if( SetAlchemicItemInfo() )
	{
		SetActive();
	}
	else
	{
		SetBlank();
		return;
	}
}

bool AlchemicItemWnd::SetAlchemicItemInfo()
{
	if( m_iCode <= 0 )
		return false;

	// icon
	SAFEDELETE( m_pIconImg );
	m_pIconImg = g_UIImageSetMgr.CreateImageByFullName( m_IconName );

	// count text
	char szCount[MAX_PATH]="";
	Help::ConvertNumToStrComma( m_iCount, szCount, sizeof( szCount ) );

	m_CountText.SetTextStyle( TS_OUTLINE_FULL_3X );
	m_CountText.SetBkColor( TCT_DEFAULT_DARKGRAY );
	m_CountText.SetTextColor( TCT_DEFAULT_WHITE );
	m_CountText.AddTextPiece( FONT_SIZE_12, szCount );

	return true;
}

void AlchemicItemWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() )
		return;

	if( m_iCode == 0 )
		return;

	if( IsOver() )
	{
		SetNewAdd( false );
	}
	else if( m_bNewAdd )
	{
		CheckNewAddEffect();
	}

	if( m_bOver && !m_bClicked )
	{
		ioLSMouse *pMouse = g_App.GetMouse();
		if( pMouse )
		{
			DWORD dwID = GetID();
			pMouse->SetToolTipWndID( dwID );

			return;
		}
	}	
}

void AlchemicItemWnd::SetNewAdd( bool bNew )
{
	m_bNewAdd = bNew;

	if( m_bNewAdd )
	{
		m_iNewAddAlpha = 0;
		m_dwNewAddEffectCheckTime = FRAMEGETTIME();
	}
	else
	{
		m_iNewAddAlpha = 0;
		m_dwNewAddEffectCheckTime = 0;
	}
}

void AlchemicItemWnd::CheckNewAddEffect()
{
	if( m_dwNewAddEffectTime == 0 )
		return;

	if( m_dwNewAddEffectCheckTime == 0 )
		return;

	DWORD dwCurTime = FRAMEGETTIME();
	DWORD dwGapTime = dwCurTime - m_dwNewAddEffectCheckTime;
	dwGapTime = min( dwGapTime, m_dwNewAddEffectTime );

	if( dwGapTime == m_dwNewAddEffectTime )
		m_dwNewAddEffectCheckTime = dwCurTime;

	float fGapRate = (float)dwGapTime / (float)m_dwNewAddEffectTime;
	float fValue = FLOAT05 - fabsf(FLOAT05 - fGapRate);
	
	m_iNewAddAlpha = MAX_ALPHA_RATE * (fValue / FLOAT05);
}

bool AlchemicItemWnd::QuestGuide( const IntVec &rValue, float &rfXPos, float &rfYPos )
{
	if( m_iCode <= 0 )
		return false;

	if( rValue.size() < 8 )
		return false;

	int iMainCode = rValue[3];
	if( m_iCode == iMainCode && IsShow() )
	{
		rfXPos = GetDerivedPosX() + (GetWidth() / 2);
		rfYPos = GetDerivedPosY();
		return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

AlchemicItemToolTip::AlchemicItemToolTip()
{
	Clear();
}

AlchemicItemToolTip::~AlchemicItemToolTip()
{
	Clear();
}

void AlchemicItemToolTip::OnProcess( float fTimePerSec )
{
	if( !g_GUIMgr.IsShow( MY_INVENTORY_WND ) )
	{
		HideWnd();
		return;
	}

	ioLSMouse *pMouse = g_App.GetMouse();
	if( !pMouse ) return;

	AlchemicToolTipInfo kInfo;
	MyInventoryWnd *pInven = dynamic_cast<MyInventoryWnd*> ( g_GUIMgr.FindWnd( MY_INVENTORY_WND ) );

	if( pInven )
	{
		pInven->GetCurAlchemicItemInfo( kInfo, pMouse->GetToolTipWndID() );
	}

	if( IsShow() )
	{
		if( !SetInfo( kInfo.m_iCode, kInfo.m_iType ) )
		{
			HideWnd();
		}
		else
		{
			pMouse->SetToolTipWndID( 0 );
			pMouse->SetToolTipCursor();

			int iXPos = 0;
			int iYPos = 0;
			Help::GetNotCutWndPos( pMouse->GetMousePos().x, pMouse->GetMousePos().y, GetWidth(), GetHeight(), iXPos, iYPos );
			SetWndPos( iXPos, iYPos );
			ProcessPosition();
		}
	}
	else
	{
		if( SetInfo( kInfo.m_iCode, kInfo.m_iType ) ) // setinfo 후에 showwnd
		{
			ShowWnd();

			int iXPos = 0;
			int iYPos = 0;
			Help::GetNotCutWndPos( pMouse->GetMousePos().x, pMouse->GetMousePos().y, GetWidth(), GetHeight(), iXPos, iYPos );
			SetWndPos( iXPos, iYPos );
			ProcessPosition();
		}
	}
}

bool AlchemicItemToolTip::SetInfo( int iCode, int iType )
{
	Clear();

	m_iCode = iCode;
	m_iType = iType;

	if( m_iCode == 0 )
	{
		Clear();
		return false;
	}

	// desc
	m_szDescArray.SetTextStyle( TS_NORMAL );
	m_szDescArray.SetBkColor( 0, 0, 0 );	
	m_szDescArray.SetTextColor( TCT_DEFAULT_DARKGRAY ); // 진회색

	switch( m_iType )
	{
	case ATT_ALCHEMIC_ITEM:
		{
			if( m_iCode < ALCHEMIC_ADDITIVE_DIV )
				m_szDescArray.AddTextPiece( FONT_SIZE_12, STR(4), g_MyInfo.GetClassName(m_iCode) );
			else
				m_szDescArray.AddTextPiece( FONT_SIZE_12, STR(5), g_AlchemicMgr.GetAlchemicItemName(m_iCode).c_str() );
		}
		break;
	case ATT_SOLDIER:
		{
			m_szDescArray.AddTextPiece( FONT_SIZE_12, STR(6), g_MyInfo.GetClassName(m_iCode) );
		}
		break;
	case ATT_ITEM:
		{
			const ioItem *pItem = g_ItemMaker.GetItemConst( m_iCode, __FUNCTION__ );
			if( pItem )
			{
				m_szDescArray.AddTextPiece( FONT_SIZE_12, STR(7), pItem->GetName().c_str() );
			}
		}
		break;
	}

	//
	int iMaxWidth  = m_szDescArray.GetFullWidth() + SIDE_TEXT_GAP;
	int iMaxHeight = MIN_HEIGHT;

	SetSize( iMaxWidth, iMaxHeight );

	return true;
}

void AlchemicItemToolTip::iwm_show()
{
}

void AlchemicItemToolTip::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX() + 7;
	int iYPos = GetDerivedPosY() + 6;

	if( !m_szDescArray.IsEmpty() )
	{
		m_szDescArray.PrintFullTextWidthCut( iXPos, iYPos, TAT_LEFT, 126.0f );
	}
}

void AlchemicItemToolTip::Clear()
{
	m_iCode = 0;
	m_iType = 0;

	m_szDescArray.ClearList();
}

void AlchemicItemToolTip::ProcessPosition()
{
	ioLSMouse *pMouse = g_App.GetMouse();
	if( pMouse )
	{
		int iOffSetXY = 8;
		int iXPos = pMouse->GetMousePos().x;
		int iYPos = pMouse->GetMousePos().y;

		if( iXPos + iOffSetXY + GetWidth() > Setting::Width() )
			iXPos -= ( GetWidth() + iOffSetXY );
		else 
			iXPos += iOffSetXY;

		if( iYPos + iOffSetXY + GetHeight() > Setting::Height() - 45 )
			iYPos -= ( GetHeight() + iOffSetXY );
		else 
			iYPos += iOffSetXY;

		if( GetParent() )
		{
			iXPos -= GetParent()->GetDerivedPosX();
			iYPos -= GetParent()->GetDerivedPosY();
		}

		SetWndPos( iXPos, iYPos );
		pMouse->SetToolTipCursor();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MyInventoryWnd::ShowItemMaterialCompoundWnd( DWORD dwCode )
{
	MyInventoryInfoWnd *pInfoWnd = (MyInventoryInfoWnd*)(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	
	ItemMaterialCompoundWnd *pWnd = (ItemMaterialCompoundWnd*)(FindChildWnd(ID_ITEM_MATERIAL_COMPOUND_WND));
	if( pWnd )
		pWnd->ShowWndWithEtcItemCode( dwCode );
}

void MyInventoryWnd::SetSelectMaterialCode( int dwCode )
{
	ItemMaterialCompoundWnd *pWnd = (ItemMaterialCompoundWnd*)(FindChildWnd(ID_ITEM_MATERIAL_COMPOUND_WND));
	if( pWnd && pWnd->IsShow() )
		pWnd->SetMaterialCode( dwCode );
}

void MyInventoryWnd::OnItemMaterialCompoundResult( bool bSuccess, int iTargetSlot, int iReinforce, DWORD dwCode )
{
	MyInventoryInfoWnd *pInfoWnd = (MyInventoryInfoWnd*)(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	HideChildWnd(ID_ITEM_MATERIAL_COMPOUND_WND);

	ShowChildWnd( ID_FLASH_WND );
	ItemMaterialCompoundResultWnd *pWnd = (ItemMaterialCompoundResultWnd*)(FindChildWnd(ID_ITEM_MATERIAL_COMPOUND_RESULT));
	if( pWnd )
		pWnd->SetCompoundResultInfo( bSuccess, iTargetSlot, iReinforce, dwCode );
}

void MyInventoryWnd::OnItemMaterialCompoundResultFailExp( int nPreFailExp, int nCurFailExp, int nItemCode )
{
	ItemMaterialCompoundResultWnd *pWnd = (ItemMaterialCompoundResultWnd*)(FindChildWnd(ID_ITEM_MATERIAL_COMPOUND_RESULT));
	if( pWnd && pWnd->IsShow() )
		pWnd->SetFailExp( nPreFailExp, nCurFailExp, nItemCode );
}

void MyInventoryWnd::ShowPetEggResultWnd( DWORD dwEtcItemType )
{
	MyInventoryInfoWnd *pInfoWnd = (MyInventoryInfoWnd*)(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	PetEggResultWnd *pWnd = (PetEggResultWnd*)(FindChildWnd(ID_PET_EGG_RESULT_WND));
	if( pWnd )
		pWnd->ShowRandomBox( dwEtcItemType );
}

void MyInventoryWnd::OnPetEggResultInfo( int nPetCode, int nPetRank, int nPetSlotIndex )
{
	MyInventoryInfoWnd *pInfoWnd = (MyInventoryInfoWnd*)(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	PetEggResultWnd *pWnd = (PetEggResultWnd*)(FindChildWnd(ID_PET_EGG_RESULT_WND));
	if( pWnd )
		pWnd->OnResultInfo( nPetCode, nPetRank, nPetSlotIndex );
}

void MyInventoryWnd::ShowHeroReinforceDownWnd( int iClassType )
{
	MyInventoryInfoWnd *pInfoWnd = (MyInventoryInfoWnd*)(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );

	HeroReinforceDownWnd *pWnd = (HeroReinforceDownWnd*)(FindChildWnd(ID_HERO_REINFORCE_DWON_WND));
	if( pWnd )
	{
		pWnd->SetSoldierInfo( iClassType );
		pWnd->ShowWnd();
	}
	
}

void MyInventoryWnd::ShowHeroReinforceDoneWnd( int iClassType )
{
	MyInventoryInfoWnd *pInfoWnd = (MyInventoryInfoWnd*)(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );

	HeroReinforceDoneWnd *pWnd = (HeroReinforceDoneWnd*)(FindChildWnd(ID_HERO_REINFORCE_DONE_WND));
	if( pWnd )
	{
		pWnd->SetSoldierInfo( iClassType );
		pWnd->ShowWnd();
	}
}

ioWnd* MyInventoryWnd::ShowItemReinforceDownWnd( int iIndex )
{
	MyInventoryInfoWnd *pInfoWnd = (MyInventoryInfoWnd*)(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );

	ItemReinforceDownWnd *pWnd = (ItemReinforceDownWnd*)(FindChildWnd(ID_ITEM_REINFORCE_DWON_WND));
	if( pWnd )
	{
		pWnd->SetItemInfo( iIndex );
		pWnd->ShowWnd();
		return pWnd;
	}

	return NULL;
}

void MyInventoryWnd::ShowItemReinforceDoneWnd( int iIndex )
{
	MyInventoryInfoWnd *pInfoWnd = (MyInventoryInfoWnd*)(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );

	ItemReinforceDoneWnd *pWnd = (ItemReinforceDoneWnd*)(FindChildWnd(ID_ITEM_REINFORCE_DONE_WND));
	if( pWnd )
	{
		pWnd->SetItemInfo( iIndex );
		pWnd->ShowWnd();
	}
}

// UI코스튬 관련 (코스튬 박스 윈도우)
void MyInventoryWnd::ShowCostumeBoxWnd( int nMachineCode )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );

	ioCostumeRandomBox *pWnd = dynamic_cast<ioCostumeRandomBox*>(FindChildWnd(ID_COSTUME_BOX_WND));
	if( pWnd )
		pWnd->ShowRandomBox( nMachineCode );
}

// UI코스튬 관련 (코스튬 박스 윈도우)
void MyInventoryWnd::OnCostumeBoxResultInfo( int nCostumeCode, int nPeriodTime )
{
	ioCostumeRandomBox *pWnd = dynamic_cast<ioCostumeRandomBox*>(FindChildWnd(ID_COSTUME_BOX_WND));
	if( pWnd && pWnd->IsShow() )
		pWnd->OnResultInfo( nCostumeCode, nPeriodTime );
}

void MyInventoryWnd::ShowNewMultipleCompoundWnd( DWORD dwType )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	ItemNewMultipleCompoundWnd *pWnd = dynamic_cast<ItemNewMultipleCompoundWnd*>(FindChildWnd(ID_NEW_MULTIPLE_COMPOUND_WND));
	if( pWnd )
		pWnd->SetInfoAndShow( dwType );
}

void MyInventoryWnd::ShowGashponWndByNewMultipleCompound( DWORD dwPossessEtcItemType, DWORD dwEtcItemType )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_NEW_MULTIPLE_COMPOUND_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	GashaponWnd *pWnd = dynamic_cast<GashaponWnd*>(FindChildWnd(ID_GASHPON_WND));
	if( pWnd )
		pWnd->ShowRandomBox( dwPossessEtcItemType, dwEtcItemType );
}

void MyInventoryWnd::ShowNamedTitleResultWnd( DWORD dwCode, int nLevel, BYTE byPremium )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	NamedTitleResultWnd *pWnd = dynamic_cast<NamedTitleResultWnd*>(FindChildWnd(ID_NAMED_TITLE_RESULT_WND));
	if( pWnd )
		pWnd->ShowResult( dwCode, nLevel, byPremium );
}

void MyInventoryWnd::ShowNamedTitlePremiumResultWnd( DWORD dwCode, int nLevel, BYTE byPremium )
{
	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	NamedTitlePremiumResultWnd *pWnd = dynamic_cast<NamedTitlePremiumResultWnd*>(FindChildWnd(ID_NAMED_TITLE_PREMIUM_RESULT_WND));
	if( pWnd )
		pWnd->ShowResult( dwCode, nLevel, byPremium );
}

void MyInventoryWnd::ShowNamedTitlePremiumSelectWnd( DWORD dwEtcItemCode )
{
	ioUserNamedTitle *pUserNamedTitle = g_MyInfo.GetUserNamedTitle();
	if ( !pUserNamedTitle )
		return;

	if ( !pUserNamedTitle->IsEnablePremiumNamedTitle() )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return;
	}

	MyInventoryInfoWnd *pInfoWnd = dynamic_cast<MyInventoryInfoWnd*>(FindChildWnd(ID_INVENTORY_INFO_WND));
	if( pInfoWnd && pInfoWnd->IsShow() )
		pInfoWnd->HideWnd();

	ShowChildWnd( ID_FLASH_WND );
	NamedTitlePremiumSelectWnd *pWnd = dynamic_cast<NamedTitlePremiumSelectWnd*>(FindChildWnd(ID_NAMED_TITLE_PREMIUM_SELECT_WND));
	if( pWnd )
		pWnd->ShowSelectWnd( dwEtcItemCode );
}

void MyInventoryWnd::CheckBonusCash()
{
	ioButton* pBonusCashBtn = dynamic_cast<ioButton*>(FindChildWnd( ID_BONUS_CASH_BTN ));
	if ( !pBonusCashBtn )
		return;

	int iBonusCash = g_MyInfo.GetBonusCash();
	if ( iBonusCash <=0 )
	{
		pBonusCashBtn->HideWnd();
		return;
	}
	else if( !pBonusCashBtn->IsShow() )
	{
		pBonusCashBtn->ShowWnd();
	}

	if ( pBonusCashBtn->IsShow() && pBonusCashBtn->IsOver() )
	{
		ioLSMouse *pMouse = g_App.GetMouse();
		if ( pMouse )
			pMouse->SetToolTipCursor();
	}

	if ( m_dwBonusCashTooltipCheckDuration == 0 )
		return;
	if ( m_dwBonusCashTooltipCheckStartTime + m_dwBonusCashTooltipCheckDuration > FRAMEGETTIME() )
		return;

	m_dwBonusCashTooltipCheckStartTime = FRAMEGETTIME();

	if ( g_BonusCashMgr.CheckAlarmExpireBonusCash() )
	{
		ioWnd* pBonusTooltip = FindChildWnd( ID_BONUS_CASH_TOOLTIP );
		if ( pBonusTooltip && !pBonusTooltip->IsShow() )
			pBonusTooltip->ShowWnd();
	}
	else
	{
		ioWnd* pBonusTooltip = FindChildWnd( ID_BONUS_CASH_TOOLTIP );
		if ( pBonusTooltip && pBonusTooltip->IsShow() )
			pBonusTooltip->HideWnd();
	}
}

int MyInventoryWnd::UpdateSmallTabName( ioWnd* pWnd, int iXOffset, const ioHashString& szTitle )
{
	if( !pWnd )
		return 0;

	ioUITitle* pTitle = pWnd->GetTitle();
	if( !pTitle )
		return 0;

	if( szTitle.IsEmpty() )
		return 0;

	float fScale	= (float)pTitle->GetSize() / (float)g_FontMgr.GetFontSize();
	int iTextWidth	= g_FontMgr.GetTextWidth( szTitle.c_str(), TS_NORMAL, fScale ) + 11;
	pTitle->SetOffset( iTextWidth/2, 0 );

	pWnd->SetSize( iTextWidth, g_FontMgr.GetFontSize() );
	pWnd->SetWndPos( iXOffset, pWnd->GetYPos() );
	pWnd->SetTitleText( szTitle.c_str() );
	pWnd->ShowWnd();

	return pWnd->GetWidth();
}

void MyInventoryWnd::SetAccessoryInfoVec( DWORD dwTabID, DWORD dwSmallTabID )
{
	m_vInventoryItemInfoList.clear();
	m_vAlchemicItemInfoList.clear();

	ioUserAccessory *pAccessory = g_MyInfo.GetUserAccessory();
	if( !pAccessory ) 
		return;

	int nSize = pAccessory->GetAccessorySlotCnt();
	for( int i=0; i<nSize; ++i )
	{
		AccessorySlot kSlot;
		if( !pAccessory->GetAccessorySlotArray( i, kSlot ) )
			continue;

		// 스몰탭 인덱스랑 같아야함
		int nGroup = (kSlot.m_iAccessoryCode/DEFAULT_BASIC_ACCESSORY_CODE);
		if( !IsRightSmallTab( dwTabID, dwSmallTabID, nGroup ) )
			continue;

		if( kSlot.m_bEquip )
			continue;

		InventoryItemInfo kInfo;
		kInfo.m_iMainType = INVENTORY_ITEM_ACCESSORY;
		kInfo.m_iMagicCode = kSlot.m_iIndex;
		kInfo.m_bEquip = kSlot.m_bEquip;

		int nSlot = kSlot.m_iAccessoryCode/DEFAULT_BASIC_ACCESSORY_CODE;
		int nValue = kSlot.m_iValue1;

		if( !kSlot.m_bEquip )
			nValue += 100;

		kInfo.m_iPriorityOrder = nSlot * 10000 + nValue;
		kInfo.m_iPriorityOrder2 = kSlot.m_iAccessoryCode;

		m_vInventoryItemInfoList.push_back( kInfo );
	}

	//AddItemInfoVecFromEtcItem( dwTabID, dwSmallTabID, SHOP_TAB_TYPE_NONE );

	if( m_vInventoryItemInfoList.empty() )
		return;

	// 각 하위텝 마다 다르게 정렬한다.
	if( dwSmallTabID == ID_SMALL_TAB_1_BTN )	// 전체
	{
		// m_iMagicCode 역순으로 정렬
		std::sort( m_vInventoryItemInfoList.begin(), m_vInventoryItemInfoList.end(), InventoryItemInfoSort2() );
	}
	else
		std::sort( m_vInventoryItemInfoList.begin(), m_vInventoryItemInfoList.end(), InventoryItemInfoSort3() );
}

bool MyInventoryWnd::SetReSellAccessory( DWORD dwID )
{
	InventoryItemWnd *pInvenItem = dynamic_cast<InventoryItemWnd*>(FindChildWnd(dwID));
	if( !pInvenItem ) 
		return false;

	int iNewSlotIndex = pInvenItem->GetMagicCode();
	if( iNewSlotIndex <= 0 )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		LOG.PrintTimeAndLog( 0, "MyInventoryWnd::SetReSellAccessory Wrong Btn : %d", dwID );
		return false;
	}

	ioUserAccessory *pAccessory = g_MyInfo.GetUserAccessory();
	if( !pAccessory )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		LOG.PrintTimeAndLog( 0, "MyInventoryWnd::SetReSellAccessory Don't Have UserExtraItem" );
		return false;
	}

	AccessorySlot kSlot;
	if( !pAccessory->GetAccessorySlot( iNewSlotIndex, kSlot ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		LOG.PrintTimeAndLog( 0, "MyInventoryWnd::SetReSellAccessory Not Exist Item : %d", iNewSlotIndex );
		return false;
	}

	if( kSlot.m_bEquip )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		LOG.PrintTimeAndLog( 0, "MyInventoryWnd::SetReSellAccessory Item : %d", iNewSlotIndex );
		return false;
	}

	if( !CheckTimeEndAccessory( iNewSlotIndex ) )
		return false;

	m_iSellSlotIndex = iNewSlotIndex;

	float fReturnPeso = 0.0f;
	DWORD dwEditType = SELL_EDIT_NONE;
	if( kSlot.m_PeriodType == ACCPT_MORTMAIN )
	{
		fReturnPeso = g_AccessoryInfoMgr.GetResellMortmainAccessoryPeso();
		dwEditType = SELL_EDIT_EXTRAITEM;
	}
	else
		fReturnPeso = g_AccessoryInfoMgr.GetResellTimeAccessoryTimePeso();

	__int64 iPeso = (__int64)fReturnPeso;

	char szConvertNum[MAX_PATH] = "";
	Help::ConvertNumToStrComma( iPeso, szConvertNum, sizeof( szConvertNum ) );

	ioHashString sInfo;
	pAccessory->GetUseInfo( iNewSlotIndex, sInfo );

	if( sInfo.IsEmpty() )
	{
		LOG.PrintTimeAndLog( 0, "%s don't have , Use Info:%d", __FUNCTION__, iNewSlotIndex );
	}

	ioComplexStringPrinter kPrinter[ItemResellWnd::MAX_INFO_LINE];

	// ItemInfo
	kPrinter[0].SetTextStyle( TS_NORMAL );
	kPrinter[0].SetBkColor( 0, 0, 0 );
	kPrinter[0].SetTextColor( TCT_DEFAULT_RED );
	kPrinter[0].AddTextPiece( FONT_SIZE_17, STR(5), kSlot.m_Name.c_str() );

	kPrinter[1].SetTextStyle( TS_NORMAL );
	kPrinter[1].SetBkColor( 0, 0, 0 );
	kPrinter[1].SetTextColor( TCT_DEFAULT_RED );
	kPrinter[1].AddTextPiece( FONT_SIZE_17, STR(6), sInfo.c_str() );

	// Desc
	kPrinter[2].SetTextStyle( TS_NORMAL );
	kPrinter[2].SetBkColor( 0, 0, 0 );
	kPrinter[2].SetTextColor( TCT_DEFAULT_GRAY );
	kPrinter[2].AddTextPiece( FONT_SIZE_13, STR(7) );

	kPrinter[3].SetTextStyle( TS_NORMAL );
	kPrinter[3].SetBkColor( 0, 0, 0 );
	kPrinter[3].SetTextColor( TCT_DEFAULT_GRAY );
	kPrinter[3].AddTextPiece( FONT_SIZE_13, STR(8) );
	kPrinter[3].SetTextColor( TCT_DEFAULT_BLUE );
	kPrinter[3].AddTextPiece( FONT_SIZE_13, STR(9), szConvertNum );

	kPrinter[4].SetTextStyle( TS_NORMAL );
	kPrinter[4].SetBkColor( 0, 0, 0 );
	kPrinter[4].SetTextColor( TCT_DEFAULT_GRAY );
	kPrinter[4].AddTextPiece( FONT_SIZE_13, STR(10) );

	ItemResellWnd *pResellWnd = dynamic_cast<ItemResellWnd*>(g_GUIMgr.FindWnd(ITEM_RESELL_WND));
	if( pResellWnd )
	{
		ioUIRenderImage *pImg = g_UIImageSetMgr.CreateImageByFullName( kSlot.m_IconName );
		pResellWnd->SetInfoAndShow( STR(11), pImg, 0, kPrinter, this, dwEditType, ACST_SELL_ACCESSORY, false );
	}

	return true;
}

bool MyInventoryWnd::CheckTimeEndAccessory( int iSlotIndex )
{
	ioUserAccessory *pAccessory = g_MyInfo.GetUserAccessory();
	if( pAccessory )
	{
		AccessorySlot kSlot;
		if( pAccessory->GetAccessorySlot( iSlotIndex, kSlot ) )
		{
			if( kSlot.m_PeriodType != ACCPT_MORTMAIN )
			{
				time_t kLimitTime = DateHelp::ConvertSecondTime( kSlot.GetYear(), kSlot.GetMonth(), kSlot.GetDay(), kSlot.GetHour(), kSlot.GetMinute(), 0 );
				time_t kCurServerTime = g_MyInfo.GetServerDate();
				kCurServerTime += ( REALGETTIME()/1000 );// 현재까지의 클라이언트초를 넣는다.

				int iGapSec = kLimitTime - kCurServerTime;
				enum { DAY_SEC = 86400, HOUR_SEC = 3600, MINUTE_SEC = 60, };
				int iDay    = iGapSec/DAY_SEC;
				int iHourSec= iGapSec-(iDay*DAY_SEC);
				int iHour   = iHourSec / HOUR_SEC;
				int iMinute = ( iHourSec-(iHour*HOUR_SEC) ) / MINUTE_SEC;

				if( iDay <= 0 && iHour <= 0 && iMinute <= 1 )
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
					return false;
				}
			}
		}
	}
	else
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return false;
	}

	return true;
}