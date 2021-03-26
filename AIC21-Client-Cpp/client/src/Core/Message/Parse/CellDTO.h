#ifndef AIC21_CLIENT_CPP_CELL_DTO_H
#define AIC21_CLIENT_CPP_CELL_DTO_H

#include <vector>
#include "Core/Message/Parse/AntDTO.h"
#include "Models/enums.h"
#include "Models/Resource.h"
using namespace std;


class CellDTO {

    private:

    int cell_x_;
    int cell_y_;
    Resource resource_;
    CellType cell_type_;
    vector<AntDTO> ants_;


    public:

    CellDTO(int x, int y, CellType cellType, int resourceValue, ResourceType resourceType);
    ~CellDTO() = default;
    int getX();
    int getY();
    CellType getCellType();
    Resource getResource();
    vector<AntDTO> getAnts();
    void addAnt(const AntDTO &ant);

};


#endif // AIC21_CLIENT_CPP_CELL_DTO_H