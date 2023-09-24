
#include "stdafx.h"

#include "resource.h"
#include "Login.h"
#include "skin.h"
#include "ioAISymbolTable.h"

#include "io.h"
#include "Aclapi.h"
#include "Psapi.h"

#include "MiniDump/MiniDump.h"
#include "MiniDump/File.h"
#include "CrashFind/BugslayerUtil.h"
#include "Version.h"

#ifdef _USE_TCMALLOC
#include "../gperftools-2.1/src/windows/gperftools/tcmalloc.h"
#endif


#define CLIENTMUTEXNAME  "Global\\_LOSTSAGA_CLIENT_3894738393994848484939308770494"

// For StickKeys Disable
STICKYKEYS g_StartupStickyKeys = {sizeof(STICKYKEYS), 0}; //Except Extracting Hangeul
FILTERKEYS g_StartupFilterKeys = {sizeof(FILTERKEYS), 0}; //Except Extracting Hangeul

void AllowAccessibilityShortcutKeys( bool bAllowKeys );

void InitOnWinMain( ioHashString &szOptions );
void RelaseOnWinMain( ioApplication *pApp );
void SendMsgToAutoUpgrade( DWORD dwWMTYPE );

// 스택의 안전을 보장하지 못하므로 그나마 미리 확보할수 있는 전역으로 
bool g_bHappenCrash = false;
bool g_bHappenError = false;
int  g_iErrorNumber = 0;
char g_szErrorPos[MAX_PATH];
char g_szErrorLog[4096];
char g_szTempLogBuf[2048];
char g_szSendErrorLog[4096];
const char *szTrace = NULL;

CLog g_MemoryLOG;

#if defined( USE_GA )
CLog g_GALog;
#endif

// error dlg skin
#define ERROR_DLG_WIDTH       410
#define ERROR_DLG_HEIGHT      298
#define ERROR_EDIT_WIDTH      376
#define ERROR_EDIT_HEIGHT     191
#define ERROR_X_WIDTH         25
#define ERROR_X_HEIGHT        26
#define ERROR_SOLUTION_WIDTH  286
#define ERROR_SOLUTION_HEIGHT 31
#define ERROR_EXIT_WIDTH      88
#define ERROR_EXIT_HEIGHT     31

#define ERROR_EDIT_BK_RGB   RGB(235, 235, 235)
#define ERROR_EDIT_TEXT_RGB RGB(58,58,58)
#define BTN_BITMAP_SIZE     3

CSkin  *g_pErrorDlgSkin = NULL;
HWND    g_hEdit         = NULL;
HBRUSH  g_hBrushEdit    = NULL;
HBITMAP g_hBitmapX[BTN_BITMAP_SIZE]={NULL,NULL,NULL};
HBITMAP g_hBitmapSolution[BTN_BITMAP_SIZE]={NULL,NULL,NULL};
HBITMAP g_hBitmapExit[BTN_BITMAP_SIZE]={NULL,NULL,NULL};
bool    g_bXOver        = false;
bool    g_bSolutionOver = false;
bool    g_bExitOver     = false;
WNDPROC	g_OldXProc      = NULL;
WNDPROC	g_OldSolutionProc= NULL;
WNDPROC	g_OldExitProc    = NULL;

BOOL CALLBACK ErrorDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK XProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK SolutionProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ExitProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void ShowErrorDlg( HINSTANCE hInstance );
void ShowOtherErrorDlg( HINSTANCE hInstance );
void OnDrawItem( LPARAM lParam, WPARAM wParam );
extern LONG __stdcall ExceptCallBack( EXCEPTION_POINTERS *pExPtrs );
//메모리
typedef enum _SYSTEM_INFORMATION_CLASS
{
	SystemFileCacheInformation = 21,
	SystemMemoryListInformation = 80
};

#define SYSTEMCACHEINFORMATION 0x15
typedef struct {
	ULONG    	CurrentSize;
	ULONG    	PeakSize;
	ULONG    	PageFaultCount;
	ULONG    	MinimumWorkingSet;
	ULONG    	MaximumWorkingSet;
	ULONG    	Unused[4];
} SYSTEM_CACHE_INFORMATION;
typedef ULONG (__stdcall* NTSETSYSTEMINFORMATION)(ULONG, PVOID, ULONG);
NTSETSYSTEMINFORMATION NtSetSystemInformation;

typedef ULONG (__stdcall* NTQUERYSYSTEMINFORMATION)(ULONG,PVOID,ULONG,PULONG);
NTQUERYSYSTEMINFORMATION NtQuerySystemInformation;

typedef struct _SYSTEM_FILECACHE_INFORMATION
{
	SIZE_T CurrentSize;
	SIZE_T PeakSize;
	ULONG PageFaultCount;
	SIZE_T MinimumWorkingSet;
	SIZE_T MaximumWorkingSet;
	SIZE_T CurrentSizeIncludingTransitionInPages;
	SIZE_T PeakSizeIncludingTransitionInPages;
	ULONG TransitionRePurposeCount;
	ULONG Flags;
} SYSTEM_FILECACHE_INFORMATION, *PSYSTEM_FILECACHE_INFORMATION;

typedef enum _SYSTEM_MEMORY_LIST_COMMAND
{
	MemoryCaptureAccessedBits,
	MemoryCaptureAndResetAccessedBits,
	MemoryEmptyWorkingSets,
	MemoryFlushModifiedList,
	MemoryPurgeStandbyList,
	MemoryPurgeLowPriorityStandbyList,
	MemoryCommandMax
} SYSTEM_MEMORY_LIST_COMMAND;

typedef LONG NTSTATUS;

/*
NTSTATUS (NTAPI *NtSetSystemInformation)(
	__in _SYSTEM_INFORMATION_CLASS SystemInformationClass,
	__in_bcount_opt(SystemInformationLength) PVOID SystemInformation,
	__in ULONG SystemInformationLength);*/

void trimProcessWorkingSet(DWORD processID);
void clearSystemCache();
void trimOtherProcessesWorkingSet();
BOOL SetPrivilege( HANDLE hToken, LPCTSTR Privilege, BOOL bEnablePrivilege );
void LogGlobalMemory();

bool bTestLog = false;
bool bMemoryTest = false;

