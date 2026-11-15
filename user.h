
#ifndef USER_H
#define USER_H

#include <string>

class User {
private:
    std::string name;
    std::string email;
    std::string password;

public:
    User();
    User(const std::string& name,
         const std::string& email,
         const std::string& password);

    virtual ~User() = default;

    std::string getName() const;
    std::string getEmail() const;

    bool checkPassword(const std::string& password) const;

    virtual void displayProfile() const;
};

#endif
