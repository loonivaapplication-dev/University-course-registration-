#include "FileManager.h"

#include <fstream>
#include <iostream>

// Longest record has 7 fields, so 10 is plenty
const int MAX_FIELDS = 10;

// ------------------------------------
// PRIVATE HELPERS
// ------------------------------------

// A field must not contain the separator '|' or a line break
bool FileManager::isValidField(const string& value) const {

    return value.find('|') == string::npos &&
           value.find('\n') == string::npos &&
           value.find('\r') == string::npos;

}

// An ID must also not be empty
bool FileManager::isValidID(const string& value) const {

    return !value.empty() && isValidField(value);

}

// Split a line at every '|' and store the pieces in fields[].
// Empty pieces are kept (for example "S2|Ben|" gives 3 fields).
// Returns the number of fields, or maxFields + 1 if there are too many.
int FileManager::splitLine(const string& line, string fields[], int maxFields) const {

    int count = 0;
    size_t start = 0;

    while (true) {

        size_t pos = line.find('|', start);
        string piece;

        if (pos == string::npos) {
            piece = line.substr(start);
        }
        else {
            piece = line.substr(start, pos - start);
        }

        if (count >= maxFields) {
            return maxFields + 1;
        }

        fields[count] = piece;
        count++;

        if (pos == string::npos) {
            break;
        }

        start = pos + 1;

    }

    return count;

}

