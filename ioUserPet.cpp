
#include "stdafx.h"

#include "ioUserPet.h"

ioUserPet::ioUserPet()
{
	m_PetSlotList.clear();
}

ioUserPet::~ioUserPet()
{
	m_PetSlotList.clear();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//����ȭ
void ioUserPet::ApplyPetSlotData( SP2Packet &rkPacket )
{
	m_PetSlotList.clear();

	int nSize;
	rkPacket >> nSize;

	for( int i=0; i < nSize; ++i )
	{
		PetSlot sPetSlot;

		rkPacket >> sPetSlot.m_nPetCode >> sPetSlot.m_PetRankType 
				 >> sPetSlot.m_nIndex >> sPetSlot.m_nCurLevel >> sPetSlot.m_CurExp >> sPetSlot.m_bEquip;
		
		sPetSlot.m_Name = g_PetInfoMgr.GetPetName( sPetSlot.m_nPetCode, (PetRankType)sPetSlot.m_PetRankType );
		sPetSlot.m_IconName = g_PetInfoMgr.GetPetIcon( sPetSlot.m_nPetCode, (PetRankType)sPetSlot.m_PetRankType );
		
		if ( sPetSlot.m_Name.IsEmpty() || sPetSlot.m_IconName.IsEmpty() )
			continue;

		m_PetSlotList.push_back( sPetSlot );
	}

	std::sort( m_PetSlotList.begin(), m_PetSlotList.end(), PetSlotSort() );
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//���� �߰�
void ioUserPet::AddPetSlotData( const PetSlot &sNewSlot )
{	
	if ( IsSlotFull() )
		return;

	PetSlot sPetSlot = sNewSlot;
	sPetSlot.m_Name = g_PetInfoMgr.GetPetName( sNewSlot.m_nPetCode, (PetRankType)sNewSlot.m_PetRankType );
	sPetSlot.m_IconName = g_PetInfoMgr.GetPetIcon( sNewSlot.m_nPetCode, (PetRankType)sNewSlot.m_PetRankType );

	m_PetSlotList.push_back( sPetSlot );
}

//���� ����
bool ioUserPet::DeletePetSlotData( int nSlotIndex )
{
	PetSlotList::iterator iter = m_PetSlotList.begin();
	while( iter != m_PetSlotList.end() )
	{
		if( iter->m_nIndex == nSlotIndex )
		{
			iter = m_PetSlotList.erase( iter );
			return true;
		}
		else
			++iter;
	}

	return false;
}

//���� ����
void ioUserPet::SetPetSlot( const PetSlot &sPetSlot )
{
	int nSize = (int)m_PetSlotList.size();
	for( int i=0; i < nSize; ++i )
	{
		if( m_PetSlotList[i].m_nIndex == sPetSlot.m_nIndex )
		{
			m_PetSlotList[i] = sPetSlot;
			return;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//���� ����
void ioUserPet::SetPetEquip( int nSlotIndex, bool bEquip )
{
	int nSize = (int)m_PetSlotList.size();
	for( int i=0; i<nSize; ++i )
	{
		m_PetSlotList[i].m_bEquip = false;
		if( m_PetSlotList[i].m_nIndex == nSlotIndex )
			m_PetSlotList[i].m_bEquip = bEquip;
	}
}

//���� ����
void ioUserPet::ClearPetEquip()
{
	int nSize = (int)m_PetSlotList.size();
	for( int i=0; i<nSize; ++i )
		m_PetSlotList[i].m_bEquip = false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//���� ���(���� �ε���)
bool ioUserPet::GetPetSlot( int nSlotIndex, PetSlot &sPetSlot )
{
	if ( m_PetSlotList.empty() )
		return false;

	int nSize = (int)m_PetSlotList.size();
	for( int i=0; i < nSize; ++i )
	{
		if( m_PetSlotList[i].m_nIndex == nSlotIndex )
		{
			sPetSlot = m_PetSlotList[i];
			return true;
		}
	}

	return false;
}

//���� ���(���� �迭)
bool ioUserPet::GetPetSlotArray( int nArray, PetSlot &sPetSlot )
{
	if ( m_PetSlotList.empty() )
		return false;

	if( COMPARE( nArray, 0, (int)m_PetSlotList.size() ) )
	{
		sPetSlot = m_PetSlotList[nArray];
		return true;
	}

	return false;
}

//���� ���� ���� ���
bool ioUserPet::GetPetSlotEquip( PetSlot &sPetSlot )
{
	if ( m_PetSlotList.empty() )
		return false;

	int nSize = (int)m_PetSlotList.size();
	for( int i=0; i < nSize; ++i )
	{
		if( m_PetSlotList[i].m_bEquip )
		{
			sPetSlot = m_PetSlotList[i];
			return true;
		}
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//���� ���� ���
int ioUserPet::GetPetSlotCnt()
{	
	return (int)m_PetSlotList.size();
}

//���� ���� Ȯ��
bool ioUserPet::IsSlotFull()
{
	if( GetPetSlotCnt() >= MAX_SLOT_CNT )
		return true;
	return false;
}

//����
bool ioUserPet::SlotSort()
{
	if ( m_PetSlotList.empty() )
		return false;

	std::sort( m_PetSlotList.begin(), m_PetSlotList.end(), PetSlotSort() );

	return true;
}