#pragma once

#include "..\DataStructures\Graph.h"
#include <queue>

namespace AStar
{
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

	float Estimate(Graph::Node current, Graph::Node end);

	void FindSmallestElement(std::vector<NodeRecord*>& list, NodeRecord** smallest);

	void Reverse(std::vector<Graph::Connection>& list);

	void Add(std::vector<Graph::Connection>& list, Graph::Connection connection);

	std::vector<Graph::Connection>* AStar(Graph& g, Graph::Node start, Graph::Node end);

	void PrintPath(std::vector<Graph::Connection>* path);
}
