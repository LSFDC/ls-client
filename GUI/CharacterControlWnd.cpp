//----------------------------------------------------------------------------------
/*
Filename	: CharacterControlWnd
Author		: sumin
Date		: 113.11.5_14:41:3
*/
//----------------------------------------------------------------------------------

#include "StdAfx.h"
#include "CharacterControlWnd.h"
#include "ioSP2GUIManager.h"
#include "../ioMyInfo.h"
#include "ioUI3DRender.h"
#include "ioPaperDollWnd.h"
#include "ioLabelWndEX.h"
#include "ioImageWndEX.h"
#include "ioButtonWndEX.h"
#include "ioTabWndEX.h"
#include "../WndID.h"
#include "ioGrowthSelectWnd.h"
#include "UISoldierSelectWnd.h"
#include "../ioMedalItemInfoManager.h"
#include "../ioDecorationPrice.h"
#include "../ioPresentMgr.h"
#include "../ioItemMaker.h"
#include "../ioSkillMaker.h"
#include "../ioGrowthInfoManager.h"
#include "../ioBaseChar.h"

//----------------------------------------------------------------------------------
/*
생성, 파괴
*/
CharacterControlWnd::CharacterControlWnd()
	: CharacterControlWndBase()
	, m_paperdollwnd( 0 )
	, m_curindex( 0 )
	, m_yaw( 0.0f )
	, m_wndTurnLeft( 0 )
	, m_wndTurnRight( 0 )
	, m_wndChangeChar( 0 )
{
}

CharacterControlWnd::~CharacterControlWnd()
{
}

//----------------------------------------------------------------------------------
/*
control
*/
void	CharacterControlWnd::Refresh()
{
	// 일단 tab창만 갱신한다.
	ioTabWndEX* tabwnd = (ioTabWndEX*)(FindChildWndEX( ID_TAB_MAINTAB ));
	if ( tabwnd ) {
		tabwnd->SetCheck( tabwnd->GetCheck(), true );
	}
}

void	CharacterControlWnd::SetPaperDoll( int index )
{
	m_curindex = index;
	SetPaperDoll();
}

#include "ioINILoader.h"
void CharacterControlWnd::SetPaperDoll()
{
	if ( g_MyInfo.GetCharCount() <= 0 ) {
		m_curindex = 0;
	} else if ( m_curindex < 0 ) {
		m_curindex = g_MyInfo.GetCharCount() - 1;
	} else {
		m_curindex %= g_MyInfo.GetCharCount();
	}

	if ( !COMPARE( m_curindex, 0, g_MyInfo.GetCharCount() ) ) {
		return;
	}

	if ( m_paperdollwnd )
	{
		m_yaw = 0.0f;

		ioPaperDoll& doll = m_paperdollwnd->GetPaperDoll();
		
		int classtype = g_MyInfo.GetClassType( m_curindex );
		int idx = g_MyInfo.GetClassArray( classtype );

		ioINILoader kLoader( "config/1_test.ini", false );
		kLoader.SetTitle_e( "const" );

		CHARACTER charinfo = g_MyInfo.GetCharacter( idx );
		g_MyInfo.GetEquipItemCode( charinfo );

		doll.SetChar( "SelectChar", charinfo );
		int iarray = g_MyInfo.GetClassArray( charinfo.m_class_type );
		for ( int i = 0; i < MAX_INVENTORY; ++i ) {
			doll.SetEquip( g_MyInfo.GetCharItem( iarray, i ) );
		}
		doll.SetPosition( D3DXVECTOR3( 0.0f, 0.0f, kLoader.LoadFloat_e( "posz", 0.0f ) ) );
		doll.SetYaw( m_yaw );
		doll.LookAt( doll.GetPosition() + D3DXVECTOR3( 0.0f, kLoader.LoadFloat_e( "h", 0.0f ), 0.0f ) );		
		doll.SetLoopAnimation( 0 );

		m_paperdollwnd->SetPosX( kLoader.LoadInt_e( "x", 0 ) );
		m_paperdollwnd->SetPosY( kLoader.LoadInt_e( "y", 0 ) );
		m_paperdollwnd->SetSize( kLoader.LoadInt_e( "width", 0 ), kLoader.LoadInt_e( "height", 0 ) );

		// 이름을 셋팅
		ioLabelWndEX* name = (ioLabelWndEX*)(FindChildWndEX( ID_LABEL_NAME ));
		if ( name )
		{
			int lv = g_MyInfo.GetClassLevel( classtype, false );
			const char* classname = g_MyInfo.GetClassName( classtype );

			char str[ MAX_PATH ];
			wsprintf( str, "Lv%d %s", lv, classname );

			name->SetLabelText( str );
		}
		name = (ioLabelWndEX*)(FindChildWndEX( ID_LABEL_NAME2 ));
		if ( name )
		{
			char str[ MAX_PATH ];
			SafeSprintf( str, sizeof(str), STR(1), ( ( charinfo.m_ePeriodType == CPT_TIME ) ? STR(2) : STR(3) ) );

			name->SetLabelText( str );
		}

		// 육성 포인트 셋팅
		ioGrowthLevel* glv = g_MyInfo.GetGrowthLevel();
		int gpoint = glv->GetCharGrowthPoint( classtype );

		// ui정보들을 업데이트한다.
		UpdateUpgradeTab();
		UpdateEquipTab();
		UpdateDecoTab();
	}
}

