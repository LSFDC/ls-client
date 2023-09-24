#pragma once


//----------------- BaseCahr

//UJ 131016, ��Ŷ����ȭ: save 3 bytes (int -> byte)
//enum CharState 
enum CharState: BYTE
{
	CS_DELAY,
	CS_RUN,
	CS_DASH,
	CS_EXTEND_DASH,
	CS_JUMP,
	CS_ATTACK,
	CS_DASH_ATTACK,
	CS_USING_SKILL,
	CS_DEFENSE,
	CS_EXTEND_DEFENSE,
	CS_WOUNDED_DEFENSE,					// 10
	CS_WOUNDED,							// ���ĵ� ��Ʈ
	CS_BLOW_WOUNDED,					// ������ ����
	CS_PICK_ITEM,
	CS_DIE,
	CS_ETC_STATE,
	CS_DROP_ZONE_DOWN,
	CS_COUNTER_ATTACK,
	CS_FALL,
	CS_ENDMOTION,
	CS_FLY,								// 20
	CS_STRUCT_PUSH,
	CS_ESCAPE_PRISONER,
	CS_FROZEN,
	CS_READY,
	CS_DRINK,
	CS_VIEW,
	CS_DASH_FAIL,
	CS_CHANGE_WAIT,
	CS_ESCAPE_CATCH,
	CS_CREEPING,						// 30
	CS_PROTECTION,
	CS_WARP_STATE,
	CS_CLOSEORDER,
	CS_STARTMOTION,
	CS_CREATEMOTION,
	CS_LOADING,
	CS_FLOAT_STATE,
	CS_STOP_MOTION,
	CS_TELEPORT,
	CS_MISS_FIRE,						// 40
	CS_SPECIAL_MOTION,
	CS_OBSERVER,
	CS_FISHING,
	CS_EXTRAITEM_CHANGE,
	CS_DUMMY_STATE,						// �ƹ��͵� ���� �ʰ� �ð��� ������ ���� ����
	CS_COIN_REBIRTH,					// ��� ���� ���� ���� ��Ȱ ���
	CS_EXCAVATING,
	CS_DODGE_DEFENSE,
	CS_BOUND_BLOW_WOUNDED,
	CS_CHANGE_GANGSI,					// 50 ���÷� ��ȯ ����
	CS_CURSE_ROLLING,					// BT_CURSE_BUFF �ɷ��� ������ ����
	CS_DOWN_ROLLING,
	CS_GRADEUP_MOTION,
	CS_MACHINE_TAKE_WAIT,
	CS_MACHINE_TAKE,
	CS_BUFF_AIR_ATTACK,
	CS_ETCITEM_MOTION,					// Ư�� ������ ���ī�� ���
	CS_NOT_MOVE_PROTECT,
	CS_ICE_STATE,
	CS_NEW_JUMP_ATTACK,					// 60 ������ ���� ����
	CS_RETREAT_STATE,
	CS_DEFENSE_ATTACK,
	CS_CREATE_STRUCT,
	CS_GRAPPLING,
	CS_GRAPPLING_WOUNDED,
	CS_GRAPPLING_PUSHED,
	CS_GRAPPLING_WOUNDED_SKILL,
	CS_GRAPPLING_WOUNDED_SWING,
	CS_LAND_AFTER_STATE,
	CS_BLOW_EXTEND_WOUND,				// 70
	CS_BLOW_DASH_ATTACK,
	CS_SKIP_STUN,
	CS_JUMP_GRAPPLING,
	CS_JUMP_GRAPPLING_WOUNDED,
	CS_UROBORUS,
	CS_WEREWOLF,
	CS_CHARGE_FLY,
	CS_WITCH_FLY,
	CS_PANIC_MOVE,
	CS_MECHANICS_FLY,					// 80
	CS_ADHESIVE_BUFF,
	CS_MAGIC_STUN,
	CS_CMD_STATE,
	CS_PHANTOM_FLY,
	CS_POSSESSION_STATE,
	CS_LUCIFER_FLY,
	CS_GRAPPLING_WOUNDED_ITEM,
	CS_GRAPPLING_PUSHED_BUFF,
	CS_NAKORURU_JUMP_ATTACK,
	CS_DESTROYER_FLY,					// 90
	CS_MICHAEL_FLY,
	CS_ATTACH_WEAPON,
	CS_ROULETTE,
	CS_GOOMIHO_LAND_FLY,
	CS_PETERPAN_FLY,
	CS_LOCKUP_BUFF,						//����
	CS_SUCCUBUS_ATTACK,
	CS_CYVOGUE_STATE,
	CS_TRINITY,
	CS_TRINITY_SWALLOW,					// 100
	CS_DEFENSE_CANCEL,
	CS_WIND_MAGNETIC,
	CS_GHOST_STEALER_STATE,
	CS_DRACULA_FLY,
	CS_HAKUMEN_DEFENSE,
	CS_DEVIL_FLY,
	CS_FLASH_SPECIAL,
	CS_EVY_BLINK,
	CS_EVY_BOMB_STATE,
	CS_PSYCHO_SPECIAL,					// 110
	CS_OBJECT_RUNNING_ATTACK,
	CS_BUFF_ACTION,
	CS_SLASHER_SPECIAL,
	CS_TITAN_EXTEND_MOVE,
	CS_SUPER_DIMENSION_FIRE,
	CS_SUPER_DIMENSION_DASH,
	CS_OBJECT_ATTACH,
	CS_NANGIN_SPECIAL,
	CS_HELICOPTER,
	CS_HELICOPTER_ATTACH,				// 120
	CS_STRIKER_MOVE,
	CS_STRIKER_LAND_ATTACK,
	CS_DRAGONNEST_ASS_SPECIAL,
	CS_THUNDER_BIRD_OBJECT_SPECIAL,
	CS_THUNDER_BIRD_HOLD,
	CS_THUNDER_BIRD_BOMB_STATE,
	CS_OBJECT_EQUIP,
	CS_DEFENCE_COUNTER,
	CS_VALKYRIE_FLY,
	CS_EARTHQUAKE_SPECIAL,				// 130
	CS_SHADOW_RANGER_SPECIAL,
	CS_VEHICLE_DRIVER,
	CS_PASSENGER,
	CS_NINJA_EXPANSION_MOVE,
	CS_DARKKNIGHT_SPECIAL,
	CS_JUMPER_SPECIAL,
	CS_OBJECT_RUNNING_COMBO_ATTACK,
	CS_FLY_BUFF_EXTRA_ATTACK,
	CS_GRAPPLING_WOUNDED_BUFF,
	CS_SABER_ATTACK,					// 140
	CS_SABER_FLY,
	CS_SABER_DASH_ATTACK,
	CS_HADES_ATTACK,
	CS_NAKORURU_FLY,
	CS_MAGIC_SWORD_ATTACK,
	CS_NATA_COMBO,
	CS_NATA_FLY,
	CS_RESEARCHER_REAR_FLY,
	CS_BLADE_FLY,
	CS_GRAPPLER_JUMP_GRAP,				// 150
	CS_GRAPPLING_EXPANSION,
	CS_JEONWOOCHI_EXTEND_MOVE,
	CS_JEONWOOCHI_EXTEND_DASH,
	CS_SAWANG_STATE,
	CS_CONSTRUCT_HOUSE,
	CS_RED_HOOD_MOVE,
	CS_STRIDER_SPECIAL,
	CS_NEPHILIM_PUSH_STATE,
	CS_NEPHILIM_BOMB_STATE,
	CS_NEPHILIM_TELEPORT_STATE,			// 160
	CS_BLOCK_INTEPLAY,
	CS_APOSTATE_MONK_SPECIAL,
	CS_MUTANT_SPECIAL_STATE,
	CS_MIR_WARRIOR_DASH_STATE,
	CS_CUCHULAIN_SPECIAL_STATE,
	CS_BUNKER_STATE,
	CS_GALFORD_CMD_STATE,
	CS_ONIMUSHA_RUN_EXTRA_STATE,
	CS_DRUID_CROUCH_GATHERING_STATE,
	CS_DRUID_EAGLE_FLY,
	CS_SASINMU_SPECIAL,
	CS_DARKSEAS_STATE,
	CS_FLY_FOLLOW_STATE, //������ ������ ������ ���󰣴�.(������ �⺻�� ����(���ݴ�������) ���������� �Ѵ�.)
	CS_PREDATOR_SPECIAL,
	CS_CROW_SPECIAL_STATE,
	CS_MEPHISTO_STATE,
};

