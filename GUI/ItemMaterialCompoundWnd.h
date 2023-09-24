#pragma once

#include "../io3DEngine/ioButton.h"
#include "NewShopBuyWnd.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
//��ȭ ������ ����Ʈ ���� (���� ������)
class ItemMaterialCompoundWnd : public ioWnd
{
public:
	struct MaterialCompoundTargetItemInfo
	{
		int m_iSlotIndex;
		int m_iItemCode;
		int m_iReinforce;

		MaterialCompoundTargetItemInfo()
		{
			m_iSlotIndex = 0;
			m_iItemCode = 0;
			m_iReinforce = 0;
		}
	};
	typedef std::vector< MaterialCompoundTargetItemInfo > InvenItemNewInfoList;

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	class MaterialCompoundItemInfoSort : public std::binary_function< const MaterialCompoundTargetItemInfo& , const MaterialCompoundTargetItemInfo&, bool >
	{
	public:
		bool operator() ( const MaterialCompoundTargetItemInfo &lhs, const MaterialCompoundTargetItemInfo &rhs ) const
		{
			if( lhs.m_iItemCode < rhs.m_iItemCode )
				return true;

			if( lhs.m_iItemCode == rhs.m_iItemCode )
			{
				if( lhs.m_iReinforce > rhs.m_iReinforce )
					return true;	
			}
			return false;
		}
	};

	class MaterialItemSort : public std::binary_function< const AlchemicItemInfo& , const AlchemicItemInfo&, bool >
	{
	public:
		bool operator() ( const AlchemicItemInfo &lhs, const AlchemicItemInfo &rhs ) const
		{
			if( lhs.m_iCount > rhs.m_iCount )
				return true;

			if( lhs.m_iCount == rhs.m_iCount )
			{
				if( lhs.m_iCode < rhs.m_iCode )
					return true;	
			}
			return false;
		}
	};

public: 
	enum 
	{
		ID_COMPOUND_BTN		= 1,

		ID_TARGETITEM_BTN	= 11,
		ID_TARGETITEM_WND	= 101,
		ID_TARGET_ICON		= 201,
		
		ID_MATERIALITEM_BTN	 = 12,
		ID_MATERIALITEM_WND	 = 102,
		ID_MATERIALITEM_ICON = 202,	

		ID_FAIL_MANUAL_BTN	 = 13,
		ID_FAIL_TOOL_TIP	 = 103,
	};

protected:
	ioUIRenderImage *m_pExtraSubBack;
	ioUIRenderImage *m_pManualMark;
	ioUIRenderImage *m_pGaugeMainBack;
	ioUIRenderImage *m_pGaugeBack;
	ioUIRenderImage *m_pGauge;

	InvenItemNewInfoList m_vTotalItemList;

	int m_nReUseSlot;
	int m_nCurReinforce;
	float m_fSuccessRate;
	int m_nNeedMaterialCnt;
	int m_nNeedMaterialCode;
	int m_nSelectMaterialCode;
	bool m_bIsExtraItem;
	bool m_bIsSelectItem;
	bool m_bIsSelectMaterial;
	int m_nMaxFailExp;
	int m_nCurFailExp;
	float m_fFailExpRate;
	int m_nExtraSubManualIdx;
	int m_nHelpTipIdx;

	DWORD m_dwEtcItemCode;

	ManualMgr* m_pManual;

public:
	virtual void iwm_create();
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );
	/*
	virtual bool iwm_dropitem( ioDragItem *pItem, const POINT &ptCheck, ioWnd *pStartWnd );
	*/

public:
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );
	virtual void ParseExtraInfo( ioXMLElement &xElement );

protected:
	virtual void OnRender();

protected:
	//������ ����
	void SuccessRateDescRender();
	//���а���ġ ����
	void RenderFailExp( int iXPos, int iYPos );
	//�Ŵ��� ���
	void PrintManual( int a_iXPos, int a_iYPos , float fScale );

public:
	//������ �ڵ� ����
	void ShowWndWithEtcItemCode( DWORD dwCode );
	//��� �ڵ� ����
	void SetMaterialCode( int nMaterialCode );

protected:
	//�ʱ�ȭ
	void InitData();
	//��� ���� ������ ���
	void LoadTotalItemList();
	//��ȭ ������ �ʱ�ȭ
	void TargetItemInit();

protected:
	//����Ʈ ���
	void GetTargetList( vNewShopPullDownItem &rkList );
	void GetTargetSeletInfo( ExtraItemList &rkList );
	//����Ʈ ��ư Ŭ�� ��
	void TargetListBtnUp( ioWnd *pOpenBtn, int iListArray );
	//���� �缳��
	bool SlotReload();
	//��ȭ ������ ����
	void UpdateItemData();	
	//��ȭ ��Ŷ ����
	void SendItemCompound();

protected:
	//��� ����Ʈ ���
	void GetMaterialList( vNewShopPullDownItem &rkList );
	//��� ����Ʈ ��ư Ŭ�� ��
	void MaterialListBtnUp( ioWnd *pOpenBtn, int iListArray );

protected:
	//��Ằ �г�Ƽ ����
	void ApplySuccessRate();

public:
	bool GetCurAlchemicItemInfo( OUT AlchemicToolTipInfo &rkInfo, IN DWORD dwItemInfoID );

public:
	ItemMaterialCompoundWnd(void);
	virtual ~ItemMaterialCompoundWnd(void);
};
