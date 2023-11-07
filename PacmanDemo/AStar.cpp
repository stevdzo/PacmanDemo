#include "AStar.h"

AStar::AStar() {

}


void AStar::update(float p_deltaTime) {
}

void AStar::render() {
	//for (auto& node : m_path) {
	//	node->renderNodeFromPath();
	//}
} 

std::vector<GraphNode*> AStar::findShortestPath(GraphNode* p_startNode, GraphNode* p_targetNode) {
	
	std::set<GraphNode*> openNodes;
	std::set<GraphNode*> closedNodes;

	openNodes.insert(p_startNode);

	while (!openNodes.empty()) {		
		GraphNode* currentNode = findNodeWithLowestCost(openNodes);

		openNodes.erase(currentNode);
		closedNodes.insert(currentNode);

		if (currentNode == p_targetNode) {
			
			std::vector<GraphNode*> finalPath;
			while (currentNode != nullptr && currentNode != p_startNode) {
				finalPath.push_back(currentNode);
				currentNode = currentNode->getParent();
			}
			std::reverse(finalPath.begin(), finalPath.end());			
			return finalPath;
		}

		for (auto* adjNode : currentNode->getConnectedNodes()) {		
			if (closedNodes.count(adjNode) || adjNode->isObstacle() || adjNode->isEmptyNode()) {	
				continue;
			}		
			if (openNodes.count(adjNode) && adjNode->getGCost() > findNodeWithHighestCost(openNodes)->getGCost()) {
				continue;
			}
			else {		
				adjNode->setParent(currentNode);

				adjNode->setGCost(currentNode->getGCost() + 1);
				adjNode->setHCost(heuristicDistance(adjNode, p_targetNode));
				adjNode->setFCost(adjNode->getGCost() + adjNode->getHCost());
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