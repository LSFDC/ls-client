//----------------------------------------------------------------------------------
/*
Filename	: C:\LostSagaClient\lostsaga\programming\src\LSClient\GUI\ShopBuyWndFrameBase.h
Author		: UITool
Date		: 113.11.24_10:26:1

*/
//----------------------------------------------------------------------------------

#ifndef _SHOPBUYWNDFRAMEBASE_

#define _SHOPBUYWNDFRAMEBASE_


#include "ioWndEX.h"

#include "ioFrameWndEX.h"


//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
ShopBuyWndFrameBase
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class ShopBuyWndFrameBase : public ioWndEX
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
	ShopBuyWndFrameBase() : ioWndEX() 
	{
		m_backframe = 0;

	}

	virtual ~ShopBuyWndFrameBase() 
	{
	}

	//----------------------------------------------------------------------------------
	/*
	楷搬 / 楷搬 产扁
	*/
	virtual void	iwm_create()
	{
		ioWndEX::iwm_create();

		m_backframe = (ioFrameWndEX*)(FindChildWndEX(0));


		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(6) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< ShopBuyWndFrameBase >( this, &ShopBuyWndFrameBase::OnClickBuy ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(8) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< ShopBuyWndFrameBase >( this, &ShopBuyWndFrameBase::OnClickClose ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(9) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< ShopBuyWndFrameBase >( this, &ShopBuyWndFrameBase::OnClickClose ) );
				}
		}

	}

	virtual void	iwm_destroy()
	{
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(6) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(7), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(8) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(7), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(9) );
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


protected:

	ioFrameWndEX* m_backframe;

};


#endif // _SHOPBUYWNDFRAMEBASE_


