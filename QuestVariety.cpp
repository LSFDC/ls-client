
#include "stdafx.h"
#include "ioTime.h"

#include "GUI/TournamentDetailMainWnd.h"
#include "GUI/QuestWnd.h"
#include "GUI/ioFriendManager.h"

#include "ioPlayStage.h"
#include "ioPlayMode.h"
#include "ioBaseChar.h"
#include "ioLadderTeamMgr.h"
#include "ioExcavationManager.h"
#include "ioReservedTask.h"
#include "QuestVariety.h"
#include "ioDateHelp.h"

#include "ioShuffleRoomMgr.h"

QuestParent::QuestParent()
{
	m_bDirectSync = true;
	m_dwDirectSyncSafeTime = 0;

	m_dwMainIndex = m_dwSubIndex = 0;
	m_dwPerformType = 0;

	m_bOneDayStyle = false;
	m_bRepeatStyle = false;
	m_bGuildStyle  = false;
	m_iGuildTermsLevel = 0;
	m_bPCRoomStyle = false;

	m_eChannelingType = CNT_NONE;

	m_dwOccurValue = m_dwCompleteValue = 0;
	
	m_iOccurValueEx  = -1;
	m_iOccurModeType = m_iOccurRoomStyle = m_iCompleteModeType = m_iCompleteRoomStyle = -1;
	m_iOccurExceptingMode = m_iCompleteExceptingMode = -1;
	m_iOccurLimitGradeMin = m_iOccurLimitGradeMax = -1;

	m_dwPeriodHour = 0;
	
	m_bRewardSelectStyle = false;
	m_iRewardSelectNum = 1;
	
	m_dwPrevMainIndex = m_dwPrevSubIndex = 0;

	m_dwNextMainIndex = m_dwNextSubIndex = 0;

	m_dwHelpIndex = 0;
	m_pIcon = NULL;

	m_bAlive = false;
	m_bInvitedPass = false;
	m_bUserModePass = false; 
	m_bDeathTimePass = false;

	m_bPlayOccurShow = true;
	m_bPlayCompleteShow = true;

	m_bRoundEndOccurShow = true;
	m_bRoundEndCompleteShow = true;

	m_bCompleteGameAlarm = false;
	m_bCompleteWebAlarm  = false;

	m_bIsCostume = false;
}

QuestParent::~QuestParent()
{
	m_vRewardPresent.clear();
	m_CustomValue.clear();

	SAFEDELETE( m_pIcon );
}

void QuestParent::ShowTooltip()
{
	if( m_szProgressResult.IsEmpty() )
		return;

	if( !IsQuestAlarm() )   // �˸��̿� ��ϵ� ����Ʈ�� ���� ǥ��
		return;

	ioHashString kString = GetProgressResult();
	if( kString == m_szBackUpTooltipResult )
		return;

	m_szBackUpTooltipResult = kString;
	QuestToolTip *pQuestTooltip = dynamic_cast<QuestToolTip*>(g_GUIMgr.FindWnd( QUEST_TOOLTIP ));
	if( pQuestTooltip )
		pQuestTooltip->ShowTooltip( GetMainIndex(), GetSubIndex() );
}

DWORD QuestParent::GetRewardPresentIndex( int iArray )
{
	if( !COMPARE( iArray, 0, (int)m_vRewardPresent.size() ) ) return 0;

	return m_vRewardPresent[iArray];
}

bool QuestParent::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( GetPerformType() == QP_EVENT )
	{
		if( !IsAlive() )
			return false;
	}

	if( IsPCRoomStyle() )
	{
		if( !g_MyInfo.GetPCRoomAuthority() )
			return false;
		if( g_MyInfo.IsUserEvent() )
			return false;
	}

	if( GetOccurValue() == 0 )
		return true;

	if( GetOccurValue() != dwTerm )
		return false;	

	// ���� ó��
	if( g_BattleRoomMgr.IsBattleRoom() )
	{
		ioHashString szBlockID = STR(1);
		if( g_BattleRoomMgr.IsHaveUser( szBlockID ) )
			return false;
	}

	// �߻� �����߿� �߰������� üũ�ؾ��ϴ� ��
	switch( GetOccurValue() )
	{
	case QO_ETCITEM_USE:
		if( GetOccurValueEx() != -1 )
		{
			if( iValue1 != GetOccurValueEx() )
				return false;
		}
		break;
	}

	// ��� ���� ����Ʈ Ȯ��
	if( IsGuildStyle() )
	{
		if( !g_GuildInfo.IsGuild() )
			return false;
		if( (int)g_GuildInfo.GetMyGuildLevel() < GetGuildTermsLevel() )
			return false;
	}

	ioQuest *pQuest = g_MyInfo.GetQuest();
	if( !pQuest )
		return false;

	// ä�θ� üũ
	if( GetChannelingType() != CNT_NONE )
	{
		if( GetChannelingType() != g_MyInfo.GetChannelingType() )
			return false;
	}

	// ���� ����Ʈ�� �Ϸ�Ǿ�� ����
	if( GetPrevMainIndex() > 0 && GetPrevSubIndex() > 0 )
	{
		if( !pQuest->IsQuestComplete( GetPrevMainIndex(), GetPrevSubIndex() ) )
			return false;
	}

	// �̹� �Ϸ��߰ų� ������ ����Ʈ�� �߻� ���ǿ��� ����.
	if( pQuest->IsQuestIndexCheck( GetMainIndex(), GetSubIndex() ) )
		return false;

	// �ּ� ��� üũ
	if( m_iOccurLimitGradeMin != -1 )
	{
		if( g_MyInfo.GetGradeLevel() < m_iOccurLimitGradeMin )
			return false;
	}

	// �ִ� ��� üũ
	if( m_iOccurLimitGradeMax != -1 )
	{
		if( g_MyInfo.GetGradeLevel() > m_iOccurLimitGradeMax )
			return false;
	}

	// �� ��Ÿ�� üũ ( ���� or ���� )
	switch( m_iOccurRoomStyle )
	{
	case BATTLE_STYLE:
		{
			if( !g_BattleRoomMgr.IsBattleRoom() )
				return false;
		}
		break;
	case LADDER_STYLE:
		{
			if( !g_LadderTeamMgr.IsLadderTeam() )
				return false;
		}
		break;
	case ALL_BATTLE_STYLE:
		{
			if( !g_BattleRoomMgr.IsBattleRoom() && !g_LadderTeamMgr.IsLadderTeam() && !g_ShuffleRoomMgr.IsShuffleRoom() )
				return false;
		}
		break;
	case SHUFFLE_STYLE:
		{
			if( !g_ShuffleRoomMgr.IsShuffleRoom() )
				return false;
		}
		break;
	}

	// ��� üũ	
	if( m_iOccurModeType != -1 )
	{
		if( ioPlayMode::GetModeType() != m_iOccurModeType )
			return false;
	}

	// ���� ��� üũ
	if( m_iOccurExceptingMode != -1 )
	{
		if( ioPlayMode::GetModeType() == m_iOccurExceptingMode )
			return false;
	}	
	return true;
}

bool QuestParent::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( GetPerformType() == QP_EVENT )
	{
		if( !IsAlive() ) return false;
	}

	if( IsPCRoomStyle() )
	{
		if( !g_MyInfo.GetPCRoomAuthority() ) return false;
		if( g_MyInfo.IsUserEvent() )
			return false;
	}

	// ���� ó��
	if( g_BattleRoomMgr.IsBattleRoom() )
	{
		//UJ 131011, ����ȭ: GetString() ���� ����
		static ioHashString szBlockID = STR(1);
		if( g_BattleRoomMgr.IsHaveUser( szBlockID ) )
			return false;
	}

	ioQuest *pQuest = g_MyInfo.GetQuest();
	if( !pQuest )
		return false;

	// �̹� �Ϸ��� ����Ʈ�� üũ���� ����.
	if( pQuest->IsQuestAttain( GetMainIndex(), GetSubIndex() ) )
		return false;
	if( pQuest->IsQuestComplete( GetMainIndex(), GetSubIndex() ) )
		return false;

	// �������� ����Ʈ�� �Ϸ��
	if( !IsMeProgressQuest() )
		return false;

	if( GetCompleteValue() != dwTerm )
		return false;
	
	// �� ��Ÿ�� üũ ( ���� or ���� )
	switch( m_iCompleteRoomStyle )
	{
	case BATTLE_STYLE:
		{
			if( !g_BattleRoomMgr.IsBattleRoom() )
				return false;
		}
		break;
	case LADDER_STYLE:
		{
			if( !g_LadderTeamMgr.IsLadderTeam() )
				return false;
		}
		break;
	case ALL_BATTLE_STYLE:
		{
			if( !g_BattleRoomMgr.IsBattleRoom() && !g_LadderTeamMgr.IsLadderTeam() && !g_ShuffleRoomMgr.IsShuffleRoom() )
				return false;
		}
		break;
	case SHUFFLE_STYLE:
		{
			if( !g_ShuffleRoomMgr.IsShuffleRoom() )
				return false;
		}
		break;
	}
    
	// ��� üũ	
	if( m_iCompleteModeType != -1 )
	{
		if( ioPlayMode::GetModeType() != m_iCompleteModeType )
			return false;
	}

	// ���� ��� üũ
	if( m_iCompleteExceptingMode != -1 )
	{
		if( ioPlayMode::GetModeType() == m_iCompleteExceptingMode )
			return false;
	}

	// ���� ��� üũ
	if( m_bUserModePass )
	{
		// TDOD : jal ��Ƽ��� Ŭ����� �� ����Ʈ üũ ���� Ȯ��
		if( g_BattleRoomMgr.IsBattleRoom() && g_BattleRoomMgr.IsUseExtraOption() )
			return false;
	}

	// ���� Ÿ�� üũ
	if( m_bDeathTimePass )
	{
		if( ioPlayMode::IsZeroHP() )
			return false;

		if( ioPlayMode::GetModeType() == MT_MONSTER_SURVIVAL || ioPlayMode::GetModeType() == MT_RAID || Help::IsMonsterDungeonMode(ioPlayMode::GetModeType()))
		{
			if( ioMonsterSurvivalMode::IsTurnDeathTime() )
				return false;
		}		
	}

	return true;
}

int QuestParent::GetCustomValue( int iIdx )
{
	if( !COMPARE( iIdx, 0, (int)m_CustomValue.size() ) ) 
		return 0;

	return m_CustomValue[iIdx];
}

ioUIRenderImage *QuestParent::GetQuestIcon()
{
	if( !m_pIcon ) return NULL;

	m_pIcon->SetScale( FLOAT1 );
	m_pIcon->SetAlpha( MAX_ALPHA_RATE );
	return m_pIcon;
}

float QuestParent::GetProgressGaugeRate()
{
	QuestData &rkData = GetMyInfoQuestData();
	if( rkData.GetMainIndex() == 0 ) return 0.0f;
	if( rkData.GetState() != QS_PROGRESS ) return FLOAT1;
	if( GetProgressCount() <= 0 ) return 0.0f;
	if( GetProgressResult().IsEmpty() ) return 0.0f;

	return (float)rkData.GetCurrentData() / (float)GetProgressCount();
}

int QuestParent::GetRenderImageStartHeight( ioUIRenderImage *pImage, int iSrcHeight, float fGapRate )
{
	if( !pImage )	return 0;

	fGapRate = min( FLOAT1, fGapRate );
	float fGapHeight = (float)iSrcHeight * fGapRate;
	int iOffYPos = pImage->GetOffsetY();
	int iHeight  = pImage->GetHeight();

	return iOffYPos + ( (iSrcHeight / 2) - fGapHeight );	
}

void QuestParent::RenderQuestIconGauge( int iXPos, int iYPos, float fScale )
{
	ioUIRenderImage *pQuestIcon = GetQuestIcon();
	if( !pQuestIcon ) return;

	pQuestIcon->SetScale( fScale );

	int iRealHeight = GetRenderImageStartHeight( pQuestIcon, pQuestIcon->GetHeight(), GetProgressGaugeRate() );
	pQuestIcon->Render( iXPos, iYPos, UI_RENDER_GRAY_LIGHT, TFO_BILINEAR );
	pQuestIcon->RenderHeightCut( iXPos, iYPos, iRealHeight, pQuestIcon->GetHeight(), UI_RENDER_NORMAL, TFO_BILINEAR );
	if( iRealHeight + 2 <= pQuestIcon->GetHeight() )
	{
		pQuestIcon->SetColor( 0, 0, 0 );
		pQuestIcon->RenderHeightCut( iXPos, iYPos, iRealHeight, iRealHeight + 2, UI_RENDER_NORMAL, TFO_BILINEAR );
		pQuestIcon->SetColor( 255, 255, 255 );
	}
}

void QuestParent::RenderQuestIcon( int iXPos, int iYPos, float fScale )
{
	ioUIRenderImage *pQuestIcon = GetQuestIcon();
	if( !pQuestIcon ) return;

	pQuestIcon->SetScale( fScale );
	pQuestIcon->Render( iXPos, iYPos, UI_RENDER_NORMAL, TFO_BILINEAR );
}

bool QuestParent::SetQuestData( DWORD dwMainIndex, DWORD dwSubIndex, ioINILoader &rkLoader )
{
	m_dwMainIndex = dwMainIndex;
	m_dwSubIndex  = dwSubIndex;

	char szKey[MAX_PATH] = "";
	char szBuf[MAX_PATH] = "";
	sprintf_e( szKey, "sub%d_title", m_dwSubIndex );
	rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );
	m_szTitle = szBuf;

	sprintf_e( szKey, "sub%d_progress", m_dwSubIndex );
	rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );
	m_szProgressText = szBuf;

	sprintf_e( szKey, "sub%d_progress_result", m_dwSubIndex );
	rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );
	m_szProgressResult = szBuf;

	sprintf_e( szKey, "sub%d_help", m_dwSubIndex );
	m_dwHelpIndex = rkLoader.LoadInt( szKey, 0 );

	SAFEDELETE( m_pIcon );
	sprintf_e( szKey, "sub%d_icon", m_dwSubIndex );
	rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );
	m_pIcon = g_UIImageSetMgr.CreateImageByFullName( szBuf );

	sprintf_e( szKey, "sub%d_costume", m_dwSubIndex );	
	m_bIsCostume = rkLoader.LoadBool( szKey, false );

	sprintf_e( szKey, "sub%d_perform_type", m_dwSubIndex );	
	m_dwPerformType = rkLoader.LoadInt( szKey, 0 );

	sprintf_e( szKey, "sub%d_oneday_style", m_dwSubIndex );
	m_bOneDayStyle = rkLoader.LoadBool( szKey, false );

	sprintf_e( szKey, "sub%d_repeat_style", m_dwSubIndex );
	m_bRepeatStyle = rkLoader.LoadBool( szKey, false );

	sprintf_e( szKey, "sub%d_guild_style", m_dwSubIndex );
	m_bGuildStyle = rkLoader.LoadBool( szKey, false );

	sprintf_e( szKey, "sub%d_guild_terms_level", m_dwSubIndex );
	m_iGuildTermsLevel = rkLoader.LoadInt( szKey, 0 );

	sprintf_e( szKey, "sub%d_pcroom_style", m_dwSubIndex );
	m_bPCRoomStyle = rkLoader.LoadBool( szKey, false );

	sprintf_e( szKey, "sub%d_channeling_type", m_dwSubIndex );
	m_eChannelingType = (ChannelingType)rkLoader.LoadInt( szKey, CNT_NONE );

	sprintf_e( szKey, "sub%d_occur_value", m_dwSubIndex );	
	m_dwOccurValue = rkLoader.LoadInt( szKey, 0 );

	sprintf_e( szKey, "sub%d_occur_value_ex", m_dwSubIndex );	
	m_iOccurValueEx = rkLoader.LoadInt( szKey, -1 );

	sprintf_e( szKey, "sub%d_occur_mode", m_dwSubIndex );
	m_iOccurModeType = rkLoader.LoadInt( szKey, -1 );

	sprintf_e( szKey, "sub%d_occur_room_style", m_dwSubIndex );
	m_iOccurRoomStyle = rkLoader.LoadInt( szKey, -1 );

	sprintf_e( szKey, "sub%d_occur_excepting_mode", m_dwSubIndex );
	m_iOccurExceptingMode = rkLoader.LoadInt( szKey, -1 );

	sprintf_e( szKey, "sub%d_occur_limit_grade_min", m_dwSubIndex );
	m_iOccurLimitGradeMin = rkLoader.LoadInt( szKey, -1 );

	sprintf_e( szKey, "sub%d_occur_limit_grade_max", m_dwSubIndex );
	m_iOccurLimitGradeMax = rkLoader.LoadInt( szKey, -1 );

	sprintf_e( szKey, "sub%d_complete_value", m_dwSubIndex );	
	m_dwCompleteValue = rkLoader.LoadInt( szKey, 0 );

	sprintf_e( szKey, "sub%d_complete_mode", m_dwSubIndex );
	m_iCompleteModeType = rkLoader.LoadInt( szKey, -1 );

	sprintf_e( szKey, "sub%d_complete_room_style", m_dwSubIndex );
	m_iCompleteRoomStyle = rkLoader.LoadInt( szKey, -1 );

	sprintf_e( szKey, "sub%d_complete_excepting_mode", m_dwSubIndex );
	m_iCompleteExceptingMode = rkLoader.LoadInt( szKey, -1 );

	sprintf_e( szKey, "sub%d_period_hour", m_dwSubIndex );	
	m_dwPeriodHour = rkLoader.LoadInt( szKey, 0 );

	m_vRewardPresent.clear();
	sprintf_e( szKey, "sub%d_max_reward", m_dwSubIndex );
	int iMaxReward = rkLoader.LoadInt( szKey, 0 );
	int i = 0;
	for(;i < iMaxReward;i++)
	{
		sprintf_e( szKey, "sub%d_reward_present%d", m_dwSubIndex, i + 1 );
		m_vRewardPresent.push_back( rkLoader.LoadInt( szKey, 0 ) );
	}

	sprintf_e( szKey, "sub%d_rewardselect_style", m_dwSubIndex );
	m_bRewardSelectStyle = rkLoader.LoadBool( szKey, false );

	sprintf_e( szKey, "sub%d_rewardselect_num", m_dwSubIndex );
	m_iRewardSelectNum = rkLoader.LoadInt( szKey, 1 );

	sprintf_e( szKey, "sub%d_prev_main_index", m_dwSubIndex );	
	m_dwPrevMainIndex = rkLoader.LoadInt( szKey, 0 );

	sprintf_e( szKey, "sub%d_prev_sub_index", m_dwSubIndex );	
	m_dwPrevSubIndex = rkLoader.LoadInt( szKey, 0 );

	sprintf_e( szKey, "sub%d_next_main_index", m_dwSubIndex );	
	m_dwNextMainIndex = rkLoader.LoadInt( szKey, 0 );

	sprintf_e( szKey, "sub%d_next_sub_index", m_dwSubIndex );	
	m_dwNextSubIndex = rkLoader.LoadInt( szKey, 0 );

	sprintf_e( szKey, "sub%d_invited_pass", m_dwSubIndex );	
	m_bInvitedPass = rkLoader.LoadBool( szKey, false );

	sprintf_e( szKey, "sub%d_user_mode_pass", m_dwSubIndex );	
	m_bUserModePass = rkLoader.LoadBool( szKey, false );

	sprintf_e( szKey, "sub%d_death_time_pass", m_dwSubIndex );	
	m_bDeathTimePass = rkLoader.LoadBool( szKey, false );

	sprintf_e( szKey, "sub%d_play_occur_show", m_dwSubIndex );	
	m_bPlayOccurShow = rkLoader.LoadBool( szKey, true );

	sprintf_e( szKey, "sub%d_play_complete_show", m_dwSubIndex );	
	m_bPlayCompleteShow = rkLoader.LoadBool( szKey, true );

	sprintf_e( szKey, "sub%d_round_end_occur_show", m_dwSubIndex );	
	m_bRoundEndOccurShow = rkLoader.LoadBool( szKey, true );

	sprintf_e( szKey, "sub%d_round_end_complete_show", m_dwSubIndex );	
	m_bRoundEndCompleteShow = rkLoader.LoadBool( szKey, true );

	sprintf_e( szKey, "sub%d_complete_game_alarm", m_dwSubIndex );	
	m_bCompleteGameAlarm = rkLoader.LoadBool( szKey, false );

	sprintf_e( szKey, "sub%d_complete_web_alarm", m_dwSubIndex );	
	m_bCompleteWebAlarm = rkLoader.LoadBool( szKey, false );

	int iDefCustom = -999999999;
	for(i = 0;i < MAX_CUSTOM_VALUE;i++)
	{
		sprintf_e( szKey, "sub%d_custom_value%d", m_dwSubIndex, i + 1 );	
		int iCustomValue = rkLoader.LoadInt( szKey, iDefCustom );
		if( iCustomValue == iDefCustom ) 
			break;
		m_CustomValue.push_back( iCustomValue );
	}

	switch( GetPerformType() )
	{
	case QP_NORMAL:
		m_bAlive = true;
		break;
	case QP_EVENT:
		m_bAlive = false;
		break;
	}
	return true;
}

