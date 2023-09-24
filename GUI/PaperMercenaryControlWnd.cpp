//----------------------------------------------------------------------------------
/*
Filename	: PaperMercenaryControlWnd
Author		: sumin
Date		: 113.11.5_14:41:3
*/
//----------------------------------------------------------------------------------

#include "StdAfx.h"
#include "PaperMercenaryControlWnd.h"
#include "UISoldierSelectWnd.h"
#include "../WndID.h"
#include "../ioMyInfo.h"


//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
PaperMercenaryControlWnd
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
/*
»ý¼º, ÆÄ±«
*/
PaperMercenaryControlWnd::PaperMercenaryControlWnd()
	: PaperMercenaryControlWndBase()
	, m_owner( 0 )
	, m_classtype( -1 )
	, m_sextype( 1 )
	, m_mode( MODE_CHANGE )
{
}

PaperMercenaryControlWnd::~PaperMercenaryControlWnd()
{
}

//----------------------------------------------------------------------------------
/*
ioWnd Override
*/
void	PaperMercenaryControlWnd::OnProcess( float fTimePerSec )
{
	UISoldierSelectWnd* soldierwnd = ( UISoldierSelectWnd* )( g_GUIMgr.FindWnd( UI_SOLDIERSELECT_WND ) );
	if ( soldierwnd && soldierwnd->IsShow() ) return;

	PaperMercenaryControlWndBase::OnProcess( fTimePerSec );
	
	if ( m_backfrm )
	{
		if ( m_changeImgBtn )
		{
			ioHashString imgname( "icon0_3d_front" );

			switch ( m_mode )
			{
			case MODE_CHANGE:
				break;

			case MODE_SEX:
				{
					if ( m_sextype == 1 ) {
						imgname = "icon0_3d_back";
					}
				}
				break;
			}

			ioGUIManager* guimgr = (ioGUIManager*)( ioSP2GUIManager::GetSingletonPtr() );					
			m_changeImgBtn->SetImageSetNormal( "BaseImage004", imgname.c_str(), guimgr );
			m_changeImgBtn->SetImageSetOver( "BaseImage004", imgname.c_str(), guimgr );
			m_changeImgBtn->SetImageSetOverAdd( "BaseImage004", imgname.c_str(), guimgr );
			m_changeImgBtn->SetImageSetPush( "BaseImage004", imgname.c_str(), guimgr );
		}

		bool show = m_backfrm->IsInWndRect( g_GUIMgr.GetLastMousePos() );

		if ( show )
		{
			ShowWnd();

			if ( m_turnLeftImgBtn && m_turnLeftImgBtn->IsClicked() ) {
				OnTurnLeft( m_turnLeftImgBtn, ioWndEXMouseEvent() );
			}
			if ( m_turnRightImgBtn && m_turnRightImgBtn->IsClicked() ) {
				OnTurnRight( m_turnRightImgBtn, ioWndEXMouseEvent() );
			}
		}
		else
		{
			HideWnd();
		}	
	}
}

//----------------------------------------------------------------------------------
/*
ioWnd Event Override
*/
void	PaperMercenaryControlWnd::iwm_command( ioWnd* wnd, int cmd, DWORD param )
{
	PaperMercenaryControlWndBase::iwm_command( wnd, cmd, param );

	if ( !wnd ) return;
	if ( !m_owner ) return;

	switch ( wnd->GetID() )
	{
	case UI_SOLDIERSELECT_WND:
		if ( cmd == IOBN_BTNUP )
		{
			int classtype = g_MyInfo.GetClassType( (int)param );

			m_owner->iwm_command( this, ICT_CHANGE_CHARACTER, classtype );
		}
		break;
	}
}

//----------------------------------------------------------------------------------
/*
Event Virtual Functions
*/
void	PaperMercenaryControlWnd::OnClickChange( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
	switch ( m_mode )
	{
	case MODE_CHANGE:
		{
			UISoldierSelectWnd* soldierwnd = ( UISoldierSelectWnd* )( g_GUIMgr.FindWnd( UI_SOLDIERSELECT_WND ) );
			if ( soldierwnd )
			{
				g_GUIMgr.SetModal( soldierwnd );
				soldierwnd->ShowUISoldierSelectWnd( this );
				int iarray = g_MyInfo.GetClassArray( m_classtype );
				if ( iarray != -1 ) {
					soldierwnd->SetSelectCharArray( iarray );
				}
			}
		}
		break;

	case MODE_SEX:
		{
			if ( m_owner ) {
				m_owner->iwm_command( this, ICT_CHANGE_SEX, ( ( m_sextype == 1 ) ? 2 : 1 ) );
			}
		}
		break;
	}
}

void	PaperMercenaryControlWnd::OnTurnLeft( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
	if ( !m_owner ) return;

	m_owner->iwm_command( this, ICT_TURN_LEFT, 0 );
}

void	PaperMercenaryControlWnd::OnTurnRight( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
	if ( !m_owner ) return;

	m_owner->iwm_command( this, ICT_TURN_RIGHT, 0 );
}