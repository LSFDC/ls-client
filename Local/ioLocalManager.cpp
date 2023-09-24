#include "stdafx.h"
#include "iolocalkorea.h"
#include "iolocalus.h"
#include "iolocaltaiwan.h"
#include "iolocalIndonesia.h"
#include "ioLocalJapan.h"
#include ".\iolocalmanager.h"
#include "ioLocalThailand.h"
#include "ioLocalChina.h"
#include "ioLocalEU.h"
#include "ioLocalLatin.h"
#include "ioLocalSingapore.h"


#ifdef SRC_EU

#include "../NMClass/NMPCH.h"
#include "../NMClass/NMManager.h"
#include "../NMClass/NMFiles.cpp"
#include "../Local/ioLocalManager.h"

#define    WM_MESSENGER_NOTIFY	( WM_USER + 100 ) 

#define    _nmman			CNMManager::GetInstance()
#define    _nmco			CNMCOClientObject::GetInstance()


HMODULE WINAPI LoadIt()
{
	return ::LoadLibraryA( "unicows.dll" );
}
extern "C" HMODULE (__stdcall *_PfnLoadUnicows)(void) = &LoadIt;

#endif

template<> ioLocalManager* Singleton< ioLocalManager >::ms_Singleton = 0;

ioLocalManager::ioLocalManager(void)
{
#ifdef LOCAL_DBG
#ifdef SRC_ID 
	m_eLocalType = LCT_INDONESIA;
#else if SRC_TW
	m_eLocalType =  LCT_TAIWAN;
#else if SRC_SEA
 	m_eLocalType =  LCT_SINGAPORE;
#endif
#ifdef SRC_TH
	m_eLocalType = LCT_THAILAND;
#endif
#endif

	m_bNexonInit = false;
	m_vLocalVector.reserve( 10 );
}

ioLocalManager::~ioLocalManager(void)
{
	for(vLocalVector::iterator iter = m_vLocalVector.begin(); iter != m_vLocalVector.end(); ++iter)
	{
		delete *iter;
	}
	m_vLocalVector.clear();
}

ioLocalParent * ioLocalManager::CreateLocal( LocalType eLocalType )
{
	ioLocalParent *pLocal = NULL;
	if( eLocalType == LCT_KOREA )
		pLocal = new ioLocalKorea;
	else if( eLocalType == LCT_US )
		pLocal = new ioLocalUS;
	else if( eLocalType == LCT_TAIWAN )
		pLocal = new ioLocalTaiwan;
	else if( eLocalType == LCT_INDONESIA )
		pLocal = new ioLocalIndonesia;
	else if( eLocalType == LCT_SINGAPORE )
		pLocal = new ioLocalSingapore;
	else if( eLocalType == LCT_MALAYSIA )
		pLocal = new ioLocalSingapore;
	else if( eLocalType == LCT_JAPAN )
		pLocal = new ioLocalJapan;
	else if( eLocalType == LCT_THAILAND )
		pLocal = new ioLocalThailand;
	else if( eLocalType == LCT_CHINA )
		pLocal = new ioLocalChina;
	else if( eLocalType == LCT_EU )
		pLocal = new ioLocalEU;
	else if( eLocalType == LCT_GERMANY )
		pLocal = new ioLocalEU;
	else if( eLocalType == LCT_FRANCE )
		pLocal = new ioLocalEU;
	else if( eLocalType == LCT_ITALIA )
		pLocal = new ioLocalEU;
	else if( eLocalType == LCT_POLAND )
		pLocal = new ioLocalEU;
	else if( eLocalType == LCT_TURKEY )
		pLocal = new ioLocalEU;
	else if( eLocalType == LCT_LATIN )
		pLocal = new ioLocalLatin;
	// 새로운 국가 추가
	else
		LOG.PrintTimeAndLog( 0, "%s Type is wrong. %s", __FUNCTION__, GetLocalName( eLocalType ) );

	return pLocal;
}

void ioLocalManager::AddLocal( ioLocalParent *pLocal )
{
	if( !pLocal )
	{
		LOG.PrintTimeAndLog( 0, "%s pLocal == NULL." , __FUNCTION__ );
		return;
	}

	if( GetLocal( pLocal->GetType() ) )
	{
		LOG.PrintTimeAndLog( 0, "%s Type is existing.%s" , __FUNCTION__ , GetLocalName( pLocal->GetType() ) );
		return;
	}

	m_vLocalVector.push_back( pLocal );
}

