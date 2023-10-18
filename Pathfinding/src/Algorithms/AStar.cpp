#include <cmath>
#include "AStar.h"
#include "..\Tools\Profiling.h"
#include "..\Tools\Logger.h"

namespace AStar
{
	NodeRecord::NodeRecord()
		: Node(NULL_NODE), Connection(NULL_CONNECTION(Graph::Node(NULL_NODE))), ParentNode(nullptr), CostSoFar(INFINITY), EstimatedTotalCost(INFINITY) {}

	NodeRecord::NodeRecord(Graph::Node node, Graph::Connection connection, NodeRecord* parentNode, float costSoFar, float estimatedTotalCost)
		: Node(node), Connection(connection), ParentNode(parentNode), CostSoFar(costSoFar), EstimatedTotalCost(estimatedTotalCost) {}

	float Estimate(Graph::Node current, Graph::Node end)
	{
		START_PROFILE();
		return (float)std::sqrt(std::pow(current.Position.X - end.Position.X, 2) + std::pow(current.Position.Y - end.Position.Y, 2));
	}

	void FindSmallestElement(std::vector<NodeRecord*>& list, NodeRecord** smallest)
	{
		START_PROFILE();
		*smallest = list[0];

		for (int i = 1; i < list.size(); i++)
		{
			*smallest = list[i]->EstimatedTotalCost < (*smallest)->EstimatedTotalCost ? list[i] : *smallest;
		}
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

	void Add(std::vector<NodeRecord*>& list, NodeRecord* node)
	{
		list.push_back(node);
	}

	void Erase(std::vector<NodeRecord*>& list, int index)
	{
		START_PROFILE();
		list.erase(list.begin() + index);
	}

	void Erase(std::vector<NodeRecord*>& list, NodeRecord* current)
	{		
		START_PROFILE();
		for (int i = 0; i < list.size(); i++)
		{
			if (list[i]->Node.ID == current->Node.ID)
			{
				list.erase(list.begin() + i);
				break;
			}
		}
	}

	bool Contains(std::vector<NodeRecord*>& list, Graph::Node& node)
	{
		START_PROFILE();
		for (int i = 0; i < list.size(); i++)
		{
			if (list[i]->Node.ID == node.ID)
			{
				return true;
			}
		}

		return false;
	}

	bool ContainsClosed(std::vector<NodeRecord*>& list, Graph::Node& node, NodeRecord** nodeRecord, int* nodeIndex)
	{
		START_PROFILE();
		for (int i = 0; i < list.size(); i++)
		{
			if (list[i]->Node.ID == node.ID)
			{
				*nodeRecord = list[i];
				*nodeIndex = i;
				return true;
			}
		}
		return false;
	}

	bool ContainsOpen(std::vector<NodeRecord*>& list, Graph::Node& node, NodeRecord** nodeRecord, int* nodeIndex)
	{
		START_PROFILE();
		for (int i = 0; i < list.size(); i++)
		{
			if (list[i]->Node.ID == node.ID)
			{
				*nodeRecord = list[i];
				*nodeIndex = i;
				return true;
			}
		}
		return false;
	}

	std::vector<Graph::Connection>* AStar(Graph& graph, Graph::Node start, Graph::Node end)
	{
		START_PROFILE();
		NodeRecord* startRecord = new NodeRecord(start, Graph::Connection(NULL_CONNECTION(start)), nullptr, 0, Estimate(start, end));

		std::vector<NodeRecord*> open;
		Add(open, startRecord);
		std::vector<NodeRecord*> closed;

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
				float endNodeHeuristic;

				NodeRecord* endNodeRecord = nullptr;
				NodeRecord** ptr_endNodeRecord = &endNodeRecord;
				int endNodeIndex = -1;

				if (ContainsClosed(closed, endNode, ptr_endNodeRecord, &endNodeIndex))
				{
					if (endNodeRecord->CostSoFar <= endNodeCost)
						continue;

					Erase(closed, endNodeIndex);
					
					endNodeHeuristic = endNodeRecord->EstimatedTotalCost - endNodeRecord->CostSoFar;
				}
				else if (ContainsOpen(open, endNode, ptr_endNodeRecord, &endNodeIndex))
				{
					if (endNodeRecord->CostSoFar <= endNodeCost)
						continue;

					endNodeHeuristic = endNodeRecord->EstimatedTotalCost - endNodeRecord->CostSoFar;
				}
				else
				{
					endNodeRecord = new NodeRecord;
					endNodeRecord->Node = endNode;
					endNodeHeuristic = Estimate(endNode, end);
				}

				endNodeRecord->Connection = connection;
				endNodeRecord->ParentNode = current;
				endNodeRecord->CostSoFar = endNodeCost;
				endNodeRecord->EstimatedTotalCost = endNodeCost + endNodeHeuristic;

				if (!Contains(open, endNode))
					Add(open, endNodeRecord);
			}

			Erase(open, current);
			Add(closed, current);
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

		DEBUG("A_STAR (Fill %): ");
		DEBUG((open.size() + closed.size()) / (float)graph.GetNodes()->size());


		return path;
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