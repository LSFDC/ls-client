//----------------------------------------------------------------------------------
/*
Filename	: MercenaryEquipInfoTabWnd
Author		: sumin
Date		: 113.11.5_14:41:3
*/
//----------------------------------------------------------------------------------

#include "StdAfx.h"
#include "MercenaryEquipInfoTabWnd.h"
#include "ioButtonWndEX.h"
#include "ioLabelWndEX.h"
#include "ioFrameWndEX.h"
#include "../ioPresentMgr.h"
#include "../ioItemMaker.h"
#include "../ioSkillMaker.h"
#include "NewShopBuyWnd.h"
#include "../CharacterInfo.h"
#include "ioPaperDollWnd.h"
#include "ShopWnd.h"
#include "CharacterControlWnd.h"
#include "UISoldierSelectWnd.h"
#include "../WndID.h"
#include "../ioMyInfo.h"
#include "PaperMercenaryControlWnd.h"

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
ShopBuyTabCommon
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
/*
생성 / 소멸
*/
ShopBuyTabCommon::ShopBuyTabCommon()
	: m_shopwnd( 0 )
	, m_paperdollwnd( 0 )
	, m_yaw( 0.0f )
{
}

ShopBuyTabCommon::~ShopBuyTabCommon()
{
}

//----------------------------------------------------------------------------------
/*
control
*/
void	ShopBuyTabCommon::SetItemInfo( const ShopItemInfoEX& info )
{
	m_iteminfo = info;
}
	
//----------------------------------------------------------------------------------
/*
ETC
*/
#include "ioINILoader.h"
void	ShopBuyTabCommon::SetPaperDollWnd( ioWnd* tabbtn, ioWnd* refwnd, int insertid, int classtype )
{
	if ( !tabbtn ) return;
	if ( !refwnd ) return;

	if ( !m_paperdollwnd )
	{
		static const DWORD PDID = 0xFFFF0000;

		m_paperdollwnd = new ioPaperDollWnd();
		m_paperdollwnd->SetID( PDID );
		m_paperdollwnd->SetWndStyle( m_paperdollwnd->GetWndStyle() | IWS_PROCESS );
		m_paperdollwnd->GetPaperDoll().Init( "config/sp2_stage_charselect_map.ini", "shop_cam" );
	
		tabbtn->AddChild( m_paperdollwnd );
		tabbtn->MoveZorderLayer2( insertid, m_paperdollwnd->GetID() );

		if ( refwnd )
		{
			m_paperdollwnd->SetPosX( refwnd->GetPosX() );
			m_paperdollwnd->SetPosY( refwnd->GetPosY() );
			m_paperdollwnd->SetSize( refwnd->GetWidth(), refwnd->GetHeight() );
		}
	}

	SetPaperDoll( true, classtype );
}

void	ShopBuyTabCommon::SetPaperDoll( bool male, int classtype )
{
	if ( !m_paperdollwnd ) return;

	m_yaw = 0.0f;

	ioINILoader kLoader( "config/1_test.ini", false );
	kLoader.SetTitle_e( "const" );
		
	CHARACTER charinfo( m_paperdollwnd->GetCharacterInfo() );
	NewShopBuyWnd::GetDefaultCharInfo( charinfo, classtype, ( male ? 0 : 1 ) );
	ioPaperDoll& doll = m_paperdollwnd->GetPaperDoll();
	doll.SetChar( "Char", charinfo );
	doll.SetPosition( D3DXVECTOR3( 0.0f, 0.0f, kLoader.LoadFloat_e( "posz", 0.0f ) ) );
	doll.SetYaw( m_yaw );
	doll.LookAt( doll.GetPosition() + D3DXVECTOR3( 0.0f, kLoader.LoadFloat_e( "h", 0.0f ), 0.0f ) );		
	doll.SetEquip();
	doll.SetLoopAnimation( 0 );

	m_paperdollwnd->ShowWnd();
}

void	ShopBuyTabCommon::TurnLeft()
{
	if ( m_paperdollwnd )
	{
		m_yaw += FLOAT1;
		m_paperdollwnd->GetPaperDoll().SetYaw( m_yaw );
	}
}

void	ShopBuyTabCommon::TurnRight()
{
	if ( m_paperdollwnd )
	{
		m_yaw -= FLOAT1;
		m_paperdollwnd->GetPaperDoll().SetYaw( m_yaw );
	}
}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
MercenaryEquipInfoTabWnd
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
/*
생성, 파괴
*/
MercenaryEquipInfoTabWnd::MercenaryEquipInfoTabWnd()
	: MercenaryEquipInfoTabWndBase()
	, ShopBuyTabCommon()
	, m_paperctrlwnd( 0 )
{
}

MercenaryEquipInfoTabWnd::~MercenaryEquipInfoTabWnd()
{
}

