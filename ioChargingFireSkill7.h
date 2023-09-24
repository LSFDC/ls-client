#pragma once

#include "ioAttackSkill.h"
#include "ioAttackAttribute.h"
#include "ioUserKeyInput.h"

class ioINILoader;
class ioBaseChar;
class ioItem;
class SP2Packet;

class ioChargingFireSkill7 : public ioAttackSkill
{
public:
	//��ų�� ����
	enum SkillState
	{		
		SS_NONE,
		SS_CHARGING,
		SS_CHANGEROT,
		SS_CHARGESHOT,
		SS_END
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

public:
	//�⺻ ������
	ioChargingFireSkill7();
	//���� ������ ( Clone ���� ����)
	ioChargingFireSkill7( const ioChargingFireSkill7 &rhs );
	//�Ҹ���
	~ioChargingFireSkill7();
};

//��ų ����ȯ (�⺻ ���¿� �°� ����)
inline ioChargingFireSkill7* ToChargingFireSkill7( ioSkill *pSkill )
{
	ioAttackSkill *pAttack = ToAttackSkill( pSkill );
	if( !pAttack || pAttack->GetSubSkillType() != AST_CHARGING_FIRE7 )
		return NULL;

	return dynamic_cast< ioChargingFireSkill7* >(pAttack);
}

