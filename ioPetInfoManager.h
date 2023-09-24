#pragma once

class ioPetInfoManager : public Singleton< ioPetInfoManager >
{
protected:
	//��ũ ���� ���� (�ִ� ����, ��ũ ���)
	PetRankInfoList			m_PetRankInfoList;
	//���� ��� ����
	PetAddStatInfoList		m_PetAddStatInfoList;
	//�� �⺻ ���� ����
	PetBaseStatInfoList		m_PetBaseInfoList;

	//�ִ� ����ġ ���
	float m_fMaxExpConst;
	//���ȱ� ��Ұ�
	int m_nResellPeso;

public:
	static ioPetInfoManager& GetSingleton();

public:
	//������ ����
	void ClearData();
	//������ �б�
	void LoadProperty();

protected:
	////��ũ ����
	void LoadPetRankInfo( ioINILoader &rkLoader );
	//���� ����
	void LoadPetAddStatInfo( ioINILoader &rkLoader );
	//���� ���� �б�
	void LoadPetBaseInfo( ioINILoader &rkLoader );
	void LoadViewList( ioINILoader &rkLoader, PetViewInfoList& vList );
	void LoadBaseGrowthList( ioINILoader &rkLoader, IntVec& vList );
	void LoadAddStatList( ioINILoader &rkLoader, IntVec& vList );

public:
	//�ִ� ����ġ ���
	int GetMaxExp( int nCurLevel, PetRankType eRankType );
	//�ִ� ���� ���
	int GetMaxLevel( PetRankType eRankType );
	//�⺻ ���� ���
	int GetBaseGrowthStat( int nPetCode, int nIndex );
	//�߰� ���� ���
	int GetAddStat( int nPetCode, int nIndex, int nCurLevel );
	//�� ���� ���
	int GetTotalStat( int nPetCode, int nIndex, int nCurLevel );
	//��ũ ���
	PetRankType GetRankType( int nPetCode, bool bMaxRank );
	//�� ���� ���
	int GetPetManual( int nPetCode );
	//�� ��õ���� ���
	int GetPetNeedMaterial( int nPetCode );
	//�� �̸� ���(UIǥ��)
	ioHashString GetPetName( int nPetCode, PetRankType eRankType );
	//�� ������ ���(UIǥ��)
	ioHashString GetPetIcon( int nPetCode, PetRankType eRankType );
	//�� �� ���(UIǥ��)
	ioHashString GetPetModel( int nPetCode, PetRankType eRankType );
	//�� �� ũ�� ���(UIǥ��)
	D3DXVECTOR3 GetPetModelScale( int nPetCode, PetRankType eRankType );
	//�� �� �ִ� ���(UIǥ��)
	ioHashString GetPetViewAni( int nPetCode, PetRankType eRankType );
	//�� ��ũ ���ھ��
	ioHashString GetRankName( int nRank );
	//��� �����
	int GetResellPeso();

public:
	ioPetInfoManager();
	virtual ~ioPetInfoManager();
};

#define g_PetInfoMgr ioPetInfoManager::GetSingleton()