QuestData &QuestParent::GetMyInfoQuestData()
{
	return g_MyInfo.GetQuest()->GetQuestData( GetMainIndex(), GetSubIndex() );
}

bool QuestParent::IsMeProgressQuest()
{
	DWORD dwState = g_MyInfo.GetQuest()->GetQuestState( GetMainIndex(), GetSubIndex() );
	if( dwState == QS_PROGRESS )
		return true;
	return false;
}

bool QuestParent::IsQuestAlarm()
{
	QuestData &rkQuest = GetMyInfoQuestData();
	if( rkQuest.GetIndexData() == 0 )
		return false;
	if( rkQuest.GetState() != QS_PROGRESS )
		return false;
	if( !rkQuest.IsAlarm() )
		return false;
	return true;
}

bool QuestParent::IsQuestGuideShow()
{
	return IsQuestAlarm();
}

ioHashString &QuestParent::GetProgressResult()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		if( !IsMeProgressQuest() )
			sprintf_e( szBuf, "%d/%d%s", GetProgressCount(), GetProgressCount(), m_szProgressResult.c_str() );
		else
			sprintf_e( szBuf, "%d/%d%s", kQuestData.GetCurrentData(), GetProgressCount(), m_szProgressResult.c_str() );
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestParent::GetProgressResultOnlyNumber()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
		sprintf_e( szBuf, "%d/%d", kQuestData.GetCurrentData(), GetProgressCount() );
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestParent::GetProgressResultOnlyText()
{
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
		sprintf_e( szBuf, "%d%s", GetProgressCount(), m_szProgressResult.c_str() );
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

int QuestParent::GetProgressPercentage( int iValue1, int iValue2 )
{
	if( iValue2 <= 0 ) return 0;

	float fPercentage = (float)iValue1 / (float)iValue2;
	return fPercentage * FLOAT100;
}

void QuestParent::SendAttainData( QuestData &rkData )
{
	SP2Packet kPacket( CTPK_QUEST_ATTAIN );
	kPacket << rkData.GetIndexData() << rkData.GetValueData() << rkData.GetMagicData() << rkData.GetDateData();
	TCPNetwork::SendToServer( kPacket );
	rkData.SyncComplete();
	StartProcessTime();
}

void QuestParent::SendMacroAttain( QuestData &rkData )
{
	SP2Packet kPacket( CTPK_QUEST_ATTAIN );
	kPacket << rkData.GetIndexData() << rkData.GetValueData() << rkData.GetMagicData() << rkData.GetDateData();
	TCPNetwork::SendToServer( kPacket );
	rkData.SyncComplete();
	StartProcessTime();
	//ShowTooltip();
}

bool QuestParent::AddAttainProcess( int iAddValue /* = 0 */, int iCheckValue /* = 0  */ )
{
	QuestData &rkData = GetMyInfoQuestData();
	if( rkData.GetMainIndex() == 0 )
	{
		//�α� ���� ����
		char szError[MAX_PATH] = "";
		SafeSprintf( szError, sizeof( szError ), STR(1), g_MyInfo.GetPublicID().c_str(), g_MyInfo.GetPrivateID().c_str(), GetMainIndex(), GetSubIndex() );
		
		SP2Packet kPacket( LUPK_LOG );
		kPacket << "QuestError";	// �α� ���� Ÿ��
		kPacket << szError;
		kPacket << 702;				// ������ȣ:317
		kPacket << false;			// write db
		P2PNetwork::SendLog(kPacket);

		LOG.PrintTimeAndLog( 0, "AddCompleteProcess None Quest Type : %d - %d", GetMainIndex(), GetSubIndex() );
		return false;
	}

	rkData.SetCurrentData( rkData.GetCurrentData() + iAddValue );
	if( rkData.GetCurrentData() >= (DWORD)iCheckValue )
	{
		if( !IsAbuseCheckQuest() )
			rkData.SetState( QS_ATTAIN );
		SendAttainData( rkData );
		rkData.SetCurrentData( iCheckValue );
	}
	else if( m_bDirectSync )
	{
		SendAttainData( rkData );
	}
	else if( !m_bDirectSync && FRAMEGETTIME() - m_dwDirectSyncSafeTime > 600000 )
	{
		// ��� ������ ���ص� �ּ� 10�п� �ѹ��� �ϵ����ؾ��� ��Ʈ�� ������ ���� �ս��� �߻������ʴ´�.
		SendAttainData( rkData );
		m_dwDirectSyncSafeTime = FRAMEGETTIME();
	}

	//ShowTooltip();
	return ( rkData.GetState() == QS_ATTAIN );
}

bool QuestParent::SetAttainProcess( int iSetValue /* = 0 */, int iCheckValue /* = 0  */ )
{
	QuestData &rkData = GetMyInfoQuestData();
	if( rkData.GetMainIndex() == 0 )
	{
		LOG.PrintTimeAndLog( 0, "SetCompleteProcess None Quest Type : %d - %d", GetMainIndex(), GetSubIndex() );		
		return false;
	}
	// Set�� ��쿡�� ������ ���� �̹� ����ǰ� �ִ� �����͸� ����� �� ���Ƿ� 
	// �����Ͱ� �����ϸ� ������ ���� �� �ʿ䰡 ����.
	DWORD dwPrevValueData = rkData.GetValueData();
	{
		rkData.SetCurrentData( iSetValue );
		if( rkData.GetCurrentData() >= (DWORD)iCheckValue )
		{
			if( !IsAbuseCheckQuest() )
				rkData.SetState( QS_ATTAIN );
			SendAttainData( rkData );
			rkData.SetCurrentData( iCheckValue );
		}
		else if( m_bDirectSync )
		{
			if( dwPrevValueData != rkData.GetValueData() )
			{
				SendAttainData( rkData );
			}
		}
		else if( !m_bDirectSync && FRAMEGETTIME() - m_dwDirectSyncSafeTime > 600000 )
		{
			// ��� ������ ���ص� �ּ� 10�п� �ѹ��� �ϵ����ؾ��� ��Ʈ�� ������ ���� �ս��� �߻������ʴ´�.
			if( dwPrevValueData != rkData.GetValueData() )
			{
				SendAttainData( rkData );
			}
			m_dwDirectSyncSafeTime = FRAMEGETTIME();
		}
	}

	//ShowTooltip();
	return ( rkData.GetState() == QS_ATTAIN );
}

void QuestParent::Process( ioPlayStage *pPlayStage )
{

}

void QuestParent::ProcessRewardComplete()
{

}

void QuestParent::ChangeStageInit()
{
	// ��� ����ȭ�� �ƴ� ����Ʈ�� �������� �Ѿ �� ����ȭ ��Ų��.
	if( !m_bDirectSync )
	{
		QuestData &rkData = GetMyInfoQuestData();
		if( rkData.GetMainIndex() == 0 ) return;
		if( rkData.GetState() != QS_PROGRESS ) return;     //�������� ����Ʈ�� ����ȭ
		if( !rkData.IsChangeData() ) return;               //����� ����Ʈ�� ����ȭ

		SendAttainData( rkData );
	}
}

void QuestParent::SetAlive( bool bAlive )
{
	m_bAlive = bAlive;

	if( !IsAlive() )
	{
		// �ð� ����� ����Ʈ�� ����Ʈ���� ����
		ioQuest *pQuest = g_MyInfo.GetQuest();
		if( pQuest )
		{
			if( pQuest->RemoveQuestData( GetMainIndex(), GetSubIndex() ) )
			{
				g_QuestMgr.AddQuestDelAndFailList( true, this ); 
			}
			else 
			{
				pQuest->RemoveQuestCompleteData( GetMainIndex(), GetSubIndex() );
			}
		}
	}
}

void QuestParent::SendErrorLog( const ioHashString& szErrorLog )
{
	//�α� ���� ����
	char szError[MAX_PATH] = "";
	SafeSprintf( szError, sizeof( szError ), STR(1), g_MyInfo.GetPublicID().c_str(), g_MyInfo.GetPrivateID().c_str(), GetMainIndex(), GetSubIndex(), szErrorLog.c_str() );

	SP2Packet kPacket( LUPK_LOG );
	kPacket << "QuestError";	// �α� ���� Ÿ��
	kPacket << szError;
	kPacket << 702;				// ������ȣ:317
	kPacket << false;			// write db
	P2PNetwork::SendLog(kPacket);
}

//////////////////////////////////////////////////////////////////////////
bool QuestBasic::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return AddAttainProcess( 1 );
}
//////////////////////////////////////////////////////////////////////////
QuestMonsterModeClear::QuestMonsterModeClear()
{
}

QuestMonsterModeClear::~QuestMonsterModeClear()
{

}

bool QuestMonsterModeClear::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestMonsterModeClear::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	if( iValue1 != GetCustomValue( CLEAR_ROUND ) )
		return false;
	if( iValue2 > GetCustomValue( PLAYER_COUNT ) )
		return false;

	return AddAttainProcess( 1, GetProgressCount() );
}

int QuestMonsterModeClear::GetProgressCount()
{
	return GetCustomValue( CLEAR_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestEnterBattlePvPMode::QuestEnterBattlePvPMode()
{
}

QuestEnterBattlePvPMode::~QuestEnterBattlePvPMode()
{

}

bool QuestEnterBattlePvPMode::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;
	return true;
}

bool QuestEnterBattlePvPMode::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return SetAttainProcess( 1 );
}
//////////////////////////////////////////////////////////////////////////
QuestEnterBattlePvPModeKO::QuestEnterBattlePvPModeKO()
{
}

QuestEnterBattlePvPModeKO::~QuestEnterBattlePvPModeKO()
{

}

bool QuestEnterBattlePvPModeKO::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	return true;
}

bool QuestEnterBattlePvPModeKO::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	if( (BOOL)GetCustomValue( BEST_ATTACKER_PASS ) == TRUE )
	{
		if( (BOOL)iValue3 == TRUE )
			return false;
	}

	return AddAttainProcess( iValue1, GetProgressCount() );
}

int QuestEnterBattlePvPModeKO::GetProgressCount()
{
	return GetCustomValue( KILL_COUNT );
}

ioHashString &QuestEnterBattlePvPModeKO::GetProgressResult()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		if( !IsMeProgressQuest() )
			SafeSprintf( szBuf, sizeof( szBuf ), STR(1) );
		else
			SafeSprintf( szBuf, sizeof( szBuf ), STR(2), GetProgressPercentage( kQuestData.GetCurrentData(), GetProgressCount() ) );
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestEnterBattlePvPModeKO::GetProgressResultOnlyNumber()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
		SafeSprintf( szBuf, sizeof( szBuf ), STR(1), GetProgressPercentage( kQuestData.GetCurrentData(), GetProgressCount() ) );
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestEnterBattlePvPModeKO::GetProgressResultOnlyText()
{
	m_szOutProgressResult = "";
	return m_szOutProgressResult;
}

float QuestEnterBattlePvPModeKO::GetProgressGaugeRate()
{
	return QuestParent::GetProgressGaugeRate();
}
//////////////////////////////////////////////////////////////////////////
QuestEnterBattlePvPModeWin::QuestEnterBattlePvPModeWin()
{
}

QuestEnterBattlePvPModeWin::~QuestEnterBattlePvPModeWin()
{

}

bool QuestEnterBattlePvPModeWin::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	return true;
}

bool QuestEnterBattlePvPModeWin::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return AddAttainProcess( iValue1, GetProgressCount() );
}

int QuestEnterBattlePvPModeWin::GetProgressCount()
{
	return GetCustomValue( WIN_COUNT );
}

ioHashString &QuestEnterBattlePvPModeWin::GetProgressResult()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		if( !IsMeProgressQuest() )
			SafeSprintf( szBuf, sizeof( szBuf ), STR(1) );
		else
			SafeSprintf( szBuf, sizeof( szBuf ), STR(2), GetProgressPercentage( kQuestData.GetCurrentData(), GetProgressCount() ) );
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestEnterBattlePvPModeWin::GetProgressResultOnlyNumber()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
		SafeSprintf( szBuf, sizeof( szBuf ), STR(1), GetProgressPercentage( kQuestData.GetCurrentData(), GetProgressCount() ) );
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestEnterBattlePvPModeWin::GetProgressResultOnlyText()
{
	m_szOutProgressResult = "";
	return m_szOutProgressResult;
}

float QuestEnterBattlePvPModeWin::GetProgressGaugeRate()
{
	return QuestParent::GetProgressGaugeRate();
}
//////////////////////////////////////////////////////////////////////////
QuestPvEMonsterKill::QuestPvEMonsterKill()
{
}

QuestPvEMonsterKill::~QuestPvEMonsterKill()
{

}

bool QuestPvEMonsterKill::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestPvEMonsterKill::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	// �������� Ȯ��
	if( !COMPARE( iValue1, GetCustomValue( LIMIT_MIN_FLOOR ), GetCustomValue( LIMIT_MAX_FLOOR ) + 1 ) )
		return false;

	// KO��Ų �뺴 Ÿ�� Ȯ��
	int iSoldierCode = GetCustomValue( KILL_SOLDIER_CODE );
	if( iSoldierCode != 0 )
	{
		if( iSoldierCode != iValue2 )
			return false;
	}

	// KO���� ���� Ÿ��
	int iMaxMonsterCount = GetCustomValue( MAX_MONSTER_CODE );
	if( iMaxMonsterCount == 0 ) // ī��Ʈ�� 0�̸� ������ ����
	{	
		return AddAttainProcess( 1, GetProgressCount() );
	}
	else
	{
		for(int i = 0;i < iMaxMonsterCount;i++)
		{
			if( iValue3 == GetCustomValue( MONSTER_CODE + i ) )
				return AddAttainProcess( 1, GetProgressCount() );
		}
	}

	return false;
}

