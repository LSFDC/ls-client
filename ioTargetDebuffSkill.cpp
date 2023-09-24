

#include "stdafx.h"

#include "ioBaseChar.h"
#include "ioPlayStage.h"
#include "ioGrowthUpInfo.h"

#include "ioTargetDebuffSkill.h"

#include "WeaponDefine.h"

ioTargetDebuffSkill::ioTargetDebuffSkill()
{
	ClearData();
}

ioTargetDebuffSkill::ioTargetDebuffSkill( const ioTargetDebuffSkill &rhs )
: ioNormalSkill( rhs ),
m_bDisableGrowth( rhs.m_bDisableGrowth ),
m_fGuagePerTic( rhs.m_fGuagePerTic ),
m_dwTicTime( rhs.m_dwTicTime ),
m_LoopDelayAnimation( rhs.m_LoopDelayAnimation ),
m_LoopMoveAnimation( rhs.m_LoopMoveAnimation ),
m_TargetWoundType( rhs.m_TargetWoundType ),
m_fTargetAngle( rhs.m_fTargetAngle ),
m_fTargetMaxRange( rhs.m_fTargetMaxRange ),
m_fTargetMinRange( rhs.m_fTargetMinRange ),
m_dwTargetMaxCnt( rhs.m_dwTargetMaxCnt ),
m_szRedGuideMarker( rhs.m_szRedGuideMarker ),
m_szBlueGuideMarker( rhs.m_szBlueGuideMarker ),
m_szRedAimMarker( rhs.m_szRedAimMarker ),
m_szBlueAimMarker( rhs.m_szBlueAimMarker ),
m_szRedChargeMarker( rhs.m_szRedChargeMarker ),
m_szBlueChargeMarker( rhs.m_szBlueChargeMarker ),
m_szRedFireMarker( rhs.m_szRedFireMarker ),
m_szBlueFireMarker( rhs.m_szBlueFireMarker ),
m_fScalingRate( rhs.m_fScalingRate ),
m_fCurScaleRate( rhs.m_fCurScaleRate ),
m_fScalingSpeed( rhs.m_fScalingSpeed ),
m_SkillZeroAimMotion( rhs.m_SkillZeroAimMotion ),
m_fSkillZeroAimMotionLate( rhs.m_fSkillZeroAimMotionLate ),
m_dwSkillZeroAimAttackCnt(  rhs.m_dwSkillZeroAimAttackCnt ),
m_dwKeyDurationTime( rhs.m_dwKeyDurationTime ),
m_AimAttribute( rhs.m_AimAttribute ),
m_TargetAimAttackBuffList( rhs.m_TargetAimAttackBuffList ),
m_bSkillZeroAimAttackGauge( rhs.m_bSkillZeroAimAttackGauge ),
m_bSkillAimAttackGauge( rhs.m_bSkillAimAttackGauge ),
m_dwAimMarkerChargeEnableCnt( rhs.m_dwAimMarkerChargeEnableCnt ),
m_dwSkillProtectedTime( rhs.m_dwSkillProtectedTime ),
m_SkillMapEffect( rhs.m_SkillMapEffect ),
m_fCharEffectOffset( rhs.m_fCharEffectOffset )
{
	ClearData();
}

ioTargetDebuffSkill::~ioTargetDebuffSkill()
{
}

void ioTargetDebuffSkill::ClearData()
{
	m_TargetList.clear();

	m_bSetHandMesh = false;
	m_bSetMoveAni = false;
	m_bFireMaker = false;

	// ������� ���
	m_dwCheckTime = 0;
	m_fTicGaugeRateByGrowth = FLOAT1;
	m_dwTicTimeByGrowth = 0;

	//maker
	m_dwGuidMaker = - 1;	
	m_dwMarkCurCnt = 0;
		
	m_dwCurScaleStartTime = 0;

	m_dwSkillZeroAimMotionEnd = 0;
	m_dwSkillAimMotionEnd = 0;
	m_dwCurSkillZeroAimAttackCnt = 0;

	m_dwFireStartTime = 0;
		
	m_dwSetLoopCnt = -1;
	m_dwCurAimMarkerChargeCnt = 0;

	m_dwStartSkillProtectedTime = 0;

	ResetSendList();
}

void ioTargetDebuffSkill::LoadProperty( ioINILoader &rkLoader )
{
	ioNormalSkill::LoadProperty( rkLoader );

	char szBuf[MAX_PATH];

	m_bDisableGrowth = rkLoader.LoadBool_e( "disable_growth", false );
	m_fGuagePerTic = rkLoader.LoadFloat_e( "gauge_per_tick", 0.0f );
	m_dwTicTime = (DWORD)rkLoader.LoadInt_e( "tick_time", 0 );

	rkLoader.LoadString_e( "skill_zero_aim_attack_motion", "", szBuf, MAX_PATH );
	m_SkillZeroAimMotion = szBuf;
	m_fSkillZeroAimMotionLate = rkLoader.LoadFloat_e( "skill_zero_aim_attack_motion_rate", FLOAT1 );

	m_dwKeyDurationTime = rkLoader.LoadInt_e( "skill_zero_aim_attack_duration", 0 );
	m_dwSkillZeroAimAttackCnt = rkLoader.LoadInt_e( "skill_zero_aim_attack_cnt", 1 );
	m_bSkillZeroAimAttackGauge = rkLoader.LoadBool_e( "skill_zero_aim_attack_reduce_gauge", 1 );

	wsprintf_e( szBuf, "skill_aim_attack");
	LoadAttackAttribute( szBuf, m_AimAttribute, rkLoader );
	m_bSkillAimAttackGauge = rkLoader.LoadBool_e( "skill_aim_attack_reduce_gauge", 1 );

	m_dwAimMarkerChargeEnableCnt = rkLoader.LoadInt_e( "charge_marker_enable_cnt", 1000 );
	m_dwAimMarkerChargeEnableCnt = max( m_dwAimMarkerChargeEnableCnt, 1 );

	m_dwSkillProtectedTime = rkLoader.LoadFloat_e( "skill_protect_time", 0 );

	// target search range
	m_TargetWoundType = (TargetWoundType)rkLoader.LoadInt_e( "target_wound_type", TWT_ALL );
	m_fTargetAngle = rkLoader.LoadFloat_e( "target_angle", 0.0f );
	m_fTargetMaxRange = rkLoader.LoadFloat_e( "target_max_range", 0.0f );
	m_fTargetMinRange = rkLoader.LoadFloat_e( "target_min_range", 0.0f );
	m_dwTargetMaxCnt = rkLoader.LoadInt_e( "attack_count", 0 );

	//loop motion
	rkLoader.LoadString_e( "delay_animation", "", szBuf, MAX_PATH );	
	m_LoopDelayAnimation = szBuf;
	rkLoader.LoadString_e( "move_animation", "", szBuf, MAX_PATH );
	m_LoopMoveAnimation	= szBuf;

	// aim mark
	rkLoader.LoadString_e( "red_guide_marker", "", szBuf, MAX_PATH );
	m_szRedGuideMarker = szBuf;
	rkLoader.LoadString_e( "blue_guide_marker", "", szBuf, MAX_PATH );
	m_szBlueGuideMarker = szBuf;

	rkLoader.LoadString_e( "red_aim_marker", "", szBuf, MAX_PATH );
	m_szRedAimMarker = szBuf;
	rkLoader.LoadString_e( "blue_aim_marker", "", szBuf, MAX_PATH );
	m_szBlueAimMarker = szBuf;

	rkLoader.LoadString_e( "red_charge_marker", "", szBuf, MAX_PATH );
	m_szRedChargeMarker = szBuf;
	rkLoader.LoadString_e( "blue_charge_marker", "", szBuf, MAX_PATH );
	m_szBlueChargeMarker = szBuf;

	rkLoader.LoadString_e( "red_fire_marker", "", szBuf, MAX_PATH );
	m_szRedFireMarker = szBuf;	
	rkLoader.LoadString_e( "blue_fire_marker", "", szBuf, MAX_PATH );
	m_szBlueFireMarker = szBuf;	

	//scale info
	m_fScalingRate = rkLoader.LoadFloat_e( "charge_scaling_rate", 0.0f );
	m_fCurScaleRate = m_fScalingRate;
	m_fScalingSpeed = rkLoader.LoadFloat_e( "charge_scaling_speed", 0.0f );
	

	rkLoader.LoadString_e( "skill_map_effect", "", szBuf, MAX_PATH );
	m_SkillMapEffect = szBuf;
	
	m_fCharEffectOffset = rkLoader.LoadFloat_e( "skill_map_effect_offset", 0.0f );

	LoadBuffList( rkLoader );
}

