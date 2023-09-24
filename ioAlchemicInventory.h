#pragma once


#include "GUI/MyInventoryWnd.h"

class ioAlchemicInventory
{
public:
	enum
	{
		MAX_DB_SLOT		= 10,
		MAX_INVEN		= 30,
		MAX_SLOT_CNT	= 2000000000,
	};

	struct AlchemicItem
	{
		int m_iCode;
		int m_iCount;
		int m_iValue1;
		int m_iValue2;

		bool m_bNewAdd;

		AlchemicItem()
		{
			Init();
		}

		void Init()
		{
			m_iCode = 0;
			m_iCount = 0;
			m_iValue1 = 0;
			m_iValue2 = 0;

			m_bNewAdd = false;
		}
	};
	typedef std::vector< AlchemicItem > vAlchemicItem;

protected:
	vAlchemicItem m_vAlchemicList;

public:
	void Initialize();
	void ApplyAlchemicData( SP2Packet &rkPacket );
	void ApplyAlchemicSync( SP2Packet &rkPacket );

protected:
	bool FindAlchemicItem( int iCode, AlchemicItem &rkAlchemicItem );
	bool AddAlchemicItem( int iCode, int iCount );
	bool DeleteAlchemicItem( int iCode );

	bool ChangeAlchemicItem( int iCode, const AlchemicItem &rkAlchemicItem );

	void AlchemicItemAddNew( int iCode );

public:
	bool HasAlchemicItem( int iCode );

	// ȹ��
	bool GainAlchemicItem( int iCode, int iCount );

	// �Ҹ�
	bool UseAlchemicItem( int iCode, int iCount );

	// ��ĭ�ִ��� üũ
	bool CheckEmptySlot();

	// ���簹�� üũ
	int GetAlchemicItemCnt( int iCode );

	// ����ȹ��
	bool GetAlchemicItemInfo( vAlchemicItemInfoList &rkInfoList );

public:
	ioAlchemicInventory(void);
	virtual ~ioAlchemicInventory(void);
};


