#pragma once
#include "ioMission.h"

#define MISSION_INFO_SUCCESS				1
#define MISSION_INFO_FAIL					2

#define COMPENSATION_RECV_OK				1
#define COMPENSATION_UNKNOWN_MISSION		2
#define COMPENSATION_PROGRESS				3
#define COMPENSATION_ALREADY_RECV			4
#define COMPENSATION_EXCEPTION				5

enum MissionDateType
{
	MDT_DAILY,
	MDT_WEEKLY,
	MDT_MONTHLY,
	MDT_MAX,
};

enum MissionState
{
	MIS_NONE,
	MIS_PROGRESS,	     // ������ - ���� ����
	MIS_ATTAIN,			 // �޼�   - ����Ʈ �޼������� ������ ���� �ʾ���
	MIS_COMPLETE,        // �Ϸ�   - ���� �޾���.
};

class ioMissionManager : public Singleton< ioMissionManager >
{	
protected:
	enum 
	{ 
		MACRO_TIC	= 10000,
		PROCESS_TIC = 60000,
	};
	typedef std::vector< ioMission* > vMissionList;
	vMissionList	m_CurDayMissionList[MDT_MAX];
	vMissionList	m_AllDayMissionList[MDT_MAX];

	CTime			m_RequestDateTime;	//���� �̼� ��û�� ��¥
	bool			m_bChangeMission;
	DWORD			m_dwProcessTime;
	DWORD			m_dwProcessTicTime;
public:
	//Init func
	void						Initialize();
	//process
	void						MissionProcess( const int& CurModeType );
protected:
	//Load func
	void						LoadMission( SP2Packet &rkPacket );
	//mission factory
	ioMission*					CreateMission( const int& iType );
	//Clear func
	void						ClreaCurMissionList();
	void						ClreaAllMissionList();

public:
	//Get Or Is func
	const ioMission* const		GetMissionToArray( const int& iType, const int& iArray )			const;
	const ioMission* const		GetMissionToCode( const int& iType, const int& iCode )				const;
	const ioMission* const		GetMissionToCode( const int& iCode )								const;
	int							GetMissionCountToType( const int& iType )							const;
	//Get MIS_ATTAIN Type Count
	int							GetAttinCountToType( const int& iType )	const;
	//Get MissionState Type Count
	int							GetClearTypeCountToType( const int& iType, const int& iClearType )	const;
	const bool&					IsChangeMission() const { return m_bChangeMission; } 
	
public:
	//Set func
	void						SetMissionToCode( int iCode, int iValue, BYTE iClearMissionType );
	void						ChangeMission() { m_bChangeMission = true; }
	void						SetMacroMinute( int	MacroMinute );

public: // Packet receive func
	void						ProcessTCPPacket( SP2Packet &rkPacket );
	// Packet receive when mission state change
	void						OnMissionStateChange( SP2Packet &rkPacket );
	// Packet receive when Login or NextDay
	void						OnSetMissionInfo( SP2Packet &rkPacket );
	// Packet receive when Press Compensation Btn
	void						OnCompensationReceive( SP2Packet &rkPacket );
public:
	static ioMissionManager		&GetSingleton();
public:
	ioMissionManager(void);
	virtual ~ioMissionManager(void);
};

class MissionDataSort : public std::binary_function< const ioMission*, const ioMission*, bool >
{
public:
	bool operator()( const ioMission *lhs , const ioMission *rhs ) const;
};

#define g_MissionManager ioMissionManager::GetSingleton()
