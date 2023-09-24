#include "stdafx.h"
#include ".\ioLocalEU.h"
#include "../resource.h"
#include "../SP2Packet.h"
#include "../Setting.h"
#include "../ioApplication.h"
#include "../WndID.h"
#include "../GUI/ioSP2GUIManager.h"
#include "../ioBrowserManager.h"

ioLocalEU::ioLocalEU(void)
{
	m_nSubType = SUB_TYPE_EN;
}

ioLocalEU::~ioLocalEU(void)
{

}

ioLocalManager::LocalType ioLocalEU::GetType()
{
	return ioLocalManager::LCT_EU;
}

const char * ioLocalEU::GetTextListFileName()
{
	switch(m_nSubType)
	{
	case SUB_TYPE_DE:
		return "text_de.txt";
	case SUB_TYPE_FR:
		return "text_fr.txt";
	case SUB_TYPE_IT:
		return "text_it.txt";
	case SUB_TYPE_PL:
		return "text_pl.txt";
	case SUB_TYPE_TR:
		return "text_tr.txt";
	default:
		return "text_en.txt";
	}
}

const char *ioLocalEU::GetMemTextList()
{
	DWORD size = 0;
	const char * pChar = NULL;
	Help::LoadTextInResource(IDR_ERROR, size, pChar);
	return pChar;

	//return "|EXE_WinMain_1|An error occurred from the security program.|"//Except Extracting Hangeul
	//	   "|EXE_WinMain_2|Lost Saga is already running.|"//Except Extracting Hangeul
	//	   "|EXE_WinMain_3|    Cannot detect DirectX\r\nPlease install the latest version of DirectX.\r\n(error code: 301)|"//Except Extracting Hangeul
	//	   "|EXE_WinMain_4|The installed DirectX is not up-to-date (%s).\nPlease install the latest version of DirectX.\nMake sure that you have the latest version of the graphics card driver installed.\n(error code: 302)|"//Except Extracting Hangeul
	//	   "|EXE_ErrorDlgProc_1|--- System Requirements ---\r\n%s\r\n\r\n--- Error Information ---\r\n%s|"//Except Extracting Hangeul
	//       "|EXE_ioNProtect::GetStartErrorMsg_1|���Ӱ��尡 ���� �� �Դϴ�. ��� �ĳ� ����� �Ŀ� �ٽ� �����غ��ñ� �ٶ��ϴ�.|" //Except Extracting Hangeul
	//	   "|EXE_ioNProtect::GetStartErrorMsg_2|������ �ߺ� ����Ǿ��ų� ���Ӱ��尡 �̹� ���� �� �Դϴ�. ���� ���� �� �ٽ� �����غ��ñ� �ٶ��ϴ�.|" //Except Extracting Hangeul
	//	   "|EXE_ioNProtect::GetStartErrorMsg_3|���Ӱ��� �ʱ�ȭ �����Դϴ�. ����� �� �ٽ� �����غ��ų� �浹�� �� �ִ� �ٸ� ���α׷����� ������ �� ������ ���ñ� �ٶ��ϴ�.|" //Except Extracting Hangeul
	//	   "|EXE_ioNProtect::GetStartErrorMsg_4|���Ӱ��� ������ ���ų� �����Ǿ����ϴ�. ���Ӱ��� �¾� ������ ��ġ�غ��ñ� �ٶ��ϴ�.|" //Except Extracting Hangeul
	//	   "|EXE_ioNProtect::GetStartErrorMsg_5|�������� �Ϻ� �ý��� ������ �ջ�Ǿ����ϴ�. ���ͳ� �ͽ��÷η�(IE)�� �ٽ� ��ġ�غ��ñ� �ٶ��ϴ�.|" //Except Extracting Hangeul
	//	   "|EXE_ioNProtect::GetStartErrorMsg_6|���Ӱ��� ���࿡ �����߽��ϴ�. ���Ӱ��� �¾� ������ �ٽ� ��ġ�غ��ñ� �ٶ��ϴ�.|" //Except Extracting Hangeul
	//	   "|EXE_ioNProtect::GetStartErrorMsg_7|�ҹ� ���α׷��� �߰ߵǾ����ϴ�. ���ʿ��� ���α׷��� ������ �� �ٽ� �����غ��ñ� �ٶ��ϴ�.|" //Except Extracting Hangeul
	//	   "|EXE_ioNProtect::GetStartErrorMsg_8|���Ӱ��� ������Ʈ�� ����ϼ̽��ϴ�. ������ ��� ���� ���� ��� ���ͳ� �� ���� ��ȭ�� ������ ������ ���ñ� �ٶ��ϴ�.|" //Except Extracting Hangeul
	//	   "|EXE_ioNProtect::GetStartErrorMsg_9|���Ӱ��� ������Ʈ ���� ���ӿ� �����Ͽ����ϴ�. ��� �� �ٽ� �����ϰų�, ��Ʈ�� ���¸� �����غ��ϴ�.|" //Except Extracting Hangeul
	//	   "|EXE_ioNProtect::GetStartErrorMsg_10|���Ӱ��� �ʱ�ȭ ���� �Ǵ� �������� ���Ӱ��� �����Դϴ�. ���Ӱ��� �¾������� �ٽ� ��ġ�ϰ� ������ �����غ��ϴ�.|" //Except Extracting Hangeul
	//	   "|EXE_ioNProtect::GetStartErrorMsg_11|ini ������ ���ų� �����Ǿ����ϴ�. ���Ӱ��� �¾� ������ ��ġ�ϸ� �ذ��� �� �ֽ��ϴ�.|" //Except Extracting Hangeul
	//	   "|EXE_ioNProtect::GetStartErrorMsg_12|npgmup.des �ʱ�ȭ �����Դϴ�. ���Ӱ��������� ������ �ٽ� ���ӽ����� �غ��ϴ�.|" //Except Extracting Hangeul
	//	   "|EXE_ioNProtect::GetStartErrorMsg_13|���Ӱ��� ������Ʈ ���� ���ӿ� �����Ͽ����ϴ�. ��� �� ��õ� �غ��ų�, ���� ��ȭ���� �ִٸ� ������ ������ ���ñ� �ٶ��ϴ�.|" //Except Extracting Hangeul
	//	   "|EXE_ioNProtect::GetStartErrorMsg_14|���Ӱ��� ������Ʈ�� �Ϸ����� �� �߽��ϴ�. ���̷��� ����� �Ͻ� �߽� ��Ų �� ��õ� �غ��ðų�, PC ���� ���α׷��� ����Ͻø� ������ ������ ���ñ� �ٶ��ϴ�.|" //Except Extracting Hangeul
	//	   "|EXE_ioNProtect::GetStartErrorMsg_15|���̷��� �� ��ŷ�� �˻� ��� �ε��� ���� �߽��ϴ�. �޸� �����̰ų� ���̷����� ���� ������ �� �ֽ��ϴ�.|" //Except Extracting Hangeul
	//	   "|EXE_ioNProtect::GetStartErrorMsg_16|���Ӱ���� �浹 ���α׷��� �߰ߵǾ����ϴ�.|" //Except Extracting Hangeul
	//	   "|EXE_ioNProtect::GetStartErrorMsg_17|���Ӱ��� ���� �� ������ �߻��Ͽ����ϴ�. ���� ���� ���� GameGuard ������ �ִ� *.erl ���ϵ��� Game1@inca.co.kr�� ÷���Ͽ� ���� �����ֽñ� �ٶ��ϴ�.|"; //Except Extracting Hangeul  
}

