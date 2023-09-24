

#include "stdafx.h"

#include "ioPlayStage.h"
#include "ioBaseChar.h"

#include "ioBuffCountSkill.h"

ioBuffCountSkill::ioBuffCountSkill()
{
	Init();
}

ioBuffCountSkill::ioBuffCountSkill( const ioBuffCountSkill &rhs )
	: ioCountSkill( rhs ),
	m_dwSkillProtectTime( rhs.m_dwSkillProtectTime ),
	m_BuffInfoList( rhs.m_BuffInfoList ),
	m_bNoResetCount( rhs.m_bNoResetCount )
{
	Init();
}

ioBuffCountSkill::~ioBuffCountSkill()
{
}

void ioBuffCountSkill::Init()
{
	m_SkillState		= SS_PRE;	
	m_dwMotionEndTime	= 0;
	m_bSetHandMesh      = false;

	m_CurrBuffList.clear();

	m_nCurNeedCount = 0;
}

void ioBuffCountSkill::LoadProperty( ioINILoader &rkLoader )
{
	ioCountSkill::LoadProperty( rkLoader );

	char szBuf[MAX_PATH];
	char szKey[MAX_PATH];

	m_dwSkillProtectTime = rkLoader.LoadInt_e( "skill_protect_time", 0 );
	m_bNoResetCount = rkLoader.LoadBool_e( "no_reset_count", false );

	//������ �ʱ�ȭ
	m_iNeedCount = m_iMaxUseCount;

	int iCount = rkLoader.LoadInt_e( "buff_max", 0 );
	for( int i = 0; i < iCount; ++i )
	{
		BuffInfo Info;
		wsprintf_e( szKey, "buff%d_need_count", i+1 );
		Info.nNeedCount = rkLoader.LoadInt( szKey, 0 );

		//m_iNeedCount ���������� ����
		if( m_iNeedCount > Info.nNeedCount )
			m_iNeedCount = Info.nNeedCount;

		wsprintf_e( szKey, "buff%d_max", i+1 );
		int iMax = rkLoader.LoadInt( szKey, 0 );
		for( int j = 0; j < iMax; ++j )
		{
			wsprintf_e( szKey, "buff%d_name%d", i + 1, j + 1 );
			rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );
			Info.BuffNameList.push_back( szBuf );
		}

		m_BuffInfoList.push_back( Info );
	}

	std::sort( m_BuffInfoList.begin(), m_BuffInfoList.end(), BuffInfoSort() );
}

ioSkill* ioBuffCountSkill::Clone()
{
	return new ioBuffCountSkill( *this );		
}

int ioBuffCountSkill::ReduceNeedCount()
{
	if ( !m_bNoResetCount )
		m_iCurUseCount = 0;
	else
	{
		m_iCurUseCount -= m_nCurNeedCount;
		m_iCurUseCount = max( m_iCurUseCount, 0 );
	}

	return m_iCurUseCount;
}

bool ioBuffCountSkill::OnSkillStart( ioBaseChar *pOwner )
{
	Init();

	//���� ����Ʈ ����
	for( BuffInfoList::reverse_iterator iter = m_BuffInfoList.rbegin(); iter != m_BuffInfoList.rend(); ++iter )
	{
		const BuffInfo& Info = *iter;
		if( Info.nNeedCount <= m_iCurUseCount )
		{
			m_CurrBuffList = Info.BuffNameList;
			m_nCurNeedCount = Info.nNeedCount;
			break;
		}
	}
	
	//��ų ����
	if( !ioCountSkill::OnSkillStart(pOwner) )
		return false;

	if( m_bUseItemMesh )
		pOwner->ShowWeaponItemMesh( false );

	if( !m_bSetHandMesh && m_bUseItemMesh && !m_HandMesh.IsEmpty() )
	{
		pOwner->ChangeSkillWeaponMesh( this, m_HandMesh, m_bVisibleEquipMesh );
		m_bSetHandMesh = true;
	}

	return true;
}

