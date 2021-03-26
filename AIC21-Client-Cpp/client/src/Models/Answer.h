#ifndef AIC21_CLIENT_CPP_ANSWER_H
#define AIC21_CLIENT_CPP_ANSWER_H

using namespace std;
#include "Models/enums.h"
#include <string>


class Answer {

public:
    Answer(Direction direction, string message, int messageValue);
    Answer(Direction direction);
    ~Answer() = default;
    Direction getDirection();
    void setDirection(Direction direction);
    string getMessage();
    void setMessage(string message);
    int getMessageValue();
    void setMessageValue(int messageValue);

private:
    Direction direction_;
    string message_;
    int message_value_;
};

#endif //AIC21_CLIENT_CPP_ANSWER_H