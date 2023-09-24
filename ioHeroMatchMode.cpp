

#include "stdafx.h"

#include "ioHeroMatchMode.h"

#include "ioPlayMode.h"
#include "ioPlayStage.h"
#include "ioBaseChar.h"

#include "ioComplexStringPrinter.h"
#include "ioLadderTeamMgr.h"
#include "ioShuffleRoomMgr.h"

#include "GUI/MiniMap.h"
#include "GUI/CenterInfoWnd.h"
#include "GUI/SoldierSelectWnd.h"
#include "GUI/MiniSoldierSelectWnd.h"

#include "GUI/TimeScoreWnd.h"
#include "GUI/ioObserverWndManager.h"
#include "GUI/ioObserverWnd.h"
#include "GUI/ModeRankWnd.h"
#include "GUI/BriefingWnd.h"
#include "GUI/GlobalWnd.h"
#include "GUI/TotalResultMainWnd.h"
#include "GUI/RoundStateMainWnd.h"

ioHeroMatchMode::ioHeroMatchMode( ioPlayStage *pCreator ) : ioPlayMode( pCreator )
{
	m_iBlueTeamCnt = 0;
	m_iRedTeamCnt = 0;

	m_ModeType = MT_HEROMATCH;

	m_vCharRecordList.reserve( 100 );
}

ioHeroMatchMode::~ioHeroMatchMode()
{
	m_bZeroHP = false;
	ClearMode();
}

void ioHeroMatchMode::InitMode( int iMapIndex )
{
	m_iModeMapIndex = iMapIndex;

	LoadMap();
	LoadPushStructs();
	LoadMachineStruct();
	LoadFuntionalObjects();
	LoadCharacters();
	LoadDisableSkillList();
	LoadStartBuffList();

	ioINILoader kLoader( GetModeINIFileName( m_ModeType ) );

	kLoader.SetTitle_e( "common_info" );
	m_dwRoundDuration = kLoader.LoadInt_e( "round_time", 300000 );
	m_dwCurRoundDuration = m_dwRoundDuration;

	m_fScoreWarningRate = kLoader.LoadFloat_e( "score_warning_rate", FLOAT05 );

	LoadRevivalTime( kLoader );
	LoadModeResource( kLoader );
	LoadModeCamera( kLoader );

	char szBuf[MAX_PATH];
	kLoader.SetTitle_e( "ko_recover" );
	m_fKORecoverRate = kLoader.LoadFloat_e( "ko_recover_rate", 0.0f );
	kLoader.LoadString_e( "ko_recover_effect", "", szBuf, MAX_PATH );
	m_KORecoverEffect = szBuf;
	kLoader.LoadString_e( "ko_perfect_effect", "", szBuf, MAX_PATH );
	m_PerfectRecoverEffect = szBuf;
	kLoader.LoadString_e( "ko_buff", "", szBuf, MAX_PATH );
	m_KOBuff = szBuf;

	kLoader.SetTitle_e( "end_scene_camera" );
	m_fEndHeight = kLoader.LoadFloat_e( "end_camera_height", 0.0f );
	m_fEndDistance = kLoader.LoadFloat_e( "end_camera_distance", 0.0f );
	m_fEndFov = kLoader.LoadFloat_e( "end_camera_fov", 0.0f );
	kLoader.LoadString_e( "round_end_focus_effect", "", szBuf, MAX_PATH );
	m_RoundEndEffect = szBuf;

	LoadGUIValues();
	LoadPushCursor();
	LoadContribute();

	kLoader.LoadString_e( "ui_info", "1point_up_emoticon", "", szBuf, MAX_PATH );
	m_PointUpEmoticon = szBuf;

	if( !P2PNetwork::IsNetworkPlaying() )
	{
		SetModeState( MS_READY );
		LoadObjectItems();
	}

	g_MyInfo.ClearCharDiedInfo();
	g_MyInfo.ClearCharDiedPenaltyInfo();
	g_MyInfo.ClearCharJoinedInfo();

	g_GUIMgr.CheckShopSound( true );

	g_MyInfo.InitPraise();

	ClearGrowthLevel();

	// LoadMap() ���� ����ؾ���
	SetMapAreaWeapon();

	SetMapDummy();

	if( g_BattleRoomMgr.IsBattleRoom() )
		g_BattleRoomMgr.ModeStartPlayerCount();

	if( g_LadderTeamMgr.IsLadderTeam() )
	{
		if( g_LadderTeamMgr.IsHeroMatchMode() )
		{
			g_GUIMgr.ShowWnd( HERO_MATCH_VERSUS_SCENE_WND );
		}
	}
}

void ioHeroMatchMode::LoadModeResource( ioINILoader &rkLoader )
{	
	TimeScoreWnd *pTimeScoreWnd = dynamic_cast<TimeScoreWnd *>( g_GUIMgr.AddWnd( "XML/TimeScoreWnd.xml") );
	if(pTimeScoreWnd)
		pTimeScoreWnd->SetModeType( GetModeType() );

	g_ModeUIController.SetKOMsgWnd( g_GUIMgr.AddWnd( "XML/kochatmsgwnd.xml") );

	g_ModeUIController.SetCenterKillInfoWnd( NULL ); 

	CenterInfoWnd *pCenterInfoWnd = dynamic_cast<CenterInfoWnd *>( g_GUIMgr.AddWnd("XML/CenterInfoWnd.xml") );
	if(pCenterInfoWnd)
	{
		g_ModeUIController.SetCenterInfoWnd( pCenterInfoWnd );
	}

	MiniSoldierSelectWnd *pMiniSoldierWnd = dynamic_cast<MiniSoldierSelectWnd*>(g_GUIMgr.AddWnd( "XML/MiniSoldierSelectWnd.xml" ));
	if(pMiniSoldierWnd)
		pMiniSoldierWnd->SetPlayStage( m_pCreator );

	g_GUIMgr.AddWnd( "XML/SkillListWnd.xml" );
	g_GUIMgr.AddWnd( "XML/AccessorySkillListWnd.xml" );
	//g_GUIMgr.AddWnd( "XML/gamequestalarmwnd.xml" ); //�̼��� �߰��ϸ� �ش� UI�� ������� �ʽ��ϴ�.

	// TimeScoreWnd ���Ŀ� �߰��ؾ��Ѵ�
	ioObserverBlueWnd *pBlueWnd = dynamic_cast<ioObserverBlueWnd *>( g_GUIMgr.AddWnd("XML/observerblueinfownd.xml") );
	if( pBlueWnd )
		pBlueWnd->InitObserverWnd( m_pCreator );

	ioObserverRedWnd *pRedWnd = dynamic_cast<ioObserverRedWnd *>( g_GUIMgr.AddWnd("XML/observerredinfownd.xml") );
	if( pRedWnd )
		pRedWnd->InitObserverWnd( m_pCreator );

	ioPlayMode::LoadModeResource( rkLoader );
	m_vRoundHistory.clear();
}

void ioHeroMatchMode::ProcessMode()
{
	ErrorReport::SetPosition( 1030, 0 );
	ProcessSelectCharacter();
	ErrorReport::SetPosition( 1030, 1 );
	ProcessGUI();
	ErrorReport::SetPosition( 1030, 2 );

	switch( m_ModeState )
	{
	case MS_READY:
		ErrorReport::SetPosition( 1030, 3 );
		ProcessReadyState();
		ErrorReport::SetPosition( 1030, 4 );
		CheckModeMapSound();
		break;
	case MS_PLAY:
		ProcessMapAreaWeapon();
		ErrorReport::SetPosition( 1030, 5 );
		ProcessPlayState();
		ErrorReport::SetPosition( 1030, 6 );
		CheckModeMapSound();
		ErrorReport::SetPosition( 1030, 7 );
		g_EventMgr.Process();
		break;
	case MS_RESULT:
		ErrorReport::SetPosition( 1030, 8 );
		ProcessResultState();
		ErrorReport::SetPosition( 1030, 9 );
		break;
	}
}

void ioHeroMatchMode::ProcessGUI()
{
	UpdateMiniMapSpotPoint();
	UpdateTimeScoreWnd();
	UpdateObserverWnd();
	UpdateRoundStateWnd();
	UpdateEndScoreWndTime();
	UpdateSoldierWndTime();

	ProcessModeGuide();
	ProcessPushEnableCursor();
	ProcessBlindMode();
	ProcessDeathTimeHelp();
	ProcessAutoStartAlarm();
}

void ioHeroMatchMode::UpdateMiniMapSpotPoint()
{
	MiniMap *pMiniMap = dynamic_cast<MiniMap*>(g_GUIMgr.FindWnd( MINIMAP_WND ));
	if( !pMiniMap )	return;

	ioPlayMode::UpdateMiniMapSpotPoint();

	pMiniMap->ClearPointList();

	D3DXVECTOR3 vPos;
	DWORD dwGapTime = 0;

	// Other Char...
	RecordList::iterator iter;
	for( iter=m_vCharRecordList.begin() ; iter!=m_vCharRecordList.end() ; ++iter )
	{
		ioBaseChar *pChar = iter->pChar;
		if( pChar->IsOwnerChar() || !pChar->IsCanMiniMapVisible() )
			continue;

		dwGapTime = pChar->GetEventGapTime();
		vPos = pChar->GetWorldPosition();
		if( pChar->IsDropZoneDownState() )
			pMiniMap->AddPoint( vPos.x, vPos.z, MiniMap::SPOT_DROP_USER, pChar->GetTeam(), 0 );
		else
			pMiniMap->AddPoint( vPos.x, vPos.z, MiniMap::SPOT_USER, pChar->GetTeam(), dwGapTime );		
	}

	//MyChar..
	ioBaseChar *pOwner = GetOwnerChar();
	if( pOwner && pOwner->IsCanMiniMapVisible() )
	{
		dwGapTime = pOwner->GetEventGapTime();
		vPos = pOwner->GetWorldPosition();
	
		if( pOwner->IsDropZoneDownState() )
			pMiniMap->AddPoint( vPos.x, vPos.z, MiniMap::SPOT_DROP_ME, pOwner->GetTeam(), 0 );
		else
			pMiniMap->AddPoint( vPos.x, vPos.z, MiniMap::SPOT_ME, pOwner->GetTeam(), dwGapTime );
	}

	// pDummy
	const GameEntityList &rkObjectList = m_pCreator->GetGameEntityList();
	GameEntityList::const_iterator iter_b;
	for( iter_b = rkObjectList.begin() ; iter_b!=rkObjectList.end() ; ++iter_b )
	{
		ioDummyChar *pDummy = ToDummyChar( *iter_b );
		if( pDummy && pDummy->IsVisibleMiniMap() )
		{
			vPos = pDummy->GetWorldPosition();
			pMiniMap->AddPoint( vPos.x, vPos.z, MiniMap::SPOT_USER, pDummy->GetTeam(), 0 );
		}
	}
}

void ioHeroMatchMode::UpdateTimeScoreWnd()
{
	TimeScoreWnd *pTimeScoreWnd = dynamic_cast<TimeScoreWnd *>( g_GUIMgr.FindWnd( TIME_SCORE_WND ) );
	if(!pTimeScoreWnd) return;

	bool bRedWarning = false;
	bool bBlueWarning = false;

	pTimeScoreWnd->UpdateScore( m_iBlueRoundWinCnt, m_iRedRoundWinCnt );

	DWORD dwTimeGap = 0;
	switch( m_ModeState )
	{
	case MS_READY:
		pTimeScoreWnd->UpdateTime( m_dwCurRoundDuration, m_dwRoundDuration );
		break;
	case MS_PLAY:
		dwTimeGap = FRAMEGETTIME() - m_dwPlayStartTime;
		if( m_dwCurRoundDuration > dwTimeGap )
		{
			pTimeScoreWnd->UpdateTime( m_dwCurRoundDuration - dwTimeGap, m_dwRoundDuration );

			ProcessWarningSound( m_dwCurRoundDuration );

			if( bRedWarning || bBlueWarning )
				ProcessScoreWarningSound( true );
			else
				ProcessScoreWarningSound( false );
		}
		else
		{
			pTimeScoreWnd->UpdateTime( 0, m_dwRoundDuration );
		}
		break;
	case MS_RESULT:
		pTimeScoreWnd->UpdateTime( 0, m_dwRoundDuration );
		break;
	}

	if( m_ModeState != MS_PLAY )
	{
		ProcessScoreWarningSound( false );
	}
}

