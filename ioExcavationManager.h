#pragma once


#include "IORandom.h"

class SP2Packet;
class ioPlayStage;
class ioBaseChar;

class ioExcavationManager : public Singleton< ioExcavationManager >
{
public:
	enum 
	{
		FAIL_ARTIFACT_START_TYPE   = 1000,
		EXTRAITEM_START_TYPE       = 10000,
	};

protected:

	struct ArtifactInfo
	{
		DWORD m_dwIndex;
		int   m_iX;   
		int   m_iY;   
		int   m_iZ;   

		ArtifactInfo()
		{
			m_dwIndex  = 0;
			m_iX = 0;
			m_iY = 0;
			m_iZ = 0;
		}

		void EncryptDecryptValue( char *pValue, int iSize )
		{
			for (int i = 0; i < iSize ; i++)
			{
				pValue[i] ^= 254;
				pValue[i] ^= 123;
				pValue[i] ^= 58;	
			}
		}

		void EncryptPos( int iX, int iY, int iZ )
		{
			enum { MAX_SIZE = 4, };
			char szValue[MAX_SIZE];
			memcpy( szValue, (void*) &iX, MAX_SIZE );
			EncryptDecryptValue( szValue, MAX_SIZE );  
			memcpy( (void*) &m_iX, szValue, MAX_SIZE );

			memcpy( szValue, (void*) &iY, MAX_SIZE );
			EncryptDecryptValue( szValue, MAX_SIZE );  
			memcpy( (void*) &m_iY, szValue, MAX_SIZE );

			memcpy( szValue, (void*) &iZ, MAX_SIZE );
			EncryptDecryptValue( szValue, MAX_SIZE );  
			memcpy( (void*) &m_iZ, szValue, MAX_SIZE );
		}

		void GetDecryptPos( OUT int &riX, OUT int &riY, OUT int &riZ )
		{
			enum { MAX_SIZE = 4, };
			char szValue[MAX_SIZE];
			memcpy( szValue, (void*) &m_iX, MAX_SIZE );
			EncryptDecryptValue( szValue, MAX_SIZE );  
			memcpy( (void*) &riX, szValue, MAX_SIZE );

			memcpy( szValue, (void*) &m_iY, MAX_SIZE );
			EncryptDecryptValue( szValue, MAX_SIZE );  
			memcpy( (void*) &riY, szValue, MAX_SIZE );

			memcpy( szValue, (void*) &m_iZ, MAX_SIZE );
			EncryptDecryptValue( szValue, MAX_SIZE );  
			memcpy( (void*) &riZ, szValue, MAX_SIZE );
		}
	};

	typedef std::vector<ArtifactInfo> vArtifactInfoVector;

	struct ItemInfo
	{
		int          m_iType;
		int          m_iValue1;
		ioHashString m_sName;
		ioHashString m_sDesc;
		ioHashString m_sIcon;

		ItemInfo()
		{
			m_iType   = 0;
			m_iValue1 = 0;
		}
	};

	typedef std::vector<ItemInfo> vItemInfoVector;

	struct GradeInfo
	{
		int          m_iType;
		ioHashString m_sRank;
		float        m_fRate;

		GradeInfo()
		{
			m_iType  = 0;
			m_fRate  = 0.0f;
		}
	};

	typedef std::vector<GradeInfo> vGradeInfoVector;

	typedef std::map< int, int > mExcavatingKitMap;

	// use etc item
	struct UseEtcItemInfo
	{
		int m_iEtcItemType;
		int m_iUsePoint;

		UseEtcItemInfo()
		{
			m_iEtcItemType = 0;
			m_iUsePoint    = 0;
		}
	};
	class UsePointSort : public std::binary_function< const UseEtcItemInfo&, const UseEtcItemInfo&, bool >
	{
	public:
		bool operator() ( const UseEtcItemInfo &lhs, const UseEtcItemInfo &rhs ) const
		{
			if( lhs.m_iUsePoint > rhs.m_iUsePoint )
				return true;
			return false;
		}
	};
	typedef std::vector<UseEtcItemInfo> vUseEtcItemInfoVector;

	struct MapMentInfo
	{
		int          m_iModeSubType;
		ioHashString m_sMent;

		MapMentInfo()
		{
			m_iModeSubType = 0;
		}
	};
	typedef std::vector<MapMentInfo> vMapMentInfoVector;

	struct ResultInfo
	{
		int m_iAddExp;
		int m_iClassType;
		int m_iAddSoldierExp;
		bool m_bLevelUp;
		bool m_bGradeLevelUp;

		void Clear()
		{
			m_iAddExp        = 0;
			m_iClassType     = 0; 
			m_iAddSoldierExp = 0;
			m_bLevelUp       = false;
			m_bGradeLevelUp  = false;
		}

		bool IsEmpty()
		{
			if( m_iAddExp == 0 && m_iClassType == 0 && m_iAddSoldierExp == 0 && m_bLevelUp == false && m_bGradeLevelUp == false )
				return true;

			return false;
		}

