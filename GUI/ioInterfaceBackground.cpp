#include "StdAfx.h"

#include "../io3DEngine/ioINILoader.h"
#include "../io3DEngine/ioUIRenderer.h"
#include "../io3DEngine/ioMath.h"
#include "../io3DEngine/ioUIImageSetManager.h"
#include "../io3DEngine/ioUIRenderImage.h"
#include "../io3dengine/safesprintf.h"

#include "../Setting.h"
#include "../ioLoadingRender.h"
#include "../ioMyInfo.h"

#include "ioSP2GUIManager.h"
#include "FileListWnd.h"
#include "ioInterfaceBackground.h"

#include <strsafe.h>

template<> ioInterfaceBackground* Singleton< ioInterfaceBackground >::ms_Singleton = 0;

ioInterfaceBackground::ioInterfaceBackground()
{
	m_eBackType     = BGT_NONE;
	m_ePrevBackType = BGT_NONE;
	m_pLoadingRes   = NULL;
	for(int i = 0;i < MAX_CUSTOM;i++)
	{
		m_CustomLobbyBack[i].m_pCustomRes = NULL;
	}
	m_CustomLoadingBack.m_pCustomRes	= NULL;
	m_CustomBattleBack.m_pCustomRes		= NULL;
	m_CustomLadderBack.m_pCustomRes		= NULL;
	
	for(int i = 0;i < MAX_CUSTOM;i++)
	{
		m_CustomFindModeBack[i].m_pCustomRes = NULL;
	}


	m_bFadeInStart  = false;
	m_dwCurrentTime = 0;
	m_dwFadeInTime  = 0;
	m_iAlphaRate    = MAX_ALPHA_RATE;
	m_bFirstLoading = false;
	m_iModeType     = -1;
	m_iMyRoomType   = 0;
}

ioInterfaceBackground::~ioInterfaceBackground()
{
	ClearBackResource();
}

ioInterfaceBackground& ioInterfaceBackground::GetSingleton()
{
	return Singleton< ioInterfaceBackground >::GetSingleton();
}

void ioInterfaceBackground::ClearBackResource()
{
	BackGroundMap::iterator iCreate;
	for( iCreate = m_BackGroundMap.begin() ; iCreate != m_BackGroundMap.end() ; ++iCreate )
	{
		SAFEDELETE( iCreate->second );
	}
	m_BackGroundMap.clear();

	SAFEDELETE( m_pLoadingRes );
	for(int i = 0;i < MAX_CUSTOM;i++)
	{
		SAFEDELETE( m_CustomLobbyBack[i].m_pCustomRes );
	}
	SAFEDELETE( m_CustomLoadingBack.m_pCustomRes );
	SAFEDELETE( m_CustomBattleBack.m_pCustomRes );
	SAFEDELETE( m_CustomLadderBack.m_pCustomRes );

	for( int i = 0; i < MAX_CUSTOM; i++ )
	{
		SAFEDELETE( m_CustomFindModeBack[i].m_pCustomRes );
	}
}

