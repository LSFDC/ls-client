
#include "stdafx.h"

#include "../io3DEngine/ioINILoader.h"
#include "../io3DEngine/ioWnd.h"
#include "../io3DEngine/ioScroll.h"
#include "../io3DEngine/ioXMLDocument.h"
#include "../io3DEngine/ioXMLElement.h"
#include "../io3DEngine/ioFontManager.h"
#include "../io3DEngine/ioUIRenderImage.h"
#include "../io3DEngine/ioUIImageSetManager.h"
#include "../io3DEngine/ioSoundManager.h"
#include "../io3DEngine/iostringmanager.h"
#include "../io3DEngine/Safesprintf.h"

#include "../ioMyInfo.h"
#include "../ioApplication.h"
#include "../NetworkWrappingFunc.h"
#include "../ioCreateChar.h"
#include "../TextColorTable.h"
#include "../ioItemMaker.h"
#include "../ioClassPrice.h"
#include "../ioSetItemInfo.h"
#include "../ioSetItemInfoManager.h"
#include "../ioDecorationPrice.h"
#include "../EtcHelpFunc.h"
#include "../ioEtcItem.h"
#include "../ioEtcItemManager.h"
#include "../WndID.h"

#include "ioUI3DRender.h"
#include "ioSP2GUIManager.h"

#include "GlobalWnd.h"
#include "ManualWnd.h"
#include "MyInventoryWnd.h"

#include "SoldierPackageWnd.h"

#include <strsafe.h>


//////////////////////////////////////////////////////////////////////////
PackagePullDownWnd::PackagePullDownWnd()
{
	m_iStartHGap  = 0;
	m_iEndHGap    = 0;
	m_iItemHSize  = 0;
	m_iItemMaxPage= 0;
	m_iCurPage    = 0;
	m_pOpenBtn    = NULL;
	m_pScrollWnd  = NULL;
	m_pDarkLine   = NULL;
	m_pLightLine  = NULL;
	m_pOverLine   = NULL;
	m_fFontSize   = FONT_SIZE_12;
}

PackagePullDownWnd::~PackagePullDownWnd()
{
	SAFEDELETE( m_pDarkLine );
	SAFEDELETE( m_pLightLine );
	SAFEDELETE( m_pOverLine );
	m_PullDownItem.clear();
}

void PackagePullDownWnd::CreatePullDown( ioWnd *pOpenBtn, int iXPos, int iYPos )
{
	m_pOpenBtn      = pOpenBtn;
	SetWndPos( iXPos, iYPos );

	m_PullDownItem.clear();
}

void PackagePullDownWnd::InsertItem( NewShopPullDownItem &rkItem )
{	
	m_PullDownItem.push_back( rkItem );	
}

void PackagePullDownWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	m_iStartHGap   = xElement.GetIntAttribute_e( "StartHGap" );
	m_iEndHGap     = xElement.GetIntAttribute_e( "EndHGap" );
	m_iItemHSize   = xElement.GetIntAttribute_e( "ItemHSize" );
	m_iItemMaxPage = xElement.GetIntAttribute_e( "ItemMaxPage" );
	m_fFontSize    = xElement.GetFloatAttribute_e( "FontSize" );
}

void PackagePullDownWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "DarkLine" )
	{
		SAFEDELETE( m_pDarkLine );
		m_pDarkLine = pImage;
	}
	else if( szType == "LightLine" )
	{
		SAFEDELETE( m_pLightLine );
		m_pLightLine = pImage;
	}
	else if( szType == "OverLine" )
	{
		SAFEDELETE( m_pOverLine );
		m_pOverLine = pImage;
	}
	else
	{
		ioWnd::AddRenderImage( szType, pImage );
	}
}

void PackagePullDownWnd::iwm_show()
{
	if( m_PullDownItem.empty() )
	{
		HideWnd();
		return;
	}

	m_pScrollWnd = FindChildWnd( ID_VERT_SCROLL );

	// ��ũ�� UI
	m_iCurPage = 0;
	int iItemSize = m_PullDownItem.size();
	if( iItemSize > m_iItemMaxPage )
	{
		ioScroll *pScroll = (ioScroll*)m_pScrollWnd;
		if( pScroll )
		{
			int iTotalList = (int)iItemSize - ( m_iItemMaxPage - 1 );
			pScroll->SetScrollRange( 0, iTotalList );
			pScroll->SetScrollPos( m_iCurPage );
			pScroll->ShowWnd();
		}
	}
	else if( m_pScrollWnd )
	{
		m_pScrollWnd->HideWnd();
	}

	// ������ ������
	SetSize( GetWidth(), m_iStartHGap + m_iEndHGap + ( m_iItemHSize * min( m_iItemMaxPage, iItemSize ) ) );
	SetCheckParentOutSide( true );
}

void PackagePullDownWnd::iwm_hide()
{
	SetCheckParentOutSide( false );
}

void PackagePullDownWnd::iwm_vscroll( DWORD dwID, int curPos )
{
	m_iCurPage = curPos;
}

void PackagePullDownWnd::iwm_wheel( int zDelta )
{
	if( !IsShow() )
		ioWnd::iwm_wheel( zDelta );
	else
	{ 
		ioScroll *pScroll = (ioScroll*)m_pScrollWnd;
		if( !pScroll ) return;
		if( !pScroll->IsShow() ) return;

		// �� �̺�Ʈ ó���� �ߴٸ� �θ�� �̺�Ʈ�� ������ �ʴ´�.
		if( zDelta == WHEEL_DELTA )
		{
			pScroll->SetScrollPos( m_iCurPage - 1 );
		}
		else if( zDelta == -WHEEL_DELTA )
		{
			pScroll->SetScrollPos( m_iCurPage + 1 );
		}
	}
}

bool PackagePullDownWnd::PrevClickCheck()
{
	ioWnd *pPreClick = g_GUIMgr.GetPreClickWnd();
	if( !pPreClick ) return false;
	if( pPreClick == m_pOpenBtn ) return false;
	if( pPreClick == this ) return false;
	if( pPreClick->GetParent() == m_pScrollWnd ) return false;
	if( pPreClick->GetParent() == this ) return false;

	return true;
}

void PackagePullDownWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;

	m_iOverListIndex = -1;
	// �ٸ� ������ Ŭ���� �Ͼ���� Ȯ��
	if( PrevClickCheck() )
	{
		HideWnd();
		return;
	}

	int iXPos, iYPos;
	iXPos = GetDerivedPosX();
	iYPos = GetDerivedPosY();

	ioLSMouse *pMouse = g_App.GetMouse();

	int iMaxItem = m_PullDownItem.size();
	int iStart   = m_iCurPage;
	for(int i = 0;i < m_iItemMaxPage;i++,iStart++)
	{
		if( iStart >= iMaxItem ) continue;

		int iWidth = GetWidth();
		if( iMaxItem > m_iItemMaxPage )
		{	
			iWidth -= 33;
		}
		else
		{	
			iWidth -= 8;
		}

		RECT rcNameRect;
		rcNameRect.left = iXPos + 4;
		rcNameRect.top  = iYPos + m_iStartHGap + i * m_iItemHSize;
		rcNameRect.right  = rcNameRect.left + iWidth;
		rcNameRect.bottom = rcNameRect.top + m_iItemHSize;

		if( ::PtInRect( &rcNameRect, pMouse->GetMousePos() ) )
		{
			m_iOverListIndex = iStart;

			if( pMouse->IsLBtnUp() )
			{
				GetParent()->iwm_command( this, IOBN_BTNUP, iStart );
			}
			else
			{
				GetParent()->iwm_command( this, IOWN_OVERED, iStart );
			}
			break;
		}
	}
}

void PackagePullDownWnd::OnRender()
{
	ioWnd::OnRender();	
	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	if( !m_pDarkLine || !m_pLightLine || !m_pOverLine ) 
		return;

	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetAlignType( TAT_LEFT );

	bool bDarkRender = false;
	int iMaxItem     = m_PullDownItem.size();
	int iStart       = m_iCurPage;
	bool bShowScroll = false;
	if( iMaxItem > m_iItemMaxPage )
		bShowScroll = true;

	for(int i = 0;i < m_iItemMaxPage;i++,iStart++)
	{
		if( iStart >= iMaxItem ) continue;

		NewShopPullDownItem &rkItem = m_PullDownItem[iStart];
		g_FontMgr.SetTextColor( rkItem.m_dwTextColor );

		bDarkRender = !bDarkRender;
		// ���� 
		if( m_iOverListIndex == iStart  )
		{
			if( bShowScroll )
				m_pOverLine->SetSize( GetWidth() - 33, m_iItemHSize );
			else
				m_pOverLine->SetSize( GetWidth() - 8, m_iItemHSize );

			m_pOverLine->Render( iXPos + 4, iYPos + m_iStartHGap + ( i * m_iItemHSize ) , UI_RENDER_MULTIPLY );
			g_FontMgr.SetTextColor( 255, 255, 255 );
		}
		else if( bDarkRender )
		{
			if( bShowScroll )
				m_pDarkLine->SetSize( GetWidth() - 33, m_iItemHSize );
			else
				m_pDarkLine->SetSize( GetWidth() - 8, m_iItemHSize );

			m_pDarkLine->Render( iXPos + 4, iYPos + m_iStartHGap + ( i * m_iItemHSize ) , UI_RENDER_MULTIPLY );
		}
		else
		{
			if( bShowScroll )
				m_pLightLine->SetSize( GetWidth() - 33, m_iItemHSize );
			else
				m_pLightLine->SetSize( GetWidth() - 8, m_iItemHSize );

			m_pLightLine->Render( iXPos + 4, iYPos + m_iStartHGap + ( i * m_iItemHSize ) , UI_RENDER_MULTIPLY );
		}

		// ������ �̸�
		int iNameWidth = 0;
		if( bShowScroll )
			iNameWidth = min( GetWidth() - 46, g_FontMgr.GetTextWidth( rkItem.m_szName.c_str(), TS_OUTLINE_FULL_2X, m_fFontSize ) + FLOAT1 );
		else
			iNameWidth = min( GetWidth() - 34, g_FontMgr.GetTextWidth( rkItem.m_szName.c_str(), TS_OUTLINE_FULL_2X, m_fFontSize ) + FLOAT1 );

		g_FontMgr.PrintTextWidthCut( iXPos + 17, iYPos + m_iStartHGap + ( i * m_iItemHSize ) + 2, m_fFontSize, iNameWidth, rkItem.m_szName.c_str() );
	}
}

bool PackagePullDownWnd::GetOverItem( OUT NewShopPullDownItem &rItem )
{
	if( COMPARE( m_iOverListIndex, 0, (int)m_PullDownItem.size() ) )
	{
		rItem = m_PullDownItem[m_iOverListIndex];
		return true;
	}

	return false;
}

