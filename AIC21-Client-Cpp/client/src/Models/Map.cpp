#include "Map.h"

Map::Map(const vector<vector<Cell*>>& cells, int width, int height, int manhattanDistance, int currentX, int currentY) {
    width_ = width;
    height_ = height;
    manhattan_distance_ = manhattanDistance;
    cells_ = createCompressedCells(cells, currentX, currentY);
}

Cell* Map::getCell(int dx, int dy, int distance) {
    if (abs(dx) + abs(dy) > distance)
        return nullptr;
    if ((distance + dx >= width_) || (distance + dx < 0))
        return nullptr;
    if ((distance + dy < 0) || (distance + dy >= height_))
        return nullptr;
    return cells_[distance + dy][distance + dx];
}

vector<vector<Cell*>> Map::createCompressedCells(const vector<vector<Cell*>>& cells, int midX, int midY) {
    vector<vector<Cell*>> compressedCells( 2 * manhattan_distance_ + 1 , vector<Cell*> (2 * manhattan_distance_ + 1));
    int startI = max(midX - manhattan_distance_, 0), endI = min(midX + manhattan_distance_ + 1, width_);
    int startJ = max(midY - manhattan_distance_, 0), endJ = min(midY + manhattan_distance_ + 1, height_);

    for (int i = startI; i < endI; i++) {
        for (int j = startJ; j < endJ; j++) {
            compressedCells[i - startI][j - startJ] = cells[i][j];
        }
    }

    return compressedCells;
}