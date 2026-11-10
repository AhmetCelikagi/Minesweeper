#pragma once

#include "Position.h"
#include <string>

enum class TextButtonType {
	NewGame,
	LoadGame,
	Settings,
	Statistics,
	Exit
};

struct TextButtons {
	Pos pos;
	Pos size;
	std::string text;
	bool is_active;
	bool clicked;

	TextButtons(const TextButtonType&, const Pos& p, const Pos& s, const std::string t, bool a) 
		: pos(p), size(s), text(t), is_active(a), clicked(false) {}

	void Activate() {
		is_active = true;
	}

	void Deactivate() {
		is_active = false;
	}

	void Clear() {
		text = "";
	}

	void UpdateText(const std::string new_text) {
		text = new_text;
	}
};
