

#include "stdafx.h"

#include "ioBaseChar.h"
#include "ioPlayStage.h"

#include "ioUroborusGuideArrow.h"

#include "ioUroborusWeapon3.h"

#include "ItemDefine.h"
#include "SkillDefine.h"

ActionStopType ioUroborusWeapon3::m_ActionStopType = AST_NONE;
ActionStopType ioUroborusWeapon3::m_DieActionStopType = AST_NONE;

ioUroborusWeapon3::ioUroborusWeapon3( ioBaseChar *pOwner, const WeaponAttribute *pAttr )
	: ioWeapon( pOwner, pAttr )
{
	//SetPeculiarity( WP_WOUND_NO_SOUND | WP_DEF_NO_SOUND );

	m_pFireMesh = NULL;
	m_UroborusState = US_MOVE;

	m_bResistance = false;
	m_fCurMoveSpeed = 0.0f;
	m_fCurMoveRange = 0.0f;

	m_fTargetCurRange = 0.0f;

	m_pGuideArrow = NULL;
	m_vTargetMovePos = ioMath::VEC3_ZERO;

	m_bTraceTarget = false;
	m_bSkillWeapon = false;
	m_bOneTimeColCheck = false;
	m_bColOccur = false;

	m_fTargetMaxRange = 0;

	m_StartPos = ioMath::VEC3_ZERO;
}

ioUroborusWeapon3::~ioUroborusWeapon3()
{
	if( m_pFireMesh )
	{
		LOG.PrintTimeAndLog( 0, "ioUroborusWeapon3::~ioUroborusWeapon3 - UnReleased Resouce" );
	}

	SAFEDELETE( m_pGuideArrow );
}

bool ioUroborusWeapon3::InitResource()
{
	return true;
}

void ioUroborusWeapon3::LoadProperty( ioINILoader &rkLoader )
{
	ioWeapon::LoadProperty( rkLoader );

	char szBuf[MAX_PATH] = "";

	rkLoader.LoadString_e( "red_team_effect", "", szBuf, MAX_PATH );
	m_RedTeamEffect = szBuf;
	rkLoader.LoadString_e( "blue_team_effect", "", szBuf, MAX_PATH );
	m_BlueTeamEffect = szBuf;

	rkLoader.LoadString_e( "target_move_effect", "", szBuf, MAX_PATH );
	m_TargetMoveEffect = szBuf;

	m_fTargetMoveSpeed = rkLoader.LoadFloat_e( "target_move_speed", 0.0f );

	//
	ioBaseChar *pOwner = GetOwner();
	if( pOwner )
	{
		ioPlayStage *pStage = pOwner->GetCreator();

		SAFEDELETE( m_pGuideArrow );
		m_pGuideArrow = new ioUroborusGuideArrow( pStage );

		m_pGuideArrow->Initialize( rkLoader );
		m_pGuideArrow->SetMaxArrowRange( GetMaxRange() );
	}

	//////////////////////////////////////////////////////////////////////////
	m_bUseEffectStartPosMode	= rkLoader.LoadBool_e( "use_effect_start_mode", false );
	m_bUseStartReturnPos		= rkLoader.LoadBool_e( "use_start_return_pos", false );
	m_bUseRotateWeapon			= rkLoader.LoadBool_e( "use_rotate_weapon", false );
	m_fRotateAngle				= rkLoader.LoadFloat_e( "rotate_angle", 0.f );
	m_fRotateAngle2				= rkLoader.LoadFloat_e( "rotate_angle2", 0.f );
	
	m_bUseReturnWeapon			= rkLoader.LoadBool_e( "use_return_weapon", false );
	m_fReturnMaxLength			= rkLoader.LoadFloat_e( "max_move_length", 0.f );

	m_bUseDelayStartTime		= rkLoader.LoadBool_e( "use_delay_start_time", false );
	m_dwDelayStartTime			= (DWORD)rkLoader.LoadInt_e( "delay_start_time", 0 );

	m_bNoItemCheck = rkLoader.LoadBool_e( "no_item_check", false );
	m_bNoAllCheck  = rkLoader.LoadBool_e( "no_all_check", false );
	m_bCheckLand   = rkLoader.LoadBool_e( "check_land_height", false );

	m_bFixedDir = rkLoader.LoadBool_e( "fixed_dir", false );
}

