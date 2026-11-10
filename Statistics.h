#pragma once

#include <string>
#include <iostream>

#include "Position.h"
#include "include/json.hpp"
#include "JsonUtils.h"

using json = nlohmann::json;

class Statistics {
private:
	json json_statistics;
	Pos max_game;
	int game_played;
	int game_won;
	int game_lost;
	int flagged_mine;
public:
	Statistics() : json_statistics(), max_game(Pos()), game_played(0), game_won(0), game_lost(0), flagged_mine(0) {}
	Statistics(Pos m_g, int g_p, int g_w, int g_l, int f_m) : json_statistics(), max_game(m_g), game_played(g_p), game_won(g_w), game_lost(g_l), flagged_mine(f_m) {}
	Statistics(json statistics_json);

	void AddGameWon();
	void AddGameLost();
	void AddGamePlayed();
	Pos GetMaxGame();
	void SetMaxGame(const Pos& pos);
	std::string ConvertText();
	void Display() const;
};
