
#include "StdAfx.h"

#include "ItemMaterialCompoundWnd.h"

#include "SelectMaterialItemBtn.h"
#include "ItemCompoundWnd.h"

#include "../ioAlchemicMgr.h"

#include "MyItemSelecteWnd.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ItemMaterialCompoundWnd::ItemMaterialCompoundWnd(void)
{
	m_nReUseSlot = 0;
	m_pManual = NULL;
	m_pExtraSubBack = NULL;
	m_pManualMark = NULL;
	m_pGaugeMainBack = NULL;
	m_pGaugeBack  = NULL;
	m_pGauge      = NULL;

	InitData();
}

ItemMaterialCompoundWnd::~ItemMaterialCompoundWnd(void)
{
	SAFEDELETE( m_pExtraSubBack );
	SAFEDELETE( m_pManualMark );
	SAFEDELETE( m_pGaugeMainBack );
	SAFEDELETE( m_pGaugeBack );
	SAFEDELETE( m_pGauge );
}

void ItemMaterialCompoundWnd::iwm_create()
{
}

void ItemMaterialCompoundWnd::iwm_show()
{
	HideChildWnd( ID_TARGETITEM_WND );
	HideChildWnd( ID_MATERIALITEM_WND );
	HideChildWnd( ID_FAIL_MANUAL_BTN );

	if ( m_pManualMark )
		m_pManualMark->SetColor( 128, 128, 128 );

	InitData();

	m_pManual = ioEtcInvenSubManualMgr::GetSingletonPtr();
	
	//강화 가능 아이템 얻기
	LoadTotalItemList();	

	//강화 가능 아이템이 없다면 버튼 비활성화
	if( m_vTotalItemList.empty() )
		SetChildInActive( ID_TARGETITEM_BTN );
	else
		SetChildActive( ID_TARGETITEM_BTN );

	//재료 초기화
	SelectMaterialItemBtn *pMaterialItemIcon = (SelectMaterialItemBtn*)(FindChildWnd(ID_MATERIALITEM_ICON));
	if( pMaterialItemIcon )
	{
		pMaterialItemIcon->InitData();
		pMaterialItemIcon->ShowWnd();
	}

	SelectExtraItemBtn *pMaterialItemBtn = (SelectExtraItemBtn*)(FindChildWnd(ID_MATERIALITEM_BTN));
	if( pMaterialItemBtn )
	{
		pMaterialItemBtn->SetSelectMaterial( 0, "" );
		pMaterialItemBtn->SetCurTitleColor( TCT_DEFAULT_DARKGRAY );
	}

	//다시 사용 가능하다면 슬롯에 재설정
	if ( m_nReUseSlot > 0 )
	{
		if ( SlotReload() )
		{
			UpdateItemData();

			//재료가 없다면 비활성
			ioAlchemicInventory *pInven = g_MyInfo.GetAlchemicInventory();
			if( !pInven )
				return;

			vAlchemicItemInfoList vMaterialList;
			if ( !pInven->GetAlchemicItemInfo( vMaterialList ) )
			{
				SetChildInActive( ID_MATERIALITEM_BTN );
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
			}
		}
		else 
			TargetItemInit();
	}
	else 
		TargetItemInit();	
}

void ItemMaterialCompoundWnd::iwm_hide()
{
	InitData();
	HideChildWnd( ID_TARGETITEM_WND );
	HideChildWnd( ID_MATERIALITEM_WND );
	HideChildWnd( ID_MATERIALITEM_ICON );
}

void ItemMaterialCompoundWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_COMPOUND_BTN:
		if( cmd == IOBN_BTNUP )
		{
			SendItemCompound();
		}
		break;
	case ID_TARGETITEM_BTN:
		if( cmd == IOBN_BTNUP )
		{
			int nItemListSize = m_vTotalItemList.size();
			
			MyItemSelecteWnd *pSelectWnd = (MyItemSelecteWnd*)( g_GUIMgr.FindWnd( MYITEM_SELECT_WND ) );
			if ( pSelectWnd && nItemListSize > 0 )
			{
				ExtraItemList rkList;
				GetTargetSeletInfo( rkList );
				pSelectWnd->ShowItemSeletWnd( rkList, this );
			}
			
			/*
			NewShopPullDownWnd *pListWnd = (NewShopPullDownWnd*)(FindChildWnd(ID_TARGETITEM_WND));
			if( pListWnd && pListWnd->IsShow() )
			{
				pListWnd->HideWnd();
			}
			else if( pListWnd && nItemListSize > 0 )
			{
				pListWnd->CreatePullDown( pWnd, pWnd->GetXPos(), pWnd->GetYPos() + 25 );
				// 조건이 달라지면 리스트 따로 구하고...
				vNewShopPullDownItem kItemList;
				GetTargetList( kItemList );

				if( !kItemList.empty() )
				{
					int iSize = kItemList.size();
					for(int i = 0;i < iSize; i++)
					{
						pListWnd->InsertItem( kItemList[i] );
					}
					pListWnd->ShowWnd();
				}
				else
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
				}
			}
			*/
		}
		break;
	case ID_TARGETITEM_WND:
		if( cmd == IOBN_BTNUP )
		{
			NewShopPullDownWnd *pPullDownWnd = (NewShopPullDownWnd *)(pWnd);
			if( pPullDownWnd )
			{
				pPullDownWnd->HideWnd();
				TargetListBtnUp( pPullDownWnd->GetOpenBtn(), param );
				UpdateItemData();
			}
		}
		break;
	case ID_MATERIALITEM_BTN:
		if( cmd == IOBN_BTNUP )
		{
			//리스트 설정
			int nItemListSize = m_vTotalItemList.size();
			NewShopPullDownWnd *pListWnd = (NewShopPullDownWnd*)(FindChildWnd(ID_MATERIALITEM_WND));
			if( pListWnd && pListWnd->IsShow() )
			{
				pListWnd->HideWnd();
			}
			else if( pListWnd && nItemListSize > 0 )
			{
				pListWnd->CreatePullDown( pWnd, pWnd->GetXPos(), pWnd->GetYPos() + 25 );
				// 조건이 달라지면 리스트 따로 구하고...
				vNewShopPullDownItem kItemList;
				GetMaterialList( kItemList );

				if( !kItemList.empty() )
				{
					int iSize = kItemList.size();
					for(int i = 0;i < iSize; i++)
					{
						pListWnd->InsertItem( kItemList[i] );
					}
					pListWnd->ShowWnd();
				}
				else
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
				}
			}
		}
		break;
	case ID_MATERIALITEM_WND:
		if( cmd == IOBN_BTNUP )
		{
			//재료 선택 완료시
			NewShopPullDownWnd *pPullDownWnd = (NewShopPullDownWnd *)(pWnd);
			if( pPullDownWnd )
			{
				pPullDownWnd->HideWnd();
				MaterialListBtnUp( pPullDownWnd->GetOpenBtn(), param );
			}
		}
		break;
	case ID_FAIL_MANUAL_BTN:
		if( cmd == IOBN_BTNUP )
		{
			HideChildWnd( ID_FAIL_TOOL_TIP );
			ManualListWnd *pManualWnd = (ManualListWnd*) (g_GUIMgr.FindWnd( MANUAL_LIST_WND ) );
			if( pManualWnd )
				pManualWnd->ShowWndByCurManual( m_nHelpTipIdx );
		}
		else if( cmd == IOWN_OVERED )
		{
			if ( m_pManualMark )
				m_pManualMark->SetColor( 206, 78, 0 );

			ShowChildWnd( ID_FAIL_TOOL_TIP );
		}
		else if( cmd == IOWN_LEAVED )
		{
			if ( m_pManualMark )
				m_pManualMark->SetColor( 128, 128, 128 );

			HideChildWnd( ID_FAIL_TOOL_TIP );
		}
		break;
	case MYITEM_SELECT_WND:
		if( cmd == IOBN_BTNUP )
		{
			SelectExtraItemBtn *pTarget = dynamic_cast<SelectExtraItemBtn*>(FindChildWnd(ID_TARGETITEM_BTN));
			if( pTarget && param > 0 )
			{
				pTarget->SetSelectExtraItemSlot( param );
				UpdateItemData();
			}
		}
		break;
	}
}

void ItemMaterialCompoundWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "ExtraSubBack" )
	{
		SAFEDELETE( m_pExtraSubBack );
		m_pExtraSubBack = pImage;
	}
	else if( szType == "ManualMark" )
	{
		SAFEDELETE( m_pManualMark );
		m_pManualMark = pImage;
	}
	else if( szType == "MainBack" )
	{
		SAFEDELETE( m_pGaugeMainBack );
		m_pGaugeMainBack = pImage;
	}
	else if( szType == "GaugeBack" )
	{
		SAFEDELETE( m_pGaugeBack );
		m_pGaugeBack = pImage;
	}
	else if( szType == "Gauge" )
	{
		SAFEDELETE( m_pGauge );
		m_pGauge = pImage;
	}
	else
		ioWnd::AddRenderImage( szType, pImage );
}

void ItemMaterialCompoundWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	char szKeyName[MAX_PATH]="";
	ioHashString szTemp;
	
	ZeroMemory( szKeyName, sizeof( szKeyName ) );
	StringCbPrintf_e( szKeyName, sizeof( szKeyName ), "extra_sub_manual" );
	m_nExtraSubManualIdx = xElement.GetIntAttribute( szKeyName );

	ZeroMemory( szKeyName, sizeof( szKeyName ) );
	StringCbPrintf_e( szKeyName, sizeof( szKeyName ), "help_tip_idx" );
	m_nHelpTipIdx = xElement.GetIntAttribute( szKeyName );	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ItemMaterialCompoundWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	SuccessRateDescRender();
	RenderFailExp( iXPos, iYPos );
	PrintManual( iXPos, iYPos, FONT_SIZE_12 );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ItemMaterialCompoundWnd::SuccessRateDescRender()
{
	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	enum 
	{
		INFO_LEFT_X_OFFSET = 33,
		INFO_LEFT_Y_OFFSET = 415,

		INFO_RIGHT_X_OFFSET = 225,
		INFO_RIGHT_Y_OFFSET = 415,
	};

	if ( m_bIsSelectMaterial )
	{
		SelectExtraItemBtn *pTarget = (SelectExtraItemBtn*)(FindChildWnd(ID_TARGETITEM_BTN));
		if( !pTarget ) 
			return;

		int nTargetSlot = pTarget->GetSelectExtraItemSlot();
		if ( nTargetSlot > 0)
		{
			g_FontMgr.SetAlignType( TAT_LEFT );
			g_FontMgr.SetTextStyle( TS_NORMAL );
			g_FontMgr.SetBkColor( 0, 0, 0 );
			g_FontMgr.SetTextColor( TCT_DEFAULT_GREEN );
			g_FontMgr.PrintText( iXPos + INFO_LEFT_X_OFFSET, iYPos + INFO_LEFT_Y_OFFSET, FONT_SIZE_12, STR(1) );

			g_FontMgr.SetAlignType( TAT_RIGHT );
			g_FontMgr.SetTextColor( TCT_DEFAULT_GREEN );
			g_FontMgr.PrintText( iXPos + INFO_RIGHT_X_OFFSET, iYPos + INFO_RIGHT_Y_OFFSET, FONT_SIZE_12, STR(2), m_fSuccessRate );
		}
	}
	else
	{
		g_FontMgr.SetAlignType( TAT_LEFT );
		g_FontMgr.SetTextStyle( TS_NORMAL );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextColor( TCT_DEFAULT_RED );
		g_FontMgr.PrintText( iXPos + INFO_LEFT_X_OFFSET, iYPos + INFO_LEFT_Y_OFFSET, FONT_SIZE_12, STR(3) );
	}
}

void ItemMaterialCompoundWnd::RenderFailExp( int iXPos, int iYPos )
{
	if( !m_pManualMark || !m_pGaugeMainBack || !m_pGaugeBack || !m_pGauge || m_nMaxFailExp <= 0 )
		return;

	enum 
	{		
		GAUGE_X_OFFSET = 57,
		GAUGE_Y_OFFSET = 392,
		GAUGE_BACK_OFFSET = 2,

		TEXT_X_OFFSET  = 130,
		TEXT_Y_OFFSET  = 374,

		MANUAL_X_OFFSET = 207,
		MANUAL_Y_OFFSET = 390,
	};
	
	ioComplexStringPrinter kPrinter;
	kPrinter.SetTextStyle( TS_NORMAL );
	kPrinter.SetBkColor( 0, 0, 0 );
	kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );		
	kPrinter.AddTextPiece( FONT_SIZE_12, STR(1) );	
	kPrinter.SetTextColor( TCT_DEFAULT_GRAY );		
	kPrinter.AddTextPiece( FONT_SIZE_12, STR(2), (int)(m_fFailExpRate * FLOAT100) );	
	kPrinter.PrintFullText( iXPos + TEXT_X_OFFSET, iYPos + TEXT_Y_OFFSET, TAT_CENTER );
	kPrinter.ClearList();

	//게이지
	int nGaugeWidth = m_pGauge->GetWidth() * (float)min( FLOAT1, m_fFailExpRate );
	m_pGaugeMainBack->Render( iXPos + GAUGE_X_OFFSET - GAUGE_BACK_OFFSET, iYPos + GAUGE_Y_OFFSET - GAUGE_BACK_OFFSET );
	m_pGaugeBack->Render( iXPos + GAUGE_X_OFFSET, iYPos + GAUGE_Y_OFFSET );
	m_pGauge->RenderWidthCut( iXPos + GAUGE_X_OFFSET, iYPos + GAUGE_Y_OFFSET, 0, nGaugeWidth );
	m_pManualMark->Render( iXPos + MANUAL_X_OFFSET, iYPos + MANUAL_Y_OFFSET, UI_RENDER_NORMAL_ADD_COLOR );
}

