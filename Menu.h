#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <string>
#include "RegistrationManagement.h"
using namespace std;

// Main Menu class controls navigation in the system
class Menu {

private:
    RegistrationManagement system;   // all students, courses and registrations

    void studentLogin();                       // log in (or create an account) by student ID
    void studentMenu(const string& studentID); // register, drop, schedule ...

public:
    // Displays the main menu and handles user selections
    void displayMainMenu();
};

#endif