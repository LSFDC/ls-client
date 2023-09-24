#include "stdafx.h"

#include "ioChannelingNodeNexon.h"
// #include "..\SP2Packet.h"
// #include "../ioMyInfo.h"
// #include "../NetworkWrappingFunc.h"
// #include ".\ioChannelingNodeNexon.h"
// #include <strsafe.h>


ioChannelingNodeNexon::ioChannelingNodeNexon(void)
{
}

ioChannelingNodeNexon::~ioChannelingNodeNexon(void)
{
}

ChannelingType ioChannelingNodeNexon::GetType()
{
	return CNT_NEXON;
}

const char * ioChannelingNodeNexon::GetININame()
{
	return "config/sp2_channeling_nexon.ini";
}

const char * ioChannelingNodeNexon::GetLogoSetName()
{
	return "LogoDefault";
}

bool ioChannelingNodeNexon::OnRecieveGetCash( SP2Packet &rkReceivePacket )
{
	int iCash          = 0;
	int iPurchasedCash = 0;
	rkReceivePacket >> iCash;
	rkReceivePacket >> iPurchasedCash;
	g_MyInfo.SetCash( iCash );
	g_MyInfo.SetPurchasedCash( iPurchasedCash );
	g_MyInfo.SetChannelingCash( 0 );

	//LOG.PrintTimeAndLog( 0 , "Nexon Get Cash Info   %d     %d", iCash, iPurchasedCash );

	return true;
}

void ioChannelingNodeNexon::GetFillCashURL( OUT ioHashString &rsFullURL , IN const ioHashString &rsShortURL )
{
	rsFullURL = rsShortURL;

#ifdef NEXONLOG
	LOG.PrintTimeAndLog( 0, "%s | %s |", __FUNCTION__ , rsFullURL.c_str(), rsShortURL.c_str() );
#endif
}

void ioChannelingNodeNexon::GetLoginURL( OUT ioHashString &rsFullURL, IN const ioHashString &rsShortURL, IN const ioHashString &rsLoginKey )
{
	char szFullURL[MAX_PATH*2]="";
	StringCbPrintf( szFullURL, sizeof( szFullURL ), rsShortURL.c_str(), rsLoginKey.c_str() );
	rsFullURL = szFullURL;

#ifdef NEXONLOG
	LOG.PrintTimeAndLog( 0, "%s | %s | %s |", __FUNCTION__ , szFullURL, rsShortURL.c_str(), rsLoginKey.c_str() );
#endif
}

int ioChannelingNodeNexon::GetCashPageWidth()
{
	return 550; 
}

int ioChannelingNodeNexon::GetCashPageHeight()
{
	return 580;
}

void ioChannelingNodeNexon::OnCashPageHide()
{
	SP2Packet kPacket( CTPK_GET_CASH );
	TCPNetwork::SendToServer( kPacket );
	TCPNetwork::MouseBusy( true );
}

bool ioChannelingNodeNexon::IsCreateProcessFillCashWeb()
{
	return false;
}

const char * ioChannelingNodeNexon::GetBillingErrorMent(ioHashString &sServerError)
{
	if( sServerError == "{\"resultCode\":-1,\"resultValue\":0}" )
		return STR(1);
	else if( sServerError == "{\"resultCode\":-2,\"resultValue\":0}" )
		return STR(2);
	else if( sServerError == "{\"resultCode\":-3,\"resultValue\":0}" )
		return STR(3);
	else if( sServerError == "{\"resultCode\":-4,\"resultValue\":0}" )
		return STR(4);
	else if( sServerError == "{\"resultCode\":-999,\"resultValue\":0}" )
		return "System Excetion";
	else if( sServerError == "{\"resultCode\":-110,\"resultValue\":0}" )
		return STR(5);
	else if( sServerError == "{\"resultCode\":-108,\"resultValue\":0}" )
		return STR(6);
	else if( sServerError == "{\"resultCode\":-106,\"resultValue\":0}" )
		return STR(7);
	else if( sServerError == "{\"resultCode\":-1002,\"resultValue\":0}" )
		return STR(8);
	else if( sServerError == "{\"resultCode\":-1202,\"resultValue\":0}" )
		return STR(9);
	else if( sServerError == "{\"resultCode\":-1100,\"resultValue\":0}" )
		return STR(10);
	else if( sServerError == "{\"resultCode\":-1104,\"resultValue\":0}" )
		return STR(11);	
	else if( sServerError == "{\"resultCode\":-1000,\"resultValue\":0}" )
		return STR(12);
	else if( sServerError == "{\"resultCode\":-9900,\"resultValue\":0}" )
		return STR(13);
	else if( sServerError == "{\"resultCode\":-9999,\"resultValue\":0}" )
		return STR(14);

	return "Billing Error : ";
}

bool ioChannelingNodeNexon::OnRecieveAddCash( SP2Packet &rkReceivePacket )
{
	int iCash          = 0;
	int iPurchasedCash = 0;
	rkReceivePacket >> iCash;
	rkReceivePacket >> iPurchasedCash;

#if defined( USE_GA )
	if( g_App.GetGAFirstCash() == false )
	{
		// GOLD_CHARGE
		int iChargeCash = iPurchasedCash - g_MyInfo.GetPurchasedCash();
		if( iChargeCash > 0 )
		{
			/*
			g_HttpMng.GA_ItemHitTracking( g_MyInfo.GetUserIndex() 
				, "Charge"
				, iChargeCash
				, 1
				, 0
				, "Gold"
				, "%2FGOLD%2FCHARGE"
				, true );
			*/

			// Gold charge page view ·Î ÀüÈ¯
			g_HttpMng.GA_PageVIewTracking( g_MyInfo.GetUserIndex(), "%2FGOLD%2FCHARGE", 4, "", iChargeCash );
		}
	}
	else
	{
		g_App.SetGAFirstCash( false );
	}
#endif

	g_MyInfo.SetCash( iCash );
	g_MyInfo.SetPurchasedCash( iPurchasedCash );
	return true;
}