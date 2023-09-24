#include "stdafx.h"
#include ".\ioLocalSingapore.h"
#include "../resource.h"
#include "../SP2Packet.h"
#include "../Setting.h"
#include "../ioApplication.h"
#include "../EtcHelpFunc.h"
#include "../ioBrowserManager.h"
#include "../ioMyInfo.h"
#include "../NetworkWrappingFunc.h"
#include <iphlpapi.h>

ioLocalSingapore::ioLocalSingapore(void)
{
	m_sAuthPC.Clear();
	m_dwSessionID = 0;
	m_nSubType = SUB_TYPE_EN;
}

ioLocalSingapore::~ioLocalSingapore(void)
{
}

ioLocalManager::LocalType ioLocalSingapore::GetType()
{
	return ioLocalManager::LCT_SINGAPORE;
}

const char * ioLocalSingapore::GetTextListFileName()
{
	switch(m_nSubType)
	{
	case SUB_TYPE_MY:
		return "text_my.txt";
	default:
		return "text_sg.txt";
	}

}

const char * ioLocalSingapore::GetMemTextList()
{

	DWORD size = 0;
	const char * pChar = NULL;
	Help::LoadTextInResource(IDR_ERROR, size, pChar);
	return pChar;

	//return "|EXE_WinMain_1|Terjadi Error di Security Program.|" //Except Extracting Hangeul
	//	   "|EXE_WinMain_2|Lost Saga sudah berjalan.|" //Except Extracting Hangeul
	//	   "|EXE_WinMain_3|    Tidak dapat menemukan DirectX\r\nSilahkan install DirectX yang terbaru.\r\n(Error Code:301)|" //Except Extracting Hangeul
	//	   "|EXE_WinMain_4|Versi DirectX yang install terlalu rendah(%1)\nSilahkan install  DirectX yang terbaru\nDan silahkan cek juga versi Driver Graphic Card.\n(Error Code:302)|" //Except Extracting Hangeul
	//	   "|EXE_WinMain_5|Game Guard Error : %1|"  //Except Extracting Hangeul
	//	   "|EXE_ErrorDlgProc_1|--- Spesifikasi System ---\r\n%1\r\n\r\n--- Info Error ---\r\n%2|" //Except Extracting Hangeul
	//	   "|EXE_ioNProtect::GetStartErrorMsg_1|Game Guard sedang berjalan. Silahkan coba beberapa saat lagi atau setelah reboot.|" //Except Extracting Hangeul
	//	   "|EXE_ioNProtect::GetStartErrorMsg_2|Game sudah berjalan atau Game Guard sedang berjalan. Silahkan coba lagi seteleh keluar dari game.|" //Except Extracting Hangeul
	//	   "|EXE_ioNProtect::GetStartErrorMsg_3|Terjadi error saat mereset Game guard. SIlahkan coba lagi seteleh reboot atau seteleh menutup program-program yang kemungkinan berpengaruh ke game.|" //Except Extracting Hangeul
	//	   "|EXE_ioNProtect::GetStartErrorMsg_4|Tidak ada file Game Guard atau telah dimodifikasi.Silahkan setup Game Guard file.|" //Except Extracting Hangeul
	//	   "|EXE_ioNProtect::GetStartErrorMsg_5|Ada beberapa system file Windows yang rusak. Silahkan install ulang Internet Explorer.|" //Except Extracting Hangeul
	//	   "|EXE_ioNProtect::GetStartErrorMsg_6|Gagal menjalankan Game Guard. Silahkan install ulang Game Guard Setup File.|" //Except Extracting Hangeul
	//	   "|EXE_ioNProtect::GetStartErrorMsg_7|Terdeteksi program illegal. Silahkan coba lagi setelah menutup program illegal.|" //Except Extracting Hangeul
	//	   "|EXE_ioNProtect::GetStartErrorMsg_8|Telah membatalkan update Game Guard. Jika tidak bisa connect silakan cek Jaringan Internet atau setting Firewall.|" //Except Extracting Hangeul
	//	   "|EXE_ioNProtect::GetStartErrorMsg_9|Gagal connect ke Game Guard update server. Silahkan coba beberapa saat lagi atau silahkan cek kondisi jaringan.|" //Except Extracting Hangeul
	//	   "|EXE_ioNProtect::GetStartErrorMsg_10|Error reset Game Guard atau versi file Game Guard yang lama. Silahkan jalankan kembali setelah setup ulang Game Guard.|" //Except Extracting Hangeul
	//	   "|EXE_ioNProtect::GetStartErrorMsg_11|Tidak ada file ini atau telah dimodifikasi. Silahkan coba install Game Guard setup file.|" //Except Extracting Hangeul
	//	   "|EXE_ioNProtect::GetStartErrorMsg_12|Error mereset file npgmup.des.Silahkan jalankan game kembali setelah menghapus folder Game Guard.|" //Except Extracting Hangeul
	//	   "|EXE_ioNProtect::GetStartErrorMsg_13|Gagal connect ke Game Guard Update Server. Silahkan coba beberapa saat lagi atau silahkan cek setting Firewall.|" //Except Extracting Hangeul
	//	   "|EXE_ioNProtect::GetStartErrorMsg_14|Tidak dapat menyelesaikan Game Guard Update. SIlahkan coba lagi setelah menon-aktifkan anti-virus untuk sementara waktu.|" //Except Extracting Hangeul
	//	   "|EXE_ioNProtect::GetStartErrorMsg_15|Gagal loading Module untuk mengecek virus atau hacking tool. Kekurangan Memory atau telah terkena virus.|" //Except Extracting Hangeul
	//	   "|EXE_ioNProtect::GetStartErrorMsg_16|Ada program yang mempengaruhi jalannya Game Guard.|" //Except Extracting Hangeul
	//	   "|EXE_ioNProtect::GetStartErrorMsg_17|Telah terjadi error saat menjalankan Game Guard. Silahkan mengirim email ke Game2@inca.co.kr sertakan file *erl yang ada di dalam folder Game Guard di Folder Game.|"; //Except Extracting Hangeul
}

