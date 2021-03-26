#ifndef AIC21_CLIENT_CPP_CREATECHATBOXMESSAGE_H
#define AIC21_CLIENT_CPP_CREATECHATBOXMESSAGE_H

#include "Core/Message/Message.h"
#include "Models/enums.h"

class CreateChatBoxMessage final : public Message {

public:

    inline explicit CreateChatBoxMessage(string message, int value) : Message("2", json::object()) {
        json tmp_json;
        tmp_json["message"] = message;
        tmp_json["value"] = value;
        setInfo(tmp_json);
    }

};

#endif //AIC21_CLIENT_CPP_CREATECHATBOXMESSAGE_H
