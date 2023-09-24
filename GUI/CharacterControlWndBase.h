//----------------------------------------------------------------------------------
/*
Filename	: C:\LostSagaClient\lostsaga\programming\src\LSClient\GUI\charactercontrolwndBase.h
Author		: UITool
Date		: 113.11.18_18:13:22

*/
//----------------------------------------------------------------------------------

#ifndef _CHARACTERCONTROLWNDBASE_

#define _CHARACTERCONTROLWNDBASE_


#include "ioWndEX.h"



//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
CharacterControlWndBase
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class CharacterControlWndBase : public ioWndEX
{
protected:

	enum
	{
		ID = 7,
		ID_BTN_AMOR_UP = 134,
		ID_BTN_ATTACK_UP = 98,
		ID_BTN_CAPE_UP = 138,
		ID_BTN_CHANGE_CHAR = 69,
		ID_BTN_CHAR_CHANGE_LEFT = 2,
		ID_BTN_CHAR_CHANGE_RIGHT = 1,
		ID_BTN_DEFENCE_UP = 122,
		ID_BTN_DROPDOWN_UP = 114,
		ID_BTN_HELM_UP = 130,
		ID_BTN_SPEED_UP = 106,
		ID_BTN_TURN_LEFT = 67,
		ID_BTN_TURN_RIGHT = 68,
		ID_BTN_WEAPON_UP = 126,
		ID_FRM_ATTACK_BACK = 95,
		ID_FRM_DROPDOWN_BACK = 111,
		ID_IMG_AMOR_ICON = 132,
		ID_IMG_ATTACK_ICON = 96,
		ID_IMG_CAPE_ICON = 136,
		ID_IMG_DECO_FACE = 56,
		ID_IMG_DECO_FACECOLOR = 57,
		ID_IMG_DECO_HAIR = 64,
		ID_IMG_DECO_HAIRCOLOR = 65,
		ID_IMG_DECO_KINDRED = 55,
		ID_IMG_DECO_PANTS = 66,
		ID_IMG_DEFENCE_ICON = 120,
		ID_IMG_DROPDOWN_ICON = 112,
		ID_IMG_EQIP_AMOR = 45,
		ID_IMG_EQIP_CAPE = 47,
		ID_IMG_EQIP_HELM = 46,
		ID_IMG_EQIP_WEAPON = 44,
		ID_IMG_HELM_ICON = 128,
		ID_IMG_MEDAL_0 = 48,
		ID_IMG_MEDAL_10 = 49,
		ID_IMG_MEDAL_20 = 50,
		ID_IMG_MEDAL_30 = 52,
		ID_IMG_MEDAL_40 = 51,
		ID_IMG_MEDAL_50 = 53,
		ID_IMG_SPEED_ICON = 104,
		ID_IMG_WEAPON_ICON = 124,
		ID_LABEL_NAME = 5,
		ID_LABEL_NAME2 = 141,
		ID_LAB_AMOR_TXT = 133,
		ID_LAB_ATTACK_TXT = 97,
		ID_LAB_CAPE_TXT = 137,
		ID_LAB_DEFENCE_TXT = 121,
		ID_LAB_DROPDOWN_TXT = 113,
		ID_LAB_EQIP_AMOR_UPGRADE = 21,
		ID_LAB_EQIP_CAPE_UPGRADE = 11,
		ID_LAB_EQIP_HELM_UPGRADE = 33,
		ID_LAB_EQIP_WEAPON_UPGRADE = 139,
		ID_LAB_HELM_TXT = 129,
		ID_LAB_SPEED_TXT = 105,
		ID_LAB_WEAPON_TXT = 125,
		ID_TAB_MAINTAB = 6,

	};

public:

	//----------------------------------------------------------------------------------
	/*
	积己 / 家戈
	*/
	CharacterControlWndBase() : ioWndEX() 
	{

	}

	virtual ~CharacterControlWndBase() 
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
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(1) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< CharacterControlWndBase >( this, &CharacterControlWndBase::OnRightChangeCharacter ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(106) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(11), new ioWndEXMouseEventHandlerEX< CharacterControlWndBase >( this, &CharacterControlWndBase::OnSpeedOver ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(106) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(13), new ioWndEXMouseEventHandlerEX< CharacterControlWndBase >( this, &CharacterControlWndBase::OnSpeedLeave ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(106) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< CharacterControlWndBase >( this, &CharacterControlWndBase::OnSpeedUpgrade ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(114) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(11), new ioWndEXMouseEventHandlerEX< CharacterControlWndBase >( this, &CharacterControlWndBase::OnDropDownOver ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(114) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(13), new ioWndEXMouseEventHandlerEX< CharacterControlWndBase >( this, &CharacterControlWndBase::OnDropDownLeave ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(114) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< CharacterControlWndBase >( this, &CharacterControlWndBase::OnDropDownUpgrade ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(122) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(11), new ioWndEXMouseEventHandlerEX< CharacterControlWndBase >( this, &CharacterControlWndBase::OnDefenceOver ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(122) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(13), new ioWndEXMouseEventHandlerEX< CharacterControlWndBase >( this, &CharacterControlWndBase::OnDefenceLeave ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(122) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< CharacterControlWndBase >( this, &CharacterControlWndBase::OnDefenceUpgrade ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(126) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(11), new ioWndEXMouseEventHandlerEX< CharacterControlWndBase >( this, &CharacterControlWndBase::OnWaeponOver ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(126) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(13), new ioWndEXMouseEventHandlerEX< CharacterControlWndBase >( this, &CharacterControlWndBase::OnWaeponLeave ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(126) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< CharacterControlWndBase >( this, &CharacterControlWndBase::OnWaeponUpgrade ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(130) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(11), new ioWndEXMouseEventHandlerEX< CharacterControlWndBase >( this, &CharacterControlWndBase::OnHelmOver ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(130) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(13), new ioWndEXMouseEventHandlerEX< CharacterControlWndBase >( this, &CharacterControlWndBase::OnHelmLeave ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(130) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< CharacterControlWndBase >( this, &CharacterControlWndBase::OnHelmUpgrade ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(134) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(11), new ioWndEXMouseEventHandlerEX< CharacterControlWndBase >( this, &CharacterControlWndBase::OnAmorOver ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(134) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(13), new ioWndEXMouseEventHandlerEX< CharacterControlWndBase >( this, &CharacterControlWndBase::OnAmorLeave ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(134) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< CharacterControlWndBase >( this, &CharacterControlWndBase::OnAmorUpgrade ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(138) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(11), new ioWndEXMouseEventHandlerEX< CharacterControlWndBase >( this, &CharacterControlWndBase::OnCapeOver ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(138) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(13), new ioWndEXMouseEventHandlerEX< CharacterControlWndBase >( this, &CharacterControlWndBase::OnCapeLeave ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(138) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< CharacterControlWndBase >( this, &CharacterControlWndBase::OnCapeUpgrade ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(2) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< CharacterControlWndBase >( this, &CharacterControlWndBase::OnLeftChangeCharacter ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(6) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(16), new ioWndEXRadioEventHandlerEX< CharacterControlWndBase >( this, &CharacterControlWndBase::OnTabMain ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(67) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< CharacterControlWndBase >( this, &CharacterControlWndBase::OnTurnLeft ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(68) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< CharacterControlWndBase >( this, &CharacterControlWndBase::OnTurnRight ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(69) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< CharacterControlWndBase >( this, &CharacterControlWndBase::OnChangeCharacter ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(98) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(11), new ioWndEXMouseEventHandlerEX< CharacterControlWndBase >( this, &CharacterControlWndBase::OnAttackOver ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(98) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(13), new ioWndEXMouseEventHandlerEX< CharacterControlWndBase >( this, &CharacterControlWndBase::OnAttackLeave ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(98) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< CharacterControlWndBase >( this, &CharacterControlWndBase::OnAttackUpgrade ) );
				}
		}

	}

	virtual void	iwm_destroy()
	{
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(1) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(7), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(106) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(11), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(106) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(13), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(106) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(7), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(114) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(11), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(114) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(13), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(114) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(7), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(122) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(11), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(122) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(13), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(122) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(7), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(126) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(11), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(126) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(13), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(126) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(7), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(130) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(11), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(130) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(13), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(130) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(7), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(134) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(11), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(134) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(13), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(134) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(7), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(138) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(11), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(138) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(13), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(138) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(7), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(2) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(7), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(6) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(16), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(67) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(7), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(68) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(7), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(69) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(7), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(98) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(11), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(98) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(13), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(98) );
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
	virtual void OnAmorLeave( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnAmorOver( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnAmorUpgrade( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnAttackLeave( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnAttackOver( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnAttackUpgrade( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnCapeLeave( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnCapeOver( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnCapeUpgrade( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnChangeCharacter( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnDefenceLeave( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnDefenceOver( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnDefenceUpgrade( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnDropDownLeave( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnDropDownOver( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnDropDownUpgrade( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnHelmLeave( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnHelmOver( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnHelmUpgrade( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnLeftChangeCharacter( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnRightChangeCharacter( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnSpeedLeave( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnSpeedOver( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnSpeedUpgrade( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnTabMain( ioWndEX* wnd, ioWndEXRadioEvent& event ) {}
	virtual void OnTurnLeft( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnTurnRight( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnWaeponLeave( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnWaeponOver( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnWaeponUpgrade( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}


protected:


};


#endif // _CHARACTERCONTROLWNDBASE_


