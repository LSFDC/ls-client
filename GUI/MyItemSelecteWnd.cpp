#include "StdAfx.h"
#include "MyItemSelecteWnd.h"
#include "../ioPowerUpManager.h"

//////////////////////////////////////////////////////////////////////////
MyItemSelectRadioButton::MyItemSelectRadioButton()
{
	m_iTabType = 0;
}

MyItemSelectRadioButton::~MyItemSelectRadioButton()
{

}

void MyItemSelectRadioButton::ParseExtraInfo( ioXMLElement &xElement )
{
	ioRadioButton::ParseExtraInfo( xElement );

	m_iTabType = xElement.GetIntAttribute_e( "TabType" );
}

//////////////////////////////////////////////////////////////////////////

ItemSelectInfoWnd::ItemSelectInfoWnd()
{
	m_pEmptyImg = NULL;
}

ItemSelectInfoWnd::~ItemSelectInfoWnd()
{
	SAFEDELETE(m_pEmptyImg);
}

void ItemSelectInfoWnd::ClearDesc()
{
	for (int i = 0; i < MAX_DESC ; i++)
	{
		m_szDescArray[i].ClearList();
	}
}

void ItemSelectInfoWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "empty_image" )
	{
		SAFEDELETE( m_pEmptyImg );
		m_pEmptyImg = pImage;
	}
	else
	{
		ioButton::AddRenderImage( szType, pImage );
	}
}

void ItemSelectInfoWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();

	switch( dwID )
	{
	case ID_SELECT_BTN:
		if( cmd == IOBN_BTNUP )
		{
			GetParent()->iwm_command( this, IOBN_BTNUP, m_ItemInfo.m_iIndex );
		}
		break;
	}
}

void ItemSelectInfoWnd::SetBlank()
{
	SetInActive();

	m_ItemInfo.Init();

	HideChildWnd( ID_ICON_BTN );
	HideChildWnd( ID_SELECT_BTN );

	ClearDesc();
}

void ItemSelectInfoWnd::SetItemInfo( const EXTRAITEMSLOT& rkItem )
{
	SetBlank();
	SetActive();

	m_ItemInfo.Init();
	m_ItemInfo = rkItem;

	if( !SetInfo() )
	{
		SetBlank();
		return;
	}
}

