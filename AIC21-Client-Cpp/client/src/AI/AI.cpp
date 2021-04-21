#include "AI.h"
#include <iostream>
#include "Models/enums.h"
#include <unordered_map>
#include <queue>
#include <time.h>
#include <string>
#include <bitset>

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


pair<int, int> AI::getRandomFarPoint(const Ant* me, int width, int height)
{
    // TODO: Remove enemies base point from random points
    int randDir = (rand() % 500 + me->getX() + me->getY() + currentTurn) % randomAreas.size();
    pair<int, int> randomArea = randomAreas[randDir];
    randomAreas.erase(randomAreas.begin() + randDir);
    return randomArea;
}


// Create a Direction object based on current position and next points
Direction AI::getDirection(const Ant* me)
{
    int x = me->getX();
    int y = me->getY();

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

    // If code reaches here, try to find a free path that is not point
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

    // If code reaches here, there is no free path so should return to previous point and also forget about going path
    farthestPoint = {-1, -1};
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
        return {1, 1};
    }

    if (x <= halfWidth && y >= halfHeight) {
        return {width-1, 1};
    }

    if (x <= halfWidth && y <= halfHeight) {
        return {width-1, height-1};
    }

    if (x >= halfWidth && y <= halfHeight) {
        return {1, height-1};
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


void AI::sendPoints(const Ant* me)
{
    sendingContents = "";
    int viewDistance = me->getViewDistance();
    bool goadd = true;
    int row = 0;
    int col = -1*viewDistance;

    // encoding points...
    for (int i = -1*row; i <= row; i++)
    {
        const Cell* cell = me->getNeighborCell(i, col);
        if (cell->getType() == WALL){
            sendingContents += "00";
        }
        else if (cell -> getResource() -> getType() == BREAD){
            sendingContents += "10";
            messageValue = 30;
        }
        else if (cell -> getResource() -> getType() == GRASS){
            sendingContents += "11";
            messageValue = 30;
        }
        else{
            sendingContents += "01";
        }

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


unsigned char* AI::encodeMessage(const Ant* me)
{
    // encoding message...
    string message;
    // not need whereGo yet
    string offset = "001"; // for printable chars
    string whereGo = "00";
    string antType;
    string isAttacked;
    string currentPointx = bitset<6>(me->getX()).to_string();
    string currentPointy = bitset<6>(me->getY()).to_string();
    // TODO:ADD other things to broadcast if need

    if (me -> getType() == KARGAR)
        antType = "0";
    else
        antType = "1";

    /*
    // TODO: if not need delete this
    if (currentDir == "UP")
        whereGo = "00";
    else if (currentDir == "RIGHT")
        whereGo = "01";
    else if (currentDir == "DOWN")
        whereGo = "10";
    else
        whereGo = "11";
        */

    // fix oon two bit e moft :/
    message = offset + antType + whereGo + "00" + offset;

    for (int i = 0; i < 5; i++)
        message += currentPointx[i];

    message += offset + currentPointx[5];

    for (int i = 0; i < 4; i++)
        message += currentPointy[i];

    message += offset;
    message = message + currentPointy[4] + currentPointy[5];

    for(int i = 0; i < 3; i++)
        message += sendingContents[i];
    // ta in ja 4 byte 32 bit

    int iter = 3;
    for (int i = 3; i < sendingContents.size(); i++) {
        if (i == iter){
            message += offset;
            iter += 5;
        }
        message += sendingContents[i];
    }

    if (ImInAttack)
        isAttacked = "1";
    else
        isAttacked = "0";

    // our last bit of message
    message += isAttacked;
    //message += '0';
    // ta in ja 160 bits = 20 bytes :)

    unsigned char* encodedMessage = new unsigned char[message.size() / 8];
    for(int i = 0; i < message.size() / 8; i++)
        encodedMessage[i] = 0;

    int j = 0;
    int counter = 0;
    // or counter > 0 in while loop
    while (j < message.size())
    {
        string part;
        for (int i = j; i < j + 8; i++)
            part += message[i];

        for(int i = 0; i < 8; ++i)
            encodedMessage[counter] |= (part[i] == '1') << (7 - i);

        j += 8;
        counter++;
    }
    return encodedMessage;
}


void AI::decodeMessage(const Ant* me, const Game* game)
{
    // decoding message...
    const ChatBox* mes = game -> getChatBox();
    vector<const Chat*> chats;

    if (currentTurn == 1)
    {
        chats = mes->getAllChats();
        // texts [x, y, jahat]
        Texts.resize(chats.size());
        for (int i=0; i < chats.size(); ++i)
            Texts[i].resize(3);

        dContents.resize(chats.size());
        dAttack.resize(chats.size());
        if (chats.size() != 0)
            currentTurn = chats.back() -> getTurn() + 1;
    }
    else {
        chats = mes->getAllChatsOfTurn(currentTurn - 1);

        Texts.resize(chats.size());
        for (int i=0; i < chats.size(); ++i)
            Texts[i].resize(3);

        dContents.resize(chats.size());
        dAttack.resize(chats.size());
    }

    int iter = 0;
    for (const Chat* i: chats)
    {
        if (i -> getText() != "We R going to FUCK :)") {
            string receive = i -> getText();
            string currentPointX = "";
            string currentPointY = "";
            string decodedMessage = "";
            string content = "";
            string typeAnt;
            string whereGo = "";

            for (int i = 0; i < receive.size(); i++)
                decodedMessage += bitset<8>(int(receive[i])).to_string();

            whereGo = decodedMessage[4];
            whereGo += decodedMessage[5];
            typeAnt = decodedMessage[6];

            for (int i = 1 * 8 + 3; i < 2 * 8; i++)
                currentPointX += decodedMessage[i];
            currentPointX += decodedMessage[2 * 8 + 3];

            for (int i = 2 * 8 + 3 + 1; i < 3 * 8 ; i++)
                currentPointY += decodedMessage[i];
            currentPointY += decodedMessage[3 * 8 + 3];
            currentPointY += decodedMessage[3 * 8 + 3 + 1];

            for (int i = 3 * 8 + 3 + 2; i < 4 * 8; i++)
                content += decodedMessage[i];

            for (int i = 4 * 8; i < decodedMessage.size() - 1; i++)
            {
                if (i % 8 == 0)
                    i += 2;
                else
                    content += decodedMessage[i];
            }

            if (decodedMessage[decodedMessage.size() - 1] == '1')
                dAttack[iter] = true;
            else
                dAttack[iter] = false;

            int decCurrentPointX = bitset<6>(currentPointX).to_ulong();
            int decCurrentPointY = bitset<6>(currentPointY).to_ulong();

            Texts[iter][0] = decCurrentPointX;
            Texts[iter][1] = decCurrentPointY;

            if (whereGo == "00")
                Texts[iter][2] = 0;
            else if (whereGo == "01")
                Texts[iter][2] = 1;
            else if (whereGo == "10")
                Texts[iter][2] = 2;
            else
                Texts[iter][2] = 3;

            dContents[iter] = content;
            iter++;
        }
    }
}


void AI::receivePoints(const Ant* me, const Game* game)
{
    int viewDistance = me->getViewDistance();
    // decoding points...
    for (int j = 0; j < Texts.size(); j++){
        int x = Texts[j][0];
        int y = Texts[j][1];

        bool goadd = true;
        int row = 0;
        int col = -1*viewDistance;
        int counter = 0;

        if (dAttack[j])
            attackPoint = {x, y};

        for (int i = -1*row; i <= row; i++){
            int xx = (x + i)  %  game->getMapWidth();
            int yy = (y + col)  %  game->getMapHeight();

            if (xx < 0)
                xx += game->getMapWidth();
            if (yy < 0)
                yy += game->getMapHeight();

            // wtf problem (i think lower than 5 ants)
            if (dContents[j] == "")
                break;

            if (dContents[j][counter] == '0' && dContents[j][counter + 1] == '0')
                savedMap[xx][yy] = 0;

            else if (dContents[j][counter] == '0' && dContents[j][counter + 1] == '1')
                savedMap[xx][yy] = 1;

            else if (dContents[j][counter] == '1' && dContents[j][counter + 1] == '0')
                savedMap[xx][yy] = 2;

            else
                savedMap[xx][yy] = 3;

            counter += 2;
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
}


Answer* AI::turn(Game* game)
{
    const Ant* me = game->getAnt();
    pair<int, int> nextGoingPoints{-1, -1}; // (x, y)
    attackPoint = {-1, -1};

    string message = "";
    // 10: viewDistance, 20: attack, 30: food
    messageValue = 10;
    Direction direction;
    ++currentTurn;

    if (currentTurn == 1)
    {
        int width=game->getMapWidth();
        int height=game->getMapHeight();
        farthestPoint = {-1, -1};
        previousPoint = {me->getX(), me->getY()};
        // not need yet
        currentDir = "CENTER";
        ImInAttack = false;

        savedMap.resize(width);
        for (int i=0; i < width; ++i)
            savedMap[i].resize(height);

        // TODO: use fill
        for (int i=0; i < width; ++i)
            for (int j=0; j < height; ++j)
                savedMap[i][j] = -1;

        message = "We R going to FUCK :)";
    }

    // If all random points have seen, re-initialize them.
    if (randomAreas.size() == 0) {
        int width=game->getMapWidth();
        int height=game->getMapHeight();
        randomAreas.push_back( {width / 2, height / 2} );
        randomAreas.push_back( {width-1, 1} );
        randomAreas.push_back( {1, height-1} );
        randomAreas.push_back( {width-1, height-1} );
        randomAreas.push_back( {1, 1} );
        randomAreas.push_back( {width / 2, height / 2} );
    }

    saveMap(me);

    //receive ChatBox
    decodeMessage(me, game);
    receivePoints(me, game);

    // Atacks Part
    if (game->getAttacks().size() != 0)
    {
        for (const Attack* attack : game->getAttacks()) {
            pair<int, int> defender = {attack->getDefenderColumn(), attack->getDefenderRow()};
            if (getManhattan(defender, {me->getX(), me->getY()}) <= 1) {
                ImInAttack = true;
            }
            // Lock on target
            if (me->getType() == SARBAZ)
            {
                pair<int, int> attacker = {attack->getAttackerColumn(), attack->getAttackerRow()};

                if (getManhattan(attacker, {me->getX(), me->getY()}) <= 1) {
                    goingPath.clear();
                    nextGoingPoints = {attack->getDefenderColumn(), attack->getDefenderRow()};
                    goingPath = findPath(me, nextGoingPoints);
                }
            }
        }
    }

    if (me->getType() == AntType::KARGAR)
    {
        if (goingPath.size() == 0) {
            if (me->getCurrentResource()->getType() != ResourceType::NONE) {
                nextGoingPoints.first = game->getBaseX();
                nextGoingPoints.second = game->getBaseY();
            }
            else {
                // Search for food
            	goingPath = getResourcePath(me);
            }
        }
    }
    else {
        if (goingPath.size() == 0) {
            if (getManhattan(farthestPoint, {me->getX(), me->getY()}) <= me->getViewDistance()-1)
                farthestPoint = {-1, -1};

            if (farthestPoint.first == -1)
                farthestPoint = getRandomFarPoint(me, game->getMapWidth(), game->getMapHeight());
            
            // Go for helping to attacked ant
            if (attackPoint.first != -1 && getManhattan(attackPoint, {me->getX(), me->getY()}) <= 12) {
                goingPath = findPath(me, attackPoint);
            }
            else if (savedMap[farthestPoint.first][farthestPoint.second] != -1) {
                // If random point is founded by broadcast, go with BFS
                goingPath = findPath(me, farthestPoint);
                // To make sure our Sarbaz will move if something bad happened
                if (goingPath.size() == 0) nextGoingPoints = getFarthestInVD(me, farthestPoint);
            }
            else {
                nextGoingPoints = getFarthestInVD(me, farthestPoint);
            }   
        }
    }

    // Kargar go to random direction in first turn if there is no food in VD
    if (currentTurn == 1 && me->getType() == KARGAR && goingPath.size() == 0) {
        int randDir = rand() % 4;
        if (randDir == 0) direction = UP;
        else if (randDir == 1) direction = RIGHT;
        else if (randDir == 2) direction = LEFT;
        else if (randDir == 3) direction = DOWN;

        // not need yet
        if (direction == UP) nextDir = "UP";
        else if (direction == DOWN) nextDir = "DOWN";
        else if (direction == RIGHT) nextDir = "RIGHT";
        else nextDir = "LEFT";

        return new Answer(direction, message, messageValue);
    }

    // If didn't found any resource, going to some random points
    if (me -> getType() == KARGAR && goingPath.size() == 0 &&
        nextGoingPoints.first == -1)
        {
        	if (getManhattan(farthestPoint, {me->getX(), me->getY()}) <= me->getViewDistance())
                farthestPoint = {-1, -1};

        	if (farthestPoint.first == -1)
            	farthestPoint = getRandomFarPoint(me, game->getMapWidth(), game->getMapHeight());

        	// If random point is founded by broadcast, go with BFS
            if (savedMap[farthestPoint.first][farthestPoint.second] != -1) {
                goingPath = findPath(me, farthestPoint);
                // To make sure our Kargar will move if something bad happened
                if (goingPath.size() == 0) nextGoingPoints = getFarthestInVD(me, farthestPoint);
            }
            else {
                nextGoingPoints = getFarthestInVD(me, farthestPoint);
            } 
    	}

    if (goingPath.size() == 0)
        goingPath = findPath(me, nextGoingPoints);

    // TODO: if not used delete send dir in chat
    //currentDir = nextDir;

    // send ChatBox
    unsigned char* mess = new unsigned char[20];
    sendPoints(me);
    mess = encodeMessage(me);
    for (int i = 0; i < 20; i++)
        message += mess[i];

    direction = getDirection(me);

    if (ImInAttack && messageValue == 10)
        messageValue = 20;
    /*
    if (direction == UP)
        nextDir = "UP";
    else if (direction == DOWN)
        nextDir = "DOWN";
    else if (direction == RIGHT)
        nextDir = "RIGHT";
    else
        nextDir = "LEFT";

        */
    previousPoint = {me->getX(), me->getY()};
    return new Answer(direction, message, messageValue);
}


AI::AI() {
    srand(time(nullptr));
}
