#include "StdAfx.h"
#include "ioKeyMarkAttackBuff.h"

#include "ioBaseChar.h"
#include "ioPlayStage.h"


ioKeyMarkAttackBuff::ioKeyMarkAttackBuff(void)
{
}

ioKeyMarkAttackBuff::ioKeyMarkAttackBuff( const ioKeyMarkAttackBuff &rhs ):
ioBuff( rhs ),
m_TargetWoundType( rhs.m_TargetWoundType ),
m_fTargetMinAngle( rhs.m_fTargetMinAngle ),
m_fTargetMaxAngle( rhs.m_fTargetMaxAngle ),
m_fTargetMinRange( rhs.m_fTargetMinRange ),
m_fTargetMaxRange( rhs.m_fTargetMaxRange ),
m_szA_MakrMapEffect( rhs.m_szA_MakrMapEffect ),
m_szS_MakrMapEffect( rhs.m_szS_MakrMapEffect ),
m_szD_MakrMapEffect( rhs.m_szD_MakrMapEffect ),
m_szMarkEffect( rhs.m_szMarkEffect ),
m_dwAttackDelayTime( rhs.m_dwAttackDelayTime ),
m_bFindTargetOnlyFirstLoop( rhs.m_bFindTargetOnlyFirstLoop ),
m_vTargetTeleportOffset( rhs.m_vTargetTeleportOffset ),
m_NormalTargetAttackAttribute( rhs.m_NormalTargetAttackAttribute ),
m_AirTargetAttackAttribute( rhs.m_AirTargetAttackAttribute ),
m_DownTargetAttackAttribute( rhs.m_DownTargetAttackAttribute ),
m_fEndJumpPower( rhs.m_fEndJumpPower ),
m_bDeleteBuffByDownState( rhs.m_bDeleteBuffByDownState ),
m_bDeleteBuffByDropState( rhs.m_bDeleteBuffByDropState ),
m_bDisableOwnerDownAttack( rhs.m_bDisableOwnerDownAttack ),
m_bDeleteBuffByWoundedState( rhs.m_bDeleteBuffByWoundedState ),
m_iAttakMaxCount( rhs.m_iAttakMaxCount ),
m_bReduceAllGauge( rhs.m_bReduceAllGauge )
{

}

ioKeyMarkAttackBuff::~ioKeyMarkAttackBuff(void)
{
}

ioBuff* ioKeyMarkAttackBuff::Clone()
{
	return new ioKeyMarkAttackBuff( *this );
}

void ioKeyMarkAttackBuff::LoadProperty( ioINILoader &rkLoader )
{
	ioBuff::LoadProperty( rkLoader );
	char szBuf[MAX_PATH];
	//Target
	m_TargetWoundType	= (TargetWoundType)rkLoader.LoadInt_e( "target_wound_type", TWT_ALL );
	m_fTargetMinAngle		= rkLoader.LoadFloat_e( "target_min_angle", 0.0f );
	m_fTargetMaxAngle		= rkLoader.LoadFloat_e( "target_max_angle", 0.0f );
	m_fTargetMinRange		= rkLoader.LoadFloat_e( "target_min_range", 0.0f );
	m_fTargetMaxRange		= rkLoader.LoadFloat_e( "target_max_range", 0.0f );

	rkLoader.LoadString_e( "a_mark_effect_name", "", szBuf, MAX_PATH );
	m_szA_MakrMapEffect = szBuf;
	rkLoader.LoadString_e( "s_mark_effect_name", "", szBuf, MAX_PATH );
	m_szS_MakrMapEffect = szBuf;
	rkLoader.LoadString_e( "d_mark_effect_name", "", szBuf, MAX_PATH );
	m_szD_MakrMapEffect = szBuf;
	rkLoader.LoadString_e( "else_mark_effect_name", "", szBuf, MAX_PATH );
	m_szMarkEffect = szBuf;

	m_dwAttackDelayTime = rkLoader.LoadInt_e( "attack_delay_time", 0 );

	m_bFindTargetOnlyFirstLoop = rkLoader.LoadBool_e( "find_target_only_first_loop", false );

	m_vTargetTeleportOffset.x = rkLoader.LoadFloat_e( "target_teleport_offset_x", 0.0f );
	m_vTargetTeleportOffset.y = rkLoader.LoadFloat_e( "target_teleport_offset_y", 0.0f );
	m_vTargetTeleportOffset.z = rkLoader.LoadFloat_e( "target_teleport_offset_z", 0.0f );

	LoadAttackAttribute_e( "normal_target_attack", m_NormalTargetAttackAttribute, rkLoader );
	LoadAttackAttribute_e( "air_target_attack", m_AirTargetAttackAttribute, rkLoader );
	LoadAttackAttribute_e( "down_target_attack", m_DownTargetAttackAttribute, rkLoader );

	m_fEndJumpPower = rkLoader.LoadFloat_e( "end_jump_power", 0.0f );

	m_bDeleteBuffByDownState = rkLoader.LoadBool_e( "delete_buff_by_down_state", false );
	m_bDeleteBuffByDropState = rkLoader.LoadBool_e( "delete_buff_by_drop_state", false );
	m_bDeleteBuffByWoundedState = rkLoader.LoadBool_e( "delete_buff_by_woundedstate", false );

	m_bDisableOwnerDownAttack = rkLoader.LoadBool( "disable_owner_down_attack", false );

	m_iAttakMaxCount = rkLoader.LoadInt_e( "attack_max_count", 0 );
	m_bReduceAllGauge = rkLoader.LoadBool_e( "reduce_all_gauge", false );
}

