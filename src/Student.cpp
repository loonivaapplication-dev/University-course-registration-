#include "../include/Student.h"
#include "../include/RegistrationManager.h"
#include "../include/Input.h"

#include <algorithm>
#include <iostream>
#include <vector>

Student::Student() : User() {
}

Student::Student(const string& id, const string& name, const string& email)
    : User(id, name, email) {
}

string Student::getRole() const {
    return "Student";
}

// ------------------------------------
// LOGIN
// ------------------------------------

bool Student::login(RegistrationManager& manager) {

    string id;

    cout << "\n---- STUDENT LOGIN ----" << endl;

    if (!readLine("Enter your Student ID: ", id)) return false;

    if (id.empty() || !safeText(id)) {
        cout << "Invalid Student ID!" << endl;
        return false;
    }

    Student* existing = manager.findStudent(id);

    if (existing != nullptr) {
        *this = *existing;   // copy the saved details
    }
    else {

        string answer;
        string name;
        string email;

        cout << "No student with this ID was found." << endl;

        if (!readLine("Create a new account? (y/n): ", answer)) return false;
        if (answer != "y" && answer != "Y") return false;

        if (!readLine("Full name: ", name)) return false;
        if (!readLine("Email: ", email)) return false;

        setUserID(id);
        setName(name);
        setEmail(email);

        if (!manager.addStudent(*this)) return false;   // prints the reason
    }

    cout << "\nWelcome, student " << getUserID() << "!" << endl;
    return true;
}

// ------------------------------------
// MENU
// ------------------------------------

void Student::menu(RegistrationManager& manager) {

    int choice;

    while (true) {

        cout << "\n===== STUDENT MENU (" << getUserID() << ") =====" << endl;
        cout << "1. View Available Courses" << endl;
        cout << "2. Register for a Course" << endl;
        cout << "3. Drop a Course" << endl;
        cout << "4. View My Schedule" << endl;
        cout << "5. Check Schedule Conflicts" << endl;
        cout << "0. Logout" << endl;

        if (!readInt("Enter your choice: ", choice)) return;

        switch (choice) {

            case 1: viewAvailableCourses(manager); break;
            case 2: registerForCourse(manager); break;
            case 3: dropCourse(manager); break;
            case 4: viewSchedule(manager); break;
            case 5: checkScheduleConflicts(manager); break;

            case 0:
                cout << "Student logged out successfully!" << endl;
                return;

            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
}

// ------------------------------------
// ACTIONS
// ------------------------------------

void Student::viewAvailableCourses(RegistrationManager& manager) {
    manager.displayCourses();
}

void Student::registerForCourse(RegistrationManager& manager) {
    string courseID;
    if (!readLine("Enter Course ID to register: ", courseID)) return;
    manager.registerCourse(getUserID(), courseID);
}

void Student::dropCourse(RegistrationManager& manager) {
    string courseID;
    if (!readLine("Enter Course ID to drop: ", courseID)) return;
    manager.dropCourse(getUserID(), courseID);
}

// Earlier day first, then earlier start time
static bool comesBefore(const Course* a, const Course* b) {
    if (a->getDayOrder() != b->getDayOrder()) {
        return a->getDayOrder() < b->getDayOrder();
    }
    return a->getStartMinutes() < b->getStartMinutes();
}

void Student::viewSchedule(RegistrationManager& manager) {

    vector<Course*> mine = manager.getStudentCourses(getUserID());

    if (mine.empty()) {
        cout << "\nNo registered courses found.\n";
        return;
    }

    sort(mine.begin(), mine.end(), comesBefore);

    cout << "\n========================================\n";
    cout << "       STUDENT WEEKLY SCHEDULE\n";
    cout << "========================================\n";
    cout << "Student ID: " << getUserID() << "\n";

    string currentDay = "";

    for (size_t i = 0; i < mine.size(); i++) {

        if (mine[i]->getDay() != currentDay) {
            currentDay = mine[i]->getDay();
            cout << "\n--- " << currentDay << " ---\n";
        }

        cout << mine[i]->getCourseName()
             << " (" << mine[i]->getCourseID() << ", " << mine[i]->getCourseType() << ")"
             << "\nTime: " << mine[i]->getStartTime()
             << " - " << mine[i]->getEndTime()
             << "\n\n";
    }

    cout << "========================================\n";
}

void Student::checkScheduleConflicts(RegistrationManager& manager) {

    vector<Course*> mine = manager.getStudentCourses(getUserID());

    if (mine.size() < 2) {
        cout << "\nNo schedule conflicts found.\n";
        return;
    }

    bool conflictFound = false;

    cout << "\n========================================\n";
    cout << "       SCHEDULE CONFLICT CHECK\n";
    cout << "========================================\n";

    for (size_t i = 0; i < mine.size(); i++) {
        for (size_t j = i + 1; j < mine.size(); j++) {

            if (!mine[i]->conflictsWith(*mine[j])) continue;

            conflictFound = true;

            int overlapStart = max(mine[i]->getStartMinutes(), mine[j]->getStartMinutes());
            int overlapEnd = min(mine[i]->getEndMinutes(), mine[j]->getEndMinutes());

            cout << "\nCONFLICT DETECTED!\n";
            cout << "Day: " << mine[i]->getDay() << endl;
            cout << "Course 1: " << mine[i]->getCourseName()
                 << " (" << mine[i]->getCourseID() << ")" << endl;
            cout << "Course 2: " << mine[j]->getCourseName()
                 << " (" << mine[j]->getCourseID() << ")" << endl;
            cout << "Overlapping time: " << Course::formatTime(overlapStart)
                 << " - " << Course::formatTime(overlapEnd) << endl;
        }
    }

    if (!conflictFound) {
        cout << "\nNo schedule conflicts found!"
             << "\nAll courses have different time slots.\n";
    }

    cout << "========================================\n";
}
