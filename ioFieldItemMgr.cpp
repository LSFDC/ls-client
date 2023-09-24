
#include "stdafx.h"

#include "ioFieldItemMgr.h"

template<> ioFieldItemMgr* Singleton< ioFieldItemMgr >::ms_Singleton = 0;

ioFieldItemMgr::ioFieldItemMgr()
{
}

ioFieldItemMgr::~ioFieldItemMgr()
{
}

ioFieldItemMgr& ioFieldItemMgr::GetSingleton()
{
	return Singleton< ioFieldItemMgr >::GetSingleton();
}

void ioFieldItemMgr::LoadINIData()
{
	char szBuf[MAX_PATH] = "";
	
	ioINILoader_e kLoader( "config/sp2_field_item.ini" );
	kLoader.SetTitle_e( "common" );

	kLoader.LoadString_e( "default_item_effect", "", szBuf, MAX_PATH );
	m_DefaultItemEffect = szBuf;
	kLoader.LoadString_e( "default_item_over_effect", "", szBuf, MAX_PATH );
	m_DefaultItemOverEffect = szBuf;

	m_fMoveDropJumpAmt = kLoader.LoadFloat_e( "move_drop_jump_amt", 0.0f );
	m_fMoveDropGravityAmt = kLoader.LoadFloat_e( "move_drop_gravity_amt", 0.0f );
}
