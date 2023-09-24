//////////////////////////////////////////////////////////////////////////
//�ۼ�:����ö( 2012.9.7 )
//����:������ ���� ����
//����:���߿��� �߻�� �밢�� �Ʒ��� �������� ���鿡 ������ �������� �ʰ� �ٴ��� Ÿ�� ����.(���� ���̾� ������. �ٴ�->�̿���)


#pragma once


#include "ioWeapon.h"

class ioEffect;

class ioBombMissileWeapon3 : public ioWeapon
{
protected:
	enum BombState
	{
		BS_FLOATING_AIR,
		BS_FLOATING_LAND,
		BS_WAIT_CLOSE,
		BS_CLOSE,
		BS_EXPLOSION_END,
	};

	enum MapHeightCheckType
	{
		MHCT_BOUNDBOX,
		MHCT_POSITION,
	};

	BombState m_BombState;
	MapHeightCheckType m_MapHeightCheckType;

	ioOrientBox m_LocalAttackBox;
	D3DXVECTOR3 m_vMoveDir;

	ioEffect *m_pBombMesh;
	ioHashString m_BoomEffect;
	float m_fExplosionRange;

	ioHashString m_szWeaponEffect_Blue;
	ioHashString m_szWeaponEffect_Red;


	bool m_bFirst;
	bool m_bEndExplosion;
	bool m_bCallExplosion;
	bool m_bEnableMapCollision;
	bool m_bDsiableChangeDir;

	float m_fCurMoveSpeed;
	float m_fCurMoveRange;

	ChangeDirInfoList m_ChangeDirInfoList;

	DWORD m_dwEndEffectID;

	//
	OwnerRotType m_OwnerRotate;
	DWORD m_dwRotateTime;
	float m_fRotateEndRange;

	//
	float m_fStartSpeed;
	float m_fMaxSpeed;
	float m_fAccelSpeed;

	//Call Attribute
	WeaponInfoList	m_CallWeaponInfoList;
	bool m_bArleadyCallWeapon;

	float m_fDefaultGapHeight;
	float m_fEnableMoveGap;

	bool			m_bSetGraceClose;
	bool			m_bSetForceClose;

	bool m_bSetOreintationFloatingAir;

public:
	static ActionStopType m_ActionStopType;
	static ActionStopType m_DieActionStopType;

protected:
	bool SetBombMesh( const ioHashString &szFileName );
	void OnFloating( float fTimePerSec, ioPlayStage *pStage );
	void OnClosing( float fTimePerSec, ioPlayStage *pStag );

	void CheckOwnerRotateDir( float fTimePerSec );
	float CheckCurSpeed( float fTimePerSec );

	void SetGraceClose();

public:
	void CheckChangeDir();
	void RotateMoveDir( float fAngleH, float fAngleV );
	float GetMoveSpeed();
	float GetMaxRange();

	bool IsFloating() const;
	bool IsExplosion() const;

	void Explosion( ioPlayStage *pStage );
	void LoadCallAttributeList( ioINILoader &rkLoader );
	void CallAttribute();

public:
	virtual bool InitResource();
	virtual void LoadProperty( ioINILoader &rkLoader );

	virtual void SetMoveDir( const D3DXVECTOR3 &vMoveDir );
	virtual void SetStartPosition( const D3DXVECTOR3 &vPos, ioPlayStage *pStage );

	virtual void Process( float fTimePerSec, ioPlayStage *pStage );
	virtual void DestroyResource( ioPlayStage *pStage=NULL );

	virtual bool IsCanMineExplosion();

public:
	virtual const ioOrientBox& GetLocalAttackBox() const;

	virtual WeaponType GetType() const;
	virtual ActionStopType GetActionStopType() const;
	virtual ActionStopType GetDieActionStopType() const;

	virtual bool IsCollision( ioPlayEntity *pEntity, bool bMapCollision=false );
	virtual void CheckPreCollision( ioPlayEntity *pTarget, ioPlayStage *pStage );
	virtual DWORD GetCollisionEnableTime() const;

	virtual bool TestMapCollision( ioOpcodeShape *pShape, ioPlayStage *pStage, const D3DXMATRIX* pShapeWorldMat );

	virtual void SetOwnerChargeScaleRate( float fRate );

protected:
	virtual void UpdateWorldAttackBox( float fTimePerSec, bool bSetNull );

public:
	void SetDsiableChangeDir( bool bDisable ){ m_bDsiableChangeDir = bDisable; }

public:
	ioBombMissileWeapon3( ioBaseChar *pOwner, const WeaponAttribute *pAttr );
	virtual ~ioBombMissileWeapon3();
};

inline ioBombMissileWeapon3* ToBombMissileWeapon3( ioWeapon *pWeapon )
{
	if( !pWeapon || pWeapon->GetType() != ioWeapon::WT_BOMB_MISSILE3 )
		return NULL;

	return dynamic_cast< ioBombMissileWeapon3* >( pWeapon );
}

