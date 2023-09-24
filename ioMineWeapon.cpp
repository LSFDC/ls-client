

#include "stdafx.h"

#include "ioBaseChar.h"
#include "ioPlayStage.h"

#include "ioMineWeapon.h"

ActionStopType ioMineWeapon::m_ActionStopType = AST_NONE;
ActionStopType ioMineWeapon::m_DieActionStopType = AST_NONE;

ioMineWeapon::ioMineWeapon( ioBaseChar *pOwner, const WeaponAttribute *pAttr )
: ioWeapon( pOwner, pAttr )
{
	SetPeculiarity( WP_MINE_EXPLOSION | WP_ATTACK_OWNER | WP_WOUND_NO_SOUND | WP_DEF_NO_SOUND );

	m_pMine = NULL;

	m_dwSensingStartTime = 0;
	m_dwCurTraceDelayTime = 0;			// ���ð� ���

	m_State = MS_CHARGE;
	m_bMineVisible = false;

	m_iConveyerMoveFrameIdx = -1;
	m_bResistance = false;

	m_bWait = false;
	m_CurTeamColor = TEAM_BLUE;
}

ioMineWeapon::~ioMineWeapon()
{
	if( m_pMine )
	{
		LOG.PrintTimeAndLog( 0, "ioMineWeapon::~ioMineWeapon - UnReleased Resource" );
	}
}

