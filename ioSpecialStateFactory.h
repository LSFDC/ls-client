#pragma once
#include "GameEnumType.h"
#include "ItemDefine.h"

#include "ioBunkerDummy.h"

#include "StateClass/ioConstructHouseSpecialState.h"
#include "StateClass/ioBlockInterplaySpecialState.h"

class ioBaseChar;

namespace ioSpecialStateFactory
{
	ioSpecialStateBase* CreateSpecialState( ioBaseChar* pOwner, CharState eNewState, CharState eCurrState )
	{
		ioSpecialStateBase* pNewState = NULL;
		switch( eNewState )
		{
			// 용병
		case CS_UROBORUS:
			pNewState = new ioUroborusSpecialState;
			break;
		case CS_WEREWOLF:
			pNewState = new ioWerewolfSpecialState;
			break;
		case CS_WITCH_FLY:
			pNewState = new ioWitchFlySpecialState;
			break;
		case CS_MECHANICS_FLY:
			pNewState = new ioMechanicsFlySpecialState;
			break;
		case CS_PHANTOM_FLY:
			pNewState = new ioPhantomFlySpecialState;
			break;
		case CS_LUCIFER_FLY:
			pNewState = new ioLuciferFlySpecialState;
			break;
		case CS_NAKORURU_FLY:
			pNewState = new ioNakoruruFlySpecialState;
			break;
		case CS_NAKORURU_JUMP_ATTACK:
			pNewState = new ioNakoruruJumpAttackSpecialState;
			break;
		case CS_DESTROYER_FLY:
			pNewState = new ioDestroyerFlySpecialState;
			break;
		case CS_MICHAEL_FLY:
			pNewState = new ioMichaelFlySpecialState;
			break;
		case CS_GOOMIHO_LAND_FLY:
			pNewState = new ioGoomihoFlySpecialState;
			break;
		case CS_PETERPAN_FLY:
			pNewState = new ioPeterpanFlySpecialState;
			break;
		case CS_SUCCUBUS_ATTACK:
			pNewState = new ioSuccubusSpecialState;
			break;
		case CS_CYVOGUE_STATE:
			pNewState = new ioCyvogueSpecialState;
			break;
		case CS_TRINITY:
			pNewState = new ioTrinitySpecialState;
			break;
		case CS_TRINITY_SWALLOW:
			pNewState = new ioTrinitySwallowMoonSpecialState;
			break;
		case CS_GHOST_STEALER_STATE:
			pNewState = new ioGhostStealerSpecialState;
			break;
		case CS_DRACULA_FLY:
			pNewState = new ioDraculaFlySpecialState;
			break;
		case CS_HAKUMEN_DEFENSE:
			pNewState = new ioHakumenDefenseSpecialState;
			break;
		case CS_DEVIL_FLY:
			pNewState = new ioDevilFlySpecialState;
			break;
		case CS_FLASH_SPECIAL:
			pNewState = new ioFlashSpecialState;
			break;
		case CS_EVY_BLINK:
			pNewState = new ioEvyBlinkSpecialState;
			break;
		case CS_EVY_BOMB_STATE:
			pNewState = new ioEvyBombSpecialState;
			break;
		case CS_PSYCHO_SPECIAL:
			pNewState = new ioPsychoMetrySpecialState;
			break;
		case CS_SLASHER_SPECIAL:
			pNewState = new ioSlasherSpecialState;
			break;
		case CS_TITAN_EXTEND_MOVE:
			pNewState = new ioTitanExtendMoveSpecialState;
			break;
		case CS_SUPER_DIMENSION_FIRE:
			pNewState = new ioSuperDimensionFireSpecialState;
			break;
		case CS_SUPER_DIMENSION_DASH:
			pNewState = new ioSuperDimensionDashSpecialState;
			break;
		case CS_NANGIN_SPECIAL:
			pNewState = new ioNanginSpecialState;
			break;
		case CS_STRIKER_MOVE:
			pNewState = new ioStrikerMoveSpecialState;
			break;
		case CS_STRIKER_LAND_ATTACK:
			pNewState = new ioStrikerLandAttackSpecialState;
			break;
		case CS_DRAGONNEST_ASS_SPECIAL:
			pNewState = new ioDragonNestAssSpecialState;
			break;
		case CS_THUNDER_BIRD_OBJECT_SPECIAL:
			pNewState = new ioThunderBirdObjectSpecialState;
			break;
		case CS_THUNDER_BIRD_HOLD:
			pNewState = new ioThunderBirdHoldSpecialState;
			break;
		case CS_THUNDER_BIRD_BOMB_STATE:
			pNewState = new ioThunderBirdBombSpecialState;
			break;
		case CS_VALKYRIE_FLY:
			pNewState = new ioValkyrieFlySpecialState;
			break;
		case CS_EARTHQUAKE_SPECIAL:
			pNewState = new ioEarthQuakeSpecialState;
			break;
		case CS_SHADOW_RANGER_SPECIAL:
			pNewState = new ioShadowRangerSpecialState;
			break;
		case CS_DARKKNIGHT_SPECIAL:
			pNewState = new ioDarkKnightSpecialState;
			break;
		case CS_JUMPER_SPECIAL:
			pNewState = new ioJumperSpecialSpState;
			break;
		case CS_SABER_ATTACK:
			pNewState = new ioSaberAttackSpecialState;
			break;
		case CS_SABER_FLY:
			pNewState = new ioSaberFlySpecialState;
			break;
		case CS_SABER_DASH_ATTACK:
			pNewState = new ioSaberDashAttackSpecialState;
			break;
		case CS_HADES_ATTACK:
			pNewState = new ioHadesSpecialState;
			break;
		case CS_MAGIC_SWORD_ATTACK:
			pNewState = new ioMagicSwordSpecialState;
			break;
		case CS_NATA_COMBO:
			pNewState = new ioNataComboSpecialState;
			break;
		case CS_NATA_FLY:
			pNewState = new ioNataFlySpecialState;
			break;
		case CS_RESEARCHER_REAR_FLY:
			pNewState = new ioResearcherRearFlySpecialState;
			break;
		case CS_BLADE_FLY:
			pNewState = new ioBladeFlySpecialState;
			break;
		case CS_GRAPPLER_JUMP_GRAP:
			pNewState = new ioGrapplerJumpAttackSpecialState;
			break;
		case CS_SAWANG_STATE:
			pNewState = new ioSawangSpecialState;
			break;
		case CS_APOSTATE_MONK_SPECIAL:
			pNewState = new ioApostateMonkSpecialState;
			break;
		case CS_JEONWOOCHI_EXTEND_MOVE:
			pNewState = new ioJeonWooChiExtendMoveSpecialState;
			break;
		case CS_JEONWOOCHI_EXTEND_DASH:
			pNewState = new ioJeonWooChiExtendDashSpecialState;
			break;
		case CS_STRIDER_SPECIAL:
			pNewState = new ioStriderSpecialState;
			break;
		case CS_RED_HOOD_MOVE:
			pNewState = new ioRedHoodItemMoveSpecialState;
			break;
		case CS_MUTANT_SPECIAL_STATE:
			pNewState = new ioMutantItemSpecialState;

			break;
		case CS_NEPHILIM_PUSH_STATE:
			pNewState = new ioNephilimPushState;
			break;
		case CS_NEPHILIM_BOMB_STATE:
			pNewState = new ioNephilimBombState;
			break;
		case CS_NEPHILIM_TELEPORT_STATE:
			pNewState = new ioNephilimTeleportState;
			break;
		case CS_MIR_WARRIOR_DASH_STATE:
			pNewState = new ioMirWarriorDashSpecialState;
			break;
		case CS_CUCHULAIN_SPECIAL_STATE:
			pNewState = new ioCuchulainItemSpecialState;
			break;
		case CS_GALFORD_CMD_STATE:
			pNewState = new ioGalfordCmdSpecialState;
			break;
		case CS_ONIMUSHA_RUN_EXTRA_STATE:
			pNewState = new ioOnimushaRunExtraState;
			break;
		case CS_DRUID_CROUCH_GATHERING_STATE:
			pNewState = new ioDruidCrouchGatheringState;
			break;
		case CS_DRUID_EAGLE_FLY:
			pNewState = new ioDruidEagleFlyState;
			break;
		case CS_SASINMU_SPECIAL:
			pNewState = new ioSasinmuSpecialState;
			break;
		case CS_DARKSEAS_STATE:
			pNewState = new ioDarkSeasSpecialState;
			break;
		case CS_PREDATOR_SPECIAL:
			pNewState = new ioPredatorSpecialState;
			break;
		case CS_CROW_SPECIAL_STATE:
			pNewState = new ioCrowItemSpecialState;
			break;
		case CS_MEPHISTO_STATE:
			pNewState = new ioMephistoSpecialState;
			break;

			//진화용병
		case CS_NINJA_EXPANSION_MOVE:
			pNewState = new ioNinjaExpansionSpecialState;
			break;

			// 아이템
		case CS_OBJECT_RUNNING_ATTACK:
			pNewState = new ioObjectRunningAttackSpecialState;
			break;
		case CS_OBJECT_ATTACH:
			pNewState = new ioObjectAttachSpecialState;
			break;			
		case CS_HELICOPTER:
			pNewState = new ioHelicopterSpecialState;
			break;
		case CS_HELICOPTER_ATTACH:
			pNewState = new ioHelipCopterAttachSpecialState;
			break;
		case CS_VEHICLE_DRIVER:
			pNewState = new ioVehicleDirverState;
			break;
		case CS_PASSENGER:
			pNewState = new ioPassengerState;
			break;
		case CS_OBJECT_RUNNING_COMBO_ATTACK:
			pNewState = new ioObjectRunningComboSpecialState;
			break;
		case CS_FLY_BUFF_EXTRA_ATTACK:
			pNewState = new ioFlyJumpExtraAttackBuffSpecialState;
			break;
		case CS_BUNKER_STATE:
			pNewState = new ioBunkerAttachSpecialState;
			break;

			// 기타
		case CS_BLOCK_INTEPLAY:
			pNewState = new ioBlockInterplaySpecialState;
			break;
		case CS_CONSTRUCT_HOUSE:
			pNewState = new ioConstructHouseSpecialState;
			break;
		}

		if( pNewState )
		{
			char szBuf[MAX_PATH];
			ioINILoader_e kLoader( "config/sp2_special_state.ini" );
			sprintf_s_e( szBuf, "state%d", (int)eNewState );
			kLoader.SetTitle( szBuf );

			pNewState->LoadProperty( kLoader );
			pNewState->Initialize( pOwner, eCurrState );
			if( pNewState->IsInitializeSync() && pOwner && pOwner->IsNeedSendNetwork() )
			{
				SP2Packet kPacket( CUPK_INITIALIZE_SPECIAL_STATE );
				kPacket << pOwner->GetCharName();
				kPacket << (int)eNewState;
				P2PNetwork::SendToAllPlayingUser( kPacket );
			}
		}

		return pNewState;
	}
};