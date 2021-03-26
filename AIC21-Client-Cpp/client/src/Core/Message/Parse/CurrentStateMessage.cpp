#include "Utility/Logger.h"
#include "Utility/Utility.h"
#include "CurrentStateMessage.h"

using namespace std;


CurrentStateMessage::CurrentStateMessage(const json &root) : Message(root) {
    if (getType() != "4")
        throw ParseError("Invalid game state message");
}

CurrentStateMessage::CurrentStateMessage(const string &json_form) : Message(json_form) {
    if (getType() != "4")
        throw ParseError("Invalid game state message");
}
