#pragma once

#include "ioObjectItem.h"

class ioWeapon;

class ioExcavationItem : public ioObjectItem
{
public:
	enum AniType
	{
		AT_NONE            = 0,
		AT_EQUIP           = 1,
		AT_RELEASE         = 2,
		AT_EXCAVATING_STRT = 3,
		AT_EXCAVATING      = 4,
		AT_RESULT_FAIL     = 5,
		AT_RESULT_SUCCESS  = 6,
		AT_RESULT_EQUIP    = 7,
	};

protected:
	ioHashString m_sEquipAnimation;
	ioHashString m_sReleaseAnimation;
	ioHashString m_sExcavatingStartAnimation;
	ioHashString m_sExcavatingAnimation;
	ioHashString m_sResultFailAnimation;
	ioHashString m_sResultSuccessAnimation;
	ioHashString m_sExcavatingToolMesh;
	
	float        m_fEquipAnimationRate;
	float        m_fReleaseAnimationRate;
	float        m_fExcavatingStartAnimationRate;
	float        m_fExcavatingAnimationRate;
	float        m_fResultFailAnimationRate;
	float        m_fResultSuccessAnimationRate;

	DWORD        m_dwAnimationEndTime;
	
	AniType      m_eAniType;
	bool         m_bExcavatingToolMesh;

	DWORD        m_dwEquipTime;
	
protected:
	bool SetActionAni( ioBaseChar * pOwner, ioHashString &rszAnimation, float fAnimationRate, bool bLoopAni = false );

public:
	void SetExcavatingStartAni( ioBaseChar *pOwner );
	void SetExcavatingAni( ioBaseChar *pOwner );
	void SetResultFailAni( ioBaseChar *pOwner );
	void SetResultSuccessAni( ioBaseChar *pOwner );
	void SetResultEquipAni( ioBaseChar *pOwner );
	void SetReleaseAni( ioBaseChar *pOwner );

	void SetExcavatingToolMesh( ioBaseChar *pOwner, bool bSet );

	bool IsAnimationEndTime( ioBaseChar *pChar );
	void ClearAniInfo();
	
	ioExcavationItem::AniType GetAniType() const { return m_eAniType; }
	DWORD GetAnimationEndTime() const { return m_dwAnimationEndTime; }
	DWORD GetEquipTime() const { return m_dwEquipTime; }

public:	// overide ioItem
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ioItem* Clone();
	virtual void SetNormalAttack( ioBaseChar *pOwner, int iCurCombo );

public:
	virtual ObjectSubType GetObjectSubType() const;

public:
	virtual void OnEquiped( ioBaseChar *pOwner );

public:
	ioExcavationItem(void);
	ioExcavationItem( const ioExcavationItem &rhs );
	virtual ~ioExcavationItem(void);
};

inline ioExcavationItem* ToExcavationItem( ioItem *pItem )
{
	ioObjectItem *pObject = ToObjectItem( pItem );
	if( !pObject || pObject->GetObjectSubType() != ioObjectItem::OST_EXCAVATION )
		return NULL;

	return dynamic_cast< ioExcavationItem* >( pObject );
}


