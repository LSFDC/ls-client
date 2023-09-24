#pragma once

class ioUIRenderImage;
class ioComplexStringPrinter;

struct MagicEffectInfo
{
	DWORD m_dwCreateTime;

	int m_iCurAlphaRate;
	float m_fCurScaleRate;


	MagicEffectInfo()
	{
		Init();
	}

	void Init()
	{
		m_dwCreateTime = 0;
		m_fCurScaleRate = 1.0f;
		m_iCurAlphaRate = MAX_ALPHA_RATE;
	}
};

typedef std::vector< MagicEffectInfo > vMagicEffectInfo;

class ItemReinforceDownWnd :
	public ioWnd
{
public:

	//weapon btn
	enum 
	{
		ID_ENABLE_REINFORECE_ITEM_SELECT_BTN	 = 100,
		ID_REINFORCE_BTN						 = 101,
	};

	//weapon info count
	enum 
	{
		ITEM_INFO_CNT = 2,
		ITEM_REINFORCE_INFO_CNT = 4,
	};
protected:
	int		  m_iItemIndex;
	ioUIRenderImage *m_pNameLine;
	ioUIRenderImage *m_pMagicCircle;
	ioUIRenderImage *m_pIconBack;
	ioUIRenderImage *m_pIcon;
	ioUIRenderImage *m_pNumText;
	ioUIRenderImage *m_pPlusText;
	ioComplexStringPrinter m_ItemInfo[ ITEM_INFO_CNT ];
	ioComplexStringPrinter m_ItemReinforceInfo[ ITEM_REINFORCE_INFO_CNT ];
	ioComplexStringPrinter m_SoulStonePrinter;
	ioComplexStringPrinter m_ItemSelectPrinter;

	//아이템 강화 수치
	int m_iItemReinforce;

	// magic effect
	vMagicEffectInfo m_vMagicEffectInfoList;
	DWORD m_dwMagicEffectCreateTime;

	//result effect
	bool m_bShowResultEffect;
	DWORD m_dwResultStartTime;
	DWORD m_dwResultEndTime;
	DWORD m_dwResultEffectEndTime;
	DWORD m_dwResultEffectTime;
	ioUIRenderImage *m_pLightImage;
	float m_fScaleRate;
	float m_fAlphaRate;
	float m_fSlopeRate;
	BYTE m_iAlpha1;
	BYTE m_iAlpha2;
	float m_fCurEffectScale;
	//result sound
	ioHashString m_szRollingSound;
	UIRenderType m_iIconRenderType;
	int m_nGradeType;

public:
	void SetItemInfo( int iIndex );
	void ProcessMagicCircle();
	void ProcessResultEffect();
public:
	virtual void iwm_hide();
	virtual void OnProcess( float fTimePerSec );
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );
	virtual void ParseExtraInfo( ioXMLElement &xElement );
	
protected:
	virtual void OnRender();
	virtual void OnRenderAfterChild();

protected:
	void ClearListAll();
	void CreateMagicCircle();
	void OnRenderMagicCircle();
	void OnRenderResultEffect();

public:
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	

public:
	ItemReinforceDownWnd(void);
	virtual ~ItemReinforceDownWnd(void);
};

class ItemReinforceDoneWnd :
	public ioWnd
{
public:
	//weapon btn
	enum 
	{
		ID_REINFORECE_AGAIN_BTN					 = 100,
		ID_SEE_ITEM_BTN							 = 101,
	};

	//weapon info count
	enum 
	{
		ITEM_INFO_CNT = 2,
		ITEM_REINFORCED_INFO_CNT = 4,
	};
protected:
	int		  m_iItemIndex;
	ioUIRenderImage *m_pNameLine;
	ioUIRenderImage *m_pMagicCircle;
	ioUIRenderImage *m_pIconBack;
	ioUIRenderImage *m_pIcon;
	ioUIRenderImage *m_pNumText;
	ioUIRenderImage *m_pPlusText;
	ioComplexStringPrinter m_ItemInfo[ ITEM_INFO_CNT ];
	ioComplexStringPrinter m_ItemReinforceInfo[ ITEM_REINFORCED_INFO_CNT ];
	ioComplexStringPrinter m_SoulStonePrinter;
	ioComplexStringPrinter m_ReinforcedPrinter;

	//아이템 강화 수치
	int m_iItemReinforce;

	// magic effect
	vMagicEffectInfo m_vMagicEffectInfoList;

	DWORD m_dwMagicEffectCreateTime;

	//Success light effect
	ioUIRenderImage *m_pSuccessLightImage;
	ioHashStringVec m_vSuccessLightImage;	// 성공
	DWORD   m_dwFinalResultStartTime;
	DWORD	m_dwSuccessEffectTime;
	int		m_iCurSuccessEffectArray;
	DWORD   m_dwRollingEffectCheckTime;
	DWORD   m_dwRollingEffectTime;
	//Success Effect
	ioHashString m_szSuccessSound;
	int m_nGradeType;

public:
	void SetItemInfo( int iIndex );
	void ProcessMagicCircle();
	void ProcessSuccessLightEffect();

public:
	virtual void OnProcess( float fTimePerSec );
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );
	virtual void iwm_show();
	virtual void iwm_hide();

protected:
	virtual void OnRender();

protected:
	void ClearListAll();
	void CreateMagicCircle();
	void OnRenderMagicCircle();
	void OnRenderSuccessLightEffect();
	
public:
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	
	virtual void ParseExtraInfo( ioXMLElement &xElement );

public:
	ItemReinforceDoneWnd(void);
	virtual ~ItemReinforceDoneWnd(void);
};