#include "GUI.h"

#include "MapGen.h"
#include "Tile.h"
#include "Settings.h"

void GUI::Initialize(const Settings& settings) {
	LoadFont(font);
	LoadIcons(icons);
	LoadIconButtons(icon_buttons);
	sf::Image game_icon = icons.game_icon;
	window.setIcon(game_icon.getSize().x, game_icon.getSize().y, game_icon.getPixelsPtr());
	tile_size = 40;
	app_bar_size = 60;
	border_size = 5;
}

void GUI::Resize(const int new_width, const int new_height) {
	window.setSize(sf::Vector2u(new_width, new_height));
	window.setView(sf::View(sf::FloatRect(0, 0, new_width, new_height)));
}

void GUI::MenuDisplay() {
	Resize(600, 800);
}

void GUI::DisplayScreen(const Status& status, const Settings& settings) {
	if (status == Status::InMenu) {
		MenuDisplay();
		return;
	}

	int map_height = map->GetHeight() * tile_size;
	int map_width = map->GetWidth() * tile_size;

	if (window.getSize().x != map_width || window.getSize().y != (map_height + app_bar_size)) {
		Resize(map_width, map_height + app_bar_size);
	}

	DrawAppBar();

	bool showing_solution = (status == Status::Solution || status == Status::Lost || status == Status::Won);

	for (int r = 0; r < map->GetHeight(); r++) {
		for (int c = 0; c < map->GetWidth(); c++) {
			Tile tile = map->GetTable()[r][c];

			Pos tile_sprite_pos = Pos(r * tile_size + app_bar_size, c * tile_size);

			DrawTileRect(tile, tile_sprite_pos);

			if (showing_solution) {
				DrawSolutionIcon(tile, tile_sprite_pos, settings);
			}
			else {
				DrawGameIcon(tile, tile_sprite_pos, settings);
			}
		}
	}
}

void GUI::DrawAppBar() {
	for (int i = 0; i < icon_buttons.size(); i++) {
		if (icon_buttons[i].is_active)
			DrawIconButton(icon_buttons[i]);
	}

	DrawMineCounter();
}

void GUI::DrawMineCounter() {
	int unflagged_mine = map->GetUnflaggedMineCount();
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(20);
	text.setString("Mines: " + std::to_string(unflagged_mine));

	if (unflagged_mine < 0)
		text.setFillColor(sf::Color::Red);
	else if (unflagged_mine == 0)
		text.setFillColor(sf::Color::Green);
	else
		text.setFillColor(sf::Color::White);

	text.setPosition(sf::Vector2f(180, 25));
	window.draw(text);
}

void GUI::DrawIconButton(const IconButtons& icon_button) {
	sf::RectangleShape button_rect(sf::Vector2f(icon_button.size.col, icon_button.size.row));
	button_rect.setFillColor(color_palette.closed_color);
	button_rect.setPosition(sf::Vector2f(icon_button.pos.col, icon_button.pos.row));
	window.draw(button_rect);
	sf::Sprite sprite(icon_button.icon);
	sprite.setPosition(sf::Vector2f(icon_button.pos.col, icon_button.pos.row));
	window.draw(sprite);
}

void GUI::DrawMenuButtons() {
	std::cout << "\nMenu BUtton drawing"; //Debug
	for (int i = 0; i < text_buttons.size(); i++) {
		if (text_buttons[i].is_active)
			DrawTextButton(text_buttons[i]);
	}
}

void GUI::DrawTextButton(const TextButtons& text_button) {
	std::cout << "\nMenu button drawed"; //Debug
	sf::RectangleShape button_rect(sf::Vector2f(text_button.size.col, text_button.size.row));
	button_rect.setFillColor(sf::Color::Blue);
	button_rect.setPosition(sf::Vector2f(text_button.pos.col, text_button.pos.row));
	window.draw(button_rect);
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(20);
	text.setString(text_button.text);
	text.setFillColor(sf::Color::White);
	text.setPosition(sf::Vector2f(text_button.pos.col, text_button.pos.row));
	window.draw(text);
}

