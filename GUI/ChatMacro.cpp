#include "StdAfx.h"

#include "../io3DEngine/ioWnd.h"
#include "../io3DEngine/ioEdit.h"
#include "../io3DEngine/ioStringManager.h"

#include "ChatMacro.h"
#include "../Local/ioLocalManager.h"
#include <strsafe.h>

char ChatMacro::m_szMacro[ChatMacro::MAX_MACRO][MAX_PATH];

ChatMacro::ChatMacro()
{
}

ChatMacro::~ChatMacro()
{
}

int ChatMacro::IsMacro(char *szChat,int &key_word)
{
	if(szChat[0] != '/')
		return NONE_MACRO;
	
	char strCommand[MAX_EDIT_TEXT+1] = "";
	
	key_word = 0;
	while(*szChat != NULL)
	{
		strCommand[key_word++] = *szChat++;
		if( *szChat == ' ' )
			break;
	}

	szChat++;      //������ �Ѱ��ش�.
	key_word++;
	
	int command_type = 0;
	for(;command_type < MAX_MACRO ;command_type++)
	{
		if(stricmp(strCommand, m_szMacro[command_type]) == 0)
			break;
	}
	
	if(!COMPARE(command_type,MACRO_HELP,MAX_MACRO)) 
		return NONE_MACRO;

	return command_type;
}

char *ChatMacro::GetMacroText( int iMacroType )
{
	if( COMPARE( iMacroType, 0, MAX_MACRO) )
		return m_szMacro[iMacroType];

	return "/UNKNOWN";
}

