
#include "stdafx.h"

#include "ioPlayStage.h"
#include "ioBaseChar.h"

#include "ioRunningAttackSkill5.h"

#include "WeaponDefine.h"

ioRunningAttackSkill5::ioRunningAttackSkill5()
{
	m_SkillState = SS_NONE;

	m_dwRunningStartTime = 0;
	m_dwRunningEndTime = 0;
	
	m_bTargetRot = false;
	m_bLeftRot = false;
	m_CurDirKey = ioUserKeyInput::DKI_NONE;

	m_bSetHandMesh = false;
	m_dwCurSkillProtectTime = 0;
	m_bIsRun = false;
	m_dwDelayEffectID = -1;
	m_dwLoopEffectID = -1;
	m_bSetDelayInit = true;
	m_bSetLoopInit = false;
}

ioRunningAttackSkill5::ioRunningAttackSkill5( const ioRunningAttackSkill5 &rhs )
	: ioAttackSkill( rhs ),
	  m_fRunSpeed( rhs.m_fRunSpeed ),
	  m_dwRotateTime( rhs.m_dwRotateTime ),
	  m_OwnerBuffList( rhs.m_OwnerBuffList ),
	  m_dwRunningDuration( rhs.m_dwRunningDuration ),
	  m_dwSkillProtectTime( rhs.m_dwSkillProtectTime ),
	  m_szCheckBuff( rhs.m_szCheckBuff ),
	  m_szDelayAnimation( rhs.m_szDelayAnimation ),
	  m_szDelayEffect( rhs.m_szDelayEffect ),
	  m_szLoopAnimation( rhs.m_szLoopAnimation ),
	  m_szLoopEffect( rhs.m_szLoopEffect )
{
	m_SkillState = SS_NONE;

	m_dwRunningStartTime = 0;
	m_dwRunningEndTime = 0;
	
	m_bTargetRot = false;
	m_bLeftRot = false;
	m_CurDirKey = ioUserKeyInput::DKI_NONE;

	m_bSetHandMesh = false;
	m_dwCurSkillProtectTime = 0;
	m_bIsRun = false;
	m_dwDelayEffectID = -1;
	m_dwLoopEffectID = -1;
	m_bSetDelayInit = true;
	m_bSetLoopInit = false;
}

ioRunningAttackSkill5::~ioRunningAttackSkill5()
{
}

void ioRunningAttackSkill5::LoadProperty( ioINILoader &rkLoader )
{
	ioAttackSkill::LoadProperty( rkLoader );

	char szBuf[MAX_PATH] = "";
	
	//�̵� ���� ����
	m_fRunSpeed = rkLoader.LoadFloat_e( "run_speed", 0.0f );
	m_dwRunningDuration = rkLoader.LoadInt_e( "run_duration", 0);
	m_dwRotateTime = (DWORD)rkLoader.LoadInt_e( "run_rotate_speed", 0 );
	//��ų ���� �ð�
	m_dwSkillProtectTime = (DWORD)rkLoader.LoadInt_e( "skill_protect_time", 0 );
	//Ȯ���� ���� �̸�
	rkLoader.LoadString_e( "check_buff_name", "", szBuf, MAX_PATH );
	m_szCheckBuff = szBuf;
	//������ �ִϸ��̼�, ����Ʈ ����
	rkLoader.LoadString_e( "delay_animation", "", szBuf, MAX_PATH );
	m_szDelayAnimation = szBuf;
	rkLoader.LoadString_e( "delay_effect", "", szBuf, MAX_PATH );
	m_szDelayEffect = szBuf;
	//���� �ִϸ��̼�, ����Ʈ ����
	rkLoader.LoadString_e( "loop_animation", "", szBuf, MAX_PATH );
	m_szLoopAnimation = szBuf;
	rkLoader.LoadString_e( "loop_effect", "", szBuf, MAX_PATH );
	m_szLoopEffect = szBuf;
	//���� ����Ʈ �б�	
	LoadOwnerBuffList( rkLoader );
}

