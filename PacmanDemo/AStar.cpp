#include "AStar.h"

AStar::AStar() {
	std::cout << "Initializing AStar." << std::endl;
}

// Implementacija A* algoritma pretraživanja grafova
// GraphNode* p_startNode    - trenutni čvor duha
// GraphNode* p_targetNode   - ciljni čvor duha
// GraphNode* p_previousNode - prethodni čvor duha
// bool p_isInsideBase       - flag da li je duh u bazi
Vector<GraphNode*> AStar::findShortestPath(GraphNode* p_startNode, GraphNode* p_targetNode, GraphNode* p_previousNode, bool p_isInsideBase) {

	if (p_startNode->isObstacle() || !p_startNode->isValidNode())
		return Vector<GraphNode*>();

	Set<GraphNode*> openNodes;
	Set<GraphNode*> closedNodes;

	openNodes.insert(p_startNode);

	while (!openNodes.empty()) {
		GraphNode* currentNode = findNodeWithLowestCost(openNodes);

		openNodes.erase(currentNode);
		closedNodes.insert(currentNode);

		if (currentNode == p_targetNode) {
			Vector<GraphNode*> finalPath;
			while (currentNode != nullptr && currentNode != p_startNode) {
				finalPath.push_back(currentNode);
				currentNode = currentNode->getParent();
			}
			std::reverse(finalPath.begin(), finalPath.end());
			return finalPath;
		}

		for (auto* adjNode : currentNode->getConnectedNodes()) {			
			
			if (closedNodes.count(adjNode) || adjNode->isObstacle() || !adjNode->isValidNode() || adjNode->getIndex() == specialBlockNodeIndex)
				continue;	

			// Dodatna provera koje ne dozvoljava duhovima da krenu u suprotnom smeru.
			if (!p_isInsideBase) {
				if ((adjNode == p_previousNode ||
					adjNode == p_startNode) &&
					p_targetNode != p_previousNode &&
					adjNode->getIndex() != baseEntranceNodeIndex) {
					continue;
				}
			}

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
GraphNode* AStar::findNodeWithLowestCost(const Set<GraphNode*>& p_nodes) {

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
GraphNode* AStar::findNodeWithHighestCost(const Set<GraphNode*>& p_nodes) {
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