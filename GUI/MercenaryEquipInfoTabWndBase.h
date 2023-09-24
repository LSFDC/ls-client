//----------------------------------------------------------------------------------
/*
Filename	: C:\LostSagaClient\lostsaga\programming\src\LSClient\GUI\MercenaryEquipInfoTabWndBase.h
Author		: UITool
Date		: 114.0.3_10:8:18

*/
//----------------------------------------------------------------------------------

#ifndef _MERCENARYEQUIPINFOTABWNDBASE_

#define _MERCENARYEQUIPINFOTABWNDBASE_


#include "ioWndEX.h"

#include "ioFlashPlayer.h"
#include "ioImageWndEX.h"
#include "ioLabelWndEX.h"
#include "ioTabWndEX.h"


//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
MercenaryEquipInfoTabWndBase
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class MercenaryEquipInfoTabWndBase : public ioWndEX
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
	MercenaryEquipInfoTabWndBase() : ioWndEX() 
	{
		m_backimg = 0;
		m_flashplayer = 0;
		m_infotabbutton = 0;
		m_lefttoplabel = 0;
		m_righttoplabel = 0;
		m_skilldesclabel[0] = 0;
		m_skilldesclabel[1] = 0;
		m_skilldesclabel[2] = 0;
		m_skilldesclabel[3] = 0;
		m_skillicon[0] = 0;
		m_skillicon[1] = 0;
		m_skillicon[2] = 0;
		m_skillicon[3] = 0;
		m_skilltypelabel[0] = 0;
		m_skilltypelabel[1] = 0;
		m_skilltypelabel[2] = 0;
		m_skilltypelabel[3] = 0;

	}

	virtual ~MercenaryEquipInfoTabWndBase() 
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
		m_flashplayer = (ioFlashPlayer*)(FindChildWndEX(64));
		m_infotabbutton = (ioTabFrameButtonEX*)(FindChildWndEX(37));
		m_lefttoplabel = (ioLabelWndEX*)(FindChildWndEX(40));
		m_righttoplabel = (ioLabelWndEX*)(FindChildWndEX(41));
		m_skilldesclabel[0] = (ioLabelWndEX*)(FindChildWndEX(56));
		m_skilldesclabel[1] = (ioLabelWndEX*)(FindChildWndEX(57));
		m_skilldesclabel[2] = (ioLabelWndEX*)(FindChildWndEX(58));
		m_skilldesclabel[3] = (ioLabelWndEX*)(FindChildWndEX(59));
		m_skillicon[0] = (ioImageWndEX*)(FindChildWndEX(44));
		m_skillicon[1] = (ioImageWndEX*)(FindChildWndEX(47));
		m_skillicon[2] = (ioImageWndEX*)(FindChildWndEX(46));
		m_skillicon[3] = (ioImageWndEX*)(FindChildWndEX(45));
		m_skilltypelabel[0] = (ioLabelWndEX*)(FindChildWndEX(52));
		m_skilltypelabel[1] = (ioLabelWndEX*)(FindChildWndEX(53));
		m_skilltypelabel[2] = (ioLabelWndEX*)(FindChildWndEX(54));
		m_skilltypelabel[3] = (ioLabelWndEX*)(FindChildWndEX(55));



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

	ioFlashPlayer* m_flashplayer;
	ioImageWndEX* m_backimg;
	ioImageWndEX* m_skillicon[4];
	ioLabelWndEX* m_lefttoplabel;
	ioLabelWndEX* m_righttoplabel;
	ioLabelWndEX* m_skilldesclabel[4];
	ioLabelWndEX* m_skilltypelabel[4];
	ioTabFrameButtonEX* m_infotabbutton;

};


#endif // _MERCENARYEQUIPINFOTABWNDBASE_


