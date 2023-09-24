#include "stdafx.h"

#include "ioBaseChar.h"
#include "ioPlayStage.h"
#include "ioHunterCoinModeItem.h"

ioHunterCoinModeItem::ioHunterCoinModeItem()
{
	m_State = IS_WAIT;
	m_vMoveDirection = -ioMath::UNIT_Z;
	m_dwActivateTime = 0;
	m_dwCurCollisionTime = 0;
}

ioHunterCoinModeItem::ioHunterCoinModeItem( const ioHunterCoinModeItem &rhs )
	: ioModeItem( rhs ),
	m_LoopEffectName( rhs.m_LoopEffectName ),
	m_dwCollisionTime( rhs.m_dwCollisionTime )
{
	m_State = IS_WAIT;
	m_vMoveDirection = -ioMath::UNIT_Z;
	m_dwActivateTime = 0;
	m_dwCurCollisionTime = 0;
}

ioHunterCoinModeItem::~ioHunterCoinModeItem()
{

}

void ioHunterCoinModeItem::LoadProperty( ioINILoader &rkLoader )
{
	ioModeItem::LoadProperty( rkLoader );

	char szBuf[MAX_PATH] = "";
	rkLoader.LoadString_e( "loop_effect", "", szBuf, MAX_PATH );
	m_LoopEffectName = szBuf;

	m_dwCollisionTime = rkLoader.LoadInt_e( "collision_time", 0 );
}
ioModeItem *ioHunterCoinModeItem::Clone()
{
	return new ioHunterCoinModeItem( *this );
}

void ioHunterCoinModeItem::OnCreate()
{
	if( !m_pCreator )
		return;

	ioModeItem::OnCreate();
	ioMapEffect *pEffect = m_pCreator->CreateMapEffect( m_LoopEffectName, GetPosition(), D3DXVECTOR3( 1.0f, 1.0f, 1.0f ) );
	if( pEffect )
	{				
		m_dwLoopEffectID = pEffect->GetUniqueID();
	}
}

void ioHunterCoinModeItem::OnDestroy()
{
	if( !m_pCreator )
		return;

	ioModeItem::OnDestroy();

	if( m_dwLoopEffectID != -1 )
	{
		m_pCreator->DestroyMapEffect( m_dwLoopEffectID );
		m_dwLoopEffectID = -1;
	}
}

void ioHunterCoinModeItem::SetItemValue( DWORD dwIndex, float fXPos, float fZPos, int iAngle, float fSpeed, float fPower, DWORD dwActivateTime )
{
	SetUniqueID( dwIndex );

	D3DXVECTOR3 vPos = D3DXVECTOR3( fXPos, 0.0f, fZPos );
	vPos.y += m_pCreator->GetMapHeight( vPos.x, vPos.z, NULL );
	SetPosition( vPos );
	
	D3DXVECTOR3 vDir = D3DXVECTOR3( 0.0f, 0.0f, -1.0f );
	D3DXQUATERNION qtRot;
	float fAngle = static_cast<float>( iAngle );
	D3DXQuaternionRotationAxis( &qtRot, &ioMath::UNIT_Y, DEGtoRAD(fAngle) );
	vDir = qtRot * vDir;
	SetDirection( vDir );

	m_fFloatingPower = fPower;
	m_fMoveSpeed = fSpeed;

	m_dwActivateTime = FRAMEGETTIME() + dwActivateTime;
	m_dwCurCollisionTime = m_dwActivateTime + m_dwCollisionTime;
}

void ioHunterCoinModeItem::SetItemValue( DWORD dwIndex, D3DXVECTOR3 vPos, int iAngle, float fSpeed, float fPower, DWORD dwActivateTime )
{
	SetUniqueID( dwIndex );

	float fHeight = m_pCreator->GetMapHeight( vPos.x, vPos.z, NULL );	
	SetPosition( vPos );

	D3DXVECTOR3 vDir = D3DXVECTOR3( 0.0f, 0.0f, -1.0f );
	D3DXQUATERNION qtRot;
	float fAngle = static_cast<float>( iAngle );
	D3DXQuaternionRotationAxis( &qtRot, &ioMath::UNIT_Y, DEGtoRAD(fAngle) );
	vDir = qtRot * vDir;
	SetDirection( vDir );

	m_fFloatingPower = fPower;
	m_fMoveSpeed = fSpeed;

	m_dwActivateTime = FRAMEGETTIME() + dwActivateTime;
	m_dwCurCollisionTime = m_dwActivateTime + m_dwCollisionTime;
}

void ioHunterCoinModeItem::Process()
{
	ioModeItem::Process();

	switch( m_State )
	{
	case IS_WAIT:
		ProcessWait();
		break;
	case IS_MOVE:
		ProcessMove();
		break;
	case IS_PINGPONG:
		ProcessPingpong();
	case IS_DELAY:
		ProcessDelay();
		break;		
	}
}

void ioHunterCoinModeItem::ProcessWait()
{
	if( m_dwActivateTime > 0 && m_dwActivateTime < FRAMEGETTIME() )
	{
		OnCreate();
		SetMoveState();
	}
}