int		CharacterControlWnd::GetCurClassType() const
{
	return g_MyInfo.GetClassType( m_curindex );
}

int		CharacterControlWnd::GetCurSexType() const
{
	if ( !m_paperdollwnd ) return 0;

	const CHARACTER& charinfo = m_paperdollwnd->GetCharacterInfo();
	return charinfo.m_sex - 1;
}

//----------------------------------------------------------------------------------
/*
ioWnd Override
*/
void	CharacterControlWnd::OnProcess( float fTimePerSec )
{
	CharacterControlWndBase::OnProcess( fTimePerSec );

	if ( m_paperdollwnd )
	{
		ioWnd* childs[3] = { m_wndTurnLeft, m_wndTurnRight, m_wndChangeChar };
		bool show = IsInRect( childs, 3, 10, 20, g_GUIMgr.GetLastMousePos() );
	
		for ( int i = 0; i < 3; ++i ) 
		{
			if ( childs[ i ] )
			{
				if ( show ) {
					childs[ i ]->ShowWnd();
				} else {
					childs[ i ]->HideWnd();
				}
			}
		}

		if ( show ) 
		{
			if ( m_wndTurnLeft && m_wndTurnLeft->IsClicked() ) {
				OnTurnLeft( m_wndTurnLeft, ioWndEXMouseEvent() );
			}
			if ( m_wndTurnRight && m_wndTurnRight->IsClicked() ) {
				OnTurnRight( m_wndTurnRight, ioWndEXMouseEvent() );
			}
		}				
	}
}

//----------------------------------------------------------------------------------
/*
ioWnd Event Override
*/
void	CharacterControlWnd::iwm_create()
{
	CharacterControlWndBase::iwm_create();

	if ( m_paperdollwnd ) 
	{
		DeleteChild( m_paperdollwnd->GetID() );
		m_paperdollwnd = 0;
	}

	static const DWORD PDID = 0xFFFF0000;

	m_paperdollwnd = new ioPaperDollWnd();
	m_paperdollwnd->SetID( PDID );
	m_paperdollwnd->SetWndStyle( m_paperdollwnd->GetWndStyle() | IWS_PROCESS );
	m_paperdollwnd->GetPaperDoll().Init( "config/sp2_stage_charselect_map.ini", "shop_cam" );
			
	AddChild( m_paperdollwnd );

	// 버튼들 뒤로 z오더를 내린다.
	MoveZorderLayer2( ID_BTN_CHAR_CHANGE_RIGHT, PDID );

	// 최적화를 위해 미리 캐쉬해둔다.
	m_wndTurnLeft = (ioWndEX*)( FindChildWnd( ID_BTN_TURN_LEFT ) );
	m_wndTurnRight = (ioWndEX*)( FindChildWnd( ID_BTN_TURN_RIGHT ) );
	m_wndChangeChar = (ioWndEX*)( FindChildWnd( ID_BTN_CHANGE_CHAR ) );
}

