//----------------------------------------------------------------------------------
/*
Filename	: MercenaryEquipInfoTabWnd
Author		: sumin
Date		: 113.11.5_14:41:3
*/
//----------------------------------------------------------------------------------

#ifndef _MERCENARYEQUIPINFOTABWND_
#define _MERCENARYEQUIPINFOTABWND_

#include "MercenaryEquipInfoTabWndBase.h"
#include "ShopWndTabController.h"

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
ShopBuyTabCommon
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class ioPaperDollWnd;
class ShopBuyTabCommon
{
public:

	//----------------------------------------------------------------------------------
	/*
	持失 / 社瑚
	*/
	ShopBuyTabCommon();
	virtual ~ShopBuyTabCommon();

	//----------------------------------------------------------------------------------
	/*
	control
	*/
	virtual void			SetItemInfo( const ShopItemInfoEX& info );
	void					SetShopWnd( ShopWnd* wnd ) { m_shopwnd = wnd; }
	void					TurnLeft();
	void					TurnRight();
	
protected:

	//----------------------------------------------------------------------------------
	/*
	ETC
	*/
	void					SetPaperDollWnd( ioWnd* tabbtn, ioWnd* refwnd, int insertid, int classtype );
	void					SetPaperDoll( bool male, int classtype );

protected:

	ShopWnd*				m_shopwnd;

	ShopItemInfoEX			m_iteminfo;

	ioPaperDollWnd*			m_paperdollwnd;

	float					m_yaw;
};


//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
MercenaryEquipInfoTabWnd
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class PaperMercenaryControlWnd;
class MercenaryEquipInfoTabWnd : public MercenaryEquipInfoTabWndBase, public ShopBuyTabCommon
{
public:

	//----------------------------------------------------------------------------------
	/*
	持失 / 社瑚
	*/
	MercenaryEquipInfoTabWnd();
	virtual ~MercenaryEquipInfoTabWnd();

	//----------------------------------------------------------------------------------
	/*
	ioWnd Override
	*/
	virtual void				OnProcess( float fTimePerSec );
	
	//----------------------------------------------------------------------------------
	/*
	ioWnd Event Override
	*/
	virtual void				iwm_create();
	virtual void				iwm_command( ioWnd* wnd, int cmd, DWORD param );
	
	//----------------------------------------------------------------------------------
	/*
	ShopBuyTabCommon override
	*/
	virtual void				SetItemInfo( const ShopItemInfoEX& info );
	
protected:

	PaperMercenaryControlWnd*	m_paperctrlwnd;
};


#endif // _MERCENARYEQUIPINFOTABWND_