//-------------------------------------------------------------------------

PackageItemBtn::PackageItemBtn()
{
	m_iKeepAndBasicItemBonus = 0;
}

PackageItemBtn::~PackageItemBtn()
{

}

void PackageItemBtn::SetPackageItem( int iSexType, PackageItem &rkItem, int iPackageType )
{
	m_PackageItem = rkItem;
	m_iKeepAndBasicItemBonus = 0;

	// ���� ������
	if( GetID() == PackageItemBuyWnd::ID_CHAR_SELECT_BTN )
	{
		if( iPackageType == (int)PackageItemBuyWnd::PACKAGE_SOLDIER || iPackageType == (int)PackageItemBuyWnd::PACKAGE_PREMIUM_SOLDIER )
		{
			if( g_MyInfo.GetExerciseCharArrayByClass( m_PackageItem.m_iMagicCode ) == -1 )
				m_iKeepAndBasicItemBonus = g_ClassPrice.GetTimeCharResellPeso( m_PackageItem.m_iMagicCode, g_MyInfo.GetCharTotalSecond( g_MyInfo.GetClassArray( m_PackageItem.m_iMagicCode ) ) );
		}
	}
	else
	{
		enum { MAX_DECO_ITEM = 6, };
		DWORD dwDecoTypeArray = GetID() - PackageItemBuyWnd::ID_KINDRED_SELECT_BTN;
		int iDecoTypeList[MAX_DECO_ITEM]={ UID_KINDRED, UID_HAIR, UID_HAIR_COLOR, UID_FACE, UID_SKIN_COLOR, UID_UNDERWEAR };
		if( COMPARE( dwDecoTypeArray, 0, MAX_DECO_ITEM ) )
		{
			if( iPackageType == PackageItemBuyWnd::PACKAGE_DECO_FACE       && iDecoTypeList[dwDecoTypeArray] != UID_FACE )
				return;
			if( iPackageType == PackageItemBuyWnd::PACKAGE_DECO_HAIR       && iDecoTypeList[dwDecoTypeArray] != UID_HAIR )
				return;
			if( iPackageType == PackageItemBuyWnd::PACKAGE_DECO_SKIN_COLOR && iDecoTypeList[dwDecoTypeArray] != UID_SKIN_COLOR )
				return;
			if( iPackageType == PackageItemBuyWnd::PACKAGE_DECO_HAIR_COLOR && iDecoTypeList[dwDecoTypeArray] != UID_HAIR_COLOR )
				return;
			if( iPackageType == PackageItemBuyWnd::PACKAGE_DECO_KINDRED    && iDecoTypeList[dwDecoTypeArray] != UID_KINDRED )
				return;
			if( iPackageType == PackageItemBuyWnd::PACKAGE_DECO_UNDERWEAR  && iDecoTypeList[dwDecoTypeArray] != UID_UNDERWEAR )
				return;

			if( m_PackageItem.m_bKeepItem || m_PackageItem.m_bBasicItem )
			{				
				m_iKeepAndBasicItemBonus = g_DecorationPrice.GetDecoPackageKeepPeso( iSexType, iDecoTypeList[dwDecoTypeArray] );
			}
		}
	}

	//
	m_Title.ClearList();

	char szConvertNum[MAX_PATH] = "";
	Help::ConvertNumToStrComma( m_iKeepAndBasicItemBonus, szConvertNum, sizeof( szConvertNum ) );

	char szName[MAX_PATH] = "";
	char szKeep[MAX_PATH] = "";

	m_Title.SetTextStyle( TS_NORMAL );
	m_Title.SetBkColor( TCT_DEFAULT_WHITE );

	int iNameWidth = 0;
	if( m_PackageItem.m_bKeepItem )   //�������� ������
	{
		m_Title.SetTextColor( TCT_DEFAULT_DARKGRAY );
		m_Title.AddTextPiece( FONT_SIZE_12, STR(1) );
		m_Title.SetTextColor( TCT_DEFAULT_BLUE );
		m_Title.AddTextPiece( FONT_SIZE_12, STR(2), szConvertNum );
	}
	else if( m_PackageItem.m_bBasicItem )
	{
		m_Title.SetTextColor( TCT_DEFAULT_DARKGRAY );
		m_Title.AddTextPiece( FONT_SIZE_12, STR(3) );
		m_Title.SetTextColor( TCT_DEFAULT_BLUE );
		m_Title.AddTextPiece( FONT_SIZE_12, STR(4), szConvertNum );
	}
	else
	{
		m_Title.SetTextColor( g_UIImageRenderMgr.GetGradeTextColor( m_PackageItem.m_nGradeType ) );
		m_Title.AddTextPiece( FONT_SIZE_12, STR(5), m_PackageItem.m_szName.c_str() );
	}
}

void PackageItemBtn::OnRender()
{
	ioButton::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	if( !m_Title.IsEmpty() )
		m_Title.PrintFullText( iXPos+102, iYPos+4, TAT_CENTER );

}

//////////////////////////////////////////////////////////////////////////
PackageItemBuyWnd::PackageItemBuyWnd()
{
	m_PackageStyle = PACKAGE_SOLDIER;
	m_dwEtcItemType = 0;

	m_CharInfo.m_class_type = 1;
	m_CharInfo.m_kindred	= 1;
	m_CharInfo.m_sex		= 1;
	m_CharInfo.m_beard	    = 1;
	m_CharInfo.m_face	    = 1;
	m_CharInfo.m_hair	    = 1;
	m_CharInfo.m_skin_color = 1;
	m_CharInfo.m_hair_color = 1;
	m_CharInfo.m_accessories= 1;
	m_CharInfo.m_underwear  = 1;
	m_OverCharInfo = m_CharInfo;

	m_pManual			= NULL;
	m_iActiveFilter		= 0;
}

PackageItemBuyWnd::~PackageItemBuyWnd()
{

}

void PackageItemBuyWnd::iwm_create()
{
	g_GUIMgr.AddWnd( "XML/PackageHelpWnd.xml", this );
}

void PackageItemBuyWnd::iwm_show()
{
	m_pManual = ioEtcInvenSubManualMgr::GetSingletonPtr();

	// ��ư ����
	vPackageItem kPackageItemList;
	// �뺴
	GetPackageItemList( ID_CHAR_SELECT_BTN, m_CharInfo.m_sex - 1, kPackageItemList );
	if( !kPackageItemList.empty() ) 
	{
		SetSelectItemBtn( ID_CHAR_SELECT_BTN, m_CharInfo, kPackageItemList[0] );
	}	
	// ���� + �Ӹ� + �Ӹ��� + �� + �Ǻλ� + �⺻���� ����Ʈ ����
	DefaultDecoration( true, true, m_CharInfo );

	NewShopHelpWnd *pCharWnd = dynamic_cast<NewShopHelpWnd *>( FindChildWnd( ID_SOLDIER_WND ));
	if( pCharWnd )
	{
		pCharWnd->SetZPosition( 600.0f );
		pCharWnd->SetChar( m_CharInfo );
		pCharWnd->SetEquip( true, false, true );
		pCharWnd->ShowWnd();
	}

	ShowChildWnd( ID_CHAR_SELECT_BTN, true );
}

void PackageItemBuyWnd::iwm_hide()
{
	HideChildWnd( ID_SOLDIER_WND );
}

int PackageItemBuyWnd::GetKeepAndBasicItemBonus()
{
	int iReturnPeso = 0;

	for(int i = ID_CHAR_SELECT_BTN;i < ID_UNDERWEAR_SELECT_BTN + 1;i++)
	{
		PackageItemBtn *pItemBtn = dynamic_cast<PackageItemBtn*>(FindChildWnd( i ));
		if( pItemBtn )
		{
			//
			iReturnPeso += pItemBtn->GetKeepAndBasicItemBonus();
		}
	}
	return iReturnPeso;
}

void PackageItemBuyWnd::ConvertItemList( IN vPackageItem &rkPackageList, OUT vNewShopPullDownItem &rkList )
{
	for(int i=0; i < (int)rkPackageList.size(); i++)
	{
		PackageItem &rkItem = rkPackageList[i];

		NewShopPullDownItem kItem;
		kItem.m_iMagicCode = rkItem.m_iMagicCode;

		char szName[MAX_PATH]="";

		if( rkItem.m_bKeepItem )
			SafeSprintf( szName, sizeof( szName ), STR(1), rkItem.m_szName.c_str() );
		else if( rkItem.m_bBasicItem )
			SafeSprintf( szName, sizeof( szName ), STR(2), rkItem.m_szName.c_str() );
		else
			SafeSprintf( szName, sizeof( szName ), STR(3), rkItem.m_szName.c_str() );

		kItem.m_dwTextColor = g_UIImageRenderMgr.GetGradeTextColor( rkItem.m_nGradeType );
		kItem.m_szName      = szName;
		rkList.push_back( kItem );
	}
}

void PackageItemBuyWnd::GetPackageItemList( DWORD dwID, int iSex, vPackageItem &rkList )
{
	switch( dwID )
	{
	case ID_CHAR_SELECT_BTN:
		GetClassList( rkList );
		break;
	case ID_KINDRED_SELECT_BTN:
		GetDecorationList( iSex, UID_KINDRED, rkList );
		break;
	case ID_HAIR_SELECT_BTN:
		GetDecorationList( iSex, UID_HAIR, rkList );
		break;
	case ID_HAIR_COLOR_SELECT_BTN:
		GetDecorationList( iSex, UID_HAIR_COLOR, rkList );
		break;
	case ID_FACE_SELECT_BTN:
		GetDecorationList( iSex, UID_FACE, rkList );
		break;
	case ID_SKIN_COLOR_SELECT_BTN:
		GetDecorationList( iSex, UID_SKIN_COLOR, rkList );
		break;
	case ID_UNDERWEAR_SELECT_BTN:
		GetDecorationList( iSex, UID_UNDERWEAR, rkList );
		break;
	}
}