DWORD ioLocalEU::GetResourceIDTitleCashBmp()
{
	return IDB_WEB_TITLE_CASH_US;
}

DWORD ioLocalEU::GetResourceIDTitleEntryBmp()
{
	return IDB_WEB_TITLE_ENTRY_US;
}

DWORD ioLocalEU::GetResourceIDTitleGuildBmp()
{
	return IDB_WEB_TITLE_GUILD_US;
}

DWORD ioLocalEU::GetResourceIDTitleGuildMarkBmp()
{
	return IDB_WEB_TITLE_GUILD_MARK_US;
}

DWORD ioLocalEU::GetResourceIDTitleHomeBmp()
{
	return IDB_WEB_TITLE_HOME_US;
}

DWORD ioLocalEU::GetResourceIDTitleBmp()
{
	return IDB_WINDOW_TITLE_LOSA;
}

void ioLocalEU::SetLoginData( const ioHashString &szBillingUserKey )
{
	m_sLoginEncodePW = szBillingUserKey;
	StringCbCopy(g_LocalMgr.m_szNexonPassport, sizeof(g_LocalMgr.m_szNexonPassport), m_sLoginEncodePW.c_str());
}

void ioLocalEU::FillLoginData( SP2Packet &rkPacket )
{
	rkPacket << m_sLoginEncodePW;

	// ���̻� �ʿ䰡 �����Ƿ� ����
	m_sLoginEncodePW.Clear();
}


