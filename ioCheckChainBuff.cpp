
#include "stdafx.h"

#include "ioCheckChainBuff.h"

#include "WeaponDefine.h"

#include "ioPlayStage.h"
#include "ioBaseChar.h"

ioCheckChainBuff::ioCheckChainBuff()
{
	m_dwWeaponBaseIndex = 0;
	m_dwFireTime = 0;
	m_dwCheckAutoTime = 0;
	m_ChainList.clear();
	m_SortList.clear();
	m_bEnableAttack = false;
}

ioCheckChainBuff::ioCheckChainBuff( const ioCheckChainBuff &rhs )
	: ioBuff( rhs ),
	  m_fColAngle( rhs. m_fColAngle ),
	  m_fColRange( rhs.m_fColRange ),
	  m_szCheckBuffName( rhs.m_szCheckBuffName ),
	  m_nChainWeapon( rhs.m_nChainWeapon ),
	  m_nTotalChainCnt( rhs.m_nTotalChainCnt ),
	  m_bSetEnableInputKey( rhs.m_bSetEnableInputKey ),
	  m_szAttackAnimation( rhs.m_szAttackAnimation ),
	  m_fAttackTimeRate( rhs.m_fAttackTimeRate ),
	  m_nAttackWeapon( rhs.m_nAttackWeapon ),
	  m_dwAutoTime( rhs.m_dwAutoTime )
{	
	m_dwWeaponBaseIndex = 0;
	m_dwFireTime = 0;
	m_dwCheckAutoTime = 0;
	m_ChainList.clear();
	m_SortList.clear();
	m_bEnableAttack = false;
}

ioCheckChainBuff::~ioCheckChainBuff()
{	
}

ioBuff* ioCheckChainBuff::Clone()
{
	return new ioCheckChainBuff( *this );
}

void ioCheckChainBuff::LoadProperty( ioINILoader &rkLoader )
{
	ioBuff::LoadProperty( rkLoader );

	char szBuf[MAX_PATH] = "";
	m_fColAngle = rkLoader.LoadFloat_e( "target_angle", 0.0f );
	m_fColRange = rkLoader.LoadFloat_e( "target_range", 0.0f );
	rkLoader.LoadString_e( "check_buff", "", szBuf, MAX_PATH );
	m_szCheckBuffName = szBuf;

	m_nChainWeapon = rkLoader.LoadInt_e( "chain_weapon", NO_WEAPON_ATTRIBUTE );

	m_nTotalChainCnt = rkLoader.LoadInt_e( "max_chain_cnt", 0 );

	m_bSetEnableInputKey = rkLoader.LoadBool_e( "set_enable_input_key", false );

	rkLoader.LoadString_e( "attack_animation", "", szBuf, MAX_PATH );
	m_szAttackAnimation = szBuf;

	m_fAttackTimeRate = rkLoader.LoadFloat_e( "attack_rate", FLOAT1 );

	m_nAttackWeapon = rkLoader.LoadInt_e( "attack_weapon", NO_WEAPON_ATTRIBUTE );

	m_dwAutoTime = rkLoader.LoadInt_e( "auto_attack_time", 0 );
}

void ioCheckChainBuff::StartBuff( ioBaseChar *pOwner )
{
	ioBuff::StartBuff( pOwner );

	m_dwWeaponBaseIndex = 0;
	m_dwFireTime = 0;
	m_ChainList.clear();
	m_SortList.clear();
	m_bEnableAttack = false;
	m_dwCheckAutoTime = FRAMEGETTIME();
	m_BuffState = BS_START;
}

bool ioCheckChainBuff::ApplyStartBuff( ioBaseChar *pOwner, DWORD dwGlobalTime, DWORD dwGapTime )
{
	if( !ioBuff::ApplyStartBuff( pOwner, dwGlobalTime, dwGapTime ) )
	{
		return false;
	}

	m_dwWeaponBaseIndex = 0;
	m_dwFireTime = 0;
	m_ChainList.clear();
	m_SortList.clear();
	m_bEnableAttack = false;
	m_dwCheckAutoTime = FRAMEGETTIME();
	m_BuffState = BS_START;

	return true;
}

void ioCheckChainBuff::ProcessBuff( float fTimePerSec )
{	
	ioBuff::ProcessBuff( fTimePerSec );
	
	switch( m_BuffState )
	{
	case BS_START:
		{
			CreateChain( m_pOwner );
		}
		break;
	case BS_WAIT:
		{
			CheckTarget( m_pOwner );
			CheckAutoAttack( m_pOwner );
			CheckCharState( m_pOwner );
		}
		break;
	case BS_EXPLOSION:
		{
			CheckCreateWeapon( m_pOwner );
		}
		break;
	}
}

