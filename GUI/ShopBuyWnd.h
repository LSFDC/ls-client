//----------------------------------------------------------------------------------
/*
Filename	: ShopBuyWnd
Author		: sumin
Date		: 113.11.5_14:41:3
*/
//----------------------------------------------------------------------------------

#ifndef _SHOPBUYWND_
#define _SHOPBUYWND_

#include "ShopBuyWndBase.h"
#include "ShopWndTabController.h"

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
ShopBuyWnd
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class ShopBuyWnd : public ShopBuyWndBase
{
public:

	//----------------------------------------------------------------------------------
	/*
	»ý¼º / ¼Ò¸ê
	*/
	ShopBuyWnd();
	virtual ~ShopBuyWnd();

	//----------------------------------------------------------------------------------
	/*
	control
	*/
	void				SetItemInfo( const ShopItemInfoEX& info );

	//----------------------------------------------------------------------------------
	/*
	Event Virtual Functions
	*/
	virtual void		OnClickBuy( ioWndEX* wnd, ioWndEXMouseEvent& event );
	virtual void		OnClickClose( ioWndEX* wnd, ioWndEXMouseEvent& event );
	virtual void		OnClickPresent( ioWndEX* wnd, ioWndEXMouseEvent& event );
	
protected:
	
	ShopItemInfoEX		m_iteminfo;
};


#endif // _SHOPBUYWND_



