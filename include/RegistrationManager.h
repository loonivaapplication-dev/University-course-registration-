#ifndef REGISTRATIONMANAGER_H
#define REGISTRATIONMANAGER_H

#include <string>
#include <vector>

#include "Course.h"
#include "Student.h"
#include "Registration.h"
using namespace std;

// Maximum number of records
const int MAX_STUDENTS = 200;
const int MAX_COURSES = 100;
const int MAX_REGISTRATIONS = 1000;

// RegistrationManager manages all students, courses and registrations
// and saves/loads them to the files in the data/ folder.
class RegistrationManager {

private:
    vector<Course*> courses;            // Course* so each element can be a Lecture/Lab/Seminar
    vector<Student> students;
    vector<Registration> registrations;

    int findCourseIndex(const string& id) const;
    int findStudentIndex(const string& id) const;
    string newRegistrationID() const;
    bool hasConflict(const string& studentID) const;
    void clearAll();

    bool saveStudents() const;
    bool saveCourses() const;
    bool saveRegistrations() const;
    bool loadStudents();
    bool loadCourses();
    bool loadRegistrations();

public:
    RegistrationManager();
    ~RegistrationManager();

    // The manager owns the Course objects, so it must not be copied
    RegistrationManager(const RegistrationManager&) = delete;
    RegistrationManager& operator=(const RegistrationManager&) = delete;

    // Files
    bool saveToFile() const;
    bool loadFromFile();

    // Students
    bool studentExists(const string& id) const;
    Student* findStudent(const string& id);
    bool addStudent(const Student& student);

    // Courses
    Course* createCourse(const string& type, const string& id, const string& name,
                         int credits, const string& instructor, int capacity,
                         int enrolled, const string& day,
                         const string& start, const string& end) const;
    bool courseExists(const string& id) const;
    Course* findCourse(const string& id) const;
    bool addCourse(Course* course);    // takes ownership (deletes it if it is rejected)
    bool editCourse(const string& id, const string& name, int credits,
                    const string& instructor, int capacity, const string& day,
                    const string& start, const string& end);
    bool deleteCourse(const string& id);   // also removes its registrations

    // Registration
    bool registerCourse(const string& studentID, const string& courseID);
    bool dropCourse(const string& studentID, const string& courseID);
    vector<Course*> getStudentCourses(const string& studentID) const;

    // Display
    void displayStudents() const;
    void displayCourses() const;
    void displayRegistrations() const;
};

#endif
