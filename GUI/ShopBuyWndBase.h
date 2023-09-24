//----------------------------------------------------------------------------------
/*
Filename	: C:\LostSagaClient\lostsaga\programming\src\LSClient\GUI\shopbuywndBase.h
Author		: UITool
Date		: 113.11.31_10:0:28

*/
//----------------------------------------------------------------------------------

#ifndef _SHOPBUYWNDBASE_

#define _SHOPBUYWNDBASE_


#include "ioWndEX.h"

#include "ioFrameWndEX.h"
#include "ioImageWndEX.h"
#include "ioLabelWndEX.h"
#include "ioRichLabel.h"
#include "ioWndEX.h"


//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
ShopBuyWndBase
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class ShopBuyWndBase : public ioWndEX
{
protected:

	enum
	{

	};

public:

	//----------------------------------------------------------------------------------
	/*
	积己 / 家戈
	*/
	ShopBuyWndBase() : ioWndEX() 
	{
		m_adddesclabel = 0;
		m_btmwndex = 0;
		m_descrich = 0;
		m_iconimg = 0;
		m_midwndex = 0;
		m_namerich = 0;

	}

	virtual ~ShopBuyWndBase() 
	{
	}

	//----------------------------------------------------------------------------------
	/*
	楷搬 / 楷搬 产扁
	*/
	virtual void	iwm_create()
	{
		ioWndEX::iwm_create();

		m_adddesclabel = (ioLabelWndEX*)(FindChildWndEX(44));
		m_btmwndex = (ioFrameWndEX*)(FindChildWndEX(45));
		m_descrich = (ioRichLabel*)(FindChildWndEX(9));
		m_iconimg = (ioImageWndEX*)(FindChildWndEX(7));
		m_midwndex = (ioWndEX*)(FindChildWndEX(31));
		m_namerich = (ioRichLabel*)(FindChildWndEX(8));


		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(46) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< ShopBuyWndBase >( this, &ShopBuyWndBase::OnClickBuy ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(47) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< ShopBuyWndBase >( this, &ShopBuyWndBase::OnClickPresent ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(5) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< ShopBuyWndBase >( this, &ShopBuyWndBase::OnClickClose ) );
				}
		}

	}

	virtual void	iwm_destroy()
	{
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(46) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(7), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(47) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(7), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(5) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(7), this );
				}
		}


		ioWndEX::iwm_destroy();
	}

	//----------------------------------------------------------------------------------
	/*
	Event Virtual Functions
	*/
	virtual void OnClickBuy( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnClickClose( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnClickPresent( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}


protected:

	ioFrameWndEX* m_btmwndex;
	ioImageWndEX* m_iconimg;
	ioLabelWndEX* m_adddesclabel;
	ioRichLabel* m_descrich;
	ioRichLabel* m_namerich;
	ioWndEX* m_midwndex;

};


#endif // _SHOPBUYWNDBASE_


