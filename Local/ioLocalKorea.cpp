#include "stdafx.h"
#include ".\iolocalkorea.h"
#include "../resource.h"
#include "../ioMyInfo.h"
#include "../ioApplication.h"
#include "../Channeling/ioChannelingNodeParent.h"
#include "../Channeling/ioChannelingNodeManager.h"

ioLocalKorea::ioLocalKorea(void)
{
}

ioLocalKorea::~ioLocalKorea(void)
{
}

ioLocalManager::LocalType ioLocalKorea::GetType()
{
	return ioLocalManager::LCT_KOREA;
}

const char * ioLocalKorea::GetTextListFileName()
{
	return "text.txt";
}

const char * ioLocalKorea::GetMemTextList()
{
	return "|EXE_WinMain_1|�������α׷����� ������ �߻��Ͽ����ϴ�.|" //Except Extracting Hangeul
		   "|EXE_WinMain_2|�ν�Ʈ�簡�� �̹� ���� �� �Դϴ�.|" //Except Extracting Hangeul
		   "|EXE_WinMain_3|    DirectX�� ã���� �����ϴ�\r\n�ֽŹ����� DirectX�� ��ġ���ּ���.\r\n(������ȣ:301)|" //Except Extracting Hangeul
		   "|EXE_WinMain_4|��ġ�� DirectX�� ������ �����ϴ�(%s)\n�ֽŹ����� DirectX�� ��ġ���ּ���\n�׷���ī���� ����̹��� �ֽŹ������� Ȯ�����ּ���\n(������ȣ:302)|" //Except Extracting Hangeul
		   "|EXE_WinMain_5|���Ӱ��� ���� : %d|" //Except Extracting Hangeul
	       "|EXE_ErrorDlgProc_1|--- �ý��� ��� ---\r\n%s\r\n\r\n--- ���� ���� ---\r\n%s|" //Except Extracting Hangeul
	       "|EXE_ioNProtect::GetStartErrorMsg_1|���Ӱ��尡 ���� �� �Դϴ�. ��� �ĳ� ����� �Ŀ� �ٽ� �����غ��ñ� �ٶ��ϴ�.|" //Except Extracting Hangeul
		   "|EXE_ioNProtect::GetStartErrorMsg_2|������ �ߺ� ����Ǿ��ų� ���Ӱ��尡 �̹� ���� �� �Դϴ�. ���� ���� �� �ٽ� �����غ��ñ� �ٶ��ϴ�.|" //Except Extracting Hangeul
		   "|EXE_ioNProtect::GetStartErrorMsg_3|���Ӱ��� �ʱ�ȭ �����Դϴ�. ����� �� �ٽ� �����غ��ų� �浹�� �� �ִ� �ٸ� ���α׷����� ������ �� ������ ���ñ� �ٶ��ϴ�.|" //Except Extracting Hangeul
		   "|EXE_ioNProtect::GetStartErrorMsg_4|���Ӱ��� ������ ���ų� �����Ǿ����ϴ�. ���Ӱ��� �¾� ������ ��ġ�غ��ñ� �ٶ��ϴ�.|" //Except Extracting Hangeul
		   "|EXE_ioNProtect::GetStartErrorMsg_5|�������� �Ϻ� �ý��� ������ �ջ�Ǿ����ϴ�. ���ͳ� �ͽ��÷η�(IE)�� �ٽ� ��ġ�غ��ñ� �ٶ��ϴ�.|" //Except Extracting Hangeul
		   "|EXE_ioNProtect::GetStartErrorMsg_6|���Ӱ��� ���࿡ �����߽��ϴ�. ���Ӱ��� �¾� ������ �ٽ� ��ġ�غ��ñ� �ٶ��ϴ�.|" //Except Extracting Hangeul
		   "|EXE_ioNProtect::GetStartErrorMsg_7|�ҹ� ���α׷��� �߰ߵǾ����ϴ�. ���ʿ��� ���α׷��� ������ �� �ٽ� �����غ��ñ� �ٶ��ϴ�.|" //Except Extracting Hangeul
		   "|EXE_ioNProtect::GetStartErrorMsg_8|���Ӱ��� ������Ʈ�� ����ϼ̽��ϴ�. ������ ��� ���� ���� ��� ���ͳ� �� ���� ��ȭ�� ������ ������ ���ñ� �ٶ��ϴ�.|" //Except Extracting Hangeul
		   "|EXE_ioNProtect::GetStartErrorMsg_9|���Ӱ��� ������Ʈ ���� ���ӿ� �����Ͽ����ϴ�. ��� �� �ٽ� �����ϰų�, ��Ʈ�� ���¸� �����غ��ϴ�.|" //Except Extracting Hangeul
		   "|EXE_ioNProtect::GetStartErrorMsg_10|���Ӱ��� �ʱ�ȭ ���� �Ǵ� �������� ���Ӱ��� �����Դϴ�. ���Ӱ��� �¾������� �ٽ� ��ġ�ϰ� ������ �����غ��ϴ�.|" //Except Extracting Hangeul
		   "|EXE_ioNProtect::GetStartErrorMsg_11|ini ������ ���ų� �����Ǿ����ϴ�. ���Ӱ��� �¾� ������ ��ġ�ϸ� �ذ��� �� �ֽ��ϴ�.|" //Except Extracting Hangeul
		   "|EXE_ioNProtect::GetStartErrorMsg_12|npgmup.des �ʱ�ȭ �����Դϴ�. ���Ӱ��������� ������ �ٽ� ���ӽ����� �غ��ϴ�.|" //Except Extracting Hangeul
		   "|EXE_ioNProtect::GetStartErrorMsg_13|���Ӱ��� ������Ʈ ���� ���ӿ� �����Ͽ����ϴ�. ��� �� ��õ� �غ��ų�, ���� ��ȭ���� �ִٸ� ������ ������ ���ñ� �ٶ��ϴ�.|" //Except Extracting Hangeul
		   "|EXE_ioNProtect::GetStartErrorMsg_14|���Ӱ��� ������Ʈ�� �Ϸ����� �� �߽��ϴ�. ���̷��� ����� �Ͻ� �߽� ��Ų �� ��õ� �غ��ðų�, PC ���� ���α׷��� ����Ͻø� ������ ������ ���ñ� �ٶ��ϴ�.|" //Except Extracting Hangeul
		   "|EXE_ioNProtect::GetStartErrorMsg_15|���̷��� �� ��ŷ�� �˻� ��� �ε��� ���� �߽��ϴ�. �޸� �����̰ų� ���̷����� ���� ������ �� �ֽ��ϴ�.|" //Except Extracting Hangeul
		   "|EXE_ioNProtect::GetStartErrorMsg_16|���Ӱ���� �浹 ���α׷��� �߰ߵǾ����ϴ�.|" //Except Extracting Hangeul
		   "|EXE_ioNProtect::GetStartErrorMsg_17|���Ӱ��� ���� �� ������ �߻��Ͽ����ϴ�. ���� ���� ���� GameGuard ������ �ִ� *.erl ���ϵ��� Game1@inca.co.kr�� ÷���Ͽ� ���� �����ֽñ� �ٶ��ϴ�.|"; //Except Extracting Hangeul  
}