void ioMineWeapon::LoadProperty( ioINILoader &rkLoader )
{
	ioWeapon::LoadProperty( rkLoader );

	char szBuf[MAX_PATH] = "";
	char szKey[MAX_PATH] = "";

	rkLoader.LoadString_e( "blue_mine_model", "", szBuf, MAX_PATH );
	m_BlueMineModel = szBuf;
	rkLoader.LoadString_e( "red_mine_model", "", szBuf, MAX_PATH );
	m_RedMineModel = szBuf;
	rkLoader.LoadString_e( "boom_effect", "", szBuf, MAX_PATH );
	m_BoomEffect = szBuf;
	rkLoader.LoadString_e( "normal_dead_effect", "", szBuf, MAX_PATH );
	m_NormalDeadEffect = szBuf;

	m_bTeamModel = rkLoader.LoadBool_e( "use_team_mine_model", false );

	m_vTeamModelList.clear();
	m_vEnemyModelList.clear();

	int iTeamCnt = rkLoader.LoadInt_e( "team_mine_model_cnt", 0 );
	int iEnemyCnt = rkLoader.LoadInt_e( "enemy_mine_model_cnt", 0 );

	if( iTeamCnt > 0 )
		m_vTeamModelList.reserve( iTeamCnt );

	if( iEnemyCnt > 0 )
		m_vEnemyModelList.reserve( iEnemyCnt );

	if( m_bTeamModel )
	{
		int i = 0;
		for( i=0; i < iTeamCnt; ++i )
		{
			wsprintf_e( szKey, "team_mine_model%d", i+1 );
			rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );

			m_vTeamModelList.push_back( ioHashString( szBuf ) );
		}
		
		for( i=0; i < iEnemyCnt; ++i )
		{
			wsprintf_e( szKey, "enemy_mine_model%d", i+1 );
			rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );

			m_vEnemyModelList.push_back( ioHashString( szBuf ) );
		}
	}

	m_fExplosionRange = rkLoader.LoadFloat_e( "explosion_range", 300.0f );
	m_fMineBoomDist = rkLoader.LoadFloat_e( "mine_boom_dist", 300.0f );
	m_dwTraceDelayTime = (DWORD)rkLoader.LoadInt_e("mine_trace_delay", 300 );
	m_fMineSizeRate = rkLoader.LoadFloat_e( "mine_size_rate", FLOAT1 );
	m_fVisibleRange = rkLoader.LoadFloat_e( "mine_visible_range", 0.0f );
	m_fMineSensingRange = rkLoader.LoadFloat_e( "mine_sensing_range", 0.0f );


	// Extend
	m_MineType = (MineType)rkLoader.LoadInt_e( "mine_type", MIT_MINE );
	m_bNoMineExplosion = rkLoader.LoadBool_e( "no_explosion_mine", false );
	m_bNoSencingMine = rkLoader.LoadBool_e( "no_sensing_mine", false );
	m_bNoMineUp = rkLoader.LoadBool_e( "no_mine_up", false );
	m_bUseTeamTargetBuff = rkLoader.LoadBool_e( "use_team_target_buff", false );
	m_bNormalDead = rkLoader.LoadBool_e( "use_normal_dead", false );

	//Buff List
	// Air
	int iAirCount = rkLoader.LoadInt_e( "red_target_air_buff_cnt", 0 );
	m_RedTargetAirBuffList.reserve( iAirCount );

	for( int i=0; i<iAirCount; i++ )
	{
		wsprintf_e( szKey, "red_target_air_buff%d", i+1 );
		rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );

		ioBuffInfo tBuffInfo;	
		tBuffInfo.m_BuffName = szBuf;

		if( !tBuffInfo.m_BuffName.IsEmpty() )
		{
			tBuffInfo.m_iBuffType = g_BuffMaker.GetBuffType( tBuffInfo.m_BuffName );
			if( tBuffInfo.m_iBuffType != BT_NONE )
			{
				m_RedTargetAirBuffList.push_back( tBuffInfo );
			}
		}
	}

	// Normal
	int iCount = rkLoader.LoadInt_e( "red_target_buff_cnt", 0 );
	m_RedTargetBuffList.reserve( iCount );

	for( int i=0; i<iCount; i++ )
	{
		wsprintf_e( szKey, "red_target_buff%d", i+1 );
		rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );

		ioBuffInfo tBuffInfo;	
		tBuffInfo.m_BuffName = szBuf;

		if( !tBuffInfo.m_BuffName.IsEmpty() )
		{
			tBuffInfo.m_iBuffType = g_BuffMaker.GetBuffType( tBuffInfo.m_BuffName );
			if( tBuffInfo.m_iBuffType != BT_NONE )
			{
				m_RedTargetBuffList.push_back( tBuffInfo );
			}
		}
	}

	// Down
	int iDownCount = rkLoader.LoadInt_e( "red_target_down_buff_cnt", 0 );
	m_RedTargetDownBuffList.reserve( iDownCount );

	for( int i=0; i<iDownCount; i++ )
	{
		wsprintf_e( szKey, "red_target_down_buff%d", i+1 );
		rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );

		ioBuffInfo tBuffInfo;	
		tBuffInfo.m_BuffName = szBuf;

		if( !tBuffInfo.m_BuffName.IsEmpty() )
		{
			tBuffInfo.m_iBuffType = g_BuffMaker.GetBuffType( tBuffInfo.m_BuffName );
			if( tBuffInfo.m_iBuffType != BT_NONE )
			{
				m_RedTargetDownBuffList.push_back( tBuffInfo );
			}
		}
	}
}

