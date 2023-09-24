#include "StdAfx.h"
#include "HeroReinforceDownWnd.h"

#include "PowerUpSelectWnd.h"
#include "ReinforceSelectWnd.h"
#include "../ioPowerUpManager.h"
#include "../ioPlayStage.h"

HeroReinforceDownWnd::HeroReinforceDownWnd( void )
{
	m_pNameLine = NULL;
	m_CharInfo.Init();
}

HeroReinforceDownWnd::~HeroReinforceDownWnd( void )
{
	SAFEDELETE( m_pNameLine );
	m_CharInfo.Init();
}

void HeroReinforceDownWnd::iwm_show()
{
	ShowChildWnd( HeroReinforceDownWnd::ID_SOLDIER_WND );
}

void HeroReinforceDownWnd::iwm_hide()
{
	HideChildWnd( HeroReinforceDownWnd::ID_SOLDIER_WND );
}

void HeroReinforceDownWnd::ClearList()
{
	for( int i=0; i < HeroReinforceDownWnd::SOLDIER_INFO_CNT; ++i )
		m_SoldierInfo[i].ClearList();

	for( int i=0; i < SOLDIER_REINFORCE_INFO_CNT; ++i )
		m_SoldierReinforceInfo[i].ClearList();

	m_SoulStonePrinter.ClearList();

	m_SoldierSelectPrinter.ClearList();
}

void HeroReinforceDownWnd::SetSoldierInfo( int iClassType )
{
	if ( g_MyInfo.GetClassArray( iClassType ) == -1 )
	{
		LOG.PrintTimeAndLog( 0, "HeroReinforceDownWnd::SetSoldierInfo - iClassType is not exist" );
		return;
	}

	m_CharInfo.Init();
	m_CharInfo = g_MyInfo.GetCharacterToClassType( iClassType );
	NewShopHelpWnd *pCharWnd = dynamic_cast<NewShopHelpWnd *>( FindChildWnd( HeroReinforceDownWnd::ID_SOLDIER_WND ) );
	if( pCharWnd )
	{
		pCharWnd->SetZPosition( 500.0f );
		pCharWnd->SetChar( m_CharInfo );
		pCharWnd->SetEquip( false, false, false );
		pCharWnd->SetUserInfoToolTip( true );
		pCharWnd->SetCharScale( 0.85f );
	}


	int iSoulStoneCount = 0;
	ioUserEtcItem *pUserEtcItem = g_MyInfo.GetUserEtcItem();
	if( pUserEtcItem )
	{
		ioUserEtcItem::ETCITEMSLOT kSlot;
		if( pUserEtcItem->GetEtcItem( ioEtcItem::EIT_ETC_SOUL_STONE, kSlot ) )
			iSoulStoneCount = kSlot.m_iValue1;
	}

	//용병 정보

	char szBuf[MAX_PATH], szText[MAX_PATH];
	memset( szBuf, 0, sizeof(szBuf) );
	memset( szText, 0, sizeof(szText) );

	if( m_CharInfo.m_eCharPowerUpType == PUGT_NONE )
		SafeSprintf( szText, sizeof( szText ), STR(2) );
	else
	{
		ioHashString szGradeTitle = g_PowerUpManager.GetCharGradeTitle( m_CharInfo.m_class_type, m_CharInfo.m_eCharPowerUpType );
		if ( szGradeTitle.IsEmpty() )
			return;
		SafeSprintf( szText, sizeof( szText ), STR(4), szGradeTitle.c_str() );
	}

	const ioHashString& szClassName = g_MyInfo.GetClassName( m_CharInfo.m_class_type ) ;
	//SafeSprintf( szBuf, sizeof( szBuf ), "Lv%d %s", g_MyInfo.GetClassLevel( m_CharInfo.m_class_type, false ), szClassName.c_str() );
	//StringCbPrintf_e( szBuf, sizeof( szBuf ), "Lv%d %s" , g_MyInfo.GetClassLevel( m_CharInfo.m_class_type, false ), szClassName.c_str() );
	ClearList();

	//용병 레벨 및 이름
	m_SoldierInfo[0].SetTextStyle( TS_NORMAL );
	m_SoldierInfo[0].SetBkColor( 0, 0, 0 );
	m_SoldierInfo[0].SetTextColor( g_UIImageRenderMgr.GetGradeTextColor( g_UIImageRenderMgr.GetPowerUpGradeType( iClassType ) ) );
	m_SoldierInfo[0].AddTextPiece( FONT_SIZE_12, STR(1), g_MyInfo.GetClassLevel( m_CharInfo.m_class_type, false ), szClassName.c_str()  );

	//영구 용병
	m_SoldierInfo[1].SetTextStyle( TS_NORMAL );
	m_SoldierInfo[1].SetBkColor( 0, 0, 0 );	
	m_SoldierInfo[1].SetTextColor( TCT_DEFAULT_BLUE );
	m_SoldierInfo[1].AddTextPiece( FONT_SIZE_12, szText );

	for( int i=0; i < SOLDIER_REINFORCE_INFO_CNT; ++i )
		g_PowerUpManager.SetCharGardePrinter( m_CharInfo.m_class_type, i,  FONT_SIZE_12, m_SoldierReinforceInfo[i] );

	memset( szText, 0, sizeof(szText) );
	//SafeSprintf( szText, sizeof( szText ), STR(3), iSoulStoneCount );

	m_SoulStonePrinter.SetTextStyle( TS_NORMAL );
	m_SoulStonePrinter.SetBkColor( 0, 0, 0 );	
	m_SoulStonePrinter.SetTextColor( TCT_DEFAULT_RED );
	SafeSprintf( szText, sizeof( szText ), STR(3) , iSoulStoneCount );
	m_SoulStonePrinter.AddTextPiece( FONT_SIZE_12, szText );

	m_SoldierSelectPrinter.SetTextStyle( TS_NORMAL );
	m_SoldierSelectPrinter.SetBkColor( 0, 0, 0 );
	m_SoldierSelectPrinter.SetTextColor( g_UIImageRenderMgr.GetGradeTextColor( g_UIImageRenderMgr.GetPowerUpGradeType( iClassType ) ) );
	m_SoldierSelectPrinter.AddTextPiece( FONT_SIZE_12, szClassName.c_str() );
}

void HeroReinforceDownWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();	
	switch( dwID )
	{
	case ID_ENABLE_REINFORECE_SOLDIER_SELECT_BTN:
		if( cmd == IOBN_BTNUP )
		{
			PowerUpSelectWnd *pWnd = dynamic_cast<PowerUpSelectWnd*>( g_GUIMgr.FindWnd( POWERUP_SELECT_WND ) );
			if( pWnd )
			{
				if( pWnd->IsShow() )
					pWnd->GoToTop();
				else
					pWnd->ShowSubWnd( ioPowerUpManager::PIT_HERO );
			}
		}
		break;

	case ID_REINFORCE_BTN:
		if( cmd == IOBN_BTNUP )
		{
			int iCharArray = g_MyInfo.GetClassArray( m_CharInfo.m_class_type );
			if( iCharArray == g_MyInfo.GetSelectCharArray() )
			{
				bool bCheck = true;
				MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(g_GUIMgr.FindWnd(MY_INVENTORY_WND));
				if( pInvenWnd && pInvenWnd->GetPlayStage() )
				{
					ioMyRoomMode *pMyRoom = ToMyRoomMode( pInvenWnd->GetPlayStage()->GetPlayMode() );
					if( pMyRoom && pMyRoom->GetMyRoomType() == MRT_LOBBY )
						bCheck = false;
				}

				if( bCheck )
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
					break;
				}
			}

			int iUseMtrlCount,iCurMtrlCount;
			if( !g_PowerUpManager.CheckEnableSoulStone( ioPowerUpManager::PIT_HERO ,m_CharInfo.m_class_type, iUseMtrlCount, iCurMtrlCount ) )
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

			g_GUIMgr.SetPrevMsgListBox( NULL, MB_YESNO, this, kPrinter );
		}
		break;
	case MESSAGE_BOX3:
		{
			if( cmd == IOBN_BTNUP )
			{
				if( param == IDYES )
				{
					int iClassType	= m_CharInfo.m_class_type;
					int iUseMtrlCount,iCurMtrlCount;
					if( !g_PowerUpManager.CheckEnableSoulStone( ioPowerUpManager::PIT_HERO ,iClassType, iUseMtrlCount, iCurMtrlCount ) )
					{
						SoulStoneBuyWnd *pSoulstoneBuyWnd = dynamic_cast<SoulStoneBuyWnd*> ( g_GUIMgr.FindWnd( SOULSTONE_BUY_WND ) );
						if ( pSoulstoneBuyWnd )
							pSoulstoneBuyWnd->ShowBuyWnd( iUseMtrlCount, iCurMtrlCount );
						break;
					}

					ioHashString szError;
					if( g_PowerUpManager.CheckEnableCharPowerUp( iClassType, szError ) )
					{
						//CheckEnableCharPowerUp함수에서 유효한 값인지 체크됨
						int iCharIndex = g_MyInfo.GetCharIndexByClassType( iClassType );
						ioEtcItemSoulStone* pEtcItem = dynamic_cast<ioEtcItemSoulStone*>( g_EtcItemMgr.FindEtcItem( g_PowerUpManager.GetMtrlEtcItemCode() ) );
						if( pEtcItem )
						{
							pEtcItem->OnUseCharPowerUp( iCharIndex );
						}
					}
				}
			}
		}
		break;
	}
}

void HeroReinforceDownWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "NameLine" )
	{
		SAFEDELETE( m_pNameLine );
		m_pNameLine = pImage;
	}
	else
		ioWnd::AddRenderImage( szType, pImage );
}

void HeroReinforceDownWnd::OnRender()
{
	ioWnd::OnRender();

	int nXPos = GetDerivedPosX();
	int nYPos = GetDerivedPosY();

	enum 
	{	
		SOLDIER_REINFORCE_INFO_OFFSET_X = 33,
		SOLDIER_REINFORCE_INFO_OFFSET_Y = 325,
		SOLDIER_REINFORCE_LINE_GAP	   = 21,

		SOULSTONE_OFFSET_X = 128,
		SOULSTONE_OFFSET_Y = 407,
	};

	for( int i=0; i < SOLDIER_REINFORCE_INFO_CNT; ++i )
	{
		if( m_SoldierReinforceInfo[i].IsEmpty() )
			break;

		m_SoldierReinforceInfo[i].PrintFullText( nXPos+SOLDIER_REINFORCE_INFO_OFFSET_X, nYPos+SOLDIER_REINFORCE_INFO_OFFSET_Y+(SOLDIER_REINFORCE_LINE_GAP*i), TAT_LEFT );
	}

	if ( m_SoulStonePrinter.IsEmpty() )
		return;

	m_SoulStonePrinter.PrintFullText( nXPos+SOULSTONE_OFFSET_X, nYPos+SOULSTONE_OFFSET_Y, TAT_CENTER );
}

void HeroReinforceDownWnd::OnRenderAfterChild()
{
	int nXPos = GetDerivedPosX();
	int nYPos = GetDerivedPosY();
	enum 
	{	
		LINE_OFFSET_X = 28, 
		LINE_OFFSET_Y = 288,

		SOLDIER_INFO_OFFSET_X = 128,
		SOLDIER_INFO_OFFSET_Y = 285,
		SOLDIER_LINE_GAP	   = 16,

		SOULDIER_SELECT_OFFSET_X = 131,
		SOULDIER_SELECT_OFFSET_Y = 428,
	};

	if ( !m_pNameLine ) 		return;

	m_pNameLine->Render( nXPos+LINE_OFFSET_X, nYPos+LINE_OFFSET_Y );

	for( int i=0; i < HeroReinforceDownWnd::SOLDIER_INFO_CNT; ++i )
	{
		if( m_SoldierInfo[i].IsEmpty() )
			break;

		m_SoldierInfo[i].PrintFullText( nXPos+SOLDIER_INFO_OFFSET_X, nYPos+SOLDIER_INFO_OFFSET_Y+(SOLDIER_LINE_GAP*i), TAT_CENTER );
	}

	m_SoldierSelectPrinter.PrintFullText( nXPos+SOULDIER_SELECT_OFFSET_X, nYPos+SOULDIER_SELECT_OFFSET_Y, TAT_CENTER );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HeroReinforceDoneWnd::HeroReinforceDoneWnd( void )
{
	m_pNameLine = NULL;

	m_szEffectName.Clear();
	m_szSuccessSound.Clear();
}

HeroReinforceDoneWnd::~HeroReinforceDoneWnd( void )
{
	SAFEDELETE( m_pNameLine );
}

void HeroReinforceDoneWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "NameLine" )
	{
		SAFEDELETE( m_pNameLine );
		m_pNameLine = pImage;
	}
	else
		ioWnd::AddRenderImage( szType, pImage );
}

void HeroReinforceDoneWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	ioWnd::ParseExtraInfo( xElement );
	char szKeyName[MAX_PATH]="";
	ZeroMemory( szKeyName, sizeof( szKeyName ) );
	StringCbPrintf( szKeyName, sizeof( szKeyName ), "powerup_effect" );
	m_szEffectName = xElement.GetStringAttribute( szKeyName );

	ZeroMemory( szKeyName, sizeof( szKeyName ) );
	StringCbPrintf( szKeyName, sizeof( szKeyName ), "success_sound" );
	m_szSuccessSound = xElement.GetStringAttribute( szKeyName );
}

void HeroReinforceDoneWnd::ClearList()
{
	for( int i=0; i < HeroReinforceDoneWnd::SOLDIER_INFO_CNT; ++i )
		m_SoldierInfo[i].ClearList();

	for( int i=0; i < HeroReinforceDoneWnd::REINFORCED_SOLDIER_INFO_CNT; ++i )
		m_ReinforcedSoldierInfo[i].ClearList();

	m_SoulStonePrinter.ClearList();
	m_ReinforcedPrinter.ClearList();
}

void HeroReinforceDoneWnd::iwm_show()
{
	ShowChildWnd( HeroReinforceDoneWnd::ID_SOLDIER_WND );
}

void HeroReinforceDoneWnd::iwm_hide()
{
	HideChildWnd( HeroReinforceDoneWnd::ID_SOLDIER_WND );
	if( !m_szSuccessSound.IsEmpty() )
		g_SoundMgr.StopSound( m_szSuccessSound, 0 );
}

void HeroReinforceDoneWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();	
	switch( dwID )
	{
	case ID_REINFORECE_AGAIN_BTN:
		{
			if( cmd == IOBN_BTNUP )
			{
				int iCharArray = g_MyInfo.GetClassArray( m_CharInfo.m_class_type );
				if( iCharArray == g_MyInfo.GetSelectCharArray() )
				{
					bool bCheck = true;
					MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(g_GUIMgr.FindWnd(MY_INVENTORY_WND));
					if( pInvenWnd && pInvenWnd->GetPlayStage() )
					{
						ioMyRoomMode *pMyRoom = ToMyRoomMode( pInvenWnd->GetPlayStage()->GetPlayMode() );
						if( pMyRoom && pMyRoom->GetMyRoomType() == MRT_LOBBY )
							bCheck = false;
					}

					if( bCheck )
					{
						g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
						break;
					}
				}

				int iUseMtrlCount,iCurMtrlCount;
				if( !g_PowerUpManager.CheckEnableSoulStone( ioPowerUpManager::PIT_HERO ,m_CharInfo.m_class_type, iUseMtrlCount, iCurMtrlCount ) )
				{
					SoulStoneBuyWnd *pSoulstoneBuyWnd = dynamic_cast<SoulStoneBuyWnd*> ( g_GUIMgr.FindWnd( SOULSTONE_BUY_WND ) );
					if ( pSoulstoneBuyWnd )
						pSoulstoneBuyWnd->ShowBuyWnd( iUseMtrlCount, iCurMtrlCount );
					break;
				}

				ioHashString szError;
				if( !g_PowerUpManager.CheckEnableCharPowerUp( m_CharInfo.m_class_type, szError ) )
				{
					g_GUIMgr.SetMsgBox( MB_OK, this, szError.c_str() );
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

				g_GUIMgr.SetPrevMsgListBox( NULL, MB_YESNO, this, kPrinter );
			}
		}
		break;
	case ID_SEE_HERO_BTN:
		{
			if( cmd == IOBN_BTNUP )
			{
				MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*> ( GetParent() );
				if( pInvenWnd && pInvenWnd->IsShow() )
				{
					HideWnd();
					pInvenWnd->ShowSoldierTabDirect( m_CharInfo.m_class_type );
				}
			}
		}
	case MESSAGE_BOX3:
		{
			if( cmd == IOBN_BTNUP )
			{
				if( param == IDYES )
				{
					int iClassType	= m_CharInfo.m_class_type;	
					int iUseMtrlCount,iCurMtrlCount;
					if ( !g_PowerUpManager.CheckEnableSoulStone( ioPowerUpManager::PIT_HERO, iClassType, iUseMtrlCount, iCurMtrlCount ) )
					{
						SoulStoneBuyWnd *pSoulstoneBuyWnd = dynamic_cast<SoulStoneBuyWnd*> ( g_GUIMgr.FindWnd( SOULSTONE_BUY_WND ) );
						if ( pSoulstoneBuyWnd )
							pSoulstoneBuyWnd->ShowBuyWnd( iUseMtrlCount, iCurMtrlCount );
						break;
					}
					ioHashString szError;
					if( g_PowerUpManager.CheckEnableCharPowerUp( iClassType, szError ) )
					{
						//CheckEnableCharPowerUp함수에서 유효한 값인지 체크됨
						int iCharIndex = g_MyInfo.GetCharIndexByClassType( iClassType );
						ioEtcItemSoulStone* pEtcItem = dynamic_cast<ioEtcItemSoulStone*>( g_EtcItemMgr.FindEtcItem( g_PowerUpManager.GetMtrlEtcItemCode() ) );
						if( pEtcItem )
						{
							pEtcItem->OnUseCharPowerUp( iCharIndex );
						}
					}
				}
			}
		}
		break;
	}
}

