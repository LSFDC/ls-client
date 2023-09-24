
#include "stdafx.h"

#include "ioCostumeInfoManager.h"

#include "ioBaseChar.h"
#include "ioPlayStage.h"

#include "ioCollisionBoxGrpManager.h"
#include "ioAlchemicMgr.h"
#include "Channeling/ioChannelingNodeManager.h"
#include "Channeling/ioChannelingNodeParent.h"

#include "GUI/MiniSoldierSelectWnd.h"
#include "GUI/SoldierSelectWnd.h"
#include "GUI/NewShopBoughtWnd.h"
#include "GUI/MyInventoryInfoWnd.h"

template<> ioCostumeInfoManager* Singleton< ioCostumeInfoManager >::ms_Singleton = 0;

ioCostumeInfoManager::ioCostumeInfoManager()
{
	ClearData();
}

ioCostumeInfoManager::~ioCostumeInfoManager()
{
	ClearData();
}

ioCostumeInfoManager& ioCostumeInfoManager::GetSingleton()
{
	return Singleton< ioCostumeInfoManager >::GetSingleton();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ioCostumeInfoManager::ClearData()
{
	m_vEnableCostumeMode.clear();
	m_vDisableCostumeMode.clear();
	m_nCurModeType = 0;

	CostumeInfoList::iterator Iter = m_vInfoList.begin();
	for ( ; Iter!=m_vInfoList.end(); ++Iter )
	{
		CostumeInfo& sCostumeInfo = Iter->second;
		SAFEDELETE( sCostumeInfo.m_pCostumeSpringParam );
	}
	m_vInfoList.clear();
}

void ioCostumeInfoManager::LoadProperty()
{	
	ioINILoader_e kLoader( "config/sp2_costume.ini" );

	char szBuf[MAX_PATH] = "";
	char szTitle[MAX_PATH] = "";
	char szGroupName[MAX_PATH] = "";

	kLoader.SetTitle_e( "common_info" );

	int nEnableModeCnt = kLoader.LoadInt_e( "enable_mode_cnt", 0 );
	for( int i=0; i<nEnableModeCnt; ++i )
	{
		wsprintf_e( szBuf, "enable_mode%d", i+1 );
		int nEnableModeType = kLoader.LoadInt( szBuf, 0 );
		if ( nEnableModeType > 0 )
			m_vEnableCostumeMode.push_back( nEnableModeType );
	}

	int nDisableModeCnt = kLoader.LoadInt_e( "disable_mode_cnt", 0 );
	for( int i=0; i<nDisableModeCnt; ++i )
	{
		wsprintf_e( szBuf, "disable_mode%d", i+1 );
		int nDisableModeType = kLoader.LoadInt( szBuf, 0 );
		if ( nDisableModeType > 0 )
			m_vDisableCostumeMode.push_back( nDisableModeType );
	}

	int nGroupCnt = kLoader.LoadInt_e( "costume_group_cnt", 0 );
	for( int i=0; i<nGroupCnt; ++i )
	{
		wsprintf_e( szTitle, "costume_group%d", i+1 );
		kLoader.LoadString( szTitle, "", szBuf, MAX_PATH );
		wsprintf_e( szGroupName, "config/%s", szBuf );
		
		ioINILoader kSetCostumeLoader( szGroupName );
		kSetCostumeLoader.SetTitle_e( "common_info" );

		int iCnt = kSetCostumeLoader.LoadInt_e( "costume_cnt", 0 );
		for( int j=0; j < iCnt; ++j )
		{
			wsprintf_e( szTitle, "costume%d", j+1 );
			kSetCostumeLoader.SetTitle( szTitle );

			LoadBase( kSetCostumeLoader );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ioCostumeInfoManager::LoadBase( ioINILoader &rkLoader )
{
	char szBuf[MAX_PATH];

	int nCostumeCode = rkLoader.LoadInt_e( "costum_code", 0 );
	if ( nCostumeCode <= 0 )
		return;

	CostumeInfo sCostumeInfo;
	rkLoader.LoadString_e( "name", "", szBuf, MAX_PATH );
	sCostumeInfo.m_szName = szBuf;
	
	rkLoader.LoadString_e( "icon_name", "", szBuf, MAX_PATH );
	sCostumeInfo.m_szIconName = szBuf;

	if ( sCostumeInfo.m_szIconName.IsEmpty() )
		sCostumeInfo.m_szIconName = g_ItemMaker.GetItemLargeIcon( nCostumeCode, 0 );
	
	sCostumeInfo.m_nEnableClass = rkLoader.LoadInt_e( "enable_class", 0 );
	sCostumeInfo.m_nManual = rkLoader.LoadInt_e( "manual", 0 );
	sCostumeInfo.m_nDisassembleCode = rkLoader.LoadInt_e( "disasemble_code", 0 );
	sCostumeInfo.m_nDisassembleMinCnt = rkLoader.LoadInt_e( "disasemble_min", 0 );
	sCostumeInfo.m_nDisassembleMaxCnt = rkLoader.LoadInt_e( "disasemble_max", 0 );

	sCostumeInfo.m_nCostumeCode = nCostumeCode;
	sCostumeInfo.m_nGradeType = rkLoader.LoadInt_e( "grade_type", 0 );
	
	rkLoader.LoadString_e( "item_equip_effect", "", szBuf, MAX_PATH );
	sCostumeInfo.m_szEffect = szBuf;

	rkLoader.LoadString_e( "item_equip_effect_hw", "", szBuf, MAX_PATH );
	sCostumeInfo.m_szEffectHW = szBuf;

	rkLoader.LoadString_e( "spring_biped", "", szBuf, MAX_PATH );
	sCostumeInfo.m_szSpring_Biped = szBuf;

	if( !sCostumeInfo.m_szSpring_Biped.IsEmpty() )
	{
		sCostumeInfo.m_pCostumeSpringParam = new RopeSpringParam;
		sCostumeInfo.m_pCostumeSpringParam->m_fGravity    = 400.0f;
		sCostumeInfo.m_pCostumeSpringParam->m_fBoxWeights = 15.0f;

		sCostumeInfo.m_pCostumeSpringParam->m_fDamping    = 0.5f;
		sCostumeInfo.m_pCostumeSpringParam->m_fAngDamping = 0.5f;

		sCostumeInfo.m_pCostumeSpringParam->m_fLengthFixRate = 0.4f;

		SwingParam kSwing;
		kSwing.m_fHSwingDOWN = -45.0f;
		kSwing.m_fHSwingUP = 45.0f;
		kSwing.m_fVSwingDOWN = -80.0f;
		kSwing.m_fVSwingUP = 30.0f;
		sCostumeInfo.m_pCostumeSpringParam->m_SwingList.push_back( kSwing );

		kSwing.m_fVSwingDOWN = 10.0f;
		kSwing.m_fVSwingUP = 70.0f;
		sCostumeInfo.m_pCostumeSpringParam->m_SwingList.push_back( kSwing );
	}

	sCostumeInfo.m_GlowColor.r = rkLoader.LoadFloat_e( "skill_color_red", 0.0f ) / 255.0f;
	sCostumeInfo.m_GlowColor.g = rkLoader.LoadFloat_e( "skill_color_green", 0.0f ) / 255.0f;
	sCostumeInfo.m_GlowColor.b = rkLoader.LoadFloat_e( "skill_color_blue", 0.0f ) / 255.0f;
	sCostumeInfo.m_GlowColor.a = rkLoader.LoadFloat_e( "skill_color_alpha", 255.0f ) / 255.0f;

	switch ( sCostumeInfo.m_nCostumeCode/DEFAULT_BASIC_COSTUME_CODE )
	{
	case PCST_Armor:
		LoadArmor( rkLoader, sCostumeInfo );
		break;
	case PCST_Helmet:
		LoadHelmet( rkLoader, sCostumeInfo );
		break;
	case PCST_Cloak:
		LoadCloak( rkLoader, sCostumeInfo );
		break;
	}

	m_vInfoList.insert( std::make_pair( nCostumeCode, sCostumeInfo ) );
}

void ioCostumeInfoManager::LoadArmor( ioINILoader &rkLoader, CostumeInfo &sCostumeInfo )
{
	char szBuf[MAX_PATH];
	sCostumeInfo.m_nArmorType = rkLoader.LoadInt_e( "armor_type", 0 );
	sCostumeInfo.m_nArmorSoundType = rkLoader.LoadInt_e( "armor_sound_type", 0 );
	
	rkLoader.LoadString_e( "body_mesh", "", szBuf, MAX_PATH );
	sCostumeInfo.m_szBodyMesh = szBuf;
	rkLoader.LoadString_e( "hand_mesh", "", szBuf, MAX_PATH );
	sCostumeInfo.m_szHandMesh = szBuf;
}

void ioCostumeInfoManager::LoadHelmet( ioINILoader &rkLoader, CostumeInfo &sCostumeInfo )
{
	char szBuf[MAX_PATH];
	sCostumeInfo.m_nFaceHM = rkLoader.LoadInt_e( "face_change_hm", 0 );
	sCostumeInfo.m_nFaceHW = rkLoader.LoadInt_e( "face_change_hw", 0 );
	sCostumeInfo.m_nHairHM = rkLoader.LoadInt_e( "hair_change_hm", 0 );
	sCostumeInfo.m_nHairHW = rkLoader.LoadInt_e( "hair_change_hw", 0 );
	sCostumeInfo.m_nInvisibleFaceType = rkLoader.LoadInt_e( "invisible_face", 0 );

	rkLoader.LoadString_e( "helmet_mesh", "", szBuf, MAX_PATH );
	sCostumeInfo.m_szHelmetMesh = szBuf;	
}

void ioCostumeInfoManager::LoadCloak( ioINILoader &rkLoader, CostumeInfo &sCostumeInfo )
{
	char szBuf[MAX_PATH];
	rkLoader.LoadString_e( "cloak_mesh", "", szBuf, MAX_PATH );
	sCostumeInfo.m_szCloakMesh = szBuf;	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ioCostumeInfoManager::GetCostumeInfo( int nCostumeCode, CostumeInfo &sCostumeInfo )
{
	sCostumeInfo.Init();

	if ( nCostumeCode <= 0 || m_vInfoList.empty() )
		return false;

	int nSubType = nCostumeCode/DEFAULT_BASIC_COSTUME_CODE;
	if ( !COMPARE( nSubType, PCST_Armor, PCST_MAX ) )
		return false;

	CostumeInfoList::iterator FindIter = m_vInfoList.find( nCostumeCode );
	if ( FindIter != m_vInfoList.end() )
	{
		const CostumeInfo &rCostumeInfo = FindIter->second;
		if ( nSubType != rCostumeInfo.m_nCostumeCode/DEFAULT_BASIC_COSTUME_CODE )
			return false;

		sCostumeInfo = rCostumeInfo;
		return true;
	}

	return false;
}

ioHashString ioCostumeInfoManager::GetCostumeName( int nCostumeCode )
{
	static ioHashString szNull;

	if ( nCostumeCode <= 0 || m_vInfoList.empty() )
		return szNull;

	int nSubType = nCostumeCode/DEFAULT_BASIC_COSTUME_CODE;
	if ( !COMPARE( nSubType, PCST_Armor, PCST_MAX ) )
		return szNull;

	CostumeInfoList::iterator FindIter = m_vInfoList.find( nCostumeCode );
	if ( FindIter != m_vInfoList.end() )
	{
		const CostumeInfo &rCostumeInfo = FindIter->second;
		if ( nSubType != rCostumeInfo.m_nCostumeCode/DEFAULT_BASIC_COSTUME_CODE )
			return szNull;
		return rCostumeInfo.m_szName;
	}

	return szNull;
}

ioHashString ioCostumeInfoManager::GetCostumeIcon( int nCostumeCode )
{
	static ioHashString szNull;

	if ( nCostumeCode <= 0 || m_vInfoList.empty() )
		return szNull;

	int nSubType = nCostumeCode/DEFAULT_BASIC_COSTUME_CODE;
	if ( !COMPARE( nSubType, PCST_Armor, PCST_MAX ) )
		return szNull;

	CostumeInfoList::iterator FindIter = m_vInfoList.find( nCostumeCode );
	if ( FindIter != m_vInfoList.end() )
	{
		const CostumeInfo &rCostumeInfo = FindIter->second;
		if ( nSubType != rCostumeInfo.m_nCostumeCode/DEFAULT_BASIC_COSTUME_CODE )
			return szNull;
		return rCostumeInfo.m_szIconName;
	}

	return szNull;
}

int ioCostumeInfoManager::GetCostumeManual( int nCostumeCode )
{
	if ( nCostumeCode <= 0 || m_vInfoList.empty() )
		return 0;

	int nSubType = nCostumeCode/DEFAULT_BASIC_COSTUME_CODE;
	if ( !COMPARE( nSubType, PCST_Armor, PCST_MAX ) )
		return 0;

	CostumeInfoList::iterator FindIter = m_vInfoList.find( nCostumeCode );
	if ( FindIter != m_vInfoList.end() )
	{
		const CostumeInfo &rCostumeInfo = FindIter->second;
		if ( nSubType != rCostumeInfo.m_nCostumeCode/DEFAULT_BASIC_COSTUME_CODE )
			return 0;
		return rCostumeInfo.m_nManual;
	}

	return 0;
}

int ioCostumeInfoManager::GetGradeType( int nCostumeCode )
{
	int nGrade = 0;

	if ( nCostumeCode <= 0 || m_vInfoList.empty() )
		return nGrade;

	int nSubType = nCostumeCode/DEFAULT_BASIC_COSTUME_CODE;
	if ( !COMPARE( nSubType, PCST_Armor, PCST_MAX ) )
		return nGrade;

	CostumeInfoList::iterator FindIter = m_vInfoList.find( nCostumeCode );
	if ( FindIter != m_vInfoList.end() )
	{
		const CostumeInfo &rCostumeInfo = FindIter->second;
		if ( nSubType != rCostumeInfo.m_nCostumeCode/DEFAULT_BASIC_COSTUME_CODE )
			return nGrade;
		return rCostumeInfo.m_nGradeType;
	}

	return nGrade;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int ioCostumeInfoManager::GetDisassembleCode( int nCostumeCode )
{
	if ( nCostumeCode <= 0 || m_vInfoList.empty() )
		return 0;

	int nSubType = nCostumeCode/DEFAULT_BASIC_COSTUME_CODE;
	if ( !COMPARE( nSubType, PCST_Armor, PCST_MAX ) )
		return 0;

	CostumeInfoList::iterator FindIter = m_vInfoList.find( nCostumeCode );
	if ( FindIter != m_vInfoList.end() )
	{
		const CostumeInfo &rCostumeInfo = FindIter->second;
		if ( nSubType != rCostumeInfo.m_nCostumeCode/DEFAULT_BASIC_COSTUME_CODE )
			return 0;
		return rCostumeInfo.m_nDisassembleCode;
	}

	return 0;
}

int ioCostumeInfoManager::GetDisassembleMinCnt( int nCostumeCode, int nTotal )
{
	if ( nCostumeCode <= 0 || m_vInfoList.empty() )
		return 0;

	int nSubType = nCostumeCode/DEFAULT_BASIC_COSTUME_CODE;
	if ( !COMPARE( nSubType, PCST_Armor, PCST_MAX ) )
		return 0;

	CostumeInfoList::iterator FindIter = m_vInfoList.find( nCostumeCode );
	if ( FindIter != m_vInfoList.end() )
	{
		const CostumeInfo &rCostumeInfo = FindIter->second;
		if ( nSubType != rCostumeInfo.m_nCostumeCode/DEFAULT_BASIC_COSTUME_CODE )
			return 0;
		return (rCostumeInfo.m_nDisassembleMinCnt * nTotal * 1.0f);
	}

	return 0;
}

int ioCostumeInfoManager::GetDisassembleMaxCnt( int nCostumeCode, int nTotal )
{
	if ( nCostumeCode <= 0 || m_vInfoList.empty() )
		return 0;

	int nSubType = nCostumeCode/DEFAULT_BASIC_COSTUME_CODE;
	if ( !COMPARE( nSubType, PCST_Armor, PCST_MAX ) )
		return 0;

	CostumeInfoList::iterator FindIter = m_vInfoList.find( nCostumeCode );
	if ( FindIter != m_vInfoList.end() )
	{
		const CostumeInfo &rCostumeInfo = FindIter->second;
		if ( nSubType != rCostumeInfo.m_nCostumeCode/DEFAULT_BASIC_COSTUME_CODE )
			return 0;
		return (rCostumeInfo.m_nDisassembleMaxCnt * nTotal * 2.0f);
	}

	return 0;
}

bool ioCostumeInfoManager::IsEnableCostumeMode()
{
	if ( m_nCurModeType == MT_MYROOM )
		return true;

	for ( int i=0; i<(int)m_vEnableCostumeMode.size(); i++ )
	{
		if ( m_vEnableCostumeMode[i] == m_nCurModeType )
			return true;
	}

	return false;
}

bool ioCostumeInfoManager::IsDisableCostumeMode()
{
	for ( int i=0; i<(int)m_vDisableCostumeMode.size(); i++ )
	{
		if ( m_vDisableCostumeMode[i] == m_nCurModeType )
			return true;
	}

	return false;
}

void ioCostumeInfoManager::SetCurPlayMode( int nCurModeType )
{
	m_nCurModeType = nCurModeType;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ioCostumeInfoManager::OnCostumeBuyResult( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );

	int eResult;

	__int64 nPeso;
	int nCash = 0;
	int nChannelingCash = 0;

	rkPacket >> eResult;

	switch( eResult )
	{
	case ITEM_BUY_OK:
		{
			int nCostumeCode = 0;
			int nMachineCode = 0;
			int nPeriodTime = 0;
			//bool bAlarm = false;

			OnCostumeAdd( rkPacket, nCostumeCode );

			rkPacket >> nMachineCode; //>> bAlarm;
			rkPacket >> nPeriodTime >> nPeso >> nCash >> nChannelingCash;

			g_MyInfo.SetMoney( nPeso );
			g_MyInfo.SetCash( nCash );
			g_MyInfo.SetChannelingCash( nChannelingCash );

			// UI ó��	
			NewShopBoughtWnd *pNewShopBoughtWnd = NULL;
			NewShopWnd *pNewShopWnd = dynamic_cast<NewShopWnd*>( g_GUIMgr.FindWnd( NEW_SHOP_WND ) );
			if( pNewShopWnd )
				pNewShopBoughtWnd = dynamic_cast<NewShopBoughtWnd*>( pNewShopWnd->FindChildWnd( NewShopWnd::ID_BOUGHT_WND ) );
			
			if( eResult == ITEM_BUY_OK )
			{
				if( pNewShopBoughtWnd && pNewShopBoughtWnd->IsShow() )
					pNewShopBoughtWnd->SetResultInfo( PRESENT_COSTUME, nCostumeCode, nPeriodTime, false, EET_DISABLE );
			}
			else
			{
				MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>( g_GUIMgr.FindWnd( MY_INVENTORY_WND ) );
				if( pInvenWnd && pInvenWnd->IsShow() )
				{
					pInvenWnd->ShowCostumeBoxWnd( nMachineCode );
					pInvenWnd->OnCostumeBoxResultInfo( nCostumeCode, nPeriodTime );
					pInvenWnd->UpdateTab( false, false );
				}
			}
			/*
			// �ڽ�Ƭ ���� (�ڽ�Ƭ ����)
			if( bAlarm )
			{
				SP2Packet kPacket( CTPK_SERVER_ALARM_MSG );
				kPacket << UDP_SERVER_ALARM_COSTUME << g_MyInfo.GetPublicID() << nMachineCode << nCostumeCode << nPeriodTime;
				TCPNetwork::SendToServer( kPacket );
			}
			*/
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
		/*
	case COSTUME_PACKAGE_BUY_OK:
		{
			int nMachineCode = 0;
			rkPacket >> nPeso >> nCash >> nChannelingCash >> nMachineCode;

			g_MyInfo.SetMoney( nPeso );
			g_MyInfo.SetCash( nCash );
			g_MyInfo.SetChannelingCash( nChannelingCash );
			
			// UI ó��	
			NewShopBoughtWnd *pNewShopBoughtWnd = NULL;
			NewShopWnd *pNewShopWnd = dynamic_cast<NewShopWnd*> ( g_GUIMgr.FindWnd( NEW_SHOP_WND ) );
			if( pNewShopWnd )
				pNewShopBoughtWnd = dynamic_cast<NewShopBoughtWnd*> ( pNewShopWnd->FindChildWnd( NewShopWnd::ID_BOUGHT_WND ) );

			if( pNewShopBoughtWnd && pNewShopBoughtWnd->IsShow() )
				pNewShopBoughtWnd->SetResultInfo( PRESENT_COSTUME_BOX, nMachineCode, 0, false, EET_DISABLE );
		}
		return;
	case COSTUME_EXCEPTION:
		{
			int iErrorNum = 0;
			rkPacket >> iErrorNum;
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3), iErrorNum );
		}
		break;
	case COSTUME_PESO:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		}
		break;
	case COSTUME_PERIOD:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
		}
		break;
	case COSTUME_LEVEL:
		{
			int iNeedLevel;
			rkPacket >> iNeedLevel;

			char szHelp[MAX_PATH]="";
			g_LevelMgr.GetGradeName( iNeedLevel, szHelp, sizeof( szHelp ), false );
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6), szHelp );
		}
		break;
	case COSTUME_BILLING_WAIT:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(7) );
		}
		break;
	case COSTUME_BILLING_DISCONNECT:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(8) );	
		}
		break;
	case COSTUME_BILLING_FAIL:
		{
			bool bBillingError    = false;
			ioHashString  sBillingError;
			rkPacket >> bBillingError;
			if( bBillingError )
				rkPacket >> sBillingError;

			ioHashString sErrorMent = STR(9);
			if( !bBillingError )
				g_GUIMgr.SetMsgBox( MB_OK, NULL, sErrorMent.c_str() );
			else
			{
				ioHashString sError = "Billing Error : ";
				ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
				if( pLocal ) 
					sError = pLocal->GetBillingErrorMent(sBillingError);

				char szErrorFullMent[MAX_PATH]="";
				ioChannelingNodeParent *pNode = g_ChannelingMgr.GetNode( g_MyInfo.GetChannelingType() );
				if( pNode && pNode->GetType() == CNT_NEXON )					
					StringCbPrintf( szErrorFullMent, sizeof( szErrorFullMent ), "%s",  sError.c_str() );
				else
					StringCbPrintf( szErrorFullMent, sizeof( szErrorFullMent ), "%s#%s%s", sErrorMent.c_str(), sError.c_str(), sBillingError.c_str() );

				g_GUIMgr.SetMsgBox( MB_OK, NULL, szErrorFullMent );
			}
		}
		break;
	case COSTUME_BUY_BILLING_EXCESS_BUY:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(10) );
		}
		break;
		*/
	}

	MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>( g_GUIMgr.FindWnd( MY_INVENTORY_WND ) );
	if( pInvenWnd && pInvenWnd->IsShow() )
		pInvenWnd->UpdateTab( false, true );
}

void ioCostumeInfoManager::OnCostumeSellResult( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );

	byte eResult;
	rkPacket >> eResult;

	if( eResult == COSTUME_SELL_OK )
	{
		int nSlotIndex = 0;
		__int64 nAddPeso, nPeso;
		rkPacket >> nSlotIndex;
		rkPacket >> nAddPeso;
		rkPacket >> nPeso;

		g_MyInfo.SetMoney( nPeso );

		ioHashString szItemName;
		ioHashString szIconName;
		bool bCustom = false;
		int nGradeType = 0;

		ioUserCostume *pCostume = g_MyInfo.GetUserCostume();
		if( pCostume )
		{
			CostumeSlot kSlot;
			if( pCostume->GetCostumeSlot( nSlotIndex, kSlot ) )
			{
				szItemName = kSlot.m_Name;
				szIconName = kSlot.m_IconName;

				if( kSlot.m_dwMaleCustom > 0 || kSlot.m_dwFemaleCustom > 0 )
					bCustom = true;

				nGradeType = GetGradeType( kSlot.m_nCostumeCode );
			}
		}

		// ������ ����
		if( !g_MyInfo.DeleteCostume( nSlotIndex ) )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
			return;
		}

		// UI ó��
		MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*> ( g_GUIMgr.FindWnd( MY_INVENTORY_WND ) );
		if( pInvenWnd && pInvenWnd->IsShow() )
		{
			pInvenWnd->UpdateTab( false, true );

			char szConvertNum[MAX_PATH] = "";
			Help::ConvertNumToStrComma( nAddPeso, szConvertNum, sizeof( szConvertNum ) );

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

			pInvenWnd->ShowItemRecvSellInfoWnd( ItemRecvSellInfoWnd::ITEM_SELL, ItemRecvSellInfoWnd::ITEM_COSTUME, szIconName, vTitle, vDesc, -1, 0, bCustom, true, nGradeType );
		}
	}
	else
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6), eResult );
}

// �ڽ�Ƭ ���� (���� �ޱ�)
void ioCostumeInfoManager::OnCostumePresentResult( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );
	
	int nCostumeCode = 0;
	OnCostumeAdd( rkPacket, nCostumeCode );
}

// �ڽ�Ƭ ���� (�ڽ�Ƭ ������ �߰�)
void ioCostumeInfoManager::OnCostumeAdd( SP2Packet &rkPacket, int &nCode )
{
	int nCostumeCode = 0;
	int nSlotIndex = 0;
	byte nPeriodType = 0;
	int nValue1 = 0;
	int nValue2 = 0;
	DWORD dwMaleCustom = 0;
	DWORD dwFemaleCustom = 0;
	
	rkPacket >> nCostumeCode >> nSlotIndex;
	rkPacket >> nPeriodType;
	rkPacket >> nValue1 >> nValue2 >> dwMaleCustom >> dwFemaleCustom;

	ioUserCostume *pCostume = g_MyInfo.GetUserCostume();
	if( pCostume )
	{
		CostumeSlot kSlot;
		kSlot.m_nCostumeCode = nCostumeCode;
		kSlot.m_nIndex = nSlotIndex;
		kSlot.m_PeriodType = nPeriodType;
		kSlot.m_iValue1 = nValue1;
		kSlot.m_iValue2 = nValue2;
		kSlot.m_dwMaleCustom = dwMaleCustom;
		kSlot.m_dwFemaleCustom = dwFemaleCustom;

		pCostume->AddCostumeSlotData( kSlot );

		nCode = nCostumeCode;
	}
}

// �ڽ�Ƭ ���� (�ڽ�Ƭ ������ ��ü)
void ioCostumeInfoManager::OnCostumeChange( SP2Packet &rkPacket, ioPlayStage* pPlayStage )
{	
	TCPNetwork::MouseBusy( false );
	
	if ( !pPlayStage )
		return;

	byte eResult;
	rkPacket >> eResult;

	ioHashString szOwner;
	rkPacket >> szOwner;
		
	if ( eResult == COSTUME_CHANGE_SUCCESS )
	{
		int nCharIndex, nSlotIndex, nCostumeCode;
		byte nSlot;
		bool bEquip;
		rkPacket >> nCharIndex;
		rkPacket >> nSlot;
		rkPacket >> nSlotIndex;
		rkPacket >> bEquip;
		rkPacket >> nCostumeCode;

		DWORD dwCostumeCustomMale, dwCostumeCustomFemale;
		rkPacket >> dwCostumeCustomMale >> dwCostumeCustomFemale;
		
		// ��ü ����� �ڽ��϶�
		if( szOwner == g_MyInfo.GetPublicID() )
		{
			int nCharArray = g_MyInfo.GetCharArray( nCharIndex );
			g_MyInfo.ApplyChangeCostume( nCharArray, nSlot, nSlotIndex, bEquip );
			g_MyInfo.SetUserInfoMgr();

			// UI ����
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
					pPlayMode->SetChangeExtraItem( nCharArray, nSlot );
				return;
			}
		}
		/*
		else
		{
			int nClassType;
			rkPacket >> nClassType;

			g_UserInfoMgr.SetEquipCostumebySubType( szOwner, nClassType, nSlot, nCostumeCode, dwCostumeCustomMale, dwCostumeCustomFemale );
		}
		*/

		// ������ �ٽ� ���� (���� ���Կ� �ִ� �������� ����)
		if( pPlayStage->GetModeType() != MT_MYROOM )
		{
			// �ʵ忡 �������� ĳ���͸� ��� ��ü
			ioBaseChar *pOwner = pPlayStage->GetBaseChar( szOwner );
			bool bSelectChar = true;
			if( pOwner && pOwner->IsOwnerChar() && ( g_MyInfo.GetSelectCharArray() != g_MyInfo.GetCharArray( nCharIndex ) ) )
				bSelectChar = false;
			
			if( pOwner && bSelectChar && !pPlayStage->IsModeStateResult() )
			{
				if ( !bEquip )
				{
					nCostumeCode = 0;
					dwCostumeCustomMale = 0;
					dwCostumeCustomFemale = 0;
				}

				pOwner->SetEquipCostumeInfo( nSlot, nCostumeCode, dwCostumeCustomMale, dwCostumeCustomFemale );

				if( pOwner->GetState() == CS_EXTRAITEM_CHANGE )
					pOwner->SetState( CS_DELAY );

				ioItem *pPreItem = pOwner->GetEquipedItem( nSlot );
				ioItem *pNewItem = NULL;

				if ( !pPreItem )
				{
					pOwner->CheckDefaultReinforce();
					return;
				}

				int dwItemCode = pPreItem->GetItemCode();
				if( dwItemCode > 0 )
					pNewItem = pPlayStage->CreateItem( dwItemCode );

				if( pNewItem )
				{
					pNewItem->ReLoadProperty();
					pNewItem->SetOwnerName( szOwner );
					pNewItem->SetItemCreateIndex( pPreItem->GetItemCreateIndex() );
					pNewItem->SetItemReinforce( pPreItem->GetItemReinforce() );
					pNewItem->SetItemCustom( pPreItem->GetItemMaleCustombyBase(), pPreItem->GetItemFemaleCustombyBase() );

					// �ڽ�Ƭ ���� (�ڽ�Ƭ ������ ����ȭ)
					pNewItem->SetCostumeCustom( dwCostumeCustomMale, dwCostumeCustomFemale );
					pNewItem->SetCostume( nCostumeCode, pOwner->IsOwnerChar() );
															
					// ��ų ������, �߼� �缳��
					pNewItem->SetCurSkillGauge( pPreItem->GetCurSkillGuage() );
					
					if( pOwner->IsOwnerChar() && pNewItem->GetType() == ioItem::IT_WEAPON )
						pNewItem->SetCurBullet( pPreItem->GetCurBullet() );
					
					pPreItem = pOwner->EquipItem( pNewItem, true );
					SAFEDELETE( pPreItem );
				}
				else if( !pNewItem && dwItemCode > 0 )
				{
					if( pOwner->GetState() == CS_EXTRAITEM_CHANGE )
						pOwner->SetState( CS_DELAY );

					pOwner->CheckDefaultReinforce();

					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1), dwItemCode );
					return;
				}

				pOwner->CheckDefaultReinforce();
			}
		}
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