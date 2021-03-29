#include "Ant.h"
#include "enums.h"

Ant::Ant(AntType type, AntTeam team, int x, int y)
{
    type_ = type;
    team_ = team;
    current_resource_ = nullptr;
    visible_map_ = nullptr;
    x_ = x;
    y_ = y;
    health_ = -1;
    view_distance_ = -1;
}

Ant::Ant(AntType type, AntTeam team, int viewDistance, Map &map, Resource* resource, int x, int y, int health)
{
    type_ = type;
    team_ = team;
    current_resource_ = resource;
    x_ = x;
    y_ = y;
    health_ = health;
    visible_map_ = &map;
    view_distance_ = viewDistance;
}

Ant::~Ant() {
    delete visible_map_;
    delete current_resource_;
}

int Ant::getX()
{
    return x_;
}

int Ant::getY()
{
    return y_;
}

int Ant::getHealth()
{
    return health_;
}

int Ant::getViewDistance()
{
    return view_distance_;
}

Cell *Ant::getNeighborCell(int xStep, int yStep)
{
    return visible_map_->getCell(xStep, yStep);
}

AntType Ant::getType()
{
    return type_;
}

AntTeam Ant::getTeam()
{
    return team_;
}

Resource *Ant::getCurrentResource()
{
    return current_resource_;
}

Cell* Ant::getLocationCell() {
    return getNeighborCell(0, 0);
}
