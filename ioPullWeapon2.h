#pragma once

#include "ioWeapon.h"
#include "ioUserKeyInput.h"

class ioEffect;

class ioPullWeapon2 : public ioWeapon
{
public:
	enum ControlState
	{
		CS_PRE,
		CS_FLOATING,
		CS_PULLING,
		CS_END,
	};

protected:
	ControlState m_State;

	ioOrientBox m_LocalAttackBox;
	D3DXVECTOR3 m_vMoveDir;
	D3DXVECTOR3 m_vStartPos;

	ioEffect *m_pMesh;
	ioHashString m_szMapCollisionEffect;
	
	CEncrypt<DWORD> m_dwMapCollisionDelayTime;
	CEncrypt<DWORD> m_dwCurMapCollisionDelayTime;

	CEncrypt<float> m_fCurMoveRange;

	CEncrypt<float> m_fPullingRange;

	CEncrypt<DWORD> m_dwPullingDurationTime;
	CEncrypt<DWORD> m_dwPullingSensingTime;
	
	DWORD m_dwPullingStartTime;
	DWORD m_dwPullingEndTime;

	ioHashString m_szPullingBuff;

	ioHashStringVec m_vTargetList;

	WeaponInfoList	m_CallWeaponInfoList;
	bool m_bArleadyCallWeapon;

public:
	static ActionStopType m_ActionStopType;
	static ActionStopType m_DieActionStopType;

protected:
	bool SetMesh( const ioHashString &szFileName );	
	void CreateMapCollisionEffect( ioPlayStage* pStage, D3DXVECTOR3 vColPoint );

public:
	virtual bool InitResource();
	virtual void LoadProperty( ioINILoader &rkLoader );

	virtual void LoadCallAttributeList( ioINILoader &rkLoader );

	virtual void SetMoveDir( const D3DXVECTOR3 &vMoveDir );
	virtual void SetStartPosition( const D3DXVECTOR3 &vPos, ioPlayStage *pStage );

	virtual void Process( float fTimePerSec, ioPlayStage *pStage );
	virtual void DestroyResource( ioPlayStage *pStage=NULL );

	virtual void ApplyWeaponSync( SP2Packet &rkPacket, ioPlayStage *pStage );

public:
	virtual const ioOrientBox& GetLocalAttackBox() const;

	virtual WeaponType GetType() const;
	virtual ActionStopType GetActionStopType() const;
	virtual ActionStopType GetDieActionStopType() const;

	virtual bool TestMapCollision( ioOpcodeShape *pShape, ioPlayStage *pStage, const D3DXMATRIX* pShapeWorldMat );
	virtual bool IsCollision( ioPlayEntity *pEntity, bool bMapCollision=false );

 public:
	void OnFloating( float fTimePerSec, DWORD dwCurTime, ioPlayStage *pStage );
	void OnPulling( float fTimePerSec, DWORD dwCurTime, ioPlayStage *pStage );

	void SetPullingState();

	void CheckTargetList( ioBaseChar *pOwner, ioPlayStage *pStage );
	bool IsEnableTargetState( ioBaseChar *pOwner, ioBaseChar *pTarget );
	void SetTargetBuff( ioBaseChar *pOwner, ioPlayStage *pStage );

	void CallAttribute();

protected:
	 float GetMoveSpeed();
	 float GetMoveRange();

protected:
	virtual void UpdateWorldAttackBox( float fTimePerSec, bool bSetNull );

public:
	ioPullWeapon2( ioBaseChar *pOwner, const WeaponAttribute *pAttr );
	virtual ~ioPullWeapon2();
};

inline ioPullWeapon2* ToPullWeapon2( ioWeapon *pWeapon )
{
	if( !pWeapon || pWeapon->GetType() != ioWeapon::WT_PULL_WEAPON2 )
		return NULL;

	return dynamic_cast< ioPullWeapon2* >( pWeapon );
}

