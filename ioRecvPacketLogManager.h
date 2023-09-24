#pragma once

//////////////////////////////////////////���� ��Ŷ�� ������ ����ϴ� �Ŵ���////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////jch/////////////////////////////////////////////////////////
class ioRecvInfoList
{
public:
	struct RecvPacketInfo
	{
		ioHashString szRecvInfoName;
		DWORD dwRecvTime;
		DWORD dwStartTime;
		int nCnt;

		void Init( ioHashString szName, DWORD StartTime, DWORD RecvTime )
		{
			szRecvInfoName = szName;
			dwStartTime = StartTime;
			dwRecvTime = RecvTime;
			nCnt = 1;
		}
		void AddCnt(DWORD dwTime)
		{
			nCnt++;
			dwRecvTime = dwTime;
		}
		void Reset()
		{
			nCnt = 0;
			dwStartTime = 0;
			dwRecvTime = 0;
		}
		ioHashString GetName()
		{
			return szRecvInfoName;
		}
		int GetCnt()
		{
			return nCnt;
		}
		DWORD GetRecvTime()
		{
			return dwRecvTime;
		}
		void SetStartTime( DWORD StartTime )
		{
			dwStartTime = StartTime;
		}
		DWORD GetStartTime()
		{
			return dwStartTime;
		}
	};

	typedef std::vector<RecvPacketInfo*> InfoList;

public:
	void AddInfo( ioHashString szName, DWORD Time );
	RecvPacketInfo* FindInfo( ioHashString szName );
	void PrintInfo( DWORD dwColor, ioHashString szCharName, int eType, ioHashString szRecvName, bool bReset );
	void DeleteAll();

protected:
	void ChatMsg( DWORD dwColor, const char *szID, const char *szChat );

protected:
	InfoList m_RecvList;

public:
	ioRecvInfoList();
	virtual ~ioRecvInfoList();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ioRecvPacketLogManager : public Singleton< ioRecvPacketLogManager >
{
public:	
	//���� ��Ŷ Ÿ��
	enum RecvPacketType 
	{
		RPT_Skill = 0,
		RPT_Buff,
		RPT_Dummy,
		RPT_Special,
		RPT_MAX
	};

	//ĳ���ͺ� ���� Ÿ�Ժ� ����Ʈ
	typedef std::map< RecvPacketType, ioRecvInfoList* > TypeList;
	//��� ĳ���� ����Ʈ
	typedef std::map< ioHashString, TypeList* > TotalInfoList;

protected:
	TotalInfoList m_TotalInfoList;

public:
	//��Ŷ ���� ĳ���� �̸�, ���� ��Ŷ Ÿ��, ���� ��Ŷ ����(��ų�̸�, �����̸�, �����̸�)
	void AddCharRecvInfo( ioHashString szCharName, RecvPacketType eType, ioHashString szRecvName );
	//���� ��Ŷ ���� ã��
	ioRecvInfoList::RecvPacketInfo* FindCharRecvInfo( ioHashString szCharName, RecvPacketType eType, ioHashString szRecvName );
	//���� ��Ŷ ���� ä��â�� ��� ( ���ڻ�, ĳ���� �̸�, ��Ŷ Ÿ��, Ȯ�� �� Ư�� ��Ŷ �̸�(��ų�̸�, �����̸�, �����̸�), ��� �� ī��Ʈ �ʱ�ȭ ���� )
	void PrintCharRecvInfo( DWORD dwColor, ioHashString szCharName, RecvPacketType eType, ioHashString szRecvName = "", bool bReset = false );
	void PrintChatLog( DWORD dwColor, ioHashString szCharName, ioHashString szLog = "" );
	//��� ���� ����
	void CleanRecvInfo();

public:
	static ioRecvPacketLogManager& GetSingleton();

public:
	ioRecvPacketLogManager();
	virtual ~ioRecvPacketLogManager();
};

#define g_RecvPacketLogManager ioRecvPacketLogManager::GetSingleton()