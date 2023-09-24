#pragma once

#include "../io3DEngine/ioMovingWnd.h"

#include "FSMStateMgr.h"
#include "..\ioSetItemInfo.h"

#define ARROW_SCALE 0.67f
#define ICON_SCALE 0.65f
#define NEW_SCALE 0.75f

typedef struct tagShopItemInfo
{
	int  m_iSetItemIdx;
	bool m_bCanBuy;
	int  m_iPrice;
	int  m_iGradeLv;
	int  m_iBattleLv;
	int  m_iTrainingLv;
	int  m_iAwardLv;
	int  m_iMedalLv;
	int  m_iPriorityOrder; // 낮을 수록 순위가 높다.
	DWORD  m_dwSetCode;
	int  m_iShopMarkType; 

	tagShopItemInfo()
	{
		m_iSetItemIdx  = 0;
		m_bCanBuy      = false;
		m_iPrice       = 0;
		m_iGradeLv     = 0;
		m_iBattleLv    = 0;
		m_iTrainingLv  = 0;
		m_iAwardLv     = 0;
		m_iMedalLv     = 0;
		m_iPriorityOrder = 999999999; // ORDER_DEFAULT;
		m_dwSetCode      = 0;
		m_iShopMarkType  = 0; //SHOP_MARK_TYPE_NONE;
	}

}ShopItemInfo;
typedef std::vector<ShopItemInfo> vShopItemInfoList;
//-------------------------------------------------------------------------
class ShopItemInfoSort : public std::binary_function< const ShopItemInfo& , const ShopItemInfo&, bool >
{
public:
	bool operator() ( const ShopItemInfo &lhs, const ShopItemInfo &rhs ) const
	{
		if( lhs.m_iShopMarkType != 0 /*SHOP_MARK_TYPE_NONE*/ || rhs.m_iShopMarkType != 0 /*SHOP_MARK_TYPE_NONE*/ )
		{
			if( lhs.m_iPriorityOrder < rhs.m_iPriorityOrder )
				return true;
		}
		else if( lhs.m_bCanBuy && rhs.m_bCanBuy )
		{
			if( lhs.m_iPrice < rhs.m_iPrice )
				return true;
			if( lhs.m_iPrice == rhs.m_iPrice )
			{
				if( lhs.m_iSetItemIdx < rhs.m_iSetItemIdx )
					return true;
			}
		}
		else if( !lhs.m_bCanBuy && !rhs.m_bCanBuy )
		{
			if( lhs.m_iPriorityOrder < rhs.m_iPriorityOrder )
				return true;
			if( lhs.m_iPriorityOrder == rhs.m_iPriorityOrder )
			{
				if( lhs.m_iGradeLv < rhs.m_iGradeLv )
					return true;
				if( lhs.m_iGradeLv == rhs.m_iGradeLv )
				{
					if( lhs.m_iBattleLv < rhs.m_iBattleLv )
						return true;
					if( lhs.m_iBattleLv == rhs.m_iBattleLv )
					{
						if( lhs.m_iTrainingLv < rhs.m_iTrainingLv )
							return true;
						if( lhs.m_iTrainingLv == rhs.m_iTrainingLv )
						{
							if( lhs.m_iAwardLv < rhs.m_iAwardLv )
								return true;
							if( lhs.m_iAwardLv == rhs.m_iAwardLv )
							{
								if( lhs.m_iMedalLv < rhs.m_iMedalLv )
									return true;
								if( lhs.m_iMedalLv == rhs.m_iMedalLv )
								{
									if( lhs.m_iPrice < rhs.m_iPrice )
										return true;
									if( lhs.m_iPrice == rhs.m_iPrice )
									{
										if( lhs.m_iSetItemIdx < rhs.m_iSetItemIdx )
											return true;
									}
								}
							}
						}
					}
				}
			}
		}
		else if( lhs.m_bCanBuy && !rhs.m_bCanBuy )
			return true;

		return false;
	}
};
//-------------------------------------------------------------------------
class SoldierExerciseBtn : public ioButton
{
public:
	enum SoldierBoughtType
	{
		SBT_NONE             = 0,
		SBT_BOUGHT           = 1,
		SBT_EXERCISE_GENERAL = 2,
		SBT_EXERCISE_PCROOM  = 3,
		SBT_EXERCISE_EVENT   = 4,
	};
protected:
	enum AniType
	{
		AT_NONE            = 0,
		AT_START_DOWN      = 1,
		AT_START_UP        = 2,
		AT_START_STOP      = 3,
		AT_WHITE_OUT       = 4,
		AT_DOWN            = 5,
		AT_UP              = 6, 
		AT_MOUSE_OVER      = 7,
		AT_MOUSE_LEAVE     = 8,
	};
protected:
	bool              m_bForceOver;
	SoldierBoughtType m_eBoughtType;
	int               m_iClassType;              
	int               m_iNeedLevel;
	ioUIRenderImage  *m_pLock;

	bool             m_bArrow;
	int              m_iDownHeight;
	ioUIRenderImage *m_pArrow;
	FSMStateMgr      m_AniState;

	bool  m_bScreenAction;
	bool  m_bScreenSoldier;
	DWORD m_dwScreenTime;
	DWORD m_dwScreenStartTime;