void ioHeroMatchMode::UpdateRoundStateWnd()
{
	if( g_MyInfo.IsSortContribute() )
	{
		DWORD dwTimeGap = FRAMEGETTIME() - m_dwPlayStartTime;
		if( m_dwCurRoundDuration > dwTimeGap )
			ContributeRoundAllPercent( m_dwRoundDuration - (m_dwCurRoundDuration - dwTimeGap), m_dwRoundDuration );
		else
			ContributeRoundAllPercent( m_dwRoundDuration, m_dwRoundDuration );

		HeroMatchRecord *pRecord = FindRecordCharName( g_MyInfo.GetPublicID() );
		if( pRecord )
			g_MyInfo.SetMyContribute( pRecord->Contribute.m_iPrevContributePer );
	}

	if( m_ModeState != MS_PLAY )
	{
		if( g_GUIMgr.IsShow( ROUND_STATE_MAIN_WND ) )
		{
			g_GUIMgr.HideWnd( ROUND_STATE_MAIN_WND );
		}
		return;
	}

	if( g_Input.IsKeyDown( KEY_TAB ) )
	{
		RoundStateMainWnd *pRoundStateWnd = dynamic_cast<RoundStateMainWnd*>(g_GUIMgr.FindWnd( ROUND_STATE_MAIN_WND ));
		if( pRoundStateWnd && !pRoundStateWnd->IsShow() )
		{	
			DWORD dwTimeGap = FRAMEGETTIME() - m_dwPlayStartTime;
			if( m_dwCurRoundDuration > dwTimeGap )
				ContributeRoundAllPercent( m_dwRoundDuration - (m_dwCurRoundDuration - dwTimeGap), m_dwRoundDuration );
			else
				ContributeRoundAllPercent( m_dwRoundDuration, m_dwRoundDuration );

			pRoundStateWnd->ClearRoundInfo();
			for( int i = 0 ; i < GetRecordCharCnt(); i++ )
			{
				RoundInfo kInfo;
				if( GetRoundInfo( i, kInfo ) )
					pRoundStateWnd->AddRoundInfo( kInfo );
			}

			pRoundStateWnd->SetTeamScore( m_iBlueRoundWinCnt, m_iRedRoundWinCnt );
			pRoundStateWnd->ShowWnd();
		}
	}
	else
	{
		if( g_GUIMgr.IsShow( ROUND_STATE_MAIN_WND ) )
		{
			g_GUIMgr.HideWnd( ROUND_STATE_MAIN_WND );			
		}
	}
}

void ioHeroMatchMode::ProcessReadyState()
{
	if( P2PNetwork::IsNetworkPlaying() )	return;
	if( m_dwReadyStartTime + 4000 > FRAMEGETTIME() )
		return;

	SetModeState( MS_PLAY );
}

void ioHeroMatchMode::ProcessPlayState()
{
	UpdateSoldierRevivalPenalty();

	m_pCreator->NotifySpringDampUseSet( true );

	if( !P2PNetwork::IsNetworkPlaying() )
	{
		CheckRoundEnd();
	}
}

void ioHeroMatchMode::CheckRoundEnd()
{
	WinTeamType eWinTeam = WTT_DRAW;

	int iRedTeamCnt  = GetTeamPlayerCount( TEAM_RED );
	int iBlueTeamCnt = GetTeamPlayerCount( TEAM_BLUE );

	DWORD dwGapTime = FRAMEGETTIME() - m_dwPlayStartTime;
	if( (m_dwCurRoundDuration - dwGapTime) <= 0 )
	{
		if( IsTeamPlayerDie( TEAM_BLUE ) )
			eWinTeam = WTT_RED_TEAM;
		else if( IsTeamPlayerDie( TEAM_RED ) )
			eWinTeam = WTT_BLUE_TEAM;

		SetRoundEndInfo( eWinTeam, true );
	}
	else
	{
		if( IsTeamPlayerDie( TEAM_BLUE ) )
			eWinTeam = WTT_RED_TEAM;
		else if( IsTeamPlayerDie( TEAM_RED ) )
			eWinTeam = WTT_BLUE_TEAM;

		if( eWinTeam != WTT_DRAW )
		{
			m_dwCurRoundDuration -= dwGapTime;
			SetRoundEndInfo( eWinTeam, false );
		}
	}
}

int ioHeroMatchMode::GetTeamPlayerCount( TeamType eTeam )
{
	int iUserCnt = 0;

	int iSize = GetRecordCharCnt();
	for(int i = 0;i < iSize;i++)
	{
		ioBaseChar *pChar = GetRecordChar( i );
		if( !pChar ) continue;
		if( pChar->GetState() == CS_VIEW ) continue;
		if( pChar->GetState() == CS_LOADING ) continue;
		if( pChar->GetState() == CS_OBSERVER ) continue;
		
		if( pChar->GetTeam() == eTeam )
			iUserCnt++;
	}

	return iUserCnt;
}

int ioHeroMatchMode::GetTeamPurePlayerCount( TeamType eTeam )
{
	int iUserCnt = 0;

	int iSize = GetRecordCharCnt();
	for(int i = 0;i < iSize;i++)
	{
		ModeRecord *pRecord = FindModeRecord( i );
		if( pRecord == NULL ) continue;

		ioBaseChar *pChar = pRecord->pChar;
		if( !pChar )	continue;
		if( pChar->GetTeam() != eTeam )		continue;

		switch( pChar->GetState() )
		{
		case CS_DIE:
		case CS_VIEW:
		case CS_READY:
		case CS_LOADING:
		case CS_OBSERVER:
			break;
		default:
			iUserCnt++;
			break;
		}
	}

	return iUserCnt;
}

bool ioHeroMatchMode::IsTeamPlayerDie( TeamType eTeam )
{
	int iSize = GetRecordCharCnt();
	for(int i = 0;i < iSize;i++)
	{
		ModeRecord *pRecord = FindModeRecord( i );
		if( pRecord == NULL ) continue;

		ioBaseChar *pChar = pRecord->pChar;
		if( !pChar )	continue;
		if( pChar->GetTeam() != eTeam )		continue;
		
		if( pChar->GetState() == CS_DIE )
			return true;
	}
	return false;
}

void ioHeroMatchMode::ProcessResultState()
{
	ShowEndScoreWnd( true );

	if( !P2PNetwork::IsNetworkPlaying() )
	{
		DWORD dwTimeGap = FRAMEGETTIME() - m_dwResultStartTime;
		if( dwTimeGap > m_dwCurResultShowDuration )
		{
			ShowEndScoreWnd( false );

			ClearModeData();
			UpdateRoundCount();

			LoadPushStructs();
			LoadMachineStruct();

			ReStartMode();

			if( m_iCurRoundCount == 1 )
			{
				UpdateUserRank();
			}
		}
	}
}

void ioHeroMatchMode::RenderPreGUI()
{
	if( m_ModeState == MS_PLAY )
	{
		RenderPushEnableCursor();
	}
}

void ioHeroMatchMode::RenderAfterGUI()
{
}

void ioHeroMatchMode::AddNewRecord( ioBaseChar *pChar )
{
	if( !pChar )
		return;

	HeroMatchRecord *pHeroMatchRecord = FindRecordCharName(pChar->GetCharName());
	if( !pHeroMatchRecord )
	{
		HeroMatchRecord kRecord;
		kRecord.pChar  = pChar;
		kRecord.szName = pChar->GetCharName();
		m_vCharRecordList.push_back( kRecord );		

		UpdateUserRank();

		if( pChar )
		{
			if( !g_BattleRoomMgr.IsObserverUser( pChar->GetCharName() ) )
				g_AwardMgr.AddNewAwardUser( pChar->GetCharName() );
			else if( g_ShuffleRoomMgr.IsShuffleRoom() )
				g_AwardMgr.AddNewAwardUser( pChar->GetCharName() );
		}
	}
	else
	{
		pHeroMatchRecord->pChar = pChar;
	}
}

void ioHeroMatchMode::RemoveRecord( const ioHashString &rkName )
{
	int iCharCnt = m_vCharRecordList.size();
	for( int i=0 ; i<iCharCnt ; i++ )
	{
		if( m_vCharRecordList[i].szName == rkName )
		{
			m_vCharRecordList.erase( m_vCharRecordList.begin() + i );
			break;
		}
	}

	UpdateUserRank();
}


void ioHeroMatchMode::RemoveRecord( int nIndex )
{
	int iCharCnt = m_vCharRecordList.size();
	if( iCharCnt > nIndex )
	{
		m_vCharRecordList.erase( m_vCharRecordList.begin() + nIndex );
		UpdateUserRank();
	}
}


ioBaseChar* ioHeroMatchMode::GetRecordChar( int i )
{
	if( COMPARE( i, 0, GetRecordCharCnt() ) )
		return m_vCharRecordList[i].pChar;

	return NULL;
}

ioBaseChar* ioHeroMatchMode::GetRecordChar( const ioHashString &rkName )
{
	HeroMatchRecord *pRecord = FindRecordCharName( rkName );
	if( pRecord )
		return pRecord->pChar;

	return NULL;
}

int ioHeroMatchMode::GetRecordCharCnt() const
{
	return m_vCharRecordList.size();
}

ModeRecord* ioHeroMatchMode::FindModeRecord( int iIdx )
{
	if( COMPARE( iIdx, 0, GetRecordCharCnt() ) )
		return &m_vCharRecordList[iIdx];

	return NULL;
}

ModeRecord* ioHeroMatchMode::FindModeRecordCharName( const ioHashString &rkName )
{
	return (ModeRecord*)FindRecordCharName( rkName );
}

HeroMatchRecord* ioHeroMatchMode::FindRecordCharName( const ioHashString &rkName )
{
	int iCharCnt = m_vCharRecordList.size();
	for( int i=0 ; i<iCharCnt ; i++ )
	{
		if( m_vCharRecordList[i].szName == rkName )
			return &m_vCharRecordList[i];
	}

	return NULL;
}

void ioHeroMatchMode::SetRoundEndInfo( WinTeamType eTeam, bool bRoundSetEnd )
{
	CheckLastKiller();
	ioPlayMode::SetRoundEndInfo( eTeam, bRoundSetEnd );

	if( bRoundSetEnd )
	{
		// �����丮 ���
		g_CampMgr.SetHeroMatchResult( eTeam, m_iBlueRoundWinCnt, m_iRedRoundWinCnt );		
	}
}

