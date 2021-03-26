#include <sstream>
#include <algorithm>
#include <memory>

#include "Utility/Utility.h"
#include "Utility/Logger.h"

#include "Message.h"
#include "Core/Message/Parse/ParseError.h"
#include "Core/Message/Parse/GameConfigMessage.h"
#include "Core/Message/Parse/CurrentStateMessage.h"
#include "Core/Message/Parse/ShutdownMessage.h"

using namespace std;

Message::Message(const json &root) : m_root_(root) {
}

Message::Message(const string &json_form) {
    istringstream stream(json_form);
    stream >> m_root_;
}

Message::Message(const string &type, const json &info) {
    setType(type);
    setInfo(info);
}

string Message::toString() const {
    return m_root_.dump();
}

void Message::setType(const string &type) {
    m_root_["type"] = type;
}

string Message::getType() const {
    return m_root_["type"];
}

void Message::setInfo(const json &info) {
    m_root_["info"] = info;
}

json &Message::getMutableInfo() {
    return m_root_["info"];
}

json Message::getInfo() const {
    return m_root_["info"];
}

unique_ptr<Message> Message::CreateFromJsonString(const string &string_form) {
    json root;
    istringstream stream(string_form);
    stream >> root;
    //There are three types
    if (root["type"] == "3")
        return unique_ptr<GameConfigMessage>(new GameConfigMessage(root));
    if (root["type"] == "4")
        return unique_ptr<CurrentStateMessage>(new CurrentStateMessage(root));
    if (root["type"] == "7")
        return unique_ptr<ShutdownMessage>(new ShutdownMessage(root));
    throw ParseError("Unknown message type");
}