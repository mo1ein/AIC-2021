#ifndef AIC21_CLIENT_CPP_ANT_H
#define AIC21_CLIENT_CPP_ANT_H

class Map;

#include <vector>
#include "Models/enums.h"
#include "Models/Resource.h"
#include "Models/Cell.h"
#include "Models/Map.h"
#include "Core/Message/Parse/CurrentStateMessage.h"

class Ant {

public:
    Ant(AntType type, AntTeam team, int x, int y);
    Ant(AntType type, AntTeam team, int viewDistance, int attackDistance, Map &map, Resource *resource, int x, int y, int health);
    ~Ant();
    const Cell* getNeighborCell(int xStep, int yStep) const;
    int getX() const;
    int getY() const;
    const Cell* getLocationCell() const;
    AntType getType() const;
    AntTeam getTeam() const;
    const Resource* getCurrentResource() const;
    int getHealth() const;
    int getViewDistance() const;
    int getAttackDistance() const;

private:
    AntType type_;
    AntTeam team_;
    Resource* current_resource_;
    int x_;
    int y_;
    int health_;
    Map* visible_map_;
    int view_distance_;
    int attack_distance_;
};

#endif //AIC21_CLIENT_CPP_ANT_H