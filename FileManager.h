
#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// Temporary Student structure
struct FileStudent {
    string studentID;
    string name;
    string email;
};

// Temporary Course structure
struct FileCourse {
    string courseID;
    string courseName;
    int credits;
    string instructor;
    string day;
    string startTime;
    string endTime;
};

// Temporary Registration structure
struct FileRegistration {
    string registrationID;
    string studentID;
    string courseID;
};

// FileManager class
class FileManager {

public:

    // Save all student information
    static bool saveStudents(
        const vector<FileStudent>& students
    );

    // Save all course information
    static bool saveCourses(
        const vector<FileCourse>& courses
    );

    // Save all registration information
    static bool saveRegistrations(
        const vector<FileRegistration>& registrations
    );

    // Load student information
    static bool loadStudents(
        vector<FileStudent>& students
    );

    // Load course information
    static bool loadCourses(
        vector<FileCourse>& courses
    );

    // Load registration information
    static bool loadRegistrations(
        vector<FileRegistration>& registrations
    );

    // Save all data
    static bool saveData(
        const vector<FileStudent>& students,
        const vector<FileCourse>& courses,
        const vector<FileRegistration>& registrations
    );

    // Load all data
    static bool loadData(
        vector<FileStudent>& students,
        vector<FileCourse>& courses,
        vector<FileRegistration>& registrations
    );

};

#endif

