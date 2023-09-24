#include "StdAfx.h"
#include "ioRingItem.h"


ioRingItem::ioRingItem(void)
{
}

ioRingItem::ioRingItem( const ioRingItem &rhs ) :
ioItem( rhs ),
m_fLocalRandomNumber( rhs.m_fLocalRandomNumber ),
m_iManualNumber( rhs.m_iManualNumber ),
m_vToolTipTextList( rhs.m_vToolTipTextList )
{

}


ioRingItem::~ioRingItem(void)
{
}

ioItem::ItemType ioRingItem::GetType() const
{
	return IT_RING;
}

ioItem* ioRingItem::Clone()
{
	return new ioRingItem( *this );
}

void ioRingItem::LoadProperty( ioINILoader &rkLoader )
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

void ioRingItem::InitializeBeforeEquip( const ioHashString& szName, ioPlayStage* pStage )
{
	ioItem::InitializeBeforeEquip( szName, pStage );

	if( !P2PNetwork::IsNetworkPlaying() )
		SetAccessoryRandomNum( m_fLocalRandomNumber );
}

const int& ioRingItem::GetManualIndex() const
{
	return m_iManualNumber;
}

const ioHashStringVec& ioRingItem::GetToolTipText() const
{
	return m_vToolTipTextList;
}
