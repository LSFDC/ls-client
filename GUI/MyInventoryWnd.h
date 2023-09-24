#pragma once

#include "../CharacterInfo.h"
#include "../io3DEngine/ioStringConverter.h"
#include "../ioComplexStringPrinter.h"

#include "ManualWnd.h"
#include "MySoldierWnd.h"
#include "SoldierPackageWnd.h"
#include "PreSetSoldierPackageWnd.h"
#include "ioPullDownEventListener.h"

class ioUI3DRender;
class ioUIRenderFrame;
class ioCreateChar;
class ioItem;
class ioSetItemInfo;
class ioPlayStage;
class ioAlchemicDragItem;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum 
{
	INVENTORY_MARK_TYPE_NONE	= 0,
	INVENTORY_MARK_TYPE_HIRED	= 1,
	INVENTORY_MARK_TYPE_EQUIP	= 2,
	INVENTORY_MARK_TYPE_GROWTH_ENABLE	= 3,
	INVENTORY_MARK_TYPE_GROWTH_DISABLE	= 4,
	INVENTORY_MARK_TYPE_TIMEGROWTH		= 5,
	INVENTORY_MARK_TYPE_TRADE_WAIT		= 6,
	INVENTORY_MARK_TYPE_TRADE_ENABLE	= 7,
};

enum
{
	INVENTORY_ITEM_EXTRAITEM	= 1,
	INVENTORY_ITEM_DECO			= 2,
	INVENTORY_ITEM_ETC			= 3,
	INVENTORY_ITEM_PRESENT		= 4,
	INVENTORY_ITEM_MEDALITEM    = 5,
	INVENTORY_ITEM_SUBSCRIPTION	= 6,
	INVENTORY_ITEM_COSTUME		= 7,
	POPUPSTORE_ITEM_PRESENT		= 8,
	INVENTORY_ITEM_ACCESSORY	= 9,
};

