#include "Settings.h"

Settings::Settings(json settings_json) {
	this->json_settings = settings_json;
	open_flagged = json_settings.value("open_flagged", false);
	end_solution = json_settings.value("end_solution", true);
	colored_numbers = json_settings.value("colored_numbers", true);
	enable_arrow_keys = json_settings.value("enable_arrow_keys", false);
	mouse_over_keyboard = json_settings.value("mouse_over_keyboard", true);
}

std::string Settings::ConvertText() {
	json_settings["open_flagged"] = open_flagged;
	json_settings["end_solution"] = end_solution;
	json_settings["colored_numbers"] = colored_numbers;
	json_settings["enable_arrow_keys"] = enable_arrow_keys;
	json_settings["mouse_over_keyboard"] = mouse_over_keyboard;
	return json_settings.dump();
}

void Settings::Display() const {
	std::cout << "\nConfirmation for opening flagged tiles: " << (open_flagged ? "Never ask" : "Always ask");
	std::cout << "\nShow solution at the end: " << (end_solution ? "Yes" : "No");
	std::cout << "\nColored Hint Numbers: " << (colored_numbers ? "Yes" : "No");
	std::cout << "\nEnable Arrow Keys: " << (enable_arrow_keys ? "Enable" : "Disable");
	std::cout << "\nMouse or Keyboard: " << (mouse_over_keyboard ? "Mouse" : "Keyboard");
}

bool Settings::ChangeSettings(GUI& gui) {
	std::cout << "\n1- Confirmation for opening flagged tiles: " << (open_flagged ? "Never ask" : "Always ask");
	std::cout << "\n2- Show solution at the end: " << (end_solution ? "Yes" : "No");
	std::cout << "\n3- Colored Numbers: " << (colored_numbers ? "Yes" : "No");
	std::cout << "\n4- Enable Arrow Keys: " << (enable_arrow_keys ? "Enable" : "Disable");
	std::cout << "\n5- Enable Mouse Buttons: " << (mouse_over_keyboard ? "Mouse" : "Keyboard");
	std::cout << "\n6- Back to menu";

	int option;
	std::string explanation = "\nChoose setting menu option: ";
	while (!gui.GetMenuChoice(option, 6, explanation));

	switch (option) {
	case 1:
		open_flagged = not open_flagged;
		break;
	case 2:
		end_solution = not end_solution;
		break;
	case 3:
		colored_numbers = not colored_numbers;
		break;
	case 4:
		mouse_over_keyboard = not mouse_over_keyboard;
		break;
	case 5:
		enable_arrow_keys = not enable_arrow_keys;
		break;
	case 6:
		return false;
		break;
	default:
		break;
	}

	return true;
}

bool Settings::GetOpenFlagged() const {
	return open_flagged;
}

bool Settings::GetColoredHintColors() const {
	return colored_numbers;
}

bool Settings::GetEndSolution() const {
	return end_solution;
}

bool Settings::GetMouseOverKeyboard() const {
	return mouse_over_keyboard;
}

bool Settings::GetEnableArrowKeys() const {
	return enable_arrow_keys;
}