#pragma once
#include "iobuff.h"
class ioTargetRushAttackBuff :
	public ioBuff
{
protected:
	enum Buff_State
	{
		BS_NONE,
		BS_SERACH,
		BS_ACTION,
		BS_MOVE_END,
		BS_SHOOT,
		BS_END
	};

	Buff_State m_BuffState;
	bool	   m_bStart;
	//Key Reserve Time
	DWORD		m_dwEnableAttackTime;
	DWORD		m_dwAttackReserveTime;
	DWORD		m_dwEnableAttackCancelTime;
	DWORD		m_dwAttackCancelReserveTime;
	
	//������ ����
	ioHashStringVec m_RemoveBuffList;

	//�Ÿ�
	float	m_fMaxMoveDist;
	float	m_fCurMoveDist;
	float	m_fRunSpeed;
	float   m_fReduceGauge;

	//Ƚ��
	int		m_iAttackMaxCount;
	int		m_iAttackCurCount;

	//Animation
	ioHashString m_szMoveAttackMotion;
	float m_fMoveAttackMotionRate;

	//Marker ���� ����
	float m_fMarkerMinRange;
	float m_fMarkerMaxRange;
	float m_fMarkerAngle;
	bool  m_bEnableDownAim;
	ioHashString m_szBlueMarker;
	ioHashString m_szRedMarker;

	//Target
	BaseCharDistanceSqList m_vTargetInfoList;
	ioHashString m_szTargetName;
	D3DXVECTOR3 m_vTargetDir;
	D3DXVECTOR3 m_vAttackDir;
	bool		m_bChangeChar;

	//��ų ��� ���� ����
	bool m_bEnableWeaponSkill;
	bool m_bEnableArmorSkill;
	bool m_bEnableHelmetSkill;
	bool m_bEnableCloakSkill;

	//ĳ���Ͱ� Wound�������� �ƴ��� üũ�ϴ� ����
	bool	m_bWound;

	//SKillEndJumpState�� jump_power
	float m_fJumpPower;

	//weapon ����
	WeaponInfoList m_vWeaponInfoList;
	DWORD		   m_dwFireTime;
	DWORD		   m_dwFireWeaponIndex;
	DWORD		   m_dwWeaponBaseIndex;

	//Move End
	ioHashString	m_szMoveEndMotion;
	float			m_fMoveEndMotionRate;
	DWORD			m_dwMoveEndMotionEndTime;
	float			m_fMoveEndAttackMoveSpeed;
	DWORD			m_dwMoveEndAttackLiveTime;

	bool			m_bShootWeapon;
	IntVec m_EnableInputKeyState;
	IntVec m_BuffEndState;

public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ioBuff* Clone();
	virtual void ApplyExtraBuffInfo( SP2Packet &rkPacket );

public:
	virtual void StartBuff( ioBaseChar *pOwner );
	virtual bool ApplyStartBuff( ioBaseChar *pOwner, DWORD dwGlobalTime, DWORD dwGapTime );
	virtual void ProcessBuff( float fTimePerSec );
	virtual void EndBuff();
public:
	bool CheckEnableUseSkill( int iSlot );
	//������
	void ProcessRushAttack( float fTimePerSec );
	void UseMoveEndAttack( ioBaseChar *pWounder );
	bool IsCharCollisionSkipState();
protected:
	//�ʱ�ȭ
	void Init();
	//TargetMarker����
	void CreateMarker( ioBaseChar *pOwner );
	
	//Target �˻� state
	void PorcessTargetFindState( ioBaseChar *pOwner );
	//sŰ �Է½� Ÿ�� ��ȯ
	void ChangeTarget( ioBaseChar *pOwner );
	
	//weapon �߻�
	void AttackWeaponFire( ioBaseChar *pOwner );
	void ShootWeapon( ioBaseChar *pOwner );
	void DestroyWeapon( ioBaseChar *pOwner );

	//���� Ȯ��
	bool CheckWoundState();
	//Ÿ�� �˻�
	void CheckFindTarget( ioBaseChar *pOwner );
	bool CheckTargetValidate( ioBaseChar *pOwner, ioHashString szTarget );
	//move collisionüũ
	bool CheckMove( ioBaseChar *pOwner );
	//weapon check
	bool CheckZoneEffectWeapon( ioBaseChar *pOwner );

	//Ÿ�ٹ������� ȸ��
	void SetRotation( ioBaseChar *pOwner );
	//moveattack state�� ��ȯ
	void SetMoveAttackState( ioBaseChar *pOwner );
	//serach state�� ��ȯ
	void SetSerachState( ioBaseChar *pOwner );
	void SetMoveEndState( ioBaseChar *pOwner );
	bool CheckEnableKey( ioBaseChar *pOwner );
	bool CheckBuffEndState( ioBaseChar *pOwner );
	
public:
	ioTargetRushAttackBuff(void);
	ioTargetRushAttackBuff( const ioTargetRushAttackBuff &rhs );
	~ioTargetRushAttackBuff(void);
};

inline ioTargetRushAttackBuff* ToTargetRushAttackBuff( ioBuff *pBuff )
{
	if( !pBuff || pBuff->GetTypeID() != BT_TARGET_RUSH_ATTACK )
		return NULL;

	return dynamic_cast< ioTargetRushAttackBuff* >( pBuff );
}