void ioTargetDebuffSkill::LoadBuffList( ioINILoader &rkLoader )
{
	int i, iCount;
	ioBuffInfo BuffInfo;
	char szTitle[MAX_PATH], szBuff[MAX_PATH];

	m_TargetAimAttackBuffList.clear();
	iCount = rkLoader.LoadInt_e( "skill_zero_aim_attack_buff_cnt", 0 );
	for( i=0; i<iCount; ++i )
	{
		wsprintf_e( szTitle, "skill_zero_aim_attack_buff%d", i+1 );
		rkLoader.LoadString( szTitle, "", szBuff, MAX_PATH );

		BuffInfo.m_BuffName = szBuff;

		if( !BuffInfo.m_BuffName.IsEmpty() )
		{			
			BuffInfo.m_iBuffType = g_BuffMaker.GetBuffType( BuffInfo.m_BuffName );			
			if( BuffInfo.m_iBuffType != BT_NONE )
			{	
				m_TargetAimAttackBuffList.push_back( BuffInfo );
			}
		}
	}
}

ioSkill* ioTargetDebuffSkill::Clone()
{
	return new ioTargetDebuffSkill( *this );
}

bool ioTargetDebuffSkill::OnSkillStart( ioBaseChar *pChar )
{
	if( !pChar )
		return false;

	ioNormalSkill::OnSkillStart( pChar );

	ClearData();

	m_SkillState = SS_PRE;
	m_dwCheckTime = FRAMEGETTIME();

	pChar->AttachEffect( m_SkillEffectName );

	if( m_bUseItemMesh )
	{
		pChar->ShowWeaponItemMesh( false );
	}

	if( !m_bSetHandMesh && m_bUseItemMesh && !m_HandMesh.IsEmpty() )
	{
		pChar->ChangeSkillWeaponMesh( this, m_HandMesh, m_bVisibleEquipMesh );
		m_bSetHandMesh = true;
	}

	SetCameraBuff( pChar );

	m_dwWeaponBaseIndex = pChar->IncreaseWeaponIndexBase();

	pChar->SetSkillMoveEnable( false, false );

	// ������� ��� �߰�.
	if( !m_bDisableGrowth )
	{
		ioPlayStage *pStage = pChar->GetCreator();
		if( pStage )
		{
			ioGrowthNormalUpInfo *pUpInfo = NULL;

			pUpInfo = ToNormalUpInfo( GetGrowthUpInfoByType(GT_SKILL_COOL_TIME, pChar->GetCharName()) );
			if( pUpInfo )
				m_fTicGaugeRateByGrowth = pUpInfo->GetValue(pStage);

			m_fTicGaugeRateByGrowth = min( m_fTicGaugeRateByGrowth, FLOAT100 );
			m_fTicGaugeRateByGrowth = FLOAT1 + ( m_fTicGaugeRateByGrowth/FLOAT100 );


			pUpInfo = ToNormalUpInfo( GetGrowthUpInfoByType(GT_BUFF_TIC_TIME, pChar->GetCharName()) );
			if( pUpInfo )
				m_dwTicTimeByGrowth = pUpInfo->GetValue(pStage);			
		}
	}

	if( !m_bReduceGauge )
	{
		float fCurGauge = pChar->ReduceNeedGaugeBySkill( this, GetNeedGauge(pChar->GetCharName()) );
		m_bReduceGauge = true;
	}
	
	ioPlayStage* pStage = g_SkillMaker.GetPlayStage();
	if( !StartPreDelayAnimation( pChar ) )	// Delay Animation�� ����
	{	
		SetLoopState( pChar, pStage );
		CreateGuideMarker( pChar, pStage );
	}	

	return true;
}

void ioTargetDebuffSkill::OnSkillEnd( ioBaseChar *pChar )
{
	ioNormalSkill::OnSkillEnd( pChar );

	// �⺻���� ����
	if( m_bUseItemMesh )
	{
		pChar->ShowWeaponItemMesh( true );
	}

	// ��ų �߰� ���ۿ� ���� �޽��� ����
	if( !m_HandMesh.IsEmpty() )
		pChar->RestoreSkillWeaponMesh( this, m_HandMesh, m_bVisibleEquipMesh );

	EndCameraBuff( pChar );

	pChar->SetSkillMoveEnable( false, false );

	if( !m_SkillSoundName.IsEmpty() )
	{
		g_SoundMgr.StopSound( m_SkillSoundName, m_SkillSoundID );		
	}

	ioPlayStage* pStage = g_SkillMaker.GetPlayStage();
	DestroyAllMarker( pChar, pStage );

	ClearData();
}

