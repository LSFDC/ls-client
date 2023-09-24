#pragma once

enum SkillType
{
	ST_NONE,
	ST_ATTACK,
	ST_BUFF,
	ST_NORMAL,
	ST_PASSIVE,
	ST_RANGE,
	ST_MULTI,
	ST_COUNT,
};

enum AttackSubType
{
	AST_DEFAULT,
	AST_COW_RUN,
	AST_THROW,
	AST_BIG_HAMMER,
	AST_NUCLEAR,
	AST_FLAME_8WAY,
	AST_SCATTER_BOMB,
	AST_FULL_SHOT,
	AST_MINE,
	AST_METEOR,
	AST_RANDOM_GENERATE,				// 10,
	AST_FIRE,
	AST_MOVE_ATTACK,
	AST_CONSECUTION,
	AST_ROT_CONSECUTION,
	AST_CONTACT_CONSECUTION,
	AST_RUNNING_ATTACK,
	AST_WHIP_ATTACK,
	AST_LINEAR_COMBO,
	AST_SPIN_CHARGING,
	AST_CONTROL_FIRE,					// 20,
	AST_GRAPPLING_SWING,
	AST_GRAPPLING_BLOW,
	AST_DUMMY_CHAR,
	AST_PSYC_GRAPPLING,
	AST_TRACE,
	AST_BREATH,
	AST_COMBOWHIP,
	AST_GRAPPLING,
	AST_RUNNING_GRAPPLING,
	AST_MONSTER_SKILL,					// 30,
	AST_GRAPPLING2,
	AST_RUNNING_GRAPPLING_JUMP,
	AST_ANGLE_FIRE,
	AST_COMBO_FORCE_EXPLOSION_FIRE,
	AST_TELEPORT_GRAPPLING_SWING,
	AST_CONTROL_DUMMY,
	AST_AIR_COMBO_ATTACK,
	AST_UROBORUS,
	AST_UROBORUS2,
	AST_DEFENSE_ATTACK,					// 40,
	AST_MOVE_ATTACK2,
	AST_TOSS_SKILL,
	AST_RUNNING_ATTACK2,
	AST_TRACE_WARP,
	AST_CONSECUTION2,
	AST_TARGET_WARP,
	AST_GRAPPLING_ATTACK,
	AST_RUNNING_ATTACK3,
	AST_GRAPPLING_ATTACK2,
	AST_GRAPPLING3,						// 50,
	AST_ANGLE_FIRE2,
	AST_CHANGE_POSITION,
	AST_GRAPPLING_ATTACK3,
	AST_GRAPPLING_ATTACK4,
	AST_MECHANICS_COMBO,
	AST_MECHANICS_FIRE,
	AST_MULTI_TELEPORT,
	AST_DEFENSE_ATTACK2,
	AST_GUIDE_ATTACK,
	AST_CANNON_CHARGE,					// 60
	AST_THROW_CHAR,
	AST_DOUBLE_ICE_MISSILE,
	AST_FIRE2,
	AST_TARGET_CHANGE_GANGSI,
	AST_DASH_HAND_ATTACK,
	AST_BLINDLY_SHOT,
	AST_CONSECUTION3,
	AST_GRAPPLING4,
	AST_TWO_STATE_ATTACK,
	AST_ANGLE_FIRE3,					// 70
	AST_MULTI_LIGHT,
	AST_LASER_CONTROL,
	AST_ANGLE_FIRE4,
	AST_RUNNING_ATTACK4,
	AST_FULL_SHOT2,
	AST_CHAIN_TRACE,
	AST_FIRE_WORKS,
	AST_RUNNING_GRAPPLING2,
	AST_CHARGE_LINE_COMBO,
	AST_CONTROL_FIRE2,					// 80
	AST_MOVE_ATTACK3,
	AST_FIRE3,
	AST_MOSESS,
	AST_LIGHT_ATTACK,
	AST_SELECT_TRIPLE_WEAPON,
	AST_AREA_CHECK_BUFF,
	AST_CRECENDO,
	AST_CHARGING_FIRE7,
	AST_PROTECT_COUNT_AREA_ATTACK,
	AST_SPAWN_NPC,						//90,
	AST_RUNNING_ATTACK5,
	AST_CONTROL_DUMMY2,
	AST_JUMPING_FIRE,
	AST_RUNNING_ATTACK6,
	AST_RUNNING_GRAPPLING3,
	AST_CREATE_DUMMY,
	AST_CONTROL_FIRE3,
	AST_FIGURESKATING,
	AST_GRAPPLING_SWING2,
	AST_DUMMY_CHAR2,					//100,
	AST_RUNNING_ATTACK7,
	AST_CONSECUTION4,
	AST_GRAPPLING_BLOW_LOOP,
	AST_TRACE3,
	AST_RANDOM_EFFECT_EXPLOSION,
	AST_COMBO_JUMP_ATTACK,
	AST_DUMMY_GRAPPLING_ATTACK,
	AST_REPEAT_MOVE_ATTACK,
	AST_MYSTERY_TARGET_POS_CHANGE,
	AST_DASH_MOVE_CHARGE_ATTACK,		//110,
	AST_RANDOM_GENERATE2,
	AST_JUMP_AREA_ATTACK,
	AST_TARGET_MOVE_COMBO_ATTACK,
	AST_REPEAT_TELEPORT_ATTACK,
	AST_REPEAT_MOVE_ATTACK2,
	AST_JUMPING_FIRE2,
	AST_KEY_WAIT_SKILL,
	AST_CREAT_WEPON_BY_INPUT_KEY,
	AST_CHARGING_FIRE8,
	AST_REPEAT_TELEPORT_ATTACK2,		//120
	AST_RUNNING_ATTACK8,
	AST_TARGET_FLOW_ATTACK,
	AST_WHIRLWND_ATTACK,
	AST_LINEAR_COMBO2,
	AST_GRAPPLING_BLOW_LOOP2,
	AST_RUNNING_ATTACK9,
	AST_RUNNING_ATTACK10,
	AST_CIRCLE_RESERVE,
	AST_CREATE_WEAPON_ATTACK,
	AST_TARGET_TIMING_ATTACK,			//130
	AST_FIRE_AND_LINER,
	AST_SPACE_MASTER_ATTACK,
	AST_REPEAT_LOOP_ATTACK,
	AST_TARGET_REPEAT_ATTACK,
	AST_RANDOM_EXPLOSION,
	AST_TOIST_MAGIC_ATTACK,
	AST_JUMP_ATTACK,
	AST_REPEAT_LOOP_ATTACK2,
	AST_TARGET_TELEPORT_ATTACK,
	AST_CONTROL_FIRE4,					//140
	AST_MULTIPLE_ELEMENT_ATTACK,
	AST_CONTROL_FIRE5,
	AST_GRAPPLING_ATTACK5,
	AST_RANDOM_GENERATE_MOVE,
	AST_FIRE_AND_BUFF_CHECK,	
	AST_CONSECUTION5,
	AST_RUNNING_ATTACK11,
	AST_CIRCLE_MINE,
	AST_LINEAR_COMBO3,
	AST_FURIOUS_TELEPORT_ATTACK,		//150
	AST_ROTATE_DIR_KEY,
	AST_TARGET_THROW_WEAPON,
	AST_EFFECT_MOVE_ATTACK,
	AST_CREATE_TIC_WEAPON_ATTACK,
	AST_BINDING_FIRE_ATTACK,
	AST_COMBO_AND_GRAPPLING_ATTACK,
	AST_PSYC_GRAPPLING2,
	AST_CONTROL_DUMMY3,
	AST_LASER,
};

