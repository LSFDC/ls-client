
#include "stdafx.h"

#include "ioDefaultPet.h"

#include "ioPlayStage.h"
#include "ioBaseChar.h"

ioDefaultPet::ioDefaultPet( ioEntityGroup *pGrp, ioPlayMode *pMode )
	: ioPet( pGrp, pMode )
{	
	m_PetState = DPS_None;
	m_bPreForceHide = false;
}

ioDefaultPet::~ioDefaultPet()
{
}

//������ �б�
void ioDefaultPet::LoadProperty( ioINILoader &rkLoader )
{
	ioPet::LoadProperty( rkLoader );

	m_vAttachOffset.x = rkLoader.LoadFloat_e( "attach_offset_x", 0.0f );
	m_vAttachOffset.y = rkLoader.LoadFloat_e( "attach_offset_y", 0.0f );
	m_vAttachOffset.z = rkLoader.LoadFloat_e( "attach_offset_z", 0.0f );
}

//����ȭ ó��
void ioDefaultPet::ApplyPetSync( ioPlayStage *pStage, SP2Packet &rkPacket )
{
	//����ȭ
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�� ���� ���� (ĳ������ ������Ʈ�� ����)
void ioDefaultPet::SetPetState( int eCharState )
{
	//���� ���� Ȯ��
	if ( m_PreOwnerState == eCharState )
		return;

	if ( eCharState == CS_SPECIAL_MOTION )
		m_PetState = DPS_Special_Loop;
	else
		m_PetState = DPS_Owner_Attach;
	
	//���� ���¸� ��ü
	m_PreOwnerState = (CharState)eCharState;

	//�ִϸ��̼� ��ü
	SetStateAni( eCharState );
}

//���� ����
bool ioDefaultPet::OnStartState()
{
	SetStateAni( CS_DELAY );
	m_PetState = DPS_Owner_Attach;

	return true;
}

//���� ����
void ioDefaultPet::OnDeadState()
{
	ioPet::OnDeadState();
}

//����
void ioDefaultPet::OnProcessState()
{
	CheckDie();
	CheckVisible();

	switch( m_PetState )
	{
	case DPS_Special_Loop:
		break;
	case DPS_Owner_Attach:
		OwnerAttachMove();
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���� Ȯ��
void ioDefaultPet::CheckDie()
{
	if ( !GetOwner() )
		OnDeadState();
}

//���̵� Ȯ��
void ioDefaultPet::CheckVisible()
{
	if ( !m_pOwner || !m_pPlayStage )
		return;

	//���� ���̵� Ȯ��
	if ( m_bPreForceHide != m_bForceHide )
	{
		m_pGroup->SetVisible( !m_bForceHide );
		m_bPreForceHide = m_bForceHide;
	}
	
	//���� ���̵�� ����
	if ( m_bForceHide )
		return;		
	
	bool bHide = false;
	if ( m_pOwner->IsHideState() )
		bHide = true;

	//���̵� ���� Ȯ��
	if ( m_bPreHide != bHide )
	{
		m_pGroup->SetVisible( !bHide );
		m_bPreHide = bHide;
	}

	//���̵�� ����
	if ( bHide )
		return;		

	int nOwnerAlphaRate = m_pOwner->GetAlphaRate();
	if ( m_nPreAlphaRate != nOwnerAlphaRate )
	{
		m_pGroup->SetAlphaRate( nOwnerAlphaRate );
		m_nPreAlphaRate = nOwnerAlphaRate;

		if ( nOwnerAlphaRate > 250 )
			m_pGroup->SetVisible( true );
		else if ( nOwnerAlphaRate < 5 )
			m_pGroup->SetVisible( false );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�̵� ����
void ioDefaultPet::OwnerAttachMove()
{
	if ( !m_pOwner )
		return;

	D3DXQUATERNION qtRot = m_pOwner->GetTargetRot();
	D3DXVECTOR3 vPos = m_pOwner->GetMidPositionByRate() + ( qtRot * m_vAttachOffset * m_pOwner->GetWorldScale() );
	SetWorldPosition( vPos );
	SetWorldOrientation( qtRot );
}