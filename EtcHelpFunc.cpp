

#include "stdafx.h"

#include "../io3DEngine/ioModelParticleIterator.h"
#include "../io3DEngine/ioModelParticle.h"

#include "ioNpcChar.h"

#include <shldisp.h> // void MinimizeAllWindows(BOOL bMinimize)

#include <ntddndis.h>
#include <iphlpapi.h>
#include <winioctl.h>
#include <time.h>

#include <IO.h>    // for GetFileSize()
#include <FCNTL.h> // for GetFileSize()
#include <TLHELP32.h> // for GetAllProcessName()
#include "Version.h"

#include "ioDateHelp.h"

HRESULT CheckDXVersion( DWORD* pdwDirectXVersion, TCHAR* strDirectXVersion, int cchDirectXVersion );

namespace Help
{

static ioINILoader s_kCharLoader;
static int s_iPlayingUserBonusCount = 8;
static int s_iFriendBonusBeforeMin  = 30;
static float s_fFriendBonus = 0.05f;
static float s_fMaxFriendBonus = 0.20f;
static float s_fPcRoomFriendBonus = 0.10f;
static float s_fPcRoomMaxFriendBonus = 0.20f;

static float s_fVolumeNormalRate = 0;
static float s_fVolume3DRate     = 0;
static float s_fVolumeStreamRate = 0;

static DWORD s_dwScorllInActiveTime = 500;

static int s_iRefillCoinCount = 0;
static int s_iRefillCoinMax = 0;
static int s_iRefillCoinSec = 0;

static int s_iPlazaMaxPlayer = MAX_PLAZA_PLAYER;

static int s_iBadNetworkKickCount = 5;

static int s_iSoldierPesoBuyDateLimit = 0;

static ioHashString s_kDeveloperKCharName="";

static bool s_bCharChangeToUDP = false;

static int s_iDefaultBestFriendCount	  = 5;
static int s_iBestFriendDismissDelayHour  = 120;

static int s_iCharRentalMinute = 30;
static int s_iBestFriendCharRentalDelayHour = 24;
static int s_iCharRentalCount = 3;
static int s_iCharRentalGrade = 20;
static int s_iMaxCharRentSet  = 2;
static CEncrypt< bool > s_bOnlyServerRelay = false;
static bool s_bWholeChatOn = true;
static bool s_bWholeChangeColorChatOn = true;

static int s_iMVPTreasureCard = 0;

static bool s_bSendReciveCheck = false;

void InitHelpConstant()
{	
	s_kCharLoader.SetFileName_e( "config/sp2_char.ini" );

	ioINILoader_e kLoader( "config/sp2.ini" );
	kLoader.SetTitle_e( "result_correction" );
	s_iPlayingUserBonusCount = kLoader.LoadInt_e( "PlayingUserBonusCount", 1 );

	kLoader.SetTitle_e( "friend_bonus" );
	s_iFriendBonusBeforeMin = kLoader.LoadInt_e( "before_min", 30 );
	s_fFriendBonus          = kLoader.LoadFloat_e( "bonus", 0.05f );
	s_fMaxFriendBonus       = kLoader.LoadFloat_e( "max_bonus", 0.20f );
	s_fPcRoomFriendBonus    = kLoader.LoadFloat_e( "pcroom_bonus", 0.10f );	
	s_fPcRoomMaxFriendBonus = kLoader.LoadFloat_e( "pcroom_max_bonus", 0.40f );

	kLoader.SetTitle_e( "list_info" );
	s_dwScorllInActiveTime = kLoader.LoadInt_e( "ScorllInActiveTime", 500 );

	kLoader.SetTitle_e( "refill_coin" );
	s_iRefillCoinMax   = kLoader.LoadInt_e( "refill_coin_max", 0 );
	s_iRefillCoinCount = kLoader.LoadInt_e( "refill_coin_cnt", 0 );
	s_iRefillCoinSec   = kLoader.LoadInt_e( "refill_coin_sec", 0 );
	
	kLoader.SetTitle_e( "bad_network" );
	s_iBadNetworkKickCount = kLoader.LoadInt_e( "kick_count", 5 );

	kLoader.SetTitle_e( "plaza" );
	s_iPlazaMaxPlayer = kLoader.LoadInt_e( "max_player", MAX_PLAZA_PLAYER );

	kLoader.SetTitle_e( "best_friend" );
	s_iDefaultBestFriendCount = kLoader.LoadInt_e( "default_best_friend_count", 5 );
	s_iBestFriendDismissDelayHour = kLoader.LoadInt_e( "best_friend_dismiss_delay_hour", 120 );

	kLoader.SetTitle_e( "char_rental" );
	s_iCharRentalMinute = kLoader.LoadInt_e( "char_rental_minute", 30 );
	s_iBestFriendCharRentalDelayHour = kLoader.LoadInt_e( "best_friend_char_rental_delay_hour", 24 );
	s_iCharRentalCount = kLoader.LoadInt_e( "char_rental_count", 3 );
	s_iCharRentalGrade = kLoader.LoadInt_e( "char_rental_grade", 20 );
	s_iMaxCharRentSet  = kLoader.LoadInt_e( "max_char_rent_set", 2 );
}

ioINILoader& GetCharLoader()
{
	return s_kCharLoader;
}

ioINILoader& GetCharLoader( int iCharType, int iSex )
{
	if( iSex == 2 )
		iCharType += 3;

	iCharType = max( 1, min( iCharType, 6 ) );

	char szTitle[MAX_PATH];
	wsprintf_e( szTitle, "character%d", iCharType );
	s_kCharLoader.SetTitle( szTitle );

	return s_kCharLoader;
}

void InitColorValue( D3DCOLORVALUE &rkColor, float fRed, float fGreen, float fBlue, float fAlpha )
{
	rkColor.r = fRed;
	rkColor.g = fGreen;
	rkColor.b = fBlue;
	rkColor.a = fAlpha;
}

int RegReadInt(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, int iDefault )
{
	HKEY key;
	DWORD dwDisp;
	if (RegCreateKeyEx(hKey, lpKey,0,NULL,REG_OPTION_NON_VOLATILE, KEY_READ,NULL,&key,&dwDisp)!=ERROR_SUCCESS) 
	{
		LOG.PrintTimeAndLog( 0, "Error RegReadInt");
		return 0;
	}

	int iResult;
	DWORD dwSize = sizeof(int);
	if( RegQueryValueEx(key, lpValue, 0, NULL,(LPBYTE)&iResult, &dwSize)!=ERROR_SUCCESS )
	{
		iResult = iDefault;
	}

	RegCloseKey(key);

	return iResult;
}

bool RegWriteInt(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, int iData)
{
	HKEY key;
	DWORD dwDisp;
	if (RegCreateKeyEx(hKey, lpKey,0,NULL,REG_OPTION_NON_VOLATILE, KEY_WRITE,NULL,&key,&dwDisp)!=ERROR_SUCCESS) 
		return false;

	if (RegSetValueEx(key, lpValue,0,REG_DWORD,(LPBYTE)&iData,sizeof(int))!=ERROR_SUCCESS) 
		return false;

	RegCloseKey(key);
	return true;
}

bool RegReadString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpDefault, 
				   LPTSTR lpRet, DWORD nSize) 
{
	HKEY key;
	DWORD dwDisp;
	if (RegCreateKeyEx(hKey, lpKey,0,NULL,REG_OPTION_NON_VOLATILE, KEY_READ,NULL,&key,&dwDisp)!=ERROR_SUCCESS) 
	{
		LOG.PrintTimeAndLog( 0, "Error RegReadString(%s)",lpKey);
		return false;
	}

	DWORD dwSize = nSize;
	if (RegQueryValueEx(key, lpValue, 0, NULL,(LPBYTE)lpRet, &dwSize)!=ERROR_SUCCESS) 
	{
		strcpy(lpRet, lpDefault);
		return false;
	}

	RegCloseKey(key);
	return true;
}

bool RegWriteString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData)
{
	HKEY key;
	DWORD dwDisp;
	if (RegCreateKeyEx(hKey, lpKey,0,NULL,REG_OPTION_NON_VOLATILE, KEY_WRITE,NULL,&key,&dwDisp)!=ERROR_SUCCESS) 
		return false;

	if (RegSetValueEx(key, lpValue,0,REG_SZ,(LPBYTE)lpData,strlen(lpData)+1)!=ERROR_SUCCESS) 
		return false;

	RegCloseKey(key);
	return true;
}

void FaceChangeColorTexAndOutLine( ioEntity *pEntity,
								   const ioHashString &szTexture,
								   const D3DCOLORVALUE *pOutLine )
{
	if( !pEntity )	return;

	int iNumSubEntity = pEntity->GetNumSubEntity();
	if( iNumSubEntity > 0 )
	{
		ioSubEntity *pSubEntity = pEntity->GetSubEntity( 0 );
		ioMaterial *pMtl = pSubEntity->GetMaterial();
		ioTechnique *pTech = pMtl->GetBestTechnique();

		int iNumPass = pTech->GetNumPasses();
		for( int iPass=0 ; iPass<iNumPass ; iPass++ )
		{
			ioPass *pPass = pTech->GetPass( iPass );

			if( pOutLine && pPass->GetShaderGroupType() == SGT_OUTLINE )
			{
				ioShaderCustomArg *pArg = pPass->GetCustomArg( "OutLineColor" );
				if( pArg )
				{
					pArg->SetValue( *pOutLine );
				}
			}

			int iNumTexStage = pPass->GetNumTextureUnitStates();
			for( int iStage=0 ; iStage<iNumTexStage ; iStage++ )
			{
				ioTextureUnitState *pStage = pPass->GetTextureUnitState( iStage );

				if( pStage->IsUseLightTexture() )
					continue;

				if( pStage->GetTextureName() != szTexture )
				{
					pStage->SetTextureName( szTexture );
					pStage->Load( false );
				}
			}
		}
	}
}

void ChangeColorTexAndOutLine( ioEntity *pEntity,
							   const ioHashString &szTexture,
							   const D3DCOLORVALUE *pOutLine )
{
	if( !pEntity )	return;

	int iNumSubEntity = pEntity->GetNumSubEntity();
	for( int i=0 ; i<iNumSubEntity ; i++ )
	{
		ioSubEntity *pSubEntity = pEntity->GetSubEntity( i );
		ioMaterial *pMtl = pSubEntity->GetMaterial();
		ioTechnique *pTech = pMtl->GetBestTechnique();

		int iNumPass = pTech->GetNumPasses();
		for( int iPass=0 ; iPass<iNumPass ; iPass++ )
		{
			ioPass *pPass = pTech->GetPass( iPass );

			if( pOutLine && pPass->GetShaderGroupType() == SGT_OUTLINE )
			{
				ioShaderCustomArg *pArg = pPass->GetCustomArg( "OutLineColor" );
				if( pArg )
				{
					pArg->SetValue( *pOutLine );
				}
			}

			int iNumTexStage = pPass->GetNumTextureUnitStates();
			for( int iStage=0 ; iStage<iNumTexStage ; iStage++ )
			{
				ioTextureUnitState *pStage = pPass->GetTextureUnitState( iStage );

				if( pStage->IsUseLightTexture() )
					continue;

				if( pStage->GetTextureName() != szTexture )
				{
					pStage->SetTextureName( szTexture );
					pStage->Load( false );
				}
			}
		}
	}
}

void ChangeSkinColor( ioEntityGroup *pGrp, 
					  const D3DCOLORVALUE &rkSkin,
					  const D3DCOLORVALUE &rkOutLine )
{
	if( !pGrp )	return;

	_ENCSTR("MeshColor", szEncMeshColor);
	ioShaderCustomArg kSkinArg( szEncMeshColor );
	kSkinArg.SetValue( rkSkin );

	_ENCSTR("OutLineColor", szEncOutLineColor);
	ioShaderCustomArg kOutLineArg( szEncOutLineColor );
	kOutLineArg.SetValue( rkOutLine );

	char szLowName[MAX_PATH];

	int iNumEntity = pGrp->GetNumEntities();
	for( int i=0 ; i<iNumEntity ; i++ )
	{
		ioEntity *pEntity = pGrp->GetEntity( i );
		if( !pEntity )	continue;

		int iNumSubEntity = pEntity->GetNumSubEntity();
		for( int j=0 ; j<iNumSubEntity ; j++ )
		{
			ioSubEntity *pSubEntity = pEntity->GetSubEntity( j );
			if( !pSubEntity )	continue;

			ioMaterial *pMtl = pSubEntity->GetMaterial();
			if( !pMtl ) continue;

			ioTechnique *pTech = pMtl->GetBestTechnique();
			if( !pTech ) continue;

			bool bSkinMaterial = false;

			ioPass *pDefPass = pTech->GetPassByShaderGrp( SGT_DEFAULT );
			if( pDefPass )
			{
				if( pDefPass->GetNumTextureUnitStates() > 0 )
				{
					ioTextureUnitState *pState = pDefPass->GetTextureUnitState( 0 );
					if( pState->GetColorStageBlendOp().m_StageOp == D3DTOP_BLENDTEXTUREALPHA )
					{
						bSkinMaterial = true;
					}
				}
			}
			else
			{
				ioPass *pToon = pTech->GetPassByShaderGrp( SGT_TOON_SHADE );
				if( pToon )
				{
					const ioHashString *pShader = pToon->GetShaderGroupName();
					if( pShader )
					{
						strcpy( szLowName, pShader->c_str() );
						strlwr( szLowName );

						if( strstr( szLowName, "skin" ) )
						{
							bSkinMaterial = true;
						}
					}
				}
			}

			if( bSkinMaterial )
			{
				pSubEntity->SetCustomParameter( kSkinArg );

				ioPass *pOutLinePass = pTech->GetPassByShaderGrp( SGT_OUTLINE );
				if( pOutLinePass )
				{
					pOutLinePass->AddCustomParameter( kOutLineArg );
				}
			}
		}
	}
}

void ChangeMaterial( ioEntityGroup *pGrp, const ioHashString &szMaterialName, bool bSoft )
{
	if( !pGrp )	return;
	if( szMaterialName.IsEmpty() ) return;

	int iNumEntity = pGrp->GetNumEntities();
	for( int i=0 ; i<iNumEntity ; i++ )
	{
		ioEntity *pEntity = pGrp->GetEntity( i );
		if( !pEntity )	continue;

		int iNumSubEntity = pEntity->GetNumSubEntity();
		for( int j=0 ; j<iNumSubEntity ; j++ )
		{
			ioSubEntity *pSubEntity = pEntity->GetSubEntity( j );
			if( !pSubEntity )	continue;
			
			pSubEntity->ChangeMaterial( szMaterialName, bSoft );
		}
	}
}

