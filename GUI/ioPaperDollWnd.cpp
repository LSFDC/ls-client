//----------------------------------------------------------------------------------
/*
Filename	: ioPaperDollWnd
Author		: sumin
Date		: 113.11.5_14:41:3
*/
//----------------------------------------------------------------------------------

#include "StdAfx.h"
#include "ioPaperDollWnd.h"
#include "ioSP2GUIManager.h"
#include "ioUI3DRender.h"
#include "../ioMyInfo.h"
#include "../ioEtcItem.h"
#include "../ioEtcItemManager.h"
#include "../ioDecorationPrice.h"
#include "../ioItemMaker.h"

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
ioPaperDoll
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
/*
생성 / 소멸
*/
ioPaperDoll::ioPaperDoll()
	: m_instancename( "" )
	, m_3drenderer( 0 )
	, m_char( 0 )	
{
}

ioPaperDoll::~ioPaperDoll()
{
	SAFEDELETE( m_3drenderer );
	SAFEDELETE( m_char );
}

//----------------------------------------------------------------------------------
/*
control
*/
void	ioPaperDoll::Init( const char* INIFilename, const char* caminfo )
{
	if ( !INIFilename || !caminfo ) {
		return;
	}

	SAFEDELETE( m_3drenderer );

	m_3drenderer = new ioUI3DRender;
	m_3drenderer->Initialize( INIFilename, caminfo );
}

void	ioPaperDoll::SetChar( const char* instancename, const CHARACTER& charinfo )
{
	if ( !m_3drenderer || !instancename ) {
		return;
	}

	if ( charinfo.m_class_type <= 0 ) {
		return;
	}
	
	SAFEDELETE( m_char );
	m_instancename = "";
	
	m_char = m_3drenderer->CreateChar( charinfo, instancename );
	if ( !m_char ) {
		return;
	}

	m_instancename = instancename;

	m_char->SetPosition( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );

	m_charinfo = charinfo;
}

void	ioPaperDoll::SetEquip()
{
	if ( !m_char ) {
		return;
	}
	
	for ( int i = 0; i < MAX_INVENTORY; ++i )
	{
		int icode = ( DEFAULT_BASIC_ITEM_CODE * i ) + m_charinfo.m_class_type;
		m_char->EquipItem( icode, 0, 0 );
	}	
}

void	ioPaperDoll::SetEquip( const ITEM_DATA& item )
{
	if ( !m_char ) {
		return;
	}

	// 코스튬 관련 (??)
	const ioItem* pItem = g_ItemMaker.GetItemConst( item.m_item_code, __FUNCTION__ );
	if ( pItem ) 
	{
		int nSubType = pItem->GetType()-1;
		int nCostumeCode = 0;
		DWORD dwItemMaleCustom = item.m_item_male_custom;
		DWORD dwItemFemaleCustom = item.m_item_female_custom;
		if ( COMPARE( nSubType, 0, MAX_INVENTORY ) )
		{
			nCostumeCode = m_charinfo.m_EquipCostume[nSubType].m_CostumeCode;
			if ( nCostumeCode > 0 )
			{
				dwItemMaleCustom = m_charinfo.m_EquipCostume[nSubType].m_Costume_Male_Custom;
				dwItemFemaleCustom = m_charinfo.m_EquipCostume[nSubType].m_Costume_Female_Custom;
			}
		}

		m_char->EquipItem( pItem->GetItemCode(), dwItemMaleCustom, dwItemFemaleCustom, nCostumeCode );
	}
}

void	ioPaperDoll::SetEquip( const ioItem* item )
{
	if ( !m_char ) return;
	if ( !item ) return;

	int nSubType = item->GetType()-1;
	int nCostumeCode = 0;
	DWORD dwItemMaleCustom = item->GetItemMaleCustom();
	DWORD dwItemFemaleCustom = item->GetItemFemaleCustom();
	if ( COMPARE( nSubType, 0, MAX_INVENTORY ) )
	{
		nCostumeCode = m_charinfo.m_EquipCostume[nSubType].m_CostumeCode;
		if ( nCostumeCode > 0 )
		{
			dwItemMaleCustom = m_charinfo.m_EquipCostume[nSubType].m_Costume_Male_Custom;
			dwItemFemaleCustom = m_charinfo.m_EquipCostume[nSubType].m_Costume_Female_Custom;
		}
	}
	// 코스튬 관련 (??)
	m_char->EquipItem( item->GetItemCode(), dwItemMaleCustom, dwItemFemaleCustom, nCostumeCode );
}

void	ioPaperDoll::UnEquip( EQUIP_TYPE type )
{
	if ( !m_char ) return;

	m_char->ReleaseItem( type );
}

