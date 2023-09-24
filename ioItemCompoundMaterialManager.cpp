
#include "stdafx.h"

#include "ioItemCompoundMaterialManager.h"

#include "ioUserExtraItem.h"

template<> ioItemCompoundMaterialManager* Singleton< ioItemCompoundMaterialManager >::ms_Singleton = 0;

ioItemCompoundMaterialManager::ioItemCompoundMaterialManager()
{
	ClearData();
}

ioItemCompoundMaterialManager::~ioItemCompoundMaterialManager()
{
	ClearData();
}

ioItemCompoundMaterialManager& ioItemCompoundMaterialManager::GetSingleton()
{
	return Singleton< ioItemCompoundMaterialManager >::GetSingleton();
}

void ioItemCompoundMaterialManager::LoadCompoundMaterialInfo()
{	
	ClearData();

	ioINILoader_e kLoader( "config/sp2_item_compound_material_info.ini" );

	char szBuf[MAX_PATH];

	kLoader.SetTitle_e( "piece_compound" );
	
	m_nMaxReinforce = (int)kLoader.LoadInt_e( "max_reinforce", 0 );
	m_nFixedFailReinforce = (int)kLoader.LoadInt_e( "fixed_fail_reinforce", 0 );
	m_nAllAlarmLevel = (int)kLoader.LoadInt_e( "all_alarm_level", 0 );
		
	int nEtcCnt = kLoader.LoadInt( "max_etc_need_material", 0 );
	for ( int i=0; i< nEtcCnt; i++ )
	{
		EtcNeedMaterial sEtcNeedMaterial;

		wsprintf_e( szBuf, "etc_itemcode%d", i+1);
		sEtcNeedMaterial.m_nItemCode = kLoader.LoadInt( szBuf, 0 );
		wsprintf_e( szBuf, "etc_material%d", i+1);
		sEtcNeedMaterial.m_nMaterial = kLoader.LoadInt( szBuf, 0 );
		m_EtcNeedMaterialList.push_back( sEtcNeedMaterial );
	}

	int nReinforceCnt = kLoader.LoadInt( "max_reinforce_need_material", 0 );
	for ( int i=0; i< nReinforceCnt; i++ )
	{
		ReinforceNeedMaterial sReinforceNeedMaterial;

		wsprintf_e( szBuf, "reinforce_%d", i+1);
		sReinforceNeedMaterial.m_nReinforce = kLoader.LoadInt( szBuf, 0 );
		wsprintf_e( szBuf, "reinforce_material%d", i+1);
		sReinforceNeedMaterial.m_nMaterial = kLoader.LoadInt( szBuf, 0 );
		m_ReinforceNeedMaterialList.push_back( sReinforceNeedMaterial );
	}

	int nMaxCnt = kLoader.LoadInt_e( "max_info_cnt", 0 );
	if( nMaxCnt > 0 )
	{
		int nTemp = 1;
		while( 1 )
		{
			wsprintf_e( szBuf, "piece_compound_rate%d", nTemp );
			kLoader.SetTitle( szBuf );

			DWORD dwCode = (DWORD)kLoader.LoadInt_e( "compound_item_code", 0 );
			if( dwCode == 0 )
				break;

			nTemp++;

			CompoundMaterialInfo sCompoundMaterialInfo;

			//성공 상수
			sCompoundMaterialInfo.m_nSuccessConst = (int)kLoader.LoadInt_e( "success_constant", 0 );
			//레어 실패 경험치 배율
			sCompoundMaterialInfo.m_fFailExpConst = (float)kLoader.LoadInt_e( "rare_max_fail_exp_rate", 0 );
			//레어 필요 개수 배율
			sCompoundMaterialInfo.m_fExtraNeedRate = (float)kLoader.LoadFloat_e( "rare_need_material_rate", 0 );

			int nInfoSize = (int)kLoader.LoadInt_e( "max_reinforce_cnt", 0 );
			sCompoundMaterialInfo.m_vMaterialRateInfoList.reserve( nInfoSize );

			for( int i=0; i < nInfoSize; ++i )
			{
				MaterialRateInfo sMaterialRateInfo;
				wsprintf_e( szBuf, "compound%d_level", i+1 );
				sMaterialRateInfo.m_nLevel = kLoader.LoadInt( szBuf, 0 );

				wsprintf_e( szBuf, "compound%d_need_material", i+1 );
				sMaterialRateInfo.m_nPiece = kLoader.LoadInt( szBuf, 100000 );

				wsprintf_e( szBuf, "compound%d_max_fail_exp", i+1 );
				sMaterialRateInfo.m_nFailExp = kLoader.LoadInt( szBuf, 0 );

				sMaterialRateInfo.m_nAdditive = sMaterialRateInfo.m_nPiece * sCompoundMaterialInfo.m_fExtraNeedRate;
				
				wsprintf_e( szBuf, "compound%d_success_rate", i+1 );
				sMaterialRateInfo.m_nSuccessRate = kLoader.LoadInt( szBuf, 0 );
				
				sMaterialRateInfo.m_nFailExpExtra = sMaterialRateInfo.m_nFailExp * sCompoundMaterialInfo.m_fFailExpConst;

				wsprintf_e( szBuf, "pc_room_bonus%d", i+1 );
				sMaterialRateInfo.m_nPcRoomBonus = kLoader.LoadInt( szBuf, 0 );

				sCompoundMaterialInfo.m_vMaterialRateInfoList.push_back( sMaterialRateInfo );
			}
			m_CompoundMaterialMap.insert( CompoundMaterialMap::value_type( dwCode, sCompoundMaterialInfo ) );
		}
	}
}

