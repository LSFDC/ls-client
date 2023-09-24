#include "StdAfx.h"
#include "ioNecklaceItem.h"


ioNecklaceItem::ioNecklaceItem(void)
{
}

ioNecklaceItem::ioNecklaceItem( const ioNecklaceItem &rhs ):
ioItem( rhs ),
m_fLocalRandomNumber( rhs.m_fLocalRandomNumber ),
m_iManualNumber( rhs.m_iManualNumber ),
m_vToolTipTextList( rhs.m_vToolTipTextList )
{

}


ioNecklaceItem::~ioNecklaceItem(void)
{
}

ioItem::ItemType ioNecklaceItem::GetType() const
{
	return IT_NECKLACE;
}

ioItem* ioNecklaceItem::Clone()
{
	return new ioNecklaceItem( *this );
}

void ioNecklaceItem::LoadProperty( ioINILoader &rkLoader )
{
	ioItem::LoadProperty( rkLoader );
	char szBuf[MAX_PATH],szKey[MAX_PATH];
	SetItemType();      //Default.
	m_fLocalRandomNumber = rkLoader.LoadFloat_e( "local_random_num", 0.0f );

	m_iManualNumber = rkLoader.LoadInt_e( "manual_number", 0 );
	int iSize = rkLoader.LoadInt_e( "max_tooltip_text_size", 0 );
	for( int i=0 ; i<iSize ; ++i )
	{
		ioHashString szText;
		wsprintf_e( szKey, "tooltip_text%d", i+1 );
		rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );
		szText = szBuf;

		m_vToolTipTextList.push_back( szText );
	}
}

void ioNecklaceItem::InitializeBeforeEquip( const ioHashString& szName, ioPlayStage* pStage )
{
	//ioItem::InitializeBeforeEquip( szName, pStage );

	if( !P2PNetwork::IsNetworkPlaying() )
		SetAccessoryRandomNum( m_fLocalRandomNumber );
}

const int& ioNecklaceItem::GetManualIndex() const
{
	return m_iManualNumber;
}

const ioHashStringVec& ioNecklaceItem::GetToolTipText() const
{
	return m_vToolTipTextList;
}
