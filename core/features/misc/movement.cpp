#include "../../../dependencies/common_includes.hpp"
#include "movement.h"
auto flags_backup = 0;
auto jumped_last_tick = false;
auto should_fake_jump = false;

void c_movement::bunnyhop(c_usercmd* user_cmd) {
	if (!c_config::get().bunny_hop || !c_config::get().misc_enabled)
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	if (!local_player || !local_player->is_alive())
		return;

	if (local_player->move_type() == movetype_ladder)
		return;

	if (!jumped_last_tick && should_fake_jump) {
		should_fake_jump = false;
		user_cmd->buttons |= in_jump;
	}
	else if (user_cmd->buttons & in_jump) {
		if (local_player->flags() & fl_onground) {
			jumped_last_tick = true;
			should_fake_jump = true;
		}
		else {
			user_cmd->buttons &= ~in_jump;
			jumped_last_tick = false;
		}
	}
	else {
		jumped_last_tick = false;
		should_fake_jump = false;
	}
}

void c_movement::edge_jump_pre_prediction(c_usercmd* user_cmd) {
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	if (!c_config::get().edge_jump)
		return;

	if (!GetAsyncKeyState(c_config::get().edge_jump_key))
		return;

	if (!local_player)
		return;

	if (local_player->move_type() == movetype_ladder || local_player->move_type() == movetype_noclip)
		return;

	flags_backup = local_player->flags();
}

void c_movement::edge_jump_post_prediction(c_usercmd* user_cmd) {
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	if (!c_config::get().edge_jump)
		return;

	if (!GetAsyncKeyState(c_config::get().edge_jump_key))
		return;

	if (!local_player)
		return;

	if (local_player->move_type() == movetype_ladder || local_player->move_type() == movetype_noclip)
		return;

	if (flags_backup & fl_onground && !(local_player->flags() & fl_onground))
		user_cmd->buttons |= in_jump;

	if (!(local_player->flags() & fl_onground) && c_config::get().edge_jump_duck_in_air)
		user_cmd->buttons |= in_duck;
}