#include "StdAfx.h"
#include "ItemReinforceDownWnd.h"

#include "PowerUpSelectWnd.h"
#include "../ioPowerUpManager.h"
#include "ReinforceSelectWnd.h"

ItemReinforceDownWnd::ItemReinforceDownWnd(void)
{
	m_pMagicCircle = NULL;
	m_pNameLine = NULL;
	m_pIconBack = NULL;
	m_pIcon = NULL;
	m_pLightImage = NULL;
	m_pNumText = NULL;
	m_pPlusText = NULL;

	ClearListAll();

	m_fScaleRate = 0.0f;
	m_fAlphaRate = 0.0f;
	m_fSlopeRate = 0.0f;
	m_dwResultEndTime = 0;
}


ItemReinforceDownWnd::~ItemReinforceDownWnd(void)
{
	SAFEDELETE(m_pMagicCircle);
	SAFEDELETE(m_pNameLine);
	SAFEDELETE(m_pIconBack);
	SAFEDELETE(m_pIcon);
	SAFEDELETE(m_pLightImage);
	SAFEDELETE( m_pNumText );
	SAFEDELETE( m_pPlusText );
	
	ClearListAll();
}

void ItemReinforceDownWnd::ClearListAll()
{
	for( int i=0; i < ItemReinforceDownWnd::ITEM_INFO_CNT; ++i )
		m_ItemInfo[i].ClearList();

	for( int i=0; i < ITEM_REINFORCE_INFO_CNT; ++i )
		m_ItemReinforceInfo[i].ClearList();

	m_SoulStonePrinter.ClearList();

	m_ItemSelectPrinter.ClearList();

	m_dwMagicEffectCreateTime = 0;
	m_vMagicEffectInfoList.clear();
	
	m_bShowResultEffect = false;
	m_dwResultStartTime = 0;
	m_dwResultEndTime = 0;
	m_dwResultEffectEndTime = 0;
	
	m_iAlpha1 = 0;
	m_iAlpha2 = 0;
	m_fCurEffectScale = 1.3f;
	m_iItemIndex = -1;
	m_iItemReinforce = 0;

	m_iIconRenderType = UI_RENDER_NORMAL;
	if ( m_pIcon )
		m_pIcon->SetColor( 0, 0, 0 );

	m_nGradeType = 0;
}

void ItemReinforceDownWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "NameLine" )
	{
		SAFEDELETE( m_pNameLine );
		m_pNameLine = pImage;
	}
	else if( szType == "magic_circle" )
	{
		SAFEDELETE( m_pMagicCircle );
		m_pMagicCircle = pImage;
	}
	else if( szType == "Icon_back" )
	{
		SAFEDELETE( m_pIconBack );
		m_pIconBack = pImage;
	}
	else if( szType == "light_image" )
	{
		SAFEDELETE( m_pLightImage );
		m_pLightImage = pImage;
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
	else
		ioWnd::AddRenderImage( szType, pImage );
}

void ItemReinforceDownWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	ioWnd::ParseExtraInfo( xElement );
	char szKeyName[MAX_PATH]="";
	// result effect
	m_fScaleRate = xElement.GetFloatAttribute_e( "scale_rate" );
	m_fAlphaRate = xElement.GetFloatAttribute_e( "alpha_rate" );
	m_fSlopeRate = xElement.GetFloatAttribute_e( "slope_rate" );

	m_dwResultEffectTime = xElement.GetIntAttribute_e( "result_effect_Time" );
	ZeroMemory( szKeyName, sizeof( szKeyName ) );
	StringCbPrintf( szKeyName, sizeof( szKeyName ), "rolling_sound" );
	m_szRollingSound = xElement.GetStringAttribute( szKeyName );
}

void ItemReinforceDownWnd::iwm_hide()
{
	if( !m_szRollingSound.IsEmpty() )
		g_SoundMgr.StopSound( m_szRollingSound, 0 );
}

void ItemReinforceDownWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();	
	switch( dwID )
	{
	case ID_ENABLE_REINFORECE_ITEM_SELECT_BTN:
		if( cmd == IOBN_BTNUP )
		{
			PowerUpSelectWnd *pWnd = dynamic_cast<PowerUpSelectWnd*>( g_GUIMgr.FindWnd( POWERUP_SELECT_WND ) );
			if( pWnd )
			{
				if( pWnd->IsShow() )
				{
					pWnd->GoToTop();
				}
				else
				{
					pWnd->ShowSubWnd( ioPowerUpManager::PIT_ITEM );
				}
			}
		}
		break;
	case ID_REINFORCE_BTN:
		{
			if( cmd == IOBN_BTNUP )
			{
				// solt check
				EXTRAITEMSLOT kSlot;
				ioUserExtraItem *pExtraItem = g_MyInfo.GetUserExtraItem();
				if( !pExtraItem )
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
					return;
				}

				EXTRAITEMSLOT ExtraItem;
				ExtraItem.Init();
				if ( !pExtraItem->GetExtraItem( m_iItemIndex, ExtraItem ) )
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
					return;
				}

				int iUseMtrlCount,iCurMtrlCount;
				if( !g_PowerUpManager.CheckEnableSoulStone( ioPowerUpManager::PIT_ITEM ,ExtraItem.m_iItemCode, iUseMtrlCount, iCurMtrlCount ) )
				{
					SoulStoneBuyWnd *pSoulstoneBuyWnd = dynamic_cast<SoulStoneBuyWnd*> ( g_GUIMgr.FindWnd( SOULSTONE_BUY_WND ) );
					if ( pSoulstoneBuyWnd )
						pSoulstoneBuyWnd->ShowBuyWnd( iUseMtrlCount, iCurMtrlCount );
					break;
				}

				ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
				char szBuf[MAX_PATH];
				memset( szBuf, 0, sizeof(szBuf) );

				if ( ExtraItem.m_dwMaleCustom || ExtraItem.m_dwFemaleCustom )
				{
					kPrinter[0].SetTextStyle( TS_NORMAL );
					kPrinter[0].SetBkColor( 0, 0, 0 );
					kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
					kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(1) );

					kPrinter[1].SetTextStyle( TS_NORMAL );
					kPrinter[1].SetBkColor( 0, 0, 0 );
					kPrinter[1].SetTextColor( TCT_DEFAULT_RED );
					kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(2) );

					kPrinter[2].SetTextStyle( TS_NORMAL );
					kPrinter[2].SetBkColor( 0, 0, 0 );
					kPrinter[2].SetTextColor( TCT_DEFAULT_GRAY );
					kPrinter[2].AddTextPiece( FONT_SIZE_13, STR(3) );
				}
				else if( ExtraItem.m_iTradeState != EET_DISABLE )
				{
					kPrinter[0].SetTextStyle( TS_NORMAL );
					kPrinter[0].SetBkColor( 0, 0, 0 );
					kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
					SafeSprintf( szBuf, sizeof( szBuf ), STR(4), iUseMtrlCount );
					kPrinter[0].AddTextPiece( FONT_SIZE_13, szBuf );

					kPrinter[1].SetTextStyle( TS_NORMAL );
					kPrinter[1].SetBkColor( 0, 0, 0 );
					kPrinter[1].SetTextColor( TCT_DEFAULT_RED );
					kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(6) );
				}
				else
				{
					kPrinter[0].SetTextStyle( TS_NORMAL );
					kPrinter[0].SetBkColor( 0, 0, 0 );
					kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
					SafeSprintf( szBuf, sizeof( szBuf ), STR(4), iUseMtrlCount );
					kPrinter[0].AddTextPiece( FONT_SIZE_13, szBuf );

					kPrinter[1].SetTextStyle( TS_NORMAL );
					kPrinter[1].SetBkColor( 0, 0, 0 );
					kPrinter[1].SetTextColor( TCT_DEFAULT_GRAY );
					kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(5) );
				}
				g_GUIMgr.SetPrevMsgListBox( NULL, MB_YESNO, this, kPrinter );
			}
		}
		break;
	case MESSAGE_BOX3:
		{
			if( cmd == IOBN_BTNUP )
			{
				if( param == IDYES )
				{
					if ( m_iItemIndex == -1 )
						break;

					// solt check
					EXTRAITEMSLOT kSlot;
					ioUserExtraItem *pExtraItem = g_MyInfo.GetUserExtraItem();
					if( !pExtraItem )
					{
						g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
						return;
					}

					EXTRAITEMSLOT ExtraItem;
					ExtraItem.Init();
					if ( !pExtraItem->GetExtraItem( m_iItemIndex, ExtraItem ) )
					{
						g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
						return;

					}

					ioHashString szError;
					if( !g_PowerUpManager.CheckEnableItemPowerUp( m_iItemIndex, szError ) )
					{
						g_GUIMgr.SetMsgBox( MB_OK, this, szError.c_str() );
						break;
					}

					int iUseMtrlCount,iCurMtrlCount;
					if( !g_PowerUpManager.CheckEnableSoulStone( ioPowerUpManager::PIT_ITEM, ExtraItem.m_iItemCode, iUseMtrlCount, iCurMtrlCount ) )
					{
						SoulStoneBuyWnd *pSoulstoneBuyWnd = dynamic_cast<SoulStoneBuyWnd*> ( g_GUIMgr.FindWnd( SOULSTONE_BUY_WND ) );
						if ( pSoulstoneBuyWnd )
							pSoulstoneBuyWnd->ShowBuyWnd( iUseMtrlCount, iCurMtrlCount );
						break;
					}
						
					ioEtcItemSoulStone* pEtcItem = dynamic_cast<ioEtcItemSoulStone*>( g_EtcItemMgr.FindEtcItem( g_PowerUpManager.GetMtrlEtcItemCode() ) );
					if( pEtcItem )
						pEtcItem->OnUseItemPowerUp( m_iItemIndex );
					m_dwResultStartTime = FRAMEGETTIME();
					m_dwResultEndTime = m_dwResultStartTime + m_dwResultEffectTime;
					m_dwResultEffectEndTime = m_dwResultEndTime;
					m_bShowResultEffect = true;
					m_iIconRenderType = UI_RENDER_NORMAL_ADD_COLOR;
					if( !m_szRollingSound.IsEmpty() )
						g_SoundMgr.PlaySound( m_szRollingSound, DSBVOLUME_MAX, PSM_LOOP | PSM_THREAD );

				}
			}
		}
		break;
	}
}

