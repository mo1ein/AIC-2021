#include "Resource.h"

Resource::Resource(ResourceType type, int value) {
    type_ = type;
    value_ = value;
}

ResourceType Resource::getType() const {
    return type_;
}

int Resource::getValue() const {
    return value_;
}