
#include "StdAfx.h"

#include "../io3DEngine/ioWnd.h"
#include "../io3DEngine/ioFontManager.h"
#include "../io3DEngine/ioUIRenderImage.h"
#include "../io3DEngine/ioUIImageSetManager.h"
#include "../io3DEngine/ioSoundManager.h"
#include "../io3DEngine/HelpFunc.h"
#include "../io3DEngine/iostringmanager.h"
#include "../io3dengine/safesprintf.h"

#include "../ioApplication.h"
#include "../Setting.h"
#include "../EtcHelpFunc.h"

#include "ioSP2GUIManager.h"
#include "TooltipHelpWnd.h"
#include <strsafe.h>

TooltipHelpWnd::TooltipHelpWnd()
{
	m_iTooltipType	  = TOOLTIP_LEFTTOP;
	m_pPrevTooltipWnd = NULL;

	m_pTopLeft	  = NULL;
	m_pTopEdge	  = NULL;
	m_pTopRight	  = NULL;
	m_pLeftEdge   = NULL;
	m_pCenter	  = NULL;
	m_pRightEdge  = NULL;
	m_pBottomLeft = NULL;
	m_pBottomEdge = NULL;
	m_pBottomRight= NULL;
}

TooltipHelpWnd::~TooltipHelpWnd()
{
	SAFEDELETE( m_pTopLeft );
	SAFEDELETE( m_pTopEdge );
	SAFEDELETE( m_pTopRight );
	SAFEDELETE( m_pLeftEdge );
	SAFEDELETE( m_pCenter );
	SAFEDELETE( m_pRightEdge );
	SAFEDELETE( m_pBottomLeft );
	SAFEDELETE( m_pBottomEdge );
	SAFEDELETE( m_pBottomRight );
}

void TooltipHelpWnd::iwm_show()
{

}

void TooltipHelpWnd::iwm_hide()
{

}

void TooltipHelpWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "TopLeft" )
	{
		SAFEDELETE( m_pTopLeft );
		m_pTopLeft = pImage;
	}
	else if( szType == "TopEdge" )
	{
		SAFEDELETE( m_pTopEdge );
		m_pTopEdge = pImage;
		m_pTopEdge->SetHorzFormatting( ioUIRenderImage::HF_STRETCH );
		m_pTopEdge->SetVertFormatting( ioUIRenderImage::VF_STRETCH );
	}
	else if( szType == "TopRight" )
	{
		SAFEDELETE( m_pTopRight );
		m_pTopRight = pImage;
	}
	else if( szType == "LeftEdge" )
	{
		SAFEDELETE( m_pLeftEdge );
		m_pLeftEdge = pImage;
		m_pLeftEdge->SetHorzFormatting( ioUIRenderImage::HF_STRETCH );
		m_pLeftEdge->SetVertFormatting( ioUIRenderImage::VF_STRETCH );
	}
	else if( szType == "Center" )
	{
		SAFEDELETE( m_pCenter );
		m_pCenter = pImage;
		m_pCenter->SetHorzFormatting( ioUIRenderImage::HF_STRETCH );
		m_pCenter->SetVertFormatting( ioUIRenderImage::VF_STRETCH );
	}
	else if( szType == "RightEdge" )
	{
		SAFEDELETE( m_pRightEdge );
		m_pRightEdge = pImage;
		m_pRightEdge->SetHorzFormatting( ioUIRenderImage::HF_STRETCH );
		m_pRightEdge->SetVertFormatting( ioUIRenderImage::VF_STRETCH );
	}
	else if( szType == "BottomLeft" )
	{
		SAFEDELETE( m_pBottomLeft );
		m_pBottomLeft = pImage;
	}
	else if( szType == "BottomEdge" )
	{
		SAFEDELETE( m_pBottomEdge );
		m_pBottomEdge = pImage;
		m_pBottomEdge->SetHorzFormatting( ioUIRenderImage::HF_STRETCH );
		m_pBottomEdge->SetVertFormatting( ioUIRenderImage::VF_STRETCH );
	}
	else if( szType == "BottomRight" )
	{
		SAFEDELETE( m_pBottomRight );
		m_pBottomRight = pImage;
	}
	else
	{
		ioWnd::AddRenderImage( szType, pImage );
	}
}

bool TooltipHelpWnd::IsResourceNULL()
{
	if( !m_pTopLeft || !m_pTopEdge || !m_pTopRight || !m_pLeftEdge || !m_pCenter || !m_pRightEdge ||
        !m_pBottomLeft || !m_pBottomEdge || !m_pBottomRight )
		return true;
	return false;
}

