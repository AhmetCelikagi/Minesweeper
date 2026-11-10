#pragma once

#include <iostream>
#include <string>
#include <climits>

#include "GUI.h"
#include "include/json.hpp"
#include "JsonUtils.h"

using json = nlohmann::json;

class Settings {
private:
	json json_settings;
	bool open_flagged;
	bool end_solution;
	bool colored_numbers;
	ColorPalette color_palette;
	bool enable_arrow_keys;
	bool mouse_over_keyboard;

public:
	Settings() :
		json_settings(), open_flagged(true), end_solution(true), colored_numbers(true), enable_arrow_keys(true), mouse_over_keyboard(true) {}
	Settings(json settings_json);
	std::string ConvertText();
	void Display() const;
	bool ChangeSettings(GUI& gui);
	bool GetOpenFlagged() const;
	bool GetColoredHintColors() const;
	bool GetEndSolution() const;
	bool GetMouseOverKeyboard() const;
	bool GetEnableArrowKeys() const;
};