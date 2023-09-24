#include "stdafx.h"

#include "ioApplication.h"

#include "ioBonusCashManager.h"

#include "ioBaseChar.h"
#include "ioLoginStage.h"
#include "ioPlayStage.h"
#include "Housing/ioBlockWorld.h"

#include "GUI/ioMemoManager.h"
#include "GUI/HackProblemWnd.h"
#include "GUI/DebugInfoWnd.h"
#include "GUI/ioExpansionBox.h"
#include "GUI/UserListWnd.h"
#include "GUI/ItemCustomWnd.h"
#include "GUI/TournamentMainWnd.h"
#include "GUI/MessengerWnd.h"
#include "GUI/RelativeGradeBonusWnd.h"
#include "GUI/ExPCRoomWnd.h"
#include "GUI/LadderTeamWnd.h"
#include "GUI/VoiceChatWnd.h"
#include "GUI/SoldierManualWnd.h"
#include "GUI/AnnouncementWnd.h"
#include "GUI/UserInfoToolTip.h"
#include "GUI/MannerValuationWnd.h"
#include "GUI/GameOptionWnd.h"
#include "GUI/EventWnd.h"
#include "GUI/GameOptionSettingTab.h"
#include "GUI/SoldierSelectWnd.h"
#include "GUI/MiniSoldierSelectWnd.h"
#include "GUI/ioGrowthSelectWnd.h"
#include "GUI/UISoldierSelectWnd.h"
#include "GUI/MyInventoryInfoWnd.h"
#include "GUI/AbuseProblemWnd.h"
#include "GUI/GuildWnd.h"
#include "GUI/CampMainWnd.h"
#include "GUI/ioInterfaceBackground.h"
#include "GUI/DisconnectWnd.h"
#include "GUI/GuildMarkChangeWnd.h"
#include "GUI/ChatMacro.h"
#include "GUI/GashaponPresentListWnd.h"
#include "GUI/AttendanceWnd.h"
#include "GUI/TimeGateSearch.h"
#include "GUI/HelpWnd.h"
#include "GUI/CostumeCustomWnd.h"
#include "GUI/RollBookWnd.h"
#include "GUI/GuildRewardWnd.h"
#include "GUI/ioShopDataCache.h"

#include "resource.h"
#include "CrashFind/BugslayerUtil.h"
#include "ioClientUDPSocket.h"
#include "ioUserNetworkTable.h"

#include "MiniDump/MiniDump.h"
#include "MiniDump/File.h"

#include "ioGuildMarkMgr.h"
#include "ioGuildChatMgr.h"

#include "ioEventGoodsMgr.h"
#include "ioDecorationPrice.h"
#include "ioTreasureCardMgr.h"
#include "ioMapEventSceneMgr.h"
#include "RaceSetting.h"
#include "ioFieldItemMgr.h"
#include "ioTournamentManager.h"
#include "ioTournamentTeamManager.h"
#include "ioTournamentRewardManager.h"
#include "ioItemRechargeManager.h"

#include "ioModeItemManager.h"

#include "ioCollisionBoxGrpManager.h"
#include "ioStatAttribute.h"
#include "ioFishingManager.h"
#include "ioAlchemicMgr.h"
#include "ioSubscriptionMgr.h"

#include "ioAISymbolTable.h"
#include "ioAIDatabase.h"

#include "ioSlotMgr.h"

#include "ioScreenShotTask.h"
 
#include "ioTickTimer.h"
#include "ioLoadingRender.h"

#include "Login.h"

#include "BlockKeyBoard.h"
 
#include "ioBrowserManager.h"
#include "ioSaleManager.h"
#include "ioExcavationManager.h"

#include "ioReservedTask.h"

#include "ioSuperGashaponMgr.h"
#include "ioGashaponMgr.h"

#include "ioShuffleRoomMgr.h"
#include "ioSoldierAwakeManager.h"
#include "ioBasicTooltipManager.h"
#include "TournamentPacketParser.h"
#include "ioModeResourceManager.h"
#include "ioSpecialShopMgr.h"
#include "ioShopManager.h"
#include "ioPowerUpManager.h"
#include "ioNpcChar.h"

#include "ioMissionManager.h"
#include "ioRenderTargetManager.h"
#include "ioRsoldierInfoManager.h"
#include "GUI/ioShopDataCache.h"
#include "Housing/ioUserBlockStorage.h"
#include "Housing/ioBlockEventAgency.h"
#include "ioPriateRullet.h"
#include "ioExtendSoundManager.h"
#include "ioAccessoryInfoManager.h"


#include "Channeling/ioChannelingNodeManager.h"
#include "Channeling/ioChannelingNodeParent.h"




#include "GUI/PopUpStoreWnd.h"

#ifdef STEAM_ATTACH
#include "ioSteamClient.h"
#endif

//#include <omp.h>

#include "Version.h"

extern luaDef myGlue[];

extern bool g_bHappenCrash;
extern bool g_bHappenError;
extern int  g_iErrorNumber;
extern char g_szErrorLog[2048];
extern char g_szErrorPos[MAX_PATH];
extern char g_szTempLogBuf[2048];
extern char g_szSendErrorLog[4096];
bool m_bNetworkPlay = false;

extern CLog g_MemoryLOG;

DWORD g_POIThreadId = 0;

void CheckPacExceptionLog();
extern LONG __stdcall ExceptCallBack( EXCEPTION_POINTERS *pExPtrs );
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

template<> ioApplication* Singleton< ioApplication >::ms_Singleton = 0;

#define APPLICATION_NAME "Lost Saga in Timegate - Client" // �ڵ����׷��̵�� ����� �ϹǷ� ���� ����
#define WINDOW_STYLE (WS_OVERLAPPED | WS_CAPTION | WS_CLIPCHILDREN)//| WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)
#define FULLSCREEN_STYLE (WS_POPUP | WS_CLIPCHILDREN)

#ifndef GET_X_LPARAM
	#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#endif

#ifndef GET_Y_LPARAM
	#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
#endif

// IME Ÿ�̸�
#define WM_IO_IMETIMER	777

bool g_bExit = false;
#ifdef _ITEST
bool g_bTestAntiHack = false;
#endif


#ifdef ANTIHACK
bool s_RUDPLOG = false;
DWORD dwRUDPMaximum = 300;		//��Ŷ��������
DWORD dwReqTimeInterval = 100;	//ms
DWORD dwAckTimeInterval = 50;	//ms
DWORD dwRUDPTryCnt = 10;

const DWORD RUDP_SKIP_MAX = 100;
#endif

bool ioApplication::s_bPenaltyTest = false;
bool ioApplication::s_bAntiHackNotUse = false;


ioApplication::ioApplication()
{
	// OpenLog ���̹Ƿ� Log ������� ����
	///////////////////////////////////
	//Relay
	m_bRelayUse = false;
	m_relayPort = -1;
	m_realyIpaddr[0] = 0;
	///////////////////////////////////
	//���� 
	m_hInstance = NULL;
	m_hWnd = NULL;

	m_bAppActive = FALSE;
	m_hWindowTitleLosa = NULL;
	m_hWindowOutLine = NULL;
	m_bFocus = FALSE;
	
	m_dwResetSoundTime = 0;

	m_bSetUpDone = false;

	m_pMathManager = NULL;
	m_pDropManager = NULL;

	m_iWidth  = 0;
	m_iHeight = 0;

	m_iDeskTopWidth  = 0;
	m_iDeskTopHeight = 0;

	m_pLogin = NULL;
	m_pTickTimer = NULL;

	m_ptPreMouse.x = 0;
	m_ptPreMouse.y = 0;
	m_ptMouseMoveAmt.x = 0;
	m_ptMouseMoveAmt.y = 0;
	m_dwMousePreCheckTime = 0;
	m_dwMouseUnvisiblTime = 0;
	m_dwMouseUnvisiblLobbyTime = 0;

	m_ptPreDragPos.x = 0;
	m_ptPreDragPos.y = 0;
	m_bWindowTitleDragging = false;

	m_pGameStage = NULL;
	m_NextStage  = ioGameStage::GS_LOBBY;
	m_iModeType  = MT_NONE;
	m_iModeSubNum = 0;
	m_iModeMapNum = 0;
	m_bCampBattle = false;
	m_bCampHeroBattle = false;
	m_ePlazaType  = PT_NONE;
	m_bReloadINILists = false;

	m_iFrameSkipCount = 0;

	m_dwLastFaceCountTime = 0;
	m_iFramePerFaceCount = 0;
	m_iTotalFrameFaceCount = 0;
	m_iPastedFrameCount = 0;

	m_bFPSRender = false;
	m_bFirstSound = true;

	m_pRenderSystem		= NULL;

	m_pMeshMgr			= NULL;
	m_pMaterialMgr		= NULL;
	m_pAnimationMgr		= NULL;
	m_pAnimationEventManager	= NULL;
	m_pSkeletonMgr		= NULL;
	m_pTextureMgr		= NULL;
	m_pShaderMgr		= NULL;
	m_pFontMgr			= NULL;
	m_pUIRenderer		= NULL;
	m_pGUIMgr			= NULL;
	m_pChatMgr			= NULL;
	m_pInputBox			= NULL;
	m_pSoundMgr			= NULL;
	m_pExSoundMgr		= NULL;
	m_pDxInput			= NULL;
	m_pFrameTimer		= NULL;
	m_pGlobalTimer		= NULL;
	m_pTexEffMgr		= NULL;
	m_pEffectFac		= NULL;
	m_pVtxBufMgr		= NULL;
	m_pIdxBufMgr		= NULL;
	m_pOpcodeMgr		= NULL;
	m_pAnimateFXMgr		= NULL;
	m_pMathManager		= NULL;
	m_pDropManager		= NULL;
	m_pTrailDataMgr		= NULL;

	m_pSceneMgr	 = NULL;
	m_pCamera = NULL;

	m_pTCPSocket = NULL;
	m_pUDPSocket = NULL;
	m_pNetworkTable = NULL;
	m_pMyInfo     = NULL;
	m_pP2PChecker = NULL;
	m_pTCPChecker = NULL;
	m_pMyLevelMgr = NULL;
	m_pBattleRoomMgr = NULL;
	m_pLadderTeamMgr  = NULL;
	m_pShuffleRoomMgr = NULL;
	m_pModeSelectInfo = NULL;
	m_pClassPrice= NULL;
	m_pEventGoodsMgr= NULL;
	m_pDecorationPrice = NULL;
	m_pAbuseMgr = NULL;
	m_pGuildInfo = NULL;
	m_pCampManager= NULL;
	m_pTournamentManager = NULL;
	m_pTournamentTeamManager = NULL;
	m_pTournamentRewardManager	= NULL;
	m_pQuestManager= NULL;
	m_pQuestGuide= NULL;
	m_pFieldRewardItemMgr = NULL;
	m_pBackground = NULL;
	m_pNpcMgr     = NULL;
	m_pSymbolTable   = NULL;
	m_pAIDB   = NULL;
	m_pMapEventSceneMgr = NULL;
	m_pFieldItemMgr = NULL;
	
	m_pCollisionGrpMgr = NULL;
	
	m_pWeaponMgr = NULL;
	m_pItemMaker = NULL;
	m_pSkillMaker = NULL;
	m_pBuffMaker = NULL;

	m_pChatMacro = NULL;
	m_pChatMacroMgr = NULL;
	m_pStatAttribute = NULL;
	m_pSetItemInfoMgr = NULL;
	m_pEtcItemMgr   = NULL;
	m_pTutorialMgr	= NULL;
	m_pMemoMgr      = NULL;
	m_pFriendMgr	= NULL;
	m_pBlackListMgr = NULL;
	m_pChannelMgr	= NULL;
	m_pModeHelpMgr	= NULL;
	m_pAlarmMgr     = NULL;
	m_pTotalManualMgr    = NULL;
	m_pSoldierManualMgr  = NULL;
	m_pAnnouncementManualMgr = NULL;
	m_pItemManualMgr     = NULL;
	m_pSkillManualMgr    = NULL;
	m_pDecoManualMgr     = NULL;
	m_pEtcManualMgr      = NULL;
	m_pEventGoodsManualMgr = NULL;
	m_pEtcInvenManualMgr = NULL;
	m_pEtcInvenSubManualMgr = NULL;
	m_pExtraInvenManualMgr  = NULL;
	m_pMedalInvenManualMgr  = NULL;
	m_pPresentInvenManualMgr = NULL;
	m_pDecoInvenManualMgr   = NULL;
	m_pItemInvenManualMgr   = NULL;
	m_pAlchemicFuncManualMgr = NULL;
	m_pPetInvenManualMgr = NULL;
	m_pCostumeInvenManualMgr = NULL;
	m_pCostumeShopManualMgr = NULL;
	m_pPopupStoreManualMgr	= NULL;
	m_pAccessoryInvenManualMgr = NULL;

	m_pAwardMgr     = NULL;
	m_pModeUIController = NULL;
	m_pVictoriesMgr = NULL;
	m_pPlayBonusMgr = NULL;
	m_pTreasureCardMgr = NULL;
	m_pGuildMarkMgr = NULL;
	m_pGuildChatMgr = NULL;
	m_pCustomTextureMgr = NULL;
	m_pUserInfoMgr	= NULL;
	m_pGrowthInfoMgr = NULL;
	m_pFishingMgr = NULL;
	m_pChannelingMgr = NULL;

	m_pGlobalSoldierInfo = NULL;

	m_pBrowserMgr	= NULL;

	m_pSlangUtil     = NULL;
	m_pBlockKeyBoard = NULL;

	m_pLoadingRender = NULL;

	m_fDefaultFov  = 45.0f;
	m_fDefaultNear = FLOAT100;
	m_fDefaultFar  = 10000.0f;

	m_pVoiceDisableListMgr = NULL;
	m_pVoiceOffListMgr     = NULL;
	m_pEventMgr            = NULL;
	m_pCustomSoundMgr      = NULL;
	m_pPresentMgr          = NULL;
	m_pSubscriptionMgr	= NULL;
	m_pMovieMaker          = NULL;
	m_pGameAnnMgr		   = NULL;
	m_pMannerTrialChatMgr  = NULL;
	m_pExtraItemInfoMgr    = NULL;
	m_pShopItemExtraInfoManager    = NULL;
	m_pItemCompoundMgr	   = NULL;
	m_pItemCompoundMaterialMgr = NULL;
	m_pRoomOptionMgr	   = NULL;
	m_pSaleMgr             = NULL;
	m_pFileWriteClient     = NULL;
	m_pExcavationMgr       = NULL;
	m_pMedalItemInfoMgr    = NULL;
	m_pExpandMedalSlotManager = NULL;
	m_pTradeInfoMgr			= NULL;
	m_pAlchemicMgr			= NULL;
	m_pSlotMgr				= NULL;
	m_pReservedTask			= NULL;
	m_pRouletteMgr			= NULL;
	m_pSuperGashponMgr		= NULL;
	m_pItemRechargeMgr      = NULL;
	m_pModeItemMgr          = NULL;
	m_pPetInfoMgr			= NULL;
	m_pPetEatInfoMgr		= NULL;
	m_pSoldierAwakeMgr		= NULL;
	m_pBasicTooltipManager	= NULL;
	m_pModeResourceMgr		= NULL;
	m_pRequestCharMgr		= NULL;
	m_pFlashDownLoadMgr		= NULL;
	m_pPowerUpMgr			= NULL;
	m_pCostumeInfoMgr		= NULL;
	m_pCostumeShopInfoMgr	= NULL;
	m_pShopMgr				= NULL;
	m_pSpecialShopMgr		= NULL;
	m_pShopDataCache		= NULL;
	m_pMissionManager		= NULL;
	m_pRSoldierInfoManager	= NULL;

	m_pRequestLagMgr		= NULL;
	m_pUIImageRenderManager	= NULL;
	m_pNamedTitleInfoManager= NULL;

#if defined( USE_GA )
	m_pHttpMng				= NULL;
#endif
	m_pRenderTargetMgr		= NULL;
	m_pGuildBlockStorage	= NULL;
	m_pHomeBlockStorage		= NULL;

	m_pBonusCashMgr   = NULL;

	m_pAccessoryInfoMgr = NULL;

	m_bProgramExit = false;

	m_iNextUDPSendIndex = 1;
	m_iNextSendIndex = 1;
	//m_bNetworkPlay = false;

	m_bFirstMyRoom  = true;
	m_bFirstPlaza   = true;
	m_bFirstLoading = true;

	m_dwReserveLogOutTime     = 0;
	m_dwReserveLogOutCnt      = 0;
	m_dwReserveLogOutRandTime = 0;
	m_dwReserveLogOutType     = 0;

	m_pLogin = new CLogin;

	m_dwNextLanCheckTime = 0;
	m_dwPreLanCheckState = 0;

	m_dwEntryRefreshTime  = 0;
	m_dwLoadingEndTime    = 0;
	m_dwLoadingLogoTime   = 0;
	m_dwLoadingRegulation = 0;
	m_dwLoadingDelay	  = 0;
	m_dwLoadingLogoColor  = 0;
	m_dwLoadingIssueColor = 0;
	m_dwMannerTrialTime   = 0;

	m_pScreenShotSaveTask = NULL;
	m_dwScreenShotStartTime = 0;
	m_dwScreenShotMinGap = 5000;
	m_bReservedScreenShot  = false;

	m_dwGrapplingCheckDelay = FLOAT100;

	m_bMouseControlModeCancel = false;
	m_all_udp_recv_time = 0;

	m_bWebLogOutForLuncher = false;

	m_bProcessCandidate = false;

	m_dwShowWindowCursorStyle = 0;
	m_bP2PDataDBLog = false;
	m_bControlSyncExpand = false;
	m_bResourceLoadFailed = false;

	m_dwGamePlayAlarmTime = 0;
	m_iGamePlayAlarmHour  = 0;

	m_dwProcessTimeCheck    = 0;
	m_dwProcessTimeLagCount = 0;
	m_dwProcessTimeLagTime  = 0;

	// Test Check
	m_dwTestReciveStartTime = 0;
	m_iTestReciveCnt = 0;
	m_dwTestReciveTime = 0;
	m_dwTestReciveSize = 0;

	m_dwTestSendStartTime = 0;
	m_iTestSendCnt = 0;
	m_dwTestSendSize = 0;

	m_dwTestFrameStartTime = 0;
	m_iTestFrameCnt = 0;
	
	m_dwTestValue = 0;
	m_bTestChangeStage = false;

	m_CurStage = ioGameStage::GS_NONE;

	g_POIThreadId = GetCurrentThreadId();

	m_bIsMeDeveloperFirst			= true;
	m_bIsMeDeveloperCache			= false;
	m_eLoadingType					= LOADING_NONE;
	m_eConnectedMoveMode			= CMM_LOBBY;
	m_iConnectTryCount				= 0;

	//ping check
	m_dwUDPDataCheckTime = 0;
	m_vecUDPData.reserve( 500 );
	//m_vecUDPDataErr.reserve( 100 );
	m_vecUDPInfo.reserve( 600 );

#ifdef STEAM_ATTACH
	m_bSteamEnable = false;
#endif

#ifdef ANTIHACK
	m_bRUDPType = true;
	m_dwUserSeed = m_dwNPCSeed = 0;
	InitRUDPList();

	for( DWORD i = 0; i < dwRUDPMaximum/2; ++i )
	{
		RUDPInfo* pRUDPInfo = new RUDPInfo;
		pRUDPInfo->pPacket = new SP2Packet;
		m_deqRUDP.push_back( pRUDPInfo );
	}

	for( DWORD i = 0; i < dwRUDPMaximum; ++i )
	{
		RUDPInfo* pRUDPInfo = new RUDPInfo;
		pRUDPInfo->pPacket = new SP2Packet;
		m_deqRUDPNPC.push_back( pRUDPInfo );
	}

#endif

#if defined( USE_GA )
	m_bGAStart			= false;
	m_bGAUser			= false;
	m_dwGAAliveTime		= 0;
	m_bGAFirstCash		= true;
	m_bFirstUnMouseBusy = true;
#endif
	m_iLoginResult		= NULL;
	m_dwLastBulletUpdateTime = 0;

#ifdef _DEBUG
	m_dwCurPacketPrintCount = 0;
	m_dwMaxPacketPrintCount = 100;
#endif
}

ioApplication::~ioApplication()
{
	SAFEDELETE(m_pLogin);
}

void ioApplication::ReleaseAll()
{
	g_bExit = true;
	LOG.PrintTimeAndLog( 0, "Release - Start" );

	SetUnRegHotKey( m_hWnd );

	if( g_MyInfo.GetChannelingType() == CNT_MGAME )
	{
		char szPath[MAX_PATH*2]="";
		::GetCurrentDirectory(MAX_PATH*2,szPath);
		StringCbCat( szPath, sizeof( szPath ), "\\info\\cd" );
		DeleteFile( szPath );
		StringCbCat( szPath, sizeof( szPath ), "l" ); // info/cdl
		DeleteFile( szPath );
		
		int iLength = strlen( szPath );
		if( COMPARE( iLength-1, 0, MAX_PATH*2 ) )
		{
			szPath[iLength-1] = 's'; // info/cds
			DeleteFile( szPath );
		}
	}

	g_MyInfo.ClearBingoIcon();

	if( !ioLocalManagerParent::IsIME() )
	{
		if( Setting::FullScreen() )
			ChangeDisplaySettings(NULL, 0); 
	}

	if( m_hWindowTitleLosa )
	{
		DeleteObject(m_hWindowTitleLosa);
		m_hWindowTitleLosa = NULL;
	}

	if( m_hWindowOutLine )
	{
		DeleteObject(m_hWindowOutLine);
		m_hWindowOutLine = NULL;
	}

#if defined( USE_GA )
	SAFEDELETE( m_pHttpMng );
#endif

#ifdef STEAM_ATTACH
	g_ioSteamClient.End();
#endif


	SAFEDELETE( m_pShopMgr );
	SAFEDELETE( m_pSpecialShopMgr );
	SAFEDELETE( m_pShopDataCache );
	SAFEDELETE( m_pPowerUpMgr );
	SAFEDELETE( m_pModeResourceMgr );
	SAFEDELETE( m_pScreenShotSaveTask );
	ioThreadTaskManager::ReleaseInstance();

	SAFEDELETE( m_pMovieMaker );
	SAFEDELETE( m_pGameAnnMgr );
	SAFEDELETE( m_pLogin );
	SAFEDELETE( m_pLoadingRender );

	m_Mouse.ReleaseResource();

	SAFEDELETE(m_pGameStage);

	SAFEDELETE(m_pCamera);
	SAFEDELETE(m_pSceneMgr);

	SAFEDELETE(m_pCollisionGrpMgr);

	SAFEDELETE(m_pWeaponMgr);
	SAFEDELETE(m_pItemMaker);
	SAFEDELETE(m_pSkillMaker);
	SAFEDELETE(m_pBuffMaker);

	SAFEDELETE(m_pChatMacro);
	SAFEDELETE(m_pChatMacroMgr);
	SAFEDELETE(m_pStatAttribute);
	SAFEDELETE(m_pSetItemInfoMgr);
	SAFEDELETE(m_pEtcItemMgr);
	SAFEDELETE( m_pBattleRoomMgr );
	SAFEDELETE( m_pLadderTeamMgr );
	SAFEDELETE( m_pShuffleRoomMgr );
	SAFEDELETE( m_pModeSelectInfo );
	SAFEDELETE(m_pSlotMgr);
	SAFEDELETE( m_pMyLevelMgr );
	

	SAFEDELETE(m_pTutorialMgr);
	SAFEDELETE(m_pMemoMgr);
	SAFEDELETE(m_pAwardMgr);
	SAFEDELETE(m_pModeUIController);
	SAFEDELETE(m_pVictoriesMgr);
	SAFEDELETE(m_pPlayBonusMgr);
	SAFEDELETE(m_pTreasureCardMgr);
	SAFEDELETE( m_pGuildMarkMgr );
	SAFEDELETE( m_pGuildChatMgr );
	SAFEDELETE(m_pUserInfoMgr);
	SAFEDELETE(m_pGrowthInfoMgr);
	SAFEDELETE(m_pFishingMgr);
	SAFEDELETE(m_pChannelingMgr);
	SAFEDELETE( m_pGlobalSoldierInfo );
	SAFEDELETE(m_pFriendMgr);

	if(m_pBlackListMgr)
		m_pBlackListMgr->SaveBlackList();
	SAFEDELETE(m_pBlackListMgr);

	if(m_pVoiceDisableListMgr)
		m_pVoiceDisableListMgr->SaveList();
	SAFEDELETE(m_pVoiceDisableListMgr);

	SAFEDELETE(m_pChannelMgr);
	SAFEDELETE(m_pModeHelpMgr);
	SAFEDELETE(m_pAlarmMgr);
	SAFEDELETE(m_pTotalManualMgr);
	SAFEDELETE(m_pSoldierManualMgr);
	SAFEDELETE(m_pAnnouncementManualMgr);
	SAFEDELETE(m_pItemManualMgr);
	SAFEDELETE(m_pSkillManualMgr);
	SAFEDELETE(m_pDecoManualMgr);
	SAFEDELETE(m_pEtcManualMgr);
	SAFEDELETE(m_pEventGoodsManualMgr);
	SAFEDELETE(m_pEtcInvenManualMgr);
	SAFEDELETE(m_pEtcInvenSubManualMgr);
	SAFEDELETE(m_pExtraInvenManualMgr);
	SAFEDELETE(m_pMedalInvenManualMgr);
	SAFEDELETE(m_pPresentInvenManualMgr);

	SAFEDELETE(m_pDecoInvenManualMgr);
	SAFEDELETE(m_pItemInvenManualMgr);
	SAFEDELETE(m_pAlchemicFuncManualMgr);
	SAFEDELETE(m_pPetInvenManualMgr);
	SAFEDELETE(m_pCostumeInvenManualMgr);
	SAFEDELETE(m_pCostumeShopManualMgr);
	SAFEDELETE(m_pPopupStoreManualMgr);	
	SAFEDELETE( m_pAccessoryInvenManualMgr );

	SAFEDELETE(m_pKeyMgr);
	SAFEDELETE(m_pVoiceOffListMgr);
	SAFEDELETE(m_pEventMgr);
	SAFEDELETE(m_pPresentMgr);
	SAFEDELETE(m_pSubscriptionMgr);
	SAFEDELETE(m_pExtraItemInfoMgr);
	SAFEDELETE(m_pShopItemExtraInfoManager);
	SAFEDELETE(m_pItemCompoundMgr);
	SAFEDELETE(m_pItemCompoundMaterialMgr);
	SAFEDELETE(m_pRoomOptionMgr);
	SAFEDELETE(m_pSaleMgr);
	SAFEDELETE( m_pFileWriteClient );
	SAFEDELETE(m_pExcavationMgr);
	SAFEDELETE(m_pMedalItemInfoMgr);
	SAFEDELETE(m_pExpandMedalSlotManager);
	SAFEDELETE(m_pTradeInfoMgr);
	SAFEDELETE(m_pAlchemicMgr);
	SAFEDELETE(m_pReservedTask);
	SAFEDELETE(m_pRouletteMgr);
	SAFEDELETE(m_pItemRechargeMgr);
	SAFEDELETE(m_pModeItemMgr);
	SAFEDELETE(m_pPetInfoMgr);
	SAFEDELETE(m_pPetEatInfoMgr);
	SAFEDELETE(m_pRequestCharMgr);
	SAFEDELETE(m_pRequestLagMgr);
	SAFEDELETE(m_pCostumeInfoMgr);
	SAFEDELETE(m_pCostumeShopInfoMgr);
	SAFEDELETE( m_pUIImageRenderManager );
	SAFEDELETE(m_pNamedTitleInfoManager);

	SAFEDELETE(m_pSlangUtil);
	if(m_pBlockKeyBoard)
	{
#ifdef SHIPPING
		m_pBlockKeyBoard->EnableSysKey();
#endif
		SAFEDELETE(m_pBlockKeyBoard);
	}
	SAFEDELETE(m_pCustomSoundMgr);
	SAFEDELETE(m_pMannerTrialChatMgr);

	SAFEDELETE( m_pTournamentManager );
	SAFEDELETE( m_pTournamentRewardManager );
	SAFEDELETE(m_pInputBox);
	SAFEDELETE(m_pChatMgr);
	SAFEDELETE( m_pQuestManager );
	SAFEDELETE( m_pQuestGuide );
	SAFEDELETE( m_pFieldRewardItemMgr );
	SAFEDELETE( m_pBackground );
	SAFEDELETE(m_pGUIMgr);            // GUI Manager Class���� ��� �̹��� �޸𸮸� �ʱ�ȭ�ϹǷ� Image �޸𸮸� �����ϴ� Ŭ������ �׻� ������ �Ҹ��ڰ� �����Ǿ����.
	SAFEDELETE(m_pFieldItemMgr);

	if( m_pMyInfo )
	{
		MannerTrialInfo *pMannerTrialInfo = m_pMyInfo->GetMannerTrialInfo();
		if( pMannerTrialInfo )
			pMannerTrialInfo->Save();
	}
	SAFEDELETE( m_pMyInfo );
	SAFEDELETE( m_pP2PChecker );
	SAFEDELETE( m_pTCPChecker );
	SAFEDELETE( m_pClassPrice );
	SAFEDELETE( m_pEventGoodsMgr );
	SAFEDELETE( m_pDecorationPrice );
	SAFEDELETE( m_pAbuseMgr );
	SAFEDELETE( m_pGuildInfo );
	SAFEDELETE( m_pNetworkTable );
	SAFEDELETE( m_pCampManager );
	SAFEDELETE( m_pNpcMgr );
	SAFEDELETE( m_pSymbolTable );
	SAFEDELETE( m_pAIDB );
	SAFEDELETE( m_pMapEventSceneMgr );
	SAFEDELETE( m_pTournamentTeamManager );
	SAFEDELETE( m_pSuperGashponMgr );
	SAFEDELETE( m_pGashponMgr );
	SAFEDELETE( m_pSoldierAwakeMgr );
	SAFEDELETE( m_pBasicTooltipManager );	
	SAFEDELETE( m_pMissionManager );
	SAFEDELETE( m_pRenderTargetMgr );
	SAFEDELETE( m_pRSoldierInfoManager );
	SAFEDELETE( m_pGuildBlockStorage );
	SAFEDELETE( m_pHomeBlockStorage );
	SAFEDELETE( m_pBonusCashMgr );
	SAFEDELETE( m_pAccessoryInfoMgr );
	
	SAFEDELETE( m_pTCPSocket );

	if( m_pUDPSocket )
	{
		m_pUDPSocket->CloseUDP();
		SAFEDELETE(m_pUDPSocket);
	}

	ioPortalHelp::CloseWinSock();
	CVoiceChat::ReleaseInstance();
	UserInfoToolTip::Clear();

#ifdef XIGNCODE
	g_ioXignCode.OnDisconnect();
#endif 

	SAFEDELETE(m_pEffectFac);
	SAFEDELETE(m_pTrailDataMgr);

	if( m_pAnimateFXMgr )
		m_pAnimateFXMgr->DestroyAll();
	SAFEDELETE(m_pAnimateFXMgr);

	if( m_pFontMgr )
		m_pFontMgr->DestroyAll();
	SAFEDELETE(m_pFontMgr);

	SAFEDELETE(m_pUIRenderer);

	if( m_pMeshMgr )
		m_pMeshMgr->DestroyAll();
	SAFEDELETE(m_pMeshMgr);

	SAFEDELETE(m_pMaterialMgr);

	if( m_pAnimationMgr )
		m_pAnimationMgr->DestroyAll();

	SAFEDELETE(m_pAnimationMgr);

	if( m_pAnimationEventManager )
		m_pAnimationEventManager->DestroyAll();

	SAFEDELETE(m_pAnimationEventManager);

	if( m_pSkeletonMgr )
		m_pSkeletonMgr->DestroyAll();
	SAFEDELETE(m_pSkeletonMgr);

	if( m_pTextureMgr )
		m_pTextureMgr->DestroyAll();
	SAFEDELETE(m_pTextureMgr);

	SAFEDELETE(m_pTexEffMgr);

	if( m_pShaderMgr )
		m_pShaderMgr->DestroyAll();
	SAFEDELETE(m_pShaderMgr);

	SAFEDELETE(m_pExSoundMgr);
	SAFEDELETE(m_pSoundMgr);
	SAFEDELETE(m_pDxInput);
	SAFEDELETE(m_pVtxBufMgr);
	SAFEDELETE(m_pIdxBufMgr);
	SAFEDELETE(m_pMathManager);
	SAFEDELETE(m_pDropManager);
	SAFEDELETE( m_pCustomTextureMgr );
	SAFEDELETE( m_pFlashDownLoadMgr );

	SAFEDELETE(m_pBrowserMgr);

	DestroyOpcodeManager();
	m_pOpcodeMgr = NULL;

	ReleaseRenderSystem();

	SAFEDELETE(m_pTickTimer);
	SAFEDELETE(m_pGlobalTimer);
	SAFEDELETE(m_pFrameTimer);

#ifdef STEAM_ATTACH
	ioSteamClient::ReleaseInstance();
#endif

	LogP2PPacket();

	m_vNotMakeIDVector.clear();
	LOG.PrintTimeAndLog( 0, "Release - Done" );
}

//UJ 130828, prevent Bandicam but keep our call (which has 1 as argument) from LostSaga.exe --start
BOOL InterceptAPI(DWORD dwAddressToIntercept, DWORD dwReplaced, char *OldCode)
{ 
	if (dwAddressToIntercept == 0) return FALSE;
	DWORD dwOldProtect;
	BYTE *pbTargetCode = (BYTE *) dwAddressToIntercept;
	BYTE *pbReplaced = (BYTE *) dwReplaced;

	if( VirtualProtect((void *) dwAddressToIntercept, 5, PAGE_EXECUTE_READWRITE, &dwOldProtect) )
		return FALSE;

	if (OldCode != NULL) memcpy(OldCode, pbTargetCode, 5);
	*pbTargetCode++ = 0xE9; // jump rel32
	*((unsigned int *)(pbTargetCode)) = pbReplaced - (pbTargetCode +4);
	HANDLE hHandle = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE , FALSE, ::GetCurrentProcessId());
	
	if( VirtualProtectEx(hHandle, (void *) dwAddressToIntercept, 5, PAGE_EXECUTE_READWRITE, &dwOldProtect) )
		return FALSE;

	FlushInstructionCache(GetCurrentProcess(), NULL, NULL);
	return TRUE;
}
void WINAPI	ioSleep(DWORD dwMilliseconds)
{
	//todo: need to check thread is exactly Bandicam's
	if( g_POIThreadId == GetCurrentThreadId() && (dwMilliseconds==5 || (dwMilliseconds>20 && dwMilliseconds<=1000)) ) return;
	::SleepEx(dwMilliseconds, FALSE);
}
DWORD WINAPI ioSignalObjectAndWait(HANDLE hObjectToSignal, HANDLE hObjectToWaitOn, DWORD dwMilliseconds, BOOL bAlertable)
{
	//fraps' arguments are x,y,10000,FALSE
	bool bFraps = ( dwMilliseconds == 10000 && bAlertable == FALSE );
	if (bFraps) //return 0;
		dwMilliseconds = 33 + (timeGetTime() % 2); //limit to 30FPS
	//todo: need to prove the following alternative is the same as the original API 
	::SetEvent(hObjectToSignal);
	DWORD dwRet = ::WaitForSingleObjectEx(hObjectToWaitOn, dwMilliseconds, bAlertable);
	if (bFraps) return WAIT_OBJECT_0;
	return dwRet;
}
//UJ --end

bool ioApplication::InitWindow( HINSTANCE hInstance, const ioHashString &szKeyOnCmd )
{
	if( ioLocalManagerParent::IsIME() )
		m_pInputBox = new ioInputBox( NULL, NULL ); // ime�� editbox�� ���� ������ �ٸ�

 	m_pFrameTimer = new ioFrameTimer;
	m_pFrameTimer->Start( 30.0f );	

#ifndef SHIPPING
	ioINILoader_e kLoader( "config/sp2.ini" );
	m_pFrameTimer->SetTimeRate( kLoader.LoadFloat_e( "time", "timer_rate", FLOAT1 ) );	
#endif

	WNDCLASS wc;
    wc.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wc.lpfnWndProc		= WndProc;
    wc.cbClsExtra		= 0;
    wc.cbWndExtra		= 0;
    wc.hInstance		= hInstance;
    wc.hIcon			= LoadIcon( hInstance, MAKEINTRESOURCE(IDR_MAINFRAME) );
    wc.hCursor			= LoadCursor( NULL, IDC_ARROW );
    wc.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName		= NULL;
    wc.lpszClassName	= APPLICATION_NAME;

    RegisterClass( &wc );
#ifdef XTRAP
	g_ioXtrap.KeepAlive();
#endif 
	
	HWND hWnd;
	hWnd = CreateWindowEx( 0,
						   APPLICATION_NAME,
						   APPLICATION_NAME,
						   FULLSCREEN_STYLE,
						   0, 0, 0, 0,
						   NULL,
						   NULL,
						   hInstance,
						   NULL );

	if( FAILED( hWnd ) )
		return false;

	m_hWnd = hWnd;
	m_hInstance = hInstance;

    if( !ioLocalManagerParent::IsIME() )
		m_pInputBox = new ioInputBox( hWnd, hInstance ); // ime�� editbox�� ���� ������ �ٸ�


	m_iWidth  = 0;
	m_iHeight = 0;
	m_iDeskTopWidth  = GetSystemMetrics(SM_CXSCREEN);
	m_iDeskTopHeight = GetSystemMetrics(SM_CYSCREEN);

	m_pBrowserMgr = new ioBrowserManager;
	if( m_pBrowserMgr )
		m_pBrowserMgr->Initialize( hInstance, m_hWnd, szKeyOnCmd );

	DWORD dwIDTitleBmp = IDB_WINDOW_TITLE_LOSA;
	ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
	if( pLocal )
	{
		dwIDTitleBmp = pLocal->GetResourceIDTitleBmp();
	}

	m_hWindowTitleLosa = LoadBitmap( hInstance, MAKEINTRESOURCE(dwIDTitleBmp) );
	m_hWindowOutLine   = LoadBitmap( hInstance, MAKEINTRESOURCE(IDB_WEB_OUTLINE) );

#ifndef XTRAP
	////UJ 130828, prevent low fps limit by Bandicam or some other potential injection tools
 	DWORD dwSleepAddress = (DWORD)GetProcAddress( GetModuleHandle("Kernel32.dll"), "Sleep" );
 	DWORD dwSleepBaseAddress = (DWORD)GetProcAddress( GetModuleHandle("KernelBase.dll"), "Sleep" );
 	InterceptAPI( dwSleepBaseAddress, (DWORD)&ioSleep, NULL );
 	InterceptAPI( dwSleepAddress, (DWORD)&ioSleep, NULL );
 	//UJ 130829, Fraps uses signal and wait API
 	DWORD dwSignalObjectAndWait = (DWORD)GetProcAddress( GetModuleHandle("Kernel32.dll"), "SignalObjectAndWait" );
 	InterceptAPI( dwSignalObjectAndWait, (DWORD)&ioSignalObjectAndWait, NULL );
 	LOG.PrintTimeAndLog( 0, "Address=%d, BaseAddress=%d, SignalAddress=%d", dwSleepAddress, dwSleepBaseAddress, dwSignalObjectAndWait );
#endif
	SetRegHotKey( m_hWnd );

	return true;
}

bool ioApplication::SetupBeforeLogin( const ioHashString &szGameServerIP, int iGameServerPort )
{
	if( !ioPortalHelp::InitWinSock() )
	{
		LOG.PrintTimeAndLog( 0, "ioApplication::SetupBeforeLogin - InitSocket Failed" );
		return false;
	}

	m_pTCPChecker = new ioTCPChecker;
	m_pP2PChecker = new ioP2PChecker;
	
	m_pTCPSocket = new ioTCPSocketManager;

//#ifdef SHIPPING

#ifdef _ITEST
	LOG.PrintTimeAndLog( 0, "ioApplication::SetupBeforeLogin - szGameServerIP(%s), port(%d)", szGameServerIP.c_str(), iGameServerPort );
#endif

#if 1
	if( !m_pTCPSocket->Connect( m_hWnd, szGameServerIP.c_str(), iGameServerPort ) )
	{
		LOG.PrintTimeAndLog( 0, "ioApplication::SetupBeforeLogin - TCP Socket Failed(%d)",
			WSAGetLastError() );
		return false;
	}
#else
	if(!m_pLogin->IsSingleMode())
	{
		if( !m_pTCPSocket->Connect( m_hWnd, szGameServerIP.c_str(), iGameServerPort ) )
		{
			LOG.PrintTimeAndLog( 0, "ioApplication::SetupBeforeLogin - TCP Socket Failed(%d)",
				WSAGetLastError() );
			return false;
		}
	}
#endif

	if( szGameServerIP == "192.168.1.133" )
	{
		m_pP2PChecker->SwitchRecvLog();
	}

	ioINILoader_e kLoader( "config/sp2.ini" );
	kLoader.LoadString_e( "resource", "path", "", m_szResourcePath, MAX_PATH );

	char szPath[MAX_PATH];
	ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
	wsprintf( szPath, "%s/text", m_szResourcePath );
	if( pLocal )
	{
		g_StringMgr.LoadData( szPath, pLocal->GetTextListFileName(), "" );
		pLocal->LoadINI();
	}
	m_pNetworkTable = new ioUserNetworkTable;
	m_bNetworkPlay = true;

	m_pMyInfo = new ioMyInfo;
	if( m_pMyInfo )
		m_pMyInfo->LoadCharSlotInfo();

	// ioSetItemInfoManager, ioDecorationPrice, ioEtcItemManager ���� ������ �Ҵ�
	m_pSaleMgr = new ioSaleManager;

	m_pClassPrice = new ioClassPrice;
	m_pClassPrice->LoadClassCommon();

	m_pEventGoodsMgr = new ioEventGoodsMgr;
	m_pEventGoodsMgr->LoadEventGoodsInfo();
	
	m_pDecorationPrice = new ioDecorationPrice;
	m_pDecorationPrice->LoadPriceInfo();

	m_pAbuseMgr = new ioAbuseManager;
	m_pAbuseMgr->LoadINIInfo( kLoader );

	m_pMemoMgr = new ioMemoManager;
	m_pFriendMgr= new ioFriendManager;
	m_pFriendMgr->LoadINIInfo( kLoader );

	m_pBlackListMgr = new ioBlackListManager;
	m_pChannelMgr= new ioChannelManager;
	m_pKeyMgr = new ioKeyManager;
	m_pUserInfoMgr = new ioUserInfoMgr;
	m_pGlobalSoldierInfo = new ioGlobalSoldierInfo;

	m_pVoiceDisableListMgr = new ioVoiceDisableListManager;
	m_pVoiceOffListMgr     = new ioVoiceOffListManager;
	m_pEventMgr            = new ioEventManager;
	m_pPresentMgr          = new ioPresentMgr;
	m_pPresentMgr->LoadINIInfo();
	m_pSubscriptionMgr = new ioSubscriptionMgr;
	m_pSubscriptionMgr->LoadINIInfo();
	m_pMannerTrialChatMgr  = new ioMannerTrialChatManager;

	m_pGuildInfo = new ioGuildInfo;
	m_pGuildInfo->Initialize();

	m_pCampManager = new ioCampManager;
	m_pCampManager->LoadINIData();

	m_pTournamentManager = new ioTournamentManager;
	m_pTournamentManager->Initialize();

	m_pTournamentTeamManager = new ioTournamentTeamManager;

	m_pSymbolTable = new ioAISymbolTable;
	m_pSymbolTable->Init();
		
	m_pAIDB = new ioAIDatabase;

	m_pNpcMgr = new ioNpcMgr;
	m_pNpcMgr->LoadFieldNPC();

	m_pMapEventSceneMgr = new ioMapEventSceneMgr;
	m_pMapEventSceneMgr->LoadMapEventScene();

	m_pFileWriteClient = new ioFileWriteClient;
	if( m_pFileWriteClient )
		m_pFileWriteClient->LoadINI();

	m_pExcavationMgr   = new ioExcavationManager;
	if( m_pExcavationMgr )
		m_pExcavationMgr->LoadExcavation();

	m_pMissionManager = new ioMissionManager;
	if ( m_pMissionManager)
		m_pMissionManager->Initialize();

	kLoader.LoadString_e( "resource", "path", "", m_szResourcePath, MAX_PATH );
	m_iTextureGrayRate = kLoader.LoadInt_e( "resource", "gray_texture_rate", 120 );
	m_iTextureGrayLightRate = kLoader.LoadInt_e( "resource", "gray_light_texture_rate", 90 );
	m_dwMouseUnvisiblTime = kLoader.LoadInt_e( "mouse", "mouse_unvisible_time", 1000 );
	m_dwMouseUnvisiblLobbyTime = kLoader.LoadInt_e( "mouse", "mouse_unvisible_lobby_time", 1000 );

	kLoader.SetTitle_e( "screenshot" );
	m_dwScreenShotMinGap = kLoader.LoadInt_e( "min_gap_time", 5000 );

	char szBuf[MAX_PATH];
	kLoader.LoadString_e( "start_sound", "", szBuf, MAX_PATH );
	m_ScreenShotStartSound = szBuf;

	// grappling check delay
	m_dwGrapplingCheckDelay = kLoader.LoadInt_e( "grappling_check", "grappling_check_delay", FLOAT100 );

	char szIP[MAX_PATH];
	memset( szIP, 0, MAX_PATH );
	kLoader.LoadString_e("connect", "log_server_ip", "", szIP, MAX_PATH);
	m_LogServerIP			= szIP;

	m_iLogPort						= kLoader.LoadInt_e( "connect", "log_server_port", 0 );
	m_bP2PDataDBLog					= kLoader.LoadBool_e( "connect", "p2p_data_log", false );
	m_bControlSyncExpand			= kLoader.LoadBool_e( "connect", "control_sync_expand", false );
	m_iConnectTryMax				= kLoader.LoadInt_e( "connect", "connected_move_try", 1 );
	m_bEnableConnectedMoveMode		= kLoader.LoadBool_e( "connect", "enable_connected_move_mode", false );
	m_eConnectedMoveMode			= (ConnectedMoveMode)kLoader.LoadInt_e( "connect", "connected_move_mode", 0 );

	if( m_pLogin && m_pLogin->IsSingleMode() )
	{
		m_NextStage  = ioGameStage::GS_PLAY;
		EnableNetworkPlay( false );
	}
	
	m_pRouletteMgr = new ioRouletteManager;
	if( m_pRouletteMgr )
	{
		m_pRouletteMgr->LoadRoulette();
	}

	m_pPowerUpMgr = new ioPowerUpManager;
	m_pPowerUpMgr->Initialize();

	m_pRSoldierInfoManager = new ioRSoldierInfoManager;
	m_pRSoldierInfoManager->Initialize();

#ifndef _DEBUG
	// minidump
	if( Help::CheckEnableTimeGapUser(1) )
		MiniDump::Begin( "LostSagaT" );

	if( !SetCrashHandlerFilter( ExceptCallBack ) )
	{
		LOG.PrintTimeAndLog( 0, "ioApplication::SetupBeforeLogin - SetCrashHandlerFilter Failed" );
		return false;
	}
#endif

#ifdef HACKSHIELD
#ifdef SRC_ID
	g_ioHackShield.AuthenticateLocalPC();
#endif
#endif

	return true;
}

bool ioApplication::InitUDPSocket() //kyg ���⼭ �̴ϼȶ���¡ udp �� 
{
	if( m_pUDPSocket )     //�̹� �ʱ�ȭ �Ǿ���.
		return true;

	ioINILoader_e kLoader( "config/sp2.ini" );

	char szLocalIP[LENGTH_IP] = "";
	GetMyLocalIP( szLocalIP );
	int iP2PPort  = CreateP2PPort( szLocalIP );
	int iSecurity = kLoader.LoadInt_e( "security", "ON", 1 );
	m_pUDPSocket = new ioClientUDPSocket( iSecurity );
	if( !m_pUDPSocket->InitUDP( m_hWnd, WM_P2PSOCKET, iP2PPort ) )
	{
		LOG.PrintTimeAndLog( 0, "ioApplication::InitUDPSocket - UDP Socket Failed(%d)", WSAGetLastError() );
		return false;
	}
	LOG.PrintTimeAndLog( 0, "Init UDP Info : %s - %d", szLocalIP, iP2PPort );
	if( m_pLogin && m_pUDPSocket )
		m_pLogin->SetMyIP( m_pUDPSocket->GetMyIP() );
	return true;
}

bool ioApplication::ResetInitUDPSocket()
{
	ioClientUDPSocket *pPrevUDPSocket = m_pUDPSocket;
	m_pUDPSocket = NULL;
	if( InitUDPSocket() )
	{
		if( pPrevUDPSocket )
		{
			pPrevUDPSocket->CloseUDP();
			SAFEDELETE( pPrevUDPSocket );
		}
		return true;
	}
	else
	{
		ioClientUDPSocket *pNextUDPSocket = m_pUDPSocket;
		m_pUDPSocket = pPrevUDPSocket;
		if( pNextUDPSocket )
			SAFEDELETE( pNextUDPSocket );
	}
	return false;
}

int ioApplication::CreateP2PPort( char szLoaclIP[LENGTH_IP] )
{
	int iP2PPort = P2PSOCKET_PORT;
	/* ������ �����⳻���� ��Ʈ�� ���� �ٸ��� ����� ���� �������� ������ ������ ���Ѵ�. */	
	int iLen = strlen( szLoaclIP );
	int iLastByte = 0;
	for(int i = 0;i < iLen;i++)
	{
		if( szLoaclIP[i] == '.' )
			iLastByte++;
		
		if( iLastByte >= 3 ) 
		{
			iLastByte = atoi( &szLoaclIP[i+1] );
			break;
		}			
	}
	iP2PPort += iLastByte;
#ifndef SHIPPING
	while( Help::CheckSocketUDPPortUsed( iP2PPort ) )
	{
		iP2PPort++;
	}
	LOG.PrintTimeAndLog( 0, "ioApplication::CreateP2PPort - UDP Port : %d", iP2PPort );
#endif
	return iP2PPort;
}

void ioApplication::ModifyMainWindow()
{

	m_iWidth  = max( Setting::Width(), 800 );
	m_iHeight = max( Setting::Height(), 600 );
	
	Setting::SetCurResolution( m_iWidth, m_iHeight );
	m_Mouse.SetCurrScreenSize( m_iWidth, m_iHeight );

	if( !Setting::FullScreen() )
	{
		SetWindowLong( m_hWnd, GWL_STYLE, WINDOW_STYLE );
		SetWindowPos( m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOREDRAW|SWP_NOSIZE );

		RECT rcRect;
		SetRect( &rcRect, 0, 0, m_iWidth, m_iHeight );
		AdjustWindowRectEx( &rcRect, WINDOW_STYLE, FALSE, 0 );

		MoveWindow( m_hWnd,
					(m_iDeskTopWidth - m_iWidth)/2 - 3,
					(m_iDeskTopHeight - m_iHeight)/2 - 22,
					rcRect.right - rcRect.left, 
					rcRect.bottom - rcRect.top, 
					FALSE );		
	}
	else
	{
		// ���ʿ� FULLSCREEN_STYLE�� ��������Ƿ� SetWindowLong ����
		MoveWindow( m_hWnd, 0, 0, m_iWidth, m_iHeight, FALSE );
	}

	ShowWindow( m_hWnd, SW_SHOW );
	UpdateWindow( m_hWnd );
}

void CALLBACK ioApplication::OnIMETimer(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	ioApplication* app = (ioApplication*)dwUser;
	if (app != NULL)
	{
		//UJ 130718, Ÿ�̸Ӵ� �ٸ� �����忡�� �����ϹǷ� ����� ����ȣ������ �ʰ� �޼����� ó��
		if( !app->m_bProgramExit && app->m_pTCPSocket )
		{
			if( !app->ChangeServer() ) 
				PostMessage(app->GetHWnd(), WM_TIMER, WM_IO_IMETIMER, 0);
		}
	}
}

int ioApplication::Run()
{
#ifndef _DEBUG
	__try
#endif
	{
		ModifyMainWindow();

		if( !Setup() )
			return -1;

		ChangeSwitch();
		ChangeStage();
		ProcessBeforeSetUpPacket();
		ProcessBeforeCharLoading();
		CheckPacExceptionLog();
		RenderFirstLoading();

		if ( IsMeDeveloper() )
			SetUnRegHotKey( m_hWnd );

		// ������ ���̵𿡸� 30�� ������ ���̼����� ���� ������ �˾�â���� ���		JCLEE 140718
#if defined( SRC_OVERSEAS )
		CheckLicenseForDev();
#endif

	}
#ifndef _DEBUG
	__except( ExceptCallBack( GetExceptionInformation() ) )
	{
		m_bProgramExit = true;
		return -1;
	}
#endif
	
	MSG msg;

	bool bProcessKeyUp = true;

	while( TRUE )
	{
		ErrorReport::SetPosition( 1000, 400 );

		if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
		{
			m_bTestChangeStage = false;

			ErrorReport::SetPosition( 1000, 401 );
			if( !GetMessage( &msg, NULL, 0, 0 ) )
			{
				break;
			}

			if( m_pBrowserMgr && m_pBrowserMgr->OnPreMessage( msg ) )
			{
				continue;
			}



			ErrorReport::SetPosition( 1000, 402 );
			TranslateMessage( &msg );
			ErrorReport::SetPosition( 1000, 403 );

			DispatchMessage( &msg );
				
			ErrorReport::SetPosition( 1000, 404 );



 			RenderFirstLoading();
 			continue;
		}
		else
		{
			//UJ 130916, ���ǹ���: timeGetTime()�� �����ֱ淡 REALGETTIME()���� ����
			DWORD dwCurTime = REALGETTIME();

			//UJ 131022, ���ǹ���: ��ü �������� �ִ� 125FPS�� ������(packet spam ����)
			if( m_dwTestValue == 0 || (dwCurTime-m_dwTestValue) >= 8 )
			{
				m_dwTestValue = dwCurTime;

				ErrorReport::SetPosition( 1000, 405 );

				WindowMouseCursorProcess();
				ErrorReport::SetPosition( 1000, 406 );

				if( !m_bProgramExit && m_pTCPSocket )
				{
					ErrorReport::SetPosition( 1000, 407 );
					if( !ChangeServer() ) 
					{
						ErrorReport::SetPosition( 1000, 408 );
						MainLoop();
						ErrorReport::SetPosition( 1000, 409 );
					}

					ErrorReport::SetPosition( 1000, 410 );
					CheckServerToClientPing();
					CheckLanConnected();
					CheckRecvUDPUnviable();
					ErrorReport::SetPosition( 1000, 411 );
					CheckPacExceptionLog();
					ErrorReport::SetPosition( 1000, 412 );
				}
			}
			else
			{
				if( dwCurTime - m_dwTestValue <= 2 )
					::SleepEx( 1, FALSE );
			}
		}

		ErrorReport::SetPosition( 1000, 413 );
		if( m_bProgramExit )
		{
			LogGlobalMemory();
			if( Help::CheckEnableTimeGapUser(1) )
				MiniDump::End();

			ErrorReport::SetPosition( 1000, 415 );
			DestroyWindow( m_hWnd );
		}

		ErrorReport::SetPosition( 1000, 416 );
	}

	return msg.wParam;
}

bool ioApplication::Setup()
{
	m_vPackFileSize.clear();
	bool bCreateDXWindow = !Setting::FullScreen();

	if( !ioLocalManagerParent::IsIME() )
	{
		if( Setting::FullScreen() )
		{
			ChangeDisplaySettingsBySetting();
			bCreateDXWindow = true;
		}
	}

#if defined( USE_GA )
	m_pHttpMng = new ioHttpMng;	
	m_pHttpMng->LoadINI();
	m_pHttpMng->InitGALocal();
#endif

	ErrorReport::SetPosition( 1070, 1 );
	m_pRenderSystem = &RenderSystem();
	m_pRenderSystem->SetMinDisplayMode( 800, 600 );	
	ErrorReport::SetPosition( 1070, 2 );

	bool bDeviceCreateOK = false;
	bDeviceCreateOK = m_pRenderSystem->Create( m_hWnd,
											   m_iWidth,
											   m_iHeight,
											   Setting::SwapCopy(),
											   bCreateDXWindow,
											   Setting::Use16BitFrameBuffer() );	
	// ���̷�Ʈ X 1�� ����
	if( !bDeviceCreateOK )
	{
		if( !Setting::FullScreen() )  // â���� �����ϸ� Ǯ���� �ٽ� ����
		{
			Setting::SetFullScreen( true );
			Setting::SaveSetting( g_MyInfo.GetPublicID() );

			bDeviceCreateOK = m_pRenderSystem->Create( m_hWnd, m_iWidth, m_iHeight,	Setting::SwapCopy(), !Setting::FullScreen(), Setting::Use16BitFrameBuffer() );
		}
	}

	// ���̷�Ʈ X 1�� ����
	if( !bDeviceCreateOK )
	{
		if( ioLocalManagerParent::IsIME() )
		{
			if( !Setting::FullScreen() )  // â���� �����ϸ� Ǯ���� �ٽ� ����
			{
				Setting::SetFullScreen( true );
				Setting::SaveSetting( g_MyInfo.GetPublicID() );

				bDeviceCreateOK = m_pRenderSystem->Create( m_hWnd, m_iWidth, m_iHeight,	Setting::SwapCopy(), !Setting::FullScreen(), Setting::Use16BitFrameBuffer() );
			}
		}
	}

	// �����ϸ� �ػ� ������ �����غ���.
	if( !bDeviceCreateOK )
	{
		// ���� �ػ� default : 1024, 768		JCLEE 140408
		if( ioLocalManager::GetLocalType() == ioLocalManager::LCT_EU ||
			ioLocalManager::GetLocalType() == ioLocalManager::LCT_GERMANY ||
			ioLocalManager::GetLocalType() == ioLocalManager::LCT_FRANCE ||
			ioLocalManager::GetLocalType() == ioLocalManager::LCT_ITALIA ||
			ioLocalManager::GetLocalType() == ioLocalManager::LCT_POLAND ||
			ioLocalManager::GetLocalType() == ioLocalManager::LCT_TURKEY )
		{
			m_iWidth  = 1024;
			m_iHeight = 768;
		}
		else
		{
			m_iWidth  = 800;
			m_iHeight = 600;
		}		

		Setting::SetCurResolution( m_iWidth, m_iHeight );
		Setting::SaveSetting( g_MyInfo.GetPublicID() );

		bDeviceCreateOK = m_pRenderSystem->Create( m_hWnd, m_iWidth, m_iHeight,	Setting::SwapCopy(), !Setting::FullScreen(), Setting::Use16BitFrameBuffer() );
	}

	ErrorReport::SetPosition( 1070, 3 );

	if( !bDeviceCreateOK )
	{
		//SafeSprintf(	g_szErrorPos, 
		//	            sizeof( g_szErrorPos ),
		//				STR(1),
		//				m_pUDPSocket->GetMyIP(),
		//				g_MyInfo.GetPrivateID().c_str(),
		//				g_MyInfo.GetPublicID().c_str(),
		//				ErrorReport::GetMajorPosition(),	
		//				ErrorReport::GetMinorPosition(),
		//				ErrorReport::GetEngineMajorPos(),
		//				ErrorReport::GetEngineMinorPos(),
		//				ErrorReport::GetString() );
		//ErrorReport::SetPosition( 1070, 4 );

		//StringCbPrintf( g_szSendErrorLog, 
		//	            sizeof( g_szSendErrorLog ),
		//				"%s\n%s\n%s\n%s",
		//				g_szErrorPos,
		//				Setting::GetFullSystemInfo(),
		//				Setting::GetSettingString(),
		//				g_szErrorLog );
		//ErrorReport::SetPosition( 1070, 5 );

		//SP2Packet kPacket( LUPK_LOG );
		//kPacket << "CheckError";  // �α� ���� Ÿ��
		//kPacket << g_szSendErrorLog;
		//kPacket << 311;  // ������ȣ
		//kPacket << true; // write db
		//P2PNetwork::SendLog(kPacket);
		ErrorReport::SetPosition( 1070, 6 );

		LOG.PrintTimeAndLog( 0, "ioApplication::Setup - RenderSystem Create Failed" );
		
		g_bHappenError = true;
		g_iErrorNumber = 311;
		SafeSprintf( g_szErrorLog, sizeof(g_szErrorLog), STR(2) );

		Setting::SetSpec( Setting::MINIMUM_SPEC );
		Setting::SetSpecAutoCheck( false );
		Setting::SaveSetting( g_MyInfo.GetPublicID() );
		ErrorReport::SetPosition( 1070, 7 );
		return false;
	}


	ErrorReport::SetPosition( 1070, 8 );
	Setting::SetMultiSampleList( m_pRenderSystem );
	Setting::SetResolutionList( m_pRenderSystem );
	ErrorReport::SetPosition( 1070, 9 );

	D3DMULTISAMPLE_TYPE eMultiSample = Setting::GetMultiSampleType( Setting::GetAntiAliasingQuality() );
	if( eMultiSample != D3DMULTISAMPLE_NONE )
	{
		if( m_pRenderSystem->ChangeMultiSamplingType( eMultiSample ) )
		{
			m_pRenderSystem->ApplyChangedPresentParameters();
			m_pRenderSystem->EnableMultiSampling( true );
		}
	}
	ErrorReport::SetPosition( 1070, 10 );
	
	m_pDxInput = new CDirectInput;
	if( !m_pDxInput->Create( m_hInstance, m_hWnd, FALSE ) )
	{
		LOG.PrintTimeAndLog( 0, "ioApplication::Setup - DirectInput Create Failed" );
		return false;
	}
	m_pDxInput->SetActiveDevices( FALSE, TRUE, TRUE );
	ErrorReport::SetPosition( 1070, 11 );

	for( int i=0; myGlue[i].name; i++ )
	{
		m_lua.AddFunction( myGlue[i].name, myGlue[i].func );
	}
	ErrorReport::SetPosition( 1070, 12 );

	m_pMathManager = new MathManager;
	m_pMathManager->Init( &m_lua );
	ErrorReport::SetPosition( 1070, 13 );

	m_pDropManager = new DropManager;
	m_pDropManager->Init( &m_lua );
	ErrorReport::SetPosition( 1070, 14 );

	char szPath[MAX_PATH];

	IDirect3DDevice9 *pDevice = m_pRenderSystem->GetDevice();
	m_pVtxBufMgr = new ioVertexBufferManager( pDevice );
	m_pIdxBufMgr = new ioIndexBufferManager( pDevice );
	ErrorReport::SetPosition( 1070, 16 );

	m_pTextureMgr = new ioTextureManager( pDevice );
	wsprintf( szPath, "%s/Texture", m_szResourcePath );
	m_pTextureMgr->SetStartDir( szPath );
	m_pTextureMgr->ChangeTextureQuality( Setting::GetTextureQuality() );
	m_pTextureMgr->SetGrayScaleRate( m_iTextureGrayRate );
	m_pTextureMgr->SetGrayLightScaleRate( m_iTextureGrayLightRate );
	ErrorReport::SetPosition( 1070, 17 );

#ifndef SHIPPING
	m_pTextureMgr->SetCheckUpdateTime( true );
#endif

	m_pMeshMgr = new ioMeshManager;
	wsprintf( szPath, "%s/Mesh", m_szResourcePath );
	m_pMeshMgr->SetStartDir( szPath );
	ErrorReport::SetPosition( 1070, 18 );

	m_pCollisionGrpMgr = new ioCollisionBoxGrpManager;
	m_pCollisionGrpMgr->SetStartDir( szPath );
	ErrorReport::SetPosition( 1070, 19 );

	m_pOpcodeMgr = CreateOpcodeManager();
	m_pOpcodeMgr->SetStartDir( szPath );
	ErrorReport::SetPosition( 1070, 20 );
	
	m_pSkeletonMgr	= new ioSkeletonManager;
	wsprintf( szPath, "%s/Skeleton", m_szResourcePath );
	m_pSkeletonMgr->SetStartDir( szPath );
	ErrorReport::SetPosition( 1070, 21 );

	m_pAnimationMgr = new ioAnimationManager;
	wsprintf( szPath, "%s/Animation", m_szResourcePath );
	m_pAnimationMgr->InitMemoryPoolSize( 120, 7200 );
	m_pAnimationMgr->SetStartDir( szPath );
	ErrorReport::SetPosition( 1070, 22 );

	m_pAnimationEventManager = new ioAnimationEventManager;
	wsprintf( szPath, "%s/Animation", m_szResourcePath );
	m_pAnimationEventManager->Initialize();
	m_pAnimationEventManager->SetStartDir( szPath );
		
	ioEntityGroup::GuaranteeAtLeastOneAnimation( true );

	if( Setting::GetLightQuality() == LQ_HIGH )
		ioEffect::EnableEffectLightCreation( true );
	else
		ioEffect::EnableEffectLightCreation( false );
	ErrorReport::SetPosition( 1070, 23 );

	m_pMaterialMgr	= new ioMaterialManager;
	wsprintf( szPath, "%s/Material", m_szResourcePath );
	m_pMaterialMgr->SetStartDir( szPath );
	ErrorReport::SetPosition( 1070, 24 );
	
	LoadingScriptMod *pModifier = new LoadingScriptMod;
	pModifier->SetLightQuality( Setting::GetLightQuality() );
	if( Setting::GetVertexShaderVersion() < 15 && Setting::GetPixelShaderVersion() < 15 )
	{
		pModifier->SetUseFixedMode( true );
	}
	m_pMaterialMgr->SetLoadScriptModifier( pModifier );
	ErrorReport::SetPosition( 1070, 25 );

	CloneScriptMod *pLoadedModifier = new CloneScriptMod;
	pLoadedModifier->EnableOutLine( Setting::UseObjectEdge() );
	m_pMaterialMgr->SetCloneScriptModifier( pLoadedModifier );
	ErrorReport::SetPosition( 1070, 26 );
	
	m_pShaderMgr = new ioShaderManager( pDevice );	
	m_pShaderMgr->SetStartDir( "Shader" );
	ErrorReport::SetPosition( 1070, 27 );

	m_pAnimateFXMgr = new ioAnimateFXManager;
	wsprintf( szPath, "%s/Effect/AnimateFX", m_szResourcePath );
	m_pAnimateFXMgr->SetStartDir( szPath );
//	m_pAnimateFXMgr->InitializeFileSet();
	ErrorReport::SetPosition( 1070, 28 );

	m_pTrailDataMgr = new ioMeshTrailDataManager;
	ioMeshTrailer::SetUseGlowTrail( Setting::TrailGlow() );
	ErrorReport::SetPosition( 1070, 29 );
	
	m_pEffectFac = new ioEffectFactory;
	m_pEffectFac->InitMemoryPoolSize( 10000, 5000 );
	ApplyEffectQuality( Setting::GetEffectQuality() );
	wsprintf( szPath, "%s/Effect", m_szResourcePath );
	m_pEffectFac->SetStartDir( szPath );
	ErrorReport::SetPosition( 1070, 30 );

	m_pTexEffMgr = new ioTextureEffectManager;
	ErrorReport::SetPosition( 1070, 31 );

	m_pSceneMgr	 = new ioGameSceneManager( m_pRenderSystem );
	m_pSceneMgr->SetSoftwareVertexBlending( Setting::SoftSkinning() );
	m_pSceneMgr->EnableLocalGlow( Setting::TrailGlow() );
	m_pSceneMgr->EnableGlow( Setting::UseGlow() );
	m_pSceneMgr->EnableFilter( Setting::GetFilterType() );
	m_pSceneMgr->EnableOveray( Setting::GetOverayType() );
	ErrorReport::SetPosition( 1070, 32 );

	m_pCamera = new ioCamera;
	ioCameraController *pController = new ioLookAtCameraController;
	m_pCamera->SetController( pController );
	ErrorReport::SetPosition( 1070, 33 );

	m_pUIRenderer = new ioUIRenderer;
	m_pUIRenderer->Initialize();
	ErrorReport::SetPosition( 1070, 34 );

	m_pFontMgr	= new ioFontManager;
	m_pFontMgr->Initialize( m_pUIRenderer );
	ErrorReport::SetPosition( 1070, 35 );

	wsprintf( szPath, "%s/Font", m_szResourcePath );
	m_pFontMgr->SetStartDir( szPath );
	m_pFontMgr->SetFont( "lostsaga.ttf" );
	ioINILoader_e kLoader( "config/sp2.ini" );
	kLoader.SetTitle_e( "font" );
	m_pFontMgr->SetFontSize( kLoader.LoadInt_e( "size", 24 ),  kLoader.LoadInt_e( "whitespacesize", 7 ) );
	ErrorReport::SetPosition( 1070, 36 );

	m_pTickTimer   = new ioTickTimer;	
	InitGlobalTimer();	
	float fNewRateX = (float)Setting::Width() / 800.0f;
	float fNewRateY = (float)Setting::Height() / 600.0f;
	ioWnd::SetScreenRate( fNewRateX, fNewRateY );
	ioMouse::SetMaxWindowSize( Setting::Width(), Setting::Height() );
	ErrorReport::SetPosition( 1070, 37 );

	m_pGUIMgr = new ioSP2GUIManager;
	if( !m_pGUIMgr )
		ErrorReport::SetPosition( 1070, 371 );
	m_pGUIMgr->InitManager();

	m_pBasicTooltipManager = new ioBasicTooltipManager;
	m_pBasicTooltipManager->Initialize();

	if( m_pPowerUpMgr )
		m_pPowerUpMgr->LoadGUI();

	ErrorReport::SetPosition( 1070, 38 );
	m_pChatMgr = new ioSP2ChatManager;

	m_pChatMgr->InitChatManager();
	ErrorReport::SetPosition( 1070, 39 );

	m_pSoundMgr = new ioSoundManager;
	wsprintf( szPath, "%s/wave", m_szResourcePath );
	m_pSoundMgr->SetStartDir( szPath );
	m_pSoundMgr->InitDSound( m_hWnd, 22050, 16, 2 );
	ErrorReport::SetPosition( 1070, 40 );

	ErrorReport::SetPosition( 1070, 40.5 );
	m_pExSoundMgr = new ioExtendSoundManager;
	m_pExSoundMgr->Initialize();
	ErrorReport::SetPosition( 1070, 40.6 );

	m_pChannelingMgr    = new ioChannelingNodeManager; // RenderLoading() ������ �Ҵ�.
	m_pChannelingMgr->Init();

	wsprintf_e( szPath, "%s/Model", m_szResourcePath );
	ioEntityGroupBuilder::SetStartDir( szPath );
	ErrorReport::SetPosition( 1070, 41 );
	
	m_pRenderSystem->SetLightEnable( false );
	m_pRenderSystem->SetTextureFiltering( 0, TFO_BILINEAR );
	m_pRenderSystem->SetTextureFiltering( 1, TFO_BILINEAR );
	ErrorReport::SetPosition( 1070, 42 );

	kLoader.SetTitle_e( "projection" );
	m_fDefaultFov  = kLoader.LoadFloat( "fov", 45.0f );
	m_fDefaultNear = kLoader.LoadFloat_e( "near", FLOAT100 );
	m_fDefaultFar  = kLoader.LoadFloat( "far", 10000.0f );
	ErrorReport::SetPosition( 1070, 43 );

	kLoader.SetTitle_e( "loading_info" );
	m_dwLoadingLogoTime = kLoader.LoadInt_e( "loading_start_delay", 1000 );
	m_dwLoadingRegulation = kLoader.LoadInt_e( "loading_regulation_delay", 3000 );
	m_dwLoadingDelay = kLoader.LoadInt_e( "loading_end_delay", 5000 );
	char *pTemp = NULL;
	kLoader.LoadString_e( "loading_logo_back_color", "", szPath, MAX_PATH );
	m_dwLoadingLogoColor = strtoul( szPath, &pTemp, 16);
	kLoader.LoadString_e( "loading_issue_back_color", "", szPath, MAX_PATH );
	m_dwLoadingIssueColor= strtoul( szPath, &pTemp, 16);
	kLoader.LoadString_e( "loading_start_sound", "", szPath, MAX_PATH );
	m_LoadingStartSound = szPath;
	kLoader.LoadString_e( "loading_end_sound", "", szPath, MAX_PATH );
	m_LoadingEndSound = szPath;

	ErrorReport::SetPosition( 1070, 44 );
	ioThreadTaskManager::GetInstance().Initialize();

	ReSize( m_iWidth, m_iHeight );
	ShowCursor( FALSE );
	ErrorReport::SetPosition( 1070, 45 );

	if( g_ExSoundMgr.IsSound( ExSound::EST_COMPANY ) )
	{
		g_ExSoundMgr.PlaySound( ExSound::EST_COMPANY );
	}
	else
	{
		g_SoundMgr.PlaySound( m_LoadingStartSound, DSBVOLUME_MAX, PSM_NONE );
	}
	
	m_pBackground = new ioInterfaceBackground; // RenderLoading() ������ �Ҵ�.
	m_pBackground->LoadBackgroundINI();

	RenderLoading( m_iModeType, m_iModeSubNum, m_iModeMapNum, m_bCampBattle, m_bCampHeroBattle, m_ePlazaType, 0, 10 );
	m_pBackground->LoadCustomLobbyBack();		// �κ� Ŀ����
	m_pBackground->LoadCustomLoadingBack();		// �ε� Ŀ����
	m_pBackground->LoadCustomBattleBack();		// ���� Ŀ����
	m_pBackground->LoadCustomLadderBack();		// ���� Ŀ����
	m_pBackground->LoadCustomFindModeBack();	// ���� ����/�� ��Ż�� ��� �˻�
	
	ErrorReport::SetPosition( 1000, 600 );

	if( g_ResourceLoader.IsLoadInPack() )
	{
		ErrorReport::SetPosition( 1000, 601 );

		char szRootDir[MAX_PATH];
		::GetCurrentDirectory(MAX_PATH,szRootDir);

		char szFullPath[MAX_PATH];
		memset( szFullPath, 0, sizeof(szFullPath) );
		StringCbPrintf_e( szFullPath, sizeof(szFullPath), "%s\\rs.iop" , szRootDir );
	
		//151221 331������ ������ ���� �÷��� �����ϵ��� ����
		bool bBeginDividePack = g_ResourceLoader.BeginDividePackFile( szFullPath, 6 , "");

		/*
		if( false )
		{
			ioHashString szIP;
			if( m_pUDPSocket )
				szIP = m_pUDPSocket->GetMyIP();
			StringCbPrintf( g_szSendErrorLog, sizeof( g_szSendErrorLog ), "Resource loading fail [0] %s/%s/%s\n%s",	szIP.c_str(), g_MyInfo.GetPrivateID().c_str(), g_MyInfo.GetPublicID().c_str(), Setting::GetFullSystemInfo() );
			SP2Packet kPacket( LUPK_LOG );
			kPacket << "CheckError";  // �α� ���� Ÿ��
			kPacket << g_szSendErrorLog;
			kPacket << 331;   // ������ȣ
			kPacket << false; // write db
			P2PNetwork::SendLog(kPacket);
			g_bHappenError = true;
			g_iErrorNumber = 331;
			SafeSprintf( g_szErrorLog, sizeof(g_szErrorLog), STR(3) );
			LOG.PrintTimeAndLog(0, "%s", g_szSendErrorLog );

#if defined( USE_GA )
			if( GetGAStart() == true )
			{
				char chLabel[32] = {0,};

				if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
					sprintf_e( chLabel, "%d", 331 );
				else
					SafeSprintf( chLabel, sizeof(chLabel), "%1", 331 );

				// GAME_END_ERR_POS
				g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
					, "Game"
					, "Error"
					, chLabel
					, 1
					, "%2FGAME%2FOVER%2FERR"
					, 1 );
			}		
#endif


#if defined( USE_GA )
			if( GetGAStart() == true )
			{
				char chLabel[32] = {0,};

				if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
					sprintf_e( chLabel, "%d", 331 );
				else
					SafeSprintf( chLabel, sizeof(chLabel), "%1", 331 );

				// GAME_END_ERR_POS
				g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
					, "Game"
					, "Error"
					, chLabel
					, 1
					, "%2FGAME%2FOVER%2FERR"
					, 1 );
			}		
#endif

			return false;
		}
		*/
		ErrorReport::SetPosition( 1000, 602 );

		//151221 331������ ������ ���� �÷��� �����ϵ��� ����
		if( bBeginDividePack )
		{
			for( int i = 0; i < 6; i++ )
			{
				ErrorReport::SetPosition( 1000, 603 );

				if( !g_ResourceLoader.LoadDividePackFile() )
				{
					ioHashString szIP;
					if( m_pUDPSocket )
						szIP = m_pUDPSocket->GetMyIP();
					StringCbPrintf( g_szSendErrorLog, sizeof( g_szSendErrorLog ), "Resource loading fail [%d] %s/%s/%s\n%s", i+1 , szIP.c_str(), g_MyInfo.GetPrivateID().c_str(), g_MyInfo.GetPublicID().c_str(), Setting::GetFullSystemInfo() );
					SP2Packet kPacket( LUPK_LOG );
					kPacket << "CheckError";  // �α� ���� Ÿ��
					kPacket << g_szSendErrorLog;
					kPacket << 331;   // ������ȣ
					kPacket << false; // write db
					P2PNetwork::SendLog(kPacket);
					g_bHappenError = true;
					g_iErrorNumber = 331;
					SafeSprintf( g_szErrorLog, sizeof(g_szErrorLog), STR(4) );
					LOG.PrintTimeAndLog(0, "%s", g_szSendErrorLog );

#if defined( USE_GA )
					if( GetGAStart() == true )
					{
						char chLabel[32] = {0,};

						if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
							sprintf_e( chLabel, "%d", 331 );
						else
							SafeSprintf( chLabel, sizeof(chLabel), "%1", 331 );

						// GAME_END_ERR_POS
						g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
							, "Game"
							, "Error"
							, chLabel
							, 1
							, "%2FGAME%2FOVER%2FERR"
							, 1 );
					}		
#endif

					return false;
				}

				ErrorReport::SetPosition( 1000, 604 );
				RenderLoading( m_iModeType, m_iModeSubNum, m_iModeMapNum, m_bCampBattle, m_bCampHeroBattle, m_ePlazaType, i+1, 10 );
			}
		}

		//151221 331������ ������ ���� �÷��� �����ϵ��� ����
		if( bBeginDividePack )
			g_ResourceLoader.EndDividePackFile();

		ErrorReport::SetPosition( 1000, 605 );
		
		int iPackSize = g_ResourceLoader.GetPackFileMemSize( szFullPath );
		if( iPackSize > 0 )
			m_vPackFileSize.push_back( std::make_pair( "rs.iop", iPackSize ) );

/*
#ifdef SHIPPING
		if( Setting::GetSystemMemory() >= 900 ) // ���� 1G �̻� ( SystemMemory�� ��Ȯ�� ������ �����Ƿ� 900 �̻� �̸� )
		bool bLocalOK = false;
		ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
		if( pLocal && pLocal->IsMemoryLoadMapPac() )
			bLocalOK = true;

		ioINILoader kIniLoder(SP::GetRC5DecodeStr( "\x24\xc8\xd5\x62\x38\xf8\x77\xba\x25\x3e\x0b\x44\xe6\x79\x24\x26\x88\x61\x58\x71\x49\x08\xe0\x5b\xb0\x62\x14\x1d\x10\x94\x96\x18", 88964032, ioHashString() ),false);
		kIniLoder.SetTitle(SP::GetRC5DecodeStr( "\x25\x36\xd1\x3f\x74\xa0\xe6\x73", 3238328008, ioHashString() ));
		if( bLocalOK && kIniLoder.LoadBool(SP::GetRC5DecodeStr( "\x6f\x7b\x2f\x45\xd3\x8c\xb1\x6a\x59\xa6\x6c\xf2\xfd\x03\x5c\xf9\x99\x13\x5f\x06\x64\x0d\x76\xdd", 2737266024, ioHashString() ), true ) )
		{
			char szFullPath[MAX_PATH];
			memset( szFullPath, 0, sizeof(szFullPath) );
			StringCbPrintf_e( szFullPath, sizeof(szFullPath), "%s\\map.iop" , szRootDir );
			if( !g_ResourceLoader.BeginDividePackFile( szFullPath, 24 , "") )
				return false;

			ErrorReport::SetPosition( 1000, 602 );

			for( int i=0 ; i<24 ; i++ )
			{
				ErrorReport::SetPosition( 1000, 603 );

				if( !g_ResourceLoader.LoadDividePackFile() )
					return false;

				ErrorReport::SetPosition( 1000, 604 );
				RenderLoading( m_iModeType, m_iModeSubNum, m_iModeMapNum, m_bCampBattle, m_bCampHeroBattle, m_ePlazaType, i+7, 33 );
			}

			g_ResourceLoader.EndDividePackFile();
			ErrorReport::SetPosition( 1000, 605 );

			iPackSize = g_ResourceLoader.GetPackFileMemSize( szFullPath );
			if( iPackSize > 0 )
			m_vPackFileSize.push_back( std::make_pair( "map.iop", iPackSize ) );
		}
#endif 
*/
	}

	ErrorReport::SetPosition( 1000, 606 );
	g_ResourceLoader.ClearDeleteFileList();

	ErrorReport::SetPosition( 1000, 607 );
	m_Mouse.InitResource();
	ErrorReport::SetPosition( 1000, 608 );
	RaceSetting::LoadSetting();
	ErrorReport::SetPosition( 1000, 609 );

	Help::InitHelpConstant();
	Help::LoadVolumesRate();
	ChatMacro::SetMacroText();

	ErrorReport::SetPosition( 1000, 610 );
	m_pStatAttribute = new ioStatAttribute;
	ErrorReport::SetPosition( 1000, 611 );
	m_pStatAttribute->InitINIValue();

	ErrorReport::SetPosition( 1000, 612 );

	m_pItemMaker = new ioItemMaker;
	m_pItemMaker->LoadItemInfo();

	// HARDCODE:: �ݵ�� skillmaker���� buffmaker�� ���� �����ؾ��Ѵ�
	_ENCSTR( "config/sp2_buff.ini", szEncBuffList );
	_ENCSTR( "config/sp2_buff_npc.ini", szEncBuffListNPC );
	_ENCSTR( "config/sp2_accessory_buff.ini", szAccessoryBuffList );

	m_pBuffMaker = new ioBuffMaker;
	m_pBuffMaker->ClearBuffList();
	m_pBuffMaker->LoadBuffList( szEncBuffList );
	m_pBuffMaker->LoadBuffList( szEncBuffListNPC );
	m_pBuffMaker->LoadBuffList( szAccessoryBuffList );

	_ENCSTR( "config/sp2_skill.ini", szEncSkillList );
	_ENCSTR( "config/sp2_skill_npc.ini", szEncSkillListNPC );
	_ENCSTR( "config/sp2_accessory_skill.ini", szAccessorySkillList );
	m_pSkillMaker = new ioSkillMaker;
	m_pSkillMaker->ClearSkillList();
	m_pSkillMaker->LoadSkillList( szEncSkillList );
	m_pSkillMaker->LoadSkillList( szEncSkillListNPC );
	m_pSkillMaker->LoadSkillList( szAccessorySkillList );

	m_pWeaponMgr = new ioWeaponManager( m_pSceneMgr );
	m_pWeaponMgr->InitManager();

	ErrorReport::SetPosition( 1000, 613 );

	ErrorReport::SetPosition( 1000, 614 );
	m_pChatMacro = new ioChatMacro;
	ErrorReport::SetPosition( 1000, 615 );
	m_pChatMacro->LoadChatMacroInfo();

	ErrorReport::SetPosition( 1000, 616 );
	m_pChatMacroMgr = new ChatMacroMgr;

	ErrorReport::SetPosition( 1000, 617 );
	m_pSetItemInfoMgr = new ioSetItemInfoManager;
	ErrorReport::SetPosition( 1000, 618 );
	m_pSetItemInfoMgr->LoadInfoList();

	ErrorReport::SetPosition( 1000, 619 );
	m_pEtcItemMgr = new ioEtcItemManager;
	ErrorReport::SetPosition( 1000, 620 );
	m_pEtcItemMgr->LoadEtcItem();
	
	m_pTournamentRewardManager = new TournamentRewardManager;
	m_pTournamentRewardManager->Initialize();

	ErrorReport::SetPosition( 1000, 621 );
	m_pExtraItemInfoMgr = new ioExtraItemInfoManager;
	ErrorReport::SetPosition( 1000, 622 );
	m_pExtraItemInfoMgr->LoadRandomMachineInfo();

	ErrorReport::SetPosition( 1000, 623 );
	m_pItemCompoundMgr = new ioItemCompoundManager;
	ErrorReport::SetPosition( 1000, 624 );
	m_pItemCompoundMgr->LoadCompoundInfo();

	ErrorReport::SetPosition( 1000, 625 );
	m_pRoomOptionMgr = new ioRoomOptionManager;
	ErrorReport::SetPosition( 1000, 626 );
	m_pRoomOptionMgr->LoadRoomOptionInfo();

	ErrorReport::SetPosition( 1000, 627 );
	m_pModeSelectInfo = new ioModeSelectInfo;
	ErrorReport::SetPosition( 1000, 628 );
	m_pModeSelectInfo->LoadModeSelectInfo();

	ErrorReport::SetPosition( 1000, 629 );
	m_pBattleRoomMgr = new ioBattleRoomMgr;
	ErrorReport::SetPosition( 1000, 630 );
	m_pBattleRoomMgr->LoadBattleRoomInfo();

	ErrorReport::SetPosition( 1000, 631 );
	m_pLadderTeamMgr = new ioLadderTeamMgr;
	ErrorReport::SetPosition( 1000, 632 );
	m_pLadderTeamMgr->LoadLadderTeamInfo();

	ErrorReport::SetPosition( 1000, 633 );
	m_pTradeInfoMgr = new TradeInfoManager;
	ErrorReport::SetPosition( 1000, 634 );
	m_pTradeInfoMgr->LoadTradeInfo();

	m_pSlotMgr = new ioSlotMgr;  // ioMyLevelMgr �ȿ��� ����ϴϱ� �̸� ����� ����

	ErrorReport::SetPosition( 1000, 635 );
	m_pMyLevelMgr = new ioMyLevelMgr;
	ErrorReport::SetPosition( 1000, 636 );
	m_pMyLevelMgr->LoadINIInfo();

	m_pAlchemicMgr = new ioAlchemicMgr;
	m_pAlchemicMgr->LoadMgrInfo();

	m_pReservedTask = new ioReservedTask;

	RenderLoading( m_iModeType, m_iModeSubNum, m_iModeMapNum, m_bCampBattle, m_bCampHeroBattle, m_ePlazaType, 31, 33 );

	ErrorReport::SetPosition( 1000, 637 );
	m_pTutorialMgr  = new ioTutorialManager;
	ErrorReport::SetPosition( 1000, 638 );
	m_pTutorialMgr->LoadTutorialList();

	ErrorReport::SetPosition( 1000, 639 );
	m_pModeHelpMgr = new ioModeHelpManager;
	ErrorReport::SetPosition( 1000, 640 );
	m_pModeHelpMgr->LoadModeHelpList();
	
	ErrorReport::SetPosition( 1000, 641 );
	m_pAlarmMgr = new ioAlarmManager;
	ErrorReport::SetPosition( 1000, 642 );
	m_pAlarmMgr->Initialize();

	ErrorReport::SetPosition( 1000, 643 );
	m_pTotalManualMgr = new ioTotalManualMgr;
	ErrorReport::SetPosition( 1000, 644 );
	m_pTotalManualMgr->LoadManual_e( "config/sp2_manual.ini" );

	ErrorReport::SetPosition( 1000, 645 );
	m_pSoldierManualMgr = new ioSoldierManualMgr;
	ErrorReport::SetPosition( 1000, 646 );
	m_pSoldierManualMgr->LoadManual_e( "config/sp2_soldier_manual.ini" );

	ErrorReport::SetPosition( 1000, 647 );
	m_pAnnouncementManualMgr = new ioAnnouncementManualMgr;
	ErrorReport::SetPosition( 1000, 648 );
	m_pAnnouncementManualMgr->LoadManual_e( "config/sp2_announcement.ini" );

	ErrorReport::SetPosition( 1000, 649 );
	m_pItemManualMgr = new ioItemManualMgr;
	ErrorReport::SetPosition( 1000, 650 );
	m_pItemManualMgr->LoadManual_e( "config/sp2_item_manual.ini");

	ErrorReport::SetPosition( 1000, 651 );
	m_pSkillManualMgr = new ioSkillManualMgr;
	ErrorReport::SetPosition( 1000, 652 );
	m_pSkillManualMgr->LoadManual_e( "config/sp2_skill_manual.ini" );

	ErrorReport::SetPosition( 1000, 653 );
	m_pDecoManualMgr = new ioDecoManualMgr;
	ErrorReport::SetPosition( 1000, 654 );
	m_pDecoManualMgr->LoadManual_e( "config/sp2_deco_manual.ini" );

	ErrorReport::SetPosition( 1000, 655 );
	m_pEtcManualMgr = new ioEtcManualMgr;
	ErrorReport::SetPosition( 1000, 656 );
	m_pEtcManualMgr->LoadManual_e( "config/sp2_etc_manual.ini" );

	ErrorReport::SetPosition( 1000, 657 );
	m_pEventGoodsManualMgr = new ioEventGoodsManualMgr;
	ErrorReport::SetPosition( 1000, 658 );
	m_pEventGoodsManualMgr->LoadManual_e( "config/sp2_event_goods_manual.ini" );

	ErrorReport::SetPosition( 1000, 659 );
	m_pEtcInvenManualMgr = new ioEtcInvenManualMgr;
	ErrorReport::SetPosition( 1000, 660 );
	m_pEtcInvenManualMgr->LoadManual_e( "config/sp2_etc_inven_manual.ini" );

	ErrorReport::SetPosition( 1000, 661 );
	m_pEtcInvenSubManualMgr = new ioEtcInvenSubManualMgr;
	ErrorReport::SetPosition( 1000, 662 );
	m_pEtcInvenSubManualMgr->LoadManual_e( "config/sp2_etc_inven_sub_manual.ini" );

	ErrorReport::SetPosition( 1000, 663 );
	m_pPresentInvenManualMgr = new ioPresentInvenManualMgr;
	ErrorReport::SetPosition( 1000, 664 );
	m_pPresentInvenManualMgr->LoadManual_e( "config/sp2_present_inven_manual.ini" );

	ErrorReport::SetPosition( 1000, 665 );
	m_pExtraInvenManualMgr = new ioExtraInvenManualMgr;
	ErrorReport::SetPosition( 1000, 666 );
	m_pExtraInvenManualMgr->LoadManual_e( "config/sp2_extra_inven_manual.ini" );

	ErrorReport::SetPosition( 1000, 667 );
	m_pMedalInvenManualMgr = new ioMedalInvenManualMgr;
	ErrorReport::SetPosition( 1000, 668 );
	m_pMedalInvenManualMgr->LoadManual_e( "config/sp2_medal_inven_manual.ini" );

	ErrorReport::SetPosition( 1000, 669 );
	m_pDecoInvenManualMgr = new ioDecoInvenManualMgr;
	ErrorReport::SetPosition( 1000, 670 );
	m_pDecoInvenManualMgr->LoadManual_e( "config/sp2_deco_inven_manual.ini" );

	ErrorReport::SetPosition( 1000, 671 );
	m_pItemInvenManualMgr = new ioItemInvenManualMgr;
	ErrorReport::SetPosition( 1000, 672 );
	m_pItemInvenManualMgr->LoadManual_e( "config/sp2_item_inven_manual.ini" );

	m_pAlchemicFuncManualMgr = new ioAlchemicFuncManualMgr;
	m_pAlchemicFuncManualMgr->LoadManual_e( "config/sp2_alchemic_func_manual.ini" );

	m_pPetInvenManualMgr = new ioPetInvenManualMgr;
	m_pPetInvenManualMgr->LoadManual_e( "config/sp2_pet_inven_manual.ini" );

	m_pCostumeInvenManualMgr = new ioCostumeInvenManualMgr;
	m_pCostumeInvenManualMgr->LoadManual_e( "config/sp2_costume_inven_manual.ini" );
	
	m_pCostumeShopManualMgr = new ioCostumeShopManualMgr;
	m_pCostumeShopManualMgr->LoadManual_e( "config/sp2_costume_shop_manual.ini" );

	m_pAccessoryInvenManualMgr = new ioAccessoryInvenManualMgr;
	m_pAccessoryInvenManualMgr->LoadManual_e( "config/sp2_accessory_manual.ini" );

	ErrorReport::SetPosition( 1000, 673 );
	m_pAwardMgr  = new ioAwardManager;
	ErrorReport::SetPosition( 1000, 674 );
	m_pAwardMgr->LoadAwardInfo();

	ErrorReport::SetPosition( 1000, 675 );
	m_pModeUIController = new ioModeUIController;
	ErrorReport::SetPosition( 1000, 676 );
	m_pModeUIController->LoadINI();

	ErrorReport::SetPosition( 1000, 677 );
	m_pVictoriesMgr = new ioVictoriesMgr;
	ErrorReport::SetPosition( 1000, 678 );
	m_pPlayBonusMgr = new ioPlayBonusMgr;

	m_pTreasureCardMgr = new ioTreasureCardMgr;
	m_pTreasureCardMgr->Initialize();

	ErrorReport::SetPosition( 1000, 679 );
	m_pGuildMarkMgr	= new ioGuildMarkMgr;
	ErrorReport::SetPosition( 1000, 680 );
	m_pGuildMarkMgr->Initialize();

	ErrorReport::SetPosition( 1000, 681 );
	m_pGuildChatMgr = new ioGuildChatMgr;

	ErrorReport::SetPosition( 1000, 682 );
	m_pCustomTextureMgr = new ioCustomTextureMgr;
	ErrorReport::SetPosition( 1000, 683 );
	m_pCustomTextureMgr->Initialize();

	ErrorReport::SetPosition( 1000, 684 );
	m_pSlangUtil = new CSlangUtil;
	ErrorReport::SetPosition( 1000, 685 );
	m_pSlangUtil->LoadSlangData();
	
	ErrorReport::SetPosition( 1000, 686 );
	m_pBlockKeyBoard = new CBlockKeyBoard( Setting::IsWin98Series() );

	ErrorReport::SetPosition( 1000, 687 );
	m_pCustomSoundMgr = new ioCustomSoundMgr;
	ErrorReport::SetPosition( 1000, 688 );
	m_pCustomSoundMgr->LoadINI( g_MyInfo.GetUserIndex(), g_MyInfo.GetPublicID() );

	ErrorReport::SetPosition( 1000, 689 );
	m_pMovieMaker = new ioMovieMaker;
	ErrorReport::SetPosition( 1000, 690 );
	m_pMovieMaker->Initialize();

	ErrorReport::SetPosition( 1000, 691 );
	m_pGameAnnMgr = new ioGameAnnMgr;
	ErrorReport::SetPosition( 1000, 692 );
	m_pGameAnnMgr->Initialize();

	ErrorReport::SetPosition( 1000, 693 );
	m_pGrowthInfoMgr = new ioGrowthInfoManager;
	ErrorReport::SetPosition( 1000, 694 );
	m_pGrowthInfoMgr->LoadGrowthInfoList();
    
	ErrorReport::SetPosition( 1000, 695 );
	m_pFishingMgr = new ioFishingManager;
	ErrorReport::SetPosition( 1000, 696 );
	m_pFishingMgr->LoadFishingInfo();

	ErrorReport::SetPosition( 1000, 697 );
	m_pQuestManager = new ioQuestManager;
	ErrorReport::SetPosition( 1000, 698 );
	m_pQuestManager->LoadINIData();

	ErrorReport::SetPosition( 1000, 699 );
	m_pQuestGuide = new ioQuestGuide;
	m_pQuestGuide->LoadINIData();

	ErrorReport::SetPosition( 1000, 700 );
	m_pItemCompoundMaterialMgr = new ioItemCompoundMaterialManager;
	ErrorReport::SetPosition( 1000, 701 );
	m_pItemCompoundMaterialMgr->LoadCompoundMaterialInfo();

	ErrorReport::SetPosition( 1000, 702 );
	m_pPetInfoMgr = new ioPetInfoManager;
	ErrorReport::SetPosition( 1000, 703 );
	m_pPetInfoMgr->LoadProperty();

	ErrorReport::SetPosition( 1000, 704 );
	m_pPetEatInfoMgr = new ioPetEatInfoManager;
	ErrorReport::SetPosition( 1000, 705 );
	m_pPetEatInfoMgr->LoadProperty();

	ErrorReport::SetPosition( 1000, 706 );
	m_pShopItemExtraInfoManager = new ioShopItemExtraInfoManager;
	ErrorReport::SetPosition( 1000, 707 );
	m_pShopItemExtraInfoManager->LoadProperty();

	ErrorReport::SetPosition( 1000, 708 );
	m_pFlashDownLoadMgr = new ioFlashDownLoadMgr;
	ErrorReport::SetPosition( 1000, 709 );
	m_pFlashDownLoadMgr->Initialize();

	ErrorReport::SetPosition( 1000, 710 );
	m_pCostumeInfoMgr = new ioCostumeInfoManager;
	ErrorReport::SetPosition( 1000, 711 );
	m_pCostumeInfoMgr->LoadProperty();

	m_pFieldRewardItemMgr = new ioFieldRewardItemMgr;
	m_pFieldRewardItemMgr->LoadINIData();

	m_pFieldItemMgr = new ioFieldItemMgr;
	m_pFieldItemMgr->LoadINIData();

	m_pMedalItemInfoMgr = new ioMedalItemInfoManager;
	m_pMedalItemInfoMgr->LoadINI();

	m_pExpandMedalSlotManager = new ioExpandMedalSlotManager;
	
	m_pItemRechargeMgr = new ioItemRechargeManager;
	m_pItemRechargeMgr->LoadInIData();

	m_pModeItemMgr = new ioModeItemManager;
	m_pModeItemMgr->ClearModeItemList();
	m_pModeItemMgr->LoadItemList();

	m_pShuffleRoomMgr = new ioShuffleRoomMgr;
	m_pShuffleRoomMgr->LoadShuffleRoomInfo();

	m_pGashponMgr = new ioGashaponMgr;
	m_pGashponMgr->Initialize();
	if ( m_pGuildInfo )
		m_pGuildInfo->InitializeGuildReward();

	m_pSoldierAwakeMgr = new ioSoldierAwakeManager;
	m_pSoldierAwakeMgr->Initialize();

	m_pModeResourceMgr = new ioModeResourceManager;
	m_pModeResourceMgr->InitCommonModeIcon();

	m_pRequestCharMgr = new ioRequestCharManager;
	m_pRequestCharMgr->LoadProperty();

	m_pRequestLagMgr = new ioRequestLagManager;
	m_pRequestLagMgr->LoadProperty();

	m_pCostumeShopInfoMgr = new ioCostumeShopInfoManager;
	m_pCostumeShopInfoMgr->LoadCostumeMachineInfo();

	m_pSpecialShopMgr = new ioSpecialShopMgr;
	m_pSpecialShopMgr->Initialize();
	
	m_pShopDataCache = new ioShopDataCache;

	m_pPopupStoreManualMgr = new ioPopupStorePresentItemManualMgr;
	m_pPopupStoreManualMgr->LoadManual_e( "config/sp2_popupstore_manual.ini" );

	m_pShopMgr = new ioShopManager;
	m_pShopMgr->Initialize();

	m_pUIImageRenderManager = new ioUIImageRenderManager;
	m_pUIImageRenderManager->LoadProperty();

	m_pSuperGashponMgr = new ioSuperGashaponMgr;
	m_pSuperGashponMgr->Initialize();

	m_pRenderTargetMgr = new ioRenderTargetManager;
	m_pGuildBlockStorage = new ioGuildBlockStorage;
	if( m_pGuildBlockStorage )
		m_pGuildBlockStorage->Initialize();

	m_pHomeBlockStorage = new ioHomeBlockStorage;
	if( m_pHomeBlockStorage )
		m_pHomeBlockStorage->Initialize();

	m_pBlockEventAgency = new ioBlockEventAgency;

	m_pNamedTitleInfoManager = new ioNamedTitleInfoManager;
	m_pNamedTitleInfoManager->LoadProperty();

	//���ʽ� ĳ��
	m_pBonusCashMgr = new ioBonusCashManager;
	m_pBonusCashMgr->LoadBonusCashInfo();

	m_pAccessoryInfoMgr = new ioAccessoryInfoManager;
	m_pAccessoryInfoMgr->LoadAccessoryInfo();

	ioChannelingNodeParent *pNode = g_ChannelingMgr.GetNode( g_MyInfo.GetChannelingType() );
	if( pNode )
	{
		if( m_pBrowserMgr )
			m_pBrowserMgr->LoadINI( pNode->GetININame() );
		else
			LOG.PrintTimeAndLog( 0, "Setup pBrowserMgr == NULL." );
	}
	else
		LOG.PrintTimeAndLog( 0, "Setup pNode == NULL." );

	LoadNotMakeID();
	SetupForSound();

	RenderLoading( m_iModeType, m_iModeSubNum, m_iModeMapNum, m_bCampBattle, m_bCampHeroBattle, m_ePlazaType, 8, 10 );


	m_pFrameTimer->Frame();

	g_MyInfo.SetGradeExpRateBeforeCon( g_MyInfo.GetGradeExpRate() );

#ifdef _DEBUG
	//g_PresentMgr._DebugLogTreasureCardItem();
#endif

	SendINILog();

#if defined( USE_GA )
	// GA ������ ���� �Ǵ�. ex> ( User Index % 100 == 0 ), 1% �� sampling
	int iUID = (int)g_MyInfo.GetUserIndex();
	if( iUID % m_pHttpMng->GetGAGameDataSampling() == 0 )
		m_bGAUser = true;

	char chResolution[32]	= {0,};
	char chOS[128]			= {0,};

	if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
	{
		sprintf_e( chOS, "OS : %s", Setting::GetOSVersion() );
		sprintf_e( chResolution, "%dx%d", Setting::Width(), Setting::Height() );
	}
	else
	{
		SafeSprintf( chOS, sizeof(chOS), "OS : %1", Setting::GetOSVersion() );
		SafeSprintf( chResolution, sizeof(chResolution), "%1x%2", Setting::Width(), Setting::Height() );
	}	

	// ACCOUNT_LOGIN
	g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FACCOUNT%2FLOGIN", 1, "", 0, chOS, chResolution );

#endif

	return true;
}

void ioApplication::ReloadINILists()
{
	if( !m_bReloadINILists )
		return;

	m_bReloadINILists = false;

	if( m_pModeSelectInfo )
		m_pModeSelectInfo->LoadModeSelectInfo();

	if( m_pDecorationPrice )
		m_pDecorationPrice->LoadPriceInfo();

	ioINILoader_e kLoader( "config/sp2.ini" );

	if( m_pAbuseMgr )
		m_pAbuseMgr->LoadINIInfo( kLoader );

	m_dwMouseUnvisiblTime		= kLoader.LoadInt_e( "mouse", "mouse_unvisible_time", 1000 );
	m_dwMouseUnvisiblLobbyTime	= kLoader.LoadInt_e( "mouse", "mouse_unvisible_lobby_time", 1000 );

	kLoader.SetTitle_e( "projection" );
	m_fDefaultFov  = kLoader.LoadFloat( "fov", 45.0f );
	m_fDefaultNear = kLoader.LoadFloat_e( "near", FLOAT100 );
	m_fDefaultFar  = kLoader.LoadFloat( "far", 10000.0f );
	ReSize( m_iWidth, m_iHeight );

	if( m_pChatMgr )
		m_pChatMgr->InitChatManager();

	m_Mouse.InitResource();
	Help::InitHelpConstant();

	if( m_pStatAttribute )
		m_pStatAttribute->InitINIValue();

	if( m_pItemMaker )
		m_pItemMaker->LoadItemInfo();

	if( m_pBuffMaker )
	{
		m_pBuffMaker->ClearBuffList();
		m_pBuffMaker->LoadBuffList_e( "config/sp2_buff.ini" );
		m_pBuffMaker->LoadBuffList_e( "config/sp2_buff_npc.ini" );
		m_pBuffMaker->LoadBuffList_e( "config/sp2_accessory_buff.ini" );
	}

	if( m_pSkillMaker )
	{
		m_pSkillMaker->ClearSkillList();
		m_pSkillMaker->LoadSkillList_e( "config/sp2_skill.ini" );
		m_pSkillMaker->LoadSkillList_e( "config/sp2_skill_npc.ini" );
		m_pSkillMaker->LoadSkillList_e( "config/sp2_accessory_skill.ini" );
	}

	if( m_pModeItemMgr )
	{
		m_pModeItemMgr->ClearModeItemList();
		m_pModeItemMgr->LoadItemList();
	}

	if( m_pWeaponMgr )
		m_pWeaponMgr->InitManager();

	if( m_pChatMacro )
		m_pChatMacro->LoadChatMacroInfo();

	if( m_pSetItemInfoMgr )
		m_pSetItemInfoMgr->LoadInfoList();

	if( m_pEtcItemMgr )
		m_pEtcItemMgr->LoadEtcItem();

	if( m_pSlotMgr )
		m_pSlotMgr->InitSlot();

	if( m_pMyLevelMgr )
		m_pMyLevelMgr->LoadINIInfo();

	if( m_pTutorialMgr )
		m_pTutorialMgr->LoadTutorialList();

	if( m_pModeHelpMgr )
		m_pModeHelpMgr->LoadModeHelpList();

	if( m_pTotalManualMgr )
		m_pTotalManualMgr->LoadManual_e( "config/sp2_manual.ini" );

	if( m_pSoldierManualMgr )
		m_pSoldierManualMgr->LoadManual_e( "config/sp2_soldier_manual.ini" );

	if( m_pAnnouncementManualMgr )
		m_pAnnouncementManualMgr->LoadManual_e( "config/sp2_announcement.ini" );

	if( m_pItemManualMgr )
		m_pItemManualMgr->LoadManual_e( "config/sp2_item_manual.ini");

	if( m_pSkillManualMgr )
		m_pSkillManualMgr->LoadManual_e( "config/sp2_skill_manual.ini" );

	if( m_pDecoManualMgr )
		m_pDecoManualMgr->LoadManual_e( "config/sp2_deco_manual.ini" );

	if( m_pEtcManualMgr )
		m_pEtcManualMgr->LoadManual_e( "config/sp2_etc_manual.ini" );

	if( m_pAwardMgr )
		m_pAwardMgr->LoadAwardInfo();

	if( m_pModeUIController )
		m_pModeUIController->LoadINI();

	if( m_pSlangUtil )
		m_pSlangUtil->LoadSlangData();

	if( m_pGuildMarkMgr )
		m_pGuildMarkMgr->Initialize();

	if( m_pBackground )
	{
		m_pBackground->LoadBackgroundINI();
		m_pBackground->LoadCustomLobbyBack();       // �κ� Ŀ����
		m_pBackground->LoadCustomLoadingBack();     // �ε� Ŀ����
		m_pBackground->LoadCustomBattleBack();      // ���� Ŀ����
		m_pBackground->LoadCustomLadderBack();      // ���� Ŀ����
		m_pBackground->LoadCustomFindModeBack();	// ���� ����/�� ��Ż�� ��� �˻�
	}

	LoadNotMakeID();
}

void ioApplication::ProcessBeforeSetUpPacket()
{
	m_bSetUpDone = true;

	ErrorReport::SetPosition( 1000, 300 );

	if( !m_TCPBeforeSetUpList.empty() )
	{
		ErrorReport::SetPosition( 1000, 301 );

		SP2PacketList::iterator iter;
		for( iter=m_TCPBeforeSetUpList.begin() ; iter!=m_TCPBeforeSetUpList.end() ; ++iter )
		{
			ErrorReport::SetPosition( 1000, 302 );
			RecvTCPPacket( *iter );
			ErrorReport::SetPosition( 1000, 303 );
		}

		m_TCPBeforeSetUpList.clear();

		ErrorReport::SetPosition( 1000, 304 );
	}

	ErrorReport::SetPosition( 1000, 305 );

	if( !m_UDPBeforeSetUpList.empty() )
	{
		ErrorReport::SetPosition( 1000, 306 );
		SP2PacketList::iterator iter;
		for( iter=m_UDPBeforeSetUpList.begin() ; iter!=m_UDPBeforeSetUpList.end() ; ++iter )
		{
			ErrorReport::SetPosition( 1000, 307 );
			RecvUDPPacket( *iter, "", 0 );
			ErrorReport::SetPosition( 1000, 308 );
		}

		m_UDPBeforeSetUpList.clear();

		ErrorReport::SetPosition( 1000, 309 );
	}

	ErrorReport::SetPosition( 1000, 310 );
	g_MyInfo.SetUserInfoMgr();
	ErrorReport::SetPosition( 1000, 311 );
	g_MyInfo.SerUserInfoAllCharData();
	ErrorReport::SetPosition( 1000, 312 );
	g_MyInfo.NewLoginQuestCheck();
	ErrorReport::SetPosition( 1000, 313 );
	g_EventGoodsMgr.SyncGoodsList();
	g_SpecialShopMgr.SyncGoodsList();
	ErrorReport::SetPosition( 1000, 314 );
}

void ioApplication::ProcessBeforeCharLoading()
{
	g_MyInfo.SetBeforeCharLoading( false );
}

void ioApplication::EnableNetworkPlay( bool bEnable )
{
	m_bNetworkPlay = bEnable;

	if( !m_bNetworkPlay )	// �̱� ����
	{
		if( m_pTCPSocket )
		{
			m_pTCPSocket->CloseClientTCP();
		}
	}
}

void ioApplication::SetDialogBoxMode( bool bDialog )
{
	if( !ioLocalManagerParent::IsIME() )
		return;

	if( !m_pRenderSystem )
		return;

	if( m_pRenderSystem->IsDialogBoxMode() == bDialog )
		return;

	if( !bDialog && m_bProcessCandidate ) 
		return;

	if( !bDialog && m_pBrowserMgr && m_pBrowserMgr->IsWebOpened() )
		return;

	m_pRenderSystem->SetDialogBoxMode( bDialog );

	// �������� or ��Ƽ�˸��ƽ� ������ ���� üũ
	if( !bDialog )
	{
		bool bResetDevice = false;

		if( Setting::SwapCopy() )
		{
			bResetDevice |= m_pRenderSystem->ChangeSwapEffect( D3DSWAPEFFECT_COPY );
		}

		AntiAliasingQuality eAntiQuality = Setting::GetAntiAliasingQuality();
		if( eAntiQuality != AQ_NONE )
		{
			D3DMULTISAMPLE_TYPE eSample = Setting::GetMultiSampleType( eAntiQuality );
			bResetDevice |= m_pRenderSystem->ChangeMultiSamplingType( eSample );
		}

		if( bResetDevice )
		{
			m_pRenderSystem->ApplyChangedPresentParameters();
			if( eAntiQuality != AQ_NONE )
			{
				m_pRenderSystem->EnableMultiSampling( true );
			}
		}
	}
}

void ioApplication::SetNagleAlgorithm( bool bOn )
{
	if( m_pTCPSocket )
	{
		m_pTCPSocket->SetNagleAlgorithm( bOn );
	}
}

void ioApplication::ReSize( int iWidth, int iHeight )
{
	if( !m_pCamera )	return;;
	if( !m_pRenderSystem->GetDevice() )	return;

	float fAspect = (float)iWidth / (float)iHeight;

	m_pCamera->SetProjection( m_fDefaultNear,
							  m_fDefaultFar,
							  m_fDefaultFov,
							  fAspect );

	m_pRenderSystem->SetProjMatrix( m_pCamera->GetProjMatrix() );
}

void ioApplication::StartChatBubble( const char *szID, const char *szChat, bool bWholeChat )
{
	if( !Setting::ChatBubbleAlarm() ) return;

	ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
	if( !pPlayStage ) return;
	if( pPlayStage->GetModeType() == MT_NONE ) return;

	if( pPlayStage->GetModeType() == MT_MYROOM )
	{
		if( ioMyRoomMode::GetMyRoomType() != MRT_PRACTICE ) return;
	}

	ioPlayMode *pPlayMode = pPlayStage->GetPlayMode();
	if( !pPlayMode ) return;

	if( pPlayStage->GetModeType() == MT_MYROOM )
	{
		ioBaseChar *pChar = pPlayMode->GetOwnerChar();
		if( !pChar ) return;

		pChar->SetChatBubble( szID, szChat, bWholeChat );
	}
	else
	{
		ioBaseChar *pChar = pPlayMode->GetRecordChar( szID );
		if( !pChar ) return;

		pChar->SetChatBubble( szID, szChat, bWholeChat );
	}
}

void ioApplication::OpenMannerWnd( const char *szNickName, bool bOnline ,ChatType eChatType /*= CT_NONE */ )
{
	if( strcmp( szNickName, "" ) == 0 )
		return;

	MannerValuationParentWnd *pMannerWnd = dynamic_cast<MannerValuationParentWnd*>(g_GUIMgr.FindWnd( MANNER_VALUATION_WND ));
	if( pMannerWnd )
	{
		if( !pMannerWnd->IsOpenState( szNickName ) )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
			return;
		}

		if( eChatType == CT_PARTY )
			pMannerWnd->SetMannerValuationInfo( szNickName, bOnline, ioMannerTrialChatManager::TT_BATTLE_CHAT );
		else if( eChatType == CT_SERVER_LOBBY )
			pMannerWnd->SetMannerValuationInfo( szNickName, bOnline, ioMannerTrialChatManager::TT_SERVER_LOBBY_CHAT );
		else if( eChatType == CT_WHOLE_SERVER )
			pMannerWnd->SetMannerValuationInfo( szNickName, bOnline, ioMannerTrialChatManager::TT_WHOLE_SERVER_CHAT );
		else if( eChatType == CT_LADDER )
			pMannerWnd->SetMannerValuationInfo( szNickName, bOnline, ioMannerTrialChatManager::TT_LADDER_CHAT );
		else if( eChatType == CT_WHOLE_SERVER_RAINBOW )
			pMannerWnd->SetMannerValuationInfo( szNickName, bOnline, ioMannerTrialChatManager::TT_WHOLE_SERVER_RAINBOW_CHAT );
		else
			pMannerWnd->SetMannerValuationInfo( szNickName, bOnline, ioMannerTrialChatManager::TT_NORMAL_CHAT );
		pMannerWnd->ShowWnd();

#if defined( USE_GA )
		// WND_USERINFO
		g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FWND%2FUSERINFO" );
#endif

	}
}

void ioApplication::OpenHeadQuaterPage( const char *szNickName, bool bNormalUser )
{
	if( !strcmp( szNickName, "" ) )
		return;

	if( !IsRightID( szNickName ) )
		return;

	ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
	if( pPlayStage )
	{
		if( pPlayStage->GetModeType() != MT_MYROOM )
		{
			ioBaseChar *pOwnerChar = pPlayStage->GetOwnerChar();
			if( pOwnerChar )
			{	
				if( !pOwnerChar->IsMiniHomePageState() )
				{
					// ���·� ���� �̴�Ȩ�� �Ұ�
					g_ChatMgr.SetSystemMsg( STR(1) );
					return;
				}
			}
		}
	}

	m_ReserveOpenHeadQuater = szNickName;

	if( P2PNetwork::IsNetworkPlaying() && bNormalUser )
	{
		SP2Packet kPacket( CTPK_REGISTERED_USER );
		kPacket << szNickName;
		SendTCPPacket( kPacket );
		SetMouseBusy( true );
	}
	else
	{
		//�̱۸���̰ų� ������K�� �ƴѰ��
		//( �̱۸�忡���� account index�� �˼� ���⶧���� �������� ��������. )
		if( m_pBrowserMgr )
		{
			char szURL[MAX_PATH];
			StringCbPrintf( szURL, sizeof( szURL ), "%s%d", m_pBrowserMgr->GetHeadQuaterURL().c_str(), 1105 );
			m_pBrowserMgr->OpenBrowser( szURL, BROWSE_SINGLE, BROWSE_SUB_QUEATER );
		}
	}
}

void ioApplication::OpenHeadQuaterPage( DWORD dwUserIndex )
{
	ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
	if( pPlayStage )
	{
		if( pPlayStage->GetModeType() != MT_MYROOM )
		{
			ioBaseChar *pOwnerChar = pPlayStage->GetOwnerChar();
			if( pOwnerChar )
			{	
				if( !pOwnerChar->IsMiniHomePageState() )
				{
					// ���·� ���� �̴�Ȩ�� �Ұ�
					g_ChatMgr.SetSystemMsg( STR(2) );
					return;
				}
			}
		}
	}

	m_ReserveOpenHeadQuater = STR(3);

	if( m_pBrowserMgr )
	{
		char szURL[MAX_PATH];
		StringCbPrintf( szURL, sizeof( szURL ), "%s%d", m_pBrowserMgr->GetHeadQuaterURL().c_str(), dwUserIndex );
		m_pBrowserMgr->OpenBrowser( szURL, BROWSE_SINGLE, BROWSE_SUB_QUEATER );

		ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
		if( pPlayStage )
		{
			ioBaseChar *pOwner = pPlayStage->GetOwnerChar();
			if( pOwner )
			{
				pOwner->SetHomePageEmoticon();
			}
		}
	}
}

void ioApplication::OpenHeadquartersMini( const ioHashString kNickName )
{
	if( kNickName.IsEmpty() ) return;
	if( m_pBrowserMgr == NULL ) return;
	if( m_pBrowserMgr->GetKeyOnCmd().IsEmpty() ) return;
	if( m_pBrowserMgr->GetHeadquartersMiniURL().IsEmpty() ) return;

	char szURL[MAX_PATH];
	StringCbPrintf( szURL, sizeof( szURL ), m_pBrowserMgr->GetHeadquartersMiniURL().c_str(), kNickName.c_str(), m_pBrowserMgr->GetKeyOnCmd().c_str() );
	m_pBrowserMgr->OpenBrowser( szURL, BROWSE_SINGLE, BROWSE_SUB_QUEATER );

	ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
	if( pPlayStage )
	{
		ioBaseChar *pOwner = pPlayStage->GetOwnerChar();
		if( pOwner )
		{
			pOwner->SetHomePageEmoticon();
		}
	}
}	

void ioApplication::OpenGuildHomePage( DWORD dwGuildIndex )
{
	if( dwGuildIndex == 0 )
		return;
	if( !m_pBrowserMgr )
		return;

	ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
	if( pPlayStage )
	{
		if( pPlayStage->GetModeType() != MT_MYROOM )
		{
			ioBaseChar *pOwnerChar = pPlayStage->GetOwnerChar();
			if( pOwnerChar )
			{	
				if( !pOwnerChar->IsMiniHomePageState() )
				{
					// ���·� ���� �̴�Ȩ�� �Ұ�
					g_ChatMgr.SetSystemMsg( STR(1) );
					return;
				}
			}
		}
	}

	char szURL[MAX_PATH];
	StringCbPrintf( szURL, sizeof( szURL ), "%s%d", m_pBrowserMgr->GetGuildHomePageURL().c_str(), dwGuildIndex );
	m_pBrowserMgr->OpenBrowser( szURL, BROWSE_SINGLE, BROWSE_SUB_GUILD );

	if( pPlayStage )
	{
		ioBaseChar *pOwner = pPlayStage->GetOwnerChar();
		if( pOwner )
			pOwner->SetHomePageEmoticon();
	}
}

bool ioApplication::IsGuildMarkChangeOK()
{
	if( !g_GuildInfo.IsGuild() )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return false;
	}

	if( !g_GuildInfo.IsGuildMaster() && !g_GuildInfo.IsGuildSecondMaster() )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return false;
	}
	return true;
}

void ioApplication::OpenGuildMarkWnd()
{
	if( !IsGuildMarkChangeOK() )
		return;

	ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
	if( !pPlayStage )
		return;

	if( g_App.GetConnectedMoveMode() == ioApplication::CMM_LOBBY )
	{
		if( pPlayStage->GetModeType() != MT_MYROOM )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
			return;
		}	
	}
	else
	{
		if( pPlayStage->GetModeType() != MT_TRAINING && pPlayStage->GetModeType() != MT_HEADQUARTERS && pPlayStage->GetModeType() != MT_HOUSE )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
			return;
		}
	}
	
	if( g_BattleRoomMgr.IsBattleRoom() )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return;
	}

	if( g_ShuffleRoomMgr.IsShuffleRoom() )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
		return;
	}

	if( g_LadderTeamMgr.IsLadderTeam() )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		return;
	}

	ioUserEtcItem *pUserEtcItem = g_MyInfo.GetUserEtcItem();
	if( !pUserEtcItem || !pUserEtcItem->IsCanUse( ioEtcItem::EIT_ETC_GUILD_MARK_CHANGE ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );		
		return;
	}

	ioGuildData *pGuildData = g_GuildInfo.GetGuildData( g_GuildInfo.GetGuildIndex() );
	if( !pGuildData ) return;

    g_GUIMgr.HideWnd( TIMEGATE_SEARCH );

	TCPNetwork::MouseBusy( true );
	DWORD dwNewGuildMark = max( (DWORD)g_GuildMarkMgr.GetMaxReserveMarkIndex(), pGuildData->GetGuildMark() ) + 1;
	SP2Packet kPacket( CTPK_GUILD_MARK_KEY_VALUE );
	kPacket << dwNewGuildMark;
	TCPNetwork::SendToServer( kPacket );
}

void ioApplication::CloseGuildMarkPage()
{
	if( m_pBrowserMgr )
		m_pBrowserMgr->CloseBrowserByType( BROWSE_GUILD_MARK );
}

void ioApplication::OpenFormalityEntryPage()
{
	if( m_pBrowserMgr )
		m_pBrowserMgr->OpenBrowser( m_pBrowserMgr->GetFormalityEntryURL().c_str(),
									BROWSE_FORMALITY_ENTRY,
									BROWSE_SUB_FORMALITY_ENTRY );
}

void ioApplication::OpenFillCashPage()
{
#ifndef SHIPPING
	m_pBrowserMgr->OpenBrowser("https://nxpay.nexon.com/cash/Page/Error.aspx?rcode=InValidHashValue&rmsg=[0]",
		BROWSE_FILL_CASH,
		BROWSE_SUB_FILL_CASH );

	return;
#endif

	ioChannelingNodeParent *pChannelingNode = g_ChannelingMgr.GetNode( g_MyInfo.GetChannelingType() );
	if( !pChannelingNode )
	{
		LOG.PrintTimeAndLog( 0, "Fail Cash URL : Node == NULL." );
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return;
	}

	if( !m_pBrowserMgr )
		return;

	if( pChannelingNode->IsSendFillCashURL() )
	{
		SP2Packet kPacket( CTPK_FILL_CASH_URL );
		TCPNetwork::SendToServer( kPacket );
		TCPNetwork::MouseBusy( true );
		return;
	}

	ioHashString sFullURL;
	pChannelingNode->GetFillCashURL( sFullURL, m_pBrowserMgr->GetCashURL() );

	ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
	if( pLocal )
	{
		pLocal->GetFillCashURL( sFullURL, m_pBrowserMgr->GetCashURL() );
	}

	if( pChannelingNode->IsCreateProcessFillCashWeb() )
	{
		if( Setting::FullScreen() )
			ShowWindow( g_App.GetHWnd(), SW_MINIMIZE );
		Help::ExcuteWeb( sFullURL.c_str() );
		return;
	}

	if( sFullURL.IsEmpty() )
	{
		LOG.PrintTimeAndLog( 0, "Fail Cash URL : empty." );
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return;
	}

	m_pBrowserMgr->OpenBrowser( sFullURL.c_str(), BROWSE_FILL_CASH, BROWSE_SUB_FILL_CASH, true, true );
}

void ioApplication::OpenCashEntryPage()
{
	if( m_pBrowserMgr )
		m_pBrowserMgr->OpenBrowser( m_pBrowserMgr->GetCashEntryURL().c_str(),
			                        BROWSE_CASH_ENTRY,
				                    BROWSE_SUB_CASH_ENTRY );
}

void ioApplication::NotifyDeleteFriendByWeb( const char *szFriendNick )
{
	if( !P2PNetwork::IsNetworkPlaying() )
		return;

	g_FriendManager.DeleteFriend( szFriendNick );

	SP2Packet kPacket( CTPK_DELETE_FRIEND_BY_WEB );
	kPacket << szFriendNick;
	TCPNetwork::SendToServer( kPacket );
}

void ioApplication::NotifyUserEntryRefresh( const char *szNick )
{
	if( !P2PNetwork::IsNetworkPlaying() )
		return;

	// 1�п� �ѹ��� ȣ����.
	if( m_dwEntryRefreshTime != 0 && ( FRAMEGETTIME() - m_dwEntryRefreshTime < 60000 ) )
		return;
	m_dwEntryRefreshTime = FRAMEGETTIME();

	SP2Packet kPacket( CTPK_USER_ENTRY_REFRESH );
	kPacket << szNick;
	TCPNetwork::SendToServer( kPacket );
}

void ioApplication::NotifyGuildMarkChangeComplete()
{
	if( !g_GuildInfo.IsGuildMarkChanging() ) return;

	g_GuildInfo.SetGuildMarkChanging( false );
	// �ϴ� �̹��� ���ε�� �Ϸ�Ǿ���.���� ó�� �� ���� ������ ���� �� ��ũ ���� ����
	if( !IsGuildMarkChangeOK() )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return;
	}

	ioUserEtcItem *pUserEtcItem = g_MyInfo.GetUserEtcItem();
	if( !pUserEtcItem || !pUserEtcItem->IsCanUse( ioEtcItem::EIT_ETC_GUILD_MARK_CHANGE ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );		
		return;
	}

	ioGuildData *pGuildData = g_GuildInfo.GetGuildData( g_GuildInfo.GetGuildIndex() );
	if( !pGuildData ) return;

	DWORD dwNewGuildMark = max( (DWORD)g_GuildMarkMgr.GetMaxReserveMarkIndex(), pGuildData->GetGuildMark() ) + 1;
	
	SP2Packet kPacket( CTPK_ETCITEM_USE );
	kPacket << (int)ioEtcItem::EIT_ETC_GUILD_MARK_CHANGE;
	kPacket << dwNewGuildMark;
	TCPNetwork::SendToServer( kPacket );
	TCPNetwork::MouseBusy( true );
}

void ioApplication::NotifyHideProcess( BrowsePageType eType )
{
	switch( eType )
	{
	case BROWSE_DEFAULT:
	case BROWSE_SINGLE:
	case BROWSE_FORMALITY_ENTRY:
	case BROWSE_FILL_CASH:
	case BROWSE_CASH_ENTRY:
	case BROWSE_WEB_SHOP:
		break;
	case BROWSE_GUILD_MARK:
		{
			SP2Packet kPacket( CTPK_GUILD_MARK_KEY_VALUE_DELETE );
			TCPNetwork::SendToServer( kPacket );
		}
		break;
	}
}

bool ioApplication::IsCanMannerTrialTime()
{
	if( IsMeDeveloper() || IsMeGameMaster() )  // �����ڴ� ���� ����
		return true;

	enum { DELAY_MANNER_TRIAL = 10000, }; // 10��
	if( m_dwMannerTrialTime != 0 && ( FRAMEGETTIME() - m_dwMannerTrialTime < DELAY_MANNER_TRIAL ) )
		return false;

	return true;
}

void ioApplication::SetMannerTrialTime()
{
	m_dwMannerTrialTime = FRAMEGETTIME();
}

bool ioApplication::IsWebShow() const
{
	if( m_pBrowserMgr )
		return m_pBrowserMgr->IsWebOpened();

	return false;
}

void ioApplication::InitGlobalTimer()
{
	SAFEDELETE( m_pGlobalTimer );
	m_pGlobalTimer = new ioGlobalTimer;
}

bool ioApplication::IsRightID( const char *iid )
{
	enum { MIN_ID_NUMBER = 4, };
	int iIDSize = strlen( iid );

#if !defined( _LSWC )
	if( ( iIDSize > ID_NUMBER ) || (iIDSize < MIN_ID_NUMBER) )
		return false;
#endif

	for(int i = 0;i < iIDSize;i++)
	{

#if defined( SRC_OVERSEAS ) && !defined( SRC_ID )

#if defined( MULTI_BYTE_CHECK )
		if( IsDBCSLeadByteEx( COUNTRY_CODE_PAGE, iid[i] ) )
#else
		if( false && IsDBCSLeadByteEx( COUNTRY_CODE_PAGE, iid[i] ) ) 
#endif

#else
		if( IsDBCSLeadByte( iid[i] ) ) 
#endif

		{
			i++;
			if(iIDSize <= i) // ������ ���ڰ� ���� ���ڴ�.
				return false;
		}
		//�г��� ����� ctrl + jŰ�� ������ ���� ���� �ڵ�
		else if (iid[i] >= 0 && iid[i] < 32 )
		{
			return false;
		}
	}
	return true;
}

void ioApplication::UpdateLoadedResource()
{
	if( m_pMeshMgr )
		m_pMeshMgr->UpdateNotifiedResources();

	if( m_pSoundMgr )
		m_pSoundMgr->UpdateNotifiedResources( m_pSceneMgr );
}

void ioApplication::Reload()
{
	g_AIDB.BuildAIDatabase();
	g_MapEventSceneMgr.LoadMapEventScene();
	g_NpcMgr.LoadFieldNPC();
	SetNextStage( ioGameStage::GS_PLAY, "" );
	m_bReloadINILists = true;
}

void ioApplication::MainLoop()
{

#ifndef _DEBUG
	__try
#endif
	{ 
		bool bCheckChar = Help::CheckEnableTimeGapUser();
		DWORD dwLoopStart = timeGetTime();

		DWORD dwStartTime = 0;

		DWORD dwGuiUpdate = 0;
		DWORD dwUpdateStage = 0;
		DWORD dwUpdateSceneGraph = 0;
		DWORD dwRenderLoop = 0;

#ifndef SHIPPING
		if( !m_bNetworkPlay )
		{
			if( m_pDxInput && m_pDxInput->IsKeyPress( KEY_NUMLOCK ) )
			{
				Reload();
			}
		}
#endif
		if( m_dwLoadingEndTime > 0 && m_dwLoadingEndTime > REALGETTIME() )
		{
			RenderFirstLoading();
			return;
		}
		m_dwLoadingEndTime = 0;
		if( m_bFirstLoading )
		{
			g_ExSoundMgr.PlaySound( ExSound::EST_LOGIN );
		}
		m_bFirstLoading    = false;

		ErrorReport::SetPosition( 1000, 0 );

		if( m_bAppActive )
		{
			m_pRenderSystem->CheckLostDevice();
		}
		CheckForegroundForSound();
		ErrorReport::SetPosition( 1000, 1 );

		ChangeStage();
		ErrorReport::SetPosition( 1000, 2 );

#ifdef STEAM_ATTACH
		g_ioSteamClient.RunCallbacks();
#endif

		DWORD dwCurTotalTime = m_pFrameTimer->Frame();
		m_pTickTimer->UpdateTick();
		ErrorReport::SetPosition( 1000, 4 );

		// ping test
#ifdef UDP_PING_CHECK
		ProcessUDPInfo();
#endif
		UpdateInput();
		ErrorReport::SetPosition( 1000, 5 );

		if( bCheckChar )
			dwStartTime = timeGetTime();

		m_Mouse.ProcessMouse();

		if( m_pGUIMgr )
			m_pGUIMgr->UpdateGUI( m_Mouse );
		ErrorReport::SetPosition( 1000, 6 );

		if( bCheckChar )
			dwGuiUpdate = timeGetTime() - dwStartTime;

		UpdateReserveLogOut();
		ErrorReport::SetPosition( 1000, 7 );

		if( bCheckChar )
			dwStartTime = timeGetTime();
		
		ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
		if( pPlayStage )
		{
			pPlayStage->FindCameraDistCharCheck( m_pCamera );
		}
		ErrorReport::SetPosition( 1000, 11 );
		
		UpdateStage( dwCurTotalTime );
		ErrorReport::SetPosition( 1000, 8 );

		if( bCheckChar )
			dwUpdateStage = timeGetTime() - dwStartTime;

		UpdateLoadedResource();
		ErrorReport::SetPosition( 1000, 9 );

		g_ThreadTaskMgr.NotifyDoneTasks();
		ErrorReport::SetPosition( 1000, 10 );

		//
		bool bNeedResetVolume = false;
		if( m_dwResetSoundTime > 0 && m_dwResetSoundTime > FRAMEGETTIME() )
		{
			bNeedResetVolume = true;
		}

		if( bCheckChar )
			dwStartTime = timeGetTime();

		if( m_pSceneMgr )
			m_pSceneMgr->UpdateSceneGraph( m_pCamera, NULL, bNeedResetVolume );

		if( bCheckChar )
			dwUpdateSceneGraph = timeGetTime() - dwStartTime;

		#ifdef ANTIHACK
		#if defined(SRC_SEA) || defined (SRC_ID) 
		UpdateBulletInfo();
		#endif
		#endif
#ifdef _ITEST
	#ifdef ANTIHACK
			if( g_Input.IsKeyDown( KEY_NUM0 ) )
			{
				UseReSkill();
			}
			if( g_Input.IsKeyDown( KEY_NUM1 ) )
			{
				ioPlayStage* pPlayStage = ToPlayStage( m_pGameStage );
				if( pPlayStage )
				{
					ioBaseChar* pBase = NULL;
					if( pPlayStage )
						pBase = pPlayStage->GetOwnerChar();
					if( pBase )
					{
						if( !g_bTestAntiHack )
						{
							g_bTestAntiHack = true;
							pBase->SetProtectionMode( PM_GM, -1 );
						}
					}
				}
			}
			if( g_Input.IsKeyDown( KEY_NUM2 ) )
			{
				if( g_bTestAntiHack )
					g_bTestAntiHack = false;

				ioPlayStage* pPlayStage = ToPlayStage( m_pGameStage );
				if( pPlayStage )
				{
					ioBaseChar* pBase = NULL;
					if( pPlayStage )
						pBase = pPlayStage->GetOwnerChar();
					if( pBase )
					{
						pBase->SetProtectionMode( PM_GM, 0 );
					}
				}
			}
	#endif
#endif

		ErrorReport::SetPosition( 1000, 12 );

		if( m_bAppActive )
		{
			if( bCheckChar )
				dwStartTime = timeGetTime();

			RenderLoop();

			if( bCheckChar )
				dwRenderLoop = timeGetTime() - dwStartTime;
		}

		ErrorReport::SetPosition( 1000, 13 );
		if( pPlayStage )
			pPlayStage->ClearCameraDistCharCheck();

		ErrorReport::SetPosition( 1000, 14 );

		if( m_pSoundMgr )
			m_pSoundMgr->UpdateStream();
		ErrorReport::SetPosition( 1000, 15 );
		
		VoiceChatLoop();
		ErrorReport::SetPosition( 1000, 16 );

		if( m_pCustomSoundMgr )
			m_pCustomSoundMgr->CustomSoundLoop();
		ErrorReport::SetPosition( 1000, 17 );

		if( m_bAppActive )
		{
			ErrorReport::SetPosition( 1000, 18 );

			if( m_pRenderSystem )
				m_pRenderSystem->Present();
		}

		ErrorReport::SetPosition( 1000, 19 );

		bool bLSFocus = true;
		HWND hCurFocus = ::GetFocus();
		if( hCurFocus != m_hWnd )
			bLSFocus = false;

		if( m_pBrowserMgr  )
		{
			m_pBrowserMgr->Process();

			ErrorReport::SetPosition( 1000, 20 );

			if( !bLSFocus )
			{
				if( m_pBrowserMgr->HasFocusInBrowser( hCurFocus ) )
					bLSFocus = true;
			}
		}

		ErrorReport::SetPosition( 1000, 21 );

		ClearInputState();
		ErrorReport::SetPosition( 1000, 22 );

		P2PNetworkProcess();
		ErrorReport::SetPosition( 1000, 23 );

		ProcessCandidateWnd();
		ErrorReport::SetPosition( 1000, 24 );

		if( m_pFileWriteClient )
		{
			m_pFileWriteClient->Process();
//			m_pFileWriteClient->ProcessTemp();   skin ���� �����κ��� ���ε��ϴ� ��ƾ �ؿܴ� ����.
		}

		ErrorReport::SetPosition( 1000, 25 );

		ResourceFailedCheck();
		ErrorReport::SetPosition( 1000, 26 );

//		
//		ProcessTickCheck();
		ProcessTestPacketCnt();
		ErrorReport::SetPosition( 1000, 27 );

		if( pPlayStage )
			m_pMissionManager->MissionProcess( pPlayStage->GetModeType() );

		if( m_pInputBox )
		{
			m_pInputBox->Process();
			if( ioEdit::m_pKeyFocusEdit )
			{
				if( !m_bProcessCandidate )
				{
					m_bProcessCandidate = true;
				}
			}
			else
			{
				if( m_bProcessCandidate )
				{
					m_bProcessCandidate = false;
				}
			}
			if( !bLSFocus && !m_pInputBox->HasFocus( hCurFocus ) )
				::SetFocus( m_hWnd );
		}

		ErrorReport::SetPosition( 1000, 28 );


		g_MyInfo.ProcessAnnounceGameAddiction();
		g_MyInfo.ProcessSelectShutDownAnnouncer();
		ErrorReport::SetPosition( 1000, 30 );

		g_EffectFactory.ProcerssTimeTempleteCheck();
		
		ProcessMemoryUsageLOG();

		if( bCheckChar )
		{
			DWORD dwLoopGap = timeGetTime() - dwLoopStart;

			if( dwLoopGap > 20 )
			{
				LOG.PrintTimeAndLog( 0, "FC - MainLoop: G:%d, St:%d, Sg:%d, R:%d, T:%d, C:%d", dwGuiUpdate,
																						 dwUpdateStage,
																						 dwUpdateSceneGraph,
																						 dwRenderLoop,
																						 dwLoopGap,
																						 FRAMEGETTIME() );
			}
		}

#if defined( USE_GA )
		// GA Alive check
		if( GetGAStart() == true )
		{
			DWORD dwGATime = REALGETTIME();
			if( m_dwGAAliveTime + g_HttpMng.GetAliveTime() < dwGATime )
			{				
				g_HttpMng.GA_AliveCheck();

				m_dwGAAliveTime = dwGATime;
			}

			// ���������� Ŭ���̾�Ʈ���� Mouse UnBusy �� �κ� üũ
			if( m_bFirstUnMouseBusy == true )
			{
				if( m_Mouse.GetCompulsionUnMouseBusy() == true )
				{
					m_bFirstUnMouseBusy = false;

					// el(EventLabel) �� ������ ��Ŷ ���̵� �ִ´�
					char chLabel[32] = {0,};

					if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
						sprintf_e( chLabel, "%d", (int)m_Mouse.GetPacketID() );
					else
						SafeSprintf( chLabel, sizeof(chLabel), "%1", (int)m_Mouse.GetPacketID() );

					// GAME_MOUSE_UNBUSY
					g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
						, "Game"
						, "MouseUnBusy"
						, chLabel
						, 1
						, "%2FGAME%2FMOUSE%2FUNBUSY"
						, 2 );

					g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FGAME%2FMOUSE%2FUNBUSY", 10 );

					m_Mouse.SetCompulsionUnMouseBusy( false );
				}
			}			
		}
#endif

	}
#ifndef _DEBUG
	__except( ExceptCallBack( GetExceptionInformation() ) )
	{
		if( m_pQuestManager )
		{		
			m_pQuestManager->ProcessChangeStage();  // �����ϸ� ������Ʈ
		}

		m_bProgramExit = true;
	}
#endif
}

void ioApplication::P2PNetworkProcess()
{
	if( !m_pNetworkTable ) return;

	ErrorReport::SetPosition( 1301, 1 );
	m_pNetworkTable->HoleSendProcess();
	ErrorReport::SetPosition( 1301, 2 );
	m_pNetworkTable->HoleRecvProcess();
}

void ioApplication::ResourceFailedCheck()
{
#ifdef SHIPPING
	if( !m_bResourceLoadFailed )
	{
		if( !g_ResourceLoader.GetLastFailedFileName().IsEmpty() )
		{
			if( g_ResourceLoader.GetLastFailedFileName() == "config/sp2_training_mode-1_map1.ini" ||
				g_ResourceLoader.GetLastFailedFileName() == "(null)"                              ||
				g_ResourceLoader.GetLastFailedFileName() == "Shader/StaticToonShadeGlow/Group.txt"||
				g_ResourceLoader.GetLastFailedFileName() == "Resource/Model\\" )
			{
				return;
			}


			// Ư�� Ȯ���ڴ� ����
			char szExt[MAX_PATH] = "";
			_splitpath( g_ResourceLoader.GetLastFailedFileName().c_str(), NULL, NULL, NULL, szExt );
			if( strcmp( szExt, ".wav" ) == 0 ||
				strcmp( szExt, ".dds" ) == 0 ||
				strcmp( szExt, ".ogg" ) == 0 )
			{
				LOG.PrintTimeAndLog( 0, "None File : %s", g_ResourceLoader.GetLastFailedFileName().c_str() );
				g_ResourceLoader.ClearLastFailedFileName();
				return;
			}

			SafeSprintf( g_szErrorLog, sizeof(g_szErrorLog), STR(1), g_ResourceLoader.GetLastFailedFileName().c_str() );

			char szResourceLog[MAX_PATH * 2] = "";
			sprintf( szResourceLog, "���ҽ����� : %s/%s : %s", g_MyInfo.GetPrivateID().c_str(), g_MyInfo.GetPublicID().c_str(), g_szErrorLog ); //Except Extracting Hangeul			
			SP2Packet kPacket( LUPK_LOG );
			kPacket << "CheckError";
			kPacket << szResourceLog;
			kPacket << 326;  // ������ȣ
			kPacket << true; // write db
			P2PNetwork::SendLog( kPacket );

			m_bResourceLoadFailed = true;        // �ѹ� �����ϸ� ��� ������ �ʴ´�.
#ifdef __DEV_QA__
			m_bProgramExit = true;				 // Ŭ���̾�Ʈ ����
#endif
			g_bHappenError = true;				 // Ŭ���̾�Ʈ ������ �� �α� �޽��� �ڽ� ����
			g_iErrorNumber = 326;
		}
	}
#endif
}

void ioApplication::UpdateInput()
{
	if( m_pDxInput )
	{
		m_pDxInput->Update();
	}
}

void ioApplication::UpdateStage( DWORD dwTotalTime )
{
	if( !m_pGameStage )
	{
		//LOG.PrintTimeAndLog(0, "%s - m_pGameStage == NULL!!!", __FUNCTION__ );
		return;
	}

#ifndef _SHIPPING
	if( SingleSettingWnd::IsPause() )
		return;
#endif

	ErrorReport::SetPosition( 1000, 200 );

	m_pGameStage->UpdatePreLoop( m_pCamera );
	ErrorReport::SetPosition( 1000, 201 );

	float fMinGap = 20.0f;//33.3f;
	float fBackUpCurFrameGap = g_FrameTimer.GetCurFrameGap();
	float fRemainGap = fBackUpCurFrameGap;

	// ��� ������ �ε����� ������ �������⶧���� 10�������� ���� ������Ʈ�� ���� �ʰ�..
	if( m_iFrameSkipCount < 10 )
	{
		fMinGap = fRemainGap;
		m_iFrameSkipCount++;
	}
	else
	{
		float fLimitGap = fRemainGap / 5.0f;
		if( fLimitGap > fMinGap )
		{
			fMinGap = fLimitGap + 0.1f;
		}
	}

	DWORD dwCurFrameTime = dwTotalTime - (DWORD)fRemainGap;

	//������Ʈ ������ ���� ���κ���/��庻��/������ UpdateLoop()�� �ѹ��� ������ ��
	bool bForceOnceUpdateLoop = false;
	ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
	if( pPlayStage )
	{	
		if( pPlayStage->GetModeType() == MT_TRAINING || pPlayStage->GetModeType() == MT_HOUSE )
			bForceOnceUpdateLoop = true;
	}

	if( IsFPSRender() || bForceOnceUpdateLoop )
	{
		m_pFrameTimer->SetCurFrameGapForce( fRemainGap );
		m_pFrameTimer->SetLoopGetTime( dwTotalTime );
		fRemainGap = -1.0f;	// for break

		m_pGameStage->UpdateLoop( m_pCamera );
	}
	else
	{
		do
		{
			if( fRemainGap > fMinGap )
			{
				m_pFrameTimer->SetCurFrameGapForce( fMinGap );
				fRemainGap -= fMinGap;

				dwCurFrameTime += (DWORD)fMinGap;
				m_pFrameTimer->SetLoopGetTime( dwCurFrameTime );
			}
			else
			{
				m_pFrameTimer->SetCurFrameGapForce( fRemainGap );
				m_pFrameTimer->SetLoopGetTime( dwTotalTime );
				fRemainGap = -1.0f;	// for break
			}

			m_pGameStage->UpdateLoop( m_pCamera );

		}while( fRemainGap > 0.0f );
	}

	ErrorReport::SetPosition( 1000, 202 );

	m_pFrameTimer->SetCurFrameGapForce( fBackUpCurFrameGap );
	m_pFrameTimer->SetLoopGetTime( dwTotalTime );
	ErrorReport::SetPosition( 1000, 203 );
	
	m_pGameStage->UpdateAfterLoop( m_pCamera );	

	if( m_pBlockEventAgency )
		m_pBlockEventAgency->ProcessEvent();

	ErrorReport::SetPosition( 1000, 204 );
}

void ioApplication::ClearInputState()
{
	if( m_pInputBox )
		m_pInputBox->ClearKeyState();

	m_Mouse.ClearCheckOnlyOnceStatus();

	m_ptMouseMoveAmt.x = 0;
	m_ptMouseMoveAmt.y = 0;
}

void ioApplication::VoiceChatLoop()
{
	if( !m_pNetworkTable )	return;

	g_VoiceChat.CheckPlayStop();

	if(!g_VoiceChat.IsEmptyData())
	{
		static VOICEDATA vd;
		g_VoiceChat.PopData(vd);

		SendVoiceChat(vd);

		// �ڽſ� ��Ҹ� ���
		g_VoiceChat.PushData(g_MyInfo.GetPublicID().c_str(), (const VOICEDATA&)vd);
	}

	g_VoiceOffListManager.ProcessDelete();
}

void ioApplication::SendVoiceChat( VOICEDATA &rVD )
{
	if( !m_pNetworkTable )	return;
	if( g_MyInfo.GetPublicID().IsEmpty() )	return;

	ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
	if( !pPlayStage ) return;

	int iOwnerTeam = g_BattleRoomMgr.GetBattleUserTeam( g_MyInfo.GetPublicID() );

	SP2Packet kPacket( CUPK_VOICE_DATA );
	kPacket << g_MyInfo.GetPublicID();
	kPacket << rVD;

	ioUserNetworkTable::NetworkInfoIter iter = m_pNetworkTable->GetNetworkInfoIter();
	while( iter.HasMoreElements() )
	{
		UserNetworkInfo *pInfo = iter.Next();
		if( !pInfo->HasStyle( P2P_STYLE_VOICE ) ) continue;
		
		// check
		if( pPlayStage->GetVoiceChatType() == ioPlayStage::VCT_TEAM &&
			g_BattleRoomMgr.IsBattleRoom() &&
			g_BattleRoomMgr.IsBattleModePlaying() && 
			pPlayStage->GetModeType() != MT_MONSTER_SURVIVAL ) // �ذ񿵿��� �������� ������ ��ȭ ����
		{
			if( iOwnerTeam != g_BattleRoomMgr.GetBattleUserTeam( pInfo->m_Name ) ||
				pPlayStage->GetModeType() == MT_SURVIVAL || pPlayStage->GetModeType() == MT_FIGHT_CLUB || pPlayStage->GetModeType() == MT_SHUFFLE_BONUS )
				continue;
		}

		if( g_VoiceChat.IsRefuse( ioHashStringVC( pInfo->m_Name.c_str() ) ) )
			continue;

		// send		
		SendP2PPacket( pInfo->m_addrPublic.m_IP, pInfo->m_addrPublic.m_Port, kPacket );
	}
}

void ioApplication::ResetVoiceChatWithGUI()
{
	if( !m_pNetworkTable )	return;

	// set data on the loading
	ioUserNetworkTable::NetworkInfoIter iter = m_pNetworkTable->GetNetworkInfoIter();
	while( iter.HasMoreElements() )
	{
		UserNetworkInfo *pInfo = iter.Next();
		if( !pInfo ) continue;
		if( !pInfo->HasStyle( P2P_STYLE_VOICE ) ) continue;

		ioHashStringVC szTemp = pInfo->m_Name.c_str();
		if( !g_VoiceChat.IsSoundPlayer( szTemp ) ) continue;
		
		// gui
		VoiceChatWnd *pWnd = dynamic_cast<VoiceChatWnd*>( g_GUIMgr.FindWnd(VOICE_CHAT_WND) );
		if( !pWnd ) continue;
	
		pWnd->SetVoiceOn(  pInfo->m_Name );
		if(!Setting::UseOtherVoiceChat() || g_VoiceDisableListManager.IsVoiceDisable(  pInfo->m_Name ))
			pWnd->SetVoiceDisable(  pInfo->m_Name );	
	}

	// my voice
	if( !g_MyInfo.GetPublicID().IsEmpty() )
	{
		ioHashStringVC szTemp = g_MyInfo.GetPublicID().c_str();
		g_VoiceChat.AddSoundPlayer( szTemp, true);
	}

	g_VoiceChat.StopCapture();

	// volume
	if(Setting::UseOtherVoiceChat())
		g_VoiceChat.SetAllVolume(Help::GetDSVolume(Setting::GetVoiceVolume(), Help::VT_NONE ));
	else
		g_VoiceChat.SetAllVolume(DSBVOLUME_MIN);
}

void ioApplication::RenderFirstLoading( int iCurRate /* = 10 */, int iTotalRate /* = 10  */ )
{
	if( !m_bFirstLoading ) return;
	if( !m_pLoadingRender ) return;

	m_pLoadingRender->RenderFirstLoading( m_pRenderSystem, m_pUIRenderer, iCurRate, iTotalRate, m_dwLoadingLogoTime, m_dwLoadingRegulation );
}

void ioApplication::RenderLoading( int iModeType, int iModeSubNum, int iModeMapNum, bool bCampBattle, bool bCampHeroBattle, PlazaType ePlazaType, int iCurRate, int iTotalRate, ExLoadingType eLoadingType )
{
	if( !m_pLoadingRender )
	{
		m_pLoadingRender = new ioLoadingRender;
		if( m_pLoadingRender )
		{
			m_pLoadingRender->LoadModeINIPATH();
		}
	}

	if( m_bFirstLoading )
	{
		if( iTotalRate > 0 )
		{
			RenderFirstLoading( iCurRate, iTotalRate );
			if( iCurRate >= iTotalRate )
			{
				while( true )
				{
					if( m_pLoadingRender->IsCloseFirstLoading() )
					{
						/*
						if( Help::CheckEnableTimeGapUser() )
						{
							LOG.PrintTimeAndLog( 0, "Test - Check ChangePresentInterval" );
							m_pRenderSystem->SetPresentationInterval();
						}
						*/
						break;
					}
					else
					{
						RenderFirstLoading( iCurRate, iTotalRate );
					}
				}
			}
		}
	}
	else
	{		
		switch( eLoadingType )
		{
		case LOADING_FIRST:
			{
				RenderFirstLoading( iCurRate, iTotalRate );
			}
			break;
		case LOADING_SEARCH_MODE:
			{
				m_pRenderSystem->ClearBack( D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xff000000 );
				m_pLoadingRender->RenderModeLoading( MT_NONE, 0, 0, false, false, PT_BATTLE, MRT_FIND_MODE, true );
			}
			break;
		case LOADING_HEADQUARTER:
			{
				m_pRenderSystem->ClearBack( D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xff000000 );
				m_pLoadingRender->RenderModeLoading( MT_HEADQUARTERS, 1, 1, false, false, PT_BATTLE, ioMyRoomMode::GetMyRoomType() );
			}
			break;
		case LOADING_HOUSE:
			{
				m_pRenderSystem->ClearBack( D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xff000000 );
				m_pLoadingRender->RenderModeLoading( MT_HOUSE, 1, 1, false, false, PT_BATTLE, ioMyRoomMode::GetMyRoomType() );
			}
			break;
		default:
			{
				m_pRenderSystem->ClearBack( D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xff000000 );
				m_pLoadingRender->RenderModeLoading( iModeType, iModeSubNum, iModeMapNum, bCampBattle, bCampHeroBattle, ePlazaType, ioMyRoomMode::GetMyRoomType() );
			}
			break;
		}

		m_pRenderSystem->SetFogEnable( false );
		m_pUIRenderer->Render( m_pRenderSystem );

		m_pRenderSystem->Present();
		m_pLoadingRender->ClearResource();
	}
}
void ioApplication::EraseManualResource()
{
	ErrorReport::SetEnginePos( 1000, 700 );

//  ����Ʈ�� ���������� ����ǵ� �۷ι��� �����ϰ� �ִ� ����Ʈ�� �����Ѵ�.
//	if( m_pEffectFac->GetCurEffectCount() > 0 )
//	{
//		LOG.PrintTimeAndLog( 0, "ioApplication::EraseManualResource - %d Effect Not Deleted",
//								m_pEffectFac->GetCurEffectCount() );
//	}

	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof (statex);
	GlobalMemoryStatusEx (&statex);

	if( (statex.ullAvailVirtual/1048576) < 700 ) // ���� �޸� 700M ����
	{
		m_pAnimationMgr->DestroyNoneRefResource( EP_MANUAL );
		m_pAnimationEventManager->DestroyNoneRefResource( EP_MANUAL );
		m_pSoundMgr->DestoryNoneRefResources();
	}

	if( Setting::GetSystemMemory() < 600 )	// 512M ����
	{
		m_pAnimationMgr->DestroyNoneRefResource( EP_MANUAL );
		m_pAnimationEventManager->DestroyNoneRefResource( EP_MANUAL );
		m_pSoundMgr->DestoryNoneRefResources();
	}

	ErrorReport::SetEnginePos( 1000, 701 );

	if( m_pMeshMgr )
		m_pMeshMgr->DestroyNoneRefResource( EP_MANUAL );

	ErrorReport::SetEnginePos( 1000, 702 );

	if( m_pTextureMgr )
		m_pTextureMgr->DestroyNoneRefResource( EP_MANUAL );

	m_iFrameSkipCount = 0;

	ErrorReport::SetEnginePos( 1000, 703 );
}

void ioApplication::EraseManualResourceByUserRemove()
{
	if( m_pAnimationMgr )
	{
		m_pAnimationMgr->DestroyNoneRefResource( EP_MANUAL );
		m_pAnimationEventManager->DestroyNoneRefResource( EP_MANUAL );
	}

	if( m_pSoundMgr )
		m_pSoundMgr->DestoryNoneRefResources();

	if( m_pMeshMgr )
		m_pMeshMgr->DestroyNoneRefResource( EP_MANUAL );

	if( m_pTextureMgr )
		m_pTextureMgr->DestroyNoneRefResource( EP_MANUAL );

	ProcessMemoryUsageLOG( ioApplication::RT_DELETE );
}

bool ioApplication::ApplyOptionChange( const SettingOption &rkNewOption )
{
	SettingOption kOldOption = Setting::GetSettingOption();

	ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
	if( pPlayStage )
	{
		if( kOldOption.m_bShadow != rkNewOption.m_bShadow )
		{
			g_BlockManager.SetShadowMapEnable( rkNewOption.m_bShadow );
			m_pSceneMgr->SetShadowMapEnable( rkNewOption.m_bShadow );			
		}

		if( kOldOption.m_bUseMapEdge != rkNewOption.m_bUseMapEdge )
		{
			m_pSceneMgr->EnableMapEdge( rkNewOption.m_bUseMapEdge );
			g_BlockManager.EnableMapEdge( rkNewOption.m_bUseMapEdge );
		}

		if( kOldOption.m_bShowMapEffect != rkNewOption.m_bShowMapEffect )
		{
			pPlayStage->EnableMapDefaultEffect( rkNewOption.m_bShowMapEffect );
		}

		if( kOldOption.m_MapObjectQuality != rkNewOption.m_MapObjectQuality )
		{
			pPlayStage->SetMapDefaultObjectQuality( rkNewOption.m_MapObjectQuality );
		}
	}

	Setting::SetUseShadow( rkNewOption.m_bShadow );
	Setting::SetShowMapEffect( rkNewOption.m_bShowMapEffect );
	Setting::SetUseMapEdge( rkNewOption.m_bUseMapEdge );
	Setting::SetSpecAutoCheck( rkNewOption.m_bSpecAutoCheck );
	Setting::SetMapObjectQuality( rkNewOption.m_MapObjectQuality );

	if( kOldOption.m_bGlow != rkNewOption.m_bGlow )
	{
		m_pSceneMgr->EnableGlow( rkNewOption.m_bGlow );
		Setting::SetUseGlow( rkNewOption.m_bGlow );
	}

	if( kOldOption.m_FilterType != rkNewOption.m_FilterType )
	{
		m_pSceneMgr->EnableFilter( rkNewOption.m_FilterType );
		Setting::SetFilterType( rkNewOption.m_FilterType );
	}

	if( kOldOption.m_OverayType != rkNewOption.m_OverayType )
	{
		m_pSceneMgr->EnableOveray( rkNewOption.m_OverayType );
		Setting::SetOverayType( rkNewOption.m_OverayType );
	}

	// etc
	Setting::setAutoRejectParty( rkNewOption.m_bAutoRejctParty );
	Setting::setAutoRejectCamp( rkNewOption.m_bAutoRejctCamp );
	Setting::SetScreenShotSavePath( rkNewOption.m_szScreenShotSavePath.c_str() );
	Setting::SetScreenShotFormatJPG( rkNewOption.m_bScreenShotFormatJPG );
	Setting::SetBlockUsersChat( rkNewOption.m_bBlockUsersChat );
	Setting::SetChatBubbleAlarm( rkNewOption.m_bChatBubbleAlarm );
	Setting::SetGuildChatAlarm( rkNewOption.m_bGuildChatAlarm );
	Setting::SetBlockUsersCustomSound( rkNewOption.m_bBlockUsersCustomSound );

	Setting::SetSlangFilterChange( rkNewOption.m_bSlangFilterChange );
	Setting::SetSlangFilterOther( rkNewOption.m_bSlangFilterOther );

	Setting::SetHideSkillBuffEmoticon( rkNewOption.m_bHideSkillBuffEmoticon );
	Setting::SetHideMiniSoldierSelectSlot( rkNewOption.m_bHideMiniSoldierSelectSlot );
	Setting::SetUseJoyPad( rkNewOption.m_bUseJoyPad );

	Setting::SetNoUIModeTeamCircleShow( rkNewOption.m_bNoUIModeTeamCircleShow );
	Setting::SetNoUIModeIDShow( rkNewOption.m_bNoUIModeIDShow );
	Setting::SetNoUIModeHPShow( rkNewOption.m_bNoUIModeHPShow );
	Setting::SetNoUIModeChatBubbleShow( rkNewOption.m_bNoUIModeChatBubbleShow );
	Setting::SetMovieModeUIHide( rkNewOption.m_bMovieModeUIHide );

	Setting::SetMovieSizeType( rkNewOption.m_iMovieSizeType );
	Setting::SetMovieFrameType( rkNewOption.m_iMovieFrameType );
	Setting::SetMovieQualityType( rkNewOption.m_iMovieQualityType );
	Setting::SetMovieSoundType( rkNewOption.m_iMovieSoundType );
	Setting::SetMovieCodec( rkNewOption.m_iMovieCodec );

	Setting::SetChatBackType( rkNewOption.m_iChatBackType );
	Setting::SetChatNormalChat( rkNewOption.m_bNormalChat );
	Setting::SetChatSystemChat( rkNewOption.m_bSystemChat );
	Setting::SetChatWholeChat( rkNewOption.m_bWholeChat );

	Setting::SetSlotItem( rkNewOption.m_dwSlot);
	Setting::SetCostumeShowChange( rkNewOption.m_bCostumeShowChange );
	Setting::SetPlayKillSound( rkNewOption.m_bPlayKillSound );
	Setting::SetPlayComboSound( rkNewOption.m_bPlayComboSound );
	Setting::SetPlayExtendSound( rkNewOption.m_bPlayExtendSound );
	Setting::SetBattleCutinShow( rkNewOption.m_bBattleCutinShow );
	
	// sound
	if( kOldOption.m_bUseBGM != rkNewOption.m_bUseBGM )
	{
		if( rkNewOption.m_bUseBGM )
		{
			g_SoundMgr.SetCanPlayStream(true);
			g_SoundMgr.StopStream();
			g_SoundMgr.SetVolumeStream( Help::GetDSVolume(Setting::GetBGMVolume(), Help::VT_STREAM ) );
			g_SoundMgr.PlayStream();
		}
		else
		{
			g_SoundMgr.SetCanPlayStream(false);
			g_SoundMgr.SetVolumeStream( DSBVOLUME_MIN );
			g_SoundMgr.StopStream();
		}

		Setting::SetUseBGM( rkNewOption.m_bUseBGM );
	}

	if( kOldOption.m_bUseEffectSound != rkNewOption.m_bUseEffectSound )
	{
		if( rkNewOption.m_bUseEffectSound )
		{
			g_SoundMgr.SetCanPlaySound(true);
			g_SoundMgr.SetVolumeNormal( Help::GetDSVolume(Setting::GetEffectSoundVolume(), Help::VT_NORMAL) );
			g_SoundMgr.SetVolume3D ( Help::GetDSVolume(Setting::GetEffectSoundVolume(), Help::VT_3D ) );
		}
		else
		{
			g_SoundMgr.SetCanPlaySound(false);
			g_SoundMgr.SetVolumeNormal( DSBVOLUME_MIN );
			g_SoundMgr.SetVolume3D( DSBVOLUME_MIN );
			g_SoundMgr.AllSoundStop();
		}

		Setting::SetUseEffectSound( rkNewOption.m_bUseEffectSound ); 
	}

	if( kOldOption.m_bUseOtherVoiceChat != rkNewOption.m_bUseOtherVoiceChat )
	{
		VoiceChatWnd *pWnd = dynamic_cast<VoiceChatWnd*>( g_GUIMgr.FindWnd(VOICE_CHAT_WND) );
		if( pWnd )
		{
			if( rkNewOption.m_bUseOtherVoiceChat )
				pWnd->AllOtherVoiceEnable();
			else
				pWnd->AllOtherVoiceDisable();
		}

		Setting::SetUseOtherVoiceChat( rkNewOption.m_bUseOtherVoiceChat );
	}

	if( kOldOption.m_bUseMIC != rkNewOption.m_bUseMIC )
	{
		if( kOldOption.m_bUseVoiceChat == rkNewOption.m_bUseVoiceChat )
		{
			VoiceChatWnd *pWnd = dynamic_cast<VoiceChatWnd*>( g_GUIMgr.FindWnd(VOICE_CHAT_WND) );
			if(pWnd)
			{
				if( rkNewOption.m_bUseMIC )
					pWnd->SetVoiceEnable( g_MyInfo.GetPublicID() );
				else
					pWnd->SetVoiceDisable( g_MyInfo.GetPublicID() );
			}
		}

		Setting::SetUseMIC( rkNewOption.m_bUseMIC );
	}
	
	if( kOldOption.m_bUseVoiceChat != rkNewOption.m_bUseVoiceChat )
	{
		VoiceChatWnd *pWnd = dynamic_cast<VoiceChatWnd*>( g_GUIMgr.FindWnd(VOICE_CHAT_WND) );
		if(pWnd)
		{
			if(rkNewOption.m_bUseVoiceChat)
				pWnd->SetVoiceOn( g_MyInfo.GetPublicID() );
			else
				pWnd->SetVoiceOff( g_MyInfo.GetPublicID() );
		}

		Setting::SetUseVoiceChat( rkNewOption.m_bUseVoiceChat );
	}

	if( kOldOption.m_bUseCustomSound != rkNewOption.m_bUseCustomSound )
	{
		if( rkNewOption.m_bUseCustomSound )
		{
			g_SoundMgr.SetCanPlayPiece(true);
			g_SoundMgr.SetVolumePiece( Help::GetDSVolume(Setting::GetCustomSoundVolume(), Help::VT_NONE ) );
			
		}
		else
		{
			g_SoundMgr.SetCanPlayPiece(false);
			g_SoundMgr.SetVolumePiece( DSBVOLUME_MIN );
			g_SoundMgr.AllSoundStop();
		}

		Setting::SetUseCustomSound( rkNewOption.m_bUseCustomSound ); 
	}
	//

	if( kOldOption.m_bSoftSkinning != rkNewOption.m_bSoftSkinning )
	{
		m_pSceneMgr->SetSoftwareVertexBlending( rkNewOption.m_bSoftSkinning );
		Setting::SetSoftSknning( rkNewOption.m_bSoftSkinning );
	}

	if( kOldOption.m_bTrailGlow != rkNewOption.m_bTrailGlow )
	{
		ioMeshTrailer::SetUseGlowTrail( rkNewOption.m_bTrailGlow );
		Setting::SetTrailGlow( rkNewOption.m_bTrailGlow );
		m_pSceneMgr->EnableLocalGlow( rkNewOption.m_bTrailGlow );
	}

	if( kOldOption.m_bUseObjectEdge != rkNewOption.m_bUseObjectEdge )
	{
		CloneScriptMod *pLoadedMod = (CloneScriptMod*)m_pMaterialMgr->GetCloneModifier();
		if( pLoadedMod )
		{
			pLoadedMod->EnableOutLine( rkNewOption.m_bUseObjectEdge );
		}

		EnableOutLineMod *pOutLIne = new EnableOutLineMod( rkNewOption.m_bUseObjectEdge );
		ioEntityGrpMaterialModifier *pMod = new ApplyAllEntityGroup( pOutLIne );
		m_pSceneMgr->ModifyEntityGroupMaterial( pMod );

		delete pMod;

		Setting::SetUseObjectEdge( rkNewOption.m_bUseObjectEdge );
	}

	if( kOldOption.m_LightQuality != rkNewOption.m_LightQuality )
	{
//		if( ( ( kOldOption.m_LightQuality == LQ_HIGH || kOldOption.m_LightQuality == LQ_MIDDLE ) && rkNewOption.m_LightQuality == LQ_LOW ) ||
//			( ( rkNewOption.m_LightQuality == LQ_HIGH || rkNewOption.m_LightQuality == LQ_MIDDLE ) && kOldOption.m_LightQuality == LQ_LOW ) )
//		{
//			Setting::SetLightQuality( rkNewOption.m_LightQuality );
//
//			g_GUIMgr.SetMsgBox( MB_OK, NULL, "����Ʈ �ɼ��� �����ӽÿ� �ݿ��˴ϴ�." );
//		}
//		else
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
			LoadingScriptMod *pScriptMod = (LoadingScriptMod*)m_pMaterialMgr->GetScriptModifier();
			if( pScriptMod )
			{
				pScriptMod->SetLightQuality( rkNewOption.m_LightQuality );
			}

//			LightQualityMod *pQualityMod = new LightQualityMod( rkNewOption.m_LightQuality );
//			m_pMaterialMgr->ModifyLoadedScript( pQualityMod );
//			pQualityMod->SetEntityModify( true );

//			ioEntityGrpMaterialModifier *pGroupMod = new ApplyAnimateGroupOnly( pQualityMod );
//			m_pSceneMgr->ModifyEntityGroupMaterial( pGroupMod );
//			delete pGroupMod;

			Setting::SetLightQuality( rkNewOption.m_LightQuality );

			if( rkNewOption.m_LightQuality == LQ_HIGH )
				ioEffect::EnableEffectLightCreation( true );
			else
				ioEffect::EnableEffectLightCreation( false );
		}
	}

	if( kOldOption.m_TextureQuality != rkNewOption.m_TextureQuality )
	{
		m_pTextureMgr->ChangeTextureQuality( rkNewOption.m_TextureQuality );
		Setting::SetTextureQuality( rkNewOption.m_TextureQuality );
	}

	if( kOldOption.m_EffectQuality != rkNewOption.m_EffectQuality )
	{
		ApplyEffectQuality( rkNewOption.m_EffectQuality );
		Setting::SetEffectQuality( rkNewOption.m_EffectQuality );
	}

	// ����̽� ������ �ʿ��� �ɼǵ�
	// ����̽� ���� �����Ҽ� �ֱ⶧���� ������ �����ؾ߸� GameOptionWnd���� Setting�� ������
	bool bResetDevice = false;

	if( kOldOption.m_bUse16bitFrameBuffer != rkNewOption.m_bUse16bitFrameBuffer )
	{
		bResetDevice |= m_pRenderSystem->ChangeBackBufferFormat( rkNewOption.m_bUse16bitFrameBuffer );
		Setting::SetUse16BitFrameBuffer( rkNewOption.m_bUse16bitFrameBuffer );
	}

	bool bMoveWindow = false;
	if( kOldOption.m_bFullScreen != rkNewOption.m_bFullScreen )
	{
		if( rkNewOption.m_bFullScreen )
			SetWindowLong( m_hWnd, GWL_STYLE, FULLSCREEN_STYLE );
		else
			SetWindowLong( m_hWnd, GWL_STYLE, WINDOW_STYLE );

		SetWindowPos( m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOREDRAW|SWP_NOSIZE );
		bMoveWindow  = true;

		if( ioLocalManagerParent::IsIME() )
			bResetDevice |= m_pRenderSystem->ChangeScreenMode( rkNewOption.m_bFullScreen );
		Setting::SetFullScreen( rkNewOption.m_bFullScreen );

		if( Setting::FullScreen() )
		{
			if( !ioLocalManagerParent::IsIME() )
				ChangeDisplaySettingsBySetting();
		}
		else
		{
			if( !ioLocalManagerParent::IsIME() )
				ChangeDisplaySettings(NULL, 0); 
		}
	}

	if( kOldOption.m_bSwapCopy != rkNewOption.m_bSwapCopy )
	{
		if( m_pBrowserMgr && !m_pBrowserMgr->IsWebOpened() )
		{
			D3DSWAPEFFECT eSwap = D3DSWAPEFFECT_DISCARD;
			if( rkNewOption.m_bSwapCopy )
				eSwap = D3DSWAPEFFECT_COPY;

			bResetDevice |= m_pRenderSystem->ChangeSwapEffect( eSwap );
		}
		
		Setting::SetSwapCopy( rkNewOption.m_bSwapCopy );

		if( rkNewOption.m_bSwapCopy )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		}
	}

	if( kOldOption.m_AntiAliasingQuality != rkNewOption.m_AntiAliasingQuality )
	{
		if( m_pBrowserMgr && !m_pBrowserMgr->IsWebOpened() )
		{
			D3DMULTISAMPLE_TYPE eMultiSample = Setting::GetMultiSampleType( rkNewOption.m_AntiAliasingQuality );
			bResetDevice |= m_pRenderSystem->ChangeMultiSamplingType( eMultiSample );
		}
	
		Setting::SetAntiAliasingQuality( rkNewOption.m_AntiAliasingQuality );
	}

	if( kOldOption.m_iWidth != rkNewOption.m_iWidth || kOldOption.m_iHeight != rkNewOption.m_iHeight )
	{
		m_iWidth  = rkNewOption.m_iWidth;
		m_iHeight = rkNewOption.m_iHeight;
		bMoveWindow = true;

		// UI���ġ
		float fNewRateX = (float)rkNewOption.m_iWidth / 800.0f;
		float fNewRateY = (float)rkNewOption.m_iHeight / 600.0f;

		ioWnd::SetScreenRate( fNewRateX, fNewRateY );
		ioMouse::SetMaxWindowSize( rkNewOption.m_iWidth, rkNewOption.m_iHeight );
		Setting::SetCurResolution( rkNewOption.m_iWidth, rkNewOption.m_iHeight );

		m_pGUIMgr->SetUIPostionToDefault();

		ReSize( rkNewOption.m_iWidth, rkNewOption.m_iHeight );

		// ����� ����
		bResetDevice |= m_pRenderSystem->ChangeBackBufferSize( rkNewOption.m_iWidth, rkNewOption.m_iHeight );

		if( !ioLocalManagerParent::IsIME() )
		{
			if( Setting::FullScreen() )
				ChangeDisplaySettingsBySetting();
		}
	}

	bool bResetOK = true;
	if( bResetDevice )
	{
		bResetOK = m_pRenderSystem->ApplyChangedPresentParameters();
	}

	if( bMoveWindow )
	{
		if( rkNewOption.m_bFullScreen )
		{
			MoveWindow( m_hWnd, 0, 0, m_iWidth, m_iHeight, FALSE );
		}
		else
		{
			RECT rcRect;
			SetRect( &rcRect, 0, 0, m_iWidth, m_iHeight );
			AdjustWindowRectEx( &rcRect, WINDOW_STYLE, FALSE, 0 );

			MoveWindow( m_hWnd,
						(m_iDeskTopWidth - m_iWidth)/2 - 3,
						(m_iDeskTopHeight - m_iHeight)/2 - 22,
						rcRect.right - rcRect.left, 
						rcRect.bottom - rcRect.top, 
						TRUE );
		}

		if( !IsWindowVisible(m_hWnd) )
		{
			ShowWindow( m_hWnd, SW_SHOW );
		}
	}

	return bResetOK;
}

void ioApplication::ApplyEffectQuality( int iQuality )
{
	if( !m_pEffectFac )	return;

	switch( iQuality )
	{
	case EQ_HIGH:
		m_pEffectFac->SetLimitMaxParticleQuota( false, FLOAT1 );
		ioMeshTrailer::SetTrailSegmentRate( FLOAT1 );
		break;
	case EQ_MIDDLE:
		m_pEffectFac->SetLimitMaxParticleQuota( true, 0.75f );
		ioMeshTrailer::SetTrailSegmentRate( 0.65f );
		break;
	case EQ_LOW:
		m_pEffectFac->SetLimitMaxParticleQuota( true, 0.4f );
		ioMeshTrailer::SetTrailSegmentRate( 0.3f );
		break;
	default:
		LOG.PrintTimeAndLog( 0, "ioApplication::ApplyEffectQuality - Unknown(%d)", iQuality );
		break;
	}
}

void ioApplication::SetMouseBusy( bool bBusy )
{
	if( bBusy )
	{
		m_Mouse.SetBusy();
	}
	else
	{
		m_Mouse.SetUnBusy();
	}
}

bool ioApplication::IsMouseBusy()
{
	return m_Mouse.IsBusy();
}

bool ioApplication::IsMouseRender()
{
	bool bLobbyStage = false;
	// �κ񿡼��� ���콺 Ŀ���� �׻� �������Ѵ�.
	ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
	if( pPlayStage )
	{
		if( pPlayStage->GetModeType() == MT_MYROOM )
		{
			if( ioMyRoomMode::GetMyRoomType() == MRT_LOBBY )
			{
				bLobbyStage = true;
			}
		}
	}

	if( bLobbyStage )
	{
		if( FRAMEGETTIME() - m_dwMousePreCheckTime < m_dwMouseUnvisiblLobbyTime )
			return true;
	}
	else
	{
		if( FRAMEGETTIME() - m_dwMousePreCheckTime < m_dwMouseUnvisiblTime )
			return true;
	}	
	return false;
}

void ioApplication::SetExitProgram()
{
	m_bProgramExit = true;
}

void ioApplication::WindowMouseCursorProcess()
{
	if( Setting::FullScreen() )
		return;

	RECT rcWindow;
	GetWindowRect( m_hWnd, &rcWindow );

	RECT rcDefault;
	SetRect( &rcDefault, 0, 0, Setting::Width(), Setting::Height() );
	AdjustWindowRectEx( &rcDefault, WINDOW_STYLE, FALSE, 0 );

    if( (rcDefault.right - rcDefault.left != rcWindow.right - rcWindow.left) || 
		(rcDefault.bottom - rcDefault.top != rcWindow.bottom - rcWindow.top ) )
	{
		static int iSetWidth  = 0;
		static int iSetHeight = 0;

		// â��带 �ִ��ػ󵵸� �����ϸ� rcWindowũ�Ⱑ rcDefault ũŰ�� �������� �����Ƿ� SetWindowPos�� 1���� �ϱ� ���ؼ�
		// SetWindowPos��� ȣ���ϸ� �������� ���ӳ� Ŭ�� �̺�Ʈ�� �ȿ´�.
		if( ( iSetWidth  != rcDefault.right - rcDefault.left ) ||
            ( iSetHeight != rcDefault.bottom - rcDefault.top )  )
		{
			if( SetWindowPos(  m_hWnd,
							   NULL,
				               0, 0,
				               rcDefault.right - rcDefault.left,
				               rcDefault.bottom - rcDefault.top,
				               SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER ) )
			{
				iSetWidth  = rcDefault.right - rcDefault.left;
				iSetHeight = rcDefault.bottom - rcDefault.top;
			}
		}
	}

	POINT ptCursor;
	GetCursorPos( &ptCursor );
	GetWindowRect( m_hWnd, &rcWindow );

	bool bNeedWindowMove = false;

	if( m_bWindowTitleDragging )
	{
		ShowWindowCursor( SWCT_TITLE, true );

		POINT ptMove;
		ptMove.x = ptCursor.x - m_ptPreDragPos.x;
		ptMove.y = ptCursor.y - m_ptPreDragPos.y;

		if( ptMove.x != 0 || ptMove.y != 0 )
		{
			rcWindow.left  += ptMove.x;
			rcWindow.right += ptMove.x;
			rcWindow.top    += ptMove.y;
			rcWindow.bottom += ptMove.y;

			m_ptPreDragPos = ptCursor;
			bNeedWindowMove = true;
		}
	}
	else
	{
		RECT rcTitle = rcWindow;
		rcTitle.bottom = rcWindow.top + 22;

		if( PtInRect( &rcTitle, ptCursor ) )
			ShowWindowCursor( SWCT_TITLE, true );
		else
			ShowWindowCursor( SWCT_TITLE, false );
	}

	// ȭ�� ���� ������� Ȯ��
	int iMoveAmt = 0;
	int iDeskTopWidth  = GetSystemMetrics(SM_CXSCREEN);
	int iDeskTopHeight = GetSystemMetrics(SM_CYSCREEN);

	if( rcWindow.top < 0 )
	{
		iMoveAmt = -rcWindow.top;
		rcWindow.top += iMoveAmt;
		rcWindow.bottom += iMoveAmt;
		bNeedWindowMove = true;
	}

	if( rcWindow.left < 0 )
	{
		iMoveAmt = -rcWindow.left;
		rcWindow.left += iMoveAmt;
		rcWindow.right += iMoveAmt;
		bNeedWindowMove = true;
	}

#ifdef SHIPPING
	//UJ 140108, �۾����� ���Ǹ� ���� ���ߺ��忡���� ���� ����ͷ� �̵������ϵ��� ��
	if( rcWindow.bottom > iDeskTopHeight )
	{
		iMoveAmt = rcWindow.bottom - iDeskTopHeight;
		rcWindow.top -= iMoveAmt;
		rcWindow.bottom -= iMoveAmt;
		bNeedWindowMove = true;
	}

	if( rcWindow.right > iDeskTopWidth )
	{
		iMoveAmt = rcWindow.right - iDeskTopWidth;
		rcWindow.left -= iMoveAmt;
		rcWindow.right -= iMoveAmt;
		bNeedWindowMove = true;
	}
#endif

	if( bNeedWindowMove )
	{
		// SetWindowPos��� ȣ���ϸ� ���ӳ� �������� Ŭ�� �̺�Ʈ�� �ȿ´�.
		static int iSetXPos = 0;
		static int iSetYPos = 0;

		if( iSetXPos != rcWindow.left ||
			iSetYPos != rcWindow.top   )
		{

			if( SetWindowPos( m_hWnd,
							  NULL,
							  rcWindow.left,
							  rcWindow.top,
							  0, 0,
							  SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER ) )
			{
				iSetXPos = rcWindow.left;
				iSetYPos = rcWindow.top;
			}
		}
	}
}

void ioApplication::WindowTitleDragStart()
{
	if( Setting::FullScreen() )
		return;

	POINT ptCursor;
	GetCursorPos( &ptCursor );

	RECT rcWindow;
	GetWindowRect( m_hWnd, &rcWindow );
	rcWindow.bottom = rcWindow.top + 22;

	if( PtInRect( &rcWindow, ptCursor ) )
	{
		m_ptPreDragPos = ptCursor;
		m_bWindowTitleDragging = true;
	}
	else
	{
		m_bWindowTitleDragging = false;
	}
}

void ioApplication::WindowTitleDragEnd()
{
	m_ptPreDragPos.x = 0;
	m_ptPreDragPos.y = 0;
	m_bWindowTitleDragging = false;
}

void ioApplication::ShowWindowCursor( DWORD dwShowWindowCursorStyle, bool bShow )
{
	if( bShow )
	{
		if( m_dwShowWindowCursorStyle > 0 ) 
		{
			m_dwShowWindowCursorStyle |= dwShowWindowCursorStyle;
			return;
		}

		m_dwShowWindowCursorStyle |= dwShowWindowCursorStyle;
		ShowCursor( TRUE );
	}
	else
	{
		if( m_dwShowWindowCursorStyle == 0 )
			return;

		m_dwShowWindowCursorStyle &= ~dwShowWindowCursorStyle;
		if( m_dwShowWindowCursorStyle != 0 )
			return;
		ShowCursor( FALSE );
	}
}

void ioApplication::AltF4ExitProcess( bool bShowMenu )
{
	if( ioSP2GUIManager::GetSingletonPtr() == NULL || ioMyInfo::GetSingletonPtr() == NULL )
	{
		//UJ 141229, ũ���� ����: �����߿� g_GuiMgr�� ���̰ų� �ٸ� �̱����� NULL�̾ �״� ��� �߰�
		LOG.PrintTimeAndLog( 0, "ioApplication::AltF4ExitProcess failed due to NULL singleton, %d/%d", (int)ioSP2GUIManager::GetSingletonPtr(), (int)ioMyInfo::GetSingletonPtr() );
		return;
	}

	switch( ioPlayMode::GetModeType() )
	{
	case MT_MYROOM:
		{
			if( ioMyRoomMode::GetMyRoomType() == MRT_PRACTICE )
			{
				if( g_MyInfo.IsTutorialUser() )
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
					return;
				}
			}
		}
		break;
	case MT_HEADQUARTERS:
		{
			if( g_MyInfo.IsTutorialUser() )
			{
				if( g_App.GetConnectedMoveMode() == ioApplication::CMM_LOBBY )
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
				}
				else
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
				}
				return;
			}
		}
		break;
	}

	if( bShowMenu )
	{
		if( g_App.g_App.GetConnectedMoveMode() != ioApplication::CMM_LOBBY )
		{
			g_GUIMgr.ShowWnd( LOBBY_QUICK_EXIT_WND );
			return;
		}
	}
	

	DisconnectWnd *pDisWnd = dynamic_cast<DisconnectWnd*> ( g_GUIMgr.FindWnd(DISCONNECT_WND) );
	if( pDisWnd && pDisWnd->IsShow() )
		return;

	if( g_BattleRoomMgr.IsBattleRoom() && !g_GUIMgr.IsShow( PARTY_SHAM_BATTLE_WND ) )
	{
		ioBattleRoomExitMsgBox *pExitBox = dynamic_cast<ioBattleRoomExitMsgBox*>( g_GUIMgr.FindWnd( BATTLEROOM_EXIT_MSG_BOX ) );
		if( pExitBox )
		{
			pExitBox->SetMsgInfo( ToPlayStage( m_pGameStage ) );
		}
	}
	else
	{
		ioExitMsgBox *pExitBox = dynamic_cast<ioExitMsgBox*>( g_GUIMgr.FindWnd( EXIT_MSG_BOX ) );
		if( pExitBox )
		{
			pExitBox->SetMsgInfo( ToPlayStage( m_pGameStage ) );
		}
	}
}

void ioApplication::QueryEndSessionProcess()
{
	ioPlayStage *pStage = ToPlayStage( m_pGameStage );
	if( !pStage || !m_bNetworkPlay )
	{
		LOG.PrintTimeAndLog( 0, "ExitProgram - 1" );

#if defined( USE_GA )
		if( GetGAStart() == true )
		{
			char chLabel[32] = {0,};

			if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
				sprintf_e( chLabel, "%d", 1 );
			else
				SafeSprintf( chLabel, sizeof(chLabel), "%1", 1 );

			// GAME_END_ERR_POS
			g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
				, "Game"
				, "Error"
				, chLabel
				, 1
				, "%2FGAME%2FOVER%2FERR"
				, 1 );

			g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FGAME%2FOVER%2FERR", 6, chLabel );
		}		
#endif

		SetExitProgram();
		return;
	}

	ModeType eModeType = pStage->GetModeType();
	if( eModeType == MT_MYROOM )
	{
		if( m_pQuestManager )
			m_pQuestManager->ProcessChangeStage();
		SP2Packet kPacket( CTPK_LOGOUT );
		TCPNetwork::SendToServer( kPacket );
		return;
	}

	pStage->SendOwnerDamageList();

	SP2Packet kExitPacket( CTPK_EXIT_ROOM );
	if( eModeType == MT_TRAINING || eModeType == MT_HEADQUARTERS || eModeType == MT_HOUSE )
	{
		kExitPacket << EXIT_ROOM_WANT << 0 << true;
	}
	else
	{
		kExitPacket << EXIT_ROOM_PENALTY << g_MyInfo.GetCurPenaltyPeso() << true;
	}

	TCPNetwork::SendToServer( kExitPacket );
}

void ioApplication::SetNextStage( ioGameStage::GameStage eStage, const char* szCall )
{
	m_NextStage = eStage;

	LOG.PrintTimeAndLog(0, "Call Next Stage : %s", szCall );
}

void ioApplication::SetModeType( int iModeType, int iSubNum, int iModeMapNum, PlazaType ePlazaType )
{
	// ������ ������ �ʱ�ȭ ( ����Ȯ���� �����ϱ� ���� ���⼭ �ʱ�ȭ���� )
	if( m_NextStage == ioGameStage::GS_LOBBY && iModeType == 0 && iSubNum == 0 && iModeMapNum == 0 && ePlazaType == PT_NONE )
	{
		if ( m_pLoadingRender )
			m_pLoadingRender->InitTipInfo();
	}
	else if( m_iModeType != iModeType || m_iModeSubNum != iSubNum || m_iModeMapNum != iModeMapNum || m_ePlazaType != ePlazaType )
	{
		if ( m_pLoadingRender )
			m_pLoadingRender->InitTipInfo();
	}
	else if( m_NextStage == ioGameStage::GS_FIND_MODE && iModeType == 0 && iSubNum == 0 && iModeMapNum == 0 && ePlazaType == PT_NONE )
	{
		if ( m_pLoadingRender )
			m_pLoadingRender->InitTipInfo();
	}

	m_iModeType   = iModeType;
	m_iModeSubNum = iSubNum;
	m_iModeMapNum = iModeMapNum;
	m_ePlazaType  = ePlazaType;
	m_bCampBattle = false;
	m_bCampHeroBattle = false;
	
	// ���������� üũ.
	if( m_pLadderTeamMgr )
	{
		if( m_iModeType != MT_MYROOM && m_iModeType != MT_TRAINING && m_iModeType != MT_HEADQUARTERS && m_iModeType != MT_HOUSE )
		{
			m_bCampBattle = g_LadderTeamMgr.IsLadderTeam();
			if( m_bCampBattle )
				m_bCampHeroBattle = g_LadderTeamMgr.IsHeroMatchMode();
		}
	}		
}

void ioApplication::SetRoomNum( int iRoomNum )
{
	m_iRoomNum = -1;
	
	if( iRoomNum != -1 )
	{
		m_iRoomNum = iRoomNum;
	}
}

int ioApplication::GetRoomNum()
{
	return m_iRoomNum;
}

void ioApplication::ChangeStage()
{
	if( m_NextStage == ioGameStage::GS_NONE )
		return;

	LOG.PrintTimeAndLog( 0, "ioApplication::ChangeState - Next Stage(%d, %d, %d, %d)", m_NextStage, m_iModeType, m_iModeSubNum, m_iModeMapNum );

	RenderLoading( m_iModeType, m_iModeSubNum, m_iModeMapNum, m_bCampBattle, m_bCampHeroBattle, m_ePlazaType, 0, 0, m_eLoadingType );

	ErrorReport::SetPosition( 1000, 100 );

	ioGameStage::GameStage eCurStage = ioGameStage::GS_NONE;
	if( m_pGameStage )
	{
		eCurStage = m_pGameStage->GetTypeID();
		SAFEDELETE( m_pGameStage );

		if( m_pItemMaker )
			m_pItemMaker->SetPlayStage( NULL );

		if( m_pSkillMaker )
			m_pSkillMaker->SetPlayStage( NULL );

		if( m_pWeaponMgr )
			m_pWeaponMgr->SetPlayStage( NULL );

		if( m_pQuestManager )
			m_pQuestManager->SetPlayStage( NULL );
	}

	ErrorReport::SetPosition( 1000, 101 );

#ifndef SHIPPING
	if( !m_bNetworkPlay )
	{
		ReloadINILists();
	}
#endif

	ErrorReport::SetPosition( 1000, 102 );

	switch( m_NextStage )
	{
	case ioGameStage::GS_LOGIN:
		m_pGameStage = new ioLoginStage( m_pSceneMgr );
		break;
	case ioGameStage::GS_LOBBY:
		m_pGameStage = new ioPlayStage( m_pSceneMgr );
		break;
	case ioGameStage::GS_PLAY:
		m_pGameStage = new ioPlayStage( m_pSceneMgr );
		break;
	case ioGameStage::GS_FIND_MODE:
		m_pGameStage = new ioPlayStage( m_pSceneMgr );
		break;
	} 

	DWORDVec vLoadingTimer;
	DWORD    dwLoadingCurTime = timeGetTime();
	vLoadingTimer.push_back( timeGetTime() );
	if( m_pFriendMgr )
		m_pFriendMgr->SetPlayStage( ToPlayStage( m_pGameStage ) );

	vLoadingTimer.push_back( timeGetTime() );
	ErrorReport::SetPosition( 1000, 103 );

	if( !m_pGameStage )
	{
		LOG.PrintTimeAndLog( 0, "ioApplication::ChangeState - Unknown Next Stage(%d)", m_NextStage );
		return;
	}

	g_GlobalTimer.SetWorkPing();
	vLoadingTimer.push_back( timeGetTime() );

	ErrorReport::SetPosition( 1000, 104 );
	
	if( m_NextStage != ioGameStage::GS_LOGIN )
	{		
		RenderLoading( m_iModeType, m_iModeSubNum, m_iModeMapNum, m_bCampBattle, m_bCampHeroBattle, m_ePlazaType, 0, 0, m_eLoadingType );
	}
	vLoadingTimer.push_back( timeGetTime() );

	ErrorReport::SetPosition( 1000, 105 );

	//m_pSceneMgr->ClearScene();
	vLoadingTimer.push_back( timeGetTime() );

	ErrorReport::SetPosition( 1000, 106 );

	EraseManualResource();
	vLoadingTimer.push_back( timeGetTime() );

	ErrorReport::SetPosition( 1000, 107 );

	g_SoundMgr.AllSoundStop();
	vLoadingTimer.push_back( timeGetTime() );

	ErrorReport::SetPosition( 1000, 108 );

	m_pGUIMgr->InitDeskTop( m_iWidth, m_iHeight );
	m_pBasicTooltipManager->InitDeskTop( m_iWidth, m_iHeight );
	vLoadingTimer.push_back( timeGetTime() );

	ErrorReport::SetPosition( 1000, 109 );

	m_pGameStage->InitStage();
	vLoadingTimer.push_back( timeGetTime() );

	ErrorReport::SetPosition( 1000, 110 );

	RenderLoading( m_iModeType, m_iModeSubNum, m_iModeMapNum, m_bCampBattle, m_bCampHeroBattle, m_ePlazaType, 5, 10, m_eLoadingType );
	vLoadingTimer.push_back( timeGetTime() );

	ErrorReport::SetPosition( 1000, 111 );

	if( m_NextStage == ioGameStage::GS_LOBBY )
	{
		ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
		if( pPlayStage )
		{
			pPlayStage->ChangePlayMode( MT_MYROOM, PT_NONE );
		}
	}
	vLoadingTimer.push_back( timeGetTime() );

	ErrorReport::SetPosition( 1000, 112 );

	m_pInputBox->ClearKeyState();
	vLoadingTimer.push_back( timeGetTime() );

	// Loading End Sound
	if( m_bFirstLoading )
	{
		if( !g_ExSoundMgr.IsSound( ExSound::EST_LOGIN ) )
			g_SoundMgr.PlaySound( m_LoadingEndSound, DSBVOLUME_MAX, PSM_NONE );

		m_dwLoadingEndTime = REALGETTIME() + m_dwLoadingDelay;
	}
	vLoadingTimer.push_back( timeGetTime() );
	
	RenderLoading( m_iModeType, m_iModeSubNum, m_iModeMapNum, m_bCampBattle, m_bCampHeroBattle, m_ePlazaType, 10, 10, m_eLoadingType );	

	if( Help::CheckEnableTimeGapUser( 1 ) && m_pEffectFac )
	{
		m_pEffectFac->DestroyTemplate();
	}

	m_CurStage = m_NextStage;
	m_NextStage = ioGameStage::GS_NONE;	
	UpdateAllUDPRecvTime();
	ErrorReport::SetPosition( 1000, 113 );
	vLoadingTimer.push_back( timeGetTime() );

	CloseGuildMarkPage();
	vLoadingTimer.push_back( timeGetTime() );

	ChangeStageLoadingCheck( vLoadingTimer, timeGetTime() - dwLoadingCurTime );
	vLoadingTimer.clear();

	if( m_pSoldierAwakeMgr )
		m_pSoldierAwakeMgr->SetPlayStage( ToPlayStage( m_pGameStage ) );

	m_bTestChangeStage = true;

#ifndef SHIPPING
	if( m_pShopMgr )
		m_pShopMgr->ReLoad();
#endif
}

void ioApplication::ChangeStageLoadingCheck( DWORDVec &rLoadingVec, DWORD dwTotalTime )
{
	if( m_bFirstLoading ) return;
	if( dwTotalTime < 30000 ) return;
	if( rLoadingVec.empty() ) return;

	// �ε� �ð� ���
	DWORD dwTopTime = 0;
	int iTopIndex   = 0;
	int iIndex      = 0;
	DWORD dwPrevTime= 0;
	DWORDVec::iterator iter;
	for(iter = rLoadingVec.begin();iter != rLoadingVec.end();iter++, iIndex++)
	{
		DWORD &kTime = *iter;
		if( dwPrevTime == 0 )
			dwPrevTime = kTime;

		DWORD dwGapTime = ( kTime - dwPrevTime );
		if( dwTopTime < dwGapTime )
		{
			dwTopTime = dwGapTime;
			iTopIndex = iIndex;
		}
		dwPrevTime = kTime;
	}

	char szError[MAX_PATH] = "";
	sprintf( szError, "Long Time Loading Check(%d:%d:%d) : %s(%d) - %d(%d)", m_iModeType, m_iModeSubNum, m_iModeMapNum, 
					  g_MyInfo.GetPublicID().c_str(), dwTotalTime, iTopIndex, dwTopTime );

	SP2Packet kPacket( LUPK_LOG );
	kPacket << "ClientError";  // �α� ���� Ÿ��
	kPacket << szError;
	kPacket << 3180;  // ������ȣ:3180
	kPacket << false; // write db
	P2PNetwork::SendLog(kPacket);
}

bool ioApplication::ChangeServer()
{
	if( !g_TCPSocketMgr.IsChangeServer() )
		return false;

	// draw state
	ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
	if( pLocal && pLocal->IsChangeServerHelp() )
	{
		ioHashString szChangeState;
		if( g_TCPSocketMgr.GetChangeStateText( szChangeState ) )
		{
			if( m_pLoadingRender )
			{
				m_pLoadingRender->SetChangeServerHelp( szChangeState );
				RenderLoading( m_iModeType, m_iModeSubNum, m_iModeMapNum, m_bCampBattle, m_bCampHeroBattle, m_ePlazaType );
			}
		}
	}

	ChangeStage();
	g_TCPSocketMgr.ChangeServerProcess();
	UpdateAllUDPRecvTime();
	return true;
}

void ioApplication::RenderLoop()
{
	ErrorReport::SetPosition( 1000, 500 );

	m_pRenderSystem->ClearRenderedCounts();

	ErrorReport::SetPosition( 1000, 501 );
	
	if( m_pGameStage )
		m_pGameStage->Render( m_pRenderSystem );

	ErrorReport::SetPosition( 1000, 502 );

	if( ToPlayStage( m_pGameStage ) )
	{
		m_Mouse.ClearMouseShowingState();

		bool bCheckMode = false;
		ioPlayStage *pPlayStage = ToPlayStage(m_pGameStage);
		if( pPlayStage )
		{
			if( m_pGUIMgr->IsNoneUIMode() ||
				pPlayStage->HasCameraFixedStyle(CAMERA_STYLE_MOVIE) ||
				pPlayStage->HasCameraFixedStyle(CAMERA_STYLE_VIEW) )
			{
				if( Setting::MovieModeUIHide() )
					bCheckMode = true;
			}
		}

		if( !bCheckMode )
		{
			if( abs(m_ptMouseMoveAmt.x) > 1 || abs(m_ptMouseMoveAmt.y) > 1 )
			{
				m_Mouse.Render();
				m_dwMousePreCheckTime = FRAMEGETTIME();
			}
			else if( m_Mouse.IsMouseBtnAction() )
			{
				m_Mouse.Render();
				m_dwMousePreCheckTime = FRAMEGETTIME();
			}
			else
			{
				if( IsMouseRender() )
				{
					m_Mouse.Render();
				}
			}
		}
	}
	else
	{
		m_Mouse.Render();
	}

#ifndef SHIPPING
	RenderDebugInfo();

	if( Help::CheckEnableTimeGapUser(2) )
	{
		if( m_dwTestFrameStartTime == 0 )
		{
			m_dwTestFrameStartTime = timeGetTime();
			m_iTestFrameCnt = 0;
		}

		m_iTestFrameCnt++;
	}

#endif

	if( IsFPSRender() )
	{
		int iCurFrameFaceCount = m_pRenderSystem->GetRenderedFaceCount() + m_pUIRenderer->GetNotRenderedQuadCount() * 2;

		m_iTotalFrameFaceCount += iCurFrameFaceCount;
		m_iPastedFrameCount++;
		if( m_dwLastFaceCountTime + 1000 < FRAMEGETTIME() )
		{
			m_iFramePerFaceCount = m_iTotalFrameFaceCount / m_iPastedFrameCount;

			m_iTotalFrameFaceCount = 0;
			m_iPastedFrameCount = 0;
			m_dwLastFaceCountTime = FRAMEGETTIME();
		}

		float fEngineFps = 0.0f;
		float fEngineSecFrame = g_FrameTimer.GetSecPerFrame();
		if ( fEngineSecFrame <= 0.0f )
			fEngineFps = 0.0f;
		else
			fEngineFps = 1.0f/fEngineSecFrame;

		g_FontMgr.SetAlignType( TAT_LEFT );
		g_FontMgr.SetTextStyle( TS_SHADOW );
		g_FontMgr.SetTextColor( 255, 255, 0 );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.PrintText( 1.0f, 1.0f, 0.7f, STR(1), m_iFramePerFaceCount, m_pTickTimer->GetFPS(), fEngineFps );
		//g_FontMgr.PrintText( 1.0f, 48.f, 0.7f, "timer = %d", (int)g_FrameTimer.GetTimerType() );
	}

	if( Help::CheckEnableTimeGapUser(2) )
	{
		if( m_dwTestFrameStartTime == 0 )
		{
			m_dwTestFrameStartTime = timeGetTime();
			m_iTestFrameCnt = 0;
		}

		m_iTestFrameCnt++;
	}

#ifdef _ITEST
	RenderClinetMessage();
	RenderPenaltyPoint();
#endif
	

	ErrorReport::SetPosition( 1000, 503 );

	if( m_pUIRenderer )
	{
		m_pUIRenderer->Render( m_pRenderSystem );
	}

	ErrorReport::SetPosition( 1000, 504 );

	if( m_bAppActive )
	{
		ScreenShotCapture();
		CheckScreenShotSaveTask();
	}

	ErrorReport::SetPosition( 1000, 505 );

	if( m_pMovieMaker )
	{
		m_pMovieMaker->MovieCapturing( m_pUIRenderer );
	}

	ErrorReport::SetPosition( 1000, 506 );

	if( m_pGameAnnMgr )
	{
		m_pGameAnnMgr->Render( m_pUIRenderer );
	}

	ErrorReport::SetPosition( 1000, 507 );

#ifndef SHIPPING
	FontTextureTestWnd *pFontTexture = dynamic_cast<FontTextureTestWnd*>(g_GUIMgr.FindWnd( FONT_TEXTURE_WND ));
	if( pFontTexture && pFontTexture->IsShow() )
	{
		pFontTexture->FontTextureRender();
	}	
#endif	

	g_GUIMgr.RenderBoundary();

	if( m_pRenderTargetMgr )
	{
		m_pRenderTargetMgr->OnRender();
	}
}

bool ioApplication::IsVisibleMouse()
{
	return m_Mouse.IsMouseShowing();
}

bool ioApplication::IsMeDeveloper()
{
#ifdef LOCAL_DBG
	return true;
#endif
	//UJ 130829, ����ȭ: IsMeDeveloper()���� �Ʒ��� ���� ini �ε��� ����ϱ� ������ 
	// zip ��������, �޸�Ž�� �� ��û�� ����(����Ÿ���� 14% �̻�)�� �߻�, ĳ���Ѵ� --start
	
	//JSM, 150210, �������� �г����� �޾ƿ����������� ĳ��ó�� �н�
	if ( m_bIsMeDeveloperFirst && !g_MyInfo.GetPublicID().IsEmpty() )
	{
		m_bIsMeDeveloperFirst = false;
		static ioHashString szDeveloper1 = STR(1);
		static ioHashString szDeveloper2 = STR(2);

		//�ڽ��� �г����� ������ �г������� Ȯ��
		if( strnicmp( g_MyInfo.GetPublicID().c_str(), szDeveloper1.c_str(), szDeveloper1.Length() ) == 0 ||
			strnicmp( g_MyInfo.GetPublicID().c_str(), szDeveloper2.c_str(), szDeveloper2.Length() ) == 0  )
		{
			m_bIsMeDeveloperCache = true;
			return true;
		}

		if( g_TCPSocketMgr.IsTestZone() )
		{
			m_bIsMeDeveloperCache = true;
			return true;
		}

		//INI�� ���Ե� �г����� ������ �г������� Ȯ��
		ioINILoader_e kLoader( "config/sp2_developerID.ini" );
		m_bIsMeDeveloperCache = kLoader.LoadBool( "ID", g_MyInfo.GetPublicID().c_str(), false );
		return m_bIsMeDeveloperCache;
	}
	return m_bIsMeDeveloperCache;
	//UJ --end
}

bool ioApplication::IsDeveloperID( const char *szID )
{
	static ioHashString szDeveloper1 = STR(1);
	static ioHashString szDeveloper2 = STR(2);

	if( strnicmp( g_MyInfo.GetPublicID().c_str(), szDeveloper1.c_str(), szDeveloper1.Length() ) == 0 ||
		strnicmp( g_MyInfo.GetPublicID().c_str(), szDeveloper2.c_str(), szDeveloper2.Length() ) == 0  )
	{
		return true;
	}

	static ioHashString szDeveloper3 = STR(3);
	static ioHashString szDeveloper4 = STR(4);

	if( strnicmp( szID, szDeveloper3.c_str(), szDeveloper3.Length() ) == 0 ||
		strnicmp( szID, szDeveloper4.c_str(), szDeveloper4.Length() ) == 0 )
	{
		return true;
	}

	return false;
}

bool ioApplication::IsMeGameMaster()
{
	static ioHashString szGM1 = STR(1);
	if( strnicmp( g_MyInfo.GetPublicID().c_str(), szGM1.c_str(), szGM1.Length() ) == 0 )
	{
		return true;
	}
	return false;
}

bool ioApplication::IsGameMasterID( const char *szID )
{
	static ioHashString szGM1 = STR(1);
	if( strnicmp( szID, szGM1.c_str(), szGM1.Length() ) == 0 ) 
	{
		return true;
	}

	static ioHashString szGM2 = STR(2);
	if( strnicmp( szID, szGM2.c_str(), szGM2.Length() ) == 0 ) 
	{
		return true;
	}

	return false;
}

bool ioApplication::IsAdminID( const char *szID )
{
	static ioHashString szAdminID1 = STR(1);
	static ioHashString szAdminID2 = STR(2);
	static ioHashString szAdminID3 = STR(3);

	if( strnicmp( g_MyInfo.GetPublicID().c_str(), szAdminID1.c_str(), szAdminID1.Length() ) == 0 ||
		strnicmp( g_MyInfo.GetPublicID().c_str(), szAdminID2.c_str(), szAdminID2.Length() ) == 0 ||
		strnicmp( g_MyInfo.GetPublicID().c_str(), szAdminID3.c_str(), szAdminID3.Length() ) == 0 )
	{
		return false;
	}

	static ioHashString szAdminID4 = STR(4);
	static ioHashString szAdminID5 = STR(5);
	static ioHashString szAdminID6 = STR(6);

	if( strnicmp( szID, szAdminID4.c_str(), szAdminID4.Length() ) == 0 ||
		strnicmp( szID, szAdminID5.c_str(), szAdminID5.Length() ) == 0 ||
		strnicmp( szID, szAdminID6.c_str(), szAdminID6.Length() ) == 0 )
	{
		return true;
	}

	return false;
}

void ioApplication::SetReserveCheckExistID( const ioHashString &ReserveCheckExistID )
{
	m_ReserveCheckExistID = ReserveCheckExistID;
}

void ioApplication::CreateScreenShotPath()
{
	ioWnd *pParent = g_GUIMgr.FindWnd( GAME_OPTION_WND );
	if( pParent )
	{
		ioWnd *pChild = pParent->FindChildWnd( GameOptionWnd::ID_GAME_TAB_WND );
		if( pChild )
		{
			GameOptionMovieScreenShotTab *pTabWnd = dynamic_cast<GameOptionMovieScreenShotTab*>( pChild->FindChildWnd( GameOptionWnd::ID_MOVIE_SCREENSHOT_TAB_WND ) );
			if( pTabWnd )
			{
				if( !pTabWnd->CreateScreenShotFolder( Setting::GetScreenShotSavePath().c_str() ) )
				{
					ioHashString szPath;
					pTabWnd->GetDefaultScreenShotFolderName( szPath );
					CreateDirectory( szPath.c_str(), NULL );
					Setting::SetScreenShotSavePath( szPath.c_str() );
				}
			}

		}
	}
}

void ioApplication::ScreenShotCapture()
{
	if( !P2PNetwork::IsNetworkPlaying() )
	{
		if( !g_Input.IsKeyPress( KEY_PRINTSCRN ) && !m_bReservedScreenShot )
			return;
	}
	else
	{
		if( (!g_KeyManager.IsScreenShotKeyPress() || g_GUIMgr.IsShowSettingKeyWnd() ) && !m_bReservedScreenShot )
			return;
	}
/*
#ifdef SHIPPING
	if( (!g_KeyManager.IsScreenShotKeyPress() || g_GUIMgr.IsShowSettingKeyWnd() ) && !m_bReservedScreenShot )
		return;
#else
	if( !g_Input.IsKeyPress( KEY_PRINTSCRN ) && !m_bReservedScreenShot )
		return;
#endif
*/
	if( m_pScreenShotSaveTask || m_dwScreenShotStartTime > 0 )
	{
		m_bReservedScreenShot = true;	// �����߿� ������ ���� ���
		return;
	}

	if( g_MovieMaker.IsScreenShotComplete() ) // ���� ��� ǥ�����̶�� ����� ��´�.
	{
		g_MovieMaker.ClearScreenShotComplete();
		m_bReservedScreenShot = true;	
		return;
	}
	m_bReservedScreenShot  = false;

	ioWnd *pParent = g_GUIMgr.FindWnd( GAME_OPTION_WND );
	if( pParent )
	{
		ioWnd *pChild = pParent->FindChildWnd( GameOptionWnd::ID_GAME_TAB_WND );
		if( pChild )
		{
			GameOptionMovieScreenShotTab *pTabWnd = dynamic_cast<GameOptionMovieScreenShotTab*> 
				 				                 ( pChild->FindChildWnd( GameOptionWnd::ID_MOVIE_SCREENSHOT_TAB_WND ) );
			if( pTabWnd )
			{
				if( !pTabWnd->CreateScreenShotFolder( Setting::GetScreenShotSavePath().c_str() ) )
				{
					ioHashString szPath;
					pTabWnd->GetDefaultScreenShotFolderName( szPath );
					CreateDirectory( szPath.c_str(), NULL );
					Setting::SetScreenShotSavePath( szPath.c_str() );
				}
			}
		}
	}
	g_MovieMaker.SetScreenShot();

	ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
	if( pPlayStage )
	{
		ioBaseChar *pOwner = pPlayStage->GetOwnerChar();
		if( pOwner )
			pOwner->SetScreenShotEmoticon();
	}

	m_pScreenShotSaveTask = new ioScreenShotTask( Setting::GetScreenShotSavePath(),
												  Setting::ScreenShotFormatJPG(),
												  Setting::Use16BitFrameBuffer() );

	if( m_pScreenShotSaveTask->Initialize( m_pRenderSystem, D3DTEXF_NONE, FLOAT1 ) )
	{
		g_SoundMgr.PlaySound( m_ScreenShotStartSound, DSBVOLUME_MAX, PSM_NONE );
		m_dwScreenShotStartTime = REALGETTIME();
	}
	else
	{
		delete m_pScreenShotSaveTask;
		m_pScreenShotSaveTask = NULL;

		m_dwScreenShotStartTime = 0;
	}
}

void ioApplication::CheckScreenShotSaveTask()
{
	if( !m_pScreenShotSaveTask || m_dwScreenShotStartTime == 0 )
		return;

	if( m_dwScreenShotStartTime + m_dwScreenShotMinGap < REALGETTIME() )
	{
		g_ThreadTaskMgr.AddTask( m_pScreenShotSaveTask );
		m_pScreenShotSaveTask = NULL;
	}
}

void ioApplication::ScreenShotDone( const char *szPath, bool bSuccess )
{
	m_pScreenShotSaveTask = NULL;
	m_dwScreenShotStartTime = 0;

	if( bSuccess )
	{
		g_MovieMaker.SetScreenShotComplete( szPath );
	}
}

void ioApplication::CheckMouseControlMode( bool bMoveMode )
{
	ioPlayStage *pPlayState = ToPlayStage( m_pGameStage );
	if( pPlayState )
	{
		ioBaseChar* pChar = pPlayState->GetOwnerChar();
		if( pChar && pChar->GetState() == CS_CONSTRUCT_HOUSE )
		{
			if( !pPlayState->HasCameraFixedStyle( CAMERA_STYLE_HOUSING ) && bMoveMode )
				m_bMouseControlModeCancel = false;

			if( bMoveMode )
				pPlayState->SetCameraModeFixedByHousing();
			else
				pPlayState->ClearCameraModeFixed( CAMERA_STYLE_HOUSING );
		}
		else if ( pChar && pChar->IsFixedExceptionCamera() )
		{
			pPlayState->ClearCameraModeFixed( CAMERA_STYLE_MOVIE );
		}
		else
		{
			if( !pPlayState->HasCameraFixedStyle( CAMERA_STYLE_MOVIE ) && bMoveMode )
				m_bMouseControlModeCancel = false;

			if( bMoveMode && !g_GUIMgr.IsShow( ITEM_CUSTOM_MAKE_DETAIL_VIEW_WND ) && !g_GUIMgr.IsShow( ITEM_CUSTOM_MAKE_WND ) && !g_GUIMgr.IsShow( COSTUME_CUSTOM_MAKE_DETAIL_VIEW_WND ) && !g_GUIMgr.IsShow( COSTUME_CUSTOM_MAKE_WND ) )
				pPlayState->SetCameraModeFixed( CAMERA_STYLE_MOVIE );
			else
				pPlayState->ClearCameraModeFixed( CAMERA_STYLE_MOVIE );
		}
	}
}

void ioApplication::CheckServerToClientPing()
{
#ifdef _ITEST
	return;
#endif

	if( !P2PNetwork::IsNetworkPlaying() )
		return;

	if( m_pGlobalTimer == NULL ) return;

	if( !g_GlobalTimer.CheckServerToClientPing() )
	{
		DisconnectWnd *pDisWnd = dynamic_cast<DisconnectWnd*> ( m_pGUIMgr->FindWnd(DISCONNECT_WND) );
		if( pDisWnd )
		{
			pDisWnd->SetType( DisconnectWnd::TYPE_SERVER_TO_CLIENT_PING_EXIT );
			pDisWnd->ShowWnd();
		}
	}
}

void ioApplication::CheckLanConnected()
{
#ifdef _ITEST
	return;
#endif

	if( !P2PNetwork::IsNetworkPlaying() )
		return;

	if( m_dwNextLanCheckTime == 0 )	// firsttime
	{
		m_dwPreLanCheckState = Help::GetLanCableConnectState();
		m_dwNextLanCheckTime = REALGETTIME() + 5000;
		return;
	}

	if( m_dwNextLanCheckTime < REALGETTIME() )
	{
		DWORD dwState = Help::GetLanCableConnectState();
		if( dwState == m_dwPreLanCheckState )
		{
			m_dwNextLanCheckTime = REALGETTIME() + 1000;
		}
		else
		{
			if( m_dwPreLanCheckState )	// DisConnected..
			{
				if( m_pGUIMgr )
				{
					DisconnectWnd *pDisWnd = dynamic_cast<DisconnectWnd*> ( m_pGUIMgr->FindWnd(DISCONNECT_WND) );
					if( pDisWnd )
					{
						pDisWnd->SetType( DisconnectWnd::TYPE_LANCABLE_EXIT );
						pDisWnd->ShowWnd();
					}
				}
				else
				{
					Help::MessageboxShow( NULL, STR(2), STR(1), MB_TOPMOST );
					LOG.PrintTimeAndLog( 0, "ExitProgram - 2" );

#if defined( USE_GA )
					if( GetGAStart() == true )
					{
						char chLabel[32] = {0,};

						if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
							sprintf_e( chLabel, "%d", 2 );
						else
							SafeSprintf( chLabel, sizeof(chLabel), "%1", 2 );

						// GAME_END_ERR_POS
						g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
							, "Game"
							, "Error"
							, chLabel
							, 1
							, "%2FGAME%2FOVER%2FERR"
							, 1 );

						g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FGAME%2FOVER%2FERR", 6, chLabel );
					}					
#endif

					SetExitProgram();
				}

				m_dwNextLanCheckTime = -1;	// ��˻� ����
			}
			else
			{
				// ReConnected..
			}
		}

		m_dwPreLanCheckState = dwState;

		// SKL hack
		static bool s_bReadFile = false;
		static ioBinaryStream s_kStream;
		if( !s_bReadFile )
		{
			s_bReadFile = true;

			char szPath[MAX_PATH];
			wsprintf_e( szPath, "resource/skeleton/eff_shadow_shop_delay.skl" );
			g_ResourceLoader.LoadStream( szPath, &s_kStream );
		}

		if( s_bReadFile )
		{
			s_kStream.SetCurPos(0);
			ioSkeletonManager* pSkel = ioSkeletonManager::GetSingletonPtr();
			if( !pSkel->IsSkeletonFile( s_kStream ) )
			{
				bool bForceKill = true;
				DisconnectWnd *pDisWnd = static_cast<DisconnectWnd*> ( m_pGUIMgr->FindWnd(DISCONNECT_WND) );
				if( pDisWnd )
				{
					if( !pDisWnd->IsShow() )
					{
						LOG.PrintTimeAndLog( 0, "ExitProgram - 200" );

#if defined( USE_GA )
						if( GetGAStart() == true )
						{
							char chLabel[32] = {0,};

							if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
								SafeSprintf( chLabel, sizeof(chLabel), STR(1), 200 );
							else
								SafeSprintf( chLabel, sizeof(chLabel), "%1", 200 );

							// GAME_END_ERR_POS
							g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
								, "Game"
								, "Error"
								, chLabel
								, 1
								, "%2FGAME%2FOVER%2FERR"
								, 1 );

							g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FGAME%2FOVER%2FERR", 6, chLabel );
						}						
#endif

						pDisWnd->SetType( DisconnectWnd::TYPE_HACK_DETECTED_EXIT );
						pDisWnd->ShowWnd();	
						if( pDisWnd->IsShow() )
							bForceKill = false;
					}
				}

				if( bForceKill )
				{
					LOG.PrintTimeAndLog( 0, "ExitProgram - 201" );

#if defined( USE_GA )
					if( GetGAStart() == true )
					{
						char chLabel[32] = {0,};

						if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
							SafeSprintf( chLabel, sizeof(chLabel), STR(2), 201 );
						else
							SafeSprintf( chLabel, sizeof(chLabel), "%1", 201 );

						// GAME_END_ERR_POS
						g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
							, "Game"
							, "Error"
							, chLabel
							, 1
							, "%2FGAME%2FOVER%2FERR"
							, 1 );

						g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FGAME%2FOVER%2FERR", 6, chLabel );
					}					
#endif

					SetExitProgram();
				}
			}
		}
	}


}

void ioApplication::UpdateAllUDPRecvTime()
{
	m_all_udp_recv_time = FRAMEGETTIME();
}

void ioApplication::CheckRecvUDPUnviable()
{
#ifdef _ITEST
	return;
#endif

	if( m_all_udp_recv_time == 0 || !m_bNetworkPlay || !m_pNetworkTable )
		UpdateAllUDPRecvTime();
	else if( !m_pNetworkTable->IsAllNetworkTimeOut() )
		UpdateAllUDPRecvTime();

	//UDP ���� üũ 20�ʷ� ����
	if( FRAMEGETTIME() - m_all_udp_recv_time <= SYNCTIME_DELAY + 11000 )
		return;
    
	UpdateAllUDPRecvTime();

	ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
	if( !pPlayStage )
		return;

	ioPlayMode *pPlayMode = pPlayStage->GetPlayMode();
	if( !pPlayMode )
		return;

	if( pPlayMode->GetModeState() != ioPlayMode::MS_PLAY )
		return;

	if( pPlayMode->GetModeType() == MT_MYROOM || pPlayMode->GetModeType() == MT_NONE )
		return;

	ioBaseChar *pOwnerChar = pPlayStage->GetOwnerChar();
	if( !pOwnerChar )
		return;

	if( pOwnerChar->GetState() == CS_LOADING )
		return;

	if( pOwnerChar->GetState() == CS_READY )
		return;

	if( pOwnerChar->GetState() == CS_VIEW )
		return;

	if( pOwnerChar->GetState() == CS_OBSERVER )
		return;

	if( IsMouseBusy() )
		return;
		
	// ���η� �������� ���� �����ʾ� �����´ٴ� �˸��˾��� ���� ��Ҹ� ���� ��Ų��.
	if( g_App.GetConnectedMoveMode() == CMM_LOBBY )
	{
		if( pPlayMode->GetModeType() == MT_TRAINING || pPlayMode->GetModeType() == MT_HEADQUARTERS || pPlayMode->GetModeType() == MT_HOUSE )
		{
			SP2Packet kPacket( CTPK_EXIT_ROOM );
			kPacket << EXIT_ROOM_LOBBY << 0 << true;
			TCPNetwork::SendToServer( kPacket );
		}
		else
		{
			pPlayStage->SendOwnerDamageList();
			SP2Packet kPacket( CTPK_EXIT_ROOM );
			kPacket << EXIT_ROOM_PENALTY << g_MyInfo.GetCurPenaltyPeso() << true;
			TCPNetwork::SendToServer( kPacket );
		}
	}
	else
	{
		if( pPlayMode->GetModeType() == MT_HEADQUARTERS || pPlayMode->GetModeType() == MT_TRAINING || pPlayMode->GetModeType() == MT_HOUSE )
		{
			SP2Packet kPacket( CTPK_EXIT_ROOM );
			kPacket << EXIT_ROOM_BAD_NETWORK << 0 << false;
			TCPNetwork::SendToServer( kPacket );
		}
		else
		{
			pPlayStage->SendOwnerDamageList();
			SP2Packet kPacket( CTPK_EXIT_ROOM );
			kPacket << EXIT_ROOM_BAD_NETWORK << 0 << false;
			TCPNetwork::SendToServer( kPacket );
		}
	}

	TCPNetwork::MouseBusy( true );
	g_GUIMgr.SetReserveMsgBox( MB_UDP_FAIL_EXIT_ROOM );

	// �α׼����� �α� ����
	char szLOG[2048] = "";
	sprintf( szLOG, "[%s] CheckRecvUDPUnviable : ConnectTCP:%d - ModeType:%d- CharState:%d - ServerPinglose:%d ", 
					g_MyInfo.GetPublicID().c_str(), (int)m_pTCPSocket->IsConnected(), (int)pPlayMode->GetModeType(), 
					(int)pOwnerChar->GetState(), g_GlobalTimer.GetPacketloseCnt() );

	ioUserNetworkTable::NetworkInfoIter iter = m_pNetworkTable->GetNetworkInfoIter();
	while( iter.HasMoreElements() )
	{
		UserNetworkInfo *pInfo = iter.Next();
		char szBuf[MAX_PATH] = "";
		sprintf( szBuf, "- %s[%dsec]", pInfo->m_Name.c_str(), ( REALGETTIME() - pInfo->m_dwLastRecvTime ) / 1000 );
		strcat( szLOG, szBuf );
	}

	SP2Packet kPacket( LUPK_LOG );
	kPacket << "CheckError";  // �α� ���� Ÿ��
	kPacket << szLOG;
	kPacket << 3181;  // ������ȣ:3181
	kPacket << true; // write db
	P2PNetwork::SendLog( kPacket );
}

void ioApplication::RenderDebugInfo()
{
	if( !Setting::ShowExtraInfo() )
	{
		if( Help::CheckEnableTimeGapUser() )
		{
			int iCurFrameFaceCount = m_pRenderSystem->GetRenderedFaceCount() +
				m_pUIRenderer->GetNotRenderedQuadCount() * 2;

			m_iTotalFrameFaceCount += iCurFrameFaceCount;
			m_iPastedFrameCount++;
			if( m_dwLastFaceCountTime + 1000 < FRAMEGETTIME() )
			{
				m_iFramePerFaceCount = m_iTotalFrameFaceCount / m_iPastedFrameCount;

				m_iTotalFrameFaceCount = 0;
				m_iPastedFrameCount = 0;
				m_dwLastFaceCountTime = FRAMEGETTIME();
			}
			
			float fEngineFps = 0.0f;
			float fEngineSecFrame = g_FrameTimer.GetSecPerFrame();
			if ( fEngineSecFrame <= 0.0f )
				fEngineFps = 0.0f;
			else
				fEngineFps = 1.0f/fEngineSecFrame;

			g_FontMgr.SetAlignType( TAT_LEFT );
			g_FontMgr.SetTextStyle( TS_SHADOW );
			g_FontMgr.SetTextColor( 255, 255, 0 );
			g_FontMgr.SetBkColor( 0, 0, 0 );
			g_FontMgr.PrintText( 1.0f, 1.0f, 0.7f, STR(1), m_iFramePerFaceCount, m_pTickTimer->GetFPS(), fEngineFps );
			//g_FontMgr.PrintText( 1.0f, 48.0f, 0.7f, "timer = %d", (int)g_FrameTimer.GetTimerType() );
		}
		return;
	}

	int iCurFrameFaceCount = m_pRenderSystem->GetRenderedFaceCount() +
							 m_pUIRenderer->GetNotRenderedQuadCount() * 2;

	m_iTotalFrameFaceCount += iCurFrameFaceCount;
	m_iPastedFrameCount++;
	if( m_dwLastFaceCountTime + 1000 < FRAMEGETTIME() )
	{
		m_iFramePerFaceCount = m_iTotalFrameFaceCount / m_iPastedFrameCount;

		m_iTotalFrameFaceCount = 0;
		m_iPastedFrameCount = 0;
		m_dwLastFaceCountTime = FRAMEGETTIME();
	}

	float fEngineFps = 0.0f;
	float fEngineSecFrame = g_FrameTimer.GetSecPerFrame();
	if ( fEngineSecFrame <= 0.0f )
		fEngineFps = 0.0f;
	else
		fEngineFps = 1.0f/fEngineSecFrame;

	g_FontMgr.SetAlignType( TAT_LEFT );
	g_FontMgr.SetTextStyle( TS_SHADOW );
	g_FontMgr.SetTextColor( 255, 255, 0 );
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.PrintText( 1.0f, 1.0f, 0.7f, STR(2), m_iFramePerFaceCount, m_pTickTimer->GetFPS(), fEngineFps );
	//g_FontMgr.PrintText( 1.0f, 48.0f, 0.7f, "timer = %d", (int)g_FrameTimer.GetTimerType() );

	const float fMegaBytes = 1024.0f * 1024.0f;

	float fTextureMemory = (float)m_pTextureMgr->GetTextureAboutMemoryBytes() / fMegaBytes;
	float fVtxMemory = (float)m_pVtxBufMgr->GetTotalHeapMemory() / fMegaBytes;
	float fIdxMemory = (float)m_pIdxBufMgr->GetTotalHeapMemory() / fMegaBytes;

	float fManagedMemory = (float)m_pVtxBufMgr->GetTotalHeapMemory( BT_MANAGED ) / fMegaBytes;
	float fDynamicMemory = (float)m_pVtxBufMgr->GetTotalHeapMemory( BT_DYNAMIC ) / fMegaBytes;

	g_FontMgr.PrintText( FLOAT1, 24.0f, 0.7f, STR(2),
											fTextureMemory,
											fVtxMemory, fManagedMemory, fDynamicMemory,
											fIdxMemory );

	if( Setting::ShowMemoryUsage() )
	{
		g_FontMgr.PrintText( FLOAT1, 44.0f, 0.7f, STR(3), 
												m_pTextureMgr->GetTotalTextureCount(),
												(float)m_pTextureMgr->GetTextureAboutMemoryBytes()/fMegaBytes );

		g_FontMgr.PrintText( FLOAT1, 64.0f, 0.7f, STR(4),
												m_pVtxBufMgr->GetTotalHeapCount(),
												(float)m_pVtxBufMgr->GetTotalHeapMemory()/fMegaBytes,
												(float)m_pVtxBufMgr->GetTotalHeapUsedMemory()/fMegaBytes,
												(float)m_pVtxBufMgr->GetTotalHeapFreeMemory()/fMegaBytes );

		g_FontMgr.PrintText( FLOAT1, 84.0f, 0.7f, STR(5),
												m_pIdxBufMgr->GetTotalHeapCount(),
												(float)m_pIdxBufMgr->GetTotalHeapMemory()/fMegaBytes,
												(float)m_pIdxBufMgr->GetTotalHeapUsedMemory()/fMegaBytes,
												(float)m_pIdxBufMgr->GetTotalHeapFreeMemory()/fMegaBytes );
	}

	DWORD dwProcessID = 0;
	::GetWindowThreadProcessId( m_hWnd, &dwProcessID );
	g_FontMgr.PrintText( 1.0f, 104.0f, 0.7f, STR(1), dwProcessID );
}

#ifndef ANTIHACK
void ioApplication::AddNetworkTable( const DWORD dwUserIndex, 
									 const ioHashString &rkName,
									 const ioHashString &szPublicIP,
									 const ioHashString &szPrivateIP,
									 const ioHashString &szTransferIP,
									 int iPort,
									 int iTransferPort,
									 DWORD dwStyle, 
									 int nDebugValue)
{
	if( m_pNetworkTable && m_pTCPSocket )
	{
		//EXCEPTION......
		if( rkName.IsEmpty() || szPublicIP.IsEmpty() || szPrivateIP.IsEmpty() || iPort == 0 )
			return;

		m_pNetworkTable->AddNewUser( dwUserIndex,
									 rkName,
									 szPublicIP.c_str(),
									 szPrivateIP.c_str(),
									 iPort,
									 szTransferIP.c_str(),
									 iTransferPort,
									 dwStyle,
									 nDebugValue );
	}
}
#else
void ioApplication::AddNetworkTable( const DWORD dwUserIndex, 
	const ioHashString &rkName,
	const ioHashString &szPublicIP,
	const ioHashString &szPrivateIP,
	const ioHashString &szTransferIP,
	int iPort,
	int iTransferPort,
	DWORD dwStyle, DWORD dwUserSeed, DWORD dwNPCSeed )
{
	if( m_pNetworkTable && m_pTCPSocket )
	{
		//EXCEPTION......
		if( rkName.IsEmpty() || szPublicIP.IsEmpty() || szPrivateIP.IsEmpty() || iPort == 0 )
			return;

		//LOG.PrintTimeAndLog( 0, "TEST - AddNetworkTable, Uidx(%d), Name(%s), Seed(%d/%d)", dwUserIndex, rkName.c_str(), dwUserSeed, dwNPCSeed );

		m_pNetworkTable->AddNewUser( dwUserIndex,
			rkName,
			szPublicIP.c_str(),
			szPrivateIP.c_str(),
			iPort,
			szTransferIP.c_str(),
			iTransferPort,
			dwStyle, dwUserSeed, dwNPCSeed );
	}
}
#endif


void ioApplication::AddNetworkTableChangeStyle(const ioHashString &rkName, DWORD dwStyle )
{
	if( m_pNetworkTable && m_pTCPSocket )
	{
		//EXCEPTION......
		if( rkName.IsEmpty() )
			return;

		UserNetworkInfo *pInfo = m_pNetworkTable->GetNetworkInfo( rkName );
		if( !pInfo )
			return;
		if( pInfo->m_dwStyle == dwStyle )
			return;

		UserNetworkInfo kInfo = *pInfo;
		
		kInfo.m_dwStyle = dwStyle;
		
#ifndef ANTIHACK
		m_pNetworkTable->AddNewUser( kInfo.m_dwUserIndex,
			kInfo.m_Name,
			kInfo.m_addrPublic.m_IP,
			kInfo.m_addrP2pHole.m_IP,
			kInfo.m_addrPublic.m_Port,
			kInfo.m_addrTransfer.m_IP,
			kInfo.m_addrTransfer.m_Port,
			kInfo.m_dwStyle,
			0);
#else
		m_pNetworkTable->AddNewUser( kInfo.m_dwUserIndex,
			kInfo.m_Name,
			kInfo.m_addrPublic.m_IP,
			kInfo.m_addrP2pHole.m_IP,
			kInfo.m_addrPublic.m_Port,
			kInfo.m_addrTransfer.m_IP,
			kInfo.m_addrTransfer.m_Port,
			kInfo.m_dwStyle,
			kInfo.m_dwUserSeed, kInfo.m_dwNPCSeed );
#endif
		
	}
}

void ioApplication::RemoveNetworkTable( const ioHashString &rkName , DWORD dwStyle )
{
	if( m_pNetworkTable )
	{
		m_pNetworkTable->RemoveUser( rkName , dwStyle );
	}
}

void ioApplication::CheckValidateNetTable( ioPlayStage *pStage )
{
	if( m_pNetworkTable )
	{
		m_pNetworkTable->CheckValidateNetTable( pStage );
	}
}

int ioApplication::GetNetworkTableCnt()
{
	int nCnt = -1;
	if(m_pNetworkTable)
	{
		nCnt = m_pNetworkTable->GetNetworkCnt();
	}

	return nCnt;
}

void ioApplication::ServerToClientPortChange( const ioHashString &rkName, 
									          const ioHashString &szPublicIP,
									          const ioHashString &szPrivateIP,
										      const ioHashString &szTransferIP,
									          int iPort,
									          int iTransferPort )
{   
	if( m_pNetworkTable )
	{
		//EXCEPTION.....
		if( rkName.IsEmpty() || szPublicIP.IsEmpty() || szPrivateIP.IsEmpty() || iPort == 0 )
			return;

		m_pNetworkTable->ServerToClientPortChange( rkName, szPublicIP, szPrivateIP, szTransferIP, iPort, iTransferPort );
	}
}

const char *ioApplication::GetUserGlobalIP( const ioHashString &rkName )
{
	if( m_pNetworkTable )
		return m_pNetworkTable->GetUserGlobalIP( rkName );
	return "";
}

void ioApplication::ClearAllNetworkTable( DWORD dwStyle )
{
	if( m_pNetworkTable )
	{
		m_pNetworkTable->RemoveAllUser( dwStyle );
	}
}

void ioApplication::AllNetworkTableServerRelay()
{
	if( !Help::IsOnlyServerRelay() ) return;

	if( m_pNetworkTable )
	{
		m_pNetworkTable->AllNetworkTableServerRelay();
	}
}

bool ioApplication::IsNetworkTableUser( const ioHashString &rkName )
{
	if( m_pNetworkTable )
	{
		if( m_pNetworkTable->GetNetworkInfo( rkName ) == NULL )
			return false;
		return true;
	}
	return false;
}

void ioApplication::GetNetworkTablePublicID( IN DWORD dwUserIndex, OUT ioHashString &rszPublicID )
{
	if( !m_pNetworkTable )
		return;

	UserNetworkInfo *pInfo = m_pNetworkTable->GetNetworkInfo( dwUserIndex );
	if( !pInfo )
		return;

	rszPublicID = pInfo->m_Name;
}

int ioApplication::GetNextSendIndex()
{
	m_iNextSendIndex++;
	return m_iNextSendIndex;
}

void ioApplication::SetLastControlIndex( const ioHashString &rkName, int iIndex )
{
	ioPlayStage *pPlayState = ToPlayStage( m_pGameStage );
	if( pPlayState )
	{
		ioBaseChar *pChar = pPlayState->GetBaseChar(rkName);
		if(pChar)
		{
			m_pNetworkTable->SetLastControlIndex( pChar->GetCharName() , iIndex );
		}
	}
}

int ioApplication::GetLastControlIndex( const ioHashString &rkName ) const
{
	ioPlayStage *pPlayState = ToPlayStage( m_pGameStage );
	if( pPlayState )
	{
		ioBaseChar *pChar = pPlayState->GetBaseChar( rkName );
		if( pChar )
		{
			return m_pNetworkTable->GetLastControlIndex( pChar->GetCharName() ); 
		}
	}

	return m_pNetworkTable->GetLastControlIndex( rkName );
}

void ioApplication::SetLastRecvTime( const ioHashString &rkName, DWORD dwPingTime )
{
	ioPlayStage *pPlayState = ToPlayStage( m_pGameStage );
	if(pPlayState == NULL) return;

	ioBaseChar *pChar = pPlayState->GetBaseChar( rkName );
	if( pChar )
	{
		m_pNetworkTable->SetLastRecvTime( pChar->GetCharName(), dwPingTime );
	}
}

int  ioApplication::GetP2pPingTime( const ioHashString &rkName )
{
	ioPlayStage *pPlayState = ToPlayStage( m_pGameStage );
	if( pPlayState )
	{
		ioBaseChar *pChar = pPlayState->GetBaseChar( rkName );
		if( pChar )
		{
			return m_pNetworkTable->GetP2pPingTime( pChar->GetCharName() ); 
		}
	}

	return m_pNetworkTable->GetP2pPingTime( rkName );
}

bool ioApplication::IsServerRelay( const ioHashString &rkName )
{
	ioPlayStage *pPlayState = ToPlayStage( m_pGameStage );
	if( pPlayState )
	{
		ioBaseChar *pChar = pPlayState->GetBaseChar( rkName );
		if( pChar )
		{
			return m_pNetworkTable->IsServerRelay( pChar->GetCharName() ); 
		}
	}

	return m_pNetworkTable->IsServerRelay( rkName );
}

bool ioApplication::IsNetworkTimeOut( const ioHashString &rkName )
{
	if( !m_pNetworkTable ) return true;

	return m_pNetworkTable->IsNetworkTimeOut( rkName );
}

void ioApplication::SetHoleSendComplete( const ioHashString &rkName, bool bComplete )
{
	if( !m_pNetworkTable ) return;

	UserNetworkInfo *pInfo = m_pNetworkTable->GetNetworkInfo( rkName );
	if( !pInfo )
	{
		LOG.PrintTimeAndLog( 0, "SetHoleSendComplete Not User : %s - %d", rkName.c_str(), (int)bComplete );
		return;
	}
	
	pInfo->m_bHoleSendComplete = bComplete;
}

bool ioApplication::IsHoleSendComplete( const ioHashString &rkName )
{
	if( !m_pNetworkTable ) return false;

	UserNetworkInfo *pInfo = m_pNetworkTable->GetNetworkInfo( rkName );
	if( !pInfo )
	{
		LOG.PrintTimeAndLog( 0, "IsHoleSendComplete Not User : %s", rkName.c_str() );
		return false;
	}
	return pInfo->m_bHoleSendComplete;
}

void ioApplication::SetHoleRecvComplete( const ioHashString &rkName, bool bComplete, const ioHashString &rkIP, const int &rkPort )
{
	if( !m_pNetworkTable ) return;

	UserNetworkInfo *pInfo = m_pNetworkTable->GetNetworkInfo( rkName );
	if( !pInfo ) 
	{
		LOG.PrintTimeAndLog( 0, "SetHoleRecvComplete Not User : %s - %d", rkName.c_str(), (int)bComplete );
		return;
	}

	if( pInfo->m_bHoleRecvComplete && bComplete )
	{
		pInfo->m_iHoleRecvCompleteCallCount++;
		if( pInfo->m_iHoleRecvCompleteCallCount > 20 )
		{
			// 20�� �̻� �ɸ��� ������ �����Ͽ� ����ȭ Ȯ������.
			// Ȧ ��Ŷ �޾Ҵٰ� ������ ����
			SP2Packet kPacket( CTPK_HOLE_SEND_COMPLETE );
			kPacket << pInfo->m_Name << rkIP << rkPort;
			TCPNetwork::SendToServer( kPacket );

			pInfo->m_iHoleRecvCompleteCallCount = 0;
		}
	}
	else
	{
		pInfo->m_bHoleRecvComplete = bComplete;
		pInfo->m_iHoleRecvCompleteCallCount = 0;
	}
}

bool ioApplication::IsHoleRecvComplete( const ioHashString &rkName )
{
	if( !m_pNetworkTable ) return false;

	UserNetworkInfo *pInfo = m_pNetworkTable->GetNetworkInfo( rkName );
	if( !pInfo )
	{
		LOG.PrintTimeAndLog( 0, "IsHoleRecvComplete Not User : %s", rkName.c_str() );
		return false;
	}
	return pInfo->m_bHoleRecvComplete;
}

void ioApplication::GetMyLocalIP(char *iip)
{
	if( m_pTCPSocket )
	{
		sockaddr_in client_addr;
		int			client_addr_size = sizeof(sockaddr_in);
		getsockname( m_pTCPSocket->GetSocket(), (struct sockaddr*)&client_addr, &client_addr_size );
		sprintf( iip,"%d.%d.%d.%d",
				 client_addr.sin_addr.s_net,
				 client_addr.sin_addr.s_host,
				 client_addr.sin_addr.s_lh,
				 client_addr.sin_addr.s_impno );
	}	
}

void ioApplication::SendAllP2PPing()
{
	if( m_bNetworkPlay && m_pUDPSocket )
	{
		SP2Packet kPacket( CUPK_P2P_PING );
		kPacket << g_MyInfo.GetPublicID() << g_GlobalTimer.GetServerDelayMS();

		ioUserNetworkTable::NetworkInfoIter iter = m_pNetworkTable->GetNetworkInfoIter();
		while( iter.HasMoreElements() )
		{
			UserNetworkInfo *pInfo = iter.Next();
			if( pInfo->HasStyle( P2P_STYLE_PLAY ) )
				SendP2PPacket( pInfo->m_addrPublic.m_IP, pInfo->m_addrPublic.m_Port, kPacket );
		}
	}
}

void ioApplication::ClientToClientPortChange( const ioHashString &rkName, const char *szRcvIP, int iRcvPort )
{
	if( m_pNetworkTable )
	{
		m_pNetworkTable->ClientToClientPortChange( rkName, szRcvIP, iRcvPort );
	}
}

ioApplication& ioApplication::GetSingleton()
{
	return Singleton< ioApplication >::GetSingleton();
}

void ioApplication::SendP2PPacket( const char *szIP, int iPort, SP2Packet &rkPacket )
{
	if( m_bNetworkPlay && m_pUDPSocket )
	{
		UserNetworkInfo *pItem = m_pNetworkTable->GetNetworkInfo( szIP, iPort );
		if( pItem )
		{

			#ifdef _DEBUG
			PrintPacketID( rkPacket.GetPacketID() );
			#endif

			g_P2PChecker.AddSendPacket( rkPacket.GetPacketID() );

			if( Help::CheckEnableTimeGapUser(2) )
			{
				SetTestSendData( rkPacket );
			}

			if( pItem->IsServerRelay() )
			{
			
				SP2Packet kPacket( pItem->m_addrPublic.m_IP,
								   pItem->m_addrPublic.m_Port,
								   g_MyInfo.GetUserIndex(),
								   m_iNextUDPSendIndex,
								   rkPacket );

				//CheckUDPData( kPacket );
				m_pUDPSocket->SendLoop( pItem->m_addrSend.m_IP, pItem->m_addrSend.m_Port, kPacket );
			}
			else
			{
				m_setP2PPacket.insert( rkPacket.GetPacketID() );

				SP2Packet kPacket( "",
								   0,
								   g_MyInfo.GetUserIndex(),
								   m_iNextUDPSendIndex,
								   rkPacket );

				//CheckUDPData( kPacket );
				m_pUDPSocket->SendLoop( pItem->m_addrSend.m_IP, pItem->m_addrSend.m_Port, kPacket );
			}

			// ���� �߰� ��Ŷ�� �ƴϸ� üũ �� ���ְ� ����
			m_iNextUDPSendIndex++;
		}
		else
		{
			LOG.PrintTimeAndLog( 0, "ioApplication::SendP2PPacket - %s:%d Not Exist Info", szIP, iPort );
		}
	}
}

void ioApplication::SendP2PDirectPacket( char *szDirectIP, int iDirectPort, SP2Packet &rkPacket )
{
	if( m_bNetworkPlay && m_pUDPSocket )
	{

		#ifdef _DEBUG
		PrintPacketID( rkPacket.GetPacketID() );
		#endif	

		g_P2PChecker.AddSendPacket( rkPacket.GetPacketID() );

		if( Help::CheckEnableTimeGapUser(2) )
		{
			SetTestSendData( rkPacket );
		}

		SP2Packet kPacket( "", 
						   0, 
						   g_MyInfo.GetUserIndex(), 
						   m_iNextUDPSendIndex, 
						   rkPacket );
		//CheckUDPData( kPacket );
    	m_pUDPSocket->SendLoop( szDirectIP, iDirectPort, kPacket );
		
		m_iNextUDPSendIndex++;
	}
}

void ioApplication::SendP2PRelayPacket( const ioHashString &rkName, SP2Packet &rkPacket )
{
	if( m_bNetworkPlay && m_pUDPSocket )
	{
		UserNetworkInfo *pItem = m_pNetworkTable->GetNetworkInfo( rkName );
		if( pItem )
		{

			#ifdef _DEBUG
			PrintPacketID( rkPacket.GetPacketID() );
			#endif

			g_P2PChecker.AddSendPacket( rkPacket.GetPacketID() );

			if( Help::CheckEnableTimeGapUser(2) )
			{
				SetTestSendData( rkPacket );
			}

			SP2Packet kPacket( pItem->m_addrPublic.m_IP,
			            	   pItem->m_addrPublic.m_Port,
							   g_MyInfo.GetUserIndex(),
							   m_iNextUDPSendIndex,
							   rkPacket );			
			CheckUDPData( kPacket );
			m_pUDPSocket->SendLoop( pItem->m_addrTransfer.m_IP, pItem->m_addrTransfer.m_Port, kPacket );			
			m_iNextUDPSendIndex++;
		}
		else
		{
			LOG.PrintTimeAndLog( 0, "ioApplication::SendP2PRelayPacket - %s Not Exist Info", rkName );
		}
	}
}

void ioApplication::SendToAllPlayingUser( SP2Packet &rkPacket, ioBaseChar* pBase )
{
	if( !m_pNetworkTable )	return;

	if( Help::CheckEnableTimeGapUser() && rkPacket.GetBufferSize() > 400 )
	{
		LOG.PrintTimeAndLog( 0, "PS - Send PacketSize : %x, %d", rkPacket.GetPacketID(), rkPacket.GetBufferSize() );
	}

#ifdef ANTIHACK
	// rudp ��Ŷ ��ó��
	RUDPPreProcess( rkPacket, pBase );
#endif

	if( Help::IsOnlyServerRelay() )
	{
		if( !m_pUDPSocket || !m_pTCPSocket ) return;
#ifndef ANTIHACK
		if( m_pNetworkTable->IsNetworkTableEmpty() ) return;
#else
		//if( m_pNetworkTable->IsNetworkTableEmpty() ) return;
#endif
		

#ifdef _DEBUG
		PrintPacketID( rkPacket.GetPacketID() );
#endif
		
		g_P2PChecker.AddSendPacket( rkPacket.GetPacketID() );

		if( Help::CheckEnableTimeGapUser(2) )
		{
			SetTestSendData( rkPacket );
		}

		SP2Packet kPacket( "", 0, g_MyInfo.GetUserIndex(), m_iNextUDPSendIndex,	rkPacket );		//for relay 	
		CheckUDPData( kPacket );
		SendUDPMessageToServer(kPacket);
		m_iNextUDPSendIndex++;
	}
	else
	{
		ioUserNetworkTable::NetworkInfoIter iter = m_pNetworkTable->GetNetworkInfoIter();
		while( iter.HasMoreElements() )
		{
			UserNetworkInfo *pInfo = iter.Next();
			if( pInfo && pInfo->HasStyle( P2P_STYLE_PLAY ) )
			{
				SendP2PPacket( pInfo->m_addrPublic.m_IP, pInfo->m_addrPublic.m_Port, rkPacket );
			}
		}
	}
}

void ioApplication::SendToAllUser( SP2Packet &rkPacket, ioBaseChar* pBase )
{
	if( !m_pNetworkTable )	return;

#ifdef ANTIHACK
	// rudp ��Ŷ ��ó��
	RUDPPreProcess( rkPacket, pBase );
#endif

	if( Help::IsOnlyServerRelay() )
	{
		if( !m_pUDPSocket || !m_pTCPSocket )	return;
		if( m_pNetworkTable->IsNetworkTableEmpty() ) return;

#ifdef _DEBUG
		PrintPacketID( rkPacket.GetPacketID() );
#endif

		g_P2PChecker.AddSendPacket( rkPacket.GetPacketID() );

		if( Help::CheckEnableTimeGapUser(2) )
		{
			SetTestSendData( rkPacket );
		}

		SP2Packet kPacket( "", 0, g_MyInfo.GetUserIndex(), m_iNextUDPSendIndex,	rkPacket );			
		//SendUDPMessageToServer(rkPacket);//kyg ������ ���� 
		CheckUDPData( kPacket );
		SendUDPMessageToServer(kPacket);//kyg ������ ����
		m_iNextUDPSendIndex++;
	}
	else
	{
		ioUserNetworkTable::NetworkInfoIter iter = m_pNetworkTable->GetNetworkInfoIter();
		while( iter.HasMoreElements() )
		{
			UserNetworkInfo *pInfo = iter.Next();

			if( pInfo->HasStyle( P2P_STYLE_PLAY ) )
				SendP2PPacket( pInfo->m_addrPublic.m_IP, pInfo->m_addrPublic.m_Port, rkPacket );
		}
	}
}

void ioApplication::SendToTeam( SP2Packet &rkPacket , TeamType eSendTeam)
{
	if( !m_pNetworkTable )	return;

	ioPlayStage *pPlayState = ToPlayStage( m_pGameStage );
	if( !pPlayState ) return;

	ioUserNetworkTable::NetworkInfoIter iter = m_pNetworkTable->GetNetworkInfoIter();
	while( iter.HasMoreElements() )
	{
		UserNetworkInfo *pInfo = iter.Next();
		
		ioBaseChar *pBaseChar = pPlayState->GetBaseChar( pInfo->m_Name );
		if( !pBaseChar ) continue;
		if( !pInfo->HasStyle( P2P_STYLE_PLAY ) ) continue;
		if( eSendTeam == pBaseChar->GetTeam() )
		{
			SendP2PPacket( pInfo->m_addrPublic.m_IP, pInfo->m_addrPublic.m_Port, rkPacket );
		}
	}
}

void ioApplication::SendToUser( const ioHashString &rkName, SP2Packet &rkPacket )
{
	if( !m_pNetworkTable )	return;

	UserNetworkInfo *pInfo = m_pNetworkTable->GetNetworkInfo( rkName );
	if( pInfo )
	{
		if( pInfo->HasStyle( P2P_STYLE_PLAY ) )
			SendP2PPacket( pInfo->m_addrPublic.m_IP, pInfo->m_addrPublic.m_Port, rkPacket );
	}
	else
	{
		LOG.PrintTimeAndLog( 0, "ioApplication::SendToUser - %s Not Exist Info", rkName.c_str() );
	}
}

void ioApplication::SendToUserByIndex( const DWORD dwUserIndex, SP2Packet &rkPacket )
{
	if( !m_pNetworkTable )	return;

	UserNetworkInfo *pInfo = m_pNetworkTable->GetNetworkInfo( dwUserIndex );
	if( pInfo )
	{
		SendP2PPacket( pInfo->m_addrPublic.m_IP, pInfo->m_addrPublic.m_Port, rkPacket );
	}
	else
	{
		LOG.PrintTimeAndLog( 0, "ioApplication::SendToUserByIndex - %d Not Exist Info", dwUserIndex);
	}
}

void ioApplication::SendToHost( SP2Packet &rkPacket )
{
	if( !m_pNetworkTable )	return;	

	UserNetworkInfo *pInfo = m_pNetworkTable->GetNetworkInfo( m_pMyInfo->GetHostName() );
	if( pInfo )
	{
		SendP2PPacket( pInfo->m_addrPublic.m_IP, pInfo->m_addrPublic.m_Port, rkPacket );
	}
	else
	{
		LOG.PrintTimeAndLog( 0, "ioApplication::SendToHost - Host Not Exist" );
	}
}

void ioApplication::SendP2PUserInfoSync( const ioHashString &rkName, SP2Packet &rkPacket )
{
	if( !m_pNetworkTable )	return;
	if( m_pNetworkTable->IsServerRelay( rkName ) ) return;         //���� �߰� ������ ��Ŷ�� ������ ����.

	UserNetworkInfo *pInfo = m_pNetworkTable->GetNetworkInfo( rkName );
	if( pInfo )
	{
		SendP2PPacket( pInfo->m_addrPublic.m_IP, pInfo->m_addrPublic.m_Port, rkPacket );
	}
}

void ioApplication::SendToRelaySelf( SP2Packet &rkPacket )
{
	if( m_bNetworkPlay && m_pUDPSocket )
	{


#ifdef _DEBUG
		PrintPacketID( rkPacket.GetPacketID() );
#endif

		g_P2PChecker.AddSendPacket( rkPacket.GetPacketID() );

		if( Help::CheckEnableTimeGapUser(2) )
		{
			SetTestSendData( rkPacket );
		}


		const ioHashString pIP = g_MyInfo.GetMyIP();
		if(pIP.IsEmpty() == false)
		{
			char strIP[64] = {0,};
			strncpy_s(strIP, 64, pIP.c_str(), pIP.Length());
			int port = g_MyInfo.GetMyPort();

			SP2Packet kPacket( strIP,
				port,
				g_MyInfo.GetUserIndex(),
				m_iNextUDPSendIndex,
				rkPacket );

			CheckUDPData( kPacket );
			SendUDPMessageToServer(kPacket);
			m_iNextUDPSendIndex++;
		}
	}
}

void ioApplication::SendToLogServer( SP2Packet &rkPacket )
{
	if( m_bNetworkPlay && m_pUDPSocket )
	{
		if( !m_LogServerIP.IsEmpty() && m_iLogPort != 0 )
		{
			// �������� ������ UDP��Ŷ���� �ε����� �ο����� ����
			g_P2PChecker.AddSendPacket( rkPacket.GetPacketID() );

			if( Help::CheckEnableTimeGapUser(2) )
			{
				SetTestSendData( rkPacket );
			}

			#ifdef _DEBUG
			PrintPacketID( rkPacket.GetPacketID() );
			#endif
			m_pUDPSocket->SendLoop( m_LogServerIP.c_str(), m_iLogPort, rkPacket );
		}
	}
}

void ioApplication::SendUDPToServer( SP2Packet &rkPacket )
{
	if( m_pTCPSocket && m_pUDPSocket )
	{
		// �������� ������ UDP��Ŷ���� �ε����� �ο����� ����
		g_P2PChecker.AddSendPacket( rkPacket.GetPacketID() );

		if( Help::CheckEnableTimeGapUser(2) )
		{
			SetTestSendData( rkPacket );
		}

		#ifdef _DEBUG
		PrintPacketID( rkPacket.GetPacketID() );
		#endif
 
		SendUDPMessageToServer(rkPacket);
	}
}

void ioApplication::SendRoomStealthEnter( const ioHashString &rkSyncName )
{
	SP2Packet kPacket( CTPK_ROOM_STEALTH_ENTER );
	kPacket << rkSyncName;
	TCPNetwork::SendToServer( kPacket );
	TCPNetwork::MouseBusy( true );
}

bool ioApplication::IsTCPConnected() const
{
	if( m_pTCPSocket )
		return m_pTCPSocket->IsConnected();
 
	return false;
}

void ioApplication::SendTCPPacket( SP2Packet &rkPacket )
{
	if( m_bNetworkPlay && m_pTCPSocket )
	{
		if( rkPacket.GetPacketID() == CTPK_LOGOUT )
		{
			LOG.PrintTimeAndLog( 0 , "CTPK_LOGOUT Send" );
		}

		if( Help::CheckEnableTimeGapUser(2) )
		{
			SetTestSendData( rkPacket );
		}

		m_Mouse.SetPacketID( rkPacket.GetPacketID() );
		m_pTCPSocket->SendLoop( rkPacket );
	}
}

bool ioApplication::FirstUDPPacketParse( SP2Packet &rkPacket, const char *szRcvIP, int iRcvPort, bool bHeaderParsing )
{
	
	if( rkPacket.GetPacketID() >= SUPK_CONNECT )
		return true;        //�������� ������ ��Ŷ�� IP / PORT / UserIndex ����

	// IP�� Port�� ������ �ּ��̸� UserIndex�� ���� ������ �ε����̴�.
	// UDPIndex : Ŭ���̾�Ʈ�� �޼��� �ߺ� üũ��	
	int iUDPIndex;
	DWORD dwIP, dwPort, dwUserIndex;
	rkPacket >> dwIP >> dwPort >> dwUserIndex >> iUDPIndex;
	if( dwUserIndex == 0 )
	{
		LOG.PrintTimeAndLog( 0, "ioApplication::FirstUDPPacketParse UserIndex Zero!" );
		return true;
	}

	if( bHeaderParsing )
		return false;     // ����� ��������.
	
	/*// ping test	
	if( g_MyInfo.GetUserIndex() == dwUserIndex )
	{
		if( ParseUDPData( iUDPIndex, rkPacket.GetPacketID() ) )
			return false;
	}*/
	
	UserNetworkInfo *pInfo = m_pNetworkTable->GetNetworkInfo( dwUserIndex );
	if( pInfo )
	{
		// �����ǿ� ��Ʈ�� ���� ���õǾ������� ���� �߰�!!
		if( dwIP != 0 && dwPort != 0 )
		{			
			return true;
		}

		pInfo->m_dwLastRecvTime = REALGETTIME();   // ���������� UDP ��ȣ ���� �ð�.
		pInfo->m_dwOnlyP2PRecvTime = REALGETTIME();
		// IP�� ��Ʈ ���� �ϳ��ٷ� �ٸ��� ��Ʈ ü����
		if( strcmp( pInfo->m_addrP2pHole.m_IP, szRcvIP ) != 0 || pInfo->m_addrP2pHole.m_Port != iRcvPort )
		{
			ClientToClientPortChange( pInfo->m_Name, szRcvIP, iRcvPort );
		}

		// ��Ŷ�ߺ� �˻�
		if( m_pNetworkTable->CheckDuplicateIndex( pInfo, iUDPIndex ) )
		{
			LOG.PrintTimeAndLog( 0, "ioApplication::FirstUDPPacketParse : %x Packet(%s:%d) Duplicated(%d)",
									rkPacket.GetPacketID(),
									szRcvIP,
									iRcvPort,
									iUDPIndex );
			return false;
		}
	}
	/*
	else
	{	
		static std::vector<DWORD> sVecUserIndex;
		sVecUserIndex.reserve( 100 );
		int nSize = sVecUserIndex.size();
		for( int i = 0; i < nSize; ++i )
		{
			if( dwUserIndex == sVecUserIndex[i] )
				return true;
		}
		sVecUserIndex.push_back( dwUserIndex );

		// ���� ������ ���� ������ ���� ��Ŷ�� �α� �ȳ��⵵�� ����.
		if( g_MyInfo.GetUserIndex() != dwUserIndex )
			LOG.PrintTimeAndLog( 0, "ioApplication::FirstUDPPacketParse Not User Index:%d, ID: %x", dwUserIndex, rkPacket.GetPacketID() );
		
		if( nSize > 100 )
			sVecUserIndex.clear();
	}
	*/

	return true;
}

void ioApplication::RecvUDPPacket( SP2Packet &rkPacket, const char *szRcvIP, int iRcvPort )
{	
	if( !m_bNetworkPlay )	return;

#ifndef _DEBUG
	__try
#endif
	{
		if( !m_bSetUpDone )
		{
			DWORD dwPacketID = rkPacket.GetPacketID();
			if( dwPacketID != SUPK_CONNECT )	// SetUp���� ó���Ǿ��ϴ� ��Ŷ�� �� ���ǿ� �߰�
			{
				m_UDPBeforeSetUpList.push_back( rkPacket );
				return;
			}
		}

		#ifdef _DEBUG
		PrintPacketID( rkPacket.GetPacketID() , false , szRcvIP , iRcvPort );
		#endif

		// UDP ��Ŷ�� ���� �ð��� �����Ѵ�.
		UpdateAllUDPRecvTime(); 

		if( FirstUDPPacketParse( rkPacket, szRcvIP, iRcvPort ) )
		{
			if( Help::CheckEnableTimeGapUser(2) )
			{
				SetTestReciveData( rkPacket );
			}

			if( Help::CheckEnableTimeGapUser() && rkPacket.GetBufferSize() > 400 )
			{
				LOG.PrintTimeAndLog( 0, "PS - Recv UDPPacketSize : %x, %d", rkPacket.GetPacketID(), rkPacket.GetBufferSize() );
			}

			if( !ProcessUDPPacket( rkPacket ) )
			{
				if( m_pGameStage )
				{
					//
					bool bCheckChar = Help::CheckEnableTimeGapUser();

					DWORD dwStartTime = 0;
					DWORD dwGapTime = 0;
					//
					if( bCheckChar )
						dwStartTime = timeGetTime();

					m_pGameStage->ProcessUDPPacket( rkPacket );

					if( bCheckChar )
					{
						dwGapTime = timeGetTime() - dwStartTime;
						m_dwTestReciveTime += dwGapTime;
					}
				}
			}
		}
	}
#ifndef _DEBUG
	__except( ExceptCallBack( GetExceptionInformation() ) )
	{
	
		m_bProgramExit = true;
	}
#endif
}

void ioApplication::RecvTCPPacket( SP2Packet &rkPacket )
{
	if( !m_bNetworkPlay )	return;

#ifndef _DEBUG
	__try
#endif
	{
		g_TCPChecker.AddRecvPacket( rkPacket.GetPacketID() );
		if( !m_bSetUpDone )
		{
			switch( rkPacket.GetPacketID() )
			{
			// SetUp���� ó���Ǿ��ϴ� ��Ŷ�� ���⿡ �߰�
			case STPK_CONNECT:
			case STPK_SLOT_ITEM:
			case STPK_CHAR_ALL_LOAD:
			case STPK_CLASSEXPERT_SET:
			case STPK_CLASSPRICE:
			case STPK_AWARD_DATA:
			case STPK_USER_RECORD_DATA:
			case STPK_MY_GUILD_INFO:
			case STPK_ENTRY_DELAY_GUILD_LIST:
			case STPK_CONTROL_KEYS:
			case STPK_DISCONNECT_ALREADY_ID:
			case STPK_SHUT_DOWN_USER:
			case STPK_SELECT_SHUT_DOWN_USER:
				break;
			default:
				m_TCPBeforeSetUpList.push_back( rkPacket );
				return;
			}
		}

		#ifdef _DEBUG
		PrintPacketID( rkPacket.GetPacketID(), false );
		#endif

		if( !ProcessUDPPacketRecvTCP( rkPacket ) )
		{
			if( Help::CheckEnableTimeGapUser(2) )
			{
				SetTestReciveData( rkPacket );
			}

			if( Help::CheckEnableTimeGapUser() && rkPacket.GetBufferSize() > 400 )
			{
				LOG.PrintTimeAndLog( 0, "PS - Recv TCPPacketSize : %x, %d", rkPacket.GetPacketID(), rkPacket.GetBufferSize() );
			}

			if( !ProcessTCPPacket( rkPacket ) )
			{
				//
				bool bCheckChar = Help::CheckEnableTimeGapUser();

				DWORD dwStartTime = 0;
				DWORD dwGapTime = 0;
				//
				if( bCheckChar )
					dwStartTime = timeGetTime();

				if( m_pGameStage )
					m_pGameStage->ProcessTCPPacket( rkPacket );

				if( bCheckChar )
				{
					dwGapTime = timeGetTime() - dwStartTime;
					m_dwTestReciveTime += dwGapTime;
				}

				ChangeStage();
			}
		}
	}
#ifndef _DEBUG
	__except( ExceptCallBack( GetExceptionInformation() ) )
	{
		m_bProgramExit = true;
	}
#endif
}

bool ioApplication::ProcessUDPPacketRecvTCP( SP2Packet &rkPacket )
{
	if( !Help::IsOnlyServerRelay() ) return false;
	if( !COMPARE( rkPacket.GetPacketID(), CUPK_CONNECT, 0x5000 ) ) return false;

	if( !m_bNetworkPlay )	return false;

	// 100% ���� �߰�
#ifndef _DEBUG
	__try
#endif
	{

#ifdef _DEBUG
		PrintPacketID( rkPacket.GetPacketID() , false , "" , 0 );
#endif

		if( !ProcessUDPPacket( rkPacket ) )
		{
			if( m_pGameStage )
			{
				m_pGameStage->ProcessUDPPacket( rkPacket );
			}
		}
	}
#ifndef _DEBUG
	__except( ExceptCallBack( GetExceptionInformation() ) )
	{
		m_bProgramExit = true;
	}
#endif
	return true;
}

bool ioApplication::ProcessTCPPacket( SP2Packet &rkPacket )
{
	ErrorReport::SetPosition( 1000, 9000 );

	switch( rkPacket.GetPacketID() )
	{
	case STPK_CONNECT:
		ErrorReport::SetPosition( 1000, 9001 );
		if( m_pLogin )
		    m_pLogin->OnConnect( rkPacket );
		ErrorReport::SetPosition( 1000, 9002 );
		return true;
	case STPK_CLASSPRICE:
		ErrorReport::SetPosition( 1000, 9004 );
		OnClassPrice( rkPacket );
		ErrorReport::SetPosition( 1000, 9005 );
		return true;
	case STPK_CHAR_CREATE:
		ErrorReport::SetPosition( 1000, 9006 );
		OnCharCreate( rkPacket );
		ErrorReport::SetPosition( 1000, 9007 );
		return true;
	case STPK_CHAR_LOAD:
		ErrorReport::SetPosition( 1000, 9008 );
		OnCharLoad( rkPacket );
		ErrorReport::SetPosition( 1000, 9009 );
		return true;
	case STPK_CHAR_ALL_LOAD:
		ErrorReport::SetPosition( 1000, 9010 );
		OnCharAllLoad( rkPacket );
		ErrorReport::SetPosition( 1000, 9011 );
		return true;
	case STPK_CLASSEXPERT_SET:
		ErrorReport::SetPosition( 1000, 9012 );
		OnClassExpert( rkPacket );
		ErrorReport::SetPosition( 1000, 9013 );
		return true;
	case STPK_SLOT_ITEM:
		ErrorReport::SetPosition( 1000, 9014 );
		OnInventorySlot( rkPacket );
		ErrorReport::SetPosition( 1000, 9015 );
		return true;
	case STPK_BUY_ITEM:
		ErrorReport::SetPosition( 1000, 9016 );
		OnBuyItem( rkPacket );
		ErrorReport::SetPosition( 1000, 9017 );
		return true;
	case STPK_EQUIP_SLOT_ITEM:
		ErrorReport::SetPosition( 1000, 9018 );
		OnEquipSlotItem( rkPacket );
		ErrorReport::SetPosition( 1000, 9019 );
		return true;
	case STPK_MOVE_SLOT_ITEM:
		ErrorReport::SetPosition( 1000, 9020 );
		OnMoveSlotItem( rkPacket );
		ErrorReport::SetPosition( 1000, 9021 );
		break;
	case STPK_SELL_SLOT_ITEM:
		ErrorReport::SetPosition( 1000, 9024 );
		OnSellSlotItem( rkPacket );
		ErrorReport::SetPosition( 1000, 9025 );
		return true;
	case STPK_SELL_EQUIP_ITEM:
		ErrorReport::SetPosition( 1000, 9026 );
		OnSellEquipItem( rkPacket );
		ErrorReport::SetPosition( 1000, 9027 );
		return true;
	case STPK_INC_STAT:
		ErrorReport::SetPosition( 1000, 9028 );
		OnIncreseStat( rkPacket );
		ErrorReport::SetPosition( 1000, 9029 );
		return true;
	case STPK_INIT_STAT:
		ErrorReport::SetPosition( 1000, 9030 );
		OnInitStat( rkPacket );
		ErrorReport::SetPosition( 1000, 9031 );
		return true;
	case STPK_LEVELUP_ITEM:
		ErrorReport::SetPosition( 1000, 9032 );
		OnLevelUpItem( rkPacket );
		ErrorReport::SetPosition( 1000, 9033 );
		return true;
	case STPK_CHAR_DELETE:
		ErrorReport::SetPosition( 1000, 9034 );
		OnCharDelete( rkPacket );
		ErrorReport::SetPosition( 1000, 9035 );
		return true;
	case STPK_CHAR_LIMIT_CHECK:
		ErrorReport::SetPosition( 1000, 9054 );
		OnCharLimitCheck( rkPacket );
		ErrorReport::SetPosition( 1000, 9055 );
		return true;
	case STPK_CHAR_DECORATION_BUY:
		ErrorReport::SetPosition( 1000, 9056 );
		OnCharDecorationBuy( rkPacket );
		ErrorReport::SetPosition( 1000, 9057 );
		return true;
	case STPK_CHAR_EXTEND:
		ErrorReport::SetPosition( 1000, 9058 );
		OnCharExtend( rkPacket );
		ErrorReport::SetPosition( 1000, 9059 );
		return true;	
	case STPK_ANNOUNCE:
		ErrorReport::SetPosition( 1000, 9060 );
		OnAnnounce( rkPacket );
		ErrorReport::SetPosition( 1000, 9061 );
		return true;
	case STPK_LOGOUT:
		ErrorReport::SetPosition( 1000, 9062 );
		OnLogOut( rkPacket );
		ErrorReport::SetPosition( 1000, 9063 );
		return true;
	case STPK_RESERVE_LOGOUT:
		ErrorReport::SetPosition( 1000, 9064 );
		OnReserveLogOut( rkPacket );
		ErrorReport::SetPosition( 1000, 9065 );
		return true;
	case STPK_MEMO_SEND_MSG:
		ErrorReport::SetPosition( 1000, 9070 );
		OnMemoSendMsg( rkPacket );
		ErrorReport::SetPosition( 1000, 9071 );
		return true;
	case STPK_OFFLINE_MEMO_MSG:
		ErrorReport::SetPosition( 1000, 9072 );
		OnOfflineMemo( rkPacket );
		ErrorReport::SetPosition( 1000, 9073 );
		return true;
	case STPK_FRIEND_LIST_MSG:
		ErrorReport::SetPosition( 1000, 9074 );
		OnFriendList( rkPacket );
		ErrorReport::SetPosition( 1000, 9075 );
		return true;
	case STPK_FRIEND_COMMAND:
		ErrorReport::SetPosition( 1000, 9076 );
		OnFriendCommand( rkPacket );
		ErrorReport::SetPosition( 1000, 9077 );
		return true;
	case STPK_FRIEND_DELETE:
		ErrorReport::SetPosition( 1000, 9078 );
		OnFriendDelete( rkPacket );
		ErrorReport::SetPosition( 1000, 9079 );
		return true;
	case STPK_USER_LOGIN:
		ErrorReport::SetPosition( 1000, 9080 );
		OnUserLogin( rkPacket );
		ErrorReport::SetPosition( 1000, 9081 );
		return true;
	case STPK_CHANNEL_CREATE:
		ErrorReport::SetPosition( 1000, 9082 );
		OnChannelCreate( rkPacket );
		ErrorReport::SetPosition( 1000, 9083 );
		return true;
	case STPK_CHANNEL_INVITE:
		ErrorReport::SetPosition( 1000, 9084 );
		OnChannelInvite( rkPacket );		
		ErrorReport::SetPosition( 1000, 9085 );
		return true;
	case STPK_CHANNEL_JOIN:
		ErrorReport::SetPosition( 1000, 9086 );
		OnChannelJoin( rkPacket );
		ErrorReport::SetPosition( 1000, 9087 );
		return true;
	case STPK_CHANNEL_LEAVE:
		ErrorReport::SetPosition( 1000, 9088 );
		OnChannelLeave( rkPacket );
		ErrorReport::SetPosition( 1000, 9089 );
		return true;
	case STPK_CHANNEL_CHAT:
		ErrorReport::SetPosition( 1000, 9090 );
		OnChannelChat( rkPacket );
		ErrorReport::SetPosition( 1000, 9091 );
		return true;
	case STPK_CHANNEL_ALL_USER:
		ErrorReport::SetPosition( 1000, 9092 );
		OnChannelAllUser( rkPacket );
		ErrorReport::SetPosition( 1000, 9093 );
		return true;
	case STPK_REGISTERED_USER:
		ErrorReport::SetPosition( 1000, 9094 );
		OnRegisteredUser( rkPacket );
		ErrorReport::SetPosition( 1000, 9095 );
		return true;
	case STPK_HACK_ANNOUNCE:
		ErrorReport::SetPosition( 1000, 9096 );
		OnHackAnnounce( rkPacket );
		ErrorReport::SetPosition( 1000, 9097 );
		return true;
	case STPK_HACK_QUIZ:
		ErrorReport::SetPosition( 1000, 9098 );
		OnHackQuiz( rkPacket );
		ErrorReport::SetPosition( 1000, 9099 );
		return true;
	case STPK_ABUSE_ANNOUNCE:
		ErrorReport::SetPosition( 1000, 9100 );
		OnAbuseAnnounce( rkPacket );
		ErrorReport::SetPosition( 1000, 9101 );
		return true;
	case STPK_ABUSE_QUIZ:
		ErrorReport::SetPosition( 1000, 9102 );
		OnAbuseQuiz( rkPacket );
		ErrorReport::SetPosition( 1000, 9103 );
		return true;
	case STPK_TUTORIAL_STEP:
		ErrorReport::SetPosition( 1000, 9104 );
		OnTutorialStepBonus( rkPacket );
		ErrorReport::SetPosition( 1000, 9105 );
		return true;
	case STPK_EXIT_ROOM_PENALTY:
		ErrorReport::SetPosition( 1000, 9108 );
		OnExitRoomPenalty( rkPacket );
		ErrorReport::SetPosition( 1000, 9109 );
		return true;
	case STPK_USER_POS_REFRESH:
		ErrorReport::SetPosition( 1000, 9110 );
		OnUserPosRefresh( rkPacket );
		ErrorReport::SetPosition( 1000, 9111 );
		return true;
	case STPK_AWARD_DATA:
		ErrorReport::SetPosition( 1000, 9112 );
		OnAwardData( rkPacket );
		ErrorReport::SetPosition( 1000, 9113 );
		return true;
	case STPK_USER_INFO_REFRESH:
		ErrorReport::SetPosition( 1000, 9116 );
		OnUserInfoRefresh( rkPacket );
		ErrorReport::SetPosition( 1000, 9117 );
		return true;
	case STPK_DEVELOPER_MACRO:
		ErrorReport::SetPosition( 1000, 9118 );
		OnDeveloperMacro( rkPacket );
		ErrorReport::SetPosition( 1000, 9119 );
		return true;
	case STPK_GRADE_CLASS_UP_BONUS:
		ErrorReport::SetPosition( 1000, 9120 );
		OnGradeNClassUpBonus( rkPacket );
		ErrorReport::SetPosition( 1000, 9121 );
		return true;
	case STPK_MOVING_SERVER:
		ErrorReport::SetPosition( 1000, 9122 );
		OnMovingServer( rkPacket );
		ErrorReport::SetPosition( 1000, 9123 );
		return true;
	case STPK_MOVING_SERVER_COMPLETE:
		ErrorReport::SetPosition( 1000, 9124 );
		OnMovingServerComplete( rkPacket );
		ErrorReport::SetPosition( 1000, 9125 );
		return true;
	case STPK_RESERVE_ROOM_JOIN:
		ErrorReport::SetPosition( 1000, 9126 );
		OnReserverRoomJoin( rkPacket );
		ErrorReport::SetPosition( 1000, 9127 );
		return true;
	case STPK_EXERCISE_EVENT_CHAR_CREATE:
		ErrorReport::SetPosition( 1000, 9128 );
		OnExerciseCharCreate( rkPacket );
		ErrorReport::SetPosition( 1000, 9129 );
		return true;
	case STPK_BANKRUPTCY_PESO:
		ErrorReport::SetPosition( 1000, 9130 );
		OnBankruptcyPeso( rkPacket );
		ErrorReport::SetPosition( 1000, 9131 );
		return true;
	case STPK_RESERVE_SERVER_MOVE:
		ErrorReport::SetPosition( 1000, 9132 );
		OnReserveServerMoving( rkPacket );
		ErrorReport::SetPosition( 1000, 9133 );
		return true;
	case STPK_TOTAL_REG_USER_CNT:
		ErrorReport::SetPosition( 1000, 9134 );
		OnTotalRegUserCount( rkPacket );
		ErrorReport::SetPosition( 1000, 9135 );
		return true;
	case STPK_START_CHAR_LIMITDATE:
		ErrorReport::SetPosition( 1000, 9136 );
		OnStartCharLimitDate( rkPacket );
		ErrorReport::SetPosition( 1000, 9137 );
		return true;
	case STPK_UPDATE_CHAR_LIMITDATE:
		ErrorReport::SetPosition( 1000, 9138 );
		OnUpdatecharLimitDate( rkPacket );
		ErrorReport::SetPosition( 1000, 9139 );
		return true;
	case STPK_CHAR_CHARGE:
		ErrorReport::SetPosition( 1000, 9140 );
		OnCharCharge( rkPacket );
		ErrorReport::SetPosition( 1000, 9141 );
		return true;	
	case STPK_EVENT_DATA:
		ErrorReport::SetPosition( 1000, 9142 );
		OnEventData( rkPacket );
		ErrorReport::SetPosition( 1000, 9143 );
		return true;	
	case STPK_USER_RECORD_DATA:
		ErrorReport::SetPosition( 1000, 9144 );
		OnUserRecordData( rkPacket );
		ErrorReport::SetPosition( 1000, 9145 );
		return true;
	case STPK_CREATE_GUILD:
		ErrorReport::SetPosition( 1000, 9146 );
		OnGuildCreate( rkPacket );
		ErrorReport::SetPosition( 1000, 9147 );
		return true;
	case STPK_MY_GUILD_INFO:
		ErrorReport::SetPosition( 1000, 9148 );
		OnMyGuildInfo( rkPacket );
		ErrorReport::SetPosition( 1000, 9149 );
		return true;
	case STPK_ENTRY_DELAY_GUILD_LIST:
		ErrorReport::SetPosition( 1000, 9150 );
		OnEntryDelayGuildLIst( rkPacket );
		ErrorReport::SetPosition( 1000, 9151 );
		return true;
	case STPK_GUILD_ENTRY_DELAY_MEMBER:
		ErrorReport::SetPosition( 1000, 9152 );
		OnGuildEntryDelayMember( rkPacket );
		ErrorReport::SetPosition( 1000, 9153 );
		return true;
	case STPK_GUILD_INFO:
		ErrorReport::SetPosition( 1000, 9154 );
		OnGuildInfo( rkPacket );
		ErrorReport::SetPosition( 1000, 9155 );
		return true;
	case STPK_GUILD_MEMBER_LIST:
		ErrorReport::SetPosition( 1000, 9156 );
		OnGuildMemberList( rkPacket );
		ErrorReport::SetPosition( 1000, 9157 );
		return true;
	case STPK_GUILD_JOINER_CHANGE:
		ErrorReport::SetPosition( 1000, 9158 );
		OnGuildJoinerChange( rkPacket );
		ErrorReport::SetPosition( 1000, 9159 );
		return true;
	case STPK_GUILD_ENTRY_APP:
		ErrorReport::SetPosition( 1000, 9160 );
		OnGuildEntryApp( rkPacket );
		ErrorReport::SetPosition( 1000, 9161 );
		return true;
	case STPK_GUILD_ENTRY_AGREE:
		ErrorReport::SetPosition( 1000, 9162 );
		OnGuildEntryAgree( rkPacket );
		ErrorReport::SetPosition( 1000, 9163 );
		return true;
	case STPK_ENTRY_DELAY_GUILD_DELETE:
		ErrorReport::SetPosition( 1000, 9164 );
		OnGuildEntryGuildDelete( rkPacket );
		ErrorReport::SetPosition( 1000, 9165 );
		return true;
	case STPK_GUILD_INVITATION:
		ErrorReport::SetPosition( 1000, 9166 );
		OnGuildInvitation( rkPacket );
		ErrorReport::SetPosition( 1000, 9167 );
		return true;
	case STPK_GUILD_INVITED:
		ErrorReport::SetPosition( 1000, 9168 );
		OnGuildInvited( rkPacket );
		ErrorReport::SetPosition( 1000, 9169 );
		return true;
	case STPK_GUILD_LEAVE:
		ErrorReport::SetPosition( 1000, 9170 );
		OnGuildLeave( rkPacket );
		ErrorReport::SetPosition( 1000, 9171 );
		return true;
	case STPK_GUILD_MASTER_CHANGE:
		ErrorReport::SetPosition( 1000, 9172 );
		OnGuildMasterChange( rkPacket );
		ErrorReport::SetPosition( 1000, 9173 );
		return true;
	case STPK_GUILD_POSITION_CHANGE:
		ErrorReport::SetPosition( 1000, 9174 );
		OnGuildPositionChange( rkPacket );
		ErrorReport::SetPosition( 1000, 9175 );
		return true;
	case STPK_GUILD_KICK_OUT:
		ErrorReport::SetPosition( 1000, 9176 );
		OnGuildKickOut( rkPacket );
		ErrorReport::SetPosition( 1000, 9177 );
		return true;
	case STPK_GUILD_SIMPLE_DATA:
		ErrorReport::SetPosition( 1000, 9178 );
		OnGuildSimpleData( rkPacket );
		ErrorReport::SetPosition( 1000, 9179 );
		return true;
	case STPK_FRIEND_REQUEST_LIST:
		ErrorReport::SetPosition( 1000, 9180 );
		OnFriendRequestList( rkPacket );
		ErrorReport::SetPosition( 1000, 9181 );
		return true;
	case STPK_FRIEND_APPLICATION:
		ErrorReport::SetPosition( 1000, 9182 );
		OnFriendApplication( rkPacket );
		ErrorReport::SetPosition( 1000, 9183 );
		return true;
	case STPK_USER_ENTRY_REFRESH:
		ErrorReport::SetPosition( 1000, 9184 );
		OnUserEntryRefresh( rkPacket );
		ErrorReport::SetPosition( 1000, 9185 );
		return true;
	case STPK_GUILD_MARK_CHANGE:
		ErrorReport::SetPosition( 1000, 9186 );
		OnGuildMarkChange( rkPacket );
		ErrorReport::SetPosition( 1000, 9187 );
		return true;
	case STPK_MYGUILD_MEMBER_LIST:
		ErrorReport::SetPosition( 1000, 9188 );
		OnMyGuildMemberList( rkPacket );
		ErrorReport::SetPosition( 1000, 9189 );
		return true;
	case STPK_MYGUILD_LEAVE_LIST:
		ErrorReport::SetPosition( 1000, 9190 );
		OnMyGuildLeaveList( rkPacket );
		ErrorReport::SetPosition( 1000, 9191 );
		return true;
	case STPK_GUILD_CHAT:
		ErrorReport::SetPosition( 1000, 9192 );
		OnGuildChat( rkPacket );
		ErrorReport::SetPosition( 1000, 9193 );
		return true;
	case STPK_GUILD_EXIST:
		ErrorReport::SetPosition( 1000, 9194 );
		OnGuildExist( rkPacket );
		ErrorReport::SetPosition( 1000, 9195 );
		return true;
	case STPK_USER_INFO_EXIST:
		ErrorReport::SetPosition( 1000, 9196 );
		OnUserInfoExist( rkPacket );
		ErrorReport::SetPosition( 1000, 9197 );
		return true;
	case STPK_GUILD_MARK_KEY_VALUE:
		ErrorReport::SetPosition( 1000, 9198 );
		OnGuildMarkChangeKeyValue( rkPacket );
		ErrorReport::SetPosition( 1000, 9199 );
		return true;
	case STPK_USER_ETC_ITEM:
		ErrorReport::SetPosition( 1000, 9200 );
		OnUserEtcItem( rkPacket );
		ErrorReport::SetPosition( 1000, 9201 );
		return true;
	case STPK_SERVICE_CHAR:
		ErrorReport::SetPosition( 1000, 9202 );
		OnServiceChar( rkPacket );
		ErrorReport::SetPosition( 1000, 9203 );
		return true;
	case STPK_PRESENT_DATA:
		ErrorReport::SetPosition( 1000, 9204 );
		OnPresentData( rkPacket );
		ErrorReport::SetPosition( 1000, 9205 );
		return true;	
	case STPK_DEFAULT_DECO_ITEM:
		ErrorReport::SetPosition( 1000, 9206 );
		OnDefaultDecoItem( rkPacket );
		ErrorReport::SetPosition( 1000, 9207 );
		return true;
	case STPK_EVENT_DATA_UPDATE:
		ErrorReport::SetPosition( 1000, 9208 );
		OnEventDataUpdate( rkPacket );
		ErrorReport::SetPosition( 1000, 9209 );
		return true;
	case STPK_GUILD_TITLE_SYNC:
		ErrorReport::SetPosition( 1000, 9210 );
		OnGuildTitleSync( rkPacket );
		ErrorReport::SetPosition( 1000, 9211 );
		return true;
	case STPK_PROTECT_CHECK:
		ErrorReport::SetPosition( 1000, 9214 );
		OnProtectCheck( rkPacket );
		ErrorReport::SetPosition( 1000, 9215 );
		return true;
	case STPK_GET_CASH:
		ErrorReport::SetPosition( 1000, 9216 );
		OnGetCash( rkPacket );
		ErrorReport::SetPosition( 1000, 9217 );
		return true;
	case STPK_HOLE_SEND_COMPLETE:
		ErrorReport::SetPosition( 1000, 9218 );
		OnHoleSendComplete( rkPacket );
		ErrorReport::SetPosition( 1000, 9219 );
		return true;
	case STPK_UDP_RECV_TIMEOUT:
		ErrorReport::SetPosition( 1000, 9220 );
		OnUDPRecvTimeOut( rkPacket );
		ErrorReport::SetPosition( 1000, 9221 );
		return true;
	case STPK_GROWTH_LEVEL_FIRST:
		ErrorReport::SetPosition( 1000, 9222 );
		OnGrowthLevelData( rkPacket );
		ErrorReport::SetPosition( 1000, 9223 );
		return true;
	case STPK_EXERCISE_CHAR_DELETE:
		ErrorReport::SetPosition( 1000, 9222 );
		OnExerciseCharDelete( rkPacket );
		ErrorReport::SetPosition( 1000, 9223 );
		return true;
	case STPK_WHOLE_CHAT:
		ErrorReport::SetPosition( 1000, 9224 );
		OnWholeServerChat( rkPacket );
		ErrorReport::SetPosition( 1000, 9225 );
		return true;
	case STPK_RAINBOW_WHOLE_CHAT:
		ErrorReport::SetPosition( 1000, 9224 );
		OnWholeServerRainbowChat( rkPacket );
		ErrorReport::SetPosition( 1000, 9225 );
		return true;
	case STPK_START_ETC_ITEM_TIME:
		ErrorReport::SetPosition( 1000, 9226 );
		OnStartEtcItemTime( rkPacket );
		ErrorReport::SetPosition( 1000, 9227 );
		return true;
	case STPK_UPDATE_ETC_ITEM_TIME:
		ErrorReport::SetPosition( 1000, 9228 );
		OnUpdateEtcItemTime( rkPacket );
		ErrorReport::SetPosition( 1000, 9229 );
		return true;
	case STPK_CHAR_CHANGE_PERIOD:
		ErrorReport::SetPosition( 1000, 9230 );
		OnCharChangePeriod( rkPacket );
		ErrorReport::SetPosition( 1000, 9231 );
		return true;	
	case STPK_EVENT_ALIVE:
		ErrorReport::SetPosition( 1000, 9232 );
		OnEventAlive( rkPacket );
		ErrorReport::SetPosition( 1000, 9233 );
		return true;	
	case STPK_UDP_RECV_TIMEOUT_RECHECK:
		ErrorReport::SetPosition( 1000, 9234 );
		OnUDPRecvTimeOutReCheck( rkPacket );
		ErrorReport::SetPosition( 1000, 9235 );
		return true;
	case STPK_CHANGE_USER_NAME:
		ErrorReport::SetPosition( 1000, 9236 );
		OnChangeUserName( rkPacket );
		ErrorReport::SetPosition( 1000, 9237 );
		return true;
	case STPK_CAMP_DATA_SYNC:
		ErrorReport::SetPosition( 1000, 9236 );
		OnCampDataSync( rkPacket );
		ErrorReport::SetPosition( 1000, 9237 );
		return true;
	case STPK_CAMP_CAHNGE_POS:
		ErrorReport::SetPosition( 1000, 9238 );
		OnCampChangePos( rkPacket );
		ErrorReport::SetPosition( 1000, 9239 );
		return true;
	case STPK_USER_LADDER_POINT_RECORD_REFRESH:
		ErrorReport::SetPosition( 1000, 9240 );
		OnRefreshLadderPointNRecord( rkPacket );
		ErrorReport::SetPosition( 1000, 9241 );
		return true;
	case STPK_DELETE_ETC_ITEM_DATE:
		ErrorReport::SetPosition( 1000, 9242 );
		OnDeleteEtcItemDate( rkPacket );
		ErrorReport::SetPosition( 1000, 9243 );
		return true;
	case STPK_CAMP_SEASON_BONUS:
		ErrorReport::SetPosition( 1000, 9244 );
		OnCampSeasonBonus( rkPacket );
		ErrorReport::SetPosition( 1000, 9245 );
		return true;
	case STPK_BLOCK_TYPE:
		ErrorReport::SetPosition( 1000, 9246 );
		OnBlockType( rkPacket );
		ErrorReport::SetPosition( 1000, 9247 );
		return true;
	case STPK_GUILD_NAME_CHANGE:
		ErrorReport::SetPosition( 1000, 9248 );
		OnGuildNameChange( rkPacket );
		ErrorReport::SetPosition( 1000, 9249 );
		return true;
	case STPK_PRESENT_RECV:
		ErrorReport::SetPosition( 1000, 9250 );
		OnPresentRecv( rkPacket );
		ErrorReport::SetPosition( 1000, 9251 );
		return true;
	case STPK_PRESENT_SELL:
		ErrorReport::SetPosition( 1000, 9252 );
		OnPresentSell( rkPacket );
		ErrorReport::SetPosition( 1000, 9253 );
		return true;
	case STPK_USER_EXTRAITEM_DATA:
		ErrorReport::SetPosition( 1000, 9254 );
		OnExtraItemData( rkPacket );
		ErrorReport::SetPosition( 1000, 9255 );
		return true;
	case STPK_DELETE_EXTRAITEM_DATE:
		ErrorReport::SetPosition( 1000, 9256 );
		OnExtraItemDeleteByDate( rkPacket );
		ErrorReport::SetPosition( 1000, 9257 );
		return true;
	case STPK_FIRST_CHANGE_ID:
		ErrorReport::SetPosition( 1000, 9258 );
		OnFirstChangeID( rkPacket );
		ErrorReport::SetPosition( 1000, 9259 );
		return true;
	case STPK_TIME_GROWTH_CHECK:
		ErrorReport::SetPosition( 1000, 9260 );
		OnTimeGrowthCheck( rkPacket );
		ErrorReport::SetPosition( 1000, 9261 );
		return true;
	case STPK_CHECK_SALE_PRICE:
		ErrorReport::SetPosition( 1000, 9262 );
		OnCheckSalePrice( rkPacket );
		ErrorReport::SetPosition( 1000, 9263 );
		return true;
	case STPK_GASHAPON_LIST:
		ErrorReport::SetPosition( 1000, 9264 );
		OnGashaponList( rkPacket );
		ErrorReport::SetPosition( 1000, 9265 );
		return true;
	case STPK_QUEST_DATA:
		ErrorReport::SetPosition( 1000, 9266 );
		OnQuestData( rkPacket );
		ErrorReport::SetPosition( 1000, 9267 );
		return true;
	case STPK_QUEST_COMPLETE_DATA:
		ErrorReport::SetPosition( 1000, 9268 );
		OnQuestCompleteData( rkPacket );
		ErrorReport::SetPosition( 1000, 9269 );
		return true;
	case STPK_QUEST_OCCUR:
		ErrorReport::SetPosition( 1000, 9270 );
		OnQuestOccur( rkPacket );
		ErrorReport::SetPosition( 1000, 9271 );
		return true;
	case STPK_QUEST_ATTAIN:
		ErrorReport::SetPosition( 1000, 9272 );
		OnQuestAttain( rkPacket );
		ErrorReport::SetPosition( 1000, 9273 );
		return true;
	case STPK_QUEST_REWARD:
		ErrorReport::SetPosition( 1000, 9274 );
		OnQuestReward( rkPacket );
		ErrorReport::SetPosition( 1000, 9275 );
		return true;
	case STPK_QUEST_DELETE:
		ErrorReport::SetPosition( 1000, 9276 );
		OnQuestDelete( rkPacket );
		ErrorReport::SetPosition( 1000, 9277 );
		return true;
	case STPK_QUEST_EVENT_ALIVE:
		ErrorReport::SetPosition( 1000, 9278 );
		OnQuestEventAlive( rkPacket );
		ErrorReport::SetPosition( 1000, 9279 );
		return true;
	case STPK_USER_MEDALITEM_DATA:
		ErrorReport::SetPosition( 1000, 9280 );
		OnMedalItemData( rkPacket );
		ErrorReport::SetPosition( 1000, 9281 );
		return true;
	case STPK_DELETE_MEDALITEM_DATE:
		ErrorReport::SetPosition( 1000, 9282 );
		OnMedalItemDeleteByDate( rkPacket );
		ErrorReport::SetPosition( 1000, 9283 );
		return true;
	case STPK_CONTROL_KEYS:
		ErrorReport::SetPosition( 1000, 9284 );
		OnControlKeys( rkPacket );
		ErrorReport::SetPosition( 1000, 9285 );
		return true;
	case STPK_ONEDAY_QUEST_DELETE:
		ErrorReport::SetPosition( 1000, 9286 );
		OnQuestOneDayDelete( rkPacket );
		ErrorReport::SetPosition( 1000, 9287 );
		return true;
	case STPK_USER_HERO_DATA:
		ErrorReport::SetPosition( 1000, 9288 );
		OnUserHeroData( rkPacket );
		ErrorReport::SetPosition( 1000, 9289 );
		return true;
	case STPK_HERO_TOP100_DATA:
		ErrorReport::SetPosition( 1000, 9290 );
		OnHeroTop100Data( rkPacket );
		ErrorReport::SetPosition( 1000, 9291 );
		return true;
	case STPK_QUEST_REPEAT_REWARD:
		ErrorReport::SetPosition( 1000, 9292 );
		OnQuestRepeatReward( rkPacket );
		ErrorReport::SetPosition( 1000, 9293 );
		return true;
	case STPK_EVENT_SHOP_GOODS_LIST:
		ErrorReport::SetPosition( 1000, 9294 );
		OnEventShopGoodsList( rkPacket );
		ErrorReport::SetPosition( 1000, 9295 );
		return true;
	case STPK_EVENT_SHOP_GOODS_BUY:
		ErrorReport::SetPosition( 1000, 9296 );
		OnEventShopGoodsBuy( rkPacket );
		ErrorReport::SetPosition( 1000, 9297 );
		return true;
	case STPK_ROOM_STEALTH_ENTER:
		ErrorReport::SetPosition( 1000, 9300 );
		OnRoomStealthEnter( rkPacket );
		ErrorReport::SetPosition( 1000, 9301 );
		return true;
	case STPK_CUSTOM_ITEM_SKIN_UNIQUE_INDEX:
		ErrorReport::SetPosition( 1000, 9302 );
		OnCustomItemSkinUniqueIndex( rkPacket );
		ErrorReport::SetPosition( 1000, 9303 );
		return true;
	case STPK_COSTUME_CUSTOM_SKIN_UNIQUE_INDEX:
		ErrorReport::SetPosition( 1000, 9303 );
		OnCustomCostumeSkinUniqueIndex( rkPacket );
		ErrorReport::SetPosition( 1000, 9303 );
		return true;
	case STPK_CUSTOM_ITEM_SKIN_DELETE:
		ErrorReport::SetPosition( 1000, 9304 );
		OnCustomItemSkinDelete( rkPacket );
		ErrorReport::SetPosition( 1000, 9305 );
		return true;
	case STPK_BEST_FRIEND_LIST_MSG:
		ErrorReport::SetPosition( 1000, 9306 );
		OnBestFriendList( rkPacket );
		ErrorReport::SetPosition( 1000, 9307 );
		return true;
	case STPK_INSERT_BESTFRIEND:
		ErrorReport::SetPosition( 1000, 9308 );
		OnBestFriendInsert( rkPacket );
		ErrorReport::SetPosition( 1000, 9309 );
		return true;
	case STPK_BESTFRIEND_DELETE_LIST:
		ErrorReport::SetPosition( 1000, 9310 );
		OnBestFriendDeleteList( rkPacket );
		ErrorReport::SetPosition( 1000, 9311 );
		return true;
	case STPK_DISMISS_BESTFRIEND:
		ErrorReport::SetPosition( 1000, 9312 );
		OnBestFriendDismiss( rkPacket );
		ErrorReport::SetPosition( 1000, 9313 );
		return true;
	case STPK_CHANGE_MY_LEADER:
		ErrorReport::SetPosition( 1000, 9322 );
		OnChangeLeaderChar( rkPacket );
		ErrorReport::SetPosition( 1000, 9323 );
		return true;
	case STPK_SET_MY_RENTAL:
		ErrorReport::SetPosition( 1000, 9324 );
		OnSetMyRentalChar( rkPacket );
		ErrorReport::SetPosition( 1000, 9325 );
		return true;
	case STPK_USER_CHAR_INFO_REFRESH:
		ErrorReport::SetPosition( 1000, 9326 );
		OnUserCharInfoRefresh( rkPacket );
		ErrorReport::SetPosition( 1000, 9327 );		
		return true;
	case STPK_USER_CHAR_SUB_INFO_REFRESH:
		ErrorReport::SetPosition( 1000, 9328 );
		OnUserCharSubInfoRefresh( rkPacket );
		ErrorReport::SetPosition( 1000, 9329 );		
		return true;
	case STPK_USER_CHAR_RENTAL_REQUEST:
		ErrorReport::SetPosition( 1000, 9330 );
		OnUserCharRentalRequest( rkPacket );
		ErrorReport::SetPosition( 1000, 9331 );		
		return true;
	case STPK_USER_CHAR_RENTAL_AGREE:
		ErrorReport::SetPosition( 1000, 9332 );
		OnUserCharRentalAgree( rkPacket );
		ErrorReport::SetPosition( 1000, 9333 );		
		return true;		
	case STPK_USER_CHAR_RENTAL_RETURN:
		ErrorReport::SetPosition( 1000, 9334 );
		OnUserCharRentalReturn( rkPacket );
		ErrorReport::SetPosition( 1000, 9335 );		
		return true;		
	case STPK_HEADQUARTERS_OPTION:
		ErrorReport::SetPosition( 1000, 9336 );
		OnHeadquartersOption( rkPacket );
		ErrorReport::SetPosition( 1000, 9337 );		
		return true;
	case STPK_PCROOM_AUTHORITY:
		ErrorReport::SetPosition( 1000, 9338 );
		OnPCRoomAuthority( rkPacket );
		ErrorReport::SetPosition( 1000, 9339 );		
		return true;
	case STPK_BESTFRIEND_EXCEPTION_LIST:
		ErrorReport::SetPosition( 1000, 9340 );
		OnBestFriendExceptionList( rkPacket );
		ErrorReport::SetPosition( 1000, 9341 );
		return true;
	case STPK_GET_MILEAGE:
		ErrorReport::SetPosition( 1000, 9306 );
		OnGetMileage( rkPacket );
		ErrorReport::SetPosition( 1000, 9307 );
		return true;
	case STPK_ADD_MILEAGE:
		ErrorReport::SetPosition( 1000, 9308 );
		OnAddMileage( rkPacket );
		ErrorReport::SetPosition( 1000, 9309 );
		return true;
	case STPK_QUEST_ABUSE_ALARM:
		ErrorReport::SetPosition( 1000, 9310 );
		OnQuestAbuseAlarm( rkPacket );
		ErrorReport::SetPosition( 1000, 9311 );
		return true;
	case STPK_SHUT_DOWN_USER:
		ErrorReport::SetPosition( 1000, 9312 );
		OnShutDownUser( rkPacket );
		ErrorReport::SetPosition( 1000, 9313 );
		return true;
	case STPK_SIMPLE_USER_INFO_REFRESH:
		ErrorReport::SetPosition( 1000, 9314 );
		OnSimpleUserInfoRefresh( rkPacket );
		ErrorReport::SetPosition( 1000, 9315 );
		return true;
	case STPK_FRIEND_RECOMMEND_DATA:
		ErrorReport::SetPosition( 1000, 9316 );
		OnFriendRecommendData( rkPacket );
		ErrorReport::SetPosition( 1000, 9317 );
		return true;
	case STPK_MACRO_NOTIFY:
		ErrorReport::SetPosition( 1000, 9318 );
		OnMacroNotify( rkPacket );
		ErrorReport::SetPosition( 1000, 9319 );
		return true;
	case STPK_DISCONNECT_ALREADY_ID:
		ErrorReport::SetPosition( 1000, 9320 );
		if(m_pLogin)
			m_pLogin->OnDisconnectAlreadyID( rkPacket );
		ErrorReport::SetPosition( 1000, 9321 );
		return true;
	case STPK_EXPAND_MEDAL_SLOT_DATA:
		ErrorReport::SetPosition( 1000, 9322 );
		OnExpandMedalSlotData( rkPacket );
		ErrorReport::SetPosition( 1000, 9323 );
		return true;
	case STPK_EXPAND_MEDAL_SLOT_DELETE_BY_DATE:
		ErrorReport::SetPosition( 1000, 9322 );
		OnExpandMedalSlotDeleteByDate( rkPacket );
		ErrorReport::SetPosition( 1000, 9323 );
		return true;
	case STPK_SELECT_SHUT_DOWN_USER:
		ErrorReport::SetPosition( 1000, 9324 );
		OnSelectShutDownUser( rkPacket );
		ErrorReport::SetPosition( 1000, 9325 );
		return true;
	case STPK_USER_ALCHEMIC_DATA:
		OnAlchemicItemData( rkPacket );
		return true;
	case STPK_ALCHEMIC_SYNC:
		OnAlchemicItemSync( rkPacket );
		return true;
	case STPK_SOLDIER_DISASSEMBLE:
		OnCharDisassemble( rkPacket );
		return true;
	case STPK_LOGIN_MOVESERVER:
		ErrorReport::SetPosition( 1000, 9340 );
		OnLoginMoveServer( rkPacket );
		ErrorReport::SetPosition( 1000, 9341 );
		return true;
	case STPK_GIFTCLOVER_INFO:
		ErrorReport::SetPosition( 1000, 9342 );
		OnSyncGiftCloverInfo( rkPacket );
		ErrorReport::SetPosition( 1000, 9343 );
		return true;
	case STPK_CLOVER_FRIEND_INFO:
		ErrorReport::SetPosition( 1000, 9344 );
		OnSyncFriendCloverInfo( rkPacket );
		ErrorReport::SetPosition( 1000, 9345 );
		return true;
	case STPK_ETCITEM_CLOVER_UPDATE:
		ErrorReport::SetPosition( 1000, 9346 );
		OnEtcItemCloverUpdate( rkPacket );
		ErrorReport::SetPosition( 1000, 9347 );
		return true;
	case STPK_ADD_CASH:
		ErrorReport::SetPosition( 1000, 9348 );
		OnAddCash( rkPacket );
		ErrorReport::SetPosition( 1000, 9349 );
		return true;
	case STPK_FILL_CASH_URL:
		ErrorReport::SetPosition( 1000, 9350 );
		OnFillCashUrl( rkPacket );
		ErrorReport::SetPosition( 1000, 9351 );
		return true;
	case STPK_RELATIVE_GRADE_REWARD:
		ErrorReport::SetPosition( 1000, 9352 );
		OnRelativeGradeBonus( rkPacket );
		ErrorReport::SetPosition( 1000, 9353 );
		return true;
	case STPK_UPDATE_RELATIVE_GRADE_EXP:
		ErrorReport::SetPosition( 1000, 9354 );
		OnUpdateRelativeGradeExp( rkPacket );
		ErrorReport::SetPosition( 1000, 9355 );
		return true;
	case STPK_SUBSCRIPTION_DATA:
		ErrorReport::SetPosition( 1000, 9358 );
		OnSubscriptionData( rkPacket );
		ErrorReport::SetPosition( 1000, 9359 );
		return true;
	case STPK_SUBSCRIPTION_RECV:
		ErrorReport::SetPosition( 1000, 9360 );
		OnSubscriptionRecv( rkPacket );
		ErrorReport::SetPosition( 1000, 9361 );
		return true;
	case STPK_SUBSCRIPTION_RETR:
		ErrorReport::SetPosition( 1000, 9362 );
		OnSubscriptionRetr( rkPacket );
		ErrorReport::SetPosition( 1000, 9363 );
		return true;
	case STPK_SESSION_CONTROL:
		ErrorReport::SetPosition( 1000, 9364 );
		OnNexonSessionMsg( rkPacket );
		ErrorReport::SetPosition( 1000, 9365 );
		return true;
	case STPK_EXERCISE_PCROOM_CHAR_CREATE:
		ErrorReport::SetPosition( 1000, 9366 );
		OnExercisePcRoomCharCreate( rkPacket );
		ErrorReport::SetPosition( 1000, 9367 );
		return true;
	case STPK_EXPIRED_CHAR_DEL_BY_PCROOM:
		ErrorReport::SetPosition( 1000, 9368 );
		OnDeleteExpiredCharByPCRoom( rkPacket );
		ErrorReport::SetPosition( 1000, 9369 );
		return true;
	case STPK_USER_PET_DATA:
		ErrorReport::SetPosition( 1000, 9370 );
		OnPetData( rkPacket );
		ErrorReport::SetPosition( 1000, 9371 );
		return true;
	case STPK_ATTENDANCE_RECORD:
		ErrorReport::SetPosition( 1000, 9370 );
		OnAttendanceRecord( rkPacket );
		ErrorReport::SetPosition( 1000, 9371 );
		return true;
	case STPK_ATTENDANCE_CHECK:
		ErrorReport::SetPosition( 1000, 9372 );
		OnAttendanceCheck( rkPacket );
		ErrorReport::SetPosition( 1000, 9373 );
		return true;
	case STPK_CHAR_AWAKE:
		ErrorReport::SetPosition( 1000, 9374 );
		g_AwakeMgr.ApplyAwake( rkPacket );
		ErrorReport::SetPosition( 1000, 9375 );
		return true;
	case STPK_CHAR_AWAKE_EXTEND:
		ErrorReport::SetPosition( 1000, 9376 );
		g_AwakeMgr.ApplyAwakeExtend( rkPacket );
		ErrorReport::SetPosition( 1000, 9377 );
		return true;
	case STPK_DELETE_CHAR_AWAKE_DATE:
		ErrorReport::SetPosition( 1000, 9378 );
		g_AwakeMgr.ApplyAwakeEnd( rkPacket );
		ErrorReport::SetPosition( 1000, 9379 );
		return true;
	case STPK_SEARCH_PLAZA_ROOM:
		ErrorReport::SetPosition( 1000, 9380 );
		OnSearchPlazaRoom( rkPacket );
		ErrorReport::SetPosition( 1000, 9381 );
		return true;
	case STPK_JOIN_HEADQUARTERS:
		ErrorReport::SetPosition( 1000, 9382 );
		OnJoinHeadQuarters( rkPacket );
		ErrorReport::SetPosition( 1000, 9383 );
		return true;
	case STPK_CREATE_PLAZA:
		ErrorReport::SetPosition( 1000, 9384 );
		OnCreatePlaza( rkPacket );
		ErrorReport::SetPosition( 1000, 9385 );
		return true;
	case STPK_TUTORIAL_STAGE_CREATE:
		ErrorReport::SetPosition( 1000, 9386 );
		OnCreateTutorialStage( rkPacket );
		ErrorReport::SetPosition( 1000, 9387 );
		return true;
	case STPK_USER_COSTUME_DATA:
		ErrorReport::SetPosition( 1000, 9388 );
		OnCostumeData( rkPacket );
		ErrorReport::SetPosition( 1000, 9389 );
		return true;
	case STPK_DELETE_COSTUME_DATE:
		ErrorReport::SetPosition( 1000, 9390 );
		OnCostumeDeleteByDate( rkPacket );
		ErrorReport::SetPosition( 1000, 9391 );
		return true;
	case STPK_MISSION_STATE_CHANGE:
	case STPK_MISSION_INFO:
	case STPK_MISSION_COMPENSATION_RECV:
		ErrorReport::SetPosition( 1000, 9392 );
		OnMissionData( rkPacket );
		ErrorReport::SetPosition( 1000, 9393 );
		return true;
	case STPK_ROLLBOOK_ATTEND:
		ErrorReport::SetPosition( 1000, 9394 );
		OnRollBookReward( rkPacket );
		ErrorReport::SetPosition( 1000, 9395 );
		return true;
	case STPK_GUILD_ATTEND:
		ErrorReport::SetPosition( 1000, 9396 );
		OnGuildAttend( rkPacket );
		ErrorReport::SetPosition( 1000, 9397 );
		return true;
	case STPK_RECV_GUILDATTEND_REWARD:
		ErrorReport::SetPosition( 1000, 9398 );
		OnGuildReceiveAttendReward( rkPacket );
		ErrorReport::SetPosition( 1000, 9399 );
		return true;
	case STPK_RECV_GUILDRANK_REWARD:
		ErrorReport::SetPosition( 1000, 9400 );
		OnGuildRankReward( rkPacket );
		ErrorReport::SetPosition( 1000, 9401 );
		return true;
	case STPK_GUILD_MEMBER_ATTEND_INFO:
		ErrorReport::SetPosition( 1000, 9402 );
		OnGuildMemberAttendInfo( rkPacket );
		ErrorReport::SetPosition( 1000, 9403 );
		return true;
	case STPK_POPUP_OPEN_INDEX:
		ErrorReport::SetPosition( 1000, 9388 );
		OnPopupOpenIndex( rkPacket );
		ErrorReport::SetPosition( 1000, 9389 );
		return true;
	case STPK_POPUP_ITEM_BUY_RESULT:
		ErrorReport::SetPosition( 1000, 9388 );
		OnPopupItemBuyResult( rkPacket );
		ErrorReport::SetPosition( 1000, 9389 );
		return true;
	case STPK_RSOLDIER_STATUS:
		ErrorReport::SetPosition( 1000, 9390 );
		OnRSoldierInfo( rkPacket );
		ErrorReport::SetPosition( 1000, 9391 );
		return true;
	case STPK_GUILD_ROOM_ACTIVE:
		ErrorReport::SetPosition( 1000, 9392 );
		OnGuildRoomActive( rkPacket );
		ErrorReport::SetPosition( 1000, 9393 );
		return true;
	case STPK_JOIN_PERSONAL_HQ:
		ErrorReport::SetPosition( 1000, 9394 );
		OnJoinHouse( rkPacket ); 
		ErrorReport::SetPosition( 1000, 9395 );
		return true;
	case STPK_TITLE_INVEN_INFO:
		ErrorReport::SetPosition( 1000, 9396 );
		OnNamedTitleData( rkPacket );
		ErrorReport::SetPosition( 1000, 9397 );
		return true;
	case STPK_OAK_INFO_REQUEST:
		ErrorReport::SetPosition( 1000, 9398 );
		OnPriateRulletInfo( rkPacket );
		ErrorReport::SetPosition( 1000, 9399 );
		return true;
	case STPK_OAK_USE_SWORD:
		ErrorReport::SetPosition( 1000, 9400 );
		OnPriateRulletUse( rkPacket );
		ErrorReport::SetPosition( 1000, 9401 );
		return true;
	case STPK_OAK_RSET:
		ErrorReport::SetPosition( 1000, 9402 );
		OnPriateRulletRset( rkPacket );
		ErrorReport::SetPosition( 1000, 9403 );
		return true;
	case STPK_SOLDIER_SET_STATUS:
		ErrorReport::SetPosition( 1000, 9404 );
		OnSoldierSetInfo( rkPacket );
		ErrorReport::SetPosition( 1000, 9405 );
		return true;
	case STPK_CHANGE_GENDER:
		ErrorReport::SetPosition( 1000, 9406 );
		OnGenderChange( rkPacket );
		ErrorReport::SetPosition( 1000, 9407 );
		return true;
	case STPK_USER_ACCESSORY_DATA:
		ErrorReport::SetPosition( 1000, 9408 );
		OnAccessoryData( rkPacket );
		ErrorReport::SetPosition( 1000, 9409 );
		return true;
	case STPK_DELETE_ACCESSORY_DATE:
		ErrorReport::SetPosition( 1000, 9410 );
		OnAccessoryDeleteByDate( rkPacket );
		ErrorReport::SetPosition( 1000, 9411 );
		return true;
		/*
		// �ڽ�Ƭ ���� (���� ������� �ʴ� ��� ����)
	case STPK_CUSTOM_COSTUME_SKIN_UNIQUE_INDEX:
		ErrorReport::SetPosition( 1000, 9392 );
		OnCustomCostumeSkinUniqueIndex( rkPacket );
		ErrorReport::SetPosition( 1000, 9393 );
		return true;
		*/
	case STPK_CUSTOM_COSTUME_SKIN_DELETE:
		ErrorReport::SetPosition( 1000, 9394 );
		OnCustomCostumeSkinDelete( rkPacket );
		ErrorReport::SetPosition( 1000, 9395 );
		return true;

	}

	if( TournamentPacketParser::ProcessTCPPacket( rkPacket ) )
		return true;

	if( g_SpecialShopMgr.ProcessTCPPacket( rkPacket ) )
		return true;

	if( m_pGuildBlockStorage && m_pGuildBlockStorage->ProcessTCPPacket( rkPacket ) )
		return true;

	if( m_pHomeBlockStorage && m_pHomeBlockStorage->ProcessTCPPacket( rkPacket ) )
		return true;

	return false;
}

bool ioApplication::ProcessUDPPacket( SP2Packet &rkPacket )
{
	switch( rkPacket.GetPacketID() )
	{
	case SUPK_CONNECT:
		ErrorReport::SetPosition( 1000, 9601 );
		if( m_pLogin )
		    m_pLogin->OnUDPConnect( rkPacket );
		ErrorReport::SetPosition( 1000, 9602 );
		return true;
	case SUPK_PROTECT:
		ErrorReport::SetPosition( 1000, 9603 );
		OnProtect( rkPacket );
		ErrorReport::SetPosition( 1000, 9604 );
		return true;
#ifdef ANTIHACK
	case SUPK_RUDP:
		ErrorReport::SetPosition( 1000, 9604 );
		OnRudpServer( rkPacket );
		ErrorReport::SetPosition( 1000, 9605 );
		return true;	
	case SUPK_CLIENT_MSG:
		ErrorReport::SetPosition( 1000, 9606 );
		AddClientMessage( rkPacket );
		ErrorReport::SetPosition( 1000, 9607 );
		return true;
#endif
	
	}

	return false;
}

void ioApplication::OnClassPrice( SP2Packet &rkPacket )
{
	g_ClassPrice.ApplyPrice( rkPacket );
}

void ioApplication::OnCharCreate( SP2Packet &rkPacket )
{
	SetMouseBusy( false );

	int iResult = 0;
	rkPacket>>iResult;

	switch( iResult )
	{
	case CREATE_CHAR_NAME_ALREADY:
		g_GUIMgr.SetPrevMsgBox( g_GUIMgr.FindWnd( CHAR_CREATE_WND ), MB_OK, NULL, STR(1) );
		return;
	case CREATE_CHAR_CASH_FAIL:
		g_GUIMgr.ShowWnd( BILLING_WANT_WND );
		return;
	case CREATE_CHAR_PESO_FAIL:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return;
	case CREATE_CHAR_EXCEPTION:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		return;
	case CREATE_CHAR_PRICE_CHANGE:
		{
			NewShopWnd *pNewShopWnd = dynamic_cast<NewShopWnd*>(g_GUIMgr.FindWnd( NEW_SHOP_WND ));
			if( pNewShopWnd && pNewShopWnd->IsShow() )
			{
				pNewShopWnd->HideWnd();
			}
			g_GUIMgr.SetPrevMsgBox( pNewShopWnd, MB_OK, NULL, STR(4) );
		}
		return;
	case CREATE_CHAR_FAIL_WORKING:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
		return;	
	case CREATE_CHAR_PCROOM_NOT:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6) );
		return;
	case CREATE_CHAR_NEED_LEVEL:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(7) );
		return;
	case CREATE_CHAR_BILLING_WAIT:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(8) );
		return;
	case CREATE_CHAR_BILLING_DISCONNECT:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(9) );
		return;
	case CREATE_CHAR_BILLING_FAIL:
		{
			bool bBillingError    = false;
			ioHashString  sBillingError;
			rkPacket >> bBillingError;
			if( bBillingError )
				rkPacket >> sBillingError;

			ioHashString sErrorMent = STR(10);
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
					StringCbPrintf( szErrorFullMent, sizeof( szErrorFullMent ), "%s",  sError.c_str() ); //Except Extracting Hangeul
				else
					StringCbPrintf( szErrorFullMent, sizeof( szErrorFullMent ), "%s#%s%s", sErrorMent.c_str(), sError.c_str(), sBillingError.c_str() ); //Except Extracting Hangeul
								
				g_GUIMgr.SetMsgBox( MB_OK, NULL, szErrorFullMent );
			}
		}
		return;
	case CREATE_CHAR_BILLING_EXCESS_BUY:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(11) );
		return;
	case CREATE_CHAR_FULL_MORTMAIN_CHAR:
		{
			int iHaveMortmainChar = g_LevelMgr.GetNumHaveMortmainChar( g_MyInfo.GetGradeLevel() );
			int iNextGradeLv = g_LevelMgr.GetNextNumHaveMortmainChar( g_MyInfo.GetGradeLevel() );
			int iNextHaveNum = g_LevelMgr.GetNumHaveMortmainChar( iNextGradeLv );
			char szGradeName[MAX_PATH];
			g_LevelMgr.GetGradeName( iNextGradeLv, szGradeName, sizeof( szGradeName ), false , true );
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(12), iHaveMortmainChar, szGradeName ,iNextHaveNum );
		}
		return;
	case CREATE_CHAR_WANT_OF_MORTMAIN_ETCITEM:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(13) );
		return;
	}

	__int64 iPrevMoney = g_MyInfo.GetMoney();
	__int64 iMoney;
	int iCash;
	int iChannelingCash;
	int iState;

	rkPacket >> iMoney >> iCash >> iChannelingCash >> iState;
	if( iMoney != g_MyInfo.GetMoney() || iCash != g_MyInfo.GetCash() )
		g_ExSoundMgr.PlaySound( ExSound::EST_SHOP_BUY_SUCCESS );

#if defined( USE_GA )
	bool bGACash	= false;
	bool bGAMoney	= false;
	int iPesoPrice	= g_MyInfo.GetMoney() - iMoney;
	int iCashPrice	= g_MyInfo.GetCash() - iCash;
	if( iCashPrice > 0 && iCashPrice < 1000000 )
		bGACash		= true;
	if( iPesoPrice > 0 )
		bGAMoney	= true;
#endif
	
	g_MyInfo.SetMoney( iMoney );
	g_MyInfo.SetCash( iCash );
	g_MyInfo.SetChannelingCash( iChannelingCash );
	g_MyInfo.SetUserState( iState );

	int  iCharArray;
	CHARACTERDATA kCharData;
	int  iBonusPeso;
	bool bCash;

	rkPacket >> iCharArray;	
	rkPacket >> kCharData.m_dwIndex >> kCharData.m_data >> iBonusPeso >> bCash;
	
	const ioClassExpert::CLASSSLOT* pClassSlot = g_MyInfo.GetClassExpertSlot( kCharData.m_data.m_class_type );
	if( pClassSlot && pClassSlot->m_ePowerUpCharGrade != PUGT_NONE )
	{
		kCharData.m_data.m_eCharPowerUpType = pClassSlot->m_ePowerUpCharGrade;
	}

	ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
	int iExerciseCharArray = g_MyInfo.GetExerciseCharArrayByClass( kCharData.m_data.m_class_type );
	if( iExerciseCharArray != -1)
	{
		DWORD dwExerciseCharIndex = g_MyInfo.GetCharIndex( iExerciseCharArray );
		if( pPlayStage )
			pPlayStage->RemoveMySoldier( dwExerciseCharIndex );
		g_MyInfo.SetCharacter( iExerciseCharArray, kCharData );
	}
	else
	{
		g_MyInfo.AddCharacter( kCharData );
	}

	if( pPlayStage )
	{
		pPlayStage->AddMySoldier( kCharData.m_dwIndex, true );
		g_MyInfo.RefreshExtraItemEquip();
		pPlayStage->ReLoadAllMySoldier();

		//R Soldier Info
		if ( g_RSoldierInfoManager.IsRSoldierType( kCharData.m_data ) && pPlayStage->GetModeType() == MT_MYROOM )
			g_MyInfo.CalcRCharPossessionCount();
	}
	
	if ( g_RSoldierInfoManager.IsSoldierSet( kCharData.m_data ) )
		g_MyInfo.CalcSoldierSetCnt();

	ExerciseSoldierEventWnd *pExerciseSoldierEventWnd = dynamic_cast<ExerciseSoldierEventWnd*> (g_GUIMgr.FindWnd( EXERCISE_SOLDIER_EVENT_WND ) );
	if( pExerciseSoldierEventWnd )
		pExerciseSoldierEventWnd->UpdateChildPos();

	if( iResult == CREATE_CHAR_OK )
	{
		if( iPrevMoney > g_MyInfo.GetMoney() && !IsMeDeveloper() && !IsMeGameMaster() )
		{
			int iBuyPeso = (int)( iPrevMoney - g_MyInfo.GetMoney() );
			g_AwardMgr.AddShopping( g_MyInfo.GetPublicID(), iBuyPeso );
			SP2Packet kPacket( CUPK_SHOPPING );
			kPacket << g_MyInfo.GetPublicID() << iBuyPeso;
			SendToAllPlayingUser( kPacket );
		}
		g_MyInfo.SetRoomOutTimer( 0 );
	}
	CloseGuildMarkPage();

	MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(g_GUIMgr.FindWnd(MY_INVENTORY_WND));
	if( pInvenWnd && pInvenWnd->IsShow() )
	{
		pInvenWnd->UpdateTab( false, false );
	}

	//if( iBonusPeso > 0 )
	//	g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(14), iBonusPeso );

	if( !bCash && kCharData.m_data.m_ePeriodType == CPT_MORTMAIN )
	{
		ioEtcItemBuyMortmainChar *pEtcItem = dynamic_cast<ioEtcItemBuyMortmainChar*> ( g_EtcItemMgr.FindEtcItem( ioEtcItem::EIT_ETC_BUY_MORTMAIN_CHAR ));
		if( !pEtcItem )
			LOG.PrintTimeAndLog( 0, "%s pEtcItem == NULL.", __FUNCTION__ );
		else
			pEtcItem->SetUsed( g_MyInfo.GetUserEtcItem() );
	}

	NewShopWnd *pNewShopWnd = dynamic_cast<NewShopWnd*> ( g_GUIMgr.FindWnd( NEW_SHOP_WND ) );
	if( pNewShopWnd && pNewShopWnd->IsShow() )
	{
		pNewShopWnd->SetBoughtMark( NewShopWnd::ID_SOLDIER_TAB_BTN, kCharData.m_data.m_class_type, 0 );
		NewShopBuyWnd *pNewShopBuyWnd = dynamic_cast<NewShopBuyWnd*> ( pNewShopWnd->FindChildWnd( NewShopWnd::ID_BUY_WND ) );
		if( pNewShopBuyWnd && pNewShopBuyWnd->IsShow() )
			pNewShopBuyWnd->ShowOnlyBoughtBtns();
	}	
	
	SoldierSelectWnd *pSoldierWnd = dynamic_cast<SoldierSelectWnd*>(g_GUIMgr.FindWnd( SOLDIER_SELECT_WND ));
	if( pSoldierWnd && pSoldierWnd->IsShow() )
	{
		pSoldierWnd->SettingSoldierBtn();
	}

	MiniSoldierSelectWnd *pMiniSoldierWnd = dynamic_cast<MiniSoldierSelectWnd*>( g_GUIMgr.FindWnd( MINI_SOLDIER_SELECT_WND ));
	if( pMiniSoldierWnd && pMiniSoldierWnd->IsShow() )
	{
		pMiniSoldierWnd->SettingMiniSoldierBtn();
	}

	HelpWnd *pHelpWnd = dynamic_cast<HelpWnd*>( g_GUIMgr.FindWnd( HELP_WND ) );
	if( pHelpWnd )
	{
		pHelpWnd->ReCreateWindow();
	}

#if defined( USE_GA )
	char chLabel[32] = {0,};

	if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
		sprintf_e( chLabel, "%d", kCharData.m_data.m_class_type );
	else
		SafeSprintf( chLabel, sizeof(chLabel), "%1", kCharData.m_data.m_class_type );

	// CHAR_CREATE
	g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
		, "Character"
		, "Create"
		, chLabel
		, 1
		, "%2FCHAR%2FCREATE" );

	if( bGACash )
	{
		if( kCharData.m_data.m_ePeriodType == CPT_MORTMAIN )
			iCashPrice = g_ClassPrice.GetMortmainCharCash( kCharData.m_data.m_class_type );
		else
			iCashPrice = g_ClassPrice.GetClassBuyCash( kCharData.m_data.m_class_type, kCharData.m_data.m_iLimitSecond );

		// GOLD_USE_HERO
		g_HttpMng.GA_ItemHitTracking( g_MyInfo.GetUserIndex() 
			, "Hero"
			, iCashPrice
			, 1
			, kCharData.m_data.m_class_type
			, "Gold"
			, "%2FGOLD%2FUSE%2FHERO"
			, true );
	}
	if( bGAMoney )
	{
		// PESO_USE_HERO
		g_HttpMng.GA_ItemHitTracking( g_MyInfo.GetUserIndex() 
			, "Hero"
			, iPesoPrice
			, 1
			, kCharData.m_data.m_class_type
			, "Peso"
			, "%2FPESO%2FUSE%2FHERO" );
	}			
#endif

}

void ioApplication::OnCharLoad( SP2Packet &rkPacket )
{
	LOG.PrintTimeAndLog( 0, "������� �ʴ� ��Ŷ�� �޾��� : OnCharLoad" );	
}

void ioApplication::OnCharAllLoad( SP2Packet &rkPacket )
{
	int i = 0;
	int char_count = 0;
	rkPacket >> char_count;	
	for(i = 0;i < char_count;i++)
	{		
		int char_array = 0;	
		CHARACTERDATA kCharData;
		
		rkPacket >> char_array;
		rkPacket >> kCharData.m_dwIndex >> kCharData.m_data;
		g_MyInfo.AddCharacter( kCharData );	
	}

	//���� �α��� �� �������� Rĳ���� ������ ���� �����Ƿ� Ŭ�� ���.
	g_MyInfo.CalcRCharPossessionCount();
	g_MyInfo.CalcSoldierSetCnt();
}

void ioApplication::OnClassExpert( SP2Packet &rkPacket )
{
	g_MyInfo.ApplyClassExpert( rkPacket );
}

void ioApplication::OnInventorySlot( SP2Packet &rkPacket )
{
	g_MyInfo.ApplyInventorySlotData( rkPacket );
	//g_MyInfo.SetAllCharAllDecoration();
}

void ioApplication::OnUserEtcItem( SP2Packet &rkPacket )
{
	ioUserEtcItem *pUserEtcItem = g_MyInfo.GetUserEtcItem();
	if( !pUserEtcItem )
		return;
	
	pUserEtcItem->ApplyData( rkPacket );
	g_EtcItemMgr.Init( pUserEtcItem, ToPlayStage( m_pGameStage ) );

	g_MyInfo.CheckCurMaxCharSlot();
	g_SlotMgr.InitSlot();
}

void ioApplication::OnExtraItemData( SP2Packet &rkPacket )
{
	ioUserExtraItem *pUserExtraItem = g_MyInfo.GetUserExtraItem();
	if( !pUserExtraItem )
		return;

	pUserExtraItem->ApplyExtraItemData( rkPacket );

	bool bEndData;
	rkPacket >> bEndData;

	// �Ǹ��������� �����͸� �޾������� ����
	if ( !bEndData )
		return;

	g_MyInfo.RefreshExtraItemEquip();

	if( ToPlayStage( m_pGameStage ) )
	{
		ToPlayStage( m_pGameStage )->ReLoadAllMySoldier();
	}

	SoldierSelectWnd *pSoldierWnd = dynamic_cast<SoldierSelectWnd*>(g_GUIMgr.FindWnd( SOLDIER_SELECT_WND ));
	if( pSoldierWnd && pSoldierWnd->IsShow() )
	{
		pSoldierWnd->SettingSoldierBtn();
	}

	MiniSoldierSelectWnd *pMiniSoldierWnd = dynamic_cast<MiniSoldierSelectWnd*>( g_GUIMgr.FindWnd( MINI_SOLDIER_SELECT_WND ));
	if( pMiniSoldierWnd && pMiniSoldierWnd->IsShow() )
	{
		pMiniSoldierWnd->SettingMiniSoldierBtn();
	}

// 	//�ӽ�
// 	DWORDVec dwCustomList;
// 	pUserExtraItem->GetCustomIdx( dwCustomList );
// 	g_FileWriteClient.CreateUploadList( dwCustomList );
}

//�� ������ ����ȭ
void ioApplication::OnPetData( SP2Packet &rkPacket )
{
	ioUserPet *pUserPet = g_MyInfo.GetUserPet();
	if( !pUserPet )
		return;

	pUserPet->ApplyPetSlotData( rkPacket );
}

void ioApplication::OnAlchemicItemData( SP2Packet &rkPacket )
{
	ioAlchemicInventory *pAlchemicInven = g_MyInfo.GetAlchemicInventory();
	if( !pAlchemicInven )
		return;

	pAlchemicInven->ApplyAlchemicData( rkPacket );
}

void ioApplication::OnAlchemicItemSync( SP2Packet &rkPacket )
{
	ioAlchemicInventory *pAlchemicInven = g_MyInfo.GetAlchemicInventory();
	if( !pAlchemicInven )
		return;

	pAlchemicInven->ApplyAlchemicSync( rkPacket );
}

void ioApplication::OnExtraItemDeleteByDate( SP2Packet &rkPacket )
{
	if( !g_MyInfo.DeleteExtraItemData( rkPacket ) )
	{
		LOG.PrintTimeAndLog( 0, "ioApplication::OnExtraItemDeleteByDate() - Delete Fail" );
		return;
	}

	MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(g_GUIMgr.FindWnd(MY_INVENTORY_WND));
	if( pInvenWnd && pInvenWnd->IsShow() )
	{
		pInvenWnd->UpdateTab( true, true );
	}

	SoldierSelectWnd *pSoldierWnd = dynamic_cast<SoldierSelectWnd*>(g_GUIMgr.FindWnd( SOLDIER_SELECT_WND ));
	if( pSoldierWnd && pSoldierWnd->IsShow() )
	{
		pSoldierWnd->SettingSoldierBtn();
	}

	MiniSoldierSelectWnd *pMiniSoldierWnd = dynamic_cast<MiniSoldierSelectWnd*>( g_GUIMgr.FindWnd( MINI_SOLDIER_SELECT_WND ));
	if( pMiniSoldierWnd && pMiniSoldierWnd->IsShow() )
	{
		pMiniSoldierWnd->SettingMiniSoldierBtn();
	}
}

// �ڽ�Ƭ ���� (�����κ��� ������ ���)
void ioApplication::OnCostumeData( SP2Packet &rkPacket )
{
	ioUserCostume *pUserCostume = g_MyInfo.GetUserCostume();
	if( !pUserCostume )
		return;

	pUserCostume->ApplyCostumeSlotData( rkPacket );

	if( ToPlayStage( m_pGameStage ) )
		ToPlayStage( m_pGameStage )->ReLoadAllMySoldier();

	SoldierSelectWnd *pSoldierWnd = dynamic_cast<SoldierSelectWnd*>(g_GUIMgr.FindWnd( SOLDIER_SELECT_WND ));
	if( pSoldierWnd && pSoldierWnd->IsShow() )
		pSoldierWnd->SettingSoldierBtn();

	MiniSoldierSelectWnd *pMiniSoldierWnd = dynamic_cast<MiniSoldierSelectWnd*>( g_GUIMgr.FindWnd( MINI_SOLDIER_SELECT_WND ));
	if( pMiniSoldierWnd && pMiniSoldierWnd->IsShow() )
		pMiniSoldierWnd->SettingMiniSoldierBtn();
}

// �ڽ�Ƭ ���� (�Ⱓ�� ����)
void ioApplication::OnCostumeDeleteByDate( SP2Packet &rkPacket )
{
	if( !g_MyInfo.DeleteCostumeData( rkPacket ) )
	{
		LOG.PrintTimeAndLog( 0, "ioApplication::OnCostumeDeleteByDate() - Delete Fail" );
		return;
	}

	MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(g_GUIMgr.FindWnd(MY_INVENTORY_WND));
	if( pInvenWnd && pInvenWnd->IsShow() )
	{
		pInvenWnd->UpdateTab( true, true );
	}

	SoldierSelectWnd *pSoldierWnd = dynamic_cast<SoldierSelectWnd*>(g_GUIMgr.FindWnd( SOLDIER_SELECT_WND ));
	if( pSoldierWnd && pSoldierWnd->IsShow() )
	{
		pSoldierWnd->SettingSoldierBtn();
	}

	MiniSoldierSelectWnd *pMiniSoldierWnd = dynamic_cast<MiniSoldierSelectWnd*>( g_GUIMgr.FindWnd( MINI_SOLDIER_SELECT_WND ));
	if( pMiniSoldierWnd && pMiniSoldierWnd->IsShow() )
	{
		pMiniSoldierWnd->SettingMiniSoldierBtn();
	}
}

void ioApplication::OnTimeGrowthCheck( SP2Packet &rkPacket )
{
	ioHashString szOwner;

	rkPacket >> szOwner;

	if( szOwner.IsEmpty() )
		return;

	bool bOwner = false;
	if( szOwner == g_MyInfo.GetPublicID() )
		bOwner = true;

	ioGrowthLevel *pLevel = NULL;
	if( bOwner )
	{
		//g_MyInfo.InitTimeGrowthEndInfo();

		pLevel = g_MyInfo.GetGrowthLevel();
	}
	else
	{
		ioPlayStage *pStage = ToPlayStage(m_pGameStage);
		if( pStage )
			pLevel = pStage->GetGrowthLevel(szOwner);
	}

	int iSize;
	rkPacket >> iSize;

	for( int i=0; i < iSize; ++i )
	{
		bool bConfirm;
		int iClassType, iSlot, iPoint;
		rkPacket >> iClassType >> iSlot >> bConfirm >> iPoint;

		if( pLevel )
		{
			if( bConfirm && COMPARE( iSlot, TIG_WEAPON, TIG_ATTACK ) )
			{
				int iIndex = iSlot - 1;
				pLevel->ApplyItemGrowthLevelUp( iClassType, iIndex, 1 );
			}
			else if( bConfirm && COMPARE( iSlot, TIG_ATTACK, TIG_DROP+1 ) )
			{
				int iIndex = iSlot - 4 - 1;
				pLevel->ApplyCharGrowthLevelUp( iClassType, iIndex, 1 );
			}

			pLevel->SetCharGrowthPoint( iClassType, iPoint );

			if( bOwner )
			{
				pLevel->RemoveTimeGrowth( iClassType );

				g_MyInfo.AddTimeGrowthEndInfo( iClassType, iSlot, bConfirm );
				g_QuestMgr.QuestCompleteTerm( QC_TIME_GROWTH_SUCCESS, pLevel->GetCharGrowthLevel( iClassType, iSlot, true ) );

#if defined( USE_GA )
				char chLabel[32] = {0,};

				if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
					sprintf_e( chLabel, "%d", iClassType );
				else
					SafeSprintf( chLabel, sizeof(chLabel), "%1", iClassType );

				// CHAR_GROWTH_TIME_END
				g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
					, "Character"
					, "End"
					, chLabel
					, 1
					, "%2FCHAR%2FGROWTH%2FTIME%2FEND" );
#endif

			}
		}
	}

	if( bOwner )
	{
		MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(g_GUIMgr.FindWnd(MY_INVENTORY_WND));
		if( pInvenWnd && pInvenWnd->IsShow() )
		{
			if( pInvenWnd->GetCurTabID() == MyInventoryWnd::ID_SOLDIER_TAB_BTN )
				pInvenWnd->UpdateTab( false, true );
		}

		ioGrowthSelectWnd *pSelectWnd = dynamic_cast<ioGrowthSelectWnd*>(g_GUIMgr.FindWnd(GROWTH_SELECT_WND));
		if( pSelectWnd && pSelectWnd->IsShow() )
		{
			pSelectWnd->UpdateInfo();
		}

		g_MyInfo.ShowTimeGrowthEndInfo();
	}
}

void ioApplication::OnServiceChar( SP2Packet &rkPacket )
{
	DWORD dwServerDate;
	rkPacket >> dwServerDate;
	Help::SavePCRoomBonusDate( dwServerDate );
}

void ioApplication::OnPresentData( SP2Packet &rkPacket )
{
	g_PresentMgr.ApplyPresentData( rkPacket );
}

void ioApplication::OnPresentRecv( SP2Packet &rkPacket )
{
	ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );

	// �� ���� �ޱ⿡�� MyInven ���� ���� ó��
	g_PresentMgr.ApplyPresentRecv( rkPacket, pPlayStage );
}

void ioApplication::OnPresentSell( SP2Packet &rkPacket )
{
	g_PresentMgr.ApplyPresentSell( rkPacket );
}

void ioApplication::OnAwardData( SP2Packet &rkPacket )
{
	g_MyInfo.ApplyAwardData( rkPacket );
}

void ioApplication::OnUserRecordData( SP2Packet &rkPacket )
{
	g_MyInfo.ApplyUserRecordData( rkPacket );
}

void ioApplication::OnGrowthLevelData( SP2Packet &rkPacket )
{
	g_MyInfo.ApplyGrowthLevelData( rkPacket );

	ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
	if( pPlayStage )
		pPlayStage->UpdateGrowthData();
}

void ioApplication::OnBuyItem( SP2Packet &rkPacket )
{
	SetMouseBusy( false );

	int iResult;
	rkPacket >> iResult;

	switch( iResult )
	{
	case BUY_ITEM_OK:
		_OnBuyItemOK( rkPacket );
		break;
	case BUY_ITEM_SERVICE:
		_OnServiceItemOK( rkPacket );
		break;
	case BUY_ITEM_SLOT_FAIL:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		break;
	case BUY_ITEM_CASH_FAIL:
		g_GUIMgr.ShowWnd( BILLING_WANT_WND );
		break;
	case BUY_ITEM_PESO_FAIL:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		break;
	case BUY_ITEM_EXCEPTION:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		break;
	}
}

void ioApplication::_OnBuyItemOK( SP2Packet &rkPacket )
{
}

void ioApplication::_OnServiceItemOK( SP2Packet &rkPacket )
{
}

void ioApplication::OnEquipSlotItem( SP2Packet &rkPacket )
{
	SetMouseBusy( false );

	int iResult;
	rkPacket >> iResult;
	if( iResult == EQUIP_SLOT_ITEM_FAIL )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return;
	}

	DWORD dwCharIndex;
	CHARACTER kCharInfo;
	int iItemType, iItemCode;
	rkPacket >> iItemType >> iItemCode >> dwCharIndex >> kCharInfo;

	g_MyInfo.SetEquipItem( iItemType, iItemCode );
#ifdef CUSTOM_COSTUME
	g_MyInfo.ApplyEquipCostume( kCharInfo );
#endif
	g_MyInfo.ApplyCharDecoration( dwCharIndex, kCharInfo );

	MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(g_GUIMgr.FindWnd(MY_INVENTORY_WND));
	if( pInvenWnd && pInvenWnd->IsShow() )
	{
		pInvenWnd->UpdateTab( false, true );
	}

	SoldierSelectWnd *pSoldierWnd = dynamic_cast<SoldierSelectWnd*>(g_GUIMgr.FindWnd( SOLDIER_SELECT_WND ));
	if( pSoldierWnd && pSoldierWnd->IsShow() )
	{
		pSoldierWnd->SettingSoldierBtn();
	}

	UISoldierSelectWnd *pUISoldierWnd = dynamic_cast<UISoldierSelectWnd*>( g_GUIMgr.FindWnd( UI_SOLDIERSELECT_WND ) );
	if( pUISoldierWnd && pUISoldierWnd->IsShow() )
	{
		pUISoldierWnd->SettingSoldierBtn();
	}

	MiniSoldierSelectWnd *pMiniSoldierWnd = dynamic_cast<MiniSoldierSelectWnd*>( g_GUIMgr.FindWnd( MINI_SOLDIER_SELECT_WND ));
	if( pMiniSoldierWnd && pMiniSoldierWnd->IsShow() )
	{
		pMiniSoldierWnd->SettingMiniSoldierBtn();
	}

	ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
	if( pPlayStage )
		pPlayStage->ReLoadCharInfo( dwCharIndex, kCharInfo );

	// ���� ����ȭ
	SP2Packet kPacket( CUPK_CHANGE_CHARINFO );
	kPacket << dwCharIndex << kCharInfo;
	g_MyInfo.FillCharItemData( dwCharIndex, kPacket );
	P2PNetwork::SendToAllUser( kPacket );
}


void ioApplication::OnMoveSlotItem( SP2Packet &rkPacket )
{
	SetMouseBusy( false );
}

void ioApplication::OnSellSlotItem( SP2Packet &rkPacket )
{
	SetMouseBusy( false );
}

void ioApplication::OnSellEquipItem( SP2Packet &rkPacket )
{
}

void ioApplication::OnIncreseStat( SP2Packet &rkPacket )
{
	SetMouseBusy( false );

	int iResult;
	rkPacket >> iResult;

	switch( iResult )
	{
	case INC_STAT_OK:
		g_MyInfo.ApplyIncreseStat( rkPacket );
		break;
	case INC_STAT_MAX:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		break;
	case INC_STAT_CHAR_JOINED:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		break;
	case INC_STAT_EXCEPTION:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		break;
	}
	
}

void ioApplication::OnInitStat( SP2Packet &rkPacket )
{
	SetMouseBusy( false );

	int iResult;
	rkPacket >> iResult;

	switch( iResult )
	{
	case INIT_STAT_OK:
		g_MyInfo.ApplyInitStat( rkPacket );
		break;
	case INIT_STAT_CHAR_JOINED:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		break;
	case INIT_STAT_EXCEPTION:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );	
		break;
	}
}

void ioApplication::OnLevelUpItem( SP2Packet &rkPacket )
{
	SetMouseBusy( false );

	int iResult;
	rkPacket >> iResult;

	switch(iResult)
	{
	case LEVELUP_ITEM_OK:
		_OnLevelUpItemOK( rkPacket );
		break;
	case LEVELUP_ITEM_NOT_EXIST_DB:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		break;
	case LEVELUP_ITEM_NOT_EXIST_RAM:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		break;
	case LEVELUP_ITEM_NOT_ENOUGH_EXPERT:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
	    break;
	case LEVELUP_ITEM_NOT_ENOUGH_PESO:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
	    break;
	case LEVELUP_ITEM_FAIL_SAVE:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
	    break;
	}
}

void ioApplication::_OnLevelUpItemOK( SP2Packet &rkPacket )
{
}

void ioApplication::OnCharDelete( SP2Packet &rkPacket )
{
	SetMouseBusy( false );

	int   iResult     = 0;
	DWORD dwCharIndex = 0;
	int   iResellPeso = 0;
	__int64 iMoney    = 0;
	rkPacket >> iResult >> dwCharIndex >> iResellPeso >> iMoney;
	
	switch(iResult)
	{
	case DELETE_EXCEPTION:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return;
	case DELETE_SELECT_CHAR:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return;
	case DELETE_EXERCISE_CHAR_FAIL:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		return;
	case DELETE_CHAR_OK_REQUEST_BANKRUPT:
		g_MyInfo.SetMoney( 0 );
		SetMouseBusy( true );
		break;
	}

	int iDeleteClassType		= g_MyInfo.GetClassType( dwCharIndex );
	int iDeleteCharPowerUpGrade	= g_MyInfo.GetCharPowerUpGrade( dwCharIndex );
	ioHashString szIconName		= g_MyInfo.GetMySoldierIconName( iDeleteClassType );

	bool bCalcSoldierSet = false;
	if ( g_RSoldierInfoManager.IsSoldierSet( g_MyInfo.GetCharacterToClassType( iDeleteClassType ) ) )
		bCalcSoldierSet = true;

	g_MyInfo.CharDelete( dwCharIndex );

	if ( bCalcSoldierSet )
		g_MyInfo.CalcSoldierSetCnt();

	ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
	if( pPlayStage )
	{
		pPlayStage->RemoveMySoldier( dwCharIndex );
		
		//R Soldier Info
		if ( pPlayStage->GetModeType() == MT_MYROOM )
			g_MyInfo.CalcRCharPossessionCount();
	}

	g_MyInfo.SetMoney( iMoney );

	if( iResult == DELETE_CHAR_OK )
	{
		ComplexStringPrinterVec kTitle;
		ComplexStringPrinterVec kDesc;

		if( iResellPeso > 0 )
		{
			char szConvertNum[MAX_PATH] = "";
			Help::ConvertNumToStrComma( iResellPeso, szConvertNum, sizeof( szConvertNum ) );

			// Title
			ioComplexStringPrinter kTitle1;
			kTitle1.SetTextStyle( TS_NORMAL );
			kTitle1.SetBkColor( 0, 0, 0 );	
			kTitle1.SetTextColor( TCT_DEFAULT_DARKGRAY );
			kTitle1.AddTextPiece( FONT_SIZE_17, STR(6), g_MyInfo.GetClassName( iDeleteClassType, iDeleteCharPowerUpGrade ) );
			kTitle.push_back( kTitle1 );

			ioComplexStringPrinter kTitle2;
			kTitle2.SetTextStyle( TS_NORMAL );
			kTitle2.SetBkColor( 0, 0, 0 );
			kTitle2.SetTextColor( TCT_DEFAULT_DARKGRAY );
			kTitle2.AddTextPiece( FONT_SIZE_17, STR(7), szConvertNum );
			kTitle.push_back( kTitle2 );

			// Desc
			ioComplexStringPrinter kDesc1;
			kDesc1.SetTextStyle( TS_NORMAL );
			kDesc1.SetBkColor( 0, 0, 0 );	
			kDesc1.SetTextColor( TCT_DEFAULT_DARKGRAY );
			kDesc1.AddTextPiece( FONT_SIZE_12, STR(8), g_MyInfo.GetClassName( iDeleteClassType, iDeleteCharPowerUpGrade ) );
			kDesc.push_back( kDesc1 );

			ioComplexStringPrinter kDesc2;
			kDesc2.SetTextStyle( TS_NORMAL );
			kDesc2.SetBkColor( 0, 0, 0 );
			kDesc2.SetTextColor( TCT_DEFAULT_DARKGRAY );
			kDesc2.AddTextPiece( FONT_SIZE_12, STR(9), szConvertNum );
			kDesc.push_back( kDesc2 );
		}

		MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(g_GUIMgr.FindWnd(MY_INVENTORY_WND));
		if( pInvenWnd && pInvenWnd->IsShow() )
		{
			//pInvenWnd->UpdateData();
			pInvenWnd->UpdateTab( false, false );
			
			if( iResellPeso > 0 )
				pInvenWnd->ShowItemRecvSellInfoWnd( ItemRecvSellInfoWnd::ITEM_SELL, ItemRecvSellInfoWnd::ITEM_SOLDIER, szIconName, kTitle, kDesc, -1, 0, false, false, g_UIImageRenderMgr.GetPowerUpGradeType( iDeleteClassType ) );
		}

		//�Ļ� Ȯ��
		if( g_MyInfo.IsBankruptcy() )
		{	
			g_GUIMgr.ShowWnd( BANKRUPTCY_CHAR_WND );
		}

#if defined( USE_GA )
		char chCharIndex[32]	= {0,};
		char chSlash[16]		= {0,};
		char chPostData[256]	= {0,};

		if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
		{
			sprintf_e( chCharIndex, "%d", dwCharIndex );
			sprintf_e( chSlash, "%%2F" );
			sprintf_e( chPostData, "%sPESO%sGET%sHERO%s%s", chSlash, chSlash, chSlash, chSlash, chCharIndex );
		}
		else
		{
			SafeSprintf( chCharIndex, sizeof(chCharIndex), "%1", dwCharIndex );
			SafeSprintf( chSlash, sizeof(chSlash), "%2F" );
			SafeSprintf( chPostData, sizeof(chPostData), "%1PESO%2GET%3HERO%4%5", chSlash, chSlash, chSlash, chSlash, chCharIndex );
		}		

		// PESO_GET_HERO
		g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
			, "Peso"
			, "Hero"
			, ""
			, iResellPeso
			, chPostData );
#endif

	}

	SoldierSelectWnd *pSelectWnd = dynamic_cast<SoldierSelectWnd*>(g_GUIMgr.FindWnd( SOLDIER_SELECT_WND ));
	if( pSelectWnd && pSelectWnd->IsShow() )
		pSelectWnd->SettingSoldierBtn();

	MiniSoldierSelectWnd *pMiniSoldierWnd = dynamic_cast<MiniSoldierSelectWnd*>( g_GUIMgr.FindWnd( MINI_SOLDIER_SELECT_WND ));
	if( pMiniSoldierWnd && pMiniSoldierWnd->IsShow() )
		pMiniSoldierWnd->SettingMiniSoldierBtn();

	ExerciseSoldierEventWnd *pExerciseSoldierEventWnd = dynamic_cast<ExerciseSoldierEventWnd*> (g_GUIMgr.FindWnd( EXERCISE_SOLDIER_EVENT_WND ) );
	if( pExerciseSoldierEventWnd )
		pExerciseSoldierEventWnd->UpdateChildPos();
}

void ioApplication::OnCharDisassemble( SP2Packet &rkPacket )
{
	SetMouseBusy( false );

	int   iResult     = 0;
	DWORD dwCharIndex = 0;

	int iGainCode = 0;
	int iGainCnt = 0;
	int iSoulStoneCode = 0;
	int iGainSoulStoneCnt = 0;
	bool bSendPresent = false;

	rkPacket >> iResult >> dwCharIndex >> iGainCode >> iGainCnt;
	rkPacket >> bSendPresent;
	rkPacket >> iSoulStoneCode;
	rkPacket >> iGainSoulStoneCnt;

	switch( iResult )
	{
	case DELETE_EXCEPTION:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(12) );
		return;
	case DELETE_SELECT_CHAR:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(13) );
		return;
	case DELETE_EXERCISE_CHAR_FAIL:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(14) );
		return;
	case DELETE_LIMIT_TIME_FAIL:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(15) );
		return;
	case DELETE_CHAR_OK_REQUEST_BANKRUPT:
		g_MyInfo.SetMoney( 0 );
		SetMouseBusy( true );
		break;
	}

	int iDeleteClassType		= g_MyInfo.GetClassType( dwCharIndex );
	int iDeleteCharPowerUpGrade	= g_MyInfo.GetCharPowerUpGrade( dwCharIndex );
	ioHashString szIconName		= g_MyInfo.GetMySoldierIconName( iDeleteClassType );

	bool bCalcSoldierSet = false;
	if ( g_RSoldierInfoManager.IsSoldierSet( g_MyInfo.GetCharacterToClassType( iDeleteClassType ) ) )
		bCalcSoldierSet = true;

	g_MyInfo.CharDelete( dwCharIndex );

	if ( bCalcSoldierSet )
		g_MyInfo.CalcSoldierSetCnt();

	ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
	if( pPlayStage )
	{
		pPlayStage->RemoveMySoldier( dwCharIndex );

		//R Soldier Info
		if ( pPlayStage->GetModeType() == MT_MYROOM )
			g_MyInfo.CalcRCharPossessionCount();
	}

	if( iResult == DELETE_CHAR_OK )
	{
		ioAlchemicInventory *pAlchemic = g_MyInfo.GetAlchemicInventory();
		if( !bSendPresent && pAlchemic )
			pAlchemic->GainAlchemicItem( iGainCode, iGainCnt );
		else if( bSendPresent && pAlchemic )
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(16) );

		ComplexStringPrinterVec vTitle;
		ComplexStringPrinterVec vDesc;

		if( iGainCnt > 0 )
		{
			char szConvertNum[MAX_PATH] = "";
			Help::ConvertNumToStrComma( iGainCnt, szConvertNum, sizeof( szConvertNum ) );

			// Title
			ioComplexStringPrinter kTitle1;
			kTitle1.SetTextStyle( TS_NORMAL );
			kTitle1.SetBkColor( 0, 0, 0 );	
			kTitle1.SetTextColor( TCT_DEFAULT_DARKGRAY );
			kTitle1.AddTextPiece( FONT_SIZE_17, STR(17), g_MyInfo.GetClassName( iDeleteClassType, iDeleteCharPowerUpGrade ) );
			vTitle.push_back( kTitle1 );

			ioComplexStringPrinter kTitle2;
			kTitle2.SetTextStyle( TS_NORMAL );
			kTitle2.SetBkColor( 0, 0, 0 );
			kTitle2.SetTextColor( TCT_DEFAULT_DARKGRAY );

			if( iGainCode > ALCHEMIC_ADDITIVE_DIV )
				kTitle2.AddTextPiece( FONT_SIZE_17, STR(18), g_AlchemicMgr.GetAlchemicItemName(iGainCode).c_str(), szConvertNum );
			else
				kTitle2.AddTextPiece( FONT_SIZE_17, STR(19), g_MyInfo.GetClassName(iGainCode), szConvertNum );

			vTitle.push_back( kTitle2 );

			// Desc
			ioComplexStringPrinter kDesc1;
			kDesc1.SetTextStyle( TS_NORMAL );
			kDesc1.SetBkColor( 0, 0, 0 );	
			kDesc1.SetTextColor( TCT_DEFAULT_DARKGRAY );
			kDesc1.AddTextPiece( FONT_SIZE_12, STR(20), g_MyInfo.GetClassName( iDeleteClassType, iDeleteCharPowerUpGrade ) );
			vDesc.push_back( kDesc1 );

			ioComplexStringPrinter kDesc2;
			kDesc2.SetTextStyle( TS_NORMAL );
			kDesc2.SetBkColor( 0, 0, 0 );
			kDesc2.SetTextColor( TCT_DEFAULT_DARKGRAY );

			if( iGainCode > ALCHEMIC_ADDITIVE_DIV )
				kDesc2.AddTextPiece( FONT_SIZE_12, STR(21), g_AlchemicMgr.GetAlchemicItemName(iGainCode).c_str(), szConvertNum );
			else
				kDesc2.AddTextPiece( FONT_SIZE_12, STR(22), g_MyInfo.GetClassName(iGainCode), szConvertNum );

			vDesc.push_back( kDesc2 );

		}

		ioEtcItemSoulStone* pEtcSoulStone = (ioEtcItemSoulStone*)( g_EtcItemMgr.FindEtcItem( iSoulStoneCode ) );
		if( 0 < iGainSoulStoneCnt && pEtcSoulStone )
		{
			ioUserEtcItem* pUserEtc = g_MyInfo.GetUserEtcItem();
			ioUserEtcItem::ETCITEMSLOT rkSlot;
			if( pUserEtc->GetEtcItem( iSoulStoneCode, rkSlot ) )
			{
				rkSlot.AddUse( iGainSoulStoneCnt );
				pUserEtc->SetEtcItem( rkSlot );
			}
			else
			{
				ioUserEtcItem::ETCITEMSLOT rkNewEtcItem;
				rkNewEtcItem.m_iType	= iSoulStoneCode;
				rkNewEtcItem.m_iValue1	= iGainSoulStoneCnt;
				rkNewEtcItem.m_iValue2	= 0;
				pUserEtc->AddEtcItem( rkNewEtcItem );
			}

			char szConvertNum[MAX_PATH] = "";
			Help::ConvertNumToStrComma( iGainSoulStoneCnt, szConvertNum, sizeof( szConvertNum ) );

			ioComplexStringPrinter kDesc3;
			kDesc3.SetTextStyle( TS_NORMAL );
			kDesc3.SetBkColor( 0, 0, 0 );
			kDesc3.SetTextColor( TCT_DEFAULT_DARKGRAY );
			kDesc3.AddTextPiece( FONT_SIZE_12, STR(1), pEtcSoulStone->GetName().c_str(), szConvertNum );
			vDesc.push_back( kDesc3 );
		}

		MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(g_GUIMgr.FindWnd(MY_INVENTORY_WND));
		if( pInvenWnd && pInvenWnd->IsShow() )
		{
			pInvenWnd->UpdateTab( false, false );

			if( iGainCode > 0 )
				pInvenWnd->ShowItemRecvSellInfoWnd( ItemRecvSellInfoWnd::ITEM_DISASSEMBLE, ItemRecvSellInfoWnd::ITEM_ALCHEMIC, szIconName, vTitle, vDesc, -1, 0, false, false, g_UIImageRenderMgr.GetPowerUpGradeType( iDeleteClassType ) );
		}

		//�Ļ� Ȯ��
		if( g_MyInfo.IsBankruptcy() )
		{	
			g_GUIMgr.ShowWnd( BANKRUPTCY_CHAR_WND );
		}

#if defined( USE_GA )
 		char chLabel[32] = {0,};

		if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
			sprintf_e( chLabel, "%d", dwCharIndex );
		else
			SafeSprintf( chLabel, sizeof(chLabel), "%1", dwCharIndex );

 		// CHAR_DISASSEMBLE
 		g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
 			, "Character"
 			, "Disassemble"
 			, chLabel
 			, 1
 			, "%2FCHAR%2FDISASSEMBLE" );
#endif

	}

	SoldierSelectWnd *pSelectWnd = dynamic_cast<SoldierSelectWnd*>(g_GUIMgr.FindWnd( SOLDIER_SELECT_WND ));
	if( pSelectWnd && pSelectWnd->IsShow() )
		pSelectWnd->SettingSoldierBtn();

	MiniSoldierSelectWnd *pMiniSoldierWnd = dynamic_cast<MiniSoldierSelectWnd*>( g_GUIMgr.FindWnd( MINI_SOLDIER_SELECT_WND ));
	if( pMiniSoldierWnd && pMiniSoldierWnd->IsShow() )
		pMiniSoldierWnd->SettingMiniSoldierBtn();

	ExerciseSoldierEventWnd *pExerciseSoldierEventWnd = dynamic_cast<ExerciseSoldierEventWnd*> (g_GUIMgr.FindWnd( EXERCISE_SOLDIER_EVENT_WND ) );
	if( pExerciseSoldierEventWnd )
		pExerciseSoldierEventWnd->UpdateChildPos();
}

void ioApplication::OnChangeLeaderChar( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );

	bool bSOK;
	rkPacket >> bSOK;

	if( bSOK == false )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
	}
	else
	{
		DWORD dwCharIndex;
		rkPacket >> dwCharIndex;
		g_MyInfo.CharLeaderChange( dwCharIndex );
	}
}

void ioApplication::OnSetMyRentalChar( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );

	int iResult;
	rkPacket >> iResult;
	switch( iResult )
	{
	case SET_MY_RENTAL_OK:
		{
			short sRentalType;
			DWORD dwCharIndex;
			rkPacket >> dwCharIndex >> sRentalType;
			g_MyInfo.CharSetMyRental( dwCharIndex, sRentalType );

			SoldierSelectWnd *pSoldierWnd = dynamic_cast<SoldierSelectWnd*>(g_GUIMgr.FindWnd( SOLDIER_SELECT_WND ));
			if( pSoldierWnd && pSoldierWnd->IsShow() )
			{
				pSoldierWnd->SettingSoldierBtn();
			}

			MiniSoldierSelectWnd *pMiniSoldierWnd = dynamic_cast<MiniSoldierSelectWnd*>( g_GUIMgr.FindWnd( MINI_SOLDIER_SELECT_WND ));
			if( pMiniSoldierWnd && pMiniSoldierWnd->IsShow() )
			{
				pMiniSoldierWnd->SettingMiniSoldierBtn();
			}
		}
		break;
	case SET_MY_RENTAL_SET_LIMIT:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1), Help::GetMaxCharRentSet() );
		break;	
	case SET_MY_RENTAL_GRADE_LIMIT:
	case SET_MY_RENTAL_MORTMAIN_LIMIT:
	case SET_MY_RENTAL_EXERCISE_LIMIT:
	case SET_MY_RENTAL_EXTRAITEM_LIMIT:
	case SET_MY_RENTAL_TIMEGROWTH_LIMIT:
		{
			ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
			kPrinter[0].SetTextStyle( TS_NORMAL );
			kPrinter[0].SetBkColor( 0, 0, 0 );	
			kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
			kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(2) );
		
			kPrinter[1].SetTextStyle( TS_NORMAL );
			kPrinter[1].SetBkColor( 0, 0, 0 );	
			kPrinter[1].SetTextColor( TCT_DEFAULT_GREEN );
			kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(3) );
		
			kPrinter[2].SetTextStyle( TS_NORMAL );
			kPrinter[2].SetBkColor( 0, 0, 0 );	
			kPrinter[2].SetTextColor( TCT_DEFAULT_DARKGRAY );
			kPrinter[2].AddTextPiece( FONT_SIZE_13, STR(4) );
			g_GUIMgr.SetPrevMsgListBoxWithTitle( NULL, MB_OK, NULL, kPrinter, STR(5), STR(6), "", "" );
		}
		break;
	case SET_MY_RENTAL_NONE_CHAR:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(7) );	
		break;
	case SET_MY_RENTAL_USE_LIMIT:
		{
			DWORD dwCharIndex, dwLimitTime;
			rkPacket >> dwCharIndex >> dwLimitTime;

			int iCharArray = g_MyInfo.GetCharArray( dwCharIndex );
			if( iCharArray == -1 )
				return;
			
			const CHARACTER &rkCharInfo = g_MyInfo.GetCharacter( iCharArray );
			g_MyInfo.SetRentalCharTime( rkCharInfo.m_class_type , dwLimitTime );			
			g_MyInfo.CharRentalTimeMsgBox( g_MyInfo.GetCharRentalMinuteToArray( iCharArray ) );
		}
		break;
	}
}

void ioApplication::OnCharLimitCheck( SP2Packet &rkPacket )
{
	SetMouseBusy( false );
	
	int i;
	int iCount;
	rkPacket >> iCount;
	for(i = 0;i < iCount;i++)
	{
		bool bActive;
		int  iTotalSecond;
		DWORD dwIndex;
		rkPacket >> dwIndex >> bActive >> iTotalSecond;
		g_MyInfo.SetCharActive( dwIndex, bActive );
		g_MyInfo.SetCharTotalSecond( dwIndex, iTotalSecond );		
	}

	ProposalEventUserNode *pNode = dynamic_cast<ProposalEventUserNode*> ( g_MyInfo.GetEventUserMgr().GetEventUserNode( EVT_PROPOSAL ) );
	if( pNode && pNode->IsGiveChar() )
	{
		g_GUIMgr.ShowWnd( EVENT_WND );
		if( g_GUIMgr.IsShow( NEW_SHOP_WND ) )
			g_GUIMgr.HideWnd( NEW_SHOP_WND );
	}
	else if( g_MyInfo.IsBankruptcy() )
	{
		// �Ļ�
		g_GUIMgr.ShowWnd( BANKRUPTCY_CHAR_WND );
	}
	else if( iCount != 0 && g_MyInfo.GetActiveCharCount() == 0 )
	{
		ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
		if( pPlayStage )
			pPlayStage->SoldierSelectWndForcedShow();
	}	
	g_GUIMgr.ShowReserveUI();
	g_GUIMgr.ShowReserveMsgBox();
}

void ioApplication::OnStartCharLimitDate( SP2Packet &rkPacket )
{
	int iCharCnt;
	rkPacket >> iCharCnt;
	for(int i = 0;i < iCharCnt;i++)
	{
		bool bActive;
		int  iTotalSecond;
		DWORD dwIndex, dwCheckSecond;
		rkPacket >> dwIndex >> bActive >> iTotalSecond >> dwCheckSecond;
		g_MyInfo.SetCharActive( dwIndex, bActive );
		g_MyInfo.SetCharTotalSecond( dwIndex, iTotalSecond );
		g_MyInfo.StartCharLimitDate( dwIndex, dwCheckSecond );		
	}

	if( g_MyInfo.IsBankruptcy() )
	{
		ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
		if( pLocal && pLocal->IsShowFirstIDWnd() )
		{
			ioWnd *pWnd = g_GUIMgr.FindWnd( FIRST_IDCHANGE_WND );
			if( pWnd )
			{
				if(!pWnd->IsShow())
					g_GUIMgr.ShowWnd( BANKRUPTCY_CHAR_WND );
			}
		}
		else
		{
			g_GUIMgr.ShowWnd( BANKRUPTCY_CHAR_WND );
		}
	}
	else if( g_MyInfo.GetActiveCharCount() == 0 ||
		     g_MyInfo.GetActiveCharCount() == 1 && g_MyInfo.GetActiveExerciseCharCount() == 1 ||
			 g_MyInfo.GetActiveCharCount() == g_MyInfo.GetCharRentalSetCount() )
	{
		ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
		if( pPlayStage && pPlayStage->GetModeType() != MT_MYROOM )
			g_MyInfo.SetRoomOutTimer( FRAMEGETTIME() );
	}
}

void ioApplication::OnUpdatecharLimitDate( SP2Packet &rkPacket )
{
	int iCharCnt;
	rkPacket >> iCharCnt;
	for(int i = 0;i < iCharCnt;i++)
	{
		bool bActive;
		int  iTotalSecond;
		DWORD dwIndex;
		rkPacket >> dwIndex >> bActive >> iTotalSecond;
		g_MyInfo.SetCharActive( dwIndex, bActive );
		g_MyInfo.SetCharTotalSecond( dwIndex, iTotalSecond );
	}
	g_MyInfo.StopCharLimitDate();
}

void ioApplication::OnCharDecorationBuy( SP2Packet &rkPacket )
{
	SetMouseBusy( false );

	int iResult;

	rkPacket >> iResult;
	
	switch(iResult)
	{
	case CHAR_DECORATION_MONEY_FAIL:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return;
	case CHAR_DECORATION_EXCEPTION:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return;
	case CHAR_DECORATION_EXERCISE_FAIL:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		return;
	case CHAR_DECORATION_BILLING_WAIT:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		return;
	case CHAR_DECORATION_BILLING_DISCONNECT:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
		return;
	case CHAR_DECORATION_BILLING_FAIL:
		{
			bool bBillingError    = false;
			ioHashString  sBillingError;
			rkPacket >> bBillingError;
			if( bBillingError )
				rkPacket >> sBillingError;

			ioHashString sErrorMent = STR(6);
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
					StringCbPrintf( szErrorFullMent, sizeof( szErrorFullMent ), "%s",  sError.c_str() ); //Except Extracting Hangeul
				else
					StringCbPrintf( szErrorFullMent, sizeof( szErrorFullMent ), "%s#%s%s", sErrorMent.c_str(), sError.c_str(), sBillingError.c_str() ); //Except Extracting Hangeul
				
				g_GUIMgr.SetMsgBox( MB_OK, NULL, szErrorFullMent );
			}
		}
		return;
	case CHAR_DECORATION_BILLING_WANT_OF_CASH:
		g_GUIMgr.ShowWnd( BILLING_WANT_WND );
		return;
	case CHAR_DECORATION_BILLING_EXCESS_BUY:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(7) );
		return;
	case CHAR_DECORATION_MAX_COUNT:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(8) );
		return;
	}

	__int64 iMoney, iPrevMoney;
	int iCash, iType, iCode, iChannelingCash;
	DWORD dwCharIndex;
	CHARACTER kCharInfo;
	int iBonusPeso = 0;
	iPrevMoney = g_MyInfo.GetMoney();
	rkPacket >> iMoney >> iCash >> iChannelingCash >> iType >> iCode >> dwCharIndex >> kCharInfo >> iBonusPeso;

#if defined( USE_GA )
	bool bGACash	= false;
	bool bGAMoney	= false;
	int iPesoPrice	= g_MyInfo.GetMoney() - iMoney;
	int iCashPrice	= g_MyInfo.GetCash() - iCash;
	if( iCashPrice > 0 && iCashPrice < 1000000 )
		bGACash		= true;
	if( iPesoPrice > 0 )
		bGAMoney	= true;
#endif

	g_MyInfo.SetMoney( iMoney );
	g_MyInfo.SetCash( iCash );
	g_MyInfo.SetChannelingCash( iChannelingCash );
	g_MyInfo.AddSlotItem( iType, iCode );
	g_MyInfo.SetEquipItem( iType, iCode );
	g_MyInfo.ApplyCharDecoration( dwCharIndex, kCharInfo );	

	ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
	if( pPlayStage )
		pPlayStage->ReLoadCharInfo( dwCharIndex, kCharInfo );

	if( iPrevMoney > g_MyInfo.GetMoney() && !IsMeDeveloper() && !IsMeGameMaster() )
	{
		int iBuyPeso = (int)( iPrevMoney - g_MyInfo.GetMoney() );
		g_AwardMgr.AddShopping( g_MyInfo.GetPublicID(), iBuyPeso );
		SP2Packet kPacket1( CUPK_SHOPPING );
		kPacket1 << g_MyInfo.GetPublicID() << iBuyPeso;
		P2PNetwork::SendToAllPlayingUser( kPacket1 );
	}

	// ���� ����ȭ
	SP2Packet kPacket2( CUPK_CHANGE_CHARINFO);
	kPacket2 << dwCharIndex << kCharInfo;
	g_MyInfo.FillCharItemData( dwCharIndex, kPacket2 );
	P2PNetwork::SendToAllUser( kPacket2 );

	CloseGuildMarkPage();

	//if( iBonusPeso > 0 )
	//	g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(8), iBonusPeso );

	NewShopWnd *pNewShopWnd = dynamic_cast<NewShopWnd*> ( g_GUIMgr.FindWnd( NEW_SHOP_WND ) );
	if( pNewShopWnd && pNewShopWnd->IsShow() )
	{
		pNewShopWnd->SetBoughtMark( NewShopWnd::ID_DECO_TAB_BTN, iType%1000, iCode%10000 );
		NewShopBuyWnd *pNewShopBuyWnd = dynamic_cast<NewShopBuyWnd*> ( pNewShopWnd->FindChildWnd( NewShopWnd::ID_BUY_WND ) );
		if( pNewShopBuyWnd && pNewShopBuyWnd->IsShow() )
		{
			pNewShopBuyWnd->ShowOnlyBoughtBtns();
		}
	}

	SoldierSelectWnd *pSoldierWnd = dynamic_cast<SoldierSelectWnd*>(g_GUIMgr.FindWnd( SOLDIER_SELECT_WND ));
	if( pSoldierWnd && pSoldierWnd->IsShow() )
	{
		pSoldierWnd->SettingSoldierBtn();
	}

	MiniSoldierSelectWnd *pMiniSoldierWnd = dynamic_cast<MiniSoldierSelectWnd*>( g_GUIMgr.FindWnd( MINI_SOLDIER_SELECT_WND ));
	if( pMiniSoldierWnd && pMiniSoldierWnd->IsShow() )
	{
		pMiniSoldierWnd->SettingMiniSoldierBtn();
	}

#if defined( USE_GA )

	char chType[32]		= {0,};
	char chParse[32]	= {0,};
	char chCode[32]		= {0,};
	int iSendCode		= 0;

	if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
		sprintf_e( chType, "%d", iType );
	else
		SafeSprintf( chType, sizeof(chType), "%1", iType );
	
	int iSize = strlen( chType ) - 4;
	strncpy( chParse, chType + iSize, 4 );

	// �ڵ� ����
	// "7 (ġ��)" + "000X(����) or 100X(����), (X�� ġ�� ����, ex> �Ӹ����, �Ǻλ�.. )," + "ġ�� ������ ���� index (ex>�Ӹ���� 1,2,3..)"
	// ex> 710031 -> 7 ġ�� + 1003 ����, �Ǻλ� + 1 �Ǻλ� �� ù���� ������.
	// 1 ǥ��, 2 �Ӹ����, 3 �Ǻλ�, 4 �Ӹ���, 7 �ӿ�
	// �ڵ� ����
	// "7 (ġ��)" + "000X(����) or 100X(����), (X�� ġ�� ����, ex> �Ӹ����, �Ǻλ�.. )," + "ġ�� ������ ���� index (ex>�Ӹ���� 1,2,3..)"
	// ex> 710031 -> 7 ġ�� + 1003 ����, �Ǻλ� + 1 �Ǻλ� �� ù���� ������.
	// 1 ǥ��, 2 �Ӹ����, 3 �Ǻλ�, 4 �Ӹ���, 7 �ӿ�
	if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
		sprintf_e( chCode, "%d%s%d", 7, chParse, iCode );
	else
		SafeSprintf( chCode, sizeof(chCode), "%1%2%3", 7, chParse, iCode );

	iSendCode = ioStringConverter::ParseInt( chCode );

	if( bGACash )
	{
		iCashPrice = g_DecorationPrice.GetDecoCashByType( iType, iCode ); 
		if( iCashPrice > 0 )
		{
			// GOLD_USE_DECORATE
			g_HttpMng.GA_ItemHitTracking( g_MyInfo.GetUserIndex() 
				, "Deco"
				, iCashPrice
				, 1
				, iSendCode
				, "Gold"
				, "%2FGOLD%2FUSE%2FDECORATE"
				, true );
		}		
	}
	if( bGAMoney )
	{
		// PESO_USE_DECORATE
		g_HttpMng.GA_ItemHitTracking( g_MyInfo.GetUserIndex() 
			, "Deco"
			, iPesoPrice
			, 1
			, iSendCode
			, "Peso"
			, "%2FPESO%2FUSE%2FDECORATE" );
	}	
#endif

}

void ioApplication::OnCharExtend( SP2Packet &rkPacket )
{
	SetMouseBusy( false );

	int iResult;
	rkPacket >> iResult;
	
	switch(iResult)
	{
	case CHAR_EXTEND_MONEY_FAIL:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return;
	case CHAR_EXTEND_EXCEPTION:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return;
	case CHAR_EXTEND_EXERCISE_CHAR_FAIL:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		return;
	case CHAR_EXTEND_NEED_LEVEL:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		return;
	case CHAR_EXTEND_BILLING_WAIT:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
		return;
	case CHAR_EXTEND_BILLING_DISCONNECT:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6) );
		return;
	case CHAR_EXTEND_BILLING_FAIL:
		{
			bool bBillingError    = false;
			ioHashString  sBillingError;
			rkPacket >> bBillingError;
			if( bBillingError )
				rkPacket >> sBillingError;

			ioHashString sErrorMent = STR(7);
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
					StringCbPrintf( szErrorFullMent, sizeof( szErrorFullMent ), "%s",  sError.c_str() ); //Except Extracting Hangeul
				else
					StringCbPrintf( szErrorFullMent, sizeof( szErrorFullMent ), "%s#%s%s", sErrorMent.c_str(), sError.c_str(), sBillingError.c_str() ); //Except Extracting Hangeul
				
				g_GUIMgr.SetMsgBox( MB_OK, NULL, szErrorFullMent );
			}
		}
		return;
	case CHAR_EXTEND_BILLING_WANT_OF_CASH:
		g_GUIMgr.ShowWnd( BILLING_WANT_WND );
		return;
	case CHAR_EXTEND_BILLING_EXCESS_BUY:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(8) );
		return;
	case CHAR_EXTEND_MORTMAIN_CHAR:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(9) );
		return;
	case CHAR_EXTEND_OK_REQUEST_BANKRUPT:
		SetMouseBusy( true );
		break; // <--- BREAK.
	}

	__int64 iPrevMoney = g_MyInfo.GetMoney();
	__int64 iMoney;
	int iCash;
	int iChannelingCash = 0;
	int iCharArray;
	int iBonusPeso = 0;
	rkPacket >> iMoney >> iCash >> iChannelingCash >> iCharArray >> iBonusPeso;

#if defined( USE_GA )
	bool bGACash	= false;
	bool bGAMoney	= false;
	int iPesoPrice	= g_MyInfo.GetMoney() - iMoney;
	int iCashPrice	= g_MyInfo.GetCash() - iCash;
	if( iCashPrice > 0 && iCashPrice < 1000000 )
		bGACash		= true;
	if( iPesoPrice > 0 )
		bGAMoney	= true;
#endif

	g_MyInfo.SetMoney( iMoney );
	g_MyInfo.SetCash( iCash ); 
	g_MyInfo.SetChannelingCash( iChannelingCash );
    g_MyInfo.ApplyCharExtend( iCharArray, rkPacket );

	SoldierSelectWnd *pSelectWnd = dynamic_cast<SoldierSelectWnd*>(g_GUIMgr.FindWnd( SOLDIER_SELECT_WND ));
	if( pSelectWnd && pSelectWnd->IsShow() )
		pSelectWnd->SettingSoldierBtn();

	UISoldierSelectWnd *pUISoldierWnd = dynamic_cast<UISoldierSelectWnd*>( g_GUIMgr.FindWnd( UI_SOLDIERSELECT_WND ) );
	if( pUISoldierWnd && pUISoldierWnd->IsShow() )
		pUISoldierWnd->SettingSoldierBtn();

	MiniSoldierSelectWnd *pMiniSoldierWnd = dynamic_cast<MiniSoldierSelectWnd*>( g_GUIMgr.FindWnd( MINI_SOLDIER_SELECT_WND ));
	if( pMiniSoldierWnd && pMiniSoldierWnd->IsShow() )
	{
		pMiniSoldierWnd->SettingMiniSoldierBtn();
	}

	if( iResult == CHAR_EXTEND_OK )
	{
		if( iPrevMoney > g_MyInfo.GetMoney() && !IsMeDeveloper() && !IsMeGameMaster() )
		{
			int iBuyPeso = (int)( iPrevMoney - g_MyInfo.GetMoney() );
			g_AwardMgr.AddShopping( g_MyInfo.GetPublicID(), iBuyPeso );
			SP2Packet kPacket( CUPK_SHOPPING );
			kPacket << g_MyInfo.GetPublicID() << iBuyPeso;
			SendToAllPlayingUser( kPacket );
		}		
		g_MyInfo.SetRoomOutTimer( 0 );
	}
	CloseGuildMarkPage();

	//if( iBonusPeso > 0 )
	//	g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(10), iBonusPeso );

	NewShopWnd *pNewShopWnd = dynamic_cast<NewShopWnd*> ( g_GUIMgr.FindWnd( NEW_SHOP_WND ) );
	if( pNewShopWnd && pNewShopWnd->IsShow() )
	{
		NewShopBuyWnd *pNewShopBuyWnd = dynamic_cast<NewShopBuyWnd*> ( pNewShopWnd->FindChildWnd( NewShopWnd::ID_BUY_WND ) );
		if( pNewShopBuyWnd && pNewShopBuyWnd->IsShow() )
			pNewShopBuyWnd->ShowOnlyBoughtBtns();
	}
		
	HelpWnd *pHelpWnd = dynamic_cast<HelpWnd*>( g_GUIMgr.FindWnd( HELP_WND ) );
	if( pHelpWnd )
	{
		pHelpWnd->ReCreateWindow();
	}

#if defined( USE_GA )
	if( bGACash )
	{
		iCashPrice = g_ClassPrice.GetClassBuyCash( g_MyInfo.GetClassType( iCharArray ), g_HttpMng.GetHeroLimitDate() );

		// GOLD_USE_HERO
		g_HttpMng.GA_ItemHitTracking( g_MyInfo.GetUserIndex() 
			, "Hero"
			, iCashPrice
			, 1
			, g_MyInfo.GetClassType( iCharArray )
			, "Gold"
			, "%2FGOLD%2FUSE%2FHERO"
			, true );
	}
	if( bGAMoney )
	{
		// PESO_USE_HERO
		g_HttpMng.GA_ItemHitTracking( g_MyInfo.GetUserIndex() 
			, "Hero"
			, iPesoPrice
			, 1
			, g_MyInfo.GetClassType( iCharArray )
			, "Peso"
			, "%2FPESO%2FUSE%2FHERO" );
	}			
#endif

}

void ioApplication::OnCharCharge( SP2Packet &rkPacket )
{
	SetMouseBusy( false );

	int iResult;
	rkPacket >> iResult;

	switch(iResult)
	{
	case CHAR_CHARGE_MONEY_FAIL:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return;
	case CHAR_CHARGE_EXCEPTION:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return;
	case CHAR_CHARGE_NEED_LEVEL:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		return;
	}

	//���� ����
	g_SoundMgr.PlaySound( "interface\\time_charge_00.wav" );

	__int64 iPrevMoney = g_MyInfo.GetMoney();
	__int64 iMoney;
	int iCharArray;
	rkPacket >> iMoney >> iCharArray;
	g_MyInfo.SetMoney( iMoney );
	g_MyInfo.ApplyCharExtend( iCharArray, rkPacket );

	SoldierSelectWnd *pSelectWnd = dynamic_cast<SoldierSelectWnd*>(g_GUIMgr.FindWnd( SOLDIER_SELECT_WND ));
	if( pSelectWnd && pSelectWnd->IsShow() )
		pSelectWnd->SettingSoldierBtn();

	UISoldierSelectWnd *pUISoldierWnd = dynamic_cast<UISoldierSelectWnd*>( g_GUIMgr.FindWnd( UI_SOLDIERSELECT_WND ) );
	if( pUISoldierWnd && pUISoldierWnd->IsShow() )
		pUISoldierWnd->SettingSoldierBtn();

	MiniSoldierSelectWnd *pMiniSoldierWnd = dynamic_cast<MiniSoldierSelectWnd*>( g_GUIMgr.FindWnd( MINI_SOLDIER_SELECT_WND ));
	if( pMiniSoldierWnd && pMiniSoldierWnd->IsShow() )
	{
		pMiniSoldierWnd->SettingMiniSoldierBtn();
	}

	if( iResult == CHAR_CHARGE_OK && !IsMeDeveloper() && !IsMeGameMaster() )
	{
		int iBuyPeso = (int)( iPrevMoney - g_MyInfo.GetMoney() );
		g_AwardMgr.AddShopping( g_MyInfo.GetPublicID(), iBuyPeso );
		SP2Packet kPacket( CUPK_SHOPPING );
		kPacket << g_MyInfo.GetPublicID() << iBuyPeso;
		SendToAllPlayingUser( kPacket );

		g_MyInfo.SetRoomOutTimer( 0 );
	}
	CloseGuildMarkPage();
}

void ioApplication::OnExitRoomPenalty( SP2Packet &rkPacket )
{
	int iPrevLadderPoint = g_MyInfo.GetLadderPoint();
	int iLadderPoint, iPeso;
	__int64 iMoney;
	rkPacket >> iMoney >> iLadderPoint >> iPeso;
	g_MyInfo.SetMoney( iMoney );	
	g_MyInfo.SetLadderPoint( iLadderPoint );

	if( iPeso > 0 )
	{
		g_GUIMgr.SetPenaltyPeso( iPeso );
		g_GUIMgr.SetPenaltyLadderPoint( max( 0, iPrevLadderPoint - iLadderPoint ) );
		g_GUIMgr.SetReserveMsgBox( MB_INFOMATION_PENALTY );
	}

	bool bLosePoint;
	rkPacket >> bLosePoint;
	if( bLosePoint )
	{
		int iRoomStyle, iModeType, iLosePoint;
		rkPacket >> iRoomStyle >> iModeType >> iLosePoint;
		g_MyInfo.AddLoseCount( (RoomStyle)iRoomStyle, (ModeType)iModeType, iLosePoint );
		g_CampMgr.SetHeroMatchLeave();
	}
}

void ioApplication::OnAnnounce( SP2Packet &rkPacket )
{
	ioHashString szAnnounce;
	rkPacket >> szAnnounce;

	if( m_pGameAnnMgr )
	{
		m_pGameAnnMgr->InsertServerAnnounce( szAnnounce, 10000 );
	}
/*	g_GUIMgr.SetMsgBox( MB_OK, NULL, (LPSTR)szAnnounce.c_str() );

	if( m_pGUIMgr )
	{
		if( m_pGUIMgr->IsNoneUIMode() )
			m_pGUIMgr->SetNoneUIMode( false );
	}
*/
}

void ioApplication::OnLogOut( SP2Packet &rkPacket )
{
#if defined( USE_GA )
	// GAME_END_NORMAL
	g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FGAME%2FOVER", 2 );
#endif

	LOG.PrintTimeAndLog( 0, "ExitProgram - 3" );
	SetExitProgram();
}

void ioApplication::OnReserveLogOut( SP2Packet &rkPacket )
{
	if( m_dwReserveLogOutTime != 0 )
		return;

	IORandom	ioRand;
	ioRand.Randomize();
	rkPacket >> m_dwReserveLogOutType;
	m_dwReserveLogOutTime = FRAMEGETTIME();
	m_dwReserveLogOutCnt = m_dwReserveLogOutTime;
	m_dwReserveLogOutRandTime = ioRand.Random(60000) + 30000;

	if( m_pGUIMgr )
	{
		if( m_pGUIMgr->IsNoneUIMode() )
			m_pGUIMgr->SetNoneUIMode( false );
	}
}

void ioApplication::OnDeveloperMacro( SP2Packet &rkPacket )
{
	int iMacro;
	rkPacket >> iMacro;
	
	switch( iMacro )
	{
	case DEVELOPER_ROOM_INFO:
		{
			DeveloperInfoWnd *pDevInfo = dynamic_cast<DeveloperInfoWnd *>(g_GUIMgr.FindWnd( DEVELOPER_INFO_WND ));
			if( pDevInfo )
				pDevInfo->OnDeveloperInfo( rkPacket );
		}
		break;
	case DEVELOPER_ROOM_ENTER:
		{
			int iLevel;
			rkPacket >> iLevel;
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1), iLevel );
		}
		break;
	case DEVELOPER_EXTRAITEM_GROWTH_CATALYST_INFO:
		{
			DWORD dwItemCode, dwItemCount, dwItemDate;
			rkPacket >> dwItemCode >> dwItemCount >> dwItemDate;

			g_ChatMgr.SetSystemMsg( STR(2), (int)dwItemCode, (int)dwItemCount, (int)dwItemDate );
		}
		break;
	case DEVELOPER_SUPERGASHAPON_LIMIT_INFO_GET:
		{
			DWORD dwEtcItemType, dwLimit, dwMaxLimit;
			rkPacket >> dwEtcItemType >> dwLimit >> dwMaxLimit;

			ioEtcItem* pEtcItem = g_EtcItemMgr.FindEtcItem( dwEtcItemType ); 
			if( !pEtcItem )
			{
				g_ChatMgr.SetSystemMsg( STR(2), dwEtcItemType );
				return;
			}

			if( g_SuperGashaponMgr.IsSuperGashpon( dwEtcItemType ) && 0 < dwMaxLimit )
			{	
				g_ChatMgr.SetSystemMsg( STR(3), pEtcItem->GetName().c_str(), dwLimit, dwMaxLimit );
				g_ChatMgr.SetSystemMsg( STR(4), pEtcItem->GetName().c_str(), dwMaxLimit - dwLimit, dwMaxLimit );
				return;								
			}

			if( dwLimit == 0 && dwMaxLimit == 0 )
				g_ChatMgr.SetSystemMsg( STR(5), pEtcItem->GetName().c_str() );
			else
				g_ChatMgr.SetSystemMsg( STR(6), pEtcItem->GetName().c_str(), dwLimit, dwMaxLimit );
		}
		break;
	case DEVELOPER_ATTENDANCE_RESET:
		{
			AttendanceWnd* pWnd = dynamic_cast<AttendanceWnd*>( g_GUIMgr.FindWnd( ATTENDANCE_WND ) );
			if( pWnd && pWnd->IsShow() )
			{
				pWnd->HideWnd();
			}

			g_MyInfo.ResetAttendanceRecord();
		}
		break;
	case DEVELOPER_ATTENDANCE_SET_DATE:
		{
			AttendanceWnd* pWnd = dynamic_cast<AttendanceWnd*>( g_GUIMgr.FindWnd( ATTENDANCE_WND ) );
			if( pWnd && pWnd->IsShow() )
			{
				pWnd->HideWnd();
			}
		}
		break;
	case DEVELOPER_HOUSING_TEST:
		{
			ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
			if( pPlayStage )
			{
				ioBlockWorld* pWorld = pPlayStage->GetBlockWorld();
				if( pWorld )
				{
					pWorld->TestMapBlock( rkPacket );
				}
			}
		}
		break;
	}	
}

void ioApplication::OnGradeNClassUpBonus( SP2Packet &rkPacket )
{
	int iUpCount;
	rkPacket >> iUpCount;
	for(int i = 0;i < iUpCount;i++)
	{
		int iBonusType;
		rkPacket >> iBonusType;
		switch( iBonusType )
		{
		case GRADE_UP_BONUS_PESO:
			{
				int iGradeLevel, iBonusPeso;
				rkPacket >> iGradeLevel >> iBonusPeso;
				g_MyInfo.SetLevelUP( iBonusType, iGradeLevel, iBonusPeso, 0 );
			}
			break;
		case GRADE_UP_BONUS_CLASS:
			{
				int iGradeLevel, iBonusClass, iLimitDate;
				rkPacket >> iGradeLevel >> iBonusClass >> iLimitDate;
				g_MyInfo.SetLevelUP( iBonusType, iGradeLevel, iBonusClass, iLimitDate );
			}
			break;
		case CLASS_UP_BONUS_PESO:
			{
				int iClassType, iBonusPeso;
				rkPacket >> iClassType >> iBonusPeso;
				g_MyInfo.SetLevelUP( iBonusType, iClassType, iBonusPeso, 0 );
			}
			break;
		case CLASS_UP_BONUS_CLASS:
			{
				int iClassType, iBonusClass, iLimitDate;
				rkPacket >> iClassType >> iBonusClass >> iLimitDate;
				g_MyInfo.SetLevelUP( iBonusType, iClassType, iBonusClass, iLimitDate );
			}
			break;
		case GRADE_UP_BONUS_ETCITEM:
			{
				g_MyInfo.SetLevelUP( iBonusType, 0, 0, 0 );
			}
		}
	}
	__int64 iMyPeso;
	rkPacket >> iMyPeso;
	g_MyInfo.SetMoney( iMyPeso );
}

void ioApplication::OnReserveServerMoving( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( true );
	int iModeType, iSubNum, iModeMapNum, iPlazaType;
	bool bSendSerachCreate = false;
	rkPacket >> iModeType >> iSubNum >> iModeMapNum >> iPlazaType;

	char szCall[MAX_PATH];
	sprintf_s( szCall, "%s - (%d) - (%d) - (%d) - (%d)" ,__FUNCTION__, iModeType, iSubNum, iModeMapNum, iPlazaType );

	// �ε� ���� ����
	SetNextStage( ioGameStage::GS_PLAY, szCall );
	SetModeType( iModeType, iSubNum, iModeMapNum, (PlazaType)iPlazaType );
	g_TCPSocketMgr.ReserveServerMoving();
}

void ioApplication::OnMovingServer( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( true );

	int iModeType, iSubNum, iMapNum, iRoomNum, iRoomIndex, iPlazaType;
	ioHashString szServerIP;
	int iPort;
	int iMovingValue, iServerIndex;
	rkPacket >> iModeType >> iSubNum >> iMapNum >> iPlazaType >> iRoomNum >> iRoomIndex;
	rkPacket >> szServerIP >> iPort;
	rkPacket >> iMovingValue >> iServerIndex;

	char szCall[MAX_PATH];
	sprintf_s( szCall, "%s - (%d), (%d), (%d), (%d)" ,__FUNCTION__, iModeType, iSubNum, iMapNum, iPlazaType );

	if( (ModeType)iModeType == MT_NONE )
	{
		if( g_App.GetConnectedMoveMode() == CMM_LOBBY )
		{
			SetNextStage( ioGameStage::GS_LOBBY, szCall );
		}
		else
		{
			SendSearchModeJoin();
			return;			
		}
	}
	else
	{
		SetNextStage( ioGameStage::GS_PLAY, szCall ); 
	}

	SetModeType( iModeType, iSubNum, iMapNum, (PlazaType)iPlazaType );
	SetRoomNum( iRoomNum );
	g_P2PChecker.Initialize();
	g_TCPSocketMgr.SetServerChange( szServerIP, iPort, iMovingValue, iServerIndex, iRoomIndex );
#ifdef SRC_SA
	//jal : ���� �̵� ���� apex �α׾ƿ�.
	g_ioApex.End();  
#endif
}

void ioApplication::OnMovingServerComplete( SP2Packet &rkPacket )
{
	g_MyInfo.ApplyExerciseIndex( rkPacket );
	g_TCPSocketMgr.CompleteChangeServer();

	// ���� ��ü�� �Ϸ�Ǿ����� ������ ģ���� �޸� �ִ��� Ȯ���Ѵ�.
	g_FriendManager.SendFriendList();
	g_FriendManager.SendFriendRequestList();
	g_MemoManager.SendOfflineMemo();

#if defined( SRC_SA )
	// ���� �̵��� ������ apex ����.	JCLEE 140311
	g_ioApex.Start();
#endif
}

void ioApplication::OnReserverRoomJoin( SP2Packet &rkPacket ) //���� �̵� �� �� ���� ó��
{
	TCPNetwork::MouseBusy( false );
	
	g_TCPSocketMgr.ChangeServerStateEnd();
	int iResult;
	rkPacket >> iResult;

	char szCall[MAX_PATH];
	sprintf_s( szCall, "%s - (%d)" ,__FUNCTION__, iResult );

	switch( iResult )
	{
	case RESERVE_ROOM_JOIN_OK:
		break;
	case RESERVE_ROOM_MYROON_JOIN_OK:
		break;
	case RESERVE_ROOM_JOIN_ERROR_1:     // �̹� �뿡 �ִ� ���	
		break;
	case RESERVE_ROOM_JOIN_ERROR_2:
	case RESERVE_ROOM_JOIN_ERROR_3:
		{
			// �����Ϸ��� ���� ����.
			// ������ or ������
			if( g_BattleRoomMgr.IsBattleRoom() )
			{
				g_BattleRoomMgr.SetBattleModePlaying( false );
				ioMyRoomMode::SetMyRoomType( MRT_BATTLE );
			}
			else if( g_LadderTeamMgr.IsLadderTeam() )
			{
				g_LadderTeamMgr.SetLadderBattleModePlaying( false );
				ioMyRoomMode::SetMyRoomType( MRT_LADDER );
			}
			else
			{
				ioMyRoomMode::SetMyRoomType( MRT_LOBBY );
			}

			//��Ʋ�� �� ���� ���� ��������� �ϴ� �κ�� �̵�
			if( g_App.GetConnectedMoveMode() == CMM_LOBBY )
			{
				g_App.SetNextStage( ioGameStage::GS_LOBBY, szCall );
				g_App.SetModeType( 0, 0, 0, PT_NONE );
			}
			else
			{
				SendSearchModeJoin();				
			}
			g_GUIMgr.SetReserveMsgBox( MB_SERVER_MOVE_NO_ROOM );
		}
		break;
	case RESERVE_ROOM_JOIN_ERROR_4:
		{
			//
			g_BattleRoomMgr.LeaveRoom();
			g_LadderTeamMgr.LeaveTeam();
			g_ShuffleRoomMgr.LeaveRoom();

			if( g_App.GetConnectedMoveMode() == CMM_LOBBY )
			{
				ioMyRoomMode::SetMyRoomType( MRT_LOBBY );
				g_App.SetNextStage( ioGameStage::GS_LOBBY, szCall );
				g_App.SetModeType( 0, 0, 0, PT_NONE );				
			}
			else
			{
				SendSearchModeJoin();
			}
			g_GUIMgr.SetReserveMsgBox( MB_SERVER_MOVE_DELAY );
		}
		break;
	}
}

void ioApplication::OnTotalRegUserCount( SP2Packet &rkPacket )
{
	int iTotalUserRegCnt;
	rkPacket >> iTotalUserRegCnt;

	g_MyInfo.SetTotalUserRegCnt( iTotalUserRegCnt );
}

void ioApplication::OnExerciseCharCreate( SP2Packet &rkPacket )
{
	SetMouseBusy( false );

	ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );

	int iResult = 0;
	rkPacket>>iResult;

	// error
	if( iResult != EXERCISE_CHAR_CREATE_OK )
	{
		if ( iResult == EXERCISE_CHAR_CREATE_EXCEPTION )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		} 
		else if( iResult == EXERCISE_CHAR_CREATE_FAIL_MAKE_INDEX )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		}
		else if( iResult == EXERCISE_CHAR_CREATE_FAIL_WORKING )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		}
		else if( iResult == EXERCISE_CHAR_CREATE_NEED_LEVEL )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		}
		else if( iResult == EXERCISE_CHAR_CREATE_BILLING_WAIT )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
		}
		else if( iResult == EXERCISE_CHAR_CREATE_EVENT_END )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6) );
			
			g_EventMgr.SetAlive( EVT_CHILDRENDAY, false );
			SoldierSelectWnd *pSelectWnd = dynamic_cast<SoldierSelectWnd*>(g_GUIMgr.FindWnd( SOLDIER_SELECT_WND ));	
			if( pSelectWnd && pSelectWnd->IsShow() )
			{
				pSelectWnd->ReSetWindow();
			}

			UISoldierSelectWnd *pUISoldierWnd = dynamic_cast<UISoldierSelectWnd*>( g_GUIMgr.FindWnd( UI_SOLDIERSELECT_WND ) );
			if( pUISoldierWnd && pUISoldierWnd->IsShow() )
				pUISoldierWnd->SettingSoldierBtn();

			MiniSoldierSelectWnd *pMiniSoldierWnd = dynamic_cast<MiniSoldierSelectWnd*>( g_GUIMgr.FindWnd( MINI_SOLDIER_SELECT_WND ));
			if( pMiniSoldierWnd && pMiniSoldierWnd->IsShow() )
				pMiniSoldierWnd->SettingMiniSoldierBtn();
		}

		if( pPlayStage )
		{
			ioBaseChar *pOwner = pPlayStage->GetOwnerChar();
			if( pOwner )
				pOwner->SetState( CS_DELAY );
		}
		return;
	}
	
	DWORD dwCharDeleteIndex;
	rkPacket >> dwCharDeleteIndex;
	if( dwCharDeleteIndex != 0 )
	{
		// ĳ���� ����
		g_MyInfo.CharDelete( dwCharDeleteIndex );

		if( pPlayStage )
		{
			pPlayStage->RemoveMySoldier( dwCharDeleteIndex );
		}
	}

	SoldierSelectWnd *pSelectWnd = dynamic_cast<SoldierSelectWnd*>(g_GUIMgr.FindWnd( SOLDIER_SELECT_WND ));	
	UISoldierSelectWnd *pUISoldierWnd = dynamic_cast<UISoldierSelectWnd*>( g_GUIMgr.FindWnd( UI_SOLDIERSELECT_WND ) );
	MiniSoldierSelectWnd *pMiniSoldierWnd = dynamic_cast<MiniSoldierSelectWnd*>( g_GUIMgr.FindWnd( MINI_SOLDIER_SELECT_WND ));
	ExerciseSoldierEventWnd *pExerciseSoldierEventWnd = dynamic_cast<ExerciseSoldierEventWnd*> (g_GUIMgr.FindWnd( EXERCISE_SOLDIER_EVENT_WND ) );
	for (int i = 0; i < g_MyInfo.GetCharCount() ; i++)
	{
		if( g_MyInfo.IsCharExercise(i) )
		{
			if( !g_MyInfo.IsCharExerciseStyle( i, EXERCISE_EVENT ) ) 
				continue;

			if( pExerciseSoldierEventWnd )
				pExerciseSoldierEventWnd->SetBoughtType( SoldierExerciseBtn::SBT_NONE, i );

			DWORD dwDeleteCharIndex = g_MyInfo.DeleteCharData( i );
			g_MyInfo.FixSelectChar();
			if( pPlayStage )
			{
				pPlayStage->RemoveMySoldier( dwDeleteCharIndex );
			}
		}
	}
	int iCharArray   = 0;
	int iTotalMinute = 0;
	CHARACTERDATA kCharData;

	rkPacket >> iCharArray;	
	rkPacket >> kCharData.m_dwIndex >> kCharData.m_data;
	g_MyInfo.AddCharacter( kCharData );

	if( pPlayStage )
	{
		pPlayStage->AddMySoldier( kCharData.m_dwIndex, true );
	}

	if( pSelectWnd && pSelectWnd->IsShow() )
		pSelectWnd->SettingSoldierBtn();

	if( pMiniSoldierWnd && pMiniSoldierWnd->IsShow() )
		pMiniSoldierWnd->SettingMiniSoldierBtn();

	if( pSelectWnd )
	{
		if( kCharData.m_data.m_chExerciseStyle == EXERCISE_EVENT )
		{
			g_MyInfo.SetExerciseEventSoldier( true );        //�̺�Ʈ �뺴�� �޾Ҵ�.
		}
	}

	if( pUISoldierWnd && pUISoldierWnd->IsShow() )
		pUISoldierWnd->SettingSoldierBtn();

	if( kCharData.m_data.m_chExerciseStyle == EXERCISE_EVENT ) 
	{
		if( pExerciseSoldierEventWnd )
			pExerciseSoldierEventWnd->SetBoughtType( SoldierExerciseBtn::SBT_EXERCISE_EVENT, iCharArray );
	}

	MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(g_GUIMgr.FindWnd(MY_INVENTORY_WND));
	if( pInvenWnd && pInvenWnd->IsShow() )
	{
		pInvenWnd->UpdateData();
	}
}

void ioApplication::OnExercisePcRoomCharCreate( SP2Packet &rkPacket )
{
	SetMouseBusy( false );
	ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );

	int iResult = 0;
	rkPacket>>iResult;

	if( iResult != EXERCISE_CHAR_CREATE_OK )
	{
		if ( iResult == EXERCISE_CHAR_CREATE_EXCEPTION )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		} 
		else if( iResult == EXERCISE_CHAR_CREATE_FAIL_MAKE_INDEX )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		}
		else if( iResult == EXERCISE_CHAR_CREATE_FAIL_WORKING )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		}
		else if( iResult == EXERCISE_CHAR_CREATE_NEED_LEVEL )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		}
		else if( iResult == EXERCISE_CHAR_CREATE_BILLING_WAIT )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
		}
		else if( iResult == EXERCISE_CHAR_CREATE_EVENT_END )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );

			SoldierSelectWnd *pSelectWnd = dynamic_cast<SoldierSelectWnd*>(g_GUIMgr.FindWnd( SOLDIER_SELECT_WND ));	
			if( pSelectWnd && pSelectWnd->IsShow() )
			{
				pSelectWnd->ReSetWindow();
			}

			UISoldierSelectWnd *pUISoldierWnd = dynamic_cast<UISoldierSelectWnd*>( g_GUIMgr.FindWnd( UI_SOLDIERSELECT_WND ) );
			if( pUISoldierWnd && pUISoldierWnd->IsShow() )
				pUISoldierWnd->SettingSoldierBtn();

			MiniSoldierSelectWnd *pMiniSoldierWnd = dynamic_cast<MiniSoldierSelectWnd*>( g_GUIMgr.FindWnd( MINI_SOLDIER_SELECT_WND ));
			if( pMiniSoldierWnd && pMiniSoldierWnd->IsShow() )
				pMiniSoldierWnd->SettingMiniSoldierBtn();
		}
		else if( iResult == EXERCISE_PCROOM_CHAR_MAX )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6) );
		}
		else
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(7), iResult );
		}

		if( pPlayStage )
		{
			ioBaseChar *pOwner = pPlayStage->GetOwnerChar();
			if( pOwner )
				pOwner->SetState( CS_DELAY );
		}
		return;
	}

	ExcercisePCRoomCharInfoVec PCRoomCharInfoVec;

	int iCharCount;
	rkPacket >> iCharCount;
	for( int i = 0; i < iCharCount; ++i )
	{
		ExcercisePCRoomCharInfo Info;
		rkPacket >> Info.m_Character.m_dwIndex;
		rkPacket >> Info.m_bNewChar;

		if( Info.m_bNewChar )
		{
			rkPacket >> Info.m_Character.m_data;

			const ioClassExpert::CLASSSLOT* pClassSlot = g_MyInfo.GetClassExpertSlot( Info.m_Character.m_data.m_class_type );
			if( pClassSlot && pClassSlot->m_ePowerUpCharGrade != PUGT_NONE )
			{
				Info.m_Character.m_data.m_eCharPowerUpType = pClassSlot->m_ePowerUpCharGrade;
			}
		}

		PCRoomCharInfoVec.push_back( Info );
	}

	for ( int i = 0; i < g_MyInfo.GetCharCount(); )
	{
		const CHARACTER& rkChar = g_MyInfo.GetCharacter( i );
		if( rkChar.m_chExerciseStyle == EXERCISE_NONE ||  rkChar.m_chExerciseStyle == EXERCISE_RENTAL ||  rkChar.m_chExerciseStyle == EXERCISE_EVENT )
		{
			++i;
			continue;
		}

		bool bContinue = false;
		for( ExcercisePCRoomCharInfoVec::const_iterator iter = PCRoomCharInfoVec.begin(); iter != PCRoomCharInfoVec.end(); ++iter )
		{
			const ExcercisePCRoomCharInfo& Info = *iter;
			if( Info.m_Character.m_dwIndex == g_MyInfo.GetCharIndex( i ) && !Info.m_bNewChar )
			{
				bContinue = true;
				break;
			}
		}

		if( bContinue )
		{
			++i;
			continue;
		}

		DWORD dwDelChar = g_MyInfo.DeleteCharData( i );		
		if( pPlayStage )
			pPlayStage->RemoveMySoldier( dwDelChar );
	}

	g_MyInfo.FixSelectChar();

	for( ExcercisePCRoomCharInfoVec::iterator iter = PCRoomCharInfoVec.begin(); iter != PCRoomCharInfoVec.end(); ++iter )
	{
		ExcercisePCRoomCharInfo& Info = *iter;
		if( Info.m_bNewChar )
		{
			g_MyInfo.AddCharacter( Info.m_Character );
			if( pPlayStage )
				pPlayStage->AddMySoldier( Info.m_Character.m_dwIndex, true );
		}
	}

	ExerciseSoldierEventWnd *pExerciseSoldierEventWnd = dynamic_cast<ExerciseSoldierEventWnd*> (g_GUIMgr.FindWnd( EXERCISE_SOLDIER_EVENT_WND ) );
	if( pExerciseSoldierEventWnd && pExerciseSoldierEventWnd->IsShow() )
		pExerciseSoldierEventWnd->UpdateChildPos();

	SoldierSelectWnd *pSelectWnd = dynamic_cast<SoldierSelectWnd*>( g_GUIMgr.FindWnd( SOLDIER_SELECT_WND ) );
	if( pSelectWnd && pSelectWnd->IsShow() )
		pSelectWnd->SettingSoldierBtn();

	MiniSoldierSelectWnd *pMiniSoldierWnd = dynamic_cast<MiniSoldierSelectWnd*>( g_GUIMgr.FindWnd( MINI_SOLDIER_SELECT_WND ) );
	if( pMiniSoldierWnd && pMiniSoldierWnd->IsShow() )
		pMiniSoldierWnd->SettingMiniSoldierBtn();

	UISoldierSelectWnd *pUISoldierWnd = dynamic_cast<UISoldierSelectWnd*>( g_GUIMgr.FindWnd( UI_SOLDIERSELECT_WND ) );
	if( pUISoldierWnd && pUISoldierWnd->IsShow() )
		pUISoldierWnd->SettingSoldierBtn();

	MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>( g_GUIMgr.FindWnd(MY_INVENTORY_WND) );
	if( pInvenWnd && pInvenWnd->IsShow() )
		pInvenWnd->UpdateData();
}

void ioApplication::OnDeleteExpiredCharByPCRoom( SP2Packet &rkPacket )
{
	int iCount;
	rkPacket >> iCount;

	ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );

	for( int i = 0; i < iCount; ++i )
	{
		DWORD dwDelIndex;
		rkPacket >> dwDelIndex;

		if( dwDelIndex != 0 )
		{
			// ĳ���� ����
			g_MyInfo.CharDelete( dwDelIndex );
			if( pPlayStage )
				pPlayStage->RemoveMySoldier( dwDelIndex );
		}
	}
}

void ioApplication::OnExerciseCharDelete( SP2Packet &rkPacket )
{
	int iExerciseChar;
	rkPacket >> iExerciseChar;

#ifdef _DEBUG
	LOG.PrintTimeAndLog( 0, "%s %d", __FUNCTION__, iExerciseChar );
#endif
		
	if( iExerciseChar == EXERCISE_PCROOM )
		return;

	DWORD dwCharIndex = g_MyInfo.GetExerciseCharIndex( iExerciseChar );
	if( dwCharIndex == 0 ) return;

	int iClassType = g_MyInfo.GetClassType( dwCharIndex );
	ExerciseSoldierEventWnd *pExerciseSoldierEventWnd = dynamic_cast<ExerciseSoldierEventWnd*> (g_GUIMgr.FindWnd( EXERCISE_SOLDIER_EVENT_WND ) );
	if( pExerciseSoldierEventWnd )
		pExerciseSoldierEventWnd->SetBoughtType( SoldierExerciseBtn::SBT_NONE, g_MyInfo.GetCharArray( dwCharIndex ) );

	g_MyInfo.DeleteExerciseChar( (byte)iExerciseChar );
	g_MyInfo.FixSelectChar();

	ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
	if( pPlayStage )
	{
		pPlayStage->RemoveMySoldier( dwCharIndex );
	}

	SoldierSelectWnd *pSelectWnd = dynamic_cast<SoldierSelectWnd*>(g_GUIMgr.FindWnd( SOLDIER_SELECT_WND ));
	if( pSelectWnd && pSelectWnd->IsShow() )
		pSelectWnd->SettingSoldierBtn();

	UISoldierSelectWnd *pUISoldierWnd = dynamic_cast<UISoldierSelectWnd*>( g_GUIMgr.FindWnd( UI_SOLDIERSELECT_WND ) );
	if( pUISoldierWnd && pUISoldierWnd->IsShow() )
		pUISoldierWnd->SettingSoldierBtn();

	MiniSoldierSelectWnd *pMiniSoldierWnd = dynamic_cast<MiniSoldierSelectWnd*>( g_GUIMgr.FindWnd( MINI_SOLDIER_SELECT_WND ));
	if( pMiniSoldierWnd && pMiniSoldierWnd->IsShow() )
		pMiniSoldierWnd->SettingMiniSoldierBtn();
}

//////////////////////////////////////////////////////////////////////////
void ioApplication::OnMemoSendMsg( SP2Packet &rkPacket )
{
	int iResult;
	rkPacket >> iResult;

	ioHashString szToID;

	switch( iResult )
	{
	case MEMO_SEND_MSG:
		g_MemoManager.OnlineMemo( rkPacket );
		break;
	case MEMO_SEND_NOT_USER:
		rkPacket >> szToID;
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1), szToID.c_str() );
		break;
	case MEMO_SEND_MSG_OK:
		rkPacket >> szToID;
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2), szToID.c_str() );
		break;
	}
}

void ioApplication::OnOfflineMemo( SP2Packet &rkPacket )
{
	g_MemoManager.OfflineMemo( rkPacket );
}

void ioApplication::OnFriendList( SP2Packet &rkPacket )
{
	g_FriendManager.ApplyFriendList( rkPacket );
}

void ioApplication::OnFriendRequestList( SP2Packet &rkPacket )
{
	g_FriendManager.ApplyFriendRequestUser( rkPacket );
}

void ioApplication::OnFriendApplication( SP2Packet &rkPacket )
{
	SetMouseBusy( false );

	// ����� ����
	int iResult;
	rkPacket >> iResult;

	switch( iResult )
	{
	case FRIEND_APPLICATION_OK:
		{
			ioHashString szFriendID;
			rkPacket >> szFriendID;

			ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
			kPrinter[0].SetTextStyle( TS_NORMAL );
			kPrinter[0].SetBkColor( 0, 0, 0 );	
			kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
			kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(9), szFriendID.c_str() );

			kPrinter[1].SetTextStyle( TS_NORMAL );
			kPrinter[1].SetBkColor( 0, 0, 0 );	
			kPrinter[1].SetTextColor( TCT_DEFAULT_DARKGRAY );
			kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(10), Help::GetFriendBonusBeforeMin() );
			kPrinter[1].SetTextColor( TCT_DEFAULT_GREEN );
			kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(11) );
			kPrinter[1].SetTextColor( TCT_DEFAULT_DARKGRAY );
			kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(12) );

			kPrinter[2].SetTextStyle( TS_NORMAL );
			kPrinter[2].SetBkColor( 0, 0, 0 );	
			kPrinter[2].SetTextColor( TCT_DEFAULT_DARKGRAY );
			kPrinter[2].AddTextPiece( FONT_SIZE_13, STR(13) );
			kPrinter[2].SetTextColor( TCT_DEFAULT_GREEN );
			kPrinter[2].AddTextPiece( FONT_SIZE_13, STR(14) );
			kPrinter[2].SetTextColor( TCT_DEFAULT_DARKGRAY );
			kPrinter[2].AddTextPiece( FONT_SIZE_13, STR(15) );

			g_GUIMgr.SetPrevMsgListBox( NULL, MB_OK, NULL, kPrinter );
			
			g_QuestMgr.QuestCompleteTerm( QC_FRIEND_REQUEST );
		}		
		break;
	case FRIEND_APPLICATION_NOT_ID:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		break;
	case FRIEND_APPLICATION_REQUST_ID:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3), g_MyInfo.GetPublicID().c_str() );
		break;
	case FRIEND_APPLICATION_ALREADY_ID:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		break;
	case FRIEND_APPLICATION_ALREADY_APP:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
		break;
	case FRIEND_APPLICATION_NONE_FORMALITY:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6) );
		break;
	case FRIEND_APPLICATION_EVENT_USER:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(7) );
		break;
	case FRIEND_APPLICATION_MY_SLOT_FULL:
		if( g_FriendManager.GetNodeSize() >= g_FriendManager.GetSlotSize() )
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(8), g_FriendManager.GetSlotSize() );
		else
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(16) );
		break;
	}
}

void ioApplication::OnFriendCommand( SP2Packet &rkPacket )
{
	SetMouseBusy( false );
	
	int iCommand;
	rkPacket >> iCommand;
	switch( iCommand )
	{
	case FRIEND_COMMAND_AGREE:
		{
			ioHashString szFriendID;
			int iResult;
			rkPacket >> iResult >> szFriendID;
			switch( iResult )
			{
			case FRIEND_INSERT_OK:
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(7), szFriendID.c_str(), Help::GetFriendBonusBeforeMin() );
					g_FriendManager.DeleteRequestFriend( szFriendID );
					g_FriendManager.SendFriendList();
				}
				break;
			case FRIEND_INSERT_NOT_ID:
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2), szFriendID.c_str() );
					g_FriendManager.DeleteRequestFriend( szFriendID );
				}
				break;
			case FRIEND_INSERT_MY_SLOT_FULL:
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3), g_FriendManager.GetSlotSize() );
				}
				break;
			case FRIEND_INSERT_FRIEND_SLOT_FULL:
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4), szFriendID.c_str(), g_FriendManager.GetDefaultSlotSize() );
				}
				break;
			case FRIEND_INSERT_OTHER_OK:
				{
					g_FriendManager.SendFriendList();
				}
				break;
			}
		}
		break;
	case FRIEND_COMMAND_BLACKLIST:
		{
			ioHashString szUserName;
			rkPacket >> szUserName;
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5), szUserName.c_str() ); 
		}
		break;
	case FRIEND_COMMAND_NONE_FORMALITY:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6) );
		}
		break;
	}
}

void ioApplication::OnFriendDelete( SP2Packet &rkPacket )
{
	ioHashString szDeleteID;
	rkPacket >> szDeleteID;
	g_FriendManager.DeleteFriend( szDeleteID );

//	if( !g_App.IsDeveloperID( szDeleteID.c_str() ) )
//		g_GUIMgr.SetMsgBox( MB_OK, NULL, "%s���� ģ������ ���ܵǾ����ϴ�.", szDeleteID.c_str() ); 
}

void ioApplication::OnBestFriendList( SP2Packet &rkPacket )
{
	g_FriendManager.ApplyBestFriendList( rkPacket );
}

void ioApplication::OnBestFriendInsert( SP2Packet &rkPacket )
{
	int iResult;
	rkPacket >> iResult;

	if( iResult != BESTFRIEND_INSERT_OTHER_OK )
		TCPNetwork::MouseBusy( false );

	switch( iResult )
	{
	case BESTFRIEND_INSERT_STATE_ERROR:
		{
			DWORD dwFriendUserIndex, dwBFState;
			rkPacket >> dwFriendUserIndex >> dwBFState;
            
			BestFriend &rkBestFriend = g_FriendManager.GetBestFriend( dwFriendUserIndex );
			if( rkBestFriend.m_dwState == BFT_SET )
			{
				FriendNode *pFriendNode = g_FriendManager.GetNodeToUserIndex( dwFriendUserIndex );
				if( pFriendNode )
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1), pFriendNode->GetName().c_str() );
				}
			}
			else if( rkBestFriend.m_dwState != BFT_DISMISS )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
			}
			else
			{
				FriendNode *pFriendNode = g_FriendManager.GetNodeToUserIndex( dwFriendUserIndex );
				if( pFriendNode )
				{
					ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
					kPrinter[0].SetTextStyle( TS_NORMAL );
					kPrinter[0].SetBkColor( 0, 0, 0 );	
					kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
					kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(3), pFriendNode->GetName().c_str() );

					kPrinter[1].SetTextStyle( TS_NORMAL );
					kPrinter[1].SetBkColor( 0, 0, 0 );	
					kPrinter[1].SetTextColor( TCT_DEFAULT_GRAY );  
					kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(4), rkBestFriend.GetMonth(), rkBestFriend.GetDay(), rkBestFriend.GetHour(), rkBestFriend.GetMinute() );

					kPrinter[2].SetTextStyle( TS_NORMAL );
					kPrinter[2].SetBkColor( 0, 0, 0 );	
					kPrinter[2].SetTextColor( TCT_DEFAULT_GRAY );
					kPrinter[2].AddTextPiece( FONT_SIZE_13, STR(5) );		
					g_GUIMgr.SetPrevMsgListBox( NULL, MB_OK, NULL, kPrinter );	
				}
				else
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6) );
				}
			}
		}
		break;
	case BESTFRIEND_INSERT_COUNT_ERROR:
		{
			if( g_FriendManager.GetBestFriendStateCount( BFT_SET ) >= Help::GetDefaultBestFriendCount() )
			{
				ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
				kPrinter[0].SetTextStyle( TS_NORMAL );
				kPrinter[0].SetBkColor( 0, 0, 0 );	
				kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
				kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(7) );

				kPrinter[1].SetTextStyle( TS_NORMAL );
				kPrinter[1].SetBkColor( 0, 0, 0 );	
				kPrinter[1].SetTextColor( TCT_DEFAULT_DARKGRAY );  
				kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(8) );
				kPrinter[1].SetTextColor( TCT_DEFAULT_GREEN );  
				kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(9), g_FriendManager.GetBestFriendStateCount( BFT_SET ) );

				g_GUIMgr.SetPrevMsgListBox( NULL, MB_OK, NULL, kPrinter );	
			}
			else
			{
				BestFriend &rkBestFriend = g_FriendManager.GetBestFriendFirstDismiss();
				if( rkBestFriend.m_dwMagicDate == 0xFFFFFFFF )
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(10) );
				}
				else 
				{
					ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
					kPrinter[0].SetTextStyle( TS_NORMAL );
					kPrinter[0].SetBkColor( 0, 0, 0 );	
					kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
					kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(11) );

					kPrinter[1].SetTextStyle( TS_NORMAL );
					kPrinter[1].SetBkColor( 0, 0, 0 );	
					kPrinter[1].SetTextColor( TCT_DEFAULT_DARKGRAY );  
					kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(12) );
					kPrinter[1].SetTextColor( TCT_DEFAULT_GREEN );  
					kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(13), g_FriendManager.GetBestFriendStateCount( BFT_SET ), g_FriendManager.GetBestFriendStateCount( BFT_DISMISS ) );

					kPrinter[2].SetTextStyle( TS_NORMAL );
					kPrinter[2].SetBkColor( 0, 0, 0 );	
					kPrinter[2].SetTextColor( TCT_DEFAULT_GRAY );
					kPrinter[2].AddTextPiece( FONT_SIZE_13, STR(14), rkBestFriend.GetMonth(), rkBestFriend.GetDay(), rkBestFriend.GetHour(), rkBestFriend.GetMinute() );		
					g_GUIMgr.SetPrevMsgListBox( NULL, MB_OK, NULL, kPrinter );	
				}
			}
		}
		break;
	case BESTFRIEND_INSERT_MEMBER_ERROR:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(15) );
		}
		break;
	case BESTFRIEND_INSERT_FRIEND_ERROR:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(16) );
		}
		break;
	case BESTFRIEND_INSERT_OK:
		{
			DWORD dwTableIndex, dwFriendIndex;
			rkPacket >> dwTableIndex >> dwFriendIndex;			
			FriendNode *pFriendNode = g_FriendManager.GetNodeToUserIndex( dwFriendIndex );
			if( pFriendNode == NULL )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(17) );
		
				// ��ģ ������ ���ÿ� ģ���� �����Ǹ� ��ģ ���з� ó���Ѵ�. - ������ ó���ϸ� 5�ϵ��� ó�� ���ϹǷ� �߿���
				SP2Packet kPacket( CTPK_INSERT_BESTFRIEND_FAILED );
				kPacket << dwFriendIndex;
				TCPNetwork::SendToServer( kPacket );
			}
			else
			{
				g_FriendManager.InsertBestFriend( dwTableIndex, dwFriendIndex );

				ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
				kPrinter[0].SetTextStyle( TS_NORMAL );
				kPrinter[0].SetBkColor( 0, 0, 0 );	
				kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
				kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(18), pFriendNode->GetName().c_str() );

				kPrinter[1].SetTextStyle( TS_NORMAL );
				kPrinter[1].SetBkColor( 0, 0, 0 );	
				kPrinter[1].SetTextColor( TCT_DEFAULT_DARKGRAY );  
				kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(19), Help::GetDefaultBestFriendCount() );
				kPrinter[1].SetTextColor( TCT_DEFAULT_GREEN );  
				kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(20), g_FriendManager.GetBestFriendStateCount( BFT_SET ), g_FriendManager.GetBestFriendStateCount( BFT_DISMISS ) );

				kPrinter[2].SetTextStyle( TS_NORMAL );
				kPrinter[2].SetBkColor( 0, 0, 0 );	
				kPrinter[2].SetTextColor( TCT_DEFAULT_DARKGRAY );
				kPrinter[2].AddTextPiece( FONT_SIZE_13, STR(21) );		
				kPrinter[2].SetTextColor( TCT_DEFAULT_GREEN );
				kPrinter[2].AddTextPiece( FONT_SIZE_13, STR(22) );		
				kPrinter[2].SetTextColor( TCT_DEFAULT_DARKGRAY );
				kPrinter[2].AddTextPiece( FONT_SIZE_13, STR(23) );		
				g_GUIMgr.SetPrevMsgListBox( NULL, MB_OK, NULL, kPrinter );	
			}			
		}
		break;
	case BESTFRIEND_INSERT_OTHER_OK:
		{
			DWORD dwFriendIndex;
			rkPacket >> dwFriendIndex;	
			FriendNode *pFriendNode = g_FriendManager.GetNodeToUserIndex( dwFriendIndex );
			if( pFriendNode )
			{
				if( m_pGUIMgr )
				{
					g_GUIMgr.ShowBestFriendPopup( pFriendNode->GetName(), true );
				}
				g_FriendManager.OnOtherBestFriend( pFriendNode->GetName(), true );
			}
		}
		break;
	}
}

void ioApplication::OnBestFriendDeleteList( SP2Packet &rkPacket )
{
	int iDeleteCount;
	rkPacket >> iDeleteCount;

	for(int i = 0;i < iDeleteCount;i++)
	{
		DWORD dwUserIndex;
		rkPacket >> dwUserIndex;
		g_FriendManager.RemoveBestFriend( dwUserIndex );
	}
}

void ioApplication::OnBestFriendDismiss( SP2Packet &rkPacket )
{
	int iResult;
	rkPacket >> iResult;

	if( iResult != BESTFRIEND_DISMISS_OTHER_OK )
		TCPNetwork::MouseBusy( false );
	
	switch( iResult )
	{
	case BESTFRIEND_DISMISS_FRIEND_ERROR:
		{
			ioHashString kName;
			rkPacket >> kName;
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1), kName.c_str() );
		}
		break;
	case BESTFRIEND_DISMISS_STATE_ERROR:
		{
			ioHashString kName;
			rkPacket >> kName;
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2), kName.c_str() );
		}
		break;
	case BESTFRIEND_DISMISS_OK:
		{
			bool bAlarm;
			ioHashString kName;
			DWORD dwFriendIndex, dwMagicDate;
			rkPacket >> dwFriendIndex >> dwMagicDate >> kName >> bAlarm;
			g_FriendManager.ChangeBestFriendData( dwFriendIndex, BFT_DISMISS, dwMagicDate );

			if( bAlarm )
			{
				UserListWnd::sg_bUserListUpdate =  true;
				int iDismissDay = (float)Help::GetBestFriendDismissDelayHour() / 24;
				if( iDismissDay > 0 )
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3), kName.c_str(), iDismissDay );
				else
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4), kName.c_str(), Help::GetBestFriendDismissDelayHour() );
			}
		}
		break;
	case BESTFRIEND_DISMISS_OTHER_OK:
		{
			ioHashString kName;
			rkPacket >> kName;
			g_FriendManager.OnOtherBestFriend( kName, false );
			g_MyInfo.ReturnRentalCharToOwenrID( kName );
		}
		break;
	}
}

void ioApplication::OnBestFriendExceptionList( SP2Packet &rkPacket )
{
	int iSize;
	rkPacket >> iSize;
	for(int i = 0;i < iSize;i++)
	{
		DWORD dwUserIndex, dwMagicTime;
		rkPacket >> dwUserIndex >> dwMagicTime;
		if( dwMagicTime == 0 ) continue;

		g_FriendManager.ChangeBestFriendData( dwUserIndex, BFT_DISMISS, dwMagicTime );
	}
}

void ioApplication::OnUserPosRefresh( SP2Packet &rkPacket )
{
	int iCurSize = 0;
	rkPacket >> iCurSize;
	for(int i = 0;i < iCurSize;i++)
	{
		bool bOnline, bOtherBestFriend;
		ioHashString szName;
		rkPacket >> szName >> bOnline;
		if( bOnline )
		{
			int iUserPos, iUserLevel;
			DWORD dwGuildIndex, dwGuildMark;
			rkPacket >> iUserPos >> iUserLevel >> dwGuildIndex >> dwGuildMark >> bOtherBestFriend;
			g_GuildInfo.SetMyGuildUserPos( szName, iUserPos, iUserLevel );
			g_FriendManager.OnUserPos( szName, iUserPos, iUserLevel, dwGuildIndex, dwGuildMark );
			g_FriendManager.OnOtherBestFriend( szName, bOtherBestFriend );
			g_MemoManager.OnLogInUser( szName );

			g_GuildInfo.SetGuildMark( szName, dwGuildIndex, dwGuildMark );
		}
		else
		{
			g_GuildInfo.SetMyGuildUserLogOut( szName );
			g_FriendManager.OnLogOutUser( szName );
			g_MemoManager.OnLogOutUser( szName );
		}
	}
}

void ioApplication::OnUserInfoRefresh( SP2Packet &rkPacket )
{
	ioHashString UserID;
	int iRSoldierPossessionCount, nSoldierSetCnt;
	int iGradeLevel, iGradeExpPer, iAwardLevel, iAwardExp, iKillDeathLevel, iFishingLevel, iFishingExp, iExcavationLevel, iExcavationExp, iBlockType, iRanking, iCampPosition, iLadderPoint, iClassLevel, iClassType;
	int iHeroWin, iHeroLose, iHeroTitle, iHeroRank;
	__int64 iPartyExp, iLadderExp;

	int nPetCode, nPetLevel;
	DWORD dwNamedTitleCode;
	BYTE byNamedTitlePremium;
	int nNamedTitleLevel;

	byte eAwakeType = AWAKE_NONE;

	rkPacket >> UserID >> iRSoldierPossessionCount >> nSoldierSetCnt >> iGradeLevel >> iGradeExpPer >> iPartyExp >> iLadderExp >> iAwardLevel >> iAwardExp
			 >> iKillDeathLevel >> iFishingLevel >> iFishingExp >> iExcavationLevel >> iExcavationExp >> iHeroWin >> iHeroLose >> iHeroTitle >> iHeroRank 
			 >> iBlockType >> iRanking >> iCampPosition >> iLadderPoint >> iClassLevel >> iClassType
			 >> nPetCode >> nPetLevel >> dwNamedTitleCode >> byNamedTitlePremium >> nNamedTitleLevel >> eAwakeType;

	bool bHouseOpen;
	rkPacket >> bHouseOpen;

	IntVec vMedalItemTypeVec;
	for (int i = 0; i < ioMedalItemInfoManager::MAX_SLOT_NUM ; i++)
	{
		int iMedalItemType = 0;
		rkPacket >> iMedalItemType;
		vMedalItemTypeVec.push_back( iMedalItemType );
	}

	IntVec vGrowthLevelVec;
	int iMax = ( MAX_CHAR_GROWTH+MAX_ITEM_GROWTH );
	for (int i = 0; i < iMax ; i++)
	{
		int iGrowthLevel = 0;
		rkPacket >> iGrowthLevel;
		vGrowthLevelVec.push_back( iGrowthLevel );
	}
	
	/*
	// ��Ʈ ȿ���� �׷캰�� �ַ��� �Ҷ� ��� (�������� �̱���)
	int nSoldierSetCnt;
	rkPacket >> nSoldierSetCnt;

	IntVec vSoldierSetVec;
	for ( int i=0; i<nSoldierSetCnt; i++ )
	{
		int nSetCnt;
		rkPacket >> nSetCnt;
		vSoldierSetVec.push_back( nSetCnt );
	}
	*/

	IntVec vSoldierSetVec;
	vSoldierSetVec.push_back( nSoldierSetCnt );

	g_UserInfoMgr.SetUserInfo(	UserID, iGradeLevel, iGradeExpPer, iPartyExp, iLadderExp, iAwardLevel, iAwardExp, iKillDeathLevel, iFishingLevel, iFishingExp, 
								iExcavationLevel, iExcavationExp, iHeroWin, iHeroLose, iHeroTitle, iHeroRank, iBlockType, iRanking, iCampPosition, iLadderPoint, iClassLevel,
								iClassType, vMedalItemTypeVec, vGrowthLevelVec, nPetCode, nPetLevel, bHouseOpen, dwNamedTitleCode, nNamedTitleLevel, byNamedTitlePremium, vSoldierSetVec );

	g_UserInfoMgr.SetCharAwake( UserID, iClassType, static_cast<int>( eAwakeType ) );
	
	//�ڽ�Ƭ ���� (���� ���� ���� ����)
	int nCostumeCode;
	DWORD dwCostumeMaleCustom;
	DWORD dwCostumeFemaleCustom;
	for ( int i=0; i<MAX_INVENTORY; i++ )
	{
		rkPacket >> nCostumeCode;
		rkPacket >> dwCostumeMaleCustom >> dwCostumeFemaleCustom;
		g_UserInfoMgr.SetEquipCostumebySubType( UserID, iClassType, i, nCostumeCode, dwCostumeMaleCustom, dwCostumeFemaleCustom );
	}
}

void ioApplication::OnSimpleUserInfoRefresh( SP2Packet &rkPacket )
{
	UserInfoToolTip::UserInfo kUserInfo;
	kUserInfo.m_dwCreateTime	= REALGETTIME();
	kUserInfo.m_dwRefreshTime	= REALGETTIME();
	int iRSoldierCount, nSoliderSetCnt;
	rkPacket >> kUserInfo.m_sName >> iRSoldierCount >> nSoliderSetCnt;
	rkPacket >> kUserInfo.m_iClassType >> kUserInfo.m_iClassLevel;

	for (int i = 0; i < ioMedalItemInfoManager::MAX_SLOT_NUM ; i++)
	{
		int iMedalItemType = 0;
		rkPacket >> iMedalItemType;
		kUserInfo.m_vMedalItemTypeVec.push_back( iMedalItemType );
	}

	int iMax = ( MAX_CHAR_GROWTH+MAX_ITEM_GROWTH );
	for (int i = 0; i < iMax ; i++ )
	{
		int iGrowthLevel = 0;
		rkPacket >> iGrowthLevel;
		kUserInfo.m_vGrowthLevelVec.push_back( iGrowthLevel );
	}
		
	//���� �߰� (��ȭ ������)
	for ( int i = 0; i < 4; i++ )
	{
		int nReinforce;
		rkPacket >> nReinforce;

		if( COMPARE( i, 0, MAX_CHAR_GROWTH ) )
		{
			if ( nReinforce <= g_CompoundMaterialMgr.GetFixedFailReinforce() )
				kUserInfo.m_vGrowthLevelVec[i] += nReinforce;
			else
				kUserInfo.m_vGrowthLevelVec[i] += g_GrowthInfoMgr.GetReinforceAddStat( nReinforce );
		}
		if( COMPARE( i+MAX_ITEM_GROWTH, MAX_ITEM_GROWTH, iMax ) )
		{
			if ( nReinforce <= g_CompoundMaterialMgr.GetFixedFailReinforce() )
				kUserInfo.m_vGrowthLevelVec[i+MAX_ITEM_GROWTH] += nReinforce;
			else
				kUserInfo.m_vGrowthLevelVec[i+MAX_ITEM_GROWTH] += g_GrowthInfoMgr.GetReinforceAddStat( nReinforce );
		}
	}	

	//�� ����(���� ���� ����)
	int nPetCode, nPetLevel;
	rkPacket >> nPetCode >> nPetLevel;

	if ( nPetCode > 0)
	{
		for ( int i=0; i<MAX_CHAR_GROWTH+MAX_ITEM_GROWTH; i++)
			kUserInfo.m_vGrowthLevelVec[i] += g_PetInfoMgr.GetTotalStat( nPetCode, i, nPetLevel );
	}

	//���� ����
	byte eAwakeType;
	rkPacket >> eAwakeType;

	kUserInfo.m_iAwakeType = static_cast<int>( eAwakeType );
	for (int i = 0; i < MAX_CHAR_GROWTH; ++i )
	{
		kUserInfo.m_vGrowthLevelVec[i] += g_AwakeMgr.GetAwakeCharGrowth( static_cast<AwakeType>( kUserInfo.m_iAwakeType ), i );
	}
	for (int i = 0; i < MAX_ITEM_GROWTH; ++i )
	{
		kUserInfo.m_vGrowthLevelVec[i + MAX_CHAR_GROWTH] +=  g_AwakeMgr.GetAwakeCharGrowth( static_cast<AwakeType>( kUserInfo.m_iAwakeType ), i );
	}
	
	byte eCharPowerUpGrade = 0;
	rkPacket >> eCharPowerUpGrade;
	kUserInfo.m_iPowerUpCharGrade = eCharPowerUpGrade;

	//R Soldier
	bool bIsRchar;
	rkPacket >> bIsRchar;
	if ( bIsRchar )
	{
		for ( int i=0; i<MAX_CHAR_GROWTH+MAX_ITEM_GROWTH; i++)
			kUserInfo.m_vGrowthLevelVec[i] += g_RSoldierInfoManager.GetSoldierAddStat( i, iRSoldierCount );
	}

	bool bIsSoliderSet;
	rkPacket >> bIsSoliderSet;
	if ( bIsSoliderSet )
	{
		for ( int i=0; i<MAX_CHAR_GROWTH+MAX_ITEM_GROWTH; i++)
			kUserInfo.m_vGrowthLevelVec[i] += g_RSoldierInfoManager.GetSoldierSetAddStat( i, kUserInfo.m_iClassType, nSoliderSetCnt );
	}
	
	ioHashString szName;
	rkPacket >> szName;
	int iSize;
	rkPacket >> iSize;
	for( int i=0; i<iSize; ++i )
	{
		int iExMedalSlotNum = 0;
		rkPacket >> iExMedalSlotNum;
		kUserInfo.m_vExMedalSlotVec.push_back( iExMedalSlotNum );
	}

	for(int i = 0; i < MAX_INVENTORY; i++ )
	{
		int iItemCode = 0;
		rkPacket >> iItemCode;

		if( iItemCode == 0 )
		{
			if( eCharPowerUpGrade != PUGT_NONE )
			{
				iItemCode = (DEFAULT_BASIC_ITEM_CODE * i)  + ConvertPowerUpCharToItem( eCharPowerUpGrade ) + kUserInfo.m_iClassType;						
				if( !g_PowerUpManager.IsPowerUpGradeItem( kUserInfo.m_iClassType, iItemCode ) || !g_ItemMaker.IsItem( iItemCode ) )
				{
					// �������� ��ؽ��϶� 12, 100012, 200012, 300012 ����					
					iItemCode = (DEFAULT_BASIC_ITEM_CODE * i) + kUserInfo.m_iClassType;
				}
			}
			else
			{
				// �������� ��ؽ��϶� 12, 100012, 200012, 300012 ����
				iItemCode = (DEFAULT_BASIC_ITEM_CODE * i) + kUserInfo.m_iClassType;
			}
			
		}

		kUserInfo.m_vItemCodeVec.push_back( iItemCode );
	}
	
	// UI�ڽ�Ƭ ���� (���� �ڽ�Ƭ ����)
	for(int i=0; i<MAX_INVENTORY; i++ )
	{
		int nCostumeCode = 0;
		rkPacket >> nCostumeCode;
		kUserInfo.m_vCostumeCodeVec.push_back( nCostumeCode );
	}

	// �׼�����
	for(int i=0; i<MAX_ACC_INVENTORY; i++ )
	{
		int iAccessoryCode = 0;
		int iRandomNum;
		rkPacket >> iAccessoryCode;
		rkPacket >> iRandomNum;
		kUserInfo.m_vAccessoryCodeVec.push_back( iAccessoryCode );
	}

	UserInfoToolTip::InsertUserInfo( kUserInfo );

	UserInfoToolTip *pUserInfoToolTip = dynamic_cast<UserInfoToolTip*>( g_GUIMgr.FindWnd( USERINFO_TOOLTIP ) );
	if( pUserInfoToolTip )
		pUserInfoToolTip->SetRefreshDelayTime( 5000 );
}

void ioApplication::OnUserCharInfoRefresh( SP2Packet &rkPacket )
{
	g_UserInfoMgr.ApplyUserInfoCharDataSync( rkPacket );
}

void ioApplication::OnUserCharSubInfoRefresh( SP2Packet &rkPacket )
{
	g_UserInfoMgr.ApplyUserInfoCharSubDataSync( rkPacket );
}

void ioApplication::OnUserCharRentalRequest( SP2Packet &rkPacket )
{
	int iResult;
	rkPacket >> iResult;

	if( iResult != USER_CHAR_RENTAL_AGREE_REQUEST )
		TCPNetwork::MouseBusy( false );

	ioHashString kTargetID;
	rkPacket >> kTargetID;
	switch( iResult )
	{
	case USER_CHAR_RENTAL_REQUEST_OFFLINE:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1), kTargetID.c_str() );
		break;
	case USER_CHAR_RENTAL_REQUEST_NONE_BF:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		break;
	case USER_CHAR_RENTAL_REQUEST_CHAR_SAME:
		{
			int iClassType;
			rkPacket >> iClassType;
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3), g_MyInfo.GetClassName( iClassType, g_MyInfo.GetCharPowerUpGradeByClassType( iClassType ) ) );
		}
		break;
	case USER_CHAR_RENTAL_REQUEST_ALREADY_LIMIT:
		{
			int iMonth, iDay, iHour, iMinute;
			rkPacket >> iMonth >> iDay >> iHour >> iMinute;
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4), iMonth, iDay, iHour, iMinute );
		}
		break;
	case USER_CHAR_RENTAL_REQUEST_ONE_DAY_LIMIT:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5), Help::GetCharRentalCount() );
		}
		break;
	case USER_CHAR_RENTAL_REQUEST_OK:
		{
			int iClassType;
			rkPacket >> iClassType;
			UserInfoNode *pUserInfoNode = g_UserInfoMgr.GetUserInfoNode( kTargetID, false );
			if( pUserInfoNode )
			{
				UserInfoCharData &rkCharData = pUserInfoNode->GetUserCharData();
				CharInfoData &rkChar = rkCharData.GetSlotCharToClassType( iClassType );
				if( rkChar.m_CharInfo.m_class_type == iClassType )
				{
					rkChar.m_dwRentalRequestTime = FRAMEGETTIME();
				}
			}
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6), Help::GetCharRentalMinute() );			
		}
		break;
	case USER_CHAR_RENTAL_AGREE_REQUEST:
		{
			//
			int iClassType;
			rkPacket >> iClassType;

			int iPowerUpChaGrade = PUGT_NONE;
			UserInfoNode *pUserInfoNode = g_UserInfoMgr.GetUserInfoNode( kTargetID, false );
			if( pUserInfoNode )
			{
				UserInfoCharData &rkCharData = pUserInfoNode->GetUserCharData();
				CharInfoData &rkChar = rkCharData.GetSlotCharToClassType( iClassType );
				if( rkChar.m_CharInfo.m_class_type == iClassType )
				{
					iPowerUpChaGrade = rkChar.m_CharInfo.m_eCharPowerUpType;
				}
			}

			CharRentalRequestWnd::SetRentalRequest( kTargetID, iClassType, iPowerUpChaGrade );
		}
		break;
	}
}

void ioApplication::OnUserCharRentalAgree( SP2Packet &rkPacket )
{
	int iResult;
	rkPacket >> iResult;
	
	if( iResult != USER_CHAR_RENTAL_AGREE_REQUEST_OK )
		TCPNetwork::MouseBusy( false );

	switch( iResult )
	{
	case USER_CHAR_RENTAL_AGREE_NONE_CHAR:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		break;
	case USER_CHAR_RENTAL_AGREE_NONE_RENTAL:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		break;
	case USER_CHAR_RENTAL_AGREE_ALREADY_RENTAL:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		break;
	case USER_CHAR_RENTAL_AGREE_NONE_USER:
	case USER_CHAR_RENTAL_AGREE_NONE_BF:
	case USER_CHAR_RENTAL_AGREE_CHAR_SAME:
	case USER_CHAR_RENTAL_AGREE_ALREADY_LIMIT:
	case USER_CHAR_RENTAL_AGREE_ONE_DAY_LIMIT:
	case USER_CHAR_RENTAL_AGREE_NONE_SLOT:
		{
			// �뿩 ��ȯ
			int iClassType;
			rkPacket >> iClassType;
			g_MyInfo.SetRentalCharTime( iClassType, 0 );			
			CharRentalReturnWnd::SetRentalReturn( iClassType, (int)g_MyInfo.GetCharPowerUpGradeByClassType( iClassType ) );

			int iCharArray = g_MyInfo.GetClassArray( iClassType );
			if( iCharArray == -1 ) return;

			DWORD dwCharIndex = g_MyInfo.GetCharIndex( iCharArray );
			g_MyInfo.RemoveRentalTimeData( dwCharIndex );
			SP2Packet kPacket( CTPK_USER_CHAR_RENTAL_TIME_END );
			kPacket << dwCharIndex;
			TCPNetwork::SendToServer( kPacket );
		}
		break;
	case USER_CHAR_RENTAL_AGREE_OK:
		{
			//
			int iClassType;
			rkPacket >> iClassType;
			g_MyInfo.SetRentalCharTime( iClassType, Help::GetCharRentalMinute() );
		}
		break;
	case USER_CHAR_RENTAL_AGREE_REQUEST_OK:
		{
			// �뺴 ����
			ioHashString kOwnerName;
			rkPacket >> kOwnerName;

			DWORD dwCharIndex;
			rkPacket >> dwCharIndex;

			CHARACTER kCharInfo;
			rkPacket >> kCharInfo;
			
			RentalData kRentalData;
			kRentalData.SetOwnerID( kOwnerName );
			kRentalData.ApplyData( rkPacket );
			kRentalData.ApplyCostumeData( kCharInfo.m_EquipCostume );

			ioCharRentalData *pCharRentalData = g_MyInfo.GetCharRentalData();
			if( pCharRentalData )
			{
				pCharRentalData->InsertRentalData( kRentalData );

				CHARACTERDATA kCharData;
				kCharData.m_dwIndex = kRentalData.m_dwCharIndex;
				kCharData.m_data    = kCharInfo;
				g_MyInfo.AddCharacter( kCharData );
			}

			g_MyInfo.SetRentalCharTime( kCharInfo.m_class_type, Help::GetCharRentalMinute() );
			CharRentalAcquireWnd *pCharRentalAcquire = dynamic_cast< CharRentalAcquireWnd * >( g_GUIMgr.FindWnd( CHAR_RENTAL_ACQUIRE_WND ) );
			if( pCharRentalAcquire )
			{
				pCharRentalAcquire->SetRentalAcquire( kOwnerName, kCharInfo.m_class_type );
				pCharRentalAcquire->ShowWnd();
			}

			// �뺴�� �뿩�ߴµ� �̹� ��ģ ������
			FriendNode *pFriendNode = g_FriendManager.GetNode( kOwnerName );
			if( pFriendNode == NULL || pFriendNode->IsOtherBF() == false )
			{
				// �뺴 �ٷ� ��ȯ
				g_MyInfo.ReturnRentalCharToOwenrID( kOwnerName );
			}
		}
		break;
	}

	SoldierSelectWnd *pSoldierWnd = dynamic_cast<SoldierSelectWnd*>(g_GUIMgr.FindWnd( SOLDIER_SELECT_WND ));
	if( pSoldierWnd && pSoldierWnd->IsShow() )
	{
		pSoldierWnd->SettingSoldierBtn();
	}

	MiniSoldierSelectWnd *pMiniSoldierWnd = dynamic_cast<MiniSoldierSelectWnd*>( g_GUIMgr.FindWnd( MINI_SOLDIER_SELECT_WND ));
	if( pMiniSoldierWnd && pMiniSoldierWnd->IsShow() )
	{
		pMiniSoldierWnd->SettingMiniSoldierBtn();
	}
}

void ioApplication::OnUserCharRentalReturn( SP2Packet &rkPacket )
{
	DWORD dwCharIndex;
	rkPacket >> dwCharIndex;

	int iClassType;
	rkPacket >> iClassType;

	int iCharArray =  g_MyInfo.GetCharArray( dwCharIndex );
	if( g_MyInfo.GetCharRentalMinuteToArray( iCharArray ) != 0 )
	{
		g_MyInfo.SetRentalCharTime( iClassType, 0 );			
		CharRentalReturnWnd::SetRentalReturn( iClassType, g_MyInfo.GetCharPowerUpGradeByClassType( iClassType ) );

		g_MyInfo.RemoveRentalTimeData( dwCharIndex );
		SP2Packet kPacket( CTPK_USER_CHAR_RENTAL_TIME_END );
		kPacket << dwCharIndex;
		TCPNetwork::SendToServer( kPacket );
	}
}

void ioApplication::OnHeadquartersOption( SP2Packet &rkPacket )
{
	UserHeadquartersOption *pHeadquartersOption = g_MyInfo.GetUserHeadquartersOption();
	if( pHeadquartersOption )
	{
		pHeadquartersOption->ApplyData( rkPacket );
	}
}

void ioApplication::OnPCRoomAuthority( SP2Packet &rkPacket )
{
	int iExcercisePCRoomCharMax;
	DWORD dwPcroom;
	rkPacket >> dwPcroom;
	rkPacket >> iExcercisePCRoomCharMax;
	g_MyInfo.SetPCRoomAuthority( dwPcroom, iExcercisePCRoomCharMax );
	
#if defined( USE_GA )
	if( dwPcroom > 0 )
	{
		// ACCOUNT_PC_ROOM
		g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FACCOUNT%2FPC_ROOM", 8 );
	}
#endif

	if( !g_MyInfo.GetPCRoomAuthority() )
		return;

	if( g_MyInfo.IsTutorialUser() || !m_pGUIMgr )
	{
		// Ʃ�丮�� ������ �κ� �̵��ÿ� ����ش�.
		g_MyInfo.ReservePCRoomCheck();
	}
	else 
	{
		g_MyInfo.PCRoomQuestCheck();
	}
}

void ioApplication::OnUserLogin( SP2Packet &rkPacket )
{
	int iCount;
	rkPacket >> iCount;
	for(int i = 0;i < iCount;i++)
	{
		ioHashString szName;
		bool         bOnline, bOtherBestFriend;
		rkPacket >> szName >> bOnline;
		if( bOnline )
		{
			int iGradeLevel;
			DWORD dwGuildIndex, dwGuildMark;			
			rkPacket >> iGradeLevel >> dwGuildIndex >> dwGuildMark >> bOtherBestFriend;

			g_GuildInfo.SetMyGuildUserLogIn( szName );			
			g_FriendManager.OnLogInUser( szName, iGradeLevel );
			g_FriendManager.OnOtherBestFriend( szName, bOtherBestFriend );
			g_MemoManager.OnLogInUser( szName );

			g_GuildInfo.SetGuildMark( szName, dwGuildIndex, dwGuildMark );
		}
		else
		{
			g_GuildInfo.SetMyGuildUserLogOut( szName );
			g_FriendManager.OnLogOutUser( szName );
			g_MemoManager.OnLogOutUser( szName );
		}
	}
}

void ioApplication::OnChannelCreate( SP2Packet &rkPacket )
{
	SetMouseBusy( false );

	int iResult;
	rkPacket >> iResult;
	switch( iResult )
	{
	case CHANNEL_CREATE_OK:
	case CHANNEL_ALREADY_CHANNEL:
		{
			int iIndex;
			rkPacket >> iIndex;
			MessengerWnd *pWnd = dynamic_cast<MessengerWnd*>(g_GUIMgr.FindWnd( MESSENGER_WND ));
			if( pWnd )
				pWnd->MoveChannel( iIndex );
		}
		break;
	case CHANNEL_MY_CHANNEL_FULL:
		{
			//���� ó��
			ioHashString szManToManID;
			rkPacket >> szManToManID;

			MessengerWnd *pWnd = dynamic_cast<MessengerWnd*>(g_GUIMgr.FindWnd( MESSENGER_WND ));
			if( pWnd )
			{
				if( pWnd->IsShow() )
					pWnd->ChannelCreate( iResult, szManToManID );
				else
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
			}
		}
		break;
	}
}

void ioApplication::OnChannelInvite( SP2Packet &rkPacket )
{
	SetMouseBusy( false );

	int iResult;
	rkPacket >> iResult;
	switch( iResult )
	{
	case CHANNEL_INVITE_OK:
		{
			int iIndex;
			ioHashString szInviteUser, szJoinUser;
			rkPacket >> iIndex >> szInviteUser >> szJoinUser;
			g_ChannelManager.OnInviteUser( iIndex, szInviteUser, szJoinUser );
		}
		break;
	case CHANNEL_INVITE_NOT_USER:
	case CHANNEL_INVITE_USER_NOT_CHANNEL:
	case CHANNEL_INVITE_USER_ALREADY_CHANNEL:
		{
			//���� ó��
			int iIndex;
			ioHashString szInvitedUser;
			rkPacket >> iIndex >> szInvitedUser;

			ChannelNode *pNode = g_ChannelManager.GetNodeIndex( iIndex );
			if( pNode )
			{
				if( pNode->GetManToManID() == szInvitedUser )
					pNode->SetManToManID( "" );
			}

			MessengerWnd *pWnd = dynamic_cast<MessengerWnd*>(g_GUIMgr.FindWnd( MESSENGER_WND ));
			if( pWnd && pWnd->IsShow() )
				pWnd->InviteResult( iResult, szInvitedUser );
		}
		break;
	}
}

void ioApplication::OnChannelJoin( SP2Packet &rkPacket )
{
	int iIndex, iUserSize;
	ioHashString szJoinName, szManToManID;
	bool bManToManChat;
	rkPacket >> iIndex >> iUserSize >> szJoinName >> szManToManID >> bManToManChat;
	if( bManToManChat )
	{
		ioHashString szClearManToMan;
		rkPacket >> szClearManToMan;
		g_ChannelManager.LeaveExceptionChannel( szClearManToMan, iIndex );
	}
	g_ChannelManager.OnJoinUser( iIndex, iUserSize, szJoinName, szManToManID );
}

void ioApplication::OnChannelLeave( SP2Packet &rkPacket )
{
	SetMouseBusy( false );

	int iIndex, iUserSize;
	ioHashString szLeaveName, szManToManID;
	rkPacket >> iIndex >> iUserSize >> szLeaveName >> szManToManID;
	g_ChannelManager.OnLeaveUser( iIndex, iUserSize, szLeaveName, szManToManID );
}

void ioApplication::OnChannelChat( SP2Packet &rkPacket )
{
	int iIndex;
	ioHashString szID, szMsg;
	rkPacket >> iIndex >> szID >> szMsg;

	g_MannerTrialChatMgr.Add( ioMannerTrialChatManager::TT_CHANNEL_CHAT, szID.c_str(), szMsg.c_str() ); // ���������� �Ұ�

	char szChatBuf[MAX_PATH] = "";
	StringCbCopy( szChatBuf, MAX_PATH, g_SlangUtil.ConvertStringWithPreLine( szMsg.c_str() ) );
	szMsg = szChatBuf;

	g_ChannelManager.OnChat( iIndex, szID, szMsg.c_str() );
}

void ioApplication::OnChannelAllUser( SP2Packet &rkPacket )
{
	g_ChannelManager.OnChannelAllUser( rkPacket );
}

void ioApplication::OnRegisteredUser( SP2Packet &rkPacket )
{
	SetMouseBusy( false );

	int iAccountIndex = -1;
	rkPacket >> iAccountIndex;

	if( iAccountIndex != -1 )
	{
#ifndef SRC_ID
		if( m_pBrowserMgr && ( !m_pBrowserMgr->GetHeadQuaterURL().IsEmpty() ) )
		{
			char szURL[MAX_PATH];
			StringCbPrintf( szURL, sizeof( szURL ), "%s%d", m_pBrowserMgr->GetHeadQuaterURL().c_str(), iAccountIndex );
			m_pBrowserMgr->OpenBrowser( szURL, BROWSE_SINGLE, BROWSE_SUB_QUEATER );

			ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
			if( pPlayStage )
			{
				ioBaseChar *pOwner = pPlayStage->GetOwnerChar();
				if( pOwner )
				{
					pOwner->SetHomePageEmoticon();
				}
			}
		}
#else
		if( m_pBrowserMgr )
		{
			char szURL[MAX_PATH];

			ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
			if( pLocal )
				StringCbPrintf( szURL, sizeof( szURL ), "%s", pLocal->GetWebShopUrl() );//, iAccountIndex );
			else
				StringCbPrintf( szURL, sizeof( szURL ), "%s%d", "" , iAccountIndex );

			m_pBrowserMgr->OpenBrowser( szURL, BROWSE_WEB_SHOP, BROWSE_SUB_WEB_SHOP );
			
			ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
			if( pPlayStage )
			{
				ioBaseChar *pOwner = pPlayStage->GetOwnerChar();
				if( pOwner )
				{
					pOwner->SetHomePageEmoticon();
				}
			}
		}
#endif
	}
	else
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1), m_ReserveOpenHeadQuater.c_str() );
	}
}

void ioApplication::OnHackAnnounce( SP2Packet &rkPacket )
{
	// ���� ���� ������ �ƿ� �����Ƿ� ������ ����Ÿ�ӿ����� ��������������� ��� �ִ�... 
	HackProblemWnd *pHack = dynamic_cast<HackProblemWnd*>(g_GUIMgr.FindWnd( HACK_PROBLEM_WND ));
	if( pHack )
	{
		pHack->SetHackAnnounce( rkPacket );
	}
}

void ioApplication::OnHackQuiz( SP2Packet &rkPacket )
{
	HackProblemWnd *pHack = dynamic_cast<HackProblemWnd*>(g_GUIMgr.FindWnd( HACK_PROBLEM_WND ));
	if( pHack )
	{
		if( !pHack->SetHackQuiz( rkPacket ) )
		{
			KickToHeadQuater( EXIT_ROOM_SPEEDHACK );
		}
	}
}

void ioApplication::OnAbuseAnnounce( SP2Packet &rkPacket )
{
	// ���� ���� ������ �ƿ� �����Ƿ� ������ ����Ÿ�ӿ����� ��������������� ��� �ִ�... 
	AbuseProblemWnd *pAbuse = dynamic_cast<AbuseProblemWnd*>(g_GUIMgr.FindWnd( ABUSE_PROBLEM_WND ));
	if( pAbuse )
	{
		bool bOpenSpace = false;
		ioPlayStage *pStage = ToPlayStage( m_pGameStage );
		if( pStage && ( pStage->GetModeType() == MT_TRAINING || pStage->GetModeType() == MT_HEADQUARTERS || pStage->GetModeType() == MT_HOUSE ) )
		{
			bOpenSpace = true;
		}

		pAbuse->SetAbuseAnnounce( rkPacket, bOpenSpace );
	}
}

void ioApplication::OnAbuseQuiz( SP2Packet &rkPacket )
{
	AbuseProblemWnd *pAbuse = dynamic_cast<AbuseProblemWnd*>(g_GUIMgr.FindWnd( ABUSE_PROBLEM_WND ));
	if( pAbuse )
	{
		pAbuse->SetAbuseQuiz( rkPacket );
	}
}

void ioApplication::OnTutorialStepBonus( SP2Packet &rkPacket )
{
	int iCurStep, iBonusPeso;
	rkPacket >> iCurStep >> iBonusPeso;
	g_MyInfo.SetMoney( g_MyInfo.GetMoney() + iBonusPeso );
}

void ioApplication::OnBankruptcyPeso( SP2Packet &rkPacket )
{
	SetMouseBusy( false );
	__int64 iMoney = 0;
	rkPacket >> iMoney;
	g_MyInfo.SetMoney( iMoney );
	g_GUIMgr.HideWnd( BANKRUPTCY_CHAR_WND );

	if( !g_GUIMgr.IsShow( SOLDIER_LIMIT_POPUP_WND ) )
	{
		g_GUIMgr.HideWnd( MY_INVENTORY_WND );
		g_GUIMgr.ShowWnd( NEW_SHOP_WND );
	}
}

void ioApplication::OnUserEntryRefresh( SP2Packet &rkPacket )
{
	int iEntryType = 0;
	rkPacket >> iEntryType;
	g_MyInfo.SetEntryType( (EntryType) iEntryType );
}

void ioApplication::KickToHeadQuater( int iKickType, bool bOpenSpace )
{
	ioPlayStage *pPlay = ToPlayStage( m_pGameStage );
	if( !pPlay )
	{
		LOG.PrintTimeAndLog( 0, "ExitProgram - 4" );

#if defined( USE_GA )
		if( GetGAStart() == true )
		{
			char chLabel[32] = {0,};

			if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
				sprintf_e( chLabel, "%d", 4 );
			else
				SafeSprintf( chLabel, sizeof(chLabel), "%1", 4 );

			// GAME_END_ERR_POS
			g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
				, "Game"
				, "Error"
				, chLabel
				, 1
				, "%2FGAME%2FOVER%2FERR"
				, 1 );

			g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FGAME%2FOVER%2FERR", 6, chLabel );
		}		
#endif

		SetExitProgram();
	}
	else
	{
		if( pPlay->GetModeType() == MT_MYROOM )
		{
			LOG.PrintTimeAndLog( 0, "ExitProgram - 5" );

#if defined( USE_GA )
			if( GetGAStart() == true )
			{
				char chLabel[32] = {0,};

				if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
					sprintf_e( chLabel, "%d", 5 );
				else
					SafeSprintf( chLabel, sizeof(chLabel), "%1", 5 );

				// GAME_END_ERR_POS
				g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
					, "Game"
					, "Error"
					, chLabel
					, 1
					, "%2FGAME%2FOVER%2FERR"
					, 1 );

				g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FGAME%2FOVER%2FERR", 6, chLabel );
			}
#endif

			SetExitProgram();	// �������� �濡 �ִ� ��츸 üũ�ϱ� ������ ���� ������� ������ �ʴ´�..
		}
		else
		{
			SetMouseBusy( true );

			pPlay->SendOwnerDamageList();

			SP2Packet kPacket( CTPK_EXIT_ROOM );
			if( iKickType == EXIT_ROOM_KEY_ABUSE && bOpenSpace )
			{
				kPacket << iKickType << 0 << true;
			}
			else if( iKickType == EXIT_ROOM_MACRO_OUT )
			{
				kPacket << iKickType << 0 << true;
			}
			else
			{
				kPacket << iKickType << g_MyInfo.GetCurPenaltyPeso() << true;
			}
			
			TCPNetwork::SendToServer( kPacket );
		}
	}
}

void ioApplication::OnEventData( SP2Packet &rkPacket )
{
	bool bWriteLog = false;
	if( IsMeDeveloper() )
		bWriteLog = true;

#ifndef SHIPPING
	bWriteLog = true;
#endif

#ifdef NO_ADMIN_LOG
	bWriteLog = false;
#endif

	EventUserManager &rEventUserMgr = g_MyInfo.GetEventUserMgr();
	int iSize = 0;
	rkPacket >> iSize;
	g_EventMgr.SetSize( iSize );
	for (int i = 0; i < iSize ; i++)
	{
		// server
		int iEventType = 0;
		int iValueSize = 0;
		bool bAlive    = false;
		int iMaxDBValue= 0;
		int iUseChannelingTypeSize = 0;
		rkPacket >> iEventType;
		rkPacket >> iValueSize;
		rkPacket >> bAlive;
		rkPacket >> iMaxDBValue;
		rkPacket >> iUseChannelingTypeSize;
		if( bWriteLog )
			LOG.PrintTimeAndLog( 0, "SERVER:[Type:%d][Size:%d][Alive:%d][MaxDB:%d][ChannelingSize:%d]", iEventType, iValueSize, bAlive, iMaxDBValue, iUseChannelingTypeSize );
		g_EventMgr.SetEventType( i, (EventType)iEventType );
		g_EventMgr.SetValueSize( (EventType)iEventType, iValueSize );
		g_EventMgr.SetAlive( (EventType)iEventType, bAlive );
		g_EventMgr.SetMaxDBValue( iMaxDBValue );
		g_EventMgr.SetUseChannelingTypeSize( (EventType) iEventType, iUseChannelingTypeSize );
		for (int j = 0; j < iValueSize ; j++)
		{
			int iValue = 0;
			rkPacket >> iValue;
			g_EventMgr.SetValue( (EventType)iEventType, j, iValue );
			if( bWriteLog )
				LOG.PrintTimeAndLog( 0, " [Value%d:%d]", j+1, iValue );
		}

		for (int k = 0; k < iUseChannelingTypeSize ; k++)
		{
			int iChannelingType = 0;
			rkPacket >> iChannelingType;
			g_EventMgr.SetUseChannelingType( (EventType) iEventType, k, (ChannelingType) iChannelingType );
			if( bWriteLog )
				LOG.PrintTimeAndLog( 0, " [ChannelingType%d:%d]", k+1, iChannelingType );
		}

		// �̺�Ʈ ������ üũ
		ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
		if( pPlayStage )
		{
			pPlayStage->CheckEventStruct( iEventType, bAlive );
		}
	}

	// user
	rEventUserMgr.Init(); // g_EventMgr ���õ� ���Ŀ� EventUserMgr �Ҵ�
	for (int i = 0; i < iSize ; i++)
	{
		EventType eType = g_EventMgr.GetType( i );
		int iNodeSize = 0;
		rkPacket >>iNodeSize;
		if( bWriteLog )
			LOG.PrintTimeAndLog( 0, "USER:[Type:%d][Size:%d]", (int)eType , iNodeSize );
		for (int k = 0; k < iNodeSize ; k++)
		{
			int iValue = 0;
			rkPacket >> iValue;
			rEventUserMgr.SetValue( eType, k, iValue);
			if( bWriteLog )
				LOG.PrintTimeAndLog( 0, " [NodeValue%d:%d]", k+1, iValue );
		}
	}
	
	g_EventMgr.LoadINI();

	if( g_EventMgr.IsAlive( EVT_CHANCE_MORTMAIN_CHAR, g_MyInfo.GetChannelingType() ) )
		g_GUIMgr.ShowWnd( EVENT_CHANCE_MORTMAIN_CHAR_WND, true );

	if( g_EventMgr.IsAlive( EVT_PLAYTIME_PRESENT, g_MyInfo.GetChannelingType() ) )
		g_GUIMgr.ShowWnd( PLAYTIME_PRESENT_EVENT_WND, true );

//	if( g_EventMgr.IsAlive( EVT_EXERCISESOLDIER, g_MyInfo.GetChannelingType() ) )
//		g_GUIMgr.ShowWnd( EXERCISE_SOLDIER_EVENT_WND, true );

	if( g_EventMgr.IsAlive( EVT_CONNECTION_TIME, g_MyInfo.GetChannelingType() ) )
		g_GUIMgr.ShowWnd( CONNECTION_TIME_EVENT_WND, true );

	if( g_EventMgr.IsAlive( EVT_CONNECTION_TIME_SELECT_GIFT, g_MyInfo.GetChannelingType() ) )
		g_GUIMgr.ShowWnd( CONNECTION_TIME_SELECT_GIFT_EVENT_WND, true );

	if( g_EventMgr.IsAlive( EVT_ONE_DAY_GIFT, g_MyInfo.GetChannelingType() ) )
	{
		SP2Packet kPacket( CTPK_EVENT_DATA_UPDATE );
		kPacket << EVT_ONE_DAY_GIFT;
		TCPNetwork::SendToServer( kPacket );
	}
	
	if( g_EventMgr.IsAlive( EVT_ONE_DAY_GIFT_2, g_MyInfo.GetChannelingType() ) )
	{
		SP2Packet kPacket( CTPK_EVENT_DATA_UPDATE );
		kPacket << EVT_ONE_DAY_GIFT_2;
		TCPNetwork::SendToServer( kPacket );
	}

	if( g_EventMgr.IsAlive( EVT_CONNECT_AND_PLAYTIME, g_MyInfo.GetChannelingType() ) )
	{
		SP2Packet kPacket( CTPK_EVENT_DATA_UPDATE );
		kPacket << EVT_CONNECT_AND_PLAYTIME;
		TCPNetwork::SendToServer( kPacket );
	}

 	if( m_pRouletteMgr && g_EventMgr.IsAlive( EVT_ROULETTE, g_MyInfo.GetChannelingType() ) )
 	{
 		m_pRouletteMgr->CheckRouletteStruct( true );
 	}
}

void ioApplication::OnEventAlive( SP2Packet &rkPacket )
{
	int  iEventType = 0;
	bool bAlive     = false; 
	rkPacket >> iEventType;
	rkPacket >> bAlive;

	if ( iEventType == EVT_PRESENT_BUY )
		g_EventMgr.SetUnablePresentBonus( bAlive );
	else
	{
		g_EventMgr.SetAlive( (EventType)iEventType, bAlive );

		// �̺�Ʈ ������ üũ
		ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
		if( pPlayStage )
			pPlayStage->CheckEventStruct( iEventType, bAlive );

 		if( m_pRouletteMgr && iEventType == EVT_ROULETTE )
 			m_pRouletteMgr->CheckRouletteStruct( bAlive );
	}

#ifdef _DEBUG
	LOG.PrintTimeAndLog( 0, "%s %d:%d", __FUNCTION__, iEventType, bAlive );
#endif
}

void ioApplication::OnGuildCreate( SP2Packet &rkPacket )
{
	SetMouseBusy( false );
	int iResult;
	rkPacket >> iResult;

	MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(g_GUIMgr.FindWnd(MY_INVENTORY_WND));
	if( pInvenWnd && pInvenWnd->IsShow() )
	{
		pInvenWnd->UpdateTab( false, true );
	}

	switch( iResult )
	{
	case CREATE_GUILD_OK:
		{
			//��� ���� �Ϸ�
			__int64 iMoney;
			rkPacket >> iMoney;
			g_MyInfo.SetMoney( iMoney );
			
			DWORD dwGuildIndex, dwGuildMark;
			ioHashString szGuildName, szGuildPos;
			rkPacket >> dwGuildIndex >> szGuildName >> dwGuildMark >> szGuildPos;

			BOOL bGuildRoomOpen;
			rkPacket >> bGuildRoomOpen;
			
			SYSTEMTIME rkServerTime;
			g_MyInfo.GetServerTime( rkServerTime );
			CTime ServerTime( rkServerTime.wYear, rkServerTime.wMonth, rkServerTime.wDay, rkServerTime.wHour, rkServerTime.wMinute, rkServerTime.wSecond );

			g_GuildInfo.SetMyGuildData( dwGuildIndex, szGuildName, szGuildPos, dwGuildMark, bGuildRoomOpen );
			g_GuildInfo.SetMyGuildTimeSetting( 0, ServerTime.GetTime(), ServerTime.GetTime() );

			ioGuildData* pGuild = g_GuildInfo.GetGuildData( dwGuildIndex );
			if( pGuild )
				pGuild->ProcessUserListSync();

			// ���� �˾� 
			GuildEventPopup::SetEventType( ioGuildInfo::GUILD_CREATE, dwGuildIndex, szGuildName, dwGuildMark );			
		}
		break;
	case CREATE_GUILD_NAME_FAILED:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );		
			return;
		}
		break;
	case CREATE_GUILD_NAME_ALREADY:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );		
			return;
		}
		break;
	case CREATE_GUILD_MONEY_FAILED:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );		
			return;
		}
		break;
	case CREATE_GUILD_ALREADY_GUILD:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );		
			return;
		}
		break;
	case CREATE_GUILD_NONE_FORMALITY:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );		
			return;
		}
		break;
	case CREATE_GUILD_NAME_RESERVE:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6) );		
		}
		break;
	case CREATE_GUILD_CAMP_NONE:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(7) );
		}
		break;
	default:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(8) );		
			return;
		}
		break;
	}
}

void ioApplication::OnMyGuildInfo( SP2Packet &rkPacket )
{
	DWORD dwGuildIndex;
	int   iGuildMark, iGuildEvent;
	ioHashString szGuildName, szGuildPos;
	DWORD dwRecievedTime, dwCreatedTime, dwLoginTime;
	dwCreatedTime = dwRecievedTime = dwLoginTime = 0;
	rkPacket >> dwGuildIndex >> szGuildName >> szGuildPos >> iGuildMark >> iGuildEvent >> dwRecievedTime >> dwCreatedTime >> dwLoginTime;
	BOOL bGuildRoomOpen;
	rkPacket >> bGuildRoomOpen;

	g_GuildInfo.SetMyGuildData( dwGuildIndex, szGuildName, szGuildPos, iGuildMark, bGuildRoomOpen );
	g_GuildInfo.SetMyGuildTimeSetting( dwRecievedTime, dwCreatedTime, dwLoginTime );


	if( m_pGUIMgr && dwGuildIndex != 0 )
	{
		GuildMainWnd *pGuildMainWnd = dynamic_cast< GuildMainWnd* >(g_GUIMgr.FindWnd( GUILD_MAIN_WND ));
		if( pGuildMainWnd )
		{
			if( pGuildMainWnd->IsShow() && pGuildMainWnd->GetGuildIndex() == dwGuildIndex )
				pGuildMainWnd->SetGuildIndex( dwGuildIndex );
		}
	}

	if( iGuildEvent == ioGuildInfo::GUILD_EVENT_NONE ) 
		return;

	// ��� �̺�Ʈ �м� �� �˾�
	switch( iGuildEvent )
	{
	case ioGuildInfo::GUILD_DELETE:
		{
			// ��尡 ��������.
			g_GuildInfo.InitMyGuildData();			
		}
		return;
	case ioGuildInfo::GUILD_LEAVE:
		{
			// ��忡�� Ż��Ǿ���.
			g_GuildInfo.InitMyGuildData();
		}
		break;
	}

	//UI�� �̺�Ʈ ������ �־��ش�.
	GuildEventPopup::SetEventType( iGuildEvent, dwGuildIndex, szGuildName, iGuildMark );
}

void ioApplication::OnEntryDelayGuildLIst( SP2Packet &rkPacket )
{
	g_GuildInfo.ApplyEntryDelayGuildList( rkPacket );
}

void ioApplication::OnGuildEntryDelayMember( SP2Packet &rkPacket )
{
	if( !m_pGUIMgr )
		return;

	// ��� ���� ����� ��� 
	GuildEntryDelayUserWnd *pWnd = dynamic_cast<GuildEntryDelayUserWnd*>(g_GUIMgr.FindWnd( GUILD_ENTRY_DELAY_USER_WND ));
	if( pWnd )
		pWnd->SetEntryDelayUser( rkPacket );
}

void ioApplication::OnGuildInfo( SP2Packet &rkPacket )
{
	g_GuildInfo.SetGuildListInfo( rkPacket );

	//
	GuildMainWnd *pGuildMainWnd = dynamic_cast<GuildMainWnd*>( g_GUIMgr.FindWnd( GUILD_MAIN_WND ) );
	if( pGuildMainWnd && pGuildMainWnd->IsShow() )
	{
		pGuildMainWnd->UpdateGuildInfo();
	}
}

void ioApplication::OnGuildMemberList( SP2Packet &rkPacket )
{
	g_GuildInfo.SetGuildUserList( rkPacket );
}

void ioApplication::OnMyGuildMemberList( SP2Packet &rkPacket )
{
	g_GuildInfo.SetMyGuildUserList( rkPacket );
}

void ioApplication::OnMyGuildLeaveList( SP2Packet &rkPacket )
{
	DWORD dwGuildIndex;
	rkPacket >> dwGuildIndex;

	ioGuildData *pGuildData = g_GuildInfo.GetGuildData( dwGuildIndex );
	if( pGuildData )
	{
		int iLeaveSize;
		rkPacket >> iLeaveSize;
		for(int i = 0;i < iLeaveSize;i++)
		{
			ioHashString szLeaveUserID;
			rkPacket >> szLeaveUserID;
			pGuildData->LeaveGuildUser( szLeaveUserID );
		}
	}
}

void ioApplication::OnGuildJoinerChange( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );

	DWORD dwGuildIndex, dwGuildMaxUser;
	rkPacket >> dwGuildIndex >> dwGuildMaxUser;

	ioGuildData *pGuildData = g_GuildInfo.GetGuildData( dwGuildIndex );
	if( pGuildData )
		pGuildData->SetGuildMaxUser( dwGuildMaxUser );
}

void ioApplication::OnGuildEntryApp( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );
	int iResult;
	rkPacket >> iResult;
	switch( iResult )
	{
	case ENTRY_GUILD_APP_OK:
		{
			DWORD dwGuildIndex;
			rkPacket >> dwGuildIndex;
			g_GuildInfo.AddEntryDelayGuildList( dwGuildIndex );

			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		}
		break;
	case ENTRY_GUILD_APP_ALREADY:
		{
			if( g_GuildInfo.IsGuild() )
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
			else
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6) );
		}
		break;
	case ENTRY_GUILD_APP_ENTRY_FULL:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		break;
	case ENTRY_GUILD_APP_JOINER_FULL:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		break;
	case ENTRY_GUILD_APP_NONE_FORMALITY:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
		break;
	}

	GuildMainWnd *pGuildMainWnd = dynamic_cast< GuildMainWnd* >(g_GUIMgr.FindWnd( GUILD_MAIN_WND ));
	if( pGuildMainWnd )
		pGuildMainWnd->UpdateGuildInfo();
}

void ioApplication::OnGuildEntryAgree( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );
	int iResult;
	rkPacket >> iResult;
	switch( iResult )
	{
	case ENTRY_GUILD_AGREE_OK:
		{
			ioGuildData *pGuildData = g_GuildInfo.GetGuildData( g_GuildInfo.GetGuildIndex() );
			if( pGuildData )
			{
				pGuildData->SetGuildJoinUser( pGuildData->GetGuildJoinUser() + 1 );
			}
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );

			GuildMainWnd *pGuildMainWnd = dynamic_cast< GuildMainWnd* >(g_GUIMgr.FindWnd( GUILD_MAIN_WND ));
			if( pGuildMainWnd )
				pGuildMainWnd->UpdateGuildInfo();
		}
		break;
	case ENTRY_GUILD_AGREE_NOT_USER:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		break;
	case ENTRY_GUILD_AGREE_MAX_ENTRY:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		break;
	}
}

void ioApplication::OnGuildEntryGuildDelete( SP2Packet &rkPacket )
{
	DWORD dwGuildIndex;
	rkPacket >> dwGuildIndex;
	g_GuildInfo.DeleteEntryDelayGuildList( dwGuildIndex );
}

void ioApplication::OnGuildInvitation( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );
	int iResult;
	rkPacket >> iResult;
	
	switch( iResult )
	{
	case GUILD_INVITATION_OK:
		{
			ioHashString szUserName;
			rkPacket >> szUserName;

			g_GUIMgr.HideWnd( GUILD_INVITATION_WND );
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1), szUserName.c_str() );
			g_GuildInfo.SetInvitationUser( szUserName );
		}
		break;
	case GUILD_INVITATION_OFFLINE:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		}
		break;
	case GUILD_INVITATION_GUILD_USER:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
		}
		break;
	case GUILD_INVITATION_EXCEPTION:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(7) );
		}
		break;
	}
}

void ioApplication::OnGuildInvited( SP2Packet &rkPacket )
{
	if( g_GuildInfo.IsGuild() ) return;
	if( g_MyInfo.IsTutorialUser() ) return;
	if( g_QuestMgr.IsInvitedPass() ) return;
	if( g_BattleRoomMgr.IsBattleRoom() )
	{
		if( g_BattleRoomMgr.IsTournamentModeType() || g_BattleRoomMgr.IsBroadcastModeType() ) 
			return;
	}
	
	GuildInvitedWnd *pInvitedWnd = dynamic_cast<GuildInvitedWnd *>(g_GUIMgr.FindWnd( GUILD_INVITED_WND ));
	if( pInvitedWnd )
		pInvitedWnd->SetInvitedInfo( rkPacket );
}

void ioApplication::OnGuildLeave( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );
	int iResult;
	rkPacket >> iResult;
	switch( iResult )
	{
	case GUILD_LEAVE_OK:
		{
			DWORD dwGuildIndex;
			ioHashString szLeaveUserID;
			rkPacket >> dwGuildIndex >> szLeaveUserID;
			ioGuildData *pGuildData = g_GuildInfo.GetGuildData(dwGuildIndex );
			if( pGuildData )
				pGuildData->LeaveGuildUser( szLeaveUserID );

			if( szLeaveUserID == g_MyInfo.GetPublicID() )
			{
				// ��� ��ü�ǹǷ� ������ �ݴ´�					
				if( g_GuildInfo.IsGuildMaster() ) 
				{
					g_GUIMgr.HideWnd( GUILD_MAIN_WND );
				}
				g_GuildInfo.InitMyGuildData();
				g_GuildChatMgr.ClearChatInfoList();

				{   // ��� ����
					__int64 iPrevMoney = g_MyInfo.GetMoney();
					__int64 iNextMoney;
					rkPacket >> iNextMoney;
					g_MyInfo.SetMoney( iNextMoney );
					if( iPrevMoney != g_MyInfo.GetMoney() )
						g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1), iPrevMoney - iNextMoney );
					else
						g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
				}				
			}
			GuildMainWnd *pGuildMainWnd = dynamic_cast< GuildMainWnd* >(g_GUIMgr.FindWnd( GUILD_MAIN_WND ));
			if( pGuildMainWnd && pGuildMainWnd->IsShow())
				pGuildMainWnd->SetGuildIndex( pGuildMainWnd->GetGuildIndex() );
		}
		break;
	case GUILD_LEAVE_NOT_GUILD:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		break;
	case GUILD_LEAVE_MASTER_USER:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		break;
	}
}

void ioApplication::OnGuildMasterChange( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );
	int iResult;
	rkPacket >> iResult;
	switch( iResult )
	{
	case GUILD_MASTER_CHANGE_OK:
		{
			ioHashString szGuildMyPosition;
			DWORD dwGuildMasterIndex;
			rkPacket >> dwGuildMasterIndex >> szGuildMyPosition;
			
			g_GuildInfo.SetMyGuildPosition( szGuildMyPosition );

			ioGuildData *pGuildData = g_GuildInfo.GetGuildData( g_GuildInfo.GetGuildIndex() );
			if( pGuildData )
			{
				pGuildData->ChangeGuildMaster( dwGuildMasterIndex );
				ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
				if( pLocal )
				{
					if( szGuildMyPosition == pLocal->GetGuildMasterPostion() )
					{
						//UI�� �̺�Ʈ ������ �־��ش�.
						GuildEventPopup::SetEventType( ioGuildInfo::GUILD_MASTER, pGuildData->GetGuildIndex(), pGuildData->GetGuildName(), pGuildData->GetGuildMark() );
					}
					else if( szGuildMyPosition == pLocal->GetGuildGeneralPosition() )
					{
						g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1), pGuildData->GetGuildMasterName().c_str() );
					}
				}
			}
		}
		break;
	case GUILD_MASTER_CHANGE_EXCEPTION:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		break;
	case GUILD_MASTER_CHANGE_LEAVEUSER:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		break;
	case GUILD_MASTER_CHANGE_DELAY:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		break;
	}
}

void ioApplication::OnGuildPositionChange( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );
	int iResult;
	rkPacket >> iResult;
	switch( iResult )
	{
	case GUILD_POSITION_CHANGE_OK:
		{
			ioHashString szEditorName;
			ioHashString szUserName, szTargetPosition;
			DWORD dwTargetIndex;
			rkPacket >> szEditorName >> dwTargetIndex >> szUserName >> szTargetPosition;
			ioGuildData *pGuildData = g_GuildInfo.GetGuildData( g_GuildInfo.GetGuildIndex() );
			if( pGuildData )
			{
				ioHashString szPreUserPosition;
				pGuildData->GetUserPos( dwTargetIndex, szPreUserPosition );
				pGuildData->ChangeGuildPosition( dwTargetIndex, szTargetPosition );

				if( szUserName == g_MyInfo.GetPublicID() )
				{
					g_GuildInfo.SetMyGuildPosition( szTargetPosition );
					ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
					if( pLocal )
					{
						if( szTargetPosition == pLocal->GetGuildSecondMasterPosition() )
						{
							//UI�� �̺�Ʈ ������ �־��ش�.
							GuildEventPopup::SetEventType( ioGuildInfo::GUILD_SECOND_MASTER, pGuildData->GetGuildIndex(), pGuildData->GetGuildName(), pGuildData->GetGuildMark() );
						}
						else if( szTargetPosition == pLocal->GetGuildGeneralPosition() )
						{
							if( szPreUserPosition == pLocal->GetGuildAdminPosition() )
							{
								GuildEventPopup::SetEventType( ioGuildInfo::GUILD_HOME_ADMIN_RELEASE, pGuildData->GetGuildIndex(), pGuildData->GetGuildName(), pGuildData->GetGuildMark() );
							}
							else
							{
								GuildEventPopup::SetEventType( ioGuildInfo::GUILD_SECOND_MASTER_DISMISS, pGuildData->GetGuildIndex(), pGuildData->GetGuildName(), pGuildData->GetGuildMark() );
								// ��� ��ũ �������� �� �����Ƿ�...
								CloseGuildMarkPage();
							}
						}
						else if( szTargetPosition == pLocal->GetGuildAdminPosition() )
						{
							GuildEventPopup::SetEventType( ioGuildInfo::GUILD_HOME_ADMIN_SET, pGuildData->GetGuildIndex(), pGuildData->GetGuildName(), pGuildData->GetGuildMark() );
						}
					}
				}			
				else
				{
					if( szEditorName == g_MyInfo.GetPublicID() )
					{
						ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
						if( pLocal )
						{
							if( szTargetPosition == pLocal->GetGuildSecondMasterPosition() )
							{
								g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1), szUserName.c_str() );
							}
							else if( szTargetPosition == pLocal->GetGuildGeneralPosition() )
							{
								if( szPreUserPosition == pLocal->GetGuildAdminPosition() ) 
								{
									g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1), szUserName.c_str() );
								}
								else
								{
									g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2), szUserName.c_str() );
								}
							}
							else if( szTargetPosition == pLocal->GetGuildAdminPosition() )
							{
								g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2), szUserName.c_str() );
							}
						}
					}
				}
			}

			GuildMainWnd *pGuildMainWnd = dynamic_cast<GuildMainWnd*>( g_GUIMgr.FindWnd( GUILD_MAIN_WND ) );
			if( pGuildMainWnd && pGuildMainWnd->IsShow() )
			{
				pGuildMainWnd->UpdateGuildInfo();
			}
		}
		break;
	case GUILD_MASTER_CHANGE_EXCEPTION:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		break;
	}
}

void ioApplication::OnGuildKickOut( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );
	int iResult;
	rkPacket >> iResult;
	switch( iResult )
	{
	case GUILD_KICK_OUT_OK:
		{
			bool  bRequestUser;
			DWORD dwGuildIndex;
			ioHashString szLeaveUserID;
			rkPacket >> dwGuildIndex >> szLeaveUserID >> bRequestUser;
			ioGuildData *pGuildData = g_GuildInfo.GetGuildData(dwGuildIndex );
			if( pGuildData )
				pGuildData->LeaveGuildUser( szLeaveUserID );

			if( szLeaveUserID == g_MyInfo.GetPublicID() )
			{
				GuildEventPopup::SetEventType( ioGuildInfo::GUILD_LEAVE, pGuildData->GetGuildIndex(), pGuildData->GetGuildName(), pGuildData->GetGuildMark() );	
				g_GuildInfo.InitMyGuildData();
				g_GuildChatMgr.ClearChatInfoList();
			}
			else if( bRequestUser )
			{
				// ��� ����
				__int64 iPrevMoney = g_MyInfo.GetMoney();
				__int64 iNextMoney;
				rkPacket >> iNextMoney;
				g_MyInfo.SetMoney( iNextMoney );
				if( iPrevMoney != g_MyInfo.GetMoney() )
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1), szLeaveUserID.c_str(), iPrevMoney - iNextMoney );
				else
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2), szLeaveUserID.c_str() );
			}

			GuildMainWnd *pGuildMainWnd = dynamic_cast< GuildMainWnd* >(g_GUIMgr.FindWnd( GUILD_MAIN_WND ));
			if( pGuildMainWnd && pGuildMainWnd->IsShow() )
				pGuildMainWnd->SetGuildIndex( pGuildMainWnd->GetGuildIndex() );
		}
		break;
	case GUILD_KICK_OUT_EXCEPTION:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		break;
	case GUILD_KICK_OUT_LEAVEUSER:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		break;
	}
}

void ioApplication::OnGuildSimpleData( SP2Packet &rkPacket )
{
	ioHashString szUserID, szGuildName;
	DWORD dwGuildIndex, dwGuildMark;
	rkPacket >> szUserID >> dwGuildIndex >> szGuildName >> dwGuildMark;

	// ��� ����
	g_GuildInfo.SetGuildSimpleInfo( dwGuildIndex, szGuildName, dwGuildMark );

	// ������ ��� ����
	g_UserInfoMgr.SetGuildIndex( szUserID, dwGuildIndex );
}

void ioApplication::OnGuildMarkChange( SP2Packet &rkPacket )
{
	if( g_GuildInfo.IsGuildMaster() || g_GuildInfo.IsGuildSecondMaster() )
		TCPNetwork::MouseBusy( false );

	MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(g_GUIMgr.FindWnd(MY_INVENTORY_WND));
	if( pInvenWnd && pInvenWnd->IsShow() )
		pInvenWnd->UpdateTab( false, true );

	int iResult;
	rkPacket >> iResult;
	switch( iResult )
	{
	case GUILD_MARK_CHANGE_OK:
		{
			bool bMarkBlock;
			__int64 iMyMoney;
			DWORD dwGuildIndex, dwGuildMark;
			rkPacket >> dwGuildIndex >> dwGuildMark >> iMyMoney >> bMarkBlock;
			g_GuildInfo.SetGuildMark( dwGuildIndex, dwGuildMark );
			g_MyInfo.SetMoney( iMyMoney );

			// ��� ������ ����ȭ
			SP2Packet kPacket( CUPK_GUILD_INFO_CHANGE );
			kPacket << g_MyInfo.GetPublicID() << dwGuildIndex << dwGuildMark;
			P2PNetwork::SendToAllPlayingUser( kPacket );

			if( bMarkBlock )
			{
				GuildEventPopup::SetEventType( ioGuildInfo::GUILD_MARK_BLOCK, dwGuildIndex, STR(1), dwGuildMark );
			}
		}
		break;
	case GUILD_MARK_CHANGE_NOT_GUILD:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		break;
	case GUILD_MARK_CHANGE_NOT_MASTER:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		break;
	case GUILD_MARK_CHANGE_NOT_MONEY:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		break;
	case GUILD_MARK_CHANGE_OK_DEVELOPER:
		{
			DWORD dwGuildIndex, dwGuildMark;
			rkPacket >> dwGuildIndex >> dwGuildMark;
			g_GuildInfo.SetGuildMark( dwGuildIndex, dwGuildMark );
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5), dwGuildIndex, dwGuildMark  );
		}
		break;
	case GUILD_MARK_CHANGE_NOT_DEVELOPER:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6) );
		break;
	}	
}

void ioApplication::OnGuildMarkChangeKeyValue( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );
	DWORD dwKeyValue;
	rkPacket >> dwKeyValue;

	if( dwKeyValue == 0 )
	{
		// �̹� �ٸ� ������ ��� ��ũ ���� ���̴�... �ٽ� �õ��ض�.
		if( g_GuildInfo.IsGuildMaster() )
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		else 
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return;
	}

	if( !IsGuildMarkChangeOK() ) return;

	ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
	if( !pPlayStage ) return;
	if( g_App.GetConnectedMoveMode() == ioApplication::CMM_LOBBY )
	{
		if( pPlayStage->GetModeType() != MT_MYROOM )		
			return;		
	}
	else
	{
		if( pPlayStage->GetModeType() != MT_TRAINING && pPlayStage->GetModeType() != MT_HEADQUARTERS && pPlayStage->GetModeType() != MT_HOUSE )
			return;
	}
	if( g_BattleRoomMgr.IsBattleRoom() ) return;
	if( g_LadderTeamMgr.IsLadderTeam() ) return;

	ioUserEtcItem *pUserEtcItem = g_MyInfo.GetUserEtcItem();
	if( !pUserEtcItem || !pUserEtcItem->IsCanUse( ioEtcItem::EIT_ETC_GUILD_MARK_CHANGE ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );		
		return;
	}

	g_GUIMgr.HideWnd( TIMEGATE_SEARCH );

	GuildMarkChangeWnd *pWnd = dynamic_cast<GuildMarkChangeWnd*> (g_GUIMgr.FindWnd( GUILD_MARK_CHANGE_WND ) );
	if( pWnd )
	{
		pWnd->ShowWnd();
		g_GuildInfo.SetGuildMarkChanging( true );
	}
	else
	{
		LOG.PrintTimeAndLog( 0, "%s pWnd == NULL", __FUNCTION__ );
	}
}

void ioApplication::OnGuildChat( SP2Packet &rkPacket )
{
	DWORD dwGuildIndex;
	ioHashString szID, szMsg;
	rkPacket >> dwGuildIndex >> szID >> szMsg;

	g_MannerTrialChatMgr.Add( ioMannerTrialChatManager::TT_GUILD_CHAT, szID.c_str(), szMsg.c_str() ); // ���������� �Ұ�

	char szChatBuf[MAX_PATH] = "";
	StringCbCopy( szChatBuf, MAX_PATH, g_SlangUtil.ConvertStringWithPreLine( szMsg.c_str() ) );
	szMsg = szChatBuf;

	if( g_GuildInfo.GetGuildIndex() == dwGuildIndex )
		g_GuildChatMgr.OnChat( szID, szMsg.c_str() );
}

void ioApplication::OnGuildTitleSync( SP2Packet &rkPacket )
{
	DWORD dwGuildIndex;
	ioHashString szGuildTitle;
	rkPacket >> dwGuildIndex >> szGuildTitle;
	g_GuildInfo.SetGuildTitle( dwGuildIndex, szGuildTitle );
}

void ioApplication::OnGuildExist( SP2Packet &rkPacket )
{
	SetMouseBusy( false );
	bool  bExist = false;
	DWORD dwGuildIndex = 0;
	rkPacket >> bExist;
	if( bExist )
	{
		rkPacket >> dwGuildIndex;
		GuildMainWnd *pGuildMainWnd = dynamic_cast<GuildMainWnd *>(g_GUIMgr.FindWnd( GUILD_MAIN_WND ));
		if( pGuildMainWnd )
			pGuildMainWnd->SetGuildIndex( dwGuildIndex );
	}
	else
	{
		g_ChatMgr.SetSystemMsg( STR(1) );
	}
}

void ioApplication::OnGuildRoomActive( SP2Packet &rkPacket )
{
	SetMouseBusy( false );

	int eResult;
	rkPacket >> eResult;

	switch( eResult )
	{
	case GUILD_ROOM_ACTIVE_SUCCESS:
		{
			ioGuildData *pGuildData = g_GuildInfo.GetGuildData( g_GuildInfo.GetGuildIndex() );
			if( pGuildData )
				pGuildData->SetGuildRoomOpen( TRUE );

			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		}
		break;
	case GUILD_ROOM_NOT_GUILD:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		}
		break;
	case GUILD_ROOM_NOT_GUILD_MASTER:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		}
		break;
	case GUILD_ROOM_ALREADY_ACTIVE:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		}
		break;
	}
}

void ioApplication::OnUserInfoExist( SP2Packet &rkPacket )
{
	SetMouseBusy( false );

	int iReturnType = 0;
	rkPacket >> iReturnType;

	if( m_ReserveCheckExistID.IsEmpty() )
		return;

	if( iReturnType == USER_INFO_EXIST_ONLINE )
	{
		OpenMannerWnd( m_ReserveCheckExistID.c_str(), true );
	}
	else if( iReturnType == USER_INFO_EXIST_OFFLINE )
	{
		OpenMannerWnd( m_ReserveCheckExistID.c_str(), false );
	}
	else if( iReturnType == USER_INFO_EXIST_NO )
	{
		g_ChatMgr.SetSystemMsg( STR(1) );
	}
	else
	{
		g_ChatMgr.SetSystemMsg( STR(2) , iReturnType );
	}

	m_ReserveCheckExistID.Clear();
}

void ioApplication::OnDefaultDecoItem( SP2Packet &rkPacket )
{
	SetMouseBusy( false );

	int iMax;

	rkPacket >> iMax;

	for (int i = 0; i < iMax ; i++)
	{
		int iType, iCode;	
		rkPacket >> iType >> iCode;
		g_MyInfo.AddSlotItem( iType, iCode );
		g_MyInfo.SetEquipItem( iType, iCode );
	}
}

void ioApplication::OnEventDataUpdate( SP2Packet &rkPacket )
{
	int iEventType  = 0;
	int iResult     = 0;
	rkPacket >> iEventType;
	rkPacket >> iResult;

	if( iResult == EVENT_DATA_UPDATE_FAIL )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		LOG.PrintTimeAndLog( 0, "OnEventDataUpdate : Error :%d", iEventType );
		return;
	}

	EventUserNode *pEventNode = g_MyInfo.GetEventUserMgr().GetEventUserNode( (EventType)iEventType );
	if( pEventNode )
		pEventNode->OnRecievePacket( rkPacket, ToPlayStage( m_pGameStage ) );
	else
	{
		LOG.PrintTimeAndLog( 0, "%s Type is Error.", __FUNCTION__, iEventType );
	}
}


void ioApplication::OnProtectCheck( SP2Packet &rkPacket )
{
#ifdef SHIPPING
	return;
#endif

}

void ioApplication::OnProtect( SP2Packet &rkPacket )
{
	int iType = 0;
	rkPacket >> iType;

	if( iType == PROTECT_FAIL_NPROTECT_CHECK )
	{
		if( m_pGUIMgr )
		{
			DisconnectWnd *pDisWnd = dynamic_cast<DisconnectWnd*> ( m_pGUIMgr->FindWnd(DISCONNECT_WND) );
			if( pDisWnd )
			{
				pDisWnd->SetType( DisconnectWnd::TYPE_NPROTECT_EXIT );
				pDisWnd->ShowWnd();
			}
		}
		else
		{
			MBox( NULL, STR(1), STR(2) );
		}
	}
}

void ioApplication::OnRoomStealthEnter( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );

	int iResult;
	rkPacket >> iResult;
	switch( iResult )
	{
	case ROOM_STEALTH_ENTER_EXCEPTION_DEVELOPER:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		break;
	case ROOM_STEALTH_ENTER_EXCEPTION_EXITUSER:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		break;
	case ROOM_STEALTH_ENTER_DIFFERENT_CHANNEL:
		{
			int iServerArray;
			rkPacket >> iServerArray;
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3), iServerArray, iServerArray );
		}
		break;
	}
}

void ioApplication::OnCustomItemSkinUniqueIndex( SP2Packet &rkPacket )
{
	DWORD dwUniqueIndex;
	rkPacket >> dwUniqueIndex;

	if( g_MyInfo.GetUseEtcItem() == ioEtcItem::EIT_ETC_COSTUM_ITEM_SKIN ) 
	{
		CostumeCustomMakeWnd *pCostumeCustomMakeWnd = dynamic_cast< CostumeCustomMakeWnd *> ( g_GUIMgr.FindWnd( COSTUME_CUSTOM_MAKE_WND ) );
		if( pCostumeCustomMakeWnd )
			pCostumeCustomMakeWnd->StartCustomFileMake( dwUniqueIndex );
	}
	else
	{
		ItemCustomMakeWnd *pItemCustom = dynamic_cast< ItemCustomMakeWnd * > ( g_GUIMgr.FindWnd( ITEM_CUSTOM_MAKE_WND ) );
		if( pItemCustom )
		{
			pItemCustom->StartCustomFileMake( dwUniqueIndex );
		}
	}
}

void ioApplication::OnCustomItemSkinDelete( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );

	bool bResult;
	rkPacket >> bResult;

	int iSlotIndex;
	rkPacket >> iSlotIndex;
	
	byte eSkinType;
	rkPacket >> eSkinType;

	if( bResult )
	{
		ioUserExtraItem *pExtraItem = g_MyInfo.GetUserExtraItem();
		if( !pExtraItem )
			return;

		EXTRAITEMSLOT kSlot;
		if( !pExtraItem->GetExtraItem( iSlotIndex, kSlot ) )
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		}
		else
		{
			ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
			kPrinter[0].SetTextStyle( TS_NORMAL );
			kPrinter[0].SetBkColor( 0, 0, 0 );
			kPrinter[1].SetTextStyle( TS_NORMAL );
			kPrinter[1].SetBkColor( 0, 0, 0 );

			if( eSkinType == SDT_MALE )
			{
				kSlot.m_dwMaleCustom = 0;
				kPrinter[0].SetTextColor( TCT_DEFAULT_BLUE );
				kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(2) );
			}
			else if( eSkinType == SDT_FEMALE ) 
			{
				kSlot.m_dwFemaleCustom = 0;
				kPrinter[0].SetTextColor( TCT_DEFAULT_RED );
				kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(3) );
			}
			else if( eSkinType == SDT_POWERUP || SDT_ALL == eSkinType )
			{
				kSlot.m_dwMaleCustom	= 0;
				kSlot.m_dwFemaleCustom	= 0;
			}

			pExtraItem->SetExtraItem( kSlot );
			g_MyInfo.UpdateEquipExtraItem( iSlotIndex );


			kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
			kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(4) );

			kPrinter[1].SetTextColor( TCT_DEFAULT_DARKGRAY );
			kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(5) );

			if( eSkinType != SDT_POWERUP )
			{
				g_GUIMgr.SetPrevMsgListBox( NULL, MB_OK, NULL, kPrinter );

				if( g_GUIMgr.IsShow( MY_INVENTORY_WND ) )
				{
					MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(g_GUIMgr.FindWnd(MY_INVENTORY_WND));
					if( pInvenWnd && pInvenWnd->IsShow() )
					{
						pInvenWnd->UpdateTab( false, false );
					}
				}
				else if( g_GUIMgr.IsShow( ITEM_CUSTOM_SELECT_WND ) )
				{
					ItemCustomSelectWnd *pSelectWnd = dynamic_cast<ItemCustomSelectWnd*>(g_GUIMgr.FindWnd( ITEM_CUSTOM_SELECT_WND ));
					if( pSelectWnd && pSelectWnd->IsShow() )
					{
						pSelectWnd->Update();
					}
				}
				else if( g_GUIMgr.IsShow( COSTUME_CUSTOM_SELECT_WND ) )
				{
					ItemCustomSelectWnd *pSelectWnd = dynamic_cast<ItemCustomSelectWnd*>(g_GUIMgr.FindWnd( COSTUME_CUSTOM_SELECT_WND ));
					if( pSelectWnd && pSelectWnd->IsShow() )
					{
						pSelectWnd->Update();
					}
				}
			}
		}
	}
	else
	{

	}
}

// �ڽ�Ƭ ���� (�ڽ�Ƭ ��Ų)
void ioApplication::OnCustomCostumeSkinUniqueIndex( SP2Packet &rkPacket )
{
	DWORD dwUniqueIndex;
	rkPacket >> dwUniqueIndex;

	CostumeCustomMakeWnd *pCostumeCustom = dynamic_cast<CostumeCustomMakeWnd*>( g_GUIMgr.FindWnd( COSTUME_CUSTOM_MAKE_WND ) );
	if( pCostumeCustom )
		pCostumeCustom->StartCustomFileMake( dwUniqueIndex );
}

// �ڽ�Ƭ ���� (�ڽ�Ƭ ��Ų)
void ioApplication::OnCustomCostumeSkinDelete( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );
	BOOL bResult	= false;
	INT iSlotIndex	= 0;
	BYTE eSkinType	= 0;

	rkPacket >> bResult;
	rkPacket >> iSlotIndex;
	rkPacket >> eSkinType;

	if( bResult )
	{
		ioUserCostume *pCostume = g_MyInfo.GetUserCostume();
		if( !pCostume )
			return;

		CostumeSlot kSlot;
		if( !pCostume->GetCostumeSlot( iSlotIndex, kSlot ) )
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		else
		{
			ioComplexStringPrinter kPrinter[MAX_MSG_LIST_SIZE];
			kPrinter[0].SetTextStyle( TS_NORMAL );
			kPrinter[0].SetBkColor( 0, 0, 0 );
			kPrinter[1].SetTextStyle( TS_NORMAL );
			kPrinter[1].SetBkColor( 0, 0, 0 );

			if( eSkinType == SDT_MALE )
			{
				kSlot.m_dwMaleCustom = 0;
				kPrinter[0].SetTextColor( TCT_DEFAULT_BLUE );
				kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(2) );
			}
			else if( eSkinType == SDT_FEMALE ) 
			{
				kSlot.m_dwFemaleCustom = 0;
				kPrinter[0].SetTextColor( TCT_DEFAULT_RED );
				kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(3) );
			}
			else if( eSkinType == SDT_POWERUP || SDT_ALL == eSkinType )
			{
				kSlot.m_dwMaleCustom	= 0;
				kSlot.m_dwFemaleCustom	= 0;
			}

			pCostume->SetCostumeSlot( kSlot );
			g_MyInfo.UpdateEquipCostume( iSlotIndex );
			
			kPrinter[0].SetTextColor( TCT_DEFAULT_DARKGRAY );
			kPrinter[0].AddTextPiece( FONT_SIZE_13, STR(4) );

			kPrinter[1].SetTextColor( TCT_DEFAULT_DARKGRAY );
			kPrinter[1].AddTextPiece( FONT_SIZE_13, STR(5) );

			if( eSkinType != SDT_POWERUP )
			{
				g_GUIMgr.SetPrevMsgListBox( NULL, MB_OK, NULL, kPrinter );

				if( g_GUIMgr.IsShow( MY_INVENTORY_WND ) )
				{
					MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(g_GUIMgr.FindWnd(MY_INVENTORY_WND));
					if( pInvenWnd && pInvenWnd->IsShow() )
						pInvenWnd->UpdateTab( false, false );
				}
				else if( g_GUIMgr.IsShow( COSTUME_CUSTOM_SELECT_WND ) )
				{
					CostumeCustomSelectWnd *pSelectWnd = dynamic_cast<CostumeCustomSelectWnd*>(g_GUIMgr.FindWnd( COSTUME_CUSTOM_SELECT_WND ));
					if( pSelectWnd && pSelectWnd->IsShow() )
						pSelectWnd->Update();
				}
			}
		}
	}
	else
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6) ); //�ڽ�Ƭ������ ���� �߽��ϴ�.
		LOG.PrintTimeAndLog( 0 , "%s - Fail to delete costume skin, idx %d ", __FUNCTION__, iSlotIndex );
	}
}

void ioApplication::OnGetMileage( SP2Packet &rkPacket )
{
	int iType = 0;
	rkPacket >> iType;
	if( iType != GET_MILEAGE_SUCCESS )
	{
		g_MyInfo.SetMileage( 0 );
		return;
	}

	int iMileage = 0;
	rkPacket >> iMileage;
	g_MyInfo.SetMileage( iMileage );
}

void ioApplication::OnAddMileage( SP2Packet &rkPacket )
{
	int iType = 0;
	rkPacket >> iType;
	if( iType != ADD_MILEAGE_SUCCESS )
	{
		bool bError = false;
		ioHashString sError;
		rkPacket >> bError;
		if( bError )
			rkPacket >> sError;

		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1), sError.c_str() );
		// ������ 0�� �������� ���� ( �Ϻ� ��û )
		//g_MyInfo.SetMileage( 0 );
		return;
	}

	int iMileage = 0;
	int iAddMileage = 0;
	rkPacket >> iMileage;
	rkPacket >> iAddMileage;
	g_MyInfo.SetMileage( iMileage );

	if( iAddMileage <= 0 )
		return;

	MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(g_GUIMgr.FindWnd(MY_INVENTORY_WND));
	if( !pInvenWnd )
		return;
	if( !pInvenWnd->IsShow() )
		return;
	
	ItemRecvSellInfoWnd *pSellWnd = dynamic_cast<ItemRecvSellInfoWnd*>(pInvenWnd->FindChildWnd( MyInventoryWnd::ID_ITEM_RECVSELL_INFO_WND ) );
	if( !pSellWnd )
		return;
	if( !pSellWnd->IsShow() )
		return;
	
	char szConvertNum[MAX_PATH] = "";
	Help::ConvertNumToStrComma( iAddMileage, szConvertNum, sizeof( szConvertNum ) );
	char szTemp[MAX_PATH]="";
	SafeSprintf( szTemp, sizeof( szTemp ), STR(2) , szConvertNum );
	ioHashString sDescMileage = szTemp;
	pSellWnd->SetDescMileage( sDescMileage );
}


void ioApplication::OnGetCash( SP2Packet &rkPacket )
{	
	int  iType = 0;
	bool bMouse= false;
	rkPacket >> iType >> bMouse;

	if( bMouse )
		SetMouseBusy( false );

	if( iType == GET_CASH_SUCCESS )
	{
		ioChannelingNodeParent *pNode = g_ChannelingMgr.GetNode( g_MyInfo.GetChannelingType() );
		if( !pNode )
		{
			LOG.PrintTimeAndLog( 0 , "Get Cash pNode == NULL." );
			g_MyInfo.SetCash( 0 );
			g_MyInfo.SetPurchasedCash( 0 );
			g_MyInfo.SetChannelingCash( 0 );
			return;
		}

		pNode->OnRecieveGetCash( rkPacket );
		//LOG.PrintTimeAndLog( 0 , "GET_CASH_SUCCESS" );
	}
	else if( iType == GET_CASH_BILLING_WAIT )
	{
		if( bMouse )
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		g_MyInfo.SetCash( 0 );
		g_MyInfo.SetPurchasedCash( 0 );
		g_MyInfo.SetChannelingCash( 0 );
	}
	else if( iType == GET_CASH_BILLING_DISCONNECT )
	{
		if( bMouse )
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		g_MyInfo.SetCash( 0 );
		g_MyInfo.SetPurchasedCash( 0 );
		g_MyInfo.SetChannelingCash( 0 );
	}
	else if( iType == GET_CASH_BILLING_FAIL )
	{
		bool bBillingError    = false;
		ioHashString  sBillingError;
		rkPacket >> bBillingError;
		if( bBillingError )
			rkPacket >> sBillingError;

		if( bMouse )
		{
			ioHashString sErrorMent = STR(3);
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
					StringCbPrintf( szErrorFullMent, sizeof( szErrorFullMent ), "%s",  sError.c_str() ); //Except Extracting Hangeul
				else
					StringCbPrintf( szErrorFullMent, sizeof( szErrorFullMent ), "%s#%s%s", sErrorMent.c_str(), sError.c_str(), sBillingError.c_str() ); //Except Extracting Hangeul

				g_GUIMgr.SetMsgBox( MB_OK, NULL, szErrorFullMent );
			}
		}
		g_MyInfo.SetCash( 0 );
		g_MyInfo.SetPurchasedCash( 0 );
		g_MyInfo.SetChannelingCash( 0 );
	}
}

void ioApplication::OnWholeServerChat( SP2Packet &rkPacket )
{
	ioHashString szID, szChat;
	rkPacket >> szID >> szChat;

	g_MannerTrialChatMgr.Add( ioMannerTrialChatManager::TT_WHOLE_SERVER_CHAT, szID.c_str(), szChat.c_str() ); // ���������� �Ұ�

	char szChatBuf[MAX_PATH] = "";
	StringCbCopy( szChatBuf, MAX_PATH, g_SlangUtil.ConvertStringWithPreLine( szChat.c_str() ) );
	szChat = szChatBuf;

	if( Setting::WholeChatOff() )	// ��ħ ���� ����
		return;

//	ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
//	if( pPlayStage )
//	{
//		pPlayStage->SetChatIcon( szID, szChat, true, true );
//	}

	SetWholeServerChatMsg( szID.c_str(), szChat.c_str(), true );
}

void ioApplication::OnWholeServerRainbowChat( SP2Packet &rkPacket )
{
	ioHashString szID, szChat;
	rkPacket >> szID >> szChat;

	g_MannerTrialChatMgr.Add( ioMannerTrialChatManager::TT_WHOLE_SERVER_RAINBOW_CHAT, szID.c_str(), szChat.c_str() ); // ���������� �Ұ�

	if( Setting::WholeChatOff() )	// ��ħ ���� ����
		return;

	char szChatBuf[MAX_PATH] = "";
	StringCbCopy( szChatBuf, MAX_PATH, g_SlangUtil.ConvertStringWithPreLine( szChat.c_str() ) );
	szChat = szChatBuf;

	SetWholeServerRainbowChatMsg( szID.c_str(), szChat.c_str(), true );
}

void ioApplication::OnStartEtcItemTime( SP2Packet &rkPacket )
{
	ioUserEtcItem *pUserEtcItem = g_MyInfo.GetUserEtcItem();
	if( !pUserEtcItem )
		return;

	pUserEtcItem->ApplyTimeData( rkPacket );
}

void ioApplication::OnUpdateEtcItemTime( SP2Packet &rkPacket )
{
	ioUserEtcItem *pUserEtcItem = g_MyInfo.GetUserEtcItem();
	if( !pUserEtcItem )
		return;

	pUserEtcItem->ApplyTimeData( rkPacket, true );
	pUserEtcItem->DeleteEtcItemZeroTime();
}

void ioApplication::OnDeleteEtcItemDate( SP2Packet &rkPacket )
{
	ioUserEtcItem *pUserEtcItem = g_MyInfo.GetUserEtcItem();
	if( !pUserEtcItem )
	{
		LOG.PrintTimeAndLog( 0, "DeleteEtcItemDate Error : pUserEtcItem == NULL" );
		return;
	}

	int iSize = 0;
	rkPacket >> iSize;
	if( iSize <= 0 )
	{
		LOG.PrintTimeAndLog( 0, "DeleteEtcItemDate Error : Size is %d", iSize );
		return;
	}

	for (int i = 0; i < iSize ; i++)
	{
		int iType = 0;
		rkPacket >> iType;
		pUserEtcItem->DeleteEtcItem( iType );
	}
}

void ioApplication::OnHoleSendComplete( SP2Packet &rkPacket )
{
	if( !m_pNetworkTable )
	{
		LOG.PrintTimeAndLog( 0, "OnHoleSendComplete NetworkTable NULL!!" );
		return;
	}

	int iPort;
	ioHashString szUserID, szUserIP;	
	rkPacket >> szUserID >> szUserIP >> iPort;

	m_pNetworkTable->HoleSendComplete( szUserID, szUserIP, iPort );
}

void ioApplication::OnUDPRecvTimeOut( SP2Packet &rkPacket )
{
	if( !m_pNetworkTable )
	{
		LOG.PrintTimeAndLog( 0, "OnUDPRecvTimeOut NetworkTable NULL!!" );
		return;
	}

	ioHashString szUserID;	
	rkPacket >> szUserID;

	m_pNetworkTable->SetUDPRecvTimeOut( szUserID );
}

void ioApplication::OnUDPRecvTimeOutReCheck( SP2Packet &rkPacket )
{
	if( !m_pNetworkTable )
	{
		LOG.PrintTimeAndLog( 0, "OnUDPRecvTimeOutReCheck NetworkTable NULL!!" );
		return;
	}

	ioHashString szUserID;	
	rkPacket >> szUserID;

	m_pNetworkTable->ReCheckUDPRecvTimeOut( szUserID );
}

void ioApplication::OnCampDataSync( SP2Packet &rkPacket )
{
	g_CampMgr.FillCampDataSync( rkPacket );
}

void ioApplication::OnCampChangePos( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );
	bool bShowResultUI;
	int iCampPosition;
	rkPacket >> iCampPosition >> bShowResultUI;
	
	// ������ Ż��ǵ� ���Եǵ� ��������Ʈ�� 0���� �ʱ�ȭ
	if( iCampPosition != g_MyInfo.GetUserCampPos() )
		g_MyInfo.SetLadderPoint( 0 );

	g_MyInfo.SetUserCampPos( iCampPosition );

	if( bShowResultUI )
	{
		bool bShowResult = true;
		if( g_TournamentMgr.IsRegularTeamCreateUIShow() )
		{
			TournamentTeamCreateWnd *pCreateWnd = dynamic_cast<TournamentTeamCreateWnd*>( g_GUIMgr.FindWnd( TOURNAMENT_TEAM_CREATE ) );
			if( pCreateWnd )
			{
				if( g_TournamentMgr.GetRegularState() == TournamentNode::STATE_TEAM_APP )
				{
					bShowResult = false;
					pCreateWnd->SetTournamentIndex( g_TournamentMgr.GetRegularIndex() );
				}
			}
		}

		if( bShowResult )
		{
			g_GUIMgr.ShowWnd( CAMP_ENTER_RESULT_WND );
		}
	}

#if defined( USE_GA )
	if( iCampPosition != 0 )
	{
		char chLabel[32] = {0,};

		if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
			sprintf_e( chLabel, "%d", iCampPosition );
		else
			SafeSprintf( chLabel, sizeof(chLabel), "%1", iCampPosition );

		// CHAR_CAMP_SELECT
		g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
			, "Character"
			, "Select"
			, chLabel
			, 1
			, "%2FCHAR%2FCAMP_SELECT" );
	}	
#endif

}

void ioApplication::OnRefreshLadderPointNRecord( SP2Packet &rkPacket )
{
	int iLadderPoint, iAccrueHeroExpert, iHeroExpert, iCampPosition;
	rkPacket >> iLadderPoint >> iAccrueHeroExpert >> iHeroExpert >> iCampPosition;
	g_MyInfo.SetLadderPoint( iLadderPoint );
	g_MyInfo.SetAccrueHeroExpert( iAccrueHeroExpert );
	g_MyInfo.SetHeroExpert( iHeroExpert );
	g_MyInfo.SetUserCampPos( iCampPosition );
	g_MyInfo.ApplyUserHeroSeasonRecord( rkPacket );
}

void ioApplication::OnCampSeasonBonus( SP2Packet &rkPacket )
{
	__int64 iMyMoney;
	rkPacket >> iMyMoney;
	g_MyInfo.SetMoney( iMyMoney );

	int iBonusPeso, iBlueCampPoint, iRedCampPoint, iMyCampType, iMyCampPoint, iMyCampRank;
	rkPacket >> iBonusPeso >> iBlueCampPoint >> iRedCampPoint >> iMyCampType >> iMyCampPoint >> iMyCampRank;

	CampSeasonBonus *pSeasonBonus = dynamic_cast<CampSeasonBonus*>( g_GUIMgr.FindWnd( CAMP_SEASON_BONUS_WND ) );
	if( pSeasonBonus )
	{
		pSeasonBonus->SetSeasonBonus( iBonusPeso, iBlueCampPoint, iRedCampPoint, iMyCampType, iMyCampPoint, iMyCampRank );
	}
}

void ioApplication::OnGuildNameChange( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );
	int iResult;
	DWORD dwUserIndex, dwGuildIndex;
	ioHashString szNewGuildName;
	rkPacket >> iResult >> dwUserIndex >> dwGuildIndex >> szNewGuildName;

	MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(g_GUIMgr.FindWnd(MY_INVENTORY_WND));

	// ����
	switch( iResult )
	{
	case GUILD_NAME_CHANGE_ALREADY:
		if( dwUserIndex == g_MyInfo.GetUserIndex() )
		{
			if( pInvenWnd && pInvenWnd->IsShow() )
			{
				pInvenWnd->OnErrorEdit( STR(1) );
			}
		}
		return;
	case GUILD_NAME_CHANGE_NOT_GUILD:
		if( dwUserIndex == g_MyInfo.GetUserIndex() )
		{
			if( pInvenWnd && pInvenWnd->IsShow() )
			{
				pInvenWnd->UpdateTab( false, true );
			}

			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		}
		return;
	case GUILD_NAME_CHANGE_NOT_MASTER:
		if( dwUserIndex == g_MyInfo.GetUserIndex() )
		{
			if( pInvenWnd && pInvenWnd->IsShow() )
			{
				pInvenWnd->UpdateTab( false, true );
			}

			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		}
		return;
	case GUILD_NAME_CHANGE_FAILED:
		if( dwUserIndex == g_MyInfo.GetUserIndex() )
		{
			if( pInvenWnd && pInvenWnd->IsShow() )
			{
				pInvenWnd->OnErrorEdit( STR(4) );
			}
		}
		return;
	}

	// ����
	if( iResult == GUILD_NAME_CHANGE_OK )
	{
		ioHashString szPrevGuildName;
		ioGuildData *pGuildData = g_GuildInfo.GetGuildData( dwGuildIndex );
		if( pGuildData )
		{
			szPrevGuildName = pGuildData->GetGuildName();
			pGuildData->SetGuildName( szNewGuildName );
		}

		if( dwUserIndex == g_MyInfo.GetUserIndex() )
		{
			if( pInvenWnd && pInvenWnd->IsShow() )
			{
				pInvenWnd->UpdateTab( false, false );
				pInvenWnd->OnChangeGuildNameResultWnd( szPrevGuildName, szNewGuildName );
			}

			/*
			GuildNameChangeWnd *pGuildName = dynamic_cast<GuildNameChangeWnd*>(g_GUIMgr.FindWnd( GUILD_NAME_CHANGE_WND ));
			if( pGuildName )
			{
				pGuildName->SetInfoAndShow( szPrevGuildName, szNewGuildName );
			}
			*/
		}
	}
}

void ioApplication::OnCharChangePeriod( SP2Packet &rkPacket )
{
	SetMouseBusy( false );

	int iResult;
	rkPacket >> iResult;

	switch(iResult)
	{
	case CHAR_CHANGE_PERIOD_CREATING_CHAR:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return;
	case CHAR_CHANGE_PERIOD_WANT_OF_MONEY:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return;
	case CHAR_CHANGE_PERIOD_EXCEPTION:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		return;
	case CHAR_CHANGE_PERIOD_EXERCISE_CHAR:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		return;
	case CHAR_CHANGE_PERIOD_MORTMAIN_CHAR:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
		return;
	case CHAR_CHANGE_PERIOD_NEED_LEVEL:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6) );
		return;
	case CHAR_CHANGE_PERIOD_BILLING_WAIT:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(7) );
		return;
	case CHAR_CHANGE_PERIOD_BILLING_DISCONNECT:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(8) );
		return;
	case CHAR_CHANGE_PERIOD_BILLING_FAIL:
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
					StringCbPrintf( szErrorFullMent, sizeof( szErrorFullMent ), "%s",  sError.c_str() ); //Except Extracting Hangeul
				else
					StringCbPrintf( szErrorFullMent, sizeof( szErrorFullMent ), "%s#%s%s", sErrorMent.c_str(), sError.c_str(), sBillingError.c_str() ); //Except Extracting Hangeul

				g_GUIMgr.SetMsgBox( MB_OK, NULL, szErrorFullMent );
			}
		}
		return;
	case CHAR_CHANGE_PERIOD_BILLING_WANT_OF_CASH:
		g_GUIMgr.ShowWnd( BILLING_WANT_WND );
		return;
	case CHAR_CHANGE_PERIOD_BILLING_EXCESS_BUY:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(10) );
		return;
	case CHAR_CHANGE_PERIOD_FULL_MORTMAIN_CHAR:
		{
			int iHaveMortmainChar = g_LevelMgr.GetNumHaveMortmainChar( g_MyInfo.GetGradeLevel() );
			int iNextGradeLv = g_LevelMgr.GetNextNumHaveMortmainChar( g_MyInfo.GetGradeLevel() );
			int iNextHaveNum = g_LevelMgr.GetNumHaveMortmainChar( iNextGradeLv );
			char szGradeName[MAX_PATH];
			g_LevelMgr.GetGradeName( iNextGradeLv, szGradeName, sizeof( szGradeName ), false , true );
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(11), iHaveMortmainChar, szGradeName ,iNextHaveNum );
		}
		return;
	case CHAR_CHANGE_PERIOD_WANT_OF_MORTMAIN_ETCITEM:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(12) );
		return;
	}

	if( iResult != CHAR_CHANGE_PERIOD_OK )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(13), iResult );
		return;
	}

	__int64 iPrevMoney = g_MyInfo.GetMoney();
	__int64 iMoney;
	int  iCash;
	int  iChannelingCash;
	int  iCharArray;
	int  iResellPeso;
	int  iBonusPeso;
	bool bCash;
	rkPacket >> iMoney >> iCash >> iChannelingCash >> iCharArray >> iResellPeso >> iBonusPeso >> bCash;

#if defined( USE_GA )
	bool bGACash	= false;
	bool bGAMoney	= false;
	int iPesoPrice	= g_MyInfo.GetMoney() - iMoney;
	int iCashPrice	= g_MyInfo.GetCash() - iCash;
	if( iCashPrice > 0 && iCashPrice < 1000000 )
		bGACash		= true;
	if( iPesoPrice > 0 )
		bGAMoney	= true;
#endif

	g_MyInfo.SetCash( iCash ); 
	g_MyInfo.SetChannelingCash( iChannelingCash );
	g_MyInfo.SetMoney( iMoney );
	g_MyInfo.SetCharChangePeriod( iCharArray );

	if ( g_RSoldierInfoManager.IsSoldierSet( g_MyInfo.GetCharacter( iCharArray ) ) )
		g_MyInfo.CalcSoldierSetCnt();

	SoldierSelectWnd *pSelectWnd = dynamic_cast<SoldierSelectWnd*>(g_GUIMgr.FindWnd( SOLDIER_SELECT_WND ));
	if( pSelectWnd && pSelectWnd->IsShow() )
		pSelectWnd->SettingSoldierBtn();

	UISoldierSelectWnd *pUISoldierWnd = dynamic_cast<UISoldierSelectWnd*>( g_GUIMgr.FindWnd( UI_SOLDIERSELECT_WND ) );
	if( pUISoldierWnd && pUISoldierWnd->IsShow() )
		pUISoldierWnd->SettingSoldierBtn();

	MiniSoldierSelectWnd *pMiniSoldierWnd = dynamic_cast<MiniSoldierSelectWnd*>( g_GUIMgr.FindWnd( MINI_SOLDIER_SELECT_WND ));
	if( pMiniSoldierWnd && pMiniSoldierWnd->IsShow() )
		pMiniSoldierWnd->SettingMiniSoldierBtn();

	if( iPrevMoney > g_MyInfo.GetMoney() && !IsMeDeveloper() && !IsMeGameMaster() )
	{
		int iBuyPeso = (int)( iPrevMoney - g_MyInfo.GetMoney() ) - iBonusPeso;
		g_AwardMgr.AddShopping( g_MyInfo.GetPublicID(), iBuyPeso );
		SP2Packet kPacket( CUPK_SHOPPING );
		kPacket << g_MyInfo.GetPublicID() << iBuyPeso;
		SendToAllPlayingUser( kPacket );
	}		
	g_MyInfo.SetRoomOutTimer( 0 );

	if( iResellPeso > 0 && iBonusPeso > 0 )
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(14), iResellPeso + iBonusPeso );
	else if( iResellPeso > 0 )
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(15), iResellPeso );
	//else if( iBonusPeso > 0 )
	//	g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(16), iBonusPeso );

	if( !bCash )
	{
		ioEtcItemBuyMortmainChar *pEtcItem = dynamic_cast<ioEtcItemBuyMortmainChar*> ( g_EtcItemMgr.FindEtcItem( ioEtcItem::EIT_ETC_BUY_MORTMAIN_CHAR ));
		if( !pEtcItem )
			LOG.PrintTimeAndLog( 0, "%s pEtcItem == NULL.", __FUNCTION__ );
		else
			pEtcItem->SetUsed( g_MyInfo.GetUserEtcItem() );
	}

	MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(g_GUIMgr.FindWnd(MY_INVENTORY_WND));
	if( pInvenWnd && pInvenWnd->IsShow() )
	{
		pInvenWnd->UpdateTab( false, true );
	}
	
	CloseGuildMarkPage();

#if defined( USE_GA )
	if( bGACash )
	{
		iCashPrice = g_ClassPrice.GetMortmainCharCash( g_MyInfo.GetClassType( iCharArray ) );

		// GOLD_USE_HERO
		g_HttpMng.GA_ItemHitTracking( g_MyInfo.GetUserIndex() 
			, "Hero"
			, iCashPrice
			, 1
			, g_MyInfo.GetClassType( iCharArray )
			, "Gold"
			, "%2FGOLD%2FUSE%2FHERO"
			, true );
	}
	if( bGAMoney )
	{
		// PESO_USE_HERO
		g_HttpMng.GA_ItemHitTracking( g_MyInfo.GetUserIndex() 
			, "Hero"
			, iPesoPrice
			, 1
			, g_MyInfo.GetClassType( iCharArray )
			, "Peso"
			, "%2FPESO%2FUSE%2FHERO" );
	}			
#endif

}

void ioApplication::OnChangeUserName( SP2Packet &rkPacket )
{
	ioHashString szName;
	ioHashString szNewName;
	rkPacket >> szName >> szNewName;

	g_GuildInfo.ChangeMyGuildUserID( szName, szNewName );
	g_FriendManager.ChangeName( szName, szNewName );
	g_MemoManager.ChangeName( szName, szNewName );
	g_UserInfoMgr.ChangeName( szName, szNewName );

	LOG.PrintTimeAndLog( 0, "Changed Friend Name %s -> %s", szName.c_str(), szNewName.c_str() );
}

void ioApplication::OnBlockType( SP2Packet &rkPacket )
{
	g_MyInfo.ApplyBlockType( rkPacket );

	if( g_MyInfo.GetBlockType() == BKT_WARNNING )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
	}
	else if( g_MyInfo.GetBlockType() == BKT_LIMITATION )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
	}
	else if( g_MyInfo.GetBlockType() == BKT_BLOCK )
	{
		bool bNotifyGUI = false;
		if( ioSP2GUIManager::GetSingletonPtr() )
		{
			DisconnectWnd *pDisWnd = dynamic_cast<DisconnectWnd*> ( g_GUIMgr.FindWnd(DISCONNECT_WND) );
			if( pDisWnd )
			{
				pDisWnd->SetType( DisconnectWnd::TYPE_BLOCK_EXIT );
				pDisWnd->ShowWnd();
				bNotifyGUI = true;
			}
		}

		if( !bNotifyGUI )
		{
			MBox( NULL, STR(3), STR(4) );
			LOG.PrintTimeAndLog( 0, "ExitProgram - 6" );

#if defined( USE_GA )
			if( GetGAStart() == true )
			{
				char chLabel[32] = {0,};

				if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
					sprintf_e( chLabel, "%d", 6 );
				else
					SafeSprintf( chLabel, sizeof(chLabel), "%1", 6 );

				// GAME_END_ERR_POS
				g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
					, "Game"
					, "Error"
					, chLabel
					, 1
					, "%2FGAME%2FOVER%2FERR"
					, 1 );

				g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FGAME%2FOVER%2FERR", 6, chLabel );
			}			
#endif

			g_App.SetExitProgram();
		}

		LOG.PrintTimeAndLog( 0, "Block Logout" );
	}
	else if( g_MyInfo.GetBlockType() == BKT_NORMAL )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
	}
	else
		LOG.PrintTimeAndLog( 0, "Block Type Error (%d)", g_MyInfo.GetBlockType() );

}

void ioApplication::OnFirstChangeID( SP2Packet &rkPacket )
{
	SetMouseBusy( false );

	int iResult = 0;
	rkPacket >> iResult;
	
	if( iResult == FIRST_CHANGE_ID_EXCEPTION )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
	}
	else if( iResult == FIRST_CHANGE_ID_NOW_WORKING )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
	}
	else if(  iResult == FIRST_CHANGE_ID_WRONG_ID )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
	}
	else if(  iResult == FIRST_CHANGE_ID_EXIST_ID )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
	}
	else if(  iResult == FIRST_CHANGE_ID_SUCCESS )
	{
		ioHashString szNewPublicID;
		rkPacket >> szNewPublicID;

		ioWnd *pWnd = g_GUIMgr.FindWnd( FIRST_IDCHANGE_WND );
		if( pWnd )
			pWnd->HideWnd();

		char szTempPublicID[ID_NUM_PLUS_ONE]="";
 		StringCbPrintf( szTempPublicID, sizeof( szTempPublicID ), "%s#", g_MyInfo.GetPrivateID().c_str() );
		ioHashString sTempPulicID = szTempPublicID;
		
		ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
		if( pPlayStage )
		{
			ioBaseChar *pChar = pPlayStage->GetBaseChar( sTempPulicID );
			if( pChar == NULL )
			{
				StringCbPrintf( szTempPublicID, sizeof( szTempPublicID ), "_%s#", g_MyInfo.GetPrivateID().c_str() );
				sTempPulicID = szTempPublicID;
			}
		}
 
 		g_MyInfo.SetPublicID( szNewPublicID );
 		g_GuildInfo.ChangeMyGuildUserID( sTempPulicID, szNewPublicID );
		g_FriendManager.ChangeName( sTempPulicID, szNewPublicID );
 		g_MemoManager.ChangeName( sTempPulicID, szNewPublicID );
 		g_UserInfoMgr.ChangeName( sTempPulicID, szNewPublicID );
 
  		if( pPlayStage )
		{
  			pPlayStage->ChangeBaseCharName( sTempPulicID, szNewPublicID );
			pPlayStage->AddAllNPCSyncFirstUser(); // PublicID�� ����ǹǷ� NPC Sync Name �缳��
			ioHeadquartersMode *pHeadquartersMode = ToHeadquartersMode( pPlayStage->GetPlayMode() );
			if( pHeadquartersMode )
				pHeadquartersMode->ChangeRecordCharName( sTempPulicID, szNewPublicID );
		}

		if( g_MyInfo.GetUserState() == 0 )
		{
			// ���� �뺴 ����.
			g_MyInfo.SetFirstSolderBonus();
		}

		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );

		// Public ID ���� �� �ɽ� ���� ��ȸ �߰�	JCLEE 150102
		SP2Packet kPacket( CTPK_GET_CASH );
		TCPNetwork::SendToServer( kPacket );
		TCPNetwork::MouseBusy( true );		

#if defined( USE_GA )
		// ACCOUNT_CREATE
		g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FACCOUNT%2FCREATE", 5 );
#endif
	}
	else
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6) );
	}
	
}

void ioApplication::OnCheckSalePrice( SP2Packet &rkPacket )
{
	g_SaleMgr.CheckAlive( rkPacket );
}

void ioApplication::OnGashaponList( SP2Packet &rkPacket )
{
	IntVec vPresentTypeList;
	IntVec vPresentValue1List;
	IntVec vPresentValue2List;
	IntVec vPresentValue3List;
	IntVec vPresentValue4List;

	int iMaxList = 0;
	rkPacket >> iMaxList;

	vPresentTypeList.reserve( iMaxList );
	vPresentValue1List.reserve( iMaxList );
	vPresentValue2List.reserve( iMaxList );

	for (int i = 0; i < iMaxList ; i++)
	{
		short iPresentType = 0;
		int   iPresentValue1 = 0;
		int   iPresentValue2 = 0;
		int   iPresentValue3 = 0;
		int   iPresentValue4 = 0;
		rkPacket >> iPresentType;
		rkPacket >> iPresentValue1;
		rkPacket >> iPresentValue2;
		rkPacket >> iPresentValue3;
		rkPacket >> iPresentValue4;

		vPresentTypeList.push_back( iPresentType );
		vPresentValue1List.push_back( iPresentValue1 );
		vPresentValue2List.push_back( iPresentValue2 );
		vPresentValue3List.push_back( iPresentValue3 );
		vPresentValue4List.push_back( iPresentValue4 );
	}

	GashaponPresentListWnd::SetPresentList( vPresentTypeList, vPresentValue1List, vPresentValue2List, vPresentValue3List, vPresentValue4List );
}

void ioApplication::OnControlKeys( SP2Packet &rkPacket )
{
	ControlKeys kControlKeys;
	rkPacket >> kControlKeys;
	g_KeyManager.SetServerKeys( kControlKeys );

#ifndef SHIPPING
	LOG.PrintTimeAndLog( 0, "%s : %s : %d", __FUNCTION__, kControlKeys.m_szControlKeys, strlen( kControlKeys.m_szControlKeys ) );
#endif
}

void ioApplication::OnQuestData( SP2Packet &rkPacket )
{
	g_MyInfo.ApplyQuestData( rkPacket );
}

void ioApplication::OnQuestCompleteData( SP2Packet &rkPacket )
{
	g_MyInfo.ApplyQuestCompleteData( rkPacket );
}

void ioApplication::OnQuestOccur( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );

	int iResult;
	rkPacket >> iResult;
	switch( iResult )
	{
	case QUEST_OCCUR_OK:
		{
			ioQuest *pQuest = g_MyInfo.GetQuest();
			if( pQuest )
			{
				pQuest->ApplyQuestOccurData( rkPacket );
			}
		}
		break;
	case QUEST_OCCUR_ALREADY_COMPLETE:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		break;
	case QUEST_OCCUR_ALREADY_PROGRESS:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		break;
	case QUEST_OCCUR_UNKNOWN_TYPE:
	case QUEST_OCCUR_CHANNELING_FAIL:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		break;
	}
}

void ioApplication::OnQuestAttain( SP2Packet &rkPacket )
{
	int iResult;
	rkPacket >> iResult;
	switch( iResult )
	{
	case QUEST_ATTAIN_OK:
	case QUEST_ATTAIN_ALREADY_COMPLETE:
		g_MyInfo.ApplyQuestData( rkPacket );
		break;
	case QUEST_ATTAIN_UNKNOWN_TYPE:
		break;
	}
}

void ioApplication::OnQuestReward( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );

	int iResult;
	rkPacket >> iResult;
	switch( iResult )
	{
	case QUEST_REWARD_OK:
		{
			ioQuest *pQuest = g_MyInfo.GetQuest();
			if( pQuest )
			{
				pQuest->ApplyQuestRewardData( rkPacket );
				// ���� ó��
				__int64 iMyMoney;
				int iGradeLevel, iGradeExp;
				int iPrevGradeLevel = g_MyInfo.GetGradeLevel();
				rkPacket >> iMyMoney >> iGradeLevel >> iGradeExp;

#if defined( USE_GA )
//				int iPesoReward	= iMyMoney - g_MyInfo.GetMoney();
#endif

				g_MyInfo.SetMoney( iMyMoney );
				g_MyInfo.SetGrade( iGradeLevel, iGradeExp );

				if( iPrevGradeLevel != g_MyInfo.GetGradeLevel() )
				{
					ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
					if( pPlayStage )
					{
						ioBaseChar *pOwnerChar = pPlayStage->GetBaseChar( g_MyInfo.GetPublicID() );
						if( pOwnerChar )
							pOwnerChar->SetLevel( iGradeLevel );
					}
	                //g_GUIMgr.ShowWnd( GRADE_UP_WND );
				}

#if defined( USE_GA )
// 				// QUEST_REWARD_RECEIVE
// 				g_HttpMng.GA_ItemHitTracking( g_MyInfo.GetUserIndex() 
// 					, "Recv"
// 					, iPesoReward
// 					, 1
// 					, 0
// 					, "Quest"
// 					, "%2FQUEST%2FREWARD%2FRECEIVE" );
#endif

			}
		}
		break;
	case QUEST_REWARD_NOT_ATTAIN:
		g_MyInfo.ApplyQuestData( rkPacket );
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		break;
	case QUEST_REWARD_ALREADY_COMPLETE:
		g_MyInfo.ApplyQuestData( rkPacket );
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		break;
	case QUEST_REWARD_UNKNOWN_TYPE:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		break;
	}
}

void ioApplication::OnQuestRepeatReward( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );

	ioQuest *pQuest = g_MyInfo.GetQuest();
	if( pQuest )
	{
		DWORD dwMainIndex, dwSubIndex;
		rkPacket >> dwMainIndex >> dwSubIndex;

		// ��� ���� ���� ó��
		g_QuestMgr.DirectPresentAlarm( dwMainIndex, dwSubIndex, rkPacket );	
		
		// ���� ó��
		__int64 iMyMoney;
		int iGradeLevel, iGradeExp;
		int iPrevGradeLevel = g_MyInfo.GetGradeLevel();
		rkPacket >> iMyMoney >> iGradeLevel >> iGradeExp;
		g_MyInfo.SetMoney( iMyMoney );
		g_MyInfo.SetGrade( iGradeLevel, iGradeExp );

		if( iPrevGradeLevel != g_MyInfo.GetGradeLevel() )
		{
			ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
			if( pPlayStage )
			{
				ioBaseChar *pOwnerChar = pPlayStage->GetBaseChar( g_MyInfo.GetPublicID() );
				if( pOwnerChar )
					pOwnerChar->SetLevel( iGradeLevel );
			}
		}
	}	
}

void ioApplication::OnQuestDelete( SP2Packet &rkPacket )
{
	int iSize, iRemoveCount;
	iSize = iRemoveCount = 0;
	rkPacket >> iSize;
	for(int i = 0;i < iSize;i++)
	{
		DWORD dwMainIndex, dwSubIndex;
		rkPacket >> dwMainIndex >> dwSubIndex;

		ioQuest *pQuest = g_MyInfo.GetQuest();
		if( pQuest )
		{
			if( !pQuest->RemoveQuestData( dwMainIndex, dwSubIndex ) )
			{
				g_QuestMgr.AddQuestDelAndFailList( true, g_QuestMgr.GetQuest( dwMainIndex, dwSubIndex ) ); 
			}
			else
				pQuest->RemoveQuestCompleteData( dwMainIndex, dwSubIndex );
		}
	}
}

void ioApplication::OnQuestEventAlive( SP2Packet &rkPacket )
{
	int iSize;
	rkPacket >> iSize;
	for(int i = 0;i < iSize;i++)
	{
		bool  bAlive;
		DWORD dwMainIndex, dwSubIndex;
		rkPacket >> dwMainIndex >> dwSubIndex >> bAlive;
		
		QuestParent *pQuestParent = g_QuestMgr.GetQuest( dwMainIndex, dwSubIndex );
		if( pQuestParent )
			pQuestParent->SetAlive( bAlive );
	}
}

void ioApplication::OnQuestOneDayDelete( SP2Packet &rkPacket )
{
	//  �Ϸ�� ����Ʈ �� ���� ����Ʈ�� ����
	ioQuest *pQuest = g_MyInfo.GetQuest();
	if( pQuest )
	{
		pQuest->RemoveQuestOneDayAll();
	}
}

void ioApplication::OnQuestAbuseAlarm( SP2Packet &rkPacket )
{
	bool bLogout;
	DWORD dwMainIndex, dwSubIndex;
	rkPacket >> bLogout >> dwMainIndex >> dwSubIndex;
    
	DWORD dwAbuseGapTime, dwAbuseGapValue;
	rkPacket >> dwAbuseGapTime >> dwAbuseGapValue;

	if( bLogout )
	{
		DisconnectWnd *pWnd = dynamic_cast<DisconnectWnd*> ( g_GUIMgr.FindWnd( DISCONNECT_WND ) );
		if( pWnd && !pWnd->IsShow() )
		{
			pWnd->SetType( DisconnectWnd::TYPE_QUEST_ABUSE_EXIT );
			pWnd->SetQuestData( dwMainIndex, dwSubIndex );
			pWnd->ShowWnd();
		}
	}

	char szLogMessage[MAX_PATH];
	sprintf( szLogMessage, "Quest Abuse User : %s - (%d:%d) - Abuse(%d:%d) - Logout(%d)", g_MyInfo.GetPublicID().c_str(), dwMainIndex, dwSubIndex,
	  	     dwAbuseGapTime, dwAbuseGapValue, (int)bLogout );
	SP2Packet kLogPacket( LUPK_LOG );
	kLogPacket << "CheckError";
	kLogPacket << szLogMessage;
	kLogPacket << 319;  // ������ȣ
	kLogPacket << true; // write db
	P2PNetwork::SendLog( kLogPacket );
}

void ioApplication::OnUserHeroData( SP2Packet &rkPacket )
{
	g_MyInfo.SetUserHeroData( rkPacket );
}

void ioApplication::OnHeroTop100Data( SP2Packet &rkPacket )
{
	g_CampMgr.ApplyHeroTop100( rkPacket );
}

void ioApplication::OnMedalItemData( SP2Packet &rkPacket )
{
	ioUserMedalItem *pUserMedalItem = g_MyInfo.GetUserMedalItem();
	if( !pUserMedalItem )
		return;

	pUserMedalItem->ApplyMedalItemData( rkPacket );
}

void ioApplication::OnMedalItemDeleteByDate( SP2Packet &rkPacket )
{
	if( !g_MyInfo.DeleteMedalItemData( rkPacket ) )
	{
		LOG.PrintTimeAndLog( 0, "ioApplication::OnMedalItemDeleteByDate() - Delete Fail" );
		return;
	}
}

void ioApplication::OnExpandMedalSlotData( SP2Packet &rkPacket )
{
	g_ExpandMedalSlotMgr.ApplyExpandMedalSlot( rkPacket );
}

void ioApplication::OnExpandMedalSlotDeleteByDate( SP2Packet &rkPacket )
{
	g_ExpandMedalSlotMgr.DeleteExpandMedalSlot( rkPacket );
}

void ioApplication::OnEventShopGoodsList( SP2Packet &rkPacket )
{
	g_EventGoodsMgr.ApplyGoodsList( rkPacket );
}

void ioApplication::OnEventShopGoodsBuy( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );

	int iResult;
	rkPacket >> iResult;
	switch( iResult )
	{
	case EVENT_SHOP_GOODS_BUY_OK:       // ���� �Ϸ�
		{
			DWORD dwGoodsIndex, dwEtcItemType, iNeedCoin;
			rkPacket >> dwGoodsIndex >> dwEtcItemType >> iNeedCoin;
			g_EventGoodsMgr.BuyGoodsComplete( dwGoodsIndex, dwEtcItemType, iNeedCoin );

#if defined( USE_GA )
			if( dwEtcItemType == 1001913 )
			{
				// ITEM_CLOVER_BUY
				g_HttpMng.GA_ItemHitTracking( g_MyInfo.GetUserIndex() 
					, "CloverBuy"
					, iNeedCoin
					, 1
					, dwGoodsIndex
					, "Item"
					, "%2FITEM%2FCLOVER%2FBUY" );
			}
			if( dwEtcItemType == 1000049 )
			{
				// ITEM_EVENT_BUY
				g_HttpMng.GA_ItemHitTracking( g_MyInfo.GetUserIndex() 
					, "EventBuy"
					, iNeedCoin
					, 1
					, dwGoodsIndex
					, "Item"
					, "%2FITEM%2FEVENT%2FBUY" );
			}
#endif

		}
		break;
	case EVENT_SHOP_GOODS_BUY_CLOSE:       // ���� ����
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );

			bool bEventClose, bCloverClose;
			rkPacket >> bEventClose >> bCloverClose;

			if( bEventClose )
			{
				g_EventGoodsMgr.EventShopClose();
			}

			if( bCloverClose )
			{
				g_EventGoodsMgr.CloverShopClose();
			}
		}
		break;
	case EVENT_SHOP_GOODS_BUY_SOLD_OUT:       // ��� ����
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		}
		break;
	case EVENT_SHOP_GOODS_BUY_UNKNOWN:       // �˼����� ��ǰ
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		}
		break;
	case EVENT_SHOP_GOODS_BUY_LIMIT:		  // ���� ���� ����
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		}
		break;
	case EVENT_SHOP_GOODS_BUY_FAILED:   	  // ���� �̵����� ������ ����
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
		}
		break;
	case EVENT_SHOP_GOODS_BUY_TERMS_FAILED:   // ���� ������ �ȵ�
		{
			DWORD dwEtcItemType;
			rkPacket >> dwEtcItemType;
			ioEtcItem *pEtcItem = g_EtcItemMgr.FindEtcItem( dwEtcItemType );
			if( pEtcItem )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(8), pEtcItem->GetName().c_str() );
			}
		}
		break;
	case EVENT_SHOP_GOODS_BUY_LIMIT_DATE:	  // �Ǹ� �Ⱓ�� �����
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(7) );
		}
		break;
	}
}

void ioApplication::OnShutDownUser( SP2Packet &rkPacket )
{
	bool bShutDownUser = false;
	bool bExitUser = false;
	rkPacket >> bShutDownUser;
	rkPacket >> bExitUser;
	g_MyInfo.SetShutDownUser( bShutDownUser );

	if( bExitUser )
	{
		LOG.PrintTimeAndLog( 0, "ExitProgram - 7" );

#if defined( USE_GA )
		if( GetGAStart() == true )
		{
			char chLabel[32] = {0,};

			if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
				sprintf_e( chLabel, "%d", 7 );
			else
				SafeSprintf( chLabel, sizeof(chLabel), "%1", 7 );

			// GAME_END_ERR_POS
			g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
				, "Game"
				, "Error"
				, chLabel
				, 1
				, "%2FGAME%2FOVER%2FERR"
				, 1 );

			g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FGAME%2FOVER%2FERR", 6, chLabel );
		}		
#endif

		SetExitProgram();
		Setting::Initialize("error");
		g_bHappenError = true;
		g_iErrorNumber = 330;

		if( g_MyInfo.GetChannelingType() == CNT_NEXON )
			SafeSprintf( g_szErrorLog, sizeof(g_szErrorLog), STR(2));
		else
			SafeSprintf( g_szErrorLog, sizeof(g_szErrorLog), STR(1));

		if( m_pLogin )
			m_pLogin->SendMsgToAutoUpgrade( WM_MY_MSG_C_ERROR );
	}
}

void ioApplication::OnSelectShutDownUser( SP2Packet &rkPacket )
{
	bool bNowShutDown = false;
	int iYear  = 2030; // ����Ʈ��
	int iMonth = 12;
	int iDay   = 31;
	int iHour  = 0;
	int iMinute= 0;
	rkPacket >> bNowShutDown;
	rkPacket >> iYear;
	rkPacket >> iMonth;
	rkPacket >> iDay;
	rkPacket >> iHour;
	rkPacket >> iMinute;
	g_MyInfo.SetSelectShutDownTime( iYear, iMonth, iDay, iHour, iMinute );

#ifdef DEBUG
	LOG.PrintTimeAndLog( 0, "%s [%d-%d-%d %d:%d]", __FUNCTION__, iYear, iMonth, iDay, iHour, iMinute );
#endif

	if( bNowShutDown )
	{
		LOG.PrintTimeAndLog( 0, "ExitProgram - 8" );

#if defined( USE_GA )
		if( GetGAStart() == true )
		{
			char chLabel[32] = {0,};

			if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
				sprintf_e( chLabel, "%d", 8 );
			else
				SafeSprintf( chLabel, sizeof(chLabel), "%1", 8 );

			// GAME_END_ERR_POS
			g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
				, "Game"
				, "Error"
				, chLabel
				, 1
				, "%2FGAME%2FOVER%2FERR"
				, 1 );

			g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FGAME%2FOVER%2FERR", 6, chLabel );
		}		
#endif

		SetExitProgram();
		Setting::Initialize("error");
		g_bHappenError = true;
		g_iErrorNumber = 331;

		if( g_MyInfo.GetChannelingType() == CNT_NEXON )
			SafeSprintf( g_szErrorLog, sizeof(g_szErrorLog), STR(2));
		else
			SafeSprintf( g_szErrorLog, sizeof(g_szErrorLog), STR(1));

		if( m_pLogin )
			m_pLogin->SendMsgToAutoUpgrade( WM_MY_MSG_C_ERROR );
	}
}

void ioApplication::OnFriendRecommendData( SP2Packet &rkPacket )
{
	DWORD dwTableIndex, dwFriendIndex;
	rkPacket >> dwTableIndex >> dwFriendIndex;

	if( dwTableIndex != 0 )
	{
		g_QuestMgr.QuestOccurTerm( QO_FRIEND_RECOMMEND );
	}
}

void ioApplication::OnMacroNotify( SP2Packet &rkPacket )
{
	ioHashString szNotifyID;
	rkPacket >> szNotifyID;

	if( !g_BattleRoomMgr.IsBattleRoom() ) return;    // �̹� �������� �ƴϴ�.

	g_MyInfo.SetBattleLockType( BattleLockProblemWnd::LT_MACRO );
}


void ioApplication::OnLoginMoveServer( SP2Packet &rkPacket )
{
	// �α��μ����� ���Ӽ��� ������� ���Ͽ� �ٸ� ������ �����̵�
	int iServerIndex;
	rkPacket >> iServerIndex;

	ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
	if( !pPlayStage ) return;

	if(0 == iServerIndex)
	{
		// �����̵� ���
		g_RserveTask.RemoveTask( RESERVEDCMD_TYPE_SERVERCHANGE );
	}
	else
	{
		// �κ� üũ
		BOOL bRightNow = FALSE;
		if( (pPlayStage->GetModeType() == MT_MYROOM) && (ioMyRoomMode::GetMyRoomType() == MRT_LOBBY) )
		{
			bRightNow = TRUE;
		}

		if( bRightNow )
		{
			TCPNetwork::MouseBusy( true );
			SP2Packet kPacket( CTPK_MYROOM_SERVER_CHANGE );
			kPacket << iServerIndex;
			TCPNetwork::SendToServer( kPacket );
		}
		else
		{
			ioTaskServerMove* task = new ioTaskServerMove;
			task->eType			= RESERVEDCMD_TYPE_SERVERCHANGE;
			task->eWhen			= RESERVEDCMD_WHEN_ENTERLOBBY;
			task->iServerIndex	= iServerIndex;
			g_RserveTask.AddReservedTask(task);
		}
	}
}

void ioApplication::OnSyncGiftCloverInfo( SP2Packet &rkPacket )
{
	ioClover *pClover = g_MyInfo.GetClover();
	if( pClover )
	{
		pClover->ApplyCloverInfo( rkPacket );
	}
}

void ioApplication::OnSyncFriendCloverInfo( SP2Packet &rkPacket )
{
	DWORD dwUserIndex;
	rkPacket >> dwUserIndex;
	FriendNode *pFriend = g_FriendManager.GetNodeToUserIndex( dwUserIndex );
	if( pFriend )
	{
		pFriend->ApplyFriendClover( rkPacket );
	}
}

void ioApplication::OnEtcItemCloverUpdate( SP2Packet &rkPacket )
{
	ioClover *pClover = g_MyInfo.GetClover();
	if( pClover )
	{
		pClover->ApplyEtcItemClover( rkPacket );
	}
}

bool ioApplication::IsWemadeChannelingGetCash()
{
	ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
	if( pLocal && pLocal->GetType() == ioLocalManager::LCT_KOREA )
	{
		ioChannelingNodeParent *pNode = g_ChannelingMgr.GetNode( g_MyInfo.GetChannelingType() );
		if( pNode && pNode->GetType() == CNT_WEMADEBUY )
		{
			return true;
		}
	}

	return false;
}

void ioApplication::OnAddCash( SP2Packet &rkPacket )
{
	if( IsWemadeChannelingGetCash() ) 
		TCPNetwork::MouseBusy( false );
	
	int  iType = 0;
	rkPacket >> iType;

	if( iType == ADD_CASH_SUCCESS )
	{
		// ui
		int   iAddCash      = 0;
		DWORD dwEtcItemType = 0; 
		rkPacket >> iAddCash;
		rkPacket >> dwEtcItemType;

		MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(g_GUIMgr.FindWnd(MY_INVENTORY_WND));
		if( pInvenWnd && pInvenWnd->IsShow() )
		{
			ioEtcItem *pEtcItem = g_EtcItemMgr.FindEtcItem( dwEtcItemType );
			if( pEtcItem )
			{
				char szConvertNum[MAX_PATH] = "";
				ioHashString szIconName = pEtcItem->GetIconName();
				Help::ConvertNumToStrComma( iAddCash, szConvertNum, sizeof( szConvertNum ) );

				ComplexStringPrinterVec vTitle;
				ComplexStringPrinterVec vDesc;

				ioComplexStringPrinter kTitle1;
				kTitle1.SetTextStyle( TS_NORMAL );
				kTitle1.SetBkColor( 0, 0, 0 );
				kTitle1.SetTextColor( TCT_DEFAULT_DARKGRAY );
				kTitle1.AddTextPiece( FONT_SIZE_17, pEtcItem->GetName().c_str() );
				vTitle.push_back( kTitle1 );

				ioComplexStringPrinter kTitle2;
				kTitle2.SetTextStyle( TS_NORMAL );
				kTitle2.SetBkColor( 0, 0, 0 );
				kTitle2.SetTextColor( TCT_DEFAULT_DARKGRAY );
				kTitle2.AddTextPiece( FONT_SIZE_17, STR(1), szConvertNum );
				vTitle.push_back( kTitle2 );

				ioComplexStringPrinter kDesc1;
				kDesc1.SetTextStyle( TS_NORMAL );
				kDesc1.SetBkColor( 0, 0, 0 );	
				kDesc1.SetTextColor( TCT_DEFAULT_DARKGRAY );
				kDesc1.AddTextPiece( FONT_SIZE_12, STR(2), pEtcItem->GetName().c_str() );
				vDesc.push_back( kDesc1 );

				ioComplexStringPrinter kDesc2;
				kDesc2.SetTextStyle( TS_NORMAL );
				kDesc2.SetBkColor( 0, 0, 0 );
				kDesc2.SetTextColor( TCT_DEFAULT_DARKGRAY );
				kDesc2.AddTextPiece( FONT_SIZE_12, STR(1), szConvertNum );
				vDesc.push_back( kDesc1 );

				pInvenWnd->UpdateTab( false, true );
				pInvenWnd->ShowItemRecvSellInfoWnd( ItemRecvSellInfoWnd::ITEM_RECV_NOT_GO, ItemRecvSellInfoWnd::ITEM_ETCITEM, szIconName, vTitle, vDesc, -1, 0, false, false, pEtcItem->GetGradeType() );
			}
		}

		// cash
		ioChannelingNodeParent *pNode = g_ChannelingMgr.GetNode( g_MyInfo.GetChannelingType() );
		if( !pNode )
		{
			LOG.PrintTimeAndLog( 0 , "Add Cash pNode == NULL." );
			return;
		}
		pNode->OnRecieveAddCash( rkPacket );
		return;
	}
	else if( iType == ADD_CASH_BILLING_WAIT )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
	}
	else if( iType == ADD_CASH_BILLING_DISCONNECT )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
	}
	else if( iType == ADD_CASH_BILLING_MOVING_SERVER )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6) );
	}
	else if( iType == ADD_CASH_BILLING_EXCEPTION )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(7) );
	}
	else if( iType == GET_CASH_BILLING_FAIL )
	{
		bool bBillingError    = false;
		ioHashString  sBillingError;
		rkPacket >> bBillingError;
		if( bBillingError )
			rkPacket >> sBillingError;

		ioHashString sErrorMent = STR(8);
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
				StringCbPrintf( szErrorFullMent, sizeof( szErrorFullMent ), "%s",  sError.c_str() ); //Except Extracting Hangeul
			else
				StringCbPrintf( szErrorFullMent, sizeof( szErrorFullMent ), "%s#%s%s", sErrorMent.c_str(), sError.c_str(), sBillingError.c_str() ); //Except Extracting Hangeul

			g_GUIMgr.SetMsgBox( MB_OK, NULL, szErrorFullMent );
		}
	}

	if( IsWemadeChannelingGetCash() )
	{
		NewShopWnd *pNewShopWnd = (NewShopWnd*)( g_GUIMgr.FindWnd( NEW_SHOP_WND ) );
		if( pNewShopWnd )
			pNewShopWnd->SendGetCash( true );			
	}
	
}

void ioApplication::OnFillCashUrl( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );

	int iType = 0;
	rkPacket >> iType;

	if( iType != FILL_CASH_URL_OK )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return;
	}

	ioHashString szLoginCashUrl;
	rkPacket >> szLoginCashUrl;

	if( szLoginCashUrl.IsEmpty() )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return;
	}

#ifdef STEAM_ATTACH
	if(g_ioSteamClient.GetSteamEnable())
	{
		g_ioSteamClient.OpenWebPage(szLoginCashUrl.c_str());
		return;
	}
#endif

	if( !m_pBrowserMgr )
		return;

	m_pBrowserMgr->OpenBrowser( szLoginCashUrl.c_str(),
		BROWSE_FILL_CASH,
		BROWSE_SUB_FILL_CASH, true, true );
}

void ioApplication::OnRelativeGradeBonus( SP2Packet &rkPacket )
{
	RelativeGradeBonusWnd *pWnd = dynamic_cast<RelativeGradeBonusWnd*>( g_GUIMgr.FindWnd(RELATIVE_GRADE_BONUS_WND) );
	if( pWnd )
		pWnd->SetInfoToPacket( rkPacket );
}

void ioApplication::OnUpdateRelativeGradeExp( SP2Packet &rkPacket )
{
	int iInitCode = 0, iExp = 0;
	rkPacket >> iInitCode >> iExp;

	if( iInitCode == g_MyInfo.GetRelativeGradeInitCode() )
		return;

	g_MyInfo.SetGradeExp( iExp );
	g_UserInfoMgr.SetGrade( g_MyInfo.GetPublicID(), g_MyInfo.GetGradeLevel(), iExp );
}

void ioApplication::OnLoginToken( SP2Packet &rkPacket )
{
	ioHashString sLoginToken;
	rkPacket >> sLoginToken;

	ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
	if( pLocal )
		pLocal->SetWebLoginKey( sLoginToken );
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return g_App.MsgProc( hWnd, uMsg, wParam, lParam );
}

bool ioApplication::OnNCPaint( HDC hDC )
{
	if( !m_hWindowTitleLosa || !m_hWindowOutLine )
		return false;

	RECT rcWindow;
	GetWindowRect( m_hWnd, &rcWindow );

	int iWidth  = rcWindow.right - rcWindow.left;
	int iHeight = rcWindow.bottom - rcWindow.top;

	HDC hBitmapDC = CreateCompatibleDC( hDC );
	HBITMAP hOldBitmap = (HBITMAP)SelectObject( hBitmapDC, m_hWindowTitleLosa );

	// top ( �̹��� �ϳ��� �����ؼ� ����Ѵ� )

	enum { FRONT_WIDTH = 173, HEIGHT = 29, };
	StretchBlt( hDC, 0, 0, FRONT_WIDTH, HEIGHT, hBitmapDC, 0, 0, FRONT_WIDTH, HEIGHT, SRCCOPY ); // title front

	enum { X_START = 173, CENTER_WIDTH = 8, END_WIDTH = 11 };
	int iTopCenterWidth = iWidth - FRONT_WIDTH + END_WIDTH;  // title center
	StretchBlt( hDC, FRONT_WIDTH, 0, iTopCenterWidth, HEIGHT, hBitmapDC, X_START, 0, CENTER_WIDTH, HEIGHT, SRCCOPY );

	int iTopRightStart = iWidth - END_WIDTH;
	StretchBlt( hDC, iTopRightStart, 0, END_WIDTH, HEIGHT, hBitmapDC, X_START, 0, END_WIDTH, HEIGHT, SRCCOPY ); // title end

	// left & right
	SelectObject( hBitmapDC, m_hWindowOutLine );

	int iSideHeight = iHeight - 29 - 3;
	StretchBlt( hDC, 0, 29, 3, iSideHeight, hBitmapDC, 0, 0, 3, 3, SRCCOPY );

	int iSideRightStart = iWidth - 3;
	StretchBlt( hDC, iSideRightStart, 29, 3, iSideHeight, hBitmapDC, 3 * 2, 0, 3, 3, SRCCOPY );

	// bottom
	int iBottomStart = iHeight - 3;
	StretchBlt( hDC, 0, iBottomStart, 3, 3, hBitmapDC, 0, 3, 3, 3, SRCCOPY );

	int iBottomCenterWidth = iWidth - 3 * 2;
	StretchBlt( hDC, 3, iBottomStart, iBottomCenterWidth, 3, hBitmapDC, 3, 3, 3, 3, SRCCOPY );

	int iBottomRightStart = iWidth - 3;
	StretchBlt( hDC, iBottomRightStart, iBottomStart, 3, 3, hBitmapDC, 3 * 2, 3, 3, 3, SRCCOPY );

	SelectObject( hBitmapDC, hOldBitmap );
	DeleteDC( hBitmapDC );

	return true;
}

LRESULT ioApplication::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( m_pInputBox )
	{
		if( m_pInputBox->InputBoxProc( hWnd, uMsg, wParam, lParam ) )
		{
			SetAfterCandidateWnd( uMsg, wParam );
			return 0;
		}
	}	

	switch( uMsg )
	{
	case WM_HOTKEY:
		{
			if ( ApplyHotKey( wParam ) )
				return 0;
		}
		break;
	case WM_TIMER:
		if (wParam == WM_IO_IMETIMER)
		{
			if( !m_bProgramExit && m_pTCPSocket )
			{
				if( !ChangeServer() ) 
					MainLoop();

				CheckServerToClientPing();
				CheckLanConnected();
				CheckRecvUDPUnviable();
				CheckPacExceptionLog();
			}
		}
		break;
	case WM_QUERYENDSESSION:
		QueryEndSessionProcess();
		return FALSE;
	case WM_NCHITTEST:		// Ÿ��Ʋ�� ����
		return HTCLIENT;
	case WM_SIZE:
		if( wParam == SIZE_MAXIMIZED || wParam == SIZE_RESTORED )
		{
			// �ִ�ȭ�� �Ǹ� ��Ȥ �Ҹ��� �ִ�� �Ǵ� ���������� �̰����� ������ �缳���մϴ�.
			// ������ ������ ������ �缳������ �����Ƿ� +1�� ��
			if( ioSoundManager::GetSingletonPtr() )
			{
				int iNewVolume = g_SoundMgr.GetVolumeStream() + 1;
				g_SoundMgr.SetVolumeStream( iNewVolume );
				g_SoundMgr.SetOggVolume( iNewVolume );
			}
		}
		break;
#ifdef SHIPPING
	case WM_SYSCOMMAND:		// ALT+F6 ����
		switch( wParam & 0xFFF0 )
		{
		case SC_PREVWINDOW:
		case SC_NEXTWINDOW:
		case SC_KEYMENU:
		case SC_MOUSEMENU:
			return 0;
		}
		break;
#endif
	case WM_ACTIVATEAPP:
		m_bAppActive = (BOOL)wParam;
		break;
	case WM_NCPAINT:
	case WM_NCACTIVATE:
		if( !Setting::FullScreen() )
		{
			bool bPaintOK = false;

			HDC hDC = GetWindowDC( hWnd );
			if( hDC )
			{
				SetWindowRgn( hWnd, NULL, FALSE );
				bPaintOK = OnNCPaint( hDC );
				ReleaseDC( hWnd, hDC );
			}

			if( bPaintOK )
			{
				if( uMsg == WM_NCPAINT )
					return 0;

				if( uMsg == WM_NCACTIVATE )
					return TRUE;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		m_ptPreMouse.x = GET_X_LPARAM(lParam);
		m_ptPreMouse.y = GET_Y_LPARAM(lParam);
		m_Mouse.SetLBtnDown( m_ptPreMouse.x, m_ptPreMouse.y );
		SetCapture( hWnd );
		WindowTitleDragStart();
		break;
	case WM_MBUTTONDOWN:
		{
			CheckMouseControlMode( true );

			m_ptPreMouse.x = GET_X_LPARAM(lParam);
			m_ptPreMouse.y = GET_Y_LPARAM(lParam);
			m_Mouse.SetMBtnDown( m_ptPreMouse.x, m_ptPreMouse.y );
			SetCapture( hWnd );			
		}
		break;
	case WM_RBUTTONDOWN:
		{
			m_bMouseControlModeCancel = true;
			CheckMouseControlMode( true );

			m_ptPreMouse.x = GET_X_LPARAM(lParam);
			m_ptPreMouse.y = GET_Y_LPARAM(lParam);
			m_Mouse.SetRBtnDown( m_ptPreMouse.x, m_ptPreMouse.y );
			SetCapture( hWnd );
		}		
		break;
	case WM_MOUSEMOVE:
		{
			m_ptMouseMoveAmt.x = GET_X_LPARAM(lParam) - m_ptPreMouse.x;
			m_ptMouseMoveAmt.y = GET_Y_LPARAM(lParam) - m_ptPreMouse.y;

			m_ptPreMouse.x = GET_X_LPARAM(lParam);
			m_ptPreMouse.y = GET_Y_LPARAM(lParam);
			m_Mouse.SetMouseMove( m_ptPreMouse.x, m_ptPreMouse.y );
			m_bMouseControlModeCancel = false;
		}
		break;
	case WM_LBUTTONUP:
		{
			m_ptPreMouse.x = GET_X_LPARAM(lParam);
			m_ptPreMouse.y = GET_Y_LPARAM(lParam);
			m_Mouse.SetLBtnUp( m_ptPreMouse.x, m_ptPreMouse.y );
			ReleaseCapture();
			WindowTitleDragEnd();			
		}		
		break;
	case WM_MBUTTONUP:
		m_ptPreMouse.x = GET_X_LPARAM(lParam);
		m_ptPreMouse.y = GET_Y_LPARAM(lParam);
		m_Mouse.SetMBtnUp( m_ptPreMouse.x, m_ptPreMouse.y );
		ReleaseCapture();
		break;
	case WM_RBUTTONUP:
		{
			m_ptPreMouse.x = GET_X_LPARAM(lParam);
			m_ptPreMouse.y = GET_Y_LPARAM(lParam);
			m_Mouse.SetRBtnUp( m_ptPreMouse.x, m_ptPreMouse.y );
			ReleaseCapture();

			if( m_bMouseControlModeCancel )
				CheckMouseControlMode( false );
			m_bMouseControlModeCancel = false;
		}
		break;
	case WM_LBUTTONDBLCLK:
		m_ptPreMouse.x = GET_X_LPARAM(lParam);
		m_ptPreMouse.y = GET_Y_LPARAM(lParam);
		m_Mouse.SetMBtnDBLCLK( m_ptPreMouse.x, m_ptPreMouse.y );

#ifndef SHIPPING
// �����̳ʵ��� �۾��� ���� ��� ( ���ϼ��̺��ϸ� �ڵ����� ��ü���ִ� )
		if( m_pTextureMgr )
		{
			m_pTextureMgr->ReLoadUpdateFiles();
		}
#endif
		break;
	case WM_MBUTTONDBLCLK:
		m_ptPreMouse.x = GET_X_LPARAM(lParam);
		m_ptPreMouse.y = GET_Y_LPARAM(lParam);
		m_Mouse.SetLBtnDBLCLK( m_ptPreMouse.x, m_ptPreMouse.y );
		break;
	case WM_RBUTTONDBLCLK:
		m_ptPreMouse.x = GET_X_LPARAM(lParam);
		m_ptPreMouse.y = GET_Y_LPARAM(lParam);
		m_Mouse.SetRBtnDBLCLK( m_ptPreMouse.x, m_ptPreMouse.y );
		break;
	case WM_MOUSEWHEEL:
		m_Mouse.SetMouseWheel( m_ptPreMouse.x, m_ptPreMouse.y, GET_WHEEL_DELTA_WPARAM(wParam) );
		break;
	case WM_GAMESOCKET1:
	case WM_GAMESOCKET2:
		if( m_pTCPSocket )
		{
			m_pTCPSocket->EvaluateSocket( uMsg, wParam, lParam );
		}
		break;
	case WM_P2PSOCKET:
		if( m_pUDPSocket )
		{
			m_pUDPSocket->EvaluateSocket( wParam, lParam );
		}
		break;
	case WM_FILESOCKET:
		if( m_pFileWriteClient )
		{
			m_pFileWriteClient->EvaluateSocket( wParam, lParam );
		}
		break;
	case WM_COPYDATA:
		{
			COPYDATASTRUCT *cds = (COPYDATASTRUCT*)lParam;
			if( cds )
			{
				switch(cds->dwData)
				{
				case WM_MY_MSG_A_EXIT:
					if(m_pLogin)
						m_pLogin->SetLoginState(CLogin::LS_USER_CLICK_EXIT);
					break;
				}
			}
		}
		break;
	case WM_QUIT:
		//UJ 140318, ���Ȱ�ȭ: ioHashString������ �񱳰� ������ ��Ʈ�� ������ ����
		// ���� �̷� �ڵ尡 ���� ���� ioHashString==char*�� ���Ƽ� ȿ���� ������
		if( wParam == 367 )
		{
			char szErrorMsg[1024];
			wsprintf( szErrorMsg, "%s/%s/%s : hacked string", g_MyInfo.GetMyIP().c_str(),
				g_MyInfo.GetPrivateID().c_str(),
				g_MyInfo.GetPublicID().c_str() );

			SP2Packet kPacket( LUPK_LOG );
			kPacket << "CheckError";  // �α� ���� Ÿ��
			kPacket << szErrorMsg;
			kPacket << 367;  // ������ȣ:367
			kPacket << true; // write db
			P2PNetwork::SendLog(kPacket);

			ioHashString a("test");
			ioHashString b("test2");
			if( a == b )
			{
				::SleepEx(0, FALSE);
			}

			LOG.PrintTimeAndLog( 0, "ExitProgram - 367" );

#if defined( USE_GA )
			if( GetGAStart() == true )
			{
				char chLabel[32] = {0,};

				if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
					SafeSprintf( chLabel, sizeof(chLabel), STR(1), 367 );
				else
					SafeSprintf( chLabel, sizeof(chLabel), "%1", 367 );

				// GAME_END_ERR_POS
				g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
					, "Game"
					, "Error"
					, chLabel
					, 1
					, "%2FGAME%2FOVER%2FERR" 
					, 1 );

				g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FGAME%2FOVER%2FERR", 6, chLabel );
			}			
#endif

			g_App.SetExitProgram();

			Setting::Initialize("error");
			g_bHappenError = true;
			g_iErrorNumber = 367;
		}
		break;
	case WM_CLOSE:
		if( P2PNetwork::IsNetworkPlaying() )
		{
			AltF4ExitProcess();
			return 0;
		}
		break;
	case WM_DESTROY:
#ifdef SHIPPING
		if( !g_bHappenCrash && !g_bHappenError )
		{
			if(!m_pBrowserMgr->GetChannelingURL().IsEmpty())
				Help::ExcuteWeb( m_pBrowserMgr->GetChannelingURL().c_str() );

#ifdef PENGUIN_PING

			STARTUPINFO si;
			PROCESS_INFORMATION pi;

			ZeroMemory(&si, sizeof(STARTUPINFO));
			si.cb = sizeof(STARTUPINFO);

			char buf[256];
			sprintf(buf, "lsdump.exe LsLog_%s_%s_%s", STRFILEVER, g_MyInfo.GetPrivateID().c_str(), 
				g_MyInfo.GetPublicID().c_str());

			if ( CreateProcess(NULL, buf, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi) )
			{
			}

#endif

#if defined( USE_GA )
			if( GetGAStart() == true )
			{
				if( g_HttpMng.GetGAFailSend() == true )
				{
					STARTUPINFO si2;
					PROCESS_INFORMATION pi2;

					ZeroMemory(&si2, sizeof(STARTUPINFO));
					si2.cb = sizeof(STARTUPINFO);

					char buf2[256];

					sprintf(buf2, "lsdump.exe GaLog_%s_%s_%s", STRFILEVER, g_MyInfo.GetPrivateID().c_str(), 
						g_MyInfo.GetPublicID().c_str());

					if ( CreateProcess(NULL, buf2, NULL, NULL, FALSE, 0, NULL, NULL, &si2, &pi2) )
					{
					}
				}	
			}

			SetGAStart( false );
#endif
		}
#endif
		try
		{
			ReleaseAll();
		}
		catch(...)
		{
			MEMORYSTATUSEX statex;
			statex.dwLength = sizeof (statex);
			GlobalMemoryStatusEx (&statex);

			LOG.PrintTimeAndLog(0, ("There is  %d percent of memory in use."),statex.dwMemoryLoad);
			LOG.PrintTimeAndLog(0, ("There are %d total Mbytes of physical memory."),statex.ullTotalPhys/1048576);
			LOG.PrintTimeAndLog(0, ("There are %d free Mbytes of physical memory."), statex.ullAvailPhys/1048576);
			LOG.PrintTimeAndLog(0, ("There are %d total Mbytes of paging file."), statex.ullTotalPageFile/1048576);
			LOG.PrintTimeAndLog(0, ("There are %d free Mbytes of paging file."), statex.ullAvailPageFile/1048576);
			LOG.PrintTimeAndLog(0, ("There are %d total Mbytes of virtual memory."), statex.ullTotalVirtual/1048576);
			LOG.PrintTimeAndLog(0, ("There are %d free Mbytes of virtual memory."), statex.ullAvailVirtual/1048576);
			LOG.PrintTimeAndLog(0, ("There are %d free Mbytes of extended memory."), statex.ullAvailExtendedVirtual/1048576);

			LOG.PrintTimeAndLog( 0, "%s", __FUNCTION__ );
		}

#ifdef SHIPPING
		if( !g_bHappenCrash && !g_bHappenError )
		{
			Help::MinimizeAllWindows(false);
			ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
			if( pLocal && pLocal->IsExecuteLuncher() )
				ExecuteLuncher();
			if( pLocal )
				pLocal->ExitAction();
		}
#endif		
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

BOOL ioApplication::LoginProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( m_pLogin )
	{
		return m_pLogin->LoginProc( hWnd, uMsg, wParam, lParam );
	}

	return FALSE;
}

void ioApplication::DebugWebLogin( OUT LPSTR &szCmdLine )
{
	if(m_pLogin )
	{
		m_pLogin->DebugWebLogin(szCmdLine);
	}
}

bool ioApplication::ParseCmd( IN const char *szCmdLine,
							  OUT ioHashString &szKeyOnCmd,
							  OUT ioHashString &szOptinosOnCmd,
							  OUT ioHashString &szIPOnCmd,
							  OUT int &iPortOnCmd )
{
	if( m_pLogin )
		return m_pLogin->ParseCmd( szCmdLine, szKeyOnCmd, szOptinosOnCmd, szIPOnCmd, iPortOnCmd );

	return false;
}

bool ioApplication::ProcessOptions( const ioHashString &szOptions )
{
	if( m_pLogin )
		return m_pLogin->ProcessOptions( szOptions );

	return false;
}

bool ioApplication::LoginLoop( const ioHashString &szKey )
{
	bool bReturn = false;
	if( m_pLogin )
	{
		if(m_pLogin->IsSingleMode())
		{
#ifndef SHIPPING // ADMIN
			Setting::Initialize( "character1" );
			m_pLogin->SetAdminSettings();
			SingleSettingWnd::SetSingle( true );
#endif
			bReturn = true;
		}
		else
		{
			bReturn = m_pLogin->LoginLoop( szKey );
			m_iLoginResult = m_pLogin->GetLoginResult();
		}

		SAFEDELETE(m_pLogin);
	}

	return bReturn;
}

void ioApplication::UpdateReserveLogOut()
{
	if( m_dwReserveLogOutTime != 0 )
	{
		if( FRAMEGETTIME() - m_dwReserveLogOutTime > m_dwReserveLogOutRandTime ) // 30 ~ 90��
		{
			DisconnectWnd *pWnd = dynamic_cast<DisconnectWnd*> ( g_GUIMgr.FindWnd( DISCONNECT_WND ) );
			if( pWnd )
			{
				if( m_dwReserveLogOutType == RESERVE_LOGOUT_SHUT_DOWN )
					pWnd->SetType( DisconnectWnd::TYPE_SHUT_DOWN_EXIT );
				else if( m_dwReserveLogOutType == RESERVE_LOGOUT_SELECT_SHUT_DOWN )
					pWnd->SetType( DisconnectWnd::TYPE_SELECT_SHUT_DOWN_EXIT );
				else
					pWnd->SetType( DisconnectWnd::TYPE_NETWORK_EXIT );
				pWnd->ShowWnd();
			}
			else
			{
				if( m_pQuestManager )
					m_pQuestManager->ProcessChangeStage();
				SP2Packet kPacket( CTPK_LOGOUT );
				TCPNetwork::SendToServer( kPacket );
			}
			g_ChatMgr.SetSystemMsg( STR(1) );
			m_dwReserveLogOutTime = 0;
		}
		else if( FRAMEGETTIME() - m_dwReserveLogOutCnt > 1000 )  // 1��
		{
			g_ChatMgr.SetSystemMsg( STR(2), ( m_dwReserveLogOutRandTime - ( FRAMEGETTIME() - m_dwReserveLogOutTime ) ) /FLOAT1000  );
			m_dwReserveLogOutCnt = FRAMEGETTIME();
		}
	}
}

void ioApplication::ExecuteLuncher()
{
	// autoluncher.exe WebRefreshType�� �����ؾ���
	enum WebRefreshType
	{
		WFT_NORMAL     = 1,
		WFT_WEB_LOGOUT = 2,
		WFT_MAX,
	};

	WebRefreshType eType = WFT_NORMAL;
	if( m_bWebLogOutForLuncher )
		eType = WFT_WEB_LOGOUT;

	char szCmd[MAX_PATH*2]="";
	StringCbPrintf(szCmd, sizeof(szCmd), " %s?%d?%s", NORMAL_EXCUTE_CHECK, (int)eType , GetAutoupgradeInfoININame().c_str() );

	char szRootDir[MAX_PATH]="";
	::GetCurrentDirectory(MAX_PATH,szRootDir);

	char szLuncherPath[MAX_PATH]="";
	StringCbPrintf(szLuncherPath, sizeof(szLuncherPath), "%s\\autoluncher.exe", szRootDir);

	Help::ExcuteProcess(szLuncherPath, szCmd);
}

void ioApplication::SetupForSound()
{
	if( Setting::UseBGM() )
	{
		g_SoundMgr.SetCanPlayStream( true );
		g_SoundMgr.SetVolumeStream( Help::GetDSVolume(Setting::GetBGMVolume(), Help::VT_STREAM ) );
	}
	else
	{
		g_SoundMgr.SetCanPlayStream( false );
		g_SoundMgr.SetVolumeStream( DSBVOLUME_MIN );
	}

	if(Setting::UseEffectSound())
	{
		g_SoundMgr.SetCanPlaySound( true );
		g_SoundMgr.SetVolumeNormal( Help::GetDSVolume(Setting::GetEffectSoundVolume(), Help::VT_NORMAL ) );
		g_SoundMgr.SetVolume3D ( Help::GetDSVolume(Setting::GetEffectSoundVolume(), Help::VT_3D ) );
	}
	else
	{
		g_SoundMgr.SetCanPlaySound( false );
		g_SoundMgr.SetVolumeNormal( DSBVOLUME_MIN );
		g_SoundMgr.SetVolume3D ( DSBVOLUME_MIN );
	}

	if(Setting::UseCustomSound())
	{
		g_SoundMgr.SetCanPlayPiece( true );
		g_SoundMgr.SetVolumePiece( Help::GetDSVolume(Setting::GetCustomSoundVolume(), Help::VT_NONE ) );
	}
	else
	{
		g_SoundMgr.SetCanPlayPiece( false );
		g_SoundMgr.SetVolumePiece( DSBVOLUME_MIN );
	}

	g_VoiceChat.Init(m_hWnd);
	g_VoiceChat.InitCapture();
	g_VoiceChat.StopCapture();

	if(Setting::UseOtherVoiceChat())
		g_VoiceChat.SetAllVolume(Help::GetDSVolume(Setting::GetVoiceVolume(), Help::VT_NONE ));
	else
		g_VoiceChat.SetAllVolume(DSBVOLUME_MIN);
}

bool ioApplication::CheckFirstMyRoom()
{
	if( m_bFirstMyRoom )
	{
		m_bFirstMyRoom = false;
		return true;
	}

	return false;
}

bool ioApplication::CheckFirstPlaza()
{
	if( m_bFirstPlaza )
	{
		m_bFirstPlaza = false;
		return true;
	}

	return false;
}

void ioApplication::SendINILog()
{
	ioINILoader_e kIniLoder("Save\\default\\setting.ini",false);
	kIniLoder.SetTitle_e("Setting");
	int iErrorNumber = kIniLoder.LoadInt_e("ClientError", 0 );
	if( iErrorNumber != 0 && m_pUDPSocket )
	{
		kIniLoder.SaveInt("ClientError", 0 );

		enum { MAX_DESC = 4, };
		int iErrorEescArray[MAX_DESC]={ 0, 0, 0, 0, };
		for (int i = 0; i < MAX_DESC ; i++)
		{
			char szKey[MAX_PATH]="";
			StringCbPrintf( szKey, sizeof( szKey ), "ClientErrorDesc%d", i+1 );
			iErrorEescArray[i] = kIniLoder.LoadInt( szKey, 0 );			
			kIniLoder.SaveInt( szKey, 0 );
		}

		char szErrorLog[MAX_PATH]="";
		StringCbPrintf(szErrorLog, sizeof(szErrorLog), "%s/%s/%s/INFO:%d/INFO:%d/VER:%d/DATE:%d/Fail Client Init(Saved INI)",
			                                                                        m_pUDPSocket->GetMyIP(),
			                                                                        g_MyInfo.GetPrivateID().c_str(),
			                                                                        g_MyInfo.GetPublicID().c_str(),
																					iErrorEescArray[0], 
																					iErrorEescArray[1],
																					iErrorEescArray[2],
																					iErrorEescArray[3] );
		SP2Packet kPacket( LUPK_LOG );
		kPacket << "CheckError";  // �α� ���� Ÿ��
		kPacket << szErrorLog;
		kPacket << iErrorNumber;  // ������ȣ
		kPacket << true;          // write db
		P2PNetwork::SendLog(kPacket);
	}
}

#ifdef _DEBUG
void ioApplication::PrintPacketID( DWORD dwPacketID , bool bSend /*= true */, const char *szRcvIP /*=""*/, int iRcvPort /*= 0*/ )
{
	return;

	if ( m_dwCurPacketPrintCount > m_dwMaxPacketPrintCount )
		return;

	m_dwCurPacketPrintCount++;

	char szName[MAX_PATH]="";
	switch( dwPacketID )
	{
	// TCP
	case CTPK_CONNECT:
		StringCbCopy( szName, sizeof( szName ), "CTPK_CONNECT" );
		break;
	case CTPK_CHAR_CREATE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_CHAR_CREATE" );
		break;
	case CTPK_LOGOUT:
		StringCbCopy( szName, sizeof( szName ), "CTPK_LOGOUT" );
		break;
	case CTPK_CHAR_DELETE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_CHAR_DELETE" );
		break;
	case CTPK_PLAZA_ROOM_LIST:
		StringCbCopy( szName, sizeof( szName ), "CTPK_PLAZA_ROOM_LIST" );
		break;
	case CTPK_JOIN_ROOM:
		StringCbCopy( szName, sizeof( szName ), "CTPK_JOIN_ROOM" );
		break;
	case CTPK_EXIT_ROOM:
		StringCbCopy( szName, sizeof( szName ), "CTPK_EXIT_ROOM" );
		break;
	case CTPK_DROP_ITEM:
		StringCbCopy( szName, sizeof( szName ), "CTPK_DROP_ITEM" );
		break;
	case CTPK_PICK_ITEM:
		StringCbCopy( szName, sizeof( szName ), "CTPK_PICK_ITEM" );
		break;
	case CTPK_DROP_DIE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_DROP_DIE" );
		break;
	case CTPK_WEAPON_DIE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_WEAPON_DIE" );
		break;
	case CTPK_SYMBOL_DIE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_SYMBOL_DIE" );
		break;
	case CTPK_CREATE_PLAZA:
		StringCbCopy( szName, sizeof( szName ), "CTPK_CREATE_PLAZA" );
		break;
	case CTPK_PRISONER_ESCAPE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_PRISONER_ESCAPE" );
		break;
	case CTPK_CHANGE_CHAR:
		StringCbCopy( szName, sizeof( szName ), "CTPK_CHANGE_CHAR" );
		break;
	case CTPK_PRISONER_DROP:
		StringCbCopy( szName, sizeof( szName ), "CTPK_PRISONER_DROP" );
		break;
	case CTPK_PUSHSTRUCT_DIE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_PUSHSTRUCT_DIE" );
		break;
	case CTPK_PUSHSTRUCT_CREATE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_PUSHSTRUCT_CREATE" );
		break;
	case CTPK_CURRENT_DAMAGELIST:
		StringCbCopy( szName, sizeof( szName ), "CTPK_CURRENT_DAMAGELIST" );
		break;
	case CTPK_USE_ITEM:
		StringCbCopy( szName, sizeof( szName ), "CTPK_USE_ITEM" );
		break;
	case CTPK_BUY_ITEM:
		StringCbCopy( szName, sizeof( szName ), "CTPK_BUY_ITEM" );
		break;
	case CTPK_EQUIP_SLOT_ITEM:
		StringCbCopy( szName, sizeof( szName ), "CTPK_EQUIP_SLOT_ITEM" );
		break;
	case CTPK_SELL_SLOT_ITEM:
		StringCbCopy( szName, sizeof( szName ), "CTPK_SELL_SLOT_ITEM" );
		break;
	case CTPK_INC_STAT:
		StringCbCopy( szName, sizeof( szName ), "CTPK_INC_STAT" );
		break;
	case CTPK_INIT_STAT:
		StringCbCopy( szName, sizeof( szName ), "CTPK_INIT_STAT" );
		break;
	case CTPK_CREATE_OBJECTITEM:
		StringCbCopy( szName, sizeof( szName ), "CTPK_CREATE_OBJECTITEM" );
		break;
	case CTPK_EVENT_SCENE_END:
		StringCbCopy( szName, sizeof( szName ), "CTPK_EVENT_SCENE_END" );
		break;
	case CTPK_LEVELUP_ITEM:
		StringCbCopy( szName, sizeof( szName ), "CTPK_LEVELUP_ITEM" );
		break;
	case CTPK_REPOSITION_FIELDITEM:
		StringCbCopy( szName, sizeof( szName ), "CTPK_REPOSITION_FIELDITEM" );
		break;
	case CTPK_SELL_EQUIP_ITEM:
		StringCbCopy( szName, sizeof( szName ), "CTPK_SELL_EQUIP_ITEM" );
		break;
	case CTPK_JOIN_BATTLEROOM_LIST:
		StringCbCopy( szName, sizeof( szName ), "CTPK_JOIN_BATTLEROOM_LIST" );
		break;
	case CTPK_CREATE_BATTLEROOM:
		StringCbCopy( szName, sizeof( szName ), "CTPK_CREATE_BATTLEROOM" );
		break;
	case CTPK_USER_BATTLEROOM_JOIN:
		StringCbCopy( szName, sizeof( szName ), "CTPK_USER_BATTLEROOM_JOIN" );
		break;
	case CTPK_USER_BATTLEROOM_LEAVE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_USER_BATTLEROOM_LEAVE" );
		break;
	case CTPK_SYMBOL_DAMAMGED:
		StringCbCopy( szName, sizeof( szName ), "CTPK_SYMBOL_DAMAMGED" );
		break;
	case CTPK_MACRO_COMMAND:
		StringCbCopy( szName, sizeof( szName ), "CTPK_MACRO_COMMAND" );
		break;
	case CTPK_PRISONERMODE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_PRISONERMODE" );
		break;
	case CTPK_CHAR_SLOT_CHANGE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_CHAR_SLOT_CHANGE" );
		break;
	case CTPK_CREATE_VIRTUAL_CHAR:
		StringCbCopy( szName, sizeof( szName ), "CTPK_CREATE_VIRTUAL_CHAR" );
		break;
	case CTPK_PASSAGE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_PASSAGE" );
		break;
	case CTPK_PLAZA_COMMAND:
		StringCbCopy( szName, sizeof( szName ), "CTPK_PLAZA_COMMAND" );
		break;
	case CTPK_PLAZA_USER_INVITE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_PLAZA_USER_INVITE" );
		break;
	case CTPK_CATCH_CHAR:
		StringCbCopy( szName, sizeof( szName ), "CTPK_CATCH_CHAR" );
		break;
	case CTPK_ESCAPE_CATCH_CHAR:
		StringCbCopy( szName, sizeof( szName ), "CTPK_ESCAPE_CATCH_CHAR" );
		break;
	case CTPK_MOVE_SLOT_ITEM:
		StringCbCopy( szName, sizeof( szName ), "CTPK_MOVE_SLOT_ITEM" );
		break;
	case CTPK_ABSORB_REQUEST:
		StringCbCopy( szName, sizeof( szName ), "CTPK_ABSORB_REQUEST" );
		break;
	case CTPK_CHAT_MODE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_CHAT_MODE" );
		break;
	case CTPK_TUTORIAL_STEP:
		StringCbCopy( szName, sizeof( szName ), "CTPK_TUTORIAL_STEP" );
		break;
	case CTPK_CHAR_LIMIT_CHECK:
		StringCbCopy( szName, sizeof( szName ), "CTPK_CHAR_LIMIT_CHECK" );
		break;
	case CTPK_CHAR_DECORATION_BUY:
		StringCbCopy( szName, sizeof( szName ), "CTPK_CHAR_DECORATION_BUY" );
		break;
	case CTPK_PLAYRECORD_INFO:
		StringCbCopy( szName, sizeof( szName ), "CTPK_PLAYRECORD_INFO" );
		break;
	case CTPK_LAST_PLAYRECORD_INFO:
		StringCbCopy( szName, sizeof( szName ), "CTPK_LAST_PLAYRECORD_INFO" );
		break;
	case CTPK_CHAR_EXTEND:
		StringCbCopy( szName, sizeof( szName ), "CTPK_CHAR_EXTEND" );
		break;
	case CTPK_CHANGE_SINGLE_CHAR:
		StringCbCopy( szName, sizeof( szName ), "CTPK_CHANGE_SINGLE_CHAR" );
		break;
	case CTPK_VOICE_INFO:
		StringCbCopy( szName, sizeof( szName ), "CTPK_VOICE_INFO" );
		break;
	case CTPK_SEARCH_PLAZA_ROOM:
		StringCbCopy( szName, sizeof( szName ), "CTPK_SEARCH_PLAZA_ROOM" );
		break;
	case CTPK_BATTLEROOM_INVITE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_BATTLEROOM_INVITE" );
		break;
	case CTPK_RESERVE_BATTLEROOM_DELETE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_RESERVE_BATTLEROOM_DELETE" );
		break;
	case CTPK_HACK_QUIZ:
		StringCbCopy( szName, sizeof( szName ), "CTPK_HACK_QUIZ" );
		break;
	case CTPK_RELAY_CHAT:
		StringCbCopy( szName, sizeof( szName ), "CTPK_RELAY_CHAT" );
		break;
	case CTPK_ABUSE_QUIZ_START:
		StringCbCopy( szName, sizeof( szName ), "CTPK_ABUSE_QUIZ_START" );
		break;
	case CTPK_ABUSE_QUIZ:
		StringCbCopy( szName, sizeof( szName ), "CTPK_ABUSE_QUIZ" );
		break;
	case CTPK_BATTLEROOM_JOIN_INFO:
		StringCbCopy( szName, sizeof( szName ), "CTPK_BATTLEROOM_JOIN_INFO" );
		break;
	case CTPK_AWARDING_INFO:
		StringCbCopy( szName, sizeof( szName ), "CTPK_AWARDING_INFO" );
		break;
	case CTPK_AWARDING_RESULT:
		StringCbCopy( szName, sizeof( szName ), "CTPK_AWARDING_RESULT" );
		break;
	case CTPK_FOLLOW_USER:
		StringCbCopy( szName, sizeof( szName ), "CTPK_FOLLOW_USER" );
		break;
	case CTPK_DEVELOPER_MACRO:
		StringCbCopy( szName, sizeof( szName ), "CTPK_DEVELOPER_MACRO" );
		break;
	case CTPK_REQUEST_REVIVAL_TIME:
		StringCbCopy( szName, sizeof( szName ), "CTPK_REQUEST_REVIVAL_TIME" );
		break;
	case CTPK_ETCITEM_BUY:
		StringCbCopy( szName, sizeof( szName ), "CTPK_ETCITEM_BUY" );
		break;
	case CTPK_BATTLEROOM_COMMAND:
		StringCbCopy( szName, sizeof( szName ), "CTPK_BATTLEROOM_COMMAND" );
		break;
	case CTPK_MOVING_SERVER:
		StringCbCopy( szName, sizeof( szName ), "CTPK_MOVING_SERVER" );
		break;
	case CTPK_FRIEND_LIST_MSG:
		StringCbCopy( szName, sizeof( szName ), "CTPK_FRIEND_LIST_MSG" );
		break;
	case CTPK_FRIEND_COMMAND:
		StringCbCopy( szName, sizeof( szName ), "CTPK_FRIEND_COMMAND" );
		break;
	case CTPK_FRIEND_DELETE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_FRIEND_DELETE" );
		break;
	case CTPK_DELETE_FRIEND_BY_WEB:
		StringCbCopy( szName, sizeof( szName ), "CTPK_DELETE_FRIEND_BY_WEB" );
		break;
	case CTPK_USER_LOGIN:
		StringCbCopy( szName, sizeof( szName ), "CTPK_USER_LOGIN" );
		break;
	case CTPK_REGISTERED_USER:
		StringCbCopy( szName, sizeof( szName ), "CTPK_REGISTERED_USER" );
		break;
	case CTPK_USER_POS_REFRESH:
		StringCbCopy( szName, sizeof( szName ), "CTPK_USER_POS_REFRESH" );
		break;
	case CTPK_PLAZA_INVITE_LIST:
		StringCbCopy( szName, sizeof( szName ), "CTPK_PLAZA_INVITE_LIST" );
		break;
	case CTPK_CHANNEL_INVITE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_CHANNEL_INVITE" );
		break;
	case CTPK_CHANNEL_LEAVE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_CHANNEL_LEAVE" );
		break;
	case CTPK_CHANNEL_CHAT:
		StringCbCopy( szName, sizeof( szName ), "CTPK_CHANNEL_CHAT" );
		break;
	case CTPK_CHANNEL_CREATE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_CHANNEL_CREATE" );
		break;
	case CTPK_BATTLEROOM_INVITE_LIST:
		StringCbCopy( szName, sizeof( szName ), "CTPK_BATTLEROOM_INVITE_LIST" );
		break;
	case CTPK_EXERCISE_EVENT_CHAR_CREATE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_EXERCISE_EVENT_CHAR_CREATE" );
		break;
	case CTPK_USER_INFO_REFRESH:
		StringCbCopy( szName, sizeof( szName ), "CTPK_USER_INFO_REFRESH" );
		break;
	case CTPK_MEMO_SEND_MSG:
		StringCbCopy( szName, sizeof( szName ), "CTPK_MEMO_SEND_MSG" );
		break;
	case CTPK_OFFLINE_MEMO_MSG:
		StringCbCopy( szName, sizeof( szName ), "CTPK_OFFLINE_MEMO_MSG" );
		break;
	case CTPK_BANKRUPTCY_PESO:
		StringCbCopy( szName, sizeof( szName ), "CTPK_BANKRUPTCY_PESO" );
		break;
	case CTPK_SERVICE_CHAR:
		StringCbCopy( szName, sizeof( szName ), "CTPK_SERVICE_CHAR" );
		break;
	case CTPK_USER_POS_INDEX:
		StringCbCopy( szName, sizeof( szName ), "CTPK_USER_POS_INDEX" );
		break;
	case CTPK_CHAR_CHARGE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_CHAR_CHARGE" );
		break;
	case CTPK_GUILD_RANK_LIST:
		StringCbCopy( szName, sizeof( szName ), "CTPK_GUILD_RANK_LIST" );
		break;
	case CTPK_GUILD_INFO:
		StringCbCopy( szName, sizeof( szName ), "CTPK_GUILD_INFO" );
		break;
	case CTPK_GUILD_USER_LIST:
		StringCbCopy( szName, sizeof( szName ), "CTPK_GUILD_USER_LIST" );
		break;
	case CTPK_GUILD_JOINER_CHANGE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_GUILD_JOINER_CHANGE" );
		break;
	case CTPK_GUILD_ENTRY_APP:
		StringCbCopy( szName, sizeof( szName ), "CTPK_GUILD_ENTRY_APP" );
		break;
	case CTPK_GUILD_ENTRY_CANCEL:
		StringCbCopy( szName, sizeof( szName ), "CTPK_GUILD_ENTRY_CANCEL" );
		break;
	case CTPK_GUILD_ENTRY_DELAY_MEMBER:
		StringCbCopy( szName, sizeof( szName ), "CTPK_GUILD_ENTRY_DELAY_MEMBER" );
		break;
	case CTPK_GUILD_ENTRY_AGREE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_GUILD_ENTRY_AGREE" );
		break;
	case CTPK_GUILD_ENTRY_REFUSE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_GUILD_ENTRY_REFUSE" );
		break;
	case CTPK_GUILD_INVITATION:
		StringCbCopy( szName, sizeof( szName ), "CTPK_GUILD_INVITATION" );
		break;
	case CTPK_GUILD_LEAVE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_GUILD_LEAVE" );
		break;
	case CTPK_GUILD_TITLE_CHANGE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_GUILD_TITLE_CHANGE" );
		break;
	case CTPK_GUILD_MASTER_CHANGE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_GUILD_MASTER_CHANGE" );
		break;
	case CTPK_GUILD_POSITION_CHANGE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_GUILD_POSITION_CHANGE" );
		break;
	case CTPK_GUILD_KICK_OUT:
		StringCbCopy( szName, sizeof( szName ), "CTPK_GUILD_KICK_OUT" );
		break;
	case CTPK_FRIEND_REQUEST_LIST:
		StringCbCopy( szName, sizeof( szName ), "CTPK_FRIEND_REQUEST_LIST" );
		break;
	case CTPK_FRIEND_APPLICATION:
		StringCbCopy( szName, sizeof( szName ), "CTPK_FRIEND_APPLICATION" );
		break;
	case CTPK_PLAZA_JOIN_INFO:
		StringCbCopy( szName, sizeof( szName ), "CTPK_PLAZA_JOIN_INFO" );
		break;
	case CTPK_USER_ENTRY_REFRESH:
		StringCbCopy( szName, sizeof( szName ), "CTPK_USER_ENTRY_REFRESH" );
		break;
	case CTPK_GUILD_CHAT:
		StringCbCopy( szName, sizeof( szName ), "CTPK_GUILD_CHAT" );
		break;
	case CTPK_GUILD_EXIST:
		StringCbCopy( szName, sizeof( szName ), "CTPK_GUILD_EXIST" );
		break;
	case CTPK_USER_INFO_EXIST:
		StringCbCopy( szName, sizeof( szName ), "CTPK_USER_INFO_EXIST" );
		break;
	case CTPK_GUILD_MARK_KEY_VALUE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_GUILD_MARK_KEY_VALUE" );
		break;
	case CTPK_VOICE_INFO_USER:
		StringCbCopy( szName, sizeof( szName ), "CTPK_VOICE_INFO_USER" );
		break;
	case CTPK_LADDER_TEAM_LIST:
		StringCbCopy( szName, sizeof( szName ), "CTPK_LADDER_TEAM_LIST" );
		break;
	case CTPK_CREATE_LADDERTEAM:
		StringCbCopy( szName, sizeof( szName ), "CTPK_CREATE_LADDERTEAM" );
		break;
	case CTPK_JOIN_LADDERTEAM:
		StringCbCopy( szName, sizeof( szName ), "CTPK_JOIN_LADDERTEAM" );
		break;
	case CTPK_LADDERTEAM_MACRO:
		StringCbCopy( szName, sizeof( szName ), "CTPK_LADDERTEAM_MACRO" );
		break;
	case CTPK_LADDERTEAM_JOIN_INFO:
		StringCbCopy( szName, sizeof( szName ), "CTPK_LADDERTEAM_JOIN_INFO" );
		break;
	case CTPK_LADDERTEAM_LEAVE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_LADDERTEAM_LEAVE" );
		break;
	case CTPK_LADDERTEAM_INVITE_LIST:
		StringCbCopy( szName, sizeof( szName ), "CTPK_LADDERTEAM_INVITE_LIST" );
		break;
	case CTPK_LADDERTEAM_INVITE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_LADDERTEAM_INVITE" );
		break;
	case CTPK_LADDER_BATTLE_RESTART:
		StringCbCopy( szName, sizeof( szName ), "CTPK_LADDER_BATTLE_RESTART" );
		break;
	case CTPK_LADDER_OTHER_NAME_CHANGE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_LADDER_OTHER_NAME_CHANGE" );
		break;
	case CTPK_LADDER_BATTLE_REQUEST_AGREE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_LADDER_BATTLE_REQUEST_AGREE" );
		break;
	case CTPK_LADDER_USER_HQ_MOVE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_LADDER_USER_HQ_MOVE" );
		break;
	case CTPK_GUILD_MARK_KEY_VALUE_DELETE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_GUILD_MARK_KEY_VALUE_DELETE" );
		break;
	case CTPK_ETCITEM_USE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_ETCITEM_USE" );
		break;
	case CTPK_MOVIE_CONTROL:
		StringCbCopy( szName, sizeof( szName ), "CTPK_MOVIE_CONTROL" );
		break;
	case CTPK_PRESENT_REQUEST:
		StringCbCopy( szName, sizeof( szName ), "CTPK_PRESENT_REQUEST" );
		break;
	case CTPK_GUILD_TITLE_SYNC:
		StringCbCopy( szName, sizeof( szName ), "CTPK_GUILD_TITLE_SYNC" );
		break;
	case CTPK_PROTECT_CHECK:
		StringCbCopy( szName, sizeof( szName ), "CTPK_PROTECT_CHECK" );
		break;
	case CTPK_GET_CASH:
		StringCbCopy( szName, sizeof( szName ), "CTPK_GET_CASH" );
		break;
	case CTPK_HOLE_SEND_COMPLETE:
		StringCbCopy( szName, sizeof( szName ), "CTPK_HOLE_SEND_COMPLETE" );
		break;
	case CTPK_UDP_RECV_TIMEOUT:
		StringCbCopy( szName, sizeof( szName ), "CTPK_UDP_RECV_TIMEOUT" );
		break;
	case CTPK_JOIN_SERVER_LOBBY_INFO:
		StringCbCopy( szName, sizeof( szName ), "CTPK_JOIN_SERVER_LOBBY_INFO" );
		break;
	case CTPK_SERVER_LOBBY_CHAT:
		StringCbCopy( szName, sizeof( szName ), "CTPK_SERVER_LOBBY_CHAT" );
		break;
	case CTPK_FISHING:
		StringCbCopy( szName, sizeof( szName ), "CTPK_FISHING" );
		break;
	case CTPK_SIMPLE_USER_INFO_REFRESH:
		StringCbCopy( szName, sizeof( szName ), "CTPK_SIMPLE_USER_INFO_REFRESH" );
		break;
	case STPK_RESERVE_LOGOUT:
		StringCbCopy( szName, sizeof( szName ), "STPK_RESERVE_LOGOUT" );
		break;
	case STPK_CONNECT:
		StringCbCopy( szName, sizeof( szName ), "STPK_CONNECT" );
		break;
	case STPK_CHAR_CREATE:
		StringCbCopy( szName, sizeof( szName ), "STPK_CHAR_CREATE" );
		break;
	case STPK_LOGOUT:
		StringCbCopy( szName, sizeof( szName ), "STPK_LOGOUT" );
		break;
	case STPK_CHAR_DELETE:
		StringCbCopy( szName, sizeof( szName ), "STPK_CHAR_DELETE" );
		break;
	case STPK_CREATE_ROOM:
		StringCbCopy( szName, sizeof( szName ), "STPK_CREATE_ROOM" );
		break;
	case STPK_JOIN_ROOM:
		StringCbCopy( szName, sizeof( szName ), "STPK_JOIN_ROOM" );
		break;
	case STPK_JOIN_ROOMDATA:
		StringCbCopy( szName, sizeof( szName ), "STPK_JOIN_ROOMDATA" );
		break;
	case STPK_JOIN_USERDATA:
		StringCbCopy( szName, sizeof( szName ), "STPK_JOIN_USERDATA" );
		break;
	case STPK_LEAVE_USERDATA:
		StringCbCopy( szName, sizeof( szName ), "STPK_LEAVE_USERDATA" );
		break;
	case STPK_EXIT_ROOM:
		StringCbCopy( szName, sizeof( szName ), "STPK_EXIT_ROOM" );
		break;
	case STPK_HOST_USERID:
		StringCbCopy( szName, sizeof( szName ), "STPK_HOST_USERID" );
		break;
	case STPK_CHANGE_UDP_INFO:
		StringCbCopy( szName, sizeof( szName ), "STPK_CHANGE_UDP_INFO" );
		break;
	case STPK_DBCHAR_EQUIPDATA:
		StringCbCopy( szName, sizeof( szName ), "STPK_DBCHAR_EQUIPDATA" );
		break;
	case STPK_CHAR_EQUIPDATA:
		StringCbCopy( szName, sizeof( szName ), "STPK_CHAR_EQUIPDATA" );
		break;
	case STPK_PICK_ITEM_FAIL:
		StringCbCopy( szName, sizeof( szName ), "STPK_PICK_ITEM_FAIL" );
		break;
	case STPK_FIELD_ITEM_LIST:
		StringCbCopy( szName, sizeof( szName ), "STPK_FIELD_ITEM_LIST" );
		break;
	case STPK_ZERO_HP:
		StringCbCopy( szName, sizeof( szName ), "STPK_ZERO_HP" );
		break;
	case STPK_USER_REVIVAL:
		StringCbCopy( szName, sizeof( szName ), "STPK_USER_REVIVAL" );
		break;
	case STPK_ROUND_READY:
		StringCbCopy( szName, sizeof( szName ), "STPK_ROUND_READY" );
		break;
	case STPK_ROUND_START:
		StringCbCopy( szName, sizeof( szName ), "STPK_ROUND_START" );
		break;
	case STPK_ROUND_END:
		StringCbCopy( szName, sizeof( szName ), "STPK_ROUND_END" );
		break;
	case STPK_FIELD_ITEM_SUPPLY:
		StringCbCopy( szName, sizeof( szName ), "STPK_FIELD_ITEM_SUPPLY" );
		break;
	case STPK_DROP_ITEM:
		StringCbCopy( szName, sizeof( szName ), "STPK_DROP_ITEM" );
		break;
	case STPK_PICK_ITEM:
		StringCbCopy( szName, sizeof( szName ), "STPK_PICK_ITEM" );
		break;
	case STPK_DROP_DIE:
		StringCbCopy( szName, sizeof( szName ), "STPK_DROP_DIE" );
		break;
	case STPK_WEAPON_DIE:
		StringCbCopy( szName, sizeof( szName ), "STPK_WEAPON_DIE" );
		break;
	case STPK_SYMBOL_DIE:
		StringCbCopy( szName, sizeof( szName ), "STPK_SYMBOL_DIE" );
		break;
	case STPK_PLAZA_ROOM_LIST:
		StringCbCopy( szName, sizeof( szName ), "STPK_PLAZA_ROOM_LIST" );
		break;
	case STPK_SYMBOL_ACTIVITY:
		StringCbCopy( szName, sizeof( szName ), "STPK_SYMBOL_ACTIVITY" );
		break;
	case STPK_CREATE_PLAZA:
		StringCbCopy( szName, sizeof( szName ), "STPK_CREATE_PLAZA" );
		break;
	case STPK_PRISONERMODE:
		StringCbCopy( szName, sizeof( szName ), "STPK_PRISONERMODE" );
		break;
	case STPK_PRISONER_ESCAPE:
		StringCbCopy( szName, sizeof( szName ), "STPK_PRISONER_ESCAPE" );
		break;
	case STPK_SLOT_ITEM:
		StringCbCopy( szName, sizeof( szName ), "STPK_SLOT_ITEM" );
		break;
	case STPK_CHANGE_CHAR:
		StringCbCopy( szName, sizeof( szName ), "STPK_CHANGE_CHAR" );
		break;
	case STPK_START_SELECT_CHAR:
		StringCbCopy( szName, sizeof( szName ), "STPK_START_SELECT_CHAR" );
		break;
	case STPK_PUSHSTRUCT_DIE:
		StringCbCopy( szName, sizeof( szName ), "STPK_PUSHSTRUCT_DIE" );
		break;
	case STPK_PUSHSTRUCT_INFO:
		StringCbCopy( szName, sizeof( szName ), "STPK_PUSHSTRUCT_INFO" );
		break;
	case STPK_FINAL_ROUND_RESULT:
		StringCbCopy( szName, sizeof( szName ), "STPK_FINAL_ROUND_RESULT" );
		break;
	case STPK_JOIN_ROOM_INFO_END:
		StringCbCopy( szName, sizeof( szName ), "STPK_JOIN_ROOM_INFO_END" );
		break;
	case STPK_MODE_SCORE:
		StringCbCopy( szName, sizeof( szName ), "STPK_MODE_SCORE" );
		break;
	case STPK_KING_TAKE:
		StringCbCopy( szName, sizeof( szName ), "STPK_KING_TAKE" );
		break;
	case STPK_CHANGE_PERIOD:
		StringCbCopy( szName, sizeof( szName ), "STPK_CHANGE_PERIOD" );
		break;
	case STPK_JOIN_NEW_USERDATA:
		StringCbCopy( szName, sizeof( szName ), "STPK_JOIN_NEW_USERDATA" );
		break;
	case STPK_PRISONER_DROP:
		StringCbCopy( szName, sizeof( szName ), "STPK_PRISONER_DROP" );
		break;
	case STPK_BUY_ITEM:
		StringCbCopy( szName, sizeof( szName ), "STPK_BUY_ITEM" );
		break;
	case STPK_EQUIP_SLOT_ITEM:
		StringCbCopy( szName, sizeof( szName ), "STPK_EQUIP_SLOT_ITEM" );
		break;
	case STPK_SELL_SLOT_ITEM:
		StringCbCopy( szName, sizeof( szName ), "STPK_SELL_SLOT_ITEM" );
		break;
	case STPK_START_SET_NEXT_MODE:
		StringCbCopy( szName, sizeof( szName ), "STPK_START_SET_NEXT_MODE" );
		break;
	case STPK_END_SET_NEXT_MODE:
		StringCbCopy( szName, sizeof( szName ), "STPK_END_SET_NEXT_MODE" );
		break;
	case STPK_INC_STAT:
		StringCbCopy( szName, sizeof( szName ), "STPK_INC_STAT" );
		break;
	case STPK_INIT_STAT:
		StringCbCopy( szName, sizeof( szName ), "STPK_INIT_STAT" );
		break;
	case STPK_MODE_INFO:
		StringCbCopy( szName, sizeof( szName ), "STPK_MODE_INFO" );
		break;
	case STPK_CHAR_LOAD:
		StringCbCopy( szName, sizeof( szName ), "STPK_CHAR_LOAD" );
		break;
	case STPK_CHAR_ALL_LOAD:
		StringCbCopy( szName, sizeof( szName ), "STPK_CHAR_ALL_LOAD" );
		break;
	case STPK_ROUND_TIME_STOP:
		StringCbCopy( szName, sizeof( szName ), "STPK_ROUND_TIME_STOP" );
		break;
	case STPK_SELL_EQUIP_ITEM:
		StringCbCopy( szName, sizeof( szName ), "STPK_SELL_EQUIP_ITEM" );
		break;
	case STPK_ROUND_JOIN_OBSERVER:
		StringCbCopy( szName, sizeof( szName ), "STPK_ROUND_JOIN_OBSERVER" );
		break;
	case STPK_ROUND_JOIN:
		StringCbCopy( szName, sizeof( szName ), "STPK_ROUND_JOIN" );
		break;
	case STPK_ROUND_JOIN_VIEW:
		StringCbCopy( szName, sizeof( szName ), "STPK_ROUND_JOIN_VIEW" );
		break;
	case STPK_LEVELUP_ITEM:
		StringCbCopy( szName, sizeof( szName ), "STPK_LEVELUP_ITEM" );
		break;
	case STPK_REPOSITION_FIELDITEM:
		StringCbCopy( szName, sizeof( szName ), "STPK_REPOSITION_FIELDITEM" );
		break;
	case STPK_OBJECT_SUPPLY:
		StringCbCopy( szName, sizeof( szName ), "STPK_OBJECT_SUPPLY" );
		break;
	case STPK_FIELD_ITEM_DELETE:
		StringCbCopy( szName, sizeof( szName ), "STPK_FIELD_ITEM_DELETE" );
		break;
	case STPK_BATTLEROOM_LIST:
		StringCbCopy( szName, sizeof( szName ), "STPK_BATTLEROOM_LIST" );
		break;
	case STPK_ABSORB_APPLY:
		StringCbCopy( szName, sizeof( szName ), "STPK_ABSORB_APPLY" );
		break;
	case STPK_CREATE_BATTLEROOM:
		StringCbCopy( szName, sizeof( szName ), "STPK_CREATE_BATTLEROOM" );
		break;
	case STPK_USER_BATTLEROOM_JOIN:
		StringCbCopy( szName, sizeof( szName ), "STPK_USER_BATTLEROOM_JOIN" );
		break;
	case STPK_BATTLEROOM_INFO:
		StringCbCopy( szName, sizeof( szName ), "STPK_BATTLEROOM_INFO" );
		break;
	case STPK_BATTLEROOM_USER_INFO:
		StringCbCopy( szName, sizeof( szName ), "STPK_BATTLEROOM_USER_INFO" );
		break;
	case STPK_BATTLEROOM_LEAVE:
		StringCbCopy( szName, sizeof( szName ), "STPK_BATTLEROOM_LEAVE" );
		break;
	case STPK_DROP_ITEM_FAIL:
		StringCbCopy( szName, sizeof( szName ), "STPK_DROP_ITEM_FAIL" );
		break;
	case STPK_MACRO_COMMAND:
		StringCbCopy( szName, sizeof( szName ), "STPK_MACRO_COMMAND" );
		break;
	case STPK_BATTLE_ROOM_END:
		StringCbCopy( szName, sizeof( szName ), "STPK_BATTLE_ROOM_END" );
		break;
	case STPK_CHAR_SLOT_CHANGE:
		StringCbCopy( szName, sizeof( szName ), "STPK_CHAR_SLOT_CHANGE" );
		break;
	case STPK_PASSAGE:
		StringCbCopy( szName, sizeof( szName ), "STPK_PASSAGE" );
		break;
	case STPK_PLAZA_COMMAND:
		StringCbCopy( szName, sizeof( szName ), "STPK_PLAZA_COMMAND" );
		break;
	case STPK_CATCH_CHAR:
		StringCbCopy( szName, sizeof( szName ), "STPK_CATCH_CHAR" );
		break;
	case STPK_ESCAPE_CATCH_CHAR:
		StringCbCopy( szName, sizeof( szName ), "STPK_ESCAPE_CATCH_CHAR" );
		break;
	case STPK_MOVE_SLOT_ITEM:
		StringCbCopy( szName, sizeof( szName ), "STPK_MOVE_SLOT_ITEM" );
		break;
	case STPK_PLAYING_USER_DATA:
		StringCbCopy( szName, sizeof( szName ), "STPK_PLAYING_USER_DATA" );
		break;
	case STPK_TUTORIAL_STEP:
		StringCbCopy( szName, sizeof( szName ), "STPK_TUTORIAL_STEP" );
		break;
	case STPK_MOVING_ROOM:
		StringCbCopy( szName, sizeof( szName ), "STPK_MOVING_ROOM" );
		break;
	case STPK_CLASSEXPERT_SET:
		StringCbCopy( szName, sizeof( szName ), "STPK_CLASSEXPERT_SET" );
		break;
	case STPK_CLASSPRICE:
		StringCbCopy( szName, sizeof( szName ), "STPK_CLASSPRICE" );
		break;
	case STPK_CHAR_LIMIT_CHECK:
		StringCbCopy( szName, sizeof( szName ), "STPK_CHAR_LIMIT_CHECK" );
		break;
	case STPK_CHAR_DECORATION_BUY:
		StringCbCopy( szName, sizeof( szName ), "STPK_CHAR_DECORATION_BUY" );
		break;
	case STPK_PLAYRECORD_INFO:
		StringCbCopy( szName, sizeof( szName ), "STPK_PLAYRECORD_INFO" );
		break;
	case STPK_CHAR_EXTEND:
		StringCbCopy( szName, sizeof( szName ), "STPK_CHAR_EXTEND" );
		break;
	case STPK_CHANGE_SINGLE_CHAR:
		StringCbCopy( szName, sizeof( szName ), "STPK_CHANGE_SINGLE_CHAR" );
		break;
	case STPK_ANNOUNCE:
		StringCbCopy( szName, sizeof( szName ), "STPK_ANNOUNCE" );
		break;
	case STPK_VOICE_INFO:
		StringCbCopy( szName, sizeof( szName ), "STPK_VOICE_INFO" );
		break;
	case STPK_CHANGE_STAGE:
		StringCbCopy( szName, sizeof( szName ), "STPK_CHANGE_STAGE" );
		break;
	case STPK_BATTLEROOM_INVITE:
		StringCbCopy( szName, sizeof( szName ), "STPK_BATTLEROOM_INVITE" );
		break;
	case STPK_PING:
		StringCbCopy( szName, sizeof( szName ), "STPK_PING" );
		break;
	case STPK_HACK_ANNOUNCE:
		StringCbCopy( szName, sizeof( szName ), "STPK_HACK_ANNOUNCE" );
		break;
	case STPK_CHAT_MODE:
		StringCbCopy( szName, sizeof( szName ), "STPK_CHAT_MODE" );
		break;
	case STPK_HACK_QUIZ:
		StringCbCopy( szName, sizeof( szName ), "STPK_HACK_QUIZ" );
		break;
	case STPK_RELAY_CHAT:
		StringCbCopy( szName, sizeof( szName ), "STPK_RELAY_CHAT" );
		break;
	case STPK_ABUSE_ANNOUNCE:
		StringCbCopy( szName, sizeof( szName ), "STPK_ABUSE_ANNOUNCE" );
		break;
	case STPK_ABUSE_QUIZ:
		StringCbCopy( szName, sizeof( szName ), "STPK_ABUSE_QUIZ" );
		break;
	case STPK_BATTLEROOM_JOIN_INFO:
		StringCbCopy( szName, sizeof( szName ), "STPK_BATTLEROOM_JOIN_INFO" );
		break;
	case STPK_BATTLEROOM_INVITE_RESULT:
		StringCbCopy( szName, sizeof( szName ), "STPK_BATTLEROOM_INVITE_RESULT" );
		break;
	case STPK_EXIT_ROOM_PENALTY:
		StringCbCopy( szName, sizeof( szName ), "STPK_EXIT_ROOM_PENALTY" );
		break;
	case STPK_AWARDING_INFO:
		StringCbCopy( szName, sizeof( szName ), "STPK_AWARDING_INFO" );
		break;
	case STPK_AWARDING_RESULT:
		StringCbCopy( szName, sizeof( szName ), "STPK_AWARDING_RESULT" );
		break;
	case STPK_FOLLOW_USER:
		StringCbCopy( szName, sizeof( szName ), "STPK_FOLLOW_USER" );
		break;
	case STPK_AWARD_DATA:
		StringCbCopy( szName, sizeof( szName ), "STPK_AWARD_DATA" );
		break;
	case STPK_DEVELOPER_MACRO:
		StringCbCopy( szName, sizeof( szName ), "STPK_DEVELOPER_MACRO" );
		break;
	case STPK_UPDATE_SCORE_GAUGE:
		StringCbCopy( szName, sizeof( szName ), "STPK_UPDATE_SCORE_GAUGE" );
		break;
	case STPK_ANSWER_REVIVAL_TIME:
		StringCbCopy( szName, sizeof( szName ), "STPK_ANSWER_REVIVAL_TIME" );
		break;
	case STPK_REQUEST_BANKRUPT:
		StringCbCopy( szName, sizeof( szName ), "STPK_REQUEST_BANKRUPT" );
		break;
	case STPK_ETCITEM_BUY:
		StringCbCopy( szName, sizeof( szName ), "STPK_ETCITEM_BUY" );
		break;
	case STPK_GRADE_CLASS_UP_BONUS:
		StringCbCopy( szName, sizeof( szName ), "STPK_GRADE_CLASS_UP_BONUS" );
		break;
	case STPK_BATTLEROOM_COMMAND:
		StringCbCopy( szName, sizeof( szName ), "STPK_BATTLEROOM_COMMAND" );
		break;
	case STPK_SEARCH_PLAZA_ROOM:
		StringCbCopy( szName, sizeof( szName ), "STPK_SEARCH_PLAZA_ROOM" );
		break;
	case STPK_PLAZA_USER_INVITE:
		StringCbCopy( szName, sizeof( szName ), "STPK_PLAZA_USER_INVITE" );
		break;
	case STPK_MOVING_SERVER:
		StringCbCopy( szName, sizeof( szName ), "STPK_MOVING_SERVER" );
		break;
	case STPK_MOVING_SERVER_COMPLETE:
		StringCbCopy( szName, sizeof( szName ), "STPK_MOVING_SERVER_COMPLETE" );
		break;
	case STPK_RESERVE_ROOM_JOIN:
		StringCbCopy( szName, sizeof( szName ), "STPK_RESERVE_ROOM_JOIN" );
		break;
	case STPK_FRIEND_LIST_MSG:
		StringCbCopy( szName, sizeof( szName ), "STPK_FRIEND_LIST_MSG" );
		break;
	case STPK_FRIEND_COMMAND:
		StringCbCopy( szName, sizeof( szName ), "STPK_FRIEND_COMMAND" );
		break;
	case STPK_FRIEND_DELETE:
		StringCbCopy( szName, sizeof( szName ), "STPK_FRIEND_DELETE" );
		break;
	case STPK_USER_LOGIN:
		StringCbCopy( szName, sizeof( szName ), "STPK_USER_LOGIN" );
		break;
	case STPK_REGISTERED_USER:
		StringCbCopy( szName, sizeof( szName ), "STPK_REGISTERED_USER" );
		break;
	case STPK_USER_POS_REFRESH:
		StringCbCopy( szName, sizeof( szName ), "STPK_USER_POS_REFRESH" );
		break;
	case STPK_PLAZA_INVITE_LIST:
		StringCbCopy( szName, sizeof( szName ), "STPK_PLAZA_INVITE_LIST" );
		break;
	case STPK_CHANNEL_INVITE:
		StringCbCopy( szName, sizeof( szName ), "STPK_CHANNEL_INVITE" );
		break;
	case STPK_CHANNEL_JOIN:
		StringCbCopy( szName, sizeof( szName ), "STPK_CHANNEL_JOIN" );
		break;
	case STPK_CHANNEL_LEAVE:
		StringCbCopy( szName, sizeof( szName ), "STPK_CHANNEL_LEAVE" );
		break;
	case STPK_CHANNEL_CHAT:
		StringCbCopy( szName, sizeof( szName ), "STPK_CHANNEL_CHAT" );
		break;
	case STPK_CHANNEL_ALL_USER:
		StringCbCopy( szName, sizeof( szName ), "STPK_CHANNEL_ALL_USER" );
		break;
	case STPK_CHANNEL_CREATE:
		StringCbCopy( szName, sizeof( szName ), "STPK_CHANNEL_CREATE" );
		break;
	case STPK_BATTLEROOM_INVITE_LIST:
		StringCbCopy( szName, sizeof( szName ), "STPK_BATTLEROOM_INVITE_LIST" );
		break;
	case STPK_EXERCISE_EVENT_CHAR_CREATE:
		StringCbCopy( szName, sizeof( szName ), "STPK_EXERCISE_EVENT_CHAR_CREATE" );
		break;
	case STPK_EXERCISE_CHAR_DELETE:
		StringCbCopy( szName, sizeof( szName ), "STPK_EXERCISE_CHAR_DELETE" );
		break;
	case STPK_USER_INFO_REFRESH:
		StringCbCopy( szName, sizeof( szName ), "STPK_USER_INFO_REFRESH" );
		break;
	case STPK_MEMO_SEND_MSG:
		StringCbCopy( szName, sizeof( szName ), "STPK_MEMO_SEND_MSG" );
		break;
	case STPK_OFFLINE_MEMO_MSG:
		StringCbCopy( szName, sizeof( szName ), "STPK_OFFLINE_MEMO_MSG" );
		break;
	case STPK_BANKRUPTCY_PESO:
		StringCbCopy( szName, sizeof( szName ), "STPK_BANKRUPTCY_PESO" );
		break;
	case STPK_USER_POS_INDEX:
		StringCbCopy( szName, sizeof( szName ), "STPK_USER_POS_INDEX" );
		break;
	case STPK_PUSHSTRUCT_CREATE:
		StringCbCopy( szName, sizeof( szName ), "STPK_PUSHSTRUCT_CREATE" );
		break;
	case STPK_BATTLEROOM_ENTERUSER_SYNC:
		StringCbCopy( szName, sizeof( szName ), "STPK_BATTLEROOM_ENTERUSER_SYNC" );
		break;
	case STPK_RESERVE_SERVER_MOVE:
		StringCbCopy( szName, sizeof( szName ), "STPK_RESERVE_SERVER_MOVE" );
		break;
	case STPK_FINAL_RESULT_USER_INFO:
		StringCbCopy( szName, sizeof( szName ), "STPK_FINAL_RESULT_USER_INFO" );
		break;
	case STPK_TOTAL_REG_USER_CNT:
		StringCbCopy( szName, sizeof( szName ), "STPK_TOTAL_REG_USER_CNT" );
		break;
	case STPK_START_CHAR_LIMITDATE:
		StringCbCopy( szName, sizeof( szName ), "STPK_START_CHAR_LIMITDATE" );
		break;
	case STPK_UPDATE_CHAR_LIMITDATE:
		StringCbCopy( szName, sizeof( szName ), "STPK_UPDATE_CHAR_LIMITDATE" );
		break;
	case STPK_CHAR_CHARGE:
		StringCbCopy( szName, sizeof( szName ), "STPK_CHAR_CHARGE" );
		break;
	case STPK_EVENT_DATA:
		StringCbCopy( szName, sizeof( szName ), "STPK_EVENT_DATA" );
		break;
	case STPK_USER_RECORD_DATA:
		StringCbCopy( szName, sizeof( szName ), "STPK_USER_RECORD_DATA" );
		break;
	case STPK_CREATE_GUILD:
		StringCbCopy( szName, sizeof( szName ), "STPK_CREATE_GUILD" );
		break;
	case STPK_MY_GUILD_INFO:
		StringCbCopy( szName, sizeof( szName ), "STPK_MY_GUILD_INFO" );
		break;
	case STPK_ENTRY_DELAY_GUILD_LIST:
		StringCbCopy( szName, sizeof( szName ), "STPK_ENTRY_DELAY_GUILD_LIST" );
		break;
	case STPK_GUILD_ENTRY_DELAY_MEMBER:
		StringCbCopy( szName, sizeof( szName ), "STPK_GUILD_ENTRY_DELAY_MEMBER" );
		break;
	case STPK_GUILD_RANK_LIST:
		StringCbCopy( szName, sizeof( szName ), "STPK_GUILD_RANK_LIST" );
		break;
	case STPK_GUILD_INFO:
		StringCbCopy( szName, sizeof( szName ), "STPK_GUILD_INFO" );
		break;
	case STPK_GUILD_MEMBER_LIST:
		StringCbCopy( szName, sizeof( szName ), "STPK_GUILD_MEMBER_LIST" );
		break;
	case STPK_GUILD_JOINER_CHANGE:
		StringCbCopy( szName, sizeof( szName ), "STPK_GUILD_JOINER_CHANGE" );
		break;
	case STPK_GUILD_ENTRY_APP:
		StringCbCopy( szName, sizeof( szName ), "STPK_GUILD_ENTRY_APP" );
		break;
	case STPK_GUILD_ENTRY_AGREE:
		StringCbCopy( szName, sizeof( szName ), "STPK_GUILD_ENTRY_AGREE" );
		break;
	case STPK_ENTRY_DELAY_GUILD_DELETE:
		StringCbCopy( szName, sizeof( szName ), "STPK_ENTRY_DELAY_GUILD_DELETE" );
		break;
	case STPK_GUILD_INVITATION:
		StringCbCopy( szName, sizeof( szName ), "STPK_GUILD_INVITATION" );
		break;
	case STPK_GUILD_INVITED:
		StringCbCopy( szName, sizeof( szName ), "STPK_GUILD_INVITED" );
		break;
	case STPK_GUILD_LEAVE:
		StringCbCopy( szName, sizeof( szName ), "STPK_GUILD_LEAVE" );
		break;
	case STPK_GUILD_MASTER_CHANGE:
		StringCbCopy( szName, sizeof( szName ), "STPK_GUILD_MASTER_CHANGE" );
		break;
	case STPK_GUILD_POSITION_CHANGE:
		StringCbCopy( szName, sizeof( szName ), "STPK_GUILD_POSITION_CHANGE" );
		break;
	case STPK_GUILD_KICK_OUT:
		StringCbCopy( szName, sizeof( szName ), "STPK_GUILD_KICK_OUT" );
		break;
	case STPK_GUILD_SIMPLE_DATA:
		StringCbCopy( szName, sizeof( szName ), "STPK_GUILD_SIMPLE_DATA" );
		break;
	case STPK_SYMBOL_DIE_LIST:
		StringCbCopy( szName, sizeof( szName ), "STPK_SYMBOL_DIE_LIST" );
		break;
	case STPK_FRIEND_REQUEST_LIST:
		StringCbCopy( szName, sizeof( szName ), "STPK_FRIEND_REQUEST_LIST" );
		break;
	case STPK_FRIEND_APPLICATION:
		StringCbCopy( szName, sizeof( szName ), "STPK_FRIEND_APPLICATION" );
		break;
	case STPK_PLAZA_JOIN_INFO:
		StringCbCopy( szName, sizeof( szName ), "STPK_PLAZA_JOIN_INFO" );
		break;
	case STPK_USER_ENTRY_REFRESH:
		StringCbCopy( szName, sizeof( szName ), "STPK_USER_ENTRY_REFRESH" );
		break;
	case STPK_GUILD_MARK_CHANGE:
		StringCbCopy( szName, sizeof( szName ), "STPK_GUILD_MARK_CHANGE" );
		break;
	case STPK_MYGUILD_MEMBER_LIST:
		StringCbCopy( szName, sizeof( szName ), "STPK_MYGUILD_MEMBER_LIST" );
		break;
	case STPK_MYGUILD_LEAVE_LIST:
		StringCbCopy( szName, sizeof( szName ), "STPK_MYGUILD_LEAVE_LIST" );
		break;
	case STPK_GUILD_CHAT:
		StringCbCopy( szName, sizeof( szName ), "STPK_GUILD_CHAT" );
		break;
	case STPK_GUILD_EXIST:
		StringCbCopy( szName, sizeof( szName ), "STPK_GUILD_EXIST" );
		break;
	case STPK_USER_INFO_EXIST:
		StringCbCopy( szName, sizeof( szName ), "STPK_USER_INFO_EXIST" );
		break;
	case STPK_GUILD_MARK_KEY_VALUE:
		StringCbCopy( szName, sizeof( szName ), "STPK_GUILD_MARK_KEY_VALUE" );
		break;
	case STPK_LADDER_TEAM_LIST:
		StringCbCopy( szName, sizeof( szName ), "STPK_LADDER_TEAM_LIST" );
		break;
	case STPK_CREATE_LADDERTEAM:
		StringCbCopy( szName, sizeof( szName ), "STPK_CREATE_LADDERTEAM" );
		break;
	case STPK_JOIN_LADDERTEAM:
		StringCbCopy( szName, sizeof( szName ), "STPK_JOIN_LADDERTEAM" );
		break;
	case STPK_LADDERTEAM_INFO:
		StringCbCopy( szName, sizeof( szName ), "STPK_LADDERTEAM_INFO" );
		break;
	case STPK_LADDERTEAM_USER_INFO:
		StringCbCopy( szName, sizeof( szName ), "STPK_LADDERTEAM_USER_INFO" );
		break;
	case STPK_LADDERTEAM_LEAVE:
		StringCbCopy( szName, sizeof( szName ), "STPK_LADDERTEAM_LEAVE" );
		break;
	case STPK_LADDERTEAM_MACRO:
		StringCbCopy( szName, sizeof( szName ), "STPK_LADDERTEAM_MACRO" );
		break;
	case STPK_LADDERTEAM_JOIN_INFO:
		StringCbCopy( szName, sizeof( szName ), "STPK_LADDERTEAM_JOIN_INFO" );
		break;
	case STPK_LADDERROOM_USER_INFO:
		StringCbCopy( szName, sizeof( szName ), "STPK_LADDERROOM_USER_INFO" );
		break;
	case STPK_LADDERTEAM_INVITE_LIST:
		StringCbCopy( szName, sizeof( szName ), "STPK_LADDERTEAM_INVITE_LIST" );
		break;
	case STPK_LADDERTEAM_INVITED:
		StringCbCopy( szName, sizeof( szName ), "STPK_LADDERTEAM_INVITED" );
		break;
	case STPK_LADDER_TEAM_RANKING:
		StringCbCopy( szName, sizeof( szName ), "STPK_LADDER_TEAM_RANKING" );
		break;
	case STPK_LADDER_OTHER_TEAM_INFO:
		StringCbCopy( szName, sizeof( szName ), "STPK_LADDER_OTHER_TEAM_INFO" );
		break;
	case STPK_LADDER_OTHER_NAME_CHANGE:
		StringCbCopy( szName, sizeof( szName ), "STPK_LADDER_OTHER_NAME_CHANGE" );
		break;
	case STPK_LADDER_BATTLE_REQUEST_AGREE:
		StringCbCopy( szName, sizeof( szName ), "STPK_LADDER_BATTLE_REQUEST_AGREE" );
		break;
	case STPK_IMMEDIATELY_EQUIP:
		StringCbCopy( szName, sizeof( szName ), "STPK_IMMEDIATELY_EQUIP" );
		break;
	case STPK_USER_ETC_ITEM:
		StringCbCopy( szName, sizeof( szName ), "STPK_USER_ETC_ITEM" );
		break;
	case STPK_ETCITEM_USE:
		StringCbCopy( szName, sizeof( szName ), "STPK_ETCITEM_USE" );
		break;
	case STPK_SERVICE_CHAR:
		StringCbCopy( szName, sizeof( szName ), "STPK_SERVICE_CHAR" );
		break;
	case STPK_MOVIE_CONTROL:
		StringCbCopy( szName, sizeof( szName ), "STPK_MOVIE_CONTROL" );
		break;
	case STPK_PRESENT_DATA:
		StringCbCopy( szName, sizeof( szName ), "STPK_PRESENT_DATA" );
		break;
	case STPK_DEFAULT_DECO_ITEM:
		StringCbCopy( szName, sizeof( szName ), "STPK_DEFAULT_DECO_ITEM" );
		break;
	case STPK_EVENT_DATA_UPDATE:
		StringCbCopy( szName, sizeof( szName ), "STPK_EVENT_DATA_UPDATE" );
		break;
	case STPK_GUILD_TITLE_SYNC:
		StringCbCopy( szName, sizeof( szName ), "STPK_GUILD_TITLE_SYNC" );
		break;
	case STPK_USER_BONUS_SYNC:
		StringCbCopy( szName, sizeof( szName ), "STPK_USER_BONUS_SYNC" );
		break;
	case STPK_PROTECT_CHECK:
		StringCbCopy( szName, sizeof( szName ), "STPK_PROTECT_CHECK" );
		break;
	case STPK_GET_CASH:
		StringCbCopy( szName, sizeof( szName ), "STPK_GET_CASH" );
		break;
	case STPK_HOLE_SEND_COMPLETE:
		StringCbCopy( szName, sizeof( szName ), "STPK_HOLE_SEND_COMPLETE" );
		break;
	case STPK_UDP_RECV_TIMEOUT:
		StringCbCopy( szName, sizeof( szName ), "STPK_UDP_RECV_TIMEOUT" );
		break;
	case STPK_JOIN_SERVER_LOBBY_INFO:
		StringCbCopy( szName, sizeof( szName ), "STPK_JOIN_SERVER_LOBBY_INFO" );
		break;
	case STPK_SERVER_LOBBY_CHAT:
		StringCbCopy( szName, sizeof( szName ), "STPK_SERVER_LOBBY_CHAT" );
		break;
	case STPK_GROWTH_LEVEL_FIRST:     
		StringCbCopy( szName, sizeof( szName ), "STPK_GROWTH_LEVEL_FIRST" );
		break;
	case STPK_GROWTH_LEVEL_UP:     
		StringCbCopy( szName, sizeof( szName ), "STPK_GROWTH_LEVEL_UP" );
		break;
	case STPK_GROWTH_LEVEL_DOWN:
		StringCbCopy( szName, sizeof( szName ), "STPK_GROWTH_LEVEL_DOWN" );
		break;
	case STPK_GROWTH_POINT_BONUS:     
		StringCbCopy( szName, sizeof( szName ), "STPK_GROWTH_POINT_BONUS" );
		break;
	case STPK_GROWTH_LEVEL_INIT:     
		StringCbCopy( szName, sizeof( szName ), "STPK_GROWTH_LEVEL_INIT" );
		break;
	case STPK_BAD_PING_CROWN_DROP:     
		StringCbCopy( szName, sizeof( szName ), "STPK_BAD_PING_CROWN_DROP" );
		break;
	case STPK_USER_EXTRAITEM_DATA:
		StringCbCopy( szName, sizeof( szName ), "STPK_USER_EXTRAITEM_DATA" );
		break;
	case STPK_EXTRAITEM_BUY:
		StringCbCopy( szName, sizeof( szName ), "STPK_EXTRAITEM_BUY" );
		break;
	case STPK_DELETE_EXTRAITEM_DATE:
		StringCbCopy( szName, sizeof( szName ), "STPK_DELETE_EXTRAITEM_DATE" );
		break;
	case STPK_BALLSTRUCT_INFO:
		StringCbCopy( szName, sizeof( szName ), "STPK_BALLSTRUCT_INFO" );
		break;
	case STPK_CONTROL_KEYS:
		StringCbCopy( szName, sizeof( szName ), "STPK_CONTROL_KEYS" );
		break;
	case STPK_PUSHSTRUCT_OWNER_CLEAR:
		StringCbCopy( szName, sizeof( szName ), "STPK_PUSHSTRUCT_OWNER_CLEAR" );
		break;
	case STPK_PUSHSTRUCT_INFO_ENTER:
		StringCbCopy( szName, sizeof( szName ), "STPK_PUSHSTRUCT_INFO_ENTER" );
		break;
	case STPK_SIMPLE_USER_INFO_REFRESH:
		StringCbCopy( szName, sizeof( szName ), "STPK_SIMPLE_USER_INFO_REFRESH" );
		break;
	case STPK_EXPAND_MEDAL_SLOT_DATA:
		StringCbCopy( szName, sizeof( szName ), "STPK_EXPAND_MEDAL_SLOT_DATA" );
		break;
	case STPK_EXTRAITEM_DISASSEMBLE:
		StringCbCopy( szName, sizeof( szName ), "STPK_EXTRAITEM_DISASSEMBLE" );
		break;
	case STPK_SOLDIER_DISASSEMBLE:
		StringCbCopy( szName, sizeof( szName ), "STPK_SOLDIERR_DISASSEMBLE" );
		break;
	case STPK_USER_COSTUME_DATA:
		StringCbCopy( szName, sizeof( szName ), "STPK_USER_COSTUME_DATA" );
		break;
	case STPK_COSTUME_BUY:
		StringCbCopy( szName, sizeof( szName ), "STPK_COSTUME_BUY" );
		break;
	case STPK_SEARCH_ROOM_RESLUT:
		StringCbCopy( szName, sizeof( szName ), "STPK_SEARCH_ROOM_RESLUT" );
		break;
	case STPK_LADDERROOM_LIST :
		StringCbCopy( szName, sizeof( szName ), "STPK_LADDERROOM_LIST" );
		break;
	case STPK_LADDERROOM_JOIN_INFO:
		StringCbCopy( szName, sizeof( szName ), "STPK_LADDERROOM_JOIN_INFO" );
		break;
	case STPK_LADDERROOM_INFO:
		StringCbCopy( szName, sizeof( szName ), "STPK_LADDERROOM_INFO" );
		break;
	// udp
	case CUPK_CONNECT:		
		StringCbCopy( szName, sizeof( szName ), "CUPK_CONNECT" );
		break;
	case CUPK_SYNCTIME:					
		StringCbCopy( szName, sizeof( szName ), "CUPK_SYNCTIME" );
		break;
	case CUPK_CHAT:						
		StringCbCopy( szName, sizeof( szName ), "CUPK_CHAT" );
		break;
	case CUPK_HOLE_CHECK:                
		StringCbCopy( szName, sizeof( szName ), "CUPK_HOLE_CHECK" );
		break;
	case CUPK_RESERVE_ROOM_JOIN:			
		StringCbCopy( szName, sizeof( szName ), "CUPK_RESERVE_ROOM_JOIN" );
		break;
	case CUPK_NEW_USER_INFO:				
		StringCbCopy( szName, sizeof( szName ), "CUPK_NEW_USER_INFO" );
		break;
	case CUPK_EXIST_USER_INFO:			
		StringCbCopy( szName, sizeof( szName ), "CUPK_EXIST_USER_INFO" );
		break;
	case CUPK_CHAR_CONTROL:					
		StringCbCopy( szName, sizeof( szName ), "CUPK_CHAR_CONTROL" );
		break;
	case CUPK_ATTACK:						
		StringCbCopy( szName, sizeof( szName ), "CUPK_ATTACK" );
		break;
	case CUPK_DEFENSE:						
		StringCbCopy( szName, sizeof( szName ), "CUPK_DEFENSE" );
		break;
	case CUPK_DEFENSE_RELEASE:				
		StringCbCopy( szName, sizeof( szName ), "CUPK_DEFENSE_RELEASE" );
		break;
	case CUPK_JUMP:							
		StringCbCopy( szName, sizeof( szName ), "CUPK_JUMP" );
		break;
	case CUPK_CROSS_BOW_JUMP:							
		StringCbCopy( szName, sizeof( szName ), "CUPK_CROSS_BOW_JUMP" );
		break;
	case CUPK_JUMP_ATTACK:					
		StringCbCopy( szName, sizeof( szName ), "CUPK_JUMP_ATTACK" );
		break;
	case CUPK_JUMP_RELOAD:
		StringCbCopy( szName, sizeof( szName ), "CUPK_JUMP_RELOAD" );
		break;
	case CUPK_DASH_ATTACK:					
		StringCbCopy( szName, sizeof( szName ), "CUPK_DASH_ATTACK" );
		break;
	case CUPK_PICK_ITEM_TRY:					
		StringCbCopy( szName, sizeof( szName ), "CUPK_PICK_ITEM_TRY" );
		break;
	case CUPK_USE_SKILL:						
		StringCbCopy( szName, sizeof( szName ), "CUPK_USE_SKILL" );
		break;
	case CUPK_WEAPON_DEFENSE:				
		StringCbCopy( szName, sizeof( szName ), "CUPK_WEAPON_DEFENSE" );
		break;
	case CUPK_WEAPON_WOUNDED:				
		StringCbCopy( szName, sizeof( szName ), "CUPK_WEAPON_WOUNDED" );
		break;
	case CUPK_EXTEND_DEFENSE:				
		StringCbCopy( szName, sizeof( szName ), "CUPK_EXTEND_DEFENSE" );
		break;
	case CUPK_DROP_DIE_MOTION:				
		StringCbCopy( szName, sizeof( szName ), "CUPK_DROP_DIE_MOTION" );
		break;
	case CUPK_BLOW_GET_UP:					
		StringCbCopy( szName, sizeof( szName ), "CUPK_BLOW_GET_UP" );
		break;
	case CUPK_DROP_DOWN_GET_UP:				
		StringCbCopy( szName, sizeof( szName ), "CUPK_DROP_DOWN_GET_UP" );
		break;
	case CUPK_SKILL_END:						
		StringCbCopy( szName, sizeof( szName ), "CUPK_SKILL_END" );
		break;
	case CUPK_SYSTEM_DAMAGE:					
		StringCbCopy( szName, sizeof( szName ), "CUPK_SYSTEM_DAMAGE" );
		break;
	case CUPK_MINE_WAIT_EXPLOSION:			
		StringCbCopy( szName, sizeof( szName ), "CUPK_MINE_WAIT_EXPLOSION" );
		break;
	case CUPK_MINE_EXPLOSION:				
		StringCbCopy( szName, sizeof( szName ), "CUPK_MINE_EXPLOSION" );
		break;
	case CUPK_COUNTER_ATTACK:				
		StringCbCopy( szName, sizeof( szName ), "CUPK_COUNTER_ATTACK" );
		break;
	case CUPK_SYMBOL_DAMAMGED:				
		StringCbCopy( szName, sizeof( szName ), "CUPK_SYMBOL_DAMAMGED" );
		break;
	case CUPK_EXPLOSION_ON_HAND:				
		StringCbCopy( szName, sizeof( szName ), "CUPK_EXPLOSION_ON_HAND" );
		break;
	case CUPK_EXPLOSION_DROP:				
		StringCbCopy( szName, sizeof( szName ), "CUPK_EXPLOSION_DROP" );
		break;
	case CUPK_ITEM_GAUGE_OK:					
		StringCbCopy( szName, sizeof( szName ), "CUPK_ITEM_GAUGE_OK" );
		break;
	case CUPK_ITEM_GAUGE_CANCEL:				
		StringCbCopy( szName, sizeof( szName ), "CUPK_ITEM_GAUGE_CANCEL" );
		break;
	case CUPK_FALL_DAMAGE:					
		StringCbCopy( szName, sizeof( szName ), "CUPK_FALL_DAMAGE" );
		break;
	case CUPK_REQUEST_CHAR_CONTROL:	
		StringCbCopy( szName, sizeof( szName ), "CUPK_REQUEST_CHAR_CONTROL" );
		break;
	case CUPK_ADD_NEW_BUFF:					
		StringCbCopy( szName, sizeof( szName ), "CUPK_ADD_NEW_BUFF" );
		break;
	case CUPK_CHANGE_DASH_RUN:				
		StringCbCopy( szName, sizeof( szName ), "CUPK_CHANGE_DASH_RUN" );
		break;
	case CUPK_UPDATE_MAGIC_CIRCLE:			
		StringCbCopy( szName, sizeof( szName ), "CUPK_UPDATE_MAGIC_CIRCLE" );
		break;
	case CUPK_FLY_JUMP:						
		StringCbCopy( szName, sizeof( szName ), "CUPK_FLY_JUMP" );
		break;
	case CUPK_PRE_DEFENSE_RELEASE:			
		StringCbCopy( szName, sizeof( szName ), "CUPK_PRE_DEFENSE_RELEASE" );
		break;
	case CUPK_DOUBLE_JUMP:					
		StringCbCopy( szName, sizeof( szName ), "CUPK_DOUBLE_JUMP" );
		break;
	case CUPK_VOICE_DATA:                    
		StringCbCopy( szName, sizeof( szName ), "CUPK_VOICE_DATA" );
		break;
	case CUPK_SKILL_EXTRA_INFO:				
		StringCbCopy( szName, sizeof( szName ), "CUPK_SKILL_EXTRA_INFO" );
		break;
	case CUPK_AUTO_SKILL:                    
		StringCbCopy( szName, sizeof( szName ), "CUPK_AUTO_SKILL" );
		break;
	case CUPK_PUSHSTRUCT_MOVE:				
		StringCbCopy( szName, sizeof( szName ), "CUPK_PUSHSTRUCT_MOVE" );
		break;
	case CUPK_PUSHSTRUCT_DAMAGE:				
		StringCbCopy( szName, sizeof( szName ), "CUPK_PUSHSTRUCT_DAMAGE" );
		break;
	case CUPK_PUSHSTRUCT_DELAY:				
		StringCbCopy( szName, sizeof( szName ), "CUPK_PUSHSTRUCT_DELAY" );
		break;
	case CUPK_PUSHSTRUCT_INFO:				
		StringCbCopy( szName, sizeof( szName ), "CUPK_PUSHSTRUCT_INFO" );
		break;
	case CUPK_BUFF_JUMP:						
		StringCbCopy( szName, sizeof( szName ), "CUPK_BUFF_JUMP" );
		break;
	case CUPK_RELEASE_FROZEN:				
		StringCbCopy( szName, sizeof( szName ), "CUPK_RELEASE_FROZEN" );
		break;
	case CUPK_AIM_FIRE:						
		StringCbCopy( szName, sizeof( szName ), "CUPK_AIM_FIRE" );
		break;
	case CUPK_JUMPPING:						
		StringCbCopy( szName, sizeof( szName ), "CUPK_JUMPPING" );
		break;
	case CUPK_FIELDITEM_INFO:				
		StringCbCopy( szName, sizeof( szName ), "CUPK_FIELDITEM_INFO" );
		break;
	case CUPK_DRINK:							
		StringCbCopy( szName, sizeof( szName ), "CUPK_DRINK" );
		break;
	case CUPK_DROP_THROWBOMB:				
		StringCbCopy( szName, sizeof( szName ), "CUPK_DROP_THROWBOMB" );
		break;
	case CUPK_FALL_STATE:					
		StringCbCopy( szName, sizeof( szName ), "CUPK_FALL_STATE" );
		break;
	case CUPK_END_BUFF:						
		StringCbCopy( szName, sizeof( szName ), "CUPK_END_BUFF" );
		break;
	case CUPK_P2P_PING:                      
		StringCbCopy( szName, sizeof( szName ), "CUPK_P2P_PING" );
		break;
	case CUPK_SHOPPING:                      
		StringCbCopy( szName, sizeof( szName ), "CUPK_SHOPPING" );
		break;
	case CUPK_END_AREA:		                
		StringCbCopy( szName, sizeof( szName ), "CUPK_END_AREA" );
		break;
	case CUPK_REMOVE_BUFF:					
		StringCbCopy( szName, sizeof( szName ), "CUPK_REMOVE_BUFF" );
		break;
	case CUPK_PUSHSTRUCT_CREATEWEAPON:		
		StringCbCopy( szName, sizeof( szName ), "CUPK_PUSHSTRUCT_CREATEWEAPON" );
		break;
	case CUPK_ELEVATOR_CHANGED:				
		StringCbCopy( szName, sizeof( szName ), "CUPK_ELEVATOR_CHANGED" );
		break;
	case CUPK_CLOSEORDER_DIRECTION:			
		StringCbCopy( szName, sizeof( szName ), "CUPK_CLOSEORDER_DIRECTION" );
		break;
	case CUPK_EXTEND_JUMP:					
		StringCbCopy( szName, sizeof( szName ), "CUPK_EXTEND_JUMP" );
		break;
	case CUPK_EXTEND_DASH:
		StringCbCopy( szName, sizeof( szName ), "CUPK_EXTEND_DASH" );
		break;
	case CUPK_BREAK_FALL:					
		StringCbCopy( szName, sizeof( szName ), "CUPK_BREAK_FALL" );
		break;
	case CUPK_GUILD_INFO_CHANGE:             
		StringCbCopy( szName, sizeof( szName ), "CUPK_GUILD_INFO_CHANGE" );
		break;
	case CUPK_EMOTICON_SYNC:					
		StringCbCopy( szName, sizeof( szName ), "CUPK_EMOTICON_SYNC" );
		break;
	case CUPK_RELEASE_FLOAT:					
		StringCbCopy( szName, sizeof( szName ), "CUPK_RELEASE_FLOAT" );
		break;
	case CUPK_RELEASE_CLOSEORDER:			
		StringCbCopy( szName, sizeof( szName ), "CUPK_RELEASE_CLOSEORDER" );
		break;
	case CUPK_TELEPORT_STATE:				
		StringCbCopy( szName, sizeof( szName ), "CUPK_TELEPORT_STATE" );
		break;
	case CUPK_CUSTOM_SOUND_INFO:             
		StringCbCopy( szName, sizeof( szName ), "CUPK_CUSTOM_SOUND_INFO" );
		break;
	case CUPK_CUSTOM_SOUND_DATA:             
		StringCbCopy( szName, sizeof( szName ), "CUPK_CUSTOM_SOUND_DATA" );
		break;
	case CUPK_CHANGE_CHARINFO:               
		StringCbCopy( szName, sizeof( szName ), "CUPK_CHANGE_CHARINFO" );
		break;
	case CUPK_NONE_PLAY_STATE:               
		StringCbCopy( szName, sizeof( szName ), "CUPK_NONE_PLAY_STATE" );
		break;
	case CUPK_SPECIAL_MOTION:				
		StringCbCopy( szName, sizeof( szName ), "CUPK_SPECIAL_MOTION" );
		break;
	case CUPK_USER_INFO_REFRESH:             
		StringCbCopy( szName, sizeof( szName ), "CUPK_USER_INFO_REFRESH" );
		break;
	case CUPK_USER_INFO_SYNC:                
		StringCbCopy( szName, sizeof( szName ), "CUPK_USER_INFO_SYNC" );
		break;
	case CUPK_EXCAVATING:
		StringCbCopy( szName, sizeof( szName ), "CUPK_EXCAVATING" );
		break;
	case CUPK_BALL_INFO:
		StringCbCopy( szName, sizeof( szName ), "CUPK_BALL_INFO" );
		break;
	case CUPK_JUMP_DASH:
		StringCbCopy( szName, sizeof( szName ), "CUPK_JUMP_DASH" );
		break;
	case SUPK_CONNECT:                       
		StringCbCopy( szName, sizeof( szName ), "SUPK_CONNECT" );
		break;
	case SUPK_SYNCTIME:                      
		StringCbCopy( szName, sizeof( szName ), "SUPK_SYNCTIME" );
		break;
	case SUPK_MODE_PING:			            
		StringCbCopy( szName, sizeof( szName ), "SUPK_MODE_PING" );
		break;
	case SUPK_PROTECT:       
		StringCbCopy( szName, sizeof( szName ), "SUPK_PROTECT" );
		break;
	case STPK_EVENT_ALIVE:       
		StringCbCopy( szName, sizeof( szName ), "STPK_EVENT_ALIVE" );
		break;
	case LUPK_LOG:     
		StringCbCopy( szName, sizeof( szName ), "LUPK_LOG" );
		break;
	case CUPK_NEW_JUMP_ATTACK:
		StringCbCopy( szName, sizeof( szName ), "CUPK_NEW_JUMP_ATTACK" );
		break;
	case CUPK_WEREWOLF_STATE:
		StringCbCopy( szName, sizeof( szName ), "CUPK_WEREWOLF_STATE" );
		break;
	case CUPK_CHAR_ACTION_INFO:
		StringCbCopy( szName, sizeof( szName ), "CUPK_CHAR_ACTION_INFO" );
		break;
	case CUPK_LAG_ACTION:               
		StringCbCopy( szName, sizeof( szName ), "CUPK_LAG_ACTION" );
		break;
	default:
		StringCbPrintf( szName, sizeof( szName ), "0x%x", dwPacketID );
		break;
	}

	char szText[MAX_PATH]="";
	if( bSend )
		StringCbPrintf( szText, sizeof( szText), "   Send %s", szName );
	else
		StringCbPrintf( szText, sizeof( szText), "Recv %s (%s:%d)", szName , szRcvIP, iRcvPort );

	//LOG.PrintConsoleLog( 0, szText );
	LOG.PrintTimeAndLog( 0, szText );
}
#endif // _DEBUG

void ioApplication::SetAfterCandidateWnd( UINT uMsg, WPARAM wParam )
{
	if( uMsg != WM_IME_NOTIFY )
		return;

	if( wParam == IMN_OPENCANDIDATE )
	{
		SetDialogBoxMode( true );
		m_bProcessCandidate = true;
	}
	else if( wParam == IMN_CLOSECANDIDATE )
	{
		SetDialogBoxMode( false );	
		m_bProcessCandidate = false;
		ShowWindowCursor( SWCT_CANDIDATE, false );
	}
}

void ioApplication::ProcessCandidateWnd()
{
	if( !m_bProcessCandidate )
		return;

	POINT ptPoint;
	GetCursorPos( &ptPoint );

	HWND hCurWnd = WindowFromPoint( ptPoint );

	if( hCurWnd != m_hWnd )
	{
		ShowWindowCursor( SWCT_CANDIDATE, true );
	}
	else
	{
		ShowWindowCursor( SWCT_CANDIDATE, false );
	}
}

void ioApplication::ProcessTickCheck()
{
	if( m_dwProcessTimeCheck == 0 )
	{
		m_dwProcessTimeCheck = timeGetTime();
		return;
	}

	if( m_pTickTimer == NULL ) return;

	DWORD dwGapTime = timeGetTime() - m_dwProcessTimeCheck;
	if( timeGetTime() - m_dwProcessTimeLagTime > 2000 )
	{
		m_dwProcessTimeLagTime = timeGetTime();
		m_dwProcessTimeLagCount= 0;
	}
	m_dwProcessTimeCheck = timeGetTime();

	if( dwGapTime > FLOAT100 )
	{
		m_dwProcessTimeLagCount++;
	}

	if( m_dwProcessTimeLagCount < 3 ) return;

	ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
	if( !pPlayStage ) return;
	
#if defined(SRC_TW) || defined (SRC_ID) || defined (SRC_SEA)
	if( (pPlayStage->GetRoomStyle() == RSTYLE_LADDERBATTLE) )
	{

	}
	else if( g_BattleRoomMgr.GetBattleEventType() == BET_TOURNAMENT_BATTLE )
	{

	}
	else
	{
		return;
	}
#else
	if( !g_LadderTeamMgr.IsHeroMatchMode()) return;
#endif

	ioBaseChar *pOwnerChar = pPlayStage->GetOwnerChar();
	if( !pOwnerChar ) return;
	if( pOwnerChar->GetState() == CS_VIEW ||
		pOwnerChar->GetState() == CS_LOADING ||
		pOwnerChar->GetState() == CS_DIE  ||
		pOwnerChar->GetState() == CS_READY||
		pOwnerChar->GetState() == CS_OBSERVER ||
		pOwnerChar->IsPrisonerMode() )
	{
		return;
	}

	DWORD dwDuration = 2000;
	pOwnerChar->SetNoInputEmoticon( dwDuration );
	pOwnerChar->StartNoInputDelayTime( dwDuration );	

/*	char szLog[MAX_PATH] = "";
	sprintf( szLog, "Frame Delay : %s - %d(%.2f)", pOwnerChar->GetCharName().c_str(), dwGapTime, m_pTickTimer->GetFPS() );
	SP2Packet kPacket( LUPK_LOG );
	kPacket << "ClientError";  // �α� ���� Ÿ��
	kPacket << szLog;
	kPacket << 317;  // ������ȣ:317
	kPacket << false; // write db
	P2PNetwork::SendLog(kPacket);
*/
}

void ioApplication::LoadNotMakeID()
{
	m_vNotMakeIDVector.clear();

	// ���� ������ �г��� ���� ���� �߰�	JCLEE 140414
	char	chNick[MAX_PATH] = {0,};
	int		iLocal = ioLocalManager::GetLocalType();

	switch( iLocal )
	{
	case ioLocalManager::LCT_SINGAPORE :
		{
			wsprintf_e( chNick, "config/sp2_not_make_id.ini" );
			LOG.PrintTimeAndLog( 0, "ioApplication::LoadNotMakeID(), ioLocalManager::LCT_SINGAPORE");
		}
		break;

	case ioLocalManager::LCT_MALAYSIA :
		{
			wsprintf_e( chNick, "config/sp2_not_make_id_my.ini" );
			LOG.PrintTimeAndLog( 0, "ioApplication::LoadNotMakeID(), ioLocalManager::LCT_MALAYSIA");
		}
		break;

	case ioLocalManager::LCT_EU :
		{
			wsprintf_e( chNick, "config/sp2_not_make_id.ini" );
			LOG.PrintTimeAndLog( 0, "ioApplication::LoadNotMakeID(), ioLocalManager::LCT_EU");
		}
		break;

	case ioLocalManager::LCT_GERMANY :
		{
			wsprintf_e( chNick, "config/sp2_not_make_id_de.ini" );
			LOG.PrintTimeAndLog( 0, "ioApplication::LoadNotMakeID(), ioLocalManager::LCT_GERMANY");
		}
		break;

	case ioLocalManager::LCT_FRANCE :
		{
			wsprintf_e( chNick, "config/sp2_not_make_id_fr.ini" );
			LOG.PrintTimeAndLog( 0, "ioApplication::LoadNotMakeID(), ioLocalManager::LCT_FRANCE");
		}
		break;

	case ioLocalManager::LCT_ITALIA :
		{
			wsprintf_e( chNick, "config/sp2_not_make_id_it.ini" );
			LOG.PrintTimeAndLog( 0, "ioApplication::LoadNotMakeID(), ioLocalManager::LCT_ITALIA");
		}
		break;

	case ioLocalManager::LCT_POLAND :
		{
			wsprintf_e( chNick, "config/sp2_not_make_id_pl.ini" );
			LOG.PrintTimeAndLog( 0, "ioApplication::LoadNotMakeID(), ioLocalManager::LCT_POLAND");
		}
		break;

	case ioLocalManager::LCT_TURKEY :
		{
			wsprintf_e( chNick, "config/sp2_not_make_id_tr.ini" );
			LOG.PrintTimeAndLog( 0, "ioApplication::LoadNotMakeID(), ioLocalManager::LCT_TURKEY");
		}
		break;

	default:
		{
			wsprintf_e( chNick, "config/sp2_not_make_id.ini" );
			//LOG.PrintTimeAndLog( 0, "ioApplication::LoadNotMakeID(), ioLocalManager:: %d", ioLocalManager::GetLocalType() );
		}
		break;
	}	

	ioINILoader kLoader( chNick );
	kLoader.SetTitle_e("word");

	char szKeyName[MAX_PATH]="";
	char szBuff[MAX_PATH]="";
	int iMax = kLoader.LoadInt("max", 0);
	for (int i = 0; i < iMax ; i++)
	{
		ZeroMemory( szKeyName, sizeof( szKeyName ) );
		ZeroMemory( szBuff, sizeof( szBuff ) );

		StringCbPrintf(szKeyName, sizeof(szKeyName), "%d", i+1);
		kLoader.LoadString(szKeyName, "", szBuff, sizeof(szBuff));
		strlwr( szBuff );
		if(!strcmp(szBuff, ""))
			continue;

		m_vNotMakeIDVector.push_back( ioHashString( szBuff ) );
	}

	kLoader.SetTitle_e( "ascii" );
	iMax = kLoader.LoadInt("max", 0);
	for (int i = 0; i < iMax ; i++)
	{
		ZeroMemory( szKeyName, sizeof( szKeyName ) );
		ZeroMemory( szBuff, sizeof( szBuff ) );

		StringCbPrintf(szKeyName, sizeof(szKeyName), "%d", i+1);
		int iAsciiCode = kLoader.LoadInt(szKeyName, -1);
		if( iAsciiCode == -1)
			break;

		szBuff[0] = iAsciiCode;
		if(!strcmp(szBuff, ""))
			continue;

		m_vNotMakeIDVector.push_back( ioHashString( szBuff ) );
	}

	kLoader.SetTitle_e("multibyte");
	iMax = kLoader.LoadInt( "max", 0);
	enum { MAX_BUFF_ARRAY = 2,};
	int iBuffArray = 0;
	for( int i = 0; i < iMax ; i++ )
	{
		ZeroMemory( szKeyName, sizeof( szKeyName ) );
		if( iBuffArray == 0 )
			ZeroMemory( szBuff, sizeof( szBuff ) );

		StringCbPrintf(szKeyName, sizeof(szKeyName), "%d", i+1 );
		int iAsciiCode = kLoader.LoadInt(szKeyName, -1);
		if( iAsciiCode == -1)
			break;

		szBuff[iBuffArray] = iAsciiCode;
		if(!strcmp(szBuff, ""))
			continue;

		iBuffArray++;
		if( iBuffArray >= MAX_BUFF_ARRAY )
		{
			iBuffArray = 0;
			m_vNotMakeIDVector.push_back( ioHashString( szBuff ) );
		}
	}
}

bool ioApplication::IsNotMakeID( const char *szNewID, OUT ioHashString &rszNotMakeString )
{
	char szLwrID[ID_NUM_PLUS_ONE]="";
	StringCbCopy( szLwrID, sizeof( szLwrID ), szNewID );
	strlwr( szLwrID );
	int iVecSize = m_vNotMakeIDVector.size();
	for (int i = 0; i < iVecSize ; i++)
	{

#if defined( SRC_OVERSEAS ) && !defined( SRC_ID )

#if defined( MULTI_BYTE_CHECK )
		if( IsDBCSLeadByteEx( COUNTRY_CODE_PAGE, m_vNotMakeIDVector[i].At( 0 ) ) )
#else
		if( false && IsDBCSLeadByteEx( COUNTRY_CODE_PAGE, m_vNotMakeIDVector[i].At( 0 ) ) )
#endif

#else
		if( IsDBCSLeadByte( m_vNotMakeIDVector[i].At( 0 ) ) )
#endif

		{
			if( _mbsstr( (const unsigned char*)szLwrID, (const unsigned char*)m_vNotMakeIDVector[i].c_str() ) )
			{
				rszNotMakeString = m_vNotMakeIDVector[i].c_str();
				return true;
			}
		}
		else if( m_vNotMakeIDVector[i].Length() == 1 )
		{
			int iIDSize = strlen( szLwrID );
			for (int i2 = 0; i2 < iIDSize ; i2++)
			{

#if defined( SRC_OVERSEAS ) && !defined( SRC_ID )

#if defined( MULTI_BYTE_CHECK )
				if( IsDBCSLeadByteEx( COUNTRY_CODE_PAGE, szLwrID[i2] ) )
#else
				if( false && IsDBCSLeadByteEx( COUNTRY_CODE_PAGE, szLwrID[i2] ) ) 
#endif

#else
				if( IsDBCSLeadByte( szLwrID[i2] ) ) 
#endif

				{
					i2++;
				}
				else
				{
					if( szLwrID[i2] == m_vNotMakeIDVector[i].At(0) )
					{
						rszNotMakeString = m_vNotMakeIDVector[i].c_str();
						return true;
					}
				}
			}
		}
		else
		{
			if( strstr( szLwrID, m_vNotMakeIDVector[i].c_str() ) )
			{
				rszNotMakeString = m_vNotMakeIDVector[i].c_str();
				return true;
			}
		}
	}

	return false;
}

void ioApplication::EnableAltCtrl()
{
	if(m_pBlockKeyBoard)
	{
		m_pBlockKeyBoard->EnableSysKey();
	}
}

void ioApplication::DisableAltCtrl()
{
	if(m_pBlockKeyBoard)
	{
		m_pBlockKeyBoard->DisableSysKey(m_hInstance);
	}
}

//UJ 130903, ����ȭ: ���׷� ResetVolume() ��� �Ҹ����� ����, ���� 2.5�ʿ��� 3.5�ʷ� �ø�
// �嵥 ������ ������ �𸣰���, â�� Ű��� �Ҹ��� Ŀ���� ���װ� �־��ٰ�??
void ioApplication::CheckForegroundForSound()
{
	HWND hFore = GetForegroundWindow();

	bool bForeground = false;
	if( m_hWnd == hFore )
	{
		if( !m_bFocus && m_dwResetSoundTime == 0 )
			m_dwResetSoundTime = FRAMEGETTIME() + 3500;//2500;

		m_bFocus = TRUE;
		bForeground = true;
	}
	else if( m_pBrowserMgr )
	{
		if( m_pBrowserMgr->HasFocusInBrowser( hFore ) )
		{
			if( !m_bFocus && m_dwResetSoundTime == 0 )
				m_dwResetSoundTime = FRAMEGETTIME() + 3500;//2500;

			m_bFocus = TRUE;
			bForeground = true;
		}
	}

	if( !bForeground )
	{
		m_bFocus = FALSE;
		m_dwResetSoundTime = 0;
	}

	if( m_dwResetSoundTime > 0 )
	{
		//UJ 130903, ResetVolume()�� ��δ� �����ϰ� ����(2500 ������ ���)
		// ���� �ڵ忡 ���װ� �־ �� �����Ӹ��� �Ҹ������� ������ --start
		//if( m_dwResetSoundTime > FRAMEGETTIME() )
		//	g_SoundMgr.ResetVolume();
		//else
		//	m_dwResetSoundTime = 0;
		if( FRAMEGETTIME() > m_dwResetSoundTime )
		{
			g_SoundMgr.ResetVolume();
			m_dwResetSoundTime = 0;
		}
		//UJ --end
	}
}

void ioApplication::ChangeDisplaySettingsBySetting()
{
	DEVMODE dmSettings;                                 // Device Mode variable
	memset(&dmSettings,0,sizeof(dmSettings));           // Makes Sure Memory's Cleared

	// Get current settings -- This function fills our the settings
	// This makes sure NT and Win98 machines change correctly
	if(!EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&dmSettings))
	{
		// Display error message if we couldn't get display settings
		LOG.PrintTimeAndLog( 0, "Could Not Enum Display Settings" );
		return;
	}

	dmSettings.dmPelsWidth  = Setting::Width();             // Selected Screen Width
	dmSettings.dmPelsHeight = Setting::Height();            // Selected Screen Height

	// This function actually changes the screen to full screen
	// CDS_FULLSCREEN Gets Rid Of Start Bar.
	// We always want to get a result from this function to check if we failed
	int result = ChangeDisplaySettings(&dmSettings,CDS_FULLSCREEN);

	// Check if we didn't recieved a good return message From the function
	if(result != DISP_CHANGE_SUCCESSFUL)
	{
		LOG.PrintTimeAndLog( 0, "Display Mode Not Compatible" );
		return;
	}
}

void ioApplication::SetTestSendData( SP2Packet &rkPacket )
{
	if( m_dwTestSendStartTime == 0 )
		m_dwTestSendStartTime = timeGetTime();

	m_iTestSendCnt++;
	m_dwTestSendSize += rkPacket.GetBufferSize();
}

void ioApplication::SetTestReciveData( SP2Packet &rkPacket )
{
	if( m_dwTestReciveStartTime == 0 )
		m_dwTestReciveStartTime = timeGetTime();

	m_iTestReciveCnt++;
	m_dwTestReciveSize += rkPacket.GetBufferSize();
}

void ioApplication::CheckTestSendRecive()
{
	DWORD dwCurTime = timeGetTime();
	DWORD dwGapTime = dwCurTime - m_dwTestReciveStartTime;
	DWORD dwGapTime2 = dwCurTime - m_dwTestSendStartTime;
	DWORD dwGapTime3 = dwCurTime - m_dwTestFrameStartTime;

	float fRecvPerSec = 0.0f;
	float fRecvPerSec2 = 0.0f;
	if( dwGapTime > 0 )
	{
		float fSec = (float)dwGapTime / FLOAT1000;
		fRecvPerSec = (float)m_iTestReciveCnt / fSec;
		fRecvPerSec2 = (float)m_dwTestReciveSize / fSec;
	}

	float fSendPerSec = 0.0f;
	float fSendPerSec2 = 0.0f;
	if( dwGapTime2 > 0 )
	{
		float fSec = (float)dwGapTime2 / FLOAT1000;
		fSendPerSec = (float)m_iTestSendCnt / fSec;
		fSendPerSec2 = (float)m_dwTestSendSize / fSec;
	}

	float fFramePerSec = 0.0f;
	if( dwGapTime3 > 0 )
	{
		float fSec = (float)dwGapTime3 / FLOAT1000;
		fFramePerSec = (float)m_iTestFrameCnt / fSec;
	}

	LOG.PrintTimeAndLog( 0, "FC - TestRecvPacket : %f, %f, %d, %d, %d", fRecvPerSec, fRecvPerSec2, m_iTestReciveCnt, m_dwTestReciveSize, dwGapTime );
	LOG.PrintTimeAndLog( 0, "FC - TestSendPacket : %f, %f, %d, %d, %d", fSendPerSec, fSendPerSec2, m_iTestSendCnt, m_dwTestSendSize, dwGapTime2 );
	LOG.PrintTimeAndLog( 0, "FC - TestFrame : %f, %d, %d", fFramePerSec, m_iTestFrameCnt, dwGapTime3 );

	m_dwTestReciveStartTime = 0;
	m_dwTestSendStartTime = 0;
	m_dwTestFrameStartTime = 0;

	m_iTestReciveCnt = 0;
	m_iTestSendCnt = 0;
	m_iTestFrameCnt = 0;

	m_dwTestSendSize = 0;
	m_dwTestReciveSize = 0;
}

void ioApplication::ProcessTestPacketCnt()
{
	if( !Help::CheckEnableTimeGapUser() )
		return;

	if( m_dwTestReciveStartTime == 0 )
	{
		m_dwTestReciveStartTime = timeGetTime();
	}
	else
	{
		DWORD dwGapTime2 = timeGetTime() - m_dwTestReciveStartTime;
		if( dwGapTime2 > 1000 )
		{
			if( m_iTestReciveCnt > 2 )
				LOG.PrintTimeAndLog( 0, "FC - RecvPacketCnt : %d, %d, %d", m_iTestReciveCnt, m_dwTestReciveTime, dwGapTime2 );

			m_dwTestReciveStartTime = timeGetTime();
			m_iTestReciveCnt = 0;
			m_dwTestReciveTime = 0;
		}
	}
}

void ioApplication::ClearLoadingChangeHelp()
{
	if( m_pLoadingRender )
	{
		m_pLoadingRender->SetChangeServerHelp( ioHashString("") );
	}
}

bool ioApplication::IsRelayServer()
{
	return m_bRelayUse;
}

void ioApplication::SetRelayServerState( bool bState )
{
	m_bRelayUse = bState;
}

void ioApplication::SetRelayServerIpaddr( const char* ipaddr, const int port )
{
	strcpy_s(m_realyIpaddr,ipaddr);
	m_relayPort = port;
}

bool ioApplication::SendUDPMessageToServer( SP2Packet& pk )
{
#ifdef _ITEST
	if( g_bTestAntiHack )
	{
		if( pk.GetPacketID() == CUPK_ANTIHACK_CHAR_WOUNDED ||
			pk.GetPacketID() == CUPK_WEAPON_WOUNDED ||
			pk.GetPacketID() == CUPK_ANTIHACK_CHAR_HIT )
			return true;
	}
#endif

	if(g_App.IsRelayServer())
	{
		return m_pUDPSocket->SendLoop(m_realyIpaddr,m_relayPort,pk);
	}
	else
	{
		 
		return m_pUDPSocket->SendLoop( m_pTCPSocket->GetConnectedIP(), m_pTCPSocket->GetConnectedPort(), pk );
	}
	
}

void ioApplication::OnSubscriptionData( SP2Packet &rkPacket )
{
	if( m_pSubscriptionMgr )
	{
		g_SubscriptionMgr.ApplySubscriptionData( rkPacket );
	}
}

void ioApplication::OnSubscriptionRecv( SP2Packet &rkPacket )
{
	ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );

	if( m_pSubscriptionMgr )
	{
		g_SubscriptionMgr.ApplySubscriptionRecv( rkPacket, pPlayStage );
	}
}

void ioApplication::OnSubscriptionRetr( SP2Packet &rkPacket )
{
	if( m_pSubscriptionMgr )
	{
		g_SubscriptionMgr.ApplySubscriptionRetr( rkPacket );
	}
}

void ioApplication::OnNexonSessionMsg( SP2Packet &rkPacket )
{
	int nMsg;
	rkPacket >> nMsg;

	bool bShutDown = false;
	char szMsg[MAX_PATH] = {0,};

	switch( nMsg )
	{
	case NexonSessionMessage:
	case NexonSessionLogin:
	case NexonSessionLogout:
	case NexonReconnect:
	case NexonIDTrial:
	case NexonOnPCRoom:
		break;

	case NexonWelcomeMessage:
		{
			int nTime = 0;
			rkPacket >> nTime;
			if( nTime <= 0 )
			{
				bShutDown = true;
				StringCbCopy( szMsg, sizeof(szMsg), STR(1) );
				SendNexonAuthLog( NexonWelcomeMessage );
			}
// 			else
// 			{
// 				g_ChatMgr.SetSystemMsg( "�ؽ� PC�� ������ �ܿ��ð��� �� %d�� ���ҽ��ϴ�.", nTime );
// 			}
		}
		break;

// Shutdown 
	case NexonTerminate:
		StringCbCopy( szMsg, sizeof(szMsg), STR(2) );
		SendNexonAuthLog( NexonTerminate );
		bShutDown = true;
		break;
	case NexonIDMaxError:
		StringCbCopy( szMsg, sizeof(szMsg), STR(3) );
		SendNexonAuthLog( NexonIDMaxError );
		bShutDown = true;
		break;
	case NexonIPDuplicate:
		StringCbCopy( szMsg, sizeof(szMsg), STR(4) );
		SendNexonAuthLog( NexonIPDuplicate );
		bShutDown = true;
		break;
	case NexonIPMaxError:
		StringCbCopy( szMsg, sizeof(szMsg), STR(5) );
		SendNexonAuthLog( NexonIPMaxError );
		bShutDown = true;
		break;
	case NexonPCRoomBlocked:
		StringCbCopy( szMsg, sizeof(szMsg), STR(6) );
		SendNexonAuthLog( NexonPCRoomBlocked );
		bShutDown = true;
		break;
	case NexonMessageShutDown:
		StringCbCopy( szMsg, sizeof(szMsg), STR(7) );
		SendNexonAuthLog( NexonMessageShutDown );
		bShutDown = true;
		break;
	case NexonMessageTerminate:
		StringCbCopy( szMsg, sizeof(szMsg), STR(8) );
		SendNexonAuthLog( NexonMessageTerminate );
		bShutDown = true;
		break;
	case NexonAccountNotAllowed:
		StringCbCopy( szMsg, sizeof(szMsg), STR(9) );
		SendNexonAuthLog( NexonAccountNotAllowed );
		bShutDown = true;
		break;
	case NexonAccountMaxConnected:
		StringCbCopy( szMsg, sizeof(szMsg), STR(10) );
		SendNexonAuthLog( NexonAccountMaxConnected );
		bShutDown = true;
		break;
	case NexonPrepaidExhausted:
		StringCbCopy( szMsg, sizeof(szMsg), STR(11) );
		SendNexonAuthLog( NexonPrepaidExhausted );
		bShutDown = true;
		break;
	case NexonAccountMachineIDBlocked:
		StringCbCopy( szMsg, sizeof(szMsg), STR(12) );
		SendNexonAuthLog( NexonAccountMachineIDBlocked );
		bShutDown = true;
		break;
	case NexonSelectShutdown:
		StringCbCopy( szMsg, sizeof(szMsg), STR(13) );
		SendNexonAuthLog( NexonSelectShutdown );
		bShutDown = true;
		break;
	case NexonShutDown:
		StringCbCopy( szMsg, sizeof(szMsg), STR(14) );
		SendNexonAuthLog( NexonShutDown );
		bShutDown = true;
		break;
	case NexonForbidden:
		StringCbCopy( szMsg, sizeof(szMsg), STR(15) );
		StringCbCopy( szMsg, sizeof(szMsg), STR(1) );
		SendNexonAuthLog( NexonForbidden );
		bShutDown = true;
		break;
	case NexonNotNexonUser:
		StringCbCopy( szMsg, sizeof(szMsg), STR(16) );
		SendNexonAuthLog( NexonNotNexonUser );
		bShutDown = true;
		break;
	case NexonUnknownShutDown:
		StringCbCopy( szMsg, sizeof(szMsg), STR(17) );
		SendNexonAuthLog( NexonUnknownShutDown );
		bShutDown = true;
		break;
	case NexonAgeError:
		StringCbCopy( szMsg, sizeof(szMsg), STR(18) );
		SendNexonAuthLog( NexonAgeError );
		bShutDown = true;
		break;
	case NexonSessionServerError:
		StringCbCopy( szMsg, sizeof(szMsg), STR(19) );
		SendNexonAuthLog( NexonSessionServerError );
		bShutDown = true;
		break;
	case NexonAddressNotAllowed:
		StringCbCopy( szMsg, sizeof(szMsg), STR(20) );
		StringCbCopy( szMsg, sizeof(szMsg), STR(2) );
		SendNexonAuthLog( NexonAddressNotAllowed );
		bShutDown = true;
		break;
	}

	if( bShutDown )
	{
		LOG.PrintTimeAndLog( 0, "ExitProgram - 26" );

#if defined( USE_GA )
		if( GetGAStart() == true )
		{
			char chLabel[32] = {0,};

			if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
				sprintf_e( chLabel, "%d", 26 );
			else
				SafeSprintf( chLabel, sizeof(chLabel), "%1", 26 );

			// GAME_END_ERR_POS
			g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
				, "Game"
				, "Error"
				, chLabel
				, 1
				, "%2FGAME%2FOVER%2FERR"
				, 1 );

			g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FGAME%2FOVER%2FERR", 6, chLabel );
		}		
#endif

		SetExitProgram();
		Setting::Initialize("error");
		g_bHappenError = true;
		g_iErrorNumber = 330;
		SafeSprintf( g_szErrorLog, sizeof(g_szErrorLog), szMsg);

		if( m_pLogin )
			m_pLogin->SendMsgToAutoUpgrade( WM_MY_MSG_C_ERROR );
	}
}

void ioApplication::OnAttendanceRecord( SP2Packet &rkPacket )
{
	g_MyInfo.ApplyAttendanceRecord( rkPacket );

	ioTask* task	= new ioTask;
	task->eType		= RESERVEDCMD_TYPE_ATTENDANCE;
	task->eWhen		= RESERVEDCMD_WHEN_RECV_ATTENDANCE;

	g_RserveTask.AddReservedTask(task);
}

void ioApplication::OnAttendanceCheck( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );

	int iState = 0;
	rkPacket >> iState;

	switch( iState )
	{
	case ATTENDANCE_RECORD_SELECT_DB_WAIT:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
			return;
		}
		break;
	case ATTENDANCE_RECORD_EXIST:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
			return;
		}
		break;
	case ATTENDANCE_CHECK_FAIL:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
			return;
		}
		break;
	}
		
	ioTaskAttendanceReward* task = new ioTaskAttendanceReward;
	task->eType = RESERVEDCMD_TYPE_ATTENDANCE;
	task->eWhen = RESERVEDCMD_WHEN_RECV_ATTENDANCE_REWARD;

	rkPacket >> task->dwDate;
	rkPacket >> task->iTodayPresentType  >> task->iTodayValue1  >> task->iTodayValue2;
	rkPacket >> task->iAccruePresentType >> task->iAccrueValue1 >> task->iAccrueValue2;

	if( 0 < task->dwDate )
	{
		g_MyInfo.AddAttendanceRecord( task->dwDate );		
		g_RserveTask.AddReservedTask(task);
	}
	else
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
	}
}

void ioApplication::OnSearchPlazaRoom( SP2Packet &rkPacket )
{
	g_GUIMgr.HideWnd( TIMEGATE_SEARCH );
	g_GUIMgr.HideWnd( PLAZA_PASSWORD_WND );
	TCPNetwork::MouseBusy( false );

	int iResult;
	rkPacket >> iResult;
	switch( iResult )
	{
	case SEARCH_TRAINING_ERROR_1:         //������ ��
		g_GUIMgr.SetPrevMsgBox( NULL,  MB_OK, NULL, STR(1) );
		break;
	case SEARCH_TRAINING_ERROR_2:         //��� ����
		g_GUIMgr.SetPrevMsgBox( NULL,  MB_OK, NULL, STR(2) );
		break;
	case SEARCH_TRAINING_ERROR_3:         //�ο� ����
		g_GUIMgr.SetPrevMsgBox( NULL,  MB_OK, NULL, STR(3) );
		break;
	case SEARCH_TRAINING_ERROR_4:         //���� ����
		g_GUIMgr.SetPrevMsgBox( NULL,  MB_OK, NULL, STR(4) );
		break;
	case SEARCH_TRAINING_ERROR_5:         //���� ��ȣ�� �������� �Ʒ��̳� ���� �뿡 ����Ǿ���.
		break;
	case SEARCH_TRAINING_ERROR_6:
		g_GUIMgr.SetPrevMsgBox( NULL,  MB_OK, NULL, STR(5) );
		break;
	case SEARCH_TRAINING_ERROR_7:
		g_GUIMgr.SetPrevMsgBox( NULL,  MB_OK, NULL, STR(6) );
		break;
	case SEARCH_TRAINING_ERROR_8:
		g_GUIMgr.SetPrevMsgBox( NULL,  MB_OK, NULL, STR(1) );
		break;
	case SEARCH_TRAINING_ERROR_9:		
		g_GUIMgr.SetPrevMsgBox( NULL,  MB_OK, NULL, STR(2) );
		break;
	case SEARCH_TRAINING_ERROR_10:
		g_GUIMgr.SetPrevMsgBox( NULL,  MB_OK, NULL, STR(8) );
		break;
	case SEARCH_TRAINING_ERROR_11:
		g_GUIMgr.SetPrevMsgBox( NULL,  MB_OK, NULL, STR(3) );
		break;
	}

	TimeGateSearch *pSearchWnd = dynamic_cast<TimeGateSearch*>(g_GUIMgr.FindWnd( TIMEGATE_SEARCH ));
	if( pSearchWnd )
		pSearchWnd->DoReserve();

	// ���� -> �Ʒü� -> �κ�� ���� ��õ�
	if( IsStepByConnectTry() )
		g_App.SendSearchModeJoin( LOADING_FIRST );
}

void ioApplication::OnJoinHeadQuarters( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );

	int iResult;
	rkPacket >> iResult;

	switch( iResult )
	{
	case JOIN_HEADQUARTERS_ALREADY_BATTLE:
		break;
	case JOIN_HEADQUARTERS_OWNER_OFFLINE:
		{
			if( g_MyInfo.GetJoinHeadquartersRequestName().IsEmpty() )
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
				//g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
			else
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2), 
					g_MyInfo.GetJoinHeadquartersRequestName().c_str(),
					g_MyInfo.GetJoinHeadquartersRequestName().c_str() );
			}
		}
		break;
	case JOIN_HEADQUARTERS_ROOM_CREATE_FAILED:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		break;
	case JOIN_HEADQUARTERS_ROOM_FULL:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		break;
	case JOIN_HEADQUARTERS_EXCEPTION:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
		break;
	case JOIN_HEADQUARTERS_LOCK:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6) );
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		break;
	case JOIN_HEADQUARTERS_JOIN_REQUEST:
		{
			JoinRequestProgressWnd* pWnd = dynamic_cast<JoinRequestProgressWnd*>( g_GUIMgr.FindWnd( JOIN_REQUEST_PROGRESS_WND ) );
			if( pWnd )
			{
				pWnd->SetModeType( MT_HEADQUARTERS );
				pWnd->ShowWnd();
			}
		}
		break;
	}
	
	// ���� -> �Ʒü� -> �κ�� ���� ��õ�
	if( IsStepByConnectTry() )
		g_App.SendSearchModeJoin( LOADING_FIRST );
}

void ioApplication::OnCreatePlaza( SP2Packet &rkPacket )
{
	g_GUIMgr.HideWnd( TIMEGATE_SEARCH );
	TCPNetwork::MouseBusy( false );
	int iResult;
	rkPacket >> iResult;
	switch( iResult )
	{
	case CREATE_PLAZA_ERROR_1:
		g_GUIMgr.SetPrevMsgBox( NULL,  MB_OK, NULL, STR(1) );		
		break;
	case CREATE_PLAZA_ERROR_2:
		g_GUIMgr.SetPrevMsgBox( NULL,  MB_OK, NULL, STR(2) );
		break;
	case CREATE_PLAZA_ERROR_3:
		g_GUIMgr.SetPrevMsgBox( NULL,  MB_OK, NULL, STR(3) );
		break;
	case CREATE_PLAZA_ERROR_4:
		g_GUIMgr.SetPrevMsgBox( NULL,  MB_OK, NULL, STR(4) );
		break;
	case CREATE_PLAZA_ERROR_5:
		g_GUIMgr.SetPrevMsgBox( NULL,  MB_OK, NULL, STR(5) );
		break;
	}

	// ���� -> �Ʒü� -> �κ�� ���� ��õ�
	if( IsStepByConnectTry() )
		g_App.SendSearchModeJoin( LOADING_FIRST );

	TimeGateSearch *pSearchWnd = dynamic_cast<TimeGateSearch*>( g_GUIMgr.FindWnd( TIMEGATE_SEARCH ) );
	if( pSearchWnd )
		pSearchWnd->DoReserve();
}

void ioApplication::OnJoinHouse( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );

	int iResult;
	rkPacket >> iResult;

	switch( iResult )
	{
	case JOIN_PERSONAL_HQ_ALREADY_BATTLE:
		break;
	case JOIN_PERSONAL_HQ_ROOM_CREATE_FAILED:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		break;
	case JOIN_PERSONAL_HQ_ROOM_FULL:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		break;
	case JOIN_PERSONAL_HQ_EXCEPTION:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		break;
	case JOIN_PERSONAL_HQ_JOIN_REQUEST:
		{
			JoinRequestProgressWnd* pWnd = dynamic_cast<JoinRequestProgressWnd*>( g_GUIMgr.FindWnd( JOIN_REQUEST_PROGRESS_WND ) );
			if( pWnd )
			{
				pWnd->SetModeType( MT_HOUSE );
				pWnd->ShowWnd();
			}
		}
		break;
	case JOIN_PERSONAL_HQ_EMPTY:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		break;
	case JOIN_PERSONAL_HQ_OWNER_OFFLINE:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
		break;
	case JOIN_PERSONAL_HQ_DISABLE:
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6) );
		break;
	}

	// ���� -> �Ʒü� -> �κ�� ���� ��õ�
	if( IsStepByConnectTry() )
		g_App.SendSearchModeJoin( LOADING_FIRST );
}

void ioApplication::OnCreateTutorialStage( SP2Packet &rkPacket )
{
#ifdef SRC_OVERSEAS //svn 32537 Ʃ�丮�� ���Ź��� �ѹ�
	if( m_pGameStage == NULL )
		SetNextStage( ioGameStage::GS_PLAY, __FUNCTION__ );
#else
	IORandom random;
	random.SetRandomSeed( timeGetTime() );
	int iRand = random.Random( 2 );

	if ( iRand == 0 )
	{
		SP2Packet kPacket( CTPK_CREATE_BATTLEROOM );
		kPacket << BMT_TEAM_SURVIVAL_AI_EASY << "" << "" << 4 << 0 << 0;
		TCPNetwork::SendToServer( kPacket );
		TCPNetwork::MouseBusy( true );
	}
	else if ( iRand == 1 )
	{
		SP2Packet kPacket( CTPK_CREATE_BATTLEROOM );
		kPacket << BMT_DARK_XMAS_EASY << "" << "" << 4 << 4 << 0;
		TCPNetwork::SendToServer( kPacket );
		TCPNetwork::MouseBusy( true );
	}

	g_MyInfo.SetTutorialMode( true );

#endif
}

// Mission

void ioApplication::OnMissionData( SP2Packet &rkPacket )
{
	g_MissionManager.ProcessTCPPacket( rkPacket );
}

void ioApplication::OnRollBookReward( SP2Packet &rkPacket )
{
	int iState = 0;
	rkPacket >> iState;

	if ( iState == ROLLBOOK_ATTEND_FAIL )
	{
		if( g_App.IsMouseBusy() )
			TCPNetwork::MouseBusy( false );
		return;
	}

	DWORD dwReceiveTime = 0;
	int iTablePage,iPosition = 0;
	rkPacket >> dwReceiveTime;
	rkPacket >> iTablePage;
	rkPacket >> iPosition;

	if( g_App.IsMouseBusy() )
		TCPNetwork::MouseBusy( false );

	RollBookWnd* pWnd = dynamic_cast<RollBookWnd*>( g_GUIMgr.FindWnd( ROLLBOOK_WND ) );
	if( pWnd && pWnd->IsDirectShow() )
	{
		pWnd->SettingRollBookByPacket( dwReceiveTime, iState, iTablePage, iPosition );
	}
	else
	{
		ioTaskRollBook* task	= new ioTaskRollBook;
		task->eType		= RESERVEDCMD_TYPE_ROLLBOOK;
		task->eWhen		= RESERVEDCMD_WHEN_ROLLBOOK;
		task->dwReceiveTime = dwReceiveTime;
		task->iState	= iState;
		task->iTablePage = iTablePage;
		task->iPosition = iPosition;
		g_RserveTask.AddReservedTask( task );
	}
}

void ioApplication::OnGuildAttend( SP2Packet &rkPacket )
{
	int iState = 0;
	rkPacket >> iState;

	if ( iState == GUILD_ATTEND_OK )
	{
		bool bIsMe;
		g_GuildInfo.SetMyGuildAttendedUserInfoByPacket( rkPacket, bIsMe );

		//�⼮�� ������ ����� UI����
		if ( bIsMe )
		{
			TCPNetwork::MouseBusy( false );

			GuildMainWnd *pGuildMainWnd = dynamic_cast< GuildMainWnd* >(g_GUIMgr.FindWnd( GUILD_MAIN_WND ));
			if( pGuildMainWnd )
			{
				if( pGuildMainWnd->IsShow() && pGuildMainWnd->GetGuildIndex() == g_GuildInfo.GetGuildIndex() )
					pGuildMainWnd->UpdateGuildInfo();
			}
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		}
	}
	else if ( iState == GUILD_STAY_DAY_SCARCITY )
	{
		TCPNetwork::MouseBusy( false );
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
	}
	else if( iState == GUILD_ATTEND_FAIL )
	{
		TCPNetwork::MouseBusy( false );
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
	}
	else if ( iState == GUILD_ATTEND_DATE_PREV )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		SP2Packet kPacket( CTPK_GUILD_MEMBER_ATTEND_RENEWAL );
		TCPNetwork::SendToServer( kPacket );
		TCPNetwork::MouseBusy( true );

		LOG.PrintTimeAndLog( 0, "%s : State : GUILD_ATTEND_DATE_PREV " ,__FUNCTION__ );
	}
	else
	{
		TCPNetwork::MouseBusy( false );
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
	}
}

void ioApplication::OnGuildReceiveAttendReward( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );
	int iState = 0;
	rkPacket >> iState;

	if ( iState == GUILD_ATTEND_REWARD_OK )
	{
		g_GuildInfo.SetMyAttendRewardTimeByPacket( rkPacket );
		GuildMainWnd *pGuildMainWnd = dynamic_cast< GuildMainWnd* >(g_GUIMgr.FindWnd( GUILD_MAIN_WND ));
		if( pGuildMainWnd )
		{
			if( pGuildMainWnd->IsShow() && g_GuildInfo.IsMyGuild( pGuildMainWnd->GetGuildIndex() ) )
				pGuildMainWnd->UpdateGuildInfo();
		}

		BYTE btYesterdayAttendCount;
		int iPresentType, iPresentValue1,iPresentValue2;
		rkPacket >> btYesterdayAttendCount >> iPresentType >> iPresentValue1 >> iPresentValue2;
		GuildAttendRewardWnd *pGuildAttendRewardWnd = dynamic_cast< GuildAttendRewardWnd* >( g_GUIMgr.FindWnd( GUILD_ATTEND_REWARD_WND ) );
		if( pGuildAttendRewardWnd )
			pGuildAttendRewardWnd->SettingRewardInfo( btYesterdayAttendCount, (short)iPresentType, iPresentValue1, iPresentValue2 );

		LOG.PrintTimeAndLog( 0, "%s : Receive Guild Attend Reward, Yesterday AttendCount : %d, Item : %d,%d,%d ",__FUNCTION__, btYesterdayAttendCount, iPresentType, iPresentValue1, iPresentValue2 );
	}
	else if( iState == GUILD_ATTEND_REWARD_FAIL )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
	}
	else if( iState == GUILD_ATTEND_REWARD_DATE_PREV )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		SP2Packet kPacket( CTPK_GUILD_MEMBER_ATTEND_RENEWAL );
		TCPNetwork::SendToServer( kPacket );
		TCPNetwork::MouseBusy( true );
		LOG.PrintTimeAndLog( 0, "%s : State : GUILD_ATTEND_REWARD_DATE_PREV " ,__FUNCTION__ );
	}
	else
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
	}
}

void ioApplication::OnGuildRankReward( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );
	int iState = 0;
	rkPacket >> iState;
	if ( iState == GUILD_RANK_REWARD_OK )
	{
		ioTaskGuildRankReward* task = new ioTaskGuildRankReward;
		task->eType			= RESERVEDCMD_TYPE_GUILD_RANK_REWARD;
		task->eWhen			= RESERVEDCMD_WHEN_RECV_GUILD_RANK_REWARD;
		rkPacket >> task->iPresentType;
		rkPacket >> task->iPresentValue1;
		rkPacket >> task->iPresentValue2;
		g_RserveTask.AddReservedTask(task);

		LOG.PrintTimeAndLog( 0 , "%s : Accept Guild Rank Reward Data : %d.%d.%d ", __FUNCTION__, task->iPresentType, task->iPresentValue1, task->iPresentValue2 );
	}
	else if ( iState == GUILD_RANK_REWARD_FAIL )
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
	else
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
}

void ioApplication::OnGuildMemberAttendInfo( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );
	int iType;
	rkPacket >> iType;
	if ( iType == TODAY_ATTENDACE_MEMBER )
	{
		TCPNetwork::MouseBusy( false );
		g_GuildInfo.SetMyGuildMemberTodayAttendInfoByPacket( rkPacket );
		GuildMainWnd *pGuildMainWnd = dynamic_cast< GuildMainWnd* >(g_GUIMgr.FindWnd( GUILD_MAIN_WND ));
		if( pGuildMainWnd )
		{
			if( pGuildMainWnd->IsShow() && g_GuildInfo.IsMyGuild( pGuildMainWnd->GetGuildIndex() ) )
				pGuildMainWnd->UpdateGuildInfo();
		}
	}
	else if( iType == YESTERDAY_ATTENDANCE_MEMEBER )
		g_GuildInfo.SetMyGuildMemberYesterdayAttendInfoByPacket( rkPacket );
	else if( iType == GUILD_ATTEND_INFO_RENWARL_FAIL )
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
	else
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );	
}

void ioApplication::OnGenderChange( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );
	int eResult;
	rkPacket >> eResult;
	switch( eResult )
	{
	case CHANGE_GENDER_SUCCESS:
		{
			int eType, nCode;
			DWORD dwCharIndex;
			CHARACTER rkChar;
			rkPacket >> eType >> nCode >> dwCharIndex >> rkChar;

			g_MyInfo.AddSlotItem( eType, nCode );
			g_MyInfo.SetEquipItem( eType, nCode );
			g_MyInfo.ApplyCharDecoration( dwCharIndex, rkChar );

			ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
			if( pPlayStage )
				pPlayStage->ReLoadCharInfo( dwCharIndex, rkChar );

			// ���� ����ȭ
			SP2Packet kPacket2( CUPK_CHANGE_CHARINFO);
			kPacket2 << dwCharIndex << rkChar;
			g_MyInfo.FillCharItemData( dwCharIndex, kPacket2 );
			P2PNetwork::SendToAllUser( kPacket2 );

			CloseGuildMarkPage();

			NewShopWnd *pNewShopWnd = dynamic_cast<NewShopWnd*> ( g_GUIMgr.FindWnd( NEW_SHOP_WND ) );
			if( pNewShopWnd && pNewShopWnd->IsShow() )
			{
				pNewShopWnd->SetBoughtMark( NewShopWnd::ID_DECO_TAB_BTN, eType%1000, nCode%10000 );
				NewShopBuyWnd *pNewShopBuyWnd = dynamic_cast<NewShopBuyWnd*> ( pNewShopWnd->FindChildWnd( NewShopWnd::ID_BUY_WND ) );
				if( pNewShopBuyWnd && pNewShopBuyWnd->IsShow() )
				{
					pNewShopBuyWnd->ShowOnlyBoughtBtns();
				}
			}

			SoldierSelectWnd *pSoldierWnd = dynamic_cast<SoldierSelectWnd*>(g_GUIMgr.FindWnd( SOLDIER_SELECT_WND ));
			if( pSoldierWnd && pSoldierWnd->IsShow() )
			{
				pSoldierWnd->SettingSoldierBtn();
			}

			MiniSoldierSelectWnd *pMiniSoldierWnd = dynamic_cast<MiniSoldierSelectWnd*>( g_GUIMgr.FindWnd( MINI_SOLDIER_SELECT_WND ));
			if( pMiniSoldierWnd && pMiniSoldierWnd->IsShow() )
			{
				pMiniSoldierWnd->SettingMiniSoldierBtn();
			}
		}
		break;
	case CHANGE_GENDER_NON_EXIST_CHAR:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		}
		break;
	case CHANGE_GENDER_EXCEPTION:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		}
		break;
	}
}

void ioApplication::SendSearchModeJoin( ExLoadingType eLoadingType /* = LOADING_SEARCH_MODE */ )
{
	m_eLoadingType = eLoadingType;

	char szCall[MAX_PATH];
	sprintf_s( szCall, "%s - (%d)" ,__FUNCTION__, (int)eLoadingType );

	switch( g_App.GetConnectedMoveMode() )
	{
	case ioApplication::CMM_PLAZA:
		{
			ioMyRoomMode::SetMyRoomType( MRT_FIND_MODE );
			g_App.SetNextStage( ioGameStage::GS_FIND_MODE, szCall );
			g_App.SetModeType( 0, 0, 0, PT_NONE );

			SP2Packet kPacket( CTPK_SEARCH_PLAZA_ROOM );
			kPacket << -2 << false;
			TCPNetwork::SendToServer( kPacket );
			TCPNetwork::MouseBusy( true );

			if( IsStepByConnectTry() )
			{
				m_iConnectTryCount++;
				m_dwLoadingEndTime = REALGETTIME() + m_dwLoadingDelay;
			}
		}
		break;
	case ioApplication::CMM_HEADQUARTER:
		{
			ioMyRoomMode::SetMyRoomType( MRT_FIND_MODE );
			g_App.SetNextStage( ioGameStage::GS_FIND_MODE, szCall );
			g_App.SetModeType( MT_HEADQUARTERS, 0, 0, PT_NONE );

			SP2Packet kPacket( CTPK_JOIN_HEADQUARTERS );
			kPacket << g_MyInfo.GetPublicID() << g_ModeSelectInfo.CheckHeadquartersMapIndex( g_ModeSelectInfo.GetHeadquartersModeIndex(), 1 ) << false;
			TCPNetwork::SendToServer( kPacket );

			if( IsStepByConnectTry() )
			{
				m_iConnectTryCount++;
				m_dwLoadingEndTime = REALGETTIME() + m_dwLoadingDelay;
			}
		}
		break;
	case ioApplication::CMM_LOBBY:
		{
			ioMyRoomMode::SetMyRoomType( MRT_LOBBY );
			g_App.SetNextStage( ioGameStage::GS_LOBBY, szCall );

			if( IsStepByConnectTry() )
			{
				m_dwLoadingEndTime = REALGETTIME() + m_dwLoadingDelay;
			}
		}
		break;
	default:
		{
			g_QuestMgr.ProcessChangeStage();
			SP2Packet kPacket( CTPK_LOGOUT );
			TCPNetwork::SendToServer( kPacket );
		}
		break;
	}

	if( m_bEnableConnectedMoveMode && IsStepByConnectTry() && m_iConnectTryMax < m_iConnectTryCount )
	{
		int iPrev = m_eConnectedMoveMode;
		m_eConnectedMoveMode++;
		m_iConnectTryCount = 0;

		LOG.PrintTimeAndLog(0, "after the failed login by mode : %d, next try mode : %d" ,__FUNCTION__, iPrev, m_eConnectedMoveMode );
	}
}

bool ioApplication::IsSingleMode()
{
	if( m_pLogin )
		return m_pLogin->IsSingleMode();

	return true;
}

void ioApplication::ProcessMemoryUsageLOG( int iRefreshType )
{	
#ifndef _DEBUG
	if( !Help::CheckEnableTimeGapUser( 1 ) )
		return;
#endif

	static int time = FRAMEGETTIME();
	if( (DWORD)time + 300000 < FRAMEGETTIME() || iRefreshType != RT_PROCESS )
	{
		time = FRAMEGETTIME();
		DWORD dwProcessID = 0;
		::GetWindowThreadProcessId( m_hWnd, &dwProcessID );

		HANDLE hProc;
		PROCESS_MEMORY_COUNTERS pMc;
		if( hProc = OpenProcess( PROCESS_ALL_ACCESS, FALSE, dwProcessID ) )
		{
			GetProcessMemoryInfo( hProc, &pMc, sizeof( pMc ) );
		}
		::CloseHandle( hProc );
		
		// ���� ������� �޸� ��뷮
		DWORD dwCount	= 0;
		DWORD dwMem		= 0;
		DWORD dwTotal	= 0;

		switch( iRefreshType )
		{
		case RT_PROCESS:
			{
				g_MemoryLOG.PrintTimeAndLog(0, "-[Process Log]--------------------------------------------------------" );
			}
			break;
		case RT_ADD:
			{
				g_MemoryLOG.PrintTimeAndLog(0, "-[Add User Log]---------------------------------------------------------" );
			}
			break;
		case RT_DELETE:
			{
				g_MemoryLOG.PrintTimeAndLog(0, "-[Remove User Log]---------------------------------------------------------" );
			}
			break;
		}
		g_MemoryLOG.PrintTimeAndLog( 0, "%-20s %-10s %-10s", "Resource Type", "Count", "Memory(KB)" );

		g_MeshMgr.GetMemStatics( dwCount, dwMem );		
		g_MemoryLOG.PrintTimeAndLog( 0, "%-20s %-10d %-10d", "Mesh", dwCount, dwMem/1024 );
		dwTotal += dwMem;

		g_TextureMgr.GetMemStatics( dwCount, dwMem );
		g_MemoryLOG.PrintTimeAndLog( 0, "%-20s %-10d %-10d", "Texture", dwCount, dwMem/1024 );		
		dwTotal += dwMem;

		g_AnimateFXMgr.GetMemStatics( dwCount, dwMem );
		g_MemoryLOG.PrintTimeAndLog( 0, "%-20s %-10d %-10d", "AnimateFx", dwCount, dwMem/1024 );		
		dwTotal += dwMem;

		g_AnimationEventManager.GetMemStatics( dwCount, dwMem );
		g_MemoryLOG.PrintTimeAndLog( 0, "%-20s %-10d %-10d", "AnimationEvent", dwCount, dwMem/1024 );		
		dwTotal += dwMem;

		g_AnimationMgr.GetMemStatics( dwCount, dwMem );
		g_MemoryLOG.PrintTimeAndLog( 0, "%-20s %-10d %-10d", "Animation", dwCount, dwMem/1024 );		
		dwTotal += dwMem;

		g_ShaderMgr.GetMemStatics( dwCount, dwMem );
		g_MemoryLOG.PrintTimeAndLog( 0, "%-20s %-10d %-10d", "Shader", dwCount, dwMem/1024 );		
		dwTotal += dwMem;

		ioSkeletonManager::GetSingleton().GetMemStatics( dwCount, dwMem );		
		g_MemoryLOG.PrintTimeAndLog( 0, "%-20s %-10d %-10d", "Skeleton", dwCount, dwMem/1024 );		
		dwTotal += dwMem;

		g_SoundMgr.GetMemStatics( dwCount, dwMem );
		g_MemoryLOG.PrintTimeAndLog( 0, "%-20s %-10d %-10d", "Sound", dwCount, dwMem/1024 );		
		dwTotal += dwMem;

		DWORD dwTempleteCount	= 0;
		DWORD dwTempleteMem		= 0;
		DWORD dwCloneCount		= 0;
		DWORD dwCloneMem		= 0;
		g_EffectFactory.GetMemStatics( dwTempleteCount, dwTempleteMem, dwCloneCount, dwCloneMem );
		g_MemoryLOG.PrintTimeAndLog( 0, "%-20s %-10d %-10d", "Effect", dwTempleteCount+dwCloneCount, (dwTempleteMem+dwCloneMem)/1024 );
		g_MemoryLOG.PrintTimeAndLog( 0, "%-20s %-10d %-10d", " ��Templete", dwTempleteCount, dwTempleteMem/1024 );
		g_MemoryLOG.PrintTimeAndLog( 0, "%-20s %-10d %-10d", " ��Clone", dwCloneCount, dwCloneMem/1024 );
		dwTotal += dwTempleteMem;
		dwTotal += dwCloneMem;

		g_GUIMgr.GetGUIMemStatics( dwCount, dwMem );
		g_MemoryLOG.PrintTimeAndLog( 0, "%-20s %-10d %-10d", "GUI", dwCount, dwMem/1024 );		
		dwTotal += dwMem;

		g_GUIMgr.GetFrameMemStatics( dwCount, dwMem );
		g_MemoryLOG.PrintTimeAndLog( 0, "%-20s %-10d %-10d", "Frame", dwCount, dwMem/1024 );
		dwTotal += dwMem;

		g_GUIMgr.GetImageSetMemStatics( dwCount, dwMem );
		g_MemoryLOG.PrintTimeAndLog( 0, "%-20s %-10d %-10d", "ImageSet", dwCount, dwMem/1024 );
		dwTotal += dwMem;

		//io2DImageManager::GetSingleton().GetMemStatics( dwCount, dwMem );
		//LOG.PrintTimeAndLog( 0, "%-15s %-10d, %-10d", "2DImage", dwCount, dwMem/FLOAT1000 );
		//dwTotal += dwMem;

		//ioOpcodeManagerImpl::GetSingleton().GetMemStatics( dwCount, dwMem );
		//LOG.PrintTimeAndLog( 0, "%-15s %-10d, %-10d", "ioOpcode", dwCount, dwMem/FLOAT1000 );
		//dwTotal += dwMem;

		DWORD dwException = 0;
		ioINIMemoryStatic::GetSingleton().GetExceptionMemStatics( dwException );
		ioINIMemoryStatic::GetSingleton().GetMemStatics( dwCount, dwMem );
		g_MemoryLOG.PrintTimeAndLog( 0, "%-20s %-10d %-10d", "INI", dwCount, dwMem/1024 );
		dwTotal += dwMem;

		DWORD dwPacTot = 0;
		for( vPackFileSize::iterator iter = m_vPackFileSize.begin(); iter != m_vPackFileSize.end(); ++iter )
		{
			dwPacTot += iter->second;
			g_MemoryLOG.PrintTimeAndLog( 0, "%-20s %-10d %-10d", iter->first.c_str(), 1, iter->second/1024 );
		}		

		DWORD dwWorkingSet = static_cast<DWORD>( pMc.WorkingSetSize ) / 1024;
		g_MemoryLOG.PrintTimeAndLog(0, "-----------------------------------------------------------------------" );
		g_MemoryLOG.PrintTimeAndLog(0, "%-35s : %-7d KB (%d MB)", "Current Usage Memory(WorkingSet)", dwWorkingSet, dwWorkingSet/1024 );
		g_MemoryLOG.PrintTimeAndLog(0, "%-35s : %-7d KB (%d MB)", "Current Usage Memory(Resource)", dwTotal/1024, dwTotal/(1024*1024) );
		g_MemoryLOG.PrintTimeAndLog(0, "%-35s : %-7d KB (%d MB)", "Current Usage Memory(Statics)", dwException/1024, dwException/(1024*1024) );
		g_MemoryLOG.PrintTimeAndLog(0, "%-35s : %-7d KB (%d MB)", "Current Usage Memory(Pac)", dwPacTot/1024, dwPacTot/(1024*1024) );
		g_MemoryLOG.PrintTimeAndLog(0, "-----------------------------------------------------------------------" );
	}
}


void ioApplication::ChangeSwitch()
{
	bool bCheck = false;
#ifndef _DEBUG
	if( Help::CheckEnableTimeGapUser(1)|| g_App.IsMeDeveloper() )
	{
		bCheck = true;
	}
#else
	bCheck = true;
#endif
	
	if( bCheck )
	{
		g_MeshMgr.SetSwitch( SATICS_ON );
		g_TextureMgr.SetSwitch( SATICS_ON );
		g_AnimateFXMgr.SetSwitch( SATICS_ON );
		g_AnimationEventManager.SetSwitch( SATICS_ON );
		g_AnimationMgr.SetSwitch( SATICS_ON );
		g_ShaderMgr.SetSwitch( SATICS_ON );
		ioSkeletonManager::GetSingleton().SetSwitch( SATICS_ON );
		g_SoundMgr.SetSwitch( SATICS_ON );
		g_EffectFactory.SetSwitch( SATICS_ON );
		g_GUIMgr.SetSwitch( SATICS_ON );
		//io2DImageManager::GetSingleton().SetSwitch( SATICS_ON );
		//ioOpcodeManagerImpl::GetSingleton().SetSwitch( SATICS_ON );
	}
}

#ifdef XTRAP

__declspec(noinline) unsigned int Themida_Check_Code_Integrity(void)
{
	unsigned int uCheckCode = 0;
	VM_START
		CHECK_CODE_INTEGRITY(uCheckCode, 01/*����� 01~10*/);
	VM_END

	return uCheckCode;
}

class COptimizationPrevent 
{ 
public : COptimizationPrevent(){ Themida_Check_Code_Integrity(); } 
} DontUseThisVariable;


#endif


void CheckPacExceptionLog()
{
	static bool bException = false;
	if( bException )
		return;

	if( ErrorReport::IsPacException() )
	{
		if( strcmp( ErrorReport::GetPacExceptionLog(), "" ) == 0 )
			ErrorReport::SetPacExeptionInfo( "Unknown file name" );

		LOG.PrintTimeAndLog( 0, "%s", ErrorReport::GetPacExceptionLog() );

		wsprintf( g_szErrorPos, "%s/%s/%s : %s", g_MyInfo.GetMyIP().c_str(),
										  	     g_MyInfo.GetPrivateID().c_str(),
											     g_MyInfo.GetPublicID().c_str(),
											     ErrorReport::GetPacExceptionLog() );

		SP2Packet kPacket( LUPK_LOG );
		kPacket << "CheckError";  // �α� ���� Ÿ��
		kPacket << g_szErrorPos;
		kPacket << 313;  // ������ȣ:313
		kPacket << true; // write db
		P2PNetwork::SendLog(kPacket);
		bException = true;

		Help::CreateDefaultSettingINI();

		// check autoupgrade
		ioINILoader_e kIniLoder("Save\\default\\setting.ini",false);
		kIniLoder.SetTitle_e("Setting");
		kIniLoder.SaveBool( "PacDamage" , true );
		//
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );

		// ����ÿ� �Ǵٽ� �ȳ���.
		g_bHappenError = true;
		g_iErrorNumber = 313;
		SafeSprintf( g_szErrorLog, sizeof(g_szErrorLog), STR(2) );

		//UJ 140330, ���Ȱ�ȭ: �ǰ� ���� ġ�� ����, USB ��ũ���� �⵿���� ��ũ�� �����ؼ� 313 ������ ����
		// ����â �ݰ� �÷��̰� �����ؼ� �޽� �����Ͱ� ���� ä�� ������ �ε��ǰ� ��� ����� �����ϴٰ���

#if defined( USE_GA )
		if( g_App.GetGAStart() == true )
		{
			char chLabel[32] = {0,};

			if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
				SafeSprintf( chLabel, sizeof(chLabel), STR(1), 313 );
			else
				SafeSprintf( chLabel, sizeof(chLabel), "%1", 313 );

			// GAME_END_ERR_POS
			g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
				, "Game"
				, "Error"
				, chLabel
				, 1
				, "%2FGAME%2FOVER%2FERR"
				, 1 );

			g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FGAME%2FOVER%2FERR", 6, chLabel );
		}		
#endif

		g_App.SetExitProgram();
	}
}

///////////////////////////////////////////////////////////

extern "C" int _LOGPrint(lua_State *L)
{
	cLua *pLua = g_App.GetLua();
	LOG.PrintTimeAndLog( 0, "%s - %f", (char *)pLua->GetStringArgument( 1 ),
										(float)pLua->GetNumberArgument( 2 ) );
	return 0;
}

extern "C" int _RunScript(lua_State *L)
{
	cLua *pLua = g_App.GetLua();

	std::string szScriptName = pLua->GetStringArgument( 1 );
	
	if( g_ResourceLoader.IsLoadInPack() )
	{
		std::string szPath = "Scripts\\";
		szPath += szScriptName;

		ioBinaryStream kStream;
		if( !g_ResourceLoader.LoadStream( szPath.c_str(), &kStream) )
		{
			LOG.PrintTimeAndLog( 0, "_RunScript - %s Load Failed", szPath.c_str());
			return 0;
		}
				
		if( !pLua->RunMemory( (const char*)kStream.GetPtr() ,kStream.GetSize()) )
		{
			LOG.PrintTimeAndLog( 0, "_RunScript - RunMemory Fail", szPath.c_str() );
		}
	}
	else
	{
		if( !pLua->RunScript( szScriptName.c_str() ) )
		{
			LOG.PrintTimeAndLog( 0, "_RunScript - %s RunScript Fail", szScriptName.c_str() );
		}
	}
	return 0;
}

luaDef myGlue[] = 
{
  {"LOGPrint",		_LOGPrint},
  {"RunScript",		_RunScript},
  {NULL,			NULL},
};


// ������ ���̵𿡸� 30�� ������ ���̼����� ���� ������ �˾�â���� ���		JCLEE 140718
#if defined( SRC_OVERSEAS )
void ioApplication::CheckLicenseForDev()
{
	if( g_App.IsMeDeveloper() || g_App.IsMeGameMaster() )
	{
		ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
		if( pLocal )
		{
			int iLicenseDate, iLocalDate = 0;
			SYSTEMTIME sysTimeLicense, sysTimeLocal;

			// ���̼��� ��¥
			iLicenseDate			= pLocal->GetLicenseDate();
			sysTimeLicense.wYear	= iLicenseDate / 10000;
			sysTimeLicense.wMonth	= ( iLicenseDate - sysTimeLicense.wYear * 10000 ) / FLOAT100;
			sysTimeLicense.wDay		= ( iLicenseDate - sysTimeLicense.wYear * 10000 ) % 100;

			// ���� ���� ��¥
			GetLocalTime( &sysTimeLocal );
			iLocalDate = sysTimeLocal.wYear * 10000 + sysTimeLocal.wMonth * FLOAT100 + sysTimeLocal.wDay;

			// 30�� ������ ��� �޼���
			if( 30 >= DateHelp::GetDatePeriod( sysTimeLocal.wYear, sysTimeLocal.wMonth, sysTimeLocal.wDay, 0, 0 
				, sysTimeLicense.wYear, sysTimeLicense.wMonth, sysTimeLicense.wDay, 0, 0, DateHelp::PT_DAY ) )
			{
				char chTemp[256] = {0,};
				sprintf_s( chTemp, "Warning!!   License Date : %d, Local Date : %d", iLicenseDate, iLocalDate );

				g_GUIMgr.SetMsgBox( MB_OK, NULL, chTemp );
			}			
		}
	}
}

#endif
void ioApplication::OnPopupOpenIndex( SP2Packet& rkPacket )
{
	static bool bFirst = false;
	if( !bFirst )
	{
		CheckFolder();

		bFirst = true;
		char fName[MAX_PATH]={0,};
		char szKey[MAX_PATH]={0,};
		ioHashString rkPublicID = g_MyInfo.GetPublicID();
		StringCbPrintf_e( fName, sizeof(fName), "Save\\%s\\popup.ini",rkPublicID.c_str() );
		ioINILoader kLoader( fName, false );
		kLoader.SetTitle_e( "Common" );

		int nSize = kLoader.LoadInt( "MaxOpenPopup", 0 );
		if( nSize != 0)
		{
			for( int i = 0; i < nSize; ++i )
			{
				sprintf( szKey, "PopupIndex%d",i );
				int iIndex = kLoader.LoadInt( szKey, 0 );
				if( iIndex != 0 )
					m_vecOpenPopupIndex.push_back( iIndex );
			}
		}
	}

	PopupStoreWnd* pPopupStoreWnd = dynamic_cast<PopupStoreWnd*>( g_GUIMgr.FindWnd( POPUP_STORE_WND ) );
	if( !pPopupStoreWnd )
		pPopupStoreWnd = dynamic_cast<PopupStoreWnd*>( g_GUIMgr.AddWnd( "xml/popupstore.xml" ) );

	if( !pPopupStoreWnd )
	{
		LOG.PrintTimeAndLog( 0, "[err] PopupStore AddWnd Fail." );
		return;
	}

	if( pPopupStoreWnd )
		pPopupStoreWnd->OpenPopup( GetActivePopupIndex( rkPacket ) );
}

void ioApplication::OnPopupItemBuyResult( SP2Packet& rkPacket )
{
	DWORD dwReturnType;
	rkPacket >> dwReturnType;

	__int64 iiMoney;
	int iCash, iPurchasedCash, iChannelingCash;
	rkPacket >> iiMoney >> iCash >> iPurchasedCash >> iChannelingCash;


	TCPNetwork::MouseBusy(false);

	PopupStoreWnd* pPopupStoreWnd = dynamic_cast<PopupStoreWnd*>( g_GUIMgr.FindWnd( POPUP_STORE_WND ) );
	if( pPopupStoreWnd )
	{
		if( dwReturnType == POPUP_ITEM_BUY_SUCCESS )
			pPopupStoreWnd->OnBuySuccess();
		else
			pPopupStoreWnd->OnButFaild( dwReturnType );
	}

	g_MyInfo.SetMoney( iiMoney );
	g_MyInfo.SetCash( iCash );
	g_MyInfo.SetPurchasedCash( iPurchasedCash );
	g_MyInfo.SetChannelingCash( iChannelingCash );
}

int ioApplication::GetActivePopupIndex( SP2Packet& rkPacket )
{
	CheckTodayPopupTime();

	int iPopupIndex = 0;
	int iActivePopupList = 0;
	rkPacket >> iActivePopupList;
	for( int i = 0; i < iActivePopupList; ++i )
	{
		int iIndex = 0;
		rkPacket >> iIndex;

		bool bFind = false;
		int iOpenPopSize = m_vecOpenPopupIndex.size();
		for( int j = 0; j < iOpenPopSize; ++j )
		{
			//popup
			//�׽�Ʈ������ ������ üũ ����
			if( iIndex == m_vecOpenPopupIndex[j] )
			{
				bFind = true;
				break;
			}
		}
		if( !bFind )
		{
			iPopupIndex = iIndex;
			break;
		}
	}

	return iPopupIndex;
}

void ioApplication::SaveOpenPopupIndex( int iIndex )
{
	//test
	bool bFind = false;
	int nSize = m_vecOpenPopupIndex.size();
	for( int i = 0; i < nSize; ++i )
	{
		if( m_vecOpenPopupIndex[i] == iIndex )
		{
			bFind = true;
			break;
		}
	}
	if( !bFind )
	{
		m_vecOpenPopupIndex.push_back( iIndex );

		char fName[MAX_PATH]={0,};
		char szKey[MAX_PATH]={0,};
		ioHashString rkPublicID = g_MyInfo.GetPublicID();
		StringCbPrintf_e( fName, sizeof(fName), "Save\\%s\\popup.ini",rkPublicID.c_str() );
		ioINILoader kLoader( fName, false );
		kLoader.SetTitle_e( "Common" );

		int nSize = m_vecOpenPopupIndex.size();
		kLoader.SaveInt( "MaxOpenPopup", nSize );
		for( int i = 0; i < nSize; ++i )
		{
			sprintf( szKey, "PopupIndex%d",i );
			kLoader.SaveInt( szKey, m_vecOpenPopupIndex[i] );
		}
	}
}

void ioApplication::CheckTodayPopupTime()
{
	CheckFolder();

	ioHashString rkPublicID = g_MyInfo.GetPublicID();
	char fName[MAX_PATH]="";
	StringCbPrintf_e( fName, sizeof(fName), "Save\\%s\\popup.ini",rkPublicID.c_str() );
	ioINILoader kLoader( fName, false );
	kLoader.SetTitle_e( "Common" );

	CTime cTime = CTime::GetCurrentTime();
	DWORD dwCurrentTime = ( ( cTime.GetYear() - 2000 ) * 100000000 ) + ( cTime.GetMonth() * 1000000 ) + 
		( cTime.GetDay() * 10000 ) + ( cTime.GetHour() * 100 ) + cTime.GetMinute();

	dwCurrentTime /= 10000;

	bool bClear = false;
	DWORD dwSaveTime = (DWORD)kLoader.LoadInt( "ReadDay", 0 );
	if( dwSaveTime == 0 )
	{
		bClear = true;
	}
	else
	{
		if( dwCurrentTime != dwSaveTime )
			bClear = true;
	}

	if( bClear )
		m_vecOpenPopupIndex.clear();

	kLoader.SaveInt( "ReadDay", dwCurrentTime );	
}

void ioApplication::CheckFolder()
{
	//���⼭ �ð� �������..	
	ioHashString rkPublicID = g_MyInfo.GetPublicID();
	char fName[MAX_PATH]="";
	StringCbPrintf_e( fName, sizeof(fName), "Save\\%s\\popup.ini",rkPublicID.c_str() );

	FILE *fp = fopen( fName, "r" );
	if( !fp )
	{
		sprintf( fName, "Save\\%s" , rkPublicID.c_str() );
		CreateDirectory("Save",NULL);
		CreateDirectory(fName,NULL);
		sprintf(fName,"Save\\%s\\popup.ini", rkPublicID.c_str() );

		fp = fopen( fName, "w" );
		if( !fp )
		{
			LOG.PrintTimeAndLog( 0, "popup.ini File Create Failed." );
			return;
		}
	}

	fclose( fp );
}

void ioApplication::OnPenalty( SP2Packet& rkPacket )
{
	DWORD dwPenalty = 0;
	BYTE byType = 0;
	rkPacket >> byType;
	rkPacket >> dwPenalty;

	ioPlayStage* pPlayStage = ToPlayStage( m_pGameStage );
	if( !pPlayStage ) return;
	ioPlayMode* pPlayMode = pPlayStage->GetPlayMode();
	if( !pPlayMode ) return;
	ioBaseChar* pChar = pPlayMode->GetOwnerChar();
	if( !pChar ) return;

	DWORD dwDuration = 3000;
	if( byType == 2 )
		dwDuration = 10000;

	pChar->SetNoInputEmoticon(dwDuration);
	pChar->StartNoInputDelayTime(dwDuration);

	return;
}


#if defined( USE_GA )

void ioApplication::SetGAEtcSubInfo( int iSubCode, int iCash )
{
	if( iSubCode == 0 || iCash == 0 )
		return;

	stGAEtcInfo GAEtcInfo;
	GAEtcInfo.iSubCode	= iSubCode;
	GAEtcInfo.iCash		= iCash;

	m_vecGAEtcInfo.push_back( GAEtcInfo );
}


void ioApplication::SetGAEtcMainInfo( int iMainCode )
{
	if( iMainCode == 0 || m_vecGAEtcInfo.size() == 0 )
		return;

	GAEtcInfoVec vecEtcInfo = m_vecGAEtcInfo;

	m_mapGAEtcInfo.insert( GAEtcInfoMap::value_type( iMainCode, vecEtcInfo ) );

	m_vecGAEtcInfo.clear();
}


int ioApplication::GetGAEtcCash( int iMainCode, int iSubCode )
{
	GAEtcInfoMap::iterator iter = m_mapGAEtcInfo.find( iMainCode );
	if( iter != m_mapGAEtcInfo.end() )
	{
		GAEtcInfoVec vecEtcInfo = iter->second;

		GAEtcInfoVec::iterator iter2 = vecEtcInfo.begin();
		for( ; iter2 != vecEtcInfo.end(); ++iter2 )
		{
			if( iter2->iSubCode == iSubCode )
			{
				return iter2->iCash;
			}
		}
	}

	return 0;
}


int ioApplication::GetGAEtcCash2( int iMainCode, int iNum )
{
	GAEtcInfoMap::iterator iter = m_mapGAEtcInfo.find( iMainCode );
	if( iter != m_mapGAEtcInfo.end() )
	{
		GAEtcInfoVec vecEtcInfo = iter->second;

		GAEtcInfoVec::iterator iter2 = vecEtcInfo.begin() + iNum;
		return iter2->iCash;
	}

	return 0;
}

#endif

void ioApplication::SetRegHotKey( HWND hWnd )
{
	m_HotKeyIDList.clear();

	int nHotKeyID;
	nHotKeyID = AddAtom( ID_HOTKEY_PS );
	RegisterHotKey(hWnd, nHotKeyID, 0, VK_SNAPSHOT);
	m_HotKeyIDList.push_back( nHotKeyID );

	nHotKeyID = AddAtom( ID_HOTKEY_Ctrl_PS );
	RegisterHotKey(hWnd, nHotKeyID,	MOD_CONTROL, VK_SNAPSHOT);
	m_HotKeyIDList.push_back( nHotKeyID );

	nHotKeyID = AddAtom( ID_HOTKEY_Shift_PS );
	RegisterHotKey(hWnd, nHotKeyID,	MOD_SHIFT, VK_SNAPSHOT);
	m_HotKeyIDList.push_back( nHotKeyID );

	nHotKeyID = AddAtom( ID_HOTKEY_Win_PS );
	RegisterHotKey(hWnd, nHotKeyID,	MOD_WIN, VK_SNAPSHOT);
	m_HotKeyIDList.push_back( nHotKeyID );

	nHotKeyID = AddAtom( ID_HOTKEY_Alt_PS );
	RegisterHotKey(hWnd, nHotKeyID,	MOD_ALT, VK_SNAPSHOT);
	m_HotKeyIDList.push_back( nHotKeyID );

	nHotKeyID = AddAtom( ID_HOTKEY_Ctrl_Shift_PS );
	RegisterHotKey(hWnd, nHotKeyID,	MOD_CONTROL | MOD_SHIFT, VK_SNAPSHOT);
	m_HotKeyIDList.push_back( nHotKeyID );

	nHotKeyID = AddAtom( ID_HOTKEY_Ctrl_Win_PS );
	RegisterHotKey(hWnd, nHotKeyID,	MOD_CONTROL | MOD_WIN, VK_SNAPSHOT);
	m_HotKeyIDList.push_back( nHotKeyID );
}

void ioApplication::SetUnRegHotKey( HWND hWnd )
{
	for ( int i=0; i<(int)m_HotKeyIDList.size(); i++ )
	{
		UnregisterHotKey(hWnd, m_HotKeyIDList[i]);
		DeleteAtom( m_HotKeyIDList[i] );
	}
	m_HotKeyIDList.clear();
}

bool ioApplication::ApplyHotKey( WPARAM wParam )
{
	for ( int i=0; i<(int)m_HotKeyIDList.size(); i++ )
	{
		if ( wParam == m_HotKeyIDList[i] )
		{
			UNREFERENCED_PARAMETER( wParam );
			return true;
		}
	}

	return false;
}

void ioApplication::OnRSoldierInfo( SP2Packet& rkPacket )
{
	int iState = 0;
	rkPacket >> iState;
	ioHashString szName;
	rkPacket >> szName;

	if ( iState == RSOLDIER_BUY )
	{
		ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
		if( pPlayStage )
		{
			ioBaseChar *pChar = pPlayStage->GetBaseChar( szName );
			if ( pChar )
			{
				int iCount = pChar->GetCurRSoldierCount();
				iCount++;
				pChar->SetCurRSoldierCount( iCount );

				if ( szName == g_MyInfo.GetPublicID() )
					g_MyInfo.SetRSoldierPossessionCount( iCount );
			}
		}

		
	}
	else if ( iState == RSOLDIER_SELL || iState == RSOLDIER_DISASSEMBLE )
	{
		ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
		if( pPlayStage )
		{
			ioBaseChar *pChar = pPlayStage->GetBaseChar( szName );
			if ( pChar )
			{
				int iCount = pChar->GetCurRSoldierCount();
				iCount--;
				iCount = max( 0, iCount );
				pChar->SetCurRSoldierCount( iCount );

				if ( szName == g_MyInfo.GetPublicID() )
					g_MyInfo.SetRSoldierPossessionCount( iCount );
			}
		}
	}	
}


void ioApplication::CheckUDPData( SP2Packet& kPacket )
{
#ifndef UDP_PING_CHECK
	return;
#endif
	DWORD dwPacketID = kPacket.GetPacketID();
	DWORD dwUDPIndex = kPacket.GetUDPIndex();

	kPacket.InitCurPos();

	DWORD dwIP,dwPort;
	kPacket >> dwIP >> dwPort;

	if( dwUDPIndex != m_iNextUDPSendIndex )
		LOG.PrintTimeAndLog( 0, "[ping][udp] checkudpIndex error - %u-%d", dwUDPIndex, m_iNextUDPSendIndex );

	sUDPData sInfo;
	sInfo.dwUDPIndex = dwUDPIndex;
	sInfo.dwPacketID = dwPacketID;
	sInfo.dwTime = REALGETTIME();
	m_vecUDPData.push_back( sInfo );

	//ping test
	//LOG.PrintTimeAndLog( 0, "[ping][test] push udp(%u), ID(%x), IP(%u), Port(%u)", dwUDPIndex, dwPacketID, dwIP, dwPort );
}

bool ioApplication::ParseUDPData( int iUDPIndex, DWORD dwPacketID )
{
#ifndef UDP_PING_CHECK
	return false;
#endif
	//ping test
	//LOG.PrintTimeAndLog( 0, "[ping][test] parse udp(%d), ID(%x)", iUDPIndex, dwPacketID  );

	//Ư�� ��Ŷ�� ���� ��ƾ����...
	if( dwPacketID == CUPK_SYNCTIME || 
		dwPacketID == CUPK_RESERVE_ROOM_JOIN || 
		dwPacketID == CUPK_CHECK_KING_PING )
	{
		LOG.PrintTimeAndLog( 0, "[ping][test] parse udp error(%d), ID(%x)", iUDPIndex, dwPacketID  );
		return false;
	}

	bool bFind = false;
	int nSize = m_vecUDPData.size();
	for( int i = 0; i < nSize; ++i )
	{
		if( m_vecUDPData[i].dwUDPIndex == iUDPIndex )
		{
			bFind = true;
			m_vecUDPData[i].dwCheckTime = REALGETTIME() - m_vecUDPData[i].dwTime;
			break;
		}
	}

	//if( !bFind )
		//LOG.PrintTimeAndLog( 0, "[App][UDP] ParseUDPData Error UDPIndex(%d),ID(%x),Size(%u)", iUDPIndex, dwPacketID, m_vecUDPData.size() );

	if( dwPacketID == CUPK_PENGUIN_PING )
		return false;

	return true;
}

void ioApplication::ProcessUDPInfo()
{
	//1�п� �ѹ��� ����
	DWORD dwCurTime = REALGETTIME();
	if( (m_dwUDPDataCheckTime+60000) > dwCurTime )
		return;
	m_dwUDPDataCheckTime = dwCurTime;
	if( m_vecUDPData.empty() )
		return;

	
	// 1. ��� �ð� ���� Ŭ ��� ���
	// 2. ������� ������ ������ ����
	// 3. �ս�üũ �� ����.. �սǵȰ� �������! �ð� ���� �Ȱ� �׳� ������..
	DWORD dwTotalAverageTime = 0;
	if( !m_vecUDPInfo.empty() )
	{
		int nSize = m_vecUDPInfo.size();
		for( int i = 0; i < nSize; ++i )
		{
			dwTotalAverageTime += m_vecUDPInfo[i].dwAverageTime;
		}

		dwTotalAverageTime /= nSize;
		//��� �����Ͻú��� 3���̻� Ŭ ��� ����� �ɵ�?
		dwTotalAverageTime *=3;
	}	
	DWORD dwEcoTime = 0;
	DWORD dwCount = 0;
	DWORD dwAverageTime = 0;
	DWORD dwMaxTime = 0;
	int nSize = m_vecUDPData.size();
	for( int i = 0; i < nSize; ++i )
	{
		if( m_vecUDPData[i].dwCheckTime != 0 && !m_vecUDPData[i].bCheck )
		{
			m_vecUDPData[i].bCheck = true;
			dwEcoTime = m_vecUDPData[i].dwCheckTime;
			dwAverageTime += dwEcoTime;
			++dwCount;
			
			//��պ��� 3�� ũ�鼭 100ms �̻��� ��츸 ������
			if( dwTotalAverageTime > 0 && dwEcoTime > dwTotalAverageTime && dwEcoTime > 100 )
			{
				//LOG.PrintTimeAndLog( 0, "[PING][WARNNING] PingTime Over: ID(%x), Ping(%u)", m_vecUDPData[i].dwPacketID, dwEcoTime );
				//m_vecUDPDataErr.push_back( m_vecUDPData[i] );
			}

			if( dwMaxTime < dwEcoTime )
				dwMaxTime = dwEcoTime;
		}
	}
	
	if( dwCount != 0 )
	{
		dwAverageTime /= dwCount;
	}
	
	//last check index
	DWORD dwLastCheck = 0;
	for( int i = nSize-1; i >= 0; --i )
	{
		if( m_vecUDPData[i].bCheck )
		{
			dwLastCheck = i;
			break;
		}
	}
	
	//udp �����Ǽ� �� ��츦 ����ؼ�.. 300ms ���� �������
	int iEraseIndex = -1;
	static std::vector<DWORD> vecLossCheck;
	vecLossCheck.reserve(50);
	for( DWORD i = 0; i < dwLastCheck; ++i )
	{
		if( !m_vecUDPData[i].bCheck )
		{
			//�����ð� üũ 300ms ���� ũ�� �׳� ����..
			DWORD dwDiffTime = dwCurTime - m_vecUDPData[i].dwTime;
			if( dwDiffTime > 300 )
			{
				iEraseIndex = i;
				vecLossCheck.push_back( i );
				//LOG.PrintTimeAndLog( 0, "[PING][WARNNING] udppacketloss udpindex(%u), pkID(%x)", m_vecUDPData[i].dwUDPIndex, m_vecUDPData[i].dwPacketID );
				//LOSS ���� ������
			}
			else
			{
				// 300ms ���ϸ� �������� üũ����
				break;
			}
		}
		else
			iEraseIndex = i;
	}

	sUDPInfo sInfo;
	sInfo.dwAverageTime = dwAverageTime;
	sInfo.dwTimeIndex = m_vecUDPInfo.size();
	sInfo.dwMaxTime = dwMaxTime;
	sInfo.dwCount = dwCount;
	sInfo.dwLossCount = vecLossCheck.size();
	vecLossCheck.clear();

	m_vecUDPInfo.push_back( sInfo );

	LOG.PrintTimeAndLog( 0, "[PING][INFO] PingCheckTime:%d, Count(%u),AverageTime(%u),MaxTime(%u),LossCnt(%u)",
		sInfo.dwTimeIndex, sInfo.dwCount, sInfo.dwAverageTime, sInfo.dwMaxTime, sInfo.dwLossCount );

	if( iEraseIndex != -1 )
	{
		//�ϴ� Ŭ����
		if( iEraseIndex == (nSize-1) )
		{
			m_vecUDPData.clear();
		}
		else
		{
			m_vecUDPData.erase( m_vecUDPData.begin(), m_vecUDPData.begin() + iEraseIndex+1 );
		}
	}
}


void ioApplication::LogGlobalMemory()
{
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof (statex);
	GlobalMemoryStatusEx (&statex);

	LOG.PrintTimeAndLog(0, ("There is  %d percent of memory in use."),statex.dwMemoryLoad);
	LOG.PrintTimeAndLog(0, ("There are %d total Mbytes of physical memory."),statex.ullTotalPhys/1048576);
	LOG.PrintTimeAndLog(0, ("There are %d free Mbytes of physical memory."), statex.ullAvailPhys/1048576);
	LOG.PrintTimeAndLog(0, ("There are %d total Mbytes of paging file."), statex.ullTotalPageFile/1048576);
	LOG.PrintTimeAndLog(0, ("There are %d free Mbytes of paging file."), statex.ullAvailPageFile/1048576);
	LOG.PrintTimeAndLog(0, ("There are %d total Mbytes of virtual memory."), statex.ullTotalVirtual/1048576);
	LOG.PrintTimeAndLog(0, ("There are %d free Mbytes of virtual memory."), statex.ullAvailVirtual/1048576);
	LOG.PrintTimeAndLog(0, ("There are %d free Mbytes of extended memory."), statex.ullAvailExtendedVirtual/1048576);
}


#ifdef STEAM_ATTACH
void ioApplication::SteamEnable( ioHashString & val )
{
	if(val != "0")
	{
		m_bSteamEnable = true;

	}
	else
	{
		m_bSteamEnable = false;
	}
}
#endif

void ioApplication::LogP2PPacket()
{
	LOG.PrintTimeAndLog( 0, "P2P Packet Check - START" );
	auto it = m_setP2PPacket.begin();
	while( it != m_setP2PPacket.end() )
	{
		LOG.PrintTimeAndLog( 0, "[P2P] ID = %X", *it );
		++it;
	}
	LOG.PrintTimeAndLog( 0, "P2P Packet Check - END" );
}

#ifdef ANTIHACK
void ioApplication::InitRUDPList()
{
	//��� ����Ʈ�� ���� �߰�
	DWORD dwRUDPList[] = 
	{
		CUPK_CHAR_CONTROL,
		CUPK_USE_SKILL,
		CUPK_ANTIHACK_CHAR_HIT,
		CUPK_ANTIHACK_CHAR_WOUNDED,
		CUPK_ANTIHACK_CHAR_DEFENCE,
		CUPK_ANTIHACK_SKILL,
		CUPK_ANTIHACK_SKILL_EXTRA_INFO,
		CUPK_ANTIHACK_BULLET_INFO,
		CUPK_ANTIHACK_BULLET_RELOAD,
	};

	int rudpSize = sizeof(dwRUDPList);
	int rudpCount = rudpSize/4;
	for( int i = 0; i < rudpCount; ++ i )
		m_setRUDPList.insert( dwRUDPList[i] );
}
bool ioApplication::ISRUDPPacket( SP2Packet& kPacket )
{
	bool bRet = m_setRUDPList.find( kPacket.GetPacketID() ) == m_setRUDPList.end() ? false : true;
	return bRet;
}
void ioApplication::RUDPPreProcess( SP2Packet& kPacket, ioBaseChar* pBase )
{
	if( ISRUDPPacket( kPacket ) == false )
		return;

	// rudp ��Ŷ �տ��� seed, time, npcIndex 3���� �߰���	
	
	DWORD dwCurTime = REALGETTIME();
	DWORD dwSeed = 0;
	DWORD dwNPCIndex = 0;		// 0 �̸� user, ���� ������ npc

	if( ToNpcChar( pBase ) )
	{
		dwSeed = GetNextNPCSeed();
		dwNPCIndex = pBase->GetNPCIndex();
		//LOG.PrintTimeAndLog( 0, "[Seed] Send NPC Seed - %u NPC(%u)", dwSeed, dwNPCIndex );
	}
	else
	{
		dwSeed = GetNextUserSeed();
		//LOG.PrintTimeAndLog( 0, "[Seed] Send User Seed - %u",dwSeed );
	}

	kPacket.CreateRUDPPacket( dwSeed, dwCurTime, dwNPCIndex );

	RUDPInfo* pRI = m_deqRUDP.back();
	*pRI->pPacket = kPacket;
	pRI->dwSeed = dwSeed;
	pRI->dwNPCIndex = dwNPCIndex;
	pRI->dwLastSendTime = 0;
	m_deqRUDP.pop_back();
	m_deqRUDP.push_front( pRI );
}

int ioApplication::RUDPPostProcess( SP2Packet& kPacket, ioBaseChar** pBase, DWORD& dwIndex )
{
	if( ISRUDPPacket( kPacket ) == false )
		return -1;

	bool bUser = true;
	DWORD dwSeed = 0, dwTime = 0;
	kPacket >> dwSeed >> dwTime >> dwIndex;

	if( dwIndex == 0 )
		dwIndex = kPacket.GetUserIndex();
	else 
		bUser = false;	// ���� ������ npc
		

	// rudp üũ �κ��ε�, ġ���� ���ؼ��� ���������� ������ ������ ��
	// p2pnetwork::checkseed
	//return -1;

	ioPlayStage* pStage = ToPlayStage( GetGameStage() );
	if( !pStage )
		return 1;	
	
	ioBaseChar* pChar = pStage->GetBaseCharByUserIndex( dwIndex, bUser );
	if( !pChar )
		return 1;
	
	*pBase = pChar;

	ioHashString hsName = pChar->GetCharName();

	// ���� ó���� app ���� �ϴϱ� ���� p2pnetwork ����� �� Ÿ�� ��
	//���⼭ rudp ����� �ϴµ�, �ϴ� Ŭ���̾�Ʈ rudp�� ��� �� �ϴ°ɷ�
	bool bRet = CheckControlSeed( hsName, dwSeed, dwTime );

	//����
	return 1;
}

DWORD ioApplication::GetNextUserSeed()
{
	if( m_dwUserSeed == 0 )
		return 0;
	return ++m_dwUserSeed;
}

DWORD ioApplication::GetNextNPCSeed()
{
	if( m_dwNPCSeed == 0 )
		return 0;
	return ++m_dwNPCSeed;
}

void ioApplication::OnRudpServer( SP2Packet& rkPacket )
{
	DWORD dwSize;
	rkPacket >> dwSize;

	DWORD dwCurTime = REALGETTIME();
	static std::vector<DWORD> dwVecRUDPID;
	static std::vector<DWORD> dwVecRUDPNotUseID;
	dwVecRUDPNotUseID.clear();
	dwVecRUDPID.clear();

	for( DWORD i = 0; i < dwSize; ++i )
	{
		DWORD dwID;
		rkPacket >> dwID;
		dwVecRUDPID.push_back( dwID );

// 		if( s_RUDPLOG )
// 			EncLog( 0, "[RUDP][INFO] OnRUDPServer - %u", dwID );
	}

	for( DWORD i = 0; i < dwVecRUDPID.size(); ++i )
	{
		bool bFind = false;
		DWORD dwSeed = dwVecRUDPID[i];

		auto it = m_deqRUDP.begin();
		while( it != m_deqRUDP.end() )
		{
			RUDPInfo* pRI = *it;
			if( pRI->dwSeed == dwSeed )
			{
				//ã�� ���
				bFind = true;
				if( (dwCurTime - pRI->dwLastSendTime) > dwAckTimeInterval )
				{
					pRI->dwLastSendTime = dwCurTime;
					SP2Packet kPacket( "", 0, g_MyInfo.GetUserIndex(), m_iNextUDPSendIndex++, *pRI->pPacket );
					SendUDPMessageToServer(kPacket);
				}
				break;
			}
			else if( pRI->dwSeed < dwSeed )
				break;
			//���ķδ� ����
			else if( pRI->dwSeed == 0 )
				break;
			++it;
		}

		if( !bFind )
			dwVecRUDPNotUseID.push_back( dwSeed );
	}

	if( !dwVecRUDPNotUseID.empty() )
	{
		SP2Packet kPacket(CUPK_RUDP_NOT_USE_ID);
		int nSize = dwVecRUDPNotUseID.size();
		kPacket << nSize;
		for( int i = 0; i < nSize; ++i )
			kPacket << dwVecRUDPNotUseID[i];

		SP2Packet kPacketS( "", 0, g_MyInfo.GetUserIndex(), m_iNextUDPSendIndex, kPacket );
		SendUDPMessageToServer(kPacketS);
		m_iNextUDPSendIndex++;
	}
}

bool ioApplication::CheckControlSeed( const ioHashString &rkName, DWORD dwSeed, DWORD dwTime )
{
	// ���� Ŭ���̾�Ʈ�� RUDP ��� �� �ϰԲ�!
	return true;

	/*//reorder ��ƾ
	//�����ϸ� ���� ó�� ��ƾ
	if( m_pNetworkTable->CheckControlSeed( rkName, dwSeed, dwTime, true ) )
		return true;

	//������ ���!
	// ���̰� �ʹ� ���� �� ��� ���� ����. ���Ա����� ���� 1ȸ�� �̷��� �ɵ�?
	DWORD dwUpdateSeed = GetControlSeed( rkName, true );

	if( (dwSeed-dwUpdateSeed) >= RUDP_SKIP_MAX )
	{
		//��ŵ	
	}



	//req

	//push

	return false;*/
}

void ioApplication::GetReSkill( SP2Packet& kPacket, ioBaseChar* pBase )
{
	m_ReSkillPacket = kPacket;
	m_pBaseTestReSkill = pBase;
}

void ioApplication::UseReSkill()
{
	if( !m_pBaseTestReSkill )
		return;

	DWORD dwCurTime = REALGETTIME();
	static DWORD dwTime = 0;
	if( dwTime != 0 && dwTime+1000 > dwCurTime )
		return;
	dwTime = dwCurTime;

	LOG.PrintTimeAndLog( 0, "[app] TEST - UseReSkill" );

	SP2Packet kPacket = m_ReSkillPacket;
	SendToAllPlayingUser( kPacket, m_pBaseTestReSkill );
}

#endif

void ioApplication::AddClientMessage( SP2Packet& rkPacket )
{
	// �ϴ� �δϴ� ��� ���� ����

	DWORD dwPenalty = 0;
	BYTE byType = 0;
	rkPacket >> byType;
	rkPacket >> dwPenalty;

	ioPlayStage* pPlayStage = ToPlayStage( m_pGameStage );
	if( !pPlayStage ) return;
	ioPlayMode* pPlayMode = pPlayStage->GetPlayMode();
	if( !pPlayMode ) return;
	ioBaseChar* pChar = pPlayMode->GetOwnerChar();
	if( !pChar ) return;

	// ��ų ġ���� 10�� ����ġƮ�� 3��
	DWORD dwDuration = 3000;
	if( byType == 1 )
		dwDuration = 10000;

	pChar->SetNoInputEmoticon(dwDuration);
	pChar->StartNoInputDelayTime(dwDuration);

	return;


	/*//��ų
	if( byType == 0 )
	{		
		ioHashString hsString;
		rkPacket >> hsString;

		float fTime1,fTime2;
		rkPacket >> fTime1 >> fTime2;

		DWORD dwCurPenalty;
		rkPacket >> dwCurPenalty;
#ifdef _ITEST
		m_vPenaltyMsg.dwTime = REALGETTIME();
		char buf[MAX_PATH]={0,};
		sprintf_s( buf, "[Penalty] Test - curPenalty(%u), Time(%.2f/%.2f) message %s skill.", dwCurPenalty, fTime1, fTime2, hsString.c_str() );
		m_vPenaltyMsg.hsString = buf;
#endif
// 		sPrintMessage sMessage;
// 		sMessage.dwTime = REALGETTIME();
// 
// 		char buf[MAX_PATH]={0,};
// 		sprintf_s_e( buf, "[Penalty] Test - curPenalty(%u), Time(%.2f/%.2f) message %s skill.", dwCurPenalty, fTime1, fTime2, hsString.c_str() );
// 		sMessage.hsString = buf;
// 		sMessage.dwOffsetX = g_FontMgr.GetTextWidth( buf, TS_NORMAL, 1.f, 1.f ) * 0.5;
// 
// 		m_vecHSPrint.push_back( sMessage );


		
		return;
	}
	//anti hit
	else if( byType == 1 )
	{
		rkPacket >> dwPenalty;
	}
	else if( byType == 2 )
	{
#ifdef _ITEST
		rkPacket >> dwPenalty;
		m_vPenaltyMsg.dwTime = REALGETTIME();
		char buf[MAX_PATH]={0,};
		sprintf_s_e( buf, "[Penalty] Test - curPenalty(%u) RecvTime(%u) ", dwPenalty, m_vPenaltyMsg.dwTime );
		m_vPenaltyMsg.hsString = buf;
#endif
		return;
	}

	ioPlayStage* pPlayStage = ToPlayStage( m_pGameStage );
	if( !pPlayStage ) return;
	ioPlayMode* pPlayMode = pPlayStage->GetPlayMode();
	if( !pPlayMode ) return;
	ioBaseChar* pChar = pPlayMode->GetOwnerChar();
	if( !pChar ) return;

	DWORD dwDuration = 3000;
	pChar->SetNoInputEmoticon(dwDuration);
	pChar->StartNoInputDelayTime(dwDuration);*/
}

#ifdef _ITEST
void ioApplication::RenderClinetMessage()
{
	g_FontMgr.SetAlignType( TAT_LEFT );
	g_FontMgr.SetTextStyle( TS_SHADOW );
	g_FontMgr.SetTextColor( 255, 255, 0 );
	g_FontMgr.SetBkColor( 0, 0, 0 );

	if( g_bTestAntiHack )
		g_FontMgr.PrintText( 10.f, 10.f, 1.f, "AntiHackPacket Block On" );
	else
		g_FontMgr.PrintText( 10.f, 10.f, 1.f, "AntiHackPacket Block Off" );

	if( m_vPenaltyMsg.dwTime != 0 )
	{
		g_FontMgr.PrintText( 10.f, 50.f, 1.f, m_vPenaltyMsg.hsString.c_str() );
	}


	if( m_vecHSPrint.empty() )
		return;

	RECT rcRect;
	GetClientRect( m_hWnd, &rcRect );	

	int centerX = rcRect.right * 0.5;
	int centerY = rcRect.bottom * 0.5;

	g_FontMgr.SetAlignType( TAT_LEFT );
	g_FontMgr.SetTextStyle( TS_SHADOW );
	g_FontMgr.SetTextColor( 255, 255, 0 );
	g_FontMgr.SetBkColor( 0, 0, 0 );

	int dwSize = m_vecHSPrint.size();

	for( int i = 0; i < dwSize; ++i )
	{
		if( centerY < 50 * i  )
			break;

		g_FontMgr.PrintText( centerX - m_vecHSPrint[i].dwOffsetX, centerY - 50 * i, 1.f,
			m_vecHSPrint[i].hsString.c_str() );
	}


	DWORD dwCurTime = REALGETTIME();

	if( m_vecHSPrint[0].dwTime + 10000 < dwCurTime )
	{
		m_vecHSPrint.erase( m_vecHSPrint.begin() );
	}
}
void ioApplication::RenderPenaltyPoint()
{
	if( ioApplication::s_bPenaltyTest )
	{
		g_FontMgr.SetAlignType( TAT_LEFT );
		g_FontMgr.SetTextStyle( TS_SHADOW );
		g_FontMgr.SetTextColor( 255, 255, 0 );
		g_FontMgr.SetBkColor( 0, 0, 0 );

		if( m_vPenaltyMsg.dwTime != 0 )
		{
			g_FontMgr.PrintText( 10.f, 50.f, 1.f, m_vPenaltyMsg.hsString.c_str() );
		}
	}
}
#endif


void ioApplication::WasteBullet( DWORD dwItemCode )
{
#ifdef ANTIHACK
#if defined( SRC_ID ) || defined( SRC_SEA )
	m_dwLastBulletUpdateTime = REALGETTIME();

	auto it = m_mapBulletInfo.find( dwItemCode );
	if( it == m_mapBulletInfo.end() )
	{
		sBulletInfo sInfo;
		sInfo.bNeedSend = true;
		sInfo.dwWastedCount = 0;
		m_mapBulletInfo.insert( std::map<DWORD,sBulletInfo>::value_type(dwItemCode,sInfo)  );
	}

	it = m_mapBulletInfo.find( dwItemCode );
	if( it == m_mapBulletInfo.end() )
		return;

	sBulletInfo& sInfo = it->second;
	sInfo.bNeedSend = true;
	sInfo.dwWastedCount++;
	sInfo.dwLastRecvTime = REALGETTIME();
	if( sInfo.dwFirstRecvTime == 0 )
		sInfo.dwFirstRecvTime = sInfo.dwLastRecvTime;

#endif
#endif
}

void ioApplication::UpdateBulletInfo()
{
	DWORD dwCurTime = REALGETTIME();
	if( dwCurTime < m_dwLastBulletUpdateTime + 25 )
		return;
	m_dwLastBulletUpdateTime = dwCurTime;

	auto it = m_mapBulletInfo.begin();
	while( it != m_mapBulletInfo.end() )
	{
		sBulletInfo& sInfo = it->second;
		if( sInfo.bNeedSend )
		{
			//������ �Ѿ� ����� 50ms �̻�
			if( dwCurTime-sInfo.dwLastRecvTime > 100 )
			{
				SP2Packet kPacket(CUPK_ANTIHACK_BULLET_INFO);
				kPacket << it->first;
				kPacket << sInfo.dwWastedCount;
				kPacket << (sInfo.dwLastRecvTime - sInfo.dwFirstRecvTime);

				sInfo.bNeedSend = false;
				sInfo.dwWastedCount = 0;
				sInfo.dwFirstRecvTime = 0;
				SendToAllPlayingUser( kPacket );
			}
		}
		++it;
	}
}

void ioApplication::ReloadBullet( DWORD dwItemCode )
{
#ifdef ANTIHACK
#if defined( SRC_ID ) || defined( SRC_SEA )
	DWORD dwCurTime = REALGETTIME();
	auto it = m_mapBulletInfo.find( dwItemCode );
	if( it != m_mapBulletInfo.end() )
	{
		sBulletInfo& sInfo = it->second;
		if( sInfo.bNeedSend )
		{
			SP2Packet kPacket(CUPK_ANTIHACK_BULLET_INFO);
			kPacket << it->first;
			kPacket << sInfo.dwWastedCount;
			kPacket << (sInfo.dwLastRecvTime-sInfo.dwFirstRecvTime);

			sInfo.bNeedSend = false;
			sInfo.dwWastedCount = 0;
			SendToAllPlayingUser( kPacket );
		}

		SP2Packet kPacket(CUPK_ANTIHACK_BULLET_RELOAD);
		kPacket << it->first;
		SendToAllPlayingUser( kPacket );
	}
#endif
#endif
}


// Īȣ ���� (�����κ��� ������ ���)
void ioApplication::OnNamedTitleData( SP2Packet &rkPacket )
{
	ioUserNamedTitle *pUserNamedTitle = g_MyInfo.GetUserNamedTitle();
	if( !pUserNamedTitle )
		return;

	pUserNamedTitle->ApplyNamedTitleSlotData( rkPacket );
}

void ioApplication::OnPriateRulletInfo( SP2Packet &rkPacket )
{
	ioPriateRullet *pRullet = g_MyInfo.GetPriateRullet();
	if( !pRullet )
		return;

	pRullet->ApplyRulletRequest( rkPacket );
}

void ioApplication::OnPriateRulletUse( SP2Packet &rkPacket )
{
	ioPriateRullet *pRullet = g_MyInfo.GetPriateRullet();
	if( !pRullet )
		return;

	pRullet->ApplyRulletUse( rkPacket );
}

void ioApplication::OnPriateRulletRset( SP2Packet &rkPacket )
{
	ioPriateRullet *pRullet = g_MyInfo.GetPriateRullet();
	if( !pRullet )
		return;

	pRullet->ApplyRulletReset( rkPacket );
}

void ioApplication::OnSoldierSetInfo( SP2Packet& rkPacket )
{
	int iState = 0;
	rkPacket >> iState;

	ioHashString szName;
	rkPacket >> szName;

	int nNewClassType;
	rkPacket >> nNewClassType;

	ioPlayStage *pPlayStage = ToPlayStage( m_pGameStage );
	if( !pPlayStage )
		return;

	ioBaseChar *pChar = pPlayStage->GetBaseChar( szName );
	if ( !pChar )
		return;

	const CHARACTER& rkCharInfo = pChar->GetCharacterInfo();
	if ( !g_RSoldierInfoManager.IsSoldierSet( rkCharInfo ) )
		return;

	int nNewSoldierSetNum = g_RSoldierInfoManager.GetSoldierSetNum( nNewClassType );
	int nCurSoldierSetNum = g_RSoldierInfoManager.GetSoldierSetNum( rkCharInfo.m_class_type );
	if ( nCurSoldierSetNum <= 0 || nNewSoldierSetNum <= 0 || nCurSoldierSetNum != nNewSoldierSetNum )
		return;

	if ( iState == RSOLDIER_BUY )
	{
		int iCount = pChar->GetCurSoldierSetCnt();
		iCount++;
		pChar->SetCurSoldierSetCnt( iCount );
	}
	else if ( iState == RSOLDIER_SELL || iState == RSOLDIER_DISASSEMBLE )
	{
		int iCount = pChar->GetCurSoldierSetCnt();
		iCount--;
		iCount = max( 0, iCount );
		pChar->SetCurSoldierSetCnt( iCount );
	}	
}

//�ؽ� ���� ������ ��� �� ���� ���� �����մϴ�.
void ioApplication::SendNexonAuthLog( int iErrorType )
{
	char szLog[MAX_PATH] = "";
	switch( iErrorType )
	{
	case NexonSessionMessage:
	case NexonSessionLogin:
	case NexonSessionLogout:
	case NexonReconnect:
	case NexonIDTrial:
	case NexonOnPCRoom:
		break;

	case NexonWelcomeMessage:
		sprintf( szLog, STR(1), g_MyInfo.GetPublicID().c_str(), g_MyInfo.GetMyIP().c_str() );
		break;
		/*-----------------Shutdown -----------------*/
	case NexonTerminate:
		sprintf( szLog, STR(2), g_MyInfo.GetPublicID().c_str(), g_MyInfo.GetMyIP().c_str() );
		break;
	case NexonIDMaxError:
		sprintf( szLog, STR(3), g_MyInfo.GetPublicID().c_str(), g_MyInfo.GetMyIP().c_str() );
		break;
	case NexonIPDuplicate:
		sprintf( szLog, STR(4), g_MyInfo.GetPublicID().c_str(), g_MyInfo.GetMyIP().c_str() );
		break;
	case NexonIPMaxError:
		sprintf( szLog, STR(5), g_MyInfo.GetPublicID().c_str(), g_MyInfo.GetMyIP().c_str() );
		break;
	case NexonPCRoomBlocked:
		sprintf( szLog, STR(6), g_MyInfo.GetPublicID().c_str(), g_MyInfo.GetMyIP().c_str() );
		break;
	case NexonMessageShutDown:
		sprintf( szLog, STR(7), g_MyInfo.GetPublicID().c_str(), g_MyInfo.GetMyIP().c_str() );
		break;
	case NexonMessageTerminate:
		sprintf( szLog, STR(8), g_MyInfo.GetPublicID().c_str(), g_MyInfo.GetMyIP().c_str() );
		break;
	case NexonAccountNotAllowed:
		sprintf( szLog, STR(9), g_MyInfo.GetPublicID().c_str(), g_MyInfo.GetMyIP().c_str() );
		break;
	case NexonAccountMaxConnected:
		sprintf( szLog, STR(10), g_MyInfo.GetPublicID().c_str(), g_MyInfo.GetMyIP().c_str() );
		break;
	case NexonPrepaidExhausted:
		sprintf( szLog, STR(11), g_MyInfo.GetPublicID().c_str(), g_MyInfo.GetMyIP().c_str() );
		break;
	case NexonAccountMachineIDBlocked:
		sprintf( szLog, STR(12), g_MyInfo.GetPublicID().c_str(), g_MyInfo.GetMyIP().c_str() );
		break;
	case NexonSelectShutdown:
		sprintf( szLog, STR(13), g_MyInfo.GetPublicID().c_str(), g_MyInfo.GetMyIP().c_str() );
		break;
	case NexonShutDown:
		sprintf( szLog, STR(14), g_MyInfo.GetPublicID().c_str(), g_MyInfo.GetMyIP().c_str() );
		break;
	case NexonForbidden:
		sprintf( szLog, STR(15), g_MyInfo.GetPublicID().c_str(), g_MyInfo.GetMyIP().c_str() );
		break;
	case NexonNotNexonUser:
		sprintf( szLog, STR(16), g_MyInfo.GetPublicID().c_str(), g_MyInfo.GetMyIP().c_str() );
		break;
	case NexonUnknownShutDown:
		sprintf( szLog, STR(17), g_MyInfo.GetPublicID().c_str(), g_MyInfo.GetMyIP().c_str() );
		break;
	case NexonAgeError:
		sprintf( szLog, STR(18), g_MyInfo.GetPublicID().c_str(), g_MyInfo.GetMyIP().c_str() );
		break;
	case NexonSessionServerError:
		sprintf( szLog, STR(19), g_MyInfo.GetPublicID().c_str(), g_MyInfo.GetMyIP().c_str() );
		break;
	case NexonAddressNotAllowed:
		sprintf( szLog, STR(20), g_MyInfo.GetPublicID().c_str(), g_MyInfo.GetMyIP().c_str() );
		break;
	}
	// �α� ���� ����
	SP2Packet kPacket( LUPK_LOG );
	kPacket << "NexonError";  // �α� ���� Ÿ��
	kPacket << szLog;
	kPacket << 400;  // ������ȣ:400 �ؽ� ���� ���� Ÿ��
	kPacket << false; // write db
	P2PNetwork::SendLog(kPacket);
}

void ioApplication::OnAccessoryData( SP2Packet &rkPacket )
{
	ioUserAccessory *pUserAccessory = g_MyInfo.GetUserAccessory();
	if( !pUserAccessory )
		return;

	pUserAccessory->ApplyAccessorySlotData( rkPacket );

	if( ToPlayStage( m_pGameStage ) )
		ToPlayStage( m_pGameStage )->ReLoadAllMySoldier();

	SoldierSelectWnd *pSoldierWnd = dynamic_cast<SoldierSelectWnd*>(g_GUIMgr.FindWnd( SOLDIER_SELECT_WND ));
	if( pSoldierWnd && pSoldierWnd->IsShow() )
		pSoldierWnd->SettingSoldierBtn();

	MiniSoldierSelectWnd *pMiniSoldierWnd = dynamic_cast<MiniSoldierSelectWnd*>( g_GUIMgr.FindWnd( MINI_SOLDIER_SELECT_WND ));
	if( pMiniSoldierWnd && pMiniSoldierWnd->IsShow() )
		pMiniSoldierWnd->SettingMiniSoldierBtn();
}

void ioApplication::OnAccessoryDeleteByDate( SP2Packet &rkPacket )
{
	if( !g_MyInfo.DeleteAccessoryData( rkPacket ) )
	{
		LOG.PrintTimeAndLog( 0, "ioApplication::OnAccessoryDeleteByDate() - Delete Fail" );
		return;
	}

	MyInventoryWnd *pInvenWnd = dynamic_cast<MyInventoryWnd*>(g_GUIMgr.FindWnd(MY_INVENTORY_WND));
	if( pInvenWnd && pInvenWnd->IsShow() )
	{
		pInvenWnd->UpdateTab( true, true );
	}

	SoldierSelectWnd *pSoldierWnd = dynamic_cast<SoldierSelectWnd*>(g_GUIMgr.FindWnd( SOLDIER_SELECT_WND ));
	if( pSoldierWnd && pSoldierWnd->IsShow() )
	{
		pSoldierWnd->SettingSoldierBtn();
	}

	MiniSoldierSelectWnd *pMiniSoldierWnd = dynamic_cast<MiniSoldierSelectWnd*>( g_GUIMgr.FindWnd( MINI_SOLDIER_SELECT_WND ));
	if( pMiniSoldierWnd && pMiniSoldierWnd->IsShow() )
	{
		pMiniSoldierWnd->SettingMiniSoldierBtn();
	}
}
