#ifndef AIC21_CLIENT_CPP_AUTHENTICATION_MESSAGE_H
#define AIC21_CLIENT_CPP_AUTHENTICATION_MESSAGE_H

#include <vector>

#include "Core/Message/Message.h"

class AuthenticationMessage : public Message {

public:

    inline explicit AuthenticationMessage(const std::string& token) : Message("token", {{"token", token},})
    {

    }

};

#endif // AIC21_CLIENT_CPP_AUTHENTICATION_MESSAGE_H
