#include "StdAfx.h"
#include "ioAreaCheckBuffSkill.h"

#include "ioPlayStage.h"
#include "ioBaseChar.h"

//�⺻ ������
ioAreaCheckBuffSkill::ioAreaCheckBuffSkill()
{
	m_SkillState = SS_NONE;
	m_bSetHandMesh = false;

	m_dwCurSkillProtectTime = 0;

	m_bSetAttackFire = false;

	m_dwAreaFireTime = 0;

	m_bIsBuffActionInit = false;
}

//���� ������ ( Clone ���� ����)
ioAreaCheckBuffSkill::ioAreaCheckBuffSkill( const ioAreaCheckBuffSkill &rhs )
	: ioAttackSkill( rhs ),
	m_vBuffAction( rhs.m_vBuffAction ),
	m_dwSkillProtectTime( rhs.m_dwSkillProtectTime ),
	m_bIsCreateAreaWeapon( rhs.m_bIsCreateAreaWeapon ),
	m_AreaWeaponPos( rhs.m_AreaWeaponPos ),
	m_szAreaWeaponName( rhs.m_szAreaWeaponName ),
	m_szBuffActionEffect( rhs.m_szBuffActionEffect ),
	m_szCheckBuff( rhs.m_szCheckBuff ),
	m_EnemyBuffList( rhs.m_EnemyBuffList ),
	m_OwnerBuffList( rhs.m_OwnerBuffList ),
	m_TargetColType( rhs.m_TargetColType ),
	m_TargetWoundType( rhs.m_TargetWoundType ),
	m_fColRange( rhs.m_fColRange ),
	m_fColAngle( rhs.m_fColAngle ),
	m_fColUpHeight( rhs.m_fColUpHeight ),
	m_fColUnderHeight( rhs.m_fColUnderHeight ),
	m_fColWidth( rhs.m_fColWidth ),
	m_fColOffSet(rhs.m_fColOffSet),
	m_bSetReserveTime( rhs.m_bSetReserveTime ),
	m_dwDelayInputTime( rhs.m_dwDelayInputTime )
{	
	m_SkillState = SS_NONE;
	m_bSetHandMesh = false;

	m_dwCurSkillProtectTime = 0;

	m_bSetAttackFire = false;	

	m_dwAreaFireTime = 0;

	m_bIsBuffActionInit = false;

	m_bSetEnemyBuff = false;

	m_dwCheckInputTime = 0;
}

//�Ҹ���
ioAreaCheckBuffSkill::~ioAreaCheckBuffSkill()
{	
}

//ini ���� �б�
void ioAreaCheckBuffSkill::LoadProperty( ioINILoader &rkLoader )
{
	ioAttackSkill::LoadProperty( rkLoader );

	m_dwSkillProtectTime = (DWORD)rkLoader.LoadInt_e( "skill_protect_time", 0 );

	//�߰� ������ �б�
	//Area ���⸦ �����Ұ��ΰ�
	m_bIsCreateAreaWeapon = rkLoader.LoadBool_e( "set_create_area_weapon", false );
	//Area ���� �̸� ���
	char szKey[MAX_PATH] = "";
	rkLoader.LoadString_e( "area_weapon_name", "", szKey, MAX_PATH );
	m_szAreaWeaponName = szKey;
	//Area ���� ��ġ
	m_AreaWeaponPos.x = rkLoader.LoadFloat_e( "area_weapon_pos_x", 0.0f );
	m_AreaWeaponPos.y = rkLoader.LoadFloat_e( "area_weapon_pos_y", 0.0f );
	m_AreaWeaponPos.z = rkLoader.LoadFloat_e( "area_weapon_pos_z", 0.0f );

	//���� �׼� ����
	LoadBuffActionProperty( rkLoader );
	//Ÿ�� �浹 ����
	LoadTargetInfoProperty( rkLoader );
	//���� ���� ����
	LoadBuffInfoProperty( rkLoader );
}

