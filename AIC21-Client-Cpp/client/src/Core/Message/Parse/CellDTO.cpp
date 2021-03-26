#include "CellDTO.h"
    
CellDTO::CellDTO(int x, int y, CellType cellType, int resourceValue, ResourceType resourceType) {
    cell_x_ = x;
    cell_y_ = y;
    cell_type_ = cellType;
    resource_ = Resource(resourceType, resourceValue);
}

int CellDTO::getX() {
    return cell_x_;
}

int CellDTO::getY() {
    return cell_y_;
}

CellType CellDTO::getCellType() {
    return cell_type_;
}

Resource CellDTO::getResource() {
    return resource_;
}

vector<AntDTO> CellDTO::getAnts() {
    return ants_;
}

void CellDTO::addAnt(const AntDTO &ant) {
    ants_.push_back(ant);
}