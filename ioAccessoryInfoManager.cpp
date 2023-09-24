#include "StdAfx.h"
#include "ioAccessoryInfoManager.h"

#include "ioBaseChar.h"
#include "ioPlayStage.h"
#include "ioAlchemicMgr.h"
#include "Channeling/ioChannelingNodeManager.h"
#include "Channeling/ioChannelingNodeParent.h"

#include "GUI/MiniSoldierSelectWnd.h"
#include "GUI/SoldierSelectWnd.h"
#include "GUI/NewShopBoughtWnd.h"
#include "GUI/MyInventoryInfoWnd.h"

template<> ioAccessoryInfoManager* Singleton< ioAccessoryInfoManager >::ms_Singleton = 0;

ioAccessoryInfoManager::ioAccessoryInfoManager(void)
{
}


ioAccessoryInfoManager::~ioAccessoryInfoManager(void)
{
}

ioAccessoryInfoManager& ioAccessoryInfoManager::GetSingleton()
{
	return Singleton< ioAccessoryInfoManager >::GetSingleton();
}

void ioAccessoryInfoManager::LoadAccessoryInfo()
{
	ioINILoader_e kLoader( "config/sp2_accessory.ini" );
	kLoader.SetTitle_e( "common_info" );

	m_fMortmainItemSellPrice = kLoader.LoadFloat_e( "mortmain_item_sell", 0.0f );
	m_fTimeItemSellPrice = kLoader.LoadFloat_e( "time_item_sell", 0.0f );
}

void ioAccessoryInfoManager::OnAccessoryBuyResult( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );

	int iResult;

	__int64 iPeso;
	int iCash = 0;
	int nChannelingCash = 0;

	rkPacket >> iResult;

	switch( iResult )
	{
	case ITEM_BUY_OK:
		{
			int iAccessoryCode = 0;
			int iMachineCode = 0;
			int iPeriodTime = 0;

			OnAccessoryAdd( rkPacket, iAccessoryCode );

			rkPacket >> iMachineCode;
			rkPacket >> iPeriodTime >> iPeso >> iCash >> nChannelingCash;

			g_MyInfo.SetMoney( iPeso );
			g_MyInfo.SetCash( iCash );
			g_MyInfo.SetChannelingCash( nChannelingCash );

			// UI 처리	
			NewShopBoughtWnd *pNewShopBoughtWnd = NULL;
			NewShopWnd *pNewShopWnd = dynamic_cast<NewShopWnd*>( g_GUIMgr.FindWnd( NEW_SHOP_WND ) );
			if( pNewShopWnd )
				pNewShopBoughtWnd = dynamic_cast<NewShopBoughtWnd*>( pNewShopWnd->FindChildWnd( NewShopWnd::ID_BOUGHT_WND ) );
			
			if( pNewShopBoughtWnd && pNewShopBoughtWnd->IsShow() )
				pNewShopBoughtWnd->SetResultInfo( PRESENT_ACCESSORY, iAccessoryCode, iPeriodTime, false, EET_DISABLE );
		}
		return;
	case ITEM_BUY_NON_EXIST_GOODS:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		}
		break;
	case ITEM_BUY_PESO_SHORTAGE:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		}
		break;
	}

	MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>( g_GUIMgr.FindWnd( MY_INVENTORY_WND ) );
	if( pInvenWnd && pInvenWnd->IsShow() )
		pInvenWnd->UpdateTab( false, true );
}

