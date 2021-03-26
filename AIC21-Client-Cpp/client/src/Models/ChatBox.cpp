#include "ChatBox.h"

#include <utility>

ChatBox::ChatBox(vector<Chat*> allChats) {
    all_chats_ = std::move(allChats);
}

ChatBox::~ChatBox() {
    for (const Chat* chat : all_chats_) {
        delete chat;
    }
}

vector<Chat*> ChatBox::getAllChatsOfTurn(int turnNumber) {
    vector<Chat*> output;
    for(Chat* chat : all_chats_)
        if(chat->getTurn() == turnNumber)
            output.push_back(chat);
    return output;
}

vector<Chat*>* ChatBox::getAllChats() {
    return &all_chats_;
}