void ioMineWeapon::SetTeam( ioPlayStage *pStage, TeamType eType )
{
	if( !pStage ) return;

	ioWeapon::SetTeam( pStage, eType );

	DestroyResource();
	ioHashString szCurModel;

	if( m_bTeamModel )
	{
		ioBaseChar *pBaseOwner = pStage->GetOwnerChar();

		if( !pBaseOwner ) return;

		if( pBaseOwner->GetTeam() == eType )
		{
			if( !m_vTeamModelList.empty() )
			{
				int iRand = rand() % m_vTeamModelList.size();
				szCurModel = m_vTeamModelList[iRand];
			}
			else
			{
				szCurModel = m_BlueMineModel;
				m_CurTeamColor = TEAM_BLUE;
			}
		}
		else
		{
			if( !m_vEnemyModelList.empty() )
			{
				int iRand = rand() % m_vEnemyModelList.size();
				szCurModel = m_vEnemyModelList[iRand];
			}
			else
			{
				szCurModel = m_RedMineModel;
				m_CurTeamColor = TEAM_RED;
			}
		}
	}
	else
	{
		if( eType == TEAM_BLUE )
		{
			szCurModel = m_BlueMineModel;
			m_CurTeamColor = TEAM_BLUE;
		}
		else if( eType == TEAM_RED )
		{
			szCurModel = m_RedMineModel;
			m_CurTeamColor = TEAM_RED;
		}
		else //������
		{
			ioBaseChar *pOwner = GetOwner();
			if( pOwner && pOwner->GetSingleConvertTeam() == TEAM_BLUE )
			{
				szCurModel = m_BlueMineModel;
				m_CurTeamColor = TEAM_BLUE;
			}
			else
			{
				szCurModel = m_RedMineModel;
				m_CurTeamColor = TEAM_RED;
			}
		}
	}

	ioSceneNode *pSNode = GetParentSceneNode();
	m_pMine = pSNode->GetSceneMgr()->CreateEntityGrp();
	pSNode->AttachObject( m_pMine );
	m_pMine->LoadGroup( szCurModel.c_str() );

	pSNode->SetScale( D3DXVECTOR3(m_fMineSizeRate, m_fMineSizeRate, m_fMineSizeRate) );
	UpdateWorldCollisionBox();
}

void ioMineWeapon::SetStartPosition( const D3DXVECTOR3 &vPos, ioPlayStage *pStage )
{
	if( !pStage )
	{
		SetPosition( vPos );
		return;
	}

	D3DXVECTOR3 vMinePos;
	vMinePos.x = vPos.x;
	vMinePos.y = pStage->GetMapHeight( vPos.x, vPos.z, NULL, true, vPos.y );
	vMinePos.z = vPos.z;

	SetPosition( vMinePos );
}

void ioMineWeapon::Process( float fTimePerSec, ioPlayStage *pStage )
{
	switch( m_State )
	{
	case MS_CHARGE:
		OnCharge( pStage );
		break;
	case MS_SENSING:
		OnSensing( pStage, fTimePerSec );
		break;
	case MS_WAIT_EXPLOSION:
		OnWaitExplosion( pStage );
		break;
	case MS_EXPLOSION:
		OnExplosion();
		break;
	case MS_NORMAL_DEAD:
		OnNormalDead();
		break;
	}

	UpdateWorldCollisionBox();
}

void ioMineWeapon::OnCharge( ioPlayStage *pStage )
{
	if( m_dwSensingStartTime > FRAMEGETTIME() )
		return;

	// ����, ���� ��ȣ�� ������ ���������� üũ��, �����Ǿ����� �ٷ� ����..~
	if( g_WeaponMgr.CheckMissingIngMineIndex( m_dwWeaponIdx ) )
	{
		SetWaitExplosionMine( pStage, false );
	}
	else
	{
		m_State = MS_SENSING;
	}
}

