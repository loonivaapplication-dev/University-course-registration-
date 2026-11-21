#ifndef LECTURECOURSE_H
#define LECTURECOURSE_H

#include "Course.h"

// A Lecture course is a kind of Course (inheritance)
class LectureCourse : public Course {

private:
    string venue;
    bool hasProjector;

public:
    LectureCourse(const string& id, const string& name, int courseCredits,
               const string& teacher, int courseCapacity, int enrolled,
               const string& courseDay, const string& start, const string& end);

    string getCourseType() const override;
    void showFeatures() const override;
};

#endif
