#pragma once

#include "EncryptValue.h"

class ioMedalItemInfoManager : public Singleton< ioMedalItemInfoManager >
{
public:
	enum 
	{
		MAX_SLOT_NUM = 6,
	};
public:
	struct ItemInfo
	{
		int		      m_iItemType;
		int           m_iLimitLevel;
		DWORDVec      m_vUseClassTypeVec;
		CEncrypt<int> m_iCharGrowth[MAX_CHAR_GROWTH];
		CEncrypt<int> m_iItemGrowth[MAX_ITEM_GROWTH];
		ioHashString  m_sName;
		ioHashString  m_sIcon;
		int           m_iManual;
		int           m_iSellPeso;

		ItemInfo()
		{
 			m_iItemType   = 0;
 			m_iLimitLevel = 0;
 			m_vUseClassTypeVec.clear(); // empty�� ��� ĳ���� ��������
 
 			for (int i = 0; i < MAX_CHAR_GROWTH ; i++)
 				m_iCharGrowth[i] = 0;
 			for (int i = 0; i < MAX_ITEM_GROWTH ; i++)
 				m_iItemGrowth[i] = 0;

			m_iManual   = 0;
			m_iSellPeso = 0;
		}
	};
protected:
	struct SlotInfo
	{
		int m_iLevelOver;
		int m_iSlotNum;

		SlotInfo()
		{
			m_iLevelOver = 0;
			m_iSlotNum    = 0;
		}
	};

protected:
	typedef std::vector< SlotInfo > vSlotInfoVec;

protected:
	vSlotInfoVec    m_vSlotInfoVec;
	ItemInfo       *m_pItemInfoArray; // ��ȣȭ ������ push_back�� �ȵǹǷ� vector��� �����͸� ���
	int             m_iMaxItemArray;

	CEncrypt<int>   m_iItemSellDevideMin;

protected:
	void Clear();

public:
	void LoadINI();

	DWORD GetLevelLimit( int iItemType );
	int   GetSlotNum( int iCurLevel );
	int   GetLevelOver( int iSlotNum );
	bool  IsRight( int iItemType, int iClassType );

	const ItemInfo *GetItemInfo( int iItemType ); 
	int   GetMedalItemGrowth( int iItemType, bool bCharGrowth, int iArray );

	int GetSellPeso( int iItemType );
	inline int GetSellPesoByMinute() { return m_iItemSellDevideMin; }

public:
	static ioMedalItemInfoManager& GetSingleton();

public:
	ioMedalItemInfoManager(void);
	virtual ~ioMedalItemInfoManager(void);
};

#define g_MedalItemMgr ioMedalItemInfoManager::GetSingleton()

