#pragma once

#include "QuestVariety.h"
#include "ioComplexStringPrinter.h"
/**************************************************************************************/
/* ����Ʈ�� �߻��� Ŭ���̾�Ʈ���� üũ ��											  */
/* ����Ʈ�� �Ϸ�� �ִ��� Ŭ���̾�Ʈ�� ���� ��� üũ�ؾ� ���� ���� �� ������	  */
/* ��ɿ� ���� ���� üũ�� �δ㽺���� �� �ִ�. �� ��� �Ǵ��� 1ȸ���ϸ� �Ϸ�Ǵ�	  */
/* (�����ϱ�) ����Ʈ�� Ŭ���̾�Ʈ���� üũ�ص� �� ���ϴ�.							  */
/**************************************************************************************/

enum QuestPerform       // ����Ʈ ���� ���
{
	QP_NORMAL = 1,      // �⺻ ����Ʈ
	QP_EVENT  = 2,      // �̺�Ʈ ����Ʈ
};

enum QuestState
{
	QS_PROGRESS = 0,    // ������ - ���� ����
	QS_ATTAIN,			// �޼� - ����Ʈ �޼������� ������ ���� �ʾ���
	QS_COMPLETE,        // �Ϸ� - ���� �޾���.
};

enum QuestOccur         // ����Ʈ �߻� ���� Ÿ�� 1
{
	QO_NONE = 0,
	QO_ENTER_LOBBY,	
	QO_ENTER_BATTLE_PVE,
	QO_ENTER_BATTLE_PVP,
	QO_BATTLE_PVP_FINAL_RESULT,
	QO_BATTLE_FINAL_RESULT,
	QO_PLAZA_ENTER,
	QO_FISHING_SUCCESS,
	QO_FISHING_FAILED,
	QO_FISHING_LEVELUP,
	QO_FISHING_SELL,
	QO_SOLDIER_ACQUIRE,
	QO_GRADE_EXP_ACQUIRE,
	QO_SOLDIER_EXP_ACQUIRE,
	QO_LOBBYNPLAZA_ENTER,
	QO_GAME_LOGIN,
	QO_PVE_ROUND_CLEAR,
	QO_ENTER_ROOM_PVE,
	QO_ENTER_ROOM_PVP,
	QO_EXCAVATION_LEVELUP,
	QO_ETCITEM_USE,
	QO_CAMP_SEASON_REWARD,        
	QO_GAME_LOGIN_DORMANT,
	QO_PCROOM_AUTHORITY,
	QO_FRIEND_RECOMMEND,        
	QO_GAME_LOGIN_DORMANT_CUSTOM,
};