//���� �׼� ���� ����
void ioAreaCheckBuffSkill::LoadBuffActionProperty( ioINILoader &rkLoader )
{
	char szKey[MAX_PATH] = "";

	wsprintf_e( szKey, "buff_action" );
	LoadAttackAttribute( szKey, m_vBuffAction, rkLoader );

	rkLoader.LoadString_e( "buff_action_effect", "", szKey, MAX_PATH );
	m_szBuffActionEffect = szKey;
	
	m_bSetReserveTime = rkLoader.LoadBool_e( "set_reserve_time", true );

	m_dwDelayInputTime = rkLoader.LoadInt_e( "delay_input_time", 0 );	
}

//�浹 ���� ����
void ioAreaCheckBuffSkill::LoadTargetInfoProperty( ioINILoader &rkLoader )
{
	m_TargetColType = (TargetColType)rkLoader.LoadInt_e( "target_collision_type", TCT_SPHERE );
	m_TargetWoundType = (TargetWoundType)rkLoader.LoadInt_e( "target_wound_type", TWT_ALL );

	m_fColRange = rkLoader.LoadFloat_e( "target_range", 0.0f );
	m_fColAngle = rkLoader.LoadFloat_e( "target_angle", 0.0f );
	m_fColOffSet = rkLoader.LoadFloat_e( "target_offset", 0.0f );

	m_fColUpHeight = rkLoader.LoadFloat_e( "target_up_height", 0.0f );
	m_fColUpHeight = max( FLOAT1, m_fColUpHeight );
	m_fColUnderHeight = rkLoader.LoadFloat_e( "target_under_height", 0.0f );
	m_fColUnderHeight = max( FLOAT1, m_fColUnderHeight );

	m_fColWidth = rkLoader.LoadFloat_e( "target_width", 0.0f );
}

//���� ���� ����
void ioAreaCheckBuffSkill::LoadBuffInfoProperty( ioINILoader &rkLoader )
{
	char szTitle[MAX_PATH], szBuf[MAX_PATH];

	wsprintf_e( szTitle, "check_buff" );
	rkLoader.LoadString( szTitle, "", szBuf, MAX_PATH );
	m_szCheckBuff = szBuf;

	ioBuffInfo tBuffInfo;
	m_EnemyBuffList.clear();
	m_OwnerBuffList.clear();

	int iCount = rkLoader.LoadInt_e( "owner_buff_cnt", 0 );

	for( int i=0; i<iCount; ++i )
	{
		wsprintf_e( szTitle, "owner_buff%d", i+1 );
		rkLoader.LoadString( szTitle, "", szBuf, MAX_PATH );

		tBuffInfo.m_BuffName = szBuf;

		if( !tBuffInfo.m_BuffName.IsEmpty() )
		{
			tBuffInfo.m_iBuffType = g_BuffMaker.GetBuffType( tBuffInfo.m_BuffName );

			if( tBuffInfo.m_iBuffType != BT_NONE )
			{
				m_OwnerBuffList.push_back( tBuffInfo );
			}
		}
	}
	
	iCount = rkLoader.LoadInt_e( "enemy_buff_cnt", 0 );

	for( int i=0; i<iCount; ++i )
	{
		wsprintf_e( szTitle, "enemy_buff%d", i+1 );
		rkLoader.LoadString( szTitle, "", szBuf, MAX_PATH );

		tBuffInfo.m_BuffName = szBuf;

		if( !tBuffInfo.m_BuffName.IsEmpty() )
		{
			tBuffInfo.m_iBuffType = g_BuffMaker.GetBuffType( tBuffInfo.m_BuffName );

			if( tBuffInfo.m_iBuffType != BT_NONE )
			{
				m_EnemyBuffList.push_back( tBuffInfo );
			}
		}
	}	
}

