#include "Graph.h"

Graph::Node::Point::Point(float x, float y)
	: X(x), Y(y) {}

Graph::Node::Point::Point(int x, int y)
	: X((float)x), Y((float)y) {}

Graph::Node::Node(int id, Point position)
	: ID(id), Position(position) {}

Graph::Connection::Connection(int from, int to, float cost)
	: From(from), To(to), Cost(cost) {}

Graph::Graph() {}

Graph::Graph(int numNodes, int numConnections)
{
	m_Nodes.reserve(numNodes);
	m_ConnectionsPerNodeID.reserve(numNodes);
}

std::vector<Graph::Node>* Graph::GetNodes()
{
	return &m_Nodes;
}

std::vector<Graph::Connection>* Graph::GetConnections(Node node)
{
	return &m_ConnectionsPerNodeID.at(node.ID);
}

void Graph::AddNode(Node node)
{
	m_Nodes.push_back(node);
	std::vector<Connection> vector;
	m_ConnectionsPerNodeID.push_back(vector);
	m_ConnectionsPerNodeID[node.ID].reserve(8);
	
}

void Graph::AddConnection(Connection connection)
{
	Node node = m_Nodes[connection.From];
	if (node.ID < 0 || node.ID > m_ConnectionsPerNodeID.size() - 1)
	{
		std::cout << "From node does not exist in this graph!" << std::endl;
		return;
	}
	m_ConnectionsPerNodeID[node.ID].push_back(connection);
}

void Graph::Print()
{
	std::cout << "Graph: " << std::endl << std::endl;
	for (int i = 0; i < m_ConnectionsPerNodeID.size(); i++)
	{
		std::cout << "Node (" << i << ") = " << std::endl;

		for (int j = 0; j < m_ConnectionsPerNodeID[i].size(); j++)
		{
			Connection c = m_ConnectionsPerNodeID[i][j];
			std::cout << "\t{" << m_Nodes[c.To].ID << ", " << c.Cost << "}" << std::endl;
		}
	}
}