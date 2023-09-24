

#include "stdafx.h"

#include "ioWearItem.h"

#include "ioBaseChar.h"

ioWearItem::ioWearItem()
{
}

ioWearItem::ioWearItem( const ioWearItem &rhs )
: ioItem( rhs ), m_EquipMesh( rhs.m_EquipMesh )
{
	
}

ioWearItem::~ioWearItem()
{
}

void ioWearItem::LoadProperty( ioINILoader &rkLoader )
{
	ioItem::LoadProperty( rkLoader );

	char szBuf[MAX_PATH];
	rkLoader.LoadString_e( "equip_mesh", "", szBuf, MAX_PATH );
	m_EquipMesh = szBuf;
}

ioItem::ItemType ioWearItem::GetType() const
{
	return IT_WEAR;
}

void ioWearItem::AddGlowLine( ioBaseChar *pOwner )
{
	pOwner->AddGlowLine( m_EquipMesh, m_GlowColor, 2.0f );
}

void ioWearItem::RemoveGlowLine( ioBaseChar *pOwner )
{
	pOwner->RemoveGlowLine( m_EquipMesh );
}

const ioHashString& ioWearItem::GetEquipMesh( ioBaseChar *pOwner ) const
{
	return m_EquipMesh;
}

const ioHashString& ioWearItem::FindFieldEntityGrp() const
{
	switch( GetCrownType() )
	{
	case MCT_DOUBLE_CROWN:
		{
			switch( GetItemTeamType() )
			{
			case TEAM_BLUE:
				{
					DecoInfoMap::const_iterator iter = m_FieldEntityGrpMap.find( 0 );
					if( iter != m_FieldEntityGrpMap.end() )
						return iter->second;
				}
				break;
			case TEAM_RED:
				{
					DecoInfoMap::const_iterator iter = m_FieldEntityGrpMap.find( 1 );
					if( iter != m_FieldEntityGrpMap.end() )
						return iter->second;
				}
				break;
			}
		}
		break;
	}

	return ioItem::FindFieldEntityGrp();
}
