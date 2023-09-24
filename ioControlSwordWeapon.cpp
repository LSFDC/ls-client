

#include "stdafx.h"

#include "ioControlSwordWeapon.h"

#include "ioBaseChar.h"
#include "ioPlayStage.h"

ActionStopType ioControlSwordWeapon::m_ActionStopType = AST_NONE;
ActionStopType ioControlSwordWeapon::m_DieActionStopType = AST_NONE;

ioControlSwordWeapon::ioControlSwordWeapon( ioBaseChar *pOwner, const WeaponAttribute *pAttr )
	: ioWeapon( pOwner, pAttr )
{
	SetPeculiarity( WP_ATTACK_OWNER | WP_MINE_EXPLOSION | WP_WOUND_NO_SOUND );

	m_pMesh = NULL;
	m_State = CS_PRE;

	m_bResistance = false;
	m_fMoveSpeed = 0.0f;	

	m_dwKeyInputTime = 0;
	m_dwAccelEndTime = 0;

	m_bTargetRot = false;
	m_CurDirKey = ioUserKeyInput::DKI_NONE;

	m_bSetUpKey = false;
	m_bSetDownKey = false;

	m_dwCurKeyInputGapTime = 0;
	m_dwCurMapCollisionDelayTime = 0;
}

ioControlSwordWeapon::~ioControlSwordWeapon()
{
	if( m_pMesh )
	{
		LOG.PrintTimeAndLog( 0, "ioControlSwordWeapon::~ioControlSwordWeapon - UnReleased Resouce" );
	}
}

bool ioControlSwordWeapon::InitResource()
{
	if( ioWeapon::InitResource() )
	{
		DestroyResource();
		return SetSwordMesh( GetAttribute()->m_WeaponEffect );
	}

	return false;
}

void ioControlSwordWeapon::LoadProperty( ioINILoader &rkLoader )
{
	ioWeapon::LoadProperty( rkLoader );

	char szBuf[MAX_PATH] = "";

	rkLoader.LoadString_e( "boom_effect", "", szBuf, MAX_PATH );
	m_szBoomEffect = szBuf;
	rkLoader.LoadString_e( "map_collision_effect", "", szBuf, MAX_PATH );
	m_szMapCollisionEffect = szBuf;
	m_dwMapCollisionDelayTime = rkLoader.LoadFloat_e( "map_collision_effect_delay", 500 );

	m_dwRotateSpeed = (DWORD)rkLoader.LoadInt_e( "rotate_speed", 0 );

	m_bDisableUpDown = rkLoader.LoadBool_e( "disable_updown", false );
	m_bPlayWoundSound = rkLoader.LoadBool_e( "play_wound_sound", false );
	m_bDisableMapCollision = rkLoader.LoadBool_e( "disable_map_collision", false );

	m_fAccelSpeed = rkLoader.LoadFloat_e( "accel_speed", 200.0f );
	m_fMinSpeed = rkLoader.LoadFloat_e( "min_speed", FLOAT500 );	
	m_dwAccelKeepTime = rkLoader.LoadInt_e( "accel_keep_time", 500 );
	m_dwKeyInputGapTime = rkLoader.LoadInt_e( "key_input_gaptime", 250 );
	m_dwAccelUpTime = rkLoader.LoadInt_e( "aceel_up_time", 50 ); 

	m_fControlOutRange = rkLoader.LoadFloat_e( "control_out_range", FLOAT1000 );
}

bool ioControlSwordWeapon::SetSwordMesh( const ioHashString &szFileName )
{
	ioBaseChar *pOwner = GetOwner();
	if( !pOwner )	
		return false;

	m_pMesh = g_EffectFactory.CreateEffect( szFileName );
	if( !m_pMesh )	
		return false;

	GetParentSceneNode()->AttachObject( m_pMesh );
	GetParentSceneNode()->SetScale( ioMath::UNIT_ALL );

	ioHashString szLinkBiped;
	D3DXVECTOR3 vLinkPosOffset;
	D3DXQUATERNION qtLinkRotOffset;

	if( m_pMesh->GetSkeletonName().IsEmpty() && m_pMesh->GetFirstLinkOffset(szLinkBiped, vLinkPosOffset, qtLinkRotOffset ) )
	{
		ioBiped *pLinked = pOwner->GetGroup()->GetBiped( szLinkBiped );
		if( pLinked )
		{
			vLinkPosOffset = pLinked->GetDerivedOrientation() * vLinkPosOffset + pLinked->GetDerivedPosition();
			vLinkPosOffset = vLinkPosOffset * pOwner->GetWorldScale();
			vLinkPosOffset = pOwner->GetWorldOrientation() * vLinkPosOffset + pOwner->GetWorldPosition();
			SetPosition( vLinkPosOffset );
		}
	}

	m_pMesh->UpdateBounds( true );
	m_LocalAttackBox = m_pMesh->GetCollisionBox();

	AddEffectEvent( m_pMesh );
	return true;
}

