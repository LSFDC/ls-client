#pragma once

#include "ioAttackSkill.h"
#include "ioAttackAttribute.h"
#include "ioUserKeyInput.h"

class ioINILoader;
class ioBaseChar;
class ioItem;
class SP2Packet;

class ioChargingFireSkill8 : public ioAttackSkill
{
	struct GatheringAniInfo
	{
		ioHashString m_stAni;
		float m_fAniRate;
	};

public:
	//��ų�� ����
	enum SkillState
	{		
		SS_NONE,
		SS_CHARGING,
		SS_CHANGEROT,
		SS_CHARGESHOT,
		SS_CHARGE_GRADE_CHANGE,
		SS_END,
		SS_JUMPING,
		SS_LAND_ATTACK,
	};

	//�߰��� ����////////////////////
protected:	
	//��ų ��ȣ �ð�
	DWORD m_dwSkillProtectTime;
	//���� ��ų ��ȣ �ð�
	DWORD m_dwCurSkillProtectTime;
	//��ų ���� ����
	CEncrypt<bool> m_bSetAttackFire;
	//���� ����
	ioUserKeyInput::DirKeyInput m_CurDirKey;		
	////////////////////////////////

	typedef std::vector< GatheringAniInfo > ChargeAniInfoList;
	ChargeAniInfoList m_vChargeAniInfoList;
	//�߰��� ����///////////////////
protected:
	//���� ��ų ����Ʈ (�ܰ躰 �Ӽ�)
	AttributeList m_vChargeShotList;

	//���� ���� �ð�, �ִ� ���� �ð�, ���� �ܰ��� ���̰�
	DWORD m_dwStartChargeTime;
	DWORD m_dwMaxChargeTime;
	DWORD m_dwChargeTic;

	//���� �� ���, ������
	ioHashString m_ChargeMotionName;
	float m_fChargeAnimationTimeRate;

	//���� �ʱ�ȭ ����, ���� �ܰ� 
	bool m_bIsChargeStateInit;
	int m_nChargeGrade;
	//���� �߻� �ʱ�ȭ ����
	bool m_bIsChargeShotInit;
	//���� �Է� ����
	ioUserKeyInput::DirKeyInput m_InputDirKey;	
	//ù���� �ߵ�����
	bool m_bSetFirstAttack;
	//ȸ�� ����
	bool m_bSetRotation;
	///////////////////////////////////

	float m_fJumpPower;

	AttackAttribute m_LandAttackAttribute;
	

protected:
	//���� ��ų�� ����
	SkillState m_SkillState;

public:
	//ini ���� �б�
	virtual void LoadProperty( ioINILoader &rkLoader );
	//��ų ���� ���� �� ��ȯ
	virtual ioSkill* Clone();

public:
	//��ų ���� �κ� ����
	virtual bool OnSkillStart( ioBaseChar *pChar );
	//��ų �ߵ� �� ���� (Loop ����)
	virtual void OnProcessState( ioBaseChar *pChar, ioPlayStage *pStage );
	//��ų ���� �κ� ����
	virtual void OnSkillEnd( ioBaseChar *pChar );

	//�޽� ���� ����
	virtual bool CheckUseItemMesh();

	//���� �Ӽ� ����
	virtual void ApplyAttributeToWeapon( ioWeapon *pWeapon, ioPlayStage *pStage );
	//��ų ���� �߰� ���� (��Ŷ�� �޾� ĳ������ ���¸� ó��, �̵�, ȸ��, Ư�� ��� ����ȭ)
	virtual void ApplyExtraSkillInfo( ioBaseChar *pChar, ioPlayStage *pStage, SP2Packet &rkPacket );

	//��ų ���� ����� �����Ѱ�
	virtual bool IsSkillMotionEnd( DWORD dwActionStop, ioBaseChar* pOwner ) const;
	//��ų ���� ���� ���¿� ���� ó��
	virtual bool IsProtected( int iDefenseBreakType ) const;
	//���� �����Ѱ�
	virtual bool IsEnableReserve() const;
	//��ų�� ���� �����ΰ�
	virtual bool IsJumpState() const;
	//��ų�� ���ִ� �����ΰ�
	virtual bool IsNoDropState() const;
	//��Ŷ (��ġ�� ����) �� ���� �� �ִ� �����ΰ�
	virtual bool IsCanSendControl() const;
	//�뽬 Ȯ���� �����Ѱ�
	virtual bool IsEnableDashCheck( ioBaseChar *pChar );
	//��ų�� ���� �����ΰ�
	virtual bool IsEndState() const;
	//���� �Ϸ� ���� (���� ��ų ��� �� ���� ����) 
	virtual bool IsAttackEndState() const;


protected:
	virtual void AttackSkillFire( ioBaseChar *pChar );
	void ChangeGradeMotion( ioBaseChar *pChar, int nGrade );

	//�߰��� �Լ�///////////////////////
protected:
	//���� ������ �б�
	void LoadChargeProperty( ioINILoader &rkLoader );
	//���� �ݺ� �ִϸ��̼� ����
	void SetChargeLoopAnimation( ioBaseChar *pChar );
	//���� �����϶� ĳ���� ȸ��
	void SetCharacterRot( ioBaseChar *pChar );
	//���� �ܰ� Ȯ��
	void CheckChargeShotState( ioBaseChar *pChar );
	//���� �߻� 
	void ActionChargeShot( ioBaseChar *pChar );
	///////////////////////////////////

	bool CheckLanding( ioBaseChar *pChar );
	void SetLandAttack( ioBaseChar *pChar );
	void SetJumpAction( ioBaseChar *pChar );

public:
	//�⺻ ������
	ioChargingFireSkill8();
	//���� ������ ( Clone ���� ����)
	ioChargingFireSkill8( const ioChargingFireSkill8 &rhs );
	//�Ҹ���
	~ioChargingFireSkill8();
};

//��ų ����ȯ (�⺻ ���¿� �°� ����)
inline ioChargingFireSkill8* ToChargingFireSkill8( ioSkill *pSkill )
{
	ioAttackSkill *pAttack = ToAttackSkill( pSkill );
	if( !pAttack || pAttack->GetSubSkillType() != AST_CHARGING_FIRE8 )
		return NULL;

	return dynamic_cast< ioChargingFireSkill8* >(pAttack);
}

