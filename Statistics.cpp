#include "Statistics.h"

Statistics::Statistics(json statistics_json) {
	this->json_statistics = statistics_json;
	max_game.row = json_statistics.value("max_game_row", 1);
	max_game.col = json_statistics.value("max_game_col", 1);
	game_played = json_statistics.value("game_played", 0);
	game_won = json_statistics.value("game_won", 0);
	game_lost = json_statistics.value("game_lost", 0);
	flagged_mine = json_statistics.value("flagged_mine", 0);
}

void Statistics::AddGameWon() {
	game_won++;
}

void Statistics::AddGameLost() {
	game_lost++;
}

void Statistics::AddGamePlayed() {
	game_played++;
}

Pos Statistics::GetMaxGame() {
	return max_game;
}

void Statistics::SetMaxGame(const Pos& pos) {
	max_game = pos;
}

std::string Statistics::ConvertText() {
	json_statistics["max_game_row"] = max_game.row;
	json_statistics["max_game_col"] = max_game.col;
	json_statistics["game_played"] = game_played;
	json_statistics["game_won"] = game_won;
	json_statistics["game_lost"] = game_lost;
	json_statistics["flagged_mine"] = flagged_mine;
	return json_statistics.dump();
}

void Statistics::Display() const {
	std::cout << "\nMax Game: " << max_game.row + 1 << " - " << max_game.col + 1;
	std::cout << "\nGames Played: " << game_played;
	std::cout << "\nGames Won: " << game_won;
	std::cout << "\nGames Lost: " << game_lost;
	std::cout << "\nFlagged Mines: " << flagged_mine;
}