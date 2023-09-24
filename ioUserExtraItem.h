#pragma once



class ioBaseChar;


enum
{
	EPT_TIME		= 0,     // �ð��� ���
	EPT_MORTMAIN	= 1,     // ���� ���
	EPT_GROW_TIME   = 2,     // �ð��� ����̸鼭 ���� ������ �̺�Ʈ ���
};

enum
{
	EET_DISABLE		= 0,
	EET_NORMAL		= 1,
	EET_ENABLE		= 2,
	EET_REGISTER	= 3,
};

struct EXTRAITEMSLOT
{
	int m_iItemCode;
	int m_iReinforce;
	int m_iIndex;

	int m_PeriodType;
	int m_iValue1;     // ������� ��Ÿ�� 20090715(2009�� 7�� 15�� )
	int m_iValue2;     // �ð��� ��Ÿ�� 1232 (12��32��)

	int m_iTradeState;

	DWORD m_dwMaleCustom;	// ���� �ؽ��� Ŀ���� 
	DWORD m_dwFemaleCustom; // ���� �ؽ��� Ŀ����
	WORD m_failExp; 
	bool m_bCharEquip;

	ioHashString m_Name;
	ioHashString m_IconName;

	EXTRAITEMSLOT()
	{
		Init();
	}

	void Init()
	{
		m_iItemCode = 0;
		m_iReinforce = 0;
		m_iIndex = 0;

		m_PeriodType = EPT_TIME;
		m_iValue1 = 0;
		m_iValue2 = 0;

		m_iTradeState = EET_DISABLE;
	
		m_dwMaleCustom = 0;
		m_dwFemaleCustom = 0;
		m_failExp = 0;

		m_bCharEquip = false;
	}

	// ��¥��
	SHORT GetYear()
	{
		return m_iValue1/10000;           // [2009]0715
	}
	SHORT GetMonth()
	{
		return ( m_iValue1/100 ) % 100;   //  2009[07]15
	}
	SHORT GetDay()
	{
		return m_iValue1 % 100;           //  200907[15]
	}
	SHORT GetHour()
	{
		return m_iValue2 / FLOAT100;           //  [21]23   ( 21�� 23�� )
	}
	SHORT GetMinute()
	{
		return m_iValue2 % 100;           //  21[23]
	}
	void SetDate( int iYear , int iMonth, int iDay, int iHour, int iMinute )
	{
		m_iValue1 = ( iYear * 10000 ) + ( iMonth * FLOAT100 ) + iDay;
		m_iValue2 = ( iHour * FLOAT100 ) + iMinute;
	}
};
typedef std::vector<EXTRAITEMSLOT> ExtraItemList;


class ExtraItemSort : public std::binary_function< const EXTRAITEMSLOT&, const EXTRAITEMSLOT&, bool >
{
public:
	bool operator()( const EXTRAITEMSLOT &lhs , const EXTRAITEMSLOT &rhs ) const
	{
		if( lhs.m_iIndex < rhs.m_iIndex )
		{
			return true;
		}
		return false;
	}
};

class ioUserExtraItem
{
public:
	enum
	{
		MAX_SLOT_CNT = 500,  // 500�� ����
	};

protected:
	ExtraItemList m_vExtraItemList;

public:
	void Initialize();
	void ApplyExtraItemData( SP2Packet &rkPacket );

	void AddExtraItemData( const EXTRAITEMSLOT &rkNewSlot );
	bool DeleteExtraItemData( int iSlotIndex );

	bool SetCharEquip( int iSlotIndex, bool bEquip );
	void ClearCharEquip();

	void SetExtraItem( const EXTRAITEMSLOT &rkExtraItem );

	bool GetExtraItem( IN int iSlotIndex, OUT EXTRAITEMSLOT &rkExtraItem );
	bool GetExtraItemArray( IN int iArray, OUT EXTRAITEMSLOT &rkExtraItem );
	bool GetExtraItemByItemCode( IN int ItemCode, OUT EXTRAITEMSLOT &rkExtraItem );
	
	int GetExtraItemCnt();
	int GetExtraItemMaxSlotCnt();

	void GetUseInfo( IN int iSlotIndex, OUT ioHashString &rsInfo );

	bool HasTradeWaitItem();
	bool IsSlotFull();

	bool GetCustomIdx( DWORDVec &dwCustomVec );

public:
	void EquipItem( CHARACTERDATA &rkCharData, bool bSetEquip, bool bBulletInit = true );

public:
	void FindAllItem( IN int iItemCode, OUT ExtraItemList& rkExtraItemList );

public:
	ioUserExtraItem();
	virtual ~ioUserExtraItem();
};

