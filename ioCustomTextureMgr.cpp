#include "stdafx.h"

#include "ioCustomTextureMgr.h"

#include "ioPlayStage.h"
#include "ioBaseChar.h"
#include "ioCreateChar.h"

//D3DXSaveTextureToFile
class ioCustomTextureTask : public IThreadTask
{
protected:
	ioHashString m_FullName;
	ioHashString m_FullURL;

protected:
	TexDownLoadData m_DownData;

protected:
	bool m_bDone;

public:
	virtual void Process();
	virtual void Done();

public:
	virtual int  GetPriority() const { return 3; }

public:
	ioCustomTextureTask( const char *szFullName, const char *szFullURL, TexDownLoadData &kData );
	virtual ~ioCustomTextureTask();
};

ioCustomTextureTask::ioCustomTextureTask( const char *szFullName, const char *szFullURL, TexDownLoadData &kData ) : m_FullName( szFullName ), m_FullURL( szFullURL )
{
	m_DownData = kData;
	m_bDone    = false;

	if( g_App.IsMeDeveloper() )
	{
		LOG.PrintTimeAndLog( 0, "ioCustomTextureTask URL : %s", szFullURL );
		LOG.PrintTimeAndLog( 0, "ioCustomTextureTask Path : %s", szFullName );
	}
}

ioCustomTextureTask::~ioCustomTextureTask()
{
}

void ioCustomTextureTask::Process()
{
	HRESULT hr = ::URLDownloadToFile( 0, m_FullURL.c_str(), m_FullName.c_str(),	0, NULL );

	if( FAILED(hr) )
		m_bDone = false;
	else
		m_bDone = true;
}

void ioCustomTextureTask::Done()
{
	g_CustomTextureMgr.FileDownLoadingDone( m_DownData );
}

template<> ioCustomTextureMgr* Singleton< ioCustomTextureMgr >::ms_Singleton = 0;
ioCustomTextureMgr::ioCustomTextureMgr()
{
	m_pPlayStage    = NULL;
	m_bCostumeDown	= FALSE;
}

ioCustomTextureMgr::~ioCustomTextureMgr()
{	
	m_DownDataList.clear();
}

ioCustomTextureMgr& ioCustomTextureMgr::GetSingleton()
{
	return Singleton< ioCustomTextureMgr >::GetSingleton();
}

void ioCustomTextureMgr::Initialize()
{
	ioINILoader_e kLoader( "config/sp2_custom_texture.ini" );

	char szBuf[MAX_PATH];
	{
		kLoader.SetTitle_e( "common" );

		kLoader.LoadString_e( "storage_url", "", szBuf, MAX_PATH );
		m_szStorageURL = szBuf;

		kLoader.LoadString_e( "storage_costum_url", "", szBuf , MAX_PATH );
		m_szStorageCostumURL = szBuf;

		kLoader.SetTitle_e( "extra_item" );
		int iMaxItem = kLoader.LoadInt_e( "item_cnt", 0 );
		for(int i = 0;i < iMaxItem;i++)
		{
			char szBuf[MAX_PATH] = "";
			char szKey[MAX_PATH] = "";
			TrainExtraItem kData;

			sprintf_e( szKey, "item%d_code", i + 1 );
			kData.m_dwItemCode = kLoader.LoadInt( szKey, 0 );

			sprintf_e( szKey, "item%d_name", i + 1 );
			kLoader.LoadString( szKey, "", szBuf, MAX_PATH );
			kData.m_szItemName = szBuf;

			sprintf_e( szKey, "item%d_icon", i + 1 );
			kLoader.LoadString( szKey, "", szBuf, MAX_PATH );
			kData.m_szItemIcon = szBuf;

			m_TrainExtraItemList.push_back( kData );
		}
	}

	//��� ����
	char szAppDir[MAX_PATH] = "";
	::GetCurrentDirectory( MAX_PATH, szAppDir );

	sprintf( szBuf, "%s/resource/", szAppDir );
	::CreateDirectory( szBuf, NULL );

	sprintf( szBuf, "%s/resource/texture/", szAppDir );
	::CreateDirectory( szBuf, NULL );
	m_szAppDirectory = szBuf;
}

