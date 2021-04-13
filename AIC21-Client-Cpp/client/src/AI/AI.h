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
    pair<int, int> getFarPoints(const Ant* me);
    vector<pair<int, int>> getResourcePath(const Ant* me);
    pair<int, int> findFarthestPointOnMap(const Ant* me, int width, int height);
    pair<int, int> getFarthestInVD(const Ant* me, pair<int, int> dest);
    bool isInRange(int x, int y);

    vector<vector<int>> savedMap;
    int currentTurn=0;
    vector<pair<int, int>> goingPath;
    pair<int, int> farthestPoint;
    int mapHeight;
    int mapWidth;
    pair<int, int> previousPoint;
};

#endif // AIC21_CLIENT_CPP_AI_H
