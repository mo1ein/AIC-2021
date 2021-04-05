#include "AI.h"
#include <iostream>
#include "Models/enums.h"

using namespace std;

Answer *AI::turn(Game *game) {
    const Ant* me = game->getAnt();
    int viewDist = me->getViewDistance();
    int targetX = -1, targetY = -1;
    return new Answer(direction, "I found a resource and I'm going to get it! :)", 10);
}
