#pragma once

enum 
{
	PRESENT_DEFAULT            = 0,
	PRESENT_SOLDIER            = 1,
	PRESENT_DECORATION         = 2,
	PRESENT_ETC_ITEM           = 3,
	PRESENT_PESO               = 4,
	PRESENT_EXTRAITEM          = 5,
	PRESENT_EXTRAITEM_BOX      = 6,
	PRESENT_RANDOM_DECO		   = 7,
	PRESENT_GRADE_EXP          = 8,
	PRESENT_MEDALITEM          = 9,
	PRESENT_ALCHEMIC_ITEM	   = 10,
	PRESENT_PET				   = 11,
	PRESENT_COSTUME			   = 12,
	PRESENT_BONUS_CASH		   = 13,
	PRESENT_ACCESSORY		   = 14,
	PRESENT_COSTUME_BOX		   = 15,
	PRESENT_MAX				   = 16,
};

// Ư���� üũ�ؾ��� ��Ʈ Ÿ�Ը� ����
#define PRESENT_SPECIAL_MENT        2
#define PRESENT_LOSA_SEARCH_MENT	4

#define PRESENT_TRADE_BUY_MENT		37
#define PRESENT_TRADE_SELL_MENT		38
#define PRESENT_TRADE_CANCEL_MENT	39
#define PRESENT_TRADE_TIMEOUT_MENT	40

#define	PRESENT_POPUPITEM_NO_MENT	9999


// PresetType2 ���� ����
#define PRESENT_EXTRAITEM_DIVISION_1      100000000
#define PRESENT_EXTRAITEM_DIVISION_2      10000

#define PRESENT_INDEX_MEMORY            0

class SP2Packet;
class ioComplexStringPrinter;
class ioPlayStage;

class ioPresentMgr : public Singleton< ioPresentMgr >
{
public:
	enum
	{
		PRESENT_STATE_NORMAL = 0,      //
		PRESENT_STATE_NEW    = 1,      //
		PRESENT_STATE_DELETE = 2,      //
	};

protected:
	struct PresentData
	{
		DWORD m_dwIndex;                   // ���� DB �ε���.         // 
		DWORD m_dwSlotIndex;               // ���� SLOT �ε���.       //
		ioHashString m_szSendID;           // ���� ����
		short   m_iPresentType;            // ( 1.�뺴 - 2.ġ�� - 3.���� - 4.��� - 5.��� )
		short   m_iPresentMent;            // ( ��ƮŸ��(Ŭ���̾�Ʈ���� ����) )
		short   m_iPresentState;           // ���ο� ���� 1, �̹� ���� ���� 0, ������ ���� 2,
		int     m_iPresentValue1;          // �뺴:(�뺴Ÿ��), ġ��,����ġ��(ITEMSLOT�� m_item_type), ����(ETCITEMSLOT�� m_iType), ���(��ұݾ�), ���(����ڵ�)
		int     m_iPresentValue2;          // �뺴:(�뺴�Ⱓ), ġ��,����ġ��(ITEMSLOT�� m_item_code), ����(ETCITEMSLOT�� m_iValue1), ���(NONE), ���(( ��� ���尪 * 10000 ) + ���Ⱓ)
		                                   // ������(00[00]:1���,0��� | [00]00:���Array 1����99 -1 ���� ���� array ���� )
		int     m_iPresentValue3;          // ���:(������Ų)
		int     m_iPresentValue4;          // ���:(������Ų)
		DWORD   m_dwLimitDate;             // ���� �Ⱓ ���� : 0909141200(2009��9��14��12�ñ��� ���� ���� ����), 0�̸� ������ ����
		PresentData()
		{
			m_dwIndex = m_dwSlotIndex = 0;
			m_iPresentType = m_iPresentMent = m_iPresentState = 0;
			m_iPresentValue1 = m_iPresentValue2 = m_iPresentValue3 = m_iPresentValue4 = 0;
			m_dwLimitDate  = 0;
		}
	};
	typedef std::vector< PresentData > vPresentData;
	vPresentData m_vPresentList;
	vPresentData m_vDeletePresentList;

