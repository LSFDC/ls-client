

#include "stdafx.h"

#include "ioPlayStage.h"
#include "ioBaseChar.h"

#include "ioCreateDummyChar7Buff.h"

#include "DummyCharDefine.h"

ioCreateDummyChar7Buff::ioCreateDummyChar7Buff()
{
	m_bCreatedDummy = false;
}

ioCreateDummyChar7Buff::ioCreateDummyChar7Buff( const ioCreateDummyChar7Buff &rhs )
	: ioBuff( rhs ),
	m_bDisableDownState( rhs.m_bDisableDownState ),
	m_fAlphaRate( rhs.m_fAlphaRate ),
	m_bNoWoundState( rhs.m_bNoWoundState ),
	m_bHideMiniMap( rhs.m_bHideMiniMap ),
	m_bHideFootDustEffect( rhs.m_bHideFootDustEffect ),
	m_bCheckCamera( rhs.m_bCheckCamera ),
	m_DummyCharList( rhs.m_DummyCharList ),
	m_vDestroyDummyCharList( rhs.m_vDestroyDummyCharList ),
	m_bEnableWeaponSkill( rhs.m_bEnableWeaponSkill ),
	m_bEnableArmorSkill( rhs.m_bEnableArmorSkill ),
	m_bEnableHelmetSkill( rhs.m_bEnableHelmetSkill ),
	m_bEnableCloakSkill( rhs.m_bEnableCloakSkill )
{
}

ioCreateDummyChar7Buff::~ioCreateDummyChar7Buff()
{
}

void ioCreateDummyChar7Buff::LoadProperty( ioINILoader &rkLoader )
{
	ioBuff::LoadProperty( rkLoader );

	m_fAlphaRate = rkLoader.LoadFloat_e( "alpha_rate", 1.0f );
	m_bNoWoundState = rkLoader.LoadBool_e( "no_wound_state", false );
	m_bHideMiniMap = rkLoader.LoadBool_e( "hide_minimap", false );
	m_bDisableDownState  = rkLoader.LoadBool_e( "disable_down_state", false );
	m_bHideFootDustEffect = rkLoader.LoadBool_e( "hide_foot_dust_effect", false );
	m_bCheckCamera = rkLoader.LoadBool_e( "check_camera", false );

	m_bEnableWeaponSkill = rkLoader.LoadBool_e( "enable_weapon_skill", false );
	m_bEnableArmorSkill = rkLoader.LoadBool_e( "enable_armor_skill", false );
	m_bEnableHelmetSkill = rkLoader.LoadBool_e( "enable_helmet_skill", false );
	m_bEnableCloakSkill = rkLoader.LoadBool_e( "enable_cloak_skill", false );

	m_DummyCharList.clear();
	m_vDestroyDummyCharList.clear();

	Help::LoadDummyCharCreateDestroyInfo( rkLoader, m_DummyCharList, m_vDestroyDummyCharList );
}

ioBuff* ioCreateDummyChar7Buff::Clone()
{
	return new ioCreateDummyChar7Buff( *this );
}

void ioCreateDummyChar7Buff::StartBuff( ioBaseChar *pOwner )
{
	ioBuff::StartBuff( pOwner );

	int iAlphaRate = m_fAlphaRate * MAX_ALPHA_RATE;
	iAlphaRate = min( iAlphaRate, MAX_ALPHA_RATE );
	iAlphaRate = max( iAlphaRate, 0 );
	if( iAlphaRate < MAX_ALPHA_RATE )
	{
		m_pOwner->SetAlphaRateDirect( iAlphaRate );
	}

	if( m_bNoWoundState )
		pOwner->SetNoWoundState( true );

	m_bCreatedDummy = false;

	if( m_bHideFootDustEffect )
	{
		pOwner->SetHideFootDustEffect( true );
	}
	m_iCurActiveCount = m_iMaxActiveCount;
}

