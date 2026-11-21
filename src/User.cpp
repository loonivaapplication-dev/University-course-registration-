#include "../include/User.h"
#include <iostream>

User::User() {
}

User::User(const string& id, const string& userName, const string& userEmail)
    : userID(id), name(userName), email(userEmail) {
}

string User::getUserID() const { return userID; }
string User::getName() const { return name; }
string User::getEmail() const { return email; }

void User::setUserID(const string& id) { userID = id; }
void User::setName(const string& userName) { name = userName; }
void User::setEmail(const string& userEmail) { email = userEmail; }

void User::displayInfo() const {
    cout << "ID: " << userID
         << " | Name: " << name
         << " | Email: " << email << endl;
}
