#pragma once

enum PremiumCostumeSubType
{
	PCST_Armor = 1,
	PCST_Helmet,
	PCST_Cloak,
	PCST_MAX
};

enum
{
	PCPT_TIME		= 0,
	PCPT_MORTMAIN	= 1,
};

enum
{
	DEFAULT_BASIC_COSTUME_CODE = 100000,
};

typedef struct tagCostumeEquipInfo
{
	int m_CostumeIdx;
	int m_CostumeCode;
	int m_Costume_Male_Custom;  
	int m_Costume_Female_Custom;

	void Init()
	{
		m_CostumeIdx = 0;
		m_CostumeCode = 0;
		m_Costume_Male_Custom = 0;  
		m_Costume_Female_Custom = 0;
	}

	tagCostumeEquipInfo()
	{
		Init();
	}

	tagCostumeEquipInfo( const tagCostumeEquipInfo &rhs )
	{
		m_CostumeIdx = rhs.m_CostumeIdx;
		m_CostumeCode = rhs.m_CostumeCode;
		m_Costume_Male_Custom = rhs.m_Costume_Male_Custom;  
		m_Costume_Female_Custom = rhs.m_Costume_Female_Custom;
	}

	bool operator==( const tagCostumeEquipInfo &rhs ) const
	{
		if( m_CostumeIdx != rhs.m_CostumeIdx )
			return false;
		if( m_CostumeCode != rhs.m_CostumeCode )
			return false;
		if( m_Costume_Male_Custom != rhs.m_Costume_Male_Custom )
			return false;
		if( m_Costume_Female_Custom	!= rhs.m_Costume_Female_Custom )
			return false;

		return true;
	}

	bool operator!=( const tagCostumeEquipInfo &rhs ) const
	{
		if( *this == rhs )
			return false;

		return true;
	}

}CostumeEquipInfo;

//�ڽ�Ƭ ����
typedef struct tagCostumeInfo
{
	int m_nCostumeCode;
	int m_nGradeType;
	D3DCOLORVALUE m_GlowColor;
	ioHashString m_szEffect;
	ioHashString m_szEffectHW;
	ioHashString m_szSpring_Biped;
	
	int m_nArmorType;
	int m_nArmorSoundType;
	ioHashString m_szBodyMesh;
	ioHashString m_szHandMesh;

	int m_nInvisibleFaceType;
	int m_nFaceHM;
	int m_nFaceHW;
	int m_nHairHM;
	int m_nHairHW;
	ioHashString m_szHelmetMesh;

	ioHashString m_szCloakMesh;

	RopeSpringParam *m_pCostumeSpringParam;

	ioHashString m_szName;
	ioHashString m_szIconName;

	int m_nEnableClass;
	int m_nManual;
	int m_nDisassembleCode;
	int m_nDisassembleMinCnt;
	int m_nDisassembleMaxCnt;

	void Init()
	{
		m_nCostumeCode = 0;
		m_nGradeType = 0;
		m_GlowColor.r = 0.0f;
		m_GlowColor.g = 0.0f;
		m_GlowColor.b = 0.0f;
		m_GlowColor.a = 1.0f;

		m_szEffect.Clear();
		m_szEffectHW.Clear();
		m_szSpring_Biped.Clear();
		
		m_nArmorType = 0;
		m_nArmorSoundType = 0;
		m_szBodyMesh.Clear();
		m_szHandMesh.Clear();

		m_nInvisibleFaceType = 0;
		m_nFaceHM = 0;
		m_nFaceHW = 0;

		m_nHairHM = 0;
		m_nHairHW = 0;
		m_szHelmetMesh.Clear();

		m_szCloakMesh.Clear();

		m_pCostumeSpringParam = NULL;

		m_szName.Clear();
		m_szIconName.Clear();

		m_nEnableClass = 0;
		m_nManual = 0;
		m_nDisassembleCode = 0;
		m_nDisassembleMinCnt = 0;
		m_nDisassembleMaxCnt = 0;
	}

	tagCostumeInfo()
	{
		Init();
	}
	
	tagCostumeInfo( const tagCostumeInfo &rhs )
	{
		m_nCostumeCode = rhs.m_nCostumeCode;
		m_nGradeType = rhs.m_nGradeType;
		m_GlowColor.r = rhs.m_GlowColor.r;
		m_GlowColor.g = rhs.m_GlowColor.g;
		m_GlowColor.b = rhs.m_GlowColor.b;
		m_GlowColor.a = rhs.m_GlowColor.a;

		m_szEffect = rhs.m_szEffect;
		m_szEffectHW = rhs.m_szEffectHW;
		m_szSpring_Biped = rhs.m_szSpring_Biped;
				
		m_nArmorType = rhs.m_nArmorType;
		m_nArmorSoundType = rhs.m_nArmorSoundType;
		m_szBodyMesh = rhs.m_szBodyMesh;
		m_szHandMesh = rhs.m_szHandMesh;

		m_nInvisibleFaceType = rhs.m_nInvisibleFaceType;
		m_nFaceHM = rhs.m_nFaceHM;
		m_nFaceHW = rhs.m_nFaceHW;

		m_nHairHM = rhs.m_nHairHM;
		m_nHairHW = rhs.m_nHairHW;
		m_szHelmetMesh = rhs.m_szHelmetMesh;

		m_szCloakMesh = rhs.m_szCloakMesh;

		if ( rhs.m_pCostumeSpringParam )
			m_pCostumeSpringParam = rhs.m_pCostumeSpringParam;
		else
			m_pCostumeSpringParam = NULL;

		m_szName = rhs.m_szName;
		m_szIconName = rhs.m_szIconName;

		m_nEnableClass = rhs.m_nEnableClass;
		m_nManual = rhs.m_nManual;
		m_nDisassembleCode = rhs.m_nDisassembleCode;
		m_nDisassembleMinCnt = rhs.m_nDisassembleMinCnt;
		m_nDisassembleMaxCnt = rhs.m_nDisassembleMaxCnt;
	}

}CostumeInfo;