int WINAPI WinMain( HINSTANCE hInstance,
				    HINSTANCE hPrevInstance,
					LPSTR lpCmdLine,
					int nShowCmd )
{

#ifdef SHIPPING
	LOG.OpenLog( 0, "info/pp.log");
#else
	//Admin버전은 다중 실행 될 수 있음으로 각 프로세스 별로 로그 파일을 생성 할 수 있도록 수정함
	int iProcessCount = Help::GetProcessRunningCount( "lostsaga.exe" );
	char szBuff[MAX_PATH];
	sprintf_s( szBuff, "info/pp_%d.log", iProcessCount );
	LOG.OpenLog( 0, szBuff );
#endif

	LOG.PrintTimeAndLog(0, "[Main] - LostSaga Start");

//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);

#ifndef _DEBUG
	// minidump
 	if( Help::CheckEnableTimeGapUser(1) )
		MiniDump::Begin( "LostSagaT" );

	if( !SetCrashHandlerFilter( ExceptCallBack ) )
	{
		LOG.PrintTimeAndLog( 0, "WINAPI WinMain - SetCrashHandlerFilter Failed" );
		return false;
	}
#endif

	ioINIMemoryStatic* pStatics = new ioINIMemoryStatic;
	ioLocalManager *pLocalMgr = new ioLocalManager;   
	if( pLocalMgr )
	{		
		pLocalMgr->Init();
		pLocalMgr->ParseCmd( lpCmdLine );
		LOG.PrintTimeAndLog(0, "[Main] - Local Manager Init Complete");
	}
	else
	{
		LOG.PrintTimeAndLog(0, "[Main] - Local Manager Init Failed");
	}

	ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
	ioStringManager *pStringMgr = new ioStringManager;
	if( pStringMgr && pLocal )
	{
#ifdef SRC_OVERSEAS
		DWORD size = 0;
		const char * buffer = NULL;
		Help::LoadTextInResource(IDR_ERROR, size, buffer);
		pStringMgr->LoadData( "", "", buffer, true );
#else	
		pStringMgr->LoadData( "", "", pLocal->GetMemTextList(), true );
#endif
	}


	if( pLocal && !pLocal->IsRightLicense() )
	{
		MessageBox( NULL, "Your license has com up for renewal.", "IOEnter", MB_OK );
		SAFEDELETE( pLocalMgr );
		SAFEDELETE( pStringMgr );
		SAFEDELETE( pStatics );


		LOG.PrintTimeAndLog(0, "[Main] - license Error" );
		return 0;
	}
	
	HANDLE hMutex = NULL;
#ifdef SHIPPING
	hMutex = CreateMutex(NULL, true, CLIENTMUTEXNAME );
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		Help::MessageboxShow( NULL, STR(2), "LostSaga_Client - Error", MB_TOPMOST );
		SAFEDELETE( pLocalMgr );
		SAFEDELETE( pStringMgr );
		SAFEDELETE( pStatics );

		LOG.PrintTimeAndLog( 0, "[Main] - Mutex Create Failed" );
		return 0;
	}
#endif

	DWORD dwVersion;
	char  szVersion[MAX_PATH];

	if( !Help::GetDXVersionInfo( &dwVersion, szVersion, MAX_PATH ) )
	{
		SendMsgToAutoUpgrade( WM_MY_MSG_C_ERROR );
		Setting::Initialize("error");
		g_bHappenError = true;
		g_iErrorNumber = 301;
		SafeSprintf( g_szErrorLog, sizeof(g_szErrorLog), STR(3) );
		// 소켓통신이 안되는 구간이므로 ini 기록하고 나중에 전송.
		Help::SaveClientError( g_iErrorNumber, 0, 0 );

		if( hInstance )
			ShowOtherErrorDlg(hInstance);

		if(hMutex != NULL)
			CloseHandle(hMutex);
		SAFEDELETE( pLocalMgr );
		SAFEDELETE( pStringMgr );
		SAFEDELETE( pStatics );

		MessageBox( NULL, "Get DX Info Error!", "IOEnter", MB_OK );
		LOG.PrintTimeAndLog( 0, "[Main] - Get DX Info Error" );
		return 0;
	}

	if( dwVersion < 0x00090003 )	// 9.0c
	{
		SendMsgToAutoUpgrade( WM_MY_MSG_C_ERROR );
		Setting::Initialize("error");
		g_bHappenError = true;
		g_iErrorNumber = 302;
		SafeSprintf( g_szErrorLog, sizeof(g_szErrorLog), STR(4) );
		// 소켓통신이 안되는 구간이므로 ini 기록하고 나중에 전송.
		Help::SaveClientError( g_iErrorNumber, 0, 0 );

		if( hInstance )
			ShowOtherErrorDlg(hInstance);

		if(hMutex != NULL)
			CloseHandle(hMutex);
		SAFEDELETE( pLocalMgr );
		SAFEDELETE( pStringMgr );
		SAFEDELETE( pStatics );

		MessageBox( NULL, "DX Version Error!", "IOEnter", MB_OK );
		LOG.PrintTimeAndLog( 0, "[Main] - DX Version Error!" );

		return 0;
	}

	Setting::SetDirectXVersion( szVersion, dwVersion );

	Setting::CheckOSVersion();
	ioFrameTimer::SetWindowsModule( !Setting::IsWin98Series() );

	ioApplication *pApp = new ioApplication;
	if( !pApp )	
	{
		if(hMutex != NULL)
			CloseHandle(hMutex);
		SAFEDELETE( pLocalMgr );
		SAFEDELETE( pStringMgr );
		SAFEDELETE( pStatics );

		MessageBox( NULL, "Create App Error!", "IOEnter", MB_OK );
		LOG.PrintTimeAndLog( 0, "[Main] - Create App Error!" );

		return 0;
	}

//브라우저창 안띄우고 실행하려면 아래 주석을 풀고 컴파일
//#define _DIRECT_RUN_
#ifdef _DIRECT_RUN_
	int iPortOncmd = 0;
	ioHashString szKeyOnCmd, szOptionsOnCmd, szIPOnCmd;
	szOptionsOnCmd += "1"; //싱글
	szOptionsOnCmd += "0"; //리소스 업데이트
	szOptionsOnCmd += "0"; //리드팩
	szOptionsOnCmd += "0"; //팩 풀기
	szOptionsOnCmd += "2"; //모드
	szOptionsOnCmd += "0"; //풀스크린
	szOptionsOnCmd += "1"; //바운드박스
	szOptionsOnCmd += "1"; //추가 정보(EXTRA INFO)
	szOptionsOnCmd += "0"; //메모리 정보(MEMORY USAGE)
	szOptionsOnCmd += "1"; //콘솔창
	szOptionsOnCmd += "0"; //VS디버그
	szOptionsOnCmd += "0"; //폴더 카피
	szOptionsOnCmd += "1"; //모드 Sub
	szOptionsOnCmd += "9"; //모드 맵
