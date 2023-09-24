#pragma once

#include "ioWeaponItem.h"

class ioEntityGroup;

class ioMagicStaffItem : public ioWeaponItem
{
protected:
	DWORD m_dwTimeGap;
	DWORD m_dwChargeStartTime;

	float m_fRange;
	float m_fMagicCircleRadius;
	float m_fMoveSpeed;
	float m_fHeightGap;
	CEncrypt<float> m_fSkillGaugeRate;

protected:
	DWORD m_dwDefenseTicTime;
	float m_fGaugePerAllDefense;

protected:
	ioHashString m_PreGatherAnimation;
	ioHashString m_GatherAnimation;
	ioHashString m_EndGatherAnimation;
	ioHashString m_GatherEffect;

	ioHashString m_ReadyCircle;
	ioHashString m_EnableCircle;

protected:
	enum FireState
	{
		FS_NONE,
		FS_CHARGING,
		FS_GATHERING,
		FS_NORMAL_ATTACK,
	};

	FireState m_FireState;
	CEncrypt<int>  m_iCurCombo;

public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ioItem* Clone();

public:
	virtual void SetNormalAttack( ioBaseChar *pOwner, int iCurCombo );
	virtual void CheckNormalAttack( ioBaseChar *pOwner );
	virtual void OnItemGaugeCancel( ioBaseChar *pOwner, SP2Packet &rkPacket );

	virtual void SetExtraInfoToWeapon( ioWeapon *pWeapon, bool bJumpAttack );
	virtual void OnEndExtendAttack( ioBaseChar *pOwner, bool bReleaseItem=false );

	virtual bool IsChargeAttackState( ioBaseChar *pOwner );
	virtual void UpdateSkillGauge( ioBaseChar *pOwner, float fRate, bool bUpdate );
	virtual bool IsEnableExtraGauge( ioBaseChar *pChar );
	virtual float GetExtraGauge() { return m_fSkillGaugeRate; }

	virtual WeaponSubType GetSubType() const;

	virtual bool SetJumpAttack( ioBaseChar *pOwner, int iAniID, float fTimeRate, AttackType eType );
	virtual void ProcessJumpAttack( ioBaseChar *pOwner, float fHeightGap );

public:
	float GetCircleRadius();

protected:
	void OnCharging( ioBaseChar *pOwner );
	void OnGathering( ioBaseChar *pOwner );

protected:
	void ChangeToNormalAttack( ioBaseChar *pOwner );
	void ChangeToGathering( ioBaseChar *pOwner );

public:
	ioMagicStaffItem();
	ioMagicStaffItem( const ioMagicStaffItem &rhs );
	virtual ~ioMagicStaffItem();
};

inline ioMagicStaffItem* ToMagicStaffItem( ioItem *pItem )
{
	ioWeaponItem *pWeapon = ToWeaponItem( pItem );
	if( !pWeapon || pWeapon->GetSubType() != ioWeaponItem::WST_MAGIC_STAFF )
		return NULL;

	return dynamic_cast< ioMagicStaffItem* >( pWeapon );
}

inline ioMagicStaffItem* ToMagicStaffItem( ioWeaponItem *pWeapon )
{
	if( !pWeapon || pWeapon->GetSubType() != ioWeaponItem::WST_MAGIC_STAFF )
		return NULL;

	return dynamic_cast< ioMagicStaffItem* >( pWeapon );
}