void ioAccessoryInfoManager::OnAccessorySellResult( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );

	byte eResult;
	rkPacket >> eResult;

	if( eResult == ACCESSORY_SELL_OK )
	{
		int iSlotIndex = 0;
		int iAddPeso = 0;
		__int64 iPeso;
		rkPacket >> iSlotIndex;
		rkPacket >> iAddPeso;
		rkPacket >> iPeso;

		g_MyInfo.SetMoney( iPeso );

		ioHashString szItemName;
		ioHashString szIconName;
		bool bCustom = false;
		int iGradeType = 0;

		ioUserAccessory *pAccessory = g_MyInfo.GetUserAccessory();
		if( pAccessory )
		{
			AccessorySlot kSlot;
			if( pAccessory->GetAccessorySlot( iSlotIndex, kSlot ) )
			{
				szItemName = kSlot.m_Name;
				szIconName = kSlot.m_IconName;

				if( kSlot.m_dwMaleCustom > 0 || kSlot.m_dwFemaleCustom > 0 )
					bCustom = true;

				const ioItem* pItem = g_ItemMaker.GetItemConst( kSlot.m_iAccessoryCode, __FUNCTION__ );
				if ( pItem )
					iGradeType = pItem->GetGradeType();
			}
		}

		// 데이터 삭제
		if( !g_MyInfo.DeleteAccessory( iSlotIndex ) )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
			return;
		}

		// UI 처리
		MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*> ( g_GUIMgr.FindWnd( MY_INVENTORY_WND ) );
		if( pInvenWnd && pInvenWnd->IsShow() )
		{
			pInvenWnd->UpdateTab( false, true );

			char szConvertNum[MAX_PATH] = "";
			Help::ConvertNumToStrComma( iAddPeso, szConvertNum, sizeof( szConvertNum ) );

			ComplexStringPrinterVec vTitle;
			ComplexStringPrinterVec vDesc;

			ioComplexStringPrinter kTitle1;
			kTitle1.SetTextStyle( TS_NORMAL );
			kTitle1.SetBkColor( 0, 0, 0 );
			kTitle1.SetTextColor( TCT_DEFAULT_DARKGRAY );
			kTitle1.AddTextPiece( FONT_SIZE_17, STR(2), szItemName.c_str() );
			vTitle.push_back( kTitle1 );

			ioComplexStringPrinter kTitle2;
			kTitle2.SetTextStyle( TS_NORMAL );
			kTitle2.SetBkColor( 0, 0, 0 );
			kTitle2.SetTextColor( TCT_DEFAULT_DARKGRAY );
			kTitle2.AddTextPiece( FONT_SIZE_17, STR(3), szConvertNum );
			vTitle.push_back( kTitle2 );

			ioComplexStringPrinter kDesc1;
			kDesc1.SetTextStyle( TS_NORMAL );
			kDesc1.SetBkColor( 0, 0, 0 );	
			kDesc1.SetTextColor( TCT_DEFAULT_DARKGRAY );
			kDesc1.AddTextPiece( FONT_SIZE_12, STR(4), szItemName.c_str() );
			vDesc.push_back( kDesc1 ); 

			ioComplexStringPrinter kDesc2;
			kDesc2.SetTextStyle( TS_NORMAL );
			kDesc2.SetBkColor( 0, 0, 0 );
			kDesc2.SetTextColor( TCT_DEFAULT_DARKGRAY );
			kDesc2.AddTextPiece( FONT_SIZE_12, STR(5), szConvertNum );
			vDesc.push_back( kDesc2 );

			pInvenWnd->ShowItemRecvSellInfoWnd( ItemRecvSellInfoWnd::ITEM_SELL, ItemRecvSellInfoWnd::ITEM_ACCESSORY, szIconName, vTitle, vDesc, -1, 0, bCustom, false, iGradeType );
		}
	}
	else
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6), eResult );
}

void ioAccessoryInfoManager::OnAccessoryPresentResult( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );

	int iAccessoryCode = 0;
	OnAccessoryAdd( rkPacket, iAccessoryCode );
}

void ioAccessoryInfoManager::OnAccessoryAdd( SP2Packet &rkPacket, int &iCode )
{
	int iAccessoryCode = 0;
	int iSlotIndex = 0;
	byte btPeriodType = 0;
	int iValue1 = 0;
	int iValue2 = 0;
	int iRandomNum=0;
	rkPacket >> iAccessoryCode >> iSlotIndex;
	rkPacket >> btPeriodType;
	rkPacket >> iValue1 >> iValue2;
	rkPacket >> iRandomNum;

	ioUserAccessory *pAccessory = g_MyInfo.GetUserAccessory();
	if( pAccessory )
	{
		AccessorySlot kSlot;
		kSlot.m_iAccessoryCode = iAccessoryCode;
		kSlot.m_iIndex = iSlotIndex;
		kSlot.m_PeriodType = btPeriodType;
		kSlot.m_iValue1 = iValue1;
		kSlot.m_iValue2 = iValue2;
		kSlot.m_iRandomNumber = iRandomNum;

#ifdef __DEV_QA__
		LOG.PrintTimeAndLog( 0, "Accessory Idx : %d, code : %d , RandomNum : %d ", iSlotIndex, iAccessoryCode, iRandomNum );
#endif
		

		pAccessory->AddAccessorySlotData( kSlot );

		iCode = iAccessoryCode;
	}
}

