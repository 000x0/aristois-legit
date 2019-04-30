#include "menu.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "config saving/config saving.h"
#include "../features/misc/logs.hpp"
#include "../features/skinchanger/kit_parser.hpp"

#define UNLEN 256

IDirect3DStateBlock9 *state_block;
ImFont* Main;
ImFont* MainCaps;
ImFont* Icons;
ImFont* Menu;

bool reverse = false;
int offset = 0;
bool show_popup = false;
static bool save_config = false;
static bool load_config = false;

namespace ImGui {
	void ColorPickerSpacing() {
		ImGui::SameLine();
		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x - 32.0f - ImGui::GetStyle().FramePadding.x * 2.0f, ImGui::GetCursorPosY() + 1));
	}

	long GetMils() {
		auto duration = std::chrono::system_clock::now().time_since_epoch();
		return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
	}

	void Popup(const char* text, int onScreenMils, bool* done) {
		if (!done)
			show_popup = true;

		ImGuiIO &io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		int Wd = io.DisplaySize.x;
		static long oldTime = -1;
		ImGui::SetNextWindowPos(ImVec2(Wd - offset, 100));
		style.WindowMinSize = ImVec2(100.f, 20.f);
		ImGui::Begin("##PopUpWindow", &show_popup, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		ImVec2 p = ImGui::GetCursorScreenPos();

		ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(p.x - 15, p.y - 13), ImVec2(p.x + ImGui::GetWindowWidth(), p.y - 16), ImColor(167, 24, 71, 255), ImColor(58, 31, 87, 255), ImColor(58, 31, 87, 255), ImColor(167, 24, 71, 255));

		long currentTime_ms = GetMils();

		ImVec2 txtSz = ImGui::CalcTextSize(text);
		ImGui::SetCursorPosY(ImGui::GetWindowHeight() / 2 - txtSz.y / 2);
		ImGui::Text(text);

		if (!reverse) {
			if (offset < ImGui::GetWindowWidth())
				offset += (ImGui::GetWindowWidth() + 5) * ((1000.0f / ImGui::GetIO().Framerate) / 100);

			if (offset >= ImGui::GetWindowWidth() && oldTime == -1) {
				oldTime = currentTime_ms;
			}
		}

		if (currentTime_ms - oldTime >= onScreenMils && oldTime != -1) // close after x mils
			reverse = true;

		if (reverse) {
			if (offset > 0)
				offset -= (ImGui::GetWindowWidth() + 5) * ((1000.0f / ImGui::GetIO().Framerate) / 100);
			if (offset <= 0) {
				offset = 0;
				reverse = false;
				*done = true;
				oldTime = -1;
				show_popup = false;
			}
		}

		ImGui::End();
	}
}

