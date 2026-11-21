#include "../include/LabCourse.h"
#include <iostream>

LabCourse::LabCourse(const string& id, const string& name, int courseCredits,
                       const string& teacher, int courseCapacity, int enrolled,
                       const string& courseDay, const string& start, const string& end)
    : Course(id, name, courseCredits, teacher, courseCapacity, enrolled,
             courseDay, start, end) {
    venue = "Computer Lab";
    hasComputers = true;
}

string LabCourse::getCourseType() const {
    return "Lab";
}

void LabCourse::showFeatures() const {
    cout << "Features: " << venue << ", computers provided, hands-on work\n";
}