int QuestPvEMonsterKill::GetProgressCount()
{
	return GetCustomValue( KILL_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestGradeUP::QuestGradeUP()
{
}

QuestGradeUP::~QuestGradeUP()
{
}

bool QuestGradeUP::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestGradeUP::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return SetAttainProcess( g_MyInfo.GetGradeLevel(), GetProgressCount() );
}

bool QuestGradeUP::IsCheckAlreadyAttain()
{
	return SetAttainProcess( g_MyInfo.GetGradeLevel(), GetProgressCount() );
}

int QuestGradeUP::GetProgressCount()
{
	return GetCustomValue( COMPLETE_GRADE );
}

ioHashString &QuestGradeUP::GetProgressResult()
{
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		if( !IsMeProgressQuest() )
			SafeSprintf( szBuf, sizeof( szBuf ), STR(1) );
		else
		{
			// ��� �ۼ�Ʈ ǥ�� - 
			if( abs( GetProgressCount() - g_MyInfo.GetGradeLevel() ) == 1 )
			{
				if( !g_MyInfo.IsGeneralGrade() )
				{
					SafeSprintf( szBuf, sizeof( szBuf ), STR(2), g_MyInfo.GetGradeExpRate() );
				}
			}
			else
				SafeSprintf( szBuf, sizeof( szBuf ), STR(3) );
		}
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestGradeUP::GetProgressResultOnlyNumber()
{
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		// ��� �ۼ�Ʈ ǥ�� - 
		if( abs( GetProgressCount() - g_MyInfo.GetGradeLevel() ) == 1 )
			SafeSprintf( szBuf, sizeof( szBuf ), STR(1), g_MyInfo.GetGradeExpRate() );
		else
			SafeSprintf( szBuf, sizeof( szBuf ), STR(2) );
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestGradeUP::GetProgressResultOnlyText()
{
	m_szOutProgressResult = "";
	return m_szOutProgressResult;
}

float QuestGradeUP::GetProgressGaugeRate()
{
	if( !IsMeProgressQuest() )
		return FLOAT1;

	// ��� �ۼ�Ʈ ǥ�� - 
	if( abs( GetProgressCount() - g_MyInfo.GetGradeLevel() ) == 1 )
		return (float)g_MyInfo.GetGradeExpRate() / FLOAT100;
	return 0.0f;
}
//////////////////////////////////////////////////////////////////////////
QuestTimeGrowthTry::QuestTimeGrowthTry()
{
}

QuestTimeGrowthTry::~QuestTimeGrowthTry()
{
}

bool QuestTimeGrowthTry::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestTimeGrowthTry::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return SetAttainProcess( 1 );
}
//////////////////////////////////////////////////////////////////////////
QuestTimeGrowthSuccess::QuestTimeGrowthSuccess()
{
}

QuestTimeGrowthSuccess::~QuestTimeGrowthSuccess()
{
}

bool QuestTimeGrowthSuccess::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	return true;
}

bool QuestTimeGrowthSuccess::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return SetAttainProcess( iValue1, GetCustomValue( GROWTH_UP_LEVEL ) );
}
//////////////////////////////////////////////////////////////////////////
QuestPesoGrowthTry::QuestPesoGrowthTry()
{
}

QuestPesoGrowthTry::~QuestPesoGrowthTry()
{
}

bool QuestPesoGrowthTry::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestPesoGrowthTry::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return SetAttainProcess( 1 );
}
//////////////////////////////////////////////////////////////////////////
QuestFishingTry::QuestFishingTry()
{
}

QuestFishingTry::~QuestFishingTry()
{

}

bool QuestFishingTry::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;
	return true;
}

bool QuestFishingTry::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return SetAttainProcess( 1 );
}
//////////////////////////////////////////////////////////////////////////
QuestFishingSuccess::QuestFishingSuccess()
{
}

QuestFishingSuccess::~QuestFishingSuccess()
{

}

bool QuestFishingSuccess::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ���� ���� üũ
	if( g_MyInfo.GetFishingLevel() < GetCustomValue( LIMIT_FISHING_LEVEL ) )
		return false;
	return true;
}

bool QuestFishingSuccess::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;
	
	return AddAttainProcess( 1, GetProgressCount() );
}

int QuestFishingSuccess::GetProgressCount()
{
	return GetCustomValue( FISHING_SUCCESS_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestFishingFailed::QuestFishingFailed()
{
}

QuestFishingFailed::~QuestFishingFailed()
{

}

bool QuestFishingFailed::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ���� ���� üũ
	if( g_MyInfo.GetFishingLevel() < GetCustomValue( LIMIT_FISHING_LEVEL ) )
		return false;
	return true;
}

bool QuestFishingFailed::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return AddAttainProcess( 1, GetProgressCount() );
}

int QuestFishingFailed::GetProgressCount()
{
	return GetCustomValue( FISHING_FAIELD_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestFishingLevelUP::QuestFishingLevelUP()
{
}

QuestFishingLevelUP::~QuestFishingLevelUP()
{

}

bool QuestFishingLevelUP::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ���� ���� üũ
	if( g_MyInfo.GetFishingLevel() < GetCustomValue( LIMIT_FISHING_LEVEL ) )
		return false;

	return true;
}

bool QuestFishingLevelUP::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return SetAttainProcess( iValue1, GetProgressCount() );
}

bool QuestFishingLevelUP::IsCheckAlreadyAttain()
{
	return SetAttainProcess( g_MyInfo.GetFishingLevel(), GetProgressCount() );
}

int QuestFishingLevelUP::GetProgressCount()
{
	return GetCustomValue( CHECK_FISHING_LEVEL );
}	

ioHashString &QuestFishingLevelUP::GetProgressResult()
{
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		if( !IsMeProgressQuest() )
			SafeSprintf( szBuf, sizeof( szBuf ), STR(1) );
		else
		{
			SafeSprintf( szBuf, sizeof( szBuf ), STR(2), GetProgressPercentage( max( 0, g_MyInfo.GetFishingLevel() - GetCustomValue( MINUS_FISHING_LEVEL ) ), 
				                                           max( 0, GetProgressCount() - GetCustomValue( MINUS_FISHING_LEVEL ) ) ) );
		}
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestFishingLevelUP::GetProgressResultOnlyNumber()
{
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		SafeSprintf( szBuf, sizeof( szBuf ), STR(1), GetProgressPercentage( max( 0, g_MyInfo.GetFishingLevel() - GetCustomValue( MINUS_FISHING_LEVEL ) ), 
													   max( 0, GetProgressCount() - GetCustomValue( MINUS_FISHING_LEVEL ) ) ) );
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestFishingLevelUP::GetProgressResultOnlyText()
{
	m_szOutProgressResult = "";
	return m_szOutProgressResult;
}

float QuestFishingLevelUP::GetProgressGaugeRate()
{
	if( !IsMeProgressQuest() )
		return FLOAT1;
	int iCurrent = max( 0, g_MyInfo.GetFishingLevel() - GetCustomValue( MINUS_FISHING_LEVEL ) );
	int iMax = max( 0, GetProgressCount() - GetCustomValue( MINUS_FISHING_LEVEL ) );
	if( iMax == 0 ) return 0.0f;

	return (float)iCurrent / (float)iMax;
}
//////////////////////////////////////////////////////////////////////////
QuestFishingSellPeso::QuestFishingSellPeso()
{
}

QuestFishingSellPeso::~QuestFishingSellPeso()
{
}

bool QuestFishingSellPeso::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ���� ���� üũ
	if( g_MyInfo.GetFishingLevel() < GetCustomValue( LIMIT_FISHING_LEVEL ) )
		return false;

	return true;
}

bool QuestFishingSellPeso::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	if( iValue1 < GetCustomValue( SELL_FISH_LIMIT_PESO ) )
		return false;

    return AddAttainProcess( 1, GetProgressCount() );
}

int QuestFishingSellPeso::GetProgressCount()
{
	return GetCustomValue( COMPLETE_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestFishingSuccessItem::QuestFishingSuccessItem()
{
}

QuestFishingSuccessItem::~QuestFishingSuccessItem()
{

}

bool QuestFishingSuccessItem::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ���� ���� üũ
	if( g_MyInfo.GetFishingLevel() < GetCustomValue( LIMIT_FISHING_LEVEL ) )
		return false;

	return true;
}

bool QuestFishingSuccessItem::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	if( GetCustomValue( FISHING_ITEM_TYPE ) == iValue1 )
	{
		// ���� ������ ī���� ���� �� �Ϸ� Ȯ��.
		return AddAttainProcess( 1, GetProgressCount() );		
	}
	return false;
}

int QuestFishingSuccessItem::GetProgressCount()
{
	return GetCustomValue( COMPLETE_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestBattlePvPModeAwardAcquire::QuestBattlePvPModeAwardAcquire()
{
}

QuestBattlePvPModeAwardAcquire::~QuestBattlePvPModeAwardAcquire()
{

}

bool QuestBattlePvPModeAwardAcquire::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestBattlePvPModeAwardAcquire::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	if( iValue1 >= GetCustomValue( AWARD_COUNT ) )
	{
		return SetAttainProcess( iValue1 );
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
QuestSoldierPractice::QuestSoldierPractice()
{
}

QuestSoldierPractice::~QuestSoldierPractice()
{

}

bool QuestSoldierPractice::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	if( iValue1 != GetCustomValue( SOLDIER_CODE ) )
		return false;

	return true;
}

bool QuestSoldierPractice::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	if( iValue1 == GetCustomValue( SOLDIER_CODE ) )
	{
		return SetAttainProcess( 1 );
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
QuestExtraItemReinforceSuccess::QuestExtraItemReinforceSuccess()
{
}

QuestExtraItemReinforceSuccess::~QuestExtraItemReinforceSuccess()
{

}

bool QuestExtraItemReinforceSuccess::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestExtraItemReinforceSuccess::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	int iMaxItemCount = GetCustomValue( MAX_EXTRA_ITEM_CODE );
	if( iMaxItemCount == 0 ) // ī��Ʈ�� 0�̸� ������ ����
	{	
		return SetAttainProcess( iValue1, GetProgressCount() );
	}
	else
	{
		for(int i = 0;i < iMaxItemCount;i++)
		{
			if( iValue2 == GetCustomValue( EXTRA_ITEM_CODE + i ) )
				return SetAttainProcess( iValue1, GetProgressCount() );
		}
	}

	return false;
}

int QuestExtraItemReinforceSuccess::GetProgressCount()
{
	return GetCustomValue( EXTRAITEM_REINFORCE_SUCCESS );
}
//////////////////////////////////////////////////////////////////////////
QuestSoldierLevelUP::QuestSoldierLevelUP()
{
}

QuestSoldierLevelUP::~QuestSoldierLevelUP()
{
}

bool QuestSoldierLevelUP::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	if( iValue1 != GetCustomValue( SOLDIER_CODE ) )
		return false;
	if( iValue2 < GetCustomValue( SOLDIER_LIMIT_LEVEL ) )
		return false;
	return true;
}

bool QuestSoldierLevelUP::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	if( iValue1 != GetCustomValue( SOLDIER_CODE ) )
		return false;

	return SetAttainProcess( g_MyInfo.GetClassLevel( GetCustomValue( SOLDIER_CODE ), true ), GetProgressCount() );
}

bool QuestSoldierLevelUP::IsCheckAlreadyAttain()
{
	return SetAttainProcess( g_MyInfo.GetClassLevel( GetCustomValue( SOLDIER_CODE ), true ), GetProgressCount() );
}

int QuestSoldierLevelUP::GetProgressCount()
{
	return GetCustomValue( SOLDIER_LEVEL );
}

ioHashString &QuestSoldierLevelUP::GetProgressResult()
{
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		if( !IsMeProgressQuest() )
			SafeSprintf( szBuf, sizeof( szBuf ), STR(1) );
		else
		{
			SafeSprintf( szBuf, sizeof( szBuf ), STR(2), GetProgressPercentage( max( 0, g_MyInfo.GetClassLevel( GetCustomValue( SOLDIER_CODE ), true ) - GetCustomValue( SOLDIER_MINUS_LEVEL ) ), 
														   max( 0, GetProgressCount() - GetCustomValue( SOLDIER_MINUS_LEVEL ) ) ) );
		}
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestSoldierLevelUP::GetProgressResultOnlyNumber()
{
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{			
		SafeSprintf( szBuf, sizeof( szBuf ), STR(1), GetProgressPercentage( max( 0, g_MyInfo.GetClassLevel( GetCustomValue( SOLDIER_CODE ), true ) - GetCustomValue( SOLDIER_MINUS_LEVEL ) ), 
													   max( 0, GetProgressCount() - GetCustomValue( SOLDIER_MINUS_LEVEL ) ) ) );
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestSoldierLevelUP::GetProgressResultOnlyText()
{
	m_szOutProgressResult = "";
	return m_szOutProgressResult;
}

float QuestSoldierLevelUP::GetProgressGaugeRate()
{
	if( !IsMeProgressQuest() )
		return FLOAT1;

	int iCurrent = max( 0, g_MyInfo.GetClassLevel( GetCustomValue( SOLDIER_CODE ), true ) - GetCustomValue( SOLDIER_MINUS_LEVEL ) );
	int iMax = max( 0, GetProgressCount() - GetCustomValue( SOLDIER_MINUS_LEVEL ) );
	if( iMax == 0 ) return 0.0f;

	return (float)iCurrent / (float)iMax;
}
//////////////////////////////////////////////////////////////////////////
QuestOpenManual::QuestOpenManual()
{
}

QuestOpenManual::~QuestOpenManual()
{

}

bool QuestOpenManual::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestOpenManual::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	int iCheckManualID = GetCustomValue( CHECK_MANUAL_ID );
	if( iCheckManualID != -1 )
	{
		if( iCheckManualID != iValue1 )
			return false;
	}
	return SetAttainProcess( 1 );
}
//////////////////////////////////////////////////////////////////////////
QuestLoginCount::QuestLoginCount()
{

}

QuestLoginCount::~QuestLoginCount()
{

}

int QuestLoginCount::GetCountLaterDay()
{
	QuestData &rkData = GetMyInfoQuestData();	
	WORD wYear = ioTime::DEFAULT_YEAR + ( rkData.GetCurrentMagicData() / 10000 );
	WORD wMonth= ( rkData.GetCurrentMagicData() % 10000 ) / 100;
	WORD wDay  = ( rkData.GetCurrentMagicData() % 100 );
	if( rkData.GetCurrentMagicData() == 0 )
	{
		return 1;
	}

	struct tm kDate = *localtime( &g_MyInfo.GetServerDate() );	
	WORD wServerYear = kDate.tm_year+1900;
	WORD wServerMonth= kDate.tm_mon+1;
	WORD wServerDay  = kDate.tm_mday;

	time_t tm_prev   = DateHelp::ConvertSecondTime( wYear, wMonth, wDay, 0, 0, 0 );
	time_t tm_current= DateHelp::ConvertSecondTime( wServerYear, wServerMonth, wServerDay, 0, 0, 0 );

	double d_diff = difftime( tm_current, tm_prev );
	int tm_day = d_diff / ( 60 * 60 * 24 );

	return tm_day;
}

bool QuestLoginCount::AddAttainProcess( int iAddValue /* = 0 */, int iCheckValue /* = 0  */ )
{
	QuestData &rkData = GetMyInfoQuestData();

	struct tm kDate = *localtime( &g_MyInfo.GetServerDate() );	
	WORD wServerYear = kDate.tm_year+1900;
	WORD wServerMonth= kDate.tm_mon+1;
	WORD wServerDay  = kDate.tm_mday;

	DWORD dwMagicCode = ( ( wServerYear - ioTime::DEFAULT_YEAR ) * 10000 ) + ( wServerMonth * FLOAT100 ) + wServerDay;
	rkData.SetCurrentMagicData( dwMagicCode );

	return QuestParent::AddAttainProcess( iAddValue, iCheckValue );
}

bool QuestLoginCount::SetAttainProcess( int iSetValue /* = 0 */, int iCheckValue /* = 0  */ )
{
	QuestData &rkData = GetMyInfoQuestData();

	struct tm kDate = *localtime( &g_MyInfo.GetServerDate() );	
	WORD wServerYear = kDate.tm_year+1900;
	WORD wServerMonth= kDate.tm_mon+1;
	WORD wServerDay  = kDate.tm_mday;

	DWORD dwPrevMagicData = rkData.GetCurrentMagicData();
	DWORD dwMagicCode = ( ( wServerYear - ioTime::DEFAULT_YEAR ) * 10000 ) + ( wServerMonth * FLOAT100 ) + wServerDay;
	rkData.SetCurrentMagicData( dwMagicCode );

	
	DWORD dwPrevValueData = rkData.GetValueData();
	bool  bReturn = QuestParent::SetAttainProcess( iSetValue, iCheckValue );	
	if( dwPrevValueData == rkData.GetValueData() &&
		dwPrevMagicData != rkData.GetCurrentMagicData() )
	{
		SendAttainData( rkData );
	}
	return bReturn;
}

bool QuestLoginCount::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestLoginCount::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	int iCountLaterDay = GetCountLaterDay();
	if( iCountLaterDay < GetCustomValue( LIMIT_LOW_DAY ) )
		return false;
	if( iCountLaterDay > GetCustomValue( LIMIT_HIGH_DAY ) )
	{
		SetAttainProcess( 0, GetProgressCount() ); // �ʱ�ȭ
		g_QuestMgr.AddQuestDelAndFailList( false, this );
		return false;
	}
	return AddAttainProcess( 1, GetProgressCount() );
}

int QuestLoginCount::GetProgressCount()
{
	return GetCustomValue( LOGIN_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestEnterPlaza::QuestEnterPlaza()
{
}

QuestEnterPlaza::~QuestEnterPlaza()
{

}

bool QuestEnterPlaza::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestEnterPlaza::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return SetAttainProcess( 1 );
}
//////////////////////////////////////////////////////////////////////////
QuestGetPotion::QuestGetPotion()
{
}

QuestGetPotion::~QuestGetPotion()
{

}

bool QuestGetPotion::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ���� üũ
	if( iValue1 < GetCustomValue( CHECK_ROUND ) )
		return false;

	return true;
}

bool QuestGetPotion::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	if( iValue1 == POTION_CODE1 ||
		iValue1 == POTION_CODE2 ||
		iValue1 == POTION_CODE3 ||
		iValue1 == POTION_CODE4 )
	{
		if( COMPARE( iValue2, GetCustomValue( LIMIT_MIN_FLOOR ), GetCustomValue( LIMIT_MAX_FLOOR ) + 1 ) )
			return AddAttainProcess( 1, GetProgressCount() );
	}
	return false;
}

int QuestGetPotion::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestTutorial::QuestTutorial()
{
}

QuestTutorial::~QuestTutorial()
{

}

bool QuestTutorial::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// Ʃ�丮���� ���� ���� ������ üũ
	if( g_MyInfo.GetUserState() == 0 ) 
		return true;
	if( g_MyInfo.GetUserState() != US_TUTORIAL_CLEAR )
	{
		if( ( g_MyInfo.GetUserState()%100 ) == 0 ) // Ʃ�丮�� ù �ܰ� ( ��) 1200, 100, 2400 )
			return true;
	}
	return false;
}

bool QuestTutorial::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	if( g_MyInfo.IsTutorialUser() )
		return false;

	return SetAttainProcess( 1 );
}