void ItemReinforceDownWnd::SetItemInfo( int iIndex )
{
	ClearListAll();
	//soulstone check
	int iSoulStoneCount = 0;
	ioUserEtcItem *pUserEtcItem = g_MyInfo.GetUserEtcItem();
	if( pUserEtcItem )
	{
		ioUserEtcItem::ETCITEMSLOT kSlot;
		if( pUserEtcItem->GetEtcItem( ioEtcItem::EIT_ETC_SOUL_STONE, kSlot ) )
			iSoulStoneCount = kSlot.m_iValue1;
	}

	// solt check
	EXTRAITEMSLOT kSlot;
	ioUserExtraItem *pExtraItem = g_MyInfo.GetUserExtraItem();
	if( !pExtraItem )
	{
		LOG.PrintConsoleLog( 0, "ItemReinforceDownWnd::SetItemInfo - ioUserExtraItem is not exist");
		return;
	}

	if ( m_pIcon != NULL )
		SAFEDELETE( m_pIcon );

	EXTRAITEMSLOT ExtraItem;
	ExtraItem.Init();
	if ( !pExtraItem->GetExtraItem( iIndex, ExtraItem ) )
	{
		m_ItemReinforceInfo[0].SetTextStyle( TS_NORMAL );
		m_ItemReinforceInfo[0].SetBkColor( 0, 0, 0 );
		m_ItemReinforceInfo[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
		m_ItemReinforceInfo[0].AddTextPiece( FONT_SIZE_12, STR(5) );

		m_ItemReinforceInfo[1].SetTextStyle( TS_NORMAL );
		m_ItemReinforceInfo[1].SetBkColor( 0, 0, 0 );
		m_ItemReinforceInfo[1].SetTextColor( TCT_DEFAULT_GRAY );
		m_ItemReinforceInfo[1].AddTextPiece( FONT_SIZE_12, STR(6) );

		m_ItemReinforceInfo[2].SetTextStyle( TS_NORMAL );
		m_ItemReinforceInfo[2].SetBkColor( 0, 0, 0 );
		m_ItemReinforceInfo[2].SetTextColor( TCT_DEFAULT_GRAY );
		m_ItemReinforceInfo[2].AddTextPiece( FONT_SIZE_12, STR(7) );

		m_ItemReinforceInfo[3].SetTextStyle( TS_NORMAL );
		m_ItemReinforceInfo[3].SetBkColor( 0, 0, 0 );
		m_ItemReinforceInfo[3].SetTextColor( TCT_DEFAULT_GRAY );
		m_ItemReinforceInfo[3].AddTextPiece( FONT_SIZE_12, STR(8) );

		m_ItemSelectPrinter.SetTextStyle( TS_NORMAL );
		m_ItemSelectPrinter.SetBkColor( 0, 0, 0 );	
		m_ItemSelectPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
		m_ItemSelectPrinter.AddTextPiece( FONT_SIZE_12, STR(9) );
		return;
	}

	//enable power up check
	ioHashString szError;
	if ( !g_PowerUpManager.CheckEnableItemPowerUp( iIndex, szError ) )
		LOG.PrintConsoleLog( 0, "ItemReinforceDownWnd::SetItemInfo - %s",szError.c_str() );

	m_iItemIndex = iIndex;

	m_iItemReinforce = ExtraItem.m_iReinforce;

	m_pIcon = g_UIImageSetMgr.CreateImageByFullName( ExtraItem.m_IconName );

	const ioItem *pItem = g_ItemMaker.GetItemConst( ExtraItem.m_iItemCode, __FUNCTION__ );
	if( pItem )
		m_nGradeType = pItem->GetGradeType();

	char szBuf[MAX_PATH], szText[MAX_PATH];
	memset( szBuf, 0, sizeof(szBuf) );
	memset( szText, 0, sizeof(szText) );

	//아이템
	int iEquipSlotIndex = ExtraItem.m_iItemCode / DEFAULT_BASIC_ITEM_CODE;
	if ( iEquipSlotIndex == ES_WEAPON )
	{
		int iGrade = ConvertPowerUpItemToItemGrade( ExtraItem.m_iItemCode );
		if ( !IsPowerUpItemGrade( iGrade ) )
			SafeSprintf( szText, sizeof( szText ), STR(2) );
		else
		{
			ioHashString szGradeTitleName = g_PowerUpManager.GetItemGradeTitle( ExtraItem.m_iItemCode, iGrade );
			if( !szGradeTitleName.IsEmpty() )
				SafeSprintf( szText, sizeof( szText ), STR(3), szGradeTitleName.c_str() );
		}
	}
	else
	{
		int iGrade = ConvertPowerUpRareItemToRareItemGrade( ExtraItem.m_iItemCode );
		if ( !IsPowerUpRareItemGrade( iGrade ) )
			SafeSprintf( szText, sizeof( szText ), STR(2) );
		else
		{
			ioHashString szGradeTitleName = g_PowerUpManager.GetExtraItemGradeTitle( ExtraItem.m_iItemCode, iGrade );
			if( !szGradeTitleName.IsEmpty() )
				SafeSprintf( szText, sizeof( szText ), STR(3), szGradeTitleName.c_str() );
		}
	}

	const ioHashString szItemName = ExtraItem.m_Name;
	SafeSprintf( szBuf, sizeof( szBuf ), STR(10), szItemName.c_str() );

	//아이템 레벨 및 이름
	m_ItemInfo[0].SetTextStyle( TS_NORMAL );
	m_ItemInfo[0].SetBkColor( 0, 0, 0 );
	m_ItemInfo[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
	m_ItemInfo[0].AddTextPiece( FONT_SIZE_12, STR(1), szBuf );

	//영구 아이템
	m_ItemInfo[1].SetTextStyle( TS_NORMAL );
	m_ItemInfo[1].SetBkColor( 0, 0, 0 );	
	m_ItemInfo[1].SetTextColor( TCT_DEFAULT_BLUE );
	m_ItemInfo[1].AddTextPiece( FONT_SIZE_12, szText );

	m_ItemReinforceInfo[0].SetTextStyle( TS_NORMAL );
	m_ItemReinforceInfo[0].SetBkColor( 0, 0, 0 );	

	for( int i=0; i < ITEM_REINFORCE_INFO_CNT; ++i )
	{
		if ( iEquipSlotIndex == ES_WEAPON )
			g_PowerUpManager.SetItemGardePrinter( ExtraItem.m_iItemCode, iEquipSlotIndex, i, FONT_SIZE_12, m_ItemReinforceInfo[i] );
		else
			g_PowerUpManager.SetExtraItemGardePrinter( ExtraItem.m_iItemCode, iEquipSlotIndex, i, FONT_SIZE_12, m_ItemReinforceInfo[i] );
	}

	memset( szText, 0, sizeof(szText) );
	SafeSprintf( szText, sizeof( szText ), STR(4), iSoulStoneCount );
	m_SoulStonePrinter.SetTextStyle( TS_NORMAL );
	m_SoulStonePrinter.SetBkColor( 0, 0, 0 );	
	m_SoulStonePrinter.SetTextColor( TCT_DEFAULT_RED );
	m_SoulStonePrinter.AddTextPiece( FONT_SIZE_12, szText );

	m_ItemSelectPrinter.SetTextStyle( TS_NORMAL );
	m_ItemSelectPrinter.SetBkColor( 0, 0, 0 );	
	m_ItemSelectPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
	m_ItemSelectPrinter.AddTextPiece( FONT_SIZE_12, szItemName.c_str() );
}

void ItemReinforceDownWnd::CreateMagicCircle()
{
	DWORD dwCurTime = FRAMEGETTIME();

	MagicEffectInfo kInfo;
	kInfo.m_dwCreateTime = dwCurTime;

	m_vMagicEffectInfoList.push_back( kInfo );
}

void ItemReinforceDownWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() )
		return;

	ProcessMagicCircle();

	if ( m_bShowResultEffect )
		ProcessResultEffect();


	ioWnd::OnProcess( fTimePerSec );
}

void ItemReinforceDownWnd::ProcessMagicCircle()
{
	DWORD dwCurTime = FRAMEGETTIME();

	if( m_dwMagicEffectCreateTime == 0 ||
		m_dwMagicEffectCreateTime+1000 < dwCurTime )
	{
		CreateMagicCircle();
		m_dwMagicEffectCreateTime = dwCurTime;
	}

	// process
	vMagicEffectInfo::iterator iter = m_vMagicEffectInfoList.begin();
	while( iter != m_vMagicEffectInfoList.end() )
	{
		DWORD dwGap = dwCurTime - (*iter).m_dwCreateTime;
		if( dwGap >= 1500 )
		{
			iter = m_vMagicEffectInfoList.erase( iter );
			continue;
		}

		float fRate = 1.0f - ((float)dwGap / 1500.0f);

		float fCurEffectRate = (float)sin(D3DX_PI/2.0f * fRate);

		// 0.5f -> 0.0f
		(*iter).m_iCurAlphaRate = (float)MAX_ALPHA_RATE * (0.5f*fCurEffectRate);

		// 1.0f -> 1.25f
		(*iter).m_fCurScaleRate = 1.25f - (0.25f*fCurEffectRate);

		++iter;
	}
}