void ioCheckChainBuff::ApplyExtraBuffInfo( SP2Packet &rkPacket )
{
	ioBaseChar *pOwner = GetOwner();
	if( !pOwner )
		return;
	
	int nBuffState;
	rkPacket >> nBuffState;

	switch( (BuffState)nBuffState )
	{
	case BS_CreateChain:
		{
			//�ε��� ����ȭ
			rkPacket >> m_dwWeaponBaseIndex;

			for ( int i=0; i<m_nTotalChainCnt; i++ )
			{
				//������� (���� �޷��ִ� ����)
				FireTime kFireTime;
				kFireTime.dwStart = FRAMEGETTIME();

				kFireTime.iAttributeIdx = m_nChainWeapon;
				kFireTime.iResistanceIdx = 0;
				kFireTime.szWoundedAni  = "";
				kFireTime.dwWoundedAniDuration = 0;
				kFireTime.bLoopWoundedAni = false;
				kFireTime.dwWeaponIdx = m_dwWeaponBaseIndex++;
				kFireTime.eFireTimeType = FTT_NORMAL_ATTACK;

				D3DXVECTOR3 vPos = pOwner->GetMidPositionByRate();
				D3DXVECTOR3 vDir = pOwner->GetTargetRot() * D3DXVECTOR3( 0, 0, -1 );

				ioWeapon *pWeapon = pOwner->ExtendFireExplicit( kFireTime, vPos, vDir, "" );	
				if ( pWeapon )
				{
					ioUroborusWeapon4 *pChainWeapon = ToUroborusWeapon4( pWeapon );
					if ( pChainWeapon )
					{
						pChainWeapon->SetVisible( false );

						Chain sChain;
						sChain.nWeaponIndex = pWeapon->GetWeaponIndex();
						sChain.bSetVisible = false;
						sChain.szTargetName = "";

						m_ChainList.push_back( sChain );
					}
				}
			}
			m_BuffState = BS_WAIT;
		}
		break;
	case BS_CreateWeapon:
		{
			//�ε��� ����ȭ
			rkPacket >> m_dwWeaponBaseIndex;

			int nTotalCnt;
			rkPacket >> nTotalCnt;
			
			DWORD dwCurTime = FRAMEGETTIME();
			for ( int i=0; i< nTotalCnt; i++)
			{				
				FireTime kFireTime;
				kFireTime.dwStart = dwCurTime;

				kFireTime.iAttributeIdx = m_nAttackWeapon;
				kFireTime.iResistanceIdx = 0;
				kFireTime.szWoundedAni  = "";
				kFireTime.dwWoundedAniDuration = 0;
				kFireTime.bLoopWoundedAni = false;
				kFireTime.dwWeaponIdx = m_dwWeaponBaseIndex++;
				kFireTime.eFireTimeType = FTT_NORMAL_ATTACK;

				D3DXVECTOR3 vPos;
				rkPacket >> vPos;
				D3DXVECTOR3 vDir = ioMath::VEC3_ZERO;

				pOwner->ExtendFireExplicit( kFireTime, vPos, vDir, "" );
			}

			//Ȯ�� ���� ����
			RemoveCheckBuff( pOwner );

			//���� �� ���� ����
			SetReserveEndBuff();
		}
		break;
	case BS_AutoAttack:
		{
			bool bEnableAttack;
			rkPacket >> bEnableAttack;

			if ( bEnableAttack )
			{
				SetAttackTime( pOwner, m_szAttackAnimation, m_fAttackTimeRate );		
			}
			else 
			{
				SetReserveEndBuff();
			}
		}
		break;
	}
}

void ioCheckChainBuff::EndBuff()
{
	ioBuff::EndBuff();

	ClearList( GetOwner() );
}

