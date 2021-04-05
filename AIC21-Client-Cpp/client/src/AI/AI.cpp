#include "AI.h"
#include <iostream>
#include "Models/enums.h"
#include <queue>
#include <unordered_map>

using namespace std;

pair<int, int> getResourcePoints(const Ant* me) {
    const Cell* cell = me->getNeighborCell(0, -1);
    if (cell != nullptr && cell->getResource()->getType() != NONE) return {cell->getX(), cell->getY()};
    cell = me->getNeighborCell(-1, 0);
    if (cell != nullptr && cell->getResource()->getType() != NONE) return {cell->getX(), cell->getY()};
    cell = me->getNeighborCell(1, 0);
    if (cell != nullptr && cell->getResource()->getType() != NONE) return {cell->getX(), cell->getY()};
    cell = me->getNeighborCell(0, 1);
    if (cell != nullptr && cell->getResource()->getType() != NONE) return {cell->getX(), cell->getY()};

    int targetX=-1, targetY=-1, farthest=-1;

    while (-1*farthest <= me->getViewDistance() && targetX == -1) {
        // Check left and right columns
        for (int j=farthest+1; j < -1*farthest && farthest != -1; ++j) {
            cell = me->getNeighborCell(farthest, j);
            if (cell != nullptr && cell->getResource()->getType() != ResourceType::NONE) {
                targetX = cell->getX();
                targetY = cell->getY();
                break;
            }
            cell = me->getNeighborCell(-1*farthest, j);
            if (cell != nullptr && cell->getResource()->getType() != ResourceType::NONE) {
                targetX = cell->getX();
                targetY = cell->getY();
                break;
            }
        }
        
        // Check up and down rows
        for (int i=farthest; i <= -1*farthest && targetX == -1; ++i) {
            cell = me->getNeighborCell(i, farthest);
            if (cell != nullptr && cell->getResource()->getType() != ResourceType::NONE) {
                targetX = cell->getX();
                targetY = cell->getY();
                break;
            }
            cell = me->getNeighborCell(i, -1*farthest);
            if (cell != nullptr && cell->getResource()->getType() != ResourceType::NONE) {
                targetX = cell->getX();
                targetY = cell->getY();
                break;
            }
        }
        --farthest;
    }

    return {targetX, targetY};
}


pair<int, int> getRandomPoints(const Ant* me) {
    const Cell* cell = me->getNeighborCell(0, -1);
    if (cell != nullptr && cell->getType() != WALL) return {cell->getX(), cell->getY()};
    cell = me->getNeighborCell(-1, 0);
    if (cell != nullptr && cell->getType() != WALL) return {cell->getX(), cell->getY()};
    cell = me->getNeighborCell(1, 0);
    if (cell != nullptr && cell->getType() != WALL) return {cell->getX(), cell->getY()};
    cell = me->getNeighborCell(0, 1);
    return {cell->getX(), cell->getY()};
}

// Create a Direction object based on current position and next points
Direction AI::getDirection(const Ant* me) {
    int x = me->getX();
    int y = me->getY();
    
    if (goingPath.size() != 0) {
        pair<int, int> latestPath = goingPath.back();
        goingPath.pop_back();
        if (latestPath.second < y) {
            return UP;
        }
        else if (latestPath.second > y) {
            return DOWN;
        }
        else if (latestPath.first < x) {
            return LEFT;
        }
        else if (latestPath.first > x) {
            return RIGHT;
        }
    }

    // TODO: If code reaches here, something bad has happened and should think about it!!!
    cout << "fuck fuck fuck";
    return UP;
}


// The whole part of finding shortest path
vector<pair<int, int>> directions{{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
// https://www.geeksforgeeks.org/how-to-create-an-unordered_map-of-pairs-in-c
struct hash_pair {
    size_t operator()(const pair<int, int>& p) const
    {
        auto hash1 = hash<int>{}(p.first);
        auto hash2 = hash<int>{}(p.second);
        return hash1 ^ hash2;
    }
};
vector<pair<int, int>> AI::findPath(const Ant* me, pair<int, int> dest)
{
    pair<int, int> node;
    pair<int, int> start_node{me->getX(), me->getY()};
    queue<pair<int, int>> neighbors;
    vector<pair<int, int>> path;
    unordered_map<pair<int, int>, pair<int, int>, hash_pair> visited;
    
    neighbors.push(start_node);

    while (neighbors.size() > 0) {
        node = neighbors.front();
        neighbors.pop();
        if (node == dest) {
            node = dest;
            while (node != start_node) {
                path.push_back(node);
                node = visited[node];
            }
            // Return the path to destination in reversed format
            return path;
        }

        for (pair<int, int> direction : directions) {
            pair<int, int> neighbour{node.first+direction.first, node.second+direction.second};
            
            if (neighbour.first < 0 || neighbour.second < 0 ||
                neighbour.first > savedMap.size() ||
                neighbour.second > savedMap[0].size() ||
                savedMap[neighbour.first][neighbour.second] == -1 ||
                savedMap[neighbour.first][neighbour.second] == 0)
            {
                continue;
            }
            if (visited.find(neighbour) != visited.end()) {
                continue;
            }
            visited[neighbour] = node;
            neighbors.push(neighbour);
        }
    }
    return path;
}




void AI::saveMap(const Ant* me) {
    int viewDistance=me->getViewDistance();
    const Cell* cell;
    
    // TODO: should not start from -1*viewDistance and go to viewDistance
    for (int i=-1*viewDistance; i <= viewDistance; ++i)
        for (int j=-1*viewDistance; j <= viewDistance; ++j) {
            cell = me->getNeighborCell(i, j);
            if (cell != nullptr) {
                if (cell->getType() == WALL) {
                    savedMap[cell->getX()][cell->getY()] = 0;
                }
                else {
                    savedMap[cell->getX()][cell->getY()] = 1;
                }

            }
        }

}

Answer* AI::turn(Game* game) {
    // Initialize all matrix elements to -1 at beginning of the game
    if (currentTurn == 0) {
        int width=game->getMapWidth();
        int height=game->getMapHeight();
        savedMap.resize(width);
        for (int i=0; i < width; ++i)
            savedMap[i].resize(height);
        
        for (int i=0; i < width; ++i)
            for (int j=0; j < height; ++j)
                savedMap[i][j] = -1; 
    }

    const Ant* me = game->getAnt();
    pair<int, int> nextGoingPoints{-1, -1}; // (x, y)
    string currentStatus="Going to saved path!";

    saveMap(me);    

    // Decide what to do if the ant is Kargar or Sarbaz
    if (me->getType() == AntType::KARGAR) {
        if (goingPath.size() == 0) {
            if (me->getCurrentResource()->getType() == ResourceType::NONE) {
                nextGoingPoints = getResourcePoints(me);
                currentStatus = "Found a resource";
            }
            else {
                nextGoingPoints.first = game->getBaseX();
                nextGoingPoints.second = game->getBaseY();
                currentStatus = "Returning to base";
            }
        }
    }
    else {
        // TODO: Decide what to do with Sarbaz
    }

    // If didn't found any resource, going to some random points
    if (nextGoingPoints.first == -1) {
        nextGoingPoints = getRandomPoints(me);
        currentStatus = "Going to some random direction";
    }
    if (goingPath.size() == 0) {
        goingPath = findPath(me, nextGoingPoints);
    }

    ++currentTurn;
    Direction direction = getDirection(me);
    return new Answer(direction, currentStatus, 10);
    
}
