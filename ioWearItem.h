#pragma once

#include "ioItem.h"

class ioINILoader;

class ioWearItem : public ioItem
{
public:
	enum WearSubType
	{
		WRT_WEAPON,
		WRT_ARMOR,
		WRT_HELMET,
		WRT_CLOAK,
		WRT_UNKNOWN,
	};
protected:
	ioHashString m_EquipMesh;
public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ItemType GetType() const;

public:
	virtual void AddGlowLine( ioBaseChar *pOwner );
	virtual void RemoveGlowLine( ioBaseChar *pOwner );

public:
	const ioHashString& GetEquipMesh( ioBaseChar *pOwner ) const;
	virtual WearSubType GetSubType() const = 0;
	virtual void OnEquiped( ioBaseChar *pOwner ) = 0;

public:
	const ioHashString& FindFieldEntityGrp() const;

public:
	ioWearItem();
	ioWearItem( const ioWearItem &rhs );
	virtual ~ioWearItem();
};

inline ioWearItem* ToWearItem( ioItem *pItem )
{
	if( !pItem || pItem->GetType() != ioItem::IT_WEAR )
		return NULL;

	return dynamic_cast< ioWearItem* >( pItem );
}

inline const ioWearItem* ToWearItemConst( const ioItem *pItem )
{
	if( !pItem || pItem->GetType() != ioItem::IT_WEAR )
		return NULL;

	return dynamic_cast< const ioWearItem* >( pItem );
}

