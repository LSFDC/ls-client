#pragma once

class ioUIRenderImage;
class TooltipHelpWnd : public ioWnd
{
public:
	enum
	{
		TOOLTIP_LEFTTOP		= 1,    //좌상단
		TOOLTIP_RIGHTTOP	= 2,    //우상단
		TOOLTIP_LEFTBOTTOM	= 3,    //좌하단
		TOOLTIP_RIGHTBOTTOM	= 4,    //우하단
	};

protected:
	ioUIRenderImage *m_pTopLeft;
	ioUIRenderImage *m_pTopEdge;
	ioUIRenderImage *m_pTopRight;

	ioUIRenderImage *m_pLeftEdge;
	ioUIRenderImage *m_pCenter;
	ioUIRenderImage *m_pRightEdge;

	ioUIRenderImage *m_pBottomLeft;
	ioUIRenderImage *m_pBottomEdge;
	ioUIRenderImage *m_pBottomRight;

	int    m_iTooltipType;
	ioWnd *m_pPrevTooltipWnd;

	bool IsResourceNULL();
public:
	virtual void iwm_show();
	virtual void iwm_hide();

public:
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();
	void OnRenderLeftTop( int iAxisXPos, int iAxisYPos, int iWidth, int iHeight );
	void OnRenderRightTop( int iAxisXPos, int iAxisYPos, int iWidth, int iHeight );
	void OnRenderLeftBottom( int iAxisXPos, int iAxisYPos, int iWidth, int iHeight );
	void OnRenderRightBottom( int iAxisXPos, int iAxisYPos, int iWidth, int iHeight );
	void OnRenderPrintHelp( int iXPos, int iYPos, int iWidth, int iHeight );

protected:
	void UpdateTooltipWnd();

public:
	TooltipHelpWnd();
	virtual ~TooltipHelpWnd();
};
//////////////////////////////////////////////////////////////////////////
class TooltipGuildPointHelpWnd : public TooltipHelpWnd
{

protected:
	//virtual void OnProcess( float fTimePerSec );

public:
	void UpdateTooltipGuildPoint( int iXPos, int iYPos, int iCurGuildPoint );

public:
	TooltipGuildPointHelpWnd();
	virtual ~TooltipGuildPointHelpWnd();
};

