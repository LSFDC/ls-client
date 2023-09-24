#pragma once

#include "ioBuff.h"

class ioBaseChar;
class ioINILoader;
class ioEffect;

class ioTargetCreateWeaponBuff : public ioBuff
{
public:
	enum TargetBuffState
	{
		TBS_None,
		TBS_FindTarget,
		TBS_CreateWeapon,
	};

protected:
	TargetBuffState m_BuffState;

	//������ ����
	ioHashStringVec m_RemoveBuffList;
	//�� ���� ���� Ƚ��
	CEncrypt<int> m_nTotalWeaponCnt;
	//���� ���� ��� Ƚ��
	CEncrypt<int> m_nCurWeaponCnt;
	//���� �ð�
	DWORD m_dwReloadFireTime;
	DWORD m_dwCheckFireTime;
	//��ũ ����
	float m_fMakerShowRange;
	//��ũ ���̵�
	UniqueObjID m_dwReadyMakerID;
	UniqueObjID m_dwAimMakerID;
	UniqueObjID m_dwZeroMakerID;
	//��ũ �̸�
	ioHashString m_szRedReadyMarker;
	ioHashString m_szBlueReadyMarker;
	ioHashString m_szRedAimMarker;
	ioHashString m_szBlueAimMarker;
	ioHashString m_szRedZeroMarker;
	ioHashString m_szBlueZeroMarker;
	//�غ� ��ũ ����
	bool m_bSetReadyMarkVisible;
	//��ũ �ð� ����
	DWORD m_dwAimTime;
	DWORD m_dwCheckAimTime;
	DWORD m_dwZeroTime;
	DWORD m_dwCheckZeroTime;
	//��ũ �� ����
	bool m_bAimFire;
	bool m_bZeroFire;
	
	int m_nAimWeapon;
	int m_nZeroWeapon;
	//���� �ִϸ��̼�
	ioHashString m_szAimAttackAni;
	float m_fAimAttackRate;
	//���� ���� �ִϸ��̼�
	ioHashString m_szJumpAttackAni;
	float m_fJumpAttackRate;
	DWORD m_dwAttackEndTime;
	//Ÿ�� �˻� ����
	float m_fTargetAngle;
	float m_fTargetMaxRange;
	float m_fTargetMinRange;

	ioHashString m_AimedTarget;
	ioHashString m_NewAimedTarget;
	ioHashString m_AttackTarget;
	bool m_bEnableDownAim;

	DWORD m_dwFireTime;
	bool m_bSetFire;
	bool m_bWound;
	bool m_bIsJumpAttack;

	CEncrypt<bool> m_bEnableWeaponSkill;
	CEncrypt<bool> m_bEnableArmorSkill;
	CEncrypt<bool> m_bEnableHelmetSkill;
	CEncrypt<bool> m_bEnableCloakSkill;

	bool m_bSetAfterJump;
	float m_fGravityAmt;
	float m_fJumpPower;
	
public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ioBuff* Clone();

public:
	virtual void StartBuff( ioBaseChar *pOwner );
	virtual bool ApplyStartBuff( ioBaseChar *pOwner, DWORD dwGlobalTime, DWORD dwGapTime );
	virtual void ProcessBuff( float fTimePerSec );
	virtual void EndBuff();
	virtual void ApplyExtraBuffInfo( SP2Packet &rkPacket );

	bool CheckEnableUseSkill( int iSlot );

protected:
	//�ʱ�ȭ
	void Init();
	//�߰� �б�
	void LoadTargetProperty( ioINILoader &rkLoader );
	//���� Ȯ��
	bool CheckWoundState();
	//Ÿ�� �˻�
	void CheckFindTarget( ioBaseChar *pOwner );
	//���ΰ� Ȯ��
	bool CompareAimedTarget( ioBaseChar *pOwner );
	//Ű�Է� Ȯ��
	void CheckInputKey( ioBaseChar *pOwner );
	//�ִϸ��̼� �ð� ���
	void SetAttackTime( ioBaseChar *pOwner, ioHashString szName, float fRate );
	//���� ����
	void CreateWeapon( ioBaseChar *pOwner, int nWeaponIndex, ioHashString TargetName );
	//���� ���� ����
	void CheckAttack( ioBaseChar *pOwner );
	//�߰� ����
	void CheckJumpAttackState( ioBaseChar *pOwner );

protected:
	//������ ���� �� ����
	void CreateReadyMarker( ioBaseChar *pOwner );
	void UpdateReadyMarker( ioBaseChar *pOwner );
	void CreateAimMarker( ioBaseChar *pOwner, ioHashString TargetName );
	void UpdateAimMarker( ioBaseChar *pOwner );
	void CreateZeroMarker( ioBaseChar *pOwner, ioHashString TargetName );
	void UpdateZeroMarker( ioBaseChar *pOwner );
	void DestroyMarker( ioBaseChar *pOwner, DWORD &dwMarkID );

public:
	ioTargetCreateWeaponBuff();
	ioTargetCreateWeaponBuff( const ioTargetCreateWeaponBuff &rhs );
	virtual ~ioTargetCreateWeaponBuff();
};

inline ioTargetCreateWeaponBuff* ToTargetCreateWeaponBuff( ioBuff *pBuff )
{
	if( !pBuff || pBuff->GetTypeID() != BT_TARGET_WEAPON )
		return NULL;

	return dynamic_cast< ioTargetCreateWeaponBuff* >( pBuff );
}