void QuestTutorial::Process( ioPlayStage *pPlayStage )
{
	if( !g_MyInfo.IsTutorialUser()  )
		return;

	if( !g_GUIMgr.IsShow( TUTORIAL_WND ) )
	{
		if( g_MyInfo.GetUserState() == 0 )
			g_MyInfo.SetUserState( TUTORIAL_STATE );

		if( this != g_QuestMgr.FirstQuestOccurList() )
		{
			g_TutorialMgr.InitTutorial( g_MyInfo.GetUserState() );
		}
	}
}
//////////////////////////////////////////////////////////////////////////
QuestPresentReceive::QuestPresentReceive()
{

}

QuestPresentReceive::~QuestPresentReceive()
{

}

bool QuestPresentReceive::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestPresentReceive::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return AddAttainProcess( 1, GetProgressCount() );
}

int QuestPresentReceive::GetProgressCount()
{
	return GetCustomValue( RECV_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestCampJoin::QuestCampJoin()
{
}

QuestCampJoin::~QuestCampJoin()
{

}

bool QuestCampJoin::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestCampJoin::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	if( g_MyInfo.GetUserCampPos() == CAMP_NONE )
		return false;

	return SetAttainProcess( 1 );
}

bool QuestCampJoin::IsCheckAlreadyAttain()
{
	if( g_MyInfo.GetUserCampPos() == CAMP_NONE )
		return false;
	return SetAttainProcess( 1 );
}
//////////////////////////////////////////////////////////////////////////
QuestEnterCampBattle::QuestEnterCampBattle()
{
}

QuestEnterCampBattle::~QuestEnterCampBattle()
{
}

bool QuestEnterCampBattle::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;
	return true;
}

bool QuestEnterCampBattle::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return SetAttainProcess( 1 );
}
//////////////////////////////////////////////////////////////////////////
QuestCampBattleKO::QuestCampBattleKO()
{
}

QuestCampBattleKO::~QuestCampBattleKO()
{

}

bool QuestCampBattleKO::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestCampBattleKO::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	if( (BOOL)GetCustomValue( BEST_ATTACKER_PASS ) == TRUE )
	{
		if( (BOOL)iValue3 == TRUE )
			return false;
	}

	return AddAttainProcess( iValue1, GetProgressCount() );
}

int QuestCampBattleKO::GetProgressCount()
{	
	return GetCustomValue( KILL_COUNT );
}

ioHashString &QuestCampBattleKO::GetProgressResult()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		if( !IsMeProgressQuest() )
			SafeSprintf( szBuf, sizeof( szBuf ), STR(1) );
		else
			SafeSprintf( szBuf, sizeof( szBuf ), STR(2), GetProgressPercentage( kQuestData.GetCurrentData(), GetProgressCount() ) );
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestCampBattleKO::GetProgressResultOnlyNumber()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
		SafeSprintf( szBuf, sizeof( szBuf ), STR(1), GetProgressPercentage( kQuestData.GetCurrentData(), GetProgressCount() ) );
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestCampBattleKO::GetProgressResultOnlyText()
{
	m_szOutProgressResult = "";
	return m_szOutProgressResult;
}

float QuestCampBattleKO::GetProgressGaugeRate()
{
	return QuestParent::GetProgressGaugeRate();
}
//////////////////////////////////////////////////////////////////////////
QuestCampBattleWin::QuestCampBattleWin()
{
}

QuestCampBattleWin::~QuestCampBattleWin()
{

}

bool QuestCampBattleWin::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestCampBattleWin::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return AddAttainProcess( iValue1, GetProgressCount() );
}

int QuestCampBattleWin::GetProgressCount()
{
	return GetCustomValue( WIN_COUNT );
}

ioHashString &QuestCampBattleWin::GetProgressResult()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		if( !IsMeProgressQuest() )
			SafeSprintf( szBuf, sizeof( szBuf ), STR(1) );
		else
			SafeSprintf( szBuf, sizeof( szBuf ), STR(2), GetProgressPercentage( kQuestData.GetCurrentData(), GetProgressCount() ) );
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestCampBattleWin::GetProgressResultOnlyNumber()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
		SafeSprintf( szBuf, sizeof( szBuf ), STR(1), GetProgressPercentage( kQuestData.GetCurrentData(), GetProgressCount() ) );
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestCampBattleWin::GetProgressResultOnlyText()
{
	m_szOutProgressResult = "";
	return m_szOutProgressResult;
}

float QuestCampBattleWin::GetProgressGaugeRate()
{
	return QuestParent::GetProgressGaugeRate();
}
//////////////////////////////////////////////////////////////////////////
QuestCampSeasonReward::QuestCampSeasonReward()
{

}

QuestCampSeasonReward::~QuestCampSeasonReward()
{

}

bool QuestCampSeasonReward::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestCampSeasonReward::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
	{
		return false;
	}

	const ioTaskTournamentReward* const pReward = dynamic_cast<const ioTaskTournamentReward* const>( g_RserveTask.GetTask( (ReservedTaskTypes)iValue1, (ReservedTaskWhens)iValue2 ) );
	if( !pReward )
		return false;

	// ���� üũ
	int iCustomValue = GetCustomValue( CHECK_WIN_LOSE );
	if( iCustomValue == WIN_CHECK )
	{
		if( pReward->iMyCampPos == CAMP_BLUE )
		{
			if( pReward->iWinCampPos != CAMP_BLUE )
			{
				return false;
			}
		}
		else
		{
			if( pReward->iWinCampPos != CAMP_RED )
			{
				return false;
			}
		}
	}
	else if( iCustomValue == LOSE_CHECK )
	{
		if( pReward->iMyCampPos == CAMP_BLUE )
		{
			if( pReward->iWinCampPos == CAMP_BLUE || pReward->iWinCampPos == CAMP_NONE )
			{
				return false;
			}
		}
		else
		{
			if( pReward->iWinCampPos == CAMP_RED || pReward->iWinCampPos == CAMP_NONE )
			{
				return false;
			}
		}
	}

	// ��ȸ ����Ʈ üũ
	iCustomValue = GetCustomValue( CHECK_CAMP_POINT );
	if( iCustomValue != -1 )
	{
		if( pReward->iLadderPoint < iCustomValue )
		{
			return false;
		}
	}

	// ���� ���� üũ
	iCustomValue = GetCustomValue( CHECK_CAMP_RANK );
	if( iCustomValue != -1 )
	{
		if( pReward->iLadderRank > iCustomValue )
		{
			return false;
		}
	}

	// ���� ���ʽ� üũ
	iCustomValue = GetCustomValue( CHECK_CAMP_BONUS );
	if( iCustomValue != -1 )
	{
		if( pReward->iLadderBonusPeso < iCustomValue )
		{
			return false;
		}
	}

	return SetAttainProcess( 1 );
}
//////////////////////////////////////////////////////////////////////////
QuestAwardAcquire::QuestAwardAcquire()
{
}

QuestAwardAcquire::~QuestAwardAcquire()
{

}

bool QuestAwardAcquire::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;
	return true;
}

bool QuestAwardAcquire::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	int iCustomValue = GetCustomValue( CHECK_AWARD_TYPE );
	if( iCustomValue != -1 )
	{
		if( iValue1 != iCustomValue )
			return false;
	}
	return AddAttainProcess( iValue2, GetProgressCount() );
}

int QuestAwardAcquire::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}

ioHashString &QuestAwardAcquire::GetProgressResult()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		if( !IsMeProgressQuest() )
			SafeSprintf( szBuf, sizeof( szBuf ), STR(1) );
		else
		{
			SafeSprintf( szBuf, sizeof( szBuf ), STR(2), GetProgressPercentage( kQuestData.GetCurrentData(), GetProgressCount() ) );
		}
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestAwardAcquire::GetProgressResultOnlyNumber()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		SafeSprintf( szBuf, sizeof( szBuf ), STR(1), GetProgressPercentage( kQuestData.GetCurrentData(), GetProgressCount() ) );
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestAwardAcquire::GetProgressResultOnlyText()
{
	m_szOutProgressResult = "";
	return m_szOutProgressResult;
}

float QuestAwardAcquire::GetProgressGaugeRate()
{
	return QuestParent::GetProgressGaugeRate();
}
//////////////////////////////////////////////////////////////////////////
QuestPrisonerDrop::QuestPrisonerDrop()
{
}

QuestPrisonerDrop::~QuestPrisonerDrop()
{

}

bool QuestPrisonerDrop::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;
	return true;
}

bool QuestPrisonerDrop::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return AddAttainProcess( iValue1, GetProgressCount() );
}

int QuestPrisonerDrop::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestPrisonerSave::QuestPrisonerSave()
{
}

QuestPrisonerSave::~QuestPrisonerSave()
{

}

bool QuestPrisonerSave::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;
	return true;
}

bool QuestPrisonerSave::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return AddAttainProcess( iValue1, GetProgressCount() );
}

int QuestPrisonerSave::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestBattleLevel::QuestBattleLevel()
{
}

QuestBattleLevel::~QuestBattleLevel()
{

}

bool QuestBattleLevel::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// �������� üũ
	if( g_MyInfo.GetPartyLevel() < GetCustomValue( LIMIT_BATTLE_LEVEL ) )
		return false;
	return true;
}

bool QuestBattleLevel::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return SetAttainProcess( g_MyInfo.GetPartyLevel(), GetProgressCount() );
}

bool QuestBattleLevel::IsCheckAlreadyAttain()
{
	return SetAttainProcess( g_MyInfo.GetPartyLevel(), GetProgressCount() );
}

int QuestBattleLevel::GetProgressCount()
{
	return GetCustomValue( CHECK_BATTLE_LEVEL );
}

ioHashString &QuestBattleLevel::GetProgressResult()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		if( !IsMeProgressQuest() )
			SafeSprintf( szBuf, sizeof( szBuf ), STR(1) );
		else
		{
			SafeSprintf( szBuf, sizeof( szBuf ), STR(2), GetProgressPercentage( max( 0, g_MyInfo.GetPartyLevel() - GetCustomValue( MINUS_BATTLE_LEVEL ) ), 
														   max( 0, GetProgressCount() - GetCustomValue( MINUS_BATTLE_LEVEL ) ) ) );
		}
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestBattleLevel::GetProgressResultOnlyNumber()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		SafeSprintf( szBuf, sizeof( szBuf ), STR(1), GetProgressPercentage( max( 0, g_MyInfo.GetPartyLevel() - GetCustomValue( MINUS_BATTLE_LEVEL ) ), 
													   max( 0, GetProgressCount() - GetCustomValue( MINUS_BATTLE_LEVEL ) ) ) );
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestBattleLevel::GetProgressResultOnlyText()
{
	m_szOutProgressResult = "";
	return m_szOutProgressResult;
}

float QuestBattleLevel::GetProgressGaugeRate()
{
	if( !IsMeProgressQuest() )
		return FLOAT1;
	int iCurrent = max( 0, g_MyInfo.GetPartyLevel() - GetCustomValue( MINUS_BATTLE_LEVEL ) );
	int iMax = max( 0, GetProgressCount() - GetCustomValue( MINUS_BATTLE_LEVEL ) );
	if( iMax == 0 ) return 0.0f;

	return (float)iCurrent / (float)iMax;
}
//////////////////////////////////////////////////////////////////////////
QuestAwardLevel::QuestAwardLevel()
{
}

QuestAwardLevel::~QuestAwardLevel()
{

}

bool QuestAwardLevel::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ���󷹺� üũ
	if( g_MyInfo.GetAwardLevel() < GetCustomValue( LIMIT_AWARD_LEVEL ) )
		return false;
	return true;
}

bool QuestAwardLevel::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return SetAttainProcess( g_MyInfo.GetAwardLevel(), GetProgressCount() );
}

bool QuestAwardLevel::IsCheckAlreadyAttain()
{
	return SetAttainProcess( g_MyInfo.GetAwardLevel(), GetProgressCount() );
}

int QuestAwardLevel::GetProgressCount()
{
	return GetCustomValue( CHECK_AWARD_LEVEL );
}

ioHashString &QuestAwardLevel::GetProgressResult()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		if( !IsMeProgressQuest() )
			SafeSprintf( szBuf, sizeof( szBuf ), STR(1) );
		else
		{
			SafeSprintf( szBuf, sizeof( szBuf ), STR(2), GetProgressPercentage( max( 0, g_MyInfo.GetAwardLevel() - GetCustomValue( MINUS_AWARD_LEVEL ) ), 
														   max( 0, GetProgressCount() - GetCustomValue( MINUS_AWARD_LEVEL ) ) ) );
		}
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestAwardLevel::GetProgressResultOnlyNumber()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		SafeSprintf( szBuf, sizeof( szBuf ), STR(1), GetProgressPercentage( max( 0, g_MyInfo.GetAwardLevel() - GetCustomValue( MINUS_AWARD_LEVEL ) ), 
													   max( 0, GetProgressCount() - GetCustomValue( MINUS_AWARD_LEVEL ) ) ) );
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestAwardLevel::GetProgressResultOnlyText()
{
	m_szOutProgressResult = "";
	return m_szOutProgressResult;
}

float QuestAwardLevel::GetProgressGaugeRate()
{
	if( !IsMeProgressQuest() )
		return FLOAT1;
	int iCurrent = max( 0, g_MyInfo.GetAwardLevel() - GetCustomValue( MINUS_AWARD_LEVEL ) );
	int iMax = max( 0, GetProgressCount() - GetCustomValue( MINUS_AWARD_LEVEL ) );
	if( iMax == 0 ) return 0.0f;

	return (float)iCurrent / (float)iMax;
}
//////////////////////////////////////////////////////////////////////////
QuestContribute::QuestContribute()
{
}

QuestContribute::~QuestContribute()
{

}

bool QuestContribute::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;
	return true;
}

bool QuestContribute::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	if( iValue1 < GetCustomValue( LIMIT_CONTRIBUTE ) )
		return false;

	return AddAttainProcess( 1, GetProgressCount() );
}

int QuestContribute::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestDropKill::QuestDropKill()
{
}

QuestDropKill::~QuestDropKill()
{

}

bool QuestDropKill::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;
	return true;
}

bool QuestDropKill::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	if( (BOOL)GetCustomValue( BEST_ATTACKER_PASS ) == TRUE )
	{
		if( (BOOL)iValue3 == TRUE )
			return false;
	}

	return AddAttainProcess( iValue1, GetProgressCount() );
}

int QuestDropKill::GetProgressCount()
{
	return GetCustomValue( KILL_COUNT );
}

ioHashString &QuestDropKill::GetProgressResult()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		if( !IsMeProgressQuest() )
			SafeSprintf( szBuf, sizeof( szBuf ), STR(1) );
		else
			SafeSprintf( szBuf, sizeof( szBuf ), STR(2), GetProgressPercentage( kQuestData.GetCurrentData(), GetProgressCount() ) );
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestDropKill::GetProgressResultOnlyNumber()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
		SafeSprintf( szBuf, sizeof( szBuf ), STR(1), GetProgressPercentage( kQuestData.GetCurrentData(), GetProgressCount() ) );
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestDropKill::GetProgressResultOnlyText()
{
	m_szOutProgressResult = "";
	return m_szOutProgressResult;
}

float QuestDropKill::GetProgressGaugeRate()
{
	return QuestParent::GetProgressGaugeRate();
}
//////////////////////////////////////////////////////////////////////////
QuestMultiKill::QuestMultiKill()
{
}

QuestMultiKill::~QuestMultiKill()
{

}

bool QuestMultiKill::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestMultiKill::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	if( (BOOL)GetCustomValue( BEST_ATTACKER_PASS ) == TRUE )
	{
		if( (BOOL)iValue3 == TRUE )
			return false;
	}

	return AddAttainProcess( iValue1, GetProgressCount() );
}

int QuestMultiKill::GetProgressCount()
{
	return GetCustomValue( KILL_COUNT );
}

ioHashString &QuestMultiKill::GetProgressResult()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		if( !IsMeProgressQuest() )
			SafeSprintf( szBuf, sizeof( szBuf ), STR(1) );
		else
			SafeSprintf( szBuf, sizeof( szBuf ), STR(2), GetProgressPercentage( kQuestData.GetCurrentData(), GetProgressCount() ) );
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestMultiKill::GetProgressResultOnlyNumber()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
		SafeSprintf( szBuf, sizeof( szBuf ), STR(1), GetProgressPercentage( kQuestData.GetCurrentData(), GetProgressCount() ) );
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestMultiKill::GetProgressResultOnlyText()
{	
	m_szOutProgressResult = "";
	return m_szOutProgressResult;
}

float QuestMultiKill::GetProgressGaugeRate()
{
	return QuestParent::GetProgressGaugeRate();
}

void QuestMultiKill::ChangeStageInit()
{
	if( IsMeProgressQuest() )
	{
		// ���������� ����Ǹ� ���� �ʱ�ȭ ��Ų��.
		SetAttainProcess( 0, GetProgressCount() );

		// ��� ����ȭ �ƴ� ����Ʈ ����ȭ
		QuestParent::ChangeStageInit();
	}
}
//////////////////////////////////////////////////////////////////////////
QuestPvPConsecution::QuestPvPConsecution()
{
}

QuestPvPConsecution::~QuestPvPConsecution()
{
}

bool QuestPvPConsecution::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestPvPConsecution::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return SetAttainProcess( iValue3, GetProgressCount() );
}

