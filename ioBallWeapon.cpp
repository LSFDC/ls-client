

#include "stdafx.h"

#include "ioBallWeapon.h"
#include "ioBaseChar.h"
#include "ioPlayStage.h"

ActionStopType ioBallWeapon::m_ActionStopType = AST_NONE;
ActionStopType ioBallWeapon::m_DieActionStopType = AST_NONE;

ioBallWeapon::ioBallWeapon( ioBaseChar *pOwner, const WeaponAttribute *pAttr )
: ioWeapon( pOwner, pAttr )
{
	SetPeculiarity( WP_DEF_NO_SOUND );

	m_pFireMesh = NULL;
	m_iCollisionCnt = 0;

	m_bCheckCollision = false;
	m_fCurMoveSpeed = 0.0f;
	m_fCurMoveRange = 0.0f;
}

ioBallWeapon::~ioBallWeapon()
{
	if( m_pFireMesh )
	{
		LOG.PrintTimeAndLog( 0, "ioBallWeapon::~ioBallWeapon - UnReleased Resource" );
	}
}

bool ioBallWeapon::InitResource()
{
	if( ioWeapon::InitResource() )
	{
		DestroyResource();

		return SetFireMesh( GetAttribute()->m_WeaponEffect );
	}

	return false;
}

void ioBallWeapon::LoadProperty( ioINILoader &rkLoader )
{
	ioWeapon::LoadProperty( rkLoader );

	char szBuf[MAX_PATH] = "";
	m_dwLiveTime = rkLoader.LoadInt_e( "live_time", 0 );

	int iCnt = rkLoader.LoadInt_e( "change_dir_cnt", 0 );
	if( iCnt <= 0 ) return;

	m_ChangeDirInfoList.clear();
	m_ChangeDirInfoList.reserve( iCnt );

	for( int i=0; i < iCnt; ++i )
	{
		ChangeDirInfo kInfo;

		wsprintf_e( szBuf, "change_range%d", i+1 );
		kInfo.m_fChangeRange = rkLoader.LoadFloat( szBuf, 0.0f );

		wsprintf_e( szBuf, "change_speed%d", i+1 );
		kInfo.m_fChangeSpeed = rkLoader.LoadFloat( szBuf, 0.0f );

		wsprintf_e( szBuf, "change_angle_h%d", i+1 );
		kInfo.m_fChangeAngleH = rkLoader.LoadFloat( szBuf, 0.0f );

		wsprintf_e( szBuf, "change_angle_v%d", i+1 );
		kInfo.m_fChangeAngleV = rkLoader.LoadFloat( szBuf, 0.0f );

		m_ChangeDirInfoList.push_back( kInfo );
	}

	std::sort( m_ChangeDirInfoList.begin(), m_ChangeDirInfoList.end(), ChangeDirSort() );
}

void ioBallWeapon::SetLiveTime( DWORD dwTime )
{
}

void ioBallWeapon::SetStartPosition( const D3DXVECTOR3 &vPos, ioPlayStage *pStage )
{
	if( m_pFireMesh && m_pFireMesh->GetSkeletonName().IsEmpty() && m_pFireMesh->HasLinkedSystem() )
		return;

	SetPosition( vPos );
	m_vPrePos = GetPosition();
}

void ioBallWeapon::SetMoveDir( const D3DXVECTOR3 &vDir )
{
	m_vMoveDir = vDir;
	m_vAttackDir = m_vMoveDir;

	D3DXQUATERNION qtRot;
	ioMath::CalcDirectionQuaternion( qtRot, -m_vMoveDir );
	SetOrientation( qtRot );

	D3DXVec3Cross( &m_vRollAxis, &D3DXVECTOR3( 0.0f, FLOAT1, 0.0f ), &m_vMoveDir );
}

