#include "AStar.h"

AStar::AStar() {

}

AStar::AStar(Graph* p_graph,
			 GraphNode* p_startNode,
			 GraphNode* p_targetNode) :
			 m_graph(p_graph),
			 m_startNode(p_startNode),
			 m_targetNode(p_targetNode)
{

}

void AStar::update(float p_deltaTime) {
}

void AStar::render() {
	for (auto& node : m_path) {
		node->renderNodeFromPath();
	}
}

void AStar::setStartNode(GraphNode* p_startNode) {
	this->m_startNode = p_startNode;
}

void AStar::setTargetNode(GraphNode* p_targetNode) {
	this->m_targetNode = p_targetNode;
}

std::vector<GraphNode*> AStar::findShortestPath(GraphNode* p_startNode, GraphNode* p_targetNode) {

	if (!m_path.empty()) m_path.clear();

	std::set<GraphNode*> openNodes;
	std::set<GraphNode*> closedNodes;

	openNodes.insert(p_startNode);

	while (!openNodes.empty()) {
		GraphNode* currentNode = findNodeWithLowestCost(openNodes);

		openNodes.erase(currentNode);
		closedNodes.insert(currentNode);

		if (currentNode == p_targetNode) {

			while (currentNode != nullptr) {
				m_path.push_back(currentNode);
				currentNode = currentNode->getParent();
			}
			std::reverse(m_path.begin(), m_path.end());
			return m_path;
		}

		for (auto* adjNode : currentNode->getConnectedNodes()) {		
			if (closedNodes.count(adjNode) || adjNode->isObstacle()) {	
				continue;
			}

			adjNode->setParent(currentNode);

			adjNode->setGCost(currentNode->getGCost() + 1);
			adjNode->setHCost(heuristicDistance(adjNode, p_targetNode));
			adjNode->setFCost(adjNode->getGCost() + adjNode->getHCost());

			if (openNodes.count(adjNode) && adjNode->getGCost() > findNodeWithHighestCost(openNodes)->getGCost()) {
				continue;
			}
			openNodes.insert(adjNode);							
		}		
	}
}

GraphNode* AStar::findNodeWithLowestCost(std::set<GraphNode*>& p_nodes) {

	GraphNode* lowestCostNode = nullptr;
	int lowestFCost = std::numeric_limits<int>::max();

	for (auto* node : p_nodes) {
		if (node->getFCost() < lowestFCost) {
			lowestFCost = node->getFCost();
			lowestCostNode = node;
			
		}
	}

	return lowestCostNode;
}

GraphNode* AStar::findNodeWithHighestCost(std::set<GraphNode*>& p_nodes) {
	GraphNode* highestCostNode = nullptr;
	int highestCost = std::numeric_limits<int>::min();

	for (auto* node : p_nodes) {
		if (node->getGCost() > highestCost) {
			highestCost = node->getGCost();
			highestCostNode = node;

		}
	}
	return highestCostNode;
}