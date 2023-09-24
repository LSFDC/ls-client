//----------------------------------------------------------------------------------
/*
Filename	: ShopWndTabController
Author		: sumin
Date		: 113.11.5_14:41:3
*/
//----------------------------------------------------------------------------------

#ifndef _SHOPWNDTABCONTROLLER_
#define _SHOPWNDTABCONTROLLER_

#include "NewShopWnd.h"
#include "ioWndEX.h"

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
ShopItemInfoEX
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class ShopItemInfoEX : public NewShopItemInfo
{
public:

	enum EVENTSHOPITEM_TYPE
	{
		EVENTSHOPITEM_TYPE_NONE,
		EVENTSHOPITEM_TYPE_CLOVER,
		EVENTSHOPITEM_TYPE_SILVER_COIN,
		EVENTSHOPITEM_TYPE_MILEAGE_COIN,
	};

public:

	ShopItemInfoEX() : NewShopItemInfo(), m_eventshoptype( EVENTSHOPITEM_TYPE_NONE ), m_itemcode( -1 ) {}
	virtual ~ShopItemInfoEX() {}
		
	// 가격을 리턴
	const char*				GetPrice();
	// 화폐 단위를 리턴
	const char*				GetPriceUnit();

	//
	bool					IsShowPeso();
	bool					IsEventItem();

public:

	EVENTSHOPITEM_TYPE		m_eventshoptype;

	int						m_itemcode;				// extra : code , deco : code , etc : -1
};
typedef std::vector< ShopItemInfoEX > ShopItemInfoVec;


//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
ShopWndTabController
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class ioScrollBarEX;
class ShopWnd;
class ShopWndTabController
{
	typedef std::vector< std::vector< ShopItemInfoEX* > >		ElementVec;

public:

	//----------------------------------------------------------------------------------
	/*
	생성 / 소멸
	*/
	ShopWndTabController();
	virtual ~ShopWndTabController();

	//----------------------------------------------------------------------------------
	/*
	virtual SubmitElement
	*/
	virtual void			SubmitElement( int uicol, int uirow, ShopItemInfoEX* elem );

	//----------------------------------------------------------------------------------
	/*
	UpdateElement
	*/
	void					UpdateElement();

	//----------------------------------------------------------------------------------
	/*
	Control
	*/
	void					Clear();
	//
	void					SetShopWnd( ShopWnd* wnd ) { m_shopwnd = wnd; }
	//
	void					SetUISize( int col, int row ) { m_uicol = col; m_uirow = row; }
	void					SetElementSize( int col, int row );
	void					SetElement( int col, int row, ShopItemInfoEX* elem );	
	//
	int						GetUICol() const { return m_uicol; }
	int						GetUIRow() const { return m_uirow; }
	int						GetNumCol() const { return static_cast<int>( m_elements.size() ); }
	int						GetNumRow() const;
	ShopItemInfoEX*			Get( int col, int row );
	//
	void					AttachVertSB( ioScrollBarEX* sb ) { m_vscrollbar = sb; }
	void					DetachVertSB() { m_vscrollbar = 0; }
	void					AttachHorzSB( ioScrollBarEX* sb ) { m_hscrollbar = sb; }
	void					DetachHorzSB() { m_hscrollbar = 0; }

protected:

	void					DeleteRow( std::vector< ShopItemInfoEX* >& rows );
	void					OnClickShopIcon( ioWndEX* wnd, ioWndEXMouseEvent& event, int col, int row );
	
protected:

	ShopWnd*				m_shopwnd;

	ElementVec				m_elements;

	ioScrollBarEX*			m_vscrollbar;
	ioScrollBarEX*			m_hscrollbar;

	int						m_uicol;
	int						m_uirow;
};

#endif // _SHOPWNDTABCONTROLLER_