void TooltipHelpWnd::UpdateTooltipWnd()
{
	if( !g_App.IsMouseRender() )
		return;

	m_pPrevTooltipWnd = g_GUIMgr.GetPreOverWnd();
	if( !m_pPrevTooltipWnd ) return;

	m_TooltipList.clear();
	m_pPrevTooltipWnd->GetTooltipHelp( m_TooltipList );

	if( m_TooltipList.empty() ) return;

	// 
	int iDefaultWidth = 14;          // 양옆 간격
	int iDefaultHeight= 23;          // 위아래 간격
	int iMaxWidth = 0;
	int iTooltipSize = m_TooltipList.size();
	for(int i = 0;i < iTooltipSize;i++)
	{
		Tooltip &kTooltip = m_TooltipList[i];
		int iTempWidth = g_FontMgr.GetTextWidth( kTooltip.m_szHelp.c_str(), TS_OUTLINE_FULL_2X, 0.56f );
		if( iTempWidth > iMaxWidth )
			iMaxWidth = iTempWidth;
	}
	SetSize( iDefaultWidth + iMaxWidth, iDefaultHeight + ( iTooltipSize * 20 ) );
	
	// 좌상단 검사
	m_iTooltipType = TOOLTIP_LEFTTOP;
	SetWndPos( m_pPrevTooltipWnd->GetDerivedPosX(), m_pPrevTooltipWnd->GetDerivedPosY() );
	if( GetDerivedPosX() + GetWidth() <= Setting::Width() && GetDerivedPosY() - GetHeight() >= 0 )
	{
		ShowWnd();
		return;      //검사 성공
	}

	// 우상단 검사
	m_iTooltipType = TOOLTIP_RIGHTTOP;
	SetWndPos( m_pPrevTooltipWnd->GetDerivedPosX() + m_pPrevTooltipWnd->GetWidth(), m_pPrevTooltipWnd->GetDerivedPosY() );
	if( GetDerivedPosX() - GetWidth() >= 0 && GetDerivedPosY() - GetHeight() >= 0 )
	{
		ShowWnd();
		return;      //검사 성공
	}

	// 좌하단 검사
	m_iTooltipType = TOOLTIP_LEFTBOTTOM;
	SetWndPos( m_pPrevTooltipWnd->GetDerivedPosX(), m_pPrevTooltipWnd->GetDerivedPosY() + m_pPrevTooltipWnd->GetHeight() );
	if( GetDerivedPosX() + GetWidth() <= Setting::Width() && GetDerivedPosY() + GetHeight() <= Setting::Height() )
	{
		ShowWnd();
		return;      //검사 성공
	}

	// 우하단 검사
	m_iTooltipType = TOOLTIP_RIGHTBOTTOM;
	SetWndPos( m_pPrevTooltipWnd->GetDerivedPosX() + m_pPrevTooltipWnd->GetWidth(), m_pPrevTooltipWnd->GetDerivedPosY() + m_pPrevTooltipWnd->GetHeight()  );
	if( GetDerivedPosX() - GetWidth() >= 0 && GetDerivedPosY() + GetHeight() <= Setting::Height() )
	{
		ShowWnd();
		return;      //검사 성공
	}
}

void TooltipHelpWnd::OnProcess( float fTimePerSec )
{
	if( IsShow() )
	{
		if( m_pPrevTooltipWnd != g_GUIMgr.GetPreOverWnd() || !g_App.IsMouseRender() )
			HideWnd();
	}
	else
	{
		UpdateTooltipWnd();
	}
}

void TooltipHelpWnd::OnRender()
{
	ioWnd::OnRender();
 
	switch( m_iTooltipType )
	{
	case TOOLTIP_LEFTTOP:
		OnRenderLeftTop( GetDerivedPosX(),GetDerivedPosY(), GetWidth(), GetHeight() );
		break;
	case TOOLTIP_RIGHTTOP:
		OnRenderRightTop( GetDerivedPosX(),GetDerivedPosY(), GetWidth(), GetHeight() );
		break;
	case TOOLTIP_LEFTBOTTOM:
		OnRenderLeftBottom( GetDerivedPosX(),GetDerivedPosY(), GetWidth(), GetHeight() );
		break;
	case TOOLTIP_RIGHTBOTTOM:
		OnRenderRightBottom( GetDerivedPosX(),GetDerivedPosY(), GetWidth(), GetHeight() );
		break;
	}	
}

