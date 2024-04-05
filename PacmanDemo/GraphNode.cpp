#include "GraphNode.h"
#include "GraphEdge.h"
#include "TextRenderer.h"

GraphNode::GraphNode(int p_index) : m_index(p_index), GameObject() {
	m_gCost = 0;
	m_hCost = 0;
	m_fCost = 0;

	m_nodeType = NodeType::invalid;

	m_isIntersection = false;
	m_isCorner = false;
	m_isTunnelNode = false;
}

GraphNode::GraphNode(int p_index, Vector2D p_position) : m_index(p_index), GameObject(p_position) {
	m_gCost = 0;
	m_hCost = 0;
	m_fCost = 0;

	m_nodeType = NodeType::invalid;

	m_isIntersection = false;
	m_isCorner = false;
	m_isTurn = false;
	m_isTunnelNode = false;
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

void GraphNode::setGCost(float p_gCost) {
	m_gCost = p_gCost;
}

float GraphNode::getGCost(void) const {
	return m_gCost;
}

void GraphNode::setFCost(float p_fCost) {
	m_fCost = p_fCost;
}

float GraphNode::getFCost(void) const {
	return m_fCost;
}

void GraphNode::setHCost(float p_hCost) {
	m_hCost = p_hCost;
}

float GraphNode::getHCost(void) const {
	return m_hCost;
}

void GraphNode::setNodeType(NodeType p_nodeType) {
	m_nodeType = p_nodeType;
}

NodeType GraphNode::getNodeType(void) const {
	return m_nodeType;
}

bool GraphNode::isValidNode(void) const {
	return m_nodeType == NodeType::valid;
}

bool GraphNode::isObstacle(void) const {
	return m_nodeType == NodeType::obstacle;
}

bool GraphNode::isSpecialNode(void) const {
	return m_nodeType == NodeType::special;
}

void GraphNode::isIntersection(bool p_isIntersection) {
	m_isIntersection = p_isIntersection;
}

bool GraphNode::isIntersection(void) const {
	return m_edges.size() > 2;
}

void GraphNode::isCorner(bool p_isCorner) {
	m_isCorner = p_isCorner;
}

// ukoliko je srednja vrednost zbira indeksa dva susedna čvora 
// jednaka trenutnom čvoru, 
// taj čvor nije skretanje
bool GraphNode::isCorner(void) const {
	int indexSum = 0;
	if (m_edges.size() == 2) {
		for (auto& node : m_connectedNodes)
			if (node->isObstacle())
				indexSum += node->getIndex();			
		if (indexSum / 2 != m_index) 
			return true;
	}
	return false;
}

void GraphNode::isTurn(bool p_isTurn) {
	m_isTurn = p_isTurn;
}

bool GraphNode::isTurn(void) const {
	return isCorner() || isIntersection();
}

void GraphNode::isTunnelNode(bool p_isTunnelNode) {
	m_isTunnelNode = p_isTunnelNode;
}

bool GraphNode::isTunnelNode(void) const {
	return m_isTunnelNode;
}

void GraphNode::isBaseNode(bool p_isBaseNode) {
	m_isBaseNode = p_isBaseNode;
}

bool GraphNode::isBaseNode(void) const {
	return m_isBaseNode;
}

void GraphNode::setParent(GraphNode* p_node) {
	m_parentNode = p_node;
}

GraphNode* GraphNode::getParent(void) const {
	return m_parentNode;
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

void GraphNode::renderWireframe() {
	if (toggleWireframe)
		if (isObstacle()) {
			drawRectangle(m_position.x, m_position.y, m_size.x, m_size.y, 0.0f, 0.0f, 1.0f, GL_LINE_LOOP);			
		}
	/*char cost[50];
	sprintf_s(cost, 50, "%i", static_cast<int>(m_gCost));
	if (cost != "0") {
		TextRenderer::getInstance()->drawStrokeText((char*)cost, m_position.x - 0.14 * 0.75, m_position.y - 0.12 * 0.75, 1, 1, 1, 0.75);
	}*/
}

void GraphNode::renderNodeFromPath() {
	//if (toggleWireframe)
		drawRectangle(m_position.x, m_position.y, m_size.x/2, m_size.y/2, 0.0f, 1.0f, 0.0f, GL_POLYGON);
}