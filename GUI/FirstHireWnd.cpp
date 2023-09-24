

#include "StdAfx.h"

#include "../io3DEngine/ioINILoader.h"

#include "../io3DEngine/ioWnd.h"
#include "../io3DEngine/ioFontManager.h"
#include "../io3DEngine/ioUIRenderImage.h"
#include "../io3DEngine/ioOrientBox.h"
#include "../io3DEngine/HelpFunc.h"
#include "../io3DEngine/iostringmanager.h"
#include "../io3dengine/safesprintf.h"

#include "../ioItemMaker.h"
#include "../ioCreateChar.h"
#include "../NetworkWrappingFunc.h"
#include "../ioApplication.h"
#include "../ioEtcItemManager.h"
#include "../ioSetItemInfoManager.h"
#include "../ioSetItemInfo.h"
#include "../ioMyInfo.h"
#include "../ioClassPrice.h"
#include "../TextColorTable.h"
#include "../RaceSetting.h"
#include "../EtcHelpFunc.h"
#include "../Setting.h"

#include "MySoldierWnd.h"
#include "ioUI3DRender.h"
#include "ioSP2GUIManager.h"
#include "FirstHireWnd.h"
#include <strsafe.h>

Hire3DCharWnd::Hire3DCharWnd()
{
	m_pUI3DRender = NULL;
	m_aMyChar	  = NULL;
	m_fRotateYaw  = 0.0f;
}

Hire3DCharWnd::~Hire3DCharWnd()
{
	SAFEDELETE( m_aMyChar );
	SAFEDELETE( m_pUI3DRender );
}

void Hire3DCharWnd::iwm_create()
{
	SAFEDELETE( m_pUI3DRender );

	m_pUI3DRender = new ioUI3DRender;
	m_pUI3DRender->Initialize( "config/sp2_stage_charselect_map.ini", "first_hire_cam" );
}

void Hire3DCharWnd::CreateCharacter( const CHARACTER &rkInfo )
{
	ioCreateChar *pChar = m_pUI3DRender->CreateChar( rkInfo, "HireChar" );
	if( !pChar )	return;

	D3DXVECTOR3 vCharPos( 0.0f, 0.0f, FLOAT500 );
	pChar->SetPosition( vCharPos );
	pChar->SetYaw( FLOAT10 );

	m_pUI3DRender->SetLookAt( vCharPos );

	SAFEDELETE( m_aMyChar );
	m_aMyChar = pChar;
}

void Hire3DCharWnd::EquipItem( const ioItem *pItem )
{
	if( m_aMyChar && pItem )
	{
		m_aMyChar->EquipItem( pItem->GetItemCode(), pItem->GetItemMaleCustom(), pItem->GetItemFemaleCustom() );
	}
}

void Hire3DCharWnd::SetCharAnimation()
{
	if( !m_aMyChar )	return;

	if( !m_aMyChar->SetWeaponShopMotion() )
	{
		ioHashString szUIAnimation = m_pUI3DRender->GetModelAnimation( 0, m_aMyChar->IsMale() );
		if( !szUIAnimation.IsEmpty() )
		{
			m_aMyChar->SetLoopAni( szUIAnimation );
		}
	}
}

void Hire3DCharWnd::RotateY( float fYaw )
{
	m_fRotateYaw = fYaw;
}

void Hire3DCharWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return ;

	if( m_aMyChar )
	{
		m_aMyChar->RotateY( m_fRotateYaw );
		m_aMyChar->Update( fTimePerSec );
	}
}

void Hire3DCharWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	D3DXMATRIX matUI = m_pUI3DRender->MakeUIProjection( iXPos - 306, iYPos + 10 );
	m_pUI3DRender->Render( &matUI, true );
}
//////////////////////////////////////////////////////////////////////////

FirstHireWnd::FirstHireWnd()
{
	m_iClassSelect      = 0;	
	m_iCurRaceDetailIdx = 0;
}