enum
{
	INVENTORY_ACTION_NONE		 = 0,
	INVENTORY_ACTION_EQUIP		 = 1,
	INVENTORY_ACTION_RELEASE	 = 2,
	INVENTORY_ACTION_DETAIL		 = 3,
	INVENTORY_ACTION_RESELL		 = 4,
	INVENTORY_ACTION_CUSTOM_INFO = 5,
	INVENTORY_ACTION_DISASSEMBLE = 6,
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


typedef struct tagSoldierItemInfo
{
	int m_iClassType;
	int m_iPriorityOrder; // 낮을 수록 순위가 높다.

	tagSoldierItemInfo()
	{
		Init();
	}

	void Init()
	{
		m_iClassType = 0;
		m_iPriorityOrder = 13;
	}

} SoldierItemInfo;

typedef std::vector<SoldierItemInfo> vSoldierItemInfoList;

class SoldierItemInfoSort : public std::binary_function< const SoldierItemInfo& , const SoldierItemInfo&, bool >
{
public:
	bool operator() ( const SoldierItemInfo &lhs, const SoldierItemInfo &rhs ) const
	{
		if( lhs.m_iPriorityOrder < rhs.m_iPriorityOrder )
			return true;

		return false;
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct tagInventoryItemInfo
{
	int m_iMainType;
	int m_iMagicCode;
	int m_iMagicCodeEx;
	int m_iMagicCodeEx2;
	int m_iManualIndex;

	bool m_bEquip;

	// 치장을 위한 값
	int m_iSexType;
	int m_iDecoType;
	int m_iDecoCode;

	// 청약 상품을 위한 값
	ioHashString m_szSubscriptionID;
	int m_iSubscriptionGold;
	int m_iSubscriptionBonusGold;

	bool m_bActive;

	int  m_iPriorityOrder;
	int  m_iPriorityOrder2;

	tagInventoryItemInfo()
	{
		Init();
	}

	void Init()
	{
		m_iMainType = 0;
		m_iMagicCode = 0;
		m_iMagicCodeEx = 0;
		m_iMagicCodeEx2= 0;
		m_iManualIndex = 0;

		m_bEquip = false;
		m_bActive = true;

		m_iSexType  = 0;
		m_iDecoType = 0;
		m_iDecoCode = 0;
		m_iPriorityOrder = 999999999; // ORDER_DEFAULT
		m_iPriorityOrder2 = 999999999;

		m_iSubscriptionGold = 0;
		m_iSubscriptionBonusGold = 0;
	}

} InventoryItemInfo;

typedef std::vector<InventoryItemInfo> vInventoryItemInfoList;

class EtcItemPrioritySort : public std::binary_function< const InventoryItemInfo& , const InventoryItemInfo&, bool >
{
public:
	bool operator() ( const InventoryItemInfo &lhs, const InventoryItemInfo &rhs ) const
	{
		if( lhs.m_iMainType == INVENTORY_ITEM_ETC && rhs.m_iMainType != INVENTORY_ITEM_ETC )
			return true;

		if( lhs.m_iMainType == INVENTORY_ITEM_ETC && rhs.m_iMainType == INVENTORY_ITEM_ETC )
		{
			if( lhs.m_iPriorityOrder < rhs.m_iPriorityOrder )
				return true;

			if( lhs.m_iPriorityOrder == rhs.m_iPriorityOrder )
			{
				if( lhs.m_iMagicCode < rhs.m_iMagicCode )
					return true;
			}
		}
		else if( lhs.m_iMainType != INVENTORY_ITEM_ETC && rhs.m_iMainType == INVENTORY_ITEM_ETC )
		{
			return false;
		}
		else
		{
			if( lhs.m_iMainType < rhs.m_iMainType )
				return true;

			if( lhs.m_iMainType == rhs.m_iMainType )
			{
				if( lhs.m_iPriorityOrder < rhs.m_iPriorityOrder )
					return true;

				if( lhs.m_iPriorityOrder == rhs.m_iPriorityOrder )
				{
					if( lhs.m_iMagicCode < rhs.m_iMagicCode )
						return true;
				}
			}
		}

		return false;
	}
};

class InventoryItemInfoSort : public std::binary_function< const InventoryItemInfo& , const InventoryItemInfo&, bool >
{
public:
	bool operator() ( const InventoryItemInfo &lhs, const InventoryItemInfo &rhs ) const
	{
		if( lhs.m_iMainType < rhs.m_iMainType )
			return true;

		if( lhs.m_iMainType == rhs.m_iMainType )
		{
			if( lhs.m_iPriorityOrder < rhs.m_iPriorityOrder )
				return true;

			if( lhs.m_iPriorityOrder == rhs.m_iPriorityOrder )
			{
				if( lhs.m_iMagicCode < rhs.m_iMagicCode )
					return true;
			}
		}

		return false;
	}
};

// m_iMagicCode 역순으로 정렬
class InventoryItemInfoSort2 : public std::binary_function< const InventoryItemInfo& , const InventoryItemInfo&, bool >
{
public:
	bool operator() ( const InventoryItemInfo &lhs, const InventoryItemInfo &rhs ) const
	{
		if( lhs.m_iMainType == INVENTORY_ITEM_ETC && rhs.m_iMainType != INVENTORY_ITEM_ETC )
			return true;

		if( lhs.m_iMainType == INVENTORY_ITEM_ETC && rhs.m_iMainType == INVENTORY_ITEM_ETC )
		{
			if( lhs.m_iMagicCode > rhs.m_iMagicCode )
				return true;

			if( lhs.m_iMagicCode == rhs.m_iMagicCode )
			{
				if( lhs.m_iPriorityOrder < rhs.m_iPriorityOrder )
					return true;
			}
		}
		else if( lhs.m_iMainType != INVENTORY_ITEM_ETC && rhs.m_iMainType == INVENTORY_ITEM_ETC )
		{
			return false;
		}
		else
		{
			if( lhs.m_iMainType < rhs.m_iMainType )
				return true;

			if( lhs.m_iMainType == rhs.m_iMainType )
			{
				if( lhs.m_iMagicCode > rhs.m_iMagicCode )
					return true;

				if( lhs.m_iMagicCode == rhs.m_iMagicCode )
				{
					if( lhs.m_iPriorityOrder < rhs.m_iPriorityOrder )
						return true;
				}
			}
		}

		return false;
	}
};

class InventoryItemInfoSort3 : public std::binary_function< const InventoryItemInfo& , const InventoryItemInfo&, bool >
{
public:
	bool operator() ( const InventoryItemInfo &lhs, const InventoryItemInfo &rhs ) const
	{
		if( lhs.m_iMainType == INVENTORY_ITEM_ETC && rhs.m_iMainType != INVENTORY_ITEM_ETC )
			return true;

		if( lhs.m_iMainType == INVENTORY_ITEM_ETC && rhs.m_iMainType == INVENTORY_ITEM_ETC )
		{
			if( lhs.m_iPriorityOrder2 < rhs.m_iPriorityOrder2 )
				return true;

			if( lhs.m_iPriorityOrder2 == rhs.m_iPriorityOrder2 )
			{
				if( lhs.m_iPriorityOrder < rhs.m_iPriorityOrder )
					return true;

				if( lhs.m_iPriorityOrder == rhs.m_iPriorityOrder )
				{
					if( lhs.m_iMagicCode < rhs.m_iMagicCode )
						return true;
				}
			}
		}
		else if( lhs.m_iMainType != INVENTORY_ITEM_ETC && rhs.m_iMainType == INVENTORY_ITEM_ETC )
		{
			return false;
		}
		else
		{
			if( lhs.m_iMainType < rhs.m_iMainType )
				return true;

			if( lhs.m_iMainType == rhs.m_iMainType )
			{
				if( lhs.m_iPriorityOrder2 < rhs.m_iPriorityOrder2 )
					return true;

				if( lhs.m_iPriorityOrder2 == rhs.m_iPriorityOrder2 )
				{
					if( lhs.m_iPriorityOrder < rhs.m_iPriorityOrder )
						return true;

					if( lhs.m_iPriorityOrder == rhs.m_iPriorityOrder )
					{
						if( lhs.m_iMagicCode < rhs.m_iMagicCode )
							return true;
					}
				}
			}
		}

		return false;
	}
};

// m_iMagicCodeEx 역순으로 정렬
class InventoryItemInfoSort4 : public std::binary_function< const InventoryItemInfo& , const InventoryItemInfo&, bool >
{
public:
	bool operator() ( const InventoryItemInfo &lhs, const InventoryItemInfo &rhs ) const
	{
		if( lhs.m_iMainType == INVENTORY_ITEM_ETC && rhs.m_iMainType != INVENTORY_ITEM_ETC )
			return true;

		if( lhs.m_iMainType == INVENTORY_ITEM_ETC && rhs.m_iMainType == INVENTORY_ITEM_ETC )
		{
			if( lhs.m_iMagicCodeEx > rhs.m_iMagicCodeEx )
				return true;

			if( lhs.m_iMagicCodeEx == rhs.m_iMagicCodeEx )
			{
				if( lhs.m_iPriorityOrder < rhs.m_iPriorityOrder )
					return true;
			}
		}
		else if( lhs.m_iMainType != INVENTORY_ITEM_ETC && rhs.m_iMainType == INVENTORY_ITEM_ETC )
		{
			return false;
		}
		else
		{
			if( lhs.m_iMainType < rhs.m_iMainType )
				return true;

			if( lhs.m_iMainType == rhs.m_iMainType )
			{
				if( lhs.m_iMagicCodeEx > rhs.m_iMagicCodeEx )
					return true;

				if( lhs.m_iMagicCodeEx == rhs.m_iMagicCodeEx )
				{
					if( lhs.m_iPriorityOrder < rhs.m_iPriorityOrder )
						return true;
				}
			}
		}

		return false;
	}
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct tagAlchemicItemInfo
{
	int m_iCode;
	int m_iCount;
	ioHashString m_szIcon;

	bool m_bNewAdd;

	tagAlchemicItemInfo()
	{
		Init();
	}

	void Init()
	{
		m_iCode = 0;
		m_iCount = 0;

		m_szIcon.Clear();

		m_bNewAdd = false;
	}

} AlchemicItemInfo;

typedef std::vector<AlchemicItemInfo> vAlchemicItemInfoList;

class AlchemicItemSort : public std::binary_function< const AlchemicItemInfo& , const AlchemicItemInfo&, bool >
{
public:
	bool operator() ( const AlchemicItemInfo &lhs, const AlchemicItemInfo &rhs ) const
	{
		if( lhs.m_iCode < rhs.m_iCode )
			return true;

		if( lhs.m_iCode == rhs.m_iCode )
		{
			if( lhs.m_iCount < rhs.m_iCount )
				return true;
		}

		return false;
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct tagAlchemicToolTipInfo
{
	int m_iCode;
	int m_iType;

	tagAlchemicToolTipInfo()
	{
		Init();
	}

	void Init()
	{
		m_iCode = 0;
		m_iType = 0;
	}

} AlchemicToolTipInfo;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct InventorySelectItem
{
	int			 m_iMagicCode;

	int			 m_iTabType;

	int			 m_iNeedCoin;
	ioHashString m_szName;
	ioHashString m_szIconName;
	ioHashString m_szSubText;
	DWORD		 m_dwSubTextColor;
	ioHashString m_szSubText2;
	int m_nGrade;

	InventorySelectItem()
	{
		Init();
	}

	void Init()
	{
		m_iNeedCoin = -1;
		m_iTabType = -1;
		m_iMagicCode = -1;
		m_dwSubTextColor = TCT_DEFAULT_BLUE;
		m_szName.Clear();
		m_szIconName.Clear();
		m_szSubText.Clear();
		m_szSubText2.Clear();
		m_nGrade = 0;
	}
};
typedef std::vector< InventorySelectItem > vInventorySelectItemList;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class InventoryIconBtn : public ioButton
{
public:
	enum NumberType
	{
		NT_NONE,
		NT_SMALL,
	};

protected:
	ioUIRenderImage *m_pIcon;
	ioUIRenderImage *m_pIconBack;
	ioUIRenderImage *m_pIconSelect;
	ioUIRenderImage *m_pCustomIcon;
	ioUIRenderImage *m_pCostumeMark;

	ioUIRenderImage *m_pNumText;
	ioUIRenderImage *m_pPlusText;
	ioUIRenderImage *m_pSmallNumText;
	ioUIRenderImage *m_pSmallPlusText;

	ioUIRenderImage *m_pGaugeBack;
	ioUIRenderImage *m_pGaugeImg;

	ioUIRenderFrame *m_pStateBlueFrm;
	ioUIRenderFrame *m_pStateGreenFrm;
	ioUIRenderFrame *m_pStateGreenFrm2;
	ioUIRenderFrame *m_pStateOrangeFrm;
	ioUIRenderFrame *m_pStatePinkFrm;

	int m_iMarkType; // INVENTORY_MARK_TYPE_NONE, INVENTORY_MARK_TYPE_HIRED, INVENTORY_MARK_TYPE_EQUIP

	int m_iNum;
	float m_fScale;
	float m_fBackScale;

	// for deco
	int m_iSexType;
	int m_iDecoType;
	int m_iDecoCode;

	// for timegrowth
	int m_iNeedTime;
	int m_iGapTime;

	// for etcitem
	int m_iEtcItemType;

	bool m_bEffect;
	DWORD m_dwCheckTime;
	DWORD m_dwEffectTime;
	DWORD m_dwEffectHide;

	int m_iClassType;
	bool m_bExtraItemCustom;
	bool m_bExtraItemGrowthCatalyst;

	bool m_bSelect;

	NewShopItemInfo *m_pNewShopItemInfo;

	bool m_bIsCostume;

	int m_nGradeType;
	InventoryIconBtn::NumberType m_eNumberType;

protected:
	int GetRenderImageStartHeight( ioUIRenderImage *pImage, int iSrcHeight, int iCur, int iMax );

protected:
	void RenderGaugeIcon( int iXPos, int iYPos, ioUIRenderImage *pImage, int iSrcHeight, int iCur, int iMax, UIRenderType eType );
	void RenderIcon(int iXPos, int iYPos, UIRenderType eType);
	void RenderBack(int iXPos, int iYPos );
	void RenderOtherInfo( int iXPos, int iYPos );
	void RenderSelect( int iXPos, int iYPos );

public:
	bool SetIcon( ioUIRenderImage *pIcon, float fScale, float fBackScale, int iNum=0, InventoryIconBtn::NumberType eType = NT_NONE );
	void SetMarkType( int iMarkType ) { m_iMarkType = iMarkType; } 
	void SetExtraItemCustom( bool bExtraItemCustom ){ m_bExtraItemCustom = bExtraItemCustom; }
	void SetExtraItemGrowthCatalyst( bool ExtraItemGrowthCatalyst ){ m_bExtraItemGrowthCatalyst = ExtraItemGrowthCatalyst; }
	void SetCostume( bool bIsCostume ){ m_bIsCostume = bIsCostume; }
	void SetGradeType( int nGradeType ){ m_nGradeType = nGradeType; }

	void SetDecoInfo( int iSexType, int iDecoType, int iDecoCode );
	void GetDecoInfo( OUT int &riSexType, OUT int &riDecoType, OUT int &riDecoCode );
	void SetClassType(int ClassType) { m_iClassType = ClassType; }
	void SetEtcItemType( int iEtcItem );
	void SetSelect(bool bSelect) { m_bSelect = bSelect; }

	void SetNewShopItemInfo( const NewShopItemInfo &rkNewShopItemInfo );
	void GetNewShopItemInfo( OUT NewShopItemInfo &rkNewShopItemInfo );

public:
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	
	virtual void AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame );
	virtual void ParseExtraInfo( ioXMLElement &xElement );

protected:
	virtual void OnDrawNormal( int iXPos, int iYPos );
	virtual void OnDrawOvered( int iXPos, int iYPos );
	virtual void OnDrawPushed( int iXPos, int iYPos );
	virtual void OnDrawDisabled( int iXPos, int iYPos );

protected:
	virtual void OnProcess( float fTimePerSec );

	void ProcessTimeGrowth( float fTimePerSec );

public:
	InventoryIconBtn();
	virtual ~InventoryIconBtn();
};

//////////////////////////////////////////////////////////////////////////
class InventoryEtcMotionBtn : public ioButton
{
protected:
	ioUIRenderFrame *m_pOrangeNor;
	ioUIRenderFrame *m_pOrangePush;
	ioUIRenderFrame *m_pGrayNor;
	ioUIRenderFrame *m_pGrayPush;
	ioUIRenderFrame *m_pCurrentOver;

	ioUIRenderFrame *m_pCurrentNor;
	ioUIRenderFrame *m_pCurrentPush;

protected:
	DWORD m_dwEtcItem;
	bool  m_bSelected;
	ioHashString m_szTitle;

public:
	void SetEtcCode( DWORD dwCode );
	DWORD GetEtcCode(){ return m_dwEtcItem; }

public:
	virtual void AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame );

public:
	virtual void OnDrawNormal( int iXPos, int iYPos );
	virtual void OnDrawOvered( int iXPos, int iYPos );
	virtual void OnDrawPushed( int iXPos, int iYPos );
	void OnDrawTitle( int iXPos, int iYPos );

public:
	InventoryEtcMotionBtn();
	virtual ~InventoryEtcMotionBtn();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class InventoryPresentBtn : public ioButton
{
public:
	enum
	{
		IPBT_RECV	= 1,
		IPBT_USE	= 2,
	};

protected:
	int m_iPresentBtnType;

public:
	void SetPresentBtnType( int iType );

	inline int GetButtonType() { return m_iPresentBtnType; }

public:
	InventoryPresentBtn();
	virtual ~InventoryPresentBtn();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define INVENTORY_ITEM_INFO_WIDTHCUT_SIZE 90.0

class InventoryItemWnd : public ioButton, public PullDownEventListener
{
public:
	enum 
	{
		ID_ICON_BTN			= 1,	// all
		ID_EQUIP_BTN		= 2,	// extraitem
		ID_RELEASE_BTN		= 3,	// extraitem
		ID_RESELL_BTN		= 4,	// extraitem
		ID_USE_ON_BTN       = 5,    // etcitem
		ID_USE_OFF_BTN      = 6,    // etcitem
		ID_CUSTOM_INFO_BTN  = 7,    // extraitem
		ID_ETC_MOTION_BTN   = 8,    // motion_item
		ID_DISASSEMBLE_BTN	= 9,	// 분해,
		ID_ETC_SLOT_BTN		= 10,	// 소모품...
		ID_RESELL_MEDAL_BTN = 11,	// MedalItem
		ID_PRESENT_BTN		= 12,	// present
		ID_ADDED_BTN		= 13,	// 추가 기능 ( 용병강화 아이템만 )
		ID_POWERUP_LIST		= 14,	// 추가 기능 List
		MAX_BTN,

		MAX_DESC			= 3,
	};

protected:
	ioPlayStage *m_pPlayStage;
	bool m_bWidthCutDescArray[MAX_DESC];
	ioComplexStringPrinter m_szDescArray[MAX_DESC];

	InventoryItemInfo m_ItemInfo;

	bool m_bEquip;
	bool m_bActive;

	int m_iMainType;
	int m_iMagicCode;
	int m_iMagicCodeEx;

	int m_iDecoType;
	int m_iDecoCode;

	DWORD m_dwCurIconActionID;

	ioUIRenderImage *m_pEmptyImg;
	ioUIRenderFrame *m_pSelectFrm;
	bool             m_bSelect;

	ioUIRenderImage *m_pPlusImg;
	ioUIRenderImage *m_pMinusImg;
	ioUIRenderImage *m_pLeftImg;

	ioHashString m_szCurActionText;
	ioUIRenderImage *m_pCurActionImg;

	int m_iCurXGap;
	int m_iCurYGap;

protected:
	void SetBlank();
	void ClearDesc();
	void RenderDesc( int iXPos, int iYPos );

	void SetIsWidthCutDesc( int iArray , bool bWidthCut );
	bool IsWidthCutDesc( int iArray );

	bool SetExtraItemInfo();
	bool SetDecoItemInfo( InventoryItemInfo &rkInfo );
	bool SetEtcItemInfo();
	bool SetPresentItemInfo();
	bool SetMedalItemInfo();
	bool SetSubscriptionItemInfo( InventoryItemInfo &rkInfo );
	bool SetCostumeInfo();
	bool SetAccessoryInfo();

	void SendEtcItemOnOff( DWORD dwID );

protected:
	void SetPresentSoldier( int iValue1, int iValue2 );
	void SetPresentDeco( int iValue1, int iValue2 );
	void SetPresentEtcItem( int iValue1, int iValue2 );
	void SetPresentPeso( int iValue1, int iValue2 );
	void SetPresentExtraItem( int iValue1, int iValue2, short sPresentMent );
	void SetPresentExtraItemBox( int iValue1, int iValue2 );
	void SetPresentRandomDeco( int iValue1, int iValue2 );
	void SetPresentGradeExp( int iValue1, int iValue2 );
	void SetPresentMedalItem( int iValue1, int iValue2 );
	void SetPresentAlchemicItem( int iValue1, int iValue2 );
	void SetPresentPet( int iValue1, int iValue2 );
	void SetPresentCostume( int iValue1, int iValue2 );
	void SetPresentCostumeBox( int iValue1, int iValue2 );
	void SetPresentBonusCash( int iValue1, int iValue2 );
	void SetPresentAccessory( int iValue1, int iValue2 );

	void ProcessDesc();
	void ProcessExtraItemDesc();
	void ProcessEtcItemDesc();
	void ProcessMedalItemDesc();
	void ProcessCostumeDesc();
	void ProcessToolTip();
	void ProcessAccessoryDesc();

public:
	void SetInfo( ioPlayStage *pPlayStage, InventoryItemInfo &rkInfo );

	InventoryItemInfo& GetInfo();

	int GetDecoType();
	int GetDecoCode();

	inline int GetMainType() const { return m_iMainType; }
	inline int GetMagicCode() const { return m_iMagicCode; }
	inline int GetMagicCodeEx() const { return m_iMagicCodeEx; }
	inline DWORD GetCurActionID() const { return m_dwCurIconActionID; }

	void SetSelect(bool bSelect);
	bool IsCustomToolTip();

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

public:
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );
	virtual void iwm_hide();

	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );
	virtual void AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame );

	virtual void iwm_pulldown_event( const PullDownEvent& Event );

public:
	bool QuestGuide( const IntVec &rValue, float &rfXPos, float &rfYPos );

public:
	InventoryItemWnd();
	virtual ~InventoryItemWnd();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class GrowthUpBtn : public ioButton
{
public:
	enum UpType
	{
		GUT_DISABLE,
		GUT_ENABLE,
		GUT_TIME,
	};

protected:
	UpType m_UpType;

	ioUIRenderImage *m_pArrowImg;
	ioUIRenderImage *m_pArrowRedImg;

	ioUIRenderFrame *m_pRedFrm;
	ioUIRenderFrame *m_pPushRedFrm;

	//
	bool m_bEffect;
	DWORD m_dwCheckTime;
	DWORD m_dwEffectTime;
	DWORD m_dwEffectHide;

public:
	void SetUpBtnType( UpType eType );

protected:
	virtual void OnProcess( float fTimePerSec );

public:
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	
	virtual void AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame );
	virtual void ParseExtraInfo( ioXMLElement &xElement );

protected:
	virtual void OnDrawNormal( int iXPos, int iYPos );
	virtual void OnDrawOvered( int iXPos, int iYPos );
	virtual void OnDrawPushed( int iXPos, int iYPos );
	virtual void OnDrawDisabled( int iXPos, int iYPos );

	void DrawUpTypeFrame( int iXPos, int iYPos, bool bCheckScreen );
	void DrawUpTypePushFrame( int iXPos, int iYPos, bool bCheckScreen );

	void RenderUpArrow( int iXPos, int iYPos, UIRenderType eType);

public:
	GrowthUpBtn();
	virtual ~GrowthUpBtn();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class GrowthSlotBtn : public ioButton
{
public:
	enum
	{
		ID_GROWTH_BTN = 1, ID_AGING_BTN
	};

protected:
	int m_iClassType;

	ioHashString m_Name;
	ioComplexStringPrinter m_GrowthText;

	int m_iItemSlot;		// 스킬 또는 기본의 몇번째 것인지...
	int m_iGrowthLevel;
	int m_iCurLevel;

	int m_iItemReinforce;

	bool m_bSkill;
	bool m_bTimeGrowth;

protected:
	ioUIRenderImage* m_pSmallIcon;

public:
	void SetInfo( int iClassType, const ioHashString &szName, bool bSkill, int iSlot );

	inline bool IsTimeGrowth() const { return m_bTimeGrowth; }

protected:
	void ClearData();

protected:
	//virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();
	virtual void OnRenderAfterChild();

public:
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );
	virtual void ParseExtraInfo( ioXMLElement &xElement );

public:
	GrowthSlotBtn();
	virtual ~GrowthSlotBtn();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class InventorySoldierItemWnd : public ioButton
{
public:
	enum 
	{
		ID_SKILL_SLOT1	= 1,
		ID_SKILL_SLOT2	= 2,
		ID_SKILL_SLOT3	= 3,
		ID_SKILL_SLOT4	= 4,

		ID_CHAR_SLOT1	= 11,
		ID_CHAR_SLOT2	= 12,
		ID_CHAR_SLOT3	= 13,
		ID_CHAR_SLOT4	= 14,

		ID_ICON_BTN		= 101,
	};

protected:
	int m_iClassType;
	int m_iCurPoint;

	DWORD m_dwCurWndID;

	ioUIRenderImage* m_pEmptyIcon;

	ioUIRenderImage* m_pBoxLeft;
	ioUIRenderImage* m_pBoxTop;

	ioUIRenderImage* m_pDarkStrip;
	ioUIRenderImage* m_pLightStrip;

	ioHashStringVec m_CharGrowthText;
	ioHashStringVec m_ItemGrowthText;
	ioHashString m_ItemSkillText[CUR_GROWTH_CNT];

	ioComplexStringPrinter m_ClassInfo[2];
	ioComplexStringPrinter m_PointText;

	int m_CharGrowth[CUR_GROWTH_CNT];
	int m_ItemGrowth[CUR_GROWTH_CNT];
	int m_CharLevel[CUR_GROWTH_CNT];
	int m_ItemLevel[CUR_GROWTH_CNT];

	ioHashString m_CharIconName[CUR_GROWTH_CNT];
	ioHashString m_SkillIconName[CUR_GROWTH_CNT];
	bool m_bHired;

protected:
	ioUIRenderImage *m_pCurGrowthBack;
	ioUIRenderImage *m_pPointImg;

	ioUIRenderFrame *m_pSelectFrm;
	bool             m_bSelect;

	ioUIRenderFrame *m_pBottomFrm;
	ioUIRenderFrame *m_pInActiveFrm;

public:
	void SetInfo( int iClassType );
	void SetSelect(bool bSelect);

	inline int GetClassType() const { return m_iClassType; }

protected:
	void BeforeRenderDesc();
	void RenderDesc();
	void RenderStrip();

	void UpdateItemData();

	void ClearSkillText();
	void ClearGrowthInfo();
	void CheckSelectInfo( const ioHashString& szIcon,
		const ioHashString& szSlotName,
		const ioHashString &szGrowthName,
		int iSlot,
		int iCurLevel,
		int iInfoNum );

protected:
	//virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

public:
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );
	virtual void AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame );

	virtual void ParseExtraInfo( ioXMLElement &xElement );

public:
	bool QuestGuide( int iClassType, float &rfXPos, float &rfYPos );

public:
	InventorySoldierItemWnd();
	virtual ~InventorySoldierItemWnd();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ALCHEMIC_ADDITIVE_DIV	100000

class AlchemicItemWnd : public ioButton
{
protected:
	int m_iCode;
	int m_iCount;
	ioHashString m_IconName;
	bool m_bLockState;

	ioUIRenderImage *m_pEmptyImg;
	ioUIRenderImage *m_pIconBackImg;

	ioUIRenderFrame *m_pEmptyFrm;
	ioUIRenderFrame *m_pPieceFrm;
	ioUIRenderFrame *m_pAdditiveFrm;

	ioUIRenderImage *m_pIconImg;

	ioAlchemicDragItem *m_pDragItem;

	ioComplexStringPrinter m_CountText;

	int m_iCurXGap;
	int m_iCurYGap;

	// new add
	bool m_bNewAdd;
	int m_iNewAddAlpha;
	DWORD m_dwNewAddEffectTime;
	DWORD m_dwNewAddEffectCheckTime;
	ioUIRenderFrame *m_pNewAddFrm;

protected:
	void SetBlank();
	void ClearDesc();
	void RenderDesc( int iXPos, int iYPos );

	bool SetAlchemicItemInfo();

	void SetNewAdd( bool bNew );
	void CheckNewAddEffect();

public:
	void SetInfo( int iCode, int iCount, ioHashString szIconName, bool bAddNew );
	void UpdateInfo();

	void SetLockState( bool bLock );

	inline int GetItemCode() const { return m_iCode; }
	inline int GetItemCount() const { return m_iCount; }
	inline bool IsLockState() const { return m_bLockState; }

	bool IsAdditive();

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

public:
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );
	virtual void AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame );

