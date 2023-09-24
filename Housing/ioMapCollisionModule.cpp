#include "stdafx.h"
#include "ioBlockDBItem.h"

#include "ioMapCollisionModule.h"
#include "ioBlockWorld.h"

#include "../ioMapCollisionHelper.h"
#include "../ioWeapon.h"
#include "../ioBaseChar.h"

ioMapCollisionModule::ioMapCollisionModule()
{
}

ioMapCollisionModule::~ioMapCollisionModule()
{
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �� Entity ����Ʈ���� ���� �� ��������
//

float ioMapCollisionModule::GetHeight( float fXPos, float fZPos )
{
	return GetHeight( D3DXVECTOR3( fXPos, 100000.0f, fZPos ) );
}

float ioMapCollisionModule::GetHeight( float fXPos, float fYPos, float fZPos )
{
	return GetHeight( D3DXVECTOR3( fXPos, fYPos, fZPos ) );
}

float ioMapCollisionModule::GetHeight( D3DXVECTOR3 vPos )
{
	if( !m_pMapBase )
		return 0.0f;
	
	ioBlockList List;
	m_pMapBase->GetBlockByTop( vPos, List );
	for( int i = 0; i < (int)List.size(); ++i )
	{
		ioBlock* pBlock = List[i];
		if( pBlock && pBlock->GetCollShape() )
		{
			ioRay kYRay( D3DXVECTOR3( vPos.x, vPos.y, vPos.z ), -ioMath::UNIT_Y );
			D3DXVECTOR3 vColPoint( 0.0f, 0.0f, 0.0f );
			if( pBlock->GetCollShape()->TestIntersection( pBlock->GetTransform(), kYRay, NULL, &vColPoint ) )
			{
				return vColPoint.y;
			}
		}
	}

	return 0.0f;
}

float ioMapCollisionModule::GetHeightByOBB( float fXPos, float fZPos )
{
	return GetHeightByOBB( D3DXVECTOR3( fXPos, 100000.0f, fZPos ) );
}

float ioMapCollisionModule::GetHeightByOBB( float fXPos, float fYPos, float fZPos )
{
	return GetHeightByOBB( D3DXVECTOR3( fXPos, fYPos, fZPos ) );
}

float ioMapCollisionModule::GetHeightByOBB( D3DXVECTOR3 vPos )
{
	if( !m_pMapBase )
		return 0.0f;

	ioBlockList List;
	m_pMapBase->GetBlockByTop( vPos, List );
	for( int i = 0; i < (int)List.size(); ++i )
	{
		ioBlock* pBlock = List[i];
		if( !pBlock )
			continue;
		
		ioRay kYRay( D3DXVECTOR3( vPos.x, vPos.y, vPos.z ), -ioMath::UNIT_Y );
		D3DXVECTOR3 vColPoint( 0.0f, 0.0f, 0.0f );
		if( pBlock->TestIntersection( kYRay, &vColPoint ) )
		{
			return vColPoint.y;
		}
	}

	return 0.0f;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------
//
// ��ƼƼ�� �� �� �浹�� ��ƼƼ ��ġ ���(������ �̵� ��������)
// 
bool ioMapCollisionModule::ReCalculateMoveVectorByBlock( IN ioPlayEntity* pEntity,
														 IN const ioAxisAlignBox& kAxisMoveBox,									   
														 IN const D3DXVECTOR3& vMoveDir,
														 IN bool bFloating,
														 OUT D3DXVECTOR3 *pMove,
														 OUT bool &bCol )
{
	if( !pEntity )
		return true;
	
	if( !m_pMapBase )
		return true;

	D3DXVECTOR3 vPos = pEntity->GetMidPositionByRate();

	BlockMapBuffer List;
	m_pMapBase->GetAdjacentBlockList( vPos, pEntity->GetWorldCollisionBox().GetBoxRadius(), List );
	for( int i = 0; i < (int)List.size(); ++i )
	{
		ioBlock* pBlock = List.m_pBuffer[i];
		if( !pBlock || pBlock->IsCollChecked() || !pBlock->GetCollShape() )
			continue;

		pBlock->SetCollChecked( true );

		//���Ǿ� üũ
		ioSphere kSphere( vPos, pEntity->GetWorldCollisionBox().GetBoxRadius() );
		if( !ioMath::TestIntersection( kSphere, pBlock->GetCollSphere() ) )
			continue;

		if( !ioMapCollisionHelper::ReCalculateMoveVectorByWorldMesh( pEntity, 
																	 pEntity->GetWorldCollisionBox(),
																	 pBlock->GetCollShape(),
																	 pBlock->GetTransform(),
																	 g_BlockManager.GetBlockStartHeight(),
																	 vMoveDir,
																	 bFloating,
																	 pMove,
																	 bCol ) )
		{
			ResetColl( &List );
			return false;
		}
	}
	
	ResetColl( &List );
	return true;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------
//
// ��ƼƼ�� �� �� �浹 üũ �� ��ƼƼ �и� ó��
// 

void ioMapCollisionModule::CheckPlayEntityBlockCollision( ioGameEntity* pEntity, ioPlayStage* pStage )
{
	if( !m_pMapBase )
		return;

	if( !pEntity )
		return;

	if( ToWorldDecoration( pEntity ) )
		return;
		
	ioBaseChar* pChar = ToBaseChar( pEntity );
	if( pChar && pChar->GetState() == CS_BLOCK_INTEPLAY )
		return;

	TilePos Pos;
	m_pMapBase->WorldCoordToTileCoord( pEntity->GetWorldPosition(), Pos );

	D3DXVECTOR3 vPos = pEntity->GetMidPositionByRate();

	BlockMapBuffer List;
	m_pMapBase->GetAdjacentBlockList( vPos, pEntity->GetWorldCollisionBox().GetBoxRadius(), List );
	for( int i = 0; i < (int)List.size(); ++i )
	{
		ioBlock* pBlock = List.m_pBuffer[i];
		if( !pBlock || pBlock->IsCollChecked() || !pBlock->GetCollShape() )
			continue;

		pBlock->SetCollChecked( true );

		//���Ǿ� üũ
		ioSphere kSphere( vPos, pEntity->GetWorldCollisionBox().GetBoxRadius() );
		if( !ioMath::TestIntersection( kSphere, pBlock->GetCollSphere() ) )
			continue;

		if( ioMapCollisionHelper::CheckPlayEntityMapCollision( pStage, pEntity, pBlock->GetCollShape(), 50, pBlock->GetTransform() ) )
		{
			//�� �浹 ���� �̺�Ʈ ó��
		}
	}

	ResetColl( &List );
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �������� ���� ���̿� ���� ���� �ϴ��� �˻�
// 

bool ioMapCollisionModule::CheckCollLine( IN const D3DXVECTOR3 &vStart, IN const D3DXVECTOR3 &vEnd, OUT D3DXVECTOR3& vColPos )
{
	if( !m_pMapBase )
		return false;

	D3DXVECTOR3 vDiff = vEnd - vStart;
	float fDiff = D3DXVec3Length( &vDiff );

	BlockMapBuffer List;
	m_pMapBase->GetAdjacentBlockList( vStart, fDiff, List );
	for( int i = 0; i < (int)List.size(); ++i )
	{
		ioBlock* pBlock = List.m_pBuffer[i];
		if( !pBlock || pBlock->IsCollChecked() || !pBlock->GetCollShape() )
			continue;

		pBlock->SetCollChecked( true );

		D3DXVECTOR3 vDir = vEnd - vStart;
		D3DXVec3Normalize( &vDir, &vDir );
		ioRay kLineRay( vStart, vDir );

		if( !ioMath::TestIntersection( kLineRay, pBlock->GetCollSphere() ) )
			continue;

		if( ioMapCollisionHelper::CheckCollisionLine( vColPos, pBlock->GetCollShape(), vStart, vEnd, pBlock->GetTransform() ) )
		{
			ResetColl( &List );
			return true;
		}
	}

	ResetColl( &List );
	return false;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------
//
// ���̿� �浹 �ϴ� ���� �ִ��� �˻�
// 

bool ioMapCollisionModule::TestBlockMeshCollision( const ioRay &rkRay, D3DXVECTOR3 *pColPoint )
{
	if( !m_pMapBase )
		return false;

	BlockMapBuffer List;
	float fDiff = D3DXVec3Length( &rkRay.GetDirection() );
	m_pMapBase->GetAdjacentBlockList( rkRay.GetOrigin(), fDiff, List );

	for( int i = 0; i < (int)List.size(); ++i )
	{
		ioBlock* pBlock = List.m_pBuffer[i];
		if( !pBlock || pBlock->IsCollChecked() || !pBlock->GetCollShape() )
			continue;

		pBlock->SetCollChecked( true );

		if( !ioMath::TestIntersection( rkRay, pBlock->GetCollSphere() ) )
			continue;

		ioOpcodeShape *pShape = pBlock->GetCollShape();
		if( pShape && pShape->TestIntersection( pBlock->GetTransform(), rkRay, NULL, pColPoint ) )
		{
			ResetColl( &List );
			return true;
		}
	}

	ResetColl( &List );
	return false;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------
//
// OBB�ڽ��� �浹 �ϴ� ���� �ִ��� �˻�
// 

bool ioMapCollisionModule::TestBlockMeshCollision( const ioOrientBox &rkBox )
{
	if( !m_pMapBase )
		return false;

	BlockMapBuffer List;
	m_pMapBase->GetAdjacentBlockList( rkBox.GetCenter(), rkBox.GetBoxRadius(), List );
	for( int i = 0; i < (int)List.size(); ++i )
	{
		ioBlock* pBlock = List.m_pBuffer[i];
		if( !pBlock || pBlock->IsCollChecked() || !pBlock->GetCollShape() )
			continue;

		pBlock->SetCollChecked( true );

		if( !ioMath::TestIntersection( rkBox, pBlock->GetCollSphere() ) )
			continue;

		ioOpcodeShape *pShape = pBlock->GetCollShape();
		if( pShape && pShape->TestIntersection( pBlock->GetTransform(), rkBox ) )
		{
			ResetColl( &List );
			return true;
		}
	}

	ResetColl( &List );
	return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �� CMS �Ž��� ���̸� �浹 üũ(�浹 ������ ���������� �Ÿ��� fCheckRange ���� ū�� üũ)
//

bool ioMapCollisionModule::CheckCollRayVsBlockByRange( IN const ioRay& rkRay, IN float fCheckRange )
{
	if( !m_pMapBase )
		return false;

	BlockMapBuffer List;
	m_pMapBase->GetAdjacentBlockList( rkRay.GetOrigin(), fCheckRange, List );

	for( int i = 0; i < (int)List.size(); ++i )
	{
		ioBlock* pBlock = List.m_pBuffer[i];
		if( !pBlock || pBlock->IsCollChecked() || !pBlock->GetCollShape() )
			continue;

		pBlock->SetCollChecked( true );

		if( !ioMath::TestIntersection( rkRay, pBlock->GetCollSphere() ) )
			continue;

		if( ioMapCollisionHelper::CheckRayVsMapCollisionByRange( rkRay, pBlock->GetCollShape(), pBlock->GetTransform(), fCheckRange ) )
		{
			ResetColl( &List );
			return true;
		}
	}

	ResetColl( &List );
	return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �� CMS �Ž��� ���� �浹 üũ
//

bool ioMapCollisionModule::CheckWeaponVSBlockCollision( IN ioPlayStage* pStage, IN ioWeapon* pWeapon, bool bPiercingCheck /*= false */ )
{
	if( !m_pMapBase )
		return false;

	BlockMapBuffer List;
	m_pMapBase->GetAdjacentBlockList( pWeapon->GetPosition(), pWeapon->GetWorldAttackBox(true).GetBoxRadius(), List );

	for( int i = 0; i < (int)List.size(); ++i )
	{
		ioBlock* pBlock = List.m_pBuffer[i];
		if( !pBlock )
			continue;

		if( pBlock->IsCollChecked() )
			continue;

		if( !pBlock->GetCollShape() )
			continue;

		pBlock->SetCollChecked( true );

		if( !ioMath::TestIntersection( pWeapon->GetWorldAttackBox(true), pBlock->GetCollSphere() ) )
			continue;

		if( bPiercingCheck && ioMapCollisionHelper::CheckWeaponPiercingCollision( pWeapon, pBlock->GetCollShape(), pBlock->GetTransform() ) )
		{
			ResetColl( &List );
			return true;
		}

		if( pWeapon->TestMapCollision( pBlock->GetCollShape(), pStage, pBlock->GetTransform() ) )
		{
			ResetColl( &List );
			return true;
		}
	}

	ResetColl( &List );
	return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// ���� �浹 üũ
//
// 
bool ioMapCollisionModule::CheckBlockVSBlockCollision( ioBlock* pBlock, const TilePos& NextPos, float fNextRotate )
{
	if( !m_pMapBase )
		return false;

	if( !pBlock )
		return false;

	BlockMapBuffer List;
	if( pBlock->GetBlockType() == ioBlock::BT_TILE )
	{
		m_pMapBase->GetAdjacentBlockListByTile( pBlock->GetTilePos(), pBlock->GetCollBlockSphere().GetRadius(), List );
	}
	else
	{
		m_pMapBase->GetAdjacentBlockList( pBlock->GetWorldPosition(), pBlock->GetCollBlockSphere().GetRadius(), List );
	}

	for( int i = 0; i < (int)List.size(); ++i )
	{
		ioBlock* pDestBlock = List.m_pBuffer[i];
		if( !pDestBlock || pDestBlock->IsCollChecked() )
			continue;

		pBlock->SetCollChecked( true );
		if( pBlock->TestPreIntersection( pDestBlock, NextPos, fNextRotate ) )
		{
			ResetColl( &List );
			return true;
		}
	}

	ResetColl( &List );
	return false;
}

void ioMapCollisionModule::ResetColl( BlockMapBuffer* List )
{
	if( List == NULL )
		return;

	for( int i = 0; i < (int)List->size(); ++i )
	{
		ioBlock* pBlock = List->m_pBuffer[i];
		if( pBlock && pBlock->IsCollChecked() )
			pBlock->SetCollChecked( false );
	}
}