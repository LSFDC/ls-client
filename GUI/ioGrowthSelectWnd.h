#pragma once

#include "../io3DEngine/ioStringConverter.h"
#include "../io3DEngine/ioButton.h"

#include "../ioComplexStringPrinter.h"
#include "ioMessageBox.h"

class ioUIRenderImage;
class ioXMLElement;

enum
{
	MAIN_DESC_LINE	= 12,
	SUB_DESC_LINE	= 3,
};

enum GrowthSubType
{
	TIME_GROWTH_TYPE	= 1,
	PESO_GROWTH_TYPE	= 2,
};

class ioGrowthSubWnd : public ioButton
{
public:
	enum
	{
		ID_TIME_START	= 1,
		ID_TIME_CANCEL	= 2,
		ID_PESO			= 3,
	};

protected:
	GrowthSubType m_SubType;

	ioUIRenderImage* m_pClockGreenImg;
	ioUIRenderImage* m_pClockRedImg;
	ioUIRenderImage* m_pPointImg;
	ioUIRenderImage* m_pArrowImg;

	ioComplexStringPrinter m_GrowthDesc[SUB_DESC_LINE];

	bool m_bTimeGrowth;

	int m_iClassType;

	int m_iTimeSlot;
	int m_iGrowthInfoNum;

	int m_iCurLevel;
	int m_iClassLevel;
	int m_iReinforce;

	int m_iNeedPoint;
	__int64 m_iNeedPeso;

	ioHashString m_szRemainTimeText;

protected:
	void ClearList();
	void UpdateTimeGrowthInfo();

	void ActiveRender();
	void InActiveRender();

public:
	void SetInfo( int iClassType, int iSlot, int iCurLevel, int iGrowthInfoNum, GrowthSubType eType );

public:
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();
	virtual void OnRenderAfterChild();

public:
	ioGrowthSubWnd();
	virtual ~ioGrowthSubWnd();
};

////////////////////////////////////////////////////////////////////////////////
class ioGrowthSelectWnd : public ioWnd
{
public:
	enum
	{
		ID_TIME_GROWTH_WND	= 1,
		ID_PESO_GROWTH_WND	= 2,
		ID_ICON_BTN			= 3,

		ID_X	= 11,
	};

protected:
	ioComplexStringPrinter m_DescList[MAIN_DESC_LINE];

	ioUIRenderImage *m_pGaugeBigBack;
	ioUIRenderImage *m_pGaugeBack;
	ioUIRenderImage *m_pGaugeLevel;
	ioUIRenderImage *m_pGaugeReinforce;
	ioUIRenderImage *m_pTriangle;

	ioUIRenderImage *m_pPointImg;

	ioWnd *m_pPrevWnd;

	DWORD m_dwCurSelectBtn;

	bool m_bTimeGrowth;
	int m_iClassType;
	int m_iTimeSlot;

	int m_iClassLevel;
	int m_iCurLevel;
	int m_iReinforce;
	int m_iReinforceMedal;

	int m_iGrowthInfoNum;

	int m_iCurPoint;
	ioHashString m_szSlotName;
	ioHashString m_szGrowthName;

	bool m_bBaseGrowthType;

protected:
	void ClearList();

	void RenderGauge( int iXPos, int iYPos );

	void CheckGrowthUp();
	void CheckTimeGrowthUp();

	bool EnableCheck();

public:
	void SetGrowthSelectInfo( const ioHashString &szIcon,
							  const ioHashString &szSlotName,
							  const ioHashString &szGrowthName,
							  int iClassType,
							  int iSlot,
							  int iCurLevel,
							  int iGrowthInfoNum,
							  ioWnd *pPreWnd );

	void UpdateInfo();

public:
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );
	virtual bool iwm_esc();
	virtual void iwm_show();
	virtual void iwm_hide();

	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );

protected:
	virtual void OnRender();

public:
	ioGrowthSelectWnd();
	virtual ~ioGrowthSelectWnd();
};

////////////////////////////////////////////////////////////////////////////////////////////////

