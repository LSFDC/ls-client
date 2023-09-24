

#include "StdAfx.h"

#include "../io3DEngine/ioWnd.h"
#include "../io3DEngine/ioFontManager.h"
#include "../io3DEngine/ioUIRenderImage.h"
#include "../io3DEngine/ioUIImageSetManager.h"
#include "../io3DEngine/ioUIRenderFrame.h"
#include "../io3DEngine/ioXMLElement.h"
#include "../io3DEngine/ioSoundManager.h"
#include "../io3DEngine/HelpFunc.h"
#include "../io3DEngine/iostringmanager.h"
#include "../io3dengine/safesprintf.h"
#include "../ioVoiceChat/Fourier.h"

#include "../WndID.h"
#include "../ioMyInfo.h"
#include "../Setting.h"
#include "../NetworkWrappingFunc.h"
#include "../ioKeyManager.h"
#include "../io3DSkillCharIcon.h"
#include "../TextColorTable.h"
#include "../ioWeaponManager.h"
#include "../ioApplication.h"
#include "../ioMineWeapon.h"
#include "../ioGraffitiWeapon.h"

#include "../ioItemMaker.h"
#include "../ioBowItem.h"
#include "../ioBowItem2.h"
#include "../ioChargeComboBowItem.h"
#include "../ioFireWeaponItem.h"
#include "../ioAutoShotItem.h"
#include "../ioCreateStaffItem.h"
#include "../ioLaserItem.h"
#include "../ioMineItem.h"
#include "../ioChargeShotItem.h"
#include "../ioChargeShotItem2.h"
#include "../ioCrossBowItem.h"
#include "../ioUroborusItem.h"
#include "../ioWereWolfItem.h"
#include "../ioBuffAddItem.h"
#include "../ioRachelItem.h"
#include "../ioIljimaeItem.h"
#include "../ioAlterEgoItem.h"
#include "../ioWitchItem.h"
#include "../ioExcaliburItem.h"
#include "../ioMechanicsItem.h"
#include "../ioPhantomItem.h"
#include "../ioYuMiRaItem.h"
#include "../ioJinMoRiItem.h"
#include "../ioLuciferItem.h"
#include "../ioChargeContactWeaponItem.h"
#include "../ioNakoruruItem.h"
#include "../ioDestroyerItem.h"
#include "../ioMichaelItem.h"
#include "../ioGoomihoItem.h"
#include "../ioBladeItem.h"
#include "../ioPeterpanItem.h"
#include "../ioSuccubusItem.h"
#include "../ioCyvogueItem.h"
#include "../ioDarkSeasItem.h"
#include "../ioTrinityItem.h"
#include "../ioWindLordItem.h"
#include "../ioLeeItem.h"
#include "../ioShaolinItem.h"
#include "../ioGhostStealer.h"
#include "../ioDraculaItem.h"
#include "../ioProject_K_Item.h"
#include "../ioHakumenItem.h"
#include "../ioDevilItem.h"
#include "../ioMaidItem.h"
#include "../ioFlash_Item.h"
#include "../ioEvyItem.h"
#include "../ioPsychoMetry_Item.h"
#include "../ioSuddenAttackItem.h"
#include "../ioSlasher_Item.h"
#include "../ioDummyCharItem2.h"
#include "../ioTitanItem.h"
#include "../ioSuperDimensionItem.h"
#include "../ioTraceItem.h"
#include "../ioKaelthasItem.h"
#include "../ioGalaxyItem.h"
#include "../ioDragonSlayerItem.h"
#include "../ioNanginItem.h"
#include "../ioStrikerItem.h"
#include "../ioDonQuixoteItem.h"
#include "../ioThunderBirdItem.h"
#include "../ioAliceItem.h"
#include "../ioValkyrie.h"
#include "../ioParkilpyoItem.h"
#include "../ioJoroExpansionItem.h"
#include "../ioEarthQuakeItem.h"
#include "../ioPentjakSilatItem.h"
#include "../ioJackItem.h"
#include "../ioShadowRangerItem.h"
#include "../ioDarkKnightItem.h"
#include "../ioJumperItem.h"
#include "../ioEngineerItem.h"
#include "../ioGeomjeItem.h"
#include "../ioZeusItem.h"
#include "../ioSaberItem.h"
#include "../ioOutBoxerItem.h"
#include "../ioPowerChargeItem.h"
#include "../ioPowerChargeItem4.h"
#include "../ioPowerChargeItem5.h"
#include "../ioPowerChargeItem6.h"
#include "../ioRunChargeItem.h"
#include "../ioSmartBoomObjectItem.h"
#include "../ioHadesItem.h"
#include "../ioMagicSwordItem.h"
#include "../ioNataItem.h"
#include "../ioLeeSunSin.h"
#include "../ioResearcher.h"
#include "../ioBullFightItem.h"
#include "../ioJeonWooChiItem.h"
#include "../ioSawangItem.h"
#include "../ioChainMagicianItem.h"
#include "../ioRedHoodItem.h"
#include "../ioStriderItem.h"
#include "../ioApostatemonkItem.h"
#include "../ioMutantItem.h"
#include "../ioGanesa.h"
#include "../ioSamuraiExpansionItem.h"
#include "../ioMirWarrior.h"
#include "../ioCuchulainItem.h"
#include "../ioGalfordItem.h"
#include "../ioOniMushaItem.h"
#include "../ioWestGunExtendItem.h"
#include "../ioDruidItem.h"
#include "../ioSasinmuItem.h"
#include "../ioPredatorItem.h"
#include "../ioCrowItem.h"
#include "../ioMephistoItem.h"
#include "../ioBlackSpearItem.h"

#include "../SkillTypeID.h"
#include "../ioSkillMaker.h"
#include "../ioSkill.h"
#include "../ioBuffSkill.h"
#include "../ioPassiveSkill.h"
#include "../ioMineSkill.h"
#include "../ioCountSkill.h"
#include "../ioCreateDummySkill.h"
#include "../ioRepeatMoveAttack.h"
#include "../ioRepeatMoveAttack2.h"
#include "../ioDashMoveChargeAttack.h"
#include "../ioMoveAttackSkill2.h"
#include "../ioEngineerMachineGunAttack.h"
#include "../ioControlDummySkill.h"

#include "../ioBuff.h"
#include "../ioFlyJumpExtraAttackBuff.h"

#include "../ioBaseChar.h"
#include "../ioCreateChar.h"
#include "../ioGrowthLevel.h"
#include "../ioGrowthInfoManager.h"
#include "../ioMedalItemInfoManager.h"
#include "../EtcHelpFunc.h"

#include "../ioTraceMineStateBuff.h"

#include "../ioConsecution3Skill.h"
#include "../ioBlindlyShotSkill.h"
#include "../ioSoldierAwakeManager.h"
#include "../ioRsoldierInfoManager.h"

#include "GlobalWnd.h"
#include "ioModeHelpManager.h"
#include "SoldierManualWnd.h"
#include "ioSP2GUIManager.h"
#include "ioTutorialManager.h"
#include "MyInventoryWnd.h"

#include "SkillListWnd.h"

#include <strsafe.h>
#include "../Local/ioLocalManager.h"
#include "UserInfoToolTip.h"

SkillCharIcon::SkillCharIcon()
{
	m_pGaugeBackBg = NULL;
	m_pGaugeBack = NULL;
	m_pGauge = NULL;
	m_pGaugeAdd = NULL;

	m_pLVText = NULL;
	m_pNumText = NULL;

	m_pSkillCharIcon = NULL;
	m_pSkillListWnd = NULL;

	m_fExpRate = 0.0f;
	m_iCurLevel = 0;
	m_iClassType = 0;
}

SkillCharIcon::~SkillCharIcon()
{
	SAFEDELETE(m_pGaugeBackBg);
	SAFEDELETE(m_pGaugeBack);
	SAFEDELETE(m_pGauge);
	SAFEDELETE(m_pGaugeAdd);

	SAFEDELETE(m_pLVText);
	SAFEDELETE(m_pNumText);

	SAFEDELETE(m_pSkillCharIcon);
}

void SkillCharIcon::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "GaugeBackBg")
	{
		SAFEDELETE( m_pGaugeBackBg );
		m_pGaugeBackBg = pImage;
	}
	else if( szType == "GaugeBack" )
	{
		SAFEDELETE( m_pGaugeBack );
		m_pGaugeBack = pImage;
	}
	else if( szType == "Gauge" )
	{
		SAFEDELETE( m_pGauge );
		m_pGauge = pImage;
	}
	else if( szType == "GaugeAdd" )
	{
		SAFEDELETE( m_pGaugeAdd );
		m_pGaugeAdd = pImage;
	}
	else if( szType == "LvText" )
	{
		SAFEDELETE( m_pLVText );
		m_pLVText = pImage;
	}
	else if( szType == "NumText" )
	{
		SAFEDELETE( m_pNumText );
		m_pNumText = pImage;
	}
	else
	{
		ioWnd::AddRenderImage( szType, pImage );
	}
}

void SkillCharIcon::iwm_create()
{
	SkillListWnd *pSkillListWnd = dynamic_cast<SkillListWnd *>( m_pParent );
	if(pSkillListWnd)
	{
		m_pSkillListWnd = pSkillListWnd;
	}
}

void SkillCharIcon::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;

	if( m_pSkillCharIcon )
	{
		m_pSkillCharIcon->Update( fTimePerSec );
	}

	ioButton::OnProcess( fTimePerSec );
}

void SkillCharIcon::OnRender()
{
#ifndef JUNGI_WORK
	if( !IsShow() ) return;

	if( m_pSkillListWnd )
	{
		if( m_pSkillListWnd->SkipRender() )
			return;
	}

	RenderChar();
	RenderGauge();
#endif

	ioButton::OnRender();
}

void SkillCharIcon::RenderChar()
{
	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	if( m_pSkillCharIcon )
	{
		m_pSkillCharIcon->Render( iXPos, iYPos );
	}
}

void SkillCharIcon::RenderGauge()
{
	enum
	{
		GAUGE_X		= 10,
		GAUGE_Y		= 74,

		TEXT_X		= 46,
		TEXT_Y		= 75,

		LEVEL_SIZE	= 13,
		FIRST_SIZE	= 8,
		SECOND_SIZE	= 15,
		THIRD_SIZE	= 22,
	};

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	if( m_pGauge && m_pGaugeAdd && m_pGaugeBack && m_pGaugeBackBg &&
		m_pLVText && m_pNumText )
	{
		// Info Check
		m_iClassType = g_MyInfo.GetClassType();
		m_iCurLevel = g_MyInfo.GetClassLevel( m_iClassType, false );
		if( g_MyInfo.IsCharExerciseStyleToClassType( m_iClassType, EXERCISE_RENTAL ) )
		{
			m_fExpRate = 0.0f;
		}
		else 
		{	
			m_fExpRate = (float)max( 0, g_MyInfo.GetExpRate( m_iClassType ) ) / FLOAT100;
		}

		// Gauge
		int iGaugeXPos = iXPos - GetXPos() + GAUGE_X;
		int iGaugeYPos = iYPos - GetYPos() + GAUGE_Y;

		m_pGaugeBackBg->Render( iGaugeXPos, iGaugeYPos );
		m_pGaugeBack->Render( iGaugeXPos, iGaugeYPos );

		if( m_fExpRate > 0.0f )
		{
			int iWidth = (float)m_pGauge->GetWidth() * m_fExpRate;
			m_pGauge->RenderWidthCut( iGaugeXPos, iGaugeYPos, 0, iWidth );
		}

		m_pGaugeAdd->Render( iGaugeXPos, iGaugeYPos, UI_RENDER_ADD );

		// Level Num
		int iLevelXPos = iXPos - GetXPos() + TEXT_X;
		int iLevelYPos = iYPos - GetYPos() + TEXT_Y;

		float fNewXPos = 0.0f;
		if( m_iCurLevel < 10 )
		{
			fNewXPos = iLevelXPos - (LEVEL_SIZE+FIRST_SIZE)/2;
			m_pLVText->Render( fNewXPos, iLevelYPos );

			fNewXPos += LEVEL_SIZE;
			m_pNumText->RenderNum( fNewXPos, iLevelYPos, m_iCurLevel );
		}
		else if( m_iCurLevel < FLOAT100 )
		{
			fNewXPos = iLevelXPos - (LEVEL_SIZE+SECOND_SIZE)/2;
			m_pLVText->Render( fNewXPos, iLevelYPos );

			fNewXPos += LEVEL_SIZE;
			m_pNumText->RenderNum( fNewXPos, iLevelYPos, m_iCurLevel, -FLOAT1 );
		}
		else
		{
			fNewXPos = iLevelXPos - (LEVEL_SIZE+THIRD_SIZE)/2;
			m_pLVText->Render( fNewXPos, iLevelYPos );

			fNewXPos += LEVEL_SIZE;
			m_pNumText->RenderNum( fNewXPos, iLevelYPos, m_iCurLevel, -FLOAT1 );
		}
	}
}

void SkillCharIcon::SetAvata( const CHARACTER &rkInfo )
{
	if( !m_pSkillCharIcon )
	{
		m_pSkillCharIcon = new io3DSkillCharIcon;
		m_pSkillCharIcon->Initialize();
	}

	if( m_pSkillCharIcon )
	{
		m_pSkillCharIcon->SetAvata( rkInfo );
	}
}

void SkillCharIcon::DestroyAvata()
{
	SAFEDELETE( m_pSkillCharIcon );
}

void SkillCharIcon::AvataFaceChange( const char *szTexPostfix, int iDuration )
{
	if( m_pSkillCharIcon )
		m_pSkillCharIcon->FaceChange( szTexPostfix, iDuration );
}

void SkillCharIcon::SetAcion()
{
	if( !IsShow() ) return;

	if( m_pSkillListWnd )
	{
		if( m_pSkillListWnd->SkipRender() )
			return;
	}

	if( !g_MyInfo.IsTutorialUser() )
	{
		MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(g_GUIMgr.FindWnd(MY_INVENTORY_WND));
		if( pInvenWnd )
		{
			int iClassType = g_MyInfo.GetClassType();
			pInvenWnd->ShowSoldierTabDirect( iClassType );
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SkillCharBtn::SkillCharBtn()
{
	m_pNormal = NULL;
	m_pOver = NULL;
	m_pTimeGrowthText = NULL;
	m_pTimeGrowthTextEffect = NULL;

	m_dwEffectTime = 0;
	m_dwEffectCheckTime = 0;
}

SkillCharBtn::~SkillCharBtn()
{
	SAFEDELETE(m_pNormal);
	SAFEDELETE(m_pOver);
	SAFEDELETE(m_pTimeGrowthText);
	SAFEDELETE(m_pTimeGrowthTextEffect);
}

void SkillCharBtn::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "BtnNormal")
	{
		SAFEDELETE( m_pNormal );
		m_pNormal = pImage;
	}
	else if( szType == "BtnOver" )
	{
		SAFEDELETE( m_pOver );
		m_pOver = pImage;
	}
	else if( szType == "TimeGrowthText" )
	{
		SAFEDELETE( m_pTimeGrowthText );
		m_pTimeGrowthText = pImage;
	}
	else if( szType == "TimeGrowthTextEffect" )
	{
		SAFEDELETE( m_pTimeGrowthTextEffect );
		m_pTimeGrowthTextEffect = pImage;
	}
	else
	{
		ioWnd::AddRenderImage( szType, pImage );
	}
}

void SkillCharBtn::ParseExtraInfo( ioXMLElement &xElement )
{
	ioButton::ParseExtraInfo( xElement );
	m_dwEffectTime = xElement.GetIntAttribute_e( "EffectTime" );
}

void SkillCharBtn::iwm_create()
{
	SkillListWnd *pSkillListWnd = dynamic_cast<SkillListWnd *>( m_pParent );
	if(pSkillListWnd)
	{
		m_pSkillListWnd = pSkillListWnd;
	}
}

void SkillCharBtn::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;
	if( m_pSkillListWnd )
	{
		if( m_pSkillListWnd->SkipRender() )
			return;
	}

	m_bTimeGrowth = false;
	int iClassType = g_MyInfo.GetClassType(g_MyInfo.GetSelectCharArray());
	if( g_MyInfo.GetGrowthLevel()->FindTimeGrowthInfo( iClassType ) )
		m_bTimeGrowth = true;

	if( !m_bTimeGrowth )
	{
		m_bEffect = false;
		return;
	}

	DWORD dwCurTime = FRAMEGETTIME();
	if( dwCurTime - m_dwEffectCheckTime < m_dwEffectTime )
		return;

	m_dwEffectCheckTime = dwCurTime;
	m_bEffect = !m_bEffect;
}

void SkillCharBtn::OnRender()
{
	if( !IsShow() ) return;

	if( m_pSkillListWnd )
	{
		if( m_pSkillListWnd->SkipRender() )
			return;
	}

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	if( m_bTimeGrowth )
	{
		if( m_pTimeGrowthText )
			m_pTimeGrowthText->Render( iXPos, iYPos );

		if( m_pTimeGrowthTextEffect && m_bEffect )
			m_pTimeGrowthTextEffect->Render( iXPos, iYPos, UI_RENDER_SCREEN );
	}
	else
	{
		if( m_pNormal && m_pOver )
		{
			if( m_pSkillListWnd && m_pSkillListWnd->GetCurOverID() == GetID() )
				m_pOver->Render( iXPos, iYPos );
			else
				m_pNormal->Render( iXPos, iYPos );
		}
	}

	ioButton::OnRender();
}