void RestoreMaterial( ioEntityGroup *pGrp, bool bSoft )
{
	if( !pGrp )	return;

	int iNumEntity = pGrp->GetNumEntities();
	for( int i=0 ; i<iNumEntity ; i++ )
	{
		ioEntity *pEntity = pGrp->GetEntity( i );
		if( !pEntity )	continue;

		int iNumSubEntity = pEntity->GetNumSubEntity();
		for( int j=0 ; j<iNumSubEntity ; j++ )
		{
			ioSubEntity *pSubEntity = pEntity->GetSubEntity( j );
			if( !pSubEntity )	continue;

			pSubEntity->RestoreMaterial( bSoft );
		}
	}
}

void ChangeMaterialTex( ioEntityGroup *pGrp, const ioHashString &szName )
{
	if( !pGrp )	return;
	if( szName.IsEmpty() ) return;

	int iNumEntity = pGrp->GetNumEntities();
	for( int i=0 ; i<iNumEntity ; i++ )
	{
		ioEntity *pEntity = pGrp->GetEntity( i );
		if( !pEntity )	continue;

		int iNumSubEntity = pEntity->GetNumSubEntity();
		for( int j=0 ; j<iNumSubEntity ; j++ )
		{
			ioSubEntity *pSubEntity = pEntity->GetSubEntity( j );
			if( !pSubEntity )	continue;

			ioMaterial *pMtl = pSubEntity->GetMaterial();
			ioTechnique *pTech = pMtl->GetBestTechnique();

			int iNumPass = pTech->GetNumPasses();
			for( int iPass=0 ; iPass<iNumPass ; iPass++ )
			{
				ioPass *pPass = pTech->GetPass( iPass );

				/*
				D3DCOLORVALUE kColor;
				Help::InitColorValue( kColor, 0.557f, 0.451f, 0.369f, FLOAT1 );

				if( pPass->GetShaderGroupType() == SGT_OUTLINE )
				{
					ioShaderCustomArg *pArg = pPass->GetCustomArg( "OutLineColor" );
					if( pArg )
					{
						pArg->SetValue( kColor );
					}
				}
				*/

				int iNumTexStage = pPass->GetNumTextureUnitStates();
				for( int iStage=0 ; iStage<iNumTexStage ; iStage++ )
				{
					ioTextureUnitState *pStage = pPass->GetTextureUnitState( iStage );

					if( pStage->IsUseLightTexture() )
						continue;

					if( pStage->GetTextureName() != szName )
					{
						pStage->SetTextureName( szName );
						pStage->Load( false );
					}
				}
			}
		}
	}
}

void RestoreMaterialTex( ioEntityGroup *pGrp )
{
	if( !pGrp ) return;

	int iNumEntity = pGrp->GetNumEntities();
	for( int i=0 ; i<iNumEntity ; i++ )
	{
		ioEntity *pEntity = pGrp->GetEntity( i );

		int iNumSubEntity = pEntity->GetNumSubEntity();
		for( int j=0 ; j<iNumSubEntity ; j++ )
		{
			ioSubEntity *pSubEntity = pEntity->GetSubEntity( j );
			if( !pSubEntity )	continue;

			ioTechnique *pCloneTech = NULL;
			ioMaterial *pClone = pSubEntity->GetMaterial();
			if( pClone )
			{
				pCloneTech = pClone->GetBestTechnique();
			}

			ioTechnique *pSrcTech = NULL;
			ioMaterial *pSrc = ioMaterialManager::GetSingleton().GetMaterial( pSubEntity->GetMaterialName() );
			if( pSrc )
			{
				pSrcTech = pSrc->GetBestTechnique();
			}

			if( !pSrcTech || !pCloneTech )
				continue;

			// Default
			ioPass *pClonePass = pCloneTech->GetPassByShaderGrp( SGT_DEFAULT );
			ioPass *pSrcPass = pSrcTech->GetPassByShaderGrp( SGT_DEFAULT );

			if( pClonePass && pSrcPass )
			{
				int iNumTexStage = pClonePass->GetNumTextureUnitStates();
				for( int iStage=0 ; iStage<iNumTexStage ; iStage++ )
				{
					ioTextureUnitState *pStage = pClonePass->GetTextureUnitState( iStage );
					ioTextureUnitState *pSrcStage = pSrcPass->GetTextureUnitState( iStage );
					if( !pSrcStage )
						continue;

					if( pStage->IsUseLightTexture() )
						continue;

					if( pSrcStage->IsUseLightTexture() )
						continue;

					pStage->SetTextureName( pSrcStage->GetTextureName() );
					pStage->Load( false );
				}
			}

			// Toon
			pClonePass = pCloneTech->GetPassByShaderGrp( SGT_TOON_SHADE );
			pSrcPass = pSrcTech->GetPassByShaderGrp( SGT_TOON_SHADE );

			if( pClonePass && pSrcPass )
			{
				int iNumTexStage = pClonePass->GetNumTextureUnitStates();
				for( int iStage=0 ; iStage<iNumTexStage ; iStage++ )
				{
					ioTextureUnitState *pStage = pClonePass->GetTextureUnitState( iStage );
					ioTextureUnitState *pSrcStage = pSrcPass->GetTextureUnitState( iStage );
					if( !pSrcStage )
						continue;

					if( pStage->IsUseLightTexture() )
						continue;

					if( pSrcStage->IsUseLightTexture() )
						continue;

					pStage->SetTextureName( pSrcStage->GetTextureName() );
					pStage->Load( false );
				}
			}

			/*
			// OutLine
			pClonePass = pCloneTech->GetPassByShaderGrp( SGT_OUTLINE );
			pSrcPass = pSrcTech->GetPassByShaderGrp( SGT_OUTLINE );

			if( !pClonePass || !pSrcPass )
				continue;

			ioShaderCustomArg *pArg = pSrcPass->GetCustomArg( "OutLineColor" );
			if( pArg )
			{
				pClonePass->AddCustomParameter( *pArg );
			}

			pArg = pSrcPass->GetCustomArg( "OutLineThickness" );
			if( pArg )
			{
				pClonePass->AddCustomParameter( *pArg );
			}

			if( !Setting::UseObjectEdge() )
			{
				pClonePass->SetPassEnable( false );
			}
			*/
		}
	}
}

void CustomEntityGroupTexture( ioEntityGroup *pGrp, const ioHashString &rkTexDst, const ioHashString &rkTexSrc, int iCustomValue )
{
	if( !pGrp )	return;
	if( rkTexSrc.IsEmpty() ) return;

	int iNumEntity = pGrp->GetNumEntities();
	for( int i=0 ; i<iNumEntity ; i++ )
	{
		CustomEntityTexture( pGrp->GetEntity( i ), rkTexDst, rkTexSrc, iCustomValue );
	}
}

void CustomEntityTexture( ioEntity *pEntity, const ioHashString &rkTexDst, const ioHashString &rkTexSrc, int iCustomValue  )
{
	if( !pEntity )	return;
	if( rkTexSrc.IsEmpty() ) return;

	int iNumSubEntity = pEntity->GetNumSubEntity();
	for( int j=0 ; j<iNumSubEntity ; j++ )
	{
		ioSubEntity *pSubEntity = pEntity->GetSubEntity( j );
		if( !pSubEntity )	continue;

		ioMaterial *pMtl = pSubEntity->GetMaterial();
		ioTechnique *pTech = pMtl->GetBestTechnique();

		int iNumPass = pTech->GetNumPasses();
		int iPass = 0;
		for( iPass=0 ; iPass<iNumPass ; iPass++ )
		{			
			ioPass *pPass = pTech->GetPass( iPass );
			int iNumTexStage = pPass->GetNumTextureUnitStates();
			for( int iStage=0 ; iStage<iNumTexStage ; iStage++ )
			{
				ioTextureUnitState *pStage = pPass->GetTextureUnitState( iStage );
				if( pStage->GetTextureName().IsEmpty() ) continue;
				if( pStage->GetTextureCustomValue() != iCustomValue ) continue;

				if( pStage->GetTextureName() == rkTexDst )
				{
					pStage->SetTextureName( rkTexSrc );
					pStage->Load( false );
					pTech->SetCustomTex( true );
				}
			}
		}

		// Ŀ���� ���� �ƿ����� ȸ������ ����
		if( pTech->IsCustomTex() )
		{
			for( iPass=0 ; iPass<iNumPass ; iPass++ )
			{			
				ioPass *pPass = pTech->GetPass( iPass );				
				if( pPass->GetShaderGroupType() == SGT_OUTLINE )
				{
					ioShaderCustomArg *pArg = pPass->GetCustomArg( "OutLineColor" );
					if( pArg )
					{
						D3DCOLORVALUE kColor;
						Help::InitColorValue( kColor, 0.240f, 0.240f, 0.240f, FLOAT1 );

						pArg->SetValue( kColor );
					}
				}
			}
		}
	}
}

void CustomEffectTexture( ioEffect *pEffect, const ioHashString &rkTexDst, const ioHashString &rkTexSrc, int iCustomValue  )
{
	if( !pEffect ) return;
	if( rkTexSrc.IsEmpty() ) return;

	int iModelSysCnt = pEffect->GetModelSystemCnt();
	for(int i = 0;i < iModelSysCnt;i++)
	{
		ioModelParticleSystem *pModelParticleSys = pEffect->GetModelSys( i );
		if( !pModelParticleSys ) continue;
	
		ioModelParticleIterator kIterator = pModelParticleSys->GetIterator();
		while( !kIterator.end() )
		{
			ioModelParticle *pModelParticle = kIterator.GetNext();
			if( pModelParticle )
			{
				CustomEntityTexture( pModelParticle->GetEntity(), rkTexDst, rkTexSrc, iCustomValue );
			}
		}
	}
}

void CustomEntityTextureNameCnt( ioEntity *pEntity, const DWORD dwCustomCode, CustomTextureList &rkReturn, BOOL bCostume )
{
	if( !pEntity )	return;

	int iNumSubEntity = pEntity->GetNumSubEntity();
	for( int j=0 ; j<iNumSubEntity ; j++ )
	{
		ioSubEntity *pSubEntity = pEntity->GetSubEntity( j );
		if( !pSubEntity )	continue;

		ioMaterial *pMtl = pSubEntity->GetMaterial();
		ioTechnique *pTech = pMtl->GetBestTechnique();

		int iNumPass = pTech->GetNumPasses();
		for( int iPass=0 ; iPass<iNumPass ; iPass++ )
		{
			ioPass *pPass = pTech->GetPass( iPass );
			int iNumTexStage = pPass->GetNumTextureUnitStates();
			for( int iStage=0 ; iStage<iNumTexStage ; iStage++ )
			{
				ioTextureUnitState *pStage = pPass->GetTextureUnitState( iStage );
				if( pStage->GetTextureName().IsEmpty() ) continue;
				if( pStage->GetTextureCustomValue() == 0 ) continue;				

				char szTextureName[MAX_PATH] = "";
				if( dwCustomCode == 0 )
				{
					// ���� �ؽ��� �̸�
					sprintf( szTextureName, "%s", pStage->GetTextureName().c_str() );
				}
				else
				{
#ifdef CUSTOM_COSTUME
					// Ŀ���ҵ� �ؽ��� �̸�
					if( bCostume )
						sprintf( szTextureName, "%d_%d_c.lsc", dwCustomCode, pStage->GetTextureCustomValue() );
					else
						sprintf( szTextureName, "%d_%d.lsc", dwCustomCode, pStage->GetTextureCustomValue() );
#else
					sprintf( szTextureName, "%d_%d.lsc", dwCustomCode, pStage->GetTextureCustomValue() );
#endif
				}

				CustomTexture kTexture;
				kTexture.m_dwCustomSeq   = pStage->GetTextureCustomValue();
				kTexture.m_szTextureName = szTextureName;

				bool bPush = true;
				for(int i = 0;i < (int)rkReturn.size();i++)
				{
					CustomTexture &rkData = rkReturn[i];
					if( rkData.m_szTextureName == kTexture.m_szTextureName &&
						rkData.m_dwCustomSeq   == kTexture.m_dwCustomSeq )
					{
						bPush = false;
						break;
					}
				}

				if( bPush )
				{
					rkReturn.push_back( kTexture );
				}
			}
		}
	}
}

void CustomEffectTextureNameCnt( ioEffect *pEffect, const DWORD dwCustomCode, CustomTextureList &rkReturn )
{
	if( !pEffect ) return;

	int iModelSysCnt = pEffect->GetModelSystemCnt();
	for(int i = 0;i < iModelSysCnt;i++)
	{
		ioModelParticleSystem *pModelParticleSys = pEffect->GetModelSys( i );
		if( !pModelParticleSys ) continue;
		
		ioModelParticleIterator kIterator = pModelParticleSys->GetIterator();
		while( !kIterator.end() )
		{
			ioModelParticle *pModelParticle = kIterator.GetNext();
			if( pModelParticle )
			{
				CustomEntityTextureNameCnt( pModelParticle->GetEntity(), dwCustomCode, rkReturn );
			}
		}
	}
}

ioTexture *CustomEntityOutTexture( ioEntity *pEntity, const ioHashString &rkTexDst )
{
	if( !pEntity )	return NULL;
	if( rkTexDst.IsEmpty() ) return NULL;

	int iNumSubEntity = pEntity->GetNumSubEntity();
	for( int j=0 ; j<iNumSubEntity ; j++ )
	{
		ioSubEntity *pSubEntity = pEntity->GetSubEntity( j );
		if( !pSubEntity )	continue;

		ioMaterial *pMtl = pSubEntity->GetMaterial();
		ioTechnique *pTech = pMtl->GetBestTechnique();

		int iNumPass = pTech->GetNumPasses();
		for( int iPass=0 ; iPass<iNumPass ; iPass++ )
		{
			ioPass *pPass = pTech->GetPass( iPass );
			int iNumTexStage = pPass->GetNumTextureUnitStates();
			for( int iStage=0 ; iStage<iNumTexStage ; iStage++ )
			{
				ioTextureUnitState *pStage = pPass->GetTextureUnitState( iStage );
				if( pStage->GetTextureName() == rkTexDst )
				{
					return pStage->GetTexture();
				}
			}
		}
	}
	return NULL;
}

