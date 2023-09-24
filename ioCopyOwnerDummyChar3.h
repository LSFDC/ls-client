#pragma once
#include "ioDummyChar.h"
#include "ioWorldEventReceiver.h"
#include "ioAttackAttribute.h"
#include "ioUserKeyInput.h"
#include "DummyCharEnumType.h"

class ioCopyOwnerDummyChar3 : public ioDummyChar
{
public:
	enum OwnerControlSync
	{
		//SYNC_APPLY_WEAPON = 1000,
		SYNC_TRACING = 1000,
		SYNC_EXPLOSION_WAIT,
	};

protected:
	DWORD m_dwLastApplyWeaponIdx;
	DWORD m_dwLastApplyWeaponTime;

	AniDuration	   m_AnimationTime;
	BlowTargetType m_BlowTargetType;
	AttackSide	   m_AttackedSide;

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

	/////////////////////////////////////////////////////////////////////////
	enum ExplsionState
	{
		ES_NONE,
		ES_EXPLOSION_WAIT,
		ES_EXPLOSION,
	};

	WeaponInfo      m_WeaponOnWound;

	ExplsionState   m_ExplosionState;
	DWORD			m_dwExplosionWaitTime;
	DWORD			m_dwExplosionTime;

	ioHashString	m_szExplosionWaitEffect;
	ioHashString	m_ExplosionEffect;
	ioHashString	m_ExplosionSound;

	float			m_fTargetTraceExplosionRange;

	DWORD			m_dwMaxLifeTime;

	ioHashString	m_stTeamEffect;

public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	void LoadPropertyByChar();
	void SetCopyOwnerInfo( ioBaseChar *pOwner );

public:
	virtual bool IsCollisionAvailableWeapon( ioWeapon *pWeapon );
	virtual bool IsCharCollisionSkipState( TeamType eTeamType, int iState=0 ) const;
	virtual bool IsDummyCharCollisionSkipState( ioBaseChar *pChar );
	virtual void SetDieState( bool bEffect );

public:
	virtual void RenderGauge();

public:
	virtual void ProcessState( float fTimePerSec, DWORD dwPreTime );
	virtual void ProcessPos( float fTimePerSec );
	virtual void ProcessForceMove( float fTimePerSec );

	void ProcessWoundedState( float fTimePerSec );

	void ProcessTraceState( float fTimePerSec, DWORD dwPreTime );

public:
	virtual ApplyWeaponExp ApplyWeapon( ioWeapon *pWeapon );
	void ApplyWeaponWounded( ioWeapon *pWeapon, AttackSide eSide, const D3DXVECTOR3 &vAttackDir );
	//void ApplyWeaponWoundedInNetwork( ioPlayStage *pStage, SP2Packet &rkPacket );
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
	void SetTraceState( DummyCharTraceState iTraceState, bool bAniDelay );
	void ProcessTraceSensing( float fTimePerSec );
	void SetTracingState( ioPlayStage *pStage, bool bSendNetwork );
	void ProcessTraceTracing( float fTimePerSec );
	void ProcessExplosionState();

	void SetExplosionWaitState( bool bSendNet, ioPlayStage *pStage );
	void SetExplosionState();
	void CreateExplosionWeapon();

public:
	ioCopyOwnerDummyChar3( ioEntityGroup *pGrp, ioPlayMode *pMode );
	virtual ~ioCopyOwnerDummyChar3(void);
};

inline ioCopyOwnerDummyChar3* ToCopyOwnerDummyChar3( ioDummyChar *pDummy )
{
	if( !pDummy || pDummy->GetType() != DCT_COPY_OWNER3 )
		return NULL;

	return dynamic_cast< ioCopyOwnerDummyChar3* >( pDummy );
}