FirstHireWnd::~FirstHireWnd()
{
	m_dwFirstClassList.clear();
	m_EnableRaceList.clear();
	m_szHelpTitleVec.clear();

	for(StringVectorList::iterator it = m_DescVecList.begin(); it != m_DescVecList.end(); ++it)
	{
		StringVector &szVec = *it;    
		szVec.clear();
	}
	m_DescVecList.clear();
}

void FirstHireWnd::iwm_show()
{
	SetChildInActive( ID_EXIT );
	SetChildInActive( ID_CHAROVER );

	m_iClassSelect      = 0;
	m_iCurRaceDetailIdx = 0;

	m_CharInfo.m_class_type  = 0;
	m_CharInfo.m_kindred	 = 0;
	m_CharInfo.m_sex		 = 0;
	m_CharInfo.m_beard		 = 1;
	m_CharInfo.m_face		 = 1;
	m_CharInfo.m_hair		 = 1;
	m_CharInfo.m_skin_color	 = 1;
	m_CharInfo.m_hair_color	 = 1;
	m_CharInfo.m_accessories = 1;
	m_CharInfo.m_underwear   = 1;

	for (int i = 0; i < MAX_INVENTORY ; i++)
		m_CharInfo.m_extra_item[i] = 0;

	m_CharInfo.m_iLimitSecond   = 0;
	m_CharInfo.m_bActive        = true;

	IntVec vDeleteArrayList;
	vDeleteArrayList.reserve(10);
	UpdateFirstClass( vDeleteArrayList );
	UpdateRaceDetail();
	UpdateCharacter();
	ShowChildWnd( ID_CHARACTER );
	SetChildInActive( ID_KINDRED_LEFT );
	SetChildInActive( ID_KINDRED_RIGHT );

	//Help
	LoadINI( vDeleteArrayList );
	vDeleteArrayList.clear();
	SetBtnTitle();

	CheckHideWnd();
}

void FirstHireWnd::iwm_hide()
{
	HideChildWnd( ID_CHARACTER );
}

void FirstHireWnd::UpdateFirstClass( OUT IntVec &rvDeleteArrayList )
{
	m_dwFirstClassList.clear();

	ioINILoader_e kLoader( "config/sp2_leader_item.ini" );
	kLoader.SetTitle_e( "LEADER_CREATE_INFO" );
	int max_item = kLoader.LoadInt_e( "max_item", 0 );

	char szKeyName[MAX_PATH];
	
	for(int i = 0;i < max_item;i++)
	{
		sprintf_e( szKeyName, "item_code_%d", i + 1);
		int iClassType = kLoader.LoadInt( szKeyName, 0 );
		DWORD dwSetItemCode = iClassType + SET_ITEM_CODE;
		const ioSetItemInfo *pSetItemInfo = g_SetItemInfoMgr.GetSetInfoByCode( dwSetItemCode );
		if( g_MyInfo.IsCanBuyItem( pSetItemInfo ) && !g_ClassPrice.IsCashOnly( iClassType ) )
			m_dwFirstClassList.push_back( iClassType );
		else
			rvDeleteArrayList.push_back( i );
	}		
}

void FirstHireWnd::UpdateRaceDetail()
{
	m_EnableRaceList.clear();
	m_EnableRaceList.push_back( RDT_HUMAN_MAN );
	m_EnableRaceList.push_back( RDT_HUMAN_WOMAN );
	m_EnableRaceList.push_back( RDT_ELF_MAN );
	m_EnableRaceList.push_back( RDT_ELF_WOMAN );
	m_EnableRaceList.push_back( RDT_DWARF_MAN );
	m_EnableRaceList.push_back( RDT_DWARF_WOMAN );
}

