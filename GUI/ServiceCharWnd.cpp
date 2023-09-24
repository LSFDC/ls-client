

#include "StdAfx.h"

#include "../io3DEngine/ioINILoader.h"

#include "../io3DEngine/ioWnd.h"
#include "../io3DEngine/ioFontManager.h"
#include "../io3DEngine/ioUIRenderImage.h"
#include "../io3DEngine/ioOrientBox.h"
#include "../io3DEngine/iostringmanager.h"

#include "../ioItemMaker.h"
#include "../ioCreateChar.h"
#include "../NetworkWrappingFunc.h"
#include "../ioApplication.h"
#include "../ioEtcItemManager.h"
#include "../ioSetItemInfoManager.h"
#include "../ioSetItemInfo.h"
#include "../ioMyInfo.h"
#include "../ioClassPrice.h"
#include "../EtcHelpFunc.h"
#include "../TextColorTable.h"
#include "../RaceSetting.h"
#include "../EtcHelpFunc.h"
#include "../Setting.h"
#include "../WndID.h"
#include "../ioDateHelp.h"

#include "MySoldierWnd.h"
#include "ioUI3DRender.h"
#include "ioSP2GUIManager.h"
#include "ServiceCharWnd.h"
#include "../TextColorTable.h"

bool ServiceCharWnd::sg_bWndShow = false;
//////////////////////////////////////////////////////////////////////////
ServiceCharWnd::ServiceCharWnd()
{
	m_iClassSelect      = 0;	
	m_iCurRaceDetailIdx = 0;
	m_iClassLimitDate   = 24;
}

ServiceCharWnd::~ServiceCharWnd()
{
	m_dwFirstClassList.clear();
	m_EnableRaceList.clear();
}

void ServiceCharWnd::SetServiceChar( bool bShow, DWORD dwServerDate )
{
	if( !bShow ) return;

	DWORD dwClientDate = Help::GetLastPCRoomBonusDate();
	if( dwClientDate != 0 )
	{		
		int iGapSec = DateHelp::ConvertSecondTime( 2000 + ( dwServerDate / 100000000 ), ( dwServerDate % 100000000 ) / 1000000, ( dwServerDate % 1000000 ) / 10000, ( dwServerDate % 10000 ) / 100, dwServerDate % 100, 0 ) - 
					  DateHelp::ConvertSecondTime( 2000 + ( dwClientDate / 100000000 ), ( dwClientDate % 100000000 ) / 1000000, ( dwClientDate % 1000000 ) / 10000, ( dwClientDate % 10000 ) / 100, dwClientDate % 100, 0 );

		if( ( iGapSec / 60 ) <= 30 ) // 30분 이내에 두번 이상 못 받음         
			return;
	}
	sg_bWndShow = bShow;
}

void ServiceCharWnd::iwm_show()
{
	SetChildInActive( ID_EXIT );
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

	UpdateFirstClass();
	UpdateRaceDetail();
	UpdateCharacter();
	ShowChildWnd( ID_CHARACTER );

	//Help
	ioINILoader_e kLoader( "config/sp2_service_char.ini" );
	kLoader.SetTitle_e( "UI_HELP" );

	char szBuf[MAX_PATH];
	kLoader.LoadString_e( "title", "", szBuf, MAX_PATH );
	m_szHelpTitle = szBuf;

	kLoader.LoadString_e( "desc", "", szBuf, MAX_PATH );
	SplitDescription( ioHashString(szBuf) );

	ioWnd *pFirstHire = FindChildWnd( ID_CHAR_RECV );
	if( pFirstHire )
	{
		kLoader.LoadString_e( "btn_name", "", szBuf, MAX_PATH );
		pFirstHire->SetTitleText( szBuf );
	}

	kLoader.LoadString_e( "wnd_title", "", szBuf, MAX_PATH );
	m_szWndTitle = szBuf;
	SetTitleText( m_szWndTitle.c_str() );

	CheckHideWnd();
}

void ServiceCharWnd::iwm_hide()
{
	HideChildWnd( ID_CHARACTER );
}

void ServiceCharWnd::UpdateFirstClass()
{
	m_dwFirstClassList.clear();

	ioINILoader_e kLoader( "config/sp2_service_char.ini" );
	kLoader.SetTitle_e( "CREATE_INFO" );
	m_iClassLimitDate = kLoader.LoadInt_e( "limit_date", 24 );
	int max_item = kLoader.LoadInt_e( "max_item", 0 );

	char szKeyName[MAX_PATH];
	for(int i = 0;i < max_item;i++)
	{
		sprintf_e( szKeyName, "item_code_%d", i + 1);
		int iClassType = kLoader.LoadInt( szKeyName, 0 );
		DWORD dwSetItemCode = iClassType + SET_ITEM_CODE;
		const ioSetItemInfo *pSetItemInfo = g_SetItemInfoMgr.GetSetInfoByCode( dwSetItemCode );
		if( g_MyInfo.IsCanBuyItem( pSetItemInfo ) )
			m_dwFirstClassList.push_back( iClassType );
	}	
}

