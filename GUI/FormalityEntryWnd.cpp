#include "StdAfx.h"

#include "../io3DEngine/ioWnd.h"
#include "../io3DEngine/ioFontManager.h"
#include "../io3DEngine/iostringmanager.h"

#include "../ioApplication.h"
#include "../TextColorTable.h"

#include "FormalityEntryWnd.h"
#include "../TextColorTable.h"

FormalityEntryWnd::FormalityEntryWnd(void)
{
}

FormalityEntryWnd::~FormalityEntryWnd(void)
{
}

void FormalityEntryWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	/*
	    해당 컨텐츠는 [회원확인]을 완료한
        유저만 이용할 수 있습니다.
        지금 회원확인을 하시겠습니까?
        [회원확인]  [ 취  소 ]
	*/
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
	
	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
	g_FontMgr.PrintText( iXPos + 117, iYPos + 42,  FONT_SIZE_13, STR(1));
	g_FontMgr.PrintText( iXPos + 117, iYPos + 62,  FONT_SIZE_13, STR(2));
	g_FontMgr.PrintText( iXPos + 117, iYPos + 82, FONT_SIZE_13,  STR(3));
}

void FormalityEntryWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return ;
	ioMovingWnd::OnProcess( fTimePerSec );
}

void FormalityEntryWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_EXIT:
	case ID_CANCEL:
		if( cmd == IOBN_BTNUP )
			HideWnd();
		break;
	case ID_FORMALITY_ENTRY:
		if( cmd == IOBN_BTNUP)
		{
			g_App.OpenFormalityEntryPage();
			HideWnd();
		}
		break;
	}
}