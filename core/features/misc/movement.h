#pragma once
#include "../../../dependencies/utilities/singleton.hpp"

class c_movement : public singleton< c_movement > {
public:
	void bunnyhop(c_usercmd* user_cmd);
	void edge_jump_pre_prediction(c_usercmd * user_cmd);
	void edge_jump_post_prediction(c_usercmd * user_cmd);
};