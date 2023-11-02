#include "Graph.h"

void Graph::initNodes() {
    std::cout << "Initializing nodes." << std::endl;
    m_nodeMatrix.resize(gv::rows, std::vector<GraphNode*>(gv::columns, nullptr));
    int index = 0;
    for (unsigned int row = 0; row < gv::rows; row++) {
        for (unsigned int col = 0; col < gv::columns; col++) {
            GraphNode* node = new GraphNode(index, Vector2D(row * gv::nodeSize + gv::nodeRenderOffset, col * gv::nodeSize + gv::nodeRenderOffset));
            if (gv::map[row][col] != 1) {
                node->isObstacle(true);
            }
            m_nodeMatrix[row][col] = node;
            index++;
        }
    }
}

void Graph::initEdges() {
    std::cout << "Initializing edges." << std::endl;
}

void Graph::initGraph() {
    std::cout << "Initializing graph." << std::endl;
    initNodes();
    initEdges();
}

Graph::Graph() {
    initGraph();
}

Graph::~Graph() {
    std::cout << "Deleting graph." << std::endl;
}

Matrix<GraphNode*> Graph::getNodes() {
    return m_nodeMatrix;
}

void Graph::render() {

}

void Graph::renderWireframe() {
    for (auto itRow = m_nodeMatrix.begin(); itRow != m_nodeMatrix.end(); ++itRow) {
        for (auto itCol = itRow->begin(); itCol != itRow->end(); ++itCol) {
            GraphNode* node = *itCol;
            node->renderWireframe();
        }
    }
}