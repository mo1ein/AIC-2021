#ifndef AIC21_CLIENT_CPP_CREATEENDTURNMESSAGE_H
#define AIC21_CLIENT_CPP_CREATEENDTURNMESSAGE_H

#include "Core/Message/Message.h"
#include "Models/enums.h"

class CreateEndTurnMessage final : public Message {

public:

    inline explicit CreateEndTurnMessage() : Message("6", json::object()) {
    }

};

#endif //AIC21_CLIENT_CPP_CREATEENDTURNMESSAGE_H