void ChatMacro::SetMacroText()
{	
	ZeroMemory( m_szMacro, sizeof( m_szMacro ) );

	// ��Ʈ�� ���⶧���� loop ��� ����.
	StringCbCopy( m_szMacro[MACRO_HELP]							,sizeof( m_szMacro[MACRO_HELP] )						, STR(1) );
	StringCbCopy( m_szMacro[MACRO_QUEST_GUIDE]					,sizeof( m_szMacro[MACRO_QUEST_GUIDE] )					, "/1" );
	StringCbCopy( m_szMacro[MACRO_QUEST_OCCUR]					,sizeof( m_szMacro[MACRO_QUEST_OCCUR] )					, "/2" );
	StringCbCopy( m_szMacro[MACRO_QUEST_ATTAIN]					,sizeof( m_szMacro[MACRO_QUEST_ATTAIN] )				, "/3" );
	StringCbCopy( m_szMacro[MACRO_QUEST_DELETE]					,sizeof( m_szMacro[MACRO_QUEST_DELETE] )				, "/4" );
	StringCbCopy( m_szMacro[MACRO_PRESENT_DELETE]				,sizeof( m_szMacro[MACRO_PRESENT_DELETE] )				, "/5" );
	StringCbCopy( m_szMacro[MACRO_QUEST_TEXT]					,sizeof( m_szMacro[MACRO_QUEST_TEXT] )					, "/6" );
	StringCbCopy( m_szMacro[MACRO_EXIT_LADDER]					,sizeof( m_szMacro[MACRO_EXIT_LADDER] )					, STR(6) );
	StringCbCopy( m_szMacro[MACRO_CHANNEL_INFO]					,sizeof( m_szMacro[MACRO_CHANNEL_INFO] )				, STR(8) );
	StringCbCopy( m_szMacro[MACRO_CONTRIBUTE]					,sizeof( m_szMacro[MACRO_CONTRIBUTE] )					, STR(9) );
	StringCbCopy( m_szMacro[MACRO_MEMO]							,sizeof( m_szMacro[MACRO_MEMO] )						, STR(10) );
	StringCbCopy( m_szMacro[MACRO_INVITE_CHANNEL]				,sizeof( m_szMacro[MACRO_INVITE_CHANNEL] )				, STR(11) );
	StringCbCopy( m_szMacro[MACRO_ENTER_PLAZA]					,sizeof( m_szMacro[MACRO_ENTER_PLAZA] )					, STR(12) );
	StringCbCopy( m_szMacro[MACRO_NONE_UI]						,sizeof( m_szMacro[MACRO_NONE_UI] )						, STR(13) );
	StringCbCopy( m_szMacro[MACRO_USER_INFO]					,sizeof( m_szMacro[MACRO_USER_INFO] )					, STR(14) );
	StringCbCopy( m_szMacro[MACRO_CHAT_MODE]					,sizeof( m_szMacro[MACRO_CHAT_MODE] )					, STR(15) );
	StringCbCopy( m_szMacro[MACRO_INIT_UI_POS]					,sizeof( m_szMacro[MACRO_INIT_UI_POS] )					, STR(16) );
	StringCbCopy( m_szMacro[MACRO_AWARD]						,sizeof( m_szMacro[MACRO_AWARD] )						, STR(17) );
	StringCbCopy( m_szMacro[MACRO_INFO]							,sizeof( m_szMacro[MACRO_INFO] )						, STR(18) );
	StringCbCopy( m_szMacro[MACRO_ROOM_ENTER]					,sizeof( m_szMacro[MACRO_ROOM_ENTER] )					, STR(19) );
	StringCbCopy( m_szMacro[MACRO_UDP]							,sizeof( m_szMacro[MACRO_UDP] )							, STR(20) );
	StringCbCopy( m_szMacro[MACRO_MOVE_PACKET]					,sizeof( m_szMacro[MACRO_MOVE_PACKET] )					, STR(21) );
	StringCbCopy( m_szMacro[MACRO_GUILD_INFO]					,sizeof( m_szMacro[MACRO_GUILD_INFO] )					, STR(22) );
	StringCbCopy( m_szMacro[MACRO_TIME]							,sizeof( m_szMacro[MACRO_TIME] )						, STR(23) );
	StringCbCopy( m_szMacro[MACRO_SHOW_ANNOUNCEMENT]			,sizeof( m_szMacro[MACRO_SHOW_ANNOUNCEMENT] )			, STR(24) );
	StringCbCopy( m_szMacro[MACRO_BLOCK_GULID_MARK]				,sizeof( m_szMacro[MACRO_BLOCK_GULID_MARK] )			, STR(25) );
	StringCbCopy( m_szMacro[MACRO_MOVIE_MODE]					,sizeof( m_szMacro[MACRO_MOVIE_MODE] )					, STR(26) );
	StringCbCopy( m_szMacro[MACRO_USER_RECORD]					,sizeof( m_szMacro[MACRO_USER_RECORD] )					, STR(27) );
	StringCbCopy( m_szMacro[MACRO_TEAM_INFO]					,sizeof( m_szMacro[MACRO_TEAM_INFO] )					, STR(28) );
	StringCbCopy( m_szMacro[MACRO_CHANNEL_MOVE]					,sizeof( m_szMacro[MACRO_CHANNEL_MOVE] )				, STR(29) );
	StringCbCopy( m_szMacro[MACRO_CAMP_INFO]					,sizeof( m_szMacro[MACRO_CAMP_INFO] )					, STR(30) );
	StringCbCopy( m_szMacro[MACRO_CAMP_USER]					,sizeof( m_szMacro[MACRO_CAMP_USER] )					, STR(31) );
	StringCbCopy( m_szMacro[MACRO_GUILDNAME_CHANGE]				,sizeof( m_szMacro[MACRO_GUILDNAME_CHANGE] )			, STR(32) );
	StringCbCopy( m_szMacro[MACRO_EVENT_TEST]					,sizeof( m_szMacro[MACRO_EVENT_TEST] )					, STR(33) );
	StringCbCopy( m_szMacro[MACRO_WHOLE_CHAT_OFF]				,sizeof( m_szMacro[MACRO_WHOLE_CHAT_OFF] )				, STR(34) );
	StringCbCopy( m_szMacro[MACRO_WHOLE_CHAT_ON]				,sizeof( m_szMacro[MACRO_WHOLE_CHAT_ON] )				, STR(35) );  
	StringCbCopy( m_szMacro[MACRO_CAMERA_TEST]					,sizeof( m_szMacro[MACRO_CAMERA_TEST] )					, STR(36) );  
	StringCbCopy( m_szMacro[MACRO_FONT_TEXTURE]					,sizeof( m_szMacro[MACRO_FONT_TEXTURE] )				, STR(2) );  
	StringCbCopy( m_szMacro[MACRO_SHOW_WND_ID]					,sizeof( m_szMacro[MACRO_SHOW_WND_ID] )					, STR(3) );
	StringCbCopy( m_szMacro[MACRO_CHAT_INFO_OFF]				,sizeof( m_szMacro[MACRO_CHAT_INFO_OFF] )				, STR(4) );
	StringCbCopy( m_szMacro[MACRO_CHAT_INFO_ON]					,sizeof( m_szMacro[MACRO_CHAT_INFO_ON] )				, STR(5) );
	StringCbCopy( m_szMacro[MACRO_CONTROL_SYNC_EX_ON]			,sizeof( m_szMacro[MACRO_CONTROL_SYNC_EX_ON] )			, STR(70) );
	StringCbCopy( m_szMacro[MACRO_CONTROL_SYNC_EX_OFF]			,sizeof( m_szMacro[MACRO_CONTROL_SYNC_EX_OFF] )			, STR(71) );
	StringCbCopy( m_szMacro[MACRO_EVENT_SHOP_GOODS_LIST]		,sizeof( m_szMacro[MACRO_EVENT_SHOP_GOODS_LIST] )		, "/10" );
	StringCbCopy( m_szMacro[MACRO_EVENT_SHOP_GOODS_BUY]			,sizeof( m_szMacro[MACRO_EVENT_SHOP_GOODS_BUY] )		, "/11" );
	StringCbCopy( m_szMacro[MACRO_EVENT_SHOP_STATE]				,sizeof( m_szMacro[MACRO_EVENT_SHOP_STATE] )			, STR(72) );
	StringCbCopy( m_szMacro[MACRO_EVENT_SHOP_BUY_USER_CLEAR]	,sizeof( m_szMacro[MACRO_EVENT_SHOP_BUY_USER_CLEAR] )	, STR(73) );
	StringCbCopy( m_szMacro[MACRO_CUSTOM_DDS_FROM_JPG]			,sizeof( m_szMacro[MACRO_CUSTOM_DDS_FROM_JPG] )			, "/20" );
	StringCbCopy( m_szMacro[MACRO_CUSTOM_JPG_MIX_DDS_FROM_DDS]	,sizeof( m_szMacro[MACRO_CUSTOM_JPG_MIX_DDS_FROM_DDS] )	, "/21" );
	StringCbCopy( m_szMacro[MACRO_CUSTOM_DDS_FROM_LSC]			,sizeof( m_szMacro[MACRO_CUSTOM_DDS_FROM_LSC] )			, "/22" );
	StringCbCopy( m_szMacro[MACRO_CUSTOM_LSC_FROM_DDS]			,sizeof( m_szMacro[MACRO_CUSTOM_LSC_FROM_DDS] )			, "/23" );
	StringCbCopy( m_szMacro[MACRO_CUSTOM_ENTITY_TEXTURE]		,sizeof( m_szMacro[MACRO_CUSTOM_ENTITY_TEXTURE] )		, "/24" );
	StringCbCopy( m_szMacro[MACRO_SHOW_WND]						,sizeof( m_szMacro[MACRO_SHOW_WND] )					, "/25" );
	StringCbCopy( m_szMacro[MACRO_PRESENT]						,sizeof( m_szMacro[MACRO_PRESENT] )						, STR(7) );
	StringCbCopy( m_szMacro[MACRO_BEST_FRIEND_APPLICATION]		,sizeof( m_szMacro[MACRO_BEST_FRIEND_APPLICATION] )		, STR(37) );
	StringCbCopy( m_szMacro[MACRO_BEST_FRIEND_AGREE]			,sizeof( m_szMacro[MACRO_BEST_FRIEND_AGREE] )			, STR(38) );
	StringCbCopy( m_szMacro[MACRO_BEST_FRIEND_REFUSE]			,sizeof( m_szMacro[MACRO_BEST_FRIEND_REFUSE] )			, STR(39) );
	StringCbCopy( m_szMacro[MACRO_BEST_FRIEND_CLEAR]			,sizeof( m_szMacro[MACRO_BEST_FRIEND_CLEAR] )			, STR(40) );
	StringCbCopy( m_szMacro[MACRO_EXPERIENCE_STATE]				,sizeof( m_szMacro[MACRO_EXPERIENCE_STATE] )			, STR(41) );
	StringCbCopy( m_szMacro[MACRO_MULTI_MAP_CHANGE]				,sizeof( m_szMacro[MACRO_MULTI_MAP_CHANGE] )			, STR(42) );
	StringCbCopy( m_szMacro[MACRO_MACRO_NOTIFY]					,sizeof( m_szMacro[MACRO_MACRO_NOTIFY] )				, STR(43) );	
	StringCbCopy( m_szMacro[MACRO_EXTRAITEM_GROWTH_CATALYST]    ,sizeof( m_szMacro[MACRO_EXTRAITEM_GROWTH_CATALYST] )   , STR(44) );
	StringCbCopy( m_szMacro[MACRO_GROWTH_LEVELUP]				,sizeof( m_szMacro[MACRO_GROWTH_LEVELUP] )				, STR(46) );
	StringCbCopy( m_szMacro[MACRO_MONSTER_CLOSE]				,sizeof( m_szMacro[MACRO_MONSTER_CLOSE] )				, STR(45) );
	StringCbCopy( m_szMacro[MACRO_BINGO_PRESENT]				,sizeof( m_szMacro[MACRO_BINGO_PRESENT] )				, STR(47) );
	StringCbCopy( m_szMacro[MACRO_MONSTER_SPAWN]				,sizeof( m_szMacro[MACRO_MONSTER_SPAWN] )				, STR(48) );
	StringCbCopy( m_szMacro[MACRO_SUPERGASHAPON_GET_ALL]		,sizeof( m_szMacro[MACRO_SUPERGASHAPON_GET_ALL] )		, STR(49) );
	StringCbCopy( m_szMacro[MACRO_SUPERGASHAPON_GET]			,sizeof( m_szMacro[MACRO_SUPERGASHAPON_GET] )			, STR(50) );
	StringCbCopy( m_szMacro[MACRO_SUPERGASHAPON_LIMIT_INFO_GET]	,sizeof( m_szMacro[MACRO_SUPERGASHAPON_LIMIT_INFO_GET] ), STR(51) );
	StringCbCopy( m_szMacro[MACRO_SUPERGASHAPON_LIMIT_RESET]	,sizeof( m_szMacro[MACRO_SUPERGASHAPON_LIMIT_RESET] ),    STR(52) );
	StringCbCopy( m_szMacro[MACRO_SEND_RECIVE_CHECK_ON]			,sizeof( m_szMacro[MACRO_SEND_RECIVE_CHECK_ON] ),	STR(53) );
	StringCbCopy( m_szMacro[MACRO_SEND_RECIVE_CHECK_OFF]		,sizeof( m_szMacro[MACRO_SEND_RECIVE_CHECK_OFF] ),	STR(54) );
	StringCbCopy( m_szMacro[MACRO_ATTENDANCE_WND_SHOW]			,sizeof( m_szMacro[MACRO_ATTENDANCE_WND_SHOW] )			, STR(55) );
	StringCbCopy( m_szMacro[MACRO_ATTENDANCE_ADD_DAY]			,sizeof( m_szMacro[MACRO_ATTENDANCE_ADD_DAY] )			, STR(56) );
	StringCbCopy( m_szMacro[MACRO_ATTENDANCE_ADD_PREV_MONTH]	,sizeof( m_szMacro[MACRO_ATTENDANCE_ADD_PREV_MONTH] )	, STR(57) );
	StringCbCopy( m_szMacro[MACRO_ATTENDANCE_RESET]				,sizeof( m_szMacro[MACRO_ATTENDANCE_RESET] )			, STR(58) );
	StringCbCopy( m_szMacro[MACRO_ATTENDANCE_SET_DATE]			,sizeof( m_szMacro[MACRO_ATTENDANCE_SET_DATE] )			, STR(59) );
	StringCbCopy( m_szMacro[MACRO_AWAKE_SET_DATE]				,sizeof( m_szMacro[MACRO_AWAKE_SET_DATE] )				, STR(60) );
	StringCbCopy( m_szMacro[MACRO_GASHAPON_GET_ALL]				,sizeof( m_szMacro[MACRO_GASHAPON_GET_ALL] )			, "/GetGashaAll" );
	StringCbCopy( m_szMacro[MACRO_CHAR_POWER_UP_ADD]			,sizeof( m_szMacro[MACRO_CHAR_POWER_UP_ADD] )			, STR(61) );
	StringCbCopy( m_szMacro[MACRO_ITEM_SLOT_INDEX_FIND]			,sizeof( m_szMacro[MACRO_ITEM_SLOT_INDEX_FIND] )		, STR(62) );
	StringCbCopy( m_szMacro[MACRO_ITEM_POWER_UP_ADD]			,sizeof( m_szMacro[MACRO_ITEM_POWER_UP_ADD] )			, STR(63) );
	StringCbCopy( m_szMacro[MACRO_ALL_LSC_FROM_DDS]				,sizeof( m_szMacro[MACRO_ALL_LSC_FROM_DDS] )			, STR(74) );
	StringCbCopy( m_szMacro[MACRO_CANNOT_OPEN_DDS_FILE]			,sizeof( m_szMacro[MACRO_CANNOT_OPEN_DDS_FILE] )		, "/CheckDDS" );
	StringCbCopy( m_szMacro[MACRO_CHAR_INFO_WND_SHOW]			,sizeof( m_szMacro[MACRO_CHAR_INFO_WND_SHOW] )			, STR(64) );
	StringCbCopy( m_szMacro[MACRO_SET_MISSION]					,sizeof( m_szMacro[MACRO_SET_MISSION] )					, STR(65) );
	StringCbCopy( m_szMacro[MACRO_SET_DATE_MISSION]				,sizeof( m_szMacro[MACRO_SET_DATE_MISSION] )			, STR(66) );
	StringCbCopy( m_szMacro[MACRO_SET_MISSION_VALUE]			,sizeof( m_szMacro[MACRO_SET_MISSION_VALUE] )			, STR(67) );
	StringCbCopy( m_szMacro[MACRO_ROLLBOOK_WND_SHOW]			,sizeof( m_szMacro[MACRO_ROLLBOOK_WND_SHOW] )			, STR(68) );
	StringCbCopy( m_szMacro[MACRO_ROLLBOOK_PROGRESS]			,sizeof( m_szMacro[MACRO_ROLLBOOK_PROGRESS] )			, STR(69) );
	StringCbCopy( m_szMacro[MACRO_STEAM_ACHIVE]				,sizeof( m_szMacro[MACRO_STEAM_ACHIVE] )			, "/achi" );
	StringCbCopy( m_szMacro[MACRO_STEAM_OPENWEB]				,sizeof( m_szMacro[MACRO_STEAM_OPENWEB] )			, "/openW" );
	StringCbCopy( m_szMacro[MACRO_PENALTY_VIEW]				,sizeof( m_szMacro[MACRO_PENALTY_VIEW] )			, "/penaltyview" );
	
	StringCbCopy( m_szMacro[MACRO_FPS_LOWER]					,sizeof( m_szMacro[MACRO_FPS_LOWER] )					, "/fps" );
	StringCbCopy( m_szMacro[MACRO_FPS_UPPER]					,sizeof( m_szMacro[MACRO_FPS_UPPER] )					, "/FPS" );
	StringCbCopy( m_szMacro[MACRO_TIMER_CHANGE]					,sizeof( m_szMacro[MACRO_TIMER_CHANGE] )				, "/timerchange" );
	StringCbCopy( m_szMacro[MACRO_FPS_LAG_CHECK]				,sizeof( m_szMacro[MACRO_FPS_LAG_CHECK] )				, "/fpslag" );
	StringCbCopy( m_szMacro[MACRO_HOUSING_TEST]					,sizeof( m_szMacro[MACRO_HOUSING_TEST] )				, "/HT" );
	StringCbCopy( m_szMacro[MACRO_HOUSING_CMD]					,sizeof( m_szMacro[MACRO_HOUSING_CMD] )					, "/HC" );
	StringCbCopy( m_szMacro[MACRO_HOUSING_SAVE]					,sizeof( m_szMacro[MACRO_HOUSING_SAVE] )				, "/HE" );
	StringCbCopy( m_szMacro[MACRO_COMFORTABLE_LIFE]				,sizeof( m_szMacro[MACRO_COMFORTABLE_LIFE] )			, STR(1) );
	StringCbCopy( m_szMacro[MACRO_TOURNAMENT_NEXT_STATE]		,sizeof( m_szMacro[MACRO_TOURNAMENT_NEXT_STATE] )		, STR(2) );
	StringCbCopy( m_szMacro[MACRO_TOURNAMENT_RESULT]			,sizeof( m_szMacro[MACRO_TOURNAMENT_RESULT] )			, STR(3) );
	StringCbCopy( m_szMacro[MACRO_BATTLEROOM_JOIN]				,sizeof( m_szMacro[MACRO_BATTLEROOM_JOIN] )				, STR(75) );
	StringCbCopy( m_szMacro[MACRO_SHOW_LADDERROOM]				,sizeof( m_szMacro[MACRO_SHOW_LADDERROOM] )				, STR(76) );
}
