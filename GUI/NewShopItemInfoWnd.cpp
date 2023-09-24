#include "StdAfx.h"

#include "../ioDateHelp.h"

#include "NewShopWnd.h"
#include "NewShopItemInfoWnd.h"
#include "NewShopIconBtn.h"

#include "../ioMyInfo.h"
#include "../ioEtcItem.h"

NewShopItemInfoWnd::NewShopItemInfoWnd()
{
	m_pSelectFrm	= NULL;
	m_pEventFrm		= NULL;
	m_pLeft			= NULL;

	m_bSelect		= false;
	m_bDecoEquip	= false;

	m_eLimitRender	= LR_NONE;
	m_nGradeType = 0;
}

NewShopItemInfoWnd::~NewShopItemInfoWnd()
{
	ClearDesc();

	SAFEDELETE( m_pSelectFrm );
	SAFEDELETE( m_pEventFrm );
	SAFEDELETE( m_pLeft );
}

void NewShopItemInfoWnd::AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage )
{
	if( szType == "Left" )
	{
		SAFEDELETE( m_pLeft );
		m_pLeft = pImage;
	}
	else
	{
		ioButton::AddRenderImage( szType, pImage );	
	}	
}

void NewShopItemInfoWnd::AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame )
{
	if( szType == "SelectFrm" )
	{
		SAFEDELETE( m_pSelectFrm );
		m_pSelectFrm = pFrame;
	}
	else if( szType == "EventFrm" )
	{
		SAFEDELETE( m_pEventFrm );
		m_pEventFrm = pFrame;
	}
	else
	{
		ioButton::AddRenderFrame( szType, pFrame );
	}
}

void NewShopItemInfoWnd::iwm_command( ioWnd *pWnd, int cmd, DWORD param )
{
	DWORD dwID = pWnd->GetID();

	switch( dwID )
	{
	case ID_ICON_BTN:
	case ID_DETAIL_BTN:
		if( cmd == IOBN_BTNUP ||
			cmd == IOWN_OVERED||
			cmd == IOWN_LEAVED )
		{
			if( GetParent() )
				GetParent()->iwm_command( this, cmd, dwID );
		}
		break;
	}

}

void NewShopItemInfoWnd::SetBlank()
{
	SetInActive();

	SetChildInActive( ID_ICON_BTN );
	HideChildWnd( ID_DETAIL_BTN );

	ClearDesc();

	m_szDescArray[0].SetTextStyle( TS_NORMAL );
	m_szDescArray[0].SetBkColor( 0, 0, 0 );	
	m_szDescArray[0].SetTextColor( TCT_DEFAULT_LIGHTGRAY ); 
	m_szDescArray[0].AddTextPiece( FONT_SIZE_13, STR(1) );

	m_szDescArray[1].SetTextStyle( TS_NORMAL );
	m_szDescArray[1].SetBkColor( 0, 0, 0);	
	m_szDescArray[1].SetTextColor( TCT_DEFAULT_LIGHTGRAY ); 
	m_szDescArray[1].AddTextPiece( FONT_SIZE_13, "-" );

	m_eLimitRender = LR_DEFULT_LIMIT;
	m_nGradeType = 0;
}

void NewShopItemInfoWnd::ClearDesc()
{
	for (int i = 0; i < MAX_DESC ; i++)
	{
		m_szDescArray[i].ClearList();
	}
}

void NewShopItemInfoWnd::OnRender()
{
	ioButton::OnRender();

	int iXPos = GetDerivedPosX();
	int iYPos = GetDerivedPosY();

	RenderDesc( iXPos, iYPos );

	if( m_bSelect )
	{
		// xml���� offset ���� �����ϹǷ� ���⼭�� �������� ����.
		if( m_pSelectFrm )
			m_pSelectFrm->Render( iXPos, iYPos );
	}
}

