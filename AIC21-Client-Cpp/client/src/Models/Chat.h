#ifndef AIC21_CLIENT_CPP_CHAT_H
#define AIC21_CLIENT_CPP_CHAT_H

using namespace std;
#include <string>

class Chat {

public:
    Chat(string text, int turn);
    string getText();
    int getTurn();

private:
    const string text_;
    int turn_;

};

#endif //AIC21_CLIENT_CPP_CHAT_H