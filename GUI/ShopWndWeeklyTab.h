//----------------------------------------------------------------------------------
/*
Filename	: ShopWndWeeklyTab
Author		: sumin
Date		: 113.11.5_14:41:3
*/
//----------------------------------------------------------------------------------

#ifndef _SHOPWNDWEEKLYTAB_
#define _SHOPWNDWEEKLYTAB_

#include "ShopWndWeeklyTabBase.h"
#include "ShopWndTabController.h"

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
ShopWndWeeklyTab
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class ioLabelWndEX;
class ioImageButtonWndEX;
class ShopWndWeeklyTab : public ioShopWndWeeklyTabBase, public ShopWndTabController
{
	enum { 
		ELEMENT_NUM = 10 
	};

public:

	//----------------------------------------------------------------------------------
	/*
	»ý¼º / ¼Ò¸ê
	*/
	ShopWndWeeklyTab();
	virtual ~ShopWndWeeklyTab();

	//----------------------------------------------------------------------------------
	/*
	ioWnd Event Override
	*/
	virtual void			iwm_create();

	//----------------------------------------------------------------------------------
	/*
	ShopWndTabController override
	*/
	virtual void			SubmitElement( int uicol, int uirow, ShopItemInfoEX* elem );
	virtual void			OnLClickShopIcon00( ioWndEX* wnd, ioWndEXMouseEvent& event ) { OnClickShopIcon( wnd, event, 0, 0 ); }
	virtual void			OnLClickShopIcon10( ioWndEX* wnd, ioWndEXMouseEvent& event ) { OnClickShopIcon( wnd, event, 1, 0 ); }
	virtual void			OnLClickShopIcon20( ioWndEX* wnd, ioWndEXMouseEvent& event ) { OnClickShopIcon( wnd, event, 2, 0 ); }
	virtual void			OnLClickShopIcon30( ioWndEX* wnd, ioWndEXMouseEvent& event ) { OnClickShopIcon( wnd, event, 3, 0 ); }
	virtual void			OnLClickShopIcon40( ioWndEX* wnd, ioWndEXMouseEvent& event ) { OnClickShopIcon( wnd, event, 4, 0 ); }
	virtual void			OnLClickShopIcon01( ioWndEX* wnd, ioWndEXMouseEvent& event ) { OnClickShopIcon( wnd, event, 0, 1 ); }
	virtual void			OnLClickShopIcon11( ioWndEX* wnd, ioWndEXMouseEvent& event ) { OnClickShopIcon( wnd, event, 1, 1 ); }
	virtual void			OnLClickShopIcon21( ioWndEX* wnd, ioWndEXMouseEvent& event ) { OnClickShopIcon( wnd, event, 2, 1 ); }
	virtual void			OnLClickShopIcon31( ioWndEX* wnd, ioWndEXMouseEvent& event ) { OnClickShopIcon( wnd, event, 3, 1 ); }
	virtual void			OnLClickShopIcon41( ioWndEX* wnd, ioWndEXMouseEvent& event ) { OnClickShopIcon( wnd, event, 4, 1 ); }

protected:
};


#endif // _SHOPWNDWEEKLYTAB_



