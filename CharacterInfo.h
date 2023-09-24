#pragma once

#include "EncryptValue.h"
#include "SP2Packet.h"


enum EntryType 
{
	ET_TEMPORARY      =  1,
	ET_FORMALITY      =  5,
	ET_TERMINATION    = -1, // ���Ӽ��������� �����ϴ� Ÿ��, DB���� �������� ����.
	ET_FORMALITY_CASH = 10, // ���İ��� �����μ� ĳ�� ������ ������ ����
};

enum
{
	USER_TYPE_NORMAL       = 0,    // �Ϲ� ����
	USER_TYPE_BROADCAST_AFRICA = 1,    // 2009.02.19 ������ī ��� �̺�Ʈ ���� ����
	USER_TYPE_LIMIT_IGNORE = 2,    // �뺴�� ġ���� �������� ���� �����ο� Ÿ��
	USER_TYPE_BROADCAST_MBC= 3,    // 2009.03.04 MBC ���� ��� �̺�Ʈ ���� ����
};

// DB�� ����Ǵ� ������ ���������� �������� �ʴ´�.
enum ChannelingType
{
	CNT_NONE		= -1,
	CNT_WEMADEBUY	= 0,
	CNT_STEAM      =    1,
	CNT_MGAME		= 300,
	CNT_DAUM		= 400,
	CNT_NAVER		= 600,
	CNT_TOONILAND	= 700,
	CNT_NEXON		= 800,
	CNT_HANGAME		= 900,
	CNT_MAX,
};

// DB�� ����Ǵ� ������ ���������� �������� �ʴ´�.
enum BlockType
{
	BKT_NONE        = -1,
	BKT_NORMAL      =  0,   // ���ܾȵ� (����)
	BKT_WARNNING    = 10,   // ��� ( �� )
	BKT_LIMITATION  = 20,   // ���� ( �� )
	BKT_BLOCK       = 100,  // ���� ( �� )
};

enum
{
	HERO_SEASON_RANK_1 = 0,    //������ ���� ��ŷ �ֱ� ù ����
	HERO_SEASON_RANK_2,		   //������ ���� ��ŷ �ֱ� �ι�° ����
	HERO_SEASON_RANK_3,		   //������ ���� ��ŷ �ֱ� ����° ����
	HERO_SEASON_RANK_4,		   //������ ���� ��ŷ �ֱ� �׹�° ����
	HERO_SEASON_RANK_5,		   //������ ���� ��ŷ �ֱ� �ټ���° ����
	HERO_SEASON_RANK_6,		   //������ ���� ��ŷ �ֱ� ������° ����
	HERO_SEASON_RANK_MAX,
};

struct RecordData
{
	int m_iWin;
	int m_iLose;
	int m_iKill;
	int m_iDeath;
	RecordData()
	{
		Init();
	}
	void Init()
	{
		m_iWin = m_iLose = 0;
		m_iKill = m_iDeath = 0;
	}
};

