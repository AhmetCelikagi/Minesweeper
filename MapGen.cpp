#include "MapGen.h"

#include "GUI.h"

void Map::GenerateRandomMineIndex(const Pos& first_select, std::vector<int>& mine_indexes) {
	std::vector<int> cells;
	cells.reserve(height * width);

	for (int i = 0; i < height * width; i++) {
		if (!first_select.Adjacent9(i, width))
			cells.push_back(i);
	}

	if (num_mine > cells.size()) {
		num_mine = cells.size();
		std::cout << "\nWarning! First Select deleted mines";
	}

	std::shuffle(cells.begin(), cells.end(), gen);

	for (int k = 0; k < num_mine; k++) {
		mine_indexes.push_back(cells[k]);
	}
}

void Map::ExtractConfigData(const json& map_json) {
	height = map_json.value("height", 4);
	width = map_json.value("width", 4);
	num_mine = map_json.value("num_mine", 3);
	int selected_row = map_json.value("selected_row", 0);
	int selected_col = map_json.value("selected_col", 0);
	table = std::vector(height, std::vector<Tile>(width));
	for (size_t r = 0; r < height; r++) {
		for (size_t c = 0; c < width; c++) {
			Tile tile(r * width + c, 0, false, false, false, false, false, false, Pos((int)r, (int)c));
			table[r][c] = tile;
		}
	}
	SetCursor(selected_row, selected_col);
}

void Map::ExtractMineData(std::vector<int>& mine_indexes, const json& map_json) {
	mine_indexes.clear();

	if (!map_json.contains("solution_table") || !map_json["solution_table"].is_array()) {
		std::cerr << "Error: 'solution_table' not found in JSON!\n";
		return;
	}

	std::vector<std::string> solution_table = map_json["solution_table"].get<std::vector<std::string>>();

	for (int r = 0; r < solution_table.size(); ++r) {
		for (int c = 0; c < solution_table[r].size(); ++c) {
			if (solution_table[r][c] == '*') {
				mine_indexes.push_back(r * width + c);
			}
		}
	}
}

void Map::ExtractUserTableData(const json& map_json) {
	if (!map_json.contains("user_table") || !map_json["user_table"].is_array())
		return;

	std::vector<std::string> user_table = map_json["user_table"].get<std::vector<std::string>>();

	for (int r = 0; r < height && r < user_table.size(); ++r) {
		for (int c = 0; c < width && c < user_table[r].size(); ++c) {
			char ch = user_table[r][c];
			Tile& tile = table[r][c];

			if (ch == 'P') {
				tile.SetFlag();
			}
			else if (ch == 'O') {
				tile.SetOpen();
			}
		}
	}

}

void Map::PlaceMines(std::vector<Tile>& mines, const std::vector<int>& mine_indexes) {
	for (int i = 0; i < num_mine; i++) {
		int r = mine_indexes[i] / width;
		int c = mine_indexes[i] % width;
		table[r][c].SetMine(true);
		mines.push_back(table[r][c]);
		Pos pos(r, c);
		PlaceHintNumbers(pos);

	}
}

void Map::PlaceHintNumbers(const Pos pos) {
	table[pos.row][pos.col].SetNumber(table[pos.row][pos.col].GetNumber() - 8);

	for (int dr = -1; dr <= 1; dr++) {
		for (int dc = -1; dc <= 1; dc++) {
			if (dr == 0 && dc == 0)
				continue;

			Pos hint_tile_pos(pos.row + dr, pos.col + dc);
			if (hint_tile_pos.IsValid(height, width)) {
				Tile* hint_tile = &table[hint_tile_pos.row][hint_tile_pos.col];
				hint_tile->SetNumber(hint_tile->GetNumber() + 1);
			}
		}
	}
}