void NewShopItemInfoWnd::RenderDesc( int iXPos, int iYPos )
{
	enum 
	{
		X_OFFSET           = 52,
		Y_OFFSET           = 105,
		Y_LINE_GAP         = 16,
		Y_OFFSET_SECOND    = -8,
	};


	if( Setting::ShowExtraInfo() )
	{
		g_FontMgr.SetAlignType( TAT_LEFT );
		g_FontMgr.SetTextStyle( TS_NORMAL );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextColor( TCT_DEFAULT_RED );	
		g_FontMgr.PrintText( iXPos, iYPos - 7, FONT_SIZE_12, STR(1), m_ItemInfo.m_iSortPoint );

		g_FontMgr.PrintText( iXPos, iYPos - 32, FONT_SIZE_12, "%1 %2 %3 %4", m_ItemInfo.m_iItemType,
			m_ItemInfo.m_iValue0, m_ItemInfo.m_iValue1, m_ItemInfo.m_iValue2 );
	}

	iXPos += X_OFFSET;
	iYPos += Y_OFFSET;

	int iDescCnt = 0;
	for( int i = 0 ; i < MAX_DESC ; i++ )
	{
		if( m_szDescArray[i].IsEmpty() )
			continue;
		iDescCnt++;
	}

	if( iDescCnt == 3 )
		iYPos += Y_OFFSET_SECOND;

	int iCnt  = 0;
	for( int i = 0 ; i < MAX_DESC ; i++ )
	{
		if( m_szDescArray[i].IsEmpty() )
			continue;
		m_szDescArray[i].PrintFullTextWidthCut( iXPos, iYPos + ( Y_LINE_GAP * iCnt ), TAT_CENTER, NEW_SHOP_WIDTHCUT_SIZE );
		iCnt++;
	}
}

