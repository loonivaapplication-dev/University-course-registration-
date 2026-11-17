#include "FileManager.h"
#include <fstream>
#include <iostream>

using namespace std;

const int MAX_FIELDS = 10;

// ==================================
// HELPER FUNCTIONS
// ==================================

// Check that a field does not contain | or a line break
bool FileManager::isValidField(const string& value) const {
    return value.find('|') == string::npos &&
           value.find('\n') == string::npos &&
           value.find('\r') == string::npos;
}

// Check that an ID is not empty
bool FileManager::isValidID(const string& value) const {
    return !value.empty() && isValidField(value);
}

// Split a line using |
int FileManager::splitLine(const string& line,
                           string fields[],
                           int maxFields) const {
    int count = 0;
    size_t start = 0;

    while (true) {
        size_t position = line.find('|', start);

        if (count >= maxFields) {
            return maxFields + 1;
        }

        if (position == string::npos) {
            fields[count] = line.substr(start);
        } else {
            fields[count] = line.substr(start, position - start);
        }

        count++;

        if (position == string::npos) {
            break;
        }

        start = position + 1;
    }

    return count;
}

// Convert a string into an integer
bool FileManager::parseNumber(const string& text, int& value) const {
    if (text.empty()) {
        return false;
    }

    value = 0;

    for (size_t i = 0; i < text.length(); i++) {
        if (text[i] < '0' || text[i] > '9') {
            return false;
        }

        value = value * 10 + (text[i] - '0');
    }

    return true;
}

// ==================================
// SAVE STUDENTS
// ==================================

bool FileManager::saveStudents(const FileStudent students[], int count) {

    if (count < 0 || count > MAX_STUDENTS) {
        return false;
    }

    ofstream file("students.txt");

    if (!file.is_open()) {
        cout << "Cannot open students.txt\n";
        return false;
    }

    for (int i = 0; i < count; i++) {

        if (!isValidID(students[i].studentID) ||
            !isValidField(students[i].name) ||
            !isValidField(students[i].email)) {

            cout << "Invalid student information.\n";
            return false;
        }

        file << students[i].studentID << "|"
             << students[i].name << "|"
             << students[i].email << "\n";
    }

    file.close();

    return !file.fail();
}

// ==================================
// SAVE COURSES
// ==================================

bool FileManager::saveCourses(const FileCourse courses[], int count) {

    if (count < 0 || count > MAX_COURSES) {
        return false;
    }

    ofstream file("courses.txt");

    if (!file.is_open()) {
        cout << "Cannot open courses.txt\n";
        return false;
    }

    for (int i = 0; i < count; i++) {

        if (!isValidID(courses[i].courseID) ||
            !isValidField(courses[i].courseName) ||
            !isValidField(courses[i].instructor) ||
            !isValidField(courses[i].day) ||
            !isValidField(courses[i].startTime) ||
            !isValidField(courses[i].endTime) ||
            courses[i].credits < 0 ||
            courses[i].capacity < 0 ||
            courses[i].enrolledStudents < 0 ||
            courses[i].enrolledStudents > courses[i].capacity) {

            cout << "Invalid course information.\n";
            return false;
        }

        // Save all 9 course fields
        file << courses[i].courseID << "|"
             << courses[i].courseName << "|"
             << courses[i].credits << "|"
             << courses[i].instructor << "|"
             << courses[i].capacity << "|"
             << courses[i].enrolledStudents << "|"
             << courses[i].day << "|"
             << courses[i].startTime << "|"
             << courses[i].endTime << "\n";
    }

    file.close();

    return !file.fail();
}

// ==================================
// SAVE REGISTRATIONS
// ==================================

bool FileManager::saveRegistrations(
    const FileRegistration registrations[], int count) {

    if (count < 0 || count > MAX_REGISTRATIONS) {
        return false;
    }

    ofstream file("registrations.txt");

    if (!file.is_open()) {
        cout << "Cannot open registrations.txt\n";
        return false;
    }

    for (int i = 0; i < count; i++) {

        if (!isValidID(registrations[i].registrationID) ||
            !isValidID(registrations[i].studentID) ||
            !isValidID(registrations[i].courseID)) {

            cout << "Invalid registration information.\n";
            return false;
        }

        file << registrations[i].registrationID << "|"
             << registrations[i].studentID << "|"
             << registrations[i].courseID << "\n";
    }

    file.close();

    return !file.fail();
}

// ==================================
// LOAD STUDENTS
// ==================================

