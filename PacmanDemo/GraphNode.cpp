#include "GraphNode.h"
#include "GraphEdge.h"

GraphNode::GraphNode(int p_index) : m_index(p_index), GameObject() {
	m_gCost = 0;
	m_fCost = 0;
	m_isEmptyNode = false;
	m_isObstacle = false;
	m_isIntersection = false;
	m_isCorner = false;
	m_wireframeColor = gv::nodeWireframeColor;
}

GraphNode::GraphNode(int p_index, Vector2D p_position) : m_index(p_index), GameObject(p_position) {
	m_gCost = 0;
	m_fCost = 0;
	m_isEmptyNode = false;
	m_isObstacle = false;
	m_isIntersection = false;
	m_isCorner = false;
	m_wireframeColor = gv::nodeWireframeColor;
}

void GraphNode::setIndex(int p_index) {
	m_index = p_index;
}

int GraphNode::getIndex(void) const {
	return m_index;
}

Index2D GraphNode::getIndexAs2D(void) const {
	return Index2D(m_index / gv::columns,
				   m_index % gv::columns);
}

void GraphNode::setGCost(int p_gCost) {
	m_gCost = p_gCost;
}

int GraphNode::getGCost(void) const {
	return m_gCost;
}

void GraphNode::setFCost(int p_fCost) {
	m_fCost = p_fCost;
}

int GraphNode::getFCost(void) const {
	return m_fCost;
}

void GraphNode::isEmptyNode(bool p_emptyNode) {
	m_isEmptyNode = p_emptyNode;
}

bool GraphNode::isEmptyNode(void) const {
	return m_isEmptyNode;
}

void GraphNode::isObstacle(bool p_isObstacle) {
	m_isObstacle = p_isObstacle;
}

bool GraphNode::isObstacle(void) const {
	return m_isObstacle;
}

void GraphNode::isIntersection(bool p_isIntersection) {
	m_isIntersection = p_isIntersection;
}

//bool GraphNode::isIntersection(void) const {
//	//return m_isIntersection;
//	return m_edges.size() > 2 ? true : false;
//}

void GraphNode::isCorner(bool p_isCorner) {
	m_isCorner = p_isCorner;
}

bool GraphNode::isIntersection(void) const {
	//return m_isCorner;
	if (m_edges.size() == 2) {
		int indexCount = 0;
		for (auto& node : m_connectedNodes) {
			if (!node->isEmptyNode() && !node->isObstacle()) {
				indexCount += node->getIndex();
			}
		}
		if (indexCount / 2 == m_index) {
			return false;
		}
		return true;
	}
}

void GraphNode::addEdge(GraphEdge* p_edge) {
	m_edges.push_back(p_edge);
}

const std::vector<GraphEdge*>& GraphNode::getEdges() const {
	return m_edges;
}

void GraphNode::addConnectedNode(GraphNode* p_node) {
	m_connectedNodes.push_back(p_node);
}

const std::vector<GraphNode*>& GraphNode::getConnectedNodes() const {
	return m_connectedNodes;
}

void GraphNode::update(GLfloat p_deltaTime) {

}

void GraphNode::render() {

}

void GraphNode::renderWireframe(){

	/*if (m_isObstacle) m_wireframeColor = Vector3D(0.8f, 0.2f, 0.2f);
	else		      m_wireframeColor = Vector3D(0.2f, 0.8f, 0.2f);*/

	if (!m_isEmptyNode && m_isObstacle)
	GameObject::renderWireframe();
	/*else {
		glPointSize(5.0f);
		glBegin(GL_POINTS);
		glColor3f(0.5, 0.5, 0.0);
		glVertex2f(m_position.x, m_position.y);
		glEnd();
	}*/
}