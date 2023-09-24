#pragma once

#include "ioWeaponItem.h"
#include "ioAttackAttribute.h"
#include "ioSpecialStateBase.h"

class ioINILoader;
class ioWeapon;
class ioBaseChar;

class ioPsychoMetry_Item : public ioWeaponItem
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

	enum PsychoSpecialState
	{
		PSS_None,
		PSS_Find_Target,
		PSS_Set_Target,
		PSS_Wait,
		PSS_InputWait,
		PSS_Combo,
		PSS_Teleport,
		PSS_End,
		PSS_End_Jump,
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
	DWORD m_dwInputWaitDuration;

	//�ڷ���Ʈ ������
	float m_fTeleportOffset;
	//���� �Ŀ�
	float m_fEndJumpPower;

	ioHashStringVec m_AttackReadyAniList;		
	AttributeList	m_vChargeComboAttListUp;
	AttributeList	m_vChargeComboAttListCenter;
	AttributeList	m_vChargeComboAttListDown;
	float			m_fChargeComboAttAngle;	

	AttackAttribute m_SuperPowerAttack;
	AttackAttribute m_TeleportAttack;

	bool m_bSetDownAim;
	
	float m_fJumpHeightGap;

protected:
	//���� ������
	float m_fCurGauge;

	WeaponItemState m_WeaponItemState;
	PsychoSpecialState m_PsychoSpecialState;
	SortList m_SortList;

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

public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ioItem* Clone();
	virtual WeaponSubType GetSubType() const;

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

	virtual void SetNextItemState( ioBaseChar *pWoundChar, ioBaseChar *pAttacker, ioWeapon *pWeapon, bool bDefense, float fDamage );

public:
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
	//������ ���
	void DecreaseGauge( float fNeedGauge );
		
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
	//Ư�� ���·� ����
	void SetPsychoSpecialState( ioBaseChar *pOwner );
	//Ư�� ���� ���μ���
	void ProcessPsychoSpecialState( ioBaseChar *pOwner );
	//Ÿ�� ����
	//void SetTarget( ioBaseChar *pTarget, ioBaseChar *pAttacker );

protected:
	//���� ����� Ÿ�� �˻�
	bool FindTarget( ioBaseChar *pOwner );
	//Ÿ�� ����Ʈ ����
	void SortTargetList( ioBaseChar *pOwner, const BaseCharList &TargetList );
	//���� ������ �������� Ȯ��
	bool IsEnableTarget( ioBaseChar *pTarget );
	
protected:
	//�ʴɷ�ź ����
	void SetSuperPowerState( ioBaseChar *pOwner );
	//�������� ����
	void SetEnergyFireState( ioBaseChar *pOwner );
	//�޺� ���
	AttackAttribute* GetCorrectDirAttibute( ioBaseChar *pOwner );
	//�ڷ���Ʈ ����
	void SetTeleportAttackState( ioBaseChar *pOwner );
	//�ڷ���Ʈ �̵�
	void SetTeleportMove( ioBaseChar *pOwner );
	//����� �ڷ���Ʈ �̵�
	void SetDropZoneTeleportMove( ioBaseChar *pOwner );

protected:
	//Ű�Է� Ȯ��
	void CheckInputKey( ioBaseChar *pOwner );
	//ȸ��
	void SetRotateToTarget( ioBaseChar *pOwner );
	//Ű�Է��� �����Ѱ�
	bool IsEnableInputKey( ioBaseChar *pOwner );
	//�ڷ���Ʈ �����Ѱ�
	bool CheckDropZone( ioBaseChar *pOwner );
	//���ִ� �����ΰ�
	bool IsFloatState( ioBaseChar *pOwner );	
	//�������
	void ReserveInputKey(ioBaseChar *pOwner );
	void CheckReserveState( ioBaseChar *pOwner );
	void SetReserveAttack( ioBaseChar *pOwner );
	void SetReserveDefence( ioBaseChar *pOwner );
	void SetReserveJump( ioBaseChar *pOwner );
	
public:
	ioPsychoMetry_Item();
	ioPsychoMetry_Item( const ioPsychoMetry_Item &rhs );
	virtual ~ioPsychoMetry_Item();
};

inline ioPsychoMetry_Item* ToPsychoMetryItem( ioItem *pItem )
{
	ioWeaponItem *pWeapon = ToWeaponItem( pItem );
	if( !pWeapon || pWeapon->GetSubType() != ioWeaponItem::WST_PSYCO_ITEM )
		return NULL;

	return dynamic_cast< ioPsychoMetry_Item* >( pWeapon );
}

//-----------------------------------------------------------------------------------------------------------

class ioPsychoMetrySpecialState : public ioSpecialStateBase
{
public:
	virtual void Initialize( ioBaseChar* pOwner, CharState eCurrState );
	virtual void Finalize( ioBaseChar* pOwner, CharState eCurrState, CharState eNewState );

	virtual const char* GetStateName(){ return "CS_PSYCHO_SPECIAL"; }
	virtual const CharState GetState(){ return CS_PSYCHO_SPECIAL; }	
	virtual bool IsEnableBlowWounded( ioBaseChar *pOwner ){ return false; }

public:
	virtual void CheckSpecialState( ioBaseChar* pOwner );
	virtual void CheckFallStateChange( ioBaseChar* pOwner, float fMinFallHeightRate, float fBottomHeight, bool bBottomMaxCheck, float fMaxBottomHeight );
	virtual bool IsNoDropState( ioBaseChar* pOwner ) { return true; }
	virtual void GetGravity( ioBaseChar* pOwner, float &fCurGravity );

public:
	ioPsychoMetrySpecialState();
	virtual ~ioPsychoMetrySpecialState();
};

inline ioPsychoMetrySpecialState* ToPsychoMetrySpecialState( ioSpecialStateBase *pState )
{
	if( !pState || pState->GetState() != CS_PSYCHO_SPECIAL )
		return NULL;

	return dynamic_cast< ioPsychoMetrySpecialState* >( pState );
}