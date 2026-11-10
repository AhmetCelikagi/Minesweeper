#include "Save&Load.h"

bool FileManager::Save(const std::string filename, const std::string& text_code){
	std::ofstream file("saved_games/" + filename + ".json");

	if (!file) {
		std::cout << "\nError! Could not opened the file named " << filename;
		return false;
	}

	file << text_code;

	std::cout << "\nGame " << filename << "Saved";
	file.close();
	return true;
}

bool FileManager::Load(const std::string filename, std::string& text_code) {
	text_code.clear();
	std::ifstream file("saved_games/" + filename + ".json");

	if (!file) {
		std::cout << "\nError! Could not opened the file named " << filename;
		return false;
	}

	std::string line;

	while(std::getline(file, line)) {
		text_code += line + "\n";
	}


	std::cout << "\nGame " << filename << " Loaded";
	file.close();
	return true;
}

void FileManager::ListSaves() {
	std::cout << "\nSaved Games: ";
	int ind_count = 1;
	for (const auto& entry : fs::directory_iterator("saved_games")) {
		std::cout << std::endl << ind_count << "- " << entry.path().filename();
		ind_count++;
	}
}

bool FileManager::SaveInquiry(GUI& gui) {
	bool answer;
	std::string explanation = "\nSave? (Y/N): ";
	while (!gui.GetBool(answer, explanation));
	return answer;
}

bool FileManager::SaveGameFile(const std::string& text_code, GUI& gui) {
	std::string name;
	std::string explanation = "\nName to save the game: ";
	while (!gui.GetString(name, explanation));

	return Save(name, text_code);
}

bool FileManager::LoadGameFile(std::string& text_code, GUI& gui) {
	ListSaves();

	std::string name;
	std::string explanation = "\nName to load the game: ";
	while (!gui.GetString(name, explanation));

	return Load(name, text_code);
}

bool FileManager::SaveStatsFile(const std::string& text_code) {
	std::ofstream file("statistics.json");

	if (!file) {
		std::cout << "\nError! Could not opened the statisitcs file.";
		return false;
	}
	file << text_code;

	std::cout << "\nStatistics Saved";
	file.close();
	return true;
}

bool FileManager::LoadStatsFile(std::string& text_code) {
	text_code.clear();
	std::ifstream file("statistics.json");

	if (!file) {
		std::cout << "\nError! Could not opened the statisitcs file.";
		return false;
	}

	std::string line;

	while (std::getline(file, line)) {
		text_code += line + "\n";
	}


	std::cout << "\nStatistics Loaded";
	file.close();
	return true;
}

bool FileManager::SaveSettingsFile(const std::string& text_code) {
	std::ofstream file("settings.json");

	if (!file) {
		std::cout << "\nError! Could not opened the settings file.";
		return false;
	}
	file << text_code;

	std::cout << "\nSettings Saved";
	file.close();
	return true;
}

bool FileManager::LoadSettingsFile(std::string& text_code) {
	text_code.clear();
	std::ifstream file("settings.json");

	if (!file) {
		std::cout << "\nError! Could not opened the settings file.";
		return false;
	}

	std::string line;
	while (std::getline(file, line)) {
		text_code += line + "\n";
	}

	std::cout << "\nSettings Loaded";
	file.close();
	return true;
}