void ioKeyMarkAttackBuff::ClearData()
{
	//Target
	m_szA_MarkTargetName.Clear();
	m_szS_MarkTargetName.Clear();
	m_szD_MarkTargetName.Clear();
	m_dwA_MarkMapEffectID = -1;
	m_dwS_MarkMapEffectID = -1;
	m_dwD_MarkMapEffectID = -1;

	//Attack
	m_szAttackTarget.Clear();
	m_iAttackCount = 0;
	m_bInputKey = false;
	m_dwKeyInputTime = 0;

	m_BuffState = BS_LOOP;
	m_dwFireMotionEndTime = 0;
}

void ioKeyMarkAttackBuff::StartBuff( ioBaseChar *pOwner )
{
	ioBuff::StartBuff( pOwner );
	ClearData();
}

bool ioKeyMarkAttackBuff::ApplyStartBuff( ioBaseChar *pOwner, DWORD dwGlobalTime, DWORD dwGapTime )
{
	if ( !ioBuff::ApplyStartBuff( pOwner, dwGlobalTime, dwGapTime ) )
		return false;
	return true;
}

void ioKeyMarkAttackBuff::ProcessBuff( float fTimePerSec )
{
	if ( !CheckOwnerState() )
	{
		SetReserveEndBuff();
		return;
	}

	ioPlayStage *pStage = m_pOwner->GetCreator();
	if ( !pStage )
	{
		SetReserveEndBuff();
		return;
	}

	if ( !CheckOperation( m_pOwner, fTimePerSec ) )
		return;

	switch( m_BuffState )
	{
	case BS_LOOP:
		{
			if ( !m_bFindTargetOnlyFirstLoop || m_iAttackCount == 0 )
				FindTarget( m_pOwner, pStage );
			UpdateMark( m_pOwner, pStage );

			if ( m_pOwner->IsNeedProcess() && CheckInputKey( m_pOwner, pStage ) && CheckEnableAttackState( m_pOwner, pStage ) )
			{
				SetActionState( m_pOwner, pStage );
				break;
			}
		}
		break;
	case BS_ATTACK:
		{
			if ( m_dwMotionEndTime > 0 && m_dwMotionEndTime < FRAMEGETTIME() )
			{
				if ( m_iAttakMaxCount <= m_iAttackCount )
					SetReserveEndBuff();
				SetActionEndState( m_pOwner );
				break;
			}
			UpdateMark( m_pOwner, pStage );
		}
		break;
	}
}

bool ioKeyMarkAttackBuff::CheckOwnerState()
{
	if( !GetOwner() || !m_pOwner )
		return false;

	if( m_bDeleteBuffByWoundedState )
	{
		if( m_pOwner->IsWoundedState() )
			return false;
	}

	if( m_bDeleteBuffByDownState )
	{
		CharState eState = m_pOwner->GetState();
		if( eState == CS_FROZEN && m_pOwner->GetBlowFrozenState() )
			return false;
		else if( eState == CS_BLOW_WOUNDED && !m_pOwner->IsFloatingState() )
			return false;
		else if( eState == CS_BOUND_BLOW_WOUNDED && !m_pOwner->IsFloatingState() )
			return false;
		else if( eState == CS_ETC_STATE && m_pOwner->IsApplyDownState(false) )
			return false;
		else if( eState == CS_STOP_MOTION && m_pOwner->GetBlowStopMotionState() )
			return false;
		else if( eState == CS_ICE_STATE && m_pOwner->GetBlowStopMotionState() )
			return false;
	}

	if ( m_bDeleteBuffByDropState )
	{
		CharState eState = m_pOwner->GetState();
		if( eState == CS_DROP_ZONE_DOWN )
			return false;
	}

	return true;
}