enum QuestComplete   // ����Ʈ �Ϸ� ���� Ÿ�� 1
{
	QC_NONE = 0,
	QC_PVE_ROUND_CLEAR,                   
	QC_ENTER_BATTLE_PVP,                      
	QC_BATTLE_PVP_KO,                         
	QC_BATTLE_PVP_WIN,                        
	QC_GRADE_UP,							  
	QC_TIME_GROWTH_TRY,                       
	QC_TIME_GROWTH_SUCCESS,                   
	QC_PESO_GROWTH_TRY,                       
	QC_BUY_ETC_ITEM,	                      
	QC_FISHING_TRY,                           
	QC_FISHING_SUCCESS,                       
	QC_FISHING_FAILED,                        
	QC_FISHING_LEVELUP,                       
	QC_FISHING_SELL_PESO,                     
	QC_FISHING_SUCCESS_ITEM,                  
	QC_BATTLE_PVP_AWARD_ROW_ACQUIRE,          
	QC_SOLDIER_PRACTICE_SUCCESS,               
	QC_BUY_EXTRA_ITEM,                        
	QC_EXTRA_ITEM_REINFORCE_SUCCESS,          
	QC_SLODIER_LEVELUP,
	QC_ENTER_PLAZA,
	QC_MANUAL_CLOSE,
	QC_GAME_LOGIN,                      
	QC_BATTLE_PVE_KO,   
	QC_PICK_ITEM,
	QC_TUTORIAL_CLEAR,
	QC_PRESENT_RECV,
	QC_CAMP_JOIN,                  
	QC_ENTER_CAMP_BATTLE,                    
	QC_CAMP_BATTLE_KO,                         
	QC_CAMP_BATTLE_WIN,     
	QC_CAMP_SEASON_REWARD,        
	QC_AWARD_ACQUIRE, 
	QC_PRISONER_DROP,
	QC_PRISONER_SAVE,
	QC_ENTER_HEADQUARTER,
	QC_BATTLE_EXP_ACQUIRE,
	QC_BATTLE_PVP_FINALRESULT_SHOW,                    
	QC_DROP_KILL,    
	QC_MULTI_KILL,
	QC_ENTER_LOBBY,
	QC_ENTER_BATTLE_OBSERVER,
	QC_ETCITEM_USE,
	QC_FRIEND_REQUEST,
	QC_FRIEND_MAKE,
	QC_STONE_ATTACK,
	QC_OTHER_KING_ATTACK,
	QC_CROWN_HOLD_TIME,
	QC_BOSSMODE_BECOME_BOSS,
	QC_BOSSMODE_BOSS_WITH_KILL,
	QC_SOLDIER_ACQUIRE,
	QC_ATTACK_HIT,
	QC_ATTACK_DEFENSE,
	QC_CAMP_EXP_ACQUIRE,
	QC_CAMP_FINALRESULT_SHOW,     
	QC_QUICK_START_OPTION_SELECT,
	QC_SOCCER_BALL_HIT,
	QC_SOCCER_GOAL,
	QC_SOCCER_ASSIST,
	QC_SOCCER_SAVE,
	QC_EXCAVATION_TRY,
	QC_EXCAVATION_SUCCESS,
	QC_BATTLE_PVE_TEAM_KO,
	QC_EXCAVATION_FAIL,
	QC_SCREEN_SHOT,
	QC_MOVIE_MODE,
	QC_MAKE_MOVIE,
	QC_EXTRA_ITEM_ACQUIRE,
	QC_EXTRA_ITEM_EQUIP,
	QC_EXTRA_ITEM_EQUIP_KO,
	QC_EXCAVATION_LEVELUP,
	QC_GANGSI_KILL,
	QC_GANGSI_HUMAN_KILL,
	QC_GANGSI_ALIVE_TIME,
	QC_GANGSI_HUMAN_WIN,
	QC_HIT_ATTACK_ATTRIBUTE,
	QC_PCROOM_LOGIN,
	QC_CHAMPION_AI_CLEAR,
};

#define QUEST_COMPLETE_HELP_ADD        100000
#define QUEST_PURPOSE_HELP_ADD         200000

class ioPlayStage;
class ioQuestManager : public Singleton< ioQuestManager >
{
protected:
	typedef std::vector< QuestParent * > vQuestVariety;
	vQuestVariety m_QuestVariety;
	vQuestVariety m_QuestOccurList;
	vQuestVariety m_QuestAttainList;

protected:
	struct QuestDelAndFail
	{
		bool m_bDelete;
		QuestParent *m_pQuestParent;
		QuestDelAndFail()
		{
			m_bDelete = false;
			m_pQuestParent = NULL;
		}
	};
	typedef std::vector< QuestDelAndFail > vQuestDelAndFail;
	vQuestDelAndFail m_QuestDelAndFailList;

	// ���� ����Ʈ - ������ ������.
protected:
	struct QuestReward
	{
		int  m_iPresentType;
		int  m_iPresentValue1;
		int  m_iPresentValue2;
		bool m_bDirectReward;
		QuestReward()
		{
			m_iPresentType = m_iPresentValue1 = m_iPresentValue2 = 0;
			m_bDirectReward = false;
		}
	};
	typedef std::map< DWORD, QuestReward > QuestRewardMap;
	QuestRewardMap m_QuestRewardMap;

	// ����Ʈ ����
protected:
	struct QuestHelp
	{
		int m_iOffSetX;
		int m_iOffSetY; 
		ioUIRenderImage *m_pImage;
		ioComplexStringPrinter m_szHelp;
		QuestHelp()
		{
			m_pImage = NULL;
			m_iOffSetX = m_iOffSetY = 0;
		}
	};
	typedef std::vector< QuestHelp* > vQuestHelp;
	struct QuestHelpList
	{
		vQuestHelp m_QuestHelp;
	};
	typedef std::map< DWORD, QuestHelpList > QuestHelpListMap;
	QuestHelpListMap m_QuestHelpListMap;
	ioUIRenderImage *m_pQuestHelpOver;

