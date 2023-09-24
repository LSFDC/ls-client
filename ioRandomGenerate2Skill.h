#pragma once
#include "ioattackskill.h"

class ioINILoader;
class ioBaseChar;
class SP2Packet;

class ioRandomGenerate2Skill :	public ioAttackSkill
{
private:
	enum SkillState
	{
		SS_NONE,
		SS_WAIT,
		SS_ACTION,
		SS_FIRE,
		SS_END,
	};

	enum WeaponStartPositionState
	{
		WSPS_AREA,
		WSPS_PLYER,
	};

protected:
	//���� skill ����
	int m_State;
	int m_WeaponStartPositionState;

	//magic circle ���� ������
	float m_fMagicCircleOffSet;
	float m_fMagicCircleMoveSpeed;
	float m_fMagicCircleHeightGap;
	float m_fMagicCircleRange;

	//���� ��ȯ�Ǵ� ������ ������ ��
	float m_fCircleRadius;

	//magic circle move������ �ð�
	DWORD m_dwCircleDuration;
	//magic circle move ���� �ð�.
	DWORD m_dwCircleEnd;
	//magic circle ��ġ
	D3DXVECTOR3 m_vCirclePos;

	//magic circle image
	ioHashString m_ReadyCircle;
	//������
	ioHashString m_RedCircle;
	ioHashString m_BlueCircle;

	//���� info list
	GenerateList m_vGenerateList;
	//���� ������ġ�� ���� �߰�
	float m_fGenerateStartHeight;
	//���� �������۽ð�
	DWORD m_dwGenerateStartTime;
	//�������� ���� ���� �ð�
	DWORD m_dwNextGenerateTime;

	//���� ���� ���� tic Ÿ�� ���� ���� INI ����
	DWORD m_dwMinGenerateTime;
	DWORD m_dwMaxGenerateTime;

	//WeaponStartPositionState�� ���� Pos ����
	D3DXVECTOR3 m_GeneratePos;

	//weapon �� movedir
	D3DXVECTOR3 m_vWeaponMoveDir;

	UniqueObjID m_dwCurMapEffect;

	//Loop �߰�.
	ioHashString m_SkillLoopMotion;
	DWORD m_dwLoopStartTime;
	DWORD m_dwLoopDuration;
	float m_fSkillLoopMotionRate;

	DWORD m_dwFireProtectEndTime;

protected:
	DWORD	m_dwSkillProtectTime;

public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ioSkill* Clone();

public:
	virtual bool OnSkillStart( ioBaseChar *pChar );
	virtual void OnSkillEnd( ioBaseChar *pChar );
	virtual bool IsProtected( int iDefenseBreakType ) const;
	virtual void OnProcessState( ioBaseChar *pChar, ioPlayStage *pStage );

	virtual void ApplyExtraSkillInfo( ioBaseChar *pChar, ioPlayStage *pStage, SP2Packet &rkPacket );
	virtual void ApplyAttributeToWeapon( ioWeapon *pWeapon, ioPlayStage *pStage );
protected:
	virtual void AttackSkillFire( ioBaseChar *pChar );
	virtual bool IsSkillMotionEnd( DWORD dwActionStop, ioBaseChar* pOwner ) const;

protected:
	//���� weapon ����
	void GenerateWeapon( ioBaseChar *pChar );
	//�������·� ����
	void SetWaitState( ioBaseChar *pChar );
	//���� ���·� ����
	void SetActionState( ioBaseChar *pChar );
	//���� ���� ����
	void FireWeapon( ioBaseChar *pChar, ioPlayStage *pStage );
	//������ ����
	void CreateMagicCircle( ioBaseChar *pChar );
	//������ ������Ʈ
	void UpdateMagicCircle( ioBaseChar *pChar );
	//������ �������� ȸ��
	void CheckRotate( ioBaseChar *pChar );
	//Ű�Է� üũ
	void CheckInputKey( ioBaseChar *pChar );
	//������ ��ġ���
	void GetMagicCirclePos( ioBaseChar *pChar );
	//Action ����Ʈ ����
	void CreateEffect( ioBaseChar *pChar );
	//move dir ����
	void SetMoveDir();
	//Skill ���� ���·� ����
	void SetDeadState( ioBaseChar *pChar );
public:
	void OnGenerateList( ioBaseChar *pChar, SP2Packet &rkPacket );
public:
	ioRandomGenerate2Skill(void);
	ioRandomGenerate2Skill( const ioRandomGenerate2Skill &rhs );
	virtual ~ioRandomGenerate2Skill(void);
};

inline ioRandomGenerate2Skill* ToRandomGenerate2Skill( ioSkill *pSkill )
{
	ioAttackSkill *pAttack = ToAttackSkill( pSkill );
	if( !pAttack || pAttack->GetSubSkillType() != AST_RANDOM_GENERATE2 )
		return NULL;

	return dynamic_cast< ioRandomGenerate2Skill* >(pAttack);
}