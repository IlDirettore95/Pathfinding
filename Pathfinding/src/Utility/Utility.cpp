#include "Utility.h"
#include "..\Core.h"
#include <memory>
#include <Windows.h>

namespace Utility
{
	Grid::Point GetIndexesFromDirection(Grid::Point start, Direction2D direction)
	{
		int offsetRow = 0;
		int offsetColumn = 0;

		switch (direction)
		{
		case NORTH:
			offsetRow = -1;
			break;
		case NORTH_EAST:
			offsetRow = -1;
			offsetColumn = 1;
			break;
		case EAST:
			offsetColumn = 1;
			break;
		case SOUTH_EAST:
			offsetRow = 1;
			offsetColumn = 1;
			break;
		case SOUTH:
			offsetRow = 1;
			break;
		case SOUTH_WEST:
			offsetRow = 1;
			offsetColumn = -1;
			break;
		case WEST:
			offsetColumn = -1;
			break;
		case NORTH_WEST:
			offsetRow = -1;
			offsetColumn = -1;
			break;
		}

		return Grid::Point(start.I + offsetRow, start.J + offsetColumn);
	}

	std::shared_ptr<Graph> GridToGraph(Grid* grid)
	{
		//PROFILE();
		//PROFILE_MEMORY();
	
		std::shared_ptr<Graph> graph = std::make_shared<Graph>();

		const int height = grid->HEIGHT;
		const int width = grid->WIDTH;

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				Graph::Node node(i * width + j, Graph::Node::Point(j, i));
				graph->AddNode(node);
			}
		}

		for (int i = 1; i < height - 1; i++)
		{
			for (int j = 1; j < width - 1; j++)
			{
				Grid::Point current(i, j);
				if (!grid->IsPassable(current)) continue;

				for (int direction = Direction2D::NORTH; direction <= Direction2D::NORTH_WEST; direction++)
				{
					Grid::Point next = GetIndexesFromDirection(current, (Direction2D)direction);

					if (!grid->IsPassable(next)) continue;

					Graph::Connection connection(i * width + j, next.I * width + next.J, 1);

					graph->AddConnection(connection);
				}
			}
		}

		Direction2D dirFromNorth[5] = { Direction2D::WEST, Direction2D::SOUTH_WEST, Direction2D::SOUTH, Direction2D::SOUTH_EAST, Direction2D::EAST };
		Direction2D dirFromSouth[5] = { Direction2D::WEST, Direction2D::NORTH_WEST, Direction2D::NORTH, Direction2D::NORTH_EAST, Direction2D::EAST };
		Direction2D dirFromWest[5] =  { Direction2D::NORTH, Direction2D::NORTH_EAST, Direction2D::EAST, Direction2D::SOUTH_EAST, Direction2D::SOUTH };
		Direction2D dirFromEast[5] =  { Direction2D::NORTH, Direction2D::NORTH_WEST, Direction2D::WEST, Direction2D::SOUTH_WEST, Direction2D::SOUTH };
		Direction2D dirFromNorthWest[3] = { Direction2D::EAST, Direction2D::SOUTH_EAST, Direction2D::SOUTH };
		Direction2D dirFromNorthEast[3] = { Direction2D::WEST, Direction2D::SOUTH_WEST, Direction2D::SOUTH };
		Direction2D dirFromSouthWest[3] = { Direction2D::NORTH, Direction2D::NORTH_EAST, Direction2D::EAST };
		Direction2D dirFromSouthEast[3] = { Direction2D::WEST, Direction2D::NORTH_WEST, Direction2D::NORTH };

		// North Border
		for (int j = 1; j < width - 1; j++)
		{
			Grid::Point current(0, j);
			if (!grid->IsPassable(current)) continue;

			for (int direction = 0; direction < 5; direction++)
			{
				Grid::Point next = GetIndexesFromDirection(current, dirFromNorth[direction]);

				if (!grid->IsPassable(next)) continue;

				Graph::Connection connection(0 * width + j, next.I * width + next.J, 1);

				graph->AddConnection(connection);
			}
		}

		// South Border
		for (int j = 1; j < width - 1; j++)
		{
			Grid::Point current(height - 1, j);
			if (!grid->IsPassable(current)) continue;

			for (int direction = 0; direction < 5; direction++)
			{
				Grid::Point next = GetIndexesFromDirection(current, dirFromSouth[direction]);

				if (!grid->IsPassable(next)) continue;

				Graph::Connection connection((height - 1) * width + j, next.I * width + next.J, 1);

				graph->AddConnection(connection);
			}
		}

		// West Border
		for (int i = 1; i < height - 1; i++)
		{
			Grid::Point current(i, 0);
			if (!grid->IsPassable(current)) continue;

			for (int direction = 0; direction < 5; direction++)
			{
				Grid::Point next = GetIndexesFromDirection(current, dirFromWest[direction]);

				if (!grid->IsPassable(next)) continue;

				Graph::Connection connection(i * width, next.I * width + next.J, 1);

				graph->AddConnection(connection);
			}
		}

		// East Border
		for (int i = 1; i < height - 1; i++)
		{
			Grid::Point current(i, width - 1);
			if (!grid->IsPassable(current)) continue;
			
			for (int direction = 0; direction < 5; direction++)
			{
				Grid::Point next = GetIndexesFromDirection(current, dirFromEast[direction]);

				if (!grid->IsPassable(next)) continue;

				Graph::Connection connection(i* width + width - 1, next.I* width + next.J, 1);

				graph->AddConnection(connection);
			}
		}
		
		// North West Corner
		Grid::Point current = Grid::Point(0, 0);
		if (grid->IsPassable(current))
		{
			for (int direction = 0; direction < 3; direction++)
			{
				Grid::Point next = GetIndexesFromDirection(current, dirFromNorthWest[direction]);

				if (!grid->IsPassable(next)) continue;

				Graph::Connection connection(0, next.I* width + next.J, 1);

				graph->AddConnection(connection);
			}
		}

		// North East Corner
		current = Grid::Point(0, width - 1);
		if (grid->IsPassable(current))
		{
			for (int direction = 0; direction < 3; direction++)
			{
				Grid::Point next = GetIndexesFromDirection(current, dirFromNorthEast[direction]);

				if (!grid->IsPassable(next)) continue;

				Graph::Connection connection(width - 1, next.I * width + next.J, 1);

				graph->AddConnection(connection);
			}
		}

		// South West Corner
		current = Grid::Point(height - 1, 0);
		if (grid->IsPassable(current))
		{
			for (int direction = 0; direction < 3; direction++)
			{
				Grid::Point next = GetIndexesFromDirection(current, dirFromSouthWest[direction]);

				if (!grid->IsPassable(next)) continue;

				Graph::Connection connection((height - 1)* width, next.I* width + next.J, 1);

				graph->AddConnection(connection);
			}
		}

		// South East Corner
		current = Grid::Point(height - 1, width - 1);
		if (grid->IsPassable(current))
		{
			for (int direction = 0; direction < 3; direction++)
			{
				Grid::Point next = GetIndexesFromDirection(current, dirFromSouthEast[direction]);

				if (!grid->IsPassable(next)) continue;

				Graph::Connection connection((height - 1)* width + width - 1, next.I* width + next.J, 1);

				graph->AddConnection(connection);
			}
		}
		return graph;
	}

	bool Contains(std::shared_ptr<std::vector<Graph::Connection>> list, int nodeID)
	{
		if (!list) return false;

		for (int i = 0; i < list->size(); i++)
		{
			if ((*list)[i].From == nodeID || (*list)[i].To == nodeID)
			{
				return true;
			}
		}

		return false;
	}

	void PrintPath(Grid* grid, std::shared_ptr<Graph> graph, std::shared_ptr<std::vector<Graph::Connection>> path, Graph::Node& startNode, Graph::Node& endNode)
	{
		int height = grid->HEIGHT;
		int width = grid->WIDTH;

		int startNodeID = startNode.ID;
		int endNodeID = endNode.ID;

		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

		for (int i = 1; i <= height * width; i++)
		{
			SetConsoleTextAttribute(h, 8);
			std::cout << "  ";

			if (Contains(path, i - 1))
			{
				if (i - 1 == startNodeID)
				{
					SetConsoleTextAttribute(h, 14);
					std::cout << 'S';
				}
				else if (i - 1 == endNodeID)
				{	
					SetConsoleTextAttribute(h, 14);
					std::cout << 'E';
				}
				else
				{
					SetConsoleTextAttribute(h, 10);
					std::cout << '*';
				}
			}
			else
			{
				if (i - 1 == startNodeID)
				{
					SetConsoleTextAttribute(h, 14);
					std::cout << 'S';
				}
				else if (i - 1 == endNodeID)
				{
					SetConsoleTextAttribute(h, 14);
					std::cout << 'E';
				}
				else
				{
					char element = grid->GetElement(Grid::Point((i - 1) / width, (i - 1) % height));
					switch (element)
					{
					case Grid::CELL_TYPE::PASSABLE:
						std::cout << '.';
						break;
					case Grid::CELL_TYPE::NON_PASSABLE:
						SetConsoleTextAttribute(h, 12);
						std::cout << 'X';
						break;
					default:
						break;
					}
				}

			}

			SetConsoleTextAttribute(h, 15);

			if (i % width == 0)
				std::cout << std::endl;
		}
	}
}