Map::Map(int h, int w, int m) : height(h), width(w), num_mine(m), table(h, std::vector<Tile>(w)), gen(std::random_device{}()), selected_tile(nullptr) {
	if (h <= 0 || w <= 0)
		throw std::invalid_argument("Height and width must be positive integers!");

	if (m <= 0 || m >= h * w)
		throw std::invalid_argument("Invalid number of mines!");

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			table[i][j] = Tile(i * width + j, 0, false, false, false, false, false, false, Pos(i, j));
		}
	}

	SetCursor();

}

Map::Map(const json& map_json) {
	ExtractConfigData(map_json);

	std::vector<int> mine_indexes(num_mine);
	ExtractMineData(mine_indexes, map_json);
	PlaceMines(mines, mine_indexes);
	ExtractUserTableData(map_json);
}

void Map::SetCursor(const int row, const int col) {
	if (selected_tile) {
		selected_tile->SetSelected(false);
		selected_tile = nullptr;
	}
	table[row][col].SetSelected(true);
	selected_tile = &table[row][col];
}

Pos Map::FirstSelect(GUI& gui, Status&  status, const Settings& settings) {
	int user_input_row;
	int user_input_col;
	Pos first_select;
	if (settings.GetMouseOverKeyboard()) {
		while (gui.GetClick(first_select, status, settings) != ClickResult::TileOpened);
	}
	else {
		std::string first_row_explanation = "\nEnter first row: ";
		while (!gui.GetInt(user_input_row, 1, height, first_row_explanation));

		std::string first_column_explanation = "\nEnter first row: ";
		while (!gui.GetInt(user_input_col, 1, width, first_column_explanation));

		if (user_input_row <= 0 || user_input_row > height) {
			user_input_row = 1;
			std::cout << "\nError. 1- indexing";
		}

		if (user_input_col <= 0 || user_input_col > width) {
			user_input_col = 1;
			std::cout << "\nError. 1- indexing";
		}

		first_select = Pos(user_input_row - 1, user_input_col - 1);
	}
	SetCursor(first_select.row, first_select.col);

	return first_select;
}

void Map::FloodFill(const Pos& pos, const Settings& settings, GUI& gui) {
	table[pos.row][pos.col].SetFlood();
	for (int dr = -1; dr <= 1; dr++) {
		for (int dc = -1; dc <= 1; dc++) {
			if (dr == 0 && dc == 0)
				continue;

			Pos flooding_pos = Pos(pos.row + dr, pos.col + dc);
			if (flooding_pos.IsValid(height, width)) {
				Tile* flooding_tile = &table[flooding_pos.row][flooding_pos.col];
				if (!flooding_tile->IsMine() && flooding_tile->Open(settings, gui) && flooding_tile->IsEmpty() && !flooding_tile->IsFlooded()) {
					FloodFill(flooding_pos, settings, gui);
				}
			}
		}
	}
}

void Map::ChordOpen(const Pos& pos, const Settings& settings, GUI& gui) {
	int num_flagged = 0;
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			Pos chord_pos(pos.row + i, pos.col + j);
			if (chord_pos.IsValid(height, width) && table[chord_pos.row][chord_pos.col].IsFlagged())
				num_flagged++;
		}
	}
	if (selected_tile->GetNumber() != num_flagged) {
		std::cout << "\nNot all flagged";
		return;
	}
	else {
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				Pos chord_pos(pos.row + i, pos.col + j);
				if (chord_pos.IsValid(height, width) && !chord_pos.SamePos(selected_tile->GetPos())) {
					Tile* chord_tile = &table[chord_pos.row][chord_pos.col];
					if (!chord_tile->IsFlagged())
						chord_tile->Open(settings, gui);
					if (chord_tile->IsEmpty() && !chord_tile->IsFlooded())
						FloodFill(chord_pos, settings, gui);
				}
			}
		}
	}
}

