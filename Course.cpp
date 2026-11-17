#include "Course.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>

using namespace std;

// Static variables
vector<Course> Course::courses;
bool Course::coursesLoaded = false;

// Default constructor
Course::Course() {
    courseID = "";
    courseName = "";
    credits = 0;
    instructor = "";
    capacity = 0;
    enrolledStudents = 0;
    day = "";
    startTime = "";
    endTime = "";
}

// Parameterized constructor
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
    if (coursesLoaded) return;

    courses.clear();

    ifstream file("courses.txt");
    string line;

    while (getline(file, line)) {
        stringstream ss(line);

        string id, name, creditText, teacher;
        string capacityText, enrolledText;
        string courseDay, start, end;

        if (!getline(ss, id, '|') ||
            !getline(ss, name, '|') ||
            !getline(ss, creditText, '|') ||
            !getline(ss, teacher, '|') ||
            !getline(ss, capacityText, '|') ||
            !getline(ss, enrolledText, '|') ||
            !getline(ss, courseDay, '|') ||
            !getline(ss, start, '|') ||
            !getline(ss, end)) {
            continue;
        }

        try {
            int credit = stoi(creditText);
            int cap = stoi(capacityText);
            int enrolled = stoi(enrolledText);

            if (credit > 0 && cap >= 0 &&
                enrolled >= 0 && enrolled <= cap) {
                courses.push_back(
                    Course(id, name, credit, teacher, cap, enrolled,
                           courseDay, start, end)
                );
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

    for (const Course& c : courses) {
        file << c.courseID << "|"
             << c.courseName << "|"
             << c.credits << "|"
             << c.instructor << "|"
             << c.capacity << "|"
             << c.enrolledStudents << "|"
             << c.day << "|"
             << c.startTime << "|"
             << c.endTime << endl;
    }

    file.close();
}

// Check course availability
bool Course::checkAvailability() const {
    return enrolledStudents < capacity;
}

// Getters
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

    string id, name, teacher;
    int credit, cap;

    cout << "\nEnter Course ID: ";
    getline(cin >> ws, id);

    for (const Course& c : courses) {
        if (c.courseID == id) {
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

    string courseDay, start, end;

    cout << "Enter Day (e.g. Monday): ";
    getline(cin >> ws, courseDay);

    cout << "Enter Start Time (HH:MM): ";
    getline(cin >> ws, start);

    cout << "Enter End Time (HH:MM): ";
    getline(cin >> ws, end);

    courses.push_back(Course(id, name, credit, teacher, cap, 0,
                             courseDay, start, end));

    saveCourses();

    cout << "Course added successfully!\n";
}

// Edit course
void Course::editCourse() {
    loadCourses();

    string id;

    cout << "\nEnter Course ID to edit: ";
    getline(cin >> ws, id);

    for (Course& c : courses) {
        if (c.courseID == id) {
            // Read into temporary variables first, so invalid input
            // cannot leave the course half-edited
            string newName, newInstructor;
            int newCredits, newCapacity;

            cout << "Enter New Course Name: ";
            getline(cin >> ws, newName);

            cout << "Enter New Credits: ";
            if (!(cin >> newCredits) || newCredits <= 0) {
                cout << "Invalid credits!\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return;
            }

            cout << "Enter New Instructor: ";
            getline(cin >> ws, newInstructor);

            cout << "Enter New Capacity: ";
            if (!(cin >> newCapacity) ||
                newCapacity < c.enrolledStudents ||
                newCapacity <= 0) {
                cout << "Invalid capacity!\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return;
            }

            c.courseName = newName;
            c.credits = newCredits;
            c.instructor = newInstructor;
            c.capacity = newCapacity;
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

    for (auto it = courses.begin(); it != courses.end(); ++it) {
        if (it->courseID == id) {
            courses.erase(it);
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

    if (courses.empty()) {
        cout << "\nNo courses available.\n";
        return;
    }

    cout << "\n========== COURSE LIST ==========\n";

    for (const Course& c : courses) {
        cout << "\nCourse ID: " << c.courseID;
        cout << "\nCourse Name: " << c.courseName;
        cout << "\nCredits: " << c.credits;
        cout << "\nInstructor: " << c.instructor;
        cout << "\nCapacity: " << c.capacity;
        cout << "\nEnrolled Students: " << c.enrolledStudents;
        cout << "\nAvailable Seats: " << c.getAvailableSeats();
        cout << "\n---------------------------------\n";
    }
}

// Enroll student
bool Course::enrollStudent() {
    loadCourses();

    string id;

    cout << "\nEnter Course ID to enroll: ";
    getline(cin >> ws, id);

    for (Course& c : courses) {
        if (c.courseID == id) {
            if (c.checkAvailability()) {
                c.enrolledStudents++;
                saveCourses();

                cout << "Enrollment successful!\n";
                cout << "Available seats: "
                     << c.getAvailableSeats() << endl;

                return true;
            }

            cout << "Sorry! Course is full.\n";
            return false;
        }
    }

    cout << "Course not found!\n";
    return false;
}