bool CustomTextureToJPG( ioTexture *pTexture, const char *szFileName )
{
	if( !pTexture ) return false;
	if( !pTexture->GetTexture() ) return false;

	HRESULT hr = D3DXSaveTextureToFile( szFileName, D3DXIFF_JPG, pTexture->GetTexture(), NULL );
	if( FAILED( hr ) )
	{
		LOG.PrintTimeAndLog( 0, "CustomTextureToJPG Error Code(0x%x)", hr );
		return false;
	}
	return true;
}

bool CustomTextureToDDS( ioTexture *pTexture, const char *szFileName )
{
	if( !pTexture ) return false;
	if( !pTexture->GetTexture() ) return false;

	HRESULT hr = D3DXSaveTextureToFile( szFileName, D3DXIFF_DDS, pTexture->GetTexture(), NULL );
	if( FAILED( hr ) )
	{
		LOG.PrintTimeAndLog( 0, "CustomTextureToDDS Error Code(0x%x)", hr );
		return false;
	}
	return true;
}

int CustomTextureJPGMixDDSFromDDS( const ioHashString &rkDDS, const ioHashString &rkJPG, const ioHashString &rkSave )
{
	ioTexture *pDDSTex = g_TextureMgr.CreateTexture( rkDDS, D3DUSAGE_DYNAMIC, D3DX_DEFAULT, TQ_HIGH, false, false, true );
	if( !pDDSTex )
	{
		LOG.PrintTimeAndLog( 0, "CustomTextureJPGMixDDSFromDDS : 1" );
		return 1;
	}

	if( Setting::GetTextureQuality() != TQ_HIGH )
	{
		g_TextureMgr.ChangeTextureQuality( pDDSTex, TQ_HIGH );
	}

	ioTexture *pJPGTex = g_TextureMgr.CreateTexture( rkJPG, D3DUSAGE_DYNAMIC, D3DX_DEFAULT, TQ_HIGH, false, false, true );
	if( !pJPGTex )
	{
		g_TextureMgr.DestroyByName( rkDDS, true );
		LOG.PrintTimeAndLog( 0, "CustomTextureJPGMixDDSFromDDS : 2" );
		return 2;
	}

	if( Setting::GetTextureQuality() != TQ_HIGH )
	{
		g_TextureMgr.ChangeTextureQuality( pJPGTex, TQ_HIGH );
	}

	// DXT1�� �����̽� ���� �� ����
/*	if( pDDSTex->GetFormat() != D3DFMT_DXT5 )
	{
		IDirect3DSurface9 *pDDSFace = NULL;
		if( pDDSTex->GetTexture() == NULL )
		{
			g_TextureMgr.DestroyByName( rkDDS, true );
			g_TextureMgr.DestroyByName( rkJPG, true );
			LOG.PrintTimeAndLog( 0, "CustomTextureJPGMixDDSFromDDS : 3 - 1" );
			return false;
		}

		HRESULT hr = pDDSTex->GetTexture()->GetSurfaceLevel( 0, &pDDSFace );
		if( FAILED( hr ) || !pDDSFace )
		{
			g_TextureMgr.DestroyByName( rkDDS, true );
			g_TextureMgr.DestroyByName( rkJPG, true );
			LOG.PrintTimeAndLog( 0, "CustomTextureJPGMixDDSFromDDS : 3 - 2(%x)", hr );
			return false;
		}
		
		IDirect3DSurface9 *pJPGFace = NULL;
		if( pJPGTex->GetTexture() == NULL )
		{
			g_TextureMgr.DestroyByName( rkDDS, true );
			g_TextureMgr.DestroyByName( rkJPG, true );
			LOG.PrintTimeAndLog( 0, "CustomTextureJPGMixDDSFromDDS : 3 - 3" );
			return false;
		}

		hr = pJPGTex->GetTexture()->GetSurfaceLevel( 0, &pJPGFace );
		if( FAILED( hr ) || !pJPGFace )
		{
			g_TextureMgr.DestroyByName( rkDDS, true );
			g_TextureMgr.DestroyByName( rkJPG, true );
			LOG.PrintTimeAndLog( 0, "CustomTextureJPGMixDDSFromDDS : 3 - 4(%x)", hr );
			return false;
		}

		hr = D3DXLoadSurfaceFromSurface( pDDSFace, NULL, NULL, pJPGFace, NULL, NULL, D3DX_FILTER_NONE, 0 );
		if( FAILED( hr ) )
		{
			g_TextureMgr.DestroyByName( rkDDS, true );
			g_TextureMgr.DestroyByName( rkJPG, true );
			LOG.PrintTimeAndLog( 0, "CustomTextureJPGMixDDSFromDDS : 3 - 5(%x)", hr );
			return false;
		}

		if( Help::CustomTextureToDDS( pDDSTex, rkSave.c_str() ) )
		{
			g_TextureMgr.DestroyByName( rkDDS, true );
			g_TextureMgr.DestroyByName( rkJPG, true );
			return true;
		}

		g_TextureMgr.DestroyByName( rkDDS, true );
		g_TextureMgr.DestroyByName( rkJPG, true );
		LOG.PrintTimeAndLog( 0, "CustomTextureJPGMixDDSFromDDS : 3 - 6" );
		return false;
		
	}
	else
*/
	{
		BYTE *pRGBA = new BYTE[pDDSTex->GetWidth() * pDDSTex->GetHeight() * 4]; 
		if( !g_TextureMgr.ExtractDDSColorValue( pDDSTex, pRGBA ) )
		{
			SAFEDELETEARRAY( pRGBA );
			g_TextureMgr.DestroyByName( rkDDS, true );
			g_TextureMgr.DestroyByName( rkJPG, true );
			LOG.PrintTimeAndLog( 0, "CustomTextureJPGMixDDSFromDDS : 4" );
			return 4;
		}

		if( !g_TextureMgr.MixJPGAlphaValue( pJPGTex, pRGBA, pDDSTex->GetWidth(), pDDSTex->GetHeight() ) )
		{
			SAFEDELETEARRAY( pRGBA );
			g_TextureMgr.DestroyByName( rkDDS, true );
			g_TextureMgr.DestroyByName( rkJPG, true );
			LOG.PrintTimeAndLog( 0, "CustomTextureJPGMixDDSFromDDS : 5" );
			return 5;
		}

		if( !g_TextureMgr.ColorCompressDDS( pDDSTex, pRGBA ) )
		{
			SAFEDELETEARRAY( pRGBA );
			g_TextureMgr.DestroyByName( rkDDS, true );
			g_TextureMgr.DestroyByName( rkJPG, true );
			LOG.PrintTimeAndLog( 0, "CustomTextureJPGMixDDSFromDDS : 6" );
			return 6;
		}
		SAFEDELETEARRAY( pRGBA );

		if( !Help::CustomTextureToDDS( pDDSTex, rkSave.c_str() ) )
		{
			LOG.PrintTimeAndLog( 0, "CustomTextureJPGMixDDSFromDDS : 7" );
			return 7;
		}
	}

	g_TextureMgr.DestroyByName( rkDDS, true );
	g_TextureMgr.DestroyByName( rkJPG, true );
	return 0;
}

bool CompressLSCFile( const ioHashString &rkFilePath )
{
	int iNameLen = rkFilePath.Length();
	if( iNameLen < 4 )
		return false;

	HANDLE hFile = ::CreateFile( rkFilePath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL );

	if( INVALID_HANDLE_VALUE == hFile )
		return false;

	DWORD dwFileSize = ::GetFileSize( hFile, NULL );
	if( INVALID_FILE_SIZE == dwFileSize )
	{
		::CloseHandle( hFile );
		return false;
	}

	unsigned char *pFileBuf = new unsigned char[dwFileSize];
	BOOL  bRet;
	DWORD dwReadedSize = 0;
	bRet = ::ReadFile( hFile, pFileBuf, dwFileSize, &dwReadedSize, NULL );
	if( FALSE == bRet )
	{
		::CloseHandle( hFile );
		SAFEDELETEARRAY(pFileBuf);
		return false;
	}
	::CloseHandle( hFile );

	char szSrc[MAX_PATH] = "";
	sprintf( szSrc, "%s", rkFilePath.c_str() );
	szSrc[iNameLen - 3] = 'l';
	szSrc[iNameLen - 2] = 's';
	szSrc[iNameLen - 1] = 'c';

	hFile = ::CreateFile( szSrc, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL );
	if( INVALID_HANDLE_VALUE == hFile )
	{
		SAFEDELETEARRAY(pFileBuf);
		return false;
	}

	DWORD dwWritedSize = 0;
	Encrypt( pFileBuf, dwFileSize );
	bRet = ::WriteFile( hFile, pFileBuf, dwFileSize, &dwWritedSize, NULL );
	if( FALSE == bRet )
	{
		::CloseHandle( hFile );
		SAFEDELETEARRAY(pFileBuf);
		return false;
	}
	::CloseHandle( hFile );
	SAFEDELETEARRAY(pFileBuf);

/*	DWORD dwWritedSize = 0;
	int iCutSize = (float)dwFileSize * LSC_FILE_CUT_RATE;
	unsigned char *pTempBuf = new unsigned char[dwFileSize];
	ioCPU::FastMemcpy( &pTempBuf[iCutSize], &pFileBuf[0], dwFileSize - iCutSize );
	ioCPU::FastMemcpy( &pTempBuf[0], &pFileBuf[dwFileSize - iCutSize], iCutSize );

	bRet = ::WriteFile( hFile, pTempBuf, dwFileSize, &dwWritedSize, NULL );
	if( FALSE == bRet )
	{
		::CloseHandle( hFile );
		SAFEDELETEARRAY(pFileBuf);
		SAFEDELETEARRAY(pTempBuf);
		return false;
	}
	::CloseHandle( hFile );
	SAFEDELETEARRAY( pTempBuf );
	SAFEDELETEARRAY(pFileBuf);
*/
	return true;
}

bool DecompressLSCMemory( unsigned char *pFileMemory, int iFileSize )
{
	if( !pFileMemory ) return false;

	Decrypt( pFileMemory, iFileSize );
/*	int iCutSize = (float)iFileSize * LSC_FILE_CUT_RATE;
	unsigned char *pTempBuf = new unsigned char[iFileSize];
	ioCPU::FastMemcpy( &pTempBuf[0], &pFileMemory[iCutSize], iFileSize - iCutSize );
	ioCPU::FastMemcpy( &pTempBuf[iFileSize - iCutSize], &pFileMemory[0], iCutSize );
	ioCPU::FastMemcpy( pFileMemory, pTempBuf, iFileSize );

	SAFEDELETEARRAY( pTempBuf );
*/
	return true;
}

bool IsAlNum(const char *szValue, const int iSize)
{
	for (int i=0; i<iSize; i++)
	{
		if ((!COMPARE(szValue[i], 'A', 'Z'+1)) &&
			(!COMPARE(szValue[i], 'a', 'z'+1)) &&
			(!COMPARE(szValue[i], '0', '9'+1)))
		{
			return false;
		}
	}
	return true;
}

void ExcuteProcess( char* szFileName , char* szCmdLine )
{
	PROCESS_INFORMATION ProcessInfo;
	STARTUPINFO         StartupInfo;
	StartupInfo.cb               = sizeof(STARTUPINFO);
	StartupInfo.lpReserved       = NULL;
	StartupInfo.lpDesktop        = NULL;
	StartupInfo.lpTitle          = NULL;
	StartupInfo.dwFlags          = STARTF_USESHOWWINDOW;
	StartupInfo.wShowWindow      = SW_SHOWNORMAL;
	StartupInfo.cbReserved2      = 0;
	StartupInfo.lpReserved2      = NULL;

	CreateProcess(szFileName, (LPSTR)szCmdLine, NULL,NULL,FALSE,0,NULL,NULL,&StartupInfo,&ProcessInfo);
}

void MinimizeAllWindows(BOOL bMinimize)
{
	HRESULT hr;

	if( FAILED(CoInitialize(NULL)) )
		return ;

	IShellDispatch *pDisp;
	hr = CoCreateInstance(CLSID_Shell,NULL,CLSCTX_INPROC_SERVER ,IID_IShellDispatch,(void**)&pDisp);
	if( SUCCEEDED(hr) )
	{
		if( bMinimize )
			pDisp->MinimizeAll();      
		else
			pDisp->UndoMinimizeALL();  

		pDisp->Release();
	}

	CoUninitialize();
}

bool IsKoreanOS()
{
	if(GetACP() == 949)
		return true;

	return false;
}


void ChangeCharSet( char *szText, char cDst, char cSrc )
{
	int iLen = strlen( szText );
	for(int i = 0;i < iLen;i++)
	{
		if( szText[i] == cDst )
			szText[i] = cSrc;
	}
}

bool GetDXVersionInfo( DWORD *pVesion, char *szVersion, int iBufLen )
{
	HRESULT hr = CheckDXVersion( pVesion, szVersion, iBufLen );
	if( FAILED(hr) )
		return false;

	return true;
}

void PrintMatrix( const D3DXMATRIX *pMat )
{
	if( !pMat )	return;

	LOG.PrintTimeAndLog( 0, "%f, %f, %f, %f", pMat->_11, pMat->_12, pMat->_13, pMat->_14 );
	LOG.PrintTimeAndLog( 0, "%f, %f, %f, %f", pMat->_21, pMat->_22, pMat->_23, pMat->_24 );
	LOG.PrintTimeAndLog( 0, "%f, %f, %f, %f", pMat->_31, pMat->_32, pMat->_33, pMat->_34 );
	LOG.PrintTimeAndLog( 0, "%f, %f, %f, %f", pMat->_41, pMat->_42, pMat->_43, pMat->_44 );
}

void HeadBlankClear( char *szBuf )
{
	int iCursor = 0;
	while( (szBuf[iCursor] == ' ' || szBuf[iCursor] == '\t') && szBuf[iCursor]!=0 )
		iCursor++;

	if( 0 < iCursor )
	{
		int iLength = strlen(szBuf + iCursor);
		memmove(szBuf, szBuf + iCursor, strlen(szBuf + iCursor) );
		szBuf[iLength] = '\0';
	}
}

// for GetActiveWindowTitle()
static char szActiveWindowTilte[MAX_PATH];

// for GetActiveWindowTitle()
BOOL CALLBACK EnumProc(HWND hWnd,LPARAM lParam)
{
	char szCurName[MAX_PATH] = "";
	GetWindowText(hWnd,szCurName,MAX_PATH);

	int iPreSize  = strlen( szActiveWindowTilte );
	int iCurSize  = strlen( szCurName );
	if( iCurSize > iPreSize )
		StringCbCopy( szActiveWindowTilte,MAX_PATH, szCurName );

	return true;
}

