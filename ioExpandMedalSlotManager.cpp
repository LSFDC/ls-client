

#include "stdafx.h"
#include "ioExpandMedalSlotManager.h"

#include "ioEtcItem.h"
#include "ioDateHelp.h"

template<> ioExpandMedalSlotManager* Singleton< ioExpandMedalSlotManager >::ms_Singleton = 0;

ioExpandMedalSlotManager::ioExpandMedalSlotManager(void)
{
}

ioExpandMedalSlotManager::~ioExpandMedalSlotManager(void)
{
	Clear();
}

ioExpandMedalSlotManager& ioExpandMedalSlotManager::GetSingleton()
{
	return Singleton< ioExpandMedalSlotManager >::GetSingleton();
}

void ioExpandMedalSlotManager::Clear()
{
	m_vExpandSlotInfoVec.clear();
	m_vReserveDeleteMedal.clear();
}

void ioExpandMedalSlotManager::ApplyExpandMedalSlot( SP2Packet &rkPacket )
{
	Clear();

	int iSize = 0;
	rkPacket >> iSize;
	for( int i=0; i<iSize; ++i )
	{
		BYTE iSlotNumber = 0;
		int  iClassType = 0;
		int  iLimitTime = 0;
		rkPacket >> iClassType >> iSlotNumber >> iLimitTime;

		ExpandMedalSlot kSlot;
		kSlot.m_iClassType = iClassType;
		kSlot.m_iSlotNumber = iSlotNumber;
		kSlot.m_dwLimitTime = iLimitTime;
		m_vExpandSlotInfoVec.push_back( kSlot );
	}
}

void ioExpandMedalSlotManager::AddExpandMedalSlot( BYTE iSlotNumber, int iClassType, int iLimitTime )
{
	if( iClassType <= 0 )	return;

	bool bUpdate = false;
	int iCnt = m_vExpandSlotInfoVec.size();
	for( int i=0; i<iCnt; ++i )
	{
		if( m_vExpandSlotInfoVec[i].m_iSlotNumber == iSlotNumber &&
			m_vExpandSlotInfoVec[i].m_iClassType == iClassType )
		{
			m_vExpandSlotInfoVec[i].m_dwLimitTime = iLimitTime;
			bUpdate = true;
		}
	}

	if( !bUpdate )
	{
		ExpandMedalSlot kSlot;
		kSlot.m_iSlotNumber = iSlotNumber;
		kSlot.m_iClassType = iClassType;
		kSlot.m_dwLimitTime = iLimitTime;
		m_vExpandSlotInfoVec.push_back( kSlot );
	}
}

