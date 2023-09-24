//----------------------------------------------------------------------------------
/*
Filename	: ShopBuyWnd
Author		: sumin
Date		: 113.11.5_14:41:3
*/
//----------------------------------------------------------------------------------

#include "StdAfx.h"
#include "ShopBuyWnd.h"
#include "ioButtonWndEX.h"
#include "ioLabelWndEX.h"
#include "ioFrameWndEX.h"
#include "../ioPresentMgr.h"
#include "../ioDecorationPrice.h"

//----------------------------------------------------------------------------------
/*
생성, 파괴
*/
ShopBuyWnd::ShopBuyWnd()
	: ShopBuyWndBase()
{
}

ShopBuyWnd::~ShopBuyWnd()
{
}

//----------------------------------------------------------------------------------
/*
control
*/
void	ShopBuyWnd::SetItemInfo( const ShopItemInfoEX& info )
{
	m_iteminfo = info;

	if ( m_iconimg ) {
		m_iconimg->SetImageSet( m_iteminfo.m_sIcon.c_str() );
	}

	if ( m_namerich )
	{
		m_namerich->ClearText();
		
		ioHashString desc = g_PresentMgr.GetPresentValue2Text( m_iteminfo.m_iValue0, m_iteminfo.m_iValue1, m_iteminfo.m_iValue2 );
		m_namerich->AddText( 17, TS_NORMAL, 0xFF000000, Format( "%s#", desc.c_str() ) );

	//	switch( m_iteminfo.m_iItemType )
	//	{
	//	case PRESENT_SOLDIER:
	//		break;

	//	case PRESENT_DECORATION:
	//		{
	//			int sextype = 0;
	//			if ( !m_iteminfo.m_vValue3.empty() ) {
	//				sextype = m_iteminfo.m_vValue3[ 0 ];
	//			}
	//			const DecoData& decodata = g_DecorationPrice.GetDecoData( sextype, m_iteminfo.m_iValue1, m_iteminfo.m_iValue2 );

	//			decodata.

	//			szReturn = kData.m_szName;
	//		}
	//		break;

	//	case PRESENT_ETC_ITEM:
	//		{
	//			ioEtcItem *pEtcItem = g_EtcItemMgr.FindEtcItem( iPresentValue1 );
	//			if( !pEtcItem )
	//				szReturn = "None";
	//			else
	//				szReturn = pEtcItem->GetName();
	//		}
	//		break;

	//	case PRESENT_EXTRAITEM:
	//		{
	//			const ioItem *pItem = g_ItemMaker.GetItemConst( iPresentValue1, __FUNCTION__ );
	//			if( pItem )	
	//				szReturn = pItem->GetName();
	//			else
	//				szReturn = "None";
	//		}
	//		break;

	//	case PRESENT_EXTRAITEM_BOX:
	//		{
	//			RandomMachineInfo *pInfo = g_ExtraItemInfoMgr.GetRandomMachineInfo( iPresentValue1 );
	//			if( pInfo )
	//				szReturn = pInfo->GetName();
	//			else
	//				szReturn = "None";
	//		}
	//		break;

	//	case PRESENT_RANDOM_DECO:
	//		{
	//			int iClassType = iPresentValue1 / 100000;
	//			int iSexType   = (iPresentValue1 % 100000) / FLOAT1000;
	//			int iDecoType  = iPresentValue1 % 1000;
	//			const DecoData &kData = g_DecorationPrice.GetDecoData( iSexType, iDecoType, iPresentValue2 );

	//			char szTemp[MAX_PATH] = "";
	//			if( iDecoType == UID_KINDRED )
	//			{
	//				SafeSprintf( szTemp, sizeof( szTemp), STR(2), kData.m_szName.c_str() );
	//			}
	//			else
	//			{
	//				if( iSexType == 0 )
	//					SafeSprintf( szTemp, sizeof( szTemp), STR(3), kData.m_szName.c_str() );
	//				else
	//					SafeSprintf( szTemp, sizeof( szTemp), STR(4), kData.m_szName.c_str() );
	//			}

	//			szReturn = szTemp;
	//		}
	//		break;

	//	case PRESENT_GRADE_EXP:
	//		{
	//			szReturn = STR(5);
	//		}
	//		break;

	//	case PRESENT_MEDALITEM:
	//		{
	//			const ioMedalItemInfoManager::ItemInfo *pItem = g_MedalItemMgr.GetItemInfo( iPresentValue1 );
	//			if( pItem )	
	//				szReturn = pItem->m_sName;
	//			else
	//				szReturn = "None";
	//		}
	//		break;

	//	case PRESENT_ALCHEMIC_ITEM:
	//		{
	//			if( iPresentValue1 > ALCHEMIC_ADDITIVE_DIV )
	//				szReturn = g_AlchemicMgr.GetAlchemicItemName(iPresentValue1);
	//			else
	//			{
	//				//szReturn = "조각";
	//				char szBuf[MAX_PATH] = "";
	//				wsprintf_e( szBuf, "%s 조각", g_MyInfo.GetClassName(iPresentValue1-1) );
	//				szReturn = szBuf;
	//			}
	//		}
	//		break;
	//	}

	//	switch ( m_iteminfo.m_iItemType )
	//	{
	//	case PRESENT_SOLDIER:
	//		m_namerich->AddText( 17, TS_NORMAL, 0xFF000000, Format( "%s#", m_iteminfo.m_sName.c_str() ) );
	//		m_namerich->AddText( 17, TS_NORMAL, 0xFF000000, Format( "%s#", m_iteminfo.m_sName.c_str() ) );
	//		m_namerich->AddText( 17, TS_NORMAL, 0xFF000000, Format( "%s#", m_iteminfo.m_sName.c_str() ) );
	//		break;

	//	case PRESENT_DECORATION:
	//PRESENT_ETC_ITEM           = 3,
	//PRESENT_PESO               = 4,
	//PRESENT_EXTRAITEM          = 5,
	//PRESENT_EXTRAITEM_BOX      = 6,
	//PRESENT_RANDOM_DECO		   = 7,
	//PRESENT_GRADE_EXP          = 8,
	//PRESENT_MEDALITEM          = 9,
	//PRESENT_ALCHEMIC_ITEM	   = 10,
	//PRESENT_MAX				   = 11,
		
	}

	if ( m_descrich )
	{
		m_descrich->ClearText();
		m_descrich->AddText( 12, TS_NORMAL, 0xFF0F0F0F, STR(1) );
	}
}

//----------------------------------------------------------------------------------
/*
Event Virtual Functions
*/
void	ShopBuyWnd::OnClickBuy( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
}

void	ShopBuyWnd::OnClickClose( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
	HideWnd();
}

void	ShopBuyWnd::OnClickPresent( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
}