	virtual void ParseExtraInfo( ioXMLElement &xElement );

public:
	virtual void iwm_mouseleave( const ioMouse& mouse );

public:
	bool QuestGuide( const IntVec &rValue, float &rfXPos, float &rfYPos );

public:
	AlchemicItemWnd();
	virtual ~AlchemicItemWnd();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class MyInventoryWnd : public ioMovingWnd
{
public:
	enum
	{
		ID_CHARGE_GOLD_BTN						= 1,
		ID_SHOP_BTN								= 2,
		ID_EXIT_BTN								= 3,

		ID_SOLDIER_TAB_BTN						= 101,
		ID_EXTRA_TAB_BTN						= 102,
		ID_DECO_TAB_BTN							= 103,
		ID_ALCHEMIC_TAB_BTN						= 104,
		ID_ETC_TAB_BTN							= 105,
		ID_PRESENT_TAB_BTN						= 106,
		ID_COSTUME_TAB_BTN						= 107,
		ID_ACCESSORY_TAB_BTN					= 108,

		ID_SMALL_TAB_1_BTN						= 201, // soldier : all         , extra : all    , deco : all        , etc : all		, present : all		, costume : all
		ID_SMALL_TAB_2_BTN						= 202, // soldier : short range , extra : weapon , deco : kindred    , etc : soldier	, present : ???		, costume : armor
		ID_SMALL_TAB_3_BTN						= 203, // soldier : long range  , extra : armor  , deco : hair       , etc : extra							, costume : helmet
		ID_SMALL_TAB_4_BTN						= 204, // soldier : magic       , extra : helmet , deco : hair color , etc : monster						, costume : cloak
		ID_SMALL_TAB_5_BTN						= 205, // soldier : special     , extra : cloak  , deco : face       , etc : fishing
		ID_SMALL_TAB_6_BTN						= 206, // soldier : package     , extra : box(?) , deco : skin color , etc : guild
		ID_SMALL_TAB_7_BTN						= 207, //                                        , deco : underwear  , etc : other
		ID_SMALL_TAB_8_BTN						= 208, //                                        , deco : package
		ID_SMALL_TAB_9_BTN						= 209,
		ID_SMALL_TAB_10_BTN						= 210,
		ID_HOUSE_ITEM_BTN						= 211,

		ID_ITEM_INFO_WND_1_BTN					= 301,
		ID_ITEM_INFO_WND_2_BTN					= 302,
		ID_ITEM_INFO_WND_3_BTN					= 303,
		ID_ITEM_INFO_WND_4_BTN					= 304,
		ID_ITEM_INFO_WND_5_BTN					= 305,
		ID_ITEM_INFO_WND_6_BTN					= 306,
		ID_ITEM_INFO_WND_7_BTN					= 307,
		ID_ITEM_INFO_WND_8_BTN					= 308,

		ID_CHAR_INFO_WND_1_BTN					= 401,
		ID_CHAR_INFO_WND_2_BTN					= 402,
		ID_CHAR_INFO_WND_3_BTN					= 403,

		ID_INVENTORY_INFO_WND					= 501,
		ID_GROWTH_DOWN_WND						= 502,
		ID_ITEM_COMPOUND_WND					= 503,
		ID_MULTIPLE_COMPOUND_WND				= 504,
		ID_PACKAGE_ITEM_WND						= 505,
		ID_GASHPON_WND							= 506,
		ID_RANDOM_DECO_WND						= 507,
		ID_DECO_SELECT_WND						= 508,
		ID_MULTIPLE_COMPOUND_RESULT				= 509,
		ID_ITEM_COMPOUND_RESULT					= 510,
		ID_EXTRAITEM_BOX_WND					= 511,
		ID_GROWTH_DOWN_RESULT					= 512,
		ID_PACKAGE_ITEM_RESULT					= 513,
		ID_CHANGE_NAME_WND						= 514,
		ID_CHANGE_NAME_RESULT_WND				= 515,
		ID_TRADE_STATE_CHANGE_WND				= 516,
		ID_ITEM_RECVSELL_INFO_WND				= 517,
		ID_RECORD_INIT_WND						= 518,
		ID_RECORD_INIT_RESULT_WND				= 519,
		ID_ITEM_GROWTH_CATALYST_WND				= 520,
		ID_ITEM_GROWTH_CATALYST_RESULT_WND		= 521,
		ID_ITEM_LUCKY_COIN_USE_WND				= 522,
		ID_ITEM_LUCKY_COIN_USE_RESULT_WND		= 523,
		ID_ITEM_COMPOUNDEX_WND					= 524,
		ID_ITEM_COMPOUNDEX_RESULT_WND			= 525,
		ID_ITEM_RAINBOW_MIXER_WND				= 526,
		ID_ITEM_RAINBOW_MIXER_RESULT_WND		= 527,
		ID_ITEM_LOSTSAGA_MIXER_WND				= 528,
		ID_ITEM_LOSTSAGA_MIXER_RESULT_WND		= 529,
		ID_ITEM_GOLD_BOX_WND					= 530,
		ID_SOLDIER_SELECT_BUY_WND				= 531,
		ID_SOLDIER_SELECT_RESULT_WND			= 532,
		ID_FOUR_EXTRA_COMPOUND_BUY_WND			= 533,
		ID_FOUR_EXTRA_COMPOUND_RESULT_WND		= 534,
		ID_EXPAND_MEDAL_SLOT_OPEN_WND			= 535,

		ID_SOLDIER_EXP_BONUS_SELECT_WND			= 536,
		ID_SOLDIER_EXP_BONUS_SELECT_RESULT_WND	= 537,

		ID_EXPAND_MEDAL_SLOT_RESULT_WND			= 538,

		ID_ALCHEMICFUNC_WND						= 539,
		ID_PRESET_PACKAGE_WND					= 540,
		ID_PRESET_PACKAGE_RESULT_WND			= 541,
		ID_HERO_REINFORCE_DWON_WND				= 542,
		ID_HERO_REINFORCE_DONE_WND				= 543,
		ID_ITEM_REINFORCE_DWON_WND				= 544,
		ID_ITEM_REINFORCE_DONE_WND				= 545,
		ID_COSTUME_BOX_WND						= 546,
		ID_NEW_MULTIPLE_COMPOUND_WND			= 547,

		ID_ALCHEMIC_INFO_WND_1_BTN				= 601,
		ID_ALCHEMIC_INFO_WND_30_BTN				= 630,
		ID_GROWTH_ALL_DOWN_WND					= 631,
		ID_GROWTH_ALL_DOWN_RESULT				= 632,
		ID_ITEM_MATERIAL_COMPOUND_WND			= 633,
		ID_ITEM_MATERIAL_COMPOUND_RESULT		= 634,
		ID_SUPER_GASHAPON_WND					= 900,

		ID_VERT_SCROLL							= 1001,
		ID_FLASH_WND							= 2001,

		ID_PET_EGG_RESULT_WND					= 2005,
		ID_NAMED_TITLE_RESULT_WND				= 2006,
		ID_NAMED_TITLE_PREMIUM_RESULT_WND		= 2007,
		ID_NAMED_TITLE_PREMIUM_SELECT_WND		= 2008,

		ID_COSTUME_HELP							= 2101,
		ID_COSTUME_HELP_TIP						= 2102,

		ID_BONUS_CASH_BTN			= 2201,
		ID_BONUS_CASH_TOOLTIP		= 2202,
	};

	enum 
	{
		MAX_TAB_BTN				= 8,
		MAX_SMALL_TAB_BTN		= 10,

		MAX_ITEM_INFO_WND_BTN	= 8,
		PAGE_PER_ITEM_NUM		= 4,

		MAX_CHAR_INFO_WND_BTN	= 3,
		PAGE_PER_CHAR_NUM		= 3,

		MAX_ALCHEMIC_INFO_BTN	= 30,

		ETC_ITEM_OTHER_VALUE     = 10000,
		MEDALITEM_SMALL_TAB_TYPE = 1000
	};

protected:
	ioPlayStage* m_pPlayStage;
	int m_iSelectClassType;

	vSoldierItemInfoList m_vCharInfoList;
	vInventoryItemInfoList m_vInventoryItemInfoList;

	vAlchemicItemInfoList m_vAlchemicItemInfoList;

	ioUIRenderImage    *m_pTabDot;
	ioHashStringVec     m_vSmallTabNameVecArray[MAX_TAB_BTN];
	IntVec              m_vSmallTabTypeVecArray[MAX_TAB_BTN];
	int                 m_iDefaultTypeArray[MAX_TAB_BTN];

	int m_iCurPage;
	DWORD m_dwCurTabID;
	DWORD m_dwCurSmallTabID;
	DWORD m_dwSmallTabEndID;

	DWORD m_dwExtraItemChangeTime;
	DWORD m_dwMedalItemChangeTime;

	static bool m_bGashaponListRequest;

	bool m_bRoomOutTimeCheck;
	bool m_bCharItem;

	int m_iDefaultTabType;

	int m_nCostumeHelpTipIdx;

protected:
	int m_iSelectArray;			// 실제 선택된 아이템 array
	int m_iSellSlotIndex;

	bool m_bMileage;
protected:
	//보너스 캐쉬
	DWORD			m_dwBonusCashTooltipCheckDuration;
	DWORD			m_dwBonusCashTooltipCheckStartTime;

public:
	virtual void iwm_create();
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

	virtual void iwm_vscroll(DWORD dwID, int curPos );
	virtual void iwm_wheel( int zDelta );
	virtual void iwm_gototop();

	virtual void ParseExtraInfo( ioXMLElement &xElement );

	virtual void RenderWnd();

protected:
	virtual void OnRender();
	virtual void OnProcess( float fTimePerSec );
	void ProcessCustomBtnTooltip();
	bool ProcessCustomBtnTooltipExtraItem();
	bool ProcessCustomBtnTooltipPresent();
	bool ProcessCustomBtnTooltipCostume();
	bool ProcessCustomBtnTooltipCostumePresent();
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );

