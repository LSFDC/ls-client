#include "StdAfx.h"
#include "ioProtectCountAreaAttackSkill.h"

#include "ioPlayStage.h"
#include "ioBaseChar.h"

//�⺻ ������
ioProtectCountAreaAttackSkill::ioProtectCountAreaAttackSkill()
{
	m_SkillState = SS_NONE;
	m_bSetHandMesh = false;

	m_dwCurSkillProtectTime = 0;
	
	m_dwCheckProtectTime = 0;
	m_bIsAreaAttackInit = false;
	m_bIsAreaCreateWeapon = false;
	m_dwAreaFireTime = 0;
	m_dwCheckNoWoundTime = 0;
}

//���� ������ ( Clone ���� ����)
ioProtectCountAreaAttackSkill::ioProtectCountAreaAttackSkill( const ioProtectCountAreaAttackSkill &rhs )
	: ioAttackSkill( rhs ),
	m_dwSkillProtectTime( rhs.m_dwSkillProtectTime ),
	m_dwProtectTime( rhs.m_dwProtectTime ),
	m_szLookBuffName( rhs.m_szLookBuffName),
	m_szAreaWeaponName( rhs.m_szAreaWeaponName),
	m_szAreaAttackAnimation( rhs.m_szAreaAttackAnimation ),
	m_fAreaAttackAnimationRate( rhs.m_fAreaAttackAnimationRate ),
	m_szAreaAttackEffect( rhs.m_szAreaAttackEffect ),
	m_dwAreaAttackDelayTime( rhs.m_dwAreaAttackDelayTime ),
	m_bSetAreaFireDelayTime( rhs.m_bSetAreaFireDelayTime ),
	m_dwAreaFireDelayTime( rhs.m_dwAreaFireDelayTime ),
	m_bSetNoWound( rhs.m_bSetNoWound ),
	m_dwNoWoundTime( rhs.m_dwNoWoundTime ),
	m_OwnerBuffList( rhs.m_OwnerBuffList )
{	
	m_SkillState = SS_NONE;
	m_bSetHandMesh = false;

	m_dwCurSkillProtectTime = 0;
	
	m_dwCheckProtectTime = 0;
	m_bIsAreaAttackInit = false;
	m_bIsAreaCreateWeapon = false;
	m_dwAreaFireTime = 0;
	m_dwCheckNoWoundTime = 0;
}

//�Ҹ���
ioProtectCountAreaAttackSkill::~ioProtectCountAreaAttackSkill()
{	
}

//ini ���� �б�
void ioProtectCountAreaAttackSkill::LoadProperty( ioINILoader &rkLoader )
{
	ioAttackSkill::LoadProperty( rkLoader );

	//�߰� ������ �б�
	LoadProtectCountProperty( rkLoader );
	LoadBuffInfoProperty( rkLoader );
	m_dwSkillProtectTime = (DWORD)rkLoader.LoadInt_e( "skill_protect_time", 0 );
}

//���� ���� ����
void ioProtectCountAreaAttackSkill::LoadBuffInfoProperty( ioINILoader &rkLoader )
{
	char szTitle[MAX_PATH], szBuf[MAX_PATH];
	
	ioBuffInfo tBuffInfo;
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
}

//���� ������ �б�
void ioProtectCountAreaAttackSkill::LoadProtectCountProperty( ioINILoader &rkLoader )
{
	char szBuf[MAX_PATH];

	//��� �ð� ����
	m_dwProtectTime = rkLoader.LoadInt_e( "defence_time", 0 );

	//�ٶ󺸱� ���� ����
	rkLoader.LoadString_e( "defence_look_buff", "", szBuf, MAX_PATH );
	m_szLookBuffName = szBuf;

	//���� ���� ����
	rkLoader.LoadString_e( "area_weapon_name", "", szBuf, MAX_PATH );
	m_szAreaWeaponName = szBuf;

	//�ִϸ��̼� ����
	rkLoader.LoadString_e( "area_attack_animation", "", szBuf, MAX_PATH );
	m_szAreaAttackAnimation = szBuf;

	m_fAreaAttackAnimationRate = rkLoader.LoadFloat_e( "area_attack_animation_rate", FLOAT1 );
	
	//����Ʈ ����
	rkLoader.LoadString_e( "area_attack_effect", "", szBuf, MAX_PATH );
	m_szAreaAttackEffect = szBuf;

	//������ �ð�
	m_dwAreaAttackDelayTime = rkLoader.LoadInt_e( "area_attack_delay_time", 0 );

	//�ð��� ���� ���� ����?
	m_bSetAreaFireDelayTime = rkLoader.LoadBool_e( "set_area_weapon_fire_time", false );
	//���� ���� ������ �ð�
	m_dwAreaFireDelayTime = rkLoader.LoadInt_e( "area_weapon_delay_time", 0 );
	//���� �ִϸ��̼� ���� ������ �ȹ���
	m_bSetNoWound = rkLoader.LoadBool_e( "area_attack_nowound", false );
	//���� �ȹ޴� �ð� ����
	m_dwNoWoundTime = rkLoader.LoadInt_e( "area_attack_nowound_time", 0 );
}