void TooltipHelpWnd::OnRenderLeftTop( int iAxisXPos, int iAxisYPos, int iWidth, int iHeight )
{
	if( IsResourceNULL() ) return;
	
	// 상단
	m_pTopLeft->SetReverseFormatting( ioUIRenderImage::RF_NONE_REVERSE );
	m_pTopLeft->Render( iAxisXPos, iAxisYPos - iHeight );

	m_pTopEdge->SetReverseFormatting( ioUIRenderImage::RF_NONE_REVERSE );
	m_pTopEdge->SetSize( iWidth - 30, m_pTopEdge->GetHeight() );
	m_pTopEdge->Render( iAxisXPos + 15, iAxisYPos - iHeight );
	
	m_pTopRight->SetReverseFormatting( ioUIRenderImage::RF_NONE_REVERSE );
	m_pTopRight->Render( iAxisXPos + ( iWidth - 15 ), iAxisYPos - iHeight );

	// 중단
	m_pLeftEdge->SetReverseFormatting( ioUIRenderImage::RF_NONE_REVERSE );
	m_pLeftEdge->SetSize( m_pLeftEdge->GetWidth(), iHeight - 30 );
	m_pLeftEdge->Render( iAxisXPos, iAxisYPos - ( iHeight - 15 ) );

	m_pCenter->SetSize( iWidth - 30, iHeight - 30 );
	m_pCenter->Render( iAxisXPos + 15, iAxisYPos - ( iHeight - 15 ) );

	m_pRightEdge->SetReverseFormatting( ioUIRenderImage::RF_NONE_REVERSE );
	m_pRightEdge->SetSize( m_pRightEdge->GetWidth(), iHeight - 30 );
	m_pRightEdge->Render( iAxisXPos + ( iWidth - 15), iAxisYPos - ( iHeight - 15 ) );

	// 하단
	m_pBottomLeft->SetReverseFormatting( ioUIRenderImage::RF_NONE_REVERSE );
	m_pBottomLeft->Render( iAxisXPos, iAxisYPos - 15 );

	m_pBottomEdge->SetReverseFormatting( ioUIRenderImage::RF_NONE_REVERSE );
	m_pBottomEdge->SetSize( iWidth - 30, m_pBottomEdge->GetHeight() );
	m_pBottomEdge->Render( iAxisXPos + 15, iAxisYPos - 15 );

	m_pBottomRight->SetReverseFormatting( ioUIRenderImage::RF_NONE_REVERSE );
	m_pBottomRight->Render( iAxisXPos + ( iWidth - 15 ), iAxisYPos - 15 );

	// 헬프
	OnRenderPrintHelp( iAxisXPos, iAxisYPos - iHeight, iWidth, iHeight );
}

void TooltipHelpWnd::OnRenderRightTop( int iAxisXPos, int iAxisYPos, int iWidth, int iHeight )
{
	if( IsResourceNULL() ) return;

	// 상단
	m_pTopLeft->SetReverseFormatting( ioUIRenderImage::RF_NONE_REVERSE );
	m_pTopLeft->Render( iAxisXPos - iWidth, iAxisYPos - iHeight );

	m_pTopEdge->SetReverseFormatting( ioUIRenderImage::RF_NONE_REVERSE );
	m_pTopEdge->SetSize( iWidth - 30, m_pTopEdge->GetHeight() );
	m_pTopEdge->Render( iAxisXPos - ( iWidth - 15 ), iAxisYPos - iHeight );

	m_pTopRight->SetReverseFormatting( ioUIRenderImage::RF_NONE_REVERSE );
	m_pTopRight->Render( iAxisXPos - 15, iAxisYPos - iHeight );

	// 중단
	m_pRightEdge->SetReverseFormatting( ioUIRenderImage::RF_HORZ_REVERSE );
	m_pRightEdge->SetSize( m_pRightEdge->GetWidth(), iHeight - 30 );
	m_pRightEdge->Render( iAxisXPos - iWidth, iAxisYPos - ( iHeight - 15 ) );

	m_pCenter->SetSize( iWidth - 30, iHeight - 30 );
	m_pCenter->Render( iAxisXPos - ( iWidth - 15 ), iAxisYPos - ( iHeight - 15 ) );

	m_pLeftEdge->SetReverseFormatting( ioUIRenderImage::RF_HORZ_REVERSE );
	m_pLeftEdge->SetSize( m_pLeftEdge->GetWidth(), iHeight - 30 );
	m_pLeftEdge->Render( iAxisXPos - 15, iAxisYPos - ( iHeight - 15 ) );

	// 하단
	m_pBottomRight->SetReverseFormatting( ioUIRenderImage::RF_HORZ_REVERSE );
	m_pBottomRight->Render( iAxisXPos - iWidth, iAxisYPos - 15 );

	m_pBottomEdge->SetReverseFormatting( ioUIRenderImage::RF_HORZ_REVERSE );
	m_pBottomEdge->SetSize( iWidth - 30, m_pBottomEdge->GetHeight() );
	m_pBottomEdge->Render( iAxisXPos - ( iWidth - 15 ), iAxisYPos - 15 );

	m_pBottomLeft->SetReverseFormatting( ioUIRenderImage::RF_HORZ_REVERSE );
	m_pBottomLeft->Render( iAxisXPos - 15, iAxisYPos - 15 );
	
	// 헬프
	OnRenderPrintHelp( iAxisXPos - iWidth, iAxisYPos - iHeight, iWidth, iHeight );
}