void SkillCharBtn::SetAcion()
{
	if( !IsShow() ) return;

	if( m_pSkillListWnd )
	{
		if( m_pSkillListWnd->SkipRender() )
			return;
	}

	if( !g_MyInfo.IsTutorialUser() )
	{
		MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(g_GUIMgr.FindWnd(MY_INVENTORY_WND));
		if( pInvenWnd )
		{
			int iClassType = g_MyInfo.GetClassType();
			pInvenWnd->ShowSoldierTabDirect( iClassType );
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool SkillListBtn::SlotInfo::IsNeedKeyRender() const
{
	if( !pSkillImg || !pItemImg )
		return false;
	if( !bOnOffEnable && IsNeedBuffGaugeProtect() )
		return false;
	if(	iSkillType == ST_PASSIVE )
		return false;

	return true;
}

bool SkillListBtn::SlotInfo::IsNeedBuffGaugeProtect() const
{
	if( pSkillImg && iSkillType == ST_BUFF && m_iCurGauge == 0 && bUseBuff )
		return true;

	return false;
}

void SkillListBtn::SlotInfo::ProcessScale()
{
	if( !bSlotGray && bUseScale )
	{
		if( m_dwScaleStartTime == 0 )
			m_dwScaleStartTime = FRAMEGETTIME();

		DWORD dwGapTime = FRAMEGETTIME() - m_dwScaleStartTime;
		if( dwGapTime > m_dwScaleTime )
		{
			dwGapTime -= m_dwScaleTime;
			m_dwScaleStartTime = FRAMEGETTIME() - ( dwGapTime % m_dwScaleTime );
		}

		float fTimeRate = (float)dwGapTime / (float)m_dwScaleTime;
		m_fCurScaleRate = FLOAT1 + m_fScaleRate * sinf( D3DX_PI * fTimeRate );
	}
	else
	{
		m_fCurScaleRate = FLOAT1;
		m_dwScaleStartTime = 0;
	}

	if( pSkillImg )
	{
		if( pItemImg )
			pItemImg->SetScale( m_fCurScaleRate );

		pSkillImg->SetScale( m_fCurScaleRate );

		if( pBackImg )
			pBackImg->SetScale( m_fCurScaleRate );
	}
	else
	{
		if( pBackImg )
			pBackImg->SetScale( FLOAT1 );
	}
}

SkillListBtn::SkillListBtn()
{
	m_pGrowthNum = NULL;
	m_pGrowthPlus = NULL;

	m_pBulletBack  = NULL;
	m_pBulletOver  = NULL;
	m_pGunOn       = NULL;
	m_pBeamOn      = NULL;
	m_pMineOn      = NULL;
	m_pGunOff      = NULL;
	m_pBeamOff     = NULL;
	m_pNumberOn    = NULL;
	m_pNumberOff   = NULL;
	m_pBeamNumberOn = NULL;
	m_pMoonOn = NULL;
	m_pMoonOff = NULL;
	m_pHideOff = NULL;
	m_pHideOn = NULL;
	m_pSylphidOff = NULL;
	m_pSylphidOn = NULL;
	m_pAlterEgoOn = NULL;
	m_pAlterEgoOff = NULL;
	m_pWitchOn = NULL;
	m_pWitchOff = NULL;
	m_pMechanicsOn = NULL;
	m_pMechanicsOff = NULL;
	
	m_pActiveCountBack	= NULL;
	m_pActiveCountOver	= NULL;
	m_pActiveCountOn	= NULL;

	m_pActiveCountOnYellow = NULL;
	m_pActiveCountOnRed = NULL;

	m_pKeyBackNor = NULL;
	m_pKeyBackOn  = NULL;
	m_pKeyBack    = NULL;

	m_pEnableEffect1 = NULL;
	m_pEnableEffect2 = NULL;

	m_iCurBullet = 0;
	m_iMaxBullet = 0;
	m_iNeedBullet = 0;
	m_bExtraValue = false;

	m_iCurSecoundBullet = 0;
	m_iMaxSecoundBullet = 0;
	m_iNeedSecoundBullet = 0;

	m_iCurActiveCount = 0;
	m_iMaxActiveCount = 0;

	m_bAllGrayState = false;

	m_SlotInfo.m_fCurScaleRate = FLOAT1;
	m_SlotInfo.m_dwScaleStartTime = 0;	
	m_pSkillListWnd = NULL;

	m_wSkillKeyLeftValue  = 0;
	m_wSkillKeyCenterValue= 0;
	m_wSkillKeyRightValue = 0;

	InitSkillState();
	InitKeyState();

	//
	m_fMaxExtraGauge = 0.0f;
	m_fCurExtraGauge = 0.0f;

	m_pExcaliburRed = NULL;
	m_pExcaliburRedS = NULL;
	m_pExcaliburYellow = NULL;
	m_pExcaliburYellowS = NULL;

	m_pExtraGaugeBack = NULL;
	m_pExtraGaugeRed = NULL;
	m_pExtraGaugeYellow = NULL;

	m_SkillKeyInfoNor.m_SkillKey.Clear();
	m_SkillKeyInfoNor.m_TextColor = 0x000000;
	m_SkillKeyInfoNor.m_BkColor = 0x000000;
	m_SkillKeyInfoOn.m_SkillKey.Clear();
	m_SkillKeyInfoOn.m_TextColor = 0x000000;
	m_SkillKeyInfoOn.m_BkColor = 0x000000;
	
	//T50 메가 개틀링건(2012. 06. 20) : 스킬에 따라 텍스트 색 및 크기 등 분류
	m_ActiveCountType = DACT_ONE_TEXT;
	m_bRedUI = false;

	//Skill Time Print
	m_eCharState	= CS_DELAY;
	m_pSkill		= NULL;
	m_pOwnerChar	= NULL;
}

SkillListBtn::~SkillListBtn()
{
	SAFEDELETE( m_SlotInfo.pItemImg );
	SAFEDELETE( m_SlotInfo.pSkillImg );
	SAFEDELETE( m_SlotInfo.pBackImg );
	SAFEDELETE( m_SlotInfo.pTextImg );

	SAFEDELETE( m_pGrowthNum );
	SAFEDELETE( m_pGrowthPlus );

	SAFEDELETE( m_pBulletBack );
	SAFEDELETE( m_pBulletOver );
	SAFEDELETE( m_pGunOn );
	SAFEDELETE( m_pBeamOn );
	SAFEDELETE( m_pMineOn );
	SAFEDELETE( m_pGunOff );
	SAFEDELETE( m_pBeamOff );
	SAFEDELETE( m_pNumberOn );
	SAFEDELETE( m_pNumberOff );
	SAFEDELETE( m_pBeamNumberOn );
	SAFEDELETE( m_pMoonOn );
	SAFEDELETE( m_pMoonOff );
	SAFEDELETE( m_pHideOn );
	SAFEDELETE( m_pHideOff );
	SAFEDELETE( m_pSylphidOff );
	SAFEDELETE( m_pSylphidOn );
	SAFEDELETE( m_pAlterEgoOff );
	SAFEDELETE( m_pAlterEgoOn );
	SAFEDELETE( m_pWitchOn );
	SAFEDELETE( m_pWitchOff );
	SAFEDELETE( m_pMechanicsOn );
	SAFEDELETE( m_pMechanicsOff );

	SAFEDELETE( m_pActiveCountBack );
	SAFEDELETE( m_pActiveCountOver );
	SAFEDELETE( m_pActiveCountOn );

	SAFEDELETE( m_pActiveCountOnYellow );
	SAFEDELETE( m_pActiveCountOnRed );

	SAFEDELETE( m_pKeyBackNor );
	SAFEDELETE( m_pKeyBackOn );
	SAFEDELETE( m_pKeyBack );

	SAFEDELETE( m_pEnableEffect1 );
	SAFEDELETE( m_pEnableEffect2 );

	//
	SAFEDELETE( m_pExcaliburRed );
	SAFEDELETE( m_pExcaliburRedS );
	SAFEDELETE( m_pExcaliburYellow );
	SAFEDELETE( m_pExcaliburYellowS );

	SAFEDELETE( m_pExtraGaugeBack );
	SAFEDELETE( m_pExtraGaugeRed );
	SAFEDELETE( m_pExtraGaugeYellow );
}

void SkillListBtn::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "BackImage")
		m_SlotInfo.pBackImg = pImage;
	else if( szType == "BulletBack" )
		m_pBulletBack = pImage;
	else if( szType == "BulletOver" )
		m_pBulletOver = pImage;
	else if( szType == "GunOn" )
		m_pGunOn = pImage;
	else if( szType == "BeamOn" )
		m_pBeamOn = pImage;
	else if( szType == "MineOn" )
		m_pMineOn = pImage;
	else if( szType == "GunOff" )
		m_pGunOff = pImage;
	else if( szType == "BeamOff" )
		m_pBeamOff = pImage;
	else if( szType == "NumberOn" )
		m_pNumberOn = pImage;
	else if( szType == "NumberOff" )
		m_pNumberOff = pImage;
	else if( szType == "BeamNumberOn" )
		m_pBeamNumberOn = pImage;
	else if( szType == "MoonOn" )
		m_pMoonOn = pImage;
	else if( szType == "MoonOff" )
		m_pMoonOff = pImage;
	else if( szType == "HideOn" )
		m_pHideOn = pImage;
	else if( szType == "HideOff" )
		m_pHideOff = pImage;
	else if( szType == "SylphidOn" )
		m_pSylphidOn = pImage;
	else if( szType == "SylphidOff" )
		m_pSylphidOff = pImage;
	else if( szType == "AlterEgoOn" )
		m_pAlterEgoOn = pImage;
	else if( szType == "AlterEgoOff" )
		m_pAlterEgoOff = pImage;
	else if( szType == "WitchOn" )
		m_pWitchOn = pImage;
	else if( szType == "WitchOff" )
		m_pWitchOff = pImage;
	else if( szType == "MechanicsOn" )
		m_pMechanicsOn = pImage;
	else if( szType == "MechanicsOff" )
		m_pMechanicsOff = pImage;
	else if( szType == "ActiveCountBack" )
		m_pActiveCountBack = pImage;
	else if( szType == "ActiveCountOver" )
		m_pActiveCountOver = pImage;
	else if( szType == "ActiveCountOn" )
		m_pActiveCountOn = pImage;
	else if( szType == "ActiveCountOn_Yellow" )
		m_pActiveCountOnYellow = pImage;
	else if( szType == "ActiveCountOn_Red" )
		m_pActiveCountOnRed = pImage;

	else if( szType == "GrowthNumText" )
	{
		SAFEDELETE( m_pGrowthNum );
		m_pGrowthNum  = pImage;
	}
	else if( szType == "GrowthPlusText" )
	{
		SAFEDELETE( m_pGrowthPlus );
		m_pGrowthPlus  = pImage;
	}
	else if( szType == "EffectImg1" )
	{
		SAFEDELETE( m_pEnableEffect1 );
		m_pEnableEffect1  = pImage;
	}
	else if( szType == "EffectImg2" )
	{
		SAFEDELETE( m_pEnableEffect2 );
		m_pEnableEffect2  = pImage;
	}
	else if( szType == "ExcaliburRed" )
		m_pExcaliburRed = pImage;
	else if( szType == "ExcaliburYellow" )
		m_pExcaliburYellow = pImage;
	else if( szType == "ExcaliburRed_S" )
		m_pExcaliburRedS = pImage;
	else if( szType == "ExcaliburYellow_S" )
		m_pExcaliburYellowS = pImage;
	else if( szType == "ExtraGaugeBack" )
		m_pExtraGaugeBack = pImage;
	else if( szType == "ExtraGaugeRed" )
		m_pExtraGaugeRed = pImage;
	else if( szType == "ExtraGaugeYellow" )
		m_pExtraGaugeYellow = pImage;
	else
	{
		ioWnd::AddRenderImage( szType, pImage );
	}
}

// 용병교체UI 관련 작업. ( 2012.02.27 )
void SkillListBtn::AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame )
{
	if( szType == "KeyBackNor" )
		m_pKeyBackNor = pFrame;
	else if( szType == "KeyBackOn" )
		m_pKeyBackOn = pFrame;
	else if( szType == "KeyBack" )
		m_pKeyBack = pFrame;
	else
	{
		ioWnd::AddRenderFrame( szType, pFrame );
	}
}

void SkillListBtn::ParseExtraInfo( ioXMLElement &xElement )
{
	m_dwEffectTime1 = xElement.GetIntAttribute_e( "EffectTime1" );
	m_dwEffectTime2 = xElement.GetIntAttribute_e( "EffectTime2" );

	m_fEffectScale = xElement.GetFloatAttribute_e( "EffectScale" );

	m_szEffectSound = xElement.GetStringAttribute_e( "EffectSound" );
}

void SkillListBtn::SkillKeySet( SkillListBtn::SkillKeyInfo &rkSkillKeyInfo, WORD wKeySetLeftValue, WORD wKeySetCenterValue, WORD wKeySetRightValue, DWORD dwTextColor, DWORD dwBkColor )
{
	if( m_pSkillListWnd == NULL ) return;

	char szSkillKey[MAX_PATH] = "";

	if( wKeySetLeftValue > 0)
	{
		char szToken[MAX_PATH] = "";
		sprintf( szToken, "%s", g_KeyManager.GetKeyText( wKeySetLeftValue ) );
		szToken[1] = NULL;
		strcat( szSkillKey, szToken );
	}

	if( wKeySetCenterValue > 0)
	{
		char szToken[MAX_PATH] = "";
		sprintf( szToken, "%s", g_KeyManager.GetKeyText( wKeySetCenterValue ) );
		szToken[1] = NULL;
		strcat( szSkillKey, szToken );
	}

	if( wKeySetRightValue > 0)
	{
		char szToken[MAX_PATH] = "";
		sprintf( szToken, "%s", g_KeyManager.GetKeyText( wKeySetRightValue ) );
		szToken[1] = NULL;
		strcat( szSkillKey, szToken );
	}

	rkSkillKeyInfo.m_SkillKey = szSkillKey;
	rkSkillKeyInfo.m_TextColor = dwTextColor;
	rkSkillKeyInfo.m_BkColor = dwBkColor;
}

void SkillListBtn::UpdateBullet( int iCur, int iMax, int iNeed, BulletIconType eBulletIconType, bool bExtra )
{
	m_iCurBullet = iCur;
	m_iMaxBullet = iMax;
	m_iNeedBullet = iNeed;
	m_eBulletIconType = eBulletIconType;
	m_bExtraValue = bExtra;
}


void SkillListBtn::UpdateSecoundBullet( int iCur, int iMax, int iNeed, BulletIconType eBulletIconType )
{
	m_iCurSecoundBullet = iCur;
	m_iMaxSecoundBullet = iMax;
	m_iNeedSecoundBullet = iNeed;
}


void SkillListBtn::UpdateExtraGauge( float fCur, float fMax )
{
	m_fCurExtraGauge = fCur;
	m_fMaxExtraGauge = fMax;
}

void SkillListBtn::UpdateActiveCount( int iCur, int iMax, int iSlotType )
{
	m_iCurActiveCount = iCur;
	m_iMaxActiveCount = iMax;
}

void SkillListBtn::UpdateActiveCountType( DrawActiveCountType iType, bool bRedUI )
{
	m_ActiveCountType = iType;
	m_bRedUI = bRedUI;
}

void SkillListBtn::ResetActiveCount( int iSlotType )
{
	m_iCurActiveCount = 0;
	m_iMaxActiveCount = 0;
}

void SkillListBtn::UpdateGaugeInfo( int iMaxValue, int iCurValue, float fRecoveryValue, int iTic, int iExtendMax, bool bUse, bool bUsePassiveGauge /*= false*/ )
{
	if( bUse )
	{
		m_SlotInfo.m_iMaxGauge = iMaxValue;
		m_SlotInfo.m_iCurGauge = iCurValue;
		m_SlotInfo.m_fRecoveryGauge = fRecoveryValue;
		m_SlotInfo.m_iRecoveryTick = iTic;
		m_SlotInfo.m_iExtendMaxGauge = iExtendMax;
		m_SlotInfo.m_bUseGauge = true;
		
		if( m_SlotInfo.iSkillType == ST_PASSIVE && !bUsePassiveGauge )       //패시브 스킬은 즉시 발동.
		{
			m_SlotInfo.m_iCurGauge = iMaxValue;
		}
		else if( m_SlotInfo.iSkillType == ST_COUNT )
		{
			m_SlotInfo.m_iCurGauge = iMaxValue;
		}
	}
	else
	{
		m_SlotInfo.m_bUseGauge = false;
	}
}

void SkillListBtn::UpdateItemSlotInfo( ioBaseChar *pOwner )
{
	if( !pOwner )	return;

	m_bAllGrayState = false;
	
	CharState eState = pOwner->GetState();
	if( eState == CS_DIE ||
		eState == CS_READY ||
		eState == CS_ENDMOTION ||
		pOwner->GetChatModeState() != CMS_NONE )
	{
		m_bAllGrayState = true;
		if( m_SkillState.GetCurState() != SDT_NONE )
			m_SkillState.ChangeState(SDT_NONE);
	}

	ioItem *pItem = pOwner->GetEquipedItem( GetID() );
	if( !pItem )	return;

	ioHashString szIconName = pItem->GetItemLargeIconName();

	bool bExtendSkill = false;
	bool bItemHwIcon = false;
	char szNewName[MAX_PATH] = "";
	ioUIRenderImage *pItemImgHw = NULL;

	int iMax = pItem->GetMaxSkillGauge( pOwner->GetCharName() );
	int iExtendMax = pItem->GetMaxExtendSkillGauge( pOwner->GetCharName() );
	int iCur = pItem->GetCurSkillGuage();

	if( iExtendMax > 0 && COMPARE(iCur, iMax, iExtendMax) )
		bExtendSkill = true;

	if( bExtendSkill )
	{
		ioSkill *pSkill = pOwner->GetEquipedSkill( GetID() );
		if( pSkill )
		{
			szIconName = pSkill->GetIconName();
		}
	}
	else if( !szIconName.IsEmpty() && !pOwner->IsMale() )
	{
		StringCbPrintf( szNewName, sizeof(szNewName), "%s_hw", szIconName.c_str() );
		pItemImgHw = g_UIImageSetMgr.CreateImageByFullName( szNewName );

		if( pItemImgHw )
		{
			bItemHwIcon = true;
			szIconName = szNewName;
		}
	}

	if( m_SlotInfo.pItemImg )
	{
		if( m_SlotInfo.m_ItemIconName != szIconName )
		{
			SAFEDELETE( m_SlotInfo.pItemImg );

			m_SlotInfo.m_ItemIconName = szIconName;

			if( bItemHwIcon )
				m_SlotInfo.pItemImg = pItemImgHw;
			else
				m_SlotInfo.pItemImg = g_UIImageSetMgr.CreateImageByFullName( m_SlotInfo.m_ItemIconName );
		}
		else if( pItemImgHw )
		{
			SAFEDELETE( m_SlotInfo.pItemImg );
		}
	}
	else
	{
		m_SlotInfo.m_ItemIconName = szIconName;

		if( bItemHwIcon )
			m_SlotInfo.pItemImg = pItemImgHw;
		else
			m_SlotInfo.pItemImg = g_UIImageSetMgr.CreateImageByFullName( m_SlotInfo.m_ItemIconName );
	}

	m_SlotInfo.bEnableUpdateGauge = pItem->IsEnableUpdateSkillGauge(pOwner->GetCharName());
}

void SkillListBtn::UpdateSkillSlotInfo( ioBaseChar *pOwner )
{
	bool bPreUseScale = m_SlotInfo.bUseScale;

	m_SlotInfo.bUseScale = false;
	m_SlotInfo.bDisable = false;
	m_SlotInfo.bUseBuffButStopped = false;

	int iSlot = GetID();
	ioSkill *pSkill = pOwner->GetEquipedSkill( iSlot );

	m_pSkill = pSkill;
	m_pOwnerChar = pOwner;
	if( !pSkill )
	{
		SAFEDELETE( m_SlotInfo.pSkillImg );

		m_SlotInfo.bSlotGray = false;
		m_SlotInfo.m_fCurScaleRate = FLOAT1;
		m_SlotInfo.m_dwScaleStartTime = 0;
	
		if( m_SlotInfo.pBackImg )
		{
			m_SlotInfo.pBackImg->SetScale( FLOAT1 );
		}

		return;
	}

	ioHashString szIconName = pSkill->GetIconName();
	bool bExtendSkill = false;
	ioItem *pItem = pOwner->GetEquipedItem( GetID() );
	if( pItem )
	{
		int iMax = pItem->GetMaxSkillGauge( pOwner->GetCharName() );
		int iExtendMax = pItem->GetMaxExtendSkillGauge( pOwner->GetCharName() );
		int iCur = pItem->GetCurSkillGuage();

		if( iExtendMax > 0 && COMPARE(iCur, iMax, iExtendMax) )
		{
			bExtendSkill = true;
			ioHashString szExtendSkill;
			pItem->GetExtendSkillName( szExtendSkill );
			const ioSkill *pExtendSkill = g_SkillMaker.GetSkillConst( szExtendSkill );
			if( pExtendSkill )
			{
				szIconName = pExtendSkill->GetIconName();
			}
		}
	}

	m_SlotInfo.iSkillType = pSkill->GetTypeID();

	if( m_SlotInfo.iSkillType == ST_BUFF )
	{
		m_SlotInfo.bOnOffEnable = pSkill->IsOnOffEnable();
	}
	else
	{
		m_SlotInfo.bOnOffEnable = false;
	}

	// 버프.
	m_SlotInfo.bUseBuff = pSkill->CheckUseBuff( pOwner );

	bool bDownUsing = false;
	bool bJumppingUsing = false;
	CharState eState = (CharState)pOwner->GetState();
	SkillUseType eUseType = SUT_NORMAL;

	// down check
	if( eState == CS_BLOW_WOUNDED || eState == CS_BOUND_BLOW_WOUNDED )
	{
		if( pOwner->GetBlowWoundedState() == BW_BLOW_ENDED )
			eUseType = SUT_DOWN;
	}
	else if( eState == CS_BLOW_DASH_ATTACK && pOwner->GetBlowDashState() == BDS_DOWN )
	{
		eUseType = SUT_DOWN;
	}
	else if( eState == CS_JUMP && pOwner->GetJumpState() == JS_JUMPPING )
	{
		eUseType = SUT_JUMP;
	}

	if( !pOwner->GetUseEnableSkill(iSlot, eUseType) || m_bAllGrayState )
	{
		if( pOwner->CheckDisableSkillByMode( pSkill->GetName() ) )
		{
			m_SlotInfo.bDisable = true;
			m_SlotInfo.bSlotGray = true;
		}
		else if( !pSkill->IsCanUseSkillWithCrown( pOwner ) )
		{
			m_SlotInfo.bDisable = true;
			m_SlotInfo.bSlotGray = true;
		}
		else if( !pSkill->IsCanUseByWeaponType( pOwner ) )
		{
			m_SlotInfo.bDisable = true;
			m_SlotInfo.bSlotGray = true;
		}
		else if( !pSkill->IsCanUseBoss( pOwner ) )
		{
			m_SlotInfo.bDisable = true;
			m_SlotInfo.bSlotGray = true;
		}
		else if( !pSkill->IsCanUseFootball() )
		{
			m_SlotInfo.bDisable = true;
			m_SlotInfo.bSlotGray = true;
		}
		else if( !pSkill->IsCanUseGangSiMode() )
		{
			m_SlotInfo.bDisable = true;
			m_SlotInfo.bSlotGray = true;
		}
		else if( !pSkill->IsCanUseZeroHP( pOwner ) )
		{
			if( m_SlotInfo.bUseBuff )
			{
				m_SlotInfo.bSlotGray = false;
				m_SlotInfo.bUseScale = false;
			}
			else
			{
				m_SlotInfo.bDisable = true;
				m_SlotInfo.bSlotGray = true;
			}
		}
		else if( pSkill->IsCanSwitchSkill() && pOwner->IsCurUsingSkill( pSkill->GetName() ) )
		{
			m_SlotInfo.bSlotGray = false;
		}
		else
		{
			m_SlotInfo.bSlotGray = true;
		}
	}
	else
	{
		m_SlotInfo.bSlotGray = false;
		m_SlotInfo.bUseScale = !m_SlotInfo.bUseBuff;
		m_SlotInfo.bUseBuffButStopped = pOwner->IsSkillBuffLiveButStopped( pSkill->GetName() );

		if( !bPreUseScale && m_SlotInfo.bUseScale && m_SlotInfo.iSkillType != ST_PASSIVE )
		{
			SetEnableEffect( true );
		}
	}

	char szBuf[MAX_PATH] = "";

	if( m_SlotInfo.pSkillImg )
	{
		if( m_SlotInfo.m_SkillIconName != szIconName )
		{
			SAFEDELETE( m_SlotInfo.pSkillImg );
			SAFEDELETE( m_SlotInfo.pTextImg );

			m_SlotInfo.m_SkillIconName = szIconName;
			m_SlotInfo.m_SkillTextImage = pSkill->GetSkillTextImage();

			m_SlotInfo.pSkillImg = g_UIImageSetMgr.CreateImageByFullName( m_SlotInfo.m_SkillIconName );
			//m_SlotInfo.pTextImg = g_UIImageSetMgr.CreateImageByFullName( m_SlotInfo.m_SkillTextImage );
		}
	}
	else
	{
		m_SlotInfo.m_SkillIconName = szIconName;
		m_SlotInfo.m_SkillTextImage = pSkill->GetSkillTextImage();

		m_SlotInfo.pSkillImg = g_UIImageSetMgr.CreateImageByFullName( m_SlotInfo.m_SkillIconName );

		if( m_SlotInfo.pTextImg )
			SAFEDELETE( m_SlotInfo.pTextImg );

		//m_SlotInfo.pTextImg = g_UIImageSetMgr.CreateImageByFullName( m_SlotInfo.m_SkillTextImage );
	}

	m_SlotInfo.m_SkillDesc = pSkill->GetDescName();
	m_SlotInfo.m_iGrowthLevel = pSkill->GetGrowthLevel(pOwner->GetCharName());
	m_SlotInfo.m_iReinforce = pSkill->GetGrowthExtraLevel();

	m_SlotInfo.m_iMedalLevel = 0;
	m_SlotInfo.m_nPetLevel = 0;
	m_SlotInfo.m_nAwakeLevel = 0;
	m_SlotInfo.m_iRCharLevel = 0;
	m_SlotInfo.m_iSoldierSetLevel = 0;

	int iClassType = g_MyInfo.GetClassType();
	IntVec vMedalItemTypeVec;
	g_MyInfo.GetMedalItemTypeVec( vMedalItemTypeVec, iClassType );

	int iSlotIndex = GetID();

	int iMedalSize = vMedalItemTypeVec.size();
	for (int i = 0; i < iMedalSize ; i++)
		m_SlotInfo.m_iMedalLevel += g_MedalItemMgr.GetMedalItemGrowth( vMedalItemTypeVec[i], false, iSlotIndex );
	
	//펫 성장
	PetSlot sPetSlot;
	ioUserPet *pUserPet = g_MyInfo.GetUserPet();
	if ( pUserPet )
		pUserPet->GetPetSlotEquip( sPetSlot );

	if ( sPetSlot.m_nPetCode > 0)
		m_SlotInfo.m_nPetLevel = g_PetInfoMgr.GetTotalStat( sPetSlot.m_nPetCode, iSlotIndex + MAX_ITEM_GROWTH, sPetSlot.m_nCurLevel );
		
	m_SlotInfo.m_nAwakeLevel += g_AwakeMgr.GetAwakeItemGrowth( static_cast<AwakeType>( pOwner->GetAwakeType() ), iSlotIndex );

	if( m_SlotInfo.iSkillType != ST_PASSIVE && m_SlotInfo.iSkillType != ST_COUNT )
	{
		m_SlotInfo.ProcessScale();
	}

	const CHARACTER& rkCharInfo = pOwner->GetCharacterInfo();
	if( g_RSoldierInfoManager.IsRSoldierType( rkCharInfo ) )
		m_SlotInfo.m_iRCharLevel += g_RSoldierInfoManager.GetSoldierAddStat( iSlotIndex+MAX_ITEM_GROWTH, g_MyInfo.GetRSoldierPossessionCount() );

	if( g_RSoldierInfoManager.IsSoldierSet( rkCharInfo ) )
		m_SlotInfo.m_iSoldierSetLevel += g_RSoldierInfoManager.GetSoldierSetAddStat( iSlotIndex+MAX_ITEM_GROWTH, rkCharInfo.m_class_type, g_MyInfo.GetSoldierSetCnt( g_RSoldierInfoManager.GetSoldierSetNum( rkCharInfo.m_class_type ) ) );
}