bool ioKeyMarkAttackBuff::CheckOperation( ioBaseChar* pChar, float fTimePerSec )
{
	if ( !pChar )
		return false;
	switch( m_iOperationType )
	{
	case OT_SWITCH:
		{
			DWORD dwCurTime = FRAMEGETTIME();
			float fCurTicGauge = GetCurTicGauge();
			int iTickGap = GetCurTicGap();
			int iCurTicTime = GetCurTicTime();
			if( iTickGap >= iCurTicTime )
			{
				m_dwCheckTime = dwCurTime - ( iTickGap - iCurTicTime );

				ioObjectItem *pObject = pChar->GetObject();
				if( pObject && !pObject->EnableReduceGaugeOnSwitchBuff() && pObject->GetItemCode() == m_dwCreateWeaponItemCode )
					break;

				if( fCurTicGauge != 0.0f )
				{
					if( !IsRightOwnerItem() )
						SetReserveEndBuff();

					if( pChar->ReduceCurSkillGaugeBySkill( m_ItemName, fCurTicGauge ) <= 0.0f )
					{
						if( m_pOwner->GetState() != CS_BUFF_ACTION )
							SetReserveEndBuff();
					}
				}
			}
			CheckCallingProcess();
		}
		
		break;
	case OT_DELAY:
	case OT_ACTIVE:
		ioBuff::ProcessBuff( fTimePerSec );
		break;
	case OT_PASSIVE:
		CheckCallingAreaWeapon();
		break;
	}

	return IsLive();
}