#else

#ifdef LOCAL_DBG
	int iPortOncmd = 0;
	ioHashString szKeyOnCmd, szOptionsOnCmd, szIPOnCmd;
	char *TempCmd = lpCmdLine;
	int iSize    = strlen( TempCmd );
	int iNum = 0;
	char szTemp[MAX_PATH] ="";
	for( int i = 0 ; i < iSize ; ++i )
	{
		if( TempCmd[i] == '*')
		{
			memcpy( szTemp, TempCmd, i );
			szIPOnCmd = szTemp;
			memcpy( szTemp, TempCmd + iNum + 1, iSize );
			szKeyOnCmd = szTemp;
			break;
		}
		++iNum;
	}
	szOptionsOnCmd = "10006011011011";
	//szIPOnCmd		= "172.20.20.175";
	iPortOncmd		= 14009;
	//szKeyOnCmd		= lpCmdLine;
#endif
#ifdef LOCAL_DBG_UNPAC
	szOptionsOnCmd = "10016011111011";
#endif

//브라우저창 없이 존에 접속하기
//#define _NO_WEB_
#ifdef _NO_WEB_
#ifndef SHIPPING
	char szURL[MAX_PATH];
	char szFile[MAX_PATH];
	::GetCurrentDirectory( MAX_PATH, szFile );
	sprintf( szFile, "%s/%s", szFile, "config/debug_connect.ini" );
	FILE* fp = fopen( szFile, "rt" );
	if(fp == NULL)
	{
		MessageBox( NULL, "Debug App Error!", "IOEnter", MB_OK );
		LOG.PrintTimeAndLog( 0, "[Main] - Debug App Error!" );
		return 0;
	}
	fclose(fp);

	GetPrivateProfileString( "no_web_connect", "login_key", "", szURL, MAX_PATH, szFile );
	lpCmdLine = szURL;
#else
	MessageBox( NULL, "Debug App Error!", "IOEnter", MB_OK );
	LOG.PrintTimeAndLog( 0, "[Main] - Debug App Error!" );
#endif
#else
	pApp->DebugWebLogin(lpCmdLine);
#endif

#ifdef SHIPPING
	int iPortOncmd = 0;
	ioHashString szKeyOnCmd, szOptionsOnCmd, szIPOnCmd;
#endif
	if(!pApp->ParseCmd(lpCmdLine, szKeyOnCmd ,szOptionsOnCmd ,szIPOnCmd ,iPortOncmd))
	{
		SendMsgToAutoUpgrade( WM_MY_MSG_C_ERROR );
		if(hMutex != NULL)
			CloseHandle(hMutex);
		SAFEDELETE( pApp );
		SAFEDELETE( pLocalMgr );
		SAFEDELETE( pStringMgr );
		SAFEDELETE( pStatics );

		MessageBox( NULL, "App Cmd Error!", "IOEnter", MB_OK );
		return 0;
	}
	InitOnWinMain( szOptionsOnCmd );

	//메모리 검사부분
	LOG.PrintTimeAndLog(0, ("Memory Test") );
	pApp->LogGlobalMemory();

	// 메모리 최적화	
#ifdef SHIPPING
		trimOtherProcessesWorkingSet();
#endif

	LOG.PrintTimeAndLog( 0, "[Main] - Process Option Start" );
	if( !pApp->ProcessOptions( szOptionsOnCmd ) )
	{
		RelaseOnWinMain(pApp);
		SendMsgToAutoUpgrade( WM_MY_MSG_C_ERROR );
		if(hMutex != NULL)
			CloseHandle(hMutex);
		SAFEDELETE( pLocalMgr );
		SAFEDELETE( pStringMgr );
		SAFEDELETE( pStatics );

		MessageBox( NULL, "App Option Error!", "IOEnter", MB_OK );
		LOG.PrintTimeAndLog( 0, "[Main] - Process Option Start Failed" );
		return 0;
	}
	LOG.PrintTimeAndLog( 0, "[Main] - Process Option Start Complete" );
