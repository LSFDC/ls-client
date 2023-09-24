
#include "stdafx.h"

#include "ioQuest.h"
#include "ioTime.h"
#include "ioSteamClient.h"

QuestDataParent::QuestDataParent()
{
	m_dwIndexData = m_dwDateData = 0;
	m_pLinkQuest = NULL;
}

QuestDataParent::~QuestDataParent()
{

}

DWORD QuestDataParent::GetMainIndex()
{
	return m_dwIndexData / INDEX_HALF_VALUE;
}

DWORD QuestDataParent::GetSubIndex()
{
	return m_dwIndexData % INDEX_HALF_VALUE;
}

DWORD QuestDataParent::GetYear()
{
	return ioTime::GetYear( m_dwDateData );
}

DWORD QuestDataParent::GetMonth()
{
	return ioTime::GetMonth( m_dwDateData );
}

DWORD QuestDataParent::GetDay()
{
	return ioTime::GetDay( m_dwDateData );
}

DWORD QuestDataParent::GetHour()
{
	return ioTime::GetHour( m_dwDateData );
}

DWORD QuestDataParent::GetMinute()
{
	return ioTime::GetMinute( m_dwDateData );
}

QuestParent *QuestDataParent::GetLinkQuest()
{
	if( !m_pLinkQuest )
	{
		SetLinkQuest( g_QuestMgr.GetQuest( GetMainIndex(), GetSubIndex() ) );
	}
	return m_pLinkQuest;
}

void QuestDataParent::SetIndexData( DWORD dwIndexData )
{
	m_dwIndexData = dwIndexData;
}

void QuestDataParent::SetDateData( DWORD dwDateData )
{
	m_dwDateData = dwDateData;
}

void QuestDataParent::SetLinkQuest( QuestParent *pQuest )
{
	m_pLinkQuest = pQuest;
}

const int QuestDataParent::GetPerformType() const
{
	if( m_pLinkQuest == NULL ) return 0;

	return m_pLinkQuest->GetPerformType();
}
//////////////////////////////////////////////////////////////////////////
QuestData::QuestData()
{
	m_dwValueData = 0;
	m_dwIndexData = m_dwMagicData = m_dwDateData = 0;
	m_pLinkQuest = NULL;
	m_bChangeData = false;
}

QuestData::~QuestData()
{

}

DWORD QuestData::GetCurrentData()
{
	return m_dwValueData;
}

DWORD QuestData::GetCurrentMagicData()
{
	return m_dwMagicData / MAGIC_VALUE;
}

bool QuestData::IsAlarm()
{
	if( ( ( m_dwMagicData % MAGIC_VALUE ) / ALARM_VALUE ) == 0 )
		return false;
	return true;
}

DWORD QuestData::GetState()
{
	return ( m_dwMagicData % ALARM_VALUE );
}

void QuestData::SetMainIndex( DWORD dwMainIndex )
{
	m_dwIndexData = (dwMainIndex * INDEX_HALF_VALUE) + GetSubIndex();
	SetLinkQuest( g_QuestMgr.GetQuest( GetMainIndex(), GetSubIndex() ) );
}

void QuestData::SetSubIndex( DWORD dwSubIndex )
{
	m_dwIndexData = (GetMainIndex() * INDEX_HALF_VALUE) + dwSubIndex;
	SetLinkQuest( g_QuestMgr.GetQuest( GetMainIndex(), GetSubIndex() ) );
}

void QuestData::SetCurrentData( DWORD dwCurrentData )
{
	DWORD dwPrevData = GetCurrentData();
	m_dwValueData = dwCurrentData;

	if( dwCurrentData != dwPrevData )
		m_bChangeData = true;
}

void QuestData::SetCurrentMagicData( DWORD dwCurrentMagicData )
{
	DWORD dwPrevData = GetCurrentMagicData();
	m_dwMagicData = (dwCurrentMagicData * MAGIC_VALUE) + ((int)IsAlarm() * ALARM_VALUE) + GetState();

	if( dwCurrentMagicData != dwPrevData )
		m_bChangeData = true;
}

