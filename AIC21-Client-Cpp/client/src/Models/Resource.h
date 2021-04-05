#ifndef AIC21_CLIENT_CPP_RESOURCE_H
#define AIC21_CLIENT_CPP_RESOURCE_H

#include "Models/enums.h"

class Resource {

public:
    Resource() = default;
    ~Resource() = default;
    Resource(ResourceType type, int value);
    ResourceType getType() const;
    int getValue() const;

private:
    ResourceType type_;
    int value_;
};

#endif //AIC21_CLIENT_CPP_RESOURCE_H