#include "nightmode.h"

bool executed = false;

convar * sv_skyname = nullptr;
std::string fallback_skybox = "";

std::vector<MaterialBackup> materials;
std::vector<MaterialBackup> skyboxes;

void c_nightmode::clear_stored_materials() {
	fallback_skybox = "";
	sv_skyname = nullptr;
	materials.clear();
	skyboxes.clear();
}

void c_nightmode::modulate(MaterialHandle_t i, i_material *material, bool backup = false) {
	float brightness = c_config::get().nightmode_brightness / 100.f;

	if (strstr(material->GetTextureGroupName(), "World")) {
		if (backup) {
			materials.push_back(MaterialBackup(i, material));
		}

		material->color_modulate(brightness, brightness, brightness);
	}
	else if (strstr(material->GetTextureGroupName(), "StaticProp")) {
		if (backup) {
			materials.push_back(MaterialBackup(i, material));
		}

		material->color_modulate(0.4f, 0.4f, 0.4f);
	}

	else if (strstr(material->GetTextureGroupName(), "SkyBox")) {
		material->color_modulate(c_config::get().clr_sky[0], c_config::get().clr_sky[1], c_config::get().clr_sky[2]);
		material->alpha_modulate(c_config::get().clr_sky[3]);
	}

}

void c_nightmode::apply() {
	if (executed) {
		return;
	}

	executed = true;

	if (!sv_skyname) {
		sv_skyname = interfaces::console->get_convar("sv_skyname");
		sv_skyname->flags &= ~fcvar_cheat;
	}

	sv_skyname->set_value("sky_csgo_night02");

	interfaces::console->get_convar("r_drawspecificstaticprop")->set_value(0);

	if (materials.size()) {
		for (int i = 0; i < (int)materials.size(); i++)
			modulate(materials[i].handle, materials[i].material);

		return;
	}

	materials.clear();

	for (MaterialHandle_t i = interfaces::material_system->first_material(); i != interfaces::material_system->invalid_material_handle(); i = interfaces::material_system->next_material(i)) {
		i_material* material = interfaces::material_system->get_material(i);

		if (!material || material->is_error_material()) {
			continue;
		}

		if (material->get_reference_count() <= 0) {
			continue;
		}

		modulate(i, material, true);
	}
}

void c_nightmode::remove() {
	if (!executed) {
		return;
	}

	executed = false;
	c_config::get().nightmode = false;

	if (sv_skyname) {
		sv_skyname->set_value(fallback_skybox.c_str());
	}

	interfaces::console->get_convar("r_drawspecificstaticprop")->set_value(1);

	for (int i = 0; i < materials.size(); i++) {
		if (materials[i].material->get_reference_count() <= 0) continue;

		materials[i].restore();
		materials[i].material->refresh();
	}

	materials.clear();

	sv_skyname = nullptr;
	fallback_skybox = "";
}