bool ioCustomTextureMgr::IsFileCheck( const ioHashString &rkFileName )
{
	FILE *pFile = fopen( rkFileName.c_str(), "r" );
	if( pFile ) 
	{
		fclose( pFile );		
		return true;
	}
	return false;
}

bool ioCustomTextureMgr::IsExistenceFile( const ioHashString &rkOwnerName, const ioHashString &rkDstTexName, DWORD dwCustomIndex, DWORD dwSubIndex, BOOL bCostume )
{
	if( rkOwnerName.IsEmpty() ) return false;
	if( rkDstTexName.IsEmpty() ) return false;
	if( dwCustomIndex == 0 || dwSubIndex == 0 ) return false;

	// ������ �����ϴ��� Ȯ��
	char szTextureDir[MAX_PATH] = "";
	char szStorageURL[MAX_PATH] = "";

#ifdef CUSTOM_COSTUME
	if( bCostume ) //�ڽ�Ƭ ����� ���, �ٿ�ε� ��ΰ� �ٸ���!
	{
		sprintf( szTextureDir, "%s%d_%d_c.lsc", m_szAppDirectory.c_str(), dwCustomIndex, dwSubIndex );
		if( IsFileCheck( szTextureDir ) ) 
			return true;

		sprintf( szStorageURL, "%s/%d_%d_c.lsc", m_szStorageCostumURL.c_str(), dwCustomIndex, dwSubIndex );
	}
	else //�Ϲ� ��� �ϰ��
	{
		sprintf( szTextureDir, "%s%d_%d.lsc", m_szAppDirectory.c_str(), dwCustomIndex, dwSubIndex );
		if( IsFileCheck( szTextureDir ) ) 
			return true;

		sprintf( szStorageURL, "%s/%d_%d.lsc", m_szStorageURL.c_str(), dwCustomIndex, dwSubIndex );
	}
#else
	sprintf( szTextureDir, "%s%d_%d.lsc", m_szAppDirectory.c_str(), dwCustomIndex, dwSubIndex );
	if( IsFileCheck( szTextureDir ) ) 
		return true;

	sprintf( szStorageURL, "%s/%d_%d.lsc", m_szStorageURL.c_str(), dwCustomIndex, dwSubIndex );
#endif
		
	TexDownLoadData kDownLoad = { rkOwnerName, rkDstTexName, dwCustomIndex, dwSubIndex };
	if( IsDownLoading( kDownLoad ) ) 
		return false;

	ioCustomTextureTask *pTask = new ioCustomTextureTask( szTextureDir, szStorageURL, kDownLoad );
	if( pTask )
	{
		m_DownDataList.push_back( kDownLoad );
		g_ThreadTaskMgr.AddTask( pTask ); //park. lsc file is not have -> download to filewrite svr
		m_bCostumeDown = TRUE;
		if( g_App.IsMeDeveloper() )
		{
			LOG.PrintTimeAndLog( 0, "%s - Texture download file : %s", __FUNCTION__, szStorageURL );
		}
	}
	return false;
}

bool ioCustomTextureMgr::IsDownLoading( TexDownLoadData &rkData )
{
	TexDownLoadDataList::iterator iter = std::find( m_DownDataList.begin(), m_DownDataList.end(), rkData );

	if( iter != m_DownDataList.end() )
		return true;

	return false;
}

