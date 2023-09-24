#include "stdafx.h"
#include "ioSuperGashaponMgr.h"
#include "GUI/SuperGashaponWnd.h"

template<> ioSuperGashaponMgr* Singleton< ioSuperGashaponMgr >::ms_Singleton = 0;
ioSuperGashaponMgr& ioSuperGashaponMgr::GetSingleton()
{
	return Singleton< ioSuperGashaponMgr >::GetSingleton();
}

ioSuperGashaponMgr::ioSuperGashaponMgr()
{
	
}

ioSuperGashaponMgr::~ioSuperGashaponMgr()
{
}

void ioSuperGashaponMgr::Initialize()
{
	m_SuperGashaponPackageInfoMap.clear();
	ioINILoader_e kLoader( "config/sp2_super_gashapon_present.ini" );

	kLoader.SetTitle_e( "common" );
	int iMaxInfo = kLoader.LoadInt_e( "MaxInfo", 0 );
	if( iMaxInfo == 0 )
		return;

	for( int i = 1; i <= iMaxInfo; ++i )
	{
		sprintf_s_e( m_szBuffer, "GashaponPresent%d", i );
		kLoader.SetTitle( m_szBuffer );

		SuperGashaponPackageInfo Info;
		DWORD dwEtcItemIndex = (DWORD)kLoader.LoadInt_e( "EtcItemType", 0 );
		if( g_EtcItemMgr.FindEtcItem( dwEtcItemIndex ) )
		{
			LoadPackage( kLoader, Info );
			LoadRollingIcon( Info );
			LoadCostume( Info );
			LoadGrade( Info );
			m_SuperGashaponPackageInfoMap.insert( SuperGashaponPackageInfoMap::value_type( dwEtcItemIndex, Info ) );
		}
		else
		{
			if(ioLocalManager::GetLocalType() != ioLocalManager::LCT_INDONESIA)
				LOG.PrintTimeAndLog(0 ,"#SuperGashapon Error : [%s] inValid EtcItemType", m_szBuffer );
		}
	}
}

void ioSuperGashaponMgr::LoadPackage( ioINILoader& rkLoader, SuperGashaponPackageInfo& rkInfo )
{
	rkInfo.m_vSuperGashaponPackage.clear();
	int iMaxPackage = rkLoader.LoadInt_e( "MaxPackage", 0 );
	for( int i = 1; i <= iMaxPackage; ++i )
	{		
		SuperGashaponPackage Package;
		LoadElement( rkLoader, Package, i );
		rkInfo.m_vSuperGashaponPackage.push_back( Package );
	}

	iMaxPackage = rkLoader.LoadInt_e( "SubMaxPackage", 0 );
	for( int i = 1; i <= iMaxPackage; ++i )
	{		
		SuperGashaponPackage Package;
		LoadElement( rkLoader, Package, i, true );
		rkInfo.m_vSuperGashaponSubPackage.push_back( Package );
	}
}

void ioSuperGashaponMgr::LoadRollingIcon( SuperGashaponPackageInfo& rkInfo )
{
	rkInfo.m_vRollIconName.clear();	

	LoadRollingIconVec( rkInfo.m_vSuperGashaponPackage, rkInfo.m_vRollIconName );
	LoadRollingIconVec( rkInfo.m_vSuperGashaponSubPackage, rkInfo.m_vRollIconName );
}

void ioSuperGashaponMgr::LoadRollingIconVec( const vSuperGashaponPackage& vPackage, ioHashStringVec& vRollIconVec )
{
	vSuperGashaponPackage::const_iterator present_iter = vPackage.begin();
	for( ; present_iter != vPackage.end(); ++present_iter )
	{
		const SuperGashaponPackage& rkPackage = *present_iter;
		vSuperGashaponElement::const_iterator element_iter = rkPackage.m_vSuperGashaponElement.begin();
		for( ; element_iter != rkPackage.m_vSuperGashaponElement.end(); ++element_iter )
		{
			const SuperGashaponElement& rkElement = *element_iter;
			ioHashString szIconName = g_PresentMgr.GetPresentIconText( rkElement.m_iPresentType, rkElement.m_iPresentValue1, rkElement.m_iPresentValue2 );
			if( szIconName.IsEmpty() )
			{
				LOG.PrintTimeAndLog( 0, "%s icon empty : %d", __FUNCTION__, rkElement.m_iPresentValue1 );
				continue;
			}
			else if( HasIcon( vRollIconVec, szIconName ) )
			{
				continue;
			}
			vRollIconVec.push_back( szIconName );
		}
	}
}

