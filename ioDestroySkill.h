#pragma once

#include "ioNormalSkill.h"

class ioBaseChar;
class ioINILoader;
class SP2Packet;

class ioDestroySkill : public ioNormalSkill
{
public:
	enum SkillState
	{
		SS_NONE,
		SS_LOOP,
		SS_CHARGE,
		SS_SUCCESS,
		SS_END,
	};
	
protected:	
	//���� �ð� ����
	DWORD m_dwSkillProtectTime;

	//���� ����
	ioHashString m_szLoopAni;
	DWORD m_dwLoopDuration;

	//���� �ð�
	DWORD m_dwChargeTime;

	//Ÿ�� ����, ����
	float m_fColRange;
	float m_fColAngle;

	//����, ���� ����Ʈ
	ioHashString m_szStartEffect;
	ioHashString m_szSuccessEffect;
	ioHashString m_szFailEffect;
	ioHashString m_szFindEffect;
	//����Ʈ ������
	D3DXVECTOR3 m_vEffectStartOffset;
	D3DXVECTOR3 m_vEffectSuccessOffset;
	D3DXVECTOR3 m_vEffectFailOffset;
	D3DXVECTOR3 m_vEffectFindOffset;
		
	ioHashStringVec m_vPassDummyList;

protected:
	//��ų ����
	SkillState m_SkillState;
	//���� ��ų ��ȣ �ð�
	DWORD m_dwCurSkillProtectTime;
	//���� �ð� Ȯ��
	DWORD m_dwCheckLoopTime;
	//���� �ð� Ȯ��
	DWORD m_dwCheckChargeTime;
	//���� ����
	bool m_bSuccess;

	DWORD m_dwStartEffectID;
	DWORD m_dwFindEffectID;
				
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
	//���� ���·� ��ȯ
	void SetLoopState( ioBaseChar *pChar );
	//���� ���·� ��ȯ
	void SetChargeState( ioBaseChar *pChar );
	//���� ���·� ��ȯ
	void SetSuccessState( ioBaseChar *pChar );
	//���� ���� ����
	void SetEndState( ioBaseChar *pChar );

protected:
	//Ű�Է� Ȯ��
	void CheckInputKey( ioBaseChar *pChar );
	void CheckChargeKey( ioBaseChar *pChar );

protected:
	//���� ã��
	void FindTargetWeapon( ioBaseChar *pChar );
	//���� ����
	void SetMineExplosion( ioBaseChar *pChar, ioPlayStage *pStage, TargetWeaponList &vWeaponList );
	//���� ã��
	void FindTargetDummy( ioBaseChar *pChar );
	//���� ����
	void SetDummyDestroy( ioBaseChar *pChar, TargetDummyList &vDummyList );
	bool IsPassDummy( ioDummyChar *pDummy );

protected:
	//����Ʈ ����
	void CreateActionEffect( ioBaseChar *pChar, ioHashString szEffect, D3DXVECTOR3 vOffset );
	void CreateLoopEffect( ioBaseChar *pChar, ioHashString szEffect, D3DXVECTOR3 vOffset, DWORD &dwID );
	void DestroyEffect( ioBaseChar *pChar, DWORD &dwID );
	
public:
	ioDestroySkill();
	ioDestroySkill( const ioDestroySkill &rhs );
	virtual ~ioDestroySkill();
};

inline ioDestroySkill* ToDestroySkill( ioSkill *pSkill )
{
	ioNormalSkill *pNormal = ToNormalSkill( pSkill );
	if( !pNormal || pNormal->GetSubSkillType() != NST_DESTROY )
		return NULL;

	return dynamic_cast< ioDestroySkill* >( pSkill );
}

