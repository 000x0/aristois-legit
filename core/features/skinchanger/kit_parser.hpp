#pragma once
#include "../../../dependencies/common_includes.hpp"
#include <vector>
struct paint_kit {
	int id;
	std::string name;
	bool operator < (const paint_kit& other) {
		return name < other.name;
	}
};
extern std::vector<paint_kit> k_skins;
extern std::vector<paint_kit> k_gloves;
extern void initialize_kits();