void PackageItemBuyWnd::GetClassList( vPackageItem &rkList )
{
	rkList.clear();
	if( m_PackageStyle == PACKAGE_SOLDIER || m_PackageStyle == PACKAGE_PREMIUM_SOLDIER )
	{
		// �������� ���� ���� �뺴
		int iMaxClass = g_ClassPrice.MaxClassPrice();
		for (int i = 0; i <  iMaxClass; i++)
		{
			int iSetIdx = g_ClassPrice.ArrayToSetItemIdx( i );
			const ioSetItemInfo *pInfo = g_SetItemInfoMgr.GetSetInfoByIdx( iSetIdx );
			if( !pInfo )
				continue;

			int iClassType = pInfo->GetSetCode() - SET_ITEM_CODE;

			// �뿩 �뺴�� ���� ����
			if( g_MyInfo.IsCharExerciseStyleToClassType( iClassType, EXERCISE_RENTAL ) == false )
			{
				// ���� �뺴�� ����Ʈ���� ����
				if( g_MyInfo.IsCharMortmain( iClassType ) )
					continue;
			}

			if( m_PackageStyle == PACKAGE_SOLDIER )
			{
				ioEtcItemSoldierPackage *pPackageEtcItem = dynamic_cast< ioEtcItemSoldierPackage * >( g_EtcItemMgr.FindEtcItem(m_dwEtcItemType) );
				if( pPackageEtcItem )
				{
					int iLimitClassType = pPackageEtcItem->GetLimitClassTypeNum();
					if( iLimitClassType < iClassType )
						continue;
				}

				//������ �뺴�� ���
				if( !pPackageEtcItem->IsRightSoldierCode( iClassType ) )
					continue;
			}
			else if( m_PackageStyle == PACKAGE_PREMIUM_SOLDIER )
			{
				if( pInfo->GetPackageType() != ioSetItemInfo::PT_PREMIUM ) 
					continue;

				ioEtcItemSoldierPackage *pPackageEtcItem = dynamic_cast< ioEtcItemSoldierPackage * >(g_EtcItemMgr.FindEtcItem(m_dwEtcItemType));
				if( pPackageEtcItem )
				{
					int iLimitClassType = pPackageEtcItem->GetLimitClassTypeNum();
					if( iLimitClassType < iClassType )
						continue;

					//������ �뺴�� ���
					if( !pPackageEtcItem->IsRightSoldierCode( iClassType ) )
						continue;
				}
			}

			switch( m_iActiveFilter )
			{
			case ioEtcItemSoldierPackage::AF_ACTIVE:
				{
					if( !g_ClassPrice.IsActive( iClassType ) )
						continue;
				}
				break;
			}
			
			PackageItem kItem;
			kItem.m_iMagicCode = iClassType;
			kItem.m_szName     = pInfo->GetName();
			kItem.m_nGradeType = g_UIImageRenderMgr.GetPowerUpGradeType( iClassType );
			rkList.push_back( kItem );
		}

		// �뺴��Ű�� UI ����ó�� ( 12.02.15 )
		if( rkList.empty() )
		{
			PackageItem kItem;
			kItem.m_iMagicCode = 0;
			kItem.m_szName     = STR(1);
			rkList.push_back( kItem );
			return;
		}
	}
	else 
	{
		// �������� �뺴 : ü��(PC��) �뺴 ����
		int iMaxClass =  g_ClassPrice.MaxClassPrice();
		for (int i = 0; i <  iMaxClass; i++)
		{
			int iCharArray = g_MyInfo.GetCharSlotIndexToArray( i );
			if( iCharArray == -1 )
				continue;

			if( g_MyInfo.IsCharExercise( iCharArray ) )
				continue;

			int iSetIdx = g_MyInfo.GetClassType( iCharArray ) - 1;
			const ioSetItemInfo *pInfo = g_SetItemInfoMgr.GetSetInfoByIdx( iSetIdx );
			if( !pInfo )
				continue;

			int iClassType = pInfo->GetSetCode() - SET_ITEM_CODE;

			PackageItem kItem;
			kItem.m_iMagicCode = iClassType;
			kItem.m_szName     = pInfo->GetName();
			kItem.m_nGradeType = g_UIImageRenderMgr.GetPowerUpGradeType( iClassType );
			rkList.push_back( kItem );
		}
	}

	if( rkList.empty() ) return;

	std::sort( rkList.begin() , rkList.end(), PackageItemSort() );
}

void PackageItemBuyWnd::GetDecorationList( int iSearchSexType, int iSearchDecoType, vPackageItem &rkList )
{
	rkList.clear();

	// �뺴��Ű�� UI ����ó�� ( 12.02.15 )
	if( m_CharInfo.m_class_type == 0 )
	{
		PackageItem kItem;
		kItem.m_szName = STR(1);
		rkList.push_back( kItem );
		return;
	}

	int iSexSize = g_DecorationPrice.GetSexSize();
	for (int i = 0; i < iSexSize ; i++)
	{
		if( g_DecorationPrice.GetSexType( i ) != iSearchSexType ) continue;

		int iDecoTypeSize = g_DecorationPrice.GetDecoTypeSize( i );
		for (int j = 0; j < iDecoTypeSize; j++)
		{
			int iDecoType = g_DecorationPrice.GetDecoType( i , j );
			if( iDecoType != iSearchDecoType ) continue;

			int iDecoSize = g_DecorationPrice.GetDecoSize( i, j );
			for (int z = 0; z < iDecoSize; z++)
			{
				bool bActive = g_DecorationPrice.GetActive( i, j, z );
				if( !bActive )
					continue;

				if( !g_DecorationPrice.GetPackageActive( i, j, z ) )
					continue;

				int iDecoCode = g_DecorationPrice.GetDecoCode( i, j, z );
				const DecoData &rkDeco = g_DecorationPrice.GetDecoData( i, iDecoType, iDecoCode );

				PackageItem kItem;
				kItem.m_szName    = rkDeco.m_szName;
				kItem.m_iMagicCode= rkDeco.m_iDecoCode;

				// ���� : �ΰ� ���ڴ� �׻� ����
				if( iDecoType == UID_KINDRED && iDecoCode == RDT_HUMAN_MAN )
				{				
					if( g_MyInfo.IsClassTypeInventory( m_CharInfo.m_class_type ) )
						kItem.m_bKeepItem = true;
					else
						kItem.m_bBasicItem = true;
				}
				else if( IsBought( m_CharInfo.m_class_type, iSearchSexType, iDecoType, iDecoCode ) )
				{
					// ���� ���� üũ
					kItem.m_bKeepItem = true;
				}
				else if( !g_MyInfo.IsClassTypeInventory( m_CharInfo.m_class_type, iSearchSexType, iSearchDecoType ) &&
					     kItem.m_iMagicCode == g_DecorationPrice.GetDefaultDecoCode( iSearchSexType, iSearchDecoType, g_MyInfo.GetPrivateID().GetHashCode() + m_CharInfo.m_class_type + iSearchDecoType ) )
				{
					// �⺻���� �����ϴ� �����۵� ���� ���������� �Ǵ�
					kItem.m_bBasicItem = true;
				}
				else if( !IsRightDecoList( iDecoType ) )
				{
					continue;
				}

				rkList.push_back( kItem );
			}
		}
	}

	if( rkList.empty() ) return;
	if( iSearchDecoType == UID_SKIN_COLOR ) return;  // ���� : �Ǻλ��� �����ϸ� ���̻� ���� 1������ �ǹǷ� �������� �ʴ´�.

	std::sort( rkList.begin() , rkList.end(), PackageItemSort() );
}

void PackageItemBuyWnd::SetPullDownMenu( ioWnd *pOpenBtn )
{
	if( !pOpenBtn ) return;
	
	// �뺴��Ű�� UI ����ó�� ( 12.02.15 )
	if( m_CharInfo.m_class_type == 0 )	return;

	NewShopPullDownWnd *pPullDownWnd = dynamic_cast<NewShopPullDownWnd *>(FindChildWnd( ID_PULLDOWN_WND ));
	if( pPullDownWnd )
	{
		if( pPullDownWnd->IsShow() )
			pPullDownWnd->HideWnd();
		else
		{
			m_OverCharInfo = m_CharInfo;

			if( pOpenBtn->GetID() == ID_CHAR_SELECT_BTN )
			{
				pPullDownWnd->CreatePullDown( pOpenBtn, pOpenBtn->GetXPos() - 206, pOpenBtn->GetYPos() );
			}
			else
			{
				int iGap = pOpenBtn->GetID() - ID_KINDRED_SELECT_BTN;
				pPullDownWnd->CreatePullDown( pOpenBtn, pOpenBtn->GetXPos() - 206, pOpenBtn->GetYPos() - (iGap * 24) );
			}

			vPackageItem kPackageItemList;
			GetPackageItemList( pOpenBtn->GetID(), m_CharInfo.m_sex - 1, kPackageItemList );

			vNewShopPullDownItem kItemList;
			ConvertItemList( kPackageItemList, kItemList );

			for(int i = 0;i < (int)kItemList.size();i++)
			{
				pPullDownWnd->InsertItem( kItemList[i] );
			}
			pPullDownWnd->ShowWnd();

			int iHeight = pPullDownWnd->GetHeight();
			int iEndYPos = pPullDownWnd->GetYPos() + iHeight;
			if( iEndYPos >= GetHeight() )
			{
				int iNewYPos = pOpenBtn->GetYPos() - iHeight - 1;
				pPullDownWnd->SetWndPos( pOpenBtn->GetXPos(), iNewYPos );
			}
		}
	}
}

void PackageItemBuyWnd::SetSelectItemBtn( DWORD dwID, CHARACTER &rkCharInfo, PackageItem &rkItem )
{
	PackageItemBtn *pItemBtn = dynamic_cast<PackageItemBtn*>(FindChildWnd( dwID ));
	if( pItemBtn )
	{
		pItemBtn->SetPackageItem( rkCharInfo.m_sex - 1, rkItem, (int)m_PackageStyle );
		SetSelectCharInfo( dwID, rkCharInfo, rkItem );
	}
}

void PackageItemBuyWnd::SetSelectCharInfo( DWORD dwID, CHARACTER &rkCharInfo, PackageItem &rkItem )
{
	switch( dwID )
	{
	case ID_CHAR_SELECT_BTN:
		rkCharInfo.m_class_type = rkItem.m_iMagicCode;
		break;
	case ID_KINDRED_SELECT_BTN:
		{
			switch( rkItem.m_iMagicCode )
			{
			case RDT_HUMAN_MAN:
				rkCharInfo.m_kindred = 1;
				rkCharInfo.m_sex = 1;
				break;
			case RDT_HUMAN_WOMAN:
				rkCharInfo.m_kindred = 1;
				rkCharInfo.m_sex = 2;
				break;
			case RDT_ELF_MAN:
				rkCharInfo.m_kindred = 2;
				rkCharInfo.m_sex = 1;
				break;
			case RDT_ELF_WOMAN:
				rkCharInfo.m_kindred = 2;
				rkCharInfo.m_sex = 2;
				break;
			case RDT_DWARF_MAN:
				rkCharInfo.m_kindred = 3;
				rkCharInfo.m_sex = 1;
				break;
			case RDT_DWARF_WOMAN:
				rkCharInfo.m_kindred = 3;
				rkCharInfo.m_sex = 2;
				break;
			}
		}
		break;
	case ID_HAIR_SELECT_BTN:
		rkCharInfo.m_hair = rkItem.m_iMagicCode;
		break;
	case ID_HAIR_COLOR_SELECT_BTN:
		rkCharInfo.m_hair_color = rkItem.m_iMagicCode;
		break;
	case ID_FACE_SELECT_BTN:
		rkCharInfo.m_face = rkItem.m_iMagicCode;
		break;
	case ID_SKIN_COLOR_SELECT_BTN:
		rkCharInfo.m_skin_color = rkItem.m_iMagicCode;
		break;
	case ID_UNDERWEAR_SELECT_BTN:
		rkCharInfo.m_underwear = rkItem.m_iMagicCode;
		break;
	}
}

