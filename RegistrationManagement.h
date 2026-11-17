#ifndef REGISTRATIONMANAGEMENT_H
#define REGISTRATIONMANAGEMENT_H

#include <iostream>
#include <vector>
#include <string>

#include "FileManager.h"   // FileStudent, FileCourse, FileRegistration, FileManager
#include "Schedule.h"

using namespace std;

// courses.txt has no capacity column, so every course gets this many seats.
const int DEFAULT_CAPACITY = 30;

class RegistrationManagement {
private:
    vector<FileStudent> students;
    vector<FileCourse> courses;
    vector<FileRegistration> registrations;
    FileManager fileManager;

    int findStudent(const string& id) const;
    int findCourse(const string& id) const;
    int countRegisteredStudents(const string& courseID) const;
    string newRegistrationID() const;
    bool saveAll();

public:
    // Load everything from students.txt, courses.txt, registrations.txt
    void loadAll();

    bool studentExists(const string& studentID) const;
    bool courseExists(const string& courseID) const;

    bool addStudent(const FileStudent& student);

    bool addCourse(const FileCourse& course);
    bool editCourse(const FileCourse& course);   // matched by courseID
    bool deleteCourse(const string& courseID);   // also removes its registrations

    bool registerCourse(const string& studentID, const string& courseID);
    bool dropCourse(const string& studentID, const string& courseID);

    int getAvailableSeats(const string& courseID) const;

    void displayStudents() const;
    void displayCourses() const;
    void displayRegistrations() const;

    // Uses the Schedule class
    void viewSchedule(const string& studentID) const;
    bool checkScheduleConflict(const string& studentID) const;
};

#endif
