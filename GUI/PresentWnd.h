#pragma once

#include "../io3DEngine/ioMovingWnd.h"

class ioUIRenderImage;
class ioUIRenderFrame;
class PresentAlarmWnd : public ioMovingWnd
{
public:
	enum
	{
		ID_EXIT = 1,
		ID_LINK = 2,
	};

protected:
	ioUIRenderFrame *m_pBottomFrm;
	ioUIRenderFrame *m_pStatePinkFrm;
	ioUIRenderImage *m_pPresentBack;
	ioUIRenderImage *m_pPresentIcon;
	ioUIRenderImage *m_pPresentNum;
	ioUIRenderImage *m_pPresentPlus;
	ioUIRenderImage *m_pPresentCustom;
	ioUIRenderImage *m_pCostumeMark;

protected:
	DWORD m_dwIndex;                   // ���� �ε���.
	DWORD m_dwSlotIndex;
	ioHashString m_szSendID;           // ���� ����
	short   m_iPresentType;            // ( 1.�뺴 - 2.ġ�� - 3.���� - 4.��� - 5.��� )
	short   m_iPresentMent;            // ( ��ƮŸ��(Ŭ���̾�Ʈ���� ����) )
	int     m_iPresentValue1;          // �뺴:(�뺴Ÿ��), ġ��(ITEMSLOT�� m_item_type), ����(ETCITEMSLOT�� m_iType), ���(��ұݾ�), ���(����ڵ�)
	int     m_iPresentValue2;          // �뺴:(�뺴�Ⱓ), ġ��(ITEMSLOT�� m_item_code), ����(ETCITEMSLOT�� m_iValue1), ���(NONE), ���(( ��� ���尪 * 10000 ) + ���Ⱓ)
	int     m_iPresentValue3;		   // ���:(������Ų)
	int     m_iPresentValue4;		   // ���:(������Ų)
	DWORD   m_dwLimitDate;      

protected:
	DWORD   m_dwHideDelayTime;
	bool    m_bRePosition;
	int m_nGradeType;

protected:
	void SetPresentIcon();
	bool IsSpecialAwardBlock();
	bool IsShowDelay();
	void ReSizePresentUI();

public:
	// Wnd�� ����ϴ� ������ ������ ���̰� �־� �� �Լ����� ���� ShowWnd...
	void ShowPresentAlarmWnd();
	void ShowPresentAlarmWnd( const ioHashString szID, short iPresentType, short iPresentMent, int iValue1, int iValue2, int iValue3, int iValue4 );

public:
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );
	virtual bool iwm_esc();
	virtual void SetPositionToDefault( int iParentWidth, int iParentHeight, bool bChildMove, bool bCreate = false );

protected:
	virtual void AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame );
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();
	void OnRenderPresent( int iXPos, int iYPos );
	void OnRenderPeso( int iXPos, int iYPos );
	void OnRenderGradeExp( int iXPos, int iYPos );

protected:
	void SetGradeType( int nPresentType, int nPresentCode );

public:
	PresentAlarmWnd();
	virtual ~PresentAlarmWnd();
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
class PresentDeleteWnd : public ioMovingWnd
{
public:
	enum
	{
		ID_EXIT  = 1,
		ID_ALL_CLEAR = 2,
		ID_CLOSE = 3,
	};

protected:
	ioUIRenderFrame *m_pStatePinkFrm;
	ioUIRenderImage *m_pIconBack;
	ioUIRenderImage *m_pPresentIcon;
	ioUIRenderImage *m_pPresentNum;
	ioUIRenderImage *m_pPresentPlus;
	ioUIRenderImage *m_pPresentCustom;
	ioUIRenderImage *m_pCostumeMark;

protected:
	DWORD m_dwPresentIndex;
	DWORD m_dwPresentSlotIndex;
	ioHashString m_szSendID;           // ���� ����
	short   m_iPresentType;            // ( 1.�뺴 - 2.ġ�� - 3.���� - 4.��� - 5.��� )
	short   m_iPresentMent;            // ( ��ƮŸ��(Ŭ���̾�Ʈ���� ����) )
	int     m_iPresentValue1;          // �뺴:(�뺴Ÿ��), ġ��(ITEMSLOT�� m_item_type), ����(ETCITEMSLOT�� m_iType), ���(��ұݾ�), ���(����ڵ�)
	int     m_iPresentValue2;          // �뺴:(�뺴�Ⱓ), ġ��(ITEMSLOT�� m_item_code), ����(ETCITEMSLOT�� m_iValue1), ���(NONE), ���(( ��� ���尪 * 10000 ) + ���Ⱓ)
	int     m_iPresentValue3;		   // ���:(������Ų)
	int     m_iPresentValue4;		   // ���:(������Ų)
	DWORD   m_dwLimitDate;      
	int m_nGradeType;

protected:
	DWORD   m_dwHideDelayTime;

protected:
	void SetPresentIcon();

public:
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual bool iwm_esc();

protected:
	virtual void AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame );
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();
	void OnRenderPresent( int iXPos, int iYPos );
	void OnRenderPeso( int iXPos, int iYPos );
	void OnRenderGradeExp( int iXPos, int iYPos );
	void OnRenderExtraItem( int iXPos, int iYPos );
	void OnRenderExtraItemBox( int iXPos, int iYPos );
	void OnRenderCostumeBox( int iXPos, int iYPos );

protected:
	void SetGradeType( int nPresentType, int nPresentCode );

public:
	PresentDeleteWnd();
	virtual ~PresentDeleteWnd();
};

class PresentTimeCashAlarmWnd : public ioMovingWnd
{
public:
	enum
	{
		ID_EXIT = 1,
		ID_LINK = 2,
	};

protected:
	ioUIRenderFrame *m_pBottomFrm;
	ioUIRenderFrame *m_pStatePinkFrm;
	ioUIRenderImage *m_pPresentBack;
	ioUIRenderImage *m_pPresentIcon;
	
protected:
	ManualMgr* m_pManual;
	bool m_bRePosition;
	int m_nAddCash;
	
public:
	virtual void iwm_show();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );
	virtual bool iwm_esc();
	virtual void SetPositionToDefault( int iParentWidth, int iParentHeight, bool bChildMove, bool bCreate = false );

	virtual void AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame );
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

protected:
	void ReSizePresentUI();
	void PrintManual( int nXPos, int nYPos, float fScale );

public:
	void ShowPresentTimeCashAlarmWnd( int nAddCash );

public:
	PresentTimeCashAlarmWnd();
	virtual ~PresentTimeCashAlarmWnd();
};

class PresentTimeCashReleaseWnd : public ioMovingWnd
{
public:
	enum
	{
		ID_X	= 1,
		ID_OK	= 2,
	};

protected:
	ioUIRenderImage *m_pIconBack;
	ioUIRenderImage *m_pIcon;

protected:
	DWORD m_dwEtcItemCode;
	ioHashString m_szItemName;

public:
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );
	virtual bool iwm_esc();

protected:
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

public:
	void ShowPresentTimeCashReleaseWnd( DWORD dwEtcItemCode );

public:
	PresentTimeCashReleaseWnd();
	virtual ~PresentTimeCashReleaseWnd();
};