void ioInterfaceBackground::LoadBackgroundINI()
{
	ClearBackResource();	
	
	{
		ioINILoader_e kLoader( "config/sp2_background.ini" );
		kLoader.SetTitle_e( "Common" );

		char szTitle[MAX_PATH], szBuf[MAX_PATH];
		m_dwFadeInTime = kLoader.LoadInt_e( "FadeInTime", 0 );
		int iMaxBack = kLoader.LoadInt_e( "MaxBackground", 0 );
		for(int i = 0;i < iMaxBack;i++)
		{
			sprintf_e( szTitle, "Back%d", i + 1 );
			kLoader.SetTitle( szTitle );
						
			int iBackType = kLoader.LoadInt_e( "Type", 0 );

			//iBackType�� �������� �ʾ��� ��� ��������
			if( iBackType == 0 )
				iBackType = i;

			kLoader.LoadString_e( "Back", "", szBuf, sizeof( szBuf ) );
			m_BackGroundMap.insert( BackGroundMap::value_type( (BackGroundType)iBackType, g_UIImageSetMgr.CreateImageByFullName( szBuf, true ) ) );
		}	
		
		if( !m_szLoadingRes.IsEmpty() )
			m_pLoadingRes = g_UIImageSetMgr.CreateImage( m_szLoadingRes.c_str(), "Back001", true );


		{   // �κ� Ŀ���� ����
			m_CustomLobbyBack[CUSTOM_RENDER].m_SetName	= "CustomLobbyBack1"; 
			m_CustomLobbyBack[CUSTOM_RENDER].m_bOnRender= false;

			m_CustomLobbyBack[CUSTOM_FADEIN].m_SetName	= "CustomLobbyBack2";
			m_CustomLobbyBack[CUSTOM_FADEIN].m_bOnRender= false;
		}

		{
			// �ε� Ŀ���� ����
			m_CustomLoadingBack.m_SetName   = "CustomLoadingBack";
			m_CustomLoadingBack.m_bOnRender = false;
		}

		{
			// �������� Ŀ���� ����
			m_CustomBattleBack.m_SetName   = "CustomBattleBack";
			m_CustomBattleBack.m_bOnRender = false;
		}

		{
			// ����&�������� Ŀ���� ����
			m_CustomLadderBack.m_SetName   = "CustomLadderBack";
			m_CustomLadderBack.m_bOnRender = false;
		}
		
		//����/�κ� ���ս� ���
		m_CustomFindModeBack[CUSTOM_RENDER].m_SetName	= "CustomFindModeBack1"; 
		m_CustomFindModeBack[CUSTOM_RENDER].m_bOnRender= false;

		m_CustomFindModeBack[CUSTOM_FADEIN].m_SetName	= "CustomFindModeBack2";
		m_CustomFindModeBack[CUSTOM_FADEIN].m_bOnRender= false;		
	}
}


void ioInterfaceBackground::LoadCustomLoadingBack()
{
	char fName[MAX_PATH]="";
	sprintf( fName, "Save\\%s\\%s.jpg", g_MyInfo.GetPublicID().c_str(), m_CustomLoadingBack.m_SetName.c_str() );
	FILE *fp = fopen( fName, "r" );
	if( fp )
	{
		fclose( fp );	

		m_CustomLoadingBack.m_bOnRender = false;
		if( m_CustomLoadingBack.m_pCustomRes )
		{
			// ���� ó��
			SAFEDELETE( m_CustomLoadingBack.m_pCustomRes );
			g_UIImageSetMgr.DestroyImageSet( m_CustomLoadingBack.m_SetName );
		}

		ioUIRenderImage *pBackImg = g_UIImageSetMgr.CreateImageByJPG( m_CustomLoadingBack.m_SetName, fName, true );
		if( pBackImg )
		{		
			m_CustomLoadingBack.m_pCustomRes = pBackImg;
			m_CustomLoadingBack.m_bOnRender  = true;
		}
	}
}

void ioInterfaceBackground::ClearLoadingBackground()
{	
	m_CustomLoadingBack.m_bOnRender = false;

	char fName[MAX_PATH]="";
	StringCbPrintf_e( fName, sizeof(fName), "Save\\%s\\%s.jpg", g_MyInfo.GetPublicID().c_str(), m_CustomLoadingBack.m_SetName.c_str() );
	::DeleteFile( fName );
}

bool ioInterfaceBackground::CopyCustomLoadingBack( char *szFullName )
{
	if( szFullName == NULL ) return false;

	char fName[MAX_PATH] = "";
	StringCbPrintf_e( fName, sizeof( fName ), "Save\\%s" , g_MyInfo.GetPublicID().c_str() );
	CreateDirectory( "Save", NULL );
	CreateDirectory( fName, NULL );

	char szDstFile[MAX_PATH] = "";
	sprintf( szDstFile, "%s\\%s.jpg", fName, m_CustomLoadingBack.m_SetName.c_str() );
	if( ::CopyFile( szFullName, szDstFile, FALSE ) )
		return true;
	return false;
}

