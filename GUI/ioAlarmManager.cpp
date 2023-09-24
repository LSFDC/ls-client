

#include "StdAfx.h"


#include "../io3DEngine/ioINILoader.h"

#include "../NetworkWrappingFunc.h"
#include "../ioMyInfo.h"
#include "../ChatMessageFunc.h"
#include "../SlangUtil.h"


#include "ioAlarmManager.h"

#include <strsafe.h>
template<> ioAlarmManager* Singleton< ioAlarmManager >::ms_Singleton = 0;
ioAlarmManager::ioAlarmManager()
{
	m_dwAutoPrintTime   = 0;
	m_dwSlangPrintCount = 0;	
	m_dwCurGreenMentTime= 0;
	m_dwCurSlangChatCnt = 0;
}

ioAlarmManager::~ioAlarmManager()
{
}

ioAlarmManager& ioAlarmManager::GetSingleton()
{
	return Singleton< ioAlarmManager >::GetSingleton();
}

void ioAlarmManager::Initialize()
{	
	ioINILoader_e kLoader( "config/sp2_alarm_info.ini" );

	{   //그린슬로건
		m_vGreenMent.clear();
		kLoader.SetTitle_e( "GreenInfo" );	
		m_dwAutoPrintTime   = kLoader.LoadInt_e( "AutoPrintTime", 0 );
		m_dwSlangPrintCount = kLoader.LoadInt_e( "SlangPrintCount", 0 );
		int iMaxGreenMent   = kLoader.LoadInt_e( "MaxGreenMent", 0 );	
		for(int i = 0;i < iMaxGreenMent;i++)
		{
			char szKey[MAX_PATH] = "";
			char szBuf[MAX_PATH] = "";
			sprintf_e( szKey, "GreenMent_%d", i + 1 );
			kLoader.LoadString( szKey, "", szBuf, MAX_PATH );
			m_vGreenMent.push_back( szBuf );
		}
	}

	{   //계급업 전체 알림
		m_dwGradeUpAlarmList.clear();
		kLoader.SetTitle_e( "GradeUpInfo" );	
		int iMaxGradeUp = kLoader.LoadInt_e( "MaxGrade", 0 );
		for(int i = 0;i < iMaxGradeUp;i++)
		{
			char szKey[MAX_PATH] = "";
			sprintf_e( szKey, "Grade_%d", i + 1 );
			m_dwGradeUpAlarmList.push_back( kLoader.LoadInt( szKey, 0 ) );
		}
	}

	{   //레벨업 전체 알림
		m_dwLevelUpAlarmList.clear();
		kLoader.SetTitle_e( "LevelUpInfo" );	
		int iMaxLevelUp = kLoader.LoadInt_e( "MaxLevel", 0 );
		for(int i = 0;i < iMaxLevelUp;i++)
		{
			char szKey[MAX_PATH] = "";
			sprintf_e( szKey, "Level_%d", i + 1 );
			m_dwLevelUpAlarmList.push_back( kLoader.LoadInt( szKey, 0 ) );
		}
	}
}

void ioAlarmManager::EnterStage()
{
	m_dwCurGreenMentTime = 0;
	m_dwCurSlangChatCnt  = 0;
}

void ioAlarmManager::PrintGreenMent( bool bSlangCntReset /* = false  */ )
{
	if( m_vGreenMent.empty() ) return;

	int r = rand()%m_vGreenMent.size();
	g_ChatMgr.SetSystemMsg( m_vGreenMent[r].c_str() );
	m_dwCurGreenMentTime= REALGETTIME();

	if( bSlangCntReset )
	{
		m_dwCurSlangChatCnt = 0;
	}
}

void ioAlarmManager::CheckChatSlang( const ioHashString &rkChat )
{
	char szChatBuf[MAX_PATH] = "";
	StringCbCopy( szChatBuf, MAX_PATH, g_SlangUtil.ConvertStringWithPreLine( rkChat.c_str() ) );
	if( strcmp(rkChat.c_str(), szChatBuf) == 0 ) return;
	
	if( ++m_dwCurSlangChatCnt >= m_dwSlangPrintCount )
	{
		PrintGreenMent( true );		
	}
}

void ioAlarmManager::CheckGradeUpMsg( DWORD dwGrade )
{
	int iCheckSize = m_dwGradeUpAlarmList.size();
	for(int i = 0;i < iCheckSize;i++)
	{
		DWORD &rkGrade = m_dwGradeUpAlarmList[i];
		if( rkGrade == dwGrade )
		{
			// 서버로 알람 정보 전송
			SP2Packet kPacket( CTPK_SERVER_ALARM_MSG );
			kPacket << UDP_SERVER_ALARM_GRADE_UP << g_MyInfo.GetPublicID() << dwGrade;
			TCPNetwork::SendToServer( kPacket );
			return;
		}
	}
}

void ioAlarmManager::CheckLevelUpMsg( int iClassType, DWORD dwLevel )
{
	int iCheckSize = m_dwLevelUpAlarmList.size();
	for(int i = 0;i < iCheckSize;i++)
	{
		DWORD &rkLevel = m_dwLevelUpAlarmList[i];
		if( rkLevel == dwLevel )
		{
			// 서버로 알람 정보 전송
			SP2Packet kPacket( CTPK_SERVER_ALARM_MSG );
			kPacket << UDP_SERVER_ALARM_LEVEL_UP << g_MyInfo.GetPublicID() << iClassType << dwLevel << static_cast<byte>( g_MyInfo.GetCharPowerUpGradeByClassType( iClassType ) );
			TCPNetwork::SendToServer( kPacket );
			return;
		}
	}
}

void ioAlarmManager::ProcessAlarm()
{
	if( m_dwCurGreenMentTime == 0 || REALGETTIME() - m_dwCurGreenMentTime > m_dwAutoPrintTime )
	{
		PrintGreenMent();
	}
}