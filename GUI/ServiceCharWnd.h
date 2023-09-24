#pragma once

#include "FirstHireWnd.h"
class ioUI3DRender;
class ioCreateChar;
class ioItem;

//////////////////////////////////////////////////////////////////////////
class ServiceCharWnd : public ioMovingWnd
{
public:
	enum
	{
		ID_EXIT				= 1,

		ID_CLASS_LEFT       = 2,
		ID_CLASS_RIGHT      = 3,

		ID_CHAR_RECV        = 6,

		ID_CHARACTER        = 7,
		ID_CHAROVER         = 8,
	};	

public:
	static void  SetServiceChar( bool bShow, DWORD dwServerDate );

protected:
	int             m_iClassLimitDate;
	int				m_iClassSelect;
	DWORDVec		m_dwFirstClassList;
	int				m_iCurRaceDetailIdx;
	typedef std::vector< RaceDetailType > RaceDetailList;
	RaceDetailList	m_EnableRaceList;

	CHARACTER		m_CharInfo;

	ioHashString    m_szClassName;
	ioHashString    m_szKindredName;

	// Help
	ioHashString    m_szHelpTitle;
	StringVector    m_DescList;
	ioHashString    m_szWndTitle;

	//
	static bool     sg_bWndShow;

protected:
	void UpdateFirstClass();
	void UpdateRaceDetail();
	void UpdateCharacter();
	void ChangeClass( bool bAdd );
	void ChangeCharacter( bool bAdd );
	void UpdateCharInfo( RaceDetailType eDetail );
	int  RotateNum( int iCurNum, int iMinNum, int iMaxNum );
	void SplitDescription( const ioHashString &szDesc );
	void CheckHideWnd();
	
public:
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

public:
	ServiceCharWnd();
	virtual ~ServiceCharWnd();
};

