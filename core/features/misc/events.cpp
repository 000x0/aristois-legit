#include "events.hpp"
#include "../misc/logs.hpp"
#include "../misc/hitmarker.hpp"
#include "../visuals/sound.hpp"

#pragma comment(lib, "Winmm.lib")

void c_hooked_events::fire_game_event(i_game_event* event) {
	auto event_name = event->get_name();

	if (!strcmp(event_name, "player_hurt")) {
		c_hitmarker::get().event(event);
		c_event_logs::get().event_player_hurt(event);
		c_sound_esp::get().event_player_hurt(event);
	}

	else if (strstr(event->get_name(), "item_purchase")) {
		c_event_logs::get().event_item_purchase(event);
	}

	else if (strstr(event->get_name(), "player_footstep")) {
		c_sound_esp::get().event_player_footstep(event);
	}
}

int c_hooked_events::get_event_debug_id(void) {
	return EVENT_DEBUG_ID_INIT;
}

void c_hooked_events::setup() {
	m_i_debug_id = EVENT_DEBUG_ID_INIT;
	interfaces::event_manager->add_listener(this, "player_hurt", false);
	interfaces::event_manager->add_listener(this, "item_purchase", false);
	interfaces::event_manager->add_listener(this, "player_footstep", false);

	printf("Events initialized!\n");
}

void c_hooked_events::release() {
	interfaces::event_manager->remove_listener(this);
}
