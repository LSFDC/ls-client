#pragma once

#include "ioExtendDash.h"
#include "ioAttackAttribute.h"

class ioINILoader;
class ioBaseChar;
class ioPlayStage;

class ioSuddenAttackDash : public ioExtendDash
{
public:
	enum DashState
	{
		DS_NONE,
		DS_CHARGE,
		DS_NORMAL_ATTACK,
		DS_COMBO_ATTACK,
		DS_BUFF_ADD,
		DS_EXTEND_COMBO,
		DS_RUN_ROTATE,
		DS_RUN_ROTATE_END,
		DS_POWER_CHARGE,
		DS_POWER_CHARGE_ATTACK,
	};

	/*enum DashExtendType
	{
		DET_NONE,
		DET_BUFF_ADD,
		DET_MECHANICS_FLY,
		DET_EXTEND_COMBO,
		DET_RUN_ROTATE,
		DET_POWER_CHARGE,
		DET_RUN_ROTATE2,
		DET_PHANTOM_FLY,
		DET_LUCIFER_FLY,
		DET_DESTROYER_FLY,
		DET_MICHAEL_FLY,
		DET_GOOMIHO_FLY,
		DET_SUCCUBUS_CHAIN,
		DET_DRACULA,
		DET_FLY_DASH_ATTACK,
		DET_FLASH,
		DET_EVY,
	};*/

	enum DashSyncType
	{
		DST_STATE,
		DST_KEY,
		DST_ROLLING,
	};

protected:
	DashState m_DashState;

	bool m_bSetCombo;
	int m_iCurComboCnt;

	DWORD m_dwActionEndTime;
	DWORD m_dwActionStartTime;

	DWORD m_dwPreReserveEnableTime;

	DWORD m_dwChargingTime;
	DWORD m_dwChargingStartTime;

	ioHashStringVec m_ChargingAniList;

	// dash extend type
protected:
	//DashExtendType m_DashExtendType;

	// buff add
	AttackAttribute m_BuffAddAttribute;
	ioHashStringVec m_vBuffAddList;

	// extend combo
	AttributeList m_ExtendAttackList;

	// run rotate
	CEncrypt<float> m_fRunSpeed;
	CEncrypt<DWORD> m_dwRunDuration;
	CEncrypt<DWORD> m_dwRotateSpeed;

	CEncrypt<DWORD> m_dwRunRotateStartTime;
	CEncrypt<DWORD> m_dwRunRotateJumpTime;
	CEncrypt<DWORD> m_dwFlyDashAttackStartTime;

	AttackAttribute m_RunRotateAttack;

	CEncrypt<bool> m_bTargetRot;
	ioUserKeyInput::DirKeyInput m_CurDirKey;

	AttackAttribute m_RunRotateAttackEnd;

	// power charge
	ioHashString m_PowerChargeAni;

	CEncrypt<DWORD> m_dwMaxChargeTime;
	CEncrypt<DWORD> m_dwChargeStartTime;

	CEncrypt<float> m_fCurChargeRate;

	CEncrypt<float> m_fMaxDamageRate;
	CEncrypt<float> m_fMaxForceRate;
	CEncrypt<float> m_fMaxBlowRate;

	CEncrypt<float> m_fMaxRangeRate;
	CEncrypt<float> m_fMaxSpeedRate;
	CEncrypt<float> m_fMaxChargeAniRate;
	CEncrypt<float> m_fMaxForceSlidingRate;

	CEncrypt<float> m_fMaxTargetRangeRate;
	CEncrypt<float> m_fMaxTargetAngleRate;

	CEncrypt<float> m_fMaxFloatRate;

	CEncrypt<float> m_fMaxScaleRate;
	CEncrypt<float> m_fMaxExplosionRate;
	CEncrypt<float> m_fMaxLiveTimeRate;
	CEncrypt<float> m_fMaxWoundedTimeRate;

	AttackAttribute m_PowerChargeAttack;

	ioHashString m_DashGatheringEffect;
	UniqueObjID m_GatheringEffectID;

	CEncrypt<bool> m_bPowerChargeUseDefense;

	//
	DWORD m_dwInputCancelCheckTime;
	DWORD m_dwCancelCheckTime;

	//
	bool m_bPlayDashExtendWoundAni;


	//////////////////////////////////////////////////////////////////////////
	bool m_bRollingDashStyle;

	ioHashString m_RollDashAction;

	float		 m_fRollActionTimeRate;
	DWORD		 m_dwRollActionPreDelay;
	DWORD		 m_dwRollActionDelay;

	ForceInfoList m_vRollForceInfoList;

