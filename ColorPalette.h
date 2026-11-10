#pragma once

struct ColorPalette {
	sf::Color error_color = sf::Color(100, 50, 50);
	sf::Color background_color = sf::Color(0, 0, 0);
	sf::Color opened_color = sf::Color(100, 100, 100);
	sf::Color closed_color = sf::Color(200, 200, 200);
	sf::Color selected_color = sf::Color(180, 255, 180);
	sf::Color mine_color = sf::Color(255, 180, 180);
	sf::Color hint_1_color = sf::Color(255, 0, 0);
	sf::Color hint_2_color = sf::Color(0, 255, 0);
	sf::Color hint_3_color = sf::Color(0, 0, 255);
	sf::Color hint_4_color = sf::Color(255, 255, 0);
	sf::Color hint_5_color = sf::Color(255, 0, 255);
	sf::Color hint_6_color = sf::Color(0, 255, 255);
	sf::Color hint_7_color = sf::Color(180, 180, 0);
	sf::Color hint_8_color = sf::Color(180, 0, 180);
	sf::Color default_hint_color = sf::Color(10, 10, 10);
};
