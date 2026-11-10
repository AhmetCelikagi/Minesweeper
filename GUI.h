#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <SFML/Graphics.hpp>

#include "PlayerStatus.h"
#include "ColorPalette.h"
#include "Icons.h"
#include "IconButtons.h"
#include "TextButtons.h"
#include "ClickResult.h"
#include "MoveType.h"
#include "Position.h"

class Map;
class Tile;
class Settings;

class GUI {
private:
	Map* map;
	sf::Font font;
	Icons icons;
	std::vector<IconButtons> icon_buttons;
	std::vector<TextButtons> text_buttons;
	ColorPalette color_palette;
	int tile_size;
	int app_bar_size;
	int border_size;

	void DrawTileRect(const Tile& tile, const Pos& sprite_pos);
	void DrawGameIcon(const Tile& tile, const Pos& sprite_pos, const Settings& settings);
	void DrawSolutionIcon(const Tile& tile, const Pos& sprite_pos, const Settings& settings);
	void DrawIconButton(const IconButtons& icon_button);
	void DrawTextButton(const TextButtons& text_button);
	void DrawAppBar();
	void DrawMineCounter();
	void DrawMenuButtons();
	bool PollKeyEvent(sf::Event& event);
	bool PollTextEvent(sf::Event& event, char& got_char);
	bool PollMouseEvent(sf::Event& event);
	bool AppBarClicked(const Pos& mouse_pos);
	void OperateAppBar(ClickResult& click_result);
	sf::Color GetHintColor(const int num);

public:
    sf::RenderWindow window;

    GUI(unsigned int w, unsigned int h, const std::string& t, Map& m): window(sf::VideoMode({ w, h }), t), map(&m), tile_size(40), app_bar_size(60), border_size(5) {}

	void Initialize(const Settings& settings);
	void Resize(const int new_width, const int new_height);
	void MenuDisplay();
	void DisplayScreen(const Status& status, const Settings& settings);
	bool GetMove(MoveType& move, const Settings& settings, const std::string& explanation = "");
	bool GetInt(int& num, const int min_val = INT_MIN, const int max_val = INT_MAX, const std::string& explanation = "");
	bool GetMenuChoice(int& choice, const int option_count = 9, const std::string& explanation = "");
	bool GetBool(bool& boolean, const std::string& explanation = "");
	bool GetChar(char& character, const std::string& explanation = "");
	bool GetString(std::string& text, const std::string& explanation = "");
	ClickResult GetClick(Pos& clicked_tile_ind, Status status, const Settings& settings);
	bool MouseToTile(Pos& mouse_pos, Pos& clicked_tile_ind, ClickResult& click_result);
	bool LoadFont(sf::Font& font);
	bool LoadIcons(Icons& icons);
	void LoadIconButtons(std::vector<IconButtons>& icon_buttons);
	void LoadTextButtons(std::vector<TextButtons>& text_buttons);
};