void ioHeroMatchMode::SetTotalResultBriefingWnd()
{
	// �� ��� ���� ��� �긮��
	ioBaseChar *pOwner = GetOwnerChar();
	if( pOwner )
	{
		BriefingWnd *pBriefingWnd = dynamic_cast<BriefingWnd*>(g_GUIMgr.FindWnd( BRIEFING_WND ));
		if( pBriefingWnd && !pBriefingWnd->IsShow() )
		{
			TeamType eWinTeam = TEAM_NONE;
			if( IsBlueWin( m_WinTeamType ) )
				eWinTeam = TEAM_BLUE;
			else if( IsRedWin( m_WinTeamType ) )
				eWinTeam = TEAM_RED;

			D3DXVECTOR3 vNewPos = ioMath::VEC3_ZERO;
			int iWinCount, iLoseCount;
			int iResultType = BriefingWnd::RESULT_DRAW;
			TeamType eOwenerTeam = pOwner->GetTeam();

			if( g_BattleRoomMgr.IsObserverUser( g_MyInfo.GetPublicID() ) || m_pCreator->IsStealthMode() )
			{
				ioBaseChar *pTargetChar = m_pCreator->GetBaseChar(m_szEndLookCharName);
				if( pTargetChar && (GetModeType() == MT_SURVIVAL || GetModeType() == MT_TEAM_SURVIVAL || GetModeType() == MT_DOUBLE_CROWN || GetModeType() == MT_FIGHT_CLUB || GetModeType() == MT_TEAM_SURVIVAL_AI) )
				{
					eOwenerTeam = pTargetChar->GetTeam();

					if( eOwenerTeam == TEAM_BLUE )
					{
						if( eWinTeam == TEAM_BLUE )
							iResultType = BriefingWnd::RESULT_WIN;
						else if( eWinTeam == TEAM_RED )
							iResultType = BriefingWnd::RESULT_LOSE;
					}
					else
					{
						if( eWinTeam == TEAM_RED )
							iResultType = BriefingWnd::RESULT_WIN;
						else if( eWinTeam == TEAM_BLUE )
							iResultType = BriefingWnd::RESULT_LOSE;
					}

					if( IsNeedUpdateMapEffect( pTargetChar ) )
						vNewPos = pTargetChar->GetWorldPosition();
				}
				else
				{
					eOwenerTeam = eWinTeam;
					iResultType = BriefingWnd::RESULT_WIN;
				}
			}
			else
			{
				if( pOwner->GetTeam() == TEAM_BLUE )
				{
					iWinCount = m_iBlueRoundWinCnt;
					iLoseCount= m_iRedRoundWinCnt;

					if( eWinTeam == TEAM_BLUE )
						iResultType = BriefingWnd::RESULT_WIN;
					else if( eWinTeam == TEAM_RED )
						iResultType = BriefingWnd::RESULT_LOSE;
				}
				else
				{
					iWinCount = m_iRedRoundWinCnt;
					iLoseCount= m_iBlueRoundWinCnt;

					if( eWinTeam == TEAM_RED )
						iResultType = BriefingWnd::RESULT_WIN;
					else if( eWinTeam == TEAM_BLUE )
						iResultType = BriefingWnd::RESULT_LOSE;
				}

				if( IsNeedUpdateMapEffect( pOwner ) )
					vNewPos = pOwner->GetWorldPosition();
			}

			pBriefingWnd->SetResultInfo( m_pCreator, BriefingWnd::BRIEFING_FINAL_RESULT, iResultType, eOwenerTeam, m_iCurRoundCount );

			ioHashString szMapEffect = pBriefingWnd->GetFinalResultMapEffect( eWinTeam );
			// �� ����Ʈ �۷�!
			if( !szMapEffect.IsEmpty() )
			{
				if( vNewPos == ioMath::VEC3_ZERO )
					CreateMapEffect( szMapEffect, GetEndSceneLookAt(), ioMath::UNIT_ALL );
				else
					CreateMapEffect( szMapEffect, vNewPos, ioMath::UNIT_ALL );
			}
		}				
	}
	SetBlindMode( true, false );
}

void ioHeroMatchMode::SetRoundEndMotion()
{
	WinTeamType eWinTeamType = m_WinTeamType;
	RoundEndMotion eEndMotion;	
	int iCharCnt = GetRecordCharCnt();	
	for( int i=0; i<iCharCnt; i++ )
	{
		ioBaseChar *pChar = GetRecordChar( i );
		if( !pChar ) continue;
		if( pChar->GetState() == CS_OBSERVER ) continue;
		if( g_BattleRoomMgr.IsObserverUser(pChar->GetCharName()) ) continue;

		if( eWinTeamType == WTT_DRAW )
		{
			eEndMotion = RM_DRAW;
		}
		else
		{
			if( IsWinTeam( eWinTeamType, pChar->GetTeam() ) )
				eEndMotion = RM_WIN;
			else
				eEndMotion = RM_LOSE;
		}

		pChar->SetRoundEndMotion( eEndMotion );
		pChar->StartNoInputDelayTime( 10000 );
	}	
}

bool ioHeroMatchMode::GetRoundInfo( int iIndex, RoundInfo &rkInfo )
{
	if( ioPlayMode::GetRoundInfo( iIndex, rkInfo ) )
	{
		HeroMatchRecord *pRecord = (HeroMatchRecord*)FindModeRecord( iIndex );
		if( !pRecord )	return false;
		if( !pRecord->pChar ) return false;

		rkInfo.m_iContribute	= pRecord->Contribute.m_iContribute;
		rkInfo.m_iContributePer = pRecord->Contribute.m_iPrevContributePer;		
		return true;
	}
	return false;
}

bool ioHeroMatchMode::GetResultInfo( int iIndex, ResultInfo &rkInfo )
{
	if( ioPlayMode::GetResultInfo( iIndex, rkInfo ) )
	{
		HeroMatchRecord *pRecord = (HeroMatchRecord*)FindModeRecord( iIndex );
		if( !pRecord )	return false;
		if( !pRecord->pChar ) return false;

		rkInfo.m_iContribute = pRecord->Contribute.m_iContribute;
		rkInfo.m_iContributePer = pRecord->Contribute.m_iPrevContributePer;
		return true;
	}
	return false;
}

bool ioHeroMatchMode::GetFinalInfo( int iIndex, FinalInfo &rkInfo )
{
	if( ioPlayMode::GetFinalInfo( iIndex, rkInfo ) )
	{
		HeroMatchRecord *pRecord = (HeroMatchRecord*)FindModeRecord( iIndex );
		if( !pRecord )	return false;
		if( !pRecord->pChar ) return false;

		rkInfo.m_iContribute = pRecord->Contribute.m_iContribute;
		rkInfo.m_iContributePer = pRecord->Contribute.m_iPrevContributePer;
		return true;
	}
	return false;
}

void ioHeroMatchMode::ApplyModeInfo( SP2Packet &rkPacket )
{
	rkPacket >> m_iCurRoundCount;
	rkPacket >> m_iMaxRoundCount;

	rkPacket >> m_dwRoundDuration;

	rkPacket >> m_iBlueRoundWinCnt;
	rkPacket >> m_iRedRoundWinCnt;

	rkPacket >> m_iBluePosArray;
	rkPacket >> m_iRedPosArray;

	m_vRoundHistory.erase( m_vRoundHistory.begin(), m_vRoundHistory.end() );
	m_vRoundHistory.reserve( m_iCurRoundCount );

	for( int i=0; i < m_iCurRoundCount; i++ )
	{
		RoundHistory rh;
		rkPacket >> rh.iBluePoint;
		rkPacket >> rh.iRedPoint;
		m_vRoundHistory.push_back( rh );
	}
}

void ioHeroMatchMode::ApplyCharModeInfo( const ioHashString &rkName, SP2Packet &rkPacket, bool bDieCheck )
{
	bool bRecord;
	rkPacket >> bRecord;

	if( !bRecord ) return;

	HeroMatchRecord *pRecord = FindRecordCharName( rkName );
	if( pRecord )
	{
		// HARDCODE: �Ʒ��ʿ� ������ ���� ��쵵 �ݵ�� �����ϰ� ���ٰ�
		int iKillCharCnt, iDeathCharCnt;
		pRecord->iKillInfoMap.clear();
		pRecord->iDeathInfoMap.clear();

		rkPacket >> iKillCharCnt;
		for( int i=0; i < iKillCharCnt; ++i )
		{
			int iClassType, iKillCnt;
			rkPacket >> iClassType >> iKillCnt;

			pRecord->iKillInfoMap.insert( KillDeathInfoMap::value_type(iClassType, iKillCnt) );
		}

		rkPacket >> iDeathCharCnt;
		for( int j=0; j < iDeathCharCnt; ++j )
		{
			int iClassType, iDeathCnt;
			rkPacket >> iClassType >> iDeathCnt;

			pRecord->iDeathInfoMap.insert( KillDeathInfoMap::value_type(iClassType, iDeathCnt) );
		}

		if( bDieCheck )
		{
			bool bDie;
			rkPacket >> bDie;
			if( bDie )
			{
				pRecord->pChar->SetDieState();
			}
		}
	}
	else
	{
		int iKillCharCnt, iDeathCharCnt;

		rkPacket >> iKillCharCnt;
		for( int i=0; i < iKillCharCnt; ++i )
		{
			int iClassType, iKillCnt;
			rkPacket >> iClassType >> iKillCnt;
		}

		rkPacket >> iDeathCharCnt;
		for( int j=0; j < iDeathCharCnt; ++j )
		{
			int iClassType, iDeathCnt;
			rkPacket >> iClassType >> iDeathCnt;
		}

		if( bDieCheck )
		{
			bool bDie;
			rkPacket >> bDie;
		}
	}
}

bool ioHeroMatchMode::ProcessTCPPacket( SP2Packet &rkPacket )
{
	ErrorReport::SetPosition( 1030, 9 );
	if( ioPlayMode::ProcessTCPPacket( rkPacket ) )
		return true;

	ErrorReport::SetPosition( 1030, 10 );

	return false;
}

bool ioHeroMatchMode::ProcessUDPPacket( SP2Packet &rkPacket )
{
	if( ioPlayMode::ProcessUDPPacket( rkPacket ) )
		return true;

	return false;
}

void ioHeroMatchMode::NotifyEntityDie( ioPlayEntity *pEntity, ioPlayEntity *pKiller, ioBaseChar *pBestAttacker,
								       float fLastRate, float fBestRate )
{	
	ioBaseChar *pDieChar = ToBaseChar( pEntity );
	ioBaseChar *pKillChar = ToBaseChar( pKiller );

	OnNotifyCharDie( pDieChar, pKillChar, pBestAttacker, fLastRate, fBestRate );
	UpdateKillDeathRecord( pEntity, pKiller, pBestAttacker );
}

void ioHeroMatchMode::NotifyCharDropDie( ioBaseChar *pDieChar, ioBaseChar *pKillChar, ioBaseChar *pBestAttacker,
										 float fLastRate, float fBestRate )
{
	OnNotifyCharDie( pDieChar, pKillChar, pBestAttacker, fLastRate, fBestRate );
	UpdateDropDieRecord( pDieChar, pKillChar, pBestAttacker );
}

void ioHeroMatchMode::OnNotifyCharDie( ioBaseChar *pDieChar, ioBaseChar *pKillChar, ioBaseChar *pBestAttacker,
									   float fLastRate, float fBestRate )
{
	if( !pDieChar )
	{
		LOG.PrintTimeAndLog( 0, "ioHeroMatchMode::OnNotifyCharDie - Entity is Not Char" );
		return;
	}

	HeroMatchRecord *pDieRecord = FindRecordCharName( pDieChar->GetCharName() );
	if( !pDieRecord )
	{
		LOG.PrintTimeAndLog( 0, "ioHeroMatchMode::OnNotifyCharDie - %s Not Exist Char",
								pDieChar->GetCharName().c_str() );
		return;
	}

	if( pKillChar && pKillChar->GetTeam() != pDieChar->GetTeam() )
	{
		if( pBestAttacker && pBestAttacker->GetCharName() == pKillChar->GetCharName() )
		{
			pKillChar->SetKOHPRecover( m_fKORecoverRate * 2.0f, m_PerfectRecoverEffect );
			
			if( m_pCreator && m_pCreator->CheckRoomKOEffect() )
			{
				SetRecoveryGauge( pKillChar, fLastRate );
				//pKillChar->AddNewBuff( m_KOBuff, "", "", NULL );
			}
		}
		else
		{
			pKillChar->SetKOHPRecover( m_fKORecoverRate, m_KORecoverEffect );
			
			if( m_pCreator && m_pCreator->CheckRoomKOEffect() )
			{
				SetRecoveryGauge( pKillChar, fLastRate );
				//pKillChar->AddNewBuff( m_KOBuff, "", "", NULL );
			}

			if( pBestAttacker )
			{
				pBestAttacker->SetKOHPRecover( m_fKORecoverRate, m_KORecoverEffect );
				
				if( m_pCreator && m_pCreator->CheckRoomKOEffect() )
				{
					SetRecoveryGauge( pBestAttacker, fBestRate );
					//pBestAttacker->AddNewBuff( m_KOBuff, "", "", NULL );
				}

				if( pBestAttacker->IsOwnerChar() )
				{
					m_pCreator->StartScreenBlind( SBT_KILL, NULL, true );
				}
			}
		}
	}

	if( P2PNetwork::IsNetworkPlaying() )
	{
		if( g_MyInfo.GetPublicID() == pDieChar->GetCharName() )
		{
			g_MyInfo.SetCharDied( pDieChar->GetSelectCharArray(), true );
		}
	}

	//For EndScene
	m_LastDieChar = pDieChar->GetCharName();

	if( pKillChar )
	{
		HeroMatchRecord *pKillRecord = FindRecordCharName( pKillChar->GetCharName() );
		if( !pKillRecord )
		{
			LOG.PrintTimeAndLog( 0, "ioHeroMatchMode::OnNotifyCharDie - %s Not Exist Kill Char",
									pKillChar->GetCharName().c_str() );

			m_LastKillChar.Clear();
		}
		else
		{
			if( pKillChar->GetTeam() == pDieChar->GetTeam() )
				m_LastKillChar.Clear();
			else
				m_LastKillChar = pKillChar->GetCharName();
		}
	}
	else
	{
		m_LastKillChar.Clear();
	}
}

