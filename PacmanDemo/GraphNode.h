#ifndef _GRAPHNODE_H
#define _GRAPHNODE_H

#include "GameObject.h"

struct Index2D {

	int row;
	int col;

	Index2D() {
		row = 0;
		col = 0;
	}

	Index2D(int p_row, int p_col) {
		row = p_row;
		col = p_col;
	}
};

class GraphNode : public GameObject {

private: 

	int m_index;
	int m_gCost;
	int m_fCost;

	bool m_isObstacle;

	std::vector<GraphNode*> m_connectedNodes;

public:

	GraphNode(int p_index);
	GraphNode(int p_index, Vector2D p_position);

	void setIndex(int);
	int getIndex(void) const;

	Index2D getIndexAs2D(void) const;

	void setGCost(int);
	int getGCost(void) const;

	void setFCost(int);
	int getFCost(void) const;

	void isObstacle(bool);
	bool isObstacle(void) const;

	/*void addEdge(GraphEdge* p_edge);
	const std::vector<GraphEdge*>& getEdges() const;*/

	void addConnectedNode(GraphNode*);
	const std::vector<GraphNode*>& getConnectedNodes() const;

	void update(GLfloat p_deltaTime) override;
	void render() override;
	void renderWireframe() override;

};
#endif