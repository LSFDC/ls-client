#pragma once

#include "ioWeaponItem.h"
#include "ioAttackAttribute.h"
#include "ioSpecialStateBase.h"

class ioINILoader;
class ioWeapon;
class ioBaseChar;

class ioSuperDimensionItem : public ioWeaponItem
{
public:
	enum WeaponItemState
	{
		WS_NONE,
		WS_CHECK_CHARGING,
		//WS_CHARGING_ATTACK,
		WS_NORMAL_ATTACK,
		WS_END,
		WS_SPECIAL_FIRE_STATE_INIT,
		//WS_SPECIAL_STATE_PROCESS,
		WS_SPECIAL_DASH_GRAP,
		WS_SPECIAL_DASH_LAST_ATTACK,
	};

	enum SuperDimensionFireState
	{
		PSS_None,
		PSS_Find_Target,
		PSS_Set_Target,
		PSS_Wait,
		PSS_End,
	};


	enum SuperDimensionDashState
	{
		MFS_NONE,
		MFS_START,
		MFS_ATTACK_MOVE,
		//MFS_GRAPPLING_MOVE,
		MFS_GRAPPLING_LOOP,
		MFS_END_ATTACK,
		MFS_END,
	};

	struct DistInfo
	{
		ioBaseChar *pChar;
		float fDistSq;

		bool operator<( const DistInfo &rhs ) const
		{
			return fDistSq < rhs.fDistSq;
		}
	};

	typedef std::vector< DistInfo > SortList;

protected:
	//�ִ� ������
	float m_fMaxGauge;
	//�ʿ� ������
	float m_fNeedGauge;
	//������ ������
	float m_fIncreaseGauge;	

	//�ִ� �޺�
	int m_nMaxComboCnt;

	//���� �ð� Ȯ��
	DWORD m_dwAttackStartTime;

	//�浹 ����
	float m_fColRange;
	//�浹 ����
	float m_fColAngle;

	//��� �ð�
	DWORD m_dwSPWaitDuration;
	//DWORD m_dwInputWaitDuration;

	//�ڷ���Ʈ ������
	//float m_fTeleportOffset;
	//���� �Ŀ�
	//float m_fEndJumpPower;

	ioHashStringVec m_AttackReadyAniList;		
	AttributeList	m_vChargeComboAttListCenter;
	
	ioHashString	m_stSuperPowerAttackCenter;
	ioHashString	m_stSuperPowerAttackUp;
	ioHashString	m_stSuperPowerAttackDown;

	ioHashString    m_stSuperPowerAttackCenter_Jump;
	ioHashString	m_stSuperPowerAttackUp_Jump;
	ioHashString	m_stSuperPowerAttackDown_Jump;

	AttackAttribute m_SuperPowerAttack;

	bool m_bSetDownAim;

	ioHashString m_stWeaponEffec1;
	DWORD m_dwEffect1;
	ioHashString m_stWeaponEffec2;
	DWORD m_dwEffect2;

	/*float m_fXScale;
	float m_fYScale;
	float m_fZScale;

	float m_fMaxScale;
	float m_fIncScaleValue;*/

	/*bool  m_bIncXScale;
	bool  m_bIncYScale;
	bool  m_bIncZScale;*/

	ioHashString m_stNormalMeshName;
	ioHashString m_stExtraMeshName;

	bool m_bLastIncWeapon;
	bool m_bItemVisible;

	//////////////////////////////////////////////////////////////////////////
	SuperDimensionExtendDashInfo m_SpecialDashInfo;
	DWORD m_dwDashStartTime;
	D3DXVECTOR3 m_vDashMoveDir;

	DWORD m_dwDashStartEndTime;
	DWORD m_dwDashEndTime;

	float m_fDashAttackMoveRange;
	ioHashString m_GrapplingTargetName;

	DWORD m_dwDashLoopEndTime;
	DWORD m_dwDashEndAttackTime;

	bool  m_bSpecialFireFromJump;
	float m_fSpecialFireEndJumpPower;

protected:
	//���� ������
	float m_fCurGauge;

	WeaponItemState m_WeaponItemState;
	SuperDimensionFireState m_SuperDimensionFireState;
	SortList m_SortList;

	SuperDimensionDashState m_SuperDimensionDashState;

	//�޺� Ƚ�� Ȯ��
	int m_nNormalCombo;
	int m_nCurComboCnt;

	//���� �ε��� ��ȣ
	DWORD m_dwWeaponBaseIndex;

	//�ð� Ȯ��
	DWORD m_dwCheckWaitTime;
	DWORD m_dwCheckInputWaitTime;
	DWORD m_dwComboReserveTime;
	DWORD m_dwComboEndTime;
	DWORD m_dwTeleportAttackEndTime;
	DWORD m_dwTeleportReserveTime;

	//Ÿ�� ����
	bool m_bSetTarget;
	ioHashString m_szTargetName;
	ioHashString m_szFirstTarget;

	//Ÿ�� ����, �Ÿ�
	D3DXVECTOR3 m_vTargetDir;
	D3DXVECTOR3 m_vTargetDist;

