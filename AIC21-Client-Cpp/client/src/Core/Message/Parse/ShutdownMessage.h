#ifndef AIC21_CLIENT_CPP_SHUTDOWN_MESSAGE_H
#define AIC21_CLIENT_CPP_SHUTDOWN_MESSAGE_H

#include "Core/Message/Message.h"
#include "Core/Message/Parse/ParseError.h"

using namespace std;


class ShutdownMessage : public Message {
public:

    explicit ShutdownMessage(const json &root) : Message(root) {
        if (Message::getType() != "7")
            throw ParseError("Invalid shutdown message");
    }

    explicit ShutdownMessage(const string &json_form) : Message(json_form) {
        if (Message::getType() != "7")
            throw ParseError("Invalid shutdown message");
    }
};

#endif // AIC21_CLIENT_CPP_SHUTDOWN_MESSAGE_H