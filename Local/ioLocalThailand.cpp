#include "stdafx.h"
#include ".\iolocalThailand.h"
#include "../resource.h"
#include "../ioMyInfo.h"
#include "../ioApplication.h"
#include "ioLocalThailandLanguage.h"
#include "../EtcHelpFunc.h"
#include "../ioBrowserManager.h"
#include "../Setting.h"

ioLocalThailand::ioLocalThailand(void)
{
}

ioLocalThailand::~ioLocalThailand(void)
{
}

ioLocalManager::LocalType ioLocalThailand::GetType()
{
	return ioLocalManager::LCT_THAILAND;
}

const char * ioLocalThailand::GetTextListFileName()
{
	return "text_th.txt";
}

const char * ioLocalThailand::GetMemTextList()
{
	DWORD size = 0;
	const char * pChar = NULL;
	Help::LoadTextInResource(IDR_ERROR, size, pChar);
	return pChar;
	//return ThailandLanguage::GetMemTextList(); 
}

DWORD ioLocalThailand::GetResourceIDTitleCashBmp()
{
	return IDB_WEB_TITLE_CASH_TH;
}

DWORD ioLocalThailand::GetResourceIDTitleEntryBmp()
{
	return IDB_WEB_TITLE_ENTRY_TH;
}

DWORD ioLocalThailand::GetResourceIDTitleGuildBmp()
{
	return IDB_WEB_TITLE_GUILD_TH;
}

DWORD ioLocalThailand::GetResourceIDTitleGuildMarkBmp()
{
	return IDB_WEB_TITLE_GUILD_MARK_TH;
}

DWORD ioLocalThailand::GetResourceIDTitleHomeBmp()
{
	return IDB_WEB_TITLE_HOME_TH;
}

DWORD ioLocalThailand::GetResourceIDTitleBmp()
{
	return IDB_WINDOW_TITLE_LOSA;
}

void ioLocalThailand::SetLoginData( const ioHashString &szBillingUserKey )
{
	m_sLoginEncodePW = szBillingUserKey;
}

void ioLocalThailand::FillLoginData( OUT SP2Packet &rkPacket )
{
	rkPacket << m_sLoginEncodePW;

	// 더이상 필요가 없으므로 삭제
	m_sLoginEncodePW.Clear();
}

void ioLocalThailand::GetClassTypeForTutorialDropItem( int &riClassType )
{

}


int ioLocalThailand::GetFirstIDMaxSize()
{
	return 12;
}

bool ioLocalThailand::IsShowFirstIDWnd()
{
	return true;
}

void ioLocalThailand::OpenFillCashPage()
{
	if( Setting::FullScreen() )
		ShowWindow( g_App.GetHWnd(), SW_MINIMIZE );

	Help::ExcuteWeb( g_BrowserMgr.GetCashURL().c_str() );
}

const char * ioLocalThailand::GetGuildAdminPosition()
{
	return "Admin"; //Except Extracting Hangeul
}

bool ioLocalThailand::IsRightNewID( const char *szID )
{
	return true;
}

bool ioLocalThailand::IsSendGashaponList()
{
	return true;
}

void ioLocalThailand::GetChangedString( OUT char *szCheckString )
{
	// 아무일도 하지 않음
}

bool ioLocalThailand::IsExecuteLuncher()
{
	return false;
}

bool ioLocalThailand::IsKoreaGameLimit()
{
	return true;
}

bool ioLocalThailand::IsCheckKorean()
{
	return false;
}

bool ioLocalThailand::IsMyHomePage()
{
	return false;
}

bool ioLocalThailand::IsShowExtraGoodsList()
{
	return false;
}

bool ioLocalThailand::IsShowRegulation()
{
	return false;
}

DWORD ioLocalThailand::GetResourceIDErrorBack()
{
	return IDB_BACK_ERROR_TH;
}

DWORD ioLocalThailand::GetResourceIDErrorExitC()
{
	return IDB_EXIT_C_TH;
}

DWORD ioLocalThailand::GetResourceIDErrorExitN()
{
	return IDB_EXIT_N_TH;
}

DWORD ioLocalThailand::GetResourceIDErrorExitO()
{
	return IDB_EXIT_O_TH;
}

DWORD ioLocalThailand::GetResourceIDErrorSolutionC()
{
	return IDB_SOLUTION_C_TH;
}

DWORD ioLocalThailand::GetResourceIDErrorSolutionN()
{
	return IDB_SOLUTION_N_TH;
}

DWORD ioLocalThailand::GetResourceIDErrorSolutionO()
{
	return IDB_SOLUTION_O_TH;
}

const char * ioLocalThailand::GetErrorSolutionURL()
{
	return "";
}

bool ioLocalThailand::SetPresentRecvDisableMsg( int iPresentType )
{
	return false;
}

int ioLocalThailand::GetLimitGradeLevel()
{
	return -1;
}

const char * ioLocalThailand::GetBillingErrorMent(ioHashString &sServerError)
{
	return "Garena Billing Error : ";
}

int ioLocalThailand::GetLicenseDate()
{
	return 20160806;
}

void ioLocalThailand::ExitAction()
{
//	Help::ExcuteWeb( "http://apps.garena.in.th/lostsaga/webgacha1/" );
}


void ioLocalThailand::ShowHackShieldErrorDlg( const ioHashString &rsPrivateID )
{
#ifdef HACKSHIELD
	g_ioHackShield.ShowErrorDlg( rsPrivateID );
#endif
}

bool ioLocalThailand::IsPointBlankPremium( int iClassType )
{
	if( iClassType == 110 )
		return true;

	return false;
}

void ioLocalThailand::OpenWebPage( int iType )
{
	if( Setting::FullScreen() )
		ShowWindow( g_App.GetHWnd(), SW_MINIMIZE );

	if( g_BrowserMgr.GetWebUrl( iType).IsEmpty() )
		return;

	if( iType == ioLocalParent::WEB_EVENT  )
		Help::ExcuteWeb( g_BrowserMgr.GetWebUrl( ioLocalParent::WEB_EVENT ).c_str() );
	if( iType == ioLocalParent::WEB_GUIDE )
		Help::ExcuteWeb( g_BrowserMgr.GetWebUrl( ioLocalParent::WEB_GUIDE ).c_str() );
}


// void ioLocalThailand::LoadINI()
// {
// 	ioLocalParent::LoadINI();
// 
// 	// 처음 셋팅시에 빈공간을 넣었기 때문에 DB와 동일하게 아래와 같이 빈공간을 넣는다.
// 	char szGuildMaster[MAX_PATH]="";
// 	StringCbPrintf( szGuildMaster, sizeof( szGuildMaster ), "%s ", m_sGuildMaster.c_str() );
// 	m_sGuildMaster = szGuildMaster;
// 
// 	char szGuildSecondMaster[MAX_PATH]="";
// 	StringCbPrintf( szGuildSecondMaster, sizeof( szGuildSecondMaster ), "%s ", m_sGuildSecondMaster.c_str() );
// 	m_sGuildSecondMaster = szGuildSecondMaster;
// 
// 	char szGuildGeneral[MAX_PATH]="";
// 	StringCbPrintf( szGuildGeneral, sizeof( szGuildGeneral ), "%s ", m_sGuildGeneral.c_str() );
// 	m_sGuildGeneral = szGuildGeneral;
// }