//ü�� ����
void ioCheckChainBuff::CreateChain( ioBaseChar *pOwner )
{
	if ( !pOwner || !pOwner->IsNeedProcess() )
		return;

	DWORD dwWeapon = pOwner->IncreaseWeaponIndexBase();
	m_dwWeaponBaseIndex = dwWeapon;

	for ( int i=0; i<m_nTotalChainCnt; i++ )
	{
		//������� (���� �޷��ִ� ����)
		FireTime kFireTime;
		kFireTime.dwStart = FRAMEGETTIME();

		kFireTime.iAttributeIdx = m_nChainWeapon;
		kFireTime.iResistanceIdx = 0;
		kFireTime.szWoundedAni  = "";
		kFireTime.dwWoundedAniDuration = 0;
		kFireTime.bLoopWoundedAni = false;
		kFireTime.dwWeaponIdx = m_dwWeaponBaseIndex++;
		kFireTime.eFireTimeType = FTT_NORMAL_ATTACK;

		D3DXVECTOR3 vPos = pOwner->GetMidPositionByRate();
		D3DXVECTOR3 vDir = pOwner->GetTargetRot() * D3DXVECTOR3( 0, 0, -1 );

		ioWeapon *pWeapon = pOwner->ExtendFireExplicit( kFireTime, vPos, vDir, "" );	
		if ( pWeapon )
		{
			ioUroborusWeapon4 *pChainWeapon = ToUroborusWeapon4( pWeapon );
			if ( pChainWeapon )
			{
				pChainWeapon->SetVisible( false );

				Chain sChain;
				sChain.nWeaponIndex = pWeapon->GetWeaponIndex();
				sChain.bSetVisible = false;
				sChain.szTargetName = "";
				m_ChainList.push_back( sChain );
			}
		}
	}

	if( pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_APPLY_EXTRA_BUFF_INFO );
		kPacket << pOwner->GetCharName();
		kPacket << GetName();
		kPacket << BS_CreateChain;
		kPacket << dwWeapon;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}

	m_BuffState = BS_WAIT;
}

//���� Ÿ�ٿ��� ǥ�ù����� �ִ°� Ȯ��
void ioCheckChainBuff::CheckTarget( ioBaseChar *pOwner )
{		
	if( !pOwner || m_ChainList.empty() ) 
		return;

	ioPlayStage *pStage = pOwner->GetCreator();
	if ( !pStage )
		return;

	//Ÿ�� ����Ʈ
	BaseCharList vTargetList;

	//��ġ ����
	D3DXVECTOR3 vPos = pOwner->GetWorldPosition();

	//Ÿ�� ����Ʈ ���
	pOwner->GetTargetCharListBySphere( vPos, m_fColRange, m_fColAngle, vTargetList );

	//���� �Ұ��� ����
	m_bEnableAttack = false;

	//Ÿ�� ����Ʈ�� ����ִٸ� ����
	if ( vTargetList.empty() )
		return;

	//Ÿ�� ����Ʈ ����
	SortTargetList( pOwner, vTargetList );

	//ü�� ����
	SetChain( pOwner );
}

//Ÿ�� ����Ʈ ����� ������ ����
void ioCheckChainBuff::SortTargetList( ioBaseChar *pOwner, const BaseCharList &TargetList )
{
	if( !pOwner || TargetList.empty() )	
		return;

	m_SortList.erase( m_SortList.begin(), m_SortList.end() );
	m_SortList.reserve( TargetList.size() );

	D3DXVECTOR3 vDiff;
	D3DXVECTOR3 vOwnerPos = m_pOwner->GetWorldPosition();

	BaseCharList::const_iterator iter;
	for( iter=TargetList.begin(); iter!=TargetList.end(); ++iter )
	{
		ioBaseChar *pCurChar = *iter;
		if( pCurChar != m_pOwner )
		{
			vDiff = pCurChar->GetWorldPosition() - vOwnerPos;
			float fCurDistSq = D3DXVec3LengthSq( &vDiff );

			DistInfo sDist;
			sDist.szName = pCurChar->GetCharName();
			sDist.fDistSq = fCurDistSq;
			sDist.vMidPos = pCurChar->GetMidPositionByRate();
			m_SortList.push_back( sDist );
		}
	}

	std::sort( m_SortList.begin(), m_SortList.end() );
}

