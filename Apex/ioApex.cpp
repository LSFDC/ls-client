#include "stdafx.h"
#include "ioApex.h"


#ifdef SRC_SA
#include "ApexClient.h"

#include "../ioApplication.h"
#include "../ioMyInfo.h"
#include "../Version.h"

ioApex *ioApex::sg_Instance = NULL;

_FUNC_C_REC pfSendToApexClient = NULL;

extern LONG __stdcall ExceptCallBack ( EXCEPTION_POINTERS * pExPtrs );

long NetSendToGameServer(const char *pBuffer, int nLen)
{
	ApexPacket rkApexPkt;
	rkApexPkt.nType = APEX_CMD_DATA;
	CopyMemory(rkApexPkt.byBuffer, pBuffer, nLen);
	rkApexPkt.nLength = nLen;

	SP2Packet kPacket( CTPK_PROTECT_CHECK );
	kPacket << rkApexPkt;
	TCPNetwork::SendToServer( kPacket );

	return 0;
}

ioApex::ioApex(void)
{
}

ioApex::~ioApex(void)
{
}

ioApex & ioApex::GetInstance()
{
	if( sg_Instance == NULL )
		sg_Instance = new ioApex;

	return (*sg_Instance);
}

void ioApex::ReleaseInstance()
{
	if( sg_Instance )
		delete sg_Instance;

	sg_Instance = NULL;
}


bool ioApex::Start()
{
	__try
	{
		LOG.PrintTimeAndLog( 0, "Apex start - 1 - start");

		long lResult = CHCStart(NetSendToGameServer, pfSendToApexClient);

		if ( lResult != 0 ) 
		{ 
			LOG.PrintTimeAndLog( 0, "Apex error - 1 - start %d", lResult);
			return false;
		} 
	}
	__except(ExceptCallBack( GetExceptionInformation() ) )
	{
		LOG.PrintTimeAndLog( 0, "Apex error - 1 - except");
		return false;
	}

	return true;
}


void ioApex::End()
{
	LOG.PrintTimeAndLog( 0, "Apex Check - 3 - start");
	__try
	{
		CHCEnd();
	}
	__except(ExceptCallBack( GetExceptionInformation() ) )
	{
		LOG.PrintTimeAndLog( 0, "Apex Check - 3 - crash : %d", GetLastError() );
	}
	LOG.PrintTimeAndLog( 0, "Apex Check - 3 - end");
}

void ioApex::OnRecieveCheck( ApexPacket &rkPacket )
{
	__try
	{
		if( rkPacket.nType == APEX_CMD_LOGIN )
		{
			Start();
		}
		else if( rkPacket.nType == APEX_CMD_DATA )
		{
			pfSendToApexClient((const char*)(&rkPacket.byBuffer[0]) , rkPacket.nLength );
		}
	}
	__except(ExceptCallBack( GetExceptionInformation() ) )
	{
		LOG.PrintTimeAndLog( 0, "Check - 4 - crash : %d", GetLastError() );
	}
}

void ioApex::Process()
{

}

#endif // SRC_SA