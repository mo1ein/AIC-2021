#include "ChatBox.h"

#include <utility>

ChatBox::ChatBox(const vector<const Chat*>& allChats) {
    all_chats_ = allChats;
}

ChatBox::~ChatBox() {
    for (const Chat* chat : all_chats_) {
        delete chat;
    }
}

vector<const Chat*> ChatBox::getAllChatsOfTurn(int turnNumber) const {
    vector<const Chat*> output;
    for(const Chat* chat : all_chats_)
        if(chat->getTurn() == turnNumber)
            output.push_back(chat);
    return output;
}

const vector<const Chat*>& ChatBox::getAllChats() const {
    return all_chats_;
}