void ioHeroMatchMode::UpdateDropDieRecord( ioBaseChar *pDieChar, ioBaseChar *pKillChar, ioBaseChar *pBestAttacker )
{
	bool bDropKill = true;
	if( pKillChar )
	{
		ModeRecord *pAttRecord = FindModeRecordCharName( pKillChar->GetCharName() );
		ModeRecord *pDieRecord = FindModeRecordCharName( pDieChar->GetCharName() );

		if( pAttRecord && pDieRecord )
		{
			float fKillPoint = FLOAT05 + ( (float)GetKillPoint( pAttRecord->pChar->GetTeam() ) * FLOAT05 );
			if( pAttRecord->pChar->GetTeam() != pDieRecord->pChar->GetTeam() )
			{
				pAttRecord->AddKill( GetModeType(), fKillPoint, bDropKill, false );
				pDieRecord->AddDeath( GetModeType(), GetDeathPoint( pDieRecord->pChar->GetTeam() ) );

				g_ModeUIController.SetKOMsg( pAttRecord->pChar->GetTeam(), pAttRecord->pChar->GetCharName(), 
					                         pDieRecord->pChar->GetLastAttackerSkillName(), pDieRecord->pChar->GetLastAttackerItemCode(),
											 pDieRecord->pChar->GetTeam(), pDieRecord->pChar->GetCharName() );
			}
			else
			{
				if( pAttRecord->pChar == pDieRecord->pChar )	// �ڱ��ڽ��� ����
				{
					pDieRecord->AddDeath( GetModeType(), GetDeathPoint( pDieRecord->pChar->GetTeam() ) );
					g_ModeUIController.SetKOMsg( TEAM_NONE, "",
						                         "", 0, pDieRecord->pChar->GetTeam(), pDieRecord->pChar->GetCharName() );
				}
				else
				{
					pAttRecord->AddKill( GetModeType(), -fKillPoint, bDropKill, false );
					pDieRecord->AddDeath( GetModeType(), GetDeathPoint( pDieRecord->pChar->GetTeam() ) );

					g_ModeUIController.SetKOMsg( TEAM_NONE, "",
						                         "", 0, pDieRecord->pChar->GetTeam(), pDieRecord->pChar->GetCharName() );
				}
			}

			pAttRecord->pChar->SetKillEmoticon( pAttRecord->GetUniqueTotalKill() );
			pDieRecord->pChar->SetDeathEmoticon();
		}
	}
	else
	{
		ModeRecord *pDieRecord = FindModeRecordCharName( pDieChar->GetCharName() );
		if( pDieRecord )
		{
			pDieRecord->AddDeath( GetModeType(), GetDeathPoint( pDieRecord->pChar->GetTeam() ) );

			g_ModeUIController.SetKOMsg( TEAM_NONE, "",
				                         "", 0, pDieRecord->pChar->GetTeam(), pDieRecord->pChar->GetCharName() );

			pDieRecord->pChar->SetDeathEmoticon();
		}
	}

	// Best Attacker ó��
	ioBaseChar *pBestAttChar = ToBaseChar( pBestAttacker );
	if( pBestAttChar && pDieChar )
	{
		ModeRecord *pAttRecord = FindModeRecordCharName( pBestAttChar->GetCharName() );
		if( pAttRecord && pDieChar->GetTeam() != pBestAttChar->GetTeam() )
		{
			float fKillPoint = FLOAT05 + ( (float)GetKillPoint( pBestAttChar->GetTeam() ) * FLOAT05 );
			pAttRecord->AddKill( GetModeType(), fKillPoint, bDropKill, true );
		}
	}
}

void ioHeroMatchMode::ReStartMode()
{
	D3DXVECTOR3 vStartPos;
	int iCharCnt = m_vCharRecordList.size();
	for( int i=0 ; i<iCharCnt ; i++ )
	{
		HeroMatchRecord &rkRecord = m_vCharRecordList[i];
		if( !rkRecord.pChar ) continue;
		vStartPos = GetStartPos( rkRecord.pChar->GetTeam() );
		rkRecord.pChar->SetWorldPosition( vStartPos );

		rkRecord.dwRevivalGap = 0;
		rkRecord.dwCurDieTime = 0;
		rkRecord.iRevivalCnt = 0;

		if( m_iCurRoundCount == 1 )	// Final Round End
		{
			rkRecord.InitValues();

			m_iBlueRoundWinCnt = 0;
			m_iRedRoundWinCnt = 0;
		}

		rkRecord.pChar->SetProtectionMode( PM_NONE, 0 );
		rkRecord.pChar->SetRoundEndMotion( RM_NONE );
		rkRecord.pChar->SetProtectionMode( PM_START, 2000 );
		rkRecord.pChar->Revival();

		if( rkRecord.pChar->GetState() != CS_OBSERVER )
		{
			if( !g_BattleRoomMgr.IsObserverUser( rkRecord.pChar->GetCharName() ) )
				rkRecord.pChar->SetReadyState();
			else if( g_ShuffleRoomMgr.IsShuffleRoom() )
				rkRecord.pChar->SetReadyState();
			else
				rkRecord.pChar->SetState( CS_OBSERVER );
		}
	}

	m_dwCurRevivalTime = 0;
	m_dwCheckWarningTime = 0;
	m_bScoreWarning = false;
	m_bFirstSelect = true;

	// ������
	m_bZeroHP = false;
	TimeScoreWnd *pTimeScoreWnd = dynamic_cast<TimeScoreWnd *>( g_GUIMgr.FindWnd( TIME_SCORE_WND ) );
	if(pTimeScoreWnd)
		pTimeScoreWnd->InitAniState();

	int iHistorySize = m_vRoundHistory.size();
	if( (m_iCurRoundCount-1) == iHistorySize )
	{
		RoundHistory rh;
		m_vRoundHistory.push_back( rh );
	}

	if( m_bCharDieClearByRound )
	{
		g_MyInfo.ClearCharDiedInfo();
	}

	if( P2PNetwork::IsNetworkPlaying() )
	{
		m_pCreator->HidingAllFieldItem();
		m_pCreator->DestroyWeaponList();
	}

	g_GUIMgr.CheckShopSound( true );
	SetModeState( MS_READY );

	m_pCreator->ReCalculateDamageRate( true );

	// ���� ���۽� �긮��
	m_pCreator->SetModeEndSceneCamera( false );

	if( !P2PNetwork::IsNetworkPlaying() )
		m_pCreator->StartModeStartEventCamera( true );
	else
		m_pCreator->StartModeStartEventCamera( false );

	SetModeMapSound();
	m_pCreator->EnableWorldEntityFunctions( true );
	m_pCreator->SetMapCameraFixedObject( false );

	// MapAreaWeapon Set
	SetMapAreaWeapon();
}

D3DXVECTOR3 ioHeroMatchMode::GetRevivalPos( ioBaseChar *pChar )
{
	D3DXVECTOR3 vPos = GetStartPos( pChar->GetTeam() );
	vPos.x += (float)( rand() % 100 ) / 20.0f;
	vPos.z += (float)( rand() % 100 ) / 20.0f;
	vPos.y = m_pCreator->GetMapHeightByRevial( vPos.x, vPos.z );

	return vPos;
}

void ioHeroMatchMode::ContributeLOG()
{
	char szLog[MAX_PATH];

	int iCharCnt = m_vCharRecordList.size();
	for( int i=0 ;i<iCharCnt; i++ )
	{
		HeroMatchRecord *pRecord = &m_vCharRecordList[i];
		if( pRecord->pChar )
		{
			sprintf( szLog, "%s[%d] : K%d(%d) D%d(%d) A%d(%d)", pRecord->szName.c_str(), (int)pRecord->pChar->GetTeam(), //Except Extracting Hangeul
																pRecord->Contribute.m_iKillRank + 1, pRecord->Contribute.m_iKill,
																pRecord->Contribute.m_iDamageRank + 1, pRecord->Contribute.m_iDamage,
																pRecord->Contribute.m_iAttackCntRank + 1, pRecord->Contribute.m_iAttackCnt );
			g_ChatMgr.SetSystemMsg( szLog );
		}		
	}
}

