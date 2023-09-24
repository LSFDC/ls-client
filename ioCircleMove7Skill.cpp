
#include "stdafx.h"

#include "ioPlayStage.h"
#include "ioBaseChar.h"

#include "ioCircleMove7Skill.h"

ioCircleMove7Skill::ioCircleMove7Skill()
{
	m_dwMotionEndTime = 0;
	m_dwCheckLoopTime = 0;
	m_SkillState = SS_None;
	m_dwLoopTime = 0;
	m_fCircleMoveSpeed = 0.0f;
	m_fCircleRange = 0.0f;
}

ioCircleMove7Skill::ioCircleMove7Skill( const ioCircleMove7Skill &rhs )
	: ioNormalSkill( rhs ),
	  m_ReadyCircle( rhs.m_ReadyCircle ),
	  m_fCircleRange( rhs.m_fCircleRange ),
	  m_fCircleMoveSpeed( rhs.m_fCircleMoveSpeed ),
	  m_fCircleHeightGap( rhs.m_fCircleHeightGap ),
	  m_fCircleOffSet( rhs.m_fCircleOffSet ),
	  m_szLoopAni( rhs.m_szLoopAni ),
	  m_dwLoopTime( rhs.m_dwLoopTime ),
	  m_DropZoneOffset( rhs.m_DropZoneOffset ),
	  m_vCreateBuffList( rhs.m_vCreateBuffList ),
	  m_vRemoveBuffList( rhs.m_vRemoveBuffList ),
	  m_dwSkillProtectTime( rhs.m_dwSkillProtectTime ),
	  m_bIsCanDown( rhs.m_bIsCanDown ),
	  m_bIsCanJump( rhs.m_bIsCanJump ),
	  m_bIsCanJumpOnBlow( rhs.m_bIsCanJumpOnBlow ),
	  m_bIsCanWound( rhs.m_bIsCanWound ),
	  m_bIsCanBlowWound( rhs.m_bIsCanBlowWound ),
	  m_AttackInfo( rhs.m_AttackInfo )
{
	m_dwMotionEndTime = 0;
	m_dwCheckLoopTime = 0;
	m_SkillState = SS_None;
}

ioCircleMove7Skill::~ioCircleMove7Skill()
{
}

void ioCircleMove7Skill::LoadProperty( ioINILoader &rkLoader )
{
	ioNormalSkill::LoadProperty( rkLoader );

	//���� �б�
	LoadBuffInfo( rkLoader );
		
	char szBuf[MAX_PATH];

	//������ ����
	rkLoader.LoadString_e( "ready_circle", "", szBuf, MAX_PATH );
	m_ReadyCircle = szBuf;
	m_fCircleRange = rkLoader.LoadFloat_e( "magic_circle_range", 0.0f );
	m_fCircleMoveSpeed = rkLoader.LoadFloat_e( "magic_circle_move_speed", 0.0f );
	m_fCircleHeightGap = rkLoader.LoadFloat_e( "magic_circle_height_gap", 0.0f );
	m_fCircleOffSet = rkLoader.LoadFloat_e( "magic_circle_offset", 0.0f );
		
	//���� �ִ� ����
	rkLoader.LoadString_e( "loop_ani", "", szBuf, MAX_PATH );
	m_szLoopAni = szBuf;
	m_dwLoopTime = rkLoader.LoadInt_e( "loop_time", 0 );
	
	//����� ������ ����
	m_DropZoneOffset = rkLoader.LoadFloat_e( "drop_zone_offset", 0.0f );

	m_dwSkillProtectTime = rkLoader.LoadInt_e( "skill_protect_time", 0 );

	m_bIsCanDown = rkLoader.LoadBool_e( "can_down", false );
	m_bIsCanJump = rkLoader.LoadBool_e( "can_jump", false );
	m_bIsCanJumpOnBlow = rkLoader.LoadBool_e( "can_jump_blow", false );
	m_bIsCanWound = rkLoader.LoadBool_e( "can_wound", false );
	m_bIsCanBlowWound = rkLoader.LoadBool_e( "can_blow_wound", false );

	LoadAttackAttribute_e( "attack_info", m_AttackInfo, rkLoader );
}

ioSkill* ioCircleMove7Skill::Clone()
{
	return new ioCircleMove7Skill( *this );
}

//////////////////////////////////////////////////////////////////////////////////////