void ioUroborusWeapon3::SetWeaponIndex( DWORD dwIndex )
{
	m_dwWeaponIdx = dwIndex;
}

void ioUroborusWeapon3::SetTeam( ioPlayStage *pStage, TeamType eType )
{
	if( !pStage ) return;

	ioWeapon::SetTeam( pStage, eType );

	DestroyResource();

	ioHashString szCurWeaponEffect;

	if( eType == TEAM_BLUE )
	{
		szCurWeaponEffect = m_BlueTeamEffect;
	}
	else if( eType == TEAM_RED )
	{
		szCurWeaponEffect = m_RedTeamEffect;
	}
	else //개인전
	{
		ioBaseChar *pOwner = GetOwner();
		if( pOwner && pOwner->GetSingleConvertTeam() == TEAM_BLUE )
		{
			szCurWeaponEffect = m_BlueTeamEffect;
		}
		else
		{
			szCurWeaponEffect = m_RedTeamEffect;
		}
	}

	if( !SetFireMesh( szCurWeaponEffect ) )
		DestroyResource();
}

bool ioUroborusWeapon3::SetFireMesh( const ioHashString &szFileName )
{
	ioBaseChar *pOwner = GetOwner();
	if( !pOwner )	return false;

	m_pFireMesh = g_EffectFactory.CreateEffect( szFileName );
	if( !m_pFireMesh )	return false;

	GetParentSceneNode()->AttachObject( m_pFireMesh );
	GetParentSceneNode()->SetScale( ioMath::UNIT_ALL );

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

	AddEffectEvent( m_pFireMesh );
	return true;
}


void ioUroborusWeapon3::SetMoveDir( const D3DXVECTOR3 &vDir )
{
	D3DXVECTOR3 vDirTemp = vDir;
	if( m_bFixedDir )
		vDirTemp = D3DXVECTOR3( 1.f, 0.f, 0.f );

	if( m_bUseRotateWeapon )
	{
		float fCurAngleX = 0.0f;
		float fCurAngleY = 0.0f;
		fCurAngleX = ioMath::ArrangeHead( m_fRotateAngle );
		fCurAngleY = ioMath::ArrangeHead( m_fRotateAngle2 );

		D3DXQUATERNION qtRot;
		D3DXVECTOR3 vAxis;
		D3DXVec3Cross( &vAxis, &vDirTemp, &D3DXVECTOR3( 0.0f, -1.0f, 0.0f ) );

		// vertical
		D3DXQuaternionRotationAxis( &qtRot, &vAxis, DEGtoRAD(fCurAngleY) );
		vDirTemp = qtRot * vDirTemp;

		// horizon
		D3DXQuaternionRotationAxis( &qtRot, &ioMath::UNIT_Y, DEGtoRAD(fCurAngleX) );
		vDirTemp = qtRot * vDirTemp;
	}

	m_vMoveDir = vDirTemp;
	D3DXVec3Normalize( &m_vMoveDir, &m_vMoveDir );

	D3DXQUATERNION qtRot = Help::ConvertDirToQuaternion( m_vMoveDir );
	//ioMath::CalcDirectionQuaternion( qtRot, m_vMoveDir );
	SetOrientation( qtRot );
}

D3DXVECTOR3 ioUroborusWeapon3::GetMoveDir()
{
	return m_vMoveDir;
}


D3DXVECTOR3 ioUroborusWeapon3::GetAttackDir( const ioPlayEntity *pEntity ) const
{
	return m_vMoveDir;
}

void ioUroborusWeapon3::SetStartPosition( const D3DXVECTOR3 &vPos, ioPlayStage *pStage )
{
	if( m_pFireMesh && m_pFireMesh->GetSkeletonName().IsEmpty() && m_pFireMesh->HasLinkedSystem() && !m_bUseEffectStartPosMode )
		return;

	SetPosition( vPos );

	m_StartPos = vPos;

	if( m_bUseEffectStartPosMode )
		SetGuidArrowPosMode();
}


