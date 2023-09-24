
#include "stdafx.h"

#include "ioRestorationGaugeBuff.h"

#include "ioBaseChar.h"

ioRestorationGaugeBuff::ioRestorationGaugeBuff()
{
	m_dwTickGap = 0;
	m_iRecoverySlot = -1;
	m_iCurRecoveryItemCnt = 0;

	m_vRecoverySlotList.clear();
}

ioRestorationGaugeBuff::ioRestorationGaugeBuff( const ioRestorationGaugeBuff &rhs )
: ioBuff( rhs ),
 m_iSortType( rhs.m_iSortType ),
 m_iSortReduceFullGauge( rhs.m_iSortReduceFullGauge ),
 m_fRecoveryGauge( rhs.m_fRecoveryGauge ),
 m_dwRecoveryTick( rhs.m_dwRecoveryTick ),
 m_bOneItem( rhs.m_bOneItem ),
 m_bLimitMax( rhs.m_bLimitMax ),
 m_fGaugeLimitRate( rhs.m_fGaugeLimitRate ),
 m_bRecoveryByRate( rhs.m_bRecoveryByRate ),
 m_RecoveryRate( rhs.m_RecoveryRate ),
 m_iRecoveryItemCnt( rhs.m_iRecoveryItemCnt ),
 m_bRecoveryCountByRate( rhs.m_bRecoveryCountByRate ),
 m_RecoveryCountRateList( rhs.m_RecoveryCountRateList ),
 m_bRestoreOwnerItem( rhs.m_bRestoreOwnerItem )
{
	m_dwTickGap = 0;
	m_iRecoverySlot = -1;
	m_iCurRecoveryItemCnt = 0;

	m_vRecoverySlotList.clear();
}

ioRestorationGaugeBuff::~ioRestorationGaugeBuff()
{
}

void ioRestorationGaugeBuff::LoadProperty( ioINILoader &rkLoader )
{
	ioBuff::LoadProperty( rkLoader );
	char szKey[MAX_PATH];

	m_iSortType = rkLoader.LoadInt_e( "sort_type", 0 );
	m_iSortReduceFullGauge = rkLoader.LoadInt_e( "sort_reduce_fullgauge", 0 );

	m_fRecoveryGauge = rkLoader.LoadFloat_e( "gauge_recovery_value", 0.0f );
	m_dwRecoveryTick = rkLoader.LoadInt_e( "gauge_recovery_tick", 0 );

	m_bOneItem	= rkLoader.LoadBool_e( "one_item_recovery", false );
	m_bLimitMax	= rkLoader.LoadBool_e( "limit_max", false );
	m_iRecoveryItemCnt		= rkLoader.LoadInt_e( "item_recovery_cnt", 0 );
	m_bRecoveryCountByRate	= rkLoader.LoadBool_e( "recovery_count_by_rate", false );

	m_RecoveryCountRateList.clear();
	for( int i=0; i<4; ++i )
	{
		float fRate;
		wsprintf_e( szKey, "recovery_rate_count%d", i+1 );
		fRate = rkLoader.LoadFloat( szKey, 0.0f );
		m_RecoveryCountRateList.push_back(fRate);
	}

	m_bRecoveryByRate	= rkLoader.LoadBool_e( "recovery_by_rate", 0 );
	m_RecoveryRate.push_back( rkLoader.LoadFloat_e( "recovery_weapon_gauge_rate", FLOAT1 ) );
	m_RecoveryRate.push_back( rkLoader.LoadFloat_e( "recovery_armor_gauge_rate", FLOAT1 ) );
	m_RecoveryRate.push_back( rkLoader.LoadFloat_e( "recovery_helmet_gauge_rate", FLOAT1 ) );
	m_RecoveryRate.push_back( rkLoader.LoadFloat_e( "recovery_cloak_gauge_rate", FLOAT1 ) );

	m_fGaugeLimitRate = rkLoader.LoadFloat_e( "gauge_limit_rate", FLOAT1 );

	m_bRestoreOwnerItem = rkLoader.LoadBool_e( "restore_owner_item", false );
}

ioBuff* ioRestorationGaugeBuff::Clone()
{
	return new ioRestorationGaugeBuff( *this );
}

void ioRestorationGaugeBuff::StartBuff( ioBaseChar *pOwner )
{
	ioBuff::StartBuff( pOwner );

	m_dwTickGap = 0;

	m_iRecoverySlot = GetLowestItem();
	GetLowestItemList( pOwner );
}