void ItemReinforceDownWnd::ProcessResultEffect()
{
	DWORD dwCurTime = FRAMEGETTIME();

	if( m_dwResultEndTime < dwCurTime )
	{
		if ( g_PowerUpManager.GetReinforcedItemIndex() != 0  )
		{
			ClearListAll();
			MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(g_GUIMgr.FindWnd(MY_INVENTORY_WND));
			if( pInvenWnd && pInvenWnd->IsShow() )
				pInvenWnd->ShowItemReinforceDoneWnd( g_PowerUpManager.GetReinforcedItemIndex() );

			g_PowerUpManager.ClearReinforcedItemIndex();
			m_bShowResultEffect = false;
			HideWnd();
		}
	}

	// 원 이펙트
	if( m_dwResultEffectEndTime > 0 && dwCurTime < m_dwResultEffectEndTime )
	{
		float fRate = 1.0f;
		if( m_dwResultEffectTime > 0 )
		{
			DWORD dwGap = m_dwResultEffectEndTime - dwCurTime;
			fRate = 1.0f - ((float)dwGap / (float)m_dwResultEffectTime);
		}

		//float fCurEffectRate = (float)sin(D3DX_PI/2.0f * fRate);
		float fXValue = fRate - 1.0f;
		float fPowValue = fabs( pow(fXValue, m_fSlopeRate) );
		float fCurEffectRate = -fPowValue + 1.0f;

		// 0.0f -> 1.0f
		m_iAlpha1 = (float)MAX_ALPHA_RATE * fCurEffectRate;

		// 0.0f -> alpharate
		m_iAlpha2 = (float)MAX_ALPHA_RATE * (m_fAlphaRate*fCurEffectRate);

		// scalerate -> 1.0f
		float fScaleGap = m_fScaleRate - 1.0f;
		m_fCurEffectScale = m_fScaleRate - (fScaleGap*fCurEffectRate);
	}
}

void ItemReinforceDownWnd::OnRender()
{
	ioWnd::OnRender();

	int nXPos = GetDerivedPosX();
	int nYPos = GetDerivedPosY();

	enum 
	{	
		LINE_OFFSET_X = 28, 
		LINE_OFFSET_Y = 288,

		WEAPON_INFO_OFFSET_X = 128,
		WEAPON_INFO_OFFSET_Y = 285,
		WEAPON_LINE_GAP	   = 16,

		WEAPON_REINFORCE_INFO_OFFSET_X = 33,
		WEAPON_REINFORCE_INFO_OFFSET_Y = 327,
		WEAPON_REINFORCE_LINE_GAP	   = 21,

		SOULSTONE_OFFSET_X = 128,
		SOULSTONE_OFFSET_Y = 407,
	};

	OnRenderMagicCircle();

	if ( m_bShowResultEffect )
		OnRenderResultEffect();

	if ( !m_pNameLine ) return;

	m_pNameLine->Render( nXPos+LINE_OFFSET_X, nYPos+LINE_OFFSET_Y );
	

	for( int i=0; i < ItemReinforceDownWnd::ITEM_INFO_CNT; ++i )
	{
		if( m_ItemInfo[i].IsEmpty() )
			break;

		m_ItemInfo[i].PrintFullText( nXPos+WEAPON_INFO_OFFSET_X, nYPos+WEAPON_INFO_OFFSET_Y+(WEAPON_LINE_GAP*i), TAT_CENTER );
	}
	
	for( int i=0; i < ITEM_REINFORCE_INFO_CNT; ++i )
	{
		if( m_ItemReinforceInfo[i].IsEmpty() )
			break;

		m_ItemReinforceInfo[i].PrintFullText( nXPos+WEAPON_REINFORCE_INFO_OFFSET_X, nYPos+WEAPON_REINFORCE_INFO_OFFSET_Y+(WEAPON_REINFORCE_LINE_GAP*i), TAT_LEFT );
	}

	if ( !m_SoulStonePrinter.IsEmpty() )
		m_SoulStonePrinter.PrintFullText( nXPos+SOULSTONE_OFFSET_X, nYPos+SOULSTONE_OFFSET_Y, TAT_CENTER );
	
}

void ItemReinforceDownWnd::OnRenderMagicCircle()
{
	
	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	enum
	{
		MAGIC_CIRCLE_OFFSET_X = 123,
		MAGIC_CIRCLE_OFFSET_Y = 167,

		ICON_OFFSET_X = 123,
		ICON_OFFSET_Y = 167,

		REINFORCE_Y	= 14,

		NUM_SIZE	= 20,
		PLUS_SIZE	= 14,

		NUM_GAP		= 4,
	};

	if( m_pMagicCircle )
	{
		m_pMagicCircle->SetAlpha( MAX_ALPHA_RATE );
		m_pMagicCircle->SetScale( 1.0f );
		m_pMagicCircle->Render( iXPos + MAGIC_CIRCLE_OFFSET_X, iYPos + MAGIC_CIRCLE_OFFSET_Y );

		// 퍼지는 이펙트
		vMagicEffectInfo::iterator iter = m_vMagicEffectInfoList.begin();
		while( iter != m_vMagicEffectInfoList.end() )
		{
			m_pMagicCircle->SetAlpha( (*iter).m_iCurAlphaRate );
			m_pMagicCircle->SetScale( (*iter).m_fCurScaleRate );
			m_pMagicCircle->Render( iXPos + MAGIC_CIRCLE_OFFSET_X, iYPos + MAGIC_CIRCLE_OFFSET_Y );

			++iter;
		}
	}

	if ( m_pIconBack )
		m_pIconBack->Render( iXPos + ICON_OFFSET_X, iYPos + ICON_OFFSET_Y, UI_RENDER_MULTIPLY );

	g_UIImageRenderMgr.RenderGradeIconBack( m_nGradeType, iXPos + ICON_OFFSET_X, iYPos + ICON_OFFSET_Y, 1.0f, m_iIconRenderType, TFO_NEAREST, m_iAlpha1, m_iAlpha1, m_iAlpha1 );

	if ( m_pIcon )
	{
		if ( m_iIconRenderType == UI_RENDER_NORMAL_ADD_COLOR )
			m_pIcon->SetColor( m_iAlpha1 ,m_iAlpha1,m_iAlpha1 );
		m_pIcon->Render( iXPos + ICON_OFFSET_X, iYPos + ICON_OFFSET_Y, m_iIconRenderType );
		if( m_pNumText && m_pPlusText && m_iItemReinforce > 0 )
		{
			int iCurPos = 0;
			int iDefaultPos = iXPos + ICON_OFFSET_X;

			int iCurYPos = iYPos + ICON_OFFSET_Y + REINFORCE_Y ;

			int iTotalSize = 0;
			if( m_iItemReinforce < 10 )
				iTotalSize = PLUS_SIZE + NUM_SIZE - NUM_GAP;
			else if( m_iItemReinforce < 100 )
				iTotalSize = PLUS_SIZE + NUM_SIZE*2 - NUM_GAP*2;
			else
				iTotalSize = PLUS_SIZE + NUM_SIZE*3 - NUM_GAP*3;

			iCurPos = iDefaultPos - (iTotalSize / 2);

			m_pPlusText->Render( iCurPos, iCurYPos );

			iCurPos += (PLUS_SIZE-NUM_GAP);

			m_pNumText->RenderNum( iCurPos, iCurYPos, m_iItemReinforce, -NUM_GAP );
		}
	}
}

