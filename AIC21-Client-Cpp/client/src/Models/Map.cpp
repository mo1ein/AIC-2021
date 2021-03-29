#include "Map.h"

Map::Map(const vector<vector<Cell*>>& cells, int width, int height, int manhattanDistance, int currentX, int currentY) {
    width_ = width;
    height_ = height;
    manhattan_distance_ = manhattanDistance;
    cells_ = cells;
    ant_x_ = currentX;
    ant_y_ = currentY;
}

Cell* Map::getCell(int dx, int dy) {
    int x = ant_x_ + dx;
    int y = ant_y_ + dy;
    if (x >= width_ || x < 0 || y >= height_ || y < 0)
        return nullptr;
    return cells_[x][y];
}
