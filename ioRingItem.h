#pragma once
#include "ioitem.h"
class ioRingItem :	public ioItem
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
	ioRingItem(void);
	ioRingItem( const ioRingItem &rhs );
	~ioRingItem(void);
};

inline ioRingItem* ToRingItem( ioItem *pItem )
{
	if( !pItem || pItem->GetType() != ioItem::IT_RING )
		return NULL;

	return dynamic_cast< ioRingItem* >( pItem );
}

inline const ioRingItem* ToRingItemConst( const ioItem *pItem )
{
	if( !pItem || pItem->GetType() != ioItem::IT_RING )
		return NULL;

	return dynamic_cast< const ioRingItem* >( pItem );
}