void ServiceCharWnd::UpdateRaceDetail()
{
	m_EnableRaceList.clear();
	m_EnableRaceList.push_back( RDT_HUMAN_MAN );
	m_EnableRaceList.push_back( RDT_HUMAN_WOMAN );
	m_EnableRaceList.push_back( RDT_ELF_MAN );
	m_EnableRaceList.push_back( RDT_ELF_WOMAN );
	m_EnableRaceList.push_back( RDT_DWARF_MAN );
	m_EnableRaceList.push_back( RDT_DWARF_WOMAN );
}

void ServiceCharWnd::UpdateCharacter()
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

void ServiceCharWnd::UpdateCharInfo( RaceDetailType eDetail )
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
	m_CharInfo.m_underwear  = RotateNum( m_CharInfo.m_underwear, 1, RaceSetting::GetNumUnderwearParts() - 1 );
}

int ServiceCharWnd::RotateNum( int iCurNum, int iMinNum, int iMaxNum )
{
	if( iMinNum == iMaxNum )
		return iMinNum;
	if( iCurNum < iMinNum )
		return iCurNum + iMaxNum;
	else if( iCurNum > iMaxNum )
		return iCurNum % iMaxNum;

	return iCurNum;
}

void ServiceCharWnd::ChangeClass( bool bAdd )
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

void ServiceCharWnd::ChangeCharacter( bool bAdd )
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

void ServiceCharWnd::SplitDescription( const ioHashString &szDesc )
{
	m_DescList.clear();
	m_DescList = ioStringConverter::Split( szDesc.c_str(), "#" );
}

void ServiceCharWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
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
	case ID_CHAR_RECV:
		if( cmd == IOBN_BTNUP )
		{			
			if( g_MyInfo.GetCharCount() >= g_MyInfo.GetCurMaxCharSlot() )
			{
				sg_bWndShow = false;
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
				HideWnd();
				return ;
			}
			else if( g_MyInfo.GetClassArrayExceptExercise( m_dwFirstClassList[m_iClassSelect] ) != -1 )
			{
				int iArray = g_MyInfo.GetClassArray( m_dwFirstClassList[m_iClassSelect] );
				if( g_MyInfo.IsCharActive( iArray ) )
				{
					g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
					return;
				}
				else
				{
					// 보유중인 용병은 기간 연장
					SP2Packet kPacket( CTPK_SERVICE_CHAR );
					kPacket << SERVICE_CHAR_EXTEND;
					kPacket << g_MyInfo.GetCharIndex( g_MyInfo.GetClassArray( m_dwFirstClassList[m_iClassSelect] ) );
					kPacket << m_iClassLimitDate;		
					TCPNetwork::SendToServer( kPacket );
				}
			}
			else
			{
				SP2Packet kPacket( CTPK_SERVICE_CHAR );
				kPacket << SERVICE_CHAR_CREATE;
				kPacket << m_dwFirstClassList[m_iClassSelect];
				kPacket << m_CharInfo.m_kindred
						<< m_CharInfo.m_sex
						<< m_iClassLimitDate;
				TCPNetwork::SendToServer( kPacket );
			}			
			g_App.SetMouseBusy( true );

			HideWnd();
			sg_bWndShow = false;
		}
		break;
	}
}

void ServiceCharWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() )
	{
		if( sg_bWndShow )
		{
			if( !g_GUIMgr.IsShow( TOTAL_RESULT_MAIN_WND ) &&
				!g_GUIMgr.IsShow( BRIEFING_WND ) )
				ShowWnd();
		}
		return;
	}	

	ioMovingWnd::OnProcess( fTimePerSec );	
}

void ServiceCharWnd::OnRender()
{
	ioWnd::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	g_FontMgr.SetAlignType( TAT_LEFT );
	g_FontMgr.SetTextStyle( TS_OUTLINE_FULL_2X );
	g_FontMgr.SetBkColor( 0, 0, 0 );
	g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
	g_FontMgr.PrintText( iXPos + 212, iYPos + 60, FONT_SIZE_13, m_szHelpTitle.c_str() );
	
	int iItemInfoSize = m_DescList.size();
	for( int i=0; i<iItemInfoSize;i++ )
	{
		if( !m_DescList[i].empty() )
		{
			g_FontMgr.PrintText( iXPos + 191, iYPos + 88 + (i * 20), FONT_SIZE_13, m_DescList[i].c_str() );
		}
	}

	g_FontMgr.PrintText( iXPos + 212, iYPos + 182, FONT_SIZE_13, STR(1) );

	g_FontMgr.SetTextColor( 255, 113, 0 );
	g_FontMgr.PrintText( iXPos + 191, iYPos + 210, FONT_SIZE_13, STR(2) );
	g_FontMgr.PrintText( iXPos + 224, iYPos + 210, FONT_SIZE_13, ":" );
	g_FontMgr.PrintText( iXPos + 286, iYPos + 210, FONT_SIZE_13, m_szClassName.c_str() );	
}

void ServiceCharWnd::CheckHideWnd()
{
	if( m_dwFirstClassList.empty() )
	{
		HideWnd();
		sg_bWndShow = false;
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
	sg_bWndShow = false;
}










