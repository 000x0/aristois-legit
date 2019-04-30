#pragma once
#include "../../../dependencies/common_includes.hpp"

class c_misc : public singleton <c_misc> {
public:
	void remove_smoke();
	void remove_flash();
	void remove_scope();
	void spectators();
	void watermark();
	void clantag_spammer();
	void viewmodel_offset();
private:
};