void GUI::DrawTileRect(const Tile& tile, const Pos& sprite_pos) {
	sf::Color color = color_palette.error_color;
	if (tile.IsOpened()) {
		color = color_palette.opened_color;
	}
	else {
		color = color_palette.closed_color;
	}
	if (tile.IsSelected()) {
		color = color_palette.selected_color;
	}

	sf::RectangleShape rect(sf::Vector2f(tile_size -2 * border_size, tile_size -2 * border_size));
	rect.setFillColor(color);
	rect.setPosition(sf::Vector2f(sprite_pos.col + border_size, sprite_pos.row + border_size));
	window.draw(rect);
}

void GUI::DrawGameIcon(const Tile& tile, const Pos& sprite_pos, const Settings& settings) {
	if (tile.IsFlagged()) {
		sf::Sprite sprite(icons.flag_icon);
		sprite.setPosition(sf::Vector2f(sprite_pos.col + border_size, sprite_pos.row + border_size));
		window.draw(sprite);
	}

	else if (tile.IsOpened()) {
		int hint_num = tile.GetNumber();
		if (hint_num > 0) {
			sf::Text text;
			text.setFont(font);
			text.setCharacterSize(20);
			text.setPosition(sf::Vector2f(sprite_pos.col + tile_size * 0.25 + border_size, sprite_pos.row + tile_size * 0.08 + border_size));
			text.setString(std::to_string(hint_num));
			text.setFillColor(color_palette.default_hint_color);
			if (settings.GetColoredHintColors())
				text.setFillColor(GetHintColor(hint_num));
			window.draw(text);
		}
		return;
	}
}

void GUI::DrawSolutionIcon(const Tile& tile, const Pos& sprite_pos, const Settings& settings) {
	if (tile.IsFlagged()) {
		sf::Sprite sprite(icons.flag_icon);
		sprite.setPosition(sf::Vector2f(sprite_pos.col + border_size, sprite_pos.row + border_size));
		window.draw(sprite);
		return;
	}
	if (tile.IsMine()) {
		sf::Sprite sprite(icons.mine_icon);
		sprite.setPosition(sf::Vector2f(sprite_pos.col + border_size, sprite_pos.row + border_size));
		window.draw(sprite);
		return;
	}

	if (tile.IsOpened()) {
		int hint_num = tile.GetNumber();
		if (hint_num > 0) {
			sf::Text text;
			text.setFont(font);
			text.setCharacterSize(20);
			text.setPosition(sf::Vector2f(sprite_pos.col + tile_size * 0.25 + border_size, sprite_pos.row + tile_size * 0.08 + border_size));
			text.setString(std::to_string(hint_num));
			text.setFillColor(color_palette.default_hint_color);
			if (settings.GetColoredHintColors())
				text.setFillColor(GetHintColor(hint_num));
			window.draw(text);
		}
		return;
	}
}

sf::Color GUI::GetHintColor(const int num) {
	switch (num){
	case 1:
		return color_palette.hint_1_color;
		break;
	case 2:
		return color_palette.hint_2_color;
		break;
	case 3:
		return color_palette.hint_3_color;
		break;
	case 4:
		return color_palette.hint_4_color;
		break;
	case 5:
		return color_palette.hint_5_color;
		break;
	case 6:
		return color_palette.hint_6_color;
		break;
	case 7:
		return color_palette.hint_7_color;
		break;
	case 8:
		return color_palette.hint_8_color;
		break;
	default:
		return color_palette.error_color;
		break;
	}
}

bool GUI::PollKeyEvent(sf::Event& event) {
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
			return false;
		}

		if (event.type == sf::Event::KeyPressed)
			return true;
	}

	return false;
}

