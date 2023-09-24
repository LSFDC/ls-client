#include "stdafx.h"

#include "ioExcavationManager.h"

#include "ItemDefine.h"

#include "SP2Packet.h"
#include "TCPProtocol.h"

#include "ioBaseChar.h"
#include "ioPlayStage.h"

#include "P2PProtocol.h"

#include "ioEtcItem.h"

#include "GUI/ExcavationWnd.h"
#include "GUI/GlobalWnd.h"

#include "ioPlayMode.h"

template<> ioExcavationManager *Singleton< ioExcavationManager >::ms_Singleton = 0;

ioExcavationManager::ioExcavationManager(void)
{
	m_dwRequestJudgementDelayTime = 0;
	m_dwRequestJudgementTime      = 0;
	m_iExcavatingFirstCoolTime    = 0;
	m_iExcavatingCoolTime         = 0;

	m_dwKeyInputTimer             = 0; 
	m_dwMaxKeyInputTime           = 0;
	m_dwCurAutoOffTimer           = 0; 

	m_iReserveRoomIndex           = 0;
	m_dwReserveArtifactIndex      = 0;

	m_CrateArtifactXPosRand.SetRandomSeed( REALGETTIME() + 1 );
	m_CrateArtifactZPosRand.SetRandomSeed( REALGETTIME() + 2 );

	m_iRealExcavatingUserTime= 0;
	m_iMaxSuccessRate        = 0;
	m_iSuccessRatePerOneUser = 0;
	m_dwWaitArtifactPosTime   = 0;
	m_dwCurWaitArtifactPosTime= 0;
}

ioExcavationManager::~ioExcavationManager(void)
{
	m_vArtifactInfoVector.clear();
	m_vItemInfoVector.clear();
	m_vGradeInfoVector.clear();
	m_mExcavatingKitMap.clear();
	m_vMapMentInfoVector.clear();
}

ioExcavationManager &ioExcavationManager::GetSingleton()
{
	return Singleton< ioExcavationManager >::GetSingleton();
}

void ioExcavationManager::OnExcavationPacket( SP2Packet &rkPacket, ioPlayStage *pPlayStage )
{
	int iCommand = 0;
	rkPacket >> iCommand;

	if( iCommand == EXCAVATION_CREATE_ARTIFACT )
		_OnCreateArtifactPacket( rkPacket, pPlayStage );
	else if( iCommand == EXCAVATION_ENTER_USER )
		_OnEnterUserPacket( rkPacket );
	else if( iCommand == EXCAVATION_RESULT_POSITION )
		_OnArtifactPosionPacket( rkPacket );
	else if( iCommand == EXCAVATION_DELETE_ARTIFACT )
		_OnDeleteArtifactPacket( rkPacket );
	else if( iCommand == EXCAVATION_RESULT_JUDGEMENT_FAIL )
		_OnJudgementFailPacket( rkPacket, pPlayStage );
	else if( iCommand == EXCAVATION_RESULT_JUDGEMENT_SUCCESS )
		_OnJudgementSuccessPacket( rkPacket, pPlayStage );
	else if( iCommand == EXCAVATION_USE_KIT )
		_OnUseExcavatingKit( rkPacket, pPlayStage );
	else
		LOG.PrintTimeAndLog( 0, "%s UNKNOWN %d", __FUNCTION__, iCommand );
}

void ioExcavationManager::OnExcavationUDPPacket( SP2Packet &rkPacket, ioPlayStage *pPlayStage )
{
	if( !pPlayStage )
	{
		LOG.PrintTimeAndLog( 0, "%s pPlayStage == NULL. ", __FUNCTION__ );
		return;
	}

	ioHashString szPublicID;
	rkPacket >> szPublicID;

	ioBaseChar *pChar = pPlayStage->GetBaseChar( szPublicID );
	if( !pChar )
	{
		LOG.PrintTimeAndLog( 0, "%s pBaseChar == NULL. ", __FUNCTION__ );
		return;
	}

	ioEquipSlot* pEquipSlot = pChar->GetEquipSlot();
	if( !pEquipSlot )
	{
		LOG.PrintTimeAndLog( 0, "%s pEquipSlot == NULL. ", __FUNCTION__ );
		return;
	}

	ioExcavationItem *pExcavationItem = ToExcavationItem( pEquipSlot->GetObject() );
	if( !pExcavationItem )
	{
		LOG.PrintTimeAndLog( 0, "%s pItem == NULL. ", __FUNCTION__ );
		return;
	}

	pExcavationItem->SetExcavatingStartAni( pChar );
}

void ioExcavationManager::ClearMode()
{
	m_vArtifactInfoVector.clear();
	m_iReserveRoomIndex      = 0;
	m_dwReserveArtifactIndex = 0;
}

void ioExcavationManager::_OnCreateArtifactPacket( SP2Packet &rkPacket, ioPlayStage *pPlayStage )
{
	int   iRoomIndex      = 0;
	DWORD dwArtifactIndex = 0;
	bool  bWait          = false;

	rkPacket >> iRoomIndex;
	rkPacket >> dwArtifactIndex;
	rkPacket >> bWait;

	if( !pPlayStage )
	{
		LOG.PrintTimeAndLog( 0, "%s pPlayStage == NULL. ", __FUNCTION__ );
		return;
	}

	if( bWait )
	{
		if( m_iReserveRoomIndex != 0 )
			return;
		m_iReserveRoomIndex       = iRoomIndex;
		m_dwReserveArtifactIndex  = dwArtifactIndex;
		m_dwCurWaitArtifactPosTime= REALGETTIME();
		return;
	}

	ioPlayMode *pPlayMode = pPlayStage->GetPlayMode();
	if( !pPlayMode || ( pPlayMode && ( pPlayMode->GetModeState() != ioPlayMode::MS_PLAY ) ) )
	{
		if( m_iReserveRoomIndex != 0 )
			return;
		m_iReserveRoomIndex      = iRoomIndex;
		m_dwReserveArtifactIndex = dwArtifactIndex;
		return;
	}

	SendArtifactPostion(pPlayStage, iRoomIndex, dwArtifactIndex);
}

void ioExcavationManager::_OnEnterUserPacket( SP2Packet &rkPacket )
{
	int iSize = 0;
	rkPacket >> iSize;
	for (int i = 0; i < iSize ; i++)
	{
		ArtifactInfo kInfo;

		int iX = 0;
		int iY = 0;
		int iZ = 0;

		rkPacket >> kInfo.m_dwIndex;
		rkPacket >> iX;
		rkPacket >> iY;
		rkPacket >> iZ;

		kInfo.EncryptPos( iX, iY, iZ );

		m_vArtifactInfoVector.push_back( kInfo );

#ifndef SHIPPING
		LOG.PrintTimeAndLog( 0, "%s (%d:%d:%d:%d)", __FUNCTION__, kInfo.m_dwIndex, iX, iY, iZ );
#endif
	}
	
}

