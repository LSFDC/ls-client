#pragma once


#include "ioNormalSkill.h"


class ioAreaWeaponControlSkill : public ioNormalSkill
{
public:
	enum ControlState
	{
		RS_NONE,
		RS_ACTION,
		RS_LOOP,
		RS_END,
	};

	enum AreaWeaponState
	{
		AS_DELAY,
		AS_MOVE,
	};

	enum SyncState
	{
		SS_CREATE,
		SS_UPDATE,
	};
	
protected:
	ControlState m_ControlState;
	AreaWeaponState m_AreaWeaponState;

	DWORD m_dwLoopProtectDuration;
	DWORD m_dwProtectTime;

	// Animation
	ioHashString m_LoopAnimation;
	DWORD m_dwLoopStartTime;
	DWORD m_dwLoopTime;

	ioHashString m_EndAnimation;
	float m_fEndAniRate;
	DWORD m_dwEndAniStartTime;
	DWORD m_dwEndAniEndTime;

	// HandMesh
	ioHashString m_StartHandMesh;
	ioHashString m_LoopHandMesh;
	ioHashString m_EndHandMesh;

	// MagicCircle
	float m_fOffSet;
	
	ioHashString m_RedCircle;
	ioHashString m_BlueCircle;
	UniqueObjID m_dwCurMapEffect;
	
	DWORD m_dwCircleDuration;
	DWORD m_dwCircleEnd;
	D3DXVECTOR3 m_vCreatePos;

	// AreaWeapon
	ioHashString m_AreaWeaponName;

	DWORD m_dwAreaWeaponIndex;

	DWORD m_dwCreateAreaWeaponTime;

	float m_fMoveSpeed;
	float m_fMoveRange;
	D3DXVECTOR3 m_vMoveDir;

public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ioSkill* Clone();

public:
	virtual bool IsProtected( int iDefenseBreakType ) const;
	virtual bool IsCanUseSkill( ioBaseChar *pChar, bool bHelpMsg );
	virtual bool IsSkillMotionEnd( DWORD dwActionStop, ioBaseChar* pOwner ) const;

public:
	virtual bool OnSkillStart( ioBaseChar *pChar );
	virtual void OnProcessState( ioBaseChar *pChar, ioPlayStage *pStage );
	virtual void OnSkillEnd( ioBaseChar *pChar );

	virtual void StartPreDelayEffect( ioBaseChar *pChar );
	virtual void EndPreDelayEffect( ioBaseChar *pChar );

	virtual void ApplyExtraSkillInfo( ioBaseChar *pChar, ioPlayStage *pStage, SP2Packet &rkPacket );

	virtual bool CheckUseItemMesh();

protected:
	void SetActionState( ioBaseChar *pChar );
	void SetLoopState( ioBaseChar *pChar, ioPlayStage *pStage );
	void SetEndState( ioBaseChar *pChar );

	void CheckControl( ioBaseChar *pChar );
	void ProcessControl( ioBaseChar *pCChar, ioPlayStage *pStage );

	void CreateAreaWeapon( ioBaseChar *pChar, ioPlayStage *pStage );
	void DestroyAreaWeapon();

public:
	bool CheckAreaWeapon();
	const D3DXVECTOR3& GetAreaWeaponPos();
	const D3DXVECTOR3& GetAreaWeaponPrePos();

public:
	ioAreaWeaponControlSkill();
	ioAreaWeaponControlSkill( const ioAreaWeaponControlSkill &rhs );
	virtual ~ioAreaWeaponControlSkill();
};

inline ioAreaWeaponControlSkill* ToAreaWeaponControlSkill( ioSkill *pSkill )
{
	ioNormalSkill *pNormal = ToNormalSkill( pSkill );
	if( !pNormal || pNormal->GetSubSkillType() != NST_AREA_CONTROL )
		return NULL;

	return dynamic_cast< ioAreaWeaponControlSkill* >( pSkill );
}