void ItemMaterialCompoundWnd::PrintManual( int a_iXPos, int a_iYPos , float fScale )
{
	enum
	{
		DEFAULT_MANUAL_X_OFFSET = 28,
		DEFAULT_MANUAL_Y_OFFSET = 281,

		SUB_MANUAL_X_OFFSET = 28,
		SUB_MANUAL_Y_OFFSET = 281,

		EXTRA_SUB_MANUAL_X_OFFSET = 28,
		EXTRA_SUB_MANUAL_Y_OFFSET = 333,

		EXTRA_SUB_BACK_X_OFFSET = 27,
		EXTRA_SUB_BACK_Y_OFFSET = 332,
	};

	if( !m_pManual )
		return;

	ioEtcItem *pItem = g_EtcItemMgr.FindEtcItem( m_dwEtcItemCode );
	if( !pItem ) 
		return;

	int iSubManual = pItem->GetInventorySubManual() - 1;
	int nXpos = a_iXPos + DEFAULT_MANUAL_X_OFFSET;
	int nYpos = a_iYPos + DEFAULT_MANUAL_Y_OFFSET;

	ioComplexStringPrinter kPrinter;
	int iMaxLine = m_pManual->GetMaxLine( iSubManual );
	for(int iLine = 1; iLine < iMaxLine; iLine++)
	{
		m_pManual->SetPrinter( iSubManual, iLine, fScale, kPrinter);
		nYpos += m_pManual->GetSpaceLine( iSubManual, iLine);
		kPrinter.PrintFullTextWidthCut( nXpos, nYpos, TAT_LEFT, DESC_WIDTH );
		kPrinter.ClearList();
	}

	if ( m_bIsSelectItem )
	{
		if ( m_nCurReinforce >= g_CompoundMaterialMgr.GetFixedFailReinforce() )
		{
			iSubManual = m_nExtraSubManualIdx - 1;
			nXpos = a_iXPos + EXTRA_SUB_MANUAL_X_OFFSET;
			nYpos = a_iYPos + EXTRA_SUB_MANUAL_Y_OFFSET;

			int iMaxLine = m_pManual->GetMaxLine( iSubManual );
			for(int iLine = 1; iLine < iMaxLine; iLine++)
			{
				m_pManual->SetPrinter( iSubManual, iLine, fScale, kPrinter);
				nYpos += m_pManual->GetSpaceLine( iSubManual, iLine);
				kPrinter.PrintFullTextWidthCut( nXpos, nYpos, TAT_LEFT, DESC_WIDTH );
				kPrinter.ClearList();
			}
			m_pExtraSubBack->Render( a_iXPos+EXTRA_SUB_BACK_X_OFFSET, a_iYPos+EXTRA_SUB_BACK_Y_OFFSET );
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ItemMaterialCompoundWnd::ShowWndWithEtcItemCode( DWORD dwCode )
{
	m_dwEtcItemCode = dwCode;

	ShowWnd();
}

void ItemMaterialCompoundWnd::SetMaterialCode( int nMaterialCode )
{
	if ( !m_bIsSelectItem || m_nSelectMaterialCode == nMaterialCode )
		return;

	ioAlchemicInventory *pInven = g_MyInfo.GetAlchemicInventory();
	if( !pInven )
		return;

	int nCurMaterialCnt = pInven->GetAlchemicItemCnt( nMaterialCode );
	//재료 개수 확인
	if ( nCurMaterialCnt < m_nNeedMaterialCnt )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return;
	}
	
	SelectMaterialItemBtn *pMaterialItemIcon = (SelectMaterialItemBtn*)(FindChildWnd(ID_MATERIALITEM_ICON));
	if( pMaterialItemIcon )
		pMaterialItemIcon->SetData( nMaterialCode, m_nNeedMaterialCnt );
	
	m_nSelectMaterialCode = nMaterialCode;
	m_bIsSelectMaterial = true;

	//성공율 설정
	ApplySuccessRate();

	//경고문구 메시지 출력 (선택 재료가 첨가제가 아닐 경우, 필요재료가 아닐경우)
	if ( m_nSelectMaterialCode != ioItemCompoundMaterialManager::CT_Additive && 
		 m_nSelectMaterialCode != m_nNeedMaterialCode )
	{
		char szName[MAX_PATH] = "";
		if( m_nNeedMaterialCode > ALCHEMIC_ADDITIVE_DIV )
		{
			SafeSprintf( szName, sizeof( szName ), STR(3), g_AlchemicMgr.GetAlchemicItemName( m_nNeedMaterialCode ).c_str() );
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2), szName );
		}
		else
		{
			SafeSprintf( szName, sizeof( szName ), STR(1), g_MyInfo.GetClassName( m_nNeedMaterialCode ) );
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3), szName );
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ItemMaterialCompoundWnd::InitData()
{
	m_vTotalItemList.clear();
	m_nCurReinforce = 0;
	m_fSuccessRate = 0.0f;
	m_nNeedMaterialCnt = 0;
	m_nNeedMaterialCode = 0;
	m_nSelectMaterialCode = 0;
	m_bIsExtraItem = false;
	m_bIsSelectItem = false;
	m_bIsSelectMaterial = false;
	m_nMaxFailExp = 0;
	m_fFailExpRate = 0.0f;
	m_nCurFailExp = 0;
}

void ItemMaterialCompoundWnd::LoadTotalItemList()
{
	ioUserExtraItem *pItem = g_MyInfo.GetUserExtraItem();
	if( !pItem )
	{
		HideWnd();
		return;
	}

	int iSize = pItem->GetExtraItemCnt();
	for( int i=0; i < iSize; ++i )
	{
		EXTRAITEMSLOT kSlot;
		if( !pItem->GetExtraItemArray( i, kSlot ) )
			continue;

		if( kSlot.m_bCharEquip )
			continue;

		if( kSlot.m_PeriodType != EPT_MORTMAIN )
			continue;

		MaterialCompoundTargetItemInfo kInfo;
		kInfo.m_iSlotIndex = kSlot.m_iIndex;
		kInfo.m_iItemCode = kSlot.m_iItemCode;
		kInfo.m_iReinforce = kSlot.m_iReinforce;

		m_vTotalItemList.push_back( kInfo );
	}

	std::sort( m_vTotalItemList.begin(), m_vTotalItemList.end(), MaterialCompoundItemInfoSort() );
}

//강화 아이템 초기화
void ItemMaterialCompoundWnd::TargetItemInit()
{
	SelectExtraItemBtn *pTarget = (SelectExtraItemBtn*)(FindChildWnd(ID_TARGETITEM_BTN));
	if( pTarget )
		pTarget->InitData();

	InventoryIconBtn *pTargetIconBtn = (InventoryIconBtn*)(FindChildWnd(ID_TARGET_ICON));
	if( pTargetIconBtn )
		pTargetIconBtn->SetIcon( NULL, FLOAT1, FLOAT1 );

	SetChildInActive( ID_MATERIALITEM_BTN );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ItemMaterialCompoundWnd::GetTargetList( vNewShopPullDownItem &rkList )
{
	rkList.clear();

	ioUserExtraItem *pItem = g_MyInfo.GetUserExtraItem();
	if( !pItem )
		return;

	int iSize = m_vTotalItemList.size();
	rkList.reserve( iSize );

	std::sort( m_vTotalItemList.begin(), m_vTotalItemList.end(), MaterialCompoundItemInfoSort() );

	for( int i=0; i < iSize; ++i )
	{
		EXTRAITEMSLOT kTargetSlot;
		if( !pItem->GetExtraItem( m_vTotalItemList[i].m_iSlotIndex, kTargetSlot ) )
			continue;

		if( kTargetSlot.m_iReinforce >= g_CompoundMaterialMgr.GetMaxReinforce() )
			continue;

		NewShopPullDownItem kItem;
		kItem.m_iMagicCode = kTargetSlot.m_iIndex;

		char szName[MAX_PATH]="";
		if( kTargetSlot.m_iTradeState == EET_ENABLE )
		{
			SafeSprintf( szName, sizeof( szName ), STR(1), kTargetSlot.m_Name.c_str(), kTargetSlot.m_iReinforce );
			kItem.m_dwTextColor = TCT_DEFAULT_RED;
		}
		else if( kTargetSlot.m_iTradeState == EET_NORMAL )
		{
			SafeSprintf( szName, sizeof( szName ), STR(2), kTargetSlot.m_Name.c_str(), kTargetSlot.m_iReinforce );
			kItem.m_dwTextColor = TCT_DEFAULT_RED;
		}
		else
		{
			StringCbPrintf_e( szName, sizeof( szName ), "%s +%d", kTargetSlot.m_Name.c_str(), kTargetSlot.m_iReinforce );
			kItem.m_dwTextColor = TCT_DEFAULT_DARKGRAY;
		}

		kItem.m_szName = szName;
		rkList.push_back( kItem );
	}
}

void ItemMaterialCompoundWnd::GetTargetSeletInfo( ExtraItemList &rkList )
{
	rkList.clear();

	ioUserExtraItem *pItem = g_MyInfo.GetUserExtraItem();
	if( !pItem )
		return;

	int iSize = m_vTotalItemList.size();
	rkList.reserve( iSize );

	std::sort( m_vTotalItemList.begin(), m_vTotalItemList.end(), MaterialCompoundItemInfoSort() );

	for( int i=0; i < iSize; ++i )
	{
		EXTRAITEMSLOT kTargetSlot;
		if( !pItem->GetExtraItem( m_vTotalItemList[i].m_iSlotIndex, kTargetSlot ) )
			continue;

		if( kTargetSlot.m_iReinforce >= g_CompoundMaterialMgr.GetMaxReinforce() )
			continue;

		rkList.push_back( kTargetSlot );
	}
}

void ItemMaterialCompoundWnd::TargetListBtnUp( ioWnd *pOpenBtn, int iListArray )
{
	if( !pOpenBtn ) 
		return;	

	vNewShopPullDownItem kItemList;
	GetTargetList( kItemList );	

	if( !COMPARE( iListArray, 0, (int)kItemList.size() ) ) 
		return;			
		
	NewShopPullDownItem &rkItem = kItemList[iListArray];

	int iSlot = rkItem.m_iMagicCode;
	SelectExtraItemBtn *pTarget = (SelectExtraItemBtn*)(FindChildWnd(ID_TARGETITEM_BTN));
	if( pTarget && iSlot > 0 )
		pTarget->SetSelectExtraItemSlot( iSlot );
}

bool ItemMaterialCompoundWnd::SlotReload()
{
	SelectExtraItemBtn *pTarget = (SelectExtraItemBtn*)(FindChildWnd(ID_TARGETITEM_BTN));
	if( !pTarget )
		return false;

	vNewShopPullDownItem kItemList;
	GetTargetList( kItemList );	

	vNewShopPullDownItem::iterator iter = kItemList.begin();
	for( ; iter != kItemList.end(); ++iter )
	{
		const NewShopPullDownItem& rkNewShopItem = *iter;
		if( rkNewShopItem.m_iMagicCode == m_nReUseSlot )
		{			
			pTarget->SetSelectExtraItemSlot( m_nReUseSlot );
			return true;
		}
	}

	return false;
}

void ItemMaterialCompoundWnd::UpdateItemData()
{
	SelectExtraItemBtn *pTarget = (SelectExtraItemBtn*)(FindChildWnd(ID_TARGETITEM_BTN));
	if( !pTarget ) 
		return;
	
	int nTargetSlot = pTarget->GetSelectExtraItemSlot();

	ioUserExtraItem *pExtraItem = g_MyInfo.GetUserExtraItem();
	if( pExtraItem )
	{
		EXTRAITEMSLOT kSlot;

		//강화 장비 설정
		if( pExtraItem->GetExtraItem( nTargetSlot, kSlot ) )
		{
			InventoryIconBtn *pIconBtn = (InventoryIconBtn*)(FindChildWnd(ID_TARGET_ICON));
			if( pIconBtn )
			{
				ioUIRenderImage *pImg = NULL;
				pImg = g_UIImageSetMgr.CreateImageByFullName( kSlot.m_IconName );
				pIconBtn->SetIcon( pImg, FLOAT1, FLOAT1, kSlot.m_iReinforce );
				pIconBtn->SetMarkType( INVENTORY_MARK_TYPE_NONE );
				pIconBtn->SetActive();

				const ioItem *pItem = g_ItemMaker.GetItemConst( kSlot.m_iItemCode, __FUNCTION__ );
				if( pItem )
					pIconBtn->SetGradeType( pItem->GetGradeType() );
			}

			//현재 강화 레벨 설정
			m_nCurReinforce = kSlot.m_iReinforce;		
			
			//필요 재료 수량 확인
			m_bIsExtraItem = g_CompoundMaterialMgr.IsExtraItem( kSlot.m_iItemCode );	
			m_nNeedMaterialCnt = g_CompoundMaterialMgr.GetNeedMaterialCount( m_nCurReinforce, m_dwEtcItemCode, m_bIsExtraItem );
						
			//선택 완료
			m_bIsSelectItem = true;

			//재료 리셋
			m_bIsSelectMaterial = false;
			m_nSelectMaterialCode = 0;

			//실패 경험치 얻기
			m_nMaxFailExp = g_CompoundMaterialMgr.GetMaxFailExp( m_nCurReinforce, m_dwEtcItemCode, m_bIsExtraItem );
			m_nCurFailExp = (int)kSlot.m_failExp;

			if ( m_nMaxFailExp > 0 )
			{
				m_fFailExpRate = ( (float)m_nCurFailExp / m_nMaxFailExp );
				ShowChildWnd( ID_FAIL_MANUAL_BTN );
			}
			else
				HideChildWnd( ID_FAIL_MANUAL_BTN );

			m_fFailExpRate = min( FLOAT1, m_fFailExpRate );

			m_nNeedMaterialCode = g_CompoundMaterialMgr.NeedMaterialCode( kSlot.m_iItemCode, kSlot.m_iReinforce );

			//재료 초기화
			SelectMaterialItemBtn *pMaterialItemIcon = (SelectMaterialItemBtn*)(FindChildWnd(ID_MATERIALITEM_ICON));
			if( pMaterialItemIcon )
				pMaterialItemIcon->SetRecommand( m_nNeedMaterialCode, m_nNeedMaterialCnt );

			SelectExtraItemBtn *pMaterialItemBtn = (SelectExtraItemBtn*)(FindChildWnd(ID_MATERIALITEM_BTN));
			if( pMaterialItemBtn )
			{
				pMaterialItemBtn->SetSelectMaterial( 0, "" );
				pMaterialItemBtn->SetCurTitleColor( TCT_DEFAULT_DARKGRAY );
			}
			
			ioAlchemicInventory *pInven = g_MyInfo.GetAlchemicInventory();
			if( !pInven )
				return;

			//재료 버튼 잠금해제
			vAlchemicItemInfoList vMaterialList;
			if ( !pInven->GetAlchemicItemInfo( vMaterialList ) )
			{
				SetChildInActive( ID_MATERIALITEM_BTN );
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
				return;
			}
			else
				SetChildActive( ID_MATERIALITEM_BTN );

			//경고 문구 출력
			int nFixedReinforce = g_CompoundMaterialMgr.GetFixedFailReinforce();
			if ( m_nCurReinforce >= nFixedReinforce )
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2), nFixedReinforce, nFixedReinforce );
		}
	}
}