void ioExcavationManager::_OnArtifactPosionPacket( SP2Packet &rkPacket )
{
	ArtifactInfo kInfo;

	int iX = 0;
	int iY = 0;
	int iZ = 0;

	rkPacket >> kInfo.m_dwIndex;
	rkPacket >> iX;
	rkPacket >> iY;
	rkPacket >> iZ;

	kInfo.EncryptPos( iX, iY, iZ );

	m_vArtifactInfoVector.push_back( kInfo );

#ifndef SHIPPING
	LOG.PrintTimeAndLog( 0, "%s (%d:%d:%d:%d)", __FUNCTION__, kInfo.m_dwIndex, iX, iY, iZ );
#endif
}

void ioExcavationManager::_OnDeleteArtifactPacket( SP2Packet &rkPacket )
{
	DWORD dwIndex = 0;
	rkPacket >> dwIndex;
	DeleteArtifact( dwIndex );
}

void ioExcavationManager::_OnJudgementFailPacket( SP2Packet &rkPacket, ioPlayStage *pPlayStage )
{
	if( !pPlayStage )
	{
		LOG.PrintTimeAndLog( 0, "%s pPlayStage == NULL. ", __FUNCTION__ );
		return;
	}

	ioHashString szPublicID;
	rkPacket >> szPublicID;

	ioBaseChar *pChar = pPlayStage->GetBaseChar( szPublicID );

	if( !pChar )
	{
		LOG.PrintTimeAndLog( 0, "%s pBaseChar == NULL. ", __FUNCTION__ );
		return;
	}

	ioEquipSlot* pEquipSlot = pChar->GetEquipSlot();
	if( !pEquipSlot )
	{
		LOG.PrintTimeAndLog( 0, "%s pEquipSlot == NULL. ", __FUNCTION__ );
		return;
	}

	ioExcavationItem *pExcavationItem = ToExcavationItem( pEquipSlot->GetObject() );
	if( !pExcavationItem )
	{
		LOG.PrintTimeAndLog( 0, "%s pItem == NULL. ", __FUNCTION__ );
		return;
	}

	pExcavationItem->SetResultFailAni( pChar );

	if( !pChar->IsOwnerChar() )
		return;

	TCPNetwork::MouseBusy( false );
}

void ioExcavationManager::_OnJudgementSuccessPacket( SP2Packet &rkPacket, ioPlayStage *pPlayStage )
{
	DWORD dwArtifactIndex = 0;
	ioHashString szPublicID;
	int iArtifactType     = 0;
	rkPacket >> dwArtifactIndex;
	rkPacket >> szPublicID;
	rkPacket >> iArtifactType;

	DeleteArtifact( dwArtifactIndex );

// 	if( !COMPARE( iArtifactType, FAIL_ARTIFACT_START_TYPE, EXTRAITEM_START_TYPE ) )
// 	{
// 		ExcavationSearchingWnd *pSearchingWnd = dynamic_cast<ExcavationSearchingWnd*> ( g_GUIMgr.FindWnd( EXCAVATION_SEARCHING_WND ) );
// 		if( pSearchingWnd && pSearchingWnd->IsShow() )
// 			pSearchingWnd->ClearExcavatingCurCoolTime();
// 	}

	if( !pPlayStage )
	{
		LOG.PrintTimeAndLog( 0, "%s pPlayStage == NULL. ", __FUNCTION__ );
		return;
	}

	ioBaseChar *pChar = pPlayStage->GetBaseChar( szPublicID );
	if( !pChar )
	{
		LOG.PrintTimeAndLog( 0, "%s pBaseChar == NULL. ", __FUNCTION__ );
		return;
	}

	ioEquipSlot* pEquipSlot = pChar->GetEquipSlot();
	if( !pEquipSlot )
	{
		LOG.PrintTimeAndLog( 0, "%s pEquipSlot == NULL. ", __FUNCTION__ );
		return;
	}

	ioExcavationItem *pExcavationItem = ToExcavationItem( pEquipSlot->GetObject() );
	if( !pExcavationItem )
	{
		LOG.PrintTimeAndLog( 0, "%s pItem == NULL. ", __FUNCTION__ );
		return;
	}

	if( !COMPARE( iArtifactType, FAIL_ARTIFACT_START_TYPE, EXTRAITEM_START_TYPE ) )
		pExcavationItem->SetResultSuccessAni( pChar );
	else
		pExcavationItem->SetResultFailAni( pChar );

	int iArtifactValue1   = 0;
	int iArtifactValue2   = 0;
	float fMapRate        = 0.0f;
	bool bAlarm           = false;

	__int64 biMoney = 0;
	float   fGrade  = 0.0f;

	int iItemCode    = 0;
	int iReinforce   = 0;
	int iSlotIndex   = 0;
	int iPeriodType  = 0;
	int iValue1      = 0;
	int iValue2      = 0;
	int iPeriodTime  = 0;
	int iMachineCode = 0;

#if defined( USE_GA )
	int iGAPeso		 = 0;
#endif

	if( iArtifactType < EXTRAITEM_START_TYPE  )
	{
		rkPacket >> biMoney;
		rkPacket >> iArtifactValue1; // item peso
		rkPacket >> iArtifactValue2; // grade type
		rkPacket >> fMapRate;
		rkPacket >> bAlarm;

#if defined( USE_GA )
		iGAPeso	= iArtifactValue1;
#endif

	}
	else
	{
		rkPacket >> iItemCode >> iReinforce >> iSlotIndex >> iPeriodType >> iValue1 >> iValue2 >> iPeriodTime >> iMachineCode >> bAlarm;

		iArtifactValue1 = iItemCode;
		iArtifactValue2 = ( iReinforce * PRESENT_EXTRAITEM_DIVISION_2 ) + iPeriodTime;
	}	

	if( pChar->IsOwnerChar() )
	{
		TCPNetwork::MouseBusy( false );

		// level , exp
		m_kResultInfo.Clear();
		int iLevel = 0;
		int iExp   = 0;
		int iGradeLevel = 0;
		int iClassLevel = 0;
		rkPacket >> iLevel >> iExp >> m_kResultInfo.m_iAddExp >> m_kResultInfo.m_iClassType >> m_kResultInfo.m_iAddSoldierExp >> iGradeLevel >> iClassLevel;

		int iPrevGradeLevel = g_MyInfo.GetGradeLevel();
		if( g_MyInfo.IsCharExerciseStyleToClassType( m_kResultInfo.m_iClassType, EXERCISE_RENTAL ) == false )
			g_MyInfo.AddClassExp( m_kResultInfo.m_iClassType, m_kResultInfo.m_iAddSoldierExp ); 
		g_MyInfo.AddGradeExp( m_kResultInfo.m_iAddSoldierExp );
		if( g_MyInfo.GetGradeLevel() > iPrevGradeLevel ) 
			m_kResultInfo.m_bGradeLevelUp = true;

		pChar->SetLevel( iGradeLevel );

		if( g_MyInfo.IsCharExerciseStyleToClassType( m_kResultInfo.m_iClassType, EXERCISE_RENTAL ) == false )
			pChar->SetClassLevel( iClassLevel);

		int iPrevLevel = g_MyInfo.GetExcavationLevel();
		g_MyInfo.SetExcavationLevel( iLevel, iExp );
		if( g_MyInfo.GetExcavationLevel() > iPrevLevel )
			m_kResultInfo.m_bLevelUp = true;

		// gift
		if( iArtifactType < EXTRAITEM_START_TYPE  )
		{
			g_MyInfo.SetMoney( biMoney );
		}
		else
		{
			ioUserExtraItem *pUserExtraItem = g_MyInfo.GetUserExtraItem();
			if( pUserExtraItem )
			{
				EXTRAITEMSLOT kSlot;
				kSlot.m_iItemCode  = iItemCode;
				kSlot.m_iReinforce = iReinforce;
				kSlot.m_iIndex     = iSlotIndex;
				kSlot.m_PeriodType = iPeriodType;
				kSlot.m_iValue1    = iValue1;
				kSlot.m_iValue2    = iValue2;

				pUserExtraItem->AddExtraItemData( kSlot );
			}
		}

		// 페소 & 장비 획득하면 퀘스트 성공
		if( iArtifactValue1 > 0 )
		{
			if( iArtifactType < EXTRAITEM_START_TYPE )
			{
				// 페소를 획득했으면 맵 페소값에 맵 보정값 적용
				int iGradeArray = GetGradeArray( iArtifactValue2 );
				int iPeso       = ( iArtifactValue1 * GetGradeRate( iGradeArray ) ) * fMapRate;
				g_QuestMgr.QuestCompleteTerm( QC_EXCAVATION_SUCCESS, iArtifactType, iPeso, iArtifactValue2 );
			}
			else
				g_QuestMgr.QuestCompleteTerm( QC_EXCAVATION_SUCCESS, iArtifactType, iArtifactValue1, iArtifactValue2 );
		}
		else
		{
			g_QuestMgr.QuestCompleteTerm( QC_EXCAVATION_FAIL );
		}

		// wnd
		ExcavationResultWnd *pResultWnd = dynamic_cast<ExcavationResultWnd*> ( g_GUIMgr.FindWnd( EXCAVATION_RESULT_WND ) );
		if( pResultWnd )
		{
			pResultWnd->SetResultInfo( iArtifactType, iArtifactValue1, iArtifactValue2, bAlarm, fMapRate );
			pResultWnd->ShowWnd();
		}

		ExcavationSearchingWnd *pSearchingWnd = dynamic_cast<ExcavationSearchingWnd*> ( g_GUIMgr.FindWnd( EXCAVATION_SEARCHING_WND ) );
		if( pSearchingWnd && pSearchingWnd->IsShow() )
		{
			pSearchingWnd->SetProcessRadarPos( false );
		}

#if defined( USE_GA )
		char chLabel[32] = {0,};

		if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
			sprintf_e( chLabel, "%d", iGAPeso );
		else
			SafeSprintf( chLabel, sizeof(chLabel), "%1", iGAPeso );

		// PESO_GET_EXPLORE
		g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
			, "Peso"
			, "Explore"
			, chLabel
			, 1
			, "%2FPESO%2FGET%2FEXPLORE" );
#endif

	}

	if( !pChar->IsOwnerChar() )
	{
		if( !bAlarm )
			SetExcavatingSuccessAlarm( szPublicID, iArtifactType, iArtifactValue1, iArtifactValue2, fMapRate, false );
		
		ioHashString szIcon;
		if( iArtifactType < EXTRAITEM_START_TYPE  )
		{
			int iArray = GetItemArray( iArtifactType );
			GetItemIcon( iArray, szIcon );
		}
		else
		{
			const ioItem *pItem = g_ItemMaker.GetItemConst( iArtifactValue1, __FUNCTION__ );
			if( pItem )
				szIcon = pItem->GetItemLargeIconName();
		}
		pChar->SetEmoticon( szIcon );
	}
	g_QuestMgr.QuestCompleteTermReserve( QC_EXCAVATION_LEVELUP );
}