bool ioInterfaceBackground::IsCustomLoadingBack()
{
	return m_CustomLoadingBack.m_bOnRender;
}

//--�κ� �ε�-------------------------------------------------------------------------------------------------------------------------------------------

void ioInterfaceBackground::LoadCustomLobbyBack( bool bFadeIn /* = false  */ )
{
	CustomBack kTempCustom = m_CustomLobbyBack[CUSTOM_RENDER];
	m_CustomLobbyBack[CUSTOM_RENDER] = m_CustomLobbyBack[CUSTOM_FADEIN];
	m_CustomLobbyBack[CUSTOM_FADEIN] = kTempCustom;

	char fName[MAX_PATH]="";
	sprintf( fName, "Save\\%s\\%s.jpg", g_MyInfo.GetPublicID().c_str(), m_CustomLobbyBack[CUSTOM_RENDER].m_SetName.c_str() );
	FILE *fp = fopen( fName, "r" );
	if( fp )
	{
		fclose( fp );	

		m_CustomLobbyBack[CUSTOM_FADEIN].m_bOnRender = false;
		if( m_CustomLobbyBack[CUSTOM_RENDER].m_pCustomRes )
		{
			// ���� ó��
			SAFEDELETE( m_CustomLobbyBack[CUSTOM_RENDER].m_pCustomRes );
			g_UIImageSetMgr.DestroyImageSet( m_CustomLobbyBack[CUSTOM_RENDER].m_SetName );
		}

		ioUIRenderImage *pBackImg = g_UIImageSetMgr.CreateImageByJPG( m_CustomLobbyBack[CUSTOM_RENDER].m_SetName, fName, true );
		if( pBackImg )
		{		
			m_CustomLobbyBack[CUSTOM_RENDER].m_pCustomRes = pBackImg;
			m_CustomLobbyBack[CUSTOM_RENDER].m_bOnRender  = true;
		}

		m_bCustomFadeIn = bFadeIn;
		if( bFadeIn )
		{
			SetBackground( BGT_LOBBY );
		}
	}
}

void ioInterfaceBackground::ClearLobbyBackground()
{	
	char fName[MAX_PATH]="";
	StringCbPrintf_e( fName, sizeof(fName), "Save\\%s\\%s.jpg", g_MyInfo.GetPublicID().c_str(), m_CustomLobbyBack[CUSTOM_RENDER].m_SetName.c_str() );
	::DeleteFile( fName );
	m_CustomLobbyBack[CUSTOM_RENDER].m_bOnRender = false;

	StringCbPrintf_e( fName, sizeof(fName), "Save\\%s\\%s.jpg", g_MyInfo.GetPublicID().c_str(), m_CustomLobbyBack[CUSTOM_FADEIN].m_SetName.c_str() );
	::DeleteFile( fName );
	m_CustomLobbyBack[CUSTOM_FADEIN].m_bOnRender = false;

	SetBackground( BGT_LOBBY );
	m_bCustomFadeIn = true;
}

bool ioInterfaceBackground::CopyCustomLobbyBack( char *szFullName )
{
	if( szFullName == NULL ) return false;

	char fName[MAX_PATH] = "";
	StringCbPrintf_e( fName, sizeof( fName ), "Save\\%s" , g_MyInfo.GetPublicID().c_str() );
	CreateDirectory( "Save", NULL );
	CreateDirectory( fName, NULL );

	char szDstFile[MAX_PATH] = "";
	sprintf( szDstFile, "%s\\%s.jpg", fName, m_CustomLobbyBack[CUSTOM_FADEIN].m_SetName.c_str() );
	if( ::CopyFile( szFullName, szDstFile, FALSE ) )
	{
		// JPG�� �ΰ� ����ϹǷ� ���� JPG�� ���粨�� �����Ѵ�.
		char szDstFile[MAX_PATH] = "";
		sprintf( szDstFile, "%s\\%s.jpg", fName, m_CustomLobbyBack[CUSTOM_RENDER].m_SetName.c_str() );
		if( ::CopyFile( szFullName, szDstFile, FALSE ) )
			return true;
	}
	return false;
}