	ioSetItemInfo::NeedLevelType m_eNeedLevelType;
	ioUIRenderImage *m_pSoldierIcon;
	ioUIRenderImage *m_pLockNum;
	ioUIRenderImage *m_pLockBattleMark;
	ioUIRenderImage *m_pLockMedalMark;
	ioUIRenderImage *m_pLockAwardMark;
	ioUIRenderImage *m_pBoughtType;
	ioUIRenderImage *m_pExerciseGeneral;
	ioUIRenderImage *m_pExercisePcroom;
	ioUIRenderImage *m_pExerciseEvent;
	ioUIRenderImage *m_pNewMark;
	ioUIRenderImage *m_pNewEffect;
	ioUIRenderImage *m_pHitMark;
	ioUIRenderImage *m_pHitEffect;
	ioUIRenderImage *m_pSaleMark;
	ioUIRenderImage *m_pSaleEffect;
	ioUIRenderImage *m_pEventMark;
	ioUIRenderImage *m_pEventEffect;
	ioUIRenderImage *m_pIconBack;

	DWORD m_dwNewEffectWaitTime;
	DWORD m_dwNewEffectCurTime;
	int   m_iMarkType; // SHOP_MARK_TYPE_NONE = 0, SHOP_MARK_TYPE_SALE = 1, SHOP_MARK_TYPE_NEW = 2, SHOP_MARK_TYPE_HIT = 3, SHOP_MARK_TYPE_EVENT = 4,
	bool  m_bMarkEffect;

public:
	virtual void iwm_create();
	virtual void iwm_mouseleave( const ioMouse& mouse );
	virtual void iwm_mouseover( const ioMouse& mouse );
	virtual void ParseExtraInfo( ioXMLElement &xElement );

	void SetSoldierIcon( ioUIRenderImage *pSoldierIcon, float fIconScale, bool bScreenAction );
	void SetForceOver( bool bOver ) { m_bForceOver = bOver; }
	void SetBoughtType( SoldierBoughtType eBoughtType );
	int  GetClassType() const { return m_iClassType; }
	void SetClassType(int iClassType) { m_iClassType = iClassType; }
	void SetNeedTypeLv( ioSetItemInfo::NeedLevelType eType, int iNeedLv );
	void SetMarkType( int iMarkType ) { m_iMarkType = iMarkType; }

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	
	virtual void OnDrawNormal( int iXPos, int iYPos );
	virtual void RenderImageList( int iXPos, int iYPos, UIRenderType eRenderType );
	virtual void OnRender();	

	void RenderSoldierIcon( int iXPos, int iYPos );
	void RenderType( int iXPos, int iYPos );
	void RenderNewMark( int iXPos, int iYPos );
	void RenderNeedLv( int iXPos, int iYPos );

	void InitAniState();
	void UpdateAniState( );
	void RenderArrow( int iXPos, int iYPos);	

	void ProcessNewEffect();

public:
	SoldierExerciseBtn();
	virtual ~SoldierExerciseBtn();
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define BIGARROW_SCALE 0.85f

class ioPlayStage;
class SoldierExerciseWnd : public ioWnd
{
public:
	enum
	{
		ID_EXERCISE_CHAR_BUY_BTN1  = 0,
		ID_EXERCISE_CHAR_BUY_BTN2  = 1,
		ID_EXERCISE_CHAR_BUY_BTN3  = 2,
		ID_EXERCISE_CHAR_BUY_BTN4  = 3,
		ID_EXERCISE_CHAR_BUY_BTN5  = 4,
		ID_EXERCISE_CHAR_BUY_BTN6  = 5,
		ID_EXERCISE_CHAR_BUY_BTN7  = 6,
		ID_EXERCISE_CHAR_BUY_BTN8  = 7,
		ID_EXERCISE_CHAR_BUY_BTN9  = 8,
		ID_EXERCISE_CHAR_BUY_BTN10 = 9,

		ID_UP_BTN   = 20000,
		ID_DOWN_BTN = 20001,

		MAX_BUY_BTN = 10,
	};

	enum AniType 
	{
		AT_NONE          = 0,
		AT_FRONT         = 1,
		AT_BACK          = 2,
	};

	enum BigArrowAniType
	{
		BAT_DOWN            = 0,
		BAT_UP              = 1, 
	};
protected:
	ioPlayStage *m_pPlayStage;
	
	int m_iExerciseStyle;
	int m_iCurPage;
	int m_iMaxPage;
	FSMStateMgr m_AniState;
	FSMStateMgr m_BigArrowAniState;
	ioUIRenderImage *m_pArrow;
	ioUIRenderImage *m_pBigArrow;
	vShopItemInfoList m_vItemInfoList;
	bool             m_bArrow;
	int              m_iDownHeight;

	void _UpdateChildPos(int a_iArray ,int iWndID);
	bool IsCanExerciseChar( bool bSelectWait );

	void SetOwnerChangeWaitState();
	void SendChangeWait( int iArray );
	void SetBlankIcon( int a_iWndID );

	void UpdatePage( bool bPlus );
	void SetMaxPage();

	void InitAniState();
	void InitBigArrowAniState();
	bool ChangeBoughtChar( int iArray, bool bWait );

	void SortItemInfo();
	int  GetSetIdx( int iArray );
	bool IsCanBuyLevel( const SoldierExerciseBtn *pItemBtn );
	void NoticeNeedLevel( const ioSetItemInfo *pInfo );

	void SetExerciseStyle();

	void RenderBigArrow( int iXPos, int iYPos );

public:
	void SetPlayStage( ioPlayStage *pPlayStage );
	void SetBoughtType( SoldierExerciseBtn::SoldierBoughtType eBoughtType , int iCharArray );
	void UpdateChildPos();

protected:
	virtual void OnRender();
	virtual void OnProcess( float fTimePerSec );
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );

public:	
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );
	virtual void iwm_create();
	virtual void iwm_show();	
	virtual void ParseExtraInfo( ioXMLElement &xElement );

public:
	SoldierExerciseWnd();
	virtual ~SoldierExerciseWnd();
};


