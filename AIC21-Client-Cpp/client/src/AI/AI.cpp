#include "AI.h"
#include <iostream>
#include "Models/enums.h"
#include <unordered_map>
#include <queue>
#include<time.h>

using namespace std;

vector<pair<int, int>> directions{{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

// For creating map with pair type keys
struct hash_pair {
    size_t operator()(const pair<int, int>& p) const
    {
        auto hash1 = hash<int>{}(p.first);
        auto hash2 = hash<int>{}(p.second);
        return hash1 ^ hash2;
    }
};

vector<pair<int, int>> AI::getResourcePath(const Ant* me)
{
    // TODO: Maybe should check for view distance
    pair<int, int> node;
    pair<int, int> start_node{me->getX(), me->getY()};
    queue<pair<int, int>> neighbors;
    vector<pair<int, int>> path;
    unordered_map<pair<int, int>, pair<int, int>, hash_pair> visited;
    const Cell* cell;

    neighbors.push(start_node);
    while (neighbors.size() > 0) {
        node = neighbors.front();
        neighbors.pop();

        if (savedMap[node.first][node.second] == 2 ||
            savedMap[node.first][node.second] == 3 )
        {
            while (node != start_node) {
                path.push_back(node);
                node = visited[node];
            }
            // Return the path to nearest food in reversed format
            return path;
        }
        
        for (pair<int, int> direction : directions) {
            pair<int, int> neighbour{node.first+direction.first, node.second+direction.second};
            
            if (neighbour.first < 0 || neighbour.second < 0 ||
                neighbour.first >= savedMap.size() ||
                neighbour.second >= savedMap[0].size() ||
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

pair<int, int> AI::getRandomFarPoint(const Ant* me, int width, int height)
{
    // TODO: Think about if this random is valid!
    // TODO: Remove enemies base point from random points
    int randDir = (rand()%200 + me->getX() + me->getY() + currentTurn) % 5;
    if (randDir == 0) return {width/2, height/2};
    if (randDir == 1) return {width, 1};
    if (randDir == 2) return {1, height};
    if (randDir == 3) return {width, height};
    if (randDir == 4) return {1, 1};
}


// Create a Direction object based on current position and next points
Direction AI::getDirection(const Ant* me)
{
    int x = me->getX();
    int y = me->getY();

    // cout << "Babakht shodim :)" << goingPath.size() << "\n";
    
    if (goingPath.size() != 0)
    {
        pair<int, int> latestPath = goingPath.back();
        goingPath.pop_back();

        if (latestPath.second < y)
            return UP;

        if (latestPath.second > y)
            return DOWN;

        if (latestPath.first < x)
            return LEFT;

        if (latestPath.first > x)
            return RIGHT;
    }

    // If code reaches here, try to find a free path that is not previous point
    const Cell* cell = me->getNeighborCell(0, -1);
    pair<int, int> nextPoint = {cell->getX(), cell->getY()};
    if (cell->getType() != WALL && nextPoint != previousPoint)
        return UP;

    cell = me->getNeighborCell(1, 0);
    nextPoint = {cell->getX(), cell->getY()};
    if (cell->getType() != WALL && nextPoint != previousPoint)
        return RIGHT;
    
    cell = me->getNeighborCell(0, 1);
    nextPoint = {cell->getX(), cell->getY()};
    if (cell->getType() != WALL && nextPoint != previousPoint)
        return DOWN;
    
    cell = me->getNeighborCell(-1, 0);
    nextPoint = {cell->getX(), cell->getY()};
    if (cell->getType() != WALL && nextPoint != previousPoint)
        return LEFT;
    
    // If code reaches here, there is no free path so should return to previous point
    if (previousPoint.second < y)
        return UP;

    if (previousPoint.second > y)
        return DOWN;

    if (previousPoint.first < x)
        return LEFT;

    return RIGHT;
}

vector<pair<int, int>> AI::findPath(const Ant* me, pair<int, int> dest)
{
    pair<int, int> node;
    pair<int, int> start_node{me->getX(), me->getY()};
    queue<pair<int, int>> neighbors;
    vector<pair<int, int>> path;
    unordered_map<pair<int, int>, pair<int, int>, hash_pair> visited;

    neighbors.push(start_node);

    //TODO: if cant go in dest. cant find path because cant see map enough!
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
                neighbour.first >= savedMap.size() ||
                neighbour.second >= savedMap[0].size() ||
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


int getManhattan(pair<int, int> a, pair<int, int> b) {
    return (abs(a.first-b.first) + abs(a.second-b.second));
}


// Return farthest point on map for Sarbaz to go
pair<int, int> AI::findFarthestPointOnMap(const Ant* me, int width, int height) {
    int halfWidth=width/2;
    int halfHeight=height/2;
    int x=me->getX();
    int y=me->getY(); 

    if (x >= halfWidth && y >= halfHeight) {
        return {3, 3};
    }

    if (x <= halfWidth && y >= halfHeight) {
        return {width-3, 3};
    }

    if (x <= halfWidth && y <= halfHeight) {
        return {width-5, height-5};
    }

    if (x >= halfWidth && y <= halfHeight) {
        return {3, height-3};
    }

    return {halfWidth, halfHeight};
}


// Return farthest cell in view distance which is nearest to destination 
pair<int, int> AI::getFarthestInVD(const Ant* me, pair<int, int> dest)
{
    int x=me->getX();
    int y=me->getY();
    int viewDistance=me->getViewDistance();
    const Cell* cell;
    vector<pair<int, int>> points;
    vector<int> distances;
    
    // Top-Left and Down-Right Diagonals
    int j=0;
    for (int i=-1*viewDistance; i <= 0; ++i)
    {
        cell = me->getNeighborCell(i, j);
        // TODO: This is just for testing, should changed with better condition
        if (getManhattan({cell->getX(), cell->getY()}, {x, y}) <= viewDistance && cell->getType() != WALL) {
            points.push_back({cell->getX(), cell->getY()});
            distances.push_back(getManhattan({cell->getX(), cell->getY()}, dest));
        }

        cell = me->getNeighborCell(-1*i, -1*j);
        if (getManhattan({cell->getX(), cell->getY()}, {x, y}) <= viewDistance && cell->getType() != WALL) {
            points.push_back({cell->getX(), cell->getY()});
            distances.push_back(getManhattan({cell->getX(), cell->getY()}, dest));
        }
        --j;
    }

    // Top-Right and Down-Left Diagonals
    j=1;
    for (int i=(-1*viewDistance)+1; i < 0; ++i)
    {
        cell = me->getNeighborCell(i, j);
        if (getManhattan({cell->getX(), cell->getY()}, {x, y}) <= viewDistance && cell->getType() != WALL) {
            points.push_back({cell->getX(), cell->getY()});
            distances.push_back(getManhattan({cell->getX(), cell->getY()}, dest));
        }
        cell = me->getNeighborCell(-1*i, -1*j);
        if (getManhattan({cell->getX(), cell->getY()}, {x, y}) <= viewDistance && cell->getType() != WALL) {
            points.push_back({cell->getX(), cell->getY()});
            distances.push_back(getManhattan({cell->getX(), cell->getY()}, dest));
        }
        ++j;
    }
    // Finding nearest point to destination
    int lowestDistanceIdx=0;
    for (int i=1; i < distances.size(); ++i) {
        if (distances[i] < distances[lowestDistanceIdx]) {
            lowestDistanceIdx = i;
        }
    }
    return points[lowestDistanceIdx];
}


void AI::saveMap(const Ant* me)
{
    int viewDistance = me->getViewDistance();
    bool goadd = true;
    int row = 0;
    int col = -1*viewDistance;
    for (int i = -1*row; i <= row; i++)
    {
        const Cell* cell = me->getNeighborCell(i, col);
        if (cell->getType() == WALL)
            savedMap[cell->getX()][cell->getY()] = 0;

        else if (cell -> getResource() -> getType() == BREAD)
            savedMap[cell->getX()][cell->getY()] = 2;

        else if (cell -> getResource() -> getType() == GRASS)
            savedMap[cell->getX()][cell->getY()] = 3;

        else
            savedMap[cell->getX()][cell->getY()] = 1;

        if (col == viewDistance)
            break;
        else if (i == row)
        {
            if (row == viewDistance) {
                goadd = false;
            }
            if (goadd) {
                row++;
                col++;
                i = -1*row - 1;
            }
            else {
                row--;
                col++;
                i = -1*row - 1;
            }
        }
    }
}


Answer* AI::turn(Game* game)
{
    const Ant* me = game->getAnt();
    pair<int, int> nextGoingPoints{-1, -1}; // (x, y)
    string currentStatus="Going to saved path!";
    
    // Initialize all matrix elements to -1 at beginning of the game
    if (currentTurn == 0)
    {
        int width=game->getMapWidth();
        int height=game->getMapHeight();
        farthestPoint = {-1, -1};
        savedMap.resize(width);
        mapHeight=game->getMapHeight();
        mapWidth=game->getMapWidth();
        previousPoint = {me->getX(), me->getY()};

        for (int i=0; i < width; ++i)
            savedMap[i].resize(height);

        for (int i=0; i < width; ++i)
            for (int j=0; j < height; ++j)
                savedMap[i][j] = -1;

        //cout << game->getAntType() << "\n";
        //cout << "Starting" << game->getAnt()->getX() << "," << game->getAnt()->getY() << "\n";
    }


    saveMap(me);

    // Going to random direction in first turn
    if (currentTurn == 0 && me->getType() == KARGAR) {
        int randDir=rand()%4;
        Direction direction;
        // cout << "Random number" << rand() << " " << randDir << "\n";
        if (randDir == 0) direction = UP;
        if (randDir == 1) direction = RIGHT;
        if (randDir == 2) direction = LEFT;
        if (randDir == 3) direction = DOWN;
        ++currentTurn;
        return new Answer(direction);
    }

    // Decide what to do if the ant is Kargar or Sarbaz
    if (me->getType() == AntType::KARGAR)
    {
        if (goingPath.size() == 0) {
            if (me->getCurrentResource()->getType() != ResourceType::NONE) {
                // Return Kargar to Base
                nextGoingPoints.first = game->getBaseX();
                nextGoingPoints.second = game->getBaseY();
                //currentStatus = "Kargar: Returning to base";
            }
            else {
                // Search for food
                goingPath = getResourcePath(me);
                //currentStatus = "Kargar: Found a resource";
            }
        }
    }
    else
    {
        if (goingPath.size() == 0) {
            if (getManhattan(farthestPoint, {me->getX(), me->getY()}) <= me->getViewDistance())
                farthestPoint = {-1, -1};
            
            if (farthestPoint.first == -1)
                farthestPoint = getRandomFarPoint(me, game->getMapWidth(), game->getMapHeight());

            //cout << "Farthest:" << farthestPoint.first << "," << farthestPoint.second << "\n";
            nextGoingPoints = getFarthestInVD(me, farthestPoint);
            //cout << "IN VD:" << nextGoingPoints.first << "," << nextGoingPoints.second << "\n";
        }
    }

    // If didn't found any resource, going to some random points
    if (me -> getType() == KARGAR && goingPath.size() == 0 && nextGoingPoints.first == -1) {
        //cout << "Didnt found any food, going random\n";
        if (getManhattan(farthestPoint, {me->getX(), me->getY()}) <= me->getViewDistance())
                farthestPoint = {-1, -1};

        if (farthestPoint.first == -1)
            farthestPoint = getRandomFarPoint(me, game->getMapWidth(), game->getMapHeight());

        nextGoingPoints = getFarthestInVD(me, farthestPoint);
        // currentStatus = "Kargar: Going to some random direction";
    }


    if (goingPath.size() == 0) {
        goingPath = findPath(me, nextGoingPoints);
    }

    Direction direction = getDirection(me);
    previousPoint = {me->getX(), me->getY()};
    ++currentTurn;
    return new Answer(direction, currentStatus, 10);
}

AI::AI() {
    srand(time(nullptr));
}