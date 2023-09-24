#pragma once

#include "ioAttackSkill.h"
#include "ioAttackAttribute.h"

class ioINILoader;
class ioBaseChar;
class SP2Packet;

class ioProtectCountAreaAttackSkill : public ioAttackSkill
{
public:
	//��ų�� ����
	enum SkillState
	{		
		SS_NONE,
		SS_PROTECT,
		SS_AREA_ATTACK,
		SS_END
	};
	
protected:	
	//��ų ��ȣ �ð�
	DWORD m_dwSkillProtectTime;
	//���� ��ų ��ȣ �ð�
	DWORD m_dwCurSkillProtectTime;
	
protected:
	//��� �ð�
	DWORD m_dwProtectTime;
	//��� �ð� Ȯ��
	DWORD m_dwCheckProtectTime;
	//�ٶ󺸱� ���� �̸�
	ioHashString m_szLookBuffName;
	ioHashString m_szAttackerName;
	//���� ���� �̸�
	ioHashString m_szAreaWeaponName;
	//���� �ִϸ��̼� �̸�
	ioHashString m_szAreaAttackAnimation;
	//���� �ִϸ��̼� ����
	float m_fAreaAttackAnimationRate;
	//���� ����Ʈ �̸�
	ioHashString m_szAreaAttackEffect;
	//���� ������ �ð�
	DWORD m_dwAreaAttackDelayTime;
	//���� ���� ���� �ð�
	DWORD m_dwAreaFireTime;
	//�ִϸ��̼��� �ʱ�ȭ �Ǿ���
	bool m_bIsAreaAttackInit;
	//�ѹ� ������ �Ͽ���
	bool m_bIsAreaCreateWeapon;
	//���� ������ �ð��� ���� �� ���ΰ�
	bool m_bSetAreaFireDelayTime;
	//���� ���� ������ �ð�
	DWORD m_dwAreaFireDelayTime;
	//���� �ִ� ���� ����
	bool m_bSetNoWound;
	//���� ���� �ð�
	DWORD m_dwNoWoundTime;
	DWORD m_dwCheckNoWoundTime;
	BuffInfoList m_OwnerBuffList;

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
	//������ �б�
	void LoadProtectCountProperty( ioINILoader &rkLoader );
	void LoadBuffInfoProperty( ioINILoader &rkLoader );
	///////////////////////////////////

protected:
	//���� �ִϸ��̼� ����
	void SetAreaAttackAnimation( ioBaseChar *pChar );
	//���� ���� ���� Ȯ��
	void CheckCreateAreaWeapon( ioBaseChar *pChar );
	//���� ����
	void AddLookBuff( ioBaseChar *pChar );
	//���� ���� ����ȭ
	void SendAreaAttackState( ioBaseChar *pChar );

public:
	//��ų ���°� ��� �����ΰ�
	bool IsProtectState() const;
	//���� ����
	void SetAreaAttackState( ioHashString szAttacker );
	
public:
	//�⺻ ������
	ioProtectCountAreaAttackSkill();
	//���� ������ ( Clone ���� ����)
	ioProtectCountAreaAttackSkill( const ioProtectCountAreaAttackSkill &rhs );
	//�Ҹ���
	~ioProtectCountAreaAttackSkill();
};

//��ų ����ȯ (�⺻ ���¿� �°� ����)
inline ioProtectCountAreaAttackSkill* ToProtectCountAreaAttackSkill( ioSkill *pSkill )
{
	ioAttackSkill *pAttack = ToAttackSkill( pSkill );
	if( !pAttack || pAttack->GetSubSkillType() != AST_PROTECT_COUNT_AREA_ATTACK )
		return NULL;

	return dynamic_cast< ioProtectCountAreaAttackSkill* >(pAttack);
}