void ioTargetDebuffSkill::OnProcessState( ioBaseChar *pChar, ioPlayStage *pStage )
{
	DWORD dwCurTime = FRAMEGETTIME();

	float fCurTicGauge = m_fGuagePerTic / m_fTicGaugeRateByGrowth;
	int iTickGap = (int)( dwCurTime - m_dwCheckTime );
	int iCurTicTime = m_dwTicTime + m_dwTicTimeByGrowth;

	switch( m_SkillState )
	{
	case SS_PRE:
		{
			DWORD dwPreDelay = GetPreDelayTime();
			if( m_dwPreDelayStartTime + dwPreDelay < dwCurTime )
			{
				m_dwPreDelayStartTime = 0;

				EndPreDelayEffect( pChar );
				SetLoopState( pChar, pStage );
				CreateGuideMarker( pChar, pStage );
			}
		}
		break;
	case SS_LOOP:
		{	
			CheckNormalMoveAni( pChar );
			UpdateGuideMarker( pChar, pStage );
			UpdateScaleInfo( pChar, pStage );

			ResetSendList();

			//ù���� �� ���� Ÿ�� �����Ŀ� ���������� Ÿ���� ���� �� ��� 
			//������ ����->������ Ƚ�� ��ŭ Ÿ���� ó�� �� Ÿ�� �õ��� ������ ��ų ����
			if( 0 < m_dwSetLoopCnt )
			{
				if(	m_dwAimMarkerChargeEnableCnt <= m_dwCurAimMarkerChargeCnt && !m_bFireMaker )
				{
					SetEndState( pChar, pStage );
					return;
				}
			}
			else
			{
				//ù��° ����,������ Ÿ�� �������� ������ ã�´�.
				FindTarget( pChar, pStage );
			}

			UpdateTargetList( pChar, pStage );
			
			//����ȭ ó��
			if( pChar->IsNeedProcess() )
				SendList( pChar, pStage );
			
			if( pChar->IsAttackKey() && pChar->IsNeedProcess() )
			{
				if( !m_TargetList.empty() )
				{
					if( m_bFireMaker )
					{
						if( 0 < m_dwCurScaleStartTime && dwCurTime < m_dwCurScaleStartTime + m_dwKeyDurationTime )
							SetZeroAimFireState( pChar, pStage );
						else
							SetAimFireState( pChar, pStage );
					}
					else
						SetAimFireState( pChar, pStage );
				}
				else
				{
					SetAimFireState( pChar, pStage );
				}
			}
			
			if( m_bFireMaker && m_dwCurScaleStartTime + m_dwKeyDurationTime < dwCurTime )
			{
				m_bFireMaker = false;
			}
		}
		break;
	case SS_ZERO_AIM_FIRE:
		{	
			UpdateAllMarker( pChar, pStage );

			if( 0 < m_dwFireStartTime && m_dwFireStartTime < dwCurTime )
			{					
				OnZeroAimFireState( pChar,pStage );
				DestroyAllMarker( pChar, pStage );
				m_dwFireStartTime = 0;
			}
			
			if( 0 < m_dwSkillZeroAimMotionEnd && m_dwSkillZeroAimMotionEnd < dwCurTime )
			{
				if( COMPARE(  m_dwCurSkillZeroAimAttackCnt, 0, m_dwSkillZeroAimAttackCnt  ) )
					SetLoopState( pChar, pStage );
				else
					SetEndState( pChar, pStage );
			}
		}
		break;
	case SS_AIM_FIRE:
		{
			OnAimFireState( pChar, pStage, dwCurTime );

			if( 0 < m_dwSkillAimMotionEnd && m_dwSkillAimMotionEnd < dwCurTime )
			{				
				SetEndState( pChar, pStage );
			}
		}
		break;
	case SS_END:
		{
			
		}
		break;
	}

	if( m_SkillState != SS_END )
	{
		//ù��° Ÿ�� ���Ĵ� ������ ���Ҹ� ���� ����
		if( m_dwSetLoopCnt <= 0 )
		{
			if( iTickGap >= iCurTicTime )
			{
				m_dwCheckTime = dwCurTime - ( iTickGap - iCurTicTime );

				if( fCurTicGauge != 0.0f )
				{				
					if( pChar->ReduceCurSkillGaugeBySkill( GetOwnerItem(), fCurTicGauge ) <= 0.0f )
					{
						if( m_SkillState != SS_ZERO_AIM_FIRE && 
							m_SkillState != SS_AIM_FIRE &&
							m_SkillState != SS_END )
						{
							SetEndState( pChar, pStage );
						}
					}
				}
			}
		}
	}
}

void ioTargetDebuffSkill::StartScaleInfo()
{
	DWORD dwCurTime = FRAMEGETTIME();
	if( m_dwCurScaleStartTime == 0 )
	{		
		m_fCurScaleRate = m_fScalingRate;
		m_dwCurScaleStartTime = dwCurTime;
	}
}

void ioTargetDebuffSkill::ResetScaleInfo( DWORD dwCurTime )
{
	m_fCurScaleRate = m_fScalingRate;
	m_dwCurScaleStartTime = dwCurTime;
}


void ioTargetDebuffSkill::UpdateScaleInfo( ioBaseChar *pChar, ioPlayStage *pStage )
{
	if( m_bFireMaker )
		return;
	
	DWORD dwCurTime = FRAMEGETTIME();
	float fTimePerSec = g_FrameTimer.GetSecPerFrame();

	m_fCurScaleRate -= (fTimePerSec * m_fScalingSpeed);

	if( m_fCurScaleRate < FLOAT1 )
	{
		m_fCurScaleRate = m_fScalingRate;
		m_dwCurScaleStartTime = dwCurTime;

		m_bFireMaker = true;
		m_dwCurAimMarkerChargeCnt++;
	}	
}

void ioTargetDebuffSkill::SetZeroAimFireState( ioBaseChar *pChar, ioPlayStage *pStage )
{
	if( !pChar ) return;
	if( !pStage ) return;

	DWORD dwCurTime = FRAMEGETTIME();

	if( m_dwStartSkillProtectedTime == 0 )
		m_dwStartSkillProtectedTime = dwCurTime;
	
	m_SkillState = SS_ZERO_AIM_FIRE;
	ioEntityGroup *pGrp = pChar->GetGroup();

	int iAniID = 0;
	iAniID = pGrp->GetAnimationIdx( m_SkillZeroAimMotion );
	if( iAniID == -1 )
		return;

	float fTimeRate = m_fSkillZeroAimMotionLate * m_fExtraAniRate;

	pGrp->ClearAllActionAni( FLOAT100, true );
	pGrp->ClearAllLoopAni( FLOAT100, true );	
	pGrp->SetActionAni( iAniID, FLOAT100, FLOAT100, FLOAT1, FLOAT1/fTimeRate );

	m_dwSkillZeroAimMotionEnd = dwCurTime + pGrp->GetAnimationFullTime( iAniID ) * fTimeRate;

	m_dwFireMotionEndTime = dwCurTime;
	m_dwFireMotionEndTime += pGrp->GetAnimationEventTime_e( iAniID, "firemotion_e" ) * fTimeRate;

	if( m_dwFireStartTime == 0 )
	{
		m_dwFireStartTime = dwCurTime;
		m_dwFireStartTime += pGrp->GetAnimationEventTime_e( iAniID, "fire_s" ) * fTimeRate;
	}
	
	pChar->SetSkillMoveEnable( false, false );

	if( m_bSkillZeroAimAttackGauge )
	{			
		pChar->ReduceCurSkillGaugeBySkill( GetOwnerItem(), pChar->GetCurItemSkillGauge( GetOwnerItem()->GetName() ) );
	}
	else if(  m_dwCurSkillZeroAimAttackCnt == m_dwSkillZeroAimAttackCnt-1 )
	{
		pChar->ReduceCurSkillGaugeBySkill( GetOwnerItem(), pChar->GetCurItemSkillGauge( GetOwnerItem()->GetName() ) );
	}


	if( pChar->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_SKILL_EXTRA_INFO );
		kPacket << pChar->GetCharName();
		kPacket << GetName();
		kPacket << SSC_ZERO_AIM_FIRE;				
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}	
}

