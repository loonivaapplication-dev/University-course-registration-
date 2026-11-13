// Stand-alone test program for Member 5's part (Schedule + FileManager).
#include <iostream>
#include "FileManager.h"
#include "Schedule.h"

int main() {

    FileStudent students[3] = {
        {"S1", "Ava", "ava@uni.edu"},
        {"S2", "Ben", ""},
        {"S3","Toothless", "tooduu@uni.edu"},                    // empty email on purpose
    };

    FileCourse courses[4] = {
        {"C1", "Math",    3, "Dr. A", "monday", "09:00", "10:30"},
        {"C2", "Physics", 3, "Dr. B", "Monday", "10:00", "11:30"},   // overlaps C1
        {"C3", "History", 2, "Dr. C", "Monday", "11:30", "12:30"},   // back-to-back with C2
        {"C4", "Art",     1, "Dr. D", "Friday", "08:00", "09:00"}
    };

    FileRegistration registrations[8] = {
        {"R1", "S1", "C1"}, {"R2", "S1", "C2"}, {"R3", "S1", "C3"},
        {"R4", "S1", "C4"}, {"R5", "S2", "C4"},
        {"R6", "S1", "C9"}, {"R7", "S3", "C2"}, {"R7", "S3", "C3"}    ,              // C9 does not exist
    };

    FileManager manager;
    manager.saveData(students, 2, courses, 4, registrations, 6);

    // Load into fresh arrays
    FileStudent loadedStudents[MAX_STUDENTS];
    FileCourse loadedCourses[MAX_COURSES];
    FileRegistration loadedRegistrations[MAX_REGISTRATIONS];
    int studentCount = 0, courseCount = 0, registrationCount = 0;

    manager.loadData(loadedStudents, studentCount,
                     loadedCourses, courseCount,
                     loadedRegistrations, registrationCount);

    Schedule schedule("S1");
    schedule.retrieveCourses(loadedRegistrations, registrationCount,
                             loadedCourses, courseCount);
    schedule.viewSchedule();
    schedule.checkScheduleConflict();

    Schedule schedule3("S3");

    schedule3.retrieveCourses(loadedRegistrations, registrationCount,
                          loadedCourses, courseCount);
    schedule3.viewSchedule();
    schedule3.checkScheduleConflict();

    return 0;
}