enum JumpState
{
	JS_NONE,
	JS_READY,
	JS_JUMPPING,
	JS_AIM_JUMP,
	JS_JUMP_ATTACK,
	JS_JUMP_DEFENSE,
	JS_BREAK_FALL,
	JS_AUTO_SHOT,
	JS_LANDING,
	JS_JUMP_RELOAD,
	JS_AIM_JUMP2,
	JS_COMBO_DASH,
	JS_EXTRA_JUMP,
};

enum BlowWoundedState
{
	BW_NONE,
	BW_BLOWING,
	BW_BLOW_ENDED,
	BW_GETUP,
	BW_BOUND_BLOWING,
};

enum DieState
{
	DS_NONE,
	DS_FLYING,
	DS_BLOW_END,
	DS_EXP_UP,
	DS_GHOST_UP,
	DS_FADE_OUT,
};

enum FallState
{
	FS_NONE,
	FS_FALLING,
	FS_LANDING,
};

enum NonePlayState
{
	NPS_NONE,
	NPS_CARE,
	NPS_WARNING,

};

enum ChatModeState
{
	CMS_NONE,			// �Ϲ�
	CMS_CHANGE_CHAT,	// �Ϲ� -> ��ȭ
	CMS_CHANGE_NORMAL,	// ��ȭ -> �Ϲ�
	CMS_CHAT,			// ��ȭ
};

