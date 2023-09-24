#pragma once

#include "ioCountSkill.h"
#include "ioAttackAttribute.h"

class ioINILoader;
class ioBaseChar;
class ioItem;
class SP2Packet;

class ioGlideSkill : public ioCountSkill
{
protected:
	struct GlideInfo
	{
		ioHashString    m_Animation;			// �۶��̵� ���.
		ioHashString    m_DelayAnimation;		// �۶��̵� �� ���� ���.
		CEncrypt<DWORD> m_dwDelayDuration;	// �ִ� ��¡ �ð�.
		CEncrypt<float> m_fAniRate;				// �۶��̵� ����� aniRate.
		CEncrypt<float> m_fDelayAniRate;		// �۶��̵� �� ���� ����� aniRate.
		CEncrypt<DWORD> m_dwChargingDuration;	// �ִ� ��¡ �ð�.
		CEncrypt<float> m_fDefaultAngle;		// �۶��̵� �߻簢�� �ʱⰪ.
		CEncrypt<float> m_fAngleMin;			// �۶��̵� �߻� �ּҰ�.
		CEncrypt<float> m_fAngleMax;			// �۶��̵� �߻� �ִ밢.
		CEncrypt<float> m_fAngleMovingSpeed;	// �۶��̵� �߻簢 ���� �ӵ�.
		CEncrypt<float> m_fRange;				// �۶��̵� �̵� �Ÿ�.
		CEncrypt<float> m_fSpeed;				// �۶��̵� �̵� �ӵ�.
		
		GlideInfo()
		{
			m_Animation.Clear();
			m_DelayAnimation.Clear();
			m_fAniRate = FLOAT1;
			m_fDelayAniRate = FLOAT1;
			m_dwChargingDuration = 0;
			m_fDefaultAngle = 0.0f;
			m_fAngleMin = 0.0f;
			m_fAngleMax = 0.0f;
			m_fAngleMovingSpeed = 0.0f;
			m_fRange = 0.0f;
			m_fSpeed = 0.0f;
		}
	};

	enum SkillState
	{
		SS_NONE,
		SS_GLIDE_LOOP,
		SS_GLIDE_CHARGING,
		SS_GLIDE_MOVING,
		SS_GLIDE_ATTACK,
		SS_GLIDE_END,
	};

	enum SkillSync
	{
		SC_MOVING,
		SC_KEY,
		SC_ATTACK,
		SC_END,
	};

protected:
	SkillState m_SkillState;

	GlideInfo m_NormalGlideInfo;
	GlideInfo m_JumpGlideInfo;

	ioHashString    m_EndAnimation;
	CEncrypt<float> m_fEndAniRate;

	float m_fMoveRange;

	bool m_bTargetRot;
	bool m_bLeftRot;
	ioUserKeyInput::DirKeyInput m_CurDirKey;
	CEncrypt<DWORD> m_dwRotateSpeed;

	float m_fCurAngle;
	float m_fCurMaxAngle;
	float m_fCurMinAngle;

	ioHashString m_GuideEffect;
	UniqueObjID m_dwGuideEffectID;

	D3DXVECTOR3 m_vMoveDir;
	bool m_bAngleUp;

protected:
	void ClearData();
	void LoadGlideInfo( const char *szType, GlideInfo &rkInfo, ioINILoader &rkLoader );

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

	virtual bool IsAttackEndState() const;
	virtual bool IsNoDropState() const;
	virtual bool IsSkillMotionEnd( DWORD dwActionStop, ioBaseChar* pOwner ) const;

protected:
	virtual bool StartPreDelayAnimation( ioBaseChar *pChar );
	virtual void AttackSkillFire( ioBaseChar *pChar );
	
protected:
	void SetLoopState( ioBaseChar *pChar );
	void SetChargingState( ioBaseChar *pChar );
	void SetGlideState( ioBaseChar *pChar );
	void SetAttackState( ioBaseChar *pChar );
	void SetEndState( ioBaseChar *pChar );

	void ProcessLoopState( ioBaseChar *pChar );
	void ProcessChargingState( ioBaseChar *pChar );
	void ProcessGlideState( ioBaseChar *pChar );

	void CheckKeyInput( ioBaseChar *pChar );
	void ProcessRotate( ioBaseChar *pChar );
	bool CheckLanding( ioBaseChar *pChar );

	void CheckAngle( ioBaseChar* pChar );
	void CheckDir( ioBaseChar *pChar );
	void UpdateGuideEffect( ioBaseChar *pChar );

protected:
	inline void SetSkillState( SkillState eState ) { m_SkillState = eState; }
	const GlideInfo &GetGlideInfo() const;

public:
	ioGlideSkill();
	ioGlideSkill( const ioGlideSkill &rhs );
	virtual ~ioGlideSkill();
};

inline ioGlideSkill* ToGlideSkill( ioSkill *pSkill )
{
	ioCountSkill *pRange = ToCountSkill( pSkill );
	if( !pRange || pRange->GetSubSkillType() != CST_GLIDE )
		return NULL;

	return dynamic_cast< ioGlideSkill* >(pRange);
}