bool ioInterfaceBackground::IsCustomLobbyBack()
{
	return m_CustomLobbyBack[CUSTOM_RENDER].m_bOnRender;
}

void ioInterfaceBackground::CustomLobbyRenderFadeIn( int iAlphaRate )
{
	if( m_eBackType != BGT_LOBBY ) return;
	if( iAlphaRate == MAX_ALPHA_RATE ) return;
	if( !m_bCustomFadeIn ) return;

    ioUIRenderImage *pAlphaBack = NULL;
	if( !m_CustomLobbyBack[CUSTOM_RENDER].m_bOnRender )
	{
		//  �⺻ �̹����� ����Ǿ��� ��� �������ϴ� Ŀ���� �̹����� �ٴڿ� ��� ���̵� �� ����
		if( m_CustomLobbyBack[CUSTOM_RENDER].m_pCustomRes )
			pAlphaBack = m_CustomLobbyBack[CUSTOM_RENDER].m_pCustomRes;
	}
	else if( m_CustomLobbyBack[CUSTOM_FADEIN].m_pCustomRes )
	{
		// Ŀ���ҿ��� Ŀ�������� ����Ǿ����� ���� Ŀ���� �̹����� �ٴڿ� ��� ���̵� �� ����
		pAlphaBack = m_CustomLobbyBack[CUSTOM_FADEIN].m_pCustomRes;
	}

	if( !pAlphaBack && m_ePrevBackType != BGT_NONE )
	{  
		// �⺻���� Ŀ�������� ����Ǿ����� ���� �⺻ �̹����� �ٴڿ� ��� ���̵� �� ����
		BackGroundMap::iterator iter = m_BackGroundMap.find( BGT_LOBBY );
		if( iter != m_BackGroundMap.end() )
			pAlphaBack = iter->second;
	}

	if( pAlphaBack )
	{
		pAlphaBack->SetHorzFormatting( ioUIRenderImage::HF_STRETCH );
		pAlphaBack->SetVertFormatting( ioUIRenderImage::VF_STRETCH );
		pAlphaBack->SetSize( Setting::Width(), Setting::Height() );
		pAlphaBack->SetAlpha( MAX_ALPHA_RATE );
		pAlphaBack->Render( 0, 0, UI_RENDER_NORMAL, TFO_BILINEAR );
	}
}

//--�������� �ε�-------------------------------------------------------------------------------------------------------------------------------------------

void ioInterfaceBackground::LoadCustomBattleBack()
{

	char fName[MAX_PATH]="";
	sprintf( fName, "Save\\%s\\%s.jpg", g_MyInfo.GetPublicID().c_str(), m_CustomBattleBack.m_SetName.c_str() );
	FILE *fp = fopen( fName, "r" );
	if( fp )
	{
		fclose( fp );	

		m_CustomBattleBack.m_bOnRender = false;
		if( m_CustomBattleBack.m_pCustomRes )
		{
			// ���� ó��
			SAFEDELETE( m_CustomBattleBack.m_pCustomRes );
			g_UIImageSetMgr.DestroyImageSet( m_CustomBattleBack.m_SetName );
		}

		ioUIRenderImage *pBackImg = g_UIImageSetMgr.CreateImageByJPG( m_CustomBattleBack.m_SetName, fName, true );
		if( pBackImg )
		{		
			m_CustomBattleBack.m_pCustomRes = pBackImg;
			m_CustomBattleBack.m_bOnRender  = true;
		}
	}
}

void ioInterfaceBackground::ClearBattleBackground()
{	
	m_CustomBattleBack.m_bOnRender = false;

	char fName[MAX_PATH]="";
	StringCbPrintf_e( fName, sizeof(fName), "Save\\%s\\%s.jpg", g_MyInfo.GetPublicID().c_str(), m_CustomBattleBack.m_SetName.c_str() );
	::DeleteFile( fName );
}

