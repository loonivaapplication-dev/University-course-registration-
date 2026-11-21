#ifndef COURSE_H
#define COURSE_H

#include <string>
using namespace std;

// Course is an abstract base class. It holds the data and behaviour that is
// common to every kind of course. LectureCourse, LabCourse and SeminarCourse
// inherit from it and provide their own getCourseType() and showFeatures().
class Course {

private:
    string courseID;
    string courseName;
    int credits;
    string instructor;
    int capacity;
    int enrolledStudents;
    string day;
    string startTime;
    string endTime;

    static string normalizeDay(string text);   // "monday" -> "Monday"

public:
    Course();
    Course(const string& id, const string& name, int courseCredits,
           const string& teacher, int courseCapacity, int enrolled,
           const string& courseDay, const string& start, const string& end);
    virtual ~Course() {}

    // Getters
    string getCourseID() const;
    string getCourseName() const;
    int getCredits() const;
    string getInstructor() const;
    int getCapacity() const;
    int getEnrolledStudents() const;
    int getAvailableSeats() const;
    string getDay() const;
    string getStartTime() const;
    string getEndTime() const;

    // Seats
    bool isFull() const;
    bool enrollOne();   // false if the course is full
    bool dropOne();     // false if nobody is enrolled

    // Validation and editing (all-or-nothing: nothing changes if the input is bad)
    bool isValid() const;
    bool update(const string& name, int newCredits, const string& teacher,
                int newCapacity, const string& newDay,
                const string& start, const string& end);

    // Schedule helpers
    int getDayOrder() const;        // Monday = 1 ... Sunday = 7, invalid = 8
    int getStartMinutes() const;    // minutes since midnight, -1 if invalid
    int getEndMinutes() const;
    bool conflictsWith(const Course& other) const;   // same day and overlapping time

    static int dayToOrder(const string& dayName);
    static int timeToMinutes(const string& timeText);
    static string formatTime(int totalMinutes);

    // Prints the common details, then calls showFeatures()
    void showDetails() const;

    // Every concrete course type must provide these (polymorphism)
    virtual string getCourseType() const = 0;
    virtual void showFeatures() const = 0;
};

#endif
