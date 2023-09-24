#pragma once

enum PetRankType
{
	PRT_NONE = 0,
	PRT_D,
	PRT_C,
	PRT_B,
	PRT_A,
	PRT_S,
	PRT_MAX,
};

enum PetMainType
{
	PMT_NONE = 0,
	PMT_DEFAULT,
	PMT_MAX,
};

enum DefaultPetSubType
{
	DPST_NONE = 0,
	DPST_DEFAULT,
	DPST_MAX,
};

//�� ��ũ �� ����
typedef struct tagPetRankInfo
{
	//��ũ Ÿ��
	PetRankType m_RankType;
	//�ִ� ����
	int m_nMaxLevel;
	//��ũ �����
	float m_fRankValue;

	tagPetRankInfo()
	{
		m_RankType = PRT_NONE;
		m_nMaxLevel = 0;
		m_fRankValue = 0.0f;
	}
}PetRankInfo;

//�� ��ũ ���� ����Ʈ
typedef std::vector<PetRankInfo> PetRankInfoList;

//���� ��� ����
typedef struct tagPetAddStatInfo
{
	//�� ������
	int m_nPerLevel;
	//�� ����
	int m_nAddStat;

	tagPetAddStatInfo()
	{
		//���� ���� * 1/m_nPerLevel 
		m_nPerLevel = 1;
		m_nAddStat = 0;
	}
}PetAddStatInfo;

//���� ��� ���� ����Ʈ
typedef std::map<int, PetAddStatInfo> PetAddStatInfoList;

//�� Ÿ�� ����
typedef struct tagPetTypeInfo
{
	//�� ���� Ÿ��
	PetMainType m_PetMainType;
	//���� Ÿ��
	int m_nPetSubType;

	tagPetTypeInfo()
	{
		m_PetMainType = PMT_NONE;
		m_nPetSubType = 0;
	}
}PetTypeInfo;

typedef struct tagPetViewInfo
{
	PetRankType m_PetRank;
	ioHashString m_szPetName;
	ioHashString m_szPetIcon;
	ioHashString m_szPetModel;
	ioHashString m_szPetAni;
	D3DXVECTOR3 m_vScale;

	tagPetViewInfo()
	{
		m_PetRank = PRT_NONE;
		m_szPetName.Clear();
		m_szPetIcon.Clear();
		m_szPetModel.Clear();
		m_szPetAni.Clear();
	}
}PetViewInfo;

typedef std::vector<PetViewInfo> PetViewInfoList;

//�� ���� �⺻ ����
typedef struct tagPetStatBaseInfo
{
	//�⺻ ��ũ
	PetRankType m_BaseRank;
	//�ִ� ��ũ
	PetRankType m_MaxRank;
	//�⺻ ����
	IntVec m_vBaseGrowthStat;
	//�߰� ���� Ÿ�� ����Ʈ
	IntVec m_vAddStatTypeList;
	//�� ������
	PetViewInfoList m_PetViewInfoList;
	//�� ����
	int m_nManualCode;
	//�� ��õ����
	int m_nNeedMaterial;
	
	tagPetStatBaseInfo()
	{
		m_BaseRank = PRT_NONE;
		m_MaxRank = PRT_NONE;
		m_vBaseGrowthStat.clear();
		m_vAddStatTypeList.clear();
		m_PetViewInfoList.clear();
		m_nManualCode = 0;
		m_nNeedMaterial = 0;
	}
}PetBaseStatInfo;

//�� ���� �⺻ ���� ����Ʈ
typedef std::map< int, PetBaseStatInfo > PetBaseStatInfoList;

//���� ���� �� ����
typedef struct tagCurPetInfo
{	
	//�� �ڵ�->�ڵ�� �⺻������ ã��
	int m_nPetCode;
	//���� ��ũ->�� ������ �ҷ���
	PetRankType m_CurRankType;
	//���� ����->���� ����
	int m_nCurLevel;

	tagCurPetInfo()
	{
		m_nPetCode = 0;
		m_CurRankType = PRT_NONE;
		m_nCurLevel	= 0;
	}
}CurPetInfo;

typedef std::map<ioHashString, CurPetInfo> CurPetInfoList;

//�� �� ����
typedef struct tagPetModelInfo
{
	//��ũ Ÿ��
	PetRankType m_PetRankType;
	//�� ���ϸ�
	ioHashString m_PetModel;
	//������ ��
	D3DXVECTOR3 m_vScale;

	tagPetModelInfo()
	{
		m_PetRankType = PRT_NONE;
		m_vScale = ioMath::VEC3_ZERO;
	}
}PetModelInfo;

