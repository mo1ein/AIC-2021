#include "AntDTO.h"

AntDTO::AntDTO(AntTeam antTeam, AntType antType) : ant_team_(antTeam), ant_type_(antType) {

}

AntTeam AntDTO::getAntTeam() {
    return ant_team_;
}

AntType AntDTO::getAntType() {
    return ant_type_;
}