void ItemReinforceDownWnd::OnRenderResultEffect()
{
	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();
	enum
	{
		ICON_OFFSET_X = 123,
		ICON_OFFSET_Y = 167,
	};

	if( m_pLightImage )
	{
		m_pLightImage->SetScale( 1.0f );
		m_pLightImage->SetAlpha( m_iAlpha1 );
		m_pLightImage->Render( iXPos + ICON_OFFSET_X, iYPos + ICON_OFFSET_Y );

		m_pLightImage->SetScale( m_fCurEffectScale );
		m_pLightImage->SetAlpha( m_iAlpha2 );
		m_pLightImage->Render( iXPos + ICON_OFFSET_X, iYPos + ICON_OFFSET_Y);
	}
}

void ItemReinforceDownWnd::OnRenderAfterChild()
{
	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();
	enum
	{
		ITEM_SELECT_OFFSET_X = 131,
		ITEM_SELECT_OFFSET_Y = 428,
	};

	m_ItemSelectPrinter.PrintFullText( iXPos+ITEM_SELECT_OFFSET_X, iYPos+ITEM_SELECT_OFFSET_Y, TAT_CENTER );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


ItemReinforceDoneWnd::ItemReinforceDoneWnd( void )
{
	m_pMagicCircle = NULL;
	m_pNameLine = NULL;
	m_pIconBack = NULL;
	m_pIcon = NULL;
	m_pSuccessLightImage = NULL;
	m_pNumText = NULL;
	m_pPlusText = NULL;

	m_dwSuccessEffectTime = 0;
	m_dwRollingEffectTime = 0;

	ClearListAll();
}

ItemReinforceDoneWnd::~ItemReinforceDoneWnd(void)
{
	SAFEDELETE(m_pMagicCircle);
	SAFEDELETE(m_pNameLine);
	SAFEDELETE(m_pIconBack);
	SAFEDELETE(m_pIcon);
	SAFEDELETE(m_pSuccessLightImage);
	SAFEDELETE( m_pNumText );
	SAFEDELETE( m_pPlusText );

	ClearListAll();
}

void ItemReinforceDoneWnd::ClearListAll()
{
	for( int i=0; i < ItemReinforceDoneWnd::ITEM_INFO_CNT; ++i )
		m_ItemInfo[i].ClearList();

	for( int i=0; i < ITEM_REINFORCED_INFO_CNT; ++i )
		m_ItemReinforceInfo[i].ClearList();

	m_SoulStonePrinter.ClearList();
	m_ReinforcedPrinter.ClearList();

	m_dwMagicEffectCreateTime = 0;
	m_vMagicEffectInfoList.clear();

	m_dwFinalResultStartTime = 0;

	m_dwRollingEffectCheckTime = 0;

	m_iCurSuccessEffectArray = -1;

	SAFEDELETE(m_pSuccessLightImage);

	m_iItemIndex = -1;

	m_iItemReinforce = 0;

	m_nGradeType = 0;
}

void ItemReinforceDoneWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "NameLine" )
	{
		SAFEDELETE( m_pNameLine );
		m_pNameLine = pImage;
	}
	else if( szType == "magic_circle" )
	{
		SAFEDELETE( m_pMagicCircle );
		m_pMagicCircle = pImage;
	}
	else if( szType == "Icon_back" )
	{
		SAFEDELETE( m_pIconBack );
		m_pIconBack = pImage;
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
	else
		ioWnd::AddRenderImage( szType, pImage );
}

void ItemReinforceDoneWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	ioWnd::ParseExtraInfo( xElement );

	char szKeyName[MAX_PATH]="";

	m_dwSuccessEffectTime = xElement.GetIntAttribute_e( "success_effect_Time" );

	m_dwRollingEffectTime = xElement.GetIntAttribute_e( "Effect_Gap_Time" );

	m_vSuccessLightImage.clear();
	int iMax = xElement.GetIntAttribute_e( "Max_Success_Effect" );
	m_vSuccessLightImage.reserve( iMax );
	for (int i = 0; i < iMax ; i++)
	{
		char szName[MAX_PATH]="";
		StringCbPrintf( szName, sizeof( szName ), "Success_Effect_%d", i+1 );
		ioHashString szEffect = xElement.GetStringAttribute( szName );
		if( szEffect == "" )
			break;
		m_vSuccessLightImage.push_back( szEffect );
	}

	ZeroMemory( szKeyName, sizeof( szKeyName ) );
	StringCbPrintf( szKeyName, sizeof( szKeyName ), "success_sound" );
	m_szSuccessSound = xElement.GetStringAttribute( szKeyName );
}

void ItemReinforceDoneWnd::iwm_show()
{
	if( !m_szSuccessSound.IsEmpty() )
		g_SoundMgr.PlaySound( m_szSuccessSound, DSBVOLUME_MAX, PSM_THREAD );
}

void ItemReinforceDoneWnd::iwm_hide()
{
	if( !m_szSuccessSound.IsEmpty() )
		g_SoundMgr.StopSound( m_szSuccessSound, 0 );
}

void ItemReinforceDoneWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();	
	switch( dwID )
	{
	case ID_REINFORECE_AGAIN_BTN:
		{
			if( cmd == IOBN_BTNUP )
			{
				// solt check
				EXTRAITEMSLOT kSlot;
				ioUserExtraItem *pExtraItem = g_MyInfo.GetUserExtraItem();
				if( !pExtraItem )
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
					return;
				}

				EXTRAITEMSLOT ExtraItem;
				ExtraItem.Init();
				if ( !pExtraItem->GetExtraItem( m_iItemIndex, ExtraItem ) )
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
					return;
				}

				ioHashString szError;
				if( !g_PowerUpManager.CheckEnableItemPowerUp( m_iItemIndex, szError ) )
				{
					g_GUIMgr.SetMsgBox( MB_OK, this, szError.c_str() );
					break;
				}

				int iUseMtrlCount,iCurMtrlCount;
				if( !g_PowerUpManager.CheckEnableSoulStone( ioPowerUpManager::PIT_ITEM ,ExtraItem.m_iItemCode, iUseMtrlCount, iCurMtrlCount ) )
				{
					SoulStoneBuyWnd *pSoulstoneBuyWnd = dynamic_cast<SoulStoneBuyWnd*> ( g_GUIMgr.FindWnd( SOULSTONE_BUY_WND ) );
					if ( pSoulstoneBuyWnd )
						pSoulstoneBuyWnd->ShowBuyWnd( iUseMtrlCount, iCurMtrlCount );
					break;
				}

				ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
				char szBuf[MAX_PATH];
				memset( szBuf, 0, sizeof(szBuf) );
				kPrinter[0].SetTextStyle( TS_NORMAL );
				kPrinter[0].SetBkColor( 0, 0, 0 );
				kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
				SafeSprintf( szBuf, sizeof( szBuf ), STR(1), iUseMtrlCount );
				kPrinter[0].AddTextPiece( FONT_SIZE_13, szBuf );

				kPrinter[1].SetTextStyle( TS_NORMAL );
				kPrinter[1].SetBkColor( 0, 0, 0 );
				kPrinter[1].SetTextColor( TCT_DEFAULT_GRAY );
				kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(2) );
				
				HideWnd();
				MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(g_GUIMgr.FindWnd(MY_INVENTORY_WND));
				ioWnd* pItemReinforceDownWnd = NULL;
				if( pInvenWnd && pInvenWnd->IsShow() )
					pItemReinforceDownWnd = pInvenWnd->ShowItemReinforceDownWnd( m_iItemIndex );
				if ( pItemReinforceDownWnd != NULL)
					g_GUIMgr.SetPrevMsgListBox( NULL, MB_YESNO, pItemReinforceDownWnd, kPrinter );
			}
		}
		break;
	case ID_SEE_ITEM_BTN:
		{
			if( cmd == IOBN_BTNUP )
			{
				MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*> ( GetParent() );
				if( pInvenWnd && pInvenWnd->IsShow() )
				{
					HideWnd();
					pInvenWnd->ShowExtraItemTabDirect( g_MyInfo.GetClassType(), m_iItemIndex );
				}
			}
		}
		break;
	case MESSAGE_BOX3:
		{
			if( cmd == IOBN_BTNUP )
			{
				if( param == IDYES )
				{
					if ( m_iItemIndex == -1 )
						break;
					// solt check
					EXTRAITEMSLOT kSlot;
					ioUserExtraItem *pExtraItem = g_MyInfo.GetUserExtraItem();
					if( !pExtraItem )
					{
						g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
						return;
					}

					EXTRAITEMSLOT ExtraItem;
					ExtraItem.Init();
					if ( !pExtraItem->GetExtraItem( m_iItemIndex, ExtraItem ) )
					{
						g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
						return;
					}

					int iUseMtrlCount,iCurMtrlCount;
					if( !g_PowerUpManager.CheckEnableSoulStone( ioPowerUpManager::PIT_ITEM ,ExtraItem.m_iItemCode, iUseMtrlCount, iCurMtrlCount ) )
					{
						SoulStoneBuyWnd *pSoulstoneBuyWnd = dynamic_cast<SoulStoneBuyWnd*> ( g_GUIMgr.FindWnd( SOULSTONE_BUY_WND ) );
						if ( pSoulstoneBuyWnd )
							pSoulstoneBuyWnd->ShowBuyWnd( iUseMtrlCount, iCurMtrlCount );
						break;
					}

					ioHashString szError;
					if( !g_PowerUpManager.CheckEnableItemPowerUp( m_iItemIndex, szError ) )
					{
						g_GUIMgr.SetMsgBox( MB_OK, this, szError.c_str() );
						break;
					}

					ioEtcItemSoulStone* pEtcItem = dynamic_cast<ioEtcItemSoulStone*>( g_EtcItemMgr.FindEtcItem( g_PowerUpManager.GetMtrlEtcItemCode() ) );
					if( pEtcItem )
						pEtcItem->OnUseItemPowerUp( m_iItemIndex );
				}
			}
		}
		break;
	}
}