bool ioInterfaceBackground::CopyCustomBattleBack( char *szFullName )
{
	if( szFullName == NULL ) return false;

	char fName[MAX_PATH] = "";
	StringCbPrintf_e( fName, sizeof( fName ), "Save\\%s" , g_MyInfo.GetPublicID().c_str() );
	CreateDirectory( "Save", NULL );
	CreateDirectory( fName, NULL );

	char szDstFile[MAX_PATH] = "";
	sprintf( szDstFile, "%s\\%s.jpg", fName, m_CustomBattleBack.m_SetName.c_str() );
	if( ::CopyFile( szFullName, szDstFile, FALSE ) )
		return true;

	return false;
}

bool ioInterfaceBackground::IsCustomBattleBack()
{
	return m_CustomBattleBack.m_bOnRender;
}

//--�������� �ε�-------------------------------------------------------------------------------------------------------------------------------------------

void ioInterfaceBackground::LoadCustomLadderBack()
{

	char fName[MAX_PATH]="";
	sprintf( fName, "Save\\%s\\%s.jpg", g_MyInfo.GetPublicID().c_str(), m_CustomLadderBack.m_SetName.c_str() );
	FILE *fp = fopen( fName, "r" );
	if( fp )
	{
		fclose( fp );	

		m_CustomLadderBack.m_bOnRender = false;
		if( m_CustomLadderBack.m_pCustomRes )
		{
			// ���� ó��
			SAFEDELETE( m_CustomLadderBack.m_pCustomRes );
			g_UIImageSetMgr.DestroyImageSet( m_CustomLadderBack.m_SetName );
		}

		ioUIRenderImage *pBackImg = g_UIImageSetMgr.CreateImageByJPG( m_CustomLadderBack.m_SetName, fName, true );
		if( pBackImg )
		{		
			m_CustomLadderBack.m_pCustomRes = pBackImg;
			m_CustomLadderBack.m_bOnRender  = true;
		}
	}
}

void ioInterfaceBackground::ClearLadderBackground()
{	
	m_CustomLadderBack.m_bOnRender = false;

	char fName[MAX_PATH]="";
	StringCbPrintf_e( fName, sizeof(fName), "Save\\%s\\%s.jpg", g_MyInfo.GetPublicID().c_str(), m_CustomLadderBack.m_SetName.c_str() );
	::DeleteFile( fName );
}

bool ioInterfaceBackground::CopyCustomLadderBack( char *szFullName )
{
	if( szFullName == NULL ) return false;

	char fName[MAX_PATH] = "";
	StringCbPrintf_e( fName, sizeof( fName ), "Save\\%s" , g_MyInfo.GetPublicID().c_str() );
	CreateDirectory( "Save", NULL );
	CreateDirectory( fName, NULL );

	char szDstFile[MAX_PATH] = "";
	sprintf( szDstFile, "%s\\%s.jpg", fName, m_CustomLadderBack.m_SetName.c_str() );
	if( ::CopyFile( szFullName, szDstFile, FALSE ) )
		return true;
	return false;
}

bool ioInterfaceBackground::IsCustomLadderBack()
{
	return m_CustomLadderBack.m_bOnRender;
}

// ���� ����/�� ��Ż�� ���� �� ��� �˻�(����/�Ʒü� ��) -------------------------------------------------------------------------------------------------------------------------------------------

void ioInterfaceBackground::LoadCustomFindModeBack( bool bFadeIn /* = false  */ )
{
	CustomBack kTempCustom = m_CustomFindModeBack[CUSTOM_RENDER];
	m_CustomFindModeBack[CUSTOM_RENDER] = m_CustomFindModeBack[CUSTOM_FADEIN];
	m_CustomFindModeBack[CUSTOM_FADEIN] = kTempCustom;

	char fName[MAX_PATH]="";
	sprintf( fName, "Save\\%s\\%s.jpg", g_MyInfo.GetPublicID().c_str(), m_CustomFindModeBack[CUSTOM_RENDER].m_SetName.c_str() );
	FILE *fp = fopen( fName, "r" );
	if( fp )
	{
		fclose( fp );	

		m_CustomFindModeBack[CUSTOM_FADEIN].m_bOnRender = false;
		if( m_CustomFindModeBack[CUSTOM_RENDER].m_pCustomRes )
		{
			// ���� ó��
			SAFEDELETE( m_CustomFindModeBack[CUSTOM_RENDER].m_pCustomRes );
			g_UIImageSetMgr.DestroyImageSet( m_CustomFindModeBack[CUSTOM_RENDER].m_SetName );
		}

		ioUIRenderImage *pBackImg = g_UIImageSetMgr.CreateImageByJPG( m_CustomFindModeBack[CUSTOM_RENDER].m_SetName, fName, true );
		if( pBackImg )
		{		
			m_CustomFindModeBack[CUSTOM_RENDER].m_pCustomRes = pBackImg;
			m_CustomFindModeBack[CUSTOM_RENDER].m_bOnRender  = true;
		}

		m_bCustomFadeIn = bFadeIn;
		if( bFadeIn )
		{
			SetBackground( BGT_MODE );
		}
	}
}

