#pragma once
#include "ioattackskill.h"
class ioJumpAreaAttackSkill :public ioAttackSkill
{
private:
	enum SkillState
	{
		SS_NONE,
		SS_PREACTION,
		SS_PREATTACACTION,
		SS_LOOP,
		SS_ACTION,
		SS_ENDACTION,
		SS_END,
	};

	enum Sync
	{
		S_DIRKEY,
		S_ATTACKKEY,
	};

protected:
	//���� ��ų ����
	SkillState m_SkillState;

	//�����ϴ� ��ų
	float m_fJumpAngle;
	float m_fJumpPower;

	//pre action �߰�
	ioHashString m_szPreActionName;
	ioHashString m_szPreActionEffect;
	DWORD m_dwPreActionEndTime;
	float m_fPreActionRate;
		
	//pre attack �߰�
	DWORD		 m_dwPreAttackStartTime;
	DWORD		 m_dwPreAttackEndTime;
	DWORD		 m_dwPreAttackProtectTime;
	DWORD		 m_dwPreAttackKeyReserveTime;
	bool		 m_bKeyInput;
	
	//loop �߰�.
	ioHashString m_szSkillLoopMotion;
	ioHashString m_szSkillLoopEffect;
	DWORD m_dwLoopStartTime;
	//loop�ð�. (Ű �Է��� ������ �� weapon ������ 0�� �ƴ϶�� m_dwLoopDuration�ð� ��ŭ loop�� �� �� ��ų ����.
	DWORD m_dwLoopDuration;
	float m_fSkillLoopMotionRate;
	DWORD m_dwSkillLoopProtectTime;

	//Attack Motion �߰�
	ioHashString m_szAttackMotionName;
	float		 m_fAttackMotionRate;

	//Weapon ����.
	D3DXVECTOR3 m_vWeaponOffset;
	bool m_bFirstWeaponCall;	//ini ���ð�

	//KeyInput����
	DWORD m_dwKeyInputLimitTime;
	DWORD m_dwCreatedWeaponTime;
	DWORD m_dwCreateWeaponTicTime;
	//���� �ɸ��� ����
	ioUserKeyInput::DirKeyInput m_CurDirKey;
public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ioSkill* Clone();
public:
	virtual bool OnSkillStart( ioBaseChar *pChar );
	virtual void OnProcessState( ioBaseChar *pChar, ioPlayStage *pStage );
	virtual void OnSkillEnd( ioBaseChar *pChar );
	virtual bool IsSkillMotionEnd( DWORD dwActionStop, ioBaseChar* pOwner ) const;
	virtual bool IsProtected( int iDefenseBreakType ) const;
	virtual bool IsNoDropState() const;
	virtual bool IsEndState() const;
	virtual bool IsAttackEndState() const;
	virtual void ApplyExtraSkillInfo( ioBaseChar *pChar, ioPlayStage *pStage, SP2Packet &rkPacket );
	virtual void ApplyAttributeToWeapon( ioWeapon *pWeapon, ioPlayStage *pStage );
protected:
	void WeaponFire( ioBaseChar *pChar, ioPlayStage *pStage );
protected:
	//ĳ���͸� jump��Ų��.
	void SetCharJumping( ioBaseChar *pChar );
	// loop�� ������ ���¸� üũ�Ѵ�.
	bool CheckLoopEnd( ioBaseChar *pChar );
	//PreAction���·� ����
	void SetPreActionState( ioBaseChar *pChar );
	//PreAttackAction ���·� ����
	void SetPreAttackAction( ioBaseChar *pChar );
	//Loop ���·� ����
	void SetLoopState( ioBaseChar *pChar );
	//ActionEnd���·� ���� (���� ������ Action�� �ִٸ�) ���ٸ� �ٷ� SetEndState ȣ��
	void SetActionEndState( ioBaseChar *pChar );
	//��ų ���� ����
	void SetEndState( ioBaseChar *pChar );
	//Weapon ����
	void CreateWeapon( ioBaseChar *pChar, ioPlayStage *pStage );
	void CreateWeaponByAni( ioBaseChar *pChar, int iAniID, float fTimeRate, DWORD dwPreDelay );
	void CheckKeyInput( ioBaseChar *pChar, ioPlayStage *pStage );
	void CheckDirKeyInput( ioBaseChar *pChar );
	void SetFirstWeaponCall( ioBaseChar *pChar, int iAniID, float fTimeRate, DWORD dwPreDelay );
	void ClearData();
public:
	ioJumpAreaAttackSkill(void);
	ioJumpAreaAttackSkill( const ioJumpAreaAttackSkill &rhs );
	virtual ~ioJumpAreaAttackSkill(void);
};

inline ioJumpAreaAttackSkill* ToJumpAreaAttackSkill( ioSkill *pSkill )
{
	ioAttackSkill *pAttack = ToAttackSkill( pSkill );
	if( !pAttack || pAttack->GetSubSkillType() != AST_JUMP_AREA_ATTACK )
		return NULL;

	return dynamic_cast< ioJumpAreaAttackSkill* >(pAttack);
}