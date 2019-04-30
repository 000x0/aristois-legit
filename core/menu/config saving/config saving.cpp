#include "config saving.h"
#include "../config.h"
#include <Windows.h>
#include <Psapi.h>
#include <lmcons.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <shlobj.h>
#include <time.h>
#include <random>
#include <sstream>
#include <fstream>
#include <shlwapi.h>
#include <iomanip>
#include <ctime>
#include "../../../source-sdk/misc/color.hpp"

void c_config_saving::Setup() {
	//aimbot

	setup_value(c_config::get().aim_enabled, false, "Aimbot", "Enabled");
	setup_value(c_config::get().aim_mode, 0, "Aimbot", "AimbotMode");
	setup_value(c_config::get().aim_silent, false, "Aimbot", "Silent");
	setup_value(c_config::get().aim_distance_based_fov, false, "Aimbot", "DynamicFov");
	setup_value(c_config::get().aim_fov_pistol, 0.f, "Aimbot", "FovPistol");
	setup_value(c_config::get().aim_smooth_pistol, 1.f, "Aimbot", "SmoothPistol");
	setup_value(c_config::get().aim_fov_rifle, 0.f, "Aimbot", "FovRifle");
	setup_value(c_config::get().aim_smooth_rifle, 1.f, "Aimbot", "SmoothRifle");
	setup_value(c_config::get().aim_fov_sniper, 0.f, "Aimbot", "FovSniper");
	setup_value(c_config::get().aim_smooth_sniper, 1.f, "Aimbot", "SmoothSniper");
	setup_value(c_config::get().aim_fov_heavy, 0.f, "Aimbot", "FovHeavy");
	setup_value(c_config::get().aim_smooth_heavy, 1.f, "Aimbot", "SmoothHeavy");
	setup_value(c_config::get().aim_fov_smg, 0.f, "Aimbot", "FovSmg");
	setup_value(c_config::get().aim_smooth_smg, 1.f, "Aimbot", "SmoothSmg");
	setup_value(c_config::get().aim_team_check, false, "Aimbot", "Team_Check");
	setup_value(c_config::get().aim_bone_pistol, 0, "Aimbot", "BonePistol");
	setup_value(c_config::get().aim_bone_sniper, 0, "Aimbot", "BoneSniper");
	setup_value(c_config::get().aim_bone_rifle, 0, "Aimbot", "BoneRifle");
	setup_value(c_config::get().aim_bone_heavy, 0, "Aimbot", "BoneHeavy");
	setup_value(c_config::get().aim_bone_smg, 0, "Aimbot", "BoneSmg");
	setup_value(c_config::get().backtrack, false, "Aimbot", "Backtrack");
	setup_value(c_config::get().aim_at_backtrack, false, "Aimbot", "AimAtBacktrack");
	setup_value(c_config::get().rcs_x_pistol, 0.f, "Aimbot", "RcsXpistol");
	setup_value(c_config::get().rcs_y_pistol, 0.f, "Aimbot", "RcsYpistol");
	setup_value(c_config::get().rcs_x_rifle, 0.f, "Aimbot", "RcsXrifle");
	setup_value(c_config::get().rcs_y_rifle, 0.f, "Aimbot", "RcsYrifle");
	setup_value(c_config::get().rcs_x_sniper, 0.f, "Aimbot", "RcsXsniper");
	setup_value(c_config::get().rcs_y_sniper, 0.f, "Aimbot", "RcsYsniper");
	setup_value(c_config::get().rcs_x_heavy, 0.f, "Aimbot", "RcsXheavy");
	setup_value(c_config::get().rcs_y_heavy, 0.f, "Aimbot", "RcsYheavy");
	setup_value(c_config::get().rcs_x_smg, 0.f, "Aimbot", "RcsXsmg");
	setup_value(c_config::get().rcs_y_smg, 0.f, "Aimbot", "RcsYsmg");
	setup_value(c_config::get().scope_aim, false, "Aimbot", "ScopeAim");
	setup_value(c_config::get().smoke_check, false, "Aimbot", "SmokeAim");

	//visuals

	setup_value(c_config::get().visuals_enabled, false, "Visuals", "Enabled");
	setup_value(c_config::get().visuals_team_check, false, "Visuals", "TeamCheck");
	setup_value(c_config::get().player_name, false, "Visuals", "Name");
	setup_value(c_config::get().player_box, false, "Visuals", "Box");
	setup_value(c_config::get().player_health, false, "Visuals", "Health");
	setup_value(c_config::get().player_flags_armor, false, "Visuals", "FlagsArmor");
	setup_value(c_config::get().player_flags_c4, false, "Visuals", "FlagsBomb");
	setup_value(c_config::get().player_flags_flashed, false, "Visuals", "FlagsFlashed");
	setup_value(c_config::get().player_flags_money, false, "Visuals", "FlagsMoney");
	setup_value(c_config::get().player_flags_scoped, false, "Visuals", "FlagsScoped");
	setup_value(c_config::get().player_weapon, false, "Visuals", "Weapon");
	setup_value(c_config::get().force_crosshair, false, "Visuals", "Force_crosshair");
	setup_value(c_config::get().fov, 0, "Visuals", "Fov");
	setup_value(c_config::get().viewmodel_fov, 0, "Visuals", "ViewmodelFov");
	setup_value(c_config::get().vis_chams_invis, false, "Visuals", "Chams_Player_Hidden");
	setup_value(c_config::get().vis_chams_vis, false, "Visuals", "Chams_Player");
	setup_value(c_config::get().vis_chams_type, 0, "Visuals", "Chams_Type");
	setup_value(c_config::get().visuals_glow, false, "Visuals", "Glow");
	setup_value(c_config::get().nightmode, false, "Visuals", "NightMode");
	setup_value(c_config::get().nightmode_brightness, 0, "Visuals", "NightmodeBrightness");
	setup_value(c_config::get().remove_smoke, false, "Visuals", "RemoveSmoke");
	setup_value(c_config::get().reduce_flash, false, "Visuals", "ReduceFlash");
	setup_value(c_config::get().remove_sleeves, false, "Visuals", "RemoveSleeves");
	setup_value(c_config::get().remove_hands, false, "Visuals", "RemoveHands");
	setup_value(c_config::get().backtrack_visualize, false, "Visuals", "Backtrack_visualize");
	setup_value(c_config::get().bomb_planted, false, "Visuals", "PlantedBomb");
	setup_value(c_config::get().dropped_weapons, false, "Visuals", "DroppedWeapons");
	setup_value(c_config::get().projectiles, false, "Visuals", "Projectiles");
	setup_value(c_config::get().danger_zone_dropped, false, "Visuals", "DangerZoneDropped");
	setup_value(c_config::get().entity_esp, false, "Visuals", "EntityEsp");
	setup_value(c_config::get().skeleton, false, "Visuals", "Skeleton");
	setup_value(c_config::get().backtrack_skeleton, false, "Visuals", "BacktrackSkeleton");
	setup_value(c_config::get().visuals_glow_enemy, false, "Visuals", "GlowEnemy");
	setup_value(c_config::get().visuals_glow_team, false, "Visuals", "GlowTeam");
	setup_value(c_config::get().visuals_glow_planted, false, "Visuals", "GlowPlanted");
	setup_value(c_config::get().visuals_glow_weapons, false, "Visuals", "GlowWeapons");
	setup_value(c_config::get().sound_footstep, false, "Visuals", "Footstep");

	//misc

	setup_value(c_config::get().misc_enabled, false, "Misc", "Enabled");
	setup_value(c_config::get().bunny_hop, false, "Misc", "BunnyHop");
	setup_value(c_config::get().clan_tag, false, "Misc", "ClanTag");
	setup_value(c_config::get().radar, false, "Misc", "Radar");
	setup_value(c_config::get().remove_scope, false, "Misc", "RemoveScope");
	setup_value(c_config::get().hitmarker, false, "Misc", "Hitmarker");
	setup_value(c_config::get().hitmarker_sound, 0, "Misc", "HitmarkerSound");
	setup_value(c_config::get().anti_screenshot, false, "Misc", "AntiScreenshot");
	setup_value(c_config::get().spectators_list, false, "Misc", "SpectatorsList");
	setup_value(c_config::get().edge_jump, false, "Misc", "EdgeJump");
	setup_value(c_config::get().edge_jump_duck_in_air, false, "Misc", "EdgeJumpDuckInAir");
	setup_value(c_config::get().edge_jump_key, 0, "Misc", "EdgeJumpKey");
	setup_value(c_config::get().watermark, true, "Misc", "Watermark");
	setup_value(c_config::get().logs_player_bought, false, "Misc", "LogsPlayerBought");
	setup_value(c_config::get().logs_player_hurt, false, "Misc", "LogsPlayerHurt");
	setup_value(c_config::get().logs_config_system, true, "Misc", "LogsConfigSystem");
	setup_value(c_config::get().viewmodel_offset, false, "Misc", "ViewModelOffset");
	setup_value(c_config::get().viewmodel_x, 0, "Visuals", "ViewmodelX");
	setup_value(c_config::get().viewmodel_y, 0, "Visuals", "ViewmodelY");
	setup_value(c_config::get().viewmodel_z, 0, "Visuals", "ViewmodelZ");

	//colors

	setup_value(c_config::get().clr_chams_vis[0], 1, "Colors", "PlayerR");
	setup_value(c_config::get().clr_chams_vis[1], 1, "Colors", "PlayerG");
	setup_value(c_config::get().clr_chams_vis[2], 1, "Colors", "PlayerB");
	setup_value(c_config::get().clr_chams_vis[3], 1, "Colors", "PlayerA");
	setup_value(c_config::get().clr_chams_invis[0], 1, "Colors", "INVPlayerR");
	setup_value(c_config::get().clr_chams_invis[1], 1, "Colors", "INVPlayerG");
	setup_value(c_config::get().clr_chams_invis[2], 1, "Colors", "INVPlayerB");
	setup_value(c_config::get().clr_chams_invis[3], 1, "Colors", "INVPlayerA");
	setup_value(c_config::get().clr_box[0], 1, "Colors", "BoxR");
	setup_value(c_config::get().clr_box[1], 1, "Colors", "BoxG");
	setup_value(c_config::get().clr_box[2], 1, "Colors", "BoxB");
	setup_value(c_config::get().clr_box[3], 1, "Colors", "BoxA");
	setup_value(c_config::get().clr_name[0], 1, "Colors", "NameR");
	setup_value(c_config::get().clr_name[1], 1, "Colors", "NameG");
	setup_value(c_config::get().clr_name[2], 1, "Colors", "NameB");
	setup_value(c_config::get().clr_name[3], 1, "Colors", "NameA");
	setup_value(c_config::get().clr_weapon[0], 1, "Colors", "WeaponR");
	setup_value(c_config::get().clr_weapon[1], 1, "Colors", "WeaponG");
	setup_value(c_config::get().clr_weapon[2], 1, "Colors", "WeaponB");
	setup_value(c_config::get().clr_weapon[3], 1, "Colors", "WeaponA");
	setup_value(c_config::get().clr_sky[0], 1, "Colors", "clr_skyR");
	setup_value(c_config::get().clr_sky[1], 1, "Colors", "clr_skyG");
	setup_value(c_config::get().clr_sky[2], 1, "Colors", "clr_skyB");
	setup_value(c_config::get().clr_sky[3], 1, "Colors", "clr_skyA");

	setup_value(c_config::get().clr_glow[0], 1, "Colors", "GlowR");
	setup_value(c_config::get().clr_glow[1], 1, "Colors", "GlowG");
	setup_value(c_config::get().clr_glow[2], 1, "Colors", "GlowB");
	setup_value(c_config::get().clr_glow[3], 1, "Colors", "GlowA");

	setup_value(c_config::get().clr_glow_team[0], 1, "Colors", "clr_glow_teamR");
	setup_value(c_config::get().clr_glow_team[1], 1, "Colors", "clr_glow_teamG");
	setup_value(c_config::get().clr_glow_team[2], 1, "Colors", "clr_glow_teamB");
	setup_value(c_config::get().clr_glow_team[3], 1, "Colors", "clr_glow_teamA");

	setup_value(c_config::get().clr_glow_dropped[0], 1, "Colors", "clr_glow_droppedR");
	setup_value(c_config::get().clr_glow_dropped[1], 1, "Colors", "clr_glow_droppedG");
	setup_value(c_config::get().clr_glow_dropped[2], 1, "Colors", "clr_glow_droppedB");
	setup_value(c_config::get().clr_glow_dropped[3], 1, "Colors", "clr_glow_droppedA");

	setup_value(c_config::get().clr_glow_planted[0], 1, "Colors", "clr_glow_plantedR");
	setup_value(c_config::get().clr_glow_planted[1], 1, "Colors", "clr_glow_plantedG");
	setup_value(c_config::get().clr_glow_planted[2], 1, "Colors", "clr_glow_plantedB");
	setup_value(c_config::get().clr_glow_planted[3], 1, "Colors", "clr_glow_plantedA");

	setup_value(c_config::get().clr_footstep[0], 1, "Colors", "clr_footstepR");
	setup_value(c_config::get().clr_footstep[1], 1, "Colors", "clr_footstepG");
	setup_value(c_config::get().clr_footstep[2], 1, "Colors", "clr_footstepB");
	setup_value(c_config::get().clr_footstep[3], 1, "Colors", "clr_footstepA");

	//skins
	setup_value(c_config::get().skinchanger_enable, false, "Skins", "Enable");
	setup_value(c_config::get().knife_model, 0, "Skins", "KnifeModel");
	setup_value(c_config::get().knife_wear, 0, "Skins", "KnifeWear");
	setup_value(c_config::get().paint_kit_index_knife, 0, "Skins", "PaintKitIndex");
	setup_value(c_config::get().paint_kit_vector_index_knife, 0, "Skins", "PaintKitVector");
	setup_value(c_config::get().paint_kit_index_usp, 0, "Skins", "PaintKitIndexUsp");
	setup_value(c_config::get().paint_kit_vector_index_usp, 0, "Skins", "PaintKitVectorUsp");
	setup_value(c_config::get().paint_kit_index_p2000, 0, "Skins", "PaintKitIndexP2000");
	setup_value(c_config::get().paint_kit_vector_index_p2000, 0, "Skins", "PaintKitVectorP2000");
	setup_value(c_config::get().paint_kit_index_glock, 0, "Skins", "PaintKitIndexGlock");
	setup_value(c_config::get().paint_kit_vector_index_glock, 0, "Skins", "PaintKitVectorGlock");
	setup_value(c_config::get().paint_kit_index_p250, 0, "Skins", "PaintKitIndexP250");
	setup_value(c_config::get().paint_kit_vector_index_p250, 0, "Skins", "PaintKitVectorP250");
	setup_value(c_config::get().paint_kit_index_fiveseven, 0, "Skins", "PaintKitIndexFiveSeven");
	setup_value(c_config::get().paint_kit_vector_index_fiveseven, 0, "Skins", "PaintKitVectorFiveSeven");
	setup_value(c_config::get().paint_kit_index_tec, 0, "Skins", "PaintKitIndexTec");
	setup_value(c_config::get().paint_kit_vector_index_tec, 0, "Skins", "PaintKitVectorTec");
	setup_value(c_config::get().paint_kit_index_cz, 0, "Skins", "PaintKitIndexCZ");
	setup_value(c_config::get().paint_kit_vector_index_cz, 0, "Skins", "PaintKitVectorCZ");
	setup_value(c_config::get().paint_kit_index_duals, 0, "Skins", "PaintKitIndexDuals");
	setup_value(c_config::get().paint_kit_vector_index_duals, 0, "Skins", "PaintKitVectorDuals");
	setup_value(c_config::get().paint_kit_index_deagle, 0, "Skins", "PaintKitIndexDeagle");
	setup_value(c_config::get().paint_kit_vector_index_deagle, 0, "Skins", "PaintKitVectorDeagle");
	setup_value(c_config::get().paint_kit_index_revolver, 0, "Skins", "PaintKitIndexRevolver");
	setup_value(c_config::get().paint_kit_vector_index_revolver, 0, "Skins", "PaintKitVectorRevolver");

	setup_value(c_config::get().paint_kit_index_famas, 0, "Skins", "PaintKitIndexFamas");
	setup_value(c_config::get().paint_kit_vector_index_famas, 0, "Skins", "PaintKitVectorFamas");
	setup_value(c_config::get().paint_kit_index_galil, 0, "Skins", "PaintKitIndexGalil");
	setup_value(c_config::get().paint_kit_vector_index_galil, 0, "Skins", "PaintKitVectorGalil");
	setup_value(c_config::get().paint_kit_index_m4a4, 0, "Skins", "PaintKitIndexM4A4");
	setup_value(c_config::get().paint_kit_vector_index_m4a4, 0, "Skins", "PaintKitVectorM4A4");
	setup_value(c_config::get().paint_kit_index_m4a1, 0, "Skins", "PaintKitIndexM4A1");
	setup_value(c_config::get().paint_kit_vector_index_m4a1, 0, "Skins", "PaintKitVectorM4A1");
	setup_value(c_config::get().paint_kit_index_ak47, 0, "Skins", "PaintKitIndexAK47");
	setup_value(c_config::get().paint_kit_vector_index_ak47, 0, "Skins", "PaintKitVectorAK47");
	setup_value(c_config::get().paint_kit_index_sg553, 0, "Skins", "PaintKitIndexSG553");
	setup_value(c_config::get().paint_kit_vector_index_sg553, 0, "Skins", "PaintKitVectorSG553");
	setup_value(c_config::get().paint_kit_index_aug, 0, "Skins", "PaintKitIndexAUG");
	setup_value(c_config::get().paint_kit_vector_index_aug, 0, "Skins", "PaintKitVectorAUG");
	setup_value(c_config::get().paint_kit_index_ssg08, 0, "Skins", "PaintKitIndexSSG08");
	setup_value(c_config::get().paint_kit_vector_index_ssg08, 0, "Skins", "PaintKitVectorSSG08");
	setup_value(c_config::get().paint_kit_index_awp, 0, "Skins", "PaintKitIndexAWP");
	setup_value(c_config::get().paint_kit_vector_index_awp, 0, "Skins", "PaintKitVectorAWP");
	setup_value(c_config::get().paint_kit_index_scar, 0, "Skins", "PaintKitIndexSCAR");
	setup_value(c_config::get().paint_kit_vector_index_scar, 0, "Skins", "PaintKitVectorSCAR");
	setup_value(c_config::get().paint_kit_index_g3sg1, 0, "Skins", "PaintKitIndexG3SG1");
	setup_value(c_config::get().paint_kit_vector_index_g3sg1, 0, "Skins", "PaintKitVectorG3SG1");
}

