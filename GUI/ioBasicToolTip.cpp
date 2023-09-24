
#include "StdAfx.h"
#include "ioRichLabel.h"

#include "ioBasicToolTip.h"
#include "../ioShuffleRoomMgr.h"

ioBasicToolTip::ioBasicToolTip()
{
}

ioBasicToolTip::~ioBasicToolTip()
{

}

void ioBasicToolTip::iwm_show()
{
}

void ioBasicToolTip::iwm_hide()
{
}

void ioBasicToolTip::OnProcess( float fTimePerSec )
{
	if( !IsShow() )
		return;
		
	ioLSMouse *pMouse = g_App.GetMouse();
	if ( !pMouse )
		return;

	int iHeight = GetHeight();
	for( ioWndList::iterator iter = m_ChildList.begin(); iter != m_ChildList.end(); ++iter )
	{
		ioRichLabel* pLabel = (ioRichLabel*)( *iter );
		if( pLabel )
		{
			iHeight = pLabel->GetTextHeight() + 8;
		}
	}
	SetSize( GetWidth(), iHeight );
	
	//������ ũ�Ⱑ ��ü ȭ���� ũ�⸦ ����� ���콺 ��ġ�� ���� ���ο� ���ͼ� ������ �������� ��Ŀ���� �Ҿ���� ������ �߻���.
	//������ �����ٰ� �Ⱥ����ٰ� ���°� ����Ǿ� ���콺 ��ġ�� ���� ��ġ ������

	//���� �ڵ�
	/*	
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
	*/

	int iOffSetXY = 8;
	POINT pt = pMouse->GetMousePos();
	int iXPos = pt.x;
	int iYPos = pt.y;

	if( iXPos + iOffSetXY + GetWidth() > Setting::Width() )
		iXPos -= ( GetWidth() + iOffSetXY );
	else 
		iXPos += iOffSetXY;

	if( iYPos + iOffSetXY + GetHeight() > Setting::Height() - 45 )
		iYPos -= ( GetHeight() + iOffSetXY );
	else 
		iYPos += iOffSetXY;
	
	SetScreenWndPos( iXPos, iYPos );
	pMouse->SetToolTipCursor();
}