#endif

	// For Sticky Keys Disable
	// 초기값 백업
	SystemParametersInfo(SPI_GETSTICKYKEYS, sizeof(STICKYKEYS), &g_StartupStickyKeys, 0);
	SystemParametersInfo(SPI_GETFILTERKEYS, sizeof(FILTERKEYS), &g_StartupFilterKeys, 0);

	// 설정변경
	AllowAccessibilityShortcutKeys( false );
	
	int iRet = -1;
	//윈도우가 생성 됬을 경우에만 다음 로직을 수행하도록 변경
	if( pApp->InitWindow( hInstance, szKeyOnCmd ) )
	{
		LOG.PrintTimeAndLog( 0, "[Main] - InitWindow Complete" );

		//소켓 생성과 초기화에 성공한 경우에만 실행 하도록 변경
		if( pApp->SetupBeforeLogin( szIPOnCmd, iPortOncmd ) )
		{
			LOG.PrintTimeAndLog( 0, "[Main] - Setup Before Login Complete" );

			if( pApp->LoginLoop( szKeyOnCmd ) )
			{
#ifndef _DEBUG
				if( Help::CheckEnableTimeGapUser(1) || g_App.IsMeDeveloper() )
				{
					ioINIMemoryStatic::GetSingleton().SetSwitch( SATICS_ON );
					g_MemoryLOG.OpenLog( 0, "info/pp_mem.log");
				}
#else			
				ioINIMemoryStatic::GetSingleton().SetSwitch( SATICS_ON );
				g_MemoryLOG.OpenLog( 0, "info/pp_mem.log");
#endif
				LOG.PrintTimeAndLog( 0, "[Main] - Run Call" );
				iRet = pApp->Run();
			}
			else
			{
#ifdef SRC_TW
				char szError[MAX_PATH] = "";
				ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
				ioStringManager *pStringMgr = new ioStringManager;
				if( pStringMgr && pLocal )
				{
#ifdef SRC_OVERSEAS
					DWORD size = 0;
					const char * buffer = NULL;
					Help::LoadTextInResource(IDR_ERROR, size, buffer);
					pStringMgr->LoadData( "", "", buffer, true );
#else	
					pStringMgr->LoadData( "", "", pLocal->GetMemTextList(), true );
#endif
				}
				switch ( pApp->GetLoginResult() )
				{
				case CONNECT_BLOCK_ID:
					SafeSprintf( szError, sizeof(szError), STR(6) );
					break;
				case CONNECT_ID_NOT:
					SafeSprintf( szError, sizeof(szError), STR(7) );
					break;
				case CONNECT_PW_NOT:
					SafeSprintf( szError, sizeof(szError), STR(8) );
					break;
				case CONNECT_ID_ALREADY:
					SafeSprintf( szError, sizeof(szError), STR(9) );
					break;
				case CONNECT_EXCEPT:
					SafeSprintf( szError, sizeof(szError), STR(10) );
					break;
				case CONNECT_EXPIRE_KEY:
					SafeSprintf( szError, sizeof(szError), STR(11) );
					break;
				case CONNECT_EXITING_SERVER:
					SafeSprintf( szError, sizeof(szError), STR(12) );
					break;
				case CONNECT_WRONG_CLIENT_VER:
					SafeSprintf( szError, sizeof(szError), STR(13) );
					break;
				case CONNECT_WRONG_LOCAL_VER:
					SafeSprintf( szError, sizeof(szError), STR(14) );
					break;
				default:
					break;
				}

				LOG.PrintTimeAndLog(0, "Login Error" ); //park 여기서 찍자~
				if( 0 < pApp->GetLoginResult() && pApp->GetLoginResult() <= CONNECT_BLOCK_ID)
					Help::MessageboxShow( NULL, szError, "IOEnter", MB_OK );
#else
				LOG.PrintTimeAndLog(0, "App LoginLoop Error" );
				MessageBox( NULL, "App LoginLoop Error!", "IOEnter", MB_OK );
#endif
			}
		}
		else
		{
			LOG.PrintTimeAndLog(0, "[Main] - App SetupBeforeLogin Error" );
			MessageBox( NULL, "App SetupBeforeLogin Error!", "IOEnter", MB_OK );
		}
	}
	else
	{
		LOG.PrintTimeAndLog(0, "[Main] - App InitWindow Error" );
		MessageBox( NULL, "App InitWindow Error!", "IOEnter", MB_OK );
	}
	
	// 정상종료 : DestroyWindow->ReleaseAll->PostQuitMessage
	if( iRet == -1 )
	{
		DestroyWindow( pApp->GetHWnd() );

		MSG msg;
		for ( int i = 0; i < 10000; i++ ) // 10초간 확인
		{
			if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
			{
				if( !GetMessage( &msg, NULL, 0, 0 ) )
					break;

				TranslateMessage( &msg );
				DispatchMessage( &msg );
				continue;
			}
			::SleepEx(1, FALSE);
		}
	}

	RelaseOnWinMain(pApp);

	// For Sticky Keys Disable, 설정복구
	AllowAccessibilityShortcutKeys( true );

	if( g_bHappenCrash || g_bHappenError )
	{
		if( hInstance )
			ShowOtherErrorDlg(hInstance);
	}

	if(hMutex != NULL)
		CloseHandle(hMutex);

	SAFEDELETE( pLocalMgr );
	SAFEDELETE( pStringMgr );
	SAFEDELETE( pStatics );
	//SAFEDELETE( pSymtable );

//	 _CrtMemDumpAllObjectsSince(0);

    return iRet;
}