//ü�� ����
void ioCheckChainBuff::SetChain( ioBaseChar *pOwner )
{
	if ( !pOwner || m_ChainList.empty() )
		return;

	ioPlayStage *pPlayStage = pOwner->GetCreator();
	if ( !pPlayStage )
		return;

	int nChainCnt = 0;
	int nTargetSortCnt = (int)m_SortList.size();
	for ( int i=0; i<nTargetSortCnt; i++ )
	{	
		ioBaseChar *pTarget = pPlayStage->GetBaseChar( m_SortList[i].szName );
		if ( !pTarget )
			continue;

		//���� ���̸� �н�
		if ( pOwner->GetTeam() == pTarget->GetTeam() )
			continue;

		/*if ( !IsEnableTarget( pTarget ) )
			continue;*/

		//Ȯ�� ������ ������ ���� Ÿ�ٸ� Ȯ��
		if ( pTarget->IsHasBuffCheckCreator( m_szCheckBuffName, pOwner->GetCharName() ) )
		{			
			//������� ü�� ���� �ִ뺸�� ������ ���̰� ����
			if ( nChainCnt < m_nTotalChainCnt )
			{
				Chain &sChain = m_ChainList[nChainCnt];
				ioWeapon *pWeapon = g_WeaponMgr.FindWeapon( pOwner, sChain.nWeaponIndex );
				ioUroborusWeapon4 *pChainWeapon = ToUroborusWeapon4( pWeapon );
				if ( pChainWeapon )
				{
					//��� ���̰� �ϴ� �Լ��� �θ��� �ʵ��� ó��
					if ( !sChain.bSetVisible && pChainWeapon->IsHasArrow() )
					{
						pChainWeapon->SetVisible( true );
						sChain.bSetVisible = true;
					}

					//Ÿ�� ����
					sChain.szTargetName = m_SortList[i].szName;

					//��ġ ���� �� ī��Ʈ ����
					pChainWeapon->SetPosition( m_SortList[i].vMidPos );
					nChainCnt++;
					m_bEnableAttack = true;
				}	
			}
		}
	}

	//������ ������ �Ⱥ��̰�
	if ( nChainCnt < m_nTotalChainCnt )
	{
		for ( int i=nChainCnt; i<m_nTotalChainCnt; i++ )
		{
			Chain &sChain = m_ChainList[i];

			ioWeapon *pWeapon = g_WeaponMgr.FindWeapon( pOwner, sChain.nWeaponIndex );
			ioUroborusWeapon4 *pChainWeapon = ToUroborusWeapon4( pWeapon );
			if ( pChainWeapon )
			{
				//��� �Ⱥ��̰� �ϴ� �Լ��� �θ��� �ʵ��� ó��
				if ( sChain.bSetVisible && pChainWeapon->IsHasArrow() )
				{
					pChainWeapon->SetVisible( false );
					sChain.bSetVisible = false;
					//Ÿ�� ����
					sChain.szTargetName = "";
				}
			}
		}
	}
}

//Ÿ�� �������� Ȯ��
bool ioCheckChainBuff::IsEnableTarget( ioBaseChar *pTarget )
{
	if ( !pTarget )
		return false;

	CharState eTargetState = pTarget->GetState();
	switch( eTargetState )
	{
	case CS_ETC_STATE:
		return false;
	}

	return true;
}

