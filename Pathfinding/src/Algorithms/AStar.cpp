#include <cmath>
#include "AStar.h"
#include "..\Core.h"
#include "..\Tools\Logger.h"
#include <memory>

namespace AStar
{
	NodeRecord::NodeRecord()
		: Node(NULL_NODE), Connection(NULL_CONNECTION), ParentNode(nullptr), CostSoFar(INFINITY), EstimatedTotalCost(INFINITY), State(NodeRecord::State::UNVISITED) {}

	float Estimate(Graph::Node current, Graph::Node end)
	{
		//PROFILE();
		//PROFILE_MEMORY();
		return (float)std::sqrt(std::pow(current.Position.X - end.Position.X, 2) + std::pow(current.Position.Y - end.Position.Y, 2));
	}

	void Reverse(std::vector<Graph::Connection>& list)
	{
		for (int i = 0; i < list.size() / 2; i++)
		{
			Graph::Connection temp = list[i];
			list[i] = list[list.size() - 1 - i];
			list[list.size() - 1 - i] = temp;
		}
	}

	void Add(std::vector<Graph::Connection>& list, Graph::Connection connection)
	{
		list.push_back(connection);
	}

	std::shared_ptr<std::vector<Graph::Connection>> AStar(std::shared_ptr<Graph> graph, Graph::Node start, Graph::Node end)
	{

		std::unique_ptr<NodeRecord[]> NodeRecordArray = std::make_unique<NodeRecord[]>(graph->GetNodes()->size());
		for (int i = 0; i < graph->GetNodes()->size(); i++)
		{
			NodeRecordArray[i].Node = graph->GetNodes()->at(i);
			NodeRecordArray[i].Connection = Graph::Connection(NULL_CONNECTION);
			NodeRecordArray[i].ParentNode = nullptr;
			NodeRecordArray[i].CostSoFar = 0;
			NodeRecordArray[i].EstimatedTotalCost = INFINITY;
			NodeRecordArray[i].State = NodeRecord::State::UNVISITED;
		}

		//PROFILE();
		//PROFILE_MEMORY();

		NodeRecord* startRecord = &NodeRecordArray[start.ID];
		startRecord->State = NodeRecord::State::OPEN;
		startRecord->EstimatedTotalCost = Estimate(start, end);
		auto cmp = [] (NodeRecord* left, NodeRecord* right) { return (left->EstimatedTotalCost) > (right->EstimatedTotalCost); };
		std::priority_queue<NodeRecord*, std::vector<NodeRecord*>, decltype(cmp)> open(cmp);

		NodeRecord* current = startRecord;
		
		open.push(current);

		while (open.size() > 0)
		{
			current = open.top();
			open.pop();

			if (current->Node.ID == end.ID)
				break;

			std::vector<Graph::Connection>* connections = graph->GetConnections((*current).Node);

			for (Graph::Connection connection : *connections)
			{
				Graph::Node& endNode = graph->GetNodes()->at(connection.To);
				float endNodeCost = current->CostSoFar + connection.Cost;
				float endNodeHeuristic = INFINITY;

				NodeRecord* endNodeRecord = &NodeRecordArray[endNode.ID];

				switch (endNodeRecord->State)
				{
				case NodeRecord::State::CLOSED:
				case NodeRecord::State::OPEN:
					if (endNodeRecord->CostSoFar <= endNodeCost)
						continue;

					endNodeHeuristic = endNodeRecord->EstimatedTotalCost - endNodeRecord->CostSoFar;
					break;
				case NodeRecord::State::UNVISITED:
					endNodeHeuristic = Estimate(endNode, end);
					break;
				}
				endNodeRecord->Connection = connection;
				endNodeRecord->ParentNode = current;
				endNodeRecord->CostSoFar = endNodeCost;
				endNodeRecord->EstimatedTotalCost = endNodeCost + endNodeHeuristic;
				if (endNodeRecord->State != NodeRecord::State::OPEN)
				{
					endNodeRecord->State = NodeRecord::State::OPEN;
					open.push(endNodeRecord);
				}
			}
			current->State = NodeRecord::State::CLOSED;
		}

		if (current->Node.ID != end.ID)
			return nullptr;

		std::shared_ptr<std::vector<Graph::Connection>> path = std::make_shared<std::vector<Graph::Connection>>();
		//std::vector<Graph::Connection>* path = new std::vector<Graph::Connection>;

		while (current->Node.ID != start.ID)
		{
			Add(*path, current->Connection);
			current = current->ParentNode;
		}
		
		Reverse(*path);

		//DEBUG("A_STAR (Fill %): ");
		//DEBUG((open.size() + closed.size()) / (float)graph.GetNodes()->size());
		return path;
	}

	void PrintPath(std::vector<Graph::Connection>* path)
	{
		std::cout << "Shortest Path: " << std::endl;
		for (int i = 0; i < path->size(); i++)
		{
			Graph::Connection c = (*path)[i];
			std::cout << "\t{" << c.From << ", " << c.To << "}" << std::endl;
		}
	}
}