///////////////////////////////////////////////////////////
BOOL CALLBACK ErrorDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static char szEditText[4096 * 2];

	switch(message)
	{
	case WM_INITDIALOG:
		{
			// back skin
			if( g_pErrorDlgSkin )
			{
				g_pErrorDlgSkin->Hook(hDlg);
				g_pErrorDlgSkin->Enable(true);
			}
			//

			// edit skin
			g_hEdit = GetDlgItem( hDlg, IDC_ERROR_EDIT );
			MoveWindow(g_hEdit, 18, 47, ERROR_EDIT_WIDTH, ERROR_EDIT_HEIGHT, TRUE);
			//

			// btn 
			HWND hWndCtrl = GetDlgItem( hDlg, IDC_X );
			MoveWindow(hWndCtrl, 380, 5, ERROR_X_WIDTH, ERROR_X_HEIGHT, TRUE);
			g_OldXProc = (WNDPROC)GetWindowLong(hWndCtrl, GWL_WNDPROC);
			SetWindowLong(hWndCtrl, GWL_WNDPROC, (LONG)XProc);

			hWndCtrl = GetDlgItem( hDlg, IDC_SOLUTION );
			MoveWindow(hWndCtrl, 17, 252, ERROR_SOLUTION_WIDTH, ERROR_SOLUTION_HEIGHT, TRUE);
			g_OldSolutionProc = (WNDPROC)GetWindowLong(hWndCtrl, GWL_WNDPROC);
			SetWindowLong(hWndCtrl, GWL_WNDPROC, (LONG)SolutionProc);

			hWndCtrl = GetDlgItem( hDlg, IDOK );
			MoveWindow(hWndCtrl, 305, 252, ERROR_EXIT_WIDTH, ERROR_EXIT_HEIGHT, TRUE);
			g_OldExitProc = (WNDPROC)GetWindowLong(hWndCtrl, GWL_WNDPROC);
			SetWindowLong(hWndCtrl, GWL_WNDPROC, (LONG)ExitProc);
			//

			g_hBrushEdit = CreateSolidBrush( ERROR_EDIT_BK_RGB );

			//----------창 중간에 띄우기 ---------------------
			RECT workrect;
			// Find how large the desktop work area is
			SystemParametersInfo(SPI_GETWORKAREA, 0, &workrect, 0);
			int workwidth  = workrect.right -  workrect.left;
			int workheight = workrect.bottom - workrect.top;

			// And how big the window is
			int winwidth  = ERROR_DLG_WIDTH;
			int winheight = ERROR_DLG_HEIGHT;
			// Make sure it"s not bigger than the work area
			winwidth  = min(winwidth, workwidth);
			winheight = min(winheight, workheight);

			// Now center it
			SetWindowPos( hDlg,
						  HWND_TOP,
						  workrect.left + (workwidth-winwidth) / 2,
						  workrect.top + (workheight-winheight) / 2,
						  winwidth, winheight, 
						  SWP_SHOWWINDOW);
			//----------------------------------------------------------

			SafeSprintf(    szEditText,
				            sizeof( szEditText ),
					        STR(1),
					        Setting::GetFullSystemInfo(),
					        g_szErrorLog );

#ifdef MESSAGEBOX_UNICODE
			std::wstring strText = CA2W(szEditText, COUNTRY_CODE_PAGE);
			SetDlgItemTextW( hDlg, IDC_ERROR_EDIT, strText.c_str() );
#else
			SetDlgItemText( hDlg, IDC_ERROR_EDIT, szEditText );
#endif
			SetForegroundWindow( hDlg );

			ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
			if( pLocal && pLocal->GetResourceIDErrorSolutionN() == -1 )
			{
				HWND hChildWnd = GetDlgItem( hDlg, IDC_SOLUTION );
				EnableWindow( hChildWnd, false );
				ShowWindow( hChildWnd, SW_HIDE );
			}

			SetWindowText( hDlg, "Lost Saga" ); 
		}
		return TRUE;
	case WM_CTLCOLORSTATIC:
		if( (HWND) lParam == g_hEdit )
		{
			SetTextColor( (HDC)wParam, ERROR_EDIT_TEXT_RGB );
			SetBkColor( (HDC)wParam, ERROR_EDIT_BK_RGB );
			return (BOOL) g_hBrushEdit;
		}
		else
			return TRUE;
	case WM_DRAWITEM:	
		OnDrawItem(lParam, wParam);
		return TRUE;
	case WM_COMMAND:
		switch( LOWORD(wParam) )
		{
		case IDOK:
		case IDCANCEL:
		case IDC_X:
			EndDialog( hDlg, 0 );
			DeleteObject( g_hBrushEdit );
			return TRUE;
		case IDC_SOLUTION:
			{
				ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
				if( pLocal )
				{
					//기능블럭
					//if( g_iErrorNumber == 328 )
					//{
					//	pLocal->ShowHackShieldErrorDlg( g_MyInfo.GetPrivateID() );
					//}
					//else
					{
						char szURL[MAX_PATH]="";
						StringCbPrintf( szURL, sizeof(szURL), pLocal->GetErrorSolutionURL(), g_iErrorNumber );
						Help::ExcuteWeb( szURL );
					}
				}
			}
			return TRUE;
		}
		return TRUE;
	}

	return FALSE;
}

void ShowOtherErrorDlg( HINSTANCE hInstance )
{
	if( ioLocalManager::GetLocalType() == ioLocalManager::LCT_THAILAND )
		return;

	TCHAR szEditText[2048] = {0,};
	char szTemp[9] = "SOJINLOV";
	char szLocal[MAX_PATH] = {0,};

	DWORD LocalData = g_LocalMgr.GetLocalType();
	itoa(LocalData, szLocal, 10);

	sprintf_s(	szEditText, 
		sizeof(szEditText), 
		"%s^%s^%s^", 
		Setting::GetFullSystemInfo(), 
		g_szErrorLog,
		szLocal
		);

	char szRootDir[MAX_PATH]="";
	::GetCurrentDirectory(MAX_PATH,szRootDir);

	char szLauncherPath[MAX_PATH]="";
	StringCbPrintf(szLauncherPath, sizeof(szLauncherPath), "%s\\ErrorDlg.exe", szRootDir);

	if( access(szLauncherPath, 0) == 0 )
		Help::ExcuteProcess(szLauncherPath, szEditText);
	else
		return;
}


void ShowErrorDlg( HINSTANCE hInstance )
{
	DWORD dwBack = IDB_BACK_ERROR;
	DWORD dwSolutionN = IDB_SOLUTION_N;
	DWORD dwSolutionO = IDB_SOLUTION_O;
	DWORD dwSolutionC = IDB_SOLUTION_C;
	DWORD dwExitN = IDB_EXIT_N;
	DWORD dwExitO = IDB_EXIT_O;
	DWORD dwExitC = IDB_EXIT_C;

	ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
	if( pLocal )
	{
		if( pLocal->GetResourceIDErrorBack() != -1 )
			dwBack = pLocal->GetResourceIDErrorBack();
		if( pLocal->GetResourceIDErrorSolutionN() != -1 )
			dwSolutionN = pLocal->GetResourceIDErrorSolutionN();
		if( pLocal->GetResourceIDErrorSolutionO() != -1 )
			dwSolutionO = pLocal->GetResourceIDErrorSolutionO();
		if( pLocal->GetResourceIDErrorSolutionC() != -1 )
			dwSolutionC = pLocal->GetResourceIDErrorSolutionC();
		if( pLocal->GetResourceIDErrorExitN() != -1 )
			dwExitN = pLocal->GetResourceIDErrorExitN();
		if( pLocal->GetResourceIDErrorExitO() != -1 )
			dwExitO = pLocal->GetResourceIDErrorExitO();
		if( pLocal->GetResourceIDErrorExitC() != -1 )
			dwExitC = pLocal->GetResourceIDErrorExitC();
	}

	g_hBitmapX[0] = LoadBitmap( hInstance, MAKEINTRESOURCE(IDB_X_N) );
	g_hBitmapX[1] = LoadBitmap( hInstance, MAKEINTRESOURCE(IDB_X_O) );
	g_hBitmapX[2] = LoadBitmap( hInstance, MAKEINTRESOURCE(IDB_X_C) );
	g_hBitmapSolution[0] = LoadBitmap( hInstance, MAKEINTRESOURCE(dwSolutionN) );
	g_hBitmapSolution[1] = LoadBitmap( hInstance, MAKEINTRESOURCE(dwSolutionO) );
	g_hBitmapSolution[2] = LoadBitmap( hInstance, MAKEINTRESOURCE(dwSolutionC) );
	g_hBitmapExit[0] = LoadBitmap( hInstance, MAKEINTRESOURCE(dwExitN) );
	g_hBitmapExit[1] = LoadBitmap( hInstance, MAKEINTRESOURCE(dwExitO) );
	g_hBitmapExit[2] = LoadBitmap( hInstance, MAKEINTRESOURCE(dwExitC) );

	ShowCursor( TRUE );
	g_pErrorDlgSkin = new CSkin(IDR_BINARY1, dwBack );

	DialogBox( hInstance, MAKEINTRESOURCE(IDD_ERROR_DLG), NULL, ErrorDlgProc );
	
	SAFEDELETE( g_pErrorDlgSkin );
	for( int i=0 ; i< BTN_BITMAP_SIZE ; i++)
	{
		DeleteObject( g_hBitmapX[i] );			
		DeleteObject( g_hBitmapSolution[i] );
		DeleteObject( g_hBitmapExit[i] );
	}
}