typedef std::map<int, CostumeInfo> CostumeInfoList;

//�ڽ�Ƭ ����
typedef struct tagCostumeSlot
{
	//�ڽ�Ƭ �ڵ�
	int m_nCostumeCode;
	//�ε���
	int m_nIndex;

	byte m_PeriodType;
	int m_iValue1;     // ������� ��Ÿ�� 20090715(2009�� 7�� 15�� )
	int m_iValue2;     // �ð��� ��Ÿ�� 1232 (12��32��)

	DWORD m_dwMaleCustom;
	DWORD m_dwFemaleCustom;

	bool m_bEquip;

	//�̸�, ������
	ioHashString m_Name;
	ioHashString m_IconName;

	tagCostumeSlot()
	{
		Init();
	}

	void Init()
	{
		m_nCostumeCode = 0;
		m_nIndex = 0;

		m_PeriodType = PCPT_TIME;
		m_iValue1 = 0;
		m_iValue2 = 0;
		m_dwMaleCustom = 0;
		m_dwFemaleCustom = 0;

		m_bEquip = false;
		m_Name.Clear();
		m_IconName.Clear();
	}

	// ��¥��
	SHORT GetYear()
	{
		return m_iValue1/10000;           // [2009]0715
	}
	SHORT GetMonth()
	{
		return ( m_iValue1/100 ) % 100;   //  2009[07]15
	}
	SHORT GetDay()
	{
		return m_iValue1 % 100;           //  200907[15]
	}
	SHORT GetHour()
	{
		return m_iValue2 / 100;           //  [21]23   ( 21�� 23�� )
	}
	SHORT GetMinute()
	{
		return m_iValue2 % 100;           //  21[23]
	}
	void SetDate( int iYear , int iMonth, int iDay, int iHour, int iMinute )
	{
		m_iValue1 = ( iYear * 10000 ) + ( iMonth * 100 ) + iDay;
		m_iValue2 = ( iHour * 100 ) + iMinute;
	}
}CostumeSlot;

//�ڽ�Ƭ ���� ����Ʈ
typedef std::vector<CostumeSlot> CostumeSlotList;

//�ڽ�Ƭ ���� ����
class CostumeSlotSort : public std::binary_function< const CostumeSlot&, const CostumeSlot&, bool >
{
public:
	bool operator()( const CostumeSlot &lhs , const CostumeSlot &rhs ) const
	{
		if( lhs.m_bEquip )
			return true;
				
		if( lhs.m_nCostumeCode < rhs.m_nCostumeCode )
			return true;	

		if( lhs.m_nCostumeCode == rhs.m_nCostumeCode )
		{
			if( lhs.m_nIndex > rhs.m_nIndex )
				return true;	
			return false;
		}
		return false;	
	}
};

//�ڽ�Ƭ ���� ����
typedef struct tagCostumeShopSubInfo
{
	int m_nCostumeCode;
	int m_nShopMarkType;
	int m_nLimitDate;
	int m_nOrder;

	tagCostumeShopSubInfo()
	{
		m_nCostumeCode = 0;
		m_nShopMarkType = 0;
		m_nLimitDate = 0;
		m_nOrder = 0;
	}
}CostumeShopSubInfo;

typedef std::vector<CostumeShopSubInfo> CostumeShopSubInfoList;

class CostumeShopSubInfoSort : public std::binary_function< const CostumeShopSubInfo& , const CostumeShopSubInfo&, bool >
{
public:
	bool operator() ( const CostumeShopSubInfo &lhs, const CostumeShopSubInfo &rhs ) const
	{
		if( lhs.m_nOrder > rhs.m_nOrder )
			return true;
		if( lhs.m_nOrder == rhs.m_nOrder )
		{
			if( lhs.m_nCostumeCode > rhs.m_nCostumeCode )
				return true;
		}

		return false;
	}
};