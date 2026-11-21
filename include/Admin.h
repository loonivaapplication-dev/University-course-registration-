#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"

// An Admin is a kind of User (inheritance)
class Admin : public User {

private:
    string password;

    void manageCourses(RegistrationManager& manager);          // view / add / edit / delete courses
    void viewAllStudents(RegistrationManager& manager);
    void viewAllRegistrations(RegistrationManager& manager);

public:
    Admin();   // default username "admin" and password "1234"

    bool login(RegistrationManager& manager) override;   // checks admin credentials
    void menu(RegistrationManager& manager) override;    // admin menu
    string getRole() const override;
};

#endif