void SkillListBtn::UpdateSkillKeySet()
{
	int  iSlot = GetID();
	bool bChangeKey = false;
	ioUserKeyInput::UseKeyValue kKeys;
	kKeys.SetDefaultFirstKey();
	g_KeyManager.GetGameKey( kKeys );

	switch( iSlot )
	{
	case ES_WEAPON:        // -SD
		{
			if ( kKeys.m_wWeaponSkillKey != KEY_NULL )
			{
				if ( m_wSkillKeyCenterValue != kKeys.m_wWeaponSkillKey )
				{
					m_wSkillKeyLeftValue   = 0;
					m_wSkillKeyCenterValue = kKeys.m_wWeaponSkillKey;
					m_wSkillKeyRightValue  = 0;
					bChangeKey = true;
				}
			}
			else
			{
				if( m_wSkillKeyCenterValue != kKeys.m_wDefenseKey ||
					m_wSkillKeyRightValue != kKeys.m_wAttackKey )
				{				
					m_wSkillKeyLeftValue   = 0;
					m_wSkillKeyCenterValue = kKeys.m_wDefenseKey;
					m_wSkillKeyRightValue  = kKeys.m_wAttackKey;
					bChangeKey = true;
				}
			}
		}
		break;
	case ES_ARMOR:         // ASD
		{
			if ( kKeys.m_wArmorSkillKey != KEY_NULL )
			{
				if ( m_wSkillKeyCenterValue != kKeys.m_wArmorSkillKey )
				{
					m_wSkillKeyLeftValue   = 0;
					m_wSkillKeyCenterValue = kKeys.m_wArmorSkillKey;
					m_wSkillKeyRightValue  = 0;
					bChangeKey = true;
				}
			}
			else
			{
				if( m_wSkillKeyLeftValue  != kKeys.m_wJumpKey ||
					m_wSkillKeyCenterValue != kKeys.m_wDefenseKey ||
					m_wSkillKeyRightValue != kKeys.m_wAttackKey )
				{				
					m_wSkillKeyLeftValue   = kKeys.m_wJumpKey;
					m_wSkillKeyCenterValue = kKeys.m_wDefenseKey;
					m_wSkillKeyRightValue  = kKeys.m_wAttackKey;
					bChangeKey = true;
				}
			}
		}
		break;
	case ES_HELMET:        // A-D
		{
			if ( kKeys.m_wArmorSkillKey != KEY_NULL )
			{
				if ( m_wSkillKeyCenterValue != kKeys.m_wHelmetSkillKey )
				{
					m_wSkillKeyLeftValue   = 0;
					m_wSkillKeyCenterValue = kKeys.m_wHelmetSkillKey;
					m_wSkillKeyRightValue  = 0;
					bChangeKey = true;
				}
			}
			else
			{
				if( m_wSkillKeyLeftValue  != kKeys.m_wJumpKey ||				
					m_wSkillKeyRightValue != kKeys.m_wAttackKey )
				{				
					m_wSkillKeyLeftValue   = kKeys.m_wJumpKey;
					m_wSkillKeyCenterValue = 0;
					m_wSkillKeyRightValue  = kKeys.m_wAttackKey;
					bChangeKey = true;
				}
			}
		}
		break;
	case ES_CLOAK:         // AS-
		{
			if ( kKeys.m_wArmorSkillKey != KEY_NULL )
			{
				if ( m_wSkillKeyCenterValue != kKeys.m_wCloakSkillKey )
				{
					m_wSkillKeyLeftValue   = 0;
					m_wSkillKeyCenterValue = kKeys.m_wCloakSkillKey;
					m_wSkillKeyRightValue  = 0;
					bChangeKey = true;
				}
			}
			else
			{
				if( m_wSkillKeyLeftValue  != kKeys.m_wJumpKey ||
					m_wSkillKeyCenterValue != kKeys.m_wDefenseKey )
				{				
					m_wSkillKeyLeftValue   = kKeys.m_wJumpKey;
					m_wSkillKeyCenterValue = kKeys.m_wDefenseKey;
					m_wSkillKeyRightValue  = 0;
					bChangeKey = true;
				}
			}
		}
		break;
	}

	if( bChangeKey )
	{ 
		SkillKeySet( m_SkillKeyInfoNor, m_wSkillKeyLeftValue, m_wSkillKeyCenterValue, m_wSkillKeyRightValue, TCT_DEFAULT_WHITE, 0xFF133587 );
		SkillKeySet( m_SkillKeyInfoOn, m_wSkillKeyLeftValue, m_wSkillKeyCenterValue, m_wSkillKeyRightValue, TCT_DEFAULT_WHITE, 0xFF133587 );
	}
}

int SkillListBtn::GetRenderImageStartHeight( ioUIRenderImage *pImage, int iSrcHeight, int iCur, int iMax )
{
	if( iMax == iCur ) return 0;
	if( !pImage )	return 0;

	float fGap = (float)iCur / (float)iMax;
	float fGapHeight = (float)iSrcHeight * fGap;
	int iOffYPos = pImage->GetOffsetY();
	int iHeight  = pImage->GetHeight();

	return iOffYPos + ( (iSrcHeight / 2) - fGapHeight );							
}

void SkillListBtn::OnRender()
{
	if( m_pSkillListWnd )
	{
		if( m_pSkillListWnd->SkipRender() ) return;
	}

	ioWnd::OnRender();

	int iXPos, iYPos;
	iXPos = GetDerivedPosX();
	iYPos = GetDerivedPosY();

	enum { SLOT_XOFFSET = 28, SLOT_YOFFSET = 27, 
		   INPUT_XOFFSET = 13, INPUT_YOFFSET = 54,
		   INPUTBACK_XOFFSET = 12, INPUTBACK_YOFFSET = 53,
	       BULLETBACK_XOFFSET = 46 , BULLETBACK_YOFFSET = -18,	
		   SECOUND_BULLET_XOFFSET = 25,
		};

	RenderNeedBulletBack( iXPos + BULLETBACK_XOFFSET, iYPos + BULLETBACK_YOFFSET );

	m_KeyState.UpdateTime();
	m_SkillState.UpdateTime();

	bool bDraw = RenderUseSkillEffect( iXPos + SLOT_XOFFSET, iYPos  + SLOT_YOFFSET);
	if( !bDraw )
		RenderSkillSlot( iXPos + SLOT_XOFFSET, iYPos + SLOT_YOFFSET );

	RenderInputKeyBack( iXPos, iYPos );
	RenderInputKey( iXPos, iYPos );
	
	//////////////////////////////////////////////////////////////////////////
	RenderExtraGauge2( iXPos, iYPos );
	RenderNeedBullet2( iXPos, iYPos );
	//////////////////////////////////////////////////////////////////////////
	RenderExtraGauge3( iXPos + SECOUND_BULLET_XOFFSET, iYPos );
	RenderNeedBullet3( iXPos + SECOUND_BULLET_XOFFSET , iYPos );
	//////////////////////////////////////////////////////////////////////////
	
	RenderActiveCount( iXPos, iYPos );

	RenderEnableEffect( iXPos+SLOT_XOFFSET, iYPos+SLOT_YOFFSET );
}

void SkillListBtn::RenderSkillSlot( int xPos, int yPos )
{
	if( m_SlotInfo.bSlotGray )
	{
		m_KeyState.ChangeState( KET_NONE );

		if( m_SlotInfo.iSkillType == ST_COUNT )
		{
			m_SlotInfo.pItemImg->Render( xPos, yPos, UI_RENDER_GRAY );
		}
		else if( !m_SlotInfo.bOnOffEnable && m_SlotInfo.IsNeedBuffGaugeProtect() )
		{
			if( m_SkillState.ChangeState( SDT_USING ) == SDT_EFFECT_WHITE )
				return;

			m_SlotInfo.pSkillImg->Render( xPos, yPos );
		}
		else if( m_SlotInfo.pItemImg )
		{
			if( m_SkillState.ChangeState( SDT_WAIT ) == SDT_EFFECT_WHITE_ITEM )
				return;

			m_SlotInfo.pItemImg->Render( xPos, yPos, UI_RENDER_GRAY );

			if( !m_bAllGrayState && !m_SlotInfo.bDisable )
			{
				if( m_SlotInfo.m_bUseGauge && (m_SlotInfo.bEnableUpdateGauge || m_SlotInfo.bUseBuff) )
				{
					if( m_SlotInfo.m_iCurGauge < m_SlotInfo.m_iMaxGauge )
					{
						RenderGaugeIcon( xPos,
										 yPos,
										 m_SlotInfo.pItemImg,
										 ICON_HEIGHT,
										 m_SlotInfo.m_iCurGauge,
										 m_SlotInfo.m_iMaxGauge );
#ifdef SKILLCOOL_COUNT
						if( m_pSkill != NULL && m_SlotInfo.m_fRecoveryGauge != 0 )
						{
							g_FontMgr.SetAlignType( TAT_CENTER );
							g_FontMgr.SetVertAlign( TVA_TOP );
							g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
							g_FontMgr.SetBkColor( TCT_DEFAULT_BLACK );
							//g_FontMgr.SetTextColor( TCT_DEFAULT_IVORY );
							bool bMageType = false;
							 float fRecoveryGauge = m_SlotInfo.m_fRecoveryGauge;
							if( abs( fRecoveryGauge ) > 0 ) //메이지 계열 skill_gauge_rate(차징)때매
							{
								bMageType = true;
								fRecoveryGauge = 0.2f;
							}

							int iRecoveryTime = (int)( ( m_SlotInfo.m_iMaxGauge - m_SlotInfo.m_iCurGauge ) / fRecoveryGauge );
							if( m_SlotInfo.m_iMaxGauge - m_SlotInfo.m_iCurGauge > m_SlotInfo.m_fRecoveryGauge * iRecoveryTime )
								iRecoveryTime++;

							int iRecoverySec = iRecoveryTime * m_SlotInfo.m_iRecoveryTick / 1000;
							if( iRecoveryTime * m_SlotInfo.m_iRecoveryTick % 1000 != 0 )
							{
								iRecoverySec++;
							}
							ioItem *pItem = m_pSkill->GetOwnerItem();
							if( pItem == NULL )
								return;
							if( m_pSkill->GetTypeID() == SkillType::ST_BUFF )
							{
								int iBufType = m_pSkill->GetBuffType();
								ioBuff *pBuff = m_pOwnerChar->GetBuff( iBufType );
								//버프 상태일 경우 카운팅 안함
								if( pBuff || iBufType <= 0 )
								{
									return;
								}
							}
							if( pItem != NULL )
							{
								if( pItem->GetSkillGaugeType() != ioItem::GT_DAMAGE )
								{
									if( m_eCharState != CS_DELAY && m_eCharState != CS_SPECIAL_MOTION
										&& m_eCharState != CS_RUN )
									{
										if( !bMageType )
											iRecoverySec /= 2;
										g_FontMgr.SetTextColor( TCT_DEFAULT_LIGHTRED );
									}
									else
										g_FontMgr.SetTextColor( TCT_DEFAULT_IVORY );

									// 아서 갑옷류는 시간을 출력하지 않는다.
									char szGrowth[MAX_PATH];
									wsprintf( szGrowth, "%d", iRecoverySec );
									if( iRecoverySec > 0 )
									{
										g_FontMgr.PrintText( xPos, yPos - 14, FONT_SIZE_20, szGrowth );
									}
								}
							}
						}
#endif
					}
					else
					{
						m_SlotInfo.pItemImg->SetScale( FLOAT1 );
						m_SlotInfo.pItemImg->Render( xPos, yPos );
					}
				}
				else
				{
					RenderGaugeIcon( xPos,
									 yPos,
									 m_SlotInfo.pItemImg,
									 ICON_HEIGHT,
									 m_SlotInfo.m_iCurGauge,
									 m_SlotInfo.m_iMaxGauge,
									 false );
				}
			}
		}
		else
		{
			if( m_SlotInfo.pBackImg )
				m_SlotInfo.pBackImg->Render( xPos, yPos );
		}
	}
	else
	{
		if( m_SlotInfo.pSkillImg )
		{
			if( !m_SlotInfo.bOnOffEnable && m_SlotInfo.IsNeedBuffGaugeProtect() )
			{
				m_KeyState.ChangeState( KET_NONE );
				if( m_SkillState.ChangeState( SDT_USING ) == SDT_EFFECT_WHITE )
					return ;

				m_SlotInfo.pSkillImg->Render( xPos, yPos, UI_RENDER_NORMAL, TFO_BILINEAR );
			}
			else if( m_SlotInfo.m_iCurGauge < m_SlotInfo.m_iMaxGauge )
			{
				m_KeyState.ChangeState( KET_NONE );
				if( m_SkillState.ChangeState( SDT_USING ) == SDT_EFFECT_WHITE )
					return ;

				RenderGaugeIcon( xPos, yPos, m_SlotInfo.pSkillImg, ICON_HEIGHT, m_SlotInfo.m_iCurGauge, m_SlotInfo.m_iMaxGauge );
				
				if( m_SlotInfo.iSkillType == ST_PASSIVE )
				{
					int iGrowthLevel = m_SlotInfo.m_iGrowthLevel + m_SlotInfo.m_iReinforce + m_SlotInfo.m_iMedalLevel + m_SlotInfo.m_nPetLevel + m_SlotInfo.m_nAwakeLevel + m_SlotInfo.m_iRCharLevel + m_SlotInfo.m_iSoldierSetLevel;
					iGrowthLevel = max( 0, iGrowthLevel );

					g_FontMgr.SetAlignType( TAT_CENTER );
					g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
					g_FontMgr.SetBkColor( 0, 0, 0 );
					g_FontMgr.SetTextColor( 255, 255, 255 );

					// 용병교체UI 관련 작업. ( 2012.02.27 )
					g_FontMgr.PrintText( xPos, yPos - 45.0f, FONT_SIZE_12, m_SlotInfo.m_SkillDesc.c_str() );
					char szGrowth[MAX_PATH];
					wsprintf( szGrowth, "+%d", iGrowthLevel );
					if( iGrowthLevel > 0 )
						g_FontMgr.PrintText( xPos, yPos - 58.0f, FONT_SIZE_12, szGrowth );
				}
			}
			else if( m_SlotInfo.m_iExtendMaxGauge > 0 && COMPARE(m_SlotInfo.m_iCurGauge, m_SlotInfo.m_iMaxGauge, m_SlotInfo.m_iExtendMaxGauge) )
			{
				m_KeyState.ChangeState( KET_NONE );
				if( m_SkillState.ChangeState( SDT_USING ) == SDT_EFFECT_WHITE )
					return ;

				int iMax = m_SlotInfo.m_iExtendMaxGauge - m_SlotInfo.m_iMaxGauge;
				int iCur = m_SlotInfo.m_iCurGauge - m_SlotInfo.m_iMaxGauge;

				RenderGaugeIcon2( xPos, yPos, m_SlotInfo.pItemImg, m_SlotInfo.pSkillImg, ICON_HEIGHT, iCur, iMax );
				
				int iGrowthLevel = m_SlotInfo.m_iGrowthLevel + m_SlotInfo.m_iReinforce + m_SlotInfo.m_iMedalLevel + m_SlotInfo.m_nPetLevel + m_SlotInfo.m_nAwakeLevel + m_SlotInfo.m_iRCharLevel + m_SlotInfo.m_iSoldierSetLevel;
				iGrowthLevel = max( 0, iGrowthLevel );

				g_FontMgr.SetAlignType( TAT_CENTER );
				g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
				g_FontMgr.SetBkColor( 0, 0, 0 );
				g_FontMgr.SetTextColor( 255, 255, 255 );

				// 용병교체UI 관련 작업. ( 2012.02.27 )
				g_FontMgr.PrintText( xPos, yPos - 45.0f, FONT_SIZE_12, m_SlotInfo.m_SkillDesc.c_str() );
				char szGrowth[MAX_PATH];
				wsprintf( szGrowth, "+%d", iGrowthLevel );
				if( iGrowthLevel > 0 )
					g_FontMgr.PrintText( xPos, yPos - 58.0f, FONT_SIZE_12, szGrowth );
			}
			else
			{
				if( m_SlotInfo.bUseBuffButStopped )
				{
					m_KeyState.ChangeState( KET_NONE );
					m_SlotInfo.pSkillImg->Render( xPos, yPos, UI_RENDER_GRAY, TFO_BILINEAR );
				}
				else
				{
					m_SkillState.ChangeState( SDT_CAN );
					m_KeyState.ChangeState( KET_CAN_SKILL );
					m_SlotInfo.pSkillImg->Render( xPos, yPos, UI_RENDER_NORMAL, TFO_BILINEAR );

					if( !m_SlotInfo.bSlotGray && !m_SlotInfo.m_SkillDesc.IsEmpty() )
					{
						int iGrowthLevel = m_SlotInfo.m_iGrowthLevel + m_SlotInfo.m_iReinforce + m_SlotInfo.m_iMedalLevel +m_SlotInfo.m_nPetLevel + m_SlotInfo.m_nAwakeLevel + m_SlotInfo.m_iRCharLevel + m_SlotInfo.m_iSoldierSetLevel;
						iGrowthLevel = max( 0, iGrowthLevel );

						if( m_SlotInfo.iSkillType == ST_PASSIVE )
						{
							g_FontMgr.SetAlignType( TAT_CENTER );
							g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
							g_FontMgr.SetBkColor( 0, 0, 0 );
							g_FontMgr.SetTextColor( 255, 255, 255 );

							// 용병교체UI 관련 작업. ( 2012.02.27 )
							g_FontMgr.PrintText( xPos, yPos - 45.0f, FONT_SIZE_12, m_SlotInfo.m_SkillDesc.c_str() );
							char szGrowth[MAX_PATH];
							wsprintf( szGrowth, "+%d", iGrowthLevel );
							if( iGrowthLevel > 0 )
								g_FontMgr.PrintText( xPos, yPos - 58.0f, FONT_SIZE_12, szGrowth );
						}
						else
						{
							if( m_SlotInfo.bUseScale )
							{
								g_FontMgr.SetAlignType( TAT_CENTER );
								g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
								g_FontMgr.SetBkColor( 0, 0, 0 );
								g_FontMgr.SetTextColor( 255, 255, 255 );

								// 용병교체UI 관련 작업. ( 2012.02.27 )
								g_FontMgr.PrintText( xPos, yPos - 45.0f, FONT_SIZE_12, m_SlotInfo.m_SkillDesc.c_str() );
								char szGrowth[MAX_PATH];
								wsprintf( szGrowth, "+%d", iGrowthLevel );
								if( iGrowthLevel > 0 )
									g_FontMgr.PrintText( xPos, yPos - 58.0f, FONT_SIZE_12, szGrowth );
							}
						}
					}
				}
			}
		}
		else
		{
			m_KeyState.ChangeState( KET_NONE );
			if( m_SlotInfo.pBackImg )
				m_SlotInfo.pBackImg->Render( xPos, yPos, UI_RENDER_NORMAL, TFO_BILINEAR );
		}
	}
}