bool ItemSelectInfoWnd::SetInfo()
{
	if( m_ItemInfo.m_iItemCode == 0 )
		return false;

	// icon btn
	ShowChildWnd( ID_ICON_BTN );

	InventoryIconBtn *pBtn = dynamic_cast< InventoryIconBtn* > ( FindChildWnd( ID_ICON_BTN ) );
	if( !pBtn )
	{
		LOG.PrintTimeAndLog( 0, "%s pBtn == NULL.", __FUNCTION__ );
		return false;
	}

	ShowChildWnd( ID_SELECT_BTN );

	ioUIRenderImage *pImg = NULL;
	ioHashString szIcon = m_ItemInfo.m_IconName;
	if( !szIcon.IsEmpty() )
		pImg = g_UIImageSetMgr.CreateImageByFullName( szIcon );

	pBtn->SetIcon( pImg, 1.0f, 1.0f, m_ItemInfo.m_iReinforce );

	if( m_ItemInfo.m_iTradeState == EET_NORMAL && m_ItemInfo.m_PeriodType == EPT_MORTMAIN )
		pBtn->SetMarkType( INVENTORY_MARK_TYPE_TRADE_WAIT );
	else if( m_ItemInfo.m_iTradeState == EET_ENABLE && m_ItemInfo.m_PeriodType == EPT_MORTMAIN )
		pBtn->SetMarkType( INVENTORY_MARK_TYPE_TRADE_ENABLE );
	else if( m_ItemInfo.m_PeriodType == EPT_GROW_TIME )
		pBtn->SetExtraItemGrowthCatalyst( true );
	else
		pBtn->SetMarkType( INVENTORY_MARK_TYPE_NONE );

	// desc
	ClearDesc();

	const ioItem *pItem = g_ItemMaker.GetItemConst( m_ItemInfo.m_iItemCode, __FUNCTION__ );
	if( !pItem )
		return false;

	pBtn->SetGradeType( pItem->GetGradeType() );

	char szBuf[MAX_PATH], szText[MAX_PATH];
	memset( szText, 0, sizeof(szText) );
	
	int iGradType;
	int iEquipSlotIndex = m_ItemInfo.m_iItemCode / DEFAULT_BASIC_ITEM_CODE;
	if ( iEquipSlotIndex == ES_WEAPON )
	{
		iGradType = ConvertPowerUpItemToItemGrade( m_ItemInfo.m_iItemCode );
		if ( !IsPowerUpItemGrade( iGradType ) )
			SafeSprintf( szText, sizeof( szText ), STR(1) );
		else 
		{
			ioHashString szGradeTitleName = g_PowerUpManager.GetItemGradeTitle( m_ItemInfo.m_iItemCode, iGradType );
			if( !szGradeTitleName.IsEmpty() )
				SafeSprintf( szText, sizeof( szText ), STR(2), szGradeTitleName.c_str() );
		}
	}
	else
	{
		iGradType = ConvertPowerUpRareItemToRareItemGrade( m_ItemInfo.m_iItemCode );
		if ( !IsPowerUpRareItemGrade( iGradType ) )
			SafeSprintf( szText, sizeof( szText ), STR(1) );
		else
		{
			ioHashString szGradeTitleName = g_PowerUpManager.GetExtraItemGradeTitle( m_ItemInfo.m_iItemCode, iGradType );
			if( !szGradeTitleName.IsEmpty() )
				SafeSprintf( szText, sizeof( szText ), STR(2), szGradeTitleName.c_str() );
		}
	}

	m_szDescArray[0].SetTextStyle( TS_NORMAL );
	m_szDescArray[0].SetBkColor( 0, 0, 0 );	
	m_szDescArray[0].SetTextColor( TCT_DEFAULT_DARKGRAY ); // 진회
	m_szDescArray[0].AddTextPiece( FONT_SIZE_13, STR(4), m_ItemInfo.m_Name.c_str() );

	m_szDescArray[1].ClearList();
	m_szDescArray[1].SetTextStyle( TS_NORMAL );
	m_szDescArray[1].SetBkColor( 0, 0, 0 );	
	m_szDescArray[1].SetTextColor( TCT_DEFAULT_BLUE );
	m_szDescArray[1].AddTextPiece( FONT_SIZE_13, STR(4), szText );

	int iSlot = m_ItemInfo.m_iItemCode / DEFAULT_BASIC_ITEM_CODE;
	int iEnableClass = pItem->GetEnableClass();
	if ( iSlot == ES_WEAPON && iEnableClass > 0 )
	{
		SafeSprintf( szBuf, sizeof( szBuf ), STR(3), g_MyInfo.GetClassName( iEnableClass, ConvertPowerUpItemToChar( iGradType ) ) );
		m_szDescArray[2].SetTextStyle( TS_NORMAL );
		m_szDescArray[2].SetBkColor( 0, 0, 0 );	
		m_szDescArray[2].SetTextColor( TCT_DEFAULT_GRAY );
		m_szDescArray[2].AddTextPiece( FONT_SIZE_11, STR(4), szBuf );
	}

	return true;
}

void ItemSelectInfoWnd::RenderDesc( int iXPos, int iYPos )
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

		m_szDescArray[2].SetTextStyle( TS_NORMAL );
		m_szDescArray[2].SetBkColor( 0, 0, 0 );	
		m_szDescArray[2].SetTextColor( TCT_DEFAULT_GRAY );
		m_szDescArray[2].AddTextPiece( FONT_SIZE_11, "" );
		m_szDescArray[2].PrintFullTextWidthCut( iXPos, iYPos+Y_LINE_GAP, TAT_CENTER, INVENTORY_ITEM_INFO_WIDTHCUT_SIZE );
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

void ItemSelectInfoWnd::OnRender()
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
		ioButton::OnRender();

	RenderDesc( iXPos, iYPos );
}

