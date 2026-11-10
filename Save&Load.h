#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>

#include "GUI.h"

namespace fs = std::filesystem;

class FileManager {
private:
	bool Save(const std::string filename, const std::string& text_code);
	bool Load(const std::string filename, std::string& text_code);
public:
	void ListSaves();
	bool SaveInquiry(GUI& gui);
	bool SaveGameFile(const std::string& text_code, GUI& gui);
	bool LoadGameFile(std::string& text_code, GUI& gui);
	bool SaveStatsFile(const std::string& text_code);
	bool LoadStatsFile(std::string& text_code);
	bool SaveSettingsFile(const std::string& text_code);
	bool LoadSettingsFile(std::string& text_code);
};
