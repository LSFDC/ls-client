

#include "stdafx.h"

#include "ioGameEntity.h"

static char szTestBuf[MAX_PATH];

ioGameEntity::ioGameEntity( ioEntityGroup *pGrp ) : m_pGroup(pGrp)
{
	m_bShadowCastEnable = false;
	m_iCurAlphaRate	   = MAX_ALPHA_RATE;
	m_iTargetAlphaRate = MAX_ALPHA_RATE;
	m_fCurBottomHeight = 0.0f;

	NeedUpdateBound();
	NeedUpdateBottomHeight();

	if( m_pGroup )
		m_pGroup->SetEventParent( this );

#ifdef USE_LAGTEST
	m_bUseLatency = false;
	m_bCanLatency = false;
	m_bIsLatency = false;

	m_dwLatency = 0;
	m_dwLatencyDelay = 0;
#endif
}

ioGameEntity::~ioGameEntity()
{
	ioEntityGroup *pGrp = GetGroup();
	if( pGrp )
	{
		ioSceneNode *pSceneNode = pGrp->GetParentSceneNode();
		if( pSceneNode )
		{
			pSceneNode->RemoveAndDestroyAllChildren();

			ioSceneManager *pSceneMgr = pSceneNode->GetSceneMgr();
			if( pSceneMgr )
			{
				pSceneMgr->DestroySceneNode( pSceneNode->GetUniqueID() );
				pSceneMgr->DestroyEntityGrp( pGrp );
				m_pGroup = NULL;
			}
		}
	}

#ifdef USE_LAGTEST
	std::list<sDelayBox*>::iterator it = m_listDelayBox.begin();
	while( it != m_listDelayBox.end() )
	{
		sDelayBox* pDelayBox = *it;
		SAFEDELETE(pDelayBox);
		it = m_listDelayBox.erase( it );
	}
#endif
}

void ioGameEntity::UpdateAfter( float fFrameGap )
{
	if( m_pGroup )
	{
		m_pGroup->UpdateAttachedObjects( fFrameGap );
	}

	ProcessAlphaRate( fFrameGap / FLOAT1000 );
}

bool ioGameEntity::IsNeedCheckMapCollision() const
{
	return false;
}

bool ioGameEntity::IsMountAvailable() const
{
	return false;
}

bool ioGameEntity::IsMountingAvailable() const
{
	return true;
}

bool ioGameEntity::IsNowMountEnable() const
{
	return false;
}

bool ioGameEntity::IsAllWeaponDefence() const
{
	return false;
}

void ioGameEntity::ProcessAlphaRate( float fTimePerSec )
{
	if( !IsNowAlphaChanging() )	return;

	if( m_iCurAlphaRate < m_iTargetAlphaRate )
	{
		m_iCurAlphaRate += MAX_ALPHA_RATE * fTimePerSec * 2.0f;
		if( m_iCurAlphaRate > m_iTargetAlphaRate )
			m_iCurAlphaRate = m_iTargetAlphaRate;
	}
	else
	{
		m_iCurAlphaRate -= MAX_ALPHA_RATE * fTimePerSec * 2.0f;
		if( m_iCurAlphaRate < m_iTargetAlphaRate )
			m_iCurAlphaRate = m_iTargetAlphaRate;
	}

	m_iCurAlphaRate = max( 0, min( m_iCurAlphaRate, MAX_ALPHA_RATE ) );

	m_pGroup->SetAlphaRate( m_iCurAlphaRate );
}

void ioGameEntity::UpdateBound() const
{
	ioNode *pParent = m_pGroup->GetParentNode();
	if( pParent )
	{
		m_WorldColBox = GetCollisionBox();
		m_WorldColBox.Transform( pParent->GetDerivedPosition(),
								 pParent->GetDerivedScale(),
								 pParent->GetDerivedOrientation() );

		m_WorldAxisBox.SetByOrientBox( m_WorldColBox );
	}
#ifdef USE_LAGTEST
	if( IsCanLatency() )
		(const_cast<ioGameEntity*>(this))->UpdateDelayBox();
#endif
}

void ioGameEntity::SetShadowCastEnable( bool bEnable )
{
	m_bShadowCastEnable = bEnable;
}

void ioGameEntity::ApplyShadowCast( bool bApply )
{
	if( !m_pGroup )	return;

	if( m_bShadowCastEnable && bApply )
	{
		m_pGroup->SetShadowCastEnable( true );
	}
	else
	{
		m_pGroup->SetShadowCastEnable( false );
	}
}

void ioGameEntity::SetAlphaRateDirect( int iRate )
{
	m_iCurAlphaRate = m_iTargetAlphaRate = iRate;

	if( m_pGroup )
	{
		m_pGroup->SetAlphaRate( iRate );
	}
}