int QuestPvPConsecution::GetProgressCount()
{
	return GetCustomValue( CONSECUTION_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestCampConsecution::QuestCampConsecution()
{
}

QuestCampConsecution::~QuestCampConsecution()
{
}

bool QuestCampConsecution::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestCampConsecution::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return SetAttainProcess( iValue3, GetProgressCount() );
}

int QuestCampConsecution::GetProgressCount()
{
	return GetCustomValue( CONSECUTION_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestEtcItemUse::QuestEtcItemUse()
{
}

QuestEtcItemUse::~QuestEtcItemUse()
{
}

bool QuestEtcItemUse::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestEtcItemUse::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	int iMaxItemCount = GetCustomValue( MAX_ETC_ITEM );
	for(int i = 0;i < iMaxItemCount;i++)
	{
		if( iValue1 == GetCustomValue( ETC_ITEM_CODE + i ) )
			return AddAttainProcess( 1, GetProgressCount() );
	}
	return false;
}

int QuestEtcItemUse::GetProgressCount()
{
	return GetCustomValue( USE_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestEtcItemCnt::QuestEtcItemCnt()
{
}

QuestEtcItemCnt::~QuestEtcItemCnt()
{
}

bool QuestEtcItemCnt::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestEtcItemCnt::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	ioUserEtcItem *pUserEtcItem =  g_MyInfo.GetUserEtcItem();
	if( !pUserEtcItem ) return false;
	
	ioUserEtcItem::ETCITEMSLOT kSlot;
	pUserEtcItem->GetEtcItem( GetCustomValue( ETC_ITEM_CODE ), kSlot );

	return SetAttainProcess( kSlot.GetUse(), GetProgressCount() );
}

bool QuestEtcItemCnt::IsCheckAlreadyAttain()
{
	ioUserEtcItem *pUserEtcItem =  g_MyInfo.GetUserEtcItem();
	if( !pUserEtcItem ) return false;

	ioUserEtcItem::ETCITEMSLOT kSlot;
	pUserEtcItem->GetEtcItem( GetCustomValue( ETC_ITEM_CODE ), kSlot );

	return SetAttainProcess( kSlot.GetUse(), GetProgressCount() );
}

int QuestEtcItemCnt::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}

void QuestEtcItemCnt::ProcessRewardComplete()
{
	if( GetCustomValue( BOOLEAN_ETC_ITEM_DELETE ) == 1 )
	{
		// ���� ������ ���� 
		ioUserEtcItem *pUserEtcItem =  g_MyInfo.GetUserEtcItem();
		if( pUserEtcItem )
		{
			ioUserEtcItem::ETCITEMSLOT kSlot;
			pUserEtcItem->GetEtcItem( GetCustomValue( ETC_ITEM_CODE ), kSlot );
			
			if( !kSlot.IsUse() ) 
			{
				return;      // ��� Ÿ���� �ƴϴ�.
			}

			kSlot.AddUse( -( min( kSlot.GetUse(), GetProgressCount() ) ) );
			if( kSlot.GetUse() > 0 )
			{	
				pUserEtcItem->AddEtcItem( kSlot );
			}
			else
			{
				pUserEtcItem->DeleteEtcItem( kSlot.m_iType );
			}
		}		
	}
}
//////////////////////////////////////////////////////////////////////////
QuestRequestFriend::QuestRequestFriend()
{
}

QuestRequestFriend::~QuestRequestFriend()
{
}

bool QuestRequestFriend::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestRequestFriend::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return SetAttainProcess( 1 );
}
//////////////////////////////////////////////////////////////////////////
QuestMakeFriends::QuestMakeFriends()
{
}

QuestMakeFriends::~QuestMakeFriends()
{
}

bool QuestMakeFriends::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;
	
	// ���� ģ���� üũ
	if( g_FriendManager.GetNodeSize() < GetCustomValue( LIMIT_FRIEND_COUNT ) )
		return false;

	return true;
}

bool QuestMakeFriends::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return SetAttainProcess( g_FriendManager.GetNodeSize(), GetProgressCount() );
}

bool QuestMakeFriends::IsCheckAlreadyAttain()
{
	return SetAttainProcess( g_FriendManager.GetNodeSize(), GetProgressCount() );
}

int QuestMakeFriends::GetProgressCount()
{
	return GetCustomValue( FRIEND_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestModePlayTime::QuestModePlayTime()
{
	m_dwProcessTime = 0;
	m_dwAccrueSec   = 0;
	m_bDirectSync   = false;
}

QuestModePlayTime::~QuestModePlayTime()
{
}

bool QuestModePlayTime::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestModePlayTime::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	return false;
}

void QuestModePlayTime::StartProcessTime() 
{
	m_dwProcessTime = FRAMEGETTIME();
	m_dwAccrueSec   = 0;
}

int QuestModePlayTime::GetProgressCount()
{
	return GetCustomValue( CHECK_PLAY_MIN );
}

void QuestModePlayTime::ChangeStageInit()
{
	// ��� ����ȭ �ƴ� ����Ʈ ����ȭ
	QuestParent::ChangeStageInit();
}

void QuestModePlayTime::Process( ioPlayStage *pPlayStage )
{
	if( !pPlayStage ) 
		return;

	// ����Ʈ ���Ἲ üũ
	if( !QuestParent::IsCheckCompleteTerm( GetCompleteValue() ) )
		return;

	// �뺴 üũ
	ioBaseChar *pOwner = pPlayStage->GetOwnerChar();
	if( !pOwner )
		return;

	int iCheckSoldier = GetCustomValue( CHECK_SOLDIER );
	if( iCheckSoldier != -1 )
	{
		const CHARACTER &rkChar = pOwner->GetCharacterInfo();
		if( iCheckSoldier != rkChar.m_class_type )
			return;
	}
	
	// ������ ����
	if( pOwner->GetState() == CS_OBSERVER || pOwner->GetState() == CS_VIEW || pOwner->GetState() == CS_LOADING || pOwner->GetState() == CS_READY )
		return;

	// ���� ��� ����
	if( pPlayStage->IsRoundSetEnd() )
		return;

	// �߰� ��� ����
	if( g_GUIMgr.IsShow( ROUND_RESULT_MAIN_WND ) )
		return;	

	// ���� UI �������� ������ ����
	if( g_GUIMgr.IsShow( PARTY_SHAM_BATTLE_WND ) )
		return;

	// �Ϸ�üũ�� ���⼭ �Ѵ�.
	if( FRAMEGETTIME() - m_dwProcessTime < 1000 )      // 1�ʸ��� üũ
		return;	
	
	m_dwProcessTime = FRAMEGETTIME();
	m_dwAccrueSec++;
	if( m_dwAccrueSec >= 60 )    // 1�и��� üũ
	{
		m_dwAccrueSec = 0;
		if( AddAttainProcess( 1, GetProgressCount() ) )
		{
			// �Ϸ� ó��
			g_QuestMgr.AddQuestAttainList( this );
		}
	}
}
//////////////////////////////////////////////////////////////////////////
QuestStoneAttack::QuestStoneAttack()
{
	m_bDirectSync = false;
}

QuestStoneAttack::~QuestStoneAttack()
{
}

bool QuestStoneAttack::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestStoneAttack::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	int iAttackStyle = GetCustomValue( TEAM_STYLE );
	switch( iAttackStyle )
	{
	case SAME_TEAM:
		if( iValue2 != iValue3 )
			return false;
		break;
	case OTHER_TEAM:
		if( iValue2 == iValue3 )
			return false;
		break;
	}

	return AddAttainProcess( iValue1, GetProgressCount() );
}

int QuestStoneAttack::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}

ioHashString &QuestStoneAttack::GetProgressResult()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		if( !IsMeProgressQuest() )
			SafeSprintf( szBuf, sizeof( szBuf ), STR(1) );
		else
			SafeSprintf( szBuf, sizeof( szBuf ), STR(2), GetProgressPercentage( kQuestData.GetCurrentData(), GetProgressCount() ) );
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestStoneAttack::GetProgressResultOnlyNumber()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
		SafeSprintf( szBuf, sizeof( szBuf ), STR(1), GetProgressPercentage( kQuestData.GetCurrentData(), GetProgressCount() ) );
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestStoneAttack::GetProgressResultOnlyText()
{
	m_szOutProgressResult = "";
	return m_szOutProgressResult;
}

float QuestStoneAttack::GetProgressGaugeRate()
{
	return QuestParent::GetProgressGaugeRate();
}
//////////////////////////////////////////////////////////////////////////
QuestKingAttack::QuestKingAttack()
{
	m_bDirectSync = false;
}

QuestKingAttack::~QuestKingAttack()
{
}

bool QuestKingAttack::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestKingAttack::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return AddAttainProcess( 1, GetProgressCount() );
}

int QuestKingAttack::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestCrownHoldTime::QuestCrownHoldTime()
{
	m_bDirectSync   = false;
	m_dwMilliSecond = 0;
}

QuestCrownHoldTime::~QuestCrownHoldTime()
{
}

bool QuestCrownHoldTime::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestCrownHoldTime::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	m_dwMilliSecond += iValue1;         // 500ms ���� ���´�.

	if( m_dwMilliSecond < 1000 )
		return false;

	// �ʴ����� �־���.
	m_dwMilliSecond = 0;	
	return AddAttainProcess( 1, GetProgressCount() );
}

int QuestCrownHoldTime::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestBossModeBecomeBoss::QuestBossModeBecomeBoss()
{
}

QuestBossModeBecomeBoss::~QuestBossModeBecomeBoss()
{
}

bool QuestBossModeBecomeBoss::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestBossModeBecomeBoss::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return AddAttainProcess( 1, GetProgressCount() );
}

int QuestBossModeBecomeBoss::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestBossModeBosswithKill::QuestBossModeBosswithKill()
{
	m_bDirectSync = false;
}

QuestBossModeBosswithKill::~QuestBossModeBosswithKill()
{
}

bool QuestBossModeBosswithKill::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestBossModeBosswithKill::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return AddAttainProcess( 1, GetProgressCount() );
}

int QuestBossModeBosswithKill::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}

void QuestBossModeBosswithKill::ChangeStageInit()
{
	if( IsMeProgressQuest() )
	{
		// ���������� ����Ǹ� ���� �ʱ�ȭ ��Ų��.
		if( GetCustomValue( DEATH_INIT ) == 0 )
		{
			SetAttainProcess( 0, GetProgressCount() );
		}

		// ��� ����ȭ �ƴ� ����Ʈ ����ȭ
		QuestParent::ChangeStageInit();
	}
}

void QuestBossModeBosswithKill::Process( ioPlayStage *pPlayStage )
{
	if( !pPlayStage ) 
		return;

	if( GetPerformType() == QP_EVENT )
	{
		if( !IsAlive() ) return;
	}

	if( IsPCRoomStyle() )
	{
		if( !g_MyInfo.GetPCRoomAuthority() ) return;
		if( g_MyInfo.IsUserEvent() )
			return;
	}

	

	if( pPlayStage->GetModeType() != MT_BOSS )
		return;

	if( GetCustomValue( DEATH_INIT ) != 0 )
		return;

	if( !IsMeProgressQuest() )
		return;

	ioBaseChar *pOwner = pPlayStage->GetOwnerChar();
	if( !pOwner )
		return;

	// ������ ����
	if( pOwner->GetState() == CS_OBSERVER || pOwner->GetState() == CS_VIEW || pOwner->GetState() == CS_LOADING || pOwner->GetState() == CS_READY )
		return;

	// ���� ��� ����
	if( pPlayStage->IsRoundSetEnd() )
		return;

	// �߰� ��� ����
	if( g_GUIMgr.IsShow( ROUND_RESULT_MAIN_WND ) )
		return;	

	QuestData &rkData = GetMyInfoQuestData();
	if( rkData.GetMainIndex() == 0 )
		return;

	// ������ �ƴϸ� �ʱ�ȭ�Ѵ�.
	if( !pOwner->IsBoss() )
	{
		if( rkData.GetCurrentData() != 0 )
			SetAttainProcess( 0, GetProgressCount() );
	}
}
//////////////////////////////////////////////////////////////////////////
QuestMortmainSoldierCount::QuestMortmainSoldierCount()
{
}

QuestMortmainSoldierCount::~QuestMortmainSoldierCount()
{
}

bool QuestMortmainSoldierCount::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	// �������� ���� �뺴 ��
	if( g_MyInfo.GetMortmainCharCount() < GetCustomValue( LIMIT_MORTMAIN_SOLDIER ) )
		return false;

	return true;
}

bool QuestMortmainSoldierCount::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return SetAttainProcess( g_MyInfo.GetMortmainCharCount(), GetProgressCount() );
}

bool QuestMortmainSoldierCount::IsCheckAlreadyAttain()
{
	return SetAttainProcess( g_MyInfo.GetMortmainCharCount(), GetProgressCount() );
}

int QuestMortmainSoldierCount::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestHitCount::QuestHitCount()
{
	m_bDirectSync = false;
}

QuestHitCount::~QuestHitCount()
{
}

bool QuestHitCount::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestHitCount::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return AddAttainProcess( 1, GetProgressCount() );
}

int QuestHitCount::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestDefenceCount::QuestDefenceCount()
{
	m_bDirectSync = false;
}

QuestDefenceCount::~QuestDefenceCount()
{
}

bool QuestDefenceCount::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestDefenceCount::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return AddAttainProcess( 1, GetProgressCount() );
}

int QuestDefenceCount::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestPickItem::QuestPickItem()
{
	m_bDirectSync = false;
}

QuestPickItem::~QuestPickItem()
{
}

bool QuestPickItem::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestPickItem::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	int iMaxItemCount = GetCustomValue( MAX_ITEM_CODE );
	if( iMaxItemCount == 0 ) // ī��Ʈ�� 0�̸� ������ ����
	{	
		return AddAttainProcess( 1, GetProgressCount() );
	}
	else
	{
		for(int i = 0;i < iMaxItemCount;i++)
		{
			if( iValue1 == GetCustomValue( PICK_ITEM_CODE + i ) )
				return AddAttainProcess( 1, GetProgressCount() );
		}
	}
	return false;
}

int QuestPickItem::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestCampLevel::QuestCampLevel()
{
}

QuestCampLevel::~QuestCampLevel()
{
}

bool QuestCampLevel::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// �������� üũ
	if( g_MyInfo.GetLadderLevel() < GetCustomValue( LIMIT_CAMP_LEVEL ) )
		return false;
	return true;
}

bool QuestCampLevel::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return SetAttainProcess( g_MyInfo.GetLadderLevel(), GetProgressCount() );
}

bool QuestCampLevel::IsCheckAlreadyAttain()
{
	return SetAttainProcess( g_MyInfo.GetLadderLevel(), GetProgressCount() );
}

int QuestCampLevel::GetProgressCount()
{
	return GetCustomValue( CHECK_CAMP_LEVEL );
}

ioHashString &QuestCampLevel::GetProgressResult()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		if( !IsMeProgressQuest() )
			SafeSprintf( szBuf, sizeof( szBuf ), STR(1) );
		else
		{
			SafeSprintf( szBuf, sizeof( szBuf ), STR(2), GetProgressPercentage( max( 0, g_MyInfo.GetLadderLevel() - GetCustomValue( MINUS_CAMP_LEVEL ) ), 
														   max( 0, GetProgressCount() - GetCustomValue( MINUS_CAMP_LEVEL ) ) ) );
		}
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestCampLevel::GetProgressResultOnlyNumber()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		SafeSprintf( szBuf, sizeof( szBuf ), STR(1), GetProgressPercentage( max( 0, g_MyInfo.GetLadderLevel() - GetCustomValue( MINUS_CAMP_LEVEL ) ), 
													   max( 0, GetProgressCount() - GetCustomValue( MINUS_CAMP_LEVEL ) ) ) );
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestCampLevel::GetProgressResultOnlyText()
{
	m_szOutProgressResult = "";
	return m_szOutProgressResult;
}

float QuestCampLevel::GetProgressGaugeRate()
{
	if( !IsMeProgressQuest() )
		return FLOAT1;
	int iCurrent = max( 0, g_MyInfo.GetLadderLevel() - GetCustomValue( MINUS_CAMP_LEVEL ) );
	int iMax = max( 0, GetProgressCount() - GetCustomValue( MINUS_CAMP_LEVEL ) );
	if( iMax == 0 ) return 0.0f;

	return (float)iCurrent / (float)iMax;
}
//////////////////////////////////////////////////////////////////////////
QuestBuyItem::QuestBuyItem()
{
}

QuestBuyItem::~QuestBuyItem()
{

}

bool QuestBuyItem::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestBuyItem::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	int iMaxItemCount = GetCustomValue( MAX_ITEM_CODE );
	if( iMaxItemCount == 0 ) // ī��Ʈ�� 0�̸� ������ ����
	{	
		return AddAttainProcess( 1, GetProgressCount() );
	}
	else
	{
		for(int i = 0;i < iMaxItemCount;i++)
		{
			if( iValue1 == GetCustomValue( ITEM_CODE + i ) )
				return AddAttainProcess( 1, GetProgressCount() );
		}
	}
	return false;
}

int QuestBuyItem::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestMaxFriendSlot::QuestMaxFriendSlot()
{
}

QuestMaxFriendSlot::~QuestMaxFriendSlot()
{
}

bool QuestMaxFriendSlot::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestMaxFriendSlot::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return SetAttainProcess( g_FriendManager.GetSlotSize(), GetProgressCount() );
}

bool QuestMaxFriendSlot::IsCheckAlreadyAttain()
{
	return SetAttainProcess( g_FriendManager.GetSlotSize(), GetProgressCount() );
}

int QuestMaxFriendSlot::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestRealTimeCount::QuestRealTimeCount()
{
	m_dwProcessTime = 0;
	m_dwAccrueSec   = 0;
}

QuestRealTimeCount::~QuestRealTimeCount()
{
}

bool QuestRealTimeCount::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( !COMPARE( g_MyInfo.GetGradeLevel(), GetCustomValue( LIMIT_MIN_GRADE ), GetCustomValue( LIMIT_MAX_GRADE ) + 1 ) )
		return false;

	return true;
}

bool QuestRealTimeCount::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	// iValue1(Sec)
	return AddAttainProcess( iValue1, GetProgressCount() );
}

void QuestRealTimeCount::StartProcessTime() 
{
	m_dwProcessTime = FRAMEGETTIME();
	m_dwAccrueSec   = 0;
}

int QuestRealTimeCount::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}

ioHashString &QuestRealTimeCount::GetProgressResult()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		if( !IsMeProgressQuest() )
			SafeSprintf( szBuf, sizeof( szBuf ), STR(1) );
		else
		{
			SafeSprintf( szBuf, sizeof( szBuf ), STR(2), GetProgressPercentage( kQuestData.GetCurrentData(), GetProgressCount() ) );
		}
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestRealTimeCount::GetProgressResultOnlyNumber()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		SafeSprintf( szBuf, sizeof( szBuf ), STR(1), GetProgressPercentage( kQuestData.GetCurrentData(), GetProgressCount() ) );
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestRealTimeCount::GetProgressResultOnlyText()
{
	m_szOutProgressResult = "";
	return m_szOutProgressResult;
}