void GetActiveWindowTitle( DWORD dwThreadID, OUT char *szName, int iSize )
{
	memset( szActiveWindowTilte, 0, sizeof( szActiveWindowTilte) );
	EnumThreadWindows(dwThreadID, EnumProc,0);

	if( strcmp( szActiveWindowTilte, "" ) == 0 )
		StringCbCopy( szName, iSize, "NoName" );
	else
		StringCbCopy( szName, iSize, szActiveWindowTilte );
}

bool CheckSocketUDPPortUsed( int iPort )
{
	SOCKET kTestSocket = INVALID_SOCKET;

	kTestSocket = socket( AF_INET, SOCK_DGRAM, 0 );
	if( kTestSocket == INVALID_SOCKET )
		return false;

	SOCKADDR_IN kLocalSockIn;
	memset( &kLocalSockIn, 0, sizeof(kLocalSockIn) );
	kLocalSockIn.sin_family		 = AF_INET;
	kLocalSockIn.sin_addr.s_addr = htonl( INADDR_ANY );
	kLocalSockIn.sin_port		 = htons( iPort );

	if( bind( kTestSocket, (LPSOCKADDR)&kLocalSockIn, sizeof(kLocalSockIn) ) == SOCKET_ERROR )
	{
		if( WSAGetLastError() == WSAEADDRINUSE )
		{
			closesocket( kTestSocket );
			return true;
		}
	}

	closesocket( kTestSocket );
	return false;
}

void ExcuteWeb( const char *szURL )
{
	PROCESS_INFORMATION process_info;
	ZeroMemory( &process_info, sizeof(process_info) );	

	STARTUPINFO si;
	ZeroMemory( &si, sizeof(si) );				
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = 0;

	char ProcessName[1024] = "";
	char RegName[1024]="";
#ifdef SRC_TH //������Ʈ���� ��ϵ� �������� ȣ��
	Help::RegReadString(HKEY_CURRENT_USER, "SOFTWARE\\Classes\\http\\Shell\\open\\command","","",RegName,sizeof(RegName));
	ioHashString RegProcessName;
	int iStringSize = sizeof(RegName), iIndex = 0;
	bool bFind = false;
	for( int i = 0 ; i < iStringSize ; ++i )
	{
		if( bFind )
			memcpy( &RegName[i - 1], &RegName[i], sizeof(char) );
		if( RegName[i] == '"')
		{
			if( bFind )
			{
				StringCbCopyN( ProcessName, sizeof( ProcessName ), RegName, i - 1 );
				break;
			}
			bFind = true;
		}
	}
#else
	RegReadString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\IEXPLORE.EXE","","",ProcessName,sizeof(ProcessName));
#endif
	char ExecProcess[1024]="";
	StringCbCopy(ExecProcess,sizeof(ExecProcess), ProcessName);
	StringCbCat(ExecProcess,sizeof(ExecProcess), " ");
	StringCbCat(ExecProcess,sizeof(ExecProcess), szURL);
	if(CreateProcess(NULL,ExecProcess,NULL,NULL,FALSE,0,NULL,NULL,&si,&process_info))
		WaitForInputIdle(process_info.hProcess,10000 );
} 

void ExcuteIE( OUT PROCESS_INFORMATION &process_info ,IN const char *szURL )
{
	STARTUPINFO si;
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = 0;

	ZeroMemory( &process_info, sizeof(process_info) );	

	char ProcessName[1024];
	Help::RegReadString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\IEXPLORE.EXE"
		,"","",ProcessName,sizeof(ProcessName));

	char ExecProcess[1024];
	strcpy(ExecProcess, ProcessName);
	strcat(ExecProcess, " ");
	strcat(ExecProcess, szURL);  //�˻�����

	CreateProcess(NULL,ExecProcess,NULL,NULL,FALSE,0,NULL,NULL,&si,&process_info);
}


void ExcuteIE( OUT PROCESS_INFORMATION &IEProcessInfo, OUT IELAUNCHURLINFO& LunchInfo, IN const char *szURL )
{
	wchar_t wURL[ MAX_PATH ];
	MultiByteToWideChar( CP_ACP, 0, szURL, -1, wURL, MAX_PATH );

	LunchInfo.cbSize = sizeof(IELAUNCHURLINFO);
	LunchInfo.dwCreationFlags = NULL;
	IELaunchURL( wURL, &IEProcessInfo, &LunchInfo );
}