void ioSuperGashaponMgr::LoadCostume( SuperGashaponPackageInfo& rkInfo )
{
	rkInfo.m_vCostume.clear();	

	LoadCostumeVec( rkInfo.m_vSuperGashaponPackage, rkInfo.m_vCostume );
	LoadCostumeVec( rkInfo.m_vSuperGashaponSubPackage, rkInfo.m_vCostume );
}

void ioSuperGashaponMgr::LoadCostumeVec( const vSuperGashaponPackage& vPackage, BoolVec& vCostumeVec )
{
	vSuperGashaponPackage::const_iterator present_iter = vPackage.begin();
	for( ; present_iter != vPackage.end(); ++present_iter )
	{
		const SuperGashaponPackage& rkPackage = *present_iter;
		vSuperGashaponElement::const_iterator element_iter = rkPackage.m_vSuperGashaponElement.begin();
		for( ; element_iter != rkPackage.m_vSuperGashaponElement.end(); ++element_iter )
		{
			const SuperGashaponElement& rkElement = *element_iter;
			if ( rkElement.m_iPresentType == PRESENT_COSTUME || rkElement.m_iPresentType == PRESENT_COSTUME_BOX )
				vCostumeVec.push_back( true );
			else
				vCostumeVec.push_back( false );
		}
	}
}

void ioSuperGashaponMgr::LoadGrade( SuperGashaponPackageInfo& rkInfo )
{
	rkInfo.m_vGrade.clear();	

	LoadGradeVec( rkInfo.m_vSuperGashaponPackage, rkInfo.m_vGrade );
	LoadGradeVec( rkInfo.m_vSuperGashaponSubPackage, rkInfo.m_vGrade );
}

void ioSuperGashaponMgr::LoadGradeVec( const vSuperGashaponPackage& vPackage, IntVec& vGradeVec )
{
	vSuperGashaponPackage::const_iterator present_iter = vPackage.begin();
	for( ; present_iter != vPackage.end(); ++present_iter )
	{
		const SuperGashaponPackage& rkPackage = *present_iter;
		vSuperGashaponElement::const_iterator element_iter = rkPackage.m_vSuperGashaponElement.begin();
		for( ; element_iter != rkPackage.m_vSuperGashaponElement.end(); ++element_iter )
		{
			const SuperGashaponElement& rkElement = *element_iter;
			int nGradeType = 0;
			switch( rkElement.m_iPresentType )
			{
			case PRESENT_SOLDIER:
				nGradeType = g_UIImageRenderMgr.GetPowerUpGradeType( rkElement.m_iPresentValue1 );
				break;
			case PRESENT_EXTRAITEM:
			case PRESENT_ACCESSORY:
				{
					const ioItem *pItem = g_ItemMaker.GetItemConst( rkElement.m_iPresentValue1, __FUNCTION__ );
					if( pItem )
						nGradeType = pItem->GetGradeType();
				}
				break;
			case PRESENT_EXTRAITEM_BOX:
				{
					RandomMachineInfo *pInfo = g_ExtraItemInfoMgr.GetRandomMachineInfo( rkElement.m_iPresentValue1 );
					if( pInfo )
						nGradeType = pInfo->GetGradeType();
				}
				break;
			case PRESENT_COSTUME:
				{
					nGradeType = g_CostumeInfoMgr.GetGradeType( rkElement.m_iPresentValue1 );
				}
				break;
			case PRESENT_COSTUME_BOX:
				{
					CostumeShopInfo *pInfo = g_CostumeShopInfoMgr.GetCostumeMachineInfo( rkElement.m_iPresentValue1 );
					if( pInfo )
						nGradeType = pInfo->GetGradeType();
				}
				break;
			}
			vGradeVec.push_back( nGradeType );
		}
	}
}