void	CharacterControlWnd::iwm_show()
{
	CharacterControlWndBase::iwm_show();

	m_curindex = 0;

	if ( m_paperdollwnd )
	{
		int x = GetDerivedPosX();
		int y = GetDerivedPosY();

		m_paperdollwnd->ShowWnd();

		SetPaperDoll();
	}
}

void	CharacterControlWnd::iwm_command( ioWnd* wnd, int cmd, DWORD param )
{
	CharacterControlWndBase::iwm_command( wnd, cmd, param );

	if ( !wnd ) {
		return;
	}

	switch ( wnd->GetID() )
	{
	case UI_SOLDIERSELECT_WND:
		if ( cmd == IOBN_BTNUP ) {
			SetPaperDoll( param );
		}
		break;
	}
}

//----------------------------------------------------------------------------------
/*
Tab 윈도우 업데이트
*/
void	CharacterControlWnd::UpdateUpgradeTab()
{
	int classtype = g_MyInfo.GetClassType( m_curindex );

	SetupGrowthElementUI( ID_IMG_WEAPON_ICON, ID_LAB_WEAPON_TXT, ID_BTN_WEAPON_UP, classtype, 0, true ); // 무기?
	SetupGrowthElementUI( ID_IMG_AMOR_ICON, ID_LAB_AMOR_TXT, ID_BTN_AMOR_UP, classtype, 1, true ); // 갑옷?
	SetupGrowthElementUI( ID_IMG_HELM_ICON, ID_LAB_HELM_TXT, ID_BTN_HELM_UP, classtype, 2, true ); // 투구?
	SetupGrowthElementUI( ID_IMG_CAPE_ICON, ID_LAB_CAPE_TXT, ID_BTN_CAPE_UP, classtype, 3, true ); // 망토?
	SetupGrowthElementUI( ID_IMG_ATTACK_ICON, ID_LAB_ATTACK_TXT, ID_BTN_ATTACK_UP, classtype, 0, false ); // 공격력?
	SetupGrowthElementUI( ID_IMG_DEFENCE_ICON, ID_LAB_DEFENCE_TXT, ID_BTN_DEFENCE_UP, classtype, 1, false ); // 방어력?
	SetupGrowthElementUI( ID_IMG_SPEED_ICON, ID_LAB_SPEED_TXT, ID_BTN_SPEED_UP, classtype, 2, false ); // 속도?
	SetupGrowthElementUI( ID_IMG_DROPDOWN_ICON, ID_LAB_DROPDOWN_TXT, ID_BTN_DROPDOWN_UP, classtype, 3, false ); // 드랍피해감소?
}