void ioExcavationManager::_OnUseExcavatingKit( SP2Packet &rkPacket, ioPlayStage *pPlayStage )
{
	if( !pPlayStage )
	{
		LOG.PrintTimeAndLog( 0, "%s pPlayStage == NULL. ", __FUNCTION__ );
		return;
	}

	ioHashString szPublicID;
	DWORD dwEtcItemType = 0;
	bool  bUse          = false;
	bool  bDeleteItem   = false;
	rkPacket >> szPublicID;
	rkPacket >> dwEtcItemType;
	rkPacket >> bUse;
	rkPacket >> bDeleteItem;

	ioBaseChar *pChar = pPlayStage->GetBaseChar( szPublicID );
	if( !pChar )
	{
		LOG.PrintTimeAndLog( 0, "%s pBaseChar == NULL. ", __FUNCTION__ );
		return;
	}

	if( pChar->IsOwnerChar() )
	{
		TCPNetwork::MouseBusy( false );

		if( bDeleteItem )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
			g_GUIMgr.HideWnd( EXCAVATION_SEARCHING_WND );
		}

		if( bUse )
		{
			m_dwKeyInputTimer = 0;
			m_dwCurAutoOffTimer = REALGETTIME();

			ioPlayMode *pPlayMode = pPlayStage->GetPlayMode();
			if( pPlayMode )
			{
				ioHashString sMent;
				GetMapMent( pPlayMode->GetModeSubNum(), sMent );
				if( !sMent.IsEmpty() )
					g_ChatMgr.SetSystemMsg( sMent.c_str() );
			}

			g_GUIMgr.ShowWnd( EXCAVATION_SEARCHING_WND );

#if defined( USE_GA )
			// PLAY_PLAZA_EXPLORE_START
			g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
				, "Play"
				, "Start"
				, ""
				, 1
				, "%2FPLAY%2FPLAZA%2FEXPLORE%2FSTART" );
#endif

		}
		else
		{
			g_GUIMgr.HideWnd( EXCAVATION_SEARCHING_WND );
			m_dwRequestJudgementTime = 0;

#if defined( USE_GA )
 			// PLAY_PLAZA_EXPLORE_END
 			g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
 				, "Play"
 				, "End"
 				, ""
 				, 1
 				, "%2FPLAY%2FPLAZA%2FEXPLORE%2FEND" );
