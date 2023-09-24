#pragma once

class ioWeapon;
class ioBaseChar;
class ioPlayEntity;
class SP2Packet;
class ioINILoader;
class ioPlayStage;
class ioPlayMode;
class ioMachineStruct;

class ioMachineStructMgr : public Singleton< ioMachineStructMgr >
{
protected:
	ioINILoader m_IniLoader;
	typedef std::vector< ioMachineStruct* > vMachineList;
	vMachineList m_vMachineStructList;

	ioPlayStage *m_pPlayStage;

	int m_iLocalIndex;

public:
	inline const ioINILoader& GetINILoader() const { return m_IniLoader; }

	ioMachineStruct* CreateMachineStruct( int iMachineNum, ioPlayMode *pMode );
	void ClearDeadStructList();

	virtual void GetMachineStructInfo( SP2Packet &rkPacket );
	virtual void ApplyMachineStructInfo( SP2Packet &rkPacket );

	void DestroyMachineStructList();

public:
	ioMachineStruct* FindTakeEnableMachine( ioBaseChar *pChar );

	ioMachineStruct* FindMachineStruct( int iArray );	// ���������� ã����
	ioMachineStruct* GetMachineStruct( int iIndex );	// ���������� ��������

public:
	inline int GetMachineStructCnt() const { return m_vMachineStructList.size(); }
	inline int GetCurLocalIndex() const { return m_iLocalIndex; }

	ioPlayStage* GetPlayStage();

public:
	static ioMachineStructMgr& GetSingleton();

public:
	ioMachineStructMgr( ioPlayStage *pStage );
	virtual ~ioMachineStructMgr();
};

#define g_MachineStructMgr ioMachineStructMgr::GetSingleton()

