#include "Map.h"

Map::Map(const vector<vector<Cell*>>& cells, int width, int height, int manhattanDistance, int currentX, int currentY) {
    width_ = width;
    height_ = height;
    manhattan_distance_ = manhattanDistance;
    cells_ = cells;
    ant_x_ = currentX;
    ant_y_ = currentY;
}

const Cell* Map::getRelativeCell(int dx, int dy) const {
    int x = (ant_x_ + dx) % width_;
    int y = (ant_y_ + dy) % height_;
    if (x < 0)
        x += width_;
    if (y < 0)
        y += height_;
    return cells_[x][y];
}
