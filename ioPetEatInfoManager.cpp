
#include "stdafx.h"

#include "ioPetEatInfoManager.h"

template<> ioPetEatInfoManager* Singleton< ioPetEatInfoManager >::ms_Singleton = 0;

ioPetEatInfoManager::ioPetEatInfoManager()
{
	ClearData();
}

ioPetEatInfoManager::~ioPetEatInfoManager()
{
	ClearData();
}

ioPetEatInfoManager& ioPetEatInfoManager::GetSingleton()
{
	return Singleton< ioPetEatInfoManager >::GetSingleton();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//데이터 비우기
void ioPetEatInfoManager::ClearData()
{
	int nRankInfoSize = (int)m_RankMaterialEatInfo.size();
	if ( nRankInfoSize > 0 )
	{
		for (int i=0; i<nRankInfoSize; ++i)
			m_RankMaterialEatInfo[i].m_MaterialInfoList.clear();
	}
	m_RankMaterialEatInfo.clear();
}

//데이터 읽기
void ioPetEatInfoManager::LoadProperty()
{	
	ioINILoader_e rkLoader( "config/sp2_pet_eat_info.ini" );

	LoadPetEatInfo( rkLoader );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//조각 정보 설정
void ioPetEatInfoManager::LoadPetEatInfo( ioINILoader &rkLoader )
{
	char szBuf[MAX_PATH];

	rkLoader.SetTitle_e( "pet_eat_info" );

	int nMaxInfo = rkLoader.LoadInt_e( "max_rank_info_cnt", 0 );
	m_RankMaterialEatInfo.clear();
	m_RankMaterialEatInfo.reserve( nMaxInfo );

	for ( int i=0; i<nMaxInfo; ++i )
	{
		RankMaterialEatInfo sRankMaterialEatInfo;

		wsprintf_e( szBuf, "eat_info%d_rank", i+1 );
		sRankMaterialEatInfo.m_nRankType = (PetRankType)rkLoader.LoadInt( szBuf, PRT_NONE );
		
		wsprintf_e( szBuf, "eat_info%d_max_level", i+1 );
		int nLvInfo = rkLoader.LoadInt( szBuf, 0 );
		for ( int j=0; j<nLvInfo; ++j )
		{
			wsprintf_e( szBuf, "eat_info%d_material%d", i+1, j+1 );
			int nMaterialCnt = rkLoader.LoadInt( szBuf, 0 );
			
			sRankMaterialEatInfo.m_MaterialInfoList.push_back( nMaterialCnt );
		}

		m_RankMaterialEatInfo.push_back( sRankMaterialEatInfo );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int ioPetEatInfoManager::GetNeedMaterialCnt( int nRankType, int nCurLevel )
{
	if ( nRankType <= PRT_NONE || nRankType >= PRT_MAX )
		return 0;

	int nMaxInfo = (int)m_RankMaterialEatInfo.size();
	for ( int i=0; i<nMaxInfo; ++i )
	{
		if ( m_RankMaterialEatInfo[i].m_nRankType == nRankType )
		{
			if ( nCurLevel < 0 || nCurLevel >= (int)m_RankMaterialEatInfo[i].m_MaterialInfoList.size() )
				return 0;

			return m_RankMaterialEatInfo[i].m_MaterialInfoList[nCurLevel];
		}
	}
	return 0;
}
