#ifndef __ioSteamArchievements_h__
#define __ioSteamArchievements_h__

#ifdef STEAM_ATTACH
#pragma comment(lib, "steam_api.lib" )
#pragma comment(lib, "sdkencryptedappticket.lib" )

#define _ACH_ID( id, name ) { id, #id, name, "", 0, 0 }

// 도전과제 ID 설정 규칙 퀘스트ID_subID (ex : 100번 퀘스트 서브ID가 20일때 100_20)
struct Achievement_t
{
	Achievement_t() : 
		m_eAchievementID(0),
		m_bAchieved(false),
		m_iIconImage(0),
		m_iMainIndex(0),
		m_iSubIndex(0)
		{ 
			ZeroMemory(m_name, sizeof(m_name));
			ZeroMemory(m_rgchName, sizeof(m_rgchName));
			ZeroMemory(m_rgchDescription, sizeof(m_rgchDescription));
		}


	int m_eAchievementID;
	char m_name[128];
	char m_rgchName[128];
	char m_rgchDescription[256];
	bool m_bAchieved;
	int m_iIconImage;
	int m_iMainIndex;
	int m_iSubIndex;
};

class ioSteamAchievements
{
private:
	int64 m_iAppID; // Our current AppID
	Achievement_t *m_pAchievements; // Achievements data
	int m_iNumAchievements; // The number of Achievements
	bool m_bInitialized; // Have we called Request stats and received the callback?
	bool m_bRedeemOpen;
	bool m_bBillingOpen;
	std::vector<Achievement_t> m_vecArchievements;

public:
	//ioSteamAchievements(Achievement_t *Achievements, int NumAchievements);
	ioSteamAchievements();
	~ioSteamAchievements();

	void ClearAchievements();
	void ResetAchievements();
	Achievement_t * FindAchievementByID(const char * pID);


	bool RequestStats();
	bool SetAchievement(const char* pID);
	bool SetAchievementByIndexes(int mainIndex, int subIndex);
	void SetRedeemOpen() { m_bRedeemOpen = true; }
	void SetBillingOpen() { m_bBillingOpen = true; }


	bool ConvertStrToIndexes(int & outMainIndex, int & outSubIndex, const char * inStr);
	bool ConvertIndexesToStr(std::string & outStr, int inMainIndex, int inSubIndex);

	STEAM_CALLBACK( ioSteamAchievements, OnUserStatsReceived, UserStatsReceived_t, 
		m_CallbackUserStatsReceived );
	STEAM_CALLBACK( ioSteamAchievements, OnUserStatsStored, UserStatsStored_t, 
		m_CallbackUserStatsStored );
	STEAM_CALLBACK( ioSteamAchievements, OnAchievementStored, 
		UserAchievementStored_t, m_CallbackAchievementStored );

	// CALLBACKS
	STEAM_CALLBACK( ioSteamAchievements, OnOverlayActivated, GameOverlayActivated_t, 
		m_CallbackGameOverlayActivated );

};

#endif // STEAM_ATTACH

#endif // __ioSteamArchievements_h__