bool GUI::PollTextEvent(sf::Event& event, char& got_char) {
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
			return false;
		}

		if (event.type == sf::Event::TextEntered) {
			got_char = static_cast<char>(event.text.unicode);
			return true;
		}
	}

	return false;
}

bool GUI::PollMouseEvent(sf::Event& event) {
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
			return false;
		}

		if (event.type == sf::Event::MouseButtonPressed)
			return true;
	}

	return false;
}

bool GUI::GetMove(MoveType& move, const Settings& settings, const std::string& explanation) {
	sf::Event event;
	bool waiting_input = true;
	bool allow_arrows = settings.GetEnableArrowKeys();
	move = MoveType::None;
	std::cout << explanation;

	while (waiting_input) {
		while (waiting_input && PollKeyEvent(event)) {
			if (event.key.code == sf::Keyboard::Key::W || (allow_arrows && event.key.code == sf::Keyboard::Key::Up)) {
				move = MoveType::Up;
				waiting_input = false;
			}
			else if (event.key.code == sf::Keyboard::Key::A || (allow_arrows && event.key.code == sf::Keyboard::Key::Left)) {
				move = MoveType::Left;
				waiting_input = false;
			}
			else if (event.key.code == sf::Keyboard::Key::S || (allow_arrows && event.key.code == sf::Keyboard::Key::Down)) {
				move = MoveType::Down;
				waiting_input = false;
			}
			else if (event.key.code == sf::Keyboard::Key::D || (allow_arrows && event.key.code == sf::Keyboard::Key::Right)) {
				move = MoveType::Right;
				waiting_input = false;
			}
			else if (event.key.code == sf::Keyboard::Key::P) {
				move = MoveType::Flag;
				waiting_input = false;
			}
			else if (event.key.code == sf::Keyboard::Key::O) {
				move = MoveType::Open;
				waiting_input = false;
			}
			else if (event.key.code == sf::Keyboard::Key::X) {
				move = MoveType::Exit;
				waiting_input = false;
			}
		}

		if (!window.isOpen())
			return false;

		if (waiting_input)
			sf::sleep(sf::milliseconds(10));
	}

	return true;
}

bool GUI::GetInt(int& num, const int min_val, const int max_val, const std::string& explanation) {
	sf::Event event;
	bool waiting_input = true;
	char character = '\0';
	num = 0;
	std::string num_str = "";
	std::cout << explanation;

	while (waiting_input) {
		while (waiting_input && PollTextEvent(event, character)) {
			if (character == '\r' || character == '\n') {
				waiting_input = false;
				continue;
			}
			else if (waiting_input && character == '\b') {
				if (num_str != "")
					num_str.pop_back();
			}
			else if (waiting_input && character >= '0' && character <= '9') {
				num_str += character;
			}

			std::cout << std::endl << num_str;
		}

		if (!window.isOpen())
			return false;

		if (waiting_input)
			sf::sleep(sf::milliseconds(10));
	}

	try {
		num = std::stoi(num_str);
	}
	catch (const std::exception&) {
		std::cout << "\nInvalid number";
		return false;
	}

	if (num <= min_val) {
		num = min_val;
	}
	else if (num >= max_val) {
		num = max_val;
	}

	std::cout << "\nGot: " << num;
	return true;
}

bool GUI::GetMenuChoice(int& choice, const int option_count, const std::string& explanation) {
	sf::Event event;
	bool waiting_input = true;
	choice = 1;
	char character = '\0';
	std::cout << explanation;

	while (waiting_input) {
		while (waiting_input && PollTextEvent(event, character)) {
			if (character >= '1' && character <= '9') {
				choice = character - '0';
				if (choice <= option_count) {
					waiting_input = false;
				}
			}
		}

		if (!window.isOpen())
			return false;

		if (waiting_input)
			sf::sleep(sf::milliseconds(10));
	}

	std::cout << "Got: " << choice;
	return true;
}

