#include "RegistrationManagement.h"
#include "Input.h"

// ------------------------------------
// PRIVATE HELPERS
// ------------------------------------

int RegistrationManagement::findStudent(const string& id) const {
    for (size_t i = 0; i < students.size(); i++) {
        if (students[i].studentID == id) return (int)i;
    }
    return -1;
}

int RegistrationManagement::findCourse(const string& id) const {
    for (size_t i = 0; i < courses.size(); i++) {
        if (courses[i].courseID == id) return (int)i;
    }
    return -1;
}

// Registration IDs look like R1, R2, ... and are never reused
string RegistrationManagement::newRegistrationID() const {
    int n = (int)registrations.size() + 1;
    while (true) {
        string id = "R" + to_string(n);
        bool used = false;
        for (size_t i = 0; i < registrations.size(); i++) {
            if (registrations[i].registrationID == id) used = true;
        }
        if (!used) return id;
        n++;
    }
}

bool RegistrationManagement::saveAll() {
    return fileManager.saveData(
        students.data(), (int)students.size(),
        courses.data(), (int)courses.size(),
        registrations.data(), (int)registrations.size());
}

// ------------------------------------
// LOAD
// ------------------------------------

void RegistrationManagement::loadAll() {
    vector<FileStudent> s(MAX_STUDENTS);
    vector<FileCourse> c(MAX_COURSES);
    vector<FileRegistration> r(MAX_REGISTRATIONS);
    int sc = 0, cc = 0, rc = 0;

    fileManager.loadData(s.data(), sc, c.data(), cc, r.data(), rc);

    students.assign(s.begin(), s.begin() + sc);
    courses.assign(c.begin(), c.begin() + cc);
    registrations.assign(r.begin(), r.begin() + rc);
}

// ------------------------------------
// EXISTS
// ------------------------------------

bool RegistrationManagement::studentExists(const string& id) const {
    return findStudent(id) != -1;
}

bool RegistrationManagement::courseExists(const string& id) const {
    return findCourse(id) != -1;
}

// ------------------------------------
// STUDENTS
// ------------------------------------

bool RegistrationManagement::addStudent(const FileStudent& student) {
    if (student.studentID.empty() || !safeText(student.studentID) ||
        !safeText(student.name) || !safeText(student.email)) {
        cout << "Invalid student information (empty ID or '|' not allowed).\n";
        return false;
    }
    if (studentExists(student.studentID)) {
        cout << "Student already exists!\n";
        return false;
    }
    if ((int)students.size() >= MAX_STUDENTS) {
        cout << "Student list is full!\n";
        return false;
    }

    students.push_back(student);
    cout << "Student added successfully!\n";
    return saveAll();
}

// ------------------------------------
// COURSES
// ------------------------------------

bool RegistrationManagement::addCourse(const FileCourse& course) {
    if (course.courseID.empty() || !safeText(course.courseID) ||
        !safeText(course.courseName) || !safeText(course.instructor) ||
        !safeText(course.day) || !safeText(course.startTime) ||
        !safeText(course.endTime) || course.credits <= 0 ||
        course.capacity <= 0 || course.enrolledStudents < 0 ||
        course.enrolledStudents > course.capacity) {
        cout << "Invalid course information!\n";
        return false;
    }
    if (courseExists(course.courseID)) {
        cout << "Course already exists!\n";
        return false;
    }
    if ((int)courses.size() >= MAX_COURSES) {
        cout << "Course list is full!\n";
        return false;
    }

    courses.push_back(course);
    cout << "Course added successfully!\n";
    return saveAll();
}

bool RegistrationManagement::editCourse(const FileCourse& course) {
    int index = findCourse(course.courseID);
    if (index == -1) {
        cout << "Course not found!\n";
        return false;
    }
    if (!safeText(course.courseName) || !safeText(course.instructor) ||
        !safeText(course.day) || !safeText(course.startTime) ||
        !safeText(course.endTime) || course.credits <= 0 ||
        course.capacity < courses[index].enrolledStudents ||
        course.capacity <= 0) {
        cout << "Invalid course information (capacity must be positive and not below enrolled students)!\n";
        return false;
    }

    FileCourse updated = course;
    updated.enrolledStudents = courses[index].enrolledStudents;   // the count is managed by register/drop
    courses[index] = updated;
    cout << "Course updated successfully!\n";
    return saveAll();
}

bool RegistrationManagement::deleteCourse(const string& courseID) {
    int index = findCourse(courseID);
    if (index == -1) {
        cout << "Course not found!\n";
        return false;
    }

    courses.erase(courses.begin() + index);

    // Remove registrations of the deleted course so none point to a missing course
    for (size_t i = registrations.size(); i > 0; i--) {
        if (registrations[i - 1].courseID == courseID) {
            registrations.erase(registrations.begin() + (i - 1));
        }
    }

    cout << "Course deleted successfully!\n";
    return saveAll();
}

