#include <iostream>
#include "Grid.h"
#include "..\Core.h"

Grid::Grid(const int height, const int width)
	: m_grid(nullptr), HEIGHT(height), WIDTH(width)
{
	//PROFILE_MEMORY();
	//PROFILE();
	m_grid = new char[HEIGHT * WIDTH];

	for (int i = 0; i < HEIGHT * WIDTH; i++)
	{
		m_grid[i] = CELL_TYPE::PASSABLE;
	}
}

Grid::~Grid()
{
	delete[] m_grid;
}

char Grid::GetElement(Point point)
{
	if (point.J < 0 || point.J > WIDTH - 1 || point.I < 0 || point.I > HEIGHT - 1)
	{
		std::cout << "Cannot check point outside the grid!" << std::endl;
		return false;
	}

	return m_grid[point.I * WIDTH + point.J];
}

bool Grid::IsPassable(Point point)
{
	if (point.J < 0 || point.J > WIDTH - 1 || point.I < 0 || point.I > HEIGHT - 1)
	{
		std::cout << "Cannot check point outside the grid!" << std::endl;
		return false;
	}

	return m_grid[point.I * WIDTH + point.J] == CELL_TYPE::PASSABLE;
}

void Grid::PlaceObstacle(Point point)
{
	if (point.J < 0 || point.J > WIDTH - 1 || point.I < 0 || point.I > HEIGHT - 1)
	{
		std::cout << "Cannot place a point outside the grid!" << std::endl;
		return;
	}

	m_grid[point.I * WIDTH + point.J] = CELL_TYPE::NON_PASSABLE;
}

void Grid::Print()
{
	std::cout << "Grid: " << std::endl << std::endl;
	for (int i = 1; i <= HEIGHT * WIDTH; i++)
	{
		std::cout << "  ";
		switch (m_grid[i - 1])
		{
		case CELL_TYPE::PASSABLE:
			std::cout << '-';
			break;
		case CELL_TYPE::NON_PASSABLE:
			std::cout << 'X';
			break;
		default:
			break;
		}
		if (i % WIDTH == 0)
			std::cout << std::endl;
	}
}

Grid::Point::Point(int i, int j)
{
	I = i;
	J = j;
}