#pragma once
#include "../../../dependencies/common_includes.hpp"

class c_hooked_events : public i_game_event_listener2 {
public:
	void fire_game_event(i_game_event * event);
	void setup();
	void release();
	int get_event_debug_id(void);
};