void QuestData::SetAlarm( bool bAlarm )
{
	m_dwMagicData = (GetCurrentMagicData() * MAGIC_VALUE) + ((int)bAlarm * ALARM_VALUE) + GetState();
}

void QuestData::SetState( DWORD dwState )
{
	m_dwMagicData = (GetCurrentMagicData() * MAGIC_VALUE) + ((int)IsAlarm() * ALARM_VALUE) + dwState;

	// �������� �ƴϸ� �˶� ����
	if( dwState != QS_PROGRESS ) 
		SetAlarm( false );
}

void QuestData::ApplyData( SP2Packet &rkPacket )
{
	rkPacket >> m_dwIndexData >> m_dwValueData >> m_dwMagicData >> m_dwDateData;
	SetLinkQuest( g_QuestMgr.GetQuest( GetMainIndex(), GetSubIndex() ) );
}
//////////////////////////////////////////////////////////////////////////
QuestCompleteData::QuestCompleteData()
{
	m_dwIndexData = m_dwDateData = 0;
	m_pLinkQuest = NULL;
}

QuestCompleteData::~QuestCompleteData()
{

}

void QuestCompleteData::ApplyData( SP2Packet &rkPacket )
{
	rkPacket >> m_dwIndexData >> m_dwDateData;
	SetLinkQuest( g_QuestMgr.GetQuest( GetMainIndex(), GetSubIndex() ) );
}
//////////////////////////////////////////////////////////////////////////
ioQuest::ioQuest()
{
}

ioQuest::~ioQuest()
{
	ClearQuestData();
}

void ioQuest::ClearQuestData()
{
	for(vQuestInfo::iterator iter = m_vItemList.begin(); iter != m_vItemList.end(); ++iter)
	{
		QuestData *pData = (*iter);
		SAFEDELETE( pData );
	}
	m_vItemList.clear();

	for(vQuestCompleteInfo::iterator iter = m_vCompleteList.begin(); iter != m_vCompleteList.end(); ++iter)
	{
		QuestCompleteData *pData = (*iter);
		SAFEDELETE( pData );
	}
	m_vCompleteList.clear();
}

void ioQuest::ApplyQuestData( SP2Packet &rkPacket )
{
	int iMaxQuest;
	rkPacket >> iMaxQuest;
	for(int i = 0;i < iMaxQuest;i++)
	{
		QuestData kData;
		kData.ApplyData( rkPacket );


		if( kData.GetIndexData() > 0 )
		{
			AddQuestData( kData );					
		}
	}
	g_QuestMgr.QuestInfoChange();
}

void ioQuest::ApplyQuestCompleteData( SP2Packet &rkPacket )
{
	int iMaxQuest;
	rkPacket >> iMaxQuest;
	for(int i = 0;i < iMaxQuest;i++)
	{
		QuestCompleteData kData;
		kData.ApplyData( rkPacket );

		if( kData.GetIndexData() > 0 )
		{
			AddQuestCompleteData( kData );					
		}
	}
	g_QuestMgr.QuestInfoChange();

}

void ioQuest::ApplyQuestOccurData( SP2Packet &rkPacket )
{
	int iMaxQuest;
	rkPacket >> iMaxQuest;

	for(int i = 0;i < iMaxQuest;i++)
	{
		QuestData kData;
		kData.ApplyData( rkPacket );

		if( kData.GetIndexData() > 0 )
		{
			AddQuestData( kData );

			// �̹� �޼��� ����Ʈ���� Ȯ��
			g_QuestMgr.QuestAlreadyAttain( kData.GetMainIndex(), kData.GetSubIndex() );
		}
	}
	g_QuestMgr.QuestInfoChange();
}

