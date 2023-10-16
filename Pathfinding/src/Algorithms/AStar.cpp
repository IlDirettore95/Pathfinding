#include <cmath>
#include "AStar.h"
#include "..\Tools\Profiling.h"
#include "..\Tools\Logger.h"
#include <memory>

namespace AStar
{
	NodeRecord::NodeRecord()
		: Node(NULL_NODE), Connection(NULL_CONNECTION(Graph::Node(NULL_NODE))), ParentNode(nullptr), CostSoFar(INFINITY), EstimatedTotalCost(INFINITY) {}

	NodeRecord::NodeRecord(Graph::Node node, Graph::Connection connection, NodeRecord* parentNode, float costSoFar, float estimatedTotalCost)
		: Node(node), Connection(connection), ParentNode(parentNode), CostSoFar(costSoFar), EstimatedTotalCost(estimatedTotalCost) {}

	float Estimate(Graph::Node current, Graph::Node end)
	{
		START_ACC_PROFILE("EuclideanDistance")
		return (float)std::sqrt(std::pow(current.Position.X - end.Position.X, 2) + std::pow(current.Position.Y - end.Position.Y, 2));
		END_ACC_PROFILE
	}

	void FindSmallestElement(std::vector<NodeRecord*>& list, NodeRecord** smallest)
	{
		START_ACC_PROFILE("FindSmallestElement");

		*smallest = list[0];

		for (int i = 1; i < list.size(); i++)
		{
			*smallest = list[i]->EstimatedTotalCost < (*smallest)->EstimatedTotalCost ? list[i] : *smallest;
		}

		END_ACC_PROFILE
	}

	void Reverse(std::vector<Graph::Connection>& list)
	{
		START_ACC_PROFILE("Reverse");

		for (int i = 0; i < list.size() / 2; i++)
		{
			Graph::Connection temp = list[i];
			list[i] = list[list.size() - 1 - i];
			list[list.size() - 1 - i] = temp;
		}

		END_ACC_PROFILE
	}

	void Add(std::vector<Graph::Connection>& list, Graph::Connection connection)
	{
		START_ACC_PROFILE("AddNode");
		list.push_back(connection);
		END_ACC_PROFILE
	}

	void Add(std::vector<NodeRecord*>& list, NodeRecord* node)
	{
		START_ACC_PROFILE("AddNode");
		list.push_back(node);
		END_ACC_PROFILE
	}

	void Erase(std::vector<NodeRecord*>& list, NodeRecord* current)
	{
		START_ACC_PROFILE("EraseNode");
		
		for (int i = 0; i < list.size(); i++)
		{
			if (list[i]->Node.ID == current->Node.ID)
			{
				list.erase(list.begin() + i);
				break;
			}
		}

		END_ACC_PROFILE
	}

	std::vector<Graph::Connection>* AStar(Graph& graph, Graph::Node start, Graph::Node end)
	{
		std::unique_ptr<NodeRecord[]> NodeRecordArray = std::make_unique<NodeRecord[]>(graph.GetNodes()->size());
		for (int i = 0; i < graph.GetNodes()->size(); i++)
		{
			NodeRecordArray[i].Node = graph.GetNodes()->at(i);
			NodeRecordArray[i].Connection = Graph::Connection(NULL_CONNECTION(Graph::Node(NULL_NODE)));
			NodeRecordArray[i].ParentNode = nullptr;
			NodeRecordArray[i].CostSoFar = 0;
			NodeRecordArray[i].EstimatedTotalCost = INFINITY;
			NodeRecordArray[i].State = NodeRecord::State::UNVISITED;
		}

		START_PROFILE("A_STAR");

		NodeRecord* startRecord = &NodeRecordArray[start.ID];
		startRecord->State = NodeRecord::State::OPEN;
		std::vector<NodeRecord*> open;
		Add(open, startRecord);

		NodeRecord* current = startRecord;

		while (open.size() > 0)
		{
			FindSmallestElement(open, &current);

			if (current->Node.ID == end.ID)
				break;

			std::vector<Graph::Connection>* connections = graph.GetConnections((*current).Node);

			for (Graph::Connection connection : *connections)
			{
				Graph::Node& endNode = connection.To;
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
					Add(open, endNodeRecord);
				}
			}

			Erase(open, current);
			current->State = NodeRecord::State::CLOSED;
		}

		if (current->Node.ID != end.ID)
			return nullptr;

		std::vector<Graph::Connection>* path = new std::vector<Graph::Connection>;

		while (current->Node.ID != start.ID)
		{
			Add(*path, current->Connection);
			current = current->ParentNode;
		}
		
		Reverse(*path);

		//DEBUG("A_STAR (Fill %): ");
		//DEBUG((open.size() + closed.size()) / (float)graph.GetNodes()->size());
		return path;
		END_PROFILE
	}

	void PrintPath(std::vector<Graph::Connection>* path)
	{
		std::cout << "Shortest Path: " << std::endl;
		for (int i = 0; i < path->size(); i++)
		{
			Graph::Connection c = (*path)[i];
			std::cout << "\t{" << c.From.ID << ", " << c.To.ID << "}" << std::endl;
		}
	}
}