DWORD ioLocalSingapore::GetResourceIDTitleCashBmp()
{
	return IDB_WEB_TITLE_CASH;
}

DWORD ioLocalSingapore::GetResourceIDTitleEntryBmp()
{
	return IDB_WEB_TITLE_ENTRY;
}

DWORD ioLocalSingapore::GetResourceIDTitleGuildBmp()
{
	return IDB_WEB_TITLE_GUILD;
}

DWORD ioLocalSingapore::GetResourceIDTitleGuildMarkBmp()
{
	return IDB_WEB_TITLE_GUILD_MARK;
}

DWORD ioLocalSingapore::GetResourceIDTitleHomeBmp()
{
	return IDB_WEB_TITLE_HOME;
}

DWORD ioLocalSingapore::GetResourceIDTitleBmp()
{
	return IDB_WINDOW_TITLE_LOSA;
}

void ioLocalSingapore::SetLoginData( const ioHashString &szBillingUserKey )
{
	m_sLoginEncodePW = szBillingUserKey;
}

void ioLocalSingapore::SetWarnetInfo(const ioHashString &szAuthInfo, const DWORD dwSessionID)
{
	m_sAuthPC = szAuthInfo;
	m_dwSessionID = dwSessionID;
}

void ioLocalSingapore::FillLoginData( OUT SP2Packet &rkPacket )
{
	char szMacAddress[MAX_PATH]="";
	char szKey[MAX_PATH]=""; 

	GetMacAddress( szMacAddress, sizeof( szMacAddress ) );
	Help::RegReadString( REG_SHCU, "SOFTWARE\\Gemscool\\Premium","Key","",szKey,sizeof(szKey));

	rkPacket << m_sLoginEncodePW;

	rkPacket << szMacAddress;
	rkPacket << szKey;
	rkPacket << m_sAuthPC << m_dwSessionID;

	// 더이상 필요가 없으므로 삭제
	m_sLoginEncodePW.Clear();
}

void ioLocalSingapore::GetClassTypeForTutorialDropItem( int &riClassType )
{

}

int ioLocalSingapore::GetFirstIDMaxSize()
{
	return 12;
}

bool ioLocalSingapore::IsShowFirstIDWnd()
{
	return true;
}

void ioLocalSingapore::OpenFillCashPage()
{
	if( g_MyInfo.IsEntryFormality() )
		g_App.OpenFillCashPage();
	else
		g_App.OpenCashEntryPage();
}

const char * ioLocalSingapore::GetGuildMasterPostion()
{
	return "Leader"; //Except Extracting Hangeul
}

const char * ioLocalSingapore::GetGuildSecondMasterPosition()
{
	return "Officer"; //Except Extracting Hangeul
}

const char * ioLocalSingapore::GetGuildGeneralPosition()
{
	return "Member"; //Except Extracting Hangeul
}

const char * ioLocalSingapore::GetGuildAdminPosition()
{
	return "Admin"; //Except Extracting Hangeul
}

bool ioLocalSingapore::IsRightNewID( const char *szID )
{
	int size = strlen(szID);
	for (int i=0; i<size; i++)
	{
		if ((!COMPARE(szID[i], 'A', 'Z'+1)) &&
			(!COMPARE(szID[i], 'a', 'z'+1)) &&
			(!COMPARE(szID[i], '0', '9'+1)) )
		{
			return false;
		}
	}
	return true;
}

bool ioLocalSingapore::IsSendGashaponList()
{
	return true;
}

void ioLocalSingapore::GetChangedString( OUT char *szCheckString )
{
		strlwr( szCheckString );
}

bool ioLocalSingapore::IsExecuteLuncher()
{
	return false;
}

bool ioLocalSingapore::IsKoreaGameLimit()
{
	return false;
}

bool ioLocalSingapore::IsCheckKorean()
{
	return false;
}

bool ioLocalSingapore::IsMyHomePage()
{
	return false;
}

bool ioLocalSingapore::IsShowExtraGoodsList()
{
	return false;
}

bool ioLocalSingapore::IsShowRegulation()
{
	return false;
}