bool ioCircleMove7Skill::OnSkillStart( ioBaseChar *pChar )
{
	if( !ioNormalSkill::OnSkillStart(pChar) )
		return false;
	
	if( m_bUseItemMesh )
		pChar->ShowWeaponItemMesh( false );

	if( !m_bSetHandMesh && m_bUseItemMesh && !m_HandMesh.IsEmpty() )
	{
		pChar->ChangeSkillWeaponMesh( this, m_HandMesh, m_bVisibleEquipMesh );
		m_bSetHandMesh = true;
	}

	m_dwCheckLoopTime = 0;
	m_SkillState = SS_None;
	m_dwCurSkillProtectTime = 0;

	if( m_dwSkillProtectTime > 0 )
		m_dwCurSkillProtectTime = FRAMEGETTIME() + m_dwSkillProtectTime;
	
	return true;
}

void ioCircleMove7Skill::OnProcessState( ioBaseChar *pChar, ioPlayStage *pStage )
{
	DWORD dwCurTime = FRAMEGETTIME();

	switch( m_SkillState )
	{
	case SS_None:
		{
			DWORD dwPreDelay = GetPreDelayTime();
			if( m_dwPreDelayStartTime + dwPreDelay < dwCurTime )
			{
				m_dwPreDelayStartTime = 0;
				EndPreDelayEffect( pChar );

				SetActionState( pChar );
			}
		}
		break;
	case SS_Action:
		{
			if ( m_dwMotionEndTime < dwCurTime )
			{
				SetLoopState( pChar );
			}
		}
		break;
	case SS_Loop:
		{
			if ( m_dwCheckLoopTime + m_dwLoopTime < dwCurTime )
			{
				SetTeleportState( pChar );
				return;
			}

			UpdateMagicCircle( pChar );
			CheckRotate( pChar );
			CheckInputKey( pChar );
		}
		break;
	case SS_Teleport:
		{
			SetAttackState( pChar );
		}
		break;
	case SS_Attack:
		{
			if ( m_dwMotionEndTime < dwCurTime )
			{
				SetEndState( pChar );
			}
		}
		break;
	}
}

void ioCircleMove7Skill::OnSkillEnd( ioBaseChar *pChar )
{
	ioNormalSkill::OnSkillEnd( pChar );

	pChar->SetSpecialHide( false );

	//������ ����
	pChar->SetMagicCircleTarget( "" );
	pChar->DestroyMagicCircle();

	pChar->SetCurJumpPowerAmt( 0.0f );
	pChar->SetGravityAmt( 0.0f );

	//���� ����
	RemoveBuff( pChar );

	if( m_bUseItemMesh )
		pChar->ShowWeaponItemMesh( true );

	if( !m_HandMesh.IsEmpty() )
		pChar->RestoreSkillWeaponMesh( this, m_HandMesh, m_bVisibleEquipMesh );
}

void ioCircleMove7Skill::ApplyExtraSkillInfo( ioBaseChar *pChar, ioPlayStage *pStage, SP2Packet &rkPacket )
{
	if( !pChar )
		return;

	int nState;
	rkPacket >> nState;

	switch ( (SkillState)nState )
	{
	case SS_Teleport:
		{
			D3DXVECTOR3 vTeleportPos;
			rkPacket >> vTeleportPos;
			
			pChar->SetWorldPosition( vTeleportPos );
			m_SkillState = SS_Teleport;
		}
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////

bool ioCircleMove7Skill::IsNoDropState() const
{
	switch( m_SkillState )
	{
	case SS_Action:
	case SS_Loop:
	case SS_Teleport:
	case SS_Attack:
		return true;
	}
	return false;
}

bool ioCircleMove7Skill::IsProtected( int iDefenseBreakType ) const
{
	if( IsPreDelayProtect() )
	{
		if( m_dwPreDelayStartTime != 0 )
			return true;
	}

	DWORD dwCurTime = FRAMEGETTIME();
	if( m_dwCurSkillProtectTime > dwCurTime )
		return true;

	return false;
}

bool ioCircleMove7Skill::IsSkillMotionEnd( DWORD dwActionStop, ioBaseChar* pOwner ) const
{
	if ( m_SkillState == SS_End )
	{
		if( m_fSkillEndNextJumpAmt == 0.0f )
			return true;
		else
			return false;
	}
	return false;
}

bool ioCircleMove7Skill::IsEndState() const
{
	if ( m_SkillState == SS_End )
	{
		return true;
	}
	return false;
}

bool ioCircleMove7Skill::IsAttackEndState() const
{
	if( m_SkillState == SS_End )
		return true;

	return false;
}

//////////////////////////////////////////////////////////////////////////////////////
//��ų ��� ���� ���
bool ioCircleMove7Skill::IsCanDownSkill() const
{
	return m_bIsCanDown;
}

bool ioCircleMove7Skill::IsCanJumpingSkill() const
{
	return m_bIsCanJump;
}

bool ioCircleMove7Skill::IsCanJumpingSkillOnBlowWound() const
{
	return m_bIsCanJumpOnBlow;
}

bool ioCircleMove7Skill::IsCanWoundSkill() const
{
	return m_bIsCanWound;
}

bool ioCircleMove7Skill::IsCanBlowWoundSkill() const
{
	return m_bIsCanBlowWound;
}

//////////////////////////////////////////////////////////////////////////////////////
//���� �б�
void ioCircleMove7Skill::LoadBuffInfo( ioINILoader &rkLoader )
{
	char szBuf[MAX_PATH], szKey[MAX_PATH];

	int nCreateCnt = rkLoader.LoadInt_e( "owner_buff_cnt", 0 );
	m_vCreateBuffList.clear();
	for ( int i=0; i<nCreateCnt; i++ )
	{
		wsprintf_e( szKey, "owner_buff%d_name", i+1 );
		rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );
		m_vCreateBuffList.push_back( szBuf );
	}
	
	int nRemoveCnt = rkLoader.LoadInt_e( "remove_buff_cnt", 0 );
	m_vRemoveBuffList.clear();
	for ( int i=0; i<nRemoveCnt; i++ )
	{
		wsprintf_e( szKey, "remove_buff%d_name", i+1 );
		rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );
		m_vRemoveBuffList.push_back( szBuf );
	}
}