bool ioRestorationGaugeBuff::ApplyStartBuff( ioBaseChar *pOwner, DWORD dwGlobalTime, DWORD dwGapTime )
{
	if( !ioBuff::ApplyStartBuff( pOwner, dwGlobalTime, dwGapTime ) )
		return false;

	m_dwTickGap = 0;
	m_iRecoverySlot = GetLowestItem();
	GetLowestItemList( pOwner );

	return true;
}

void ioRestorationGaugeBuff::ProcessBuff( float fTimePerSec )
{
	switch( m_iOperationType )
	{
	case OT_SWITCH:
		if( !CheckSwitchType( fTimePerSec ) )
		{
			SetReserveEndBuff();
			return;
		}

		CheckCallingProcess();
		break;
	case OT_DELAY:
	case OT_ACTIVE:
		ioBuff::ProcessBuff( fTimePerSec );
		break;
	case OT_PASSIVE:
		CheckCallingAreaWeapon();
		break;
	}
	
	m_dwTickGap += (DWORD)( fTimePerSec * FLOAT1000 );
	ioItem *pOwnerItem = m_pOwner->GetEquipedItem( m_ItemName );

	if( m_dwTickGap > m_dwRecoveryTick )
	{
		if( m_fRecoveryGauge != 0.0f )
		{
			if( m_bRecoveryByRate )
			{
				DWORD dwSeed = m_pOwner->GetRandomSeed();

				IORandom random;
				random.SetRandomSeed( dwSeed );

				IntVec	iRecoverySlot;

				while( (int)iRecoverySlot.size() < m_iCurRecoveryItemCnt )
				{
					for( int i=0; i<4; ++i )
					{
						if( m_RecoveryRate[i] == 0.0f )	continue;
						
						float fRate = 0.0f;
						float fRand = (float)random.Random( FLOAT100 ) * 0.01f;

						if( COMPARE( fRand, fRate, m_RecoveryRate[i] ) )
						{
							IntVec::iterator	iter;
							iter = std::find( iRecoverySlot.begin(), iRecoverySlot.end(), i );

							if( iter == iRecoverySlot.end() )
							{
								iRecoverySlot.push_back(i);
							}
						}
					}
				}

				for( int i=0; i < m_iCurRecoveryItemCnt; ++i )
				{
					if( iRecoverySlot.empty() )	continue;

					int iSlot = iRecoverySlot[i];
					ioItem *pItem = m_pOwner->GetEquipedItem( iSlot );
					ioSkill *pSkill = m_pOwner->GetEquipedSkill( iSlot );

					if( !pItem || !pSkill ) continue;

					//if( pItem == m_pOwnerItem && !m_bRestoreOwnerItem ) continue;
					if( pItem == pOwnerItem && !m_bRestoreOwnerItem ) continue;

					if( pSkill->CheckUseBuff( m_pOwner ) ) continue;

					if( g_WeaponMgr.CheckRotationWeaponByItem(m_pOwner->GetCharName(), pItem->GetItemCreateIndex()) )
						continue;

					float fMaxGauge = pItem->GetMaxSkillGauge(m_pOwner->GetCharName());
					float fCurGauge = pItem->GetCurSkillGuage();

					float fLimitRate = 0.0f;
					if( fMaxGauge > 0.0f )
						fLimitRate = fCurGauge / fMaxGauge;

					if( fLimitRate > 0.0f &&  m_fGaugeLimitRate < FLOAT1 && fLimitRate >= m_fGaugeLimitRate )
						continue;

					if( m_bLimitMax )
					{
						if( fMaxGauge > fCurGauge )
						{
							fCurGauge += m_fRecoveryGauge;
							if( m_bLimitMax && fCurGauge > fMaxGauge )
								fCurGauge = fMaxGauge;

							pItem->SetCurSkillGauge( fCurGauge );
						}
					}
					else
					{
						fCurGauge += m_fRecoveryGauge;
						pItem->SetCurSkillGauge( fCurGauge );
					}

#ifdef ANTIHACK
					// recover skill gauge
					AddRecoverSkillInfo( iSlot, pItem );
#endif

				}
			}
			else
			{
				for( int i=0; i < m_iCurRecoveryItemCnt; ++i )
				{
					int iSlot = m_vRecoverySlotList[i].m_iSlotNum;
					ioItem *pItem = m_pOwner->GetEquipedItem( iSlot );
					ioSkill *pSkill = m_pOwner->GetEquipedSkill( iSlot );

					if( !pItem || !pSkill ) 
						continue;

					//if( pItem == m_pOwnerItem && !m_bRestoreOwnerItem ) continue;
					if( pItem == pOwnerItem && !m_bRestoreOwnerItem ) 
						continue;

					if( pSkill->CheckUseBuff( m_pOwner ) ) 
						continue;

					if( g_WeaponMgr.CheckRotationWeaponByItem(m_pOwner->GetCharName(), pItem->GetItemCreateIndex()) )
						continue;

					float fMaxGauge = pItem->GetMaxSkillGauge(m_pOwner->GetCharName());
					float fCurGauge = pItem->GetCurSkillGuage();

					float fLimitRate = 0.0f;
					if( fMaxGauge > 0.0f )
						fLimitRate = fCurGauge / fMaxGauge;

					if( fLimitRate > 0.0f &&  m_fGaugeLimitRate < FLOAT1 && fLimitRate >= m_fGaugeLimitRate )
						continue;

					if( m_bLimitMax )
					{
						if( fMaxGauge > fCurGauge )
						{
							fCurGauge += m_fRecoveryGauge;
							if( m_bLimitMax && fCurGauge > fMaxGauge )
								fCurGauge = fMaxGauge;

							pItem->SetCurSkillGauge( fCurGauge );
						}
					}
					else
					{
						fCurGauge += m_fRecoveryGauge;
						pItem->SetCurSkillGauge( fCurGauge );
					}
				}
			}
		}

		m_dwTickGap -= m_dwRecoveryTick;
	}
}