void ioItemCompoundMaterialManager::ClearData()
{
	CompoundMaterialMap::iterator iter = m_CompoundMaterialMap.begin();
	for( ;iter != m_CompoundMaterialMap.end(); ++iter )
	{
		CompoundMaterialInfo &Info = iter->second;
		Info.m_vMaterialRateInfoList.clear();
	}

	m_CompoundMaterialMap.clear();
	m_EtcNeedMaterialList.clear();
	m_ReinforceNeedMaterialList.clear();
}

bool ioItemCompoundMaterialManager::CheckAllAlarm( int nTargetReinforce )
{
	if( m_nAllAlarmLevel <= nTargetReinforce )
		return true;

	return false;
}

//강화 성공 상수 얻기
int ioItemCompoundMaterialManager::GetSuccessConst( DWORD dwCode )
{	
	if ( dwCode == 0 )
		return 0;

	CompoundMaterialMap::iterator iter = m_CompoundMaterialMap.find( dwCode );
	if( iter != m_CompoundMaterialMap.end() )
	{
		CompoundMaterialInfo &Info = iter->second;
		return Info.m_nSuccessConst;
	}
	return 0;
}

//강화 레벨별 성공율 얻기
int ioItemCompoundMaterialManager::GetReinforceSuccessRate( int nTargetReinforce, DWORD dwCode )
{
	if ( dwCode == 0 )
		return 0;

	CompoundMaterialMap::iterator iter = m_CompoundMaterialMap.find( dwCode );
	if( iter != m_CompoundMaterialMap.end() )
	{
		CompoundMaterialInfo &Info = iter->second;
		int nMaxCnt = Info.m_vMaterialRateInfoList.size();
		for (int i=0; i<nMaxCnt; i++)
		{
			if ( Info.m_vMaterialRateInfoList[i].m_nLevel == nTargetReinforce )
				return Info.m_vMaterialRateInfoList[i].m_nSuccessRate;
		}
	}
	return 0;
}

//강화 레벨별 추가 성공율 얻기 (PC방 보너스)
int ioItemCompoundMaterialManager::GetPCRoomBonusReinforceSuccessRate( int nTargetReinforce, DWORD dwCode )
{
	if ( dwCode == 0 )
		return 0;

	if ( !g_MyInfo.GetPCRoomAuthority() )
		return 0;

	if( g_MyInfo.IsUserEvent() )
		return 0;

	CompoundMaterialMap::iterator iter = m_CompoundMaterialMap.find( dwCode );
	if( iter != m_CompoundMaterialMap.end() )
	{
		CompoundMaterialInfo &Info = iter->second;
		int nMaxCnt = Info.m_vMaterialRateInfoList.size();
		for (int i=0; i<nMaxCnt; i++)
		{
			if ( Info.m_vMaterialRateInfoList[i].m_nLevel == nTargetReinforce )
				return Info.m_vMaterialRateInfoList[i].m_nPcRoomBonus;
		}
	}
	return 0;
}

