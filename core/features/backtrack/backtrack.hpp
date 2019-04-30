#pragma once
#include "../../../dependencies/common_includes.hpp"
#include "../../../dependencies/math/math.hpp"
#include <deque>
#include <list>

struct backtrack_data {
	float simtime;
	vec3_t hitbox_position;
	int tick_count;
	matrix_t bone_matrix[128];
};

extern backtrack_data bone_data[64][12];
extern backtrack_data entity_data[64][12];

class c_backtrack : public singleton<c_backtrack>
{
public:
	void run(c_usercmd* user_cmd);
protected:
};