//��ų ���� ���� �� ��ȯ
ioSkill* ioAreaCheckBuffSkill::Clone()
{
	return new ioAreaCheckBuffSkill( *this );
}

//��ų ���� �κ� ����
bool ioAreaCheckBuffSkill::OnSkillStart( ioBaseChar *pChar )
{		
	m_dwEnableReserveTime = 0;
	m_dwFireMotionEndTime = 0;

	m_SkillState = SS_NONE;

	InitExtraAniJump();

	m_bSetAttackFire = false;

	if( !ioAttackSkill::OnSkillStart( pChar ) )
		return false;
	// �⺻ ���� �տ��� ����
	
	if( m_bUseItemMesh )
	{
		pChar->ShowWeaponItemMesh( false );
	}

	if( !m_bSetHandMesh && m_bUseItemMesh && !m_HandMesh.IsEmpty() )
	{
		pChar->ChangeSkillWeaponMesh( this, m_HandMesh, m_bVisibleEquipMesh );
		m_bSetHandMesh = true;
	}	

	if( !m_SkillSoundName.IsEmpty() && pChar )
	{
		ioEntityGroup *pGrp = pChar->GetGroup();
		m_SkillSoundID = g_SoundMgr.PlaySound( m_SkillSoundName, pGrp->GetParentSceneNode() );
	}

	m_dwCurSkillProtectTime = 0;
	if( m_dwSkillProtectTime > 0 )
		m_dwCurSkillProtectTime = FRAMEGETTIME() + m_dwSkillProtectTime;
		

	m_bIsBuffActionInit = false;

	m_dwAreaFireTime = 0;

	if ( m_bIsCreateAreaWeapon )
	{
		m_bRefreshFireTimeList = false;
	}

	m_bSetEnemyBuff = false;

	m_dwCheckInputTime = 0;

	//�ڽſ��� �����ϴ� �����߰�
	for (int i=0; i < (int)m_OwnerBuffList.size(); ++i)
	{
		ioBuffInfo pOwnerBuff = m_OwnerBuffList[i];
		pChar->AddNewBuff( pOwnerBuff.m_BuffName, 
						   pChar->GetCharName(),
						   GetOwnerItem()->GetName(),
						   this);					
	}

	return true;
}

//��ų �ߵ� �� ���� (Loop ����)
void ioAreaCheckBuffSkill::OnProcessState( ioBaseChar *pChar, ioPlayStage *pStage )
{	
	DWORD dwCurTime = FRAMEGETTIME();

	switch( m_SkillState )
	{
	case SS_NONE:
		{
			DWORD dwPreDelay = GetPreDelayTime();
			if( m_dwPreDelayStartTime + dwPreDelay < dwCurTime )
			{
				m_dwPreDelayStartTime = 0;

				AttackSkillFire( pChar );
				EndPreDelayEffect( pChar );
			}
		}
		break;
	case SS_ACTION:
		{
			CheckExtraAniJump( pChar );
			CheckAniRotate( pChar );
			CheckCreateAreaWeapon( pChar );			
		}
		break;	
	case SS_CHECKBUFF:
		{			
			CheckBuff( pChar );			
		}
		break;
	}
		
	//��ų ���°� ����Ȯ�� ����� ������ �ȴٸ� ���� ���·� ����
	if ( (m_bIsBuffActionInit && m_dwMotionEndTime <= dwCurTime ) || 
		 (m_SkillState == SS_CHECKBUFF && m_dwCheckInputTime < dwCurTime) )
	{
		//���� ���·� ����
		m_SkillState = SS_END;
	}
}

