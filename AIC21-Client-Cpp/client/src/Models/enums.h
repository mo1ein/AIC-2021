#ifndef AIC21_CLIENT_CPP_ENUMS_H
#define AIC21_CLIENT_CPP_ENUMS_H

#include <string>
#include <Core/Message/Parse/ParseError.h>

enum AntTeam {
    ALLY,
    ENEMY
};

enum AntType {
    SARBAZ,
    KARGAR
};

enum CellType {
    BASE,
    EMPTY,
    WALL,
    TRAP,
    SWAMP
};

enum ResourceType {
    BREAD,
    GRASS,
    NONE
};

enum Direction {
    CENTER,
    RIGHT,
    UP,
    LEFT,
    DOWN
};

class EnumUtils {

public:
    static CellType getCellTypeByInt(int cellType);
    static ResourceType getResourceTypeByInt(int resourceType);
    static AntType getAntTypeByInt(int antType);
    static AntTeam getAntTeamByInt(int antTeam);
};


#endif //AIC21_CLIENT_CPP_ENUMS_H
