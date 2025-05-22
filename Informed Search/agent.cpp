#include "agent.h"

int gridMatrix[GRID_SIZE][GRID_SIZE] = { 0 };


// Heuristic function (Manhattan distance)
int Agent::heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// A* pathfinding implementation
bool Agent::findPathAStar(int startX, int startY, int goalX, int goalY, int gridMatrix[][GRID_SIZE]) {
    // Convert pixel coordinates to grid coordinates
    startX /= CELL_SIZE;
    startY /= CELL_SIZE;
    goalX /= CELL_SIZE;
    goalY /= CELL_SIZE;

    // Clear previous path
    path.clear();

    // Priority queue for open list (min-heap)
    std::priority_queue<Node> openList;
    // Set for closed list to track visited nodes
    std::set<std::pair<int, int>> closedList;

    // Create start node
    Node startNode(startX, startY, 0, heuristic(startX, startY, goalX, goalY));
    openList.push(startNode);

    // Possible movements (up, down, left, right)
    const int dx[] = { 0, 0, -1, 1 };
    const int dy[] = { -1, 1, 0, 0 };

    while (!openList.empty()) {
        // Get node with lowest f-cost
        Node current = openList.top();
        openList.pop();

        // Add to our closed list
        closedList.insert({ current.x, current.y });

        // Check if goal is reached
        if (current.x == goalX && current.y == goalY) {
            // Reconstruct path
            while (current.parent != nullptr) {
                path.push_back({ current.x * CELL_SIZE, current.y * CELL_SIZE }); 
                current = *current.parent;
            }
            // Reverse to get path from start to goal
            std::reverse(path.begin(), path.end()); 
            return true;
        }

        // For all neighbors
        for (int i = 0; i < 4; ++i) {
            int newX = current.x + dx[i];
            int newY = current.y + dy[i];

            // Check if neighbor is valid
            if (newX >= 0 && newX < GRID_SIZE && newY >= 0 && newY < GRID_SIZE &&
                gridMatrix[newX][newY] != 1 &&
                closedList.find({ newX, newY }) == closedList.end()) { 
                int newG = current.g + 1; 
                int newH = heuristic(newX, newY, goalX, goalY);
                Node* neighbor = new Node(newX, newY, newG, newH, new Node(current));
                openList.push(*neighbor);
                delete neighbor; 
            }
        }
    }

    return false;
}

// Find samples using A*
void Agent::move(int gridMatrix[][GRID_SIZE]) {
    if (!isMoving || hasSample) {
        return;
    }

    // Closest sample
    int closestSampleX = -1, closestSampleY = -1;
    int minDist = INT_MAX;
    for (const auto& sample : activeSamples) {
        int dist = heuristic(x / CELL_SIZE, y / CELL_SIZE, sample.first, sample.second);
        if (dist < minDist) {
            minDist = dist;
            closestSampleX = sample.first * CELL_SIZE;
            closestSampleY = sample.second * CELL_SIZE;
        }
    }

    if (closestSampleX == -1) {
        return; 
    }

    if (path.empty()) {
        if (!findPathAStar(x, y, closestSampleX, closestSampleY, gridMatrix)) {
            return; // No path found
        }
    }

    if (!path.empty()) {
        auto nextPos = path.front();
        x = nextPos.first;
        y = nextPos.second;
        path.erase(path.begin()); 
    }
}

void Agent::moveTowardsShip(int shipX, int shipY, int gridMatrix[][GRID_SIZE]) {
    if (!hasSample || !isMoving) {
        return;
    }

    if (path.empty()) {
        if (!findPathAStar(x, y, shipX, shipY, gridMatrix)) {
            return; 
        }
    }

    if (!path.empty()) {
        auto nextPos = path.front();
        x = nextPos.first;
        y = nextPos.second;
        path.erase(path.begin());
    }
}

