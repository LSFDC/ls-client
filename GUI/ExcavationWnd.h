#pragma once
#include "../io3DEngine/ioMovingWnd.h"

class ioWnd;
class ioButton;
class ExcavationResultWnd : public ioWnd
{
public:
	enum
	{
		ID_GO	= 1,
		ID_OK	= 2,

		ID_X	= 3,
	};

	enum
	{
		ROLLING_BTN = 1,
		RESULT_BTN	= 2,
	};

protected:
	ioUIRenderImage *m_pIcon;
	ioUIRenderImage *m_pRedBackEffect;
	ioUIRenderImage *m_pBlueBackEffect;
	ioUIRenderImage *m_pNumText;
	ioUIRenderImage *m_pPlusText;

	int   m_iArtifactType;
	int   m_iArtifactValue1;
	int   m_iArtifactValue2;
	bool  m_bAlarm;
	float m_fMapRate;

	bool m_bFinalResult;

	float m_fIconScale;
	DWORD m_dwIconScaleTime;
	DWORD m_dwRollingTime;
	DWORD m_dwRollingTotalTime;

	DWORD m_dwRollingCheckTime;
	DWORD m_dwRollingEndTime;
	DWORD m_dwScaleStartTime;

	int m_iCurIconArray;
	int m_iCurGradeArray;

	ioHashString m_szRollingSound;
	ioHashString m_szResultSound;
	ioHashString m_szResultFailSound;

	ioHashString m_szName;
	ioHashString m_szDesc;
	ioHashString m_szGrade;
	ioHashString m_szResultArtifact;

	ioPlayStage* m_pPlayStage;

public:
	void SetResultInfo( int iArtifactType, int iArtifactValue1, int iArtifactValue2, bool bAlram, float fMapRate );
	void SetPlayStage( ioPlayStage *pPlayStage );

protected:
	virtual void ReBatchButton( int iType );
	void SetArtifactInfo();

public:
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );
	virtual void iwm_hide();
	virtual bool iwm_spacebar();
	virtual bool iwm_esc();
	virtual void iwm_show();

	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );
	virtual void ParseExtraInfo( ioXMLElement &xElement );

protected:
	virtual void OnRender();
	virtual void OnProcess( float fTimePerSec );

public:
	ExcavationResultWnd(void);
	virtual ~ExcavationResultWnd(void);
};

//-------------------------------------------------------------------------------------------------------------------------------
class ExcavationExpWnd : public ioWnd
{
protected:
	ioUIRenderImage *m_pExpGauge;

	int m_iCurLevel;
	int m_iCurExp;

public:
	void SetInfo( int iLevel, int iExp );

protected:
	virtual void OnRender();

public:
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );

public:
	ExcavationExpWnd();
	virtual ~ExcavationExpWnd();
};
//-------------------------------------------------------------------------------------------------------------------------------
class ExcavatingBtn : public ioButton
{
protected:
	ioUIRenderImage *m_pIcon;

	ioUIRenderImage *m_pKeyBackNor;
	ioUIRenderImage *m_pKeyBackOn;
	ioUIRenderImage *m_pKeyBack;

	ioUIRenderImage *m_pAttackKeyNor;
	ioUIRenderImage *m_pAttackKeyOn;

	ioUIRenderImage *m_pKeyDashNor;
	ioUIRenderImage *m_pKeyDashOn;

	CEncrypt<int> m_iMaxCoolTime;
	CEncrypt<int> m_iCurCoolTime;
	DWORD         m_dwCoolStartTime;

	DWORD m_dwScaleStartTime;
	float m_fCurScaleRate;

	WORD m_wAttackValue;

protected:
	void ProcessScale();
	void ProcessGaugeIcon();

	int  GetRenderImageStartHeight( ioUIRenderImage *pImage, int iSrcHeight, int iCur, int iMax );
	void RenderGaugeIcon( int xPos, int yPos, ioUIRenderImage *pImage, int iSrcHeight, int iCur, int iMax );

	void UpdateAttackKeyImage();

	void RenderInputKeyBack( int xPos, int yPos );
	void RenderInputKey( int xPos, int yPos );

