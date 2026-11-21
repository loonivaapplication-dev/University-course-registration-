#ifndef SEMINARCOURSE_H
#define SEMINARCOURSE_H

#include "Course.h"

// A Seminar course is a kind of Course (inheritance)
class SeminarCourse : public Course {

private:
    string venue;
    bool discussionBased;

public:
    SeminarCourse(const string& id, const string& name, int courseCredits,
               const string& teacher, int courseCapacity, int enrolled,
               const string& courseDay, const string& start, const string& end);

    string getCourseType() const override;
    void showFeatures() const override;
};

#endif
