//----------------------------------------------------------------------------------
/*
Filename	: ETCInfoTabWnd
Author		: sumin
Date		: 113.11.5_14:41:3
*/
//----------------------------------------------------------------------------------

#ifndef _ETCINFOTABWND_
#define _ETCINFOTABWND_

#include "ETCInfoTabWndBase.h"
#include "ShopWndTabController.h"
#include "MercenaryEquipInfoTabWnd.h"

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
ETCInfoTabWnd
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class ETCInfoTabWnd : public ETCInfoTabWndBase, public ShopBuyTabCommon
{
public:

	//----------------------------------------------------------------------------------
	/*
	»ý¼º / ¼Ò¸ê
	*/
	ETCInfoTabWnd();
	virtual ~ETCInfoTabWnd();

	//----------------------------------------------------------------------------------
	/*
	ShopBuyTabCommon override
	*/
	virtual void		SetItemInfo( const ShopItemInfoEX& info );
		
protected:
};


#endif // _ETCINFOTABWND_



