#pragma once
#include "ioitem.h"
class ioNecklaceItem : public ioItem
{
private:
	float m_fLocalRandomNumber;
	int	  m_iManualNumber;
	ioHashStringVec m_vToolTipTextList;
public:
	virtual ItemType GetType() const;
	virtual ioItem* Clone();
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual void InitializeBeforeEquip( const ioHashString& szName, ioPlayStage* pStage );
public:
	const int& GetManualIndex() const;
	const ioHashStringVec& GetToolTipText() const;
public:
	ioNecklaceItem(void);
	ioNecklaceItem( const ioNecklaceItem &rhs );
	virtual ~ioNecklaceItem(void);
};

inline ioNecklaceItem* ToNecklaceItem( ioItem *pItem )
{
	if( !pItem || pItem->GetType() != ioItem::IT_NECKLACE )
		return NULL;

	return dynamic_cast< ioNecklaceItem* >( pItem );
}

inline const ioNecklaceItem* ToNecklaceItemConst( const ioItem *pItem )
{
	if( !pItem || pItem->GetType() != ioItem::IT_NECKLACE )
		return NULL;

	return dynamic_cast< const ioNecklaceItem* >( pItem );
}
