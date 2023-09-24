#pragma once

#include "EncryptValue.h"

class SP2Packet;
class QuestParent;
class QuestDataParent
{
public:
	enum
	{
		INDEX_HALF_VALUE = 10000,		// IndexData���� �߶� 2���� ������ �ִ´�.
		MAGIC_VALUE      = 100,			// MagicData���� �߶� 3���� ������ �ִ´�.
		ALARM_VALUE      = 10,			// MagicData���� �߶� 3���� ������ �ִ´�.
	};

protected:
	DWORD m_dwIndexData;           // ���� �ε��� + ���� �ε���
	DWORD m_dwDateData;            // ����/�Ϸ� �ð��� : 00�� 01�� 01�� 00�� 00��  = 2010(Default) << �� << �� << �� << �� << ��

protected:
	QuestParent *m_pLinkQuest;

public:
	DWORD GetMainIndex();
	DWORD GetSubIndex();

public:
	DWORD GetYear();
	DWORD GetMonth();
	DWORD GetDay();
	DWORD GetHour();
	DWORD GetMinute();

public:
	virtual const DWORD GetIndexData() const { return m_dwIndexData; }
	virtual const DWORD GetDateData() const { return m_dwDateData; }
	virtual QuestParent *GetLinkQuest();

public:
	void SetIndexData( DWORD dwIndexData );
	void SetDateData( DWORD dwDateData );
	void SetLinkQuest( QuestParent *pQuest );
    
public:
	const int GetPerformType() const;

public:
	QuestDataParent();
	virtual ~QuestDataParent();
};

class QuestData : public QuestDataParent
{
protected:
	CEncrypt<DWORD> m_dwValueData;           // �޼���
	DWORD m_dwMagicData;           // (�̺�Ʈ ����Ʈ��... �������� �ǹ̷� ���) + �˸��� ��� + ����(����,�޼�,�Ϸ�)  

protected:
	bool m_bChangeData;

public:
	DWORD GetCurrentData();
	DWORD GetCurrentMagicData();
	DWORD GetState();

public:
	bool  IsAlarm();
	bool  IsChangeData() { return m_bChangeData; }

public:
	void SetMainIndex( DWORD dwMainIndex );
	void SetSubIndex( DWORD dwSubIndex );
	void SetCurrentData( DWORD dwCurrentData );
	void SetCurrentMagicData( DWORD dwCurrentMagicData );
	void SetAlarm( bool bAlarm );
	void SetState( DWORD dwState );

public:
	inline const DWORD GetValueData() const { return m_dwValueData; }
	inline const DWORD GetMagicData() const { return m_dwMagicData; }
	
	inline void SetValueData( DWORD dwData ){ m_dwValueData = dwData; }
	
public:
	void SyncComplete(){ m_bChangeData = false; }

public:
	void ApplyData( SP2Packet &rkPacket );

public:
	QuestData();
	virtual ~QuestData();
};

class QuestDataSort : public std::binary_function< const QuestData*, const QuestData*, bool >
{
public:
	bool operator()( const QuestData *lhs , const QuestData *rhs ) const
	{
		if( !lhs || !rhs )
			return false;

		if( lhs->GetPerformType() > rhs->GetPerformType() )
			return true;
		else if( lhs->GetPerformType() == rhs->GetPerformType() )
		{
			if( lhs->GetDateData() > rhs->GetDateData() )
			{
				return true;
			}
		}				
		return false;
	}
};

class QuestCompleteData : public QuestDataParent
{

public:
	void ApplyData( SP2Packet &rkPacket );

public:
	QuestCompleteData();
	virtual ~QuestCompleteData();
};

class QuestCompleteDataSort : public std::binary_function< const QuestCompleteData*, const QuestCompleteData*, bool >
{
public:
	bool operator()( const QuestCompleteData *lhs , const QuestCompleteData *rhs ) const
	{
		if( !lhs || !rhs )
			return false;

		if( lhs->GetPerformType() > rhs->GetPerformType() )
			return true;
		else if( lhs->GetPerformType() == rhs->GetPerformType() )
		{
			if( lhs->GetDateData() > rhs->GetDateData() )
			{
				return true;
			}				
		}
		return false;
	}
};
//////////////////////////////////////////////////////////////////////////
class ioQuest
{
protected:
	typedef std::vector< QuestData* > vQuestInfo;
	vQuestInfo m_vItemList;

	typedef std::vector< QuestCompleteData* > vQuestCompleteInfo;
	vQuestCompleteInfo m_vCompleteList;

public:
	void ApplyQuestData( SP2Packet &rkPacket );
	void ApplyQuestCompleteData( SP2Packet &rkPacket );
	void ApplyQuestOccurData( SP2Packet &rkPacket );
	void ApplyQuestRewardData( SP2Packet &rkPacket );
	void AddQuestData( QuestData &rkData );
	void AddQuestCompleteData( QuestCompleteData &rkData );
	
public:
	bool RemoveQuestData( DWORD dwMainIndex, DWORD dwSubIndex );
	bool RemoveQuestCompleteData( DWORD dwMainIndex, DWORD dwSubIndex );
	bool RemoveQuestOneDayAll();

public:
	bool IsQuestAttain( DWORD dwMainIndex, DWORD dwSubIndex );
	bool IsQuestComplete( DWORD dwMainIndex, DWORD dwSubIndex );
	bool IsQuestIndexCheck( DWORD dwMainIndex, DWORD dwSubIndex );

public:
	/************************************************************************/
	/* ��������Ʈ�� ������ �ִ� �����̴�. m_vItemList.size()���� ���� ũ��	*/
	/************************************************************************/
	int GetMaxQuest( DWORD dwState );
	int GetMaxAlarmQuest();

public:
	QuestData &GetQuestData( DWORD dwMainIndex, DWORD dwSubIndex );
	DWORD GetQuestState( DWORD dwMainIndex, DWORD dwSubIndex );
	QuestData GetProcessToArray( int iArray );
	QuestData GetAttainNCompleteToArray( int iArray );
	QuestData GetAlarmQuestToArray( int iArray );

public: //�׽�Ʈ�� ��ũ�� 
	void ClearQuestData();

public:
	ioQuest();
	virtual ~ioQuest();
};