struct USERDATA           //2007.05.29 LJH
{
	DWORD			m_user_idx;				//���� ����Ű
	ioHashString	m_private_id;		    //���� �г���
	ioHashString    m_public_id;            //�г���
	int		m_cash;							//���� ĳ��
	__int64	m_money;						//���� �Ӵ�  EX.������ �ڵ� ����.
	int     m_connect_count;                //���� Ƚ��.
	DWORD   m_connect_date;                 //���� ������
	int     m_user_state;                   //�Ʒ� ����
	int     m_grade_level;                  //���
	int     m_grade_exp;                    //��� ���� ����ġ
	int		m_fishing_level;				//���� ����
	int		m_fishing_exp;					//���� ���� ����ġ
	int     m_user_event_type;              //Ư���� ���� �̺�Ʈ Ÿ��
	int     m_user_rank;                    //��ü ��ŷ
	EntryType m_eEntryType;					//�ӽð���, ���İ���(�Ǹ�Ȯ��), �ӽð��� ���� ( UserMemberDB.joinType , -1���� DB�� �������� ����-���Ӽ���������-)
	int     m_camp_position;                //�Ҽ� ���� : (0:���Ҽ�)(1:���α�)(2:�ݶ���)
	int     m_ladder_point;                 //��ȸ ����Ʈ
	int     m_camp_rank;                    //���� ��ŷ
	ChannelingType m_eChannelingType;       //ä�θ� : �Ϲ�, mgame, ��
	ioHashString   m_szChannelingUserNo;    //ä�θ����� �����ĺ�no : naver���� ���
	ioHashString   m_szChannelingUserID;    //ä�θ����� ID
	int            m_iChannelingCash;       //ä�θ��翡 �����ϴ� Cash�� �Ӵ�
	BlockType      m_eBlockType;            //����Ÿ��
	DWORD          m_dwBlockYearMonthDay;   //���ܳ�¥
	DWORD          m_dwBlockHourMinute;     //���ܽð�
	int            m_refill_data;           //���ʹޱ���� ���� ��
	int            m_purchased_cash;        //������ ������ �ְ� ������ ĳ�� m_cash�� �Ϻ� �ݾ�
	int            m_iExcavationLevel;      //�߱�����
	int            m_iExcavationExp;        //�߱� ���� ����ġ
	int            m_iAccrueHeroExpert;     //������ ���� ����ġ
	int		       m_iHeroExpert;           //������ ����ġ
	int			   m_iBonusCash;			//���ʽ� ĳ��

	USERDATA()
	{
		m_user_idx      = 0;
		m_private_id.Clear();
		m_public_id.Clear();
		m_cash			= 0;
		m_money			= 0;
		m_connect_count	= 0;
		m_connect_date	= 0;
		m_user_state    = 0;
		m_grade_level   = 0;
		m_grade_exp		= 0;      
		m_fishing_level	  = 0;
		m_fishing_exp	  = 0;
		m_user_event_type = USER_TYPE_NORMAL;
		m_user_rank     = 0;
		m_eEntryType    = ET_TEMPORARY;
		m_camp_position = 0;
		m_ladder_point  = 0;
		m_camp_rank     = 0;
		m_eChannelingType   = CNT_NONE;
		m_szChannelingUserID.Clear();
		m_szChannelingUserNo.Clear();
		m_iChannelingCash   = 0;
		m_eBlockType        = BKT_NONE;
		m_dwBlockYearMonthDay = 0;
		m_dwBlockHourMinute   = 0;
		m_refill_data = 0;
		m_purchased_cash  = 0;
		m_iExcavationLevel= 0;
		m_iExcavationExp  = 0;
		m_iAccrueHeroExpert = 0;
		m_iHeroExpert     = 0;
		m_iBonusCash = 0;
	}
};

struct UserRelativeGradeData
{
	int   m_init_code;     //����� ��� ����ġ ���� �ڵ�
	bool  m_enable_reward; //����� ��� ������ �ִ°�?

	UserRelativeGradeData()
	{
		m_init_code = 0;
		m_enable_reward = 0;
	}
};

struct UserHeroData
{
	int            m_iHeroTitle;            //������ Īȣ
	int            m_iHeroTodayRank;        //������ ���� ��ŷ
	int            m_iHeroSeasonRank[HERO_SEASON_RANK_MAX];       //������ ���� ��ŷ

	UserHeroData()
	{
		Initialize();
	}

	void Initialize()
	{
		m_iHeroTitle      = 0;
		m_iHeroTodayRank  = 0;
		for (int i = 0; i < HERO_SEASON_RANK_MAX ; i++)
			m_iHeroSeasonRank[i] = 0;
	}
};

enum
{
	MAX_DISPLAY_CNT = 10,
};
struct UserHeadquartersOption
{
	// ���� �ɼ�
	short m_sLock;

	// ���� �ɼ�
	DWORD m_dwCharacterIndex[MAX_DISPLAY_CNT];
	int   m_iCharacterXPos[MAX_DISPLAY_CNT];
	int   m_iCharacterZPos[MAX_DISPLAY_CNT];

	UserHeadquartersOption()
	{
		Initialize();		
	}

