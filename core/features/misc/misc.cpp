#include "misc.hpp"

void c_misc::remove_smoke() {
	if (!c_config::get().remove_smoke || !c_config::get().visuals_enabled)
		return;

	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
		return;

	static auto smoke_count = *reinterpret_cast<uint32_t **>(utilities::pattern_scan(GetModuleHandleA("client_panorama.dll"), "A3 ? ? ? ? 57 8B CB") + 1);

	static std::vector<const char*> smoke_materials = {
		"particle/vistasmokev1/vistasmokev1_fire",
		"particle/vistasmokev1/vistasmokev1_smokegrenade",
		"particle/vistasmokev1/vistasmokev1_emods",
		"particle/vistasmokev1/vistasmokev1_emods_impactdust"
	};

	for (auto material_name : smoke_materials) {
		i_material * smoke = interfaces::material_system->find_material(material_name, TEXTURE_GROUP_OTHER);
		smoke->increment_reference_count();
		smoke->set_material_var_flag(MATERIAL_VAR_WIREFRAME, true);

		*(int*)smoke_count = 0;
	}
}

void c_misc::remove_flash() {
	if (!c_config::get().reduce_flash || !c_config::get().visuals_enabled)
		return;

	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	if (local_player->flash_duration() > 0.0f)
		local_player->flash_duration() = 0.0f;
}

void c_misc::remove_scope() {
	if (!c_config::get().remove_scope || !c_config::get().visuals_enabled)
		return;

	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	if (local_player && !local_player->is_scoped())
		return;

	int w, h;
	interfaces::engine->get_screen_size(w, h);
	interfaces::surface->set_drawing_color(0, 0, 0, 255);
	interfaces::surface->draw_line(0, h / 2, w, h / 2);
	interfaces::surface->draw_line(w / 2, 0, w / 2, h);
}

void c_misc::spectators() {
	if (!c_config::get().spectators_list || !c_config::get().misc_enabled)
		return;

	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
		return;

	int spectator_index = 0;
	int width, height;
	interfaces::engine->get_screen_size(width, height);

	render::get().draw_text(width - 80, height / 2 - 10, render::get().name_font, "spectators", true, color(255, 255, 255));
	for (int i = 0; i < interfaces::entity_list->get_highest_index(); i++) {
		auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		player_info_t info;

		if (entity && entity != local_player) {
			interfaces::engine->get_player_info(i, &info);
			if (!entity->is_alive() && !entity->dormant()) {
				auto target = entity->observer_target();
				if (target) {
					auto spectator_target = interfaces::entity_list->get_client_entity_handle(target);
					if (spectator_target == local_player) {
						int x, y;
						interfaces::surface->get_text_size(render::get().name_font, utilities::to_wchar(info.name), x, y);
						std::string player_name = info.name;
						std::transform(player_name.begin(), player_name.end(), player_name.begin(), ::tolower);
						player_info_t spectator_info;
						interfaces::engine->get_player_info(i, &spectator_info);
						render::get().draw_text(width - 80, height / 2 + (10 * spectator_index), render::get().name_font, player_name.c_str(), true, color(255, 255, 255));
						spectator_index++;
					}
				}
			}
		}
	}
}

void c_misc::watermark() {
	if (!c_config::get().watermark || !c_config::get().misc_enabled)
		return;

	int width, height;
	interfaces::engine->get_screen_size(width, height);

	static int fps, old_tick_count;

	if ((interfaces::globals->tick_count - old_tick_count) > 50) {
		fps = static_cast<int>(1.f / interfaces::globals->frame_time);
		old_tick_count = interfaces::globals->tick_count;
	}
	std::stringstream ss;

	auto net_channel = interfaces::engine->get_net_channel_info();
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	std::string incoming = local_player ? std::to_string((int)(net_channel->get_latency(FLOW_INCOMING) * 1000)) : "0";
	std::string outgoing = local_player ? std::to_string((int)(net_channel->get_latency(FLOW_OUTGOING) * 1000)) : "0";

	ss << "aristois.me | fps: " << fps << " | incoming: " << incoming.c_str() << "ms" << " | outgoing: " << outgoing.c_str() << "ms";

	render::get().draw_filled_rect(width - 275, 4, 260, 20, color(33, 35, 47, 255));
	render::get().draw_outline(width - 275, 4, 260, 20, color(30, 30, 41, 255));
	render::get().draw_text(width - 270, 7, render::get().watermark_font, ss.str().c_str(), false, color(255, 255, 255, 255));
}

void c_misc::clantag_spammer() {
	if (!c_config::get().clan_tag || !c_config::get().misc_enabled)
		return;

	static std::string tag = "    aristois.me    ";
	static float last_time = 0;

	if (interfaces::globals->cur_time > last_time) {
		tag += tag.at(0);
		tag.erase(0, 1);
		utilities::apply_clan_tag(tag.c_str());

		last_time = interfaces::globals->cur_time + 0.9f;
	}

	if (fabs(last_time - interfaces::globals->cur_time) > 1.f)
		last_time = interfaces::globals->cur_time;
}

void c_misc::viewmodel_offset() {
	if (!c_config::get().viewmodel_offset || !c_config::get().misc_enabled)
		return;

	interfaces::console->get_convar("viewmodel_offset_x")->set_value(c_config::get().viewmodel_x);
	interfaces::console->get_convar("viewmodel_offset_y")->set_value(c_config::get().viewmodel_y);
	interfaces::console->get_convar("viewmodel_offset_z")->set_value(c_config::get().viewmodel_z);
}