#pragma once

#include "ioCountSkill.h"
#include "ioAttackAttribute.h"

class ioINILoader;
class ioBaseChar;
class ioItem;
class SP2Packet;

class ioCountLightSkill : public ioCountSkill
{
protected:
	enum SkillState
	{
		SS_None,
		SS_Action,
		SS_Loop,
		SS_Teleport,
		SS_Effect,
		SS_End,
	};

	enum UseEnable 
	{
		UE_DOWN,
		UE_JUMP,
		UE_JUMP_BLOW,
		UE_WOUND,
		UE_BLOW,
		UE_MAX,
	};

	struct EnableInfo
	{
		bool bUseEnable[UE_MAX];
		int nMinGauge;
		int nMaxGuage;
		ioHashString szEffect;
		float fDuration;
		float fRange;
		float fSpeed;
	};

	typedef std::vector< EnableInfo > InfoList;

protected:
	//���� ����Ʈ
	InfoList m_InfoList;
	
	//������ ����
	float m_fCircleRange;
	float m_fCircleMoveSpeed;
	float m_fCircleHeightGap;
	float m_fCircleOffSet;

	//����Ʈ ����
	ioHashString m_ReadyCircle;

	//���� �ִ� ����
	ioHashString m_szLoopAni;

	//�����ð�
	DWORD m_dwLoopTime;

	//����� ������
	float m_DropZoneOffset;

	//���� ����Ʈ
	ioHashStringVec m_vCreateBuffList;
	ioHashStringVec m_vRemoveBuffList;

	//��ȣ �ð�
	DWORD m_dwSkillProtectTime;

protected:
	//��ų ����
	SkillState m_SkillState;

	//������ ��ġ
	D3DXVECTOR3 m_vCirclePos;
	
	//�����ð� Ȯ��
	DWORD m_dwCheckLoopTime;

	//����� Ȯ��
	bool m_bIsDropZone;

	//��ȣ�ð� Ȯ��
	DWORD m_dwCurSkillProtectTime;

	UniqueObjID m_dwEffectID;
	int m_nGrade;

public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ioSkill* Clone();

public:
	virtual bool IsCanUseSkill( ioBaseChar *pChar, bool bHelpMsg );

public:
	virtual bool OnSkillStart( ioBaseChar *pChar );
	virtual void OnProcessState( ioBaseChar *pChar, ioPlayStage *pStage );
	virtual void OnSkillEnd( ioBaseChar *pChar );

	virtual void ApplyExtraSkillInfo( ioBaseChar *pChar, ioPlayStage *pStage, SP2Packet &rkPacket );
	virtual void ApplyEquipSlotSkillExtraInfo( ioBaseChar *pChar, ioPlayStage *pStage, SP2Packet &rkPacket );

	virtual bool IsNoDropState() const;
	virtual bool IsProtected( int iDefenseBreakType ) const;
	virtual bool IsSkillMotionEnd( DWORD dwActionStop, ioBaseChar* pOwner ) const;
	virtual bool IsEndState() const;
	virtual void RecoveryCount( ioBaseChar *pChar );
	virtual void OnSkillItemRelease( ioBaseChar *pChar );

public:
	//��ų ��� ���� ���
	virtual bool IsCanDownSkill() const;
	virtual bool IsCanJumpingSkill() const;
	virtual bool IsCanJumpingSkillOnBlowWound() const;
	virtual bool IsCanWoundSkill() const;
	virtual bool IsCanBlowWoundSkill() const;
	virtual int  GetCurUseCount();
	virtual int  GetMaxUseCount();
	virtual int  GetNeedCount();

protected:
	void LoadEnableInfo( ioINILoader &rkLoader );
	void LoadBuffInfo( ioINILoader &rkLoader );
	bool FindUseEnable( UseEnable eType ) const;
	void SetSkillInfo( int nGrade );

protected:
	//���� ����
	void SetActionState( ioBaseChar *pChar );
	void SetLoopState( ioBaseChar *pChar );
	void SetTeleportState( ioBaseChar *pChar );
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
	//����Ʈ ����
	void SetEffect( ioBaseChar *pOwner, ioHashString szEffectName );

public:
	ioCountLightSkill();
	ioCountLightSkill( const ioCountLightSkill &rhs );
	virtual ~ioCountLightSkill();
};

inline ioCountLightSkill* ToCountLightSkill( ioSkill *pSkill )
{
	ioCountSkill *pRange = ToCountSkill( pSkill );
	if( !pRange || pRange->GetSubSkillType() != CST_LIGHT )
		return NULL;

	return dynamic_cast< ioCountLightSkill* >(pRange);
}