#pragma once

#include "../ioComplexStringPrinter.h"

class ioAlarmManager : public Singleton< ioAlarmManager >
{	
protected:
	DWORD m_dwAutoPrintTime;
	DWORD m_dwSlangPrintCount;
	ioHashStringVec m_vGreenMent;

	DWORD m_dwCurGreenMentTime;
	DWORD m_dwCurSlangChatCnt;

protected:
	DWORDVec m_dwGradeUpAlarmList;
	DWORDVec m_dwLevelUpAlarmList;

public:
	void Initialize();

public:
	void EnterStage();
	void PrintGreenMent( bool bSlangCntReset = false );
	void CheckChatSlang( const ioHashString &rkChat );

public:
	void CheckGradeUpMsg( DWORD dwGrade );
	void CheckLevelUpMsg( int iClassType, DWORD dwLevel );

public:
	void ProcessAlarm();

public:
	static ioAlarmManager& GetSingleton();

public:
	ioAlarmManager();
	virtual ~ioAlarmManager();
};

#define g_AlarmMgr ioAlarmManager::GetSingleton()