		ResultInfo()
		{
			Clear();
		}
	};

protected:
	vArtifactInfoVector       m_vArtifactInfoVector;
	vItemInfoVector           m_vItemInfoVector;
	vGradeInfoVector          m_vGradeInfoVector;
	mExcavatingKitMap         m_mExcavatingKitMap;
	vMapMentInfoVector        m_vMapMentInfoVector;
	DWORD                     m_dwRequestJudgementDelayTime;

	DWORD                     m_dwRequestJudgementTime;
	CEncrypt<int>             m_iExcavatingFirstCoolTime;
	CEncrypt<int>             m_iExcavatingCoolTime;

	IORandom                  m_CrateArtifactXPosRand;
	IORandom                  m_CrateArtifactZPosRand;

	DWORD                     m_dwKeyInputTimer;
	DWORD                     m_dwCurAutoOffTimer;
	DWORD                     m_dwMaxKeyInputTime;

	int                       m_iReserveRoomIndex;
	DWORD                     m_dwReserveArtifactIndex;

	int                       m_iRealExcavatingUserTime;
	int                       m_iSuccessRatePerOneUser;
	int                       m_iMaxSuccessRate;

	DWORD                     m_dwWaitArtifactPosTime;
	DWORD                     m_dwCurWaitArtifactPosTime;

	ResultInfo                m_kResultInfo;
	
protected:
	void _OnCreateArtifactPacket( SP2Packet &rkPacket, ioPlayStage *pPlayStage );
	void _OnEnterUserPacket( SP2Packet &rkPacket );
	void _OnArtifactPosionPacket( SP2Packet &rkPacket );
	void _OnDeleteArtifactPacket( SP2Packet &rkPacket );
	void _OnJudgementFailPacket( SP2Packet &rkPacket, ioPlayStage *pPlayStage );
	void _OnJudgementSuccessPacket( SP2Packet &rkPacket, ioPlayStage *pPlayStage );
	void _OnUseExcavatingKit( SP2Packet &rkPacket, ioPlayStage *pPlayStage ); 

	void DeleteArtifact( DWORD dwIndex );

	bool SendUseExcavatingKit( int iEtcItemType );

protected:
	int GetObjectItemCode( int iEtcItemType );

	void ProcessAutoOff( ioPlayStage *pPlayStage );
	void ProcessWaitArtifactPos( ioPlayStage *pPlayStage );
	void CheckAutoOffKey( ioPlayStage *pPlayStage );

	void SendArtifactPostion( ioPlayStage *pPlayStage, int iRoomIndex, DWORD dwArtifactIndex );

public:
	void OnExcavationPacket( SP2Packet &rkPacket, ioPlayStage *pPlayStage );
	void OnExcavationUDPPacket( SP2Packet &rkPacket, ioPlayStage *pPlayStage );

	void ClearMode();
	void SendRequestJudgement( ioBaseChar *pOwner, ioPlayStage *pPlayStage );
	void SendEndUsingExcavaingKit();

	void OnExcavationBtnUp( ioBaseChar *pOwner ); 

	bool SetExcavating( ioBaseChar *pOwner );

	void SetExcavatingSuccessAlarm( ioHashString &szSendID, int iArtifactType, int iArtifactValue1, int iArtifactValue2, float fMapRate, bool bAllAlarm );

	void SendReserveArtifactPosition( ioPlayStage *pPlayStage );

public:
	void LoadExcavation();

	int GetArtifactSize();
	int GetItemSize();
	int GetGradeSize();

	int GetItemArray( int iItemType );
	int GetGradeArray( int iGradeType );

	void GetItemName( IN int iArray, OUT ioHashString &rsName );
	void GetItemDesc( IN int iArray, OUT ioHashString &rsDesc );
	void GetItemIcon( IN int iArray, OUT ioHashString &rsIcon );
	void GetGradeRank( IN int iArray, OUT ioHashString &rsRank );
	float GetGradeRate( IN int iArray );

	void GetArtifactPos( IN int iArray, OUT int &riX, OUT int &riY, OUT int &riZ );
	int  GetArtifactIndex( int iArray );

	void GetMapMent( IN int iMapModeSubType, OUT ioHashString &rsMent );

public:
	int GetExcavatingFirstCoolTime();
	int GetExcavatingCoolTime();

	void ProcessPlayState( ioPlayStage *pPlayStage );
	void CheckInputKey( DWORD dwDirKey, DWORD dwActionKey, bool bExcavating );

	int GetRealExcavatingUserTime() const { return m_iRealExcavatingUserTime; }
	int GetMaxSuccessRate() const { return m_iMaxSuccessRate; }
	int GetSuccessRatePerOneUser() const { return m_iSuccessRatePerOneUser; }

	void SetKeyInputTimer(DWORD dwKeyInputTimer) { m_dwKeyInputTimer = dwKeyInputTimer; }

	void NoticeLevelAndExp();

public:
	static ioExcavationManager &GetSingleton();

public:
	ioExcavationManager(void);
	virtual ~ioExcavationManager(void);
};

#define g_ExcavationMgr ioExcavationManager::GetSingleton()

