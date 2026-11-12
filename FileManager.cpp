
#include "FileManager.h"

#include <sstream>
#include <filesystem>
#include <cstdio>
#include <limits>

namespace fs = std::filesystem;

// Data directory
const string DATA_FOLDER = "data/";

// Check whether a field contains invalid separators
bool isValidField(const string& value) {

    return value.find('|') == string::npos &&
           value.find('\n') == string::npos &&
           value.find('\r') == string::npos;

}

// Split a line using the | separator
vector<string> splitLine(const string& line) {

    vector<string> fields;
    string field;
    stringstream ss(line);

    while (getline(ss, field, '|')) {
        fields.push_back(field);
    }

    return fields;

}

// Safely replace a file using a temporary file and backup
template <typename Writer>
bool writeFileSafely(const string& filename, Writer writer) {

    try {

        fs::create_directories(DATA_FOLDER);

        string tempFile = filename + ".tmp";
        string backupFile = filename + ".bak";

        // Write new information to a temporary file
        ofstream file(tempFile);

        if (!file.is_open()) {
            cout << "Error: Cannot create temporary file.\n";
            return false;
        }

        writer(file);

        file.close();

        if (!file) {
            fs::remove(tempFile);
            cout << "Error: Writing failed.\n";
            return false;
        }

        // Back up the existing file
        bool hadOriginal = fs::exists(filename);

        if (hadOriginal) {

            fs::remove(backupFile);
            fs::rename(filename, backupFile);

        }

        // Replace the original file
        try {

            fs::rename(tempFile, filename);

        }
        catch (...) {

            // Restore the original file if replacement fails
            if (hadOriginal && fs::exists(backupFile)) {
                fs::rename(backupFile, filename);
            }

            throw;

        }

        // Remove backup after successful replacement
        if (hadOriginal) {
            fs::remove(backupFile);
        }

        return true;

    }
    catch (const exception& e) {

        cout << "File error: " << e.what() << endl;
        return false;

    }

}

// ------------------------------------
// SAVE STUDENTS
// ------------------------------------

bool FileManager::saveStudents(
    const vector<FileStudent>& students
) {

    return writeFileSafely(
        DATA_FOLDER + "students.txt",
        [&](ofstream& file) {

            for (const auto& student : students) {

                if (!isValidField(student.studentID) ||
                    !isValidField(student.name) ||
                    !isValidField(student.email)) {

                    throw runtime_error("Invalid student information.");

                }

                file << student.studentID << "|"
                     << student.name << "|"
                     << student.email << "\n";

            }

        }
    );

}

// ------------------------------------
// SAVE COURSES
// ------------------------------------

bool FileManager::saveCourses(
    const vector<FileCourse>& courses
) {

    return writeFileSafely(
        DATA_FOLDER + "courses.txt",
        [&](ofstream& file) {

            for (const auto& course : courses) {

                if (!isValidField(course.courseID) ||
                    !isValidField(course.courseName) ||
                    !isValidField(course.instructor) ||
                    !isValidField(course.day) ||
                    !isValidField(course.startTime) ||
                    !isValidField(course.endTime) ||
                    course.credits < 0) {

                    throw runtime_error("Invalid course information.");

                }

                file << course.courseID << "|"
                     << course.courseName << "|"
                     << course.credits << "|"
                     << course.instructor << "|"
                     << course.day << "|"
                     << course.startTime << "|"
                     << course.endTime << "\n";

            }

        }
    );

}

// ------------------------------------
// SAVE REGISTRATIONS
// ------------------------------------

bool FileManager::saveRegistrations(
    const vector<FileRegistration>& registrations
) {

    return writeFileSafely(
        DATA_FOLDER + "registrations.txt",
        [&](ofstream& file) {

            for (const auto& registration : registrations) {

                if (!isValidField(registration.registrationID) ||
                    !isValidField(registration.studentID) ||
                    !isValidField(registration.courseID)) {

                    throw runtime_error("Invalid registration information.");

                }

                file << registration.registrationID << "|"
                     << registration.studentID << "|"
                     << registration.courseID << "\n";

            }

        }
    );

}

// ------------------------------------
// LOAD STUDENTS
// ------------------------------------

