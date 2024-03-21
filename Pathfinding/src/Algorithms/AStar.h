#pragma once

#include "..\DataStructures\Graph.h"
#include <queue>

namespace PriorityQueueAStar
{
	// Struct used to hold runtime information about graph's nodes
	struct NodeRecord
	{
		enum State
		{
			OPEN,
			CLOSED,
			UNVISITED,
		};

		Graph::Node Node;
		Graph::Connection Connection;
		NodeRecord* ParentNode;
		float CostSoFar;
		float EstimatedTotalCost;
		State State;
		NodeRecord();
	};

	// Calculates the Estimated cost from CURRENT node to END node using Euclidean Distance
	float Estimate(Graph::Node current, Graph::Node end);

	// Reverts the resulting path
	void Reverse(std::vector<Graph::Connection>& list);

	// Add connection to the connection list
	void Add(std::vector<Graph::Connection>& list, Graph::Connection connection);

	// Main call to the AStar algorithm
	std::shared_ptr<std::vector<Graph::Connection>> AStar(std::shared_ptr<Graph> graph, Graph::Node start, Graph::Node end);

	// Debug print for the shortes path
	void PrintPath(std::vector<Graph::Connection>* path);
}
