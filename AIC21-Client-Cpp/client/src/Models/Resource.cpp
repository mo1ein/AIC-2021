#include "Resource.h"

Resource::Resource(ResourceType type, int value) {
    type_ = type;
    value_ = value;
}

ResourceType Resource::getType() {
    return type_;
}

int Resource::getValue() {
    return value_;
}