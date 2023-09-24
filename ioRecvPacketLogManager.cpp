#include "stdafx.h"
#include "ioRecvPacketLogManager.h"

ioRecvInfoList::ioRecvInfoList()
{
	m_RecvList.clear();
}

ioRecvInfoList::~ioRecvInfoList()
{
	DeleteAll();
}

void ioRecvInfoList::AddInfo( ioHashString szName, DWORD Time )
{
	//���� �߰�
	RecvPacketInfo* sInfo = new RecvPacketInfo;
	//���� �ʱ�ȭ
	sInfo->Init( szName, Time, Time );
	m_RecvList.push_back( sInfo );			
}

ioRecvInfoList::RecvPacketInfo* ioRecvInfoList::FindInfo( ioHashString szName )
{
	//����ٸ� ����
	if ( m_RecvList.empty() )
		return NULL;

	//��� ���鼭
	InfoList::iterator Iter = m_RecvList.begin();
	for ( ; Iter!=m_RecvList.end() ; ++Iter)
	{
		RecvPacketInfo* pRecvInfo = *Iter;
		//���� �̸��̸� ��ȯ
		if ( pRecvInfo && pRecvInfo->GetName() == szName )
		{
			return pRecvInfo;
		}
	}
	return NULL;
}

void ioRecvInfoList::PrintInfo( DWORD dwColor, ioHashString szCharName, int eType, ioHashString szRecvName, bool bReset )
{
	//����ٸ� ����
	if ( m_RecvList.empty() )
		return;

	//��� ���鼭
	InfoList::iterator Iter = m_RecvList.begin();
	for ( ; Iter!=m_RecvList.end() ; ++Iter)
	{
		RecvPacketInfo* pRecvInfo = *Iter;
		//������ �ְ�, �̸��� ����ų�, �����鼭, ī��Ʈ�� 0���� ũ�� ���
		if ( pRecvInfo && ( szRecvName.IsEmpty() || pRecvInfo->GetName() == szRecvName ) && pRecvInfo->GetCnt() > 0  )
		{			
			char buf[6][MAX_PATH];
			char total[MAX_PATH];

			//ĳ���� �̸�, Ÿ��, ���� �̸�, Ƚ��, ���۽ð�, ���� �ð�
			wsprintf_e( buf[0], "CharName : %s", szCharName );
			wsprintf_e( buf[1], "Type : %d", eType );
			wsprintf_e( buf[2], "RecvName : %s", pRecvInfo->GetName() );
			wsprintf_e( buf[3], "Cnt : %d", pRecvInfo->GetCnt() );
			wsprintf_e( buf[4], "StartTime : %d", pRecvInfo->GetStartTime() );
			wsprintf_e( buf[5], "RecvTime : %d", pRecvInfo->GetRecvTime() );
			wsprintf_e( total,"%s, %s, %s, %s, %s, %s", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5] );

			ChatMsg( dwColor, g_MyInfo.GetPublicID().c_str(), total );
			
			//���� ����
			if ( bReset )
			{
				pRecvInfo->Reset();
			}
		}
	}
}

void ioRecvInfoList::ChatMsg( DWORD dwColor, const char *szID, const char *szChat )
{
	g_ChatMgr.SetRecvChatMsg( dwColor, szID, szChat );
}

