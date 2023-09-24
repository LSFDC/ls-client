#include "stdafx.h"
#include "..\SP2Packet.h"
#include "../ioMyInfo.h"
#include "../NetworkWrappingFunc.h"
#include ".\ioChannelingNodeSteam.h"
#include <strsafe.h>


ioChannelingNodeSteam::ioChannelingNodeSteam(void)
{
}

ioChannelingNodeSteam::~ioChannelingNodeSteam(void)
{
}

ChannelingType ioChannelingNodeSteam::GetType()
{
	return CNT_STEAM;
}

const char * ioChannelingNodeSteam::GetININame()
{
	return "config/sp2_channeling_steam.ini";
}

const char * ioChannelingNodeSteam::GetLogoSetName()
{
	return "LogoDefault";
}

bool ioChannelingNodeSteam::OnRecieveGetCash( SP2Packet &rkReceivePacket )
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

	if( iCash != g_MyInfo.GetCash() )
	{
		ioLocalParent *pLocal = g_LocalMgr.GetLocal( ioLocalManager::GetLocalType() );
		if( pLocal )
		{
			pLocal->SetGoldChargeBtn( false );
			pLocal->SetCashReqTime( 0 );
		}
	}

	g_MyInfo.SetCash( iCash );
	g_MyInfo.SetPurchasedCash( iPurchasedCash );
	g_MyInfo.SetChannelingCash( 0 );
	return true;
}

void ioChannelingNodeSteam::GetFillCashURL( OUT ioHashString &rsFullURL , IN const ioHashString &rsShortURL )
{
	rsFullURL = rsShortURL;
}

void ioChannelingNodeSteam::GetLoginURL( OUT ioHashString &rsFullURL, IN const ioHashString &rsShortURL, IN const ioHashString &rsLoginKey )
{
	char szFullURL[MAX_PATH*2]="";
	StringCbPrintf( szFullURL, sizeof( szFullURL ), rsShortURL.c_str(), rsLoginKey.c_str() );
	rsFullURL = szFullURL;

#ifdef _DEBUG
	LOG.PrintTimeAndLog( 0, "%s | %s | %s |", __FUNCTION__ , szFullURL, rsShortURL.c_str(), rsLoginKey.c_str() );
#endif
}

int ioChannelingNodeSteam::GetCashPageWidth()
{
	return 550; 
}

int ioChannelingNodeSteam::GetCashPageHeight()
{
	return 580;
}

void ioChannelingNodeSteam::OnCashPageHide()
{
	SP2Packet kPacket( CTPK_GET_CASH );
	TCPNetwork::SendToServer( kPacket );
	TCPNetwork::MouseBusy( true );
}

bool ioChannelingNodeSteam::IsCreateProcessFillCashWeb()
{
	return false;
}

const char * ioChannelingNodeSteam::GetBillingErrorMent(ioHashString &sServerError)
{
	return "Steam Billing Error : ";
}

bool ioChannelingNodeSteam::OnRecieveAddCash( SP2Packet &rkReceivePacket )
{
	int iCash          = 0;
	int iPurchasedCash = 0;
	rkReceivePacket >> iCash;
	rkReceivePacket >> iPurchasedCash;
	g_MyInfo.SetCash( iCash );
	g_MyInfo.SetPurchasedCash( iPurchasedCash );
	return true;
}