//���� ����Ʈ �б�
void ioRunningAttackSkill5::LoadOwnerBuffList( ioINILoader &rkLoader )
{
	int iCnt = rkLoader.LoadInt_e( "owner_buff_cnt", 0 );
	m_OwnerBuffList.clear();

	if( iCnt <= 0 ) return;

	m_OwnerBuffList.reserve( iCnt );

	char szTitle[MAX_PATH], szBuf[MAX_PATH];
	for( int i=0; i < iCnt; ++i )
	{
		wsprintf_e( szTitle, "owner_buff%d", i+1 );
		rkLoader.LoadString( szTitle, "", szBuf, MAX_PATH );

		ioBuffInfo tBuffInfo;
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

ioSkill* ioRunningAttackSkill5::Clone()
{
	return new ioRunningAttackSkill5( *this );
}

bool ioRunningAttackSkill5::OnSkillStart( ioBaseChar *pChar )
{	
	m_SkillState = SS_NONE;

	m_dwRunningStartTime = 0;
	m_dwRunningEndTime = 0;

	m_bTargetRot = false;
	m_bLeftRot = false;
	m_CurDirKey = ioUserKeyInput::DKI_NONE;

	m_bSetHandMesh = false;
	m_dwCurSkillProtectTime = 0;
	m_bIsRun = false;
	m_dwDelayEffectID = -1;
	m_dwLoopEffectID = -1;
	m_bSetDelayInit = true;
	m_bSetLoopInit = false;

	if( m_dwSkillProtectTime > 0 )
		m_dwCurSkillProtectTime = FRAMEGETTIME() + m_dwSkillProtectTime;

	CheckChangeDirRot( pChar );

	// �⺻ ���� �տ��� ����
	if( m_bUseItemMesh )
	{
		pChar->ShowWeaponItemMesh( false );
	}

	// ��� ������ ó��
	if( !m_bSetHandMesh && m_bUseItemMesh && !m_HandMesh.IsEmpty() )
	{
		pChar->ChangeSkillWeaponMesh( this, m_HandMesh, m_bVisibleEquipMesh );
		m_bSetHandMesh = true;
	}
	
	if( !ioAttackSkill::OnSkillStart( pChar ) )
		return false;
	
	if( !m_bReduceGauge )
	{
		pChar->ReduceNeedGaugeBySkill( this, GetNeedGauge(pChar->GetCharName()) );
		m_bReduceGauge = true;
	}

	return true;
}

void ioRunningAttackSkill5::OnProcessState( ioBaseChar *pChar, ioPlayStage *pStage )
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
				EndPreDelayEffect( pChar );
				SetRunState( pChar );
				//���� ����
				SetOwnerBuffList( pChar, pStage );
			}
		}
		break;
	case SS_RUN:
		{
			if( m_dwRunningEndTime < dwCurTime )
			{
				m_dwRunningEndTime = 0;
				SetRunEndState( pChar );
				return;
			}
			else
			{
				if ( CheckBuff( pChar ) )
				{
					CheckKeyInput( pChar );
					ProcessRotate( pChar );

					if ( m_bIsRun )
					{
						pChar->SetCurMoveSpeed( m_fRunSpeed );
					}
				}
			}
		}
		break;
	}

	if ( !m_bIsRun && !m_bSetDelayInit )
	{
		SetDelayAnimation( pChar );
		m_bSetDelayInit = true;
		m_bSetLoopInit = false;
	}

	if ( m_bIsRun && !m_bSetLoopInit )
	{
		SetLoopAnimation( pChar );
		m_bSetDelayInit = false;
		m_bSetLoopInit = true;
	}

	if( pChar->GetWorldPosition().y >= 10000.f )
	{
		ioBuff* pBuff= pChar->GetBuff( m_szCheckBuff );
		if( pBuff )
			pBuff->SetReserveEndBuff();
	}
}

