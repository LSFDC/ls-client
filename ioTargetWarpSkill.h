#pragma once

#include "ioAttackSkill.h"
#include "ioAttackAttribute.h"

class ioINILoader;
class ioBaseChar;
class ioItem;
class SP2Packet;

class ioTargetWarpSkill : public ioAttackSkill
{
public:
	enum SkillState
	{
		SS_NONE,
		SS_FIRE,
		SS_WARP,
		SS_END,
	};

protected:
	SkillState m_SkillState;

	// Warp motion
	ioHashString m_WarpMotion;
	float m_fWarpMotionRate;

	// warp offset
	float m_fWarpOffsetHeight;

public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ioSkill* Clone();

public:
	virtual bool OnSkillStart( ioBaseChar *pChar );
	virtual void OnProcessState( ioBaseChar *pChar, ioPlayStage *pStage );
	virtual void OnSkillEnd( ioBaseChar *pChar );

	virtual bool CheckUseItemMesh();

	virtual void ApplyExtraSkillInfo( ioBaseChar *pChar, ioPlayStage *pStage, SP2Packet &rkPacket );
	virtual void ApplyAttributeToWeapon( ioWeapon *pWeapon, ioPlayStage *pStage );

	virtual bool IsProtected( int iDefenseBreakType ) const;
	virtual bool IsSkillMotionEnd( DWORD dwActionStop, ioBaseChar* pOwner ) const;

	virtual bool IsEndState() const;

protected:
	virtual void AttackSkillFire( ioBaseChar *pChar );

public:
	void SetWarpMotionState( ioBaseChar *pChar, ioWeapon *pWeapon );

public:
	ioTargetWarpSkill();
	ioTargetWarpSkill( const ioTargetWarpSkill &rhs );
	virtual ~ioTargetWarpSkill();
};

inline ioTargetWarpSkill* ToTargetWarpSkill( ioSkill *pSkill )
{
	ioAttackSkill *pAttack = ToAttackSkill( pSkill );
	if( !pAttack || pAttack->GetSubSkillType() != AST_TARGET_WARP )
		return NULL;

	return dynamic_cast< ioTargetWarpSkill* >(pAttack);
}


