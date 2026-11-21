#ifndef LABCOURSE_H
#define LABCOURSE_H

#include "Course.h"

// A Lab course is a kind of Course (inheritance)
class LabCourse : public Course {

private:
    string venue;
    bool hasComputers;

public:
    LabCourse(const string& id, const string& name, int courseCredits,
               const string& teacher, int courseCapacity, int enrolled,
               const string& courseDay, const string& start, const string& end);

    string getCourseType() const override;
    void showFeatures() const override;
};

#endif