DWORD GetLanCableConnectState()
{
	DWORD dwRetState = 1;

	PIP_ADAPTER_INFO pAdapter = (IP_ADAPTER_INFO *)malloc( sizeof(IP_ADAPTER_INFO) );
	if( !pAdapter )	return dwRetState; // ����

	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	if (GetAdaptersInfo( pAdapter, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
	{
		free(pAdapter);
		
		pAdapter = (IP_ADAPTER_INFO *) malloc(ulOutBufLen);
		if( !pAdapter )
			return dwRetState; // ����
	}

	if( GetAdaptersInfo(pAdapter, &ulOutBufLen) != ERROR_SUCCESS )
	{
		free( pAdapter );
		return true;
	}

	std::list< std::string > vAdapterList;
	char szAdapterName[MAX_ADAPTER_NAME_LENGTH + 4 + 4];

	PIP_ADAPTER_INFO pAdapterInfo = pAdapter;
	while( pAdapterInfo )
	{
		sprintf( szAdapterName, "\\\\.\\%s", pAdapterInfo->AdapterName );
		vAdapterList.push_back( std::string(szAdapterName) );

		pAdapterInfo = pAdapterInfo->Next;
	}

	DWORD dwStatus, dwBytesRet;
	DWORD dwObj = OID_GEN_MEDIA_CONNECT_STATUS;

	std::list< std::string >::iterator iter = vAdapterList.begin();
	for( ; iter!=vAdapterList.end() ; ++iter )
	{
		HANDLE hFile = CreateFile( iter->c_str(), 0, 0, NULL, OPEN_EXISTING, 0, 0 );
		if( hFile != INVALID_HANDLE_VALUE )
		{
			BOOL bSuccess = DeviceIoControl( hFile,
											 IOCTL_NDIS_QUERY_GLOBAL_STATS,
											 &dwObj,
											 sizeof(dwObj),
											 &dwStatus,
											 sizeof(dwStatus),
											 &dwBytesRet,
											 NULL );

			if( bSuccess && dwStatus == 0 )
			{
				dwRetState = 0;
				CloseHandle( hFile );
				break;
			}

			CloseHandle( hFile );
		}
	}

	vAdapterList.clear();

	if( pAdapter )
	{
		free(pAdapter);
		pAdapter = NULL;
	}
	
	return dwRetState;
}

void CreateDefaultSettingINI()
{
	char fName[MAX_PATH]="";
	StringCbCopy( fName, sizeof(fName), "Save\\default\\setting.ini" );
	FILE *fp = fopen( fName, "r" );
	if( !fp )
	{
		CreateDirectory("Save",NULL);
		ZeroMemory(fName, sizeof(fName));
		StringCbCopy( fName, sizeof(fName), "Save\\default" );
		CreateDirectory(fName,NULL);
		ZeroMemory(fName, sizeof(fName));
		StringCbCopy( fName, sizeof(fName), "Save\\default\\setting.ini" );

		fp = fopen( fName, "w" );
		if( !fp )
			return;
	}
	fclose( fp );
}


int  GetFileSize(const char *szFileName)
{
	int handle =_open( szFileName ,_O_RDONLY|_O_TEXT);
	int iSize = -1;
	if( handle == -1 )
		return iSize;

	iSize = filelength(handle);
	_close(handle);

	return iSize;
}

void SetCreateDirectory( const char *szDir )
{
	if(szDir == NULL) return;

	const char *pDir = szDir;
	char createDir[MAX_PATH] = "";
	int len = 0;
	while (*pDir != '\0') 
	{
		if( *pDir=='/' || 
			*pDir=='\\' )
		{
			CreateDirectory(createDir,NULL);			
		}
		if(len >= MAX_PATH)
			break;
		createDir[len++] = *pDir;
		pDir++;
	}
	CreateDirectory(createDir,NULL);
}

bool GetAllProcessName(OUT char *szAllProcessName, IN int iSize)
{
	enum
	{
		START_CHECK_CNT = 10,
	};
	HANDLE hSnap;
	PROCESSENTRY32 pe;
	hSnap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if (hSnap == (HANDLE)-1)
	{
		return false;
	}

	int iCheckCnt = 0;
	char szLocalAllName[MAX_PATH*2]="";
	pe.dwSize=sizeof(PROCESSENTRY32);
	if (Process32First(hSnap,&pe)) 
	{
		do{
			iCheckCnt++;
			if(iCheckCnt >= START_CHECK_CNT)
			{
				char szTemp[MAX_PATH]="";
				char szName[MAX_PATH]="";
				_splitpath(pe.szExeFile, NULL, NULL, szName, NULL);
				StringCbPrintf_e(szTemp, sizeof( szTemp ), "%s / ", szName);
				StringCbCat(szLocalAllName, sizeof(szLocalAllName), szTemp);
			}
		} while (Process32Next(hSnap,&pe));
	}
	CloseHandle(hSnap);

	StringCbCopyN( szAllProcessName, iSize, szLocalAllName, sizeof(szLocalAllName) );
	return true;
}

DWORD GetLastPCRoomBonusDate()
{
	char szTempDir[MAX_PATH];
	memset( szTempDir, 0, MAX_PATH );
	::GetWindowsDirectory( szTempDir, MAX_PATH );
	strcat( szTempDir, "\\Help\\saalgsto.cnt" );

	DWORD dwRegDate = 0;
	FILE *pFile = fopen( szTempDir, "rb" );	
	if( pFile != NULL )
	{
		fread( &dwRegDate, sizeof( dwRegDate ), 1, pFile );
		fclose( pFile );
	}
	return dwRegDate;
}

void  SavePCRoomBonusDate( DWORD dwSaveDate )
{
	char szTempDir[MAX_PATH];
	memset( szTempDir, 0, MAX_PATH );
	::GetWindowsDirectory( szTempDir, MAX_PATH );
	strcat( szTempDir, "\\Help\\saalgsto.cnt" );

	FILE *pFile = fopen( szTempDir, "wb" );
	if( pFile != NULL )
	{
		fwrite( &dwSaveDate, sizeof( dwSaveDate ), 1, pFile );
		fclose( pFile );
	}
}

int GetFriendBonusBeforeMin()
{
	return s_iFriendBonusBeforeMin;
}

float GetFriendBonus()
{
	return s_fFriendBonus;
}

float GetMaxFriendBonus()
{
	return s_fMaxFriendBonus;
}

float GetPcRoomFriendBonus()
{
	return s_fPcRoomFriendBonus;
}

float GetPcRoomMaxFriendBonus()
{
	return s_fPcRoomMaxFriendBonus;
}

int GetPlayingUserBonusCount()
{
	return s_iPlayingUserBonusCount;
}

DWORD GetScorllInActiveTime()
{
	return s_dwScorllInActiveTime;
}

void LoadVolumesRate()
{
	ioINILoader_e kLoader( "config/sp2.ini" );
	kLoader.SetTitle_e( "volume_rate" );
	s_fVolumeNormalRate = kLoader.LoadFloat_e( "normal", FLOAT1 );
	s_fVolume3DRate     = kLoader.LoadFloat_e( "3d", FLOAT1 );
	s_fVolumeStreamRate = kLoader.LoadFloat_e( "stream", FLOAT1 );
}

int GetDSVolume( int iVolume , VolumeType eVolumeType )
{
	int iDSVolumePerMax = (DSBVOLUME_MIN/(MAX_VOLUME-1));
	iDSVolumePerMax /=4; // ������ 4�ܰ� ������ ���еǾ� ��ġ�� ���� �۰�.

	int iReverseNum =  (MAX_VOLUME-1) - iVolume ;
	int iDSVolume = iReverseNum * iDSVolumePerMax;

	// iVolumeLength  : 0~10000 ( DSBVOLUME_MAX ~ DSBVOLUME_MIN )
	int iVolumeLength =  ( -DSBVOLUME_MIN ) + ( iDSVolume ) ;
	if( eVolumeType == VT_NORMAL )
		iVolumeLength *= s_fVolumeNormalRate;
	else if( eVolumeType == VT_3D )
		iVolumeLength *= s_fVolume3DRate;
	else if( eVolumeType == VT_STREAM )
		iVolumeLength *= s_fVolumeStreamRate;

	iDSVolume = -( (-DSBVOLUME_MIN)-iVolumeLength );

	return iDSVolume;
}

ioHashString ConvertName( const ioHashString &rkName, bool bMale )
{
	char szNewName[MAX_PATH]="";
	if( bMale )  
		return rkName; // human man
	else
		StringCbPrintf_e( szNewName, sizeof( szNewName ), "%s_hw", rkName.c_str() ); // human woman

	return ioHashString( szNewName );
}

ioHashString ConvertTailName( const ioHashString &rkName, bool bMale ) // XXXX[add].XXX
{
	std::string szNewName;
	if( bMale )  
		return rkName; // human man
	else
		szNewName = ioStringConverter::AddTailToFileName( rkName.c_str(), "_hw" ); // human woman

	return ioHashString( szNewName.c_str() );
}

ioHashString ConvertTailNameAndFileCheck( const ioHashString &rkName, bool bMale ) // XXXX[add].XXX
{
	if( bMale )  
		return rkName; // human man
	else
	{
		std::string szNewName = ioStringConverter::AddTailToFileName( rkName.c_str(), "_hw" ); // human woman

		char szFullName[MAX_PATH]="";
		StringCbPrintf_e( szFullName, sizeof( szFullName ), "resource\\animation\\%s" , szNewName.c_str() );
		if( g_ResourceLoader.IsFileExist( szFullName ) )
			return ioHashString( szNewName.c_str() );	
	}
	return "";
}

int StringCutFun( float fScale, float fWidth, int iTextStyle, OUT char *szDst, int iDstSize, const char *szSrc )
{
	int iLen = strlen( szSrc );
	char szTemp[MAX_PATH * 2];
	memset( szTemp, 0, sizeof( szTemp ) );

	int iBack = 0;
	int iTemp = 0;
	int i = 0;
	for(;i < iLen;)
	{
		if( iTemp >= (MAX_PATH*2) )
			return iBack;

		iBack = i;
		szTemp[iTemp++]   = szSrc[i++];

#if defined( SRC_OVERSEAS ) && !defined( SRC_ID )

#if defined( MULTI_BYTE_CHECK )
		if( IsDBCSLeadByteEx( COUNTRY_CODE_PAGE, (BYTE)szTemp[iTemp - 1] ) )
#else
		if( false && IsDBCSLeadByteEx( COUNTRY_CODE_PAGE, (BYTE)szTemp[iTemp - 1] ) )
#endif

#else
		if( IsDBCSLeadByte( (BYTE)szTemp[iTemp - 1] ) )
#endif

			szTemp[iTemp++] = szSrc[i++];

		if( g_FontMgr.GetTextWidth( szTemp, (TextStyle)iTextStyle, fScale ) >= fWidth )
			return iBack;
		StringCbCopy( szDst, iDstSize, szTemp );
	}

	return iLen;
}

void ConvertNumToStrComma( int iSourceNum, OUT char *szDestCommaNum, int iDestSize )
{
	char szNum[MAX_PATH] = "";
	int iCnt = 0 ;

	StringCbPrintf_e( szNum, sizeof( szNum ), "%d", iSourceNum );

	size_t iSize = 0;
	StringCbLength( szNum, sizeof( szNum ), &iSize );

	// �Ųٷ� ���ڸ� ����. 1,000 �ڿ��� ���� ���� ���Ƿ� 
	for(int i = iSize - 1 ; i >= 0 ; i--)
	{
		szDestCommaNum[iCnt] = szNum[i];  
		iCnt++;
		if( iCnt >= iDestSize )
			break;
	}
	if( COMPARE( iCnt , 0, iDestSize ) )
		szDestCommaNum[iCnt] = NULL;
	else
		szDestCommaNum[iDestSize-1] = NULL;

	iCnt = 0;
	for (int i = 0; i < (int)iSize ; i++)
	{
		if( i%3 == 0 && i != 0 )
			szNum[iCnt++] = ',';

		if( iCnt >= MAX_PATH )
			break;
		if( i >= iDestSize )
			break;

		szNum[iCnt++] = szDestCommaNum[i];
	}

	if( COMPARE( iCnt , 0, MAX_PATH ) )
		szNum[iCnt] = NULL;
	else
		szNum[MAX_PATH-1] = NULL;

	iCnt  = 0;	
	iSize = 0;
	StringCbLength( szNum, sizeof( szNum ), &iSize );
	// �Ųٷ� ���ڸ� ����. 1,000 �ڿ��� ���� ���� ���Ƿ� 
	for(int i = iSize - 1 ; i >= 0 ; i--)
	{
		szDestCommaNum[iCnt] = szNum[i];  
		iCnt++;
		if( iCnt >= iDestSize )
			break;
	}
	if( COMPARE( iCnt , 0, iDestSize ) )
		szDestCommaNum[iCnt] = NULL;
	else
		szDestCommaNum[iDestSize-1] = NULL;
}

int GetRefillCoinCount()
{
	return s_iRefillCoinCount;
}

int GetRefillCoinMax()
{
	return s_iRefillCoinMax;
}

int GetRefillCoinSec()
{
	return s_iRefillCoinSec;
}
//
int GetBadNetworkKickCount()
{
	return s_iBadNetworkKickCount;
}
//
int GetSoldierPesoBuyDateLimit()
{
	return s_iSoldierPesoBuyDateLimit;
}

int GetPlazaMaxPlayer()
{
	return min( s_iPlazaMaxPlayer, MAX_PLAZA_PLAYER );
}
//
void SetDeveloperKCharName( const char *szCharName )
{
	s_kDeveloperKCharName = szCharName;
}

const ioHashString &GetDeveloperKCharName()
{
	return s_kDeveloperKCharName;
}
//

void GetRemainTime( int a_iYear, int a_iMonth, int a_iDay, int a_iHour, int a_iMinute, OUT char *szTime, int iTimeSize )
{
	ZeroMemory( szTime, iTimeSize );

	time_t kLimitTime = DateHelp::ConvertSecondTime( a_iYear, a_iMonth, a_iDay, a_iHour, a_iMinute, 0 );
	kLimitTime += 30; // ������ �ٷ� �ð��� ���� ���� �����ϱ� ���ؼ� 30���� �����ش�.
	time_t kCurServerTime = g_MyInfo.GetServerDate();
	kCurServerTime += ( REALGETTIME()/FLOAT1000 );// ��������� Ŭ���̾�Ʈ�ʸ� �ִ´�.

	int iGapSec = kLimitTime - kCurServerTime;
	enum { DAY_SEC = 86400, HOUR_SEC = 3600, MINUTE_SEC = 60, };
	int iDay    = iGapSec/DAY_SEC;
	int iHourSec= iGapSec-(iDay*DAY_SEC);
	int iHour   = iHourSec / HOUR_SEC;
	int iMinute = ( iHourSec-(iHour*HOUR_SEC) ) / MINUTE_SEC;

	if( iDay <= 0 && iHour <= 0 && iMinute <= 0 )
		SafeSprintf( szTime, iTimeSize, STR(1) );
	else if( iDay <= 0 && iHour <= 0 && iMinute > 0 )
		SafeSprintf( szTime, iTimeSize, STR(2), iMinute );
	else if( iDay <= 0 && iHour > 0 && iMinute <= 0 )
		SafeSprintf( szTime, iTimeSize, STR(3), iHour );
	else if( iDay <= 0 && iHour > 0 && iMinute > 0 )
		SafeSprintf( szTime, iTimeSize, STR(6), iHour, iMinute );
	else if( iDay > 0 && iHour <= 0 )
		SafeSprintf( szTime, iTimeSize, STR(4), iDay );
	else 
		SafeSprintf( szTime, iTimeSize, STR(5), iDay, iHour );
}


// GetRemainTime() �������� string ���� �����ϱ� ���� �߰�	JCLEE 131320
std::string GetRemainTimeToStr( int a_iYear, int a_iMonth, int a_iDay, int a_iHour, int a_iMinute, OUT char *szTime, int iTimeSize )
{
	ZeroMemory( szTime, iTimeSize );
	std::string strTime;

	time_t kLimitTime = DateHelp::ConvertSecondTime( a_iYear, a_iMonth, a_iDay, a_iHour, a_iMinute, 0 );
	kLimitTime += 30; // ������ �ٷ� �ð��� ���� ���� �����ϱ� ���ؼ� 30���� �����ش�.
	time_t kCurServerTime = g_MyInfo.GetServerDate();
	kCurServerTime += ( REALGETTIME()/FLOAT1000 );// ��������� Ŭ���̾�Ʈ�ʸ� �ִ´�.

	int iGapSec = kLimitTime - kCurServerTime;
	enum { DAY_SEC = 86400, HOUR_SEC = 3600, MINUTE_SEC = 60, };
	int iDay    = iGapSec/DAY_SEC;
	int iHourSec= iGapSec-(iDay*DAY_SEC);
	int iHour   = iHourSec / HOUR_SEC;
	int iMinute = ( iHourSec-(iHour*HOUR_SEC) ) / MINUTE_SEC;

	if( iDay <= 0 && iHour <= 0 && iMinute <= 0 )
		SafeSprintf( szTime, iTimeSize, STR(1) );
	else if( iDay <= 0 && iHour <= 0 && iMinute > 0 )
		SafeSprintf( szTime, iTimeSize, STR(2), iMinute );
	else if( iDay <= 0 && iHour > 0 && iMinute <= 0 )
		SafeSprintf( szTime, iTimeSize, STR(3), iHour );
	else if( iDay <= 0 && iHour > 0 && iMinute > 0 )
		SafeSprintf( szTime, iTimeSize, STR(6), iHour, iMinute );
	else if( iDay > 0 && iHour <= 0 )
		SafeSprintf( szTime, iTimeSize, STR(4), iDay );
	else 
		SafeSprintf( szTime, iTimeSize, STR(5), iDay, iHour );

	strTime = szTime;

	return strTime;
}


// GetRemainTime() �������� integer minite ���� �����ϱ� ���� �߰�	JCLEE 131320
int GetRemainTimeToMin( int a_iYear, int a_iMonth, int a_iDay, int a_iHour, int a_iMinute )
{
	time_t kLimitTime = DateHelp::ConvertSecondTime( a_iYear, a_iMonth, a_iDay, a_iHour, a_iMinute, 0 );
	kLimitTime += 30; // ������ �ٷ� �ð��� ���� ���� �����ϱ� ���ؼ� 30���� �����ش�.
	time_t kCurServerTime = g_MyInfo.GetServerDate();
	kCurServerTime += ( REALGETTIME()/FLOAT1000 );// ��������� Ŭ���̾�Ʈ�ʸ� �ִ´�.

	int iGapSec	= kLimitTime - kCurServerTime;
	int iGapMin	= iGapSec / 60;

	// 0 ���� �� ��� 0���� ǥ��
	if( iGapMin <= 0 )
		iGapMin = 0;

	return iGapMin;
}

void GetGrowthTitle( IN int iArray, IN bool bTimeSlot, OUT ioHashString &rsGrowthTitle )
{
	// iTimeSlot�� ��ų������� 1�� ���Ƿ� ��ȯ �۾��� �Ѵ�. 12345678->45670123
	if( bTimeSlot )
	{
		if( iArray > 4 )
			iArray = iArray - 5;
		else
			iArray = iArray + 3;
	}

	// Char Growth
	if( iArray == 0 )	
		rsGrowthTitle = STR(1);
	else if( iArray == 1 )
		rsGrowthTitle = STR(2);
	else if( iArray == 2 )
		rsGrowthTitle = STR(3);
	else if( iArray == 3 )
		rsGrowthTitle = STR(4);
	// Item Growth
	else if( iArray == 4 )
		rsGrowthTitle = STR(5);
	else if( iArray == 5 )
		rsGrowthTitle = STR(6);
	else if( iArray == 6 )
		rsGrowthTitle = STR(7);
	else if( iArray == 7 )
		rsGrowthTitle = STR(8);
}

void GetNotCutWndPos( IN int iMouseX, IN int iMouseY, IN int iWndWidth, IN int iWndHeight, OUT int &riXPos, OUT int &riYPos )
{
	riXPos = 0;
	riYPos = 0;
	enum { MOUSE_POS_GAP = 8, HEIGHT_DESKBAR_OFFSET = -45 };
	int iTooltipRightPos = iWndWidth + iMouseX + MOUSE_POS_GAP ;
	if( iTooltipRightPos >= Setting::Width() )
	{
		riXPos = iMouseX - iWndWidth - MOUSE_POS_GAP;
	}
	else
	{
		riXPos = iMouseX + MOUSE_POS_GAP;
	}

	int iTooltipBottonPos = iWndHeight + iMouseY + MOUSE_POS_GAP;
	if( iTooltipBottonPos >= ( Setting::Height() + HEIGHT_DESKBAR_OFFSET ) )
	{
		riYPos = iMouseY - iWndHeight - MOUSE_POS_GAP;
	}
	else
	{
		riYPos = iMouseY + MOUSE_POS_GAP;
	}

	if( riYPos < 0 )
		riYPos = 0;
}

void TextCut( IN const char *pSrc, IN int iCutWidth, IN TextStyle eTextStyle, IN float fScale, OUT ioHashString &rsReturn )
{
	char szAdd[MAX_PATH]="..";
	if( strlen( szAdd ) > 0 )
	{
		iCutWidth -= g_FontMgr.GetTextWidth( szAdd, eTextStyle, fScale );
	}

	int iLen = strlen( pSrc );
	char szDst[MAX_PATH];
	char szTemp[MAX_PATH];
	memset( szDst, 0, MAX_PATH );
	memset( szTemp, 0, MAX_PATH );

	int iTemp = 0;
	int iBuf = 0;
	for(;iBuf < iLen;)
	{
		if( iTemp >= MAX_PATH )
			break;

		szTemp[iTemp++]   = pSrc[iBuf++];

#if defined( SRC_OVERSEAS ) && !defined( SRC_ID )

#if defined( MULTI_BYTE_CHECK )
		if( IsDBCSLeadByteEx( COUNTRY_CODE_PAGE, (BYTE)szTemp[iTemp - 1] ) )
#else
		if( false && IsDBCSLeadByteEx( COUNTRY_CODE_PAGE, (BYTE)szTemp[iTemp - 1] ) )
#endif

#else
		if( IsDBCSLeadByte( (BYTE)szTemp[iTemp - 1] ) )
#endif

			szTemp[iTemp++] = pSrc[iBuf++];

		if( g_FontMgr.GetTextWidth( szTemp, eTextStyle, fScale ) >= iCutWidth )
			break;
		StringCbCopy( szDst, sizeof( szDst ), szTemp );
	}

	if( strlen( szAdd ) > 0 )
		StringCbCat( szDst, sizeof( szDst),  szAdd );

	rsReturn = szDst;
}

void SaveClientError( int iErrorNumber, int iDesc1, int iDesc2 )
{
	CreateDefaultSettingINI();
	ioINILoader kIniLoder("Save\\default\\setting.ini",false);
	kIniLoder.SetTitle_e("Setting");
	kIniLoder.SaveInt("ClientError", iErrorNumber );
	kIniLoder.SaveInt("ClientErrorDesc1", iDesc1 );
	kIniLoder.SaveInt("ClientErrorDesc2", iDesc2 );

	SYSTEMTIME st;
	GetLocalTime( &st );
	kIniLoder.SaveInt("ClientErrorDesc3", FILEVER );
	int iDate = (st.wMonth * 1000000) + ( st.wDay * 10000 ) + ( st.wHour * FLOAT100 ) + st.wMinute;
	kIniLoder.SaveInt("ClientErrorDesc4", iDate );
}


void SetCharChangeToUDP( bool bUDP )
{
	s_bCharChangeToUDP = bUDP;
}

bool IsCharChangeToUDP()
{
	return s_bCharChangeToUDP;
}

void SetOnlyServerRelay( bool bRelay )
{
	s_bOnlyServerRelay = bRelay;
}

bool IsOnlyServerRelay()
{
	return s_bOnlyServerRelay;
}

void SetWholeChatOn( bool bOn )
{
	s_bWholeChatOn = bOn;
}

bool IsWholeChatOn()
{
	return s_bWholeChatOn;
}

void SetWholeRainbowChatOn( bool bOn )
{
	s_bWholeChangeColorChatOn = bOn;
}

bool IsWholeRainbowChatOn()
{
	return s_bWholeChangeColorChatOn;
}

int GetDefaultBestFriendCount()
{
	return s_iDefaultBestFriendCount;
}

int GetBestFriendDismissDelayHour()
{
	return s_iBestFriendDismissDelayHour;
}

int GetCharRentalMinute()
{
	return s_iCharRentalMinute;
}

int GetBestFriendCharRentalDelayHour()
{
	return s_iBestFriendCharRentalDelayHour;
}

int GetCharRentalCount()
{
	return s_iCharRentalCount;
}

int GetCharRentalGrade()
{
	return s_iCharRentalGrade;
}

int GetMaxCharRentSet()
{
	return s_iMaxCharRentSet;
}

void SetSendReciveCheck( bool bCheck )
{
	if( s_bSendReciveCheck && !bCheck )
	{
		g_App.CheckTestSendRecive();
		g_WeaponMgr.ClearApplyWeaponInfo();
	}

	s_bSendReciveCheck = bCheck;
}

bool CheckEnableTimeGapUser( int iType )
{
#ifdef SRC_OVERSEAS 
	return false;
#endif 

	if( iType == 0 )
		return false;

	ioMyInfo* pMyinfo = ioMyInfo::GetSingletonPtr();
	if( pMyinfo == NULL )
		return false;
	//���̵� ������ ���޹����� ���� ĳ���� ���ϵ��� false ó��
	if( g_MyInfo.GetPublicID().IsEmpty() )
		return false;

	//UJ 130915, ����ȭ: ����ȭ�� ����Ǹ鼭 CheckEnableTimeGapUser() ���ϸ� ���ø��� ������ �Ǿ�, ĳ�ø� ���� 
	static bool bIsMeCheckUserFirst = true;
	static bool bIsMeCheckUser = false;
	if( bIsMeCheckUserFirst )
	{
		bIsMeCheckUserFirst = false;
		if( (strcmp( g_MyInfo.GetPublicID().c_str(), STR(1) ) != 0)			&&
			(strcmp( g_MyInfo.GetPublicID().c_str(), STR(1) ) != 0)		&&
			(strcmp( g_MyInfo.GetPublicID().c_str(), STR(2) ) != 0)		&&
			(strcmp( g_MyInfo.GetPublicID().c_str(), STR(3) ) != 0)		&&

			//������ ���ΰ���
			(strcmp( g_MyInfo.GetPublicID().c_str(), STR(4) ) != 0)		&&
			(strcmp( g_MyInfo.GetPublicID().c_str(), STR(5) ) != 0)	&&
			(strcmp( g_MyInfo.GetPublicID().c_str(), STR(6) ) != 0)		&&
			(strcmp( g_MyInfo.GetPublicID().c_str(), STR(7) ) != 0)		&&
			(strcmp( g_MyInfo.GetPublicID().c_str(), "jch2305" ) != 0)		&&
			(strcmp( g_MyInfo.GetPublicID().c_str(), STR(8) ) != 0)	&&
			(strcmp( g_MyInfo.GetPublicID().c_str(), "MyMonarch" ) != 0)	&&
			(strcmp( g_MyInfo.GetPublicID().c_str(), STR(9) ) != 0)		&&
			(strcmp( g_MyInfo.GetPublicID().c_str(), STR(10) ) != 0)		&&

			//������ ���ΰ���2
			(strcmp( g_MyInfo.GetPublicID().c_str(), STR(1) ) != 0)		&&
			(strcmp( g_MyInfo.GetPublicID().c_str(), "HolicLS" ) != 0)		&&
			(strcmp( g_MyInfo.GetPublicID().c_str(), "jch4378" ) != 0)		&&
			(strcmp( g_MyInfo.GetPublicID().c_str(), STR(2) ) != 0)	&&
			(strcmp( g_MyInfo.GetPublicID().c_str(), STR(3) ) != 0)		&&
			(strcmp( g_MyInfo.GetPublicID().c_str(), "SOCKET" ) != 0)		&&

			//���� QA��
			(strcmp( g_MyInfo.GetPublicID().c_str(), "JuneTM" ) != 0)		&&
			(strcmp( g_MyInfo.GetPublicID().c_str(), "JuneTMs" ) != 0)		&&
			(strcmp( g_MyInfo.GetPublicID().c_str(), STR(11) ) != 0)			&&
			(strcmp( g_MyInfo.GetPublicID().c_str(), STR(12) ) != 0)			&&
			(strcmp( g_MyInfo.GetPublicID().c_str(), "�޸�ī��" ) != 0) ) //Except Extracting Hangeul
		{
			bIsMeCheckUser = false;
			return false;
		}
		bIsMeCheckUser = true;
	}
	else
	{
		if( !bIsMeCheckUser )
			return false;
	}

	switch( iType )
	{
	case 1:
		return true;
	case 2:
		if( s_bSendReciveCheck )
			return true;
		break;
	}

	return false;
}


bool CheckEnableTimeGapUser2( int iType )
{
#ifdef SRC_OVERSEAS
	return false;
#endif

	if( !ioMyInfo::GetSingletonPtr() )
		return false;

	if( iType == 0 )
		return false;

	//UJ 130915, ����ȭ: ����ȭ�� ����Ǹ鼭 CheckEnableTimeGapUser() ���ϸ� ���ø��� ������ �Ǿ�, ĳ�ø� ���� 
	static bool bIsMeCheckUserFirst = true;
	static bool bIsMeCheckUser = false;

	if( bIsMeCheckUserFirst )
	{
		bIsMeCheckUserFirst = false;
		if( (strcmp( g_MyInfo.GetPublicID().c_str(), STR(1) ) != 0) &&
			(strcmp( g_MyInfo.GetPublicID().c_str(), STR(2) ) != 0) &&
			(strcmp( g_MyInfo.GetPublicID().c_str(), STR(3) ) != 0) &&
			(strcmp( g_MyInfo.GetPublicID().c_str(), STR(4) ) != 0) &&
			(strcmp( g_MyInfo.GetPublicID().c_str(), STR(5) ) != 0) &&
			(strcmp( g_MyInfo.GetPublicID().c_str(), STR(6)) != 0) &&
			(strcmp( g_MyInfo.GetPublicID().c_str(), "jch2305" ) != 0) ) //Except Extracting Hangeul
		{
			bIsMeCheckUser = false;
			return false;
		}
		bIsMeCheckUser = true;
	}
	else
	{
		if( !bIsMeCheckUser )
			return false;
	}

	switch( iType )
	{
	case 1:
		return true;
	case 2:
		if( s_bSendReciveCheck )
			return true;
		break;
	}

	return false;
}

int TournamentRound( int iMaxTeam )
{
	int iReturnVal = 0;
	for(int i = iMaxTeam;i > 0;)
	{
		i = i / 2;
		iReturnVal++;
	}
	return iReturnVal - 1;
}

int TournamentCurrentRoundWithTeam( int iMaxTeam, int iCurrentRound )
{
	for(int i = 0;i < iCurrentRound;i++)
	{
		iMaxTeam /= 2;
	}
	return iMaxTeam;
}

void ConvertTournamentRoundResultText( OUT ioHashString& szDestTtitle, int iMaxTeam, int iPos )
{
	char szBuff[MAX_PATH];
	int iRound = TournamentCurrentRoundWithTeam( iMaxTeam, iPos );
	if( iRound == 1 )
		SafeSprintf( szBuff, sizeof(szBuff), STR(1), iRound );
	else if(  iRound == 2 )
		SafeSprintf( szBuff, sizeof(szBuff), STR(2), iRound );
	else
		SafeSprintf( szBuff, sizeof(szBuff), STR(3), iRound );

	szDestTtitle = szBuff;
}

void ConvertTournamentRoundProressText( OUT ioHashString& szDestTtitle, int iMaxTeam, int iPos )
{
	char szBuff[MAX_PATH];
	int iRound = TournamentCurrentRoundWithTeam( iMaxTeam, iPos );
	if( iRound == 1 )
		SafeSprintf( szBuff, sizeof(szBuff), STR(1), iRound );
	else if(  iRound == 2 )
		SafeSprintf( szBuff, sizeof(szBuff), STR(2), iRound );
	else
		SafeSprintf( szBuff, sizeof(szBuff), STR(3), iRound );

	szDestTtitle = szBuff;
}

bool CaptureScreen()
{
	HWND hWnd = g_App.GetHWnd();
	HDC hdcWindow;
	HDC hdcMemDC = NULL;
	HBITMAP hbmScreen = NULL;
	BITMAP bmpScreen;

	hdcWindow = GetDC(g_App.GetHWnd());
	hdcMemDC = CreateCompatibleDC(hdcWindow); 

	if(!hdcMemDC)
	{
		LOG.PrintTimeAndLog(0, "Can not capture the screen");
		DeleteObject(hbmScreen);
		DeleteObject(hdcMemDC);
		ReleaseDC(hWnd,hdcWindow);
		return false;
	}

	RECT rcClient;
	GetClientRect(g_App.GetHWnd(), &rcClient);

	hbmScreen = CreateCompatibleBitmap(hdcWindow, rcClient.right-rcClient.left, rcClient.bottom-rcClient.top);

	if(!hbmScreen)
	{
		LOG.PrintTimeAndLog(0, "Can not capture the screen");
		DeleteObject(hbmScreen);
		DeleteObject(hdcMemDC);
		ReleaseDC(hWnd,hdcWindow);
		return false;
	}

	SelectObject(hdcMemDC,hbmScreen);
	if(!BitBlt(hdcMemDC, 0,0, rcClient.right-rcClient.left, rcClient.bottom-rcClient.top, 
		hdcWindow, 0, 0, SRCCOPY))
	{
		LOG.PrintTimeAndLog(0, "BitBlt has failed");
		DeleteObject(hbmScreen);
		DeleteObject(hdcMemDC);
		ReleaseDC(hWnd,hdcWindow);
		return false;
	}

	GetObject(hbmScreen,sizeof(BITMAP),&bmpScreen);

	BITMAPFILEHEADER   bmfHeader;    
	BITMAPINFOHEADER   bi;

	bi.biSize = sizeof(BITMAPINFOHEADER);    
	bi.biWidth = bmpScreen.bmWidth;    
	bi.biHeight = bmpScreen.bmHeight;  
	bi.biPlanes = 1;    
	bi.biBitCount = 32;    
	bi.biCompression = BI_RGB;    
	bi.biSizeImage = 0;  
	bi.biXPelsPerMeter = 0;    
	bi.biYPelsPerMeter = 0;    
	bi.biClrUsed = 0;    
	bi.biClrImportant = 0;

	DWORD dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;

	HANDLE hDIB = GlobalAlloc(GHND,dwBmpSize); 
	char *lpbitmap = (char *)GlobalLock(hDIB);    

	GetDIBits(hdcWindow, hbmScreen, 0, (UINT)bmpScreen.bmHeight, lpbitmap,
		(BITMAPINFO *)&bi, DIB_RGB_COLORS);

	HANDLE hFile = CreateFile("info\\Crash.bmp",
		GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);   

	DWORD dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER); 
	bmfHeader.bfSize = dwSizeofDIB; 
	bmfHeader.bfType = 0x4D42; //BM   

	DWORD dwBytesWritten = 0;
	WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);

	GlobalUnlock(hDIB);    
	GlobalFree(hDIB);

	CloseHandle(hFile);

	DeleteObject(hbmScreen);
	DeleteObject(hdcMemDC);
	ReleaseDC(hWnd,hdcWindow);
	return true;
}

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if(size == 0)
		return -1;  // Failure

	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
		return -1;  // Failure

	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

	for(UINT j = 0; j < num; ++j)
	{
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}    
	}

	free(pImageCodecInfo);
	return 0;
}