void ioTargetDebuffSkill::OnZeroAimFireState( ioBaseChar *pChar, ioPlayStage *pStage )
{	
	if( !pChar ) return;
	if( !pStage ) return;

	ioHashString szItemName;
	ioItem *pItem = GetOwnerItem();

	if( pItem )
		szItemName = pItem->GetName();	
	else
		return;
		
	TargetList::iterator iter = m_TargetList.begin();

	bool bFired = false;
	for( ; iter != m_TargetList.end(); ++iter )
	{
		ioBaseChar* pTarget = pStage->GetBaseChar( (*iter).second.m_szTargetName );
		if( pTarget )
		{
			if( IsEnableTargetState( pTarget, false, false ) )
			{
				ioBuffInfo pInfo = m_TargetAimAttackBuffList[m_dwCurSkillZeroAimAttackCnt];
				pTarget->AddNewBuff( pInfo.m_BuffName,  pChar->GetCharName(), szItemName, NULL );
				bFired = true;
			}
		}		
	}

	if( bFired )
		m_dwCurSkillZeroAimAttackCnt++;
}

void ioTargetDebuffSkill::SetAimFireState( ioBaseChar *pChar, ioPlayStage *pStage )
{
	if( !pChar ) return;
	if( !pStage ) return;

	m_SkillState = SS_AIM_FIRE;

	DWORD dwCurTime = FRAMEGETTIME();
	ioEntityGroup *pGrp = pChar->GetGroup();

	int iAniID = 0;
	iAniID = pGrp->GetAnimationIdx( m_AimAttribute.m_AttackAnimation );
	if( iAniID == -1 )
		return;

	float fTimeRate = m_AimAttribute.m_fAttackAniRate * m_fExtraAniRate;

	pGrp->ClearAllActionAni( FLOAT100, true );
	pGrp->ClearAllLoopAni( FLOAT100, true );
	pGrp->SetActionAni( iAniID, FLOAT100, FLOAT100, FLOAT1, FLOAT1/fTimeRate );

	m_dwSkillAimMotionEnd = dwCurTime + pGrp->GetAnimationFullTime( iAniID );
	
	m_dwFireMotionEndTime = dwCurTime;
	m_dwFireMotionEndTime += pGrp->GetAnimationEventTime_e( iAniID, "firemotion_e" ) * fTimeRate;

	if( m_dwFireStartTime == 0 )
	{
		m_dwFireStartTime = dwCurTime;
		m_dwFireStartTime += pGrp->GetAnimationEventTime_e( iAniID, "fire_s" ) * fTimeRate;
	}

	if( m_TargetList.empty() )
	{
		D3DXVECTOR3 vScale = D3DXVECTOR3(FLOAT1, FLOAT1, FLOAT1 );
		D3DXVECTOR3 vDir = pChar->GetTargetRot() * D3DXVECTOR3(0.0f, 0.0f, -FLOAT1);
		D3DXVec3Normalize( &vDir, &vDir );

		D3DXQUATERNION qtCharRot;
		ioMath::CalcDirectionQuaternion( qtCharRot, -vDir );

		D3DXVECTOR3 vPos = pChar->GetWorldPosition();
		
		vPos += m_fCharEffectOffset * vDir;

		if( !m_SkillMapEffect.IsEmpty() )
		{
			ioMapEffect* pMapEffect = pStage->CreateMapEffect( m_SkillMapEffect, vPos, vScale );

			if( pMapEffect )
			{	
				pMapEffect->SetWorldOrientation( qtCharRot );
			}
		}
	}

	pChar->SetSkillMoveEnable( false, false );

	if( pChar->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_SKILL_EXTRA_INFO );
		kPacket << pChar->GetCharName();
		kPacket << GetName();
		kPacket << SSC_AIM_FIRE;	
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
	
	if( m_bSkillAimAttackGauge )
		pChar->ReduceCurSkillGaugeBySkill( GetOwnerItem(), pChar->GetCurItemSkillGauge( GetOwnerItem()->GetName() ) );
}

void ioTargetDebuffSkill::OnAimFireState( ioBaseChar *pChar, ioPlayStage *pStage, DWORD dwCurTime )
{	
	if( !pChar )
		return;

	if( !pStage )
		return;

	UpdateAllMarker( pChar, pStage );

	int iWeaponCnt = 0;
	if( 0 < m_dwFireStartTime && m_dwFireStartTime < dwCurTime )
	{
		if( !m_TargetList.empty() )
		{
			TargetList::iterator target_iter = m_TargetList.begin();
			m_CurFireIter = m_TargetList.begin();

			for( ; target_iter != m_TargetList.end(); ++target_iter )
			{				
				ioBaseChar* pTarget = pStage->GetBaseChar( (*target_iter).second.m_szTargetName );
				if( pTarget )
				{					
					WeaponInfoList::iterator iter = m_AimAttribute.m_vWeaponInfoList.begin();
					for( ; iter != m_AimAttribute.m_vWeaponInfoList.end(); ++iter )
					{
						pChar->SkillFireExplicit( (*iter),
												  GetName(),
												  m_dwWeaponBaseIndex+iWeaponCnt,
												  pChar->GetWorldPosition() );

						iWeaponCnt++;
					}

					if( !m_SkillMapEffect.IsEmpty() )
					{
						D3DXVECTOR3 vScale = D3DXVECTOR3(FLOAT1, FLOAT1, FLOAT1 );
						ioMapEffect* pMapEffect = pStage->CreateMapEffect( m_SkillMapEffect, 
							pTarget->GetWorldPosition(), 
							vScale );
					}
				}
			}
		}
		else
		{
			WeaponInfoList::iterator iter = m_AimAttribute.m_vWeaponInfoList.begin();
			for( ; iter != m_AimAttribute.m_vWeaponInfoList.end(); ++iter )
			{
				pChar->SkillFireExplicit( (*iter),
										  GetName(),
										  m_dwWeaponBaseIndex+iWeaponCnt,
										  pChar->GetWorldPosition() );

				iWeaponCnt++;
			}
		}

		DestroyAllMarker( pChar, pStage );
		m_dwFireStartTime = 0;
	}
}

void ioTargetDebuffSkill::CreateGuideMarker( ioBaseChar *pChar, ioPlayStage *pStage )
{
	if( !pChar ) return;
	if( !pStage ) return;


	D3DXVECTOR3 vScale( FLOAT1, FLOAT1, FLOAT1 );
	D3DXVECTOR3 vPos = pChar->GetWorldPosition();
	D3DXVECTOR3 vDir = pChar->GetTargetRot() * D3DXVECTOR3( 0.0f, 0.0f, -FLOAT1 );
	
	vPos.y = pChar->GetMidHeightByRate();
	vPos += ( vDir * m_fTargetMaxRange );
	
	ioMapEffect *pMapEffect = NULL;

	if( pChar->IsNeedProcess() ) 
	{
		pMapEffect = pStage->CreateMapEffect( m_szBlueGuideMarker, vPos, vScale );
	}
	else
	{
		if( pChar->GetTeam() == TEAM_BLUE )
			pMapEffect = pStage->CreateMapEffect( m_szBlueGuideMarker, vPos, vScale );
		else
			pMapEffect = pStage->CreateMapEffect( m_szRedGuideMarker, vPos, vScale );
	}

	if( pMapEffect )
	{
		m_dwGuidMaker = pMapEffect->GetUniqueID();

		D3DXQUATERNION qtCharRot;
		ioMath::CalcDirectionQuaternion( qtCharRot, -vDir );
		pMapEffect->SetWorldOrientation( qtCharRot );
	}
}

