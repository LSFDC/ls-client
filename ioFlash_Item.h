#pragma once

#include "ioWeaponItem.h"
#include "ioAttackAttribute.h"
#include "ioUserKeyInput.h"
#include "ioSpecialStateBase.h"

class ioINILoader;
class ioWeapon;
class ioBaseChar;

class ioFlash_Item : public ioWeaponItem
{
public:
	enum WeaponItemState
	{
		WS_NONE,
		WS_CHECK_CHARGING,
		WS_NORMAL_ATTACK,
		WS_END,
		WS_SPECIAL_STATE_INIT,
		WS_SPECIAL_STATE_PROCESS,
	};

	enum FlashSpecialState
	{
		FMSS_NONE,
		FMSS_CHECK_CHARGING,
		FMSS_MOVE_DELAY,
		FMSS_MOVE_RUN,
		FMSS_MOVE_END,
		FMSS_FIND_TARGET,
		FMSS_ENABLE_TELEPORT,
		FMSS_SET_TARGET,
		FMSS_TELEPORT,
		FMSS_END,
		FMSS_END_JUMP,
		FMSS_CHAR_ROT,
		FMSS_RUN_FIRE,
		FMSS_RUN_END_INPUT,
		FMSS_TELEPORT_MOVE,
		FMSS_CREATE_ATTACH_WEAPON,
	};

	enum TeleportDir
	{
		TD_Forward,
		TD_Right,
		TD_Backward,
		TD_Left,
		TD_MAX,
	};

protected:
	//���� ������ ����
	WeaponItemState m_WeaponItemState;

	//���� �غ� �ִϸ��̼�
	ioHashStringVec m_AttackReadyAniList;
	//���� ���� �ð�
	DWORD m_dwAttackStartTime;
	//���� �޺� ���� ����
	CEncrypt<int> m_nNormalCombo;

	//�ִ� ������
	CEncrypt<float> m_fMaxGauge;
	//�ʿ� ������
	CEncrypt<float> m_fNeedGauge;
	//���� ������
	CEncrypt<float> m_fCurGauge;
	//������ ������
	CEncrypt<float> m_fIncreaseGauge;		

protected:
	//Ư�� ����
	FlashSpecialState m_FlashSpecialState;

	//�޸���
	//RunDelay �ִϸ��̼�
	ioHashString m_szRunDelayAni;
	float m_fRunDelayAniRate;
	DWORD m_dwRunDelay_PreDelay;
	//RunEnd �ִϸ��̼�
	ioHashString m_szRunEndAni;
	float m_fRunEndAniRate;
	DWORD m_dwRunEnd_PreDelay;
	//RunLoop �ִϸ��̼�
	ioHashString m_szRunLoopAniName;

	DWORD m_dwRunDelayMotionEndTime;
	DWORD m_dwRunEndMotionEndTime;
	DWORD m_dwRunWeaponBaseIndex;

	//�޸��� �ɼ�
	CEncrypt<float> m_fRunSpeed;
	DWORD m_dwRunTime;
	DWORD m_dwCheckRunTime;
	CEncrypt<float> m_fCurMoveDist;
	CEncrypt<float> m_fMaxMoveDist;

	//Run ����
	int m_nRunAttachWeaponNum;
	int m_nRunEndAttackWeaponNum;
	int m_nRunEndAttackResistNum;
	ioHashString m_szRunEndAttackWoundedAni;
	DWORD m_dwRunEndAttackWoundedDuration;
	bool m_bRunEndAttackWoundedLoop;

	D3DXVECTOR3 m_vRunEndAttackOffset;
		
	//������
	bool m_bEnableDownAim;	
	float m_fAimAngle;
	float m_fAimRange;
	float m_fMinRange;
	float m_fMarkerRange;

	float m_fMaxMarkerRangeRate;
	float m_fMaxAimRangeRate;
	float m_fMaxAimAngleRate;
	
	//���� �Է� ����
	ioUserKeyInput::DirKeyInput m_InputDirKey;
	bool m_bSetMoveAttack;
	bool m_bSetTarget;

	D3DXVECTOR3 m_vTargetDir;

	//�ڷ���Ʈ
	ioHashString m_szTeleportTargetName;

