#pragma once

#include "ioAttackSkill.h"
#include "ioAttackAttribute.h"
#include "ioUserKeyInput.h"

class ioINILoader;
class ioBaseChar;
class ioItem;
class SP2Packet;

class ioAreaCheckBuffSkill : public ioAttackSkill
{
public:
	//��ų�� ����
	enum SkillState
	{		
		SS_NONE,
		SS_ACTION,
		SS_CHECKBUFF,
		SS_CHECKHASBUFF,
		SS_END
	};
	
	//�߰��� ����////////////////////
protected:	
	//���� �׼�
	AttackAttribute m_vBuffAction;
	//��ų ��ȣ �ð�
	DWORD m_dwSkillProtectTime;
	//���� ��ų ��ȣ �ð�
	DWORD m_dwCurSkillProtectTime;
	//��ų ���� ����
	CEncrypt<bool> m_bSetAttackFire;	
	////////////////////////////////
	
	//�߰��� ����///////////////////////////
protected:
	//Area���⸦ ������ ���ΰ�
	bool m_bIsCreateAreaWeapon;
	//Area���� ��ȣ
	ioHashString m_szAreaWeaponName;
	//Area���� �ߵ� �ð�
	DWORD m_dwAreaFireTime;
	//Area���� ���� ��ġ
	D3DXVECTOR3 m_AreaWeaponPos;
	//���� �׼� �� ����Ʈ
	ioHashString m_szBuffActionEffect;
	//ǥ�� �����̸� (Ȯ���� ������ �ϳ�!)
	ioHashString m_szCheckBuff;
	//���� �����̸� (������ ������ ������)
	BuffInfoList m_EnemyBuffList;
	BuffInfoList m_OwnerBuffList;
	//�浹 Ȯ�� Ÿ��
	TargetColType m_TargetColType;
	//�浹 �ǰ� Ÿ��
	TargetWoundType m_TargetWoundType;
	//�浹 ����
	float m_fColRange;
	//�浹 ����
	float m_fColAngle;
	//�浹 ����
	float m_fColUpHeight;
	//�浹 �Ʒ� ����
	float m_fColUnderHeight;
	//�浹 �ʺ�
	float m_fColWidth;
	//�浹 ������
	float m_fColOffSet;
	//���� �׼� �ʱ�ȭ
	bool m_bIsBuffActionInit;
	//������ �������
	bool m_bSetEnemyBuff;
	//�ִϸ��̼� ����ð��� ����� ���ΰ�
	bool m_bSetReserveTime;
	//Ű�Է� ���� �ð�
	DWORD m_dwDelayInputTime;
	//Ű�Է� ���� �ð� Ȯ��
	DWORD m_dwCheckInputTime;
	///////////////////////////////////////

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
	//���� ��ų �ߵ�
	virtual void AttackSkillFire( ioBaseChar *pChar );
	
	//�߰��� �Լ�////////////////////////////////////
protected:
	//�߰� ������ �б�
	void LoadBuffActionProperty( ioINILoader &rkLoader );
	void LoadTargetInfoProperty( ioINILoader &rkLoader );
	void LoadBuffInfoProperty( ioINILoader &rkLoader );
	//�ڽſ��� ���� ����

	//Area���� ���� Ȯ��
	void CheckCreateAreaWeapon( ioBaseChar *pChar );
	//���� Ȯ�� ������Ʈ
	void CheckBuff( ioBaseChar *pChar );
	//���� Ȯ�� �ִϸ��̼� ����
	void SetCheckBuffAnimation( ioBaseChar *pChar );
	//���� Ÿ�ٿ��� ǥ�ù����� �ִ°� Ȯ��
	void CheckTargetHasBuff( ioBaseChar *pChar );
	/////////////////////////////////////////////////

public:
	//�⺻ ������
	ioAreaCheckBuffSkill();
	//���� ������ ( Clone ���� ����)
	ioAreaCheckBuffSkill( const ioAreaCheckBuffSkill &rhs );
	//�Ҹ���
	~ioAreaCheckBuffSkill();
};

//��ų ����ȯ (�⺻ ���¿� �°� ����)
inline ioAreaCheckBuffSkill* ToAreaCheckBuffSkill( ioSkill *pSkill )
{
	ioAttackSkill *pAttack = ToAttackSkill( pSkill );
	if( !pAttack || pAttack->GetSubSkillType() != AST_AREA_CHECK_BUFF )
		return NULL;

	return dynamic_cast< ioAreaCheckBuffSkill* >(pAttack);
}