void PackageItemBuyWnd::DefaultDecoration( bool bChangekindred, bool bSetBtn, CHARACTER &rkCharInfo )
{
	vPackageItem kPackageItemList;
	// ����
	if( bChangekindred )
	{
		GetPackageItemList( ID_KINDRED_SELECT_BTN, rkCharInfo.m_sex - 1, kPackageItemList );
		if( !kPackageItemList.empty() ) 
		{
			if( bSetBtn )
				SetSelectItemBtn( ID_KINDRED_SELECT_BTN, rkCharInfo, kPackageItemList[0] );
			else
				SetSelectCharInfo( ID_KINDRED_SELECT_BTN, rkCharInfo, kPackageItemList[0] );
		}
	}

	// �Ӹ�
	PackageItem kDefaultItem;
	DefaultDecoration( ID_HAIR_SELECT_BTN, UID_HAIR, rkCharInfo, kDefaultItem );
	if( bSetBtn )
		SetSelectItemBtn( ID_HAIR_SELECT_BTN, rkCharInfo, kDefaultItem );
	else
		SetSelectCharInfo( ID_HAIR_SELECT_BTN, rkCharInfo, kDefaultItem );
	// �Ӹ���
	DefaultDecoration( ID_HAIR_COLOR_SELECT_BTN, UID_HAIR_COLOR, rkCharInfo, kDefaultItem );
	if( bSetBtn )
		SetSelectItemBtn( ID_HAIR_COLOR_SELECT_BTN, rkCharInfo, kDefaultItem );
	else
		SetSelectCharInfo( ID_HAIR_COLOR_SELECT_BTN, rkCharInfo, kDefaultItem );
	// ��
	DefaultDecoration( ID_FACE_SELECT_BTN, UID_FACE, rkCharInfo, kDefaultItem );
	if( bSetBtn )
		SetSelectItemBtn( ID_FACE_SELECT_BTN, rkCharInfo, kDefaultItem );
	else
		SetSelectCharInfo( ID_FACE_SELECT_BTN, rkCharInfo, kDefaultItem );
	// �Ǻλ�
	DefaultDecoration( ID_SKIN_COLOR_SELECT_BTN, UID_SKIN_COLOR, rkCharInfo, kDefaultItem );
	if( bSetBtn )
		SetSelectItemBtn( ID_SKIN_COLOR_SELECT_BTN, rkCharInfo, kDefaultItem );
	else
		SetSelectCharInfo( ID_SKIN_COLOR_SELECT_BTN, rkCharInfo, kDefaultItem );
	// �⺻��
	DefaultDecoration( ID_UNDERWEAR_SELECT_BTN, UID_UNDERWEAR, rkCharInfo, kDefaultItem );
	if( bSetBtn )
		SetSelectItemBtn( ID_UNDERWEAR_SELECT_BTN, rkCharInfo, kDefaultItem );
	else
		SetSelectCharInfo( ID_UNDERWEAR_SELECT_BTN, rkCharInfo, kDefaultItem );	
}

void PackageItemBuyWnd::DefaultDecoration( DWORD dwID, int iDecoType, CHARACTER &rkCharInfo, PackageItem &rkReturnItem )
{
	{
		rkReturnItem.m_szName.Clear();
		rkReturnItem.m_iMagicCode = 0;
	}

	// �뺴��Ű�� UI ����ó�� ( 12.02.15 )
	if( rkCharInfo.m_class_type == 0 )
	{
		rkReturnItem.m_szName = STR(1);
		return;
	}

	vPackageItem kPackageItemList;
	GetPackageItemList( dwID, rkCharInfo.m_sex - 1, kPackageItemList );
	if( kPackageItemList.empty() ) return;

	// ������������ �����Ѵ�.
	if( (m_PackageStyle == PACKAGE_DECO_FACE       && iDecoType != UID_FACE)       ||
		(m_PackageStyle == PACKAGE_DECO_HAIR       && iDecoType != UID_HAIR)       || 
		(m_PackageStyle == PACKAGE_DECO_SKIN_COLOR && iDecoType != UID_SKIN_COLOR) ||
		(m_PackageStyle == PACKAGE_DECO_HAIR_COLOR && iDecoType != UID_HAIR_COLOR) ||
		(m_PackageStyle == PACKAGE_DECO_KINDRED    && iDecoType != UID_KINDRED)    ||
		(m_PackageStyle == PACKAGE_DECO_UNDERWEAR  && iDecoType != UID_UNDERWEAR) )
	{
		rkReturnItem = kPackageItemList[0];
		return;
	}

	const ioSetItemInfo *pInfo = g_SetItemInfoMgr.GetSetInfoByIdx( rkCharInfo.m_class_type - 1 );
	if( !pInfo ) return;

	int iDefaultCode = pInfo->GetPackageDefaultDecoCode( iDecoType, rkCharInfo.m_sex - 1 );
	for(int i = 0;i < (int)kPackageItemList.size();i++)
	{
		PackageItem &rkItem = kPackageItemList[i];
		if( rkItem.m_iMagicCode == iDefaultCode )
		{
			rkReturnItem = rkItem;
			return;
		}
	}
}

void PackageItemBuyWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_BUY:
		if( cmd == IOBN_BTNUP )
		{
			// �뺴��Ű�� UI ����ó�� ( 12.02.15 )
			if( m_CharInfo.m_class_type == 0 )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
				return;
			}

			PackageItemBuyAgreeWnd *pAgreeWnd = dynamic_cast< PackageItemBuyAgreeWnd* >(g_GUIMgr.FindWnd( PACKAGE_ITEM_BUY_AGREE_WND ));
			if( pAgreeWnd )
			{
				if( g_MyInfo.IsCharRentalToClassType( m_CharInfo.m_class_type ) )
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
					return;
				}

			/*	if( g_MyInfo.IsCharExerciseStyleToClassType( m_CharInfo.m_class_type, EXERCISE_RENTAL ) )
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
					return;
				}	
			*/
				pAgreeWnd->HideWnd();
				if( m_PackageStyle == PACKAGE_SOLDIER || m_PackageStyle == PACKAGE_PREMIUM_SOLDIER )
				{
					pAgreeWnd->SetAgreeSoldierPackage( m_CharInfo, m_PackageStyle, m_dwEtcItemType );
					pAgreeWnd->ShowWnd();
				}
				else
				{
					pAgreeWnd->SetAgreeDecorationPackage( m_CharInfo, m_PackageStyle, m_dwEtcItemType );
					pAgreeWnd->ShowWnd();
				}
			}
		}
		break;
	case ID_CHAR_SELECT_BTN:
	case ID_KINDRED_SELECT_BTN:
	case ID_HAIR_SELECT_BTN:
	case ID_HAIR_COLOR_SELECT_BTN:
	case ID_FACE_SELECT_BTN:
	case ID_SKIN_COLOR_SELECT_BTN:
	case ID_UNDERWEAR_SELECT_BTN:
		if( cmd == IOBN_BTNUP )
		{
			SetPullDownMenu( pWnd );
		}
		break;
	case ID_PULLDOWN_WND:
		if( cmd == IOBN_BTNUP )
		{
			NewShopPullDownWnd *pPullDownWnd = dynamic_cast<NewShopPullDownWnd *>(pWnd);
			if( pPullDownWnd )
			{
				PullDownBtnUP( pPullDownWnd->GetOpenBtn(), param );
				pPullDownWnd->HideWnd();
			}
		}
		else if( cmd == IOWN_OVERED )
		{
			NewShopPullDownWnd *pPullDownWnd = dynamic_cast<NewShopPullDownWnd *>(pWnd);
			if( pPullDownWnd )
				PullDownBtnOver( pPullDownWnd->GetOpenBtn(), param );
		}
		break;
	}
}

void PackageItemBuyWnd::PullDownBtnUP( ioWnd *pOpenBtn, int iListArray )
{
	PackageItemBtn *pItemBtn = dynamic_cast<PackageItemBtn*>( pOpenBtn );
	if( !pItemBtn ) return;	

	vPackageItem kPackageItemList;
	GetPackageItemList( pItemBtn->GetID(), m_CharInfo.m_sex - 1, kPackageItemList );	
	if( !COMPARE( iListArray, 0, (int)kPackageItemList.size() ) ) return;			
	if( pItemBtn->GetMagicCode() == kPackageItemList[iListArray].m_iMagicCode ) return;

	SetSelectItemBtn( pOpenBtn->GetID(), m_CharInfo, kPackageItemList[iListArray] );
	if( pItemBtn->GetID() == ID_CHAR_SELECT_BTN || pItemBtn->GetID() == ID_KINDRED_SELECT_BTN )      // �뺴 & ������ �ٲ�� ����Ʈ ����
		DefaultDecoration( (pItemBtn->GetID() == ID_CHAR_SELECT_BTN), true, m_CharInfo );

	NewShopHelpWnd *pCharWnd = dynamic_cast<NewShopHelpWnd *>( FindChildWnd( ID_SOLDIER_WND ));
	if( pCharWnd )
	{
		pCharWnd->SetZPosition( 600.0f );
		pCharWnd->SetChar( m_CharInfo );
		pCharWnd->SetEquip( true, false, true );
	}
}

