#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

class RegistrationManager;   // defined in RegistrationManager.h

// User is an abstract base class for everyone who can log in.
// Student and Admin inherit from it and each provide their own
// login(), menu() and getRole() (polymorphism).
class User {

private:
    string userID;
    string name;
    string email;

public:
    User();
    User(const string& id, const string& userName, const string& userEmail);
    virtual ~User() {}

    string getUserID() const;
    string getName() const;
    string getEmail() const;

    void setUserID(const string& id);
    void setName(const string& userName);
    void setEmail(const string& userEmail);

    void displayInfo() const;

    virtual bool login(RegistrationManager& manager) = 0;   // true if login succeeded
    virtual void menu(RegistrationManager& manager) = 0;    // menu shown after login
    virtual string getRole() const = 0;                     // "Student" or "Admin"
};

#endif
