#include "Schedule.h"

#include <iostream>

// ------------------------------------
// PRIVATE HELPERS
// ------------------------------------

// Convert text such as "09" into the number 9 (at most 2 digits).
bool Schedule::readNumber(const string& text, int& value) const {

    if (text.empty() || text.length() > 2) {
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

// Convert "HH:MM" into minutes since midnight. Returns -1 if invalid.
int Schedule::convertToMinutes(const string& timeText) const {

    size_t colon = timeText.find(':');

    if (colon == string::npos) {
        return -1;
    }

    string hoursText = timeText.substr(0, colon);
    string minutesText = timeText.substr(colon + 1);

    int hours = 0;
    int minutes = 0;

    if (minutesText.length() != 2 ||
        !readNumber(hoursText, hours) ||
        !readNumber(minutesText, minutes)) {
        return -1;
    }

    if (hours > 23 || minutes > 59) {
        return -1;
    }

    return hours * 60 + minutes;

}

// Convert minutes since midnight into "HH:MM"
string Schedule::formatTime(int totalMinutes) const {

    int hours = totalMinutes / 60;
    int minutes = totalMinutes % 60;

    string result = "";

    result += char('0' + hours / 10);
    result += char('0' + hours % 10);
    result += ':';
    result += char('0' + minutes / 10);
    result += char('0' + minutes % 10);

    return result;

}

// "monday" or "MONDAY" becomes "Monday"
string Schedule::normalizeDay(string day) const {

    for (size_t i = 0; i < day.length(); i++) {

        if (day[i] >= 'A' && day[i] <= 'Z') {
            day[i] = day[i] + ('a' - 'A');    // to lowercase
        }

    }

    if (!day.empty() && day[0] >= 'a' && day[0] <= 'z') {
        day[0] = day[0] - ('a' - 'A');        // first letter to uppercase
    }

    return day;

}

// Monday = 1 ... Sunday = 7, anything else = 8
int Schedule::getDayOrder(const string& day) const {

    if (day == "Monday") return 1;
    if (day == "Tuesday") return 2;
    if (day == "Wednesday") return 3;
    if (day == "Thursday") return 4;
    if (day == "Friday") return 5;
    if (day == "Saturday") return 6;
    if (day == "Sunday") return 7;

    return 8;

}

// True if course a should be shown AFTER course b
bool Schedule::comesAfter(const ScheduleCourse& a, const ScheduleCourse& b) const {

    int dayA = getDayOrder(a.day);
    int dayB = getDayOrder(b.day);

    if (dayA != dayB) {
        return dayA > dayB;
    }

    return convertToMinutes(a.startTime) > convertToMinutes(b.startTime);

}

// Check a course and add it to the schedule. Returns false if it was skipped.
bool Schedule::addCourse(ScheduleCourse course) {

    course.day = normalizeDay(course.day);

    int start = convertToMinutes(course.startTime);
    int end = convertToMinutes(course.endTime);

    if (getDayOrder(course.day) == 8 || start < 0 || end < 0 || end <= start) {
        cout << "Warning: skipped " << course.courseID
             << " (invalid day or time).\n";
        return false;
    }

    // Skip a course that is already in the schedule
    for (int i = 0; i < courseCount; i++) {

        if (registeredCourses[i].courseID == course.courseID) {
            cout << "Warning: skipped duplicate course "
                 << course.courseID << ".\n";
            return false;
        }

    }

    if (courseCount >= MAX_SCHEDULE_COURSES) {
        cout << "Warning: schedule is full, skipped "
             << course.courseID << ".\n";
        return false;
    }

    registeredCourses[courseCount] = course;
    courseCount++;

    return true;

}

// ------------------------------------
// CONSTRUCTOR
// ------------------------------------

Schedule::Schedule(const string& id) : studentID(id), courseCount(0) {
}

// ------------------------------------
// RETRIEVE COURSES
// ------------------------------------

void Schedule::retrieveCourses(const ScheduleCourse courses[], int count) {

    courseCount = 0;

    for (int i = 0; i < count; i++) {
        addCourse(courses[i]);
    }

    cout << "\nCourses retrieved successfully for student: " << studentID
         << " (" << courseCount << " course(s))\n";

}

void Schedule::retrieveCourses(const FileRegistration registrations[], int registrationCount,
                               const FileCourse courses[], int courseTotal) {

    courseCount = 0;

    for (int i = 0; i < registrationCount; i++) {

        // Only this student's registrations
        if (registrations[i].studentID != studentID) {
            continue;
        }

        // Find the matching course
        int found = -1;

        for (int j = 0; j < courseTotal; j++) {

            if (courses[j].courseID == registrations[i].courseID) {
                found = j;
                break;
            }

        }

        if (found == -1) {
            cout << "Warning: registration " << registrations[i].registrationID
                 << " refers to unknown course "
                 << registrations[i].courseID << ".\n";
            continue;
        }

        ScheduleCourse course;
        course.courseID = courses[found].courseID;
        course.courseName = courses[found].courseName;
        course.day = courses[found].day;
        course.startTime = courses[found].startTime;
        course.endTime = courses[found].endTime;

        addCourse(course);

    }

    cout << "\nCourses retrieved successfully for student: " << studentID
         << " (" << courseCount << " course(s))\n";

}

// ------------------------------------
// VIEW SCHEDULE
// ------------------------------------

void Schedule::viewSchedule() const {

    if (courseCount == 0) {
        cout << "\nNo registered courses found.\n";
        return;
    }

    // Sort a copy (bubble sort) so the original order stays unchanged
    ScheduleCourse sorted[MAX_SCHEDULE_COURSES];

    for (int i = 0; i < courseCount; i++) {
        sorted[i] = registeredCourses[i];
    }

    for (int i = 0; i < courseCount - 1; i++) {

        for (int j = 0; j < courseCount - 1 - i; j++) {

            if (comesAfter(sorted[j], sorted[j + 1])) {

                ScheduleCourse temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;

            }

        }

    }

    cout << "\n========================================\n";
    cout << "       STUDENT WEEKLY SCHEDULE\n";
    cout << "========================================\n";
    cout << "Student ID: " << studentID << "\n";

    string currentDay = "";

    for (int i = 0; i < courseCount; i++) {

        if (sorted[i].day != currentDay) {

            currentDay = sorted[i].day;
            cout << "\n--- " << currentDay << " ---\n";

        }

        cout << sorted[i].courseName
             << " (" << sorted[i].courseID << ")"
             << "\nTime: " << sorted[i].startTime
             << " - " << sorted[i].endTime
             << "\n\n";

    }

    cout << "========================================\n";

}

// ------------------------------------
// CHECK SCHEDULE CONFLICT
// ------------------------------------

bool Schedule::checkScheduleConflict() const {

    if (courseCount < 2) {
        cout << "\nNo schedule conflicts found.\n";
        return false;
    }

    bool conflictFound = false;

    cout << "\n========================================\n";
    cout << "       SCHEDULE CONFLICT CHECK\n";
    cout << "========================================\n";

    // Compare every pair of courses
    for (int i = 0; i < courseCount; i++) {

        for (int j = i + 1; j < courseCount; j++) {

            const ScheduleCourse& course1 = registeredCourses[i];
            const ScheduleCourse& course2 = registeredCourses[j];

            // Must be on the same day
            if (course1.day != course2.day) {
                continue;
            }

            int start1 = convertToMinutes(course1.startTime);
            int end1 = convertToMinutes(course1.endTime);
            int start2 = convertToMinutes(course2.startTime);
            int end2 = convertToMinutes(course2.endTime);

            // Overlap test (back-to-back classes are NOT a conflict)
            if (start1 < end2 && start2 < end1) {

                conflictFound = true;

                int overlapStart = start1;
                if (start2 > overlapStart) overlapStart = start2;

                int overlapEnd = end1;
                if (end2 < overlapEnd) overlapEnd = end2;

                cout << "\nCONFLICT DETECTED!\n";
                cout << "Day: " << course1.day << endl;
                cout << "Course 1: " << course1.courseName
                     << " (" << course1.courseID << ")" << endl;
                cout << "Course 2: " << course2.courseName
                     << " (" << course2.courseID << ")" << endl;
                cout << "Overlapping time: " << formatTime(overlapStart)
                     << " - " << formatTime(overlapEnd) << endl;

            }

        }

    }

    if (!conflictFound) {

        cout << "\nNo schedule conflicts found!"
             << "\nAll courses have different time slots.\n";

    }

    cout << "========================================\n";

    return conflictFound;

}