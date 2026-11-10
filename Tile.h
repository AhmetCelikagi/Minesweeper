#pragma once

#include "Position.h"
#include "Settings.h"
#include "GUI.h"

#include <iostream>

class Tile {
private:
	int id;
	int number;
	bool mine;
	bool opened;
	bool flagged;
	bool selected;
	bool flooded;
	bool chorded;
	Pos pos;

public:
	Tile() : id(0), number(0), mine(false), opened(false), flagged(false), selected(false), flooded(false), chorded(false), pos() {}
	Tile(int _id, int _number, bool _mine, bool _opened, bool _flagged, bool _selected, bool _flooded, bool _chorded, Pos _pos) :
		id(_id), number(_number), mine(_mine), opened(_opened), flagged(_flagged), selected(_selected), flooded(_flooded), chorded(_chorded), pos(_pos) {}

	void Display() const;
	bool Open(const Settings& settings, GUI& gui);
	bool IsEmpty();
	int GetId() const;
	int GetNumber() const;
	bool IsMine() const;
	bool IsOpened() const;
	bool IsSelected() const;
	bool IsFlagged() const;
	bool IsFlooded() const;
	bool IsChorded() const;
	Pos GetPos() const;

	void SetOpen();
	void SetFlag();
	void SetFlood();
	void SetChord();
	void SetNumber(const int n);
	void SetMine(const bool m);
	void SetSelected(const bool s);
};