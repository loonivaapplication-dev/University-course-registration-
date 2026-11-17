
#include "Admin.h"
#include <limits> //provides numeric limits,clear invalid input from buffer

// Constructor 
Admin::Admin() {
    username = "admin"; //default username and pw
    password = "1234";
}

// Admin Login
bool Admin::adminLogin() { //bool returns only tru or false

    string enteredUsername; //var to store user's input
    string enteredPassword;

    cout << "\n---- ADMIN LOGIN ----" << endl;

    cout << "Enter username: ";
    cin >> enteredUsername;

    cout << "Enter password: ";
    cin >> enteredPassword;

    if (enteredUsername == username &&
        enteredPassword == password) {

        cout << "\nLogin successful!" << endl;
        return true;
    }
    else {
        cout << "\nInvalid username or password!" << endl;
        return false;
    }
}

// Admin Menu
void Admin::adminMenu() { //using void bcs it persforms action and displays a menu

    int choice;

  //keep displaying menu until the adin chooses to log out
    while (true) {

        cout << "\n===== ADMIN MENU =====" << endl;
        cout << "1. Manage Courses" << endl;
        cout << "2. View All Students" << endl;
        cout << "3. View All Registrations" << endl;
        cout << "0. Logout" << endl;

        cout << "Enter your choice: ";
      
 //invalid input handling
        if (!(cin >> choice)) {
            cin.clear(); //resets endtered error
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
          //removes invalid char from input buffer

            cout << "Invalid input! Enter a number." << endl;
            continue;
        }

        switch (choice) {

            case 1:
                manageCourses();
                break;

            case 2:
                viewAllStudents();
                break;

            case 3:
                viewAllRegistrations();
                break;

            case 0:
                cout << "Admin logged out successfully!" << endl;
                return;

            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
}

// Allow admin to Manage Courses
void Admin::manageCourses() {
    cout << "\nCourse management will be connected here." << endl;
}

// Allow admin to view All Students
void Admin::viewAllStudents() {
    cout << "\nStudent list will be connected here." << endl;
}

// Allow administration to View All Registrations
void Admin::viewAllRegistrations() {
    cout << "\nRegistration records will be connected here." << endl;
}
