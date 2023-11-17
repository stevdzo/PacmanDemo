#include "GraphNode.h"
#include "GraphEdge.h"

GraphNode::GraphNode(int p_index) : m_index(p_index), GameObject() {
	m_gCost = 0;
	m_hCost = 0;
	m_fCost = 0;

	m_nodeType = NodeType::none;

	m_isEmptyNode = false;
	m_isObstacle = false;
	m_isIntersection = false;
	m_isCorner = false;
	m_wireframeColor = nodeWireframeColor;
}

GraphNode::GraphNode(int p_index, Vector2D p_position) : m_index(p_index), GameObject(p_position) {
	m_gCost = 0;
	m_hCost = 0;
	m_fCost = 0;

	m_nodeType = NodeType::none;

	m_isEmptyNode = false;
	m_isObstacle = false;
	m_isIntersection = false;
	m_isCorner = false;
	m_wireframeColor = nodeWireframeColor;
}

void GraphNode::setIndex(int p_index) {
	m_index = p_index;
}

int GraphNode::getIndex(void) const {
	return m_index;
}

Index2D GraphNode::getIndexAs2D(void) const {
	return Index2D(m_index / columns,
				   m_index % columns);
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

void GraphNode::setHCost(int p_hCost) {
	m_hCost = p_hCost;
}

int GraphNode::getHCost(void) const {
	return m_hCost;
}

void GraphNode::setNodeType(NodeType p_nodeType) {
	m_nodeType = p_nodeType;
}

NodeType GraphNode::getNodeType(void) const {
	return m_nodeType;
}

void GraphNode::isEmptyNode(bool p_emptyNode) {
	m_isEmptyNode = p_emptyNode;
}

bool GraphNode::isEmptyNode(void) const {
	return m_nodeType == NodeType::none;
	//return m_isEmptyNode;
}

void GraphNode::isObstacle(bool p_isObstacle) {
	m_isObstacle = p_isObstacle;
}

bool GraphNode::isObstacle(void) const {
	return m_nodeType == NodeType::obstacle;
	//return m_isObstacle;
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

void GraphNode::setParent(GraphNode* p_node) {
	m_parentNode = p_node;
}

GraphNode* GraphNode::getParent(void) const {
	return m_parentNode;
}

bool GraphNode::isIntersection(void) const {

	int indexSum = 0;

	if (m_edges.size() > 2) {
		return true;
	}

	if (m_edges.size() == 2) {

		for (auto& node : m_connectedNodes) {
			if (node->isObstacle()) {
				indexSum += node->getIndex();
			}
		}

		if (indexSum / 2 != m_index) {
			return true;
		}
	}
	return false;
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

	if (!isEmptyNode() && isObstacle())
	GameObject::renderWireframe();
	/*else {
		glPointSize(5.0f);
		glBegin(GL_POINTS);
		glColor3f(0.5, 0.5, 0.0);
		glVertex2f(m_position.x, m_position.y);
		glEnd();
	}*/
}

void GraphNode::renderNodeFromPath() {
	if (toggleWireframe) {
		glBegin(GL_POLYGON);
		glColor3fv(m_wireframeColor.toArray());
		glVertex2f(m_position.x - m_size.x / 2, m_position.y - m_size.y / 2);
		glVertex2f(m_position.x + m_size.x / 2, m_position.y - m_size.y / 2);
		glVertex2f(m_position.x + m_size.x / 2, m_position.y + m_size.y / 2);
		glVertex2f(m_position.x - m_size.x / 2, m_position.y + m_size.y / 2);
		glEnd();
	}
}