#endif

		}
	}

	if( !bUse )
	{
		ioEquipSlot* pEquipSlot = pChar->GetEquipSlot();
		if( !pEquipSlot )
		{
			LOG.PrintTimeAndLog( 0, "%s pEquipSlot == NULL. ", __FUNCTION__ );
			if( pChar->IsOwnerChar() )
				pChar->SetState( CS_DELAY ); 
			return;
		}

		ioExcavationItem *pExcavationItem = ToExcavationItem( pEquipSlot->GetObject() );
		if( !pExcavationItem )
		{
			LOG.PrintTimeAndLog( 0, "%s pItem == NULL. ", __FUNCTION__ );
			if( pChar->IsOwnerChar() )
				pChar->SetState( CS_DELAY ); 
			return;
		}

		if( pChar->GetState() != CS_DELAY && pChar->GetState() != CS_EXCAVATING ) // 강제처리함.
		{
			pExcavationItem->SetExcavatingToolMesh( pChar, false ); 

			ioItem *pItem = NULL;
			ioEquipSlot *pSlot = pChar->GetEquipSlot();
			if( pSlot )
				pItem =	pSlot->ReleaseObjectSlot();
			if( pItem )	
				delete pItem; // 삭제했으니 pExcavationItem 호출하지 말것

			pChar->SetExcavating( false );
			if( pChar->IsOwnerChar() )
				pChar->SetState( CS_DELAY ); 
		}
		else
			pExcavationItem->SetReleaseAni( pChar );
		return;
	}

	if( ToExcavationItem( pChar->GetObject() ) )
	{
		LOG.PrintTimeAndLog( 0, "%s had Excavation Item. ", __FUNCTION__ );
		if( pChar->IsOwnerChar() )
			pChar->SetState( CS_DELAY ); 
		return;
	}

	ioItem *pItem = pPlayStage->CreateItem( GetObjectItemCode( dwEtcItemType ) );
	if( !pItem )
	{
		LOG.PrintTimeAndLog( 0, "%s pItem == NULL. ", __FUNCTION__ );
		if( pChar->IsOwnerChar() )
			pChar->SetState( CS_DELAY ); 
		return;
	}
	
	pItem->ReLoadProperty();
	pItem->SetOwnerName( pChar->GetCharName() );
	pItem->SetItemCreateIndex( pChar->GetIndex() * FLOAT100 );
	pItem->SetItemReinforce( 0 );
	pItem->SetItemCustom( 0, 0 );
	
	ioItem *pPreItem = pChar->EquipItem( pItem );
	SAFEDELETE( pPreItem );
}

void ioExcavationManager::DeleteArtifact( DWORD dwIndex )
{
	for(vArtifactInfoVector::iterator iter = m_vArtifactInfoVector.begin(); iter != m_vArtifactInfoVector.end(); ++iter)
	{
		ArtifactInfo &rkInfo = *iter;
		if( rkInfo.m_dwIndex == dwIndex )
		{
			m_vArtifactInfoVector.erase( iter );
			return;
		}
	}
}

bool ioExcavationManager::SendUseExcavatingKit( int iEtcItemType )
{
	if( g_App.IsMouseBusy() )
		return false;

	ioUserEtcItem *pUserEtcItem = g_MyInfo.GetUserEtcItem();
	if( !pUserEtcItem )
	{
		LOG.PrintTimeAndLog( 0, "%s pUserEtcItem == NULL.", __FUNCTION__ );
		return false;
	}

	ioUserEtcItem::ETCITEMSLOT kItemSlot;
	if( !pUserEtcItem->GetEtcItem( iEtcItemType, kItemSlot ) )
		return false;

	SP2Packet kPacket( CTPK_ETCITEM_USE );
	kPacket << iEtcItemType;
	kPacket << !kItemSlot.IsUse();
	TCPNetwork::SendToServer( kPacket );
	TCPNetwork::MouseBusy( true );
	return true;
}

void ioExcavationManager::SendRequestJudgement( ioBaseChar *pOwner, ioPlayStage *pPlayStage )
{
	if( m_dwRequestJudgementTime == 0 )
		return;

	if( !pOwner )
		return;

	if( !pPlayStage )
		return;

	if( g_App.IsMouseBusy() )
		return;

	if( REALGETTIME() - m_dwRequestJudgementTime < m_dwRequestJudgementDelayTime )
		return;
	m_dwRequestJudgementTime = 0;

	SP2Packet kPacket( CTPK_EXCAVATION_COMMAND );
	kPacket << EXCAVATION_REQUEST_JUDGEMENT;
	kPacket << pPlayStage->GetRoomIndex();
	kPacket << (int)pOwner->GetWorldPosition().x;
	kPacket << (int)pOwner->GetWorldPosition().y;
	kPacket << (int)pOwner->GetWorldPosition().z;
	TCPNetwork::SendToServer( kPacket );
	TCPNetwork::MouseBusy( true );
}