void	ioPaperDoll::SetDeco( const CHARACTER& info )
{
	if ( !m_char ) return;
	if ( !m_3drenderer ) return;

	m_charinfo.m_beard = info.m_beard;
	m_charinfo.m_face = info.m_face;
	m_charinfo.m_hair = info.m_hair;
	m_charinfo.m_accessories = info.m_accessories;
	m_charinfo.m_skin_color = info.m_skin_color;
	m_charinfo.m_hair_color = info.m_hair_color;
	m_charinfo.m_underwear = info.m_underwear;

	m_3drenderer->ChangeDeco( m_char, m_charinfo );
}

//void	ioPaperDoll::SetMyChar( const char* instancename, int classtype, int sextype )
//{	
//	SetMyCharInfo( classtype, sextype );
//
//	if ( !m_3drenderer || !instancename ) {
//		return;
//	}
//
//	if ( m_charinfo.m_class_type <= 0 ) {
//		return;
//	}
//
//	SAFEDELETE( m_char );
//	
//	m_char = m_3drenderer->CreateChar( m_charinfo, instancename );
//	if ( !m_char ) {
//		return;
//	}
//
//	m_char->SetPosition( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
//}
//
//void	ioPaperDoll::SetMyCharInfo( int classtype, int sextype )
//{
//	int index = g_MyInfo.GetClassArray( classtype );
//	
//	if ( COMPARE( index, 0, g_MyInfo.GetCharCount() ) )
//	{
//		CHARACTER charinfo = g_MyInfo.GetCharacter( index );
//		if ( ( charinfo.m_sex - 1 ) == sextype )
//		{
//			m_charinfo = charinfo;
//			return;
//		}
//	}
//
//	SetMyDefaultCharInfo( classtype, sextype );
//}
//
//void	ioPaperDoll::SetMyDefaultCharInfo( int classtype, int sextype )
//{
//	m_charinfo.m_beard = 1;
//	m_charinfo.m_accessories = 1;
//
//	m_charinfo.m_class_type = classtype;
//
//	// 인벤토리에 있는 아이템 세팅
//	g_MyInfo.GetEquipItemCode( m_charinfo ); 
//
//	// -1이아니면 인자값을 셋팅한다.
//	if ( sextype != -1) 
//	{
//		// 성별이 맞지 않는 정보가 설정되었다면 초기화
//		if ( m_charinfo.m_sex != sextype + 1 )
//		{
//			m_charinfo.m_sex = sextype + 1;
//			m_charinfo.m_face = -1;
//			m_charinfo.m_hair = -1;
//			m_charinfo.m_skin_color = -1;
//			m_charinfo.m_hair_color = -1;
//			m_charinfo.m_underwear  = -1;
//		}
//	}
//
//	int decosextype = m_charinfo.m_sex - 1;
//	int defrandseed = g_MyInfo.GetPrivateID().GetHashCode() + m_charinfo.m_class_type;
//
//	// 얼굴 UID_FACE
//	if ( m_charinfo.m_face == -1 ) {
//		m_charinfo.m_face = g_DecorationPrice.GetDefaultDecoCode( decosextype, UID_FACE, defrandseed + UID_FACE );
//	}
//
//	// 머리 UID_HAIR
//	if ( m_charinfo.m_hair == -1 ) {
//		m_charinfo.m_hair = g_DecorationPrice.GetDefaultDecoCode( decosextype, UID_HAIR, defrandseed + UID_HAIR );
//	}
//
//	// 피부색 UID_SKIN_COLOR
//	if ( m_charinfo.m_skin_color == -1 ) {
//		m_charinfo.m_skin_color = g_DecorationPrice.GetDefaultDecoCode( decosextype, UID_SKIN_COLOR, defrandseed + UID_SKIN_COLOR );
//	}
//
//	// 머리색 UID_HAIR_COLOR
//	if ( m_charinfo.m_hair_color == -1 ) {
//		m_charinfo.m_hair_color = g_DecorationPrice.GetDefaultDecoCode( decosextype, UID_HAIR_COLOR, defrandseed + UID_HAIR_COLOR );
//	}
//
//	// 속옷 UID_UNDERWEAR
//	if ( m_charinfo.m_underwear == -1 ) {
//		m_charinfo.m_underwear = g_DecorationPrice.GetDefaultDecoCode( decosextype, UID_UNDERWEAR, defrandseed + UID_UNDERWEAR );
//	}
//}

void	ioPaperDoll::SetPosition( const D3DXVECTOR3& pos )
{
	if ( !m_char ) {
		return;
	}

	m_char->SetPosition( pos );
}

