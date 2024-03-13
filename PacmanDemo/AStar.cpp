#include "AStar.h"

AStar::AStar() {
	std::cout << "Initializing AStar." << std::endl;
}

void AStar::update(float p_deltaTime) {
}

void AStar::render() {
	for (auto& node : m_path) {
		node->renderNodeFromPath();

		for (auto& adjNode : node->getConnectedNodes())
		{
			drawPoint(adjNode->getPosition().x, adjNode->getPosition().y, 12, 1.0f, 0.0f, 0.0f);
		}
		//std::cout << node->getIndex() << std::endl;
	}
}

// GraphNode* p_startNode - trenutni čvor neprijatelja
// GraphNode* p_targetNode - ciljni čvor neprijatelja
std::vector<GraphNode*> AStar::findShortestPath(GraphNode* p_startNode, GraphNode* p_targetNode, GraphNode* p_previousNode) {

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
				//m_path.push_back(currentNode);
				currentNode = currentNode->getParent();
			}
			std::reverse(finalPath.begin(), finalPath.end());
			return finalPath;
		}

		for (auto* adjNode : currentNode->getConnectedNodes()) {

			/*if (p_startNode != p_previousNode && adjNode == p_previousNode) {

				
				continue;
			}*/

			if (closedNodes.count(adjNode) || adjNode->isObstacle() || adjNode->isEmptyNode())
				continue;	

			adjNode->setParent(currentNode);

			adjNode->setGCost(currentNode->getGCost() + 1.0f);
			adjNode->setHCost(heuristicDistance(adjNode, p_targetNode));
			adjNode->setFCost(adjNode->getGCost() + adjNode->getHCost());

			if (openNodes.count(adjNode) && adjNode->getGCost() > findNodeWithHighestCost(openNodes)->getGCost())
				continue;
			
			openNodes.insert(adjNode);							
		}		
	}
}

// std::set<GraphNode*>& p_nodes - skup čvorova na osnovu koga se traži čvor sa najmanjom cenom
GraphNode* AStar::findNodeWithLowestCost(std::set<GraphNode*>& p_nodes) {

	GraphNode* lowestCostNode = nullptr;
	int lowestCost = std::numeric_limits<int>::max();

	for (auto* node : p_nodes) {
		if (node->getFCost() < lowestCost) {
			lowestCost = node->getFCost();
			lowestCostNode = node;			
		}
	}
	return lowestCostNode;
}

// std::set<GraphNode*>& p_nodes - skup čvorova na osnovu koga se traži čvor sa najvišom cenom
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