#pragma once

class CMannerTrialChat
{
protected:
	struct ChatItem
	{
		ioHashString szID;
		ioHashString szChat;
	};

	typedef std::deque< ChatItem *> ChatList;

	ChatList m_ChatList;
	int m_iMaxChat;

public:
	void DeleteAll();
	void Add( const char *szID, const char *szChat );
	void DeleteHead();
	void CheckDeleteHead();

	void SetMaxChat( int iMax );
	bool IsHaveID( const ioHashString &rszID );
	void GetChatList( OUT ioHashString &rkChatList );

public:
	CMannerTrialChat(void);
	virtual ~CMannerTrialChat(void);
};

class ioMannerTrialChatManager : public Singleton < ioMannerTrialChatManager >
{
public:
	enum TrialType
	{
		// loop ���鼭 ó���ϴ� �κ��� ����
		TT_NONE         = 0,
		TT_NORMAL_CHAT  = 1, // ��
		TT_BATTLE_CHAT  = 2, // ������ 
		TT_CHANNEL_CHAT = 3,
		TT_MEMO         = 4,
		TT_GUILD_CHAT   = 5,
		TT_SERVER_LOBBY_CHAT  = 6,
		TT_WHOLE_SERVER_CHAT  = 7,
		TT_LADDER_CHAT  = 8,
		TT_WHOLE_SERVER_RAINBOW_CHAT = 9,
		// ���Ӱ� �߰��Ǵ� ���� �Ʒ��� ���������� �߰�
		
		TT_MAX,
	};
protected:
	typedef std::map< DWORD, CMannerTrialChat* > MannerTrialChatMap;
	MannerTrialChatMap m_MannerTrialChatMap;

public:
	static ioMannerTrialChatManager& GetSingleton();

public:
	void SetMaxChat( int iMax );
	void Add( TrialType eTrialType, const char *szID, const char *szChat );
	bool IsHaveID( TrialType eTrialType, const ioHashString &rszID );
	void GetChatList( IN TrialType eTrialType, OUT ioHashString &rkChatList );
	const char *GetTrialTypeName( TrialType eTrialType );
public:
	ioMannerTrialChatManager();
	virtual ~ioMannerTrialChatManager();
};

#define g_MannerTrialChatMgr ioMannerTrialChatManager::GetSingleton()

