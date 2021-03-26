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
    int getX();
    int getY();
    CellType getType();
    Resource* getResource();
    vector<const Ant*> getPresentAnts();
    void addAntToCell(const Ant *ant);

private:
    int x_;
    int y_;
    CellType type_;
    Resource* resource_;
    vector<const Ant*> present_ants_;
};


#endif //AIC21_CLIENT_CPP_CELL_H