//////////////////////////////////////////////////////////////////////////////////////
//�׼� ���� ����
void ioCircleMove7Skill::SetActionState( ioBaseChar *pChar )
{
	if ( !pChar )
		return;

	ioEntityGroup *pGrp = pChar->GetGroup();

	pGrp->ClearAllLoopAni( 100.0f, true );
	pGrp->ClearAllActionAni( 100.0f, true );

	ioHashString szAniName = GetSkillMotionName();
	float fTimeRate = GetSkillMotionRate();

	DWORD dwCurTime = FRAMEGETTIME();
	int iAniID = pGrp->GetAnimationIdx( szAniName );
	if ( iAniID != -1 )
	{
		pGrp->SetActionAni( iAniID, 100.0f, 100.0f, 1.0f, 1.0f/fTimeRate );
		m_dwMotionEndTime = dwCurTime + pGrp->GetAnimationFullTime( iAniID ) * fTimeRate;
	}

	pChar->ReduceNeedGaugeBySkill( this, GetNeedGauge( pChar->GetCharName() ) );
	m_bReduceGauge = true;

	m_SkillState = SS_Action;
}

//���� ���� ����
void ioCircleMove7Skill::SetLoopState( ioBaseChar *pChar )
{
	if ( !pChar )
		return;
	
	ioEntityGroup *pGrp = pChar->GetGroup();

	pGrp->ClearAllLoopAni( 100.0f, true );
	pGrp->ClearAllActionAni( 100.0f, true );

	int iAniID = pGrp->GetAnimationIdx( m_szLoopAni );
	if( iAniID != -1 )
		pGrp->SetLoopAni( iAniID, 100.0f );	

	CreateMagicCircle( pChar );
	CreateBuff( pChar );
	pChar->SetSpecialHide( true );

	DWORD dwCurTime = FRAMEGETTIME();
	m_dwCheckLoopTime = dwCurTime;
	m_SkillState = SS_Loop;
}

