
#ifndef ADMIN_H
#define ADMIN_H

#include <iostream>
#include <string>
using namespace std;

class Admin {

private:
    string username;
    string password;

public:
    Admin();

    bool adminLogin();

    void adminMenu();

    void manageCourses();

    void viewAllStudents();

    void viewAllRegistrations();
};

#endif
