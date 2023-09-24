//----------------------------------------------------------------------------------
/*
Filename	: C:\LostSagaClient\lostsaga\programming\src\LSClient\GUI\ShopwndEtcTabBase.h
Author		: UITool
Date		: 113.11.26_13:13:0

*/
//----------------------------------------------------------------------------------

#ifndef _IOSHOPWNDETCTABBASE_

#define _IOSHOPWNDETCTABBASE_


#include "ioWndEX.h"

#include "ioButtonWndEX.h"
#include "ioFrameWndEX.h"
#include "ioLabelWndEX.h"
#include "ioRichLabel.h"


//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
ioShopWndEtcTabBase
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class ioShopWndEtcTabBase : public ioWndEX
{
protected:

	enum
	{
		ID_FRM_BACK_0 = 2,
		ID_FRM_BACK_1 = 26,
		ID_FRM_BACK_10 = 32,
		ID_FRM_BACK_11 = 74,
		ID_FRM_BACK_12 = 80,
		ID_FRM_BACK_13 = 86,
		ID_FRM_BACK_14 = 92,
		ID_FRM_BACK_2 = 20,
		ID_FRM_BACK_3 = 14,
		ID_FRM_BACK_4 = 8,
		ID_FRM_BACK_5 = 38,
		ID_FRM_BACK_6 = 44,
		ID_FRM_BACK_7 = 56,
		ID_FRM_BACK_8 = 62,
		ID_FRM_BACK_9 = 68,
		ID_FRM_PRICE_BACK_0 = 5,
		ID_FRM_PRICE_BACK_1 = 29,
		ID_FRM_PRICE_BACK_10 = 35,
		ID_FRM_PRICE_BACK_11 = 77,
		ID_FRM_PRICE_BACK_12 = 83,
		ID_FRM_PRICE_BACK_13 = 89,
		ID_FRM_PRICE_BACK_14 = 95,
		ID_FRM_PRICE_BACK_2 = 23,
		ID_FRM_PRICE_BACK_3 = 17,
		ID_FRM_PRICE_BACK_4 = 11,
		ID_FRM_PRICE_BACK_5 = 41,
		ID_FRM_PRICE_BACK_6 = 47,
		ID_FRM_PRICE_BACK_7 = 59,
		ID_FRM_PRICE_BACK_8 = 65,
		ID_FRM_PRICE_BACK_9 = 71,
		ID_IMGBTN_ICON_0 = 3,
		ID_IMGBTN_ICON_1 = 27,
		ID_IMGBTN_ICON_10 = 33,
		ID_IMGBTN_ICON_11 = 75,
		ID_IMGBTN_ICON_12 = 81,
		ID_IMGBTN_ICON_13 = 87,
		ID_IMGBTN_ICON_14 = 93,
		ID_IMGBTN_ICON_2 = 21,
		ID_IMGBTN_ICON_3 = 15,
		ID_IMGBTN_ICON_4 = 9,
		ID_IMGBTN_ICON_5 = 39,
		ID_IMGBTN_ICON_6 = 45,
		ID_IMGBTN_ICON_7 = 57,
		ID_IMGBTN_ICON_8 = 63,
		ID_IMGBTN_ICON_9 = 69,
		ID_LAB_NAME_0 = 4,
		ID_LAB_NAME_1 = 28,
		ID_LAB_NAME_10 = 34,
		ID_LAB_NAME_11 = 76,
		ID_LAB_NAME_12 = 82,
		ID_LAB_NAME_13 = 88,
		ID_LAB_NAME_14 = 94,
		ID_LAB_NAME_2 = 22,
		ID_LAB_NAME_3 = 16,
		ID_LAB_NAME_4 = 10,
		ID_LAB_NAME_5 = 40,
		ID_LAB_NAME_6 = 46,
		ID_LAB_NAME_7 = 58,
		ID_LAB_NAME_8 = 64,
		ID_LAB_NAME_9 = 70,

	};

public:

	//----------------------------------------------------------------------------------
	/*
	积己 / 家戈
	*/
	ioShopWndEtcTabBase() : ioWndEX() 
	{
		m_backfrm[0] = 0;
		m_backfrm[10] = 0;
		m_backfrm[11] = 0;
		m_backfrm[12] = 0;
		m_backfrm[13] = 0;
		m_backfrm[14] = 0;
		m_backfrm[1] = 0;
		m_backfrm[2] = 0;
		m_backfrm[3] = 0;
		m_backfrm[4] = 0;
		m_backfrm[5] = 0;
		m_backfrm[6] = 0;
		m_backfrm[7] = 0;
		m_backfrm[8] = 0;
		m_backfrm[9] = 0;
		m_iconbtn[0] = 0;
		m_iconbtn[10] = 0;
		m_iconbtn[11] = 0;
		m_iconbtn[12] = 0;
		m_iconbtn[13] = 0;
		m_iconbtn[14] = 0;
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
		m_namelabel[10] = 0;
		m_namelabel[11] = 0;
		m_namelabel[12] = 0;
		m_namelabel[13] = 0;
		m_namelabel[14] = 0;
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
		m_pricebackfrm[10] = 0;
		m_pricebackfrm[11] = 0;
		m_pricebackfrm[12] = 0;
		m_pricebackfrm[13] = 0;
		m_pricebackfrm[14] = 0;
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
		m_pricelabel[10] = 0;
		m_pricelabel[11] = 0;
		m_pricelabel[12] = 0;
		m_pricelabel[13] = 0;
		m_pricelabel[14] = 0;
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

	virtual ~ioShopWndEtcTabBase() 
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
		m_backfrm[10] = (ioFrameWndEX*)(FindChildWndEX(32));
		m_backfrm[11] = (ioFrameWndEX*)(FindChildWndEX(74));
		m_backfrm[12] = (ioFrameWndEX*)(FindChildWndEX(80));
		m_backfrm[13] = (ioFrameWndEX*)(FindChildWndEX(86));
		m_backfrm[14] = (ioFrameWndEX*)(FindChildWndEX(92));
		m_backfrm[1] = (ioFrameWndEX*)(FindChildWndEX(26));
		m_backfrm[2] = (ioFrameWndEX*)(FindChildWndEX(20));
		m_backfrm[3] = (ioFrameWndEX*)(FindChildWndEX(14));
		m_backfrm[4] = (ioFrameWndEX*)(FindChildWndEX(8));
		m_backfrm[5] = (ioFrameWndEX*)(FindChildWndEX(38));
		m_backfrm[6] = (ioFrameWndEX*)(FindChildWndEX(44));
		m_backfrm[7] = (ioFrameWndEX*)(FindChildWndEX(56));
		m_backfrm[8] = (ioFrameWndEX*)(FindChildWndEX(62));
		m_backfrm[9] = (ioFrameWndEX*)(FindChildWndEX(68));
		m_iconbtn[0] = (ioImageButtonWndEX*)(FindChildWndEX(3));
		m_iconbtn[10] = (ioImageButtonWndEX*)(FindChildWndEX(33));
		m_iconbtn[11] = (ioImageButtonWndEX*)(FindChildWndEX(75));
		m_iconbtn[12] = (ioImageButtonWndEX*)(FindChildWndEX(81));
		m_iconbtn[13] = (ioImageButtonWndEX*)(FindChildWndEX(87));
		m_iconbtn[14] = (ioImageButtonWndEX*)(FindChildWndEX(93));
		m_iconbtn[1] = (ioImageButtonWndEX*)(FindChildWndEX(27));
		m_iconbtn[2] = (ioImageButtonWndEX*)(FindChildWndEX(21));
		m_iconbtn[3] = (ioImageButtonWndEX*)(FindChildWndEX(15));
		m_iconbtn[4] = (ioImageButtonWndEX*)(FindChildWndEX(9));
		m_iconbtn[5] = (ioImageButtonWndEX*)(FindChildWndEX(39));
		m_iconbtn[6] = (ioImageButtonWndEX*)(FindChildWndEX(45));
		m_iconbtn[7] = (ioImageButtonWndEX*)(FindChildWndEX(57));
		m_iconbtn[8] = (ioImageButtonWndEX*)(FindChildWndEX(63));
		m_iconbtn[9] = (ioImageButtonWndEX*)(FindChildWndEX(69));
		m_namelabel[0] = (ioLabelWndEX*)(FindChildWndEX(4));
		m_namelabel[10] = (ioLabelWndEX*)(FindChildWndEX(34));
		m_namelabel[11] = (ioLabelWndEX*)(FindChildWndEX(76));
		m_namelabel[12] = (ioLabelWndEX*)(FindChildWndEX(82));
		m_namelabel[13] = (ioLabelWndEX*)(FindChildWndEX(88));
		m_namelabel[14] = (ioLabelWndEX*)(FindChildWndEX(94));
		m_namelabel[1] = (ioLabelWndEX*)(FindChildWndEX(28));
		m_namelabel[2] = (ioLabelWndEX*)(FindChildWndEX(22));
		m_namelabel[3] = (ioLabelWndEX*)(FindChildWndEX(16));
		m_namelabel[4] = (ioLabelWndEX*)(FindChildWndEX(10));
		m_namelabel[5] = (ioLabelWndEX*)(FindChildWndEX(40));
		m_namelabel[6] = (ioLabelWndEX*)(FindChildWndEX(46));
		m_namelabel[7] = (ioLabelWndEX*)(FindChildWndEX(58));
		m_namelabel[8] = (ioLabelWndEX*)(FindChildWndEX(64));
		m_namelabel[9] = (ioLabelWndEX*)(FindChildWndEX(70));
		m_pricebackfrm[0] = (ioFrameWndEX*)(FindChildWndEX(5));
		m_pricebackfrm[10] = (ioFrameWndEX*)(FindChildWndEX(35));
		m_pricebackfrm[11] = (ioFrameWndEX*)(FindChildWndEX(77));
		m_pricebackfrm[12] = (ioFrameWndEX*)(FindChildWndEX(83));
		m_pricebackfrm[13] = (ioFrameWndEX*)(FindChildWndEX(89));
		m_pricebackfrm[14] = (ioFrameWndEX*)(FindChildWndEX(95));
		m_pricebackfrm[1] = (ioFrameWndEX*)(FindChildWndEX(29));
		m_pricebackfrm[2] = (ioFrameWndEX*)(FindChildWndEX(23));
		m_pricebackfrm[3] = (ioFrameWndEX*)(FindChildWndEX(17));
		m_pricebackfrm[4] = (ioFrameWndEX*)(FindChildWndEX(11));
		m_pricebackfrm[5] = (ioFrameWndEX*)(FindChildWndEX(41));
		m_pricebackfrm[6] = (ioFrameWndEX*)(FindChildWndEX(47));
		m_pricebackfrm[7] = (ioFrameWndEX*)(FindChildWndEX(59));
		m_pricebackfrm[8] = (ioFrameWndEX*)(FindChildWndEX(65));
		m_pricebackfrm[9] = (ioFrameWndEX*)(FindChildWndEX(71));
		m_pricelabel[0] = (ioRichLabel*)(FindChildWndEX(96));
		m_pricelabel[10] = (ioRichLabel*)(FindChildWndEX(106));
		m_pricelabel[11] = (ioRichLabel*)(FindChildWndEX(107));
		m_pricelabel[12] = (ioRichLabel*)(FindChildWndEX(108));
		m_pricelabel[13] = (ioRichLabel*)(FindChildWndEX(109));
		m_pricelabel[14] = (ioRichLabel*)(FindChildWndEX(110));
		m_pricelabel[1] = (ioRichLabel*)(FindChildWndEX(97));
		m_pricelabel[2] = (ioRichLabel*)(FindChildWndEX(98));
		m_pricelabel[3] = (ioRichLabel*)(FindChildWndEX(99));
		m_pricelabel[4] = (ioRichLabel*)(FindChildWndEX(100));
		m_pricelabel[5] = (ioRichLabel*)(FindChildWndEX(101));
		m_pricelabel[6] = (ioRichLabel*)(FindChildWndEX(102));
		m_pricelabel[7] = (ioRichLabel*)(FindChildWndEX(103));
		m_pricelabel[8] = (ioRichLabel*)(FindChildWndEX(104));
		m_pricelabel[9] = (ioRichLabel*)(FindChildWndEX(105));


		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(15) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< ioShopWndEtcTabBase >( this, &ioShopWndEtcTabBase::OnLClickShopIcon30 ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(21) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< ioShopWndEtcTabBase >( this, &ioShopWndEtcTabBase::OnLClickShopIcon20 ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(27) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< ioShopWndEtcTabBase >( this, &ioShopWndEtcTabBase::OnLClickShopIcon10 ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(3) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< ioShopWndEtcTabBase >( this, &ioShopWndEtcTabBase::OnLClickShopIcon00 ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(33) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< ioShopWndEtcTabBase >( this, &ioShopWndEtcTabBase::OnLClickShopIcon02 ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(39) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< ioShopWndEtcTabBase >( this, &ioShopWndEtcTabBase::OnLClickShopIcon01 ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(45) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< ioShopWndEtcTabBase >( this, &ioShopWndEtcTabBase::OnLClickShopIcon11 ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(57) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< ioShopWndEtcTabBase >( this, &ioShopWndEtcTabBase::OnLClickShopIcon21 ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(63) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< ioShopWndEtcTabBase >( this, &ioShopWndEtcTabBase::OnLClickShopIcon31 ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(69) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< ioShopWndEtcTabBase >( this, &ioShopWndEtcTabBase::OnLClickShopIcon41 ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(75) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< ioShopWndEtcTabBase >( this, &ioShopWndEtcTabBase::OnLClickShopIcon12 ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(81) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< ioShopWndEtcTabBase >( this, &ioShopWndEtcTabBase::OnLClickShopIcon22 ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(87) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< ioShopWndEtcTabBase >( this, &ioShopWndEtcTabBase::OnLClickShopIcon32 ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(9) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< ioShopWndEtcTabBase >( this, &ioShopWndEtcTabBase::OnLClickShopIcon40 ) );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(93) );
				if ( child ) {
					child->Connect( static_cast<WNDEX_EVENT_TYPE>(7), new ioWndEXMouseEventHandlerEX< ioShopWndEtcTabBase >( this, &ioShopWndEtcTabBase::OnLClickShopIcon42 ) );
				}
		}

	}

	virtual void	iwm_destroy()
	{
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(15) );
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
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(27) );
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
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(33) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(7), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(39) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(7), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(45) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(7), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(57) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(7), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(63) );
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
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(75) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(7), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(81) );
				if ( child ) {
					child->Disconnect( static_cast<WNDEX_EVENT_TYPE>(7), this );
				}
		}
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(87) );
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
		{
				ioWndEX* child = (ioWndEX*)( FindChildWndEX(93) );
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
	virtual void OnLClickShopIcon02( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnLClickShopIcon10( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnLClickShopIcon11( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnLClickShopIcon12( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnLClickShopIcon20( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnLClickShopIcon21( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnLClickShopIcon22( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnLClickShopIcon30( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnLClickShopIcon31( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnLClickShopIcon32( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnLClickShopIcon40( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnLClickShopIcon41( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}
	virtual void OnLClickShopIcon42( ioWndEX* wnd, ioWndEXMouseEvent& event ) {}


protected:

	ioFrameWndEX* m_backfrm[15];
	ioFrameWndEX* m_pricebackfrm[15];
	ioImageButtonWndEX* m_iconbtn[15];
	ioLabelWndEX* m_namelabel[15];
	ioRichLabel* m_pricelabel[15];

};


#endif // _IOSHOPWNDETCTABBASE_


