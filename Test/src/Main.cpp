#include <iostream>
#include <vector>
#include "Pathfinding.h"
#include <memory>

#define GRID_HEIGHT 1000
#define GRID_WIDTH 1000

int main()
{
	std::cout << "Running Pathfinding version= " << "\"" << VERSION << "\"" << std::endl << std::endl;
	{
		MEMORY_PROFILER_SESSION("Results.txt");
		PROFILE();
		PROFILE_MEMORY();

		Grid grid(GRID_HEIGHT, GRID_WIDTH);
		//grid.PlaceObstacle(Grid::Point(1, 4));
		//grid.PlaceObstacle(Grid::Point(1, 5));
		//grid.PlaceObstacle(Grid::Point(2, 5));
		//grid.PlaceObstacle(Grid::Point(3, 5));
		//grid.PlaceObstacle(Grid::Point(4, 5));
		//grid.PlaceObstacle(Grid::Point(5, 5));
		//grid.PlaceObstacle(Grid::Point(6, 5));
		//grid.PlaceObstacle(Grid::Point(6, 4));
		//grid.PlaceObstacle(Grid::Point(9, 7));
		//grid.PlaceObstacle(Grid::Point(8, 7));
		//grid.PlaceObstacle(Grid::Point(7, 7));
		//grid.PlaceObstacle(Grid::Point(6, 7));
		//grid.PlaceObstacle(Grid::Point(5, 7));

		std::shared_ptr<Graph> graph = Utility::GridToGraph(&grid);
		//graph->Print();

		Graph::Node start = graph->GetNodes()->at(0);
		Graph::Node end = graph->GetNodes()->at((GRID_HEIGHT * GRID_WIDTH) - 1);
		std::shared_ptr<std::vector<Graph::Connection>> shortestPath = AStar::AStar(graph, start, end);
		//Utility::PrintPath(&grid, graph, shortestPath);
	}
	PRINT_PROFILING_RESULTS();
	PRINT_MEMORY_PROFILER_RESULTS();
}
