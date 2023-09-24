#pragma once

#include "../io3DEngine/ioStringConverter.h"

#include "../io3DEngine/ioMovingWnd.h"

class ioUIRenderImage;
class ioUIRenderFrame;
class PracticeHelpWnd : public ioWnd
{
public:
	enum
	{
		ID_EXIT = 1,
		ID_NEXT = 2,
		ID_CLOSE= 3,
		ID_PREV = 4,
	};

protected:
	ioUIRenderImage *m_pDevK;
	ioUIRenderImage *m_pDevKBack;

protected:
	struct PracticeHelp
	{
		ioHashString m_szTitle;
		ioHashString m_szHelp;
	};
	typedef std::vector< PracticeHelp > vPracticeHelp;
	vPracticeHelp m_vPracticeHelp;

protected:
	DWORD     m_dwTextTickTime;
	DWORD     m_dwCurTextTickTime;
	int       m_iCurTextTickCount;
	int       m_iCurTextLineCount;
	ioHashString m_szTextTickSnd;

	struct StringTick
	{
		char m_szCopyDesc[MAX_PATH];
		StringTick()
		{
			memset( m_szCopyDesc, 0, sizeof( m_szCopyDesc ) );
		}
	};
	typedef std::vector< StringTick > vStringTick;
	vStringTick m_vStringTickList;

protected:
	StringVector m_DescList;
	int m_iStep;
	int m_iClassType;
	ioPlayStage *m_pPlayStage;

public:
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual bool iwm_esc();
	virtual bool iwm_spacebar();

protected:
	void SplitDescription( const ioHashString &szDesc );
	void SetStep( int iStep );
	bool SetNextAction();

public:
	bool SetPracticeHelp( int iClassType );
	void SetPlayStage( ioPlayStage *pPlayStage );

public:
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

public:
	virtual void ParseExtraInfo( ioXMLElement &xElement );

protected:
	void ProcessTextTick();
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

public:
	PracticeHelpWnd();
	virtual ~PracticeHelpWnd();
};
//////////////////////////////////////////////////////////////////////////
class SoldierPracticeSelectBtn : public ioButton
{
protected:
	ioUIRenderFrame *m_pCustomNormal;
	ioUIRenderFrame *m_pCustomOver;

	ioUIRenderImage *m_pNone;
	ioUIRenderImage *m_pIconBack;
	ioUIRenderImage *m_pSoldierIcon;

protected:
	int  m_iClassType;
	bool m_bHaveClass;
	int m_nGradeType;

public:
	void SetClassType( int iClassType );
	int  GetClassType(){ return m_iClassType; }

protected:
	virtual void AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame );
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	

protected:
	virtual void OnRender();
	
public:
	SoldierPracticeSelectBtn();
	virtual ~SoldierPracticeSelectBtn();
};

class SoldierPracticeSelectWnd : public ioMovingWnd
{
public:
	enum
	{
		ID_EXIT = 1,
		ID_LEFT_PAGE = 2,
		ID_RIGHT_PAGE= 3,

		ID_SOLDIER_BTN1 = 100,
		ID_SOLDIER_BTN30= 129,
	};

	enum
	{
		MAX_SOLDIER_BTN = 30,
		MAX_HORZ = 6,
		MAX_VERT = 5,
	};

protected:
	ioPlayStage *m_pPlayStage;
	IntVec m_CharList;
	int m_iCurPage;
	int m_iMaxPage;
	IntVec m_vExtraClassVec;

public:
	void SetPlayStage( ioPlayStage *pPlayStage );

protected:
	void SetBtnPos();
	void UpdateSetPage( int iCurPage );

public:
	virtual void iwm_show();
	virtual void iwm_hide();

public:
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

public:
	virtual void ParseExtraInfo( ioXMLElement &xElement );

public:
	bool QuestGuide( int iClassType, float &rfXPos, float &rfYPos );

public:
	SoldierPracticeSelectWnd();
	virtual ~SoldierPracticeSelectWnd();
};
//////////////////////////////////////////////////////////////////////////
class SoldierPracticeAlarmWnd : public ioWnd
{
public:
	enum
	{
		ID_EXIT = 1,
		ID_CLOSE= 2,
		ID_SHOP = 3,
	};

protected:
	ioUIRenderImage *m_pSoldierIcon;
	
protected:
	int m_iClassType;

public:
	void SetClassType( int iClassType );

public:
	virtual void iwm_hide();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

protected:
	virtual void OnRender();

public:
	SoldierPracticeAlarmWnd();
	virtual ~SoldierPracticeAlarmWnd();
};
