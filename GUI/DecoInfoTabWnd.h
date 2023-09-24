//----------------------------------------------------------------------------------
/*
Filename	: DecoInfoTabWnd
Author		: sumin
Date		: 113.11.5_14:41:3
*/
//----------------------------------------------------------------------------------

#ifndef _DECOINFOTABWND_
#define _DECOINFOTABWND_

#include "DecoInfoTabWndBase.h"
#include "ShopWndTabController.h"
#include "MercenaryEquipInfoTabWnd.h"

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
DecoInfoTabWnd
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class PaperMercenaryControlWnd;
class DecoInfoTabWnd : public DecoInfoTabWndBase, public ShopBuyTabCommon
{
public:

	//----------------------------------------------------------------------------------
	/*
	»ý¼º / ¼Ò¸ê
	*/
	DecoInfoTabWnd();
	virtual ~DecoInfoTabWnd();

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

	//----------------------------------------------------------------------------------
	/*
	control
	*/
	void						SetShopMode( bool b ) { m_isshop = b; }
	void						SetDeco();

	//----------------------------------------------------------------------------------
	/*
	static
	*/
	static void					GetDecoClassTypesFromMyInfo( std::vector< int >& out );
	static int					GetDecoClassTypesFromMyInfo();
	static int					GetDecoSexTypeFromMyInfo();
		
protected:

	bool						m_isshop;

	PaperMercenaryControlWnd*	m_paperctrlwnd;
};


#endif // _DECOINFOTABWND_