void SkillListBtn::RenderGrowthLevel( int xPos, int yPos, int iGrowthLevel )
{
	if( !m_pGrowthPlus || !m_pGrowthNum )
		return;

	enum
	{
		PLUS_SIZE	= 7,
		
		FIRST_SIZE	= 8,
		SECOND_SIZE	= 15,
		THIRD_SIZE	= 22,
	};

	// Level Num
	int iLevelXPos = xPos;
	int iLevelYPos = yPos - 50.0f;

	float fNewXPos = 0.0f;
	if( iGrowthLevel < 10 )
	{
		fNewXPos = iLevelXPos - (PLUS_SIZE+FIRST_SIZE)/2;
		m_pGrowthPlus->Render( fNewXPos, iLevelYPos );

		fNewXPos += PLUS_SIZE;
		m_pGrowthNum->RenderNum( fNewXPos, iLevelYPos, iGrowthLevel );
	}
	else if( iGrowthLevel < FLOAT100 )
	{
		fNewXPos = iLevelXPos - (PLUS_SIZE+SECOND_SIZE)/2;
		m_pGrowthPlus->Render( fNewXPos, iLevelYPos );

		fNewXPos += PLUS_SIZE;
		m_pGrowthNum->RenderNum( fNewXPos, iLevelYPos, iGrowthLevel, -FLOAT1 );
	}
	else
	{
		fNewXPos = iLevelXPos - (PLUS_SIZE+THIRD_SIZE)/2;
		m_pGrowthPlus->Render( fNewXPos, iLevelYPos );

		fNewXPos += PLUS_SIZE;
		m_pGrowthNum->RenderNum( fNewXPos, iLevelYPos, iGrowthLevel, -FLOAT1 );
	}
}

void SkillListBtn::RenderGaugeIcon( int xPos, int yPos, ioUIRenderImage *pImage, int iSrcHeight, int iCur, int iMax, bool bEnableUpdate )
{
	int iRealHeight = GetRenderImageStartHeight( pImage, iSrcHeight, iCur, iMax );

	if( bEnableUpdate )
	{
		pImage->Render( xPos, yPos, UI_RENDER_GRAY );
		pImage->RenderHeightCut( xPos, yPos, iRealHeight, pImage->GetHeight() );
	}
	else
	{
		pImage->SetColor( 150, 150, 150 );
		pImage->RenderHeightCut( xPos, yPos, iRealHeight, pImage->GetHeight(), UI_RENDER_GRAY );
		pImage->SetColor( 255, 255, 255 );
	}

	if( iRealHeight + 2 <= pImage->GetHeight() )
	{
		pImage->SetColor( 0, 0, 0 );
		pImage->RenderHeightCut( xPos, yPos, iRealHeight, iRealHeight + 2 );
		pImage->SetColor( 255, 255, 255 );
	}
}

void SkillListBtn::RenderGaugeIcon2( int xPos, int yPos,
									 ioUIRenderImage *pBaseImage, ioUIRenderImage *pOverImage,
									 int iSrcHeight, int iCur, int iMax, bool bEnableUpdate )
{
	int iRealHeight = GetRenderImageStartHeight( pBaseImage, iSrcHeight, iCur, iMax );

	if( bEnableUpdate )
	{
		pBaseImage->Render( xPos, yPos );
		pOverImage->RenderHeightCut( xPos, yPos, iRealHeight, pOverImage->GetHeight() );
	}
	else
	{
		pBaseImage->SetColor( 150, 150, 150 );
		pOverImage->RenderHeightCut( xPos, yPos, iRealHeight, pOverImage->GetHeight(), UI_RENDER_GRAY );
		pOverImage->SetColor( 255, 255, 255 );
	}

	if( iRealHeight + 2 <= pBaseImage->GetHeight() )
	{
		pBaseImage->SetColor( 0, 0, 0 );
		pBaseImage->RenderHeightCut( xPos, yPos, iRealHeight, iRealHeight + 2 );
		pBaseImage->SetColor( 255, 255, 255 );
	}
}

void SkillListBtn::RenderInputKeyBack( int xPos, int yPos )
{
	if( !m_pKeyBack ) return;
	if( !m_SlotInfo.IsNeedKeyRender() ) return;

	m_pKeyBack->Render( xPos, yPos);
}


void SkillListBtn::RenderInputKey( int xPos, int yPos )
{
	if( !m_pKeyBackNor || !m_pKeyBackOn ) return;
	if( !m_SlotInfo.IsNeedKeyRender() ) return;

	if( m_KeyState.GetCurState() == KET_NONE )
	{
		m_pKeyBackNor->Render(xPos, yPos);
		g_FontMgr.SetAlignType( TAT_CENTER );
		g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
		g_FontMgr.SetBkColor( m_SkillKeyInfoNor.m_BkColor );
		g_FontMgr.SetTextColor( m_SkillKeyInfoNor.m_TextColor );
		g_FontMgr.PrintText( xPos+28, yPos+53, FONT_SIZE_11, m_SkillKeyInfoNor.m_SkillKey.c_str() );
	}
	else
	{
		m_pKeyBackOn->Render(xPos, yPos);
		g_FontMgr.SetAlignType( TAT_CENTER );
		g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
		g_FontMgr.SetBkColor( m_SkillKeyInfoOn.m_BkColor );
		g_FontMgr.SetTextColor( m_SkillKeyInfoOn.m_TextColor );
		g_FontMgr.PrintText( xPos+28, yPos+53, FONT_SIZE_11, m_SkillKeyInfoOn.m_SkillKey.c_str() );
	}
}

void SkillListBtn::RenderNeedBulletBack( int xPos, int yPos )
{
	if( m_iMaxBullet == 0 ) return;
	if( m_eBulletIconType == BIT_CHARGING )
		return;

	if( !m_pBulletBack ) 
		return;

	m_pBulletBack->Render( xPos, yPos);
}

void SkillListBtn::RenderActiveCount( int xPos, int yPos )
{
	if( !m_pActiveCountBack )
		m_pActiveCountBack->Render( xPos + 46, yPos - 18 );

	if( !m_pActiveCountOn && !m_pActiveCountOver && !m_pActiveCountOnRed && !m_pActiveCountOnYellow )
		return;

	if( m_iMaxActiveCount <= 0 )
		return;

	enum 
	{
		OVER1_XOFFSET = 52, OVER1_YOFFSET = -17,
		NUMBER1_XOFFSET = 64, NUMBER1_YOFFSET = -15,

		OVER2_XOFFSET = 52, OVER2_YOFFSET = -17,
		NUMBER2_XOFFSET = 61, NUMBER2_YOFFSET = -14,

		OVER3_XOFFSET = -22, OVER3_YOFFSET = -19,
		NUMBER3_XOFFSET = -13, NUMBER3_YOFFSET = -16,
	};

	switch( m_ActiveCountType )
	{
	case DACT_ONE_TEXT:
		{
			m_pActiveCountOver->Render( xPos + OVER1_XOFFSET, yPos + OVER1_YOFFSET );

			m_pActiveCountOn->SetHorzFormatting( ioUIRenderImage::HF_CENTER );
			m_pActiveCountOn->RenderNum( xPos + NUMBER1_XOFFSET, yPos + NUMBER1_YOFFSET, m_iCurActiveCount, FLOAT1 );
		}
		break;
	case DACT_TWO_TEXT:
		{
			m_pActiveCountOver->Render( xPos + OVER2_XOFFSET, yPos + OVER2_YOFFSET );

			if( m_bRedUI )
			{
				m_pActiveCountOnRed->SetHorzFormatting( ioUIRenderImage::HF_CENTER );
				m_pActiveCountOnRed->RenderNum( xPos + NUMBER2_XOFFSET, yPos + NUMBER2_YOFFSET, m_iCurActiveCount, -4.0f );
			}
			else
			{
				m_pActiveCountOnYellow->SetHorzFormatting( ioUIRenderImage::HF_CENTER );
				m_pActiveCountOnYellow->RenderNum( xPos + NUMBER2_XOFFSET, yPos + NUMBER2_YOFFSET, m_iCurActiveCount, -4.0f );
			}
		}
		break;
	case DACT_CONSECUTION3:
		{
			m_pActiveCountOver->Render( xPos + OVER3_XOFFSET, yPos + OVER3_YOFFSET );

			if( m_bRedUI )
			{
				m_pActiveCountOnRed->SetHorzFormatting( ioUIRenderImage::HF_CENTER );
				m_pActiveCountOnRed->RenderNum( xPos + NUMBER3_XOFFSET, yPos + NUMBER3_YOFFSET, m_iCurActiveCount, -4.0f );
			}
			else
			{
				m_pActiveCountOnYellow->SetHorzFormatting( ioUIRenderImage::HF_CENTER );
				m_pActiveCountOnYellow->RenderNum( xPos + NUMBER3_XOFFSET, yPos + NUMBER3_YOFFSET, m_iCurActiveCount, -4.0f );
			}
		}
		break;
	}
}

bool SkillListBtn::RenderUseSkillEffect( int xPos, int yPos )
{
	ioUIRenderImage *pImg = NULL;
	int iColor = 0;
	if( m_SkillState.GetCurState() == SDT_EFFECT_WHITE )
	{
		pImg =m_SlotInfo.pSkillImg;
		iColor = 255.0f * ( (float)m_SkillState.GetCurTime() / (float)m_SkillState.GetDelay() );
	}
	else if( m_SkillState.GetCurState() == SDT_EFFECT_NORMAL )
	{
		pImg =m_SlotInfo.pSkillImg;
		float fRate = FLOAT1 - ( (float)m_SkillState.GetCurTime() / (float)m_SkillState.GetDelay() );
		iColor = 255.0f * fRate;
	}
	else if( m_SkillState.GetCurState() == SDT_EFFECT_WHITE_ITEM )
	{
		pImg =m_SlotInfo.pItemImg;
		iColor = 255.0f * ( (float)m_SkillState.GetCurTime() / (float)m_SkillState.GetDelay() );
	}
	else if( m_SkillState.GetCurState() == SDT_EFFECT_NORMAL_ITEM )
	{
		pImg =m_SlotInfo.pItemImg;
		float fRate = FLOAT1 - ( (float)m_SkillState.GetCurTime() / (float)m_SkillState.GetDelay() );
		iColor = 255.0f * fRate;
	}
	else if( m_SkillState.GetCurState() == SDT_EFFECT_END )
	{
		if( m_SlotInfo.pItemImg )
			m_SlotInfo.pItemImg->SetColor( 255, 255, 255 );
		
		if( m_SlotInfo.pSkillImg)
		{
			m_SlotInfo.pSkillImg->SetColor(  255, 255, 255 );
		}
	}

	if( pImg )
	{
		pImg->SetColor( iColor, iColor, iColor );
		pImg->Render( xPos, yPos, UI_RENDER_NORMAL_ADD_COLOR );
		return true;
	}

	return false;
}


void SkillListBtn::InitSkillState()
{
	enum { DELAY_BEING_WHITE = 66, MAX_BEING_NORMAL = 66, };

	m_SkillState.SetCurState( SDT_NONE );

	FSMState *pState = new FSMState(SDT_NONE);
	if( pState )
		pState->Add( SDT_CAN,  SDT_CAN );
	m_SkillState.Add( pState );

	pState = new FSMState(SDT_CAN);
	if( pState )
	{
		pState->Add( SDT_WAIT,  SDT_EFFECT_WHITE_ITEM );
		pState->Add( SDT_USING, SDT_EFFECT_WHITE );
		pState->Add( SDT_NONE, SDT_NONE );
	}
	m_SkillState.Add( pState );

	pState = new FSMState(SDT_EFFECT_WHITE);
	if( pState )
		pState->Add( SDT_EFFECT_WHITE,  SDT_EFFECT_NORMAL, DELAY_BEING_WHITE );
	m_SkillState.Add( pState );

	pState = new FSMState(SDT_EFFECT_WHITE_ITEM);
	if( pState )
		pState->Add( SDT_EFFECT_WHITE_ITEM,  SDT_EFFECT_NORMAL_ITEM, DELAY_BEING_WHITE );
	m_SkillState.Add( pState );

	pState = new FSMState(SDT_EFFECT_NORMAL);
	if( pState )
		pState->Add( SDT_EFFECT_NORMAL,  SDT_EFFECT_END, MAX_BEING_NORMAL );
	m_SkillState.Add( pState );

	pState = new FSMState(SDT_EFFECT_NORMAL_ITEM);
	if( pState )
		pState->Add( SDT_EFFECT_NORMAL_ITEM,  SDT_EFFECT_END,  MAX_BEING_NORMAL );
	m_SkillState.Add( pState );

	pState = new FSMState(SDT_EFFECT_END);
	if( pState )
		pState->Add( SDT_EFFECT_END,  SDT_NONE );
	m_SkillState.Add( pState );
}

void SkillListBtn::InitKeyState()
{
	enum { DELAY_BEING_ALPHA = 165, DELAY_DONT_RENDER = 168 };

	m_KeyState.SetCurState( KET_NONE );

	FSMState *pState = new FSMState(KET_NONE);
	if( pState )
		pState->Add( KET_CAN_SKILL,  KET_EFFECT_ALPHA );
	m_KeyState.Add( pState );

	pState = new FSMState(KET_EFFECT_ALPHA);
	if( pState )
	{
		pState->Add( KET_EFFECT_ALPHA,  KET_EFFECT_NONE,  DELAY_BEING_ALPHA );
		pState->Add( KET_NONE,  KET_NONE );
	}
	m_KeyState.Add( pState );

	pState = new FSMState(KET_EFFECT_NONE);
	if( pState )
	{
		pState->Add( KET_EFFECT_NONE,  KET_EFFECT_ALPHA,  DELAY_DONT_RENDER );
		pState->Add( KET_NONE,  KET_NONE );
	}
	m_KeyState.Add( pState );

}

void SkillListBtn::iwm_create()
{
	SkillListWnd *pSkillListWnd = dynamic_cast<SkillListWnd *>( m_pParent );
	if(pSkillListWnd)
	{
		m_pSkillListWnd = pSkillListWnd;
	}
}

void SkillListBtn::SetEnableEffect( bool bEnable )
{
	if( bEnable )
	{
		m_bEnableEffect = true;
		m_dwEffectStartTime = FRAMEGETTIME();
		m_fCurEffectRate = 0.0f;

		if( !m_szEffectSound.IsEmpty() )
			g_SoundMgr.PlaySound( m_szEffectSound );
	}
	else
	{
		m_bEnableEffect = false;
	}
}

void SkillListBtn::UpdateEnableEffect()
{
	if( !m_bEnableEffect ) return;

	DWORD dwCurTime = FRAMEGETTIME();
	DWORD dwGapTime = dwCurTime - m_dwEffectStartTime;

	if( COMPARE( dwGapTime, m_dwEffectTime1, m_dwEffectTime1+m_dwEffectTime2 ) )
	{
		DWORD dwNewGap = dwGapTime - m_dwEffectTime1;
		float fCurRate = (float)dwNewGap/m_dwEffectTime2;
		m_fCurEffectRate = (float)sin( FLOAT_PI * FLOAT05 * fCurRate);
	}
	else
	{
		if( !COMPARE( dwGapTime, 0, m_dwEffectTime1 ) )
		{
			SetEnableEffect( false );
		}
	}
}

void SkillListBtn::RenderEnableEffect( int iXPos, int iYPos )
{
	if( !m_bEnableEffect ) return;

	DWORD dwCurTime = FRAMEGETTIME();
	DWORD dwGapTime = dwCurTime - m_dwEffectStartTime;

	if( dwGapTime < m_dwEffectTime1 )
	{
		if( m_pEnableEffect1 )
		{
			//m_pEnableEffect1->SetHorzFormatting( ioUIRenderImage::HF_CENTER );
			//m_pEnableEffect1->SetVertFormatting( ioUIRenderImage::VF_CENTER );
			m_pEnableEffect1->Render( iXPos, iYPos, UI_RENDER_NORMAL, TFO_BILINEAR );
		}
	}
	else if( dwGapTime < m_dwEffectTime1+m_dwEffectTime2 )
	{
		float fCurScale = (float)((m_fEffectScale - FLOAT1) * m_fCurEffectRate + FLOAT1);
		float fCurAlpha = (float)(255.0f * (FLOAT1 - m_fCurEffectRate));

		if( m_pEnableEffect2 )
		{
			//m_pEnableEffect2->SetHorzFormatting( ioUIRenderImage::HF_CENTER );
			//m_pEnableEffect2->SetVertFormatting( ioUIRenderImage::VF_CENTER );

			m_pEnableEffect2->SetScale( fCurScale );
			m_pEnableEffect2->SetAlpha( (BYTE)fCurAlpha );
			m_pEnableEffect2->Render( iXPos, iYPos, UI_RENDER_NORMAL, TFO_BILINEAR );
		}
	}
}