void HeroReinforceDoneWnd::OnRender()
{
	ioWnd::OnRender();

	int nXPos = GetDerivedPosX();
	int nYPos = GetDerivedPosY();

	enum 
	{	
		SOLDIER_REINFORCED_INFO_OFFSET_X = 34,
		SOLDIER_REINFORCED_INFO_OFFSET_Y = 323,
		SOLDIER_REINFORCED_LINE_GAP	   = 21,

		SOULSTONE_OFFSET_X = 128,
		SOULSTONE_OFFSET_Y = 407,

		REINFORCED_OFFSET_X = 128,
		REINFORCED_OFFSET_Y = 424,
	};

	for( int i=0; i < REINFORCED_SOLDIER_INFO_CNT; ++i )
	{
		if( m_ReinforcedSoldierInfo[i].IsEmpty() )
			break;

		m_ReinforcedSoldierInfo[i].PrintFullText( nXPos+SOLDIER_REINFORCED_INFO_OFFSET_X, nYPos+SOLDIER_REINFORCED_INFO_OFFSET_Y+(SOLDIER_REINFORCED_LINE_GAP*i), TAT_LEFT );
	}

	if ( !m_SoulStonePrinter.IsEmpty() )
		m_SoulStonePrinter.PrintFullText( nXPos+SOULSTONE_OFFSET_X, nYPos+SOULSTONE_OFFSET_Y, TAT_CENTER );

	if ( !m_ReinforcedPrinter.IsEmpty() )
		m_ReinforcedPrinter.PrintFullText( nXPos+REINFORCED_OFFSET_X, nYPos+REINFORCED_OFFSET_Y, TAT_CENTER );

}

void HeroReinforceDoneWnd::OnRenderAfterChild()
{
	int nXPos = GetDerivedPosX();
	int nYPos = GetDerivedPosY();

	enum 
	{	
		LINE_OFFSET_X = 28, 
		LINE_OFFSET_Y = 288,

		SOLDIER_INFO_OFFSET_X = 128,
		SOLDIER_INFO_OFFSET_Y = 285,
		SOLDIER_LINE_GAP	   = 16,
	};
	if ( !m_pNameLine ) return;

	m_pNameLine->Render( nXPos+LINE_OFFSET_X, nYPos+LINE_OFFSET_Y );

	for( int i=0; i < HeroReinforceDoneWnd::SOLDIER_INFO_CNT; ++i )
	{
		if( m_SoldierInfo[i].IsEmpty() )
			break;

		m_SoldierInfo[i].PrintFullText( nXPos+SOLDIER_INFO_OFFSET_X, nYPos+SOLDIER_INFO_OFFSET_Y+(SOLDIER_LINE_GAP*i), TAT_CENTER );
	}
}