void ioUroborusWeapon3::SetGuidArrowPosMode()
{
	if( m_pGuideArrow )
		m_pGuideArrow->SetStartPosMode( true, GetPosition() );
}

void ioUroborusWeapon3::Process( float fTimePerSec, ioPlayStage *pStage )
{
	if( !CheckPiercing() )
		return;

	DWORD dwCurTime = FRAMEGETTIME();

	switch( m_UroborusState )
	{
	case US_MOVE:
	//case US_WAIT:
		if( m_bUseDelayStartTime && dwCurTime < (m_dwCreatedTime+m_dwDelayStartTime) )
			return;

		OnFloating( fTimePerSec, pStage );
		
		if( m_bUseReturnWeapon && m_fCurMoveRange > m_fReturnMaxLength )
			SetTargetMoveState( false );

		break;
	case US_TARGET_MOVE:
		OnTargetMove( fTimePerSec, pStage );
		break;
	case US_END:
		if( !m_pFireMesh || !m_pFireMesh->IsLive() )
		{
			CheckCreateWeaponList();
			DestroyResource( pStage );
			SetWeaponDead();
			return;
		}
		break;
	}

	if( m_pFireMesh )
	{
		m_pFireMesh->Update( fTimePerSec * FLOAT1000 );
	}

	if( m_bCheckLand && CheckLandHeight( pStage ) )
	{
		SetEndState( false, false, false );
	}

	UpdateWorldAttackBox( fTimePerSec, false );
}


bool ioUroborusWeapon3::CheckLandHeight( ioPlayStage *pStage )
{
	D3DXVECTOR3 vPos = GetPosition();
	float fMapHeight = pStage->GetMapHeight( vPos.x, vPos.z );
	if( vPos.y < fMapHeight )
		return true;

	return false;
}


bool ioUroborusWeapon3::TestMapCollision( ioOpcodeShape *pShape, ioPlayStage *pStage, const D3DXMATRIX* pShapeWorldMat )
{
	//pShape에 대한 맵 충돌 검사를 중지 하고자 할때 TRUE
	//일반 모드의 경우 월드 메쉬가 1개이지만 하우징시스템의 경우 N개 이기 때문에 TestMapCollision() 함수가 여러번 호출됨

	if( m_UroborusState != US_MOVE && m_UroborusState != US_TARGET_MOVE )
		return true;

	if( pShape->TestIntersection( pShapeWorldMat, GetWorldAttackBox(true) ) )
	{
		ioBaseChar *pOwner = GetOwner();
		if( pOwner )
		{
			pStage->CreateMapEffect( GetExplosionEffectName(), GetWorldAttackBox(true).GetCenter(), GetScale() );

			PlayWoundedSound( NULL );

			if( pOwner->IsNeedProcess() )
			{
				switch( m_UroborusState )
				{
				case US_MOVE:
					SetEndState( true, false, true );
					break;
				case US_TARGET_MOVE:
					SetEndState( true, false, true );
					break;
				}
			}
		}

		return true;
	}

	return false;
}

void ioUroborusWeapon3::OnFloating( float fTimePerSec, ioPlayStage *pStage )
{
	if( !CheckOwnerState() )
	{
		SetEndState( false, false, false );
		return;
	}

	ioBaseChar *pOwner = GetOwner();
	float fCurSpeed = GetMoveSpeed();
	float fMoveDist = fCurSpeed * fTimePerSec;

	m_vPrePos = GetPosition();

	//////////////////////////////////////////////////////////////////////////
	if( m_bTraceTarget && !m_TraceTargetName.IsEmpty() )
	{
		ioBaseChar *pTarget = pStage->GetBaseChar( m_TraceTargetName );
		if( pTarget )
		{
			if( pTarget->IsSystemState() )
				m_TraceTargetName.Clear();

			if( !pTarget->IsEnableTarget() )
				m_bTraceTarget = false;

			if( m_bTraceTarget )
			{
				D3DXVECTOR3 vDiff = pTarget->GetMidPositionByRate() - m_vPrePos;
				D3DXVec3Normalize( &vDiff, &vDiff );
				m_vMoveDir = vDiff;
			}
		}
	}
	

	//////////////////////////////////////////////////////////////////////////
	D3DXVECTOR3 vNewPos = m_vPrePos + (m_vMoveDir * fMoveDist);
	m_fCurMoveRange += fMoveDist;

	float fMaxRange = GetMaxRange() + m_fGrowthMoveRange;
	if( m_fCurMoveRange < fMaxRange )
	{
		//if( m_UroborusState != US_WAIT )
			SetPosition( vNewPos );
	}
	else
	{
		if( pOwner && pOwner->IsNeedProcess() )
		{
			SetEndState( false, false, true );
		}
	}

	ProcessGuidArrow();
}

