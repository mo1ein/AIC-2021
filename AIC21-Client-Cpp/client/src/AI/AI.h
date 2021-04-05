#ifndef AIC21_CLIENT_CPP_AI_H
#define AIC21_CLIENT_CPP_AI_H

#include "Models/Game.h"
#include "Models/Answer.h"

class AI {
public:

    Answer* turn(Game *game);
    void saveMap(const Ant* me);
    vector<pair<int, int>> findPath(const Ant* me, pair<int, int> dest);
    Direction getDirection(const Ant* me);

    vector<vector<int>> savedMap;
    int currentTurn=0;
    vector<pair<int, int>> goingPath;

};

#endif // AIC21_CLIENT_CPP_AI_H

