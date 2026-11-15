
#include "User.h"
#include <iostream>

User::User() {}

User::User(const std::string& name,
           const std::string& email,
           const std::string& password)
    : name(name), email(email), password(password) {}

std::string User::getName() const {
    return name;
}

std::string User::getEmail() const {
    return email;
}

bool User::checkPassword(const std::string& input) const {
    return password == input;
}

void User::displayProfile() const {
    std::cout << "Name: " << name << '\n';
    std::cout << "Email: " << email << '\n';
}