//////////////////////////////////////////////////////////////////////////
MyItemSelecteWnd::MyItemSelecteWnd(void)
{
	m_dwCurTabID = ID_TAB_1_BTN;

	m_pEditFrm   = NULL;
	m_vAllItemList.clear();
	m_pPreSelectedWnd = NULL;
}

MyItemSelecteWnd::~MyItemSelecteWnd(void)
{
	SAFEDELETE( m_pEditFrm );
	m_pPreSelectedWnd = NULL;
	m_vAllItemList.clear();
}

void MyItemSelecteWnd::ClearAll()
{

}

void MyItemSelecteWnd::AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame )
{
	if( szType == "EditFrm" )
	{
		SAFEDELETE( m_pEditFrm );
		m_pEditFrm = pFrame;
	}
	else
	{
		ioWnd::AddRenderFrame( szType, pFrame );
	}
}

void MyItemSelecteWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
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
	case ID_TAB_1_BTN:
	case ID_TAB_2_BTN:
	case ID_TAB_3_BTN:
	case ID_TAB_4_BTN:
	case ID_TAB_5_BTN:
		if( cmd == IOBN_BTNDOWN )
		{
			ChangeTab( dwID, "" );
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
			if ( m_pPreSelectedWnd && param != 0 )
			{
				m_pPreSelectedWnd->iwm_command( this, IOBN_BTNUP, param );
				HideWnd();
			}
		}
		break;
	case ID_FIND_BTN:
		if( cmd == IOBN_BTNUP )
		{
			SetFind();
		}
		break;
	}
}

void MyItemSelecteWnd::ShowItemSeletWnd( ExtraItemList vExtraitemList, ioWnd* pPreSelectedWnd )
{
	m_vAllItemList = vExtraitemList;
	m_vCurItemList.clear();
	m_szCurrFindKeyword.Clear();

	ChangeTab( ID_TAB_1_BTN, "" );
	ioEdit *pEdit = dynamic_cast<ioEdit*> ( FindChildWnd( ID_FIND_EDIT ) );
	if( pEdit )
	{
		pEdit->ClearString();
		pEdit->KillKeyFocus();
	}

	m_pPreSelectedWnd = NULL;
	if ( pPreSelectedWnd != NULL )
		m_pPreSelectedWnd = pPreSelectedWnd;

	ShowWnd();
}

void MyItemSelecteWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() )
		return;

	ioEdit* pEdit = dynamic_cast<ioEdit*>( FindChildWnd( ID_FIND_EDIT ) );
	if( pEdit && pEdit->IsKeyFocus() )
	{
		if( g_InputBox.IsEnter() )
		{
			iwm_command( FindChildWnd( ID_FIND_BTN ), IOBN_BTNUP, 0 );
		}

		else if( g_InputBox.IsKeyDown() || g_InputBox.IsBackSpace() )
		{
			m_szCurrFindKeyword = g_InputBox.GetString();
			iwm_command( FindChildWnd( ID_FIND_BTN ), IOBN_BTNUP, 0 );
		}
	}
}

void MyItemSelecteWnd::ChangeTab( DWORD dwID, const char *szFind )
{
	if( strcmp( szFind, "" ) == 0 )
	{
		CheckRadioButton( ID_TAB_1_BTN, ID_TAB_5_BTN, dwID );
		m_dwCurTabID = dwID;
	}
	else
	{
		// 검색이 있으면 모든 탭 선택되지 않은 상태로 변경
		for (int i = ID_TAB_1_BTN; i < ID_TAB_5_BTN+1 ; i++)
		{
			ioRadioButton *pButton = dynamic_cast<ioRadioButton*> ( FindChildWnd( i ) );
			if( !pButton )
				continue;

			pButton->SetClick( false );
		}
	}

	SetCurItemInfoVec( m_dwCurTabID, szFind );

	m_iCurPage = 0;

	ioScroll *pScroll = (ioScroll*)FindChildWnd( ID_VERT_SCROLL );
	if( pScroll )
	{
		int iItemSize = 0;
		iItemSize = m_vCurItemList.size();
		int iMaxPos =  iItemSize / PAGE_PER_ITEM_NUM;
		if( iItemSize % PAGE_PER_ITEM_NUM )
			iMaxPos++;

		pScroll->SetPageRange(2);
		pScroll->SetScrollRange( 0, iMaxPos );
		pScroll->SetScrollPos( m_iCurPage );
	}
}

