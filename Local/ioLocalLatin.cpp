#include "stdafx.h"
#include ".\ioLocalLatin.h"
#include "../resource.h"
#include "../SP2Packet.h"
#include "../Setting.h"
#include "../ioApplication.h"
#include "../WndID.h"
#include "../GUI/ioSP2GUIManager.h"
#include "../ioBrowserManager.h"

ioLocalLatin::ioLocalLatin(void)
{
}

ioLocalLatin::~ioLocalLatin(void)
{

}

ioLocalManager::LocalType ioLocalLatin::GetType()
{
	return ioLocalManager::LCT_LATIN;
}

const char * ioLocalLatin::GetTextListFileName()
{
	return "text_sa.txt";
}

const char *ioLocalLatin::GetMemTextList()
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

DWORD ioLocalLatin::GetResourceIDTitleCashBmp()
{
	return IDB_WEB_TITLE_CASH_US;
}

DWORD ioLocalLatin::GetResourceIDTitleEntryBmp()
{
	return IDB_WEB_TITLE_ENTRY_US;
}

DWORD ioLocalLatin::GetResourceIDTitleGuildBmp()
{
	return IDB_WEB_TITLE_GUILD_US;
}

DWORD ioLocalLatin::GetResourceIDTitleGuildMarkBmp()
{
	return IDB_WEB_TITLE_GUILD_MARK_US;
}

DWORD ioLocalLatin::GetResourceIDTitleHomeBmp()
{
	return IDB_WEB_TITLE_HOME_US;
}

DWORD ioLocalLatin::GetResourceIDTitleBmp()
{
	return IDB_WINDOW_TITLE_LOSA;
}

void ioLocalLatin::SetLoginData( const ioHashString &szBillingUserKey )
{
	m_sLoginEncodePW = szBillingUserKey;
}

void ioLocalLatin::FillLoginData( SP2Packet &rkPacket )
{
	rkPacket << m_sLoginEncodePW;

	// ���̻� �ʿ䰡 �����Ƿ� ����
	m_sLoginEncodePW.Clear();
}


void ioLocalLatin::GetClassTypeForTutorialDropItem( int &riClassType )
{
	enum { CLASS_TYPE_SHADOW = 12, };
	riClassType = CLASS_TYPE_SHADOW;
}


int ioLocalLatin::GetFirstIDMaxSize()
{
	return 12;
}

bool ioLocalLatin::IsShowFirstIDWnd()
{
	return true;
}

void ioLocalLatin::OpenFillCashPage()
{
	if( g_BrowserMgr.GetCashURL().IsEmpty() )
		return;

	if( Setting::FullScreen() )
		ShowWindow( g_App.GetHWnd(), SW_MINIMIZE );

	Help::ExcuteWeb( g_BrowserMgr.GetCashURL().c_str() );
}

const char * ioLocalLatin::GetGuildMasterPostion()
{
	return "Leader"; //Except Extracting Hangeul
}

const char * ioLocalLatin::GetGuildSecondMasterPosition()
{
	return "Officer"; //Except Extracting Hangeul
}

const char * ioLocalLatin::GetGuildGeneralPosition()
{
	return "Member"; //Except Extracting Hangeul
}

const char * ioLocalLatin::GetGuildAdminPosition()
{
	return "Member"; //Except Extracting Hangeul
}

bool ioLocalLatin::IsRightNewID( const char *szID )
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

bool ioLocalLatin::IsSendGashaponList()
{
	return true;
}

void ioLocalLatin::GetChangedString( OUT char *szCheckString )
{
	strlwr( szCheckString );
}

bool ioLocalLatin::IsExecuteLuncher()
{
	return false;
}

bool ioLocalLatin::IsKoreaGameLimit()
{
	return false;
}

bool ioLocalLatin::IsCheckKorean()
{
	return false;
}

bool ioLocalLatin::IsMyHomePage()
{
	return false;
}

bool ioLocalLatin::IsShowExtraGoodsList()
{
	return true;
}

bool ioLocalLatin::IsShowRegulation()
{
	return false;
}

DWORD ioLocalLatin::GetResourceIDErrorBack()
{
	return IDB_BACK_ERROR_US;
}

DWORD ioLocalLatin::GetResourceIDErrorExitC()
{
	return IDB_EXIT_C_US;
}

DWORD ioLocalLatin::GetResourceIDErrorExitN()
{
	return IDB_EXIT_N_US;
}

DWORD ioLocalLatin::GetResourceIDErrorExitO()
{
	return IDB_EXIT_O_US;
}

DWORD ioLocalLatin::GetResourceIDErrorSolutionC()
{
	return IDB_SOLUTION_C_US;
}

DWORD ioLocalLatin::GetResourceIDErrorSolutionN()
{
	return IDB_SOLUTION_N_US;
}

DWORD ioLocalLatin::GetResourceIDErrorSolutionO()
{
	return IDB_SOLUTION_O_US;
}

const char * ioLocalLatin::GetErrorSolutionURL()
{
	return "http://support.z8games.com/lostsaga/";
}

bool ioLocalLatin::SetPresentRecvDisableMsg( int iPresentType )
{
	return false;
}

int ioLocalLatin::GetLimitGradeLevel()
{
	return -1;
}

const char * ioLocalLatin::GetBillingErrorMent(ioHashString &sServerError)
{
	return "Billing Error";
}

int ioLocalLatin::GetLicenseDate()
{
	return 20150806;
}

void ioLocalLatin::SetWebLoginKey( const ioHashString &rsWebLoginKey )
{
	m_sWebLoginKey = rsWebLoginKey;
}
