#pragma once


#include "ioNormalSkill.h"
#include "ioAttackAttribute.h"
#include "ioUserKeyInput.h"

class ioBaseChar;
class ioINILoader;
class SP2Packet;

class ioCircleMoveWeapon5Skill : public ioNormalSkill
{
public:
	enum SkillState
	{
		SS_NONE,//ó�� ���� ��� ����
		SS_PREACTION,//�ö󰡰� �ִ� ����
		SS_ACTION,//���� ���� ����
		SS_LOOP,//������ ������ �ϴ� ����
		SS_END,
		SS_CREATE_WEAPON,
		SS_CREATE_AREA_WEAPON,
	};

	enum
	{
		SST_RUN,
		SST_ROTATE,
	};

	//�߰� yong jun
	//���� ����
	enum WeaponFireCountType
	{
		WFCS_MULTI,  //������ : �����̼� �ð��� ���� ��ų ���� �� �� ������ ���� ��ų ����
		WFCS_SINGLE,	//�ܹ� : �����̼� �ð��� ���� ��ų ����
		WFCS_INPUT, //Ű�Է� ����
	};
	//

protected:
	CEncrypt<int> m_State;

	// Attack
	DWORD	m_dwFireStartTime;
	DWORD	m_dwNextFireStartTime;
	DWORD   m_dwNextFireStartTime_Second;
	int		m_iCurFireCount;

	DWORD m_dwFireGapTime;
	DWORD m_dwFireGapTime_Second;
	CEncrypt<float>	m_fFireHightOffset;
	CEncrypt<float>	m_fFireFrontOffset;
	int	m_iRandomRange;
	int m_iRandomRange_second;
	CEncrypt<int>	m_iTotalFireCount;

	AttackAttribute m_AttackAttribute;
	AttackAttribute m_AttackAttribute_Second;
	AttackAttribute m_EndAttackAttribute;

	// Loop Motion
	ioHashString	m_szSkillLoopMotion;
	CEncrypt<float>	m_fSkillLoopAniRate;
	CEncrypt<DWORD> m_dwSkillLoopDuration;
	CEncrypt<DWORD> m_dwSkillLoopProtectTime;
	DWORD			m_dwSkillLoopStartTime;

	// Circle
	float m_fCircleRange;
	float m_fCircleMoveSpeed;
	float m_fCircleHeightGap;
	float m_fCircleOffSet;
	float m_fRevisionRate;
	float m_fCircleStartOffSet;
	float m_fCirMaxRange;

	D3DXVECTOR3 m_vCirclePos;

	ioHashString m_ReadyCircle;
	ioHashString m_TargetCircleEffect;

	ioHashString m_RedEffect;
	ioHashString m_BlueEffect;

	UniqueObjID m_dwCurMapEffect;
	UniqueObjID m_dwTargetCircleEffect;

	ioHashString m_StartEffect;
	ioHashString m_EndEffect;

	CEncrypt<DWORD>	m_dwRotateTime;
	bool m_bTargetRot;
	bool m_bLeftRot;

	ioUserKeyInput::DirKeyInput	m_CurDirKey;

	// Weapon
	int m_iCurWeaponCnt;
	CEncrypt<float> m_fWeaponGravityRate;
	CEncrypt<float> m_fWeaponFloatPower;
	CEncrypt<float> m_fWeaponFloatMinPower;
	CEncrypt<float> m_fWeaponMinSpeed;
	CEncrypt<float> m_fWeaponGapDistance;

	WeaponMoveInfo m_vWeaponMoveInfo;
	WeaponMoveInfo m_vWeaponMoveInfo_second;

	float m_fUpLimitHeight;
	float m_fUpLimitRange;

	float m_fFirstTargetDistance;
	float m_fCircleSlowDownSpeedRate;

	// ��ų ������ ����
	CEncrypt<bool> m_bDisableGrowth;
	CEncrypt<float> m_fGuagePerTic;
	CEncrypt<DWORD> m_dwTicTime;

	DWORD m_dwCheckTime;
	CEncrypt<float> m_fTicGaugeRateByGrowth;
	CEncrypt<DWORD> m_dwTicTimeByGrowth;

	//yunga add
	bool				m_bSkillInJump;

	float				m_fJumpPowerInJump;
	float				m_fCharJumpPower;

	//�Ϲ������� ����
	float				m_fPreActionAniRate_Normal;
	ioHashString		m_strPreActionAni_Normal;

	float				m_fActionAniRate_Normal;
	ioHashString		m_strActionAni_Normal;

	//�����߿� ����
	float				m_fPreActionAniRate_Jump;
	ioHashString		m_strPreActionAni_Jump;

	float				m_fActionAniRate_Jump;
	ioHashString		m_strActionAni_Jump;
	//////////////////////////////////////////////////////////////////////////
	DWORD				m_dwPreActionEndTime;

	float				m_fJumpAngle;
	float				m_fJumpAngleInJump;
	float				m_fBulletSpeed;
	//////////////////////////////////////////////////////////////////////////
	float				m_fPreActionAniRate;
	ioHashString		m_strPreActionAni;

	float				m_fActionAniRate;
	ioHashString		m_strActionAni;
	//////////////////////////////////////////////////////////////////////////