float QuestRealTimeCount::GetProgressGaugeRate()
{
	return QuestParent::GetProgressGaugeRate();
}

void QuestRealTimeCount::Process( ioPlayStage *pPlayStage )
{
	if( !pPlayStage ) 
		return;

	// �Ϸ�üũ�� ���⼭ �Ѵ�.
	if( FRAMEGETTIME() - m_dwProcessTime < 1000 )      // 1�ʸ��� üũ
		return;	
	m_dwProcessTime = FRAMEGETTIME();

	// ����Ʈ ���Ἲ üũ
	if( !QuestParent::IsCheckCompleteTerm( GetCompleteValue() ) )
		return;

	// ������ �ð��� ����.	
	m_dwAccrueSec++;
	if( m_dwAccrueSec >= 60 )    // 1�и��� üũ
	{
		if( AddAttainProcess( m_dwAccrueSec, GetProgressCount() ) )
		{
			// �Ϸ� ó��
			g_QuestMgr.AddQuestAttainList( this );
		}
		m_dwAccrueSec = 0;
	}
}
//////////////////////////////////////////////////////////////////////////
QuestPlayTimeCount::QuestPlayTimeCount()
{
	m_dwProcessTime = 0;
	m_dwAccrueSec   = 0;
	m_bDirectSync   = false;
}

QuestPlayTimeCount::~QuestPlayTimeCount()
{
}

bool QuestPlayTimeCount::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( !COMPARE( g_MyInfo.GetGradeLevel(), GetCustomValue( LIMIT_MIN_GRADE ), GetCustomValue( LIMIT_MAX_GRADE ) + 1 ) )
		return false;

	return true;
}

bool QuestPlayTimeCount::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{	
	return false;
}

void QuestPlayTimeCount::StartProcessTime() 
{
	m_dwProcessTime = FRAMEGETTIME();
	m_dwAccrueSec   = 0;
}

int QuestPlayTimeCount::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}

ioHashString &QuestPlayTimeCount::GetProgressResult()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		if( !IsMeProgressQuest() )
			SafeSprintf( szBuf, sizeof( szBuf ), STR(1) );
		else
		{
			SafeSprintf( szBuf, sizeof( szBuf ), STR(2), GetProgressPercentage( kQuestData.GetCurrentData(), GetProgressCount() ) );
		}
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestPlayTimeCount::GetProgressResultOnlyNumber()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		SafeSprintf( szBuf, sizeof( szBuf ), STR(1), GetProgressPercentage( kQuestData.GetCurrentData(), GetProgressCount() ) );
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestPlayTimeCount::GetProgressResultOnlyText()
{
	m_szOutProgressResult = "";
	return m_szOutProgressResult;
}

float QuestPlayTimeCount::GetProgressGaugeRate()
{
	return QuestParent::GetProgressGaugeRate();
}

void QuestPlayTimeCount::Process( ioPlayStage *pPlayStage )
{
	if( !pPlayStage ) 
		return;

	// �Ϸ�üũ�� ���⼭ �Ѵ�.
	if( FRAMEGETTIME() - m_dwProcessTime < 1000 )      // 1�ʸ��� üũ
		return;	

	m_dwProcessTime = FRAMEGETTIME();

	// ����Ʈ ���Ἲ üũ
	if( GetPerformType() == QP_EVENT )
	{
		if( !IsAlive() ) return;
	}

	if( IsPCRoomStyle() )
	{
		if( !g_MyInfo.GetPCRoomAuthority() )
			return;
		if( g_MyInfo.IsUserEvent() )
			return;
	}

	// �������� ����Ʈ�� üũ
	if( !IsMeProgressQuest() ) return;

	// �� ��Ÿ�� üũ ( ���� or ���� )
	switch( m_iCompleteRoomStyle )
	{
	case BATTLE_STYLE:
		{
			if( !g_BattleRoomMgr.IsBattleRoom() )
				return;
		}
		break;
	case LADDER_STYLE:
		{
			if( !g_LadderTeamMgr.IsLadderTeam() )
				return;
		}
		break;
	case ALL_BATTLE_STYLE:
		{
			if( !g_BattleRoomMgr.IsBattleRoom() && !g_LadderTeamMgr.IsLadderTeam() && !g_ShuffleRoomMgr.IsShuffleRoom() )
				return;
		}
		break;
	case SHUFFLE_STYLE:
		{
			if( !g_ShuffleRoomMgr.IsShuffleRoom() )
				return;
		}
		break;
	}

	if( m_iCompleteRoomStyle != -1 )
	{
		if( ioPlayMode::GetModeType() == MT_MYROOM			||
			ioPlayMode::GetModeType() == MT_TRAINING		||
			ioPlayMode::GetModeType() == MT_HEADQUARTERS	||
			ioPlayMode::GetModeType() == MT_HOUSE )
		{
			return;
		}

		ioBaseChar *pOwner = pPlayStage->GetOwnerChar();
		if( !pOwner )
			return;

		// ������ ����
		if( pOwner->GetState() == CS_OBSERVER || pOwner->GetState() == CS_VIEW || pOwner->GetState() == CS_LOADING || pOwner->GetState() == CS_READY )
			return;

		// ���� ��� ����
		if( pPlayStage->IsRoundSetEnd() )
			return;

		// �߰� ��� ����
		if( g_GUIMgr.IsShow( ROUND_RESULT_MAIN_WND ) )
			return;	

		// ���� UI �������� ������ ����
		if( g_GUIMgr.IsShow( PARTY_SHAM_BATTLE_WND ) )
			return;

		// ���� Ÿ�� üũ
		if( m_bDeathTimePass )
		{
			if( ioPlayMode::IsZeroHP() )
				return;

			if( ioPlayMode::GetModeType() == MT_MONSTER_SURVIVAL || ioPlayMode::GetModeType() == MT_RAID || Help::IsMonsterDungeonMode(ioPlayMode::GetModeType()) )
			{
				if( ioMonsterSurvivalMode::IsTurnDeathTime() )
					return;
			}		
		}
	}

	// ��� üũ	
	if( m_iCompleteModeType != -1 )
	{		
		if( ioPlayMode::GetModeType() != m_iCompleteModeType )
			return;
	}

	// ������ �ð��� ����.	
	m_dwAccrueSec++;
	if( m_dwAccrueSec >= 60 )    // 1�и��� üũ
	{
		if( AddAttainProcess( m_dwAccrueSec, GetProgressCount() ) )
		{
			// �Ϸ� ó��
			g_QuestMgr.AddQuestAttainList( this );
		}
		m_dwAccrueSec = 0;
	}
}
//////////////////////////////////////////////////////////////////////////
QuestFriendRecommendPlayTime::QuestFriendRecommendPlayTime()
{
	m_dwProcessTime = 0;
	m_dwAccrueSec   = 0;
	m_bDirectSync   = false;
}

QuestFriendRecommendPlayTime::~QuestFriendRecommendPlayTime()
{
}

bool QuestFriendRecommendPlayTime::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( !COMPARE( g_MyInfo.GetGradeLevel(), GetCustomValue( LIMIT_MIN_GRADE ), GetCustomValue( LIMIT_MAX_GRADE ) + 1 ) )
		return false;

	return true;
}

bool QuestFriendRecommendPlayTime::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{	
	return false;
}

void QuestFriendRecommendPlayTime::StartProcessTime() 
{
	m_dwProcessTime = FRAMEGETTIME();
	m_dwAccrueSec   = 0;
}

int QuestFriendRecommendPlayTime::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}

ioHashString &QuestFriendRecommendPlayTime::GetProgressResult()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		if( !IsMeProgressQuest() )
			SafeSprintf( szBuf, sizeof( szBuf ), STR(1) );
		else
		{
			SafeSprintf( szBuf, sizeof( szBuf ), STR(2), GetProgressPercentage( kQuestData.GetCurrentData(), GetProgressCount() ) );
		}
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestFriendRecommendPlayTime::GetProgressResultOnlyNumber()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		SafeSprintf( szBuf, sizeof( szBuf ), STR(1), GetProgressPercentage( kQuestData.GetCurrentData(), GetProgressCount() ) );
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestFriendRecommendPlayTime::GetProgressResultOnlyText()
{
	m_szOutProgressResult = "";
	return m_szOutProgressResult;
}

float QuestFriendRecommendPlayTime::GetProgressGaugeRate()
{
	return QuestParent::GetProgressGaugeRate();
}

void QuestFriendRecommendPlayTime::Process( ioPlayStage *pPlayStage )
{
	if( !pPlayStage ) 
		return;

	// �Ϸ�üũ�� ���⼭ �Ѵ�.
	if( FRAMEGETTIME() - m_dwProcessTime < 1000 )      // 1�ʸ��� üũ
		return;	

	m_dwProcessTime = FRAMEGETTIME();

	// ����Ʈ ���Ἲ üũ
	if( GetPerformType() == QP_EVENT )
	{
		if( !IsAlive() )
			return;
	}

	if( IsPCRoomStyle() )
	{
		if( !g_MyInfo.GetPCRoomAuthority() )
			return;
		if( g_MyInfo.IsUserEvent() )
			return;
	}

	// �������� ����Ʈ�� üũ
	if( !IsMeProgressQuest() )
		return;

	// �� ��Ÿ�� üũ ( ���� or ���� )
	switch( m_iCompleteRoomStyle )
	{
	case BATTLE_STYLE:
		{
			if( !g_BattleRoomMgr.IsBattleRoom() )
				return;
		}
		break;
	case LADDER_STYLE:
		{
			if( !g_LadderTeamMgr.IsLadderTeam() )
				return;
		}
		break;
	case ALL_BATTLE_STYLE:
		{
			if( !g_BattleRoomMgr.IsBattleRoom() && !g_LadderTeamMgr.IsLadderTeam() && !g_ShuffleRoomMgr.IsShuffleRoom() )
				return;
		}
		break;
	case SHUFFLE_STYLE:
		{
			if( !g_ShuffleRoomMgr.IsShuffleRoom() )
				return;
		}
		break;
	}

	if( m_iCompleteRoomStyle != -1 )
	{
		if( ioPlayMode::GetModeType() == MT_MYROOM			||
			ioPlayMode::GetModeType() == MT_TRAINING		||
			ioPlayMode::GetModeType() == MT_HEADQUARTERS	||
			ioPlayMode::GetModeType() == MT_HOUSE )
		{
			return;
		}

		ioBaseChar *pOwner = pPlayStage->GetOwnerChar();
		if( !pOwner )
			return;

		// ������ ����
		if( pOwner->GetState() == CS_OBSERVER || pOwner->GetState() == CS_VIEW || pOwner->GetState() == CS_LOADING || pOwner->GetState() == CS_READY )
			return;

		// ���� ��� ����
		if( pPlayStage->IsRoundSetEnd() )
			return;

		// �߰� ��� ����
		if( g_GUIMgr.IsShow( ROUND_RESULT_MAIN_WND ) )
			return;	

		// ���� Ÿ�� üũ
		if( m_bDeathTimePass )
		{
			if( ioPlayMode::IsZeroHP() )
				return;

			if( ioPlayMode::GetModeType() == MT_MONSTER_SURVIVAL || ioPlayMode::GetModeType() == MT_RAID || Help::IsMonsterDungeonMode(ioPlayMode::GetModeType()))
			{
				if( ioMonsterSurvivalMode::IsTurnDeathTime() )
					return;
			}		
		}
	}

	// ��� üũ	
	if( m_iCompleteModeType != -1 )
	{		
		if( ioPlayMode::GetModeType() != m_iCompleteModeType )
			return;
	}

	// ������ �ð��� ����.	
	m_dwAccrueSec++;
	if( m_dwAccrueSec >= 60 )    // 1�и��� üũ
	{
		if( AddAttainProcess( m_dwAccrueSec, GetProgressCount() ) )
		{
			// �Ϸ� ó��
			g_QuestMgr.AddQuestAttainList( this );
		}
		m_dwAccrueSec = 0;
	}
}
//////////////////////////////////////////////////////////////////////////
QuestQuickStartOption::QuestQuickStartOption()
{
}

QuestQuickStartOption::~QuestQuickStartOption()
{
}

bool QuestQuickStartOption::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestQuickStartOption::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	int iOptionIndex = GetCustomValue( CHECK_INDEX );
	if( iOptionIndex != iValue1 )
		return false;

	return SetAttainProcess( 1, GetProgressCount() );
}
//////////////////////////////////////////////////////////////////////////
QuestBuySoldier::QuestBuySoldier()
{
}

QuestBuySoldier::~QuestBuySoldier()
{
}

bool QuestBuySoldier::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestBuySoldier::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;
	
	// iValue1 = �뺴 �ڵ�
	// iValue2 = �뺴 Ÿ�� : 0(�ð���), 1(����)
	// iValue3 = �뺴 ��Ÿ�� : 0(���ſ뺴), 1(�Ϲ� ü��), 2(PC�� ü��), 3(���� ü��)
	int iSoldierType = GetCustomValue( CHECK_TYPE );
	if( iSoldierType != -1 )
	{
		if( iSoldierType != iValue2 )
			return false;
	}

	int iSoldierStyle = GetCustomValue( CHECK_STYLE );
	if( iSoldierStyle != -1 )
	{
		if( iSoldierStyle != iValue3 )
			return false;
	}

	int iMaxSoldier = GetCustomValue( MAX_SOLDIER );
	for(int i = 0;i < iMaxSoldier;i++)
	{
		if( iValue1 == GetCustomValue( SOLDIER_CODE + i ) )
			return AddAttainProcess( 1, GetProgressCount() );
	}		
	if( iMaxSoldier == 0 )
		return AddAttainProcess( 1, GetProgressCount() );
	return false;
}

bool QuestBuySoldier::IsCheckAlreadyAttain()
{
	int iMaxCharCount = g_MyInfo.GetCharCount();
	for(int i = 0;i < iMaxCharCount;i++)
	{
		const CHARACTER &rkCharInfo = g_MyInfo.GetCharacter( i );
		if( IsCheckCompleteTerm( GetCompleteValue(), rkCharInfo.m_class_type, (int)rkCharInfo.m_ePeriodType, (int)rkCharInfo.m_chExerciseStyle ) )
		{
			return true;
		}
	}	
	return false;
}

int QuestBuySoldier::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestPvETimeAttack::QuestPvETimeAttack()
{
}

QuestPvETimeAttack::~QuestPvETimeAttack()
{
}

bool QuestPvETimeAttack::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestPvETimeAttack::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	// ���� üũ
	if( iValue1 != GetCustomValue( LIMIT_ROUND ) )
		return false;

	// �ɸ� �ð� üũ
	if( iValue3 > GetCustomValue( LIMIT_SECOND ) )
		return false;

	return SetAttainProcess( 1 );
}
//////////////////////////////////////////////////////////////////////////
QuestSoccerBallHit::QuestSoccerBallHit()
{
}

QuestSoccerBallHit::~QuestSoccerBallHit()
{
}

bool QuestSoccerBallHit::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestSoccerBallHit::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return AddAttainProcess( iValue1, GetProgressCount() );
}

int QuestSoccerBallHit::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}

ioHashString &QuestSoccerBallHit::GetProgressResult()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		if( !IsMeProgressQuest() )
			SafeSprintf( szBuf, sizeof( szBuf ), STR(1) );
		else
		{
			SafeSprintf( szBuf, sizeof( szBuf ), STR(2), GetProgressPercentage( kQuestData.GetCurrentData(), GetProgressCount() ) );
		}
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestSoccerBallHit::GetProgressResultOnlyNumber()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		SafeSprintf( szBuf, sizeof( szBuf ), STR(1), GetProgressPercentage( kQuestData.GetCurrentData(), GetProgressCount() ) );
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestSoccerBallHit::GetProgressResultOnlyText()
{
	m_szOutProgressResult = "";
	return m_szOutProgressResult;
}

float QuestSoccerBallHit::GetProgressGaugeRate()
{
	return QuestParent::GetProgressGaugeRate();
}
//////////////////////////////////////////////////////////////////////////
QuestSoccerGoal::QuestSoccerGoal()
{
}

QuestSoccerGoal::~QuestSoccerGoal()
{
}

bool QuestSoccerGoal::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestSoccerGoal::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return AddAttainProcess( iValue1, GetProgressCount() );
}

int QuestSoccerGoal::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}

ioHashString &QuestSoccerGoal::GetProgressResult()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		if( !IsMeProgressQuest() )
			SafeSprintf( szBuf, sizeof( szBuf ), STR(1) );
		else
		{
			SafeSprintf( szBuf, sizeof( szBuf ), STR(2), GetProgressPercentage( kQuestData.GetCurrentData(), GetProgressCount() ) );
		}
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestSoccerGoal::GetProgressResultOnlyNumber()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		SafeSprintf( szBuf, sizeof( szBuf ), STR(1), GetProgressPercentage( kQuestData.GetCurrentData(), GetProgressCount() ) );
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestSoccerGoal::GetProgressResultOnlyText()
{
	m_szOutProgressResult = "";
	return m_szOutProgressResult;
}

float QuestSoccerGoal::GetProgressGaugeRate()
{
	return QuestParent::GetProgressGaugeRate();
}
//////////////////////////////////////////////////////////////////////////
QuestSoccerAssist::QuestSoccerAssist()
{
}

QuestSoccerAssist::~QuestSoccerAssist()
{
}

bool QuestSoccerAssist::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestSoccerAssist::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return AddAttainProcess( iValue1, GetProgressCount() );
}

int QuestSoccerAssist::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}