void ioTargetDebuffSkill::UpdateGuideMarker( ioBaseChar *pChar, ioPlayStage *pStage )
{
	if( !pChar ) return;
	if( !pStage ) return;

	if( !m_TargetList.empty() )
	{			
		DestroyGuideMarker( pChar );
		return;
	}

	if( m_dwGuidMaker == -1 && m_TargetList.empty() )
	{
		CreateGuideMarker( pChar, pStage );
		return;
	}

	D3DXVECTOR3 vScale( FLOAT1, FLOAT1, FLOAT1 );
	D3DXVECTOR3 vPos = pChar->GetWorldPosition();
	D3DXVECTOR3 vDir = pChar->GetTargetRot() * D3DXVECTOR3( 0.0f, 0.0f, -FLOAT1 );
	vPos.y = pChar->GetMidHeightByRate();
	vPos += ( vDir * m_fTargetMaxRange );

	ioMapEffect *pMapEffect = pStage->FindMapEffect( m_dwGuidMaker );
	if( pMapEffect )
	{
		pMapEffect->SetWorldPosition( vPos );

		D3DXQUATERNION qtCharRot;
		ioMath::CalcDirectionQuaternion( qtCharRot, -vDir );
		pMapEffect->SetWorldOrientation( qtCharRot );
	}
	
}

void ioTargetDebuffSkill::DestroyGuideMarker( ioBaseChar *pChar )
{
	if( !pChar )
		return;

	if( m_dwGuidMaker != -1 )
	{
		pChar->EndMapEffect( m_dwGuidMaker, false );
		m_dwGuidMaker = -1;
	}
}

void ioTargetDebuffSkill::CreateFireMarker( ioBaseChar *pChar, ioPlayStage *pStage, EffectInfo& pInfo )
{
	if( !pChar ) return;
	if( !pStage ) return;

	ioMapEffect *pMapEffect = pStage->FindMapEffect( pInfo.m_dwFireEffectID );
	if( !pMapEffect )
	{
		D3DXVECTOR3 vScale( FLOAT1, FLOAT1, FLOAT1 );
		D3DXVECTOR3 vPos = pChar->GetWorldPosition();
		D3DXVECTOR3 vDir = pChar->GetTargetRot() * D3DXVECTOR3( 0.0f, 0.0f, -FLOAT1 );

		if( pChar->IsNeedProcess() ) 
		{
			pMapEffect = pStage->CreateMapEffect( m_szBlueFireMarker, vPos, vScale );
		}
		else
		{
			if( pChar->GetTeam() == TEAM_BLUE )
				pMapEffect = pStage->CreateMapEffect( m_szBlueFireMarker, vPos, vScale );
			else
				pMapEffect = pStage->CreateMapEffect( m_szRedFireMarker, vPos, vScale );
		}		
		if( pMapEffect )
		{
			pInfo.m_dwFireEffectID = pMapEffect->GetUniqueID();

			D3DXQUATERNION qtCharRot;
			ioMath::CalcDirectionQuaternion( qtCharRot, -vDir );
			pMapEffect->SetWorldOrientation( qtCharRot );
		}
	}
}

void ioTargetDebuffSkill::UpdateFireMarker( ioBaseChar *pTarget, ioPlayStage *pStage, UniqueObjID dwEffectID )
{
	D3DXVECTOR3 vTargetPos = pTarget->GetWorldPosition();
	vTargetPos.y = pTarget->GetMidHeightByRate();

	ioMapEffect *pMapEffect = pStage->FindMapEffect( dwEffectID );
	if( pMapEffect )
	{
		pMapEffect->SetWorldPosition( vTargetPos );
	}
}

void ioTargetDebuffSkill::SetLoopState( ioBaseChar *pChar, ioPlayStage *pStage )
{
	if( !pChar )
		return;

	DWORD dwCurTime = FRAMEGETTIME();
	m_SkillState = SS_LOOP;

	m_dwSetLoopCnt++;
	m_dwCurAimMarkerChargeCnt = 0;

	ioEntityGroup *pGrp = pChar->GetGroup();

	int iAniID = 0;
	iAniID = pGrp->GetAnimationIdx( m_LoopDelayAnimation );
	if( iAniID == -1 )
		return;

	pGrp->ClearAllActionAni( FLOAT100, true );
	pGrp->ClearAllLoopAni( FLOAT100, true );

	pGrp->SetLoopAni( iAniID, FLOAT100 );
	m_dwMotionEndTime = 0;

	pChar->SetSkillMoveEnable( true, true );

	m_bSetMoveAni = false;
}

void ioTargetDebuffSkill::CheckNormalMoveAni( ioBaseChar* pChar )
{
	if( !pChar ) return;

	DWORD dwCurTime = FRAMEGETTIME();
	ioEntityGroup *pGrp = pChar->GetGroup();
	int iAniID = 0;

	if( pChar->IsSettedDirection() )
	{	
		if( !m_bSetMoveAni )
		{	
			iAniID = pGrp->GetAnimationIdx( m_LoopMoveAnimation );
			if( iAniID == -1 )
				return;

			pGrp->ClearAllActionAni( FLOAT100, true );
			pGrp->ClearAllLoopAni( FLOAT100, true );

			pGrp->SetLoopAni( iAniID, FLOAT100 );	
			m_bSetMoveAni = true;
		}
	}
	else
	{
		if( m_bSetMoveAni )
		{
			iAniID = pGrp->GetAnimationIdx( m_LoopDelayAnimation );
			if( iAniID == -1 )
				return;

			pGrp->ClearAllActionAni( FLOAT100, true );
			pGrp->ClearAllLoopAni( FLOAT100, true );

			pGrp->SetLoopAni( iAniID, FLOAT100 );
			m_bSetMoveAni = false;
		}
	}
}

