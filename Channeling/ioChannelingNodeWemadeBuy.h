#pragma once

#include "ioChannelingNodeParent.h"

class ioChannelingNodeWemadeBuy : public ioChannelingNodeParent
{
public:
	virtual ChannelingType GetType();
	virtual const char *GetININame();
	virtual const char *GetLogoSetName();
	virtual const char *GetBillingErrorMent(ioHashString &sServerError);

	virtual void GetFillCashURL( OUT ioHashString &rsFullURL , IN const ioHashString &rsShortURL );
	virtual void GetLoginURL( OUT ioHashString &rsFullURL, IN const ioHashString &rsShortURL, IN const ioHashString &rsLoginKey );

	virtual int GetCashPageWidth();
	virtual int GetCashPageHeight();

	virtual void OnCashPageHide();

	virtual bool IsCreateProcessFillCashWeb();
	virtual bool IsSendFillCashURL() { return false; }  // �δ϶����� 

public:
	virtual bool OnRecieveGetCash( SP2Packet &rkReceivePacket );
	virtual bool OnRecieveAddCash( SP2Packet &rkReceivePacket );

public:
	virtual bool IsSubscriptionRetractCheck() { return false; }
	virtual bool IsSubscriptionRetract() { return false; }

public:
	ioChannelingNodeWemadeBuy(void);
	virtual ~ioChannelingNodeWemadeBuy(void);
};

