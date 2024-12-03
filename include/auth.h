#ifndef AUTH_H
#define AUTH_H

#include <string>

class Auth {
public:
    Auth(const std::string& client_id, const std::string& client_secret);
    std::string authenticate();
    const std::string& getToken() const;

private:
    std::string client_id_;
    std::string client_secret_;
    std::string token_;
};

#endif