void ioMineWeapon::OnSensing( ioPlayStage *pStage, float fTimePerSec )
{
	VisibleCheck( pStage );
	
	D3DXVECTOR3 vDiff;
	D3DXVECTOR3 vCurPos = GetPosition();
	float fSensingRangeSq = m_fMineBoomDist * m_fMineBoomDist;

	// Char Check
	const BaseCharList &rkCharList = pStage->GetBaseCharList();
	BaseCharList::const_iterator iter;
	if( IsExperienceOwner() == false )
	{
		for( iter=rkCharList.begin() ; iter!=rkCharList.end() ; ++iter )
		{
			ioBaseChar *pChar = *iter;
			if( !pChar->IsNeedProcess() )
				continue;

			if( pChar->GetTeam() == GetTeam() )
				continue;

			if( pChar->GetExperienceMode() == EMS_EXPERIENCE )
				continue;

			if( pChar->IsEntityCollisionSkipState( NULL ) || pChar->IsFloating() )
				continue;

			if( pChar->IsChatModeState( false ) )
				continue;

			if( pChar->HasBuff(BT_WEAK_BOMB) )
				continue;

			if( pChar->HasBuff(BT_CHANGE_TEAM) )
				continue;

			if( Help::CheckMineNoCollision( pChar->GetState() ) )
				continue;

			vDiff = vCurPos - pChar->GetWorldPosition();
			if( D3DXVec3LengthSq( &vDiff ) < fSensingRangeSq )
			{
				SetWaitExplosionMine( pStage, true );
				return;
			}
		}
	}

	// DropZone Check
	if( DropZoneCheck( pStage ) )
	{
		SetWaitExplosionMine( pStage, true );
	}
	
	// Mine Check
	if( pStage->CheckSensingMine( this, m_fMineSensingRange ) )
	{
		SetWaitExplosionMine( pStage, true );
	}
	else
	{
		bool bCheckMoving = true;
		if( P2PNetwork::IsNetworkPlaying() && pStage->IsNetworkMode() )
		{
			bCheckMoving = IsGameOwnerWeapon();
		}

		if( bCheckMoving && m_iConveyerMoveFrameIdx > 0 )
		{
			if( m_iConveyerMoveFrameIdx + 1 < (int)g_FrameTimer.GetFrameCounter() )
			{
				SetWaitExplosionMine( pStage, true );
			}
		}
	}
}

void ioMineWeapon::VisibleCheck( ioPlayStage *pStage )
{
	if( !m_pMine )	return;

	ioBaseChar *pTarget = pStage->GetCameraTargetChar();
	if( pTarget )
	{
		if( GetTeam() == pTarget->GetTeam() )
		{
			m_pMine->SetVisible( true );
		}
		else
		{
			float fVisibleRangeSq = m_fVisibleRange * m_fVisibleRange;

			D3DXVECTOR3 vDiff = GetPosition() - pTarget->GetWorldPosition();
			vDiff.y = 0.0f;

			if( D3DXVec3LengthSq( &vDiff ) < fVisibleRangeSq )
				m_bMineVisible = true;
			else
				m_bMineVisible = false;

			m_pMine->SetVisible( m_bMineVisible );
		}
	}
	else
	{
		m_pMine->SetVisible( false );
	}
}

bool ioMineWeapon::DropZoneCheck( ioPlayStage *pStage )
{	
	D3DXVECTOR3 vCurPos = GetPosition();
	float f_MapHeight = pStage->GetMapHeightNoneEntity( vCurPos.x, vCurPos.z );
	
	if ( f_MapHeight > 0.0f )
	{
		return false;
	}

	f_MapHeight = pStage->GetMapHeightOnlyTerrain( vCurPos.x, vCurPos.z );
	if( fabs(f_MapHeight - vCurPos.y) >= FLOAT10 )
		return true;

	return false;
}

void ioMineWeapon::UpdateWorldCollisionBox()
{
	if( m_State != MS_SENSING )
	{
		m_WorldCollisionBox.SetNull();
		return;
	}

	if( m_pMine )
	{
		ioSceneNode *pSNode = GetParentSceneNode();
		if( pSNode )
		{
			m_WorldCollisionBox = m_pMine->GetCollisionBox();

			m_WorldCollisionBox.Transform( pSNode->GetDerivedPosition(),
										   pSNode->GetDerivedScale(),
										   pSNode->GetDerivedOrientation() );
		}
	}
}

void ioMineWeapon::OnWaitExplosion( ioPlayStage *pStage )
{		
	if( m_dwCurTraceDelayTime + m_dwTraceDelayTime < FRAMEGETTIME() )
	{
		m_dwCurTraceDelayTime = 0;
		ExplosionMine( pStage );
	}
}

