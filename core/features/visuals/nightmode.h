#pragma once

#include "../../../dependencies/common_includes.hpp"
#include <deque>
#include <list>

class MaterialBackup {
public:
	MaterialHandle_t handle;
	i_material* material;
	float color[3];
	float alpha;
	bool translucent;
	bool nodraw;

	MaterialBackup() {
	}

	MaterialBackup(MaterialHandle_t h, i_material* p) {
		handle = handle;
		material = p;

		material->get_color_modulation(&color[0], &color[1], &color[2]);
		alpha = material->get_alpha_modulation();

		translucent = material->get_material_var_flag(MATERIAL_VAR_TRANSLUCENT);
		nodraw = material->get_material_var_flag(MATERIAL_VAR_NO_DRAW);
	}

	void restore() {
		material->color_modulate(color[0], color[1], color[2]);
		material->alpha_modulate(alpha);
		material->set_material_var_flag(MATERIAL_VAR_TRANSLUCENT, translucent);
		material->set_material_var_flag(MATERIAL_VAR_NO_DRAW, nodraw);
	}
};

class c_nightmode : public singleton< c_nightmode > {
public:
	void clear_stored_materials();
	void modulate(MaterialHandle_t i, i_material *material, bool backup);
	void apply();
	void remove();
};