bool GUI::GetBool(bool& boolean, const std::string& explanation) {
	sf::Event event;
	bool waiting_input = true;
	boolean = false;

	std::cout << explanation;

	while (waiting_input) {
		while (waiting_input && PollKeyEvent(event)) {
			if (event.key.code == sf::Keyboard::Key::Y) {
				boolean = true;
				waiting_input = false;
			}
			else if (event.key.code == sf::Keyboard::Key::N) {
				boolean = false;
				waiting_input = false;
			}
		}

		if (!window.isOpen())
			return false;

		if (waiting_input)
			sf::sleep(sf::milliseconds(10));
	}

	std::cout << "\nGot: " << (boolean ? "true" : "false");

	return false;
} 

bool GUI::GetChar(char& character, const std::string& explanation) {
	sf::Event event;
	bool waiting_input = true;
	character = '\0';
	std::cout << explanation;

	while (waiting_input) {
		while (waiting_input && PollTextEvent(event, character)) {
			if (character >= 32 && character <= 126)
				waiting_input = false;
		}

		if (!window.isOpen())
			return false;

		if (waiting_input)
			sf::sleep(sf::milliseconds(10));
	}

	std::cout << "Got: " << character;
	return true;
}

bool GUI::GetString(std::string& text, const std::string& explanation) {
	sf::Event event;
	bool waiting_input = true;
	char character = '\0';
	std::cout << explanation;

	while (waiting_input) {
		while (waiting_input && PollTextEvent(event, character)) {
			if (character == '\r' || character == '\n') {
				waiting_input = false;
				continue;
			}
			else if (waiting_input && character == '\b') {
				if (!text.empty())
					text.pop_back();
			}
			else if (waiting_input && character >= ' ' && character <= '~') {
				text += character;
			}

			std::cout << std::endl << text;
		}

		if (!window.isOpen())
			return false;

		if (waiting_input)
			sf::sleep(sf::milliseconds(10));
	}

	std::cout << "\nGot: " << text;
	return true;
}

bool GUI::AppBarClicked(const Pos& mouse_pos) {
	for (int i = 0; i < icon_buttons.size(); i++) {
		IconButtons& icon_button = icon_buttons[i];
		std::cout << "\nIcon Button Positions:";
		icon_button.pos.Display(); //Debug
		if (icon_button.is_active && mouse_pos.InsideRect(icon_button.pos, icon_button.size)) {
			icon_button.clicked = true;
			std::cout << "\nIcon Button clicked";
			return true;
		}
	}
	return false;
}

bool GUI::MouseToTile(Pos& mouse_pos, Pos& clicked_tile_ind, ClickResult& click_result) {
	std::cout << "\nMouse Position: ";
	mouse_pos.Display(); //Debug
	if (AppBarClicked(mouse_pos)) {
		OperateAppBar(click_result);
		return false;
	}


	Pos adjusted_mouse_pos(mouse_pos.row - app_bar_size - border_size, mouse_pos.col - border_size);
	std::cout << "\nAdjusted Position: ";
	adjusted_mouse_pos.Display(); //Debug
	if (!adjusted_mouse_pos.IsValid()) {
		std::cout << "\nInvalid Pos";
		click_result = ClickResult::None;
		return false;
	}

	Pos relative_pos(adjusted_mouse_pos.row % 40, adjusted_mouse_pos.col % 40);
	std::cout << "\nRelative Position: ";
	relative_pos.Display(); //Debug
	clicked_tile_ind = Pos(adjusted_mouse_pos.row / 40, adjusted_mouse_pos.col / 40);
	std::cout << "\nClicked Tile Index: ";
	clicked_tile_ind.Display(); //Debug
	return (relative_pos.row >= 0 && relative_pos.row < tile_size &&
		relative_pos.col >= 0 && relative_pos.col < tile_size);
}

