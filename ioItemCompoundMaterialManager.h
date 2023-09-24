#pragma once

typedef struct tagMaterialRateInfo
{
	int m_nLevel;
	int m_nPiece;
	int m_nAdditive;
	int m_nSuccessRate;
	int m_nFailExp;
	int m_nFailExpExtra;
	int m_nPcRoomBonus;

	tagMaterialRateInfo()
	{
		m_nLevel = 0;
		m_nPiece = 0;
		m_nAdditive = 0;
		m_nSuccessRate = 0;
		m_nFailExp = 0;
		m_nFailExpExtra = 0;
		m_nPcRoomBonus = 0;
	}
}MaterialRateInfo;

typedef std::vector<MaterialRateInfo> MaterialRateInfoList;

typedef struct tagCompoundMaterialInfo
{
	int m_nSuccessConst;
	float m_fFailExpConst;
	float m_fExtraNeedRate;
	MaterialRateInfoList m_vMaterialRateInfoList;

	tagCompoundMaterialInfo()
	{
		m_nSuccessConst = 0;
		m_fFailExpConst = 0;
		m_fExtraNeedRate = 0.0f;
		m_vMaterialRateInfoList.clear();
	}
}CompoundMaterialInfo;

typedef struct tagEtcNeedMaterial
{
	int m_nItemCode;
	int m_nMaterial;

	tagEtcNeedMaterial()
	{
		m_nItemCode = 0;
		m_nMaterial = 0;
	}
}EtcNeedMaterial;

typedef struct tagReinforceNeedMaterial
{
	int m_nReinforce;
	int m_nMaterial;

	tagReinforceNeedMaterial()
	{
		m_nReinforce = 0;
		m_nMaterial = 0;
	}
}ReinforceNeedMaterial;

class ioItemCompoundMaterialManager : public Singleton< ioItemCompoundMaterialManager >
{
public:
	enum ConfirmType
	{
		CT_Additive			= 100001,
		CT_ExtraCode		= 100000,
		CT_ExtraItemStart	= 11000,
		CT_WeaponCode		= 10000,
		CT_Mercenary		= 1000,
		CT_Yes				= 1,
	};

protected:
	typedef std::map< DWORD, tagCompoundMaterialInfo > CompoundMaterialMap;
	typedef std::vector< EtcNeedMaterial > EtcNeedMaterialList;
	typedef std::vector< ReinforceNeedMaterial > ReinforceNeedMaterialList;

	CompoundMaterialMap m_CompoundMaterialMap;
	EtcNeedMaterialList m_EtcNeedMaterialList;
	ReinforceNeedMaterialList m_ReinforceNeedMaterialList;

	float m_fPenaltyRate;
	float m_fAdditivePenaltyRate;
	float m_fExtraPenaltyRate;
	float m_fExtraSuccessRate;
	float m_fSuccessRate;

	int m_nMaxReinforce;
	int m_nFixedFailReinforce;
	int m_nAllAlarmLevel;	

public:
	static ioItemCompoundMaterialManager& GetSingleton();

public:
	void LoadCompoundMaterialInfo();
	void ClearData();

	//강화 성공 상수 얻기
	int GetSuccessConst( DWORD dwCode );
	//강화 레벨별 성공율 얻기
	int GetReinforceSuccessRate( int nTargetReinforce, DWORD dwCode );
	//강화 레벨별 추가 성공율 얻기 (PC방 보너스)
	int GetPCRoomBonusReinforceSuccessRate( int nTargetReinforce, DWORD dwCode );
	//강화 레벨별 요구 재료수 얻기
	int GetNeedMaterialCount( int nTargetReinforce, DWORD dwCode, bool bExtra );
	//강화 레벨별 최대 실패 경험치 얻기
	int GetMaxFailExp( int nTargetReinforce, DWORD dwCode, bool bExtra );
	//레어 아이템인가 확인
	bool IsExtraItem( int nItemCode );
	//필요한 재료 아이템 코드 확인
	int NeedMaterialCode( int nItemCode, int nReinforce );

	bool CheckAllAlarm( int iTargetReinforce );

public:
	inline int GetMaxReinforce() const { return m_nMaxReinforce; }
	inline int GetFixedFailReinforce() const { return m_nFixedFailReinforce; }

public:
	ioItemCompoundMaterialManager();
	virtual ~ioItemCompoundMaterialManager();
};

#define g_CompoundMaterialMgr ioItemCompoundMaterialManager::GetSingleton()