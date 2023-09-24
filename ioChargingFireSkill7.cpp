#include "StdAfx.h"
#include "ioChargingFireSkill7.h"

#include "ioPlayStage.h"
#include "ioBaseChar.h"

//�⺻ ������
ioChargingFireSkill7::ioChargingFireSkill7()
{
	m_SkillState = SS_NONE;
	m_bSetHandMesh = false;

	m_dwCurSkillProtectTime = 0;

	m_bSetAttackFire = false;

	m_CurDirKey = ioUserKeyInput::DKI_NONE;
	m_InputDirKey = ioUserKeyInput::DKI_NONE;
}

//���� ������ ( Clone ���� ����)
ioChargingFireSkill7::ioChargingFireSkill7( const ioChargingFireSkill7 &rhs )
	: ioAttackSkill( rhs ),
	m_dwSkillProtectTime( rhs.m_dwSkillProtectTime ),
	m_vChargeShotList( rhs.m_vChargeShotList ),
	m_dwMaxChargeTime( rhs.m_dwMaxChargeTime ),
	m_dwChargeTic( rhs.m_dwChargeTic ),
	m_ChargeMotionName( rhs.m_ChargeMotionName ),
	m_fChargeAnimationTimeRate( rhs.m_fChargeAnimationTimeRate ),
	m_bSetFirstAttack( rhs.m_bSetFirstAttack ),
	m_bSetRotation( rhs.m_bSetRotation )
{	
	m_SkillState = SS_NONE;
	m_bSetHandMesh = false;

	m_dwCurSkillProtectTime = 0;

	m_bSetAttackFire = false;

	m_CurDirKey = ioUserKeyInput::DKI_NONE;
	m_InputDirKey = ioUserKeyInput::DKI_NONE;
	m_dwStartChargeTime = 0;
}

//�Ҹ���
ioChargingFireSkill7::~ioChargingFireSkill7()
{	
}

//ini ���� �б�
void ioChargingFireSkill7::LoadProperty( ioINILoader &rkLoader )
{
	ioAttackSkill::LoadProperty( rkLoader );

	//�߰� ������ �б�
	LoadChargeProperty(rkLoader);
	
	m_dwSkillProtectTime = (DWORD)rkLoader.LoadInt_e( "skill_protect_time", 0 );
}

//���� ������ �б�
void ioChargingFireSkill7::LoadChargeProperty( ioINILoader &rkLoader )
{
	char szKey[MAX_PATH] = "";
	char szBuf[MAX_PATH];

	//���� �ִϸ��̼� �̸�
	rkLoader.LoadString_e( "gathering_ani", "", szBuf, MAX_PATH );
	m_ChargeMotionName = szBuf;
	//�ִϸ��̼� ������
	m_fChargeAnimationTimeRate = rkLoader.LoadFloat_e("gathering_ani_rate",FLOAT1);

	//���� ���� �ܰ� ���
	int iChargeCnt = rkLoader.LoadInt_e( "gathering_charge_attack_cnt", 0 );
	m_vChargeShotList.clear();

	//�ܰ谡 �ִٸ� ũ�� ���� �� ������ �ֱ�
	if( iChargeCnt > 0 )
	{
		m_vChargeShotList.reserve( iChargeCnt );

		for( int i=0; i < iChargeCnt; ++i )
		{
			AttackAttribute kChargeShot;

			wsprintf_e( szKey, "gathering_charge_attack_combo%d", i+1 );
			LoadAttackAttribute( szKey, kChargeShot, rkLoader );

			m_vChargeShotList.push_back( kChargeShot );
		}
	}

	//�ִ� ���� �ð�, ���� �ܰ��� ���̰�, ��������Ȯ�� �ð�
	m_dwMaxChargeTime = rkLoader.LoadInt_e( "max_gather_time", 0 );
	m_dwChargeTic = rkLoader.LoadInt_e( "gather_tic", 0 );
	m_bSetFirstAttack = rkLoader.LoadBool_e( "set_first_attack", 0);
	m_bSetRotation = rkLoader.LoadBool_e( "set_charging_rotation", 0);
}

//��ų ���� ���� �� ��ȯ
ioSkill* ioChargingFireSkill7::Clone()
{
	return new ioChargingFireSkill7( *this );
}

