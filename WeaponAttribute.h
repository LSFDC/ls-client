#pragma once

#include "../io3DEngine/ioINILoader.h"

enum AttackMethodType
{
	AMT_NONE,
	AMT_RANGE,
	AMT_CONTACT,
	AMT_MAGIC,
	AMT_BOMB,
	MAX_AMT_TYPE
};

enum AttackStatType
{
	ATT_STAT_NONE,
	ATT_STAT_STR,
	ATT_STAT_DEX,
	ATT_STAT_INT,
	ATT_STAT_VIT,
};

enum AttackWeaponType
{
	AWT_DUMMY,
	AWT_CONTACT,
	AWT_MISSILE,
	AWT_THROW,
	AWT_BOMB,
	AWT_MINE,
	AWT_NUCLEAR,
	AWT_FLAME,
	AWT_ZONE_EFFECT,
	AWT_FLAMEWAVE,
	AWT_SPLASH,						// 10,
	AWT_ARROW,
	AWT_BLIZZARD,
	AWT_THROWBOMB,
	AWT_FLOAT_THROW,
	AWT_THROW_RETURN,
	AWT_EXPLOSION,
	AWT_BOMB_MISSILE,
	AWT_CANNON_BALL,
	AWT_ROTATE,
	AWT_TRACE,						// 20,
	AWT_BOUNDBOMB,
	AWT_TARGET,
	AWT_CHAIN,
	AWT_BALL_WEAPON,
	AWT_IGNORE_MISSILE,
	AWT_AREA_EFFECT,
	AWT_MAP_ATTACH,
	AWT_LIMIT_ATTACH,
	AWT_OWNER_ATTACK,
	AWT_RUN_CONTACT,				// 30,
	AWT_WHIP,
	AWT_ICE_MISSILE,
	AWT_SUMMON_MISSILE,
	AWT_GROUND_TRACE,
	AWT_CONTROL_RETURN,
	AWT_CONTROL_MISSILE,
	AWT_TRACE_RETURN,
	AWT_TSUNAMI,
	AWT_TRACE2,
	AWT_NORMAL,						// 40,
	AWT_ADHESIVE_MISSILE,
	AWT_LIMIT_ATTACH2,
	AWT_CONTACT2,
	AWT_BOMB_MISSILE2,
	AWT_ADHESIVE_MISSILE2,
	AWT_ZONE_EFFECT2,
	AWT_UROBORUS,
	AWT_UROBORUS2,
	AWT_TRACE3,
	AWT_MISSILE2,					// 50,
	AWT_CALL_ATTRIBUTE_ARROW,
	AWT_RETURN_PINGPONG,
	AWT_MISSILE3,
	AWT_LOBELIA,
	AWT_SYLPHID_MISSILE,
	AWT_GUIDE,
	AWT_BALL2_WEAPON,
	AWT_GUIDE2,
	AWT_TRACE4,
	AWT_FALL_WEAPON,				//60
	AWT_TRACE5,
	AWT_DOUBLE_ICE_MISSILE_WEAPON,
	AWT_SMART_BOOM_WEAPON,
	AWT_PULL_WEAPON,
	AWT_ADHESIVE_MISSILE3,
	AWT_GRAFFITI,
	AWT_MINE2,
	AWT_TRACE6,
	AWT_CONTROL_SWORD,
	AWT_PULL_WEAPON2,				//70
	AWT_GROUND_TRACE2,
	AWT_SELECT_POSITION_MOVE,
	AWT_SIMPLE_BOX,
	AWT_ADHESIVE_MISSILE4,
	AWT_BOMB_MISSILE3,		//������ ���� ����
	AWT_TRACE7,
	AWT_UROBORUS3,
	AWT_LUCIFER_AIR_GRAP,			//����� ���� ���
	AWT_TRACE_OWNER,
	AWT_IGNORE_MISSILE2,			//80
	AWT_ONE_TARGET_WEAPON,
	AWT_BOMB_MISSILE4,
	AWT_ATTACH_WEAPON,
	AWT_RETURN_MISSILE_WEAPON,
	AWT_UROBORUS4,
	AWT_FIRE_WORKS_STEP1_WEAPON,
	AWT_FIRE_THOW_BOM_WEAPON,
	AWT_THROWBOMB2,
	AWT_ROTATE_TARGET_WEAPON,
	AWT_MINE3,						//90
	AWT_RETURN_MISSILE_WEAPON2,
	AWT_ZONE_MOVE_WEAPON,
	AWT_PRISON_WEAPON,
	AWT_UROBORUS5,
	AWT_ROTATE2,
	AWT_CONTROL_MESH,
	AWT_MINE4,
	AWT_DESTROY_CALL,
	AWT_ATTACH_AREA,
	AWT_MISSILE4,					//100
	AWT_UROBORUS6,
	AWT_ROTATE_ATTACH_CHAR,
	AWT_PULL_WEAPON3,
	AWT_CONTACT3,
	AWT_BOMB_MISSILE5,
	AWT_MINE5,
	AWT_MISSILE5,
	AWT_PULL_WEAPON4,
	AWT_ROTATE_RETURN_WEAPON,
	AWT_UROBORUS7,					//110
	AWT_CONTROL_RETURN2,
	AWT_GROUND_TRACE3,
	AWT_THROWBOMB3,
	AWT_SUMMON_MISSILE2,
	AWT_MAX,
};