ClickResult GUI::GetClick(Pos& clicked_tile_ind, Status status, const Settings& settings) {
	sf::Event event;

	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
				return ClickResult::None;
			}
			if (event.type == sf::Event::Resized) {
				window.setView(sf::View(sf::FloatRect(0.f, 0.f,
					(float)event.size.width, (float)event.size.height)));
				continue;
			}
			if (event.type != sf::Event::MouseButtonPressed)
				continue;

			ClickResult result;
			if (event.mouseButton.button == sf::Mouse::Left)
				result = ClickResult::TileOpened;
			else if (event.mouseButton.button == sf::Mouse::Right)
				result = ClickResult::TileFlagged;
			else
				continue;

			Pos mouse_pos(event.mouseButton.y, event.mouseButton.x);
			if (!MouseToTile(mouse_pos, clicked_tile_ind, result) &&
				(result == ClickResult::TileOpened || result == ClickResult::TileFlagged))
				result = ClickResult::None;
			return result;
		}

		window.clear();
		DisplayScreen(status, settings);
		window.display();
		sf::sleep(sf::milliseconds(10));
	}
	return ClickResult::None;
}

void GUI::OperateAppBar(ClickResult& click_result) {
	for (int i = 0; i < icon_buttons.size(); i++) {
		IconButtons& icon_button = icon_buttons[i];
		if (icon_button.clicked) {
			if (icon_button.type == IconButtonType::ExitButton) {
				click_result = ClickResult::ExitClicked;
			}
			else if (icon_button.type == IconButtonType::SaveButton) {
				click_result = ClickResult::SaveClicked;
			}
			else if (icon_button.type == IconButtonType::SolutionButton) {
				click_result = ClickResult::SolutionClicked;
			}
			icon_button.clicked = false;
		}
	}
}


bool GUI::LoadFont(sf::Font& font) {
	if (!font.loadFromFile("OpenSans-BoldItalic.ttf")) {
		std::cout << "\nFont file could not find";
		return false;
	}
	return true;
}

bool GUI::LoadIcons(Icons& icons) {
	if (!icons.game_icon.loadFromFile("Icons/Game Icon.png")) {
		std::cout << "\nGame icon file could not find";
		return false;
	}
	if (!icons.mine_icon.loadFromFile("Icons/Mine Icon.png")) {
		std::cout << "\nMine icon file could not find";
		return false;
	}
	if (!icons.flag_icon.loadFromFile("Icons/Flag Icon.png")) {
		std::cout << "\nFlag icon file could not find";
		return false;
	}
	if (!icons.exit_icon.loadFromFile("Icons/Exit Icon.png")) {
		std::cout << "\nExit icon file could not find";
		return false;
	}
	if (!icons.solution_icon.loadFromFile("Icons/Solution Icon.png")) {
		std::cout << "\nSolution icon file could not find";
		return false;
	}
	if (!icons.save_icon.loadFromFile("Icons/Save Icon.png")) {
		std::cout << "\nSave icon file could not find";
		return false;
	}
	return true;
}

void GUI::LoadIconButtons(std::vector<IconButtons>& icon_buttons) {
	IconButtons exit_icon_button(IconButtonType::ExitButton, Pos(20, 20), Pos(30, 30), icons.exit_icon, true);
	icon_buttons.push_back(exit_icon_button);
	IconButtons solution_icon_button(IconButtonType::SolutionButton, Pos(20, 70), Pos(30, 30), icons.solution_icon, false);
	icon_buttons.push_back(solution_icon_button);
	IconButtons save_icon_button(IconButtonType::SaveButton, Pos(20, 120), Pos(30, 30), icons.save_icon, true);
	icon_buttons.push_back(save_icon_button);
}

void GUI::LoadTextButtons(std::vector<TextButtons>& text_buttons) {
	TextButtons new_game_menu(TextButtonType::NewGame, Pos(20, 20), Pos(30, 30), "New Game", true);
	text_buttons.push_back(new_game_menu);
}