
#include "Course.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
using namespace std;

Course Course::courses[100];
int Course::courseCount = 0;
bool Course::coursesLoaded = false;

// Default constructor
Course::Course() {
    day = "";
    startTime = "";
    endTime = "";
    courseID = "";
    courseName = "";
    credits = 0;
    instructor = "";
    capacity = 0;
    enrolledStudents = 0;
}

// Six-parameter constructor
Course::Course(string id, string name, int courseCredits,
               string teacher, int courseCapacity, int enrolled) {
    courseID = id;
    courseName = name;
    credits = courseCredits;
    instructor = teacher;
    capacity = courseCapacity;
    enrolledStudents = enrolled;

    day = "";
    startTime = "";
    endTime = "";
}
// Nine-parameter constructor
Course::Course(string id, string name, int courseCredits,
               string teacher, int courseCapacity, int enrolled,
               string courseDay, string start, string end) {

    courseID = id;
    courseName = name;
    credits = courseCredits;
    instructor = teacher;
    capacity = courseCapacity;
    enrolledStudents = enrolled;

    day = courseDay;
    startTime = start;
    endTime = end;
}

// Load courses from file
void Course::loadCourses() {

    if (coursesLoaded) {
        return;
    }
    courseCount = 0;

    ifstream file("courses.txt");
    string line;

    while (getline(file, line) && courseCount < 100) {
        stringstream ss(line);
        string id, name, creditText, teacher;
        string capacityText, enrolledText;
        string courseDay, start, end;

        if (!getline(ss, id, '|') ||
            !getline(ss, name, '|') ||
            !getline(ss, creditText, '|') ||
            !getline(ss, teacher, '|') ||
            !getline(ss, capacityText, '|') ||
            !getline(ss, enrolledText)) {
            continue;
        }

        // Optional schedule fields for older six-column files
        getline(ss, courseDay, '|');
        getline(ss, start, '|');
        getline(ss, end);

        try {
            int credit = stoi(creditText);
            int cap = stoi(capacityText);
            int enrolled = stoi(enrolledText);

            if (credit > 0 && cap > 0 &&
                enrolled >= 0 && enrolled <= cap) {

                courses[courseCount] = Course(
                    id, name, credit, teacher,
                    cap, enrolled, courseDay, start, end
                );

                courseCount++;
            }
        }
        catch (...) {
            continue;
        }
    }

    file.close();
    coursesLoaded = true;
}

// Save courses to file
void Course::saveCourses() {

    ofstream file("courses.txt");

    if (!file) {
        cout << "Error saving courses!\n";
        return;
    }

    for (int i = 0; i < courseCount; i++) {

        file << courses[i].courseID << "|"
             << courses[i].courseName << "|"
             << courses[i].credits << "|"
             << courses[i].instructor << "|"
             << courses[i].capacity << "|"
             << courses[i].enrolledStudents << "|"
             << courses[i].day << "|"
             << courses[i].startTime << "|"
             << courses[i].endTime << endl;
    }

    file.close();
}

// Check course availability
bool Course::checkAvailability() const {
    return enrolledStudents < capacity;
}

// Getter functions
string Course::getCourseID() const {
    return courseID;
}

string Course::getCourseName() const {
    return courseName;
}

int Course::getCredits() const {
    return credits;
}

string Course::getInstructor() const {
    return instructor;
}

int Course::getCapacity() const {
    return capacity;
}

int Course::getEnrolledStudents() const {
    return enrolledStudents;
}

int Course::getAvailableSeats() const {
    return capacity - enrolledStudents;
}

