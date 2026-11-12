
#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Temporary course information for independent development
// This can later be adapted to the group's actual Course class.

struct ScheduleCourse {
    string courseID;
    string courseName;
    string day;
    string startTime;
    string endTime;
};

// Schedule class
class Schedule {

private:
    string studentID;
    vector<ScheduleCourse> registeredCourses;

public:

    // Constructor
    Schedule(string id);

    // Retrieve courses registered by a student
    void retrieveCourses(
        const vector<ScheduleCourse>& allCourses
    );

    // Display the student's weekly schedule
    void viewSchedule() const;

    // Check for overlapping courses
    bool checkScheduleConflict() const;

};

#endif

