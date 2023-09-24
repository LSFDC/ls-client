//----------------------------------------------------------------------------------
/*
Filename	: C:\LostSagaClient\lostsaga\programming\src\LSClient\GUI\ETCInfoTabWndBase.h
Author		: UITool
Date		: 113.11.26_11:37:24

*/
//----------------------------------------------------------------------------------

#ifndef _ETCINFOTABWNDBASE_

#define _ETCINFOTABWNDBASE_


#include "ioWndEX.h"

#include "ioImageWndEX.h"
#include "ioLabelWndEX.h"
#include "ioRichLabel.h"
#include "ioTabWndEX.h"


//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
ETCInfoTabWndBase
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class ETCInfoTabWndBase : public ioWndEX
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
	ETCInfoTabWndBase() : ioWndEX() 
	{
		m_descrich = 0;
		m_iconimg = 0;
		m_infotabbutton = 0;
		m_lefttoplabel = 0;

	}

	virtual ~ETCInfoTabWndBase() 
	{
	}

	//----------------------------------------------------------------------------------
	/*
	楷搬 / 楷搬 产扁
	*/
	virtual void	iwm_create()
	{
		ioWndEX::iwm_create();

		m_descrich = (ioRichLabel*)(FindChildWndEX(46));
		m_iconimg = (ioImageWndEX*)(FindChildWndEX(47));
		m_infotabbutton = (ioTabFrameButtonEX*)(FindChildWndEX(37));
		m_lefttoplabel = (ioLabelWndEX*)(FindChildWndEX(44));



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

	ioImageWndEX* m_iconimg;
	ioLabelWndEX* m_lefttoplabel;
	ioRichLabel* m_descrich;
	ioTabFrameButtonEX* m_infotabbutton;

};


#endif // _ETCINFOTABWNDBASE_