	DWORD m_dwPresentCurTimer;
	DWORD m_dwPresentRequestTime;
	DWORD m_dwPresentRequestCount;

protected:
	struct MentData
	{
		DWORD m_dwMentColor;
		char  m_szMentText[MAX_PATH];
		MentData()
		{
			m_dwMentColor = 0;
			memset( m_szMentText, 0, sizeof( m_szMentText ) );
		}
	};
	typedef std::vector < MentData > vMentData;
	struct MentList
	{
		vMentData m_MentList;
	};
	typedef std::vector < MentList > vMentList;
	typedef std::map< short, vMentList > MentMap;
	MentMap m_MentList;

protected:
	bool m_bPresentChange;

protected:
	bool IsAlreadyPresent( DWORD dwIndex, DWORD dwSlotIndex );
	void DeletePresent( DWORD dwIndex, DWORD dwSlotIndex );

	bool RecvEtcItem( PresentData &rkData, int &iItemType, ComplexStringPrinterVec& vTitle, ComplexStringPrinterVec& vDesc, int &nGradeType );

public:
	void LoadINIInfo();

public:
	void SendPresentData();
	void ProcessPresent();
	void SendPresentDataToTime();

public:
	void ApplyPresentData( SP2Packet &rkPacket );
	void ApplyPresentRecv( SP2Packet &rkPacket, ioPlayStage *pStage );
	void ApplyPresentSell( SP2Packet &rkPacket );

public:
	bool IsAlarmPresentData( bool bSpecialAwardBlock );
	bool IsDeletePresentData();
	bool IsPresentChange();

public:
	void ClearAlarmPresentData( DWORD dwIndex, DWORD dwSlotIndex );
	DWORD FirstAlarmPresentData( bool bSpecialAwardBlock, DWORD &rSlotIndex );

public:
	void AllRemoveDeletePresentData();
	void AllDeletePresentData();
	void RemoveDeletePresentData( DWORD dwIndex, DWORD dwSlotIndex );
	DWORD FirstDeletePresentData( DWORD &rSlotIndex );

public:
	void GetPresentDataToIndex( DWORD dwIndex, DWORD dwSlotIndex, ioHashString &rSendID, short &rPresentType, short &rPresentMent, 
								int &rPresentValue1, int &rPresentValue2, int &rPresentValue3, int &rPresentValue4, DWORD &rLimitDate );
	void GetDeletePresentDataToIndex( DWORD dwIndex, DWORD dwSlotIndex, ioHashString &rSendID, short &rPresentType, short &rPresentMent, 
						 			  int &rPresentValue1, int &rPresentValue2, int &rPresentValue3, int &rPresentValue4, DWORD &rLimitDate );

	DWORD GetPresentIndexToArray( int iArray, DWORD &rSlotIndex );

public:
	int  GetMentMaxLine( short iPresentMent );
	void GetMentLineText( short iPresentMent, int iLine, ioComplexStringPrinter &rkPrinter );

public:
	const ioHashString &GetPresentValue1Text( short iPresentType, int iPresentValue1, int iPresentValue2 );
	const ioHashString &GetPresentValue2Text( short iPresentType, int iPresentValue1, int iPresentValue2 );
	const ioHashString &GetPresentIconText( short iPresentType, int iPresentValue1, int iPresentValue2 );
	const ioHashString &GetLmitDateText( DWORD dwLimitDate );

public:
	int GetMaxPresent(){ return m_vPresentList.size(); }
	inline const DWORD& GetLimitPresentSize() const { return m_dwPresentRequestCount; }

public:
	void _DebugLogTreasureCardItem();

public:
	static ioPresentMgr& GetSingleton();

public:
	ioPresentMgr();
	virtual ~ioPresentMgr();
};

#define g_PresentMgr ioPresentMgr::GetSingleton()

