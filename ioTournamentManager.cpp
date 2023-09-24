
#include "stdafx.h"

#include "ioTournamentManager.h"

#include "GUI/ioMessageBox.h"


template<> ioTournamentManager* Singleton< ioTournamentManager >::ms_Singleton = 0;
ioTournamentManager::ioTournamentManager()
{
	m_dwRegularTeamCreateUITimer	= 0;
	m_dwRegularTourRequestTime		= 0;
}

ioTournamentManager::~ioTournamentManager()
{
	TournamentVec::iterator iter = m_TournamentList.begin();
	for( ;iter != m_TournamentList.end();++iter )
	{
		SAFEDELETE( *iter );
	}
	m_TournamentList.clear();
}

ioTournamentManager& ioTournamentManager::GetSingleton()
{
	return Singleton< ioTournamentManager >::GetSingleton();
}

void ioTournamentManager::Initialize()
{

}

void ioTournamentManager::Process()
{

}

void ioTournamentManager::CreateTournamentRegular( DWORD dwIndex, int iRegularResourceType )
{
	TournamentVec::iterator iter = m_TournamentList.begin();
	for(;iter != m_TournamentList.end();++iter)
	{
		TournamentNode *pTournament = *iter;
		if( pTournament )
		{
			if( pTournament->GetIndex() == dwIndex )
			{
				// �̹� ������� ���� ����
				pTournament->SetType( TournamentNode::TYPE_REGULAR );
				pTournament->SetReguralResourceType( iRegularResourceType );
				return;
			}
		}
	}

	// ������ insert
	TournamentNode *pNewTournament = new TournamentNode;
	pNewTournament->SetIndex( dwIndex );
	pNewTournament->SetReguralResourceType( iRegularResourceType );
	pNewTournament->SetType( TournamentNode::TYPE_REGULAR );
	m_TournamentList.push_back( pNewTournament );
}

void ioTournamentManager::CreateTournamentCustom( DWORD dwIndex, DWORD dwOwnerIndex, DWORD dwBannerB, DWORD dwBannerS )
{
	TournamentVec::iterator iter = m_TournamentList.begin();
	for(;iter != m_TournamentList.end();++iter)
	{
		TournamentNode *pTournament = *iter;
		if( pTournament )
		{
			if( pTournament->GetIndex() == dwIndex )
			{
				// �̹� ������� ���� ����
				pTournament->SetType( TournamentNode::TYPE_CUSTOM );
				pTournament->SetOwnerIndex( dwOwnerIndex );
				pTournament->SetCustomBanner( dwBannerB, dwBannerS );
				return;
			}
		}
	}

	// ������ insert
	TournamentNode *pNewTournament = new TournamentNode;
	pNewTournament->SetIndex( dwIndex );
	pNewTournament->SetOwnerIndex( dwOwnerIndex );
	pNewTournament->SetCustomBanner( dwBannerB, dwBannerS );
	pNewTournament->SetType( TournamentNode::TYPE_CUSTOM );
	m_TournamentList.push_back( pNewTournament );

}

void ioTournamentManager::CreateTournament( DWORD dwIndex )
{
	TournamentVec::iterator iter = m_TournamentList.begin();
	for(;iter != m_TournamentList.end();++iter)
	{
		TournamentNode *pTournament = *iter;
		if( pTournament )
		{
			if( pTournament->GetIndex() == dwIndex )
				return;
		}
	}

	// ������ insert
	TournamentNode *pNewTournament = new TournamentNode;
	pNewTournament->SetIndex( dwIndex );
	m_TournamentList.push_back( pNewTournament );
}

void ioTournamentManager::ApplyTournamentRegularInfo( SP2Packet &rkPacket )
{
	BYTE  State;
	DWORD dwIndex;
	int iReguralResource;
	bool bDisable;
	rkPacket >> dwIndex >> State >> iReguralResource >> bDisable;

	TournamentVec::iterator iter = m_TournamentList.begin();
	for(;iter != m_TournamentList.end();++iter)
	{
		TournamentNode *pTournament = *iter;
		if( pTournament )
		{
			if( pTournament->GetIndex() == dwIndex )
			{
				// �̹� ������� ���� ����
				pTournament->SetState( State );
				pTournament->SetReguralResourceType( iReguralResource );
				pTournament->SetReguralDisableTournament( bDisable );
				return;
			}
		}
	}

	// ������ insert
	TournamentNode *pNewTournament = new TournamentNode;
	pNewTournament->SetIndex( dwIndex );
	pNewTournament->SetState( State );
	pNewTournament->SetReguralResourceType( iReguralResource );
	pNewTournament->SetType( TournamentNode::TYPE_REGULAR );
	pNewTournament->SetReguralDisableTournament( bDisable );
	m_dwRegularTourRequestTime = FRAMEGETTIME();

	m_TournamentList.push_back( pNewTournament );
}

void ioTournamentManager::ApplyTournamentMainInfo( SP2Packet &rkPacket )
{
	DWORD dwIndex;
	rkPacket >> dwIndex;

	TournamentVec::iterator iter = m_TournamentList.begin();
	for(;iter != m_TournamentList.end();++iter)
	{
		TournamentNode *pTournament = *iter;
		if( pTournament )
		{
			if( pTournament->GetIndex() == dwIndex )
			{
				pTournament->ApplyMainInfo( rkPacket );
				return;
			}
		}
	}
}

