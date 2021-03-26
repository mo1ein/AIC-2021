#ifndef AIC21_CLIENT_CPP_ANT_DTO_H
#define AIC21_CLIENT_CPP_ANT_DTO_H

#include "Models/enums.h"

class AntDTO {

public:
    AntDTO(AntTeam antTeam, AntType antType);
    ~AntDTO() = default;
    AntTeam getAntTeam();
    AntType getAntType();
    

private:
    AntTeam ant_team_;
    AntType ant_type_;
};


#endif // AIC21_CLIENT_CPP_ANT_DTO_H