enum ExperienceMode
{
	EMS_NONE,
	EMS_CHANGE_EXPERIENCE,	// �Ϲ� -> ü��
	EMS_EXPERIENCE,			// ü�� ����
	EMS_SEND_EXPERIENCE,	// ü�� ���·� ��ȯ�ϰڴٰ� ������ ����
};

enum FishingState
{
	FSS_NONE,
	FSS_READY,
	FSS_FISHING,
	FSS_FISHING_WAIT,
	FSS_HOOK,
	FSS_RESULT,
};

enum FishingTackleChange
{
	FTC_NONE,
	FTC_ROD,
	FTC_BAIT,
	FTC_ALL,
};

enum RouletteState
{
	RS_NONE,
	RS_JUMPING,
	RS_LANDING,
};

enum SkillUseType
{
	SUT_NORMAL,
	SUT_JUMP,
	SUT_DOWN,
};

enum RoundEndMotion
{
	RM_NONE,
	RM_DRAW,
	RM_WIN,
	RM_LOSE,
};

enum ControlType
{
	CONTROL_USER = 1,
	CONTROL_AI   = 2,
};

enum CharCounterAttackKeyState
{
	CAKS_ATTACK_KEY,
	CAKS_JUMP_KEY,
};
//----------------------

enum AntiAliasingQuality
{
	AQ_NONE,
	AQ_LOW,
	AQ_MIDDLE,
	AQ_HIGH,
};

enum LightQuality
{
	LQ_LOW,
	LQ_MIDDLE,
	LQ_HIGH,
};

enum EffectQuality
{
	EQ_LOW,
	EQ_MIDDLE,
	EQ_HIGH,
};

enum MapObjectQuality
{
	MQ_LOW,
	MQ_MIDDLE,
	MQ_HIGH,
};

enum FilterType
{
	FILTER_NONE,
	FILTER_GRAY40,
	FILTER_GRAY80,
	FILTER_GRAY100,
	FILTER_SEPIA40,
	FILTER_SEPIA80,
	FILTER_SEPIA100,
	FILTER_MAX
};

enum OverayType
{
	OVERAY_NONE,
	OVERAY_BLACK,
	OVERAY_WHITE,
	OVERAY_MAX
};