	void Initialize()
	{
		m_sLock = 0;

		for(int i = 0;i < MAX_DISPLAY_CNT;i++)
		{
			m_dwCharacterIndex[i] = 0;
			m_iCharacterXPos[i] = m_iCharacterZPos[i] = 0;
		}
	}

	void ApplyData( SP2Packet &rkPacket )
	{
		rkPacket >> m_sLock;
		for (int i = 0; i < MAX_DISPLAY_CNT ; i++)
		{
			rkPacket >> m_dwCharacterIndex[i] >> m_iCharacterXPos[i] >> m_iCharacterZPos[i]; 
		}
	}
};

struct ITEM_DATA          //2007.05.29 LJH
{ 
	int		m_item_code;             //������ ����
	int  	m_item_reinforce;        //��ȭ
	int		m_item_bullet;			 //Bullet
	DWORD   m_item_male_custom;      //���� Ŀ����
	DWORD   m_item_female_custom;    //���� Ŀ����

	ITEM_DATA()
	{
		Initialize();
	}

	void Initialize()
	{
		m_item_code		 = 0;
		m_item_reinforce = 0;
		m_item_bullet	 = -1;
		m_item_male_custom = m_item_female_custom = 0;
	}	

	ITEM_DATA& operator = ( const ITEM_DATA &rhs )
	{
		m_item_code			= rhs.m_item_code;
		m_item_reinforce	= rhs.m_item_reinforce;
		m_item_bullet		= rhs.m_item_bullet;
		m_item_male_custom	= rhs.m_item_male_custom;
		m_item_female_custom= rhs.m_item_female_custom;
		return *this;
	}
};

struct ITEMSLOT          //2007.08.25 LJH 
{
	int  	m_item_type;             //���� Ư�� 1111 22 333 ( ��Ʈ, ����(����), ġ��Ÿ�� )
	int		m_item_code;	         //������ ġ�� Ÿ��      ( ġ�� ���� ��ȣ )
	int     m_item_equip;    //������ ���� ����      ( 0:������, 1:���� ) (UserItemDB: itemX_code  �� ���ڸ�)     

	ITEMSLOT()
	{
		Initialize();
	}

	void Initialize()
	{
		m_item_type		= 0;
		m_item_code     = 0;
		m_item_equip    = 0;
	}
};
typedef std::vector< ITEMSLOT > ITEMSLOTVec;

// �Ⱓ�� & ����
enum  CharPeriodType
{
	CPT_TIME     = 0,
	CPT_MORTMAIN = 1,
};

// ��ǥ �뺴 ����
enum
{
	CLT_GENERAL= 0,
	CLT_LEADER = 1,
};

// �뿩 �뺴
enum
{
	CRT_GENERAL= 0,
	CRT_RENTAL = 1,
};

//���� ����
enum
{
	AwakeTypeCount = 2
};

enum AwakeType
{
	AWAKE_NONE		= 0,
	AWAKE_NORMAL	= 1,
	AWAKE_RARE		= 2,
	AWAKE_MAX		= 3,
};

struct CHARACTER                        //2007.05.29 LJH 
{
	// ���� �߰��ÿ� FillData(), ApplayData(), operator=, operator==, Init() �Լ��� �����ؾ���. 
	// ��ȣ���� �߰��� CHARACTER����ü�� memset, memcpy, memcmp ��� ����

	CEncrypt<int>	m_class_type;       //Ŭ���� Ÿ�� : ��Ʈ ������ Ÿ��
	CEncrypt<int>	m_kindred;          //���� 1(�޸�)2(����)3(�����)
	CEncrypt<int>	m_sex;              //���� 1(����)2(����)
	CEncrypt<int>	m_beard;            //���� 1(���������) 2,3,4(���� ����)
	CEncrypt<int>	m_face;             //�� 1(�����������⺻)
	CEncrypt<int>	m_hair;		        //�Ӹ� 1(�����������⺻)
	CEncrypt<int>	m_skin_color;       //�Ǻλ� 1,2,3,4
	CEncrypt<int>	m_hair_color;       //�Ӹ��� 1,2,3
	CEncrypt<int>	m_accessories;      //��ű� 1
	CEncrypt<int>   m_underwear;        //�ӿ�   1,2,.....
	