void FirstHireWnd::UpdateCharacter()
{
	UpdateCharInfo( m_EnableRaceList[m_iCurRaceDetailIdx] );

	Hire3DCharWnd *pCharWnd = (Hire3DCharWnd *)FindChildWnd( ID_CHARACTER );
	if( !pCharWnd )	return;

	// 캐릭터
	pCharWnd->CreateCharacter( m_CharInfo );

	// 장비 장착 
	DWORD dwSetItemIdx = 0;
	if( COMPARE( m_iClassSelect, 0, (int)m_dwFirstClassList.size() ) )
	{
		dwSetItemIdx = m_dwFirstClassList[m_iClassSelect];
	}

	const ioSetItemInfo *pSetInfo = g_SetItemInfoMgr.GetSetInfoByIdx( dwSetItemIdx - 1 );
	if( pSetInfo )
	{
		m_szClassName = pSetInfo->GetName();

		ConstItemVector rkItemList = pSetInfo->GetSetItemList();

		int iMaxItemCnt = min( rkItemList.size(), MAX_INVENTORY );
		for( int i=0 ; i<iMaxItemCnt; i++ )
		{
			pCharWnd->EquipItem( rkItemList[i] );
		}
	}

	pCharWnd->SetCharAnimation();
}

void FirstHireWnd::UpdateCharInfo( RaceDetailType eDetail )
{
	switch( eDetail )
	{
	case RDT_HUMAN_MAN:
		m_CharInfo.m_kindred = 1;
		m_CharInfo.m_sex = 1;
		m_szKindredName = STR(1);
		break;
	case RDT_HUMAN_WOMAN:
		m_CharInfo.m_kindred = 1;
		m_CharInfo.m_sex = 2;
		m_szKindredName = STR(2);
		break;
	case RDT_ELF_MAN:
		m_CharInfo.m_kindred = 2;
		m_CharInfo.m_sex = 1;
		m_szKindredName = STR(3);
		break;
	case RDT_ELF_WOMAN:
		m_CharInfo.m_kindred = 2;
		m_CharInfo.m_sex = 2;
		m_szKindredName = STR(4);
		break;
	case RDT_DWARF_MAN:
		m_CharInfo.m_kindred = 3;
		m_CharInfo.m_sex = 1;
		m_szKindredName = STR(5);
		break;
	case RDT_DWARF_WOMAN:
		m_CharInfo.m_kindred = 3;
		m_CharInfo.m_sex = 2;
		m_szKindredName = STR(6);
		break;
	}

	RaceSetting::SetCurRaceDetail( m_CharInfo );	
	m_CharInfo.m_beard	= RotateNum( m_CharInfo.m_beard, 1, RaceSetting::GetNumBeardParts() - 1 );
	m_CharInfo.m_face	= RotateNum( m_CharInfo.m_face, 1, RaceSetting::GetNumFaceParts() - 1 );
	m_CharInfo.m_hair	= RotateNum( m_CharInfo.m_hair, 1, RaceSetting::GetNumHairParts() - 1 );
	m_CharInfo.m_hair_color = RotateNum( m_CharInfo.m_hair_color, 1, RaceSetting::GetNumHairColor( m_CharInfo.m_hair ) - 1 );
	m_CharInfo.m_skin_color = RotateNum( m_CharInfo.m_skin_color, 1, RaceSetting::GetNumSkinColor() - 1 );
	m_CharInfo.m_accessories= RotateNum( m_CharInfo.m_accessories, 1, RaceSetting::GetNumAccessoryParts() - 1 );
	m_CharInfo.m_underwear= RotateNum( m_CharInfo.m_underwear, 1, RaceSetting::GetNumUnderwearParts() - 1 );
}

int FirstHireWnd::RotateNum( int iCurNum, int iMinNum, int iMaxNum )
{
	if( iMinNum == iMaxNum )
		return iMinNum;
	if( iCurNum < iMinNum )
		return iCurNum + iMaxNum;
	else if( iCurNum > iMaxNum )
		return iCurNum % iMaxNum;

	return iCurNum;
}