	//�߰� yong jun
	WeaponFireCountType m_WeaponFireCountType;
	//magic circle ���� ���� ��ȯ.
	DWORD	m_dwMagicCircleNextFireStartTime;
	DWORD   m_dwMagicCircleNextFireStartTime2;

	WeaponInfoList m_MagicCircleWeaponInfoList;
	ioHashString	m_szSkillZoneEffect;
	D3DXVECTOR3		m_vZoneEffectOffset;
	DWORD m_dwSingleWeaponIdx;
	//magic circle�� ���� ���� �߰�
	DWORD m_dwMagicCircleFireGapTime;
	DWORD m_dwMagicCircleFireGapTime2;
	DWORD m_dwCircleWeaponIndex;
	bool  m_bCreateSingleWeapon;
	bool m_bNotUsePreActionAni;
	//
	DWORD m_dwRepeatTime;
	DWORD m_dwCheckRepeatTime;

	ioHashString m_szAreaWeapon;
	D3DXVECTOR3 m_vAreaWeaponOffset;
	bool m_bSetCreateAreaWeaponState;
	AttackAttribute m_AreaWeaponAttribute;
	AttackAttribute m_JumpAreaWeaponAttribute;
	DWORD m_dwCheckInputTime;
	DWORD m_dwInputTime;

	//����ũ ���( ��Ʈ���̴� �Ƹ� )
	ioHashString m_SkillActionLoopUpMotion;
	ioHashString m_SkillActionLoopCenterMotion;
	ioHashString m_SkillActionLoopDownMotion;

protected:
	void ClearData();

public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ioSkill* Clone();

public:
	virtual bool OnSkillStart( ioBaseChar *pChar );
	virtual void OnSkillEnd( ioBaseChar *pChar );	
	virtual void OnProcessState( ioBaseChar *pChar, ioPlayStage *pStage );

	virtual void ApplyAttributeToWeapon( ioWeapon *pWeapon, ioPlayStage *pStage );
	virtual void ApplyExtraSkillInfo( ioBaseChar *pChar, ioPlayStage *pStage, SP2Packet &rkPacket );

	virtual bool CheckUseItemMesh();
	virtual bool IsProtected( int iDefenseBreakType ) const;
	virtual bool IsSkillMotionEnd( DWORD dwActionStop, ioBaseChar* pOwner ) const;	

	virtual bool IsEndState() const;
	virtual bool IsNoDropState() const;
	virtual bool IsAttackEndState() const;

protected:
	void SetPreAction( ioBaseChar *pChar );
	void CheckMotion( ioBaseChar* pChar );

protected:
	virtual bool StartPreDelayAnimation( ioBaseChar *pChar );
	virtual void SetNormalSkill( ioBaseChar *pChar );

protected:
	void SetLoopState( ioBaseChar *pChar, ioPlayStage *pStage );
	void SetEndState( ioBaseChar *pChar );

	void OnLoopState( ioBaseChar *pChar, ioPlayStage *pStage );
	//void ProcessRotate( ioBaseChar *pChar );
	void UpdateCircle( ioBaseChar *pOwner );

	void AttackFire( ioBaseChar *pChar );

	//�߰� yong jun
	void LoadMagicCircleWeaponInfo( ioINILoader &rkLoader );
	//������ �������� ȸ��
	void CheckRotate( ioBaseChar *pChar );
	//���������� ���� ��ȯ
	void MagicCircleFire( ioBaseChar *pChar );
	//���� ����( ���Ⱑ �ϳ��Ͻ�)
	void RemoveWeapon( ioBaseChar *pChar );
	//������ ������ ��ġ�� �̵������ش�.
	void CheckWeapon( ioBaseChar *pChar );
	void AttackSingleFire( ioBaseChar *pChar );
	//

protected:
	float GetFlightDuration( float fCurGravity, float fCurWeaponFloatPower, float fGapHeight );
	float GetTargetTime( IN float fCurGravity, IN float fGapHeight, OUT float &fFloatPower );

	void CheckWeaponFloatInfo( ioBaseChar *pOwner );
	void CheckWeaponFloatInfo_Second( ioBaseChar *pOwner );

public:
	void GenerateEndAttack( const D3DXVECTOR3 &vPos, ioBaseChar *pOwner );

protected:
	bool CheckInputAttack( ioBaseChar *pOwner );
	void CreateWeapon( ioBaseChar *pOwner );
	void SetEndStatePacket( ioBaseChar *pOwner );
	void CreateAreaWeapon( ioBaseChar *pOwner, ioPlayStage *pStage );
	void SetCreateAreaWeaponState( ioBaseChar *pOwner, const AttackAttribute& Attribute );

public:
	ioCircleMoveWeapon5Skill();
	ioCircleMoveWeapon5Skill( const ioCircleMoveWeapon5Skill &rhs );
	virtual ~ioCircleMoveWeapon5Skill();
};

inline ioCircleMoveWeapon5Skill* ToCircleMoveWeapon5Skill( ioSkill *pSkill )
{
	ioNormalSkill *pNormal = ToNormalSkill( pSkill );
	if( !pNormal || pNormal->GetSubSkillType() != NST_CIRCLE_MOVE_WEAPON5 )
		return NULL;

	return dynamic_cast< ioCircleMoveWeapon5Skill* >( pSkill );
}

