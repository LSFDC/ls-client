
#include "stdafx.h"

#include "ioModeUIController.h"

#include "GUI/CenterInfoWnd.h"
#include "GUI/GameChatInput.h"
#include "GUI/ChatMsgWnd.h"
#include "GUI/CenterKillInfoWnd.h"

#include "ioBaseChar.h"

#include "ioRsoldierInfoManager.h"

template<> ioModeUIController* Singleton< ioModeUIController >::ms_Singleton = 0;

ioModeUIController::ioModeUIController()
{
	m_pCenterInfoWnd = NULL;
	m_pKOMsgWnd = NULL;
	m_pCenterKillInfoWnd = NULL;
}

ioModeUIController::~ioModeUIController()
{
}

ioModeUIController& ioModeUIController::GetSingleton()
{
	return Singleton< ioModeUIController >::GetSingleton();
}

void ioModeUIController::LoadINI()
{
	
}

void ioModeUIController::ProcessController()
{

}

void ioModeUIController::SetCenterInfoWnd( ioWnd *pCenterInfoWnd )
{
	m_pCenterInfoWnd = pCenterInfoWnd;
}

void ioModeUIController::SetKOMsgWnd( ioWnd *pKOMsgWnd )
{
	m_pKOMsgWnd = pKOMsgWnd;
}

void ioModeUIController::SetDeathTime()
{
	if( !m_pCenterInfoWnd ) return;

	m_pCenterInfoWnd->HideWnd();

	CenterInfoWnd *pCenterInfo = dynamic_cast<CenterInfoWnd*>(m_pCenterInfoWnd);
	pCenterInfo->UpdateDeathTime();
}

void ioModeUIController::SetTeamDeathKO( TeamType eKill, const ioHashString &szKiller, TeamType eDie, const ioHashString &szDier, float fBlueRate, float fRedRate )
{
	if( !m_pCenterInfoWnd ) return;

	CenterInfoWnd *pCenterInfo = dynamic_cast<CenterInfoWnd*>(m_pCenterInfoWnd);
	pCenterInfo->UpdateTeamDeathMode( eKill, szKiller, eDie, szDier, fBlueRate, fRedRate );
}

void ioModeUIController::SetTeamDeathWarning( float fBlueRate, float fRedRate )
{
	if( !m_pCenterInfoWnd ) return;

	CenterInfoWnd *pCenterInfo = dynamic_cast<CenterInfoWnd*>(m_pCenterInfoWnd);
	pCenterInfo->UpdateTeamDeathWarning( fBlueRate, fRedRate );
}

void ioModeUIController::SetDeathMatch( int iCurrentRank )
{
	if( !m_pCenterInfoWnd ) return;
	//��� ȣ�� �� ���� ����� show.
	CenterInfoWnd *pCenterInfo = dynamic_cast<CenterInfoWnd*>(m_pCenterInfoWnd);
	pCenterInfo->UpdateDeathMatchMode( iCurrentRank );
}

void ioModeUIController::SetDeathMatchWarning( int iCurrentRank )
{
	if( !m_pCenterInfoWnd ) return;

	CenterInfoWnd *pCenterInfo = dynamic_cast<CenterInfoWnd*>(m_pCenterInfoWnd);
	pCenterInfo->UpdateDeathMatchWarning( iCurrentRank );
}

void ioModeUIController::SetGangsiLiveUser( int iLiveUser, const ioHashString &szGangsi, const ioHashString &szSurvivor )
{
	if( !m_pCenterInfoWnd ) return;

	CenterInfoWnd *pCenterInfo = dynamic_cast<CenterInfoWnd*>(m_pCenterInfoWnd);
	pCenterInfo->UpdateGangsiLiveUser( iLiveUser, szGangsi, szSurvivor );
}

void ioModeUIController::SetGangsiAlarm( bool bGangsiAlarmText, DWORD dwCurrentTime, DWORD dwWarningTime )
{
	if( !m_pCenterInfoWnd ) return;

	CenterInfoWnd *pCenterInfo = dynamic_cast<CenterInfoWnd*>(m_pCenterInfoWnd);
	pCenterInfo->UpdateGangsiAlarm( bGangsiAlarmText, dwCurrentTime, dwWarningTime );
}

void ioModeUIController::SetKingGauge( bool bKing, TeamType eKingTeam, TeamType eOwnerTeam, DWORD dwGivePointTime, DWORD dwCrownHoldTime, float fScoreWarningRate )
{
	if( !m_pCenterInfoWnd ) return;

	CenterInfoWnd *pCenterInfo = dynamic_cast<CenterInfoWnd*>(m_pCenterInfoWnd);
	pCenterInfo->UpdateKingMode( bKing, eKingTeam, eOwnerTeam, dwGivePointTime, dwCrownHoldTime, fScoreWarningRate );
}

