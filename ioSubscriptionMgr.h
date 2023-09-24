#pragma once

class SP2Packet;
class ioComplexStringPrinter;
class ioPlayStage;

class ioSubscriptionMgr : public Singleton< ioSubscriptionMgr >
{
public:
	struct SubscriptionData
	{
		DWORD m_dwIndex;					// DB �ε���.

		ioHashString m_szSubscriptionID;
		int m_iSubscriptionGold;
		int m_iSubscriptionBonusGold;

		short m_iPresentType;				// ( 1.�뺴 - 2.ġ�� - 3.���� - 4.��� - 5.��� )
		int m_iPresentValue1;				// �뺴:(�뺴Ÿ��), ġ��,����ġ��(ITEMSLOT�� m_item_type), ����(ETCITEMSLOT�� m_iType), ���(��ұݾ�), ���(����ڵ�)
		int m_iPresentValue2;				// �뺴:(�뺴�Ⱓ), ġ��,����ġ��(ITEMSLOT�� m_item_code), ����(ETCITEMSLOT�� m_iValue1), ���(NONE), ���(( ��� ���尪 * 10000 ) + ���Ⱓ)
											// ������(00[00]:1���,0��� | [00]00:���Array 1����99 -1 ���� ���� array ���� )
		int m_iMentType;
		DWORD m_dwLimitDate;				// ����

		SubscriptionData()
		{
			m_dwIndex = 0;
			m_iSubscriptionGold = 0;
			m_iSubscriptionBonusGold = 0;
			m_iMentType = 0;

			m_iPresentType = 0;
			m_iPresentValue1 = m_iPresentValue2 = 0;
			m_dwLimitDate = 0;
		}
	};

protected:
	typedef std::vector< SubscriptionData > vSubscriptionData;
	vSubscriptionData m_vSubscriptionList;

	int m_iSubscriptionMentType;
	int m_iSubscriptionBonusCashMentType;
	int m_iSubscriptionLimitDate;

	DWORD m_dwSubscriptionCurTimer;
	DWORD m_dwSubscriptionRequestTime;
	DWORD m_dwSubscriptionRequestCount;

protected:
	bool IsAlreadySubscription( DWORD dwIndex, const ioHashString &szSubscriptionID );
	void DeleteSubscription( DWORD dwIndex, const ioHashString &szSubscriptionID );
	
	void SendSubscriptionData();

	bool RecvEtcItem( SubscriptionData &rkData, int &iItemType, ComplexStringPrinterVec& vTitle, ComplexStringPrinterVec& vDesc, int &nGradeType );

public:
	void LoadINIInfo();

public:
	void SendSubscriptionDataToTime();
	void ProcessSubscription();

public:
	void ApplySubscriptionData( SP2Packet &rkPacket );
	void ApplySubscriptionRecv( SP2Packet &rkPacket, ioPlayStage *pStage );
	void ApplySubscriptionRetr( SP2Packet &rkPacket );

	void ApplySubscriptionDataAdd( SP2Packet &rkPacket );

public:
	bool GetSubscriptionDataToArray( IN int iArray, OUT SubscriptionData& rkData );
	bool GetSubscriptionDataToIndex( IN DWORD dwIndex, IN const ioHashString& szSubscriptionID, OUT SubscriptionData& rkData );

	bool CheckSubscriptionRetractLimit( DWORD dwIndex, const ioHashString &szSubscriptionID );

	bool CheckBuyDate( IN DWORD dwLimitDate, OUT time_t &rkBuyTime );

public:
	int GetMaxSubscription() { return m_vSubscriptionList.size(); }

public:
	static ioSubscriptionMgr& GetSingleton();

public:
	ioSubscriptionMgr();
	virtual ~ioSubscriptionMgr();
};

#define g_SubscriptionMgr ioSubscriptionMgr::GetSingleton()

