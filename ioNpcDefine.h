#pragma once

#define NPC_INDEX     0xffffffff

enum NpcType
{
	NT_DEFAULT		= 0,
	NT_DEVELOPER_K  = 1,  // �Ʒ����ʹ� ������ �� ���� npc
	NT_BIGTOWER		= 2,
	NT_TOWERGUARD	= 3,
	NT_LAIR			= 4,
	NT_LAIRGUARD	= 5,
	NT_NEXUS		= 6,
	NT_NEXUSGUARD	= 7,
	END_FIXING_NPC,   // ������ �� �����̴� npc ��  TODO:���߿� �����ϴ� structure �� ��ü �ʿ�
};

enum AIType
{
	AI_NONE		= 0,
	AI_NORMAL	= 1,
	AI_CHAMPION	= 2,
};
