#pragma once


// 확률 상승형 가챠 윈도우

#include "../ioGashaponMgr.h"

///////////////////////////////////////////////////////////////////////////


class RisingGashaponItemInfoBtn : public ioButton
{
public:
	enum AniType
	{
		AT_NONE       = 0,
		AT_BACK_WHITE = 1,
		AT_BACK_WAIT  = 2,
		AT_OUT        = 3, // 감속
		AT_IN         = 4, // 가속
		AT_WHITE      = 5,
		AT_END        = 6,
		AT_BACK_OUT   = 7,
		AT_BACK_IN    = 8,
		AT_BACK_END   = 9,
	};

	enum
	{
		MAX_SPLIT_LINE = 2,
	};

protected:
	ioUIRenderFrame *m_pSelect;
	ioUIRenderFrame *m_pEmpty;

protected:
	ioUIRenderImage *m_pPresentIcon;

	ioUIRenderImage *m_pNum;
	ioUIRenderImage *m_pPlus;
	ioUIRenderImage *m_pGain;

protected:
	GashaponPresent m_GashaponPresent;

protected:

	bool  m_bGetCard;
	bool  m_bSelect;

	ioUIRenderImage *m_pEffect;

	ioHashStringVec m_vEndEfffect_S;
	int m_iCurEffectArray;
	DWORD m_dwRollingEffectTime;
	DWORD m_dwRollingEffectCheckTime;

	ioHashString m_szSuccessSound;

	ioHashString m_szTitles[MAX_SPLIT_LINE];

protected:
	void RollingEffect( float fTimePerSec );
	void RenderInfo(int iXPos, int iYPos, UIRenderType eType );
	void RenderEffect( int iXPos, int iYPos, UIRenderType eType );
	void RenderSelect( int iXPos, int iYPos, UIRenderType eType );
	void RenderEmpty( int iXPos, int iYPos, UIRenderType eType );


public:
	virtual void iwm_create();
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual void ParseExtraInfo( ioXMLElement &xElement );

protected:
	virtual void AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame );
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

protected:
	virtual void OnDrawNormal( int iXPos, int iYPos );
	virtual void OnDrawOvered( int iXPos, int iYPos );
	virtual void OnDrawPushed( int iXPos, int iYPos );
	virtual void OnDrawDisabled( int iXPos, int iYPos );


public:
	void SetPresent( const GashaponPresent& rkPresent, ioHashString strName );

public:
	bool IsEmptyCard(){ return m_GashaponPresent.IsEmpty(); }
	void SetEmptyCard();
	const GashaponPresent& GetPresent(){ return m_GashaponPresent; }

	void SetGetCard( bool bGetCard );
	bool IsGetCard() { return m_bGetCard; }

	void SetSelect( bool bSelect );
	bool IsSelect() { return m_bSelect; }
	void SetGetEffect();

public:
	RisingGashaponItemInfoBtn();
	virtual ~RisingGashaponItemInfoBtn();
};

///////////////////////////////////////////////////////////////////////////
// 메인 윈도우
class RisingGashaponWnd : public ioWnd
{
public:
	enum
	{
		ID_EXIT   = 1,
		ID_BUY    = 2,
		ID_WAITING= 3,
		ID_GOLD_CHARGE = 4,

		ID_ITEM_1  = 1001,
		ID_ITEM_2  = 1002,
		ID_ITEM_3  = 1003,
		ID_ITEM_4  = 1004,
		ID_ITEM_5  = 1005,
		ID_ITEM_6  = 1006,
		ID_ITEM_7  = 1007,
		ID_ITEM_8  = 1008,
		ID_ITEM_9  = 1009,
		ID_ITEM_10 = 1010,
	};

	enum 
	{
		MAX_ITEM = 10,
	};

	enum AniType
	{
		AT_NONE  = 0,
		AT_WAIT  = 1,
		AT_SOUND = 2,
		AT_ROLL  = 3, 
		AT_CHANGE= 4,
		AT_END   = 5,
	};

	enum MsgBoxType
	{
		eMSGBOX_TYPE_NONE = 0,
		eMSGBOX_TYPE_INITAGAIN,
		eMSGBOX_TYPE_EXIT,
	};

	enum ErrorNumType
	{
		eERROR_NUM_TYPE_PRESENT_CANT_FIND = 0,

	};

protected:	
	ioHashString m_sDescCard;
	
	ioUIRenderImage *m_pLineLeft;
	ioUIRenderImage *m_pLineCenter;
	ioUIRenderImage *m_pLineRight;


	ioHashString m_szRollingSound;

	FSMStateMgr m_AniState;

	DWORD m_dwAniWaitDelay;
	DWORD m_dwAniRollDelay;
	DWORD m_dwAniRollCnt;
	DWORD m_dwCurAniRollCnt;

	int   m_iMaxSelectItem;
	int   m_iEtcItemType;


	ioHashString m_szMent1;
	ioHashString m_szMent2;


	int m_bBuyCash;
	int m_iBuyCount;
	MsgBoxType m_eMsgType;

	std::vector<int> m_vGettedIndexList;
	std::vector<int> m_vUnGettedIndexList;



protected:
	vGashaponPresent m_vGashaponItemList;

public:
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	

protected:
	void Init();
	void InitAniState();
	void ProcessAni();

	void CheckBuy();
	void SetTitleBuyBtn( int iCashArray );

	void OnBuy();
	void OnCashCharge();

	void ShowNeedPeso();
	void ShowExitMsg();
	void ShowInitWarning();
	void ShowError(ErrorNumType eErrorType);

	void SendInit();

public:
	void SendBuy();

	void SetResult( const GashaponPresent& rkResultPresent );

public:
	virtual void iwm_hide();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );
	virtual bool iwm_esc();

	virtual void ParseExtraInfo( ioXMLElement &xElement );

	
public:
	void ShowRisingGashaponWnd( int iEtcItemType, DWORD dwID);


protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();
	virtual void OnRenderAfterChild();

public:
	RisingGashaponWnd(void);
	virtual ~RisingGashaponWnd(void);
};

