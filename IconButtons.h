#pragma once

#include "Position.h"

enum class IconButtonType {
	ExitButton,
	SaveButton,
	SolutionButton
};

struct IconButtons {
	IconButtonType type;
	Pos pos;
	Pos size;
	sf::Texture icon;
	bool is_active;
	bool clicked;

	IconButtons(const IconButtonType& t, const Pos& p, const Pos& s, const sf::Texture& i, const bool a) 
		: type(t), pos(p), size(s), icon(i), is_active(a), clicked(false) {}
	void Activate() {
		is_active = true;
	}
	void Deactivate() {
		is_active = false;
	}
};