ioHashString &QuestSoccerAssist::GetProgressResult()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		if( !IsMeProgressQuest() )
			SafeSprintf( szBuf, sizeof( szBuf ), STR(1) );
		else
		{
			SafeSprintf( szBuf, sizeof( szBuf ), STR(2), GetProgressPercentage( kQuestData.GetCurrentData(), GetProgressCount() ) );
		}
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestSoccerAssist::GetProgressResultOnlyNumber()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		SafeSprintf( szBuf, sizeof( szBuf ), STR(1), GetProgressPercentage( kQuestData.GetCurrentData(), GetProgressCount() ) );
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestSoccerAssist::GetProgressResultOnlyText()
{
	m_szOutProgressResult = "";
	return m_szOutProgressResult;
}

float QuestSoccerAssist::GetProgressGaugeRate()
{
	return QuestParent::GetProgressGaugeRate();
}
//////////////////////////////////////////////////////////////////////////
QuestSoccerSave::QuestSoccerSave()
{
}

QuestSoccerSave::~QuestSoccerSave()
{
}

bool QuestSoccerSave::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestSoccerSave::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return AddAttainProcess( iValue1, GetProgressCount() );
}

int QuestSoccerSave::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestExcavationTry::QuestExcavationTry()
{
}

QuestExcavationTry::~QuestExcavationTry()
{
}

bool QuestExcavationTry::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ���� ���� üũ
	if( g_MyInfo.GetExcavationLevel() < GetCustomValue( LIMIT_EXCAVATION_LEVEL ) )
		return false;

	return true;
}

bool QuestExcavationTry::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return SetAttainProcess();
}
//////////////////////////////////////////////////////////////////////////
QuestExcavationLevelUP::QuestExcavationLevelUP()
{
}

QuestExcavationLevelUP::~QuestExcavationLevelUP()
{

}

bool QuestExcavationLevelUP::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ���� ���� üũ
	if( g_MyInfo.GetExcavationLevel() < GetCustomValue( LIMIT_EXCAVATION_LEVEL ) )
		return false;

	return true;
}

bool QuestExcavationLevelUP::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return SetAttainProcess( g_MyInfo.GetExcavationLevel(), GetProgressCount() );
}

bool QuestExcavationLevelUP::IsCheckAlreadyAttain()
{
	return SetAttainProcess( g_MyInfo.GetExcavationLevel(), GetProgressCount() );
}

int QuestExcavationLevelUP::GetProgressCount()
{
	return GetCustomValue( CHECK_EXCAVATION_LEVEL );
}	

ioHashString &QuestExcavationLevelUP::GetProgressResult()
{
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		if( !IsMeProgressQuest() )
			SafeSprintf( szBuf, sizeof( szBuf ), STR(1) );
		else
		{
			SafeSprintf( szBuf, sizeof( szBuf ), STR(2), GetProgressPercentage( max( 0, g_MyInfo.GetExcavationLevel() - GetCustomValue( MINUS_EXCAVATION_LEVEL ) ), 
									max( 0, GetProgressCount() - GetCustomValue( MINUS_EXCAVATION_LEVEL ) ) ) );
		}
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestExcavationLevelUP::GetProgressResultOnlyNumber()
{
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		SafeSprintf( szBuf, sizeof( szBuf ), STR(1), GetProgressPercentage( max( 0, g_MyInfo.GetExcavationLevel() - GetCustomValue( MINUS_EXCAVATION_LEVEL ) ), 
								max( 0, GetProgressCount() - GetCustomValue( MINUS_EXCAVATION_LEVEL ) ) ) );
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestExcavationLevelUP::GetProgressResultOnlyText()
{
	m_szOutProgressResult = "";
	return m_szOutProgressResult;
}

float QuestExcavationLevelUP::GetProgressGaugeRate()
{
	if( !IsMeProgressQuest() )
		return FLOAT1;
	int iCurrent = max( 0, g_MyInfo.GetExcavationLevel() - GetCustomValue( MINUS_EXCAVATION_LEVEL ) );
	int iMax = max( 0, GetProgressCount() - GetCustomValue( MINUS_EXCAVATION_LEVEL ) );
	if( iMax == 0 ) return 0.0f;

	return (float)iCurrent / (float)iMax;
}
//////////////////////////////////////////////////////////////////////////
QuestExcavationSuccess::QuestExcavationSuccess()
{
}

QuestExcavationSuccess::~QuestExcavationSuccess()
{
}

bool QuestExcavationSuccess::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ���� ���� üũ
	if( g_MyInfo.GetExcavationLevel() < GetCustomValue( LIMIT_EXCAVATION_LEVEL ) )
		return false;

	return true;
}

bool QuestExcavationSuccess::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	int iMaxArtifact = GetCustomValue( MAX_ARTIFACT );
	if( iMaxArtifact == 0 )
	{
		// ��� ���� ȹ��
		if( iValue1 < ioExcavationManager::EXTRAITEM_START_TYPE  )
		{
			int iItemPeso = iValue2;
			int iItemGrade= iValue3;

			// ��Ұ� �̻��� �Ǿ����
			if( iItemPeso < GetCustomValue( LIMIT_PESO ) ) return false;

			// ����� �̻��� �Ǿ����
			int iLimitItemGrade = GetCustomValue( LIMIT_PESO_GRADE );
			if( iLimitItemGrade > 0 )
			{
				if( g_ExcavationMgr.GetGradeRate( g_ExcavationMgr.GetGradeArray( iItemGrade ) ) < 
					g_ExcavationMgr.GetGradeRate( g_ExcavationMgr.GetGradeArray( iLimitItemGrade ) ) ) return false;

			}

			return AddAttainProcess( 1, GetProgressCount() );
		}
		else
		{
			return false;
		}
	}
	else
	{
		for(int i = 0;i < iMaxArtifact;i++)
		{
			if( iValue1 < ioExcavationManager::EXTRAITEM_START_TYPE  )
			{
				// ��� ����
				if( iValue1 != GetCustomValue( ARTIFACT_CODE + i ) ) continue; 
			}
			else
			{
				// ��� ����
				if( iValue2 != GetCustomValue( ARTIFACT_CODE + i ) ) continue;
			}

			// ��� ���� ȹ��
			if( iValue1 < ioExcavationManager::EXTRAITEM_START_TYPE  )
			{
				int iItemPeso = iValue2;
				int iItemGrade= iValue3;

				// ��Ұ� �̻��� �Ǿ����
				if( iItemPeso < GetCustomValue( LIMIT_PESO ) ) return false;

				// ����� �̻��� �Ǿ����
				int iLimitItemGrade = GetCustomValue( LIMIT_PESO_GRADE );
				if( iLimitItemGrade > 0 )
				{
					if( g_ExcavationMgr.GetGradeRate( g_ExcavationMgr.GetGradeArray( iItemGrade ) ) < 
						g_ExcavationMgr.GetGradeRate( g_ExcavationMgr.GetGradeArray( iLimitItemGrade ) ) ) return false;

				}
				
				return AddAttainProcess( 1, GetProgressCount() );
			}
			else // ��� ���� ȹ��
			{
				int iReinforce  = iValue3 / 10000;
				int iPeriodTime = iValue3 % 10000;
				
				// ��� ���尪 �̻��� �Ǿ����
				if( iReinforce < GetCustomValue( LIMIT_REINFORCE ) ) return false;

				// ��� �Ⱓ �̻��� �Ǿ����
				int iLimitPeriodTime = GetCustomValue( LIMIT_PERIOD_TIME );
				if( iLimitPeriodTime == -1 ) //���� ���
				{
					if( iPeriodTime != 0 ) return false;
				}
				else if( iLimitPeriodTime > 0 )
				{
					// ������� �ƴѵ� �ð��� �̻��� �ȵǸ� �ƿ�!
					if( iPeriodTime != 0 && iPeriodTime < iLimitPeriodTime ) return false;
				}

				return AddAttainProcess( 1, GetProgressCount() );
			}				
		}
	}
	return false;
}

int QuestExcavationSuccess::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestExcavationFail::QuestExcavationFail()
{
}

QuestExcavationFail::~QuestExcavationFail()
{
}

bool QuestExcavationFail::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ���� ���� üũ
	if( g_MyInfo.GetExcavationLevel() < GetCustomValue( LIMIT_EXCAVATION_LEVEL ) )
		return false;

	return true;
}

bool QuestExcavationFail::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return AddAttainProcess( 1, GetProgressCount() );
}

int QuestExcavationFail::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestExcavationTime::QuestExcavationTime()
{
	m_dwProcessTime = 0;
	m_dwAccrueSec   = 0;
}

QuestExcavationTime::~QuestExcavationTime()
{
}

bool QuestExcavationTime::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ���� ���� üũ
	if( g_MyInfo.GetExcavationLevel() < GetCustomValue( LIMIT_EXCAVATION_LEVEL ) )
		return false;

	return true;
}

bool QuestExcavationTime::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{	
	return false;
}

void QuestExcavationTime::StartProcessTime() 
{
	m_dwProcessTime = FRAMEGETTIME();
	m_dwAccrueSec   = 0;
}

int QuestExcavationTime::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}

ioHashString &QuestExcavationTime::GetProgressResult()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		if( !IsMeProgressQuest() )
			SafeSprintf( szBuf, sizeof( szBuf ), STR(1) );
		else
		{
			SafeSprintf( szBuf, sizeof( szBuf ), STR(2), GetProgressPercentage( kQuestData.GetCurrentData(), GetProgressCount() ) );
		}
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestExcavationTime::GetProgressResultOnlyNumber()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		SafeSprintf( szBuf, sizeof( szBuf ), STR(1), GetProgressPercentage( kQuestData.GetCurrentData(), GetProgressCount() ) );
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestExcavationTime::GetProgressResultOnlyText()
{
	m_szOutProgressResult = "";
	return m_szOutProgressResult;
}

float QuestExcavationTime::GetProgressGaugeRate()
{
	return QuestParent::GetProgressGaugeRate();
}

void QuestExcavationTime::Process( ioPlayStage *pPlayStage )
{
	if( !pPlayStage ) return;

	// �Ϸ�üũ�� ���⼭ �Ѵ�.
	if( FRAMEGETTIME() - m_dwProcessTime < 1000 )      // 1�ʸ��� üũ
		return;	

	m_dwProcessTime = FRAMEGETTIME();

	// ����Ʈ ���Ἲ üũ
	if( !QuestParent::IsCheckCompleteTerm( GetCompleteValue() ) )
		return;

	ioBaseChar *pOwnerChar = pPlayStage->GetOwnerChar();
	if( !pOwnerChar ) return;
	if( !pOwnerChar->IsExcavating() ) return;

	// ������ �ð��� ����.	
	m_dwAccrueSec++;
	if( m_dwAccrueSec >= 60 )    // 1�и��� üũ
	{
		if( AddAttainProcess( m_dwAccrueSec, GetProgressCount() ) )
		{
			// �Ϸ� ó��
			g_QuestMgr.AddQuestAttainList( this );
		}
		m_dwAccrueSec = 0;
	}
}
//////////////////////////////////////////////////////////////////////////
QuestScreenShot::QuestScreenShot()
{
}

QuestScreenShot::~QuestScreenShot()
{
}

bool QuestScreenShot::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestScreenShot::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{	
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return AddAttainProcess( 1, GetProgressCount() );
}

int QuestScreenShot::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestMovieMode::QuestMovieMode()
{
}

QuestMovieMode::~QuestMovieMode()
{
}

bool QuestMovieMode::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestMovieMode::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{	
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return SetAttainProcess();
}
//////////////////////////////////////////////////////////////////////////
QuestMakeMovie::QuestMakeMovie()
{
}

QuestMakeMovie::~QuestMakeMovie()
{
}

bool QuestMakeMovie::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestMakeMovie::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{	
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return SetAttainProcess();
}
//////////////////////////////////////////////////////////////////////////
QuestExtraItemAcquire::QuestExtraItemAcquire()
{
}

QuestExtraItemAcquire::~QuestExtraItemAcquire()
{
}

bool QuestExtraItemAcquire::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestExtraItemAcquire::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;
	
	int iMaxExtraItem = GetCustomValue( MAX_EXTRA_ITEM );
	if( iMaxExtraItem == 0 )
		return SetAttainProcess();
	else
	{
		int iLimitPeriodTime = GetCustomValue( LIMIT_PERIOD_TIME );
		for(int i = 0;i < iMaxExtraItem;i++)
		{
			if( iValue1 != GetCustomValue( EXTRA_ITEM_CODE + i ) ) continue;
			
			// ��� �Ⱓ �̻��� �Ǿ����
			if( iLimitPeriodTime == -1 ) //���� ���
			{
				if( iValue2 != 0 ) return false;
			}
			else if( iLimitPeriodTime > 0 )
			{
				// ������� �ƴѵ� �ð��� �̻��� �ȵǸ� �ƿ�!
				if( iValue2 != 0 && iValue2 < iLimitPeriodTime ) return false;
			}
			return SetAttainProcess();
		}
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
QuestExtraItemEquip::QuestExtraItemEquip()
{
}

QuestExtraItemEquip::~QuestExtraItemEquip()
{
}

bool QuestExtraItemEquip::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestExtraItemEquip::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	int iExtraItemCode = GetCustomValue( EXTRA_ITEM_CODE );
	if( iExtraItemCode == 0 )
		return SetAttainProcess();
	else
	{
		int iLimitPeriodType = GetCustomValue( LIMIT_PERIOD_TIME );
		if( iLimitPeriodType != -1 )
		{
			if( iValue2 != iLimitPeriodType ) return false;
		}
		return SetAttainProcess();
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
QuestExtraItemEquipKo::QuestExtraItemEquipKo()
{
}

QuestExtraItemEquipKo::~QuestExtraItemEquipKo()
{
}

bool QuestExtraItemEquipKo::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestExtraItemEquipKo::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	if( (BOOL)GetCustomValue( BEST_ATTACKER_PASS ) == TRUE )
	{
		if( (BOOL)iValue2 == TRUE )
			return false;
	}

	int iExtraItemCode = GetCustomValue( EXTRA_ITEM_CODE );
	if( iExtraItemCode == 0 )
		return false;

	if( !g_QuestMgr.IsOwnerCharExtraItem( iExtraItemCode ) )
		return false;

	return AddAttainProcess( iValue1, GetProgressCount() );
}

int QuestExtraItemEquipKo::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}

ioHashString &QuestExtraItemEquipKo::GetProgressResult()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		if( !IsMeProgressQuest() )
			SafeSprintf( szBuf, sizeof( szBuf ), STR(1) );
		else
		{
			SafeSprintf( szBuf, sizeof( szBuf ), STR(2), GetProgressPercentage( kQuestData.GetCurrentData(), GetProgressCount() ) );
		}
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestExtraItemEquipKo::GetProgressResultOnlyNumber()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		SafeSprintf( szBuf, sizeof( szBuf ), STR(1), GetProgressPercentage( kQuestData.GetCurrentData(), GetProgressCount() ) );
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestExtraItemEquipKo::GetProgressResultOnlyText()
{
	m_szOutProgressResult = "";
	return m_szOutProgressResult;
}

float QuestExtraItemEquipKo::GetProgressGaugeRate()
{
	return QuestParent::GetProgressGaugeRate();
}
//////////////////////////////////////////////////////////////////////////
QuestGangsiKill::QuestGangsiKill()
{
}

QuestGangsiKill::~QuestGangsiKill()
{

}

bool QuestGangsiKill::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestGangsiKill::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return AddAttainProcess( 1, GetProgressCount() );
}

int QuestGangsiKill::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestGangsiHumanKill::QuestGangsiHumanKill()
{
}

QuestGangsiHumanKill::~QuestGangsiHumanKill()
{

}

bool QuestGangsiHumanKill::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestGangsiHumanKill::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return AddAttainProcess( 1, GetProgressCount() );
}

int QuestGangsiHumanKill::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestGangsiAliveTime::QuestGangsiAliveTime()
{
	m_bDirectSync   = false;
	m_dwMilliSecond = 0;
}

QuestGangsiAliveTime::~QuestGangsiAliveTime()
{
}

bool QuestGangsiAliveTime::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestGangsiAliveTime::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	m_dwMilliSecond += iValue1;         // 500ms ���� ���´�.

	if( m_dwMilliSecond < 1000 )
		return false;

	// �ʴ����� �־���.
	m_dwMilliSecond = 0;	
	return AddAttainProcess( 1, GetProgressCount() );
}

int QuestGangsiAliveTime::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestGangsiHumanWin::QuestGangsiHumanWin()
{
}

QuestGangsiHumanWin::~QuestGangsiHumanWin()
{

}

bool QuestGangsiHumanWin::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestGangsiHumanWin::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return AddAttainProcess( 1, GetProgressCount() );
}

int QuestGangsiHumanWin::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestHitAttackAttribute::QuestHitAttackAttribute()
{
}

QuestHitAttackAttribute::~QuestHitAttackAttribute()
{

}

bool QuestHitAttackAttribute::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestHitAttackAttribute::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	int iMaxAttribute = GetCustomValue( MAX_ATTRIBUTE_CODE );
	for(int i = 0;i < iMaxAttribute;i++)
	{
		if( iValue1 == GetCustomValue( ATTRIBUTE_CODE ) )
		{
			return AddAttainProcess( 1, GetProgressCount() );
		}
	}

	return false;
}

int QuestHitAttackAttribute::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestGuildLevel::QuestGuildLevel()
{
}

QuestGuildLevel::~QuestGuildLevel()
{
}

bool QuestGuildLevel::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;
	return true;
}

bool QuestGuildLevel::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return SetAttainProcess( g_GuildInfo.GetMyGuildLevel(), GetProgressCount() );
}

int QuestGuildLevel::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestGuildLevelMaintenance::QuestGuildLevelMaintenance()
{
}

QuestGuildLevelMaintenance::~QuestGuildLevelMaintenance()
{
}

bool QuestGuildLevelMaintenance::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;
	return true;
}