void ioRunningAttackSkill5::OnSkillEnd( ioBaseChar *pChar )
{
	DeleteEffect();

	ioAttackSkill::OnSkillEnd( pChar );

	// �⺻���� ����
	if( m_bUseItemMesh )
	{
		pChar->ShowWeaponItemMesh( true );
	}

	if( !m_HandMesh.IsEmpty() )
	{
		pChar->RestoreSkillWeaponMesh( this, m_HandMesh, m_bVisibleEquipMesh );
	}

	m_bSetHandMesh = false;
	pChar->SetCharColSkipSkillTime( 0, 0 );

	int nBuffCnt = (int)m_OwnerBuffList.size();	
	for (int i=0;i<nBuffCnt; ++i)
	{
		pChar->RemoveBuff( m_OwnerBuffList[i].m_BuffName );
	}
}

void ioRunningAttackSkill5::AttackSkillFire( ioBaseChar *pChar )
{
	//���� ����
	CreateWeapon( pChar );
		
	//�浹 ���� �ð� ����
	DWORD dwCurTime = FRAMEGETTIME();
	m_dwRunningStartTime = dwCurTime;
	m_dwRunningEndTime = dwCurTime + m_dwRunningDuration;
	pChar->SetCharColSkipSkillTime( m_dwRunningStartTime, m_dwRunningEndTime );

	m_bSetDelayInit = false;
}

bool ioRunningAttackSkill5::IsProtected( int iDefenseBreakType ) const
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

bool ioRunningAttackSkill5::IsSkillMotionEnd( DWORD dwActionStop, ioBaseChar* pOwner ) const
{	
	if ( m_SkillState != SS_END )
		return false;

	if( m_dwPreDelayStartTime > 0 )
		return false;

	if( m_dwMotionEndTime + dwActionStop >= FRAMEGETTIME() )
		return false;

	return true;
}

void ioRunningAttackSkill5::ApplyAttributeToWeapon( ioWeapon *pWeapon, ioPlayStage *pStage )
{
	ioSkill::ApplyAttributeToWeapon( pWeapon, pStage );

	ioZoneEffectWeapon *pZone = ToZoneEffectWeapon( pWeapon );
	if( pZone )
	{
		ioBaseChar *pChar = pZone->GetOwner();
		if( pChar )
		{
			D3DXVECTOR3 vDir = pChar->GetTargetRot() * D3DXVECTOR3( 0.0f, 0.0f, -FLOAT1 );
			pZone->SetCollisionDir( vDir );
			pZone->SetZoneValue( true, true );
		}
	}
}

void ioRunningAttackSkill5::ApplyExtraSkillInfo( ioBaseChar *pChar, ioPlayStage *pStage, SP2Packet &rkPacket )
{
	if( !pChar || !pStage ) 
		return;

	int iType;
	rkPacket >> iType;

	switch( iType )
	{		
	case SS_SWAPRUN:
		{
			rkPacket >> m_bIsRun;
		}
		break;
	case SS_ROTATE:
		{			
			//���� ��ȯ
			int nDir;
			rkPacket >> nDir;
			rkPacket >> m_bLeftRot;

			m_CurDirKey = (ioUserKeyInput::DirKeyInput)nDir;

			D3DXQUATERNION qtRot;
			rkPacket >> qtRot;
			pChar->SetTargetRot( qtRot ); 
		}
		break;
	}	
}

void ioRunningAttackSkill5::SetRunState( ioBaseChar *pChar )
{
	AttackSkillFire( pChar );
		
	m_SkillState = SS_RUN;	
}

void ioRunningAttackSkill5::SetRunEndState( ioBaseChar *pChar )
{
	if( !pChar ) return;

	g_WeaponMgr.DestroyReserveContactWeapon( pChar->GetCharName() );
	g_WeaponMgr.DestroyReserveZoneWeapon( pChar->GetCharName() );
	
	pChar->SetSkillMoveEnable( false, false );
	pChar->SetMoveDirByRotate( pChar->GetTargetRot() );
	pChar->SetCharColSkipSkillTime( 0, 0 );

	m_SkillState = SS_END;
}

