#pragma once

#include <memory>
#include <vector>
#include <utility>

class Desk {
public:
	Desk();
	Desk(const Desk& d);
	~Desk();

	Desk operator=(const Desk& d);

	bool checkMove(int x, int y, int color) const;
	bool checkAnyMove(int color) const;
	bool makeMove(int x, int y);

	int getCurrentColor() const;
	const int* const* getDeskState() const;
	std::vector<std::pair<int, int>> getPossibleMoves() const;

private:
	int* fieldData;
	int** field;
	int currentColor;

	int distanceNearest(int x, int y, int color, int dx, int dy) const;
};

