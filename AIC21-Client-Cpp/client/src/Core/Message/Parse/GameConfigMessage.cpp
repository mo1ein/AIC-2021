#include "Utility/Logger.h"
#include "Utility/Utility.h"
#include "Core/Message/Parse/GameConfigMessage.h"

using namespace std;

GameConfigMessage::GameConfigMessage(const json &root) : Message(root) {
    if (getType() != "3")
        throw ParseError("Invalid game config message");
}

GameConfigMessage::GameConfigMessage(const string &json_form) : Message(json_form) {
    if (getType() != "3")
        throw ParseError("Invalid game config message");
}
