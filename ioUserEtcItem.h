#pragma once


class SP2Packet;
// ���� , ��, ��Ÿ ������
class ioUserEtcItem 
{
public:
	enum
	{
		MAX_SLOT   = 20,	
		ITEM_USE_ON  = 1,
		ITEM_USE_OFF = 0,
	};

	struct ETCITEMSLOT
	{
		int m_iType;       // AABBBBB // AA: 1:Ƚ����, 2:�ð���, 3:��û�� // BBBBB: ���� 1~9999 : ���� , 10000 ~ 19999 : ��(��Ȯ��), 20000 ~ 29999 : ��Ÿ(��Ȯ��) - �������� ������ 
		int m_iValue1;     // Type�� ���� ���ӻ��� �ٸ� ���� , ����ȿ�����̸� ���� Ƚ��, ��Ÿ �ð��Ҹ����̸� �����ð�
		int m_iValue2;     // ��뿩�� // 0 : �̻��, 1:���

		ETCITEMSLOT()
		{
			m_iType   = 0;
			m_iValue1 = 0;
			m_iValue2 = 0;
		}

		void AddUse( int iUse )
		{
			m_iValue1 += iUse;
		}
		int GetUse()
		{
			return m_iValue1;
		}
		bool IsUse()
		{
			if( m_iValue2 == ITEM_USE_ON )
				return true;
			
			return false;
		}
		void SetUse( bool bUse )
		{
			if( bUse )
				m_iValue2 = ITEM_USE_ON;
			else
				m_iValue2 = ITEM_USE_OFF;
		}
		// UT_DATE: ��¥��
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
		// Value2���� ��¥�� ���� 4�ڸ��� ����Ѵ�. ���� �ڸ��� Ư�� �����ۿ��� ��� -
		SHORT GetHour()
		{
			return (m_iValue2 % 10000) / 100;           //  [21]23   ( 21�� 23�� )
		}
		SHORT GetMinute()
		{
			return (m_iValue2 % 10000) % 100;           //  21[23]
		}
		void SetDate( int iYear , int iMonth, int iDay, int iHour, int iMinute )
		{
			m_iValue1 = ( iYear * 10000 ) + ( iMonth * FLOAT100 ) + iDay;
			m_iValue2 = ((m_iValue2/10000)*10000) + ( ( iHour * FLOAT100 ) + iMinute );
		}

		int GetDateExcludeValue2()
		{
			return (m_iValue2 / 10000);
		}

		void SetDateExcludeValue2( int iValue2 )
		{
			int iDate = (m_iValue2 % 10000);
			m_iValue2 = (iValue2 * 10000) + iDate;
		}

		int GetDateExcludeValue3Time()
		{
			int iValue = GetDateExcludeValue2();
			return iValue % 1000;
		}

		int GetDateExcludeValue3State()
		{
			int iValue = GetDateExcludeValue2();
			return (iValue / 10000)-1;
		}

		void SetDateExcludeValue3( int iValue, int iState )
		{
			int iDate = ( m_iValue2 % 10000 );

			//999�� �̻� ���� �Ұ�
			iValue = min( 999, iValue );

			//State���
			iState = min( 9, iState );
			iState = (iState+1) * 10000;
			m_iValue2 = ( (iState + iValue) * 10000) + iDate;
		}
	};

protected:
	struct ETCITEMDB
	{
		ETCITEMSLOT m_EtcItem[MAX_SLOT];		
		ETCITEMDB()
		{
			memset( m_EtcItem, 0, sizeof( m_EtcItem ) );
		}
	};
	typedef std::vector< ETCITEMDB > vETCITEMDB;
	vETCITEMDB m_vEtcItemList;

	typedef std::map< int, DWORD > StartTimeMap; // int : iType, DWORD : StartTime
	StartTimeMap m_StartTimeMap;

protected:
	void InsertStartTimeMap( int iType );
	void DeleteStartTimeMap( int iType );
	void SetStartTimeMap( int iType, DWORD dwStartTime );

public:
	void Initialize();
	void ApplyData( SP2Packet &rkPacket );

public:
	int  GetUse( int iType );
	bool AddEtcItem( const ETCITEMSLOT &rkNewSlot );
	bool GetEtcItem( IN int iType, OUT ETCITEMSLOT &rkEtcItem );
	bool GetEtcItemByArray( IN int iArray, OUT ETCITEMSLOT &rkEtcItem );
	bool GetEtcItemRange( IN int iStarType, IN int iEndType, OUT ETCITEMSLOT &rkEtcItem );
	int  GetEtcItemCurrentSlot();
	void SetEtcItem( const ETCITEMSLOT &rkEtcItem );
	void DeleteEtcItem( int iType );

	void ApplyTimeData( SP2Packet &rkPacket, bool bClearStartTime = false );
	void DeleteEtcItemZeroTime();
	int  GetValueByCalcTime( int iType );

	bool IsCanUse( int iType );
	void SendUse( int iType );

	int GetEtcItemCnt();

	void GetUseInfo( IN int iType, OUT ioHashString &rsInfo1, OUT ioHashString &rsInfo2, OUT DWORD &rInfo1Color, OUT DWORD &rInfo2Color );

	void UpdateGashaponTime( int iMinute );
public:
	ioUserEtcItem(void);
	virtual ~ioUserEtcItem(void);
};