void SkillListBtn::RenderNeedBullet2( int xPos, int yPos )
{
	if( m_iMaxBullet == 0 )	return;

	if( !m_pBulletOver ) 
		return;

	if( !m_pExcaliburYellow || !m_pExcaliburRed )
		return;

	if( !m_pExcaliburYellowS || !m_pExcaliburRedS )
		return;

	enum { OVER_XOFFSET = 52, OVER_YOFFSET = - 17, 
		LV1_XOFFSET  = 58, LV1_YOFFSET  = - 12,
		LV2_XOFFSET  = 57, LV2_YOFFSET  = - 6,
		LV3_XOFFSET  = 62, LV3_YOFFSET  = - 6,
	    NUM_GAP = 3 };

	m_pBulletOver->Render( xPos + OVER_XOFFSET, yPos + OVER_YOFFSET);

	if( m_eBulletIconType == BIT_GUN ||
		m_eBulletIconType == BIT_SYLPHID ||
		m_eBulletIconType == BIT_UROBORUS ||
		m_eBulletIconType == BIT_ALTER_EGO )
	{
		if( m_iCurBullet == 0 )
		{
			m_pExcaliburRed->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
			m_pExcaliburRed->RenderNum( xPos+OVER_XOFFSET+4, yPos+OVER_YOFFSET+2, m_iCurBullet );
		}
		else
		{
			if( m_iCurBullet < 10 )
			{
				m_pExcaliburYellow->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
				m_pExcaliburYellow->RenderNum( xPos+OVER_XOFFSET+4, yPos+OVER_YOFFSET+2, m_iCurBullet );
			}
			else if( m_iCurBullet < FLOAT100 )
			{
				m_pExcaliburYellowS->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
				m_pExcaliburYellowS->RenderNum( xPos+OVER_XOFFSET, yPos+OVER_YOFFSET+3, m_iCurBullet, -NUM_GAP );
			}
			else
			{
				m_pExcaliburYellowS->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
				m_pExcaliburYellowS->RenderNum( xPos+OVER_XOFFSET-4, yPos+OVER_YOFFSET+3, m_iCurBullet, -NUM_GAP );
			}
		}
	}
	else if( m_eBulletIconType == BIT_BEAM )
	{
		// 우병의 bullet은 총알 개념이 아니므로 쿨타임인 ms로 설정됨 예) m_iMaxBullet = 1500
		int iMax = m_iMaxBullet / 10;
		if( m_iCurBullet == m_iMaxBullet )
		{
			if( iMax < 10 )
			{
				m_pExcaliburYellow->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
				m_pExcaliburYellow->RenderNum( xPos+OVER_XOFFSET+4, yPos+OVER_YOFFSET+2, iMax );
			}
			else if( iMax < FLOAT100 )
			{
				m_pExcaliburYellowS->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
				m_pExcaliburYellowS->RenderNum( xPos+OVER_XOFFSET, yPos+OVER_YOFFSET+3, iMax, -NUM_GAP );
			}
			else
			{
				m_pExcaliburYellowS->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
				m_pExcaliburYellowS->RenderNum( xPos+OVER_XOFFSET-4, yPos+OVER_YOFFSET+3, iMax, -NUM_GAP );
			}
		}
		else
		{
			if( m_iCurBullet < m_iNeedBullet )
			{
				float fRate = (float)m_iCurBullet / (float)m_iMaxBullet;
				int iNum = iMax*fRate;

				if( iNum < 10 )
				{
					m_pExcaliburRed->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
					m_pExcaliburRed->RenderNum( xPos+OVER_XOFFSET+4, yPos+OVER_YOFFSET+2, iNum );
				}
				else if( iNum < FLOAT100 )
				{
					m_pExcaliburRedS->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
					m_pExcaliburRedS->RenderNum( xPos+OVER_XOFFSET, yPos+OVER_YOFFSET+3, iNum, -NUM_GAP );
				}
				else
				{
					m_pExcaliburRedS->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
					m_pExcaliburRedS->RenderNum( xPos+OVER_XOFFSET-4, yPos+OVER_YOFFSET+3, iNum, -NUM_GAP );
				}
			}
			else
			{
				float fRate = (float)m_iCurBullet / (float)m_iMaxBullet;
				int iNum = iMax*fRate;

				if( iNum < 10 )
				{
					m_pExcaliburYellow->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
					m_pExcaliburYellow->RenderNum( xPos+OVER_XOFFSET+4, yPos+OVER_YOFFSET+2, iNum );
				}
				else if( iNum < FLOAT100 )
				{
					m_pExcaliburYellowS->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
					m_pExcaliburYellowS->RenderNum( xPos+OVER_XOFFSET, yPos+OVER_YOFFSET+3, iNum, -NUM_GAP );
				}
				else
				{
					m_pExcaliburYellowS->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
					m_pExcaliburYellowS->RenderNum( xPos+OVER_XOFFSET-4, yPos+OVER_YOFFSET+3, iNum, -NUM_GAP );
				}
			}
		}
	}
	else if( m_eBulletIconType == BIT_MOON || 
			 m_eBulletIconType == BIT_WITCH ||
			 m_eBulletIconType == BIT_MECHANICS )
	{
		if( m_bExtraValue )
		{
			if( m_iCurBullet < 10 )
			{
				m_pExcaliburYellow->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
				m_pExcaliburYellow->RenderNum( xPos+OVER_XOFFSET+4, yPos+OVER_YOFFSET+2, m_iCurBullet );
			}
			else if( m_iCurBullet < FLOAT100 )
			{
				m_pExcaliburYellowS->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
				m_pExcaliburYellowS->RenderNum( xPos+OVER_XOFFSET, yPos+OVER_YOFFSET+3, m_iCurBullet, -NUM_GAP );
			}
			else
			{
				m_pExcaliburYellowS->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
				m_pExcaliburYellowS->RenderNum( xPos+OVER_XOFFSET-4, yPos+OVER_YOFFSET+3, m_iCurBullet, -NUM_GAP );
			}
		}
		else
		{
			if( m_iCurBullet < m_iNeedBullet )
			{
				if( m_iCurBullet < 10 )
				{
					m_pExcaliburRed->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
					m_pExcaliburRed->RenderNum( xPos+OVER_XOFFSET+4, yPos+OVER_YOFFSET+2, m_iCurBullet );
				}
				else if( m_iCurBullet < FLOAT100 )
				{
					m_pExcaliburRedS->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
					m_pExcaliburRedS->RenderNum( xPos+OVER_XOFFSET, yPos+OVER_YOFFSET+3, m_iCurBullet, -NUM_GAP );
				}
				else
				{
					m_pExcaliburRedS->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
					m_pExcaliburRedS->RenderNum( xPos+OVER_XOFFSET-4, yPos+OVER_YOFFSET+3, m_iCurBullet, -NUM_GAP );
				}
			}
			else
			{
				if( m_iCurBullet < 10 )
				{
					m_pExcaliburYellow->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
					m_pExcaliburYellow->RenderNum( xPos+OVER_XOFFSET+4, yPos+OVER_YOFFSET+2, m_iCurBullet );
				}
				else if( m_iCurBullet < FLOAT100 )
				{
					m_pExcaliburYellowS->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
					m_pExcaliburYellowS->RenderNum( xPos+OVER_XOFFSET, yPos+OVER_YOFFSET+3, m_iCurBullet, -NUM_GAP );
				}
				else
				{
					m_pExcaliburYellowS->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
					m_pExcaliburYellowS->RenderNum( xPos+OVER_XOFFSET-4, yPos+OVER_YOFFSET+3, m_iCurBullet, -NUM_GAP );
				}
			}
		}
	}
	else if( m_eBulletIconType == BIT_HIDE )
	{
		if( m_iCurBullet < m_iMaxBullet )
		{
			if( m_iCurBullet < 10 )
			{
				m_pExcaliburRed->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
				m_pExcaliburRed->RenderNum( xPos+OVER_XOFFSET+4, yPos+OVER_YOFFSET+2, m_iCurBullet );
			}
			else if( m_iCurBullet < FLOAT100 )
			{
				m_pExcaliburRedS->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
				m_pExcaliburRedS->RenderNum( xPos+OVER_XOFFSET, yPos+OVER_YOFFSET+3, m_iCurBullet, -NUM_GAP );
			}
			else
			{
				m_pExcaliburRedS->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
				m_pExcaliburRedS->RenderNum( xPos+OVER_XOFFSET-4, yPos+OVER_YOFFSET+3, m_iCurBullet, -NUM_GAP );
			}
		}
		else
		{
			if( m_iCurBullet < 10 )
			{
				m_pExcaliburYellow->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
				m_pExcaliburYellow->RenderNum( xPos+OVER_XOFFSET+4, yPos+OVER_YOFFSET+2, m_iCurBullet );
			}
			else if( m_iCurBullet < FLOAT100 )
			{
				m_pExcaliburYellowS->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
				m_pExcaliburYellowS->RenderNum( xPos+OVER_XOFFSET, yPos+OVER_YOFFSET+3, m_iCurBullet, -NUM_GAP );
			}
			else
			{
				m_pExcaliburYellowS->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
				m_pExcaliburYellowS->RenderNum( xPos+OVER_XOFFSET-4, yPos+OVER_YOFFSET+3, m_iCurBullet, -NUM_GAP );
			}
		}
	}
	else if( m_eBulletIconType == BIT_MINE || m_eBulletIconType == BIT_CHARGING )
	{
		if( m_iCurBullet < 10 )
		{
			m_pExcaliburYellow->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
			m_pExcaliburYellow->RenderNum( xPos+OVER_XOFFSET+4, yPos+OVER_YOFFSET+2, m_iCurBullet );
		}
		else if( m_iCurBullet < FLOAT100 )
		{
			m_pExcaliburYellowS->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
			m_pExcaliburYellowS->RenderNum( xPos+OVER_XOFFSET, yPos+OVER_YOFFSET+3, m_iCurBullet, -NUM_GAP );
		}
		else
		{
			m_pExcaliburYellowS->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
			m_pExcaliburYellowS->RenderNum( xPos+OVER_XOFFSET-4, yPos+OVER_YOFFSET+3, m_iCurBullet, -NUM_GAP );
		}
	}
	else if( m_eBulletIconType == BIT_EXCALIBUR )
	{
		if( m_iCurBullet < m_iNeedBullet )
		{
			if( m_iCurBullet < 10 )
			{
				m_pExcaliburRed->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
				m_pExcaliburRed->RenderNum( xPos+OVER_XOFFSET+4, yPos+OVER_YOFFSET+2, m_iCurBullet );
			}
			else if( m_iCurBullet < FLOAT100 )
			{
				m_pExcaliburRedS->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
				m_pExcaliburRedS->RenderNum( xPos+OVER_XOFFSET, yPos+OVER_YOFFSET+3, m_iCurBullet, -NUM_GAP );
			}
			else
			{
				m_pExcaliburRedS->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
				m_pExcaliburRedS->RenderNum( xPos+OVER_XOFFSET-4, yPos+OVER_YOFFSET+3, m_iCurBullet, -NUM_GAP );
			}
		}
		else
		{
			if( m_iCurBullet < 10 )
			{
				m_pExcaliburYellow->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
				m_pExcaliburYellow->RenderNum( xPos+OVER_XOFFSET+4, yPos+OVER_YOFFSET+2, m_iCurBullet );
			}
			else if( m_iCurBullet < FLOAT100 )
			{
				m_pExcaliburYellowS->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
				m_pExcaliburYellowS->RenderNum( xPos+OVER_XOFFSET, yPos+OVER_YOFFSET+3, m_iCurBullet, -NUM_GAP );
			}
			else
			{
				m_pExcaliburYellowS->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
				m_pExcaliburYellowS->RenderNum( xPos+OVER_XOFFSET-4, yPos+OVER_YOFFSET+3, m_iCurBullet, -NUM_GAP );
			}
		}
	}
	else if( m_eBulletIconType == BIT_EXTRA )
	{
		if( m_bExtraValue )
		{
			if( m_iCurBullet < 10 )
			{
				m_pExcaliburRed->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
				m_pExcaliburRed->RenderNum( xPos+OVER_XOFFSET+4, yPos+OVER_YOFFSET+2, m_iCurBullet );
			}
			else if( m_iCurBullet < FLOAT100 )
			{
				m_pExcaliburRedS->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
				m_pExcaliburRedS->RenderNum( xPos+OVER_XOFFSET, yPos+OVER_YOFFSET+3, m_iCurBullet, -NUM_GAP );
			}
			else
			{
				m_pExcaliburRedS->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
				m_pExcaliburRedS->RenderNum( xPos+OVER_XOFFSET-4, yPos+OVER_YOFFSET+3, m_iCurBullet, -NUM_GAP );
			}
		}
		else
		{
			if( m_iCurBullet < 10 )
			{
				m_pExcaliburYellow->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
				m_pExcaliburYellow->RenderNum( xPos+OVER_XOFFSET+4, yPos+OVER_YOFFSET+2, m_iCurBullet );
			}
			else if( m_iCurBullet < FLOAT100 )
			{
				m_pExcaliburYellowS->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
				m_pExcaliburYellowS->RenderNum( xPos+OVER_XOFFSET, yPos+OVER_YOFFSET+3, m_iCurBullet, -NUM_GAP );
			}
			else
			{
				m_pExcaliburYellowS->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
				m_pExcaliburYellowS->RenderNum( xPos+OVER_XOFFSET-4, yPos+OVER_YOFFSET+3, m_iCurBullet, -NUM_GAP );
			}
		}
	}
}