void ioControlSwordWeapon::DestroyResource( ioPlayStage *pStage )
{
	if( m_pMesh )
	{
		g_EffectFactory.DestroyEffect( m_pMesh );
		m_pMesh = NULL;
	}
}

const ioOrientBox& ioControlSwordWeapon::GetLocalAttackBox() const
{
	return m_LocalAttackBox;
}

ioWeapon::WeaponType ioControlSwordWeapon::GetType() const
{
	return WT_CONTROL_SWORD;
}

ActionStopType ioControlSwordWeapon::GetActionStopType() const
{
	return m_ActionStopType;
}

ActionStopType ioControlSwordWeapon::GetDieActionStopType() const
{
	return m_DieActionStopType;
}

void ioControlSwordWeapon::SetMoveDir( const D3DXVECTOR3 &vDir )
{
	m_vMoveDir = vDir;

	D3DXQUATERNION qtRot;
	ioMath::CalcDirectionQuaternion( qtRot, -vDir );
	SetOrientation( qtRot );
}

void ioControlSwordWeapon::SetStartPosition( const D3DXVECTOR3 &vPos, ioPlayStage *pStage )
{
	if( m_pMesh && m_pMesh->GetSkeletonName().IsEmpty() && m_pMesh->HasLinkedSystem() )
		return;

	SetPosition( vPos );
	m_vStartPos = GetPosition();
}

void ioControlSwordWeapon::PlayWoundedSound( ioPlayEntity *pEntity )
{
	if( HasPeculiarity( WP_WOUND_NO_SOUND ) && !m_bPlayWoundSound )
		return;

	const ioHashString &szSound = GetAttribute()->m_WoundedSound;

	if( ToBaseChar( pEntity ) )
	{
		std::string szNewSound = ToBaseChar(pEntity)->GetWoundedSoundName( szSound );
		g_SoundMgr.PlaySound( ioHashString(szNewSound.c_str()), GetParentSceneNode() );
	}
	else
	{
		g_SoundMgr.PlaySound( szSound, GetParentSceneNode() );
	}

	StopLoopSound();
}

void ioControlSwordWeapon::Process( float fTimePerSec, ioPlayStage *pStage )
{
	DWORD dwCurTime = FRAMEGETTIME();

	switch( m_State )
	{
	case CS_PRE:
		{			
			m_fCurMoveSpeed = GetMoveSpeed();
			m_State = CS_FLOATING;
		}
		break;
	case CS_FLOATING:
		{
			OnFloating( fTimePerSec, dwCurTime, pStage );			
		}
		break;		
	case CS_END:
		SetWeaponDead();
	}

	UpdateWorldAttackBox( fTimePerSec, false );
}

bool ioControlSwordWeapon::TestMapCollision( ioOpcodeShape *pShape, ioPlayStage *pStage, const D3DXMATRIX* pShapeWorldMat )
{
	//pShape�� ���� �� �浹 �˻縦 ���� �ϰ��� �Ҷ� TRUE
	//�Ϲ� ����� ��� ���� �޽��� 1�������� �Ͽ�¡�ý����� ��� N�� �̱� ������ TestMapCollision() �Լ��� ������ ȣ���

	if( m_State != CS_FLOATING )
		return true;

	DWORD dwCurTime = FRAMEGETTIME();

	if( m_dwCurMapCollisionDelayTime < dwCurTime )
	{
		if( pShape->TestIntersection( pShapeWorldMat, GetWorldAttackBox(true) ) )
		{
			//�� ��� ����Ʈ ����
			CreateMapCollisionEffect( pStage );
			m_dwCurMapCollisionDelayTime = dwCurTime + m_dwMapCollisionDelayTime;
			return true;
		}
	}

	 return false;
}

