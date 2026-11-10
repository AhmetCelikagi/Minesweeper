#include "Mechanics.h"

void Mechanics::MoveCursor(Map& map, const Pos& target) {
	if (!map.ContainPos(target))
		return;

	map.SetSelectedTile(target.row, target.col);
}

void Mechanics::Move(Map& map, Status& status, const Settings& settings, GUI& gui) {
	if (settings.GetMouseOverKeyboard()) {
		MouseMove(map, status, settings, gui);
	}
	else {
		KeyboardMove(map, status, settings, gui);
	}
}

void Mechanics::KeyboardMove(Map& map, Status& status, const Settings& settings, GUI& gui) {
	std::string explanation = "\nPress a key to move (W,A,S,D,P,O,X): ";
	MoveType move = MoveType::None;
	while (!gui.GetMove(move, settings, explanation));
	int selected_tile_id = map.GetSelectedTile()->GetId();
	Pos selected_tile_pos = map.GetSelectedTile()->GetPos();
	Pos target_pos = selected_tile_pos;

	if (move == MoveType::Exit) {
		status = Status::Exiting;
	}
	else if (move == MoveType::Up) {
		target_pos.row--;
		MoveCursor(map, target_pos);
	}
	else if (move == MoveType::Left) {
		target_pos.col--;
		MoveCursor(map, target_pos);
	}
	else if (move == MoveType::Down) {
		target_pos.row++;
		MoveCursor(map, target_pos);
	}
	else if (move == MoveType::Right) {
		target_pos.col++;
		MoveCursor(map, target_pos);
	}
	else if (move == MoveType::Flag) {
		map.SetFlagOnSelected();
	}
	else if (move == MoveType::Open) {
		map.OpenTile(settings, gui);
	}
	else if (move == MoveType::None) {
		std::cout << "\nError occured";
	}
}

void Mechanics::MouseMove(Map& map, Status& status, const Settings& settings, GUI& gui) {
	Pos clicked_tile_ind;
	ClickResult click_result = gui.GetClick(clicked_tile_ind, status, settings);

	if (click_result == ClickResult::TileOpened) {
		MoveCursor(map, clicked_tile_ind);
		map.OpenTile(settings, gui);
		std::cout << "\nOpened";
	}

	else if (click_result == ClickResult::TileFlagged) {
		MoveCursor(map, clicked_tile_ind);
		map.SetFlagOnSelected();
		std::cout << "\nFlagged";
	}

	else if (click_result == ClickResult::ExitClicked) {
		status = Status::Exiting;
		std::cout << "\nExitted";
	}

	else if (click_result == ClickResult::SolutionClicked) {
		if (status == Status::Won || status == Status::Lost)
			status = Status::Solution;
		else
			status = Status::Playing;
		std::cout << "\nSolution showing";
	}

	else if (click_result == ClickResult::SaveClicked) {
		status = Status::Saving;
		std::cout << "\nSaving";
	}
}