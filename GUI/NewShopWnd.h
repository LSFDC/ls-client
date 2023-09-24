#pragma once

#include "../ioComplexStringPrinter.h"
#include "../io3DEngine/ioMovingWnd.h"

#include "NewShopDataDefine.h"
#include "NewShopSortDefine.h"

#include "ShopEventItemCounter.h"

class ioShopDataCache;
class ioPlayStage;

class NewShopWnd : public ioMovingWnd
{
public:
	friend class NewShopBuyWnd;
	friend class NewShopSearchWnd;

public:

	// ��ư���� ���������� ����
	enum
	{
		ID_CHARGE_GOLD_BTN			= 1,
		ID_MY_ITEM_BTN				= 2,
		ID_EXIT_BTN					= 3,
#ifdef STEAM_ATTACH
		ID_REDEEM_BTN				= 4,
		ID_HOME_BTN					= 5,
		ID_HOME_TOOLTIP				= 6,
#else
		ID_HOME_BTN					= 4,
		ID_HOME_TOOLTIP				= 5,
#endif
		ID_RECOMMEND_TAB_BTN		= 101,
		ID_SOLDIER_TAB_BTN			= 102,
		ID_EXTRA_TAB_BTN			= 103,
		ID_DECO_TAB_BTN				= 104,
		ID_ETC_TAB_BTN				= 105,
		ID_EVENT_GOODS_TAB_BTN		= 106,
		ID_COSTUME_TAB_BTN			= 107,
		ID_SECRET_TAB_BTN			= 108,
		ID_TAB_MAX					= 109,

		ID_SMALL_TAB_1_BTN			= 201,	// recommend : all , soldier : all         , extra : all    , deco : all        , etc : all
		ID_SMALL_TAB_2_BTN			= 202,	// recommend : hot , soldier : premium     , extra : weapon , deco : kindred    , etc : soldier
		ID_SMALL_TAB_3_BTN			= 203,	// recommend : new , soldier : short range , extra : armor  , deco : hair       , etc : extra
		ID_SMALL_TAB_4_BTN			= 204,	//                 , soldier : long range  , extra : helmet , deco : hair color , etc : monster
		ID_SMALL_TAB_5_BTN			= 205,	//                 , soldier : magic       , extra : cloak  , deco : face       , etc : fishing
		ID_SMALL_TAB_6_BTN			= 206,	//                 , soldier : special                      , deco : skin color , etc : guild
		ID_SMALL_TAB_7_BTN			= 207,	//                                                          , deco : underwear  , etc : other
		ID_SMALL_TAB_8_BTN			= 208,
		ID_SMALL_TAB_9_BTN			= 209,
		ID_SMALL_TAB_10_BTN			= 210,

		ID_ITEM_INFO_WND_1_BTN		= 301,
		ID_ITEM_INFO_WND_2_BTN		= 302,
		ID_ITEM_INFO_WND_3_BTN		= 303,
		ID_ITEM_INFO_WND_4_BTN		= 304,
		ID_ITEM_INFO_WND_5_BTN		= 305,
		ID_ITEM_INFO_WND_6_BTN		= 306,
		ID_ITEM_INFO_WND_7_BTN		= 307,
		ID_ITEM_INFO_WND_8_BTN		= 308,

		ID_BUY_WND					= 401,
		ID_BOUGHT_WND				= 402,
		ID_SUPERGASHAPON_WND		= 403,

		ID_BONUS_CASH_BTN			= 501,
		ID_BONUS_CASH_TOOLTIP		= 502,

		ID_VERT_SCROLL				= 1001,
		
		ID_START_WND				= 5000,
		ID_SEARCH_SHOW_BTN			= 5001,
		ID_SEARCH_HIDE_BTN			= 5002,
		ID_SEARCH_WND				= 5003,
	};

	enum
	{
		PAGE_PER_BTN_NUM		= 4,

		MAX_TAB_BTN				= 8,
		MAX_BUY_INFO_WND_BTN	= 8,
		MAX_SMALL_TAB_BTN		= 10, 
		

		SORT_WEIGHT_SMALL		= 100000000,  // ��
		SORT_WEIGHT_BIG			= 1000000000, // �ʾ�
				
		ETC_ITEM_OTHER_VALUE	= 10000,
		ETC_ITEM_MOTION_VALUE	= 20000,