void ioQuest::ApplyQuestRewardData( SP2Packet &rkPacket )
{
	int iMaxQuest;
	rkPacket >> iMaxQuest;
	for(int i = 0;i < iMaxQuest;i++)
	{
		QuestCompleteData kData;
		kData.ApplyData( rkPacket );

		// ��� ���� ���� ó��
		g_QuestMgr.DirectPresentAlarm( kData.GetMainIndex(), kData.GetSubIndex(), rkPacket );

		if( kData.GetIndexData() > 0 )
		{
			// ��Ͽ��� ����
			RemoveQuestData( kData.GetMainIndex(), kData.GetSubIndex() ); 

			QuestParent *pQuestParent = g_QuestMgr.GetQuest( kData.GetMainIndex(), kData.GetSubIndex() );
			if( pQuestParent )
			{
				pQuestParent->ProcessRewardComplete();
				if( !pQuestParent->IsRepeatStyle() )
				{
					// �Ϸ� ���� ����
					AddQuestCompleteData( kData );
				}
			}		

			// ���� ���� ����Ʈ�� �ִ��� Ȯ��
			g_QuestMgr.QuestCompleteNextOccur( kData.GetMainIndex(), kData.GetSubIndex() );
		}
	}
	g_QuestMgr.QuestInfoChange();
}

void ioQuest::AddQuestData( QuestData &rkData )
{
	QuestParent *pQuestParent = g_QuestMgr.GetQuest( rkData.GetMainIndex(), rkData.GetSubIndex() );
	if( pQuestParent )
	{
		switch( rkData.GetState() )
		{
		case QS_ATTAIN:
			{
				g_QuestMgr.AddQuestAttainList( pQuestParent );

#if defined( USE_GA )
				char chQuestIndex[32]	= {0,};
				char chSlash[16]		= {0,};
				char chPostData[256]	= {0,};

				if ( ioLocalManager::GetLocalType() == ioLocalManager::LCT_KOREA )
				{
					sprintf_e( chQuestIndex, "%d-%d", (int)rkData.GetMainIndex(), (int)rkData.GetSubIndex() );
					sprintf_e( chSlash, "%%2F" );
					sprintf_e( chPostData, "%sQUEST%sCOMPLETE%s%s", chSlash, chSlash, chSlash, chQuestIndex );
				}
				else
				{
					SafeSprintf( chQuestIndex, sizeof(chQuestIndex), "%1-%2", (int)rkData.GetMainIndex(), (int)rkData.GetSubIndex() );
					SafeSprintf( chSlash, sizeof(chSlash), "%2F" );
					SafeSprintf( chPostData, sizeof(chPostData), "%1QUEST%2COMPLETE%3%4", chSlash, chSlash, chSlash, chQuestIndex );
				}				

				// QUEST_STATE_COMPLETE
				g_HttpMng.GA_EventTracking( g_MyInfo.GetUserIndex()
					, "Quest"
					, "Complete"
					, ""
					, 1
					, chPostData );
#endif

			}
			
			break;
		case QS_COMPLETE:
			g_QuestMgr.EraseQuestAttainList( rkData.GetMainIndex(), rkData.GetSubIndex() );	
			break;
		}
	}

	// �̹� �����ϰ� �ִ� ����Ʈ�� ����� ���� ����Ʈ�� �߰��Ѵ�.
	vQuestInfo::iterator iter, iEnd;
	iEnd = m_vItemList.end();
	for(iter = m_vItemList.begin();iter != iEnd;iter++)
	{
		QuestData *pItemDB = *iter;
		if( !pItemDB ) continue;

		if( pItemDB->GetMainIndex() != rkData.GetMainIndex() ) continue;

		if( rkData.GetSubIndex() >= pItemDB->GetSubIndex() )
		{
			*pItemDB = rkData;
			std::sort( m_vItemList.begin(), m_vItemList.end(), QuestDataSort() );
			return;          // �̹� �ִ� ����Ʈ�� ������ ����
		}
		else
		{
			LOG.PrintTimeAndLog( 0, "���� �Ϸ�� ����Ʈ�� ȹ���Ͽ���!!!! : %d - %d", rkData.GetMainIndex(), rkData.GetSubIndex() );
			return;          // Error
		}
	}
	QuestData *pData = new QuestData;
	if( !pData )
		return;
	*pData = rkData;
	m_vItemList.insert( m_vItemList.begin(), pData );
	std::sort( m_vItemList.begin(), m_vItemList.end(), QuestDataSort() );
	g_QuestMgr.QuestInfoChange();

	if( pQuestParent )
		pQuestParent->StartProcessTime();
}

