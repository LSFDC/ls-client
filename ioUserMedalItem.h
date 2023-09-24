#pragma once

class ioUserMedalItem 
{
public:
	enum
	{
		PT_TIME		= 0,
		PT_MORTMAIN	= 1,
	};

	enum
	{
		MAX_SLOT_CNT = 1000,  // 1000�� ����
	};

	struct MEDALITEMSLOT
	{
		int m_iItemType;   // 1���� �����ϸ� �������� �ʴ´�. 0�� �������� ����.
		int m_iEquipClass; // class type, 0�̸� ������  
		int m_iPeriodType; // PT_TIME, PT_MORTMAIN
		int m_iLimitDate;  // ������� ��Ÿ�� 20090715(2009�� 7�� 15�� )  , Ŭ���̾�Ʈ�� ���̾��� ���ؼ� CTime�� ������� ����.
		int m_iLimitTime;  // �ð��� ��Ÿ�� 1232 (12��32��)

		MEDALITEMSLOT()
		{
			Init();
		}

		void Init()
		{
			m_iItemType  = 0;
			m_iEquipClass = 0;
			m_iPeriodType = PT_TIME;
			m_iLimitDate = 0;
			m_iLimitTime = 0;
		}

		// ��¥��
		SHORT GetYear()
		{
			return m_iLimitDate/10000;           // [2009]0715
		}
		SHORT GetMonth()
		{
			return ( m_iLimitDate/100 ) % 100;   //  2009[07]15
		}
		SHORT GetDay()
		{
			return m_iLimitDate % 100;           //  200907[15]
		}
		SHORT GetHour()
		{
			return m_iLimitTime / 100;           //  [21]23   ( 21�� 23�� )
		}
		SHORT GetMinute()
		{
			return m_iLimitTime % 100;           //  21[23]
		}
		void SetDate( int iYear , int iMonth, int iDay, int iHour, int iMinute )
		{
			m_iLimitDate = ( iYear * 10000 ) + ( iMonth * 100 ) + iDay;
			m_iLimitTime = ( iHour * 100 ) + iMinute;
		}
	};

protected:
	typedef std::vector< MEDALITEMSLOT > vMedalItemVec;
	vMedalItemVec m_vMedalItemVec;

public:
	void Initialize();
	void ApplyMedalItemData( SP2Packet &rkPacket );

	void AddMedalItemData( const MEDALITEMSLOT &rkNewSlot );
	bool DeleteMedalItemData( int iItemType );

	void SetMedalItem( const MEDALITEMSLOT &rkMedalItem, bool bFindNotEquipMedal = false, int nClassType = 0 );

	bool GetMedalItem( IN int iItemType, OUT MEDALITEMSLOT &rkMedalItem, bool FindNotEquipMedal = false, int nClassType = 0 );
	bool GetMedalItemArray( IN int iArray, OUT MEDALITEMSLOT &rkMedalItem );

	int GetMedalItemCnt();

	bool GetMedalItemTypeVec( OUT IntVec &rvItemTypeVec, IN int iClassType );

	void FillEquipClass( IN int iClassType, IN int iMaxSlotNum, OUT SP2Packet &rkPacket );

	void GetUseInfo( IN int iItemType, OUT ioHashString &rsInfo );

	void ReleaseEquipMedal( int iClassType );

	int  GetExtraItemCnt();
	bool IsSlotFull();

public:
	ioUserMedalItem(void);
	virtual ~ioUserMedalItem(void);
};


