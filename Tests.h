#pragma once

#include <string>

#include "MapGen.h"
#include "Config.h"
#include "GUI.h"

class Tests {
private:
public:
	bool TestAll();
	bool BoardCreation();
	bool ValidMapSize();
};
