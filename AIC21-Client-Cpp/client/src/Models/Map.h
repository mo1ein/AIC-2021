#ifndef AIC21_CLIENT_CPP_MAP_H
#define AIC21_CLIENT_CPP_MAP_H

class Cell;

#include "Models/Cell.h"
#include <vector>
using namespace std;


class Map {

public:
    Map(const vector<vector<Cell*>>& cells, int width, int height, int manhattanDistance, int currentX, int currentY);
    const Cell* getRelativeCell(int dx, int dy) const;

private:
    vector<vector<Cell*>> cells_;   //Todo vector<vector*>* ?
    int width_;
    int height_;
    int manhattan_distance_;
    int ant_x_;
    int ant_y_;
};

#endif //AIC21_CLIENT_CPP_MAP_H