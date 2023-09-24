
#include "stdafx.h"

#include "ioBaseChar.h"

#include "ioUserMedalItem.h"

ioUserMedalItem::ioUserMedalItem()
{
	Initialize();
}

ioUserMedalItem::~ioUserMedalItem()
{
}

void ioUserMedalItem::Initialize()
{
	m_vMedalItemVec.clear();
}

void ioUserMedalItem::ApplyMedalItemData( SP2Packet &rkPacket )
{
	m_vMedalItemVec.clear();

	int iSize;
	rkPacket >> iSize;

	for( int i=0; i < iSize; ++i )
	{
		MEDALITEMSLOT kItem;

		rkPacket >> kItem.m_iItemType
			     >> kItem.m_iEquipClass
			     >> kItem.m_iPeriodType
			     >> kItem.m_iLimitDate
			     >> kItem.m_iLimitTime;

		m_vMedalItemVec.push_back( kItem );
	}
}

void ioUserMedalItem::AddMedalItemData( const MEDALITEMSLOT &rkNewSlot )
{
	MEDALITEMSLOT kTempSlot;
	//if( GetMedalItem( rkNewSlot.m_iItemType, kTempSlot, false, 0 ) )
		//return;

	MEDALITEMSLOT kItem;
	kItem = rkNewSlot;
	m_vMedalItemVec.push_back( kItem );
}

bool ioUserMedalItem::DeleteMedalItemData( int iItemType )
{
	int iSize = m_vMedalItemVec.size();

	for( int i=0; i < iSize; ++i )
	{
		MEDALITEMSLOT &rkItem = m_vMedalItemVec[i];
		if( rkItem.m_iItemType == iItemType && rkItem.m_iEquipClass == 0 )
		{
			m_vMedalItemVec.erase( m_vMedalItemVec.begin() + i );
			return true;
		}
	}

	return false;
}

int ioUserMedalItem::GetMedalItemCnt()
{
	return (int)m_vMedalItemVec.size();
}

void ioUserMedalItem::SetMedalItem( const MEDALITEMSLOT &rkMedalItem, bool bFindNotEquipMedal, int nClassType )
{
	int iSize = m_vMedalItemVec.size();
	for( int i=0; i < iSize; ++i )
	{
		MEDALITEMSLOT &rkItem = m_vMedalItemVec[i];

		if( bFindNotEquipMedal )
		{
			if( rkItem.m_iItemType == rkMedalItem.m_iItemType && rkItem.m_iEquipClass == 0 )
			{
				m_vMedalItemVec[i] = rkMedalItem;
				return;
			}
		}
		else
		{
			if( nClassType == 0 )
			{
				if( rkItem.m_iItemType == rkMedalItem.m_iItemType )
				{
					m_vMedalItemVec[i] = rkMedalItem;
					return;
				}
			}
			else
			{
				if( rkItem.m_iItemType == rkMedalItem.m_iItemType && rkItem.m_iEquipClass == nClassType )
				{
					m_vMedalItemVec[i] = rkMedalItem;
					return;
				}
			}
		}
	}
}

bool ioUserMedalItem::GetMedalItem( IN int iItemType, OUT MEDALITEMSLOT &rkMedalItem, bool FindNotEquipMedal, int nClassType )
{
	// 초기화
	rkMedalItem.Init();

	int iSize = m_vMedalItemVec.size();
	for( int i=0; i < iSize; ++i )
	{
		MEDALITEMSLOT &rkItem = m_vMedalItemVec[i];
		if( FindNotEquipMedal )
		{
			if( rkItem.m_iItemType == iItemType && rkItem.m_iEquipClass == 0 )
			{
				rkMedalItem = m_vMedalItemVec[i];
				return true;
			}
		}
		else
		{
			if( nClassType == 0 )
			{
				if( rkItem.m_iItemType == iItemType )
				{
					rkMedalItem = m_vMedalItemVec[i];
					return true;
				}
			}
			else
			{
				if( rkItem.m_iItemType == iItemType && rkItem.m_iEquipClass ==  nClassType )
				{
					rkMedalItem = m_vMedalItemVec[i];
					return true;
				}
			}
		}
		
	}

	return false;
}

bool ioUserMedalItem::GetMedalItemArray( IN int iArray, OUT MEDALITEMSLOT &rkMedalItem )
{
	// 초기화
	rkMedalItem.Init();

	if( COMPARE( iArray, 0, (int)m_vMedalItemVec.size() ) )
	{
		rkMedalItem = m_vMedalItemVec[iArray];
		return true;
	}

	return false;
}

bool ioUserMedalItem::GetMedalItemTypeVec( OUT IntVec &rvItemTypeVec, IN int iClassType )
{
	rvItemTypeVec.clear();
	bool bExist = false;
	int iSize = m_vMedalItemVec.size();
	for( int i=0; i < iSize; ++i )
	{
		MEDALITEMSLOT &rkItem = m_vMedalItemVec[i];
		if( rkItem.m_iEquipClass == iClassType )
		{
			rvItemTypeVec.push_back( rkItem.m_iItemType );		
			bExist = true;
		}
	}

	return bExist;
}

void ioUserMedalItem::FillEquipClass( IN int iClassType, IN int iMaxSlotNum, OUT SP2Packet &rkPacket )
{
	int iEquipNum = 0;
	int iSize = m_vMedalItemVec.size();
	for( int i=0; i < iSize; ++i )
	{
		MEDALITEMSLOT &rkItem = m_vMedalItemVec[i];
		if( rkItem.m_iEquipClass == iClassType )
		{
			rkPacket << rkItem.m_iItemType;
			iEquipNum++;
			if( iEquipNum >= iMaxSlotNum )
				return;
		}
	}


	// 빈값 셋팅
	for (int i = iEquipNum; i < iMaxSlotNum ; i++)
	{
		rkPacket << 0;
	}
}

void ioUserMedalItem::GetUseInfo( IN int iItemType, OUT ioHashString &rsInfo )
{
	MEDALITEMSLOT kSlot;
	if( !GetMedalItem( iItemType, kSlot ) )
		return;

	char szText[MAX_PATH]="";
	if( kSlot.m_iPeriodType == PT_MORTMAIN )
	{
		SafeSprintf( szText, sizeof( szText ), STR(1) );
	}
	else 
	{
		Help::GetRemainTime( kSlot.GetYear(), kSlot.GetMonth(), kSlot.GetDay(), kSlot.GetHour(), kSlot.GetMinute(), szText, sizeof( szText ) );
	}

	rsInfo = szText;
}

void ioUserMedalItem::ReleaseEquipMedal( int iClassType )
{
	int iSize = m_vMedalItemVec.size();
	for( int i=0; i < iSize; ++i )
	{
		MEDALITEMSLOT &rkItem = m_vMedalItemVec[i];
		if( rkItem.m_iEquipClass == iClassType )
		{
			rkItem.m_iEquipClass = 0;
		}
	}
}

int ioUserMedalItem::GetExtraItemCnt()
{
	int iSize = m_vMedalItemVec.size();
	return iSize;
}

bool ioUserMedalItem::IsSlotFull()
{
	if( GetExtraItemCnt() > MAX_SLOT_CNT )
		return true;
	return false;
}