bool CaptureScreenWithJPEG()
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);


	{
		HDC scrdc, memdc;
		HBITMAP membit;
		scrdc = ::GetDC(g_App.GetHWnd());

		RECT rcClient;
		GetClientRect(g_App.GetHWnd(), &rcClient);

		int nWidth = rcClient.right-rcClient.left;
		int nHeight = rcClient.bottom-rcClient.top;

		memdc = CreateCompatibleDC(scrdc);
		membit = CreateCompatibleBitmap(scrdc, nWidth, nHeight);
		HBITMAP hOldBitmap =(HBITMAP) SelectObject(memdc, membit);
		BitBlt(memdc, rcClient.left, rcClient.top, nWidth, nHeight, scrdc, 0, 0, SRCCOPY);

		Gdiplus::Bitmap bitmap(membit, NULL);
		CLSID clsid;
		GetEncoderClsid(L"image/jpeg", &clsid);

		SYSTEMTIME SysTime;
		GetLocalTime(&SysTime);

		bitmap.Save(L"info\\crash.jpg", &clsid);

		SelectObject(memdc, hOldBitmap);

		DeleteObject(memdc);

		DeleteObject(membit);

		::ReleaseDC(0,scrdc);
	}

	Gdiplus::GdiplusShutdown(gdiplusToken);

	return true;
}

void SetRaidModeMVPTreasureCard( int iMVPTreasureCard )
{
	s_iMVPTreasureCard = iMVPTreasureCard;
}

int GetRaidModeMVPTreasureCard()
{
	return s_iMVPTreasureCard;
}

int GetTournamentDateYear( DWORD dwDate )
{
	return 2000 + ( dwDate / 100000000 );
}

int GetTournamentDateMonth( DWORD dwDate )
{
	return ( dwDate % 100000000 ) / 1000000;
}

int GetTournamentDateDay( DWORD dwDate )
{
	return ( dwDate % 1000000 ) / 10000;
}

int GetTournamentDateHour( DWORD dwDate )
{
	return ( dwDate % 10000 ) / 100;
}

int GetTournamentDateMinute( DWORD dwDate )
{
	return ( dwDate % 100 );
}

DWORD GetConvertDwDate( int iYear, int iMonth, int iDay, int iHour, int iMin )
{
	DWORD dwDate = 0;

	dwDate += ( iYear - 2000 ) * 100000000;
	dwDate += iMonth * 1000000;
	dwDate += iDay * 10000;
	dwDate += iHour * 100;
	dwDate += iMin;

	return dwDate;
}

int GetTodayOfYear()
{
	time_t today;
	time( &today );
	struct tm* tmday;
	tmday = localtime( &today );
	
	return 1900 + tmday->tm_year;
}

int GetTodayOfMon()
{
	time_t today;
	time( &today );
	struct tm* tmday;
	tmday = localtime( &today );
	return tmday->tm_mon+1;
}