void c_menu::run() {
	ImGui::GetIO().MouseDrawCursor = opened;
	static int page = 0;

	if (opened) {
		ImGui::GetStyle().Colors[ImGuiCol_CheckMark] = ImVec4(167 / 255.f, 24 / 255.f, 71 / 255.f, 1.f);
		ImGui::GetStyle().Colors[ImGuiCol_SliderGrab] = ImVec4(167 / 255.f, 24 / 255.f, 71 / 255.f, 1.f);
		ImGui::GetStyle().Colors[ImGuiCol_SliderGrabActive] = ImVec4(167 / 255.f, 24 / 71, 247 / 255.f, 1.f);

		ImGui::SetNextWindowSize(ImVec2(600, 600), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("aristois", &opened, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar); {
			ImVec2 p = ImGui::GetCursorScreenPos();
			ImColor c = ImColor(32, 114, 247);

			//title bar
			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y + 30), ImVec2(p.x + ImGui::GetWindowWidth(), p.y - 3), ImColor(30, 30, 39));
			//draw gradient bellow title bar
			ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(p.x, p.y + 32), ImVec2(p.x + ImGui::GetWindowWidth(), p.y + +30), ImColor(167, 24, 71, 255), ImColor(58, 31, 87, 255), ImColor(58, 31, 87, 255), ImColor(167, 24, 71, 255));
			//push font for window stuff
			ImGui::PushFont(Menu);
			//set cheat name position
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7); //góra, dół
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7); //lewo prawo
			//render cheat name
			ImGui::Text("aristois.me");
			ImGui::SameLine();

			//set tabs position
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 9); //góra, dół
			//render tabs
			if (ImGui::ButtonT("legit", ImVec2(40, 30), page, 0, false, false)) page = 0; ImGui::SameLine(0, 0);
			if (ImGui::ButtonT("visuals", ImVec2(40, 30), page, 1, false, false)) page = 1; ImGui::SameLine(0, 0);
			if (ImGui::ButtonT("misc", ImVec2(40, 30), page, 2, false, false)) page = 2; ImGui::SameLine(0, 0);
			if (ImGui::ButtonT("skins", ImVec2(40, 30), page, 3, false, false)) page = 3; ImGui::SameLine(0, 0);
			if (ImGui::ButtonT("config", ImVec2(40, 30), page, 4, false, false)) page = 4; ImGui::SameLine(0, 0);
			//pop window font
			ImGui::PopFont();

			ImGui::PushFont(Menu);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 45); //góra, dół
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 222); //lewo prawo

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));

			//push window color for child
			ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(30 / 255.f, 30 / 255.f, 39 / 255.f, 1.0f));
			//push border color for child
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0 / 255.f, 0 / 255.f, 0 / 255.f, 0.1f));

			if (c_config::get().visuals_preview) {
				run_visuals_preview();
			}

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleVar();

			static int test = 0;
			switch (page) {
			case 0:
				ImGui::Columns(2, NULL, false);
				ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
				ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));

				//push window color for child
				ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(30 / 255.f, 30 / 255.f, 39 / 255.f, 1.0f));
				//push border color for child
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0 / 255.f, 0 / 255.f, 0 / 255.f, 0.1f));
				ImGui::BeginChild("aimbot", ImVec2(279, 543), true); {
					ImGui::Checkbox("active", &c_config::get().aim_enabled);
					ImGui::Combo("mode", &c_config::get().aim_mode, "hitbox\0nearest hitbox"); //todo add custom bone selection - designer
					ImGui::Checkbox("dynamic fov", &c_config::get().aim_distance_based_fov);
					ImGui::Checkbox("silent aim", &c_config::get().aim_silent);
					ImGui::Checkbox("scope aim", &c_config::get().scope_aim);
					ImGui::Checkbox("smoke aim", &c_config::get().smoke_check);
					ImGui::Checkbox("friendly fire", &c_config::get().aim_team_check);
					ImGui::Checkbox("backtrack", &c_config::get().backtrack);
					if (c_config::get().backtrack) {
						ImGui::Checkbox("aim at records", &c_config::get().aim_at_backtrack);
					}
				} ImGui::EndChild(true);

				ImGui::NextColumn();

				ImGui::BeginChild("settings", ImVec2(279, 543), true); {
					ImGui::PushFont(Icons);
					if (ImGui::ButtonT("A", ImVec2(50, 30), test, 0, false, ImColor(0, 0, 0))) test = 0; ImGui::SameLine(0, 0); //pistol
					if (ImGui::ButtonT("W", ImVec2(50, 30), test, 1, false, false)) test = 1; ImGui::SameLine(0, 0); //rifle
					if (ImGui::ButtonT("Z", ImVec2(50, 30), test, 2, false, false)) test = 2; ImGui::SameLine(0, 0); //sniper
					if (ImGui::ButtonT("L", ImVec2(50, 30), test, 3, false, false)) test = 3; ImGui::SameLine(0, 0); //mg
					if (ImGui::ButtonT("f", ImVec2(50, 30), test, 4, false, false)) test = 4; //heavy
					ImGui::PopFont();

					ImGui::PushFont(Menu);

					switch (test) {
					case 0:
						if (c_config::get().aim_mode == 0) {
							ImGui::Combo("pistol hitbox", &c_config::get().aim_bone_pistol, "head\0neck\0chest\0stomach\0pelvis");
						}
						ImGui::SliderFloat("pistol fov", &c_config::get().aim_fov_pistol, 0.0f, 180.0f, "%.2f");
						ImGui::SliderFloat("pistol smooth", &c_config::get().aim_smooth_pistol, 1.f, 10.f, "%.2f");
						ImGui::SliderFloat("pistol rcs x", &c_config::get().rcs_x_pistol, 0.0f, 1.0f, "%.2f");
						ImGui::SliderFloat("pistol rcs y", &c_config::get().rcs_y_pistol, 0.0f, 1.0f, "%.2f");
						break;
					case 1:
						if (c_config::get().aim_mode == 0) {
							ImGui::Combo("rifle hitbox", &c_config::get().aim_bone_rifle, "head\0neck\0chest\0stomach\0pelvis");
						}
						ImGui::SliderFloat("rifle fov", &c_config::get().aim_fov_rifle, 0.0f, 180.0f, "%.2f");
						ImGui::SliderFloat("rifle smooth", &c_config::get().aim_smooth_rifle, 1.f, 10.f, "%.2f");
						ImGui::SliderFloat("rifle rcs x", &c_config::get().rcs_x_rifle, 0.0f, 1.0f, "%.2f");
						ImGui::SliderFloat("rifle rcs y", &c_config::get().rcs_y_rifle, 0.0f, 1.0f, "%.2f");
						break;
					case 2:
						if (c_config::get().aim_mode == 0) {
							ImGui::Combo("sniper hitbox", &c_config::get().aim_bone_sniper, "head\0neck\0chest\0stomach\0pelvis");
						}
						ImGui::SliderFloat("sniper fov", &c_config::get().aim_fov_sniper, 0.0f, 180.0f, "%.2f");
						ImGui::SliderFloat("sniper smooth", &c_config::get().aim_smooth_sniper, 1.f, 10.f, "%.2f");
						ImGui::SliderFloat("sniper rcs x", &c_config::get().rcs_x_sniper, 0.0f, 1.0f, "%.2f");
						ImGui::SliderFloat("sniper rcs y", &c_config::get().rcs_y_sniper, 0.0f, 1.0f, "%.2f");
						break;
					case 3:
						if (c_config::get().aim_mode == 0) {
							ImGui::Combo("smg hitbox", &c_config::get().aim_bone_smg, "head\0neck\0chest\0stomach\0pelvis");
						}

						ImGui::SliderFloat("smg fov", &c_config::get().aim_fov_smg, 0.0f, 180.0f, "%.2f");
						ImGui::SliderFloat("smg smooth", &c_config::get().aim_smooth_smg, 1.f, 10.f, "%.2f");
						ImGui::SliderFloat("smg rcs x", &c_config::get().rcs_x_smg, 0.0f, 1.0f, "%.2f");
						ImGui::SliderFloat("smg rcs y", &c_config::get().rcs_y_smg, 0.0f, 1.0f, "%.2f");
						break;
					case 4:
						if (c_config::get().aim_mode == 0) {
							ImGui::Combo("heavy hitbox", &c_config::get().aim_bone_heavy, "head\0neck\0chest\0stomach\0pelvis");
						}
						ImGui::SliderFloat("heavy fov", &c_config::get().aim_fov_heavy, 0.0f, 180.0f, "%.2f");
						ImGui::SliderFloat("heavy smooth", &c_config::get().aim_smooth_heavy, 1.f, 10.f, "%.2f");
						ImGui::SliderFloat("heavy rcs x", &c_config::get().rcs_x_heavy, 0.0f, 1.0f, "%.2f");
						ImGui::SliderFloat("heavy rcs y", &c_config::get().rcs_y_heavy, 0.0f, 1.0f, "%.2f");
						break;
					}

					ImGui::PopFont();
				} ImGui::EndChild(true);
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::PopStyleVar();

				break;
			case 1:
				ImGui::Columns(2, NULL, false);

				ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
				ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();

				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
				//push window color for child
				ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(30 / 255.f, 30 / 255.f, 39 / 255.f, 1.0f));
				//push border color for child
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0 / 255.f, 0 / 255.f, 0 / 255.f, 0.1f));

				ImGui::BeginChild("player", ImVec2(279, 268), true);
				{
					ImGui::Checkbox("active", &c_config::get().visuals_enabled);
					if (c_config::get().visuals_enabled) {
						ImGui::Checkbox("team", &c_config::get().visuals_team_check);
					}
					ImGui::Checkbox("name", &c_config::get().player_name);
					ImGui::ColorPickerSpacing();
					ImGui::ColorEdit4("name color", c_config::get().clr_name, ImGuiColorEditFlags_NoInputs);
					ImGui::Checkbox("box", &c_config::get().player_box);
					ImGui::ColorPickerSpacing();
					ImGui::ColorEdit4("box color", c_config::get().clr_box, ImGuiColorEditFlags_NoInputs);
					ImGui::Checkbox("health", &c_config::get().player_health);
					ImGui::Checkbox("weapon", &c_config::get().player_weapon);
					ImGui::ColorPickerSpacing();
					ImGui::ColorEdit4("weapon color", c_config::get().clr_weapon, ImGuiColorEditFlags_NoInputs);

					if (ImGui::BeginCombo("flags", "...", ImVec2(0, 105)))
					{
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 8);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
						ImGui::Selectable(("armor"), &c_config::get().player_flags_armor, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
						ImGui::Selectable(("bomb"), &c_config::get().player_flags_c4, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
						ImGui::Selectable(("flashed"), &c_config::get().player_flags_flashed, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
						ImGui::Selectable(("money"), &c_config::get().player_flags_money, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
						ImGui::Selectable(("scoped"), &c_config::get().player_flags_scoped, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);

						ImGui::EndCombo();
					}
					ImGui::Checkbox("footstep", &c_config::get().sound_footstep);
					ImGui::ColorPickerSpacing();
					ImGui::ColorEdit4("footstep color", c_config::get().clr_footstep, ImGuiColorEditFlags_NoInputs);
					ImGui::Checkbox("skeleton", &c_config::get().skeleton);
					ImGui::Checkbox("backtrack skeleton", &c_config::get().backtrack_skeleton);
					ImGui::Checkbox("preview visuals", &c_config::get().visuals_preview);
				}
				ImGui::EndChild(true);

				ImGui::PopStyleVar();
				ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
				ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));

				ImGui::BeginChild("effects", ImVec2(279, 267), true);
				{
					ImGui::Checkbox("force crosshair", &c_config::get().force_crosshair);
					ImGui::SliderInt("viewmodel field of view", &c_config::get().viewmodel_fov, 0, 135);
					ImGui::SliderInt("field of view", &c_config::get().fov, 0, 60);
					ImGui::Checkbox("night mode", &c_config::get().nightmode);
					ImGui::ColorPickerSpacing();
					ImGui::ColorEdit4("sky color", c_config::get().clr_sky, ImGuiColorEditFlags_NoInputs);
					if (c_config::get().nightmode) {
						ImGui::SliderInt("brightness", &c_config::get().nightmode_brightness, 0, 100);
					}


					if (ImGui::BeginCombo("removals", "...", ImVec2(0, 105)))
					{
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 8);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
						ImGui::Selectable(("smoke"), &c_config::get().remove_smoke, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
						ImGui::Selectable(("flash"), &c_config::get().reduce_flash, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
						ImGui::Selectable(("sleeves"), &c_config::get().remove_sleeves, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
						ImGui::Selectable(("hands"), &c_config::get().remove_hands, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
						ImGui::Selectable(("scope overlay"), &c_config::get().remove_scope, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						ImGui::EndCombo();
					}

					if (ImGui::BeginCombo("world", "...", ImVec2(0, 105)))
					{
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 8);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
						ImGui::Selectable(("planted bomb"), &c_config::get().bomb_planted, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
						ImGui::Selectable(("dropped weapons"), &c_config::get().dropped_weapons, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
						ImGui::Selectable(("projectiles"), &c_config::get().projectiles, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
						ImGui::Selectable(("misc entities"), &c_config::get().entity_esp, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
						ImGui::Selectable(("danger zone"), &c_config::get().danger_zone_dropped, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						ImGui::EndCombo();
					}
				}
				ImGui::EndChild(true);

				ImGui::NextColumn();

				ImGui::BeginChild("glow", ImVec2(279, 268), true);
				{
					ImGui::Checkbox("active", &c_config::get().visuals_glow);
					ImGui::Checkbox("enemy", &c_config::get().visuals_glow_enemy);
					ImGui::ColorPickerSpacing();
					ImGui::ColorEdit4("glow color", c_config::get().clr_glow, ImGuiColorEditFlags_NoInputs);


					ImGui::Checkbox("teammate", &c_config::get().visuals_glow_team);
					ImGui::ColorPickerSpacing();
					ImGui::ColorEdit4("glow color team", c_config::get().clr_glow_team, ImGuiColorEditFlags_NoInputs);


					ImGui::Checkbox("planted bomb", &c_config::get().visuals_glow_planted);
					ImGui::ColorPickerSpacing();
					ImGui::ColorEdit4("glow color planted", c_config::get().clr_glow_planted, ImGuiColorEditFlags_NoInputs);


					ImGui::Checkbox("dropped weapons", &c_config::get().visuals_glow_weapons);
					ImGui::ColorPickerSpacing();
					ImGui::ColorEdit4("glow color weapons", c_config::get().clr_glow_dropped, ImGuiColorEditFlags_NoInputs);

				}
				ImGui::EndChild(true);

				ImGui::Spacing();

				ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 8);

				ImGui::BeginChild("chams", ImVec2(279, 267), true);
				{
					ImGui::Combo("chams type", &c_config::get().vis_chams_type, "textured\0flat\0metallic\0pulsating");

					ImGui::Checkbox("enemy", &c_config::get().vis_chams_vis);
					ImGui::ColorPickerSpacing();
					ImGui::ColorEdit4("enemy color", c_config::get().clr_chams_vis, ImGuiColorEditFlags_NoInputs);

					ImGui::Checkbox("enemy (behind wall)", &c_config::get().vis_chams_invis);
					ImGui::ColorPickerSpacing();
					ImGui::ColorEdit4("enemy (behind wall) color", c_config::get().clr_chams_invis, ImGuiColorEditFlags_NoInputs);

					ImGui::Checkbox("backtrack", &c_config::get().backtrack_visualize);
				}
				ImGui::EndChild(true);

				ImGui::PopStyleVar();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::Columns();
				break;
			case 2:
				ImGui::Columns(2, NULL, false);

				ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
				ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
				//push window color for child
				ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(30 / 255.f, 30 / 255.f, 39 / 255.f, 1.0f));
				//push border color for child
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0 / 255.f, 0 / 255.f, 0 / 255.f, 0.1f));

				ImGui::BeginChild("misc", ImVec2(279, 543), true); {
					ImGui::Checkbox("active", &c_config::get().misc_enabled);
					ImGui::Checkbox("clantag spammer", &c_config::get().clan_tag);
					ImGui::Checkbox("engine radar", &c_config::get().radar);

					//static std::string preview = "";
					//if (ImGui::BeginCombo("logs", preview.c_str(), ImVec2(0, 65))) {
					if (ImGui::BeginCombo("logs", "...", ImVec2(0, 65))) {
						//std::vector<std::string> vector;
						//preview = "";

						ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 8);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
						ImGui::Selectable(("player hurt"), &c_config::get().logs_player_hurt, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						//if (c_config::get().logs_player_hurt)
						//	vector.push_back("player hurt");
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
						ImGui::Selectable(("player bought"), &c_config::get().logs_player_bought, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						//if (c_config::get().logs_player_bought)
						//	vector.push_back("player bought");
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
						ImGui::Selectable(("config system"), &c_config::get().logs_config_system, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						//if (c_config::get().logs_config_system)
						//	vector.push_back("config system");

						/*for (size_t i = 0; i < vector.size(); i++)
						{
							if (vector.size() == 1)
								preview += vector.at(i);
							else if (!(i == vector.size() - 1))
								preview += vector.at(i) + ", ";
							else
								preview += vector.at(i);
						} */

						ImGui::EndCombo();
					}

					ImGui::Checkbox("hitmarker", &c_config::get().hitmarker);
					if (c_config::get().hitmarker) {
						ImGui::Combo("hitmarker sound", &c_config::get().hitmarker_sound, "none\0one\0two\0three");
					}
					ImGui::Checkbox("anti screenshot", &c_config::get().anti_screenshot);
					ImGui::Checkbox("spectators", &c_config::get().spectators_list);
					ImGui::Checkbox("watermark", &c_config::get().watermark);


					ImGui::Checkbox("viewmodel offset", &c_config::get().viewmodel_offset);
					if (c_config::get().viewmodel_offset) {
						ImGui::SliderInt("viewmodel x", &c_config::get().viewmodel_x, -10, 10);
						ImGui::SliderInt("viewmodel y", &c_config::get().viewmodel_y, -10, 10);
						ImGui::SliderInt("viewmodel z", &c_config::get().viewmodel_z, -10, 10);

					}

					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 18);
					if (ImGui::Button("dump steam id", ImVec2(84, 18))) {
						utilities::dump_steam_id();
					}

					ImGui::SameLine();
					if (ImGui::Button("hide name", ImVec2(84, 18))) {
						utilities::change_name("\n\xAD\xAD\xAD");
					}

				}
				ImGui::EndChild(true);
				ImGui::NextColumn();

				ImGui::BeginChild("movement", ImVec2(279, 543), true);
				{
					ImGui::Checkbox("bunny hop", &c_config::get().bunny_hop);
					ImGui::Checkbox("edge jump", &c_config::get().edge_jump);
					if (c_config::get().edge_jump) {
						ImGui::Checkbox("duck in air", &c_config::get().edge_jump_duck_in_air);
					}
				}
				ImGui::EndChild(true);
				ImGui::PopStyleVar();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::Columns();
				break;
			case 3:
				ImGui::Columns(2, NULL, false);

				ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
				ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();

				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
				//push window color for child
				ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(30 / 255.f, 30 / 255.f, 39 / 255.f, 1.0f));
				//push border color for child
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0 / 255.f, 0 / 255.f, 0 / 255.f, 0.1f));
				ImGui::BeginChild("knife", ImVec2(279, 268), true);
				{
					ImGui::Checkbox("enable", &c_config::get().skinchanger_enable);
					ImGui::Combo("knife", &c_config::get().knife_model, "default\0bayonet\0m9\0karambit\0bowie\0butterfly\0falchion\0flip\0gut\0huntsman\0shaddow daggers\0navaja\0stiletto\0talon\0ursus");
					ImGui::Combo("condition", &c_config::get().knife_wear, "factory new\0minimal wear\0field-tested\0well-worn\0battle-scarred");

					ImGui::Combo(("skin"), &c_config::get().paint_kit_vector_index_knife, [](void* data, int idx, const char** out_text)
					{
						*out_text = k_skins[idx].name.c_str();
						return true;
					}, nullptr, k_skins.size(), 10);
					c_config::get().paint_kit_index_knife = k_skins[c_config::get().paint_kit_vector_index_knife].id;

				}
				ImGui::EndChild(true);

				ImGui::PopStyleVar();
				ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
				ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));

				ImGui::BeginChild("settings", ImVec2(279, 267), true);
				{
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 18);
					if (ImGui::Button("force update", ImVec2(84, 18))) {
						utilities::force_update();
					}

				}
				ImGui::EndChild(true);

				ImGui::NextColumn();

				ImGui::BeginChild("weapons", ImVec2(279, 543), true);
				{
					static int weapons_page = 0;
					ImGui::PushFont(Icons);
					if (ImGui::ButtonT("A", ImVec2(50, 30), weapons_page, 0, false, ImColor(0, 0, 0))) weapons_page = 0; ImGui::SameLine(0, 0); //pistol
					if (ImGui::ButtonT("W", ImVec2(50, 30), weapons_page, 1, false, false)) weapons_page = 1; ImGui::SameLine(0, 0); //rifle
					if (ImGui::ButtonT("Z", ImVec2(50, 30), weapons_page, 2, false, false)) weapons_page = 2; ImGui::SameLine(0, 0); //sniper
					if (ImGui::ButtonT("L", ImVec2(50, 30), weapons_page, 3, false, false)) weapons_page = 3; ImGui::SameLine(0, 0); //mg
					if (ImGui::ButtonT("f", ImVec2(50, 30), weapons_page, 4, false, false)) weapons_page = 4; //heavy
					ImGui::PopFont();

					switch (weapons_page) {
					case 0:
						ImGui::Combo(("p2000"), &c_config::get().paint_kit_vector_index_p2000, [](void* data, int idx, const char** out_text)
						{
							*out_text = k_skins[idx].name.c_str();
							return true;
						}, nullptr, k_skins.size(), 10);
						c_config::get().paint_kit_index_p2000 = k_skins[c_config::get().paint_kit_vector_index_p2000].id;


						ImGui::Combo(("usp-s"), &c_config::get().paint_kit_vector_index_usp, [](void* data, int idx, const char** out_text)
						{
							*out_text = k_skins[idx].name.c_str();
							return true;
						}, nullptr, k_skins.size(), 10);
						c_config::get().paint_kit_index_usp = k_skins[c_config::get().paint_kit_vector_index_usp].id;

						ImGui::Combo(("glock"), &c_config::get().paint_kit_vector_index_glock, [](void* data, int idx, const char** out_text)
						{
							*out_text = k_skins[idx].name.c_str();
							return true;
						}, nullptr, k_skins.size(), 10);
						c_config::get().paint_kit_index_glock = k_skins[c_config::get().paint_kit_vector_index_glock].id;

						ImGui::Combo(("p250"), &c_config::get().paint_kit_vector_index_p250, [](void* data, int idx, const char** out_text)
						{
							*out_text = k_skins[idx].name.c_str();
							return true;
						}, nullptr, k_skins.size(), 10);
						c_config::get().paint_kit_index_p250 = k_skins[c_config::get().paint_kit_vector_index_p250].id;

						ImGui::Combo(("five-seven"), &c_config::get().paint_kit_vector_index_fiveseven, [](void* data, int idx, const char** out_text)
						{
							*out_text = k_skins[idx].name.c_str();
							return true;
						}, nullptr, k_skins.size(), 10);
						c_config::get().paint_kit_index_fiveseven = k_skins[c_config::get().paint_kit_vector_index_fiveseven].id;

						ImGui::Combo(("tec9"), &c_config::get().paint_kit_vector_index_tec, [](void* data, int idx, const char** out_text)
						{
							*out_text = k_skins[idx].name.c_str();
							return true;
						}, nullptr, k_skins.size(), 10);
						c_config::get().paint_kit_index_tec = k_skins[c_config::get().paint_kit_vector_index_tec].id;

						ImGui::Combo(("cz75a"), &c_config::get().paint_kit_vector_index_cz, [](void* data, int idx, const char** out_text)
						{
							*out_text = k_skins[idx].name.c_str();
							return true;
						}, nullptr, k_skins.size(), 10);
						c_config::get().paint_kit_index_cz = k_skins[c_config::get().paint_kit_vector_index_cz].id;

						ImGui::Combo(("duals"), &c_config::get().paint_kit_vector_index_duals, [](void* data, int idx, const char** out_text)
						{
							*out_text = k_skins[idx].name.c_str();
							return true;
						}, nullptr, k_skins.size(), 10);
						c_config::get().paint_kit_index_duals = k_skins[c_config::get().paint_kit_vector_index_duals].id;

						ImGui::Combo(("deagle"), &c_config::get().paint_kit_vector_index_deagle, [](void* data, int idx, const char** out_text)
						{
							*out_text = k_skins[idx].name.c_str();
							return true;
						}, nullptr, k_skins.size(), 10);
						c_config::get().paint_kit_index_deagle = k_skins[c_config::get().paint_kit_vector_index_deagle].id;

						ImGui::Combo(("revolver"), &c_config::get().paint_kit_vector_index_revolver, [](void* data, int idx, const char** out_text)
						{
							*out_text = k_skins[idx].name.c_str();
							return true;
						}, nullptr, k_skins.size(), 10);
						c_config::get().paint_kit_index_revolver = k_skins[c_config::get().paint_kit_vector_index_revolver].id;

						break;
					case 1:
						ImGui::Combo(("famas"), &c_config::get().paint_kit_vector_index_famas, [](void* data, int idx, const char** out_text)
						{
							*out_text = k_skins[idx].name.c_str();
							return true;
						}, nullptr, k_skins.size(), 10);
						c_config::get().paint_kit_index_famas = k_skins[c_config::get().paint_kit_vector_index_famas].id;

						ImGui::Combo(("galil"), &c_config::get().paint_kit_vector_index_galil, [](void* data, int idx, const char** out_text)
						{
							*out_text = k_skins[idx].name.c_str();
							return true;
						}, nullptr, k_skins.size(), 10);
						c_config::get().paint_kit_index_galil = k_skins[c_config::get().paint_kit_vector_index_galil].id;

						ImGui::Combo(("m4a4"), &c_config::get().paint_kit_vector_index_m4a4, [](void* data, int idx, const char** out_text)
						{
							*out_text = k_skins[idx].name.c_str();
							return true;
						}, nullptr, k_skins.size(), 10);
						c_config::get().paint_kit_index_m4a4 = k_skins[c_config::get().paint_kit_vector_index_m4a4].id;

						ImGui::Combo(("m4a1"), &c_config::get().paint_kit_vector_index_m4a1, [](void* data, int idx, const char** out_text)
						{
							*out_text = k_skins[idx].name.c_str();
							return true;
						}, nullptr, k_skins.size(), 10);
						c_config::get().paint_kit_index_m4a1 = k_skins[c_config::get().paint_kit_vector_index_m4a1].id;

						ImGui::Combo(("ak47"), &c_config::get().paint_kit_vector_index_ak47, [](void* data, int idx, const char** out_text)
						{
							*out_text = k_skins[idx].name.c_str();
							return true;
						}, nullptr, k_skins.size(), 10);
						c_config::get().paint_kit_index_ak47 = k_skins[c_config::get().paint_kit_vector_index_ak47].id;

						ImGui::Combo(("sg 553"), &c_config::get().paint_kit_vector_index_sg553, [](void* data, int idx, const char** out_text)
						{
							*out_text = k_skins[idx].name.c_str();
							return true;
						}, nullptr, k_skins.size(), 10);
						c_config::get().paint_kit_index_sg553 = k_skins[c_config::get().paint_kit_vector_index_sg553].id;

						ImGui::Combo(("aug"), &c_config::get().paint_kit_vector_index_aug, [](void* data, int idx, const char** out_text)
						{
							*out_text = k_skins[idx].name.c_str();
							return true;
						}, nullptr, k_skins.size(), 10);
						c_config::get().paint_kit_index_aug = k_skins[c_config::get().paint_kit_vector_index_aug].id;

						break;
					case 2:

						ImGui::Combo(("ssg08"), &c_config::get().paint_kit_vector_index_ssg08, [](void* data, int idx, const char** out_text)
						{
							*out_text = k_skins[idx].name.c_str();
							return true;
						}, nullptr, k_skins.size(), 10);
						c_config::get().paint_kit_index_ssg08 = k_skins[c_config::get().paint_kit_vector_index_ssg08].id;

						ImGui::Combo(("awp"), &c_config::get().paint_kit_vector_index_awp, [](void* data, int idx, const char** out_text)
						{
							*out_text = k_skins[idx].name.c_str();
							return true;
						}, nullptr, k_skins.size(), 10);
						c_config::get().paint_kit_index_awp = k_skins[c_config::get().paint_kit_vector_index_awp].id;

						ImGui::Combo(("scar20"), &c_config::get().paint_kit_vector_index_scar, [](void* data, int idx, const char** out_text)
						{
							*out_text = k_skins[idx].name.c_str();
							return true;
						}, nullptr, k_skins.size(), 10);
						c_config::get().paint_kit_index_scar = k_skins[c_config::get().paint_kit_vector_index_scar].id;

						ImGui::Combo(("g3sg1"), &c_config::get().paint_kit_vector_index_g3sg1, [](void* data, int idx, const char** out_text)
						{
							*out_text = k_skins[idx].name.c_str();
							return true;
						}, nullptr, k_skins.size(), 10);
						c_config::get().paint_kit_index_g3sg1 = k_skins[c_config::get().paint_kit_vector_index_g3sg1].id;

						break;
					case 3:
						break;
					case 4:
						break;

					}
				}
				ImGui::EndChild(true);
				ImGui::PopStyleVar();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::Columns();
				break;

			case 4:
				ImGui::Columns(2, NULL, false);

				ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
				ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();

				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
				//push window color for child
				ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(30 / 255.f, 30 / 255.f, 39 / 255.f, 1.0f));
				//push border color for child
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0 / 255.f, 0 / 255.f, 0 / 255.f, 0.1f));
				ImGui::BeginChild("config", ImVec2(279, 268), true);
				{
					ImGui::Combo("config", &c_config::get().config_selection, "alpha\0beta\0gamma");
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 18);
					if (ImGui::Button("save", ImVec2(84, 18))) {
						c_config_saving::get().save();
						save_config = true;

						if (c_config::get().logs_config_system) {
							utilities::console_warning("[config system] ");
							interfaces::console->console_printf("config saved. \n");
							c_event_logs::get().add("config saved.", color(167, 255, 255, 255));
						}
					}

					ImGui::SameLine();
					if (ImGui::Button("load", ImVec2(83, 18))) {
						c_config_saving::get().load();
						load_config = true;

						if (c_config::get().logs_config_system) {
							utilities::console_warning("[config system] ");
							interfaces::console->console_printf("config loaded. \n");
							c_event_logs::get().add("config loaded.", color(167, 255, 255, 255));
						}
					}
				}
				ImGui::EndChild(true);

				ImGui::PopStyleVar();
				ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
				ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));

				ImGui::BeginChild("settings", ImVec2(279, 267), true);
				{
					ImGui::Combo("keybinds", &c_config::get().keybinds_selection, "edge jump\0load config: alpha\0load config: beta\0load config: gamma");

					if (c_config::get().keybinds_selection == 0) {
						ImGui::Hotkey("##edge jump key", &c_config::get().edge_jump_key, ImVec2(100, 20));
					}
				}
				ImGui::EndChild(true);

				ImGui::NextColumn();

				ImGui::BeginChild("info", ImVec2(279, 543), true);
				{
					char buffer[UNLEN + 1];
					DWORD size;
					size = sizeof(buffer);
					GetUserName(buffer, &size);
					char title[UNLEN];
					char ch1[25] = "welcome, ";
					char *ch = strcat(ch1, buffer);

					ImGui::Text(ch);
					ImGui::Text("build: " __DATE__ " / " __TIME__);
					if (std::strstr(GetCommandLineA(), "-insecure")) {
						ImGui::Text("insecure mode!");
					}
				}
				ImGui::EndChild(true);
				ImGui::PopStyleVar();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::Columns();
				break;
			}
		}

		ImGui::PopFont();

		ImGui::End();
	}
}

