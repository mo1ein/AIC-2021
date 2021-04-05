#include "AI.h"
#include <iostream>
#include "Models/enums.h"

using namespace std;

Answer *AI::turn(Game *game) {
    const Ant* me = game->getAnt();

    int viewDist = me->getViewDistance();
    int targetX = -1, targetY = -1;
    if (me->getType() == KARGAR) {
        for (int i = 0; i < viewDist; i++) {
            for (int j = 0; j < viewDist; j++) {
                const Cell* cell = game->getAnt()->getNeighborCell(i, j);
                if (cell && cell->getResource()->getType() != NONE) {
                    targetX = cell->getX();
                    targetY = cell->getY();
                    break;
                }
            }
            if (targetX != -1)
                break;
        }
    }

    if (targetX == -1) {
        return new Answer(UP, "I'm searching up area!", 5);
    }

    int x = me->getX();
    int y = me->getY();
    Direction direction = CENTER;
    if (targetX > x) {
        direction = RIGHT;
    }
    else if (targetX < x) {
        direction = LEFT;
    }
    else if (targetY > y) {
        direction = DOWN;
    }
    else {
        direction = UP;
    }
    return new Answer(direction, "I found a resource and I'm going to get it! :)", 10);
}