void ioMineWeapon::SetWaitExplosionMine( ioPlayStage *pStage, bool bSendNetwork )
{
	if( m_State == MS_WAIT_EXPLOSION ||
		m_State == MS_EXPLOSION ||
		m_State == MS_NORMAL_DEAD )
		return;

	// Ÿ�ٽ� ���� ���̸� �÷���
	if( !m_bNoMineUp )
	{
		D3DXVECTOR3 vTracePos = GetPosition();
		vTracePos.y += GetWorldCollisionBox().GetExtents(1);
		SetPosition( vTracePos );
	}

	m_dwCurTraceDelayTime = FRAMEGETTIME();	// ��� �ð� ����
	m_State = MS_WAIT_EXPLOSION;

	if( bSendNetwork )
	{
		if( P2PNetwork::IsNetworkPlaying() && pStage->IsNetworkMode() )
		{
			SP2Packet kPacket( CUPK_MINE_WAIT_EXPLOSION );
			kPacket << GetOwnerName();
			kPacket << m_dwWeaponIdx;
			kPacket << GetPosition();
			P2PNetwork::SendToAllPlayingUser( kPacket );
		}
	}
}

void ioMineWeapon::SetNormalDeadMine( ioPlayStage *pStage, bool bSendNetwork )
{
	if( m_State == MS_WAIT_EXPLOSION ||
		m_State == MS_EXPLOSION ||
		m_State == MS_NORMAL_DEAD )
		return;

	UpdateWorldAttackBox( 0.0f, true );
	if( !m_NormalDeadEffect.IsEmpty() )
	{
		pStage->CreateMapEffect( m_NormalDeadEffect,
			GetPosition(),
			GetScale() );

		ioWeapon::PlayExplosionSound();
	}

	m_State = MS_NORMAL_DEAD;

	if( bSendNetwork )
	{
		if( P2PNetwork::IsNetworkPlaying() && pStage->IsNetworkMode() )
		{
			SP2Packet kPacket( CUPK_NORMAL_DEAD_MINE );
			kPacket << GetOwnerName();
			kPacket << m_dwWeaponIdx;
			kPacket << GetPosition();
			P2PNetwork::SendToAllPlayingUser( kPacket );
		}
	}
}

void ioMineWeapon::ExplosionMine( ioPlayStage *pStage, bool bWait )
{
	if( !IsCanExplosion() )	return;

	m_bWait = bWait;

	float fHalfRange = m_fExplosionRange;
	m_LocalAttackBox.SetExtents( 0, fHalfRange );
	m_LocalAttackBox.SetExtents( 1, fHalfRange );
	m_LocalAttackBox.SetExtents( 2, fHalfRange );
	UpdateWorldAttackBox( 0.0f, false );

	if( !m_BoomEffect.IsEmpty() )
	{
		pStage->CreateMapEffect( m_BoomEffect,
								 GetPosition(),
								 GetScale() );

		ioWeapon::PlayExplosionSound();
	}

	m_State = MS_EXPLOSION;
}

void ioMineWeapon::NormalDeadMine( ioPlayStage *pStage, bool bWait )
{
	if( !IsCanExplosion() )	return;

	m_bWait = bWait;

	UpdateWorldAttackBox( 0.0f, true );

	if( !m_NormalDeadEffect.IsEmpty() )
	{
		pStage->CreateMapEffect( m_NormalDeadEffect,
			GetPosition(),
			GetScale() );

		ioWeapon::PlayExplosionSound();
	}

	m_State = MS_NORMAL_DEAD;
}

void ioMineWeapon::TranslateByConveyer( const D3DXVECTOR3 &vMoveAmt )
{
	if( m_State != MS_WAIT_EXPLOSION && m_State != MS_EXPLOSION )
	{
		SetPosition( GetPosition() + vMoveAmt );
		m_iConveyerMoveFrameIdx = g_FrameTimer.GetFrameCounter();
	}
}

bool ioMineWeapon::IsCanMineExplosion()
{
	if( !IsLive() )	return false;
	if( m_bNoMineExplosion ) return false;
	if( !HasPeculiarity( WP_MINE_EXPLOSION ) )
		return false;

	ioBaseChar *pOwner = GetOwner();
	if( !pOwner || !pOwner->IsNeedProcess() )
		return false;

	if( m_State != MS_EXPLOSION )
		return false;

	return true;
}