//��ų ���� �κ� ����
void ioAreaCheckBuffSkill::OnSkillEnd( ioBaseChar *pChar )
{
	//�ڽſ��� �����ϴ� ������ ����
	for (int i=0; i < (int)m_OwnerBuffList.size(); ++i)
	{
		ioBuffInfo pOwnerBuff = m_OwnerBuffList[i];
		pChar->RemoveBuff( pOwnerBuff.m_BuffName, false);			
	}

	ioAttackSkill::OnSkillEnd( pChar );

	m_bSetAttackFire = false;

	pChar->EndEffect( m_szBuffActionEffect );

	// �⺻���� ����
	if( m_bUseItemMesh )
	{
		pChar->ShowWeaponItemMesh( true );
	}

	if( !m_HandMesh.IsEmpty() )
	{
		pChar->RestoreSkillWeaponMesh( this, m_HandMesh, m_bVisibleEquipMesh );
	}

	if( !m_SkillSoundName.IsEmpty() )
	{
		g_SoundMgr.StopSound( m_SkillSoundName, m_SkillSoundID );
	}
}

//�޽� ���� ����
bool ioAreaCheckBuffSkill::CheckUseItemMesh()
{
	return m_bUseItemMesh;
}

//���� �Ӽ� ����
void ioAreaCheckBuffSkill::ApplyAttributeToWeapon( ioWeapon *pWeapon, ioPlayStage *pStage )
{
	ioSkill::ApplyAttributeToWeapon( pWeapon, pStage );
}

//��ų ���� �߰� ���� (��Ŷ�� �޾� ĳ������ ���¸� ó��, �̵�, ȸ��, Ư�� ��� ����ȭ)
void ioAreaCheckBuffSkill::ApplyExtraSkillInfo( ioBaseChar *pChar, ioPlayStage *pStage, SP2Packet &rkPacket )
{	
	if( !pChar )
		return;

	int iState;
	rkPacket >> iState;

	switch( iState )
	{		
	case SS_CHECKHASBUFF:
		{
			//���� �׼� �ִϸ��̼� �ʱ�ȭ
			if ( !m_bIsBuffActionInit )
			{
				//�ִϸ��̼� ����
				SetCheckBuffAnimation( pChar );

				m_bIsBuffActionInit = true;
			}
			//Ÿ�ٿ� ���� ǥ�� ���� Ȯ��
			CheckTargetHasBuff( pChar );
		}
		break;
	}	
}

//��ų ���� ����� �����Ѱ�
bool ioAreaCheckBuffSkill::IsSkillMotionEnd( DWORD dwActionStop, ioBaseChar* pOwner ) const
{
	//���� Ȯ�� ���¿����� ���� ó���� ���� ��	
	if ( m_SkillState == SS_CHECKBUFF )
		return false;

	if( m_dwPreDelayStartTime > 0 )
		return false;

	if( m_dwMotionEndTime + dwActionStop >= FRAMEGETTIME() )
		return false;

	return true;
}

//��ų ���� ���� ���¿� ���� ó��
bool ioAreaCheckBuffSkill::IsProtected( int iDefenseBreakType ) const
{	
	if( m_bDisableSkillProtect )
		return false;

	if( DBT_NONE != (DefenseBreakType)m_ProtectCancelType )
	{
		if( iDefenseBreakType == m_ProtectCancelType )   // ��ų ���� �ı�
			return false;
	}

	DWORD dwCurTime = FRAMEGETTIME();

	if( IsPreDelayProtect() )
	{
		if( m_dwPreDelayStartTime != 0 )
			return true;
	}

	if( m_dwPreDelayStartTime == 0 )
	{		
		if( m_dwCurSkillProtectTime > dwCurTime )
			return true;
	}

	return false;
}
//���� �����Ѱ�
bool ioAreaCheckBuffSkill::IsEnableReserve() const
{
	if ( m_SkillState == SS_CHECKBUFF )
		return false;

	if( m_dwPreDelayStartTime != 0 )
		return false;

	if( m_dwEnableReserveTime == 0 || m_dwMotionStartTime >= m_dwEnableReserveTime )
		return false;

	if( m_dwEnableReserveTime < FRAMEGETTIME() )
		return true;

	return false;
}

//��ų�� ���� �����ΰ�
bool ioAreaCheckBuffSkill::IsJumpState() const
{	
	return false;
}

