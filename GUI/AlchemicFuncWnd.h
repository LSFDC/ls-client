#pragma once

#include "../io3DEngine/ioButton.h"
#include "../ioComplexStringPrinter.h"
#include "../ioAlchemicMgr.h"

#include "NewShopBuyWnd.h"

class ManualMgr;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SelectAlchemicFuncBtn : public ioButton
{
protected:
	int m_iCurSelectFunc;
	DWORD m_dwCurTitleColor;

	ioHashString m_szDefaultText;
	ioHashString m_szSelectText;

public:
	void InitData();
	void SetSelectAlchemicFunc( int iSelectFunc );
	void SetSelectNewAlchemicFunc( int iSelectFunc, int iValue );
	void SetCurTitleColor( DWORD dwTitleColor );

	inline int GetSelectAlchemicFunc() const { return m_iCurSelectFunc; }
	
protected:
	virtual void OnRender();

public:
	virtual void iwm_show();

	virtual void ParseExtraInfo( ioXMLElement &xElement );

public:
	SelectAlchemicFuncBtn();
	virtual ~SelectAlchemicFuncBtn();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class AlchemicItemRegisterWnd : public ioMovingWnd
{
public:
	enum
	{
		ID_REGISTER_BTN	= 2,
		ID_CANCEL_BTN	= 3,
		ID_EXIT_BTN		= 4,
		ID_COUNT_EDIT	= 5,

		MAX_DESC		= 3,
	};

protected:
	int m_iCode;
	int m_iCount;
	
	int m_iMaxCount;
	int m_iCurCount;

	ioEdit *m_pPreEdit;
	ioUIRenderFrame *m_pEditFrm;

	ioUIRenderImage *m_pIconImg;
	ioUIRenderImage *m_pIconBackImg;
	ioUIRenderFrame *m_pPieceFrm;
	ioUIRenderFrame *m_pAdditiveFrm;

	ioWnd* m_pPreWnd;

public:
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );
	virtual bool iwm_spacebar();

protected:
	virtual void OnRender();
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );
	virtual void AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame );

protected:
	void SetCountEditFocus();
	void SetCountRegister();

public:
	void SetRegisterItemInfo( int iType, int iCode, int iMaxCnt, ioWnd *pWnd );

public:
	AlchemicItemRegisterWnd();
	virtual ~AlchemicItemRegisterWnd();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SelectAlchemicItemBtn : public ioButton
{
public:
	enum
	{
		BTT_PIECE1			= 1,	// 용병/장비 조합용 조각
		BTT_PIECE2			= 2,	// 랜덤 조합용 조각
		BTT_PIECE3			= 3,	// 선택 결과용 조각
		BTT_PIECE4			= 4,	// 선택 재료 조각
		BTT_ADDITIVE		= 5,	// 용병/장비/선택 조합용 첨가물
		BTT_ALL				= 6,	// 페소 전환
		BTT_NEW_PIECE		= 7,	// 용병/장비 새로운 조합용 조각
		BTT_NEW_ADDITIVE	= 8,	// 용병/장비 새로운 조합용 첨가물
	};

protected:
	int m_iType;
	int m_iCode;
	int m_iPreCode;
	int m_iCurCnt;
	int m_iEmptyCnt;
	int m_iDecreaseCnt;

	ioComplexStringPrinter m_CountText;
	ioComplexStringPrinter m_CountText2;

	ioUIRenderFrame *m_pEmptyFrm;
	ioUIRenderFrame *m_pPieceFrm;
	ioUIRenderFrame *m_pAdditiveFrm;

	ioUIRenderImage *m_pPieceImg1;
	ioUIRenderImage *m_pPieceImg2;
	ioUIRenderImage *m_pPieceImg3;
	ioUIRenderImage *m_pAdditiveImg;
	ioUIRenderImage *m_pAllImg;

	ioUIRenderImage *m_pMaxNum;

	ioUIRenderImage *m_pIconImg;
	ioUIRenderImage *m_pIconBackImg;

public:
	void InitData();
	void InitData( int iType, int iEmptyCnt );
	void InitNewData( int iType, int iValue, int iMaxCnt );
	
	void SetDragData( const AlchemicFuncInfo& rkInfo, int iCode );
	void SetData( int iCode, int iCnt );
	void SetDecrease( int iCnt );

	inline int GetType() const { return m_iType; }
	inline int GetPreCode() const { return m_iPreCode; }
	inline int GetCode() const { return m_iCode; }
	inline int GetCount() const { return m_iCurCnt; }

protected:
	void AlchemicRender();
	void NewAlchemicRender();

public:
	virtual void iwm_hide();

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

public:
	virtual void AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame );
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );

