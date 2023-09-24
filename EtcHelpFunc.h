#pragma once

#include "iepmapi.h"

#define REG_SHLM HKEY_LOCAL_MACHINE
#define REG_SHCU HKEY_CURRENT_USER
#define REG_SHCR HKEY_CLASSES_ROOT
#define REG_KEY_IO "Software\\SP2Client\\"
#define REG_KEY_CPU "Hardware\\Description\\System\\CentralProcessor\\0\\"

#include "CharacterInfo.h"

class ioEntity;
class ioEntityGroup;
class ioBaseChar;
class ioEffect;
class ioTexture;
class ioHashString;
class ioINILoader;

namespace Help
{
	void InitHelpConstant();
	void InitColorValue( D3DCOLORVALUE &rkColor, float fRed, float fGreen, float fBlue, float fAlpha );

	ioINILoader& GetCharLoader();
	ioINILoader& GetCharLoader( int iCharType, int iSex );

	int  RegReadInt(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, int iDefault );
	bool RegWriteInt(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, int iData);

	bool RegReadString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpDefault, LPTSTR lpRet, DWORD nSize);
	bool RegWriteString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData);

	void ChangeColorTexAndOutLine( ioEntity *pEntity,
								   const ioHashString &szTexture,
								   const D3DCOLORVALUE *pOutLine = NULL );

	void FaceChangeColorTexAndOutLine( ioEntity *pEntity, 
									   const ioHashString &szTexture,
									   const D3DCOLORVALUE *pOutLine = NULL );

	void ChangeSkinColor( ioEntityGroup *pGrp, 
						  const D3DCOLORVALUE &rkSkin,
						  const D3DCOLORVALUE &rkOutLine );

	void ChangeMaterial( ioEntityGroup *pGrp, const ioHashString &szMaterialName, bool bSoft );
	void RestoreMaterial( ioEntityGroup *pGrp, bool bSoft );

	void ChangeMaterialTex( ioEntityGroup *pGrp, const ioHashString &szName );
	void RestoreMaterialTex( ioEntityGroup *pGrp );

	void CustomEntityGroupTexture( ioEntityGroup *pGrp, const ioHashString &rkTexDst, const ioHashString &rkTexSrc, int iCustomValue );
	void CustomEntityTexture( ioEntity *pEntity, const ioHashString &rkTexDst, const ioHashString &rkTexSrc, int iCustomValue );
	void CustomEffectTexture( ioEffect *pEffect, const ioHashString &rkTexDst, const ioHashString &rkTexSrc, int iCustomValue );
	void CustomEntityTextureNameCnt( ioEntity *pEntity, const DWORD dwCustomCode, CustomTextureList &rkReturn, BOOL bCostume = FALSE );
	void CustomEffectTextureNameCnt( ioEffect *pEffect, const DWORD dwCustomCode, CustomTextureList &rkReturn );
	ioTexture *CustomEntityOutTexture( ioEntity *pEntity, const ioHashString &rkTexDst );
	bool CustomTextureToJPG( ioTexture *pTexture, const char *szFileName );
	bool CustomTextureToDDS( ioTexture *pTexture, const char *szFileName );
	int  CustomTextureJPGMixDDSFromDDS( const ioHashString &rkDDS, const ioHashString &rkJPG, const ioHashString &rkSave );
	bool CompressLSCFile( const ioHashString &rkFilePath );
	bool DecompressLSCMemory( unsigned char *pFileMemory, int iFileSize );

	bool IsAlNum(const char *szValue, const int iSize);
	void ChangeCharSet( char *szText, char cDst, char cSrc );

	bool IsKoreanOS();
	void ExcuteProcess( char* szFileName , char* szCmdLine );
	void MinimizeAllWindows(BOOL bMinimize);

	bool GetDXVersionInfo( DWORD *pVesion, char *szVersion, int iBufLen );
	void PrintMatrix( const D3DXMATRIX *pMat );

	void HeadBlankClear( char *szBuf );
	void GetActiveWindowTitle(DWORD dwThreadID, OUT char *szName, int iSize);

	bool CheckSocketUDPPortUsed( int iPort );
	void ExcuteWeb( const char *szURL );
	void ExcuteIE( OUT PROCESS_INFORMATION &process_info ,IN const char *szURL );
	void ExcuteIE( OUT PROCESS_INFORMATION &IEProcessInfo, OUT IELAUNCHURLINFO& LunchInfo, IN const char *szURL );

	DWORD GetLanCableConnectState();
	void CreateDefaultSettingINI();

	int GetFileSize(const char *szFileName);
	void SetCreateDirectory( const char *szDir );

	bool GetAllProcessName(OUT char *szAllProcessName, IN int iSize);

	int GetFriendBonusBeforeMin();
	float GetFriendBonus();
	float GetMaxFriendBonus();
	float GetPcRoomFriendBonus();
	float GetPcRoomMaxFriendBonus();

	DWORD GetLastPCRoomBonusDate();
	void  SavePCRoomBonusDate( DWORD dwSaveDate );
	
	//
	int   GetPlayingUserBonusCount();
	DWORD GetScorllInActiveTime();

	//
	int GetRefillCoinCount();
	int GetRefillCoinMax();
	int GetRefillCoinSec();

	//
	int GetBadNetworkKickCount();

	//
	int GetSoldierPesoBuyDateLimit();

	//
	int GetPlazaMaxPlayer();
	//
	enum VolumeType 
	{
		VT_NONE   = 0,
		VT_NORMAL = 1,
		VT_3D     = 2,
		VT_STREAM = 3,
	};
	// 0~MAX_VOLUME-1 : �ܰ��� MAX_VOLUME
	enum
	{
		MAX_VOLUME = 10,
	};
	void LoadVolumesRate();
	int  GetDSVolume( int iVolume , VolumeType eVolumeType );
	//

	ioHashString ConvertName( const ioHashString &rkName, bool bMale );
	ioHashString ConvertTailName( const ioHashString &rkName, bool bMale ) /* XXXX[add].XXX */;
	ioHashString ConvertTailNameAndFileCheck( const ioHashString &rkName, bool bMale ) /* XXXX[add].XXX */;
	int StringCutFun( float fScale, float fWidth, int iTextStyle, OUT char *szDst, int iDstSize, const char *szSrc );
	void ConvertNumToStrComma( int iSourceNum, OUT char *szDestCommaNum, int iDestSize );

	//
	void SetDeveloperKCharName( const char *szCharName );
	const ioHashString &GetDeveloperKCharName();
	//

	void GetRemainTime( int a_iYear, int a_iMonth, int a_iDay, int a_iHour, int a_iMinute, OUT char *szTime, int iTimeSize );
	// GetRemainTime() �������� string ���� �����ϱ� ���� �߰�	JCLEE 131320
	std::string GetRemainTimeToStr( int a_iYear, int a_iMonth, int a_iDay, int a_iHour, int a_iMinute, OUT char *szTime, int iTimeSize );
	// GetRemainTime() �������� integer minite ���� �����ϱ� ���� �߰�	JCLEE 131320
	int GetRemainTimeToMin( int a_iYear, int a_iMonth, int a_iDay, int a_iHour, int a_iMinute );

	void GetGrowthTitle( IN int iArray, IN bool bTimeSlot, OUT ioHashString &rsGrowthTitle );
	void GetNotCutWndPos( IN int iMouseX, IN int iMouseY, IN int iWndWidth, IN int iWndHeight, OUT int &riXPos, OUT int &riYPos );
	void TextCut( IN const char *pSrc, IN int iCutWidth, IN TextStyle eTextStyle, IN float fScale, OUT ioHashString &rsReturn );
	void SaveClientError( int iErrorNumber, int iDesc1, int iDesc2 );

	void SetCharChangeToUDP( bool bUDP );
	bool IsCharChangeToUDP();

	void SetOnlyServerRelay( bool bRelay );
	bool IsOnlyServerRelay();

	void SetWholeChatOn( bool bOn );
	bool IsWholeChatOn();

	void SetWholeRainbowChatOn( bool bOn );
	bool IsWholeRainbowChatOn();

	int GetDefaultBestFriendCount();
	int GetBestFriendDismissDelayHour();
	int GetCharRentalMinute();
	int GetBestFriendCharRentalDelayHour();
	int GetCharRentalCount();
	int GetCharRentalGrade();
	int GetMaxCharRentSet();

	void SetSendReciveCheck( bool bCheck );
	bool CheckEnableTimeGapUser( int iType=0 );
	bool CheckEnableTimeGapUser2( int iType=0 );

	int TournamentRound( int iMaxTeam );
	int TournamentCurrentRoundWithTeam( int iMaxTeam, int iCurrentRound );
	void ConvertTournamentRoundResultText( OUT ioHashString& szDestTtitle, int iMaxTeam, int iPos );
	void ConvertTournamentRoundProressText( OUT ioHashString& szDestTtitle, int iMaxTeam, int iPos );

	// dump
	bool CaptureScreen();
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	bool CaptureScreenWithJPEG();
	//

	void SetRaidModeMVPTreasureCard( int iMVPTreasureCard );
	int  GetRaidModeMVPTreasureCard();

	// Monster Dungeon
	bool IsMonsterDungeonMode(ModeType eMode);

	D3DXQUATERNION ConvertDirToQuaternion( const D3DXVECTOR3 &vDir );

	void GetParsingDate( IN int iDate, IN int iYearOffset, OUT int &iYear, OUT int &iMonth, OUT int &iDay, OUT int &iHour, OUT int &iMinute );

	bool CheckHanByte( char *szText );
	void SplitTextWidth( std::string szSource, StringVector& szOutSplitList, int iCutWith, TextStyle eStyle, float fTextScale );
	bool IsOnlyEngHanNumText( const char *szText );

	// Char�� �浹üũ(������ ���Ѱ�) ����
	bool CheckCharColState( DWORD dwStartTime, DWORD dwEndTime,
							DWORD dwSkipType, bool bTeamOnly, bool bDefense,
							TeamType eOwnerTeam, TeamType eOtherTeam );

	// For Char

	// �浹ü�� �ǰݵ��� �ʰ� ������ �ɸ��� �ʴ� �⺻ state
	bool CheckWeaponBuffNoCollision( CharState eState );
	// �ѱ�� ����, ��ų, ���� ĳ����, ������� Ÿ������ �ɸ��� �ʴ� �⺻ state
	bool CheckTargetNoCollision( CharState eState );
	// ���ڷ��� Ÿ������ �ɸ��� �ʴ� �⺻ state
	bool CheckMineNoCollision( CharState eState );

	// 1/4 sine y�� ���
	float GetHalfSineValue( float max, float cur );

	// dummy char create, destroy info
	void LoadDummyCharCreateDestroyInfo( ioINILoader &rkLoader,
										 vDummyCharLoadInfoList &rkCreateList,
										 ioHashStringVec &rkDestroyList );

	//���� �ڸ��� ����
	int  GetNumberLength( int number );
	void GetNumberSplit( int number, OUT DWORDVec& NumVec );

	bool IsAvailableDate( int iYear, int iMonth, int iDay );
	int GetProcessRunningCount( const ioHashString& szProcessName );

	bool FullPathGenerate( const TCHAR* FileName, TCHAR* szFullPathFileName, size_t iSize );

	// hex����Ÿ�� ���ڿ��� ��ȯ
	bool HexToString( std::string & outStr, const unsigned char * hexArray, size_t hexSize );

	// �޽��� �ڽ� �����ڵ� ���

	void LoadFileInResource(int name, const char * type, DWORD& size, const char*& data);
	void LoadTextInResource(int name, DWORD& size, const char*& data);
	int MessageboxShow(HWND hwnd, const char * pStr, const char * pCaption, UINT uType);
	
	enum CharSetType
	{ 
		CST_HEAD,
		CST_MID,
		CST_TAIL,
	};
	int DivideKoreanLanguage( wchar_t *str, wchar_t *buffer, UINT nSize, CharSetType eType );
	bool Strstr( const char* szSrc, const char* szKeyWord );

}

typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef uint16_t HALF;
float HALFToFloat(HALF);
HALF floatToHALF(float);
static uint32_t halfToFloatI(HALF);
static HALF floatToHalfI(uint32_t);

//��ֶ����� ���־����.
void encodeXYZ( D3DXVECTOR3& In, float& OutX, float& OutY );
void encodeXYZ( const D3DXVECTOR3& In, float& OutX, float& OutY );
void decodeXYZ( D3DXVECTOR3& Out, float& InX, float& InY );

//0~360������..
BYTE ConvertAngle2Byte( float fAngle );
float ConvertByte2Angle( BYTE byAngle );
void ConvertPackAngle2Vec( BYTE byAngle, D3DXVECTOR3& vOut );

void InputHalf2Vec( HALF* pIn, D3DXVECTOR3& vOut );
void InputHalf2Quat( HALF* pIn, D3DXQUATERNION& vOut );

void OutputQuat2Half( D3DXQUATERNION& vIn, HALF* pOut );
void OutputVec2Half( D3DXVECTOR3& vIn, HALF* pOut );

void Quat2Angle( const D3DXQUATERNION &qt, D3DXVECTOR3& vOut );
//��ֶ������ float -1 ~ +1 �� 256���� �ɰ�..
float CompactFNormalized( float In );

void ConvertForceXYZ2Vec( D3DXVECTOR3 vIn, D3DXVECTOR3& vOut );