void NewShopItemInfoWnd::SetInfo( NewShopItemInfo &rkInfo )
{
	m_ItemInfo = rkInfo;
	m_nGradeType = 0;

	if( rkInfo.m_sName.IsEmpty() || rkInfo.m_sIcon.IsEmpty() )
	{
		SetBlank();
		return;
	}

	SetActive();

	// etc btn
	SetChildActive( ID_ICON_BTN );
	ShowChildWnd( ID_DETAIL_BTN );

	// 
	// �������ݵ���Ʈ
	bool bTextPeso = false;
	if( ( rkInfo.m_bShowPeso && rkInfo.GetPeso( 0 ) > 0 )       ||
		( rkInfo.GetPeso( 0 ) > 0 && rkInfo.GetCash( 0 ) <= 0 ) ||
		( rkInfo.m_iNeedLevel == 0 && rkInfo.GetPeso( 0 ) > 0 && rkInfo.GetCash( 0 ) > 0 ) )
		bTextPeso = true;

	// �̺�Ʈ ��ǰ�̴�
	bool bTextStamp = false;  
	if( rkInfo.GetStamp( 0 ) > 0 )
		bTextStamp = true;

	// �̺�Ʈ ��ǰ ��� ��ȭ��
	int iReinforceNumber = 0;

	m_eLimitRender = LR_NONE;
	if( rkInfo.m_iItemType == PRESENT_DEFAULT )
	{
		// ����� ���
		if( rkInfo.m_iValue0 == PRESENT_EXTRAITEM )
		{
			// ��ȭ��
			iReinforceNumber = ( rkInfo.m_iValue2 % PRESENT_EXTRAITEM_DIVISION_1 ) / PRESENT_EXTRAITEM_DIVISION_2;

			// ���� ���
			if( rkInfo.m_iValue2 / PRESENT_EXTRAITEM_DIVISION_1 != 0 )
				rkInfo.m_iShopMarkType = SHOP_MARK_TYPE_EXTRAITEM_GROWTH_CATALYST;
		}

		// ���� ��ǰ�� ���� �ȷ����� Sold Out�� ��ü
		if( rkInfo.m_iShopMarkType == SHOP_MARK_TYPE_LIMIT_GOODS || rkInfo.m_iShopMarkType == SHOP_MARK_TYPE_LIMIT_DAYS )
		{
			if( rkInfo.GetValue3( 1 ) == 0 )
				rkInfo.m_iShopMarkType = SHOP_MARK_TYPE_SOLD_OUT;

			m_eLimitRender = LR_DEFULT_LIMIT;
		}
	}
	else if( rkInfo.m_iItemType == PRESENT_ETC_ITEM )
	{
		// ���� ��ǰ�� ���� �ȷ����� Sold Out�� ��ü
		if( rkInfo.m_iShopMarkType == SHOP_MARK_TYPE_LIMIT_GOODS || rkInfo.m_iShopMarkType == SHOP_MARK_TYPE_LIMIT_DAYS || rkInfo.m_iShopMarkType == SHOP_MARK_TYPE_SOLD_OUT )
		{
			if( rkInfo.m_iLimit == 0 )
				rkInfo.m_iShopMarkType = SHOP_MARK_TYPE_SOLD_OUT;

			m_eLimitRender = LR_ETC_LIMIT;
		}
	}

	// desc
	ClearDesc();

	m_szDescArray[0].SetTextStyle( TS_NORMAL );
	m_szDescArray[0].SetBkColor( 0, 0, 0 );	
	m_szDescArray[0].SetTextColor( TCT_DEFAULT_DARKGRAY ); // ��ȸ��
	m_szDescArray[0].AddTextPiece( FONT_SIZE_13, rkInfo.m_sName.c_str() );

	m_szDescArray[1].SetTextStyle( TS_NORMAL );
	m_szDescArray[1].SetBkColor( 0, 0, 0);	
	m_szDescArray[1].SetTextColor( TCT_DEFAULT_GRAY ); // ��ȸ��


	bool bAscendingOrder = false; // ���� ĳ������ ���� ���� �����ϸ� bAscendingOrder �˻��ؼ� ��� �������ٰ�

	if( rkInfo.m_iItemType == PRESENT_ETC_ITEM ||
		rkInfo.m_iItemType == PRESENT_EXTRAITEM_BOX ||
		rkInfo.m_iItemType == PRESENT_DECORATION ||
		rkInfo.m_iItemType == PRESENT_COSTUME_BOX )
		bAscendingOrder = true;

	// Ȯ����� ������ ���� ǥ��
	if( rkInfo.m_iItemType == PRESENT_ETC_ITEM && rkInfo.m_iValue1 == ioEtcItem::EIT_ETC_WHOLE_CHAT )
		bAscendingOrder = false;

	// ����ġ+200 ����
	if( rkInfo.m_iItemType == PRESENT_ETC_ITEM && rkInfo.m_iValue1 == ioEtcItem::EIT_ETC_EXP_BONUS && rkInfo.GetPeso( 2 ) > 0 )
	{
		bTextPeso = true;
	}

	// ���+100 ����
	if( rkInfo.m_iItemType == PRESENT_ETC_ITEM && rkInfo.m_iValue1 == ioEtcItem::EIT_ETC_PESO_BONUS && rkInfo.GetPeso( 2 ) > 0 )
	{
		bTextPeso = true;
	}

	// �뺴�����̺�Ʈ�̸� ���� ������ ĳ�� ����
	if( rkInfo.m_iItemType == PRESENT_SOLDIER && rkInfo.m_iShopMarkType == SHOP_MARK_TYPE_EVENT )
	{
		bAscendingOrder = false;
		bTextPeso       = false;
	}

	// ���� ǥ�� ����Ʈ
	if( rkInfo.m_iItemType == PRESENT_DEFAULT )
	{
		m_szDescArray[1].AddTextPiece( FONT_SIZE_11, g_PresentMgr.GetPresentValue2Text( rkInfo.m_iValue0, rkInfo.m_iValue1, rkInfo.m_iValue2 ).c_str() );
	}
	else
	{
		int iBonusPesoArray = 0;
		if( !bAscendingOrder )
			iBonusPesoArray = rkInfo.m_vBonusPeso.size()-1;

		if( rkInfo.GetBonusPeso( iBonusPesoArray ) <= 0 )
		{
			if( !bAscendingOrder )
				iBonusPesoArray--;
			else
				iBonusPesoArray++;
		}
		
		if( rkInfo.m_iNeedLevel > 0 )
		{
			char szText[MAX_PATH]="";
			if( rkInfo.m_iItemType == PRESENT_DECORATION )
				SafeSprintf( szText, sizeof( szText ),  STR(1), rkInfo.m_iNeedLevel );
			else
			{
				if( rkInfo.GetPeso( 0 ) != 0 )
				{
					g_LevelMgr.GetGradeName( rkInfo.m_iNeedLevel, szText, sizeof(szText), true );
					StringCbCat( szText, sizeof( szText ), STR(2) );
				}
				else
					StringCbCopy( szText, sizeof( szText ), STR(3) );
			}
			m_szDescArray[1].AddTextPiece( FONT_SIZE_11, szText );
		}
		else if( rkInfo.GetBonusPeso( iBonusPesoArray ) > 0 && !bTextPeso )
		{
			char szComma[MAX_PATH]="";
			Help::ConvertNumToStrComma( rkInfo.GetBonusPeso( iBonusPesoArray ), szComma, sizeof( szComma ) );
			char szText[MAX_PATH]="";
			SafeSprintf( szText, sizeof( szText ), STR(4), szComma );
			m_szDescArray[1].AddTextPiece( FONT_SIZE_11, szText );
		}
	}

	if( bTextPeso )
	{
		// �������ݵ���Ʈ
		int iArray = 0;

		// ����ġ+200 ����
		if( rkInfo.m_iItemType == PRESENT_ETC_ITEM && rkInfo.m_iValue1 == ioEtcItem::EIT_ETC_EXP_BONUS && rkInfo.GetPeso( 2 ) > 0 )
		{
			iArray = 2;
		}

		// ���+100 ����
		if( rkInfo.m_iItemType == PRESENT_ETC_ITEM && rkInfo.m_iValue1 == ioEtcItem::EIT_ETC_PESO_BONUS && rkInfo.GetPeso( 2 ) > 0 )
		{
			iArray = 2;
		}

		char szComma[MAX_PATH]="";
		Help::ConvertNumToStrComma( rkInfo.GetPeso( iArray ), szComma, sizeof( szComma ) );
		m_szDescArray[2].SetTextStyle( TS_NORMAL );
		m_szDescArray[2].SetBkColor( 0, 0, 0 );	
		m_szDescArray[2].SetTextColor( TCT_DEFAULT_BLUE ); // �Ķ���
		m_szDescArray[2].AddTextPiece( FONT_SIZE_13, STR(5), szComma );
	}
	else if( bTextStamp )
	{
		// �������ݵ���Ʈ
		int iArray = 0;
		char szComma[MAX_PATH]="";
		Help::ConvertNumToStrComma( rkInfo.GetStamp( iArray ), szComma, sizeof( szComma ) );
		m_szDescArray[2].SetTextStyle( TS_NORMAL );
		m_szDescArray[2].SetBkColor( 0, 0, 0 );	
		m_szDescArray[2].SetTextColor( TCT_DEFAULT_BLUE ); // �Ķ���

		ioEtcItem *pEtcItem = g_EtcItemMgr.FindEtcItem( rkInfo.m_dwStampEtcItem );
		if( pEtcItem )
			m_szDescArray[2].AddTextPiece( FONT_SIZE_13, STR(8), pEtcItem->GetName().c_str(), szComma );
		else
			m_szDescArray[2].AddTextPiece( FONT_SIZE_13, STR(7), szComma );
	}
	else
	{
		// �������ݵ���Ʈ
		int iArray = 0;
		if( !bAscendingOrder )
			iArray = rkInfo.m_vCash.size()-1;

		if( rkInfo.GetCash( iArray ) <= 0 )
		{
			if( !bAscendingOrder )
				iArray--;
			else
				iArray++;
		}
		char szComma[MAX_PATH]="";
		Help::ConvertNumToStrComma( rkInfo.GetCash( iArray ) , szComma, sizeof( szComma ) );
		m_szDescArray[2].SetTextStyle( TS_NORMAL );
		m_szDescArray[2].SetBkColor( 0, 0, 0 );	
		m_szDescArray[2].SetTextColor( 141,87,4 ); // ����
		m_szDescArray[2].AddTextPiece( FONT_SIZE_13, STR(6), szComma );
	}


	// icon btn
	NewShopIconBtn *pBtn = dynamic_cast< NewShopIconBtn* > ( FindChildWnd( ID_ICON_BTN ) );
	if( !pBtn )
	{
		LOG.PrintTimeAndLog( 0, "%s pBtn == NULL", __FUNCTION__ );
		SetBlank();
		return;
	}
	pBtn->SetNumber( iReinforceNumber );

	if( rkInfo.m_iShopMarkType == SHOP_MARK_TYPE_DATE && 0 < rkInfo.m_iLimitDate )
	{
		CTime kLimitTime;
		CTime kCurrTime = DateHelp::GetCurrTime();
		DateHelp::ConvertCTime( rkInfo.m_iLimitDate, kLimitTime );
		CTimeSpan TimeGap = kLimitTime - kCurrTime;
		if( 0 < TimeGap.GetDays() )
			pBtn->SetLimitDay( TimeGap.GetDays() );
		else
			pBtn->SetLimitDay( 0 );
	}

	ioUIRenderImage *pIcon = g_UIImageSetMgr.CreateImageByFullName( rkInfo.m_sIcon );
	if( !pBtn->SetIcon( pIcon ) )
	{
		SetBlank();
		return;
	}

	pBtn->SetMarkType( rkInfo.m_iShopMarkType );

	if ( rkInfo.m_iItemType == PRESENT_COSTUME || rkInfo.m_iItemType == PRESENT_COSTUME_BOX )
		pBtn->SetTopSubIconType( NewShopIconBtn::TSIT_COSTUME );

	if ( rkInfo.m_iItemType == PRESENT_DEFAULT && ( rkInfo.m_iValue0 == PRESENT_COSTUME || rkInfo.m_iValue0 == PRESENT_COSTUME_BOX ) )
		pBtn->SetTopSubIconType( NewShopIconBtn::TSIT_COSTUME );
	
	if( rkInfo.m_iItemType == PRESENT_ETC_ITEM && COMPARE(  rkInfo.m_iValue1, ioEtcItem::EIT_ETC_GUILD_HOUSING_BLOCK_0001, ioEtcItem::EIT_ETC_GUILD_HOUSING_BLOCK_1000+1 ) )
		pBtn->SetTopSubIconType( NewShopIconBtn::TSIT_GUILD );

	if( rkInfo.m_iItemType == PRESENT_ETC_ITEM && COMPARE(  rkInfo.m_iValue1, ioEtcItem::EIT_ETC_HOME_HOUSING_BLOCK_0001, ioEtcItem::EIT_ETC_HOME_HOUSING_BLOCK_1000+1 ) )
		pBtn->SetTopSubIconType( NewShopIconBtn::TSIT_HOME );

	switch( rkInfo.m_iItemType )
	{
	case PRESENT_SOLDIER:
		m_nGradeType = g_UIImageRenderMgr.GetPowerUpGradeType( rkInfo.m_iValue1 );
		break;
	case PRESENT_EXTRAITEM:
	case PRESENT_ACCESSORY:
		{
			const ioItem *pItem = g_ItemMaker.GetItemConst( rkInfo.m_iValue1, __FUNCTION__ );
			if( pItem )
				m_nGradeType = pItem->GetGradeType();
		}
		break;
	case PRESENT_EXTRAITEM_BOX:
		{
			RandomMachineInfo *pInfo = g_ExtraItemInfoMgr.GetRandomMachineInfo( rkInfo.m_iValue1 );
			if( pInfo )
				m_nGradeType = pInfo->GetGradeType();
		}
		break;
	case PRESENT_COSTUME:
		{
			m_nGradeType = g_CostumeInfoMgr.GetGradeType( rkInfo.m_iValue1 );
		}
		break;
	case PRESENT_COSTUME_BOX:
		{
			CostumeShopInfo *pInfo = g_CostumeShopInfoMgr.GetCostumeMachineInfo( rkInfo.m_iValue1 );
			if( pInfo )
				m_nGradeType = pInfo->GetGradeType();
		}
		break;
	case PRESENT_ETC_ITEM:
		{
			ioEtcItem *pEtcItem = g_EtcItemMgr.FindEtcItem( rkInfo.m_iValue1 );
			if( pEtcItem ) 
				m_nGradeType = pEtcItem->GetGradeType();
		}
		break;
	}

	pBtn->SetGradeType( m_nGradeType );
}

