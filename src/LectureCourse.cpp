#include "../include/LectureCourse.h"
#include <iostream>

LectureCourse::LectureCourse(const string& id, const string& name, int courseCredits,
                       const string& teacher, int courseCapacity, int enrolled,
                       const string& courseDay, const string& start, const string& end)
    : Course(id, name, courseCredits, teacher, courseCapacity, enrolled,
             courseDay, start, end) {
    venue = "Lecture Hall";
    hasProjector = true;
}

string LectureCourse::getCourseType() const {
    return "Lecture";
}

void LectureCourse::showFeatures() const {
    cout << "Features: " << venue << ", projector available\n";
}