	virtual void OnRenderAfterChild();

protected:
	void UpdatePage( int iNewPage );
	void ChangeSmallTab( DWORD dwID );

	int FindSelectCharArray( int iClassType );

	void SetSoldierItemInfoVec( DWORD dwTabID, DWORD dwSmallTabID );
	void SetDecoItemInfoVec( DWORD dwTabID, DWORD dwSmallTabID );
	void SetEtcItemInfoVec( DWORD dwTabID, DWORD dwSmallTabID );
	void SetExtraItemInfoVec( DWORD dwTabID, DWORD dwSmallTabID );
	void SetPresentItemInfoVec( DWORD dwTabID, DWORD dwSmallTabID );
	void SetCostumeInfoVec( DWORD dwTabID, DWORD dwSmallTabID );
	void SetAlchemicItemInfoVec( DWORD dwTabID, DWORD dwSmallTabID );
	void SetAccessoryInfoVec( DWORD dwTabID, DWORD dwSmallTabID );

	void AddItemInfoVecFromEtcItem( DWORD dwTabID, DWORD dwSmallTabID, int iShopTabType );
	void AddItemInfoVecFromMedalItem( DWORD dwTabID, DWORD dwSmallTabID );

	static bool IsBoughtDeco( int iClassType, int iSexType, int iDecoType, int iDecoCode );
	bool IsEquipDeco( const CHARACTER &rkCharInfo, int iDecoType, int iDecoCode );

