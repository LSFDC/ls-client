
#include "stdafx.h"

#include "ioMoveAttack3Dummy.h"

#include "ioPlayStage.h"
#include "ioBaseChar.h"

#include "BuffDefine.h"

ioMoveAttack3Dummy::ioMoveAttack3Dummy( ioEntityGroup *pGrp, ioPlayMode *pMode )
	: ioDummyChar( pGrp, pMode )
{
	m_dwReturnTime = 0;
	m_fCheckMoveRange = 0.0f;
	m_MoveState  = MS_WAIT;
	m_CurDirKey = ioUserKeyInput::DKI_NONE;

	m_vTargetList.clear();

	m_vTargetPos = ioMath::VEC3_ZERO;
	m_bCreateWeapon = false;
}

ioMoveAttack3Dummy::~ioMoveAttack3Dummy()
{
}

void ioMoveAttack3Dummy::LoadProperty( ioINILoader &rkLoader )
{
	ioDummyChar::LoadProperty( rkLoader );

	m_TargetWoundType = (TargetWoundType)rkLoader.LoadInt_e( "target_wound_type", TWT_ALL );
	m_fTargetRange    = rkLoader.LoadFloat_e( "target_range", 0.0f );
	m_fTargetAngle    = rkLoader.LoadFloat_e( "target_angle", 0.0f );
	m_fDummyHeightOffset = rkLoader.LoadFloat_e( "dummy_height_offset", 0.0f );
	m_dwTargetRefreshTime = rkLoader.LoadInt_e( "refresh_target_time", 0 );

	m_dwDelayTime = (DWORD)rkLoader.LoadInt_e( "delay_time", 0 );
	m_fMoveSpeed  = rkLoader.LoadFloat_e( "move_speed", FLOAT1 );
}

void ioMoveAttack3Dummy::ProcessState( float fTimePerSec, DWORD dwPreTime )
{
	/*switch( m_State )
	{
	case DCS_START:
		ProcessStartState( fTimePerSec );
		break;
	default: ProcessMoveState( fTimePerSec ); return;
	}*/

	if( !m_bCreateWeapon )
	{
		//SetAttachWeapon();
		m_bCreateWeapon = true;
		m_State = DCS_MOVE;
		
		//SetDelayTimeKillWeaponValue( false );

		SetMoveState();
		//SetDelayState( false );
	}

	ProcessMoveState( fTimePerSec );
}

void ioMoveAttack3Dummy::ProcessStartState( float fTimePerSec )
{
	SetAttachWeapon();
	//SetAttackState();
}

void ioMoveAttack3Dummy::ProcessMoveState( float fTimePerSec )
{
	ioBaseChar *pOwner = GetOwner();
	if( !pOwner )
		return;

	ioPlayStage *pStage = pOwner->GetCreator();
	if( !pStage )
		return;

	D3DXVECTOR3 vPos = GetWorldPosition();
	D3DXQUATERNION qtRot;

	switch( m_MoveState )
	{
	case MS_WAIT:
		break;
	case MS_MOVE:
		{
			float fMoveSpeed = m_fMoveSpeed * fTimePerSec;
			if( ( m_fNeedMoveLength - fMoveSpeed ) > 0.f )
			{
				m_fNeedMoveLength -= fMoveSpeed;
			}
			else
			{
				fMoveSpeed = m_fNeedMoveLength;
				SetWaitState();	
			}
			
			bool bCol = false;

			D3DXVECTOR3 vMove;
			vMove = m_vTargetPos - GetWorldPosition();
			D3DXVec3Normalize( &vMove, &vMove );
			vMove = fMoveSpeed * vMove;

			if( pStage && pStage->ReCalculateMoveVectorByTerrain( this, &vMove, bCol ) )
			{
				Translate( vMove );
			}
			if( bCol )
				SetWaitState();	

			float fMapHeight = GetBottomHeight();
			if( GetWorldPosition().y <= fMapHeight )
			{
				D3DXVECTOR3 vPos = GetWorldPosition();
				vPos.y = fMapHeight;
				SetWorldPosition( vPos );
				//SetWaitState();	
				return;
			}
		}
		break;
	}
}


void ioMoveAttack3Dummy::SetWaitState()
{
	m_MoveState = MS_WAIT;
	m_fNeedMoveLength = 0.f;
}


/*void ioMoveAttack3Dummy::ApplyDummyCharSync( ioPlayStage *pStage, SP2Packet &rkPacket )
{
	if( !pStage ) return;

	D3DXVECTOR3 vPos;
	rkPacket >> vPos;
	SetWorldPosition( vPos );

//	SetDelayState( DS_STANDBY );

	int iDirKey;
	rkPacket >> iDirKey;
	if( iDirKey != ioUserKeyInput::DKI_NONE )
	{
		float fYawD = 180.0f + 45.0f * ( iDirKey - ioUserKeyInput::DKI_UP );
		fYawD = ioMath::ArrangeHead( fYawD );

		float fYawR = DEGtoRAD( fYawD );
		D3DXQUATERNION qtRotate;
		D3DXQuaternionRotationYawPitchRoll( &qtRotate, fYawR, 0.0f, 0.0f );

		D3DXVECTOR3 vDir = qtRotate * D3DXVECTOR3( 0.0f, 0.0f, -FLOAT1 );
		D3DXVec3Normalize( &m_vAttackDir, &vDir );
	}
}*/

/*void ioMoveAttack3Dummy::SetDieState( bool bEffect )
{
	ioDummyChar::SetDieState( bEffect );
}*/

bool ioMoveAttack3Dummy::IsActivateWeapon()
{
	if( m_State == DCS_DELAY )
		return false;

	return true;
}

void ioMoveAttack3Dummy::SetMovePosition( const D3DXVECTOR3& vTargetPos )
{
	m_vTargetPos = vTargetPos;

	D3DXVECTOR3 vPos = GetWorldPosition();

	vPos = m_vTargetPos - vPos;

	m_fNeedMoveLength = D3DXVec3Length( &vPos );

	//SetMoveState();
	m_MoveState = MS_MOVE;
}