void ioHunterCoinModeItem::ProcessMove()
{
	if( !m_pCreator )
		return;

	float fTimePerSec = g_FrameTimer.GetSecPerFrame();
	float fCurGravity = m_pCreator->GetRoomGravity() * fTimePerSec;

	D3DXVECTOR3 vMove = m_vMoveDirection * ( m_fCurMoveSpeed * fTimePerSec );

	vMove.y += m_fCurFloatingPower * fTimePerSec;
	m_fCurFloatingPower -= fCurGravity;
		
	D3DXVECTOR3 vPos = GetPosition() + vMove;

	float fMapHeight = m_pCreator->GetMapHeight( vPos.x, vPos.z, NULL, false );
	if( vPos.y < fMapHeight )
	{			
		vPos.y = fMapHeight;
		SetPingpongState();
	}
	SetPosition( vPos );
	

	ioMapEffect* pMapEffect = m_pCreator->FindMapEffect( m_dwLoopEffectID );
	if( pMapEffect )
	{
		pMapEffect->SetWorldPosition( vPos );
	}
}

void ioHunterCoinModeItem::ProcessPingpong()
{
	if( !m_pCreator )
		return;

	float fTimePerSec = g_FrameTimer.GetSecPerFrame();
	float fCurGravity = m_pCreator->GetRoomGravity() * fTimePerSec;

	D3DXVECTOR3 vMove = m_vMoveDirection * ( m_fCurMoveSpeed * fTimePerSec );

	vMove.y += m_fCurFloatingPower * fTimePerSec;
	m_fCurFloatingPower -= fCurGravity;

	D3DXVECTOR3 vPos = GetPosition() + vMove;
	float fMapHeight = m_pCreator->GetMapHeight( vPos.x, vPos.z, NULL, false );
	if( vPos.y < fMapHeight )
	{			
		vPos.y = fMapHeight;

		m_iCurPingPongCnt++;
		if( m_iCurPingPongCnt >= 8 )
		{
			SetDelayState();
		}
		else
		{
			m_fCurMoveSpeed     = m_fMoveSpeed/m_iCurPingPongCnt;
			m_fCurFloatingPower = m_fFloatingPower/m_iCurPingPongCnt;
		}
	}
	SetPosition( vPos );

	ioMapEffect* pMapEffect = m_pCreator->FindMapEffect( m_dwLoopEffectID );
	if( pMapEffect )		
		pMapEffect->SetWorldPosition( vPos );
}

void ioHunterCoinModeItem::ProcessDelay()
{
}

void ioHunterCoinModeItem::SetMoveState()
{
	if( m_State == IS_MOVE )
		return;

	m_State = IS_MOVE;

	m_fCurMoveSpeed = m_fMoveSpeed;
	m_fCurFloatingPower  = m_fFloatingPower;
	m_iCurPingPongCnt = 1;
}

void ioHunterCoinModeItem::SetPingpongState()
{
	if( m_State == IS_PINGPONG )
		return;

	m_State = IS_PINGPONG;	

	m_fCurMoveSpeed = m_fMoveSpeed * 0.5f;
	m_fCurFloatingPower  = m_fFloatingPower * 0.5f;	
}

void ioHunterCoinModeItem::SetDelayState()
{
	if( m_State == IS_DELAY )
		return;

	m_State = IS_DELAY;
}

bool ioHunterCoinModeItem::IsCollision( ioBaseChar *pChar )
{
	if ( m_dwCurCollisionTime <= 0 || m_dwCurCollisionTime > FRAMEGETTIME() )
		return false;

	return ioModeItem::IsCollision( pChar );
}

bool ioHunterCoinModeItem::TestMapCollision( ioOpcodeShape *pShape, ioPlayStage *pStage, const D3DXMATRIX* pShapeWorldMat )
{
	//pShape�� ���� �� �浹 �˻縦 ���� �ϰ��� �Ҷ� TRUE
	//�Ϲ� ����� ��� ���� �޽��� 1�������� �Ͽ�¡�ý����� ��� N�� �̱� ������ TestMapCollision() �Լ��� ������ ȣ���

	if( m_State != IS_MOVE && m_State != IS_PINGPONG )
		return true;

	int iFaceIndex = 0;
	if( !pShape->FindIntersection( pShapeWorldMat, GetAttackBox(), &iFaceIndex ) )
		return false;

	D3DXVECTOR3 vNormal;
	pShape->GetTriNormalInModelSpace( iFaceIndex, vNormal );

	SetWallCollision( vNormal );
	return true;
}

void ioHunterCoinModeItem::SetWallCollision( const D3DXVECTOR3 &vFaceNrm )
{
	D3DXVECTOR3 vMoveDir = m_vMoveDirection;
	vMoveDir.y = 0.0f;
	D3DXVec3Normalize( &vMoveDir, &vMoveDir );

	float fDotValue = D3DXVec3Dot( &vMoveDir, &vFaceNrm );
	if( fDotValue > 0.1f )
		return;

	D3DXVECTOR3 vReflect = vMoveDir - 2.0f * fDotValue * vFaceNrm;
	vReflect.y = 0.0f;
	D3DXVec3Normalize( &vReflect, &vReflect );

	m_vMoveDirection = vReflect;
	D3DXVec3Normalize( &m_vMoveDirection, &m_vMoveDirection );
}