void ioGameEntity::SetAttachEffectAlphaRateDirect( int iRate, bool bMinZero )
{
	if( m_pGroup )
	{
		m_pGroup->SetAttachEffectAlphaRate( iRate, bMinZero );
	}
}

void ioGameEntity::SetTargetAlphaRate( int iTarget )
{
	m_iTargetAlphaRate = iTarget;
}

bool ioGameEntity::HasEntity( const ioHashString &szMeshName )
{
	if( m_pGroup )
	{
		if( m_pGroup->GetEntity( szMeshName ) )
			return true;
	}

	return false;
}

ioEntity *ioGameEntity::GetEntity( const ioHashString &szMeshName )
{
	if( m_pGroup )
		return m_pGroup->GetEntity( szMeshName );
	
	return NULL;
}

ioEntity *ioGameEntity::AddEntity( const ioHashString &szMeshName )
{
	if( m_pGroup && !szMeshName.IsEmpty() )
	{
		return m_pGroup->AddEntity( szMeshName );
	}
	return NULL;
}

bool ioGameEntity::DestroyEntity( const ioHashString &szMeshName )
{
	if( m_pGroup && !szMeshName.IsEmpty() )
		return m_pGroup->DestroyEntity( szMeshName );

	return false;
}

void ioGameEntity::SetWorldPosition( const D3DXVECTOR3 &vPos )
{
	if( !ioMath::IsEqual( GetWorldPosition(), vPos ) )
	{
		m_pGroup->GetParentNode()->SetPosition( vPos );

		NeedUpdateBound();
		NeedUpdateBottomHeight();
	}
}

void ioGameEntity::SetWorldOrientation( const D3DXQUATERNION &qtRot )
{
	m_pGroup->GetParentNode()->SetOrientation( qtRot );
	
	NeedUpdateBound();
	NeedUpdateBottomHeight();
}

void ioGameEntity::SetWorldScale( const D3DXVECTOR3 &vScale )
{
	m_pGroup->GetParentNode()->SetScale( vScale );
	NeedUpdateBound();
}

void ioGameEntity::Translate( const D3DXVECTOR3 &vMove )
{
	m_pGroup->GetParentNode()->Translate( vMove );
	NeedUpdateBound();
	NeedUpdateBottomHeight();
}

void ioGameEntity::TranslateByWorldEntity( const D3DXVECTOR3 &vMove )
{
	m_pGroup->GetParentNode()->Translate( vMove );
	NeedUpdateBound();
	NeedUpdateBottomHeight();
}

void ioGameEntity::TranslateByConveyer( const D3DXVECTOR3 &vMove )
{
	m_pGroup->GetParentNode()->Translate( vMove );
	NeedUpdateBound();
	NeedUpdateBottomHeight();
}

void ioGameEntity::MapCollisionPushingTranslate( const D3DXVECTOR3 &vMoveAmt )
{
	Translate( vMoveAmt );
}

void ioGameEntity::DontMoveCollisionPushingTranslate( ioGameEntity *pPusher, const D3DXVECTOR3 &vMoveAmt )
{
	Translate( vMoveAmt );
}

void ioGameEntity::Scale( const D3DXVECTOR3 &vScale )
{
	m_pGroup->GetParentNode()->Scale( vScale );
	NeedUpdateBound();
	NeedUpdateBottomHeight();
}

void ioGameEntity::Scale( float fScaleX, float fScaleY, float fScaleZ )
{
	Scale( D3DXVECTOR3( fScaleX, fScaleY, fScaleZ ) );
}

const D3DXMATRIX* ioGameEntity::GetFullTransform() const
{
	return m_pGroup->GetParentNode()->GetFullTransform();
}

const D3DXVECTOR3& ioGameEntity::GetWorldPosition() const
{
#ifdef USE_LAGTEST
	if( m_bIsLatency )
		return GetDelayBox()->WorldPosition;
#endif
	return m_pGroup->GetParentNode()->GetDerivedPosition();
}

const D3DXQUATERNION& ioGameEntity::GetWorldOrientation() const
{
	return m_pGroup->GetParentNode()->GetDerivedOrientation();
}

const D3DXVECTOR3& ioGameEntity::GetWorldScale() const
{
	return m_pGroup->GetParentNode()->GetDerivedScale();
}

const ioOrientBox& ioGameEntity::GetWorldCollisionBox() const
{
	if( m_bNeedUpdateBound )
	{
		UpdateBound();

		m_bNeedUpdateBound = false;
	}

#ifdef USE_LAGTEST
	if( m_bIsLatency )
		return GetDelayBox()->WorldColBox;
#endif

	return m_WorldColBox;
}

const ioOrientBox& ioGameEntity::GetCollisionBox() const
{
	return m_pGroup->GetCollisionBox();
}