void	CharacterControlWnd::SetupGrowthElementUI( int iconid, int labelid, int btnid, int classtype, int slotid, bool isitem )
{
	int level = 0;
	bool con = false;
	if ( isitem ) 
	{
		level = GetItemGrowthLevel( static_cast<ITEM_SLOT_TYPE>( slotid ) ) + GetMedalItemGrowthLevel( static_cast<ITEM_SLOT_TYPE>( slotid ) );
		con = ( g_MyInfo.GetCharItemToClassType( classtype, slotid ).m_item_code > 0 && level > 0 );  // 장착중 && 성장레벨이 0 이상인경우
	}
	else 
	{
		level = GetAbilityGrowthLevel( static_cast<ABILITY_SLOT_TYPE>( slotid ) ) + GetMedalAbilityGrowthLevel( static_cast<ABILITY_SLOT_TYPE>( slotid ) );
		con = ( level > 0 );
	}
		
	ioImageWndEX* img = (ioImageWndEX*)(FindChildWndEX( iconid ));
	ioLabelWndEX* label = (ioLabelWndEX*)(FindChildWndEX( labelid ));
	if ( img && label )
	{
		if ( con )
		{
			img->SetColor( 0xFFFFFFFF );
			img->SetRenderType( UI_RENDER_NORMAL );
			label->SetLabelTextColor( 0xFFFFFFFF );
		}
		else
		{
			img->SetColor( 0xFFC4C4C4 );
			img->SetRenderType( UI_RENDER_ADD );
			label->SetLabelTextColor( 0xFFC4C4C4 );
		}
	}

	ioFrameButtonWndEX* btn = (ioFrameButtonWndEX*)(FindChildWndEX( btnid ));
	if ( btn )
	{
		char str[ MAX_PATH ];		
		if ( level >= FLOAT100 ) {
			wsprintf_e( str, "%d", level );
		} else {
			wsprintf( str, "+%d", level );
		}
		btn->SetLabelText( str );

		if ( g_MyInfo.GetGrowthLevel()->GetCharGrowthPoint( classtype ) > 0 ) {
			btn->SetActive();
		} else {
			btn->SetInActive();
		}
	}
}

void	CharacterControlWnd::ResetMedalUI( ioImageWndEX* img, int idx )
{
	if ( !img ) {
		return;
	}

	int classtype = g_MyInfo.GetClassType( m_curindex );
	int lv = g_MyInfo.GetClassLevel( classtype, false );

	ioGUIManager* guimgr = (ioGUIManager*)( ioSP2GUIManager::GetSingletonPtr() );
	if ( lv / 10 + 1 > idx ) {
		img->SetImageSet( "BaseImage005", "iconslot_5", guimgr );
	} else {
		img->SetImageSet( "BaseImage009", "icon0_medalopen_bt_nor", guimgr );
	}
}

void	CharacterControlWnd::UpdateEquipTab()
{
	if ( !m_paperdollwnd ) {
		return;
	}

	static const int eqimgids[ MAX_INVENTORY ] = 
	{
		ID_IMG_EQIP_AMOR,
		ID_IMG_EQIP_CAPE,
		ID_IMG_EQIP_HELM,
		ID_IMG_EQIP_WEAPON 
	};

	static const int eqtxtids[ MAX_INVENTORY ] = 
	{
		ID_LAB_EQIP_WEAPON_UPGRADE,
		ID_LAB_EQIP_AMOR_UPGRADE,
		ID_LAB_EQIP_HELM_UPGRADE,
		ID_LAB_EQIP_CAPE_UPGRADE 
	};

	// 장비 ui 업데이트
	for ( int i = 0; i < MAX_INVENTORY; ++i )
	{
		ioImageWndEX* img = (ioImageWndEX*)( FindChildWndEX( eqimgids[ i ] ) );
		if ( img )
		{
			EXTRAITEMSLOT slot;
			ioUserExtraItem* extraitem = g_MyInfo.GetUserExtraItem();
			if ( extraitem && extraitem->GetExtraItem( m_paperdollwnd->GetCharacterInfo().m_extra_item[ i ], slot ) && !slot.m_IconName.IsEmpty() ) {
				img->SetImageSet( slot.m_IconName.c_str() );
			} else {
				img->SetImageSet( "BaseImage005", Format( "iconslot_%d", i + 1 ), (ioGUIManager*)( ioSP2GUIManager::GetSingletonPtr() ) );
			}
		}

		ioLabelWndEX* label = (ioLabelWndEX*)( FindChildWndEX( eqtxtids[ i ] ) );
		if ( label )
		{
			int level = GetItemGrowthLevel( static_cast<ITEM_SLOT_TYPE>( i ) );
			if ( level > 0 ) {
				label->SetLabelText( Format( "+%d", level ) );
			} else {
				label->SetLabelText( "" );
			}
		}
	}

	// 메달 ui 업데이트
	static const int mdimgids[ 6 ] = 
	{
		ID_IMG_MEDAL_0,
		ID_IMG_MEDAL_10,
		ID_IMG_MEDAL_20,
		ID_IMG_MEDAL_30,
		ID_IMG_MEDAL_40,
		ID_IMG_MEDAL_50,
	};

	int classtype = g_MyInfo.GetClassType( m_curindex );
	int lv = g_MyInfo.GetClassLevel( classtype, false );

	IntVec medaltypes;
	g_MyInfo.GetMedalItemTypeVec( medaltypes, g_MyInfo.GetClassType( m_curindex ) );
	int nummedal = medaltypes.size();
	for ( int i = 0; i < countof( mdimgids ); ++i ) 
	{
		ioImageWndEX* img = (ioImageWndEX*)( FindChildWndEX( mdimgids[ i ] ) );
		if ( img )
		{
			if ( nummedal > i )
			{
				const ioMedalItemInfoManager::ItemInfo* medalinfo = g_MedalItemMgr.GetItemInfo( medaltypes[ i ] );
				if ( medalinfo && !medalinfo->m_sIcon.IsEmpty() ) {
					img->SetImageSet( medalinfo->m_sIcon.c_str() );
				} else {
					ResetMedalUI( img, i );
				}
			}
			else
			{
				ResetMedalUI( img, i );
			}
		}
	}
}