void FirstHireWnd::ChangeClass( bool bAdd )
{
	if( bAdd )
	{
		m_iClassSelect++;
		if( !COMPARE( m_iClassSelect, 0, (int)m_dwFirstClassList.size() ) )
			m_iClassSelect = 0;
	}
	else
	{
		m_iClassSelect--;
		if( !COMPARE( m_iClassSelect, 0, (int)m_dwFirstClassList.size() ) )
			m_iClassSelect = m_dwFirstClassList.size() - 1;
	}

	UpdateCharacter();
}

void FirstHireWnd::ChangeCharacter( bool bAdd )
{
	if( bAdd )
	{
		m_iCurRaceDetailIdx++;
		if( !COMPARE( m_iCurRaceDetailIdx, 0, (int)m_EnableRaceList.size() ) )
			m_iCurRaceDetailIdx = 0;
	}
	else
	{
		m_iCurRaceDetailIdx--;
		if( !COMPARE( m_iCurRaceDetailIdx, 0, (int)m_EnableRaceList.size() ) )
			m_iCurRaceDetailIdx = m_EnableRaceList.size() - 1;
	}

	UpdateCharInfo( m_EnableRaceList[m_iCurRaceDetailIdx] );
	UpdateCharacter();
}

void FirstHireWnd::SplitDescription( const ioHashString &szDesc, OUT StringVector &rszStringVec )
{
	rszStringVec.clear();
	rszStringVec = ioStringConverter::Split( szDesc.c_str(), "#" );
}

void FirstHireWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_EXIT:
		if( cmd == IOBN_BTNUP )
		{
		}
		break;
	case ID_CLASS_LEFT:
		if( cmd == IOBN_BTNUP )
		{
			ChangeClass( false );
		}
		break;
	case ID_CLASS_RIGHT:
		if( cmd == IOBN_BTNUP )
		{
			ChangeClass( true );
		}
		break;
	case ID_KINDRED_LEFT:
		if( cmd == IOBN_BTNUP )
		{
			ChangeCharacter( false );
		}
		break;
	case ID_KINDRED_RIGHT:
		if( cmd == IOBN_BTNUP )
		{
			ChangeCharacter( true );
		}
		break;
	case ID_CHAROVER:
		if( cmd == IOBN_BTNDOWN )
		{
			switch( param )
			{
			case MSCrossWnd::ID_L_ROTATE:
				{
					Hire3DCharWnd *pChar = (Hire3DCharWnd *)FindChildWnd( ID_CHARACTER );
					if( pChar )
						pChar->RotateY( FLOAT1 );
				}
				break;
			case MSCrossWnd::ID_R_ROTATE:
				{
					Hire3DCharWnd *pChar = (Hire3DCharWnd *)FindChildWnd( ID_CHARACTER );
					if( pChar )
						pChar->RotateY( -FLOAT1 );
				}
				break;
			case MSCrossWnd::ID_PLUS:
				break;
			case MSCrossWnd::ID_MINUS:
				break;
			case MSCrossWnd::ID_CHANGE:
				break;
			}
		}
		else if( cmd == IOBN_BTNUP )
		{
			switch( param )
			{
			case MSCrossWnd::ID_L_ROTATE:
				{
					Hire3DCharWnd *pChar = (Hire3DCharWnd *)FindChildWnd( ID_CHARACTER );
					if( pChar )
						pChar->RotateY( 0.0f );
				}
				break;
			case MSCrossWnd::ID_R_ROTATE:
				{
					Hire3DCharWnd *pChar = (Hire3DCharWnd *)FindChildWnd( ID_CHARACTER );
					if( pChar )
						pChar->RotateY( 0.0f );
				}
				break;
			case MSCrossWnd::ID_PLUS:
				break;
			case MSCrossWnd::ID_MINUS:
				break;
			case MSCrossWnd::ID_CHANGE:
				break;
			}
		}
		break;
	case ID_FIRST_HIRE:
		if( cmd == IOBN_BTNUP )
		{
			if( g_MyInfo.GetUserState() != 0 )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
				return;
			}
			HideWnd();

			SP2Packet kPacket( CTPK_CHAR_CREATE );
			kPacket << CHAR_CREATE_FIRST_HIRE;
			kPacket << m_dwFirstClassList[m_iClassSelect];
			kPacket << m_CharInfo.m_kindred;
			kPacket	<< m_CharInfo.m_sex;
			kPacket	<< g_ClassPrice.GetFirstHireLimit();
			kPacket	<< 0;     // iPrice
            kPacket << false; // bCash
			kPacket << CPT_TIME;

			TCPNetwork::SendToServer( kPacket );
			g_App.SetMouseBusy( true );
		}
		break;
	}
}

void FirstHireWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	g_FontMgr.SetAlignType( TAT_LEFT );
	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
	g_FontMgr.PrintText( iXPos + 212, iYPos + 60, FONT_SIZE_13, GetCurHelpTitle() );
	
	const StringVector *pDescVec = GetCurDescVec();
	if( pDescVec ) 
	{
		int i = 0;
		for(StringVector::const_iterator it = pDescVec->begin(); it != pDescVec->end(); ++it)
		{
			const std::string &szLine = *it;
			if( !szLine.empty() )
				g_FontMgr.PrintText( iXPos + 191, iYPos + 88 + (i * 20), FONT_SIZE_13, szLine.c_str() );
			i++;
		}
	}
}

const char *FirstHireWnd::GetCurHelpTitle() const
{
	if( COMPARE( m_iClassSelect, 0, (int) m_szHelpTitleVec.size() ) )
		return m_szHelpTitleVec[m_iClassSelect].c_str();

	return NULL;
}

const StringVector *FirstHireWnd::GetCurDescVec() 
{
	if( COMPARE( m_iClassSelect, 0, (int) m_DescVecList.size() ) )
		return &m_DescVecList[m_iClassSelect];

	return NULL;
}

void FirstHireWnd::LoadINI( IN const IntVec &rvDeleteArrayList )
{
	m_szHelpTitleVec.clear();
	for(StringVectorList::iterator it = m_DescVecList.begin(); it != m_DescVecList.end(); ++it)
	{
		StringVector &szVec = *it;    
		szVec.clear();
	}
	m_DescVecList.clear();

	ioINILoader_e kLoader( "config/sp2_leader_item.ini" );
	kLoader.SetTitle_e( "LEADER_UI_HELP" );
	int max_help = kLoader.LoadInt_e( "max_help", 0 );

	char szKeyName[MAX_PATH]="";
	for (int i = 0; i < max_help ; i++)
	{
		// class list에서 삭제되는 항목은 help에서도 삭제한다
		bool bContinue = false;
		for(IntVec::const_iterator it = rvDeleteArrayList.begin(); it != rvDeleteArrayList.end(); ++it)
		{
			const int &rArray = *it;
			if( i == rArray ) 
			{
				bContinue = true;
				break;
			}
		}
		if( bContinue )
			continue;

		ZeroMemory(szKeyName, sizeof( szKeyName ) );
		StringCbPrintf_e( szKeyName, sizeof(szKeyName), "title_%d", i+1);

		char szBuf[MAX_PATH]="";
		kLoader.LoadString( szKeyName, "", szBuf, MAX_PATH );
		m_szHelpTitleVec.push_back( ioHashString(szBuf) );

		ZeroMemory(szKeyName, sizeof( szKeyName ) );
		StringCbPrintf_e( szKeyName, sizeof(szKeyName), "desc_%d", i+1);

		ZeroMemory( szBuf, sizeof( szBuf) );
		kLoader.LoadString( szKeyName, "", szBuf, MAX_PATH );
		StringVector szDescVec;
		SplitDescription( ioHashString(szBuf) , szDescVec );	
		m_DescVecList.push_back( szDescVec );
		szDescVec.clear();
	}
}

