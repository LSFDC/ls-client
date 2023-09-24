#pragma once

#include "ioLocalManager.h"

class SP2Packet;

class ioLocalParent
{
protected:
	ioHashString m_sErrorSolutionURL;
	ioHashString m_sGuildMaster;
	ioHashString m_sGuildSecondMaster;
	ioHashString m_sGuildGeneral;

	int m_nSubType;

	// 국가별 GMT 시간값 세팅		JCLEE 140421
	ioHashString m_sLocalGMT;

	// 케쉬 충전 버튼이 눌렸는지	JCLEE 140120
	bool		 m_bGoldChargeBtn;

	// 케쉬 요청 틱 값	JCLEE 150202
	DWORD        m_dwCashReqTime;

	// 인도네시아 웹 상점 url
	ioHashString m_sWeb_shop_url;

public:

	enum
	{ 
		SUB_TYPE_EN = 0,
		SUB_TYPE_DE,
		SUB_TYPE_FR,
		SUB_TYPE_IT,
		SUB_TYPE_PL,
		SUB_TYPE_TR,
	};

	enum
	{
		WEB_EVENT = 0,
		WEB_GUIDE
	};

	virtual ioLocalManager::LocalType GetType() = 0;
	virtual const char *GetTextListFileName() = 0;
	virtual const char *GetMemTextList();
	virtual int GetSubType() {return m_nSubType;}
	virtual void SetSubType(int nSubType) {m_nSubType = nSubType;}

	virtual DWORD GetResourceIDTitleCashBmp()  = 0;
	virtual DWORD GetResourceIDTitleEntryBmp() = 0;
	virtual DWORD GetResourceIDTitleGuildBmp() = 0;
	virtual DWORD GetResourceIDTitleGuildMarkBmp() = 0;
	virtual DWORD GetResourceIDTitleHomeBmp() = 0;
	virtual DWORD GetResourceIDTitleBmp() = 0;

	virtual DWORD GetResourceIDTitleWebShop() { return 0; };

	virtual DWORD GetResourceIDErrorBack() = 0;
	virtual DWORD GetResourceIDErrorExitC() = 0;
	virtual DWORD GetResourceIDErrorExitN() = 0;
	virtual DWORD GetResourceIDErrorExitO() = 0;
	virtual DWORD GetResourceIDErrorSolutionC() = 0;
	virtual DWORD GetResourceIDErrorSolutionN() = 0;
	virtual DWORD GetResourceIDErrorSolutionO() = 0;

	virtual void SetLoginData( const ioHashString &szBillingUserKey ) = 0;
	virtual void SetWarnetInfo(const ioHashString &szAuthInfo, const DWORD dwSessionID){};
	virtual void FillLoginData( OUT SP2Packet &rkPacket ) = 0;

	virtual void GetClassTypeForTutorialDropItem( int &riClassType ) = 0;

	virtual const char *GetGuildMasterPostion();
	virtual const char *GetGuildSecondMasterPosition();
	virtual const char *GetGuildGeneralPosition();
	virtual const char *GetGuildAdminPosition() = 0;
	virtual const char *GetErrorSolutionURL() = 0;
	virtual const char *GetBillingErrorMent(ioHashString &sServerError) = 0;

	virtual int  GetFirstIDMaxSize() = 0;
	virtual bool IsShowFirstIDWnd() = 0;

	virtual void OpenFillCashPage() = 0;
	virtual bool IsRightNewID( const char *szID ) = 0;

	// 국가별 별도로 코드상에 했던 부분을 sp2_Local.ini 에서 하도록 수정		JCLEE 140421
	virtual void SetTimeZone();
	virtual bool IsSendGashaponList() = 0;
	virtual void GetChangedString( OUT char *szCheckString ) = 0;

	virtual bool IsExecuteLuncher() = 0;
	virtual bool IsKoreaGameLimit() = 0;
	virtual bool IsCheckKorean() = 0;
	virtual bool IsFirstIDCheckPass() { return false; }
	virtual bool IsMyHomePage() = 0;
	virtual bool IsShowExtraGoodsList() = 0;
	virtual bool IsShowRegulation() = 0;
	virtual bool IsStartFullScreen(){ return false; }
	virtual bool IsTooltipAutoLine(){ return false; }
	virtual bool IsMileage() { return false; }
	virtual bool IsGatotkacaPremium( int iClassType ) { return false; }
	virtual bool IsPointBlankPremium( int iClassType ) { return false; }

	virtual bool SetPresentRecvDisableMsg( int iPresentType ) = 0;

    virtual int GetLimitGradeLevel() = 0; // -1이면 limit가 없음.

	virtual bool IsEndFishingByMove() { return true; }

	virtual bool IsRightLicense();
	virtual int GetLicenseDate() { return 0; }
	virtual bool IsShowFirstSoldierSelectWnd() { return false; }

	virtual void ExitAction(){}
	virtual void ShowHackShieldErrorDlg( const ioHashString &rsPrivateID ){}

	//virtual DWORD GetCashSendTime() { return 540000; } // ms
	virtual DWORD GetCashSendTime() { return 15000; } // ms
	virtual bool IsHeadquarterWebPage() { return false; }
	virtual bool IsFirstIDControlAltCtrl() { return true; }
	virtual bool IsPCRoomBonusSoldier() { return false; }

	virtual bool IsCheckValueServer() { return false; }
	virtual bool IsChangeServerHelp() { return false; }
	virtual bool IsSendCollisionPos() { return true; }
	virtual bool IsLimitStartCancel() { return false; }
	virtual bool IsCreateMiniDump() { return false; }
	virtual bool IsTickCheck() { return true; }
	virtual bool IsAnnounceShutDown() { return false; }
	virtual bool IsUseMedalLevelOpen() { return true; }
	virtual bool IsUsePresentMacroInShipping() { return true; }
	virtual bool IsSendChatLog() { return true; }
	virtual bool IsUseVoiceChat() { return true; }
	virtual bool IsMemoryLoadMapPac() { return false; }
	
	virtual int GetCashPageWidth() { return 0; }
	virtual int GetCashPageHeight() { return 0; }
	virtual void OnCashPageHide(){}
	virtual int GetProgramExitWaitSec() { return 60; } // 1분
	virtual void GetFillCashURL( OUT ioHashString &rsFullURL , IN const ioHashString &rsShortURL ){}

	virtual void LoadINI();

	virtual void SetWebLoginKey( const ioHashString &rsWebLoginKey ) {}

	virtual int GetHighSpecWidth() { return 1024; }
	virtual int GetHighSpecHeight() { return 768; }

	// 국가별 GMT 시간값		JCLEE 140421
	virtual const char *GetLocalGMT();

	// 케쉬 충전 버튼이 눌렸는지	JCLEE 140120
	virtual void SetGoldChargeBtn( bool bBtnOn ){ m_bGoldChargeBtn = bBtnOn; }
	virtual bool GetGoldChargeBtn(){ return m_bGoldChargeBtn; }

	// 케쉬 요청 루프	JCLEE 150202
	virtual void ProcessGetCash();

	// 케쉬 요청 틱	JCLEE 150202
	virtual void SetCashReqTime( DWORD dwTime ){ m_dwCashReqTime = dwTime; }
	virtual DWORD GetCashReqTime(){ return m_dwCashReqTime; }

	virtual ioHashString GetWebShopUrl(){ return m_sWeb_shop_url; }

	//이벤트 & 가이드 페이지
	virtual void OpenWebPage( int iType ) {};
public:
	ioLocalParent(void);
	virtual ~ioLocalParent(void);
};