//��ų ���� �κ� ����
bool ioChargingFireSkill7::OnSkillStart( ioBaseChar *pChar )
{	
	m_dwEnableReserveTime = 0;
	m_dwFireMotionEndTime = 0;

	m_SkillState = SS_NONE;
	m_CurDirKey = ioUserKeyInput::DKI_NONE;
	m_InputDirKey = ioUserKeyInput::DKI_NONE;

	InitExtraAniJump();

	m_bSetAttackFire = false;

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
	
	//���� ���� �ð�, ���� �ܰ�, ���� �ʱ�ȭ ����
	m_dwStartChargeTime = 0;
	m_nChargeGrade = 0;
	m_bIsChargeStateInit = false;
	m_bIsChargeShotInit = false;
	
	return true;
}

//��ų �ߵ� �� ���� (Loop ����)
void ioChargingFireSkill7::OnProcessState( ioBaseChar *pChar, ioPlayStage *pStage )
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
	case SS_CHARGING:
		{		
			if ( m_bSetFirstAttack && m_dwMotionEndTime > dwCurTime )
				return;

			//Ű�� ������ ������, �������� Ȯ���Ͽ� �ܰ� ���� �� ���� ����, �ݺ� �ִϸ��̼�
			CheckChargeShotState( pChar );
		}
		break;
	case SS_CHARGESHOT:
		{
			//���� ���� �� �ִϸ��̼� ����
			ActionChargeShot( pChar );
		}
		break;
	}
	
	//��ų ���°� �׼� ��, �޺� ��, ���� �߻� �߿� ����� ������ �ȴٸ� ���� ���·� ����
	if ( m_dwMotionEndTime <= dwCurTime && m_bIsChargeShotInit )
	{
		//���� ���·� ����
		m_SkillState = SS_END;
	}
}

//��ų ���� �κ� ����
void ioChargingFireSkill7::OnSkillEnd( ioBaseChar *pChar )
{
	ioAttackSkill::OnSkillEnd( pChar );

	if( GetOwnerItem() )
		GetOwnerItem()->ClearCurChargeRateForSkill();
	
	m_bSetAttackFire = false;
	
	if( !m_SkillSoundName.IsEmpty() )
	{
		g_SoundMgr.StopSound( m_SkillSoundName, m_SkillSoundID );
	}
}

//�޽� ���� ����
bool ioChargingFireSkill7::CheckUseItemMesh()
{
	return m_bUseItemMesh;
}

//���� �Ӽ� ����
void ioChargingFireSkill7::ApplyAttributeToWeapon( ioWeapon *pWeapon, ioPlayStage *pStage )
{
	ioSkill::ApplyAttributeToWeapon( pWeapon, pStage );
	SetChargingRateForWeapon( pWeapon ); 	
}

//��ų ���� �߰� ���� (��Ŷ�� �޾� ĳ������ ���¸� ó��, �̵�, ȸ��, Ư�� ��� ����ȭ)
void ioChargingFireSkill7::ApplyExtraSkillInfo( ioBaseChar *pChar, ioPlayStage *pStage, SP2Packet &rkPacket )
{	
	if( !pChar )
		return;

	int iState;
	rkPacket >> iState;

	switch( iState )
	{	
	case SS_CHANGEROT:
		{
			//ȸ�� ���� ����
			int nInputDir;
			rkPacket >> nInputDir;

			m_InputDirKey = (ioUserKeyInput::DirKeyInput)nInputDir;
			pChar->SetTargetRotToDirKey( m_InputDirKey );
		}
		break;
	case SS_CHARGESHOT:
		{
			//��ų ���� ���� �� �ܰ� ����
			int nChargeGrade;
			rkPacket >> nChargeGrade;

			m_SkillState = (SkillState)iState;
			m_nChargeGrade = nChargeGrade;
		}
		break;
	}
}

//��ų ���� ����� �����Ѱ�
bool ioChargingFireSkill7::IsSkillMotionEnd( DWORD dwActionStop, ioBaseChar* pOwner ) const
{
	if ( m_SkillState == SS_END )
		return true;

	return false;
}

//��ų ���� ���� ���¿� ���� ó��
bool ioChargingFireSkill7::IsProtected( int iDefenseBreakType ) const
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
bool ioChargingFireSkill7::IsEnableReserve() const
{
	if( m_dwPreDelayStartTime != 0 )
		return false;

	if( m_dwEnableReserveTime == 0 || m_dwMotionStartTime >= m_dwEnableReserveTime )
		return false;

	if( m_dwEnableReserveTime < FRAMEGETTIME() )
		return true;

	return false;
}

