#pragma once

class ioPetEatInfoManager : public Singleton< ioPetEatInfoManager >
{
public:
	struct RankMaterialEatInfo
	{
		int m_nRankType;
		IntVec m_MaterialInfoList;

		RankMaterialEatInfo()
		{
			m_nRankType = 0;
			m_MaterialInfoList.clear();
		}
	};

	typedef std::vector<RankMaterialEatInfo> RankMaterialEatInfoList;

protected:
	RankMaterialEatInfoList m_RankMaterialEatInfo;

public:
	static ioPetEatInfoManager& GetSingleton();

public:
	//데이터 비우기
	void ClearData();
	//데이터 읽기
	void LoadProperty();

protected:
	//랭크 정보
	void LoadPetEatInfo( ioINILoader &rkLoader );

public:
	int GetNeedMaterialCnt( int nRankType, int nCurLevel );

public:
	ioPetEatInfoManager();
	virtual ~ioPetEatInfoManager();
};

#define g_PetEatInfoMgr ioPetEatInfoManager::GetSingleton()