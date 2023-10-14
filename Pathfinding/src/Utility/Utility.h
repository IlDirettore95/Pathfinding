#pragma once

#include "..\DataStructures\Graph.h"
#include "..\DataStructures\Grid.h"

namespace Utility
{
	enum Direction2D
	{
		NORTH,
		NORTH_EAST,
		EAST,
		SOUTH_EAST,
		SOUTH,
		SOUTH_WEST,
		WEST,
		NORTH_WEST
	};

	Grid::Point GetIndexesFromDirection(Grid::Point start, Direction2D direction);

	Graph* GridToGraph(Grid* grid);

	void PrintPath(Grid* grid, Graph* graph, std::vector<Graph::Connection>* path);
}