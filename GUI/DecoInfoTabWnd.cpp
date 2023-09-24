//----------------------------------------------------------------------------------
/*
Filename	: DecoInfoTabWnd
Author		: sumin
Date		: 113.11.5_14:41:3
*/
//----------------------------------------------------------------------------------

#include "StdAfx.h"
#include "DecoInfoTabWnd.h"
#include "ioButtonWndEX.h"
#include "ioLabelWndEX.h"
#include "ioFrameWndEX.h"
#include "../ioPresentMgr.h"
#include "../ioItemMaker.h"
#include "../ioSkillMaker.h"
#include "ShopWnd.h"
#include "CharacterControlWnd.h"
#include "ioPaperDollWnd.h"
#include "../ioDecorationPrice.h"
#include "../ioClassPrice.h"
#include "../ioMyInfo.h"
#include "../ioSetItemInfoManager.h"
#include "../ioSetItemInfo.h"
#include "../WndID.h"
#include "PaperMercenaryControlWnd.h"

//----------------------------------------------------------------------------------
/*
static 
*/
void	DecoInfoTabWnd::GetDecoClassTypesFromMyInfo( std::vector< int >& out )
{
	out.clear();

	int maxnum =  g_ClassPrice.MaxClassPrice();
	for ( int i = 0; i < maxnum; ++i )
	{
		int iarray = g_MyInfo.GetCharSlotIndexToArray( i );
		if ( iarray == -1 ) continue;
		if ( g_MyInfo.IsCharExercise( iarray ) ) continue;       

		int idx = g_MyInfo.GetClassType( iarray ) - 1;
		const ioSetItemInfo* pInfo = g_SetItemInfoMgr.GetSetInfoByIdx( idx );
		if ( !pInfo ) continue;

		out.push_back( pInfo->GetSetCode() - SET_ITEM_CODE );
	}
}

int		DecoInfoTabWnd::GetDecoClassTypesFromMyInfo()
{
	std::vector< int > out;
	GetDecoClassTypesFromMyInfo( out );

	if ( out.empty() ) return 0;
	return out[ 0 ];
}

int		DecoInfoTabWnd::GetDecoSexTypeFromMyInfo()
{
	int idx = g_MyInfo.GetClassArray( GetDecoClassTypesFromMyInfo() );
	if ( idx < 0 ) return 0;
	if ( idx >= g_MyInfo.GetCharCount() ) return 0;

	return g_MyInfo.GetCharacter( idx ).m_sex - 1;
}

//----------------------------------------------------------------------------------
/*
»ý¼º, ÆÄ±«
*/
DecoInfoTabWnd::DecoInfoTabWnd()
	: DecoInfoTabWndBase()
	, ShopBuyTabCommon()
	, m_isshop( true )
	, m_paperctrlwnd( 0 )
{
}

DecoInfoTabWnd::~DecoInfoTabWnd()
{
}

//----------------------------------------------------------------------------------
/*
ioWnd Override
*/
void	DecoInfoTabWnd::OnProcess( float fTimePerSec )
{
	DecoInfoTabWndBase::OnProcess( fTimePerSec );

	if ( m_paperctrlwnd && m_paperdollwnd && m_shopwnd && m_shopwnd->GetCharControlWnd() )
	{
		int classtype = m_shopwnd->GetCharControlWnd()->GetCurClassType();

		m_paperctrlwnd->SetClassType( classtype );
		m_paperctrlwnd->SetSexType( m_paperdollwnd->GetCharacterInfo().m_sex );

		m_paperctrlwnd->OnProcess( fTimePerSec );
	}
}

