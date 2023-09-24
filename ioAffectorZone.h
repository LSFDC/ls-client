#pragma once


#include "ioPlayEntity.h"

class ioWeapon;
class ioPlayMode;
class ioINILoader;

class ioZoneAffector;

class ioAffectorZone : public ioPlayEntity
{
protected:
	ioZoneAffector *m_pAffector;
	int m_iCreateIndex;

	// ȿ���� ������� ��ƼƼ��, �ٸ������� �����Ǿ������� �����Ƿ� �̰ɰ����� ���Ϸ���� ���� ����
	typedef std::set< const ioPlayEntity* > AppliedEntitySet;
	AppliedEntitySet m_AppliedEntitySet;

public:
	inline int GetCreateIndex() const { return m_iCreateIndex; }

// for ioGameEntity-----------------------------
public:
	virtual void Update( float fTimePerSec, DWORD dwPreTime, DWORD dwCurTime );
	virtual void UpdateAfter( float fFrameGap );

	virtual bool IsMountingAvailable() const;

protected:
	virtual void UpdateBottomHeight() const;

// for ioPlayEntity-----------------------------
public:
	virtual ApplyWeaponExp ApplyWeapon( ioWeapon *pWeapon );
	virtual bool IsCollisionAvailableWeapon( ioWeapon *pWeapon );
	virtual bool IsEntityCollisionSkipState( ioGameEntity* pColEntity ) const;
	virtual bool IsWoundedCollision( const ioOrientBox &rkAttackBox, D3DXVECTOR3 *pColPoint = NULL ) const;

	virtual PlaySubType GetSubType() const;
	virtual ArmorType GetArmorType() const;

public:
	virtual int  DontMoveEntityLevel() const;
	virtual void LoadProperty( ioINILoader &rkLoader );

//----------------------------------------------

protected:
	void ApplyAffector( ioPlayEntity *pEntity );

public:
	ioAffectorZone( ioEntityGroup *pGrp, ioPlayMode *pMode, int iCreateIndex );
	virtual ~ioAffectorZone();
};

inline ioAffectorZone* ToAffectorZone( ioGameEntity *pObj )
{
	ioPlayEntity *pPlay = ToPlayEntity( pObj );
	if( !pPlay || pPlay->GetSubType() != ioPlayEntity::PST_AFFECTOR_ZONE )
		return NULL;

	return dynamic_cast< ioAffectorZone* >( pPlay );
}

inline const ioAffectorZone* ToAffectorZoneConst( ioGameEntity *pObj )
{
	const ioPlayEntity *pPlay = ToPlayEntity( pObj );
	if( !pPlay || pPlay->GetSubType() != ioPlayEntity::PST_AFFECTOR_ZONE )
		return NULL;

	return dynamic_cast< const ioAffectorZone* >( pPlay );
}

