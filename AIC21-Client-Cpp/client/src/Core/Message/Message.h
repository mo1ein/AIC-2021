#ifndef AIC21_CLIENT_CPP_MESSAGE_H
#define AIC21_CLIENT_CPP_MESSAGE_H

#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>

using namespace std;
using nlohmann::json;


class Message {

public:

    Message() = default;
    virtual ~Message() = default;

    Message(const Message&) = default;
    Message& operator= (const Message&) = default;

    Message(Message&&) = default;
    Message& operator= (Message&&) = default;

    explicit Message(const json &root);
    explicit Message(const string &json_form);

    explicit Message(const string &type, const json &info);

    string toString() const;

    void setType(const string& type);
    string getType() const;

    void setInfo(const json& info);
    json& getMutableInfo();
    json getInfo() const;

    /**
     * Parse and construct the appropriate message from its string form
     *
     * @throws ParseError if the message type is unknown
     * @throws Json::Exception if the message is malformed
     *
     * @return Pointer to the appropriate created message object
     */
    static unique_ptr<Message> CreateFromJsonString(const string& json_form);


protected:

    json m_root_;

};

#endif // AIC21_CLIENT_CPP_MESSAGE_H
