#pragma once

#include "ioEventManager.h"

class SP2Packet;
class ioPlayStage;

// user�� ������ �ִ� event
class EventUserNode
{
protected:
	EventType m_eEventType;
	IntVec    m_ValueVec;

public:
	int  GetSize() const;
	void SetSize(int iSize );
	int  GetValue( int iArray );
	void SetValue( int iArray, int iValue );

public:
	virtual EventType GetType() const;
	virtual void Init();
	virtual int  GetAddSize();
	virtual bool IsEmptyValue();
	virtual void OnRecievePacket( SP2Packet &rkPacket, ioPlayStage *pPlayStage );
	void SetType( EventType eEventType );

public:
	EventUserNode();
	EventUserNode( EventType eEventType );
	virtual ~EventUserNode();
};

//-------------------------------------------------------------------------------------------------------------------------------
// class �߰��ÿ� CreatEventUserNode() ���� �߰��� ��
enum
{
	// EVT_PROPOSAL 
	// vec array
	VA_PROPOSAL_CNT       = 0, // from db
	VA_GAVE_CHAR          = 1, // from db
	VA_ADD_PROPOSAL_CNT   = 2, // from memory
	VA_ADD_GAVE_CHAR_CNT  = 3, // from memory
	// add size
	ADD_PROPOSAL_SIZE     = 2, // from memory ������ 2�� �߰���

	// EVT_COIN
	// vec array
	VA_COIN_CNT           = 0, // from db
	VA_PLAY_SEC           = 1, // from db
	VA_ADD_COIN_CNT       = 2, // from memory
	// add size
	ADD_COIN_SIZE         = 1, // from memory ������ 1�� �߰���

	// EVT_PLAYTIME
	  // vec array
	VA_PLAYTIME_PLAY_SEC  = 0, // from db
	VA_PLAYTIME_GET_GIFT  = 1, // from db    11111 : 0�̸� ������ 1�̸� ���� 10000�̸� 5��° ������ �����ϰ� �������� ������

	// EVT_CHANCE_MORTMAIN_CHAR
	// vec array
	VA_CMC_MAGIC_CODE     = 0, // from db   >= 0�̸� �÷����� �ð��̰� -1�� �Ϲ� ���� -2�� �Ǿ��� ���� -3�� �Ϲ�/�Ǿ��� ���� 2�� ����
	VA_CMC_MORTMAIN_CNT   = 1, // from db   ȹ���� �����뺴 ī��Ʈ

	//EVT_ONE_DAY_GOLD_ITEM
	// vec array
	VA_GOLD_ITEM_RECV_DATE = 0, // from db   >= ���������� ���� ��:�� ����

    //EV_PLAYTIME_PRESENT_TIME
	  // vec array
    VA_PLAYTIME_PRESENT_TIME_CNT = 0, // from db   >= 0�̸� �÷����� �ð��̰� -1�� ������ ���� �� �ִ� 1���� ��ȸ

	//EVT_CHRISTMAS
		// vec array
	VA_CHRISTMAS_GET_GIFT_DATE   = 0, // from db   ������ ���� ��¥

	//EVT_CONNECTION_TIME
		// vec array
	VA_CT_GET_CHANCE_DATE = 0, // from db ������ ���� ��¥
	VA_CT_IS_CHANCE       = 1, // form db 1�̸� ������ �ְ� 0�̸� ������ ����

	//EVT_ONE_DAY_GIFT
	   // vec array
	VA_OG_GET_GIFT        = 0, // from db 0:�̸� ������ �޾Ҵ�, 1:�̸� ������ ���� �ʾҴ�.

	//EVT_GRADEUP_PRESENT
		// vec array
	VA_GP_CAN_RECEIVE_GIFT = 0, // from memory 1�̸� ������ ���� �� �ְ� 0�̸� ������ �� �޴´�.