	CEncrypt<int>   m_extra_item[MAX_INVENTORY];       // ������ ������ �߰� ���

	int m_iSlotIndex;       //�뺴 ���Կ����� ��ġ
	int m_iLimitSecond;     //���� �Ⱓ

	short m_sLeaderType;          // ��ǥ �뺴
	short m_sRentalType;          // �뿩 ���� 
	DWORD m_dwRentalMinute;       // �뿩�� �ð�
	CharPeriodType m_ePeriodType; // �����ѿ뺴, �Ⱓ���뺴

	bool m_bActive;         //��� ������ �뺴.
	byte m_chExerciseStyle; //ü�� ��Ÿ�� 

	BYTE m_iAwakeType;		//���� ����
	int m_iAwakeLimitTime;	//���� ���� �ð�
	
	BYTE m_eCharPowerUpType;
	
	CostumeEquipInfo m_EquipCostume[MAX_INVENTORY];
	AccessoryEquipInfo m_EquipAccessory[MAX_ACC_INVENTORY];

	CHARACTER()
	{
		Init();
	}

	void Init()
	{
		m_class_type	    = 0;
		m_kindred			= 1;
		m_sex				= 1;
		m_beard				= 1;
		m_face				= 1;
		m_hair				= 1;
		m_skin_color		= 1;
		m_hair_color		= 1;
		m_accessories		= 1;
		m_underwear			= 1;

		for(int i = 0; i < MAX_INVENTORY ; i++ )
			m_extra_item[i]	= 0;

		m_iSlotIndex		= -1;
		m_iLimitSecond		= 0;

		m_sLeaderType		= CLT_GENERAL;
		m_sRentalType		= CRT_GENERAL;
		m_dwRentalMinute	= 0;
		m_ePeriodType		= CPT_TIME;

		m_bActive			= true;
		m_chExerciseStyle	= 0x00;

		m_iAwakeType		= AWAKE_NONE;
		m_iAwakeLimitTime	= 0;

		m_eCharPowerUpType = PUGT_NONE;

		for(int i = 0; i < MAX_INVENTORY ; i++ )
			m_EquipCostume[i].Init();
		for(int i = 0; i < MAX_ACC_INVENTORY ; i++ )
			m_EquipAccessory[i].Init();
	}

	CHARACTER& operator = ( const CHARACTER &rhs )
	{
		m_class_type		= rhs.m_class_type;
		m_kindred			= rhs.m_kindred;
		m_sex				= rhs.m_sex;
		m_beard				= rhs.m_beard;
		m_face				= rhs.m_face;
		m_hair				= rhs.m_hair;
		m_skin_color		= rhs.m_skin_color;
		m_hair_color		= rhs.m_hair_color;
		m_accessories		= rhs.m_accessories;
		m_underwear			= rhs.m_underwear;

		for( int i = 0; i < MAX_INVENTORY ; i++ )
			m_extra_item[i]	= rhs.m_extra_item[i];

		m_iSlotIndex		= rhs.m_iSlotIndex;
		m_iLimitSecond		= rhs.m_iLimitSecond;

		m_sLeaderType		= rhs.m_sLeaderType;
		m_sRentalType		= rhs.m_sRentalType;
		m_dwRentalMinute	= rhs.m_dwRentalMinute;
		m_ePeriodType		= rhs.m_ePeriodType;

		m_bActive			= rhs.m_bActive;
		m_chExerciseStyle	= rhs.m_chExerciseStyle;

		m_iAwakeType		= rhs.m_iAwakeType;
		m_iAwakeLimitTime	= rhs.m_iAwakeLimitTime;

		m_eCharPowerUpType = rhs.m_eCharPowerUpType;

		for(int i = 0; i < MAX_INVENTORY ; i++ )
			m_EquipCostume[i] = rhs.m_EquipCostume[i];
		for(int i = 0; i < MAX_ACC_INVENTORY ; i++ )
			m_EquipAccessory[i] = rhs.m_EquipAccessory[i];

		return *this;
	}

