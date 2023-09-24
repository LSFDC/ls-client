#pragma once

/////////////////////////////////////////////////////////////////////////////////////////////////////
class PetEatWnd : public ioWnd
{
public: 
	enum 
	{
		ID_EAT_BTN		= 1,
		ID_BACK_BTN		= 2,
		ID_EMPTY_BTN	= 3,
				
		ID_MATERIALPET_BTN	 = 11,
		ID_MATERIALPET_WND	 = 101,	

		ID_HELP_MARK = 12,
		ID_HELP_TOOL_TIP = 13,
	};

protected:	
	ioUIRenderImage *m_pManualMark;

	int m_nHelpTipIdx;

	int m_nSelectPetSlot;
	int m_nNeedMaterialCnt;
	int m_nNeedMaterialCode;
	int m_nSelectMaterialCode;
	bool m_bIsSelectMaterial;

	bool m_bEmpty;
	bool m_bMaxLv;

public:
	virtual void iwm_create();
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

public:
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );
	virtual void ParseExtraInfo( ioXMLElement &xElement );

protected:
	virtual void OnRender();
	
public:
	//���ڵ� ����
	void SelectPetSlot( int nSelectSlot );
	//��� �ڵ� ����
	void SetMaterialCode( int nMaterialCode );

protected:
	//�ʱ�ȭ
	void InitData();
	//�ڵ� ��� ������ ����
	void AutoShowMaterialList();
	//���� ������
	void ResetWnd();

protected:
	//�� ������ ����
	void UpdatePetData();	
	//����ġ ��Ŷ ����
	void SendExpUp();

protected:
	//��� ����Ʈ ��ġ ����
	int GetMaterialListPosY( int nSize );
	//��� ����Ʈ ���
	void GetMaterialList( vNewShopPullDownItem &rkList );
	//��� ����Ʈ ��ư Ŭ�� ��
	void MaterialListBtnUp( ioWnd *pOpenBtn, int nListArray );
	
public:
	//�ִ� ������ UI ����
	void ApplyMaxLv();

protected:
	//UI ����
	void HideMainInfo();
	void ShowMainInfo();

public:
	PetEatWnd(void);
	virtual ~PetEatWnd(void);
};