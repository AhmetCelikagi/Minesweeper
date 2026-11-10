#pragma once

#include "Position.h"
#include "Tile.h"
#include "Settings.h"
#include "PlayerStatus.h"

#include <vector>
#include <string>
#include <iostream>
#include <random>
#include <numeric>
#include <stdexcept>

class GUI;

class Map {
private:
	std::vector<std::vector<Tile>> table;
	json json_game;
	int height;
	int width;
	int num_mine;
	std::mt19937 gen;
	Tile* selected_tile;
	std::vector<Tile> mines;

	void GenerateRandomMineIndex(const Pos& first_select_ind, std::vector<int>& mine_indexes);
	void ExtractConfigData(const json& text_code);
	void ExtractMineData(std::vector<int>& mine_indexes, const json& map_json);
	void ExtractUserTableData(const json& map_json);
	void PlaceMines(std::vector<Tile>& mines, const std::vector<int>& mine_indexes);
	void PlaceHintNumbers(const Pos pos);
	std::vector<std::string> EncodeUserTable() const;
	std::vector<std::string> EncodeSolutionTable() const;

public:
	Map(int h = 20, int w = 20, int m = 5);
	Map(const json& map_json);
	void FloodFill(const Pos& pos, const Settings& settings, GUI& gui);
	void OpenTile(const Settings& settings, GUI& gui);
	void ChordOpen(const Pos& selected_pos, const Settings& settings, GUI& gui);
	void PlaceMinesRandom(const Settings& settings, Status& status, GUI& gui);
	void SetCursor(const int row = 0, const int col = 0);
	Pos FirstSelect(GUI& gui, Status& status, const Settings& settings);
	int GetHeight() const;
	int GetWidth() const;
	int GetNumMine() const;
	Tile* GetSelectedTile();
	std::vector<std::vector<Tile>>& GetTable();
	void SetSelectedTile(const int id);
	void SetSelectedTile(const int r, const int c);
	void SetFlagOnSelected();
	bool EndCheck(Status& status);
	std::string ConvertText();
	bool ContainPos(const Pos& pos) const;
	int GetUnflaggedMineCount() const;
};