	//����Ű Ȯ��
	bool m_bAttackKeyReserved;
	bool m_bDefenseKeyReserved;
	bool m_bJumpKeyReserved;


	std::vector<SuperDimensionHpScaleInfo> m_vHpScaleInfo;
	int m_nHpPercentStep;
	int m_nNowPercentStep;
	int m_nChangedNowStep;

	D3DXVECTOR3 m_vAttackFireDir;

public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ioItem* Clone();
	virtual WeaponSubType GetSubType() const;

public:
	int GetWeaponStep(){ return m_nChangedNowStep; }
	void SetWeaponStep( ioBaseChar* pOwner, int nWeaponStep );
	float GetActualScale();

public:
	//���� ����
	virtual void SetNormalAttack( ioBaseChar *pOwner, int iCurCombo );
	virtual void CheckNormalAttack( ioBaseChar *pOwner );

	//���� ����
	virtual void SetExtraInfoToWeapon( ioWeapon *pWeapon, bool bJumpAttack );
	//������ ���� (������ ȸ��)
	virtual void UpdateExtraData( ioBaseChar *pOwner );

	//����ȭ
	virtual void OnItemGaugeOK( ioBaseChar *pOwner, SP2Packet &rkPacket );
	virtual bool IsChargeAttackState( ioBaseChar *pOwner );

	const ioHashString& GetExtraEffectName();

	virtual void CheckReserve( ioBaseChar *pOwner );

public:
	virtual void OnEquiped( ioBaseChar *pOwner );
	virtual void OnReleased( ioBaseChar *pOwner );

	void ShowWeapon( ioBaseChar *pOwner );
	void HideWeapon( ioBaseChar *pOwner );

	//������ ���� �� ������ ���
	virtual int GetMaxBullet();
	virtual int GetCurBullet();
	virtual void SetCurBullet( int iCurBullet );
	virtual void InitExtraGauge();
	virtual void MaxExtraGauge();
	virtual float GetMaxExtraGauge();
	virtual float GetCurExtraGauge();
	virtual void SetCurExtraGauge( float fGauge );

	virtual void SetVisible( ioBaseChar *pOwner, bool bVisible );

	int GetNeedBullet();
	//������ ���
	void DecreaseGauge( float fNeedGauge );

	virtual void CallItemProcess( ioBaseChar *pOwner );

	void SetCharScale( float fScale, ioBaseChar *pOwner );
	void RemoveCharScale( float fScale, ioBaseChar *pOwner );

protected:
	//���� �б�
	void LoadAttackProperty( ioINILoader &rkLoader );
	//����ȭ
	void RecvSpecialState( ioBaseChar *pOwner, SP2Packet &rkPacket );
	//�ʱ�ȭ
	void Init();
	//��¡ Ȯ��
	void CheckCharging( ioBaseChar *pOwner );
	//�⺻ ���� ���·� ����
	void SetNormalAttackState( ioBaseChar *pOwner );

public:
	//Ư�� ���� �ʱ�ȭ
	void ClearState( ioBaseChar *pOwner );
	//Ÿ�� ����
	//void SetTarget( ioBaseChar *pTarget, ioBaseChar *pAttacker );

	//Ư�� �߻�
	void SetSuperDimensionFireState( ioBaseChar *pOwner, bool bJump = false );

	void SetSuperDimensionDashGrapState( ioBaseChar *pOwner, const ioHashString& stTarget );

	void ProcessSuperDimensionFireState( ioBaseChar *pOwner );
	void ProcessSuperDimensionDashState( ioBaseChar *pOwner );

	bool SetItemGrapplingState( ioBaseChar *pChar, const ioHashString &szTargetName );
	D3DXVECTOR3 GetGrapplingTargetPos( ioBaseChar *pChar );

	//bool IsCharCollisionSkipState();

protected:
	//���� ����� Ÿ�� �˻�
	bool FindTarget( ioBaseChar *pOwner );
	//Ÿ�� ����Ʈ ����
	void SortTargetList( ioBaseChar *pOwner, const BaseCharList &TargetList );
	//���� ������ �������� Ȯ��
	bool IsEnableTarget( ioBaseChar *pTarget );

	bool TargetCheck( ioBaseChar *pOwner );

protected:
	//�ʴɷ�ź ����
	void SetSuperPowerState( ioBaseChar *pOwner );
	

protected:
	//Ű�Է� Ȯ��
	//void CheckInputKey( ioBaseChar *pOwner );
	//ȸ��
	void SetRotateToTarget( ioBaseChar *pOwner );
	//Ű�Է��� �����Ѱ�
	//bool IsEnableInputKey( ioBaseChar *pOwner );
	
	//�������
	//void ReserveInputKey(ioBaseChar *pOwner );
	//void CheckReserveState( ioBaseChar *pOwner );
	//void SetReserveAttack( ioBaseChar *pOwner );
	//void SetReserveDefence( ioBaseChar *pOwner );
	//void SetReserveJump( ioBaseChar *pOwner );


