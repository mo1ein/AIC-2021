#ifndef AIC21_CLIENT_CPP_PARSE_ERROR_H
#define AIC21_CLIENT_CPP_PARSE_ERROR_H

#include <string>
#include <stdexcept>

using namespace std;

/**
 * An exception encountered when parsing messages from server
 */
class ParseError : public runtime_error {

public:

    explicit inline ParseError(const string& message) : runtime_error(message)
    {
    }

};

#endif // AIC21_CLIENT_CPP_PARSE_ERROR_H