void ioUroborusWeapon3::OnTargetMove( float fTimePerSec, ioPlayStage *pStage )
{
	if( !CheckOwnerState() )
	{
		SetEndState( false, false, false );
		return;
	}

	DWORD dwCurTime = FRAMEGETTIME();

	D3DXVECTOR3 vCurDir = m_vTargetMovePos - GetPosition();
	D3DXVec3Normalize( &vCurDir, &vCurDir );

	float fMoveDist = m_fTargetMoveSpeed * fTimePerSec;

	m_vPrePos = GetPosition();
	D3DXVECTOR3 vNewPos = m_vPrePos + (vCurDir * fMoveDist);
	m_fTargetCurRange += fMoveDist;

	if( m_fTargetCurRange < m_fTargetMaxRange )
	{
		SetPosition( vNewPos );
	}
	else
	{	
		SetEndState( false, true, false );
		return;
	}

	ProcessGuidArrow();
}

void ioUroborusWeapon3::DestroyResource( ioPlayStage *pStage )
{
	if( m_pFireMesh )
	{
		g_EffectFactory.DestroyEffect( m_pFireMesh );
		m_pFireMesh = NULL;
	}
}

const ioOrientBox& ioUroborusWeapon3::GetLocalAttackBox() const
{
	return m_LocalAttackBox;
}

ioWeapon::WeaponType ioUroborusWeapon3::GetType() const
{
	return WT_UROBORUS3;
}

ActionStopType ioUroborusWeapon3::GetActionStopType() const
{
	return m_ActionStopType;
}

ActionStopType ioUroborusWeapon3::GetDieActionStopType() const
{
	return m_DieActionStopType;
}

float ioUroborusWeapon3::GetMoveSpeed()
{
	DWORD dwCurTime = FRAMEGETTIME();
	if( m_dwMoveDelayTime > 0 && m_dwCreatedTime+m_dwMoveDelayTime >= dwCurTime )
		return 0.0f;

	if( m_fCurMoveSpeed == 0.0f )
		m_fCurMoveSpeed = GetAttribute()->m_fMoveSpeed;

	return m_fCurMoveSpeed * m_fOwnerChargeSpeedRate;
}

float ioUroborusWeapon3::GetMaxRange()
{
	return GetAttribute()->m_fMoveRange * m_fOwnerChargeRangeRate;
}

void ioUroborusWeapon3::UpdateWorldAttackBox( float fTimePerSec, bool bSetNull )
{
	if( !IsLive() || bSetNull )
	{
		m_WorldAttackBox.SetNull();
		m_WorldAttackBoxForMap.SetNull();
		return;
	}

	m_WorldAttackBox = GetLocalAttackBox();
	m_WorldAttackBoxForMap = m_WorldAttackBox;

	float fMapCollisionRate = GetAttribute()->m_fMapCollisionBoxRate;
	m_WorldAttackBoxForMap.SetExtents( 0, m_WorldAttackBox.GetExtents(0)*fMapCollisionRate );
	m_WorldAttackBoxForMap.SetExtents( 1, m_WorldAttackBox.GetExtents(1)*fMapCollisionRate );
	m_WorldAttackBoxForMap.SetExtents( 2, m_WorldAttackBox.GetExtents(2)*fMapCollisionRate );

	if( !m_WorldAttackBox.IsNull() )
	{
		if( m_fOwnerChargeScaleRate > FLOAT1 )
		{
			D3DXVECTOR3 vScale( m_fOwnerChargeScaleRate, m_fOwnerChargeScaleRate, m_fOwnerChargeScaleRate );
			m_WorldAttackBox.Transform( GetPosition(), vScale, GetOrientation() );
			m_WorldAttackBoxForMap.Transform( GetPosition(), vScale, GetOrientation() );
		}
		else
		{
			m_WorldAttackBox.Transform( GetPosition(), ioMath::UNIT_ALL, GetOrientation() );
			m_WorldAttackBoxForMap.Transform( GetPosition(), ioMath::UNIT_ALL, GetOrientation() );
		}
	}
}

