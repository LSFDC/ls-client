#pragma once

#include "ioExtendJump.h"
#include "ioAttackAttribute.h"

class ioBaseChar;
class ioINILoader;
class SP2Packet;

class ioIljimaeJump : public ioExtendJump
{
public:
	enum ComboState
	{
		CST_NONE,
		CST_DASH,
		CST_DASH_END,
		CST_NEW_JUMP,
	};

	enum ProcessJumpState
	{
		PJS_NONE,
		PJS_NORMAL_ATTACK,
		PJS_TELEPORT,
		PJS_EXTEND_ATTACK,
		PJS_DASH_ATTACK,
		PJS_DASH_ADD_ATTACK,
		PJS_END,
	};

	enum ComboDashType
	{
		CDT_GRAVITY		= 1,
		CDT_NO_GRAVITY	= 2,
	};

	enum SyncState
	{
		SSC_ATTACK,
		SSC_DASH,
		SSC_DOUBLE_JUMP,
		SSC_CHANGE_TELEPORT,
		SSC_TELEPORT,
		SSC_EXTEND_ATTACK,
		SSC_DASH_ADD_ATTACK,
	};

protected:
	ComboState m_ComboState;
	ProcessJumpState m_JumpState;

	AttributeList m_ComboAttackList;
	int m_iCurComboCnt;

	DWORD m_dwEnableComboTime_S;
	DWORD m_dwEnableComboTime_E;

	float m_fDoubleJumpSpeedRateFR;
	float m_fDoubleJumpSpeedRateSI;
	float m_fDoubleJumpSpeedRateBG;

	float m_fExtendJumpSpeedRateFR;
	float m_fExtendJumpSpeedRateSI;
	float m_fExtendJumpSpeedRateBG;

	bool m_bReserveJumpAttack;

	float m_fExtraJumpPowerRate;
	DWORD m_dwJumpChargeTime;
	DWORD m_dwChargingStartTime;

	// Dash
	bool m_bEnableJumpDash;
	ComboDashType m_ComboDashType;

	AttackAttribute m_DashJumpAttack;
	
	float m_fComboDashGravityAmt;
	DWORD m_dwComboDashStartTime;
	DWORD m_dwComboDashEndTime;

	ioHashString m_ComboDashLandAni;
	float m_fComboDashLandAniRate;

	bool m_bUseNewJumpTypeDash;

	// DoubleJump
protected:
	float m_fDoubleJumpPower;
	float m_fDoubleJumpForce;
	float m_fDoubleJumpEnableHeight;
	DWORD m_dwDoubleJumpEnableTime;

	float m_fDoubleJumpAniRate;
	ioHashStringVec m_vDoubleJumpAniList;

	bool m_bUsedDoubleJump;

	int m_iMaxJumpCnt;
	int m_iCurJumpCnt;

	// Teleport
protected:
	ioHashString	m_TeleportMotion;
	CEncrypt<float>	m_fTeleportMotionRate;
	ioHashString	m_TeleportEndMotion;
	CEncrypt<float>	m_fTeleportEndMotionRate;

	TargetWoundType	m_TargetWoundType;

	CEncrypt<float>	m_fTeleportRange;
	CEncrypt<float>	m_fTeleportAngle;
	CEncrypt<float>	m_fMoveRange;
	CEncrypt<float>	m_fMoveHeightRange;

	DWORD m_dwMotionEndTime;
	ioHashString	m_ReturnMark;
	DWORD m_dwReturnMarkDuration;

	ioHashStringVec	m_vTargetNameList;
	ioHashString	m_PreTargetName;
	D3DXVECTOR3		m_vReturnPosition;

	CEncrypt<bool> m_bUsedTeleport;

protected:
	float m_fChargeJumpPower;
	float m_fChargeGravityRate;
	AttackAttribute m_ExtendLandAttack;

	bool m_bUsedChargeAttack;
	float m_fCurCharMoveSpeed;
	DWORD m_dwChargeAttackEndTime;

// 점프대쉬 이후 Add Attack
private:
	AttributeList m_vJumpDashAddAttack;
	DWORD m_dwJumpDashAddAttackReserveTime;
	int m_iJumpDashAddAttackIndex;

public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ioExtendJump* Clone();
	virtual JumpType GetType() const;

public:
	virtual void SetJumpping( ioBaseChar *pOwner, bool bFullTime );
	virtual void ProcessJumpping( ioBaseChar *pOwner, float fHeightGap, float fNeedGaguge );
	virtual void ProcessJumpAttack( ioBaseChar *pOwner, float fHeightGap );
	virtual void ApplyJump( ioBaseChar *pOwner, SP2Packet &rkPacket );

	void ProcessComboDash( ioBaseChar *pOwner );

	virtual DWORD GetChargingTime( ioBaseChar *pChar );
	virtual void CheckComboJumpAttack( ioBaseChar *pOwner );
	
	virtual float GetExtraJumpSpeedRate( const D3DXVECTOR3 &vDir, const D3DXQUATERNION &qtRot ) const;
	virtual bool IsCanJumpingSkill( ioBaseChar *pOwner );

	virtual bool IsEnableJumpLandEndDash();

	const ioHashString& GetComboDashLandAni() const { return m_ComboDashLandAni; }
	float GetComboDashLandAniRate() const { return m_fComboDashLandAniRate; }

	virtual bool IsEnableJumpDash( ioBaseChar *pOwner );
	bool IsComboDashState();

	float GetCurGravityAmt();

	virtual void SetLandingState( ioBaseChar *pOwner );

	inline const bool IsUsedTeleport() const { return m_bUsedTeleport; }

protected:
	void SetComboJumpAttack( ioBaseChar *pOwner );
	void LoadComboList( ioINILoader &rkLoader );

	bool IsCanComboDash( ioBaseChar *pOwner, float fHeightGap );
	void SetComboDash( ioBaseChar *pOwner, float fHeightGap );

	bool IsCanDoubleJump( ioBaseChar *pOwner, float fHeightGap );
	void SetDoubleJumpAni( ioBaseChar *pOwner );

	bool CheckAliceGun( ioBaseChar *pOwner );

	void SetNewJumpTypeDashAttack( ioBaseChar *pOwner, DWORD dwEndTime, DWORD dwReserveTime );
	void SetJumpDashAddAttack( ioBaseChar *pOwner, int iAttackIndex );

protected:
	void ProcessTeleport( ioBaseChar *pOwner );

	void ChangeToJumpAttack( ioBaseChar *pOwner );
	void ChangeToTeleport( ioBaseChar *pOwner );

public:
	inline int GetCurrComboCnt(){ return m_iCurComboCnt; }

protected:
	void ChangeToPowerChargeJumpAttack( ioBaseChar *pOwner );

public:
	inline float GetChargeGravityRate() const { return m_fChargeGravityRate; }
	bool IsChargeAttacked();
	bool IsChargeEnd();

public:
	inline const ioHashString& GetChargeLandAni() const { return m_ExtendLandAttack.m_AttackAnimation; }
	inline float GetChargeLandAniRate() const { return m_ExtendLandAttack.m_fAttackAniRate; }
	const AttackAttribute& GetLandAttackAttribute() const  { return m_ExtendLandAttack; }

public:
	ioIljimaeJump();
	ioIljimaeJump( const ioIljimaeJump &rhs );
	virtual ~ioIljimaeJump();
};

inline ioIljimaeJump* ToIljimaeJump( ioExtendJump *pJump )
{
	if( !pJump || pJump->GetType() != JT_ILJIMAE_JUMP )
		return NULL;

	return dynamic_cast< ioIljimaeJump* >( pJump );
}

