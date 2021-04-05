#include "Ant.h"
#include "enums.h"

Ant::Ant(AntType type, AntTeam team, int x, int y) {
    type_ = type;
    team_ = team;
    current_resource_ = nullptr;
    visible_map_ = nullptr;
    x_ = x;
    y_ = y;
    health_ = -1;
    view_distance_ = -1;
    attack_distance_ = -1;
}

Ant::Ant(AntType type, AntTeam team, int viewDistance, int attackDistance, Map &map, Resource *resource, int x, int y, int health) {
    type_ = type;
    team_ = team;
    current_resource_ = resource;
    x_ = x;
    y_ = y;
    health_ = health;
    visible_map_ = &map;
    view_distance_ = viewDistance;
    attack_distance_ = attackDistance;
}

Ant::~Ant() {
    delete visible_map_;
    delete current_resource_;
}

int Ant::getX() const {
    return x_;
}

int Ant::getY() const {
    return y_;
}

int Ant::getHealth() const {
    return health_;
}

int Ant::getViewDistance() const {
    return view_distance_;
}

const Cell *Ant::getNeighborCell(int xStep, int yStep) const {
    return visible_map_->getRelativeCell(xStep, yStep);
}

AntType Ant::getType() const {
    return type_;
}

AntTeam Ant::getTeam() const {
    return team_;
}

const Resource *Ant::getCurrentResource() const {
    return current_resource_;
}

const Cell *Ant::getLocationCell() const {
    return getNeighborCell(0, 0);
}

int Ant::getAttackDistance() const {
    return attack_distance_;
}
