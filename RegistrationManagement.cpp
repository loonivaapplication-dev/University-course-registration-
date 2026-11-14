#include "RegistrationManagement.h"

// Check whether a student exists
bool RegistrationManagement::studentExists(string studentID) {
    for (const Student& student : students) {
        if (student.studentID == studentID) {
            return true;
        }
    }
    return false;
}

// Check whether a course exists
bool RegistrationManagement::courseExists(string courseID) {
    for (const Course& course : courses) {
        if (course.courseID == courseID) {
            return true;
        }
    }
    return false;
}

// Count registered students in a course
int RegistrationManagement::countRegisteredStudents(string courseID) {
    int count = 0;

    for (const Registration& reg : registrations) {
        if (reg.courseID == courseID) {
            count++;
        }
    }

    return count;
}

// Add a student
void RegistrationManagement::addStudent(Student student) {
    if (studentExists(student.studentID)) {
        cout << "Student already exists!\n";
        return;
    }

    students.push_back(student);
    cout << "Student added successfully!\n";
}

// Add a course
void RegistrationManagement::addCourse(Course course) {
    if (courseExists(course.courseID)) {
        cout << "Course already exists!\n";
        return;
    }

    if (course.capacity <= 0) {
        cout << "Invalid course capacity!\n";
        return;
    }

    courses.push_back(course);
    cout << "Course added successfully!\n";
}

// Register a student for a course
void RegistrationManagement::registerCourse(
    string studentID, string courseID) {

    if (!studentExists(studentID)) {
        cout << "Error: Student does not exist!\n";
        return;
    }

    if (!courseExists(courseID)) {
        cout << "Error: Course does not exist!\n";
        return;
    }

    for (const Registration& reg : registrations) {
        if (reg.studentID == studentID &&
            reg.courseID == courseID) {
            cout << "Error: Duplicate registration!\n";
            return;
        }
    }

    Course* selectedCourse = nullptr;

    for (Course& course : courses) {
        if (course.courseID == courseID) {
            selectedCourse = &course;
            break;
        }
    }

    if (countRegisteredStudents(courseID) >=
        selectedCourse->capacity) {
        cout << "Error: No seats available!\n";
        return;
    }

    registrations.push_back({studentID, courseID});

    cout << "Registration successful!\n";
}

// Drop a registered course
void RegistrationManagement::dropCourse(
    string studentID, string courseID) {

    for (auto it = registrations.begin();
         it != registrations.end(); ++it) {

        if (it->studentID == studentID &&
            it->courseID == courseID) {

            registrations.erase(it);

            cout << "Course dropped successfully!\n";
            cout << "One seat is now available.\n";
            return;
        }
    }

    cout << "Registration not found!\n";
}

// Check available seats
int RegistrationManagement::getAvailableSeats(
    string courseID) {

    if (!courseExists(courseID)) {
        cout << "Course does not exist!\n";
        return -1;
    }

    for (const Course& course : courses) {
        if (course.courseID == courseID) {
            return course.capacity -
                   countRegisteredStudents(courseID);
        }
    }

    return -1;
}

// Display all registrations
void RegistrationManagement::displayRegistrations() {
    cout << "\n--- Course Registrations ---\n";

    if (registrations.empty()) {
        cout << "No registrations found.\n";
        return;
    }

    for (const Registration& reg : registrations) {
        cout << "Student ID: " << reg.studentID
             << " | Course ID: " << reg.courseID << endl;
    }
}