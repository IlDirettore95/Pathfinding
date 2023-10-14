#pragma once

class Grid
{
public:
	struct Point
	{
		int I;
		int J;

		Point(int i, int j);
	};

	enum CELL_TYPE
	{
		PASSABLE,
		NON_PASSABLE
	};
public:
	const int HEIGHT;
	const int WIDTH;

private:
	char* m_grid;

public:
	Grid(const int height, const int width);

	~Grid();

	char GetElement(Point point);

	bool IsPassable(Point point);

	void PlaceObstacle(Point point);

	void Print();
};