void ioTargetDebuffSkill::FindTarget( ioBaseChar *pChar, ioPlayStage* pStage )
{
	if( !pChar ) return;
	if( !pStage ) return;

	D3DXQUATERNION qtRot = pChar->GetTargetRot();
	D3DXVECTOR3 vDir = qtRot * D3DXVECTOR3( 0.0f, 0.0f, -FLOAT1 );
	D3DXVec3Normalize( &vDir, &vDir );

	const BaseCharList &rkCharList = pStage->GetBaseCharList();

	BaseCharList::const_iterator iter;
	for( iter=rkCharList.begin() ; iter!=rkCharList.end() ; ++iter )
	{
		ioBaseChar *pTarget = *iter;

		if( pTarget == pChar )
			continue;

		if( pTarget->GetTeam() == pChar->GetTeam() )
			continue;

		if( pTarget->IsSystemState() )
			continue;
			
		if( !pTarget->IsEnableTarget() )
			continue;


		bool bEnableTarget = false;
		if( IsEnableTargetState( pTarget, true ) )
			bEnableTarget = true;

		DWORD dwCurTime = FRAMEGETTIME();

		D3DXVECTOR3 vDiff = pTarget->GetMidPositionByRate() - pChar->GetMidPositionByRate();
		float fLengthSq = D3DXVec3LengthSq( &vDiff );

		float fMaxRangeSq = m_fTargetMaxRange * m_fTargetMaxRange;
		float fMinRangeSq = m_fTargetMinRange * m_fTargetMinRange;

		//Already Target Find
		bool bFind = false;
		TargetList::iterator iter = m_TargetList.find( pTarget->GetCharName() );
		if( ( m_TargetList.end() != iter ) )
			bFind = true;

		//Range Test
		bool bRangeAvailable = false;
		if( COMPARE( fLengthSq, fMinRangeSq, fMaxRangeSq ) )
			bRangeAvailable = true;

		//Angle Test
		bool bAngleAvailable = false;
		D3DXVec3Normalize( &vDiff, &vDiff );
		float fValue = D3DXVec3Dot( &vDir, &vDiff );
		float fAngleGap = RADtoDEG( acosf(fValue) );
		if( fAngleGap <= m_fTargetAngle )
			bAngleAvailable = true;

		if( bRangeAvailable && bAngleAvailable )
		{	
			if( !bFind )
			{
				if( !bEnableTarget )
					continue;

				//Ÿ���� �Ǵ� ����� ��ų ����ڿ��� Ÿ���� ������ �˸�
				if( m_dwMarkCurCnt < m_dwTargetMaxCnt )
				{
					AddTarget( pStage, pChar, pTarget, dwCurTime, m_bFireMaker );
					m_vAddTargetList.push_back( pTarget->GetCharName() );
				}
			}
			else
			{
				//2012. 6. 25 ������Ʈ �κ� �и� => UpdateTargetList()
			}
		}
		else
		{
			//Ÿ���ÿ��� ���� �Ǵ� ����� ��ų ����ڿ��� �˸�
			if( bFind )
			{				
				m_vRemoveTargetList.push_back( pTarget->GetCharName() );
			}
		}
	}

	if( 0 < m_TargetList.size() )
		StartScaleInfo();
	else
		ResetScaleInfo();
}

void ioTargetDebuffSkill::AddTarget( ioPlayStage* pStage,
									 ioBaseChar* pOwner,
									 ioBaseChar* pTarget,
									 DWORD dwCurTime,
									 bool bMarker,
									 bool CheckList )
{
	if( !pTarget )
		return;

	if( !pOwner )
		return;

	if( CheckList )
	{
		//����Ʈ�� �̹� �����ϸ� �н�
		TargetList::iterator iter = m_TargetList.find( pTarget->GetCharName() );
		if( ( m_TargetList.end() != iter ) )
			return;
	}

	//Create Effect
	EffectInfo pInfo;
	pInfo.m_dwCreateTime = dwCurTime;
	pInfo.m_szTargetName = pTarget->GetCharName();
	if( m_bFireMaker )
	{
		DestroyMarker( pOwner, pInfo.m_dwAimEffectID );
		DestroyMarker( pOwner, pInfo.m_dwChargeEffectID );
		CreateFireMarker( pOwner, pStage, pInfo );
	}
	else
	{
		DestroyMarker( pOwner, pInfo.m_dwFireEffectID );		
		CreateAimMarker( pOwner, pTarget, pStage, pInfo );				
		CreateChargeMarker( pOwner, pTarget, pStage, pInfo );
	}

	//Add Effect
	m_TargetList[pTarget->GetCharName()] = pInfo;
	m_dwMarkCurCnt++;
}

void ioTargetDebuffSkill::RemoveTarget( ioBaseChar* pOwner, ioBaseChar* pTarget, bool CheckList )
{
	if( !pTarget )
		return;

	if( !pOwner )
		return;

	if( CheckList )
	{
		//����Ʈ�� ������ �н�
		TargetList::iterator iter = m_TargetList.find( pTarget->GetCharName() );
		if( ( m_TargetList.end() == iter ) )
			return;
	}

	//Destory Effect
	EffectInfo pInfo = m_TargetList[pTarget->GetCharName()];

	DestroyMarker( pOwner, pInfo.m_dwAimEffectID );
	DestroyMarker( pOwner, pInfo.m_dwChargeEffectID );
	DestroyMarker( pOwner, pInfo.m_dwFireEffectID );
	m_TargetList.erase(pTarget->GetCharName());

	m_dwMarkCurCnt--;
}

void ioTargetDebuffSkill::RemoveTarget( ioBaseChar* pOwner, ioHashString szTargetName )
{
	//����Ʈ�� ������ �н�
	TargetList::iterator iter = m_TargetList.find( szTargetName );
	if( ( m_TargetList.end() == iter ) )
		return;

	//Destory Effect
	EffectInfo pInfo = m_TargetList[szTargetName];

	DestroyMarker( pOwner, pInfo.m_dwAimEffectID );
	DestroyMarker( pOwner, pInfo.m_dwChargeEffectID );
	DestroyMarker( pOwner, pInfo.m_dwFireEffectID );

	m_TargetList.erase( szTargetName );
	m_dwMarkCurCnt--;
}

void ioTargetDebuffSkill::UpdateTarget( ioBaseChar* pOwner,
										ioBaseChar* pTarget,
										ioPlayStage* pStage,
										bool bMarker )
{
	if( !pTarget )
		return;

	if( !pOwner )
		return;

	EffectInfo pInfo = m_TargetList[pTarget->GetCharName()];
	if( m_bFireMaker )
	{
		DestroyMarker( pOwner, pInfo.m_dwAimEffectID );
		DestroyMarker( pOwner, pInfo.m_dwChargeEffectID );

		//�̹� �����Ѵٸ� �Լ� ������ ����Ʈ�� �������� ����
		CreateFireMarker( pOwner, pStage, pInfo );
		UpdateFireMarker( pTarget, pStage, pInfo.m_dwFireEffectID );

		m_TargetList[pTarget->GetCharName()] = pInfo;
	}
	else
	{
		DestroyMarker( pOwner, pInfo.m_dwFireEffectID );

		//�̹� �����Ѵٸ� �Լ� ������ ����Ʈ�� �������� ����
		CreateAimMarker( pOwner, pTarget, pStage, pInfo );
		CreateChargeMarker( pOwner, pTarget, pStage, pInfo );

		UpdateAimMarker( pTarget, pStage, pInfo.m_dwAimEffectID );
		UpdateChargeMarker( pTarget, pStage, pInfo.m_dwChargeEffectID );	

		m_TargetList[pTarget->GetCharName()] = pInfo;
	}
}

void ioTargetDebuffSkill::UpdateTargetList( ioBaseChar* pOwner, ioPlayStage* pStage )
{
	TargetList::iterator target_iter;	
	for( target_iter = m_TargetList.begin(); target_iter != m_TargetList.end(); ++target_iter )
	{
		ioHashString szTargetName = (*target_iter).second.m_szTargetName;
		ioBaseChar* pTarget = pStage->GetBaseChar( szTargetName );

		//Ÿ�� ��ȿ�� üũ
		if( pTarget )
		{			
			if( IsEnableTargetState( pTarget, false ) )
			{
				UpdateTarget( pOwner, pTarget, pStage, m_bFireMaker );					
			}
			else
			{
				//Ÿ���ÿ��� ���� �Ǵ� ����� ��ų ����ڿ��� �˸�
				m_vRemoveTargetList.push_back( szTargetName );
			}
		}
		else
		{
			m_vRemoveNonTargetList.push_back( szTargetName );
		}
	}

	//������ ���������� ���������� ���ǿ� ���� �ʴ� ���
	int iSize = m_vRemoveTargetList.size();
	for( int i = 0; i < iSize; ++i )
	{
		ioHashString szTargetName = m_vRemoveTargetList[i];
		ioBaseChar* pTarget = pStage->GetBaseChar( szTargetName );
		
		if( pTarget )
		{
			RemoveTarget( pOwner, pTarget );
		}
	}

	//������ ���������� �������� �ʴ� ���
	iSize = m_vRemoveNonTargetList.size();
	for( int i = 0; i < iSize; ++i )
	{		
		ioHashString szTargetName = m_vRemoveNonTargetList[i];
		RemoveTarget( pOwner, szTargetName );
	}
}

