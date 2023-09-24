#pragma once

#include "../ioComplexStringPrinter.h"
#include "../io3DEngine/ioStringConverter.h"
#include "../io3DEngine/ioButton.h"

#include "ioMessageBox.h"
#include "SoldierPackageWnd.h"

class RandomDecoSelectWnd : public ioWnd
{
public:
	enum
	{
		ID_SOLDIER_BTN	= 1,
		ID_RECV			= 2,

		ID_GO			= 3,

		ID_SOLDIER_WND  = 1000,
		ID_PULLDOWN_WND	= 1001,
	};

protected:
	ioUIRenderImage *m_pPresentPreBack;
	ioUIRenderImage *m_pPresentBack;

	ioUIRenderImage *m_pPresentIcon;

protected:
	bool m_bSubscription;

	DWORD m_dwIndex;                   // ���� �ε���.
	DWORD m_dwSlotIndex;               // ���� ���� �ε���.
	ioHashString m_szSendID;           // ���� ����
	short   m_iPresentType;            // ( 1.�뺴 - 2.ġ�� - 3.���� - 4.��� - 5.��� )
	short   m_iPresentMent;            // ( ��ƮŸ��(Ŭ���̾�Ʈ���� ����) )
	int     m_iPresentValue1;          // �뺴:(�뺴Ÿ��), ġ��(ITEMSLOT�� m_item_type), ����(ETCITEMSLOT�� m_iType), ���(��ұݾ�), ���(����ڵ�)
	int     m_iPresentValue2;          // �뺴:(�뺴�Ⱓ), ġ��(ITEMSLOT�� m_item_code), ����(ETCITEMSLOT�� m_iValue1), ���(NONE), ���(( ��� ���尪 * 10000 ) + ���Ⱓ)
	int     m_iPresentValue3;          // ���:(������Ų)
	int     m_iPresentValue4;		   // ���:(������Ų)
	DWORD   m_dwLimitDate;

	ioHashString m_szSubscriptionID;
	int m_iSubscriptionGold;

protected:
	int m_iSelectClassType;
	bool m_bResult;

	CHARACTER m_CharInfo;
	CHARACTER m_OverCharInfo;

	vPackageItem m_PackageItemList;

	bool m_bPullDownShow;

	ioComplexStringPrinter m_PresentInfo[2];

protected:
	void SetPresentIcon();
	void PresentRecv();
	void ReSizePresentUI();

public:
	virtual void iwm_create();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );
	virtual void iwm_show();
	virtual void iwm_hide();

	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

	void OnRenderPresent( int iXPos, int iYPos );

protected:
	void GetClassList( vPackageItem &rkList );
	void ConvertItemList( IN vPackageItem &rkPackageList, OUT vNewShopPullDownItem &rkList );

	void SetPullDownMenu( ioWnd *pOpenBtn );

	void PullDownBtnUP( ioWnd *pOpenBtn, int iListArray );
	void PullDownBtnOver( ioWnd *pOpenBtn, int iListArray );

	void UpdateBtn();

	void DefaultDecoration( CHARACTER &rkCharInfo );
	void CheckSoldierWnd( int iClassType, CHARACTER &rkCharInfo );

public:
	void SetPresent( DWORD dwIndex, DWORD dwSlotIndex );
	void SetSubscription( DWORD dwIndex, const ioHashString szSubscriptionID );
	void OnResultState();

public:
	RandomDecoSelectWnd();
	virtual ~RandomDecoSelectWnd();
};

