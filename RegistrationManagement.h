#ifndef REGISTRATIONMANAGEMENT_H
#define REGISTRATIONMANAGEMENT_H

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

using namespace std;

struct Student {
    string studentID;
    string studentName;
};

struct Course {
    string courseID;
    string courseName;
    int capacity;
};

struct Registration {
    string studentID;
    string courseID;
};

class RegistrationManagement {
private:
    vector<Student> students;
    vector<Course> courses;
    vector<Registration> registrations;

    bool studentExists(string studentID);
    bool courseExists(string courseID);
    int countRegisteredStudents(string courseID);

public:
    void addStudent(Student student);
    void addCourse(Course course);

    void registerCourse(string studentID, string courseID);
    void dropCourse(string studentID, string courseID);

    int getAvailableSeats(string courseID);

    void displayRegistrations();
};

#endif