void	CharacterControlWnd::UpdateDecoTab()
{
	if ( !m_paperdollwnd ) {
		return;
	}

	static const int NUMDECO = 6;
	static const int decoimgids[ NUMDECO ] = 
	{	
		ID_IMG_DECO_KINDRED,
		ID_IMG_DECO_FACE,
		ID_IMG_DECO_FACECOLOR,
		ID_IMG_DECO_HAIR,
		ID_IMG_DECO_HAIRCOLOR,		
		ID_IMG_DECO_PANTS,
	};

	static const int decotypes[ NUMDECO ] = 
	{
		UID_KINDRED,
		UID_FACE,
		UID_SKIN_COLOR,
		UID_HAIR,		
		UID_HAIR_COLOR,
		UID_UNDERWEAR,
	};

	const CHARACTER& charinfo = m_paperdollwnd->GetCharacterInfo();
	int decocodes[ NUMDECO ] =
	{
		charinfo.m_kindred,
		charinfo.m_face,
		charinfo.m_skin_color,
		charinfo.m_hair,
		charinfo.m_hair_color,
		charinfo.m_underwear,
	};
	
	bool ispcroom = g_MyInfo.IsCharExerciseStyle( m_curindex, EXERCISE_PCROOM );

	for ( int i = 0; i < countof( decoimgids ); ++i ) 
	{
		ioImageWndEX* img = (ioImageWndEX*)( FindChildWndEX( decoimgids[ i ] ) );
		if ( img )
		{
			if ( ispcroom )
			{
				img->SetImageSet( "BaseImage009", "icon0_medalopen_bt_nor", (ioGUIManager*)( ioSP2GUIManager::GetSingletonPtr() ) );
			}
			else
			{
				if ( decocodes[ i ] > 0 )
				{
					if ( i == 0 ) // kindred 는 좀 특별하다...;;;;
					{
						img->SetImage( g_MyInfo.GetSoldierIcon( charinfo.m_class_type, ( charinfo.m_sex == 1 ? true : false ) ) );
					}
					else
					{
						const DecoData* data = g_DecorationPrice.GetDecoDataPtr( charinfo.m_sex - 1, decotypes[ i ], decocodes[ i ] ); // 왜!! sex - 1인지는 나도 모른다...;;;
						if ( data ) {
							img->SetImageSet( data->m_szIcon.c_str() );
						} else {
							img->SetImage( 0 );
						}
					}
				}
				else
				{
					img->SetImage( 0 );
				}
			}
		}
	}
}

