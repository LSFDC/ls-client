#include "stdafx.h"
#include "steam/steam_api.h"
#include "ioSteamArchievements.h"
#include "stringext.h"

#ifdef STEAM_ATTACH


ioSteamAchievements::ioSteamAchievements(): 				
	m_iAppID( 0 ),
	m_bInitialized( false ),
	m_bRedeemOpen( false ),
	m_bBillingOpen(false),
	m_CallbackUserStatsReceived( this, &ioSteamAchievements::OnUserStatsReceived ),
	m_CallbackUserStatsStored( this, &ioSteamAchievements::OnUserStatsStored ),
	m_CallbackAchievementStored( this, &ioSteamAchievements::OnAchievementStored ),
	m_CallbackGameOverlayActivated( this, &ioSteamAchievements::OnOverlayActivated )
{
	m_iAppID = SteamUtils()->GetAppID();
	m_vecArchievements.clear();
	RequestStats();
}

ioSteamAchievements::~ioSteamAchievements()
{
	ClearAchievements();
}

void ioSteamAchievements::ClearAchievements()
{
	m_vecArchievements.clear();
}

bool ioSteamAchievements::RequestStats()
{
	// Is Steam loaded? If not we can't get stats.
	if ( NULL == SteamUserStats() || NULL == SteamUser() )
	{
		return false;
	}
	// Is the user logged on?  If not we can't get stats.
	if ( !SteamUser()->BLoggedOn() )
	{
		return false;
	}
	// Request user stats.
	return SteamUserStats()->RequestCurrentStats();
}


bool ioSteamAchievements::SetAchievement(const char* pID)
{
	if(!pID) return false;
	// Have we received a call back from Steam yet?
	if (m_bInitialized)
	{
		Achievement_t * pAchievement = FindAchievementByID(pID);
		if(pAchievement)
		{
			if(!pAchievement->m_bAchieved)
			{
				SteamUserStats()->SetAchievement(pID);
				return SteamUserStats()->StoreStats();
			}
		}
		return false;
	}
	// If not then we can't set achievements yet
	return false;
}

bool ioSteamAchievements::SetAchievementByIndexes( int mainIndex, int subIndex )
{
	std::string achievementID;
	if(ConvertIndexesToStr(achievementID, mainIndex, subIndex))
	{
		return SetAchievement(achievementID.c_str());
	}

	return false;
}


void ioSteamAchievements::OnUserStatsReceived( UserStatsReceived_t *pCallback )
{
	if(!pCallback) return;
	// we may get callbacks for other games' stats arriving, ignore them
	if ( m_iAppID == pCallback->m_nGameID )
	{
		if ( k_EResultOK == pCallback->m_eResult )
		{
			LOG.PrintTimeAndLog( 0, "Steam Log - Received stats and achievements from Steam");

			ClearAchievements();
			m_bInitialized = true;

			uint32 iNumArchievements = SteamUserStats()->GetNumAchievements();
			ISteamUserStats * stats = SteamUserStats();

			// load achievements
			for ( int iAch = 0; iAch < iNumArchievements; ++iAch )
			{
				// 도전과제 정보를 얻자.
				Achievement_t ach;

				ach.m_eAchievementID = iAch;

				_snprintf( ach.m_name, sizeof(ach.m_name), "%s", 
					SteamUserStats()->GetAchievementName(iAch));
				_snprintf( ach.m_rgchName, sizeof(ach.m_rgchName), "%s", 
					SteamUserStats()->GetAchievementDisplayAttribute(ach.m_name, 
					"name"));
				_snprintf( ach.m_rgchDescription, sizeof(ach.m_rgchDescription), "%s", 
					SteamUserStats()->GetAchievementDisplayAttribute(ach.m_name, 
					"desc"));
				SteamUserStats()->GetAchievement(ach.m_name, &ach.m_bAchieved);

				int mainIndex = 0;
				int subIndex = 0;
				if(ConvertStrToIndexes(mainIndex, subIndex, ach.m_name))
				{
					ach.m_iMainIndex = mainIndex;
					ach.m_iSubIndex = subIndex;
				}

				m_vecArchievements.push_back(ach);
			}
		}
		else
		{
			LOG.PrintTimeAndLog( 0, "Steam Log - RequestStats - failed, %d", pCallback->m_eResult);
		}
	}
}