void ioKeyMarkAttackBuff::FindTarget( ioBaseChar* pChar, ioPlayStage *pStage )
{
	if( !pChar || !pStage ) 
		return;
	if( !pChar->IsNeedProcess() )
		return;

	D3DXVECTOR3 vCurPos = pChar->GetMidPositionByRate();
	D3DXVECTOR3 vDir = pChar->GetTargetRot() * D3DXVECTOR3( 0.0f, 0.0f, -1.0f );

	float fMaxRangeSq = m_fTargetMaxRange * m_fTargetMaxRange;
	float fMinRangeSq = m_fTargetMinRange * m_fTargetMinRange;

	ioHashString szTarget;
	float fCurGap = -1.0f;
	//
	const BaseCharList &rkCharList = pStage->GetBaseCharList();

	BaseCharDistanceSqList TargetNameList;

	for ( auto iter = rkCharList.begin(); iter != rkCharList.end(); ++iter )
	{
		ioBaseChar* pTarget = *iter;

		if( !pTarget )	continue;

		if ( pTarget == pChar )
			continue;

		if ( pTarget->GetTeam() == pChar->GetTeam() )
			continue;

		if ( pTarget->IsSkipState() )
			continue;

		if( !IsEnableTargetState( pTarget, true ) )
			continue;

		D3DXVECTOR3 vDiff = pTarget->GetMidPositionByRate() - pChar->GetMidPositionByRate();
		float fLengthSq = D3DXVec3LengthSq( &vDiff );

		if( !COMPARE( fLengthSq, fMinRangeSq, fMaxRangeSq ) )
			continue;

		D3DXVec3Normalize( &vDiff, &vDiff );

		float fValue = D3DXVec3Dot( &vDir, &vDiff );
		float fAngleGap = RADtoDEG( acosf(fValue) );

		if ( !COMPARE( fAngleGap, m_fTargetMinAngle, m_fTargetMaxAngle ) )
			continue;

		float fDistanceSq = D3DXVec3LengthSq( &vDiff );

		BaseCharDistanceSq kTarget;
		kTarget.m_Name = pTarget->GetCharName();
		kTarget.m_fDistanceSq = fLengthSq;
		kTarget.m_vPos = pTarget->GetMidPositionByRate();

		TargetNameList.push_back( kTarget );
	}

	bool bChange = false;
	ioHashString szTarget_A,szTarget_S,szTarget_D;
	if ( !TargetNameList.empty() )
	{
		std::sort( TargetNameList.begin(), TargetNameList.end(), BaseCharDistanceSqSort2() );

		for ( int i=0 ; i< (int)TargetNameList.size() ; ++i )
		{
			if( i == 0 )
				szTarget_D = TargetNameList[i].m_Name;
			if( i == 1 )
				szTarget_S = TargetNameList[i].m_Name;
			if( i == 2 )
			{
				szTarget_A = TargetNameList[i].m_Name;
				break;
			}
		}
	}

	if ( m_szA_MarkTargetName != szTarget_A )
	{
		m_szA_MarkTargetName = szTarget_A;
		bChange = true;

		if ( !m_szA_MarkTargetName.IsEmpty() )
			CreateMarker( pChar, pStage, m_szA_MarkTargetName, m_szA_MakrMapEffect, m_dwA_MarkMapEffectID );
		else
			DestroyMaker( pChar, pStage, m_dwA_MarkMapEffectID );
	}
	if ( m_szS_MarkTargetName != szTarget_S )
	{
		m_szS_MarkTargetName = szTarget_S;
		bChange = true;

		if ( !m_szS_MarkTargetName.IsEmpty() )
			CreateMarker( pChar, pStage, m_szS_MarkTargetName, m_szS_MakrMapEffect, m_dwS_MarkMapEffectID );
		else
			DestroyMaker( pChar, pStage, m_dwS_MarkMapEffectID );

	}
	if ( m_szD_MarkTargetName != szTarget_D )
	{
		m_szD_MarkTargetName = szTarget_D;
		bChange = true;

		if ( !m_szD_MarkTargetName.IsEmpty() )
			CreateMarker( pChar, pStage, m_szD_MarkTargetName, m_szD_MakrMapEffect, m_dwD_MarkMapEffectID );
		else
			DestroyMaker( pChar, pStage, m_dwD_MarkMapEffectID );
	}
	//��Ŷ
	if ( pChar->IsNeedSendNetwork() && bChange )
	{
		SP2Packet kPacket( CUPK_APPLY_EXTRA_BUFF_INFO );
		kPacket << pChar->GetCharName();
		kPacket << GetName();
		kPacket << static_cast<BYTE>(BST_CHANGE_TARGET);
		kPacket << m_szA_MarkTargetName;
		kPacket << m_szS_MarkTargetName;
		kPacket << m_szD_MarkTargetName;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

bool ioKeyMarkAttackBuff::IsEnableTargetState( ioBaseChar *pTarget, bool bCheckTargetWound )
{
	if( !pTarget )
		return false;

	CharState eState = pTarget->GetState();
	switch( eState )
	{
	case CS_DIE:
	case CS_DROP_ZONE_DOWN:
	case CS_READY:
	case CS_VIEW:
	case CS_OBSERVER:
	case CS_LOADING:
	case CS_FISHING:
	case CS_STARTMOTION:
	case CS_ENDMOTION:
	case CS_WARP_STATE:
	case CS_EXCAVATING:
	case CS_ROULETTE:
		return false;
	}

	if( pTarget->IsChatModeState( false ) )
		return false;

	if( pTarget->IsExcavating() )
		return false;

	if( pTarget->GetExperienceMode() == EMS_EXPERIENCE )
		return false;

	if( pTarget->HasHideBuff() )
		return false;

	if( bCheckTargetWound )
	{
		if( !pTarget->CheckTargetWoundType(m_TargetWoundType) )
			return false;
	}

	return true;
}

void ioKeyMarkAttackBuff::CreateMarker( ioBaseChar *pChar, ioPlayStage *pStage, const ioHashString& szTargetName, const ioHashString& szEffectName, DWORD& dwEffectID )
{
	if ( !pChar || !pStage )
		return;

	ioBaseChar* pTarget = pStage->GetBaseChar( szTargetName );
	if ( !pTarget )
		return;

	ioMapEffect *pMapEffect = pStage->FindMapEffect( dwEffectID );
	if( !pMapEffect )
	{
		D3DXVECTOR3 vScale( 1.0f, 1.0f, 1.0f );
		D3DXVECTOR3 vPos = pTarget->GetMidPositionByRate();
		D3DXVECTOR3 vDir = pTarget->GetTargetRot() * D3DXVECTOR3( 0.0f, 0.0f, -1.0f );
		pMapEffect = pStage->CreateMapEffect( szEffectName, vPos, vScale );
		if( pMapEffect )
		{
			dwEffectID = pMapEffect->GetUniqueID();

			D3DXQUATERNION qtCharRot;
			ioMath::CalcDirectionQuaternion( qtCharRot, -vDir );
			pMapEffect->SetWorldOrientation( qtCharRot );
		}
	}
}

void ioKeyMarkAttackBuff::DestroyMaker( ioBaseChar *pChar, ioPlayStage *pStage, DWORD& dwEffectID )
{
	if ( !pChar || !pStage )
		return;
	ioMapEffect *pMapEffect = pStage->FindMapEffect( dwEffectID );
	if ( pMapEffect )
	{
		pMapEffect->EndEffectForce();
		dwEffectID = -1;
	}
}

void ioKeyMarkAttackBuff::UpdateMark( ioBaseChar* pChar, ioPlayStage *pStage )
{
	if ( !pChar || !pStage )
		return;

	ioMapEffect *pMapEffect = pStage->FindMapEffect( m_dwA_MarkMapEffectID );
	if( pMapEffect )
	{
		ioBaseChar* pTarget = pStage->GetBaseChar( m_szA_MarkTargetName );
		if ( pTarget )
		{
			D3DXVECTOR3 vPos = pTarget->GetMidPositionByRate();
			pMapEffect->SetWorldPosition( vPos );
		}
		else
			DestroyMaker( pChar, pStage, m_dwA_MarkMapEffectID );
	}

	pMapEffect = pStage->FindMapEffect( m_dwS_MarkMapEffectID );
	if( pMapEffect )
	{
		ioBaseChar* pTarget = pStage->GetBaseChar( m_szS_MarkTargetName );
		if ( pTarget )
		{
			D3DXVECTOR3 vPos = pTarget->GetMidPositionByRate();
			pMapEffect->SetWorldPosition( vPos );
		}
		else
			DestroyMaker( pChar, pStage, m_dwS_MarkMapEffectID );
	}

	pMapEffect = pStage->FindMapEffect( m_dwD_MarkMapEffectID );
	if( pMapEffect )
	{
		ioBaseChar* pTarget = pStage->GetBaseChar( m_szD_MarkTargetName );
		if ( pTarget )
		{
			D3DXVECTOR3 vPos = pTarget->GetMidPositionByRate();
			pMapEffect->SetWorldPosition( vPos );
		}
		else
			DestroyMaker( pChar, pStage, m_dwD_MarkMapEffectID );
	}
}

bool ioKeyMarkAttackBuff::CheckInputKey( ioBaseChar* pChar, ioPlayStage *pStage )
{
	if ( !pChar || !pStage )
		return false;

	if ( !m_bInputKey )
	{
		if ( pChar->IsAttackKey() )
		{
			ioBaseChar* pTarget = pStage->GetBaseChar( m_szD_MarkTargetName );
			if ( pTarget )
			{
				m_szAttackTarget = m_szD_MarkTargetName;
				m_dwKeyInputTime = FRAMEGETTIME();
				m_bInputKey = true;
			}
		}

		if ( pChar->IsDefenseKey() )
		{
			ioBaseChar* pTarget = pStage->GetBaseChar( m_szS_MarkTargetName );
			if ( pTarget )
			{
				m_szAttackTarget = m_szS_MarkTargetName;
				m_dwKeyInputTime = FRAMEGETTIME();
				m_bInputKey = true;
			}
		}

		if ( pChar->IsJumpKey() )
		{
			ioBaseChar* pTarget = pStage->GetBaseChar( m_szA_MarkTargetName );
			if ( pTarget )
			{
				m_szAttackTarget = m_szA_MarkTargetName;
				m_dwKeyInputTime = FRAMEGETTIME();
				m_bInputKey = true;
			}
		}
	}
	else
	{
		if ( m_dwKeyInputTime + m_dwAttackDelayTime < FRAMEGETTIME() )
		{
			ioBaseChar* pTarget = pStage->GetBaseChar( m_szAttackTarget );
			if ( !pTarget )
			{
				m_bInputKey = false;
				return false;
			}
			return true;
		}
	}

	return false;
}

bool ioKeyMarkAttackBuff::CheckEnableAttackState( ioBaseChar* pChar, ioPlayStage *pStage )
{
	if ( !pChar || !pStage )
		return false;

	if ( m_iAttakMaxCount <= m_iAttackCount )
		return false;

	if ( !CheckEnableOwnerState( pChar, pStage ) )
		return false;

	if( !CheckEnableTargetState( pStage ) )
		return false;

	return true;
}

bool ioKeyMarkAttackBuff::CheckEnableOwnerState( ioBaseChar* pChar, ioPlayStage *pStage )
{
	if ( !pChar || !pStage )
		return false;

	CharState eState = pChar->GetState();

	if( eState == CS_DROP_ZONE_DOWN )
		return false;

	if ( m_bDisableOwnerDownAttack )
	{
		if( eState == CS_FROZEN && m_pOwner->GetBlowFrozenState() )
			return false;
		else if( eState == CS_BLOW_WOUNDED && !m_pOwner->IsFloatingState() )
			return false;
		else if( eState == CS_BOUND_BLOW_WOUNDED && !m_pOwner->IsFloatingState() )
			return false;
		else if( eState == CS_ETC_STATE && m_pOwner->IsApplyDownState(false) )
			return false;
		else if( eState == CS_STOP_MOTION && m_pOwner->GetBlowStopMotionState() )
			return false;
		else if( eState == CS_ICE_STATE && m_pOwner->GetBlowStopMotionState() )
			return false;
	}
	return true;
}

bool ioKeyMarkAttackBuff::CheckEnableTargetState( ioPlayStage *pStage )
{
	if ( !pStage )
		return false;

	if ( m_szAttackTarget.IsEmpty() )
		return false;

	ioBaseChar* pTarget = pStage->GetBaseChar( m_szAttackTarget );
	if ( !pTarget )
		return false;

	//Check TargetState
	CharState eState = pTarget->GetState();
	if( eState == CS_DIE )
		return false;
	if( eState == CS_VIEW )
		return false;
	if( eState == CS_OBSERVER )
		return false;
	if( eState == CS_READY )
		return false;
	if( eState == CS_LOADING )
		return false;
	if( eState == CS_FISHING )
		return false;
	if( eState == CS_CREEPING )
		return false;
	if( eState == CS_DROP_ZONE_DOWN )
		return false;
	if( eState == CS_EXCAVATING )
		return false;
	if( eState == CS_ROULETTE )
		return false;
	if ( eState == CS_STARTMOTION )
		return false;
	if ( eState == CS_ENDMOTION )
		return false;
	if ( eState == CS_WARP_STATE )
		return false;
	if ( eState == CS_LOCKUP_BUFF )
		return false;
	if ( pTarget->IsSkipState() )
		return false;
	if( pTarget->IsExcavating() )
		return false;
	if( pTarget->GetExperienceMode() == EMS_EXPERIENCE )
		return false;
	if( pTarget->IsChatModeState( false ) )
		return false;

	return true;
}


void ioKeyMarkAttackBuff::SetActionState( ioBaseChar* pChar, ioPlayStage *pStage )
{
	if ( !pChar || !pStage )
		return;

	ioBaseChar* pTarget = pStage->GetBaseChar( m_szAttackTarget );
	if ( !pTarget )
		return;

	pChar->SetState( CS_BUFF_ACTION );

	SetTeleport( pChar, pStage );
	m_BuffState = BS_ATTACK;

	const AttackAttribute& Attri = GetActionAttribute( pChar );

	SetActionAni( pChar, Attri );

	m_iAttackCount++;

	//��Ŷ
	if ( pChar->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_APPLY_EXTRA_BUFF_INFO );
		kPacket << pChar->GetCharName();
		kPacket << GetName();
		kPacket << static_cast<BYTE>(BST_ATTACK);
		kPacket << pChar->GetWorldPosition();
		kPacket << pChar->GetTargetRot();
		kPacket << m_szAttackTarget;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

void ioKeyMarkAttackBuff::SetTeleport( ioBaseChar *pChar, ioPlayStage *pStage )
{
	if ( !pChar || !pStage )
		return;

	ioBaseChar *pTarget = pChar->GetBaseChar( m_szAttackTarget );
	if( !pTarget )
		return;
	D3DXVECTOR3	vNewPos = pTarget->GetWorldPosition() + pTarget->GetWorldOrientation() * m_vTargetTeleportOffset;

	float fMapHeight = pStage->GetMapHeightOnlyTerrain( vNewPos.x, vNewPos.z );
	if ( vNewPos.y <= fMapHeight )
		vNewPos.y = fMapHeight;

	pChar->SetWorldPosition( vNewPos );

	SetTargetRotation( pChar );
}

void ioKeyMarkAttackBuff::SetTargetRotation( ioBaseChar *pChar )
{
	if ( !pChar )
		return;
	ioEntityGroup *pGrp = pChar->GetGroup();
	if ( !pGrp )
		return;

	ioBaseChar *pTarget = pChar->GetBaseChar( m_szAttackTarget );

	if( !pTarget )
		return;

	D3DXVECTOR3 vAimedDir = pTarget->GetMidPositionByRate() - pChar->GetMidPositionByRate();
	D3DXVec3Normalize( &vAimedDir, &vAimedDir );
	pChar->SetTargetRotToDir( vAimedDir, true );
}

void ioKeyMarkAttackBuff::SetActionAni( ioBaseChar* pChar, const AttackAttribute& Attri )
{
	if ( !pChar )
		return;

	ioEntityGroup *pGrp = pChar->GetGroup();
	int iAniID = pGrp->GetAnimationIdx( Attri.m_AttackAnimation );
	if( iAniID == -1 )	
		return;

	float fTimeRate  = 1.0f;
	if ( Attri.m_fAttackAniRate > 0.0f )
		fTimeRate = Attri.m_fAttackAniRate;
	DWORD dwPreDelay = Attri.m_dwPreDelay;

	DWORD dwWeaponBaseIndex = pChar->IncreaseWeaponIndexBase();
	pChar->RefreshFireTimeListWithBase( dwWeaponBaseIndex,
		iAniID,
		Attri.m_vWeaponInfoList,
		FTT_NORMAL_ATTACK,
		fTimeRate,
		dwPreDelay );

	pGrp->ClearAllLoopAni( 100.0f, true );
	pGrp->ClearAllActionAni( 100.0f, true );

	pGrp->SetActionAni( iAniID, 100.0f, 100.0f, 1.0f, 1.0f/fTimeRate, 0.0f, dwPreDelay );

	DWORD dwCurTime = FRAMEGETTIME();
	m_dwMotionEndTime = m_dwFireMotionEndTime = dwCurTime;

	m_dwMotionEndTime += pGrp->GetAnimationFullTime( iAniID ) * fTimeRate;
	m_dwMotionEndTime += dwPreDelay + Attri.m_dwEndDelay;

	m_dwFireMotionEndTime += dwPreDelay + pGrp->GetAnimationEventTime_e( iAniID, "firemotion_e" ) * fTimeRate;

	// Col Skip
	pChar->CheckCharColSkipSkillTime( iAniID, fTimeRate, dwPreDelay );

	pChar->SetReservedSliding( Attri.m_vForceInfoList, iAniID, fTimeRate, dwPreDelay );
}

const AttackAttribute& ioKeyMarkAttackBuff::GetActionAttribute( ioBaseChar* pChar )
{
	static AttackAttribute attri;
	ioBaseChar *pTarget = pChar->GetBaseChar( m_szAttackTarget );
	if( pTarget )
	{
		//Air
		if( IsAirState( pTarget ) )
		{
			if ( m_AirTargetAttackAttribute.m_AttackAnimation.IsEmpty() )
				return m_NormalTargetAttackAttribute;
			return m_AirTargetAttackAttribute;
		}
		//Down
		else if ( IsDownState( pTarget ) )
		{
			if ( m_DownTargetAttackAttribute.m_AttackAnimation.IsEmpty() )
				return m_NormalTargetAttackAttribute;
			return m_DownTargetAttackAttribute;
		}
		//Normal
		else
		{
			return m_NormalTargetAttackAttribute;
		}

	}
	return attri;
}

bool ioKeyMarkAttackBuff::IsAirState( ioBaseChar* pTarget )
{
	if( pTarget->IsFloating() )
		return true;

	return false;
}

bool ioKeyMarkAttackBuff::IsDownState( ioBaseChar* pTarget )
{
	CharState eState = pTarget->GetState();
	if( eState == CS_FROZEN && pTarget->GetBlowFrozenState() )
		return true;
	else if( eState == CS_BLOW_WOUNDED && !pTarget->IsFloatingState() )
		return true;
	else if( eState == CS_BOUND_BLOW_WOUNDED && !pTarget->IsFloatingState() )
		return true;
	else if( eState == CS_ETC_STATE && pTarget->IsApplyDownState(false) )
		return true;
	else if( eState == CS_STOP_MOTION && pTarget->GetBlowStopMotionState() )
		return true;
	else if( eState == CS_ICE_STATE && pTarget->GetBlowStopMotionState() )
		return true;
	else if( eState == CS_USING_SKILL && pTarget->IsApplyDownState(false) )
		return true;

	return false;
}

void ioKeyMarkAttackBuff::SetActionEndState( ioBaseChar *pChar )
{
	if ( !pChar )
		return;

	pChar->SetCurJumpPowerAmt( 0.0f );
	pChar->SetForcePowerAmt( 0.0f );
	pChar->SetGravityAmt( 0.0f );

	if ( pChar->IsNeedProcess() )
	{
		if ( pChar->IsFloating() )
			pChar->SetSKillEndJumpState( m_fEndJumpPower, false );
		else
			pChar->SetState( CS_DELAY, true );
	}
	m_szAttackTarget.Clear();
	m_dwKeyInputTime = 0;
	m_bInputKey = false;

	m_BuffState = BS_LOOP;
}

void ioKeyMarkAttackBuff::EndBuff()
{
	ioBuff::EndBuff();
	if ( !GetOwner() || !m_pOwner )
		return;

	if ( m_bReduceAllGauge )
		m_pOwner->SetCurItemSkillGauge( m_ItemName, 0 );
	ioPlayStage *pStage = m_pOwner->GetCreator();
	DestroyMaker( m_pOwner, pStage, m_dwA_MarkMapEffectID );
	DestroyMaker( m_pOwner, pStage, m_dwS_MarkMapEffectID );
	DestroyMaker( m_pOwner, pStage, m_dwD_MarkMapEffectID );
	ClearData();
}

void ioKeyMarkAttackBuff::ApplyExtraBuffInfo( SP2Packet &rkPacket )
{
	ioBaseChar *pOwner = GetOwner();
	if( !pOwner )
		return;

	ioPlayStage *pStage = pOwner->GetCreator();
	if ( !pStage )
		return;

	BYTE btState;
	rkPacket >> btState;

	switch( btState )
	{
	case BST_CHANGE_TARGET:
		{
			ioHashString szTarget_A,szTarget_S,szTarget_D;
			rkPacket >> szTarget_A >> szTarget_S >> szTarget_D;

			if ( m_szA_MarkTargetName != szTarget_A )
			{
				m_szA_MarkTargetName = szTarget_A;
				if ( !m_szA_MarkTargetName.IsEmpty() )
					CreateMarker( pOwner, pStage, m_szA_MarkTargetName, m_szMarkEffect, m_dwA_MarkMapEffectID );
				else
					DestroyMaker( pOwner, pStage, m_dwA_MarkMapEffectID );
			}
			if ( m_szS_MarkTargetName != szTarget_S )
			{
				m_szS_MarkTargetName = szTarget_S;

				if ( !m_szS_MarkTargetName.IsEmpty() )
					CreateMarker( pOwner, pStage, m_szS_MarkTargetName, m_szMarkEffect, m_dwS_MarkMapEffectID );
				else
					DestroyMaker( pOwner, pStage, m_dwS_MarkMapEffectID );

			}
			if ( m_szD_MarkTargetName != szTarget_D )
			{
				m_szD_MarkTargetName = szTarget_D;

				if ( !m_szD_MarkTargetName.IsEmpty() )
					CreateMarker( pOwner, pStage, m_szD_MarkTargetName, m_szMarkEffect, m_dwD_MarkMapEffectID );
				else
					DestroyMaker( pOwner, pStage, m_dwD_MarkMapEffectID );
			}
		}
		break;
	case BST_ATTACK:
		{
			D3DXVECTOR3 vPos;
			rkPacket >> vPos;
			D3DXQUATERNION qtRot;
			rkPacket >> qtRot;

			pOwner->SetWorldPosition( vPos );
			pOwner->SetTargetRotAndMoveDirChange( qtRot );
			rkPacket >> m_szAttackTarget;
			SetActionState( pOwner, pStage );
		}
	}
}

bool ioKeyMarkAttackBuff::IsProtected() const
{
	if( m_dwFireMotionEndTime > 0 && m_dwFireMotionEndTime > FRAMEGETTIME() )
	{
		return true;
	}
	return false;
}
