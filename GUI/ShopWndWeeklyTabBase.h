//----------------------------------------------------------------------------------
/*
Filename	: C:\LostSagaClient\lostsaga\programming\src\LSClient\GUI\ShopWndWeeklyTabBase.h
Author		: UITool
Date		: 113.11.24_12:28:23

*/
//----------------------------------------------------------------------------------

#ifndef _IOSHOPWNDWEEKLYTABBASE_

#define _IOSHOPWNDWEEKLYTABBASE_


#include "ioWndEX.h"

#include "ioButtonWndEX.h"
#include "ioFrameWndEX.h"
#include "ioLabelWndEX.h"
#include "ioRichLabel.h"


//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
ioShopWndWeeklyTabBase
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class ioShopWndWeeklyTabBase : public ioWndEX
{
protected:

	enum
	{
		ID_FRM_ITEMBONUS_0_0 = 12,
		ID_FRM_ITEMBONUS_0_1 = 14,
		ID_FRM_ITEMBONUS_0_2 = 18,
		ID_FRM_ITEMBONUS_1_0 = 30,
		ID_FRM_ITEMBONUS_1_1 = 32,
		ID_FRM_ITEMBONUS_1_2 = 34,
		ID_FRM_ITEMBONUS_2_0 = 46,
		ID_FRM_ITEMBONUS_2_1 = 48,
		ID_FRM_ITEMBONUS_2_2 = 50,
		ID_FRM_ITEMBONUS_3_0 = 62,
		ID_FRM_ITEMBONUS_3_1 = 64,
		ID_FRM_ITEMBONUS_3_2 = 66,
		ID_FRM_ITEMBONUS_4_0 = 78,
		ID_FRM_ITEMBONUS_4_1 = 80,
		ID_FRM_ITEMBONUS_4_2 = 82,
		ID_FRM_ITEMBONUS_5_0 = 94,
		ID_FRM_ITEMBONUS_5_1 = 96,
		ID_FRM_ITEMBONUS_5_2 = 98,
		ID_FRM_ITEMBONUS_6_0 = 110,
		ID_FRM_ITEMBONUS_6_1 = 112,
		ID_FRM_ITEMBONUS_6_2 = 114,
		ID_FRM_ITEMBONUS_7_0 = 126,
		ID_FRM_ITEMBONUS_7_1 = 128,
		ID_FRM_ITEMBONUS_7_2 = 130,
		ID_FRM_ITEMBONUS_8_0 = 142,
		ID_FRM_ITEMBONUS_8_1 = 144,
		ID_FRM_ITEMBONUS_8_2 = 146,
		ID_FRM_ITEMBONUS_9_0 = 158,
		ID_FRM_ITEMBONUS_9_1 = 160,
		ID_FRM_ITEMBONUS_9_2 = 162,
		ID_IMGBTN_ITEMICON_0 = 3,
		ID_IMGBTN_ITEMICON_1 = 21,
		ID_IMGBTN_ITEMICON_2 = 37,
		ID_IMGBTN_ITEMICON_3 = 53,
		ID_IMGBTN_ITEMICON_4 = 69,
		ID_IMGBTN_ITEMICON_5 = 85,
		ID_IMGBTN_ITEMICON_6 = 101,
		ID_IMGBTN_ITEMICON_7 = 117,
		ID_IMGBTN_ITEMICON_8 = 133,
		ID_IMGBTN_ITEMICON_9 = 149,
		ID_LAB_ITEMBONUS_0_0 = 13,
		ID_LAB_ITEMBONUS_0_1 = 15,
		ID_LAB_ITEMBONUS_0_2 = 19,
		ID_LAB_ITEMBONUS_1_0 = 31,
		ID_LAB_ITEMBONUS_1_1 = 33,
		ID_LAB_ITEMBONUS_1_2 = 35,
		ID_LAB_ITEMBONUS_2_0 = 47,
		ID_LAB_ITEMBONUS_2_1 = 49,
		ID_LAB_ITEMBONUS_2_2 = 51,
		ID_LAB_ITEMBONUS_3_0 = 63,
		ID_LAB_ITEMBONUS_3_1 = 65,
		ID_LAB_ITEMBONUS_3_2 = 67,
		ID_LAB_ITEMBONUS_4_0 = 79,
		ID_LAB_ITEMBONUS_4_1 = 81,
		ID_LAB_ITEMBONUS_4_2 = 83,
		ID_LAB_ITEMBONUS_5_0 = 95,
		ID_LAB_ITEMBONUS_5_1 = 97,
		ID_LAB_ITEMBONUS_5_2 = 99,
		ID_LAB_ITEMBONUS_6_0 = 111,
		ID_LAB_ITEMBONUS_6_1 = 113,
		ID_LAB_ITEMBONUS_6_2 = 115,
		ID_LAB_ITEMBONUS_7_0 = 127,
		ID_LAB_ITEMBONUS_7_1 = 129,
		ID_LAB_ITEMBONUS_7_2 = 131,
		ID_LAB_ITEMBONUS_8_0 = 143,
		ID_LAB_ITEMBONUS_8_1 = 145,
		ID_LAB_ITEMBONUS_8_2 = 147,
		ID_LAB_ITEMBONUS_9_0 = 159,
		ID_LAB_ITEMBONUS_9_1 = 161,
		ID_LAB_ITEMBONUS_9_2 = 163,
		ID_LAB_ITEMNAME_0 = 4,
		ID_LAB_ITEMNAME_1 = 22,
		ID_LAB_ITEMNAME_2 = 38,
		ID_LAB_ITEMNAME_3 = 54,
		ID_LAB_ITEMNAME_4 = 70,
		ID_LAB_ITEMNAME_5 = 86,
		ID_LAB_ITEMNAME_6 = 102,
		ID_LAB_ITEMNAME_7 = 118,
		ID_LAB_ITEMNAME_8 = 134,
		ID_LAB_ITEMNAME_9 = 150,

	};

public:

	//----------------------------------------------------------------------------------
	/*
	积己 / 家戈
	*/
	ioShopWndWeeklyTabBase() : ioWndEX() 
	{
		m_backfrm[0] = 0;
		m_backfrm[1] = 0;
		m_backfrm[2] = 0;
		m_backfrm[3] = 0;
		m_backfrm[4] = 0;
		m_backfrm[5] = 0;
		m_backfrm[6] = 0;
		m_backfrm[7] = 0;
		m_backfrm[8] = 0;
		m_backfrm[9] = 0;
		m_bonuslabel0[0] = 0;
		m_bonuslabel0[1] = 0;
		m_bonuslabel0[2] = 0;
		m_bonuslabel0[3] = 0;
		m_bonuslabel0[4] = 0;
		m_bonuslabel0[5] = 0;
		m_bonuslabel0[6] = 0;
		m_bonuslabel0[7] = 0;
		m_bonuslabel0[8] = 0;
		m_bonuslabel0[9] = 0;
		m_bonuslabel1[0] = 0;
		m_bonuslabel1[1] = 0;
		m_bonuslabel1[2] = 0;
		m_bonuslabel1[3] = 0;
		m_bonuslabel1[4] = 0;
		m_bonuslabel1[5] = 0;
		m_bonuslabel1[6] = 0;
		m_bonuslabel1[7] = 0;
		m_bonuslabel1[8] = 0;
		m_bonuslabel1[9] = 0;
		m_bonuslabel2[0] = 0;
		m_bonuslabel2[1] = 0;
		m_bonuslabel2[2] = 0;
		m_bonuslabel2[3] = 0;
		m_bonuslabel2[4] = 0;
		m_bonuslabel2[5] = 0;
		m_bonuslabel2[6] = 0;
		m_bonuslabel2[7] = 0;
		m_bonuslabel2[8] = 0;
		m_bonuslabel2[9] = 0;
		m_iconbtn[0] = 0;
		m_iconbtn[1] = 0;
		m_iconbtn[2] = 0;
		m_iconbtn[3] = 0;
		m_iconbtn[4] = 0;
		m_iconbtn[5] = 0;
		m_iconbtn[6] = 0;
		m_iconbtn[7] = 0;
		m_iconbtn[8] = 0;
		m_iconbtn[9] = 0;
		m_namelabel[0] = 0;
		m_namelabel[1] = 0;
		m_namelabel[2] = 0;
		m_namelabel[3] = 0;
		m_namelabel[4] = 0;
		m_namelabel[5] = 0;
		m_namelabel[6] = 0;
		m_namelabel[7] = 0;
		m_namelabel[8] = 0;
		m_namelabel[9] = 0;
		m_pricebackfrm[0] = 0;
		m_pricebackfrm[1] = 0;
		m_pricebackfrm[2] = 0;
		m_pricebackfrm[3] = 0;
		m_pricebackfrm[4] = 0;
		m_pricebackfrm[5] = 0;
		m_pricebackfrm[6] = 0;
		m_pricebackfrm[7] = 0;
		m_pricebackfrm[8] = 0;
		m_pricebackfrm[9] = 0;
		m_pricelabel[0] = 0;
		m_pricelabel[1] = 0;
		m_pricelabel[2] = 0;
		m_pricelabel[3] = 0;
		m_pricelabel[4] = 0;
		m_pricelabel[5] = 0;
		m_pricelabel[6] = 0;
		m_pricelabel[7] = 0;
		m_pricelabel[8] = 0;
		m_pricelabel[9] = 0;

	}

	virtual ~ioShopWndWeeklyTabBase() 
	{
	}

	//----------------------------------------------------------------------------------
	/*
	楷搬 / 楷搬 产扁
	*/
	virtual void	iwm_create()
	{
		ioWndEX::iwm_create();

		m_backfrm[0] = (ioFrameWndEX*)(FindChildWndEX(2));
		m_backfrm[1] = (ioFrameWndEX*)(FindChildWndEX(20));
		m_backfrm[2] = (ioFrameWndEX*)(FindChildWndEX(36));
		m_backfrm[3] = (ioFrameWndEX*)(FindChildWndEX(52));
		m_backfrm[4] = (ioFrameWndEX*)(FindChildWndEX(68));
		m_backfrm[5] = (ioFrameWndEX*)(FindChildWndEX(84));
		m_backfrm[6] = (ioFrameWndEX*)(FindChildWndEX(100));
		m_backfrm[7] = (ioFrameWndEX*)(FindChildWndEX(116));
		m_backfrm[8] = (ioFrameWndEX*)(FindChildWndEX(132));
		m_backfrm[9] = (ioFrameWndEX*)(FindChildWndEX(148));
		m_bonuslabel0[0] = (ioLabelWndEX*)(FindChildWndEX(13));
		m_bonuslabel0[1] = (ioLabelWndEX*)(FindChildWndEX(31));
		m_bonuslabel0[2] = (ioLabelWndEX*)(FindChildWndEX(47));
		m_bonuslabel0[3] = (ioLabelWndEX*)(FindChildWndEX(63));
		m_bonuslabel0[4] = (ioLabelWndEX*)(FindChildWndEX(79));
		m_bonuslabel0[5] = (ioLabelWndEX*)(FindChildWndEX(95));
		m_bonuslabel0[6] = (ioLabelWndEX*)(FindChildWndEX(111));
		m_bonuslabel0[7] = (ioLabelWndEX*)(FindChildWndEX(127));
		m_bonuslabel0[8] = (ioLabelWndEX*)(FindChildWndEX(143));
		m_bonuslabel0[9] = (ioLabelWndEX*)(FindChildWndEX(159));
		m_bonuslabel1[0] = (ioLabelWndEX*)(FindChildWndEX(15));
		m_bonuslabel1[1] = (ioLabelWndEX*)(FindChildWndEX(33));
		m_bonuslabel1[2] = (ioLabelWndEX*)(FindChildWndEX(49));
		m_bonuslabel1[3] = (ioLabelWndEX*)(FindChildWndEX(65));
		m_bonuslabel1[4] = (ioLabelWndEX*)(FindChildWndEX(81));
		m_bonuslabel1[5] = (ioLabelWndEX*)(FindChildWndEX(97));
		m_bonuslabel1[6] = (ioLabelWndEX*)(FindChildWndEX(113));
		m_bonuslabel1[7] = (ioLabelWndEX*)(FindChildWndEX(129));
		m_bonuslabel1[8] = (ioLabelWndEX*)(FindChildWndEX(145));
		m_bonuslabel1[9] = (ioLabelWndEX*)(FindChildWndEX(161));
		m_bonuslabel2[0] = (ioLabelWndEX*)(FindChildWndEX(19));
		m_bonuslabel2[1] = (ioLabelWndEX*)(FindChildWndEX(35));
		m_bonuslabel2[2] = (ioLabelWndEX*)(FindChildWndEX(51));
		m_bonuslabel2[3] = (ioLabelWndEX*)(FindChildWndEX(67));
		m_bonuslabel2[4] = (ioLabelWndEX*)(FindChildWndEX(83));
		m_bonuslabel2[5] = (ioLabelWndEX*)(FindChildWndEX(99));
		m_bonuslabel2[6] = (ioLabelWndEX*)(FindChildWndEX(115));
		m_bonuslabel2[7] = (ioLabelWndEX*)(FindChildWndEX(131));
		m_bonuslabel2[8] = (ioLabelWndEX*)(FindChildWndEX(147));
		m_bonuslabel2[9] = (ioLabelWndEX*)(FindChildWndEX(163));
		m_iconbtn[0] = (ioImageButtonWndEX*)(FindChildWndEX(3));
		m_iconbtn[1] = (ioImageButtonWndEX*)(FindChildWndEX(21));
		m_iconbtn[2] = (ioImageButtonWndEX*)(FindChildWndEX(37));
		m_iconbtn[3] = (ioImageButtonWndEX*)(FindChildWndEX(53));
		m_iconbtn[4] = (ioImageButtonWndEX*)(FindChildWndEX(69));
		m_iconbtn[5] = (ioImageButtonWndEX*)(FindChildWndEX(85));
		m_iconbtn[6] = (ioImageButtonWndEX*)(FindChildWndEX(101));
		m_iconbtn[7] = (ioImageButtonWndEX*)(FindChildWndEX(117));
		m_iconbtn[8] = (ioImageButtonWndEX*)(FindChildWndEX(133));
		m_iconbtn[9] = (ioImageButtonWndEX*)(FindChildWndEX(149));
		m_namelabel[0] = (ioLabelWndEX*)(FindChildWndEX(4));
		m_namelabel[1] = (ioLabelWndEX*)(FindChildWndEX(22));
		m_namelabel[2] = (ioLabelWndEX*)(FindChildWndEX(38));
		m_namelabel[3] = (ioLabelWndEX*)(FindChildWndEX(54));
		m_namelabel[4] = (ioLabelWndEX*)(FindChildWndEX(70));
		m_namelabel[5] = (ioLabelWndEX*)(FindChildWndEX(86));
		m_namelabel[6] = (ioLabelWndEX*)(FindChildWndEX(102));
		m_namelabel[7] = (ioLabelWndEX*)(FindChildWndEX(118));
		m_namelabel[8] = (ioLabelWndEX*)(FindChildWndEX(134));
		m_namelabel[9] = (ioLabelWndEX*)(FindChildWndEX(150));
		m_pricebackfrm[0] = (ioFrameWndEX*)(FindChildWndEX(5));
		m_pricebackfrm[1] = (ioFrameWndEX*)(FindChildWndEX(23));
		m_pricebackfrm[2] = (ioFrameWndEX*)(FindChildWndEX(39));
		m_pricebackfrm[3] = (ioFrameWndEX*)(FindChildWndEX(55));
		m_pricebackfrm[4] = (ioFrameWndEX*)(FindChildWndEX(71));
		m_pricebackfrm[5] = (ioFrameWndEX*)(FindChildWndEX(87));
		m_pricebackfrm[6] = (ioFrameWndEX*)(FindChildWndEX(103));
		m_pricebackfrm[7] = (ioFrameWndEX*)(FindChildWndEX(119));
		m_pricebackfrm[8] = (ioFrameWndEX*)(FindChildWndEX(135));
		m_pricebackfrm[9] = (ioFrameWndEX*)(FindChildWndEX(151));
		m_pricelabel[0] = (ioRichLabel*)(FindChildWndEX(164));
		m_pricelabel[1] = (ioRichLabel*)(FindChildWndEX(165));
		m_pricelabel[2] = (ioRichLabel*)(FindChildWndEX(166));
		m_pricelabel[3] = (ioRichLabel*)(FindChildWndEX(167));
		m_pricelabel[4] = (ioRichLabel*)(FindChildWndEX(168));
		m_pricelabel[5] = (ioRichLabel*)(FindChildWndEX(169));
		m_pricelabel[6] = (ioRichLabel*)(FindChildWndEX(170));
		m_pricelabel[7] = (ioRichLabel*)(FindChildWndEX(171));
		m_pricelabel[8] = (ioRichLabel*)(FindChildWndEX(172));
		m_pricelabel[9] = (ioRichLabel*)(FindChildWndEX(173));


		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(101) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< ioShopWndWeeklyTabBase >( this, &ioShopWndWeeklyTabBase::OnLClickShopIcon11 ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(117) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< ioShopWndWeeklyTabBase >( this, &ioShopWndWeeklyTabBase::OnLClickShopIcon21 ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(133) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< ioShopWndWeeklyTabBase >( this, &ioShopWndWeeklyTabBase::OnLClickShopIcon31 ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(149) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< ioShopWndWeeklyTabBase >( this, &ioShopWndWeeklyTabBase::OnLClickShopIcon41 ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(21) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< ioShopWndWeeklyTabBase >( this, &ioShopWndWeeklyTabBase::OnLClickShopIcon10 ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(3) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< ioShopWndWeeklyTabBase >( this, &ioShopWndWeeklyTabBase::OnLClickShopIcon00 ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(37) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< ioShopWndWeeklyTabBase >( this, &ioShopWndWeeklyTabBase::OnLClickShopIcon20 ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(53) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< ioShopWndWeeklyTabBase >( this, &ioShopWndWeeklyTabBase::OnLClickShopIcon30 ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(69) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< ioShopWndWeeklyTabBase >( this, &ioShopWndWeeklyTabBase::OnLClickShopIcon40 ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(85) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< ioShopWndWeeklyTabBase >( this, &ioShopWndWeeklyTabBase::OnLClickShopIcon01 ) );
				}
		}

	}

	virtual void	iwm_destroy()
	{
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(101) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(7), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(117) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(7), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(133) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(7), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(149) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(7), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(21) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(7), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(3) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(7), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(37) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(7), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(53) );
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
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(85) );
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
	virtual void OnLClickShopIcon00( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnLClickShopIcon01( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnLClickShopIcon10( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnLClickShopIcon11( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnLClickShopIcon20( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnLClickShopIcon21( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnLClickShopIcon30( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnLClickShopIcon31( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnLClickShopIcon40( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnLClickShopIcon41( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}


protected:

	ioFrameWndEX* m_backfrm[10];
	ioFrameWndEX* m_pricebackfrm[10];
	ioImageButtonWndEX* m_iconbtn[10];
	ioLabelWndEX* m_bonuslabel0[10];
	ioLabelWndEX* m_bonuslabel1[10];
	ioLabelWndEX* m_bonuslabel2[10];
	ioLabelWndEX* m_namelabel[10];
	ioRichLabel* m_pricelabel[10];

};


#endif // _IOSHOPWNDWEEKLYTABBASE_