//��ų�� ���ִ� �����ΰ�
bool ioAreaCheckBuffSkill::IsNoDropState() const
{	
	return false;
}

//��Ŷ (��ġ�� ����) �� ���� �� �ִ� �����ΰ�
bool ioAreaCheckBuffSkill::IsCanSendControl() const
{	
	if( m_bSetAttackFire )
		return false;

	return true;
}

//�뽬 Ȯ���� �����Ѱ�
bool ioAreaCheckBuffSkill::IsEnableDashCheck( ioBaseChar *pChar )
{	
	switch( m_SkillState )
	{
	case SS_ACTION:
		return true;
	}

	return false;
}

//��ų ���� �����ΰ�
bool ioAreaCheckBuffSkill::IsEndState() const
{
	if( m_SkillState == SS_END )
		return true;

	return false;
}

//���� �Ϸ� ���� (���� ��ų ��� �� ���� ����) 
bool ioAreaCheckBuffSkill::IsAttackEndState() const
{
	return IsEndState();
}

//���� ��ų �ߵ�
void ioAreaCheckBuffSkill::AttackSkillFire( ioBaseChar *pChar )
{
	//�⺻ ���� ���� �� �ִϸ��̼�
	ioAttackSkill::AttackSkillFire( pChar );	

	//Area ���⸦ �����Ѵٸ�
	if ( m_bIsCreateAreaWeapon )
	{
		//Area ���� ���� �ð��� ��� ����
		ioEntityGroup *pGrp = pChar->GetGroup();
		int iAniID = pGrp->GetAnimationIdx( GetAttribute().m_AttackAnimation );
		if( iAniID == -1 )	return;

		DWORD dwPreDelay = GetAttribute().m_dwPreDelay;

		DWORD dwCurTime = FRAMEGETTIME();
		m_dwAreaFireTime = dwCurTime;
		m_dwAreaFireTime += dwPreDelay;
		m_dwAreaFireTime += pGrp->GetAnimationEventTime( iAniID, "fire_s" ) * GetAttackTimeRate();
	}
			
	m_bSetAttackFire = true;
	m_SkillState = SS_ACTION;
}

//Area���� ���� Ȯ��
void ioAreaCheckBuffSkill::CheckCreateAreaWeapon( ioBaseChar *pChar )
{
	//���� ó��
	//ĳ���Ͱ� ���ų�, ���� ������ ���ϰų�, ���� �̸��� ���ų�
	if( !pChar || !m_bIsCreateAreaWeapon || m_szAreaWeaponName.IsEmpty() ) 
	{
		//������Ʈ ����
		m_SkillState = SS_CHECKBUFF;
		return;
	}

	//Area���Ⱑ ������ �ð��� �����ٸ� 
	if ( m_dwAreaFireTime <= FRAMEGETTIME() )
	{			
		//����, ��ġ
		D3DXVECTOR3 vPos = pChar->GetWorldPosition() + m_AreaWeaponPos;
		D3DXQUATERNION qtRot = pChar->GetTargetRot();

		//Area ���� ����
		g_AreaWeaponMgr.CreateAreaWeapon( pChar->GetCharName(), "", GetName(), m_szAreaWeaponName, vPos, qtRot, ioAreaWeapon::CT_NORMAL );

		//������Ʈ ����
		m_SkillState = SS_CHECKBUFF;

		if ( !m_bSetReserveTime )
		{
			m_dwCheckInputTime = FRAMEGETTIME() + m_dwDelayInputTime;
		}
	}
}