const ioAxisAlignBox& ioGameEntity::GetWorldAxisBox() const
{
	if( m_bNeedUpdateBound )
	{
		// for Update m_WorldAxisBox
		GetWorldCollisionBox();
	}

	return m_WorldAxisBox;
}

float ioGameEntity::GetMidHeightByRate( float fRate ) const
{
	D3DXVECTOR3 vPos = GetWorldPosition();
	ioOrientBox kBox = GetWorldCollisionBox();

	float fMidHeight;
	fMidHeight = kBox.GetCenter().y + kBox.GetExtents(1) - vPos.y;
	fMidHeight = vPos.y + fMidHeight * fRate;

	return fMidHeight;
}

D3DXVECTOR3 ioGameEntity::GetMidPositionByRate( float fRate ) const
{
	D3DXVECTOR3 vPos = GetWorldPosition();
	vPos.y = GetMidHeightByRate( fRate );

	return vPos;
}

float ioGameEntity::GetBottomHeight() const
{
	if( m_bNeedUpdateBottomHeight )
	{
		UpdateBottomHeight();
		m_bNeedUpdateBottomHeight = false;
	}

	return m_fCurBottomHeight;
}

UniqueObjID ioGameEntity::PlaySound( const ioHashString &rkName, bool bLoop )
{
	UniqueObjID eSoundID = 0;

	if( m_pGroup )
	{
		DWORD dwPlayMask = PSM_THREAD;
		if( bLoop )
			dwPlayMask |= PSM_LOOP;

		eSoundID = g_SoundMgr.PlaySound( rkName, m_pGroup->GetParentSceneNode(), dwPlayMask );
	}

	return eSoundID;
}
	
void ioGameEntity::Rotate( const D3DXVECTOR3 &vAxis, float fDegrees, ioNode::TransformSpace eRelativeTo )
{
	m_pGroup->GetParentNode()->Rotate( vAxis, fDegrees, eRelativeTo );
	NeedUpdateBound();
	NeedUpdateBottomHeight();
}
#ifdef USE_LAGTEST
void ioGameEntity::InitLatencyDelay()
{
	if( m_listDelayBox.size() == 0 )
	{
		m_bCanLatency = true;

		for( int i = 0; i < ICHECK_CNT; ++i )
		{
			sDelayBox* pDelayBox = new sDelayBox();
			m_listDelayBox.push_back( pDelayBox );
		}
	}
}
void ioGameEntity::SetLatencyDelay( DWORD dwLatencyDelay )
{
	m_bIsLatency = true;
	m_dwLatencyDelay = dwLatencyDelay;
}

void ioGameEntity::UpdateDelayBox()
{
	if( IsCanLatency() && !m_listDelayBox.empty() )
	{
		if( m_pGroup && m_pGroup->GetParentNode() )
		{
			sDelayBox* pDelayBox = m_listDelayBox.back();
			if( pDelayBox )
			{
				pDelayBox->dwLastTime = REALGETTIME();
				pDelayBox->WorldPosition = m_pGroup->GetParentNode()->GetDerivedPosition();
				pDelayBox->WorldColBox = m_WorldColBox;
				m_listDelayBox.pop_back();
				m_listDelayBox.push_front(pDelayBox);
			}
		}
	}
}

ioGameEntity::sDelayBox* ioGameEntity::GetDelayBox() const
{
	if( m_listDelayBox.size() == 0 )
	{
		ioGameEntity* pThis = const_cast<ioGameEntity*>(this);
		pThis->InitLatencyDelay();
		pThis->UpdateDelayBox();

		return *(m_listDelayBox.begin());
	}

	ioGameEntity::sDelayBox* pDelayBox = NULL;

	//가까운 시간
	DWORD dwCurTime = REALGETTIME();
	DWORD dwCheckTime = 0;

	//가져올 시간
	DWORD dwPrevTime = dwCurTime - m_dwLatencyDelay;	
	for( auto it = m_listDelayBox.begin(); it != m_listDelayBox.end(); ++it )
	{
		if( (*it)->dwLastTime == 0 )
			break;

		pDelayBox = *it;
		DWORD dwUpdateLatency = pDelayBox->dwLastTime;
		dwCheckTime = dwUpdateLatency;
		if( dwPrevTime > dwUpdateLatency )
			break;
	}

	// 있을 수 있나?
	if( pDelayBox == NULL )
	{
		pDelayBox = *(m_listDelayBox.begin());
	}

	//너무 차이가 크면 현재시간
	if( (dwPrevTime - dwCheckTime) > check_frm )
		pDelayBox = *(m_listDelayBox.begin());

	return pDelayBox;
}

void ioGameEntity::UpdateDelayBound()
{
	if( IsCanLatency() )
		UpdateDelayBox();
}

void ioGameEntity::UseLatencyDelay()
{
	m_bUseLatency = true;
	InitLatencyDelay();
}

#endif