enum TeamType
{
	TEAM_NONE,
	TEAM_RED,
	TEAM_BLUE,
	TEAM_PRIVATE_1,
	TEAM_PRIVATE_2,
	TEAM_PRIVATE_3,
	TEAM_PRIVATE_4,
	TEAM_PRIVATE_5,
	TEAM_PRIVATE_6,
	TEAM_PRIVATE_7,
	TEAM_PRIVATE_8,
	TEAM_PRIVATE_9,
	TEAM_PRIVATE_10,
	TEAM_PRIVATE_11,
	TEAM_PRIVATE_12,
	TEAM_PRIVATE_13,
	TEAM_PRIVATE_14,
	TEAM_PRIVATE_15,
	TEAM_PRIVATE_16,
	TEAM_PRIVATE_17,
	TEAM_PRIVATE_18,
	TEAM_PRIVATE_19,
	TEAM_PRIVATE_20,
	TEAM_PRIVATE_21,
	TEAM_PRIVATE_22,
	TEAM_PRIVATE_23,
	TEAM_PRIVATE_24,
	TEAM_PRIVATE_25,
	TEAM_PRIVATE_26,
	TEAM_PRIVATE_27,
	TEAM_PRIVATE_28,
	TEAM_PRIVATE_29,
	TEAM_PRIVATE_30,
	TEAM_PRIVATE_31,
	TEAM_PRIVATE_32,
	TEAM_PRIVATE_33,
	TEAM_PRIVATE_34,
	TEAM_PRIVATE_35,
	TEAM_PRIVATE_36,
	TEAM_PRIVATE_37,
	TEAM_PRIVATE_38,
	TEAM_PRIVATE_39,
	TEAM_PRIVATE_40,
	TEAM_PRIVATE_41,
	TEAM_PRIVATE_42,
	TEAM_PRIVATE_43,
	TEAM_PRIVATE_44,
	TEAM_PRIVATE_45,
	TEAM_PRIVATE_46,
	TEAM_PRIVATE_47,
	TEAM_PRIVATE_48,
	TEAM_PRIVATE_49,
	TEAM_PRIVATE_50,
	TEAM_PRIVATE_51,
	TEAM_PRIVATE_52,
	TEAM_PRIVATE_53,
	TEAM_PRIVATE_54,
	TEAM_PRIVATE_55,
	TEAM_PRIVATE_56,
	TEAM_PRIVATE_57,
	TEAM_PRIVATE_58,
	TEAM_PRIVATE_59,
	TEAM_PRIVATE_60,
	TEAM_PRIVATE_61,
	TEAM_PRIVATE_62,
	TEAM_PRIVATE_63,
	TEAM_PRIVATE_64,
	TEAM_PRIVATE_65,
	TEAM_PRIVATE_66,
	TEAM_PRIVATE_67,
	TEAM_PRIVATE_68,
	TEAM_PRIVATE_69,
	TEAM_PRIVATE_70,
	TEAM_PRIVATE_71,
	TEAM_PRIVATE_72,
	TEAM_PRIVATE_73,
	TEAM_PRIVATE_74,
	TEAM_PRIVATE_75,
	TEAM_PRIVATE_76,
	TEAM_PRIVATE_77,
	TEAM_PRIVATE_78,
	TEAM_PRIVATE_79,
	TEAM_PRIVATE_80,
	TEAM_PRIVATE_81,
	TEAM_PRIVATE_82,
	TEAM_PRIVATE_83,
	TEAM_PRIVATE_84,
	TEAM_PRIVATE_85,
	TEAM_PRIVATE_86,
	TEAM_PRIVATE_87,
	TEAM_PRIVATE_88,
	TEAM_PRIVATE_89,
	TEAM_PRIVATE_90,
	TEAM_PRIVATE_91,
	TEAM_PRIVATE_92,
	TEAM_PRIVATE_93,
	TEAM_PRIVATE_94,
	TEAM_PRIVATE_95,
	TEAM_PRIVATE_96,
	TEAM_PRIVATE_97,
	TEAM_PRIVATE_98,
	TEAM_PRIVATE_99,
	TEAM_PRIVATE_100,
	TEAM_PRIVATE_101,
	TEAM_PRIVATE_102,
	TEAM_PRIVATE_103,
	TEAM_PRIVATE_104,
	TEAM_PRIVATE_105,
	TEAM_PRIVATE_106,
	TEAM_PRIVATE_107,
	TEAM_PRIVATE_108,
	TEAM_PRIVATE_109,
	TEAM_PRIVATE_110,
	TEAM_PRIVATE_111,
	TEAM_PRIVATE_112,
	TEAM_PRIVATE_113,
	TEAM_PRIVATE_114,
	TEAM_PRIVATE_115,
	TEAM_PRIVATE_116,
	TEAM_PRIVATE_117,
	TEAM_PRIVATE_118,
	TEAM_PRIVATE_119,
	TEAM_PRIVATE_120,
};

enum CampType
{
	CAMP_NONE,
	CAMP_BLUE,
	CAMP_RED,
};

enum WinTeamType
{
	WTT_NONE,
	WTT_RED_TEAM,
	WTT_BLUE_TEAM,
	WTT_DRAW,
	WTT_VICTORY_RED_TEAM,
	WTT_VICTORY_BLUE_TEAM
};

enum RaceType
{
	RT_HUMAN,
	RT_ELF,
	RT_DWARF,
	RT_MAX_RACE_TYPE
};

enum RaceDetailType
{
	RDT_HUMAN_MAN,
	RDT_HUMAN_WOMAN,
	RDT_ELF_MAN,
	RDT_ELF_WOMAN,
	RDT_DWARF_MAN,
	RDT_DWARF_WOMAN,
	MAX_RACE_DETAIL
};

