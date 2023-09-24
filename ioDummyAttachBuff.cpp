
#include "stdafx.h"

#include "ioDummyAttachBuff.h"

#include "ioBaseChar.h"
#include "ioPlayStage.h"

#include "DummyCharDefine.h"

ioDummyAttachBuff::ioDummyAttachBuff()
{
	m_DummyInfoList.clear();
	m_dwCheckDuration = 0;
	m_dwCheckAreaWeapon = 0;
	m_dwCheckDummyWeapon = 0;
	m_dwCheckHornSoundTime = 0;
	m_dwCheckAttackSoundTime = 0;
	m_bStartGauge = false;
}

ioDummyAttachBuff::ioDummyAttachBuff( const ioDummyAttachBuff &rhs )
	: ioBuff( rhs ),
	  m_vDestroyDummyCharList( rhs.m_vDestroyDummyCharList ),
	  m_DummyCharList( rhs.m_DummyCharList ),
	  m_dwActiveDuration( rhs.m_dwActiveDuration ),
	  m_bSetActiveDuration( rhs.m_bSetActiveDuration ),
	  m_dwAreaWeaponTick( rhs.m_dwAreaWeaponTick ),
	  m_szAreaWeaponName( rhs.m_szAreaWeaponName ),
	  m_fAreaWeaponUseGauge( rhs.m_fAreaWeaponUseGauge ),
	  m_bSetDummyAttach( rhs.m_bSetDummyAttach ),
	  m_bSetAreaWeapon( rhs.m_bSetAreaWeapon ),
	  m_bSetDummyWeapon( rhs.m_bSetDummyWeapon ),
	  m_fDummyWeaponUseGauge( rhs.m_fDummyWeaponUseGauge ),
	  m_dwDummyWeaponTick( rhs.m_dwDummyWeaponTick ),
	  m_szHornSoundName( rhs.m_szHornSoundName ),
	  m_szAttackSoundName( rhs.m_szAttackSoundName ),
	  m_dwDelayHornSoundTime( rhs.m_dwDelayHornSoundTime ),
	  m_dwDelayAttackSoundTime( rhs.m_dwDelayAttackSoundTime )
{
	m_DummyInfoList.clear();
	m_dwCheckDuration = 0;
	m_dwCheckAreaWeapon = 0;
	m_dwCheckDummyWeapon = 0;
	m_dwCheckHornSoundTime = 0;
	m_dwCheckAttackSoundTime = 0;
	m_bStartGauge = false;
}

ioDummyAttachBuff::~ioDummyAttachBuff()
{
	m_vDestroyDummyCharList.clear();
	m_DummyCharList.clear();
	m_DummyInfoList.clear();
}

void ioDummyAttachBuff::LoadProperty( ioINILoader &rkLoader )
{
	char szBuf[MAX_PATH] = "";

	ioBuff::LoadProperty( rkLoader );
	//�ð� ����
	m_bSetActiveDuration = rkLoader.LoadBool_e( "set_duration", false );
	m_dwActiveDuration = rkLoader.LoadInt_e( "active_duration", 0);

	//���� ����
	rkLoader.LoadString_e( "horn_sound_name", "", szBuf, MAX_PATH );
	m_szHornSoundName = szBuf;
	rkLoader.LoadString_e( "attack_sound_name", "", szBuf, MAX_PATH );
	m_szAttackSoundName = szBuf;
	m_dwDelayHornSoundTime = rkLoader.LoadInt_e( "horn_sound_delay", 0);
	m_dwDelayAttackSoundTime = rkLoader.LoadInt_e( "attack_sound_delay", 0);

	//���� ���� ����
	m_bSetAreaWeapon = rkLoader.LoadBool_e( "set_area_weapon_use",false );
	rkLoader.LoadString_e( "area_weapon_name", "", szBuf, MAX_PATH );
	m_szAreaWeaponName = szBuf;
	m_dwAreaWeaponTick = rkLoader.LoadInt_e( "area_weapon_tick", 0);
	m_fAreaWeaponUseGauge = rkLoader.LoadFloat_e( "area_weapon_use_gauge", 0.0f );

	//���� ���� ����
	m_bSetDummyWeapon = rkLoader.LoadBool_e( "set_dummy_weapon_use",false );
	m_dwDummyWeaponTick = rkLoader.LoadInt_e( "dummy_weapon_tick", 0);
	m_fDummyWeaponUseGauge = rkLoader.LoadFloat_e( "dummy_weapon_use_gauge", 0.0f );
	m_bSetDummyAttach = rkLoader.LoadBool_e( "set_dummy_attach", false );

	m_DummyCharList.clear();
	m_vDestroyDummyCharList.clear();

	Help::LoadDummyCharCreateDestroyInfo( rkLoader, m_DummyCharList, m_vDestroyDummyCharList );
}

