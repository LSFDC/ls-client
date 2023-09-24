#include "stdafx.h"
#include ".\iolocaltaiwan.h"
#include "../resource.h"
#include "../ioPresentMgr.h"
#include "../GUI/ioSP2GUIManager.h"
#include "../Setting.h"
#include "../ioApplication.h"
#include "../EtcHelpFunc.h"
#include "../ioBrowserManager.h"
#include "ioLocalTaiwanLanguage.h"

ioLocalTaiwan::ioLocalTaiwan(void)
{
}

ioLocalTaiwan::~ioLocalTaiwan(void)
{
}

ioLocalManager::LocalType ioLocalTaiwan::GetType()
{
	return ioLocalManager::LCT_TAIWAN;
}

const char * ioLocalTaiwan::GetTextListFileName()
{
	return "text_tw.txt";
}

DWORD ioLocalTaiwan::GetResourceIDTitleCashBmp()
{
	return IDB_WEB_TITLE_CASH_TW;
}

DWORD ioLocalTaiwan::GetResourceIDTitleEntryBmp()
{
	return IDB_WEB_TITLE_ENTRY_TW;
}

DWORD ioLocalTaiwan::GetResourceIDTitleGuildBmp()
{
	return IDB_WEB_TITLE_GUILD_TW;
}

DWORD ioLocalTaiwan::GetResourceIDTitleGuildMarkBmp()
{
	return IDB_WEB_TITLE_GUILD_MARK_TW;
}

DWORD ioLocalTaiwan::GetResourceIDTitleHomeBmp()
{
	return IDB_WEB_TITLE_HOME_TW;
}

DWORD ioLocalTaiwan::GetResourceIDTitleBmp()
{
	return IDB_WINDOW_TITLE_LOSA_TW;
}


void ioLocalTaiwan::SetLoginData( const ioHashString &szBillingUserKey )
{
	m_sLoginEncodePW = szBillingUserKey;
}

void ioLocalTaiwan::FillLoginData( OUT SP2Packet &rkPacket )
{
	rkPacket << m_sLoginEncodePW;

	// ���̻� �ʿ䰡 �����Ƿ� ����
	m_sLoginEncodePW.Clear();
}

void ioLocalTaiwan::GetClassTypeForTutorialDropItem( int &riClassType )
{

}


int ioLocalTaiwan::GetFirstIDMaxSize()
{
	return 12;
}

bool ioLocalTaiwan::IsShowFirstIDWnd()
{
	return true;
}

void ioLocalTaiwan::OpenFillCashPage()
{
	if( g_BrowserMgr.GetCashURL().IsEmpty() )
		return;

	if( Setting::FullScreen() )
		ShowWindow( g_App.GetHWnd(), SW_MINIMIZE );
			
	Help::ExcuteWeb( g_BrowserMgr.GetCashURL().c_str() );
}

const char * ioLocalTaiwan::GetGuildAdminPosition()
{
	return "Admin";
}

bool ioLocalTaiwan::IsRightNewID( const char *szID )
{
	return true;
}

bool ioLocalTaiwan::IsSendGashaponList()
{
	return false;
}

void ioLocalTaiwan::GetChangedString( OUT char *szCheckString )
{
	// �ƹ��ϵ� ���� ����
}

bool ioLocalTaiwan::IsExecuteLuncher()
{
	return false;
}

bool ioLocalTaiwan::IsKoreaGameLimit()
{
	return false;
}

bool ioLocalTaiwan::IsCheckKorean()
{
	return false;
}

bool ioLocalTaiwan::IsMyHomePage()
{
	return false;
}

bool ioLocalTaiwan::IsShowExtraGoodsList()
{
	return false;
}

bool ioLocalTaiwan::IsShowRegulation()
{
	return false;
}

DWORD ioLocalTaiwan::GetResourceIDErrorBack()
{
	return IDB_BACK_ERROR_TW;
}

DWORD ioLocalTaiwan::GetResourceIDErrorExitC()
{
	return IDB_EXIT_C_TW;
}

DWORD ioLocalTaiwan::GetResourceIDErrorExitN()
{
	return IDB_EXIT_N_TW;
}

DWORD ioLocalTaiwan::GetResourceIDErrorExitO()
{
	return IDB_EXIT_O_TW;
}

DWORD ioLocalTaiwan::GetResourceIDErrorSolutionC()
{
	return IDB_SOLUTION_C_TW;
}

DWORD ioLocalTaiwan::GetResourceIDErrorSolutionN()
{
	return IDB_SOLUTION_N_TW;
}

DWORD ioLocalTaiwan::GetResourceIDErrorSolutionO()
{
	return IDB_SOLUTION_O_TW;
}

const char * ioLocalTaiwan::GetErrorSolutionURL()
{
	return "http://lostsaga.gamon.net/app/help/faq.aspx";
}

bool ioLocalTaiwan::SetPresentRecvDisableMsg( int iPresentType )
{
	return false;
}

int ioLocalTaiwan::GetLimitGradeLevel()
{
	return -1;
}

const char * ioLocalTaiwan::GetBillingErrorMent(ioHashString &sServerError)
{
	return "GaMonster Billing Error : ";
}

int ioLocalTaiwan::GetLicenseDate()
{
	return 30160806;
}

const char * ioLocalTaiwan::GetMemTextList()
{
// 	DWORD size = 0;
// 	const char * pChar = NULL;
// 	Help::LoadTextInResource(IDR_ERROR, size, pChar);
// 	return pChar;
	return TaiwanLanguage::GetMemTextList();
}