enum ModeType
{
	MT_NONE,
	MT_SYMBOL,
	MT_CATCH,
	MT_KING,
	MT_TRAINING,
	MT_SURVIVAL,
	MT_TEAM_SURVIVAL,
	MT_BOSS,
	MT_MONSTER_SURVIVAL,
	MT_FOOTBALL,
	MT_HEROMATCH,
	MT_GANGSI,
	MT_DUNGEON_A,
	MT_HEADQUARTERS,
	MT_CATCH_RUNNINGMAN,
	MT_FIGHT_CLUB,
	MT_TOWER_DEFENSE,
	MT_DARK_XMAS,
	MT_FIRE_TEMPLE,
	MT_DOUBLE_CROWN,
	MT_SHUFFLE_BONUS,
	MT_FACTORY,
	MT_TEAM_SURVIVAL_AI,
	MT_HOUSE,
	MT_MYROOM,		
	MT_UNDERWEAR,
	MT_CBT,
	MT_RAID,
	MAX_MODE_TYPE,
};

enum RoomStyle
{
	RSTYLE_NONE	= 0,
	RSTYLE_BATTLEROOM,   //������
	RSTYLE_PLAZA,		 //����
	RSTYLE_LADDERBATTLE, //�����
	RSTYLE_HEADQUARTERS, //�Ʒü�
	RSTYLE_SHUFFLEROOM,  //����������
};

enum PlazaType
{
	PT_NONE      = 0,
	PT_BATTLE	 = 1,
	PT_COMMUNITY = 2,
	PT_GUILD     = 3,
};

enum MyRoomType
{
	MRT_LOBBY = 0,	
	MRT_BATTLE,
	MRT_LADDER,
	MRT_PRACTICE,
	MRT_SHUFFLE,
	MRT_FIND_MODE,
};

enum ApplyWeaponExp
{
	AWE_NO_EXPLOSION,
	AWE_EXPLOSION,
	AWE_DEFENSE_EXPLOSION,
	AWE_MISSILE_EXPLOSION,
	AWE_G_EXPLOSION,
	AWE_G_DEFENSE_EXPLOSION,
};

enum StatType
{
	STAT_NONE,
	STAT_STR,
	STAT_DEX,
	STAT_INT,
	STAT_VIT,
};

enum ElementType
{
	ET_NONE,
	ET_FIRE		= 1,
	ET_ICE		= 2,
	ET_BOMB		= 3,
	ET_MINE		= 4,
	ET_LIGHTING	= 5,
	ET_PSYCHO   = 6,
	//ET_AIRMINE  = 6,
	MAX_ELEMENT_TYPE
};

enum ElementProtectionType
{
	EPT_WEAK_RATE       = 0,
	EPT_WEAK_PROTECTION = 1,
};

enum ArmorType
{
	AT_UNARMOR,
	AT_LIGHT,
	AT_MEDIUM,
	AT_HEAVY,
	MAX_ARMOR_TYPE,
};

enum AttackSide
{
	AS_NONE,
	AS_FRONT,
	AS_BACK,
};

enum EquipSlot
{
	ES_WEAPON,
	ES_ARMOR,
	ES_HELMET,
	ES_CLOAK,
	ES_OBJECT,
	ES_WEAR,
	ES_RING,
	ES_NECKLACE,
	ES_BRACELET,
	MAX_EQUIP_SLOT,
	ES_UNKNOWN
};

enum DashType
{
	DT_ATTACK,
	DT_SUPER,
	DT_ROLLING,
	DT_SPEAR,
	DT_ROLLING_AIM,
	DT_ASSAULT,
	DT_ASSAULT_CHARGE,
	DT_DASH_AIM,
	DT_SIMPLE,
	DT_POWER_CHARGE,
	DT_EXTEND_ASSAULT,
	DT_POWER_CHARGE2,
	DT_POWER_CHARGE3,
	DT_CHARGE_COMBO,
	DT_RUN_ASSAULT,
	DT_NORMAL_COMBO,
	DT_SHAOLIN,
	DT_DASH_ATTACK2,
	DT_SUDDEN_ATTACK,
};