DWORD ioLocalKorea::GetResourceIDTitleCashBmp()
{
	return IDB_WEB_TITLE_CASH;
}

DWORD ioLocalKorea::GetResourceIDTitleEntryBmp()
{
	return IDB_WEB_TITLE_ENTRY;
}

DWORD ioLocalKorea::GetResourceIDTitleGuildBmp()
{
	return IDB_WEB_TITLE_GUILD;
}

DWORD ioLocalKorea::GetResourceIDTitleGuildMarkBmp()
{
	return IDB_WEB_TITLE_GUILD_MARK;
}

DWORD ioLocalKorea::GetResourceIDTitleHomeBmp()
{
	return IDB_WEB_TITLE_HOME;
}

DWORD ioLocalKorea::GetResourceIDTitleBmp()
{
	return IDB_WINDOW_TITLE_LOSA;
}

void ioLocalKorea::SetLoginData( const ioHashString &szBillingUserKey )
{

}

void ioLocalKorea::FillLoginData( OUT SP2Packet &rkPacket )
{

}

void ioLocalKorea::GetClassTypeForTutorialDropItem( int &riClassType )
{

}


int ioLocalKorea::GetFirstIDMaxSize()
{
	return 12;
}

bool ioLocalKorea::IsShowFirstIDWnd()
{
	return false;
}

void ioLocalKorea::OpenFillCashPage()
{
	if( g_MyInfo.IsEntryFormality() )
		g_App.OpenFillCashPage();
	else
		g_App.OpenCashEntryPage();
}