enum BlowTargetType
{
	BTT_WEAK				= 0,	//���ǰ�
	BTT_BLOW				= 1,	//���ǰ�, Blow_Wounded �ִϸ��̼� ���
	BTT_FLATTEN				= 2,	//��¦
	BTT_M_STIFF				= 3,	//����(��), BTT_M_STIFF ���¿��� �ٽ� BTT_M_STIFF�� ������ BTT_STAND_KO���� ����
	BTT_S_STIFF				= 4,	//����(��), ���ϻ���
	BTT_STAND_KO			= 5,	//���� �ݱ�(õõ�� ������)
	BTT_STAND_KO2			= 6,	//���� �ݱ�(õõ�� ������), �ٸ� �ִϸ��̼� ���
	BTT_BLOW2				= 7,	//���ǰ�, BTT_BLOW�� �ٸ� Blow_Wounded �ִϸ��̼� ���
	BTT_BLOW3				= 8,	//���ǰ�, BTT_BLOW�� �ٸ� Blow_Wounded �ִϸ��̼� ���
	BTT_BOUND_BLOW			= 9,	//�ٿ��, Bound_Blow_Wounded �ִϸ��̼� ���
	BTT_M_BLOW				= 10,	//�����, BTT_M_BLOW ���¿��� BTT_M_BLOW�� �߰��� ���ð�� BTT_BLOW3����, BTT_M_BLOW���¿��� BTT_M_STIFF��� �� ��� BTT_STAND_KO ����
	BTT_GETUP				= 11,	//�⸳, BTT_GETUP ���¿��� BTT_M_STIFF�� ������ BTT_STAND_KO�� ����
	BTT_GETUP_STAND_KO		= 12,	//���� �⸳, ���¿� ������� ����� ������ ���� BTT_STAND_KO2�� ����
	BTT_BLOW_EXTEND_WOUND	= 13,	//�������ٰ� ���鿡�� ������ �̲�����
	BTT_MAX_TYPE,
	BTT_NONE,
};

enum CharPiercingType
{
	CPT_NONE,
	CPT_NORMAL,
	CPT_LIMITE,
	CPT_ALL,
	CPT_ALL_LIMITE,
	CPT_MAX,
};

enum DefenseBreakType
{
	DBT_NONE,
	DBT_NORMAL,
	DBT_ALL,
	DBT_CHANGE,
	DBT_MAX,
};

enum IgnoreProtectType
{
	IPT_NONE,
	IPT_ALL,
	IPT_EXCEPT_SYS,
};

enum DefenseSuccessType
{
	DST_STRONG,
	DST_WEAK,
	DST_SKILL,
	DST_MAX,
};

enum EnableSideType
{
	EST_NORMAL,
	EST_FRONT,
	EST_BACK,
	EST_MAX,
};

enum ChangeJumpType
{
	CJT_NONE,
	CJT_NORMAL,
	CJT_WOUNDED_ONLY,
	CJT_BLOW_WOUNDED_ONLY,
};

