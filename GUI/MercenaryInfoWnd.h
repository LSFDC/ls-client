//----------------------------------------------------------------------------------
/*
Filename	: C:\LostSagaClient\lostsaga\programming\src\LSClient\GUI\MercenaryInfoWndBase.h
Author		: UITool
Date		: 113.11.5_14:41:3

*/
//----------------------------------------------------------------------------------

#ifndef _MERCENARYINFOWND_

#define _MERCENARYINFOWND_


#include "MercenaryInfoWndBase.h"


//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
MercenaryInfoWnd
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class MercenaryInfoWnd : public MercenaryInfoWndBase
{
public:

	//----------------------------------------------------------------------------------
	/*
	»ý¼º / ¼Ò¸ê
	*/
	MercenaryInfoWnd();
	virtual ~MercenaryInfoWnd();

	//----------------------------------------------------------------------------------
	/*
	ioWnd Event Override
	*/
	virtual void	iwm_show();

	//----------------------------------------------------------------------------------
	/*
	Event Virtual Functions
	*/
	virtual void	OnClickBuy( ioWndEX* wnd, ioWndEXMouseEvent& event );
	virtual void	OnClickClose( ioWndEX* wnd, ioWndEXMouseEvent& event );


protected:
};


#endif // _MERCENARYINFOWND_