void ioHeroMatchMode::ContributeConvertTeam( TeamType eTeam )
{
	if( m_vContributeTable.size() < MAX_PLAYER )
		return;

	int i = 0;
	int iCharCnt = m_vCharRecordList.size();
	int iInfoCnt = 0;
	ContributeInfoList vInfo;

	//KO
	for(i = 0;i < iCharCnt;i++)
	{
		HeroMatchRecord *pRecord = &m_vCharRecordList[i];
		if( !pRecord ) continue;
		if( !pRecord->pChar ) continue;
		if( pRecord->pChar->GetState() == CS_OBSERVER ) continue;
		if( g_BattleRoomMgr.IsObserverUser(pRecord->pChar->GetCharName()) ) continue;

		if( eTeam != TEAM_NONE )
		{
			if( pRecord->pChar && pRecord->pChar->GetTeam() != eTeam ) continue;
		}

		ContributeInfo Contribute;
		Contribute.m_iRecordArray     = i;
		Contribute.m_iContributePoint = pRecord->Contribute.m_iKill;
		vInfo.push_back( Contribute );
	}
	iInfoCnt = vInfo.size();

	std::sort( vInfo.begin(), vInfo.end(), ContributeSort() );
	ContributeLowerSort( vInfo );

	for(i = 0;i < iInfoCnt;i++)
	{
		int iArray = vInfo[i].m_iRecordArray;
		if( COMPARE( iArray, 0, iCharCnt ) )
		{
			HeroMatchRecord *pRecord = &m_vCharRecordList[iArray];
			if( !pRecord->Contribute.m_bLastResultSync )
				pRecord->Contribute.m_iContribute = m_vContributeTable[vInfo[i].m_iRank];
			pRecord->Contribute.m_iKillRank = vInfo[i].m_iRank;
		}
	}
	vInfo.clear();

	// ������ & ���� ����
	for(i = 0;i < iCharCnt;i++)
	{
		HeroMatchRecord *pRecord = &m_vCharRecordList[i];
		if( !pRecord ) continue;
		if( !pRecord->pChar ) continue;
		if( pRecord->pChar->GetState() == CS_OBSERVER ) continue;
		if( g_BattleRoomMgr.IsObserverUser( pRecord->pChar->GetCharName() ) ) continue;

		if( eTeam != TEAM_NONE )
		{
			if( pRecord->pChar && pRecord->pChar->GetTeam() != eTeam ) continue;
		}

		ContributeInfo Contribute;
		Contribute.m_iRecordArray = i;
		Contribute.m_iContributePoint = pRecord->Contribute.m_iDamage;
		vInfo.push_back( Contribute );
	}
	iInfoCnt = vInfo.size();

	std::sort( vInfo.begin(), vInfo.end(), ContributeSort() );
	ContributeLowerSort( vInfo );
	
	for(i = 0;i < iInfoCnt;i++)
	{
		int iArray = vInfo[i].m_iRecordArray;
		if( COMPARE( iArray, 0, iCharCnt ) )
		{
			HeroMatchRecord *pRecord = &m_vCharRecordList[iArray];
			if( !pRecord->Contribute.m_bLastResultSync )
				pRecord->Contribute.m_iContribute += m_vContributeTable[vInfo[i].m_iRank]  * 3;
			pRecord->Contribute.m_iDamageRank = vInfo[i].m_iRank;
		}
	}
	vInfo.clear();

	//��ȿ Ÿ�� Ƚ��
	for(i = 0;i < iCharCnt;i++)
	{
		HeroMatchRecord *pRecord = &m_vCharRecordList[i];
		if( !pRecord ) continue;
		if( !pRecord->pChar ) continue;
		if( pRecord->pChar->GetState() == CS_OBSERVER ) continue;
		if( g_BattleRoomMgr.IsObserverUser( pRecord->pChar->GetCharName() ) ) continue;

		if( eTeam != TEAM_NONE )
		{
			if( pRecord->pChar && pRecord->pChar->GetTeam() != eTeam ) continue;
		}

		ContributeInfo Contribute;
		Contribute.m_iRecordArray = i;
		Contribute.m_iContributePoint = pRecord->Contribute.m_iAttackCnt;
		vInfo.push_back( Contribute );
	}
	iInfoCnt = vInfo.size();

	std::sort( vInfo.begin(), vInfo.end(), ContributeSort() );
	ContributeLowerSort( vInfo );

    for(i = 0;i < iInfoCnt;i++)
	{
		int iArray = vInfo[i].m_iRecordArray;
		if( COMPARE( iArray, 0, iCharCnt ) )
		{
			HeroMatchRecord *pRecord = &m_vCharRecordList[iArray];
			if( !pRecord->Contribute.m_bLastResultSync )
				pRecord->Contribute.m_iContribute += m_vContributeTable[vInfo[i].m_iRank] * 2;
			pRecord->Contribute.m_iAttackCntRank = vInfo[i].m_iRank;
		}
	}
	vInfo.clear();

	//����ó��
	for(i = 0;i < iCharCnt;i++)
	{
		HeroMatchRecord *pRecord = &m_vCharRecordList[i];
		if( !pRecord ) continue;
		if( !pRecord->pChar ) continue;
		if( pRecord->pChar->GetState() == CS_OBSERVER ) continue;
		if( g_BattleRoomMgr.IsObserverUser(pRecord->pChar->GetCharName()) ) continue;

		if( eTeam != TEAM_NONE )
		{
			if( pRecord->pChar && pRecord->pChar->GetTeam() != eTeam ) continue;
		}

		ContributeInfo Contribute;
		Contribute.m_iRecordArray = i;
		Contribute.m_iContributePoint = 100000 - pRecord->Contribute.m_iDeath;          
		vInfo.push_back( Contribute );
	}
	iInfoCnt = vInfo.size();

	std::sort( vInfo.begin(), vInfo.end(), ContributeSort() );
	ContributeLowerSort( vInfo );

	for(i = 0;i < iInfoCnt;i++)
	{
		int iArray = vInfo[i].m_iRecordArray;
		if( COMPARE( iArray, 0, iCharCnt ) )
		{
			HeroMatchRecord *pRecord = &m_vCharRecordList[iArray];
			if( !pRecord->Contribute.m_bLastResultSync )
				pRecord->Contribute.m_iContribute += m_vContributeTable[vInfo[i].m_iRank];
			pRecord->Contribute.m_iDeathRank = vInfo[i].m_iRank;
		}
	}
	vInfo.clear();
}

void ioHeroMatchMode::ContributeConvert()
{
	ContributeConvertTeam( TEAM_NONE );        //TEAM_NONE�� ��ü ������ ���� �⿩�� ��	
}

int ioHeroMatchMode::ContributeMyRank( int &rRankValue1, int &rRankValue2, int &rRankValue3, int &rRankValue4 )
{
	rRankValue1 = rRankValue2 = rRankValue3 = rRankValue4 = -1;

	bool bObserver = false;
	ioBaseChar *pOwner = GetOwnerChar();
	if( pOwner && ( g_BattleRoomMgr.IsObserverUser( pOwner->GetCharName() ) || m_pCreator->IsStealthMode() ) )
		bObserver = true;

	int i = 0;
	int iCharCnt = m_vCharRecordList.size();
	int iInfoCnt = 0;
	ContributeInfoList vInfo;
	//KO
	for(i = 0;i < iCharCnt;i++)
	{
		HeroMatchRecord *pRecord = &m_vCharRecordList[i];
		if( !pRecord ) continue;
		if( !pRecord->pChar ) continue;
		if( pRecord->pChar->GetState() == CS_OBSERVER ) continue;
		if( g_BattleRoomMgr.IsObserverUser(pRecord->pChar->GetCharName()) ) continue;

		ContributeInfo Contribute;
		Contribute.m_iRecordArray     = i;
		Contribute.m_iContributePoint = pRecord->Contribute.m_iKill;
		Contribute.m_iGradeLevel      = pRecord->pChar->GetLevel();
		Contribute.m_szName           = pRecord->szName;
		vInfo.push_back( Contribute );
	}
	std::sort( vInfo.begin(), vInfo.end(), ContributeRankSort() );
	iInfoCnt = vInfo.size();

	if( !bObserver )
	{
		for(rRankValue1 = 0;rRankValue1 < iInfoCnt;rRankValue1++)
		{
			ContributeInfo &kContribute = vInfo[rRankValue1];
			if( kContribute.m_szName == g_MyInfo.GetPublicID() )
				break;
		}
	}

	vInfo.clear();	

	//��ȿ Ÿ�� Ƚ��
	for(i = 0;i < iCharCnt;i++)
	{
		HeroMatchRecord *pRecord = &m_vCharRecordList[i];		
		if( !pRecord ) continue;
		if( !pRecord->pChar ) continue;
		if( pRecord->pChar->GetState() == CS_OBSERVER ) continue;
		if( g_BattleRoomMgr.IsObserverUser( pRecord->pChar->GetCharName() ) ) continue;

		ContributeInfo Contribute;
		Contribute.m_iRecordArray = i;
		Contribute.m_iContributePoint = pRecord->Contribute.m_iAttackCnt;
		Contribute.m_iGradeLevel      = pRecord->pChar->GetLevel();
		Contribute.m_szName           = pRecord->szName;
		vInfo.push_back( Contribute );
	}
	std::sort( vInfo.begin(), vInfo.end(), ContributeRankSort() );
	iInfoCnt = vInfo.size();

	if( !bObserver )
	{
		for(rRankValue2 = 0;rRankValue2 < iInfoCnt;rRankValue2++)
		{
			ContributeInfo &kContribute = vInfo[rRankValue2];
			if( kContribute.m_szName == g_MyInfo.GetPublicID() )
				break;
		}
	}

	vInfo.clear();	

	// ������ & ���� ����
	for(i = 0;i < iCharCnt;i++)
	{
		HeroMatchRecord *pRecord = &m_vCharRecordList[i];		
		if( !pRecord ) continue;
		if( !pRecord->pChar ) continue;
		if( pRecord->pChar->GetState() == CS_OBSERVER ) continue;
		if( g_BattleRoomMgr.IsObserverUser( pRecord->pChar->GetCharName() ) ) continue;

		ContributeInfo Contribute;
		Contribute.m_iRecordArray = i;
		Contribute.m_iContributePoint = pRecord->Contribute.m_iDamage;
		Contribute.m_iGradeLevel      = pRecord->pChar->GetLevel();
		Contribute.m_szName           = pRecord->szName;
		vInfo.push_back( Contribute );
	}
	std::sort( vInfo.begin(), vInfo.end(), ContributeRankSort() );
	iInfoCnt = vInfo.size();

	if( !bObserver )
	{
		for(rRankValue3 = 0;rRankValue3 < iInfoCnt;rRankValue3++)
		{
			ContributeInfo &kContribute = vInfo[rRankValue3];
			if( kContribute.m_szName == g_MyInfo.GetPublicID() )
				break;
		}
	}

	vInfo.clear();	

	//������ & ��
	for(i = 0;i < iCharCnt;i++)
	{
		HeroMatchRecord *pRecord = &m_vCharRecordList[i];
		if( !pRecord ) continue;
		if( !pRecord->pChar ) continue;
		if( pRecord->pChar->GetState() == CS_OBSERVER ) continue;
		if( g_BattleRoomMgr.IsObserverUser(pRecord->pChar->GetCharName()) ) continue;

		ContributeInfo Contribute;
		Contribute.m_iRecordArray = i;
		Contribute.m_iContributePoint = pRecord->Contribute.m_iDamage;
		Contribute.m_iGradeLevel      = pRecord->pChar->GetLevel();
		Contribute.m_szName           = pRecord->szName;
		vInfo.push_back( Contribute );
	}
	std::sort( vInfo.begin(), vInfo.end(), ContributeRankSort() );
	iInfoCnt = vInfo.size();

	if( !bObserver )
	{
		for(rRankValue3 = 0;rRankValue3 < iInfoCnt;rRankValue3++)
		{
			ContributeInfo &kContribute = vInfo[rRankValue3];
			if( kContribute.m_szName == g_MyInfo.GetPublicID() )
				break;
		}
	}

	vInfo.clear();	
	return iInfoCnt;
}

void ioHeroMatchMode::ContributeRoundPercent( DWORD dwCurTime, DWORD dwTotalTime )
{
	//�⿩���� 100����
	int iMaxBlueContribute	= 0;
	int iMaxRedContribute	= 0;
	int iMaxBlueUser		= 0;
	int iMaxRedUser			= 0;
	int iCharCnt			= m_vCharRecordList.size();
	int i = 0;
	for(i = 0;i < iCharCnt;i++)
	{
		HeroMatchRecord *pRecord = &m_vCharRecordList[i];
		if( !pRecord->pChar ) continue;
		if( pRecord->pChar->GetTeam() == TEAM_RED )
		{
			iMaxRedContribute += pRecord->Contribute.m_iContribute;
			iMaxRedUser++;
		}
		else if( pRecord->pChar->GetTeam() == TEAM_BLUE )
		{
			iMaxBlueContribute += pRecord->Contribute.m_iContribute;
			iMaxBlueUser++;
		}		
	}

	for(i = 0;i < iCharCnt;i++)
	{
		HeroMatchRecord *pRecord = &m_vCharRecordList[i];

		if( pRecord->pChar->GetTeam() == TEAM_RED )
		{
			if( iMaxRedContribute > 0 )
				pRecord->Contribute.m_iPrevContributePer = ((float)pRecord->Contribute.m_iContribute / iMaxRedContribute ) * FLOAT100;
		}
		else if( pRecord->pChar->GetTeam() == TEAM_BLUE )
		{
			if( iMaxBlueContribute > 0 )
				pRecord->Contribute.m_iPrevContributePer = ((float)pRecord->Contribute.m_iContribute / iMaxBlueContribute ) * FLOAT100;
		}		
	}

	int iBlueCnt       = m_iBlueRoundWinCnt;
	int iRedCnt        = m_iRedRoundWinCnt;
	TeamType eWinTeam  = TEAM_NONE;

	int iAverageClick  = 8;
	int iWinBonus      = 3;
	int iDrawBonus     = 1;
	int iLoseBonus     = 1;

	if( iBlueCnt > iRedCnt )
		eWinTeam = TEAM_BLUE;
	else if( iRedCnt > iBlueCnt )
		eWinTeam = TEAM_RED;

	int iDiffCnt       = min( abs( iBlueCnt - iRedCnt ), iAverageClick );
	int iWinCnt        = iAverageClick + iDiffCnt;
	int iLoseCnt       = iAverageClick - iDiffCnt;
	int iClickCnt      = 0;
	float fPer         = 0.0f;
	if( dwTotalTime > 0 )
		fPer = (float)dwCurTime / dwTotalTime;

	for(i = 0;i < iCharCnt;i++)
	{
		HeroMatchRecord *pRecord = &m_vCharRecordList[i];

		if( pRecord->pChar->GetTeam() == eWinTeam )
			iClickCnt = iWinCnt;
		else 
			iClickCnt = iLoseCnt;

		if( eWinTeam == TEAM_NONE )  //����� ��� ���ʽ�.
			iClickCnt += iDrawBonus;
		else if( pRecord->pChar->GetTeam() == eWinTeam )       // �¸��� ���ʽ�.
			iClickCnt += iWinBonus;
		else	//�й��� ���ʽ�
			iClickCnt += iLoseBonus;

		if( pRecord->pChar->GetTeam() == TEAM_BLUE )
			pRecord->Contribute.m_iPrevContributePer = (float)((float)(pRecord->Contribute.m_iPrevContributePer * ( iMaxBlueUser * iClickCnt )) * 0.35f) * fPer;
		else if( pRecord->pChar->GetTeam() == TEAM_RED )
			pRecord->Contribute.m_iPrevContributePer = (float)((float)(pRecord->Contribute.m_iPrevContributePer * ( iMaxRedUser * iClickCnt )) * 0.35f) * fPer;		
	}	
}

