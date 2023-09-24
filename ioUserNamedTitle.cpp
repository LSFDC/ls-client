
#include "stdafx.h"

#include "ioUserNamedTitle.h"

ioUserNamedTitle::ioUserNamedTitle()
{
	m_NamedTitleSlotList.clear();
}

ioUserNamedTitle::~ioUserNamedTitle()
{
	m_NamedTitleSlotList.clear();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

//����ȭ
void ioUserNamedTitle::ApplyNamedTitleSlotData( SP2Packet &rkPacket )
{
	m_NamedTitleSlotList.clear();

	int nSize;
	rkPacket >> nSize;

	for( int i=0; i<nSize; ++i )
	{
		NamedTitleSlot sNamedTitleSlot;

		rkPacket >> sNamedTitleSlot.m_dwCode >> sNamedTitleSlot.m_nValue >> sNamedTitleSlot.m_nLevel;
		rkPacket >> sNamedTitleSlot.m_byPremium >> sNamedTitleSlot.m_byEquip >> sNamedTitleSlot.m_byStatus;

		if ( sNamedTitleSlot.m_byStatus == TITLE_DISABLE )
			continue;

		m_NamedTitleSlotList.push_back( sNamedTitleSlot );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

//���� �߰�
bool ioUserNamedTitle::AddNamedTitleSlotData( const NamedTitleSlot &sNewSlot )
{	
	for ( int i=0; i<(int)m_NamedTitleSlotList.size(); i++ )
	{
		if ( m_NamedTitleSlotList[i].m_dwCode == sNewSlot.m_dwCode )
			return false;
	}

	m_NamedTitleSlotList.push_back( sNewSlot );
	return true;
}

//���� ����
bool ioUserNamedTitle::DeleteNamedTitleSlotData( DWORD dwCode )
{
	NamedTitleSlotList::iterator iter = m_NamedTitleSlotList.begin();
	while( iter != m_NamedTitleSlotList.end() )
	{
		if( iter->m_dwCode == dwCode )
		{
			iter = m_NamedTitleSlotList.erase( iter );
			return true;
		}
		else
			++iter;
	}

	return false;
}

//���� ����
bool ioUserNamedTitle::SetNamedTitleSlot( const NamedTitleSlot &sNamedTitleSlot )
{
	int nSize = (int)m_NamedTitleSlotList.size();
	for( int i=0; i < nSize; ++i )
	{
		if( m_NamedTitleSlotList[i].m_dwCode == sNamedTitleSlot.m_dwCode )
		{
			m_NamedTitleSlotList[i] = sNamedTitleSlot;
			return true;
		}
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

//���� ����
void ioUserNamedTitle::SetNamedTitleEquip( DWORD dwCode )
{
	int nSize = (int)m_NamedTitleSlotList.size();
	for( int i=0; i<nSize; ++i )
	{
		if( m_NamedTitleSlotList[i].m_dwCode == dwCode )
		{
			m_NamedTitleSlotList[i].m_byEquip = true;
			return;
		}
	}
}

//��� ����
void ioUserNamedTitle::SetNamedTitleGrade( DWORD dwCode, int nLevel )
{
	int nSize = (int)m_NamedTitleSlotList.size();
	for( int i=0; i<nSize; ++i )
	{
		if( m_NamedTitleSlotList[i].m_dwCode == dwCode )
		{
			m_NamedTitleSlotList[i].m_nLevel = nLevel;
			return;
		}
	}
}

//�����̾� ����
void ioUserNamedTitle::SetNamedTitlePremium( DWORD dwCode )
{
	int nSize = (int)m_NamedTitleSlotList.size();
	for( int i=0; i<nSize; ++i )
	{
		if( m_NamedTitleSlotList[i].m_dwCode == dwCode )
		{
			m_NamedTitleSlotList[i].m_byPremium = true;
			m_NamedTitleSlotList[i].m_byStatus = TITLE_NEW;
			return;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void ioUserNamedTitle::ClearNamedTitleEquip()
{
	int nSize = (int)m_NamedTitleSlotList.size();
	for( int i=0; i<nSize; ++i )
		m_NamedTitleSlotList[i].m_byEquip = false;
}

void ioUserNamedTitle::ClearNamedTitleNew()
{
	int nSize = (int)m_NamedTitleSlotList.size();
	for( int i=0; i<nSize; ++i )
	{
		if ( m_NamedTitleSlotList[i].m_byStatus == TITLE_NEW )
			m_NamedTitleSlotList[i].m_byStatus = TITLE_ACTIVE;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

//���� ���� ���� ���
bool ioUserNamedTitle::GetNamedTitleSlotEquip( NamedTitleSlot &sNamedTitleSlot )
{
	if ( m_NamedTitleSlotList.empty() )
		return false;

	int nSize = (int)m_NamedTitleSlotList.size();
	for( int i=0; i < nSize; ++i )
	{
		if( m_NamedTitleSlotList[i].m_byEquip )
		{
			sNamedTitleSlot = m_NamedTitleSlotList[i];
			return true;
		}
	}

	return false;
}

//���� ���(���� �ڵ�)
bool ioUserNamedTitle::GetNamedTitleSlot( DWORD dwCode, NamedTitleSlot &sNamedTitleSlot )
{
	if ( m_NamedTitleSlotList.empty() )
		return false;

	int nSize = (int)m_NamedTitleSlotList.size();
	for( int i=0; i < nSize; ++i )
	{
		if( m_NamedTitleSlotList[i].m_dwCode == dwCode )
		{
			sNamedTitleSlot = m_NamedTitleSlotList[i];
			return true;
		}
	}

	return false;
}

//���� ���(���� �迭)
bool ioUserNamedTitle::GetNamedTitleSlotArray( int nArray, NamedTitleSlot &sNamedTitleSlot )
{
	if ( m_NamedTitleSlotList.empty() )
		return false;

	if( COMPARE( nArray, 0, (int)m_NamedTitleSlotList.size() ) )
	{
		sNamedTitleSlot = m_NamedTitleSlotList[nArray];
		return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

//���� ���� ���
int ioUserNamedTitle::GetNamedTitleSlotCnt()
{	
	return (int)m_NamedTitleSlotList.size();
}

//���ο� Īȣ
bool ioUserNamedTitle::IsNewNamedTitle()
{
	if ( m_NamedTitleSlotList.empty() )
		return false;

	int nSize = (int)m_NamedTitleSlotList.size();
	for( int i=0; i < nSize; ++i )
	{
		if( m_NamedTitleSlotList[i].m_byStatus == TITLE_NEW )
			return true;
	}

	return false;
}

//�����̾� ���� Īȣ
bool ioUserNamedTitle::IsEnablePremiumNamedTitle()
{
	if ( m_NamedTitleSlotList.empty() )
		return false;

	int nSize = (int)m_NamedTitleSlotList.size();
	for( int i=0; i < nSize; ++i )
	{
		if( !m_NamedTitleSlotList[i].m_byPremium )
			return true;
	}

	return false;
}