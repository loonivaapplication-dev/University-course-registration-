#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <string>
#include "FileManager.h"   // for FileCourse and FileRegistration (temporary)
using namespace std;

// A student can register for at most this many courses
const int MAX_SCHEDULE_COURSES = 20;

// TEMPORARY course type used by the schedule
struct ScheduleCourse {
    string courseID;
    string courseName;
    string day;        // "Monday" ... "Sunday"
    string startTime;  // "HH:MM" (24-hour)
    string endTime;    // "HH:MM" (24-hour)
};

class Schedule {
private:
    string studentID;
    ScheduleCourse registeredCourses[MAX_SCHEDULE_COURSES];
    int courseCount;

    // Helper functions
    bool readNumber(const string& text, int& value) const;
    int convertToMinutes(const string& timeText) const;
    string formatTime(int totalMinutes) const;
    string normalizeDay(string day) const;
    int getDayOrder(const string& day) const;
    bool comesAfter(const ScheduleCourse& a, const ScheduleCourse& b) const;
    bool addCourse(ScheduleCourse course);

public:
    Schedule(const string& id);

    // Independent-development version: the courses given belong to this student.
    void retrieveCourses(const ScheduleCourse courses[], int count);

    // Real version: pick this student's registrations and look up each course.
    void retrieveCourses(const FileRegistration registrations[], int registrationCount,
                         const FileCourse courses[], int courseCount);

    void viewSchedule() const;
    bool checkScheduleConflict() const;
};

#endif