enum BuffSubType
{
	BST_DEFAULT,
	BST_AUTO,
	BST_CREATE_ITEM,
	BST_MULTI_ACTIVE,
	BST_TARGET,
	BST_MULTI_ACTIVE2,
};

enum NormalSubType
{
	NST_DEFAULT,
	NST_BILDING,
	NST_TELEPORT,
	NST_DODGE,
	NST_OBJECT,
	NST_AREA_STRUCT,
	NST_AREA_CONTROL,
	NST_STATE_ESCAPE,
	NST_ABSORB,
	NST_CREEP,
	NST_CIRCLE_MOVE,					// 10,
	NST_CIRCLE_MOVE_ATTACK,
	NST_RELOAD,
	NST_WEAPON_ACTION,
	NST_CIRCLE_MOVE_WEAPON,
	NST_LINEAR_FLOAT_WEAPON,
	NST_TARGET_MOVE_ATTACK,
	NST_COMBO_MOVE_ATTACK,
	NST_CHARGING_FIRE,
	NST_CHARGING_FIRE2,
	NST_CIRCLE_MOVE_KEY,				// 20,
	NST_MULTI_AREA,
	NST_CIRCLE_MOVE_ATTACK2,
	NST_CHARGING_FIRE3,
	NST_MULTI_DUMMYCHAR,
	NST_COMBO_ATTACK,
	NST_TARGET_MISSILE,
	NST_WEAPON_CONTROL,
	NST_NORMAL_FIRE,
	NST_CHARGING_FIRE4,
	NST_CHARGING_FIRE5,					// 30,
	NST_AREA_STRUCT2,
	NST_CIRCLE_MOVE2,
	NST_MULTI_AREA2,
	NST_CIRCLE_MOVE_WEAPON2,
	NST_FLYING_TELEPORT,
	NST_CIRCLE_MOVE3,
	NST_TELEPORT_ATTACK,
	NST_TARGET_DUMMY_CHAR,
	NST_CIRCLE_MOVE_ATTACK3,
	NST_COMBO_MOVE_ATTACK2,				// 40,
	NST_TARGET_TELEPORT,
	NST_COMBO_MOVE_ATTACK3,
	NST_MULTI_AREA3,
	NST_MULTIPLE_OBJECT,
	NST_FLASH_TRANSFORMATION,
	NST_TARGET_DEBUFF,
	NST_TWO_CIRCLE,
	NST_SUMMON_WOOD,
	NST_WEAPON_CONTROL2,
	NST_CIRCLE_MOVE4,					// 50,
	NST_MULTI_CIRCLE,
	NST_TARGET_SHOT_OWNER,
	NST_TRACE2,
	NST_AREA_CONTROL2,
	NST_CIRCLE_MOVE_WEAPON3,
	NST_CHARGING_FIRE6,
	NST_CIRCLE_MOVE_WEAPON4,
	//T60 메가 리스트릭션
	NST_TARGET_DEBUFF2,
	NST_COMBO_JUMP,
	NST_MULTI_CIRCLE2,					// 60,
	NST_TARGET_DEBUFF3,
	NST_TARGET_TELEPORT2,
	NST_CIRCLE_MOVE_ATTACK4,
	NST_TARGET_DEBUFF4,
	NST_CIRCLE_MOVE_ATTACK5,
	NST_CIRCLE_MOVE_ATTACK6,
	NST_CIRCLE_MOVE_ATTACK7,			// T70 메가 낚시대
	NST_TARGET_MOVE_ATTACK2,
	NST_RANDOM_BUFF,
	NST_COMBO_MOVE_ATTACK4,				// 70,
	NST_COMBO_JUMP2,
	NST_CIRCLE_MOVE5,
	NST_TARGET_MISSILE2,
	NST_SCAB_SKILL,						//고무딱지 기념 스킬
	NST_RANGE_TARGET_SHOT,
	NST_DESTROY,
	NST_CIRCLE_MOVE6,
	NST_DUMMY_COMBO_FIRE,
	NST_MULTI_AREA4,
	NST_CIRCLE_MOVE_CREATE_DUMMY,		// 80
	NST_TELEPORT_ATTACK2,
	NST_JUMP_LINEAR_COMBO,
	NST_TELEPORT_ATTACK3,
	NST_CIRCLE_MOVE_ATTACK8,
	NST_ENGINEER_MACHINE_GUN,
	NST_WEAPON_CONTROL3,
	NST_FLY_MOVE_ATTACK,
	NST_TARGET_DEBUFF5,
	NST_CIRCLE_MOVE_ATTACK9,			// 90
	NST_TARGET_DEBUFF6,
	NST_CIRCLE_MOVE7,
	NST_CIRCLE_MOVE_WEAPON5,
	NST_CIRCLE_ATTACK,
	NST_CIRCLE_MOVE_WEAPON6,
	NST_TARGET_MOVE_ATTACK3,
	NST_CIRCLE_MOVE8,
	NST_MULTI_CIRCLE_MOVE_ATTACK,
	NST_CIRCLE_MOVE_AREA_WEAPON,
};

enum RangeSubType
{
	RST_BUFF,
	RST_WEAPON,
	RST_TELEPORT,
	RST_SYNC_BUFF,
	RST_TELEPORT2,
	RST_POSITION_INVERSE,
	RST_BUFF_ATTACK,
};

enum PassiveSubType
{
	PST_DEFAULT,
	PST_WOUND,
	PST_RANDOM,
	PST_ASSESSORY,
};

enum SkillCharColType
{
	SCCT_NONE,
	SCCT_ALL,
	SCCT_TEAM,
};

enum CountSubType
{
	CST_ATTACK,
	CST_GLIDE,
	CST_LIGHT,
	CST_FORCE,
	CST_ROLLING,
	CST_DASH_ATTACK,
	CST_TELEPORT_ATTACK,
	CST_WHIP_ATTACK,
	CST_BUFF_CHECK_ATTACK,
	CST_BUFF,
};