		MAX_DECO_EQUIP			= 6, 
	};

protected:
	ioPlayStage*			m_pPlayStage;

	ioUIRenderFrame*		m_pFlashFrame;
		
	ioHashStringVec			m_vSmallTabNameVecArray[MAX_TAB_BTN];	//�������̸�
	IntVec					m_vSmallTabTypeVecArray[MAX_TAB_BTN];	//������Ÿ��
		
	//���� �� �з� �� ���� �з� � ���� ��ǰ
	vNewShopItemInfoVec		m_vNewShopItemInfoVec;
	
	//�� �� ������ ����
	DWORD					m_dwCurTabID;
	DWORD					m_dwCurSmallTabID;
	DWORD					m_dwSmallTabEndID;
	DWORD					m_dwDefaultShowTab;

	static DWORD			m_dwCashSendTime;
	static bool				m_bGashaponListRequest;

	int						m_iCurPage;
	int						m_iSelectCurArray;
	int						m_iSelectCurPage;

protected:
	//�̺�Ʈ
	ioUIRenderImage*		m_pEventMark;
	bool					m_bScreen;
	DWORD					m_dwScreenCheckTime;
	DWORD					m_dwScreenGapTime;
	DWORD					m_dwScreenTime;

	EventItemCounter		m_EventItemCounter;

protected:
	//ġ�� �� 3Dĳ���� �������
	int						m_iDecoClassType;
	int						m_iDecoSexType;
	
	int						m_iDecoReserveSexType;
	bool					m_bDecoReserve;
	bool					m_bSetDecoCharInfo;

	int						m_iEquipDecoTypeCodeArray[MAX_DECO_EQUIP];
	bool					m_bReleaseDecoEquip;

protected:
	//���ʽ� ĳ��
	DWORD			m_dwBonusCashTooltipCheckDuration;
	DWORD			m_dwBonusCashTooltipCheckStartTime;

//�ʱ�ȭ
public:
	virtual void ParseExtraInfo( ioXMLElement &xElement );	
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	
	void SetPlayStage( ioPlayStage *pPlayStage );
	
public:
	void ParseShopData( bool bMustParse = false );
	void ParseShopDataCache( DWORD dwTabID, DWORD dwSamllTabID, int iSexType = 0 );

//������ �̺�Ʈ
public:
	virtual void iwm_create();

	virtual void iwm_show();
	virtual void iwm_hide();
	virtual void SetPositionToDefault( int iParentWidth, int iParentHeight, bool bChildMove, bool bCreate = false );

	virtual void iwm_gototop();

	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );
	virtual void iwm_vscroll(DWORD dwID, int curPos );
	virtual void iwm_wheel( int zDelta );


//���� ��
protected:
	virtual void iwm_updatetab( DWORD dwID, int nSmallTabID = -1, bool bBeforeTabUse = true, bool bAfterTabUse = true, bool bSetFirstItem = true, bool bSearchHide = true );
	DWORD UpdateTabInActive( DWORD dwCurTab );

public:
	void UpdateTab( DWORD dwTabBtn, bool bSetFirstItem, int nSmallTab = -1, bool bSearchHide = true );
	void UpdateTabBtnUpBefore( DWORD dwTabID );
	void UpdateTabBtnUpAfter( DWORD dwTabID, int iSetPage=-1, int iCurrInfoBtn=-1 );

//���� ��
protected:
	bool UpdateSmallTab( DWORD dwSmallTabID, bool bSetFirstItem );
	bool UpdateSmallTabElement( DWORD dwTabBtn );

	int UpdateSmallTabInActive();
	int UpdateSmallTabName( ioWnd* pWnd, int iXOffset, const ioHashString& szTitle );

public:
	void GetSmallTabNameList( OUT ioHashStringVec& NameList );

public:
	int  GetSmallTabType( DWORD dwTabID, DWORD dwSmallTabID );
	bool IsRightSmallTab( DWORD dwTabID, DWORD dwSmallTabID , int iValue1, int iValue2 );

//�˻� ������
protected:
	void UpdateSearchWnd();

//��ǰ ��������
protected:
	void GetItemInfoVec( OUT vNewShopItemInfoVec& rkVec, IN DWORD dwCurTab, IN DWORD dwCurSmallTab, IN int eFilterType, IN const char* szFind );
	void UpdateShopItemInfoByMyInfo( IN int eFilterType, OUT NewShopItemInfo& kInfo );