void TooltipHelpWnd::OnRenderLeftBottom( int iAxisXPos, int iAxisYPos, int iWidth, int iHeight )
{
	if( IsResourceNULL() ) return;

	// 상단
	m_pBottomLeft->SetReverseFormatting( ioUIRenderImage::RF_VERT_REVERSE );
	m_pBottomLeft->Render( iAxisXPos, iAxisYPos );

	m_pBottomEdge->SetReverseFormatting( ioUIRenderImage::RF_VERT_REVERSE );
	m_pBottomEdge->SetSize( iWidth - 30, m_pBottomEdge->GetHeight() );
	m_pBottomEdge->Render( iAxisXPos + 15, iAxisYPos );

	m_pBottomRight->SetReverseFormatting( ioUIRenderImage::RF_VERT_REVERSE );
	m_pBottomRight->Render( iAxisXPos + ( iWidth - 15 ), iAxisYPos );

	// 중단
	m_pLeftEdge->SetReverseFormatting( ioUIRenderImage::RF_NONE_REVERSE );
	m_pLeftEdge->SetSize( m_pLeftEdge->GetWidth(), iHeight - 30 );
	m_pLeftEdge->Render( iAxisXPos, iAxisYPos + 15 );

	m_pCenter->SetSize( iWidth - 30, iHeight - 30 );
	m_pCenter->Render( iAxisXPos + 15, iAxisYPos + 15 );

	m_pRightEdge->SetReverseFormatting( ioUIRenderImage::RF_NONE_REVERSE );
	m_pRightEdge->SetSize( m_pRightEdge->GetWidth(), iHeight - 30 );
	m_pRightEdge->Render( iAxisXPos + ( iWidth - 15), iAxisYPos + 15 );

	// 하단
	m_pTopLeft->SetReverseFormatting( ioUIRenderImage::RF_VERT_REVERSE );
	m_pTopLeft->Render( iAxisXPos, iAxisYPos + ( iHeight - 15 ) );

	m_pTopEdge->SetReverseFormatting( ioUIRenderImage::RF_VERT_REVERSE );
	m_pTopEdge->SetSize( iWidth - 30, m_pTopEdge->GetHeight() );
	m_pTopEdge->Render( iAxisXPos + 15, iAxisYPos + ( iHeight - 15 ) );	

	m_pTopRight->SetReverseFormatting( ioUIRenderImage::RF_VERT_REVERSE );
	m_pTopRight->Render( iAxisXPos + ( iWidth - 15 ), iAxisYPos + ( iHeight - 15 ) );

	// 헬프
	OnRenderPrintHelp( iAxisXPos, iAxisYPos, iWidth, iHeight );
}

