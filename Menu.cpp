#include "Menu.h"
#include "Admin.h"
#include <limits>

//displays the main menu
void Menu::displayMainMenu() {

    int choice;
    Admin admin; //create admin obj

    // Keep displaying the menu until the user exits
    while (true) {

        cout << "\n===== UNIVERSITY COURSE REGISTRATION SYSTEM =====" << endl;
        cout << "1. Student Login" << endl;
        cout << "2. Admin Login" << endl;
        cout << "0. Exit" << endl;

        cout << "Enter your choice: ";

        // Check whether the input is a valid integer
        if (!(cin >> choice)) {

            // Clear the input error
            cin.clear();

            // Remove invalid input from the input buffer
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Invalid input! Please enter a number." << endl;
            continue;
        }
      
        switch (choice) {

            case 1:
                cout << "\nStudent login will be connected here." << endl;
                break;

            case 2:
                // Check admin login credentials 
                if (admin.adminLogin()) {

                    // Open admin menu only after successful login
                    admin.adminMenu();
                }
                break;

            case 0:
                // Exit the entire system
                cout << "\nThank you for using the system!" << endl;
                return;

            default:
                // Handle invalid menu selections
                cout << "Invalid choice! Please select 0, 1, or 2." << endl;
        }
    }
}