void ioControlSwordWeapon::OnFloating( float fTimePerSec, DWORD dwCurTime, ioPlayStage *pStage )
{
	if( 0 < m_dwAccelEndTime )
	{
		if( dwCurTime < m_dwAccelEndTime )
		{
			//���ӵ��� �� �����Ӹ��� ���� ���̴°� �ƴ϶� ������ �ð����� �ѹ��� ������
			if( m_dwAccelUpStartTime < dwCurTime )
			{
				m_dwAccelUpStartTime = dwCurTime;
				float fCurAccel =  m_fAccelSpeed / (m_dwKeyInputGapTime / m_dwAccelUpTime);

				m_fCurMoveSpeed -= fCurAccel;
				m_fCurMoveSpeed = max( m_fCurMoveSpeed, m_fMinSpeed );
			}
		}
		else
		{
			//���ӵ��� �� �����Ӹ��� ���� �ø��°� �ƴ϶� ������ �ð����� �ѹ��� �÷���
			if( m_dwAccelDownStartTime < dwCurTime )
			{
				m_dwAccelDownStartTime = dwCurTime;
				float fCurAccel = m_fAccelSpeed / (m_dwKeyInputGapTime / m_dwAccelUpTime);

				m_fCurMoveSpeed += fCurAccel;
				m_fCurMoveSpeed = min( m_fCurMoveSpeed, GetMoveSpeed() );
			}
		}
	}
	else
	{
		m_fCurMoveSpeed = GetMoveSpeed() + m_fAccelSpeed;
	}

	float fMoveDist = m_fCurMoveSpeed * fTimePerSec;

	CheckMoveKey( fTimePerSec, dwCurTime );
	CheckMoveDir();

	GetParentSceneNode()->Translate( m_vMoveDir * fMoveDist );

	if( m_pMesh )
	{
		m_pMesh->Update( fTimePerSec * FLOAT1000 );
	}

	D3DXVECTOR3 vDiff = m_vStartPos - GetPosition();
	float fDistanceSq = D3DXVec3LengthSq( &vDiff );
	if( m_fControlOutRange * m_fControlOutRange < fDistanceSq )
	{
		m_State = CS_END;
	}
}

void ioControlSwordWeapon::UpdateWorldAttackBox( float fTimePerSec, bool bSetNull )
{
	if( !IsLive() || bSetNull )
	{
		m_WorldAttackBox.SetNull();
		m_WorldAttackBoxForMap.SetNull();
		return;
	}

	m_WorldAttackBox = GetLocalAttackBox();
	m_WorldAttackBoxForMap = m_WorldAttackBox;

	float fMapCollisionRate = GetAttribute()->m_fMapCollisionBoxRate;
	m_WorldAttackBoxForMap.SetExtents( 0, m_WorldAttackBox.GetExtents(0)*fMapCollisionRate );
	m_WorldAttackBoxForMap.SetExtents( 1, m_WorldAttackBox.GetExtents(1)*fMapCollisionRate );
	m_WorldAttackBoxForMap.SetExtents( 2, m_WorldAttackBox.GetExtents(2)*fMapCollisionRate );

	if( !m_WorldAttackBox.IsNull() )
	{
		if( m_fOwnerChargeScaleRate > FLOAT1 )
		{
			D3DXVECTOR3 vScale( m_fOwnerChargeScaleRate, m_fOwnerChargeScaleRate, m_fOwnerChargeScaleRate );
			m_WorldAttackBox.Transform( GetPosition(), vScale, GetOrientation() );
			m_WorldAttackBoxForMap.Transform( GetPosition(), vScale, GetOrientation() );
		}
		else
		{
			m_WorldAttackBox.Transform( GetPosition(), ioMath::UNIT_ALL, GetOrientation() );
			m_WorldAttackBoxForMap.Transform( GetPosition(), ioMath::UNIT_ALL, GetOrientation() );
		}
	}
}

bool ioControlSwordWeapon::IsFloating() const
{
	if( m_State == CS_FLOATING )
		return true;

	return false;
}

bool ioControlSwordWeapon::IsCollision( ioPlayEntity *pEntity, bool bMapCollision )
{
	bool bCollision = false;
	D3DXVECTOR3 vColPoint( 0.0f, 0.0f, 0.0f );
	if( pEntity->IsWoundedCollision( GetWorldAttackBox(bMapCollision), &vColPoint ) )
		bCollision = true;

	return bCollision;
}

