#include "StdAfx.h"
#include "../ioSoldierAwakeManager.h"
#include "ioAwakeReleaseWnd.h"

ioAwakeReleaseWnd::ioAwakeReleaseWnd()
{
	m_pIconBack		 = NULL;
	m_pSoliderIcon	 = NULL;

	m_dwHideDelayTime = 0;
	m_nGradeType = 0;
}

ioAwakeReleaseWnd::~ioAwakeReleaseWnd()
{
	SAFEDELETE( m_pIconBack );
	SAFEDELETE( m_pSoliderIcon );
}

void ioAwakeReleaseWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "IconBack" )
	{
		SAFEDELETE( m_pIconBack );
		m_pIconBack = pImage;
	}
	else if( szType == "SoliderIcon" )
	{
		SAFEDELETE( m_pSoliderIcon );
		m_pSoliderIcon = pImage;
	}
	else
		ioWnd::AddRenderImage( szType, pImage );
}

void ioAwakeReleaseWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() )
	{
		if( FRAMEGETTIME() - m_dwHideDelayTime < FLOAT100 )
			return;

		if( g_AwakeMgr.IsReserveDeleteAwake() )
		{
			ShowWnd();
		}
		return;
	}

	ioMovingWnd::OnProcess( fTimePerSec );
}

void ioAwakeReleaseWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	if( m_pIconBack )
		m_pIconBack->Render( iXPos, iYPos, UI_RENDER_MULTIPLY );

	g_UIImageRenderMgr.RenderGradeIconBack( m_nGradeType, iXPos + 103, iYPos + 98 );

	if( m_pSoliderIcon )
		m_pSoliderIcon->Render( iXPos + 103, iYPos + 98 );

	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextColor( m_dwAwakeColor );
	g_FontMgr.PrintText( iXPos + 103, iYPos + 144, FONT_SIZE_13, STR(1), m_szClassName.c_str() ); 

	g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
	g_FontMgr.PrintText( iXPos + 103, iYPos + 162, FONT_SIZE_13, STR(2), m_szAwakeName.c_str() ); 

	g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
	g_FontMgr.PrintText( iXPos + 103, iYPos + 180, FONT_SIZE_13, STR(3) ); 	
}

void ioAwakeReleaseWnd::iwm_show()
{
	SAFEDELETE( m_pSoliderIcon );
	m_szAwakeName.clear();	
	m_szIconName.clear();
	m_szClassName.clear();
	m_dwAwakeColor = TCT_DEFAULT_DARKGRAY;

	int nClassType = 0;
	g_AwakeMgr.GetFirstDeleteAwake( m_szClassName, m_szIconName, m_szAwakeName, m_dwAwakeColor, nClassType );
	m_pSoliderIcon = g_UIImageSetMgr.CreateImageByFullName( m_szIconName.c_str() );

	m_nGradeType = g_UIImageRenderMgr.GetPowerUpGradeType( nClassType );

	char szBuf[MAX_PATH];
	SafeSprintf( szBuf, sizeof(szBuf), STR(1), m_szAwakeName.c_str() );
	SetTitleText( szBuf );
}

void ioAwakeReleaseWnd::iwm_hide()
{
	m_szIconName.clear();
	m_szAwakeName.clear();

	m_dwHideDelayTime = FRAMEGETTIME();
}

bool ioAwakeReleaseWnd::iwm_esc()
{
	if( !IsShow() ) return false;
	if( ioWnd::iwm_esc() )
	{
		g_AwakeMgr.EraseDeleteExtraAwake( false );
		return true;
	}
	return false;
}

void ioAwakeReleaseWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();

	switch( dwID )
	{
	case ID_ALL_OK:
		if( cmd == IOBN_BTNUP )
		{
			g_AwakeMgr.EraseDeleteExtraAwake( true );
			HideWnd();
		}
		break;
	case ID_X:
	case ID_OK:
		if( cmd == IOBN_BTNUP )
		{
			g_AwakeMgr.EraseDeleteExtraAwake( false );
			HideWnd();
		}
		break;
	}
}