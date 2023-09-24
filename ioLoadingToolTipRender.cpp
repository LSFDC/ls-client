#include "StdAfx.h"
#include "ioLoadingToolTipRender.h"

bool TipStatgeAnalysis( ioINILoader_e& kTipLoader, ioComplexStringPrinter& rTipPrinter, IORandom& rRandom )
{
	static const int MAX_TIP_STRING_NUM = 20; // ��Ÿ���� �ִ� ���� �ĺ� �ִ� ����
	static const int MAX_TIP_SECTION_NUM = 20; // ���� ���� �����Ҽ� �ִ� ���� �ִ� ����

	unsigned int nSelectIndex = -1;

	// � String�� ����� ������ �����ϴ� ��
	{
		int nRandMax = -1;
		char szRandKeyName[ MAX_PATH ] = "";
		for ( int i = 0; i < MAX_TIP_STRING_NUM; ++i )
		{
			wsprintf_e( szRandKeyName, "%dtitle_rand", i + 1 );
			int nRandTitle = kTipLoader.LoadInt( szRandKeyName, -1 );
			if ( nRandTitle < 0 ) {
				break;
			}
			nRandMax += nRandTitle;
		}
		if ( nRandMax < 0 ) {
			return false;
		}

		unsigned int nRand = rRandom.Random( 0, nRandMax );
		unsigned int nRandAcc = 0;
		for ( int i = 0; i < MAX_TIP_STRING_NUM; ++i )
		{
			wsprintf_e( szRandKeyName, "%dtitle_rand", i + 1 );
			int nRandTitle = kTipLoader.LoadInt( szRandKeyName, -1 );
			nRandAcc += nRandTitle;
			if ( nRandTitle < 0 ) {
				break;
			}
			if ( nRandTitle == 0 ) {
				continue;
			}
			if ( nRandAcc >= nRand ) 
			{
				nSelectIndex = i + 1;
				break;
			}
		}
		if ( nSelectIndex < 0 ) {
			return false;
		}
	}

	// �ؽ�Ʈ�� ������ �����Ͽ� �����ϴ� ��
	{
		char szColorKeyName[ MAX_PATH ] = "";
		char szColorValue[ MAX_PATH ]= "";
		char szTextKeyName[ MAX_PATH ] = "";
		char szTextValue[ MAX_PATH ]= "";

		for ( int i = 0; i < MAX_TIP_SECTION_NUM; ++i )
		{
			wsprintf_e( szColorKeyName, "%dtitle_color%d", nSelectIndex, i + 1 );
			wsprintf_e( szTextKeyName, "%dtitle_text%d", nSelectIndex, i + 1 );
			kTipLoader.LoadString( szColorKeyName, "", szColorValue, MAX_PATH );
			kTipLoader.LoadString( szTextKeyName, "", szTextValue, MAX_PATH );
			if ( strcmp( szColorValue, "" ) == 0 || strcmp( szTextValue, "" ) == 0 ) {
				break;
			}

			char* pTemp = NULL;
			DWORD dwColor = strtoul( szColorValue, &pTemp, 16 );
			Help::ChangeCharSet( szTextValue, '#', ' ' );

			rTipPrinter.SetTextColor( dwColor );
			rTipPrinter.AddTextPieceWithoutXOffset( FONT_SIZE_15, szTextValue );
		}
	}

	return !rTipPrinter.IsEmpty();
}

bool TipAnalysis( const std::string& stagename, ioComplexStringPrinter& rTipPrinter, IORandom& rRandom )
{
	ioINILoader_e kTipLoader( "config/sp2_mode_loading_tip.ini" );

	// section�� �м� �Ͽ� � ������ ������ ����
	unsigned int selectsection = -1;
	{
		kTipLoader.SetTitle( "section" );
		int sectionnum = kTipLoader.LoadInt( "num", 0 );
		if ( sectionnum <= 0 ) {
			return false;
		}

		int lv = g_MyInfo.GetGradeLevel();
		char sectionkeyname[ MAX_PATH ] = "";
		char sectionkeyvalue[ MAX_PATH ] = "";
		std::string sectionparser;
		for ( int i = 0; i < sectionnum; ++i )
		{
			wsprintf_e( sectionkeyname, "section%d", i + 1 );
			kTipLoader.LoadString( sectionkeyname, "", sectionkeyvalue, MAX_PATH );
			if ( strcmp( sectionkeyvalue, "" ) == 0 ) {
				continue;
			}

			sectionparser = sectionkeyvalue;
			size_t fpos = sectionparser.find( "~" );
			if ( fpos >= sectionparser.size() ) {
				continue;
			}

			int lvbegin = atoi( sectionparser.substr( 0, fpos ).c_str() );
			int lvend = std::max<int>( lvbegin, atoi( sectionparser.substr( fpos + 1, sectionparser.size() - ( fpos + 1 ) ).c_str() ) );
			if ( lvbegin <= lv && lvend >= lv ) 
			{
				selectsection = i + 1;
				break;
			}
		}
		if ( selectsection < 0 ) {
			return false;
		}
	}

	// common section�� ���� ������ statge section�� ���� ������ ����
	char sectioncrkeyname[ MAX_PATH ] = "";
	{		
		wsprintf_e( sectioncrkeyname, "section%d_common_rand", selectsection );
		int commonrand = kTipLoader.LoadInt( sectioncrkeyname, -1 );
		if ( commonrand > 0 && commonrand >= static_cast<int>( rRandom.Random( 1, FLOAT100 ) ) ) {
			wsprintf_e( sectioncrkeyname, "common_section%d", selectsection );
		} else {
			wsprintf_e( sectioncrkeyname, "%s_section%d", stagename.c_str(), selectsection );			
		}
		kTipLoader.SetTitle( sectioncrkeyname );
	}

	if ( !TipStatgeAnalysis( kTipLoader, rTipPrinter, rRandom ) ) 
	{
		wsprintf_e( sectioncrkeyname, "common_section%d", selectsection );
		kTipLoader.SetTitle( sectioncrkeyname );
		if ( !TipStatgeAnalysis( kTipLoader, rTipPrinter, rRandom ) ) {
			return false;
		}
	}

	return true;
}