void c_menu::run_popup() {
	ImGui::PushFont(Menu);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
	ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(30 / 255.f, 30 / 255.f, 39 / 255.f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0 / 255.f, 0 / 255.f, 0 / 255.f, 0.1f));

	if (save_config) {
		bool done = false;
		ImGui::Popup("config saved.", 2000, &done);
		if (done)
			save_config = false;
	}

	if (load_config) {
		bool done = false;
		ImGui::Popup("config loaded.", 2000, &done);
		if (done)
			load_config = false;
	}

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
	ImGui::PopFont();
}

void c_menu::run_visuals_preview() {
	static std::vector<esp_info_s> info;
	static bool enabled = true;

	ImGui::SetNextWindowSize(ImVec2(235, 400));

	ImGui::Begin("ESP Preview", &enabled, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);
	{
		const auto cur_window = ImGui::GetCurrentWindow();
		const ImVec2 w_pos = cur_window->Pos;

		ImVec2 p = ImGui::GetCursorScreenPos();
		ImColor c = ImColor(32, 114, 247);

		//title bar
		ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x - 20, p.y + 15), ImVec2(p.x + ImGui::GetWindowWidth(), p.y - 40), ImColor(30, 30, 39));

		//draw gradient bellow title bar
		ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(p.x - 20, p.y + 14), ImVec2(p.x + ImGui::GetWindowWidth(), p.y + 16), ImColor(167, 24, 71, 255), ImColor(58, 31, 87, 255), ImColor(58, 31, 87, 255), ImColor(167, 24, 71, 255));

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 9); //góra, dół
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8); //lewo prawo
		//render cheat name
		ImGui::Text("visuals preview");


		if (c_config::get().player_box) {
			cur_window->DrawList->AddRect(ImVec2(w_pos.x + 40, w_pos.y + 60), ImVec2(w_pos.x + 200, w_pos.y + 360), ImGui::GetColorU32(ImGuiCol_Text));
		}

		if (c_config::get().player_health)
			cur_window->DrawList->AddRectFilled(ImVec2(w_pos.x + 34, w_pos.y + 60), ImVec2(w_pos.x + 36, w_pos.y + 360), ImGui::GetColorU32(ImVec4(83 / 255.f, 200 / 255.f, 84 / 255.f, 255 / 255.f)));

		if (c_config::get().player_name)
			info.emplace_back(esp_info_s("name", color(255, 255, 255, 255), CENTER_UP));

		if (c_config::get().player_weapon)
			info.emplace_back(esp_info_s("awp", color(255, 255, 255, 255), CENTER_DOWN));

		if (c_config::get().player_flags_armor)
			info.emplace_back(esp_info_s("hk", color(255, 255, 255, 255), RIGHT));


		for (auto render : info)
		{
			const auto text_size = ImGui::CalcTextSize(render.f_name.c_str());

			auto pos = ImVec2(w_pos.x + 205, w_pos.y + 60);

			if (render.f_position == CENTER_DOWN)
			{
				pos = ImVec2(w_pos.x + (240 / 2) - text_size.x / 2, pos.y + 315 - text_size.y);
			}
			else if (render.f_position == CENTER_UP)
			{
				pos = ImVec2(w_pos.x + (240 / 2) - text_size.x / 2, pos.y - 5 - text_size.y);
			}

			cur_window->DrawList->AddText(pos, ImGui::GetColorU32(ImVec4(255 / 255.f, 255 / 255.f, 255 / 255.f, 255 / 255.f)), render.f_name.c_str());
		}
	}
	ImGui::End();

	info.clear();
}