bool ioCreateDummyChar7Buff::ApplyStartBuff(ioBaseChar *pOwner, DWORD dwGlobalTime, DWORD dwGapTime )
{
	if( !ioBuff::ApplyStartBuff( pOwner, dwGlobalTime, dwGapTime ) )
		return false;

	int iAlphaRate = m_fAlphaRate * MAX_ALPHA_RATE;
	iAlphaRate = min( iAlphaRate, MAX_ALPHA_RATE );
	iAlphaRate = max( iAlphaRate, 0 );
	if( iAlphaRate < MAX_ALPHA_RATE )
	{
		m_pOwner->SetHideChar( true );
		m_pOwner->SetAlphaRateDirect( iAlphaRate );
	}

	if( m_bNoWoundState )
		pOwner->SetNoWoundState( true );

	if( m_bHideFootDustEffect )
	{
		pOwner->SetHideFootDustEffect( true );
	}

	return true;
}

void ioCreateDummyChar7Buff::ProcessBuff( float fTimePerSec )
{
	if( CheckOwnerStateCheck() )
		return;

	if( m_bNoWoundState && !m_pOwner->IsCanNoWound() )
	{
		SetReserveEndBuff();
		return;
	}

	switch( m_iOperationType )
	{
	case OT_SWITCH:
		if( !CheckSwitchType( fTimePerSec ) )
		{
			if( GetOwner() && CheckRestoreStateCheck() )
				GetOwner()->SetState( CS_DELAY, m_bSendDelayNetWork );

			m_pOwner->CheckUseBuffSkill( m_Name, false );
			SetReserveEndBuff();
			return;
		}

		CheckCallingProcess();
		break;
	case OT_DELAY:
	case OT_ACTIVE:
		{
			if( CheckEnableDelayState() )
				return;

			CheckCallingProcess();

			if( GetBuffDuration() == 0 )
				return;

			if( m_dwBuffEndTime <= FRAMEGETTIME() )
			{
				if( CheckRestoreStateCheck() )
				{
					m_pOwner->SetState( CS_DELAY, m_bSendDelayNetWork );
				}

				m_pOwner->CheckUseBuffSkill( m_Name, false );
				SetReserveEndBuff();
				return;
			}
		}
		break;
	case OT_PASSIVE:
		CheckCallingAreaWeapon();
		break;
	}

	if( GetOwner() && GetOwner()->IsNeedProcess() && !m_bCreatedDummy )
	{
		CreateDummyChar( GetOwner() );
		m_bCreatedDummy = true;
	}

	//if( m_bCreatedDummy )
		//CheckDummyChar();

	//SetReserveEndBuff();
}

bool ioCreateDummyChar7Buff::CheckOwnerStateCheck()
{
	if( !m_pOwner ) return false;

	CharState eState = m_pOwner->GetState();

	bool bDown = false;
	if( m_bDisableDownState )
	{
		if( eState == CS_FROZEN && m_pOwner->GetBlowFrozenState() )
			bDown = true;
		else if( eState == CS_BLOW_WOUNDED && !m_pOwner->IsFloatingState() )
			bDown = true;
		else if( eState == CS_BOUND_BLOW_WOUNDED && !m_pOwner->IsFloatingState() )
			bDown = true;
		else if( eState == CS_ETC_STATE && m_pOwner->IsApplyDownState(false) )
			bDown = true;
		else if( eState == CS_STOP_MOTION && m_pOwner->GetBlowStopMotionState() )
			bDown = true;
		else if( eState == CS_DROP_ZONE_DOWN )
			bDown = true;
		else if( eState == CS_ICE_STATE && m_pOwner->GetBlowStopMotionState() )
			bDown = true;

		if( bDown )
		{
			if( CheckRestoreStateCheck() )
			{
				m_pOwner->SetState( CS_DELAY, m_bSendDelayNetWork );
			}

			m_pOwner->CheckUseBuffSkill( m_Name, false );
			SetReserveEndBuff();
			return true;
		}
	}

	bool bWound = false;
	if( m_bEnableWoundCancel )
	{
		if( eState == CS_BLOW_WOUNDED )
			bWound = true;
		else if( eState == CS_BOUND_BLOW_WOUNDED )
			bWound = true;
		else if( eState == CS_WOUNDED )
			bWound = true;
		else if( eState == CS_ETC_STATE )
			bWound = true;

		if( bWound )
		{
			m_pOwner->CheckUseBuffSkill( m_Name, false );
			SetReserveEndBuff();
			return true;
		}
	}

	return false;
}