	CEncrypt<int> m_nCurTeleportCnt;
	CEncrypt<int> m_nMaxTeleportCnt;
	AttributeList m_TeleportList;

	DWORD m_dwTeleportWeaponBaseIndex;
	DWORD m_dwTeleportAttackEndTime;
	DWORD m_dwTeleportReserveTime;
	
	CEncrypt<float> m_fTeleportUseGauge;
	float m_fTeleportRange;

	bool m_bEnableTeleport;

	float m_fEndJumpPower;

	bool m_bAttackKeyReserved;
	bool m_bDefenseKeyReserved;
	bool m_bJumpKeyReserved;

	float m_fTeleportHeightGap;

	bool m_bCheckMove;

public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ioItem* Clone();
	virtual WeaponSubType GetSubType() const;

public:
	//��� ����
	virtual void OnEquiped( ioBaseChar *pOwner );
	//��� ����
	virtual void OnReleased( ioBaseChar *pOwner );

	//���� �غ� �ִϸ��̼� ����
	virtual void SetNormalAttack( ioBaseChar *pOwner, int iCurCombo );
	//���� ���� Ȯ�� (���� ���μ���)
	virtual void CheckNormalAttack( ioBaseChar *pOwner );
	//������ ���⿡ �ɼ� ����
	virtual void SetExtraInfoToWeapon( ioWeapon *pWeapon, bool bJumpAttack );
	//������ ���� (������ ȸ��)
	virtual void UpdateExtraData( ioBaseChar *pOwner );
	//������ ����ȭ �κ�
	virtual void OnItemGaugeOK( ioBaseChar *pOwner, SP2Packet &rkPacket );
	virtual void OnItemGaugeCancel( ioBaseChar *pOwner, SP2Packet &rkPacket );
	//��¡ ���� �����ΰ�
	virtual bool IsChargeAttackState( ioBaseChar *pOwner );
	
	//������ ���� �� ������ ���
	virtual int GetMaxBullet();
	virtual int GetCurBullet();
	virtual void SetCurBullet( int iCurBullet );
	virtual void InitExtraGauge();
	virtual void MaxExtraGauge();
	virtual float GetMaxExtraGauge();
	virtual float GetCurExtraGauge();
	virtual void SetCurExtraGauge( float fGauge );
	int GetNeedBullet();

	virtual void GetAutoTargetValue( ioBaseChar *pOwner,
		float &fRange, float &fMinAngle, float &fMaxAngle,
		AutoTargetType eType );

	//���� ����
	virtual void SetNextItemState( ioBaseChar *pWoundChar, ioBaseChar *pAttacker, ioWeapon *pWeapon, bool bDefense, float fDamage );

	//�⺻ ���� ����
protected:
	//�ʱ�ȭ
	void Init();
	//��¡ Ȯ��
	void CheckCharging( ioBaseChar *pOwner );
	//�⺻ ���� ���·� ����
	void SetNormalAttackState( ioBaseChar *pOwner );
	//������ ���
	void DecreaseGauge( float fNeedGauge );

	//Ư�� ����
public:
	//Ư�� ���� �ʱ�ȭ
	void ClearState( ioBaseChar *pOwner );
	//������ ���� ����
	bool IsEnableGauge();
	//���� Ư�� ���� ���
	FlashSpecialState GetFlashSpecialState();
	//Ư�� ���·� ����
	void SetFlashSpecialState( ioBaseChar *pOwner );
	//Ư�� ���� ���μ���
	void ProcessFlashSpecialState( ioBaseChar *pOwner );
	//ĳ���� ��� ����
	bool IsFlashSpecialCharCollisionSkipState();
	//�ڷ���ƮŸ�� �̸� ����
	//void SetTeleportTarget( ioBaseChar *pTarget, ioBaseChar *pAttacker );
	//�뽬 ������ ����
	void SetDashCharge( ioBaseChar *pOwner );
	//�ڷ���Ʈ ����
	void SetEnableTeleport( ioBaseChar *pTarget, ioBaseChar *pAttacker );

protected:
	void LoadTeleportAttribute( ioINILoader &rkLoader );
	//����ȭ
	void RecvSpecialState( ioBaseChar *pOwner, SP2Packet &rkPacket );
	//Ű�Է� Ȯ��
	void CheckInputKey( ioBaseChar *pOwner );
	//�̵� ��� ����
	void SetRunDelayState( ioBaseChar *pOwner );
	void SetRunLoopState( ioBaseChar *pOwner );
	void SetRunEndState( ioBaseChar *pOwner );
	//ȸ��
	void CheckRotation( ioBaseChar *pOwner );
	//ĳ���� ȸ��
	void SetCharacterRot( ioBaseChar *pOwner );
	//�̵�
	void CheckMove( ioBaseChar *pOwner );