	//EVT_CONNECTION_TIME_SELECT_GIFT
		// vec array
	VA_CTSG_GET_CHANCE_DATE = 0, // from db ������ ���� ��¥
	VA_CTSG_IS_CHANCE       = 1, // form db 1�̸� ������ �ְ� 0�̸� ������ ����.

	//EVT_ENTRY
	  // vec array
	VA_E_GET_GIFT           = 0, // from db 1:�̸� ������ �޾Ҵ�, 0:�̸� ������ ���� �ʾҴ�.
	                             // 1�� array DB�� ���̹Ƿ� �޸𸮰��� 2������ ����
	VA_E_CAN_RECEIVE_GIFT   = 2, // from memory 1:�̸� ������ ���� �����̴�. 0:�̸� ������ ���� ������ �ȵȴ�.
	  // add size
	ADD_ENTRY_SIZE          = 1, // from memory ������ 1�� �߰���

	//EVT_CONNECT_AND_PLAYTIME
		// vec array
	VA_CAP_CONNECT_RESET_DATE = 0, // from db [0000]0000 : ���� ����Ʈ�� ���� ����,    0000[0000] : ����Ʈ�� ���µ� ����.
	VA_CAP_POINT_AND_SEC      = 1, // from db [0000]0000 : �������ӽð����� ��������Ʈ, 0000[0000] : ���� ���� ��
};

class ProposalEventUserNode : public EventUserNode
{
public:
	virtual void Init();
	virtual int GetAddSize();

public:
	bool IsGiveChar();
	void SetValueGiveChar();

public:
	ProposalEventUserNode();
	virtual ~ProposalEventUserNode();
};

//---------------------------------------------------------------------------------------------------------------------------------
class CoinEventUserNode : public EventUserNode
{
public:
	virtual void Init();
	virtual int  GetAddSize();
	virtual void OnRecievePacket( SP2Packet &rkPacket, ioPlayStage *pPlayStage );

public:
	CoinEventUserNode();
	virtual ~CoinEventUserNode();
};

//---------------------------------------------------------------------------------------------------------------------------------
class ExpEventUserNode : public EventUserNode
{
public:
	virtual bool IsEmptyValue();
public:
	ExpEventUserNode();
	virtual ~ExpEventUserNode();
};

//--------------------------------------------------------------------------------------------------------------------------------
class PesoEventUserNode : public EventUserNode
{
public:
	virtual bool IsEmptyValue();
public:
	PesoEventUserNode();
	virtual ~PesoEventUserNode();
};

//--------------------------------------------------------------------------------------------------------------------------------
class FishingEventUserNode : public EventUserNode
{
public:
	virtual bool IsEmptyValue();
public:
	FishingEventUserNode();
	virtual ~FishingEventUserNode();
};

//-------------------------------------------------------------------------------------------------------------------------------
// VA_PLAYTIME_GET_GIFT : XXXXX �ڸ��� ����ϸ� 1�ڸ� ���� gift 1�� ������(10->2, 100->3, 1000->4, 10000->5) , 11111�� 
// �ȳ� �޼����� ���� ���� ��Ÿ��, 22222�� ��ǰ�� ���� ���� ���� ��Ÿ��, 00112�� gift 1�� ����߰� 2~3���� �ȳ� �޼����� ���´ٴ� ǥ��
class PlayTimeEventUserNode : public EventUserNode
{
public:
	enum GiftType 
	{
		GT_1_PESO    = 10001,
		GT_2_PESO    = 10002,
		GT_3_SOLDIER = 10003,
		GT_4_SOLDIER = 10004,
		GT_5_SOLDIER = 10005, 
	};

	enum GiftState
	{
		GS_NONE   = 0,
		GS_NOTICE = 1,
		GS_USED   = 2,
	};
public:
	virtual void OnRecievePacket( SP2Packet &rkPacket, ioPlayStage *pPlayStage );

public:
	void SetGift( GiftType eType );
	bool IsGift( GiftType eType );
	int GetLimitSecond( GiftType eType );
	bool IsGiftData( GiftType eType, GiftState eGiftState );
	
protected:
	void SetGiftData( GiftType eGiftType , GiftState eGiftState );

public:
	PlayTimeEventUserNode();
	virtual ~PlayTimeEventUserNode();
};

