#pragma once
#include "ioextendjump.h"
class ioSaberJump :	public ioExtendJump
{
protected:
	enum SaberJumpState
	{
		SJS_NONE,
		SJS_NORMAL,
		SJS_D_CHARGE,
		SJS_A_CHARGE,
		SJS_DOUBLE_JUMP,
	};
protected:
	SaberJumpState m_SaberJumpState;

	DWORD m_dwFullJumpChargeTime;

	bool m_bReserveJumpAttack;

	int m_iMaxJumpCnt;
	int m_iCurJumpCnt;
	
	//jump attack
	int m_iCurComboCnt;
	DWORD m_dwEnableComboTime_S;
	DWORD m_dwEnableComboTime_E;
	AttributeList m_ComboAttackList;

	//double jump
	ioHashStringVec m_vDoubleJumpAniList;
	float m_fDoubleJumpAniRate;
	bool m_bUsedDoubleJump;
	float m_fDoubleJumpPower;
	float m_fDoubleJumpForce;
	float m_fDoubleJumpEnableHeight;
	DWORD m_dwDoubleJumpEnableTime;

	//AttackKey Charge
	ioHashStringVec m_AttackKeyChargingAniList;
	//JumpKey Charge
	DWORD m_dwAttackKeyChargingTime;
	DWORD m_dwJumpKeyChargingTime;
	DWORD m_dwChargingStartTime;

	float m_fJumpSpeedRateFR;
	float m_fJumpSpeedRateSI;
	float m_fJumpSpeedRateBG;

	float m_fDoubleJumpSpeedRateFR;
	float m_fDoubleJumpSpeedRateSI;
	float m_fDoubleJumpSpeedRateBG;
protected:
	void ClearData();

protected:	//double jump
	bool IsCanJumpCharge( ioBaseChar *pOwner, float fHeightGap );
	bool IsCanDoubleJump( ioBaseChar *pOwner, float fHeightGap );
	void SetDoubleJump( ioBaseChar *pOwner );
	void SetDoubleJumpAni( ioBaseChar *pOwner );

	
protected:	//D charge
	void SetAttackChargingState( ioBaseChar *pOwner );
	void ChangeToSaberChargeAttack( ioBaseChar *pOwner );
	void ProcessAttackCharging( ioBaseChar *pOwner );
	void ProcessComboJumpAttack( ioBaseChar *pOwner );
	void SetComboJumpAttack( ioBaseChar *pOwner );

protected:	//A charge
	void SetJumpChargingState( ioBaseChar *pOwner );
	bool CheckJumpCharging( ioBaseChar *pOwner );

public:
	inline bool IsJumpAttackReserve() { return m_bReserveJumpAttack; }

public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ioExtendJump* Clone();
	virtual JumpType GetType() const;
	virtual DWORD GetChargingTime( ioBaseChar *pChar );
	
	virtual void CheckComboJumpAttack( ioBaseChar *pOwner );

	virtual void SetJumpping( ioBaseChar *pOwner, bool bFullTime );
	virtual void ProcessJumpping( ioBaseChar *pOwner, float fHeightGap, float fNeedGauge );
	virtual void ProcessJumpAttack( ioBaseChar *pOwner, float fHeightGap );

	virtual void ApplyJump( ioBaseChar *pOwner, SP2Packet &rkPacket );

	virtual float GetExtraJumpSpeedRate( const D3DXVECTOR3 &vDir, const D3DXQUATERNION &qtRot ) const;
public:
	ioSaberJump(void);
	ioSaberJump( const ioSaberJump &rhs );
	virtual ~ioSaberJump(void);
};

inline ioSaberJump* ToSaberJump( ioExtendJump *pJump )
{
	if( !pJump || pJump->GetType() != JT_SABER_JUMP )
		return NULL;

	return dynamic_cast< ioSaberJump* >( pJump );
}
