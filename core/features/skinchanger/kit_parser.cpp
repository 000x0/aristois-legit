#include <algorithm>
#include "kit_parser.hpp"
std::vector<paint_kit> k_skins;
std::vector<paint_kit> k_gloves;
class CCStrike15ItemSchema;
class CCStrike15ItemSystem;
template <typename Key, typename Value>

struct Node_t {
	int previous_id; //0x0000
	int next_id; //0x0004
	void* _unknown_ptr; //0x0008
	int _unknown; //0x000C
	Key key; //0x0010
	Value value; //0x0014
};
template <typename Key, typename Value>
struct Head_t {
	Node_t<Key, Value>* memory; //0x0000
	int allocation_count; //0x0004
	int grow_size; //0x0008
	int start_element; //0x000C
	int next_available; //0x0010
	int _unknown; //0x0014
	int last_element; //0x0018
}; //Size=0x001C

struct String_t {
	char* buffer; //0x0000
	int capacity; //0x0004
	int grow_size; //0x0008
	int length; //0x000C
}; //Size=0x0010
struct CPaintKit {
	int id; //0x0000
	String_t name; //0x0004
	String_t description; //0x0014
	String_t item_name; //0x0024
	String_t material_name; //0x0034
	String_t image_inventory; //0x0044
	char pad_0x0054[0x8C]; //0x0054
}; //Size=0x00E0
void* get_export(const char* module_name, const char* export_name) {
	HMODULE mod;
	while (!((mod = GetModuleHandleA(module_name))))
		Sleep(100);
	return reinterpret_cast<void*>(GetProcAddress(mod, export_name));
}

void initialize_kits() {
	const auto V_UCS2ToUTF8 = static_cast<int(*)(const wchar_t* ucs2, char* utf8, int len)>(get_export("vstdlib.dll", "V_UCS2ToUTF8"));
	const auto sig_address = utilities::pattern_scan(GetModuleHandleA("client_panorama.dll"), "E8 ? ? ? ? FF 76 0C 8D 48 04 E8");
	const auto item_system_offset = *reinterpret_cast<std::int32_t*>(sig_address + 1);
	const auto item_system_fn = reinterpret_cast<CCStrike15ItemSystem* (*)()>(sig_address + 5 + item_system_offset);
	const auto item_schema = reinterpret_cast<CCStrike15ItemSchema*>(std::uintptr_t(item_system_fn()) + sizeof(void*));

	{
		const auto get_paint_kit_definition_offset = *reinterpret_cast<std::int32_t*>(sig_address + 11 + 1);
		const auto get_paint_kit_definition_fn = reinterpret_cast<CPaintKit*(__thiscall*)(CCStrike15ItemSchema*, int)>(sig_address + 11 + 5 + get_paint_kit_definition_offset);
		const auto start_element_offset = *reinterpret_cast<std::intptr_t*>(std::uintptr_t(get_paint_kit_definition_fn) + 8 + 2);
		const auto head_offset = start_element_offset - 12;
		const auto map_head = reinterpret_cast<Head_t<int, CPaintKit*>*>(std::uintptr_t(item_schema) + head_offset);
		for (auto i = 0; i <= map_head->last_element; ++i) {
			const auto paint_kit = map_head->memory[i].value;
			if (paint_kit->id == 9001)
				continue;
			const auto wide_name = interfaces::localize->find(paint_kit->item_name.buffer + 1);
			char name[256];

			V_UCS2ToUTF8(wide_name, name, sizeof(name));
			if (paint_kit->id < 10000)
				k_skins.push_back({ paint_kit->id, name });
			else
				k_gloves.push_back({ paint_kit->id, name });
		}
		std::sort(k_skins.begin(), k_skins.end());
		std::sort(k_gloves.begin(), k_gloves.end());
	}

	printf("Kit Parser initialized!\n");
}