// Add course
void Course::addCourse() {

    loadCourses();

    if (courseCount >= 100) {
        cout << "Course storage is full!\n";
        return;
    }

    string id, name, teacher;
    string courseDay, start, end;
    int credit, cap;

    cout << "\nEnter Course ID: ";
    getline(cin >> ws, id);

    // Check duplicate course ID
    for (int i = 0; i < courseCount; i++) {

        if (courses[i].courseID == id) {
            cout << "Course ID already exists!\n";
            return;
        }
    }

    cout << "Enter Course Name: ";
    getline(cin, name);

    cout << "Enter Credits: ";

    if (!(cin >> credit) || credit <= 0) {
        cout << "Invalid credits!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    cout << "Enter Instructor Name: ";
    getline(cin >> ws, teacher);

    cout << "Enter Course Capacity: ";

    if (!(cin >> cap) || cap <= 0) {
        cout << "Invalid capacity!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    cout << "Enter Course Day: ";
    getline(cin >> ws, courseDay);

    cout << "Enter Start Time: ";
    getline(cin, start);

    cout << "Enter End Time: ";
    getline(cin, end);

    courses[courseCount] = Course(
        id, name, credit, teacher, cap, 0,
        courseDay, start, end
    );

    courseCount++;

    saveCourses();

    cout << "Course added successfully!\n";
}

// Edit course
void Course::editCourse() {

    loadCourses();

    string id;

    cout << "\nEnter Course ID to edit: ";
    getline(cin >> ws, id);

    for (int i = 0; i < courseCount; i++) {

        if (courses[i].courseID == id) {

            cout << "Enter New Course Name: ";
            getline(cin >> ws, courses[i].courseName);

            cout << "Enter New Credits: ";

            if (!(cin >> courses[i].credits) ||
                courses[i].credits <= 0) {

                cout << "Invalid credits!\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return;
            }

            cout << "Enter New Instructor: ";
            getline(cin >> ws, courses[i].instructor);

            int newCapacity;

            cout << "Enter New Capacity: ";

            if (!(cin >> newCapacity) ||
                newCapacity < courses[i].enrolledStudents ||
                newCapacity <= 0) {

                cout << "Invalid capacity!\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return;
            }

            courses[i].capacity = newCapacity;

            cout << "Enter New Course Day: ";
            getline(cin >> ws, courses[i].day);

            cout << "Enter New Start Time: ";
            getline(cin, courses[i].startTime);

            cout << "Enter New End Time: ";
            getline(cin, courses[i].endTime);

            saveCourses();

            cout << "Course updated successfully!\n";
            return;
        }
    }

    cout << "Course not found!\n";
}

// Delete course
void Course::deleteCourse() {

    loadCourses();

    string id;

    cout << "\nEnter Course ID to delete: ";
    getline(cin >> ws, id);

    for (int i = 0; i < courseCount; i++) {

        if (courses[i].courseID == id) {

            // Shift remaining courses to the left
            for (int j = i; j < courseCount - 1; j++) {
                courses[j] = courses[j + 1];
            }

            courseCount--;

            saveCourses();

            cout << "Course deleted successfully!\n";
            return;
        }
    }

    cout << "Course not found!\n";
}

// Display courses
void Course::displayCourses() {

    loadCourses();

    if (courseCount == 0) {
        cout << "\nNo courses available.\n";
        return;
    }

    cout << "\n========== COURSE LIST ==========\n";

    for (int i = 0; i < courseCount; i++) {

        cout << "\nCourse ID: " << courses[i].courseID;
        cout << "\nCourse Name: " << courses[i].courseName;
        cout << "\nCredits: " << courses[i].credits;
        cout << "\nInstructor: " << courses[i].instructor;
        cout << "\nCapacity: " << courses[i].capacity;
        cout << "\nEnrolled Students: "
             << courses[i].enrolledStudents;
        cout << "\nAvailable Seats: "
             << courses[i].getAvailableSeats();
        cout << "\nDay: " << courses[i].day;
        cout << "\nTime: " << courses[i].startTime
             << " - " << courses[i].endTime;

        cout << "\n---------------------------------\n";
    }
}

// Enroll student
bool Course::enrollStudent() {

    loadCourses();

    string id;

    cout << "\nEnter Course ID to enroll: ";
    getline(cin >> ws, id);

    for (int i = 0; i < courseCount; i++) {

        if (courses[i].courseID == id) {

            if (courses[i].checkAvailability()) {

                courses[i].enrolledStudents++;

                saveCourses();

                cout << "Enrollment successful!\n";
                cout << "Available seats: "
                     << courses[i].getAvailableSeats() << endl;

                return true;
            }

            cout << "Sorry! Course is full.\n";
            return false;
        }
    }

    cout << "Course not found!\n";
    return false;
}
