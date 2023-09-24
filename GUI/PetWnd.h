#pragma once

class ioUIRenderImage;
class ioPlayStage;

//////////////////////////////////////////////////////////////////////////
class PetWnd : public ioMovingWnd
{
public:
	enum
	{
		ID_CHARGE_GOLD = 1,
		ID_SHOP = 2,
		ID_EXIT = 3,

		ID_ACTIVE = 11,
		ID_INACTIVE = 12,
		ID_REMOVE = 13,
		ID_EAT = 14,
		ID_COMPOUND = 15,
		ID_EAT_TIP = 16,
		ID_COMPOUND_TIP = 17,
		
		ID_PETVIEW = 101,

		ID_PETSLOT1 = 201,
		ID_PETSLOT20 = 220,

		PET_EAT_WND = 501,
		PET_COMPOUND_WND = 502,
		PET_RESELL_RESULT_WND = 503,
	};

protected:
	ioPlayStage* m_pPlayStage;

	ioUIRenderImage *m_pNameLine;
	ioUIRenderImage *m_pGaugeMainBack;
	ioUIRenderImage *m_pGaugeBack;
	ioUIRenderImage *m_pGauge;
	ioUIRenderImage *m_pNewGauge;
	ioUIRenderImage *m_pGrowthBackGray;
	ioUIRenderImage *m_pGrowthBackLight;
	ioUIRenderImage *m_pExpText;
	ioUIRenderImage *m_pExpNumText;
	ioUIRenderImage *m_pPerText;

	int m_nSelectSlotIndex;
	int m_nCurEquipPetCode;
	int m_nSetSelectIndex;

	int m_nCurPetCode;
	int m_nCurPetLv;
	int m_nMaxPetLv;
	int m_nCurRank;
	int m_nCurPetExp;
	float m_fSelectPetLevelRate;
	ioHashString m_szPetName;

	int m_nMaxExp;
	float m_fNewExpRate;
	float m_fPreExpRate;

	int m_nGrowthStat[MAX_CHAR_GROWTH+MAX_ITEM_GROWTH];

	bool m_bEatResultExp;

	DWORD m_dwEquipWndID;

	DWORD m_dwExpGaugeUpTime;
	DWORD m_dwCheckExpGaugeUpTime;

	float m_fExpGaugeSpeed;

	ioHashString m_szExpUpSound;
	ioHashString m_szLevelUpSound;
	ioHashString m_szRankUpSound;
	ioHashString m_szCompoundSound;

	ioHashString m_szExpUpEffect;
	ioHashString m_szLevelUpEffect;
	ioHashString m_szRankUpEffect;
	ioHashString m_szCompoundEffect;
	
public:
	virtual void iwm_create();
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

public:
	virtual void AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame );
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );	
	virtual void ParseExtraInfo( ioXMLElement &xElement );

protected:
	void RenderLevelInfo( int nXPos, int nYPos );
	void RenderInfo( int nXPos, int nYPos  );
	void RenderStat( int nXPos, int nYPos  );
	void RenderTitle( int nXPos, int nYPos  );
	void RenderEatResultExp( int nXPos, int nYPos );
	void RenderExpNum( int nXPos, int nYPos, float fExpRate );

public:
	//Ȱ��ȭ
	void SetActivePet();
	//��Ȱ��ȭ
	void SetInActivePet();
	//����
	void SetRemovePet( int nPetCode, int nPetRank, int nPetLevel );	
	//�����ֱ�
	void SetEatPet();
	//�ռ��ϱ�
	void SetCompoundPet();

public:
	//�÷��̽������� ����
	void SetPlayStage( ioPlayStage *pPlayStage );
	ioPlayStage *GetPlayStage() { return m_pPlayStage; }
	//�� ���� ���� ����
	void SetSelectPetSlot( int nSlotIndex );

protected:
	//�ʱ�ȭ
	void Init();
	//������ ����
	void RefreshPetIcon( bool bSelect );
	//�� ����
	void SelectPet( DWORD dwID );
	//ù ��° ���� ����
	void SelectFirstSlot();
	//���� ���� ����
	void SelectSlot( bool bInitAni );
	//�� �� ����
	void SetPetView( int nPetCode, int nPetRank );
	//�� ���� ����
	void SetPetInfo( bool bSelect );
	//���� ������
	void ResetPetWnd( bool bInitAni );
	//���� Ȯ��
	bool CheckSlot();
	
protected:
	//�����ֱ� ��� ����ġ ����
	void SetEatResultExp( int nPreLevel, int nNewLevel, int nPetRank, int nPreExp, int nCurExp );
	void ExpGaugeUp();

public:
	//�����ֱ� ��� ����
	void OnPetEatResultInfo( int nPetCode, int nPetRank, int nPreLevel, int nNewLevel, int nPreExp, int nCurExp );
	//�����ֱ� ��� ����
	void OnPetEatResultApply( int nPetIndex );

public:
	//�ռ� ��� ����
	void OnPetCompoundResultInfo( int nPetCode, int nPetRank, bool bResultType );
	//�ռ� ��� ����
	void OnPetCompoundResultApply( int nPetIndex );

public:
	//�� ��� ����
	void OnPetAddResultApply( int nPetIndex );

public:
	//UI �缳�� 
	void ResetMainMenu();

protected:
	//UI ����
	void HideMainMenu();
	void ShowMainMenu();
	
protected:
	void SetChildTitleOffset( DWORD dwWndID, bool bActive );

public:
	PetWnd();
	virtual ~PetWnd();
};