#ifndef PATHFINDER
#define PATHFINDER
#include <iostream>
#include <vector>
#include <cmath>

// Pathfinder class using A* algorithm
class Pathfinder {
    private:
    // Attributes
    const int DIRECTIONS[4][2] = {{0, 1}, {1, 0},{ 0, -1}, {-1, 0}};
    std::vector<std::vector<int>> map;
    int xMin;
    int xMax;
    int yMin;
    int yMax;
    int simplificationFactor = 1;

    // Internal node class
    class Node {
        public:
        // Attributes
        int x;
        int y;
        Node* predecessor;
        int pastCost;
        int futureCost;

        // Constructor
        Node(int x, int y, Node* predecessor = nullptr);

        // Method to get the total cost
        int getTotalCost();
    };

    // Helper method to detect a collision
    bool detectCollision(int x, int y);

    // Helper method to find a node by its coordinates
    Node* findNodeInList(std::vector<Node*> list, int x, int y);

    public:
    // Constructors
    Pathfinder();

    Pathfinder(std::vector<std::vector<int>> map);

    Pathfinder(int simplificationFactor);

    Pathfinder(std::vector<std::vector<int>> map, int simplificationFactor);

    // Getter
    int getSimplificationFactor();

    // Setter
    void setMap(std::vector<std::vector<int>> map);

    void setSimplificationFactor(int factor);

    // Method to find the shortest path
    std::vector<std::vector<int>> findPath(int sourceX, int sourceY, int targetX, int targetY);
};
#endif
