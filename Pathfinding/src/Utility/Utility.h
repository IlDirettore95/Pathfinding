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

	std::shared_ptr<Graph> GridToGraph(Grid* grid);

	bool Contains(std::shared_ptr<std::vector<Graph::Connection>> list, int nodeID);

	void PrintPath(Grid* grid, std::shared_ptr<Graph> graph, std::shared_ptr<std::vector<Graph::Connection>> path, Graph::Node& startNode, Graph::Node& endNode);
}