int ioRestorationGaugeBuff::GetLowestItem()
{
	int iSlot = -1;
	float fGaugeRate = FLOAT1;

	ioItem *pOwnerItem = m_pOwner->GetEquipedItem( m_ItemName );

	for( int i=0; i < 4; ++i )
	{
		ioItem *pItem = m_pOwner->GetEquipedItem( i );
		ioSkill *pSkill = m_pOwner->GetEquipedSkill( i );

		if( !pItem || !pSkill ) continue;
		//if( pItem == m_pOwnerItem && !m_bRestoreOwnerItem) continue;
		if( pItem == pOwnerItem && !m_bRestoreOwnerItem) continue;
		if( ToPassiveSkill( pSkill ) ) continue;
		if( pSkill->CheckUseBuff( m_pOwner ) ) continue;

		if( g_WeaponMgr.CheckRotationWeaponByItem(m_pOwner->GetCharName(), pItem->GetItemCreateIndex()) )
			continue;

		float fCurGauge = pItem->GetCurSkillGuage();
		float fMaxGauge = pItem->GetMaxSkillGauge(m_pOwner->GetCharName());
		float fRate = fCurGauge / fMaxGauge;

		if( fRate < fGaugeRate )
		{
			fGaugeRate = fRate;
			iSlot = i;
		}
	}

	return iSlot;
}

