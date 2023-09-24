#include "StdAfx.h"

#include "../ioShuffleRoomMgr.h"
#include "../ioSoldierAwakeManager.h"

#include "TotalResultRewardWnd.h"

//////////////////////////////////////////////////////////////////////////
FSMStateMgr TotalResultExpCharSlotWnd::sg_LevelUpAni;
float TotalResultExpCharSlotWnd::sg_fLevelUpAlpha = 0.0f;
TotalResultExpCharSlotWnd::TotalResultExpCharSlotWnd()
{
	m_pUI3DRender = NULL;
	m_aMyChar     = NULL;

	m_pCharBottom = NULL;
	m_pGaugeBar	  = NULL;
	m_pGaugeOrange= NULL;
	m_pGaugeYellow= NULL;
	m_pLevelUP = NULL;
	m_pExpBonus= NULL;

	m_iClassType  = -1;
	m_iClassPoint = -1;

	m_fCharXPos = m_fCharYPos = m_fCharZPos = 0.0f;
	m_iCharOffSetX = m_iCharOffSetY = 0;
	m_iTitleX = m_iTitleY = 0;

	m_dwGaugeTime  = 0;
	m_dwCurrentTime= 0;
	m_fCurrentPer  = 0.0f;

	m_bSoldierExpBonus = false;
}

TotalResultExpCharSlotWnd::~TotalResultExpCharSlotWnd()
{
	SAFEDELETE( m_aMyChar );
	SAFEDELETE( m_pUI3DRender );

	SAFEDELETE( m_pCharBottom );
	SAFEDELETE( m_pGaugeBar );
	SAFEDELETE( m_pGaugeOrange );
	SAFEDELETE( m_pGaugeYellow );
	SAFEDELETE( m_pLevelUP );
	SAFEDELETE( m_pExpBonus );

	if( !m_szCharGaugeSound.IsEmpty() )
		g_SoundMgr.StopSound( m_szCharGaugeSound, 0 );

	if( !m_szCharEffectSound.IsEmpty() )
		g_SoundMgr.StopSound( m_szCharEffectSound, 0 );
}

void TotalResultExpCharSlotWnd::InitLevelUpAni()
{
	enum { FADE_IN = 300, FADE_DELAY_IN = 300, FADE_OUT = 300, FADE_DELAY_OUT = 300, };

	sg_LevelUpAni.SetCurState( LUA_NONE );

	FSMState *pState = new FSMState( LUA_NONE );
	if( pState )
		pState->Add( LUA_NONE, LUA_START );
	sg_LevelUpAni.Add( pState );

	pState = new FSMState( LUA_START );
	if( pState )
		pState->Add( LUA_START, LUA_FADEIN, 0 );
	sg_LevelUpAni.Add( pState );

	pState = new FSMState( LUA_FADEIN );
	if( pState )
		pState->Add( LUA_FADEIN,  LUA_DELAY_IN, FADE_IN );
	sg_LevelUpAni.Add( pState );

	pState = new FSMState( LUA_DELAY_IN );
	if( pState )
		pState->Add( LUA_DELAY_IN,  LUA_FADEOUT , FADE_DELAY_IN );
	sg_LevelUpAni.Add( pState );	

	pState = new FSMState( LUA_FADEOUT );
	if( pState )
		pState->Add( LUA_FADEOUT,  LUA_DELAY_OUT , FADE_OUT );
	sg_LevelUpAni.Add( pState );

	pState = new FSMState( LUA_DELAY_OUT );
	if( pState )
		pState->Add( LUA_DELAY_OUT,  LUA_FADEIN , FADE_DELAY_OUT );
	sg_LevelUpAni.Add( pState );	

	sg_LevelUpAni.SetCurState( LUA_START );
}

void TotalResultExpCharSlotWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "CharBottom" )
	{
		SAFEDELETE( m_pCharBottom );
		m_pCharBottom = pImage;
	}
	else if( szType == "GaugeBar" )
	{
		SAFEDELETE( m_pGaugeBar );
		m_pGaugeBar = pImage;
	}
	else if( szType == "GaugeOrange" )
	{
		SAFEDELETE( m_pGaugeOrange );
		m_pGaugeOrange = pImage;
	}
	else if( szType == "GaugeYellow" )
	{
		SAFEDELETE( m_pGaugeYellow );
		m_pGaugeYellow = pImage;
	}
	else if( szType == "LevelUP" )
	{
		SAFEDELETE( m_pLevelUP );
		m_pLevelUP = pImage;
	}
	else if( szType == "ExpBonus" )
	{
		SAFEDELETE( m_pExpBonus );
		m_pExpBonus= pImage;
	}
	else
	{
		ioWnd::AddRenderImage( szType, pImage );
	}
}

void TotalResultExpCharSlotWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	m_fCharXPos = xElement.GetIntAttribute_e( "CharX" );
	m_fCharYPos = xElement.GetIntAttribute_e( "CharY" );
	m_fCharZPos = xElement.GetIntAttribute_e( "CharZ" );
	m_iCharOffSetX = xElement.GetIntAttribute_e( "CharOffSetX" );
	m_iCharOffSetY = xElement.GetIntAttribute_e( "CharOffSetY" );

	m_iTitleX = xElement.GetIntAttribute_e( "TitleX" );
	m_iTitleY = xElement.GetIntAttribute_e( "TitleY" );

	m_szCharGaugeSound  = xElement.GetStringAttribute_e( "CharGaugeSound" );
	m_szCharEffectSound = xElement.GetStringAttribute_e( "CharEffectSound" );
}

void TotalResultExpCharSlotWnd::SetFinalRoundPoint( int iClassType, int iClassPoint, int iAniCode /* = 0  */ )
{
	m_iClassType = iClassType;
	m_iClassPoint= iClassPoint;

	int iClassArray = g_MyInfo.GetClassArray( m_iClassType );
	if( iClassArray == -1 )
		return;

	const CHARACTER &rkCharInfo = g_MyInfo.GetCharacter( iClassArray );

	// ĳ����
	ITEM_DATA kEquipItem[MAX_INVENTORY];
	for(int i = 0;i < MAX_INVENTORY;i++)
	{
		kEquipItem[i].m_item_code = g_MyInfo.GetCharItem( iClassArray, i ).m_item_code;
		kEquipItem[i].m_item_male_custom = g_MyInfo.GetCharItem( iClassArray, i ).m_item_male_custom;
		kEquipItem[i].m_item_female_custom = g_MyInfo.GetCharItem( iClassArray, i ).m_item_female_custom;
	}
	CreateCharacter( rkCharInfo, (const ITEM_DATA*)&kEquipItem, iAniCode, "" );
}

void TotalResultExpCharSlotWnd::StartGauge( DWORD dwGaugeTime )
{
	m_dwGaugeTime  = dwGaugeTime;
	m_dwCurrentTime= FRAMEGETTIME();
	m_fCurrentPer  = 0.0f;

	//
	if( GetID() == TotalResultExpWnd::ID_CHAR_1 )
	{
		InitLevelUpAni();
	}

	if( !m_szCharGaugeSound.IsEmpty() )
		g_SoundMgr.PlaySound( m_szCharGaugeSound );
}

void TotalResultExpCharSlotWnd::CreateCharacter( const CHARACTER &rkInfo, const ITEM_DATA* pItem, int iAniCode, const ioHashString &szAniName )
{
	ioCreateChar *pChar = m_pUI3DRender->CreateChar( rkInfo, "AwardChar" );

	D3DXVECTOR3 vCharPos( m_fCharXPos, m_fCharYPos, m_fCharZPos );
	pChar->SetPosition( vCharPos );
	pChar->SetYaw( FLOAT10 );

	m_pUI3DRender->SetLookAt( vCharPos );
	
	// �ڽ�Ƭ ���� (����ġ ���)
	const ioSetItemInfo *pSetInfo = g_SetItemInfoMgr.GetSetInfoByIdx( rkInfo.m_class_type - 1 );
	if( pSetInfo )
	{
		ConstItemVector rkItemList = pSetInfo->GetSetItemList();

		int iMaxItemCnt = min( rkItemList.size(), MAX_INVENTORY );
		for(int i = 0;i < iMaxItemCnt;i++)
		{
			ITEM_DATA kItem = pItem[i];
			int iItemCode = kItem.m_item_code;
			DWORD dwCostumeMaleCustom = rkInfo.m_EquipCostume[i].m_Costume_Male_Custom;
			DWORD dwCostumeFemaleCustom = rkInfo.m_EquipCostume[i].m_Costume_Female_Custom;
			bool bCostume = true;
			if( iItemCode == 0 )
			{
				if ( rkInfo.m_EquipCostume[i].m_CostumeCode <= 0 )
				{
					dwCostumeMaleCustom = 0;
					dwCostumeFemaleCustom = 0;
					bCostume = false;
				}
				pChar->EquipItem( rkItemList[i]->GetItemCode(), dwCostumeMaleCustom, dwCostumeFemaleCustom, rkInfo.m_EquipCostume[i].m_CostumeCode, bCostume );
			}
			else
			{
				if ( rkInfo.m_EquipCostume[i].m_CostumeCode <= 0 )
				{
					dwCostumeMaleCustom = kItem.m_item_male_custom;
					dwCostumeFemaleCustom = kItem.m_item_female_custom;
					bCostume = false;
				}
				pChar->EquipItem( kItem.m_item_code, dwCostumeMaleCustom, dwCostumeFemaleCustom, rkInfo.m_EquipCostume[i].m_CostumeCode, bCostume );
			}
		}
	}

	if( !szAniName.IsEmpty() )
		pChar->SetLoopAni( szAniName );
	else // �ִϸ��̼� �ڵ�
		pChar->SetAwardWeaponMotion( iAniCode );

	SAFEDELETE( m_aMyChar );
	m_aMyChar = pChar;

	if( GetID() == TotalResultExpWnd::ID_CHAR_1 )
		m_aMyChar->AttachEffect( "reward_bg_eff.txt", NULL );

	if( !m_szAwakeEffect.IsEmpty() )
		pChar->EndEffect( m_szAwakeEffect, true );

	m_szAwakeEffect = g_AwakeMgr.GetAwakeEffect( static_cast<AwakeType>( rkInfo.m_iAwakeType ) ).c_str();
	pChar->AttachEffect( m_szAwakeEffect, NULL );
}

void TotalResultExpCharSlotWnd::iwm_create()
{
	SAFEDELETE( m_pUI3DRender );

	m_pUI3DRender = new ioUI3DRender;
	m_pUI3DRender->Initialize( "config/sp2_stage_charselect_map.ini", "award_cam" );
}

void TotalResultExpCharSlotWnd::iwm_show()
{
	if( m_iClassType <= 0 )
	{
		HideWnd();
	}
	else
	{
		ioUserEtcItem *pUserEtcItem = g_MyInfo.GetUserEtcItem();
		if( pUserEtcItem )
		{
			ioUserEtcItem::ETCITEMSLOT kSlot;
			if( pUserEtcItem->GetEtcItem( ioEtcItem::EIT_ETC_SOLDIER_EXP_BONUS, kSlot ) )
			{
				if( kSlot.GetDateExcludeValue2() == m_iClassType )
				{
					ioEtcItemSoldierExpBonus *pEtcItem = dynamic_cast< ioEtcItemSoldierExpBonus * >( g_EtcItemMgr.FindEtcItem( ioEtcItem::EIT_ETC_SOLDIER_EXP_BONUS ) );
					if( pEtcItem )
					{
						int iClassLevel = g_MyInfo.GetClassLevel( m_iClassType, true );
						if( iClassLevel == pEtcItem->GetLimitLevel() )
						{
							if( g_MyInfo.IsClassLevelUP( m_iClassType ) )
								m_bSoldierExpBonus = true;
						}
						else if( iClassLevel < pEtcItem->GetLimitLevel() )
						{							
								m_bSoldierExpBonus = true;
						}
					}
				}
			}
		}
	}
}

void TotalResultExpCharSlotWnd::iwm_hide()
{

}

void TotalResultExpCharSlotWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;

	if( m_aMyChar )
	{		
		m_aMyChar->Update( fTimePerSec );
	}

	if( m_dwGaugeTime > 0 )
	{
		DWORD dwGapTime = FRAMEGETTIME() - m_dwCurrentTime;
		if( dwGapTime >= m_dwGaugeTime )
		{
			m_dwGaugeTime = 0;
			if( m_aMyChar )
			{
				m_aMyChar->AttachEffect( "exp_up.txt", NULL );
				if( !m_szCharEffectSound.IsEmpty() )
					g_SoundMgr.PlaySound( m_szCharEffectSound );
			}
		}
		else
		{
			m_fCurrentPer = (float)dwGapTime / (float)m_dwGaugeTime;
		}
	}
	
	OnProcessLevelUpAni();
}

void TotalResultExpCharSlotWnd::OnProcessLevelUpAni()
{
	if( GetID() != TotalResultExpWnd::ID_CHAR_1 ) return;
	if( sg_LevelUpAni.GetCurState() == LUA_NONE ) return;

	sg_LevelUpAni.UpdateTime();
	switch( sg_LevelUpAni.GetCurState() )
	{
	case LUA_FADEIN:
		{
			sg_fLevelUpAlpha = min( FLOAT1, (float)( sg_LevelUpAni.GetCurTime() ) / sg_LevelUpAni.GetDelay() );
		}
		break;
	case LUA_DELAY_IN:
		{
			sg_fLevelUpAlpha = FLOAT1;
		}
		break;
	case LUA_FADEOUT:
		{
			sg_fLevelUpAlpha = max( 0.0f, FLOAT1 - ( (float)( sg_LevelUpAni.GetCurTime() ) / sg_LevelUpAni.GetDelay() ) );
		}
		break;
	case LUA_DELAY_OUT:
		{
			sg_fLevelUpAlpha = 0.0f;
		}
		break;
	}
}

void TotalResultExpCharSlotWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos, iYPos;
	iXPos = GetDerivedPosX();
	iYPos = GetDerivedPosY();
	if( m_pCharBottom )
	{
		m_pCharBottom->SetScale( 0.60f );
		m_pCharBottom->Render( iXPos - (m_pCharBottom->GetWidth() / 2), iYPos - (m_pCharBottom->GetHeight() / 2), UI_RENDER_MULTIPLY, TFO_BILINEAR );
	}

	if( m_pUI3DRender )
	{
		D3DXMATRIX matUI = m_pUI3DRender->MakeUIProjection( iXPos + m_iCharOffSetX, iYPos + m_iCharOffSetY );
		m_pUI3DRender->Render( &matUI, true );
	}

	OnRenderTitle( iXPos + m_iTitleX, iYPos + m_iTitleY );
}

void TotalResultExpCharSlotWnd::OnRenderTitle( int iXPos, int iYPos )
{
	ioComplexStringPrinter kPrinter;
	kPrinter.SetTextStyle( TS_NORMAL );
	kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter.AddTextPiece( FONT_SIZE_11, g_MyInfo.GetClassName( m_iClassType, g_MyInfo.GetCharPowerUpGradeByClassType( m_iClassType ) ) );
	kPrinter.PrintFullText( iXPos + 46, iYPos, TAT_CENTER );
	kPrinter.ClearList();

	kPrinter.SetTextStyle( TS_NORMAL );
	kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter.AddTextPiece( FONT_SIZE_11, STR(1), g_MyInfo.GetClassLevel( m_iClassType, true ) );
	kPrinter.SetTextColor( TCT_DEFAULT_GRAY );
	kPrinter.AddTextPiece( FONT_SIZE_11, STR(2), g_MyInfo.GetExpRate( m_iClassType ) );
	kPrinter.SetTextColor( TCT_DEFAULT_ORANGE );
	kPrinter.AddTextPiece( FONT_SIZE_11, STR(3), m_iClassPoint );
	kPrinter.PrintFullText( iXPos + 46, iYPos + 13, TAT_CENTER );
	kPrinter.ClearList();

	if( !m_pGaugeBar || !m_pGaugeOrange || !m_pGaugeYellow ) return;

	m_pGaugeBar->Render( iXPos + 6, iYPos + 29 );

	int iExpert = g_MyInfo.GetClassExpert( m_iClassType );
	int iPrevPoint = max( 0, iExpert - m_iClassPoint );
	int iNextExpert= g_LevelMgr.GetNextLevelupExp( g_MyInfo.GetClassLevel( m_iClassType, true ) );
	if( iNextExpert == 0 )
		return;

	float fYellowPer = min( (float)iExpert / (float)iNextExpert, m_fCurrentPer );
	m_pGaugeYellow->RenderWidthCut( iXPos + 6, iYPos + 29, 0, (float)m_pGaugeYellow->GetWidth() * fYellowPer );

	float fOrangePer = min( (float)iPrevPoint / (float)iNextExpert, m_fCurrentPer );
	m_pGaugeOrange->RenderWidthCut( iXPos + 6, iYPos + 29, 0, (float)m_pGaugeOrange->GetWidth() * fOrangePer );	


	if( g_MyInfo.IsClassLevelUP( m_iClassType ) )
	{
		// ������ ǥ��
		if( m_pLevelUP )
		{
			m_pLevelUP->SetAlpha( (float)MAX_ALPHA_RATE * sg_fLevelUpAlpha );
			m_pLevelUP->Render( iXPos, iYPos + 108, UI_RENDER_NORMAL );
		}
		// ������ ���� ���
		ioComplexStringPrinter kPrinter;
		kPrinter.SetTextStyle( TS_OUTLINE_FULL_2X );
		kPrinter.SetTextColor( TCT_DEFAULT_YELLOW );
		kPrinter.AddTextPiece( FONT_SIZE_14, STR(4), g_MyInfo.GetClassLevelUPPeso( m_iClassType ) );
		kPrinter.PrintFullText( iXPos + 46, iYPos + 130, TAT_CENTER );
		kPrinter.ClearList();
	}

	if( m_bSoldierExpBonus )
	{
		if( m_pExpBonus )
		{
			m_pExpBonus->SetAlpha( (float)MAX_ALPHA_RATE * sg_fLevelUpAlpha );
			m_pExpBonus->Render( iXPos + 13, iYPos + 26, UI_RENDER_NORMAL );
		}
	}
}
//////////////////////////////////////////////////////////////////////////
TotalResultExpGradeWnd::TotalResultExpGradeWnd()
{
	m_pBackEffect = NULL;
	m_pGaugeBar   = NULL;
	m_pGaugeOrange= NULL;
	m_pGaugeYellow= NULL;
	m_pGradeLevelUP = NULL;
	m_pGradeIcon = NULL;
	
	m_iResultExp = 0;
	m_bLevelUP   = false;
	m_dwGaugeTime= 0;
	m_dwCurrentTime = 0;
	m_fCurrentPer = 0.0f;
	m_iGradeIconPage = 0;

	m_fGradeUpAlpha = 0.0f;
	InitGradeUpAni();
}

TotalResultExpGradeWnd::~TotalResultExpGradeWnd()
{
	SAFEDELETE( m_pBackEffect );
	SAFEDELETE( m_pGaugeBar );
	SAFEDELETE( m_pGaugeOrange );
	SAFEDELETE( m_pGaugeYellow );
	SAFEDELETE( m_pGradeLevelUP );
	SAFEDELETE( m_pGradeIcon );

	if( !m_szGradeGaugeSound.IsEmpty() )
		g_SoundMgr.StopSound( m_szGradeGaugeSound, 0 );
}

void TotalResultExpGradeWnd::InitGradeUpAni()
{
	enum { FADE_IN = 300, FADE_DELAY_IN = 300, FADE_OUT = 300, FADE_DELAY_OUT = 300, };

	m_GradeUpAni.SetCurState( GUA_NONE );

	FSMState *pState = new FSMState( GUA_NONE );
	if( pState )
		pState->Add( GUA_NONE, GUA_START );
	m_GradeUpAni.Add( pState );

	pState = new FSMState( GUA_START );
	if( pState )
		pState->Add( GUA_START, GUA_FADEIN, 0 );
	m_GradeUpAni.Add( pState );

	pState = new FSMState( GUA_FADEIN );
	if( pState )
		pState->Add( GUA_FADEIN,  GUA_DELAY_IN, FADE_IN );
	m_GradeUpAni.Add( pState );

	pState = new FSMState( GUA_DELAY_IN );
	if( pState )
		pState->Add( GUA_DELAY_IN,  GUA_FADEOUT , FADE_DELAY_IN );
	m_GradeUpAni.Add( pState );	

	pState = new FSMState( GUA_FADEOUT );
	if( pState )
		pState->Add( GUA_FADEOUT,  GUA_DELAY_OUT , FADE_OUT );
	m_GradeUpAni.Add( pState );

	pState = new FSMState( GUA_DELAY_OUT );
	if( pState )
		pState->Add( GUA_DELAY_OUT,  GUA_FADEIN , FADE_DELAY_OUT );
	m_GradeUpAni.Add( pState );	
}

void TotalResultExpGradeWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "BackEffect" )
	{
		SAFEDELETE( m_pBackEffect );
		m_pBackEffect = pImage;
	}
	else if( szType == "GaugeBar" )
	{
		SAFEDELETE( m_pGaugeBar );
		m_pGaugeBar = pImage;
	}
	else if( szType == "GaugeOrange" )
	{
		SAFEDELETE( m_pGaugeOrange );
		m_pGaugeOrange = pImage;
	}
	else if( szType == "GaugeYellow" )
	{
		SAFEDELETE( m_pGaugeYellow );
		m_pGaugeYellow = pImage;
	}
	else if( szType == "GradeLevelUP" )
	{
		SAFEDELETE( m_pGradeLevelUP );
		m_pGradeLevelUP = pImage;
	}
	else
	{
		ioWnd::AddRenderImage( szType, pImage );
	}
}

void TotalResultExpGradeWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	m_iGradeIconPage      = xElement.GetIntAttribute_e( "GradeIconPage" );
	m_szGradeIconMainName = xElement.GetStringAttribute_e( "GradeIconMainName" );
	m_szGradeIconSubName  = xElement.GetStringAttribute_e( "GradeIconSubName" );
	m_szGradeGaugeSound    = xElement.GetStringAttribute_e( "GradeGaugeSound" );
}

void TotalResultExpGradeWnd::SetFinalRoundPoint( int iResultExp, bool bLevelUP )
{
	m_iResultExp = iResultExp;
	m_bLevelUP   = bLevelUP;

	char szGradeIconName[MAX_PATH] = "";
	sprintf( szGradeIconName, "%s%d#%s%.2d", m_szGradeIconMainName.c_str(), ( g_MyInfo.GetGradeLevel() / m_iGradeIconPage ) + 1, m_szGradeIconSubName.c_str(), g_MyInfo.GetGradeLevel() ); //Except Extracting Hangeul
	SAFEDELETE( m_pGradeIcon );
	m_pGradeIcon = g_UIImageSetMgr.CreateImageByFullName( szGradeIconName );

	if( m_bLevelUP )
	{
		m_GradeUpAni.SetCurState( GUA_START );
	}
}

void TotalResultExpGradeWnd::StartGauge( DWORD dwGaugeTime )
{
	m_dwGaugeTime  = dwGaugeTime;
	m_dwCurrentTime= FRAMEGETTIME();
	m_fCurrentPer  = 0.0f;

	if( !m_szGradeGaugeSound.IsEmpty() )
		g_SoundMgr.PlaySound( m_szGradeGaugeSound );
}

void TotalResultExpGradeWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;

	if( m_dwGaugeTime > 0 )
	{
		DWORD dwGapTime = FRAMEGETTIME() - m_dwCurrentTime;
		if( dwGapTime >= m_dwGaugeTime )
		{
			m_dwGaugeTime = 0;
			if( GetParent() )
				GetParent()->iwm_command( this, END_GAUGE_ANI, 0 );
		}
		else
		{
			m_fCurrentPer = (float)dwGapTime / (float)m_dwGaugeTime;
		}
	}

	OnProcessGradeUpAni();
}

