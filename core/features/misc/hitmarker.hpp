#pragma once
#include "../../../dependencies/common_includes.hpp"

class c_hitmarker : public singleton <c_hitmarker> {
public:
	void run();
	void event(i_game_event * event);
protected:
	void draw();
};