void PackageItemBuyWnd::PullDownBtnOver( ioWnd *pOpenBtn, int iListArray )
{
	PackageItemBtn *pItemBtn = dynamic_cast<PackageItemBtn*>( pOpenBtn );
	if( !pItemBtn ) return;	

	NewShopHelpWnd *pCharWnd = dynamic_cast<NewShopHelpWnd *>( FindChildWnd( ID_SOLDIER_WND ));
	if( !pCharWnd ) return;		

	vPackageItem kPackageItemList;
	GetPackageItemList( pItemBtn->GetID(), m_OverCharInfo.m_sex - 1, kPackageItemList );

	bool bReleaseHelm = false;
	bool bReleaseAll  = false;
	int  iMagicCode   = 0;
	switch( pItemBtn->GetID() )
	{
	case ID_CHAR_SELECT_BTN:
		iMagicCode = m_OverCharInfo.m_class_type;
		break;
	case ID_KINDRED_SELECT_BTN:
		bReleaseHelm = true;
		iMagicCode = m_OverCharInfo.m_sex - 1;
		break;
	case ID_HAIR_SELECT_BTN:
		bReleaseHelm = true;
		iMagicCode = m_OverCharInfo.m_hair;
		break;
	case ID_HAIR_COLOR_SELECT_BTN:
		bReleaseHelm = true;
		iMagicCode = m_OverCharInfo.m_hair_color;
		break;
	case ID_FACE_SELECT_BTN:
		bReleaseHelm = true;
		iMagicCode = m_OverCharInfo.m_face;
		break;
	case ID_SKIN_COLOR_SELECT_BTN:
		bReleaseHelm = true;
		iMagicCode = m_OverCharInfo.m_skin_color;
		break;
	case ID_UNDERWEAR_SELECT_BTN:
		bReleaseHelm = true;
		bReleaseAll  = true;
		iMagicCode = m_OverCharInfo.m_underwear;
		break;
	}

	if( !COMPARE( iListArray, 0, (int)kPackageItemList.size() ) ) return;			
	if( iMagicCode == kPackageItemList[iListArray].m_iMagicCode ) return;

	SetSelectCharInfo( pOpenBtn->GetID(), m_OverCharInfo, kPackageItemList[iListArray] );
	if( pItemBtn->GetID() == ID_CHAR_SELECT_BTN || pItemBtn->GetID() == ID_KINDRED_SELECT_BTN )      // �뺴 & ������ �ٲ�� ����Ʈ ����
		DefaultDecoration( (pItemBtn->GetID() == ID_CHAR_SELECT_BTN), false, m_OverCharInfo );

	pCharWnd->SetZPosition( 600.0f );
	pCharWnd->SetChar( m_OverCharInfo );
	pCharWnd->SetEquip( bReleaseHelm, bReleaseAll, ( pItemBtn->GetID() == ID_CHAR_SELECT_BTN ) );
}


void PackageItemBuyWnd::ShowPackage( PackageStyle ePackageStyle, DWORD dwType, int eActiveFilter /* = 0 : none */ )
{
	m_PackageStyle		= ePackageStyle;
	m_dwEtcItemType		= dwType;
	m_iActiveFilter		= eActiveFilter;

	if( m_PackageStyle == PACKAGE_SOLDIER || m_PackageStyle == PACKAGE_PREMIUM_SOLDIER )
		SetTitleText( STR(1) );
	else if( m_PackageStyle == PACKAGE_DECORATION )
		SetTitleText( STR(2) );
	else if( m_PackageStyle == PACKAGE_DECO_FACE )
		SetTitleText( STR(3) );
	else if( m_PackageStyle == PACKAGE_DECO_HAIR )
		SetTitleText( STR(4) );
	else if( m_PackageStyle == PACKAGE_DECO_SKIN_COLOR )
		SetTitleText( STR(5) );
	else if( m_PackageStyle == PACKAGE_DECO_HAIR_COLOR )
		SetTitleText( STR(6) );
	else if( m_PackageStyle == PACKAGE_DECO_KINDRED )
		SetTitleText( STR(7) );
	else if( m_PackageStyle == PACKAGE_DECO_UNDERWEAR )
		SetTitleText( STR(8) );

	ShowWnd();
}

void PackageItemBuyWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;	

	NewShopPullDownWnd *pPullDownWnd = dynamic_cast<NewShopPullDownWnd *>( FindChildWnd( ID_PULLDOWN_WND ));
	if( !pPullDownWnd ) return;
	NewShopHelpWnd *pCharWnd = dynamic_cast<NewShopHelpWnd *>( FindChildWnd( ID_SOLDIER_WND ));
	if( !pCharWnd ) return;

	if( !pPullDownWnd->IsShow() )
	{
		if( pCharWnd->IsReleaseHelm() == false || pCharWnd->IsReleaseAllItem() == true || pCharWnd->IsShopMotion() == false )
		{
			pCharWnd->SetZPosition( 600.0f );
			pCharWnd->SetChar( m_CharInfo );
			pCharWnd->SetEquip( true, false, true );
		}
	}
	else if( pPullDownWnd->GetOpenBtn() && pPullDownWnd->GetListArray() == -1 )
	{
		if( pPullDownWnd->GetOpenBtn()->GetID() == ID_CHAR_SELECT_BTN )
		{
			if( pCharWnd->IsReleaseHelm() == true || pCharWnd->IsReleaseAllItem() == true || pCharWnd->IsShopMotion() == false )
			{
				pCharWnd->SetZPosition( 600.0f );
				pCharWnd->SetChar( m_CharInfo );
				pCharWnd->SetEquip( false, false, true );
			}
		}
		else if( pPullDownWnd->GetOpenBtn()->GetID() == ID_UNDERWEAR_SELECT_BTN )
		{
			if( pCharWnd->IsReleaseHelm() == false || pCharWnd->IsReleaseAllItem() == false || pCharWnd->IsShopMotion() == true )
			{
				pCharWnd->SetZPosition( 600.0f );
				pCharWnd->SetChar( m_CharInfo );
				pCharWnd->SetEquip( true, true, false );
			}
		}
		else
		{
			if( pCharWnd->IsReleaseHelm() == false || pCharWnd->IsReleaseAllItem() == true || pCharWnd->IsShopMotion() == true )
			{
				pCharWnd->SetZPosition( 600.0f );
				pCharWnd->SetChar( m_CharInfo );
				pCharWnd->SetEquip( true, false, false );
			}
		}
	}
}

void PackageItemBuyWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();
}

bool PackageItemBuyWnd::IsRightDecoList( int iDecoType )
{
	if( m_PackageStyle == PACKAGE_SOLDIER || m_PackageStyle == PACKAGE_PREMIUM_SOLDIER )
		return true;
	if( m_PackageStyle == PACKAGE_DECORATION )
		return true;
	if( m_PackageStyle == PACKAGE_DECO_FACE && iDecoType == UID_FACE )
		return true;
	if( m_PackageStyle == PACKAGE_DECO_HAIR && iDecoType == UID_HAIR )
		return true;
	if( m_PackageStyle == PACKAGE_DECO_SKIN_COLOR && iDecoType == UID_SKIN_COLOR )
		return true;
	if( m_PackageStyle == PACKAGE_DECO_HAIR_COLOR && iDecoType == UID_HAIR_COLOR )
		return true;
	if( m_PackageStyle == PACKAGE_DECO_KINDRED && iDecoType == UID_KINDRED )
		return true;
	if( m_PackageStyle == PACKAGE_DECO_UNDERWEAR && iDecoType == UID_UNDERWEAR )
		return true;

	return false;
}

bool PackageItemBuyWnd::IsBought( int iClassType, int iSexType, int iDecoType, int iDecoCode )
{
	int iCharArray = g_MyInfo.GetClassArray( iClassType );

	// ü��,�̰��뺴�̸� ���������� �������� ���� ��
	if( g_MyInfo.IsCharExercise( iCharArray ) || !COMPARE( iCharArray, 0, g_MyInfo.GetCharCount() ) ) 
	{
		if( iDecoType == UID_KINDRED && iDecoCode == RDT_HUMAN_MAN )
			return false;
	}

	//���� Ư�� 1111 22 333 ( ��Ʈ, ����(����), ġ��Ÿ�� )

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
			int iEquipType = iDecoCode / FLOAT1000;
			int iEquipCode = iDecoCode % 1000;
			iType += iEquipType;
			iCode  = iEquipCode;
		}
		break;			
	}
	return g_MyInfo.IsSlotItem( iType, iCode );
}

void PackageItemBuyWnd::PrintManual( int a_iXPos, int a_iYPos , float fScale )
{
	if( !m_pManual )
		return;

	ioEtcItem *pItem = g_EtcItemMgr.FindEtcItem( m_dwEtcItemType );
	/*
	switch( m_PackageStyle )
	{
	case PACKAGE_SOLDIER:
		pItem = g_EtcItemMgr.FindEtcItem( ioEtcItem::EIT_ETC_SOLDIER_PACKAGE );
		break;
	case PACKAGE_PREMIUM_SOLDIER:
		pItem = g_EtcItemMgr.FindEtcItem( ioEtcItem::EIT_ETC_PREMIUM_SOLDIER_PACKAGE );
		break;
	case PACKAGE_DECORATION:
		pItem = g_EtcItemMgr.FindEtcItem( ioEtcItem::EIT_ETC_DECORATION_PACKAGE );
		break;
	case PACKAGE_DECO_UNDERWEAR:
		pItem = g_EtcItemMgr.FindEtcItem( ioEtcItem::EIT_ETC_DECO_UNDERWEAR_PACKAGE );
		break;
	}
	*/

	if( !pItem ) return;

	int iSubManual = pItem->GetInventorySubManual() - 1;
	ioComplexStringPrinter kPrinter;
	int iMaxLine = m_pManual->GetMaxLine( iSubManual );
	for(int iLine = 1; iLine < iMaxLine; iLine++)
	{
		m_pManual->SetPrinter( iSubManual, iLine, fScale, kPrinter);
		a_iYPos += m_pManual->GetSpaceLine( iSubManual, iLine);
		kPrinter.PrintFullTextWidthCut( a_iXPos, a_iYPos, TAT_LEFT, DESC_WIDTH );
		kPrinter.ClearList();
	}
}

//////////////////////////////////////////////////////////////////////////
PackageItemBuyAgreeWnd::PackageItemBuyAgreeWnd()
{
	m_pCharIcon = NULL;
	m_pBackEffect = NULL;

	m_PackageStyle = PackageItemBuyWnd::PACKAGE_SOLDIER;
	m_nGradeType = 0;
}

PackageItemBuyAgreeWnd::~PackageItemBuyAgreeWnd()
{
	SAFEDELETE(m_pCharIcon);
	SAFEDELETE(m_pBackEffect);

	for( int i=0; i < MAX_AGREE_LIST; ++i )
	{
		m_AgreeList[i].ClearList();
		m_PesoList[i].ClearList();
	}
}

