#include "Chat.h"

Chat::Chat(string text, int turn) : text_(text), turn_(turn) {
}

string Chat::getText() const {
    return text_;
}

int Chat::getTurn() const {
    return turn_;
}