void ioInterfaceBackground::ClearFindModeBackground()
{	
	char fName[MAX_PATH]="";
	StringCbPrintf_e( fName, sizeof(fName), "Save\\%s\\%s.jpg", g_MyInfo.GetPublicID().c_str(), m_CustomFindModeBack[CUSTOM_RENDER].m_SetName.c_str() );
	::DeleteFile( fName );
	m_CustomFindModeBack[CUSTOM_RENDER].m_bOnRender = false;

	StringCbPrintf_e( fName, sizeof(fName), "Save\\%s\\%s.jpg", g_MyInfo.GetPublicID().c_str(), m_CustomFindModeBack[CUSTOM_FADEIN].m_SetName.c_str() );
	::DeleteFile( fName );
	m_CustomFindModeBack[CUSTOM_FADEIN].m_bOnRender = false;

	SetBackground( BGT_MODE );
	m_bCustomFadeIn = true;
}

bool ioInterfaceBackground::CopyCustomFindModeBack( char *szFullName )
{
	if( szFullName == NULL ) return false;

	char fName[MAX_PATH] = "";
	StringCbPrintf_e( fName, sizeof( fName ), "Save\\%s" , g_MyInfo.GetPublicID().c_str() );
	CreateDirectory( "Save", NULL );
	CreateDirectory( fName, NULL );

	char szDstFile[MAX_PATH] = "";
	sprintf( szDstFile, "%s\\%s.jpg", fName, m_CustomFindModeBack[CUSTOM_FADEIN].m_SetName.c_str() );
	if( ::CopyFile( szFullName, szDstFile, FALSE ) )
	{
		// JPG�� �ΰ� ����ϹǷ� ���� JPG�� ���粨�� �����Ѵ�.
		char szDstFile[MAX_PATH] = "";
		sprintf( szDstFile, "%s\\%s.jpg", fName, m_CustomFindModeBack[CUSTOM_RENDER].m_SetName.c_str() );
		if( ::CopyFile( szFullName, szDstFile, FALSE ) )
			return true;
	}
	return false;
}

bool ioInterfaceBackground::IsCustomFindModeBack()
{
	return m_CustomFindModeBack[CUSTOM_RENDER].m_bOnRender;
}