void NewShopItemInfoWnd::OnProcess( float fTimePerSec )
{
	if( !IsShow() ) return;
}

void NewShopItemInfoWnd::OnDrawNormal( int iXPos, int iYPos )
{
	if( m_ItemInfo.m_iShopMarkType == SHOP_MARK_TYPE_EVENT )
	{
		if( m_pEventFrm )
			m_pEventFrm->Render( iXPos, iYPos );

		RenderImageList( iXPos, iYPos, UI_RENDER_NORMAL );
		RenderBottom( iXPos, iYPos );
	}
	else
	{
		ioButton::OnDrawNormal( iXPos, iYPos );
		RenderBottom( iXPos, iYPos );
	}
}

void NewShopItemInfoWnd::OnDrawOvered( int iXPos, int iYPos )
{
	if( m_ItemInfo.m_iShopMarkType == SHOP_MARK_TYPE_EVENT )
	{
		if( m_pEventFrm )
			m_pEventFrm->Render( iXPos, iYPos );

		RenderImageList( iXPos, iYPos, UI_RENDER_NORMAL );
		RenderBottom( iXPos, iYPos );
	}
	else
	{
		ioButton::OnDrawOvered( iXPos, iYPos );
		RenderBottom( iXPos, iYPos );
	}
}

void NewShopItemInfoWnd::OnDrawPushed( int iXPos, int iYPos )
{
	if( m_ItemInfo.m_iShopMarkType == SHOP_MARK_TYPE_EVENT )
	{
		if( m_pEventFrm )
			m_pEventFrm->Render( iXPos, iYPos );

		RenderImageList( iXPos, iYPos, UI_RENDER_NORMAL );
		RenderBottom( iXPos, iYPos );
	}
	else
	{
		ioButton::OnDrawPushed( iXPos, iYPos );
		RenderBottom( iXPos, iYPos );
	}
}