void TotalResultExpGradeWnd::OnProcessGradeUpAni()
{
	if( m_GradeUpAni.GetCurState() == GUA_NONE )
		return;

	m_GradeUpAni.UpdateTime();
	switch( m_GradeUpAni.GetCurState() )
	{
	case GUA_FADEIN:
		{
			m_fGradeUpAlpha = min( FLOAT1, (float)( m_GradeUpAni.GetCurTime() ) / m_GradeUpAni.GetDelay() );
		}
		break;
	case GUA_DELAY_IN:
		{
			m_fGradeUpAlpha = FLOAT1;
		}
		break;
	case GUA_FADEOUT:
		{
			m_fGradeUpAlpha = max( 0.0f, FLOAT1 - ( (float)( m_GradeUpAni.GetCurTime() ) / m_GradeUpAni.GetDelay() ) );
		}
		break;
	case GUA_DELAY_OUT:
		{
			m_fGradeUpAlpha = 0.0f;
		}
		break;
	}
}

void TotalResultExpGradeWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	if( m_pBackEffect )
		m_pBackEffect->Render( iXPos, iYPos );

	int iGradeLevel = g_MyInfo.GetGradeLevel();
	int iExpert = g_MyInfo.GetGradeExpert();
	int iNextExpert = g_LevelMgr.GetNextGradeupExp( g_MyInfo.GetGradeLevel() );

	char szGradeName[MAX_PATH] = "";
	g_LevelMgr.GetGradeName( iGradeLevel, szGradeName, sizeof( szGradeName ), true, true );
	
	if( COMPARE( iGradeLevel, 50, 54 + 1 ) )
	{
		ioComplexStringPrinter kPrinter;
		kPrinter.SetTextStyle( TS_NORMAL );
		kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
		kPrinter.AddTextPiece( FONT_SIZE_12, szGradeName );
		kPrinter.PrintFullText( iXPos + 46, iYPos, TAT_LEFT );
		kPrinter.ClearList();

		char szConvertText[MAX_PATH] = "";
		Help::ConvertNumToStrComma( iExpert, szConvertText, sizeof( szConvertText ) );

		kPrinter.SetTextStyle( TS_NORMAL );
		kPrinter.SetTextColor( TCT_DEFAULT_GRAY );
		kPrinter.AddTextPiece( FONT_SIZE_12, STR(4), szConvertText );
		kPrinter.PrintFullText( iXPos + 490, iYPos, TAT_RIGHT );
	}
	else
	{
		ioComplexStringPrinter kPrinter;
		kPrinter.SetTextStyle( TS_NORMAL );
		kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
		kPrinter.AddTextPiece( FONT_SIZE_12, szGradeName );
		kPrinter.SetTextColor( TCT_DEFAULT_GRAY );
		kPrinter.AddTextPiece( FONT_SIZE_12, STR(1), g_MyInfo.GetGradeExpRate() );
		kPrinter.PrintFullText( iXPos + 46, iYPos, TAT_LEFT );
		kPrinter.ClearList();

		kPrinter.SetTextStyle( TS_NORMAL );
		kPrinter.SetTextColor( TCT_DEFAULT_GRAY );
		kPrinter.AddTextPiece( FONT_SIZE_12, STR(2), iExpert );
		kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
		kPrinter.AddTextPiece( FONT_SIZE_12, STR(3), iNextExpert );
		kPrinter.PrintFullText( iXPos + 490, iYPos, TAT_RIGHT );
	}

	if( !m_pGaugeBar || !m_pGaugeOrange || !m_pGaugeYellow ) return;

	m_pGaugeBar->Render( iXPos + 42, iYPos + 16 );

	int iPrevPoint = max( 0, iExpert - m_iResultExp );
	if( iNextExpert == 0 )
		return;

	if( g_MyInfo.IsGeneralGrade() )
	{
		m_pGaugeYellow->RenderWidthCut( iXPos + 42, iYPos + 16, 0, m_pGaugeYellow->GetWidth() );
		m_pGaugeOrange->RenderWidthCut( iXPos + 42, iYPos + 16, 0, m_pGaugeOrange->GetWidth() );	
	}
	else
	{
		float fYellowPer = min( (float)iExpert / (float)iNextExpert, m_fCurrentPer );
		m_pGaugeYellow->RenderWidthCut( iXPos + 42, iYPos + 16, 0, (float)m_pGaugeYellow->GetWidth() * fYellowPer );

		float fOrangePer = min( (float)iPrevPoint / (float)iNextExpert, m_fCurrentPer );
		m_pGaugeOrange->RenderWidthCut( iXPos + 42, iYPos + 16, 0, (float)m_pGaugeOrange->GetWidth() * fOrangePer );	
	}

	if( m_pGradeIcon )
	{
		m_pGradeIcon->SetScale( 0.66f );
		m_pGradeIcon->Render( iXPos + 22, iYPos + 22, UI_RENDER_NORMAL, TFO_BILINEAR );
	}

	if( m_bLevelUP && m_pGradeLevelUP )
	{
		m_pGradeLevelUP->SetAlpha( (float)MAX_ALPHA_RATE * m_fGradeUpAlpha );
		m_pGradeLevelUP->Render( iXPos + 183, iYPos + 9 );
	}
}
//////////////////////////////////////////////////////////////////////////
TotalResultExpWnd::TotalResultExpWnd()
{
	m_pBackEffect = NULL;
	
	m_dwCharShowTime = 0;
	m_dwGaugeTime    = 0;
	m_dwCurrentTime  = 0;
	m_dwShowID	     = 0;
}

TotalResultExpWnd::~TotalResultExpWnd()
{
	SAFEDELETE( m_pBackEffect );

	if( !m_szBackSound.IsEmpty() )
		g_SoundMgr.StopSound( m_szBackSound, 0 );
}

void TotalResultExpWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "BackEffect" )
	{
		SAFEDELETE( m_pBackEffect );
		m_pBackEffect = pImage;
	}
	else
	{
		ioWnd::AddRenderImage( szType, pImage );
	}
}

void TotalResultExpWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	m_dwCharShowTime = xElement.GetIntAttribute_e( "CharShowTime" );
	m_dwGaugeTime    = xElement.GetIntAttribute_e( "GaugeTime" );
	m_szBackSound    = xElement.GetStringAttribute_e( "BackSound" );
}

void TotalResultExpWnd::iwm_show()
{
	char szTitle[MAX_PATH] = "";
	SafeSprintf( szTitle, sizeof( szTitle ), STR(1), g_MyInfo.GetPublicID().c_str() );
	SetTitleText( szTitle );

	//
	ShowChildWnd( ID_GRADE );

	m_dwCurrentTime  = FRAMEGETTIME();
	m_dwShowID	     = 0;

	//
//	if( g_MyInfo.IsGradeLevelUP() )
//		g_GUIMgr.ShowWnd( GRADE_UP_WND );

	if( !m_szBackSound.IsEmpty() )
		g_SoundMgr.PlaySound( m_szBackSound );
}

void TotalResultExpWnd::iwm_hide()
{
	if( !m_szBackSound.IsEmpty() )
		g_SoundMgr.StopSound( m_szBackSound, 0 );
}

void TotalResultExpWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_GRADE:
		if( cmd == TotalResultExpGradeWnd::END_GAUGE_ANI )
		{
			if( GetParent() )
				GetParent()->iwm_command( this, TotalResultExpGradeWnd::END_GAUGE_ANI, 0 );
		}
		break;
	}
}

void TotalResultExpWnd::SetFinalRoundPoint( int iGradeExp, bool bLevelUP, IntVec &rvClassTypeList, IntVec &rvClassPointList )
{
	enum{ MAX_CHAR = 5, };
	int iExpClassCount = 0;
	int iSize = min( rvClassTypeList.size(), MAX_CHAR );
	for(int i = 0;i < iSize;i++)
	{
		if( rvClassTypeList[i] <= 0 ) continue;
		
        TotalResultExpCharSlotWnd *pSlotWnd = dynamic_cast<TotalResultExpCharSlotWnd *>(FindChildWnd(ID_CHAR_1 + i));
		if( pSlotWnd )
		{
			int iClassPoint = 0;
			if( COMPARE( i, 0, (int)rvClassPointList.size() ) )
				iClassPoint = rvClassPointList[i];
			if( iClassPoint > 0 )
			{
				pSlotWnd->SetFinalRoundPoint( rvClassTypeList[i], iClassPoint );
				iExpClassCount++;
			}
		}
	}

	// ����ó�� - ����ġ ���� �뺴�� ������ �������� �뺴�� �й� ����� ����
	if( iExpClassCount == 0 )
	{
		// �������� �뺴
		TotalResultExpCharSlotWnd *pSlotWnd = dynamic_cast<TotalResultExpCharSlotWnd *>(FindChildWnd(ID_CHAR_1));
		if( pSlotWnd )
		{
			pSlotWnd->SetFinalRoundPoint( g_MyInfo.GetClassType( g_MyInfo.GetSelectCharArray() ), 0, 1 );
		}
	}



	TotalResultExpGradeWnd *pGradeWnd = dynamic_cast<TotalResultExpGradeWnd*>(FindChildWnd(ID_GRADE));
	if( pGradeWnd )
	{
		pGradeWnd->SetFinalRoundPoint( iGradeExp, bLevelUP );
	}
}

void TotalResultExpWnd::StartGauge()
{
	for(int i = ID_CHAR_1;i < ID_CHAR_5 + 1;i++)
	{
		TotalResultExpCharSlotWnd *pSlotWnd = dynamic_cast<TotalResultExpCharSlotWnd *>(FindChildWnd(i));
		if( pSlotWnd )
		{
			pSlotWnd->StartGauge( m_dwGaugeTime );
		}
	}

	TotalResultExpGradeWnd *pGradeWnd = dynamic_cast<TotalResultExpGradeWnd*>(FindChildWnd(ID_GRADE));
	if( pGradeWnd )
	{
		pGradeWnd->StartGauge( m_dwGaugeTime );
	}
}

void TotalResultExpWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;

	if( m_dwCurrentTime == 0 )
		return;

	if( FRAMEGETTIME() - m_dwCurrentTime > m_dwCharShowTime )
	{
		m_dwCurrentTime = FRAMEGETTIME();
		m_dwShowID++;
		if( COMPARE( m_dwShowID, ID_CHAR_1, ID_CHAR_5 + 1 ) )
			ShowChildWnd( m_dwShowID );
		else
		{
			m_dwCurrentTime = 0;
			StartGauge();
		}
	}
}

void TotalResultExpWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();
	if( m_pBackEffect )
	{
		m_pBackEffect->SetScale( 2.0f );
		m_pBackEffect->Render( iXPos + 269, iYPos + 182, UI_RENDER_ADD );
	}
}
//////////////////////////////////////////////////////////////////////////
TotalResultRawardWnd::TotalResultRawardWnd()
{
	m_iResultExp = m_iResultPeso = 0;
	m_iGradeIconPage  = 0;
	m_iCurrentBonus   = 0;
	m_dwCurrentTime   = 0;
	m_dwBonusTickTime = 0;

	m_dwScaleTime = m_dwPesoScaleTime = m_dwExpScaleTime = 0;
	m_fPesoScale = m_fExpScale = FLOAT1;

	m_iPlayPoint = m_iWinningPoint = m_iConsecutivePoint = m_iAwardPoint = 0;

	m_pBlueStateFrm			= NULL;
	m_pOrangeStateFrm		= NULL;
	m_pBonusFrm				= NULL;
	m_pBonusNoneFrm			= NULL;
	m_pShufflePointFrm		= NULL;

	m_pBonusOffX			= NULL;
	m_pIconBack				= NULL;
	m_pPesoIcon				= NULL;
	m_pGradeIcon			= NULL;

	m_pBonusIconBack		= NULL;
	m_pMannerIcon			= NULL;
	m_pSoldierIcon			= NULL;
	m_pGuildIcon			= NULL;
	m_pFriendIcon			= NULL;
	m_pPcroomIcon			= NULL;
	m_pModeIcon				= NULL;
	m_pAwardIcon			= NULL;
	m_pCampIcon				= NULL;
	m_pEtcItemIcon			= NULL;
	m_pBonusIcon			= NULL;
	m_pPesoBonusIcon		= NULL;
	m_pEtcItemExpIcon		= NULL;
	m_pEtcItemPesoIcon		= NULL;
	m_pEtcItemExpPesoIcon	= NULL;
	m_pVictoriesIcon		= NULL;
	m_pContributeIcon		= NULL;
	m_pModeConsecutively	= NULL;
	m_pRewardPesoNum		= NULL;
	m_pRewardExpNum			= NULL;
	m_pShuffleStarBg		= NULL;
	m_pShuffleStar			= NULL;
	m_pDarkLine				= NULL;
	m_pLightLine			= NULL;
}

