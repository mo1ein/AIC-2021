#include "Cell.h"


Cell::Cell(CellType type, int x, int y, Resource* resource) {
    x_ = x;
    y_ = y;
    type_ = type;
    resource_ = resource;
}

Cell::Cell(const Cell& obj) {
    x_ = obj.x_;
    y_ = obj.y_;
    type_ = obj.type_;
    resource_ = obj.resource_;
    present_ants_ = obj.present_ants_;
}

Cell::~Cell() {
    delete resource_;
    for (const Ant* ant : present_ants_) {
        delete ant;
    }
}

int Cell::getX() {
    return x_;
}

int Cell::getY() {
    return y_;
}

CellType Cell::getType() {
    return type_;
}

Resource* Cell::getResource() {
    return resource_;
}

vector<const Ant*> Cell::getPresentAnts() {
    return present_ants_;
}

void Cell::addAntToCell(const Ant *ant) {
    present_ants_.push_back(ant);
}
