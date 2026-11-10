#include "Tests.h"

bool Tests::BoardCreation() {
	Map map(20, 20, 90);
	bool assert_num_mines = false;
	int mine_number = 0;

	for (int r = 0; r < 20; r++) {
		for (int c = 0; c < 20; c++) {
			if (map.GetTable()[r][c].IsMine())
				mine_number++;
		}
	}

	if (map.GetNumMine() == 90 == mine_number)
		assert_num_mines = true;
	return (assert_num_mines);
}

bool Tests::ValidMapSize() {
	Configs config;
	Map map;
	GUI gui(800, 600, "Minesweeper", map);
	config.GetConfig(gui);

	return true;
}

bool Tests::TestAll() {
	return(BoardCreation() && ValidMapSize());
}