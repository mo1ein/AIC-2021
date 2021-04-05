#ifndef AIC21_CLIENT_CPP_CHATBOX_H
#define AIC21_CLIENT_CPP_CHATBOX_H

using namespace std;
#include <vector>
#include "Models/enums.h"
#include "Models/Chat.h"


class ChatBox {

private:
    vector<const Chat*> all_chats_;

public:
    explicit ChatBox(const vector<const Chat*>& allChats);
    ~ChatBox();
    vector<const Chat*> getAllChatsOfTurn(int turnNumber) const;
    const vector<const Chat*>& getAllChats() const;
};

#endif //AIC21_CLIENT_CPP_CHATBOX_H