void ioRunningAttackSkill5::CheckKeyInput( ioBaseChar *pOwner )
{
	if( !pOwner ) return;
	if( !pOwner->IsNeedProcess() ) return;

	bool bSendRun = false;
	bool bSendRotate = false;

	ioUserKeyInput::DirKeyInput eNewDirKey = pOwner->CheckCurDirKey();

	//�޸��� ���� (���� �ش� ����Ű�̸�)
	if ( eNewDirKey == ioUserKeyInput::DKI_LEFTUP || 
		 eNewDirKey == ioUserKeyInput::DKI_RIGHTUP || 
		 eNewDirKey == ioUserKeyInput::DKI_UP )
	{
		//�޸��� ���� �ƴϸ� �ٲٰ�
		if ( !m_bIsRun )
		{
			//�޸��� ����
			m_bIsRun = true;
			bSendRun = true;
		}
	}
	else 
	{
		//�޸��� ���̸� �ٲٰ�
		if ( m_bIsRun )
		{
			//�޸��� ����
			m_bIsRun = false;
			bSendRun = true;		
		}
	}

	//ȸ�� ���� �Է�
	if ( m_bIsRun && CheckDirKey( eNewDirKey ) )
	{
		//���� ���� ���� ����
		if( (eNewDirKey == ioUserKeyInput::DKI_LEFTUP && m_CurDirKey != ioUserKeyInput::DKI_LEFT ) || 
			(eNewDirKey == ioUserKeyInput::DKI_RIGHTUP && m_CurDirKey != ioUserKeyInput::DKI_RIGHT ) )
		{
			m_CurDirKey = eNewDirKey;
			//���� ���� ��ȯ
			if ( m_CurDirKey == ioUserKeyInput::DKI_LEFTUP)
			{
				m_CurDirKey = ioUserKeyInput::DKI_LEFT;
			}
			if ( m_CurDirKey == ioUserKeyInput::DKI_RIGHTUP)
			{
				m_CurDirKey = ioUserKeyInput::DKI_RIGHT;
			}

			D3DXQUATERNION qtCurRot = pOwner->GetTargetRot();
			//���� ����
			float fCurYaw;
			fCurYaw = RADtoDEG( ioMath::QuaterToYaw( qtCurRot ) );
			fCurYaw = ioMath::ArrangeHead( fCurYaw );

			//��ǥ ����
			float fYawD = 45.0f * ( m_CurDirKey - ioUserKeyInput::DKI_UP ) + fCurYaw;
			fYawD = ioMath::ArrangeHead( fYawD );

			float fYawR = DEGtoRAD( fYawD );
			D3DXQUATERNION qtRotate;
			D3DXQuaternionRotationYawPitchRoll( &qtRotate, fYawR, 0.0f, 0.0f );

			//���� ����
			D3DXVECTOR3 vDir = pOwner->GetMoveDir();
			//��ǥ ����
			D3DXVECTOR3 vTargetDir = qtRotate * D3DXVECTOR3( 0.0f, 0.0f, -FLOAT1 );

			m_bTargetRot = false;
			m_bLeftRot = false;

			//ȸ�� ���� ����
			if( D3DXVec3Dot( &vTargetDir, &vDir ) > -FLOAT1 )
			{
				float fTargetYaw;
				fTargetYaw = RADtoDEG( ioMath::QuaterToYaw( qtRotate ) );
				fTargetYaw = ioMath::ArrangeHead( fTargetYaw );

				float fRotAngle = ioMath::AngleGap( fCurYaw, fTargetYaw );
				if( fRotAngle < 0.0f )
					m_bLeftRot = true;
			}
			bSendRotate = true;	
		}
	}
	else
	{
		if( m_CurDirKey != ioUserKeyInput::DKI_NONE )
		{
			m_CurDirKey = ioUserKeyInput::DKI_NONE;
			bSendRotate = true;
		}
	}

	if( pOwner->IsNeedSendNetwork() )
	{
		if ( bSendRun )
		{
			SP2Packet kPacket( CUPK_SKILL_EXTRA_INFO );
			kPacket << pOwner->GetCharName();
			kPacket << GetName();
			kPacket << SS_SWAPRUN;
			kPacket << m_bIsRun;
			P2PNetwork::SendToAllPlayingUser( kPacket );
		}

		if ( bSendRotate )
		{
			D3DXQUATERNION qtRot = pOwner->GetTargetRot();
			SP2Packet kPacket( CUPK_SKILL_EXTRA_INFO );
			kPacket << pOwner->GetCharName();
			kPacket << GetName();
			kPacket << SS_ROTATE;
			kPacket << (int)m_CurDirKey;
			kPacket << m_bLeftRot;
			kPacket << qtRot;
			P2PNetwork::SendToAllPlayingUser( kPacket );
		}
	}
}

