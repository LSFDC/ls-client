//----------------------------------------------------------------------------------
/*
Filename	: ShopBuyWndFrame
Author		: sumin
Date		: 113.11.5_14:41:3
*/
//----------------------------------------------------------------------------------

#ifndef _SHOPBUYWNDFRAME_
#define _SHOPBUYWNDFRAME_

#include "ShopBuyWndFrameBase.h"
#include "ShopWndTabController.h"

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
ShopBuyWndFrame
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class MercenaryEquipInfoTabWnd;
class DecoInfoTabWnd;
class ETCInfoTabWnd;
class ShopWnd;
class ShopBuyWndFrame : public ShopBuyWndFrameBase
{
public:

	//----------------------------------------------------------------------------------
	/*
	»ý¼º / ¼Ò¸ê
	*/
	ShopBuyWndFrame();
	virtual ~ShopBuyWndFrame();

	//----------------------------------------------------------------------------------
	/*
	control
	*/
	void						SetShopWnd( ShopWnd* wnd ) { m_shopwnd = wnd; }
	void						SetItemInfo( const ShopItemInfoEX& info );

	//----------------------------------------------------------------------------------
	/*
	Event Virtual Functions
	*/
	virtual void				OnClickBuy( ioWndEX* wnd, ioWndEXMouseEvent& event );
	virtual void				OnClickClose( ioWndEX* wnd, ioWndEXMouseEvent& event );
	
protected:

	ShopWnd*					m_shopwnd;

	MercenaryEquipInfoTabWnd*	m_meinfotabwnd;
	DecoInfoTabWnd*				m_decoinfotabwnd;
	ETCInfoTabWnd*				m_etcinfotabwnd;

	ShopItemInfoEX				m_iteminfo;
};


#endif // _SHOPBUYWNDFRAME_