void ioModeUIController::SetBossRaidMode( bool bShow, int iBossRaidOrder, int iBossRaidMaxOrder, int iMyBossLevel )
{
	if( !m_pCenterInfoWnd ) return;

	if( bShow )
	{
		CenterInfoWnd *pCenterInfo = dynamic_cast<CenterInfoWnd*>(m_pCenterInfoWnd);	
		pCenterInfo->UpdateBossMode( iBossRaidOrder, iBossRaidMaxOrder, iMyBossLevel );
	}
	else if( m_pCenterInfoWnd->IsShow() )
	{
		m_pCenterInfoWnd->HideWnd();
	}
}

void ioModeUIController::SetSymbolGaugeRate( bool bShow, float fBlueRate, float fRedRate )
{
	if( !m_pCenterInfoWnd ) return;

	if( bShow )
	{
		CenterInfoWnd *pCenterInfo = dynamic_cast<CenterInfoWnd*>(m_pCenterInfoWnd);	
		pCenterInfo->UpdateSymbolGauge( fBlueRate, fRedRate );
	}
	else if( m_pCenterInfoWnd->IsShow() )
	{
		m_pCenterInfoWnd->HideWnd();
	}
}

void ioModeUIController::SetSymbolAttack( TeamType eTeam )
{
	if( !m_pCenterInfoWnd ) return;

	CenterInfoWnd *pCenterInfo = dynamic_cast<CenterInfoWnd*>(m_pCenterInfoWnd);	
	pCenterInfo->UpdateSymbolAttack( eTeam );
}

void ioModeUIController::SetMonsterDiceResult( const ioHashString &kPrizeWinner, int iPresentType, int iPresentValue1, int iPresentValue2 )
{
	if( !m_pCenterInfoWnd ) return;

	CenterInfoWnd *pCenterInfo = dynamic_cast<CenterInfoWnd*>(m_pCenterInfoWnd);	
	pCenterInfo->UpdateMonsterDiceResult( kPrizeWinner, iPresentType, iPresentValue1, iPresentValue2 );
}

void ioModeUIController::SetKOMsg( TeamType eKillerTeam, const ioHashString &szKillerName, const ioHashString &szKillerSkillName, DWORD dwKillerItem, TeamType eDierTeam, const ioHashString &szDierName )
{
	if( !m_pKOMsgWnd ) return;

	KOChatMsgWnd *pKOMsgWnd = dynamic_cast<KOChatMsgWnd*>(m_pKOMsgWnd);
	pKOMsgWnd->SetKOMsg( eKillerTeam, szKillerName, szKillerSkillName, dwKillerItem, eDierTeam, szDierName );
}

void ioModeUIController::SetPrisonerMsg( TeamType eKillerTeam, const ioHashString &szKillerName, const ioHashString &szKillerSkillName, DWORD dwKillerItem,	TeamType eDierTeam, const ioHashString &szDierName )
{
	if( !m_pKOMsgWnd ) return;

	KOChatMsgWnd *pKOMsgWnd = dynamic_cast<KOChatMsgWnd*>(m_pKOMsgWnd);
	pKOMsgWnd->SetPrisonerMsg( eKillerTeam, szKillerName, szKillerSkillName, dwKillerItem, eDierTeam, szDierName );
}

void ioModeUIController::SetEscapeMsg( TeamType eKillerTeam, const ioHashString &szKillerName, const ioHashString &szKillerSkillName, DWORD dwKillerItem, TeamType eDierTeam, const ioHashString &szDierName )
{
	if( !m_pKOMsgWnd ) return;

	KOChatMsgWnd *pKOMsgWnd = dynamic_cast<KOChatMsgWnd*>(m_pKOMsgWnd);
	pKOMsgWnd->SetEscapeMsg( eKillerTeam, szKillerName, szKillerSkillName, dwKillerItem, eDierTeam, szDierName );
}

void ioModeUIController::SetInfectionMsg( TeamType eKillerTeam, const ioHashString &szKillerName, const ioHashString &szKillerSkillName, DWORD dwKillerItem, TeamType eDierTeam, const ioHashString &szDierName )
{
	if( !m_pKOMsgWnd ) return;

	KOChatMsgWnd *pKOMsgWnd = dynamic_cast<KOChatMsgWnd*>(m_pKOMsgWnd);
	pKOMsgWnd->SetInfectionMsg( eKillerTeam, szKillerName, szKillerSkillName, dwKillerItem, eDierTeam, szDierName );
}

bool ioModeUIController::IsHiddenKingWarning()
{
	if( !m_pCenterInfoWnd ) return false;

	CenterInfoWnd *pCenterInfo = dynamic_cast<CenterInfoWnd*>(m_pCenterInfoWnd);	

	return pCenterInfo->IsHiddenKingWarning();
}