	bool operator==( const CHARACTER &rhs ) const
	{
		if( m_class_type != rhs.m_class_type )
			return false;
		if( m_kindred != rhs.m_kindred )
			return false;
		if( m_sex != rhs.m_sex )
			return false;
		if( m_beard	!= rhs.m_beard )
			return false;
		if( m_face != rhs.m_face )
			return false;
		if( m_hair != rhs.m_hair )
			return false;
		if( m_skin_color != rhs.m_skin_color )
			return false;
		if( m_hair_color != rhs.m_hair_color )
			return false;
		if( m_accessories != rhs.m_accessories )
			return false;
		if( m_underwear != rhs.m_underwear )
			return false;

		for (int i = 0; i < MAX_INVENTORY ; i++)
		{
			if( m_extra_item[i] != rhs.m_extra_item[i] )
				return false;
		}

		if( m_iSlotIndex != rhs.m_iSlotIndex )
			return false;
		if( m_iLimitSecond != rhs.m_iLimitSecond )
			return false;

		if( m_sLeaderType != rhs.m_sLeaderType )
			return false;
		if( m_sRentalType != rhs.m_sRentalType )
			return false;
		if( m_dwRentalMinute != rhs.m_dwRentalMinute )
			return false;
		if( m_ePeriodType != rhs.m_ePeriodType )
			return false;

		if( m_bActive != rhs.m_bActive )
			return false;
		if( m_chExerciseStyle != rhs.m_chExerciseStyle )
			return false;
		if( m_iAwakeType != rhs.m_iAwakeType )
			return false;
		if( m_iAwakeLimitTime != rhs.m_iAwakeLimitTime )
			return false;
		
		if( m_eCharPowerUpType != rhs.m_eCharPowerUpType )
			return false;

		for (int i=0; i<MAX_INVENTORY; i++)
		{
			if( m_EquipCostume[i] != rhs.m_EquipCostume[i] )
				return false;
		}

		for (int i=0; i<MAX_ACC_INVENTORY; i++)
		{
			if( m_EquipAccessory[i] != rhs.m_EquipAccessory[i] )
				return false;
		}

		return true;
	}

	bool operator!=( const CHARACTER &rhs ) const
	{
		if( *this == rhs )
			return false;

		return true;
	}

	void FillData( SP2Packet &rkPacket )
	{
		rkPacket << m_class_type;
		rkPacket << m_kindred;
		rkPacket << m_sex;			
		rkPacket << m_beard;		
		rkPacket << m_face;			
		rkPacket << m_hair;			
		rkPacket << m_skin_color;	
		rkPacket << m_hair_color;	
		rkPacket << m_accessories;	
		rkPacket << m_underwear;    

		for (int i = 0; i < MAX_INVENTORY ; i++)
			rkPacket << m_extra_item[i]; 

		rkPacket << m_iSlotIndex;    
		rkPacket << m_iLimitSecond;  

		rkPacket << m_sLeaderType;
		rkPacket << m_sRentalType;
		rkPacket << m_dwRentalMinute;
		rkPacket << (int)m_ePeriodType;   

		rkPacket << m_bActive;
		rkPacket << m_chExerciseStyle;

		rkPacket << static_cast<byte>( m_iAwakeType );
		rkPacket << m_iAwakeLimitTime;

		rkPacket << m_eCharPowerUpType;
		
		for( int i=0; i<MAX_INVENTORY; i++ )
		{
			rkPacket << m_EquipCostume[i].m_CostumeIdx;
			rkPacket << m_EquipCostume[i].m_CostumeCode;
#ifdef CUSTOM_COSTUME
			rkPacket << m_EquipCostume[i].m_Costume_Male_Custom;
			rkPacket << m_EquipCostume[i].m_Costume_Female_Custom;
#endif
		}

		for( int i=0; i<MAX_ACC_INVENTORY; i++ )
		{
			rkPacket << m_EquipAccessory[i].m_iAccessoryIdx;
			rkPacket << m_EquipAccessory[i].m_iAccessoryCode;
			rkPacket << m_EquipAccessory[i].m_iRandomNumber;
		}
	}

