#pragma once
#include "../../../dependencies/common_includes.hpp"
class c_skinchanger : public singleton<c_skinchanger> {
public:
	bool apply_knife_model(attributable_item_t * weapon, const char * model);
	bool apply_knife_skin(attributable_item_t * weapon, int item_definition_index, int paint_kit, int model_index, int entity_quality, float fallback_wear);
	void run();
}; 