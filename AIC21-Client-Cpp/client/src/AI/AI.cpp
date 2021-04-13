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

        if (savedMap[node.first][node.second] == 2)
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

pair<int, int> AI::getFarPoints(const Ant* me)
{
    // set system time for seed
    srand((int)time(0));
    vector<int> valid;


    int myView = me -> getViewDistance();

    // far viewDistance points
    const Cell* cell_1 = me -> getNeighborCell(0, myView);
    const Cell* cell_2 = me -> getNeighborCell(0, -myView);
    const Cell* cell_3 = me -> getNeighborCell(-myView, 0);
    const Cell* cell_4 = me -> getNeighborCell(myView, 0);

    if (me -> getType() == KARGAR) {
        myView = 1;
        cell_1 = me -> getNeighborCell(0, myView);
        cell_2 = me -> getNeighborCell(0, -myView);
        cell_3 = me -> getNeighborCell(-myView, 0);
        cell_4 = me -> getNeighborCell(myView, 0);
    }

    if (cell_1 != nullptr && cell_1 -> getType() != WALL && savedMap[cell_1 -> getX()][cell_1 -> getY()] != -1)
        valid.push_back(1);

    if (cell_2 != nullptr && cell_2 -> getType() != WALL && savedMap[cell_2 -> getX()][cell_2 -> getY()] != -1)
        valid.push_back(2);

    if (cell_3 != nullptr && cell_3 -> getType() != WALL && savedMap[cell_3 -> getX()][cell_3 -> getY()] != -1)
        valid.push_back(3);

    if (cell_4 != nullptr && cell_4 -> getType() != WALL && savedMap[cell_4 -> getX()][cell_4 -> getY()] != -1)
        valid.push_back(4);

    int choice = rand() % valid.size();

    if (valid[choice] == 1)
        return {cell_1 -> getX(), cell_1 -> getY()};

    if (valid[choice] == 2)
        return {cell_2 -> getX(), cell_2 -> getY()};

    if (valid[choice] == 3)
        return {cell_3 -> getX(), cell_3 -> getY()};

    return {cell_4 -> getX(), cell_4 -> getY()};

    // TODO: check when see enemy
}


// Create a Direction object based on current position and next points
Direction AI::getDirection(const Ant* me)
{
    int x = me->getX();
    int y = me->getY();

    cout << "Babakht shodim :)" << goingPath.size() << "\n";
    
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


void AI::saveMap(const Ant* me) {
    int viewDistance=me->getViewDistance();
    const Cell* cell;

    // TODO: fix viewDistance
    // TODO: should not start from -1*viewDistance and go to viewDistance
    for (int i=-1*viewDistance; i <= viewDistance; ++i)
        for (int j=-1*viewDistance; j <= viewDistance; ++j) {
            cell = me->getNeighborCell(i, j);
            if (cell != nullptr) {
                if (cell->getType() == WALL) {
                    savedMap[cell->getX()][cell->getY()] = 0;
                }
                else if (cell->getResource()->getType() != NONE) {
                    savedMap[cell->getX()][cell->getY()] = 2;
                }
                else {
                    savedMap[cell->getX()][cell->getY()] = 1;
                }
            }
        }
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

        cout << game->getAntType() << "\n";
        cout << "Starting" << game->getAnt()->getX() << "," << game->getAnt()->getY() << "\n";
    }


    saveMap(me);

    // Decide what to do if the ant is Kargar or Sarbaz
    if (me->getType() == AntType::KARGAR)
    {
        if (goingPath.size() == 0) {
            if (me->getCurrentResource()->getType() == ResourceType::NONE ||
                me->getCurrentResource()->getValue() < 10) {
                goingPath = getResourcePath(me);
                currentStatus = "Kargar: Found a resource";
            }
            else {
                nextGoingPoints.first = game->getBaseX();
                nextGoingPoints.second = game->getBaseY();
                currentStatus = "Kargar: Returning to base";
            }
        }
    }
    else
    {
        if (goingPath.size() == 0) {
            if (getManhattan(farthestPoint, {me->getX(), me->getY()}) <= me->getViewDistance())
                farthestPoint = {-1, -1};
            
            if (farthestPoint.first == -1)
                farthestPoint = findFarthestPointOnMap(me, game->getMapWidth(), game->getMapHeight());

            cout << "Farthest:" << farthestPoint.first << "," << farthestPoint.second << "\n";
            pair<int ,int > a = getFarthestInVD(me, farthestPoint);
            cout << "IN VD:" << a.first << "," << a.second << "\n";
            /*nextGoingPoints = getFarthestInVD(me, farthestPoint);
            cout << "G:" << nextGoingPoints.first << "," << nextGoingPoints.second << "\n";*/
            nextGoingPoints = a;
        }
    }

    // If didn't found any resource, going to some random points
    if (me -> getType() == KARGAR && goingPath.size() == 0 && nextGoingPoints.first == -1) {
        nextGoingPoints = getFarPoints(me);
        currentStatus = "Kargar: Going to some random direction";
    }


    if (goingPath.size() == 0) {
        cout << "Current:" << me->getX() << "," << me->getY() << "\n";
        cout << "Going:" << nextGoingPoints.first << "," << nextGoingPoints.second << "\n";
        goingPath = findPath(me, nextGoingPoints);
    }
    cout << "Jan Jan\n";
        


    ++currentTurn;
    Direction direction = getDirection(me);
    previousPoint = {me->getX(), me->getY()};
    return new Answer(direction, currentStatus, 10);
}
