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

enum class NodeType {

	invalid = 0,
	obstacle = 1,
	valid = 2,
	special = 3
};

class GraphEdge;

class GraphNode : public GameObject {

private: 

	int m_index;
	float m_gCost;
	float m_fCost;
	float m_hCost;

	NodeType m_nodeType;

	bool m_isEmptyNode;
	bool m_isObstacle;
	bool m_isIntersection;
	bool m_isCorner;
	bool m_isTurn;

	GraphNode* m_parentNode;

	std::vector<GraphEdge*> m_edges;
	std::vector<GraphNode*> m_connectedNodes;

public:

	GraphNode(int p_index);
	GraphNode(int p_index, Vector2D p_position);

	void setIndex(int);
	int getIndex(void) const;

	Index2D getIndexAs2D(void) const;

	void setGCost(float);
	float getGCost(void) const;

	void setFCost(float);
	float getFCost(void) const;

	void setHCost(float);
	float getHCost(void) const;

	void setNodeType(NodeType p_nodeType);
	NodeType getNodeType(void) const;

	void isEmptyNode(bool);
	bool isValidNode(void) const;

	void isObstacle(bool);
	bool isObstacle(void) const;

	bool isSpecialNode(void) const;

	void isIntersection(bool);
	bool isIntersection(void) const;

	void isCorner(bool);
	bool isCorner(void) const;

	void isTurn(bool);
	bool isTurn(void) const;

	void setParent(GraphNode* p_node);
	GraphNode* getParent(void) const;

	void addEdge(GraphEdge* p_edge);
	const std::vector<GraphEdge*>& getEdges() const;

	void addConnectedNode(GraphNode*);
	const std::vector<GraphNode*>& getConnectedNodes() const;

	void update(GLfloat p_deltaTime) override;
	void render() override;
	void renderWireframe() override;
	void renderNodeFromPath();
};
#endif