void ioHeroMatchMode::ContributeRoundAllPercent( DWORD dwCurTime, DWORD dwTotalTime )
{
	//�⿩���� 100����
	int iMaxContribute	= 0;
	int iCharCnt		= m_vCharRecordList.size();
	int iPlayCharCnt	= 0;

	int i = 0;
	for(i = 0;i < iCharCnt;i++)
	{
		HeroMatchRecord *pRecord = &m_vCharRecordList[i];
		if( !pRecord ) continue;
		if( !pRecord->pChar ) continue;
		if( pRecord->pChar->GetState() == CS_OBSERVER ) continue;
		if( g_BattleRoomMgr.IsObserverUser( pRecord->pChar->GetCharName() ) ) continue;
		
		iMaxContribute += pRecord->Contribute.m_iContribute;
		iPlayCharCnt++;
	}

	int iOwnerContribute = -1;
	if( iMaxContribute > 0 )
	{
		for(i = 0;i < iCharCnt;i++)
		{
			HeroMatchRecord *pRecord = &m_vCharRecordList[i];
			if( !pRecord ) continue;
			if( !pRecord->pChar ) continue;
			if( pRecord->pChar->GetState() == CS_OBSERVER ) continue;
			if( g_BattleRoomMgr.IsObserverUser( pRecord->pChar->GetCharName() ) ) continue;
			
			float fContributePer = (float)iPlayCharCnt * ((float)pRecord->Contribute.m_iContribute / iMaxContribute );
			pRecord->Contribute.m_iPrevContributePer = ( fContributePer + FLOAT0001 ) * FLOAT100;

			if( pRecord->pChar->IsOwnerChar() )
				iOwnerContribute = pRecord->Contribute.m_iPrevContributePer;

			g_AwardMgr.AddContribute( pRecord->szName, pRecord->Contribute.m_iPrevContributePer );
		}
	}

	if( m_ModeState == MS_PLAY && iOwnerContribute >= 0 )
	{
		int iPraise = g_MyInfo.CheckContributePraise( iOwnerContribute );
		if( iPraise > 0 )
		{
			/*
			ioBaseChar *pOwner = GetOwnerChar();
			if( pOwner )
				pOwner->SetPraiseEmoticon( ioModeHelpManager::HT_CONTRIBUTE_PRAISE, iPraise );
			*/
			g_ChatMgr.SetSystemMsg( STR(1), iPraise );
			g_ModeHelpMgr.ShowContributePraise( iPraise );
		}
	}
}

void ioHeroMatchMode::DamageContribute( const ioHashString szAttacker, const ioHashString szTarget, float fDamage, DWORD dwItemCode, int iWeaponType )
{
	if( szAttacker.IsEmpty() ) return;

	HeroMatchRecord *pRecord = FindRecordCharName( szAttacker );
	if( !pRecord || !pRecord->pChar ) return;
	
	ioBaseChar *pAttacker = m_pCreator->GetBaseChar( szAttacker );
	if( !pAttacker ) return;

	ioBaseChar *pTarget   = m_pCreator->GetBaseChar( szTarget );
	if( !pTarget ) return;

	// �ڽſ� ���� Ÿ�� ������ ����
	if( pAttacker->GetCharName() == pTarget->GetCharName() ) return;

	float fBonusDamage = 0;
	switch( iWeaponType )
	{
	case ioWeapon::WT_BLIZZARD:
		fBonusDamage = fDamage;
		break;
	}

	float fAddDamagePoint = FLOAT1;
//	if( IsContributeAddPointItemCode( dwItemCode ) )
//		fAddDamagePoint = 1.1f;
    
	float fDamagePoint = (float)((fDamage + fBonusDamage) * FLOAT100) * pow( ContributeTeamPerA( pRecord->pChar->GetTeam(), 8.0f, FLOAT1 ), 3 );
	pRecord->Contribute.m_iDamage += (float)fDamagePoint * fAddDamagePoint; 	
	ContributeConvert();
	ContributeRoundAllPercent();
	UpdateRoundInfoList();
	CheckTotalChamp();

	if( fDamagePoint > 0.0f && !pAttacker->IsOwnerChar() )
	{
		CheckNonePlayTime( pAttacker );
	}

	g_AwardMgr.AddDamage( pRecord->szName, fDamagePoint, fDamage );
}

void ioHeroMatchMode::KillContribute( const ioHashString szKiller, const ioHashString szKilled, DWORD dwItemCode )
{
	if( szKilled.IsEmpty() ) return;

	int iKillPoint = FLOAT100;

	float fAddKillPoint = FLOAT1;
	if( IsContributeAddPointItemCode( dwItemCode ) )
		fAddKillPoint = 1.1f;

	if( szKiller.IsEmpty() || szKiller == szKilled )     //�ڻ�
	{
		HeroMatchRecord *pRecord = FindRecordCharName( szKilled );
		if( !pRecord || !pRecord->pChar ) return;

		pRecord->Contribute.m_iKill -= iKillPoint;
	}
	else
	{
		HeroMatchRecord *pKillerRecord = FindRecordCharName( szKiller );
		HeroMatchRecord *pKilledRecord = FindRecordCharName( szKilled );
		if( pKillerRecord && pKilledRecord )
		{
			if( pKillerRecord->pChar && pKilledRecord->pChar )
			{
				if( pKillerRecord->pChar->GetTeam() == pKilledRecord->pChar->GetTeam() )    //��ų
				{
					pKillerRecord->Contribute.m_iKill -= (float)iKillPoint * fAddKillPoint;
					g_AwardMgr.AddTeamKill( pKillerRecord->szName );
				}
				else
					pKillerRecord->Contribute.m_iKill += (float)((float)iKillPoint * fAddKillPoint) * ContributeTeamPerA( pKillerRecord->pChar->GetTeam(), 8.0f, FLOAT1 );
			}
		}
	}

	ContributeConvert();
	ContributeRoundAllPercent();
	UpdateRoundInfoList();
	CheckTotalChamp();
}

void ioHeroMatchMode::AttackCntContribute( const ioHashString szAttacker, const ioHashString szTarget, DWORD dwItemCode, int iAttackValue/* = 100 */ )
{
	if( szAttacker.IsEmpty() || szTarget.IsEmpty() ) return;

	HeroMatchRecord *pAttackerRecord = FindRecordCharName( szAttacker );
	HeroMatchRecord *pTargetRecord = FindRecordCharName( szTarget );

	ioBaseChar *pAttack = m_pCreator->GetBaseChar( szAttacker );
	ioBaseChar *pTarget = m_pCreator->GetBaseChar( szTarget );
	if( !pAttack || !pTarget )
		return;

	if( pAttack != pTarget )
		CheckNonePlayTime( pAttack );

	//�� Ÿ�� ���� �ؼ� �������� �ȵǾ���-_-;
	//�ӽ� ����
	if( iAttackValue != 50 )
	{
		if( pAttack->GetTeam() == pTarget->GetTeam() )
			return;
	}	

	float fAddAttackPoint = FLOAT1;
	if( IsContributeAddPointItemCode( dwItemCode ) )
		fAddAttackPoint = 1.1f;

	if( pAttackerRecord && pTargetRecord )
	{
		if( pAttackerRecord->pChar && pTargetRecord->pChar )
		{
			pAttackerRecord->Contribute.m_iAttackCnt += (float)iAttackValue * fAddAttackPoint;
		}
	}

	ContributeConvert();
	ContributeRoundAllPercent();
	UpdateRoundInfoList();
	CheckTotalChamp();
}

void ioHeroMatchMode::CheckLastKiller()
{
}

void ioHeroMatchMode::FillLastPlayRecordInfo( SP2Packet &rkPacket )
{
	int iCharCnt = m_vCharRecordList.size();
	rkPacket << iCharCnt;
	for(int i = 0;i < iCharCnt;i++)
	{
		HeroMatchRecord *pRecord = &m_vCharRecordList[i];
		rkPacket << pRecord->pChar->GetCharName();
		rkPacket << pRecord->Contribute.m_iContribute;		
		rkPacket << pRecord->GetUniqueTotalKill();
		rkPacket << pRecord->GetUniqueTotalDeath();
		rkPacket << g_VictoriesMgr.GetVictories( pRecord->pChar->GetCharName() );

#if defined( USE_GA )
		if( g_MyInfo.GetPublicID() == pRecord->pChar->GetCharName() )
		{
			g_HttpMng.SetKill( pRecord->GetUniqueTotalKill() );
			g_HttpMng.SetDeath( pRecord->GetUniqueTotalDeath() );
		}
#endif
	}
}

void ioHeroMatchMode::FillPlayRecordInfo( SP2Packet &rkPacket )
{	
	int iCharCnt = m_vCharRecordList.size();
	rkPacket << iCharCnt;
	for(int i = 0;i < iCharCnt;i++)
	{
		HeroMatchRecord *pRecord = &m_vCharRecordList[i];
		rkPacket << pRecord->pChar->GetCharName();
		rkPacket << pRecord->Contribute.m_iDamage;
		rkPacket << pRecord->Contribute.m_iAttackCnt;
		rkPacket << pRecord->Contribute.m_iKill;
		rkPacket << pRecord->Contribute.m_iDeath;
		rkPacket << pRecord->Contribute.m_iPrevContributePer;
		rkPacket << pRecord->GetUniqueTotalKill();
		rkPacket << pRecord->GetUniqueTotalDeath();
		rkPacket << g_VictoriesMgr.GetVictories( pRecord->pChar->GetCharName() );
	}
}

void ioHeroMatchMode::ApplyPlayRecordInfo( SP2Packet &rkPacket )
{
	int iCharCnt;
	rkPacket >> iCharCnt;
	for(int i = 0;i < iCharCnt;i++)
	{
		ioHashString szName;
		int iDamage, iAttackCnt, iKill, iDeath, iPrevContributePer, iUniqueTotalKill, iUniqueTotalDeath, iVictories;
		rkPacket >> szName >> iDamage >> iAttackCnt >> iKill >> iDeath >> iPrevContributePer >> iUniqueTotalKill >> iUniqueTotalDeath >> iVictories;

		HeroMatchRecord *pRecord = FindRecordCharName( szName );
		if( pRecord )
		{
			pRecord->Contribute.m_iDamage		     = iDamage;
			pRecord->Contribute.m_iAttackCnt         = iAttackCnt;
			pRecord->Contribute.m_iKill              = iKill;
			pRecord->Contribute.m_iDeath             = iDeath;
			pRecord->Contribute.m_iPrevContributePer = iPrevContributePer;
			pRecord->iUniqueTotalKill = iUniqueTotalKill;
			pRecord->iUniqueTotalDeath= iUniqueTotalDeath;

			if( !g_LadderTeamMgr.IsLadderTeam() )
				g_VictoriesMgr.SetVictories( szName, iVictories );
		}
	}
	
	ContributeConvert();
	ContributeRoundAllPercent();
	UpdateRoundInfoList();
	CheckTotalChamp();
}