bool FileManager::loadStudents(
    vector<FileStudent>& students
) {

    ifstream file(DATA_FOLDER + "students.txt");

    if (!file.is_open()) {

        cout << "Warning: students.txt not found.\n";
        return false;

    }

    vector<FileStudent> loadedStudents;
    string line;
    bool valid = true;

    while (getline(file, line)) {

        if (line.empty()) continue;

        vector<string> fields = splitLine(line);

        if (fields.size() != 3) {

            cout << "Invalid student record skipped.\n";
            valid = false;
            continue;

        }

        loadedStudents.push_back({
            fields[0],
            fields[1],
            fields[2]
        });

    }

    if (file.bad()) {
        cout << "Error reading students.txt.\n";
        return false;
    }

    students.swap(loadedStudents);

    cout << "Students loaded: "
         << students.size() << endl;

    return valid;

}

// ------------------------------------
// LOAD COURSES
// ------------------------------------

bool FileManager::loadCourses(
    vector<FileCourse>& courses
) {

    ifstream file(DATA_FOLDER + "courses.txt");

    if (!file.is_open()) {

        cout << "Warning: courses.txt not found.\n";
        return false;

    }

    vector<FileCourse> loadedCourses;
    string line;
    bool valid = true;

    while (getline(file, line)) {

        if (line.empty()) continue;

        vector<string> fields = splitLine(line);

        if (fields.size() != 7) {

            cout << "Invalid course record skipped.\n";
            valid = false;
            continue;

        }

        try {

            size_t position = 0;

            int credits = stoi(fields[2], &position);

            if (position != fields[2].size() || credits < 0) {
                throw invalid_argument("Invalid credits");
            }

            loadedCourses.push_back({
                fields[0],
                fields[1],
                credits,
                fields[3],
                fields[4],
                fields[5],
                fields[6]
            });

        }
        catch (...) {

            cout << "Invalid course record skipped.\n";
            valid = false;

        }

    }

    if (file.bad()) {
        cout << "Error reading courses.txt.\n";
        return false;
    }

    courses.swap(loadedCourses);

    cout << "Courses loaded: "
         << courses.size() << endl;

    return valid;

}

// ------------------------------------
// LOAD REGISTRATIONS
// ------------------------------------

bool FileManager::loadRegistrations(
    vector<FileRegistration>& registrations
) {

    ifstream file(DATA_FOLDER + "registrations.txt");

    if (!file.is_open()) {

        cout << "Warning: registrations.txt not found.\n";
        return false;

    }

    vector<FileRegistration> loadedRegistrations;
    string line;
    bool valid = true;

    while (getline(file, line)) {

        if (line.empty()) continue;

        vector<string> fields = splitLine(line);

        if (fields.size() != 3) {

            cout << "Invalid registration record skipped.\n";
            valid = false;
            continue;

        }

        loadedRegistrations.push_back({
            fields[0],
            fields[1],
            fields[2]
        });

    }

    if (file.bad()) {
        cout << "Error reading registrations.txt.\n";
        return false;
    }

    registrations.swap(loadedRegistrations);

    cout << "Registrations loaded: "
         << registrations.size() << endl;

    return valid;

}

// ------------------------------------
// SAVE ALL DATA
// ------------------------------------

bool FileManager::saveData(
    const vector<FileStudent>& students,
    const vector<FileCourse>& courses,
    const vector<FileRegistration>& registrations
) {

    cout << "\nSaving all data...\n";

    bool studentsSaved = saveStudents(students);
    bool coursesSaved = saveCourses(courses);
    bool registrationsSaved = saveRegistrations(registrations);

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

bool FileManager::loadData(
    vector<FileStudent>& students,
    vector<FileCourse>& courses,
    vector<FileRegistration>& registrations
) {

    cout << "\nLoading saved data...\n";

    bool studentsLoaded = loadStudents(students);
    bool coursesLoaded = loadCourses(courses);
    bool registrationsLoaded = loadRegistrations(registrations);

    if (studentsLoaded && coursesLoaded && registrationsLoaded) {

        cout << "\nAll data loaded successfully!\n";
        return true;

    }

    cout << "\nSome data could not be loaded.\n";
    return false;

}

