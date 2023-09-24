#pragma once

#include "ioNormalSkill.h"
#include "ioBuff.h"

class ioBaseChar;
class ioINILoader;
class SP2Packet;

class ioCircleMove6Skill : public ioNormalSkill
{
public:
	enum SkillState
	{
		SS_NONE,
		SS_LOOP,
		SS_ACTION,
		SS_END,
		SS_WAIT,
		SS_CREATE_WEAPON,
	};
	
protected:
	//������ ����
	float m_fCircleRange;
	float m_fCircleMoveSpeed;
	float m_fCircleHeightGap;
	float m_fCircleOffSet;

	//����Ʈ ����
	ioHashString m_ReadyCircle;
	ioHashString m_RedEffect;
	ioHashString m_BlueEffect;
	ioHashString m_RedGuidCircle;
	ioHashString m_BlueGuidCircle;
	//����Ʈ ���� ������
	D3DXVECTOR3 m_vEffectOffset;
	
	//���� �ð� ����
	DWORD m_dwSkillProtectTime;
	//���� ���� ����
	int m_nFireWeaponNum;
	int m_nFireWeaponResist;
	//���� ����
	ioHashString m_szLoopAni;
	DWORD m_dwLoopDuration;
	
	ioHashString m_GuideEffect;
	bool m_bEnableTeamOnlyVisible;
		
protected:
	//��ų ����
	SkillState m_SkillState;
	//���� ��ų ��ȣ �ð�
	DWORD m_dwCurSkillProtectTime;
	//���� �ð� Ȯ��
	DWORD m_dwCheckLoopTime;
	//������ ��ġ
	D3DXVECTOR3 m_vCirclePos;
	//���� �ð�
	DWORD m_dwFireStartTime;
	//���� ���� ����
	bool m_bCreateWeapon;
	DWORD m_dwGuideEffectID;
				
public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ioSkill* Clone();

public:
	virtual bool OnSkillStart( ioBaseChar *pChar );
	virtual void OnSkillEnd( ioBaseChar *pChar );	
	virtual void OnProcessState( ioBaseChar *pChar, ioPlayStage *pStage );

	virtual void ApplyExtraSkillInfo( ioBaseChar *pChar, ioPlayStage *pStage, SP2Packet &rkPacket );
	virtual void ApplyAttributeToWeapon( ioWeapon *pWeapon, ioPlayStage *pStage );

	virtual bool CheckUseItemMesh();
	virtual bool IsProtected( int iDefenseBreakType ) const;

	virtual bool IsEndState() const;
	virtual bool IsSkillMotionEnd( DWORD dwActionStop, ioBaseChar* pOwner ) const;

protected:
	virtual void SetNormalSkill( ioBaseChar *pChar );
	
protected:
	//���� ���� ���
	float GetGrowthCircleRange( ioBaseChar *pChar );
	//���� ���·� ��ȯ
	void SetLoopState( ioBaseChar *pChar );
	//������ ����
	void CreateMagicCircle( ioBaseChar *pChar );
	//������ ������Ʈ
	void UpdateMagicCircle( ioBaseChar *pChar );
	//������ �������� ȸ��
	void CheckRotate( ioBaseChar *pChar );
	//Ű�Է� Ȯ��
	void CheckInputKey( ioBaseChar *pChar );
	//���� ���·� ��ȯ
	void SetActionState( ioBaseChar *pChar );
	//������ ��ġ���
	void GetMagicCirclePos( ioBaseChar *pChar );
	//����Ʈ ����
	void CreateEffect( ioBaseChar *pChar );
	//���� ����
	void CreateWeapon( ioBaseChar *pChar );
	//���� ���� ����
	void SetEndState( ioBaseChar *pChar );
	void CreateGuideEffect( ioBaseChar *pChar );

public:
	ioCircleMove6Skill();
	ioCircleMove6Skill( const ioCircleMove6Skill &rhs );
	virtual ~ioCircleMove6Skill();
};

inline ioCircleMove6Skill* ToCircleMove6Skill( ioSkill *pSkill )
{
	ioNormalSkill *pNormal = ToNormalSkill( pSkill );
	if( !pNormal || pNormal->GetSubSkillType() != NST_CIRCLE_MOVE6 )
		return NULL;

	return dynamic_cast< ioCircleMove6Skill* >( pSkill );
}

