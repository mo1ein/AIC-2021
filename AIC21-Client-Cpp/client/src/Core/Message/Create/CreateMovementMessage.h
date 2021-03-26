#ifndef AIC21_CLIENT_CPP_CREATEMOVEMENTMESSAGE_H
#define AIC21_CLIENT_CPP_CREATEMOVEMENTMESSAGE_H

#include "Core/Message/Message.h"
#include "Models/enums.h"

class CreateMovementMessage final : public Message {

public:

    inline explicit CreateMovementMessage(int direction) : Message("1", json::object()) {
        json tmp_json;
        tmp_json["direction"] = direction;
        setInfo(tmp_json);
    }

};

#endif //AIC21_CLIENT_CPP_CREATEMOVEMENTMESSAGE_H