enum JumpType
{
	JT_DEFAULT,
	JT_DEFENSE_ATTACK,
	JT_HIGH_JUMP,
	JT_LONG_JUMP,
	JT_DOUBLE_JUMP,
	JT_AIM_JUMP,
	JT_COMBO_JUMP,
	JT_ROTATE_DOUBLE_JUMP,
	JT_LAND_ATTACK_JUMP,
	JT_AUTO_SHOT_JUMP,
	JT_FLASH_JUMP,
	JT_BADGUY_JUMP,
	JT_KISARAGI_JUMP,
	JT_COLLECT_JUMP,
	JT_POWER_CHARGE_JUMP,
	JT_AIM_JUMP2,
	JT_RAGNA_JUMP,
	JT_CHARGE_COMBO,
	JT_CHARGE_COMBO2,
	JT_CHARGE_COMBO3,
	JT_GATOTKACA_JUMP,
	JT_RACHEL_JUMP,
	JT_ILJIMAE_JUMP,
	JT_AIR_GRAP_ATTACK_JUMP,
	JT_DESPERADO_JUMP,
	JT_TRINITY_JUMP,
	JT_CHARGE_FLASH_JUMP,
	JT_DOUBLE_SHOT_JUMP,
	JT_CHARGE_COMBO4,
	JT_DOUBLE_JUMP2,
	JT_COMBO_FLASH,
	JT_CHARGE_COMBO_DEFENSE,
	JT_HAKUMEN_JUMP,
	JT_SUDDEN_ATTACK_JUMP,
	JT_TRACE_JUMP,
	JT_LEE_JUMP,
	JT_JORO_EXPANSION_JUMP,
	JT_PARKILPYO_JUMP,
	JT_DOUBLE_JUMP3,
	JT_ROBIN_EXPANSION_JUMP,
	JT_SABER_JUMP,
	JT_NATA_JUMP,
	JT_GRAPPLING_EXPANSION_JUMP,
	JT_STRIDER_JUMP,
};

enum CounterAttackType
{
	CAT_NONE,
	CAT_DEFAULT,
	CAT_CHARGE,
	CAT_EXTRA,
	CAT_BREAK,
	CAT_CHARGE_COMBO,
};

enum ProtectionMode
{
	PM_NONE,
	PM_START,
	PM_REVIVAL,
	PM_GETUP,
	PM_CATCH,
	PM_ESCAPE,
	PM_BOSS,
	PM_GM
};

enum SpecialAtcionType
{
	SPA_NONE,
	SPA_KILL,
	SPA_DIE,
	SPA_FALL,
	SPA_ACTION,
	SPA_MAX,
};

enum BuffExceptionType
{	
	BET_KILLING = 1,
	BET_SUICIDE,
};

enum CameraType
{
	CT_DEFAULT,
	CT_RUNDASH,
	CT_EXTEND,
	CT_BUFF,
	CT_SKILL,
};


enum TargetColType
{
	TCT_SPHERE	= 1,
	TCT_CYLINDER= 2,
	TCT_BOX		= 3,
	TCT_DONUT	= 4,
	TCT_CONE	= 5,
};

enum TargetWoundType
{
	TWT_ALL					= 1,
	TWT_AIR_WOUND_EXCEPT	= 2,
	TWT_DOWN_WOUND_EXCEPT	= 3,
	TWT_ALL_WOUND_EXCEPT	= 4,
	TWT_ALL_WOUNDED			= 5,
	TWT_FLOATING_STATE		= 6,
	TWT_AIR_WOUND			= 7,
	TWT_DOWN_WOUNDED		= 8,
	TWT_WOUNDED				= 9,
};

enum PushStructType
{
	PSTT_NOMAL,
	PSTT_ARMY,
	PSTT_CANNON,
	PSTT_VULCAN,
	PSTT_BOMBER,
	PSTT_BLOCK,
};

enum StructCreateType
{
	SCT_NORMAL,
	SCT_KILLER,
	SCT_NO_OWNER,
};

enum MachineStructType
{
	MCT_NONE,
	MCT_CANNON,
	MCT_CANNON2,
};

enum CharLandingType
{
	CLT_NORMAL,
	CLT_EXTEND,
	CLT_LAND_ATTACK,
	CLT_EXTEDN_LAND_ATTTACK,
	CLT_JUMP_DEFENSE,
	CLT_USE_LAND_AFTER,
};

enum FlyMoveType
{
	FMT_NORMAL,
	FMT_BIRD,
	FMT_DIRECT,
	FMT_ROTATE,
	FMT_PETERPAN,
};

enum IgnoreGhostStateType
{
	IGT_NONE,
	IGT_MISSILE,
	IGT_EXPLOSION,
};

enum SpecialGaugeType
{
	SSGT_NONE,			// Ÿ�Կ� ������� ��� ����
	SSGT_WOLF,			// ������� ���� �������� ����
	SSGT_FOX,			// ����ȣ ���� �������� ����
	SSGT_RAPTOR,		// �巡����̴� ���� �������� ����
	SSGT_GRIFFIN,		// ���巯�� ���� �������� ����
	SSGT_GHOST_STEALER,	//�Ƿɵ���
	SSGT_HAKUMEN,		//������
};