bool ioBallWeapon::SetFireMesh( const ioHashString &szFileName )
{
	ioBaseChar *pOwner = GetOwner();
	if( !pOwner )	return false;

	m_pFireMesh = g_EffectFactory.CreateEffect( szFileName );
	if( !m_pFireMesh )	return false;

	GetParentSceneNode()->AttachObject( m_pFireMesh );
	GetParentSceneNode()->SetScale( FLOAT1, FLOAT1, FLOAT1 );

	ioHashString szLinkBiped;
	D3DXVECTOR3 vLinkPosOffset;
	D3DXQUATERNION qtLinkRotOffset;

	if( m_pFireMesh->GetSkeletonName().IsEmpty() && m_pFireMesh->GetFirstLinkOffset(szLinkBiped, vLinkPosOffset, qtLinkRotOffset ) )
	{
		ioBiped *pLinked = pOwner->GetGroup()->GetBiped( szLinkBiped );
		if( pLinked )
		{
			vLinkPosOffset = pLinked->GetDerivedOrientation() * vLinkPosOffset + pLinked->GetDerivedPosition();
			vLinkPosOffset = vLinkPosOffset * pOwner->GetWorldScale();
			vLinkPosOffset = pOwner->GetWorldOrientation() * vLinkPosOffset + pOwner->GetWorldPosition();
			SetPosition( vLinkPosOffset );
		}
	}

	m_pFireMesh->UpdateBounds( true );
	m_LocalAttackBox = m_pFireMesh->GetCollisionBox();

	m_vPrePos = GetPosition();

	AddEffectEvent( m_pFireMesh );
	return true;
}

bool ioBallWeapon::SetWallCollision( const D3DXVECTOR3 &vFaceNrm, bool bMap, bool bOnlyAttack )
{
	float fCheck = D3DXVec3Dot( &m_vMoveDir, &vFaceNrm );
	if( bMap && fCheck >= 0.0f )
	{
		return false;	// ������ ����
	}

	if( !bOnlyAttack )
	{
 		D3DXVECTOR3 vReflect = m_vMoveDir - 2.0f * D3DXVec3Dot( &m_vMoveDir, &vFaceNrm ) * vFaceNrm;
		D3DXVec3Normalize( &m_vMoveDir, &vReflect );
	}

	if( bMap )
	{
		m_vAttackDir = m_vMoveDir;
		PlayWoundedSound( NULL );
	}
	else
	{
		m_vAttackDir = -vFaceNrm;
		D3DXVec3Normalize( &m_vAttackDir, &m_vAttackDir );
	}

	D3DXQUATERNION qtRot;
	ioMath::CalcDirectionQuaternion( qtRot, -m_vMoveDir );
	SetOrientation( qtRot );

	D3DXVec3Cross( &m_vRollAxis, &D3DXVECTOR3( 0.0f, FLOAT1, 0.0f ), &m_vMoveDir );

	m_bCheckCollision = true;

	return true;
}

void ioBallWeapon::Process( float fTimePerSec, ioPlayStage *pStage )
{
	DWORD dwLooseTime = 0;
	if( GetOwner() )
		dwLooseTime = GetOwner()->GetActionStopTotalDelay();

	if( m_dwCreatedTime + m_dwLiveTime + dwLooseTime < FRAMEGETTIME() )
	{
		pStage->CreateMapEffect( GetWeaponEndEffect(), GetPosition(), GetScale() );
		DestroyResource( pStage );

		SetWeaponDead();
		return;
	}

	if( !m_bCheckCollision )
		CheckChangeDir();

	float fMoveDist = GetMoveSpeed() * fTimePerSec;
	m_fCurMoveRange += fMoveDist;

	GetParentSceneNode()->Translate( m_vMoveDir * fMoveDist );

	UpdateWorldAttackBox( fTimePerSec, false );

	if( m_pFireMesh )
	{
		m_pFireMesh->Update( fTimePerSec * FLOAT1000 );
	}
}

