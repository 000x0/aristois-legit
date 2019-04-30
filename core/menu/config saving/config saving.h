#pragma once
#include <iostream>
#include <vector>
#include "../../../dependencies/utilities/singleton.hpp"

template< typename T >
class config_item {
	std::string category, name;
	T* value;
public:
	config_item(std::string category, std::string name, T* value) {
		this->category = category;
		this->name = name;
		this->value = value;
	}
};

template< typename T >
class config_value {
public:
	config_value(std::string category_, std::string name_, T* value_) {
		category = category_;
		name = name_;
		value = value_;
	}

	std::string category, name;
	T* value;
};

class c_config_saving : public singleton< c_config_saving > {
protected:
	std::vector< config_value< int >* > ints;
	std::vector< config_value< bool >* > bools;
	std::vector< config_value< float >* > floats;
private:
	void setup_value(int&, int, std::string, std::string);
	void setup_value(bool&, bool, std::string, std::string);
	void setup_value(float&, float, std::string, std::string);
public:
	c_config_saving() {
		Setup();
	}

	void Setup();

	void save();
	void load();
};