void PackageItemBuyAgreeWnd::SetAgreeSoldierPackage( CHARACTER &rkCharInfo, PackageItemBuyWnd::PackageStyle ePackageStyle, DWORD dwEtcType )
{
	m_PackageStyle = ePackageStyle;
	m_dwEtcItemType = dwEtcType;
	m_CharInfo     = rkCharInfo;

	for( int i=0; i < MAX_AGREE_LIST; ++i )
	{
		m_AgreeList[i].ClearList();
		m_PesoList[i].ClearList();
	}

	char szBuf[MAX_PATH] = "";
	char szConvertNum[MAX_PATH] = "";
	int iKeepBonusPeso = 0;

	// �뺴 Ÿ��
	SAFEDELETE( m_pCharIcon );
	m_pCharIcon     = g_MyInfo.GetSoldierIcon( m_CharInfo.m_class_type, ( ( m_CharInfo.m_sex - 1 ) == RDT_HUMAN_MAN ) );

	m_nGradeType = g_UIImageRenderMgr.GetPowerUpGradeType( m_CharInfo.m_class_type );

	ioHashString szClassName = g_MyInfo.GetClassName( m_CharInfo.m_class_type, m_CharInfo.m_eCharPowerUpType );
	m_AgreeList[0].SetTextStyle( TS_NORMAL );
	m_AgreeList[0].SetBkColor( TCT_DEFAULT_WHITE );
	m_AgreeList[0].SetTextColor( TCT_DEFAULT_GRAY );
	m_AgreeList[0].AddTextPiece( FONT_SIZE_12, STR(1), szClassName.c_str() );

	// ������ �뺴 Ȯ��
	if( g_MyInfo.GetExerciseCharArrayByClass( m_CharInfo.m_class_type ) == -1 )
		iKeepBonusPeso = g_ClassPrice.GetTimeCharResellPeso( m_CharInfo.m_class_type, g_MyInfo.GetCharTotalSecond( g_MyInfo.GetClassArray( m_CharInfo.m_class_type ) ) );

	if( iKeepBonusPeso > 0 )
	{
		Help::ConvertNumToStrComma( iKeepBonusPeso, szConvertNum, sizeof( szConvertNum ) );
		SafeSprintf( szBuf, sizeof( szBuf ), STR(2), szConvertNum );

		m_PesoList[0].SetTextStyle( TS_NORMAL );
		m_PesoList[0].SetBkColor( TCT_DEFAULT_WHITE );
		m_PesoList[0].SetTextColor( TCT_DEFAULT_BLUE );
		m_PesoList[0].AddTextPiece( FONT_SIZE_12, STR(3), szBuf );
	}

	enum { MAX_DECO_ITEM = 6, };
	int iDecoTypeList[MAX_DECO_ITEM]={ UID_KINDRED, UID_HAIR, UID_HAIR_COLOR, UID_FACE, UID_SKIN_COLOR, UID_UNDERWEAR };
	int iDecoCodeList[MAX_DECO_ITEM]={ (int)GetRaceDetailType( m_CharInfo ), m_CharInfo.m_hair, m_CharInfo.m_hair_color, m_CharInfo.m_face, m_CharInfo.m_skin_color, m_CharInfo.m_underwear };
	for(int i = 0;i < MAX_DECO_ITEM;i++)
	{
		const DecoData &rkDeco = g_DecorationPrice.GetDecoData( m_CharInfo.m_sex - 1, iDecoTypeList[i], iDecoCodeList[i] );
		int iCurPeso = GetDecoKeepAndBasicCheck( iDecoTypeList[i], iDecoCodeList[i], rkDeco.m_szName, szBuf );
		iKeepBonusPeso += iCurPeso;

		m_AgreeList[i+1].SetTextStyle( TS_NORMAL );
		m_AgreeList[i+1].SetBkColor( TCT_DEFAULT_WHITE );
		m_AgreeList[i+1].SetTextColor( TCT_DEFAULT_DARKGRAY );
		m_AgreeList[i+1].AddTextPiece( FONT_SIZE_12, STR(4), szBuf );

		if( iCurPeso > 0 )
		{
			Help::ConvertNumToStrComma( iCurPeso, szConvertNum, sizeof( szConvertNum ) );
			SafeSprintf( szBuf, sizeof( szBuf ), STR(5), szConvertNum );

			m_PesoList[i+1].SetTextStyle( TS_NORMAL );
			m_PesoList[i+1].SetBkColor( TCT_DEFAULT_WHITE );
			m_PesoList[i+1].SetTextColor( TCT_DEFAULT_BLUE );
			m_PesoList[i+1].AddTextPiece( FONT_SIZE_12, STR(6), szBuf );
		}
	}

	//���ʽ� ���
	if( iKeepBonusPeso > 0 )
	{
		m_AgreeList[7].SetTextStyle( TS_NORMAL );
		m_AgreeList[7].SetBkColor( TCT_DEFAULT_WHITE );
		m_AgreeList[7].SetTextColor( TCT_DEFAULT_BLUE );
		m_AgreeList[7].AddTextPiece( FONT_SIZE_12, STR(7) );

		Help::ConvertNumToStrComma( iKeepBonusPeso, szConvertNum, sizeof( szConvertNum ) );
		SafeSprintf( szBuf, sizeof( szBuf ), STR(8), szConvertNum );

		m_PesoList[7].SetTextStyle( TS_NORMAL );
		m_PesoList[7].SetBkColor( TCT_DEFAULT_WHITE );
		m_PesoList[7].SetTextColor( TCT_DEFAULT_BLUE );
		m_PesoList[7].AddTextPiece( FONT_SIZE_12, STR(9), szBuf );
	}
}

void PackageItemBuyAgreeWnd::SetAgreeDecorationPackage( CHARACTER &rkCharInfo , PackageItemBuyWnd::PackageStyle ePackageStyle, DWORD dwEtcType )
{
	m_PackageStyle = ePackageStyle;
	m_dwEtcItemType = dwEtcType;
	m_CharInfo     = rkCharInfo;

	for( int i=0; i < MAX_AGREE_LIST; ++i )
	{
		m_AgreeList[i].ClearList();
		m_PesoList[i].ClearList();
	}

	char szBuf[MAX_PATH] = "";
	char szConvertNum[MAX_PATH] = "";
	int iKeepBonusPeso = 0;

	// �뺴 Ÿ��
	SAFEDELETE( m_pCharIcon );
	m_pCharIcon = g_MyInfo.GetSoldierIcon( m_CharInfo.m_class_type, ( ( m_CharInfo.m_sex - 1 ) == RDT_HUMAN_MAN ) );

	m_nGradeType = g_UIImageRenderMgr.GetPowerUpGradeType( m_CharInfo.m_class_type );

	ioHashString szClassName = g_MyInfo.GetClassName( m_CharInfo.m_class_type, m_CharInfo.m_eCharPowerUpType );
	m_AgreeList[0].SetTextStyle( TS_NORMAL );
	m_AgreeList[0].SetBkColor( TCT_DEFAULT_WHITE );
	m_AgreeList[0].SetTextColor( TCT_DEFAULT_GRAY );
	m_AgreeList[0].AddTextPiece( FONT_SIZE_12, STR(1), szClassName.c_str() );

	enum { MAX_DECO_ITEM = 6, };
	int iDecoTypeList[MAX_DECO_ITEM]={ UID_KINDRED, UID_HAIR, UID_HAIR_COLOR, UID_FACE, UID_SKIN_COLOR, UID_UNDERWEAR };
	int iDecoCodeList[MAX_DECO_ITEM]={ (int)GetRaceDetailType( m_CharInfo ), m_CharInfo.m_hair, m_CharInfo.m_hair_color, m_CharInfo.m_face, m_CharInfo.m_skin_color, m_CharInfo.m_underwear };
	for(int i = 0;i < MAX_DECO_ITEM;i++)
	{
		const DecoData &rkDeco = g_DecorationPrice.GetDecoData( m_CharInfo.m_sex - 1, iDecoTypeList[i], iDecoCodeList[i] ); 
		int iPeso =  GetDecoKeepAndBasicCheck( iDecoTypeList[i], iDecoCodeList[i], rkDeco.m_szName, szBuf );
		if( IsRightDecoType( iDecoTypeList[i] ) )
			iKeepBonusPeso  += iPeso;

		m_AgreeList[i+1].SetTextStyle( TS_NORMAL );
		m_AgreeList[i+1].SetBkColor( TCT_DEFAULT_WHITE );
		m_AgreeList[i+1].SetTextColor( TCT_DEFAULT_DARKGRAY );
		m_AgreeList[i+1].AddTextPiece( FONT_SIZE_12, STR(2), szBuf );

		if( iPeso > 0 )
		{
			Help::ConvertNumToStrComma( iPeso, szConvertNum, sizeof( szConvertNum ) );
			SafeSprintf( szBuf, sizeof( szBuf ), STR(3), szConvertNum );

			m_PesoList[i+1].SetTextStyle( TS_NORMAL );
			m_PesoList[i+1].SetBkColor( TCT_DEFAULT_WHITE );
			m_PesoList[i+1].SetTextColor( TCT_DEFAULT_BLUE );
			m_PesoList[i+1].AddTextPiece( FONT_SIZE_12, STR(4), szBuf );
		}
	}

	//���ʽ� ���
	if( iKeepBonusPeso > 0 )
	{
		m_AgreeList[7].SetTextStyle( TS_NORMAL );
		m_AgreeList[7].SetBkColor( TCT_DEFAULT_WHITE );
		m_AgreeList[7].SetTextColor( TCT_DEFAULT_BLUE );
		m_AgreeList[7].AddTextPiece( FONT_SIZE_12, STR(5) );

		Help::ConvertNumToStrComma( iKeepBonusPeso, szConvertNum, sizeof( szConvertNum ) );
		SafeSprintf( szBuf, sizeof( szBuf ), STR(6), szConvertNum );

		m_PesoList[7].SetTextStyle( TS_NORMAL );
		m_PesoList[7].SetBkColor( TCT_DEFAULT_WHITE );
		m_PesoList[7].SetTextColor( TCT_DEFAULT_BLUE );
		m_PesoList[7].AddTextPiece( FONT_SIZE_12, STR(7), szBuf );
	}
}