void FirstHireWnd::SetBtnTitle()
{
	ioButton *pFirsHireBtn = (ioButton*)FindChildWnd( ID_FIRST_HIRE );
	if( pFirsHireBtn )
	{
		char szTitle[MAX_PATH] = "";
		if( g_ClassPrice.GetFirstHireLimit() >= 3600 )
			SafeSprintf( szTitle, sizeof( szTitle ), STR(1), g_ClassPrice.GetFirstHireLimit() / 3600 );
		else
			SafeSprintf( szTitle, sizeof( szTitle ), STR(2), g_ClassPrice.GetFirstHireLimit() / 60 );
		pFirsHireBtn->SetTitleText( szTitle );
	}
}

void FirstHireWnd::CheckHideWnd()
{
	if( m_dwFirstClassList.empty() )
	{
		HideWnd();
		return;
	}

	int iMax = (int) m_dwFirstClassList.size();
	for (int i = 0; i < iMax ; i++)
	{
		if( g_MyInfo.GetClassArrayExceptExercise( m_dwFirstClassList[i] ) == -1 )
			return;
		if( !g_MyInfo.IsCharActive( g_MyInfo.GetClassArray( m_dwFirstClassList[i] ) ) )
			return;
	}

	// 구매할 용병이 없다면
	HideWnd();
}

//////////////////////////////////////////////////////////////////////////
FirstSoldierWnd::FirstSoldierWnd()
{
	m_pBackGround = NULL;
	m_pSoldierIcon= NULL;

	m_iClassType = 0;
}

FirstSoldierWnd::~FirstSoldierWnd()
{
	SAFEDELETE( m_pBackGround );
	SAFEDELETE( m_pSoldierIcon );
}

void FirstSoldierWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "BackGround" )
	{
		SAFEDELETE( m_pBackGround );
		m_pBackGround = pImage;
	}
	else
	{
		ioWnd::AddRenderImage( szType, pImage );
	}
}

void FirstSoldierWnd::iwm_show()
{
	m_iClassType = 0;

	ioINILoader_e kLoader( "config/sp2_leader_item.ini" );
	kLoader.SetTitle_e( "LEADER_CREATE_INFO" );
	int max_item = kLoader.LoadInt_e( "max_item", 0 );
	char szKeyName[MAX_PATH];
	for(int i = 0;i < max_item;i++)
	{
		sprintf_e( szKeyName, "item_code_%d", i + 1);
		int iClassType = kLoader.LoadInt( szKeyName, 0 );
		DWORD dwSetItemCode = iClassType + SET_ITEM_CODE;
		const ioSetItemInfo *pSetItemInfo = g_SetItemInfoMgr.GetSetInfoByCode( dwSetItemCode );
		if( g_MyInfo.IsCanBuyItem( pSetItemInfo ) && !g_ClassPrice.IsCashOnly( iClassType ) )
		{
			m_iClassType = iClassType;
			break;
		}
	}		

	if( m_iClassType == 0 )
	{
		HideWnd();
	}
	else
	{
		m_pSoldierIcon = g_MyInfo.GetSoldierIcon( m_iClassType, true );
		m_szClassName  = g_MyInfo.GetClassName( m_iClassType );

		ioWnd *pHireBtn = FindChildWnd( ID_FIRST_HIRE );
		if( pHireBtn )
		{
			char szTitle[MAX_PATH] = "";
			if( g_ClassPrice.GetFirstHireLimit() >= 3600 )
				SafeSprintf( szTitle, sizeof( szTitle ), STR(1), m_szClassName.c_str(), g_ClassPrice.GetFirstHireLimit() / 3600 );
			else
				SafeSprintf( szTitle, sizeof( szTitle ), STR(2), m_szClassName.c_str(), g_ClassPrice.GetFirstHireLimit() / 60 );
			pHireBtn->SetTitleText( szTitle );
		}
	}
}

void FirstSoldierWnd::iwm_hide()
{

}

void FirstSoldierWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_FIRST_HIRE:
		if( cmd == IOBN_BTNUP )
		{
			if( g_MyInfo.GetUserState() != 0 )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
				return;
			}
			HideWnd();

			SP2Packet kPacket( CTPK_CHAR_CREATE );
			kPacket << CHAR_CREATE_FIRST_HIRE;
			kPacket << m_iClassType;
			kPacket << 1;      // 종족 인간
			kPacket	<< 1;      // 성별 남자
			kPacket	<< g_ClassPrice.GetFirstHireLimit();
			kPacket	<< 0;      // iPrice
			kPacket << false;  // bCash
			kPacket << CPT_TIME;

			TCPNetwork::SendToServer( kPacket );
			g_App.SetMouseBusy( true );
		}
		break;
	}
}

void FirstSoldierWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	if( m_pBackGround )
	{
		m_pBackGround->Render( iXPos + 124, iYPos + 97, UI_RENDER_MULTIPLY );
		if( m_pSoldierIcon )
			m_pSoldierIcon->Render( iXPos + 124, iYPos + 97 );
	}

	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
	g_FontMgr.PrintText( iXPos + 124, iYPos + 149, FONT_SIZE_13, STR(1), g_MyInfo.GetPublicID().c_str() );
	g_FontMgr.PrintText( iXPos + 124, iYPos + 167, FONT_SIZE_13, STR(2), m_szClassName.c_str() );
	g_FontMgr.PrintText( iXPos + 124, iYPos + 185, FONT_SIZE_13, STR(3) );
}
///////////////////////////////////////////////////////////////////////////
int BankruptcyCharWnd::m_iClassSelect = 0;
BankruptcyCharWnd::BankruptcyCharWnd()
{
	m_pBackGround  = NULL;
	m_pSoldierIcon = NULL;
}

BankruptcyCharWnd::~BankruptcyCharWnd()
{
	SAFEDELETE( m_pBackGround );
	SAFEDELETE( m_pSoldierIcon );
}

void BankruptcyCharWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "BackGround" )
	{
		SAFEDELETE( m_pBackGround );
		m_pBackGround = pImage;
	}
	else
	{
		ioWnd::AddRenderImage( szType, pImage );
	}
}

void BankruptcyCharWnd::UpdateFirstClass()
{
	if( m_iClassSelect == 0 )
	{
		DWORDVec dwFirstClassList;
		int iSize = g_SetItemInfoMgr.GetTotalSetCount();
		for (int i = 0; i < iSize ; i++)
		{
			if( g_ClassPrice.IsCashOnlyByArray( i ) )
				continue;

			if( !g_ClassPrice.IsActiveClass( i ) )
				continue;

			int iItemCode = g_ClassPrice.GetClassType( i );
			const ioSetItemInfo *pSetItemInfo = g_SetItemInfoMgr.GetSetInfoByItemCode( iItemCode );

			if( !pSetItemInfo )
				continue;

			if( pSetItemInfo->GetPackageType() != ioSetItemInfo::PT_NORMAL )
				continue;

			if( g_MyInfo.IsCanBuyItem( pSetItemInfo ) )
				dwFirstClassList.push_back( pSetItemInfo->GetSetCode() - SET_ITEM_CODE );
		}

		if( dwFirstClassList.empty() )
		{
			HideWnd();
		}
		else
		{
			 int iSize = dwFirstClassList.size();
			 int iRand = rand()%iSize;
			 if( COMPARE( iRand , 0, iSize ) )
				m_iClassSelect = dwFirstClassList[iRand];
		}
	}

	if( m_iClassSelect != 0 )
	{
		// 지급 용병
		SAFEDELETE( m_pSoldierIcon );
		m_pSoldierIcon = g_MyInfo.GetSoldierIcon( m_iClassSelect, true );
		m_szClassName  = g_MyInfo.GetClassName( m_iClassSelect );

		// 내용
		m_szHelpVec.clear();
		ioINILoader_e kLoader( "config/sp2_bankruptcy.ini" );
		kLoader.SetTitle_e( "help" );
		int max_help = kLoader.LoadInt_e( "max_help", 0 );
		char szKeyName[MAX_PATH]="";
		for (int i = 0; i < max_help ; i++)
		{
			ZeroMemory(szKeyName, sizeof( szKeyName ) );
			StringCbPrintf_e( szKeyName, sizeof(szKeyName), "desc_%d", i+1);

			char szBuf[MAX_PATH]="";
			kLoader.LoadString( szKeyName, "", szBuf, MAX_PATH );
			m_szHelpVec.push_back( ioHashString( szBuf ) );
		}

		// 버튼
		ioButton *pFirsHireBtn = (ioButton*)FindChildWnd( ID_RECEIVE_CHAR_BTN );
		if( pFirsHireBtn )
		{
			char szTitle[MAX_PATH] = "";
			SafeSprintf( szTitle, sizeof( szTitle ), STR(1), m_szClassName.c_str(), g_ClassPrice.GetBankruptcyLimit() / 3600 );
			pFirsHireBtn->SetTitleText( szTitle );
		}
	}
}

