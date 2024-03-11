#ifndef _GRAPH_H
#define _GRAPH_H

#include "GraphNode.h"

// Singleton class
class Graph {

private:

	static Graph* instance;

	Matrix<GraphNode*> m_nodeMatrix;
	std::vector<GraphNode*> m_nodeVector;

	void initNodes();
	void initEdges();
	void initGraph();

	Graph();

public:

	static Graph* getInstance();

	~Graph();

	Matrix<GraphNode*> getNodes();
	std::vector<GraphNode*> getNodeVector();
	
	int calculateDistanceInNodes(const GraphNode* p_node1, const GraphNode* p_node2) const;
	int getNodeIndexByDirection(GraphNode* p_currentNode, Direction p_direction) const;
	Direction getDirectionByNode(GraphNode* p_currentNode, GraphNode* p_targetNode) const;
	GraphNode* getNodeInPlayerDirection(const GraphNode* playerNode, const Direction playerDirection, const int p_tileDistance) const;
	GraphNode* getNodeByPosition(Vector2D p_position);
	GraphNode* calculateInkyTargetNode(const GraphNode* p_node1, const GraphNode* p_node2, const Direction p_direction);

	void render();
	void renderWireframe();
};
#endif