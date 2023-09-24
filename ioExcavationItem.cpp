
#include "stdafx.h"

#include "ioExcavationItem.h"
#include "ioExcavationManager.h"
#include "ioBaseChar.h"

#include "GUI/ExcavationWnd.h"

ioExcavationItem::ioExcavationItem(void)
{
	m_dwAnimationEndTime = 0;
	m_eAniType           = AT_NONE;
	m_bExcavatingToolMesh= false;
	m_dwEquipTime        = 0;
}

ioExcavationItem::ioExcavationItem( const ioExcavationItem &rhs )
: ioObjectItem( rhs ),
  m_sEquipAnimation( rhs.m_sEquipAnimation ),
  m_sReleaseAnimation( rhs.m_sReleaseAnimation ),
  m_sExcavatingStartAnimation( rhs.m_sExcavatingStartAnimation ),
  m_sExcavatingAnimation( rhs.m_sExcavatingAnimation ),
  m_sResultFailAnimation( rhs.m_sResultFailAnimation ),
  m_sResultSuccessAnimation( rhs.m_sResultSuccessAnimation ),
  m_fEquipAnimationRate ( rhs.m_fEquipAnimationRate ),
  m_fReleaseAnimationRate ( rhs.m_fReleaseAnimationRate ),
  m_fExcavatingStartAnimationRate( rhs.m_fExcavatingStartAnimationRate ),
  m_fExcavatingAnimationRate( rhs.m_fExcavatingAnimationRate ),
  m_fResultFailAnimationRate( rhs.m_fResultFailAnimationRate ),
  m_fResultSuccessAnimationRate( rhs.m_fResultSuccessAnimationRate ),
  m_sExcavatingToolMesh( rhs.m_sExcavatingToolMesh )
{
	m_dwAnimationEndTime = 0;
	m_eAniType           = AT_NONE;
	m_dwEquipTime        = 0;
	m_bExcavatingToolMesh= false;
}

ioExcavationItem::~ioExcavationItem(void)
{
}

ioItem* ioExcavationItem::Clone()
{
	return new ioExcavationItem( *this );
}

ioObjectItem::ObjectSubType ioExcavationItem::GetObjectSubType() const
{
	return OST_EXCAVATION;
}

void ioExcavationItem::LoadProperty( ioINILoader &rkLoader )
{
	ioObjectItem::LoadProperty( rkLoader );

	char szBuf[MAX_PATH]="";
	rkLoader.LoadString_e( "equip_animation", "", szBuf, MAX_PATH );
	m_sEquipAnimation = szBuf;
	m_fEquipAnimationRate = rkLoader.LoadFloat_e( "equip_animation_rate", FLOAT1 );

	rkLoader.LoadString_e( "release_animation", "", szBuf, MAX_PATH );
	m_sReleaseAnimation = szBuf;
	m_fReleaseAnimationRate = rkLoader.LoadFloat_e( "release_animation_rate", FLOAT1 );

	rkLoader.LoadString_e( "excavating_start_animation", "", szBuf, MAX_PATH );
	m_sExcavatingStartAnimation = szBuf;
	m_fExcavatingStartAnimationRate = rkLoader.LoadFloat_e( "excavating_start_animation_rate", FLOAT1 );

	rkLoader.LoadString_e( "excavating_animation", "", szBuf, MAX_PATH );
	m_sExcavatingAnimation = szBuf;
	m_fExcavatingAnimationRate = rkLoader.LoadFloat_e( "excavating_animation_rate", FLOAT1 );

	rkLoader.LoadString_e( "result_fail_animation", "", szBuf, MAX_PATH );
	m_sResultFailAnimation = szBuf;
	m_fResultFailAnimationRate = rkLoader.LoadFloat_e( "result_fail_animation_rate", FLOAT1 );

	rkLoader.LoadString_e( "result_success_animation", "", szBuf, MAX_PATH );
	m_sResultSuccessAnimation = szBuf;
	m_fResultSuccessAnimationRate = rkLoader.LoadFloat_e( "result_success_animation_rate", FLOAT1 );

	rkLoader.LoadString_e( "excavating_tool_mesh", "", szBuf, MAX_PATH );
	m_sExcavatingToolMesh = szBuf;
}

void ioExcavationItem::OnEquiped( ioBaseChar *pOwner )
{
	ioObjectItem::OnEquiped( pOwner );
	
	if( SetActionAni( pOwner, m_sEquipAnimation, m_fEquipAnimationRate ) )
		m_eAniType = AT_EQUIP;

	if( pOwner )
		pOwner->SetExcavating( true );

	m_dwEquipTime = REALGETTIME();
}