void OnDrawItem( LPARAM lParam, WPARAM wParam )
{
	HDC	hdc;
	DRAWITEMSTRUCT *lpDi = NULL;
	lpDi = (LPDRAWITEMSTRUCT)lParam;	
	if( !lpDi )
		return;

	switch(wParam)
	{
	case IDC_X:
		hdc = CreateCompatibleDC(lpDi->hDC);
		if( lpDi->itemState & ODS_SELECTED)
			SelectObject(hdc, g_hBitmapX[2]);
		else if(g_bXOver == true)
			SelectObject(hdc, g_hBitmapX[1]);		
		else
			SelectObject(hdc, g_hBitmapX[0]);

		BitBlt(lpDi->hDC,0,0, ERROR_X_WIDTH, ERROR_X_HEIGHT, hdc, 0, 0, SRCCOPY);				
		DeleteDC(hdc);
		break;
	case IDC_SOLUTION:
		hdc = CreateCompatibleDC(lpDi->hDC);
		if( lpDi->itemState & ODS_SELECTED)
			SelectObject(hdc, g_hBitmapSolution[2]);
		else if(g_bSolutionOver == true)
			SelectObject(hdc, g_hBitmapSolution[1]);		
		else
			SelectObject(hdc, g_hBitmapSolution[0]);

		BitBlt(lpDi->hDC,0,0, ERROR_SOLUTION_WIDTH, ERROR_SOLUTION_HEIGHT, hdc, 0, 0, SRCCOPY);				
		DeleteDC(hdc);
		break;
	case IDOK:
		hdc = CreateCompatibleDC(lpDi->hDC);
		if( lpDi->itemState & ODS_SELECTED)
			SelectObject(hdc, g_hBitmapExit[2]);
		else if(g_bExitOver == true)
			SelectObject(hdc, g_hBitmapExit[1]);		
		else
			SelectObject(hdc, g_hBitmapExit[0]);

		BitBlt(lpDi->hDC,0,0, ERROR_EXIT_WIDTH, ERROR_EXIT_HEIGHT, hdc, 0, 0, SRCCOPY);				
		DeleteDC(hdc);
		break;
	}
}

LRESULT CALLBACK XProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_MOUSEMOVE:
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.dwFlags = TME_HOVER | TME_LEAVE; 
		tme.hwndTrack = hwnd;
		tme.dwHoverTime = 1; 
		TrackMouseEvent(&tme);  
		break;
	case WM_MOUSEHOVER:
		g_bXOver = true;
		InvalidateRect(hwnd, NULL, FALSE);	
		break;
	case WM_MOUSELEAVE:		
		g_bXOver = false;
		InvalidateRect(hwnd, NULL, FALSE);	
		break;
	}

	return CallWindowProc(g_OldXProc, hwnd, message, wParam, lParam);
}

LRESULT CALLBACK SolutionProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_MOUSEMOVE:
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.dwFlags = TME_HOVER | TME_LEAVE; 
		tme.hwndTrack = hwnd;
		tme.dwHoverTime = 1; 
		TrackMouseEvent(&tme);  
		break;
	case WM_MOUSEHOVER:
		g_bSolutionOver = true;
		InvalidateRect(hwnd, NULL, FALSE);	
		break;
	case WM_MOUSELEAVE:		
		g_bSolutionOver = false;
		InvalidateRect(hwnd, NULL, FALSE);	
		break;
	}	

	return CallWindowProc(g_OldXProc, hwnd, message, wParam, lParam);
}

LRESULT CALLBACK ExitProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_MOUSEMOVE:
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.dwFlags = TME_HOVER | TME_LEAVE; 
		tme.hwndTrack = hwnd;
		tme.dwHoverTime = 1; 
		TrackMouseEvent(&tme);  
		break;
	case WM_MOUSEHOVER:
		g_bExitOver = true;
		InvalidateRect(hwnd, NULL, FALSE);	
		break;
	case WM_MOUSELEAVE:		
		g_bExitOver = false;
		InvalidateRect(hwnd, NULL, FALSE);	
		break;
	}

	return CallWindowProc(g_OldXProc, hwnd, message, wParam, lParam);
}

void InitOnWinMain(ioHashString &szOptions)
{
	timeBeginPeriod(1);
	srand( timeGetTime() );	

#ifndef SHIPPING		
	if( szOptions.IsEmpty() || szOptions.At(CLogin::OT_SHOW_CONSOLWINDOW) != '0')
	{		
		LOG.OpenConsole( 1 );
	}
#endif // SHIPPING

	ioCPU::Init();
}




void RelaseOnWinMain(ioApplication *pApp)
{
	SAFEDELETE( pApp );

	ioCPU::ShotDown();
	ioResourceLoader::ReleaseInstance();

	LOG.CloseAndRelease();
	timeEndPeriod(1);
}

void SendMsgToAutoUpgrade( DWORD dwWMTYPE )
{
	HWND hWnd = FindWindow(AUTPUPGRADE_CLASS_NAME,NULL);
	if(hWnd != NULL)
	{
		static COPYDATASTRUCT cds;
		cds.dwData = dwWMTYPE;
		cds.cbData = 0;
		cds.lpData = NULL;
		SendMessage(hWnd, WM_COPYDATA, 0,(LPARAM)&cds);
	}
}