//----------------------------------------------------------------------------------
/*
기타
*/
int		CharacterControlWnd::GetItemGrowthLevel( ITEM_SLOT_TYPE type )
{
	int classtype = g_MyInfo.GetClassType( m_curindex );

	ioGrowthLevel* growthinfo = g_MyInfo.GetGrowthLevel();

	return growthinfo->GetItemGrowthLevel( classtype, type, true ) + g_MyInfo.GetCharItemToClassType( classtype, type ).m_item_reinforce;
}

int		CharacterControlWnd::GetAbilityGrowthLevel( ABILITY_SLOT_TYPE type )
{
	int classtype = g_MyInfo.GetClassType( m_curindex );

	ioGrowthLevel* growthinfo = g_MyInfo.GetGrowthLevel();

	return growthinfo->GetCharGrowthLevel( g_MyInfo.GetClassType( m_curindex ), type, true ) + g_MyInfo.GetCharItemToClassType( classtype, type ).m_item_reinforce;
}

int		CharacterControlWnd::GetMedalItemGrowthLevel( ITEM_SLOT_TYPE type )
{
	return GetMedalGrowthLevel( type, true );
}

int		CharacterControlWnd::GetMedalAbilityGrowthLevel( ABILITY_SLOT_TYPE type )
{
	return GetMedalGrowthLevel( type, false );
}

int		CharacterControlWnd::GetMedalGrowthLevel( int slot, bool isitem )
{
	int ret = 0;
	int classtype = g_MyInfo.GetClassType( m_curindex );

	IntVec vec;
	g_MyInfo.GetMedalItemTypeVec( vec, classtype );
	int num = vec.size();
	for ( int i = 0; i < num ; ++i ) {
		ret += g_MedalItemMgr.GetMedalItemGrowth( vec[ i ], !isitem, slot );
	}

	return ret;
}

//----------------------------------------------------------------------------------
/*
Event Virtual Functions
*/
void	CharacterControlWnd::ShowUpgradeWnd( int growthtype )
{
	ioGrowthSelectWnd* gwnd = dynamic_cast<ioGrowthSelectWnd*>( g_GUIMgr.FindWnd( GROWTH_SELECT_WND ) );
	if ( gwnd )
	{		
		ioHashString iconname;
		ioHashString slotname;
		ioHashString growthinfoname;
		int growthinfonum = 0;
		int slotidx = ( growthtype - 1 ) % 4;

		if ( growthtype < TIG_ATTACK )
		{
			int classtype = g_MyInfo.GetClassType( m_curindex );
			int itemcode = 0;
			int ary = g_MyInfo.GetClassArray( classtype );
			if ( ary != -1 ) {
				itemcode = g_MyInfo.GetCharItem( ary, slotidx ).m_item_code;
			} else {
				itemcode = slotidx * DEFAULT_BASIC_ITEM_CODE + classtype;
			}
			
			const ioItem* pItem = g_ItemMaker.GetItemConst( itemcode, __FUNCTION__ );
			if ( pItem )
			{
				ioHashString skillname = pItem->GetSkillName();
				const ioSkill* pSkill = g_SkillMaker.GetSkillConst( skillname );
				if ( pSkill )
				{
					iconname = pSkill->GetIconName();
					slotname = pSkill->GetDescName();
				}
				growthinfonum = pItem->GetGrowthInfoNum( 1 );
				ioGrowthInfo* pgInfo = g_GrowthInfoMgr.GetGrowthInfo( growthinfonum );
				if ( pgInfo ) {
					growthinfoname = pgInfo->GetGrowthInfoName();
				}
			}
		}
		else
		{
			growthinfonum = ioBaseChar::m_vGrowthInfoNum[ slotidx ];
		}

		g_GUIMgr.SetModal( gwnd );
		gwnd->SetGrowthSelectInfo( iconname, slotname, growthinfoname, m_paperdollwnd->GetCharacterInfo().m_class_type, growthtype, 0, growthinfonum, this );
		gwnd->UpdateInfo();
	}
}

