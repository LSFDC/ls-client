#pragma once

#include "EncryptValue.h"

////////////////////////////////////////////////////////////////////////////////////////////////
/// ioAlchemicInventory�� �ִ� ��ǰ���� ������ ����Ҽ� �ְ��ϴ� ��ġ��
////////////////////////////////////////////////////////////////////////////////////////////////

enum AlchemicType
{
	ALT_NONE,
	ALT_SOLDIER			= 1,	// �뺴
	ALT_ITEM			= 2,	// ���
	ALT_CHANGE			= 3,	// �ٸ��ɷ� ����
	ALT_EXCHANGE		= 4,	// Ư���� ������ ����
	ALT_SELL			= 5,	// peso�� ��ȯ
	ALT_NEW_SOLDIER		= 6,	// ���ο� �뺴 ����
	ALT_NEW_ITEM		= 7,	// ���ο� ��� ����
};

enum AlchemicResultType
{
	ART_SUCCESS,		// ����
	ART_FAIL,			// ����
	ART_NOT_FIND_FUNC,	// ��� code ����
	ART_NOT_MACH_FUNC,	// code�� ������ AlchemicType�� �ٸ�
	ART_NOT_MACH_VALUE,	// ������ ���� ����
	ART_NOT_EMPTY_SLOT,	// ��ĭ����
	ART_OVER_MAX_CNT,	// �ִ밹���ʰ�
	ART_NOT_ENOUGH_CNT,	// ��������
	ART_EXCEPTION,		// ���ܿ���
	ART_NOT_MACH_CODE,	// �ʿ��� ����, ÷���� ������ �ٸ�
	ART_PERIOD_ERROR,	// �Ⱓ�� ����
	ART_TABLE_ERROR,	// alchemic table error
};

enum DisassembleType
{
	ADT_NONE		= 0,
	ADT_SOLDIER		= 1,	// �뺴����
	ADT_EXTRAITEM	= 2,	// ������
};

struct RandomResultInfo
{
	IntVec m_vList;

	RandomResultInfo()
	{
		Init();
	}

	void Init()
	{
		m_vList.clear();
	}
};

struct AlchemicFuncInfo
{
	int m_iCode;

	int m_iMaxCnt1;
	int m_iMaxCnt2;
	int m_iMaxCnt3;
	int m_iMaxCnt4;

	int m_iMaxAdditive;

	int m_iPeriodValue;

	int m_iManaual;
	int m_iPriorityOrder;
	bool m_bActive;

	AlchemicType m_AlchemicType;

	ioHashString m_Desc;
	ioHashString m_Desc2;

	int m_iRandomListNum;
	int m_iNewAlchemicListTable;

	AlchemicFuncInfo()
	{
		Init();
	}

	void Init()
	{
		m_iCode = 0;
		m_iManaual = 0;
		m_iPriorityOrder = 0;
		m_bActive = false;
		m_AlchemicType = ALT_NONE;
		m_iRandomListNum = 0;

		m_iMaxCnt1 = 0;
		m_iMaxCnt2 = 0;
		m_iMaxCnt3 = 0;
		m_iMaxCnt4 = 0;
		m_iMaxAdditive = 0;

		m_iPeriodValue = 0;

		m_iNewAlchemicListTable = 0;

		m_Desc.Clear();
		m_Desc2.Clear();
	}
};
typedef std::vector< AlchemicFuncInfo > vAlchemicFuncInfoList;

class AlchemicFuncSort : public std::binary_function< const AlchemicFuncInfo& , const AlchemicFuncInfo&, bool >
{
public:
	bool operator() ( const AlchemicFuncInfo &lhs, const AlchemicFuncInfo &rhs ) const
	{
		if( lhs.m_iPriorityOrder < rhs.m_iPriorityOrder )
			return true;

		if( lhs.m_iPriorityOrder == rhs.m_iPriorityOrder )
		{
			if( lhs.m_iCode < rhs.m_iCode )
				return true;
		}

		return false;
	}
};

