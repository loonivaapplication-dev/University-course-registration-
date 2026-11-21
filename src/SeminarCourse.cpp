#include "../include/SeminarCourse.h"
#include <iostream>

SeminarCourse::SeminarCourse(const string& id, const string& name, int courseCredits,
                       const string& teacher, int courseCapacity, int enrolled,
                       const string& courseDay, const string& start, const string& end)
    : Course(id, name, courseCredits, teacher, courseCapacity, enrolled,
             courseDay, start, end) {
    venue = "Seminar Room";
    discussionBased = true;
}

string SeminarCourse::getCourseType() const {
    return "Seminar";
}

void SeminarCourse::showFeatures() const {
    cout << "Features: " << venue << ", discussion based, small groups\n";
}