ioBuff* ioDummyAttachBuff::Clone()
{
	return new ioDummyAttachBuff( *this );
}

void ioDummyAttachBuff::StartBuff( ioBaseChar *pOwner )
{
	m_DummyInfoList.clear();

	ioBuff::StartBuff( pOwner );

	DWORD dwCurTime = FRAMEGETTIME();
	m_dwCheckDuration = dwCurTime;
	m_DummyInfoList.clear();
	m_dwCheckAreaWeapon = 0;
	m_dwCheckDummyWeapon = 0;
	m_dwCheckHornSoundTime = 0;
	m_dwDelayAttackSoundTime = 0;
	m_bStartGauge = false;
	CreateDummyChar();
}

bool ioDummyAttachBuff::ApplyStartBuff(ioBaseChar *pOwner, DWORD dwGlobalTime, DWORD dwGapTime )
{
	m_DummyInfoList.clear();

	if( !ioBuff::ApplyStartBuff( pOwner, dwGlobalTime, dwGapTime ) )
		return false;

	DWORD dwCurTime = FRAMEGETTIME();
	m_dwCheckDuration = dwCurTime;
	m_DummyInfoList.clear();
	m_dwCheckAreaWeapon = 0;
	m_dwCheckDummyWeapon = 0;
	m_dwCheckHornSoundTime = 0;
	m_dwDelayAttackSoundTime = 0;
	m_bStartGauge = false;

	return true;
}

void ioDummyAttachBuff::ProcessBuff( float fTimePerSec )
{	
	if ( m_bSetActiveDuration && ( m_dwCheckDuration + m_dwActiveDuration < FRAMEGETTIME()) )
	{
		m_pOwner->CheckUseBuffSkill( m_Name, false );
		SetReserveEndBuff();
		return;
	}

	//���� ���� ����
	if( (m_bStartGauge && !CheckSwitchType( fTimePerSec )) || !CheckDummy() )
	{
		m_pOwner->CheckUseBuffSkill( m_Name, false );
		SetReserveEndBuff();
		return;
	}		

	//���� ��ǥ ��ġ ����	
	ProcessDummyAttach();
	//Ű�Է�
	ProcessKeyInput();
}

void ioDummyAttachBuff::EndBuff()
{	
	ioBuff::EndBuff();

	m_DummyInfoList.clear();

	SetSkillGauge( 0.0f );
}

