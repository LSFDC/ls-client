#pragma once

#include <vector>


// ����� ��ɾ� ����
enum ReservedTaskTypes
{
	RESERVEDCMD_TYPE_NONE = 0,
	RESERVEDCMD_TYPE_SERVERCHANGE,
	RESERVEDCMD_TYPE_ATTENDANCE,
	RESERVEDCMD_TYPE_PCROOOM,
	RESERVEDCMD_TYPE_TOURNAMENT,
	RESERVEDCMD_TYPE_ROLLBOOK,
	RESERVEDCMD_TYPE_GUILD_RANK_REWARD,
	RESERVEDCMD_TYPE_END,
};

// ����� ��ɾ� ����
enum ReservedTaskWhens
{
	RESERVEDCMD_WHEN_NONE = 0,
	RESERVEDCMD_WHEN_ENTERLOBBY,				// �κ� �������� �� ����
	RESERVEDCMD_WHEN_ENTERROOM,					// �濡 ���������� ����
	RESERVEDCMD_WHEN_ENTERHEADQUATER,			// ���ο� ���������� ����
	RESERVEDCMD_WHEN_RECV_ATTENDANCE,
	RESERVEDCMD_WHEN_RECV_ATTENDANCE_REWARD,
	RESERVEDCMD_WHEN_RECV_PCROOOM,
	RESERVEDCMD_WHEN_RECV_TOURNAEMNT_REWARD,
	RESERVEDCMD_WHEN_ROLLBOOK,					// �����⼮��
	RESERVEDCMD_WHEN_RECV_GUILD_RANK_REWARD,	//��� ��ũ ����

	RESERVEDCMD_WHEN_END,
};

// ��ɾ� ����Ÿ�� ��� Ŭ����
struct ioTask
{
	ioTask()
	{
		eType	= RESERVEDCMD_TYPE_NONE;
		eWhen	= RESERVEDCMD_WHEN_NONE;
		bEraseAfterExecution = false;
	}
	virtual ~ioTask()
	{}

	ReservedTaskTypes eType;
	ReservedTaskWhens eWhen;
	bool bEraseAfterExecution;
};

// �����̵� ����
struct ioTaskServerMove : ioTask
{
	ioTaskServerMove()
	{
		iServerIndex	= 0;
	}
	int iServerIndex;
};

//�⼮üũ �� ���� ���� ���� Ȯ��
struct ioTaskAttendanceReward : ioTask
{
	ioTaskAttendanceReward()
	{
		iTodayPresentType = iTodayValue1 = iTodayValue2 = iAccruePresentType = iAccrueValue1 = iAccrueValue2 = 0;
		dwDate = 0;
	}
	int iTodayPresentType;
	int iTodayValue1;
	int iTodayValue2;

	int iAccruePresentType;
	int iAccrueValue1;
	int iAccrueValue2;
	DWORD dwDate;
};

//��ȸ ����
struct ioTaskTournamentReward : ioTask
{
	ioTaskTournamentReward()
	{
		AttainTourPos = 0;
		dwStartDate = dwCheerPeso = 0;
		iMyCampPos = iWinCampPos = iLadderBonusPeso = iLadderRank = iLadderPoint = 0;
		m_eQuestProgress = QP_NONE;
	}

	BYTE AttainTourPos;
	DWORD dwStartDate, dwCheerPeso;
	int iMyCampPos, iWinCampPos, iLadderBonusPeso, iLadderRank, iLadderPoint;
	ioHashString szWinnerTeamName;

	enum QuestProgress
	{
		QP_NONE,
		QP_OCURR,
		QP_COMPLETE,
	};
	QuestProgress m_eQuestProgress;
};

//�����⼮üũ Ÿ�� �� ����
struct ioTaskRollBook: ioTask
{
	ioTaskRollBook()
	{
		iTablePage = iPosition = iState = dwReceiveTime = 0;
	}
	DWORD	dwReceiveTime;
	int		iState;
	int		iTablePage;
	int		iPosition;
	
};

struct ioTaskGuildRankReward: ioTask
{
	ioTaskGuildRankReward()
	{
		iPresentType = iPresentValue1 = iPresentValue2 = 0;
	}
	int iPresentType;
	int iPresentValue1;
	int iPresentValue2;

};

// ���� ����Ÿ ���� Ŭ����
class ioReservedTask : public Singleton< ioReservedTask >
{
protected:
	typedef std::vector<ioTask*> vReservedTask;
	vReservedTask m_vReservedTask;

public:
	static ioReservedTask& GetSingleton();

public:
	const ioTask* const GetTask( ReservedTaskTypes eTaskType, ReservedTaskWhens eWhen );

	void AddReservedTask( ioTask* pTask );
	void ExecuteReservedTask( ReservedTaskWhens eWhen );

	void RemoveTask( const ReservedTaskTypes eType );
	
	int GetReservedTaskCount()	{ return m_vReservedTask.size(); }

protected:
	bool OnExecute( ioTask* pTask );
	bool OnExecuteServerChange( ioTask* pTask );
	bool OnExecuteRecvAtttendance( ioTask* pTask );
	bool OnExecuteRecvPCRoom( ioTask* pTask );
	bool OnExecuteRecvTournament( ioTask* pTask );
	bool OnExecuteRecvRollBook( ioTask* pTask );
	bool OnExecuteRecvGuldRankReward( ioTask* pTask );

public:
	void Init();
	void Destroy();

	ioReservedTask(void);
	virtual ~ioReservedTask(void);
};

#define g_RserveTask ioReservedTask::GetSingleton()