public:
	SelectAlchemicItemBtn();
	virtual ~SelectAlchemicItemBtn();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class PreSelectItemBtn : public ioButton
{
protected:
	int m_iType;
	int m_iCode;
	int m_nGradeType;

	bool m_bAlchemicEnable;

	ioUIRenderFrame *m_pEmptyFrm;
	ioUIRenderFrame *m_pItemFrm;

	ioUIRenderImage *m_pEmptyImg;
	ioUIRenderImage *m_pIconImg;
	ioUIRenderImage *m_pIconBackImg;

public:
	void InitData();
	void SetItemValue( int iType, int iValue );

	inline int GetType() const { return m_iType; }
	inline int GetCode() const { return m_iCode; }

public:
	virtual void iwm_hide();

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

public:
	virtual void AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame );
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );

public:
	PreSelectItemBtn();
	virtual ~PreSelectItemBtn();
};

////////////////////////////////////////////////////////////////////////////////////////////////////

class AlchemicFuncWnd : public ioWnd
{
public: 
	enum 
	{
		ID_ACTION_BTN			= 1,		// 실행버튼
		ID_ACTION_BTN2			= 2,		// 처음으로
		ID_ACTION_BTN3			= 3,		// 선물함으로

		ID_ALCHEMIC_FUNC_BTN	= 11,		// 상단 기능선택 버튼 SelectAlchemicFuncBtn

		ID_ALCHEMIC_FUNC_WND	= 101,		// 기능선택 버튼 눌러서 나오는 wnd

		ID_SELECT_ITEM_BTN1		= 201,
		ID_SELECT_ITEM_BTN2		= 202,
		ID_SELECT_ITEM_BTN3		= 203,
		ID_SELECT_ITEM_BTN4		= 204,

		ID_ADDITIVE_ITEM_BTN	= 301,

		ID_RESULT_BTN			= 401,		//

		ID_PRE_SELECT_ITEM_BTN1		= 501,
		ID_PRE_SELECT_ITEM_BTN15	= 515,

		ID_PAGE_PRE_BTN				= 516,
		ID_PAGE_NEXT_BTN			= 517,
	};

	enum
	{
		MAX_PRE_SELECT_ITEM	= 15,
	};

	enum FuncState
	{
		AFS_PRE_SELECT,
		AFS_SELECT,
		AFS_RESULT,
		AFS_FINAL_RESULT,
	};

	enum
	{
		RBB_SELECT_NONE,
		RBB_SELECT_ONE,
		RBB_RESULT_ONE,
		RBB_TWO,
	};

	struct MagicEffectInfo
	{
		DWORD m_dwCreateTime;
		
		int m_iCurAlphaRate;
		float m_fCurScaleRate;
		

		MagicEffectInfo()
		{
			Init();
		}

		void Init()
		{
			m_dwCreateTime = 0;
			m_fCurScaleRate = FLOAT1;
			m_iCurAlphaRate = MAX_ALPHA_RATE;
		}
	};

	typedef std::vector< MagicEffectInfo > vMagicEffectInfo;

protected:
	bool m_bActionEnable;
	int m_iCurSelectFunc;

	AlchemicFuncInfo m_CurFuncInfo;
	vAlchemicFuncInfoSList m_InfoList;

	ioComplexStringPrinter m_SuccessRateDesc;
	ioComplexStringPrinter m_ProcessDesc[4];

	ManualMgr* m_pManual;

// new alchemic
protected:
	int m_iCurNewAlchemicSelectValue;
	int m_iNewAlchemicPiece;
	int m_iNewAlchemicAdditive;

	IntVec m_vPreSelectItemList;

	int m_iMaxPreItemPage;
	int m_iCurPreItemPage;

	ioComplexStringPrinter m_PreSelectDesc[2];
	ioComplexStringPrinter m_PreSelectPage;

protected:
	ioUIRenderImage *m_pResultIcon;
	ioUIRenderImage *m_pPesoIcon;
	ioUIRenderImage *m_pIconBack;

	ioUIRenderImage *m_pStripDark;
	ioUIRenderImage *m_pStripLight;

	ioUIRenderImage *m_pMagicCircle;
	ioUIRenderImage *m_pMagicCircleArrow;
	ioUIRenderImage *m_pSmallCircle;

	ioUIRenderImage *m_pNewAlchemicIcon;
	ioUIRenderImage *m_pNewAlchemicIconBack;

	ioUIRenderImage *m_pLightImage;

protected:
	ioUIRenderImage *m_pEffect;
	ioUIRenderImage *m_pEffect2;

	ioHashStringVec m_vRollingEffect;
	ioHashStringVec m_vEndEfffect_S;	// 성공
	ioHashStringVec m_vEndEfffect_F;	// 실패