TotalResultRawardWnd::~TotalResultRawardWnd()
{
	SAFEDELETE( m_pBlueStateFrm );
	SAFEDELETE( m_pOrangeStateFrm );
	SAFEDELETE( m_pBonusFrm );
	SAFEDELETE( m_pBonusNoneFrm );
	SAFEDELETE( m_pShufflePointFrm );
	SAFEDELETE( m_pIconBack );
	SAFEDELETE( m_pPesoIcon );
	SAFEDELETE( m_pGradeIcon );
	SAFEDELETE( m_pBonusOffX );

	SAFEDELETE( m_pBonusIconBack );
	SAFEDELETE( m_pMannerIcon );
	SAFEDELETE( m_pSoldierIcon );
	SAFEDELETE( m_pGuildIcon );
	SAFEDELETE( m_pFriendIcon );
	SAFEDELETE( m_pPcroomIcon );
	SAFEDELETE( m_pModeIcon );
	SAFEDELETE( m_pAwardIcon );
	SAFEDELETE( m_pCampIcon );
	SAFEDELETE( m_pEtcItemIcon );
	SAFEDELETE( m_pBonusIcon );
	SAFEDELETE( m_pPesoBonusIcon );
	SAFEDELETE( m_pEtcItemExpIcon );
	SAFEDELETE( m_pEtcItemPesoIcon );
	SAFEDELETE( m_pEtcItemExpPesoIcon );
	SAFEDELETE( m_pVictoriesIcon );
	SAFEDELETE( m_pContributeIcon );
	SAFEDELETE( m_pModeConsecutively );
	SAFEDELETE( m_pRewardPesoNum );
	SAFEDELETE( m_pRewardExpNum );
	SAFEDELETE( m_pShuffleStarBg );
	SAFEDELETE( m_pShuffleStar );
	SAFEDELETE( m_pDarkLine );
	SAFEDELETE( m_pLightLine );

	if( !m_szPesoRollingSound.IsEmpty() )
		g_SoundMgr.StopSound( m_szPesoRollingSound, 0 );

	if( !m_szExpRollingSound.IsEmpty() )
		g_SoundMgr.StopSound( m_szExpRollingSound, 0 );

	if( !m_szPesoStopSound.IsEmpty() )
		g_SoundMgr.StopSound( m_szPesoStopSound, 0 );

	if( !m_szExpStopSound.IsEmpty() )
		g_SoundMgr.StopSound( m_szExpStopSound, 0 );

	if( !m_szBonusCountSound.IsEmpty() )
		g_SoundMgr.StopSound( m_szBonusCountSound, 0 );

	for(int i = 0;i < (int)m_BonusData.size();i++)
	{
		SAFEDELETE( m_BonusData[i].m_pClassIcon );
	}
	m_BonusData.clear();
}

void TotalResultRawardWnd::AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame )
{
	if( szType == "StateBlueFrm" )
	{
		SAFEDELETE( m_pBlueStateFrm );
		m_pBlueStateFrm = pFrame;
	}
	else if( szType == "StateOrangeFrm" )
	{
		SAFEDELETE( m_pOrangeStateFrm );
		m_pOrangeStateFrm = pFrame;
	}
	else if( szType == "BonusFrm" )
	{
		SAFEDELETE( m_pBonusFrm );
		m_pBonusFrm = pFrame;
	}
	else if( szType == "BonusNoneFrm" )
	{
		SAFEDELETE( m_pBonusNoneFrm );
		m_pBonusNoneFrm = pFrame;
	}
	else if( szType == "ShufflePointFrm" )
	{
		SAFEDELETE( m_pShufflePointFrm );
		m_pShufflePointFrm = pFrame;
	}
	else
	{
		ioWnd::AddRenderFrame( szType, pFrame );
	}
}

void TotalResultRawardWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "IconBack" )
	{
		SAFEDELETE( m_pIconBack );
		m_pIconBack = pImage;
	}
	else if( szType == "PesoIcon" )
	{
		SAFEDELETE( m_pPesoIcon );
		m_pPesoIcon = pImage;
	}
	else if( szType == "BonusOffX" )
	{
		SAFEDELETE( m_pBonusOffX );
		m_pBonusOffX = pImage;
	}
	else if( szType == "BonusIconBack" )
	{
		SAFEDELETE( m_pBonusIconBack );
		m_pBonusIconBack = pImage;
	}
	else if( szType == "MannerIcon" )
	{
		SAFEDELETE( m_pMannerIcon );
		m_pMannerIcon = pImage;
	}
	else if( szType == "SoldierIcon" )
	{
		SAFEDELETE( m_pSoldierIcon );
		m_pSoldierIcon = pImage;
	}
	else if( szType == "GuildIcon" )
	{
		SAFEDELETE( m_pGuildIcon );
		m_pGuildIcon = pImage;
	}
	else if( szType == "FriendIcon" )
	{
		SAFEDELETE( m_pFriendIcon );
		m_pFriendIcon = pImage;
	}
	else if( szType == "PcroomIcon" )
	{
		SAFEDELETE( m_pPcroomIcon );
		m_pPcroomIcon = pImage;
	}
	else if( szType == "ModeIcon" )
	{
		SAFEDELETE( m_pModeIcon );
		m_pModeIcon = pImage;
	}
	else if( szType == "AwardIcon" )
	{
		SAFEDELETE( m_pAwardIcon );
		m_pAwardIcon = pImage;
	}
	else if( szType == "CampIcon" )
	{
		SAFEDELETE( m_pCampIcon );
		m_pCampIcon = pImage;
	}
	else if( szType == "EtcItemIcon" )
	{
		SAFEDELETE( m_pEtcItemIcon );
		m_pEtcItemIcon = pImage;
	}
	else if( szType == "BonusIcon" )
	{
		SAFEDELETE( m_pBonusIcon );
		m_pBonusIcon = pImage;
	}
	else if( szType == "PesoBonusIcon" )
	{
		SAFEDELETE( m_pPesoBonusIcon );
		m_pPesoBonusIcon = pImage;
	}
	else if( szType == "EtcItemExpIcon" )
	{
		SAFEDELETE( m_pEtcItemExpIcon );
		m_pEtcItemExpIcon = pImage;
	}
	else if( szType == "EtcItemPesoIcon" )
	{
		SAFEDELETE( m_pEtcItemPesoIcon );
		m_pEtcItemPesoIcon = pImage;
	}
	else if( szType == "EtcItemExpPesoIcon" )
	{
		SAFEDELETE( m_pEtcItemExpPesoIcon );
		m_pEtcItemExpPesoIcon = pImage;
	}
	else if( szType == "VictoriesIcon" )
	{
		SAFEDELETE( m_pVictoriesIcon );
		m_pVictoriesIcon = pImage;
	}
	else if( szType == "ContributeIcon" )
	{
		SAFEDELETE( m_pContributeIcon );
		m_pContributeIcon = pImage;
	}
	else if( szType == "ModeConsecutively" )
	{
		SAFEDELETE( m_pModeConsecutively );
		m_pModeConsecutively = pImage;
	}
	else if( szType == "RewardPesoNum" )
	{
		SAFEDELETE( m_pRewardPesoNum );
		m_pRewardPesoNum = pImage;
	}
	else if( szType == "RewardExpNum" )
	{
		SAFEDELETE( m_pRewardExpNum );
		m_pRewardExpNum = pImage;
	}
	else if( szType == "ShuffleStarBg" )
	{
		SAFEDELETE( m_pShuffleStarBg );
		m_pShuffleStarBg = pImage;

		if( m_pShuffleStarBg )
			m_pShuffleStarBg->SetScale( 0.58f, 0.58f );
	}
	else if( szType == "ShuffleStar" )
	{
		SAFEDELETE( m_pShuffleStar );
		m_pShuffleStar = pImage;

		if( m_pShuffleStar )
			m_pShuffleStar->SetScale( 0.58f, 0.58f );
	}
	else if( szType == "DarkLine" )
	{
		SAFEDELETE( m_pDarkLine );
		m_pDarkLine = pImage;
	}
	else if( szType == "LightLine" )
	{
		SAFEDELETE( m_pLightLine );
		m_pLightLine = pImage;
	}
	else
	{
		ioWnd::AddRenderImage( szType, pImage );
	}
}

void TotalResultRawardWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	m_iGradeIconPage      = xElement.GetIntAttribute_e( "GradeIconPage" );
	m_szGradeIconMainName = xElement.GetStringAttribute_e( "GradeIconMainName" );
	m_szGradeIconSubName  = xElement.GetStringAttribute_e( "GradeIconSubName" );

	m_dwBonusTickTime = xElement.GetIntAttribute_e( "BonusTickTime" );
	m_dwScaleTime = xElement.GetIntAttribute_e( "ScaleTime" );

	m_szPesoRollingSound = xElement.GetStringAttribute_e( "PesoRollingSound" );
	m_szExpRollingSound = xElement.GetStringAttribute_e( "ExpRollingSound" );
	m_szPesoStopSound = xElement.GetStringAttribute_e( "PesoStopSound" );
	m_szExpStopSound = xElement.GetStringAttribute_e( "ExpStopSound" );
	m_szBonusCountSound = xElement.GetStringAttribute_e( "BonusCountSound" );
}

void TotalResultRawardWnd::StartBonus()
{
	m_iCurrentBonus = 0;
	m_dwCurrentTime = FRAMEGETTIME();
}

int TotalResultRawardWnd::GetNumberCountRand( int iNumber )
{
	if( iNumber < 10 )
		iNumber = rand()%9+1;
	else if( iNumber < FLOAT100 )
		iNumber = rand()%90+10;
	else if( iNumber < 1000 )
		iNumber = rand()%900+100;
	else if( iNumber < 10000 )
		iNumber = rand()%9000+1000;
	else
		iNumber = rand()%90000+10000;
	return iNumber;
}

void TotalResultRawardWnd::iwm_show()
{
	char szGradeIconName[MAX_PATH] = "";
	sprintf( szGradeIconName, "%s%d#%s%.2d", m_szGradeIconMainName.c_str(), ( g_MyInfo.GetGradeLevel() / m_iGradeIconPage ) + 1, m_szGradeIconSubName.c_str(), g_MyInfo.GetGradeLevel() ); //Except Extracting Hangeul
	SAFEDELETE( m_pGradeIcon );
	m_pGradeIcon = g_UIImageSetMgr.CreateImageByFullName( szGradeIconName );

	if( !m_szPesoRollingSound.IsEmpty() )
		g_SoundMgr.PlaySound( m_szPesoRollingSound, DSBVOLUME_MAX, PSM_LOOP );

	if( !m_szExpRollingSound.IsEmpty() )
		g_SoundMgr.PlaySound( m_szExpRollingSound, DSBVOLUME_MAX, PSM_LOOP );
}

void TotalResultRawardWnd::iwm_hide()
{

}

void TotalResultRawardWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;

	// ���ʽ� ƽ
	if( m_dwCurrentTime > 0 )
	{
		DWORD dwGapTime = FRAMEGETTIME() - m_dwCurrentTime;
		if( dwGapTime > m_dwBonusTickTime )
		{
			m_iCurrentBonus++;
			m_dwCurrentTime = FRAMEGETTIME();

			if( m_iCurrentBonus <= (int)m_BonusData.size() )       //���ʽ� �Ѹ�
			{
				if( !m_szBonusCountSound.IsEmpty() )
					g_SoundMgr.PlaySound( m_szBonusCountSound );
			}
			else if( m_iCurrentBonus == (int)m_BonusData.size() + 1 ) // ��� ����
			{
				if( !m_szPesoRollingSound.IsEmpty() )
					g_SoundMgr.StopSound( m_szPesoRollingSound, 0 );

				if( !m_szPesoStopSound.IsEmpty() )
					g_SoundMgr.PlaySound( m_szPesoStopSound );

				m_fPesoScale = 2.0f;
				m_dwPesoScaleTime = FRAMEGETTIME();
			}
			else if( m_iCurrentBonus == (int)m_BonusData.size() + 2 ) // ����ġ ����
			{
				if( !m_szExpRollingSound.IsEmpty() )
					g_SoundMgr.StopSound( m_szExpRollingSound, 0 );

				if( !m_szExpStopSound.IsEmpty() )
					g_SoundMgr.PlaySound( m_szExpStopSound );
				
				m_fExpScale = 2.0f;
				m_dwExpScaleTime = FRAMEGETTIME();
			}
		}
	}

	// ��� Ȯ�� ���
	if( m_dwPesoScaleTime > 0 && m_dwScaleTime > 0 )
	{
		DWORD dwGapTime = FRAMEGETTIME() - m_dwPesoScaleTime;
		if( dwGapTime < m_dwScaleTime )
		{
			float fRate = (float)dwGapTime / (float)m_dwScaleTime;
			m_fPesoScale = max( FLOAT1, 2.0f - fRate );
		}
		else
		{
			m_fPesoScale = FLOAT1;
			m_dwPesoScaleTime = 0;
		}
	}

	// ����ġ Ȯ�� ���
	if( m_dwExpScaleTime > 0 && m_dwScaleTime > 0 )
	{
		DWORD dwGapTime = FRAMEGETTIME() - m_dwExpScaleTime;
		if( dwGapTime < m_dwScaleTime )
		{
			float fRate = (float)dwGapTime / (float)m_dwScaleTime;
			m_fExpScale = max( FLOAT1, 2.0f - fRate );
		}
		else
		{
			m_fExpScale = FLOAT1;
			m_dwExpScaleTime = 0;
		}
	}
}

void TotalResultRawardWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();
	OnRenderPeso( iXPos + 22, iYPos + 59 );
	OnRenderExp( iXPos + 116, iYPos + 59 );

	if( g_ShuffleRoomMgr.IsShuffleRoom() )
	{
		OnRenderBonusTtitle( iXPos + 22, iYPos + 243 );
		OnRenderBonus( iXPos + 22, iYPos + 326 );
		OnRenderShufflePoint( iXPos + 22, iYPos + 257 );
	}
	else
	{
		OnRenderBonusTtitle( iXPos + 22, iYPos + 243 );
		OnRenderBonus( iXPos + 22, iYPos + 243 );
	}
}

void TotalResultRawardWnd::OnRenderPeso( int iXPos, int iYPos )
{
	if( m_pIconBack )
	{
		m_pIconBack->SetScale( FLOAT1 );
		m_pIconBack->Render( iXPos + 46, iYPos + 46, UI_RENDER_MULTIPLY );
	}

	if( m_pPesoIcon )
	{
		m_pPesoIcon->SetScale( FLOAT1 );
		m_pPesoIcon->Render( iXPos + 46, iYPos + 46 );
	}

	if( m_pBlueStateFrm )
	{
		m_pBlueStateFrm->SetSize( 40, 16 );
		m_pBlueStateFrm->Render( iXPos + 1, iYPos );
	}

	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetTextColor( TCT_DEFAULT_WHITE );
	g_FontMgr.PrintText( iXPos + 21, iYPos + 2, FONT_SIZE_11, STR(1) );

	g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
	g_FontMgr.PrintText( iXPos + 46, iYPos + 92, FONT_SIZE_12, STR(2) );

	int iResultPeso = m_iResultPeso;
	if( m_iCurrentBonus < (int)m_BonusData.size() + 1 )
		iResultPeso = GetNumberCountRand( iResultPeso );
	if( m_pRewardPesoNum )
	{
		m_pRewardPesoNum->SetHorzFormatting( ioUIRenderImage::HF_CENTER );
		m_pRewardPesoNum->SetVertFormatting( ioUIRenderImage::VF_CENTER );
		m_pRewardPesoNum->RenderNum( iXPos + 46, iYPos + 109 + m_pRewardPesoNum->GetHeight() / 2, iResultPeso, 0, m_fPesoScale, UI_RENDER_NORMAL, TFO_BILINEAR );
	}
	char szConvertNum[MAX_PATH] = "";
/*	Help::ConvertNumToStrComma( iResultPeso, szConvertNum, sizeof( szConvertNum ) );
	g_FontMgr.SetTextColor( TCT_DEFAULT_BLUE );
	g_FontMgr.PrintText( iXPos + 46, iYPos + 109, FONT_SIZE_32, szConvertNum );
*/
	Help::ConvertNumToStrComma( (int)g_MyInfo.GetMoney(), szConvertNum, sizeof( szConvertNum ) );
	g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
	g_FontMgr.PrintText( iXPos + 46, iYPos + 145, FONT_SIZE_12, STR(3), szConvertNum );
}

void TotalResultRawardWnd::OnRenderExp( int iXPos, int iYPos )
{
	if( m_pIconBack )
	{
		m_pIconBack->SetScale( FLOAT1 );
		m_pIconBack->Render( iXPos + 46, iYPos + 46, UI_RENDER_MULTIPLY );
	}

	if( m_pGradeIcon )
	{
		m_pGradeIcon->SetScale( FLOAT1 );
		m_pGradeIcon->Render( iXPos + 46, iYPos + 46 );
	}

	if( m_pOrangeStateFrm )
	{
		m_pOrangeStateFrm->SetSize( 40, 16 );
		m_pOrangeStateFrm->Render( iXPos + 1, iYPos );
	}

	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetTextColor( TCT_DEFAULT_WHITE );
	g_FontMgr.PrintText( iXPos + 21, iYPos + 2, FONT_SIZE_11, STR(1) );

	g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
	g_FontMgr.PrintText( iXPos + 46, iYPos + 92, FONT_SIZE_12, STR(2) );

	int iResultExp = m_iResultExp;
	if( m_iCurrentBonus < (int)m_BonusData.size() + 2 )
		iResultExp = GetNumberCountRand( iResultExp );
	if( m_pRewardExpNum )
	{
		m_pRewardExpNum->SetHorzFormatting( ioUIRenderImage::HF_CENTER );
		m_pRewardExpNum->SetVertFormatting( ioUIRenderImage::VF_CENTER );
		m_pRewardExpNum->RenderNum( iXPos + 46, iYPos + 109 + m_pRewardExpNum->GetHeight() / 2, iResultExp, 0, m_fExpScale, UI_RENDER_NORMAL, TFO_BILINEAR );
	}

/*	char szConvertNum[MAX_PATH] = "";
	Help::ConvertNumToStrComma( iResultExp, szConvertNum, sizeof( szConvertNum ) );
	g_FontMgr.SetTextColor( TCT_DEFAULT_ORANGE );
	g_FontMgr.PrintText( iXPos + 46, iYPos + 109, FONT_SIZE_32, szConvertNum );
*/
	char szGradeName[MAX_PATH] = "";
	g_LevelMgr.GetGradeName( g_MyInfo.GetGradeLevel(), szGradeName, sizeof( szGradeName ), true, true );
	g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
	g_FontMgr.PrintText( iXPos + 46, iYPos + 145, FONT_SIZE_12, STR(3), szGradeName, g_MyInfo.GetGradeExpRate() );
}

void TotalResultRawardWnd::OnRenderBonusTtitle( int iXPos, int iYPos )
{
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
	g_FontMgr.PrintText( iXPos + 93, iYPos - 4, FONT_SIZE_11, STR(1) );
}

void TotalResultRawardWnd::OnRenderBonus( int iXPos, int iYPos )
{
	if( m_pBonusNoneFrm && m_pBonusOffX )
	{
		int iBonusX	= iXPos;
		int iBonusY	= iYPos + 14;
		int iMax	= 0;
		if( g_ShuffleRoomMgr.IsShuffleRoom() )
			iMax = MAX_BONUS_BY_SHUFFLE;
		else
			iMax = MAX_BONUS;

		for( int i = 0; i < iMax; i++ )
		{
			if( i >= min( m_iCurrentBonus, (int)m_BonusData.size() ) )
			{
				m_pBonusNoneFrm->SetSize( 61, 81 );
				m_pBonusNoneFrm->Render( iBonusX, iBonusY );

				m_pBonusOffX->SetScale( 0.65f );
				m_pBonusOffX->Render( iBonusX + 30, iBonusY + 38, UI_RENDER_MULTIPLY );
			}

			if( i % 3 == 2 )
			{
				iBonusX = iXPos;
				iBonusY += 83;
			}
			else
			{
				iBonusX += 63;
			}
		}
	}

	if( m_pBonusFrm )
	{
		int iBonusX = iXPos;
		int iBonusY = iYPos + 14;
		int iMaxBonus = min( m_iCurrentBonus, (int)m_BonusData.size() );
		for(int i = 0;i < iMaxBonus;i++)
		{
			m_pBonusFrm->SetSize( 61, 81 );
			m_pBonusFrm->Render( iBonusX, iBonusY );
			
			if( m_BonusData[i].m_dwStartTime == 0 )
				m_BonusData[i].m_dwStartTime = FRAMEGETTIME();

			OnRenderBonusIcon( m_BonusData[i], iBonusX, iBonusY );

			if( i % 3 == 2 )
			{
				iBonusX = iXPos;
				iBonusY += 83;
			}
			else
			{
				iBonusX += 63;
			}
		}
	}
}

