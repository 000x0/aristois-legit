#pragma once
#include "../../../dependencies/common_includes.hpp"

class c_sound_info {
public:
	c_sound_info(vec3_t positions, float times, int userids) {
		this->position = positions;
		this->time = times;
		this->userid = userids;
	}

	vec3_t position;
	float time;
	int userid;
};

class c_sound_esp : public singleton<c_sound_esp> {
public:
	void draw();
	void draw_circle(color colors, vec3_t position);
	void event_player_footstep(i_game_event * event);
	void event_player_hurt(i_game_event * event);
};