int PackageItemBuyAgreeWnd::GetDecoKeepAndBasicCheck( int iDecoType, int iDecoCode, ioHashString szDecoName, char *pReturn )
{
	if( pReturn == NULL )
		return 0;

	int iKeepBonusPeso = 0;
	int iNameWidth = 0;
	char szAddText[MAX_PATH] = "";
	// ���� : �ΰ� ���ڴ� �׻� ����
	if( iDecoType == UID_KINDRED && iDecoCode == RDT_HUMAN_MAN )
	{				
		if( g_MyInfo.IsClassTypeInventory( m_CharInfo.m_class_type ) )
			SafeSprintf( szAddText, sizeof( szAddText ), STR(1) );
		else
			SafeSprintf( szAddText, sizeof( szAddText ), STR(1) );
		iKeepBonusPeso = g_DecorationPrice.GetDecoPackageKeepPeso( m_CharInfo.m_sex - 1, iDecoType );
		iNameWidth     = min( 155, g_FontMgr.GetTextWidth( szDecoName.c_str(), TS_OUTLINE_FULL_2X, FONT_SIZE_13 ) + FLOAT1 );
	}
	else if( PackageItemBuyWnd::IsBought( m_CharInfo.m_class_type, m_CharInfo.m_sex - 1, iDecoType, iDecoCode ) )
	{
		SafeSprintf( szAddText, sizeof( szAddText ), STR(3) );
		iKeepBonusPeso = g_DecorationPrice.GetDecoPackageKeepPeso( m_CharInfo.m_sex - 1, iDecoType );
		iNameWidth     = min( 155, g_FontMgr.GetTextWidth( szDecoName.c_str(), TS_OUTLINE_FULL_2X, FONT_SIZE_13 ) + FLOAT1 );
	}
	else if( !g_MyInfo.IsClassTypeInventory( m_CharInfo.m_class_type, m_CharInfo.m_sex - 1, iDecoType ) &&
			 iDecoCode == g_DecorationPrice.GetDefaultDecoCode( m_CharInfo.m_sex - 1, iDecoType, g_MyInfo.GetPrivateID().GetHashCode() + m_CharInfo.m_class_type + iDecoType ) )
	{
		SafeSprintf( szAddText, sizeof( szAddText ), STR(1) );
		iKeepBonusPeso = g_DecorationPrice.GetDecoPackageKeepPeso( m_CharInfo.m_sex - 1, iDecoType );
		iNameWidth = min( 155, g_FontMgr.GetTextWidth( szDecoName.c_str(), TS_OUTLINE_FULL_2X, FONT_SIZE_13 ) + FLOAT1 );
	}
	else
	{
		iNameWidth = min( 198, g_FontMgr.GetTextWidth( szDecoName.c_str(), TS_OUTLINE_FULL_2X, FONT_SIZE_13 ) + FLOAT1 );
	}
	char szName[MAX_PATH] = "";
	Help::StringCutFun( FONT_SIZE_13, iNameWidth, TS_OUTLINE_FULL_2X, szName, sizeof( szName ), szDecoName.c_str() );

	sprintf( pReturn, "%s%s", szName, szAddText );
	return iKeepBonusPeso;
}

bool PackageItemBuyAgreeWnd::FillPackageSet( SP2Packet &rkPacket )
{
	// �뺴
	rkPacket << m_CharInfo.m_class_type;

	// �⺻ ġ�� 5��(�Ӹ�, �Ӹ���, ��, �Ǻλ�, �ӿ�) + ���� ġ�� 6��(����:����, �Ӹ�, �Ӹ���, ��, �Ǻλ�, �ӿ�) 
	// �⺻ ġ�� 5���� �⺻������ �־��ش�.
	int i;
	ITEMSLOTVec vItemSlot;	
	{   // �⺻ ġ�� ���� : �⺻ ġ�忡�� ������ ���ܽ�Ų��.
		g_DecorationPrice.GetPackageBuyDefaultList( m_CharInfo, vItemSlot );
		int iDefaultSlotItem = vItemSlot.size();
		rkPacket << iDefaultSlotItem;
		for(i = 0;i < iDefaultSlotItem;i++)
		{
			rkPacket << vItemSlot[i].m_item_type << vItemSlot[i].m_item_code;
		}
		vItemSlot.clear();
	}

	// ������ ġ���� �����Ѵ�.		
	IntVec vDecoTypeVec;
	vDecoTypeVec.reserve(10);

	if( m_PackageStyle == PackageItemBuyWnd::PACKAGE_SOLDIER ||
		m_PackageStyle == PackageItemBuyWnd::PACKAGE_PREMIUM_SOLDIER ||
		m_PackageStyle == PackageItemBuyWnd::PACKAGE_DECORATION )
	{
		vDecoTypeVec.push_back( UID_KINDRED );
		vDecoTypeVec.push_back( UID_HAIR );
		vDecoTypeVec.push_back( UID_HAIR_COLOR );
		vDecoTypeVec.push_back( UID_FACE );
		vDecoTypeVec.push_back( UID_SKIN_COLOR );
		vDecoTypeVec.push_back( UID_UNDERWEAR );
	}

	else if( m_PackageStyle == PackageItemBuyWnd::PACKAGE_DECO_KINDRED )
		vDecoTypeVec.push_back( UID_KINDRED );
	else if( m_PackageStyle == PackageItemBuyWnd::PACKAGE_DECO_HAIR )
		vDecoTypeVec.push_back( UID_HAIR );
	else if( m_PackageStyle == PackageItemBuyWnd::PACKAGE_DECO_HAIR_COLOR )
		vDecoTypeVec.push_back( UID_HAIR_COLOR );
	else if( m_PackageStyle == PackageItemBuyWnd::PACKAGE_DECO_FACE )
		vDecoTypeVec.push_back( UID_FACE );
	else if( m_PackageStyle == PackageItemBuyWnd::PACKAGE_DECO_SKIN_COLOR )
		vDecoTypeVec.push_back( UID_SKIN_COLOR );
	else if( m_PackageStyle == PackageItemBuyWnd::PACKAGE_DECO_UNDERWEAR )
		vDecoTypeVec.push_back( UID_UNDERWEAR );


	int iSize = vDecoTypeVec.size();
	for(i = 0;i < iSize;i++)
	{
		vItemSlot.push_back( g_DecorationPrice.GetItemSlot( m_CharInfo, vDecoTypeVec[i] ) );
	}
	vDecoTypeVec.clear();

	int iDecoSize = vItemSlot.size();
	rkPacket << iDecoSize;
	for(i = 0;i < iDecoSize;i++)
	{
		rkPacket << vItemSlot[i].m_item_type << vItemSlot[i].m_item_code;
	}
	vItemSlot.clear();
	
	//
	return true;
}

void PackageItemBuyAgreeWnd::SendSoldierPackage()
{
	// ���� ���� �뺴�̰ų� ���� �뺴�̴�.
	if( g_MyInfo.GetMortmainCharCount() + g_MyInfo.GetExerciseCharCount() >= g_MyInfo.GetCurMaxCharSlot() )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return;
	}

	DWORD dwEtcItemType = 0;
	if( m_PackageStyle == PackageItemBuyWnd::PACKAGE_SOLDIER )
		dwEtcItemType = m_dwEtcItemType;
	else if( m_PackageStyle == PackageItemBuyWnd::PACKAGE_PREMIUM_SOLDIER )
		dwEtcItemType = m_dwEtcItemType;


	ioUserEtcItem *pUserEtcItem = g_MyInfo.GetUserEtcItem();
	if( !pUserEtcItem || !pUserEtcItem->IsCanUse( dwEtcItemType ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
	}
	else
	{
		SP2Packet kPacket( CTPK_ETCITEM_USE );
		kPacket << dwEtcItemType;	
		if( FillPackageSet( kPacket ) )			
		{
			TCPNetwork::SendToServer( kPacket );
			TCPNetwork::MouseBusy( true );
		}
	}

	HideWnd();
}

void PackageItemBuyAgreeWnd::SendDecorationPackage()
{
	int iEtcItemType = 0;
	if( m_PackageStyle == PackageItemBuyWnd::PACKAGE_DECORATION )
		iEtcItemType = ioEtcItem::EIT_ETC_DECORATION_PACKAGE;
	else if( m_PackageStyle == PackageItemBuyWnd::PACKAGE_DECO_UNDERWEAR )
		iEtcItemType = ioEtcItem::EIT_ETC_DECO_UNDERWEAR_PACKAGE;
	else
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		HideWnd();
		return;
	}

	ioUserEtcItem *pUserEtcItem = g_MyInfo.GetUserEtcItem();
	if( !pUserEtcItem || !pUserEtcItem->IsCanUse( iEtcItemType ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
	}
	else
	{
		SP2Packet kPacket( CTPK_ETCITEM_USE );
		kPacket << iEtcItemType;	
		if( FillPackageSet( kPacket ) )			
		{
			TCPNetwork::SendToServer( kPacket );
			TCPNetwork::MouseBusy( true );
		}
	}

	HideWnd();
}

void PackageItemBuyAgreeWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_EXIT:
	case ID_CANCEL:
		if( cmd == IOBN_BTNUP )
			HideWnd();
		break;
	case ID_AGREE:
		if( cmd == IOBN_BTNUP )
		{
			if( m_PackageStyle == PackageItemBuyWnd::PACKAGE_SOLDIER || m_PackageStyle == PackageItemBuyWnd::PACKAGE_PREMIUM_SOLDIER )
				SendSoldierPackage();
			else
				SendDecorationPackage();
		}
		break;
	}
}

void PackageItemBuyAgreeWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "back_effect" )
	{
		SAFEDELETE( m_pBackEffect );
		m_pBackEffect = pImage;
	}
	else
		ioWnd::AddRenderImage( szType, pImage );
}

void PackageItemBuyAgreeWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	enum
	{
		ICON_X		= 125,
		ICON_Y		= 98,

		TITLE_X		= 125,
		TITLE_Y		= 146,

		LIST_X		= 27,
		LIST_X2		= 223,
		LIST_Y		= 175,
		LIST_GAP	= 21,

		FINAL_DESC_X	= 125,
		FINAL_DESC_Y	= 359,
	};

	if( m_pBackEffect )
		m_pBackEffect->Render( iXPos+ICON_X, iYPos+ICON_Y, UI_RENDER_MULTIPLY );

	g_UIImageRenderMgr.RenderGradeIconBack( m_nGradeType, iXPos+ICON_X, iYPos+ICON_Y );

	if( m_pCharIcon )
		m_pCharIcon->Render( iXPos+ICON_X, iYPos+ICON_Y );

	// List
	for( int i=0; i < MAX_AGREE_LIST; ++i )
	{
		if( !m_AgreeList[i].IsEmpty() )
		{
			m_AgreeList[i].PrintFullText( iXPos+LIST_X, iYPos+LIST_Y+(LIST_GAP*i), TAT_LEFT );
		}

		if( !m_PesoList[i].IsEmpty() )
		{
			m_PesoList[i].PrintFullText( iXPos+LIST_X2, iYPos+LIST_Y+(LIST_GAP*i), TAT_RIGHT );
		}
	}

	// Title
	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );

	if( m_PackageStyle == PackageItemBuyWnd::PACKAGE_SOLDIER || m_PackageStyle == PackageItemBuyWnd::PACKAGE_PREMIUM_SOLDIER )
		g_FontMgr.PrintText( iXPos+TITLE_X, iYPos+TITLE_Y, FONT_SIZE_17, STR(1) );
	else
		g_FontMgr.PrintText( iXPos+TITLE_X, iYPos+TITLE_Y, FONT_SIZE_17, STR(2) );


	// Last
	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetTextColor( TCT_DEFAULT_GREEN );
	g_FontMgr.PrintText( iXPos+FINAL_DESC_X, iYPos+FINAL_DESC_Y, FONT_SIZE_13, STR(3) );
}