void ioExcavationManager::LoadExcavation()
{
	ioINILoader_e kLoader( "config/sp2_excavation_info.ini" );

	kLoader.SetTitle_e( "Common" );
	m_dwRequestJudgementDelayTime = kLoader.LoadInt_e( "Request_Judgement_Time", 1000 );
	m_iExcavatingFirstCoolTime    = kLoader.LoadInt_e( "Excavating_First_Cool_Time", 20000 );
	m_iExcavatingCoolTime         = kLoader.LoadInt_e( "Excavating_Cool_Time", 10000 );
	m_dwMaxKeyInputTime           = kLoader.LoadInt_e( "Max_Key_Input_Time", 60000 );
	m_iRealExcavatingUserTime     = kLoader.LoadInt_e( "Real_Excavating_User_Time", 0 );
	m_iSuccessRatePerOneUser      = kLoader.LoadInt_e( "Success_Rate_Per_One_User", 0 );
	m_iMaxSuccessRate             = kLoader.LoadInt_e( "Max_Success_Rate", 0 );
	m_dwWaitArtifactPosTime       = kLoader.LoadInt_e( "Wait_Artifact_Pos_Time", 0 );

	m_vItemInfoVector.clear();
	kLoader.SetTitle_e( "ArtifactItem" );
	int iMax = kLoader.LoadInt_e( "Max_Item", 0 );
	for (int i = 0; i < iMax ; i++)
	{
		ItemInfo kItemInfo;
		char szKey[MAX_PATH]="";
		StringCbPrintf_e( szKey, sizeof( szKey ), "Item%d_Type", i+1 );
		kItemInfo.m_iType = kLoader.LoadInt( szKey, 0 );
		StringCbPrintf_e( szKey,  sizeof( szKey ), "Item%d_Value1", i+1 );
		kItemInfo.m_iValue1 = kLoader.LoadInt( szKey, 0 );
		char szBuf[MAX_PATH]="";
		StringCbPrintf_e( szKey,  sizeof( szKey ), "Item%d_Name", i+1 );
		kLoader.LoadString( szKey, "", szBuf, MAX_PATH );
		kItemInfo.m_sName = szBuf;
		StringCbPrintf_e( szKey,  sizeof( szKey ), "Item%d_Desc", i+1 );
		kLoader.LoadString( szKey, "", szBuf, MAX_PATH );
		kItemInfo.m_sDesc = szBuf;
		StringCbPrintf_e( szKey,  sizeof( szKey ), "Item%d_Icon", i+1 );
		kLoader.LoadString( szKey, "", szBuf, MAX_PATH );
		kItemInfo.m_sIcon = szBuf;

		m_vItemInfoVector.push_back( kItemInfo );
	}

	m_vGradeInfoVector.clear();
	kLoader.SetTitle_e( "ArtifactGrade" );
	iMax = kLoader.LoadInt_e( "Max_Grade", 0 );
	for (int i = 0; i < iMax ; i++)
	{
		GradeInfo kGradeInfo;
		char szKey[MAX_PATH]="";
		char szBuf[MAX_PATH]="";
		StringCbPrintf_e( szKey, sizeof( szKey ), "Grade%d_Type", i+1 );
		kGradeInfo.m_iType = kLoader.LoadInt( szKey, 0 );
		StringCbPrintf_e( szKey, sizeof( szKey ), "Grade%d_Rank", i+1 );
		kLoader.LoadString( szKey, "", szBuf, MAX_PATH );
		kGradeInfo.m_sRank = szBuf;
		StringCbPrintf_e( szKey, sizeof( szKey ), "Grade%d_Rate", i+1 );
		kGradeInfo.m_fRate = kLoader.LoadFloat( szKey, 0.0f );

		m_vGradeInfoVector.push_back( kGradeInfo );
	}

	m_mExcavatingKitMap.clear();
	kLoader.SetTitle_e( "ExcavatingKit" );
	iMax = kLoader.LoadInt_e( "Max_Kit", 0 );
	for (int i = 0; i < iMax ; i++)
	{
		char szKey[MAX_PATH]="";
		StringCbPrintf_e( szKey, sizeof( szKey ), "Kit%d_Etc_Item_Type", i+1 );
		int iEtcItemType = kLoader.LoadInt( szKey, 0 );
		StringCbPrintf_e( szKey, sizeof( szKey ), "Kit%d_Object_Item_Code", i+1 );
		int iObjectItemCode = kLoader.LoadInt( szKey, 0 );

		m_mExcavatingKitMap.insert( mExcavatingKitMap::value_type( iEtcItemType, iObjectItemCode ) );
	}	

	m_vMapMentInfoVector.clear();
	kLoader.SetTitle_e( "MapMentInfo" );
	iMax = kLoader.LoadInt_e( "Max_Map", 0 );
	for (int i = 0; i < iMax ; i++)
	{
		MapMentInfo kInfo;
		char szKey[MAX_PATH]="";
		char szBuf[MAX_PATH]="";
		StringCbPrintf_e( szKey, sizeof( szKey ), "Map%d_Mode_Sub_Type", i+1 );
		kInfo.m_iModeSubType = kLoader.LoadInt( szKey, 0 );
		StringCbPrintf_e( szKey, sizeof( szKey ), "Map%d_Ment", i+1 );
		kLoader.LoadString( szKey, "", szBuf, MAX_PATH );
		kInfo.m_sMent = szBuf;
		
		m_vMapMentInfoVector.push_back( kInfo );
	}

}

bool ioExcavationManager::SetExcavating( ioBaseChar *pOwner )
{
	if( !pOwner )
	{
		LOG.PrintTimeAndLog( 0, "%s pOwner == NULL. ", __FUNCTION__ );
		return false;
	}

	if( pOwner->GetState() != CS_DELAY && pOwner->GetState() != CS_RUN )
	{
		g_ChatMgr.SetSystemMsg( STR(1) );
		return false;
	}

	ioEquipSlot* pEquipSlot = pOwner->GetEquipSlot();
	if( !pEquipSlot )
	{
		LOG.PrintTimeAndLog( 0, "%s pEquipSlot == NULL. ", __FUNCTION__ );
		return false;
	}

	ioExcavationItem *pExcavationItem = ToExcavationItem( pEquipSlot->GetObject() );
	if( !pExcavationItem )
	{
		LOG.PrintTimeAndLog( 0, "%s pItem == NULL. ", __FUNCTION__ );
		return false;
	}

	pExcavationItem->SetExcavatingStartAni( pOwner );
	m_dwRequestJudgementTime = REALGETTIME();

	
	SP2Packet kPacket( CUPK_EXCAVATING );
	kPacket << pOwner->GetCharName();
	P2PNetwork::SendToAllPlayingUser( kPacket );

	return true;
}

int ioExcavationManager::GetArtifactSize()
{
	return m_vArtifactInfoVector.size();
}

int ioExcavationManager::GetItemSize()
{
	return m_vItemInfoVector.size();
}

int ioExcavationManager::GetGradeSize()
{
	return m_vGradeInfoVector.size();
}

void ioExcavationManager::GetItemName( IN int iArray, OUT ioHashString &rsName )
{
	if( COMPARE( iArray, 0, (int)m_vItemInfoVector.size() ) )
		rsName = m_vItemInfoVector[iArray].m_sName;
}

void ioExcavationManager::GetItemDesc( IN int iArray, OUT ioHashString &rsDesc )
{
	if( COMPARE( iArray, 0, (int)m_vItemInfoVector.size() ) )
		rsDesc = m_vItemInfoVector[iArray].m_sDesc;
}

void ioExcavationManager::GetItemIcon( IN int iArray, OUT ioHashString &rsIcon )
{
	if( COMPARE( iArray, 0, (int)m_vItemInfoVector.size() ) )
		rsIcon = m_vItemInfoVector[iArray].m_sIcon;
}

void ioExcavationManager::GetGradeRank( IN int iArray, OUT ioHashString &rsRank )
{
	rsRank.Clear();
	if( COMPARE( iArray, 0, (int)m_vGradeInfoVector.size() ) )
		rsRank = m_vGradeInfoVector[iArray].m_sRank;
}

float ioExcavationManager::GetGradeRate( IN int iArray )
{
	if( COMPARE( iArray, 0, (int)m_vGradeInfoVector.size() ) )
		return m_vGradeInfoVector[iArray].m_fRate;

	return 0.0f;
}

int ioExcavationManager::GetItemArray( int iItemType )
{
	int iSize = m_vItemInfoVector.size();
	for (int i = 0; i < iSize ; i++)
	{
		ItemInfo &rkInfo = m_vItemInfoVector[i];
		if( rkInfo.m_iType == iItemType )
			return i;
	}

	return -1;
}

int ioExcavationManager::GetGradeArray( int iGradeType )
{
	int iSize = m_vGradeInfoVector.size();
	for (int i = 0; i < iSize ; i++)
	{
		GradeInfo &rkInfo = m_vGradeInfoVector[i];
		if( rkInfo.m_iType == iGradeType )
			return i;
	}

	return -1;
}

