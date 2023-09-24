//----------------------------------------------------------------------------------
/*
Filename	: PaperMercenaryControlWnd
Author		: sumin
Date		: 113.11.5_14:41:3
*/
//----------------------------------------------------------------------------------

#ifndef _PAPERMERCENARYCONTROLLWND_
#define _PAPERMERCENARYCONTROLLWND_

#include "PaperMercenaryControlWndBase.h"

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
PaperMercenaryControlWnd
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class PaperMercenaryControlWnd : public PaperMercenaryControlWndBase
{
public:

	enum IWM_CMD_TYPE
	{
		ICT_CHANGE_CHARACTER = 100000,
		ICT_CHANGE_SEX,
		ICT_TURN_LEFT,
		ICT_TURN_RIGHT,
	};

	enum MODE
	{
		MODE_CHANGE,
		MODE_SEX
	};

public:

	//----------------------------------------------------------------------------------
	/*
	»ý¼º / ¼Ò¸ê
	*/
	PaperMercenaryControlWnd();
	virtual ~PaperMercenaryControlWnd();

	//----------------------------------------------------------------------------------
	/*
	ioWnd Override
	*/
	virtual void			OnProcess( float fTimePerSec );
	
	//----------------------------------------------------------------------------------
	/*
	ioWnd Event Override
	*/
	void					iwm_command( ioWnd* wnd, int cmd, DWORD param );

	//----------------------------------------------------------------------------------
	/*
	Event Virtual Functions
	*/
	virtual void			OnClickChange( ioWndEX* wnd, ioWndEXMouseEvent& event );
	virtual void			OnTurnLeft( ioWndEX* wnd, ioWndEXMouseEvent& event );
	virtual void			OnTurnRight( ioWndEX* wnd, ioWndEXMouseEvent& event );

	//----------------------------------------------------------------------------------
	/*
	Control
	*/
	void					SetOwner( ioWnd* owner ) { m_owner = owner; }
	void					SetClassType( int classtype ) { m_classtype = classtype; }
	void					SetSexType( int sextype ) { m_sextype = sextype; }
	void					SetMode( MODE mode ) { m_mode = mode; }
	
protected:

	ioWnd*					m_owner;

	int						m_classtype;
	int						m_sextype;

	MODE					m_mode;
};


#endif // _PAPERMERCENARYCONTROLLWND_