void MyItemSelecteWnd::SetCurItemInfoVec( DWORD dwTabID, const char *szFind )
{
	if( m_vAllItemList.empty() )
	{
		return;
	}

	ExtraItemList vTotalList = m_vAllItemList;
	m_vCurItemList.clear();

	bool bFind = false;
	if( strcmp( szFind, "" ) != 0 )
		bFind = true;

	int iSize = vTotalList.size();
	for( int i = 0; i < iSize ; i++ )
	{
		const EXTRAITEMSLOT& rkInfo = vTotalList[i];
		if( bFind )
		{
			ioHashString szItemName = rkInfo.m_Name;
			if( strstr( szItemName.c_str(), szFind ) == NULL )
				continue;
		}
		else
		{
			int iSlot = rkInfo.m_iItemCode / DEFAULT_BASIC_ITEM_CODE;
			if( !IsRightTab( dwTabID, iSlot ) )
				continue;
		}

		m_vCurItemList.push_back( vTotalList[i] );
	}

	if( m_vCurItemList.empty() )
		return;
}

bool MyItemSelecteWnd::IsRightTab( DWORD dwTabID, int iType )
{
	MyItemSelectRadioButton* pButton = dynamic_cast<MyItemSelectRadioButton*>( FindChildWnd( dwTabID ) );
	if( pButton )
	{
		if ( pButton->GetTabType() == -1 )
			return true;
		else if ( pButton->GetTabType() == iType )
			return true;
	}
	return false;
}

void MyItemSelecteWnd::OnRender()
{
	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	ioWnd::OnRender();

	enum
	{
		X_OFFSET = 17,
		Y_OFFSET = 474,
	};

	if( m_pEditFrm )
		m_pEditFrm->Render( iXPos+X_OFFSET, iYPos+Y_OFFSET );
}

void MyItemSelecteWnd::SetFind()
{
	ioEdit *pEdit = dynamic_cast<ioEdit*> ( FindChildWnd( ID_FIND_EDIT ) );
	if( !pEdit )
	{
		LOG.PrintTimeAndLog( 0, "%s pEdit == NULL.", __FUNCTION__ );
		return;
	}

	ChangeTab( m_dwCurTabID, m_szCurrFindKeyword.c_str() );
}

void MyItemSelecteWnd::UpdatePage( int iNewPage )
{
	for (int i = 0; i < MAX_ITEM_INFO_WND_BTN ; i++)
		HideChildWnd( ID_ITEM_INFO_WND_1_BTN + i );

	m_iCurPage = iNewPage;
	int iItemSize = m_vCurItemList.size();
	int iStartArray = m_iCurPage * PAGE_PER_ITEM_NUM;

	for( int i = 0 ; i < MAX_ITEM_INFO_WND_BTN ; i++ )
	{
		ItemSelectInfoWnd *pItem = dynamic_cast<ItemSelectInfoWnd*>( FindChildWnd(ID_ITEM_INFO_WND_1_BTN + i) );
		if( !pItem )
			continue;

		pItem->ShowWnd();

		int iCurArray = iStartArray + i;

		if( !COMPARE( iCurArray, 0, iItemSize ) )
		{
			// 초기화
			pItem->SetBlank();
			continue;
		}

		pItem->SetItemInfo( m_vCurItemList[ iCurArray ]);
	}
}

void MyItemSelecteWnd::iwm_vscroll( DWORD dwID, int curPos )
{
	UpdatePage( curPos );
}

void MyItemSelecteWnd::iwm_wheel( int zDelta )
{
	if( !IsShow() )
	{
		ioWnd::iwm_wheel( zDelta );
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