// Convert text such as "3" into the number 3.
// Returns false if the text is empty, too long or has a non-digit.
bool FileManager::parseNumber(const string& text, int& value) const {

    if (text.empty() || text.length() > 4) {
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

// ------------------------------------
// SAVE STUDENTS
// ------------------------------------

bool FileManager::saveStudents(const FileStudent students[], int count) {

    // Check every record BEFORE opening the file,
    // so a bad record cannot leave a half-written file
    for (int i = 0; i < count; i++) {

        if (!isValidID(students[i].studentID) ||
            !isValidField(students[i].name) ||
            !isValidField(students[i].email)) {

            cout << "Error: Invalid student information (ID: "
                 << students[i].studentID << ").\n";
            return false;

        }

    }

    ofstream file("students.txt");

    if (!file.is_open()) {
        cout << "Error: Cannot open students.txt for writing.\n";
        return false;
    }

    for (int i = 0; i < count; i++) {

        file << students[i].studentID << "|"
             << students[i].name << "|"
             << students[i].email << "\n";

    }

    file.close();

    if (file.fail()) {
        cout << "Error: Writing students.txt failed.\n";
        return false;
    }

    return true;

}

// ------------------------------------
// SAVE COURSES
// ------------------------------------

bool FileManager::saveCourses(const FileCourse courses[], int count) {

    for (int i = 0; i < count; i++) {

        if (!isValidID(courses[i].courseID) ||
            !isValidField(courses[i].courseName) ||
            !isValidField(courses[i].instructor) ||
            !isValidField(courses[i].day) ||
            !isValidField(courses[i].startTime) ||
            !isValidField(courses[i].endTime) ||
            courses[i].credits < 0) {

            cout << "Error: Invalid course information (ID: "
                 << courses[i].courseID << ").\n";
            return false;

        }

    }

    ofstream file("courses.txt");

    if (!file.is_open()) {
        cout << "Error: Cannot open courses.txt for writing.\n";
        return false;
    }

    for (int i = 0; i < count; i++) {

        file << courses[i].courseID << "|"
             << courses[i].courseName << "|"
             << courses[i].credits << "|"
             << courses[i].instructor << "|"
             << courses[i].day << "|"
             << courses[i].startTime << "|"
             << courses[i].endTime << "\n";

    }

    file.close();

    if (file.fail()) {
        cout << "Error: Writing courses.txt failed.\n";
        return false;
    }

    return true;

}

// ------------------------------------
// SAVE REGISTRATIONS
// ------------------------------------

bool FileManager::saveRegistrations(const FileRegistration registrations[], int count) {

    for (int i = 0; i < count; i++) {

        if (!isValidID(registrations[i].registrationID) ||
            !isValidID(registrations[i].studentID) ||
            !isValidID(registrations[i].courseID)) {

            cout << "Error: Invalid registration information (ID: "
                 << registrations[i].registrationID << ").\n";
            return false;

        }

    }

    ofstream file("registrations.txt");

    if (!file.is_open()) {
        cout << "Error: Cannot open registrations.txt for writing.\n";
        return false;
    }

    for (int i = 0; i < count; i++) {

        file << registrations[i].registrationID << "|"
             << registrations[i].studentID << "|"
             << registrations[i].courseID << "\n";

    }

    file.close();

    if (file.fail()) {
        cout << "Error: Writing registrations.txt failed.\n";
        return false;
    }

    return true;

}

// ------------------------------------
// LOAD STUDENTS
// ------------------------------------

bool FileManager::loadStudents(FileStudent students[], int& count) {

    ifstream file("students.txt");

    if (!file.is_open()) {
        cout << "Warning: students.txt not found.\n";
        return false;
    }

    count = 0;

    string line;
    string fields[MAX_FIELDS];
    bool valid = true;
    int lineNumber = 0;

    while (getline(file, line)) {

        lineNumber++;

        // Remove the extra '\r' that Windows files may have
        if (!line.empty() && line[line.length() - 1] == '\r') {
            line.erase(line.length() - 1);
        }

        if (line.empty()) continue;

        int fieldCount = splitLine(line, fields, MAX_FIELDS);

        if (fieldCount != 3 || !isValidID(fields[0])) {
            cout << "Invalid student record skipped (line " << lineNumber << ").\n";
            valid = false;
            continue;
        }

        if (count >= MAX_STUDENTS) {
            cout << "Warning: too many students, the rest were ignored.\n";
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

// ------------------------------------
// LOAD COURSES
// ------------------------------------

bool FileManager::loadCourses(FileCourse courses[], int& count) {

    ifstream file("courses.txt");

    if (!file.is_open()) {
        cout << "Warning: courses.txt not found.\n";
        return false;
    }

    count = 0;

    string line;
    string fields[MAX_FIELDS];
    bool valid = true;
    int lineNumber = 0;

    while (getline(file, line)) {

        lineNumber++;

        if (!line.empty() && line[line.length() - 1] == '\r') {
            line.erase(line.length() - 1);
        }

        if (line.empty()) continue;

        int fieldCount = splitLine(line, fields, MAX_FIELDS);
        int credits = 0;

        if (fieldCount != 7 || !isValidID(fields[0]) ||
            !parseNumber(fields[2], credits)) {

            cout << "Invalid course record skipped (line " << lineNumber << ").\n";
            valid = false;
            continue;

        }

        if (count >= MAX_COURSES) {
            cout << "Warning: too many courses, the rest were ignored.\n";
            valid = false;
            break;
        }

        courses[count].courseID = fields[0];
        courses[count].courseName = fields[1];
        courses[count].credits = credits;
        courses[count].instructor = fields[3];
        courses[count].day = fields[4];
        courses[count].startTime = fields[5];
        courses[count].endTime = fields[6];
        count++;

    }

    file.close();

    cout << "Courses loaded: " << count << endl;

    return valid;

}

// ------------------------------------
// LOAD REGISTRATIONS
// ------------------------------------

bool FileManager::loadRegistrations(FileRegistration registrations[], int& count) {

    ifstream file("registrations.txt");

    if (!file.is_open()) {
        cout << "Warning: registrations.txt not found.\n";
        return false;
    }

    count = 0;

    string line;
    string fields[MAX_FIELDS];
    bool valid = true;
    int lineNumber = 0;

    while (getline(file, line)) {

        lineNumber++;

        if (!line.empty() && line[line.length() - 1] == '\r') {
            line.erase(line.length() - 1);
        }

        if (line.empty()) continue;

        int fieldCount = splitLine(line, fields, MAX_FIELDS);

        if (fieldCount != 3 || !isValidID(fields[0]) ||
            !isValidID(fields[1]) || !isValidID(fields[2])) {

            cout << "Invalid registration record skipped (line " << lineNumber << ").\n";
            valid = false;
            continue;

        }

        if (count >= MAX_REGISTRATIONS) {
            cout << "Warning: too many registrations, the rest were ignored.\n";
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

// ------------------------------------
// SAVE ALL DATA
// ------------------------------------

bool FileManager::saveData(const FileStudent students[], int studentCount,
                           const FileCourse courses[], int courseCount,
                           const FileRegistration registrations[], int registrationCount) {

    cout << "\nSaving all data...\n";

    // All three are attempted, even if one of them fails
    bool studentsSaved = saveStudents(students, studentCount);
    bool coursesSaved = saveCourses(courses, courseCount);
    bool registrationsSaved = saveRegistrations(registrations, registrationCount);

    if (studentsSaved && coursesSaved && registrationsSaved) {
        cout << "\nAll data saved successfully!\n";
        return true;
    }

    cout << "\nSome data could not be saved.\n";
    return false;

}

// ------------------------------------
// LOAD ALL DATA
// ------------------------------------

bool FileManager::loadData(FileStudent students[], int& studentCount,
                           FileCourse courses[], int& courseCount,
                           FileRegistration registrations[], int& registrationCount) {

    cout << "\nLoading saved data...\n";

    bool studentsLoaded = loadStudents(students, studentCount);
    bool coursesLoaded = loadCourses(courses, courseCount);
    bool registrationsLoaded = loadRegistrations(registrations, registrationCount);

    if (studentsLoaded && coursesLoaded && registrationsLoaded) {
        cout << "\nAll data loaded successfully!\n";
        return true;
    }

    cout << "\nSome data could not be loaded.\n";
    return false;

}