//�ڵ� ���� Ȯ��
void ioCheckChainBuff::CheckAutoAttack( ioBaseChar *pOwner )
{
	if ( !pOwner || !pOwner->IsNeedProcess() || m_bSetEnableInputKey )
		return;

	//�ð� Ȯ��
	if ( m_dwCheckAutoTime + m_dwAutoTime > FRAMEGETTIME() )
		return;
	
	if ( m_bEnableAttack )
	{
		//���� �����ϸ� �ִϸ��̼� ����	
		SetAttackTime( pOwner, m_szAttackAnimation, m_fAttackTimeRate );
	}
	else 
	{
		//���� ����
		SetReserveEndBuff();
	}

	if( pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_APPLY_EXTRA_BUFF_INFO );
		kPacket << pOwner->GetCharName();
		kPacket << GetName();
		kPacket << BS_AutoAttack;
		kPacket << m_bEnableAttack;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

//ĳ���� ���� Ȯ��
void ioCheckChainBuff::CheckCharState( ioBaseChar *pOwner )
{
	if ( !pOwner || !m_bSetEnableInputKey )
		return;
		
	if ( IsEnableAttack( pOwner ) )
	{
		if ( m_bEnableAttack )
		{
			//���� �����ϸ鼭 Ű�Է½� �ִϸ��̼� ����
			SetAttackTime( pOwner, m_szAttackAnimation, m_fAttackTimeRate );
		}
		else 
		{
			SetReserveEndBuff();
			return;
		}
	}	
}

//���� ������ �������� Ȯ��
bool ioCheckChainBuff::IsEnableAttack( ioBaseChar *pOwner )
{
	if ( !pOwner )
		return false;

	CharState eOwnerState = pOwner->GetState();
	switch( eOwnerState )
	{
	case CS_EVY_BOMB_STATE:
		return true;
	}

	return false;
}

//�ִϸ��̼� �ð� ���
void ioCheckChainBuff::SetAttackTime( ioBaseChar *pOwner, ioHashString szName, float fRate )
{
	if ( !pOwner || szName.IsEmpty() )
		return;

	ioEntityGroup *pGroup = pOwner->GetGroup();
	int iAniID = pGroup->GetAnimationIdx( szName );
	if( iAniID == -1 ) 
		return;

	//���� ���� �ð� ����
	DWORD dwCurTime = FRAMEGETTIME();
	m_dwFireTime = dwCurTime;
	m_dwFireTime += pGroup->GetAnimationEventTime_e( iAniID, "fire_s" ) * fRate;

	m_BuffState = BS_EXPLOSION;
}

//���� ����
void ioCheckChainBuff::CheckCreateWeapon( ioBaseChar *pOwner )
{
	if ( !pOwner || !pOwner->IsNeedProcess() || m_ChainList.empty() )
		return;

	ioPlayStage *pPlayStage = pOwner->GetCreator();
	if ( !pPlayStage )
		return;

	//�ð� Ȯ��
	DWORD dwCurTime = FRAMEGETTIME();
	if ( m_dwFireTime == 0 || m_dwFireTime > dwCurTime )
		return;

	DWORD dwWeapon = pOwner->IncreaseWeaponIndexBase();
	m_dwWeaponBaseIndex = dwWeapon;

	std::vector<D3DXVECTOR3> vPosList;
	vPosList.clear();
	
	for (int i=0; i<m_nTotalChainCnt && i < (int)m_ChainList.size(); i++)
	{
		const Chain &sChain = m_ChainList[i];

		//Ÿ���� �ִٸ�
		if ( !sChain.szTargetName.IsEmpty() )
		{
			ioBaseChar *pTarget = pPlayStage->GetBaseChar( sChain.szTargetName );
			if ( pTarget )
			{
				FireTime kFireTime;
				kFireTime.dwStart = dwCurTime;

				kFireTime.iAttributeIdx = m_nAttackWeapon;
				kFireTime.iResistanceIdx = 0;
				kFireTime.szWoundedAni  = "";
				kFireTime.dwWoundedAniDuration = 0;
				kFireTime.bLoopWoundedAni = false;
				kFireTime.dwWeaponIdx = m_dwWeaponBaseIndex++;
				kFireTime.eFireTimeType = FTT_NORMAL_ATTACK;

				D3DXVECTOR3 vPos = pTarget->GetMidPositionByRate();
				D3DXVECTOR3 vDir = ioMath::VEC3_ZERO;

				pOwner->ExtendFireExplicit( kFireTime, vPos, vDir, "" );

				vPosList.push_back( vPos );
			}
		}
	}

	if( pOwner->IsNeedSendNetwork() )
	{
		int nTotalCnt = (int)vPosList.size();
		SP2Packet kPacket( CUPK_APPLY_EXTRA_BUFF_INFO );
		kPacket << pOwner->GetCharName();
		kPacket << GetName();
		kPacket << BS_CreateWeapon;
		kPacket << dwWeapon;
		kPacket << nTotalCnt;

		for ( int i=0; i< nTotalCnt; i++)
		{
			kPacket << vPosList[i];
		}
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}

	vPosList.erase( vPosList.begin(), vPosList.end() );
	vPosList.clear();

	//Ȯ�� ���� ����
	RemoveCheckBuff( pOwner );
	
	//���� �� ���� ����
	SetReserveEndBuff();
}

//����� Ÿ�ٸ� Ȯ�� ���� ����
void ioCheckChainBuff::RemoveCheckBuff( ioBaseChar *pOwner )
{
	if ( !pOwner || m_ChainList.empty() )
		return;

	ioPlayStage *pPlayStage = pOwner->GetCreator();
	if ( !pPlayStage )
		return;

	for (int i=0; i<m_nTotalChainCnt && i < (int)m_ChainList.size(); i++)
	{
		const Chain &sChain = m_ChainList[i];

		//Ÿ���� �ִٸ�
		if ( !sChain.szTargetName.IsEmpty() )
		{
			ioBaseChar *pTarget = pPlayStage->GetBaseChar( sChain.szTargetName );
			if ( pTarget )
				pTarget->RemoveBuffOwnerName( m_szCheckBuffName, pOwner->GetCharName() );
		}
	}
}

//����Ʈ ����
void ioCheckChainBuff::ClearList( ioBaseChar *pOwner )
{
	if ( pOwner && !m_ChainList.empty() )
	{
		for( int i=0; i<m_nTotalChainCnt; i++)
		{
			const Chain &sChain = m_ChainList[i];
			ioWeapon *pWeapon = g_WeaponMgr.FindWeapon( pOwner, sChain.nWeaponIndex );
			if ( pWeapon )
			{
				pWeapon->SetWeaponDead();
			}
		}

		m_ChainList.erase( m_ChainList.begin(), m_ChainList.end() );
	}
	m_ChainList.clear();

	m_SortList.erase( m_SortList.begin(), m_SortList.end() );
	m_SortList.clear();
}