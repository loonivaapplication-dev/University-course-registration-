#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
using namespace std;

// Maximum number of records each array can hold
const int MAX_STUDENTS = 200;
const int MAX_COURSES = 100;
const int MAX_REGISTRATIONS = 1000;

// TEMPORARY record types for independent development.
// (The order of the members matters for the initial values used in main.)
struct FileStudent {
    string studentID;
    string name;
    string email;
};

struct FileCourse {
    string courseID;
    string courseName;
    int credits;
    string instructor;
    string day;
    string startTime;
    string endTime;
};

struct FileRegistration {
    string registrationID;
    string studentID;
    string courseID;
};

class FileManager {
private:
    // Helper functions (private, so they cannot clash with other members' code)
    bool isValidField(const string& value) const;
    bool isValidID(const string& value) const;
    int splitLine(const string& line, string fields[], int maxFields) const;
    bool parseNumber(const string& text, int& value) const;

public:
    // Save: write the first 'count' records of the array to a file
    bool saveStudents(const FileStudent students[], int count);
    bool saveCourses(const FileCourse courses[], int count);
    bool saveRegistrations(const FileRegistration registrations[], int count);

    // Load: fill the array and set 'count' to the number of records read
    bool loadStudents(FileStudent students[], int& count);
    bool loadCourses(FileCourse courses[], int& count);
    bool loadRegistrations(FileRegistration registrations[], int& count);

    bool saveData(const FileStudent students[], int studentCount,
                  const FileCourse courses[], int courseCount,
                  const FileRegistration registrations[], int registrationCount);

    bool loadData(FileStudent students[], int& studentCount,
                  FileCourse courses[], int& courseCount,
                  FileRegistration registrations[], int& registrationCount);
};

#endif