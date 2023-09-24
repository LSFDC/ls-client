
#include "stdafx.h"

#include "ioPetInfoManager.h"

template<> ioPetInfoManager* Singleton< ioPetInfoManager >::ms_Singleton = 0;

ioPetInfoManager::ioPetInfoManager()
{
	ClearData();
}

ioPetInfoManager::~ioPetInfoManager()
{
	ClearData();
}

ioPetInfoManager& ioPetInfoManager::GetSingleton()
{
	return Singleton< ioPetInfoManager >::GetSingleton();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//������ ����
void ioPetInfoManager::ClearData()
{
	//�� �⺻ ���� ����
	PetBaseStatInfoList::iterator Iter = m_PetBaseInfoList.begin();
	for ( ; Iter!=m_PetBaseInfoList.end(); ++Iter )
	{
		PetBaseStatInfo& sPetBaseStatInfo = Iter->second;
		//�⺻ ���� ��� ����Ʈ ����
		sPetBaseStatInfo.m_vBaseGrowthStat.clear();
		//��� Ÿ�� ����Ʈ ����
		sPetBaseStatInfo.m_vAddStatTypeList.clear();
		//������ ����Ʈ ����
		sPetBaseStatInfo.m_PetViewInfoList.clear();
	}
	m_PetBaseInfoList.clear();
	//��ũ ���� ����
	m_PetRankInfoList.clear();
	//���� ��� ���� ����
	m_PetAddStatInfoList.clear();
}

//������ �б�
void ioPetInfoManager::LoadProperty()
{	
	ioINILoader_e rkLoader( "config/sp2_pet_info.ini" );
	
	rkLoader.SetTitle_e( "common" );
	//�ִ� ����ġ ���
	m_fMaxExpConst = rkLoader.LoadFloat_e( "max_exp_const", 0.0f );
	//���ȱ� ��Ұ�
	m_nResellPeso = rkLoader.LoadInt_e( "sell_add_peso", 0 );
	//��ũ ����
	LoadPetRankInfo( rkLoader );
	//���� ����
	LoadPetAddStatInfo( rkLoader );
	//�⺻ �� ����
	LoadPetBaseInfo( rkLoader );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//��ũ ����
void ioPetInfoManager::LoadPetRankInfo( ioINILoader &rkLoader )
{
	char szBuf[MAX_PATH];

	rkLoader.SetTitle_e( "pet_rank_info" );

	int nMaxInfo = rkLoader.LoadInt_e( "max_rank_info_cnt", 0 );
	m_PetRankInfoList.clear();
	m_PetRankInfoList.reserve( nMaxInfo );

	for ( int i=0; i< nMaxInfo; ++i )
	{
		PetRankInfo sPetRankInfo;

		wsprintf_e( szBuf, "rank_info%d_type", i+1 );
		sPetRankInfo.m_RankType = (PetRankType)rkLoader.LoadInt( szBuf, PRT_NONE );

		wsprintf_e( szBuf, "rank_info%d_max_level", i+1 );
		sPetRankInfo.m_nMaxLevel = rkLoader.LoadInt( szBuf, 0 );

		wsprintf_e( szBuf, "rank_info%d_value", i+1 );
		sPetRankInfo.m_fRankValue = rkLoader.LoadFloat( szBuf, 0.0f );

		m_PetRankInfoList.push_back( sPetRankInfo );
	}
}

//���� ����
void ioPetInfoManager::LoadPetAddStatInfo( ioINILoader &rkLoader )
{
	char szBuf[MAX_PATH];

	rkLoader.SetTitle_e( "pet_add_stat_info" );

	int nMaxInfo = rkLoader.LoadInt_e( "max_add_stat_info_cnt", 0 );
	m_PetAddStatInfoList.clear();

	for ( int i=0; i< nMaxInfo; ++i )
	{
		wsprintf_e( szBuf, "add_stat_info%d_type", i+1 );
		int nAddStatType = rkLoader.LoadInt( szBuf, 0 );

		PetAddStatInfo sPetAddStatInfo;
		wsprintf_e( szBuf, "add_stat_info%d_per_level", i+1 );
		sPetAddStatInfo.m_nPerLevel = rkLoader.LoadInt( szBuf, 1 );

		wsprintf_e( szBuf, "add_stat_info%d_stat", i+1 );
		sPetAddStatInfo.m_nAddStat = rkLoader.LoadInt( szBuf, 0 );

		m_PetAddStatInfoList.insert( std::make_pair( nAddStatType, sPetAddStatInfo ) );
	}
}

//���� ���� �б�
void ioPetInfoManager::LoadPetBaseInfo( ioINILoader &rkLoader )
{
	char szBuf[MAX_PATH];

	rkLoader.SetTitle_e( "pet_base_common" );
	int nMaxInfo = rkLoader.LoadInt_e( "max_base_info_cnt", 0 );
	m_PetBaseInfoList.clear();

	for ( int i=0; i<nMaxInfo ; ++i )
	{
		wsprintf_e( szBuf, "pet_base_info%d", i+1 );
		rkLoader.SetTitle( szBuf );

		PetBaseStatInfo sPetBaseInfo;
		int nPetCode = rkLoader.LoadInt_e( "pet_code", 0 );
		sPetBaseInfo.m_BaseRank = (PetRankType)rkLoader.LoadInt_e( "pet_base_rank", PRT_NONE );
		sPetBaseInfo.m_MaxRank = (PetRankType)rkLoader.LoadInt_e( "pet_max_rank", PRT_NONE );
		sPetBaseInfo.m_nManualCode = rkLoader.LoadInt_e( "pet_manual", 0 );
		sPetBaseInfo.m_nNeedMaterial = rkLoader.LoadInt_e( "pet_need_material", 0 );

		LoadViewList( rkLoader, sPetBaseInfo.m_PetViewInfoList );
		LoadBaseGrowthList( rkLoader, sPetBaseInfo.m_vBaseGrowthStat );
		LoadAddStatList( rkLoader, sPetBaseInfo.m_vAddStatTypeList );			

		m_PetBaseInfoList.insert( std::make_pair( nPetCode, sPetBaseInfo ) );
	}
}

void ioPetInfoManager::LoadViewList( ioINILoader &rkLoader, PetViewInfoList& vList )
{
	char szBuf[MAX_PATH], szKey[MAX_PATH];
	
	int nIconCnt = rkLoader.LoadInt_e( "icon_cnt", 0 );
	for (int i=0; i<nIconCnt; ++i )
	{
		PetViewInfo sPetViewInfo;
		wsprintf_e( szKey, "pet_view%d_rank", i+1 );
		sPetViewInfo.m_PetRank = (PetRankType)rkLoader.LoadInt( szKey, PRT_NONE );

		wsprintf_e( szKey, "pet_view%d_name", i+1 );
		rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );
		sPetViewInfo.m_szPetName = szBuf;

		wsprintf_e( szKey, "pet_view%d_icon", i+1 );
		rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );
		sPetViewInfo.m_szPetIcon = szBuf;

		wsprintf_e( szKey, "pet_view%d_model", i+1 );
		rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );
		sPetViewInfo.m_szPetModel = szBuf;

		wsprintf_e( szKey, "pet_view%d_model_scale_x", i+1 );
		sPetViewInfo.m_vScale.x = rkLoader.LoadFloat( szKey, 2.0f );
		wsprintf_e( szKey, "pet_view%d_model_scale_y", i+1 );
		sPetViewInfo.m_vScale.y = rkLoader.LoadFloat( szKey, 2.0f );
		wsprintf_e( szKey, "pet_view%d_model_scale_z", i+1 );
		sPetViewInfo.m_vScale.z = rkLoader.LoadFloat( szKey, 2.0f );

		wsprintf_e( szKey, "pet_view%d_ani", i+1 );
		rkLoader.LoadString( szKey, "", szBuf, MAX_PATH );
		sPetViewInfo.m_szPetAni = szBuf;

		vList.push_back( sPetViewInfo );
	}
}

