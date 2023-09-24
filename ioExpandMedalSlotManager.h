#pragma once

class SP2Packet;

// �޴޽��Ծ������� ����Ͽ� ���� �޴޽��Ը��� �����ϴ� �޴��� Ŭ����

class ioExpandMedalSlotManager : public Singleton< ioExpandMedalSlotManager >
{
public:
	enum
	{
		DEFAULT_YEAR    = 2010,			// 2010���� DB�� �������� �ʴ´�. �� DateData�� �⵵�� 0�̸� 2010�̶� ���̴�. 1�̸� 2011��
		DATE_YEAR_VALUE = 100000000,    // ����� ������.
		DATE_MONTH_VALUE= 1000000,      // ������ ������.
		DATE_DAY_VALUE =  10000,        // �ϱ��� ������.
		DATE_HOUR_VALUE = 100,          // �ñ��� ������.
	};

	struct ExpandMedalSlot
	{
		BYTE	m_iSlotNumber;	// ���Թ�ȣ.
		int		m_iClassType;	// �ش�뺴.
		DWORD	m_dwLimitTime;	// ��ȿ�Ⱓ. ( ���� Ȱ��ȭ ���� �ð� )

		ExpandMedalSlot()
		{
			m_iSlotNumber	= 0;
			m_iClassType	= 0;
			m_dwLimitTime	= 0;
		}

		// ��¥�� (�� 2013.04.20.02.40)
		SHORT GetYear()
		{
			int iYear = m_dwLimitTime/DATE_YEAR_VALUE;
			iYear += DEFAULT_YEAR;
			return iYear;			// [2013]04200240
		}
		SHORT GetMonth()
		{
			return ( m_dwLimitTime/DATE_MONTH_VALUE ) % 100;	// 2013[04]200240
		}
		SHORT GetDay()
		{
			return ( m_dwLimitTime/DATE_DAY_VALUE) % 100;	// 201304[20]0240
		}
		SHORT GetHour()
		{
			return ( m_dwLimitTime/DATE_HOUR_VALUE ) % 100;		// 20130420[02]40
		}
		SHORT GetMinute()
		{
			return m_dwLimitTime % 100;				// 2013042002[40]
		}
		bool IsMortmain()
		{
			if( m_dwLimitTime == 0 )
				return true;
			else
				return false;
		}
		void SetDate( int iYear , int iMonth, int iDay, int iHour, int iMinute )
		{
			m_dwLimitTime = ( iYear * 100000000 ) + ( iMonth * 1000000 ) + ( iDay * 10000 ) + ( iHour * 100 ) + iMinute;
		}
	};
	typedef std::vector<ExpandMedalSlot>	ExpandMedalSlotVec;

	struct ReserveDeleteMedal
	{
		int m_iClassType;
		int m_iSlotNumber;
		int m_iMedalItemType;
		ReserveDeleteMedal()
		{
			Init();
		}
		void Init()
		{
			m_iClassType = 0;
			m_iSlotNumber = 0;
			m_iMedalItemType = 0;
		}

	};
	typedef std::vector<ReserveDeleteMedal> ReserveDeleteMedalVec;

protected:
	ExpandMedalSlotVec	m_vExpandSlotInfoVec;
	ReserveDeleteMedalVec m_vReserveDeleteMedal;

protected:
	void Clear();

public:
	void ApplyExpandMedalSlot( SP2Packet &rkPacket );
	void AddExpandMedalSlot( BYTE iSlotNumber, int iClassType, int iLimitTime );
	void DeleteExpandMedalSlot( SP2Packet &rkPacket );
	void DeleteMedalListByChar( int iClassType, int iSlotNumber );
	int  GetExpandMedalSlotNum( int iClassType );
	int  GetExpandMedalSlot( IN int iClassType, OUT ExpandMedalSlotVec &vExpandMedalSlot );
	bool CheckRemainTime( IN int iClassType, IN int iSlotNumber, OUT char *szRemainTime, int iRemainTimeSize );
	bool CheckExpandMedalSlot( int iClassType, int iSlotNumber );

public:
	static ioExpandMedalSlotManager& GetSingleton();

public:
	ioExpandMedalSlotManager(void);
	virtual ~ioExpandMedalSlotManager(void);
};

#define g_ExpandMedalSlotMgr ioExpandMedalSlotManager::GetSingleton()

