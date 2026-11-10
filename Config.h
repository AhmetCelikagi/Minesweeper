#pragma once

#include <iostream>

#include "GUI.h"

struct Configs {
	int con_height;
	int con_width;
	int con_num_mine;

	Configs() : con_height(20), con_width(20), con_num_mine(50) {}
	Configs(int h, int w, int m) : con_height(h), con_width(w), con_num_mine(m) {}
	Configs GetConfig(GUI& gui);
};