void Map::OpenTile(const Settings& settings, GUI& gui) {
	std::cout << "\nSelected Tile: ";
	if (!selected_tile->IsOpened())
		selected_tile->Open(settings, gui);
	else {
		Pos selected_pos = selected_tile->GetPos();
		if (selected_tile->IsEmpty() && !selected_tile->IsFlooded())
			FloodFill(selected_pos, settings, gui);
		else if (!selected_tile->IsChorded()){
			ChordOpen(selected_pos, settings, gui);
		}
	}
}

void Map::PlaceMinesRandom(const Settings& settings, Status& status, GUI& gui) {
	Pos first_select = FirstSelect(gui, status, settings);

	std::vector<int> mine_indexes;
	GenerateRandomMineIndex(first_select, mine_indexes);
	PlaceMines(mines, mine_indexes);

	OpenTile(settings, gui);
}

int Map::GetHeight() const {
	return height;
}

int Map::GetWidth() const {
	return width;
}

int Map::GetNumMine() const {
	return num_mine;
}

Tile* Map::GetSelectedTile() {
	return selected_tile;
}

std::vector<std::vector<Tile>>& Map::GetTable() {
	return table;
}

void Map::SetSelectedTile(const int id) {
	int r = id / width;
	int c = id % width;
	SetSelectedTile(r, c);
}

void Map::SetSelectedTile(const int r, const int c) {
	if (!Pos(r, c).IsValid(height, width))
		return;

	std::cout << "\nSelecting: (" << r << ", " << c << ")";
	selected_tile->SetSelected(false);
	selected_tile = &table[r][c];
	selected_tile->SetSelected(true);
}

void Map::SetFlagOnSelected() {
	selected_tile->SetFlag();
}

bool Map::EndCheck(Status& status) {
	int opened = 0;
	int flagged_mines = 0;

	for (int r = 0; r < height; r++) {
		for (int c = 0; c < width; c++) {
			if (table[r][c].IsMine() && table[r][c].IsOpened()) {
				status = Status::Lost;
				return true;
			}
			if (table[r][c].IsOpened())
				opened++;
			if (table[r][c].IsMine() && table[r][c].IsFlagged())
				flagged_mines++;
		}
	}

	if (flagged_mines == mines.size()) {
		status = Status::Won;
		return true;
	}

	if (opened == height * width - num_mine) {
		status = Status::Won;
		return true;
	}
 
	return false;
}

std::vector<std::string> Map::EncodeUserTable() const {
	std::vector<std::string> user_text_code(height);
	for (int r = 0; r < height; r++) {
		for (int c = 0; c < width; c++) {
			Tile tile = table[r][c];
			if (tile.IsFlagged())
				user_text_code[r] += 'P';
			else if (tile.IsOpened())
				user_text_code[r] += 'O';
			else
				user_text_code[r] += '.';
		}
	}

	return user_text_code;
}

std::vector<std::string> Map::EncodeSolutionTable() const {
	std::vector<std::string> solution_text_code(height);
	for (int r = 0; r < height; r++) {
		for (int c = 0; c < width; c++) {
			if (table[r][c].IsMine())
				solution_text_code[r] += "*";
			else
				solution_text_code[r] += ".";
		}
	}

	return solution_text_code;
}

std::string Map::ConvertText() {
	json_game["height"] = height;
	json_game["width"] = width;
	json_game["num_mine"] = num_mine;
	json_game["selected_row"] = selected_tile->GetPos().row;
	json_game["selected_col"] = selected_tile->GetPos().col;
	json_game["user_table"] = EncodeUserTable();
	json_game["solution_table"] = EncodeSolutionTable();

	return json_game.dump();
}

bool Map::ContainPos(const Pos& pos) const {
	if (pos.IsValid())
		return (pos.row < height && pos.col < width);
	else
		return false;
}

int Map::GetUnflaggedMineCount() const {
	int flagged_tiles = 0;
	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {
			if (table[h][w].IsFlagged())
				flagged_tiles++;
		}
	}
	return (num_mine - flagged_tiles);
}