// ------------------------------------
// REGISTER / DROP
// ------------------------------------

bool RegistrationManagement::registerCourse(const string& studentID, const string& courseID) {
    if (!studentExists(studentID)) {
        cout << "Error: Student does not exist!\n";
        return false;
    }
    if (!courseExists(courseID)) {
        cout << "Error: Course does not exist!\n";
        return false;
    }

    for (size_t i = 0; i < registrations.size(); i++) {
        if (registrations[i].studentID == studentID &&
            registrations[i].courseID == courseID) {
            cout << "Error: Duplicate registration!\n";
            return false;
        }
    }

    if (getAvailableSeats(courseID) <= 0) {
        cout << "Error: No seats available!\n";
        return false;
    }

    if ((int)registrations.size() >= MAX_REGISTRATIONS) {
        cout << "Error: Registration list is full!\n";
        return false;
    }

    FileRegistration reg;
    reg.registrationID = newRegistrationID();
    reg.studentID = studentID;
    reg.courseID = courseID;
    registrations.push_back(reg);
    courses[findCourse(courseID)].enrolledStudents++;

    cout << "Registration successful!\n";

    if (checkScheduleConflict(studentID)) {
        cout << "Warning: this course overlaps with another of your courses.\n";
    }

    return saveAll();
}

bool RegistrationManagement::dropCourse(const string& studentID, const string& courseID) {
    for (size_t i = 0; i < registrations.size(); i++) {
        if (registrations[i].studentID == studentID &&
            registrations[i].courseID == courseID) {

            registrations.erase(registrations.begin() + i);

            int c = findCourse(courseID);
            if (c != -1 && courses[c].enrolledStudents > 0) {
                courses[c].enrolledStudents--;
            }

            cout << "Course dropped successfully!\n";
            cout << "One seat is now available.\n";
            return saveAll();
        }
    }

    cout << "Registration not found!\n";
    return false;
}

int RegistrationManagement::getAvailableSeats(const string& courseID) const {
    if (!courseExists(courseID)) {
        cout << "Course does not exist!\n";
        return -1;
    }
    return courses[findCourse(courseID)].capacity - courses[findCourse(courseID)].enrolledStudents;
}

// ------------------------------------
// DISPLAY
// ------------------------------------

void RegistrationManagement::displayStudents() const {
    cout << "\n--- Students ---\n";
    if (students.empty()) {
        cout << "No students found.\n";
        return;
    }
    for (size_t i = 0; i < students.size(); i++) {
        cout << "ID: " << students[i].studentID
             << " | Name: " << students[i].name
             << " | Email: " << students[i].email << endl;
    }
}

void RegistrationManagement::displayCourses() const {
    cout << "\n========== COURSE LIST ==========\n";
    if (courses.empty()) {
        cout << "No courses available.\n";
        return;
    }
    for (size_t i = 0; i < courses.size(); i++) {
        const FileCourse& c = courses[i];
        cout << "\nCourse ID: " << c.courseID
             << "\nCourse Name: " << c.courseName
             << "\nCredits: " << c.credits
             << "\nInstructor: " << c.instructor
             << "\nTime: " << c.day << " " << c.startTime << " - " << c.endTime
             << "\nAvailable Seats: " << (c.capacity - c.enrolledStudents)
             << " / " << c.capacity
             << "\n---------------------------------\n";
    }
}

void RegistrationManagement::displayRegistrations() const {
    cout << "\n--- Course Registrations ---\n";
    if (registrations.empty()) {
        cout << "No registrations found.\n";
        return;
    }
    for (size_t i = 0; i < registrations.size(); i++) {
        cout << "Registration: " << registrations[i].registrationID
             << " | Student ID: " << registrations[i].studentID
             << " | Course ID: " << registrations[i].courseID << endl;
    }
}

// ------------------------------------
// SCHEDULE (uses Schedule class)
// ------------------------------------

void RegistrationManagement::viewSchedule(const string& studentID) const {
    Schedule schedule(studentID);
    schedule.retrieveCourses(registrations.data(), (int)registrations.size(),
                             courses.data(), (int)courses.size());
    schedule.viewSchedule();
}

bool RegistrationManagement::checkScheduleConflict(const string& studentID) const {
    Schedule schedule(studentID);
    schedule.retrieveCourses(registrations.data(), (int)registrations.size(),
                             courses.data(), (int)courses.size());
    return schedule.checkScheduleConflict();
}