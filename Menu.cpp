#include "Menu.h"
#include "Admin.h"
#include "Input.h"

// ------------------------------------
// STUDENT LOGIN
// ------------------------------------

void Menu::studentLogin() {

    string id;

    cout << "\n---- STUDENT LOGIN ----" << endl;

    if (!readLine("Enter your Student ID: ", id)) return;

    if (id.empty() || !safeText(id)) {
        cout << "Invalid Student ID!" << endl;
        return;
    }

    if (!system.studentExists(id)) {

        string answer;
        cout << "No student with this ID was found." << endl;

        if (!readLine("Create a new account? (y/n): ", answer)) return;
        if (answer != "y" && answer != "Y") return;

        FileStudent student;
        student.studentID = id;

        if (!readLine("Full name: ", student.name)) return;
        if (!readLine("Email: ", student.email)) return;

        if (!system.addStudent(student)) return;   // prints the reason
    }

    cout << "\nWelcome, student " << id << "!" << endl;
    studentMenu(id);
}

// ------------------------------------
// STUDENT MENU
// ------------------------------------

void Menu::studentMenu(const string& studentID) {

    int choice;

    while (true) {

        cout << "\n===== STUDENT MENU (" << studentID << ") =====" << endl;
        cout << "1. View Available Courses" << endl;
        cout << "2. Register for a Course" << endl;
        cout << "3. Drop a Course" << endl;
        cout << "4. View My Schedule" << endl;
        cout << "5. Check Schedule Conflicts" << endl;
        cout << "0. Logout" << endl;

        if (!readInt("Enter your choice: ", choice)) return;

        string courseID;

        switch (choice) {

            case 1:
                system.displayCourses();
                break;

            case 2:
                if (!readLine("Enter Course ID to register: ", courseID)) return;
                system.registerCourse(studentID, courseID);
                break;

            case 3:
                if (!readLine("Enter Course ID to drop: ", courseID)) return;
                system.dropCourse(studentID, courseID);
                break;

            case 4:
                system.viewSchedule(studentID);
                break;

            case 5:
                system.checkScheduleConflict(studentID);
                break;

            case 0:
                cout << "Student logged out successfully!" << endl;
                return;

            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
}

// ------------------------------------
// MAIN MENU
// ------------------------------------

void Menu::displayMainMenu() {

    int choice;
    Admin admin(system);

    // Load saved students, courses and registrations once at start-up
    system.loadAll();

    while (true) {

        cout << "\n===== UNIVERSITY COURSE REGISTRATION SYSTEM =====" << endl;
        cout << "1. Student Login" << endl;
        cout << "2. Admin Login" << endl;
        cout << "0. Exit" << endl;

        if (!readInt("Enter your choice: ", choice)) return;

        switch (choice) {

            case 1:
                studentLogin();
                break;

            case 2:
                if (admin.adminLogin()) {
                    admin.adminMenu();
                }
                break;

            case 0:
                cout << "\nThank you for using the system!" << endl;
                return;

            default:
                cout << "Invalid choice! Please select 0, 1, or 2." << endl;
        }
    }
}