//��ų ���� ���� �� ��ȯ
ioSkill* ioProtectCountAreaAttackSkill::Clone()
{
	return new ioProtectCountAreaAttackSkill( *this );
}

//��ų ���� �κ� ����
bool ioProtectCountAreaAttackSkill::OnSkillStart( ioBaseChar *pChar )
{	
	if( !ioAttackSkill::OnSkillStart( pChar ) )
		return false;

	if( !m_SkillSoundName.IsEmpty() && pChar )
	{
		ioEntityGroup *pGrp = pChar->GetGroup();
		m_SkillSoundID = g_SoundMgr.PlaySound( m_SkillSoundName, pGrp->GetParentSceneNode() );
	}

	m_dwCurSkillProtectTime = 0;
	if( m_dwSkillProtectTime > 0 )
		m_dwCurSkillProtectTime = FRAMEGETTIME() + m_dwSkillProtectTime;

	m_SkillState = SS_NONE;
	m_bSetHandMesh = false;
	
	m_dwCheckProtectTime = 0;
	m_bIsAreaAttackInit = false;
	m_bIsAreaCreateWeapon = false;
	m_dwAreaFireTime = 0;
	m_dwCheckNoWoundTime = 0;
	
	if( m_bUseItemMesh )
	{
		pChar->ShowWeaponItemMesh( false );
	}

	if( !m_bSetHandMesh && m_bUseItemMesh && !m_HandMesh.IsEmpty() )
	{
		pChar->ChangeSkillWeaponMesh( this, m_HandMesh, m_bVisibleEquipMesh );
		m_bSetHandMesh = true;
	}

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
void ioProtectCountAreaAttackSkill::OnProcessState( ioBaseChar *pChar, ioPlayStage *pStage )
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
	case SS_PROTECT:
		{
			//��� ���¿����� �ð��� Ȯ���Ͽ� ���¸� ���� (��������1)
			if ( m_dwCheckProtectTime < dwCurTime )
			{
				//���� ���·� ����
				m_SkillState = SS_END;
			}
		}
		break;
	case SS_AREA_ATTACK:
		{
			if ( !m_bIsAreaAttackInit )
			{
				//���� ���� ����ȭ
				SendAreaAttackState( pChar );
				//�ٶ󺸱� ����
				AddLookBuff( pChar );
				//�ִϸ��̼� ����
				SetAreaAttackAnimation( pChar );
				m_bIsAreaAttackInit = true;
			}

			//���� ����
			CheckCreateAreaWeapon( pChar );

			//������ �ִϸ��̼� ������ �Ϸ�� �� ��� �ð��� �Ѿ�� ����
			if ( m_dwMotionEndTime < dwCurTime )
			{
				//���� ���·� ����
				m_SkillState = SS_END;
			}
		}
		break;
	}
}

