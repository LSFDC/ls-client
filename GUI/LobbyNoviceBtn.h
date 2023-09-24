#pragma once

#include "../io3DEngine/ioButton.h"


class LobbyNoviceBtn : public ioButton
{
public:
	enum 
	{
		MAX_TOOLTIP = 3,
		ALPHA_MAX = 255,
	};
	LobbyNoviceBtn();
	virtual ~LobbyNoviceBtn();

	virtual void iwm_create();
	virtual void iwm_show();

	virtual void OnProcess( float fTimePerSec );

	virtual void ParseExtraInfo( ioXMLElement &xElement );
	

	void SetScale(float fScale);
	void SetToolTipText();

protected:

	DWORD m_dwAlphaTime;
	DWORD m_dwAlphaTimeMax;
	int m_nNameOffset;
	ioUIRenderImage * m_pImgAlphaAdd;

};

