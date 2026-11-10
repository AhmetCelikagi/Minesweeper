#pragma once

#include "cmath"

#include <iostream>

struct Pos {
	int row;
	int col;

	Pos() : row(0), col(0) {}
	Pos(int r, int c) : row(r), col(c) {}
	Pos(float r, float c) : Pos((int)r, (int)c){}

	int Size() {
		return row * col;
	}

	int Convert1D(int width) const {
		return row * width + col;
	}

	bool IsValid() const {
		return (row >= 0 && col >= 0);
	}

	bool IsValid(const int height, const int width) {
		return (row < height && row >= 0 && col < width && col >= 0);
	}

	bool SamePos(const Pos& other) const {
		return (row == other.row && col == other.col);
	}

	bool InsideRect(const Pos& rect_pos, const Pos& rect_size) const {
		if (row >= rect_pos.row + rect_size.row || row <= rect_pos.row)
			return false;
		if (col >= rect_pos.col + rect_size.col || col <= rect_pos.col)
			return false;

		return true;
	}

	bool Adjacent9(const Pos& other) const {
		return (abs(other.row - row) <= 1 && abs(other.col - col) <= 1);
	}

	bool Adjacent9(const int ind, const int width) const {
		int other_row = ind / width;
		int other_col = ind % width;

		return Adjacent9(Pos(other_row, other_col));
	}

	void Display() const{
		std::cout << "\nPos: (row, col) : (" << row << ", " << col << ")";
	}
};