bool ioBallWeapon::TestMapCollision( ioOpcodeShape *pShape, ioPlayStage *pStage, const D3DXMATRIX* pShapeWorldMat )
{
	//pShape�� ���� �� �浹 �˻縦 ���� �ϰ��� �Ҷ� TRUE
	//�Ϲ� ����� ��� ���� �޽��� 1�������� �Ͽ�¡�ý����� ��� N�� �̱� ������ TestMapCollision() �Լ��� ������ ȣ���
	int iFaceIndex = 0;

	// 1�� �浹 �������� üũ
	if( !pShape->FindIntersection( pShapeWorldMat, GetWorldAttackBox(true), &iFaceIndex ) )
		return false;

	D3DXVECTOR3 vNormal;
	pShape->GetTriNormalInModelSpace( iFaceIndex, vNormal );

	if( SetWallCollision( vNormal, true, false ) )
	{
		pStage->CreateMapEffect( GetExplosionEffectName(), GetWorldAttackBox( true ).GetCenter(), GetScale() );
		return true;
	}

	// 2�� �������� üũ
	D3DXVECTOR3 vEnd = GetPosition();
	D3DXVECTOR3 vOrgDiff = vEnd - m_vPrePos;
	float fOrgDiffSq = D3DXVec3LengthSq( &vOrgDiff );

	D3DXVECTOR3 vDir;
	D3DXVec3Normalize( &vDir, &vOrgDiff );

	D3DXVECTOR3 vColPoint;
	ioRay kLineRay( m_vPrePos, vDir );

	if( !pShape->TestIntersection( pShapeWorldMat, kLineRay, &iFaceIndex, &vColPoint ) )
		return false;

	pShape->GetTriNormalInModelSpace( iFaceIndex, vNormal );

	if( SetWallCollision( vNormal, true, false ) )
	{
		pStage->CreateMapEffect( GetExplosionEffectName(), GetWorldAttackBox( true ).GetCenter(), GetScale() );
	}

	return true;
}

void ioBallWeapon::RollWeapon( float fTimePerSec )
{
	if( m_pFireMesh )
	{
		GetParentSceneNode()->Rotate( m_vRollAxis, 1600.0f * fTimePerSec );
	}
}

void ioBallWeapon::DestroyResource( ioPlayStage *pStage )
{
	if( m_pFireMesh )
	{
		g_EffectFactory.DestroyEffect( m_pFireMesh );
		m_pFireMesh = NULL;
	}
}

ioWeapon::WeaponType ioBallWeapon::GetType() const
{
	return WT_BALL_WEAPON;
}

ActionStopType ioBallWeapon::GetActionStopType() const
{
	return m_ActionStopType;
}

ActionStopType ioBallWeapon::GetDieActionStopType() const
{
	return m_DieActionStopType;
}

D3DXVECTOR3 ioBallWeapon::GetAttackDir( const ioPlayEntity *pEntity ) const
{
	return m_vAttackDir;
}

const ioOrientBox& ioBallWeapon::GetLocalAttackBox() const
{
	return m_LocalAttackBox;
}

bool ioBallWeapon::IsCollision( ioPlayEntity *pEntity, bool bMapCollision )
{
	if( !pEntity )
		return false;

	bool bCollision = pEntity->IsWoundedCollision( GetWorldAttackBox(bMapCollision) );
	if( bCollision )
	{
		const ioOrientBox &kBallBox = GetWorldAttackBox();
		const ioOrientBox &kWoundedBox = pEntity->GetWorldCollisionBox();
		D3DXVECTOR3 vDiff = kBallBox.GetCenter() - kWoundedBox.GetCenter();
		vDiff.y = 0.0f;
		D3DXVec3Normalize( &vDiff, &vDiff );

		if( ToSymbolStruct(pEntity) || ToPushStruct(pEntity) || ToDummyChar(pEntity) || ToWallZone(pEntity) )
		{
			bool bShapeCol = false;
			ioEntityGroup *pGrp = pEntity->GetGroup();
			if( pGrp )
			{
				ioOpcodeShape *pShape = pGrp->GetOpcodeShape();
				if( pShape )
				{
					int iFaceIndex = 0;
					D3DXVECTOR3 vNormal;

					// 1��
					ioOrientBox kTest = GetWorldAttackBox(true);
					if( pShape->FindIntersection( pEntity->GetFullTransform(), kTest, &iFaceIndex ) )
					{
						pShape->GetTriNormalInModelSpace( iFaceIndex, vNormal );

						vNormal = pEntity->GetWorldOrientation() * vNormal;
						if( SetWallCollision( vNormal, true, false ) )
							bShapeCol = true;
					}

					// 2�� �������� üũ
					if( !bShapeCol )
					{
						D3DXVECTOR3 vStart = m_vPrePos - pEntity->GetWorldPosition();
						D3DXVECTOR3 vEnd = GetPosition() - pEntity->GetWorldPosition();
						D3DXVECTOR3 vOrgDiff = vEnd - vStart;

						D3DXVECTOR3 vDir;
						D3DXVec3Normalize( &vDir, &vOrgDiff );

						D3DXVECTOR3 vColPoint;
						ioRay kLineRay( vStart, vDir );

						if( pShape->TestIntersection( pEntity->GetFullTransform(), kLineRay, &iFaceIndex, &vColPoint ) )
						{
							pShape->GetTriNormalInModelSpace( iFaceIndex, vNormal );
							
							if( SetWallCollision( vNormal, true, false ) )
								bShapeCol = true;
						}
					}
				}
			}

			if( !bShapeCol )
			{
				SetWallCollision( vDiff, true, false );
			}
		}
		else if( ToBallStruct(pEntity) )
		{
			SetWallCollision( vDiff, false, false );
		}
		else if( ToBaseChar(pEntity) )
		{
			if( GetPiercingType() == CPT_NONE )
				SetWallCollision( vDiff, false, false );
			else
				SetWallCollision( vDiff, false, true );

			PlayWoundedSound( NULL );
		}
	}

	CheckPiercing( bCollision );

	if( bCollision )
		m_bCheckCollision = true;

	return bCollision;
}

