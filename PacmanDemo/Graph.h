#ifndef _GRAPH_H
#define _GRAPH_H

#include "GraphNode.h"

class Graph {

private:

	Matrix<GraphNode*> m_nodeMatrix;

	//void addEdge(GraphEdge* p_edge);
	void initNodes();
	void initEdges();
	void initGraph();

public:

	Graph();
	~Graph();

	Matrix<GraphNode*> getNodes();

	void render();
	void renderWireframe();

	template<typename T>
	std::vector<T*> getMatrixAsVector(const std::vector<std::vector<T*>>& p_matrix);
};

template<typename T>
inline std::vector<T*> Graph::getMatrixAsVector(const std::vector<std::vector<T*>>& p_matrix) {
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