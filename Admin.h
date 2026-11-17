//header guards 
#ifndef ADMIN_H //if it has not been defined
#define ADMIN_H //defines the identifier ADMIN_H

#include <iostream>
#include <string>
using namespace std;

class Admin {

private:
    string username;
    string password;

public:
    Admin(); //declares Admin constructor

    bool adminLogin(); //declares admin login function, 
   // checks admin credentials and returns true if succesfull

    void adminMenu(); // Displays the admin menu and handles admin selections

    void manageCourses(); // Provides access to course management functionality

    void viewAllStudents(); // Displays all registered students in the system

    void viewAllRegistrations(); // displays all course registration records
};

#endif