void c_config_saving::setup_value(int& value, int def, std::string category, std::string name) {
	value = def;
	ints.push_back(new config_value< int >(category, name, &value));
}

void c_config_saving::setup_value(float& value, float def, std::string category, std::string name) {
	value = def;
	floats.push_back(new config_value< float >(category, name, &value));
}

void c_config_saving::setup_value(bool& value, bool def, std::string category, std::string name) {
	value = def;
	bools.push_back(new config_value< bool >(category, name, &value));
}

void c_config_saving::save() {
	std::string  file;
	CreateDirectoryA("C:\\aristois\\", NULL);

	if (c_config::get().config_selection == 0)
		file = "C:\\aristois\\alpha.ini";
	if (c_config::get().config_selection == 1)
		file = "C:\\aristois\\beta.ini";
	if (c_config::get().config_selection == 2)
		file = "C:\\aristois\\gamma.ini";


	for (auto value : ints)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : floats)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : bools)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), *value->value ? "true" : "false", file.c_str());
}

void c_config_saving::load() {
	static TCHAR path[MAX_PATH];
	std::string folder, file;

	CreateDirectoryA("C:\\aristois\\", NULL);

	if (c_config::get().config_selection == 0)
		file = "C:\\aristois\\alpha.ini";
	if (c_config::get().config_selection == 1)
		file = "C:\\aristois\\beta.ini";
	if (c_config::get().config_selection == 2)
		file = "C:\\aristois\\gamma.ini";

	char value_l[32] = { '\0' };

	for (auto value : ints) {
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atoi(value_l);
	}

	for (auto value : floats) {
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atof(value_l);
	}

	for (auto value : bools) {
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = !strcmp(value_l, "true");
	}
}