void BankruptcyCharWnd::iwm_show()
{
	UpdateFirstClass();
}	

void BankruptcyCharWnd::iwm_hide()
{

}

void BankruptcyCharWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();
	switch( dwID )
	{
	case ID_RECEIVE_CHAR_BTN:
		if( cmd == IOBN_BTNUP)
		{	
			if( !g_MyInfo.IsBankruptcy() )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
				HideWnd();
			}
			else if( g_MyInfo.GetClassArrayExceptExercise( m_iClassSelect ) != -1 )
			{
				if( g_MyInfo.IsCharActive( g_MyInfo.GetClassArray( m_iClassSelect ) ) )
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
					return;
				}

				SP2Packet kPacket( CTPK_CHAR_EXTEND );
				kPacket << CHAR_EXTEND_BANKRUPTCY;
				kPacket << g_MyInfo.GetCharIndex( g_MyInfo.GetClassArray( m_iClassSelect ) );
				kPacket << g_ClassPrice.GetBankruptcyLimit();
				kPacket << false; // bCash
				TCPNetwork::SendToServer( kPacket );
				g_App.SetMouseBusy( true );
				g_App.ResetConnectedMoveMode();

				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
				HideWnd();
			}
			else if( g_MyInfo.GetCharCount() >= g_MyInfo.GetCurMaxCharSlot() )
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
				HideWnd();
			}
			else
			{
				SP2Packet kPacket( CTPK_CHAR_CREATE );
				kPacket << CHAR_CREATE_BANKRUPTCY; 
				kPacket << m_iClassSelect;
				kPacket << 1;      // 종족 인간
				kPacket	<< 1;      // 성별 남자
				kPacket << g_ClassPrice.GetBankruptcyLimit();
				kPacket << 0;     // iPrice
				kPacket << false; // bCash
				kPacket << CPT_TIME;

				TCPNetwork::SendToServer( kPacket );
				g_App.SetMouseBusy( true );
				g_App.ResetConnectedMoveMode();

				HideWnd();
				m_iClassSelect = 0;
			}
		}
		break;
	}
}

void BankruptcyCharWnd::OnRender()
{
	ioWnd::OnRender();
	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	if( m_pBackGround )
	{
		m_pBackGround->Render( iXPos + 124, iYPos + 97, UI_RENDER_MULTIPLY );
		if( m_pSoldierIcon )
			m_pSoldierIcon->Render( iXPos + 124, iYPos + 97 );
	}

	g_FontMgr.SetAlignType( TAT_CENTER );
	g_FontMgr.SetTextStyle( TS_NORMAL );
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextColor( TCT_DEFAULT_DARKGRAY );
	for(int i = 0;i < (int)m_szHelpVec.size();i++)
	{
		g_FontMgr.PrintText( iXPos + 124, iYPos + 149 + i * 18, FONT_SIZE_13, m_szHelpVec[i].c_str() );
	}
}