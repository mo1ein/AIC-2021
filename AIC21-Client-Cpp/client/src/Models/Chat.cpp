#include "Chat.h"

Chat::Chat(string text, int turn) : text_(text), turn_(turn) {
}

string Chat::getText() {
    return text_;
}

int Chat::getTurn() {
    return turn_;
}