enum WeaponEventType
{
	WET_NONE,
	WET_NORMAL_DEAD,		// �븻 ����
	WET_MAP_COLL,			// �ʿ� �浹�ؼ� �׾�����
	WET_MAP_COLL_NO_DEAD,	// �� ���� ������ ������ �ʿ� �浹 ������
};

// û��öȸ
enum
{
	SUBSCRIPTION_NONE	= 0,	// û��öȸ �Ұ�
	SUBSCRIPTION_TYPE1	= 1,	// û��öȸ ����
};

#define MAX_PLAYER		    16 // ��� �� ������
#define MAX_PLAZA_PLAYER	32 // ���� �� ������

#define MAX_CHARACTER_KEY   36 // ����Ű �����ϴ� �ִ� ĳ���� ��

#define MAX_ITEMSLOT    3      //������ �����Ҽ� �ִ� �ִ� ������ ���� ��.
#define MAX_INVENTORY   4
#define MAX_ACC_INVENTORY	3	//�׼�����
#define MAX_ONESTATATTRIBUTE	5 //�� ������ ������ �ִ� Ư�� ��.
#define MAX_RIGHT_SLOT_SIZE 20	//������ ���� �� �ִ� �ε��� ������

// ���ø�� Phase ��.
enum
{
	SHUFFLE_PHASE_1   = 0,
	SHUFFLE_PHASE_2   = 1,
	SHUFFLE_PHASE_3   = 2,
	SHUFFLE_PHASE_4   = 3,
	SHUFFLE_PHASE_MAX = 4,
};

enum ModeItemType
{
	MIT_NONE,
	MIT_SHUFFLE_STAR,
	MIT_BUFF,
	MIT_HUNTER_COIN,
	MIT_MAX,
};

enum EquipItemExtendType
{
	EIET_BASIC,				// �⺻ ������(������ �ڵ��� ����° �ڸ����� 0, ex) 1[0]0001 )
	EIET_BASIC_EXTREA,		// ���� ������(����° �ڸ��� 1, õ��° �ڸ��� 0, ex) 1[1][0]001  ) 
	EIET_RARE,				// ���� ������(����° �ڸ��� 1, õ��° �ڸ��� 1, ex) 1[1][1]001  ) 
	EIET_RARE_POWERUP,		// ��ȭ ���� ������(����° �ڸ��� 1, õ��° �ڸ��� 5 ~ 9, ex) 1[1][5]001  ) 
	EIET_SPECIAL_EXTREA,	// Ư�� ���� ������(����° �ڸ��� 2 ~ 4, õ��° �ڸ��� 0, ex) 1[2][0]001  )
	EIET_BASIC_POWERUP,		// ��ȭ �⺻ ������(����° �ڸ��� 5 ~ 9, õ��° �ڸ��� 0, ex) 1[5][0]001  )
	EIET_EXTRA_POWERUP,		// ��ȭ ���� ������(����° �ڸ��� 5 ~ 9, õ��° �ڸ��� 1, ex) 1[5][1]001  )
};

enum DEFAULTITEMTYPE
{
	DEFAULT_BASIC_ITEM_CODE = 100000,
	DEFAULT_EXTRA_ITEM_CODE = 10000,
	DEFAULT_RARE_ITEM_CODE	= 1000,	
};

//�뺴 , ������ �Ŀ���
enum PowerUpTargetType
{
	PUTT_NONE = 0,
	PUTT_CHAR = 1,
	PUTT_ITEM = 2,
	PUTT_MAX  = 3,
};

enum PowerUpGradeType
{
	PUGT_NONE				= 0,

	PUGT_CHAR_GRADE_BASE	= 1,

	PUGT_CHAR_GRADE1		= 1,
	PUGT_CHAR_GRADE2		= 2,
	PUGT_CHAR_GRADE3		= 3,
	PUGT_CHAR_GRADE4		= 4,
	PUGT_CHAR_GRADE5		= 5,
	PUGT_CHAR_GRADE_MAX		= 6,

	PUGT_ITEM_EXTREA		= 1000,
	PUGT_ITEM_GRADE_BASE	= 10000,
	
	PUGT_ITEM_GRADE1		= 50000,
	PUGT_ITEM_GRADE2		= 60000,
	PUGT_ITEM_GRADE3		= 70000,
	PUGT_ITEM_GRADE4		= 80000,
	PUGT_ITEM_GRADE5		= 90000,
	PUGT_ITEM_GRADE_MAX		= 100000,
};