float ioControlSwordWeapon::GetMoveSpeed()
{
	if( m_fMoveSpeed == 0.0f )
		m_fMoveSpeed = GetAttribute()->m_fMoveSpeed;

	return m_fMoveSpeed * m_fOwnerChargeSpeedRate;
}

void ioControlSwordWeapon::CheckMoveKey( float fTimePerSec, DWORD dwCurTime )
{
	ioBaseChar *pOwner = GetOwner();
	if( !pOwner ) return;

	if( !pOwner->IsNeedProcess() )
		return;

	bool bChange = false;
	ioUserKeyInput::DirKeyInput eNewDirKey = pOwner->CheckCurDirKey();
	if( eNewDirKey != m_CurDirKey )
	{
		bChange = true;
		m_bTargetRot = false;
		m_CurDirKey = eNewDirKey;
	}

	if( !m_bDisableUpDown )
	{
		if( !m_bSetDownKey && m_bSetUpKey != pOwner->IsJumpKeyDown() )
		{
			bChange = true;
			m_bTargetRot = false;
			m_bSetUpKey = pOwner->IsJumpKeyDown();
		}

		if( !m_bSetUpKey && m_bSetDownKey != pOwner->IsDefenseKeyDown() )
		{
			bChange = true;
			m_bTargetRot = false;
			m_bSetDownKey = pOwner->IsDefenseKeyDown();
		}
	}

	bool bSetAttackKey = false;
	if( m_dwCurKeyInputGapTime < dwCurTime )
	{
		if( pOwner->IsAttackKey() )
		{
			bSetAttackKey = true;
			bChange = true;
			m_dwCurKeyInputGapTime = dwCurTime;
			m_dwCurKeyInputGapTime += m_dwKeyInputGapTime;			

			m_dwAccelUpStartTime = dwCurTime;
			m_dwAccelEndTime = dwCurTime + m_dwAccelKeepTime;
			m_dwAccelDownStartTime = m_dwAccelEndTime;
		}
	}

	if( bChange && pOwner->IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_WEAPON_SYNC );
		kPacket << GetOwnerName();
		kPacket << m_dwWeaponIdx;
		kPacket << SSC_CHANGE_MOVE;
		kPacket << (int)m_CurDirKey;
		kPacket << m_bSetUpKey;
		kPacket << m_bSetDownKey;
		kPacket << bSetAttackKey;
		kPacket << m_vMoveDir;
		P2PNetwork::SendToAllPlayingUser( kPacket );
	}
}

