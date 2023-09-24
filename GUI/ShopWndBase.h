//----------------------------------------------------------------------------------
/*
Filename	: C:\LostSagaClient\lostsaga\programming\src\LSClient\GUI\ShopWndBase.h
Author		: UITool
Date		: 114.0.2_10:42:55

*/
//----------------------------------------------------------------------------------

#ifndef _IOSHOPWNDBASE_

#define _IOSHOPWNDBASE_


#include "ioWndEX.h"

#include "ioScrollBarEX.h"
#include "ioTabWndEX.h"


//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
ioShopWndBase
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class ioShopWndBase : public ioWndEX
{
protected:

	enum
	{
		ID_FRM_BACKGROUND = 1,
		ID_SCR_VERTSB = 45,
		ID_TABBTN_ACCESSORY = 20,
		ID_TABBTN_EQUIP = 19,
		ID_TABBTN_EVENTSHOP = 22,
		ID_TABBTN_MERCENARY = 18,
		ID_TABBTN_UNIQUE = 21,
		ID_TABBTN_WEEKLYITEM = 12,
		ID_TAB_ITEM = 11,

	};

public:

	//----------------------------------------------------------------------------------
	/*
	积己 / 家戈
	*/
	ioShopWndBase() : ioWndEX() 
	{
		m_itemtabwnd = 0;
		m_tabaccessoryfilter = 0;
		m_tabequipfilter = 0;
		m_tabeventshopfilter = 0;
		m_tabmecenaryfilter = 0;
		m_tabuniquefilter = 0;
		m_tabweeklyfilter = 0;
		m_vscrollbar = 0;

	}

	virtual ~ioShopWndBase() 
	{
	}

	//----------------------------------------------------------------------------------
	/*
	楷搬 / 楷搬 产扁
	*/
	virtual void	iwm_create()
	{
		ioWndEX::iwm_create();

		m_itemtabwnd = (ioTabWndEX*)(FindChildWndEX(11));
		m_tabaccessoryfilter = (ioTabWndEX*)(FindChildWndEX(63));
		m_tabequipfilter = (ioTabWndEX*)(FindChildWndEX(59));
		m_tabeventshopfilter = (ioTabWndEX*)(FindChildWndEX(71));
		m_tabmecenaryfilter = (ioTabWndEX*)(FindChildWndEX(56));
		m_tabuniquefilter = (ioTabWndEX*)(FindChildWndEX(67));
		m_tabweeklyfilter = (ioTabWndEX*)(FindChildWndEX(49));
		m_vscrollbar = (ioScrollBarEX*)(FindChildWndEX(45));


		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(1) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(14), new ioWndEXMouseEventHandlerEX< ioShopWndBase >( this, &ioShopWndBase::OnMouseWheel ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(11) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(16), new ioWndEXRadioEventHandlerEX< ioShopWndBase >( this, &ioShopWndBase::OnTabItem ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(45) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(17), new ioWndEXScrollBarEventHandlerEX< ioShopWndBase >( this, &ioShopWndBase::OnClickSBDecBtn ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(45) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(18), new ioWndEXScrollBarEventHandlerEX< ioShopWndBase >( this, &ioShopWndBase::OnClickSBIncBtn ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(45) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(19), new ioWndEXScrollBarEventHandlerEX< ioShopWndBase >( this, &ioShopWndBase::OnScrollSB ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(49) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(16), new ioWndEXRadioEventHandlerEX< ioShopWndBase >( this, &ioShopWndBase::OnTabWeeklyFilter ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(56) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(16), new ioWndEXRadioEventHandlerEX< ioShopWndBase >( this, &ioShopWndBase::OnTabMecenaryFilter ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(59) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(16), new ioWndEXRadioEventHandlerEX< ioShopWndBase >( this, &ioShopWndBase::OnTabEquipFilter ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(63) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(16), new ioWndEXRadioEventHandlerEX< ioShopWndBase >( this, &ioShopWndBase::OnTabAccessoryFilter ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(67) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(16), new ioWndEXRadioEventHandlerEX< ioShopWndBase >( this, &ioShopWndBase::OnTabUniqueFilter ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(71) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(16), new ioWndEXRadioEventHandlerEX< ioShopWndBase >( this, &ioShopWndBase::OnTabEventShopFilter ) );
				}
		}

	}

	virtual void	iwm_destroy()
	{
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(1) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(14), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(11) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(16), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(45) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(17), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(45) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(18), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(45) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(19), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(49) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(16), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(56) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(16), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(59) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(16), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(63) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(16), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(67) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(16), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(71) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(16), this );
				}
		}


		ioWndEX::iwm_destroy();
	}

	//----------------------------------------------------------------------------------
	/*
	Event Virtual Functions
	*/
	virtual void OnClickSBDecBtn( ioWndEX* wnd, ioWndEXScrollBarEvent& event ) {}
	virtual void OnClickSBIncBtn( ioWndEX* wnd, ioWndEXScrollBarEvent& event ) {}
	virtual void OnMouseWheel( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnScrollSB( ioWndEX* wnd, ioWndEXScrollBarEvent& event ) {}
	virtual void OnTabAccessoryFilter( ioWndEX* wnd, ioWndEXRadioEvent& event ) {}
	virtual void OnTabEquipFilter( ioWndEX* wnd, ioWndEXRadioEvent& event ) {}
	virtual void OnTabEventShopFilter( ioWndEX* wnd, ioWndEXRadioEvent& event ) {}
	virtual void OnTabItem( ioWndEX* wnd, ioWndEXRadioEvent& event ) {}
	virtual void OnTabMecenaryFilter( ioWndEX* wnd, ioWndEXRadioEvent& event ) {}
	virtual void OnTabUniqueFilter( ioWndEX* wnd, ioWndEXRadioEvent& event ) {}
	virtual void OnTabWeeklyFilter( ioWndEX* wnd, ioWndEXRadioEvent& event ) {}


protected:

	ioScrollBarEX* m_vscrollbar;
	ioTabWndEX* m_itemtabwnd;
	ioTabWndEX* m_tabaccessoryfilter;
	ioTabWndEX* m_tabequipfilter;
	ioTabWndEX* m_tabeventshopfilter;
	ioTabWndEX* m_tabmecenaryfilter;
	ioTabWndEX* m_tabuniquefilter;
	ioTabWndEX* m_tabweeklyfilter;

};


#endif // _IOSHOPWNDBASE_


