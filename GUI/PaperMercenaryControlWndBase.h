//----------------------------------------------------------------------------------
/*
Filename	: C:\LostSagaClient\lostsaga\programming\src\LSClient\GUI\PaperMercenaryControlWndBase.h
Author		: UITool
Date		: 114.0.3_9:23:7

*/
//----------------------------------------------------------------------------------

#ifndef _PAPERMERCENARYCONTROLWNDBASE_

#define _PAPERMERCENARYCONTROLWNDBASE_


#include "ioWndEX.h"

#include "ioButtonWndEX.h"
#include "ioFrameWndEX.h"


//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
PaperMercenaryControlWndBase
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class PaperMercenaryControlWndBase : public ioWndEX
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
	PaperMercenaryControlWndBase() : ioWndEX() 
	{
		m_backfrm = 0;
		m_changeImgBtn = 0;
		m_turnLeftImgBtn = 0;
		m_turnRightImgBtn = 0;

	}

	virtual ~PaperMercenaryControlWndBase() 
	{
	}

	//----------------------------------------------------------------------------------
	/*
	楷搬 / 楷搬 产扁
	*/
	virtual void	iwm_create()
	{
		ioWndEX::iwm_create();

		m_backfrm = (ioFrameWndEX*)(FindChildWndEX(4));
		m_changeImgBtn = (ioImageButtonWndEX*)(FindChildWndEX(5));
		m_turnLeftImgBtn = (ioImageButtonWndEX*)(FindChildWndEX(6));
		m_turnRightImgBtn = (ioImageButtonWndEX*)(FindChildWndEX(7));


		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(5) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< PaperMercenaryControlWndBase >( this, &PaperMercenaryControlWndBase::OnClickChange ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(6) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(6), new ioWndEXMouseEventHandlerEX< PaperMercenaryControlWndBase >( this, &PaperMercenaryControlWndBase::OnTurnLeft ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(7) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(6), new ioWndEXMouseEventHandlerEX< PaperMercenaryControlWndBase >( this, &PaperMercenaryControlWndBase::OnTurnRight ) );
				}
		}

	}

	virtual void	iwm_destroy()
	{
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(5) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(7), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(6) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(6), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(7) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(6), this );
				}
		}


		ioWndEX::iwm_destroy();
	}

	//----------------------------------------------------------------------------------
	/*
	Event Virtual Functions
	*/
	virtual void OnClickChange( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnTurnLeft( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnTurnRight( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}


protected:

	ioFrameWndEX* m_backfrm;
	ioImageButtonWndEX* m_changeImgBtn;
	ioImageButtonWndEX* m_turnLeftImgBtn;
	ioImageButtonWndEX* m_turnRightImgBtn;

};


#endif // _PAPERMERCENARYCONTROLWNDBASE_


