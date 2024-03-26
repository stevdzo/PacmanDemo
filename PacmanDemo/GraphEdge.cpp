#include "GraphEdge.h"

GraphEdge::GraphEdge(GraphNode* p_from, GraphNode* p_to) : m_from(p_from), m_to(p_to) {
}

GraphNode* GraphEdge::getFrom() {
    return m_from;
}

GraphNode* GraphEdge::getTo() {
    return m_to;
}

int GraphEdge::getFromIndex() {
    return m_from->getIndex();
}

int GraphEdge::getToIndex() {
    return m_to->getIndex();
}

bool GraphEdge::connectsTo(int p_Index) const {
    return (m_from->getIndex() == p_Index || m_to->getIndex() == p_Index);
}

GraphNode* GraphEdge::getConnectedNode(GraphNode* p_node) const {
	if (p_node->getIndex() == m_from->getIndex()) {
		return m_to;
	}
	if (p_node->getIndex() == m_to->getIndex()) {
		return m_from;
	}
	return nullptr;
}

void GraphEdge::update(float p_deltatime) {

}

void GraphEdge::render() {

}

void GraphEdge::renderWireframe() {
	if (toggleWireframe) {
		if (m_from->isValidNode() && m_to->isValidNode()) {
			drawLine(m_from->getPosition().x, m_from->getPosition().y, m_to->getPosition().x, m_to->getPosition().y, 0.0f, 1.0f, 1.0f);
		}
	}
}