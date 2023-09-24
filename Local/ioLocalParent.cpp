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

	// GMT �ð��� ����		JCLEE 140421
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

// ������ GMT �ð���		JCLEE 140421
const char * ioLocalParent::GetLocalGMT()
{
	return m_sLocalGMT.c_str();
}

/*
- ������ GMT ���� ���� ����
������ ������ �ڵ�� �����ִ� �κ��� �����ϰ� ini ���� �����ϵ��� ����.
sp2_Local.ini ���Ͽ� local_gmt ���� �����Ѵ�. ���� GMT �� üũ�Ͽ� ���� GMT+3 �� ���
�̿� ���߱� ���Ͽ� ���ð��� -3 ���� �����ϵ��� �Ѵ�.

���� :  -2 (�ϱ� ���� �ð��� ���� �Ⱓ)
		-1 (�ϱ� ���� �ð��� ����� �Ⱓ)
��ƾ : 	+5
�߱� :	-9    
�δ� :	-7
�Ϻ� : 	-9
�ѱ� :	-9
�븸 :	-8
�±� : 	-7
�Ϲ� : 	+4
*/
// ������ GMT �ð��� ����	JCLEE 1404221
void ioLocalParent::SetTimeZone()
{
	char szBuf[128] = {0,};
	sprintf_s( szBuf, sizeof(szBuf), "TZ=GMT%s", GetLocalGMT() );

	LOG.PrintTimeAndLog( 0, "ioLocalParent::SetTimeZone() GMT Setting : %s", szBuf );

	putenv( szBuf );
	_tzset();
}


// �ɽ� ��û ����	JCLEE 150202
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