#pragma once
#include "GUI/ioPullDownItem.h"

class PowerUpPullDownItem : public SinglePullDownItem
{
public:
	int m_iTooltipIdx;
	int m_iManualIndex;

	PowerUpPullDownItem()
	{
		m_iTooltipIdx = 0;
		m_iManualIndex = 0;
	}
public:
	virtual void OnRender( int iOverIdx, int iOverImageIdx );
	virtual ~PowerUpPullDownItem(){}
};
//////////////////////////////////////////////////////////////////////////////////////////
typedef std::vector< PowerUpPullDownItem >	vPowerUpPullDownItemVec;

class ioPowerUpManager : public Singleton< ioPowerUpManager >
{
public:
	static ioPowerUpManager& GetSingleton();

public:
	enum
	{
		MAX_EQUIP_ITEM	 = 4,		
	};

	//Grade Info Max Line
	enum 
	{
		MAX_GRADE_INFO_LINE = 5,
	};
	enum PowerUpInfoType
	{
		PIT_HERO = 0,
		PIT_ITEM = 1,
		PIT_MAX	 = 2,
	};

protected:
	struct ColorText
	{
		DWORD m_dwColor;
		ioHashString m_szText;

		ColorText()
		{
			m_dwColor = 0;
		}

		void Clear()
		{
			m_dwColor = 0;

			m_szText.Clear();
		}
	};
	typedef std::vector< ColorText > vLineText;
	typedef std::vector< vLineText > vGradeText;
	typedef std::vector< vGradeText > vGradeList;

public:
	struct PowerUpItemInfo
	{
		int	iItemCode[MAX_EQUIP_ITEM];

		PowerUpItemInfo()
		{
			for( int i = 0; i < MAX_EQUIP_ITEM; ++i )
				iItemCode[i] = 0;
		}
	};
	typedef std::vector<PowerUpItemInfo> PowerUpItemInfoVec;

	struct CharPowerUpInfo
	{		
		int					iPowerUpMax;
		int					iCharCode;

		PowerUpItemInfoVec	vPowerUpDefaultItemInfo;		//ItemMaker가 생성되기전에 디폴트 아이템 정보를 검증하기 위한 아이템 정보
		PowerUpItemInfoVec	vPowerUpExtraItemItemInfo;

		vGradeText			vCharGradeText;
		vGradeText			vItemGradeList[MAX_EQUIP_ITEM];

		vGradeList			vCharGradedList;
		vGradeList			vItemGradedList[MAX_EQUIP_ITEM];

		vGradeText			vCharGradeToolTip;
		int					m_iCharGradeToolTipSizeX;
		int					m_iCharGradeToolTipSizeY;
		ioHashStringVec		m_CharTitleList;
		ioHashStringVec		m_ItemTitleList;

		CharPowerUpInfo()
		{
			iPowerUpMax = 0;
			iCharCode	= 0;
		}

		const int GetCharPowerUpValue( int eItemGrade, int iSlotIndex ) const
		{
			int Index = (eItemGrade - PUGT_ITEM_GRADE1) / PUGT_ITEM_GRADE_BASE;
			if( !COMPARE( Index, 0, (int)vPowerUpExtraItemItemInfo.size() ) )
				return 0;

			const PowerUpItemInfo& Info = vPowerUpExtraItemItemInfo[ Index ];
			if( !COMPARE( iSlotIndex, 0, MAX_EQUIP_ITEM ) )
				return 0;

			return Info.iItemCode[iSlotIndex];
		}
	};
	typedef std::map<int,CharPowerUpInfo> CharPowerUpInfoMap;
	
	struct PowerUpRareItemInfo
	{
		IntVec	GradeItemCode;

		vGradeText	vItemGradeList[MAX_EQUIP_ITEM];
		vGradeList	vItemGradedList[MAX_EQUIP_ITEM];

		vGradeText	vItemGradeToolTip;
		int			m_nItemGradeToolTipSizeX;
		int			m_nItemGradeToolTipSizeY;
		ioHashStringVec		m_ItemTitleList;

		const int GetPowerUpValue( int eItemGrade ) const
		{
			int Index = (eItemGrade - PURIGT_RARE_ITEM_GRADE1) / PURIGT_RARE_ITEM_GRADE_BASE;
			if( !COMPARE( Index, 0, (int)GradeItemCode.size() ) )
				return 0;

			return GradeItemCode[Index];
		}
	};
	typedef std::map<int, PowerUpRareItemInfo> PowerUpRareItemInfoMap;

protected:
	vPowerUpPullDownItemVec	m_PowerUpPullDownItemVec;
	CharPowerUpInfoMap		m_CharPowerUpInfoMap;
	PowerUpRareItemInfoMap	m_PowerUpRareItemInfoMap;

	int						m_iMtrlEtcCode;
	IntVec					m_CharGradeNeedCountList;
	IntVec					m_ItemGradeNeedCountList;
	IntVec					m_RareItemGradeNeedCountList;

	TabUISettingVec m_TabUISettingVec[PIT_MAX];

protected:
	int					m_iReinforcedClassType;
	int					m_iReinforcedItemIndex;

public:
	inline int GetReinforcedClassType() { return m_iReinforcedClassType; }
	inline int GetReinforcedItemIndex() { return m_iReinforcedItemIndex; }
	inline void ClearReinforcedClassType() { m_iReinforcedClassType = 0; }
	inline void ClearReinforcedItemIndex() { m_iReinforcedItemIndex = 0; }
	const TabUISettingVec& GetTabUISettinVec( int iType );

