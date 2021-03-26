#include "Answer.h"

Answer::Answer(Direction direction, string message, int messageValue) {
    direction_ = direction;
    message_ = message;
    message_value_ = messageValue;
}

Answer::Answer(Direction direction) {
    direction_ = direction;
}

Direction Answer::getDirection() {
    return direction_;
}

void Answer::setDirection(Direction direction) {
    direction_ = direction;
}

string Answer::getMessage() {
    return message_;
}

void Answer::setMessage(string message) {
    message_ = message;
}

int Answer::getMessageValue() {
    return message_value_;
}

void Answer::setMessageValue(int messageValue) {
    message_value_ = messageValue;
}