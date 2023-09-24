#pragma once

#include "ioDummyChar.h"
#include "ioWorldEventReceiver.h"
#include "ioAttackAttribute.h"
#include "ioUserKeyInput.h"
#include "DummyCharEnumType.h"

class ioPlayStage;
class ioPlayMode;
class ioGaugeHelper;
class ioINILoader;
class ioWeapon;
class SP2Packet;
class ioBaseChar;
class ioAniCollisionBoxGrp;

class ioCurseDummyChar : public ioDummyChar
{
public:
	enum OwnerControlSync
	{
		SYNC_APPLY_WEAPON = 1000,
	};

protected:
	DWORD m_dwLastApplyWeaponIdx;
	DWORD m_dwLastApplyWeaponTime;

	AniDuration	   m_AnimationTime;
	BlowTargetType m_BlowTargetType;
	AttackSide	   m_AttackedSide;
	CEncrypt<BlowWoundedState> m_BlowState;
	int m_iFloatingWoundedCnt;
	int m_iBackAttackedCnt;

	CEncrypt<float> m_fFloatingBlowDownRate;
	CEncrypt<float> m_fKOActionBlowPower;

	bool m_bJumpedBlowWounded;
	bool m_bFallBlowWounded;
	bool m_bBlowEndAni;

	float m_fBlowGetUpTimeRate;
	DWORD m_dwCurBlowGetUpTime;
	DWORD m_dwMaxBlowWoundedGetUpTime;
	CEncrypt<DWORD> m_dwGeneralBlowWoundedGetUpTime;

	DWORD m_dwBlowGetUpCheckStartTime;
	DWORD m_dwBlowAniEndTime;
	DWORD m_dwGetUpDurationTime;
	DWORD m_dwPreBackAttackedTime;

	float m_fBoundBlowJumpAmt;
	float m_fBoundBlowForceAmt;
	float m_fBoundBlowGravityRate;
	float m_fBoundBlowBoundDamage;

	ioHashString m_CurBlowWoundAction;
	ioHashString m_CurBlowDownAction;

	ioHashString m_AniFallDownAction;
	ioHashString m_AniFallBackDownAction;

	ioHashString m_AniKoAction;
	ioHashString m_AniBackKoAction;
	ioHashString m_AniDownKoAction;
	ioHashString m_AniBackdownKoAction;
	ioHashString m_AniJumpKoAction;
	ioHashString m_AniBackJumpKoAction;

	ioHashString m_AniKoAction2;
	ioHashString m_AniBackKoAction2;
	ioHashString m_AniDownKoAction2;
	ioHashString m_AniBackdownKoAction2;

	ioHashString m_AniKoAction3;
	ioHashString m_AniBackKoAction3;
	ioHashString m_AniDownKoAction3;
	ioHashString m_AniBackdownKoAction3;

	float m_fCurGravityGapAmt;

	CEncrypt<float> m_fWeakAttackWoundedRate;
	CEncrypt<float> m_fMidStiffWoundedRate;
	CEncrypt<float> m_fStrStiffWoundedRate;
	CEncrypt<float> m_fStandKoWoundedRate;
	CEncrypt<float> m_fStandKo2WoundedRate;

	ioHashString m_AniBlowPush;
	ioHashString m_AniBlowGetUp;
	ioHashString m_AniBackBlowGetUp;

	ioHashString m_WeakWounded;
	ioHashString m_MidStiffWounded;
	ioHashString m_StrStiffWounded;

	ioHashString m_BackWeakWounded;
	ioHashString m_BackMidStiffWounded;
	ioHashString m_BackStrStiffWounded;

	ioHashString m_AniStandKoAction;
	ioHashString m_AniStandKoDown;
	ioHashString m_AniStandKo2Action;
	ioHashString m_AniStandKo2Down;

	ioHashString m_AniBackStandKoAction;
	ioHashString m_AniBackStandKoDown;
	ioHashString m_AniBackStandKo2Action;
	ioHashString m_AniBackStandKo2Down;

	ioHashString m_AniBoundBlowAction;
	ioHashString m_AniBoundBlowBackAction;
	ioHashString m_AniBoundBlowAction2;
	ioHashString m_AniBoundBlowBackAction2;
	ioHashString m_AniBoundDownAction;
	ioHashString m_AniBoundDownBackAction;

	WeaponIgnoreInfo m_WeaponIgnoreInfo;

	CEncrypt<float> m_fForcePowerAddRate;

	CEncrypt<float> m_fRecoverySkillGaugeOnWounded;
	CEncrypt<float> m_fRecoveryMaxSkillGauge;
	CEncrypt<float> m_fRecoveryCurSkillGauge;
	ioHashString   m_szRecoverySkillGaugeEffect;

