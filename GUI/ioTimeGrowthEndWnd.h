#pragma once

#include "../ioComplexStringPrinter.h"
#include "../io3DEngine/ioStringConverter.h"
#include "../io3DEngine/ioButton.h"

#include "../io3DEngine/ioMovingWnd.h"

#include "ioMessageBox.h"

class ioUIRenderImage;

//////////////////////////////////////////////////////////////////////
struct TimeEndInfo
{
	int m_iClassType;

	ioHashString m_ClassName;
	ioHashString m_IconName;
	ioHashString m_GrowthName;
	int m_iLevel;

	bool m_bConfirm;

	TimeEndInfo()
	{
		m_ClassName.Clear();
		m_IconName.Clear();
		m_GrowthName.Clear();

		m_iClassType = 0;
		m_iLevel = 0;

		m_bConfirm = false;
	}
};

typedef std::vector< TimeEndInfo > TimeEndInfoList;

//////////////////////////////////////////////////////////////////////
class ioTimeGrowthEndWnd : public ioMovingWnd
{
public:
	enum
	{
		ID_SUCCESS	= 1,
		ID_FAIL		= 2,
		ID_OK		= 3,
	};

protected:
	int m_iMaxInfoCnt;
	int m_iCurInfoCnt;

	ioUIRenderImage *m_pIcon;
	ioUIRenderImage *m_pIconBack;

	TimeEndInfoList m_vEndInfoList;

public:
	void InitInfoWnd();
	void ShowEndInfoWnd();
	void AddEndInfo( int iClassType,
					 const ioHashString& szClassName,
					 const ioHashString& szIconName,
					 const ioHashString& szGrowthName,
					 int iLevel,
					 bool bConfirm );

protected:
	void NextInfoWnd();

public:
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );
	virtual void iwm_hide();

	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

public:
	ioTimeGrowthEndWnd();
	virtual ~ioTimeGrowthEndWnd();
};
////////////////////////////////////////////////////////////////////////////////////////////////
