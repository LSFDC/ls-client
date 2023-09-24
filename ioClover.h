
#pragma once

class ioClover
{
public:
	enum GIFTCLOVER
	{
		GIFT_CLOVER_DEFAULT					= 0,
		GIFT_CLOVER_LOGIN,					// �α���
		GIFT_CLOVER_CHARGE_SUCCESS,			// ���� ����
		GIFT_CLOVER_CHARGE_FAIL,			// ���� ����
		GIFT_CLOVER_REFILL,					// ��ü����
	};

	enum FRIENDCLOVER
	{
		FRIEND_CLOVER_DEFAULT						= 10,

		FRIEND_CLOVER_SEND_SUCCESS,					// Ŭ�ι� ������ ����
		FRIEND_CLOVER_SEND_FAIL_ALREADY_SEND,		// Ŭ�ι� ������ ���� - ���� �� �ִ� �ð��� ���� �ȵ���.
		FRIEND_CLOVER_SEND_FAIL_NOT_FRIEND,			// Ŭ�ι� ������ ���� - ģ���� �ƴ�.
		FRIEND_CLOVER_SEND_FAIL_FRIEND_REG_TIME,	// Ŭ�ι� ������ ���� - ģ���� ���� �� �� �� ����.
		FRIEND_CLOVER_SEND_FAIL_NOT_ENOUGH_CLOVER,	// Ŭ�ι� ������ ���� - Ŭ�ι� ����

		FRIEND_CLOVER_RECV_SUCCESS,					// Ŭ�ι� �ޱ� ����
		FRIEND_CLOVER_RECV_FAIL_NOT_CLOVER,			// Ŭ�ι� �ޱ� ���� - ���� Ŭ�ι��� ����.
		FRIEND_CLOVER_RECV_FAIL_NOT_FRIEND,			// Ŭ�ι� �ޱ� ���� - ģ���� �ƴ�.

		FRIEND_CLOVER_COME_TO_FRIEND,				// ģ���� ���� Ŭ�ι��� �����ߴ�.
	};

	enum
	{
		DAY_SEC = 86400,
		HOUR_SEC = 3600,
		MINUTE_SEC = 60,
	};

	enum
	{
		GIFT_CLOVER_MAX = 5,
		PROCESS_TIME    = 1000,
	};

private:
	int	m_iGiftCloverCount;	// ������ �ִ� Ŭ�ι� ����.
	int m_iRemainChargeTime;	// ������ ���� �ð� (��).
	int m_iRemainChargeSec;	// ������ ���� �ð� (��).
	DWORD m_dwCheckTime;

	CEncrypt<int> m_iEnableSendTime;
	CEncrypt<int> m_iEnableRecvTime;
	CEncrypt<int> m_iAbusingTime;
	CEncrypt<int> m_iChargingTime;
	CEncrypt<int> m_iChargingCount;

	bool m_bRequestServer;

public:
	void Initialize();
	void LoadINI();

public:
	void UpdateClover();

	void ApplyCloverInfo( SP2Packet &rkPacket );
	void ApplyEtcItemClover( SP2Packet &rkPacket );

	bool CheckSendRemainTime( const ioHashString &szUserName );
	bool CheckFriendRegTime( const ioHashString &szUserName );
	bool EnableSendClover( const ioHashString &szUserName );
	bool EnableRecvClover( const ioHashString &szUserName );

	int GetRemainChargeMinute();
	int GetRemainChargeSecond();
	void GetSendRemainTime( IN const ioHashString &szUserName, OUT ioHashString &szRemainTime );
	void GetRecvRemainTime( IN const ioHashString &szUserName, OUT ioHashString &szRemainTime );
	void GetFriendRegRemainTime( IN const ioHashString &szUserName, OUT ioHashString &szRemainTime );
	int GetRecvCloverCount();
	const ioHashString &GetRemainString( int iRemainSec );

	const int GetChargingTime() { return m_iChargingTime; }
	const int GetChargingCount() { return m_iChargingCount; }
	const int GetSendTimeGap() { return m_iEnableSendTime; }
	const int GetRecvTimeGap() { return m_iEnableRecvTime; }
	const int GetAbusingTime() { return m_iAbusingTime; }

protected:
	void SendChargeCloverReqeust();

public:
	const int GetCurGiftCloverCount(){ return m_iGiftCloverCount; }
	const int GetMaxGiftCloverCount(){ return GIFT_CLOVER_MAX; }

	void SetCurGiftCloverCount( int iGiftCloverCount ){ m_iGiftCloverCount = iGiftCloverCount; }

	const bool IsMaxCharge();

public:
	ioClover();
	virtual ~ioClover();
};
