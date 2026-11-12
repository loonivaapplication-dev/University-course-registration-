
#include "Schedule.h"

#include <algorithm>
#include <iomanip>
#include <sstream>

// Convert HH:MM into minutes for easier comparison
int convertToMinutes(const string& time) {

    int hours = 0;
    int minutes = 0;

    char colon;

    stringstream ss(time);
    ss >> hours >> colon >> minutes;

    return (hours * 60) + minutes;
}

// Assign numerical values to weekdays
int getDayOrder(const string& day) {

    if (day == "Monday") return 1;
    if (day == "Tuesday") return 2;
    if (day == "Wednesday") return 3;
    if (day == "Thursday") return 4;
    if (day == "Friday") return 5;
    if (day == "Saturday") return 6;
    if (day == "Sunday") return 7;

    return 8;
}

// Constructor
Schedule::Schedule(string id) {

    studentID = id;

}

// Retrieve courses belonging to the student
void Schedule::retrieveCourses(
    const vector<ScheduleCourse>& allCourses
) {

    registeredCourses.clear();

    // For independent development, allCourses contains
    // the courses supplied for this student's schedule.

    registeredCourses = allCourses;

    cout << "\nCourses retrieved successfully for student: "
         << studentID << endl;

}

// Display the student's schedule
void Schedule::viewSchedule() const {

    if (registeredCourses.empty()) {

        cout << "\nNo registered courses found.\n";
        return;

    }

    // Create a copy so the original vector remains unchanged
    vector<ScheduleCourse> sortedCourses = registeredCourses;

    // Sort by weekday and then starting time
    sort(
        sortedCourses.begin(),
        sortedCourses.end(),
        [](const ScheduleCourse& a, const ScheduleCourse& b) {

            if (getDayOrder(a.day) != getDayOrder(b.day)) {
                return getDayOrder(a.day) < getDayOrder(b.day);
            }

            return convertToMinutes(a.startTime) <
                   convertToMinutes(b.startTime);

        }
    );

    cout << "\n========================================\n";
    cout << "       STUDENT WEEKLY SCHEDULE\n";
    cout << "========================================\n";

    cout << "Student ID: " << studentID << "\n\n";

    string currentDay = "";

    for (const auto& course : sortedCourses) {

        if (course.day != currentDay) {

            currentDay = course.day;

            cout << "\n--- " << currentDay << " ---\n";

        }

        cout << course.courseName
             << " (" << course.courseID << ")"
             << "\nTime: " << course.startTime
             << " - " << course.endTime
             << "\n\n";

    }

    cout << "========================================\n";

}

// Check for schedule conflicts
bool Schedule::checkScheduleConflict() const {

    bool conflictFound = false;

    if (registeredCourses.size() < 2) {

        cout << "\nNo schedule conflicts found.\n";
        return false;

    }

    cout << "\n========================================\n";
    cout << "       SCHEDULE CONFLICT CHECK\n";
    cout << "========================================\n";

    // Compare every pair of registered courses
    for (size_t i = 0; i < registeredCourses.size(); i++) {

        for (size_t j = i + 1; j < registeredCourses.size(); j++) {

            const ScheduleCourse& course1 = registeredCourses[i];
            const ScheduleCourse& course2 = registeredCourses[j];

            // Courses must occur on the same day
            if (course1.day != course2.day) {
                continue;
            }

            int start1 = convertToMinutes(course1.startTime);
            int end1 = convertToMinutes(course1.endTime);

            int start2 = convertToMinutes(course2.startTime);
            int end2 = convertToMinutes(course2.endTime);

            // Check whether the courses overlap
            if (start1 < end2 && start2 < end1) {

                conflictFound = true;

                int overlapStart = max(start1, start2);
                int overlapEnd = min(end1, end2);

                cout << "\nCONFLICT DETECTED!\n";

                cout << "Day: " << course1.day << endl;

                cout << "Course 1: "
                     << course1.courseName << endl;

                cout << "Course 2: "
                     << course2.courseName << endl;

                cout << "Overlapping time: "
                     << overlapStart / 60 << ":"
                     << setw(2) << setfill('0')
                     << overlapStart % 60
                     << " - "
                     << overlapEnd / 60 << ":"
                     << setw(2) << setfill('0')
                     << overlapEnd % 60
                     << setfill(' ') << endl;

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