//���� ȸ��
void ioRunningAttackSkill5::ProcessRotate( ioBaseChar *pOwner )
{
	if( !pOwner ) return;
	if( m_dwRotateTime <= 0 ) return;
	if( m_bTargetRot ) return;

	if( m_CurDirKey == ioUserKeyInput::DKI_NONE )
		return;

	float fFrameGap = g_FrameTimer.GetCurFrameGap();
	float fRate = fFrameGap / m_dwRotateTime;
	float fRotateAngle = FLOAT360 * fRate;

	D3DXQUATERNION qtCurRot = pOwner->GetTargetRot();
	//���� ����
	float fCurYaw;
	fCurYaw = RADtoDEG( ioMath::QuaterToYaw( qtCurRot ) );
	fCurYaw = ioMath::ArrangeHead( fCurYaw );

	// ��ǥ�� ����üũ �� ó��
	float fYawD = 180.0f + 45.0f * ( m_CurDirKey - ioUserKeyInput::DKI_UP ) + fCurYaw;
	fYawD = ioMath::ArrangeHead( fYawD );

	float fYawR = DEGtoRAD( fYawD );
	D3DXQUATERNION qtRotate;
	D3DXQuaternionRotationYawPitchRoll( &qtRotate, fYawR, 0.0f, 0.0f );

	//��ǥ ����
	float fTargetYaw;
	fTargetYaw = RADtoDEG( ioMath::QuaterToYaw( qtRotate ) );
	fTargetYaw = ioMath::ArrangeHead( fTargetYaw );

	float fRotAngle = ioMath::AngleGap( fCurYaw, fTargetYaw );
	if( fabs( fRotAngle ) < fRotateAngle )
	{
		m_bTargetRot = true;
		pOwner->SetTargetRotToRotate( qtRotate, true, false );
		return;
	}

	if( m_bLeftRot )
		fRotateAngle *= -1;

	fRotateAngle = ioMath::ArrangeHead( fRotateAngle );
	D3DXVECTOR3 vDir = pOwner->GetMoveDir();

	D3DXQUATERNION qtAngle;
	D3DXQuaternionRotationYawPitchRoll( &qtAngle, DEGtoRAD(fRotateAngle), 0.0f, 0.0f );

	vDir = qtAngle * vDir;
	ioMath::CalcDirectionQuaternion( qtAngle, -vDir );

	pOwner->SetTargetRotToRotate( qtAngle, true, false );
}

//�߰��� �Լ�////////////////////////////////////////////////////////////////////////////////////
//���� ����
void ioRunningAttackSkill5::SetOwnerBuffList( ioBaseChar *pChar, ioPlayStage* pStage )
{
	if ( !pChar )
		return;

	if ( !pStage )
		return;

	if( m_OwnerBuffList.empty() ) 
		return;

	int nBuffCnt = m_OwnerBuffList.size();

	//�ش� ������ ����
	for( int i=0; i < nBuffCnt; ++i )
	{
		ioHashString szBuffName = m_OwnerBuffList[i].m_BuffName;

		if( !szBuffName.IsEmpty() )
			pChar->AddNewBuff( szBuffName, pChar->GetCharName(), GetOwnerItem()->GetName(), this );	
	}
}

