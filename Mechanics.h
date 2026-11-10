#pragma once

#include "MapGen.h"
#include "Position.h"
#include "ClickResult.h"
#include "MoveType.h"
#include "GUI.h"

#include <iostream>

class Mechanics {
private:
public:
	void Move(Map& map, Status& status, const Settings& settings, GUI& gui);
	void KeyboardMove(Map& map, Status& status, const Settings& settings, GUI& gui);
	void MouseMove(Map& map, Status& status, const Settings& settings, GUI& gui);
	void MoveCursor(Map& map, const Pos& target);
};