void ioRecvInfoList::DeleteAll()
{
	//����ٸ� ����
	if ( m_RecvList.empty() )
		return;

	//��� ���鼭 ����
	InfoList::iterator Iter = m_RecvList.begin();
	for ( ; Iter!=m_RecvList.end() ; ++Iter)
	{
		RecvPacketInfo* pRecvInfo = *Iter;
		if (pRecvInfo)
		{
			SAFEDELETE( pRecvInfo );
		}
	}
	m_RecvList.erase( m_RecvList.begin(), m_RecvList.end() );
	m_RecvList.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////
template<> ioRecvPacketLogManager* Singleton< ioRecvPacketLogManager >::ms_Singleton = 0;

ioRecvPacketLogManager::ioRecvPacketLogManager()
{
}

ioRecvPacketLogManager::~ioRecvPacketLogManager()
{
	CleanRecvInfo();
}

ioRecvPacketLogManager& ioRecvPacketLogManager::GetSingleton()
{
	return Singleton< ioRecvPacketLogManager >::GetSingleton();
}

void ioRecvPacketLogManager::AddCharRecvInfo( ioHashString szCharName, RecvPacketType eType, ioHashString szRecvName )
{
	//����ִٸ� ����
	if ( szCharName.IsEmpty() || szRecvName.IsEmpty() || eType >= RPT_MAX )
		return;

	//ĳ���� �˻�
	TotalInfoList::iterator First_Find_Iter = m_TotalInfoList.find( szCharName );

	//ĳ���� �̸��� �� ã�Ҵٸ� ����Ʈ�� �߰�
	if ( First_Find_Iter == m_TotalInfoList.end() )
	{
		TypeList* pTypeList = new TypeList;
		pTypeList->clear();

		for ( int i = 0; i<RPT_MAX; ++i )
		{
			//Ÿ�� ���� ����Ʈ �߰�
			ioRecvInfoList* pRecvInfoList = new ioRecvInfoList;		
			pTypeList->insert( std::make_pair( (RecvPacketType)i, pRecvInfoList ) );
		}

		//ĳ���� ����Ʈ �߰�
		m_TotalInfoList.insert( std::make_pair( szCharName, pTypeList) );
		
		//ĳ���� �ٽ� �˻�
		First_Find_Iter = m_TotalInfoList.find( szCharName );	
	}
	
	//ĳ���Ͱ� �ִٸ�
	//�ش� ĳ������ ��Ŷ ����Ʈ�� ���
	TypeList* pTypeList = First_Find_Iter->second;
	if ( pTypeList )
	{
		//�ش� Ÿ���� ����Ʈ�� ��� (ĳ���� ����Ʈ�� �ִٸ� Ÿ�Ժ� ����Ʈ�� ������)
		TypeList::iterator Second_Find_Iter = pTypeList->find( eType );
			
		ioRecvInfoList* pRecvInfoList = Second_Find_Iter->second;
		if ( pRecvInfoList )
		{
			DWORD RecvTime = FRAMEGETTIME();
			ioRecvInfoList::RecvPacketInfo* pInfo = pRecvInfoList->FindInfo( szRecvName );
			//ã�Ҵٸ� ���� ��Ű�� ����
			if ( pInfo )
			{
				if ( pInfo->GetStartTime() == 0)
				{
					pInfo->SetStartTime( RecvTime );
				}
				pInfo->AddCnt( RecvTime );
				return;
			}
			//��ã���� ���� ����
			pRecvInfoList->AddInfo( szRecvName, RecvTime );			
		}
	}
}

ioRecvInfoList::RecvPacketInfo* ioRecvPacketLogManager::FindCharRecvInfo( ioHashString szCharName, RecvPacketType eType, ioHashString szRecvName )
{
	//����ִٸ� ����
	if ( szCharName.IsEmpty() || szRecvName.IsEmpty() || eType >= RPT_MAX || m_TotalInfoList.empty() )
		return NULL;

	//ĳ���� �˻�
	TotalInfoList::iterator First_Find_Iter = m_TotalInfoList.find( szCharName );

	//ã�Ҵٸ�
	if ( First_Find_Iter != m_TotalInfoList.end() )
	{
		//ĳ������ ����Ʈ�� ���
		TypeList* pTypeList = First_Find_Iter->second;
		if ( pTypeList )
		{
			//�ش� Ÿ���� ����Ʈ�� ��� (ĳ���� ����Ʈ�� �ִٸ� Ÿ�Ժ� ����Ʈ�� ������)
			TypeList::iterator Second_Find_Iter = pTypeList->find( eType );

			ioRecvInfoList* pRecvInfoList = Second_Find_Iter->second;
			if ( pRecvInfoList )
			{
				return pRecvInfoList->FindInfo( szRecvName );
			}
		}
	}

	return NULL;
}

void ioRecvPacketLogManager::PrintCharRecvInfo( DWORD dwColor, ioHashString szCharName, RecvPacketType eType, ioHashString szRecvName,  bool bReset )
{
	//����ִٸ� ����
	if ( szCharName.IsEmpty() || eType >= RPT_MAX || m_TotalInfoList.empty() )
		return;

	//ĳ���� �˻�
	TotalInfoList::iterator First_Find_Iter = m_TotalInfoList.find( szCharName );

	//ã�Ҵٸ�
	if ( First_Find_Iter != m_TotalInfoList.end() )
	{
		//ĳ������ ����Ʈ�� ���
		TypeList* pTypeList = First_Find_Iter->second;
		if ( pTypeList )
		{
			//�ش� Ÿ���� ����Ʈ�� ��� (ĳ���� ����Ʈ�� �ִٸ� Ÿ�Ժ� ����Ʈ�� ������)
			TypeList::iterator Second_Find_Iter = pTypeList->find( eType );

			ioRecvInfoList* pRecvInfoList = Second_Find_Iter->second;
			//�ִٸ� ���
			if ( pRecvInfoList )
			{
				pRecvInfoList->PrintInfo( dwColor, szCharName, (int)eType, szRecvName, bReset );
			}
		}
	}
}

void ioRecvPacketLogManager::PrintChatLog( DWORD dwColor, ioHashString szCharName, ioHashString szLog )
{
	g_ChatMgr.SetRecvChatMsg( dwColor, szCharName.c_str(), szLog.c_str() );
}

void ioRecvPacketLogManager::CleanRecvInfo()
{
	//����ִٸ� ����
	if ( m_TotalInfoList.empty() )
	{
		return;
	}

	//��� ĳ���� ����Ʈ�� ���鼭
	TotalInfoList::iterator First_Iter = m_TotalInfoList.begin();
	for ( ; First_Iter!=m_TotalInfoList.end() ; )
	{
		//ĳ������ Ÿ�� ����Ʈ�� ���
		TypeList* pTypeList = First_Iter->second;
		if ( pTypeList )
		{
			//ĳ������ ��� Ÿ�Ժ� ����Ʈ�� ���鼭
			TypeList::iterator Second_Iter = pTypeList->begin();
			for ( ; Second_Iter!=pTypeList->end() ; )
			{
				//���� ��Ŷ�� ���� ����Ʈ�� ���鼭
				ioRecvInfoList* pRecvInfoList = Second_Iter->second;
				if ( pRecvInfoList )
				{
					//����
					SAFEDELETE( pRecvInfoList );
				}
				//�ش� ĳ������ Ÿ�Ժ� ����Ʈ���� ���
				Second_Iter = pTypeList->erase( Second_Iter );
			}
			pTypeList->clear();
			SAFEDELETE( pTypeList );
		}
		//��� ĳ���� ����Ʈ���� ���
		First_Iter = m_TotalInfoList.erase( First_Iter );
	}

	//��� ���
	m_TotalInfoList.clear();
}