void ioPetInfoManager::LoadBaseGrowthList( ioINILoader &rkLoader, IntVec& vList )
{
	char szBuf[MAX_PATH];

	int nMaxCnt = rkLoader.LoadInt_e( "max_base_growth_stat_info", 0 );
	vList.clear();
	vList.reserve( nMaxCnt );

	for ( int i=0; i< nMaxCnt; i++ )
	{
		wsprintf_e( szBuf, "base_stat_info%d", i+1 );
		int nBaseStat = rkLoader.LoadInt( szBuf, 0 );
		vList.push_back( nBaseStat );
	}
}

void ioPetInfoManager::LoadAddStatList( ioINILoader &rkLoader, IntVec& vList )
{
	char szBuf[MAX_PATH];

	int nMaxCnt = rkLoader.LoadInt_e( "max_add_stat_info", 0 );
	vList.clear();
	vList.reserve( nMaxCnt );

	for ( int i=0; i< nMaxCnt; i++ )
	{
		wsprintf_e( szBuf, "add_stat_info%d_type", i+1 );
		int nAddStatType = rkLoader.LoadInt( szBuf, 0 );
		vList.push_back( nAddStatType );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�ִ� ����ġ ���
int ioPetInfoManager::GetMaxExp( int nCurLevel, PetRankType eRankType )
{
	if ( !COMPARE( eRankType, PRT_NONE+1, PRT_MAX ) )
		return 0;

	int nMaxInfo = (int)m_PetRankInfoList.size();
	for ( int i=0; i<nMaxInfo; ++i )
	{
		//�ش� ��ũ�϶� ��ũ�� ��� ������ ���
		if ( m_PetRankInfoList[i].m_RankType == eRankType )
			return (nCurLevel + 1) * m_fMaxExpConst * m_PetRankInfoList[i].m_fRankValue;
	}
	return 0;
}

//�ִ� ���� ���
int ioPetInfoManager::GetMaxLevel( PetRankType eRankType )
{
	if ( !COMPARE( eRankType, PRT_NONE+1, PRT_MAX ) )
		return 0;
	
	int nMaxInfo = (int)m_PetRankInfoList.size();
	for ( int i=0; i<nMaxInfo; ++i )
	{
		//�ش� ��ũ�϶� �ִ� ���� ��ȯ
		if ( m_PetRankInfoList[i].m_RankType == eRankType )
			return m_PetRankInfoList[i].m_nMaxLevel;
	}
	return 0;
}

//�⺻ ���� ���
int ioPetInfoManager::GetBaseGrowthStat( int nPetCode, int nIndex )
{
	if ( nIndex < 0 )
		return 0;

	PetBaseStatInfoList::iterator FindIter = m_PetBaseInfoList.find( nPetCode );
	if ( FindIter != m_PetBaseInfoList.end() )
	{
		PetBaseStatInfo sPetBaseInfo = FindIter->second;
		if ( nIndex < (int)sPetBaseInfo.m_vBaseGrowthStat.size() )
			return sPetBaseInfo.m_vBaseGrowthStat[nIndex];
	}
	
	return 0;
}

//�߰� ���� ���
int ioPetInfoManager::GetAddStat( int nPetCode, int nIndex, int nCurLevel )
{
	if ( nIndex < 0 )
		return 0;

	PetBaseStatInfoList::iterator FindIter = m_PetBaseInfoList.find( nPetCode );
	if ( FindIter != m_PetBaseInfoList.end() )
	{
		PetBaseStatInfo sPetBaseInfo = FindIter->second;
		if ( nIndex < (int)sPetBaseInfo.m_vAddStatTypeList.size() )
		{
			int nAddStatType = sPetBaseInfo.m_vAddStatTypeList[nIndex];

			PetAddStatInfoList::iterator FindIter = m_PetAddStatInfoList.find( nAddStatType );
			if ( FindIter != m_PetAddStatInfoList.end() )
			{
				PetAddStatInfo sPetAddStatInfo = FindIter->second;
				return nCurLevel * ((float)sPetAddStatInfo.m_nAddStat / sPetAddStatInfo.m_nPerLevel);
			}
		}
	}
	return 0;
}

//�� ���� ���
int ioPetInfoManager::GetTotalStat( int nPetCode, int nIndex, int nCurLevel )
{
	return GetBaseGrowthStat( nPetCode, nIndex ) + GetAddStat( nPetCode, nIndex, nCurLevel );
}

//��ũ ���
PetRankType ioPetInfoManager::GetRankType( int nPetCode, bool bMaxRank )
{
	PetBaseStatInfoList::iterator FindIter = m_PetBaseInfoList.find( nPetCode );
	if ( FindIter != m_PetBaseInfoList.end() )
	{
		PetBaseStatInfo sPetBaseInfo = FindIter->second;

		if ( bMaxRank )
			return sPetBaseInfo.m_MaxRank;
		else
			return sPetBaseInfo.m_BaseRank;
	}
	return PRT_NONE;
}

//�� ���� ���
int ioPetInfoManager::GetPetManual( int nPetCode )
{
	PetBaseStatInfoList::iterator FindIter = m_PetBaseInfoList.find( nPetCode );
	if ( FindIter != m_PetBaseInfoList.end() )
	{
		PetBaseStatInfo sPetBaseInfo = FindIter->second;
		return sPetBaseInfo.m_nManualCode;
	}
	return 0;
}

//�� ��õ���� ���
int ioPetInfoManager::GetPetNeedMaterial( int nPetCode )
{
	PetBaseStatInfoList::iterator FindIter = m_PetBaseInfoList.find( nPetCode );
	if ( FindIter != m_PetBaseInfoList.end() )
	{
		PetBaseStatInfo sPetBaseInfo = FindIter->second;
		return sPetBaseInfo.m_nNeedMaterial;
	}
	return 0;
}

//�� �̸� ���(UIǥ��)
ioHashString ioPetInfoManager::GetPetName( int nPetCode, PetRankType eRankType )
{
	static ioHashString szNull;

	PetBaseStatInfoList::iterator FindIter = m_PetBaseInfoList.find( nPetCode );
	if ( FindIter != m_PetBaseInfoList.end() )
	{
		PetBaseStatInfo sPetBaseInfo = FindIter->second;
		int nViewCnt = (int)sPetBaseInfo.m_PetViewInfoList.size();
		for ( int i=0; i<nViewCnt; ++i )
		{
			PetViewInfo sPetViewInfo = sPetBaseInfo.m_PetViewInfoList[i];
			if ( sPetViewInfo.m_PetRank == eRankType )
				return sPetViewInfo.m_szPetName;
		}
	}
	return szNull;
}

//�� ������ ���(UIǥ��)
ioHashString ioPetInfoManager::GetPetIcon( int nPetCode, PetRankType eRankType )
{
	static ioHashString szNull;

	PetBaseStatInfoList::iterator FindIter = m_PetBaseInfoList.find( nPetCode );
	if ( FindIter != m_PetBaseInfoList.end() )
	{
		PetBaseStatInfo sPetBaseInfo = FindIter->second;
		int nViewCnt = (int)sPetBaseInfo.m_PetViewInfoList.size();
		for ( int i=0; i<nViewCnt; ++i )
		{
			PetViewInfo sPetViewInfo = sPetBaseInfo.m_PetViewInfoList[i];
			if ( sPetViewInfo.m_PetRank == eRankType )
				return sPetViewInfo.m_szPetIcon;
		}
	}
	return szNull;
}

//�� �� ���(UIǥ��)
ioHashString ioPetInfoManager::GetPetModel( int nPetCode, PetRankType eRankType )
{
	static ioHashString szNull;

	PetBaseStatInfoList::iterator FindIter = m_PetBaseInfoList.find( nPetCode );
	if ( FindIter != m_PetBaseInfoList.end() )
	{
		PetBaseStatInfo sPetBaseInfo = FindIter->second;
		int nViewCnt = (int)sPetBaseInfo.m_PetViewInfoList.size();
		for ( int i=0; i<nViewCnt; ++i )
		{
			PetViewInfo sPetViewInfo = sPetBaseInfo.m_PetViewInfoList[i];
			if ( sPetViewInfo.m_PetRank == eRankType )
				return sPetViewInfo.m_szPetModel;
		}
	}
	return szNull;
}

//�� �� ũ�� ���(UIǥ��)
D3DXVECTOR3 ioPetInfoManager::GetPetModelScale( int nPetCode, PetRankType eRankType )
{
	PetBaseStatInfoList::iterator FindIter = m_PetBaseInfoList.find( nPetCode );
	if ( FindIter != m_PetBaseInfoList.end() )
	{
		PetBaseStatInfo sPetBaseInfo = FindIter->second;
		int nViewCnt = (int)sPetBaseInfo.m_PetViewInfoList.size();
		for ( int i=0; i<nViewCnt; ++i )
		{
			PetViewInfo sPetViewInfo = sPetBaseInfo.m_PetViewInfoList[i];
			if ( sPetViewInfo.m_PetRank == eRankType )
				return sPetViewInfo.m_vScale;
		}
	}
	return D3DXVECTOR3( FLOAT1, FLOAT1, FLOAT1 );
}

//�� �� �ִ� ���(UIǥ��)
ioHashString ioPetInfoManager::GetPetViewAni( int nPetCode, PetRankType eRankType )
{
	static ioHashString szNull;

	PetBaseStatInfoList::iterator FindIter = m_PetBaseInfoList.find( nPetCode );
	if ( FindIter != m_PetBaseInfoList.end() )
	{
		PetBaseStatInfo sPetBaseInfo = FindIter->second;
		int nViewCnt = (int)sPetBaseInfo.m_PetViewInfoList.size();
		for ( int i=0; i<nViewCnt; ++i )
		{
			PetViewInfo sPetViewInfo = sPetBaseInfo.m_PetViewInfoList[i];
			if ( sPetViewInfo.m_PetRank == eRankType )
				return sPetViewInfo.m_szPetAni;
		}
	}
	return szNull;
}

//�� ��ũ ���ھ��
ioHashString ioPetInfoManager::GetRankName( int nRank )
{
	//�� ��ũ ����
	ioHashString szRank;
	char szTemp[MAX_PATH];

	switch( (PetRankType)nRank )
	{
	case PRT_NONE:
		SafeSprintf( szTemp, sizeof( szTemp ), "Error" );
		break;
	case PRT_D:
		SafeSprintf( szTemp, sizeof( szTemp ), "D" );
		break;
	case PRT_C:
		SafeSprintf( szTemp, sizeof( szTemp ), "C" );
		break;
	case PRT_B:
		SafeSprintf( szTemp, sizeof( szTemp ), "B" );
		break;
	case PRT_A:
		SafeSprintf( szTemp, sizeof( szTemp ), "A" );
		break;
	case PRT_S:
		SafeSprintf( szTemp, sizeof( szTemp ), "S" );
		break;
	}
	szRank = szTemp;

	return szRank;
}

//��� �����
int ioPetInfoManager::GetResellPeso()
{
	return m_nResellPeso;
}