void HeroReinforceDoneWnd::SetSoldierInfo( int iClassType )
{
	if ( g_MyInfo.GetClassArray( iClassType ) == -1 )
	{
		LOG.PrintTimeAndLog( 0, "HeroReinforceDoneWnd::UpdateSoldierInfo - iClassType is not exist" );
		HideChildWnd( HeroReinforceDoneWnd::ID_SOLDIER_WND );
		return;
	}

	m_CharInfo.Init();
	m_CharInfo = g_MyInfo.GetCharacterToClassType( iClassType );
	NewShopHelpWnd *pCharWnd = dynamic_cast<NewShopHelpWnd *>( FindChildWnd( HeroReinforceDoneWnd::ID_SOLDIER_WND ) );
	if( pCharWnd )
	{
		pCharWnd->SetZPosition( 500.0f );
		pCharWnd->SetChar( m_CharInfo );
		pCharWnd->SetEquip( false, false, false );
		pCharWnd->SetUserInfoToolTip( true );
		pCharWnd->SetCharScale( 0.85f );
		if ( !m_szEffectName.IsEmpty() )
		{
			pCharWnd->CharAttachEffect( m_szEffectName );
			if( !m_szSuccessSound.IsEmpty() )
				g_SoundMgr.PlaySound( m_szSuccessSound, DSBVOLUME_MAX, PSM_THREAD );
		}
	}

	int iSoulStoneCount = 0;
	ioUserEtcItem *pUserEtcItem = g_MyInfo.GetUserEtcItem();
	if( pUserEtcItem )
	{
		ioUserEtcItem::ETCITEMSLOT kSlot;
		if( pUserEtcItem->GetEtcItem( ioEtcItem::EIT_ETC_SOUL_STONE, kSlot ) )
			iSoulStoneCount = kSlot.m_iValue1;
	}

	//용병 정보

	char szBuf[MAX_PATH], szText[MAX_PATH];
	memset( szBuf, 0, sizeof(szBuf) );
	memset( szText, 0, sizeof(szText) );

	SafeSprintf( szBuf, sizeof( szBuf ), STR(1), g_MyInfo.GetClassLevel( m_CharInfo.m_class_type, false ), g_MyInfo.GetClassName( m_CharInfo.m_class_type ) );

	ioHashString szGradeTitle = g_PowerUpManager.GetCharGradeTitle( m_CharInfo.m_class_type, m_CharInfo.m_eCharPowerUpType );
	if ( szGradeTitle.IsEmpty() )
		return;
	SafeSprintf( szText, sizeof( szText ), STR(2), szGradeTitle.c_str() );

	ClearList();
	//용병 레벨 및 이름
	m_SoldierInfo[0].SetTextStyle( TS_NORMAL );
	m_SoldierInfo[0].SetBkColor( 0, 0, 0 );
	m_SoldierInfo[0].SetTextColor( g_UIImageRenderMgr.GetGradeTextColor( g_UIImageRenderMgr.GetPowerUpGradeType( iClassType ) ) );
	m_SoldierInfo[0].AddTextPiece( FONT_SIZE_12, szBuf );

	//영구 용병
	m_SoldierInfo[1].SetTextStyle( TS_NORMAL );
	m_SoldierInfo[1].SetBkColor( 0, 0, 0 );	
	m_SoldierInfo[1].SetTextColor( TCT_DEFAULT_BLUE );
	m_SoldierInfo[1].AddTextPiece( FONT_SIZE_12, szText );

	//진화된 내용 설명
	for( int i=0; i < REINFORCED_SOLDIER_INFO_CNT; ++i )
		g_PowerUpManager.SetCharGardedPrinter( m_CharInfo.m_class_type, m_CharInfo.m_eCharPowerUpType, i,  FONT_SIZE_12, m_ReinforcedSoldierInfo[i] );

	//soultone 보유 개수
	memset( szText, 0, sizeof(szText) );
	SafeSprintf( szText, sizeof( szText ), STR(3), iSoulStoneCount );
	m_SoulStonePrinter.SetTextStyle( TS_NORMAL );
	m_SoulStonePrinter.SetBkColor( 0, 0, 0 );	
	m_SoulStonePrinter.SetTextColor( TCT_DEFAULT_RED );
	m_SoulStonePrinter.AddTextPiece( FONT_SIZE_12, szText );

	memset( szText, 0, sizeof(szText) );
	SafeSprintf( szText, sizeof( szText ), STR(4),  szGradeTitle.c_str() );
	m_ReinforcedPrinter.SetTextStyle( TS_NORMAL );
	m_ReinforcedPrinter.SetBkColor( 0, 0, 0 );	
	m_ReinforcedPrinter.SetTextColor( TCT_DEFAULT_GREEN );
	m_ReinforcedPrinter.AddTextPiece( FONT_SIZE_17, szText );
}