void ioCustomTextureMgr::FileDownLoadingDone( TexDownLoadData &rkData )
{
	m_DownDataList.remove( rkData );

//	LOG.PrintTimeAndLog( 0, "FileDownLoadingDone : %s - %s - %d - %d", rkData.m_szOwnerName.c_str(), rkData.m_szDstTexName.c_str(), 
//							rkData.m_dwCustomIndex, rkData.m_dwSubIndex );
	// ���� �ٿ�ε尡 �Ϸ�Ǹ� ���⼭ ó���Ѵ�.
	
	// UI ĳ���� ó��.
	if( rkData.m_szOwnerName == g_MyInfo.GetPublicID() )
	{
		for(int i = 0;i < (int)m_dwUICharPtrList.size();i++)
		{
			ioCreateChar *pUIChar = (ioCreateChar *)m_dwUICharPtrList[i];
			if( pUIChar )
			{
				pUIChar->CustomTextureDownLoadComplete( rkData.m_szDstTexName, rkData.m_dwCustomIndex, rkData.m_dwSubIndex, m_bCostumeDown );
			}
		}
	}
	
	// �ʵ� ĳ���� ó��
	if( m_pPlayStage )
	{
		ioBaseChar *pBaseChar = m_pPlayStage->GetBaseChar( rkData.m_szOwnerName );
		if( pBaseChar )
		{
			pBaseChar->CustomTextureDownLoadComplete( rkData.m_szDstTexName, rkData.m_dwCustomIndex, rkData.m_dwSubIndex, m_bCostumeDown );
		}
	}
}

void ioCustomTextureMgr::CreateNewUIChar( DWORD dwCharPtr )
{
	m_dwUICharPtrList.push_back( dwCharPtr );
}

void ioCustomTextureMgr::DestroyUIChar( DWORD dwCharPtr )
{
	for(int i = 0;i < (int)m_dwUICharPtrList.size();i++)
	{
		if( m_dwUICharPtrList[i] == dwCharPtr )
		{
			m_dwUICharPtrList.erase( m_dwUICharPtrList.begin() + i );
			break;
		}
	}
}

bool ioCustomTextureMgr::IsUICharPtrCheck( DWORD dwCharPtr )
{
	for(int i = 0;i < (int)m_dwUICharPtrList.size();i++)
	{
		if( m_dwUICharPtrList[i] == dwCharPtr )
		{
			return true;
		}
	}
	return false;
}

void ioCustomTextureMgr::CompleteCustomItem( DWORD dwItemCode, DWORD dwMaleCustom, DWORD dwFemaleCustom )
{
	if( dwItemCode == 0 ) return;

	for(int i = 0;i < (int)m_dwUICharPtrList.size();i++)
	{
		ioCreateChar *pUICharacter = (ioCreateChar *)m_dwUICharPtrList[i];
		if( !pUICharacter ) continue;

		pUICharacter->ReloadEquipItem( dwItemCode, dwMaleCustom, dwFemaleCustom );
	}
}

DWORD ioCustomTextureMgr::GetTrainExtraItem( int iArray )
{
	if( COMPARE( iArray, 0, (int)m_TrainExtraItemList.size() ) )
		return m_TrainExtraItemList[iArray].m_dwItemCode;
	return 0;
}

const ioHashString &ioCustomTextureMgr::GetTrainExtraItemName( DWORD dwItemCode )
{
	static ioHashString szReturn = "None";
	for(int i = 0;i < (int)m_TrainExtraItemList.size();i++)
	{
		if( m_TrainExtraItemList[i].m_dwItemCode == dwItemCode )
			return m_TrainExtraItemList[i].m_szItemName;
	}
	return szReturn;
}

const ioHashString &ioCustomTextureMgr::GetTrainExtraItemIcon( DWORD dwItemCode )
{
	static ioHashString szReturn = "None";
	for(int i = 0;i < (int)m_TrainExtraItemList.size();i++)
	{
		if( m_TrainExtraItemList[i].m_dwItemCode == dwItemCode )
			return m_TrainExtraItemList[i].m_szItemIcon;
	}
	return szReturn;
}
