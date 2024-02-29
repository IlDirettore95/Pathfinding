#include <iostream>
#include <vector>
#include "Pathfinding.h"
#include <memory>
#include <Windows.h>

#define GRID_HEIGHT 20
#define GRID_WIDTH 20

int main()
{
	std::cout << "Running Pathfinding version= " << "\"" << VERSION << "\"" << std::endl << std::endl;
	{
		//MEMORY_PROFILER_SESSION("Results.txt");
		//PROFILE();
		//PROFILE_MEMORY();

		Grid grid(GRID_HEIGHT, GRID_WIDTH);
		grid.PlaceObstacle(Grid::Point(1, 4));
		grid.PlaceObstacle(Grid::Point(1, 5));
		grid.PlaceObstacle(Grid::Point(2, 5));
		grid.PlaceObstacle(Grid::Point(3, 5));
		grid.PlaceObstacle(Grid::Point(4, 5));
		grid.PlaceObstacle(Grid::Point(5, 5));
		grid.PlaceObstacle(Grid::Point(6, 5));
		grid.PlaceObstacle(Grid::Point(6, 4));
		grid.PlaceObstacle(Grid::Point(9, 7));
		grid.PlaceObstacle(Grid::Point(8, 7));
		grid.PlaceObstacle(Grid::Point(7, 7));
		grid.PlaceObstacle(Grid::Point(6, 7));
		grid.PlaceObstacle(Grid::Point(9, 7));
		grid.PlaceObstacle(Grid::Point(9, 6));
		grid.PlaceObstacle(Grid::Point(9, 5));
		grid.PlaceObstacle(Grid::Point(16, 16));
		grid.PlaceObstacle(Grid::Point(16, 15));
		grid.PlaceObstacle(Grid::Point(16, 14));
		grid.PlaceObstacle(Grid::Point(17, 13));
		grid.PlaceObstacle(Grid::Point(17, 14));
		grid.PlaceObstacle(Grid::Point(18, 13));
		grid.PlaceObstacle(Grid::Point(19, 13));
		grid.PlaceObstacle(Grid::Point(7, 4));
		grid.PlaceObstacle(Grid::Point(7, 5));
		grid.PlaceObstacle(Grid::Point(10, 4));
		grid.PlaceObstacle(Grid::Point(10, 5));
		grid.PlaceObstacle(Grid::Point(10, 6));
		grid.PlaceObstacle(Grid::Point(10, 3));
		grid.PlaceObstacle(Grid::Point(10, 2));
		grid.PlaceObstacle(Grid::Point(0, 5));
		grid.PlaceObstacle(Grid::Point(16, 13));
		grid.PlaceObstacle(Grid::Point(16, 12));
		grid.PlaceObstacle(Grid::Point(16, 11));
		grid.PlaceObstacle(Grid::Point(16, 10));
		grid.PlaceObstacle(Grid::Point(16, 9));
		grid.PlaceObstacle(Grid::Point(15, 12));
		grid.PlaceObstacle(Grid::Point(13, 12));
		grid.PlaceObstacle(Grid::Point(14, 12));
		grid.PlaceObstacle(Grid::Point(12, 12));
		grid.PlaceObstacle(Grid::Point(11, 12));
		grid.PlaceObstacle(Grid::Point(10, 12));
		grid.PlaceObstacle(Grid::Point(9, 12));
		grid.PlaceObstacle(Grid::Point(9, 15));
		grid.PlaceObstacle(Grid::Point(8, 15));
		grid.PlaceObstacle(Grid::Point(7, 15));
		grid.PlaceObstacle(Grid::Point(10, 15));
		grid.PlaceObstacle(Grid::Point(11, 15));
		grid.PlaceObstacle(Grid::Point(6, 15));
		grid.PlaceObstacle(Grid::Point(5, 15));
		grid.PlaceObstacle(Grid::Point(12, 15));
		grid.PlaceObstacle(Grid::Point(13, 15));

		std::shared_ptr<Graph> graph = Utility::GridToGraph(&grid);
		//graph->Print();

		Graph::Node start = graph->GetNodes()->at(0);
		Graph::Node end = graph->GetNodes()->at((GRID_HEIGHT * GRID_WIDTH) - 1);
		
		std::shared_ptr<std::vector<Graph::Connection>> shortestPath;
		std::shared_ptr<std::vector<Graph::Connection>> path = std::make_shared<std::vector<Graph::Connection>>();
		Utility::PrintPath(&grid, graph, shortestPath, start, end);

		std::cin.get();
		system("cls");

		shortestPath = AStar::AStar(graph, start, end);

		for (int i = 0; i < shortestPath->size(); i++)
		{
			path->push_back(shortestPath->at(i));

			system("cls");

			Utility::PrintPath(&grid, graph, path, start, end);

			Sleep(200);
		}

		std::cout << "GOAL REACHED" << std::endl;
	}
	//PRINT_PROFILING_RESULTS();
	//PRINT_MEMORY_PROFILER_RESULTS();
}
