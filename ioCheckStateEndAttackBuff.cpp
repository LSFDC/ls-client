
#include "stdafx.h"

#include "ioCheckStateEndAttackBuff.h"

#include "WeaponDefine.h"

#include "ioPlayStage.h"
#include "ioBaseChar.h"

ioCheckStateEndAttackBuff::ioCheckStateEndAttackBuff()
{
	m_dwWeaponBaseIndex = 0;
	m_dwFireTime = 0;
	m_BuffState = BS_WAIT;
}

ioCheckStateEndAttackBuff::ioCheckStateEndAttackBuff( const ioCheckStateEndAttackBuff &rhs )
	: ioBuff( rhs ),
	  m_szCheckBuffName( rhs.m_szCheckBuffName ),
	  m_szAttackAnimation( rhs.m_szAttackAnimation ),
	  m_fAttackTimeRate( rhs.m_fAttackTimeRate ),
	  m_nAttackWeapon( rhs.m_nAttackWeapon ),
	  m_nCheckState( rhs.m_nCheckState )
{	
	m_dwWeaponBaseIndex = 0;
	m_dwFireTime = 0;
	m_BuffState = BS_WAIT;
}

ioCheckStateEndAttackBuff::~ioCheckStateEndAttackBuff()
{	
}

ioBuff* ioCheckStateEndAttackBuff::Clone()
{
	return new ioCheckStateEndAttackBuff( *this );
}

void ioCheckStateEndAttackBuff::LoadProperty( ioINILoader &rkLoader )
{
	ioBuff::LoadProperty( rkLoader );

	char szBuf[MAX_PATH] = "";
	rkLoader.LoadString_e( "check_buff", "", szBuf, MAX_PATH );
	m_szCheckBuffName = szBuf;
	
	rkLoader.LoadString_e( "attack_animation", "", szBuf, MAX_PATH );
	m_szAttackAnimation = szBuf;

	m_fAttackTimeRate = rkLoader.LoadFloat_e( "attack_rate", 1.0f );

	m_nAttackWeapon = rkLoader.LoadInt_e( "attack_weapon", NO_WEAPON_ATTRIBUTE );

	m_nCheckState = rkLoader.LoadInt_e( "check_state", CS_DELAY );
}

void ioCheckStateEndAttackBuff::StartBuff( ioBaseChar *pOwner )
{
	ioBuff::StartBuff( pOwner );

	m_dwWeaponBaseIndex = 0;
	m_dwFireTime = 0;
	m_BuffState = BS_WAIT;
}

bool ioCheckStateEndAttackBuff::ApplyStartBuff( ioBaseChar *pOwner, DWORD dwGlobalTime, DWORD dwGapTime )
{
	if( !ioBuff::ApplyStartBuff( pOwner, dwGlobalTime, dwGapTime ) )
		return false;

	m_dwWeaponBaseIndex = 0;
	m_dwFireTime = 0;
	m_BuffState = BS_WAIT;

	return true;
}

void ioCheckStateEndAttackBuff::ProcessBuff( float fTimePerSec )
{	
	ioBuff::ProcessBuff( fTimePerSec );
	
	switch( m_BuffState )
	{
	case BS_WAIT:
		CheckCharState( m_pOwner );
		break;
	case BS_EXPLOSION:
		CheckCreateWeapon( m_pOwner );
		break;
	}
}

void ioCheckStateEndAttackBuff::EndBuff()
{
	ioBuff::EndBuff();
}

//ĳ���� ���� Ȯ��
void ioCheckStateEndAttackBuff::CheckCharState( ioBaseChar *pOwner )
{
	if ( !pOwner )
		return;
		
	if ( IsEnableAttack( pOwner ) )
	{
		//���� �����ϸ鼭 Ű�Է½� �ִϸ��̼� ����
		SetAttackTime( pOwner, m_szAttackAnimation, m_fAttackTimeRate );
	}	
}

//���� ������ �������� Ȯ��
bool ioCheckStateEndAttackBuff::IsEnableAttack( ioBaseChar *pOwner )
{
	if ( !pOwner )
		return false;

	int nOwnerState = (int)pOwner->GetState();	
	if ( nOwnerState == m_nCheckState)
		return true;
	
	return false;
}

//�ִϸ��̼� �ð� ���
void ioCheckStateEndAttackBuff::SetAttackTime( ioBaseChar *pOwner, ioHashString szName, float fRate )
{
	if ( !pOwner || szName.IsEmpty() )
		return;

	ioEntityGroup *pGroup = pOwner->GetGroup();
	int iAniID = pGroup->GetAnimationIdx( szName );
	if( iAniID == -1 ) 
		return;

	//���� ���� �ð� ����
	DWORD dwCurTime = FRAMEGETTIME();
	m_dwFireTime = dwCurTime;
	m_dwFireTime += pGroup->GetAnimationEventTime_e( iAniID, "fire_s" ) * fRate;

	m_BuffState = BS_EXPLOSION;
}

//���� ����
void ioCheckStateEndAttackBuff::CheckCreateWeapon( ioBaseChar *pOwner )
{
	if ( !pOwner )
		return;

	ioPlayStage *pPlayStage = pOwner->GetCreator();
	if ( !pPlayStage )
		return;

	DWORD dwCurTime = FRAMEGETTIME();
	if ( m_dwFireTime == 0 || m_dwFireTime > dwCurTime )
		return;

	DWORD dwWeapon = pOwner->IncreaseWeaponIndexBase();
	m_dwWeaponBaseIndex = dwWeapon;
	
	const BaseCharList &rkCharList = pPlayStage->GetBaseCharList();
	BaseCharList::const_iterator iter;
	for( iter=rkCharList.begin() ; iter!=rkCharList.end() ; ++iter )
	{
		ioBaseChar *pTarget = *iter;
		if( !pTarget )	
			continue;

		if( pTarget == pOwner )
			continue;

		if ( !pTarget->HasBuff( pOwner->GetCharName(), m_szCheckBuffName ) )
			continue;

		FireTime kFireTime;
		kFireTime.dwStart = dwCurTime;

		kFireTime.iAttributeIdx = m_nAttackWeapon;
		kFireTime.iResistanceIdx = 0;
		kFireTime.szWoundedAni  = "";
		kFireTime.dwWoundedAniDuration = 0;
		kFireTime.bLoopWoundedAni = false;
		kFireTime.dwWeaponIdx = m_dwWeaponBaseIndex++;
		kFireTime.eFireTimeType = FTT_NORMAL_ATTACK;

		D3DXVECTOR3 vPos = pTarget->GetMidPositionByRate();
		D3DXVECTOR3 vDir = ioMath::VEC3_ZERO;

		pOwner->ExtendFireExplicit( kFireTime, vPos, vDir, "" );
	}
	
	//���� �� ���� ����
	SetReserveEndBuff();
	m_dwFireTime = 0;
}