void ioExcavationManager::OnExcavationBtnUp( ioBaseChar *pOwner )
{
	if( g_GUIMgr.IsShow( BRIEFING_WND ) ) 
		return;

	if( !pOwner )
	{
		LOG.PrintTimeAndLog( 0, "%s pOwner == NULL.", __FUNCTION__ );
		return;
	}

	if( pOwner->GetState() == CS_FISHING || pOwner->GetState() == CS_CONSTRUCT_HOUSE )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return;
	}

	// 캐릭터 상태 체크
	if( pOwner->GetState() != CS_DELAY || pOwner->IsCatchMode() || pOwner->IsPrisonerMode() || pOwner->IsObejctEquipState() )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return;
	}

	// Buff 체크
	if( pOwner->HasBuff( BT_LIMIT_MOVE ) || pOwner->HasBuff( BT_LIMIT_ACTION ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		return;
	}

	// 아이템 체크
	if( pOwner->IsHasCrownByAllMode() )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		return;
	}

	ioObjectItem *pObject = pOwner->GetObject();
	if( pObject && !ToExcavationItem( pObject ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
		return;
	}

	ioEquipSlot *pEquipslot = pOwner->GetEquipSlot();
	ioItem *pItem = NULL;
	if( pEquipslot )
		pItem = pEquipslot->GetEquipedItem( ES_WEAPON );
	if( pItem )
	{
		ioHookItem *pHookItem = ToHookItem( pItem );
		if( pHookItem && pHookItem->CheckOnHand() )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6) );
			return;
		}
	}

	// 광장 체크
	if( pOwner->GetCurModeType() != MT_TRAINING )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(7) );
		return;
	}


	ioUserEtcItem *pUserEtcItem = g_MyInfo.GetUserEtcItem();
	if( !pUserEtcItem )
	{
		LOG.PrintTimeAndLog( 0, "%s pUserEtcItem == NULL.", __FUNCTION__ );
		return;
	}
	
	vUseEtcItemInfoVector vUseEtcItemInfoVec;
	UseEtcItemInfo kInfo;
	kInfo.m_iEtcItemType = ioEtcItem::EIT_ETC_GOLD_EXCAVATING_KIT;
	kInfo.m_iUsePoint    = 1; // 골드가 우선순위가 높다
	vUseEtcItemInfoVec.push_back( kInfo );

	kInfo.m_iEtcItemType = ioEtcItem::EIT_ETC_EXCAVATING_KIT;
	kInfo.m_iUsePoint    = 0;
	vUseEtcItemInfoVec.push_back( kInfo );

	bool bExist = false;
	for(vUseEtcItemInfoVector::iterator iter = vUseEtcItemInfoVec.begin(); iter != vUseEtcItemInfoVec.end(); ++iter)
	{
	    UseEtcItemInfo &rkInfo = *iter;
		ioUserEtcItem::ETCITEMSLOT kItemSlot;
		if( pUserEtcItem->GetEtcItem( rkInfo.m_iEtcItemType, kItemSlot ) )
		{
			bExist = true;
			if( kItemSlot.IsUse() )
				rkInfo.m_iUsePoint += FLOAT100; // 사용중인 아이템이 가장 높은 순위를 가진다.
			else
				rkInfo.m_iUsePoint += 10;  // 가지고 있는 것이 골드 보다 높은 순위를 가진다.
		}
	}

	// 사용 점수가 가장 높은 순으로 정렬한다.
	std::sort( vUseEtcItemInfoVec.begin(), vUseEtcItemInfoVec.end(), UsePointSort() );

	if( !bExist )
	{
		g_GUIMgr.ShowWnd( EXCAVATION_BUY_WND );
		return;
	}

	if( !vUseEtcItemInfoVec.empty() )
	{
		if( SendUseExcavatingKit( vUseEtcItemInfoVec[0].m_iEtcItemType ) )
		{			
			if( !pOwner->IsExcavating() )
			{
				g_QuestMgr.QuestCompleteTerm( QC_EXCAVATION_TRY );
			}
			pOwner->SetState( CS_EXCAVATING );
		}
	}
}

int ioExcavationManager::GetExcavatingFirstCoolTime()
{
	return m_iExcavatingFirstCoolTime;
}

int ioExcavationManager::GetExcavatingCoolTime()
{
	return m_iExcavatingCoolTime;
}

void ioExcavationManager::GetArtifactPos( IN int iArray, OUT int &riX, OUT int &riY, OUT int &riZ )
{
	if( COMPARE( iArray, 0, (int)m_vArtifactInfoVector.size() ) )
	{
		m_vArtifactInfoVector[iArray].GetDecryptPos( riX, riY, riZ );
	}
}

int ioExcavationManager::GetArtifactIndex( int iArray )
{
	if( COMPARE( iArray, 0, (int)m_vArtifactInfoVector.size() ) )
		return m_vArtifactInfoVector[iArray].m_dwIndex;

	return -1;
}

int ioExcavationManager::GetObjectItemCode( int iEtcItemType )
{
	mExcavatingKitMap::iterator iter = m_mExcavatingKitMap.find( iEtcItemType );
	if( iter != m_mExcavatingKitMap.end() )
		return iter->second;

	return 0;
}


void ioExcavationManager::GetMapMent( IN int iMapModeSubType, OUT ioHashString &rsMent )
{
	for(vMapMentInfoVector::iterator iter = m_vMapMentInfoVector.begin(); iter != m_vMapMentInfoVector.end(); ++iter)
	{
	    MapMentInfo &rkInfo = *iter;
		if( rkInfo.m_iModeSubType != iMapModeSubType )
			continue;

		rsMent = rkInfo.m_sMent;
		break;
	}
}

void ioExcavationManager::CheckInputKey( DWORD dwDirKey, DWORD dwActionKey, bool bExcavating )
{
	if( !bExcavating )
		return;

	if( dwDirKey == ioUserKeyInput::DKI_NONE && dwActionKey == ioUserKeyInput::AKI_NONE )
		return;     // 아무 키도 눌리지 않았다.

	if( dwActionKey == ioUserKeyInput::AKI_ATTACK )
	{
		ExcavationSearchingWnd *pSearchingWnd = dynamic_cast<ExcavationSearchingWnd*> ( g_GUIMgr.FindWnd( EXCAVATION_SEARCHING_WND ) );
		if( pSearchingWnd )
			pSearchingWnd->OnExcavatingBtnUp( ExcavationSearchingWnd::ID_EXCAVATING_BTN );
	}

	m_dwKeyInputTimer = 0;
}

void ioExcavationManager::CheckAutoOffKey( ioPlayStage *pPlayStage )
{
	ioBaseChar *pOwnerChar = pPlayStage->GetOwnerChar();
	if( pOwnerChar )      //예외 처리
	{
		if( !pOwnerChar->IsExcavating() )
			return;

		if( pOwnerChar->GetState() == CS_VIEW ||
			pOwnerChar->GetState() == CS_LOADING ||
			pOwnerChar->GetState() == CS_DIE  ||
			pOwnerChar->GetState() == CS_READY||
			pOwnerChar->GetState() == CS_OBSERVER ||
			pOwnerChar->IsPrisonerMode() )
			return;
	}
	else
		return;

	// 키 체크
	m_dwKeyInputTimer += REALGETTIME() - m_dwCurAutoOffTimer;

	DWORD dwMaxInputTime = m_dwMaxKeyInputTime;
	if( m_dwKeyInputTimer > dwMaxInputTime )
	{
		if( dwMaxInputTime != 0 )
		{
			SendEndUsingExcavaingKit();
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
			m_dwKeyInputTimer = 0; // 메세지를 또다시 보내지 않도록 초기화
		}
	}
}