//
void	CharacterControlWnd::OnTabMain( ioWndEX* wnd, ioWndEXRadioEvent& event )
{
	switch ( event.m_index )
	{
	case 0: // 육성
		UpdateUpgradeTab();
		break;

	case 1: // 장비
		UpdateEquipTab();
		break;

	case 2: // 치장
		UpdateDecoTab();
		break;
	}
}

void	CharacterControlWnd::OnAmorOver( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
}

void	CharacterControlWnd::OnAmorLeave( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
}

void	CharacterControlWnd::OnAmorUpgrade( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
	ShowUpgradeWnd( TIG_ARMOR );
}

void	CharacterControlWnd::OnAttackOver( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
}

void	CharacterControlWnd::OnAttackLeave( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
}

void	CharacterControlWnd::OnAttackUpgrade( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
	ShowUpgradeWnd( TIG_ATTACK );
}

void	CharacterControlWnd::OnCapeOver( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
}

void	CharacterControlWnd::OnCapeLeave( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
}

void	CharacterControlWnd::OnCapeUpgrade( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
	ShowUpgradeWnd( TIG_CLOAK );
}

void	CharacterControlWnd::OnDefenceOver( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
}

void	CharacterControlWnd::OnDefenceLeave( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
}

void	CharacterControlWnd::OnDefenceUpgrade( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
	ShowUpgradeWnd( TIG_DEFENCE );
}

void	CharacterControlWnd::OnDropDownOver( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
}

void	CharacterControlWnd::OnDropDownLeave( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
}

void	CharacterControlWnd::OnDropDownUpgrade( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
	ShowUpgradeWnd( TIG_DROP );
}

void	CharacterControlWnd::OnHelmOver( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
}

void	CharacterControlWnd::OnHelmLeave( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
}

void	CharacterControlWnd::OnHelmUpgrade( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
	ShowUpgradeWnd( TIG_HELMET );
}

void	CharacterControlWnd::OnSpeedOver( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
}

void	CharacterControlWnd::OnSpeedLeave( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
}

void	CharacterControlWnd::OnSpeedUpgrade( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
	ShowUpgradeWnd( TIG_MOVE );
}

void	CharacterControlWnd::OnWaeponOver( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
}

void	CharacterControlWnd::OnWaeponLeave( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
}

void	CharacterControlWnd::OnWaeponUpgrade( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
	ShowUpgradeWnd( TIG_WEAPON );
}

void	CharacterControlWnd::OnLeftChangeCharacter( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
	SetPaperDoll( m_curindex - 1 );
}

void	CharacterControlWnd::OnRightChangeCharacter( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
	SetPaperDoll( m_curindex + 1 );
}

void	CharacterControlWnd::OnChangeCharacter( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
	if ( m_paperdollwnd )
	{
		UISoldierSelectWnd* soldierwnd = ( UISoldierSelectWnd* )( g_GUIMgr.FindWnd( UI_SOLDIERSELECT_WND ) );
		if ( soldierwnd )
		{
			g_GUIMgr.SetModal( soldierwnd );
			soldierwnd->ShowUISoldierSelectWnd( this );
			int iarray = g_MyInfo.GetClassArray( m_paperdollwnd->GetCharacterInfo().m_class_type );
			if ( iarray != -1 ) {
				soldierwnd->SetSelectCharArray( iarray );
			}
		}
	}
}

void	CharacterControlWnd::OnTurnLeft( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
	if ( m_paperdollwnd )
	{
		m_yaw += FLOAT1;
		m_paperdollwnd->GetPaperDoll().SetYaw( m_yaw );
	}
}

void	CharacterControlWnd::OnTurnRight( ioWndEX* wnd, ioWndEXMouseEvent& event )
{
	if ( m_paperdollwnd )
	{
		m_yaw -= FLOAT1;
		m_paperdollwnd->GetPaperDoll().SetYaw( m_yaw );
	}
}