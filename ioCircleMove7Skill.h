#pragma once

#include "ioNormalSkill.h"
#include "ioAttackAttribute.h"

class ioINILoader;
class ioBaseChar;
class ioItem;
class SP2Packet;

class ioCircleMove7Skill : public ioNormalSkill
{
protected:
	enum SkillState
	{
		SS_None,
		SS_Action,
		SS_Loop,
		SS_Teleport,
		SS_Attack,
		SS_End,
	};

protected:	
	ioHashString m_ReadyCircle;
	float m_fCircleRange;
	float m_fCircleMoveSpeed;
	float m_fCircleHeightGap;
	float m_fCircleOffSet;

	ioHashString m_szLoopAni;
	DWORD m_dwLoopTime;

	float m_DropZoneOffset;

	ioHashStringVec m_vCreateBuffList;
	ioHashStringVec m_vRemoveBuffList;

	DWORD m_dwSkillProtectTime;

	bool m_bIsCanDown;
	bool m_bIsCanJump;
	bool m_bIsCanJumpOnBlow;
	bool m_bIsCanWound;
	bool m_bIsCanBlowWound;

	AttackAttribute m_AttackInfo;

protected:
	SkillState m_SkillState;

	D3DXVECTOR3 m_vCirclePos;
	DWORD m_dwCheckLoopTime;
	DWORD m_dwCurSkillProtectTime;

public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ioSkill* Clone();
	
public:
	virtual bool OnSkillStart( ioBaseChar *pChar );
	virtual void OnProcessState( ioBaseChar *pChar, ioPlayStage *pStage );
	virtual void OnSkillEnd( ioBaseChar *pChar );

	virtual void ApplyExtraSkillInfo( ioBaseChar *pChar, ioPlayStage *pStage, SP2Packet &rkPacket );

	virtual bool IsNoDropState() const;
	virtual bool IsProtected( int iDefenseBreakType ) const;
	virtual bool IsSkillMotionEnd( DWORD dwActionStop, ioBaseChar* pOwner ) const;
	virtual bool IsEndState() const;
	virtual bool IsAttackEndState() const;

public:
	//��ų ��� ���� ���
	virtual bool IsCanDownSkill() const;
	virtual bool IsCanJumpingSkill() const;
	virtual bool IsCanJumpingSkillOnBlowWound() const;
	virtual bool IsCanWoundSkill() const;
	virtual bool IsCanBlowWoundSkill() const;

protected:
	void LoadBuffInfo( ioINILoader &rkLoader );

protected:
	//���� ����
	void SetActionState( ioBaseChar *pChar );
	void SetLoopState( ioBaseChar *pChar );
	void SetTeleportState( ioBaseChar *pChar );
	void SetAttackState( ioBaseChar *pChar );
	void SetEndState( ioBaseChar *pChar );

protected:
	//������ ����
	void CreateMagicCircle( ioBaseChar *pChar );
	//������ ������Ʈ
	void UpdateMagicCircle( ioBaseChar *pChar );
	//������ ��ġ���
	void GetMagicCirclePos( ioBaseChar *pChar );
	//������ �������� ȸ��
	void CheckRotate( ioBaseChar *pChar );
	//Ű�Է� Ȯ��
	void CheckInputKey( ioBaseChar *pChar );

protected:
	//���� ����
	void CreateBuff( ioBaseChar *pChar );
	//���� ����
	void RemoveBuff( ioBaseChar *pChar );

public:
	ioCircleMove7Skill();
	ioCircleMove7Skill( const ioCircleMove7Skill &rhs );
	virtual ~ioCircleMove7Skill();
};

inline ioCircleMove7Skill* ToCircleMove7Skill( ioSkill *pSkill )
{
	ioNormalSkill *pNormal = ToNormalSkill( pSkill );
	if( !pNormal || pNormal->GetSubSkillType() != NST_CIRCLE_MOVE7 )
		return NULL;

	return dynamic_cast< ioCircleMove7Skill* >( pSkill );
}