	int  GetSmallTabType( DWORD dwTabID, DWORD dwSmallTabID );
	bool IsRightSmallTab( DWORD dwTabID, DWORD dwSmallTabID, int iValue1 );

	void SelectItemBtn( DWORD dwID, DWORD dwParam );
	void SelectCharItemBtn( DWORD dwID, DWORD dwParam );

	//
	void OnItemIconOver( DWORD dwID );
	void OnItemIconPush( DWORD dwID );

	void OnItemEquipPush( DWORD dwID );
	void OnItemReleasePush( DWORD dwID );

	void OnItemDetailPush( DWORD dwID );
	void OnItemReSellPush( DWORD dwID );
	void OnItemCustomInfoPush( DWORD dwID );

	void OnItemDisassemblePush( DWORD dwID );

	//
	bool CheckTimeEndExtraItem( int iSlotIndex );
	bool CheckTimeEndMedalItem( ioUserMedalItem::MEDALITEMSLOT kSlot );
	bool CheckTimeEndCostume( int iSlotIndex );
	bool CheckTimeEndAccessory( int iSlotIndex );

	bool SetEquipExtraItem( DWORD dwID );
	bool SetEquipDecoItem( DWORD dwID );
	bool SetEquipMedalItem( DWORD dwID );
	bool SetEquipCostume( DWORD dwID );
	bool SetEquipAccessory( DWORD dwID );