	void ChangeWeaponScale( ioBaseChar *pOwner, float fScale );
	void ProcessSpecialFireTargetTracking( ioBaseChar *pOwner, const ioHashString& stUp, const ioHashString& stCenter, const ioHashString& stDown );
	float GetAimedUpDownWeight( const D3DXVECTOR3 &vAimDir, float fMaxAngle );

public:
	const ioHashString& FindDashAttTarget( ioBaseChar *pOwner );
protected:
	bool IsEnableTargetState( ioBaseChar *pTarget );

	void ChangeSpecialDashMoveState( ioBaseChar *pOwner );
	//void ChangeSpecialDashGrapMoveState( ioBaseChar *pOwner, ioBaseChar *pTarget );
	void ChangeSpecialDashLoopState( ioBaseChar *pOwner, ioBaseChar *pTarget );
	void ChangeSpecialDashLoopAttackState( ioBaseChar *pOwner, ioBaseChar *pTarget, const D3DXVECTOR3& vDir );
	void ProcessSpecailDashAttackMove( ioBaseChar *pOwner );
	void ProcessSpecailDashLoopState( ioBaseChar *pOwner, ioBaseChar *pTarget );

public:
	ioSuperDimensionItem();
	ioSuperDimensionItem( const ioSuperDimensionItem &rhs );
	virtual ~ioSuperDimensionItem();
};

inline ioSuperDimensionItem* ToSuperDimensionItem( ioItem *pItem )
{
	ioWeaponItem *pWeapon = ToWeaponItem( pItem );
	if( !pWeapon || pWeapon->GetSubType() != ioWeaponItem::WST_SUPER_DIMENSION_ITEM )
		return NULL;

	return dynamic_cast< ioSuperDimensionItem* >( pWeapon );
}

//-----------------------------------------------------------------------------------------------------------

class ioSuperDimensionFireSpecialState : public ioSpecialStateBase
{
public:
	virtual void Initialize( ioBaseChar* pOwner, CharState eCurrState );
	virtual void Finalize( ioBaseChar* pOwner, CharState eCurrState, CharState eNewState );

	virtual const char* GetStateName(){ return "CS_SUPER_DIMENSION_FIRE"; }
	virtual const CharState GetState(){ return CS_SUPER_DIMENSION_FIRE; }	
	virtual bool IsSendControlInfoToUser( ioBaseChar* pOwner ){ return false; }
	virtual bool IsLandingEnable(){ return false; }

public:
	virtual void CheckSpecialState( ioBaseChar* pOwner );
	virtual void CheckFallStateChange( ioBaseChar* pOwner, float fMinFallHeightRate, float fBottomHeight, bool bBottomMaxCheck, float fMaxBottomHeight );
	virtual bool ProcessMove( ioBaseChar* pOwner, float fTimePerSec, DWORD dwPreTime, DWORD dwCurTime );
	virtual bool IsNoDropState( ioBaseChar* pOwner ) { return true; }
	virtual void GetGravity( ioBaseChar* pOwner, float &fCurGravity );

public:
	ioSuperDimensionFireSpecialState();
	virtual ~ioSuperDimensionFireSpecialState();
};

inline ioSuperDimensionFireSpecialState* ToSuperDimensionFireSpecialState( ioSpecialStateBase *pState )
{
	if( !pState || pState->GetState() != CS_SUPER_DIMENSION_FIRE )
		return NULL;

	return dynamic_cast< ioSuperDimensionFireSpecialState* >( pState );
}

//-----------------------------------------------------------------------------------------------------------

class ioSuperDimensionDashSpecialState : public ioSpecialStateBase
{
public:
	virtual void Initialize( ioBaseChar* pOwner, CharState eCurrState );
	virtual void Finalize( ioBaseChar* pOwner, CharState eCurrState, CharState eNewState );

	virtual const char* GetStateName(){ return "CS_SUPER_DIMENSION_DASH"; }
	virtual const CharState GetState(){ return CS_SUPER_DIMENSION_DASH; }	
	virtual bool IsSendControlInfoToUser( ioBaseChar* pOwner ){ return false; }
	virtual bool IsLandingEnable(){ return false; }

public:
	virtual void CheckSpecialState( ioBaseChar* pOwner );
	virtual void CheckFallStateChange( ioBaseChar* pOwner, float fMinFallHeightRate, float fBottomHeight, bool bBottomMaxCheck, float fMaxBottomHeight );
	virtual bool ProcessMove( ioBaseChar* pOwner, float fTimePerSec, DWORD dwPreTime, DWORD dwCurTime );

public:
	ioSuperDimensionDashSpecialState();
	virtual ~ioSuperDimensionDashSpecialState();
};

inline ioSuperDimensionDashSpecialState* ToSuperDimensionDashSpecialState( ioSpecialStateBase *pState )
{
	if( !pState || pState->GetState() != CS_SUPER_DIMENSION_DASH )
		return NULL;

	return dynamic_cast< ioSuperDimensionDashSpecialState* >( pState );
}