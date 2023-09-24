#pragma once

class ioINILoader;
class SP2Packet;

class ioRequestCharManager : public Singleton< ioRequestCharManager >
{
protected:
	typedef struct tagRequestInfo
	{
		int m_nRequestValue;
		bool m_bStart;
		bool m_bDead;
		DWORD m_dwCreateTime;
		DWORD m_dwDestroyTime;
		DWORD m_dwIncreaseTime;
		DWORD m_dwDecreaseTime;

		tagRequestInfo()
		{
			Init();
		}

		void Init()
		{
			m_nRequestValue = 0;
			m_bStart = false;
			m_bDead = false;
			m_dwCreateTime = 0;
			m_dwDestroyTime = 0;
			m_dwIncreaseTime = 0;
			m_dwDecreaseTime = 0;
		}
	}RequestInfo;

	typedef std::map<ioHashString, RequestInfo> RequestData;

	//���̺�
	RequestData m_RequestData;

	//�ð� ����
	DWORD m_dwCheckCreateTime;
	DWORD m_dwCheckDestroyTime;
	DWORD m_dwCheckIncreaseTime;
	DWORD m_dwCheckDecreaseTime;

	//�񱳰� ����
	int m_nCheckValue;
	int m_nIncreaseValue;
	int m_nDecreaseValue;

public:
	//�⺻ ������ �б�
	void LoadProperty();

public:
	void ClearData();
	void OnProcess();

public:
	void AddData( ioHashString &szName );
	void DelData( ioHashString &szName );

public:
	void IncreaseValue( ioHashString &szName );
	void DecreaseValue( ioHashString &szName );
	bool IsCheckValue( ioHashString &szName );

public:
	static ioRequestCharManager& GetSingleton();

public:
	ioRequestCharManager();
	virtual ~ioRequestCharManager();
};

#define g_RequestCharMgr ioRequestCharManager::GetSingleton()