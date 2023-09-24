//----------------------------------------------------------------------------------
/*
Filename	: CharacterControlWnd
Author		: sumin
Date		: 113.11.5_14:41:3
*/
//----------------------------------------------------------------------------------

#ifndef _CHARACTERCONTROLWND_
#define _CHARACTERCONTROLWND_

#include "CharacterControlWndBase.h"

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
CharacterControlWnd
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class ioPaperDollWnd;
class ioImageWndEX;
class CharacterControlWnd : public CharacterControlWndBase
{
	enum ITEM_SLOT_TYPE
	{
		IST_WEAPON,
		IST_AMOR,
		IST_HELM,
		IST_CAPE,
	};

	enum ABILITY_SLOT_TYPE
	{
		AST_ATTACK,
		AST_DEFENCE,
		AST_SPEED,
		AST_DROPDOWN,
	};

public:

	//----------------------------------------------------------------------------------
	/*
	생성 / 소멸
	*/
	CharacterControlWnd();
	virtual ~CharacterControlWnd();

	//----------------------------------------------------------------------------------
	/*
	control
	*/
	void				Refresh();
	void				SetPaperDoll();
	void				SetPaperDoll( int index );
	int					GetCurClassType() const;
	int					GetCurSexType() const;

	//----------------------------------------------------------------------------------
	/*
	ioWnd Override
	*/
	virtual void		OnProcess( float fTimePerSec );

	//----------------------------------------------------------------------------------
	/*
	ioWnd Event Override
	*/
	virtual void		iwm_create();
	virtual void		iwm_show();
	virtual void		iwm_command( ioWnd* wnd, int cmd, DWORD param );

	//----------------------------------------------------------------------------------
	/*
	Event Virtual Functions
	*/
	//
	virtual void		OnTabMain( ioWndEX* wnd, ioWndEXRadioEvent& event );
	//
	virtual void		OnAmorUpgrade( ioWndEX* wnd, ioWndEXMouseEvent& event );
	virtual void		OnAmorLeave( ioWndEX* wnd, ioWndEXMouseEvent& event );
	virtual void		OnAmorOver( ioWndEX* wnd, ioWndEXMouseEvent& event );	
	virtual void		OnAttackUpgrade( ioWndEX* wnd, ioWndEXMouseEvent& event );
	virtual void		OnAttackLeave( ioWndEX* wnd, ioWndEXMouseEvent& event );
	virtual void		OnAttackOver( ioWndEX* wnd, ioWndEXMouseEvent& event );
	virtual void		OnCapeUpgrade( ioWndEX* wnd, ioWndEXMouseEvent& event );
	virtual void		OnCapeLeave( ioWndEX* wnd, ioWndEXMouseEvent& event );
	virtual void		OnCapeOver( ioWndEX* wnd, ioWndEXMouseEvent& event );
	virtual void		OnDefenceUpgrade( ioWndEX* wnd, ioWndEXMouseEvent& event );
	virtual void		OnDefenceLeave( ioWndEX* wnd, ioWndEXMouseEvent& event );
	virtual void		OnDefenceOver( ioWndEX* wnd, ioWndEXMouseEvent& event );
	virtual void		OnDropDownUpgrade( ioWndEX* wnd, ioWndEXMouseEvent& event );
	virtual void		OnDropDownLeave( ioWndEX* wnd, ioWndEXMouseEvent& event );
	virtual void		OnDropDownOver( ioWndEX* wnd, ioWndEXMouseEvent& event );
	virtual void		OnHelmUpgrade( ioWndEX* wnd, ioWndEXMouseEvent& event );
	virtual void		OnHelmLeave( ioWndEX* wnd, ioWndEXMouseEvent& event );
	virtual void		OnHelmOver( ioWndEX* wnd, ioWndEXMouseEvent& event );
	virtual void		OnSpeedUpgrade( ioWndEX* wnd, ioWndEXMouseEvent& event );
	virtual void		OnSpeedLeave( ioWndEX* wnd, ioWndEXMouseEvent& event );
	virtual void		OnSpeedOver( ioWndEX* wnd, ioWndEXMouseEvent& event );
	virtual void		OnWaeponUpgrade( ioWndEX* wnd, ioWndEXMouseEvent& event );
	virtual void		OnWaeponLeave( ioWndEX* wnd, ioWndEXMouseEvent& event );
	virtual void		OnWaeponOver( ioWndEX* wnd, ioWndEXMouseEvent& event );
	//
	virtual void		OnLeftChangeCharacter( ioWndEX* wnd, ioWndEXMouseEvent& event );
	virtual void		OnRightChangeCharacter( ioWndEX* wnd, ioWndEXMouseEvent& event );
	virtual void		OnChangeCharacter( ioWndEX* wnd, ioWndEXMouseEvent& event );
	//
	virtual void		OnTurnLeft( ioWndEX* wnd, ioWndEXMouseEvent& event );
	virtual void		OnTurnRight( ioWndEX* wnd, ioWndEXMouseEvent& event );

protected:

	//----------------------------------------------------------------------------------
	/*
	Tab 윈도우 업데이트
	*/
	void				UpdateUpgradeTab();
	void				UpdateEquipTab();
	void				UpdateDecoTab();

	//----------------------------------------------------------------------------------
	/*
	기타
	*/
	int					GetItemGrowthLevel( ITEM_SLOT_TYPE type ); // 아이템으로 성장된 레벨 수치
	int					GetAbilityGrowthLevel( ABILITY_SLOT_TYPE type ); // 직접 성장된 레벨 수치
	int					GetMedalItemGrowthLevel( ITEM_SLOT_TYPE type ); // 메달로 올라간 아이템 레벨 수치
	int					GetMedalAbilityGrowthLevel( ABILITY_SLOT_TYPE type ); // 메달로 직접 올라간 레벨 수치
	int					GetMedalGrowthLevel( int slot, bool isitem ); // 메달로 올라간 레벨 수치
	void				ResetMedalUI( ioImageWndEX* img, int idx );

	void				ShowUpgradeWnd( int growthtype );
	void				SetupGrowthElementUI( int iconid, int labelid, int btnid, int classtype, int slotid, bool isitem );

protected:

	ioPaperDollWnd*		m_paperdollwnd;

	int					m_curindex;

	float				m_yaw;

	// 최적화를 위한 멤버변수
	ioWndEX*			m_wndTurnLeft;
	ioWndEX*			m_wndTurnRight;
	ioWndEX*			m_wndChangeChar;
};


#endif // _CHARACTERCONTROLWND_



