//----------------------------------------------------------------------------------
/*
Filename	: C:\LostSagaClient\lostsaga\programming\src\LSClient\GUI\DecoInfoTabWndBase.h
Author		: UITool
Date		: 113.11.26_10:40:39

*/
//----------------------------------------------------------------------------------

#ifndef _DECOINFOTABWNDBASE_

#define _DECOINFOTABWNDBASE_


#include "ioWndEX.h"

#include "ioImageWndEX.h"
#include "ioLabelWndEX.h"
#include "ioTabWndEX.h"


//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
DecoInfoTabWndBase
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class DecoInfoTabWndBase : public ioWndEX
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
	DecoInfoTabWndBase() : ioWndEX() 
	{
		m_backimg = 0;
		m_infotabbutton = 0;
		m_lefttoplabel = 0;
		m_righttoplabel = 0;

	}

	virtual ~DecoInfoTabWndBase() 
	{
	}

	//----------------------------------------------------------------------------------
	/*
	楷搬 / 楷搬 产扁
	*/
	virtual void	iwm_create()
	{
		ioWndEX::iwm_create();

		m_backimg = (ioImageWndEX*)(FindChildWndEX(38));
		m_infotabbutton = (ioTabFrameButtonEX*)(FindChildWndEX(37));
		m_lefttoplabel = (ioLabelWndEX*)(FindChildWndEX(40));
		m_righttoplabel = (ioLabelWndEX*)(FindChildWndEX(41));



	}

	virtual void	iwm_destroy()
	{


		ioWndEX::iwm_destroy();
	}

	//----------------------------------------------------------------------------------
	/*
	Event Virtual Functions
	*/


protected:

	ioImageWndEX* m_backimg;
	ioLabelWndEX* m_lefttoplabel;
	ioLabelWndEX* m_righttoplabel;
	ioTabFrameButtonEX* m_infotabbutton;

};


#endif // _DECOINFOTABWNDBASE_