void TooltipHelpWnd::OnRenderRightBottom( int iAxisXPos, int iAxisYPos, int iWidth, int iHeight )
{
	if( IsResourceNULL() ) return;

	// 상단
	m_pBottomRight->SetReverseFormatting( ioUIRenderImage::RF_HORZ_VERT_REVERSE );
	m_pBottomRight->Render( iAxisXPos - iWidth, iAxisYPos );

	m_pBottomEdge->SetReverseFormatting( ioUIRenderImage::RF_HORZ_VERT_REVERSE );
	m_pBottomEdge->SetSize( iWidth - 30, m_pBottomEdge->GetHeight() );
	m_pBottomEdge->Render( iAxisXPos - ( iWidth - 15 ), iAxisYPos );

	m_pBottomLeft->SetReverseFormatting( ioUIRenderImage::RF_HORZ_VERT_REVERSE );
	m_pBottomLeft->Render( iAxisXPos - 15, iAxisYPos );	

	// 중단
	m_pRightEdge->SetReverseFormatting( ioUIRenderImage::RF_HORZ_VERT_REVERSE );
	m_pRightEdge->SetSize( m_pRightEdge->GetWidth(), iHeight - 30 );
	m_pRightEdge->Render( iAxisXPos - iWidth, iAxisYPos + 15 );

	m_pCenter->SetSize( iWidth - 30, iHeight - 30 );
	m_pCenter->Render( iAxisXPos - ( iWidth - 15 ), iAxisYPos + 15 );

	m_pLeftEdge->SetReverseFormatting( ioUIRenderImage::RF_HORZ_VERT_REVERSE );
	m_pLeftEdge->SetSize( m_pLeftEdge->GetWidth(), iHeight - 30 );
	m_pLeftEdge->Render( iAxisXPos - 15, iAxisYPos + 15 );
	
	// 하단
	m_pTopRight->SetReverseFormatting( ioUIRenderImage::RF_HORZ_VERT_REVERSE );
	m_pTopRight->Render( iAxisXPos - iWidth, iAxisYPos + ( iHeight - 15 ) );

	m_pTopEdge->SetReverseFormatting( ioUIRenderImage::RF_HORZ_VERT_REVERSE );
	m_pTopEdge->SetSize( iWidth - 30, m_pTopEdge->GetHeight() );
	m_pTopEdge->Render( iAxisXPos - ( iWidth - 15 ), iAxisYPos + ( iHeight - 15 ) );	

	m_pTopLeft->SetReverseFormatting( ioUIRenderImage::RF_HORZ_VERT_REVERSE );
	m_pTopLeft->Render( iAxisXPos - 15, iAxisYPos + ( iHeight - 15 ) );	

	// 헬프
	OnRenderPrintHelp( iAxisXPos - iWidth, iAxisYPos, iWidth, iHeight );
}

void TooltipHelpWnd::OnRenderPrintHelp( int iXPos, int iYPos, int iWidth, int iHeight )
{
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
	
	int iSize = m_TooltipList.size();
	for(int i = 0;i < iSize;i++)
	{
		Tooltip &kTooltip = m_TooltipList[i];
		g_FontMgr.SetTextColor( kTooltip.m_dwTextColor );
		g_FontMgr.SetAlignType( kTooltip.m_TextAlign );
		switch( kTooltip.m_TextAlign )
		{
		case TAT_CENTER:
			g_FontMgr.PrintText( iXPos + ( iWidth / 2 ), iYPos + 11 + ( i * 20 ), 0.56f, kTooltip.m_szHelp.c_str() );
			break;
		case TAT_RIGHT:
			g_FontMgr.PrintText( iXPos + iWidth - 7, iYPos + 11 + ( i * 20 ), 0.56f, kTooltip.m_szHelp.c_str() );
			break;
		default:
			g_FontMgr.PrintText( iXPos + 7, iYPos + 11 + ( i * 20 ), 0.56f, kTooltip.m_szHelp.c_str() );
			break;
		}
	}
}
//////////////////////////////////////////////////////////////////////////
TooltipGuildPointHelpWnd::TooltipGuildPointHelpWnd()
{
}

TooltipGuildPointHelpWnd::~TooltipGuildPointHelpWnd()
{
}

//void TooltipGuildPointHelpWnd::OnProcess( float fTimePerSec )
//{
//	if( !IsShow() ) return;
//}

void TooltipGuildPointHelpWnd::UpdateTooltipGuildPoint( int iXPos, int iYPos, int iCurGuildPoint )
{
	m_TooltipList.clear();

	char szHelp[MAX_PATH] = "";
	SafeSprintf( szHelp, sizeof( szHelp ), STR(1), iCurGuildPoint );
	Tooltip kTooltip;
	kTooltip.m_dwTextColor = 0xFFFF8200;
	kTooltip.m_szHelp      = szHelp;
	kTooltip.m_TextAlign   = TAT_LEFT;
	m_TooltipList.push_back( kTooltip );

	// 
	int iDefaultWidth = 14;          // 양옆 간격
	int iDefaultHeight= 23;          // 위아래 간격
	int iMaxWidth = 0;
	int iTooltipSize = m_TooltipList.size();
	for(int i = 0;i < iTooltipSize;i++)
	{
		Tooltip &kTooltip = m_TooltipList[i];
		int iTempWidth = g_FontMgr.GetTextWidth( kTooltip.m_szHelp.c_str(), TS_OUTLINE_FULL_2X, 0.56f );
		if( iTempWidth > iMaxWidth )
			iMaxWidth = iTempWidth;
	}
	SetSize( iDefaultWidth + iMaxWidth, iDefaultHeight + ( iTooltipSize * 20 ) );

	m_iTooltipType = TOOLTIP_LEFTTOP;
	SetWndPos( iXPos, iYPos );
	ShowWnd();
}