enum ChangeNextItemType
{
	CNIT_NONE,
	CNIT_TYPE1,//(�����ʿ� ���)
	CNIT_TYPE2,//(�����ʿ� ���)
	CNIT_TYPE3,//(�����ʿ� ���)
	CNIT_TYPE4,//(�������� ���)
	CNIT_TIGER1,//Ÿ�̰� ��ȭ�� ���
	CNIT_TIGER2,//Ÿ�̰� ��ȭ�� ���
	CNIT_TYPE5,//(�뽬�ʿ� ���)
};

//���� ��ų�� �߰�
enum ChangeNextSkillType
{
	CNST_NONE,
	CNST_TYPE1,
};

enum EscapeCatchType
{
	ECT_NONE,
	ECT_CATCH,
	ECT_CATCH_RETURN,
};

enum SkillEndJumpType
{
	SEJT_DEFAULT,
	SEJT_NOT_JUMP,
	SEJT_JUMP,	
};

enum WeaponExtraGaugeType
{
	WGT_NONE			= 0,
	WGT_EXCALIBUR		= 1,
	WGT_COMMAND			= 2,
	WGT_YUMIRA			= 3,
	WGT_CHARGE_CONTACT	= 4,
	WGT_WINDLORD		= 5,
	WGT_DARKKNIGHT		= 6,
	WGT_LEESUNSIN		= 7,
	WGT_ONIMUSHA		= 8,
	WGT_SASINMU			= 9,
};

enum MineType
{
	MIT_MINE,
	MIT_TRACE,
	MIT_TRAP,
	MIT_TRAP2,
	MIT_AIR,
	MIT_CONTROL_MINE,
	MIT_CIRCLE_MINE,
	MIT_MAX_CNT,
};

typedef struct BuffInfoStruct
{
	ioHashString m_BuffName;
	int m_iBuffType;

	BuffInfoStruct()
	{
		m_BuffName.Clear();
		m_iBuffType = 0;
	}

	BuffInfoStruct( const ioHashString &szName, int iType )
	{
		m_BuffName  = szName;
		m_iBuffType = iType;
	}
} ioBuffInfo;

typedef std::vector< ioBuffInfo > BuffInfoList;

typedef struct tagBlowWoundAniInfo
{
	ioHashString m_BlowingAni;
	ioHashString m_BlowingBackAni;

	ioHashString m_JumpBlowingAni;
	ioHashString m_JumpBlowingBackAni;

	ioHashString m_DownAni;
	ioHashString m_DownBackAni;

	tagBlowWoundAniInfo()
	{
		Init();
	}

	void Init()
	{
		m_BlowingAni.Clear();
		m_BlowingBackAni.Clear();

		m_JumpBlowingAni.Clear();
		m_JumpBlowingBackAni.Clear();

		m_DownAni.Clear();
		m_DownBackAni.Clear();
	}
} BlowWoundAniInfo;

struct WeaponAttribute
{
	int m_iAttributeIndex;

	AttackWeaponType m_WeaponType;
	BlowTargetType	 m_BlowTargetType;
	ElementType		 m_ElementType;
	AttackStatType	 m_StatType;
	CharPiercingType m_PiercingType;
	DefenseBreakType m_DefenseBreakType;
	DefenseSuccessType m_DefenseSuccessType;
	AttackMethodType m_AttackMethodType;
	EnableSideType m_EnableSideType;
	EscapeCatchType m_EscapeCatchType;

	int m_iShakeCamera;

	float m_fPushPower;
	float m_fBlowPower;
	bool  m_bNoCalcFloatingCnt;//��� ���� ���ҽ� FloatCnt�� �߰� �Ǹ鼭 BlowPower�� ���� �ش�. �װ� ���� ó�� �Ҷ� �ʿ�
	float m_fAirPushPower;
	float m_fAirBlowPower;
	float m_fDefensePushPower;
	float m_fPushAngle;

	float m_fBallPushPower;
	float m_fBallBlowPower;
	float m_fBallAirPushPower;
	float m_fBallAirBlowPower;

	float m_fGangsiPushPower;
	float m_fGangsiBlowPower;
	float m_fGangsiAirPushPower;
	float m_fGangsiAirBlowPower;

	float m_fDamageRate;
	float m_fDefenseEnableRate;
	float m_fNeedGauge;

	float m_fDownStateBlowPowerRate;
	float m_fDownStatePushPowerRate;

