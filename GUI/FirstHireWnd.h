#pragma once

#include "../CharacterInfo.h"
#include "../io3DEngine/ioStringConverter.h"

class ioUIRenderImage;
class ioUI3DRender;
class ioCreateChar;
class ioItem;

class Hire3DCharWnd : public ioWnd
{
protected:
	ioUI3DRender *m_pUI3DRender;
	ioCreateChar *m_aMyChar;
	float         m_fRotateYaw;

public:
	void CreateCharacter( const CHARACTER &rkInfo );
	void EquipItem( const ioItem *pItem );
	void SetCharAnimation();

	void RotateY( float fYaw );

public:
	virtual void iwm_create();

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

public:
	Hire3DCharWnd();
	virtual ~Hire3DCharWnd();
};
//////////////////////////////////////////////////////////////////////////
// 아래의 클래스를 이벤트에서 사용하므로 변경되는 UI에서 최초 용병 고용은
// 새로 만든다. 아래의 클래스는 더 이상 최초 용병 고용과 상관 없다.
//////////////////////////////////////////////////////////////////////////
class FirstHireWnd : public ioWnd
{
public:
	enum
	{
		ID_EXIT				= 1,

		ID_CLASS_LEFT       = 2,
		ID_CLASS_RIGHT      = 3,

		ID_KINDRED_LEFT     = 4,
		ID_KINDRED_RIGHT    = 5,

		ID_FIRST_HIRE       = 6,

		ID_CHARACTER        = 7,
		ID_CHAROVER         = 8,
		
		// EventWnd 1000부터 사용.
		// BankruptcyCharWnd 2000 부터 사용
	};	

protected:
	int				m_iClassSelect;
	DWORDVec		m_dwFirstClassList;
	int				m_iCurRaceDetailIdx;
	typedef std::vector< RaceDetailType > RaceDetailList;
	RaceDetailList	m_EnableRaceList;

	CHARACTER		m_CharInfo;

	ioHashString    m_szClassName;
	ioHashString    m_szKindredName;

	// Help
	ioHashStringVec m_szHelpTitleVec;
	typedef std::vector< StringVector > StringVectorList;
	StringVectorList    m_DescVecList;

protected:
	virtual void UpdateFirstClass( OUT IntVec &rvDeleteArrayList );
	virtual void LoadINI( IN const IntVec &rvDeleteArrayList );
	virtual void SetBtnTitle();
	virtual const char *GetCurHelpTitle() const;
	virtual	const StringVector *GetCurDescVec();

	void UpdateRaceDetail();
	void UpdateCharacter();
	void ChangeClass( bool bAdd );
	void ChangeCharacter( bool bAdd );
	void UpdateCharInfo( RaceDetailType eDetail );
	int  RotateNum( int iCurNum, int iMinNum, int iMaxNum );
	void SplitDescription( const ioHashString &szDesc, OUT StringVector &rszStringVec);

	void CheckHideWnd();

public:
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

protected:
	virtual void OnRender();

public:
	FirstHireWnd();
	virtual ~FirstHireWnd();
};
//////////////////////////////////////////////////////////////////////////
class FirstSoldierWnd : public ioWnd
{
public:
	enum
	{
		ID_FIRST_HIRE = 1,
	};

protected:
	ioUIRenderImage *m_pBackGround;
	ioUIRenderImage *m_pSoldierIcon;

protected:
	int m_iClassType;
	ioHashString m_szClassName;

protected:
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	
	
public:
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

protected:
	virtual void OnRender();

public:
	FirstSoldierWnd();
	virtual ~FirstSoldierWnd();
};
//////////////////////////////////////////////////////////////////////////
class BankruptcyCharWnd : public ioWnd
{
public:
	enum 
	{	
		ID_RECEIVE_CHAR_BTN = 1,
	};

protected:
	ioUIRenderImage *m_pBackGround;
	ioUIRenderImage *m_pSoldierIcon;

protected:
	static int m_iClassSelect;
	
protected:
	// Help
	ioHashString    m_szClassName;
	ioHashStringVec m_szHelpVec;

protected:
	void UpdateFirstClass();

protected:
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	

public:
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

protected:
	virtual void OnRender();

public:
	BankruptcyCharWnd();
	virtual ~BankruptcyCharWnd();
};
