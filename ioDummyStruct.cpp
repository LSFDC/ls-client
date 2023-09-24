
#include "stdafx.h"
#include "ioDummyStruct.h"

#include "ioPlayStage.h"
#include "ioBaseChar.h"

#include "BuffDefine.h"

ioDummyStruct::ioDummyStruct( ioEntityGroup *pGrp, ioPlayMode *pMode )
: ioDummyChar( pGrp, pMode )
{
	m_DummyStructState = DSS_START;
}

ioDummyStruct::~ioDummyStruct()
{
	m_OwnerBuffList.clear();
	m_RemoveBuffList.clear();
}

void ioDummyStruct::LoadProperty( ioINILoader &rkLoader )
{
	ioDummyChar::LoadProperty( rkLoader );

	char szBuf[MAX_PATH];

	rkLoader.LoadString_e( "check_buff", "", szBuf, MAX_PATH );
	m_szCheckBuff = szBuf;

	m_bSetAllWeaponDefence = rkLoader.LoadBool_e( "set_all_weapon_defence", false );
	m_bCheckRange = rkLoader.LoadBool_e( "set_check_range", false );
	m_fRange = rkLoader.LoadFloat_e( "check_range_dist", 0.0f );
	m_bOnlyOwnerAddBuff = rkLoader.LoadBool_e( "set_owner_add_buff", false );

	LoadOwnerBuffList( rkLoader );
	LoadRemoveBuffList( rkLoader );
}

void ioDummyStruct::ProcessState( float fTimePerSec, DWORD dwPreTime )
{
	switch( m_DummyStructState )
	{
	case DSS_START:
		{
			if( !m_DummyEffect.IsEmpty() )
			{
				m_pGroup->AttachEffect( m_DummyEffect, NULL );
				m_bMoveEffectCreated = true;
			}

			SetAttachWeapon();
			SetOwenrBuff();

			m_DummyStructState = DSS_LOOP;
		}
		break;
	case DSS_LOOP:
		{
			CheckBuff();
			CheckRange();
		}
		break;
	}
}

ApplyWeaponExp ioDummyStruct::ApplyWeapon( ioWeapon *pWeapon )
{
	ioBaseChar *pAttacker = pWeapon->GetOwner();
	if( !pAttacker )
	{
		LOG.PrintTimeAndLog( 0, "ioDummyChar::ApplyWeapon - %s Not Exist", pWeapon->GetOwnerName().c_str() );
		return AWE_EXPLOSION;
	}

	if( pWeapon->IsNoWoundedWeapon() )
		return AWE_NO_EXPLOSION;

	m_LastAttacker = pWeapon->GetOwnerName();

	float fDamage = pWeapon->GetDamage( this ) * m_fDamageRate;
	float fPushPower = pWeapon->GetDefensePushPower( fDamage );

	if( !m_bNoDamage && !m_bPiercingProtected )
		m_HP.m_fCurValue -= fDamage;
	AddHeadDamageInfo( (int)fDamage );

	bool bContactType = false;
	if( pWeapon->GetType() == ioWeapon::WT_CONTACT ||
		pWeapon->GetType() == ioWeapon::WT_RUN_CONTACT ||
		pWeapon->GetType() == ioWeapon::WT_CONTACT2 )
		bContactType = true;

	bool bStructWeapon = false;
	if( pWeapon->CheckWeaponByStruct() || pWeapon->CheckWeaponByDummyChar() )
		bStructWeapon = true;

	D3DXVECTOR3 vAttackDir = pWeapon->GetAttackDir(this);

	if( bContactType && !bStructWeapon && !pWeapon->CheckWeaponBySkill() && !m_BlockBuff.IsEmpty() )
	{
		if( pAttacker->IsEnableAttackCancelState() && !pWeapon->IsChangeJumpping( false, false )
			&& !pAttacker->IgnoreBlock() )
		{
			pAttacker->AddNewBuff( m_BlockBuff, "", "", NULL );
			pAttacker->ReverseReflectPush( vAttackDir, fPushPower * 0.5f );
		}
	}

	if( !m_BlockBuff.IsEmpty() )
		pAttacker->SetDisableAttackEmoticon();

	int iShakeCamera = pWeapon->GetShakeCamera();
	ActionStopType eActionStop = pWeapon->GetActionStopType();

	if( eActionStop == AST_BOTH || eActionStop == AST_ATTACKER )
	{
		pAttacker->SetActionStopDelay( fDamage, false, iShakeCamera );
		pWeapon->SetActionStopDuration( ioBaseChar::CalcActionStopDelay( fDamage, false ),
			ioBaseChar::GetActionStopLooseRate( false ) );
	}

	if( m_HP.m_fCurValue <= 0.0f )
	{
		m_HP.m_fCurValue = 0.0f;
		SetDieState( true );

		// 여기서 스킬 캔슬 처리 필요
		switch( m_DummyCharType )
		{
		case DCT_OWNER_CONTROL:
		case DCT_OWNER_CONTROL3:
		case DCT_OWNER_ATTACH:
			if( !m_ControlSkillName.IsEmpty() )
			{
				if( GetOwner() && GetOwner()->IsCurUsingSkill( m_ControlSkillName ) )
				{
					GetOwner()->ClearAttackFireTimeAndSkill( CAFT_APPLY_WOUNDED_BY_DUMMY );
					m_ControlSkillName.Clear();
				}
			}
			break;
		}
	}

	if( pWeapon )
	{
		int iDummyIndex = pWeapon->GetWeaponByDummyChar();
		g_DummyCharMgr.CheckEntityCollision( pAttacker, this, iDummyIndex );
	}

	return AWE_EXPLOSION;
}