bool ioSuperGashaponMgr::HasIcon( const ioHashStringVec& szIconVec, const ioHashString& szIconName )
{
	if( szIconVec.empty() )
		return false;

	ioHashStringVec::const_iterator iter = szIconVec.begin();
	for( ; iter != szIconVec.end(); ++iter )
	{
		const ioHashString& rkIconName = *iter;
		if( strcmp( rkIconName.c_str(), szIconName.c_str() ) == 0 )
			return true;
	}

	return false;
}

void ioSuperGashaponMgr::LoadElement( ioINILoader& rkLoader, SuperGashaponPackage& rkPackage, int iPackage, bool bSubPackage )
{
	ZeroMemory( m_szBuffer, sizeof( m_szBuffer ) );
	if( bSubPackage )
		sprintf_s( m_szBuffer, "Sub" );
	
	sprintf_s_e( m_szKey, "%sPackage%d_Element_Cnt", m_szBuffer, iPackage );
	int iMaxElement = rkLoader.LoadInt( m_szKey, 0 );
	rkPackage.m_vSuperGashaponElement.clear();

	if( 0 < iMaxElement )
		rkPackage.m_vSuperGashaponElement.reserve( iMaxElement );

	sprintf_s_e( m_szKey, "%sPackage%d_Alaram", m_szBuffer, iPackage );
	rkPackage.m_bWholeAlarm = rkLoader.LoadBool( m_szKey, 0 );

	for( int i = 1; i <= iMaxElement; ++i )
	{
		sprintf_s_e( m_szKey, "%sPackage%d_Element%d_Alarm", m_szBuffer, iPackage, i );
		bool bAlarm = rkLoader.LoadBool( m_szKey, false );

		sprintf_s_e( m_szKey, "%sPackage%d_Element%d_Type", m_szBuffer, iPackage, i );
		int iType = rkLoader.LoadInt( m_szKey, 0 );

		sprintf_s_e( m_szKey, "%sPackage%d_Element%d_Value1", m_szBuffer, iPackage, i );
		int iValue1 = rkLoader.LoadInt( m_szKey, 0 );

		sprintf_s_e( m_szKey, "%sPackage%d_Element%d_Value2", m_szBuffer, iPackage, i );		
		int iValue2 = rkLoader.LoadInt( m_szKey, 0 );

		SuperGashaponElement Element;
		Element.m_iPresentType	 = iType;
		Element.m_iPresentValue1 = iValue1;
		Element.m_iPresentValue2 = iValue2;

		rkPackage.m_vSuperGashaponElement.push_back( Element );
	}
}

bool ioSuperGashaponMgr::IsSuperGashpon( DWORD dwEtcItemType )
{
	SuperGashaponPackageInfoMap::const_iterator info_iter = m_SuperGashaponPackageInfoMap.find( dwEtcItemType );
	if( info_iter == m_SuperGashaponPackageInfoMap.end() )
		return false;

	return true;
}

const ioSuperGashaponMgr::SuperGashaponPackage& ioSuperGashaponMgr::GetSuperGashaponPackage( DWORD dwEtcItemType, int iPackage )
{
	static SuperGashaponPackage kNone;
	if( iPackage < 0 )
		return kNone;

	SuperGashaponPackageInfoMap::iterator info_iter = m_SuperGashaponPackageInfoMap.find( dwEtcItemType );
	if( info_iter != m_SuperGashaponPackageInfoMap.end() )
	{
		const SuperGashaponPackageInfo& rkInfo = info_iter->second;

		if( iPackage < (int)rkInfo.m_vSuperGashaponPackage.size() )
			return rkInfo.m_vSuperGashaponPackage[iPackage];
	}

	return kNone;
}

const ioSuperGashaponMgr::SuperGashaponPackage& ioSuperGashaponMgr::GetSuperGashaponSubPackage( DWORD dwEtcItemType, int iPackage )
{
	static SuperGashaponPackage kNone;
	if( iPackage < 0 )
		return kNone;

	SuperGashaponPackageInfoMap::iterator info_iter = m_SuperGashaponPackageInfoMap.find( dwEtcItemType );
	if( info_iter != m_SuperGashaponPackageInfoMap.end() )
	{
		const SuperGashaponPackageInfo& rkInfo = info_iter->second;

		if( iPackage < (int)rkInfo.m_vSuperGashaponSubPackage.size() )
			return rkInfo.m_vSuperGashaponSubPackage[iPackage];
	}

	return kNone;
}