	// ����Ʈ �Ϸ� ����
protected:
	struct CompleteReserve
	{
		QuestComplete m_eComplete;
		int m_iValue1;
		int m_iValue2;
		int m_iValue3;

		CompleteReserve()
		{
			m_eComplete = QC_NONE;
			m_iValue1 = m_iValue2 = m_iValue3 = 0;
		}
	};
	typedef std::vector< CompleteReserve > vCompleteReserve;
	vCompleteReserve m_CompleteReserve;

protected:
	bool m_bQuestInfoChange;
	bool m_bQuestAlarmChange;

protected:
	ioPlayStage *m_pPlayStage;

protected:
	void ClearQuestVariety();
	void ClearQuestHelp();
	void EraseCompleteReserve( QuestComplete eComplete, int iValue1, int iValue2, int iValue3 ); 

protected:
	QuestParent *CreateQuest( const ioHashString &rClassName );

public:
	void LoadINIData();

public:
	void SetPlayStage( ioPlayStage *pPlayStage );
	void QuestOccurTerm( QuestOccur eOccur, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );                // ����Ʈ �߻�
	void QuestCompleteTerm( QuestComplete eComplete, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );       // ����Ʈ �Ϸ� üũ
	void QuestCompleteTermReserve( QuestComplete eComplete, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0 );// ����Ʈ �Ϸ� ����

public:
	void QuestAlreadyAttain( DWORD dwMainIndex, DWORD dwSubIndex );      //����Ʈ�� �������� �� �̹� �Ϸ�� ����Ʈ���� Ȯ��
	void QuestCompleteNextOccur( DWORD dwMainIndex, DWORD dwSubIndex );  //����Ʈ �Ϸ� �� ���� ����Ʈ�� �ִ��� Ȯ��

public:
	QuestParent *GetQuest( DWORD dwMainIndex, DWORD dwSubIndex );

public:
	void AddQuestOccurList( QuestParent *pQuestParent );
	QuestParent *FirstQuestOccurList();
	void EraseQuestOccurList( DWORD dwMainIndex, DWORD dwSubIndex );

public:
	void AddQuestAttainList( QuestParent *pQuestParent );
	QuestParent *FirstQuestAttainList();
	void EraseQuestAttainList( DWORD dwMainIndex, DWORD dwSubIndex );

public:
	void AddQuestDelAndFailList( bool bDelete, QuestParent *pQuestParent );
	QuestParent *FirstQuestDelAndFailList( bool &rbDelete );
	void EraseQuestDelAndFailList( DWORD dwMainIndex, DWORD dwSubIndex );

public:
	void QuestInfoChange();
	bool IsQuestInfoChange();
	void QuestAlarmChange();
	bool IsQuestAlarmChange();

public:
	bool IsInvitedPass();

public:
	bool IsAllDirectPresent( QuestParent *pQuestParent );
	const ioHashString &GetPresentName( DWORD dwIndex );
	void GetPresentData( DWORD dwIndex, int &rPresentType, int &rPresentValue1, int &rPresentValue2, bool &rPresentDirect );
	void DirectPresentAlarm( DWORD dwMainIndex, DWORD dwSubIndex, SP2Packet &rkPacket );
	
public:
	int GetMaxQuestHelp( QuestParent *pQuestParent, DWORD dwState );
	int GetMaxQuestPurposeHelp( QuestParent *pQuestParent );
	float GetMaxQuestHelpWidth( QuestParent *pQuestParent, DWORD dwState );
	void RenderQuestProgressHelp( QuestParent *pQuestParent, int iXPos, int iYPos, int iScrollPos, int iMaxLine, float fCurWidth );
	void RenderQuestPurposeHelp( QuestParent *pQuestParent, int iXPos, int iYPos );
	void RenderQuestCompleteHelp( QuestParent *pQuestParent, int iXPos, int iYPos );

public:
	void Process();
	void ProcessChangeStage();
	void ProcessCompleteReserve();

public:
	bool IsOwnerCharExtraItem( DWORD dwItemCode );

public:
	static ioQuestManager& GetSingleton();

public:   
	ioQuestManager();
	virtual ~ioQuestManager();
};
#define g_QuestMgr ioQuestManager::GetSingleton()