void ioExpandMedalSlotManager::DeleteExpandMedalSlot( SP2Packet &rkPacket )
{
	m_vReserveDeleteMedal.clear();

	int iSize;
	rkPacket >> iSize;

	for( int i=0; i<iSize; ++i )
	{
		int iClassType;
		BYTE iSlotNumber;
		rkPacket >> iClassType >> iSlotNumber;
		DeleteMedalListByChar( iClassType, iSlotNumber );
	}
	
	iSize = m_vReserveDeleteMedal.size();
	for( int i=0; i<iSize; ++i )
	{
		int iClassType = m_vReserveDeleteMedal[i].m_iClassType;
		BYTE iSlotNumber = m_vReserveDeleteMedal[i].m_iSlotNumber;

		ExpandMedalSlotVec::iterator iter = m_vExpandSlotInfoVec.begin();
		while( iter != m_vExpandSlotInfoVec.end() )
		{
			ExpandMedalSlot &eExpandSlot = *iter;

			if( eExpandSlot.m_iClassType == iClassType && eExpandSlot.m_iSlotNumber == iSlotNumber )
			{
				for( int j=ioEtcItem::EIT_ETC_EXPAND_MEDAL_SLOT01; j<ioEtcItem::EIT_ETC_EXPAND_MEDAL_SLOT20+1; ++j )
				{
					ioEtcItemExpandMedalSlot *pItem = dynamic_cast<ioEtcItemExpandMedalSlot*> (g_EtcItemMgr.FindEtcItem(j));
					if( pItem &&
						pItem->GetLimitTime() > 0 &&
						pItem->GetUseSlotNumber() == iSlotNumber )
					{
						ioHashString szIconName = pItem->GetIconName();
						ioHashString szClassName = g_MyInfo.GetClassName( iClassType, g_MyInfo.GetCharPowerUpGradeByClassType( iClassType ) );
						g_MyInfo.SetReserveDeleteExMedalSlot( szIconName, szClassName, iSlotNumber * 20 );
						break;
					}
				}

				int iCharArray = g_MyInfo.GetClassArray( eExpandSlot.m_iClassType );
				if( !COMPARE( iCharArray, 0, g_MyInfo.GetCharCount() ) )
				{
					LOG.PrintTimeAndLog( 0, "Wrong Char Array : %d", iCharArray );
					++iter;
					continue;
				}

				ioUserMedalItem *pMedalItem = g_MyInfo.GetUserMedalItem();
				if( !pMedalItem )
				{
					LOG.PrintTimeAndLog( 0, "Don't Have UserMedalItem" );
					++iter;
					continue;
				}

				if( m_vReserveDeleteMedal[i].m_iMedalItemType > 0 )
				{
					SP2Packet kPacket( CTPK_MEDALITEM_CHANGE );
					kPacket << iCharArray;
					kPacket << m_vReserveDeleteMedal[i].m_iMedalItemType;
					kPacket << false;
					TCPNetwork::SendToServer( kPacket );
				}

				iter = m_vExpandSlotInfoVec.erase( iter );
			}
			else
				++iter;
		}
	}
}

void ioExpandMedalSlotManager::DeleteMedalListByChar( int iClassType, int iSlotNumber )
{
	// ������� Ȯ�彽�� üũ.
	ExpandMedalSlotVec	vExMedalSlot;
	int iMedalIndexBySlot[6] = { -1, -1, -1, -1, -1, -1 };
	int iSize = GetExpandMedalSlot( iClassType, vExMedalSlot );
	for( int i=0; i<iSize; ++i )
	{
		for( int j=0; j<6; ++j )
		{
			if( vExMedalSlot[i].m_iSlotNumber == j )
				iMedalIndexBySlot[j] = 0;
		}
	}

	// ������ ���� ���� �ִ� ���� üũ.
	int iMax = g_MedalItemMgr.GetSlotNum( g_MyInfo.GetClassLevel( iClassType, true ) );
	for( int i=0; i<iMax; ++i )
	{
		iMedalIndexBySlot[i] = 0;
	}

	// �����ִ� ���Կ� �޴޾����� �ε��� ����.
	ioUserMedalItem *pMedalItem = g_MyInfo.GetUserMedalItem();
	if( !pMedalItem )	return;
	IntVec vMedalItemIndex;
	if( pMedalItem->GetMedalItemTypeVec( vMedalItemIndex, iClassType ) )
	{
		int iSize = vMedalItemIndex.size();
		for( int i=0; i<iSize; ++i )
		{
			for( int j=0; j<6; ++j )
			{
				if( iMedalIndexBySlot[j] == 0 )
				{
					iMedalIndexBySlot[j] = vMedalItemIndex[i];
					break;
				}
			}
		}
	}

	ReserveDeleteMedal eDeleteMedal;
	eDeleteMedal.Init();
	eDeleteMedal.m_iClassType = iClassType;
	eDeleteMedal.m_iSlotNumber = iSlotNumber;
	eDeleteMedal.m_iMedalItemType = iMedalIndexBySlot[iSlotNumber];
	m_vReserveDeleteMedal.push_back(eDeleteMedal);
}

int ioExpandMedalSlotManager::GetExpandMedalSlotNum( int iClassType )
{
	if( iClassType == 0 )
		return 0;

	int iCount = 0;
	int iMax = m_vExpandSlotInfoVec.size();
	for( int i=0; i<iMax; ++i )
	{
		if( m_vExpandSlotInfoVec[i].m_iClassType == iClassType )
			iCount++;
	}
	return iCount;
}

