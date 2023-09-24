#pragma once

#include "ioWeapon.h"
#include "ioUserKeyInput.h"

class ioEffect;

class ioBombMissileWeapon2 : public ioWeapon
{
public:
	enum BombState
	{
		BS_FLOATING,
		BS_EXPLOSION,
		BS_EXPLOSION_END,
	};

	enum StateSync
	{
		SSC_CHANGE_MOVE,
	};

protected:
	BombState m_BombState;

	ioOrientBox m_LocalAttackBox;
	D3DXVECTOR3 m_vMoveDir;

	ioEffect *m_pBombMesh;
	ioHashString m_BoomEffect;
	float m_fExplosionRange;

	bool m_bFirst;
	float m_fCurMoveSpeed;

	ioUserKeyInput::DirKeyInput m_CurDirKey;
	
	bool m_bTargetRot;
	DWORD m_dwRotateSpeed;

	bool m_bSetUpKey;
	bool m_bSetDownKey;
	float m_fUpDownSpeed;

	CEncrypt<bool>	m_bDisableUpDown;

	CEncrypt<bool>	m_bPlayWoundSound;
	CEncrypt<bool>	m_bDisableMapCollision;

public:
	static ActionStopType m_ActionStopType;
	static ActionStopType m_DieActionStopType;

public:
	bool IsFloating() const;
	bool IsExplosion() const;

	void Explosion( ioPlayStage *pStage );

protected:
	bool SetBombMesh( const ioHashString &szFileName );
	void OnFloating( float fTimePerSec, ioPlayStage *pStage );

	float GetMoveSpeed();

	void CheckMoveKey();
	void CheckMoveDir();

public:
	virtual bool InitResource();
	virtual void LoadProperty( ioINILoader &rkLoader );

	virtual void SetLiveTime( DWORD dwTime );
	virtual void SetMoveDir( const D3DXVECTOR3 &vMoveDir );
	virtual void SetStartPosition( const D3DXVECTOR3 &vPos, ioPlayStage *pStage );
	virtual void PlayWoundedSound( ioPlayEntity *pEntity );

	virtual void Process( float fTimePerSec, ioPlayStage *pStage );
	virtual void DestroyResource( ioPlayStage *pStage=NULL );

	virtual bool IsCanMineExplosion();

	virtual void ApplyWeaponSync( SP2Packet &rkPacket, ioPlayStage *pStage );

public:
	virtual const ioOrientBox& GetLocalAttackBox() const;
	
	virtual WeaponType GetType() const;
	virtual ActionStopType GetActionStopType() const;
	virtual ActionStopType GetDieActionStopType() const;

	virtual bool IsCollision( ioPlayEntity *pEntity, bool bMapCollision=false );
	virtual void CheckPreCollision( ioPlayEntity *pTarget, ioPlayStage *pStage );
	virtual DWORD GetCollisionEnableTime() const;

	virtual bool TestMapCollision( ioOpcodeShape *pShape, ioPlayStage *pStage, const D3DXMATRIX* pShapeWorldMat );

protected:
	virtual void UpdateWorldAttackBox( float fTimePerSec, bool bSetNull );

public:
	ioBombMissileWeapon2( ioBaseChar *pOwner, const WeaponAttribute *pAttr );
	virtual ~ioBombMissileWeapon2();
};

inline ioBombMissileWeapon2* ToBombMissileWeapon2( ioWeapon *pWeapon )
{
	if( !pWeapon || pWeapon->GetType() != ioWeapon::WT_BOMB_MISSILE2 )
		return NULL;

	return dynamic_cast< ioBombMissileWeapon2* >( pWeapon );
}

