#pragma once

struct FishingItem
{
	ioHashString m_Name;
	ioHashString m_IconName;
	ioHashString m_Desc;

	int m_iItemNum;
	__int64 m_iPeso;

	FishingItem()
	{
		m_iItemNum = 0;
		m_iPeso = 0;
	}
};
typedef std::vector<FishingItem> FishingItemList;

struct FishingItemGrade
{
	int m_iGradeNum;
	ioHashString m_Name;
	ioHashString m_Desc;
	float m_fValue;

	FishingItemGrade()
	{
		m_iGradeNum = 0;
		m_fValue = FLOAT1;
	}
};
typedef std::vector<FishingItemGrade> FishingItemGradeList;

struct FishingTypeInfo
{
	ioHashString m_FishingRod;
	ioHashString m_FishingReadyEffect;
	ioHashString m_FishingWaitEffect;
	ioHashString m_FishingPullEffect;
};
typedef std::vector<FishingTypeInfo> FishingTypeInfoList;

class ioFishingManager : public Singleton< ioFishingManager >
{
protected:
	FishingItemList m_vFishingItemList;
	FishingItemGradeList m_vFishingItemGradeList;
	FishingTypeInfoList m_vFishingTypeInfoList;

public:
	void LoadFishingInfo();
	void LoadGuildFishingInfo();

	void LoadFishItemList( ioINILoader& rkLoader );
	void LoadFishGradeList( ioINILoader& rkLoader );
	void LoadFishTypeList( ioINILoader& rkLoader );
 
protected:
	void ClearAllInfo();

public:
	ioHashString GetFishingItemName( int iItemType );
	ioHashString GetFishingItemDesc( int iItemType );
	ioHashString GetFishingItemIconName( int iItemType );
	ioHashString GetFishingItemGradeName( int iItemGrade );
	ioHashString GetFishingItemGradeDesc( int iItemGrade );
	ioHashString GetFishingPresentItemName( short iPresentType, int iValue1, int iValue2 );
	ioHashString GetFishingPresentIconName( short iPresentType, int iValue1, int iValue2 );

	ioHashString GetFishingRod( int iType );
	ioHashString GetFishingReadyEffect( int iType );
	ioHashString GetFishingWaitEffect( int iType );
	ioHashString GetFishingPullEffect( int iType );

	//__int64 GetFishingItemSellPeso( int iItemType, int iItemGrade );

public:
	inline int GetFishingItemGradeSize(){ return m_vFishingItemGradeList.size(); }

public:
	static ioFishingManager& GetSingleton();

public:
	ioFishingManager();
	virtual ~ioFishingManager();
};

#define g_FishingMgr ioFishingManager::GetSingleton()