void ioDummyAttachBuff::ApplyExtraBuffInfo( SP2Packet &rkPacket )
{
	ioBaseChar *pOwner = GetOwner();
	if( !pOwner )
		return;
	
	ioPlayStage *pStage = pOwner->GetCreator();
	if ( !pStage )
		return;

	int iState;
	rkPacket >> iState;

	switch( iState )
	{
	case BS_CreateAreaWeapon:
		{
			for (int i=0; i<(int)m_DummyInfoList.size(); ++i)
			{	
				ioDummyChar *pDummyChar = g_DummyCharMgr.FindDummyChar( pOwner, m_DummyInfoList[i].nIndex );

				if ( pDummyChar )
				{
					D3DXVECTOR3 vPos = pDummyChar->GetWorldPosition();
					D3DXQUATERNION qtRot = pDummyChar->GetWorldOrientation();

					g_AreaWeaponMgr.CreateAreaWeapon( pOwner->GetCharName(), "", GetName(),	m_szAreaWeaponName,	vPos, qtRot, ioAreaWeapon::CT_NORMAL );
				}		
			}					

			pOwner->ReduceCurSkillGaugeBySkill( m_ItemName, m_fAreaWeaponUseGauge );
			
			if ( !m_bStartGauge )
			{
				m_bStartGauge = true;
				m_dwCheckTime = FRAMEGETTIME();
			}
		}
		break;
	case BS_CreateDummy:
		{
			if( m_DummyCharList.empty() )
				return;

			//���� ���� ���� (�߰� ���� ����)
			g_DummyCharMgr.DestroyDummyCharByNameList( pOwner, m_vDestroyDummyCharList );
			
			//���� ����
			D3DXVECTOR3 vDir = pOwner->GetTargetRot() * D3DXVECTOR3( 0.0f, 0.0f, -FLOAT1 );
			D3DXVec3Normalize( &vDir, &vDir );

			//���⿡ ���� ���� ���� ����
			D3DXVECTOR3 vSideDir;
			D3DXVec3Cross( &vSideDir, &ioMath::UNIT_Y, &vDir );
			D3DXVec3Normalize( &vSideDir, &vSideDir );

			int iCurIndex;
			int iSize = m_DummyCharList.size();
			for( int i=0; i < iSize; ++i )
			{
				//�ε��� ���
				rkPacket >> iCurIndex;

				//���� �̸� ���
				ioHashString szDummyCharName = m_DummyCharList[i].m_DummyCharName;
				
				//������ ����
				float fOffset = m_DummyCharList[i].m_fDummyCharOffset;
				float fSideOffset = m_DummyCharList[i].m_fDummyCharSideOffset;
				float fStartAngle = m_DummyCharList[i].m_fDummyCharStartAngle;

				//���� ��ġ
				D3DXVECTOR3 vTargetPos = pOwner->GetWorldPosition() + (fOffset * vDir);
				vTargetPos += fSideOffset * vSideDir;
				
				//���� ����
				ioDummyChar *pDummy = pStage->CreateDummyChar( szDummyCharName, iCurIndex,
					pOwner->GetCharName(),
					vTargetPos,
					fStartAngle,
					0, true );

				//���̿� ��ų �̸� ����
				if( pDummy && pOwner->GetCurActiveSkill() != NULL )
					pDummy->SetControlSkillName( pOwner->GetCurActiveSkill()->GetName() );

				//Ư�� ���̸�
				ioOwnerAttach4Dummy *pOwnerAttach4Dummy = ToOwnerAttach4Dummy(pDummy);
				if ( pOwnerAttach4Dummy)
				{
					//�� �� �����϶�
					if ( i == 0 )
					{
						pOwnerAttach4Dummy->SetHead( true );
					}
				}
				AddDummyInfo( iCurIndex );	
			}
		}
		break;
	case BS_CreateDummyWeapon:
		{
			CreateDummyWeapon();
		}
		break;
	case BS_PlayHornSound:
		{
			if( !m_szHornSoundName.IsEmpty() )
			{
				pOwner->PlaySound( m_szHornSoundName );
			}
		}
		break;
	case BS_PlayAttackSound:
		{
			if( !m_szAttackSoundName.IsEmpty() )
			{
				pOwner->PlaySound( m_szAttackSoundName );
			}
		}
		break;
	}
}

//�߰��� �Լ�//////////////////////////////////////////////////////////////////
//Ű�Է�
void ioDummyAttachBuff::ProcessKeyInput()
{
	ioBaseChar *pOwner = GetOwner();
	if( !pOwner || !pOwner->IsNeedProcess() )
		return;
	
	if( pOwner->IsAttackKey() )
	{
		//���� ���� ����
		if ( m_bSetAreaWeapon )
		{
			CreateAreaWeapon();
		}	
		//���� ���� ����
		if ( m_bSetDummyWeapon )
		{
			CreateDummyWeapon();
		}		
		//���� ���� ���
		PlayAttackSound();
	}

	//���� ���
	if ( pOwner->IsDefenseKey() )
	{
		PlayHornSound();
	}
}