void ioInterfaceBackground::CustomFindModehRenderFadeIn( int iAlphaRate )
{
	if( m_eBackType != BGT_MODE ) return;
	if( iAlphaRate == MAX_ALPHA_RATE ) return;
	if( !m_bCustomFadeIn ) return;

	ioUIRenderImage *pAlphaBack = NULL;
	if( !m_CustomFindModeBack[CUSTOM_RENDER].m_bOnRender )
	{
		//  �⺻ �̹����� ����Ǿ��� ��� �������ϴ� Ŀ���� �̹����� �ٴڿ� ��� ���̵� �� ����
		if( m_CustomFindModeBack[CUSTOM_RENDER].m_pCustomRes )
			pAlphaBack = m_CustomFindModeBack[CUSTOM_RENDER].m_pCustomRes;
	}
	else if( m_CustomFindModeBack[CUSTOM_FADEIN].m_pCustomRes )
	{
		// Ŀ���ҿ��� Ŀ�������� ����Ǿ����� ���� Ŀ���� �̹����� �ٴڿ� ��� ���̵� �� ����
		pAlphaBack = m_CustomFindModeBack[CUSTOM_FADEIN].m_pCustomRes;
	}

	if( !pAlphaBack && m_ePrevBackType != BGT_NONE )
	{  
		// �⺻���� Ŀ�������� ����Ǿ����� ���� �⺻ �̹����� �ٴڿ� ��� ���̵� �� ����
		BackGroundMap::iterator iter = m_BackGroundMap.find( BGT_MODE );
		if( iter != m_BackGroundMap.end() )
			pAlphaBack = iter->second;
	}

	if( pAlphaBack )
	{
		pAlphaBack->SetHorzFormatting( ioUIRenderImage::HF_STRETCH );
		pAlphaBack->SetVertFormatting( ioUIRenderImage::VF_STRETCH );
		pAlphaBack->SetSize( Setting::Width(), Setting::Height() );
		pAlphaBack->SetAlpha( MAX_ALPHA_RATE );
		pAlphaBack->Render( 0, 0, UI_RENDER_NORMAL, TFO_BILINEAR );
	}
}

// -------------------------------------------------------------------------------------------------------------------------------------------

ioUIRenderImage *ioInterfaceBackground::GetBackgroundRes( BackGroundType eBackType )
{
	if( BGT_LOBBY == eBackType )
	{
		// Ŀ���� �̹����� ������ Ŀ���� �̹����� ��ü
		if( m_CustomLobbyBack[CUSTOM_RENDER].m_bOnRender )
		{
			if( m_CustomLobbyBack[CUSTOM_RENDER].m_pCustomRes )
				return m_CustomLobbyBack[CUSTOM_RENDER].m_pCustomRes;
		}
	}
	else if( BGT_BATTLE == eBackType )
	{
		// Ŀ���� �̹����� ������ Ŀ���� �̹����� ��ü
		if( m_CustomBattleBack.m_bOnRender )
		{
			if( m_CustomBattleBack.m_pCustomRes )
				return m_CustomBattleBack.m_pCustomRes;
		}
	}
	else if( BGT_LADDER == eBackType )
	{
		// Ŀ���� �̹����� ������ Ŀ���� �̹����� ��ü
		if( m_CustomLadderBack.m_bOnRender )
		{
			if( m_CustomLadderBack.m_pCustomRes )
				return m_CustomLadderBack.m_pCustomRes;
		}
	}
	else if( BGT_MODE == eBackType )
	{
		// Ŀ���� �̹����� ������ Ŀ���� �̹����� ��ü
		if( m_CustomFindModeBack[CUSTOM_RENDER].m_bOnRender )
		{
			if( m_CustomFindModeBack[CUSTOM_RENDER].m_pCustomRes )
				return m_CustomFindModeBack[CUSTOM_RENDER].m_pCustomRes;
		}
	}

	// Ŀ���� �̹��� ������ �⺻ �̹��� ���
	BackGroundMap::iterator iter = m_BackGroundMap.find( eBackType );
	if( iter != m_BackGroundMap.end() )
	{
		ioUIRenderImage *pReturn = iter->second;
		if( pReturn )
			return pReturn;
	}

	// �̹����� ������ �ε� �̹����� ��ü
	return GetLoading( m_bFirstLoading, m_iModeType, m_iMyRoomType );
}

void ioInterfaceBackground::SetBackground( BackGroundType eBackType )
{
	m_ePrevBackType = m_eBackType;
	m_eBackType = eBackType;

	if( m_eBackType == BGT_NONE ) return;

	m_dwCurrentTime = 0;
	if( m_dwFadeInTime == 0 )
		m_bFadeInStart = false;
	else
		m_bFadeInStart = true;

	if( m_bFadeInStart )
		m_iAlphaRate = 0;
	else
		m_iAlphaRate = MAX_ALPHA_RATE;
}

