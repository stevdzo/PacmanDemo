#ifndef _ASTAR_H
#define _ASTAR_H

#include "Graph.h"

class AStar {

private:

	GraphNode* findNodeWithLowestCost(std::set<GraphNode*>& p_nodes);
	GraphNode* findNodeWithHighestCost(std::set<GraphNode*>& p_nodes);

public:

	AStar();
	AStar(Graph* p_graph, GraphNode* p_startNode, GraphNode* p_targetNode);

	void update(float p_deltaTime);
	void render();

	/*void setStartNode(GraphNode* p_startNode);
	void setTargetNode(GraphNode* p_targetNode);*/

	std::vector<GraphNode*> findShortestPath(GraphNode* p_startNode, GraphNode* p_targetNode);
};

inline GLint heuristicDistance(GraphNode* p_node1, GraphNode* p_node2) {

	Vector2D distance = p_node1->getPosition() - p_node2->getPosition();

	int absDistX = std::abs(distance.x);
	int absDistY = std::abs(distance.y);

	return absDistX + absDistY;
}
#endif