int GetTodayOfWeek()
{
	time_t today;
	time( &today );
	struct tm* tmday;
	tmday = localtime( &today );
	return tmday->tm_wday;
}

int GetTodayOfDay()
{
	time_t today;
	time( &today );
	struct tm* tmday;
	tmday = localtime( &today );

	return tmday->tm_mday;
}

int GetTodayOfHour()
{
	time_t today;
	time( &today );
	struct tm* tmday;
	tmday = localtime( &today );

	return tmday->tm_hour;
}

int GetTodayOfMin()
{
	time_t today;
	time( &today );
	struct tm* tmday;
	tmday = localtime( &today );

	return tmday->tm_min;
}

int togetday( int num, int mon )              // �Է��� ���� �� ĥ������ ���Ѵ�.
{ 
	int nm[12]={ 31,28,31,30,31,30,31,31,30,31,30,31 }, i = 0;
	if(mon  > 0 && mon < 13)
	{  
		if(mon == 2)
		{
			nm[1] = (num%4) ? 28 : (num%100) ? 29 : (num%400) ? 28 : 29;  // �������� üũ �Ѵ�. 
		}
		i = nm[mon-1];
	}
	return  i;
}
char *weekday( int ky ) //  ���ڷ� ��ȯ �Ѵ�.
{
	char a[7][3];
	ZeroMemory( a, sizeof( a ) );
	for (int i = 0; i < 7 ; i++)
	{
		if( i == 0 )
			SafeSprintf( a[i], sizeof( a[i] ), STR(1) );
		else if( i == 1 )
			SafeSprintf( a[i], sizeof( a[i] ), STR(2) );
		else if( i == 2 )
			SafeSprintf( a[i], sizeof( a[i] ), STR(3) );
		else if( i == 3 )
			SafeSprintf( a[i], sizeof( a[i] ), STR(4) );
		else if( i == 4 )
			SafeSprintf( a[i], sizeof( a[i] ), STR(5) );
		else if( i == 5 )
			SafeSprintf( a[i], sizeof( a[i] ), STR(6) );
		else if( i == 6 )
			SafeSprintf( a[i], sizeof( a[i] ), STR(7) );
	}

	static char b[3];
	ZeroMemory( b, sizeof( b ) );
	SafeSprintf( b, sizeof( b ), STR(8) );

	if( ky >= 7 )  // ���� ó��
		return b;

	b[0] = a[ky][0]; b[1] = a[ky][1];
	return b;
}
char * getweekday( int num, int mon, int day )       // �Է��� ��¥�� ���� ���ϱ�
{
	int iy=num-1, nal = iy * 365;
	nal += iy/4-iy/100+iy/400;                 // ������ �� ���־ �Է��� �մޱ����� �� �ϼ��� ���Ѵ�.
	for(iy=1; iy<mon; iy++) 
	{
		nal +=  togetday( num, iy);
	}
	return weekday( (nal+day)%7);
}

char *GetTournamentDateDayOfWeek( DWORD dwDate )
{
	int Y = GetTournamentDateYear( dwDate );
	int M = GetTournamentDateMonth( dwDate );
	int D = GetTournamentDateDay( dwDate );

	return getweekday( Y, M, D );
}

bool isLeapYear( int iYear )
{
	if( ( iYear - 2000 ) % 4  == 0 )
		return true;

	return false;
}

bool isOddMonth( int iMonth )
{
	int nm[12]={ 31,28,31,30,31,30,31,31,30,31,30,31 };

	if( nm[iMonth-1] == 31 )
		return true;
	else
		return false;
}

void IncreaseMonth( int& iYear, int& iMonth, int& iDay )
{
	if( 12 == iMonth )
	{
		iMonth = 1;
		iYear++;
	}
	else
	{
		iMonth++;
	}

	iDay = 1;	
}

void DecreaseMonth( int& iYear, int& iMonth, int& iDay )
{
	if( 1 == iMonth )
	{
		iMonth = 12;
		iYear--;
	}
	else
	{
		iMonth--;
	}

	iDay = 1;
}


void IncreaseDay( int& iYear, int& iMonth, int& iDay )
{
	int nm[12]={ 31,28,31,30,31,30,31,31,30,31,30,31 };
	if( isLeapYear(iYear) )
		nm[1] = 29;
	
	if( iDay < nm[iMonth-1] )
	{
		iDay++;
	}
	else
	{
		if( iMonth == 12 )
		{
			iYear++;
			iMonth = 1;
			iDay = 1;
		}
		else
		{
			iMonth++;
			iDay = 1;
		}
	}
}

void DecreaseDay( int& iYear, int& iMonth, int& iDay )
{
	int nm[12]={ 31,28,31,30,31,30,31,31,30,31,30,31 };
	if( isLeapYear(iYear) )
		nm[1] = 29;

	if( 1 < iDay )
	{
		iDay--;
	}
	else
	{
		if( iMonth == 1 )
		{
			iYear--;
			iMonth = 12;
			iDay = nm[11];
		}
		else
		{
			iMonth--;
			iDay = nm[iMonth-1];
		}
	}
}

void IncreaseMin( int& iYear, int& iMonth, int& iDay, int& iHour, int& iMin, int iIncraseMin )
{
	int nm[12]={ 31,28,31,30,31,30,31,31,30,31,30,31 };
	if( isLeapYear(iYear) )
		nm[1] = 29;
	
	int iCurDay = 0;

	if( 60 <= iIncraseMin + iMin )
	{
		iHour += (iIncraseMin + iMin) / 60;
		iMin = (iIncraseMin + iMin) % 60;

		if( 24 <= iHour )
		{
			iCurDay = iDay + (iHour / 24);
			iHour   = iHour % 24;

			int iRestDay = iCurDay - iDay;
			for( int i = 0; i < iRestDay; ++i )
				IncreaseDay( iYear, iMonth, iDay );

		}
	}
	else
	{
		iMin += iIncraseMin;
	}

}

bool IsMonsterDungeonMode(ModeType eMode)
{
	switch (eMode)
	{
	case MT_TOWER_DEFENSE:
	case MT_DARK_XMAS:
	case MT_FIRE_TEMPLE:
	case MT_FACTORY:
		return true;
	}

	return false;
}

void GetParsingDate( IN int iDate, IN int iYearOffset, OUT int &iYear, OUT int &iMonth, OUT int &iDay, OUT int &iHour, OUT int &iMinute )
{
	iYear   =   iDate / 100000000 + iYearOffset;
	iMonth  = ( iDate / 1000000 ) % 100;
	iDay    = ( iDate / 10000 ) % 100;
	iHour   = ( iDate / 100 ) % 100;
	iMinute =   iDate % 100;
}
D3DXQUATERNION ConvertDirToQuaternion( const D3DXVECTOR3 &vDir )
{
	D3DXQUATERNION qtRot;
	D3DXVECTOR3 vUp = ioMath::UNIT_Y;
	D3DXVECTOR3 vSide, vFront;

	D3DXVec3Cross( &vSide, &vUp, &vDir );
	D3DXVec3Cross( &vFront, &vSide, &vUp );
	D3DXVec3Normalize( &vSide, &vSide );
	D3DXVec3Normalize( &vFront, &vFront );

	float fYaw = D3DXVec3Dot( &vFront, &-ioMath::UNIT_Z );
	float fPitch = D3DXVec3Dot( &vFront, &vDir );
	float fCheckYaw = D3DXVec3Dot( &vDir, &ioMath::UNIT_X );
	float fCheckPitch = D3DXVec3Dot( &vDir, &ioMath::UNIT_Y );

	if( fPitch >= FLOAT1 )
	{
		ioMath::CalcDirectionQuaternion( qtRot, -vDir );
	}
	else if( fPitch <= -FLOAT1 )
	{
		float fYawAngle = acos(fYaw);
		float fPitchAngle = acos(fPitch);
		D3DXQuaternionRotationYawPitchRoll( &qtRot, fYawAngle, fPitchAngle, 0.0f );
	}
	else
	{
		float fYawAngle;
		if( fYaw >= FLOAT1 )
		{
			fYawAngle = 0.0f;
		}
		else
		{
			if( fYaw <= -FLOAT1 )
				fYawAngle = D3DX_PI;
			else
				fYawAngle = acos(fYaw);

			if( fCheckYaw > 0.0f )
				fYawAngle *= -FLOAT1;
		}

		float fPitchAngle = acos(fPitch);
		if( fPitch >= FLOAT1 )
		{
			fPitchAngle = 0.0f;
		}
		else
		{
			fPitchAngle = acos(fPitch);
			if( fCheckPitch < 0.0f )
				fPitchAngle *= -FLOAT1;
		}

		D3DXQuaternionRotationYawPitchRoll( &qtRot, fYawAngle, fPitchAngle, 0.0f );
	}

	return qtRot;
}

bool CheckHanByte( char *szText )
{
	int len = strlen(szText);
	int hbyte_count = 0;
	for(int i = 0; i < len; i++ )
	{

#if defined( SRC_OVERSEAS ) && !defined( SRC_ID )

#if defined( MULTI_BYTE_CHECK )
		if( IsDBCSLeadByteEx( COUNTRY_CODE_PAGE, (BYTE)szText[i] ) )
#else
		if( false && IsDBCSLeadByteEx( COUNTRY_CODE_PAGE, (BYTE)szText[i] ) )
#endif

#else
		if( IsDBCSLeadByte( (BYTE)szText[i] ) )
#endif

		{
			i++;		
			if(len <= i) // ������ �ѱ��� ���� ���ڴ�.
				return true;
		}
	}

	return false;
}

void SplitTextWidth( std::string szSource, StringVector& szOutSplitList, int iCutWith, TextStyle eStyle, float fTextScale )
{
	//Width üũ�� ���� �ӽ� ����
	char szBuffer[MAX_EDIT_TEXT];
	memset( szBuffer, 0, sizeof( szBuffer ) );

	std::string::const_iterator iter = szSource.begin();
	float fWdith = 0;
	for( int i = 0; iter != szSource.end(); ++iter, ++i )
	{
		szBuffer[i] = *iter;
		float fWidth = g_FontMgr.GetTextWidth( szBuffer, eStyle, fTextScale );
		if( iCutWith < fWidth )
		{
			szBuffer[i] = NULL;
			if( CheckHanByte( szBuffer ) )
			{
				szBuffer[--i] = NULL;
			}

			szOutSplitList.push_back( szSource.substr( 0, i ) );
			szSource = szSource.substr( i, szSource.length() );
			SplitTextWidth( szSource, szOutSplitList, iCutWith, eStyle, fTextScale );
			return;
		}
	}

	if( !szSource.empty() )
		szOutSplitList.push_back( szSource );
}

bool IsOnlyEngHanNumText( const char *szText )
{
	int iSize = strlen( szText );
	if( iSize < 1 )	return false;

	ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
	if( !pLocal ) 
		return false;

	for(int i = 0;i < iSize;i++)
	{
		if( !COMPARE( szText[i], 'A', 'Z'+1) && !COMPARE( szText[i], 'a', 'z'+1 ) && !COMPARE( szText[i], '0', '9'+1 ) && szText[i] != '-' )
		{
			if( i < iSize - 1 )
			{
				if( pLocal->IsCheckKorean() )
				{
					// �ѱ� ���� ���
					if( (byte)szText[i] == 0xa4 && (byte)szText[i+1] >= 0xa1 && (byte)szText[i+1] <= 0xd3 )
					{
						return false;
					}
					if( (byte)szText[i] >= 0xb0 && (byte)szText[i] <= 0xc8 && (byte)szText[i+1] >= 0xa1 && (byte)szText[i+1] <= 0xfe )
					{
						i++;
						continue;
					}
				}
				else
				{

#if defined( SRC_OVERSEAS ) && !defined( SRC_ID )

#if defined( MULTI_BYTE_CHECK )
					if( IsDBCSLeadByteEx( COUNTRY_CODE_PAGE, (byte)szText[i] ) )
#else
					if( false && IsDBCSLeadByteEx( COUNTRY_CODE_PAGE, (byte)szText[i] ) )
#endif

#else
					if( IsDBCSLeadByte( (byte)szText[i] ) )
#endif

					{
						i++;
						continue;
					}
				}
			}
			return false;
		}
	}
	return true;
}

bool CheckCharColState( DWORD dwStartTime, DWORD dwEndTime,
						 DWORD dwSkipType, bool bTeamOnly, bool bDefense,
						 TeamType eOwnerTeam, TeamType eOtherTeam )
{
	DWORD dwCurTime = FRAMEGETTIME();

	bool bSkip = false;

	if( COMPARE(dwCurTime, dwStartTime, dwEndTime) )
	{
		if( eOwnerTeam == eOtherTeam )
		{
			bSkip = true;
		}
		else
		{
			if( bDefense && (dwSkipType/10) == 1)
				bSkip = false;
			else if( bTeamOnly )
				bSkip = false;
			else
				bSkip = true;
		}
	}

	return bSkip;
}


bool CheckWeaponBuffNoCollision( CharState eState )
{
	switch( eState )
	{
	case CS_LOCKUP_BUFF:
		return true;
	}

	return false;
}

bool CheckTargetNoCollision( CharState eState )
{
	switch( eState )
	{
	case CS_LOCKUP_BUFF:
		return true;
	}

	return false;
}

bool CheckMineNoCollision( CharState eState )
{
	switch( eState )
	{
	case CS_LOCKUP_BUFF:
		return true;
	}

	return false;
}

float GetHalfSineValue( float max, float cur )
{
	float fValue = (cur / max) * D3DX_PI;
	return sinf( fValue );
}

void LoadDummyCharCreateDestroyInfo( ioINILoader &rkLoader,
									 vDummyCharLoadInfoList &rkCreateList,
									 ioHashStringVec &rkDestroyList )
{
	char szBuf[MAX_PATH] = "";
	char szKey[MAX_PATH] = "";

	int iDummyCnt = rkLoader.LoadInt_e( "dummy_char_cnt", 0 );
	rkCreateList.clear();

	for( int j=0; j < iDummyCnt; ++j )
	{
		DummyCharLoadInfo kLoadInfo;

		wsprintf_e( szKey, "dummy_char%d_name", j+1 );
		rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );
		kLoadInfo.m_DummyCharName = szBuf;

		wsprintf_e( szKey, "dummy_char%d_offset", j+1 );
		kLoadInfo.m_fDummyCharOffset = rkLoader.LoadFloat( szKey, 0.0f );

		wsprintf_e( szKey, "dummy_char%d_side_offset", j+1 );
		kLoadInfo.m_fDummyCharSideOffset = rkLoader.LoadFloat( szKey, 0.0f );

		wsprintf_e( szKey, "dummy_char%d_start_angle", j+1 );
		kLoadInfo.m_fDummyCharStartAngle = rkLoader.LoadFloat( szKey, 0.0f );

		rkCreateList.push_back( kLoadInfo );
	}

	int iDestroyCnt = rkLoader.LoadInt_e( "destroy_dummy_cnt", 0 );
	rkDestroyList.clear();
	for( int k=0; k < iDestroyCnt; ++k )
	{
		wsprintf_e( szKey, "destroy_dummy%d_name", k+1 );
		rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );

		rkDestroyList.push_back( szBuf );
	}
}