//��ų ���� �κ� ����
void ioProtectCountAreaAttackSkill::OnSkillEnd( ioBaseChar *pChar )
{
	//�ڽſ��� �����ϴ� ������ ����
	for (int i=0; i < (int)m_OwnerBuffList.size(); ++i)
	{
		ioBuffInfo pOwnerBuff = m_OwnerBuffList[i];
		pChar->RemoveBuff( pOwnerBuff.m_BuffName, false);			
	}

	ioAttackSkill::OnSkillEnd( pChar );
			
	if( !m_SkillSoundName.IsEmpty() )
	{
		g_SoundMgr.StopSound( m_SkillSoundName, m_SkillSoundID );
	}

	//���� ����
	if ( !m_szLookBuffName.IsEmpty() && pChar->HasBuff( m_szLookBuffName ) )
	{
		pChar->RemoveBuff( m_szLookBuffName, false);	
	}
	
	pChar->EndEffect( m_szAreaAttackEffect );

	// �⺻���� ����
	if( m_bUseItemMesh )
	{
		pChar->ShowWeaponItemMesh( true );
	}

	if( !m_HandMesh.IsEmpty() )
	{
		pChar->RestoreSkillWeaponMesh( this, m_HandMesh, m_bVisibleEquipMesh );
	}
}

//�޽� ���� ����
bool ioProtectCountAreaAttackSkill::CheckUseItemMesh()
{
	return m_bUseItemMesh;
}

//���� �Ӽ� ����
void ioProtectCountAreaAttackSkill::ApplyAttributeToWeapon( ioWeapon *pWeapon, ioPlayStage *pStage )
{
	ioSkill::ApplyAttributeToWeapon( pWeapon, pStage );
}

//��ų ���� �߰� ���� (��Ŷ�� �޾� ĳ������ ���¸� ó��, �̵�, ȸ��, Ư�� ��� ����ȭ)
void ioProtectCountAreaAttackSkill::ApplyExtraSkillInfo( ioBaseChar *pChar, ioPlayStage *pStage, SP2Packet &rkPacket )
{	
	if( !pChar )
		return;

	int iState;
	rkPacket >> iState;

	switch( iState )
	{		
	case SS_AREA_ATTACK:
		{			
			m_SkillState = static_cast< SkillState >( iState );
			rkPacket >> m_szAttackerName;
		}
		break;
	}	
}

//��ų ���� ����� �����Ѱ�
bool ioProtectCountAreaAttackSkill::IsSkillMotionEnd( DWORD dwActionStop, ioBaseChar* pOwner ) const
{
	if ( m_SkillState == SS_END )
		return true;

	return false;
}

