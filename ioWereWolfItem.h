#pragma once

#include "ioWeaponItem.h"
#include "ioAttackAttribute.h"
#include "ioSpecialStateBase.h"

class ioINILoader;
class ioWeapon;
class ioBaseChar;

class ioWereWolfItem : public ioWeaponItem
{
protected:
	ioHashStringVec m_AttackReadyAniList;
	AttributeList m_vExtendAttributeList;

protected:
	enum ChargeState
	{
		CS_NONE,
		CS_CHARGING,
		CS_GATHERING,
		CS_ATTACK_FIRE,
	};

	ChargeState m_ChargeState;
	DWORD m_dwAttackStartTime;
	int m_iCurCombo;

	bool m_bCharged;

	CEncrypt<bool> m_bEnableMaxCharge;
	ioHashString m_GatherAnimation;
	float m_fGatherAniRate;

	DWORD m_dwGatheringStartTime;

	// bullet
	CEncrypt<float> m_fMaxChangeDamage;
	CEncrypt<float> m_fNeedChangeDamage;
	
	CEncrypt<float> m_fIncreaseChangeDamage;
	CEncrypt<float> m_fDecreaseChangeDamage;
	CEncrypt<float> m_fDownDecreaseChangeDamage;

	CEncrypt<float> m_fChangeDamageRateMin;
	CEncrypt<float> m_fChangeDamageRateMax;

	CEncrypt<float> m_fCurChangeDamage;

	ioHashString m_MaxChangeDamageEffect;
	UniqueObjID m_dwChangeDamageEffect;

protected:
	ioHashString m_RidingDummyCharName;
	int m_iRidingDummyCharIndex;

public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ioItem* Clone();

	virtual WeaponSubType GetSubType() const;

public:
	virtual void OnReleased( ioBaseChar *pOwner );

	virtual void SetNormalAttack( ioBaseChar *pOwner, int iCurCombo );
	virtual void CheckNormalAttack( ioBaseChar *pOwner );
	virtual void SetExtraInfoToWeapon( ioWeapon *pWeapon, bool bJumpAttack );

	virtual void OnEndExtendAttack( ioBaseChar *pOwner, bool bReleaseItem=false );
	virtual void OnItemGaugeOK( ioBaseChar *pOwner, SP2Packet &rkPacket );

	virtual bool IsChargeAttackState( ioBaseChar *pOwner );
	virtual bool IsEnableAttackDashCheck( ioBaseChar *pOwner, bool bCheck );

	virtual bool SetJumpAttack( ioBaseChar *pOwner, int iAniID, float fTimeRate, AttackType eType );
	virtual void ProcessJumpAttack( ioBaseChar *pOwner, float fHeightGap );

	virtual void SetExtraGaugeTypeByWeapon( ioBaseChar *pOwner, int iType, int iWGT, float fDamage );

protected:
	void OnCharging( ioBaseChar *pOwner );
	void OnGathering( ioBaseChar *pOwner );

	void ChangeToGathering( ioBaseChar *pOwner );
	void ChangeToAttackFire( ioBaseChar *pOwner, bool bCharged );
	void ChangeToRiding( ioBaseChar *pOwner );

	virtual void CheckReserve( ioBaseChar *pOwner );

	virtual bool ProcessCancel( ioBaseChar *pOwner );

	bool CheckOwnerDownState( ioBaseChar *pOwner );

public:
	bool CreateDummyChar( ioBaseChar *pOwner, int iIndex );

	bool UsingRidingDummyChar();
	void SetRidingDummyCharIndex( ioBaseChar *pOwner, int iCurIndex );
	inline int GetRidingDummyCharIndex() const { return m_iRidingDummyCharIndex; }

public:
	virtual int GetCurBullet();
	virtual int GetMaxBullet();
	virtual void SetCurBullet( int iCurBullet );

	int GetNeedBullet();

	void InitChangeDamage();
	void MaxChangeDamage();

	float GetCurChangeDamage();
	void SetCurChangeDamage( float fDamage );

	void SetEnableChangeDamageEffect( ioBaseChar *pChar );
	void ReleaseEnableChangeDamageEffect( ioBaseChar *pChar );

	void CheckEnableChangeDamage( ioBaseChar *pChar );
	void ApplyChangeWereWolfState( ioBaseChar *pChar );

	virtual bool CheckChangeWereWolfState( ioBaseChar *pChar );
	virtual void SetChangeWereWolfState( ioBaseChar *pChar );

	virtual void IncreaseChangeDamage( float fDamage, int iType, bool bBuff );
	virtual void IncreaseChangeDamageByWeapon( float fDamage );
	virtual void CheckIncreaseChangeDamage( ioBaseChar *pOwner );
	virtual void DecreaseChangeDamage( ioBaseChar *pOwner );

	virtual bool CheckReleaseWereWolfState( ioBaseChar *pChar );

	virtual void ReleseWereWolfState( ioBaseChar *pChar );
	virtual void ReleseWereWolfStateByOwner( ioBaseChar *pChar );

	virtual bool IsEnableActionStopDelay();

	bool IsRidingState();

	virtual void ReleaseObjectState( ioBaseChar *pOwner, ioObjectItem* pReleasedObject );

public:
	ioWereWolfItem();
	ioWereWolfItem( const ioWereWolfItem &rhs );
	virtual ~ioWereWolfItem();
};

inline ioWereWolfItem* ToWereWolfItem( ioItem *pItem )
{
	ioWeaponItem *pWeapon = ToWeaponItem( pItem );
	if( !pWeapon || pWeapon->GetSubType() != ioWeaponItem::WST_WEREWOLF )
		return NULL;

	return dynamic_cast< ioWereWolfItem* >( pItem );
}

//-----------------------------------------------------------------------------------------------------------

class ioWerewolfSpecialState : public ioSpecialStateBase
{
public:
	virtual void Initialize( ioBaseChar* pOwner, CharState eCurrState );
	virtual void Finalize( ioBaseChar* pOwner, CharState eCurrState, CharState eNewState );

	virtual const char* GetStateName(){ return "CS_WEREWOLF"; }
	virtual const CharState GetState(){ return CS_WEREWOLF; }
	virtual bool IsEnableBlowWounded( ioBaseChar *pOwner ){ return false; }

public:
	virtual void CheckSpecialState( ioBaseChar* pOwner );
	virtual void CheckFallStateChange( ioBaseChar* pOwner, float fMinFallHeightRate, float fBottomHeight, bool bBottomMaxCheck, float fMaxBottomHeight );

public:
	ioWerewolfSpecialState();
	virtual ~ioWerewolfSpecialState();
};

inline ioWerewolfSpecialState* ToWerewolfSpecialState( ioSpecialStateBase *pState )
{
	if( !pState || pState->GetState() != CS_WEREWOLF )
		return NULL;

	return dynamic_cast< ioWerewolfSpecialState* >( pState );
}