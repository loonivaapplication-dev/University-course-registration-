#include <iostream>

#include "include/User.h"
#include "include/Student.h"
#include "include/Admin.h"
#include "include/RegistrationManager.h"
#include "include/Input.h"

using namespace std;

int main() {

    RegistrationManager manager;

    // Load saved students, courses and registrations once at start-up
    manager.loadFromFile();

    int choice;

    while (true) {

        cout << "\n===== UNIVERSITY COURSE REGISTRATION SYSTEM =====" << endl;
        cout << "1. Student Login" << endl;
        cout << "2. Admin Login" << endl;
        cout << "0. Exit" << endl;

        if (!readInt("Enter your choice: ", choice)) break;

        if (choice == 0) {
            cout << "\nThank you for using the system!" << endl;
            break;
        }

        // One User pointer can hold a Student or an Admin (polymorphism)
        User* user = nullptr;

        if (choice == 1) user = new Student();
        else if (choice == 2) user = new Admin();
        else {
            cout << "Invalid choice! Please select 0, 1, or 2." << endl;
            continue;
        }

        if (user->login(manager)) {
            user->menu(manager);
        }

        delete user;
    }

    return 0;
}