//��ų ���� ���� ���¿� ���� ó��
bool ioProtectCountAreaAttackSkill::IsProtected( int iDefenseBreakType ) const
{	
	//���� �ִϸ��̼� �߿� �ǰ��� �ȴ��ϴ� ���¸鼭, 
	//�ð� ������ ���ų�, 
	//�ð��� �����鼭 ���� �ð��� Ȯ�� �ð��� ������ �ʾҴٸ� ����
	if ( m_SkillState == SS_AREA_ATTACK && m_bSetNoWound && 
		( m_dwNoWoundTime == 0 || ( m_dwNoWoundTime > 0 && (m_dwCheckNoWoundTime + m_dwNoWoundTime >= FRAMEGETTIME()) ) ) )
		return true;
	
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
bool ioProtectCountAreaAttackSkill::IsEnableReserve() const
{	
	return false;
}

//��ų�� ���� �����ΰ�
bool ioProtectCountAreaAttackSkill::IsJumpState() const
{	
	return false;
}

//��ų�� ���ִ� �����ΰ�
bool ioProtectCountAreaAttackSkill::IsNoDropState() const
{	
	return false;
}

//��Ŷ (��ġ�� ����) �� ���� �� �ִ� �����ΰ�
bool ioProtectCountAreaAttackSkill::IsCanSendControl() const
{	
	return true;
}

//�뽬 Ȯ���� �����Ѱ�
bool ioProtectCountAreaAttackSkill::IsEnableDashCheck( ioBaseChar *pChar )
{		
	return false;
}

//��ų ���� �����ΰ�
bool ioProtectCountAreaAttackSkill::IsEndState() const
{
	if( m_SkillState == SS_END )
		return true;

	return false;
}

//���� �Ϸ� ���� (���� ��ų ��� �� ���� ����) 
bool ioProtectCountAreaAttackSkill::IsAttackEndState() const
{
	return IsEndState();
}

void ioProtectCountAreaAttackSkill::AttackSkillFire( ioBaseChar *pChar )
{
	ioAttackSkill::AttackSkillFire( pChar );
	m_SkillState = SS_PROTECT;

	m_dwCheckProtectTime = FRAMEGETTIME();
	m_dwCheckProtectTime += m_dwProtectTime;	
}

//�߰��� �Լ�
//��ų ���� Ȯ��
bool ioProtectCountAreaAttackSkill::IsProtectState() const
{
	if ( m_SkillState == SS_PROTECT )
	{
		return true;
	}

	return false;
}

//��ų ���� ����
void ioProtectCountAreaAttackSkill::SetAreaAttackState( ioHashString szAttacker )
{
	m_szAttackerName = szAttacker;
	m_SkillState = SS_AREA_ATTACK;
}

//���� ����
void ioProtectCountAreaAttackSkill::AddLookBuff( ioBaseChar *pChar )
{
	if ( !pChar )
		return;

	//���� �̸��� �����鼭 ������ �̸��� ������ ���� ����
	if ( !m_szLookBuffName.IsEmpty() && !m_szAttackerName.IsEmpty() )
	{
		pChar->AddNewBuff( m_szLookBuffName, m_szAttackerName, GetOwnerItem()->GetName(), this);	
	}
}

//������ �ִϸ��̼� ����
void ioProtectCountAreaAttackSkill::SetAreaAttackAnimation( ioBaseChar *pChar )
{
	if ( !pChar )
		return;

	ioEntityGroup *pGrp = pChar->GetGroup();
	int iAniID = pGrp->GetAnimationIdx( m_szAreaAttackAnimation );
	if( iAniID == -1 )	return;

	pChar->InitActionStopDelay( true );

	pChar->AttachEffect( m_szAreaAttackEffect );

	float fTimeRate  = m_fAreaAttackAnimationRate;
	DWORD dwPreDelay = m_dwAreaAttackDelayTime;

	pGrp->ClearAllLoopAni( FLOAT100, true );
	pGrp->ClearAllActionAni( FLOAT100, true );

	DWORD dwCurTime = FRAMEGETTIME();
	m_dwMotionEndTime = dwCurTime;
	m_dwMotionEndTime += dwPreDelay;
	m_dwMotionEndTime += pGrp->GetAnimationFullTime( iAniID ) * fTimeRate;

	m_dwAreaFireTime = dwCurTime;
	m_dwAreaFireTime += dwPreDelay;

	if ( m_bSetAreaFireDelayTime )
	{
		m_dwAreaFireTime += m_dwAreaFireDelayTime;
	}
	else
	{
		m_dwAreaFireTime += pGrp->GetAnimationEventTime_e( iAniID, "fire_s" ) * fTimeRate;
	}
	
	pGrp->SetActionAni( iAniID, FLOAT100, FLOAT100, FLOAT1, FLOAT1/fTimeRate, 0.0f, dwPreDelay, true );
	
	// Col Skip
	pChar->CheckCharColSkipSkillTime( iAniID, fTimeRate, dwPreDelay );	

	m_dwCheckNoWoundTime = dwCurTime;
}

//���� ���� ����
void ioProtectCountAreaAttackSkill::CheckCreateAreaWeapon( ioBaseChar *pChar )
{
	//���� ó��
	//ĳ���Ͱ� ���ų�, ���� �̸��� ���ų�, ���⸦ �ѹ� ���� �߰ų�
	if( !pChar || m_szAreaWeaponName.IsEmpty() || m_bIsAreaCreateWeapon ) 
		return;

	//Area���Ⱑ ������ �ð��� �����ٸ� 
	if ( m_dwAreaFireTime <= FRAMEGETTIME() )
	{			
		//����, ��ġ
		D3DXVECTOR3 vPos = pChar->GetWorldPosition();
		D3DXQUATERNION qtRot = pChar->GetTargetRot();

		//Area ���� ����
		g_AreaWeaponMgr.CreateAreaWeapon( pChar->GetCharName(), "",GetName(), m_szAreaWeaponName, vPos, qtRot, ioAreaWeapon::CT_NORMAL );

		m_bIsAreaCreateWeapon = true;
	}
}

//���� ���� ����ȭ
void ioProtectCountAreaAttackSkill::SendAreaAttackState( ioBaseChar *pChar )
{
	if ( !pChar )
		return;

	if ( pChar->IsNeedProcess() && pChar->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_SKILL_EXTRA_INFO );
		kPacket << pChar->GetCharName();
		kPacket << GetName();
		kPacket << SS_AREA_ATTACK;
		kPacket << m_szAttackerName;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}
////////////////////////////////////////////////////////////////////////
