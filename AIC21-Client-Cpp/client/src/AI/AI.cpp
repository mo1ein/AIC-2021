#include "AI.h"
#include <iostream>
#include "Models/enums.h"

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
Direction getDirection(const Ant* me, pair<int, int> nextPoints) {
    int x = me->getX();
    int y = me->getY();
    const Cell* up_cell = me->getNeighborCell(0, -1);
    if (up_cell != nullptr && up_cell->getType() != WALL && nextPoints.second < y) {
        return Direction::UP;
    }
    const Cell* left_cell = me->getNeighborCell(-1, 0);
    if (left_cell != nullptr && left_cell->getType() != WALL && nextPoints.first < x) {
        return Direction::LEFT;
    }
    const Cell* right_cell = me->getNeighborCell(1, 0);
    if (right_cell != nullptr && right_cell->getType() != WALL && nextPoints.first > x) {
        return Direction::RIGHT;
    }
    const Cell* down_cell = me->getNeighborCell(0, 1);
    if (down_cell != nullptr && down_cell->getType() != WALL && nextPoints.second > y) {
        return Direction::DOWN;
    }

    // TODO: If code reaches here, should find a better path. This should be changed
    if (up_cell == nullptr || up_cell->getType() != WALL) {
        return Direction::UP;
    }
    else if (left_cell == nullptr || left_cell->getType() != WALL) {
        return Direction::LEFT;
    }
    else if (right_cell == nullptr || right_cell->getType() != WALL) {
        return Direction::RIGHT;
    }
    else {
        return Direction::DOWN;
    }

}


Answer* AI::turn(Game* game) {
    const Ant* me = game->getAnt();
    pair<int, int> nextGoingPoints{-1, -1}; // (x, y)
    string currentStatus;

    // Decide what to do if the ant is Kargar or Sarbaz
    if (me->getType() == AntType::KARGAR) {
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
    else {
        // TODO: Decide what to do with Sarbaz
    }

    // If didn't found any resource, going to some random points
    if (nextGoingPoints.first == -1) {
        nextGoingPoints = getRandomPoints(me);
        currentStatus = "Going to some random direction";
    } 


    Direction direction = getDirection(me, nextGoingPoints);
    return new Answer(direction, currentStatus, 10);
    
}
