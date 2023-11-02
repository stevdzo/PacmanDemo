#ifndef _GRAPHEDGE_H
#define _GRAPHEDGE_H

#include "GraphNode.h"

class GraphEdge : public GameObject {

private:

	GraphNode* m_from;
	GraphNode* m_to;

public:

	GraphEdge(GraphNode* p_from, GraphNode* p_to);

	GraphNode* getFrom();
	GraphNode* getTo();

	int getFromIndex();
	int getToIndex();

	bool connectsTo(int p_Index) const;
	GraphNode* getConnectedNode(GraphNode* p_node) const;

	void update(float p_deltatime) override;
	void render() override;
	void renderWireframe() override;
};
#endif