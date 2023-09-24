#pragma once
#include "ioItem.h"
class ioBraceletItem :	public ioItem
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
	ioBraceletItem(void);
	ioBraceletItem( const ioBraceletItem &rhs );
	virtual ~ioBraceletItem(void);
};

inline ioBraceletItem* ToBraceletItem( ioItem *pItem )
{
	if( !pItem || pItem->GetType() != ioItem::IT_BRACELET )
		return NULL;

	return dynamic_cast< ioBraceletItem* >( pItem );
}

inline const ioBraceletItem* ToBraceletItemConst( const ioItem *pItem )
{
	if( !pItem || pItem->GetType() != ioItem::IT_BRACELET )
		return NULL;

	return dynamic_cast< const ioBraceletItem* >( pItem );
}
