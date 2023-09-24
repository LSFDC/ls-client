#pragma once

#include "../ioComplexStringPrinter.h"

class ShuffleToolTip : public ioWnd
{
	enum
	{
		SUN_    = 1,
		MON_    = 2,
		TUE_    = 3,
		WED_    = 4,
		THU_    = 5,
		FRI_    = 6,
		SAT_    = 7,
		DEFAULT = 8, //날짜를 못찾을때 출력
	};

public:
	virtual void iwm_show();	
	virtual void iwm_hide();

protected:
	virtual void OnProcess( float fTimePerSec );

public:
	ShuffleToolTip();
	virtual ~ShuffleToolTip();
};
