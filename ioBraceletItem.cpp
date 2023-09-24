#include "StdAfx.h"
#include "ioBraceletItem.h"


ioBraceletItem::ioBraceletItem(void)
{
}

ioBraceletItem::ioBraceletItem( const ioBraceletItem &rhs ) :
ioItem( rhs ),
m_fLocalRandomNumber( rhs.m_fLocalRandomNumber ),
m_iManualNumber( rhs.m_iManualNumber ),
m_vToolTipTextList( rhs.m_vToolTipTextList )
{

}


ioBraceletItem::~ioBraceletItem(void)
{
}

ioItem::ItemType ioBraceletItem::GetType() const
{
	return IT_BRACELET;
}

ioItem* ioBraceletItem::Clone()
{
	return new ioBraceletItem( *this );
}

void ioBraceletItem::LoadProperty( ioINILoader &rkLoader )
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

void ioBraceletItem::InitializeBeforeEquip( const ioHashString& szName, ioPlayStage* pStage )
{
	ioItem::InitializeBeforeEquip( szName, pStage );

	if( !P2PNetwork::IsNetworkPlaying() )
		SetAccessoryRandomNum( m_fLocalRandomNumber );
}

const int& ioBraceletItem::GetManualIndex() const
{
	return m_iManualNumber;
}

const ioHashStringVec& ioBraceletItem::GetToolTipText() const
{
	return m_vToolTipTextList;
}
