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
    pair<int, int> getRandomFarPoint(const Ant* me, int width, int height);
    vector<pair<int, int>> getResourcePath(const Ant* me);
    pair<int, int> findFarthestPointOnMap(const Ant* me, int width, int height);
    pair<int, int> getFarthestInVD(const Ant* me, pair<int, int> dest);
    bool isInRange(int x, int y);
    AI();

    void sendPoints(const Ant* me);
    unsigned char* encodeMessage(const Ant* me);
    void decodeMessage(const Ant* me, int currentTurn, const Game* game);
    void receivePoints(const Ant* me, const Game* game);

    vector<vector<int>> savedMap;
    vector<pair<int, int>> goingPath;
    vector<vector<int>> Texts;
    vector<string> dContents;
    vector<pair<int, int>> randomAreas;

    pair<int, int> farthestPoint;
    pair<int, int> previousPoint;
    pair<int, int> attackPoint;

    int currentTurn=0;
    int messageValue;

    string sendingContents;
    string currentDir;
    string nextDir;

    bool ImInAttack;
    vector<bool> dAttack;

};

#endif // AIC21_CLIENT_CPP_AI_H