void ioSuperGashaponMgr::GetSuperGashaponPackageElement( DWORD dwEtcItemType, int iPackage, ioSuperGashaponMgr::vSuperGashaponElement& vElement )
{
	if( iPackage < 0 )
		return;

	SuperGashaponPackageInfoMap::iterator info_iter = m_SuperGashaponPackageInfoMap.find( dwEtcItemType );
	if( info_iter != m_SuperGashaponPackageInfoMap.end() )
	{
		const SuperGashaponPackageInfo rkInfo = info_iter->second;
				
		if( iPackage < (int)rkInfo.m_vSuperGashaponPackage.size() )
			vElement = rkInfo.m_vSuperGashaponPackage[iPackage].m_vSuperGashaponElement;
	}
}

const ioHashStringVec& ioSuperGashaponMgr::GetRollIconVec( DWORD dwEtcItemType )
{ 
	static ioHashStringVec szNoneVec;
	SuperGashaponPackageInfoMap::const_iterator info_iter = m_SuperGashaponPackageInfoMap.find( dwEtcItemType );
	if( info_iter != m_SuperGashaponPackageInfoMap.end() )
	{
		const SuperGashaponPackageInfo& rkInfo = info_iter->second;
		return rkInfo.m_vRollIconName;
	}

	return szNoneVec;
}

const BoolVec& ioSuperGashaponMgr::GetCostumeVec( DWORD dwEtcItemType )
{ 
	static BoolVec szNoneVec;
	SuperGashaponPackageInfoMap::const_iterator info_iter = m_SuperGashaponPackageInfoMap.find( dwEtcItemType );
	if( info_iter != m_SuperGashaponPackageInfoMap.end() )
	{
		const SuperGashaponPackageInfo& rkInfo = info_iter->second;
		return rkInfo.m_vCostume;
	}

	return szNoneVec;
}

const IntVec& ioSuperGashaponMgr::GetGradeVec( DWORD dwEtcItemType )
{ 
	static IntVec szNoneVec;
	SuperGashaponPackageInfoMap::const_iterator info_iter = m_SuperGashaponPackageInfoMap.find( dwEtcItemType );
	if( info_iter != m_SuperGashaponPackageInfoMap.end() )
	{
		const SuperGashaponPackageInfo& rkInfo = info_iter->second;
		return rkInfo.m_vGrade;
	}

	return szNoneVec;
}

bool ioSuperGashaponMgr::PrintSuperGashaponChatAlaram( const ioHashString& szSendID, DWORD dwEtcItemType, int iPackage )
{
	const SuperGashaponPackage& rkPackage = GetSuperGashaponPackage( dwEtcItemType, iPackage );
	
	if( rkPackage.m_vSuperGashaponElement.empty() )
	{
		LOG.PrintTimeAndLog( 0, "%s Element Empty : %d, %d", __FUNCTION__, dwEtcItemType, iPackage );
		return false;
	}

	ioHashString szEtcItemName;
	 ioEtcItem* pItem = g_EtcItemMgr.FindEtcItem( dwEtcItemType );	
	if( pItem )
	{
		szEtcItemName = pItem->GetName();
	}
	else
	{
		LOG.PrintTimeAndLog( 0, "%s not find etcitemtype : %d", __FUNCTION__, dwEtcItemType );
		return false;
	}
	
	//���� �̺κп��� EtcŸ�Ժ��� �޽��� �з�
	PrintDefaultAlaram( szSendID, rkPackage, szEtcItemName );

	return true;
}

