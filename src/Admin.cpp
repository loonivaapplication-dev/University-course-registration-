#include "../include/Admin.h"
#include "../include/RegistrationManager.h"
#include "../include/Input.h"

#include <iostream>

// Constructor (default username and password)
Admin::Admin() : User("admin", "Administrator", "") {
    password = "1234";
}

string Admin::getRole() const {
    return "Admin";
}

// ------------------------------------
// LOGIN
// ------------------------------------

bool Admin::login(RegistrationManager&) {

    string enteredUsername;
    string enteredPassword;

    cout << "\n---- ADMIN LOGIN ----" << endl;

    if (!readLine("Enter username: ", enteredUsername)) return false;
    if (!readLine("Enter password: ", enteredPassword)) return false;

    if (enteredUsername == getUserID() && enteredPassword == password) {
        cout << "\nLogin successful!" << endl;
        return true;
    }

    cout << "\nInvalid username or password!" << endl;
    return false;
}

// ------------------------------------
// MENU
// ------------------------------------

void Admin::menu(RegistrationManager& manager) {

    int choice;

    while (true) {

        cout << "\n===== ADMIN MENU =====" << endl;
        cout << "1. Manage Courses" << endl;
        cout << "2. View All Students" << endl;
        cout << "3. View All Registrations" << endl;
        cout << "0. Logout" << endl;

        if (!readInt("Enter your choice: ", choice)) return;

        switch (choice) {
            case 1: manageCourses(manager); break;
            case 2: viewAllStudents(manager); break;
            case 3: viewAllRegistrations(manager); break;
            case 0:
                cout << "Admin logged out successfully!" << endl;
                return;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
}

// ------------------------------------
// COURSE MANAGEMENT
// ------------------------------------

void Admin::manageCourses(RegistrationManager& manager) {

    int choice;

    while (true) {

        cout << "\n--- MANAGE COURSES ---" << endl;
        cout << "1. View Courses" << endl;
        cout << "2. Add Course" << endl;
        cout << "3. Edit Course" << endl;
        cout << "4. Delete Course" << endl;
        cout << "0. Back" << endl;

        if (!readInt("Enter your choice: ", choice)) return;

        if (choice == 0) return;

        if (choice == 1) {
            manager.displayCourses();
        }
        else if (choice == 2 || choice == 3) {

            string id, name, instructor, day, start, end;
            int credits, capacity;

            if (!readLine("Course ID: ", id)) return;

            if (choice == 3 && !manager.courseExists(id)) {
                cout << "Course not found!\n";
                continue;
            }

            if (!readLine("Course Name: ", name)) return;
            if (!readInt("Credits: ", credits)) return;
            if (!readLine("Instructor: ", instructor)) return;
            if (!readInt("Capacity: ", capacity)) return;
            if (!readLine("Day (e.g. Monday): ", day)) return;
            if (!readLine("Start Time (HH:MM): ", start)) return;
            if (!readLine("End Time (HH:MM): ", end)) return;

            if (choice == 3) {
                manager.editCourse(id, name, credits, instructor, capacity, day, start, end);
            }
            else {
                int typeChoice;
                if (!readInt("Type (1 = Lecture, 2 = Lab, 3 = Seminar): ", typeChoice)) return;

                string type = "";
                if (typeChoice == 1) type = "Lecture";
                else if (typeChoice == 2) type = "Lab";
                else if (typeChoice == 3) type = "Seminar";
                else {
                    cout << "Invalid course type!\n";
                    continue;
                }

                Course* course = manager.createCourse(type, id, name, credits, instructor,
                                                      capacity, 0, day, start, end);
                manager.addCourse(course);   // the manager takes ownership
            }
        }
        else if (choice == 4) {
            string id;
            if (!readLine("Course ID to delete: ", id)) return;
            manager.deleteCourse(id);
        }
        else {
            cout << "Invalid choice! Please try again." << endl;
        }
    }
}

void Admin::viewAllStudents(RegistrationManager& manager) {
    manager.displayStudents();
}

void Admin::viewAllRegistrations(RegistrationManager& manager) {
    manager.displayRegistrations();
}