//-------------------------------------------------------------------------------------------------------------------------------
// ��̳� �̺�Ʈ ���
class ChildrenDayEventUserNode : public EventUserNode
{
public:
	virtual bool IsEmptyValue();

public:
	ChildrenDayEventUserNode();
	virtual ~ChildrenDayEventUserNode();
};

//-------------------------------------------------------------------------------------------------------------------------------
// ���� �뺴 �̺�Ʈ ���
class PesoBonusEventUserNode : public EventUserNode
{
public:
	virtual bool IsEmptyValue();
	virtual void OnRecievePacket( SP2Packet &rkPacket, ioPlayStage *pPlayStage );

public:
	PesoBonusEventUserNode();
	virtual ~PesoBonusEventUserNode();
};

//---------------------------------------------------------------------------------------------------------------------------------
// 2�ð� �뺴 �������� ���� �����ϴ� �̺�Ʈ
class BuyCharNoLevelLimitEventUserNode : public EventUserNode
{
public:
	virtual bool IsEmptyValue();

public:
	bool IsNoLevelLimit( bool bCharBuyPeso, int iCharLimitSecond, int iCharPeriodType );
	bool IsAliveUserNode();

public:
	BuyCharNoLevelLimitEventUserNode();
	virtual ~BuyCharNoLevelLimitEventUserNode();
};

//----------------------------------------------------------------------------------------------------------------------------------
// �뺴�� �������� ��� �����ϴ� �̺�Ʈ
class GradeUpEventUserNode : public EventUserNode
{
public:
	virtual bool IsEmptyValue();

public:
	bool IsGift();
	int  GetBounsPeso();

public:
	GradeUpEventUserNode();
	virtual ~GradeUpEventUserNode();
};

//-------------------------------------------------------------------------------------------------------------------------------
// �Ǿ��� �̺�Ʈ ���
class PCRoomEventUserNode : public EventUserNode
{
public:
	virtual bool IsEmptyValue();

public:
	PCRoomEventUserNode();
	virtual ~PCRoomEventUserNode();
};

//-------------------------------------------------------------------------------------------------------------------------------
// ����! �����뺴 �̺�Ʈ ���
class ChanceMortmainCharEventUserNode : public EventUserNode
{
public:
	virtual void Init();
	virtual void OnRecievePacket( SP2Packet &rkPacket, ioPlayStage *pPlayStage );

public:
	ChanceMortmainCharEventUserNode();
	virtual ~ChanceMortmainCharEventUserNode();
};

//-------------------------------------------------------------------------------------------------------------------------------
// ���ϸ��� ��� ������~�� �̺�Ʈ ���
class OneDayGoldItemEvent : public EventUserNode
{
public:
	virtual void Init();
	virtual void OnRecievePacket( SP2Packet &rkPacket, ioPlayStage *pPlayStage );

public:
	OneDayGoldItemEvent();
	virtual ~OneDayGoldItemEvent();
};
//-------------------------------------------------------------------------------------------------------------------------------
// �޸����� ComeBack ���ʽ�
class DormancyUserEvent : public EventUserNode
{
public:
	virtual bool IsEmptyValue();

public:
	DormancyUserEvent();
	virtual ~DormancyUserEvent();
};
//----------------------------------------------------------------------------------------------------------------------------------
class PlayTimePresentEventUserNode : public EventUserNode
{
public:
	enum 
	{
		CHANCE_GET_PRESENT = -1,
	};
public:
	virtual void Init();
	virtual void OnRecievePacket( SP2Packet &rkPacket, ioPlayStage *pPlayStage );

public:
	PlayTimePresentEventUserNode();
	virtual ~PlayTimePresentEventUserNode();
};
//-----------------------------------------------------------------------------------------------------------------------------------
class ChristmasEventUserNode : public EventUserNode
{
protected:
	DWORD m_dwSendTimer;

public:
	virtual void Init();
	virtual void OnRecievePacket( SP2Packet &rkPacket, ioPlayStage *pPlayStage );

