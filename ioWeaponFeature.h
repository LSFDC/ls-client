#pragma once


enum WeaponFeature
{

	WF_MUST_ITEM_DROP = 0x2,
	WF_ATTACK_DIRECTION_STRAIGHT = 0x4,
	WF_NO_ITEM_DROP = 0x8,
	WF_WEAK_ANI_DEFENSE_SUCCESS = 0x10,
	WF_TEAM_ATTACK_NO_DAMAGE = 0x20,
	WF_PUSH_POWER_LINEAR = 0x40,
	WF_AIR_PUSH_POWER_LINEAR = 0x80,
	WF_ATTACKER_LOOK = 0x100,
	WF_DOWN_ATTACK_WEAPON = 0x200,
	WF_FROZEN_BREAK = 0x400,
	WF_DISABLE_HIT_COUNT = 0x800,
	//WF_ESCAPE_CATCH = 0x1000,
	WF_DIR_LOOK = 0x2000,
};

enum WeaponPeculiarity
{
	WP_ONE_TIME_USE = 0x1,
	WP_MISSILE_WEAPON = 0x2,
	WP_ATTACK_OWNER = 0x4,
	WP_MINE_EXPLOSION = 0x10,
	WP_SAME_TEAM_NO_ITEM_DROP = 0x20,
	WP_WOUND_NO_SOUND = 0x40,
	WP_DEF_NO_SOUND = 0x80,
	WP_DIVIDE_ACTION = 0x100,
};