// new alchemic
struct NewAlchemicInfo
{
	int m_iPieceNum;
	int m_iAdditiveNum;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct tagAlchemicFuncInfoS
{
	int m_iCode;
	ioHashString m_Desc;

	tagAlchemicFuncInfoS()
	{
		Init();
	}

	void Init()
	{
		m_iCode = 0;
		m_Desc.Clear();
	}

} AlchemicFuncInfoS;
typedef std::vector<AlchemicFuncInfoS> vAlchemicFuncInfoSList;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct tagAlchemicItemData
{
	ioHashString m_IconName;
	ioHashString m_DescName;
} AlchemicItemData;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ioAlchemicMgr : public Singleton< ioAlchemicMgr >
{
protected:
	float m_fMinFailRate;
	float m_fMaxFailRate;

	float m_fAdditiveConstSoldier;
	float m_fAdditiveConstItem;
	float m_fAdditiveConstExchange;

	float m_fPieceChangeConst;
	float m_fItemSellConst;

	int m_iMinTotalCnt;

	float m_fMinExchangeRate;
	float m_fMaxExchangeRate;

protected:
	DWORD m_dwSoldierDisassembleConst;
	DWORD m_dwExtraItemDisassembleConst;

protected:
	typedef std::map< int, AlchemicItemData > AlchemicItemIconMap;			// �� item�� ������ ������ ����...
	typedef std::map< int, RandomResultInfo > RandomResultInfoMap;
	typedef std::map< int, int > DisassembleInfoMap;

	AlchemicItemIconMap m_AlchemicItemIconMap;

	RandomResultInfoMap m_RandomMap;
	vAlchemicFuncInfoList m_AlchemicFuncInfoList;

	DisassembleInfoMap m_SoldierDisassembleMap;
	DisassembleInfoMap m_ExtraItemDisassembleMap;

// new alchemic
protected:
	typedef std::map< int, IntVec > AlcemicListMap;
	typedef std::map< int, NewAlchemicInfo > NewAlchemicInfoMap;

	AlcemicListMap m_NewAlchemicListMap;

	NewAlchemicInfoMap m_NewAlchemicSoldierInfoMap;
	NewAlchemicInfoMap m_NewAlchemicItemInfoMap;

	int m_iSoldierAdditive;
	int m_iWeaponAdditive;
	int m_iArmorAdditive;
	int m_iHelmetAdditive;
	int m_iCloakAdditive;

	int m_iExchangeAdditive;

	int m_iNewAlchemicMinTotalCnt;

public:
	void LoadMgrInfo();

	void LoadRandomInfo( ioINILoader &rkLoader );
	void LoadFuncInfo( ioINILoader &rkLoader );

	void LoadSoldierDisassembleInfo( ioINILoader &rkLoader );
	void LoadExtraItemDisassembleInfo( ioINILoader &rkLoader );

	//
	void LoadNewAlchemicListTable( ioINILoader &rkLoader );
	void LoadNewAlchemicInfo( ioINILoader &rkLoader );

protected:
	void ClearAllInfo();

public:
	bool FindAlchemicFunc( int iCode, AlchemicFuncInfo &rkFuncInfo );
	bool FindAlchemicFuncFirst( AlchemicFuncInfo &rkFuncInfo );
	bool FindRandomResultInfo( int iNum, RandomResultInfo &rkRandomInfo );

	bool GetAlchemicFuncInfo( vAlchemicFuncInfoSList &rkInfoList );

	ioHashString GetAlchemicItemIcon( int iCode );
	ioHashString GetAlchemicItemName( int iCode );

	bool HasAlchemicFunc();

	inline float GetSellConst() const { return m_fItemSellConst; }
	inline float GetSoldierConst() const { return m_fAdditiveConstSoldier; }

	inline int GetMinTotalCnt() const { return m_iMinTotalCnt; }

	inline float GetMinExchangeRate() const { return m_fMinExchangeRate; }
	inline float GetMaxExchangeRate() const { return m_fMaxExchangeRate; }

public:
	int GetDisassembleCode( int iType, DWORD dwMagicCode );

	int GetDisassembleMinCnt( int iType, bool bMortmain, DWORD dwTime, DWORD dwMagicValue );
	int GetDisassembleMaxCnt( int iType, bool bMortmain, DWORD dwTime, DWORD dwMagicValue );

// new alchemic
public:
	bool FindNewAlchemicSoldierInfo( IN int iClassType, OUT int &iPiece, OUT int &iAdditive );
	bool FindNewAlchemicItemInfo( IN int iItemCode, OUT int &iPiece, OUT int &iAdditive );

	bool CheckEnableNewAlchemicSoldier( int iClassType );
	bool CheckEnableNewAlchemicItem( int iItemCode );

	bool GetNewAlchemicList( IN int iCode, OUT IntVec &rkvList );

	inline int GetExchangeAdditive() const { return m_iExchangeAdditive; }
	inline int GetNewAlchemicMinTotalCnt() const { return m_iNewAlchemicMinTotalCnt; }

public:
	int GetRareSoldierPiece( DWORD dwCode );
	int GetRareSoldierAdditive( DWORD dwCode );

public:
	static ioAlchemicMgr& GetSingleton();

public:
	ioAlchemicMgr();
	virtual ~ioAlchemicMgr();
};

#define g_AlchemicMgr ioAlchemicMgr::GetSingleton()