void ioExcavationItem::SetReleaseAni( ioBaseChar *pOwner )
{
	if( SetActionAni( pOwner, m_sReleaseAnimation, m_fReleaseAnimationRate ) )
		m_eAniType = AT_RELEASE;

	SetExcavatingToolMesh( pOwner, false );

	m_dwEquipTime = 0;
}

void ioExcavationItem::SetExcavatingStartAni( ioBaseChar *pOwner )
{
	if( SetActionAni( pOwner, m_sExcavatingStartAnimation, m_fExcavatingStartAnimationRate ) )
		m_eAniType = AT_EXCAVATING_STRT;
}

void ioExcavationItem::SetExcavatingAni( ioBaseChar *pOwner )
{
	if( SetActionAni( pOwner, m_sExcavatingAnimation, m_fExcavatingAnimationRate, true ) )
		m_eAniType = AT_EXCAVATING;
}

void ioExcavationItem::SetResultFailAni( ioBaseChar *pOwner )
{
	if( SetActionAni( pOwner, m_sResultFailAnimation, m_fResultFailAnimationRate ) )
		m_eAniType = AT_RESULT_FAIL;
}

void ioExcavationItem::SetResultSuccessAni( ioBaseChar *pOwner )
{
	if( SetActionAni( pOwner, m_sResultSuccessAnimation, m_fResultSuccessAnimationRate ) )
		m_eAniType = AT_RESULT_SUCCESS;
}

void ioExcavationItem::SetResultEquipAni( ioBaseChar *pOwner )
{
	if( SetActionAni( pOwner, m_sEquipAnimation, m_fEquipAnimationRate ) )
		m_eAniType = AT_RESULT_EQUIP;
}

bool ioExcavationItem::IsAnimationEndTime( ioBaseChar *pChar )
{
	if( !pChar )
		return false;

	if( m_dwAnimationEndTime + pChar->GetActionStopTotalDelay() < FRAMEGETTIME() )
		return true;

	return false;
}

bool ioExcavationItem::SetActionAni( ioBaseChar * pOwner, ioHashString &rszAnimation, float fAnimationRate, bool bLoopAni /*= false */ )
{
	if( !pOwner )
		return false;

	if( rszAnimation.IsEmpty() )
		return false;

	ioEntityGroup *pGrp = pOwner->GetGroup();
	int iAniID = pGrp->GetAnimationIdx( rszAnimation );
	if( iAniID == -1 ) 
		return false;

	pOwner->SetState( CS_EXCAVATING );

	pGrp->ClearAllActionAni( FLOAT100, true );
	pGrp->ClearAllLoopAni( FLOAT100, true );
	if( bLoopAni)
	{
		pGrp->SetLoopAni( rszAnimation, FLOAT100, FLOAT100, FLOAT1, FLOAT1/fAnimationRate );
		m_dwAnimationEndTime = FRAMEGETTIME();
		m_dwAnimationEndTime += 120000 * fAnimationRate; // 중간에 종료 되므로 최장 2분 설정
	}
	else
	{
		pGrp->SetActionAni( rszAnimation, FLOAT100, FLOAT100, FLOAT1, FLOAT1/fAnimationRate );

		m_dwAnimationEndTime = FRAMEGETTIME();
		m_dwAnimationEndTime += pGrp->GetAnimationFullTime( iAniID ) * fAnimationRate;
	}
	
	return true;
}

void ioExcavationItem::ClearAniInfo()
{
	m_dwAnimationEndTime = 0;
	m_eAniType = AT_NONE;
}


void ioExcavationItem::SetExcavatingToolMesh( ioBaseChar *pOwner, bool bSet )
{
	if( !pOwner )
		return;

	if( m_sExcavatingToolMesh.IsEmpty() )
		return;

	if( bSet )
	{
		if( m_bExcavatingToolMesh )
			return;
		pOwner->AddEntity( m_sExcavatingToolMesh );
		m_bExcavatingToolMesh = true;
	}
	else
	{
		if( !m_bExcavatingToolMesh )
			return;
		pOwner->DestroyEntity( m_sExcavatingToolMesh );
		m_bExcavatingToolMesh = false;
	}
}	

void ioExcavationItem::SetNormalAttack( ioBaseChar *pOwner, int iCurCombo )
{

}