void TotalResultRawardWnd::OnRenderBonusIcon( const BonusData &rkData, int iXPos, int iYPos )
{
	char szBonusPeso[MAX_PATH] = "";
	char szBonusExp[MAX_PATH]  = "";
	SafeSprintf( szBonusPeso, sizeof( szBonusPeso ), STR(1), rkData.m_iBonusPeso );
	SafeSprintf( szBonusExp, sizeof( szBonusExp ), STR(2), rkData.m_iBonusExp );

	ioUIRenderImage *pViewIcon = NULL;
	switch( rkData.m_dwBonusID )
	{
	case BONUS_SOLDIER:
		pViewIcon = m_pSoldierIcon;
		break;
	case BONUS_GUILD:
		pViewIcon = m_pGuildIcon;
		break;
	case BONUS_PCROOM:
		pViewIcon = m_pPcroomIcon;
		break;
	case BONUS_MODE:
		pViewIcon = m_pModeIcon;
		break;
	case BONUS_MANNER:
		pViewIcon = m_pMannerIcon;
		SafeSprintf( szBonusPeso, sizeof( szBonusPeso ), STR(3), rkData.m_iBonusPeso );
		SafeSprintf( szBonusExp, sizeof( szBonusExp ), STR(4), rkData.m_iBonusExp );
		break;
	case BONUS_FRIEND:
		pViewIcon = m_pFriendIcon;
		break;
	case BONUS_EVENT:
		pViewIcon = m_pBonusIcon;
		break;
	case BONUS_PESO_EVENT:
		pViewIcon = m_pPesoBonusIcon;
		break;
	case BONUS_ETC_ITEM:
		pViewIcon = m_pEtcItemIcon;
		break;
	case BONUS_CAMP_BATTLE:
		pViewIcon = m_pCampIcon;
		break;
	case BONUS_AWARD:
		pViewIcon = m_pAwardIcon;
		break;
	case BONUS_ETC_ITEM_EXP_PESO: 
		if( rkData.m_iBonusPeso == 0 )
		{
			pViewIcon = m_pEtcItemExpIcon;
		}
		else if( rkData.m_iBonusExp == 0 )
		{
			pViewIcon = m_pEtcItemPesoIcon;
		}
		else
		{
			pViewIcon = m_pEtcItemExpPesoIcon;
		}
		break;
	case BONUS_VICTORIES_PESO:
		pViewIcon = m_pVictoriesIcon;
		break;
	case BONUS_EVENT_PESO_EXP:
		pViewIcon = m_pPesoBonusIcon;
		break;
	case BONUS_LEVELUP:
		{
			pViewIcon = rkData.m_pClassIcon;
			sprintf( szBonusPeso, "+%d", rkData.m_iBonusPeso );
		}
		break;
	case BONUS_CONTRIBUTE:
		{
			pViewIcon = m_pContributeIcon;
			SafeSprintf( szBonusPeso, sizeof( szBonusPeso ), STR(5), rkData.m_iBonusPeso );
			SafeSprintf( szBonusExp, sizeof( szBonusExp ), STR(6), rkData.m_iBonusExp );
		}
		break;
	case BONUS_HERO_TITLE_PESO:
		{
			pViewIcon = g_LevelMgr.GetHeroTitleIcon( g_MyInfo.GetHeroTitle() );
		}
		break;
	case BONUS_MODE_CONSECUTIVELY:
		{
			pViewIcon = m_pModeConsecutively;
		}
		break;
	default:
		return;
	}

	if( m_pBonusIconBack )
	{
		m_pBonusIconBack->SetScale( 0.65f );
		m_pBonusIconBack->Render( iXPos + 30, iYPos + 38, UI_RENDER_MULTIPLY, TFO_BILINEAR );
	}

	// ���ʽ� �̸�
	g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
	g_FontMgr.PrintText( iXPos + 30, iYPos + 63, FONT_SIZE_11, rkData.m_szTitle.c_str() );

	// ������ �ִϸ��̼�
	const DWORD dwTickTime = max( 1, m_dwScaleTime );
	DWORD dwGapTime = FRAMEGETTIME() - rkData.m_dwStartTime;
	if( pViewIcon )
	{		
		float fRate =  0.35f * ( FLOAT1 - min( (float)dwGapTime / dwTickTime, FLOAT1 ) );
		pViewIcon->SetScale( 0.65f + fRate );
		pViewIcon->Render( iXPos + 30, iYPos + 38, UI_RENDER_NORMAL, TFO_BILINEAR );
	}

	// ���ʽ�
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetTextColor( TCT_DEFAULT_WHITE );

	if( dwGapTime > dwTickTime * 3 )
	{
		if( rkData.m_iBonusPeso > 0 && rkData.m_iBonusExp > 0 )
		{
			if( m_pOrangeStateFrm )
			{
				m_pOrangeStateFrm->SetSize( 51, 16 );
				m_pOrangeStateFrm->Render( iXPos + 5, iYPos + 19 );
			}
			g_FontMgr.PrintText( iXPos + 30, iYPos + 21, FONT_SIZE_11, szBonusExp );
		}
	}

	if( dwGapTime > dwTickTime * 2 )
	{
		if( rkData.m_iBonusPeso > 0 )
		{
			if( m_pBlueStateFrm )
			{
				m_pBlueStateFrm->SetSize( 51, 16 );
				m_pBlueStateFrm->Render( iXPos + 5, iYPos + 5 );
			}
			g_FontMgr.PrintText( iXPos + 30, iYPos + 7, FONT_SIZE_11, szBonusPeso );
		}
		else
		{
			if( m_pOrangeStateFrm )
			{
				m_pOrangeStateFrm->SetSize( 51, 16 );
				m_pOrangeStateFrm->Render( iXPos + 5, iYPos + 5 );
			}
			g_FontMgr.PrintText( iXPos + 30, iYPos + 7, FONT_SIZE_11, szBonusExp );
		}
	}	
}

