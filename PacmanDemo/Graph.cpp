#include "Graph.h"
#include "GraphEdge.h"

Graph* Graph::instance = nullptr;

void Graph::initNodes() {
    std::cout << "Initializing Nodes." << std::endl;
    m_nodeMatrix.resize(rows, std::vector<GraphNode*>(columns, nullptr));
    int index = 0;
    for (unsigned int row = 0; row < rows; row++) {
        for (unsigned int col = 0; col < columns; col++) {
            GraphNode* node = new GraphNode(index, Vector2D(row * nodeSize + nodeRenderOffset, col * nodeSize + nodeRenderOffset) - Vector2D(32.0f, 0.0f));

            switch (map[row][col]) {

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
                node->setPosition(node->getPosition() + Vector2D(nodeSize/2.0f, 0.0f));
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
    std::cout << "Initializing Edges." << std::endl;
    int index = 0;
    for (unsigned int row = 0; row < rows; row++) {
        for (unsigned int col = 0; col < columns; col++) {
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    if (i == 0 && j == 0) continue;
                    if (abs(i) == abs(j)) continue;
                    if (row + i >= 0 &&
                        row + i < rows &&
                        col + j >= 0 &&
                        col + j < columns) {
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
            if (m_nodeMatrix[row][col]->getIndex() == 16) {
                m_nodeMatrix[row][col]->addConnectedNode(m_nodeVector[915]);
            }
            if (m_nodeMatrix[row][col]->getIndex() == 915) {
                m_nodeMatrix[row][col]->addConnectedNode(m_nodeVector[16]);
            }
        }
    }
}

void Graph::initGraph() {
    std::cout << "Initializing Graph." << std::endl;
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
    std::cout << "Deleting Graph." << std::endl;
}

Matrix<GraphNode*> Graph::getNodes() {
    return m_nodeMatrix;
}

std::vector<GraphNode*> Graph::getNodeVector() {
    return m_nodeVector;
}

int Graph::calculateDistanceInNodes(const GraphNode* p_node1, const GraphNode* p_node2) const {
    if (!p_node1 || !p_node2)
        return -1;

    Index2D index1 = p_node1->getIndexAs2D();
    Index2D index2 = p_node2->getIndexAs2D();

    return (std::abs(index1.row - index2.row) + std::abs(index1.col - index2.col));
}

int Graph::getNodeIndexByDirection(GraphNode* p_currentNode, Direction p_direction) const {

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
    if (p_direction == Direction::down) {
        if (currentIndex2D.row < numRows - 1) {
            adjacentCol--;
        }
    }
    if (p_direction == Direction::left) {
        if (currentIndex2D.col > 0) {
            adjacentRow--;
        }
    }
    if (p_direction == Direction::right) {
        if (currentIndex2D.col < numCols - 1) {
            adjacentRow++;
        }
    }
    GLint adjacentIndex = adjacentRow * numCols + adjacentCol;
    return adjacentIndex;
}

Direction Graph::getDirectionByNode(GraphNode* p_currentNode, GraphNode* p_targetNode) const {

    if (!p_currentNode || !p_targetNode)
        return Direction::none;

    Index2D currentIndex2D = p_currentNode->getIndexAs2D();
    Index2D targetIndex2D = p_targetNode->getIndexAs2D();

    if (currentIndex2D.row > targetIndex2D.row)
        return Direction::left;

    if (currentIndex2D.row < targetIndex2D.row)
        return Direction::right;

    if (currentIndex2D.col > targetIndex2D.col)
        return Direction::down;

    if (currentIndex2D.col < targetIndex2D.col)
        return Direction::up;
   
    return Direction::none;
}

// const GraphNode* p_node - čvor u odnosu na koga se traži ciljni čvor, u ovom slučaju čvor igrača
// const Direction p_direction - trenutni smer igrača
// const int p_tileDistance - udaljenost igrača u čvorovima
GraphNode* Graph::getNodeInPlayerDirection(const GraphNode* p_node, const Direction p_direction, const int p_tileDistance) const {

    if (!p_node)     
        return nullptr;

    Index2D playerIndex2D = p_node->getIndexAs2D();

    int targetRow = playerIndex2D.row;
    int targetCol = playerIndex2D.col;

    switch (p_direction) {
    case Direction::up:
        targetCol += p_tileDistance;
        break;
    case Direction::down:
        targetCol -= p_tileDistance;
        break;
    case Direction::left:
        targetRow -= p_tileDistance;
        break;
    case Direction::right:
        targetRow += p_tileDistance;
        break;
    default:
        break;
    }

    // traži se čvor grafa sa odabranim indeksom I vraća se ukoliko je validan
    GraphNode* node = m_nodeMatrix[targetRow][targetCol];
    if (targetRow >= 0 && targetRow < static_cast<int>(m_nodeMatrix.size()) &&
        targetCol >= 0 && targetCol < static_cast<int>(m_nodeMatrix[0].size()) &&
        !node->isObstacle() &&
        !node->isEmptyNode())
        return node;
    else return nullptr;

}

// Vector2D p_position - vektor pozicije u odnosu na koga se traži čvor
GraphNode* Graph::getNodeByPosition(Vector2D p_position) {

    if (nodeSize == 0) return nullptr;

    int xIndex = std::floor((p_position.x + nodeSize) / nodeSize);
    int yIndex = std::floor(p_position.y / nodeSize);

    if (xIndex >= 0 && xIndex < m_nodeMatrix.size() &&
        yIndex >= 0 && yIndex < m_nodeMatrix[0].size()) {
        return m_nodeMatrix[xIndex][yIndex];
    }
    else return nullptr;
}

GraphNode* Graph::calculateInkyTargetNode(const GraphNode* p_node1, const GraphNode* p_node2, const Direction p_direction) {

    if (!p_node1 || !p_node2)
        return nullptr;   

    GraphNode* targetNode = getNodeInPlayerDirection(p_node1, p_direction, 2);

    if (!targetNode)
        return nullptr;

    Index2D blinkyIndex = p_node2->getIndexAs2D();
    Index2D targetIndex = targetNode->getIndexAs2D();

    int vectorX = targetIndex.row - blinkyIndex.row;
    int vectorY = targetIndex.col - blinkyIndex.col;

    int extendedVectorX = 2 * vectorX;
    int extendedVectorY = 2 * vectorY;

    int targetRow = blinkyIndex.row + extendedVectorX;
    int targetCol = blinkyIndex.col + extendedVectorY;

    GraphNode* node = m_nodeMatrix[targetRow][targetCol];
    if (targetRow >= 0 && targetRow < static_cast<int>(m_nodeMatrix.size()) &&
        targetCol >= 0 && targetCol < static_cast<int>(m_nodeMatrix[0].size()) &&
        !node->isObstacle() &&
        !node->isEmptyNode())
         return node;
    else return nullptr;
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