bool ioCreateDummyChar7Buff::CheckRestoreStateCheck()
{
	if( !m_pOwner ) return false;

	CharState eState = m_pOwner->GetState();

	switch( eState )
	{
	case CS_DELAY:
	case CS_RUN:
	case CS_DASH:
		return true;
	}

	return false;
}

void ioCreateDummyChar7Buff::EndBuff()
{
	/*ioBaseChar *pOwner = GetOwner();
	if( pOwner )
	{
	int iSize = m_CreateDummyList.size();
	for( int i=0; i<iSize; ++i )
	{
	ioDummyChar *pDummy = g_DummyCharMgr.FindDummyChar( pOwner, m_CreateDummyList[i].m_iDummyCharIdx );

	ioCopyOwnerDummyChar2 *pCopyOwnerDummy2 = ToCopyOwnerDummyChar2( pDummy );
	if( pCopyOwnerDummy2 )
	{
	pCopyOwnerDummy2->SetDieState( true );
	}
	}

	pOwner->SetAlphaRateDirect( MAX_ALPHA_RATE );

	if( m_bNoWoundState )
	pOwner->SetNoWoundState( false );

	if( m_bHideFootDustEffect )
	{
	pOwner->SetHideFootDustEffect( false );
	}
	}*/

	ioBuff::EndBuff();
}

void ioCreateDummyChar7Buff::GetBuffInfo( SP2Packet &rkPacket )
{
	ioBuff::GetBuffInfo(rkPacket);
	rkPacket << m_iCurActiveCount;

	int iSize = m_CreateDummyList.size();
	rkPacket << iSize;
	for( int i=0; i<iSize; ++i )
	{
		rkPacket << m_CreateDummyList[i].m_DummyCharName;
		rkPacket << m_CreateDummyList[i].m_iDummyCharIdx;
		rkPacket << m_CreateDummyList[i].m_fDummyCharStartAngle;
		rkPacket << m_CreateDummyList[i].m_vTargetPos;
	}
}

void ioCreateDummyChar7Buff::ApplyBuffInfo( SP2Packet &rkPacket )
{
	rkPacket >> m_iCurActiveCount;

	m_CreateDummyList.clear();
	int iSize;
	rkPacket >> iSize;
	for( int i=0; i<iSize; ++i )
	{
		DummyCharCreateInfo kInfo;
		rkPacket >> kInfo.m_DummyCharName;
		rkPacket >> kInfo.m_iDummyCharIdx;
		rkPacket >> kInfo.m_fDummyCharStartAngle;
		rkPacket >> kInfo.m_vTargetPos;
		m_CreateDummyList.push_back(kInfo);
	}
}