int GetNumberLength( int number )
{
	if( number <= 0 )
		return 1;

	int Seq = (int)(log10( (float)number ) + FLOAT1);	
	return Seq;
}

void GetNumberSplit( int number, DWORDVec& NumVec )
{
	int Seq = GetNumberLength( number );	

	for( int i = Seq; i >= 1; --i )
	{
		int val = number / pow( (double)(FLOAT10), (double)i-FLOAT1 );
		NumVec.push_back( val );
		number = number - (pow( (double)(FLOAT10), (double)i-FLOAT1 ) * val );
	}
}

bool IsAvailableDate( int iYear, int iMonth, int iDay )
{
	// 2000 ~ 3000 Year
	if( iYear <= 0 || 99 < iYear )
	{
		return false;
	}

	if( iMonth < 1 || 12 < iMonth )
	{
		return false;
	}

	if( iDay < 1 || DateHelp::GetMonthDayPeriod( iYear, iMonth ) < iDay )
	{
		return false;
	}

	return true;
}

int GetProcessRunningCount( const ioHashString& szProcessName )
{
	PROCESSENTRY32 Entry = { sizeof(PROCESSENTRY32) };
	HANDLE hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );

	int iCount = 0;
	if( Process32First( hSnapshot, &Entry ) )
	{
		do
		{
			if( strcmp( Entry.szExeFile, szProcessName.c_str() ) == 0 )
			{				
				iCount++;
			}
		} 
		while( Process32Next( hSnapshot, &Entry ) );
	}

	CloseHandle( hSnapshot );
	return iCount;
} 


bool FullPathGenerate( const TCHAR* FileName, TCHAR* szFullPathFileName, size_t iSize )
{
	TCHAR szBuf[MAX_PATH+1];
	GetModuleFileName(NULL, szBuf, MAX_PATH);

	TCHAR* token = _tcsrchr(szBuf, _T('\\'));
	*(token+1) = _T('\0');
		
	_stprintf_s( szFullPathFileName, iSize, _T("%s%s"), szBuf, FileName );
	return true;
}


bool HexToString( std::string & outStr, const unsigned char * hexArray, size_t hexSize )
{
	// �ִ밪 128
	if(!hexArray)
		return false;
	if(hexSize > ((MAX_PATH / 2) - 1) || hexSize < 1)
		return false;

	char strTemp[MAX_PATH] = {0,};
	for(int i = 0; i < hexSize; ++i)
	{
		sprintf_e(strTemp, "%s%x", strTemp, hexArray[i]);
	}
	outStr = strTemp;

	return true;
}

void LoadFileInResource(int name, const char * type, DWORD& size, const char*& data)
{
	HMODULE handle = ::GetModuleHandle(NULL);
	HRSRC rc = ::FindResource(handle, MAKEINTRESOURCE(name), type);
	HGLOBAL rcData = ::LoadResource(handle, rc);
	size = ::SizeofResource(handle, rc);
	data = static_cast<const char*>(::LockResource(rcData));
}

void LoadTextInResource( int name, DWORD& size, const char*& data )
{
	LoadFileInResource(name, TEXT_RESOURCE, size, data);
}

int MessageboxShow( HWND hwnd, const char * pStr, const char * pCaption, UINT uType )
{
	if(pStr && pCaption)
	{
#ifdef MESSAGEBOX_UNICODE
		std::wstring strText = CA2W(pStr, COUNTRY_CODE_PAGE);
		std::wstring strCaption = CA2W(pCaption, COUNTRY_CODE_PAGE);
		return MessageBoxW( hwnd, strText.c_str(), strCaption.c_str(), uType);
#else
		return MessageBox( hwnd, pStr, pCaption, uType);
#endif
	}
	else
	{
		return MessageBox( hwnd, "Unknown Error", "Unknown Error", uType);
	}
}

int DivideKoreanLanguage( wchar_t *str, wchar_t *buffer, UINT nSize, CharSetType eType )
{
	//�ʼ� 
	static const wchar_t wcHead[] = {L'��', L'��', L'��', L'��', 
		L'��', L'��', L'��', L'��', 
		L'��', L'��', L'��', L'��', 
		L'��', L'��', L'��', L'��', 
		L'��', L'��', L'��'}; 

	//�߼� 
	static const wchar_t wcMid[] = {L'��', L'��', L'��', L'��', 
		L'��', L'��', L'��', L'��', 
		L'��', L'��', L'��', L'��', 
		L'��', L'��', L'��', L'��', 
		L'��', L'��', L'��', L'��', L'��'}; 

	//���� 
	static const wchar_t wcTail[] = {L' ', L'��', L'��', L'��', 
		L'��', L'��', L'��', L'��', 
		L'��', L'��', L'��', L'��', 
		L'��', L'��', L'��', L'��', 
		L'��', L'��', L'��', L'��', 
		L'��', L'��', L'��', L'��', 
		L'��', L'��', L'��', L'��'}; 

	UINT pos = 0;

	while(*str != '\0') 
	{ 
		if(*str < 256) 
		{ 
			if(pos+2 >= nSize-1) 
				break; 

			buffer[pos] = *str; 
			++pos; 
		} 
		else 
		{ 
			if(pos+4 >= nSize-1) 
				break; 

			if( eType == CST_HEAD )
				buffer[pos] = wcHead[(*str - 0xAC00) / (21*28)]; 
			else if( eType == CST_MID )
				buffer[pos] = wcMid[(*str - 0xAC00) % (21 * 28) / 28]; 
			else if( eType == CST_TAIL )
				buffer[pos] = wcTail[(*str - 0xAC00) % 28]; 

			pos++;
		} 

		++str; 
	} 

	buffer[pos] = '\0'; 
	return pos; 
}

bool Strstr( const char* szSrc, const char* szKeyWord )
{
	if( strstr( szSrc, szKeyWord ) == NULL )
	{
		wchar_t wSrc[ MAX_PATH ];
		wchar_t wKeyWord[ MAX_PATH ];
		wchar_t wDevideWord[ MAX_PATH ];

		MultiByteToWideChar( CP_ACP, 0, szKeyWord, -1, wKeyWord, MAX_PATH );
		MultiByteToWideChar( CP_ACP, 0, szSrc, -1, wSrc, MAX_PATH );

		Help::DivideKoreanLanguage( wSrc, wDevideWord, sizeof( wDevideWord ), CST_HEAD );		
		if( wcsstr( wDevideWord, wKeyWord ) == NULL)
			return false;
	}

	return true;
}


} // namespace Help End

float HALFToFloat(HALF y)
{
	union { float f; uint32_t i; } v;
	v.i = halfToFloatI(y);
	return v.f;
}

uint32_t static halfToFloatI(HALF y)
{
	int s = (y >> 15) & 0x00000001;                            // sign
	int e = (y >> 10) & 0x0000001f;                            // exponent
	int f =  y        & 0x000003ff;                            // fraction

	if (e == 0) {
		if (f == 0)                                            // Plus or minus zero
			return s << 31;
		else {                                                 // Denormalized number -- renormalize it
			while (!(f & 0x00000400)) {
				f <<= 1;
				e -=  1;
			}
			e += 1;
			f &= ~0x00000400;
		}
	} else if (e == 31) {
		if (f == 0)                                             // Inf
			return (s << 31) | 0x7f800000;
		else                                                    // NaN
			return (s << 31) | 0x7f800000 | (f << 13);
	}

	e = e + (127 - 15);
	f = f << 13;

	return ((s << 31) | (e << 23) | f);
}
HALF floatToHALF(float i)
{
	union { float f; uint32_t i; } v;
	v.f = i;
	return floatToHalfI(v.i);
}

HALF static floatToHalfI(uint32_t i)
{
	register int s =  (i >> 16) & 0x00008000;                   // sign
	register int e = ((i >> 23) & 0x000000ff) - (127 - 15);     // exponent
	register int f =   i        & 0x007fffff;                   // fraction

	if (e <= 0) {
		if (e < -10) {
			if (s)                                              // handle -0.0
				return 0x8000;
			else
				return 0;
		}
		f = (f | 0x00800000) >> (1 - e);
		return s | (f >> 13);
	} else if (e == 0xff - (127 - 15)) {
		if (f == 0)                                             // Inf
			return s | 0x7c00;
		else {                                                  // NAN
			f >>= 13;
			return s | 0x7c00 | f | (f == 0);
		}
	} else {
		if (e > 30)                                             // Overflow
			return s | 0x7c00;
		return s | (e << 10) | (f >> 13);
	}
}

void encodeXYZ( D3DXVECTOR3& In, float& OutX, float& OutY )
{
	OutX = In.x * FLOAT05 + FLOAT05;
	OutY = In.y * FLOAT05 + FLOAT05;
}

void encodeXYZ( const D3DXVECTOR3& In, float& OutX, float& OutY )
{
	OutX = In.x * FLOAT05 + FLOAT05;
	OutY = In.y * FLOAT05 + FLOAT05;
}

void decodeXYZ( D3DXVECTOR3& Out, float& InX, float& InY )
{
	Out.x = InX * 2.f - FLOAT1;
	Out.y = InY * 2.f - FLOAT1;
	Out.z = sqrt( FLOAT1 - D3DXVec2Dot( &D3DXVECTOR2(Out.x,Out.y), &D3DXVECTOR2(Out.x,Out.y)) );
	D3DXVec3Normalize( &Out, &Out );
}

BYTE ConvertAngle2Byte( float fAngle )
{
	BYTE byAngle;
	float fConA = fAngle * 0.70833;
	byAngle = fConA;
	if( fConA - (int)fConA > FLOAT05 )
		byAngle += 1;

	return byAngle;
}
float ConvertByte2Angle( BYTE byAngle )
{
	float fAngle;
	float fConA = (float)byAngle * 1.41176;
	fAngle = fConA;
	if( fConA - (int)fConA > FLOAT05 )
		fAngle += FLOAT1;

	return fAngle;
}
void ConvertPackAngle2Vec( BYTE byAngle, D3DXVECTOR3& vOut )
{
	float fAngle = ConvertByte2Angle( byAngle );
	float fYawR = DEGtoRAD( fAngle );
	D3DXQUATERNION qtRotate;
	D3DXQuaternionRotationYawPitchRoll( &qtRotate, fYawR, 0.0f, 0.0f );
	vOut = qtRotate * D3DXVECTOR3( 0.0f, 0.0f, -FLOAT1 );
	D3DXVec3Normalize( &vOut, &vOut );
}

void InputHalf2Vec( HALF* pIn, D3DXVECTOR3& vOut )
{
	vOut.x = HALFToFloat(pIn[0]);
	vOut.y = HALFToFloat(pIn[1]);
	vOut.z = HALFToFloat(pIn[2]);
}

void InputHalf2Quat( HALF* pIn, D3DXQUATERNION& qtOut )
{
	qtOut.x = HALFToFloat(pIn[0]);
	qtOut.y = HALFToFloat(pIn[1]);
	qtOut.z = HALFToFloat(pIn[2]);
	qtOut.w = HALFToFloat(pIn[3]);
}

void OutputQuat2Half( D3DXQUATERNION& qtIn, HALF* pOut )
{
	pOut[0] = floatToHALF(qtIn.x);
	pOut[1] = floatToHALF(qtIn.y);
	pOut[2] = floatToHALF(qtIn.z);
	pOut[3] = floatToHALF(qtIn.w);
}

void OutputVec2Half( D3DXVECTOR3& vIn, HALF* pOut )
{
	pOut[0] = floatToHALF(vIn.x);
	pOut[1] = floatToHALF(vIn.y);
	pOut[2] = floatToHALF(vIn.z);
}

void Quat2Angle( const D3DXQUATERNION &qt, D3DXVECTOR3& vOut )
{
	double pitch, roll, yaw;
	double sqw = qt.w * qt.w;
	double sqx = qt.x * qt.x;
	double sqy = qt.y * qt.y;
	double sqz = qt.z * qt.z;
	double unit = sqx + sqy + sqz + sqw;
	double test = (qt.x * qt.y) + (qt.z * qt.w);
	if (test > 0.499 * unit)
	{
		pitch = 0.0;
		roll  = 2.0 * atan2(qt.x, qt.w);
		yaw   = D3DX_PI;
		vOut.x = pitch;
		vOut.y = roll;
		vOut.z = yaw;
		return;
	}
	if (test < -0.499 * unit)
	{
		pitch = 0.0;
		roll  = -2.0 * atan2(qt.x, qt.w);
		yaw   = -D3DX_PI;

		vOut.x = pitch;
		vOut.y = roll;
		vOut.z = yaw;
		return;
	}
	pitch = atan2((2.0 * qt.x * qt.w) - (2.0 * qt.y * qt.z), -sqx + sqy - sqz + sqw);
	roll  = atan2((2.0 * qt.y * qt.w) - (2.0 * qt.x * qt.z),  sqx - sqy - sqz + sqw);
	yaw   = asin(2.0 * test / unit);

	vOut.x = pitch;
	vOut.y = roll;
	vOut.z = yaw;
}

void ConvertForceXYZ2Vec( D3DXVECTOR3 vIn, D3DXVECTOR3& vOut )
{
	const float yPerBit = 0.0078125;	// >>  2 / 256
	vIn *= yPerBit;
	vIn += D3DXVECTOR3( -FLOAT1, -FLOAT1, -FLOAT1 );
	D3DXVec3Normalize( &vOut, &vIn );
}

float CompactFNormalized( float In )
{
	if( In <= -FLOAT1 )
		return 0.f;

	if( In >= FLOAT1 )
		return 255.f;

	const float yPerBit = 0.0078125;	// >>  2 / 256
	In += FLOAT1;	
	float float2f = In / yPerBit;
	if( float2f >= 255.f )
		return 255.f;

	return float2f;

}