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
	
	int getNodeIndexByDirection(GraphNode* p_currentNode, Direction p_direction) const;
	Direction getDirectionByNode(GraphNode* p_currentNode, GraphNode* p_targetNode) const;

	GraphNode* getNodeInPlayerDirection(GraphNode* playerNode, Direction playerDirection) const;

	GraphNode* getNodeByPosition(Vector2D p_position);

	void render();
	void renderWireframe();

	template<typename T>
	std::vector<T*> getMatrixAsVector(const Matrix<T*> p_matrix);
};

template<typename T>
inline std::vector<T*> Graph::getMatrixAsVector(const Matrix<T*> p_matrix) {
	std::vector<T*> result;
	int index = 0;
	for (const auto& row : p_matrix) {
		for (const auto& element : row) {
			if (element) {
				result.push_back(element);
			}
			index++;
		}
	}
	return result;
}
#endif