void ItemReinforceDoneWnd::SetItemInfo( int iIndex )
{
	ClearListAll();

	m_dwFinalResultStartTime = FRAMEGETTIME();

	//soulstone check
	int iSoulStoneCount = 0;
	ioUserEtcItem *pUserEtcItem = g_MyInfo.GetUserEtcItem();
	if( pUserEtcItem )
	{
		ioUserEtcItem::ETCITEMSLOT kSlot;
		if( pUserEtcItem->GetEtcItem( ioEtcItem::EIT_ETC_SOUL_STONE, kSlot ) )
			iSoulStoneCount = kSlot.m_iValue1;
	}

	// solt check
	EXTRAITEMSLOT kSlot;
	ioUserExtraItem *pExtraItem = g_MyInfo.GetUserExtraItem();
	if( !pExtraItem )
		return;

	EXTRAITEMSLOT ExtraItem;
	ExtraItem.Init();
	if ( !pExtraItem->GetExtraItem( iIndex, ExtraItem ) )
	{
		LOG.PrintConsoleLog( 0, "ItemReinforceDownWnd::SetItemInfo - item is not exist");
		return;
	}

	m_iItemIndex = iIndex;

	m_iItemReinforce = ExtraItem.m_iReinforce;

	if ( m_pIcon != NULL )
		SAFEDELETE( m_pIcon );

	m_pIcon = g_UIImageSetMgr.CreateImageByFullName( ExtraItem.m_IconName );

	const ioItem *pItem = g_ItemMaker.GetItemConst( ExtraItem.m_iItemCode, __FUNCTION__ );
	if( pItem )
		m_nGradeType = pItem->GetGradeType();

	char szBuf[MAX_PATH], szText[MAX_PATH];
	memset( szBuf, 0, sizeof(szBuf) );
	memset( szText, 0, sizeof(szText) );

	//아이템
	int iEquipSlotIndex = ExtraItem.m_iItemCode / DEFAULT_BASIC_ITEM_CODE;
	int iGrade;
	ioHashString szGradeTitleName;
	if ( iEquipSlotIndex == ES_WEAPON )
	{
		iGrade = ConvertPowerUpItemToItemGrade( ExtraItem.m_iItemCode );
		szGradeTitleName = g_PowerUpManager.GetItemGradeTitle( ExtraItem.m_iItemCode, iGrade );
	}
	else
	{
		iGrade = ConvertPowerUpRareItemToRareItemGrade( ExtraItem.m_iItemCode );
		szGradeTitleName = g_PowerUpManager.GetExtraItemGradeTitle( ExtraItem.m_iItemCode, iGrade );
	}

	if( szGradeTitleName.IsEmpty() )
		return;

	SafeSprintf( szText, sizeof( szText ), STR(3), szGradeTitleName.c_str() );
	SafeSprintf( szBuf, sizeof( szBuf ), STR(1), ExtraItem.m_Name.c_str() );

	//아이템 레벨 및 이름
	m_ItemInfo[0].SetTextStyle( TS_NORMAL );
	m_ItemInfo[0].SetBkColor( 0, 0, 0 );
	m_ItemInfo[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
	m_ItemInfo[0].AddTextPiece( FONT_SIZE_12, szBuf );

	//영구 사용
	m_ItemInfo[1].SetTextStyle( TS_NORMAL );
	m_ItemInfo[1].SetBkColor( 0, 0, 0 );	
	m_ItemInfo[1].SetTextColor( TCT_DEFAULT_BLUE );
	m_ItemInfo[1].AddTextPiece( FONT_SIZE_12, szText );
	
	for( int i=0; i < ITEM_REINFORCED_INFO_CNT; ++i )
	{
		if ( iEquipSlotIndex == ES_WEAPON )
			g_PowerUpManager.SetItemGardedPrinter( ExtraItem.m_iItemCode, ConvertPowerUpItemToItemGrade( ExtraItem.m_iItemCode ), iEquipSlotIndex, i, FONT_SIZE_12, m_ItemReinforceInfo[i] );
		else
			g_PowerUpManager.SetExtraItemGardedPrinter( ExtraItem.m_iItemCode, ConvertPowerUpRareItemToRareItemGrade( ExtraItem.m_iItemCode ), iEquipSlotIndex, i, FONT_SIZE_12, m_ItemReinforceInfo[i] );
	}

	memset( szText, 0, sizeof(szText) );
	SafeSprintf( szText, sizeof( szText ), STR(4), iSoulStoneCount );
	m_SoulStonePrinter.SetTextStyle( TS_NORMAL );
	m_SoulStonePrinter.SetBkColor( 0, 0, 0 );	
	m_SoulStonePrinter.SetTextColor( TCT_DEFAULT_RED );
	m_SoulStonePrinter.AddTextPiece( FONT_SIZE_12, szText );

	memset( szText, 0, sizeof(szText) );
	SafeSprintf( szText, sizeof( szText ), STR(5),  szGradeTitleName.c_str() );
	m_ReinforcedPrinter.SetTextStyle( TS_NORMAL );
	m_ReinforcedPrinter.SetBkColor( 0, 0, 0 );	
	m_ReinforcedPrinter.SetTextColor( TCT_DEFAULT_GREEN );
	m_ReinforcedPrinter.AddTextPiece( FONT_SIZE_17, szText );
}

void ItemReinforceDoneWnd::OnRender()
{
	ioWnd::OnRender();

	int nXPos = GetDerivedPosX();
	int nYPos = GetDerivedPosY();

	enum 
	{	
		LINE_OFFSET_X = 28, 
		LINE_OFFSET_Y = 288,

		WEAPON_INFO_OFFSET_X = 128,
		WEAPON_INFO_OFFSET_Y = 285,
		WEAPON_LINE_GAP	   = 16,

		WEAPON_REINFORCE_INFO_OFFSET_X = 33,
		WEAPON_REINFORCE_INFO_OFFSET_Y = 327,
		WEAPON_REINFORCE_LINE_GAP	   = 21,

		SOULSTONE_OFFSET_X = 128,
		SOULSTONE_OFFSET_Y = 407,

		REINFORCED_OFFSET_X = 129,
		REINFORCED_OFFSET_Y = 424,
	};

	OnRenderMagicCircle();

	if ( !m_pNameLine ) return;

	m_pNameLine->Render( nXPos+LINE_OFFSET_X, nYPos+LINE_OFFSET_Y );


	for( int i=0; i < ItemReinforceDoneWnd::ITEM_INFO_CNT; ++i )
	{
		if( m_ItemInfo[i].IsEmpty() )
			break;

		m_ItemInfo[i].PrintFullText( nXPos+WEAPON_INFO_OFFSET_X, nYPos+WEAPON_INFO_OFFSET_Y+(WEAPON_LINE_GAP*i), TAT_CENTER );
	}

	for( int i=0; i < ITEM_REINFORCED_INFO_CNT; ++i )
	{
		if( m_ItemReinforceInfo[i].IsEmpty() )
			break;

		m_ItemReinforceInfo[i].PrintFullText( nXPos+WEAPON_REINFORCE_INFO_OFFSET_X, nYPos+WEAPON_REINFORCE_INFO_OFFSET_Y+(WEAPON_REINFORCE_LINE_GAP*i), TAT_LEFT );
	}

	if ( !m_SoulStonePrinter.IsEmpty() )
		m_SoulStonePrinter.PrintFullText( nXPos+SOULSTONE_OFFSET_X, nYPos+SOULSTONE_OFFSET_Y, TAT_CENTER );

	if ( !m_ReinforcedPrinter.IsEmpty() )
		m_ReinforcedPrinter.PrintFullText( nXPos+REINFORCED_OFFSET_X, nYPos+REINFORCED_OFFSET_Y, TAT_CENTER );
}

void ItemReinforceDoneWnd::OnRenderMagicCircle()
{

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	enum
	{
		MAGIC_CIRCLE_OFFSET_X = 123,
		MAGIC_CIRCLE_OFFSET_Y = 167,

		ICON_OFFSET_X = 123,
		ICON_OFFSET_Y = 167,

		REINFORCE_Y	= 14,

		NUM_SIZE	= 20,
		PLUS_SIZE	= 14,

		NUM_GAP		= 4,
	};

	if( m_pMagicCircle )
	{
		m_pMagicCircle->SetAlpha( MAX_ALPHA_RATE );
		m_pMagicCircle->SetScale( 1.0f );
		m_pMagicCircle->Render( iXPos + MAGIC_CIRCLE_OFFSET_X, iYPos + MAGIC_CIRCLE_OFFSET_Y );

		// 퍼지는 이펙트
		vMagicEffectInfo::iterator iter = m_vMagicEffectInfoList.begin();
		while( iter != m_vMagicEffectInfoList.end() )
		{
			m_pMagicCircle->SetAlpha( (*iter).m_iCurAlphaRate );
			m_pMagicCircle->SetScale( (*iter).m_fCurScaleRate );
			m_pMagicCircle->Render( iXPos + MAGIC_CIRCLE_OFFSET_X, iYPos + MAGIC_CIRCLE_OFFSET_Y );

			++iter;
		}
	}

	if ( m_pIconBack )
		m_pIconBack->Render( iXPos + ICON_OFFSET_X, iYPos + ICON_OFFSET_Y, UI_RENDER_MULTIPLY );


	g_UIImageRenderMgr.RenderGradeIconBack( m_nGradeType, iXPos + ICON_OFFSET_X, iYPos + ICON_OFFSET_Y );

	if ( m_pIcon )
	{
		m_pIcon->Render( iXPos + ICON_OFFSET_X, iYPos + ICON_OFFSET_Y );
		if( m_pNumText && m_pPlusText && m_iItemReinforce > 0 )
		{
			int iCurPos = 0;
			int iDefaultPos = iXPos + ICON_OFFSET_X;

			int iCurYPos = iYPos + ICON_OFFSET_Y + REINFORCE_Y ;

			int iTotalSize = 0;
			if( m_iItemReinforce < 10 )
				iTotalSize = PLUS_SIZE + NUM_SIZE - NUM_GAP;
			else if( m_iItemReinforce < 100 )
				iTotalSize = PLUS_SIZE + NUM_SIZE*2 - NUM_GAP*2;
			else
				iTotalSize = PLUS_SIZE + NUM_SIZE*3 - NUM_GAP*3;

			iCurPos = iDefaultPos - (iTotalSize / 2);

			m_pPlusText->Render( iCurPos, iCurYPos );

			iCurPos += (PLUS_SIZE-NUM_GAP);

			m_pNumText->RenderNum( iCurPos, iCurYPos, m_iItemReinforce, -NUM_GAP );
		}
	}

	if ( m_pSuccessLightImage )
	{
		m_pSuccessLightImage->SetScale( 2.0f );
		m_pSuccessLightImage->Render( iXPos + ICON_OFFSET_X, iYPos + ICON_OFFSET_Y, UI_RENDER_ADD );
	}
}

void ItemReinforceDoneWnd::CreateMagicCircle()
{
	DWORD dwCurTime = FRAMEGETTIME();

	MagicEffectInfo kInfo;
	kInfo.m_dwCreateTime = dwCurTime;

	m_vMagicEffectInfoList.push_back( kInfo );
}

void ItemReinforceDoneWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() )
		return;

	DWORD dwCurTime = FRAMEGETTIME();

	ProcessMagicCircle();

	ProcessSuccessLightEffect();

	ioWnd::OnProcess( fTimePerSec );
}