	bool SetReSellExtraItem( DWORD dwID );
	bool SetReSellCostume( DWORD dwID );
	bool SetReSellMedalItem( DWORD dwID );
	bool SetEtcItemUse( int iType );
	bool SetReSellAccessory( DWORD dwID );

	bool EnableDecoChangeByEquipItem( CHARACTER &rkCharInfo );

	bool SetDisassembleExtraItem( DWORD dwID );
	bool SetDisassembleCostume( DWORD dwID );

	void ShowMainInfoWnd();

public:
	// 각 텝의 첫번째 버튼정보를 오른쪽 창에 전달하는 함수.
	void SetFirstBtnInfo();
	void AddItemInfoVecFromPresent( DWORD dwTabID, int iValue1, int iValue2 );

public:
	DWORD GetDefaultSmallTab( DWORD dwMainTab );

public:
	void ChangeMainTab( DWORD dwID );

	// 선물 아이템에서...
	void CheckDefaultActionBtn();

	// 오른쪽 창에서 특별아이템 사용버튼 눌렀을때
	void SetEtcItemUseBtn( const InventoryItemInfo &rkInfo );

	// 내부 데이터 변화시... 용병 고용/삭제, 장비를 비롯한 아이템들 수량 변화등
	// MainTab 변화없이 SmallTab은 처음것으로..
	void UpdateData();

	// Tab 변화없이 오른쪽 창에서 용병이 바뀌었을때
	void ChangeClassType( int iClassType );

	// SmallTab 변화X, Page유지 여부, 오른쪽 변화 여부
	void UpdateTab( bool bPage, bool bInfo, bool bSelectChange = true );