void ioAccessoryInfoManager::OnAccessoryChange( SP2Packet &rkPacket, ioPlayStage* pPlayStage )
{
	TCPNetwork::MouseBusy( false );
	
	if ( !pPlayStage )
		return;

	byte eResult;
	rkPacket >> eResult;

	ioHashString szOwner;
	rkPacket >> szOwner;
	
	if ( eResult == ACCESSORY_CHANGE_SUCCESS )
	{
		int iCharIndex, iSlotIndex, iAccessoryCode, iRandomNum;
		byte iSlot;
		bool bEquip;
		rkPacket >> iCharIndex;
		rkPacket >> iSlot;
		rkPacket >> iSlotIndex;
		rkPacket >> bEquip;
		rkPacket >> iAccessoryCode;
		rkPacket >> iRandomNum;
		// 교체 대상이 자신일때
		if( szOwner == g_MyInfo.GetPublicID() )
		{
			int iCharArray = g_MyInfo.GetCharArray( iCharIndex );
			g_MyInfo.ApplyChangeAccessory( iCharArray, iSlot, iSlotIndex, bEquip );
			g_MyInfo.SetUserInfoMgr();

			// UI 갱신
			MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(g_GUIMgr.FindWnd(MY_INVENTORY_WND));
			if( pInvenWnd && pInvenWnd->IsShow() )
				pInvenWnd->UpdateTab( false, true );

			SoldierSelectWnd *pSoldierWnd = dynamic_cast<SoldierSelectWnd*>(g_GUIMgr.FindWnd( SOLDIER_SELECT_WND ));
			if( pSoldierWnd && pSoldierWnd->IsShow() )
				pSoldierWnd->SettingSoldierBtn();

			MiniSoldierSelectWnd *pMiniSoldierWnd = dynamic_cast<MiniSoldierSelectWnd*>( g_GUIMgr.FindWnd( MINI_SOLDIER_SELECT_WND ));
			if( pMiniSoldierWnd && pMiniSoldierWnd->IsShow() )
				pMiniSoldierWnd->SettingMiniSoldierBtn();

			if( pPlayStage->GetModeType() == MT_MYROOM )
			{
				ioPlayMode* pPlayMode = pPlayStage->GetPlayMode();
				if( pPlayMode )
					pPlayMode->SetChangeExtraItem( iCharArray, iSlot );
				return;
			}
		}

		// 아이템 다시 장착 (현재 슬롯에 있는 아이템을 복사)
		if( pPlayStage->GetModeType() != MT_MYROOM )
		{
			// 필드에 출전중인 캐릭터면 장비 교체
			ioBaseChar *pOwner = pPlayStage->GetBaseChar( szOwner );
			bool bSelectChar = true;
			if( pOwner && pOwner->IsOwnerChar() && ( g_MyInfo.GetSelectCharArray() != g_MyInfo.GetCharArray( iCharIndex ) ) )
				bSelectChar = false;
			
			if( pOwner && bSelectChar && !pPlayStage->IsModeStateResult() )
			{
				if ( !bEquip )
				{
					int iEquipAccessorySlot = iSlot + ES_RING;
					pOwner->SetEquipAccessoryInfo( iSlot, 0, 0, 0 );
					pOwner->GetEquipSlot()->ReleaseItem( (EquipSlot)iEquipAccessorySlot );
				}
				else
				{
					pOwner->SetEquipAccessoryInfo( iSlot, iSlotIndex, iAccessoryCode, iRandomNum );
					ioItem *pNewItem = pPlayStage->CreateItem( iAccessoryCode );
					if ( pNewItem )
					{
						pNewItem->SetOwnerName( szOwner );
						pNewItem->SetItemCreateIndex( iSlotIndex );
						pNewItem->InitializeBeforeEquip( szOwner, NULL );
						pNewItem->SetAccessoryRandomNum( iRandomNum );
						ioItem *pPreItem = pOwner->EquipItem( pNewItem, true );
						if ( pPreItem )
							SAFEDELETE( pPreItem );
					}
				}

				if( pOwner->GetState() == CS_EXTRAITEM_CHANGE )
					pOwner->SetState( CS_DELAY );
				
			}
		}
	}
	else if ( eResult == STPK_DELETE_ACCESSORY_DATE )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return;
	}
	else
	{
		ioBaseChar *pOwner = pPlayStage->GetBaseChar( szOwner );
		if( pPlayStage->GetModeType() != MT_MYROOM && !pOwner )
			return;

		if( pOwner && pOwner->IsOwnerChar() )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2), eResult );

			if( pOwner->GetState() == CS_EXTRAITEM_CHANGE )
				pOwner->SetState( CS_DELAY );
		}
	}
	
}

const float& ioAccessoryInfoManager::GetResellMortmainAccessoryPeso() const
{
	return m_fMortmainItemSellPrice; 
}

const float& ioAccessoryInfoManager::GetResellTimeAccessoryTimePeso() const
{
	return m_fTimeItemSellPrice;
}
