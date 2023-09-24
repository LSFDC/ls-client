#pragma once

#include "ioAttackSkill.h"
#include "ioAttackAttribute.h"

class ioINILoader;
class ioPlayStage;
class ioBaseChar;
class ioItem;
class SP2Packet;

class ioControlDummySkill2 : public ioAttackSkill
{
public:
	enum SkillState
	{
		SS_NONE,
		SS_ACTION,
		SS_LOOP,
		SS_CREATE_DUMMY,
		SS_REMOVE_DUMMY,
		SS_END,
	};

protected:
	//��ų ����
	SkillState m_SkillState;
	//���� �ð�
	CEncrypt<DWORD> m_dwSkillProtectTime;
	//���� �̸�
	ioHashString m_DummyCharName;
	//���� �ε���
	int	m_iDummyIndex;		
	//���� ������
	float	m_fDummyCharOffset;
	//���� ���� ����Ʈ
	ioHashStringVec m_vDestroyDummyCharList;
	//���� �ð�
	DWORD m_dwCheckActiveTime;
	DWORD m_dwActiveTime;
	//���� �ִϸ��̼�, �ʱ�ȭ
	ioHashString m_szLoopAnimation;
	bool m_bLoopInit;

public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ioSkill* Clone();

public:
	virtual bool OnSkillStart( ioBaseChar *pChar );
	virtual void OnProcessState( ioBaseChar *pChar, ioPlayStage *pStage );
	virtual void OnSkillEnd( ioBaseChar *pChar );

	virtual bool CheckUseItemMesh();

	virtual void ApplyExtraSkillInfo( ioBaseChar *pChar, ioPlayStage *pStage, SP2Packet &rkPacket );
	virtual void ApplyAttributeToWeapon( ioWeapon *pWeapon, ioPlayStage *pStage );

	virtual bool IsProtected( int iDefenseBreakType ) const;
	virtual bool IsSkillMotionEnd( DWORD dwActionStop, ioBaseChar* pOwner ) const;

protected:
	virtual void AttackSkillFire( ioBaseChar *pChar );

public:
	bool IsDummyCharControl( ioBaseChar *pChar );
	bool IsDummyCharEnable( ioBaseChar *pChar );
	D3DXVECTOR3 GetDummyCharControlPos( ioBaseChar *pChar );

protected:
	void CreateDummyChar( ioBaseChar *pChar, ioPlayStage *pStage );
	void LookAtDummy( ioBaseChar *pChar );
	void CheckInputKey( ioBaseChar *pChar );
	void RemoveDummy( ioBaseChar *pChar );
	void SetLoopAnimation( ioBaseChar *pChar );

public:
	void SetSkillEndState( ioBaseChar *pChar );
	
public:
	ioControlDummySkill2();
	ioControlDummySkill2( const ioControlDummySkill2 &rhs );
	virtual ~ioControlDummySkill2();
};

inline ioControlDummySkill2* ToControlDummySkill2( ioSkill *pSkill )
{
	ioAttackSkill *pAttack = ToAttackSkill( pSkill );
	if( !pAttack || pAttack->GetSubSkillType() != AST_CONTROL_DUMMY2 )
		return NULL;

	return dynamic_cast< ioControlDummySkill2* >(pAttack);
}

