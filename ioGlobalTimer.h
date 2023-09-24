#pragma once


class SP2Packet;

#define SYNCTIME_DELAY 9000
#define MAX_SYNC_MS    150
#define PENGUIN_DELAY  1000

class ioGlobalTimer : public Singleton< ioGlobalTimer >
{
protected:
	DWORD m_server_time;
	DWORD m_current_time;
	DWORD m_send_time;
	DWORD m_recv_time;
	DWORD m_server_to_client_ping_time;

protected:
	DWORD m_server_delay_ms;    //���� ���� ms.

	int   m_packet_lose_cnt;    //UDP ��Ŷ ���� Ƚ��.
	int   m_packet_lose_send;
	int   m_packet_lose_recv;

protected:
	int   m_average_error;      //������ Ŭ���̾�Ʈ ��� ����
	int   m_total_send_index;

protected:
	bool m_bWorkPing;

public:
	int SetGlobalTime( SP2Packet &rkPacket );
	void SetWorkPing() { m_bWorkPing = true; }

public:
	void Process();

public:
	DWORD GetGlobalTime() const;

	// ���� �ð��� ����ð����� ũ�� 0�� �����Ѵ�.
	DWORD CalcGlobalGapTime( DWORD dwTime ) const;

public:
	inline int   GetAverageError() const { return m_average_error; }
	inline DWORD GetServerDelayMS() const { return m_server_delay_ms; }
	inline int   GetPacketloseCnt() const { return m_packet_lose_cnt; }	

public:
	bool CheckServerToClientPing();

#ifdef PENGUIN_PING
	//����� �߰�
protected:
	DWORD m_penguin_send_time;
public:
	void ProcessPenguin();
#endif

public:
	static ioGlobalTimer& GetSingleton();

public:
	ioGlobalTimer();
	virtual ~ioGlobalTimer();
};

#define g_GlobalTimer ioGlobalTimer::GetSingleton()

