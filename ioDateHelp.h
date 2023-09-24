#pragma once

namespace DateHelp
{
	//���� ��¥ ���ϱ�
	int GetTodayOfYear();
	int GetTodayOfMon();
	int GetTodayOfWeek();
	int GetTodayOfDay();
	int GetTodayOfHour();
	int GetTodayOfMin();
	int GetTodayOfSec();

	const CTime& GetCurrTime();

	//DWORD -> DATE ��ȯ
	int GetDateYear( DWORD dwDate );
	int GetDateMonth( DWORD dwDate );
	int GetDateDay( DWORD dwDate );
	int GetDateHour( DWORD dwDate );
	int GetDateMinute( DWORD dwDate );

	//DATE -> DWORD ��ȯ
	DWORD ConvertDwDate( int iYear, int iMonth, int iDay, int iHour, int iMin );
	
	// DWORD -> Ctime ��ȯ
	void ConvertCTime( IN DWORD dwDate, OUT CTime& Time );

	//����/���� ���
	bool isLeapYear( int iYear );
	bool isOddMonth( int iMonth );

	//��¥ ����
	void IncreaseMonth( int& iYear, int& iMonth, int& iDay );
	void DecreaseMonth( int& iYear, int& iMonth, int& iDay );

	void IncreaseDay( int& iYear, int& iMonth, int& iDay );
	void DecreaseDay( int& iYear, int& iMonth, int& iDay );

	void IncreaseMin( int& iYear, int& iMonth, int& iDay, int& iHour, int& iMin, int iIncraseMin );

	//��¥ ���� ���ϱ�
	enum PeriodType
	{
		PT_DAY,
		PT_HOUR,
		PT_MIN,
	};
	int GetDatePeriod( int iStartYear,
		int iStartMonth,
		int iStartDay,
		int iStartHour,
		int iStartMin,
		int iEndYear,
		int iEndMonth,
		int iEndDay,
		int iEndHour,
		int iEndMin,
		PeriodType eType=PT_DAY);


	//togetday
	int GetMonthDayPeriod( int num, int mon );

	// �Է��� ��¥�� ���� ���ϱ� : 0(Sunday) ~
	int GetWeekDay( int iYear, int iMonth, int iDay );
	
	//���� ���ڸ� ��������(0 ~ 6)
	char* weekday( int ky ); //  ���ڷ� ��ȯ �Ѵ�. //�ؿ�
	char* GetWeekDayString( int index );
	char* GetWeekDayString( DWORD dwDate );
	char* GetWeekDayString( int num, int mon, int day );
	
	time_t ConvertSecondTime( int iYear, int iMonth, int iDay, int iHour, int iMinute, int iSecond );
}