enum PowerUpRareItemGradeType
{
	PURIGT_NONE					= 0,

	PURIGT_RARE_ITEM_GRADE_BASE	= 1000,

	PURIGT_RARE_ITEM_GRADE1		= 5000,
	PURIGT_RARE_ITEM_GRADE2		= 6000,
	PURIGT_RARE_ITEM_GRADE3		= 7000,
	PURIGT_RARE_ITEM_GRADE4		= 8000,
	PURIGT_RARE_ITEM_GRADE5		= 9000,
	PURIGT_RARE_ITEM_GRADE_MAX	= 10000,
};

enum ClearAttackFireType
{
	CAFT_NONE,
	CAFT_APPLY_WOUNDED,
	CAFT_APPLY_WOUNDED_BY_DUMMY,
};

int ConvertPowerUpItemToChar( int eItemGradeType );
int ConvertPowerUpCharToItem( int eCharGradeType );
int ConvertPowerUpRareItemToChar( int eItemGradeType );
int ConvertPowerUpItemToItemGrade( int iItemCode );
int ConvertPowerUpRareItemToRareItemGrade( int iItemCode );
int ConvertItemToClassType( int iItemCode );

bool IsPowerUpItemGrade( int eItemGrade );
bool IsPowerUpRareItemGrade( int eItemGrade );
bool IsPowerUpItem( int iItemCode );

EquipItemExtendType GetEquipItemExtendType( int iItemCode );

#define SET_ITEM_CODE   700000  // ��Ʈ ������ �ν� �ڵ�. 
#define SET_NOT_FIND_CODE -99999

#ifdef SRC_TH
#define ID_NUMBER       519
#define ID_NUM_PLUS_ONE 520
#else
#define ID_NUMBER       20
#define ID_NUM_PLUS_ONE 21
#endif

#define MAX_CHAR_GROWTH	4
#define MAX_ITEM_GROWTH 4
#define CUR_GROWTH_CNT 4

enum EventShopType
{
	EST_NONE,
	EST_EVENT,
	EST_CLOVER,
	EST_MILEAGE,	
	EST_SECRET,
};

// UDP ��� ��Ÿ��
#define P2P_STYLE_PLAY                 0x0001                // �÷��� ��Ŷ ��ȣ
#define P2P_STYLE_VOICE                0x0002                // ���� ��Ŷ ��ȣ

#define FONT_SIZE_9   0.45f
#define FONT_SIZE_10  0.50f
#define FONT_SIZE_11  0.55f
#define FONT_SIZE_12  0.60f
#define FONT_SIZE_13  0.65f
#define FONT_SIZE_14  0.70f
#define FONT_SIZE_15  0.75f
#define FONT_SIZE_16  0.80f
#define FONT_SIZE_17  0.85f
#define FONT_SIZE_18  0.90f
#define FONT_SIZE_19  0.95f
#define FONT_SIZE_20  1.00f
#define FONT_SIZE_22  1.10f
#define FONT_SIZE_24  1.20f
#define FONT_SIZE_25  1.25f
#define FONT_SIZE_26  1.30f
#define FONT_SIZE_27  1.35f
#define FONT_SIZE_28  1.40f
#define FONT_SIZE_32  1.60f
#define FONT_SIZE_36  1.80f

// Show Window Cursor Style ( Bit ����� )
#define SWCT_TITLE     0x0001
#define SWCT_WEB       0x0002
#define SWCT_CANDIDATE 0x0004

#ifdef STEAM_ATTACH
	#define STAEM_APP_ID 266150
#endif

#define FREEDAY_EVENT_CODE			(0x01000000)

bool IsWinTeam( WinTeamType eWinTeam, TeamType eTeam );
bool IsRedWin( WinTeamType eWinTeam );
bool IsBlueWin( WinTeamType eWinTeam );

bool IsVictoryWin( WinTeamType eWinTeam );

TeamType ConvertStringToTeamType( const char *szType );
RaceType ConvertStringToRaceType( const char *szType );

class ioINILoader;
struct CHARACTER;

bool SetCharLoader( ioINILoader &rkLoader, RaceDetailType eType );
EquipSlot GetEquipSlotByCode( DWORD dwItemCode );

RaceDetailType GetRaceDetailType( const CHARACTER &rkInfo );
RaceDetailType GetRaceDetailType( RaceType eRace, bool  bMale );

bool GetFaceChangeName( OUT ioHashString &rszNewFaceName,
					    IN const char *szPostFix,
						IN bool bMale,
						IN int iFaceType = -1 );
