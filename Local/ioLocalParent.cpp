#include "stdafx.h"
#include ".\iolocalparent.h"

ioLocalParent::ioLocalParent(void)
{
	m_sGuildMaster.Clear();
	m_sGuildSecondMaster.Clear();
	m_sGuildGeneral.Clear();
	m_nSubType = 0;
	m_bGoldChargeBtn = false;
	m_dwCashReqTime = 0;
}

ioLocalParent::~ioLocalParent(void)
{
}

bool ioLocalParent::IsRightLicense()
{
	SYSTEMTIME st;
	GetLocalTime( &st );
	int iDate = (st.wYear * 10000) + (st.wMonth * FLOAT100) + st.wDay;

	if( iDate >= GetLicenseDate() )
		return false;

	return true;
}

void ioLocalParent::LoadINI()
{
	char	szBuf[MAX_PATH]		= {0,};

	ioINILoader kLoaderLocal( "config/sp2_local.ini" );
	kLoaderLocal.SetTitle( "text" );

	ZeroMemory( szBuf, sizeof( szBuf ) );
	kLoaderLocal.LoadString( "guild_master", "", szBuf, MAX_PATH );
	m_sGuildMaster = szBuf;

	ZeroMemory( szBuf, sizeof( szBuf ) );
	kLoaderLocal.LoadString( "guild_second_master", "", szBuf, MAX_PATH );
	m_sGuildSecondMaster = szBuf;

	ZeroMemory( szBuf, sizeof( szBuf ) );
	kLoaderLocal.LoadString( "guild_general", "", szBuf, MAX_PATH );
	m_sGuildGeneral = szBuf;

	// GMT 시간값 세팅		JCLEE 140421
	ZeroMemory( szBuf, sizeof( szBuf ) );
	kLoaderLocal.LoadString_e( "local_gmt", "", szBuf, MAX_PATH );
	m_sLocalGMT = szBuf;

	ZeroMemory( szBuf, sizeof( szBuf ) );
	kLoaderLocal.LoadString_e( "local_web_url", "", szBuf, MAX_PATH );
	m_sWeb_shop_url = szBuf;
}
const char * ioLocalParent::GetMemTextList()
{
	return "No_Use_GetMemTextList";
}
const char * ioLocalParent::GetGuildMasterPostion()
{
	return m_sGuildMaster.c_str();
}

const char * ioLocalParent::GetGuildSecondMasterPosition()
{
	return m_sGuildSecondMaster.c_str();
}

const char * ioLocalParent::GetGuildGeneralPosition()
{
	return m_sGuildGeneral.c_str();
}

// 국가별 GMT 시간값		JCLEE 140421
const char * ioLocalParent::GetLocalGMT()
{
	return m_sLocalGMT.c_str();
}

/*
- 국가별 GMT 세팅 참고 사항
기존의 국가별 코드상에 박혀있던 부분은 삭제하고 ini 에서 세팅하도록 수정.
sp2_Local.ini 파일에 local_gmt 값을 수정한다. 현지 GMT 를 체크하여 만약 GMT+3 인 경우
이에 맞추기 위하여 세팅값을 -3 으로 수정하도록 한다.

유럽 :  -2 (일광 절약 시간제 시행 기간)
		-1 (일광 절약 시간제 비시행 기간)
라틴 : 	+5
중국 :	-9    
인니 :	-7
일본 : 	-9
한국 :	-9
대만 :	-8
태국 : 	-7
북미 : 	+4
*/
// 국가별 GMT 시간값 세팅	JCLEE 1404221
void ioLocalParent::SetTimeZone()
{
	char szBuf[128] = {0,};
	sprintf_s( szBuf, sizeof(szBuf), "TZ=GMT%s", GetLocalGMT() );

	LOG.PrintTimeAndLog( 0, "ioLocalParent::SetTimeZone() GMT Setting : %s", szBuf );

	putenv( szBuf );
	_tzset();
}


// 케쉬 요청 루프	JCLEE 150202
void ioLocalParent::ProcessGetCash()
{
	if( m_bGoldChargeBtn == false )
		return;

	if( m_dwCashReqTime == 0 )
	{
		m_dwCashReqTime = REALGETTIME();
		return;
	}

	if( REALGETTIME() - m_dwCashReqTime < 30000 )
		return;

	SP2Packet kPacket( CTPK_GET_CASH );
	TCPNetwork::SendToServer( kPacket );
	TCPNetwork::MouseBusy( true );

	m_dwCashReqTime = REALGETTIME();
}