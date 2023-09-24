#pragma once

#include "ioNormalSkill.h"

class ioBaseChar;
class ioINILoader;
class SP2Packet;

class ioCircleMoveCreateAreaWeaponSkill : public ioNormalSkill
{
public:
	enum SkillState
	{
		SS_NONE,
		SS_LOOP,
		SS_ACTION,
		SS_END,
		SS_CREATE_WEAPON,
	};
	
protected:
	DWORD m_dwSkillProtectTime;

	float m_fGaugePerTic;
	DWORD m_dwTicTime;

	float m_fCircleRange;
	float m_fCircleMoveSpeed;
	float m_fCircleHeightGap;
	float m_fCircleOffSet;
	ioHashString m_ReadyCircle;
	
	ioHashString m_szLoopAni;

	int m_nAreaWeaponMaxCnt;
	ioHashStringVec m_szAreaWeaponName;
	ioHashStringVec m_OwnerBuffList;

protected:
	SkillState m_SkillState;
	DWORD m_dwCurSkillProtectTime;
	DWORD m_dwCheckGaugeTime;

	D3DXVECTOR3 m_vCirclePos;
	DWORD m_dwFireStartTime;
	int m_nAreaWeaponCurCnt;

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
	void LoadAreaWeaponList( ioINILoader &rkLoader );
	void Init();

	//���� ���� ���
	float GetGrowthCircleRange( ioBaseChar *pChar );
	//������ ����
	void CreateMagicCircle( ioBaseChar *pChar );
	//������ ������Ʈ
	void UpdateMagicCircle( ioBaseChar *pChar );
	//������ ��ġ���
	void GetMagicCirclePos( ioBaseChar *pChar );

protected:
	//���� ���·� ��ȯ
	void SetLoopState( ioBaseChar *pChar );
	//���� ���·� ��ȯ
	void SetActionState( ioBaseChar *pChar );
	//���� ���� ����
	void SetEndState( ioBaseChar *pChar );

protected:
	//Ű�Է� Ȯ��
	void CheckInputKey( ioBaseChar *pChar );
	//������ �������� ȸ��
	void CheckRotate( ioBaseChar *pChar );

protected:	
	void CreateWeapon( ioBaseChar *pChar );

protected:
	void LoadOwnerBuffList( ioINILoader &rkLoader );
	void SetOwnerBuffList( ioBaseChar *pOwner );

public:
	ioCircleMoveCreateAreaWeaponSkill();
	ioCircleMoveCreateAreaWeaponSkill( const ioCircleMoveCreateAreaWeaponSkill &rhs );
	virtual ~ioCircleMoveCreateAreaWeaponSkill();
};

inline ioCircleMoveCreateAreaWeaponSkill* ToCircleMoveCreateAreaWeaponSkill( ioSkill *pSkill )
{
	ioNormalSkill *pNormal = ToNormalSkill( pSkill );
	if( !pNormal || pNormal->GetSubSkillType() != NST_CIRCLE_MOVE_AREA_WEAPON )
		return NULL;

	return static_cast< ioCircleMoveCreateAreaWeaponSkill* >( pSkill );
}