	float m_fBoundBlowJumpAmt;
	float m_fBoundBlowForceAmt;
	float m_fBoundBlowGravityRate;
	float m_fBoundBlowBoundDamage;

	float m_fDefenseDamageRate;
	float m_fDefenseDamageRate2;

	ioHashString m_ExtendWoundAni;
	DWORD m_dwExtendWoundDuration;
	float m_fExtendWoundJumpAmt;
	float m_fExtendWoundForceAmt;

	BlowWoundAniInfo m_BlowWoundAniInfo;

	ioHashString m_ExplosionEffect;
	ioHashString m_ExtraExplosionEffect;
	ioHashString m_WeaponEffect;
	ioHashString m_WeaponEndEffect;

	bool m_bNoExplosionEffectWithChar;

	ioHashString m_ExplosionSound;
	ioHashString m_WoundedSound;
	ioHashString m_DefensedSound;
	ioHashString m_LoopSound;

	float m_fMoveSpeed;
	float m_fMoveRange;
	float m_fFloatPower;
	DWORD m_dwWeaponFeature;
	
	float m_fMapCollisionBoxRate;
	float m_fScaleRate;

	int m_iBlockingRate;
	int m_iNpcBlockingRate;
	int m_iNpcBlockingType;
	int m_iGangsiBlockingRate;
	int m_iFCBlockingRate;

	DWORD m_dwCollisionDelay;
	DWORD m_dwPiercingLiveTime;

	DWORD m_dwRefreshDuration;
	DWORD m_dwIgnoreDuration;

	BuffInfoList m_TargetAirBuffList;
	BuffInfoList m_TargetBuffList;
	BuffInfoList m_TargetDownBuffList;

	// increase buff
	ioHashString m_DamageIncreaseBuff;
	ioHashString m_PushIncreaseBuff;
	ioHashString m_BlowIncreaseBuff;

	ioHashString m_AreaWeaponName;

	bool m_bDefenseBreakBuffEnable;
	ioBuffInfo m_DefenseBreakBuff;

	bool m_bSelectCollisionBuffEnable;
	bool m_bSelectCollisionBuffEnableReverse;

	// werewolf
	float m_fWereWolfChangeDamage;

	bool m_bEnableSylphid;
	int m_iSylphidNeed;
	WeaponExtraGaugeType m_WeaponExtraGaugeType;
	bool m_bEnablePossession;

	// for last apply weapon
	DWORD m_dwIgnoreWeaponTime;

	//�۱�� ��û( ��������Ʈ )
	DWORD m_dwTypeProtectedTime;
	DWORD m_dwProtectedType;

	// ���������� ����� �����ϸ� �� ��� �����ϱ� ���� �ɼ�
	bool m_bUseGrapplingTarget;

	// defense cancel
	ioHashString m_DefenseCancelAni;
	float m_fDefenseCancelAniRate;

	WeaponAttribute()
	{
		m_ElementType  = ET_NONE;
		m_AttackMethodType = AMT_NONE;

		m_dwWeaponFeature = 0;
		m_iShakeCamera = 0;

		m_fPushAngle = 0.0f;

		m_fMoveSpeed = 0.0f;
		m_fMoveRange = 0.0f;
		m_fFloatPower = 0.0f;
		m_dwCollisionDelay = 0;
		m_dwPiercingLiveTime = 0;

		m_dwRefreshDuration = 0;
		m_dwIgnoreDuration = 0;

		m_fMapCollisionBoxRate = FLOAT1;
		m_fScaleRate = 0.0f;

		m_fDownStateBlowPowerRate = FLOAT1;
		m_fDownStatePushPowerRate = FLOAT1;

		m_bDefenseBreakBuffEnable = false;
		m_bSelectCollisionBuffEnable = false;

		m_bEnableSylphid = false;
		m_bEnablePossession = false;

		m_WeaponExtraGaugeType = WGT_NONE;

		m_dwIgnoreWeaponTime = 0;

		m_dwTypeProtectedTime = 0;
		m_dwProtectedType = 0;

		m_fDefenseDamageRate = FLOAT1;
		m_fDefenseDamageRate2 = FLOAT1;

		m_bUseGrapplingTarget = false;

		m_bNoCalcFloatingCnt = false;

		m_fDefenseCancelAniRate = FLOAT1;
	}
};

typedef std::vector< WeaponAttribute > WeaponAttributeList;

