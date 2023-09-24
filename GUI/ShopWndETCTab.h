//----------------------------------------------------------------------------------
/*
Filename	: ShopWndETCTab
Author		: sumin
Date		: 113.11.5_14:41:3
*/
//----------------------------------------------------------------------------------

#ifndef _SHOPWNDETCTAB_
#define _SHOPWNDETCTAB_

#include "ShopWndETCTabBase.h"
#include "ShopWndTabController.h"

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
ShopWndETCTab
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class ShopWndETCTab : public ioShopWndEtcTabBase, public ShopWndTabController
{
	enum { 
		ELEMENT_NUM = 15
	};

public:

	//----------------------------------------------------------------------------------
	/*
	»ý¼º / ¼Ò¸ê
	*/
	ShopWndETCTab();
	virtual ~ShopWndETCTab();

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
	virtual void			OnLClickShopIcon02( ioWndEX* wnd, ioWndEXMouseEvent& event ) { OnClickShopIcon( wnd, event, 0, 2 ); }
	virtual void			OnLClickShopIcon12( ioWndEX* wnd, ioWndEXMouseEvent& event ) { OnClickShopIcon( wnd, event, 1, 2 ); }
	virtual void			OnLClickShopIcon22( ioWndEX* wnd, ioWndEXMouseEvent& event ) { OnClickShopIcon( wnd, event, 2, 2 ); }
	virtual void			OnLClickShopIcon32( ioWndEX* wnd, ioWndEXMouseEvent& event ) { OnClickShopIcon( wnd, event, 3, 2 ); }
	virtual void			OnLClickShopIcon42( ioWndEX* wnd, ioWndEXMouseEvent& event ) { OnClickShopIcon( wnd, event, 4, 2 ); }

protected:
};


#endif // _SHOPWNDETCTAB_