//강화 레벨별 요구 재료수 얻기
int ioItemCompoundMaterialManager::GetNeedMaterialCount( int nTargetReinforce, DWORD dwCode, bool bExtra )
{
	if ( dwCode == 0 )
		return 0;

	CompoundMaterialMap::iterator iter = m_CompoundMaterialMap.find( dwCode );
	if( iter != m_CompoundMaterialMap.end() )
	{
		CompoundMaterialInfo &Info = iter->second;

		int nMaxCnt = Info.m_vMaterialRateInfoList.size();
		for( int i=0; i<nMaxCnt; ++i )
		{
			if( Info.m_vMaterialRateInfoList[i].m_nLevel == nTargetReinforce )
			{
				if ( bExtra )
					return Info.m_vMaterialRateInfoList[i].m_nAdditive;
				else
					return Info.m_vMaterialRateInfoList[i].m_nPiece;
			}
		}
	}
	return 0;
}

//강화 레벨별 최대 실패 경험치 얻기
int ioItemCompoundMaterialManager::GetMaxFailExp( int nTargetReinforce, DWORD dwCode, bool bExtra )
{
	if ( dwCode == 0 || nTargetReinforce >= m_nFixedFailReinforce )
		return 0;
	
	CompoundMaterialMap::iterator iter = m_CompoundMaterialMap.find( dwCode );
	if( iter != m_CompoundMaterialMap.end() )
	{
		CompoundMaterialInfo &Info = iter->second;

		int nMaxCnt = Info.m_vMaterialRateInfoList.size();		
		for( int i=0; i<nMaxCnt; ++i )
		{
			if( Info.m_vMaterialRateInfoList[i].m_nLevel == nTargetReinforce )
			{
				if ( bExtra )
					return Info.m_vMaterialRateInfoList[i].m_nFailExpExtra;
				else
					return Info.m_vMaterialRateInfoList[i].m_nFailExp;
			}
		}
	}
	return 0;
}

bool ioItemCompoundMaterialManager::IsExtraItem( int nItemCode )
{
	int nEtcCnt = (int)m_EtcNeedMaterialList.size();
	for ( int i=0; i<nEtcCnt; ++i )
	{
		if ( m_EtcNeedMaterialList[i].m_nItemCode == nItemCode )
			return true;
	}

	int nConfirmCode = nItemCode / CT_WeaponCode;	

	//무기가 아니면
	if ( nConfirmCode != CT_Yes )
	{		
		nConfirmCode = nItemCode % CT_ExtraCode;

		if ( nConfirmCode > CT_ExtraItemStart )
			return true;
	}
	return false;
}

int ioItemCompoundMaterialManager::NeedMaterialCode( int nItemCode, int nReinforce )
{
	if ( nItemCode == 0 )
		return nItemCode;

	if ( nReinforce >= m_nFixedFailReinforce )
	{
		int nCnt = (int)m_ReinforceNeedMaterialList.size();
		for ( int i=0; i<nCnt; ++i )
		{
			if ( m_ReinforceNeedMaterialList[i].m_nReinforce == nReinforce )
				return m_ReinforceNeedMaterialList[i].m_nMaterial;
		}
	}

	int nEtcCnt = (int)m_EtcNeedMaterialList.size();
	for ( int i=0; i<nEtcCnt; ++i )
	{
		if ( m_EtcNeedMaterialList[i].m_nItemCode == nItemCode )
			return m_EtcNeedMaterialList[i].m_nMaterial;
	}

	int nConfirmCode = nItemCode / CT_WeaponCode;	

	//무기가 아니면
	if ( nConfirmCode != CT_Yes )
	{		
		nConfirmCode = nItemCode % CT_ExtraCode;

		if ( nConfirmCode > CT_ExtraItemStart )
			return CT_Additive;
	}

	nConfirmCode = nItemCode % CT_Mercenary;

	return nConfirmCode;
}
