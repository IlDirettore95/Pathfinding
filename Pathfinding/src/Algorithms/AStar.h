#pragma once

#include "..\DataStructures\Graph.h"

namespace AStar
{
	struct NodeRecord
	{
		Graph::Node Node;
		Graph::Connection Connection;
		NodeRecord* ParentNode;
		float CostSoFar;
		float EstimatedTotalCost;

		NodeRecord();

		NodeRecord(Graph::Node node, Graph::Connection connection, NodeRecord* parentNode, float costSoFar, float estimatedTotalCost);
	};

	float Estimate(Graph::Node current, Graph::Node end);

	void FindSmallestElement(std::vector<NodeRecord*>& list, NodeRecord** smallest);

	void Reverse(std::vector<Graph::Connection>& list);

	void Add(std::vector<Graph::Connection>& list, Graph::Connection connection);

	void Add(std::vector<NodeRecord*>& list, NodeRecord* node);

	void Erase(std::vector<NodeRecord*>& list, int index);

	void Erase(std::vector<NodeRecord*>& list, NodeRecord* current);

	bool Contains(std::vector<NodeRecord*>& list, Graph::Node& node);

	bool ContainsClosed(std::vector<NodeRecord*>& list, Graph::Node& node, NodeRecord** nodeRecord, int* nodeIndex);

	bool ContainsOpen(std::vector<NodeRecord*>& list, Graph::Node& node, NodeRecord** nodeRecord, int* nodeIndex);

	std::vector<Graph::Connection>* AStar(Graph& g, Graph::Node start, Graph::Node end);

	void PrintPath(std::vector<Graph::Connection>* path);
}
