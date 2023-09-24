#pragma once

class ioBaseChar;
class ioPlayStage;
class ioPlayMode;
class ioINILoader;
class SP2Packet;

class ioDefaultPet : public ioPet
{
protected:
	enum PetState
	{
		DPS_None,
		DPS_Special_Loop,
		DPS_Owner_Attach,
	};

protected:
	//���� ����
	PetState m_PetState;
	
	//��ġ ����
	D3DXVECTOR3 m_vAttachOffset;

public:
	//������ �б�
	virtual void LoadProperty( ioINILoader &rkLoader );
	//����ȭ ó��
	virtual void ApplyPetSync( ioPlayStage *pStage, SP2Packet &rkPacket );

public:
	//�� ���� ���� (ĳ������ ������Ʈ�� ����)
	virtual void SetPetState( int eCharState ); 
	//���� ����
	virtual bool OnStartState();
	//���� ����
	virtual void OnDeadState();

protected:
	//����
	virtual void OnProcessState();
	
protected:
	//���� Ȯ��
	void CheckDie();
	//���̵� Ȯ��
	void CheckVisible();
	
protected:
	//�̵� ����
	void OwnerAttachMove();

public:
	ioDefaultPet( ioEntityGroup *pGrp, ioPlayMode *pMode );
	virtual ~ioDefaultPet();
};

inline ioDefaultPet* ToDefaultPet( ioPet *pPet )
{
	if( !pPet || pPet->GetTypeID() != DPST_DEFAULT )
		return NULL;

	return dynamic_cast< ioDefaultPet* >( pPet );
}