//----------------------------------------------------------------------------------
/*
ioWnd Event Override
*/
void	DecoInfoTabWnd::iwm_create()
{
	DecoInfoTabWndBase::iwm_create();

	m_paperctrlwnd = (PaperMercenaryControlWnd*)( g_GUIMgr.AddWnd( "xml/PaperMercenaryControlWnd.xml", this ) );
	if ( m_paperctrlwnd )
	{
		m_paperctrlwnd->SetOwner( this );
		m_paperctrlwnd->SetPosX( FLOAT100 );
		m_paperctrlwnd->SetPosY( 320 );
		m_paperctrlwnd->HideWnd();
	}
}

void	DecoInfoTabWnd::iwm_command( ioWnd* wnd, int cmd, DWORD param )
{
	DecoInfoTabWndBase::iwm_command( wnd, cmd, param );

	if ( !wnd ) return;

	switch ( wnd->GetID() )
	{
	case PAPER_CTRL_WND:
		if ( PaperMercenaryControlWnd::ICT_CHANGE_CHARACTER == cmd )
		{
			SetPaperDollWnd( m_infotabbutton, m_backimg, m_paperctrlwnd->GetID(), param );
			SetDeco();
		}
		else if ( PaperMercenaryControlWnd::ICT_TURN_LEFT == cmd )
		{
			TurnLeft();
		}
		else if ( PaperMercenaryControlWnd::ICT_TURN_RIGHT == cmd )
		{
			TurnRight();
		}
		break;
	}
}

//----------------------------------------------------------------------------------
/*
control
*/
void	DecoInfoTabWnd::SetItemInfo( const ShopItemInfoEX& info )
{
	ShopBuyTabCommon::SetItemInfo( info );

	switch ( m_iteminfo.m_iItemType )
	{
	case PRESENT_DECORATION:
		{
			if ( m_paperctrlwnd ) {
				m_paperctrlwnd->SetMode( PaperMercenaryControlWnd::MODE_CHANGE );
			}

			int classtype = m_iteminfo.m_iValue1;
			if ( m_shopwnd && m_shopwnd->GetCharControlWnd() ) {
				classtype = m_shopwnd->GetCharControlWnd()->GetCurClassType();
			}

			if ( m_backimg ) {
				//m_backimg->SetImage();
			}

			if ( m_lefttoplabel ) {
				m_lefttoplabel->SetLabelText( STR(1) );
			}

			if ( m_righttoplabel ) {
				if ( m_isshop ) {
					m_righttoplabel->SetLabelText( STR(2) );
				} else {
					m_righttoplabel->SetLabelText( STR(3) );
				}
			}

			SetPaperDollWnd( m_infotabbutton, m_backimg, m_paperctrlwnd->GetID(), classtype );
			SetDeco();
		}
		break;
	}
}

void	DecoInfoTabWnd::SetDeco()
{
	if ( m_paperdollwnd && m_iteminfo.m_itemcode != -1 )
	{
		ioPaperDoll& doll = m_paperdollwnd->GetPaperDoll();

		CHARACTER charinfo = doll.GetCharacterInfo();

		switch ( m_iteminfo.m_iValue1 )
		{
		case UID_FACE:
			charinfo.m_face = m_iteminfo.m_iValue2; 
			break;

		case UID_HAIR:
			charinfo.m_hair = m_iteminfo.m_iValue2;
			doll.UnEquip( ioPaperDoll::ET_HELMET );
			break;

		case UID_UNDERWEAR:
			charinfo.m_underwear = m_iteminfo.m_iValue2;
			doll.UnEquip( ioPaperDoll::ET_WEAPON );
			doll.UnEquip( ioPaperDoll::ET_ARMOR );
			break;

		case UID_SKIN_COLOR:
			charinfo.m_skin_color = m_iteminfo.m_iValue2;
			break;

		case UID_HAIR_COLOR:
			charinfo.m_hair_color = m_iteminfo.m_iValue2;
			doll.UnEquip( ioPaperDoll::ET_HELMET );
			break;

		case UID_KINDRED:
			charinfo.m_kindred = m_iteminfo.m_iValue2;
			break;
		}

		doll.SetDeco( charinfo );
	}
}