void TotalResultRawardWnd::SetFinalRoundPoint( int iResultContribute, int iResultExp, int iResultPeso, IntVec &rvClassTypeList, float fBonusArray[ioPlayMode::BA_MAX] )
{
	m_iResultExp = iResultExp;
	m_iResultPeso = iResultPeso;

	// ���ʽ� ǥ�� �켱 ����
	/*
	�� ��ġ �켱���� - 0
	�� �뺴 ������ : +���ʽ����
	�� ��ġ �켱���� - 1
	�� �⿩�� : x�⿩��%
	�� ��ġ �켱���� - 2
	�� �����Ƽ : x10%, x50% �� �����¿� ���� ����
	> ��ü ��������� (x)���·� ����
	�� ��ġ �켱���� - 3
	�� �뺴���ʽ� : +0~100% (�뺴���������� ���� ���� ����)
	�� ��ġ �켱���� - 4
	�� ����Ÿ��Ʋ ��� ���ʽ� : +0~100% (Ÿ��Ʋ�� ���� ���� ����)
	�� ��ġ �켱���� - 5
	�� ��庸�ʽ� : +15%~30% (��巩ũ�� ���� ����, ������������ ����)
	�� ��ġ �켱���� - 6
	�� ģ�����ʽ� : +5% (ģ����� �� 30�� ���� �ĺ��� ����)
	�� ��ġ �켱���� - 7
	�� �̼Ǻ��ʽ� : +10% �� �̼Ǹ��(�Ŀ�/����) �÷��̽� ����
	�� PC�溸�ʽ� : +20% �� PC�� ���°� Ȱ��ȭ�� ���� ����
	�� �̺�Ʈ���ʽ� : +?% �� �̺�Ʈ ���ý� ������ ���ʽ� ����
	�� �켱���� : �̺�Ʈ���ʽ� > PC�溸�ʽ� > �̼Ǻ��ʽ�
	�� ��ġ �켱���� - 8
	�� ���Ӱ��� : +?% �� �����ؼ� ���� ���
	�� ��ġ �켱���� - 9
	�� ���󺸳ʽ� : +?% �� ����ȹ��� ������ ���ʽ��� ����, (-)�� ����
	�� ��ġ �켱���� - 10
	�� Ư�������� ���ʽ� : +?% �� ����ġ/��� (+)Ư�������� ���� ������ ���ʽ� ����
	�� ��ġ �켱���� - 11
	�� ���ο��� ���ʽ� : +10% x ����Ƚ��(2ȸ �̻��϶� ��������Ƚ��-1 ��)
	*/

	int iMax = 0;
	if( g_ShuffleRoomMgr.IsShuffleRoom() )
		iMax = MAX_BONUS_BY_SHUFFLE;
	else
		iMax = MAX_BONUS;

	int iCurBonusIcon = 0;
	// �뺴 ������
	for(int i = 0;i < (int)rvClassTypeList.size();i++)
	{
		if( g_MyInfo.IsClassLevelUP( rvClassTypeList[i] ) )
		{
			if( iCurBonusIcon >= iMax ) return;

			BonusData kData;
			kData.m_dwBonusID	= BONUS_LEVELUP;
			kData.m_iBonusPeso  = g_MyInfo.GetClassLevelUPPeso( rvClassTypeList[i] );
			kData.m_iBonusExp   = 0;
			kData.m_pClassIcon  = g_MyInfo.GetMySoldierIcon( rvClassTypeList[i] );
			kData.m_dwStartTime = 0;
			kData.m_szTitle     = STR(1);
			m_BonusData.push_back( kData );
			iCurBonusIcon++;
		}
	}

	// �⿩��
	if( iResultContribute > 0 )
	{
		// ������ ����
		if( iCurBonusIcon >= iMax ) return;

		BonusData kData;
		kData.m_dwBonusID	= BONUS_CONTRIBUTE;
		kData.m_iBonusPeso  = iResultContribute;
		kData.m_iBonusExp   = iResultContribute;
		kData.m_pClassIcon  = NULL;
		kData.m_dwStartTime = 0;
		kData.m_szTitle     = STR(2);
		m_BonusData.push_back( kData );
		iCurBonusIcon++;
	}

	// �� ���Ƽ 
	if( g_MyInfo.GetBlockPointPer() != FLOAT1 )
	{
		// ������ ����
		if( iCurBonusIcon >= iMax ) return;

		BonusData kData;
		kData.m_dwBonusID	= BONUS_MANNER;
		kData.m_iBonusPeso  = (float)( g_MyInfo.GetBlockPointPer() * FLOAT100 ) + FLOAT0001;
		kData.m_iBonusExp   = (float)( g_MyInfo.GetBlockPointPer() * FLOAT100 ) + FLOAT0001;
		kData.m_pClassIcon  = NULL;
		kData.m_dwStartTime = 0;
		kData.m_szTitle     = STR(3);
		m_BonusData.push_back( kData );
		iCurBonusIcon++;
	}

	// �뺴 ���� ���ʽ�
	if( fBonusArray[ioPlayMode::BA_SOLDIER_CNT] != 0.0f )
	{
		// ������ ����
		if( iCurBonusIcon >= iMax ) return;

		BonusData kData;
		kData.m_dwBonusID	= BONUS_SOLDIER;
		kData.m_iBonusPeso  = 0;
		kData.m_iBonusExp   = (float)( fBonusArray[ioPlayMode::BA_SOLDIER_CNT] * FLOAT100 ) + FLOAT0001;
		kData.m_pClassIcon  = NULL;
		kData.m_dwStartTime = 0;
		kData.m_szTitle     = STR(4);
		m_BonusData.push_back( kData );
		iCurBonusIcon++;
	}

	// ������ Ÿ��Ʋ ���ʽ�
	if( fBonusArray[ioPlayMode::BA_HERO_TITLE_PESO] != 0.0f )
	{
		// ������ ����
		if( iCurBonusIcon >= iMax ) return;

		BonusData kData;
		kData.m_dwBonusID	= BONUS_HERO_TITLE_PESO;
		kData.m_iBonusPeso  = (float)( fBonusArray[ioPlayMode::BA_HERO_TITLE_PESO] * FLOAT100 ) + FLOAT0001;
		kData.m_iBonusExp   = 0;
		kData.m_pClassIcon  = NULL;
		kData.m_dwStartTime = 0;
		kData.m_szTitle     = g_LevelMgr.GetHeroTitleString( g_MyInfo.GetHeroTitle() );
		m_BonusData.push_back( kData );
		iCurBonusIcon++;
	}

	//���� ������ ���ʽ�
	if( fBonusArray[ioPlayMode::BA_ETC_ITEM] != 0.0f )
	{
		// ������ ����
		if( iCurBonusIcon >= iMax ) return;

		BonusData kData;
		kData.m_dwBonusID	= BONUS_ETC_ITEM;
		kData.m_iBonusPeso  = (float)( fBonusArray[ioPlayMode::BA_ETC_ITEM] * FLOAT100 ) + FLOAT0001;
		kData.m_iBonusExp   = (float)( fBonusArray[ioPlayMode::BA_ETC_ITEM] * FLOAT100 ) + FLOAT0001;
		kData.m_pClassIcon  = NULL;
		kData.m_dwStartTime = 0;
		kData.m_szTitle     = STR(5);
		m_BonusData.push_back( kData );
		iCurBonusIcon++;
	}

	if( fBonusArray[ioPlayMode::BA_ETC_ITEM_EXP]  != 0.0f ||
		fBonusArray[ioPlayMode::BA_ETC_ITEM_PESO] != 0.0f  )
	{
		// ������ ����
		if( iCurBonusIcon >= iMax ) return;

		BonusData kData;
		kData.m_dwBonusID	= BONUS_ETC_ITEM_EXP_PESO;
		kData.m_iBonusPeso  = (float)( fBonusArray[ioPlayMode::BA_ETC_ITEM_PESO] * FLOAT100 ) + FLOAT0001;
		kData.m_iBonusExp   = (float)( fBonusArray[ioPlayMode::BA_ETC_ITEM_EXP] * FLOAT100 ) + FLOAT0001;
		kData.m_pClassIcon  = NULL;
		kData.m_dwStartTime = 0;
		kData.m_szTitle     = STR(6);
		m_BonusData.push_back( kData );
		iCurBonusIcon++;
	}

	// ��� ���ʽ�
	if( fBonusArray[ioPlayMode::BA_GUILD] != 0.0f )
	{
		// ������ ����
		if( iCurBonusIcon >= iMax ) return;

		BonusData kData;
		kData.m_dwBonusID	= BONUS_GUILD;
		kData.m_iBonusPeso  = (float)( fBonusArray[ioPlayMode::BA_GUILD] * FLOAT100 ) + FLOAT0001;
		kData.m_iBonusExp   = (float)( fBonusArray[ioPlayMode::BA_GUILD] * FLOAT100 ) + FLOAT0001;
		kData.m_pClassIcon  = NULL;
		kData.m_dwStartTime = 0;
		kData.m_szTitle     = STR(7);
		m_BonusData.push_back( kData );
		iCurBonusIcon++;
	}

	// ģ�� ���ʽ�
	if( fBonusArray[ioPlayMode::BA_FRIEND] != 0.0f ) 
	{
		// ������ ����
		if( iCurBonusIcon >= iMax ) return;

		BonusData kData;
		kData.m_dwBonusID	= BONUS_FRIEND;
		kData.m_iBonusPeso  = (float)( fBonusArray[ioPlayMode::BA_FRIEND] * FLOAT100 ) + FLOAT0001;
		kData.m_iBonusExp   = (float)( fBonusArray[ioPlayMode::BA_FRIEND] * FLOAT100 ) + FLOAT0001;
		kData.m_pClassIcon  = NULL;
		kData.m_dwStartTime = 0;
		kData.m_szTitle     = STR(8);
		m_BonusData.push_back( kData );
		iCurBonusIcon++;
	}

	// PCRoom ���ʽ�
	if( fBonusArray[ioPlayMode::BA_PCROOM_EXP] != 0.0f || fBonusArray[ioPlayMode::BA_PCROOM_PESO] != 0.0f )
	{
		// ������ ����
		if( iCurBonusIcon >= iMax ) return;

		BonusData kData;
		kData.m_dwBonusID	= BONUS_PCROOM;
		kData.m_iBonusPeso  = (float)( fBonusArray[ioPlayMode::BA_PCROOM_PESO] * FLOAT100 ) + FLOAT0001;
		kData.m_iBonusExp   = (float)( fBonusArray[ioPlayMode::BA_PCROOM_EXP] * FLOAT100 ) + FLOAT0001;
		kData.m_pClassIcon  = NULL;
		kData.m_dwStartTime = 0;
		kData.m_szTitle     = STR(9);
		m_BonusData.push_back( kData );
		iCurBonusIcon++;
	} 
	
	// �̼� ���ʽ�	
	if( fBonusArray[ioPlayMode::BA_PLAYMODE] != 0.0f )
	{
		// ������ ����
		if( iCurBonusIcon >= iMax ) return;

		BonusData kData;
		kData.m_dwBonusID	= BONUS_MODE;
		kData.m_iBonusPeso  = (float)( fBonusArray[ioPlayMode::BA_PLAYMODE] * FLOAT100 ) + FLOAT0001;
		kData.m_iBonusExp   = (float)( fBonusArray[ioPlayMode::BA_PLAYMODE] * FLOAT100 ) + FLOAT0001;
		kData.m_pClassIcon  = NULL;
		kData.m_dwStartTime = 0;
		kData.m_szTitle     = STR(10);
		m_BonusData.push_back( kData );
		iCurBonusIcon++;
	}

	// �û�� ���ʽ�
	if( fBonusArray[ioPlayMode::BA_AWARD_BONUS] != 0.0f )
	{
		// ������ ����
		if( iCurBonusIcon >= iMax ) return;

		BonusData kData;
		kData.m_dwBonusID	= BONUS_AWARD;
		kData.m_iBonusPeso  = (float)( fBonusArray[ioPlayMode::BA_AWARD_BONUS] * FLOAT100 ) + FLOAT0001;
		kData.m_iBonusExp   = 0;
		kData.m_pClassIcon  = NULL;
		kData.m_dwStartTime = 0;
		kData.m_szTitle     = STR(11);
		m_BonusData.push_back( kData );
		iCurBonusIcon++;
	}

	// ���Ӱ��� ���ʽ�
	if( fBonusArray[ioPlayMode::BA_MODE_CONSECUTIVELY] != 0.0f )
	{
		// ������ ����
		if( iCurBonusIcon >= iMax ) return;

		BonusData kData;
		kData.m_dwBonusID	= BONUS_MODE_CONSECUTIVELY;
		kData.m_iBonusPeso  = (float)( fBonusArray[ioPlayMode::BA_MODE_CONSECUTIVELY] * FLOAT100 ) + FLOAT0001;
		kData.m_iBonusExp   = (float)( fBonusArray[ioPlayMode::BA_MODE_CONSECUTIVELY] * FLOAT100 ) + FLOAT0001;
		kData.m_pClassIcon  = NULL;
		kData.m_dwStartTime = 0;
		kData.m_szTitle     = STR(17);
		m_BonusData.push_back( kData );
		iCurBonusIcon++;
	}

	// ���� ���ʽ�
	if( fBonusArray[ioPlayMode::BA_VICTORIES_PESO] != 0.0f )
	{
		// ������ ����
		if( iCurBonusIcon >= iMax ) return;

		BonusData kData;
		kData.m_dwBonusID	= BONUS_VICTORIES_PESO;
		kData.m_iBonusPeso  = (float)( fBonusArray[ioPlayMode::BA_VICTORIES_PESO] * FLOAT100 ) + FLOAT0001;
		kData.m_iBonusExp   = 0;
		kData.m_pClassIcon  = NULL;
		kData.m_dwStartTime = 0;
		kData.m_szTitle     = STR(12);
		m_BonusData.push_back( kData );
		iCurBonusIcon++;
	}

	// ������ ���ʽ�
	if( fBonusArray[ioPlayMode::BA_CAMP_BONUS] != 0.0f )
	{
		// ������ ����
		if( iCurBonusIcon >= iMax ) return;

		BonusData kData;
		kData.m_dwBonusID	= BONUS_CAMP_BATTLE;
		kData.m_iBonusPeso  = (float)( fBonusArray[ioPlayMode::BA_CAMP_BONUS] * FLOAT100 ) + FLOAT0001;
		kData.m_iBonusExp   = (float)( fBonusArray[ioPlayMode::BA_CAMP_BONUS] * FLOAT100 ) + FLOAT0001;
		kData.m_pClassIcon  = NULL;
		kData.m_dwStartTime = 0;
		kData.m_szTitle     = STR(13);
		m_BonusData.push_back( kData );
		iCurBonusIcon++;
	}

	// �̺�Ʈ ��� ���ʽ�
	// Event ���ʽ�
	if( fBonusArray[ioPlayMode::BA_EVENT] != 0.0f &&
		fBonusArray[ioPlayMode::BA_EVENT_PESO] != 0.0f )
	{
		// ������ ����
		if( iCurBonusIcon >= iMax ) return;

		BonusData kData;
		kData.m_dwBonusID	= BONUS_EVENT_PESO_EXP;
		kData.m_iBonusPeso  = (float)( fBonusArray[ioPlayMode::BA_EVENT_PESO] * FLOAT100 ) + FLOAT0001;
		kData.m_iBonusExp   = (float)( fBonusArray[ioPlayMode::BA_EVENT] * FLOAT100 ) + FLOAT0001;
		kData.m_pClassIcon  = NULL;
		kData.m_dwStartTime = 0;
		kData.m_szTitle     = STR(14);
		m_BonusData.push_back( kData );
		iCurBonusIcon++;
	}
	else if( fBonusArray[ioPlayMode::BA_EVENT_PESO] != 0.0f )
	{
		// ������ ����
		if( iCurBonusIcon >= iMax ) return;

		BonusData kData;
		kData.m_dwBonusID	= BONUS_PESO_EVENT;
		kData.m_iBonusPeso  = (float)( fBonusArray[ioPlayMode::BA_EVENT_PESO] * FLOAT100 ) + FLOAT0001;
		kData.m_iBonusExp   = 0;
		kData.m_pClassIcon  = NULL;
		kData.m_dwStartTime = 0;
		kData.m_szTitle     = STR(15);
		m_BonusData.push_back( kData );
		iCurBonusIcon++;
	}
	else if( fBonusArray[ioPlayMode::BA_EVENT] != 0.0f )
	{
		// ������ ����
		if( iCurBonusIcon >= iMax ) return;

		BonusData kData;
		kData.m_dwBonusID	= BONUS_EVENT;
		kData.m_iBonusPeso  = 0;
		kData.m_iBonusExp   = (float)( fBonusArray[ioPlayMode::BA_EVENT] * FLOAT100 ) + FLOAT0001;
		kData.m_pClassIcon  = NULL;
		kData.m_dwStartTime = 0;
		kData.m_szTitle     = STR(16);
		m_BonusData.push_back( kData );
		iCurBonusIcon++;
	}

#if defined( USE_GA )
	char chSlash[16]		= {0,};
	char chPostData[256]	= {0,};	

	// set battle end time;
	int iTime = (int)REALGETTIME() / 1000;
	g_HttpMng.SetBattleEndTime( iTime );

	if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
		sprintf_e( chSlash, "%%2F" );
	else
		SafeSprintf( chSlash, sizeof(chSlash), "%2F" );

	int iModeType = (int)ioPlayMode::GetModeType();
	switch( iModeType )
	{
	case MT_SYMBOL:
		{
			// PLAY_RESULT_POWERSTONE
			if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
				sprintf_e( chPostData, "%sPLAY%sRESULT%sPOWERSTONE", chSlash, chSlash, chSlash );
			else
				SafeSprintf( chPostData, sizeof(chPostData), "%1PLAY%2RESULT%3POWERSTONE", chSlash, chSlash, chSlash );

			g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), chPostData, 7 );
		}
		break;
	case MT_CATCH:
		{
			// PLAY_RESULT_PRISONER
			if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
				sprintf_e( chPostData, "%sPLAY%sRESULT%sPRISONER", chSlash, chSlash, chSlash );
			else
				SafeSprintf( chPostData, sizeof(chPostData), "%1PLAY%2RESULT%3PRISONER", chSlash, chSlash, chSlash );

			g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), chPostData, 7 );
		}
		break;
	case MT_KING:
		{
			// PLAY_RESULT_HIDENCROWN
			if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
				sprintf_e( chPostData, "%sPLAY%sRESULT%sHIDENCROWN", chSlash, chSlash, chSlash );
			else
				SafeSprintf( chPostData, sizeof(chPostData), "%1PLAY%2RESULT%3HIDENCROWN", chSlash, chSlash, chSlash );

			g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), chPostData, 7 );
		}
		break;
	case MT_SURVIVAL:
		{
			// PLAY_RESULT_DEATH
			if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
				sprintf_e( chPostData, "%sPLAY%sRESULT%sDEATH", chSlash, chSlash, chSlash );
			else
				SafeSprintf( chPostData, sizeof(chPostData), "%1PLAY%2RESULT%3DEATH", chSlash, chSlash, chSlash );

			g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), chPostData, 7 );
		}
		break;
	case MT_TEAM_SURVIVAL:
		{
			// PLAY_RESULT_TEAMDEATH
			if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
				sprintf_e( chPostData, "%sPLAY%sRESULT%sTEAMDEATH", chSlash, chSlash, chSlash );
			else
				SafeSprintf( chPostData, sizeof(chPostData), "%1PLAY%2RESULT%3TEAMDEATH", chSlash, chSlash, chSlash );

			g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), chPostData, 7 );
		}
		break;
	case MT_BOSS:
		{
			// PLAY_RESULT_BOSSRAID
			if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
				sprintf_e( chPostData, "%sPLAY%sRESULT%sBOSSRAID", chSlash, chSlash, chSlash );
			else
				SafeSprintf( chPostData, sizeof(chPostData), "%1PLAY%2RESULT%3BOSSRAID", chSlash, chSlash, chSlash );

			g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), chPostData, 7 );
		}
		break;
	case MT_MONSTER_SURVIVAL:
		{
			// PLAY_RESULT_SKELETON
			if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
				sprintf_e( chPostData, "%sPLAY%sRESULT%sSKELETON", chSlash, chSlash, chSlash );
			else
				SafeSprintf( chPostData, sizeof(chPostData), "%1PLAY%2RESULT%3SKELETON", chSlash, chSlash, chSlash );

			g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), chPostData, 7 );
		}
		break;
	case MT_FOOTBALL:
		{
			// PLAY_RESULT_SOCCER
			if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
				sprintf_e( chPostData, "%sPLAY%sRESULT%sSOCCER", chSlash, chSlash, chSlash );
			else
				SafeSprintf( chPostData, sizeof(chPostData), "%1PLAY%2RESULT%3SOCCER", chSlash, chSlash, chSlash );

			g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), chPostData, 7 );
		}
		break;
	case MT_HEROMATCH:
		{
			// PLAY_RESULT_LADDER
			if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
				sprintf_e( chPostData, "%sPLAY%sRESULT%sLADDER", chSlash, chSlash, chSlash );
			else
				SafeSprintf( chPostData, sizeof(chPostData), "%1PLAY%2RESULT%3LADDER", chSlash, chSlash, chSlash );

			g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), chPostData, 7 );
		}
		break;
	case MT_GANGSI:
		{
			// PLAY_RESULT_GANGSI
			if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
				sprintf_e( chPostData, "%sPLAY%sRESULT%sGANGSI", chSlash, chSlash, chSlash );
			else
				SafeSprintf( chPostData, sizeof(chPostData), "%1PLAY%2RESULT%3GANGSI", chSlash, chSlash, chSlash );

			g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), chPostData, 7 );
		}
		break;
	case MT_CATCH_RUNNINGMAN:
		{
			// PLAY_RESULT_DWARF
			if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
				sprintf_e( chPostData, "%sPLAY%sRESULT%sDWARF", chSlash, chSlash, chSlash );
			else
				SafeSprintf( chPostData, sizeof(chPostData), "%1PLAY%2RESULT%3DWARF", chSlash, chSlash, chSlash );

			g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), chPostData, 7 );
		}
		break;
	case MT_FIGHT_CLUB:
		{
			// PLAY_RESULT_CHAMP
			if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
				sprintf_e( chPostData, "%sPLAY%sRESULT%sCHAMP", chSlash, chSlash, chSlash );
			else
				SafeSprintf( chPostData, sizeof(chPostData), "%1PLAY%2RESULT%3CHAMP", chSlash, chSlash, chSlash );

			g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), chPostData, 7 );
		}
		break;
	case MT_TOWER_DEFENSE:
		{
			// PLAY_RESULT_FOREST
			if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
				sprintf_e( chPostData, "%sPLAY%sRESULT%sFOREST", chSlash, chSlash, chSlash );
			else
				SafeSprintf( chPostData, sizeof(chPostData), "%1PLAY%2RESULT%3FOREST", chSlash, chSlash, chSlash );

			g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), chPostData, 7 );
		}
		break;
	case MT_DARK_XMAS:
		{
			// PLAY_RESULT_SNOW
			if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
				sprintf_e( chPostData, "%sPLAY%sRESULT%sSNOW", chSlash, chSlash, chSlash );
			else
				SafeSprintf( chPostData, sizeof(chPostData), "%1PLAY%2RESULT%3SNOW", chSlash, chSlash, chSlash );

			g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), chPostData, 7 );
		}
		break;
	case MT_FIRE_TEMPLE:
		{
			// PLAY_RESULT_FIRE
			if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
				sprintf_e( chPostData, "%sPLAY%sRESULT%sFIRE", chSlash, chSlash, chSlash );
			else
				SafeSprintf( chPostData, sizeof(chPostData), "%1PLAY%2RESULT%3FIRE", chSlash, chSlash, chSlash );

			g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), chPostData, 7 );
		}
		break;
	case MT_DOUBLE_CROWN:
		{
			// PLAY_RESULT_DOUBLECROWN
			if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
				sprintf_e( chPostData, "%sPLAY%sRESULT%sDOUBLECROWN", chSlash, chSlash, chSlash );
			else
				SafeSprintf( chPostData, sizeof(chPostData), "%1PLAY%2RESULT%3DOUBLECROWN", chSlash, chSlash, chSlash );

			g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), chPostData, 7 );
		}
		break;
	case MT_SHUFFLE_BONUS:
		{
			// PLAY_RESULT_BONUS
			if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
				sprintf_e( chPostData, "%sPLAY%sRESULT%sBONUS", chSlash, chSlash, chSlash );
			else
				SafeSprintf( chPostData, sizeof(chPostData), "%1PLAY%2RESULT%3BONUS", chSlash, chSlash, chSlash );

			g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), chPostData, 7 );
		}
		break;
	case MT_FACTORY:
		{
			// PLAY_RESULT_FACTORY
			if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
				sprintf_e( chPostData, "%sPLAY%sRESULT%sFACTORY", chSlash, chSlash, chSlash );
			else
				SafeSprintf( chPostData, sizeof(chPostData), "%1PLAY%2RESULT%3FACTORY", chSlash, chSlash, chSlash );

			g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), chPostData, 7 );
		}
		break;
	case MT_TEAM_SURVIVAL_AI:
		{
			// PLAY_RESULT_TEAMDEATH_AI
			if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
				sprintf_e( chPostData, "%sPLAY%sRESULT%sTEAMDEATH_AI", chSlash, chSlash, chSlash );
			else
				SafeSprintf( chPostData, sizeof(chPostData), "%1PLAY%2RESULT%3TEAMDEATH_AI", chSlash, chSlash, chSlash );

			g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), chPostData, 7 );
		}
		break;
	case MT_RAID:
		{

		}
		break;
	case MT_UNDERWEAR:
		{
			// PLAY_RESULT_PRISONER
			if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
				sprintf_e( chPostData, "%sPLAY%sRESULT%sUNDERWEAR", chSlash, chSlash, chSlash );
			else
				SafeSprintf( chPostData, sizeof(chPostData), "%1PLAY%2RESULT%3UNDERWEAR", chSlash, chSlash, chSlash );

			g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), chPostData, 7 );
		}
		break;
	case MT_CBT:
		{
			// PLAY_RESULT_PRISONER
			if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
				sprintf_e( chPostData, "%sPLAY%sRESULT%sCBT", chSlash, chSlash, chSlash );
			else
				SafeSprintf( chPostData, sizeof(chPostData), "%1PLAY%2RESULT%3CBT", chSlash, chSlash, chSlash );

			g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), chPostData, 7 );
		}
		break;
	}

	if( iModeType != MT_NONE && 
		iModeType != MT_TRAINING &&
		iModeType != MT_HEADQUARTERS &&
		iModeType != MT_HOUSE &&
		g_HttpMng.GetSendHeroChange() == true )
	{
		if( g_HttpMng.GetHeroStartTime() != 0 )
		{
			// end choice hero duration time;
			int iTime = (int)REALGETTIME() / 1000;
			g_HttpMng.SetHeroEndTime( iTime );

			char chType[64]			= {0,};
			char chMode[64]			= {0,};

			if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
			{
				sprintf_e( chType, "%s%s", chSlash, g_HttpMng.GetBattleType() );
				sprintf_e( chMode, "%s%s", chSlash, g_HttpMng.GetBattleMode() );
				sprintf_e( chPostData, "%sPLAY%s%s%sHERO_CHANGE", chSlash, chType, chMode, chSlash );
			}
			else
			{
				SafeSprintf( chType, sizeof(chType), "%1%2", chSlash, g_HttpMng.GetBattleType() );
				SafeSprintf( chMode, sizeof(chMode), "%1%2", chSlash, g_HttpMng.GetBattleMode() );
				SafeSprintf( chPostData, sizeof(chPostData), "%1PLAY%2%3%4HERO_CHANGE", chSlash, chType, chMode, chSlash );
			}

			// PLAY_<TYPE>_<MODE>_HERO_CHANGE
			g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), chPostData, 9 );

			g_HttpMng.SetSendHeroChange( false );
			g_HttpMng.SetHeroStartTime( 0 );
			g_HttpMng.SetHeroEndTime( 0 );
			g_HttpMng.SetHeroIndex( 0 );
		}
		else
		{
			// start choice hero duration time;
			int iTime = (int)REALGETTIME() / 1000;
			g_HttpMng.SetHeroStartTime( iTime );				
			g_HttpMng.SetHeroIndex( g_MyInfo.GetClassType() );
		}
	}