void ioLocalManager::Init()
{
	AddLocal( CreateLocal( LCT_KOREA ) );
	AddLocal( CreateLocal( LCT_US ) );
	AddLocal( CreateLocal( LCT_TAIWAN ) );
	AddLocal( CreateLocal( LCT_INDONESIA ) );
	AddLocal( CreateLocal( LCT_SINGAPORE ) );
	AddLocal( CreateLocal( LCT_JAPAN ) );
	AddLocal( CreateLocal( LCT_THAILAND ) );
	AddLocal( CreateLocal( LCT_CHINA ) );
	AddLocal( CreateLocal( LCT_EU ) );
	AddLocal( CreateLocal( LCT_LATIN ) );
	// 새로운 국가 추가
}

ioLocalParent * ioLocalManager::GetLocal( LocalType eLocalType )
{
	for(vLocalVector::iterator iter = m_vLocalVector.begin(); iter != m_vLocalVector.end(); ++iter)
	{
		ioLocalParent *pLocal = *iter;
		if( !pLocal )
			continue;
		if( pLocal->GetType() + pLocal->GetSubType() == eLocalType )
			return pLocal;
	}

	return NULL;
}

ioLocalManager &ioLocalManager::GetSingleton()
{
	return Singleton< ioLocalManager >::GetSingleton();
}

const char * ioLocalManager::GetLocalName( LocalType eLocalType )
{
	if(  m_eLocalType == LCT_KOREA )
		return "KOREA";
	else if( m_eLocalType == LCT_US )
		return "US";
	else if( m_eLocalType == LCT_TAIWAN )
		return "TAIWAN";
	else if( m_eLocalType == LCT_INDONESIA )
		return "INDONESIA";
	else if( m_eLocalType == LCT_SINGAPORE )
		return "SEA";
	else if( m_eLocalType == LCT_MALAYSIA )
		return "SEA";
	else if( m_eLocalType == LCT_JAPAN )
		return "JAPAN";
	else if( m_eLocalType == LCT_THAILAND )
		return "THAILAND";
	else if( m_eLocalType == LCT_CHINA )
		return "CHINA";
	else if( m_eLocalType == LCT_EU )
		return "EU";
	else if( m_eLocalType == LCT_GERMANY )
		return "EU";
	else if( m_eLocalType == LCT_FRANCE )
		return "EU";
	else if( m_eLocalType == LCT_ITALIA )
		return "EU";
	else if( m_eLocalType == LCT_POLAND )
		return "EU";
	else if( m_eLocalType == LCT_TURKEY )
		return "EU";
	else if( m_eLocalType == LCT_LATIN )
		return "LATIN";
	else
		return "UNKNOWN";
}

