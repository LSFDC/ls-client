#pragma once

#include "../io3DEngine/ioMovingWnd.h"

class FormalityEntryWnd : public ioMovingWnd
{
public:
	enum
	{
		ID_EXIT             = 1,
		ID_FORMALITY_ENTRY  = 2,
		ID_CANCEL           = 3, 
	};

public:
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

protected:
	virtual void OnRender();
	virtual void OnProcess( float fTimePerSec );

public:
	FormalityEntryWnd(void);
	virtual ~FormalityEntryWnd(void);
};
