#ifndef AIC21_CLIENT_CPP_AI_H
#define AIC21_CLIENT_CPP_AI_H

#include "Models/Game.h"
#include "Models/Answer.h"
#include <random>

class AI {
public:

    Answer* turn(Game *game);
    void saveMap(const Ant* me);
    vector<pair<int, int>> findPath(const Ant* me, pair<int, int> dest);
    vector<pair<int, int>> findPathSafe(const Ant* me, pair<int, int> dest);
    Direction getDirection(const Ant* me);
    pair<int, int> getRandomFarPoint(const Ant* me, int width, int height);
    vector<pair<int, int>> getResourcePath(const Ant* me);
    pair<int, int> findFarthestPointOnMap(const Ant* me, int width, int height);
    pair<int, int> getFarthestInVD(const Ant* me, pair<int, int> dest);
    bool isInRange(int x, int y);
    AI();
    vector<pair<int, int>> findFirstUnfound(const Ant* me);
    vector<pair<int, int>> findFirstUnfoundSafe(const Ant* me);

    unsigned char* encodeMessage(const Ant* me);
    void decodeMessage(const Ant* me, const Game* game);
    void receivePoints(const Ant* me, const Game* game);

    vector<vector<int>> savedMap;
    vector<vector<int>> Texts;
    vector<pair<int, int>> goingPath;
    vector<pair<int, int>> randomAreas;
    vector<string> dContents;

    pair<int, int> farthestPoint;
    pair<int, int> previousPoint;
    pair<int, int> attackPoint;
    pair<int, int> enemyPoint;
    pair<int, int> ourBase;

    int width;
    int height;
    int maxHeightOrWidth;
    int messageValue;
    int currentTurn = 0;
    std::default_random_engine rng;

    string sendingContents;
    string enemyBase;

    bool ImInAttack;
    bool foundBase;
    vector<bool> dAttack;
};

#endif // AIC21_CLIENT_CPP_AI_H