void ioSteamAchievements::OnUserStatsStored( UserStatsStored_t *pCallback )
{
	// we may get callbacks for other games' stats arriving, ignore them
	if ( m_iAppID == pCallback->m_nGameID )	
	{
		if ( k_EResultOK == pCallback->m_eResult )
		{
			LOG.PrintTimeAndLog( 0, "Steam Log - Stored stats for Steam");
		}
		else
		{
			LOG.PrintTimeAndLog( 0, "Steam Log - StatsStored - failed, %d", pCallback->m_eResult);
		}
	}
}


void ioSteamAchievements::OnAchievementStored( UserAchievementStored_t *pCallback )
{
	// we may get callbacks for other games' stats arriving, ignore them
	if ( m_iAppID == pCallback->m_nGameID )	
	{
		LOG.PrintTimeAndLog( 0, "Steam Log - Stored Achievement for Steam");

		Achievement_t * pAchievement = FindAchievementByID(pCallback->m_rgchAchievementName);
		if(pAchievement)
		{
			pAchievement->m_bAchieved = true;
		}
	}
}

void ioSteamAchievements::OnOverlayActivated( GameOverlayActivated_t *pCallback )
{
	if(!pCallback) return;

	// Overlay Deactivated.
	if ( !pCallback->m_bActive )
	{
		if(m_bRedeemOpen)
		{
			g_PresentMgr.SendPresentData();
			m_bRedeemOpen = false;
		}

		if(m_bBillingOpen)
		{
			SP2Packet kPacket( CTPK_GET_CASH );
			TCPNetwork::SendToServer( kPacket );
			TCPNetwork::MouseBusy( true );
			m_bBillingOpen = false;
		}

		
	}
}


Achievement_t * ioSteamAchievements::FindAchievementByID( const char * pID )
{
	ioHashString strID = pID;
	for(auto itAchieve = m_vecArchievements.begin(); itAchieve != m_vecArchievements.end(); ++itAchieve)
	{
		Achievement_t & achieve = *itAchieve;
		if(strcmp(pID, achieve.m_name) == 0)
			return &achieve;

	}
	return NULL;
}

void ioSteamAchievements::ResetAchievements()
{
	LOG.PrintTimeAndLog( 0, "Steam Log - Stored Achievement for Steam");
	SteamUserStats()->ResetAllStats(true);
	RequestStats();
}

bool ioSteamAchievements::ConvertStrToIndexes( int & outMainIndex, int & outSubIndex, const char * inStr )
{
	////id 생성 규칙 퀘스트 아이디값_서브값
	if(inStr == NULL ||
		strlen(inStr) < 1)
	{
		return false;
	}

	std::vector<std::string> vecStr = explode(inStr, "_");
	if(vecStr.size() < 2)
	{
		return false;
	}

	std::string strFirst = vecStr.at(0);
	std::string strSecond = vecStr.at(1);

	outMainIndex = atoi(strFirst.c_str());
	outSubIndex = atoi(strSecond.c_str());

	return true;
}

bool ioSteamAchievements::ConvertIndexesToStr( std::string & outStr, int inMainIndex, int inSubIndex )
{
	////id 생성 규칙 퀘스트 아이디값_서브값
	if(inMainIndex < 1 ||
		inSubIndex < 1)
	{
		return false;
	}

	char achievementID[MAX_PATH] = {0,};
	sprintf_e(achievementID, "%d_%d", inMainIndex, inSubIndex);
	outStr = achievementID;

	return true;
}


#endif // STEAM_ATTACH