	ioUIRenderImage *GetNorKeyImage( WORD wKeySetValue );
	ioUIRenderImage *GetOnKeyImage( WORD wKeySetValue );

public:
	bool IsCanExcavating();

	void InitCoolTime();
	void ClearCurCoolTime();

public:
	virtual void iwm_show();
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

public:
	ExcavatingBtn();
	virtual ~ExcavatingBtn();
};

//------------------------------------------------------------------------
class ExcavationSearchingWnd : public ioMovingWnd
{
public:
	enum
	{
		ID_EXIT_BTN       = 1,
		ID_EXCAVATING_BTN = 2,
		ID_EXP_WND        = 3,
		ID_TOOL_TIP       = 4,
	};

protected:
	enum 
	{
		POS_SAVE_VERSION   = 4,
		MAX_SEARCHING_TYPE = 6,
	};

protected:
	class DistLevelSort : public std::binary_function< const int&, const int&, bool >
	{
	public:
		bool operator() ( const int &lhs, const int &rhs ) const
		{
			if( lhs > rhs )
				return true;
			return false;
		}
	};

protected:
	ioPlayStage* m_pPlayStage;

	ioUIRenderImage *m_pBatteryGauge;
	ioUIRenderImage *m_pWave[MAX_SEARCHING_TYPE];
	ioUIRenderImage *m_pDistNumBlue;
	ioUIRenderImage *m_pDistNumYellow;
	ioUIRenderImage *m_pDistNumRed;
	ioUIRenderImage *m_pRateNum;
	ioUIRenderImage *m_pDistNone;

	DWORD m_dwFullBatteryTime;
	DWORD m_dwMiddleBatteryTime;
	DWORD m_dwLowBatteryTime;

	int m_iRangeTypeArray;
	int m_iCurGrowWaveTypeArray;
	int m_iCurExtinctWaveTypeArray;
	int m_iWaveWidthcut;

	IntVec m_vDistLevelVector;

	DWORD m_dwArtifactPosCheckTime;
	DWORD m_dwCurArtifactPosCheckTime;

	DWORD m_dwRadarCheckTime;
	DWORD m_dwCurRadarCheckTime;
	int   m_iWaveAddSize;
	int   m_iArtifactDist;

	int m_iDefaultX;
	int m_iDefaultY;

	ioHashString m_sWaveSoundArray[MAX_SEARCHING_TYPE];
	ioHashString m_sCurWaveSound;

	int   m_iCurSuccessPercent;

	DWORD m_dwSuccessPerCheckTime;
	DWORD m_dwCurSuccessPerCheckTime;

	bool  m_bProcessRadarPos;
	
protected:
	void ProcessArtifactsPos();
	void ProcessRadar();
	void ProcessSuccessPercent();	
	void ProcessToolTip();
	void UpdateDefaultPos();

public:
	void SetPlayStage( ioPlayStage *pPlayStage );

	void ClearExcavatingCurCoolTime();

	void SetProcessRadarPos(bool bProcessRadarPos) { m_bProcessRadarPos = bProcessRadarPos; }

	void OnExcavatingBtnUp( DWORD dwID );
	void UpdateExp();

public:
	virtual void iwm_create();
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );
	virtual void ParseExtraInfo( ioXMLElement &xElement );
	virtual void SetPositionToDefault( int iParentWidth, int iParentHeight, bool bChildMove, bool bCreate = false );

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

public:
	ExcavationSearchingWnd();
	virtual ~ExcavationSearchingWnd();
};
//////////////////////////////////////////////////////////////////////////
class ExcavationBuyPopup : public ioWnd
{
public:
	enum
	{
		ID_EXIT  = 1,
		ID_SHOP  = 2,
		ID_CANCEL= 3,
	};

protected:
	ioUIRenderImage *m_pIconBack;
	ioUIRenderImage *m_pItemIcon;

protected:
	ioHashString     m_szItemName;

public:
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	

protected:
	virtual void OnRender();

public:
	ExcavationBuyPopup();
	virtual ~ExcavationBuyPopup();
};
