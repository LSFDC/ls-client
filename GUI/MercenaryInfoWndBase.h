//----------------------------------------------------------------------------------
/*
Filename	: C:\LostSagaClient\lostsaga\programming\src\LSClient\GUI\MercenaryInfoWndBase.h
Author		: UITool
Date		: 113.11.24_10:7:36

*/
//----------------------------------------------------------------------------------

#ifndef _MERCENARYINFOWNDBASE_

#define _MERCENARYINFOWNDBASE_


#include "ioWndEX.h"



//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
MercenaryInfoWndBase
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class MercenaryInfoWndBase : public ioWndEX
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
	MercenaryInfoWndBase() : ioWndEX() 
	{

	}

	virtual ~MercenaryInfoWndBase() 
	{
	}

	//----------------------------------------------------------------------------------
	/*
	楷搬 / 楷搬 产扁
	*/
	virtual void	iwm_create()
	{
		ioWndEX::iwm_create();



		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(6) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< MercenaryInfoWndBase >( this, &MercenaryInfoWndBase::OnClickBuy ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(8) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< MercenaryInfoWndBase >( this, &MercenaryInfoWndBase::OnClickClose ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(9) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< MercenaryInfoWndBase >( this, &MercenaryInfoWndBase::OnClickClose ) );
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


};


#endif // _MERCENARYINFOWNDBASE_