void ItemMaterialCompoundWnd::SendItemCompound()
{
	SelectExtraItemBtn *pTarget = (SelectExtraItemBtn*)(FindChildWnd(ID_TARGETITEM_BTN));
	if( !pTarget ) 
		return;
	
	bool bTarget;
	bTarget = false;

	int nTargetSlot = pTarget->GetSelectExtraItemSlot();
	if ( nTargetSlot <= 0 )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return;
	}

	if ( m_nSelectMaterialCode <= 0 )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return;
	}

	ioUserExtraItem *pExtraItem = g_MyInfo.GetUserExtraItem();
	if( pExtraItem )
	{
		EXTRAITEMSLOT kSlot;

		// Target Check
		if( pExtraItem->GetExtraItem( nTargetSlot, kSlot ) )
			bTarget = true;
	}

	if( !bTarget )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		return;
	}

	m_nReUseSlot = nTargetSlot;

	SP2Packet kPacket( CTPK_ETCITEM_USE );
	kPacket << (int)m_dwEtcItemCode;
	kPacket << nTargetSlot;
	kPacket << m_nSelectMaterialCode;
	TCPNetwork::SendToServer( kPacket );

	TCPNetwork::MouseBusy( true );

	HideWnd();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//재료 리스트 얻기
void ItemMaterialCompoundWnd::GetMaterialList( vNewShopPullDownItem &rkList )
{
	rkList.clear();

	ioAlchemicInventory *pInven = g_MyInfo.GetAlchemicInventory();
	if( !pInven )
		return;

	vAlchemicItemInfoList vMaterialList;
	if ( !pInven->GetAlchemicItemInfo( vMaterialList ) )
		return;
	
	//추천 재료 코드 설정
	SelectExtraItemBtn *pTarget = (SelectExtraItemBtn*)(FindChildWnd(ID_TARGETITEM_BTN));
	if( !pTarget ) 
		return;

	//추천 조각 얻기
	int nTargetSlot = pTarget->GetSelectExtraItemSlot();
	int nTargetReinforce = 0;
	ioUserExtraItem *pExtraItem = g_MyInfo.GetUserExtraItem();
	if( pExtraItem )
	{
		EXTRAITEMSLOT kSlot;
		if( pExtraItem->GetExtraItem( nTargetSlot, kSlot ) )
		{
			m_nNeedMaterialCode = g_CompoundMaterialMgr.NeedMaterialCode( kSlot.m_iItemCode, kSlot.m_iReinforce );
			nTargetReinforce = kSlot.m_iReinforce;
		}
	}

	//선 정렬
	std::sort( vMaterialList.begin(), vMaterialList.end(), MaterialItemSort() );

	vAlchemicItemInfoList vSortMaterialList;
	vSortMaterialList.clear();
	
	//추천조각 정렬
	vAlchemicItemInfoList::iterator iter = vMaterialList.begin();
	for ( ; iter!=vMaterialList.end(); ++iter )
	{
		//추천 조각 설정
		const AlchemicItemInfo& sInfo = *iter;
		if ( sInfo.m_iCode == m_nNeedMaterialCode )
		{
			vSortMaterialList.push_back( sInfo );
			vMaterialList.erase( iter );
			break;
		}
	}

	iter = vMaterialList.begin();
	for ( ; iter!=vMaterialList.end(); ++iter )
	{
		//추천 조각 설정
		const AlchemicItemInfo& sInfo = *iter;
		if ( sInfo.m_iCode > ALCHEMIC_ADDITIVE_DIV )
		{
			vSortMaterialList.push_back( sInfo );
			vMaterialList.erase( iter );
			break;
		}
	}

	//재료 리스트 추가
	iter = vMaterialList.begin();
	for ( ; iter!=vMaterialList.end(); ++iter )
	{
		vSortMaterialList.push_back( *iter );
	}
	vMaterialList.clear();

	//출력 리스트에 추가
	int nSize = vSortMaterialList.size();
	rkList.reserve( nSize );

	char szName[MAX_PATH]="", szCnt[MAX_PATH]="";	
	for( int i=0; i < nSize; ++i )
	{
		NewShopPullDownItem kItem;	

		int nMaterialCode = vSortMaterialList[i].m_iCode;
		int nMaterialCount = vSortMaterialList[i].m_iCount;

		if ( nMaterialCode == 100002 || nMaterialCode == 100003 || 
			 nMaterialCode == 100004 || nMaterialCode == 100005 )
			continue;

		if ( m_nNeedMaterialCnt > nMaterialCount )
			continue;

		if ( nTargetReinforce >= g_CompoundMaterialMgr.GetFixedFailReinforce() )
		{
			if ( nMaterialCode != m_nNeedMaterialCode )
				continue;
		}

		if( nMaterialCode > ALCHEMIC_ADDITIVE_DIV )
			StringCbPrintf( szName, sizeof( szName ), "%s", g_AlchemicMgr.GetAlchemicItemName( nMaterialCode ).c_str() );
		else
			SafeSprintf( szName, sizeof( szName ), STR(1), g_MyInfo.GetClassName( nMaterialCode ) );

		SafeSprintf( szCnt, sizeof( szCnt ), STR(2), nMaterialCount );

		//색설정
		if ( nMaterialCode == m_nNeedMaterialCode )
		{
			kItem.m_dwTextColor = TCT_DEFAULT_BLUE;
			kItem.m_dwCntTextColor = TCT_DEFAULT_BLUE;
		}
		else if ( nMaterialCode > ALCHEMIC_ADDITIVE_DIV )
		{
			kItem.m_dwTextColor = TCT_DEFAULT_GREEN;
			kItem.m_dwCntTextColor = TCT_DEFAULT_GREEN;
		}
		else
		{
			kItem.m_dwTextColor = TCT_DEFAULT_DARKGRAY;
			kItem.m_dwCntTextColor = TCT_DEFAULT_DARKGRAY;
		}

		kItem.m_szName      = szName;
		kItem.m_iMagicCode	= nMaterialCode;
		kItem.m_bCount = true;
		kItem.m_szCnt = szCnt;
		rkList.push_back( kItem );
	}
}

