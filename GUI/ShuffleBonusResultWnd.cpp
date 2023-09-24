

#include "StdAfx.h"

#include "../ioApplication.h"
#include "../WndID.h"
#include "../Setting.h"
#include "../NetworkWrappingFunc.h"
#include "../TextColorTable.h"
#include "../PlayEntityHelpStruct.h"
#include "../EtcHelpFunc.h"

#include "ioSP2GUIManager.h"
#include "ShuffleBonusResultWnd.h"

#include "../ioShuffleRoomMgr.h"

#include <strsafe.h>

/* �� UI�� ������� ����... */

ShuffleBonusResultWnd::ShuffleBonusResultWnd()
{
	m_pStarIcon = NULL;
	m_pCross    = NULL;
	m_pNumber   = NULL;
}

ShuffleBonusResultWnd::~ShuffleBonusResultWnd()
{
	SAFEDELETE( m_pStarIcon );
	SAFEDELETE( m_pCross );
	SAFEDELETE( m_pNumber );
}

void ShuffleBonusResultWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "StarIcon" )
	{
		SAFEDELETE( m_pStarIcon );
		m_pStarIcon = pImage;
	}
	else if( szType == "Cross" )
	{
		SAFEDELETE( m_pCross );
		m_pCross = pImage;
	}
	else if( szType == "Number" )
	{
		SAFEDELETE( m_pNumber );
		m_pNumber = pImage;
	}
	else
	{
		ioWnd::AddRenderImage( szType, pImage );
	}
}

void ShuffleBonusResultWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;

	ioWnd::OnProcess( fTimePerSec );
}

void ShuffleBonusResultWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_CONTINUE:
		if( cmd == IOBN_BTNUP )
		{
			ioButton *pBtn = (ioButton*)( FindChildWnd( ID_CONTINUE ) );
			if( pBtn )
			{
				pBtn->SetTitleText( STR(1) );
				pBtn->SetInActive();
			}
		}
		break;
	case ID_LOBBY:
		if( cmd == IOBN_BTNUP )
		{
			// �κ�� �̵�
			SP2Packet kPacket( CTPK_EXIT_ROOM );
			kPacket << EXIT_ROOM_LOBBY << 0 << true;
			TCPNetwork::SendToServer( kPacket );
			TCPNetwork::MouseBusy( true );
		}
		break;
	}
}

void ShuffleBonusResultWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	ioComplexStringPrinter kPrinter;
	kPrinter.SetTextStyle( TS_NORMAL );
	kPrinter.SetBkColor( TCT_DEFAULT_DARKGRAY );
	kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter.AddTextPiece( FONT_SIZE_17, STR(1) );
	kPrinter.PrintFullTextWidthCut( iXPos + 166, iYPos + 124, TAT_CENTER, 255.0f );
	kPrinter.ClearList();

	kPrinter.SetTextColor( TCT_DEFAULT_BLUE );
	kPrinter.AddTextPiece( FONT_SIZE_17, STR(2), g_ShuffleRoomMgr.GetShuffleStarCnt() );
	kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter.AddTextPiece( FONT_SIZE_17, STR(3) );
	kPrinter.PrintFullTextWidthCut( iXPos + 166, iYPos + 147, TAT_CENTER, 255.0f );
	kPrinter.ClearList();

	kPrinter.SetTextColor( TCT_DEFAULT_DARKGRAY );
	kPrinter.AddTextPiece( FONT_SIZE_13, STR(4) );
	kPrinter.PrintFullTextWidthCut( iXPos + 166, iYPos + 177, TAT_CENTER, 255.0f );
	kPrinter.ClearList();

	kPrinter.AddTextPiece( FONT_SIZE_13, STR(5) );
	kPrinter.PrintFullTextWidthCut( iXPos + 166, iYPos + 195, TAT_CENTER, 255.0f );
	kPrinter.ClearList();

	kPrinter.AddTextPiece( FONT_SIZE_13, STR(6) );
	kPrinter.PrintFullTextWidthCut( iXPos + 166, iYPos + 213, TAT_CENTER, 255.0f );
	kPrinter.ClearList();

	RenderStarCount( iXPos + 163, iYPos + 63 );
}

void ShuffleBonusResultWnd::RenderStarCount( int iXPos, int iYPos )
{
	if( !m_pStarIcon && !m_pCross && !m_pNumber )
		return;

	int iStarWidth = m_pStarIcon->GetWidth();
	int iCrossWidth = m_pCross->GetWidth();
	int iNumberWidth = m_pNumber->GetWidth() / 10;

	int iStarCnt = g_ShuffleRoomMgr.GetShuffleStarCnt();

	if( iStarCnt < 10 )
		iNumberWidth *= 1;
	else if( iStarCnt < FLOAT100 )
		iNumberWidth *= 2;
	else if( iStarCnt < 1000 )
		iNumberWidth *= 3;

	int iXOffset = iXPos - ( ( iStarWidth + iCrossWidth + iNumberWidth - 25 ) / 2 );

	m_pStarIcon->Render( iXOffset, iYPos, UI_RENDER_NORMAL, TFO_BILINEAR );

	iXOffset += iStarWidth - 24;
	m_pCross->Render( iXOffset, iYPos + 24 );

	iXOffset += iCrossWidth - 1;
	m_pNumber->RenderNum( iXOffset, iYPos + 16, iStarCnt, -FLOAT1 );
}