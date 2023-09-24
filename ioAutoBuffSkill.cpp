
#include "stdafx.h"

#include "ioBaseChar.h"

#include "ioAutoBuffSkill.h"

ioAutoBuffSkill::ioAutoBuffSkill()
{
}

ioAutoBuffSkill::ioAutoBuffSkill( const ioAutoBuffSkill &rhs )
: ioBuffSkill( rhs )
{
}

ioAutoBuffSkill::~ioAutoBuffSkill()
{
}

void ioAutoBuffSkill::LoadProperty( ioINILoader &rkLoader )
{
	ioBuffSkill::LoadProperty( rkLoader );
}

ioSkill* ioAutoBuffSkill::Clone()
{
	return new ioAutoBuffSkill( *this );
}

int ioAutoBuffSkill::GetTypeID() const
{
	return ST_BUFF;
}

bool ioAutoBuffSkill::OnSkillStart( ioBaseChar *pChar )
{
	if( !CheckEnableStart( pChar ) )
		return false;

	// Ÿ���� �ʿ��� ������ �ִ��� üũ
	int iCnt = m_TargetBuffList.size();
	for( int i=0; i < iCnt; ++i )
	{
		if( m_TargetBuffList[i].m_iBuffType == BT_RESTORATION ||
			m_TargetBuffList[i].m_iBuffType == BT_RECOVERY )
		{
			ioBaseChar *pTarget = pChar->GetBaseChar( GetOwnerItem()->GetTargetName() );
			if( !pTarget )
				return false;
		}
	}

	if( !ioSkill::OnSkillStart( pChar ) )
		return false;

	m_dwPreDelayStartTime = 0;
	m_dwFireStartTime = 0;
	m_dwFireMotionEndTime = 0;
	m_bSetHandMesh = false;

	BuffSkillSet( pChar );

	return true;
}

void ioAutoBuffSkill::OnProcessState( ioBaseChar *pChar, ioPlayStage *pStage )
{
}

void ioAutoBuffSkill::OnSkillEnd( ioBaseChar *pChar )
{
	ioSkill::OnSkillEnd( pChar );
}

bool ioAutoBuffSkill::IsCanUseSkill( ioBaseChar *pChar, bool bHelpMsg )
{
	if( CheckUseBuff( pChar ) )
		return false;

	// ��밡������ üũ
	ioAttackableItem *pAttackable = ToAttackableItem( GetOwnerItem() );
	if( pAttackable && !pAttackable->IsCanFire( pChar ) )
		return false;


	// ������ ���������� ���Ұ��� ���� ���
	ioHashString szName;
	if( pChar )
		szName = pChar->GetCharName();

	if( !CheckGaugeMax(szName) )
		return false;

	return true;
}

void ioAutoBuffSkill::BuffSkillSet( ioBaseChar *pChar )
{
	AddOwnerBuff( pChar );
	CreateAreaWeapon( pChar );

	m_bReduceGauge = true;
	pChar->ReduceNeedGaugeBySkill( this, GetNeedGauge(pChar->GetCharName()) );
}

bool ioAutoBuffSkill::StartPreDelayAnimation( ioBaseChar *pChar )
{
	return false;
}

bool ioAutoBuffSkill::IsOnOffEnable()
{
	return false;
}