void ItemMaterialCompoundWnd::MaterialListBtnUp( ioWnd *pOpenBtn, int iListArray )
{
	if( !pOpenBtn ) 
		return;	

	vNewShopPullDownItem kItemList;
	GetMaterialList( kItemList );	

	if( !COMPARE( iListArray, 0, (int)kItemList.size() ) ) 
		return;			

	ioUITitle *pTitle = pOpenBtn->GetTitle();
	if( !pTitle )
		return;

	ioHashString szTitle;
	pTitle->GetFirstText( szTitle );

	NewShopPullDownItem &rkItem = kItemList[iListArray];
	if( szTitle == rkItem.m_szName ) 
		return;

	int nSelectMaterialCode = rkItem.m_iMagicCode;
	SetMaterialCode( nSelectMaterialCode );

	SelectExtraItemBtn *pTarget = (SelectExtraItemBtn*)(FindChildWnd(ID_MATERIALITEM_BTN));
	if( pTarget )
	{
		pTarget->SetSelectMaterial( nSelectMaterialCode, rkItem.m_szName );
		pTarget->SetCurTitleColor( rkItem.m_dwTextColor );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ItemMaterialCompoundWnd::ApplySuccessRate()
{	
	float fSuccessRate = 0.0f;

	//최대 실패경험치가 존재할 때
	if ( m_nMaxFailExp > 0 )
	{
		//성공율 얻기
		fSuccessRate = g_CompoundMaterialMgr.GetSuccessConst( m_dwEtcItemCode );
		fSuccessRate = ((float)( m_nCurFailExp + fSuccessRate ) / m_nMaxFailExp * FLOAT100 );
	}
	else if ( m_nMaxFailExp == 0 )
		fSuccessRate = g_CompoundMaterialMgr.GetReinforceSuccessRate( m_nCurReinforce, m_dwEtcItemCode );

	fSuccessRate += g_CompoundMaterialMgr.GetPCRoomBonusReinforceSuccessRate( m_nCurReinforce, m_dwEtcItemCode );

	m_fSuccessRate = fSuccessRate;

	m_fSuccessRate = min( FLOAT100, m_fSuccessRate );
	m_fSuccessRate = max( 0.0f, m_fSuccessRate );
}

bool ItemMaterialCompoundWnd::GetCurAlchemicItemInfo( OUT AlchemicToolTipInfo &rkInfo, IN DWORD dwItemInfoID )
{
	if( COMPARE(dwItemInfoID, ID_MATERIALITEM_ICON, ID_MATERIALITEM_ICON+1) )
	{
		SelectMaterialItemBtn *pItem = (SelectMaterialItemBtn*)(FindChildWnd(dwItemInfoID));
		if( pItem )
		{
			int nCode = pItem->GetCode();
			if( nCode <= 0 )
				return false;

			rkInfo.m_iCode = nCode;
			rkInfo.m_iType = AlchemicItemToolTip::ATT_ALCHEMIC_ITEM;

			return true;
		}
	}
	return false;
}