//���� Ȯ�� ������Ʈ
void ioAreaCheckBuffSkill::CheckBuff( ioBaseChar *pChar )
{
	//����ó��
	if ( !pChar || m_bSetEnemyBuff )
		return;
	
	DWORD dwCurTime = FRAMEGETTIME();
	
	//�Է� ���� �ð��� ���� �ð��� �Ѿ����鼭, ���� ���� ���¸�
	if( pChar->IsNeedProcess() && 
		( ( m_bSetReserveTime && m_dwEnableReserveTime <= dwCurTime ) || 
		  ( !m_bSetReserveTime && m_dwCheckInputTime >= FRAMEGETTIME() ) 
		))
	{				
		//���� Ű �Է� Ȯ�� ��
		if( pChar->IsAttackKey() )
		{
			//���� �׼� �ִϸ��̼� �ʱ�ȭ
			if ( !m_bIsBuffActionInit )
			{
				//�ִϸ��̼� ����
				SetCheckBuffAnimation( pChar );

				m_bIsBuffActionInit = true;
			}

			//ǥ�� ������ Ȯ��
			CheckTargetHasBuff( pChar );

			//��Ŷ�� ����
			if( pChar->IsNeedSendNetwork() )
			{
				SP2Packet kPacket( CUPK_SKILL_EXTRA_INFO );
				kPacket << pChar->GetCharName();
				kPacket << GetName();
				kPacket << SS_CHECKHASBUFF;
				P2PNetwork::SendToAllPlayingUser( kPacket );
			}
			//������ �����ߴ�.
			m_bSetEnemyBuff = true;
		}
	}	
}

//���� Ȯ�� �ִϸ��̼� ����
void ioAreaCheckBuffSkill::SetCheckBuffAnimation( ioBaseChar *pChar )
{
	ioEntityGroup *pGrp = pChar->GetGroup();
	int iAniID = pGrp->GetAnimationIdx( m_vBuffAction.m_AttackAnimation );
	if( iAniID == -1 )	return;

	pChar->InitActionStopDelay( true );

	pChar->AttachEffect( m_szBuffActionEffect );

	float fTimeRate  = m_vBuffAction.m_fAttackAniRate;
	DWORD dwPreDelay = m_vBuffAction.m_dwPreDelay;
	
	pGrp->ClearAllLoopAni( FLOAT100, true );
	pGrp->ClearAllActionAni( FLOAT100, true );

	DWORD dwCurTime = FRAMEGETTIME();
	m_dwMotionEndTime = dwCurTime;
	m_dwMotionEndTime += pGrp->GetAnimationFullTime( iAniID ) * fTimeRate;
	m_dwMotionEndTime += dwPreDelay + m_vBuffAction.m_dwEndDelay;
	
	float fAttackEnd = pGrp->GetAnimationEventTime( iAniID, "attack_end" ) * fTimeRate;
	
	if( fAttackEnd > 0.0f )
		m_dwAttackEndTime = dwCurTime + fAttackEnd;
	else
		m_dwAttackEndTime = 0;
	
	if( m_vBuffAction.m_AttackEndAnimation.IsEmpty() )
	{
		pGrp->SetActionAni( iAniID, FLOAT100, FLOAT100, FLOAT1, FLOAT1/fTimeRate, 0.0f, dwPreDelay, true );
	}
	else
	{
		pGrp->SetActionAni( iAniID, FLOAT100, 0.0f, FLOAT1, FLOAT1/fTimeRate, 0.0f, dwPreDelay );

		float fWaitTime = pGrp->GetAnimationFullTime( iAniID ) * fTimeRate + dwPreDelay;
		float fEndTimeRate = m_vBuffAction.m_fAttackEndAniRate;

		pGrp->SetActionAni( m_vBuffAction.m_AttackEndAnimation,
			0.0f, FLOAT100,
			FLOAT1, FLOAT1/fEndTimeRate,
			fWaitTime, 0.0f, true );

		int iEndAniID = pGrp->GetAnimationIdx( m_vBuffAction.m_AttackEndAnimation );
		m_dwMotionEndTime += pGrp->GetAnimationFullTime( iEndAniID ) * fEndTimeRate;
	}

	// Col Skip
	pChar->CheckCharColSkipSkillTime( iAniID, fTimeRate, dwPreDelay );

	SetExtraAniJump( pChar, iAniID, fTimeRate );

	pChar->SetReservedSliding( m_vBuffAction.m_vForceInfoList, iAniID, fTimeRate, dwPreDelay );
	
	if( m_vBuffAction.m_vWeaponInfoList.size() > 0 )
	{
		pChar->RefreshFireTimeListWithBase( m_dwWeaponBaseIndex,
			iAniID,
			m_vBuffAction.m_vWeaponInfoList,
			FTT_SKILL_ATTACK,
			fTimeRate,
			dwPreDelay );
	}
}