void ioLocalManager::ParseCmd( const char *szCmd )
{
	// ? 구분자 : , 확장 구분자
	int iSize    = strlen( szCmd );
	int iMarkCnt = 0;
	int iExpandMarkCnt = 0;
	char szLocalType[MAX_PATH]="";
	int  iLocalTypeCnt = 0;
	int  iNexonTypeCnt = 0;
	enum { MAX_MARK = 0, MAX_NEXON_MARK=4, MAX_EXPAND_MARK = 9 };

	for (int i = 0; i < iSize ; i++)
	{
		if( szCmd[i] == '?' )
		{
			iMarkCnt++;
			continue;
		}

		if( iMarkCnt == MAX_MARK )
		{
			if( szCmd[i] == ',' )
			{
				iExpandMarkCnt++;
				continue;
			}

			if( iExpandMarkCnt == MAX_EXPAND_MARK )
			{
				szLocalType[iLocalTypeCnt] = szCmd[i];
				iLocalTypeCnt++;

				if( iLocalTypeCnt >= MAX_PATH )
					break;
			}
		}
		else if( iMarkCnt > MAX_MARK )
		{
			break;
		}
	}
	
	int iLocalType = atoi( szLocalType );

	if( iLocalType != LCT_KOREA &&
		iLocalType != LCT_US    &&
		iLocalType != LCT_TAIWAN&&
		iLocalType != LCT_JAPAN &&
		iLocalType != LCT_SINGAPORE &&
		iLocalType != LCT_MALAYSIA &&
		iLocalType != LCT_INDONESIA &&
		iLocalType != LCT_VIETNAM  &&
		iLocalType != LCT_THAILAND &&
		iLocalType != LCT_EU    &&
		iLocalType != LCT_GERMANY    &&
		iLocalType != LCT_FRANCE    &&
		iLocalType != LCT_ITALIA    &&
		iLocalType != LCT_POLAND    &&
		iLocalType != LCT_TURKEY    &&
		iLocalType != LCT_LATIN   &&
		iLocalType != LCT_CHINA )
	{
		return;
	}

	m_eLocalType = (LocalType) iLocalType;

	if( m_eLocalType == LCT_GERMANY)
	{
		ioLocalParent *pLocal = g_LocalMgr.GetLocal(LCT_EU);
		if(pLocal)
			((ioLocalEU*)pLocal)->SetSubType(ioLocalEU::SUB_TYPE_DE);
	}
	else if( m_eLocalType == LCT_FRANCE)
	{
		ioLocalParent *pLocal = g_LocalMgr.GetLocal(LCT_EU);
		if(pLocal)
			((ioLocalEU*)pLocal)->SetSubType(ioLocalEU::SUB_TYPE_FR);
	}
	else if( m_eLocalType == LCT_ITALIA)
	{
		ioLocalParent *pLocal = g_LocalMgr.GetLocal(LCT_EU);
		if(pLocal)
			((ioLocalEU*)pLocal)->SetSubType(ioLocalEU::SUB_TYPE_IT);
	}
	else if( m_eLocalType == LCT_POLAND)
	{
		ioLocalParent *pLocal = g_LocalMgr.GetLocal(LCT_EU);
		if(pLocal)
			((ioLocalEU*)pLocal)->SetSubType(ioLocalEU::SUB_TYPE_PL);
	}
	else if( m_eLocalType == LCT_TURKEY)
	{
		ioLocalParent *pLocal = g_LocalMgr.GetLocal(LCT_EU);
		if(pLocal)
			((ioLocalEU*)pLocal)->SetSubType(ioLocalEU::SUB_TYPE_TR);
	}
	else if( m_eLocalType == LCT_MALAYSIA)   // singapore, malaysia
	{
		ioLocalParent *pLocal = g_LocalMgr.GetLocal(LCT_SINGAPORE);
		if(pLocal)
			((ioLocalSingapore*)pLocal)->SetSubType(ioLocalSingapore::SUB_TYPE_MY);
	}
}

#ifdef SRC_EU
int ioLocalManager::NexonCertification(HWND hWnd)
{
	if( !m_bNexonInit )
	{
		LoadIt();
		_nmman.Init();
		_nmco.SetPatchOption( FALSE );

		bool bResult = _nmco.SetLocaleAndRegion( kLocaleID_EU, kRegionCode_EU );
		if(!bResult)
			return false;

		if ( !_nmco.Initialize( kGameCode_lostsaga_eu ) ) // test 0
			return false;

		if ( !_nmco.RegisterCallbackMessage(hWnd , WM_MESSENGER_NOTIFY ) )
			return false;

		m_bNexonInit = true;
	}
	
	int iLoginResult = CONNECT_EXCEPT;

	NMLoginAuthReplyCode resultAuth = _nmco.AttachAuth( m_szNexonPassport );

	switch (resultAuth)
	{
	case kLoginAuth_OK:
		{
			TCHAR szNexonPassport[ NXPASSPORT_SIZE ] = {0,};
			_nmco.GetNexonPassport( szNexonPassport );
			iLoginResult = CONNECT_OK;
		}
		break;

	case kLoginAuth_ServerFailed:
		iLoginResult = CONNECT_EXITING_SERVER;
		break;

	case kLoginAuth_ServiceShutdown:
		iLoginResult = CONNECT_EXITING_SERVER;
		break;

	case kLoginAuth_SessionDataNotExist:
		iLoginResult = CONNECT_EXCEPT;
		break;

	case kLoginAuth_InvalidUserIP:
		iLoginResult = CONNECT_ID_NOT;
		break;

	case kLoginAuth_InvalidPassportKey:
		iLoginResult = CONNECT_PW_NOT;
		break;

	case kLoginAuth_Disconnected:
		iLoginResult = CONNECT_EXCEPT;
		break;

	default:
		break;
	}

	return iLoginResult;
}

void ioLocalManager::NexonLogout(HWND hWnd)
{
	bool bOk = false;
	if(! _nmco.DetachAuth() )
	{
		LOG.PrintTimeAndLog(0, "Nexon Detach Fail");
	}
	if(!_nmco.ResetCallbackMessage( hWnd ))
	{
		LOG.PrintTimeAndLog(0, "Nexon ResetCallBack Fail");
	}
	// 이벤트 핸들러 등록 제거
	_nmman.Finalize();			// 메신저 매니저 종료
	LOG.PrintTimeAndLog(0, "Nexon Filnalize End");
}
#endif