	bool IsChargeAutoTarget( ioBaseChar *pOwner );

	void CheckFindTarget( ioBaseChar *pOwner );
	float GetChargeAimRange();
	float GetChargeAimAngle();
	bool CompareAimedTarget( ioBaseChar *pOwner );

	//�޸��� ���� �� Ű�Է� Ȯ��
	void CheckRunEndInputKey( ioBaseChar *pOwner );
	//�ڷ���Ʈ ���� ����
	void SetTeleportState( ioBaseChar *pOwner );
	//�ڷ���Ʈ Ű �Է� Ȯ��
	void CheckTeleportInputKey( ioBaseChar *pOwner );
	//�ڷ���Ʈ �̵�
	bool SetTeleportMove( ioBaseChar *pOwner );
	//�������
	void CheckReserveInput( ioBaseChar *pOwner );
	void SetReserveAttack( ioBaseChar *pOwner );
	void SetReserveDefence( ioBaseChar *pOwner );
	void SetReserveJump( ioBaseChar *pOwner );
	//���ִ� �����ΰ�
	bool IsFloatState( ioBaseChar *pOwner );	

	bool IsTargetHasHideBuff( ioBaseChar *pOwner, ioHashString szTarget );

	void CreateAttachWeapon( ioBaseChar *pOwner );

public:
	ioFlash_Item();
	ioFlash_Item( const ioFlash_Item &rhs );
	virtual ~ioFlash_Item();
};

inline ioFlash_Item* ToFlashItem( ioItem *pItem )
{
	ioWeaponItem *pWeapon = ToWeaponItem( pItem );
	if( !pWeapon || pWeapon->GetSubType() != ioWeaponItem::WST_FLASHITEM )
		return NULL;

	return dynamic_cast< ioFlash_Item* >( pItem );
}

//-----------------------------------------------------------------------------------------------------------

class ioFlashSpecialState : public ioSpecialStateBase
{
public:
	virtual void Initialize( ioBaseChar* pOwner, CharState eCurrState );
	virtual void Finalize( ioBaseChar* pOwner, CharState eCurrState, CharState eNewState );

	virtual const char* GetStateName(){ return "CS_FLASH_SPECIAL"; }
	virtual const CharState GetState(){ return CS_FLASH_SPECIAL; }	

public:
	virtual bool IsEntityCollisionSkipState( const ioBaseChar *const pOwner, ioGameEntity* pColEntity ) const;
	virtual bool IsCharCollisionSkipState( const ioBaseChar *const pOwner,
		DWORD dwStartTime,
		DWORD dwEndTime,
		DWORD dwSkipType,
		bool bTeamOnly,
		bool bDefense,
		TeamType eOwnerTeam,
		TeamType eOtherTeam ) const;
	virtual void CheckSpecialState( ioBaseChar* pOwner );
	virtual void CheckFallStateChange( ioBaseChar* pOwner, float fMinFallHeightRate, float fBottomHeight, bool bBottomMaxCheck, float fMaxBottomHeight );
	virtual bool ProcessMove( ioBaseChar* pOwner, float fTimePerSec, DWORD dwPreTime, DWORD dwCurTime );

protected:
	int GetFlashSpecialState( ioBaseChar* pOwner );

public:
	ioFlashSpecialState();
	virtual ~ioFlashSpecialState();
};

inline ioFlashSpecialState* ToFlashSpecialState( ioSpecialStateBase *pState )
{
	if( !pState || pState->GetState() != CS_FLASH_SPECIAL )
		return NULL;

	return dynamic_cast< ioFlashSpecialState* >( pState );
}