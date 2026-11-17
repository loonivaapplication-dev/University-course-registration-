
#ifndef USER_IMPLEMENTATION
#define USER_IMPLEMENTATION

#include <iostream>
#include <string>
using namespace std;

class User {
private:
    string name;
    string email;
    string password;

public:
    User() = default;

    User(string n, string e, string p) {
        name = n;
        email = e;
        password = p;
    }

    virtual ~User() = default;

    string getName() const {
        return name;
    }

    string getEmail() const {
        return email;
    }

    bool checkPassword(const string& p) const {
        return password == p;
    }

    virtual void displayProfile() const {
        cout << "Name: " << name << endl;
        cout << "Email: " << email << endl;
    }
};

#endif