void ioBallWeapon::ChangeBallWeapon( int iCurCnt,
									 const D3DXVECTOR3 &vPos,
									 const D3DXVECTOR3 &vMoveDir,
									 const D3DXVECTOR3 &vAttackDir )
{
	m_iCollisionCnt = iCurCnt;

	m_vPrePos = GetPosition();
	SetPosition( vPos );

	m_vMoveDir = vMoveDir;
	m_vAttackDir = vAttackDir;

	D3DXQUATERNION qtRot;
	ioMath::CalcDirectionQuaternion( qtRot, -m_vMoveDir );
	SetOrientation( qtRot );

	D3DXVec3Cross( &m_vRollAxis, &D3DXVECTOR3( 0.0f, FLOAT1, 0.0f ), &m_vMoveDir );
}

D3DXVECTOR3 ioBallWeapon::GetMoveDir()
{
	return m_vMoveDir;
}

void ioBallWeapon::CheckChangeDir()
{
	if( m_ChangeDirInfoList.empty() ) return;

	float fChangeRange = m_ChangeDirInfoList[0].m_fChangeRange;

	if( m_fCurMoveRange >= fChangeRange )
	{
		if( m_ChangeDirInfoList[0].m_fChangeSpeed > 0.0f )
			m_fCurMoveSpeed = m_ChangeDirInfoList[0].m_fChangeSpeed;

		RotateMoveDir( m_ChangeDirInfoList[0].m_fChangeAngleH, m_ChangeDirInfoList[0].m_fChangeAngleV );
		m_ChangeDirInfoList.erase( m_ChangeDirInfoList.begin() );
	}
}

void ioBallWeapon::RotateMoveDir( float fAngleH, float fAngleV )
{
	fAngleH = ioMath::ArrangeHead( fAngleH );
	fAngleV = ioMath::ArrangeHead( fAngleV );

	D3DXVECTOR3 vAxis;
	D3DXVec3Cross( &vAxis, &m_vMoveDir, &D3DXVECTOR3( 0.0f, -FLOAT1, 0.0f ) );

	// vertical
	D3DXQUATERNION qtRot;
	D3DXQuaternionRotationAxis( &qtRot, &vAxis, DEGtoRAD(fAngleV) );
	m_vMoveDir = qtRot * m_vMoveDir;

	// horizon
	D3DXQuaternionRotationAxis( &qtRot, &ioMath::UNIT_Y, DEGtoRAD(fAngleH) );
	m_vMoveDir = qtRot * m_vMoveDir;

	ioMath::CalcDirectionQuaternion( qtRot, -m_vMoveDir );
	SetOrientation( qtRot );
}

float ioBallWeapon::GetMoveSpeed()
{
	DWORD dwCurTime = FRAMEGETTIME();
	if( m_dwMoveDelayTime > 0 && m_dwCreatedTime+m_dwMoveDelayTime >= dwCurTime )
		return 0.0f;

	if( m_fCurMoveSpeed == 0.0f )
		m_fCurMoveSpeed = GetAttribute()->m_fMoveSpeed;

	return m_fCurMoveSpeed * m_fOwnerChargeSpeedRate;
}

float ioBallWeapon::GetMaxRange()
{
	return GetAttribute()->m_fMoveRange * m_fOwnerChargeRangeRate;
}