void ioHeroMatchMode::ContributeResultSync( const ioHashString &szName, int iContribute )
{
	HeroMatchRecord *pRecord = FindRecordCharName( szName );
	if( !pRecord ) return;

	pRecord->Contribute.m_bLastResultSync = true;
	pRecord->Contribute.m_iContribute     = iContribute;
}

void ioHeroMatchMode::CheckNonePlayTime( ioBaseChar *pChar, bool bPlayerChanged )
{
	if( !pChar )
	{
		ioBaseChar *pOwner = GetOwnerChar();

		TeamType eOwnerTeam = pOwner->GetTeam();
		TeamType eEnemyTeam = TEAM_NONE;

		if( eOwnerTeam == TEAM_BLUE )
			eEnemyTeam = TEAM_RED;
		else if( eOwnerTeam == TEAM_RED )
			eEnemyTeam = TEAM_BLUE;

		int iEnemyCount = GetTeamPurePlayerCount( eEnemyTeam );
		int iTeamCount = GetTeamPurePlayerCount( eOwnerTeam );
		int iTotalCount = iTeamCount + iEnemyCount;

		if( iTotalCount > 2 && iTeamCount == 1 )
			pOwner->CheckNonePlayTime( false, bPlayerChanged );
		else
			pOwner->CheckNonePlayTime( true, false );
	}
	else
	{
		TeamType eOwnerTeam = pChar->GetTeam();
		TeamType eEnemyTeam = TEAM_NONE;

		if( eOwnerTeam == TEAM_BLUE )
			eEnemyTeam = TEAM_RED;
		else if( eOwnerTeam == TEAM_RED )
			eEnemyTeam = TEAM_BLUE;

		int iEnemyCount = GetTeamPurePlayerCount( eEnemyTeam );
		int iTeamCount = GetTeamPurePlayerCount( eOwnerTeam );
		int iTotalCount = iTeamCount + iEnemyCount;

		if( iTotalCount > 2 && iTeamCount == 1 )
			pChar->CheckNonePlayTime( false, bPlayerChanged );
		else
			pChar->CheckNonePlayTime( true, false );
	}
}

void ioHeroMatchMode::OnZeroHP( SP2Packet &rkPacket )
{
	rkPacket >> m_dwSuddenDeathTime;

	int iCharCnt = m_vCharRecordList.size();
	for( int i=0 ; i<iCharCnt ; i++ )
	{
		m_vCharRecordList[i].pChar->SetCurHP( 0.0f );
		m_vCharRecordList[i].pChar->SetCurHPRate( 0.0f );
	}

	m_bZeroHP = true;
	g_SoundMgr.PlaySound( m_TimeOutSound, DSBVOLUME_MAX, PSM_LOOP | PSM_THREAD );
	g_ModeUIController.SetDeathTime();
}

void ioHeroMatchMode::OnRoundReady( SP2Packet &rkPacket )
{
	ShowEndScoreWnd( false );
	SetBlindMode( false );

	rkPacket >> m_iCurRoundCount;
	rkPacket >> m_iBluePosArray;
	rkPacket >> m_iRedPosArray;
	rkPacket >> m_dwCurRoundDuration;

	if( m_iCurRoundCount == 1 )
	{
		g_MyInfo.ClearCharDiedInfo();
		g_MyInfo.ClearCharDiedPenaltyInfo();
		g_MyInfo.ClearCharJoinedInfo();
	}

	ClearModeData();
	ReStartMode();

	g_GUIMgr.HideWnd( MIDDLE_JOINDELAY_WND );
}

void ioHeroMatchMode::OnRoundJoin( SP2Packet &rkPacket )
{
	ioHashString szJoinChar;
	int iModeState;

	rkPacket >> szJoinChar;
	rkPacket >> iModeState;

	if( szJoinChar.IsEmpty() ) return;

	ioBaseChar *pJoinChar = m_pCreator->GetBaseChar( szJoinChar );
	if( !pJoinChar ) return;

	if( pJoinChar->IsOwnerChar() )
	{
		SetModeState( (ModeState)iModeState );

		DWORD dwStateGap;
		rkPacket >> dwStateGap;

		DWORD dwStartTime = FRAMEGETTIME() - dwStateGap;
		switch( m_ModeState )
		{
		case MS_READY:
			m_dwReadyStartTime = dwStartTime;
			break;
		case MS_PLAY:
			m_dwPlayStartTime = dwStartTime;
			break;
		case MS_RESULT:
			m_dwResultStartTime = dwStartTime;
			break;
		}

		if( m_ModeState != MS_RESULT )
		{
			int iSelectChar = 0;
			
			iSelectChar = pJoinChar->GetSelectCharArray();
			pJoinChar->SetReadyState();

			DWORD dwSelectTime;
			rkPacket >> dwSelectTime;

			StartChangeCharacter( iSelectChar, dwSelectTime );

			if( g_BattleRoomMgr.IsBattleRoom() )
				g_QuestMgr.QuestCompleteTerm( QC_ENTER_BATTLE_PVP, GetModeType() );
			else if( g_LadderTeamMgr.IsLadderTeam() )
				g_QuestMgr.QuestCompleteTerm( QC_ENTER_CAMP_BATTLE, GetModeType() );
			if( g_ShuffleRoomMgr.IsShuffleRoom() )
				g_QuestMgr.QuestCompleteTerm( QC_ENTER_BATTLE_PVP, GetModeType() );
		}		
		else
		{
			SetResultJoinState();
		}

		g_ModeHelpMgr.InitModeHelp();

		rkPacket >> m_dwCurRoundDuration;
	}
	else 
	{
		pJoinChar->SetReadyState();

		if( m_ModeState == MS_PLAY )
		{
			ioBaseChar *pOwner = GetOwnerChar();
			DWORD dwCurTime = FRAMEGETTIME();
			if( pOwner )
			{
				if( m_iCurRoundCount != 1 )
					g_ChatMgr.SetJoinUserChatMsg( pJoinChar->GetCharName().c_str(), pJoinChar->GetTeam(), pJoinChar->GetLevel() );
				else if( dwCurTime > m_dwPlayStartTime && dwCurTime - m_dwPlayStartTime >= 10000 )
					g_ChatMgr.SetJoinUserChatMsg( pJoinChar->GetCharName().c_str(), pJoinChar->GetTeam(), pJoinChar->GetLevel() );
			}
		}
	}

	m_pCreator->IncreaseTeamCntForBalance( pJoinChar->GetTeam() );
	m_pCreator->ReCalculateDamageRate( false );
}

void ioHeroMatchMode::OnRoundJoinView( SP2Packet &rkPacket )
{
	ioHashString szJoinChar;
	int iModeState;

	rkPacket >> szJoinChar;
	rkPacket >> iModeState;

	if( szJoinChar.IsEmpty() ) return;

	ioBaseChar *pJoinChar = m_pCreator->GetBaseChar( szJoinChar );
	if( !pJoinChar ) return;

	if( pJoinChar->IsOwnerChar() )
	{
		SetModeState( (ModeState)iModeState );

		DWORD dwStateGap;
		rkPacket >> dwStateGap;

		DWORD dwStartTime = FRAMEGETTIME() - dwStateGap;
		switch( m_ModeState )
		{
		case MS_READY:
			m_dwReadyStartTime = dwStartTime;
			break;
		case MS_PLAY:
			m_dwPlayStartTime = dwStartTime;
			break;
		case MS_RESULT:
			m_dwResultStartTime = dwStartTime;
			break;
		}

		int iSelectChar = 0;

		pJoinChar->SetState( CS_VIEW );

		g_ModeHelpMgr.InitModeHelp();
		g_ModeHelpMgr.ShowHelp( ioModeHelpManager::HT_VIEW );

		SetBlindMode( true );

		g_ChatMgr.SetInfomationMsg( STR(1) );
		g_ChatMgr.SetInfomationMsg( STR(2) );

		rkPacket >> m_dwCurRoundDuration;
	}
	else if( m_ModeState == MS_PLAY )
	{
		ioBaseChar *pOwner = GetOwnerChar();
		DWORD dwCurTime = FRAMEGETTIME();
		if( pOwner )
		{
			if( m_iCurRoundCount != 1 )
				g_ChatMgr.SetJoinUserChatMsg( pJoinChar->GetCharName().c_str(), pJoinChar->GetTeam(), pJoinChar->GetLevel() );
			else if( dwCurTime > m_dwPlayStartTime && dwCurTime - m_dwPlayStartTime >= 10000 )
				g_ChatMgr.SetJoinUserChatMsg( pJoinChar->GetCharName().c_str(), pJoinChar->GetTeam(), pJoinChar->GetLevel() );
		}

		pJoinChar->SetState( CS_VIEW );
	}

	m_pCreator->ReCalculateDamageRate( false );
}

void ioHeroMatchMode::OnRoundJoinObserver( SP2Packet &rkPacket )
{
	ioHashString szJoinChar;
	int iModeState;

	rkPacket >> szJoinChar;
	rkPacket >> iModeState;

	if( szJoinChar.IsEmpty() ) return;

	ioBaseChar *pJoinChar = m_pCreator->GetBaseChar( szJoinChar );
	if( !pJoinChar ) return;

	if( pJoinChar->IsOwnerChar() )
	{
		SetModeState( (ModeState)iModeState );

		DWORD dwStateGap;
		rkPacket >> dwStateGap;

		DWORD dwStartTime = FRAMEGETTIME() - dwStateGap;
		switch( m_ModeState )
		{
		case MS_READY:
			m_dwReadyStartTime = dwStartTime;
			break;
		case MS_PLAY:
			m_dwPlayStartTime = dwStartTime;
			break;
		case MS_RESULT:
			m_dwResultStartTime = dwStartTime;
			break;
		}

		int iSelectChar = 0;

		pJoinChar->SetState( CS_OBSERVER );

		g_ModeHelpMgr.InitModeHelp();

		g_ChatMgr.SetInfomationMsg( STR(1) );
		g_ChatMgr.SetInfomationMsg( STR(2) );

		rkPacket >> m_dwCurRoundDuration;

		if( g_BattleRoomMgr.IsBattleRoom() )
		{
			g_QuestMgr.QuestCompleteTerm( QC_ENTER_BATTLE_OBSERVER, GetModeType() );
		}
	}
	else if( m_ModeState == MS_PLAY )
	{
		pJoinChar->SetState( CS_OBSERVER );
	}

	m_pCreator->ReCalculateDamageRate( false );
}