	const IntVec GetEnablePowerUpClassNumberInfo() const;
	const IntVec GetEnablePowerUpExtraItemNumberInfo( int nSlot ) const;

public:
	virtual void Initialize();

public:
	void LoadGUI();

	void LoadPowerUpCharInfo( ioINILoader& rkLoader );
	void LoadPowerUpItemInfo( ioINILoader& rkLoader, const char* szTitle, const char* szExtend, PowerUpItemInfo& ItemInfo );

	void LoadMtrlInfo( ioINILoader& rkLoader, const char* szTitle, IntVec& vNeedList );
	void LoadRareItemMtrlInfo( ioINILoader& rkLoader );
	void LoadGradeTitlelInfo( ioINILoader &kLoader, const char* szTitle, ioHashStringVec& vTitleList );

	//진화사용시 나올 모든 진화 단계의 내용
	void LoadGradeInfo( ioINILoader& kLoader, CharPowerUpInfo& info );
	void LoadTextDescInfo( ioINILoader& kLoader, const char* szTitle, int nMaxLine, vGradeText& vTextList );

	//진화성공시 나올 현 진화 단계의 내용
	void LoadGradedInfo( ioINILoader& kLoader, CharPowerUpInfo& info );
	void LoadUITabBtnInfo( ioINILoader &rkLoader );

	//진화 정보에 관한 ToolTip
	void LoadCharToolTip( ioINILoader &kLoader, CharPowerUpInfo& info );

protected:
	void LoadRarePowerUpItemInfo();

public:
	inline int GetMtrlEtcItemCode(){ return m_iMtrlEtcCode; }

	int GetCharGradeUpMtrlCount( int eCharGrade );
	int GetItemGradeUpMtrlCount( int eItemGrade );
	int GetRareItemGradeUpMtrlCount( int eItemGrade );

	const ioPowerUpManager::CharPowerUpInfo* GetCharGradeInfo( int iClassType ) const;
	const ioPowerUpManager::PowerUpRareItemInfo* GetRarePowerUpItemInfo( int iCode ) const;

	const ioHashString& GetCharGradeTitle( int iClassType, int eCharGrade ) const;
	const ioHashString& GetItemGradeTitle( int iItemCode, int eItemGrade ) const;
	const ioHashString& GetExtraItemGradeTitle( int iItemCode, int eItemGrade ) const;
	void GetCharGradeToolTipSize( int iClassType, int& iSizeX, int& iSizeY );
	void GetExtraItemGradeToolTipSize( int nItemCode, int& iSizeX, int& iSizeY );

	void GetMenuList( IN ioPullDownList* pList );
	void SetMenuList( ioXMLElement &xElement );
	void OnPowerUpMenuToolTipShow( const OnOverPullDownEvent& Event ) const;
	void OnPowerUpMenuToolTipHide( const OnLeavePullDownEvent& Event ) const;
	void OnPowerUpMenuClick( const ClickPullDownEvent& Event, int itemindex, ioWnd* pCalledWnd );
	void OnPowerUpImageClick( const ClickPullDownEvent& Event );

public:
	bool IsPowerUpGradeItem( int iClassType, int iItemCode );

public:
	bool CheckEnableSoulStone( int iType, int iData, int& iUseMtrlCount, int& iCurMtrlCount );
	bool CheckEnableCharPowerUp( int iClassType, ioHashString& szError );
	bool CheckEnableItemPowerUp( int iSlotIndex, ioHashString& szError );
	bool CheckEnableEquipPowerUpItem( int eCharPowerUpType, int iItemCode );

public:
	void ApplyCharPowerUp( int iIndex, int iCode, byte eGrade );
	void ApplyItemPowerUp( int iIndex, int iCode );
public:
	void SetCharGardePrinter( const int iClassType, int iCurLine, float fScale, ioComplexStringPrinter &rkPrinter );
	void SetItemGardePrinter( const int iItemCode, int iEquipSlotIndex, int iCurLine, float fScale, ioComplexStringPrinter &rkPrinter );
	void SetExtraItemGardePrinter( const int iItemCode, int iEquipSlotIndex, int iCurLine, float fScale, ioComplexStringPrinter &rkPrinter );

	void SetCharGardedPrinter( const int iClassType, const int iPowerUpGrade, int iCurLine, float fScale, ioComplexStringPrinter &rkPrinter );
	void SetItemGardedPrinter( const int iItemCode, const int iPowerUpGrade, int iEquipSlotIndex, int iCurLine, float fScale, ioComplexStringPrinter &rkPrinter );
	void SetExtraItemGardedPrinter( const int iItemCode, const int iPowerUpGrade, int iEquipSlotIndex, int iCurLine, float fScale, ioComplexStringPrinter &rkPrinter );

	void SetCharToolTipPrinter( const int iClassType, int iCurLine, float fScale, ioComplexStringPrinter &rkPrinter );
	void SetExtraItemToolTipPrinter( const int nItemCode, int iCurLine, float fScale, ioComplexStringPrinter &rkPrinter );

public:
	ioPowerUpManager();
	virtual ~ioPowerUpManager();
};

#define g_PowerUpManager ioPowerUpManager::GetSingleton()