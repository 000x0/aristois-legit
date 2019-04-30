#include "backtrack.hpp"
#define TICK_INTERVAL (interfaces::globals->interval_per_tick)
#define TIME_TO_TICKS(dt) ((int)(0.5f + (float)(dt) / TICK_INTERVAL))
#define MAXSTUDIOBONES		128
#define BONE_USED_BY_HITBOX             0x00000100

backtrack_data bone_data[64][12];
backtrack_data entity_data[64][12];

void c_backtrack::run(c_usercmd* user_cmd) {
	if (!c_config::get().backtrack)
		return;

	auto best_target = -1;
	auto best_fov = FLT_MAX;

	player_info_t info;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	if (!local_player->is_alive())
		return;

	vec3_t view_direction = c_math::get().angle_vector(user_cmd->viewangles + (local_player->punch_angle() * 2.f));

	for (int i = 1; i < interfaces::globals->max_clients; i++) {
		auto entity = (player_t*)interfaces::entity_list->get_client_entity(i);

		interfaces::engine->get_player_info(i, &info);

		if (!local_player)
			continue;

		if (!entity)
			continue;

		if (entity == local_player)
			continue;

		if (entity->dormant())
			continue;

		if (entity->team() == local_player->team())
			continue;

		if (!entity->setup_bones(bone_data[i][user_cmd->command_number % 12].bone_matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, interfaces::globals->cur_time))
			continue;

		if (entity->is_alive()) {
			float simtime = entity->simulation_time(); 

			vec3_t hitbox_position = local_player->get_hitbox_position(entity, hitbox_head); //this

			entity_data[i][user_cmd->command_number % 12] = backtrack_data{ simtime, hitbox_position }; 

			float fov_distance = c_math::get().point_to_line(hitbox_position, local_player->eye_pos(), view_direction);

			if (best_fov > fov_distance) {
				best_fov = fov_distance;
				best_target = i;
			}
		}
	}

	float best_simtime;

	if (best_target != -1) {
		float temporary_float = FLT_MAX;

		for (int t = 0; t < 12; ++t) {
			float temp_fov_distance = c_math::get().point_to_line(entity_data[best_target][t].hitbox_position, local_player->eye_pos(), view_direction);

			if (temporary_float > temp_fov_distance && entity_data[best_target][t].simtime > local_player->simulation_time() - 1) {
				temporary_float = temp_fov_distance;
				best_simtime = entity_data[best_target][t].simtime;
			}
		}

		if (user_cmd->buttons & in_attack)
			user_cmd->tick_count = TIME_TO_TICKS(best_simtime);
	}
}