//�Ҹ� ���
void ioDummyAttachBuff::PlayHornSound()
{
	ioBaseChar *pOwner = GetOwner();

	if( !pOwner )
		return;

	DWORD dwCurTime = FRAMEGETTIME();

	if ( m_dwCheckHornSoundTime + m_dwDelayHornSoundTime > dwCurTime )
		return;

	if( !m_szHornSoundName.IsEmpty() )
	{
		m_dwCheckHornSoundTime = dwCurTime;
		pOwner->PlaySound( m_szHornSoundName );
	}

	//��Ŷ����
	if ( pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_APPLY_EXTRA_BUFF_INFO );
		kPacket << pOwner->GetCharName();
		kPacket << GetName();
		kPacket << BS_PlayHornSound;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

void ioDummyAttachBuff::PlayAttackSound()
{
	ioBaseChar *pOwner = GetOwner();

	if( !pOwner )
		return;

	DWORD dwCurTime = FRAMEGETTIME();

	if ( m_dwCheckAttackSoundTime + m_dwDelayAttackSoundTime > dwCurTime )
		return;

	if( !m_szAttackSoundName.IsEmpty() )
	{
		m_dwCheckAttackSoundTime = dwCurTime;
		pOwner->PlaySound( m_szAttackSoundName );
	}

	//��Ŷ����
	if ( pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_APPLY_EXTRA_BUFF_INFO );
		kPacket << pOwner->GetCharName();
		kPacket << GetName();
		kPacket << BS_PlayAttackSound;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

//��ų������ ����
void ioDummyAttachBuff::SetSkillGauge( float fSkillGauge )
{
	//������ ���� ���
	ioBaseChar *pOwner = GetOwner();

	if( !pOwner )
		return;

	//������ �ش� �������� ��ų �������� �缳��
	pOwner->SetCurItemSkillGauge( m_ItemName, fSkillGauge );
}

//���� ���� ����
void ioDummyAttachBuff::CreateAreaWeapon()
{
	ioBaseChar *pOwner = GetOwner();
	if( !pOwner )
		return;
	
	DWORD dwCurTime = FRAMEGETTIME();

	if ( m_dwCheckAreaWeapon + m_dwAreaWeaponTick > dwCurTime )
		return;

	//������ ��ġ �������� ���� ���� ����
	for (int i=0; i<(int)m_DummyInfoList.size(); ++i)
	{	
		ioDummyChar *pDummyChar = g_DummyCharMgr.FindDummyChar( pOwner, m_DummyInfoList[i].nIndex );

		if ( pDummyChar )
		{
			D3DXVECTOR3 vPos = pDummyChar->GetWorldPosition();
			D3DXQUATERNION qtRot = pDummyChar->GetWorldOrientation();

			g_AreaWeaponMgr.CreateAreaWeapon( pOwner->GetCharName(), "", GetName(),	m_szAreaWeaponName,	vPos, qtRot, ioAreaWeapon::CT_NORMAL );
		}		
	}	

	m_dwCheckAreaWeapon = dwCurTime;

	if ( !m_bStartGauge )
	{
		m_bStartGauge = true;
		m_dwCheckTime = FRAMEGETTIME();
	}
		
	pOwner->ReduceCurSkillGaugeBySkill( m_ItemName, m_fAreaWeaponUseGauge );

	//��Ŷ����
	if ( pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_APPLY_EXTRA_BUFF_INFO );
		kPacket << pOwner->GetCharName();
		kPacket << GetName();
		kPacket << BS_CreateAreaWeapon;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

//���� ĳ���� ����
void ioDummyAttachBuff::CreateDummyChar()
{
	//������ ���� ���
	ioBaseChar *pOwner = GetOwner();
	ioPlayStage *pStage = pOwner->GetCreator();

	if( !pOwner || !pOwner->IsNeedProcess() || !pStage) 
		return;

	if( m_DummyCharList.empty() )
		return;

	//���� ���� ���� (�߰� ���� ����)
	g_DummyCharMgr.DestroyDummyCharByNameList( pOwner, m_vDestroyDummyCharList );
	
	//���� ����
	D3DXVECTOR3 vDir = pOwner->GetTargetRot() * D3DXVECTOR3( 0.0f, 0.0f, -FLOAT1 );
	D3DXVec3Normalize( &vDir, &vDir );

	//���⿡ ���� ���� ���� ����
	D3DXVECTOR3 vSideDir;
	D3DXVec3Cross( &vSideDir, &ioMath::UNIT_Y, &vDir );
	D3DXVec3Normalize( &vSideDir, &vSideDir );

	int iSize = m_DummyCharList.size();
	for( int i=0; i < iSize; ++i )
	{
		//���� �̸� ���
		ioHashString szDummyCharName = m_DummyCharList[i].m_DummyCharName;

		//���� ���� �ε��� ��ȣ
		int iCurIndex = g_DummyCharMgr.GetCurLocalIndex();

		//������ ����
		float fOffset = m_DummyCharList[i].m_fDummyCharOffset;
		float fSideOffset = m_DummyCharList[i].m_fDummyCharSideOffset;
		float fStartAngle = m_DummyCharList[i].m_fDummyCharStartAngle;

		//���� ��ġ
		D3DXVECTOR3 vTargetPos = pOwner->GetWorldPosition() + (fOffset * vDir);
		vTargetPos += fSideOffset * vSideDir;

		//���� ����
		ioDummyChar *pDummy = pStage->CreateDummyChar( szDummyCharName, iCurIndex,
			pOwner->GetCharName(),
			vTargetPos,
			fStartAngle,
			0, true );

		//���̿� ��ų �̸� ����
		if( pDummy && pOwner->GetCurActiveSkill() != NULL )
			pDummy->SetControlSkillName( pOwner->GetCurActiveSkill()->GetName() );

		//Ư�� ���̸�
		ioOwnerAttach4Dummy *pOwnerAttach4Dummy = ToOwnerAttach4Dummy(pDummy);
		if ( pOwnerAttach4Dummy)
		{
			//�� �� �����϶�
			if ( i == 0 )
			{
				pOwnerAttach4Dummy->SetHead( true );
			}
		}
		AddDummyInfo( iCurIndex );			
	}
	
	if ( pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_APPLY_EXTRA_BUFF_INFO );
		kPacket << pOwner->GetCharName();
		kPacket << GetName();
		kPacket << BS_CreateDummy;
		for (int i=0; i<(int)m_DummyInfoList.size(); ++i)
		{
			kPacket << m_DummyInfoList[i].nIndex;
		}
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

//���� Ȯ��
bool ioDummyAttachBuff::CheckDummy()
{
	ioBaseChar *pOwner = GetOwner();

	if ( !pOwner )
		return false;

	for (int i=0; i<(int)m_DummyInfoList.size(); ++i)
	{
		ioDummyChar *pDummyChar = g_DummyCharMgr.FindDummyChar( pOwner, m_DummyInfoList[i].nIndex );

		if ( !pDummyChar )
			return false;
	}
	return true;
}

//���� ��ġ ����
void ioDummyAttachBuff::ProcessDummyAttach()
{
	if ( !m_pOwner )
		return;

	int nCnt = (int)m_DummyInfoList.size();
	for (int i=0; i<nCnt; ++i)
	{
		ioDummyChar *pDummyChar = g_DummyCharMgr.FindDummyChar( m_pOwner, m_DummyInfoList[i].nIndex );

		if ( !pDummyChar )
			continue;

		m_DummyInfoList[i].vPos = pDummyChar->GetWorldPosition();

		ioOwnerAttach4Dummy *pOwnerAttach4Dummy = ToOwnerAttach4Dummy( pDummyChar );
		if ( pOwnerAttach4Dummy)
		{
			if ( m_bSetDummyAttach )
			{
				if ( pOwnerAttach4Dummy->GetHead() )
				{
					pOwnerAttach4Dummy->SetTargetPos( m_pOwner->GetWorldPosition() );
				}
				else 
				{
					pOwnerAttach4Dummy->SetTargetPos( m_DummyInfoList[i-1].vPos );
				}
			}
			else
			{
				pOwnerAttach4Dummy->SetDontMove( true );
			}			
		}
	}
}

//���� ���� �߰�
void ioDummyAttachBuff::AddDummyInfo(int nIndex)
{
	DummyInfo CreateDummyInfo;
	CreateDummyInfo.nIndex = nIndex;
	CreateDummyInfo.vPos = ioMath::VEC3_ZERO;
	m_DummyInfoList.push_back( CreateDummyInfo );
}

//������ ���� �߻�
void ioDummyAttachBuff::CreateDummyWeapon()
{
	ioBaseChar *pOwner = GetOwner();

	if ( !pOwner )
		return;

	DWORD dwCurTime = FRAMEGETTIME();

	if ( m_dwCheckDummyWeapon + m_dwDummyWeaponTick > dwCurTime )
		return;

	for (int i=0; i<(int)m_DummyInfoList.size(); ++i)
	{
		ioDummyChar *pDummyChar = g_DummyCharMgr.FindDummyChar( pOwner, m_DummyInfoList[i].nIndex );
		ioOwnerAttach4Dummy *pOwnerAttachDummy = ToOwnerAttach4Dummy( pDummyChar );
		//���� �߻� ����
		if ( pOwnerAttachDummy )
		{
			pOwnerAttachDummy->SetFire( true );
		}
	}

	//������ �Ҹ�
	pOwner->ReduceCurSkillGaugeBySkill( m_ItemName, m_fDummyWeaponUseGauge );

	if ( !m_bStartGauge )
	{
		m_bStartGauge = true;
		m_dwCheckTime = FRAMEGETTIME();
	}

	m_dwCheckDummyWeapon = dwCurTime;

	//��Ŷ ����
	if ( pOwner->IsNeedProcess() && pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_APPLY_EXTRA_BUFF_INFO );
		kPacket << pOwner->GetCharName();
		kPacket << GetName();
		kPacket << BS_CreateDummyWeapon;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}