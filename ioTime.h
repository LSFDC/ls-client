#pragma once
#include "StdAfx.h"

namespace ioTime
{
	enum
	{
		DEFAULT_YEAR    = 2010,			// 2010���� DB�� �������� �ʴ´�. �� DateData�� �⵵�� 0�̸� 2010�̶� ���̴�. 1�̸� 2011��
		DATE_YEAR_VALUE = 100000000,    // ����� ������.
		DATE_MONTH_VALUE= 1000000,      // ������ ������.
		DATE_DAY_VALUE =  10000,        // �ϱ��� ������.
		DATE_HOUR_VALUE = 100,          // �ñ��� ������.
	};

	DWORD GetYear( DWORD dwDate );
	DWORD GetMonth( DWORD dwDate );
	DWORD GetDay( DWORD dwDate );
	DWORD GetHour( DWORD dwDate );
	DWORD GetMinute( DWORD dwDate );
}