//----------------------------------------------------------------------------------
/*
ioWnd Override
*/
void	MercenaryEquipInfoTabWnd::OnProcess( float fTimePerSec )
{
	MercenaryEquipInfoTabWndBase::OnProcess( fTimePerSec );

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
void	MercenaryEquipInfoTabWnd::iwm_create()
{
	MercenaryEquipInfoTabWndBase::iwm_create();

	m_paperctrlwnd = (PaperMercenaryControlWnd*)( g_GUIMgr.AddWnd( "xml/PaperMercenaryControlWnd.xml", this ) );
	if ( m_paperctrlwnd )
	{
		m_paperctrlwnd->SetOwner( this );
		m_paperctrlwnd->SetPosX( FLOAT100 );
		m_paperctrlwnd->SetPosY( 320 );
		m_paperctrlwnd->HideWnd();
	}
}

void	MercenaryEquipInfoTabWnd::iwm_command( ioWnd* wnd, int cmd, DWORD param )
{
	MercenaryEquipInfoTabWndBase::iwm_command( wnd, cmd, param );

	if ( !wnd ) return;

	switch ( wnd->GetID() )
	{
	case PAPER_CTRL_WND:
		if ( PaperMercenaryControlWnd::ICT_CHANGE_CHARACTER == cmd )
		{
			SetPaperDollWnd( m_infotabbutton, m_backimg, m_paperctrlwnd->GetID(), param );
			if ( m_paperdollwnd && m_iteminfo.m_itemcode != -1 )
			{
				ioPaperDoll& doll = m_paperdollwnd->GetPaperDoll();
				doll.SetEquip( g_ItemMaker.GetItemConst( m_iteminfo.m_itemcode, __FUNCTION__ ) );
			}
		}
		else if ( PaperMercenaryControlWnd::ICT_CHANGE_SEX == cmd )
		{
			if ( m_paperdollwnd ) {
				SetPaperDoll( m_paperdollwnd->GetCharacterInfo().m_sex != 1, m_paperdollwnd->GetCharacterInfo().m_class_type );
			}
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
void	MercenaryEquipInfoTabWnd::SetItemInfo( const ShopItemInfoEX& info )
{
	ShopBuyTabCommon::SetItemInfo( info );

	switch ( m_iteminfo.m_iItemType )
	{
	case PRESENT_SOLDIER:
		{
			if ( m_paperctrlwnd ) {
				m_paperctrlwnd->SetMode( PaperMercenaryControlWnd::MODE_SEX );
			}

			int classtype = m_iteminfo.m_iValue1;

			// 정보 tab
			{
				if ( m_backimg ) {
					//m_backimg->SetImage();
				}

				if ( m_lefttoplabel ) {
					m_lefttoplabel->SetLabelText( STR(1) );
				}

				if ( m_righttoplabel ) {
					m_righttoplabel->SetLabelText( STR(2) );
				}

				SetPaperDollWnd( m_infotabbutton, m_backimg, m_paperctrlwnd->GetID(), classtype );
			}

			// 스킬영상 tab
			{
				if ( m_flashplayer ) {
					m_flashplayer->Load( Format( "hero_%d.swf", classtype ) );
				}
								
				for ( int i = 0; i < MAX_INVENTORY; ++i )
				{
					int itemcode = ( DEFAULT_BASIC_ITEM_CODE * i ) + classtype;
					const ioItem* pItem = g_ItemMaker.GetItemConst( itemcode, __FUNCTION__ );
					if ( pItem )
					{
						const ioSkill* pSkill = g_SkillMaker.GetSkillConst( pItem->GetSkillName() );
						if ( pSkill )
						{
							if ( m_skilltypelabel[ i ] ) {								
								m_skilltypelabel[ i ]->SetLabelText( pSkill->GetDescName().c_str() );
							}

							if ( m_skilldesclabel[ i ] ) {									
								m_skilldesclabel[ i ]->SetLabelText( pSkill->GetDesc().c_str() );
							}

							if ( m_skillicon[ i ] ) {
								m_skillicon[ i ]->SetImageSet( pSkill->GetIconName().c_str() );
							}
						}
					}
				}
			}
		}
		break;

	case PRESENT_EXTRAITEM:
	case PRESENT_EXTRAITEM_BOX:
		{
			if ( m_paperctrlwnd ) {
				m_paperctrlwnd->SetMode( PaperMercenaryControlWnd::MODE_CHANGE );
			}

			int classtype = m_iteminfo.m_iValue1;
			if ( m_shopwnd && m_shopwnd->GetCharControlWnd() ) {
				classtype = m_shopwnd->GetCharControlWnd()->GetCurClassType();
			}

			// 정보 tab
			{
				if ( m_backimg ) {
					//m_backimg->SetImage();
				}

				if ( m_lefttoplabel ) {
					m_lefttoplabel->SetLabelText( STR(3) );
				}

				if ( m_righttoplabel ) {
					m_righttoplabel->SetLabelText( STR(4) );
				}

				SetPaperDollWnd( m_infotabbutton, m_backimg, m_paperctrlwnd->GetID(), classtype );

				if ( m_paperdollwnd && m_iteminfo.m_itemcode != -1 )
				{
					ioPaperDoll& doll = m_paperdollwnd->GetPaperDoll();
					doll.SetEquip( g_ItemMaker.GetItemConst( m_iteminfo.m_itemcode, __FUNCTION__ ) );
				}
			}

			// 스킬영상 tab
			{
				if ( m_flashplayer ) {
					m_flashplayer->Load( Format( "hero_%dth_skill_flv0%d.swf", classtype, 0 ) );
				}

				for ( int i = 0; i < MAX_INVENTORY; ++i )
				{
					if ( m_skilltypelabel[ i ] ) {
						m_skilltypelabel[ i ]->HideWnd();
					}

					if ( m_skilldesclabel[ i ] ) {			
						m_skilldesclabel[ i ]->HideWnd();
					}

					if ( m_skillicon[ i ] ) {
						m_skillicon[ i ]->HideWnd();
					}
				}
			}
		}
		break;
	}
}