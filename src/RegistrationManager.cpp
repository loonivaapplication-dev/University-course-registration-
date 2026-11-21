#include "../include/RegistrationManager.h"
#include "../include/LectureCourse.h"
#include "../include/LabCourse.h"
#include "../include/SeminarCourse.h"
#include "../include/Input.h"

#include <fstream>
#include <iostream>

// Folder that holds students.txt, courses.txt and registrations.txt
#define DATA_DIR "data/"

RegistrationManager::RegistrationManager() {
}

RegistrationManager::~RegistrationManager() {
    clearAll();
}

void RegistrationManager::clearAll() {
    for (size_t i = 0; i < courses.size(); i++) {
        delete courses[i];
    }
    courses.clear();
    students.clear();
    registrations.clear();
}

// ------------------------------------
// PRIVATE HELPERS
// ------------------------------------

int RegistrationManager::findCourseIndex(const string& id) const {
    for (size_t i = 0; i < courses.size(); i++) {
        if (courses[i]->getCourseID() == id) return (int)i;
    }
    return -1;
}

int RegistrationManager::findStudentIndex(const string& id) const {
    for (size_t i = 0; i < students.size(); i++) {
        if (students[i].getUserID() == id) return (int)i;
    }
    return -1;
}

// Registration IDs look like R1, R2, ... and are never reused
string RegistrationManager::newRegistrationID() const {
    int n = (int)registrations.size() + 1;
    while (true) {
        string id = "R" + to_string(n);
        bool used = false;
        for (size_t i = 0; i < registrations.size(); i++) {
            if (registrations[i].getRegistrationID() == id) used = true;
        }
        if (!used) return id;
        n++;
    }
}

// True if two of this student's courses overlap
bool RegistrationManager::hasConflict(const string& studentID) const {
    vector<Course*> mine = getStudentCourses(studentID);
    for (size_t i = 0; i < mine.size(); i++) {
        for (size_t j = i + 1; j < mine.size(); j++) {
            if (mine[i]->conflictsWith(*mine[j])) return true;
        }
    }
    return false;
}

// ------------------------------------
// SAVE
// ------------------------------------

bool RegistrationManager::saveStudents() const {
    ofstream file(DATA_DIR "students.txt");
    if (!file.is_open()) {
        cout << "Error: Cannot open students.txt for writing.\n";
        return false;
    }
    for (size_t i = 0; i < students.size(); i++) {
        file << students[i].getUserID() << "|"
             << students[i].getName() << "|"
             << students[i].getEmail() << "\n";
    }
    file.close();
    if (file.fail()) {
        cout << "Error: Writing students.txt failed.\n";
        return false;
    }
    return true;
}

bool RegistrationManager::saveCourses() const {
    ofstream file(DATA_DIR "courses.txt");
    if (!file.is_open()) {
        cout << "Error: Cannot open courses.txt for writing.\n";
        return false;
    }
    for (size_t i = 0; i < courses.size(); i++) {
        const Course* c = courses[i];
        file << c->getCourseID() << "|"
             << c->getCourseName() << "|"
             << c->getCredits() << "|"
             << c->getInstructor() << "|"
             << c->getCapacity() << "|"
             << c->getEnrolledStudents() << "|"
             << c->getDay() << "|"
             << c->getStartTime() << "|"
             << c->getEndTime() << "|"
             << c->getCourseType() << "\n";
    }
    file.close();
    if (file.fail()) {
        cout << "Error: Writing courses.txt failed.\n";
        return false;
    }
    return true;
}

bool RegistrationManager::saveRegistrations() const {
    ofstream file(DATA_DIR "registrations.txt");
    if (!file.is_open()) {
        cout << "Error: Cannot open registrations.txt for writing.\n";
        return false;
    }
    for (size_t i = 0; i < registrations.size(); i++) {
        file << registrations[i].getRegistrationID() << "|"
             << registrations[i].getStudentID() << "|"
             << registrations[i].getCourseID() << "\n";
    }
    file.close();
    if (file.fail()) {
        cout << "Error: Writing registrations.txt failed.\n";
        return false;
    }
    return true;
}

// All three are attempted, even if one of them fails
bool RegistrationManager::saveToFile() const {
    bool a = saveStudents();
    bool b = saveCourses();
    bool c = saveRegistrations();
    if (!(a && b && c)) {
        cout << "Some data could not be saved.\n";
        return false;
    }
    return true;
}

// ------------------------------------
// LOAD
// ------------------------------------

bool RegistrationManager::loadStudents() {

    ifstream file(DATA_DIR "students.txt");
    if (!file.is_open()) {
        cout << "Warning: students.txt not found.\n";
        return false;
    }

    string line;
    bool valid = true;
    int lineNumber = 0;

    while (getline(file, line)) {
        lineNumber++;
        if (!line.empty() && line[line.size() - 1] == '\r') line.erase(line.size() - 1);
        if (line.empty()) continue;

        vector<string> f = splitFields(line);

        if (f.size() != 3 || f[0].empty() || findStudentIndex(f[0]) != -1) {
            cout << "Invalid student record skipped (line " << lineNumber << ").\n";
            valid = false;
            continue;
        }
        if ((int)students.size() >= MAX_STUDENTS) {
            cout << "Warning: too many students, the rest were ignored.\n";
            valid = false;
            break;
        }
        students.push_back(Student(f[0], f[1], f[2]));
    }

    cout << "Students loaded: " << students.size() << endl;
    return valid;
}

