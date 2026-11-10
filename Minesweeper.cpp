#include "MapGen.h"
#include "Mechanics.h"
#include "Save&Load.h"
#include "Settings.h"
#include "Config.h"
#include "Statistics.h"
#include "PlayerStatus.h"
#include "GUI.h"
#include "include/json.hpp"

bool InitializeMap(Map& map, const Configs& config, const Settings& settings, Status& status, GUI& gui) {
	try {
		map = Map(config.con_height, config.con_width, config.con_num_mine);
		map.PlaceMinesRandom(settings, status, gui);
		return true;
	}
	catch (const std::invalid_argument& e) {
		std::cout << e.what();
		return false;
	}
}

void UpdateStatistics(Statistics& statistics, const Status& status, GUI& gui, FileManager& fima) {
	statistics.AddGamePlayed();

	if (status == Status::Lost) {
		statistics.AddGameLost();
	}
	if (status == Status::Won) {
		statistics.AddGameWon();
	}

	int table_size_width = gui.window.getSize().x;
	int table_size_length = gui.window.getSize().y;
	if (table_size_width * table_size_length > statistics.GetMaxGame().Size()) {
		statistics.SetMaxGame(Pos(table_size_width, table_size_length));
	}

	fima.SaveStatsFile(statistics.ConvertText());
}

void GameEnd(const Settings& settings, FileManager& fima, Statistics& statistics, Status& status, GUI& gui) {
	UpdateStatistics(statistics, status, gui, fima);

	if (status == Status::Lost) {
		std::cout << "\nYou Lost";
	}
	if (status == Status::Won) {
		std::cout << "\nYou Won";
	}

	fima.SaveStatsFile(statistics.ConvertText());
	status = Status::None;
}

void GameLoop(Map& map, Mechanics& mecha, FileManager& fima, const Settings& settings, Statistics& statistics, Status& status, GUI& gui) {
	mecha.Move(map, status, settings, gui);
	if (status == Status::Exiting){
		std::cout << "\nExiting to menu";
		status = Status::InMenu;
		if (fima.SaveInquiry(gui))
			fima.SaveGameFile(map.ConvertText(), gui);
	}
	else if (status == Status::Saving) {
		fima.SaveGameFile(map.ConvertText(), gui);
		status = Status::Playing;
	}
	else {
		map.EndCheck(status);
	}
}

void NewGame(Map& map, FileManager& fima, const Settings& settings, Statistics& statistics, Status& status, GUI& gui) {
	Configs config;
	status = Status::Playing;

	config = config.GetConfig(gui);

	while (!InitializeMap(map, config, settings, status, gui));

	if (status == Status::Lost || status == Status::Won)
		GameEnd(settings, fima, statistics, status, gui);

}

void LoadGame(Map& map, FileManager&  fima, const Settings& settings, Statistics& statistics, Status& status, GUI& gui) {
	std::string map_text_code;
	json map_json;
	if (!fima.LoadGameFile(map_text_code, gui))
		return;

	map_json = json::parse(map_text_code);
	map = Map(map_json);

	status = Status::Playing;

	if (status == Status::Lost || status == Status::Won)
		GameEnd(settings, fima, statistics, status, gui);

}

void SettingsMenu(Settings& settings, GUI& gui) {
	while(settings.ChangeSettings(gui));
	settings.Display();
}

void StatisticsMenu(Statistics& statistics, FileManager fima) {
	statistics.Display();
}

bool InitializeStatistics(Statistics& statistics, FileManager& fima) {
	json statistics_json;
	std::string stats_text_code;
	if (!fima.LoadStatsFile(stats_text_code))
		return false;
	statistics_json = json::parse(stats_text_code);
	statistics = Statistics(statistics_json);
	return true;
}

bool InitializeSettings(Settings& settings, FileManager& fima) {
	json settings_json;
	std::string settings_text_code;
	if (!fima.LoadSettingsFile(settings_text_code))
		return false;
	settings_json = json::parse(settings_text_code);
	settings = Settings(settings_json);
	return true;
}

bool Initialization(Settings& settings, Statistics& statistics, FileManager& fima, Status& status, GUI& gui) {
	status = Status::InMenu;
	gui.Initialize(settings);
	if (!InitializeStatistics(statistics, fima))
		return false;
	if (!InitializeSettings(settings, fima))
		return false;
	return true;
}


bool Menu(Map& map, Settings& settings, Statistics& statistics, FileManager& fima, Status& status, GUI& gui) {
	status = Status::InMenu;

	std::cout << "\n1-Start a new game";
	std::cout << "\n2-Load a game";
	std::cout << "\n3-Settings";
	std::cout << "\n4-Statistics";
	std::cout << "\n5-Exit";

	int choice;
	std::string explanation = "\nChoose main menu option: ";
	while (!gui.GetMenuChoice(choice, 5, explanation));

	switch (choice){
	case 1:
		NewGame(map, fima, settings, statistics, status, gui);
		break;
	case 2:
		LoadGame(map, fima, settings, statistics, status, gui);
		break;
	case 3:
		SettingsMenu(settings, gui);
		fima.SaveSettingsFile(settings.ConvertText());
		break;
	case 4:
		StatisticsMenu(statistics, fima);
		break;
	case 5:
		status = Status::None;
		gui.window.close();
		return false;
		break;
	default:
		return true;
		break;
	}

	return true;
}

bool Render(GUI& gui, Status& status, const Settings& settings) {
	if (status == Status::None || !gui.window.isOpen())
		return false;

	gui.window.clear();
	gui.DisplayScreen(status, settings);
	gui.window.display();
	return true;
}

int main() {
	Settings settings;
	Statistics statistics;
	FileManager fima;
	Mechanics mecha;
	Status status = Status::None;
	Map map;
	GUI gui(800, 600, "Minesweeper", map);

	bool game = true;

	if (!Initialization(settings, statistics, fima, status, gui))
		return 0;

	while (game) {
		if (status == Status::InMenu)
			game = Menu(map, settings, statistics, fima, status, gui);
		if (status == Status::Playing || status == Status::Solution)
			GameLoop(map, mecha, fima, settings, statistics, status, gui);
		if (status == Status::Lost || status == Status::Won)
			GameEnd(settings, fima, statistics, status, gui);
		game = Render(gui, status, settings);
	}

	return 0;
}