void ioQuest::AddQuestCompleteData( QuestCompleteData &rkData )
{
	g_QuestMgr.EraseQuestAttainList( rkData.GetMainIndex(), rkData.GetSubIndex() );	

	// �̹� �����ϰ� �ִ� ����Ʈ�� ����� ���� ����Ʈ�� �߰��Ѵ�.
	vQuestCompleteInfo::iterator iter, iEnd;
	iEnd = m_vCompleteList.end();
	for(iter = m_vCompleteList.begin();iter != iEnd;iter++)
	{
		QuestCompleteData *pItemDB = *iter;
		if( !pItemDB ) continue;
		if( pItemDB->GetMainIndex() != rkData.GetMainIndex() ) continue;

		if( rkData.GetSubIndex() >= pItemDB->GetSubIndex() )
		{
			*pItemDB = rkData;
			std::sort( m_vCompleteList.begin(), m_vCompleteList.end(), QuestCompleteDataSort() );
			return;          // �̹� �ִ� ����Ʈ�� ������ ����
		}
		else
		{
			LOG.PrintTimeAndLog( 0, "���� �Ϸ�� ����Ʈ�� �Ϸ��Ͽ���!!!! : %d - %d", rkData.GetMainIndex(), rkData.GetSubIndex() );
			return;          // Error
		}
	}

	QuestCompleteData *pData = new QuestCompleteData;
	if( !pData )
		return;
	*pData = rkData;
	m_vCompleteList.insert( m_vCompleteList.begin(), pData );
	std::sort( m_vCompleteList.begin(), m_vCompleteList.end(), QuestCompleteDataSort() );
	g_QuestMgr.QuestInfoChange();
}

bool ioQuest::RemoveQuestData( DWORD dwMainIndex, DWORD dwSubIndex )
{
	bool bRemove = false;
	vQuestInfo::iterator iter, iEnd;
	iEnd = m_vItemList.end();
	for(iter = m_vItemList.begin();iter != iEnd;iter++)
	{
		QuestData *pItemDB = *iter;
		if( !pItemDB ) continue;
		if( pItemDB->GetMainIndex() != dwMainIndex ) continue;
		if( pItemDB->GetSubIndex() != dwSubIndex ) continue;
		
		bRemove = true;
		SAFEDELETE( pItemDB );
		m_vItemList.erase( iter );
		break;
	}

	if( bRemove )
	{
		std::sort( m_vItemList.begin(), m_vItemList.end(), QuestDataSort() );
		g_QuestMgr.QuestInfoChange();
	}
	return bRemove;
}

bool ioQuest::RemoveQuestCompleteData( DWORD dwMainIndex, DWORD dwSubIndex )
{
	bool bRemove = false;
	vQuestCompleteInfo::iterator iter, iEnd;
	iEnd = m_vCompleteList.end();
	for(iter = m_vCompleteList.begin();iter != iEnd;iter++)
	{
		QuestCompleteData *pItemDB = *iter;
		if( !pItemDB ) continue;
		if( pItemDB->GetMainIndex() != dwMainIndex ) continue;
		if( pItemDB->GetSubIndex() != dwSubIndex ) continue;

		bRemove = true;
		SAFEDELETE( pItemDB );
		m_vCompleteList.erase( iter );
		break;
	}

	if( bRemove )
	{
		std::sort( m_vCompleteList.begin(), m_vCompleteList.end(), QuestCompleteDataSort() );
		g_QuestMgr.QuestInfoChange();
	}
	return bRemove;
}

bool ioQuest::RemoveQuestOneDayAll()
{
	bool bRemove = false;
	vQuestCompleteInfo::iterator iter;
	for(iter = m_vCompleteList.begin();iter != m_vCompleteList.end();)
	{
		QuestCompleteData *pItemDB = *iter;
		if( !pItemDB || !pItemDB->GetLinkQuest() || !pItemDB->GetLinkQuest()->IsOneDayStyle() )
		{
			iter++;
			continue;
		}

		bRemove = true;
		SAFEDELETE( pItemDB );
		iter = m_vCompleteList.erase( iter );
	}

	if( bRemove )
	{
		std::sort( m_vCompleteList.begin(), m_vCompleteList.end(), QuestCompleteDataSort() );
		g_QuestMgr.QuestInfoChange();
	}
	return bRemove;
}