#endif

}

void TotalResultRawardWnd::OnRenderShufflePoint( int iXPos, int iYPos )
{
	if( m_pShufflePointFrm )
		m_pShufflePointFrm->Render( iXPos, iYPos );

	if( m_pShuffleStarBg )
		m_pShuffleStarBg->Render( iXPos + 33, iYPos + 29, UI_RENDER_MULTIPLY, TFO_BILINEAR );

	if( m_pShuffleStar )
		m_pShuffleStar->Render( iXPos + 33, iYPos + 29, UI_RENDER_NORMAL, TFO_BILINEAR );

	if( m_pDarkLine )
		m_pDarkLine->Render( iXPos + 63, iYPos + 6 );

	if( m_pLightLine )
		m_pLightLine->Render( iXPos + 63, iYPos + 23 );

	if( m_pDarkLine )
		m_pDarkLine->Render( iXPos + 63, iYPos + 40 );

	if( m_pLightLine )
		m_pLightLine->Render( iXPos + 63, iYPos + 57 );

	// ���ʽ�
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
	g_FontMgr.PrintText( iXPos + 33, iYPos + 50,  FONT_SIZE_10, STR(1) );
	
	ioComplexStringPrinter kPrinter;
	kPrinter.SetTextStyle( TS_NORMAL );
	kPrinter.SetTextColor( TCT_DEFAULT_BLUE );
	kPrinter.AddTextPiece( FONT_SIZE_10, STR(2), m_iPlayPoint + m_iWinningPoint + m_iConsecutivePoint + m_iAwardPoint );
	kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter.AddTextPiece( FONT_SIZE_10, STR(3) );
	kPrinter.PrintFullText( iXPos + 33, iYPos + 62, TAT_CENTER );
	kPrinter.ClearList();
	
	g_FontMgr.PrintText( iXPos + 66, iYPos + 9,  FONT_SIZE_10, STR(4) );
	g_FontMgr.PrintText( iXPos + 66, iYPos + 26, FONT_SIZE_10, STR(5) );
	g_FontMgr.PrintText( iXPos + 66, iYPos + 43, FONT_SIZE_10, STR(6) );
	g_FontMgr.PrintText( iXPos + 66, iYPos + 60, FONT_SIZE_10, STR(7) );

	g_FontMgr.SetAlignType( TAT_RIGHT );
	g_FontMgr.SetTextColor( TCT_DEFAULT_BLUE );
	g_FontMgr.PrintText( iXPos + 178, iYPos + 9,  FONT_SIZE_10, STR(8), m_iPlayPoint );
	g_FontMgr.SetTextColor( TCT_DEFAULT_GREEN );
	g_FontMgr.PrintText( iXPos + 178, iYPos + 26, FONT_SIZE_10, STR(9), m_iWinningPoint );
	g_FontMgr.PrintText( iXPos + 178, iYPos + 43, FONT_SIZE_10, STR(10), m_iConsecutivePoint );
	g_FontMgr.PrintText( iXPos + 178, iYPos + 60, FONT_SIZE_10, STR(11), m_iAwardPoint );
}

void TotalResultRawardWnd::SetShufflePoint( int iPlayPoint, int iWinningPoint, int iConsecutivePoint, int iAwardPoint )
{
	m_iPlayPoint		= iPlayPoint;
	m_iWinningPoint		= iWinningPoint;
	m_iConsecutivePoint = iConsecutivePoint;
	m_iAwardPoint		= iAwardPoint;
}