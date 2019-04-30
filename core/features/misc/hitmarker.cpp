#include "hitmarker.hpp"
#include "../misc/logs.hpp"
int hitmarker_time = 0;

void c_hitmarker::run() {
	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
		return;

	if (c_config::get().hitmarker || c_config::get().hitmarker_sound) {
		c_hitmarker::get().draw();
	}
}

void c_hitmarker::event(i_game_event* event) {
	if (!event)
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	auto attacker = interfaces::entity_list->get_client_entity(interfaces::engine->get_player_for_user_id(event->get_int("attacker")));

	if (attacker == local_player) {
		hitmarker_time = 255;

		switch (c_config::get().hitmarker_sound) {
		case 0:
			break;
		case 1:
			interfaces::surface->play_sound("buttons\\arena_switch_press_02.wav");
			break;
		case 2:
			interfaces::surface->play_sound("survival\\money_collect_01.wav");
			break;
		case 3:
			interfaces::surface->play_sound("survival\\turret_idle_01.wav");
			break;
		}
	}
}

void c_hitmarker::draw() {
	if (!c_config::get().hitmarker)
		return;

	int width, height;
	interfaces::engine->get_screen_size(width, height);
	int width_mid = width / 2;
	int height_mid = height / 2;

	if (hitmarker_time > 0) {
		float alpha = hitmarker_time;

		render::get().draw_line(width_mid - 5, height_mid - 5, width_mid + 5, height_mid + 5, color(255, 255, 255, alpha));
		render::get().draw_line(width_mid - 6, height_mid - 6, width_mid + 6, height_mid + 6, color(255, 255, 255, alpha));
		render::get().draw_line(width_mid + 5, height_mid - 5, width_mid - 5, height_mid + 5, color(255, 255, 255, alpha));
		render::get().draw_line(width_mid + 6, height_mid - 6, width_mid - 6, height_mid + 6, color(255, 255, 255, alpha));

		hitmarker_time -= 2;
	}
}
 