	//기본무기가 어떤 상태에서 요청한 것인가
	int			m_WeaponItemState;
	bool		m_bAttackReserved;

protected:
	// For Ani Rotate
	vAniRotateInfoList m_vAniRotateInfoList;

	int m_iCurAniRotate;
	DWORD m_dwAniRotateTime;
	float m_fAniRotateAngle;

protected:
	void CheckAniRotate( ioBaseChar *pChar );

public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual DashType GetType() const;
	virtual ioExtendDash* Clone();

	virtual bool StartDash( ioBaseChar *pOwner );
	virtual void ProcessDash( ioBaseChar *pOwner );
	virtual void ExtendDashEnd( ioBaseChar *pOwner );

	virtual void ApplyDash( ioBaseChar *pOwner, SP2Packet &rkPacket );

	virtual bool GetAutoTargetValue( IN float &fDashRange, IN float &fDashMinAngle, IN float &fDashMaxAngle,
		OUT float &fRange, OUT float &fMinAngle, OUT float &fMaxAngle );

	virtual bool IsEnableDefense();
	bool IsPowerChargingState();

public:
	void SetExtraInfoToWeapon( ioBaseChar *pOwner, ioPlayStage *pStage, ioWeapon *pWeapon, const ioHashString &szName );

protected:
	void SetComboDash( ioBaseChar *pOwner );

	void ProcessCharging( ioBaseChar *pOwner );
	void ProcessNormalComboDash( ioBaseChar *pOwner );
	void ProcessExtendComboDash( ioBaseChar *pOwner );
	void ProcessRunRotate( ioBaseChar *pOwner );
	void ProcessRunRotateEnd( ioBaseChar *pOwner );
	void ProcessPowerCharge( ioBaseChar *pOwner );
	void ProcessPowerChargeAttack( ioBaseChar *pOwner );

	void ChangeToCharging( ioBaseChar *pOwner );
	void ChangeToNormalAttack( ioBaseChar *pOwner );

	virtual void CheckKeyReserve( ioBaseChar *pOwner );
	virtual void ProcessKeyReserve( ioBaseChar *pOwner );

	void CheckExtraReserve( ioBaseChar *pOwner );
	void CheckExtendReserve( ioBaseChar *pOwner );
	bool CheckPreReserve( ioBaseChar *pOwner );

	void ApplyDashState( ioBaseChar *pOwner, SP2Packet &rkPacket );
	void ApplyDashDirKey( ioBaseChar *pOwner, SP2Packet &rkPacket );
	void ApplyDashRolling( ioBaseChar *pOwner, SP2Packet &rkPacket );

	bool NoramlAttackStartDash( ioBaseChar *pOwner );
	bool RollingStartDash( ioBaseChar *pOwner );

	// buff add
protected:
	bool CheckEnableBuffAddState( ioBaseChar *pOwner );
	void SetBuffAddState( ioBaseChar *pOwner );


protected:
	bool CheckEnableExtendComboState( ioBaseChar *pOwner );
	void SetExtendComboState( ioBaseChar *pOwner );

protected:
	bool CheckRunRotateState( ioBaseChar *pOwner );
	bool CheckRunRotateState2( ioBaseChar *pOwner );

	void SetRunRotateState( ioBaseChar *pOwner, bool bSendNet );
	void SetRunRotateStateEnd( ioBaseChar *pOwner );

	void CheckKeyInput( ioBaseChar *pOwner );
	void ProcessRotate( ioBaseChar *pOwner );

	bool CheckJumpRunRoateEnd( ioBaseChar *pOwner );

	ioHashString	m_stAniJump;
	float			m_fAniJumpRate;
	bool CheckAniJump_Input_Jump_S( ioBaseChar *pOwner );

protected:
	bool CheckPowerChargeState( ioBaseChar *pOwner );
	void SetPowerChargeState( ioBaseChar *pOwner, bool bSendNet );
	void SetPowerChargeAttack( ioBaseChar *pOwner );

	void CheckChargeInfo( ioWeapon *pWeapon );

	//공용
public:
	bool IsCanPlayWoundedAniFromExtendDash();
	bool IsNoDropState();

protected:
	bool CheckCanComboDash( ioBaseChar *pOwner );

	void SetRunRotateBuff( ioBaseChar *pOwner );
	void RemoveRundRotateBuff( ioBaseChar *pOwner );

public:
	ioSuddenAttackDash();
	ioSuddenAttackDash( const ioSuddenAttackDash &rhs );
	virtual ~ioSuddenAttackDash();
};

inline ioSuddenAttackDash* ToSuddenAttackDash( ioExtendDash *pDash )
{
	if( !pDash || pDash->GetType() != DT_SUDDEN_ATTACK )
		return NULL;

	return dynamic_cast< ioSuddenAttackDash* >( pDash );
}