void ioUroborusWeapon3::ApplyWeaponSync( SP2Packet &rkPacket, ioPlayStage *pStage )
{
	int iState;
	rkPacket >> iState;

	switch( iState )
	{
	case US_TARGET_MOVE:
		SetTargetMoveState( false );
		break;
	case US_END:
		{
			bool bSkillEnd, bCheckCombo;
			rkPacket >> bSkillEnd >> bCheckCombo;

			SetEndState( bSkillEnd, bCheckCombo, false );
		}
		break;
	}
}


void ioUroborusWeapon3::SetTargetMoveState( bool bSendNet )
{
	if( m_UroborusState == US_TARGET_MOVE )
		return;

	m_UroborusState = US_TARGET_MOVE;

	ioBaseChar *pOwner = GetOwner();
	if( !pOwner )
		return;

	if( m_bUseStartReturnPos )
	{
		m_vTargetMovePos = m_StartPos;
		D3DXVECTOR3 vPos =	GetPosition();
		vPos = m_vTargetMovePos - vPos;
		D3DXVec3Normalize( &vPos, &vPos );

		m_vTargetMovePos = m_vTargetMovePos - vPos*FLOAT10;

		D3DXVECTOR3 vDiff = m_vTargetMovePos - GetPosition();
		m_fTargetMaxRange = D3DXVec3Length( &vDiff );
	}
	else
	{
		//Case1
		//스킬에서 사용할때
		ioTraceSkill2 *pTraceSkill2 = ToTraceSkill2( pOwner->GetCurActiveSkill() );
		ioTraceSkill3 *pTraceSkill3 = ToTraceSkill3( pOwner->GetCurActiveSkill() );
		if(	m_bSkillWeapon && pTraceSkill2 /*&& pTraceSkill2->GetName() == GetWeaponBySkillName()*/ )
		{
			m_vTargetMovePos = pTraceSkill2->CalcReturnUroborusPosition( this );

			D3DXVECTOR3 vPos =	GetPosition();
			vPos = m_vTargetMovePos - vPos;
			D3DXVec3Normalize( &vPos, &vPos );


			m_vTargetMovePos = m_vTargetMovePos - vPos*FLOAT10;


			D3DXVECTOR3 vDiff = m_vTargetMovePos - GetPosition();
			m_fTargetMaxRange = D3DXVec3Length( &vDiff );
		}
		else if( m_bSkillWeapon && pTraceSkill3 )
		{
			m_vTargetMovePos = pTraceSkill3->CalcReturnUroborusPosition( this );

			D3DXVECTOR3 vPos =	GetPosition();
			vPos = m_vTargetMovePos - vPos;
			D3DXVec3Normalize( &vPos, &vPos );

			m_vTargetMovePos = m_vTargetMovePos - vPos*FLOAT10;

			D3DXVECTOR3 vDiff = m_vTargetMovePos - GetPosition();
			m_fTargetMaxRange = D3DXVec3Length( &vDiff );
		}
		else
		{
			//Case2
			//아이템에서 사용할때 
			ioWeaponItem *pWeapon = pOwner->GetWeapon();
			if( !pWeapon )
			{
				SetWeaponDead();
				return;
			}

			ioLuciferItem *pLuciferItem = ToLuciferItem( pWeapon );
			if( !pLuciferItem )
			{
				SetWeaponDead();
				return;
			}

			if( pOwner->GetState() != CS_LUCIFER_FLY )
			{
				SetWeaponDead();
				return;
			}

			m_vTargetMovePos = pLuciferItem->CalcReturnUroborusPosition( this );

			D3DXVECTOR3 vDiff = m_vTargetMovePos - GetPosition();
			m_fTargetMaxRange = D3DXVec3Length( &vDiff );
		}
	}

	//////////////////////////////////////////////////////////////////////////
	if( !m_TargetMoveEffect.IsEmpty() )
	{
		ioMapEffect *pEndEffect = pOwner->GetCreator()->CreateMapEffect( m_TargetMoveEffect, GetPosition(), GetScale() );
		if( pEndEffect )
		{
			D3DXQUATERNION qtRot = GetOrientation();
			pEndEffect->SetWorldOrientation( qtRot );
		}
	}
	//////////////////////////////////////////////////////////////////////////

	if( bSendNet )
	{
		SP2Packet kPacket( CUPK_WEAPON_SYNC );
		kPacket << GetOwnerName();
		kPacket << m_dwWeaponIdx;
		kPacket << m_UroborusState;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

void ioUroborusWeapon3::SetEndState( bool bSkillEnd, bool bCheckCombo, bool bSendNet )
{
	if( m_UroborusState == US_END )
		return;

	m_UroborusState = US_END;

	ioBaseChar *pOwner = GetOwner();
	if( !pOwner ) 
		return;

	ioPlayStage *pStage = pOwner->GetCreator();
	if( !GetWeaponEndEffect().IsEmpty() )
	{
		ioMapEffect *pEndEffect = pStage->CreateMapEffect( GetWeaponEndEffect(), GetPosition(), GetScale() );
		if( pEndEffect )
		{
			D3DXQUATERNION qtRot = GetOrientation();
			pEndEffect->SetWorldOrientation( qtRot );
		}
	}

	if( m_pFireMesh )
	{
		SAFEDELETE( m_pGuideArrow );
		m_pFireMesh->EndEffectGrace();
	}

	if( bSendNet && pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_WEAPON_SYNC );
		kPacket << GetOwnerName();
		kPacket << m_dwWeaponIdx;
		kPacket << m_UroborusState;
		kPacket << bSkillEnd;
		kPacket << bCheckCombo;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

bool ioUroborusWeapon3::IsCollision( ioPlayEntity *pEntity, bool bMapCollision )
{
	if( m_UroborusState != US_MOVE && m_UroborusState != US_TARGET_MOVE )
		return false;

	if( m_bOneTimeColCheck && m_bColOccur )
		return false;

	ioBaseChar *pTarget = ToBaseChar( pEntity );
	if( pTarget && !pTarget->IsNeedProcess() )
		return false;
	else if( !pTarget )
	{
		if( !GetOwner() || !GetOwner()->IsNeedProcess() )
			return false;
	}

	//if( pTarget->GetState() == CS_FLOAT_STATE )
		//return false;

	bool bCollision = false;

	D3DXVECTOR3 vColPoint( 0.0f, 0.0f, 0.0f );
	if( pEntity->IsWoundedCollision( GetWorldAttackBox(bMapCollision), &vColPoint ) )
		bCollision = true;

	CheckPiercing( bCollision );

	if( m_bOneTimeColCheck && bCollision )
		m_bColOccur  =true;

	//if( bCollision )
		//SetWaitState( pTarget );

	return bCollision;
}

DWORD ioUroborusWeapon3::GetCollisionEnableTime() const
{
	if( m_UroborusState == US_MOVE || m_UroborusState == US_TARGET_MOVE )
		return m_dwCreatedTime + GetAttribute()->m_dwCollisionDelay;

	return FRAMEGETTIME() + 1;
}

void ioUroborusWeapon3::SetOwnerChargeScaleRate( float fRate )
{
	m_fOwnerChargeScaleRate = fRate;

	if( m_fOwnerChargeScaleRate > FLOAT1 )
	{
		D3DXVECTOR3 vScale( m_fOwnerChargeScaleRate, m_fOwnerChargeScaleRate, m_fOwnerChargeScaleRate );
		GetParentSceneNode()->SetScale( vScale );
	}
}

bool ioUroborusWeapon3::CheckOwnerState()
{
	ioBaseChar *pOwner = GetOwner();
	if( !pOwner )
		return false;

	if( m_bNoAllCheck )
		return true;

	if ( m_bNoItemCheck )
	{
		if ( pOwner->GetState() == CS_USING_SKILL )
		{
			return true;
		}		
	}

	if( !m_bUseStartReturnPos )
	{
		ioWeaponItem *pWeapon = pOwner->GetWeapon();
		ioLuciferItem *pLuciferItem = ToLuciferItem( pWeapon );

		if( pWeapon )
		{
			if( pOwner->GetState() == CS_USING_SKILL || pOwner->GetState() == CS_LUCIFER_FLY )
				return true;
		}

		return false;
	}

	return true;
}

bool ioUroborusWeapon3::IsUroborusMove()
{
	if( m_UroborusState == US_MOVE )
		return true;

	return false;
}


bool ioUroborusWeapon3::IsUroborusTargetMove()
{
	if( m_UroborusState == US_TARGET_MOVE )
		return true;

	return false;
}


void ioUroborusWeapon3::ProcessGuidArrow()
{
	if( !m_pGuideArrow )
		return;

	m_pGuideArrow->Process( GetOwner(), GetPosition() );
}


void ioUroborusWeapon3::SetTraceTargetName( const ioHashString &szName )
{
	m_bTraceTarget = true;
	m_TraceTargetName = szName;
}


void ioUroborusWeapon3::CheckTargetMoveState( ioBaseChar *pChar, bool bApplyCol )
{
	if( !pChar ) return;

	//스킬에서 발생했을때
	if( m_bSkillWeapon )
	{
		if( bApplyCol )//&& m_vTargetMovePos != ioMath::VEC3_ZERO )
		{
			bool bSend = false;
			if( pChar->IsNeedSendNetwork() )
				bSend = true;

			m_TraceTargetName = pChar->GetCharName();
			SetTargetMoveState( bSend );
		}
		else
		{
			SetEndState( false, false, true );
		}

		return;
	}

	ioBaseChar *pOwner = GetOwner();

	if( !m_bUseStartReturnPos )//루시퍼
	{
		if( !pOwner || pOwner->GetState() != CS_LUCIFER_FLY )
		{
			SetEndState( false, false, true );
			return;
		}
	}
	else
	if( !pOwner )
	{
		SetEndState( false, false, true );
		return;
	}

	ioWeaponItem *pWeaponItem = pOwner->GetWeapon();
	ioLuciferItem *pLuciferItem = ToLuciferItem( pWeaponItem );

	if( !m_bNoItemCheck )
	{
		
		if( !pWeaponItem )
		{
			SetEndState( false, false, true );
			return;
		}

		if( !m_bUseStartReturnPos && !pLuciferItem )
		{
			SetEndState( false, false, true );
			return;
		}
	}


	if( bApplyCol )//&& m_vTargetMovePos != ioMath::VEC3_ZERO )
	{
		bool bSend = false;
		if( pChar->IsNeedSendNetwork() )
			bSend = true;

		m_TraceTargetName = pChar->GetCharName();

		if( pLuciferItem )
			pLuciferItem->SetTarget( m_TraceTargetName );

		SetTargetMoveState( bSend );
	}
	else
	{
		SetEndState( false, false, true );
	}
}

void ioUroborusWeapon3::CheckPreCollision( ioPlayEntity *pTarget, ioPlayStage *pStage )
{
	if( pTarget->IsWoundedCollision( GetWorldAttackBox() ) )
	{
		if( ToSymbolStruct(pTarget) || ToPushStruct(pTarget) || ToBallStruct(pTarget) ||
			ToDummyChar(pTarget) || ToWallZone(pTarget) )
		{
			SetEndState( true, false, true );
			return;
		}
	}
}

void ioUroborusWeapon3::SetWeaponDead()
{
	if( m_UroborusState != US_END )
	{
		SetEndState( false, false, false );
	}

	ioWeapon::SetWeaponDead();
}


/*void ioUroborusWeapon3::SetWaitState( ioBaseChar *pChar )
{
	m_UroborusState = US_WAIT;
}*/