void ioBuffCountSkill::OnProcessState( ioBaseChar *pOwner, ioPlayStage *pStage )
{
	switch( m_SkillState )
	{
	case SS_PRE: 
		{
			CheckPreState( pOwner, pStage );
		}
		break;
	case SS_ACTION:
		{
			if( 0 < m_dwMotionEndTime && m_dwMotionEndTime < FRAMEGETTIME() )
			{
				SetOwnerBuff( pOwner, pStage );
				m_SkillState = SS_END;
			}
		}
		break;
	}
}

void ioBuffCountSkill::CheckPreState( ioBaseChar *pOwner, ioPlayStage *pStage )
{
	if( !pOwner || !pOwner->IsNeedProcess() )
		return;

	DWORD dwCurTime  = FRAMEGETTIME();
	DWORD dwPreDelay = GetPreDelayTime();

	if( m_dwPreDelayStartTime + dwPreDelay < dwCurTime )
	{
		m_dwPreDelayStartTime = 0;
		EndPreDelayEffect( pOwner );
		SetSkillActionState( pOwner, pStage );
	}
}

void ioBuffCountSkill::SetSkillActionState( ioBaseChar* pOwner, ioPlayStage* pStage )
{
	m_SkillState = SS_ACTION;

	if( !pOwner )
		return;

	ioEntityGroup *pGrp = pOwner->GetGroup();
	if( !pGrp )
		return;

	int iAniID = pGrp->GetAnimationIdx( m_SkillMotionName );
	if( iAniID == -1 )
		return;

	pGrp->ClearAllLoopAni( 100.0f, true );
	pGrp->ClearAllActionAni( 100.0f, true );
	pGrp->SetActionAni( iAniID, 100.0f, 100.0f, 1.0f, 1.0f/m_fAnimationTimeRate );
	m_dwMotionEndTime += FRAMEGETTIME() + pGrp->GetAnimationFullTime( iAniID ) * m_fAnimationTimeRate;

	if( pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_SKILL_EXTRA_INFO );
		kPacket << pOwner->GetCharName();
		kPacket << GetName();
		kPacket << pOwner->GetTargetRot();
		kPacket << m_iCurUseCount;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

void ioBuffCountSkill::SetOwnerBuff( ioBaseChar* pOwner, ioPlayStage* pStage )
{
	for( int i = 0; i < (int)m_CurrBuffList.size(); ++i )
	{
		pOwner->AddNewBuff( m_CurrBuffList[i], pOwner->GetCharName(), "", this );
	}
}

void ioBuffCountSkill::OnSkillEnd( ioBaseChar *pOwner )
{
	ioCountSkill::OnSkillEnd( pOwner );

	pOwner->SetSkillMoveEnable( false, false );

	if( m_bUseItemMesh )
		pOwner->ShowWeaponItemMesh( true );

	if( !m_HandMesh.IsEmpty() )
		pOwner->RestoreSkillWeaponMesh( this, m_HandMesh, m_bVisibleEquipMesh );

	Init();
}

bool ioBuffCountSkill::IsSkillMotionEnd( DWORD dwActionStop, ioBaseChar* pOwner ) const
{
	if( m_dwPreDelayStartTime > 0 )
		return false;

	if( m_SkillState == SS_END )
		return true;

	return false;
}

void ioBuffCountSkill::ApplyExtraSkillInfo( ioBaseChar *pChar, ioPlayStage *pStage, SP2Packet &rkPacket )
{
	if( !pChar )
		return;

	D3DXQUATERNION qtRot;
	rkPacket >> qtRot;

	if( pChar )
		pChar->SetTargetRot( qtRot );
	
	rkPacket >> m_iCurUseCount;
	SetSkillActionState( pChar, pStage );
}

bool ioBuffCountSkill::IsProtected( int iDefenseBreakType ) const
{
	if( m_bDisableSkillProtect )
		return false;

	// ��ų ���� �ı�
	if( DBT_NONE != (DefenseBreakType)m_ProtectCancelType )
	{
		if( iDefenseBreakType == m_ProtectCancelType )
			return false;
	}

	if( IsPreDelayProtect() )
	{
		if( m_dwPreDelayStartTime != 0 )
			return true;
	}

	if( 0 < m_dwSkillProtectTime && FRAMEGETTIME() < m_dwSkillStartTime + m_dwSkillProtectTime )
		return true;

	return false;
}