void ioExcavationManager::ProcessAutoOff( ioPlayStage *pPlayStage )
{
	if( pPlayStage && pPlayStage->GetModeType() != MT_TRAINING )
		return;

	CheckAutoOffKey( pPlayStage );
	m_dwCurAutoOffTimer = REALGETTIME();	
}

void ioExcavationManager::SetExcavatingSuccessAlarm( ioHashString &szSendID, int iArtifactType, int iArtifactValue1, int iArtifactValue2, float fMapRate, bool bAllAlarm )
{
	ioComplexStringPrinter kPrinter;
	if( bAllAlarm )
		g_ChatMgr.PresetChatComplexString( MSG_ANNOUNCE_CHAT, kPrinter );
	else
		g_ChatMgr.PresetChatComplexString( MSG_INFO_CHAT, kPrinter );

	kPrinter.SetTextColor( TCT_DEFAULT_VIOLET );
	kPrinter.AddTextPiece( g_ChatMgr.GetChatFontScale(), szSendID.c_str() );

	kPrinter.SetTextColor( g_ChatMgr.GetChatColor() );
	kPrinter.AddTextPiece( g_ChatMgr.GetChatFontScale(), STR(2) );

	char szMent[MAX_PATH]="";
	if( iArtifactType < ioExcavationManager::EXTRAITEM_START_TYPE )
	{
		int iItemArray = GetItemArray( iArtifactType );
		ioHashString szName;
		GetItemName( iItemArray, szName );
		StringCbPrintf_e( szMent, sizeof( szMent ), "%s", szName.c_str() );
	}
	else
	{
		ioHashString szName = g_PresentMgr.GetPresentValue1Text( PRESENT_EXTRAITEM, iArtifactValue1, 0 );

		int iHours = (iArtifactValue2%10000); // %10000은 선물하기 value2의 장비 기간값과 동일함.				
		if( iHours == 0 )
			SafeSprintf( szMent, sizeof( szMent ), STR(3), iArtifactValue2/10000, szName.c_str() );
		else if( ( iHours/24 ) > 0 )
			SafeSprintf( szMent, sizeof( szMent ), STR(4), iArtifactValue2/10000, szName.c_str(), iHours/24 );
		else
			SafeSprintf( szMent, sizeof( szMent ), STR(5), iArtifactValue2/10000, szName.c_str(), iHours );
	}

	kPrinter.SetTextColor( TCT_DEFAULT_VIOLET );
	kPrinter.AddTextPiece( g_ChatMgr.GetChatFontScale(), szMent );

	ZeroMemory( szMent, sizeof( szMent ) );
	if( iArtifactType < ioExcavationManager::EXTRAITEM_START_TYPE )
	{
		char szComma[MAX_PATH]="";
		int iGradeArray = GetGradeArray( iArtifactValue2 );
		int iPeso = iArtifactValue1;
		iPeso = ( iPeso * GetGradeRate( iGradeArray ) ) * fMapRate;
		Help::ConvertNumToStrComma( iPeso, szComma, sizeof( szComma ) );

		enum { MAX_GRADE = 9, MAX_TEXT = 4, };
		char szGrade[MAX_GRADE][MAX_TEXT]={"A", "B", "C", "D", "E", "F", "S", "SS", "SSS" };
		int iGradeChar = 0;
		if( !COMPARE( iGradeArray, 0, MAX_GRADE ) )
			iGradeArray = 0;
		SafeSprintf( szMent, sizeof( szMent ), STR(6), szGrade[iGradeArray], szComma );
	}
	else
		SafeSprintf( szMent, sizeof( szMent ), STR(7) );

	kPrinter.SetTextColor( g_ChatMgr.GetChatColor() );
	kPrinter.AddTextPiece( g_ChatMgr.GetChatFontScale(), szMent );

	g_ChatMgr.SetChatComplexString( STR(1), kPrinter );
}

void ioExcavationManager::SendEndUsingExcavaingKit()
{
	ioUserEtcItem *pUserEtcItem = g_MyInfo.GetUserEtcItem();
	if( !pUserEtcItem )
	{
		LOG.PrintTimeAndLog( 0, "%s pUserEtcItem == NULL.", __FUNCTION__ );
		return;
	}

	enum { MAX_SIZE = 2, };
	int iEtcItemArray[MAX_SIZE]={ ioEtcItem::EIT_ETC_GOLD_EXCAVATING_KIT, ioEtcItem::EIT_ETC_EXCAVATING_KIT };
	for (int i = 0; i < MAX_SIZE ; i++)
	{
		ioUserEtcItem::ETCITEMSLOT kItemSlot;
		if( pUserEtcItem->GetEtcItem( iEtcItemArray[i], kItemSlot ) )
		{
			if( kItemSlot.IsUse() )
			{
				SP2Packet kPacket( CTPK_ETCITEM_USE );
				kPacket << iEtcItemArray[i];
				kPacket << false; 
				TCPNetwork::SendToServer( kPacket );
				TCPNetwork::MouseBusy( true );
				return;
			}
		}
	}
}

void ioExcavationManager::ProcessPlayState( ioPlayStage *pPlayStage )
{
	ProcessAutoOff( pPlayStage );
	ProcessWaitArtifactPos( pPlayStage );
}