void AllowAccessibilityShortcutKeys( bool bAllowKeys )
{
    if( bAllowKeys )
    {
        // 초기값으로 복귀
        SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &g_StartupStickyKeys, 0);
        SystemParametersInfo(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &g_StartupFilterKeys, 0);
    }
    else
    {
        // 불가상태로 전환
         STICKYKEYS skOff = g_StartupStickyKeys;

		if( (skOff.dwFlags & SKF_STICKYKEYSON) == 0 )
		{
			// Disable the hotkey
			skOff.dwFlags &= ~SKF_HOTKEYACTIVE;
	 
			SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &skOff, 0);
		}
 
        FILTERKEYS fkOff = g_StartupFilterKeys;

		if( (fkOff.dwFlags & FKF_FILTERKEYSON) == 0 )
		{
			// Disable the hotkey
			fkOff.dwFlags &= ~FKF_HOTKEYACTIVE;
	 
			SystemParametersInfo(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &fkOff, 0);
		}
    }
}


void trimOtherProcessesWorkingSet()
{
	// Get the list of process identifiers.
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;

	if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
		return;

	// Calculate how many process identifiers were returned.
	cProcesses = cbNeeded / sizeof(DWORD);

	for ( i = 0; i < cProcesses; i++ )
		trimProcessWorkingSet( aProcesses[i] );
	
	// 캐시 초기화는 안 하는걸로..
	//clearSystemCache();
}