	enum ExplsionState
	{
		ES_NONE,
		ES_EXPLOSION_WAIT,
		ES_EXPLOSION,
	};

	ExplsionState   m_ExplosionState;
	CEncrypt<DWORD> m_dwExplosionWaitTime;
	DWORD           m_dwExplosionTime;
	WeaponInfo      m_WeaponOnWound;
	ioHashString    m_szExplosionWaitEffect;

	ioHashString    m_ExplosionEffect;
	ioHashString    m_ExplosionSound;

	//////////////////////////////////////////////////////////////////////////
	ioHashString	m_Creater;
	ioHashString	m_CurseChar;

	ioHashString	m_MaxAttackBuff;

	int				m_nMaxAttackCnt;
	int				m_nCurAttackCnt;

	DWORD			m_dwMaxLifeTime;
	DWORD			m_dwStartTime;
	//////////////////////////////////////////////////////////////////////////

	

public:
	void SetCharNameInfo( const ioHashString& stCurseName, const ioHashString& stCreaterNamme );

protected:
	virtual void ProcessDelayState( float fTimePerSec );


public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	void LoadPropertyByChar();
	void SetCopyOwnerInfo( ioBaseChar *pOwner, bool bOnlyInfo = false );
	bool CheckOwnerDestroyState();

public:
	virtual bool IsCollisionAvailableWeapon( ioWeapon *pWeapon );
	virtual bool IsCharCollisionSkipState( TeamType eTeamType, int iState=0 ) const;
	virtual bool IsDummyCharCollisionSkipState( ioBaseChar *pChar );
	virtual bool IsEntityCollisionSkipState( ioGameEntity* pColEntity ) const;
	virtual void SetDieState( bool bEffect );

	virtual void Update( float fTimePerSec, DWORD dwPreTime, DWORD dwCurTime );

public:
	virtual void RenderGauge();

protected:
	virtual void ProcessState( float fTimerPerSec, DWORD dwPreTime );
	virtual void ProcessPos( float fTimePerSec );
	virtual void ProcessForceMove( float fTimePerSec );

	void ProcessWoundedState( float fTimePerSec );
	void ProcessBlowWoundedState( float fTimePerSec );
	void ProcessBoundBlowWoundedState( float fTimePerSec );

public:
	virtual ApplyWeaponExp ApplyWeapon( ioWeapon *pWeapon );
	void ApplyWeaponWounded( ioWeapon *pWeapon, AttackSide eSide, const D3DXVECTOR3 &vAttackDir );
	void ApplyWeaponWoundedInNetwork( ioPlayStage *pStage, SP2Packet &rkPacket );
	virtual void ApplyDummyCharSync( ioPlayStage *pStage, SP2Packet &rkPacket );

public:
	void SetDummyState( DummyCharState eState );
	void SetWoundedState( const ioHashString &szWoundedAni, DWORD dwWoundedDuration, bool bLoopAni );
	void SetBlowWoundedState( bool bJumped, const BlowWoundAniInfo &rkBlowWoundAniInfo,
		bool bGetUpState = false, bool bFallState = false );
	void SetBoundWoundedState( float fBoundJump,
		float fBoundForce,
		float fBoundGravity,
		float fBoundDamage,
		bool bGetUpState = false );
	void SetBlowWoundedGetUp();
	int GetWoundedAnimationIdx( AttackSide eAttackSide, BlowTargetType eBlowTargetType ) const;
	AttackSide GetAttackedSide( const D3DXVECTOR3 &vAttackDir ) const;

	void SetIgnoreWeaponInfo( ioWeapon *pWeapon );
	void SetLastApplyWeapon( int iIndex, DWORD dwTime );

	bool IsDownState();
	bool IsFloating();
	bool IsFloatingState();

	void RenderGaugeOwnerInfo();

	void SetJumpPower( float fPowerAmt );
	inline void SetForcePowerAmt( float fAmt ) { m_fForceAmt = fAmt; }

protected:
	void SetExplosionWaitState();
	void ProcessExplosionState();

public:
	void SetExplosion();

public:
	ioCurseDummyChar( ioEntityGroup *pGrp, ioPlayMode *pMode );
	virtual ~ioCurseDummyChar();
};

inline ioCurseDummyChar* ToCurseDummyChar( ioDummyChar *pDummy )
{
	if( !pDummy || pDummy->GetType() != DCT_CURSE_CHAR )
		return NULL;

	return dynamic_cast< ioCurseDummyChar* >( pDummy );
}