DWORD ioLocalSingapore::GetResourceIDErrorBack()
{
	return IDB_BACK_ERROR_ID;
}

DWORD ioLocalSingapore::GetResourceIDErrorExitC()
{
	return IDB_EXIT_C_ID;
}

DWORD ioLocalSingapore::GetResourceIDErrorExitN()
{
	return IDB_EXIT_N_ID;
}

DWORD ioLocalSingapore::GetResourceIDErrorExitO()
{
	return IDB_EXIT_O_ID;
}

DWORD ioLocalSingapore::GetResourceIDErrorSolutionC()
{
	return IDB_SOLUTION_C_ID;
}

DWORD ioLocalSingapore::GetResourceIDErrorSolutionN()
{
	return IDB_SOLUTION_N_ID;
}

DWORD ioLocalSingapore::GetResourceIDErrorSolutionO()
{
	return IDB_SOLUTION_O_ID;
}

const char * ioLocalSingapore::GetErrorSolutionURL()
{
	return "http://forum.gemscool.com/forum-143.html";
}

bool ioLocalSingapore::SetPresentRecvDisableMsg( int iPresentType )
{
	return false;
}

int ioLocalSingapore::GetLimitGradeLevel()
{
	return -1;
}

const char * ioLocalSingapore::GetBillingErrorMent(ioHashString &sServerError)
{
	return "Billing Error : ";
}

int ioLocalSingapore::GetLicenseDate()
{
	return 20161206;
}

int ioLocalSingapore::GetCashPageWidth()
{
	return 356;
}

int ioLocalSingapore::GetCashPageHeight()
{
	return 459;
}

void ioLocalSingapore::OnCashPageHide()
{
	SP2Packet kPacket( CTPK_GET_CASH );
	TCPNetwork::SendToServer( kPacket );
	TCPNetwork::MouseBusy( true );
}

bool ioLocalSingapore::IsGatotkacaPremium( int iClassType )
{
	if( iClassType == 61 )
		return true;

	return false;
}

bool ioLocalSingapore::IsPointBlankPremium( int iClassType )
{
	if( iClassType == 110 )
		return true;

	return false;
}


void ioLocalSingapore::ShowHackShieldErrorDlg( const ioHashString &rsPrivateID )
{
#ifdef HACKSHIELD
	g_ioHackShield.ShowErrorDlg( rsPrivateID );
#endif
}




#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

void ioLocalSingapore::GetMacAddress( OUT char *szMacAddress, IN int iSize )
{
	PIP_ADAPTER_INFO pAdapterInfo=NULL;
	PIP_ADAPTER_INFO pAdapter = NULL;

	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	pAdapterInfo = (IP_ADAPTER_INFO *) HeapAlloc(GetProcessHeap(), 0, (sizeof(IP_ADAPTER_INFO)));
	if (pAdapterInfo == NULL) 
	{
		StringCbCopy( szMacAddress, iSize,  "00-00-00-00-00-00" );
		return;
	}

	if ( GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW )  
	{
		HeapFree(GetProcessHeap(), 0, (pAdapterInfo));
		pAdapterInfo = (IP_ADAPTER_INFO *) HeapAlloc(GetProcessHeap(), 0, (ulOutBufLen));
		if (pAdapterInfo == NULL) 
		{
			StringCbCopy( szMacAddress, iSize,  "00-00-00-00-00-00" );
			return;
		}
	}

	if( GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) != NO_ERROR )  // 첫번째 랜카드 MAC address 가져오기
	{
		HeapFree(GetProcessHeap(), 0, (pAdapterInfo));
		StringCbCopy( szMacAddress, iSize,  "00-00-00-00-00-00" );
		return;
	}

	char szPassDes[MAX_PATH]="Microsoft Virtual WiFi Miniport Adapter";
	pAdapter = pAdapterInfo;
	enum { MAX_LOOP = 100 };
	for (int i = 0; i < MAX_LOOP ; i++)
	{
		if( !pAdapter )
		{
			StringCbCopy( szMacAddress, iSize,  "00-00-00-00-00-00" );
			break;
		}
		if( pAdapter->Type != MIB_IF_TYPE_ETHERNET )
		{
			pAdapter = pAdapter->Next;
			continue;
		}
		if( strncmp( pAdapter->Description, szPassDes, strlen( szPassDes ) )  == 0 ) // 크레온 요청 예외
		{
			pAdapter = pAdapter->Next;
			continue;
		}
		if( strcmp( pAdapter->IpAddressList.IpAddress.String, "0.0.0.0" ) == 0 )	 // 비활성 예외
		{
			pAdapter = pAdapter->Next;
			continue;
		}

		StringCbPrintf( szMacAddress, iSize,  "%0.2X-%0.2X-%0.2X-%0.2X-%0.2X-%0.2X", pAdapter->Address[0], pAdapter->Address[1], pAdapter->Address[2], pAdapter->Address[3], pAdapter->Address[4], pAdapter->Address[5] );
		break;
	}

	if (pAdapterInfo)
		HeapFree(GetProcessHeap(), 0, (pAdapterInfo));
}


