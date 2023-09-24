#pragma once

#include "ioLocalParent.h"

class ioLocalSingapore  : public ioLocalParent
{
protected:
	ioHashString	m_sLoginEncodePW;
	ioHashString	m_sAuthPC;
	DWORD			m_dwSessionID;

	void GetMacAddress( OUT char *szMacAddress, IN int iSize );

public:
	enum
	{ 
		SUB_TYPE_EN = 0,
		SUB_TYPE_MY,
	};

public:
	virtual ioLocalManager::LocalType GetType();
	virtual const char *GetTextListFileName();
	virtual const char *GetMemTextList();

	virtual DWORD GetResourceIDTitleCashBmp();
	virtual DWORD GetResourceIDTitleEntryBmp();
	virtual DWORD GetResourceIDTitleGuildBmp();
	virtual DWORD GetResourceIDTitleGuildMarkBmp();
	virtual DWORD GetResourceIDTitleHomeBmp();
	virtual DWORD GetResourceIDTitleBmp();

	virtual DWORD GetResourceIDErrorBack();
	virtual DWORD GetResourceIDErrorExitC();
	virtual DWORD GetResourceIDErrorExitN();
	virtual DWORD GetResourceIDErrorExitO();
	virtual DWORD GetResourceIDErrorSolutionC();
	virtual DWORD GetResourceIDErrorSolutionN();
	virtual DWORD GetResourceIDErrorSolutionO();

	virtual void SetLoginData( const ioHashString &szBillingUserKey );
	virtual void SetWarnetInfo(const ioHashString &szAuthInfo, const DWORD dwSessionID);
	virtual void FillLoginData( OUT SP2Packet &rkPacket );

	virtual void GetClassTypeForTutorialDropItem( int &riClassType );

	virtual const char *GetGuildMasterPostion();
	virtual const char *GetGuildSecondMasterPosition();
	virtual const char *GetGuildGeneralPosition();
	virtual const char *GetGuildAdminPosition();
	virtual const char *GetErrorSolutionURL();
	virtual const char *GetBillingErrorMent(ioHashString &sServerError);

	virtual int  GetFirstIDMaxSize();
	virtual bool IsShowFirstIDWnd();

	virtual void OpenFillCashPage();

	virtual bool IsRightNewID( const char *szID );

	virtual bool IsSendGashaponList();

	virtual void GetChangedString( OUT char *szCheckString );

	virtual bool IsExecuteLuncher();
	virtual bool IsKoreaGameLimit();
	virtual bool IsCheckKorean();
	virtual bool IsMyHomePage();
	virtual bool IsShowExtraGoodsList();	
	virtual bool IsShowRegulation();
	virtual bool IsStartFullScreen(){ return true; }
	virtual bool IsTooltipAutoLine(){ return true; }

	virtual bool SetPresentRecvDisableMsg( int iPresentType );

	virtual int GetLimitGradeLevel();

	virtual int GetLicenseDate();

	virtual DWORD GetCashSendTime() { return 3600000; } // ms
	virtual bool IsCheckValueServer() { return true; }
	virtual bool IsChangeServerHelp() { return true; }
	virtual bool IsSendCollisionPos() { return false; }
	virtual bool IsLimitStartCancel() { return true; }
	virtual bool IsCreateMiniDump() { return false; }
	virtual bool IsTickCheck() { return true; }

	virtual int GetCashPageWidth();
	virtual int GetCashPageHeight();
	virtual void OnCashPageHide();

	virtual int GetHighSpecWidth() { return 800; }
	virtual int GetHighSpecHeight() { return 600; }

	virtual void ShowHackShieldErrorDlg( const ioHashString &rsPrivateID );

	virtual bool IsGatotkacaPremium( int iClassType );
	virtual bool IsPointBlankPremium( int iClassType );

	virtual bool IsPCRoomBonusSoldier() { return true; }

public:
	ioLocalSingapore(void);
	virtual ~ioLocalSingapore(void);
};

