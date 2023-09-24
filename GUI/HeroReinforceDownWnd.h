#pragma once

class ioUIRenderImage;
class ioComplexStringPrinter;

class ioPetView;

class HeroReinforceDownWnd : public ioWnd
{
public: 
	enum 
	{
		ID_ENABLE_REINFORECE_SOLDIER_SELECT_BTN  = 100,
		ID_REINFORCE_BTN						 = 101,
		ID_SOLDIER_WND							 = 102,
	};

	//soldier info count
	enum 
	{
		SOLDIER_INFO_CNT = 2,
		SOLDIER_REINFORCE_INFO_CNT = 4,
	};

protected:
	CHARACTER m_CharInfo;
	ioUIRenderImage *m_pNameLine;
	ioComplexStringPrinter m_SoldierInfo[ SOLDIER_INFO_CNT ];
	ioComplexStringPrinter m_SoldierReinforceInfo[ SOLDIER_REINFORCE_INFO_CNT ];
	ioComplexStringPrinter m_SoulStonePrinter;
	ioComplexStringPrinter m_SoldierSelectPrinter;
public:
	void SetSoldierInfo( int iClassType );
public:
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

protected:
	virtual void OnRender();
	virtual void OnRenderAfterChild();

protected:
	void ClearList();

public:
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	

public:
	HeroReinforceDownWnd(void);
	virtual ~HeroReinforceDownWnd(void);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class HeroReinforceDoneWnd : public ioWnd
{
public: 

	enum 
	{
		ID_REINFORECE_AGAIN_BTN					 = 100,
		ID_SEE_HERO_BTN							 = 101,
		ID_SOLDIER_WND							 = 102,
	};

	//soldier info count
	enum 
	{
		SOLDIER_INFO_CNT = 2,
		REINFORCED_SOLDIER_INFO_CNT = 4,
	};

protected:
	ioUIRenderImage *m_pNameLine;
	CHARACTER m_CharInfo;
	ioComplexStringPrinter m_SoldierInfo[ SOLDIER_INFO_CNT ];
	ioComplexStringPrinter m_ReinforcedSoldierInfo[ REINFORCED_SOLDIER_INFO_CNT ];
	ioComplexStringPrinter m_SoulStonePrinter;
	ioComplexStringPrinter m_ReinforcedPrinter;
	ioHashString	m_szEffectName;
	ioHashString    m_szSuccessSound;
public:
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );
	virtual void ParseExtraInfo( ioXMLElement &xElement );

protected:
	virtual void OnRender();
	virtual void OnRenderAfterChild();

protected:
	void ClearList();

public:
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	
public:
	void SetSoldierInfo( int iClassType );

public:
	HeroReinforceDoneWnd(void);
	virtual ~HeroReinforceDoneWnd(void);
};
