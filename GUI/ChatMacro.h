#pragma once

// �̺�Ʈ�� �����Ѵ�.
class ChatMacro
{
public:
	enum
	{
		// Ÿ���� array ���ǹǷ� ���� �������� ����.
		NONE_MACRO = -1,
		MACRO_HELP = 0,						//    ��ũ�� �Է��� �߸� �Ǿ���.
		MACRO_QUEST_GUIDE,					//    ����Ʈ���̵�
		MACRO_QUEST_OCCUR,					//    ����Ʈ�߻�
		MACRO_QUEST_ATTAIN,					//    ����Ʈ�޼�
		MACRO_QUEST_DELETE,					//    ����Ʈ�ʱ�ȭ
		MACRO_QUEST_TEXT,					//	  ����Ʈ �ؽ�Ʈ ��ġ ���
		MACRO_EXIT_LADDER,					//    ������(������)
		MACRO_PRESENT_DELETE,				//    ������ Ŭ����
		MACRO_CHANNEL_INFO,					//    ä������(�Ʒü�&���忡���� �����ϸ� ���� ����Ʈ ���) /ä������ ������
		MACRO_CONTRIBUTE,					//    ������ ���� : �⿩�� �α�
		MACRO_MEMO,							//    ���� ������(�̻��)
		MACRO_INVITE_CHANNEL,				//    ä�� �ʴ�(�̻��)
		MACRO_ENTER_PLAZA,					//    �뺴���� ����(�̻��)
		MACRO_NONE_UI,						//    UI��¾���
		MACRO_USER_INFO,					//    ��������������
		MACRO_CHAT_MODE,					//	   �뺴���� ��ȭ���(�̻��)
		MACRO_INIT_UI_POS,					//    UI��ġ �ʱ�ȭ(�̻��)
		MACRO_AWARD,						//    ������ ���� : �û��
		MACRO_INFO,							//    ������ ���� : ����
		MACRO_ROOM_ENTER,					//    ������ ���� : �Ʒ�
		MACRO_UDP,							//    ������ ���� : UDP ��Ŷ 
		MACRO_MOVE_PACKET,					//    ������ ���� : �̵���Ŷ �α�(�̻��)
		MACRO_GUILD_INFO,					//    �������������
		MACRO_TIME,							//    �ð� �ȳ�
		MACRO_SHOW_ANNOUNCEMENT,			//    ����������(AnnouncementWnd) show
		MACRO_BLOCK_GULID_MARK,				//    ������ ���� : ��� ��ũ ����
		MACRO_MOVIE_MODE,					//    ��ȭ���
		MACRO_USER_RECORD,					//    ������ ���� : ���� �α�
		MACRO_TEAM_INFO,					//    ������ ���� : �� ����
		MACRO_CHANNEL_MOVE,					//    ä�� �̵�(���ο����� ����)	
		MACRO_CAMP_INFO,					//    ������ ���� : ��������
		MACRO_CAMP_USER,  					//    ������ ���� : ��ȸ����Ʈ
		MACRO_GUILDNAME_CHANGE,				//    ������ ���� : ���� ���� �׽�Ʈ
		MACRO_EVENT_TEST,					//    ������ ���� : �̺�Ʈ �׽�Ʈ
		MACRO_WHOLE_CHAT_OFF,				//	   ��ħ����
		MACRO_WHOLE_CHAT_ON,				//	   ��ħ�ѱ�
		MACRO_CAMERA_TEST,					//    ������ ���� : �� ī�޶� �̺�Ʈ �׽�Ʈ
		MACRO_FONT_TEXTURE,					//    ��Ʈ�ؽ���
		MACRO_SHOW_WND_ID,					//    Show ������ Ȯ��
		MACRO_CHAT_INFO_OFF,				//    ä�� �˸� �޼��� off
		MACRO_CHAT_INFO_ON,					//    ä�� �˸� �޼��� on
		MACRO_CONTROL_SYNC_EX_ON,			//    Control Packet�� �ε��� üũ�� �ٸ� ��Ŷ���� Ȯ��
		MACRO_CONTROL_SYNC_EX_OFF,			//    Control Packet�� �ε��� üũ�� Control Packet���� ����
		MACRO_EVENT_SHOP_GOODS_LIST,		//    ������ ���� : �̺�Ʈ ��ǰ ��� �����´�.	/10
		MACRO_EVENT_SHOP_GOODS_BUY,			//    ������ ���� : �̺�Ʈ ��ǰ ����.			/11
		MACRO_EVENT_SHOP_STATE,				//    ������ ���� : �̺�Ʈ ���� On / Off.		/ShopClose = 0(Open), 1(Close)
		MACRO_EVENT_SHOP_BUY_USER_CLEAR,	//    ������ ���� : �̺�Ʈ ���� ������ ��� ����/BuyListClear
		MACRO_CUSTOM_DDS_FROM_JPG,			//    ������ ���� : DDS ������ JPG�� ����
		MACRO_CUSTOM_JPG_MIX_DDS_FROM_DDS,  //    ������ ���� : JPG�� ���� DDS ���� ���ļ� DDS�� ����
		MACRO_CUSTOM_DDS_FROM_LSC,			//    ������ ���� : DDS ������ LSC�� ����
		MACRO_CUSTOM_LSC_FROM_DDS,			//    ������ ���� : LSC ������ DDS�� ����
		MACRO_CUSTOM_ENTITY_TEXTURE,        //    ������ ���� : �ش� ������ Ŀ���� ������ �ؽ��Ŀ� ��ĥ��.
		MACRO_SHOW_WND,						//    ������ ���� : ������ ��
		MACRO_PRESENT,                      //    �系 admin ���� : ���� ������
		MACRO_BEST_FRIEND_APPLICATION,      //    �系 admin ���� : ��ģ ��û
		MACRO_BEST_FRIEND_AGREE,            //    �系 admin ���� : ��ģ ����
		MACRO_BEST_FRIEND_REFUSE,           //    �系 admin ���� : ��ģ ����
		MACRO_BEST_FRIEND_CLEAR,            //    �系 admin ���� : ��ģ ����
		MACRO_EXPERIENCE_STATE,             //    �系 admin ���� : ü�����
		MACRO_MULTI_MAP_CHANGE,             //    �系 admin ���� : ��Ƽ�� �ε��� ���� 
		MACRO_MACRO_NOTIFY,                 //    ��ũ�� ���� �Ű�
		MACRO_EXTRAITEM_GROWTH_CATALYST,    //    ��� ���� ������ : /����� 
		MACRO_GROWTH_LEVELUP,				//    �系 admin ���� : Ÿ�����潺 ��� �� 150���� �� ��ũ��
		MACRO_MONSTER_CLOSE,				//    ���忡 ���� �������� �ʴ� ��ũ��  /MonsterClose = 0, 1    0�̸� �� ���´�. 1�̸� ������
		MACRO_BINGO_PRESENT,				//	  ����( sp2_bingo.ini )�� ������ ���� ��δ� �ޱ�.
		MACRO_MONSTER_SPAWN,				//    ���忡 ����ȯ�ϱ� /����ȯ ���, ��� 
		MACRO_SUPERGASHAPON_GET_ALL,		//	  ������ ���� : ������ EtcItemCode�� ������ ��Ű���� ���� ������
		MACRO_SUPERGASHAPON_GET,			//	  ������ ���� : ������ EtcItemCode�� Ư�� ������ ��Ű���� ������
		MACRO_SUPERGASHAPON_LIMIT_INFO_GET,	//	  ������ ���� : ������ EtcItemCode�� Ư�� ������ Limit ������ ������
		MACRO_SUPERGASHAPON_LIMIT_RESET,	//    ������ ���� : ������ EtcItemCode�� Ư�� ������ Limit ������ �ʱ�ȭ ��Ŵ
		MACRO_SEND_RECIVE_CHECK_ON,			//	  ������V ���� : ��Ŷ ����, �ޱ� üũ ����
		MACRO_SEND_RECIVE_CHECK_OFF,		//	  ������V ���� : ��Ŷ ����, �ޱ� üũ ����
		MACRO_ATTENDANCE_WND_SHOW,			//	  ������ ���� : �⼮üũ ������ Show
		MACRO_ATTENDANCE_ADD_DAY,			//	  ������ ���� : �⼮���� �߰�
		MACRO_ATTENDANCE_ADD_PREV_MONTH,	//	  ������ ���� : ������ �⼮������ �߰�
		MACRO_ATTENDANCE_RESET,				//	  ������ ���� : �⼮�� �ʱ�ȭ
		MACRO_ATTENDANCE_SET_DATE,			//	  ������ ���� : �⼮�� ��¥ ����
		MACRO_AWAKE_SET_DATE,				//	  ������ ���� : ����ð�
		MACRO_CHAR_POWER_UP_ADD,			//    �׽�Ʈ ���� : �뺴 ��ȭ �߰�
		MACRO_ITEM_SLOT_INDEX_FIND,			//	  �׽�Ʈ ���� : ������ ���� ������ ã��
		MACRO_ITEM_POWER_UP_ADD,			//    �׽�Ʈ ���� : ������ ��ȭ �߰�
		MACRO_CANNOT_OPEN_DDS_FILE,			//    �׽�Ʈ ���� : �Ұ����� DDS ���� Log�� ���
		MACRO_CHAR_INFO_WND_SHOW,			//    �׽�Ʈ ���� : ĳ���� ������ ���� �� ���� ���� ���
		MACRO_SET_MISSION,					//    �׽�Ʈ ���� : �Է� Ÿ���� ( ���� �̼� ��¥ + �Է� �� ) ���� �̼� ����
		MACRO_SET_DATE_MISSION,				//    �׽�Ʈ ���� : �Է� Ÿ���� ������¥ �̼� ����
		MACRO_SET_MISSION_VALUE,			//	  �׽�Ʈ ���� : �Է� ���� ��ŭ �̼� ����.
		MACRO_ROLLBOOK_WND_SHOW,			//	  �׽�Ʈ ���� : ���� �⼮�� ����
		MACRO_ROLLBOOK_PROGRESS,			//	  �׽�Ʈ ���� : /�����⼮ Ƚ��   �ش� Ƚ����ŭ �⼮�� ����˴ϴ�.( �����⼮ + �Է� Ƚ��(<8) )
		MACRO_GASHAPON_GET_ALL,				//	  ������ ����, ���ļ��� ���� : ������ EtcItemCode�� ��í���� ������ ��Ű���� ���� ������
		MACRO_FPS_LOWER,					//	  ������ ��� ��ũ��(�ҹ���)
		MACRO_FPS_UPPER,					//	  ������ ��� ��ũ��(�빮��)
		MACRO_TIMER_CHANGE,					//	  ������ �ð�API ����
		MACRO_FPS_LAG_CHECK,				//	  ������ üũ ��ũ�� (����)
		MACRO_ALL_LSC_FROM_DDS,				//    ������ ���� : LSC ������ DDS�� ����		
		MACRO_HOUSING_TEST,					//    �Ͽ�¡ �ý��� ���� ��ũ��
		MACRO_HOUSING_CMD,					//    �Ͽ�¡ �ý��� Ŀ�ǵ� ��ũ��
		MACRO_HOUSING_SAVE,					//    �Ͽ�¡ �� ��ġ ���� INI�� ����
		MACRO_COMFORTABLE_LIFE,				//    ���ϰ� ���� ������(���� ���߿�) ��ũ��
		MACRO_TOURNAMENT_NEXT_STATE,		//    ��ȸ�� ��ũ�� : ���� ���� ���� ����
		MACRO_TOURNAMENT_RESULT,			//    ��ȸ�� ��ũ�� : ���� ��ȸ�� ���� ������ ���� ȣ��
		MACRO_BATTLEROOM_JOIN,				//	  �Ϲ� ������ ��Ʋ�� ���� ���� ���
		MACRO_STEAM_ACHIVE,					//	  ������ ����, ���� �������� �׽�Ʈ��
		MACRO_STEAM_OPENWEB,				//	  ������ ����, ���� �������� ����
		MACRO_SHOW_LADDERROOM,				//	  ������ ����, ���� �� �����ֱ�
		MACRO_PENALTY_VIEW,					// ���Ƽ ���� ����
		MAX_MACRO,
	};

protected:
	static char m_szMacro[MAX_MACRO][MAX_PATH];

public:
	static void SetMacroText();
	static char *GetMacroText( int iMacroType );

public:
	int IsMacro(char *szChat,int &key_word);

public:
	ChatMacro();
	virtual ~ChatMacro();
};