void ioCreateDummyChar7Buff::CreateDummyChar( ioBaseChar *pOwner )
{
	if( !pOwner ) return;
	ioPlayStage *pStage = pOwner->GetCreator();
	if( !pStage ) return;
	if( !pOwner->IsNeedProcess() )
		return;
	if( m_DummyCharList.empty() )
		return;

	g_DummyCharMgr.DestroyDummyCharByNameList( pOwner, m_vDestroyDummyCharList );

	m_CreateDummyList.clear();

	D3DXVECTOR3 vDir = pOwner->GetTargetRot() * D3DXVECTOR3( 0.0f, 0.0f, -1.0f );
	D3DXVec3Normalize( &vDir, &vDir );

	D3DXVECTOR3 vSideDir;
	D3DXVec3Cross( &vSideDir, &ioMath::UNIT_Y, &vDir );
	D3DXVec3Normalize( &vSideDir, &vSideDir );

	IORandom random;
	DWORD dwSeed = pOwner->GetRandomSeed();
	random.SetRandomSeed( dwSeed );

	int iSize = m_DummyCharList.size();
	int iException = random.Random( iSize );

	float fOffset = m_DummyCharList[iException].m_fDummyCharOffset;
	float fSideOffset = m_DummyCharList[iException].m_fDummyCharSideOffset;
	float fStartAngle = m_DummyCharList[iException].m_fDummyCharStartAngle;

	D3DXVECTOR3 vCheckTargetPos = pOwner->GetWorldPosition() + (fOffset * vDir);
	vCheckTargetPos += fSideOffset * vSideDir;

	float fDropCheckMapHeight = pStage->GetMapHeight( vCheckTargetPos.x, vCheckTargetPos.z );
	vCheckTargetPos.y = fDropCheckMapHeight;

	if( fDropCheckMapHeight < 10.f )//드랍존
	{
		//우선 나는 제자리에
		for( int i=0; i < iSize; ++i )
		{
			ioHashString szDummyCharName = m_DummyCharList[i].m_DummyCharName;
			int iCurIndex = g_DummyCharMgr.GetCurLocalIndex();
			float fOffset = m_DummyCharList[i].m_fDummyCharOffset;
			float fSideOffset = m_DummyCharList[i].m_fDummyCharSideOffset;
			float fStartAngle = m_DummyCharList[i].m_fDummyCharStartAngle;

			D3DXVECTOR3 vTargetPos = pOwner->GetWorldPosition() + (fOffset * vDir);
			vTargetPos += fSideOffset * vSideDir;

			float fMapHeight = pStage->GetMapHeight( vTargetPos.x, vTargetPos.z );
			if( fMapHeight < 10.f )
				continue;

			ioDummyChar *pDummy = pStage->CreateDummyChar( szDummyCharName, iCurIndex,
				pOwner->GetCharName(),
				vTargetPos,
				fStartAngle,
				0, true );

			DummyCharCreateInfo kInfo;
			kInfo.m_DummyCharName = szDummyCharName;
			kInfo.m_iDummyCharIdx = iCurIndex;
			kInfo.m_fDummyCharStartAngle = fStartAngle;
			kInfo.m_vTargetPos = vTargetPos;

			m_CreateDummyList.push_back( kInfo );
		}
	}
	else
	{
		//나는 더미 위치로
		D3DXVECTOR3 vMyOldPos = pOwner->GetWorldPosition();
		//더미중 exception 더미는 내 위치로 
		for( int i=0; i < iSize; ++i )
		{
			ioHashString szDummyCharName = m_DummyCharList[i].m_DummyCharName;
			int iCurIndex = g_DummyCharMgr.GetCurLocalIndex();
			float fOffset = m_DummyCharList[i].m_fDummyCharOffset;
			float fSideOffset = m_DummyCharList[i].m_fDummyCharSideOffset;
			float fStartAngle = m_DummyCharList[i].m_fDummyCharStartAngle;

			D3DXVECTOR3 vTargetPos = pOwner->GetWorldPosition() + (fOffset * vDir);
			vTargetPos += fSideOffset * vSideDir;

			vTargetPos.y= pStage->GetMapHeight( vTargetPos.x, vTargetPos.z );

			ioDummyChar *pDummy = NULL;
			if( i == iException )
				vTargetPos = vMyOldPos;
			
			pDummy = pStage->CreateDummyChar( szDummyCharName, iCurIndex,
				pOwner->GetCharName(),
				vTargetPos,
				fStartAngle,
				0, true );

			DummyCharCreateInfo kInfo;
			kInfo.m_DummyCharName = szDummyCharName;
			kInfo.m_iDummyCharIdx = iCurIndex;
			kInfo.m_fDummyCharStartAngle = fStartAngle;
			kInfo.m_vTargetPos = vTargetPos;

			m_CreateDummyList.push_back( kInfo );
		}

		pOwner->SetWorldPosition( vCheckTargetPos );
	}


	int iCreateCnt = m_CreateDummyList.size();
	SP2Packet kPacket( CUPK_APPLY_EXTRA_BUFF_INFO );
	kPacket << pOwner->GetCharName();
	kPacket << GetName();
	kPacket << pOwner->GetWorldPosition();
	kPacket << iCreateCnt;
	if( pOwner->IsNeedSendNetwork() && iCreateCnt > 0 )
	{
		for( int i=0; i<iCreateCnt; ++i )
		{
			kPacket << m_CreateDummyList[i].m_DummyCharName;
			kPacket << m_CreateDummyList[i].m_iDummyCharIdx;
			kPacket << m_CreateDummyList[i].m_vTargetPos;
			kPacket << m_CreateDummyList[i].m_fDummyCharStartAngle;
		}
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

void ioCreateDummyChar7Buff::ApplyExtraBuffInfo( SP2Packet &rkPacket )
{
	ioBaseChar *pOwner = GetOwner();
	if( !pOwner )
		return;

	ioPlayStage *pStage = pOwner->GetCreator();
	if( !pStage )
		return;

	D3DXVECTOR3 vWorldPos;
	rkPacket >> vWorldPos;
	pOwner->SetWorldPosition( vWorldPos );

	m_CreateDummyList.clear();

	int iSize;
	rkPacket >> iSize;
	for( int i=0; i<iSize; ++i )
	{
		int iCurIndex;
		ioHashString szDummyCharName;
		D3DXVECTOR3 vPos;

		float fAngle;
		rkPacket >> szDummyCharName;
		rkPacket >> iCurIndex;
		rkPacket >> vPos;
		rkPacket >> fAngle;
		ioDummyChar *pDummy = pStage->CreateDummyChar( szDummyCharName, iCurIndex,
			pOwner->GetCharName(),
			vPos, fAngle,
			0, false );

		DummyCharCreateInfo kInfo;
		kInfo.m_DummyCharName = szDummyCharName;
		kInfo.m_iDummyCharIdx = iCurIndex;
		kInfo.m_fDummyCharStartAngle = fAngle;
		kInfo.m_vTargetPos = vPos;

		m_CreateDummyList.push_back( kInfo );
	}

	m_bCreatedDummy = true;
}

void ioCreateDummyChar7Buff::CheckDummyChar()
{
	ioBaseChar *pOwner = GetOwner();
	if( !pOwner )
		return;

	int iSize = m_CreateDummyList.size();
	for( int i=0; i<iSize; ++i )
	{
		ioDummyChar *pDummy = g_DummyCharMgr.FindDummyChar( pOwner, m_CreateDummyList[i].m_iDummyCharIdx );
		if( pDummy && pDummy->GetState() == DCS_DIE )
		{
			pOwner->CheckUseBuffSkill( m_Name, false );
			SetReserveEndBuff();
			return;
		}

		if( !pDummy )
		{
			pOwner->CheckUseBuffSkill( m_Name, false );
			SetReserveEndBuff();
			return;
		}
	}
}

D3DXVECTOR3 ioCreateDummyChar7Buff::GetDummyCharPos()
{
	ioBaseChar *pOwner = GetOwner();
	if( !pOwner )
		return ioMath::VEC3_ZERO;

	int iSize = m_CreateDummyList.size();
	for( int i=0; i<iSize; ++i )
	{
		ioDummyChar *pDummy = g_DummyCharMgr.FindDummyChar( pOwner, m_CreateDummyList[i].m_iDummyCharIdx );
		if( pDummy  )
		{
			return pDummy->GetWorldPosition();
		}
	}

	return pOwner->GetWorldPosition();
}

void ioCreateDummyChar7Buff::CheckActiveCount()
{
	m_iCurActiveCount--;
	m_iCurActiveCount = max( m_iCurActiveCount, 0 );
}


bool ioCreateDummyChar7Buff::CheckEnableUseSkill( int iSlot )
{
	switch( iSlot )
	{
	case ES_WEAPON:
		if( m_bEnableWeaponSkill )
			return true;
		break;
	case ES_ARMOR:
		if( m_bEnableArmorSkill )
			return true;
		break;
	case ES_HELMET:
		if( m_bEnableHelmetSkill )
			return true;
		break;
	case ES_CLOAK:
		if( m_bEnableCloakSkill )
			return true;
		break;
	}

	return false;
}