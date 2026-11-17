#include "Admin.h"
#include "Input.h"

// Constructor (default username and password)
Admin::Admin(RegistrationManagement& sys) : system(sys) {
    username = "admin";
    password = "1234";
}

// Admin Login
bool Admin::adminLogin() {

    string enteredUsername;
    string enteredPassword;

    cout << "\n---- ADMIN LOGIN ----" << endl;

    if (!readLine("Enter username: ", enteredUsername)) return false;
    if (!readLine("Enter password: ", enteredPassword)) return false;

    if (enteredUsername == username && enteredPassword == password) {
        cout << "\nLogin successful!" << endl;
        return true;
    }

    cout << "\nInvalid username or password!" << endl;
    return false;
}

// Admin Menu
void Admin::adminMenu() {

    int choice;

    while (true) {

        cout << "\n===== ADMIN MENU =====" << endl;
        cout << "1. Manage Courses" << endl;
        cout << "2. View All Students" << endl;
        cout << "3. View All Registrations" << endl;
        cout << "0. Logout" << endl;

        if (!readInt("Enter your choice: ", choice)) return;

        switch (choice) {
            case 1: manageCourses(); break;
            case 2: viewAllStudents(); break;
            case 3: viewAllRegistrations(); break;
            case 0:
                cout << "Admin logged out successfully!" << endl;
                return;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
}

// Course management submenu
void Admin::manageCourses() {

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
            system.displayCourses();
        }
        else if (choice == 2 || choice == 3) {

            FileCourse c;
            c.enrolledStudents = 0;

            if (!readLine("Course ID: ", c.courseID)) return;

            if (choice == 3 && !system.courseExists(c.courseID)) {
                cout << "Course not found!\n";
                continue;
            }

            if (!readLine("Course Name: ", c.courseName)) return;
            if (!readInt("Credits: ", c.credits)) return;
            if (!readLine("Instructor: ", c.instructor)) return;
            if (!readInt("Capacity: ", c.capacity)) return;
            if (!readLine("Day (e.g. Monday): ", c.day)) return;
            if (!readLine("Start Time (HH:MM): ", c.startTime)) return;
            if (!readLine("End Time (HH:MM): ", c.endTime)) return;

            if (choice == 2) system.addCourse(c);
            else system.editCourse(c);
        }
        else if (choice == 4) {
            string id;
            if (!readLine("Course ID to delete: ", id)) return;
            system.deleteCourse(id);
        }
        else {
            cout << "Invalid choice! Please try again." << endl;
        }
    }
}

void Admin::viewAllStudents() {
    system.displayStudents();
}

void Admin::viewAllRegistrations() {
    system.displayRegistrations();
}