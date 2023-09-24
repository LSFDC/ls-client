
#include "stdafx.h"

#include "ioItemRechargeManager.h"
#include "GUI/ItemRechargeWnd.h"

template<> ioItemRechargeManager* Singleton< ioItemRechargeManager >::ms_Singleton = 0;
ioItemRechargeManager::ioItemRechargeManager()
{
}

ioItemRechargeManager::~ioItemRechargeManager()
{
}

ioItemRechargeManager& ioItemRechargeManager::GetSingleton()
{
	return Singleton< ioItemRechargeManager >::GetSingleton();
}

void ioItemRechargeManager::LoadInIData()
{
	ioINILoader_e kLoader( "config/sp2_item_recharge.ini" );

	char szKey[MAX_PATH] = "";

	kLoader.SetTitle_e( "normal" );
	LoadPromotionInfo( kLoader, m_NormalItem );

	kLoader.SetTitle_e( "extra" );
	LoadPromotionInfo( kLoader, m_ExtraItem );

	kLoader.SetTitle_e( "rare" );
	LoadPromotionInfo( kLoader, m_RareItem );

	kLoader.SetTitle_e( "exception" );
	m_ExceptionInfoList.clear();
	int iCnt = kLoader.LoadInt_e( "exception_count", 0 );
	for( int i=0; i<iCnt; ++i )
	{
		wsprintf_e( szKey, "exception_item_code_%d", i+1 );
		int iCode = kLoader.LoadInt( szKey, 0 );
		wsprintf_e( szKey, "exception_promotion_time_%d", i+1 );
		int iLimit = kLoader.LoadInt( szKey, 0 );

		m_ExceptionInfoList.insert( mapExceptionInfo::value_type( iCode, iLimit ) );
	}
}

void ioItemRechargeManager::LoadPromotionInfo( ioINILoader &rkLoader, PromotionInfo &rkInfo )
{
	rkInfo.m_iWeapon = rkLoader.LoadInt_e( "promotion_weapon_time", 0 );
	rkInfo.m_iArmor  = rkLoader.LoadInt_e( "promotion_armor_time",  0 );
	rkInfo.m_iHelmet = rkLoader.LoadInt_e( "promotion_helmet_time", 0 );
	rkInfo.m_iCloak  = rkLoader.LoadInt_e( "promotion_cloak_time",  0 );
}

int ioItemRechargeManager::GetPromotionTime( int iItemCode )
{
	mapExceptionInfo::iterator iter = m_ExceptionInfoList.find( iItemCode );
	if( iter != m_ExceptionInfoList.end() )
	{
		return iter->second;
	}

	int iGroup = static_cast<int>(iItemCode/DEFAULT_BASIC_ITEM_CODE) + 1;
	int iExtra = static_cast<int>(iItemCode/10000) % 10;
	int iRare  = static_cast<int>(iItemCode/FLOAT1000) % 10;
	switch( iGroup )
	{
	case 1:
		{
			if( iRare > 0 )
				return m_RareItem.m_iWeapon;
			else if( iExtra > 0 )
				return m_ExtraItem.m_iWeapon;
			else
				return m_NormalItem.m_iWeapon;
		}
	case 2:
		{
			if( iRare > 0 )
				return m_RareItem.m_iArmor;
			else if( iExtra > 0 )
				return m_ExtraItem.m_iArmor;
			else
				return m_NormalItem.m_iArmor;
		}
	case 3:
		{
			if( iRare > 0 )
				return m_RareItem.m_iHelmet;
			else if( iExtra > 0 )
				return m_ExtraItem.m_iHelmet;
			else
				return m_NormalItem.m_iHelmet;
		}
	case 4:
		{
			if( iRare > 0 )
				return m_RareItem.m_iCloak;
			else if( iExtra > 0 )
				return m_ExtraItem.m_iCloak;
			else
				return m_NormalItem.m_iCloak;
		}
	}

	return 0;
}

void ioItemRechargeManager::ApplyRechargeExtraItem( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );
	int iState;
	rkPacket >> iState;

	switch( iState )
	{
	case EXTRA_ITEM_RECHARGE_TIME_SUCCESS:
		{
			int iSlotIndex, iValue1, iValue2, iPeriodType, iRechargeTime;
			rkPacket >> iSlotIndex >> iValue1 >> iValue2 >> iPeriodType >> iRechargeTime;

			ioUserExtraItem *pExtraItem = g_MyInfo.GetUserExtraItem();
			if( pExtraItem )
			{
				ioWnd *pWnd = g_GUIMgr.FindWnd(ITEM_RECHARGE_WND);
				ItemRechargeProcessWnd *pChild = NULL;
				if( pWnd )
				{
					pChild = (ItemRechargeProcessWnd*)( pWnd->FindChildWnd(ITEM_RECHARGE_PROCESS_WND) );
				}

				EXTRAITEMSLOT kTargetSlot;
				if( pExtraItem->GetExtraItem( iSlotIndex, kTargetSlot ) )
				{
					kTargetSlot.m_iValue1 = iValue1;
					kTargetSlot.m_iValue2 = iValue2;
					kTargetSlot.m_PeriodType = iPeriodType;
					pExtraItem->SetExtraItem( kTargetSlot );

					if( pChild && pChild->IsShow() )
						pChild->SetUIEndState( iRechargeTime );
				}
				else
				{
					LOG.PrintTimeAndLog( 0, "ioItemRechargeManager::ApplyRechargeExtraItem - invalid index : %d", iSlotIndex );

					if( pChild && pChild->IsShow() )
						pChild->HideWnd();
				}
			}

			ItemRechargeWnd *pItemRechargeWnd = (ItemRechargeWnd*)( g_GUIMgr.FindWnd(ITEM_RECHARGE_WND) );
			if( pItemRechargeWnd )
				pItemRechargeWnd->Update();
		}
		break;
	case EXTRA_ITEM_RECHARGE_TIME_FAIL:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		}
		break;
	case EXTRA_ITEM_RECHARGE_TIME_FAIL_UNKNOWN_ITEM:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		}
		break;
	case EXTRA_ITEM_RECHARGE_TIME_FAIL_IMPOSSIBLE:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		}
		break;
	}
}