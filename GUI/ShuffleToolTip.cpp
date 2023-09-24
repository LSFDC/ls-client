
#include "StdAfx.h"
#include "ioRichLabel.h"

#include "ShuffleToolTip.h"
#include "../ioShuffleRoomMgr.h"

ShuffleToolTip::ShuffleToolTip()
{
}

ShuffleToolTip::~ShuffleToolTip()
{

}

void ShuffleToolTip::iwm_show()
{
}

void ShuffleToolTip::iwm_hide()
{
}

void ShuffleToolTip::OnProcess( float fTimePerSec )
{
	if( !IsShow() )
		return;

	ioLSMouse *pMouse = g_App.GetMouse();
	POINT pt = pMouse->GetMousePos();
	int iXPos = pt.x + 8;
	int iYPos = pt.y + 8;

	int iXOffset = pt.x + GetWidth() + 8;
	if( Setting::Width() < iXOffset )
	{
		iXPos -= ( iXOffset - Setting::Width() ) + 8;
	}

	int iYOffset = pt.y + GetHeight() + 8;
	if( Setting::Height() < iYOffset )
	{
		iYPos -= ( iYOffset - Setting::Height() ) + 8;
	}

	SetScreenWndPos( iXPos, iYPos );
	pMouse->SetToolTipCursor();

	int iDayOfWeek = DEFAULT;
#ifdef SHIPPING	
	SYSTEMTIME rkServerTime;
	g_MyInfo.GetServerTime( rkServerTime );	
	iDayOfWeek = rkServerTime.wDayOfWeek;
#else
	SYSTEMTIME systime;
	GetSystemTime(&systime);
	iDayOfWeek = systime.wDayOfWeek;
#endif

	int i = 0;
	for( int iDay = SUN_; iDay <= DEFAULT; ++iDay, ++i )
	{
		ioRichLabel* pLabel = (ioRichLabel*)( FindChildWnd( iDay ) );
		if( pLabel )
		{
			if( iDayOfWeek == i )
			{
				pLabel->ShowWnd();
				SetSize( pLabel->GetWidth(), pLabel->GetTextHeight() + 3 );
			}
			else
			{
				pLabel->HideWnd();
			}
		}
	}
}