void SkillListBtn::RenderExtraGauge2( int iXPos, int iYPos )
{
	if( !m_pExtraGaugeBack || !m_pExtraGaugeRed || !m_pExtraGaugeYellow )
		return;

	enum { OVER_XOFFSET = 52, OVER_YOFFSET = - 17, 
		LV1_XOFFSET  = 58, LV1_YOFFSET  = - 12,
		LV2_XOFFSET  = 57, LV2_YOFFSET  = - 6,
		LV3_XOFFSET  = 62, LV3_YOFFSET  = - 6,};

	int iGaugeXPos = iXPos + OVER_XOFFSET + 8;
	int iGaugeYPos = iYPos + OVER_YOFFSET + 15;


	if( m_iMaxBullet > 0 &&
		(m_eBulletIconType == BIT_GUN ||
		 m_eBulletIconType == BIT_MINE ||
		 m_eBulletIconType == BIT_SYLPHID ||
		 m_eBulletIconType == BIT_CHARGING ||
		 m_eBulletIconType == BIT_UROBORUS ||
		 m_eBulletIconType == BIT_ALTER_EGO) )
	{
		float fGaugeRate = (float)m_iCurBullet / m_iMaxBullet;
		m_pExtraGaugeBack->Render( iGaugeXPos, iGaugeYPos );

		if( fGaugeRate > 0.0f )
		{
			int iDefaultHeight = m_pExtraGaugeYellow->GetHeight();
			int iHeight = (float)iDefaultHeight * fGaugeRate;
			m_pExtraGaugeYellow->RenderHeightCut( iGaugeXPos, iGaugeYPos, iDefaultHeight-iHeight, iDefaultHeight );
		}
	}
	else if( m_iMaxBullet > 0 &&
			 (m_eBulletIconType == BIT_MOON ||
			 m_eBulletIconType == BIT_WITCH ||
			 m_eBulletIconType == BIT_MECHANICS) )
	{
		float fGaugeRate = (float)m_iCurBullet / m_iMaxBullet;
		m_pExtraGaugeBack->Render( iGaugeXPos, iGaugeYPos );

		if( fGaugeRate > 0.0f )
		{
			int iDefaultHeight = m_pExtraGaugeYellow->GetHeight();
			int iHeight = (float)iDefaultHeight * fGaugeRate;

			if( m_bExtraValue || (m_iCurBullet >= m_iNeedBullet) )
				m_pExtraGaugeYellow->RenderHeightCut( iGaugeXPos, iGaugeYPos, iDefaultHeight-iHeight, iDefaultHeight );
			else
				m_pExtraGaugeRed->RenderHeightCut( iGaugeXPos, iGaugeYPos, iDefaultHeight-iHeight, iDefaultHeight );
		}
	}
	else if( m_iMaxBullet > 0 && m_eBulletIconType == BIT_HIDE )
	{
		float fGaugeRate = (float)m_iCurBullet / m_iMaxBullet;
		m_pExtraGaugeBack->Render( iGaugeXPos, iGaugeYPos );

		if( fGaugeRate > 0.0f )
		{
			int iDefaultHeight = m_pExtraGaugeYellow->GetHeight();
			int iHeight = (float)iDefaultHeight * fGaugeRate;

			if( m_iCurBullet == m_iMaxBullet )
				m_pExtraGaugeYellow->RenderHeightCut( iGaugeXPos, iGaugeYPos, iDefaultHeight-iHeight, iDefaultHeight );
			else
				m_pExtraGaugeRed->RenderHeightCut( iGaugeXPos, iGaugeYPos, iDefaultHeight-iHeight, iDefaultHeight );
		}
	}
	else if( m_iMaxBullet > 0 && m_eBulletIconType == BIT_BEAM )
	{
		float fGaugeRate = (float)m_iCurBullet / m_iMaxBullet;
		m_pExtraGaugeBack->Render( iGaugeXPos, iGaugeYPos );

		if( fGaugeRate > 0.0f )
		{
			int iDefaultHeight = m_pExtraGaugeRed->GetHeight();
			int iHeight = (float)iDefaultHeight * fGaugeRate;

			if( m_iCurBullet < m_iNeedBullet )
				m_pExtraGaugeRed->RenderHeightCut( iGaugeXPos, iGaugeYPos, iDefaultHeight-iHeight, iDefaultHeight );
			else
				m_pExtraGaugeYellow->RenderHeightCut( iGaugeXPos, iGaugeYPos, iDefaultHeight-iHeight, iDefaultHeight );
		}
	}
	else if( m_eBulletIconType == BIT_EXCALIBUR )
	{
		if( m_fMaxExtraGauge <= 0.0f )
			return;

		float fGaugeRate = m_fCurExtraGauge / m_fMaxExtraGauge;
		m_pExtraGaugeBack->Render( iGaugeXPos, iGaugeYPos );

		switch( m_iCurBullet )
		{
		case 0:
			if( fGaugeRate > 0.0f )
			{
				int iDefaultHeight = m_pExtraGaugeRed->GetHeight();
				int iHeight = (float)iDefaultHeight * fGaugeRate;
				m_pExtraGaugeRed->RenderHeightCut( iGaugeXPos, iGaugeYPos, iDefaultHeight-iHeight, iDefaultHeight );
			}
			break;
		case 1:
			if( fGaugeRate > 0.0f )
			{
				int iDefaultHeight = m_pExtraGaugeYellow->GetHeight();
				int iHeight = (float)iDefaultHeight * fGaugeRate;
				m_pExtraGaugeYellow->RenderHeightCut( iGaugeXPos, iGaugeYPos, iDefaultHeight-iHeight, iDefaultHeight );
			}
			break;
		case 2:
			m_pExtraGaugeYellow->Render( iGaugeXPos, iGaugeYPos );
			break;
		}
	}
	else if( m_eBulletIconType == BIT_EXTRA )
	{
		if( m_fMaxExtraGauge <= 0.0f )
			return;

		float fGaugeRate = m_fCurExtraGauge / m_fMaxExtraGauge;
		m_pExtraGaugeBack->Render( iGaugeXPos, iGaugeYPos );

		if( fGaugeRate > 0.0f )
		{
			int iDefaultHeight = m_pExtraGaugeYellow->GetHeight();
			int iHeight = (float)iDefaultHeight * fGaugeRate;

			if( m_bExtraValue )
				m_pExtraGaugeRed->RenderHeightCut( iGaugeXPos, iGaugeYPos, iDefaultHeight-iHeight, iDefaultHeight );
			else
				m_pExtraGaugeYellow->RenderHeightCut( iGaugeXPos, iGaugeYPos, iDefaultHeight-iHeight, iDefaultHeight );
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void SkillListBtn::RenderNeedBullet3( int xPos, int yPos )
{
	if( m_iMaxSecoundBullet == 0 )	return;

	if( !m_pBulletOver ) 
		return;

	if( !m_pExcaliburYellow || !m_pExcaliburRed )
		return;

	if( !m_pExcaliburYellowS || !m_pExcaliburRedS )
		return;

	enum { OVER_XOFFSET = 52, OVER_YOFFSET = - 17, 
		LV1_XOFFSET  = 58, LV1_YOFFSET  = - 12,
		LV2_XOFFSET  = 57, LV2_YOFFSET  = - 6,
		LV3_XOFFSET  = 62, LV3_YOFFSET  = - 6,
		NUM_GAP = 3 };

	m_pBulletOver->Render( xPos + OVER_XOFFSET, yPos + OVER_YOFFSET);

	if( m_eBulletIconType == BIT_MOON || 
		m_eBulletIconType == BIT_WITCH ||
		m_eBulletIconType == BIT_MECHANICS )
	{
		if( m_bExtraValue )
		{
			if( m_iCurSecoundBullet < 10 )
			{
				m_pExcaliburYellow->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
				m_pExcaliburYellow->RenderNum( xPos+OVER_XOFFSET+4, yPos+OVER_YOFFSET+2, m_iCurSecoundBullet );
			}
			else if( m_iCurSecoundBullet < FLOAT100 )
			{
				m_pExcaliburYellowS->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
				m_pExcaliburYellowS->RenderNum( xPos+OVER_XOFFSET, yPos+OVER_YOFFSET+3, m_iCurSecoundBullet, -NUM_GAP );
			}
			else
			{
				m_pExcaliburYellowS->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
				m_pExcaliburYellowS->RenderNum( xPos+OVER_XOFFSET-4, yPos+OVER_YOFFSET+3, m_iCurSecoundBullet, -NUM_GAP );
			}
		}
		else
		{
			if( m_iCurSecoundBullet < m_iNeedSecoundBullet )
			{
				if( m_iCurSecoundBullet < 10 )
				{
					m_pExcaliburRed->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
					m_pExcaliburRed->RenderNum( xPos+OVER_XOFFSET+4, yPos+OVER_YOFFSET+2, m_iCurSecoundBullet );
				}
				else if( m_iCurBullet < FLOAT100 )
				{
					m_pExcaliburRedS->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
					m_pExcaliburRedS->RenderNum( xPos+OVER_XOFFSET, yPos+OVER_YOFFSET+3, m_iCurSecoundBullet, -NUM_GAP );
				}
				else
				{
					m_pExcaliburRedS->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
					m_pExcaliburRedS->RenderNum( xPos+OVER_XOFFSET-4, yPos+OVER_YOFFSET+3, m_iCurSecoundBullet, -NUM_GAP );
				}
			}
			else
			{
				if( m_iCurSecoundBullet < 10 )
				{
					m_pExcaliburYellow->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
					m_pExcaliburYellow->RenderNum( xPos+OVER_XOFFSET+4, yPos+OVER_YOFFSET+2, m_iCurSecoundBullet );
				}
				else if( m_iCurSecoundBullet < FLOAT100 )
				{
					m_pExcaliburYellowS->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
					m_pExcaliburYellowS->RenderNum( xPos+OVER_XOFFSET, yPos+OVER_YOFFSET+3, m_iCurSecoundBullet, -NUM_GAP );
				}
				else
				{
					m_pExcaliburYellowS->SetHorzFormatting( ioUIRenderImage::HF_LEFT_ALIGN );
					m_pExcaliburYellowS->RenderNum( xPos+OVER_XOFFSET-4, yPos+OVER_YOFFSET+3, m_iCurSecoundBullet, -NUM_GAP );
				}
			}
		}
	}
}


void SkillListBtn::RenderExtraGauge3( int iXPos, int iYPos )
{
	if( !m_pExtraGaugeBack || !m_pExtraGaugeRed || !m_pExtraGaugeYellow )
		return;

	enum { OVER_XOFFSET = 52, OVER_YOFFSET = - 17, 
		LV1_XOFFSET  = 58, LV1_YOFFSET  = - 12,
		LV2_XOFFSET  = 57, LV2_YOFFSET  = - 6,
		LV3_XOFFSET  = 62, LV3_YOFFSET  = - 6,};

	int iGaugeXPos = iXPos + OVER_XOFFSET + 8;
	int iGaugeYPos = iYPos + OVER_YOFFSET + 15;


	if( m_iMaxSecoundBullet > 0 &&
		(m_eBulletIconType == BIT_MOON ||
		m_eBulletIconType == BIT_WITCH ||
		m_eBulletIconType == BIT_MECHANICS) )
	{
		float fGaugeRate = (float)m_iCurSecoundBullet / m_iMaxSecoundBullet;
		m_pExtraGaugeBack->Render( iGaugeXPos, iGaugeYPos );

		if( fGaugeRate > 0.0f )
		{
			int iDefaultHeight = m_pExtraGaugeYellow->GetHeight();
			int iHeight = (float)iDefaultHeight * fGaugeRate;

			if( m_bExtraValue || (m_iCurSecoundBullet >= m_iNeedSecoundBullet) )
				m_pExtraGaugeYellow->RenderHeightCut( iGaugeXPos, iGaugeYPos, iDefaultHeight-iHeight, iDefaultHeight );
			else
				m_pExtraGaugeRed->RenderHeightCut( iGaugeXPos, iGaugeYPos, iDefaultHeight-iHeight, iDefaultHeight );
		}
	}
}

void SkillListBtn::DeleteSkillInfo( bool bDeleteInfo /*= false */ )
{
	if( bDeleteInfo )
		m_pSkill = NULL;
}

//////////////////////////////////////////////////////////////////////////
SkillListWnd::SkillListWnd()
{
	for (int i = 0; i < MAX_SLOT ; i++)
	{
		m_iClassTypes[i] = 0;	
	}
	m_iDefaultX = 0;
	m_iDefaultY = 0;
	m_iCurOverWnd = -1;
	m_eOwnerState = CS_DELAY;

	m_pToolTipCen = NULL;
	m_pToolTipLMid = NULL;
	m_pToolTipRMid = NULL;
	m_pToolTipLeft = NULL;
	m_pToolTipRight = NULL;

	m_pPointImage = NULL;

	m_dwTimeGrowthToolTipStartTime = 0;
	m_dwTimeGrowthToolTipRefreshTime = 0;
	m_dwToolTipShowTime = 0;
	m_dwToolTipRefreshTime = 0;

	m_eModeType = MT_NONE;

	InitShowAniState();
}

SkillListWnd::~SkillListWnd()
{
	SAFEDELETE( m_pToolTipCen );
	SAFEDELETE( m_pToolTipLMid );
	SAFEDELETE( m_pToolTipRMid );
	SAFEDELETE( m_pToolTipLeft );
	SAFEDELETE( m_pToolTipRight );

	SAFEDELETE( m_pPointImage );

	KeyImageSetMap::iterator iCreate;
	for( iCreate = m_KeyImageSetMap.begin() ; iCreate != m_KeyImageSetMap.end() ; ++iCreate )
	{
		SAFEDELETE( iCreate->second );
	}
	m_KeyImageSetMap.clear();

	if( !COMPARE( m_iDefaultX, 0, Setting::Width() ) || 
		!COMPARE( m_iDefaultY, 0, Setting::Height() ) )
		return;

	g_GUIMgr.GUIPosSaveInt( "SkillListWnd", "XPos", m_iDefaultX ,POS_SAVE_VERSION );
	g_GUIMgr.GUIPosSaveInt( "SkillListWnd", "YPos", m_iDefaultY ,POS_SAVE_VERSION );
}

void SkillListWnd::SetCurModeType( ModeType eModeType )
{
	m_eModeType = eModeType;
}

void SkillListWnd::UpdateByOwner( ioBaseChar *pOwner )
{
	if( !pOwner )	return;

	m_eOwnerState = pOwner->GetState();
	UpdateSkillList( pOwner );

	for( int i=0; i<MAX_SLOT; i++ )
	{
		int iClassType = 0;
		bool bGaugeMax = false;
		ioItem *pCurItem = pOwner->GetEquipedItem( i );
		if( pCurItem )
		{
			int iMax = pCurItem->GetMaxSkillGauge(pOwner->GetCharName());
			int iCur = pCurItem->GetCurSkillGuage();
			float fRecovery = pCurItem->GetRecoverySkillGuage();
			int iRecoveryTic = (int)pOwner->GetDefaultRecoveryGaugeTic();
			int iExtendMax = 0;
			bool bUsePassiveGauge = false;
			
			ioSkill *pSkill = pOwner->GetEquipedSkill( i );
			ioPassiveSkill *pPassiveSkill = ToPassiveSkill( pSkill );
			if( pPassiveSkill && pPassiveSkill->UsePassiveGauge() )
			{
				iMax = pPassiveSkill->GetMaxPassiveGauge();
				iCur = pPassiveSkill->GetCurPassiveGauge();
				bUsePassiveGauge = true;
			}

			ioHashString szExtendSkill;
			if( pCurItem->GetExtendSkillName( szExtendSkill ) )
			{
				iExtendMax = pCurItem->GetMaxExtendSkillGauge( pOwner->GetCharName() );
			}

			//iCur = min( iCur, iMax );
			UpdateGaugeInfo( i, iMax, iCur, fRecovery, iRecoveryTic, iExtendMax, pCurItem->HasSkillGauge(), bUsePassiveGauge );
			iClassType = pOwner->GetCharacterInfo().m_class_type;

			if( iCur >= iMax )
				bGaugeMax = true;
		}
		else
		{
			UpdateGaugeInfo( i, 0, 0, 0, 0, 0, false );
			iClassType = 0;
		}

		bool bChange = false;
		if( m_iClassTypes[i] != iClassType )
		{
			m_iClassTypes[i] = iClassType;
			bChange = true;
		}
		// skill desc
		ioSkill *pSkill = pOwner->GetEquipedSkill( i );
		if( pSkill )
		{
			m_kSkillDesc[i].m_szDescName = pSkill->GetDescName();
			m_kSkillDesc[i].m_szDescFullName = pSkill->GetDescFullName();
			m_kSkillDesc[i].m_szDesc     = pSkill->GetDesc();
			m_kSkillDesc[i].m_szIconName = pSkill->GetIconName();
		}
		else
		{
			m_kSkillDesc[i].m_szDescName.Clear();
			m_kSkillDesc[i].m_szDescFullName.Clear();
			m_kSkillDesc[i].m_szDesc.Clear();
			m_kSkillDesc[i].m_szIconName.Clear();
		}

		SkillListBtn *pSkillBtn = (SkillListBtn*)FindChildWnd( i );
		if( pSkillBtn && bChange && bGaugeMax )
		{
			if( pSkill && pSkill->GetTypeID() != ST_PASSIVE )
			{
				pSkillBtn->SetEnableEffect( true );
			}
		}

		UpdateActiveCount( pOwner, i );
	}
	
	UpdateBullet( 0, 0, 0, SkillListBtn::BIT_GUN );
	UpdateExtraGauge( 0.0f, 0.0f );

	//
	SetUpdateBulletNormal( pOwner );
}

void SkillListWnd::SetUpdateBulletNormal( ioBaseChar *pOwner )
{
	if( !pOwner )
		return;

	ioItem *pItem = pOwner->GetEquipedItem( ID_WEAPON );
	if( pItem )
	{
		int iMax = pItem->GetMaxBullet();
		int iCur = pItem->GetCurBullet();
		int iNeed = 0;
		iCur = min( iCur, iMax );

		ioFireWeaponItem *pFireItem = ToFireWeaponItem(pItem);
		ioBowItem *pBowItem = ToBowItem(pItem);
		ioAutoShotItem *pAutoShot = ToAutoShotItem(pItem);
		ioLaserItem *pLaserWeapon = ToLaserItem(pItem);
		ioMineItem *pMineWeapon = ToMineItem(pItem);
		ioUroborusItem *pUroborus = ToUroborusItem(pItem);
		ioWereWolfItem *pWereWolf = ToWereWolfItem(pItem);
		ioJoroExpansionItem *pJoroExpansionItem = ToJoroExpansionItem(pItem);
		ioBuffAddItem *pBuffAdd = ToBuffAddItem(pItem);
		ioRachelItem *pRachelItem = ToRachelItem(pItem);
		ioIljimaeItem *pIljimaeItem = ToIljimaeItem(pItem);
		ioAlterEgoItem *pAlterEgoItem = ToAlterEgoItem(pItem);
		ioWitchItem *pWitchItem = ToWitchItem(pItem);
		ioExcaliburItem *pExcalibur = ToExcaliburItem(pItem);
		ioWindLordItem *pWindLordItem = ToWindLordItem(pItem);
		ioProject_K_Item *pProjectKItem = ToProject_K_Item(pItem);
		ioMechanicsItem *pMechanics = ToMechanicsItem(pItem);
		ioMichaelItem *pMichael = ToMichaelItem(pItem);
		ioGoomihoItem *pGoomiho = ToGoomihoItem(pItem);
		ioBladeItem *pBladeItem = ToBladeItem( pItem );
		ioSuccubusItem *pSuccubus = ToSuccubusItem(pItem);
		ioHadesItem *pHades = ToHadesItem(pItem);
		ioCrowItem *pCrowItem = ToCrowItem(pItem);
		ioShaolinItem *pShaolinItem = ToShaolinItem(pItem);
		ioGhostStealer *pGhostStealer = ToGhostStealerItem(pItem);
		ioMutantItem *pMutantItem = ToMutantItem(pItem);
		ioDraculaItem *pDraculaItem = ToDraculaItem( pItem );
		ioHakumenItem *pHakumenItem = ToHakumenItem( pItem );
		ioEvyItem *pEvyItem = ToEvyItem( pItem );
		ioRedHoodItem *pRedHoodItem = ToRedHoodItem( pItem );
		ioPsychoMetry_Item *pPsychoItem = ToPsychoMetryItem( pItem );
		ioSlasher_Item *pSlasherItem = ToSlasherItem( pItem );
		ioSuperDimensionItem *pSuperDimentionItem = ToSuperDimensionItem( pItem );
		ioTraceItem *pTraceItem = ToTraceItem( pItem );
		ioKaelthasItem *pKaelthasItem = ToKaelthasItem( pItem );
		ioStrikerItem *pStrikerItem = ToStrikerItem( pItem );
		ioMephistoItem *pMephistoItem = ToMephistoItem(pItem);

		ioChargeCommandItem *pChargeCmdItem = ToChargeCommandItem( pItem );

		ioCrossBowItem *pCrossBowItem = ToCrossBowItem(pItem);
		ioPhantomItem *pPhantom = ToPhantomItem(pItem);
		ioYuMiRaItem *pYuMiRa = ToYuMiRaItem(pItem);
		ioJinMoRiItem *pJinMoRi = ToJinMoRiItem(pItem);
		ioLuciferItem *pLucifer = ToLuciferItem(pItem);
		ioChargeContactWeaponItem *pChargeContact = ToChargeContactWeaponItem(pItem);
		ioChargeComboBowItem *pChargeComboBowItem = ToChargeComboBowItem(pItem);
		ioNakoruruItem *pNakoruru = ToNakoruruItem( pItem );
		ioDestroyerItem *pDestroyer = ToDestroyerItem( pItem );
		ioPeterpanItem *pPerterpan = ToPeterpanItem( pItem );
		ioCyvogueItem *pCyvogueItem = ToCyvogueItem( pItem );
		ioDarkSeasItem *pDarkSeasItem = ToDarkSeasItem( pItem );
		ioTrinityItem *pTrinityItem = ToTrinityItem( pItem );
		ioDevilItem *pDevilItem = ToDevilItem( pItem );
		ioMaidItem *pMaidItem = ToMaidItem( pItem );
		ioFlash_Item *pFlashItem = ToFlashItem(pItem);
		ioSuddenAttackItem *pSuddenAttackItem = ToSuddenAttackItem(pItem);
		ioDummyCharItem2 *pDummyCharItem2 = ToDummyCharItem2(pItem);
		ioWestGunExtendItem *pWestGunExtendItem = ToWestGunExtendItem( pItem );

		ioPowerChargeItem *pPowerCharge = ToPowerChargeItem(pItem);
		ioPowerChargeItem4 *pPowerCharge4 = ToPowerChargeItem4(pItem);
		ioPowerChargeItem6 *pPowerCharge6 = ToPowerChargeItem6(pItem);
		ioRunChargeItem *pRunCharge = ToRunChargeItem(pItem);

		ioBowItem2 *pBowItem2 = ToBowItem2( pItem );

		ioTitanItem *pTitanItem = ToTitanWeaponItem(pItem);
		ioJeonWooChiItem *pJeonWooChiItem = ToJeonWooChiWeaponItem(pItem);
		ioGalaxyItem *pGalaxy = ToGalaxyItem(pItem);
		ioDragonSlayerItem *pDragonSlayer = ToDragonSlayerItem( pItem );
		ioNanginItem* pNangin = ToNanginItem( pItem );

		ioDonQuixoteItem *pDonQuixote = ToDonQuixoteItem( pItem );
		ioThunderBirdItem *pThunderBird = ToThunderBirdItem( pItem );
		ioAliceItem* pAlice = ToAliceItem( pItem );
		ioLeeItem *pLeeItem = ToLeeItem( pItem );
		ioCuchulainItem *pCuchulainItem = ToCuchulainWeaponItem( pItem );
		ioValkyrie* pValkyrie = ToValkyrie( pItem );
		ioParkilpyoItem *pParkilpyoItem = ToParkilpyoItem( pItem );
		ioEarthQuakeItem *pEarthQuakeItem = ToEarthQuakeItem( pItem );
		ioPentjakSilatItem *pPentjakSilatItem = ToPentjakSilatItem( pItem );
		ioJackItem *pJackItem = ToJackItem(pItem);
		ioShadowRangerItem *pShadowRangerItem = ToShadowRangerItem( pItem );
		ioDarkKnightItem* pDarkKnight = ToDarkKnightItem( pItem );
		ioJumperItem* pJumper = ToJumperItem( pItem );
		ioEngineerItem* pEngineer = ToEngineerItem( pItem );
		ioGeomjeItem* pGeomje = ToGeomjeItem( pItem );
		ioZeusItem*	pZeus	= ToZeusItem( pItem );
		ioSaberItem *pSaber= ToSaberItem( pItem );
		ioOutBoxerItem *pOutBoxer = ToOutBoxerItem( pItem );
		ioMagicSwordItem *pMagicSwordItem = ToMagicSwordItem(pItem);
		ioNataItem *pNataItem = ToNataItem( pItem );
		ioLeeSunSin* pLeeSunSin = ToLeeSunSin( pItem );
		ioResearcher* pResearcher = ToResearcher( pItem );
		ioBullFightItem *pBullFightItem = ToBullFightItem(pItem);
		ioSawangItem *pSawang = ToSawangItem(pItem);
		ioChainMagicianItem *pChainMagicianItem = ToChainMagicianItem( pItem );
		ioStriderItem *pStrider = ToStriderItem( pItem );
		ioApostatemonkItem *ApostateMonk = ToApostatemonkItem( pItem );
		ioGanesa *pGanesa = ToGanesa( pItem );
		ioSamuraiExpansionItem *pSamuraiExpansionItem = ToSamuraiExpansionItem( pItem );
		ioMirWarrior* pMir = ToMirWarrior( pItem );
		ioGalfordItem *pGalford = ToGalfordItem( pItem );
		ioOniMushaItem *pOniMushaItem = ToOniMushaItem( pItem );
		ioDruidItem* pDruid = ToDruidItem( pItem );
		ioSasinmuItem *pSasinmuItem = ToSasinmuItem( pItem );
		ioPredatorItem *pPredatorItem = ToPredatorItem( pItem );
		ioBlackSpearItem *pBlackSpearItem= ToBlackSpearItem(pItem);
		ioAttackableItem *pAttackItem = ToAttackableItem(pItem);

		int iGraffitiMaxCnt = 0;
		if( pAttackItem )
			iGraffitiMaxCnt = pAttackItem->GetGraffitiWeaponActionMax();

		if( pLaserWeapon )
		{
			iNeed = pLaserWeapon->GetNeedBullet();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_BEAM );
		}
		else if( pUroborus )
		{
			iNeed = pUroborus->GetNeedBullet();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_UROBORUS );
		}
		else if( pTitanItem )
		{
			iNeed = pTitanItem->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_UROBORUS );
		}
		else if( pJeonWooChiItem )
		{
			iNeed = pJeonWooChiItem->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_UROBORUS );
		}
		else if( pGhostStealer )
		{
			iNeed = pGhostStealer->GetNeedBullet();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_UROBORUS );
		}
		else if( pMutantItem )
		{
			iNeed = pMutantItem->GetNeedBullet();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_UROBORUS );
		}
		else if( pWereWolf )
		{
			iNeed = pWereWolf->GetNeedBullet();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_MOON, pWereWolf->IsWereWolfState() );
		}
		else if( pJoroExpansionItem )
		{
			iNeed = pJoroExpansionItem->GetNeedBullet();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_MOON );
		}
		else if( pChargeContact && pChargeContact->IsEnableExtraGauge() )
		{
			float fCurGauge = pChargeContact->GetCurExtraGauge();
			float fMaxGauge = pChargeContact->GetMaxExtraGauge();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_EXTRA, pChargeContact->IsUsedExtraGauge() );
			UpdateExtraGauge( fCurGauge, fMaxGauge );
		}
		else if( pWitchItem )
		{
			iNeed = pWitchItem->GetNeedBullet();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_WITCH, pWitchItem->IsWitchState() );
		}
		else if( pNakoruru )
		{
			iNeed = pNakoruru->GetNeedBullet();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_WITCH, pNakoruru->IsWitchState() );
		}
		else if( pMineWeapon )
		{
			iCur = iMax - g_WeaponMgr.GetCurMineWeaponCnt( pOwner, MIT_MINE );
			iCur = max( 0, iCur );
			UpdateBullet( iCur, iMax, 0, SkillListBtn::BIT_MINE );
		}
		else if( pAttackItem && iGraffitiMaxCnt > 0 )
		{
			iCur = iGraffitiMaxCnt - g_WeaponMgr.GetCurGraffitiWeaponCnt( pOwner );
			iCur = max( 0, iCur );
			UpdateBullet( iCur, iGraffitiMaxCnt, 0, SkillListBtn::BIT_MINE );
		}
		else if( pBuffAdd )
		{
			UpdateBullet( iCur, iMax, 0, SkillListBtn::BIT_HIDE );
		}
		else if( pRachelItem || pIljimaeItem || pTrinityItem || pMephistoItem )
		{
			UpdateBullet( iCur, iMax, 0, SkillListBtn::BIT_SYLPHID );
		}
		else if( pAlterEgoItem )
		{
			UpdateBullet( iCur, iMax, 0, SkillListBtn::BIT_ALTER_EGO );
		}
		else if( pExcalibur )
		{
			iNeed = pExcalibur->GetNeedBullet();
			float fCurGauge = pExcalibur->GetCurExtraGauge();
			float fMaxGauge = pExcalibur->GetMaxExtraGauge();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_EXCALIBUR );
			UpdateExtraGauge( fCurGauge, fMaxGauge );
		}
		else if( pWindLordItem )
		{
			iNeed = pWindLordItem->GetNeedBullet();
			float fCurGauge = pWindLordItem->GetCurExtraGauge();
			float fMaxGauge = pWindLordItem->GetMaxExtraGauge();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_EXCALIBUR );
			UpdateExtraGauge( fCurGauge, fMaxGauge );
		}
		else if( pLeeItem )
		{
			iNeed = pLeeItem->GetNeedBullet();
			float fCurGauge = pLeeItem->GetCurExtraGauge();
			float fMaxGauge = pLeeItem->GetMaxExtraGauge();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_EXTRA );
			UpdateExtraGauge( fCurGauge, fMaxGauge );
		}
		else if( pCuchulainItem )
		{
			iNeed = pCuchulainItem->GetNeedBullet();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_UROBORUS );
		}
		else if( pEarthQuakeItem )
		{
			iNeed = pEarthQuakeItem->GetNeedBullet();
			float fCurGauge = pEarthQuakeItem->GetCurExtraGauge();
			float fMaxGauge = pEarthQuakeItem->GetMaxExtraGauge();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_EXTRA );
			UpdateExtraGauge( fCurGauge, fMaxGauge );
		}
		else if( pSasinmuItem )
		{
			iNeed = pSasinmuItem->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_MECHANICS );
		}
		else if( pProjectKItem )
		{
			iNeed = pProjectKItem->GetNeedBullet();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_UROBORUS );
		}
		else if( pFlashItem )
		{
			iNeed = pFlashItem->GetNeedBullet();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_UROBORUS );
		}
		else if( pPsychoItem )
		{
			iNeed = pPsychoItem->GetNeedBullet();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_BEAM );
		}
		else if( pStrikerItem )
		{
			iNeed = pStrikerItem->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_BEAM );
		}
		else if( pKaelthasItem )
		{
			iNeed = pKaelthasItem->GetNeedBullet();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_BEAM );
		}
		else if( pSuperDimentionItem )
		{
			iNeed = pSuperDimentionItem->GetNeedBullet();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_BEAM );
		}
		else if( pTraceItem )
		{
			iNeed = pTraceItem->GetNeedBullet();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_UROBORUS );
		}
		else if( pSlasherItem )
		{
			iNeed = pSlasherItem->GetNeedBullet();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_UROBORUS );
		}
		else if( pShadowRangerItem )
		{
			iNeed = pShadowRangerItem->GetNeedBullet();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_UROBORUS );
		}
		else if( pMechanics )
		{
			iNeed = pMechanics->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_MECHANICS, pMechanics->IsMechanicsState() );
		}
		else if( pPhantom )
		{
			iNeed = pPhantom->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_MECHANICS, pPhantom->IsPhantomState() );
		}
		else if( pMichael )
		{
			iNeed = pMichael->GetNeedBullet();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_UROBORUS );
		}
		else if( pGoomiho )
		{
			iNeed = pGoomiho->GetNeedBullet();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_UROBORUS );
		}
		else if( pBladeItem )
		{
			iNeed = pBladeItem->GetNeedBullet();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_UROBORUS );
		}
		else if( pJackItem )
		{
			iNeed = pJackItem->GetNeedBullet();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_UROBORUS );
		}
		else if( pEvyItem )
		{
			iNeed = pEvyItem->GetNeedBullet();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_UROBORUS );
		}
		else if( pRedHoodItem )
		{
			iNeed = pRedHoodItem->GetNeedBullet();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_UROBORUS );
		}
		else if( pChargeCmdItem )
		{
			iNeed = pChargeCmdItem->GetNeedBullet();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_UROBORUS );
		}
		else if( pHakumenItem )
		{
			iNeed = pHakumenItem->GetNeedBullet();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_UROBORUS );
		}
		else if( pDraculaItem )
		{
			iNeed = pDraculaItem->GetNeedBullet();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_UROBORUS );
		}
		else if( pShaolinItem )
		{
			iNeed = pShaolinItem->GetNeedBullet();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_UROBORUS );
		}
		else if( pSuccubus )
		{
			iNeed = pSuccubus->GetNeedBullet();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_UROBORUS );
		}
		else if( pHades )
		{
			iNeed = pHades->GetNeedBullet();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_UROBORUS );
		}
		else if( pCrowItem )
		{
			iNeed = pCrowItem->GetNeedBullet();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_UROBORUS );
		}
		else if( pYuMiRa )
		{
			iNeed = pYuMiRa->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_MECHANICS, pYuMiRa->IsGPState() );
		}
		else if( pJinMoRi )
		{
			iNeed = pJinMoRi->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_MECHANICS, pJinMoRi->IsGPState() );
		}
		else if( pLucifer )
		{
			iNeed = pLucifer->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_MECHANICS, pLucifer->IsLuciferState() );
		}
		else if( pDummyCharItem2 )
		{
			iNeed = pDummyCharItem2->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_MECHANICS, pDummyCharItem2->GetMeshType() == ioDummyCharItem2::IMT_EXTRA  );
		}
		else if( pDevilItem )
		{
			iNeed = pDevilItem->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_MECHANICS, pDevilItem->IsDevilFlyState() );
		}
		else if( pMaidItem )
		{
			iNeed = pMaidItem->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_MECHANICS, pMaidItem->IsMaidState() );
		}
		else if( pDestroyer )
		{
			iNeed = pDestroyer->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_MECHANICS, pDestroyer->IsDestroyerState() );
		}
		else if( pPerterpan )
		{
			iNeed = pPerterpan->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_MECHANICS, pPerterpan->IsPeterpanState() );
		}
		else if( pCyvogueItem )
		{
			iNeed = pCyvogueItem->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_MECHANICS, pCyvogueItem->IsCyvogueState() );
		}
		else if( pDarkSeasItem )
		{
			iNeed = pDarkSeasItem->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_MECHANICS, pDarkSeasItem->IsDarkSeasState() );
		}
		else if( pPowerCharge || pPowerCharge4 || pRunCharge || pPentjakSilatItem || pZeus || pBowItem2 || pSamuraiExpansionItem )
		{
			UpdateBullet( iCur, iMax, 0, SkillListBtn::BIT_CHARGING );
		}
		else if( pFireItem || pAutoShot || pCrossBowItem )
		{
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_GUN );
		}
		else if( pBowItem )
		{
			if( pBowItem->IsChargeBulletType() )
			{
				iNeed = pBowItem->GetNeedBullet();
				UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_BEAM );
			}
			else
			{
				UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_GUN );
			}
		}
		else if( pChargeComboBowItem )
		{	
			iNeed = pChargeComboBowItem->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_GUN );
		}
		else if( pWestGunExtendItem )
		{	
			iNeed = pWestGunExtendItem->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_GUN );
		}
		else if( pSuddenAttackItem && pSuddenAttackItem->GetMeshType() == ioSuddenAttackItem::IMT_EXTRA1 )
		{
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_GUN );
		}
		else if( pGalaxy )
		{
			iNeed = pGalaxy->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_MECHANICS, pGalaxy->IsEquipObject( pOwner ) );
		}
		else if( pDragonSlayer )
		{
			iNeed = pDragonSlayer->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_GUN );
		}
		else if( pNangin )
		{
			iNeed = pNangin->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_MECHANICS, pNangin->GetSpecialState() == ioNanginItem::SS_TRACE_ATTACK ? true : false );
		}
		else if( pDonQuixote )
		{
			iNeed = pDonQuixote->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_MECHANICS );
		}
		else if( pThunderBird )
		{
			iNeed = pThunderBird->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_MECHANICS, pThunderBird->IsEquipObject( pOwner ) );
		}
		else if( pAlice )
		{
			iNeed = pAlice->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_MECHANICS, pAlice->IsEquipObject( pOwner ) );
		}
		else if( pValkyrie )
		{
			iNeed = pValkyrie->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_GUN );
		}
		else if( pParkilpyoItem )
		{
			iNeed = pParkilpyoItem->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_MECHANICS, pParkilpyoItem->IsEquipObject( pOwner ) );
		}
		else if( pDarkKnight )
		{
			iNeed = pDarkKnight->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_MECHANICS, pDarkKnight->IsEquipObject( pOwner ) );
		}
		else if( pJumper )
		{
			iNeed = pJumper->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_MECHANICS, pJumper->GetNeedBullet() < pJumper->GetCurBullet() );
		}
		else if( pEngineer )
		{
			iNeed = pEngineer->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_UROBORUS );
		}
		else if( pGeomje )
		{
			UpdateBullet( iCur, iMax, pGeomje->GetMaxBullet(), SkillListBtn::BIT_MECHANICS, pGeomje->IsSwordChangeState() == ioGeomjeItem::SCS_SWORD );
		}
		else if( pSaber )
		{
			iNeed = pSaber->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_MECHANICS );
		}
		else if( pOutBoxer )
		{
			iNeed = pOutBoxer->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_MECHANICS );
		}
		else if ( pMagicSwordItem )
		{
			iNeed = pMagicSwordItem->GetNeedBullet();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_UROBORUS );
		}
		else if( pNataItem )
		{
			iNeed = pNataItem->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_MECHANICS );
		}
		else if( pLeeSunSin )
		{
			iNeed = pLeeSunSin->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_MECHANICS, pLeeSunSin->GetNeedBullet() < pLeeSunSin->GetCurBullet() );
		}
		else if( pResearcher )
		{
			if( pResearcher->IsOverHeat() )
				UpdateBullet( iCur, iMax, INT_MAX, SkillListBtn::BIT_MECHANICS, false );
			else
				UpdateBullet( iCur, iMax, 0, SkillListBtn::BIT_MECHANICS, true );
		}
		else if( pBullFightItem )
		{
			iNeed = pBullFightItem->GetNeedBullet();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_UROBORUS );
		}
		else if( pSawang )
		{
			iNeed = pSawang->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_GUN );
		}
		else if( pChainMagicianItem )
		{
			iNeed = pChainMagicianItem->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_UROBORUS );
		}
		else if( pStrider )
		{
			iNeed = pStrider->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_MECHANICS );
		}
		else if( ApostateMonk )
		{
			iNeed = ApostateMonk->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_MECHANICS );
		}
		else if( pGanesa )
		{
			iNeed = pGanesa->GetNeedBullet();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_UROBORUS );
		}
		else if( pMir )
		{
			iNeed = pMir->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_GUN );
		}
		else if( pGalford )
		{
			iNeed = pGalford->GetNeedBullet();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_UROBORUS );
		}
		else if( pOniMushaItem )
		{
			iNeed = pOniMushaItem->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_MECHANICS, pOniMushaItem->IsEquipObject( pOwner ) );
		}
		else if( pDruid )
		{
			iNeed = pDruid->GetNeedBullet();
			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_MECHANICS, pDruid->IsEquipObject( pOwner ) );
		}
		else if( pPredatorItem )
		{
			iNeed = pPredatorItem->GetNeedBullet();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_UROBORUS );
		}
		else if( pBlackSpearItem )
		{
			UpdateBullet( iCur, iMax, 0, SkillListBtn::BIT_ALTER_EGO );
		}
		else if( pGalford )
		{
			iNeed = pGalford->GetNeedBullet();

			UpdateBullet( iCur, iMax, iNeed, SkillListBtn::BIT_UROBORUS );
		}
		else
			UpdateBullet( 0, 0, 0, SkillListBtn::BIT_GUN );
	}
}