void ioDummyStruct::ApplyDummyCharSync( ioPlayStage *pStage, SP2Packet &rkPacket )
{
	if( !pStage ) 
		return;	
	
	int iType;
	rkPacket >> iType;	
}

void ioDummyStruct::TranslateByConveyer( const D3DXVECTOR3 &vMove )
{
}

int ioDummyStruct::DontMoveEntityLevel() const
{
	return 2;
}

//더미 죽는 상태 설정
void ioDummyStruct::SetDieState( bool bEffect )
{
	ProcessEnd();

	ioDummyChar::SetDieState( bEffect );
}

bool ioDummyStruct::IsActivateWeapon()
{
	return true;
}

void ioDummyStruct::LoadOwnerBuffList( ioINILoader &rkLoader )
{
	int iCnt = rkLoader.LoadInt_e( "owner_buff_cnt", 0 );
	m_OwnerBuffList.clear();

	if( iCnt <= 0 ) 
		return;

	m_OwnerBuffList.reserve( iCnt );

	char szTitle[MAX_PATH], szBuf[MAX_PATH];
	for( int i=0; i < iCnt; ++i )
	{
		wsprintf_e( szTitle, "owner_buff%d", i+1 );
		rkLoader.LoadString( szTitle, "", szBuf, MAX_PATH );

		m_OwnerBuffList.push_back( szBuf );
	}
}

void ioDummyStruct::LoadRemoveBuffList( ioINILoader &rkLoader )
{
	int iCnt = rkLoader.LoadInt_e( "remove_buff_cnt", 0 );
	m_RemoveBuffList.clear();

	if( iCnt <= 0 ) 
		return;

	m_RemoveBuffList.reserve( iCnt );

	char szTitle[MAX_PATH], szBuf[MAX_PATH];
	for( int i=0; i < iCnt; ++i )
	{
		wsprintf_e( szTitle, "remove_buff%d", i+1 );
		rkLoader.LoadString( szTitle, "", szBuf, MAX_PATH );

		m_RemoveBuffList.push_back( szBuf );
	}
}

void ioDummyStruct::CheckBuff()
{
	if( !m_pModeParent || m_szCheckBuff.IsEmpty() )
		return;

	ioPlayStage *pStage = m_pModeParent->GetCreator();
	if( !pStage )
		return;

	ioBaseChar *pOwner = pStage->GetBaseChar( m_OwnerName );
	if ( pOwner )
	{
		if ( !pOwner->HasBuff( m_szCheckBuff ) )
			SetDieState( true );
	}
}

bool ioDummyStruct::IsAllWeaponDefence() const
{
	return m_bSetAllWeaponDefence;
}

void ioDummyStruct::ProcessEnd()
{
	ioBaseChar *pOwner = GetOwner();
	if ( !pOwner )
		return;
	
	int nBuffCnt = (int)m_RemoveBuffList.size();	
	for ( int i=0; i<nBuffCnt; ++i )
		pOwner->RemoveBuff( m_RemoveBuffList[i], true );
}

bool ioDummyStruct::IsDummyVsDummyCollisionSkipState( const ioDummyChar *pDummy )
{
	if( pDummy )
		return true;

	return ioDummyChar::IsDummyVsDummyCollisionSkipState( pDummy );
}

void ioDummyStruct::SetOwenrBuff()
{
	ioBaseChar *pOwner = GetOwner();
	if ( !pOwner )
		return;

	if ( m_bOnlyOwnerAddBuff && !pOwner->IsNeedProcess() )
		return;

	int nBuffCnt = (int)m_OwnerBuffList.size();	
	for ( int i=0; i<nBuffCnt; ++i )
		pOwner->AddNewBuff( m_OwnerBuffList[i], pOwner->GetCharName(), "", NULL );
}

void ioDummyStruct::CheckRange()
{
	if ( !m_bCheckRange )
		return;

	ioBaseChar *pOwner = GetOwner();
	if ( !pOwner )
		return;

	D3DXVECTOR3 vDist = GetWorldPosition() - pOwner->GetWorldPosition();
	if ( D3DXVec3LengthSq( &vDist ) > m_fRange * m_fRange )
		SetDieState( true );
}