void ioTournamentManager::ApplyTournamentScheduleInfo( SP2Packet &rkPacket )
{
	DWORD dwIndex;
	rkPacket >> dwIndex;

	TournamentVec::iterator iter = m_TournamentList.begin();
	for(;iter != m_TournamentList.end();++iter)
	{
		TournamentNode *pTournament = *iter;
		if( pTournament )
		{
			if( pTournament->GetIndex() == dwIndex )
			{
				pTournament->ApplyScheduleInfo( rkPacket );
				return;
			}
		}
	}
}

void ioTournamentManager::ApplyTournamentRoundTeamData( SP2Packet &rkPacket )
{
	DWORD dwTourIndex;
	rkPacket >> dwTourIndex;

	TournamentNode *pTournament = GetTournament( dwTourIndex );
	if( pTournament == NULL ) return; // ����� ��ȸ

	pTournament->ApplyRoundTeamData( rkPacket );
}

DWORD ioTournamentManager::GetRegularIndex()
{
	TournamentVec::iterator iter = m_TournamentList.begin();
	for(;iter != m_TournamentList.end();++iter)
	{
		TournamentNode *pTournament = *iter;
		if( pTournament )
		{
			if( pTournament->GetType() == TournamentNode::TYPE_REGULAR )
				return pTournament->GetIndex();
		}
	}
	return 0;     // ���� ��ȸ ����.
}

int ioTournamentManager::GetRegularState()
{
	TournamentVec::iterator iter = m_TournamentList.begin();
	for(;iter != m_TournamentList.end();++iter)
	{
		TournamentNode *pTournament = *iter;
		if( pTournament )
		{
			if( pTournament->GetType() == TournamentNode::TYPE_REGULAR )
				return pTournament->GetState();
		}
	}
	return -1;     // ���� ��ȸ ����.
}

int ioTournamentManager::GetRegularResourceType()
{
	TournamentVec::iterator iter = m_TournamentList.begin();
	for(;iter != m_TournamentList.end();++iter)
	{
		TournamentNode *pTournament = *iter;
		if( pTournament )
		{
			if( pTournament->GetType() == TournamentNode::TYPE_REGULAR )
				return pTournament->GetReguralResourceType();
		}
	}
	return 1;     // ���� ��ȸ ����.
}

bool ioTournamentManager::IsRegularDisableTournament()
{
	TournamentVec::iterator iter = m_TournamentList.begin();
	for(;iter != m_TournamentList.end();++iter)
	{
		TournamentNode *pTournament = *iter;
		if( pTournament )
		{
			if( pTournament->GetType() == TournamentNode::TYPE_REGULAR )
				return pTournament->isDisableTournament();
		}
	}
	return false;     // ���� ��ȸ ����.
}

const ioHashString& ioTournamentManager::GetRegularDisableTournamentMent()
{
	static ioHashString szNone;
	TournamentVec::iterator iter = m_TournamentList.begin();
	for(;iter != m_TournamentList.end();++iter)
	{
		TournamentNode *pTournament = *iter;
		if( pTournament )
		{
			if( pTournament->GetType() == TournamentNode::TYPE_REGULAR )
				return pTournament->GetDisableTournamentMent();
		}
	}
	return szNone;     // ���� ��ȸ ����.
}

TournamentNode *ioTournamentManager::GetTournament( DWORD dwIndex, bool bDataSyncCheck )
{
	if( dwIndex == 0 ) return NULL;

	TournamentVec::iterator iter = m_TournamentList.begin();
	for(;iter != m_TournamentList.end();++iter)
	{
		TournamentNode *pTournament = *iter;
		if( pTournament )
		{
			if( pTournament->GetIndex() == dwIndex )
			{
				if( bDataSyncCheck )
				{
					if( !pTournament->IsDataSync() )
						pTournament->SendDataSync();
					if( !pTournament->IsCustomRewardDataSync() )
						pTournament->SendCustomRewardDataSync();
				}
				return pTournament;
			}
		}
	}
	return NULL;
}

bool ioTournamentManager::IsRegularTeamCreateUIShow()
{
	if( m_dwRegularTeamCreateUITimer == 0 )
		return false;
	if( FRAMEGETTIME() - m_dwRegularTeamCreateUITimer > REGULAR_TEAM_CREATE_TIMER )
		return false;
	return true;
}

void ioTournamentManager::CheckRegularTournaemntRequest()
{
	//���� ��ȸ��� ������ ���� �������� �ٽÿ�û
	if( m_dwRegularTourRequestTime == 0 )
	{
		m_dwRegularTourRequestTime = FRAMEGETTIME();
		if( g_TournamentMgr.GetRegularIndex() == 0 )
		{
			SP2Packet kPacket( CTPK_TOURNAMENT_REQUEST );
			TCPNetwork::SendToServer( kPacket );
		}
	}
	else if( m_dwRegularTourRequestTime + 60000 < FRAMEGETTIME() )
	{
		m_dwRegularTourRequestTime = 0;
	}
}