bool ioQuest::IsQuestAttain( DWORD dwMainIndex, DWORD dwSubIndex )
{
	vQuestInfo::iterator iter, iEnd;
	iEnd = m_vItemList.end();
	for(iter = m_vItemList.begin();iter != iEnd;iter++)
	{
		QuestData *pItemDB = *iter;
		if( !pItemDB ) continue;
		if( pItemDB->GetMainIndex() != dwMainIndex ) continue;

		if( pItemDB->GetState() == QS_ATTAIN )
			return true;
		return false;
	}
	return false;
}

bool ioQuest::IsQuestComplete( DWORD dwMainIndex, DWORD dwSubIndex )
{
	vQuestCompleteInfo::iterator iter, iEnd;
	iEnd = m_vCompleteList.end();
	for(iter = m_vCompleteList.begin();iter != iEnd;iter++)
	{
		QuestCompleteData *pItemDB = *iter;
		if( !pItemDB ) continue;
		if( pItemDB->GetMainIndex() != dwMainIndex ) continue;
		// ���� �ε����� �Ϸ�� ����Ʈ�� �Ǵ�
		if( dwSubIndex <= pItemDB->GetSubIndex() )   
			return true;
		return false;
	}
	return false;
}

bool ioQuest::IsQuestIndexCheck( DWORD dwMainIndex, DWORD dwSubIndex )
{
	{
		vQuestInfo::iterator iter, iEnd;
		iEnd = m_vItemList.end();
		for(iter = m_vItemList.begin();iter != iEnd;iter++)
		{
			QuestData *pItemDB = *iter;
			if( !pItemDB ) continue;
			if( pItemDB->GetMainIndex() != dwMainIndex ) continue;

			// ���� �ε����� ���������� �Ǵ�.
			if( dwSubIndex <= pItemDB->GetSubIndex() )   
				return true;
		}
	}

	{
		vQuestCompleteInfo::iterator iter, iEnd;
		iEnd = m_vCompleteList.end();
		for(iter = m_vCompleteList.begin();iter != iEnd;iter++)
		{
			QuestCompleteData *pItemDB = *iter;
			if( !pItemDB ) continue;
			if( pItemDB->GetMainIndex() != dwMainIndex ) continue;

			// ���� �ε����� ���������� �Ǵ�.
			if( dwSubIndex <= pItemDB->GetSubIndex() )   
				return true;
		}
	}
	return false;
}

int ioQuest::GetMaxQuest( DWORD dwState )
{
	int iCount = 0;	
	{
		vQuestInfo::iterator iter, iEnd;
		iEnd = m_vItemList.end();
		for(iter = m_vItemList.begin();iter != iEnd;iter++)
		{
			QuestData *pItemDB = *iter;
			if( !pItemDB ) continue;
			if( pItemDB->GetState() == dwState )
				iCount++;
		}
	}

	if( dwState == QS_COMPLETE )
	{
		vQuestCompleteInfo::iterator iter, iEnd;
		iEnd = m_vCompleteList.end();
		for(iter = m_vCompleteList.begin();iter != iEnd;iter++)
		{
			QuestCompleteData *pItemDB = *iter;
			if( !pItemDB ) continue;
			iCount += pItemDB->GetSubIndex();
		}
	}
	return iCount;
}

int ioQuest::GetMaxAlarmQuest()
{
	int iCount = 0;
	vQuestInfo::iterator iter, iEnd;
	iEnd = m_vItemList.end();
	for(iter = m_vItemList.begin();iter != iEnd;iter++)
	{
		QuestData *pItemDB = *iter;
		if( !pItemDB ) continue;
		if( pItemDB->IsAlarm() )
			iCount++;
	}
	return iCount;
}

QuestData &ioQuest::GetQuestData( DWORD dwMainIndex, DWORD dwSubIndex )
{
	vQuestInfo::iterator iter, iEnd;
	iEnd = m_vItemList.end();
	for(iter = m_vItemList.begin();iter != iEnd;iter++)
	{
		QuestData *pItemDB = *iter;
		if( !pItemDB ) continue;
		if( pItemDB->GetMainIndex() == dwMainIndex &&
			pItemDB->GetSubIndex() == dwSubIndex )
		{
			return *pItemDB;
		}
	}

	static QuestData kNoneItem;
	return kNoneItem;
}

