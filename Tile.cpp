#include "Tile.h"

void Tile::Display() const {
	std::cout << "Id: " << id << ", number: " << number << (mine ? " mine" : " no mine") << (opened ? " open" : " close") << (flagged ? " flagged" : " not flagged") << std::endl;
}

bool Tile::IsEmpty() {
	if (number == 0)
		return true;
	return false;
}

bool Tile::Open(const Settings& settings, GUI& gui) {
	if (opened) {
		return false;
	}

	if (flagged && not settings.GetOpenFlagged()) {
		return false;
	}

	opened = true;
	return true;
}

int Tile::GetId() const {
	return id;
}

int Tile::GetNumber() const {
	return number;
}

bool Tile::IsMine() const {
	return mine;
}

bool Tile::IsOpened() const {
	return opened;
}

bool Tile::IsSelected() const {
	return selected;
}

bool Tile::IsFlagged() const {
	return flagged;
}

bool Tile::IsFlooded() const {
	return flooded;
}

bool Tile::IsChorded() const {
	return chorded;
}

Pos Tile::GetPos() const {
	return pos;
}

void Tile::SetOpen() {
	opened = true;
}

void Tile::SetFlag() {
	if (IsOpened())
		return;

	if (flagged) {
		flagged = false;
	}
	else {
		flagged = true;
	}
}

void Tile::SetFlood() {
	flooded = true;
}

void Tile::SetChord() {
	chorded = true;
}

void Tile::SetNumber(const int n) {
	number = n;
}

void Tile::SetMine(const bool m) {
	mine = m;
}

void Tile::SetSelected(const bool s) {
	selected = s;
}