/*
const char * ioLocalKorea::GetGuildMasterPostion()
{
	return "�����"; //Except Extracting Hangeul
}

const char * ioLocalKorea::GetGuildSecondMasterPosition()
{
	return "�α����"; //Except Extracting Hangeul
}

const char * ioLocalKorea::GetGuildGeneralPosition()
{
	return "����"; //Except Extracting Hangeul
}

const char * ioLocalKorea::GetGuildAdminPosition()
{
	return "Admin"; //Except Extracting Hangeul
}
*/
const char * ioLocalKorea::GetGuildMasterPostion()
{
	return "Leader"; //Except Extracting Hangeul
}

const char * ioLocalKorea::GetGuildSecondMasterPosition()
{
	return "Officer"; //Except Extracting Hangeul
}

const char * ioLocalKorea::GetGuildGeneralPosition()
{
	return "Member"; //Except Extracting Hangeul
}

const char *ioLocalKorea::GetGuildAdminPosition()
{
	return "Admin"; //Except Extracting Hangeul
}

bool ioLocalKorea::IsRightNewID( const char *szID )
{
	return true;
}

bool ioLocalKorea::IsSendGashaponList()
{
	return false;
}

void ioLocalKorea::GetChangedString( OUT char *szCheckString )
{
	// �ƹ��ϵ� ���� ����
}

bool ioLocalKorea::IsExecuteLuncher()
{
	return true;
}

bool ioLocalKorea::IsKoreaGameLimit()
{
	return true;
}

bool ioLocalKorea::IsCheckKorean()
{
	return true;
}

bool ioLocalKorea::IsMyHomePage()
{
	return true;
}

bool ioLocalKorea::IsShowExtraGoodsList()
{
	return false;
}

bool ioLocalKorea::IsShowRegulation()
{
	return true;
}

DWORD ioLocalKorea::GetResourceIDErrorBack()
{
	return IDB_BACK_ERROR;
}

DWORD ioLocalKorea::GetResourceIDErrorExitC()
{
	return IDB_EXIT_C;
}

DWORD ioLocalKorea::GetResourceIDErrorExitN()
{
	return IDB_EXIT_N;
}

DWORD ioLocalKorea::GetResourceIDErrorExitO()
{
	return IDB_EXIT_O;
}

DWORD ioLocalKorea::GetResourceIDErrorSolutionC()
{
	return IDB_SOLUTION_C;
}

DWORD ioLocalKorea::GetResourceIDErrorSolutionN()
{
	return IDB_SOLUTION_N;
}

DWORD ioLocalKorea::GetResourceIDErrorSolutionO()
{
	return IDB_SOLUTION_O;
}

const char * ioLocalKorea::GetErrorSolutionURL()
{
	return "http://www.lostsaga.com/common/rgfaq.asp?errnumber=%d";
}

bool ioLocalKorea::SetPresentRecvDisableMsg( int iPresentType )
{
	return false;
}

int ioLocalKorea::GetLimitGradeLevel()
{
	return -1;
}

const char * ioLocalKorea::GetBillingErrorMent(ioHashString &sServerError)
{
	ioChannelingNodeParent *pChannelingNode = g_ChannelingMgr.GetNode( g_MyInfo.GetChannelingType() );
	if( pChannelingNode )
		return pChannelingNode->GetBillingErrorMent(sServerError);

	return "Billing Error : ";
}

int ioLocalKorea::GetLicenseDate()
{
	return 30111131;
}

DWORD ioLocalKorea::GetCashSendTime()
{
	ioChannelingNodeParent *pNode = g_ChannelingMgr.GetNode( g_MyInfo.GetChannelingType() );
	if( pNode )
		return pNode->GetCashSendTime();

	return ioLocalParent::GetCashSendTime();
}

int ioLocalKorea::GetCashPageWidth()
{
	ioChannelingNodeParent *pChannelingNode = g_ChannelingMgr.GetNode( g_MyInfo.GetChannelingType() );
	if( pChannelingNode )
	{
		return pChannelingNode->GetCashPageWidth();
	}

	return 0;
}

int ioLocalKorea::GetCashPageHeight()
{
	ioChannelingNodeParent *pChannelingNode = g_ChannelingMgr.GetNode( g_MyInfo.GetChannelingType() );
	if( pChannelingNode )
	{
		return pChannelingNode->GetCashPageHeight();
	}

	return 0;
}

void ioLocalKorea::OnCashPageHide()
{
	ioChannelingNodeParent *pChannelingNode = g_ChannelingMgr.GetNode( g_MyInfo.GetChannelingType() );
	if( pChannelingNode )
		pChannelingNode->OnCashPageHide();
}