void ioSuperGashaponMgr::PrintDefaultAlaram( const ioHashString& szSendID, const SuperGashaponPackage& rkPackage, const ioHashString& szEtcItemName )
{
	ioComplexStringPrinter kPrinter;
	g_ChatMgr.PresetChatComplexString( MSG_ANNOUNCE_CHAT, kPrinter );

	kPrinter.SetTextColor( TCT_DEFAULT_VIOLET );
	kPrinter.AddTextPiece( g_ChatMgr.GetChatFontScale(), szSendID.c_str() );

	kPrinter.SetTextColor( g_ChatMgr.GetChatColor() );
	kPrinter.AddTextPiece( g_ChatMgr.GetChatFontScale(), STR(1) );

	kPrinter.SetTextColor( TCT_DEFAULT_VIOLET );
	kPrinter.AddTextPiece( g_ChatMgr.GetChatFontScale(), STR(2), szEtcItemName.c_str() );

	kPrinter.SetTextColor( g_ChatMgr.GetChatColor() );
	kPrinter.AddTextPiece( g_ChatMgr.GetChatFontScale(), STR(3) );

	vSuperGashaponElement::const_iterator iter = rkPackage.m_vSuperGashaponElement.begin();
	if( iter != rkPackage.m_vSuperGashaponElement.end() )
	{
		const SuperGashaponElement& element = *iter;
		sprintf_s_e( m_szBuffer, "[%s]", g_PresentMgr.GetPresentValue1Text( element.m_iPresentType, element.m_iPresentValue1, element.m_iPresentValue2 ).c_str() );		

		if( rkPackage.m_vSuperGashaponElement.size() <= 1 )
		{
			kPrinter.SetTextColor( TCT_DEFAULT_VIOLET );
			kPrinter.AddTextPiece( g_ChatMgr.GetChatFontScale(), STR(4), m_szBuffer );
		}
		else
		{
			kPrinter.SetTextColor( TCT_DEFAULT_VIOLET );
			kPrinter.AddTextPiece( g_ChatMgr.GetChatFontScale(), STR(5), m_szBuffer, max( 0, rkPackage.m_vSuperGashaponElement.size() - 1 ) );
		}

		kPrinter.SetTextColor( g_ChatMgr.GetChatColor() );
		kPrinter.AddTextPiece( g_ChatMgr.GetChatFontScale(), STR(6) );
	}	
	else
	{
		LOG.PrintTimeAndLog( 0, "%s Element Empty", __FUNCTION__ );
	}	
	g_ChatMgr.SetChatComplexString( STR(7), kPrinter );
}

bool ioSuperGashaponMgr::SendAutoUse( DWORD dwEtcItemType, byte eType )
{
	ioEtcItemSuperGashapon* pEtcItem = dynamic_cast<ioEtcItemSuperGashapon*>( g_EtcItemMgr.FindEtcItem( dwEtcItemType ) );
	if( !pEtcItem )
	{
		LOG.PrintTimeAndLog( 0, "%s pEtcItem == NULL : %d", __FUNCTION__, dwEtcItemType );
		return false;
	}

	ioUserEtcItem *pUserEtcItem =  g_MyInfo.GetUserEtcItem();
	if( !pUserEtcItem )
	{
		LOG.PrintTimeAndLog( 0, "%s pUserEtcItem == NULL : %d", __FUNCTION__, dwEtcItemType );
		return false;
	}

	if( !pEtcItem->IsCanPresentUse() )
		return true;

	if( eType == EBRT_BUY )
	{
		pEtcItem->SetUseType( SUPERGASHPON_BUY );

		//�������� ���� ���
		if( !pEtcItem->OnUseBtnPush( dwEtcItemType, pUserEtcItem ) )
			return false;

		//�������� ���Ž� �ڵ����� ���
		NewShopWnd *pNewShopWnd = dynamic_cast<NewShopWnd*> ( g_GUIMgr.FindWnd( NEW_SHOP_WND ) );
		if( pNewShopWnd && pNewShopWnd->IsShow() )
		{
			//���԰��â Show
			SuperGashaponWnd *pSuperGashaponWnd = dynamic_cast<SuperGashaponWnd*> ( pNewShopWnd->FindChildWnd( NewShopWnd::ID_SUPERGASHAPON_WND ) );
			if( pSuperGashaponWnd )
				pSuperGashaponWnd->ShowSuperGashaponWnd( PRESENT_ETC_ITEM, dwEtcItemType, 0, SUPERGASHPON_BUY );

			//��������â Hide
			ioWnd *pBuyWnd = pNewShopWnd->FindChildWnd( NewShopWnd::ID_BUY_WND );
			if( pBuyWnd )
				pBuyWnd->HideWnd();
		}
	}

	return true;
}