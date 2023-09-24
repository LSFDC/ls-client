//----------------------------------------------------------------------------------
/*
Filename	: ShopWnd
Author		: sumin
Date		: 113.11.5_14:41:3
*/
//----------------------------------------------------------------------------------

#ifndef _SHOPWND_
#define _SHOPWND_

#include "ShopWndBase.h"
#include "ShopWndTabController.h"

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
ShopWnd
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class ioTabWndEX;
class ioEtcItem;
class ioScrollBarEX;
class ioSetItemInfo;
class RandomMachineInfo;
class CharacterControlWnd;
class ShopWnd : public ioShopWndBase
{
	enum TAB_TYPE
	{
		TT_WEEKLYITEM,
		TT_MERCENARY,
		TT_EQUIP,
		TT_ACCESSORY,
		TT_UNIQUE,
		TT_EVENTSHOP,

		TT_MAX,
	};

public:

	//----------------------------------------------------------------------------------
	/*
	각 Tab의 Filter들
	*/
	enum TAB_FILTER_WEEKLY
	{
		TFW_WEEKLY,
		TFW_NEW,
		TFW_POP,
		TFW_UNLOCK_GRADE
	};

	enum TAB_FILTER_MECENARY
	{
		TFM_MECENARY,
		TFM_PACKAGE
	};

	enum TAB_FILTER_EQUIP
	{
		TFE_EQUIP,
		TFE_SUPPLY,
		TFE_FUNCTION
	};

	enum TAB_FILTER_ACCESSORY
	{
		TFE_ACCESSORY,
		TFE_MOTION,
		TFE_PACKAGE
	};

	enum TAB_FILTER_UNIQUE
	{
		TFE_UNIQUE,
		TFE_BLOCK,
		TFE_DUNGEON
	};

	enum TAB_FILTER_EVENTSHOP
	{
		TFE_CLOVER,
		TFE_EVENT,
		TFE_MILEAGE,
	};

public:

	//----------------------------------------------------------------------------------
	/*
	생성 / 소멸
	*/
	ShopWnd();
	virtual ~ShopWnd();

	//----------------------------------------------------------------------------------
	/*
	ioWnd Event Override
	*/
	virtual void			iwm_create();
	virtual void			iwm_show();

	//----------------------------------------------------------------------------------
	/*
	Event Virtual Functions
	*/
	virtual void			OnMouseWheel( ioWndEX* wnd, ioWndEXMouseEvent& event );
	virtual void			OnClickSBDecBtn( ioWndEX* wnd, ioWndEXScrollBarEvent& event );
	virtual void			OnClickSBIncBtn( ioWndEX* wnd, ioWndEXScrollBarEvent& event );
	virtual void			OnScrollSB( ioWndEX* wnd, ioWndEXScrollBarEvent& event );
	virtual void			OnTabItem( ioWndEX* wnd, ioWndEXRadioEvent& event );
	// filter tab	
	virtual void			OnTabWeeklyFilter( ioWndEX* wnd, ioWndEXRadioEvent& event );
	virtual void			OnTabMecenaryFilter( ioWndEX* wnd, ioWndEXRadioEvent& event );
	virtual void			OnTabEquipFilter( ioWndEX* wnd, ioWndEXRadioEvent& event );
	virtual void			OnTabAccessoryFilter( ioWndEX* wnd, ioWndEXRadioEvent& event );	
	virtual void			OnTabUniqueFilter( ioWndEX* wnd, ioWndEXRadioEvent& event );	
	virtual void			OnTabEventShopFilter( ioWndEX* wnd, ioWndEXRadioEvent& event );

	//----------------------------------------------------------------------------------
	/*
	Get
	*/
	CharacterControlWnd*	GetCharControlWnd() { return m_charinfownd; }
	int						GetCurClassType() const;
	int						GetCurSexType() const;

protected:

	//----------------------------------------------------------------------------------
	/*
	ETC
	*/
	void					SetupController( TAB_TYPE type, int elementcol, ShopItemInfoVec& infos, int uicol, int uirow );
	ShopWndTabController*	GetShopWndTabController( TAB_TYPE type ) { return (ShopWndTabController*)( m_tabwnds[ type ] ); }
	void					AddTabWnd( DWORD parentid, TAB_TYPE type, const char* filename, int posx, int posy );
	void					UpdateController( TAB_TYPE type );
	void					OnTabWeeklyItem();
	void					OnTabMercenary();
	void					OnTabEquip();
	void					OnTabAccessory();
	void					OnTabUnique();
	void					OnTabEventShop();
	void					ShowSortComboBox( bool show );
	// 상점 정보 얻어오는 함수들
	bool					IsMecenaryEvent() const;
	bool					IsEnableSubscription() const;
	void					GetShopMecenaryInfo( ShopItemInfoVec& out, bool filteringweekly );
	void					GetShopEquipInfo( ShopItemInfoVec& out, bool filteringweekly );
	void					GetShopAccessoryInfo( ShopItemInfoVec& out, int decoclasstype, int sextype, bool filteringweekly );
	void					GetShopUniqueInfo( ShopItemInfoVec& out, bool filteringweekly, int tabtype );
	void					GetShopEventShopInfo( ShopItemInfoVec& out, DWORD dwNeedEtcItemIdx );
	
protected:
		
	CharacterControlWnd*	m_charinfownd;

	ioWnd*					m_tabwnds[ TT_MAX ];

	TAB_TYPE				m_curtab;
};


#endif // _SHOPWND_



