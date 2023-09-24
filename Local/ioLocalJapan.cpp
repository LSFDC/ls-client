#include "stdafx.h"
#include ".\iolocalJapan.h"
#include "../resource.h"
#include "../ioMyInfo.h"
#include "../ioApplication.h"
#include "ioLocalJapanLanguage.h"
#include "../Setting.h"
#include "../EtcHelpFunc.h"
#include "../ioBrowserManager.h"

ioLocalJapan::ioLocalJapan(void)
{
}

ioLocalJapan::~ioLocalJapan(void)
{
}

ioLocalManager::LocalType ioLocalJapan::GetType()
{
	return ioLocalManager::LCT_JAPAN;
}

const char * ioLocalJapan::GetTextListFileName()
{
	return "text.txt";
}

const char * ioLocalJapan::GetMemTextList()
{
	return JapanLanguage::GetMemTextList(); 
}

DWORD ioLocalJapan::GetResourceIDTitleCashBmp()
{
	return IDB_WEB_TITLE_CASH_JP;
}

DWORD ioLocalJapan::GetResourceIDTitleEntryBmp()
{
	return IDB_WEB_TITLE_ENTRY_JP;
}

DWORD ioLocalJapan::GetResourceIDTitleGuildBmp()
{
	return IDB_WEB_TITLE_GUILD;
}

DWORD ioLocalJapan::GetResourceIDTitleGuildMarkBmp()
{
	return IDB_WEB_TITLE_GUILD_MARK;
}

DWORD ioLocalJapan::GetResourceIDTitleHomeBmp()
{
	return IDB_WEB_TITLE_HOME_JP;
}

DWORD ioLocalJapan::GetResourceIDTitleBmp()
{
	return IDB_WINDOW_TITLE_LOSA_JP;
}

void ioLocalJapan::SetLoginData( const ioHashString &szBillingUserKey )
{

}

void ioLocalJapan::FillLoginData( OUT SP2Packet &rkPacket )
{

}

void ioLocalJapan::GetClassTypeForTutorialDropItem( int &riClassType )
{

}


int ioLocalJapan::GetFirstIDMaxSize()
{
	return 12;
}

bool ioLocalJapan::IsShowFirstIDWnd()
{
	return false;
}

void ioLocalJapan::OpenFillCashPage()
{
	if( g_BrowserMgr.GetCashURL().IsEmpty() )
		return;

	if( Setting::FullScreen() )
		ShowWindow( g_App.GetHWnd(), SW_MINIMIZE );

	Help::ExcuteWeb( g_BrowserMgr.GetCashURL().c_str() );
}

const char * ioLocalJapan::GetGuildMasterPostion()
{
	return GUILD_MASTER_POSTION_JP;
}

const char * ioLocalJapan::GetGuildSecondMasterPosition()
{
	return GUILD_SECOND_MASTER_POSTION_JP;
}

const char * ioLocalJapan::GetGuildGeneralPosition()
{
	return GUILD_GENERAL_POSTION_JP;
}

const char * ioLocalJapan::GetGuildAdminPosition()
{
	return GUILD_GENERAL_POSTION_JP;
}

bool ioLocalJapan::IsRightNewID( const char *szID )
{
	return true;
}

bool ioLocalJapan::IsSendGashaponList()
{
	return false;
}

void ioLocalJapan::GetChangedString( OUT char *szCheckString )
{
	// �ƹ��ϵ� ���� ����
}

bool ioLocalJapan::IsExecuteLuncher()
{
	return false;
}

bool ioLocalJapan::IsKoreaGameLimit()
{
	return false;
}

bool ioLocalJapan::IsCheckKorean()
{
	return true;
}

bool ioLocalJapan::IsMyHomePage()
{
	return true;
}

bool ioLocalJapan::IsShowExtraGoodsList()
{
	return false;
}

bool ioLocalJapan::IsShowRegulation()
{
	return true;
}

DWORD ioLocalJapan::GetResourceIDErrorBack()
{
	return IDB_BACK_ERROR_JP;
}

DWORD ioLocalJapan::GetResourceIDErrorExitC()
{
	return IDB_EXIT_C_JP;
}

DWORD ioLocalJapan::GetResourceIDErrorExitN()
{
	return IDB_EXIT_N_JP;
}

DWORD ioLocalJapan::GetResourceIDErrorExitO()
{
	return IDB_EXIT_O_JP;
}

DWORD ioLocalJapan::GetResourceIDErrorSolutionC()
{
	return IDB_SOLUTION_C_JP;
}

DWORD ioLocalJapan::GetResourceIDErrorSolutionN()
{
	return IDB_SOLUTION_N_JP;
}

DWORD ioLocalJapan::GetResourceIDErrorSolutionO()
{
	return IDB_SOLUTION_O_JP;
}

const char * ioLocalJapan::GetErrorSolutionURL()
{
	return "http://www.lostsaga.jp";
}

bool ioLocalJapan::SetPresentRecvDisableMsg( int iPresentType )
{
	return false;
}

int ioLocalJapan::GetLimitGradeLevel()
{
	return -1;
}

const char * ioLocalJapan::GetBillingErrorMent(ioHashString &sServerError)
{
	return "CJIJ Billing Error : ";
}

int ioLocalJapan::GetLicenseDate()
{
	return 20150806;
}