void ioTargetDebuffSkill::ResetSendList()
{
	m_vAddTargetList.clear();
	m_vRemoveTargetList.clear();
	m_vRemoveNonTargetList.clear();
}

void ioTargetDebuffSkill::SendList(  ioBaseChar* pOwner, ioPlayStage* pStage )
{
	int iAddUserSize = (int)m_vAddTargetList.size();
	int iRemoveUserSize = (int)m_vRemoveTargetList.size();
	int iRemoveNonUserSize = (int)m_vRemoveNonTargetList.size();

	if( iAddUserSize <= 0 && iRemoveUserSize <= 0 && iRemoveNonUserSize <= 0 )
		return;

	SP2Packet kPacket( CUPK_SKILL_EXTRA_INFO );
	kPacket << pOwner->GetCharName();
	kPacket << GetName();
	kPacket << SSC_UPDATE_TARGETLIST;
	kPacket << iAddUserSize;

	if( 0 < iAddUserSize )
	{
		for( int i = 0 ; i < iAddUserSize; ++i )
			kPacket << m_vAddTargetList[i];
	}

	kPacket << iRemoveUserSize;
	if( 0 < iRemoveUserSize )
	{
		for( int i = 0 ; i < iRemoveUserSize; ++i )
			kPacket << m_vRemoveTargetList[i];
	}

	kPacket << iRemoveNonUserSize;
	if( 0 < iRemoveNonUserSize )
	{
		for( int i = 0 ; i < iRemoveNonUserSize; ++i )
			kPacket << m_vRemoveNonTargetList[i];
	}

	P2PNetwork::SendToAllPlayingUser( kPacket );
	
}

bool ioTargetDebuffSkill::IsEnableTargetState( ioBaseChar *pTarget, bool bCheckTargetWound, bool bCheckProtected )
{
	if( !pTarget )
		return false;

	if( bCheckProtected )
	{	
		if( pTarget->IsProtectState() )
			return false;

		if( pTarget->IsSkillProtected() )
			return false;
	}

	if( 0 < m_dwSetLoopCnt )
	{
		if( !pTarget->HasBuff( BT_FLAME_STATUE ) )
			return false;
	}

	ioPlayStage *pStage = pTarget->GetCreator();
	if( pStage )
	{
		ioPlayMode *pMode = pStage->GetPlayMode();
		if( pMode && pMode->GetModeType() == MT_FOOTBALL )
			return false;
	}
	else{
		return false;
	}

	if( bCheckTargetWound )
	{
		if( !pTarget->CheckTargetWoundType(m_TargetWoundType) )
			return false;
	}

	return true;
}

void ioTargetDebuffSkill::CreateAimMarker( ioBaseChar* pChar, ioBaseChar *pTarget, ioPlayStage* pStage, EffectInfo& pInfo )
{		
	if( !pTarget ) return;
	
	ioMapEffect *pMapEffect = pStage->FindMapEffect( pInfo.m_dwAimEffectID );
	if( !pMapEffect )
	{
		D3DXVECTOR3 vScale( FLOAT1, FLOAT1, FLOAT1 );
		D3DXVECTOR3 vTargetPos = pTarget->GetWorldPosition();
		vTargetPos.y = pTarget->GetMidHeightByRate();

		if( pChar->IsNeedProcess() ) 
		{
			pMapEffect = pStage->CreateMapEffect( m_szBlueAimMarker, vTargetPos, vScale );
		}
		else
		{
			if( pChar->GetTeam() == TEAM_BLUE )
				pMapEffect = pStage->CreateMapEffect( m_szBlueAimMarker, vTargetPos, vScale );
			else
				pMapEffect = pStage->CreateMapEffect( m_szRedAimMarker, vTargetPos, vScale );
		}

		if( pMapEffect )
		{	
			pInfo.m_dwAimEffectID = pMapEffect->GetUniqueID();
		}
	}
}

void ioTargetDebuffSkill::UpdateAimMarker( ioBaseChar *pTarget, ioPlayStage *pStage, UniqueObjID dwEffectID )
{
	D3DXVECTOR3 vTargetPos = pTarget->GetWorldPosition();
	vTargetPos.y = pTarget->GetMidHeightByRate();

	ioMapEffect *pMapEffect = pStage->FindMapEffect( dwEffectID );
	if( pMapEffect )
	{
		pMapEffect->SetWorldPosition( vTargetPos );
	}

}

void ioTargetDebuffSkill::DestroyMarker( ioBaseChar *pChar, DWORD& dwMarker )
{	
	if( !pChar )
		return;

	if( dwMarker != -1 )
	{
		pChar->EndMapEffect( dwMarker, false );
		dwMarker = -1;
	}
}

void ioTargetDebuffSkill::CreateChargeMarker(ioBaseChar *pChar, ioBaseChar *pTarget, ioPlayStage* pStage, EffectInfo& pInfo )
{		
	if( !pTarget ) return;
	if( !pStage ) return;

	ioMapEffect *pMapEffect = pStage->FindMapEffect( pInfo.m_dwChargeEffectID );
	if( !pMapEffect )
	{
		D3DXVECTOR3 vScale( m_fCurScaleRate, m_fCurScaleRate, m_fCurScaleRate );
		D3DXVECTOR3 vTargetPos = pTarget->GetWorldPosition();
		vTargetPos.y = pTarget->GetMidHeightByRate();
		
		if( pChar->IsNeedProcess() )
		{
			pMapEffect = pStage->CreateMapEffect( m_szBlueChargeMarker, vTargetPos, vScale );
		}
		else
		{
			if( pChar->GetTeam() == TEAM_BLUE )
				pMapEffect = pStage->CreateMapEffect( m_szBlueChargeMarker, vTargetPos, vScale );
			else
				pMapEffect = pStage->CreateMapEffect( m_szRedChargeMarker, vTargetPos, vScale );
		}
		if( pMapEffect )
		{
			pInfo.m_dwChargeEffectID = pMapEffect->GetUniqueID();		
		}
	}
}

void ioTargetDebuffSkill::UpdateChargeMarker( ioBaseChar *pTarget, ioPlayStage *pStage, UniqueObjID dwEffectID )
{
	if( !pTarget ) return;
	if( !pStage ) return;
		
	float fScaleRate = m_fCurScaleRate;

	D3DXVECTOR3 vScale( fScaleRate, fScaleRate, fScaleRate );
	D3DXVECTOR3 vTargetPos = pTarget->GetWorldPosition();
	vTargetPos.y = pTarget->GetMidHeightByRate();
	
	ioMapEffect *pMapEffect = pStage->FindMapEffect( dwEffectID );
	if( pMapEffect )
	{
		pMapEffect->SetWorldPosition( vTargetPos );
		ioEffect* pEffect = pMapEffect->GetEffect();
		if( pEffect )
		{
			pEffect->GetParentNode()->SetScale( vScale );
		}
	}
}

