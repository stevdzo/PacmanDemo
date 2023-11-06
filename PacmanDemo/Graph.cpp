#include "Graph.h"
#include "GraphEdge.h"

Graph* Graph::instance = nullptr;

void Graph::initNodes() {
    std::cout << "Initializing nodes." << std::endl;
    m_nodeMatrix.resize(gv::rows, std::vector<GraphNode*>(gv::columns, nullptr));
    int index = 0;
    for (unsigned int row = 0; row < gv::rows; row++) {
        for (unsigned int col = 0; col < gv::columns; col++) {
            GraphNode* node = new GraphNode(index, Vector2D(row * gv::nodeSize + gv::nodeRenderOffset, col * gv::nodeSize + gv::nodeRenderOffset) - Vector2D(32.0f, 0.0f));

            switch (gv::map[row][col]) {

            case -1: {
                node->isObstacle(true);
                node->setNodeType(NodeType::obstacle);
            }
                break;
            case 0: {
                node->isEmptyNode(true);
                node->setNodeType(NodeType::none);
            }
                break;
            case 1: {
                node->isObstacle(false);
                node->setNodeType(NodeType::valid);
            }
                break;
            case 2: {
                node->isObstacle(false);
                node->setPosition(node->getPosition() + Vector2D(16.0f, 0.0f));
                node->setNodeType(NodeType::valid);
            }
                  break;
            }
            m_nodeMatrix[row][col] = node;
            m_nodeVector.push_back(node);
            index++;
        }
    }
}

void Graph::initEdges() {
    std::cout << "Initializing edges." << std::endl;
    int index = 0;
    for (unsigned int row = 0; row < gv::rows; row++) {
        for (unsigned int col = 0; col < gv::columns; col++) {
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    if (i == 0 && j == 0) continue;
                    if (abs(i) == abs(j)) continue;
                    if (row + i >= 0 &&
                        row + i < gv::rows &&
                        col + j >= 0 &&
                        col + j < gv::columns) {
                        if (!m_nodeMatrix[row][col]->isEmptyNode() &&
                            !m_nodeMatrix[row + i][col + j]->isEmptyNode() &&
                            !m_nodeMatrix[row][col]->isObstacle() &&
                            !m_nodeMatrix[row + i][col + j]->isObstacle()) {
                            GraphEdge* edge = new GraphEdge(m_nodeMatrix[row][col], m_nodeMatrix[row + i][col + j]);
                            m_nodeMatrix[row][col]->addEdge(edge);
                        }
                        m_nodeMatrix[row][col]->addConnectedNode(m_nodeMatrix[row + i][col + j]);
                    }
                }
            }
        }
    }
}

void Graph::initGraph() {
    std::cout << "Initializing graph." << std::endl;
    initNodes();
    initEdges();
}

Graph::Graph() {
    initGraph();
}

Graph* Graph::getInstance() {
    if (instance == nullptr) {
        instance = new Graph();
    }
    return instance;
}

Graph::~Graph() {
    std::cout << "Deleting graph." << std::endl;
}

Matrix<GraphNode*> Graph::getNodes() {
    return m_nodeMatrix;
}

std::vector<GraphNode*> Graph::getNodeVector() {
    return m_nodeVector;
}

int Graph::getAdjacentNodeIndex(GraphNode* p_currentNode, Direction p_direction) const {

    GLint numRows = m_nodeMatrix.size();
    if (numRows == 0) {
        return -1;
    }
    GLint numCols = m_nodeMatrix[0].size();
    if (numCols == 0) {
        return -1;
    }

    Index2D currentIndex2D = p_currentNode->getIndexAs2D();

    GLint adjacentRow = currentIndex2D.row;
    GLint adjacentCol = currentIndex2D.col;

    if (p_direction == Direction::up) {
        if (currentIndex2D.row > 0) {
            adjacentCol++;
        }
    }
    else if (p_direction == Direction::down) {
        if (currentIndex2D.row < numRows - 1) {
            adjacentCol--;
        }
    }
    else if (p_direction == Direction::left) {
        if (currentIndex2D.col > 0) {
            adjacentRow--;
        }
    }
    else if (p_direction == Direction::right) {
        if (currentIndex2D.col < numCols - 1) {
            adjacentRow++;
        }
    }
    GLint adjacentIndex = adjacentRow * numCols + adjacentCol;
    return adjacentIndex;
}

GraphNode* Graph::getNodeByPosition(Vector2D p_position) {
    return m_nodeMatrix
        [std::floor((p_position.x + gv::nodeSize) / gv::nodeSize)]
        [std::floor(p_position.y / gv::nodeSize)];
}

void Graph::render() {

}

void Graph::renderWireframe() {
    for (auto itRow = m_nodeMatrix.begin(); itRow != m_nodeMatrix.end(); ++itRow) {
        for (auto itCol = itRow->begin(); itCol != itRow->end(); ++itCol) {
            GraphNode* node = *itCol;
            node->renderWireframe();
            for (auto& edge : node->getEdges()) {
               edge->renderWireframe();
            }
        } 
    }
}