//�� �� ���� ����Ʈ
typedef std::vector<PetModelInfo> PetModelInfoList;

//�� �⺻��ƼƼ ����
typedef struct tagPetBaseInfo
{
	//���� ��ġ
	ioHashString m_PetPath;
	//Ÿ��Ʋ
	ioHashString m_PetTitle;
	//�� Ÿ��
	PetTypeInfo m_PetTypeInfo;
	//�� ����
	PetModelInfoList m_vPetModelInfoList;

	tagPetBaseInfo()
	{
		m_PetPath.Clear();
		m_PetTitle.Clear();
		m_vPetModelInfoList.clear();
	}
}PetBaseInfo;

//�� �⺻��ƼƼ ���� ����Ʈ
typedef std::map<int, PetBaseInfo> PetBaseInfoList;

//�� ����
typedef struct tagPetSlot
{
	//�� �ڵ�
	int m_nPetCode;
	//��ũ
	int m_PetRankType;
	//�ε���
	int m_nIndex;
	//����, ����ġ
	int m_nCurLevel;
	int m_CurExp; 
	bool m_bEquip;

	//�̸�, ������
	ioHashString m_Name;
	ioHashString m_IconName;

	tagPetSlot()
	{
		m_nPetCode = 0;
		m_PetRankType = 0;
		m_nIndex = 0;
		m_nCurLevel = 0;
		m_CurExp = 0;
		m_bEquip = false;
		m_Name.Clear();
		m_IconName.Clear();
	}
}PetSlot;

//�� ���� ����Ʈ
typedef std::vector<PetSlot> PetSlotList;

//�� ���� ����
class PetSlotSort : public std::binary_function< const PetSlot&, const PetSlot&, bool >
{
public:
	bool operator()( const PetSlot &lhs , const PetSlot &rhs ) const
	{
		if( lhs.m_PetRankType > rhs.m_PetRankType )
			return true;

		if( lhs.m_PetRankType == rhs.m_PetRankType )
		{
			if( lhs.m_nPetCode < rhs.m_nPetCode )
				return true;	

			if( lhs.m_nPetCode == rhs.m_nPetCode )
			{
				if( lhs.m_nCurLevel > rhs.m_nCurLevel )
					return true;	
				return false;
			}
			return false;			
		}
		return false;
	}
};

//�� ������Ʈ�� �ִϸ��̼� ����
typedef struct tagPetStateAniInfo
{
	int m_nState;
	ioHashString m_szAnimation;
	float m_fAniRate;

	tagPetStateAniInfo()
	{
		m_nState = 0;
		m_szAnimation.Clear();
		m_fAniRate = FLOAT1;
	}
}PetStateAniInfo;

typedef std::vector<PetStateAniInfo> PetStateAniInfoList;

typedef std::map<int, PetStateAniInfoList> PetRankAniInfoList;

//�� ��ũ�� ����Ʈ �ִϸ��̼� ����
typedef struct tagPetDefaultAniInfo
{
	ioHashString m_szAnimation;
	float m_fAniRate;

	tagPetDefaultAniInfo()
	{
		m_szAnimation.Clear();
		m_fAniRate = FLOAT1;
	}
}PetDefaultAniInfo;

typedef std::map<int, PetDefaultAniInfo> PetDefaultAniInfoList;

typedef struct tagPetPullDownInfo
{
	int m_nPetCode;
	int m_nPetRank;
	int m_nSlotIndex;

	tagPetPullDownInfo()
	{
		m_nPetCode = 0;
		m_nPetRank = 0;
		m_nSlotIndex = 0;
	}
}PetPullDownInfo;

typedef std::vector<PetPullDownInfo> PetPullDownInfoList;

/////////////////////////////////////////////////////////////////////////////////////////////////////
class PetPullDownInfoSort : public std::binary_function< const PetPullDownInfo&, const PetPullDownInfo&, bool >
{
public:
	bool operator() ( const PetPullDownInfo &lhs, const PetPullDownInfo &rhs ) const
	{
		if( lhs.m_nPetRank > rhs.m_nPetRank )
			return true;

		if( lhs.m_nPetRank == rhs.m_nPetRank )
		{
			if( lhs.m_nPetCode < rhs.m_nPetCode )
				return true;	
		}
		return false;
	}
};