	// 다른 곳에서 한 번에 이동할때
	void ShowSoldierTabDirect();					// 텝 이동 개념
	void ShowSoldierTabDirect( int iClassType );	// 타겟 용병 이동 개념
	void ShowExtraItemTabDirect( int iClassType, int iIndex = -1 );
	void ShowDecoTabDirect( int iClassType );
	void ShowEtcTabDirect( DWORD dwItemCode=0 );
	bool IsShowPresentTab();
	void ShowPresentTabDirect();
	void ShowAlchemicTabDirect();
	void ShowSubscriptionTabDirect();
	void ShowCostumeTabDirect( int iClassType, int iIndex = -1 );
	void ShowAccessoryTabDirect( int iClassType, int iIndex = -1 );
	//

	bool SetReleaseExtraItem( int iSlotIndex );
	bool SetReleaseMedalItem( int iMedalItemType );
	bool SetReleaseDecoItem( int iDecoType, int iDecoCode );
	bool SetReleaseCostume( int iSlotIndex );
	bool SetReleaseAccessory( int iSlotIndex );

	bool GetCurItemInfo( OUT InventoryItemInfo &rkInfo, IN DWORD dwItemInfoID );
	bool GetCurAlchemicItemInfo( OUT AlchemicToolTipInfo &rkInfo, IN DWORD dwItemInfoID );

	void SetSelectAlchemicClickAction( DWORD dwID );

public:
	void ShowItemCompoundWnd( DWORD dwCode );			// 장비 강화
	void OnItemCompoundResult( bool bSuccess, int iTargetSlot, int iReinforce, DWORD dwCode );

	void ShowItemGrowthCatalystWnd( DWORD dwCode );			// 장비 성장 촉진
	void OnItemGrowthCatalystDelay( int iTargetSlot );
	void OnItemGrowthCatalystResult( bool bSuccess, int iTargetSlot, int iReinforce, int iTargetPeriodType, DWORD dwCode );

	void ShowItemCompoundExWnd( DWORD dwCode );			// 장비 강화
	void OnItemCompoundExResult( bool bSuccess, int iTargetSlot, int iReinforce, DWORD dwCode );

	void ShowItemLuckyCoinWnd( DWORD dwCode );			// 주화 교환
	void OnItemLuckyCoinResult( DWORD dwCode, int iSelectIndex );

	void ShowMultipleCompoundWnd( DWORD dwType );	// 장비 합성
	void OnMultipleCompoundResult(int iEtcType, int iItemCode, int iReinforce );

	void ShowRainbowMixerWnd( DWORD dwType );       // 
	void OnRainbowMixerResult( int iEtcType, int iSelectIndex );

	void ShowLostSagaMixerWnd( DWORD dwType );       // 
	void OnLostSagaMixerResult( int iEtcType, int iSelectIndex );

	void ShowGoldBoxWnd( DWORD dwType );       // 

	void ShowPackageItemWnd( PackageItemBuyWnd::PackageStyle ePackageStyle, DWORD dwType, int eActiveFilter = 1 );	// package
	void OnPackageItemSoldierResult( CHARACTER &rkCharInfo, int iKeepBonusPeso );
	void OnPackageItemDecoResult( CHARACTER &rkCharInfo, int iKeepBonusPeso );
	void ShowSoldierSelectBuyWnd( DWORD dwEtcItemType );
	void OnSoldierSelectResult( int iClassType, int iClassTime, DWORD dwEtcItemType );
	void ShowFourExtraCompoundBuyWnd( DWORD dwEtcItemType );
	void OnForExtraCompoundResult( int iClassType, int iClassTime, DWORD dwEtcItemType, bool bSuccess );

	void ShowPreSetPackageItemWnd( DWORD dwType );
	void OnPreSetPackageItemResult( CHARACTER &rkCharInfo, int iKeepBonusPeso, int iLimitTime );

	void ShowSoldierExpSelectWnd( DWORD dwEtcItemType );
	void OnSoldierExpSelectResult( int iClassType, DWORD dwEtcItemType );

	void ShowDecoSelectWnd( int iPresentIndex, int iPresentSlot );			// 치장 용병 선택
	void ShowDecoSelectWnd( int iIndex, const ioHashString& szSubscriptionID );
	void OnDecoSelectResult();

	void ShowGrowthDownWnd();			// 육성 복구
	void ShowGrowthAllDownWnd();		// 육성 복구
	void OnGrowthDownResult( int iClassType, int iReturnPoint );
	void OnGrowthAllDownResult( int iClassType, int iReturnPoint, __int64 iReturnPeso );

	void ShowGashponWnd( DWORD dwPossessEtcItemType, DWORD dwEtcItemType );
	void ShowGashponWndByNewMultipleCompound( DWORD dwPossessEtcItemType, DWORD dwEtcItemType );
	void OnGashponResultInfo( short iPresentType, int iPresentValue1, int iPresentValue2, int iPresentValue3, int iPresentValue4, bool bAlram, int iPresentPeso );

	void ShowSuperGashponWnd( DWORD dwEtcItemType );

	void ShowRandomDecoWnd( bool bMan );
	void OnRandomDecoResultInfo( short iPresentType, int iPresentValue1, int iPresentValue2, int iPresentValue3, int iPresentValue4, bool bAlram, int iPresentPeso );

	void ShowExtraItemBoxWnd( int iMachineCode );
	void OnExtraItemBoxResultInfo( int iItemCode, int iPeriodTime, int iReinforce, int iTradeType );

	void ShowChangeNameWnd( DWORD dwEtcType );
	void OnChangeNameResultWnd( const ioHashString &szNewName );
	void OnChangeGuildNameResultWnd( const ioHashString &szPreName, const ioHashString &szNewName );
	void OnErrorEdit( const char *szError );

	void OnChangeTradeStateWnd( int iIndex );

	void ShowRecordInitWnd( DWORD dwEtcType );
	void OnRecordInitResultWnd( DWORD dwEtcType );
	
	void ShowItemRecvSellInfoWnd( int iCommandType,
								  int iItemInfoType,
								  const ioHashString szIconName,
								  const ComplexStringPrinterVec& pTitleVec,
								  const ComplexStringPrinterVec& pDescVec,
								  int iClassType,
								  int iReinforce,
								  bool bExtraItemCustom,
								  bool bIsCostume,
								  int nGradeType );

	void ShowExpandMedalSlotOpenWnd( DWORD dwClassType );
	void OnExpandMedalSlotResultWnd( int iClassType, int iSlotNumber );

	void ShowAlchemicFuncWnd();
	void SetAlchemicItemLockStat( int iCode, bool bLock );
	void SetAlchemicItemLockAll();
	void SetAlchemicItemLockAllAdditive();
	void SetAlchemicItemLockClear();
	void CheckAlchemicFuncEnable();
	void OnAlchemicFuncResult( int iResult, SP2Packet &rkPacket );

	bool CheckAlchemicEnableDrag();

	void ShowSubscriptionRetrCheckLimit();
	void ShowSubscriptionRetrCheck( int iIndex, const ioHashString& szSubscriptionID, int iSubscriptionGold, int iRetraGold );

	// 소울 스톤
	void ShowHeroReinforceDownWnd( int iClassType );
	void ShowHeroReinforceDoneWnd( int iClassType );
	ioWnd* ShowItemReinforceDownWnd( int iIndex );
	void ShowItemReinforceDoneWnd( int iIndex );

	void ShowCostumeBoxWnd( int nMachineCode );
	void OnCostumeBoxResultInfo( int nCostumeCode, int nPeriodTime );

	void ShowNewMultipleCompoundWnd( DWORD dwType );	// 장비 합성

public:
	void SetPlayStage( ioPlayStage *pPlayStage );
	ioPlayStage *GetPlayStage() { return m_pPlayStage; }