//���� Ȯ��
bool ioRunningAttackSkill5::CheckBuff( ioBaseChar *pChar )
{	
	if ( !m_szCheckBuff.IsEmpty() && !pChar->HasBuff( m_szCheckBuff ) )
	{
		SetRunEndState( pChar );
		return false;
	}

	return true;
}

//���� ����
void ioRunningAttackSkill5::CreateWeapon( ioBaseChar *pChar )
{
	if ( !pChar )
		return;

	const WeaponInfoList &vWeaponInfoList = GetAttribute().m_vWeaponInfoList;

	int nWeaponCnt = (int)vWeaponInfoList.size();
	int nWeaponIndex = pChar->IncreaseWeaponIndexBase();

	D3DXVECTOR3 vFirePos = pChar->GetWorldPosition();

	for (int i=0; i<nWeaponCnt; ++i)
	{
		pChar->SkillFireExplicit( vWeaponInfoList[i], GetName(), nWeaponIndex + i, vFirePos );
	}

}

//���� �ִϸ��̼� ����
void ioRunningAttackSkill5::SetLoopAnimation( ioBaseChar *pChar )
{
	if ( !pChar )
		return;

	ioEntityGroup *pGrp = pChar->GetGroup();
	int iAniID = pGrp->GetAnimationIdx( m_szLoopAnimation );
	if( iAniID == -1 )	return;

	DeleteEffect();

	ioEffect* pEffect = pChar->AttachEffect( m_szLoopEffect );
	if( pEffect )
		m_dwLoopEffectID = pEffect->GetUniqueID();
	
	pGrp->ClearAllLoopAni( FLOAT100, true );
	pGrp->ClearAllActionAni( FLOAT100, true );

	pGrp->SetLoopAni( iAniID, FLOAT100 );	
}

//������ �ִϸ��̼� ����
void ioRunningAttackSkill5::SetDelayAnimation( ioBaseChar *pChar )
{
	if ( !pChar )
		return;

	ioEntityGroup *pGrp = pChar->GetGroup();
	int iAniID = pGrp->GetAnimationIdx( m_szDelayAnimation );
	if( iAniID == -1 )	return;	

	DeleteEffect();

	ioEffect* pEffect = pChar->AttachEffect( m_szDelayEffect );
	if( pEffect )
		m_dwDelayEffectID = pEffect->GetUniqueID();
	
	pGrp->ClearAllLoopAni( FLOAT100, true );
	pGrp->ClearAllActionAni( FLOAT100, true );

	pGrp->SetLoopAni( iAniID, FLOAT100 );	
}

//����Ʈ ����
void ioRunningAttackSkill5::DeleteEffect()
{
	if ( m_dwDelayEffectID != -1)
	{
		ioEffect *pEffect = g_EffectFactory.GetEffect( m_dwDelayEffectID );
		if( pEffect )
		{
			pEffect->EndEffectForce();
			m_dwDelayEffectID = -1;
		}
	}

	if ( m_dwLoopEffectID != -1)
	{
		ioEffect *pEffect = g_EffectFactory.GetEffect( m_dwLoopEffectID );
		if( pEffect )
		{
			pEffect->EndEffectForce();
			m_dwLoopEffectID = -1;
		}
	}
}

//����Ű Ȯ��
bool ioRunningAttackSkill5::CheckDirKey( ioUserKeyInput::DirKeyInput CurDirKey )
{
	switch ( CurDirKey )
	{
	case ioUserKeyInput::DKI_LEFTUP:
	case ioUserKeyInput::DKI_LEFT:
	case ioUserKeyInput::DKI_RIGHTUP:
	case ioUserKeyInput::DKI_RIGHT:
		return true;
	}

	return false;
}