//�ڷ���Ʈ ���� ����
void ioCircleMove7Skill::SetTeleportState( ioBaseChar *pChar )
{
	if ( !pChar || !pChar->IsNeedProcess() )
		return;

	ioPlayStage *pStage = pChar->GetCreator();
	if ( !pStage )
		return;
	
	GetMagicCirclePos( pChar );

	D3DXVECTOR3 TeleportPos = m_vCirclePos;

	float fMapHeight = pStage->GetMapHeightOnlyTerrain( TeleportPos.x, TeleportPos.z );
	
	if ( fMapHeight <= 0.0f )
		TeleportPos.y = m_vCirclePos.y + m_DropZoneOffset;
	
	pChar->SetWorldPosition( TeleportPos );
	pChar->SetSpecialHide( false );

	//������ ����
	pChar->SetMagicCircleTarget( "" );
	pChar->DestroyMagicCircle();

	pChar->SetCurJumpPowerAmt( 0.0f );
	pChar->SetGravityAmt( 0.0f );

	//���� ����
	RemoveBuff( pChar );

	m_SkillState = SS_Teleport;

	//��Ŷ ����
	if( pChar->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_SKILL_EXTRA_INFO );
		kPacket << pChar->GetCharName();
		kPacket << GetName();
		kPacket << SS_Teleport;
		kPacket << TeleportPos;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

void ioCircleMove7Skill::SetAttackState( ioBaseChar *pChar )
{
	if ( !pChar )
		return;

	ioEntityGroup *pGrp = pChar->GetGroup();
	int iAniID = pGrp->GetAnimationIdx( m_AttackInfo.m_AttackAnimation );
	if ( iAniID == -1 )
		return;

	float fTimeRate  = m_AttackInfo.m_fAttackAniRate;
	DWORD dwPreDelay = m_AttackInfo.m_dwPreDelay;

	pGrp->ClearAllLoopAni( 100.0f, true );
	pGrp->ClearAllActionAni( 100.0f, true );
	pGrp->SetActionAni( iAniID, 100.0f, 100.0f, 1.0f, 1.0f/fTimeRate, 0.0f, dwPreDelay );	

	pChar->ClearReservedSliding();
	pChar->SetReservedSliding( m_AttackInfo.m_vForceInfoList, iAniID, fTimeRate, dwPreDelay );

	pChar->RefreshFireTimeList( iAniID, m_AttackInfo.m_vWeaponInfoList, FTT_SKILL_ATTACK, fTimeRate, dwPreDelay );

	DWORD dwCurTime = FRAMEGETTIME();
	m_dwMotionEndTime = dwCurTime + pGrp->GetAnimationFullTime( iAniID ) * fTimeRate;
		
	pChar->SetAutoTarget( ATT_SKILL );

	DWORD dwTrackingTime = dwCurTime + pGrp->GetAnimationEventTime_e( iAniID, "tracking_e" ) * fTimeRate + dwPreDelay;
	pChar->SetAutoTargetTrackingEndTime( dwTrackingTime );

	m_SkillState = SS_Attack;
}

//���� ���� ����
void ioCircleMove7Skill::SetEndState( ioBaseChar *pChar )
{
	if ( !pChar )
		return;

	m_SkillState = SS_End;
}

//////////////////////////////////////////////////////////////////////////////////////
//������ ����
void ioCircleMove7Skill::CreateMagicCircle( ioBaseChar *pChar )
{
	if ( !pChar )
		return;

	if( !pChar->CheckMagicCircle() )
		pChar->CreateMagicCircle( m_ReadyCircle, m_ReadyCircle, m_ReadyCircle, m_fCircleOffSet );
}

//������ ������Ʈ
void ioCircleMove7Skill::UpdateMagicCircle( ioBaseChar *pChar )
{
	if ( !pChar )
		return;

	if( pChar->CheckMagicCircle() )
	{
		//���� üũ
		pChar->CheckChangeDirectionForMagicCircle();
		//��ġ ����
		pChar->UpdateMagicCirclePosAll( m_fCircleMoveSpeed, m_fCircleRange, m_fCircleHeightGap, false );
	}
}

//������ ��ġ ���
void ioCircleMove7Skill::GetMagicCirclePos( ioBaseChar *pChar )
{
	if ( !pChar )
		return;

	if( pChar->CheckMagicCircle() )
	{
		pChar->CheckMagicCircleVisible( false );
		pChar->UpdateMagicCirclePos( m_fCircleMoveSpeed, m_fCircleRange, m_fCircleHeightGap, true, true );
	}

	m_vCirclePos = pChar->GetMagicCirclePos( m_fCircleOffSet, m_bCircleNoChangeY );
}

//������ �������� ȸ��
void ioCircleMove7Skill::CheckRotate( ioBaseChar *pChar )
{
	if ( !pChar )
		return;

	//������ �������� ĳ���� ȸ��
	D3DXVECTOR3 vTargetPos = pChar->GetMagicCirclePos();
	pChar->SetTargetRotToTargetPos( vTargetPos, true, false );
}

//Ű�Է� Ȯ��
void ioCircleMove7Skill::CheckInputKey( ioBaseChar *pChar )
{
	if ( !pChar || !pChar->IsNeedProcess() )
		return;	

	if( pChar->IsAttackKey() )
	{
		SetTeleportState( pChar );
	}
}

//////////////////////////////////////////////////////////////////////////////////////
//���� ����
void ioCircleMove7Skill::CreateBuff( ioBaseChar *pChar )
{
	if ( !pChar || m_vCreateBuffList.empty() )
		return;

	int nCreateCnt = (int)m_vCreateBuffList.size();
	for ( int i=0; i<nCreateCnt; i++ )
	{
		//�ش� �̸��� ������ ������
		if ( !pChar->HasBuff( m_vCreateBuffList[i] ) )
			pChar->AddNewBuff( m_vCreateBuffList[i], pChar->GetCharName(), "", NULL, false );
	}
}

//���� ����
void ioCircleMove7Skill::RemoveBuff( ioBaseChar *pChar )
{
	if ( !pChar || m_vRemoveBuffList.empty() )
		return;

	int nRemoveCnt = (int)m_vRemoveBuffList.size();
	for ( int i=0; i<nRemoveCnt; i++ )
	{
		//�ش� �̸��� ������ ������
		if ( pChar->HasBuff( m_vRemoveBuffList[i] ) )
			pChar->RemoveBuff( m_vRemoveBuffList[i], true );
	}
}