void ioExcavationManager::SendArtifactPostion( ioPlayStage * pPlayStage, int iRoomIndex, DWORD dwArtifactIndex )
{
	if( !pPlayStage )
	{
		LOG.PrintTimeAndLog( 0, "%s pPlayStage == NULL. ", __FUNCTION__ );
		return;
	}

	float fMapMinX = pPlayStage->GetWorldMapBox().GetMinPos().x;
	float fMapMinZ = pPlayStage->GetWorldMapBox().GetMinPos().z;
	float fXSize = pPlayStage->GetWorldMapBox().GetMaxPos().x - fMapMinX;
	float fZSize = pPlayStage->GetWorldMapBox().GetMaxPos().z - fMapMinZ;

	int iXPos = 0;
	int iYPos = 0;
	int iZPos = 0;
	float fMapHeight       = 0.0f;
	float fMapHeightXPlus  = 0.0f;
	float fMapHeightXMinus = 0.0f;
	float fMapHeightZPlus  = 0.0f;
	float fMapHeightZMinus = 0.0f;
	bool  bSuccess   = false;

	enum { MAX_LOOP = 10000, OFFSET = 10, };
	for (int i = 0; i < MAX_LOOP; i++)
	{
		iXPos = m_CrateArtifactXPosRand.Random( (DWORD)fXSize );
		iXPos += (int) fMapMinX;

		iZPos = m_CrateArtifactZPosRand.Random( (DWORD)fZSize );
		iZPos += (int) fMapMinZ;

		// 경사진곳을 피하기 위해서 주위 픽셀을 검사 한다.
		fMapHeight      = pPlayStage->GetMapHeight( (float)iXPos, (float)iZPos );
		fMapHeightXPlus = pPlayStage->GetMapHeight( (float)iXPos+FLOAT10, (float)iZPos );
		fMapHeightXMinus= pPlayStage->GetMapHeight( (float)iXPos-FLOAT10, (float)iZPos );
		fMapHeightZPlus = pPlayStage->GetMapHeight( (float)iXPos, (float)iZPos+FLOAT10 );
		fMapHeightZMinus= pPlayStage->GetMapHeight( (float)iXPos, (float)iZPos-FLOAT10 );

		if( fMapHeight <= 0.0f ) // drop zone
			continue;
		else if( (int)fMapHeight != (int)fMapHeightXPlus ) // 소수점 이하 오차는 무시한다.
			continue;
		else if( (int)fMapHeight != (int)fMapHeightXMinus )
			continue;
		else if( (int)fMapHeight != (int)fMapHeightZPlus )
			continue;
		else if( (int)fMapHeight != (int)fMapHeightZMinus )
			continue;
		else
		{
			iYPos = (int)fMapHeight;
			bSuccess = true;
			break;
		}
	}

	// 랜덤 위치 실패한 경우 서버에 값을 남기기 위해서 초기화값을 보낸다.
	if( !bSuccess )
	{
		iXPos = 0;
		iYPos = 0;
		iZPos = 0;
	}


	SP2Packet kPacket( CTPK_EXCAVATION_COMMAND );
	kPacket << EXCAVATION_ATRTIFACT_POSITION;
	kPacket << iRoomIndex;
	kPacket << dwArtifactIndex;
	kPacket << iXPos;
	kPacket << iYPos;
	kPacket << iZPos;
	TCPNetwork::SendToServer( kPacket );
}

void ioExcavationManager::ProcessWaitArtifactPos( ioPlayStage *pPlayStage )
{
	if( m_dwCurWaitArtifactPosTime == 0 )
		return;

	if( REALGETTIME() - m_dwCurWaitArtifactPosTime < m_dwWaitArtifactPosTime )
		return;

	m_dwCurWaitArtifactPosTime = 0;

	SendArtifactPostion( pPlayStage, m_iReserveRoomIndex, m_dwReserveArtifactIndex );

	m_iReserveRoomIndex      = 0;
	m_dwReserveArtifactIndex = 0;
}

void ioExcavationManager::SendReserveArtifactPosition( ioPlayStage *pPlayStage )
{
	if( m_iReserveRoomIndex == 0 || m_dwReserveArtifactIndex == 0 )
		return;

	SendArtifactPostion( pPlayStage, m_iReserveRoomIndex, m_dwReserveArtifactIndex );

	m_iReserveRoomIndex      = 0;
	m_dwReserveArtifactIndex = 0;
}

void ioExcavationManager::NoticeLevelAndExp()
{
	if( m_kResultInfo.IsEmpty() )
		return;

	// 경험치 알림
	ioComplexStringPrinter kPrinter;
	g_ChatMgr.PresetChatComplexString( MSG_INFO_CHAT, kPrinter );

	kPrinter.SetTextColor( g_ChatMgr.GetChatColor() );
	kPrinter.AddTextPiece( g_ChatMgr.GetChatFontScale(), STR(1) );

	kPrinter.SetTextColor( TCT_DEFAULT_VIOLET );
	kPrinter.AddTextPiece( g_ChatMgr.GetChatFontScale(), STR(2), (int)m_kResultInfo.m_iAddExp );

	if( g_MyInfo.IsCharExerciseStyleToClassType( m_kResultInfo.m_iClassType, EXERCISE_RENTAL ) == false )
	{
		kPrinter.SetTextColor( g_ChatMgr.GetChatColor() );
		kPrinter.AddTextPiece( g_ChatMgr.GetChatFontScale(), STR(3), g_MyInfo.GetClassName( m_kResultInfo.m_iClassType, g_MyInfo.GetCharPowerUpGradeByClassType( m_kResultInfo.m_iClassType ) ) );

		kPrinter.SetTextColor( TCT_DEFAULT_VIOLET );
		kPrinter.AddTextPiece( g_ChatMgr.GetChatFontScale(), STR(4), (int)m_kResultInfo.m_iAddSoldierExp );
	}

	kPrinter.SetTextColor( g_ChatMgr.GetChatColor() );
	kPrinter.AddTextPiece( g_ChatMgr.GetChatFontScale(), STR(5) );
	
	kPrinter.SetTextColor( TCT_DEFAULT_VIOLET );
	kPrinter.AddTextPiece( g_ChatMgr.GetChatFontScale(), STR(6), (int)m_kResultInfo.m_iAddSoldierExp );

	kPrinter.SetTextColor( g_ChatMgr.GetChatColor() );
	kPrinter.AddTextPiece( g_ChatMgr.GetChatFontScale(), STR(7) );

	g_ChatMgr.SetChatComplexString( STR(8), kPrinter );

	if( m_kResultInfo.m_bGradeLevelUp )
	{
		//g_GUIMgr.ShowWnd( GRADE_UP_WND );
	}

	if( g_MyInfo.IsClassLevelUP( m_kResultInfo.m_iClassType ) )   // 용병 레벨업
	{
		int iPeso = g_MyInfo.GetClassLevelUPPeso( m_kResultInfo.m_iClassType );
		SoldierLevelUPWnd *pLevelUPWnd = dynamic_cast<SoldierLevelUPWnd*>(g_GUIMgr.FindWnd( SOLDIER_LEVEL_UP_WND ));
		if( pLevelUPWnd )
			pLevelUPWnd->SetLevelUPData( m_kResultInfo.m_iClassType, iPeso );
		g_MyInfo.InitLevelUPData();
	}

	g_QuestMgr.QuestCompleteTerm( QC_EXCAVATION_LEVELUP );
	// 레벨업 메세지
	if( m_kResultInfo.m_bLevelUp )
	{
		ioComplexStringPrinter kPrinter;
		g_ChatMgr.PresetChatComplexString( MSG_INFO_CHAT, kPrinter );
		kPrinter.SetTextColor( TCT_DEFAULT_VIOLET );
		kPrinter.AddTextPiece( g_ChatMgr.GetChatFontScale(), STR(9), g_MyInfo.GetPublicID().c_str(), (int)g_MyInfo.GetExcavationLevel() );
		g_ChatMgr.SetChatComplexString( STR(10), kPrinter );		
		g_QuestMgr.QuestOccurTerm( QO_EXCAVATION_LEVELUP );
	}
	m_kResultInfo.Clear();
}