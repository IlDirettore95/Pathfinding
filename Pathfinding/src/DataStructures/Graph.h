#pragma once

#include <vector>
#include <iostream>

#define NULL_POSITION Graph::Node::Point(INFINITY, INFINITY)
#define NULL_NODE -1, NULL_POSITION
#define NULL_CONNECTION -1, -1, INFINITY
class Graph
{
public:
	struct Node
	{
		struct Point
		{
			float X;
			float Y;

			Point(float x, float y);
			Point(int x, int y);
		};

		int ID;
		Point Position;
		Node(int id, Point position);
	};

	struct Connection
	{
		int From;
		int To;
		float Cost;

		Connection(int from, int to, float cost);
	};

public:
	Graph();
	Graph(int numNodes, int numConnections);
	std::vector<Node>* GetNodes();
	std::vector<Connection>* GetConnections(Node node);
	void AddNode(Node node);
	void AddConnection(Connection connection);
	void Print();
private:
	std::vector<Node> m_Nodes;
	std::vector<std::vector<Connection>> m_ConnectionsPerNodeID;
};
