#include "pathfinder.hpp"

// Constructor
Pathfinder::Node::Node(int x, int y, Node* predecessor) {
    this->x = x;
    this->y = y;
    this->predecessor = predecessor;
    pastCost = 0;
    futureCost = 0;
}

// Method to get the total cost
int Pathfinder::Node::getTotalCost() {
    return pastCost + futureCost;
}

// Helper method to detect a collision
bool Pathfinder::detectCollision(int x, int y) {
    int collisionBoxSize = std::ceil(240.0 / simplificationFactor);
    bool returnValue = false;
    if (x < 0 || x > std::abs(xMin) + std::abs(xMax) || y < 0 || y > std::abs(yMin) + std::abs(yMax)) {
        returnValue = true;
    }
    for (int i = 0; i < map.size(); i++) {
        if (map[i][0] >= x - collisionBoxSize && map[i][0] <= x + collisionBoxSize && map[i][1] >= y - collisionBoxSize && map[i][1] <= y + collisionBoxSize) {
            returnValue = true;
        }
    }
    return returnValue;
}

// Helper method to find a node by its coordinates
Pathfinder::Node* Pathfinder::findNodeInList(std::vector<Node*> list, int x, int y) {
    Node* returnValue = nullptr;
    for (Node* node : list) {
        if (node->x == x && node->y == y) {
            returnValue = node;
        }
    }
    return returnValue;
}

// Constructors
Pathfinder::Pathfinder() {
}

Pathfinder::Pathfinder(std::vector<std::vector<int>> map) : Pathfinder() {
    setMap(map);
}

Pathfinder::Pathfinder(int simplificationFactor) : Pathfinder() {
    setSimplificationFactor(simplificationFactor);
}

Pathfinder::Pathfinder(std::vector<std::vector<int>> map, int simplificationFactor) : Pathfinder() {
    setMap(map);
    setSimplificationFactor(simplificationFactor);
}

// Getter
int Pathfinder::getSimplificationFactor() {
    return simplificationFactor;
}

// Setter
void Pathfinder::setMap(std::vector<std::vector<int>> map) {
    std::vector<std::vector<int>> uniqueCoordinates;
    // Simplify map and filter newly created duplicates
    for (int i = 0; i < map.size(); i++) {
        map[i][0] = std::round((float) map[i][0] / simplificationFactor);
        map[i][1] = std::round((float) map[i][1] / simplificationFactor);
    }
    for (int i = 0; i < map.size(); i++) {
        bool duplicate = false;
        for (int j = 0; j < uniqueCoordinates.size(); j++) {
            if (map[i][0] == uniqueCoordinates[j][0] && map[i][1] == uniqueCoordinates[j][1]) {
                duplicate = true;
            }
        }
        if (duplicate) {
            map.erase(map.begin() + i);
        } else {
            uniqueCoordinates.push_back(map[i]);
        }
    }
    // Determine value ranges
    xMin = 0;
    xMax = 0;
    yMin = 0;
    yMax = 0;
    for (int i = 0; i < map.size(); i++) {
        if (map[i][0] < xMin) {
            xMin = map[i][0];
        } else if (map[i][0] > xMax) {
            xMax = map[i][0];
        }
        if (map[i][1] < yMin) {
            yMin = map[i][1];
        } else if (map[i][1] > yMax) {
            yMax = map[i][1];
        }
    }
    // Move all the data to positive values
    for (int i = 0; i < map.size(); i++) {
        map[i][0] -= xMin;
        map[i][1] -= yMin;
    }
    this->map = map;
}

void Pathfinder::setSimplificationFactor(int factor) {
    if (factor < 1) {
        factor = 1;
    } else if (factor > 1000) {
        factor = 1000;
    }
    simplificationFactor = factor;
}

// Method to find the shortest path from source to target
std::vector<std::vector<int>> Pathfinder::findPath(int sourceX, int sourceY, int targetX, int targetY) {
    Node* current = nullptr;
    std::vector<Node*> openList;
    std::vector<Node*> closedList;
    std::vector<std::vector<int>> path;
    std::vector<std::vector<int>> simplifiedPath;
    // Check for valid source and target
    sourceX = std::round((float) sourceX / simplificationFactor) - xMin;
    sourceY = std::round((float) sourceY / simplificationFactor) - yMin;
    targetX = std::round((float) targetX / simplificationFactor) - xMin;
    targetY = std::round((float) targetY / simplificationFactor) - yMin;
    if (detectCollision(sourceX, sourceY) || detectCollision(targetX, targetY)) {
        return simplifiedPath;
    }
    // Initialize
    openList.push_back(new Node(sourceX, sourceY));
    // Find shortest path
    while (!openList.empty()) {
        std::vector<Node*>::iterator currentIterator = openList.begin();
        current = *currentIterator;
        // Find best node to further investigate
        for (std::vector<Node*>::iterator i = openList.begin(); i < openList.end(); i++) {
            Node* node = *i;
            if (node->getTotalCost() <= current->getTotalCost()) {
                current = node;
                currentIterator = i;
            }
        }
        // Check for success
        if (current->x == targetX && current->y == targetY) {
            break;
        }
        // Move current node to closed list
        closedList.push_back(current);
        openList.erase(currentIterator);
        // Add successors to open list
        for (int i = 0; i < 4; i++) {
            int successorX = current->x + DIRECTIONS[i][0];
            int successorY = current->y + DIRECTIONS[i][1];
            int successorPastCost = current->pastCost + 1;
            Node* successor;
            // Skip invalid successor
            if (detectCollision(successorX, successorY) || findNodeInList(closedList, successorX, successorY)) {
                continue;
            }
            // Add or update successor
            successor = findNodeInList(openList, successorX, successorY);
            if (successor == nullptr) {
                successor = new Node(successorX, successorY, current);
                successor->pastCost = successorPastCost;
                successor->futureCost = abs(successorX - targetX) + abs(successorY - targetY);
                openList.push_back(successor);
            } else if (successorPastCost < successor->pastCost) {
                successor->predecessor = current;
                successor->pastCost = successorPastCost;
            }
        }
    }
    // Reconstruct path
    while (current != nullptr) {
        std::vector<int> coordinates;
        coordinates.push_back(current->x);
        coordinates.push_back(current->y);
        coordinates[0] = (coordinates[0] + xMin) * simplificationFactor;
        coordinates[1] = (coordinates[1] + yMin) * simplificationFactor;
        path.insert(path.begin(), coordinates);
        current = current->predecessor;
    }
    // Simplify path to corners
    for (int i = 1; i < path.size() - 1; i++) {
        if ((path[i][0] == path[i - 1][0] && path[i][0] != path[i + 1][0]) || (path[i][1] == path[i - 1][1] && path[i][1] != path[i + 1][1])) {
            simplifiedPath.push_back(path[i]);
        }
    }
    simplifiedPath.push_back(path.back());
    // Check for successful pathfinding
    if (simplifiedPath.back()[0] != (targetX + xMin) * simplificationFactor || simplifiedPath.back()[1] != (targetY + yMin) * simplificationFactor) {
        simplifiedPath.clear();
    }
    return simplifiedPath;
}