	bool IsNetworkMode();

	inline int GetSelectClassType() const { return m_iSelectClassType; }
	inline DWORD GetCurTabID() const { return m_dwCurTabID; }

public:
	bool IsGashaponListRequest() const { return m_bGashaponListRequest; }

public:
	void ShowItemMaterialCompoundWnd( DWORD dwCode );			// 장비 강화
	void SetSelectMaterialCode( int dwCode );
	void OnItemMaterialCompoundResult( bool bSuccess, int iTargetSlot, int iReinforce, DWORD dwCode );
	void OnItemMaterialCompoundResultFailExp( int nPreFailExp, int nCurFailExp, int nItemCode );
	void ShowPetEggResultWnd( DWORD dwEtcItemType );
	void OnPetEggResultInfo( int nPetCode, int nPetRank, int nPetSlotIndex );

protected:
	void SendRequestGashaponList();

protected:
	void UpdateHouseButton();

//보너스캐시
protected:
	void CheckBonusCash();

public:
	bool QuestGuide( const IntVec &rValue, float &rfXPos, float &rfYPos );

public:
	void ShowNamedTitleResultWnd( DWORD dwCode, int nLevel, BYTE byPremium );
	void ShowNamedTitlePremiumResultWnd( DWORD dwCode, int nLevel, BYTE byPremium );
	void ShowNamedTitlePremiumSelectWnd( DWORD dwEtcItemCode );
protected:
	int UpdateSmallTabName( ioWnd* pWnd, int iXOffset, const ioHashString& szTitle );

public:
	MyInventoryWnd();
	virtual ~MyInventoryWnd();
};

//-------------------------------------------------------------------------
#define INVEN_TOOLTIP_WIDTHCUT_SIZE 130.0f

class MyInventoryToolTip : public ioWnd
{
public:
	enum
	{
		ID_ICON_BTN      = 1,
		MAX_ACC_DESC	 = 2,
		MAX_DESC         = 3,

		ACC_LINE_GAP	= 22,
	};

public:
	struct MToolTipItemInfo
	{
		ioHashString m_sName;
		ioHashString m_sIcon;
		int m_iLimitLevel;
		int m_iReinForce;
		int m_iManualIndex;
		int m_iCharGrowth[MAX_CHAR_GROWTH];
		int m_iItemGrowth[MAX_ITEM_GROWTH];
		int m_nGradeType;

		void Clear()
		{
			m_sName.Clear();
			m_sIcon.Clear();
			m_iLimitLevel  = 0;
			m_iReinForce   = 0;
			m_iManualIndex = 0;
			for (int i = 0; i < MAX_CHAR_GROWTH ; i++)
				m_iCharGrowth[i] = 0;
			for (int i = 0; i < MAX_ITEM_GROWTH ; i++)
				m_iItemGrowth[i] = 0;
			m_nGradeType = 0;
		}

		MToolTipItemInfo()
		{
			Clear();
		}
	};

protected:
	int m_iMainType;
	int m_iMagicCode;
	int m_iMagicCodeEx;
	int m_iMagicCodeEx2;
	int m_iManualIndex;
	int m_iCharGrowth[MAX_CHAR_GROWTH];
	int m_iItemGrowth[MAX_ITEM_GROWTH];

	ioComplexStringPrinter m_szDescArray[MAX_DESC];

	int m_iMaxAccDesc;
	ioComplexStringPrinter m_szAccDescArray[MAX_ACC_DESC];

	ioUIRenderImage *m_pGrowthLineLeft;
	ioUIRenderImage *m_pGrowthLineCenter;
	ioUIRenderImage *m_pGrowthLineRight;
	ioUIRenderImage *m_pGrowthBackGray;
	ioUIRenderImage *m_pGrowthBackLight;

protected:
	void ClearDesc();
	void RenderDesc( int iXPos, int iYPos );
	void RenderGrowth( int iXPos, int iYPos );
	void RenderAccDesc( int iXPos, int iYPos );
	void PrintManual( int a_iXPos, int a_iYPos , float fScale );
	void ProcessDesc();

	void GetItemInfo( IN const InventoryItemInfo &rkInfo, OUT MToolTipItemInfo &rkReturnInfo );
	void GetExtraItemInfo( IN const InventoryItemInfo &rkInfo, OUT MToolTipItemInfo &rkReturnInfo );
	void GetExtraItemInfoByItemCode( IN int iItemCode , IN int iReinforce, OUT MToolTipItemInfo &rkReturnInfo );
	void GetDecoItemInfo( IN const InventoryItemInfo &rkInfo, OUT MToolTipItemInfo &rkReturnInfo );
	void GetEtcItemInfo( IN const InventoryItemInfo &rkInfo, OUT MToolTipItemInfo &rkReturnInfo );
	void GetMedalItemInfo( IN const InventoryItemInfo &rkInfo, OUT MToolTipItemInfo &rkReturnInfo );
	void GetPresentItemInfo( IN const InventoryItemInfo &rkInfo, OUT MToolTipItemInfo &rkReturnInfo );
	void GetCostumeInfo( IN const InventoryItemInfo &rkInfo, OUT MToolTipItemInfo &rkReturnInfo );
	void GetAccessoryInfo( IN const InventoryItemInfo &rkInfo, OUT MToolTipItemInfo &rkReturnInfo );

	DWORD GetUseInfo( OUT ioHashString &rsInfo );

	void Clear();
	void UpdateWndSize();

public:
	bool SetInfo( InventoryItemInfo &rkInfo );
	bool SetInfoPopup( MToolTipItemInfo &rkInfo );
	void OnProcessPopup();
	void GetPopupItemInfo( int iPresentType, int iMent, int iPresentValue1, int iPresentValue2, OUT MToolTipItemInfo &rkReturnInfo );

public:
	virtual void iwm_show();

public:
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

public:
	MyInventoryToolTip();
	virtual ~MyInventoryToolTip();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ExtraItemEquipConfirmWnd : public ioMovingWnd
{
public:
	enum
	{
		ID_ICON_BTN		= 1,
		ID_EQUIP_BTN	= 2,
		ID_CANCEL_BTN	= 3,
		ID_EXIT_BTN		= 4,
	};

protected:
	ioHashString m_szName;

	int m_iCharArray;
	int m_iEquipSlot;
	int m_iExtraItemIndex;

	int m_iItemCode;
	int m_iPeriodType;

	ioUIRenderImage *m_pExclamationMark;

	ioPlayStage *m_pPlayStage;
	
public:
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

protected:
	virtual void OnRender();
	virtual void OnRenderAfterChild();

	virtual void AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame );
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );

public:
	bool SetExtraItemEquipInfo( int iCharArray, int iSlot, int iSlotIndex );

public:
	void SetPlayStage( ioPlayStage *pPlayStage );
	ioPlayStage *GetPlayStage() { return m_pPlayStage; }

public:
	ExtraItemEquipConfirmWnd();
	virtual ~ExtraItemEquipConfirmWnd();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class AlchemicItemToolTip : public ioWnd
{
public:
	enum
	{
		ATT_ALCHEMIC_ITEM,
		ATT_SOLDIER,
		ATT_ITEM,
	};

	enum
	{
		MIN_WIDTH		= 0,
		MIN_HEIGHT		= 26,
		TEXT_LINE_GAP   = 18,
		SIDE_TEXT_GAP	= 14,

	};

protected:
	int m_iCode;
	int m_iType;

	ioComplexStringPrinter m_szDescArray;

protected:
	void Clear();
	void ProcessPosition();

public:
	bool SetInfo( int iCode, int iType );

public:
	virtual void iwm_show();

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

public:
	AlchemicItemToolTip();
	virtual ~AlchemicItemToolTip();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