DWORD ioQuest::GetQuestState( DWORD dwMainIndex, DWORD dwSubIndex )
{
	{
		vQuestInfo::iterator iter, iEnd;
		iEnd = m_vItemList.end();
		for(iter = m_vItemList.begin();iter != iEnd;iter++)
		{
			QuestData *pItemDB = *iter;
			if( !pItemDB ) continue;
			if( pItemDB->GetMainIndex() == dwMainIndex )			
			{
				if( dwSubIndex == pItemDB->GetSubIndex() )
					return pItemDB->GetState();
			}
		}
	}

	{
		vQuestCompleteInfo::iterator iter, iEnd;
		iEnd = m_vCompleteList.end();
		for(iter = m_vCompleteList.begin();iter != iEnd;iter++)
		{
			QuestCompleteData *pItemDB = *iter;
			if( !pItemDB ) continue;
			if( pItemDB->GetMainIndex() == dwMainIndex )			
			{
				if( dwSubIndex <= pItemDB->GetSubIndex() )
					return QS_COMPLETE; 
			}
		}
	}
	return 0xFFFFFFFF;
}

QuestData ioQuest::GetProcessToArray( int iArray )
{
	vQuestInfo::iterator iter;
	for(iter = m_vItemList.begin();iter != m_vItemList.end();iter++)
	{
		QuestData *pItemDB = *iter;
		if( !pItemDB ) continue;
		if( pItemDB->GetState() == QS_PROGRESS )
		{
			if( --iArray < 0 )
			{
				return *pItemDB;
			}
		}
	}

	static QuestData kNoneItem;
	return kNoneItem;
}


QuestData ioQuest::GetAttainNCompleteToArray( int iArray )
{
	// ���� �޼� ����Ʈ �˻���
	{
		vQuestInfo::iterator iter;
		for(iter = m_vItemList.begin();iter != m_vItemList.end();iter++)
		{
			QuestData *pItemDB = *iter;
			if( !pItemDB ) continue;
			if( pItemDB->GetState() == QS_ATTAIN )
			{
				if( --iArray < 0 )
				{
					return *pItemDB;
				}
			}
		}
	}

	// ���� �Ϸ� ����Ʈ�� ���� ����Ʈ�� �˻��ؾ��Ѵ�.
	{
		vQuestCompleteInfo::iterator iter, iEnd;
		iEnd = m_vCompleteList.end();
		for(iter = m_vCompleteList.begin();iter != iEnd;iter++)
		{
			QuestCompleteData *pItemDB = *iter;
			if( !pItemDB ) continue;
			QuestData kVirtualItem;
			// ���� ����Ʈ�� �Ϸ��� ����Ʈ�̹Ƿ� �������� �Ϸ�� ����Ʈ�� ������.
			for(int iSubIndex = ( (int)pItemDB->GetSubIndex() ) - 1;iSubIndex >= 1;iSubIndex--)
			{
				if( --iArray < 0 )
				{
					kVirtualItem.SetMainIndex( pItemDB->GetMainIndex() );
					kVirtualItem.SetSubIndex( iSubIndex );
					kVirtualItem.SetState( QS_COMPLETE );
					return kVirtualItem;
				}
			}

			if( --iArray < 0 )
			{
				kVirtualItem.SetIndexData( pItemDB->GetIndexData() );
				kVirtualItem.SetState( QS_COMPLETE );
				return kVirtualItem;
			}
		}
	}

	static QuestData kNoneItem;
	return kNoneItem;
}

QuestData ioQuest::GetAlarmQuestToArray( int iArray )
{
	vQuestInfo::iterator iter;
	for(iter = m_vItemList.begin();iter != m_vItemList.end();iter++)
	{
		QuestData *pItemDB = *iter;
		if( !pItemDB ) continue;
		if( pItemDB->IsAlarm() )
		{
			if( --iArray < 0 )
			{
				return *pItemDB;
			}
		}
	}

	static QuestData kNoneItem;
	return kNoneItem;
}