void ItemReinforceDoneWnd::ProcessMagicCircle()
{
	DWORD dwCurTime = FRAMEGETTIME();

	if( m_dwMagicEffectCreateTime == 0 ||
		m_dwMagicEffectCreateTime+1000 < dwCurTime )
	{
		CreateMagicCircle();
		m_dwMagicEffectCreateTime = dwCurTime;
	}

	// process
	vMagicEffectInfo::iterator iter = m_vMagicEffectInfoList.begin();
	while( iter != m_vMagicEffectInfoList.end() )
	{
		DWORD dwGap = dwCurTime - (*iter).m_dwCreateTime;
		if( dwGap >= 1500 )
		{
			iter = m_vMagicEffectInfoList.erase( iter );
			continue;
		}

		float fRate = 1.0f - ((float)dwGap / 1500.0f);

		float fCurEffectRate = (float)sin(D3DX_PI/2.0f * fRate);

		// 0.5f -> 0.0f
		(*iter).m_iCurAlphaRate = (float)MAX_ALPHA_RATE * (0.5f*fCurEffectRate);

		// 1.0f -> 1.25f
		(*iter).m_fCurScaleRate = 1.25f - (0.25f*fCurEffectRate);

		++iter;
	}
}

void ItemReinforceDoneWnd::ProcessSuccessLightEffect()
{
	bool bSuccess = true;
	bool bChange = false;

	DWORD dwCurTime = FRAMEGETTIME();

	if( bSuccess )
	{
		if( m_dwFinalResultStartTime > 0 &&  (dwCurTime - m_dwFinalResultStartTime) < m_dwSuccessEffectTime )
			bSuccess = true;
		else
		{
			SAFEDELETE( m_pSuccessLightImage );
			bSuccess = false;
		}
	}
	if( m_dwRollingEffectCheckTime == 0 || dwCurTime - m_dwRollingEffectCheckTime > m_dwRollingEffectTime )		// 교체
	{
		bChange = true;
		m_dwRollingEffectCheckTime = dwCurTime;
	}

	// success effect
	if( bSuccess )
	{
		int iEffectListCnt = m_vSuccessLightImage.size();
		if( iEffectListCnt == 0 )
		{
			SAFEDELETE( m_pSuccessLightImage );
			return;
		}
		else if( bChange )
		{
			SAFEDELETE( m_pSuccessLightImage );

			m_iCurSuccessEffectArray++;

			if( COMPARE( m_iCurSuccessEffectArray, 0, iEffectListCnt ) )
			{
				ioHashString szEffect = m_vSuccessLightImage[m_iCurSuccessEffectArray];

				if( !szEffect.IsEmpty() )
					m_pSuccessLightImage = g_UIImageSetMgr.CreateImageByFullName( szEffect );
			}
		}
	}

}

void ItemReinforceDoneWnd::OnRenderSuccessLightEffect()
{
	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();
	enum
	{
		ICON_OFFSET_X = 123,
		ICON_OFFSET_Y = 167,
	};
	if ( m_pSuccessLightImage )
	{
		m_pSuccessLightImage->SetScale( 2.0f );
		m_pSuccessLightImage->Render( iXPos + ICON_OFFSET_X, iYPos + ICON_OFFSET_Y, UI_RENDER_ADD );
	}
}
