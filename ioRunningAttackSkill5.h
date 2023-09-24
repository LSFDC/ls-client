#pragma once

#include "ioAttackSkill.h"
#include "ioUserKeyInput.h"

class ioINILoader;
class ioBaseChar;
class ioItem;
class SP2Packet;

class ioRunningAttackSkill5 : public ioAttackSkill
{
public:
	enum SkillState
	{
		SS_NONE,		
		SS_RUN,
		SS_ROTATE,
		SS_SWAPRUN,
		SS_END,
	};

protected:
	//��ų ����
	SkillState m_SkillState;
	//���� ����Ʈ
	BuffInfoList m_OwnerBuffList;	
	//��ų ��ȣ �ð�
	DWORD m_dwSkillProtectTime;
	//���� ��ų ��ȣ �ð�
	DWORD m_dwCurSkillProtectTime;
	//Ȯ���� ���� �̸�
	ioHashString m_szCheckBuff;
	//������ �ִϸ��̼�
	ioHashString m_szDelayAnimation;
	ioHashString m_szDelayEffect;
	DWORD m_dwDelayEffectID;
	//���� �ִϸ��̼�
	ioHashString m_szLoopAnimation;
	ioHashString m_szLoopEffect;
	DWORD m_dwLoopEffectID;
	//�޸��� ���ΰ�
	bool m_bIsRun;
	//������ �ΰ� �����ΰ�
	bool m_bSetDelayInit;
	bool m_bSetLoopInit;
	//�޸��� �ð�
	DWORD m_dwRunningStartTime;
	DWORD m_dwRunningEndTime;	
	DWORD m_dwRunningDuration;
	//�ӵ�
	CEncrypt<float> m_fRunSpeed;
	//ȸ��
	bool m_bTargetRot;
	bool m_bLeftRot;
	ioUserKeyInput::DirKeyInput m_CurDirKey;
	CEncrypt<DWORD> m_dwRotateTime;

public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ioSkill* Clone();

public:
	virtual bool OnSkillStart( ioBaseChar *pChar );
	virtual void OnProcessState( ioBaseChar *pChar, ioPlayStage *pStage );
	virtual void OnSkillEnd( ioBaseChar *pChar );

	virtual void ApplyExtraSkillInfo( ioBaseChar *pChar, ioPlayStage *pStage, SP2Packet &rkPacket );
	virtual void ApplyAttributeToWeapon( ioWeapon *pWeapon, ioPlayStage *pStage );
		
	virtual bool IsProtected( int iDefenseBreakType ) const;
	virtual bool IsSkillMotionEnd( DWORD dwActionStop, ioBaseChar* pOwner ) const;

protected:
	virtual void AttackSkillFire( ioBaseChar *pChar );

	void CheckKeyInput( ioBaseChar *pOwner );
	void ProcessRotate( ioBaseChar *pOwner );

	void SetRunState( ioBaseChar *pChar );
	void SetRunEndState( ioBaseChar *pChar );

protected:
	//���� ������ �б�
	void LoadOwnerBuffList( ioINILoader &rkLoader );
	//���� ����
	void SetOwnerBuffList( ioBaseChar *pChar, ioPlayStage* pStage );
	//���� Ȯ��
	bool CheckBuff( ioBaseChar *pChar );
	//���� ����
	void CreateWeapon( ioBaseChar *pChar );
	//������, ���� �ִϸ��̼� ����
	void SetLoopAnimation( ioBaseChar *pChar );
	void SetDelayAnimation( ioBaseChar *pChar );
	//����Ʈ ����
	void DeleteEffect();
	//����Ű Ȯ��
	bool CheckDirKey( ioUserKeyInput::DirKeyInput CurDirKey );

public:
	ioRunningAttackSkill5();
	ioRunningAttackSkill5( const ioRunningAttackSkill5 &rhs );
	virtual ~ioRunningAttackSkill5();
};

inline ioRunningAttackSkill5* ToRunningAttackSkill5( ioSkill *pSkill )
{
	ioAttackSkill *pAttack = ToAttackSkill( pSkill );
	if( !pAttack || pAttack->GetSubSkillType() != AST_RUNNING_ATTACK5 )
		return NULL;

	return dynamic_cast< ioRunningAttackSkill5* >( pAttack );
}