//��ų�� ���� �����ΰ�
bool ioChargingFireSkill7::IsJumpState() const
{	
	return false;
}

//��ų�� ���ִ� �����ΰ�
bool ioChargingFireSkill7::IsNoDropState() const
{	
	return false;
}

//��Ŷ (��ġ�� ����) �� ���� �� �ִ� �����ΰ�
bool ioChargingFireSkill7::IsCanSendControl() const
{	
	if( m_bSetAttackFire )
		return false;

	return true;
}

//�뽬 Ȯ���� �����Ѱ�
bool ioChargingFireSkill7::IsEnableDashCheck( ioBaseChar *pChar )
{		
	return false;
}

//��ų ���� �����ΰ�
bool ioChargingFireSkill7::IsEndState() const
{
	if( m_SkillState == SS_END )
		return true;

	return false;
}

//���� �Ϸ� ���� (���� ��ų ��� �� ���� ����) 
bool ioChargingFireSkill7::IsAttackEndState() const
{
	return IsEndState();
}

void ioChargingFireSkill7::AttackSkillFire( ioBaseChar *pChar )
{
	if ( m_bSetFirstAttack )
	{
		ioAttackSkill::AttackSkillFire( pChar );
	}
	else 
	{
		if( !m_bReduceGauge )
		{
			pChar->ReduceNeedGaugeBySkill( this, GetNeedGauge(pChar->GetCharName()) );
			m_bReduceGauge = true;
		}
	}
	m_bSetAttackFire = true;	
	m_SkillState = SS_CHARGING;
}
//�߰��� �Լ�/////////////////////////////////////////////////////////

//���� �ݺ� �ִϸ��̼� ����
void ioChargingFireSkill7::SetChargeLoopAnimation( ioBaseChar *pChar )
{
	ioEntityGroup *pGrp = pChar->GetGroup();

	int iAniID = pGrp->GetAnimationIdx( m_ChargeMotionName );
	if( iAniID == -1 )	return;
	
	pGrp->ClearAllLoopAni( FLOAT100, true );
	pGrp->ClearAllActionAni( FLOAT100, true );

	pGrp->SetLoopAni( iAniID, FLOAT100, FLOAT1, FLOAT1, 0.0f );
}

//���� �����϶� ĳ���� ȸ��
void ioChargingFireSkill7::SetCharacterRot( ioBaseChar *pChar )
{
	if ( !m_bSetRotation )
		return;

	//����Ű �Է��� �ִٸ�
	if( pChar->IsNeedProcess() && pChar->IsSettedDirection() )
	{
		//���� �Էµ� ����Ű Ȯ���ϰ� ����
		ioUserKeyInput::DirKeyInput eNewDirKey = pChar->CheckCurDirKey();
	
		//�Էµ� ����Ű�� ���� ĳ������ ������ �ٸ��ٸ�
		if ( m_InputDirKey != eNewDirKey )
		{
			m_InputDirKey = eNewDirKey;

			//�Էµ� �������� ȸ��
			pChar->SetTargetRotToDirKey( m_InputDirKey );

			if ( pChar->IsNeedSendNetwork() )
			{
				//��Ŷ����
				SP2Packet kPacket( CUPK_SKILL_EXTRA_INFO );
				kPacket << pChar->GetCharName();
				kPacket << GetName();
				kPacket << SS_CHANGEROT;
				kPacket << (int)m_InputDirKey;
				P2PNetwork::SendToAllPlayingUser( kPacket );
			}
		}
	}
}

