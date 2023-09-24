#include "stdafx.h"
#include "steam/steam_api.h"
//#include "steam/steamencryptedappticket.h"
#include "ioSteamArchievements.h"
#include "ioSHA256.h"
#include "EtcHelpFunc.h"
#include "ioSteamClient.h"

#ifdef STEAM_ATTACH



//const char * C_STEAM_REDEEM_PAGE_KEY = "LM2d3k54pU";
//const int C_TIME_MINUTE_STEP = 3;
//
//const char * C_TIME_FORMAT = "%02d:%02d";
const char C_KEY_SEPARATOR = '|';
//const char C_REDEEM_SEPARATOR = '|';
//const char * C_REDEEM_FORMATTER = "%s|%s|%s|%s";
//const char * C_REDEEM_BILLINGPAGE = "https://lostsaga.z8games.com/steam/redeem?usn=%s&auth=%s";

#define SIZE_BILLINGPAGE 1024

ioSteamClient *ioSteamClient::sg_Instance = NULL;

ioSteamClient::ioSteamClient(void) : 
m_bSteamEnable(false),
m_pSteamArchievements(NULL),
m_pHashMaker(NULL)
{
}

ioSteamClient::~ioSteamClient(void)
{
	SAFEDELETE(m_pSteamArchievements);
	SAFEDELETE(m_pHashMaker);

}

ioSteamClient &ioSteamClient::GetInstance()
{
	if( sg_Instance == NULL )
		sg_Instance = new ioSteamClient;

	return (*sg_Instance);
}

void ioSteamClient::ReleaseInstance()
{
	if( sg_Instance )
		delete sg_Instance;

	sg_Instance = NULL;
}

extern "C" void __cdecl SteamAPIDebugTextHook( int nSeverity, const char *pchDebugText )
{
	// if you're running in the debugger, only warnings (nSeverity >= 1) will be sent
	// if you add -debug_steamapi to the command-line, a lot of extra informational messages will also be sent
	LOG.PrintTimeAndLog( 0, "Steam Log - %s", pchDebugText);

	if ( nSeverity >= 1 )
	{
		// place to set a breakpoint for catching API errors
		int x = 3;
		x = x;
	}
}


bool ioSteamClient::Start()
{

	LOG.PrintTimeAndLog( 0, "Steam Log - StartSteam");

	if ( SteamAPI_RestartAppIfNecessary( STAEM_APP_ID ) )
	//if ( SteamAPI_RestartAppIfNecessary( 0x00 ) )
	{
		// if Steam is not running or the game wasn't started through Steam, SteamAPI_RestartAppIfNecessary starts the 
		// local Steam client and also launches this game again.

		// Once you get a public Steam AppID assigned for this game, you need to replace k_uAppIdInvalid with it and
		// removed steam_appid.txt from the game depot.
		m_bSteamEnable = false;
		LOG.PrintTimeAndLog( 0, "Steam Log - SteamAPI_RestartAppIfNecessary Fail");
		return false;
	}


	// Initialize SteamAPI, if this fails we bail out since we depend on Steam for lots of stuff.
	// You don't necessarily have to though if you write your code to check whether all the Steam
	// interfaces are NULL before using them and provide alternate paths when they are unavailable.
	//
	// This will also load the in-game steam overlay dll into your process.  That dll is normally
	// injected by steam when it launches games, but by calling this you cause it to always load,
	// even when not launched via steam.

	if(!SteamAPI_Init())
	{
		LOG.PrintTimeAndLog( 0, "Steam fail - SteamAPI_Init Fail");
		m_bSteamEnable = false;
		return false;
	}

	// set our debug handler
	SteamClient()->SetWarningMessageHook( &SteamAPIDebugTextHook );

	// Tell Steam where it's overlay should show notification dialogs, this can be top right, top left,
	// bottom right, bottom left. The default position is the bottom left if you don't call this.  
	// Generally you should use the default and not call this as users will be most comfortable with 
	// the default position.  The API is provided in case the bottom right creates a serious conflict 
	// with important UI in your game.
	SteamUtils()->SetOverlayNotificationPosition( k_EPositionBottomRight );

	// Ensure that the user has logged into Steam. This will always return true if the game is launched
	// from Steam, but if Steam is at the login prompt when you run your game from the debugger, it
	// will return false.
	if ( !SteamUser()->BLoggedOn() )
	{
		LOG.PrintTimeAndLog( 0, "Steam Log - Steam user is not logged in");
		SteamAPI_Shutdown();
		m_bSteamEnable = false;
		return false;
	}

	if(!m_pSteamArchievements)
		m_pSteamArchievements = new ioSteamAchievements();
	if(!m_pHashMaker)
		m_pHashMaker = new ioSHA256();

	LOG.PrintTimeAndLog( 0, "Steam Log - Steam Start Complete!");
	return true;

}