int ioExpandMedalSlotManager::GetExpandMedalSlot( IN int iClassType, OUT ExpandMedalSlotVec &vExpandMedalSlot )
{
	if( iClassType == 0 )
		return 0;

	vExpandMedalSlot.clear();
	int iCount = 0;
	int iMax = m_vExpandSlotInfoVec.size();
	for( int i=0; i<iMax; ++i )
	{
		ExpandMedalSlot kMedalSlot;
		if( m_vExpandSlotInfoVec[i].m_iClassType == iClassType )
		{
			vExpandMedalSlot.push_back( m_vExpandSlotInfoVec[i] );
			iCount++;
		}
	}

	return iCount;
}

bool ioExpandMedalSlotManager::CheckRemainTime( IN int iClassType, IN int iSlotNumber, OUT char *szRemainTime, int iRemainTimeSize )
{
	if( iClassType == 0 )
		return false;

	int iMax = m_vExpandSlotInfoVec.size();
	for( int i=0; i<iMax; ++i )
	{
		if( m_vExpandSlotInfoVec[i].m_iClassType == iClassType &&
			m_vExpandSlotInfoVec[i].m_iSlotNumber == iSlotNumber )
		{
			if( m_vExpandSlotInfoVec[i].IsMortmain() )
				return true;

			ZeroMemory( szRemainTime, iRemainTimeSize );
			time_t kLimitTime = DateHelp::ConvertSecondTime( m_vExpandSlotInfoVec[i].GetYear(),
															 m_vExpandSlotInfoVec[i].GetMonth(),
															 m_vExpandSlotInfoVec[i].GetDay(),
															 m_vExpandSlotInfoVec[i].GetHour(),
															 m_vExpandSlotInfoVec[i].GetMinute(),
															 0 );
			kLimitTime += 30; // ������ �ٷ� �ð��� ���� ���� �����ϱ� ���ؼ� 30���� �����ش�.
			time_t kCurServerTime = g_MyInfo.GetServerDate();
			kCurServerTime += ( REALGETTIME()/FLOAT1000 );// ��������� Ŭ���̾�Ʈ�ʸ� �ִ´�.

			int iGapSec = kLimitTime - kCurServerTime;
			enum { DAY_SEC = 86400, HOUR_SEC = 3600, MINUTE_SEC = 60, };
			int iDay    = iGapSec/DAY_SEC;
			int iHourSec= iGapSec-(iDay*DAY_SEC);
			int iHour   = iHourSec / HOUR_SEC;
			int iMinute = ( iHourSec-(iHour*HOUR_SEC) ) / MINUTE_SEC;

			if( iDay <= 0 && iHour <= 0 && iMinute <= 0 )
			{
				SafeSprintf( szRemainTime, iRemainTimeSize, STR(1) );
				return true;
			}
			else if( iDay <= 0 && iHour <= 0 && iMinute > 0 )
			{
				SafeSprintf( szRemainTime, iRemainTimeSize, STR(2), iMinute );
				return true;
			}
			else if( iDay <= 0 && iHour > 0 )
			{
				SafeSprintf( szRemainTime, iRemainTimeSize, STR(3), iHour );
				return true;
			}
			else if( iDay > 0 )
			{
				SafeSprintf( szRemainTime, iRemainTimeSize, STR(4), iDay );
				return true;
			}
			else
			{
				SafeSprintf( szRemainTime, iRemainTimeSize, STR(5) );
				return true;
			}
		}
	}

	return false;
}

bool ioExpandMedalSlotManager::CheckExpandMedalSlot( int iClassType, int iSlotNumber )
{
	int iCnt = m_vExpandSlotInfoVec.size();
	for( int i=0; i<iCnt; ++i )
	{
		if( m_vExpandSlotInfoVec[i].m_iClassType == iClassType &&
			m_vExpandSlotInfoVec[i].m_iSlotNumber == iSlotNumber )
			return true;
	}
	return false;
}