	void SendRequestGift();

public:
	ChristmasEventUserNode();
	virtual ~ChristmasEventUserNode();
};
//---------------------------------------------------------------------------------------------------------------------------------
class BuyItemEventUserNode : public EventUserNode
{
public:
	virtual bool IsEmptyValue();

public:
	BuyItemEventUserNode();
	virtual ~BuyItemEventUserNode();
};
//-----------------------------------------------------------------------------------------------------------------------------------
// ���ο��� ü��뺴�� ����� �� �ִ� �̺�Ʈ
class ExerciseSoldierEventUserNode : public EventUserNode
{
public:
	virtual bool IsEmptyValue();

public:
	ExerciseSoldierEventUserNode();
	virtual ~ExerciseSoldierEventUserNode();
};
//----------------------------------------------------------------------------------------------------------------------------------
class ConnectionTimeEventUserNode : public EventUserNode
{
public:
	virtual void Init();
	virtual void OnRecievePacket( SP2Packet &rkPacket, ioPlayStage *pPlayStage );

public:
	ConnectionTimeEventUserNode();
	virtual ~ConnectionTimeEventUserNode();
};
//----------------------------------------------------------------------------------------------------------------------------------
class OneDayGiftEventUserNode : public EventUserNode
{
public:
	virtual void Init();

public:
	OneDayGiftEventUserNode();
	virtual ~OneDayGiftEventUserNode();
};
//----------------------------------------------------------------------------------------------------------------------------------
class GradePresentEventUserNode : public EventUserNode
{
public:
	virtual void Init();

public:
	GradePresentEventUserNode();
	virtual ~GradePresentEventUserNode();
};
//----------------------------------------------------------------------------------------------------------------------------------
class ConnectionTimeSelectGiftEventUserNode : public EventUserNode
{
public:
	virtual void Init();
	virtual void OnRecievePacket( SP2Packet &rkPacket, ioPlayStage *pPlayStage );

public:
	ConnectionTimeSelectGiftEventUserNode();
	virtual ~ConnectionTimeSelectGiftEventUserNode();
};

//-----------------------------------------------------------------------------------------------------------------------------------
class EntryEventUserNode : public EventUserNode
{
public:
	virtual void Init();
	virtual int  GetAddSize();

public:
	EntryEventUserNode();
	virtual ~EntryEventUserNode();
};

//---------------------------------------------------------------------------------------------------------------------------------------
class LadderPointEventUserNode : public ExpEventUserNode 
{
public:
	LadderPointEventUserNode();
	virtual ~LadderPointEventUserNode();
};

//-----------------------------------------------------------------------------------------------------------------------------------
class ConnectAndPlayTimeEventUserNode : public EventUserNode
{
public:
	virtual void Init();
	virtual void OnRecievePacket( SP2Packet &rkPacket, ioPlayStage *pPlayStage );

public:
	ConnectAndPlayTimeEventUserNode();
	virtual ~ConnectAndPlayTimeEventUserNode();
};


//----------------------------------------------------------------------------------------------------------------------------------

class FreeDayEventUserNode : public EventUserNode
{
public:
	FreeDayEventUserNode();
	virtual ~FreeDayEventUserNode();
	virtual void OnRecievePacket( SP2Packet &rkPacket, ioPlayStage *pPlayStage );
};

//----------------------------------------------------------------------------------------------------------------------------------


class HeroExpBoostEventUserNode : public EventUserNode
{
public:
	HeroExpBoostEventUserNode();
	virtual ~HeroExpBoostEventUserNode();
	virtual void OnRecievePacket( SP2Packet &rkPacket, ioPlayStage *pPlayStage );
};