bool PackageItemBuyAgreeWnd::IsRightDecoType( int iDecoType )
{
	if( m_PackageStyle == PackageItemBuyWnd::PACKAGE_SOLDIER ||
		m_PackageStyle == PackageItemBuyWnd::PACKAGE_PREMIUM_SOLDIER ||
		m_PackageStyle == PackageItemBuyWnd::PACKAGE_DECORATION )
		return true;

	if( m_PackageStyle == PackageItemBuyWnd::PACKAGE_DECO_FACE && iDecoType == UID_FACE )
		return true;
	if( m_PackageStyle == PackageItemBuyWnd::PACKAGE_DECO_HAIR && iDecoType == UID_HAIR )
		return true;
	if( m_PackageStyle == PackageItemBuyWnd::PACKAGE_DECO_SKIN_COLOR && iDecoType == UID_SKIN_COLOR )
		return true;
	if( m_PackageStyle == PackageItemBuyWnd::PACKAGE_DECO_HAIR_COLOR && iDecoType == UID_HAIR_COLOR )
		return true;
	if( m_PackageStyle == PackageItemBuyWnd::PACKAGE_DECO_KINDRED && iDecoType == UID_KINDRED )
		return true;
	if( m_PackageStyle == PackageItemBuyWnd::PACKAGE_DECO_UNDERWEAR && iDecoType == UID_UNDERWEAR )
		return true;

	return false;
}
//////////////////////////////////////////////////////////////////////////
PackageItemBuyResultWnd::PackageItemBuyResultWnd()
{
	m_pBackEffect = NULL;
	m_pCharIcon = NULL;

	m_iClassType = 0;
	m_nGradeType = 0;
}

PackageItemBuyResultWnd::~PackageItemBuyResultWnd()
{
	SAFEDELETE( m_pBackEffect );
	SAFEDELETE( m_pCharIcon );

	for( int i=0; i < MAX_AGREE_LIST; ++i )
	{
		m_AgreeList[i].ClearList();
	}
}

void PackageItemBuyResultWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "back_effect" )
	{
		SAFEDELETE( m_pBackEffect );
		m_pBackEffect = pImage;
	}
	else
		ioWnd::AddRenderImage( szType, pImage );
}

void PackageItemBuyResultWnd::SetResultSoldierPackage( CHARACTER &rkCharInfo, int iKeepBonusPeso )
{
	for( int i=0; i < MAX_AGREE_LIST; ++i )
	{
		m_AgreeList[i].ClearList();
	}

	ShowWnd();

	char szBuf[MAX_PATH] = "";

	// Title
	SafeSprintf( szBuf, sizeof( szBuf ), STR(1) );
	m_TitleName = szBuf;

	// �뺴 Ÿ��
	m_iClassType = rkCharInfo.m_class_type;

	SAFEDELETE( m_pCharIcon );
	m_pCharIcon     = g_MyInfo.GetSoldierIcon( rkCharInfo.m_class_type, ( ( rkCharInfo.m_sex - 1 ) == RDT_HUMAN_MAN ) );

	m_nGradeType = g_UIImageRenderMgr.GetPowerUpGradeType( m_iClassType );

	ioHashString szClassName = g_MyInfo.GetClassName( rkCharInfo.m_class_type, rkCharInfo.m_eCharPowerUpType );
	m_AgreeList[0].SetTextStyle( TS_NORMAL );
	m_AgreeList[0].SetBkColor( TCT_DEFAULT_WHITE );
	m_AgreeList[0].SetTextColor( TCT_DEFAULT_GRAY );
	m_AgreeList[0].AddTextPiece( FONT_SIZE_12, STR(3), szClassName.c_str() );


	enum { MAX_DECO_ITEM = 6, };
	int iDecoTypeList[MAX_DECO_ITEM]={ UID_KINDRED, UID_HAIR, UID_HAIR_COLOR, UID_FACE, UID_SKIN_COLOR, UID_UNDERWEAR };
	int iDecoCodeList[MAX_DECO_ITEM]={ (int)GetRaceDetailType( rkCharInfo ), rkCharInfo.m_hair, rkCharInfo.m_hair_color, rkCharInfo.m_face, rkCharInfo.m_skin_color, rkCharInfo.m_underwear };
	for(int i = 0;i < MAX_DECO_ITEM;i++)
	{
		const DecoData &rkDeco = g_DecorationPrice.GetDecoData( rkCharInfo.m_sex - 1, iDecoTypeList[i], iDecoCodeList[i] ); 

		m_AgreeList[i+1].SetTextStyle( TS_NORMAL );
		m_AgreeList[i+1].SetBkColor( TCT_DEFAULT_WHITE );
		m_AgreeList[i+1].SetTextColor( TCT_DEFAULT_DARKGRAY );
		m_AgreeList[i+1].AddTextPiece( FONT_SIZE_12, STR(4), rkDeco.m_szName.c_str() );
	}

	//���ʽ� ���
	if( iKeepBonusPeso > 0 )
	{
		char szConvertNum[MAX_PATH] = "";
		Help::ConvertNumToStrComma( iKeepBonusPeso, szConvertNum, sizeof( szConvertNum ) );
		SafeSprintf( szBuf, sizeof( szBuf ), STR(2), szConvertNum );

		m_AgreeList[7].SetTextStyle( TS_NORMAL );
		m_AgreeList[7].SetBkColor( TCT_DEFAULT_WHITE );
		m_AgreeList[7].SetTextColor( TCT_DEFAULT_BLUE );
		m_AgreeList[7].AddTextPiece( FONT_SIZE_12, STR(5), szBuf );
	}
}

void PackageItemBuyResultWnd::SetResultDecorationPackage( CHARACTER &rkCharInfo, int iKeepBonusPeso )
{
	for( int i=0; i < MAX_AGREE_LIST; ++i )
	{
		m_AgreeList[i].ClearList();
	}

	ShowWnd();

	char szBuf[MAX_PATH] = "";

	// Title
	SafeSprintf( szBuf, sizeof( szBuf ), STR(1) );
	m_TitleName = szBuf;

	// �뺴 Ÿ��
	m_iClassType = rkCharInfo.m_class_type;

	SAFEDELETE( m_pCharIcon );
	m_pCharIcon = g_MyInfo.GetSoldierIcon( rkCharInfo.m_class_type, ( ( rkCharInfo.m_sex - 1 ) == RDT_HUMAN_MAN ) );

	m_nGradeType = g_UIImageRenderMgr.GetPowerUpGradeType( m_iClassType );

	ioHashString szClassName = g_MyInfo.GetClassName( rkCharInfo.m_class_type, rkCharInfo.m_eCharPowerUpType );
	m_AgreeList[0].SetTextStyle( TS_NORMAL );
	m_AgreeList[0].SetBkColor( TCT_DEFAULT_WHITE );
	m_AgreeList[0].SetTextColor( TCT_DEFAULT_GRAY );
	m_AgreeList[0].AddTextPiece( FONT_SIZE_12, STR(3), szClassName.c_str() );

	enum { MAX_DECO_ITEM = 6, };
	int iDecoTypeList[MAX_DECO_ITEM]={ UID_KINDRED, UID_HAIR, UID_HAIR_COLOR, UID_FACE, UID_SKIN_COLOR, UID_UNDERWEAR };
	int iDecoCodeList[MAX_DECO_ITEM]={ (int)GetRaceDetailType( rkCharInfo ), rkCharInfo.m_hair, rkCharInfo.m_hair_color, rkCharInfo.m_face, rkCharInfo.m_skin_color, rkCharInfo.m_underwear };
	for(int i = 0;i < MAX_DECO_ITEM;i++)
	{
		const DecoData &rkDeco = g_DecorationPrice.GetDecoData( rkCharInfo.m_sex - 1, iDecoTypeList[i], iDecoCodeList[i] ); 

		m_AgreeList[i+1].SetTextStyle( TS_NORMAL );
		m_AgreeList[i+1].SetBkColor( TCT_DEFAULT_WHITE );
		m_AgreeList[i+1].SetTextColor( TCT_DEFAULT_DARKGRAY );
		m_AgreeList[i+1].AddTextPiece( FONT_SIZE_12, STR(4), rkDeco.m_szName.c_str() );
	}

	//���ʽ� ���
	if( iKeepBonusPeso > 0 )
	{
		char szConvertNum[MAX_PATH] = "";
		Help::ConvertNumToStrComma( iKeepBonusPeso, szConvertNum, sizeof( szConvertNum ) );
		SafeSprintf( szBuf, sizeof( szBuf ), STR(2), szConvertNum );

		m_AgreeList[7].SetTextStyle( TS_NORMAL );
		m_AgreeList[7].SetBkColor( TCT_DEFAULT_WHITE );
		m_AgreeList[7].SetTextColor( TCT_DEFAULT_BLUE );
		m_AgreeList[7].AddTextPiece( FONT_SIZE_12, STR(5), szBuf );
	}
}

void PackageItemBuyResultWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
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
				pInvenWnd->ShowSoldierTabDirect( m_iClassType );
			}
		}
		break;
	}
}

void PackageItemBuyResultWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	enum
	{
		ICON_X		= 129,
		ICON_Y		= 105,

		TITLE_X		= 129,
		TITLE_Y		= 152,

		LIST_X		= 27,
		LIST_Y		= 206,
		LIST_GAP	= 21,

		FINAL_DESC_X	= 129,
		FINAL_DESC_Y	= 421,
	};

	if( m_pBackEffect )
		m_pBackEffect->Render( iXPos+ICON_X, iYPos+ICON_Y, UI_RENDER_MULTIPLY );

	g_UIImageRenderMgr.RenderGradeIconBack( m_nGradeType, iXPos+ICON_X, iYPos+ICON_Y );

	if( m_pCharIcon )
		m_pCharIcon->Render( iXPos+ICON_X, iYPos+ICON_Y );

	// List
	for( int i=0; i < MAX_AGREE_LIST; ++i )
	{
		if( m_AgreeList[i].IsEmpty() )
			continue;

		m_AgreeList[i].PrintFullText( iXPos+LIST_X, iYPos+LIST_Y+(LIST_GAP*i), TAT_LEFT );
	}

	// Title
	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
	g_FontMgr.PrintText( iXPos+TITLE_X, iYPos+TITLE_Y, FONT_SIZE_17, STR(1), m_TitleName.c_str() );

	// Last
	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetTextColor( TCT_DEFAULT_GREEN );
	g_FontMgr.PrintText( iXPos+FINAL_DESC_X, iYPos+FINAL_DESC_Y, FONT_SIZE_17, STR(2) );
}