public:
	void GetBonusPesoItemInfoVec( OUT vNewShopItemInfoVec &rvInfoVec );

//������ ������Ʈ
protected:	
	void UpdatePage( int iNewPage );

//���� ���� ���� ����
public:
	int GetCurrTab();
	int GetCurrSmallTab();
	int GetCurrPage();
	int GetCurrInfoBtn();
	bool GetCurItemInfo( OUT NewShopItemInfo &rkInfo, IN DWORD dwItemInfoID );	

 //����
protected:
	void OnBuyInfoBtnUp( DWORD dwItemInfoID, bool bReleaseDecoEquip = true );

public:
	void SetBoughtMark( int iTabID,  int iValue1, int iValue2 );
	void SetBoughtMsg( const char *szName, int iBuyPopupType );

//�� �̵�
public:
	void SetDirectTab( int iTabID,  int iValue1, int iValue2, bool bAllUpdate = true );
	void SetDirectSmallTab( int iTabID, int iSmallTabID, int iValue1, int iValue2, bool bAllUpdate = true );

	void RefreshCurrTab();
	void ShowADPage( bool bSecretShopMove = true );
	void ShowSoldierTab();
	void ShowExtraItemTab();
	void ShowDecoItemTab();
	void ShowEtcItemTab();
	void ShowEventGoodsItemTab();
	void ShowCostumeItemTab();	
	void ShowSecretShopItemTab();

	void SetTabAndItem( NewShopItemInfo &rkInfo );

//ġ�� �� 3Dĳ���� ��� ����
public:
	void SetDecoEquipArray( int iDecoType, int iDecoCode );
	int  GetDecoEquipArray( int iDecoType );
	void ReleaseDecoEquipArray();

	void SetDecoClassSex( int iDecoClassType, int iDecoSexType, bool bMyCharSex );
	void SetReserveDecoSex( int iDecoSexType );
	void SetDecoClassSexByRecommendedOrder( int iDecoSexType, int iClassType=0 );
	
	int  GetDecoClassType() const { return m_iDecoClassType; }
	int  GetDecoSexType() const { return m_iDecoSexType; }

	bool IsSetDecoCharInfo() const { return m_bSetDecoCharInfo; }
	bool IsReleaseDecoEquip() const { return m_bReleaseDecoEquip; }

	void SetReleaseDecoEquip( bool bReleaseDecoEquip ) { m_bReleaseDecoEquip = bReleaseDecoEquip; }
	void SetDecoCharInfo( bool bSetDecoCharInfo ) { m_bSetDecoCharInfo = bSetDecoCharInfo; }

//�̺�Ʈ ������	
public:
	int GetTabEventItemCnt( DWORD dwTabID );
	int GetTotalEventItemCnt() { return m_EventItemCounter.GetTotalItem(); }
	int GetSoldierEventItemCnt();
	int GetDecoEventItemCnt();
	int GetExtraItemEventItemCnt();
	int GetEtcItemEventItemCnt();
	int GetEventGoodsItemEventItemCnt();
	int GetCostumeEventItemCnt();	
	void UpdateEventItemCnt();

	void GoodsSyncComplete( EventShopType eType, bool bShopClose, DWORD dwEtcItem = 0, int iValue1 = 0, int iValue2 = 0 );

//�׿� ���� ��û
public:
	void SendRequestGashaponList();
	bool IsGashaponListRequest() const { return m_bGashaponListRequest; }
	void SendGetCash( bool bMust=false );

//����Ʈ ���̵�
public:
	bool QuestGuide( int iMTabID, int iSTabID, int iBtnValue1, int iBtnValue2, int iBtnValue3, int iBtnValue4, int iBtnValue5, float &rfXPos, float &rfYPos );
	bool CompareBtnInfo( NewShopItemInfo &rkInfo, int iBtnValue1, int iBtnValue2, int iBtnValue3 );

protected:
	virtual void OnProcess( float fTimePerSec );

	virtual void OnRender();
	virtual void OnRenderAfterChild();

	void MainTabEventMarkRender( DWORD dwTabID );
//���ʽ� ĳ��
protected:
	void CheckBonusCash();

public:
	NewShopWnd(void);
	virtual ~NewShopWnd(void);
};