void NewShopItemInfoWnd::RenderBottom( int iXPos, int iYPos )
{
	enum 
	{
		TEXT_X_OFFSET = 52,
		TEXT_Y_OFFSET = 157,

		LEFT_MARK_X_OFFSET = 71, 
	};

	// xml���� ��ġ ����
	if( m_pMultiplyFrm )
		m_pMultiplyFrm->Render( iXPos, iYPos, UI_RENDER_MULTIPLY );
	
	if( m_eLimitRender == LR_DEFULT_LIMIT || m_eLimitRender == LR_ETC_LIMIT )
	{
		g_FontMgr.SetAlignType( TAT_CENTER );
		g_FontMgr.SetTextStyle( TS_NORMAL );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );

		int iValue3 = 0;
		if( m_eLimitRender == LR_ETC_LIMIT )
			iValue3 = m_ItemInfo.m_iLimit;
		else
			iValue3 = m_ItemInfo.GetValue3(1);

		if( iValue3 != EVENT_GOODS_INFINITY_COUNT )
		{
			g_FontMgr.PrintText( iXPos + (GetWidth() / 2), iYPos + TEXT_Y_OFFSET, FONT_SIZE_12, STR(2), iValue3 );
		}
		else
		{
			g_FontMgr.PrintText( iXPos + (GetWidth() / 2), iYPos + TEXT_Y_OFFSET, FONT_SIZE_12, STR(3) );
		}
	}
	else
	{
		if( !m_bDecoEquip )
		{
			// xml���� ��ġ ����
			if( m_pLeft )
				m_pLeft->Render( iXPos, iYPos );
		}
		else
		{
			// xml���� ��ġ ����
			if( m_pLeft )
				m_pLeft->Render( iXPos + LEFT_MARK_X_OFFSET, iYPos );
		}

		g_FontMgr.SetAlignType( TAT_CENTER );
		g_FontMgr.SetTextStyle( TS_NORMAL );
		g_FontMgr.SetBkColor( 0, 0, 0 );
		g_FontMgr.SetTextColor( TCT_DEFAULT_GRAY );
		if( !m_bDecoEquip )
			g_FontMgr.PrintText( iXPos + TEXT_X_OFFSET, iYPos + TEXT_Y_OFFSET, FONT_SIZE_12, STR(1) );
		else
			g_FontMgr.PrintText( iXPos + TEXT_X_OFFSET, iYPos + TEXT_Y_OFFSET, FONT_SIZE_12, STR(4) );
	}
}