void ioInterfaceBackground::SetLoading( const ioHashString &rkLoading )
{
	SAFEDELETE( m_pLoadingRes );
	m_szLoadingRes = rkLoading;	
	if( !m_szLoadingRes.IsEmpty() )
	{
		m_pLoadingRes = g_UIImageSetMgr.CreateImage( m_szLoadingRes.c_str(), "Back001", true );
	}
}

ioUIRenderImage *ioInterfaceBackground::GetLoading( bool bFirstLoading, int iModeType, int iMyRoomType )
{
	m_bFirstLoading = bFirstLoading;
	m_iModeType     = iModeType;
	m_iMyRoomType   = iMyRoomType;

	if( !m_bFirstLoading )  
	{
		// �κ� / �������� / ����&���������� �ε� ȭ���� �ش� ��� ȭ������ ��ü
		if( m_iModeType == MT_NONE || m_iModeType == MT_MYROOM )
		{
			switch( m_iMyRoomType )
			{
			case MRT_LOBBY:
				return GetBackgroundRes( BGT_LOBBY );
			case MRT_BATTLE:
			case MRT_SHUFFLE:
				return GetBackgroundRes( BGT_BATTLE );
			case MRT_LADDER:
				return GetBackgroundRes( BGT_LADDER );
			case MRT_FIND_MODE:
				return GetBackgroundRes( BGT_MODE );
			}
		}

		// �Ϲ� �ε� Ŀ���� 
		if( m_CustomLoadingBack.m_bOnRender )
			return m_CustomLoadingBack.m_pCustomRes;
	}

	// ù �ε� & �Ϲ� �ε�(Ŀ���� ���� ���� ���)
	return m_pLoadingRes;
}

bool ioInterfaceBackground::Render( ioUIRenderImage *pRes, int iAlphaRate )
{
	if( !pRes )	return false;

	pRes->SetHorzFormatting( ioUIRenderImage::HF_STRETCH );
	pRes->SetVertFormatting( ioUIRenderImage::VF_STRETCH );
	pRes->SetSize( Setting::Width(), Setting::Height() );
	pRes->SetAlpha( (BYTE)iAlphaRate );
	pRes->Render( 0, 0, UI_RENDER_NORMAL, TFO_BILINEAR );

	return true;
}

void ioInterfaceBackground::ProcessBackground()
{
	if( m_dwFadeInTime == 0 ) return;

	if( m_dwCurrentTime == 0 )
	{
		if( m_bFadeInStart )
		{
			m_dwCurrentTime = FRAMEGETTIME();
			m_bFadeInStart  = false;
		}
		return;
	}

	DWORD dwGap = FRAMEGETTIME() - m_dwCurrentTime;
	m_iAlphaRate = (float)MAX_ALPHA_RATE * (float)( (float)dwGap / (float)m_dwFadeInTime );

	if( dwGap >= m_dwFadeInTime )
	{
		m_iAlphaRate = MAX_ALPHA_RATE;
		m_dwCurrentTime = 0;
		m_bCustomFadeIn = false;
		for(int i = 0;i < MAX_CUSTOM;i++)
		{
			if( !m_CustomLobbyBack[i].m_bOnRender )
			{
				SAFEDELETE( m_CustomLobbyBack[i].m_pCustomRes );
				g_UIImageSetMgr.DestroyImageSet( m_CustomLobbyBack[i].m_SetName );
			}
		}
	}
}

void ioInterfaceBackground::RenderBackground()
{
	ioUIRenderImage *pBackImage = GetBackgroundRes( m_eBackType );
	if( !pBackImage ) return;

	if( m_eBackType != BGT_RESULT && m_iAlphaRate != MAX_ALPHA_RATE )
	{
		Render( GetLoading( m_bFirstLoading, m_iModeType, m_iMyRoomType ), MAX_ALPHA_RATE );
	}

	switch( m_eBackType )
	{
	case BGT_LOBBY:
		CustomLobbyRenderFadeIn( m_iAlphaRate );	
		break;
	case BGT_MODE:
		CustomFindModehRenderFadeIn( m_iAlphaRate );
		break;
	}

	Render( pBackImage, m_iAlphaRate );
}