void trimProcessWorkingSet(DWORD processID)
{
	if (processID == GetCurrentProcessId())
		return;

	// Get a handle to the process.
	HANDLE hProcess = OpenProcess( PROCESS_SET_QUOTA |
		PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID );
	if (!hProcess)
		return;

	HMODULE hMods[1024];
	DWORD cbNeeded;
	//char szModName[MAX_PATH];
	char szModBaseName[MAX_PATH];

	// 가져올 수 있는 모듈에서만
	if( EnumProcessModules( hProcess, hMods, sizeof(hMods), &cbNeeded ) )
	{
		//bGetBaseName = true;
		GetModuleBaseName(hProcess, hMods[0], szModBaseName, sizeof(szModBaseName) );

		PSID pSidOwner = NULL;
		PSECURITY_DESCRIPTOR pSD = NULL;
		LPTSTR AcctName = NULL;
		LPTSTR DomainName = NULL;
		DWORD dwAcctName = 1, dwDomainName = 1;
		SID_NAME_USE eUse = SidTypeUnknown;

		BOOL dwRtnCode = GetSecurityInfo(
			hProcess,
			SE_FILE_OBJECT,
			OWNER_SECURITY_INFORMATION,
			&pSidOwner,
			NULL,
			NULL,
			NULL,
			&pSD);
		if( dwRtnCode == ERROR_SUCCESS )
		{
			BOOL bRtnBool = LookupAccountSid(
				NULL,           // local computer
				pSidOwner,
				AcctName,
				(LPDWORD)&dwAcctName,
				DomainName,
				(LPDWORD)&dwDomainName,
				&eUse);

			AcctName = (LPTSTR)GlobalAlloc(
				GMEM_FIXED,
				dwAcctName);

			// Check GetLastError for GlobalAlloc error condition.
			if (AcctName == NULL) {
				DWORD dwErrorCode = 0;
				dwErrorCode = GetLastError();
				return;
			}

			// Second call to LookupAccountSid to get the account name.
			bRtnBool = LookupAccountSid(
				NULL,                   // name of local or remote computer
				pSidOwner,              // security identifier
				AcctName,               // account name buffer
				(LPDWORD)&dwAcctName,   // size of account name buffer 
				DomainName,             // domain name
				(LPDWORD)&dwDomainName, // size of domain name buffer
				&eUse);

			//테스트로는 Admin껏만 들어오지만 혹시 모르니..
			if( bRtnBool )
			{
				//기본적으로 Admin것만 넘어오지만 혹시 몰라서 체크 넣었다가 뺌
				//const char admin[] = "Administrators";
				if( AcctName /*&& (strcmp( AcctName,admin )==0)*/  )
				{	
					EmptyWorkingSet(hProcess);
					GlobalFree(AcctName);
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////

	CloseHandle( hProcess );
}
void clearSystemCache()
{
	if (NULL == (NtSetSystemInformation = (NTSETSYSTEMINFORMATION) GetProcAddress( GetModuleHandleW(L"ntdll.dll"), "NtSetSystemInformation")))
		return;
	if (NULL == (NtQuerySystemInformation = (NTQUERYSYSTEMINFORMATION) GetProcAddress( GetModuleHandleW(L"ntdll.dll"), "NtQuerySystemInformation")))
		return;

	// Enable increase quota privilege
	HANDLE hToken;
	if (!OpenProcessToken( GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
		&hToken )) {
			printf("You do not have the necessary privilege to run this program\n");
			return;
	}

	// Enable SeDebugPrivilege
	if(SetPrivilege(hToken, SE_INCREASE_QUOTA_NAME, TRUE))
	{
		SYSTEM_CACHE_INFORMATION newCacheSize;
		ULONG returnLength;
		if (NtQuerySystemInformation( SYSTEMCACHEINFORMATION,
			&newCacheSize, sizeof(newCacheSize),
			&returnLength ) ) 
		{		
			//return;
		}
		
		newCacheSize.MinimumWorkingSet = (ULONG) -1;
		newCacheSize.MaximumWorkingSet = (ULONG) -1;
		NTSTATUS ret = NtSetSystemInformation( SYSTEMCACHEINFORMATION,	&newCacheSize, sizeof( newCacheSize));
		if( ret >= 0 )
		{
			//success
		}

		SYSTEM_FILECACHE_INFORMATION info;
		ZeroMemory(&info, sizeof(info));
		info.MinimumWorkingSet = -1;
		info.MaximumWorkingSet = -1;
		ret = NtSetSystemInformation(SystemFileCacheInformation, &info, sizeof(info));
		if( ret >= 0 )
		{
			//success
		}
	}
	else
	{
		CloseHandle(hToken);
	}

	if(SetPrivilege(hToken, SE_PROF_SINGLE_PROCESS_NAME, TRUE))
	{
		SYSTEM_MEMORY_LIST_COMMAND command = MemoryPurgeStandbyList;
		NTSTATUS ret = NtSetSystemInformation(SystemMemoryListInformation, &command, sizeof(command));
		if( ret >= 0 )
		{
			//success
		}
	}
	else
	{
		CloseHandle(hToken);
	}
	
}

BOOL SetPrivilege( HANDLE hToken, LPCTSTR Privilege, BOOL bEnablePrivilege )
{
	TOKEN_PRIVILEGES tp;
	LUID luid;
	TOKEN_PRIVILEGES tpPrevious;
	DWORD cbPrevious=sizeof(TOKEN_PRIVILEGES);

	if(!LookupPrivilegeValue( NULL, Privilege, &luid )) return FALSE;

	//
	// first pass.  get current privilege setting
	//
	tp.PrivilegeCount           = 1;
	tp.Privileges[0].Luid       = luid;
	tp.Privileges[0].Attributes = 0;

	if( !AdjustTokenPrivileges(
		hToken,
		FALSE,
		&tp,
		sizeof(TOKEN_PRIVILEGES),
		&tpPrevious,
		&cbPrevious
		)) {

			return FALSE;
	}

	//
	// second pass.  set privilege based on previous setting
	//
	tpPrevious.PrivilegeCount       = 1;
	tpPrevious.Privileges[0].Luid   = luid;
	tpPrevious.Privileges[0].Attributes  = 0;

	if(bEnablePrivilege) {
		tpPrevious.Privileges[0].Attributes |= (SE_PRIVILEGE_ENABLED);
	}
	else {
		tpPrevious.Privileges[0].Attributes ^= (SE_PRIVILEGE_ENABLED &
			tpPrevious.Privileges[0].Attributes);
	}

	return AdjustTokenPrivileges(
		hToken,
		FALSE,
		&tpPrevious,
		cbPrevious,
		NULL,
		NULL
		);
}


LONG __stdcall ExceptCallBack ( EXCEPTION_POINTERS * pExPtrs )
{
	if( g_bHappenCrash )
		return EXCEPTION_EXECUTE_HANDLER;

	g_bHappenCrash = true;

#ifdef STEAM_ATTACH
	g_ioSteamClient.End();
#endif

#ifdef HACKSHIELD
	g_ioHackShield.End();
#endif 

	Help::CaptureScreenWithJPEG();

	BOOL Success = FALSE;

	HANDLE hMiniDumpFile = CreateFile(
		_T("LostSaga.dmp"),
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH,
		NULL);

	// Write the minidump to the file
	if (hMiniDumpFile != INVALID_HANDLE_VALUE)
	{
		// 		DumpMiniDump(hMiniDumpFile, pExPtrs);

		_MINIDUMP_EXCEPTION_INFORMATION MiniDumpExceptionInfo;
		MiniDumpExceptionInfo.ThreadId			= GetCurrentThreadId();
		MiniDumpExceptionInfo.ExceptionPointers	= (PEXCEPTION_POINTERS)pExPtrs;
		MiniDumpExceptionInfo.ClientPointers		= NULL;

		Success = MiniDumpWriteDump(
			GetCurrentProcess(),
			GetCurrentProcessId(),
			hMiniDumpFile,				//덤프를 기록할 파일 핸들
			MiniDumpNormal,
			&MiniDumpExceptionInfo,	//MiniDump 예외 정보
			NULL,
			NULL);


		CloseHandle(hMiniDumpFile);
	}

	ioThreadTaskManager::ReleaseInstance();
	ioResourceLoader::ReleaseInstance();

	wsprintf( g_szErrorPos, "%s/%s/%s ErrorPos : %d, %d, %d, %d, %d, %s", g_MyInfo.GetMyIP().c_str(),
		g_MyInfo.GetPrivateID().c_str(),
		g_MyInfo.GetPublicID().c_str(),
		ErrorReport::GetMajorPosition(),
		ErrorReport::GetMinorPosition(),
		ErrorReport::GetEngineMajorPos(),
		ErrorReport::GetEngineMinorPos(),
		ErrorReport::GetAndSoOnInfo(),
		ErrorReport::GetString() );

	LOG.PrintLog( 0, "---- Crash Help Data ----");

	LOG.PrintLog( 0, "%s", g_szErrorPos );

#ifndef SHIPPING
	LOG.PrintLog( 0, "%s", g_TCPChecker.GetLOG() );
#endif

	wsprintf( g_szTempLogBuf, "%s\r\n", GetFaultReason(pExPtrs) );
	LOG.PrintNoEnterLog(0, "%s", g_szTempLogBuf);
	strcpy( g_szErrorLog, g_szTempLogBuf );

	wsprintf( g_szTempLogBuf, "%s\r\n", GetRegisterString(pExPtrs) );
	LOG.PrintNoEnterLog(0, "%s", g_szTempLogBuf);
	strcat( g_szErrorLog, g_szTempLogBuf );

	szTrace = GetFirstStackTraceString( GSTSO_SYMBOL | GSTSO_SRCLINE, pExPtrs );
	while( szTrace )
	{
		LOG.PrintLog(0,"%s" , szTrace );

		if( strlen(g_szErrorLog) + strlen(szTrace) < 2047 )
		{
			strcat( g_szErrorLog, "\r\n" );
			strcat( g_szErrorLog, szTrace );
		}
		else
		{
			break;
		}

		szTrace = GetNextStackTraceString( GSTSO_SYMBOL | GSTSO_SRCLINE, pExPtrs );
	};


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

	wsprintf( g_szSendErrorLog, "%s\n%s\n%s\n%s\n%s", g_szErrorPos, 
		g_TCPChecker.GetLOG(), 
		Setting::GetFullSystemInfo(),
		Setting::GetSettingString(),
		g_szErrorLog );

	SP2Packet kPacket( LUPK_LOG );
	kPacket << "ClientError";  // 로그 파일 타입
	kPacket << g_szSendErrorLog;
	kPacket << 317;  // 오류번호:317
	kPacket << true; // write db
	P2PNetwork::SendLog(kPacket);

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	char buf[256];
	sprintf(buf, "lsdump.exe LsVer_%s_%s_%s", STRFILEVER, g_MyInfo.GetPrivateID().c_str(), 
		g_MyInfo.GetPublicID().c_str());

	if ( CreateProcess(NULL, buf, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi) )
	{
		LOG.PrintLog(0, " Crash Files Sent ");
	}

	LOG.PrintLog(0, "---- Crash Help End ----");

	if (Success)
		return EXCEPTION_EXECUTE_HANDLER;


	return EXCEPTION_CONTINUE_EXECUTION;
}