//���� �߻� ���� Ȯ��
void ioChargingFireSkill7::CheckChargeShotState( ioBaseChar *pChar )
{
	if ( !pChar ) 
		return;

	DWORD dwCurTime = FRAMEGETTIME();

	//���� �ʱ�ȭ (ó�� �ѹ��� ����)
	if ( !m_bIsChargeStateInit )
	{
		//���� �ݺ� �ִϸ��̼� ����
		SetChargeLoopAnimation( pChar );
		//���� ���� �ð� ����
		m_dwStartChargeTime = dwCurTime;

		m_bIsChargeStateInit = true;
	}

	//ĳ���� ȸ�� Ȯ��
	SetCharacterRot( pChar );
	
	if( pChar->IsNeedProcess() )
	{
		//�ִ� ���� �ð��� �������鼭 Ű�� �����ִٸ� �ִ� �ܰ� ���� �� �߻� (�ִ� �ð� ��� �� �ڵ� �߻� ���·� ����)
		if ( m_dwStartChargeTime + m_dwMaxChargeTime <= dwCurTime && pChar->IsAttackKeyDown() )
		{
			m_nChargeGrade = (int)m_vChargeShotList.size() - 1;
			m_SkillState = SS_CHARGESHOT;			
		}
		//Ű�� �������ٸ� �ð��� ���� �ܰ� ���� �� ���� ����
		else if ( pChar->IsAttackKeyRelease() || !pChar->IsAttackKeyDown() )
		{
			m_nChargeGrade = (int)( ( dwCurTime - m_dwStartChargeTime ) / m_dwChargeTic );
			m_SkillState = SS_CHARGESHOT;
		}

		//���� �߻� ���¸�
		if ( m_SkillState == SS_CHARGESHOT && pChar->IsNeedSendNetwork() )
		{
			//��Ŷ����
			SP2Packet kPacket( CUPK_SKILL_EXTRA_INFO );
			kPacket << pChar->GetCharName();
			kPacket << GetName();
			kPacket << m_SkillState;
			kPacket << m_nChargeGrade;
			P2PNetwork::SendToAllPlayingUser( kPacket );
		}
	}
}

//���� �߻� 
void ioChargingFireSkill7::ActionChargeShot( ioBaseChar *pChar )
{
	if ( !pChar || m_bIsChargeShotInit )
		return;

	m_bIsChargeShotInit = true;

	ioEntityGroup *pGrp = pChar->GetGroup();

	//�ܰ��� �ִϸ��̼� �� �б�
	int iAniID = pGrp->GetAnimationIdx( m_vChargeShotList[m_nChargeGrade].m_AttackAnimation );
	if( iAniID == -1 )	return;

	//������ ����, ������ ����
	float fTimeRate  = m_vChargeShotList[m_nChargeGrade].m_fAttackAniRate;
	DWORD dwPreDelay = m_vChargeShotList[m_nChargeGrade].m_dwPreDelay;

	pGrp->ClearAllLoopAni( FLOAT100, true );
	pGrp->ClearAllActionAni( FLOAT100, true );
	
	//��� ���� �ð� ����
	m_dwMotionStartTime = m_dwMotionEndTime = FRAMEGETTIME();
	//���� �ð� ����
	m_dwMotionEndTime += pGrp->GetAnimationFullTime( iAniID ) * fTimeRate;
	m_dwMotionEndTime += dwPreDelay + m_vChargeShotList[m_nChargeGrade].m_dwEndDelay;

	//���� �ִϸ��̼��� ���ٸ�
	if( m_vChargeShotList[m_nChargeGrade].m_AttackEndAnimation.IsEmpty() )
	{
		pGrp->SetActionAni( iAniID, FLOAT100, FLOAT100, FLOAT1, FLOAT1/fTimeRate, 0.0f, dwPreDelay, true );
	}
	//�ִٸ�
	else
	{
		//�⺻ �ִϸ��̼� ����
		pGrp->SetActionAni( iAniID, FLOAT100, 0.0f, FLOAT1, FLOAT1/fTimeRate, 0.0f, dwPreDelay );

		//������ �ð�
		float fWaitTime = pGrp->GetAnimationFullTime( iAniID ) * fTimeRate + dwPreDelay;
		float fEndTimeRate = m_vChargeShotList[m_nChargeGrade].m_fAttackEndAniRate;

		int iEndAniID = pGrp->GetAnimationIdx( m_vChargeShotList[m_nChargeGrade].m_AttackEndAnimation );
		m_dwMotionEndTime += pGrp->GetAnimationFullTime( iEndAniID ) * fEndTimeRate;

		//���� �ִϸ��̼� ����
		pGrp->SetActionAni( iEndAniID, 0.0f, FLOAT100, FLOAT1, FLOAT1/fEndTimeRate, fWaitTime, 0.0f, true );		
	}

	//���� ���� �ε��� �ֱ�
	m_dwWeaponBaseIndex = pChar->IncreaseWeaponIndexBase();

	//������ŭ �ش� ���� ���� (������ ���� ĳ���Ͱ� ���� ����)
	if( m_bRefreshFireTimeList )
	{
		pChar->RefreshFireTimeListWithBase( m_dwWeaponBaseIndex,
											iAniID,
											m_vChargeShotList[m_nChargeGrade].m_vWeaponInfoList,
											FTT_SKILL_ATTACK,
											fTimeRate,
											dwPreDelay );
	}
}
////////////////////////////////////////////////////////////////////////
