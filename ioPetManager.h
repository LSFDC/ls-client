#pragma once

class ioINILoader;
class ioPlayEntity;
class ioBaseChar;
class ioPlayMode;
class ioPlayStage;
class ioPet;
class SP2Packet;

class ioPetManager : public Singleton< ioPetManager >
{
protected:
	typedef std::vector< ioPet* > PetList;
	PetList m_PetList;
	PetBaseInfoList m_vPetBaseInfoList;

	ioPlayStage *m_pPlayStage;
	CurPetInfoList m_CurPetInfoList;
	
public:
	//�⺻ ������ �б�
	void LoadPetBaseInfo();

protected:
	//�� ���� �б� (��ũ��)
	void LoadModelInfo( ioINILoader &rkLoader, PetModelInfoList &vPetModelInfoList );
	//������ ����
	void ClearPetBaseInfo();

public:
	//�� ����
	ioPet* CreatePetEntity( ioHashString szModelName, ioPlayMode *pMode, PetTypeInfo sPetType );

protected:
	//���� Ÿ�Կ� ���� ���� Ÿ�� �� ����
	ioPet* CreateDefaultPet( ioEntityGroup *pGrp, ioPlayMode *pMode, int nSubType );
	
public:
	//���� ��� �� ����
	void DestroyPetList();
	//���� ���� Ȯ�� �� ����
	void ClearDeadPet();
	
public:
	//�⺻ �� ���� ���
	bool GetPetBaseInfo( int nPetCode, PetBaseInfo &rkInfo );
	//�ش� ĳ������ �� ã��
	ioPet* FindPet( ioBaseChar *pOwner );
	//�ش� ĳ������ �� ���� ���·� ��ȯ
	void SetPetDieState( ioBaseChar *pOwner );
				
public:
	//�� �� ���� ��� (�̸��� ��ũ�� ����)
	bool GetPetModelInfo( int nPetCode, PetRankType eCurPetRankType, PetModelInfo &sPetModelInfo );
	
public:
	//�� ����
	ioPet* CreatePet( const ioHashString &szOwnerName, const D3DXVECTOR3 &vTargetPos );

protected:
	void AddCurPetInfo( ioHashString szCharName, CurPetInfo sCurPetInfo, bool bOnlyChange );

public:
	bool GetCurPetInfo( ioHashString szCharName, CurPetInfo &sCurPetInfo );

public:
	//�� ����ȭ
	void OnPetSync( SP2Packet &rkPacket );
	//�� ����(���Ը� ����)
	void OnPetChangeData( SP2Packet &rkPacket );
	//�� ����(������)
	void OnPetEquipData( SP2Packet &rkPacket );
	//�� ����(���� ����)
	void OnPetRemoveData( SP2Packet &rkPacket );
	//�� �߰�
	void OnPetAdd( SP2Packet &rkPacket );
	//�� ����ġ ����
	void OnPetNurture( SP2Packet &rkPacket );
	//�� ���� ����(������)
	void OnPetNurtureLevelUp( SP2Packet &rkPacket );
	//�� �ռ�
	void OnPetCompound( SP2Packet &rkPacket );

public:
	static ioPetManager& GetSingleton();

public:
	ioPetManager( ioPlayStage *pStage );
	virtual ~ioPetManager();
};

#define g_PetMgr ioPetManager::GetSingleton()