void ioTargetDebuffSkill::UpdateAllMarker( ioBaseChar *pChar, ioPlayStage *pStage )
{
	TargetList::iterator target_iter = m_TargetList.begin();
	for( ; target_iter != m_TargetList.end(); ++target_iter )
	{
		ioBaseChar* pTarget = pStage->GetBaseChar( (*target_iter).second.m_szTargetName );
		if( pTarget )
		{
			UpdateFireMarker( pTarget, pStage, (*target_iter).second.m_dwFireEffectID );
			UpdateAimMarker( pTarget, pStage, (*target_iter).second.m_dwAimEffectID );
			UpdateChargeMarker(  pTarget, pStage, (*target_iter).second.m_dwChargeEffectID );
		}
	}
}

void ioTargetDebuffSkill::DestroyAllMarker( ioBaseChar *pChar, ioPlayStage *pStage )
{
	if( !pChar )
		return;
	
	DestroyGuideMarker( pChar );

	TargetList::iterator iter = m_TargetList.begin();
	for( ; iter != m_TargetList.end(); ++iter )
	{	
		EffectInfo pInfo = (*iter).second;
	
		if( pInfo.m_dwAimEffectID != -1 )
		{			
			DestroyMarker( pChar, pInfo.m_dwAimEffectID );
			pInfo.m_dwAimEffectID = -1;
		}

		if( pInfo.m_dwChargeEffectID != -1 )
		{
			DestroyMarker( pChar, pInfo.m_dwChargeEffectID );
			pInfo.m_dwChargeEffectID = -1;
		}

		if( pInfo.m_dwFireEffectID != -1 )
		{
			DestroyMarker( pChar, pInfo.m_dwFireEffectID );
			pInfo.m_dwFireEffectID = -1;
		}

		(*iter).second = pInfo;
	}
}

void ioTargetDebuffSkill::SetEndState( ioBaseChar *pChar, ioPlayStage *pStage )
{
	m_SkillState = SS_END;

	DestroyAllMarker( pChar, pStage );
}

void ioTargetDebuffSkill::ApplyAttributeToWeapon( ioWeapon *pWeapon, ioPlayStage *pStage )
{	
	ioSkill::ApplyAttributeToWeapon( pWeapon, pStage );
	
	if( !pStage )
		return;

	ioZoneEffectWeapon *pZone = ToZoneEffectWeapon( pWeapon );
	if( pZone )
	{		
		ioBaseChar *pChar = pZone->GetOwner();
		if( pChar )
		{
			D3DXVECTOR3 vDir = pChar->GetTargetRot() * D3DXVECTOR3( 0.0f, 0.0f, -FLOAT1 );
			pZone->SetCollisionDir( vDir );
			pZone->SetZoneValue( false, false );

			if( !m_TargetList.empty() )
			{
				if( ( m_TargetList.end() != m_CurFireIter ) )
				{	
					ioBaseChar* pTarget = pStage->GetBaseChar( (*m_CurFireIter).second.m_szTargetName );
					if( pTarget )
					{
						pZone->SetStartPosition( pTarget->GetWorldPosition(), pStage );
						m_CurFireIter++;
					}
				}
			}
		}
	}
}

void ioTargetDebuffSkill::ApplyExtraSkillInfo( ioBaseChar *pChar, ioPlayStage *pStage, SP2Packet &rkPacket )
{
	
	DWORD dwCurTime = FRAMEGETTIME();
	int iType;
	rkPacket >> iType;


	switch( iType )
	{
	case SSC_UPDATE_TARGETLIST:
		{
			int iAddUserSize = 0;
			int iRemoveUserSize = 0;
			int iRemoveNonUserSize = 0;

			rkPacket >> iAddUserSize;
			if( 0 < iAddUserSize )
			{
				for( int i = 0; i <iAddUserSize; ++i )
				{
					ioHashString szCharName;
					rkPacket >> szCharName;

					ioBaseChar* pTarget = pStage->GetBaseChar( szCharName );
					if( pTarget )
						AddTarget( pStage, pChar, pTarget, dwCurTime, m_bFireMaker, true );
				}
			}

			rkPacket >> iRemoveUserSize;
			if( 0 < iRemoveUserSize )
			{
				for( int i = 0; i < iRemoveUserSize; ++i ) 
				{
					ioHashString szCharName;
					rkPacket >> szCharName;

					ioBaseChar* pTarget = pStage->GetBaseChar( szCharName );
					if( pTarget )
						RemoveTarget( pChar, pTarget, true );
				}
			}

			rkPacket >> iRemoveNonUserSize;
			if( 0 < iRemoveNonUserSize )
			{
				for( int i = 0; i < iRemoveNonUserSize; ++i ) 
				{
					ioHashString szCharName;
					rkPacket >> szCharName;					
					RemoveTarget( pChar, szCharName );
				}
			}
		}
		break;
	case SSC_ZERO_AIM_FIRE:
		{
			SetZeroAimFireState( pChar, pStage );
		}
		break;
	case SSC_AIM_FIRE:
		{
			SetAimFireState( pChar, pStage );
		}
		break;
	}
}


bool ioTargetDebuffSkill::CheckUseItemMesh()
{
	return m_bUseItemMesh;
}

bool ioTargetDebuffSkill::IsProtected( int iDefenseBreakType ) const
{
	if( m_bDisableSkillProtect )
		return false;

	if( DBT_NONE != (DefenseBreakType)m_ProtectCancelType )
	{
		if( iDefenseBreakType == m_ProtectCancelType )   // ��ų ���� �ı�
			return false;
	}

	if( IsPreDelayProtect() )
	{
		if( m_dwPreDelayStartTime != 0 )
			return true;
	}

	DWORD dwCurTime = FRAMEGETTIME();
	
	switch( m_SkillState )
	{
		case SS_LOOP:
			{
				if( 0 < m_dwStartSkillProtectedTime && 
					dwCurTime < m_dwStartSkillProtectedTime + m_dwSkillProtectedTime )
					return true;
			}
			break;			
		case SS_ZERO_AIM_FIRE:
		case SS_AIM_FIRE:				
			if( 0 < m_dwStartSkillProtectedTime && 
				dwCurTime < m_dwStartSkillProtectedTime + m_dwSkillProtectedTime )
				return true;
			else
				if( 0 < m_dwFireMotionEndTime && dwCurTime <= m_dwFireMotionEndTime )
					return true;

	}
	
	return false;
}

bool ioTargetDebuffSkill::IsSkillMotionEnd( DWORD dwActionStop, ioBaseChar* pOwner ) const
{
	DWORD dwCurTime = FRAMEGETTIME();

	if( m_dwPreDelayStartTime > 0 )
		return false;

	switch( m_SkillState )
	{
	case SS_PRE:
	case SS_LOOP:
	case SS_ZERO_AIM_FIRE:
	case SS_AIM_FIRE:
		return false;
	case SS_END:
		return true;
	}

	return true;
}