const D3DXVECTOR3&	ioPaperDoll::GetPosition() const
{
	static D3DXVECTOR3 ret( 0.0f, 0.0f, 0.0f );
	if ( !m_char ) 
	{
		ret = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		return ret;
	}

	ret = m_char->GetWorldPosition();
	return ret;
}

void	ioPaperDoll::SetScale( const D3DXVECTOR3& scale )
{
	if ( !m_char ) {
		return;
	}

	m_char->SetScale( scale );
}

void	ioPaperDoll::SetYaw( float yaw )
{
	if ( !m_char ) {
		return;
	}

	m_char->SetYaw( yaw );
}

void	ioPaperDoll::AttachEffect( const char* filename )
{
	if ( !m_char ) {
		return;
	}

	m_char->AttachEffect( filename, NULL );
}

// animation
void	ioPaperDoll::SetWeaponShopAnimation()
{
	if ( !m_char ) {
		return;
	}

	m_char->SetWeaponShopMotion();
}

void	ioPaperDoll::SetLoopAnimation( int aniidx, float fadein )
{
	if ( !m_char || !m_3drenderer ) {
		return;
	}

	ioHashString anim = m_3drenderer->GetModelAnimation( 0, m_char->IsMale() );
	if ( !anim.IsEmpty() ) {
		m_char->SetLoopAni( anim, fadein );
	}
}

void	ioPaperDoll::SetLoopAnimation( const ioHashString& aniname, float fadein )
{
	if ( !m_char || aniname.IsEmpty() ) {
		return;
	}
	
	m_char->SetLoopAni( aniname, fadein );
}

void	ioPaperDoll::SetActionAnimation( const ioHashString& aniname, float fadein, float fadeout )
{
	if ( !m_char || aniname.IsEmpty() ) {
		return;
	}
	
	m_char->SetActionAni( aniname, fadein, fadeout );
}

void	ioPaperDoll::SetItemAnimation( DWORD item )
{
	if ( !m_char ) {
		return;
	}

	if 
	(
		!COMPARE( item, ioEtcItem::EIT_ETC_MOTION1, ioEtcItem::EIT_ETC_MOTION100 + 1 ) 
		&&
		!COMPARE( item, ioEtcItem::EIT_ETC_MOTION101, ioEtcItem::EIT_ETC_MOTION400 + 1 )
	)
	{
		return;
	}

	ioEtcItemMotion* etcitemmotion = dynamic_cast<ioEtcItemMotion*>( g_EtcItemMgr.FindEtcItem( item ) );
	if ( etcitemmotion )
	{
		if ( m_char->IsMale() ) {
			m_char->SetMotionItemAni( etcitemmotion->GetHumanManMotion() );
		} else {
			m_char->SetMotionItemAni( etcitemmotion->GetHumanWomanMotion() );
		}
	}
}

void	ioPaperDoll::SetItemAnimation( const ioHashString& aniname )
{
	if ( !m_char || aniname.IsEmpty() ) {
		return;
	}
	
	m_char->SetMotionItemAni( aniname );
}

// camera
void	ioPaperDoll::LookAt( const D3DXVECTOR3& pos )
{
	if ( !m_3drenderer ) {
		return;
	}

	m_3drenderer->SetLookAt( pos );
}

//----------------------------------------------------------------------------------
/*
render
*/
void	ioPaperDoll::Update( float dt )
{
	if ( m_char ) {
		m_char->Update( dt );
	}
}

void	ioPaperDoll::Render( int x, int y, float width, float height )
{
	if ( m_3drenderer )
	{
		ioCamera* camera = m_3drenderer->GetCamera();
		if ( camera ) {
			m_3drenderer->SetProjection( camera->GetNearP(), camera->GetFarP(), camera->GetFovY(), width / height );
		}

		D3DRECT vport;
		vport.x1 = x;
		vport.y1 = y;
		vport.x2 = x + width;
		vport.y2 = y + height;
		
		m_3drenderer->RenderViewPort( &vport, true );
	}
}



//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
ioPaperDollWnd
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
/*
생성 / 소멸
*/
ioPaperDollWnd::ioPaperDollWnd()
	: ioWndEX()
	, m_dolloffsetx( 0 )
	, m_dolloffsety( 0 )
{
}

ioPaperDollWnd::~ioPaperDollWnd()
{
}

//----------------------------------------------------------------------------------
/*
ioWnd Override
*/
void	ioPaperDollWnd::OnProcess( float dt )
{
	if ( !IsShow() ) {
		return;
	}

	m_doll.Update( dt );
}

void	ioPaperDollWnd::OnRender()
{
	ioWnd::OnRender();

	m_doll.Render( GetDerivedPosX() + m_dolloffsetx, GetDerivedPosY() + m_dolloffsety, GetWidth(), GetHeight() );
}