void ioModeUIController::SetDoubleCrownGaugeRate( bool bShow, float fBlueRate, float fRedRate, bool bBlueKing, bool bRedKIng )
{
	if( !m_pCenterInfoWnd ) return;

	if( bShow )
	{
		CenterInfoWnd *pCenterInfo = dynamic_cast<CenterInfoWnd*>(m_pCenterInfoWnd);	
		pCenterInfo->UpdateDoubleCrownGauge( fBlueRate, fRedRate, bBlueKing, bRedKIng );
	}
	else if( m_pCenterInfoWnd->IsShow() )
	{
		m_pCenterInfoWnd->HideWnd();
	}
}

void ioModeUIController::SetDoubleCrownAttack( TeamType eTeam )
{
	if( !m_pCenterInfoWnd ) return;

	CenterInfoWnd *pCenterInfo = dynamic_cast<CenterInfoWnd*>(m_pCenterInfoWnd);	
	pCenterInfo->UpdateDoubleCrownAttack( eTeam );
}

void ioModeUIController::SetDoubleCrownWinningTeam( TeamType eTeam )
{
	if( !m_pCenterInfoWnd ) return;

	CenterInfoWnd *pCenterInfo = dynamic_cast<CenterInfoWnd*>(m_pCenterInfoWnd);	
	pCenterInfo->UpdateDoubleCrownWinningTeam( eTeam );
}


////////////////////////////////////////////////////////////////////////////////////////
// Center Kill Info
void ioModeUIController::SetCenterKillInfoWnd( ioWnd *pCenterKillInfoWnd )
{
	m_pCenterKillInfoWnd = pCenterKillInfoWnd;
}

void ioModeUIController::SetCenterKillUI( const ModeType& CurModeType, const int& iKillCount, const ioBaseChar* const pOwner, TeamType eKillerTeam, const ioHashString &szKillerName, 
											DWORD dwKillerItem, TeamType eDierTeam, const ioHashString &szDierName, bool bKillerMale )
{
	if ( !m_pCenterKillInfoWnd )
		return;

	CenterKillInfoWnd *pCenterKillInfo = dynamic_cast<CenterKillInfoWnd*>(m_pCenterKillInfoWnd);
	if ( !pCenterKillInfo )
		return;

	//��ų üũ
	if ( eKillerTeam == eDierTeam )
		return;

	const CHARACTER& rkChar = pOwner->GetCharacterInfo();
	bool IsRSet = g_RSoldierInfoManager.IsSoldierSet( rkChar );
	//����ų �̻��̶��
	if ( iKillCount > 1 )
	{
		pCenterKillInfo->SetOtherKillCenterUI( iKillCount, eKillerTeam, szKillerName, eDierTeam, szDierName, dwKillerItem, bKillerMale );
		if ( ( pOwner->GetCharName() == szKillerName || pOwner->GetCharName() == szDierName ) && IsRSet )
			pCenterKillInfo->ClearCurInfoSound();
	}
	else
	{
		//���� killer�� �ڽ��̶��
		if ( pOwner->GetCharName() == szKillerName )
		{
			pCenterKillInfo->SetOneKillCenterUI( CKT_OWNER_KILL, iKillCount, eKillerTeam, szKillerName, eDierTeam, szDierName, dwKillerItem, bKillerMale );
			if ( IsRSet )
				pCenterKillInfo->ClearCurInfoSound();
		}
		//killer�� �ڽ��� ���̶��
		else if ( pOwner->GetTeam() == eKillerTeam )
		{
			pCenterKillInfo->SetOneKillCenterUI( CKT_TEAM_CHAR_KILL, iKillCount, eKillerTeam, szKillerName, eDierTeam, szDierName, dwKillerItem, bKillerMale );
		}
		//���� ����� �����
		else if( pOwner->GetCharName() == szDierName )
		{
			pCenterKillInfo->SetOneKillCenterUI( CKT_OWNER_DIE, iKillCount, eKillerTeam, szKillerName, eDierTeam, szDierName, dwKillerItem, bKillerMale );
			if ( IsRSet )
				pCenterKillInfo->ClearCurInfoSound();
		}
		//���� ĳ���Ͱ� ���� ���̶��
		else if ( pOwner->GetTeam() == eDierTeam )
		{
			pCenterKillInfo->SetOneKillCenterUI( CKT_TEAM_CHAR_DIE, iKillCount, eKillerTeam, szKillerName, eDierTeam, szDierName, dwKillerItem, bKillerMale );
		}
	}
}

bool ioModeUIController::IsCenterKillInfoWnd() const
{
	if ( m_pCenterKillInfoWnd )
		return true;

	return false;
}