//���� Ÿ�ٿ��� ǥ�ù����� �ִ°� Ȯ��
void ioAreaCheckBuffSkill::CheckTargetHasBuff( ioBaseChar *pChar )
{		
	if( !pChar ) 
		return;

	if( m_EnemyBuffList.empty() )
		return;
	
	//Ÿ�� ����Ʈ
	BaseCharList vTargetList;
	//��ġ ����
	D3DXVECTOR3 vPos = pChar->GetMidPositionByRate();
	D3DXVECTOR3 vSightDir = pChar->GetTargetRot() * D3DXVECTOR3( 0.0f, 0.0f, -FLOAT1 );
	D3DXVec3Normalize( &vSightDir, &vSightDir );

	if( m_fColOffSet != 0.0f )
		vPos += vSightDir * m_fColOffSet;

	// �浹Ÿ�Կ� ���� ����Ʈ ��������
	switch( m_TargetColType )
	{
	case TCT_SPHERE:
		pChar->GetTargetCharListBySphere( vPos, m_fColRange, m_fColAngle, vTargetList );
		break;
	case TCT_CYLINDER:
		pChar->GetTargetCharListByCylinder( vPos, m_fColRange, m_fColAngle, m_fColUpHeight, m_fColUnderHeight, vTargetList );
		break;
	case TCT_BOX:
		// �簢���� center�� vPos�� �����ϰ� �ǹǷ� ���⼭ ĳ���;��ʿ� �����ϰ� ���̸� �����Ѵ�.
		// /2.0f�� *FLOAT05�� ����
		vPos += vSightDir * m_fColRange * FLOAT05 ; 
		// /2.0f �� * FLOAT05�� ����
		D3DXVECTOR3 vOffset( 0.0f, ( ( m_fColUpHeight + m_fColUnderHeight ) * FLOAT05 ) - m_fColUpHeight, 0.0f );
		vPos -= vOffset;
		pChar->GetTargetCharListByBox( vPos, m_fColRange, m_fColWidth, m_fColUpHeight, m_fColUnderHeight, vTargetList );
		break;
	}

	//Ÿ�� ����Ʈ�� ����ִٸ� ����
	if ( vTargetList.empty() )
		return;

	int nTargetCnt = vTargetList.size();	

	int nBuffCnt = m_EnemyBuffList.size();

	for ( int i=0; i < nTargetCnt; ++i )
	{
		ioBaseChar* pTarget = vTargetList[i];
		//���� ���̸� Ȯ������ ����
		if ( pChar->GetTeam() == pTarget->GetTeam() )
			continue;

		//ǥ�� ������ ���� �ִٸ�
		if ( pTarget->HasBuff( m_szCheckBuff ) )
		{
			if( pTarget->IsProtectState() )
				continue;
						
			//�ش� ������ ����
			for( int i=0; i < nBuffCnt; ++i )
			{
				ioHashString szBuffName = m_EnemyBuffList[i].m_BuffName;
				
				if( !szBuffName.IsEmpty() )
				{
					if( pTarget->IsHasCrown() && m_bDisableKingSkill )
					{
						continue;
					}

					pTarget->AddNewBuff( szBuffName, pChar->GetCharName(), GetOwnerItem()->GetName(), this );					
				}
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////