#pragma once

enum AccessoryPeriodType
{
	ACCPT_TIME		= 0,
	ACCPT_MORTMAIN	= 1,
};

enum
{
	DEFAULT_BASIC_SLOT = 5,
	DEFAULT_BASIC_ACCESSORY_CODE = 1000000,
};

typedef struct tagAccessoryEquipInfo
{
	int m_iAccessoryIdx;
	int m_iAccessoryCode;
	int m_iRandomNumber;

	void Init()
	{
		m_iAccessoryIdx = 0;
		m_iAccessoryCode = 0;
		m_iRandomNumber = 0;
	}

	tagAccessoryEquipInfo()
	{
		Init();
	}

	tagAccessoryEquipInfo( const tagAccessoryEquipInfo &rhs )
	{
		m_iAccessoryIdx = rhs.m_iAccessoryIdx;
		m_iAccessoryCode = rhs.m_iAccessoryCode;
		m_iRandomNumber = rhs.m_iRandomNumber;
	}

	bool operator==( const tagAccessoryEquipInfo &rhs ) const
	{
		if( m_iAccessoryIdx != rhs.m_iAccessoryIdx )
			return false;
		if( m_iAccessoryCode != rhs.m_iAccessoryCode )
			return false;
		if ( m_iRandomNumber != rhs.m_iRandomNumber )
			return false;

		return true;
	}

	bool operator!=( const tagAccessoryEquipInfo &rhs ) const
	{
		if( *this == rhs )
			return false;

		return true;
	}

}AccessoryEquipInfo;

//�׼����� ����
typedef struct tagAccessoryInfo
{
	int m_iAccessoryCode;
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

	RopeSpringParam *m_pAccessorySpringParam;

	ioHashString m_szName;
	ioHashString m_szIconName;

	int m_nEnableClass;
	int m_nManual;
	int m_nDisassembleCode;
	int m_nDisassembleMinCnt;
	int m_nDisassembleMaxCnt;

	void Init()
	{
		m_iAccessoryCode = 0;
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

		m_pAccessorySpringParam = NULL;

		m_szName.Clear();
		m_szIconName.Clear();

		m_nEnableClass = 0;
		m_nManual = 0;
		m_nDisassembleCode = 0;
		m_nDisassembleMinCnt = 0;
		m_nDisassembleMaxCnt = 0;
	}

	tagAccessoryInfo()
	{
		Init();
	}

	tagAccessoryInfo( const tagAccessoryInfo &rhs )
	{
		m_iAccessoryCode = rhs.m_iAccessoryCode;
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

		if ( rhs.m_pAccessorySpringParam )
			m_pAccessorySpringParam = rhs.m_pAccessorySpringParam;
		else
			m_pAccessorySpringParam = NULL;

		m_szName = rhs.m_szName;
		m_szIconName = rhs.m_szIconName;

		m_nEnableClass = rhs.m_nEnableClass;
		m_nManual = rhs.m_nManual;
		m_nDisassembleCode = rhs.m_nDisassembleCode;
		m_nDisassembleMinCnt = rhs.m_nDisassembleMinCnt;
		m_nDisassembleMaxCnt = rhs.m_nDisassembleMaxCnt;
	}

}AccessoryInfo;

typedef std::map<int, AccessoryInfo> AccessoryInfoList;

typedef struct tagAccessorySlot
{
	int m_iAccessoryCode;
	//�ε���
	int m_iIndex;
	int m_iRandomNumber;

	byte m_PeriodType;
	int m_iValue1;     // ������� ��Ÿ�� 20090715(2009�� 7�� 15�� )
	int m_iValue2;     // �ð��� ��Ÿ�� 1232 (12��32��)

	DWORD m_dwMaleCustom;
	DWORD m_dwFemaleCustom;

	bool m_bEquip;

	//�̸�, ������
	ioHashString m_Name;
	ioHashString m_IconName;

	tagAccessorySlot()
	{
		Init();
	}

	void Init()
	{
		m_iAccessoryCode = 0;
		m_iIndex = 0;
		m_iRandomNumber = 0;

		m_PeriodType = ACCPT_TIME;
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
}AccessorySlot;

//�׼����� ���� ����Ʈ
typedef std::vector<AccessorySlot> AccessorySlotList;

//�׼����� ���� ����
class AccessorySlotSort : public std::binary_function< const AccessorySlot&, const AccessorySlot&, bool >
{
public:
	bool operator()( const AccessorySlot &lhs , const AccessorySlot &rhs ) const
	{
		if( lhs.m_bEquip )
			return true;

		if( lhs.m_iAccessoryCode < rhs.m_iAccessoryCode )
			return true;	

		if( lhs.m_iAccessoryCode == rhs.m_iAccessoryCode )
		{
			if( lhs.m_iIndex > rhs.m_iIndex )
				return true;	
			return false;
		}
		return false;	
	}
};

//�׼����� ���� ����
typedef struct tagAccessoryShopSubInfo
{
	int m_iAccessoryCode;
	int m_nShopMarkType;
	float m_iRandomNumber;
	int m_nLimitDate;
	int m_nOrder;

	tagAccessoryShopSubInfo()
	{
		m_iAccessoryCode = 0;
		m_nShopMarkType = 0;
		m_nLimitDate = 0;
		m_nOrder = 0;
		m_iRandomNumber = 0;
	}
}AccessoryShopSubInfo;

typedef std::vector<AccessoryShopSubInfo> AceessoryShopSubInfoList;

class AccessoryShopSubInfoSort : public std::binary_function< const AccessoryShopSubInfo& , const AccessoryShopSubInfo&, bool >
{
public:
	bool operator() ( const AccessoryShopSubInfo &lhs, const AccessoryShopSubInfo &rhs ) const
	{
		if( lhs.m_nOrder > rhs.m_nOrder )
			return true;
		if( lhs.m_nOrder == rhs.m_nOrder )
		{
			if( lhs.m_iAccessoryCode > rhs.m_iAccessoryCode )
				return true;
		}

		return false;
	}
};