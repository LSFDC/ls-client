#pragma once

class ioButton;
class ioUIRenderImage;
class ioUI3DEffectRender;

class PopupStoreWnd : public ioWnd
{
public:
	//��ư, �̹��� ���
	enum { 
		//LAYER
		eID_BASE = 0,
		eID_EFFECTS = 2,
		//��ư
		eID_ITEM = 1,
		eID_NAME,
		eID_GOLDS,
		eID_INFO,
		eID_BUY,
		eID_ETCS,
		eID_SHOW,
		eID_CLOSE,
		eID_IMGS1,
		eID_IMGS2,
		eID_COUNT,
		eID_INFO2,

		eID_ITEM_IMG = 101,
		eID_GOLD_SUB1 = 301,
		eID_GOLD_SUB2,
		eID_GOLD_SUB3,
		eID_GOLD_SUB4,

		eID_ETC_SUB1 = 601,
		eID_ETC_SUB2,
		eID_ETC_SUB3,
		eID_ETC_SUB4,
		eID_ETC_SUB5,
		eID_ETC_SUB6,
	};

	enum { ePopupConditionTotalCash = 0, ePopupConditionMonthCash, ePopupConditionCurCash, ePopupConditionGrade, ePopupConditionPlayTime, ePopupConditionMax };
	enum { eEffectBefore = 0, eEffectAfter, eEffectResult, eEffectSub, eEffectMax };
	enum { eMsgClose = 0, eMsgRealBuy, eMsgError };

	struct sPopupItemInfo
	{
		bool bActive;

		int iIndex;
		int iPresentType;
		int iPresentValue1;
		int iPresentValue2;
		int iCash;
		int iPrevCash;
		int iDisCount;
		int iManualIdx;
		int iChance;

		DWORD dwConditionMin[ePopupConditionMax];
		DWORD dwConditionMax[ePopupConditionMax];
	};

private:
	ioButton*			m_pBtnBuy;
	ioButton*			m_pBtnShow;
	ioButton*			m_pBtnClose;

	ioWnd*				m_pWndImgs1;
	ioWnd*				m_pWndImgs2;

	ioWnd*				m_pWndName;
	ioWnd*				m_pWndCount;
	ioWnd*				m_pWndItem;

	ioWnd*				m_pWndInfo1;
	ioWnd*				m_pWndInfo2;

	ioWnd*				m_pWndGolds;
	ioWnd*				m_pWndGoldWnd1;	// ��������
	ioWnd*				m_pWndGoldWnd2;	// �̹��� ��
	ioWnd*				m_pWndGoldWnd3;	// ���εȰ���
	ioWnd*				m_pWndGoldWnd4;	// ������

	ioWnd*				m_pWndEtcs;
	ioWnd*				m_pWndEtcWnd1;	// �ؽ�Ʈ
	ioWnd*				m_pWndEtcWnd2;	// ���
	ioWnd*				m_pWndEtcWnd3;	// ����
	ioWnd*				m_pWndEtcWnd4;	// â�ݱ�
	ioWnd*				m_pWndEtcWnd5;	// | �̹���
	ioWnd*				m_pWndEtcWnd6;	// | �̹���

	ioUI3DEffectRender* m_pEffectItem[eEffectMax];

private:
	bool		m_bGoldShowType; // �������� �ƴ���

	//string
	ioHashString m_szGold;			// ���
	ioHashString m_szRealClose;		// ���� �����ðڽ��ϱ�
	ioHashString m_szRealBuy;		// ���� ����..
	ioHashString m_szCount;			// ��
	ioHashString m_szString1;	//���콺 ������ �� ������ ���� �ֽ��ϴ�.
	ioHashString m_szString2;	//�����Ͻ� �������� �����Կ��� Ȯ���Ͻ� �� �ֽ��ϴ�.
	ioHashString m_szError;		// ���� ���� ���
	ioHashString m_szBtnNameBuy;	//����
	ioHashString m_szBtnNameCharge;	// ����
	ioHashString m_szBtnNameClose;	// �ݱ�
	ioHashString m_szBtnNameShow;	// �����԰���
	ioHashString m_szEffectPlaySound;
	ioHashString m_szBtnOverSound;
	ioHashString m_szBtnClickSound;

	ioHashString m_szTitle;
	ioHashString m_szTitle2;
	ioHashString m_szIconName;
	ioHashString m_szCashText1;
	ioHashString m_szCashText2;
	ioHashString m_szCashText3;

public:
	bool			m_bSubPopup;
	bool			m_bPage;
	int				m_iCash;
	int				m_iMsgType;
	int				m_iPopupIndex;	
	DWORD			m_dwShowStartTime;
	sPopupItemInfo	m_OnPopupItemInfo;
	std::vector<sPopupItemInfo> m_vecPopupItemInfo;

public:
	virtual void iwm_create();
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual bool iwm_esc();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

	void OnUpdateMyCash( int iCash );	
	void OnUpdatePresentText();			// TEXT1��°��
	void OnUpdatePresentText2();		// TEXT2��°��
	void OnUpdatePresentIconText();
	void OnUpdatePresentWnd();
	void OnUpdateGoldWindow();

public:	

	void SetSubPopup( bool bSet ) { m_bSubPopup = bSet; }
	void OnChanceAfter();	//�տ� ��÷â �� �����..
	void OnPage1();	//�⺻â
	void OnPage2();	//���â

	int OnItemOver();
	int OnItemLeave();

	int OnBtnBuy();
	int OnBtnShow();
	int OnBtnClose();

	int OnChargeOver();
	int OnBtnCharge();
	int OnChargeLeave();

	int OnCloseOver();
	int OnBtnExit();
	int OnCloseLeave();

	int GetPopupIndex() { return m_iPopupIndex; }
	sPopupItemInfo GetPopupItemInfo();
	
	void OpenPopup( int iPopupIndex );
	void OnBuySuccess();
	void OnButFaild( DWORD dwReturnType );

public:
	PopupStoreWnd();
	virtual ~PopupStoreWnd();
};

class PopupStoreSubWnd : public ioWnd
{
public:
	enum { eID_BASE = 0, eBTN_OPEN };
		
	virtual void iwm_create();
	virtual void iwm_show();
	virtual void iwm_hide();

	DWORD		m_dwShowStartTime;
	ioButton*	m_pBtnOpen;
	ioUI3DEffectRender* m_pEffectReady;

	int OnBtnOpen();


	PopupStoreSubWnd();
	virtual ~PopupStoreSubWnd();
};