bool FileManager::loadStudents(FileStudent students[], int& count) {

    ifstream file("students.txt");

    if (!file.is_open()) {
        cout << "students.txt not found.\n";
        count = 0;
        return false;
    }

    count = 0;
    string line;
    string fields[MAX_FIELDS];
    bool valid = true;

    while (getline(file, line)) {

        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        if (line.empty()) {
            continue;
        }

        int fieldCount = splitLine(line, fields, MAX_FIELDS);

        if (fieldCount != 3 || !isValidID(fields[0])) {
            cout << "Invalid student record skipped.\n";
            valid = false;
            continue;
        }

        if (count >= MAX_STUDENTS) {
            cout << "Maximum student limit reached.\n";
            valid = false;
            break;
        }

        students[count].studentID = fields[0];
        students[count].name = fields[1];
        students[count].email = fields[2];

        count++;
    }

    file.close();

    cout << "Students loaded: " << count << endl;

    return valid;
}

// ==================================
// LOAD COURSES
// ==================================

bool FileManager::loadCourses(FileCourse courses[], int& count) {

    ifstream file("courses.txt");

    if (!file.is_open()) {
        cout << "courses.txt not found.\n";
        count = 0;
        return false;
    }

    count = 0;
    string line;
    string fields[MAX_FIELDS];
    bool valid = true;

    while (getline(file, line)) {

        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        if (line.empty()) {
            continue;
        }

        int fieldCount = splitLine(line, fields, MAX_FIELDS);

        int credits;
        int capacity;
        int enrolledStudents;

        // A course must have exactly 9 fields
        if (fieldCount != 9 ||
            !isValidID(fields[0]) ||
            !parseNumber(fields[2], credits) ||
            !parseNumber(fields[4], capacity) ||
            !parseNumber(fields[5], enrolledStudents) ||
            enrolledStudents > capacity) {

            cout << "Invalid course record skipped.\n";
            valid = false;
            continue;
        }

        if (count >= MAX_COURSES) {
            cout << "Maximum course limit reached.\n";
            valid = false;
            break;
        }

        courses[count].courseID = fields[0];
        courses[count].courseName = fields[1];
        courses[count].credits = credits;
        courses[count].instructor = fields[3];
        courses[count].capacity = capacity;
        courses[count].enrolledStudents = enrolledStudents;
        courses[count].day = fields[6];
        courses[count].startTime = fields[7];
        courses[count].endTime = fields[8];

        count++;
    }

    file.close();

    cout << "Courses loaded: " << count << endl;

    return valid;
}

// ==================================
// LOAD REGISTRATIONS
// ==================================

bool FileManager::loadRegistrations(
    FileRegistration registrations[], int& count) {

    ifstream file("registrations.txt");

    if (!file.is_open()) {
        cout << "registrations.txt not found.\n";
        count = 0;
        return false;
    }

    count = 0;
    string line;
    string fields[MAX_FIELDS];
    bool valid = true;

    while (getline(file, line)) {

        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        if (line.empty()) {
            continue;
        }

        int fieldCount = splitLine(line, fields, MAX_FIELDS);

        if (fieldCount != 3 ||
            !isValidID(fields[0]) ||
            !isValidID(fields[1]) ||
            !isValidID(fields[2])) {

            cout << "Invalid registration record skipped.\n";
            valid = false;
            continue;
        }

        if (count >= MAX_REGISTRATIONS) {
            cout << "Maximum registration limit reached.\n";
            valid = false;
            break;
        }

        registrations[count].registrationID = fields[0];
        registrations[count].studentID = fields[1];
        registrations[count].courseID = fields[2];

        count++;
    }

    file.close();

    cout << "Registrations loaded: " << count << endl;

    return valid;
}

// ==================================
// SAVE ALL DATA
// ==================================

bool FileManager::saveData(
    const FileStudent students[], int studentCount,
    const FileCourse courses[], int courseCount,
    const FileRegistration registrations[], int registrationCount) {

    cout << "\nSaving all data...\n";

    bool studentsSaved = saveStudents(students, studentCount);
    bool coursesSaved = saveCourses(courses, courseCount);
    bool registrationsSaved =
        saveRegistrations(registrations, registrationCount);

    if (studentsSaved && coursesSaved && registrationsSaved) {
        cout << "All data saved successfully!\n";
        return true;
    }

    cout << "Some data could not be saved.\n";
    return false;
}

// ==================================
// LOAD ALL DATA
// ==================================

bool FileManager::loadData(
    FileStudent students[], int& studentCount,
    FileCourse courses[], int& courseCount,
    FileRegistration registrations[], int& registrationCount) {

    cout << "\nLoading saved data...\n";

    bool studentsLoaded = loadStudents(students, studentCount);
    bool coursesLoaded = loadCourses(courses, courseCount);
    bool registrationsLoaded =
        loadRegistrations(registrations, registrationCount);

    if (studentsLoaded && coursesLoaded && registrationsLoaded) {
        cout << "All data loaded successfully!\n";
        return true;
    }

    cout << "Some data could not be loaded.\n";
    return false;
}