	float m_fScaleRate;
	float m_fAlphaRate;
	float m_fSlopeRate;

	DWORD m_dwRollingEffectTime;
	DWORD m_dwRollingEffectCheckTime;

	ioHashString m_szRollingSound;
	ioHashString m_szSuccessSound;
	ioHashString m_szFailSound;

	// result effect
	int m_iCurEffectArray;
	DWORD m_dwResultEffectTime;

	// final effect
	int m_iCurSuccessEffectArray;
	DWORD m_dwSuccessEffectTime;
	DWORD m_dwDestroyEffectTime;

protected:
	FuncState m_FuncState;

	int m_iResultType;

	int m_iResultCode1;
	int m_iResultCode2;
	int m_iResultCode3;
	int m_iResultCode4;
	int m_iResultAddCode;

	int m_iResultValue1;
	int m_iResultValue2;
	int m_iResultValue3;
	int m_iResultValue4;
	int m_iResultAddValue;

	int m_iResultValue;
	int m_iResultPeriod;

	int m_iChangeCode;
	int m_iChangeCnt;

	__int64 m_iChangePeso;
	__int64 m_iCurPeso;

	DWORD m_dwResultStartTime;
	DWORD m_dwResultEndTime;

	// result effect
	DWORD m_dwResultEffectEndTime;

	BYTE m_iAlpha1;
	BYTE m_iAlpha2;
	float m_fCurEffectScale;

	// final effect
	DWORD m_dwFinalResultStartTime;

	// magic effect
	vMagicEffectInfo m_vMagicEffectInfoList;

	DWORD m_dwMagicEffectCreateTime;

	int m_nGradeType;

protected:
	void GetAlchemicFuncList( vNewShopPullDownItem &rkList );
	void AlchemicFuncListBtnUp( ioWnd *pOpenBtn, int iListArray );

	void ChangeFunc( AlchemicFuncInfo kFuncInfo, bool bCheckCode );

	void SuccessRateDescRender();
	void OnRenderStrip();
	void OnRenderProcessDesc();
	void PrintManual( int a_iXPos, int a_iYPos , float fScale );

	void SendAlchemicAction();
	bool FillSendInfo( SP2Packet &rkPacket );

	void ClearProcessDesc();
	void ClearResultData();

	void SetResultState( int iResultType );
	void SetFinalResultState();

	void CheckActionBtnText();
	void CheckProcessText();
	void CheckSuccessRateText();
	void CheckSelectItemBtn();

	void ReBatchButton( int iType );

	void ProcessResultEffect();
	void ProcessFinalResultEffect();

	void OnRenderResultEffect();
	void OnRenderFinalResultEffect();
	void OnRenderDestroyEffect();
	void OnRenderSuccessEffect();

	void CreateMagicCircle();
	void ProcessMagicCircle();
	void OnRenderMagicCircle();

protected:
	void SetAlchemicSoldier();
	void SetAlchemicItem();
	void SetAlchemicChange();
	void SetAlchemicExchange();
	void SetAlchemicSell();

	DWORD CheckDropEnable( const ioMouse& mouse, ioDragItem *pItem );
	void InitAlchemicItem( DWORD dwID );

protected:
	void SetPreNewAlchemicSoldier();
	void SetPreNewAlchemicItem();

	void SetNewAlchemicSoldier( DWORD dwID );
	void SetNewAlchemicItem( DWORD dwID );

	void CheckPreSelectDesc();
	void UpdatePreSelectItemList();
	void UpdatePreSelectPage();

public:
	void OnAlchemicFuncResult( int iResult, SP2Packet &rkPacket );
	void OnAlchemicFuncSubResult( int iResult );

	void CheckActionEnable();
	void RefreshAlchemicFunc();

	void SelectClickAction( int iCode );

	inline int GetFuncState() const { return m_FuncState; }

// new alchemic
public:
	void SelectNewAlchemicSoldier( int iClassType );
	void SelectNewalchemicItem( int iItemCode );

	void ClearPreSelectDesc();

	bool GetCurAlchemicItemInfo( OUT AlchemicToolTipInfo &rkInfo, IN DWORD dwItemInfoID );

public:
	virtual void iwm_create();
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

	virtual IWDropEffect iwm_dragenter( ioDragItem *pItem, const ioMouse& mouse );
	virtual IWDropEffect iwm_dragover( ioDragItem *pItem, const ioMouse& mouse );
	virtual bool iwm_dropitem( ioDragItem *pItem, const ioMouse& mouse, ioWnd *pStartWnd );

public:
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );
	virtual void ParseExtraInfo( ioXMLElement &xElement );

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();
	virtual void OnRenderAfterChild();

public:
	AlchemicFuncWnd(void);
	virtual ~AlchemicFuncWnd(void);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