void SkillListWnd::SetUpdateBulletExtend( ioBaseChar *pOwner )
{
}

void SkillListWnd::UpdateSkillList( ioBaseChar *pOwner )
{
	if( !pOwner )	return;

	for( int i=ID_WEAPON; i<ID_CLOAK + 1; i++ )
	{
		SkillListBtn *pSkillBtn = (SkillListBtn*)FindChildWnd( i );
		if( pSkillBtn )
		{
			pSkillBtn->UpdateItemSlotInfo( pOwner );
			pSkillBtn->UpdateSkillSlotInfo( pOwner );
			pSkillBtn->UpdateSkillKeySet();
			pSkillBtn->UpdateEnableEffect();
			pSkillBtn->UpdateCharState( pOwner->GetState() );
		}
	}
}

void SkillListWnd::UpdateGaugeInfo( int iSlot, int iMaxValue, int iCurValue, float fRecoveryValue, int iTic, int iExtendMax, bool bUse, bool bUsePassiveGauge /*= false*/ )
{
	SkillListBtn *pSkillBtn = (SkillListBtn*)FindChildWnd( iSlot );
	if( pSkillBtn )
	{
		pSkillBtn->UpdateGaugeInfo( iMaxValue, iCurValue, fRecoveryValue, iTic, iExtendMax, bUse, bUsePassiveGauge );
	}
}

void SkillListWnd::UpdateBullet( int iCur, int iMax, int iNeed, SkillListBtn::BulletIconType eBulletIconType, bool bWereWolf )
{
	SkillListBtn *pSkillBtn = (SkillListBtn*)FindChildWnd( ID_WEAPON );
	if( pSkillBtn )
	{
		pSkillBtn->UpdateBullet( iCur, iMax, iNeed, eBulletIconType, bWereWolf );
	}
}

void SkillListWnd::UpdateSecoundBullet( int iCur, int iMax, int iNeed, SkillListBtn::BulletIconType eBulletIconType )
{
	SkillListBtn *pSkillBtn = (SkillListBtn*)FindChildWnd( ID_WEAPON );
	if( pSkillBtn )
	{
		pSkillBtn->UpdateSecoundBullet( iCur, iMax, iNeed, eBulletIconType );
	}
}

void SkillListWnd::UpdateExtraGauge( float fCur, float fMax )
{
	SkillListBtn *pSkillBtn = (SkillListBtn*)FindChildWnd( ID_WEAPON );
	if( pSkillBtn )
	{
		pSkillBtn->UpdateExtraGauge( fCur, fMax );
	}
}

void SkillListWnd::UpdateActiveCount( ioBaseChar *pOwner, int iSlotType )
{
	SkillListBtn *pSkillBtn = (SkillListBtn*)FindChildWnd( iSlotType );
	if( !pSkillBtn )
	{
		ResetActiveCount( iSlotType );
		return;
	}

	bool bDisable = false;

	// 오브젝트 아이템 처리
	ioObjectItem *pObject = pOwner->GetObject();
	ioItem *pCurItem = pOwner->GetEquipedItem( iSlotType );

	if( pObject && pCurItem )
	{
		ioBuff *pBuff = pOwner->GetUseActiveCountInBuff();
		ioHashString szBuffCreateItem;
		if ( pBuff && pBuff->GetOwnerItem() )
			szBuffCreateItem = pBuff->GetOwnerItem()->GetName();
		if( pBuff && szBuffCreateItem == pCurItem->GetName() && pObject->GetObjectCreateItem() == pCurItem->GetName() )
		{
			int iCurCount = pBuff->GetCurActiveCount();
			int iMaxCount = pBuff->GetMaxActiveCount();

			pSkillBtn->UpdateActiveCount( iCurCount, iMaxCount, iSlotType );
			UpdateActiveCountType(iSlotType, SkillListBtn::DACT_ONE_TEXT, bDisable );
			return;
		}
	}

	// 버프 / 버프 스킬 처리
	ioSkill *pEquipSkill = pOwner->GetEquipedSkill( iSlotType );
	if( pEquipSkill )
	{
		if( pEquipSkill->CheckUseBuff( pOwner ) || pEquipSkill->CheckUseActiveCount() )
		{
			ioBuff *pBuff = pOwner->GetUseActiveCountInBuff( pEquipSkill->GetName() );
			if( pBuff && pBuff->UseActiveCount() )
			{
				int iCurCount = pBuff->GetCurActiveCount();
				int iMaxCount = pBuff->GetMaxActiveCount();

				UpdateActiveCountType(iSlotType, SkillListBtn::DACT_ONE_TEXT, bDisable );
				pSkillBtn->UpdateActiveCount( iCurCount, iMaxCount, iSlotType );
				return;
			}

			ioFlyJumpExtraAttackBuff *pFlyJumpExtraAttackBuff = ToFlyJumpExtraAttackBuff( pOwner->GetBuff( BT_FLY_JUMP_EXTRA_ATTACK ) );
			if ( pFlyJumpExtraAttackBuff && pFlyJumpExtraAttackBuff->GetSkillName() == pEquipSkill->GetName() )
			{
				int iCurCount = pFlyJumpExtraAttackBuff->GetCurCnt();
				int iMaxCount = pFlyJumpExtraAttackBuff->GetMaxCnt();

				UpdateActiveCountType(iSlotType, SkillListBtn::DACT_ONE_TEXT, bDisable );
				pSkillBtn->UpdateActiveCount( iCurCount, iMaxCount, iSlotType );
				return;
			}
		}
		
		if ( pEquipSkill->IsUseActiveCnt() )
		{
			int nMaxActiveCnt = pEquipSkill->GetMaxActiveCnt( pOwner );			
			int nCurCnt = nMaxActiveCnt - pEquipSkill->GetCurActiveCnt( pOwner );

			UpdateActiveCountType(iSlotType, SkillListBtn::DACT_ONE_TEXT, bDisable );
			pSkillBtn->UpdateActiveCount( nCurCnt, nMaxActiveCnt, iSlotType );
			return;
		}

		ioMineSkill *pMineSkill = ToMine( pEquipSkill );
		if( pMineSkill )
		{
			int iMineType = pMineSkill->GetMineType();
			int iMaxMineCnt = pMineSkill->GetMaxMineCnt();
			int iCurMineCnt = g_WeaponMgr.GetCurMineWeaponCnt( pOwner, (MineType)iMineType );

			if( iMineType > 0 )
			{
				int iNewCurCnt = iCurMineCnt;
				if( iMineType == MIT_TRACE )
					iNewCurCnt = iMaxMineCnt - iCurMineCnt;
				else if( iMineType == MIT_TRAP2 )
					iNewCurCnt = iMaxMineCnt - iCurMineCnt;
				else if( iMineType == MIT_AIR )
					iNewCurCnt = iMaxMineCnt - iCurMineCnt;

				UpdateActiveCountType(iSlotType, SkillListBtn::DACT_ONE_TEXT, bDisable );
				pSkillBtn->UpdateActiveCount( iNewCurCnt, iMaxMineCnt, iSlotType );
				return;
			}
		}

		ioCountSkill *pCountSkill = ToCountSkill( pEquipSkill );
		if( pCountSkill )
		{
			int iCurCount = pCountSkill->GetCurUseCount();
			int iMaxCount = pCountSkill->GetMaxUseCount();
			int iNeedCount = pCountSkill->GetNeedCount();

			if( iCurCount < iNeedCount )
				bDisable = true;
			else
				bDisable = false;

			UpdateActiveCountType(iSlotType, SkillListBtn::DACT_TWO_TEXT, bDisable );
			pSkillBtn->UpdateActiveCount( iCurCount, iMaxCount, iSlotType );
			return;
		}
	}

	// 사용중인 스킬 처리
	ioSkill *pActiveSkill = pOwner->GetCurActiveSkill();
	if( pActiveSkill && pEquipSkill && pActiveSkill == pEquipSkill )
	{
		ioConsecution3Skill* pCon3 = ToConsecution3Skill( pActiveSkill );
		if( pCon3 )
		{
			int iCurEquip = (int)pCon3->GetOwerItemEquipSlot();			

			if( iSlotType == iCurEquip )
			{
				int iDisableCnt = pCon3->GetChangeUIOverHeatCnt();
				int iCurCount = (int)pCon3->GetCurOverHeatCnt();
				int iMaxCount = (int)pCon3->GetMaxOverHeatCnt();

				if( iCurCount < iDisableCnt )
					bDisable = true;
				else
					bDisable = false;

				UpdateActiveCountType( iSlotType, SkillListBtn::DACT_CONSECUTION3, bDisable );
				pSkillBtn->UpdateActiveCount( iCurCount, iMaxCount, iSlotType );
				return;
			}
		}

		ioBlindlyShotSkill* pBlindly = ToBlindlyShotSkill( pActiveSkill );
		if( pBlindly && !pBlindly->GetNotUseActiveCount() )
		{
			int iCurEquip = (int)pBlindly->GetOwerItemEquipSlot();

			if( iSlotType == iCurEquip )
			{
				int iCurCount = pBlindly->GetCurBulletCnt();
				int iMaxCount = pBlindly->GetMaxBulletCnt();
				UpdateActiveCountType( iSlotType, SkillListBtn::DACT_TWO_TEXT, bDisable );
				pSkillBtn->UpdateActiveCount( iCurCount, iMaxCount, iSlotType );
				return;
			}
		}

		ioEngineerMachineGunAttack* pMachine = ToEngineerMachineGunAttack( pActiveSkill );
		if( pMachine )
		{
			int iCurEquip = (int)pMachine->GetOwerItemEquipSlot();
			if( iSlotType == iCurEquip )
			{
				UpdateActiveCountType( iSlotType, SkillListBtn::DACT_TWO_TEXT, bDisable );
				pSkillBtn->UpdateActiveCount( pMachine->GetCurBullet(), pMachine->GetMaxBullet(), iSlotType );
				return;
			}
		}
		
		if( pActiveSkill->UseActiveCount( pOwner ) )
		{
			int iCurCount = pActiveSkill->GetCurActiveCount( pOwner );
			int iMaxCount = pActiveSkill->GetMaxActiveCount( pOwner );
			UpdateActiveCountType( iSlotType, SkillListBtn::DACT_ONE_TEXT, bDisable );				
			pSkillBtn->UpdateActiveCount( iCurCount, iMaxCount, iSlotType );
			return;
		}
	}

	ResetActiveCount( iSlotType );
}

void SkillListWnd::UpdateActiveCountType( int iSlotType, SkillListBtn::DrawActiveCountType iType, bool bRedUI )
{
	SkillListBtn *pSkillBtn = (SkillListBtn*)FindChildWnd( iSlotType );
	if( pSkillBtn )
	{
		pSkillBtn->UpdateActiveCountType( iType, bRedUI );
	}	
}

void SkillListWnd::ResetActiveCount( int iSlotType )
{
	SkillListBtn *pSkillBtn = (SkillListBtn*)FindChildWnd( iSlotType );
	if( pSkillBtn )
	{
		pSkillBtn->ResetActiveCount( iSlotType );
	}
}

void SkillListWnd::iwm_lbuttonup( const ioMouse& mouse )
{
	if( !m_MoveWnd.MouseLUp( mouse.GetMousePos() ) )
	{
#ifndef JUNGI_WORK
		// CharIcon
		SkillCharIcon *pSkillIcon = dynamic_cast<SkillCharIcon*> (FindChildWnd( ID_CHAR_ICON ));
		if( pSkillIcon && pSkillIcon->IsInWndRect( mouse.GetMousePos() ) )
		{
			pSkillIcon->SetAcion();
		}

		// + Btn
		SkillCharBtn *pBtn = dynamic_cast<SkillCharBtn*> (FindChildWnd( ID_ICON_BTN ));
		if( pBtn && pBtn->IsInWndRect( mouse.GetMousePos() )  )
		{
			pBtn->SetAcion();
		}
#endif
	}

	ioWnd::iwm_lbuttonup( mouse );
}

bool SkillListWnd::SkipRender()
{
	if( g_GUIMgr.IsSkipUIByBlind() ) return true;

	if( m_ShowAniState.GetCurState() == SAT_DELAY || 
		m_ShowAniState.GetCurState() == SAT_START_ROUND )
		return true;
	if( m_eOwnerState == CS_DIE || 
		m_eOwnerState == CS_READY ||
		m_eOwnerState == CS_VIEW ||
		m_eOwnerState == CS_OBSERVER )
		return true;
	return false;
}

ioUIRenderImage *SkillListWnd::GetKeyImageSet( const ioHashString &szImageFullName )
{
	KeyImageSetMap::iterator iter = m_KeyImageSetMap.find( szImageFullName );
	if( iter != m_KeyImageSetMap.end() )
		return iter->second;
	else 
	{
		ioUIRenderImage *pKeySetImage = g_UIImageSetMgr.CreateImageByFullName( szImageFullName );
		if( pKeySetImage )
			m_KeyImageSetMap.insert( KeyImageSetMap::value_type( szImageFullName, pKeySetImage ) );
		
		return pKeySetImage;
	}
	return NULL;
}

void SkillListWnd::RenderWnd()
{
	if( !IsShow() )	return;

	OnRender();

	if( HasChild() )
	{
		m_bNowGoToTopReserving = true;

		ioWnd *pWnd = NULL;
		// 스킬 아이콘
		for ( int i=0; i < MAX_SLOT; ++i )
		{
			pWnd = FindChildWnd( i );
			if( pWnd )
			{
				pWnd->RenderWnd();
			}
		}

#ifndef JUNGI_WORK
		// 캐릭터 아이콘
		pWnd = FindChildWnd( ID_CHAR_ICON );
		if( pWnd )
			pWnd->RenderWnd();

		// 아이콘 버튼
		pWnd = FindChildWnd( ID_ICON_BTN );
		if( pWnd )
			pWnd->RenderWnd();
#endif
		// HARDCODE : ID_TOOL_TIP는 RenderAfterChild 시점에 한다

		m_bNowGoToTopReserving = false;
		ExecuteReservedChildGoToTop();
	}

	OnRenderAfterChild();
}

void SkillListWnd::OnRender()
{
	if( SkipRender() ) return;

	ioWnd::OnRender();
}

void SkillListWnd::OnRenderAfterChild()
{
	if( !IsShow() ) return;
	if( SkipRender() ) return;

#ifndef JUNGI_WORK
	RenderTimeGrowthToolTip();

	if( m_iCurOverWnd == ID_CHAR_ICON || m_iCurOverWnd == ID_ICON_BTN )
	{
		// Char ToolTip
		if( HasChild() )
		{
			m_bNowGoToTopReserving = true;
			m_bNowGoToTopReserving = false;
			ExecuteReservedChildGoToTop();
		}
	}
#endif

	if( HasChild() )
	{
		m_bNowGoToTopReserving = true;

		ioWnd *pWnd = NULL;
		pWnd = FindChildWnd( ID_TOOL_TIP );
		if( pWnd )
			pWnd->RenderWnd();

		m_bNowGoToTopReserving = false;
		ExecuteReservedChildGoToTop();
	}
}

void SkillListWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;

	if( !g_GUIMgr.IsSkipUIByBlind() && m_ShowAniState.GetCurState() == SAT_NONE )
		m_ShowAniState.ChangeState( SAT_END  );

	if( !g_GUIMgr.IsSkipUIByBlind() && m_ShowAniState.GetCurState() == SAT_START_ROUND )
	{
		if( g_TutorialMgr.IsShowSkillStep() )
		{
			m_ShowAniState.ChangeState( SAT_DELAY );
			g_SoundMgr.PlaySound( m_szAniSound );
		}
	}

	if( g_GUIMgr.IsSkipUIByBlind() && m_ShowAniState.GetCurState() == SAT_END )
		m_ShowAniState.ChangeState( SAT_NONE );

	m_ShowAniState.UpdateTime();

	if( m_ShowAniState.GetCurState() == SAT_UP )
	{
		enum { PLUS_Y_POS = 75 };
		int iStartYPos = PLUS_Y_POS + ( Setting::Height() - ( m_iDefaultY + GetHeight() ) );
		int iCurYpos = m_iDefaultY + ( cos( (FLOAT_PI * FLOAT05 * m_ShowAniState.GetCurTime()  ) / m_ShowAniState.GetDelay() ) * iStartYPos );	
		SetWndPos( m_iDefaultX, iCurYpos );
	}
	else if( m_ShowAniState.GetCurState() == SAT_DOWN )
	{
		enum { MAX_OFFSET_YPOS = 11 };
		int iCurYpos = m_iDefaultY + ( sin( (FLOAT_PI * FLOAT05 * m_ShowAniState.GetCurTime()  ) / m_ShowAniState.GetDelay() ) * MAX_OFFSET_YPOS );	
		SetWndPos( m_iDefaultX, iCurYpos );
	}
	else if( m_ShowAniState.GetCurState() == SAT_UP_STOP )
	{
		enum { MAX_OFFSET_YPOS = 11 };
		int iCurYpos = m_iDefaultY + ( cos( (FLOAT_PI * FLOAT05 * m_ShowAniState.GetCurTime()  ) / m_ShowAniState.GetDelay() ) * MAX_OFFSET_YPOS );	
		SetWndPos( m_iDefaultX, iCurYpos );
	}
	else if( m_ShowAniState.GetCurState() == SAT_STOP )
	{
		SetWndPos( m_iDefaultX, m_iDefaultY );
		m_ShowAniState.ChangeState( SAT_END );
	}

	UpdateDefaultPos();

	ProcessMouseOver();

#ifndef JUNGI_WORK
	int iClassType = g_MyInfo.GetClassType();
	ioGrowthLevel *pLevel = g_MyInfo.GetGrowthLevel();
	if( pLevel )
	{
		bool bExist = false;
		if( pLevel->FindTimeGrowthInfo(iClassType) )
			bExist = true;

		int iCurPoint = pLevel->GetCharGrowthPoint( iClassType );
		if( g_MyInfo.IsCharExerciseStyleToClassType( iClassType, EXERCISE_RENTAL ) )
			iCurPoint = 0;
		if( iCurPoint <= 0 && !bExist )
			HideChildWnd( ID_ICON_BTN );
		else
			ShowChildWnd( ID_ICON_BTN );
	}
	else
	{
		HideChildWnd( ID_ICON_BTN );
	}
#endif
	ioMovingWnd::OnProcess( fTimePerSec );
}

void SkillListWnd::ProcessMouseOver()
{
	ioLSMouse *pMouse = g_App.GetMouse();

	POINT pt;
	pt.x = pMouse->GetMousePos().x;
	pt.y = pMouse->GetMousePos().y;

	ioWnd *pWnd = NULL;
	bool bSkillOver = false;
	if( pMouse && pMouse->IsMouseShowing() )
	{
		for (int i = 0; i < MAX_SLOT ; i++)
		{
			pWnd = FindChildWnd( i );
			if( pWnd )
			{
				if( pWnd->IsInWndRect( pt ) && m_iClassTypes[i] != 0 )
				{
					if( m_iCurOverWnd != pWnd->GetID() )
					{
						//g_ModeHelpMgr.ShowSkillHelp( m_kSkillDesc[i].m_szDescName, m_kSkillDesc[i].m_szDesc, m_kSkillDesc[i].m_szIconName );
						m_iCurOverWnd = pWnd->GetID();
					}

					bSkillOver = true;
					break;
				}
			}
		}
	}

	if( !bSkillOver )
		m_iCurOverWnd = -1;

	// Skill Over
	ioComplexStringPrinter kPrinter[GLOBAL_TEXT_TOOLTIP_LINE];
	if( COMPARE( m_iCurOverWnd, ID_WEAPON, MAX_SLOT ) )
	{
		SkillListBtn *pSkillBtn = (SkillListBtn*)FindChildWnd( m_iCurOverWnd );
		if( pSkillBtn )
		{
			kPrinter[0].SetTextStyle( TS_NORMAL );
			kPrinter[0].SetBkColor( 0, 0, 0 );
			kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
			if( m_kSkillDesc[m_iCurOverWnd].m_szDescFullName.IsEmpty() )
				kPrinter[0].AddTextPiece( FONT_SIZE_12, STR(1), m_kSkillDesc[m_iCurOverWnd].m_szDescName.c_str() );
			else
				kPrinter[0].AddTextPiece( FONT_SIZE_12, STR(1), m_kSkillDesc[m_iCurOverWnd].m_szDescFullName.c_str() );

			kPrinter[1].SetTextStyle( TS_NORMAL );
			kPrinter[1].SetBkColor( 0, 0, 0 );
			kPrinter[1].SetTextColor( TCT_DEFAULT_GRAY );
			if( !m_kSkillDesc[m_iCurOverWnd].m_szDesc.IsEmpty() )
				kPrinter[1].AddTextPiece( FONT_SIZE_12, STR(2), m_kSkillDesc[m_iCurOverWnd].m_szDesc.c_str() );
		}
	}

	GlobalTextTooltip *pTooltip = dynamic_cast<GlobalTextTooltip*>(FindChildWnd( ID_TOOL_TIP ));
	if( pTooltip )
	{
		pTooltip->SetTooltipText( kPrinter );
	}

#ifndef JUNGI_WORK
	// CharIcon
	pWnd = FindChildWnd( ID_CHAR_ICON );
	if( pWnd && pWnd->IsInWndRect( pt ) )
	{
		m_iCurOverWnd = pWnd->GetID();

		SkillCharIcon *pCharIcon = (SkillCharIcon*)FindChildWnd( ID_CHAR_ICON );
		if( pCharIcon )
		{
			if( pMouse && pMouse->IsMouseShowing() )
				pMouse->SetToolTipID( g_MyInfo.GetPublicID(), UserInfoToolTip::TT_ROOM );
		}
	}

	// + Btn
	pWnd = FindChildWnd( ID_ICON_BTN );
	if( pWnd && pWnd->IsInWndRect( pt ) )
	{
		m_iCurOverWnd = pWnd->GetID();

		SkillCharIcon *pCharIcon = (SkillCharIcon*)FindChildWnd( ID_CHAR_ICON );
		if( pCharIcon )
		{
			if( pMouse && pMouse->IsMouseShowing() )
				pMouse->SetToolTipID( g_MyInfo.GetPublicID(), UserInfoToolTip::TT_ROOM );
		}
	}
#endif
}

int SkillListWnd::GetCurOverID()
{
	return m_iCurOverWnd;
}

void SkillListWnd::InitShowAniState()
{
	enum { DELAY_DELAY = 165, DELAY_UP = 198, DELAY_DOWN = 66, DELAY_UP_STOP = 66, };

	m_ShowAniState.SetCurState( SAT_NONE );

	FSMState *pState = new FSMState(SAT_NONE);
	if( pState )
	{
		pState->Add( SAT_DELAY,  SAT_DELAY );
		pState->Add( SAT_START_ROUND, SAT_START_ROUND );
		pState->Add( SAT_END,  SAT_END );
	}
	m_ShowAniState.Add( pState );

	pState = new FSMState(SAT_START_ROUND);
	if( pState )
		pState->Add( SAT_DELAY,  SAT_DELAY);
	m_ShowAniState.Add( pState );	

	pState = new FSMState(SAT_DELAY);
	if( pState )
		pState->Add( SAT_DELAY,  SAT_UP , DELAY_DELAY);
	m_ShowAniState.Add( pState );	

	pState = new FSMState(SAT_UP);
	if( pState )
		pState->Add( SAT_UP,  SAT_DOWN , DELAY_UP);
	m_ShowAniState.Add( pState );

	pState = new FSMState(SAT_DOWN);
	if( pState )
		pState->Add( SAT_DOWN,  SAT_UP_STOP , DELAY_DOWN );
	m_ShowAniState.Add( pState );

	pState = new FSMState(SAT_UP_STOP);
	if( pState )
		pState->Add( SAT_UP_STOP,  SAT_STOP , DELAY_UP_STOP );
	m_ShowAniState.Add( pState );

	pState = new FSMState(SAT_STOP);
	if( pState )
		pState->Add( SAT_END,  SAT_END );
	m_ShowAniState.Add( pState );

	pState = new FSMState(SAT_END);
	if( pState )
	{
		pState->Add( SAT_NONE,  SAT_NONE );
		pState->Add( SAT_START_ROUND, SAT_START_ROUND );
	}
	m_ShowAniState.Add( pState );
}

void SkillListWnd::iwm_create()
{
	int iXPos = g_GUIMgr.GUIPosLoadInt( "SkillListWnd", "XPos", GetXPos() ,POS_SAVE_VERSION );
	int iYPos = g_GUIMgr.GUIPosLoadInt( "SkillListWnd", "YPos", GetYPos() ,POS_SAVE_VERSION );

	if( COMPARE( iXPos, 0, Setting::Width() ) && 
		COMPARE( iYPos, 0, Setting::Height() ) )
	{
		SetWndPos( iXPos, iYPos );
	}

	m_iDefaultX = GetXPos();
	m_iDefaultY = GetYPos();
}

int SkillListWnd::GetCurState() const
{
	return m_ShowAniState.GetCurState();
}

void SkillListWnd::ChangeShowAniState( ShowAniType eShowAniType )
{
	m_ShowAniState.ChangeState( (int) eShowAniType );
}

void SkillListWnd::UpdateDefaultPos()
{
	if(  g_GUIMgr.IsDragMove( m_dwID )  || IsMoving() )
	{
		if( m_iDefaultX != GetXPos() )
		{
			if( COMPARE( GetXPos(), 0, (Setting::Width() - GetWidth() ) + 1 ))
				m_iDefaultX = GetXPos();
		}

		if( m_iDefaultY != GetYPos() )
		{
			if( COMPARE( GetYPos(), 0, (Setting::Height() - GetHeight() ) + 1 ))
				m_iDefaultY = GetYPos();
		}
	}
}

void SkillListWnd::SetPositionToDefault( int iParentWidth, int iParentHeight, bool bChildMove, bool bCreate /*= false */ )
{
	float fPosGap = ( Setting::Height() - 600.0f ) / FLOAT10;

	int iTempDefYPos = m_iDefYPos;

	m_iDefYPos = iTempDefYPos - fPosGap;

	ioMovingWnd::SetPositionToDefault( iParentWidth, iParentHeight, bChildMove, bCreate );

	m_iDefYPos = iTempDefYPos;

	if( !bCreate )
	{
		m_iDefaultX = GetXPos();
		m_iDefaultY = GetYPos();
		SetWndPos( m_iDefaultX, m_iDefaultY );
	}
}

void SkillListWnd::ParseExtraInfo( ioXMLElement &xElement )
{
	m_szAniSound = xElement.GetStringAttribute_e( "AniSound" );
	m_dwToolTipShowTime = xElement.GetIntAttribute_e( "ShowTime" );
	m_dwToolTipRefreshTime = xElement.GetIntAttribute_e( "RefreshTime" );
}

void SkillListWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "ToolTipCenter")
	{
		SAFEDELETE( m_pToolTipCen );
		m_pToolTipCen = pImage;
	}
	else if( szType == "ToolTipLMiddle" )
	{
		SAFEDELETE( m_pToolTipLMid );
		m_pToolTipLMid = pImage;
	}
	else if( szType == "ToolTipRMiddle" )
	{
		SAFEDELETE( m_pToolTipRMid );
		m_pToolTipRMid = pImage;
	}
	else if( szType == "ToolTipLeft" )
	{
		SAFEDELETE( m_pToolTipLeft );
		m_pToolTipLeft = pImage;
	}
	else if( szType == "ToolTipRight" )
	{
		SAFEDELETE( m_pToolTipRight );
		m_pToolTipRight = pImage;
	}
	else if( szType == "PointImage" )
	{
		SAFEDELETE( m_pPointImage );
		m_pPointImage = pImage;
	}
	else
	{
		ioWnd::AddRenderImage( szType, pImage );
	}
}

void SkillListWnd::RenderTimeGrowthToolTip()
{
	SkillCharIcon *pIcon = dynamic_cast<SkillCharIcon*>(FindChildWnd(ID_CHAR_ICON));
	if( !pIcon ) return;

	SkillCharBtn *pBtn = dynamic_cast<SkillCharBtn*> (FindChildWnd( ID_ICON_BTN ));
	if( !pBtn ) return;

	int iXPos, iYPos;
	iXPos = pBtn->GetDerivedPosX() + 45;
	iYPos = pBtn->GetDerivedPosY() - 39;

	ioLSMouse *pMouse = g_App.GetMouse();
	if( pMouse && pMouse->IsMouseShowing() )
	{
		if( m_iCurOverWnd == ID_CHAR_ICON || m_iCurOverWnd == ID_ICON_BTN )
			return;
	}

	int iLevel = 0;
	int iPoint = 0;
	int iTimeSlot = 0;
	bool bExist = false;
	int iClassType = pIcon->GetClassType();

	ioGrowthLevel *pGrowthLevel = g_MyInfo.GetGrowthLevel();
	if( !pGrowthLevel ) return;

	char szText1[MAX_PATH] = "";
	char szText2[MAX_PATH] = "";

	ioComplexStringPrinter kPrinter;
	kPrinter.SetTextStyle( TS_NORMAL );
	kPrinter.SetBkColor( 0, 0, 0 );

	int iTextSize = 0;
	TimeGrowthInfo *pInfo = pGrowthLevel->FindTimeGrowthInfo( iClassType );
	if( pInfo )
	{
		bExist = true;
		iTimeSlot = pInfo->m_iTimeSlot;
		iLevel = pGrowthLevel->GetGrowthLevelByTimeSlot( iClassType, iTimeSlot ) + 1;

		int iGapSec = pGrowthLevel->GetTimeGrowthRemainTime( iClassType );

		// 항목 레벨
		ioHashString szGrowthName;
		Help::GetGrowthTitle( iTimeSlot, true, szGrowthName );

		// 남은 시간
		char szTime[MAX_PATH] = "";
		ZeroMemory( szTime, sizeof( szTime ) );
		enum { DAY_SEC = 86400, HOUR_SEC = 3600, MINUTE_SEC = 60, };

		int iDay    = iGapSec/DAY_SEC;
		int iHourSec= iGapSec-(iDay*DAY_SEC);
		int iHour   = iHourSec / HOUR_SEC;
		int iMinute = ( iHourSec-(iHour*HOUR_SEC) ) / MINUTE_SEC;
		int iTotalMin = iGapSec / MINUTE_SEC;

		if( iDay <= 0 && iHour <= 0 && iMinute <= 0 )
			SafeSprintf( szTime, sizeof( szTime ), STR(2) );
		else if( iDay <= 0 && iHour <= 0 && iMinute > 0 )
			SafeSprintf( szTime, sizeof( szTime ), STR(3), iMinute );
		else if( iDay <= 0 && iHour > 0 )
			SafeSprintf( szTime, sizeof( szTime ), STR(4), iHour );
		else if( iDay > 0 && iHour <= 0 )
			SafeSprintf( szTime, sizeof( szTime ), STR(5),  iDay );
		else 
			SafeSprintf( szTime, sizeof( szTime ), STR(6), iDay, iHour );

		kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
		kPrinter.AddTextPiece( FONT_SIZE_13, STR(1), szGrowthName.c_str(), iLevel );
		kPrinter.SetTextColor( TCT_DEFAULT_RED );

		if( iTotalMin > 1 )
			kPrinter.AddTextPiece( FONT_SIZE_13, STR(7), iTotalMin );
		else
			kPrinter.AddTextPiece( FONT_SIZE_13, STR(8) );

		// 텍스트 + 여분
		iTextSize = kPrinter.GetFullWidth() + 20;
	}
	else
	{
		// 남은 포인트
		iPoint = pGrowthLevel->GetCharGrowthPoint( iClassType );
		if( g_MyInfo.IsCharExerciseStyleToClassType( iClassType, EXERCISE_RENTAL ) )
			iPoint = 0;
		if( iPoint <= 0 ) return;

		kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
		kPrinter.AddTextPiece( FONT_SIZE_13, STR(9) );
		kPrinter.SetTextColor( TCT_DEFAULT_RED );
		kPrinter.AddTextPiece( FONT_SIZE_13, STR(10), iPoint );

		// 텍스트 + 갭 + 이미지 + 여분
		iTextSize = kPrinter.GetFullWidth() + FLOAT1 + 12 + 20;
	}

	
	// 출력 시간 체크
	DWORD dwCurTime = FRAMEGETTIME();
	if( m_dwTimeGrowthToolTipStartTime <= 0 && m_dwTimeGrowthToolTipRefreshTime <= 0 )
		return;

	if( m_dwTimeGrowthToolTipStartTime <= 0 )
	{
		if( m_dwTimeGrowthToolTipRefreshTime > 0 && m_dwTimeGrowthToolTipRefreshTime < dwCurTime )
		{
			m_dwTimeGrowthToolTipStartTime = m_dwTimeGrowthToolTipRefreshTime;
			m_dwTimeGrowthToolTipRefreshTime = 0;
		}
	}

	if( m_dwTimeGrowthToolTipStartTime+m_dwToolTipShowTime < dwCurTime )
	{
		m_dwTimeGrowthToolTipStartTime = 0;
		m_dwTimeGrowthToolTipRefreshTime = 0;

		// 개인본부/길드본부/훈련소/광장이면...
		if( m_eModeType == MT_MYROOM || m_eModeType == MT_TRAINING || m_eModeType == MT_HEADQUARTERS || m_eModeType == MT_HOUSE )
			m_dwTimeGrowthToolTipRefreshTime = dwCurTime + m_dwToolTipRefreshTime;

		return;
	}

	enum
	{
		BACK_HEIGHT = 38,
	};

	int iNewXPos = iXPos;
	int iNewYPos = iYPos+10;
	if( iTextSize > 0 && m_pToolTipCen && m_pToolTipLMid && m_pToolTipRMid && m_pToolTipLeft && m_pToolTipRight )
	{
		int iHalfSize = iTextSize / 2;
		int iCenterSize = m_pToolTipCen->GetWidth();

		iNewXPos = iXPos - iHalfSize;
		int iMidSize = (iTextSize - (iCenterSize+16)) / 2;

		//Left
		int iCurPos = iNewXPos - 7;
		m_pToolTipLeft->Render( iCurPos, iYPos-6 );

		//Left Mid
		iCurPos = iNewXPos + 8;
		m_pToolTipLMid->SetSize( iMidSize, BACK_HEIGHT );
		m_pToolTipLMid->SetHorzFormatting( ioUIRenderImage::HF_STRETCH );
		m_pToolTipLMid->Render( iCurPos, iYPos-6 );

		//Center
		iCurPos += iMidSize;
		m_pToolTipCen->Render( iCurPos, iYPos-6 );

		//Right Mid
		iCurPos += iCenterSize;
		m_pToolTipRMid->SetSize( iMidSize, BACK_HEIGHT );
		m_pToolTipRMid->SetHorzFormatting( ioUIRenderImage::HF_STRETCH );
		m_pToolTipRMid->Render( iCurPos, iYPos-6 );

		//Right
		iCurPos += iMidSize;
		m_pToolTipRight->Render( iCurPos, iYPos-6 );
	}

	if( !kPrinter.IsEmpty() )
	{
		if( bExist )
		{
			kPrinter.PrintFullText( iXPos, iNewYPos, TAT_CENTER );
		}
		else
		{
			int iRealTextSize = kPrinter.GetFullWidth();
			int iTotalTextSize = iRealTextSize + 1 + 12;
			int iTextHalf = iTotalTextSize / 2;

			iNewXPos = iXPos - iTextHalf;
			kPrinter.PrintFullText( iNewXPos, iNewYPos, TAT_LEFT );

			iNewXPos += (iRealTextSize+1);
			if( m_pPointImage )
				m_pPointImage->Render( iNewXPos, iNewYPos+1 );
		}
	}
}

void SkillListWnd::SetAvata( const CHARACTER &rkInfo )
{
#ifndef JUNGI_WORK
	SkillCharIcon *pWnd = dynamic_cast<SkillCharIcon*> (FindChildWnd( ID_CHAR_ICON ) );
	if( pWnd )
		pWnd->SetAvata( rkInfo );
#endif
}

void SkillListWnd::CheckTimeGrowthToolTip()
{
#ifndef JUNGI_WORK
	m_dwTimeGrowthToolTipStartTime = FRAMEGETTIME();
	m_dwTimeGrowthToolTipRefreshTime = 0;
#endif
}

void SkillListWnd::DestroyAvata()
{
#ifndef JUNGI_WORK
	SkillCharIcon *pWnd = dynamic_cast<SkillCharIcon*> (FindChildWnd( ID_CHAR_ICON ) );
	if( pWnd )
		pWnd->DestroyAvata();
#endif
}

void SkillListWnd::AvataFaceChange( const char *szTexPostfix, int iDuration )
{
#ifndef JUNGI_WORK
	SkillCharIcon *pWnd = dynamic_cast<SkillCharIcon*> (FindChildWnd( ID_CHAR_ICON ) );
	if( pWnd )
		pWnd->AvataFaceChange( szTexPostfix, iDuration );
#endif
}

void SkillListWnd::InterruptUpdate( bool bCharCahnge /*= false */ )
{
	for( int i=ID_WEAPON; i<ID_CLOAK + 1; i++ )
	{
		SkillListBtn *pSkillBtn = (SkillListBtn*)FindChildWnd( i );
		if( pSkillBtn )
		{
			pSkillBtn->DeleteSkillInfo( bCharCahnge );
		}
	}
}