void ioControlSwordWeapon::CheckMoveDir()
{
	ioBaseChar *pOwner = GetOwner();
	if( !pOwner ) return;

	if( m_dwRotateSpeed == 0 )
		return;

	if( m_bTargetRot )
		return;

	if( m_CurDirKey == ioUserKeyInput::DKI_NONE && !m_bSetUpKey && !m_bSetDownKey )
	{
		return;
	}

	float fFrameGap = g_FrameTimer.GetCurFrameGap();
	float fRate = fFrameGap / m_dwRotateSpeed;
	float fRotateAngle = FLOAT360 * fRate;

	D3DXVECTOR3 vCurMoveDir = m_vMoveDir;
	D3DXVec3Normalize( &vCurMoveDir, &vCurMoveDir );

	// ��ǥ�� ����üũ �� ó��
	D3DXVECTOR3 vTargetDir = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	if( m_CurDirKey != ioUserKeyInput::DKI_NONE )
	{
		float fYawD = 180.0f + 45.0f * ( m_CurDirKey - ioUserKeyInput::DKI_UP );
		fYawD = ioMath::ArrangeHead( fYawD );

		float fYawR = DEGtoRAD( fYawD );
		D3DXQUATERNION qtRotate;
		D3DXQuaternionRotationYawPitchRoll( &qtRotate, fYawR, 0.0f, 0.0f );

		vTargetDir = qtRotate * D3DXVECTOR3(0.0f, 0.0f, -FLOAT1);
	}

	if( m_bSetUpKey )
	{
		vTargetDir += ioMath::UNIT_Y;
	}
	else if( m_bSetDownKey )
	{
		vTargetDir += (-ioMath::UNIT_Y);
	}

	D3DXVec3Normalize( &vTargetDir, &vTargetDir );

	float fDotValue = (float)D3DXVec3Dot( &vCurMoveDir, &vTargetDir );
	float fGapAngle = RADtoDEG( acosf(fDotValue) );

	if( fDotValue >= FLOAT1 )
	{
		m_bTargetRot = true;

		D3DXQUATERNION qtTargetRot;
		ioMath::CalcDirectionQuaternion( qtTargetRot, -vTargetDir );

		m_vMoveDir = qtTargetRot * D3DXVECTOR3(0.0f, 0.0f, -FLOAT1);
		D3DXVec3Normalize( &m_vMoveDir, &m_vMoveDir );

		SetOrientation( qtTargetRot );
		return;
	}

	D3DXVECTOR3 vAxis;
	if( fDotValue <= -FLOAT1 )
	{
		D3DXVECTOR3 vTmpAxis;

		if( vCurMoveDir != ioMath::UNIT_Y )
		{
			D3DXVec3Cross( &vTmpAxis, &ioMath::UNIT_Y, &vCurMoveDir );
			D3DXVec3Normalize( &vTmpAxis, &vTmpAxis );
		}
		else
		{
			D3DXVec3Cross( &vTmpAxis, &ioMath::UNIT_X, &vCurMoveDir );
			D3DXVec3Normalize( &vTmpAxis, &vTmpAxis );
		}

		D3DXVec3Cross( &vAxis, &vTmpAxis, &vCurMoveDir );
		D3DXVec3Normalize( &vAxis, &vAxis );
	}
	else
	{
		D3DXVec3Cross( &vAxis, &vCurMoveDir, &vTargetDir );
		D3DXVec3Normalize( &vAxis, &vAxis );
	}

	float fCurRotateAngle = fRotateAngle;
	if( fGapAngle < fRotateAngle )
		fCurRotateAngle = fGapAngle;

	D3DXQUATERNION qtNewRot;
	D3DXQuaternionRotationAxis( &qtNewRot, &vAxis, DEGtoRAD(fCurRotateAngle) );

	D3DXVECTOR3 vNewDir = qtNewRot * vCurMoveDir;
	D3DXVec3Normalize( &vNewDir, &vNewDir );

	m_vMoveDir = vNewDir;

	GetParentSceneNode()->Rotate( vAxis, fCurRotateAngle );
}

void ioControlSwordWeapon::ApplyWeaponSync( SP2Packet &rkPacket, ioPlayStage *pStage )
{
	int iState;
	rkPacket >> iState;

	switch( iState )
	{
	case SSC_CHANGE_MOVE:
		{
			int iDirKey;
			D3DXVECTOR3 vMoveDir;
			rkPacket >> iDirKey;

			m_CurDirKey = (ioUserKeyInput::DirKeyInput)iDirKey;

			bool bSetAttackKey;
			rkPacket >> m_bSetUpKey;
			rkPacket >> m_bSetDownKey;
			rkPacket >> bSetAttackKey;
			rkPacket >> vMoveDir;

			if( bSetAttackKey )
			{
				DWORD dwCurTime = FRAMEGETTIME();
				m_dwCurKeyInputGapTime = dwCurTime;
				m_dwCurKeyInputGapTime += m_dwKeyInputGapTime;

				m_dwAccelUpStartTime = dwCurTime;
				m_dwAccelEndTime = dwCurTime + m_dwAccelKeepTime;
				m_dwAccelDownStartTime = m_dwAccelEndTime;
			}

			m_bTargetRot = false;
			SetMoveDir( vMoveDir );
		}
		break;
	}
}

void ioControlSwordWeapon::CreateMapCollisionEffect( ioPlayStage* pStage )
{
	pStage->CreateMapEffect( m_szMapCollisionEffect, GetPosition(), D3DXVECTOR3( FLOAT1, FLOAT1, FLOAT1 ) );
}

void ioControlSwordWeapon::SetWeaponDead()
{
	if( GetOwner() && GetOwner()->GetCreator() )
	{
		GetOwner()->GetCreator()->CreateMapEffect( GetExplosionEffectName(), GetWorldAttackBox().GetCenter(), GetScale() );
		PlayExplosionSound();
	}

	StopLoopSound();
	m_bLive = false;
}
