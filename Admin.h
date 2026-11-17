//header guards
#ifndef ADMIN_H
#define ADMIN_H

#include <iostream>
#include <string>
#include "RegistrationManagement.h"
using namespace std;

class Admin {

private:
    string username;
    string password;
    RegistrationManagement& system;   // shared data (students, courses, registrations)

public:
    Admin(RegistrationManagement& sys);

    bool adminLogin();            // checks admin credentials, true if successful
    void adminMenu();             // displays the admin menu and handles selections
    void manageCourses();         // add / edit / delete / list courses
    void viewAllStudents();       // displays all students
    void viewAllRegistrations();  // displays all registration records
};

#endif