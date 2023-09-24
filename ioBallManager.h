#pragma once

class ioWeapon;
class ioBaseChar;
class ioPlayEntity;
class SP2Packet;
class ioINILoader;
class ioPlayStage;
class ioPlayMode;
class ioBall;

class ioBallManager : public Singleton< ioBallManager >
{
protected:
	typedef std::vector< ioBall* > vBallList;
	vBallList m_vBallStructList;

	ioPlayStage *m_pPlayStage;

	int m_iLocalIndex;

public:
	ioBall* CreateBallStruct( ioHashString szStructName, ioPlayMode *pMode );

	void GetBallStructInfo( SP2Packet &rkPacket );
	void ApplyBallStructInfo( SP2Packet &rkPacket );

	void OnBallStructReposition( SP2Packet &rkPacket );

	void DestroyBallStructList();

public:
	ioBall* FindBallStruct( int iArray );	// ���������� ã����
	ioBall* GetBallStruct( int iIndex );	// list ������� �����ö�

public:
	inline int GetBallStructCnt() const { return m_vBallStructList.size(); }
	inline int GetCurLocalIndex() const { return m_iLocalIndex; }

public:
	static ioBallManager& GetSingleton();

public:
	ioBallManager( ioPlayStage *pStage );
	virtual ~ioBallManager();
};

#define g_BallStructMgr ioBallManager::GetSingleton()