	void ApplyData( SP2Packet &rkPacket )
	{
		rkPacket >> m_class_type;
		rkPacket >> m_kindred;
		rkPacket >> m_sex;			
		rkPacket >> m_beard;		
		rkPacket >> m_face;			
		rkPacket >> m_hair;			
		rkPacket >> m_skin_color;	
		rkPacket >> m_hair_color;	
		rkPacket >> m_accessories;	
		rkPacket >> m_underwear;    

		for (int i = 0; i < MAX_INVENTORY ; i++)
			rkPacket >> m_extra_item[i]; 

		rkPacket >> m_iSlotIndex;    
		rkPacket >> m_iLimitSecond;  

		rkPacket >> m_sLeaderType;
		rkPacket >> m_sRentalType;
		rkPacket >> m_dwRentalMinute;
		int iPeriodType = 0;
		rkPacket >> iPeriodType; 
		m_ePeriodType = (CharPeriodType) iPeriodType;   

		rkPacket >> m_bActive;       
		rkPacket >> m_chExerciseStyle;

		byte eAwakeType;
		rkPacket >> eAwakeType;
		m_iAwakeType = eAwakeType;
		rkPacket >> m_iAwakeLimitTime;

		rkPacket >> m_eCharPowerUpType;
		
		for( int i=0; i<MAX_INVENTORY; i++ )
		{
			rkPacket >> m_EquipCostume[i].m_CostumeIdx;
			rkPacket >> m_EquipCostume[i].m_CostumeCode;
#ifdef CUSTOM_COSTUME
			rkPacket >> m_EquipCostume[i].m_Costume_Male_Custom;
			rkPacket >> m_EquipCostume[i].m_Costume_Female_Custom;
#endif
		}

		for( int i=0; i<MAX_ACC_INVENTORY; i++ )
		{
			rkPacket >> m_EquipAccessory[i].m_iAccessoryIdx;
			rkPacket >> m_EquipAccessory[i].m_iAccessoryCode;
			rkPacket >> m_EquipAccessory[i].m_iRandomNumber;
		}
	}
};

struct CHARACTERDATA         //2007.05.29 LJH 
{
	DWORD           m_dwIndex;
	CHARACTER       m_data;
	ITEM_DATA       m_equip_item[MAX_INVENTORY];       //���� ������.

	CHARACTERDATA()
	{
		m_dwIndex = 0;

		for (int i = 0; i < MAX_INVENTORY ; i++)
			m_equip_item[i].Initialize();
	}
};

struct ITEM_GAUGE_DATA
{
	float m_fMaxGauge;
	float m_fCurGauge;
	float m_fRecoveryGauge;
	DWORD m_dwRecoveryTic;
	DWORD m_dwCheckTime;

	bool m_bRecoverEnable;

	ITEM_GAUGE_DATA()
	{
		m_fMaxGauge = 0.0f;
		m_fCurGauge = 0.0f;
		m_fRecoveryGauge = 0.0f;
		m_dwRecoveryTic = 0;
		m_dwCheckTime = 0;

		m_bRecoverEnable = true;
	}
};

struct PlayCharInfo
{
	float m_fRevivalPenalty;
	bool m_bCharDied;
	bool m_bCharDiedPenalty;
	bool m_bCharJoined;

	ITEM_GAUGE_DATA m_EquipItem[MAX_INVENTORY];

	PlayCharInfo()
	{
		m_fRevivalPenalty = 0.0f;
		m_bCharDied = false;
		m_bCharDiedPenalty = false;
		m_bCharJoined = false;
	}
};

typedef std::vector< PlayCharInfo* > PlayCharInfoList;

struct CustomTexture
{
	DWORD m_dwCustomSeq;
	ioHashString m_szTextureName;
	CustomTexture()
	{
		m_dwCustomSeq = 0;
	}
};
typedef std::vector< CustomTexture > CustomTextureList;