void ioHeroMatchMode::SetResultScoreWnd()
{
	RoundResultMainWnd *pResult = dynamic_cast<RoundResultMainWnd*>(g_GUIMgr.FindWnd( ROUND_RESULT_MAIN_WND ));
	if( !pResult )	return;

	// RoundHistory�� �����ϴ� ���� ���� �ϴ� ����. ���߿� �������� ���� �ʿ�
	int iHisRoundBlue = m_iBlueRoundWinCnt;
	int iHisRoundRed  = m_iRedRoundWinCnt;

	if( P2PNetwork::IsNetworkPlaying() )
	{
		int iRoundCount = min( m_iCurRoundCount, (int)m_vRoundHistory.size() );
		if ( iRoundCount >= 1 )
		{
			for(int i = 0;i < iRoundCount - 1;i++)
			{
				iHisRoundBlue -= m_vRoundHistory[ i ].iBluePoint;
				iHisRoundRed  -= m_vRoundHistory[ i ].iRedPoint;
			}
			m_vRoundHistory[ iRoundCount - 1 ].iBluePoint = max( iHisRoundBlue, 0 );
			m_vRoundHistory[ iRoundCount - 1 ].iRedPoint  = max( iHisRoundRed, 0 );
		}
		/*
		for(int i = 0;i < m_iCurRoundCount - 1;i++)
		{
			iHisRoundBlue -= m_vRoundHistory[ i ].iBluePoint;
			iHisRoundRed  -= m_vRoundHistory[ i ].iRedPoint;
		}
		m_vRoundHistory[ m_iCurRoundCount - 1 ].iBluePoint = max( iHisRoundBlue, 0 );
		m_vRoundHistory[ m_iCurRoundCount - 1 ].iRedPoint  = max( iHisRoundRed, 0 );
		*/
	}
	//

	m_dwCurResultShowDuration = m_dwRoundResultDuration;

	DWORD dwTimeGap = FRAMEGETTIME() - m_dwPlayStartTime;
	if( m_dwCurRoundDuration > dwTimeGap )
		ContributeRoundAllPercent( m_dwRoundDuration - (m_dwCurRoundDuration - dwTimeGap), m_dwRoundDuration );
	else
		ContributeRoundAllPercent( m_dwRoundDuration, m_dwRoundDuration );

	pResult->ClearRoundInfo();
	for(int i = 0 ; i < GetRecordCharCnt(); i++ )
	{
		RoundInfo kInfo;
		if( GetRoundInfo( i, kInfo ) )
			pResult->AddRoundInfo( kInfo );
	}

	pResult->SetTeamScore( m_iBlueRoundWinCnt, m_iRedRoundWinCnt );

	SetResultBriefingWnd();
	if( m_iRedRoundWinCnt > m_iBlueRoundWinCnt )
		PointUpEvent( TEAM_RED );
	else if( m_iRedRoundWinCnt < m_iBlueRoundWinCnt )
		PointUpEvent( TEAM_BLUE );
}

void ioHeroMatchMode::SetTotalScoreWnd( IntVec &rvClassTypeList,
								    IntVec &rvClassPointList,
									float fBlueGuildTeamBonus,
									float fRedGuildTeamBonus,
									float fBonusArray[BA_MAX] )
{
	TotalResultMainWnd *pTotalResult = dynamic_cast<TotalResultMainWnd *>(g_GUIMgr.FindWnd( TOTAL_RESULT_MAIN_WND ));
	if( !pTotalResult ) return;
    
	if( g_GUIMgr.IsShow( ROUND_RESULT_MAIN_WND ) )
		g_GUIMgr.HideWnd( ROUND_RESULT_MAIN_WND );

	ContributeRoundAllPercent( m_dwRoundDuration, m_dwRoundDuration );

	int i = 0;
	int iRecordCnt = GetRecordCharCnt();
	for( i=0 ; i<iRecordCnt ; i++ )
	{
		FinalInfo kInfo;
		if( GetFinalInfo( i, kInfo ) )
		{
			pTotalResult->AddFinalInfo( kInfo );
			if( g_BattleRoomMgr.IsBattleRoom() )
				g_BattleRoomMgr.UpdateUserLevel( kInfo.m_szName, kInfo.m_iLevel );
			else if( g_LadderTeamMgr.IsLadderTeam() )
				g_LadderTeamMgr.UpdateUserData( kInfo.m_szName, kInfo.m_iLevel, kInfo.m_iResultLadderPoint );
			else if( g_ShuffleRoomMgr.IsShuffleRoom() )
				g_ShuffleRoomMgr.UpdateUserLevel( kInfo.m_szName, kInfo.m_iLevel );
		}
	}

	pTotalResult->SetGuildTeamBonus( fBlueGuildTeamBonus, fRedGuildTeamBonus );
	pTotalResult->SetFinalRoundPoint( rvClassTypeList, rvClassPointList, fBonusArray );

	int iHisRoundBlue = m_iBlueRoundWinCnt;
	int iHisRoundRed  = m_iRedRoundWinCnt;

	if( P2PNetwork::IsNetworkPlaying() )
	{
		int iRoundCount = min( m_iCurRoundCount, (int)m_vRoundHistory.size() );
		if ( iRoundCount >= 1 )
		{
			for(int i = 0;i < iRoundCount - 1;i++)
			{
				iHisRoundBlue -= m_vRoundHistory[ i ].iBluePoint;
				iHisRoundRed  -= m_vRoundHistory[ i ].iRedPoint;
			}
			m_vRoundHistory[ iRoundCount - 1 ].iBluePoint = max( iHisRoundBlue, 0 );
			m_vRoundHistory[ iRoundCount - 1 ].iRedPoint  = max( iHisRoundRed, 0 );
		}
		/*
		for(int i = 0;i < m_iCurRoundCount - 1;i++)
		{
			iHisRoundBlue -= m_vRoundHistory[ i ].iBluePoint;
			iHisRoundRed  -= m_vRoundHistory[ i ].iRedPoint;
		}
		m_vRoundHistory[ m_iCurRoundCount - 1 ].iBluePoint = max( iHisRoundBlue, 0 );
		m_vRoundHistory[ m_iCurRoundCount - 1 ].iRedPoint  = max( iHisRoundRed, 0 );
		*/
	}

	m_dwCurResultShowDuration = m_dwFinalRoundResultDuration;

//	pTotal->SetMaxRound( m_vRoundHistory.size() );

//	for( i = 0 ; i < (int)m_vRoundHistory.size() ; i++ )
//	{
//		pTotal->SetRoundHistory( i, m_vRoundHistory[i] );
//	}

	pTotalResult->SetTeamScore( GetOwnerTeam(), m_iBlueRoundWinCnt, m_iRedRoundWinCnt );
	SetTotalResultBriefingWnd();
}

ioGameEntity* ioHeroMatchMode::GetGuideTarget( const D3DXVECTOR3& vArrowPos )
{
	return NULL;
}

void ioHeroMatchMode::CheckEndSceneLookAt()
{
	if( !m_LastKillChar.IsEmpty() )
	{
		HeroMatchRecord *pKillRecord = FindRecordCharName( m_LastKillChar );
		if( !pKillRecord )
		{
			m_vEndLookAt = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			m_szEndLookCharName.Clear();
			return;
		}
		else
		{
			m_vEndLookAt = pKillRecord->pChar->GetWorldPosition();	
			m_szEndLookCharName = pKillRecord->pChar->GetCharName();
			pKillRecord->pChar->SetEndFocusEffect( GetRoundEndEffect() );
		}
	}
	else
	{
		if( !m_LastDieChar.IsEmpty() )
		{
			HeroMatchRecord *pDieRecord = FindRecordCharName( m_LastDieChar );
			if( !pDieRecord )
			{
				m_vEndLookAt = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
				m_szEndLookCharName.Clear();
				return;
			}
			else
			{
				ioBaseChar *pTarget = CheckMostNearChar( pDieRecord->pChar );
				if( pTarget )
				{
					m_vEndLookAt = pTarget->GetWorldPosition();		
					m_szEndLookCharName = pTarget->GetCharName();
					pTarget->SetEndFocusEffect( GetRoundEndEffect() );
				}
				else
				{
					m_vEndLookAt = pDieRecord->pChar->GetWorldPosition();	
					m_szEndLookCharName = pDieRecord->pChar->GetCharName();
					pDieRecord->pChar->SetEndFocusEffect( GetRoundEndEffect() );
				}
			}
		}
		else
		{
			ioBaseChar *pOwner = m_pCreator->GetOwnerChar();
			if( pOwner )
			{
				m_vEndLookAt = pOwner->GetWorldPosition();				
				m_szEndLookCharName = pOwner->GetCharName();
				pOwner->SetEndFocusEffect( GetRoundEndEffect() );
			}
			else
			{
				m_vEndLookAt = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );				
				m_szEndLookCharName.Clear();
			}
		}
	}
}

void ioHeroMatchMode::ClearModeData()
{
	m_LastDieChar.Clear();
	m_LastKillChar.Clear();

	g_SoundMgr.AllSoundStop();
	g_SoundMgr.StopStream();
	m_pCreator->HidingAllFieldItem();
}

bool ioHeroMatchMode::IsNeedRevival( ioBaseChar *pChar )
{
	if( !pChar ) return false;

	if( !m_LastKillChar.IsEmpty() && m_LastKillChar == pChar->GetCharName() )
		return true;

	return false;
}

ioBaseChar* ioHeroMatchMode::CheckMostNearChar( ioBaseChar *pChar )
{
	ioBaseChar *pNearChar = NULL;
	float fCharDistSq = std::numeric_limits<float>::infinity();
	int iCharCnt = m_vCharRecordList.size();
	for( int i=0 ; i<iCharCnt ; i++ )
	{
		ioBaseChar *pTarget = m_vCharRecordList[i].pChar;
		if( !pChar ) continue;
		if( pChar->GetTeam() == pTarget->GetTeam() ) continue;
		if( pTarget->GetState() == CS_DIE ||
			pTarget->GetState() == CS_VIEW ||
			pTarget->GetState() == CS_READY ||
			pTarget->GetState() == CS_OBSERVER ||
			pTarget->GetState() == CS_LOADING )
			continue;
		
		D3DXVECTOR3 vDiff = pChar->GetWorldPosition() - pTarget->GetWorldPosition();
		float fDiffSq = D3DXVec3LengthSq( &vDiff );
		if( fDiffSq < fCharDistSq )
		{
			pNearChar = pTarget;
			fCharDistSq = fDiffSq;
		}
	}

	return pNearChar;
}

void ioHeroMatchMode::UpdateRevivalTime( const ioHashString &szName, DWORD dwCurRevivalTime )
{
	ModeRecord *pDieRecord = FindModeRecordCharName( szName );
	if( !pDieRecord )
	{
		LOG.PrintTimeAndLog( 0, "ioHeroMatchMode::UpdateRevivalTime() - %s is not exist to the record", szName.c_str() );
		return;
	}

	m_dwCurRevivalTime = dwCurRevivalTime;

	pDieRecord->dwCurDieTime = FRAMEGETTIME();
	pDieRecord->dwRevivalGap = m_dwCurRevivalTime;
	pDieRecord->iRevivalCnt++;

	if( pDieRecord->pChar->IsOwnerChar() )
	{
		SetBlindMode( true );
	}
}

int ioHeroMatchMode::GetCurRank( const ioHashString& szCharName )
{
	ModeRecord *pTargetRecord = FindModeRecordCharName( szCharName );
	if( !pTargetRecord )	return -1;
	if( !pTargetRecord->pChar ) return -1;

	TeamType eTeam = pTargetRecord->pChar->GetTeam();

	int iCurRank = 1;
	int iSize = m_RoundInfoList.size();
	for( int i=0; i < iSize; i++ )
	{
		const RoundInfo &kRoundInfo = m_RoundInfoList[i];

		if( kRoundInfo.m_szName == szCharName )
			return iCurRank;

		if( kRoundInfo.m_Team == eTeam )
			iCurRank++;
	}

	return -1;
}

int ioHeroMatchMode::GetContributePerChar( const ioHashString& szName )
{
	int iCharCnt = m_vCharRecordList.size();

	for(int i = 0;i < iCharCnt;i++)
	{
		HeroMatchRecord *pRecord = &m_vCharRecordList[i];
		ErrorReport::SetPosition( 3000, 1 );
		if( !pRecord )	continue;
		ErrorReport::SetPosition( 3000, 2 );
		if( !pRecord->pChar ) continue;
		ErrorReport::SetPosition( 3000, 3 );

		if( pRecord->szName == szName )
		{
			ErrorReport::SetPosition( 3000, 4 );
			return pRecord->Contribute.m_iPrevContributePer;
			ErrorReport::SetPosition( 3000, 5 );
		}
	}

	ErrorReport::SetPosition( 3000, 6 );
	return 0;
}
