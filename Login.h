#pragma once
#include "iepmapi.h"

#define NORMAL_EXCUTE_CHECK      "EDEW3940FVDP4950"
#define CMD_SECTION_TOKEN        "?"
#define CMD_EXPAND_SECTION_TOKEN ","

#define AUTPUPGRADE_CLASS_NAME "_LOSTSAGA_AUTOUPGRADE_"
//----------------Auto updade �� ���------------
#define WM_MY_MSG_A_EXIT       (WM_USER + 300)  // Auto upgrade
#define WM_MY_MSG_C_CONNECT    (WM_USER + 301)  // client
#define WM_MY_MSG_C_CONNECT_OK (WM_USER + 302)  // client 
#define WM_MY_MSG_C_ERROR      (WM_USER + 303)  // client
#define WM_MY_MSG_C_SUCCESS    (WM_USER + 304)  // client
#define WM_MY_MSG_C_IP_BLOCK   (WM_USER + 305)  // client

class SP2Packet;

struct HANDLEToHWNDInfo
{
	DWORD m_dwProcessID;
	HWND m_hHwnd;
};

static BOOL CALLBACK HandleFromPIDProc( HWND hwnd, LPARAM lParam );
HWND GetHandleFromPID( DWORD pid );

class CLogin
{
public:
	enum OPTION_TYPE
	{
		OT_SINGLE_MODE            = 0,
		OT_RESOURCE_UPDATE        = 1,
		OT_READ_PAC               = 2,
		OT_EXTRACT_PAC            = 3,
		OT_GAME_MODE              = 4,
		OT_FULLSCREEN             = 5,
		OT_DEBUG_SHOW_BOUND_BOX   = 6,
		OT_DEBUG_SHOW_EXTRA_INFO  = 7,
		OT_DEBUG_MEMORY_USAGE     = 8,
		OT_SHOW_CONSOLWINDOW      = 9,
		OT_VISUAL_STUDIO_DEBUG    = 10,
		OT_ADMIN_FOLDER_COPY      = 11,
		OT_GAME_MODE_SUB          = 12,
		OT_GAME_MODE_MAP          = 13,
		OT_MAX
	};

	enum LOGIN_STATE
	{
		LS_NONE               = 0,
		LS_SENT_MSG           = 1,
		LS_SENT_UDP_MSG       = 2,
		LS_USER_CLICK_EXIT    = 3,
		LS_RECEIVE_MSG_ERROR  = 4,
		LS_RECEIVE_MSG_NORMAL = 5,
	};
protected:
	enum PROCESS_OPTIONS_RETURN
	{
		POR_RETURN_FALSE = 0,
		POR_RETURN_TRUE  = 1,
		POR_NEXT_MOVE    = 2
	};

protected:
	char m_szLoginKey[MAX_PATH*2];
	LOGIN_STATE m_eLoginState;
	bool m_bSingleMode;
	DWORD m_dwSentUDPMsgTime;
	ioHashString m_szOptions;
	ioHashString m_szMyIP;
	int m_iCheckCnt;
	DWORD m_dwConnectPreTime;
	HWND	m_hWnd;
protected:

#ifndef SHIPPING
	void ExcuteIE( OUT PROCESS_INFORMATION &process_info ,IN const char *szURL );
	void ExcuteIE( OUT PROCESS_INFORMATION &IEProcessInfo, OUT IELAUNCHURLINFO& LunchInfo, IN const char *szURL );
#endif

	int GetProcessIEParentID( DWORD pid );
	void SetSetting(const int iType , const int iData);
	int  GetNumber( char *szOneOption ); 

	void SendLogin(const ioHashString &szLoginKey);

	bool _OnConnectOK( const ioHashString &szID, SP2Packet &rkPacket );
	
	void SendUDPMsg();

	void ParseExpandValue( const std::string &rszExpandValue );

	bool DeleteDirectoryInFile( const char *DirName, const char *FileExten );
	void ShowDisconnectDlg( HINSTANCE hInstance );
	void SendPCInfo();

public:
	BOOL LoginProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void DebugWebLogin(OUT LPSTR &szCmdLine);
	bool ParseCmd( IN const char *szCmdLine,
				   OUT ioHashString &szKeyOnCmd,
				   OUT ioHashString &szOptinosOnCmd,
		           OUT ioHashString &szIPOnCmd,
				   OUT int &iPortOnCmd );

	bool ProcessOptions( const ioHashString &szOptions);
	bool LoginLoop(const ioHashString &szKey);
	void SetAdminSettings();

	void OnConnect( SP2Packet &rkPacket );
	void OnUDPConnect( SP2Packet &rkPacket );
	void OnDisconnectAlreadyID( SP2Packet &rkPacket );

	void SetLoginState(CLogin::LOGIN_STATE eLoginState);

	bool IsSingleMode() const { return m_bSingleMode; }
	void SetMyIP( const char *szMyIP );

	void SendMsgToAutoUpgrade(DWORD dwWMTYPE);
	int	 GetLoginResult() { return m_iLoginResult; };

private:
	int	m_iLoginResult;

public: 
	CLogin(void);
	~CLogin(void);
};

