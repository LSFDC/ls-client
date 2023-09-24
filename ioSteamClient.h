#ifndef __ioSteamClient_h__
#define __ioSteamClient_h__

#ifdef STEAM_ATTACH
#pragma comment(lib, "steam_api.lib" )
#pragma comment(lib, "sdkencryptedappticket.lib" )

class ioSteamAchievements;
class ioSHA256;
	
class ioSteamClient 
{
protected:
	static ioSteamClient *sg_Instance;

private: // Singleton class
	ioSteamClient(void);
	virtual ~ioSteamClient(void);
public:
	static ioSteamClient &GetInstance();
	static void ReleaseInstance();


private:
	bool m_bSteamEnable;
	ioSteamAchievements * m_pSteamArchievements;
	ioSHA256 * m_pHashMaker;
	ioHashString m_pIDValue;

public:
	bool Start();
	void End();
	void RunCallbacks();
	bool GetAuthSessionTicket();
	bool ParseCmd(const char * szCmd);

	void SetAchievement(int questID, int subID);
	void ResetAchievement();

	void OpenWebPage(const char * szURL);

	void OpenRedeemPage();



	bool GetSteamEnable() const { return m_bSteamEnable; }
	void SetSteamEnable(bool val) { m_bSteamEnable = val; }
	void SetKeyValue(ioHashString val);

	void SetBillingOpen();

};

#define g_ioSteamClient ioSteamClient::GetInstance()



#endif // STEAM_ATTACH

#endif // __ioSteamClient_h__