bool QuestGuildLevelMaintenance::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	const ioTaskTournamentReward* const pReward = dynamic_cast<const ioTaskTournamentReward* const>( g_RserveTask.GetTask( (ReservedTaskTypes)iValue1, (ReservedTaskWhens)iValue2 ) );
	if( !pReward )
		return false;

	// ��� ��ŷ�� �ȵǰų� ���� ������ ������ �ʱ�ȭ
	int iMyGuildLevel	= (int)g_GuildInfo.GetMyGuildLevel();
	int iSeasonBonus	= pReward->iLadderBonusPeso;
	int iCustomValue	= GetCustomValue( LIMIT_GUILD_LEVEL );
	int iCustomValue2	= GetCustomValue( LIMIT_CAMP_REWARD );

	QuestData &rkData = GetMyInfoQuestData();
	int iCurQuestData = 0;
	int iProgressCount = GetProgressCount();
	if( rkData.GetMainIndex() > 0 )
	{
		iCurQuestData = rkData.GetCurrentData();
	}

    if( iMyGuildLevel < iCustomValue || iSeasonBonus < iCustomValue2 )
	{
		// �ʱ�ȭ üũ
		{
			ErrorReport::SetPosition( 10000, 1 );
			ErrorReport::SetFormat( "" );
			// �α� ���� ����
			char szErrorPos[MAX_PATH] = "";
			SafeSprintf( szErrorPos, sizeof( szErrorPos ), STR(2), g_MyInfo.GetMyIP().c_str(),
																										g_MyInfo.GetPrivateID().c_str(),
																										g_MyInfo.GetPublicID().c_str(),
																										iMyGuildLevel, iSeasonBonus,
																										iCurQuestData, iProgressCount,
																										(int)P2PNetwork::IsNetworkPlaying() );


			SP2Packet kPacket( LUPK_LOG );
			kPacket << "ClientError";  // �α� ���� Ÿ��
			kPacket << szErrorPos;
			kPacket << 317;  // ������ȣ:317
			kPacket << false; // write db
			P2PNetwork::SendLog(kPacket);
		}

		SetAttainProcess( 0, GetProgressCount() ); // �ʱ�ȭ
		g_QuestMgr.AddQuestDelAndFailList( false, this );
		return false;
	}
	return AddAttainProcess( 1, GetProgressCount() );
}

int QuestGuildLevelMaintenance::GetProgressCount()
{
	return GetCustomValue( CHECK_COUNT );
}
//////////////////////////////////////////////////////////////////////////
QuestGuildTeamWin::QuestGuildTeamWin()
{
}

QuestGuildTeamWin::~QuestGuildTeamWin()
{

}

bool QuestGuildTeamWin::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestGuildTeamWin::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	if( !g_GuildInfo.IsGuild() )						//��� ������ ����
		return false;
	if( !g_LadderTeamMgr.IsLadderTeam() )				//������ �ƴϸ� ����
		return false;
	if( g_LadderTeamMgr.IsHeroMatchMode() )				//�������̸� ����
		return false;
	if( !g_LadderTeamMgr.IsGuildTeam() )				//������� �ƴϸ� ����
		return false;

	return AddAttainProcess( iValue1, GetProgressCount() );
}

int QuestGuildTeamWin::GetProgressCount()
{
	return GetCustomValue( WIN_COUNT );
}

ioHashString &QuestGuildTeamWin::GetProgressResult()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		if( !IsMeProgressQuest() )
			SafeSprintf( szBuf, sizeof( szBuf ), STR(1) );
		else
			SafeSprintf( szBuf, sizeof( szBuf ), STR(2), GetProgressPercentage( kQuestData.GetCurrentData(), GetProgressCount() ) );
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestGuildTeamWin::GetProgressResultOnlyNumber()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
		SafeSprintf( szBuf, sizeof( szBuf ), STR(1), GetProgressPercentage( kQuestData.GetCurrentData(), GetProgressCount() ) );
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestGuildTeamWin::GetProgressResultOnlyText()
{
	m_szOutProgressResult = "";
	return m_szOutProgressResult;
}

float QuestGuildTeamWin::GetProgressGaugeRate()
{
	return QuestParent::GetProgressGaugeRate();
}
//////////////////////////////////////////////////////////////////////////
QuestGuildTeamPlayTime::QuestGuildTeamPlayTime()
{
	m_dwProcessTime = 0;
	m_dwAccrueSec   = 0;
}

QuestGuildTeamPlayTime::~QuestGuildTeamPlayTime()
{
}

bool QuestGuildTeamPlayTime::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestGuildTeamPlayTime::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	return false;
}

void QuestGuildTeamPlayTime::StartProcessTime() 
{
	m_dwProcessTime = FRAMEGETTIME();
	m_dwAccrueSec   = 0;
}

int QuestGuildTeamPlayTime::GetProgressCount()
{
	return GetCustomValue( CHECK_PLAY_MIN );
}

void QuestGuildTeamPlayTime::ChangeStageInit()
{
	// ��� ����ȭ �ƴ� ����Ʈ ����ȭ
	QuestParent::ChangeStageInit();
}

void QuestGuildTeamPlayTime::Process( ioPlayStage *pPlayStage )
{
	if( !pPlayStage ) 
		return;

	// ����Ʈ ���Ἲ üũ
	if( !QuestParent::IsCheckCompleteTerm( GetCompleteValue() ) )
		return;

	    // ��� ������ �н�		  // ������ �ƴϸ� �н�				 // �������̸� �н�					  // ������� �ƴϸ� �н�
	if( !g_GuildInfo.IsGuild() || !g_LadderTeamMgr.IsLadderTeam() || g_LadderTeamMgr.IsHeroMatchMode() || !g_LadderTeamMgr.IsGuildTeam() )
		return;

	// ���� üũ
	ioBaseChar *pOwner = pPlayStage->GetOwnerChar();
	if( !pOwner )
		return;

	// ������ ����
	if( pOwner->GetState() == CS_OBSERVER || pOwner->GetState() == CS_VIEW || pOwner->GetState() == CS_LOADING || pOwner->GetState() == CS_READY )
		return;

	// ���� ��� ����
	if( pPlayStage->IsRoundSetEnd() )
		return;

	// �߰� ��� ����
	if( g_GUIMgr.IsShow( ROUND_RESULT_MAIN_WND ) )
		return;	

	// �Ϸ�üũ�� ���⼭ �Ѵ�.
	if( FRAMEGETTIME() - m_dwProcessTime < 1000 )      // 1�ʸ��� üũ
		return;	

	m_dwProcessTime = FRAMEGETTIME();
	m_dwAccrueSec++;
	if( m_dwAccrueSec >= 60 )    // 1�и��� üũ
	{
		m_dwAccrueSec = 0;
		if( AddAttainProcess( 1, GetProgressCount() ) )
		{
			// �Ϸ� ó��
			g_QuestMgr.AddQuestAttainList( this );
		}
	}
}
//////////////////////////////////////////////////////////////////////////
QuestPlayTimeRepeat::QuestPlayTimeRepeat()
{
	m_dwProcessTime = 0;
	m_dwAccrueSec   = 0;
	m_bDirectSync   = false;
}

QuestPlayTimeRepeat::~QuestPlayTimeRepeat()
{
}

bool QuestPlayTimeRepeat::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	// ��� üũ
	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	return true;
}

bool QuestPlayTimeRepeat::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{	
	return false;
}

void QuestPlayTimeRepeat::StartProcessTime() 
{
	m_dwProcessTime = FRAMEGETTIME();
	m_dwAccrueSec   = 0;
}

int QuestPlayTimeRepeat::GetProgressCount()
{
	return GetCustomValue( REPEAT_TIME );
}

ioHashString &QuestPlayTimeRepeat::GetProgressResult()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
	{
		if( !IsMeProgressQuest() )
			sprintf_e( szBuf, "%d/%d%s", GetProgressCount(), GetProgressCount(), m_szProgressResult.c_str() );
		else
			sprintf_e( szBuf, "%d/%d%s", min( GetProgressCount() - 1, (int)kQuestData.GetCurrentData() ), GetProgressCount(), m_szProgressResult.c_str() );
	}
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestPlayTimeRepeat::GetProgressResultOnlyNumber()
{
	QuestData &kQuestData = GetMyInfoQuestData();
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
		sprintf_e( szBuf, "%d/%d", min( GetProgressCount() - 1, (int)kQuestData.GetCurrentData() ), GetProgressCount() );
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

ioHashString &QuestPlayTimeRepeat::GetProgressResultOnlyText()
{
	char szBuf[MAX_PATH] = "";
	if( !m_szProgressResult.IsEmpty() )
		sprintf_e( szBuf, "%d%s", GetProgressCount(), m_szProgressResult.c_str() );
	m_szOutProgressResult = szBuf;
	return m_szOutProgressResult;
}

float QuestPlayTimeRepeat::GetProgressGaugeRate()
{
	if( !IsMeProgressQuest() )
		return FLOAT1;

	QuestData &kQuestData = GetMyInfoQuestData();
	int iCurrent = min( GetProgressCount() - 1, (int)kQuestData.GetCurrentData() );
	int iMax = GetProgressCount();
	if( iMax == 0 ) return 0.0f;

	return (float)iCurrent / (float)iMax;
}

bool QuestPlayTimeRepeat::AddAttainProcess( int iAddValue /* = 0 */, int iCheckValue /* = 0  */ )
{
	QuestData &rkData = GetMyInfoQuestData();
	if( rkData.GetMainIndex() == 0 )
	{
		LOG.PrintTimeAndLog( 0, "QuestPlayTimeRepeat None Quest Type : %d - %d", GetMainIndex(), GetSubIndex() );
		return false;
	}

	rkData.SetCurrentData( min( iCheckValue, (int)rkData.GetCurrentData() + iAddValue ) );
	if( rkData.GetCurrentData() >= (DWORD)iCheckValue )
	{
		SendAttainData( rkData );
	}
	else if( m_bDirectSync )
	{
		SendAttainData( rkData );
	}
	else if( !m_bDirectSync && FRAMEGETTIME() - m_dwDirectSyncSafeTime > 600000 )
	{
		// ��� ������ ���ص� �ּ� 10�п� �ѹ��� �ϵ����ؾ��� ��Ʈ�� ������ ���� �ս��� �߻������ʴ´�.
		SendAttainData( rkData );
		m_dwDirectSyncSafeTime = FRAMEGETTIME();
	}
	//ShowTooltip();
	return false;
}

void QuestPlayTimeRepeat::Process( ioPlayStage *pPlayStage )
{
	if( !pPlayStage ) 
		return;

	// �Ϸ�üũ�� ���⼭ �Ѵ�.
	if( FRAMEGETTIME() - m_dwProcessTime < 1000 )      // 1�ʸ��� üũ
		return;	
	m_dwProcessTime = FRAMEGETTIME();

	// ����Ʈ ���Ἲ üũ
	if( GetPerformType() == QP_EVENT )
	{
		if( !IsAlive() )
			return;
	}

	if( IsPCRoomStyle() )
	{
		if( !g_MyInfo.GetPCRoomAuthority() )
			return;
		if( g_MyInfo.IsUserEvent() )
			return;
	}

	// �������� ����Ʈ�� üũ
	if( !IsMeProgressQuest() )
		return;

	// �� ��Ÿ�� üũ ( ���� or ���� )
	switch( m_iCompleteRoomStyle )
	{
	case BATTLE_STYLE:
		{
			if( !g_BattleRoomMgr.IsBattleRoom() )
				return;
		}
		break;
	case LADDER_STYLE:
		{
			if( !g_LadderTeamMgr.IsLadderTeam() )
				return;
		}
		break;
	case ALL_BATTLE_STYLE:
		{
			if( !g_BattleRoomMgr.IsBattleRoom() && !g_LadderTeamMgr.IsLadderTeam() && !g_ShuffleRoomMgr.IsShuffleRoom() )
				return;
		}
		break;
	case SHUFFLE_STYLE:
		{
			if( !g_ShuffleRoomMgr.IsShuffleRoom() )
				return;
		}
		break;
	}

	if( m_iCompleteRoomStyle != -1 )
	{
		if( ioPlayMode::GetModeType() == MT_MYROOM			||
			ioPlayMode::GetModeType() == MT_TRAINING		|| 
			ioPlayMode::GetModeType() == MT_HEADQUARTERS	||
			ioPlayMode::GetModeType() == MT_HOUSE )
		{
			return;
		}

		ioBaseChar *pOwner = pPlayStage->GetOwnerChar();
		if( !pOwner )
			return;

		// ������ ����
		if( pOwner->GetState() == CS_OBSERVER || pOwner->GetState() == CS_VIEW || pOwner->GetState() == CS_LOADING || pOwner->GetState() == CS_READY )
			return;

		// ���� ��� ����
		if( pPlayStage->IsRoundSetEnd() )
			return;

		// �߰� ��� ����
		if( g_GUIMgr.IsShow( ROUND_RESULT_MAIN_WND ) )
			return;	

		// ���� Ÿ�� üũ
		if( m_bDeathTimePass )
		{
			if( ioPlayMode::IsZeroHP() )
				return;

			if( ioPlayMode::GetModeType() == MT_MONSTER_SURVIVAL || ioPlayMode::GetModeType() == MT_RAID || Help::IsMonsterDungeonMode(ioPlayMode::GetModeType()))
			{
				if( ioMonsterSurvivalMode::IsTurnDeathTime() )
					return;
			}		
		}
	}

	// ��� üũ	
	if( m_iCompleteModeType != -1 )
	{		
		if( ioPlayMode::GetModeType() != m_iCompleteModeType )
			return;
	}

	// ������ �ð��� ����.	
	m_dwAccrueSec++;
	if( m_dwAccrueSec >= 60 )    // 1�и��� üũ
	{
		m_dwAccrueSec = 0;
		AddAttainProcess( 1, GetProgressCount() );
	}
}
//////////////////////////////////////////////////////////////////////////
QuestDormantUser::QuestDormantUser()
{
}

QuestDormantUser::~QuestDormantUser()
{
}

bool QuestDormantUser::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;
	if( iValue1 < GetCustomValue( LIMIT_DAY ) )
		return false;
	return true;
}

bool QuestDormantUser::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return SetAttainProcess( 1 ); 
}

bool QuestDormantUser::IsCheckAlreadyAttain()
{
	return SetAttainProcess( 1 );   // ������ �Ϸ�
}

void QuestDormantUser::Process( ioPlayStage *pPlayStage )
{
	// ����Ʈ ���Ἲ üũ
	if( GetPerformType() == QP_EVENT )
	{
		if( !IsAlive() ) return;
	}

	if( IsPCRoomStyle() )
	{
		if( !g_MyInfo.GetPCRoomAuthority() ) return;
		if( g_MyInfo.IsUserEvent() )
			return;
	}

	// �������� ����Ʈ�� üũ
	if( !IsMeProgressQuest() ) return;

	// ������ �Ϸ� ó��
	if( SetAttainProcess( 1 ) )
		g_QuestMgr.AddQuestAttainList( this );
}
//////////////////////////////////////////////////////////////////////////
QuestDormantUserCustom::QuestDormantUserCustom()
{
}

QuestDormantUserCustom::~QuestDormantUserCustom()
{
}

bool QuestDormantUserCustom::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;

	if ( iValue1 <= 0 || iValue2 <= 0 || iValue3 <= 0 )
		return false;

	if( iValue1 > GetCustomValue( LIMIT_YEAR ) ||
		iValue2 > GetCustomValue( LIMIT_MONTH ) ||
		iValue3 > GetCustomValue( LIMIT_DAY ) )
		return false;
	
	return true;
}

bool QuestDormantUserCustom::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckCompleteTerm( dwTerm ) ) 
		return false;

	return SetAttainProcess( 1 );
}

bool QuestDormantUserCustom::IsCheckAlreadyAttain()
{
	return SetAttainProcess( 1 );   // ������ �Ϸ�
}
//////////////////////////////////////////////////////////////////////////
QuestDevKMove::QuestDevKMove()
{
	m_dwProcessTime = 0;
}

QuestDevKMove::~QuestDevKMove()
{
}

bool QuestDevKMove::IsCheckOccurTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	if( !QuestParent::IsCheckOccurTerm( dwTerm ) ) 
		return false;

	if( g_MyInfo.GetGradeLevel() < GetCustomValue( LIMIT_GRADE ) )
		return false;
	return true;
}

bool QuestDevKMove::IsCheckCompleteTerm( const DWORD dwTerm, int iValue1 /* = 0 */, int iValue2 /* = 0 */, int iValue3 /* = 0  */ )
{
	return false;
}

void QuestDevKMove::Process( ioPlayStage *pPlayStage )
{
	if( !pPlayStage ) 
		return;

	// �Ϸ�üũ�� ���⼭ �Ѵ�.
	if( FRAMEGETTIME() - m_dwProcessTime < 1000 )      // 1�ʸ��� üũ
		return;	
	m_dwProcessTime = FRAMEGETTIME();

	// ����Ʈ ���Ἲ üũ
	if( GetPerformType() == QP_EVENT )
	{
		if( !IsAlive() ) return;
	}

	if( IsPCRoomStyle() )
	{
		if( !g_MyInfo.GetPCRoomAuthority() ) return;
		if( g_MyInfo.IsUserEvent() ) return;
	}

	// �������� ����Ʈ�� üũ
	if( !IsMeProgressQuest() ) return;

	// ���� üũ
	if( pPlayStage->GetModeType() == MT_MYROOM )
	{
		if( ioMyRoomMode::GetMyRoomType() != MRT_PRACTICE )
			return;
	}
	else
	{
		return;
	}

	// ������ K üũ    
	ioBaseChar *pDeveloperChar = pPlayStage->GetBaseChar( Help::GetDeveloperKCharName() );
	if( !pDeveloperChar ) return;
	if( pDeveloperChar->GetState() != CS_DELAY ) return;

	// �Ÿ� üũ
	float fCheckRange = GetCustomValue( POSITION_RANGE ) * GetCustomValue( POSITION_RANGE );
	D3DXVECTOR3 vPos1 = pDeveloperChar->GetWorldPosition();
	D3DXVECTOR3 vPos2( GetCustomValue( POSITION_X ), GetCustomValue( POSITION_Y ), GetCustomValue( POSITION_Z ) );
	D3DXVECTOR3 vDiff = vPos1 - vPos2;
	if( D3DXVec3LengthSq( &vDiff ) < fCheckRange )
	{
		SetAttainProcess( 1 );
	}
}
