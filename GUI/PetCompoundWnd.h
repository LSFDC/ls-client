#pragma once

/////////////////////////////////////////////////////////////////////////////////////////////////////
class PetCompoundWnd : public ioWnd
{
public: 
	enum 
	{
		ID_COMPOUND_BTN		= 1,
		ID_BACK_BTN			= 2,
		ID_RESULT_BTN		= 3,
		ID_EMPTY_BTN		= 4,
				
		ID_MATERIALPET_BTN	 = 11,
		ID_MATERIALPET_WND	 = 101,		

		ID_HELP_MARK = 12,
		ID_HELP_TOOL_TIP = 13,
	};

protected:	
	PetPullDownInfoList m_vTotalPetList;

	ioUIRenderImage *m_pManualMark;

	int m_nHelpTipIdx;

	int m_nSelectPetSlot;

	bool m_bEmpty;
	bool m_bResult;
	
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

protected:
	void RenderEmpty( int nXPos, int nYPos );
	void RenderResult( int nXPos, int nYPos );
	
public:
	//���ڵ� ����
	void SelectPetSlot( int nSelectSlot );

protected:
	//�ʱ�ȭ
	void InitData();
	//�긮��Ʈ ���
	void LoadTotalPetList();
	//�ڵ� ��� ������ ����
	void AutoShowMaterialList();
	//���� ������
	void ResetWnd();

protected:	
	//�ռ� ��Ŷ ����
	void SendCompound();

protected:
	//��� ����Ʈ ��ġ ����
	int GetMaterialListPosY( int nSize );
	//��� ����Ʈ ���
	void GetMaterialList( vNewShopPullDownItem &rkList );
	//��� ����Ʈ ��ư Ŭ�� ��
	void MaterialListBtnUp( ioWnd *pOpenBtn, int nListArray );
	
public:
	//��� UI ����
	void ApplyCompoundResult();

protected:
	//UI ����
	void HideMainInfo();
	void ShowMainInfo();

public:
	PetCompoundWnd(void);
	virtual ~PetCompoundWnd(void);
};