#include "MainScript.hpp"
#include "ScriptCallback.hpp"
#include "Natives.hpp"
#include "Timer.hpp"
#include "CustomText.hpp"
#include "UIManagement.h"
#include "RegularOption.h"
#include "SubOption.h"
#include "BoolOption.h"
#include "HackFuncs.h"

namespace Big
{
	bool godModeBoolean = false;
	bool neverWantedBoolean = false;
	bool localInvisibilityBoolean = false;
	bool superJumpBoolean = false;
	bool infiniteAmmoBoolean = false;
	bool autoFixVehicleBoolean = false;
	bool superRunBoolean = false;

	bool MainScript::IsInitialized()
	{
		return m_Initialized;
	}

	ScriptType MainScript::GetType()
	{
		return ScriptType::Game;
	}

	void SelfSub()
	{

		int selfIndex = g_UIManagement->add_parent_sub("Self");
		int selfHomeIndex = g_UIManagement->m_ChildParentSubs[selfIndex].add_child_sub("Home");

		ParentSub* parentSub = &g_UIManagement->m_ChildParentSubs[selfIndex];

		static BoolOption godModeBooleanOption{ "God mode", godModeBoolean };
		parentSub->m_ChildSubs[selfHomeIndex].add_option(godModeBooleanOption);

		static RegularOption suicideRegularOption{ "Easy way out", [] { ENTITY::SET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID(), 0); } };
		parentSub->m_ChildSubs[selfHomeIndex].add_option(suicideRegularOption);

		static BoolOption neverWantedBooleanOption{ "Never wanted", neverWantedBoolean };
		parentSub->m_ChildSubs[selfHomeIndex].add_option(neverWantedBooleanOption);

		static RegularOption removeCopsFromAreaOption{ "Remove police from area", [] {
			auto playerCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
			MISC::CLEAR_AREA_OF_COPS(playerCoords.x, playerCoords.y, playerCoords.z, 15000, 0);
		} };
		parentSub->m_ChildSubs[selfHomeIndex].add_option(removeCopsFromAreaOption);

		static BoolOption localInvisibilityBooleanOption{ "Invisibility", localInvisibilityBoolean };
		parentSub->m_ChildSubs[selfHomeIndex].add_option(localInvisibilityBooleanOption);

		static BoolOption superJumpBooleanOption{ "Super jump", superJumpBoolean };
		parentSub->m_ChildSubs[selfHomeIndex].add_option(superJumpBooleanOption);

		int selfWeaponsIndex = parentSub->add_child_sub("Weapons");

		static SubOption selfWeaponsSubOption{ "Weapon options", selfIndex, selfWeaponsIndex };
		parentSub->m_ChildSubs[selfHomeIndex].add_option(selfWeaponsSubOption);

		static RegularOption selfWeaponsEmergencyWeaponOption{ "Get emergency gun", [] { WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Joaat("weapon_rayminigun"), 1500, false, true); } };
		parentSub->m_ChildSubs[selfWeaponsIndex].add_option(selfWeaponsEmergencyWeaponOption);

		static BoolOption selfWeaponsInfiniteAmmoBooleanOption{ "Infinite ammo", infiniteAmmoBoolean };
		parentSub->m_ChildSubs[selfWeaponsIndex].add_option(selfWeaponsInfiniteAmmoBooleanOption);

		int selfTeleportationIndex = parentSub->add_child_sub("Teleportation");

		static SubOption selfTeleportationSubOption{ "Teleportation", selfIndex, selfTeleportationIndex };
		parentSub->m_ChildSubs[selfHomeIndex].add_option(selfTeleportationSubOption);

		int selfMiscIndex = parentSub->add_child_sub("Misc");
		
		static SubOption selfMiscSubOption{ "Misc", selfIndex, selfMiscIndex };
		parentSub->m_ChildSubs[selfHomeIndex].add_option(selfMiscSubOption);

		static BoolOption casinoSwapBooleanOption{ "Casino model swap", lesterCasino };
		parentSub->m_ChildSubs[selfMiscIndex].add_option(casinoSwapBooleanOption);
	}

	void VehicleSub()
	{
		int vehicleIndex = g_UIManagement->add_parent_sub("Vehicle");
		int vehicleHomeIndex = g_UIManagement->m_ChildParentSubs[vehicleIndex].add_child_sub("Home");

		ParentSub* parentSub = &g_UIManagement->m_ChildParentSubs[vehicleIndex];

		static BoolOption autoFixVehicleOption{ "Auto-fix vehicle", autoFixVehicleBoolean };
		parentSub->m_ChildSubs[vehicleHomeIndex].add_option(autoFixVehicleOption);
	}

	void SpawnerSub()
	{
		int spawnerIndex = g_UIManagement->add_parent_sub("Spawn");
		int spawnerHomeIndex = g_UIManagement->m_ChildParentSubs[spawnerIndex].add_child_sub("Home");

		ParentSub* parentSub = &g_UIManagement->m_ChildParentSubs[spawnerIndex];

		int spawnerOptionsIndex = g_UIManagement->m_ChildParentSubs[spawnerIndex].add_child_sub("Spawner options");

		static SubOption spawnerOptionsSubOption{ "Spawner options", spawnerIndex, spawnerOptionsIndex };
		parentSub->m_ChildSubs[spawnerHomeIndex].add_option(spawnerOptionsSubOption);

		static BoolOption spawnerOptionsSpawnInVehicleBooleanOption{ "Spawn in vehicle", spawnInVehicle };
		parentSub->m_ChildSubs[spawnerOptionsIndex].add_option(spawnerOptionsSpawnInVehicleBooleanOption);

		static RegularOption spawnTankOption{ "Mini Cooper", [] { SpawnVehicle(Joaat("khanjali")); } };
		parentSub->m_ChildSubs[spawnerHomeIndex].add_option(spawnTankOption);

		static RegularOption spawnDeluxoOption{ "Deluxo", [] { SpawnVehicle(Joaat("deluxo")); } };
		parentSub->m_ChildSubs[spawnerHomeIndex].add_option(spawnDeluxoOption);
	}

	void MainScript::Initialize()
	{
		m_Initialized = true;

		g_CustomText->AddText(CONSTEXPR_JOAAT("HUD_JOINING"), "OOPSIE WOOPSIE!! Uwu We made a fucky wucky!! A wittle fucko boingo! The code monkeys at our headquarters are working VEWY HAWD to fix this!");
		g_CustomText->AddText(CONSTEXPR_JOAAT("HUD_TRANSP"), "OOPSIE WOOPSIE!! Uwu We made a fucky wucky!! A wittle fucko boingo! The code monkeys at our headquarters are working VEWY HAWD to fix this!");

		SelfSub();
		VehicleSub();
		SpawnerSub();
	}

	void MainScript::Destroy()
	{
		GodModeBooleanFunction(false);
		NeverWantedBooleanFunction(false);
		LocalInvisibilityBooleanFunction(false);
		SuperJumpBooleanFunction(false);
		InfiniteAmmoBooleanFunction(false);
		AutoFixVehicle(false);
	}

	void MainScript::Tick()
	{
		g_UIManagement->tick();
		GodModeBooleanFunction(godModeBoolean);
		NeverWantedBooleanFunction(neverWantedBoolean);
		LocalInvisibilityBooleanFunction(localInvisibilityBoolean);
		SuperJumpBooleanFunction(superJumpBoolean);
		InfiniteAmmoBooleanFunction(infiniteAmmoBoolean);
		AutoFixVehicle(autoFixVehicleBoolean);
		SuperRun(superRunBoolean);
	}
}