void ioSteamClient::End()
{
	if(m_bSteamEnable)
	{
		//// Shutdown the SteamAPI
		//SteamController()->Shutdown();
		SAFEDELETE(m_pSteamArchievements);
		SAFEDELETE(m_pHashMaker);
		m_bSteamEnable = false;
		LOG.PrintTimeAndLog( 0, "Steam Log - Steam End!");
		SteamAPI_Shutdown();
	}
}

void ioSteamClient::RunCallbacks()
{
	if(m_bSteamEnable)
	{
		SteamAPI_RunCallbacks();
	}
}

bool ioSteamClient::GetAuthSessionTicket()
{
	if(m_bSteamEnable)
	{
		// 테스트 코드
		char rgchToken[1024];
		uint32 unTokenLen = 0;
		HAuthTicket m_hAuthTicket;
		m_hAuthTicket = SteamUser()->GetAuthSessionTicket( rgchToken, sizeof( rgchToken ), &unTokenLen );
		return true;
	}
	return false;
}

bool ioSteamClient::ParseCmd( const char * szCmd )
{
	// ? 구분자 : , 확장 구분자
	int iSize    = strlen( szCmd );
	int iMarkCnt = 0;
	int iExpandMarkCnt = 0;
	char szEnable[MAX_PATH]="";
	//char msg[MAX_PATH]="";

	int  iEnable = 0;
	enum { MAX_MARK = 0, STEAM_ENABLE_POS = 11};

	for (int i = 0; i < iSize ; i++)
	{
		if( szCmd[i] == '?' )
		{
			iMarkCnt++;
			continue;
		}

		if( iMarkCnt == MAX_MARK )
		{
			if( szCmd[i] == ',' )
			{
				iExpandMarkCnt++;
				continue;
			}

			if( iExpandMarkCnt == STEAM_ENABLE_POS )
			{
				//sprintf(msg, "%d, %c \n", i, szCmd[i]);
				//OutputDebugString(msg);
				szEnable[iEnable] = szCmd[i];
				iEnable++;

				if( iEnable >= MAX_PATH )
					break;
			}
		}
		else if( iMarkCnt > MAX_MARK )
		{
			break;
		}
	}

	int iRet = atoi(szEnable);
	if(iRet == 0)
	{
		m_bSteamEnable = false;
		return false;
	}
	else
	{
		m_bSteamEnable = true;
		return true;
	}
}

void ioSteamClient::SetAchievement( int questID, int subID )
{
	if(m_bSteamEnable && m_pSteamArchievements)
	{
		////id 생성 규칙 퀘스트 아이디값_서브값
		m_pSteamArchievements->SetAchievementByIndexes(questID, subID);
	}
}

void ioSteamClient::ResetAchievement()
{
	if(m_bSteamEnable && m_pSteamArchievements)
	{
		m_pSteamArchievements->ResetAchievements();
	}
}

void ioSteamClient::OpenWebPage( const char * szURL )
{
	if(m_bSteamEnable && szURL)
	{
		// load the test html page, it just has a steam://gamewebcallback link in it
		SteamFriends()->ActivateGameOverlayToWebPage( szURL );

	}
}

void ioSteamClient::OpenRedeemPage()
{
	if(m_bSteamEnable && m_pHashMaker)
	{
		if(m_pSteamArchievements)
			m_pSteamArchievements->SetRedeemOpen();

		SP2Packet kPacket( CTPK_FILL_CASH_URL );

		// 빌링인지 리딤인지 int형으로
		int nCharge = FILL_CASH_URL_STEAM_REDEEM;
		kPacket << nCharge;

		TCPNetwork::SendToServer( kPacket );
		TCPNetwork::MouseBusy( true );
		return;
	}


}

void ioSteamClient::SetKeyValue( ioHashString val )
{

	// 접속 키값에서 ID 분리하기.
	std::string key = val.c_str();
	size_t count = key.find(C_KEY_SEPARATOR);

	std::string strID = key.substr(0, count);
	m_pIDValue = strID.c_str();
//	LOG.PrintTimeAndLog(0, "STEAM_LOG - ID : %s", m_pIDValue.c_str());
}

void ioSteamClient::SetBillingOpen()
{
	if(m_pSteamArchievements)
	{
		m_pSteamArchievements->SetBillingOpen();
	}
}



#endif // STEAM_ATTACH