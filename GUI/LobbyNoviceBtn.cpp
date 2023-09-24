

#include "StdAfx.h"

#include "../io3DEngine/ioWnd.h"
#include "../io3DEngine/ioFontManager.h"
#include "../io3DEngine/ioUIRenderer.h"
#include "../io3DEngine/ioUIImageSetManager.h"
#include "../io3DEngine/ioUIRenderImage.h"
#include "../io3DEngine/ioUIRenderFrame.h"
#include "../io3DEngine\ioXMLElement.h"
#include "../io3DEngine/ioXMLDocument.h"
#include "GlobalWnd.h"

#include "LobbyNoviceBtn.h"





void LobbyNoviceBtn::iwm_show()
{
	m_dwAlphaTime = FRAMEGETTIME();
}

void LobbyNoviceBtn::iwm_create()
{
	ioUIRenderImage * pImg = m_NormalList.back();

	if(pImg)
	{
		SAFEDELETE(m_pImgAlphaAdd);
		m_pImgAlphaAdd = (ioUIRenderImage *)pImg->Clone();
		m_pImgAlphaAdd->SetRenderType(UI_RENDER_ADD);
		AddRenderImage("NormalList", m_pImgAlphaAdd);
	}

}

void LobbyNoviceBtn::OnProcess( float fTimePerSec )
{
	if(m_pImgAlphaAdd)
	{
		DWORD curTime = FRAMEGETTIME();
		DWORD elapsedTime = curTime - m_dwAlphaTime;
		if(elapsedTime > m_dwAlphaTimeMax)
		{
			m_dwAlphaTime = curTime;
			elapsedTime = curTime - m_dwAlphaTime;
		}
		float alpha = (float)elapsedTime / (float)m_dwAlphaTimeMax;
		float alphaVar = sin(alpha * D3DX_PI);
		m_pImgAlphaAdd->SetAlpha(alphaVar * ALPHA_MAX);
	}

}

LobbyNoviceBtn::LobbyNoviceBtn()
{
	m_dwAlphaTimeMax = 1000;
	m_nNameOffset = 0;
	m_pImgAlphaAdd = NULL;

}

LobbyNoviceBtn::~LobbyNoviceBtn()
{
}

void LobbyNoviceBtn::ParseExtraInfo( ioXMLElement &xElement )
{
	m_nNameOffset = xElement.GetIntAttribute( "NameOffset" );
	m_dwAlphaTimeMax = xElement.GetIntAttribute( "AlphaTime" );
}

