#ifndef AIC21_CLIENT_CPP_CHATBOX_H
#define AIC21_CLIENT_CPP_CHATBOX_H

using namespace std;
#include <vector>
#include "Models/enums.h"
#include "Models/Chat.h"


class ChatBox {

private:
    vector<Chat*> all_chats_;

public:
    ChatBox(vector<Chat*> allChats);
    ~ChatBox();
    vector<Chat*> getAllChatsOfTurn(int turnNumber);
    vector<Chat*>* getAllChats();
};

#endif //AIC21_CLIENT_CPP_CHATBOX_H