void ioRestorationGaugeBuff::GetLowestItemList( ioBaseChar* pOwner )
{
	m_vRecoverySlotList.clear();

	ioItem *pOwnerItem = m_pOwner->GetEquipedItem( m_ItemName );

	for( int i=0; i < 4; ++i )
	{
		ioItem *pItem = m_pOwner->GetEquipedItem( i );
		ioSkill *pSkill = m_pOwner->GetEquipedSkill( i );

		if( !pItem || !pSkill )	continue;
		//if( pItem == m_pOwnerItem && !m_bRestoreOwnerItem)	continue;
		if( pItem == pOwnerItem && !m_bRestoreOwnerItem)	continue;
		if( ToPassiveSkill( pSkill ) ) continue;
		if( pSkill->CheckUseBuff( m_pOwner ) ) continue;

		if( g_WeaponMgr.CheckRotationWeaponByItem(m_pOwner->GetCharName(), pItem->GetItemCreateIndex()) )
			continue;

		float fCurGauge = pItem->GetCurSkillGuage();
		float fMaxGauge = pItem->GetMaxSkillGauge(m_pOwner->GetCharName());

		// m_iSortReduceFullGauge�� TRUE�� ��� �������� ��� �� ��ų�� ����Ʈ���� ���ܵȴ�.
		if( m_iSortReduceFullGauge && fCurGauge == FLOAT100 )	continue;

		float fRate = fCurGauge / fMaxGauge;

		GaugeSlotInfo kInfo;
		kInfo.m_iSlotNum = i;
		kInfo.m_fGaugeRate = fRate;

		m_vRecoverySlotList.push_back( kInfo );
	}

	if( m_vRecoverySlotList.empty() )
	{
		m_iCurRecoveryItemCnt = 0;
		return;
	}

	// Sort Ÿ�Կ� ���� Sort ��� ����
	switch( m_iSortType )
	{
	case ST_SORT:
		std::sort( m_vRecoverySlotList.begin(), m_vRecoverySlotList.end(), GaugeSlotInfoSort() );
		break;
	case ST_RANDOM_SHUFFLE:
		std::random_shuffle( m_vRecoverySlotList.begin(), m_vRecoverySlotList.end() );
		break;
	}

	m_iCurRecoveryItemCnt = 4;

	int iCheckCnt = min( m_iRecoveryItemCnt, (int)m_vRecoverySlotList.size() );
	if( iCheckCnt > 0 )
	{
		DWORD dwSeed = m_pOwner->GetRandomSeed();

		IORandom random;
		random.SetRandomSeed( dwSeed );

		if( m_bRecoveryCountByRate )
		{
			m_iCurRecoveryItemCnt = 0;
			float fRate = 0.0f;
			float fRand = (float)random.Random( FLOAT100 ) * 0.01f;

			for( int i=0; i<4; i++ )
			{
				if( m_RecoveryCountRateList[i] == 0.0f )	continue;
				if( COMPARE( fRand, fRate, fRate + m_RecoveryCountRateList[i] ) )
				{
					m_iCurRecoveryItemCnt = i + 1;
				}
				fRate += m_RecoveryCountRateList[i];
			}
			if( m_iCurRecoveryItemCnt > iCheckCnt )
				m_iCurRecoveryItemCnt = iCheckCnt;
		}
		else
		{
			int iRand = random.Random( iCheckCnt );
			m_iCurRecoveryItemCnt = iRand + 1;
		}
	}
	else if( m_iRecoveryItemCnt == 0 )
	{
		m_iCurRecoveryItemCnt = (int)m_vRecoverySlotList.size();
	}


#ifdef ANTIHACK
	// ���⼭ ȸ�� Ÿ�Կ� ���� ������.. 	
	// slot Ÿ���ϰ��
	
	// ���⼭ ������ ���� ��ü ȸ���̶�, ��ų �������� ����..

	if( pOwner->IsNeedSendNetwork() )
	{
		if( g_MyInfo.GetUserIndex() == pOwner->GetUserIndex() )
		{
			if( !m_bRecoveryByRate )
			{
				int nSize = m_iCurRecoveryItemCnt;

				if( nSize > m_vRecoverySlotList.size() )
					nSize = m_vRecoverySlotList.size();

				if( nSize != 0 )
				{

					// ��ų ���� 1, 2, ���� ��ũ���� ����..
					float fRate = 1.f;

					DWORD dwBuffDuration = GetBuffDuration();
					if( dwBuffDuration != 0 )
					{
						int nCnt = dwBuffDuration / m_dwRecoveryTick;
						float fRecoverGauge = m_fRecoveryGauge * nCnt;
						fRate = fRecoverGauge * 0.01f;
						if( fRate > 1.f)
							fRate = 1.f;
					}

					//���⼭ ȸ�� ������ ���ؼ� �ð����� ����ؼ� �Ѱ���..
					SP2Packet kPacket( CUPK_ANTIHACK_SKILL );
					//LOG.PrintTimeAndLog( 0, "CUPK_ANTIHACK_SKILL Type1 - size(%d)", nSize );					
					kPacket << nSize;
					for( int i = 0; i < nSize; ++i )
					{
						int iSlot = m_vRecoverySlotList[i].m_iSlotNum;
						DWORD dwLatedTime = m_pOwner->GetEquipedItemLatedTime( iSlot, fRate );
						kPacket << iSlot << dwLatedTime;
						//LOG.PrintTimeAndLog( 0, "CUPK_ANTIHACK_SKILL RecoverSlotNum = %d", m_vRecoverySlotList[i].m_iSlotNum );
					}
					P2PNetwork::SendToAllPlayingUser( kPacket, pOwner );
				}
			}
			//rate �� ���� �ǽð� ����̶� ù ��꿡����..
			else
			{
				m_bNeedRateTypeSend = true;
				m_vecSlotRate.clear();
			}
		}
	}
#endif
}