bool RegistrationManager::loadCourses() {

    ifstream file(DATA_DIR "courses.txt");
    if (!file.is_open()) {
        cout << "Warning: courses.txt not found.\n";
        return false;
    }

    string line;
    bool valid = true;
    int lineNumber = 0;

    while (getline(file, line)) {
        lineNumber++;
        if (!line.empty() && line[line.size() - 1] == '\r') line.erase(line.size() - 1);
        if (line.empty()) continue;

        vector<string> f = splitFields(line);

        // 9 fields = old format without a type (treated as a Lecture), 10 = with type
        int credits = 0, capacity = 0, enrolled = 0;
        Course* course = nullptr;

        if ((f.size() == 9 || f.size() == 10) &&
            parseNumber(f[2], credits) && parseNumber(f[4], capacity) &&
            parseNumber(f[5], enrolled)) {

            string type = (f.size() == 10) ? f[9] : "Lecture";
            course = createCourse(type, f[0], f[1], credits, f[3], capacity,
                                  enrolled, f[6], f[7], f[8]);
        }

        if (course == nullptr || !course->isValid() || findCourseIndex(f[0]) != -1) {
            cout << "Invalid course record skipped (line " << lineNumber << ").\n";
            delete course;
            valid = false;
            continue;
        }
        if ((int)courses.size() >= MAX_COURSES) {
            cout << "Warning: too many courses, the rest were ignored.\n";
            delete course;
            valid = false;
            break;
        }
        courses.push_back(course);
    }

    cout << "Courses loaded: " << courses.size() << endl;
    return valid;
}

bool RegistrationManager::loadRegistrations() {

    ifstream file(DATA_DIR "registrations.txt");
    if (!file.is_open()) {
        cout << "Warning: registrations.txt not found.\n";
        return false;
    }

    string line;
    bool valid = true;
    int lineNumber = 0;

    while (getline(file, line)) {
        lineNumber++;
        if (!line.empty() && line[line.size() - 1] == '\r') line.erase(line.size() - 1);
        if (line.empty()) continue;

        vector<string> f = splitFields(line);

        if (f.size() != 3 || f[0].empty() || f[1].empty() || f[2].empty()) {
            cout << "Invalid registration record skipped (line " << lineNumber << ").\n";
            valid = false;
            continue;
        }
        if ((int)registrations.size() >= MAX_REGISTRATIONS) {
            cout << "Warning: too many registrations, the rest were ignored.\n";
            valid = false;
            break;
        }
        registrations.push_back(Registration(f[0], f[1], f[2]));
    }

    cout << "Registrations loaded: " << registrations.size() << endl;
    return valid;
}

bool RegistrationManager::loadFromFile() {

    cout << "\nLoading saved data...\n";
    clearAll();

    bool a = loadStudents();
    bool b = loadCourses();
    bool c = loadRegistrations();

    if (!(a && b && c)) {
        cout << "Some data could not be loaded.\n";
        return false;
    }
    return true;
}

// ------------------------------------
// STUDENTS
// ------------------------------------

bool RegistrationManager::studentExists(const string& id) const {
    return findStudentIndex(id) != -1;
}

Student* RegistrationManager::findStudent(const string& id) {
    int index = findStudentIndex(id);
    if (index == -1) return nullptr;
    return &students[index];
}

bool RegistrationManager::addStudent(const Student& student) {
    if (student.getUserID().empty() || !safeText(student.getUserID()) ||
        !safeText(student.getName()) || !safeText(student.getEmail())) {
        cout << "Invalid student information (empty ID or '|' not allowed).\n";
        return false;
    }
    if (studentExists(student.getUserID())) {
        cout << "Student already exists!\n";
        return false;
    }
    if ((int)students.size() >= MAX_STUDENTS) {
        cout << "Student list is full!\n";
        return false;
    }

    students.push_back(student);
    cout << "Student added successfully!\n";
    return saveToFile();
}

// ------------------------------------
// COURSES
// ------------------------------------

// Creates the right kind of course for the given type, or nullptr if the type is unknown
Course* RegistrationManager::createCourse(const string& type, const string& id,
                                          const string& name, int credits,
                                          const string& instructor, int capacity,
                                          int enrolled, const string& day,
                                          const string& start, const string& end) const {
    string t = type;
    for (size_t i = 0; i < t.size(); i++) {
        if (t[i] >= 'A' && t[i] <= 'Z') t[i] = t[i] + ('a' - 'A');
    }

    if (t == "lecture") {
        return new LectureCourse(id, name, credits, instructor, capacity, enrolled, day, start, end);
    }
    if (t == "lab") {
        return new LabCourse(id, name, credits, instructor, capacity, enrolled, day, start, end);
    }
    if (t == "seminar") {
        return new SeminarCourse(id, name, credits, instructor, capacity, enrolled, day, start, end);
    }
    return nullptr;
}