void ioLocalEU::GetClassTypeForTutorialDropItem( int &riClassType )
{
	enum { CLASS_TYPE_SHADOW = 12, };
	riClassType = CLASS_TYPE_SHADOW;
}


int ioLocalEU::GetFirstIDMaxSize()
{
	return 12;
}

bool ioLocalEU::IsShowFirstIDWnd()
{
	return true;
}

void ioLocalEU::OpenFillCashPage()
{
	if( g_BrowserMgr.GetCashURL().IsEmpty() )
		return;

	if( Setting::FullScreen() )
		ShowWindow( g_App.GetHWnd(), SW_MINIMIZE );

	Help::ExcuteWeb( g_BrowserMgr.GetCashURL().c_str() );
}

const char * ioLocalEU::GetGuildMasterPostion()
{
	return "Leader"; //Except Extracting Hangeul
}

const char * ioLocalEU::GetGuildSecondMasterPosition()
{
	return "Officer"; //Except Extracting Hangeul
}

const char * ioLocalEU::GetGuildGeneralPosition()
{
	return "Member"; //Except Extracting Hangeul
}

const char * ioLocalEU::GetGuildAdminPosition()
{
	return "Member"; //Except Extracting Hangeul
}

bool ioLocalEU::IsRightNewID( const char *szID )
{
	int size = strlen(szID);
	for (int i=0; i<size; i++)
	{
		if ((!COMPARE(szID[i], 'A', 'Z'+1)) &&
			(!COMPARE(szID[i], 'a', 'z'+1)) &&
			(!COMPARE(szID[i], '0', '9'+1)) &&
					  szID[i] != '!' &&
			          szID[i] != '$' &&
					  szID[i] != '^' &&
					  szID[i] != '*' &&
					  szID[i] != '(' &&
					  szID[i] != ')' &&
					  szID[i] != '_' &&
					  szID[i] != '-' &&
					  szID[i] != '=' &&
					  szID[i] != '+' &&
					  szID[i] != '|' &&
					  szID[i] != '[' &&
					  szID[i] != ']' &&
					  szID[i] != '{' &&
					  szID[i] != '}' &&
					  szID[i] != '<' &&
					  szID[i] != '>' &&
					  szID[i] != '?' &&
					  szID[i] != '~' &&
					  szID[i] != '.' &&
					  szID[i] != ',' )
		{
			return false;
		}
	}
	return true;
}

bool ioLocalEU::IsSendGashaponList()
{
	return true;
}

void ioLocalEU::GetChangedString( OUT char *szCheckString )
{
	strlwr( szCheckString );
}

bool ioLocalEU::IsExecuteLuncher()
{
	return false;
}

bool ioLocalEU::IsKoreaGameLimit()
{
	return false;
}

bool ioLocalEU::IsCheckKorean()
{
	return false;
}

bool ioLocalEU::IsMyHomePage()
{
	return false;
}

bool ioLocalEU::IsShowExtraGoodsList()
{
	return true;
}

bool ioLocalEU::IsShowRegulation()
{
	return false;
}

DWORD ioLocalEU::GetResourceIDErrorBack()
{
	return IDB_BACK_ERROR_US;
}

DWORD ioLocalEU::GetResourceIDErrorExitC()
{
	return IDB_EXIT_C_US;
}

DWORD ioLocalEU::GetResourceIDErrorExitN()
{
	return IDB_EXIT_N_US;
}

DWORD ioLocalEU::GetResourceIDErrorExitO()
{
	return IDB_EXIT_O_US;
}

DWORD ioLocalEU::GetResourceIDErrorSolutionC()
{
	return IDB_SOLUTION_C_US;
}

DWORD ioLocalEU::GetResourceIDErrorSolutionN()
{
	return IDB_SOLUTION_N_US;
}

DWORD ioLocalEU::GetResourceIDErrorSolutionO()
{
	return IDB_SOLUTION_O_US;
}

const char * ioLocalEU::GetErrorSolutionURL()
{
	return "http://support.z8games.com/lostsaga/";
}

bool ioLocalEU::SetPresentRecvDisableMsg( int iPresentType )
{
	return false;
}

int ioLocalEU::GetLimitGradeLevel()
{
	return -1;
}

const char * ioLocalEU::GetBillingErrorMent(ioHashString &sServerError)
{
	return "Billing Error";
}

int ioLocalEU::GetLicenseDate()
{
	return 20150806;
}

void ioLocalEU::SetWebLoginKey( const ioHashString &rsWebLoginKey )
{
	m_sWebLoginKey = rsWebLoginKey;
}
