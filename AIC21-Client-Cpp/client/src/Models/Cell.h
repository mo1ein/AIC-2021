#ifndef AIC21_CLIENT_CPP_CELL_H
#define AIC21_CLIENT_CPP_CELL_H

class Ant;

#include <vector>
#include "Models/enums.h"
#include "Models/Resource.h"
#include "Models/Ant.h"
using namespace std;

class Cell {

public:
    Cell(CellType type, int x, int y, Resource* resource);
    Cell(const Cell&);  //Todo why is this needed?
    ~Cell();
    int getX() const;
    int getY() const;
    CellType getType() const;
    const Resource* getResource() const;
    const vector<const Ant*>& getPresentAnts() const;

private:
    int x_;
    int y_;
    CellType type_;
    Resource* resource_;
    vector<const Ant*> present_ants_;
    void addAntToCell(const Ant *ant);

    friend class Game;
};


#endif //AIC21_CLIENT_CPP_CELL_H