bool RegistrationManager::courseExists(const string& id) const {
    return findCourseIndex(id) != -1;
}

Course* RegistrationManager::findCourse(const string& id) const {
    int index = findCourseIndex(id);
    if (index == -1) return nullptr;
    return courses[index];
}

bool RegistrationManager::addCourse(Course* course) {
    if (course == nullptr || !course->isValid()) {
        cout << "Invalid course information!\n";
        delete course;
        return false;
    }
    if (courseExists(course->getCourseID())) {
        cout << "Course already exists!\n";
        delete course;
        return false;
    }
    if ((int)courses.size() >= MAX_COURSES) {
        cout << "Course list is full!\n";
        delete course;
        return false;
    }

    courses.push_back(course);
    cout << "Course added successfully!\n";
    return saveToFile();
}

bool RegistrationManager::editCourse(const string& id, const string& name, int credits,
                                     const string& instructor, int capacity,
                                     const string& day, const string& start,
                                     const string& end) {
    Course* course = findCourse(id);
    if (course == nullptr) {
        cout << "Course not found!\n";
        return false;
    }

    if (!course->update(name, credits, instructor, capacity, day, start, end)) {
        cout << "Invalid course information (check capacity, day and times)!\n";
        return false;
    }

    cout << "Course updated successfully!\n";
    return saveToFile();
}

bool RegistrationManager::deleteCourse(const string& id) {
    int index = findCourseIndex(id);
    if (index == -1) {
        cout << "Course not found!\n";
        return false;
    }

    delete courses[index];
    courses.erase(courses.begin() + index);

    // Remove registrations of the deleted course so none point to a missing course
    for (size_t i = registrations.size(); i > 0; i--) {
        if (registrations[i - 1].getCourseID() == id) {
            registrations.erase(registrations.begin() + (i - 1));
        }
    }

    cout << "Course deleted successfully!\n";
    return saveToFile();
}

// ------------------------------------
// REGISTER / DROP
// ------------------------------------

bool RegistrationManager::registerCourse(const string& studentID, const string& courseID) {
    if (!studentExists(studentID)) {
        cout << "Error: Student does not exist!\n";
        return false;
    }

    Course* course = findCourse(courseID);
    if (course == nullptr) {
        cout << "Error: Course does not exist!\n";
        return false;
    }

    for (size_t i = 0; i < registrations.size(); i++) {
        if (registrations[i].getStudentID() == studentID &&
            registrations[i].getCourseID() == courseID) {
            cout << "Error: Duplicate registration!\n";
            return false;
        }
    }

    if (course->isFull()) {
        cout << "Error: No seats available!\n";
        return false;
    }
    if ((int)registrations.size() >= MAX_REGISTRATIONS) {
        cout << "Error: Registration list is full!\n";
        return false;
    }

    registrations.push_back(Registration(newRegistrationID(), studentID, courseID));
    course->enrollOne();

    cout << "Registration successful!\n";

    if (hasConflict(studentID)) {
        cout << "Warning: this course overlaps with another of your courses.\n";
    }

    return saveToFile();
}

bool RegistrationManager::dropCourse(const string& studentID, const string& courseID) {
    for (size_t i = 0; i < registrations.size(); i++) {
        if (registrations[i].getStudentID() == studentID &&
            registrations[i].getCourseID() == courseID) {

            registrations.erase(registrations.begin() + i);

            Course* course = findCourse(courseID);
            if (course != nullptr) course->dropOne();

            cout << "Course dropped successfully!\n";
            cout << "One seat is now available.\n";
            return saveToFile();
        }
    }

    cout << "Registration not found!\n";
    return false;
}

// All courses this student is registered in
vector<Course*> RegistrationManager::getStudentCourses(const string& studentID) const {
    vector<Course*> result;
    for (size_t i = 0; i < registrations.size(); i++) {
        if (registrations[i].getStudentID() != studentID) continue;

        Course* course = findCourse(registrations[i].getCourseID());
        if (course != nullptr) result.push_back(course);
    }
    return result;
}

// ------------------------------------
// DISPLAY
// ------------------------------------

void RegistrationManager::displayStudents() const {
    cout << "\n--- Students ---\n";
    if (students.empty()) {
        cout << "No students found.\n";
        return;
    }
    for (size_t i = 0; i < students.size(); i++) {
        students[i].displayInfo();
    }
}

void RegistrationManager::displayCourses() const {
    cout << "\n========== COURSE LIST ==========\n";
    if (courses.empty()) {
        cout << "No courses available.\n";
        return;
    }
    for (size_t i = 0; i < courses.size(); i++) {
        courses[i]->showDetails();   // each course type shows its own features
    }
}

void RegistrationManager::displayRegistrations() const {
    cout << "\n--- Course Registrations ---\n";
    if (registrations.empty()) {
        cout << "No registrations found.\n";
        return;
    }
    for (size_t i = 0; i < registrations.size(); i++) {
        registrations[i].showDetails();
    }
}