bool ioMineWeapon::IsCanExplosion() const
{
	if( !IsLive() || m_State == MS_EXPLOSION )
		return false;

	if( m_State == MS_NORMAL_DEAD )
		return false;

	return true;
}

void ioMineWeapon::OnExplosion()
{
	if( m_bWait )
	{
		m_bWait = false;
		return;
	}

	SetWeaponDead();
}

void ioMineWeapon::OnNormalDead()
{
	if( m_bWait )
	{
		m_bWait = false;
		return;
	}

	SetWeaponDead();
}

void ioMineWeapon::DestroyResource( ioPlayStage *pStage )
{
	if( m_pMine )
	{
		GetParentSceneNode()->GetSceneMgr()->DestroyEntityGrp( m_pMine );
		m_pMine = NULL;
	}
}

const ioOrientBox& ioMineWeapon::GetLocalAttackBox() const
{
	return m_LocalAttackBox;
}

ioWeapon::WeaponType ioMineWeapon::GetType() const
{
	return WT_MINE;
}

ActionStopType ioMineWeapon::GetActionStopType() const
{
	return m_ActionStopType;
}

ActionStopType ioMineWeapon::GetDieActionStopType() const
{
	return m_DieActionStopType;
}

bool ioMineWeapon::IsCanVisible( ioWeapon *pWeapon ) const
{
	ioBaseChar *pChar = pWeapon->GetOwner();
	if( !pChar ) return false;

	D3DXVECTOR3 vDiff;
	vDiff = GetPosition() - pChar->GetWorldPosition();
	vDiff.y = 0.0f;

	float fVisibleRangeSq = m_fVisibleRange * m_fVisibleRange;

	if( pChar->IsEntityCollisionSkipState( NULL ) || pChar->IsFloating() )
		return false;

	if( pChar->GetTeam() == GetTeam() )
		return true;

	if( D3DXVec3LengthSq( &vDiff ) < fVisibleRangeSq )
		return true;

	return false;
}

bool ioMineWeapon::IsCollision( ioPlayEntity *pEntity, bool bMapCollision )
{
	if( m_State != MS_EXPLOSION )
		return false;

	float fTargetDist = -FLOAT1;
	D3DXVECTOR3 vCenter = GetWorldAttackBox().GetCenter();
	if( !pEntity->CheckSphereDistance( vCenter, m_fExplosionRange, fTargetDist ) )
		return false;

	if( COMPARE( fTargetDist, 0.0f, m_fExplosionRange ) )
		return true;

	return false;
}

DWORD ioMineWeapon::GetSensingStartGapTime()
{
	if( m_dwSensingStartTime != 0 )
		return FRAMEGETTIME() - m_dwSensingStartTime;

	return 0;
}

DWORD ioMineWeapon::GetCollisionEnableTime() const
{
	if( m_State == MS_EXPLOSION )
		return m_dwCreatedTime;

	return m_dwCreatedTime + GetAttribute()->m_dwCollisionDelay;
}

const BuffInfoList& ioMineWeapon::GetTargetAirBuffList() const
{
	if( !m_bUseTeamTargetBuff )
		return GetAttribute()->m_TargetAirBuffList;

	if( m_CurTeamColor == TEAM_BLUE )
		return GetAttribute()->m_TargetAirBuffList;

	return m_RedTargetAirBuffList;
}

const BuffInfoList& ioMineWeapon::GetTargetBuffList() const
{
	if( !m_bUseTeamTargetBuff )
		return GetAttribute()->m_TargetBuffList;

	if( m_CurTeamColor == TEAM_BLUE )
		return GetAttribute()->m_TargetBuffList;

	return m_RedTargetBuffList;
}

const BuffInfoList& ioMineWeapon::GetTargetDownBuffList() const
{
	if( !m_bUseTeamTargetBuff )
		return GetAttribute()->m_TargetDownBuffList;

	if( m_CurTeamColor == TEAM_BLUE )
		return GetAttribute()->m_TargetDownBuffList;

	return m_RedTargetDownBuffList;
}

