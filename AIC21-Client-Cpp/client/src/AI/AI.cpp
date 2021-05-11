#include "AI.h"
#include <iostream>
#include "Models/enums.h"
#include <unordered_map>
#include <queue>
#include <time.h>
#include <string>
#include <bitset>

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
            pair<int, int> neighbour{(node.first+direction.first) % width,
                                     (node.second+direction.second) % height};
            if (neighbour.first < 0)
                neighbour.first += width;

            if (neighbour.second < 0)
                neighbour.second += height;

            if (savedMap[neighbour.first][neighbour.second] == -1 ||
                savedMap[neighbour.first][neighbour.second] == 0 ||
                savedMap[neighbour.first][neighbour.second] == 5 ||
                savedMap[neighbour.first][neighbour.second] == 4)
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


vector<pair<int, int>> AI::findFirstUnfound(const Ant* me)
{
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

        if (savedMap[node.first][node.second] == -1)
        {
            while (node != start_node) {
                path.push_back(node);
                node = visited[node];
            }
            // Return the path to destination in reversed format
            return path;
        }

        for (pair<int, int> direction : directions) {
            pair<int, int> neighbour{(node.first+direction.first) % width,
                                     (node.second+direction.second) % height};
            if (neighbour.first < 0)
                neighbour.first += width;

            if (neighbour.second < 0)
                neighbour.second += height;

            if (savedMap[neighbour.first][neighbour.second] == 0 ||
                savedMap[neighbour.first][neighbour.second] == 5)
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

vector<pair<int, int>> AI::findFirstUnfoundSafe(const Ant* me)
{
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

        if (savedMap[node.first][node.second] == -1)
        {
            while (node != start_node) {
                path.push_back(node);
                node = visited[node];
            }
            // Return the path to destination in reversed format
            return path;
        }

        for (pair<int, int> direction : directions) {
            pair<int, int> neighbour{(node.first+direction.first) % width,
                                     (node.second+direction.second) % height};
            if (neighbour.first < 0)
                neighbour.first += width;

            if (neighbour.second < 0)
                neighbour.second += height;

            if (savedMap[neighbour.first][neighbour.second] == 0)
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


int getManhattan(pair<int, int> a, pair<int, int> b) {
    return (abs(a.first-b.first) + abs(a.second-b.second));
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

        if (getManhattan(latestPath, {x, y}) == 1) {
            if (latestPath.second < y)
                return UP;

            if (latestPath.second > y)
                return DOWN;

            if (latestPath.first < x)
                return LEFT;

            if (latestPath.first > x)
                return RIGHT;
        }
        else {
            if (latestPath.second < y)
                return DOWN;

            if (latestPath.second > y)
                return UP;

            if (latestPath.first < x)
                return RIGHT;

            if (latestPath.first > x)
                return LEFT;
        }
    }
    // If code reaches here, try to find a free path that is not point and also forget about going path
    farthestPoint = {-1, -1};
    const Cell* cell = me->getNeighborCell(0, -1);
    pair<int, int> nextPoint = {cell->getX(), cell->getY()};
    if (cell->getType() != WALL && nextPoint != previousPoint) {
        if (getManhattan(nextPoint, {x, y}) == 1)
            return UP;
        else
            return DOWN;
    }

    cell = me->getNeighborCell(1, 0);
    nextPoint = {cell->getX(), cell->getY()};
    if (cell->getType() != WALL && nextPoint != previousPoint) {
        if (getManhattan(nextPoint, {x, y}) == 1)
            return RIGHT;
        else
            return LEFT;
    }

    cell = me->getNeighborCell(0, 1);
    nextPoint = {cell->getX(), cell->getY()};
    if (cell->getType() != WALL && nextPoint != previousPoint) {
        if (getManhattan(nextPoint, {x, y}) == 1)
            return DOWN;
        else
            return UP;
    }
    cell = me->getNeighborCell(-1, 0);
    nextPoint = {cell->getX(), cell->getY()};
    if (cell->getType() != WALL && nextPoint != previousPoint) {
        if (getManhattan(nextPoint, {x, y}) == 1)
            return LEFT;
        else
            return RIGHT;
    }

    // If code reaches here, there is no free path so should return to previous point
    if (previousPoint.second < y) {
        if (getManhattan(nextPoint, {x, y}) == 1)
            return UP;
        else
            return DOWN;
    }

    if (previousPoint.second > y) {
        if (getManhattan(nextPoint, {x, y}) == 1)
            return DOWN;
        else
            return UP;
    }

    if (previousPoint.first < x) {
        if (getManhattan(nextPoint, {x, y}) == 1)
            return LEFT;
        else
            return RIGHT;
    }

    if (previousPoint.first > x) {
        if (getManhattan(nextPoint, {x, y}) == 1)
            return RIGHT;
        else
            return LEFT;
    }
}


vector<pair<int, int>> AI::findPath(const Ant* me, pair<int, int> dest)
{
    pair<int, int> node;
    pair<int, int> start_node{me->getX(), me->getY()};
    queue<pair<int, int>> neighbors;
    vector<pair<int, int>> path;
    unordered_map<pair<int, int>, pair<int, int>, hash_pair> visited;

    neighbors.push(start_node);

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
            pair<int, int> neighbour{(node.first+direction.first) % width,
                                     (node.second+direction.second) % height};
            if (neighbour.first < 0)
                neighbour.first += width;

            if (neighbour.second < 0)
                neighbour.second += height;

            if (me->getType() == KARGAR && me->getCurrentResource()->getType() != ResourceType::NONE)
            {
                if (savedMap[neighbour.first][neighbour.second] == -1 ||
                    savedMap[neighbour.first][neighbour.second] == 0 ||
                    savedMap[neighbour.first][neighbour.second] == 4)
                {
                    continue;
                }
            }
            else {
                if (savedMap[neighbour.first][neighbour.second] == -1 ||
                    savedMap[neighbour.first][neighbour.second] == 0)
                {
                    continue;
                }
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

vector<pair<int, int>> AI::findPathWithoutTraps(const Ant* me, pair<int, int> dest)
{
    pair<int, int> node;
    pair<int, int> start_node{me->getX(), me->getY()};
    queue<pair<int, int>> neighbors;
    vector<pair<int, int>> path;
    unordered_map<pair<int, int>, pair<int, int>, hash_pair> visited;

    neighbors.push(start_node);

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
            pair<int, int> neighbour{(node.first+direction.first) % width,
                                     (node.second+direction.second) % height};
            if (neighbour.first < 0)
                neighbour.first += width;

            if (neighbour.second < 0)
                neighbour.second += height;

            if (savedMap[neighbour.first][neighbour.second] == -1 ||
                savedMap[neighbour.first][neighbour.second] == 0 ||
                savedMap[neighbour.first][neighbour.second] == 5 ||
                savedMap[neighbour.first][neighbour.second] == 4)
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

vector<pair<int, int>> AI::findPathSafe(const Ant* me, pair<int, int> dest)
{
    pair<int, int> node;
    pair<int, int> start_node{me->getX(), me->getY()};
    queue<pair<int, int>> neighbors;
    vector<pair<int, int>> path;
    unordered_map<pair<int, int>, pair<int, int>, hash_pair> visited;

    neighbors.push(start_node);

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
            pair<int, int> neighbour{(node.first+direction.first) % width,
                                     (node.second+direction.second) % height};
            if (neighbour.first < 0)
                neighbour.first += width;

            if (neighbour.second < 0)
                neighbour.second += height;

            if (savedMap[neighbour.first][neighbour.second] == -1 ||
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


// Return farthest point on map for Sarbaz to go
pair<int, int> AI::findFarthestPointOnMap(const Ant* me, int width, int height) {
    int halfWidth=width/2;
    int halfHeight=height/2;
    int x=me->getX();
    int y=me->getY();

    if (x >= halfWidth && y >= halfHeight)
        return {1, 1};

    if (x <= halfWidth && y >= halfHeight)
        return {width-1, 1};

    if (x <= halfWidth && y <= halfHeight)
        return {width-1, height-1};

    if (x >= halfWidth && y <= halfHeight)
        return {1, height-1};

    return {halfWidth, halfHeight};
}


// Return farthest cell in view distance which is nearest to destination
pair<int, int> AI::getFarthestInVD(const Ant* me, pair<int, int> dest)
{
    int x = me->getX();
    int y = me->getY();
    int viewDistance = me->getViewDistance();
    const Cell* cell;
    vector<pair<int, int>> points;
    vector<int> distances;

    // Top-Left and Down-Right Diagonals
    int j=0;
    for (int i=-1*viewDistance; i <= 0; ++i)
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
        --j;
    }

    // Top-Right and Down-Left Diagonals
    j = 1;
    for (int i = (-1*viewDistance) + 1; i < 0; ++i)
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

    if (points.size() == 0)
        return {-1, -1};

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
    sendingContents = "";
    int viewDistance = me->getViewDistance();
    bool goadd = true;
    int row = 0;
    int col = -1*viewDistance;
    for (int i = -1*row; i <= row; i++)
    {
        const Cell* cell = me->getNeighborCell(i, col);
        if (cell->getType() == WALL)
        {
            savedMap[cell->getX()][cell->getY()] = 0;
            sendingContents += "000";
        }

        else if (cell->getType() == TRAP)
        {
            savedMap[cell->getX()][cell->getY()] = 4;
            sendingContents += "100";
        }

        else if (cell->getType() == SWAMP)
        {
            savedMap[cell->getX()][cell->getY()] = 5;
            sendingContents += "101";
        }

        else if (cell -> getResource() -> getType() == BREAD)
        {
            if (savedMap[cell->getX()][cell->getY()] == -1 && messageValue < 30)
                messageValue = 30;

            savedMap[cell->getX()][cell->getY()] = 2;
            sendingContents += "010";
        }

        else if (cell -> getResource() -> getType() == GRASS)
        {
            if (savedMap[cell->getX()][cell->getY()] == -1 && messageValue < 30)
                messageValue = 30;

            savedMap[cell->getX()][cell->getY()] = 3;
            sendingContents += "011";
        }

        else {
            savedMap[cell->getX()][cell->getY()] = 1;
            sendingContents += "001";
        }

        if (cell->getType() == BASE &&
            cell->getX() != ourBase.first &&
            cell->getY() != ourBase.second)
        {
            messageValue = 70;
            enemyPoint = {cell->getX(), cell->getY()};
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
    string offset = "001"; // for printable chars
    string antType;
    string isAttacked;
    string enemyBaseX = "";
    string enemyBaseY = "";
    string currentPointx = bitset<6>(me->getX()).to_string();
    string currentPointy = bitset<6>(me->getY()).to_string();

    if (me -> getType() == KARGAR)
        antType = "0";
    else
        antType = "1";

    if (ImInAttack)
        isAttacked = "1";
    else
        isAttacked = "0";

    // 3 bits for fill byte
    message = offset + "000" + antType + isAttacked + offset;

    // encoding current x point of ant
    for (int i = 0; i < 5; i++)
        message += currentPointx[i];

    message += offset + currentPointx[5];

    // encoding current y point of ant
    for (int i = 0; i < 4; i++)
        message += currentPointy[i];

    // offset for start of a new byte
    message += offset;

    // join bytes together
    message = message + currentPointy[4] + currentPointy[5];

    for(int i = 0; i < 3; i++)
        message += sendingContents[i];

    // === used 32 bit = 4 byte until here ===

    // joining sendingContents with other parts
    int iter = 3;
    for (int i = 3; i < sendingContents.size(); i++) {
        if (i == iter){
            message += offset;
            iter += 5;
        }
        message += sendingContents[i];
    }

    // === used 224 bit = 4 byte until here ===

    // encoding Enemy point
    if (enemyPoint.first != -1)
    {
        enemyBaseX = bitset<6>(enemyPoint.first).to_string();
        enemyBaseY = bitset<6>(enemyPoint.second).to_string();

        // encoding enemy x point
        message += offset;
        for (int i = 0; i < enemyBaseX.size() - 1; i++)
            message += enemyBaseX[i];
        message += offset;
        message += enemyBaseX[5];

        // encoding enemy y point
        for (int i = 0; i < enemyBaseY.size() - 2; i++)
            message += enemyBaseY[i];
        message += offset;
        message += enemyBaseY[4];
        message += enemyBaseY[5];

        // just for fill byte
        message += "000";
    }

    // fill char with zeros
    unsigned char* encodedMessage = new unsigned char[message.size() / 8];
    for(int i = 0; i < message.size() / 8; i++)
        encodedMessage[i] = 0;

    // fill chars with binary values
    int j = 0;
    int counter = 0;
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
        // texts [x, y]
        Texts.resize(chats.size());
        for (int i=0; i < chats.size(); ++i)
            Texts[i].resize(2);

        dContents.resize(chats.size());
        dAttack.resize(chats.size());
        if (chats.size() != 0)
            currentTurn = chats.back() -> getTurn() + 1;
    }
    else {
        chats = mes->getAllChatsOfTurn(currentTurn - 1);

        Texts.resize(chats.size());
        for (int i=0; i < chats.size(); ++i)
            Texts[i].resize(2);

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
            string enemyBaseX = "";
            string enemyBaseY = "";
            string content = "";
            string typeAnt;

            for (int i = 0; i < receive.size(); i++)
                decodedMessage += bitset<8>(int(receive[i])).to_string();

            typeAnt = decodedMessage[6];

            // isAttacked? for help
            if (decodedMessage[7] == '1')
                dAttack[iter] = true;
            else
                dAttack[iter] = false;

            // decoding current x and y points of ant
            for (int i = 1 * 8 + 3; i < 2 * 8; i++)
                currentPointX += decodedMessage[i];
            currentPointX += decodedMessage[2 * 8 + 3];

            for (int i = 2 * 8 + 3 + 1; i < 3 * 8 ; i++)
                currentPointY += decodedMessage[i];
            currentPointY += decodedMessage[3 * 8 + 3];
            currentPointY += decodedMessage[3 * 8 + 3 + 1];

            for (int i = 3 * 8 + 3 + 2; i < 4 * 8; i++)
                content += decodedMessage[i];

            int endOfPoints;
            if (receive.size() == 28)
                endOfPoints = decodedMessage.size();
            else
                endOfPoints = decodedMessage.size() - (3 * 8);

            // decoding content of points
            for (int i = 4 * 8; i < endOfPoints; i++)
            {
                if (i % 8 == 0)
                    i += 2;
                else
                    content += decodedMessage[i];
            }

            // decode enemybase if founded
            if (receive.size() == 31)
            {
                for (int i = 227; i < 232; i++)
                    enemyBaseX += decodedMessage[i];
                enemyBaseX += decodedMessage[235];

                for (int i = 236; i < 240; i++)
                    enemyBaseY += decodedMessage[i];
                enemyBaseY += decodedMessage[243];
                enemyBaseY += decodedMessage[244];

                enemyPoint = {bitset<6>(enemyBaseX).to_ulong(), bitset<6>(enemyBaseY).to_ulong()};
            }

            int decCurrentPointX = bitset<6>(currentPointX).to_ulong();
            int decCurrentPointY = bitset<6>(currentPointY).to_ulong();

            Texts[iter][0] = decCurrentPointX;
            Texts[iter][1] = decCurrentPointY;

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
            int xx = (x + i)  %  width;
            int yy = (y + col)  %  height;

            if (xx < 0)
                xx += width;
            if (yy < 0)
                yy += height;

            // wtf problem (i think lower than 5 ants)
            if (dContents[j] == "")
                break;

            // BLOCK
            if (dContents[j][counter] == '0' &&
                dContents[j][counter + 1] == '0' &&
                dContents[j][counter + 2] == '0'
                ) savedMap[xx][yy] = 0;

            // TRAP
            else if (dContents[j][counter] == '1' &&
                dContents[j][counter + 1] == '0' &&
                dContents[j][counter + 2] == '0'
                ) savedMap[xx][yy] = 4;

            // SWAMP
            else if (dContents[j][counter] == '1' &&
                dContents[j][counter + 1] == '0' &&
                dContents[j][counter + 2] == '1'
                ) savedMap[xx][yy] = 5;

            // EMPTY
            else if (dContents[j][counter] == '0' &&
                dContents[j][counter + 1] == '0' &&
                dContents[j][counter + 2] == '1'
                ) savedMap[xx][yy] = 1;

            // BREAD
            else if (dContents[j][counter] == '0' &&
                dContents[j][counter + 1] == '1' &&
                dContents[j][counter + 2] == '0'
                ) savedMap[xx][yy] = 2;

            // GRASS
            else if (dContents[j][counter] == '0' &&
                dContents[j][counter + 1] == '1' &&
                dContents[j][counter + 2] == '1'
                ) savedMap[xx][yy] = 3;

            counter += 3;
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
    ImInAttack = false;
    string message = "";

    /* messageValues:
        10: Kargar viewDistance,
        15: Sarbaz viewDistance,
        20: attack,
        30: food,
        70: found enemy base
    */

    if (me->getType() == SARBAZ)
        messageValue = 15;
    else
        messageValue = 10;

    Direction direction;
    ++currentTurn;

    if (currentTurn == 1)
    {
        width=game->getMapWidth();
        height=game->getMapHeight();
        previousPoint = {me->getX(), me->getY()};
        farthestPoint = {-1, -1};
        ourBase = {me->getX(), me->getY()};
        foundBase = false;
        goingHome = false;

	    // means not found Enemy base yet
        enemyPoint = {-1, -1};

        if (height > width)
            maxHeightOrWidth = height;
        else
            maxHeightOrWidth = width;

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
        randomAreas.push_back( {width / 2, height / 2} );
        randomAreas.push_back( {width-1, 1} );
        randomAreas.push_back( {1, height-1} );
        randomAreas.push_back( {width-1, height-1} );
        randomAreas.push_back( {1, 1} );
        randomAreas.push_back( {width / 2, height / 2} );
        shuffle(randomAreas.begin(), randomAreas.end(), rng);
    }

    saveMap(me);

    //receive ChatBox
    decodeMessage(me, game);
    receivePoints(me, game);

    // If Sarbaz is born new, wait for 2 Sarbaz then move
    if (me->getType() == SARBAZ && currentTurn <= 100 && me->getX() == game->getBaseX() &&
        me->getY() == game->getBaseY())
    {
        int totalSoldiers=0;
        for (const Ant* i : me->getLocationCell()->getPresentAnts())
            if (i->getType() == SARBAZ)
                ++totalSoldiers;
        for (const Ant* i : me->getNeighborCell(1, 0)->getPresentAnts())
            if (i->getType() == SARBAZ)
                ++totalSoldiers;
        for (const Ant* i : me->getNeighborCell(0, 1)->getPresentAnts())
            if (i->getType() == SARBAZ)
                ++totalSoldiers;
        for (const Ant* i : me->getNeighborCell(-1, 0)->getPresentAnts())
            if (i->getType() == SARBAZ)
                ++totalSoldiers;
        for (const Ant* i : me->getNeighborCell(0, -1)->getPresentAnts())
            if (i->getType() == SARBAZ)
                ++totalSoldiers;

        if (totalSoldiers <= 1)
            return new Answer(CENTER);
    }

    //If stuck, reset going path
    pair<int ,int> currentPoint = {me->getX(), me->getY()};
    if (currentPoint == previousPoint) {
        goingPath.clear();
        goingHome = false;
    }

    // If Kargar bring food to our base, mark as ready for searching new food
    if (me->getType() == KARGAR && currentPoint == ourBase)
        goingHome = false;

    // if found enemyBase Go fuck base
    if (me->getType() == SARBAZ &&
        enemyPoint.first != -1 &&
        enemyPoint.second != -1 &&
        !foundBase)
    {
        foundBase = true;
        goingPath.clear();
        nextGoingPoints = {enemyPoint.first, enemyPoint.second};
        goingPath = findPath(me, nextGoingPoints);
    }
    // Atacks Part
    else if (game->getAttacks().size() != 0)
    {
        for (const Attack* attack : game->getAttacks()) {
            pair<int, int> defender = {attack->getDefenderColumn(), attack->getDefenderRow()};
            if (getManhattan(defender, {me->getX(), me->getY()}) <= 1) {
                ImInAttack = true;
            }
            // Lock on target
            if (me->getType() == SARBAZ && enemyPoint.first == -1)
            {
                pair<int, int> attacker = {attack->getAttackerColumn(), attack->getAttackerRow()};

                if (getManhattan(attacker, {me->getX(), me->getY()}) <= 1) {
                    goingPath.clear();
                    nextGoingPoints = {attack->getDefenderColumn(), attack->getDefenderRow()};
                    goingPath = findPath(me, nextGoingPoints);
                    // If we attack the base, then base will be Defender and we will be Attacker

                    if (getManhattan(defender, {me->getX(), me->getY()}) > me->getAttackDistance())
                        break;
                }
            }
        }
    }

    // If unfounded point by Sarbaz is a block, remove it
    if (me->getType() == SARBAZ && goingPath.size() == 1)
        if (savedMap[goingPath[0].first][goingPath[0].second] == 0)
            goingPath.clear();


    if (me->getType() == AntType::KARGAR)
    {
        if ((goingPath.size() == 0 || currentTurn % 2 == 0) && !goingHome) {
            if (me->getCurrentResource()->getType() != ResourceType::NONE) {
                goingPath.clear();
                nextGoingPoints.first = game->getBaseX();
                nextGoingPoints.second = game->getBaseY();
                goingPath = findPathWithoutTraps(me, nextGoingPoints);
                goingHome = true;
            }
            else {
                // Search for food
            	goingPath = getResourcePath(me);
            }
        }
    }
    else {
        if ((goingPath.size() == 0 || attackPoint.first != -1) && !foundBase) {
 	    // Go for helping to attacked ant
            int distance = getManhattan(attackPoint, {me->getX(), me->getY()});
            if (attackPoint.first != -1 && distance <= maxHeightOrWidth/2 && distance >= 2) {
                goingPath.clear();
                goingPath = findPath(me, attackPoint);
            }
            else {
                // Go to first -1 point on map
                goingPath = findFirstUnfound(me);
                // If didn't find, then path has swamps in it
                if (goingPath.size() == 0) findFirstUnfoundSafe(me);
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
        goingPath = findPathWithoutTraps(me, nextGoingPoints);
    // If prevoius call didn't find a way, way has Swamps and Traps in it
    if (goingPath.size() == 0)
        goingPath = findPath(me, nextGoingPoints);
    // If prevoius call didn't find a way, way has Traps in it
    if (goingPath.size() == 0)
        goingPath = findPathSafe(me, nextGoingPoints);

    // send ChatBox
    if (enemyPoint.first == -1)
    {
        unsigned char* mess = new unsigned char[28];
        mess = encodeMessage(me);
        for (int i = 0; i < 28; i++)
            message += mess[i];
    }
    else {
        unsigned char* mess = new unsigned char[31];
        mess = encodeMessage(me);
        for (int i = 0; i < 31; i++)
            message += mess[i];
    }

    direction = getDirection(me);

    if (ImInAttack && messageValue < 30)
        messageValue = 20;

    previousPoint = {me->getX(), me->getY()};
    return new Answer(direction, message, messageValue);
}


AI::AI() {
    srand(time(nullptr));
    rng.seed(time(nullptr));
}
