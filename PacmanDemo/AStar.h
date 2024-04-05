#ifndef _ASTAR_H
#define _ASTAR_H

#include "Graph.h"

class AStar {

private:

	GraphNode* findNodeWithLowestCost (const Set<GraphNode*>& p_nodes);
	GraphNode* findNodeWithHighestCost(const Set<GraphNode*>& p_nodes);

public:

	AStar();

	Vector<GraphNode*> findShortestPath(GraphNode* p_startNode, 
										GraphNode* p_targetNode, 
										GraphNode* p_previousNode, 
										bool       p_canGoBack);
};

inline float heuristicDistance(GraphNode* p_node1, GraphNode* p_node2) {

	Vector2D distance = p_node1->getPosition() - p_node2->getPosition();

	float absDistX = std::abs(distance.x / heuristicCoeff);
	float absDistY = std::abs(distance.y / heuristicCoeff);

	float result = absDistX + absDistY;

	return result;
}
#endif