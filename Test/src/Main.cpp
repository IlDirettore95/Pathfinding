#include <iostream>
#include <vector>
#include "Pathfinding.h"
#include <memory>
#include <cmath>

// Grid size macro
#define GRID_HEIGHT 2000
#define GRID_WIDTH 2000

enum AlgorithmTypes
{
	None,
	ESimpleAStar,
	ENodeArrayAStar,
	EPriorityQueueAStar
};

AlgorithmTypes chosenAlgorithm = AlgorithmTypes::ESimpleAStar;

bool ChoseAlgorithm()
{
	std::cout << "Available algorithms:" << "\n\t1. Simple A*" << "\n\t2. Node Array A*" << "\n\t3. Priority Queue A*" << std::endl;
	std::cout << "Enter the number of the chosen algorithm and clik enter." << std::endl;

	int number = -1;
	std::cin >> number;

	if (std::cin.fail() || number < AlgorithmTypes::ESimpleAStar || number > AlgorithmTypes::EPriorityQueueAStar)
	{
		std::cout << "You must enter a valid algorithm type!";
		std::cin.get();
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return false;
	}
	chosenAlgorithm = (AlgorithmTypes)number;
	return true;
}

int main()
{
	std::cout << "Running Pathfinding on a " << GRID_HEIGHT << " x " << GRID_WIDTH << " grid" << std::endl;

	for (int algType = AlgorithmTypes::ESimpleAStar; algType <= AlgorithmTypes::EPriorityQueueAStar; algType++)
	{
		{
			// Profiler inizialization
			//MEMORY_PROFILER_SESSION("Results.txt");
			//PROFILE();
			//PROFILE_MEMORY();

			// Grid creation
			Grid grid(GRID_HEIGHT, GRID_WIDTH);

			// GRID OBSTACLES
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
			//grid.PlaceObstacle(Grid::Point(9, 7));
			//grid.PlaceObstacle(Grid::Point(9, 6));
			//grid.PlaceObstacle(Grid::Point(9, 5));
			//grid.PlaceObstacle(Grid::Point(16, 16));
			//grid.PlaceObstacle(Grid::Point(16, 15));
			//grid.PlaceObstacle(Grid::Point(16, 14));
			//grid.PlaceObstacle(Grid::Point(17, 13));
			//grid.PlaceObstacle(Grid::Point(17, 14));
			//grid.PlaceObstacle(Grid::Point(18, 13));
			//grid.PlaceObstacle(Grid::Point(19, 13));
			//grid.PlaceObstacle(Grid::Point(7, 4));
			//grid.PlaceObstacle(Grid::Point(7, 5));
			//grid.PlaceObstacle(Grid::Point(10, 4));
			//grid.PlaceObstacle(Grid::Point(10, 5));
			//grid.PlaceObstacle(Grid::Point(10, 6));
			//grid.PlaceObstacle(Grid::Point(10, 3));
			//grid.PlaceObstacle(Grid::Point(10, 2));
			//grid.PlaceObstacle(Grid::Point(0, 5));
			//grid.PlaceObstacle(Grid::Point(16, 13));
			//grid.PlaceObstacle(Grid::Point(16, 12));
			//grid.PlaceObstacle(Grid::Point(16, 11));
			//grid.PlaceObstacle(Grid::Point(16, 10));
			//grid.PlaceObstacle(Grid::Point(16, 9));
			//grid.PlaceObstacle(Grid::Point(15, 12));
			//grid.PlaceObstacle(Grid::Point(13, 12));
			//grid.PlaceObstacle(Grid::Point(14, 12));
			//grid.PlaceObstacle(Grid::Point(12, 12));
			//grid.PlaceObstacle(Grid::Point(11, 12));
			//grid.PlaceObstacle(Grid::Point(10, 12));
			//grid.PlaceObstacle(Grid::Point(9, 12));
			//grid.PlaceObstacle(Grid::Point(9, 15));
			//grid.PlaceObstacle(Grid::Point(8, 15));
			//grid.PlaceObstacle(Grid::Point(7, 15));
			//grid.PlaceObstacle(Grid::Point(10, 15));
			//grid.PlaceObstacle(Grid::Point(11, 15));
			//grid.PlaceObstacle(Grid::Point(6, 15));
			//grid.PlaceObstacle(Grid::Point(5, 15));
			//grid.PlaceObstacle(Grid::Point(12, 15));
			//grid.PlaceObstacle(Grid::Point(13, 15));

			// Conversion from a GRID to a GRAPH since the algorithm requires the latter
			std::shared_ptr<Graph> graph = Utility::GridToGraph(&grid);

			//graph->Print();

			Graph::Node start = graph->GetNodes()->at(0);
			Graph::Node end = graph->GetNodes()->at((GRID_HEIGHT * GRID_WIDTH) - 1);

			// Call the A-Star algorithm that returns a vector of Connections that go from the START node to the END node of the Graph

			std::shared_ptr<std::vector<Graph::Connection>> shortestPath;

			switch (algType)
			{
			case AlgorithmTypes::ESimpleAStar:
			{
				shortestPath = SimpleAStar::AStar(graph, start, end);
				break;
			}
			case AlgorithmTypes::ENodeArrayAStar:
			{
				shortestPath = NodeArrayAStar::AStar(graph, start, end);
				break;
			}
			case AlgorithmTypes::EPriorityQueueAStar:
			{
				shortestPath = PriorityQueueAStar::AStar(graph, start, end);
				break;
			}
			}

			//Utility::PrintPath(&grid, graph, shortestPath, start, end);

			// Print of the profiler results
			//PRINT_MEMORY_PROFILER_RESULTS();
		}
		PRINT_PROFILING_RESULTS();
	}	
}
