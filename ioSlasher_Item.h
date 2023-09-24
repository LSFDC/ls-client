#pragma once

#include "ioWeaponItem.h"
#include "ioAttackAttribute.h"
#include "ioSpecialStateBase.h"

class ioINILoader;
class ioWeapon;
class ioBaseChar;

class ioSlasher_Item : public ioWeaponItem
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

	enum SlasherSpecialState
	{
		SSS_None,
		SSS_Find_Target,
		SSS_Char_Rot,
		SSS_Run_Ready,
		SSS_Run_Loop,
		SSS_Run_End,
		SSS_Combo_Attack,
		SSS_Combo_Check,
		SSS_Create_Attach_Weapon,
		SSS_Second_Run,
		SSS_End,
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
	
	//���� �ð� Ȯ��
	DWORD m_dwAttackStartTime;
	
	//�浹 ����
	float m_fColRange;
	//�浹 ����
	float m_fColAngle;
	
	ioHashStringVec m_AttackReadyAniList;	

	bool m_bSetDownAim;
	
	//�޸��� �ɼ�
	SlasherRunInfo m_RunInfo;
	
	AttributeList m_vChargeComboAttList;

	DWORD m_dwSecondChargeTime;

	bool m_bSetAutoAttack;

protected:
	//���� ������
	float m_fCurGauge;

	WeaponItemState m_WeaponItemState;
	SlasherSpecialState m_SlasherSpecialState;
	SortList m_SortList;

	//�޺� Ƚ�� Ȯ��
	int m_nNormalCombo;

	//���� �ε��� ��ȣ
	DWORD m_dwWeaponBaseIndex;
	WeaponInfo m_sCurWeaponInfo;

	//�޸��� �ִϸ��̼�
	ioHashString m_szCurRunReadyAni;
	float m_fCurRunReadyAniRate;
	ioHashString m_szCurRunLoopAni;
	ioHashString m_szCurRunEndAni;
	float m_fCurRunEndAniRate;

	//�޸��� �ɼ�
	float m_fCurRunSpeed;
	DWORD m_dwCurRunTime;
	float m_fCurMaxMoveDist;

	SlasherRunInfo m_CurRunInfo;

	//����Ű Ȯ��
	bool m_bAttackKeyReserved;
	bool m_bDefenseKeyReserved;
	bool m_bJumpKeyReserved;		

	DWORD m_dwRunReadyMotionEndTime;
	DWORD m_dwRunEndMotionEndTime;
	DWORD m_dwComboEndMotionEndTime;

	DWORD m_dwCheckRunTime;
	float m_fCurMoveDist;

	DWORD m_dwComboReserveTime;

	int m_nCurCombo;
	int m_nMaxCombo;

	ioUserKeyInput::DirKeyInput m_CurDirKey;

	bool m_bIsSecondCharge;

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
	//�浹 ����
	bool IsSlasherSpecialCharCollisionSkipState();
	//���� ����
	virtual void SetNextItemState( ioBaseChar *pWoundChar, ioBaseChar *pAttacker, ioWeapon *pWeapon, bool bDefense, float fDamage );
	//������ ���� ����
	bool IsEnableGauge();

	//�ʱ�ȭ
	void Init();

protected:
	//���� �б�
	void LoadAttackProperty( ioINILoader &rkLoader );
	//����ȭ
	void RecvSpecialState( ioBaseChar *pOwner, SP2Packet &rkPacket );
	//��¡ Ȯ��
	void CheckCharging( ioBaseChar *pOwner );
	//�⺻ ���� ���·� ����
	void SetNormalAttackState( ioBaseChar *pOwner );

public:
	//Ư�� ���� �ʱ�ȭ
	void ClearState( ioBaseChar *pOwner );
	//Ư�� ���·� ����
	void SetSlasherSpecialState( ioBaseChar *pOwner );
	//Ư�� ���� ���μ���
	void ProcessSlasherSpecialState( ioBaseChar *pOwner );
	void SetSpecialState( SlasherSpecialState eType );

protected:
	//���� ����� Ÿ�� �˻�
	bool FindTarget( ioBaseChar *pOwner );
	//Ÿ�� ����Ʈ ����
	void SortTargetList( ioBaseChar *pOwner, const BaseCharList &TargetList );
	//���� ������ �������� Ȯ��
	bool IsEnableTarget( ioBaseChar *pTarget );	

protected:
	void SetRotateState( ioBaseChar *pOwner );
	void SetRunReadyState( ioBaseChar *pOwner );
	void SetRunLoopState( ioBaseChar *pOwner );
	void SetRunEndState( ioBaseChar *pOwner );
	void SetComboAttackState( ioBaseChar *pOwner );
	void SetSecondRunState( ioBaseChar *pOwner );

protected:
	//�̵�
	void CheckMove( ioBaseChar *pOwner );
	void CreateAttachWeapon( ioBaseChar *pOwner );
	bool CheckComboInput( ioBaseChar *pOwner );
	void SetCharRotate( ioBaseChar *pOwner );

protected:
	//�������
	void ReserveInputKey(ioBaseChar *pOwner );
	void CheckReserveState( ioBaseChar *pOwner );
	void SetReserveAttack( ioBaseChar *pOwner );
	void SetReserveDefence( ioBaseChar *pOwner );
	void SetReserveJump( ioBaseChar *pOwner );

public:
	void SetCurRunInfo( const SlasherRunInfo& RunInfo, bool bFirst );
	
public:
	ioSlasher_Item();
	ioSlasher_Item( const ioSlasher_Item &rhs );
	virtual ~ioSlasher_Item();
};

inline ioSlasher_Item* ToSlasherItem( ioItem *pItem )
{
	ioWeaponItem *pWeapon = ToWeaponItem( pItem );
	if( !pWeapon || pWeapon->GetSubType() != ioWeaponItem::WST_SLASHER_ITEM )
		return NULL;

	return dynamic_cast< ioSlasher_Item* >( pWeapon );
}

//-----------------------------------------------------------------------------------------------------------

class ioSlasherSpecialState : public ioSpecialStateBase
{
public:
	virtual void Initialize( ioBaseChar* pOwner, CharState eCurrState );
	virtual void Finalize( ioBaseChar* pOwner, CharState eCurrState, CharState eNewState );

	virtual const char* GetStateName(){ return "CS_SLASHER_SPECIAL"; }
	virtual const CharState GetState(){ return CS_SLASHER_SPECIAL; }	

public:
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
	
public:
	ioSlasherSpecialState();
	virtual ~ioSlasherSpecialState();
};

inline ioSlasherSpecialState* ToSlasherSpecialState( ioSpecialStateBase *pState )
{
	if( !pState || pState->GetState() != CS_SLASHER_SPECIAL )
		return NULL;

	return dynamic_cast< ioSlasherSpecialState* >( pState );
}