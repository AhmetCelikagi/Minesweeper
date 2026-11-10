#include "Config.h"

Configs Configs::GetConfig(GUI& gui) {
	Configs config;
	std::string height_explanation = "\nEnter map height: ";
	while (!gui.GetInt(config.con_height, 4, 24, height_explanation));

	std::string width_explanation = "\nEnter map width: ";
	while (!gui.GetInt(config.con_width, 4, 24, width_explanation));

	std::string mines_explanation = "\nEnter number of mines: ";
	while (!gui.GetInt(config.con_num_mine, 1, config.con_height * config.con_width-2, mines_explanation));

	return config;
}