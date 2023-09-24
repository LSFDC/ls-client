#pragma once

class SP2Packet;
class ioClassExpert
{
public:
	enum
	{
		MAX_SLOT   = 10,	
	};

	struct CLASSSLOT
	{
		int m_iType;				// �뺴 Ÿ�� 
		int m_iLevel;				// �뺴 ����
		int m_iExpert;				// �뺴 ����ġ
		byte m_ePowerUpCharGrade;	// �뺴 ��ȭ ���
	};

	struct ClassInfo
	{
		CLASSSLOT	m_Data[MAX_SLOT];
		ClassInfo()
		{
			memset( m_Data, 0, sizeof( m_Data ) );
		}
	};
	typedef std::vector< ClassInfo > vClassInfo;

protected:
	vClassInfo m_vClassInfo;

protected:
	void CreateClass( int iClassType );  

public:
	void Initialize();
	
public:
	void ApplyClassInfo( SP2Packet &rkPacket );
	void AddClassInfo( CLASSSLOT &kSlot );

public:
	bool AddClassExp( int iClassType, int iExp, int &rRemainExp );
	bool AddPowerUpGrade( int iClassType, byte ePowerUpCharGrade );

public:
	int  GetClassLevel( int iClassType );	
	int  GetClassExpert( int iClassType );
	int  GetTopLevelClassType();

public:
	const ioClassExpert::CLASSSLOT* GetClassExpertSlot( int iClassType );

public:
	ioClassExpert();
	virtual ~ioClassExpert();
};

